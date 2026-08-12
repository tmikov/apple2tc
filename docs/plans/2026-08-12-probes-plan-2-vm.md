# Probe VM Implementation Plan (phase 1, part 2 of 2)

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Execute the bytecode part 1 compiles, in both engines, and prove it by
reproducing the trusted 1,991/1,991 ROM-boot control-flow result through probes.

**Architecture:** A stack machine in `lib/a2host/probe_vm.c`, reached two ways:
`debugCB` in the interpreter and the `CYCLES` macro in the two generated-code
headers. Both call `probe_dispatch(pc)`, which walks the install chain for that
address and runs each probe's bytecode. Because `CYCLES` lives in a shared
header rather than generated text, **existing decompiled programs gain probes on
a plain recompile** — no regeneration, no decompiler change.

**Tech Stack:** C11, built into the existing `a2host` library. Tests are shell +
`diff`, following `tests/run-tests.sh`.

**Spec:** `docs/plans/2026-08-11-probes-design.md`. Part 1:
`docs/plans/2026-08-11-probes-plan-1-compiler.md`. Language:
`docs/probes.md`.

---

## What part 1 left you

Read `lib/a2host/probe_internal.h` first — its opcode comments are the contract
and they are precise about the two things easiest to get wrong:

- **Pop order.** Both operands are pushed in source order, so the *right*
  operand is on top. Every non-commutative opcode pops right first, then left,
  and computes `left <op> right`. `OP_HASH` follows the same rule:
  `hash(start, end)` pushes `start` first, so `end` pops first.
- **`OP_PRINTF` argument order.** Arguments are pushed in source order, so the
  *last* one is on top. Popping top-first yields them backwards. Collect `argc`
  values and index from `argc-1` down, or fill an array in reverse.

Also available: `probe_find_slot(sc, addr)` (precondition: `sc->slots != NULL`),
`probe_fatal(fmt, ...)` and `probe_error(lx, ...)`, both `_Noreturn`, and
`probe_out()` mapping a NULL output file to stdout. `s_script` is a
heap-allocated `script_t *`, NULL when nothing is loaded.

**Nothing executes today.** `probe_dispatch`, `probe_report_unfired` and
`probe_installed` are stubs, and no engine calls any of them.

---

## File Structure

| file | responsibility |
|---|---|
| `lib/a2host/probe_vm.c` | the stack machine, the integer formatter, key delivery |
| `lib/a2host/probe.c` | modified: `probe_dispatch`, `probe_report_unfired`, the site-pointer export |
| `lib/a2host/probe_internal.h` | modified: VM entry points shared with `probe.c` |
| `include/apple2tc/probe.h` | modified: `g_probe_sites`, the inline dispatch gate |
| `include/apple2tc/system-inc.h` | modified: `CYCLES` calls `probe_dispatch` |
| `include/apple2tc/system2-inc.h` | modified: same |
| `lib/engine6502/engine6502.cpp` | modified: `debugCB` dispatches; init enables the callback |
| `tests/probe/*.probe`, `tests/run-tests.sh` | runtime tests |
| `decoded/rom/probe-acceptance.sh`, `decoded/rom/trace.probe` | the acceptance test |

---

## Task 1: The VM, reached from the interpreter

The smallest end-to-end slice: dispatch, a stack machine that handles
`PUSH_LIT`/`PRINTF`/`END`, and the interpreter wiring. Everything after this
adds opcodes to a loop that already runs.

**Files:**
- Create: `lib/a2host/probe_vm.c`
- Modify: `lib/a2host/probe_internal.h`, `lib/a2host/probe.c`,
  `include/apple2tc/probe.h`, `lib/a2host/CMakeLists.txt`,
  `lib/engine6502/engine6502.cpp`
- Create: `tests/probe/hello.probe`, `tests/probe/hello.expected`
- Modify: `tests/run-tests.sh`

- [ ] **Step 1: Write the failing test**

`tests/probe/hello.probe` — `$FA62` is the ROM reset entry, executed once
during boot. **A literal, not a counter:** `inc hits` compiles to
`LOAD_COUNTER`/`ADD`/`STORE_COUNTER`, none of which this task implements, so
the probe would die on `opcode 3 is not implemented` rather than print. The
single-line baseline still proves dispatch fires exactly once — any extra or
missing firing changes the line count.

```
# A literal rather than a counter: Task 1 implements only PUSH_LIT, PRINTF and
# END, and `inc` needs three opcodes that do not exist yet.
probe reset() {
  printf("reset %u\n", 1)
}

install reset at $FA62
```

`tests/probe/hello.expected`:

```
reset 1
```

Add to the probe section of `tests/run-tests.sh`, after the compile tests:

```sh
# --- Probes: execution ------------------------------------------------------
#
# Unlike the compile tests above, these run the machine. a2run boots the ROM
# deterministically with no input, so a probe's report is stable.

probe_run_test() {
  # $1: base name under probe/, $2: frame count
  if ! $a2run --frames="$2" --probe="probe/$1.probe" \
              --probe-out="probe-tmp/$1.txt" > /dev/null; then
    echo "FAIL: running probe/$1.probe" >&2
    exit 1
  fi
  diff -q "probe/$1.expected" "probe-tmp/$1.txt"
}

probe_run_test hello 2
```

- [ ] **Step 2: Run it to verify it fails**

```
cd tests && ./run-tests.sh ../cmake-build-debug
```

Expected: FAIL — the report file is empty, because `probe_dispatch` is a stub.

- [ ] **Step 3: Export the dispatch gate**

The design requires `CYCLES` to test a core-owned pointer **inline**, so a
function call will not do. In `include/apple2tc/probe.h`, replace
`probe_installed`'s declaration block with:

```c
/// Non-NULL exactly when a script with at least one install site is loaded.
/// Exported rather than wrapped in a function because `CYCLES` — a macro
/// expanded into the hot path of every generated program — has to test it
/// inline, beside its existing inline test of `g_debug`.
extern const void *g_probe_sites;

/// True once a script with at least one installed site is loaded. Equivalent
/// to `g_probe_sites != NULL`; kept for callers that read better this way and
/// are not on a hot path.
bool probe_installed(void);
```

- [ ] **Step 4: Write the VM**

Create `lib/a2host/probe_vm.c`:

```c
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

#include "probe_internal.h"

#include <stdlib.h>
#include <string.h>

enum { PROBE_STACK_SIZE = 512 };

static uint32_t s_stack[PROBE_STACK_SIZE];
static unsigned s_sp;

/// The address dispatch reached. LOAD_REG REG_PC reads this rather than
/// get_regs().pc: in a generated program the latter is whatever the last
/// CYCLES stored, which is the same value here but is not guaranteed to be,
/// and in the interpreter it is the *next* instruction.
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

void probe_vm_run(const script_t *sc, uint32_t ip) {
  // The initializers push nparams values here and the body indexes them as
  // slots 0..nparams-1 relative to this base. See probe_t::init_offset.
  unsigned frame_base = s_sp;

  for (;;) {
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

    default:
      probe_fatal("probe VM: opcode %u is not implemented", (unsigned)op);
    }
  }
}

void probe_vm_set_pc(uint16_t pc) {
  s_dispatch_pc = pc;
}
```

Add to `probe_internal.h`:

```c
enum { PROBE_MAX_PRINTF_ARGS = 32 };

/// Execute bytecode from \p ip. Used for the address path with
/// ip = init_offset; part 3's macro path will enter at body_offset with the
/// parameter values already pushed. Takes no probe_t: the caller owns hit
/// counting, and passing one the VM does not read would only warn under
/// -Wunused-parameter.
void probe_vm_run(const script_t *sc, uint32_t ip);
/// Record the address dispatch reached, for LOAD_REG REG_PC.
void probe_vm_set_pc(uint16_t pc);
```

`probe_vm_printf` and `probe_vm_hash` (Task 4) stay `static` — each has one
caller, in `probe_vm.c` itself. Declare `probe_vm_init_counters` (Task 3),
`probe_stop_requested` and `probe_deliver_keys` (Task 5) **in the task that
defines them, not here**: a header that documents behaviour four tasks before
it exists is a small lie with a long shelf life, in a codebase whose convention
is that comments are load-bearing.

### `Emu6502::DebugASM` now has three owners

`init_emulated` enables it for probes, but it already meant "trace" (`g_debug &
DebugASM`) and "collect", and `finishCollecting()` at `engine6502.cpp:163`
**clears it unconditionally**. Measured before this was fixed: a script that
produced 2,369 report lines produced 47 under `--collect --from-rom
--limit=2000`, exit 0, no diagnostic.

That matters beyond Task 1. Task 7 compares two reports, and a truncation that
hits both sides identically passes the diff — the "reads as agreement" failure
the whole design exists to prevent. Whatever the fix, leave the pattern visible
for a fourth owner.

`PROBE_MAX_PRINTF_ARGS` is a new limit; the compiler does not enforce one, so
add the check in `parse_printf` too — a format with 33 conversions must be a
compile error, not a runtime one. Add a rejection test beside the other printf
cases.

- [ ] **Step 5: Write the integer formatter**

Still in `probe_vm.c`. The design commits to rendering integers here rather
than handing a constructed `va_list` to libc. For the four conversions the
compiler accepts that buys determinism by construction rather than by
argument:

```c
/// Render one conversion. Only %d %u %x %X reach here -- count_conversions
/// rejected everything else at compile time -- so this handles exactly the
/// flags and width it validated, and nothing about it is locale-sensitive.
static void emit_conv(FILE *f, uint32_t v, char conv, unsigned width,
                      bool zero, bool left, bool plus, bool space) {
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

  char sign = negative ? '-' : (conv == 'd' && plus) ? '+'
      : (conv == 'd' && space)                       ? ' '
                                                     : 0;
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

void probe_vm_printf(const char *fmt, const uint32_t *args, unsigned argc) {
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
```

`probe_out()` is `static` in `probe.c`; give it external linkage and declare it
in `probe_internal.h` so the VM can reach it. That also retires the
`probe_out defined but not used` warning part 1 carried.

- [ ] **Step 6: Implement dispatch**

In `probe.c`, replace the `probe_dispatch` and `probe_installed` stubs:

```c
const void *g_probe_sites = NULL;

bool probe_installed(void) {
  return g_probe_sites != NULL;
}

void probe_dispatch(uint16_t pc) {
  // g_probe_sites is the inline gate callers test; re-checked here because
  // debugCB calls unconditionally.
  if (!g_probe_sites)
    return;
  uint32_t slot = probe_find_slot(s_script, pc);
  if (!s_script->slots[slot].used)
    return;
  probe_vm_set_pc(pc);
  for (uint32_t i = s_script->slots[slot].first; i != PROBE_NO_SITE;
       i = s_script->insts[i].next) {
    probe_t *pr = &s_script->probes[s_script->insts[i].probe_id];
    ++pr->hits;
    probe_vm_run(s_script, pr->init_offset);
  }
}
```

Set the gate at the end of `probe_load_script`, after `probe_build_sites`:

```c
  // Only now, and only if something was actually installed: an empty script
  // must leave the hot path untouched.
  g_probe_sites = s_script->nsites ? (const void *)s_script->slots : NULL;
```

and clear it in `free_script_resources` before freeing.

- [ ] **Step 7: Wire the interpreter**

In `lib/engine6502/engine6502.cpp`, add `#include "apple2tc/probe.h"` and
extend `debugCB`:

```c
Emu6502::StopReason debugCB(void *ctx, Emu6502 *emu, uint16_t pc) {
  if (g_debug & DebugASM)
    debug_asm(pc);
  if (g_probe_sites)
    probe_dispatch(pc);
  return DebugState6502::debugStateCB(ctx, emu, pc);
}
```

and in `init_emulated`, beside the existing `DebugASM` enable:

```c
  // Probes need the per-instruction callback even when tracing is off.
  if ((g_debug & DebugASM) || probe_installed())
    s_emu.addDebugFlags(Emu6502::DebugASM);
```

Add `probe_vm.c` to `lib/a2host/CMakeLists.txt`.

- [ ] **Step 8: Run the tests**

```
ninja -C cmake-build-debug && cd tests && ./run-tests.sh ../cmake-build-debug
```

Expected: `Success!`

If `reset 1` does not appear, check the order of operations in
`a2host_parse_args`: `--probe-out=` is opened after the loop, and
`probe_load_script` runs inside it, so the gate is set before any output is
opened. That is fine, but a probe firing before the output file exists would
write to stdout.

- [ ] **Step 9: Commit**

```bash
git add lib/a2host/probe_vm.c lib/a2host/probe.c lib/a2host/probe_internal.h \
        lib/a2host/CMakeLists.txt include/apple2tc/probe.h \
        lib/engine6502/engine6502.cpp tests/probe/hello.probe \
        tests/probe/hello.expected tests/run-tests.sh
git commit -m "probe: a stack machine, reached from the interpreter"
```

---

## Task 2: Arithmetic, logic, comparison, unary

**Files:** modify `lib/a2host/probe_vm.c`; create `tests/probe/arith.probe`,
`tests/probe/arith.expected`; modify `tests/run-tests.sh`.

- [ ] **Step 1: Write the failing test**

`tests/probe/arith.probe`. Every non-commutative operator appears with operands
that make a reversed pop **visibly wrong** — that is the whole point of this
test, since part 1's suite could not catch it:

```
probe m() {
  printf("sub %d\n", 100 - 30)
  printf("div %u\n", 100 / 4)
  printf("mod %u\n", 100 % 30)
  printf("shl %u\n", 3 << 2)
  printf("shr %u\n", 96 >> 2)
  printf("lt %u %u\n", 3 < 9, 9 < 3)
  printf("le %u %u\n", 9 <= 9, 9 <= 3)
  printf("gt %u %u\n", 9 > 3, 3 > 9)
  printf("ge %u %u\n", 3 >= 3, 3 >= 9)
  printf("add %u mul %u\n", 3 + 4, 3 * 4)
  printf("and %04X or %04X xor %04X\n", $FF00 & $0FF0, $FF00 | $000F, $FF00 ^ $0F0F)
  printf("eq %u ne %u\n", 5 == 5, 5 != 5)
  printf("not %u %u bitnot %08X neg %d\n", !0, !7, ~$0F, -5)
  printf("pad [%5u] [%-5u] [%05u] [%+d] [% d]\n", 42, 42, 42, 42, 42)
}

install m at $FA62
```

`tests/probe/arith.expected` is generated in step 4 — but **derive the right
answers by hand first and write them down in your report**, then compare. A
generated baseline from a VM that pops `SUB` backwards would enshrine
`sub -70`.

Add `probe_run_test arith 2` to `tests/run-tests.sh`.

- [ ] **Step 2: Run it to verify it fails**

Expected: `FATAL: probe VM: opcode 11 is not implemented`.

- [ ] **Step 3: Implement the opcodes**

In `probe_vm.c`'s switch. **Right pops first**, per `probe_internal.h`:

```c
#define BINOP(name, expr)                                                      \
  case name: {                                                                 \
    uint32_t r = vm_pop();                                                     \
    uint32_t l = vm_pop();                                                     \
    vm_push(expr);                                                             \
    break;                                                                     \
  }

      BINOP(OP_ADD, l + r)
      BINOP(OP_SUB, l - r)
      BINOP(OP_MUL, l * r)
      BINOP(OP_AND, l & r)
      BINOP(OP_OR, l | r)
      BINOP(OP_XOR, l ^ r)
      BINOP(OP_EQ, l == r)
      BINOP(OP_NE, l != r)
      BINOP(OP_LT, l < r)
      BINOP(OP_LE, l <= r)
      BINOP(OP_GT, l > r)
      BINOP(OP_GE, l >= r)
```

Division, shifts and the unary operators need their own cases, because each has
a way to be undefined:

```c
    case OP_DIV:
    case OP_MOD: {
      uint32_t r = vm_pop();
      uint32_t l = vm_pop();
      // A probe must not take the process down; report and stop instead.
      if (r == 0)
        probe_fatal("probe VM: division by zero");
      vm_push(op == OP_DIV ? l / r : l % r);
      break;
    }
    case OP_SHL:
    case OP_SHR: {
      uint32_t r = vm_pop();
      uint32_t l = vm_pop();
      // A shift of 32 or more is undefined in C. Define it as zero, so the
      // two engines cannot disagree about it.
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
      vm_push((uint32_t)(0u - vm_pop()));
      break;
```

Comparisons yield `uint32_t` 0 or 1 — C's `==` already does, but note it
explicitly in a comment, because the language's `&&`/`||` lowering depends on
`JZ` testing exactly this.

- [ ] **Step 4: Generate, check against your hand-derivation, and inspect**

```
ninja -C cmake-build-debug
cmake-build-debug/tools/a2run/a2run --frames=2 --probe=tests/probe/arith.probe \
  --probe-out=tests/probe/arith.expected > /dev/null
```

Compare every line against what you derived in step 1. Report both.

- [ ] **Step 5: Tests and commit**

```
cd tests && ./run-tests.sh ../cmake-build-debug
```

```bash
git add lib/a2host/probe_vm.c tests/probe/arith.probe tests/probe/arith.expected \
        tests/run-tests.sh
git commit -m "probe VM: arithmetic, comparison and unary opcodes"
```

---

## Task 3: Counters, parameters, registers, control flow

**Files:** modify `lib/a2host/probe_vm.c`; create `tests/probe/flow.probe`,
`tests/probe/flow.expected`; modify `tests/run-tests.sh`.

- [ ] **Step 1: Write the failing test**

`tests/probe/flow.probe`:

```
counter n
counter every = 0

probe f(pc_at = pc, doubled = n * 2) {
  inc n
  if (n == 1)
    printf("first pc=%04X doubled=%u\n", pc_at, doubled)
  else if (n == 3)
    printf("third pc=%04X doubled=%u\n", pc_at, doubled)
  if (n >= 2 && n <= 3)
    every = every + n
  if (n == 3)
    printf("every=%u\n", every)
}

install f at $FA62, $FA63, $FA66
```

Three *instruction starts* on the boot path, each firing exactly once. An
earlier draft used `$FA62, $FA63, $FA64`, which does not work: `$FA63` is a
3-byte `JSR`, so `$FA64` and `$FA65` are its operand bytes and are never the
value of `pc` at dispatch. Measured — a probe at `$FA64` fires **0** times.

That is worth pausing on, because it is the design's central hazard appearing
in the plan's own test: a probe at an address the engine never dispatches on
produces no output and no error, and a report missing lines it should have had
reads exactly like agreement. Verify any new install address by installing a
bare `printf` there and counting hits before building a test on it.

Add `probe_run_test flow 2`.

- [ ] **Step 2: Run it and see it fail** — an unimplemented opcode.

- [ ] **Step 3: Implement**

```c
    case OP_LOAD_PARAM:
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
      case REG_A: vm_push(r.a); break;
      case REG_X: vm_push(r.x); break;
      case REG_Y: vm_push(r.y); break;
      case REG_SP: vm_push(r.sp); break;
      case REG_SR: vm_push(r.status); break;
      // Not r.pc: in a generated program that is whatever the last CYCLES
      // stored, and in the interpreter it is the instruction about to run.
      // The dispatch address is the one both engines agree on.
      case REG_PC: vm_push(s_dispatch_pc); break;
      }
      break;
    }
    case OP_JMP:
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
```

Counters live in the VM, initialised from the script when it loads:

```c
static uint32_t s_counters[PROBE_MAX_COUNTERS];

void probe_vm_init_counters(const script_t *sc) {
  for (unsigned i = 0; i != sc->ncounters; ++i)
    s_counters[i] = sc->counters[i].init;
}
```

Call it from `probe_load_script` after `probe_build_sites`. Declare it in
`probe_internal.h`.

`OP_JMP` reads its target without advancing `ip` past it, because it
overwrites `ip` anyway — note that in a comment so nobody "fixes" it into
`ip = sc->code[ip++]`, which is unsequenced and undefined.

Add `#include "apple2tc/a2engine.h"` to `probe_vm.c` for `get_regs`/`regs_t`.

- [ ] **Step 4: Generate the baseline and read it**

Verify by hand: `first` prints `doubled=0` (the initializer runs before `inc`),
`third` prints `doubled=4`, and `every=5` (2+3).

- [ ] **Step 5: Tests and commit**

```bash
git add lib/a2host/probe_vm.c lib/a2host/probe_internal.h lib/a2host/probe.c \
        tests/probe/flow.probe tests/probe/flow.expected tests/run-tests.sh
git commit -m "probe VM: counters, parameters, registers and control flow"
```

---

## Task 4: Memory reads and hashing

**Files:** modify `lib/a2host/probe_vm.c`, `lib/a2host/probe_internal.h`;
create `tests/probe/mem.probe`, `tests/probe/mem.expected`; modify
`tests/run-tests.sh`.

- [ ] **Step 1: Write the failing test**

`tests/probe/mem.probe`. `$FFFC`/`$FFFD` is the reset vector — a fixed,
known value in ROM, so this test asserts real content rather than whatever
happens to be there:

```
probe m() {
  printf("vec8 %02X %02X\n", peek8($FFFC), peek8($FFFD))
  printf("vec16 %04X\n", peek16($FFFC))
  printf("zp %08X\n", hash($0000, $00FF))
  printf("same %08X\n", hash($0000, $00FF))
  printf("one %08X\n", hash($FFFC, $FFFC))
}

install m at $FA62
```

`zp` and `same` must be identical — the hash must not depend on call order or
leave state behind.

- [ ] **Step 2: Run it and see it fail.**

- [ ] **Step 3: Implement**

```c
    case OP_PEEK8:
      // ram_peek, never peek: inspecting a machine must not alter it. A peek
      // through the IO range would trip the soft switches at $C0xx.
      vm_push(ram_peek((uint16_t)vm_pop()));
      break;
    case OP_PEEK16: {
      uint16_t addr = (uint16_t)vm_pop();
      vm_push((uint32_t)ram_peek(addr) | ((uint32_t)ram_peek((uint16_t)(addr + 1)) << 8));
      break;
    }
    case OP_HASH: {
      // hash(start, end): start is pushed first, so end pops first.
      uint32_t end = vm_pop();
      uint32_t start = vm_pop();
      vm_push(probe_vm_hash((uint16_t)start, (uint16_t)end));
      break;
    }
```

```c
/// FNV-1a over the inclusive byte range, read through ram_peek. Chosen for
/// being fully specified and endianness-free: both engines must produce
/// identical values from identical memory, and a reversed or platform-varying
/// hash would produce *consistent* wrong values on both sides, which is the
/// one failure this facility cannot see.
uint32_t probe_vm_hash(uint16_t start, uint16_t end) {
  uint32_t h = 2166136261u;
  for (uint32_t a = start; a <= end; ++a) {
    h ^= ram_peek((uint16_t)a);
    h *= 16777619u;
  }
  return h;
}
```

The loop counter is `uint32_t` deliberately: with `uint16_t`, `hash($0, $FFFF)`
would never terminate.

- [ ] **Step 4: Generate the baseline and check it**

Verify `vec16` equals `vec8`'s two bytes little-endian, and that `zp` and
`same` match. Confirm `hash($FFFC, $FFFC)` hashes exactly one byte by computing
FNV-1a of that byte by hand.

- [ ] **Step 5: Tests and commit**

```bash
git add lib/a2host/probe_vm.c lib/a2host/probe_internal.h tests/probe/mem.probe \
        tests/probe/mem.expected tests/run-tests.sh
git commit -m "probe VM: memory reads and range hashing"
```

---

## Task 5: `stop`, `key`, and never-fired reporting

**Files:** modify `lib/a2host/probe_vm.c`, `lib/a2host/probe.c`,
`lib/a2host/a2host.c`, `lib/a2host/probe_internal.h`; create
`tests/probe/stopat.probe`, `tests/probe/stopat.expected`; modify
`tests/run-tests.sh`.

- [ ] **Step 1: Write the failing tests**

`tests/probe/stopat.probe`:

```
counter n

probe s() {
  inc n
  printf("hit %u\n", n)
  if (n == 3)
    stop
}

probe never() {
  printf("unreachable\n")
}

install s at $FA62
install never at $0001
```

`tests/probe/stopat.expected` — `$FA62` is reached once per boot, so with
enough frames the counter reaches 3 and `stop` fires:

```
hit 1
hit 2
hit 3
```

And a stderr assertion, since the never-fired report is the guard against a
probe silently covering nothing:

```sh
probe_run_test stopat 20

# A probe that never fires must say so. This is the guard against the design's
# main hazard: a probe on an address one engine does not have looks exactly
# like agreement.
$a2run --frames=20 --probe=probe/stopat.probe --probe-out=/dev/null \
  2>probe-tmp/unfired.txt >/dev/null
if ! grep -q "never" probe-tmp/unfired.txt; then
  echo "FAIL: no never-fired report for a probe that never ran" >&2
  cat probe-tmp/unfired.txt >&2
  exit 1
fi
if grep -q "^  s " probe-tmp/unfired.txt; then
  echo "FAIL: a probe that did fire was reported as never firing" >&2
  exit 1
fi
```

- [ ] **Step 2: Run and see it fail.**

- [ ] **Step 3: Implement `stop`**

`stop` must end the run cleanly rather than the process. `a2host` already has
the shape for it: add a `probe_stop_requested()` query and have
`a2host_run_headless`'s loop and `a2host_record_frame` consult it, next to the
existing frame-limit check.

```c
static bool s_stop_requested;

bool probe_stop_requested(void) {
  return s_stop_requested;
}
```

```c
    case OP_STOP:
      // Cannot longjmp out: the engine is mid-block. Ask, and let the host
      // notice after run_emulated returns.
      s_stop_requested = true;
      break;
```

In `a2host.c`, in the headless loop and wherever `a2host_record_frame`'s
return is checked, treat `probe_stop_requested()` the same as reaching the
frame limit.

- [ ] **Step 4: Implement `key`**

```c
    case OP_KEY:
      probe_deliver_keys(vm_pop());
      break;
```

`probe_deliver_keys` belongs in `a2host.c`, beside the existing key list —
`key_presses_`, `next_key_press_`, `key_press_count_` are already there:

```c
/// Deliver every pending key whose stamp is <= `now`. The stamp is read in
/// whatever coordinate the script passes -- typically a counter. The field is
/// still named `cycles` because --trace-keys still records cycle stamps;
/// re-recording in counter coordinates is deferred, and until it happens an
/// existing .keys file replayed through `key` is being reinterpreted rather
/// than replayed.
void probe_deliver_keys(uint32_t now) {
  while (next_key_press_ != key_press_count_ &&
         key_presses_[next_key_press_].cycles <= now) {
    a2_io_push_key(&io_, key_presses_[next_key_press_].ch);
    ++next_key_press_;
  }
}
```

Declare it in `probe_internal.h`. **No test for `key` in this task** — a
meaningful one needs a recording in counter coordinates, which is deferred.
Say so in the commit message rather than writing a test that asserts nothing.

- [ ] **Step 5: Implement the never-fired report**

In `probe.c`:

```c
void probe_report_unfired(void) {
  if (!s_script)
    return;
  unsigned quiet = 0;
  for (unsigned i = 0; i != s_script->nprobes; ++i)
    if (s_script->probes[i].hits == 0)
      ++quiet;
  // To stderr, never the report file: a diff of two reports must not be
  // perturbed by diagnostics.
  fprintf(stderr, "probes: %u declared, %u never fired\n", s_script->nprobes, quiet);
  for (unsigned i = 0; i != s_script->nprobes; ++i) {
    const probe_t *pr = &s_script->probes[i];
    if (pr->hits == 0)
      fprintf(stderr, "  %s never fired\n", pr->name);
    else
      fprintf(stderr, "  %s %u\n", pr->name, pr->hits);
  }
}
```

Call it from `a2host_shutdown`, before `probe_close_output()`.

- [ ] **Step 6: Tests and commit**

```bash
git add lib/a2host/probe_vm.c lib/a2host/probe.c lib/a2host/a2host.c \
        lib/a2host/probe_internal.h tests/probe/stopat.probe \
        tests/probe/stopat.expected tests/run-tests.sh
git commit -m "probe VM: stop, key delivery and the never-fired report"
```

---

## Task 6: Wire the generated engines

Until now only the interpreter dispatches. This is the task that makes probes
work in decompiled programs — and it needs no regeneration, because `CYCLES`
is a shared header.

**Files:** modify `include/apple2tc/system-inc.h`,
`include/apple2tc/system2-inc.h`; modify `tests/run-tests.sh`.

- [ ] **Step 1: Write the failing test**

The two front ends share a host and an engine, so with the same script they
must produce identical reports. Add after the existing frontend equivalence
check:

```sh
# The same probe script through both front ends. They share everything but the
# window, so any difference is a bug in the front end, not the probe.
$a2run --frames=40 --probe=probe/hello.probe --probe-out=probe-tmp/fe-run.txt \
  > /dev/null
$a2emu --headless --frames=40 --probe=probe/hello.probe \
  --probe-out=probe-tmp/fe-emu.txt > /dev/null
if ! diff -q probe-tmp/fe-run.txt probe-tmp/fe-emu.txt > /dev/null; then
  echo "FAIL: a2emu and a2run disagree on a probe report" >&2
  diff probe-tmp/fe-run.txt probe-tmp/fe-emu.txt | head -5 >&2
  exit 1
fi
```

That passes already — both are the interpreter. The real assertion is Task 7's.
Write it anyway: it is cheap and it pins the front ends.

- [ ] **Step 2: Wire both `CYCLES` macros**

`include/apple2tc/system-inc.h`:

```c
#define CYCLES(pc, cycles)                                                 \
  do {                                                                     \
    s_cycles += (cycles);                                                  \
    s_remaining_cycles -= (cycles);                                        \
    if ((g_debug & DebugASM) && (!(g_debug & DebugEmu) || branchTarget)) { \
      branchTarget = false;                                                \
      debug_asm(pc);                                                       \
    }                                                                      \
    if (g_probe_sites)                                                     \
      probe_dispatch(pc);                                                  \
  } while (0)
```

`include/apple2tc/system2-inc.h` — identical addition, at the end of the macro
body, after the existing `debug_asm` block.

Both files must `#include "apple2tc/probe.h"`. Check where each gets its other
includes and match.

**Why at the end rather than beside `debug_asm`:** the probe must observe the
block's entry state, and everything above it in the macro is cycle accounting
and tracing, not machine state. Keeping it out of the `g_debug` condition is
deliberate — probes are not a debug flag, and `add_default_nondebug()` blanks
`$FCA8-$FCB3` and `$FD0C-$FD3C`, which is exactly where a keyboard probe wants
to sit.

- [ ] **Step 3: Confirm generated programs still build and behave**

```
ninja -C cmake-build-debug
cd tests && ./run-tests.sh ../cmake-build-debug     # Success!
cd ../decoded/snake-byte && ./verify.sh              # 4x PASS
```

`verify.sh` must still pass 4/4 — with no script loaded, `g_probe_sites` is
NULL and the added test is one predictable branch per block. If timing changed
enough to break a frame hash, say so rather than adjusting the baseline.

- [ ] **Step 4: Commit**

```bash
git add include/apple2tc/system-inc.h include/apple2tc/system2-inc.h \
        tests/run-tests.sh
git commit -m "probe: dispatch from the generated engines too"
```

---

## Task 7: The acceptance test

The point of the whole plan: reproduce a result already trusted, through the
new mechanism, so that a disagreement indicts the probe system rather than the
decompiler.

**Files:** create `decoded/rom/trace.probe`,
`decoded/rom/probe-acceptance.sh`; modify `decoded/rom/README.md` if one
exists, else create a note.

- [ ] **Step 1: Write the script and the harness**

`decoded/rom/trace.probe`:

```
probe trace(site = pc) {
  printf("%04X\n", site)
}

install trace at @"blocks.txt"
```

`decoded/rom/probe-acceptance.sh`:

```sh
#!/bin/bash
#
# The acceptance test for probes: the interpreter and the decompiled ROM must
# produce byte-identical probe reports over a boot.
#
# Why this test and not a new one. 1,991 of 1,991 branch targets already match
# between these two over 120 frames of ROM boot -- measured 2026-08-08, with a
# bespoke trace comparison. Reproducing that number through probes validates
# the probe mechanism against a result we already believe. A disagreement here
# means the probes are wrong, not the decompiler.
#
# The site list is derived from the generated C rather than from apple2tc,
# because CYCLES is emitted per basic block: grepping it out of romc1.c yields
# exactly the addresses the generated program *can* probe. Installing at any
# other address would fire in the interpreter and silently not exist on the
# other side -- which is the one failure mode this design cannot otherwise
# see.

set -e

if [ -z "$1" ]; then
  echo "Usage: $0 <build-dir>" >&2
  exit 1
fi

bin=$1
a2run="$bin/tools/a2run/a2run"
romc1="$bin/decoded/rom/romc1-run"
here=$(dirname "$0")

for tool in "$a2run" "$romc1"; do
  [ -x "$tool" ] || { echo "Error: not found: $tool" >&2; exit 1; }
done

frames=${FRAMES:-120}

# Block heads, straight from the artifact that defines them.
grep -oE 'CYCLES\(0x[0-9a-f]+' "$here/romc1.c" \
  | sed 's/CYCLES(0x//' | sort -u > "$here/blocks.txt"
echo "site list: $(wc -l < "$here/blocks.txt") block heads"

"$a2run" --frames="$frames" --probe="$here/trace.probe" \
  --probe-out=/tmp/probe-interp.txt > /dev/null 2>/tmp/probe-interp.err
"$romc1" --frames="$frames" --probe="$here/trace.probe" \
  --probe-out=/tmp/probe-gen.txt > /dev/null 2>/tmp/probe-gen.err

# A probe that never fired means the two sides did not cover the same set,
# even if what they did emit matches.
for side in interp gen; do
  if grep -q "never fired" "/tmp/probe-$side.err"; then
    echo "FAIL [$side]: a probe never fired" >&2
    grep "never fired" "/tmp/probe-$side.err" >&2
    exit 1
  fi
done

if ! diff -q /tmp/probe-interp.txt /tmp/probe-gen.txt > /dev/null; then
  echo "FAIL: the two engines disagree" >&2
  diff /tmp/probe-interp.txt /tmp/probe-gen.txt | head -20 >&2
  exit 1
fi

echo "PASS: $(wc -l < /tmp/probe-interp.txt) probe hits match over $frames frames"
```

`chmod +x` it.

- [ ] **Step 2: Run it, and expect it to be interesting**

```
./decoded/rom/probe-acceptance.sh cmake-build-debug
```

**Do not assume this passes first time, and do not adjust anything to make it
pass.** Two known asymmetries could show up:

1. **The interpreter dispatches per instruction; the generated program per
   block.** Installing only at block heads should make the sets identical — but
   if the interpreter fires at an address that is a block head *and* an
   instruction inside another block, the counts differ. Report what you see.
2. **The generated ROM may not execute the same boot path** as the interpreter
   booting the real ROM image.

If the reports differ, your job is to **characterise the difference precisely**
— first differing line, how many hits each side has, whether one is a
subsequence of the other — and report it. Do not weaken the test. A real
divergence here is a genuine finding and exactly what this test exists to
surface.

- [ ] **Step 3: Record the outcome**

Whatever happens, append an entry to `docs/decompiling/decision-log.md` dated
today, following the format of the entries already there — `**Scope:**`,
`**Status:**`, `**Decision:**`, then what was measured. If the test passes,
state the hit count and the frame count. If it does not, state exactly how the
two differ and what you ruled out. This is the log's purpose: the 2026-08-08
entry exists because the previous comparison's *failure* was worth recording.

- [ ] **Step 4: Commit**

```bash
git add decoded/rom/trace.probe decoded/rom/probe-acceptance.sh \
        decoded/rom/blocks.txt docs/decompiling/decision-log.md
git commit -m "probe: the ROM-boot acceptance test"
```

Decide whether `blocks.txt` belongs in git: it is generated, but committing it
makes the test's input reviewable and lets a failure be bisected. If you commit
it, say why in the message; if not, add it to `.gitignore`.

---

## Self-review against the spec

**Covered:** the VM and every opcode; `printf` rendering with the flags and
widths the compiler accepts; `ram_peek`-based memory access; FNV-1a hashing
with its rationale; `stop` via a host-consulted flag; `key` delivery; the
never-fired report; dispatch from both engines; the front-end equivalence test;
and the ROM-boot acceptance test.

**Deferred, and stated in the tasks that touch them:**

- **Counter-stamped key recording.** `--trace-keys` still writes cycle stamps.
  Task 5 implements `key` against the existing list and says plainly that an
  existing `.keys` file replayed through it is being reinterpreted. The
  frame-472 attribution needs the recording change and belongs with it.
- **The macro path** (`PROBE_x(...)` call sites emitted by apple2tc) is phase 3.
  `probe_vm_run` takes an entry `ip` so that path can enter at `body_offset`
  with values already pushed, but nothing calls it that way yet.

**Two decisions this plan makes that the spec left open:**

1. **A shift of 32 or more yields zero**, rather than being undefined. Two
   engines must not be free to disagree.
2. **Division by zero is fatal**, reported through `probe_fatal`. The
   alternative — defining it as zero — would let a script with a real bug
   produce a clean-looking report.

**One risk worth naming up front:** Task 6 adds a branch to `CYCLES`, which is
in the hot path of every generated program. It is a predictable test of a
global that is NULL whenever no script is loaded, but `verify.sh` compares
cycle counts as well as frame hashes, so Task 6's step 3 is a real gate rather
than a formality.
