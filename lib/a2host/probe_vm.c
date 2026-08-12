/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The probe stack machine. See probe_internal.h for the bytecode contract --
/// in particular the pop order of non-commutative opcodes and the argument
/// order of OP_PRINTF, both of which a VM can get backwards while passing
/// every compiler test, because nothing in part 1 executed bytecode.
///
/// OP_END, OP_PUSH_LIT, OP_PRINTF (Task 1), the arithmetic, bitwise,
/// comparison and unary opcodes (Task 2), counters, parameters, registers and
/// control flow (Task 3), memory reads and hashing (Task 4), and now stop and
/// key delivery (Task 5) are implemented here -- every opcode probe_parse.c
/// can emit.

#include "probe_internal.h"

#include "apple2tc/a2engine.h" // get_regs/regs_t, for OP_LOAD_REG

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

enum { PROBE_STACK_SIZE = 512 };

static uint32_t s_stack[PROBE_STACK_SIZE];
static unsigned s_sp;

/// The address dispatch reached. LOAD_REG REG_PC reads this rather than
/// get_regs().pc so both engines go through the same source: in a generated
/// program get_regs().pc is whatever the last CYCLES stored for the *block*,
/// which is this same address only as long as nothing inside the block has
/// advanced it -- OP_LOAD_REG REG_PC must not depend on that. In the
/// interpreter get_regs().pc does happen to equal the dispatch address too
/// (Emu6502::runFor calls the debug callback with pc_ before decoding the
/// instruction at it, so pc_ has not moved yet -- see emu6502.cpp's main
/// loop), but relying on that per-engine coincidence instead of this shared
/// variable is exactly the kind of asymmetry that would only show up as a
/// disagreement between the two, not as a bug in either one alone.
static uint16_t s_dispatch_pc;

/// Counter storage, indexed by the same slot a counter got in the compiler's
/// symbol table (script_t::counters). Lives in the VM rather than in
/// script_t: script_t is the compiled *program* -- reloading a script (the
/// `only one probe script may be loaded` check in probe.c means that never
/// happens today, but nothing here should assume it never will) should not
/// require deciding whether counter state carries over, and a plain array
/// here sidesteps that question entirely by always starting fresh in
/// probe_vm_init_counters.
static uint32_t s_counters[PROBE_MAX_COUNTERS];

/// Set by OP_STOP, read by probe_stop_requested(). Never cleared: once a run
/// asks to stop, nothing in this process runs another frame -- both
/// a2host_run_headless and a2host_gui.c's frame_cb exit (the latter via
/// sapp_request_quit()) the moment they observe it, so there is no "next run"
/// in the same process for a stale true to leak into.
static bool s_stop_requested;

static void vm_push(uint32_t v) {
  if (s_sp == PROBE_STACK_SIZE)
    probe_fatal("probe stack overflow");
  s_stack[s_sp++] = v;
}

static uint32_t vm_pop(void) {
  if (s_sp == 0)
    probe_fatal("probe stack underflow");
  return s_stack[--s_sp];
}

/// Render one conversion. Only %d %u %x %X reach here -- count_conversions
/// rejected everything else at compile time -- so this handles exactly the
/// flags and width it validated, and nothing about it is locale-sensitive.
static void emit_conv(
    FILE *f,
    uint32_t v,
    char conv,
    unsigned width,
    bool zero,
    bool left,
    bool plus,
    bool space) {
  char digits[16];
  unsigned n = 0;
  bool negative = false;
  uint32_t mag = v;

  if (conv == 'd' && (int32_t)v < 0) {
    negative = true;
    mag = (uint32_t)(-(int64_t)(int32_t)v);
  }

  unsigned base = (conv == 'x' || conv == 'X') ? 16 : 10;
  const char *alpha = (conv == 'X') ? "0123456789ABCDEF" : "0123456789abcdef";
  do {
    digits[n++] = alpha[mag % base];
    mag /= base;
  } while (mag);

  char sign = negative ? '-' : (conv == 'd' && plus) ? '+' : (conv == 'd' && space) ? ' ' : 0;
  unsigned len = n + (sign ? 1u : 0u);

  // Zero-padding goes after the sign; left-justification wins over it, as in C.
  if (!left && zero && width > len) {
    if (sign)
      fputc(sign, f);
    for (unsigned i = len; i < width; ++i)
      fputc('0', f);
    sign = 0;
  } else if (!left) {
    for (unsigned i = len; i < width; ++i)
      fputc(' ', f);
  }
  if (sign)
    fputc(sign, f);
  while (n)
    fputc(digits[--n], f);
  if (left)
    for (unsigned i = len; i < width; ++i)
      fputc(' ', f);
}

/// Render one format string against argc values already in source order.
/// Precondition: \p fmt passed count_conversions (probe_parse.c) at compile
/// time -- every `%` is either `%%` or one of the four conversions this
/// understands, never a trailing `%` at end-of-string. A hand-corrupted
/// script (or a future caller that skips the compiler) violating that would
/// walk `p` past the format's NUL the moment `*p` is dereferenced after an
/// unmatched `%`. `static`: probe_vm_run below is the only caller.
static void probe_vm_printf(const char *fmt, const uint32_t *args, unsigned argc) {
  FILE *f = probe_out();
  unsigned ai = 0;
  for (const char *p = fmt; *p; ++p) {
    if (*p != '%') {
      fputc(*p, f);
      continue;
    }
    ++p;
    if (*p == '%') {
      fputc('%', f);
      continue;
    }
    bool zero = false, left = false, plus = false, space = false;
    for (;; ++p) {
      if (*p == '0')
        zero = true;
      else if (*p == '-')
        left = true;
      else if (*p == '+')
        plus = true;
      else if (*p == ' ')
        space = true;
      else
        break;
    }
    unsigned width = 0;
    while (*p >= '0' && *p <= '9')
      width = width * 10 + (unsigned)(*p++ - '0');
    if (ai == argc)
      probe_fatal("probe VM: format '%s' wants more arguments than were pushed", fmt);
    emit_conv(f, args[ai++], *p, width, zero, left, plus, space);
  }
}

/// FNV-1a over the \p start..\p end inclusive byte range, read through
/// ram_peek. This is the one value in the whole design that both engines
/// compute from the *same* source file (this one) rather than from
/// independently maintained code, which is exactly why it has to be this
/// dumb: a probe script cannot see its own hash go wrong. If probe_vm.c
/// reversed the byte order, used a different constant, or (worse) let
/// something platform-dependent -- e.g. `int`'s width, or a signed-overflow
/// UB shift -- leak into the result, the interpreter and the generated
/// program would still compute the identical wrong value, and Task 7's
/// acceptance diff would pass anyway. FNV-1a is chosen for being fully
/// specified bit-for-bit (unlike, say, hashing raw bytes through a
/// std::hash-shaped API) and endianness-free (it consumes one byte at a
/// time, so it does not care how a 16-/32-bit load would be laid out on a
/// given platform) -- there is nothing left in it for two conforming C
/// implementations to disagree about. `static`: probe_vm_run below is the
/// only caller.
static uint32_t probe_vm_hash(uint16_t start, uint16_t end) {
  uint32_t h = 2166136261u;
  // `a` is uint32_t, not uint16_t, on purpose: hash($0000, $FFFF) requires
  // `a` to reach 0x10000 to fall out of the loop after hashing byte $FFFF.
  // A uint16_t `a` would wrap from 0xFFFF back to 0x0000 and never compare
  // greater than `end` -- an infinite loop for exactly the range a script is
  // most likely to want to hash (all of memory).
  for (uint32_t a = start; a <= end; ++a) {
    h ^= ram_peek((uint16_t)a);
    h *= 16777619u;
  }
  return h;
}

void probe_vm_run(const script_t *sc, uint32_t ip) {
  // The initializers push nparams values here and the body indexes them as
  // slots 0..nparams-1 relative to this base. See probe_t::init_offset.
  unsigned frame_base = s_sp;

  for (;;) {
    // Safe today only by accident: code[] sits inside script_t, which is
    // calloc'd, so an unguarded out-of-range ip would read zeros and
    // OP_END == 0 would stop it. That accident stops covering ip once a
    // later task adds JMP/JZ/JNZ, whose absolute targets come out of the
    // bytecode itself rather than off this loop's own increment.
    if (ip >= sc->ncode)
      probe_fatal("corrupt probe bytecode: ip %u is out of range (ncode=%u)", ip, sc->ncode);
    opcode_t op = (opcode_t)sc->code[ip++];
    switch (op) {
    case OP_END:
      s_sp = frame_base; // discard the parameter frame
      return;

    case OP_PUSH_LIT:
      vm_push(sc->code[ip++]);
      break;

    case OP_PRINTF: {
      uint32_t fmt = sc->code[ip++];
      uint32_t argc = sc->code[ip++];
      uint32_t args[PROBE_MAX_PRINTF_ARGS];
      if (argc > PROBE_MAX_PRINTF_ARGS)
        probe_fatal("printf with %u arguments exceeds the limit", argc);
      // Pushed in source order, so the last argument is on top: fill
      // backwards to recover source order.
      for (uint32_t i = argc; i-- > 0;)
        args[i] = vm_pop();
      probe_vm_printf(sc->formats[fmt], args, argc);
      break;
    }

// Both operands are pushed in source order (probe_internal.h), so the right
// operand is on top: pop it first. Each invocation below expands into its
// own `case` with its own braces, so `r`/`l` never collide with another
// invocation's `r`/`l`, or with anything outside the switch.
#define BINOP(name, expr)  \
  case name: {             \
    uint32_t r = vm_pop(); \
    uint32_t l = vm_pop(); \
    vm_push(expr);         \
    break;                 \
  }

      BINOP(OP_ADD, l + r)
      BINOP(OP_SUB, l - r)
      BINOP(OP_MUL, l * r)
      BINOP(OP_AND, l & r)
      BINOP(OP_OR, l | r)
      BINOP(OP_XOR, l ^ r)
      // 0/1, exactly what C's relational/equality operators already give --
      // OP_JZ (Task 3) tests a popped value against 0, and its lowering of
      // `&&`/`||` depends on that being the whole range of results a
      // comparison can produce.
      BINOP(OP_EQ, l == r)
      BINOP(OP_NE, l != r)
      BINOP(OP_LT, l < r)
      BINOP(OP_LE, l <= r)
      BINOP(OP_GT, l > r)
      BINOP(OP_GE, l >= r)
#undef BINOP

    case OP_DIV:
    case OP_MOD: {
      uint32_t r = vm_pop();
      uint32_t l = vm_pop();
      // A script's divide-by-zero is a bug in the script, not in the machine
      // being probed. Defining it as some value (say, 0) would let that bug
      // produce a report that reads as clean -- the exact failure mode this
      // facility exists to rule out -- so it is fatal instead, like a stack
      // overflow above.
      if (r == 0)
        probe_fatal("probe VM: division by zero");
      vm_push(op == OP_DIV ? l / r : l % r);
      break;
    }
    case OP_SHL:
    case OP_SHR: {
      uint32_t r = vm_pop();
      uint32_t l = vm_pop();
      // A shift of 32 or more is undefined in C, and probe_vm.c is one
      // implementation shared by every engine -- unlike the interpreter vs.
      // generated-code split elsewhere, there is no second copy of this file
      // to disagree with. Left undefined, the *same* source could still
      // legally return different values on different compilers/platforms
      // (Windows/LLP64 is a supported target), which is indistinguishable
      // from a real bug once it shows up as noise in a report. Defining it
      // as zero -- consistent with "shifted every bit out" -- removes that
      // platform dependence; the 6502 itself has no variable-count shift to
      // be bit-compatible with, so nothing is owed to hardware behaviour
      // here.
      uint32_t v = r >= 32 ? 0u : (op == OP_SHL ? l << r : l >> r);
      vm_push(v);
      break;
    }
    case OP_NOT:
      vm_push(vm_pop() == 0);
      break;
    case OP_BITNOT:
      vm_push(~vm_pop());
      break;
    case OP_NEG:
      // Unsigned subtraction is fully defined for every operand, including
      // 0x80000000: the result wraps to 0x80000000 itself, which as %d's
      // (int32_t) is INT32_MIN -- the same value two's-complement negation
      // of INT32_MIN produces elsewhere, reached here without the signed
      // overflow that spelling it as `-(int32_t)v` would risk.
      vm_push((uint32_t)(0u - vm_pop()));
      break;

    case OP_LOAD_PARAM:
      // frame_base + idx, not s_sp-relative: see probe_t::init_offset in
      // probe_internal.h. The compiler emits idx values that only ever
      // address this probe's own frame, so no bounds check against
      // PROBE_MAX_PARAMS is needed beyond the ip range check already above.
      vm_push(s_stack[frame_base + sc->code[ip++]]);
      break;
    case OP_STORE_PARAM: {
      uint32_t idx = sc->code[ip++];
      s_stack[frame_base + idx] = vm_pop();
      break;
    }
    case OP_LOAD_COUNTER:
      vm_push(s_counters[sc->code[ip++]]);
      break;
    case OP_STORE_COUNTER: {
      uint32_t idx = sc->code[ip++];
      s_counters[idx] = vm_pop();
      break;
    }
    case OP_LOAD_REG: {
      regs_t r = get_regs();
      switch ((reg_t)sc->code[ip++]) {
      case REG_A:
        vm_push(r.a);
        break;
      case REG_X:
        vm_push(r.x);
        break;
      case REG_Y:
        vm_push(r.y);
        break;
      case REG_SP:
        vm_push(r.sp);
        break;
      case REG_SR:
        vm_push(r.status);
        break;
      // Not r.pc -- see s_dispatch_pc's comment above the file-scope
      // declaration for why the two are not interchangeable in a generated
      // program even though they agree in the interpreter.
      case REG_PC:
        vm_push(s_dispatch_pc);
        break;
      }
      break;
    }
    case OP_PEEK8:
      // ram_peek, never peek (a2engine.h's other read): inspecting a machine
      // must not alter it. peek() decodes the IO range and calls ioPeek(),
      // which trips the soft switches at $C0xx -- a probe reading through it
      // would change the very machine it is trying to observe. ram_peek() is
      // a plain array index (see Emu6502::ram_peek / the generated engines'
      // ram_peek in system-inc.h and system2-inc.h) with no such branch, so
      // this is safe by construction for every address, IO range included.
      vm_push(ram_peek((uint16_t)vm_pop()));
      break;
    case OP_PEEK16:
      // ram_peek16, not two OP_PEEK8-style ram_peek() calls open-coded here:
      // both engines already define the 16-bit, low-byte-first (6502 /
      // little-endian) convention once each -- system-inc.h, system2-inc.h,
      // and Emu6502::ram_peek16 in emu6502.h -- and calling it keeps the VM
      // from carrying a fourth copy of that byte order that could drift from
      // the other three.
      vm_push(ram_peek16((uint16_t)vm_pop()));
      break;
    case OP_HASH: {
      // hash(start, end): start is pushed first, so end pops first (see
      // OP_HASH's comment in probe_internal.h).
      uint32_t end = vm_pop();
      uint32_t start = vm_pop();
      vm_push(probe_vm_hash((uint16_t)start, (uint16_t)end));
      break;
    }
    case OP_JMP:
      // Overwrites ip with the target, so there is nothing to advance past.
      // Do not "fix" this to `ip = sc->code[ip++]`: that assigns to ip and
      // reads it (for the postfix ++) with no sequence point between them,
      // which is undefined behaviour in C, not merely a style nit -- unlike
      // the JZ/JNZ cases below, which read the operand with `ip++` and only
      // assign to ip afterwards, in a separate statement.
      ip = sc->code[ip];
      break;
    case OP_JZ: {
      uint32_t target = sc->code[ip++];
      if (vm_pop() == 0)
        ip = target;
      break;
    }
    case OP_JNZ: {
      uint32_t target = sc->code[ip++];
      if (vm_pop() != 0)
        ip = target;
      break;
    }

    case OP_KEY:
      // Host state (the key list, the IO queue) lives in a2host.c, not here
      // -- see probe_deliver_keys's comment in probe_internal.h.
      probe_deliver_keys(vm_pop());
      break;

    case OP_STOP:
      // Cannot longjmp or return out of here: for the interpreter this call
      // is nested inside Emu6502::runFor's instruction loop, and for a
      // generated program it is nested inside a basic block's straight-line
      // C, mid-CYCLES -- neither has anywhere to unwind to. Set the flag and
      // let the host notice once run_emulated() returns control to it.
      s_stop_requested = true;
      break;

    default: {
      // opname() returns NULL only for a code[] cell that isn't one of the
      // opcode_t enumerators at all -- corrupt bytecode, distinct from an
      // opcode that exists but this task hasn't implemented yet.
      const char *name = opname(op);
      if (name)
        probe_fatal("probe VM: opcode %u (%s) is not implemented", (unsigned)op, name);
      else
        probe_fatal("corrupt probe bytecode: opcode %u is not valid", (unsigned)op);
    }
    }
  }
}

void probe_vm_set_pc(uint16_t pc) {
  s_dispatch_pc = pc;
}

void probe_vm_init_counters(const script_t *sc) {
  for (unsigned i = 0; i != sc->ncounters; ++i)
    s_counters[i] = sc->counters[i].init;
}

bool probe_stop_requested(void) {
  return s_stop_requested;
}
