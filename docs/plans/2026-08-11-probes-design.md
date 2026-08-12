# Probes: comparing two implementations without a shared clock

Date: 2026-08-11

The language reference is `docs/probes.md`. This document is the rationale.

## The problem

Question 1 of the three in `2026-08-07-host-engine-split-design.md` — *is
apple2tc correct?* — needs an observable that both the interpreter and the
generated C can produce and that can be compared. Every candidate tried so far
failed, and all of them failed the same way.

| observable | outcome | why |
|---|---|---|
| register state | unusable | `CPURegLiveness` and `dce` drop dead stores; traces diverge on line 2 |
| cycle counts | unusable | the generated engine yields at block boundaries (overshoot ≤63), the interpreter between instructions (overshoot ≤1) |
| frame / screen hashes | unusable | requires a shared sampling point; two *correct* implementations still disagree |
| branch-target PCs, no input | **works** | 1,991/1,991 over 120 frames of ROM boot, consuming 8,013/8,014 interpreter instructions, no drift |
| branch-target PCs, with keys | unusable | 21 of 23 keys land a frame apart; the streams drift apart and stay apart |

The common cause: **we were sampling on a clock the two programs do not
share.** Frame boundaries, cycle counts and wall time are all coordinates
external to the program, and neither engine defines them the same way. Key
delivery is decided by `get_cycles() >= stamp` evaluated at frame boundaries, so
a stamp that sits exactly on one build's boundary sits three cycles short of the
other's, and the key lands a frame later.

## The idea

A **probe** is a small program that runs at a chosen point in the emulated
program. Both engines reach that point, in the same order, the same number of
times — that is what equivalence *means*. So the program supplies the
coordinate, and there is no clock left to disagree about.

A probe can read registers and memory, count, branch, print, hash a memory
range, and pull a key from a recording. Reports from two runs are compared with
`diff`.

This is not only a question-1 tool. Question 3 — *does the hand-written C match
the original?* — has no address correspondence at all between the two programs,
so nothing automatic can compare them. A probe is a **manually declared
correspondence**, which is exactly and only what that question needs.

### A probe is a program, and it is independent of where it runs

Two properties do the work:

**It is a program, not a record.** Conditions are ordinary control flow inside
it, not a clause attached to it. This is why the bytecode is variable-length —
see below.

**It is independent of its install site.** In the decompiled program a probe is
not at a 6502 address, and in hand-written C for question 3 there are no
addresses at all. Binding the address into the probe definition would break the
case that motivated the design. So probes are declared, and `install` binds them
to sites separately; one probe may be installed at many.

### Parameters are the signature

A probe's parameter list is its signature — name, arity, order. That is the part
both implementations must satisfy identically, because it is what makes two
report streams comparable. Distinct from it are the **default initializers**:
expressions saying how to obtain each value, correct in the emulator by
construction.

In the decompiled program some initializers do not hold — a register may be
dead, or the value may live in a C variable rather than at an address. Those
parameters are overridden individually, which shrinks the surface on which the
two sides can disagree from "a hand-written body per side" to "one expression,
only where the default fails."

## Script language

`--probe=path` loads a script and `--probe-out=path` names the report. Both are
`a2host` options, so every front end has them.

**Declarations**

| form | notes |
|---|---|
| `counter <name> [= <expr>]` | init defaults to 0, constant-folded at compile |
| `probe <name>([p = expr, ...]) { ... }` | params are the signature |
| `install <probe> at <sites>` | `$ADDR`, `$LO-$HI`, `@"file"`, comma-separated mix |

**Statements**

| form | notes |
|---|---|
| `{ ... }` | block |
| `if (e) s [else s]` | compiles to branches |
| `printf("fmt", ...)` | `%d %u %x %X` with width and zero-pad, and `%%` |
| `<counter> = e`, `<param> = e` | assignment |
| `inc <counter>` | sugar for `c = c + 1` |
| `key e` | deliver every pending key stamped ≤ `e` |
| `stop` | end the run cleanly |

**Expression atoms**

`<literal>` (decimal or `$hex`) · `<param>` · `<counter>` · `a` `x` `y` `sp`
`sr` `pc` · `peek8(e)` · `peek16(e)` · `hash(lo, hi)`

Initializers are ordinary expressions rather than a prefix sublanguage, and each
renders to C for phase 3: `peek8($1F00)` → `ram_peek(0x1F00)`, `a` → `s_a`, `pc`
→ the literal block address.

Names resolve **params → counters → registers**, so a declared name shadows a
register of the same name, in reads and in assignments alike. `x` and `y` are
the obvious names for coordinates — the `pos` example above declares both — so
forbidding them would be a bad language for an implementation's convenience. The
order has to be identical on both paths: resolving registers first on reads
while assignment saw only counters would make `counter y` … `y = y + 1` read the
register and write the counter, a silent divergence in the one tool built to
find divergences.

Shadowing is resolved as the script is read, so a counter must be declared
before any probe that would otherwise resolve its name to a register. Declaring
it later is an error rather than a silent change of meaning: the same spelling
would compile to `LOAD_REG` above the declaration and `LOAD_COUNTER` below it,
and phase 3 has apple2tc generating these scripts, where declaration placement
is a code-generator detail rather than a considered choice.

`peek8`, `peek16`, `hash` and the statement keywords are matched before
identifier lookup and cannot be shadowed coherently, so declaring one — as a
counter, a parameter or a probe — is an error.

**Operators**, C precedence: `||` · `&&` · `|` `^` `&` · `==` `!=` · `<` `<=`
`>` `>=` · `<<` `>>` · `+` `-` · `*` `/` `%` · unary `!` `~` `-`

### Examples

Control-flow trace, the acceptance test:

```
probe trace(site = pc) {
  printf("%04X\n", site)
}

install trace at @"blocks.txt"
```

Attributing a divergence — two probes at one site, where order matters:

```
counter frame

probe tick() {
  inc frame
}

probe state(f = frame, sx = peek8($1F00), sy = peek8($1F01),
            scr = hash($2000, $3FFF)) {
  if (f >= 460 && f <= 480)
    printf("f=%u snake=%02X,%02X scr=%08X\n", f, sx, sy, scr)
}

install tick  at $6100
install state at $6100
```

`state`'s initializers run at entry, so it observes the post-increment counter
only because `tick` is installed first. Script order is chain order.

Key delivery in program coordinates, and a phase-3 override target:

```
probe keys(f = frame)          { key f }
probe snake(x = peek8($1F00), y = peek8($1F01)) {
  printf("snake %02X %02X\n", x, y)
}

install keys  at $6100
install snake at $2A10
```

## Architecture

The core is `lib/a2host/probe.c` and `include/apple2tc/probe.h`. No new library
and no CMake changes: `CMakeLists.txt:18-25` already links `a2host` into
`<name>`, `<name>-obj` and `<name>-run`, and `a2emu`/`a2run` link it directly.

The core owns the parser, the bytecode VM, the counters, the key source, the
dispatch table and the reporter.

### One program, two entries

A probe compiles to two offsets into a single instruction stream:

```c
typedef struct { uint32_t init_offset; uint32_t body_offset; uint8_t nparams; } probe_t;
```

`init_offset` is the initializer expressions, which push `nparams` values and
fall through into the body; `body_offset` is the body, which reads them as stack
slots `0..n-1`. No prologue instruction, no frame setup.

**Address path.** `probe_dispatch(pc)` enters at `init_offset`, so the
initializers run. Called from `debugCB` (`lib/engine6502/engine6502.cpp:193`)
and from the `CYCLES` macro (`include/apple2tc/system-inc.h:29`).

`CYCLES` is a *shared header*, not generated text. Adding the call there means
**every existing generated program gains probes on recompile — no regeneration,
no decompiler change.** That is what makes phase 1 self-contained.

**Call path.** `PROBE_snake(...)` pushes the caller's values and enters at
`body_offset`, skipping the initializers because apple2tc rendered them into the
call site:

```c
#ifndef PROBE_snake
#define PROBE_snake(x, y) probe_run(PROBE_ID_snake, (x), (y))
#endif
...
PROBE_snake(ram_peek(0x1F00), ram_peek(0x1F01));
```

An override is one line in a hand-maintained header, surviving regeneration:

```c
#define PROBE_snake(x, y) probe_run(PROBE_ID_snake, snake_x, (y))
```

**So the body is literally the same bytecode on both sides.** Counters, gating,
formatting and key-fetch have one implementation, not two, and the only
difference between the emulator and the decompiled program is where a few values
came from. Initializers therefore run at entry, once, before the body — not a
choice, but forced by the call path having already evaluated them.

### No contract change

`a2engine.h` and `a2host.h` are untouched. `engine6502` asks the core at init
whether any probe is installed and enables `Emu6502`'s per-instruction callback
if so; `CYCLES` tests a core-owned pointer inline, null when no script is
loaded. The host/engine split does exactly the work it was built for.

`stop` needs no contract change either. `engine_stop_reason()` only reports
`A2_STOP_REQUESTED` from `engine6502`, and generated engines always return
`A2_STOP_CYCLES`; so `stop` sets a core flag that `a2host` consults after
`run_emulated`, which behaves identically for both.

### Probes do not go through `debug_asm`

That path is gated on `DebugASM` and filtered by the `add_nondebug` ranges, and
`add_default_nondebug()` (`lib/a2host/a2host.c`) silently blanks `$FCA8-$FCB3`
and `$FD0C-$FD3C` — precisely where a keyboard probe wants to sit. Probes get
their own entry point and their own state.

## Execution model

Bytecode rather than fixed-size structs, and **for flexibility, not speed**: a
`{first_action, n_actions}` pair can never grow a branch target, whereas a
linear stream absorbs conditionals, expressions and whatever comes later at the
cost of documenting an opcode. Cells are `uint32_t`, so an address or a literal
is one cell with no alignment games.

Dispatch is a `switch` in a loop over a value stack. Not indirect threading: the
miss path dominates by a factor of a thousand, so body dispatch is unmeasurable
here, and computed goto is a GCC/Clang extension absent from MSVC — which
matters because console and GUI front ends are separate executables specifically
to support Windows.

| group | opcodes |
|---|---|
| load | `PUSH_LIT n` · `LOAD_PARAM i` · `LOAD_COUNTER i` · `LOAD_REG r` · `PEEK8` · `PEEK16` · `HASH` |
| store | `STORE_PARAM i` · `STORE_COUNTER i` |
| arith | `ADD SUB MUL DIV MOD` |
| bitwise | `AND OR XOR SHL SHR` |
| compare | `EQ NE LT LE GT GE` |
| unary | `NOT BITNOT NEG` |
| control | `JMP t` · `JZ t` · `JNZ t` |
| effect | `PRINTF fmtid nargs` · `KEY` · `STOP` · `END` |

Two rules are enforced by the opcode set rather than by discipline:

- **`PEEK8`/`PEEK16` go through `ram_peek`, never `peek`.** Inspecting a machine
  must not alter it — the lesson `disasm`/`printDB` already learned with the
  `$C0xx` soft switches.
- **There is no cycles opcode.** `CYCLES` bills a whole block *before* calling
  out, so at any sample point the generated program's cycle count is ahead of
  the interpreter's. Making it unrepresentable beats documenting it.

Registers are read via `get_regs()`, which works in both engines — stale in the
generated one, which is the case parameter overrides exist for, and which
surfaces as a divergence rather than passing quietly.

### Dispatch table

An open-addressed hash keyed on address. Chains live in a separate array rather
than in spare slots, so a chain node can never sit in the path of a linear
probe — every occupied slot is a real key:

```c
typedef struct { uint16_t addr; uint16_t used; uint32_t first; } slot_t;
typedef struct { uint32_t probe_id; uint32_t next; } inst_t;
```

Not a direct `uint16_t map[0x10000]`. The deciding argument is footprint: 128KB
of BSS in *every* generated program, permanently, for a debugging facility most
runs never enable. A hash sized to the script costs nothing when unloaded.
Cache locality points the same way — a direct map's working set is proportional
to the address span the program roams over and competes for L1 with `s_ram`,
indexed by the same `pc` — but at ~5M lookups against 14M emulated cycles of
full instruction decode, neither scheme is measurable.

Probes sharing a site chain through `next` in script order, which only touches
the hit path; misses are ~98% of lookups and see one probe of an empty slot.

6502 addresses cluster hard — blocks land on nearby addresses within a page — so
the hash needs a mixing step. `pc & mask` would pile everything into adjacent
slots.

### Install sites, and why `@file` is the one that matters

`CYCLES` is emitted **per block, not per instruction**: in `snake-bytec1.c`,
`CYCLES(0x60e7, 28)` covers all of `bb_0` while `$60E9`, `$60EB`, `$60F0` and
`$60F5` are bare comments. The 2,203 sites are block heads.

So the interpreter can probe any address and the generated program can only
probe block heads. A probe on `$60E9` fires happily under `a2emu`, does not
exist in the generated C, **and the report still reads as agreement.** This is
the main hazard of the design.

It is why a wildcard install would be wrong — "every address" and "every block
head" are different sets, so the two streams would differ by construction — and
why cross-engine comparison should install from `@file`, with apple2tc emitting
the block-head list it already knows. Both sides then install at exactly the
same finite set, the reports have the same length, and **`diff` works directly
with no subsequence matcher.** That component is the one least worth trusting:
the greedy matcher written during the phase-6 investigation drifted and reported
a meaningless "50% match".

`$LO-$HI` ranges remain useful for probing a routine whose shape you know, but
carry the same asymmetry. Guarding both: the core names every declared probe
that never fired, and phase 3 has apple2tc validate a script against its block
heads.

The sampling *points* do coincide, which is the thing that would have quietly
wrecked this and does not: `debugCB` fires before the instruction at `pc`
executes, and `CYCLES` sits at the top of its block, before the block's
statements.

## The report

`printf` output goes to the file named by `--probe-out=path`.

Using `printf` is safe here for a reason worth stating: **the format string
lives in the probe's bytecode, which is the same bytecode on both sides.**
Formatting cannot diverge between the emulator and the decompiled program. Only
values can, which is exactly what a divergence should mean.

Three constraints:

- A validated directive subset — `%d %u %x %X` with width and zero-padding, plus
  `%%`. No `%s`, `%p`, `%n`, no floats; stack values are numeric cells.
- Argument count is checked when the probe compiles, so a mismatch is a parse
  error rather than a runtime surprise.
- The format string is interpreted by the core, not handed to libc with a
  constructed `va_list`. About sixty lines, and it sidesteps locale sensitivity
  — a class of nondeterminism that has no business near an equivalence oracle.

**Comparison is `diff`.** Two runs, two files, `diff -q`, following the
`--hash-frames` idiom already in `tests/run-tests.sh`. A matcher that degrades
gracefully on divergent input will manufacture agreement; `diff` either matches
or points at the first difference.

**Diagnostics go to stderr, never the report file**, so they cannot perturb a
diff. At shutdown the core lists declared probes with hit counts and names every
probe that never fired.

**Errors are loud.** Parse failures report file and line and exit non-zero,
following the `--code-at` FATAL convention that `run-tests.sh` asserts case by
case. Unknown counter and parameter names are parse errors. Stack
under/overflow aborts naming the probe.

## Key fetch

`key e` delivers every pending key whose stamp is ≤ `e`, pushing into the
keyboard queue via `a2_io_push_key`. Exhaustion is a no-op, counted and
reported.

This inverts the input problem. Today the host *pushes* keys on a schedule it
owns; a probe *pulls* them at a point the program owns. There is no schedule
left to disagree about.

The key file keeps its shape — `<stamp> <ascii>`, one `fscanf` in
`lib/a2host/a2host.c` — but the stamp is read in whatever coordinate the script
passes to `key`, typically a counter, instead of cycles. Recording mirrors it:
`--trace-keys` stamps from the same counter. One mechanism in both directions,
program-defined at both ends.

The consequence is that existing cycle-stamped `.keys` files must be
re-recorded. They are 2022 artefacts of a windowed `a2emu`, and re-recording
headlessly is exactly what the host/engine split made possible. `play.kbd` is
unaffected and still covers the boot.

## Testing

Following the project split — decompiler regression in `tests/`, game data in
`decoded/`:

- **`tests/run-tests.sh`**: a small `.s` program, a probe script, an expected
  report, under `a2run`. Covers parser, expressions, VM, counters, control flow
  and formatting, with no game data. Rejection cases follow the existing
  `expect_reject` pattern.
- **The a2emu/a2run equivalence test** gains a probe script: same host, same
  engine, one has a window, the reports must be identical.
- **The acceptance test**, in `decoded/`: `a2run` versus a generated program,
  same script installed from the same `@file`, byte-identical reports, and a
  hard failure if any probe never fired.

The acceptance test is deliberately the **ROM boot**, replicating the known
1,991/1,991 result through probes. It is the one result already trusted, so it
validates the mechanism rather than merely exercising it: if the probe report
disagrees where the old trace agreed, the probe system is wrong.

## Phasing

1. **Core.** Parser, expression compiler, VM, hash dispatch, address path,
   `printf`, the three tests above. No decompiler change and no macros. Ends
   with the ROM boot proven through probes.
2. **Key fetch and counter-stamped recording.** Unblocks input-driven
   comparison. First real use: attributing the frame-472 divergence, which is
   currently a dead end because "frame 472" is a host-side count with no meaning
   inside either program.
3. **Macros.** apple2tc reads the same script, emits the block-head list for
   `@file` installs, validates scripts against it, and emits `PROBE_x(...)` call
   sites with initializers rendered as C. Unblocks C-variable parameters, and
   question 3.

Phase 1 is self-contained and answers question 1's instrumentation problem on
its own.

## What this does not solve

- **Cycle counts still will not match.** That is the point of the reframing:
  cycles stop being the coordinate. They return as a real question for
  questions 2 and 3 — "does it still run at the right speed" — but separately.
- **Arithmetic that never changes control flow** is only covered where a probe
  happens to print it. Probes narrow this gap where aimed; they do not close it.
- **A probe set is an oracle we author**, so it inherits our mistakes. A
  misplaced probe gives a false divergence; a badly chosen one gives false
  confidence. A matching report proves what the probes cover and nothing more.
