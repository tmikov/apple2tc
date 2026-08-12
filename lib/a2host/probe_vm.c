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
/// Only OP_END, OP_PUSH_LIT and OP_PRINTF are implemented here. Every other
/// opcode hits the `default:` case in probe_vm_run() and reports clearly --
/// later tasks add them to this same loop.

#include "probe_internal.h"

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
