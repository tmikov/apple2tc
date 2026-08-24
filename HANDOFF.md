# Handoff: converting Snake Byte into maintainable C

Read this first. It is the entry point for resuming the work on branch
`snake-byte`. Everything below is measured or committed — where something is a
guess, it says so.

**Last commit:** run `git log -1`; this line has been stale before. As of
2026-08-22 the tree is clean and nothing is pushed. The most recent work is
`$69A9` — the last emulator-shaped game routine — becoming real C, on a
**standing decision that no further recordings will be made**: unexercised code
is decoded from the binary rather than held until something runs it. See the
2026-08-22 log entry, and note that it supersedes a playbook rule that said the
opposite.

Before that, two decompiler capabilities — `--inline-str` and
`--alt-exit` — and the conversion they unblocked: **the main loop and the
auto-steer are now C** (`game_play_loop_native`, `game_auto_steer`). Before
that, a long run of infrastructure that did not touch the game at
all: the **host/engine split** (2026-08-11 log entry), the **probe compiler and
VM** (2026-08-12, 2026-08-13), **accurate cycle costs** in both engines
(2026-08-13), two apple2tc bug fixes (2026-08-13, 2026-08-14), and
**probe-stamped input** (2026-08-14, 2026-08-15), which is what finally made
the interpreter and the generated C comparable. All are summarised under
"Host, engines and probes" below; read that before assuming anything about how
verification runs, because it changed.

---

## The goal

Turn apple2tc's decompiler output for Snake Byte into C worth maintaining — real
functions, structured control flow, named variables. The wider aim is a
repeatable method for *any* Apple II game.

The decompiler is a bootstrap and an accelerator, **not the thing doing the
work**. Where it cannot reach a routine, decompile it by hand. Its rejections
and its code/data classifications are evidence, not verdicts.

## Working agreements

- **Decide routine-level questions yourself.** Do not escalate "what should I do
  about `$X`". Escalate only project-level forks: scope, architecture,
  verification strategy.
- **Never edit `snake-byte.json`.** It is a recording of what actually happened
  during a play session. Editing it to assert reachability the run never
  observed destroys its value as evidence.
- **Reachability the run never observed goes in `code-at.txt`**, one
  `ORIGIN TARGET` edge per line, each with the argument for why it is real. See
  the 2026-08-05 log entry: this supersedes the earlier "hand-decompile what the
  tool cannot reach, do not feed the decompiler more input" agreement, which was
  weighed and set aside once the cost of the alternative was measured.
- **Hand-decompilation is still the fallback**, for anything `--code-at` cannot
  reach — but note there is no general way to *call* hand-written code at an
  untraced address. `$664A` works only because `$FDED` is an externalised ROM
  vector that `rom_cout` dispatches on.
- **No new recordings.** The committed set (`play`, `play-hires`, `play-rebind`,
  and the `easy` fixture) is what there is. Code no recording reaches is decoded
  from the binary — `id` for the instructions, the opcode table for the cycle
  charges, `--ir` for the liveness — and never held back waiting for something
  to run it. Name the unexercised blocks in a comment above the routine, since
  converting takes them off the site list, and do not call the result verified.
  See the 2026-08-22 log entry; this supersedes the playbook rule that said to
  wait.
- **`tests/` is decompiler regression only** — hand-written `.s` assembled by
  `a6502`, decompiled, diffed against `.ir` baselines. Game data lives in
  `decoded/<game>/`.

---

## Read these, in this order

| File | What it holds |
| --- | --- |
| `docs/decompiling/playbook.md` | The transferable method: findings, procedure, red-flag table. Maturity is honestly marked — steps 1-4 have been executed, 5-6 have not. |
| `docs/decompiling/decision-log.md` | Append-only rationale. **Never edit existing entries**; add new ones and mark old ones `superseded`. Read it from the end: the 2026-08-15 entry's "Left open" list went stale for six days before anyone noticed, so trust the newest entry over any earlier one's forward-looking notes. |
| `decoded/snake-byte/labels.txt` | Established names, each with its evidence in a comment. |
| `decoded/snake-byte/code-at.txt` | The two hand-asserted reachability claims, each with its argument. Read before adding a third. |
| `docs/superpowers/specs/2026-08-02-snake-byte-proper-c-design.md` | The original design. Parts are superseded — the log says which. |

The two plans in `docs/superpowers/plans/` are executed and historical.

Infrastructure, only if you are touching how things run or are verified:

| File | What it holds |
| --- | --- |
| `docs/plans/2026-08-07-host-engine-split-design.md` | Why `a2run` exists. Leads with an outcome section that retracts part of its own reasoning — read that first, not the design below it. |
| `docs/plans/2026-08-11-probes-design.md` | Why probes exist: every other way of comparing the two engines failed, and all of them failed the same way. |
| `docs/probes.md` | The probe language reference. The design doc is the rationale; this is the *what*. |
| `decoded/rom/boot-state.txt` | The Apple II's boot state at the BASIC prompt, with nothing loaded — the floor any retargeted entry point starts from. Game-independent; captured with `--snapshot-at`. |
| `docs/plans/2026-08-11-probes-plan-1-compiler.md` | The executed plan. Its closing self-review is the most useful part — it names what part 2 inherits. |

**Read the log before acting on anything that seems obvious.** It deliberately
preserves the wrong turns so they are not repeated. See "Traps" below.

---

## Where things stand

### Decompiler (`tools/apple2tc/`)

- **Per-routine stack tracking.** `scanCandidate` used to balance the 6502 stack
  per *basic block*, rejecting the universal `PHA`-at-entry / `PLA`-before-`RTS`
  idiom. Depth now propagates along CFG edges, required zero only at `RTS`.
  Snake Byte: **53 → 75** routines.
- **A routine entry can also be a call's return point.** `scanCandidate` used to
  reject an `RTS` predecessor outright, losing every block that a caller falls
  into after a `JSR` — Snake Byte's `$60E7`, plus five more through the cascade.
  Accepting it was one line; the extraction then needed three further fixes. See
  the 2026-08-07 log entry. Snake Byte: **74 → 81** routines.
- **`--routines-report=<path>`** — every candidate, accepted or rejected, with
  block sets, call sites, dominator chains and natural loops. This is the input
  for hand-writing structured C. Note it reflects the CFG *before* `simplifyCFG`,
  so its blocks are more granular than `--irc1` output.
- **`--extern-routines=<file>`** — maps addresses to C function names; calls
  become calls to bodyless `Function`s and unreachable blocks are deleted.
  Note it rejects any address that is not already a known block.
- **`--inline-str=<file>`** — routines that take a NUL-terminated string after
  the `JSR` and return past it. Without it the tracer follows the fall-through
  into the text and decodes it as instructions, which is a wrong decompilation
  nothing can notice. Declared routines must also be in `--extern-routines`,
  because a *generated* one would still pop a return address the option has
  moved; both refusals are enforced and tested.
- **`--alt-exit`** — routines that return by discarding their return address and
  jumping into the caller (`PLA/PLA/JMP`). The call becomes `CallAlt`, a
  terminator with one successor per exit, and the routine returns which exit it
  took. Read the playbook entry before touching this: the obvious fix — relaxing
  the stack check — passes every behavioural oracle and produces a function that
  calls itself. Externalizing such a routine is refused unless every generated
  call site goes away too.
- **`--code-at=<file>`** — hand-asserted `ORIGIN TARGET` dynamic branch edges,
  merged into the runtime data before disassembly. `TARGET` is disassembled
  *and* becomes a successor of the branch at `ORIGIN`; the latter is what earns
  it an entry in the generated address-to-block map, without which it would be
  decompiled but unreachable. This is how `$7541` and the reference build's
  `$664A` are reached.
- **`Dominators.{h,cpp}`** — Cooper-Harvey-Kennedy, validated against a DAG, a
  single-loop routine and a nested delay loop. The codebase had none before.

### Host, engines and probes (`lib/a2host`, `lib/engine6502`, `tools/a2run`)

Added 2026-08-11/12. None of it touches the game; all of it changes how you run
and verify one.

**`system.h` is now two contracts.** `a2engine.h` is what an engine provides and
the host calls; `a2host.h` is what the host provides and an engine calls;
`a2host_api.h` is what the host offers a *front end*. There are two front ends —
`a2host_gui` (sokol) and a console `main` — and they are **separate executables
rather than one binary with a flag, because on Windows console versus GUI is a
link-time subsystem property**. So:

| Binary | What it is |
| --- | --- |
| `a2emu` | the windowed emulator; now 131 lines, all of it F1/F2 game loading and two gdb helpers |
| `a2run` | the same host and engine with a console `main` and no graphics |
| `<name>` / `<name>-run` | every decompiled game now builds both, via `add_a2_program` |

**Three things this bought, and they matter to the questions below:**

1. **Run data can be regenerated headlessly.** Previously impossible —
   `snake-byte.json` is a 2022 artefact of a windowed `a2emu`. Recipe in
   `decoded/snake-byte/README.md`, which is explicit that it does *not*
   reproduce that file byte for byte.
2. **`verify.sh` links no graphics at all.** It drives `snake-bytec1-run` and
   `snake-bytec1-ext-run`.
3. **`a2emu` has a test for the first time**, in `tests/run-tests.sh`: sharing
   everything but the window, `a2emu --headless` must byte-match `a2run`.

**Probes** (`lib/a2host/probe*.c`, `include/apple2tc/probe.h`) are a compiler
*and* a VM, running in every front end and in every generated program. A probe
is a small program bound to install sites, so the two engines can be compared
at points *the program* defines rather than at moments the host defines. Read
`docs/probes.md` for the language and the 2026-08-12 log entry for why.
Options: `--probe=`, `--probe-out=`, `--probe-dump`, `--record-keys=`.

`decoded/rom/probe-acceptance.sh <build-dir>` is the cross-engine gate: the
interpreter against both generated back ends over 120 frames of ROM boot,
276,255 probe hits each, byte-identical. Run it alongside `verify.sh`.

`decoded/snake-byte/probe-acceptance.sh <build-dir>` is its Snake Byte
counterpart and the stronger one. It replays probe-stamped input (`play.pkeys`,
`play-hires.pkeys` — not cycle-stamped; item 5 under "Next") through both of
`verify.sh`'s scenarios, and checks two things per scenario per back end:

| | play | hires |
|---|---|---|
| block-head trace | 2,744,938 hits identical | 2,864,242 identical |
| memory (`ram.probe`) | identical at 6,808 samples | identical at 8,465 |

The memory check is not redundant with the trace: a wrong byte written need not
change control flow within one run. The hires scenario is not redundant with
play: it reaches `$664A` and `$7541`, code the recording never took, and in the
`-ext` build `$664A` is hand-written C in `game.c` — so hires/ext is the only
cross-engine check that hand-written replacement gets.

**The hazard, stated once here because it is easy to lose:** `CYCLES` is emitted
per basic block, not per instruction, so a probe installed at a non-block-head
address fires under `a2emu`/`a2run` and *does not exist* in a generated program
— and the report still reads as agreement. Cross-engine comparison must install
from `@"file"`, using a block-head list. The list is grepped out of the
generated C, where `CYCLES(0x` names block heads and `CYCLES_EDGE(0x` names
taken-branch edges, which deliberately do not dispatch.

### Tooling (`tools/id/`)

`id` is a REPL reading stdin, so it scripts directly:

```bash
printf 'loadb33 snake-byte.b33\nlabels labels.txt\ndis $664A $66A8\n' | id
```

It resolves the built-in Apple II symbol database (`COUT1`, `BASH`, `CSWL`).
`dis` takes a count or a start/end range. `labels <file>` loads `ADDR name` pairs
that override the built-in names. **Use this instead of decoding bytes by eye.**

### Snake Byte (`decoded/snake-byte/`)

Five targets, and they are not five copies of the same thing.
`decoded/snake-byte/README.md` now carries the full file inventory; this is the
short version.

| Target | Source | Role |
| --- | --- | --- |
| `snake-byte-cold` | `snake-byte-cold.c` | **The artifact this work is aimed at.** One file, 9,462 lines, sharing nothing. Entered at `$3750`; no boot, no ROM code, no generated code. |
| `snake-bytec1-ext` | `snake-byte-ext.c` | The verified reference. Boots, types `CALL 14160`. ROM entry points from hand-written `a2rom.c` + `game.c`. |
| `snake-bytec1` | `snake-bytec1.c` | Self-contained control. ROM decompiled alongside the game; links alone. `play.frames` was recorded from it. |
| `snake-byte-easyc1-ext` | `snake-byte-easy-ext.c` | Test fixture, not a variant: apple quota 16 -> 2. |
| `snake-byte` | `snake-byte.c` | Historical `--simple-c` output, no longer regenerated. |

**Both are gated now.** `probe-acceptance.sh` compares cold against ext on every
run — 788,097 block heads and 6,808 screen samples, aligned at the first
`$3750`. The control is ext rather than the interpreter because there is no
interpreter that starts at `$3750`; ext earns the role by being checked against
the interpreter a few dozen lines earlier in the same script.

That pair checks only what is cold-specific — entry state, pruning, key offset.
Both builds share `game.c` and `game_native.c`, so a bug in the game's own C
changes both identically and is caught upstream, not here.

**The adapters in `game.c` are now mostly scaffolding, not an interface.**
Measured 2026-08-23: of the 42, only **14 still have a generated caller**
(`game_place_apple` has 5, `game_draw_status` and `game_plot_shape` 2 each, the
rest 1). For the other 28 every caller is hand-written C that could call the
`_native` function directly. What they still buy is verification, not
structure:

- `CYCLES(addr, 0)` keeps the entry address on the block-head list, so the
  cross-engine trace still compares it. Delete the adapter and that comparison
  point disappears silently.
- `push16(ret)`/`pop16()` keeps the emulated stack matching the 6502's, which
  `ram.probe` compares and which the inline-string idiom actually *reads*.
- Three do real work rather than marshalling — see the playbook's red flag.

So they are exactly what a shipped artifact sheds (`ret_addr == 0`, no emulated
stack, no probe sites), and exactly what Phase 1b makes redundant in bulk. Do
not delete them one at a time before then; the site count is the thing that
notices.

One was fully dead and is gone: `game_rand_byte` had no caller anywhere and no
`CYCLES` site. Its `$6C4B` mapping went with it — a mapping for a routine no
generated code calls buys nothing, and is not how a routine gets deleted.

**`./decompile.sh` reproduces the committed generated C, and this is now
gated.** `probe-acceptance.sh` regenerates into a temporary directory and
compares, before it runs anything else — because everything else in that script
runs the *committed* `.c` files, so a stale artifact would leave every PASS a
statement about the wrong file, site counts included. When it fails it says
which kind of drift it is: whitespace-stripped identical means layout only,
differing tokens mean the decompiler's output really changed.

It takes a build directory and an optional output directory
(`decompile.sh [build-dir] [out-dir]`), reads its inputs relative to the script
rather than the caller's cwd, and defaults to regenerating in place.

`a2rom.c` and `game.c` are **`#include`d, never compiled separately**.
`system2-inc.h` defines the machine state (`s_ram`, `s_a`, the `CYCLES` macro)
with internal linkage, so a second translation unit would get its *own* copy of
everything and link cleanly while being silently wrong. Both are marked
`HEADER_FILE_ONLY` in CMake to prevent exactly that.

- `a2rom.c` — the 9 ROM entry points, plus `GBASCALC`, `PLOT1` and the `$FB78`
  `COUTZ` subtree, which were deleted as collateral when the 9 became external.
- `game.c` — hand-decompiled *game* routines. Currently just `$664A`. Keep it
  separate from `a2rom.c`; ROM replacements and game code are different things.

---

## Build and verify

```bash
cmake -G Ninja -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
ninja -C cmake-build-debug

cd tests && ./run-tests.sh ../cmake-build-debug     # expect: Success!
cd decoded/snake-byte && ./verify.sh ../../cmake-build-debug   # expect: 4x PASS
./probe-acceptance.sh ../../cmake-build-debug                 # the real gate
./decompile.sh                                                # regenerates the c1 variants
```

`decompile.sh` reproduces the committed generated C exactly: run it at a clean
HEAD and `git status` comes back empty. `probe-acceptance.sh` checks that before
it checks anything else, because everything else it does runs the committed
`.c` files. Note `snake-byte-cold.c` is *not* regenerated and never will be —
it is the decompilation, owned by hand.

`run-tests.sh` is no longer only decompiler regression. It also asserts that
`a2emu --headless` byte-matches `a2run`, and carries the probe compiler's 4
baseline diffs and 63 rejection assertions. Both were verified green at
`4ffb88a`, as was `verify.sh` at 4/4.

`verify.sh` replays two scenarios against both builds:

```
PASS [play/ref]:  1300 frames match     ordinary game
PASS [play/ext]:  1300 frames match
PASS [hires/ref]: 1300 frames match     C at attract -> redefine all six keys
PASS [hires/ext]: 1300 frames match
```

It replays cycle-stamped keys headless and compares per-frame FNV-1a hashes of
video memory (mode + text page + hi-res page) *and* cycle counts. `--record`
re-records both traces from the reference build, twice each, and refuses to
write unless the two runs agree.

**Never edit a `.frames` file to make a test pass.** The trace is the reference.

`[hires]` is a real cross-check, not a regression test: the reference build runs
a decompiler-generated `$664A`, while in the ext build `$FDED` is externalised,
which deletes that block and routes COUT through hand-written `game_cout_hook`.
The two builds run different code and must agree frame for frame.

---

## Coverage — what is left

**Measured, not estimated.** `decompile.sh` regenerates
`decoded/snake-byte/coverage.txt` alongside the C, so it cannot go stale:

```
range $3750-$854E  19967 bytes
  code              4244   21.3%
  declared data    12883   64.5%
  unknown zero      2389   12.0%
  unknown nonzero    451    2.3%   <- what is left to identify
```

`known-data.txt` declares the identified non-code regions, each with its
evidence; the report flags any of them the disassembler also reached as code, so
a wrong declaration cannot sit there quietly. The 2026-08-04 figure of 2,657
unknown bytes was a hand count that lumped assets, tables and buffers together.

The largest remaining gaps:

```
$799B-$7FFF  1637 bytes,  275 nonzero   84% zero -- a buffer with scattered state
$616B-$61FF   149 bytes,   98 nonzero   tbl_hgr_masks, extent not yet derived
$7680-$7690    17 bytes,   17 nonzero
$6633-$6640    14 bytes,   14 nonzero
$6232-$6255    36 bytes,   11 nonzero   near buf_keys
```

then a long tail of runs under 10 bytes. Nothing left is a cluster; this is
variables and small tables, not undiscovered subsystems.

---

## `$7541` — done (2026-08-05)

Reached via `--code-at` (`7251 7541`) rather than hand-decompiled; see the
2026-08-05 log entry for why, and `code-at.txt` for the argument that the edge
is real. It is the **key-redefinition screen**:

```
        ABSOLUTE            RELATIVE
       DIRECTION           DIRECTION
           b                             b/c/d/e are font glyphs $E2-$E5,
         d + e               d   e       drawn as arrows by $664A
           c
      PRESS THE KEY FOR THIS FUNCTION
```

Six slots — absolute up/left/right/down, then relative turn-left/turn-right —
over the key table at `$6C63` (`C9 CA CB CD 88 95` = `I J K M ← →`). For each
slot it prints the current key (`$7590`), blinks the slot's arrow while polling
for a new one (`$75D1`), accepts anything `>= $A1` or the two arrows (`$761C`),
writes it back, and finally restores CSWL/CSWH to `$FDF0` at `$7587`.

`play-hires.keys` now presses six keys after `C` and `play-hires.frames` covers
1,300 frames, up from 390.

### Next

Items 1-4 below are all struck through now. What actually remains, in order:

**a. ~~Put `snake-byte-cold` in `probe-acceptance.sh`.~~** Done 2026-08-23.

**b. ~~Convert the last 89 blocks.~~** Done 2026-08-23 — 377 lines of C in
place of 968 lines of dispatch. **The cold build contains no
decompiler-generated code at all.** What is left that is not the game is data
and runtime: `s_mem_3750` (the game's own image), `s_mem_d000` (ROM bytes the
death pause reads at `$E000` as delay lengths), five ROM helpers `a2rom.c`
calls, and the machine definition.

**And then the sharing went away.** `snake-byte-cold.c` is a single
self-contained file of 9,462 lines: the pruned body, `a2rom.c`,
`game_native.c`, `game.c` and the top level, each as this target's own copy,
with the entry state inlined. The other four targets keep the shared originals
and are unaffected.

That was forced rather than chosen. While the file was shared, every change had
to stay safe for builds still running a generated dispatch over the same
addresses — the top-level conversion had already had to be split into a separate
file for exactly that reason. Cold owns its code now, and the gate is what says
whether the game still behaves.

**Know which half of the gate a change spends.** The cold gate compares against
`snake-bytec1-ext`, which still shares the originals. Refactoring inside the
single file stays fully checked while it preserves behaviour *and* block-head
order. The moment a change deliberately breaks the second — dropping the
emulated stack, moving storage out of RAM — the trace half stops applying and
the screen half is all that is left. `screen.probe` was built for exactly that
moment; see its header.

**Dead code does not announce itself here: the build has no `-Wall`.** Three
functions were found by hand on 2026-08-23 -- `addr_to_block_id` and its
comparator, left behind when the dispatch went, and `game_hi_cmp`, which had
been dead in the shared `game.c` since its call sites moved into a converted
routine. The check is a manual compile, and it must be a real one:

```bash
gcc -c -o /dev/null -Wall -I../../include -I. snake-byte-cold.c
```

`-fsyntax-only` does *not* report unused functions and comes back clean, which
is how one survived a check that looked like it covered this. Turning `-Wall`
on for the generated targets is a separate decision: a generated block nothing
jumps to still gets a label, so they emit dozens of unused-label warnings.

Two conditions came out inverted and neither oracle would have caught them
without help: at `$7851` the original *branches away* when the value is not
`$FE`, so equality is the fall-through; and the paddle button at `$788E` reads
with bit 7 clear when pressed. The technique that found them is worth reusing —
temporarily respell every `GAME_CYCLES` in the converted C as
`GAME_CYCLES_ANCHOR`, which keeps the probe, and the converted code becomes
comparable block-for-block against the generated original. Take the edges out
first: the original's `CYCLES_EDGE` addresses must stay unprobed or they show as
phantom differences.

### Cleaning up the C

`snake-byte-cold.c` is the decompilation, and it is not yet good C: values are
passed in `s_a` (203 uses), 115 distinct addresses stand in for variables, 40
functions still take a `uint16_t ret_addr` they mostly ignore, and there are
842 `CYCLES` sites to maintain. The plan below is ordered by *which oracle each
step spends*, cheapest-and-fully-checked first.

**~~Step 0 — strengthen the cold gate first.~~** Done 2026-08-23. It compared
trace and screen but not memory, and most of what follows moves values without
moving pixels. `ram-cold.probe` adds the memory comparison. It deliberately
drops the stack — the cold build is heading for no emulated stack at all, so
comparing it would pin the scaffolding and go red on the first adapter deleted
rather than on the first bug. It also skips `$4E/$4F`: RNDL/RNDH record how
long the ROM's KEYIN spun, so they differ between two boots of the same build,
and they are measurably the only two bytes below `$C000` that do.

**~~Step 1 — the free deletions.~~** Done 2026-08-23. `func_t001` and the 15
adapters that were nothing but `push16`/call/`pop16`. No cycle sites, no
marshalling; the site count stayed at 120 and all three checks stayed green.
`plot_shape_at` lost a `ret` parameter that existed only to feed one of them,
which is the cascade to expect from the rest.

**Step 2 — names.** All 115 addresses, `ram_peek(0x6253)` -> `life_outcome()`.
Pure rename: same cycles, same blocks, so the trace checks it at full strength.
Do it while the trace is strongest, sliced by subsystem, a commit each. Biggest
readability win at the lowest risk, and it is where the remaining "what *is*
`$725A`?" knowledge stops being re-derived.

**Step 3 — real parameters.** `$0000-$0003` and `$0024/$0025` are the plotter's
argument block and the cursor, and the five most-touched addresses in the file
(63, 55, 26, 25, 23 uses). They are parameters passed through fixed memory
because the 6502 had no other way. This is the step that moves storage, so it
is the one that spends Step 0's memory oracle — retire it explicitly here.

**Step 4 — return values instead of `s_a` and the flags.** As callers stop
reading `s_a`, the 23 remaining marshalling adapters empty out and follow the
first 15.

**Step 5 — drop `ret_addr` and the emulated stack.** 40 signatures, mechanical,
with one trap: `game_print_inline_str` finds its string by *reading* the pushed
return address. It has to take a real string parameter before the stack goes,
or it breaks silently.

**Step 6 — `CYCLES`.** Last. It is what the trace oracle is made of, so after
this the screen is the only check left.

**d. Loose ends:** 451 unknown nonzero bytes in the coverage report; probe phase
3 (apple2tc emitting `PROBE_x(...)` sites so a probe can read the generated C's
own variables); and `robotron`/`bolo`, which have never been regenerated against
any of this.

1. ~~**The two remaining rejection roots.**~~ — **both done.** `$7230` became
   `--inline-str` (2026-08-17) and `$6A32` became `--alt-exit` (2026-08-18), the
   latter after one wrong attempt that passed every oracle and produced a
   self-recursive function; see the playbook entry on structural checks.

   **No game routine is rejected any more.** The report now reads 53 accepted /
   64 rejected, and every one of the 64 is Applesoft ROM — the dominant class
   being `stack level 2 at RTS`, the push-address-then-`RTS` computed jump.
   Nothing there is on the path to readable game C. Re-measure with
   `--routines-report` before treating any rejection as work.
2. ~~**Teach the disassembler the inline-string idiom.**~~ — **done**,
   `--inline-str=<file>` (`tools/apple2tc/Disas.cpp`). One declaration file per
   game; the routine must also be in `--extern-routines`, which is enforced.

2a. ~~**Every identified game routine is converted.**~~ — **done, and now
   complete.** `$6256`, `$6288`, `$6A32`, `$72CE`, `$78B3` and `$7980` went on
   2026-08-21; `$69A9`, the last emulator-shaped one, on 2026-08-22. `game.c`
   is adapters only, and the sole generated function left in the whole
   translation unit is `func_t001`, the top-level dispatch.

   The pinned site count is **1171**, of which **97 are in the game range
   ($3750-$854E) and 1074 are Applesoft ROM** — measured against
   `blocks-ext.txt`, so it is re-derivable:

   ```bash
   python3 -c "
   import io
   a=[int(l,16) for l in io.open('decoded/snake-byte/blocks-ext.txt') if l.strip()]
   g=[x for x in a if 0x3750<=x<=0x854e]
   print(len(a), len(g), len(a)-len(g))"
   ```

   **92% of what is left is ROM**, and no amount of further game work touches
   it. That is what makes retargeting the entry (item 4) the dominant item
   rather than a someday one — and it is why "convert another routine" is no
   longer available as the next step.

   Three things a new conversion should know, all of them learned here:

   - ~~**`game_print_inline_str` leaves orphans.**~~ **Fixed by
     `--prune-returns`** (2026-08-22). The orphans were never about the
     inline-string printer: `$7239` is where `$7230`'s own `JSR $FC68` returned,
     `$FC68` is external, and a ROM RTS still listed `$7239` among its returns.
     That one edge held the printer's body, its RTS, and the ten blocks its
     callers resume at. Pruning return edges no surviving call can produce
     removed 64 edges and 298 more blocks, and 74 `GAME_CYCLES_SHARED`
     spellings became plain `GAME_CYCLES` -- each one named by the lint, which
     checks the claim in both directions.
   - **`$741F` is a coordinate address** and lives inside `$7980`. It is
     spelled `GAME_CYCLES_COORD` there. Converting a coordinate site without
     that spelling stops the replay counter advancing and is caught only as a
     several-hundred-thousand-line trace diff.
   - **`$78B3` is entered with decimal mode set**, so its adapter omits the
     assertion every other adapter carries.

   Nineteen blocks are now converted without ever having been run: seven in
   `$6A32`, nine in `$7980` and three in `$69A9`, listed in the comments above
   `game_auto_steer()`, `game_setup_screen()` and
   `game_pause_or_toggle_sound_native()`. Those comments are the only record,
   because converting takes the addresses off the site list. The unverified
   count in probe-acceptance.sh is **20, and every one of them is ROM** — the
   number can no longer say anything about the game.

   **Read a routine's coverage per property, not as one figure.** `$69A9`
   measured it: inverting its Ctrl-S test fails `trace-ext`, but mis-charging
   its cycles by one passes every gate, because verify.sh compares cycles and
   never enters the routine while probe-acceptance.sh enters it and is
   deliberately cycle-blind. Control flow checked, timing not, one number for
   both.

3. ~~**`$8000-$84A4`**~~ — **answered 2026-08-07: it is data**, 29 vector
   display lists read as a byte stream by `$7019`. Nothing reaches it because
   nothing jumps into it. See the log entry for the command grammar, which
   parses the region exactly.
4. ~~**Phase 1b: retarget the entry to `$3750`.**~~ — **done 2026-08-23**, as
   the `snake-byte-cold` target. See `decoded/snake-byte/snake-byte-cold.c`.

   `snake-byte-cold-body.c` is a hand-owned fork of the generated
   `snake-bytec1-ext.c`: the dispatch is rooted at `$3750` instead of the reset
   vector, and everything unreachable from there is deleted. **16,195 lines ->
   3,046.** `func_t001` went from 1,775 cases to 89 — every one of them game
   code — its address map from 136 entries to 1, and 42 of the 47 generated ROM
   helpers went with it. The five that stayed are the ones `a2rom.c` calls.

   What could not go: `s_mem_d000`, the ROM image. The death pause reads its
   delay lengths out of ROM *as data* at `$E000`, so the bytes are still needed
   even though none of the code is.

   The reachable set came from `apple2tc --ir`, which prints `Succ(...)` per
   block and every `Call` — walk it from `$3750` and 1,776 blocks reduce to 89.
   Doing the same walk on the emitted C instead is a trap: `pop16`, `tmp3_U16`
   and `sbc_dec16` all contain "16", so a naive scan for `block_id = N`
   manufactures edges to block 16 and reports 104 live cases instead of 89.

   Verified against the booting build, from `$3750` on: **block-head trace
   identical over 788,097 blocks, screen identical at all 6,808 in-game
   samples.** Nothing about the entry state was guessed -- `--snapshot-at`
   captured it, and `make-entry-state.sh` regenerates it.

   **`--snapshot-at` now exists, as capture only** (2026-08-23). Every front
   end takes `--snapshot-at=<hex> --snapshot-out=<path>` and writes 64KB of RAM
   plus a `.regs` sidecar the first time the PC reaches that address — one shot,
   because the interesting address is usually re-entered (Snake Byte reaches
   `$3750` eight times a run) and what is wanted is the first arrival. It fails
   loudly if the address is never reached.

   That is the measuring half. **The restoring half does not exist**: nothing
   consumes a snapshot, and the generated engine still starts at the reset
   vector. That is what Phase 1b still needs.

   The soft switches are captured too, into the `.regs` sidecar, because they
   are the half of the machine a RAM image cannot carry — they are host IO
   state, not memory. At the prompt they are the power-on values: text, page 1,
   not mixed, `vid_control = $01`. A rebased entry has to set them explicitly.

   What the capture already settled is the size of the problem, which was being
   guessed at. See `decoded/rom/boot-state.txt`: the machine's entire
   boot-created RAM state is **`$0000-$0802`** — zero page, the stack, the input
   buffer, and the text screen. Everything from `$0803` to `$BFFF` is `$FF`,
   i.e. uninitialised and not boot's doing at all. It is two kilobytes, recorded
   once, and game-independent.

   **The re-based trace does not need a human — measured 2026-08-23.** This
   was the obvious way for Phase 1b to collide with the no-new-recordings
   agreement, so it was checked before any design work. It does not.

   Both halves are mechanical:

   - The `.frames` files always were. `verify.sh --record` re-runs the
     reference build twice and refuses to write unless the two agree.
   - The **key files re-stamp by a constant**. Instrument the coordinate
     (`rec.probe`'s seven sites) with a `printf` alongside its `key n`, add a
     probe at `$3750`, and replay each scenario: the first `$3750` entry lands
     at **n=181,207** for `play` and `play-hires` and **n=217,543** for
     `play-rebind`, and no coordinate site fires between the end of boot and
     that entry. `$FB7C` is the one ROM site that also fires *after* the game
     starts — 17 times, in the two hi-res scenarios — and it fires from
     `a2rom.c`, which a retargeted build still has, so the post-entry sequence
     is unchanged.

   So re-stamping is: **drop the keys stamped below the entry constant, and
   subtract it from the rest.** The dropped keys are the same 11 in all three
   scenarios and they are literally `CALL 14160` + CR — 14160 is `$3750`, so
   they exist only to reach the address the retargeted build starts at. What
   is left is 12 / 8 / 34 keys.

   Two things this does *not* say. The snapshot format is still the real work
   and still unbuilt. And `$3750` is entered **8 times** in a single play run
   — the game restarts through it — so a cold start there is something the
   game already does to itself; what has to be captured is the state at the
   *first* entry, which is the one arriving from BASIC.

   The probe used is five lines; reproduce it rather than trusting these
   numbers if anything upstream has moved.
5. **Frame 623 is resolved; a narrower divergence remains, not yet
   root-caused.** Frame 623 was the old cycle-stamped-input artefact: `play.keys`
   was recorded from the generated build, so its stamps were that engine's own
   cycle coordinate, replayed against the interpreter's different one (see the
   2026-08-14 "Probe-stamped key recording" decision-log entry). Probe phase 2
   closes that gap — `record <expr>` (the recording counterpart of `key
   <expr>`) and `--record-keys=` let a script stamp keys on a coordinate the
   *program* defines, not the host's cycle counter (`docs/probes.md`).
   `decoded/snake-byte/play.keys` was converted to `play.pkeys` on that
   coordinate, and `decoded/snake-byte/probe-acceptance.sh` is the resulting
   cross-engine gate: replaying `play.pkeys` against the interpreter and both
   generated builds produces a byte-identical 2,744,938-hit block-head trace
   over all 1,300 frames, and frame 623 itself now matches.

   **The 8 residual frames are now accounted for too** (2026-08-15 entry).
   8 of 1,300 frame hashes still differ, but nothing in the machine does:
   sampling state at program-defined instants instead of at the host's frame
   timer shows all memory outside the stack page identical at 6,808 in-game
   samples, SP identical, and — once `--ret-addr` was turned on — the live
   stack identical too. The frame hashes differ only because the host samples
   video on a fixed cycle interval while the two engines sit up to one basic
   block apart in *phase* (not rate: 22,147,953 vs 22,147,957 cycles over the
   whole run). It is a property of the measuring apparatus.

   Both scenarios are converted (`play.pkeys`, `play-hires.pkeys`) and both are
   gated. What is genuinely left is `robotron` and `bolo`: the machinery is
   generic, but neither has a coordinate script, and `bolo`'s `--irc1` line is
   commented out in its `decompile.sh`.

   Probe phase 3 is still unbuilt: apple2tc does not yet emit `PROBE_x(...)`
   placeholder sites, so a probe can observe machine state but not the
   generated C's own variables.

**No longer deferred:** the headless-vs-windowed trace comparison was deferred
for want of a display. `a2run` removes the need for one, and the comparison is
now a test that runs on every invocation of `tests/run-tests.sh`.

**Still deferred:** regenerating `decoded/robotron/**` and `decoded/bolo/**`,
which would pick up both the recovery fix and `--extern-routines`.

---

## Traps

Mistakes already made here. The log has the full accounts.

| Trap | Reality |
| --- | --- |
| "A changed frame trace means my code ran" | It only proves *something* changed. Pressing `C` at nine different cycles changed the trace every time; a call counter showed the target ran **zero** times. Instrument and count. |
| "Absent from `BranchTargets`" = unreachable | The list is capped at 500 entries and records only *targets*, so fall-through blocks read as absent either way. Different claims; only "unreachable" is worth relying on, and it needs an argument. |
| `--asm` `Code range` markers as a coverage measure | They appear only at discontinuities — 260 bytes against 3,931 bytes of real instructions. Count instructions. |
| Externalizing entry points will shrink the output a lot | It removed 112 blocks, not ~1,430. Library code is reachable both via its entry points *and* via the start PC, non-additively (112 and 48 alone, 1,530 together). |
| A self-modification warning in game code | Check for the inline-string-after-`JSR` idiom first. Snake Byte's only steady-state "self-modifying" block was the string `"VALUE: "`. |
| Replacing a routine that dispatches through a vector | `COUT` is `JMP ($36)` and the game repoints it. `rom_cout` honours the vector and aborts loudly on an unknown target — the trace cannot catch a wrong guess there. |
| clangd/IDE diagnostics in this repo | It lacks the include paths and reports cascading phantom errors (`CircularList.h file not found`, `s_a` undeclared in `a2rom.c`). Trust `ninja` and the test suite. |
| A bodyless `ir::Function` | `getAddress()` derives from `getEntryBlock()`, which asserts on an empty block list — silent UB under `NDEBUG`. Use `Function::isExternal()`. |
| Adding a `--code-at` target is enough to reach it | It also needs the `ORIGIN` half. Only a block that is a successor of a *dynamic* branch gets an address-to-block map entry; without one the code is decompiled but unreachable, and you still get `Unknown address`. |
| `--code-at` will tell you if you point it at data | It cannot. That untraced bytes are code is exactly what you are asserting. It only checks the target starts a block and the origin is a branch, and warns if the target decodes to an invalid instruction. The real check is that the surrounding data boundaries come out right. |
| The compiler will DCE unreachable blocks | It will not. `dce()` removes only instructions, and every `Void`-typed instruction counts as having side effects. A pass that orphans blocks must delete them itself. |
| Two frame-hash traces that disagree mean one engine is wrong | Not between *different* engines. Frame hashing presumes a shared sampling clock; the generated engine yields only at block boundaries and an interpreter between instructions, so two perfectly equivalent implementations still disagree in 1-3 frame bursts around transitions. Same-engine comparison is fine, and is what `verify.sh` does. |
| Comparing register traces across engines | `CPURegLiveness` and `dce` drop stores to dead registers by design, so the generated code does not maintain `Y` or the flags where nothing reads them. Traces diverge on line 2. `--compat` makes the format diffable, not the content. |
| A rejection test that greps for `FATAL` | Twice during the probe work a test passed while covering nothing, because a *different* check fired first and satisfied the grep. Assert the specific message, and prove the test can fail by deleting the check it covers and watching the suite go red. |
| A probe that produces no output | Says nothing about agreement. A probe on a non-block-head address fires in the interpreter and does not exist in the generated program; the report then reads as agreement while covering less than you think. |
| `snake-byte.lst` shows you every keyboard read | It cannot. `$7541` and `$664A` are reached only through `--code-at` control flow, so the disassembler never traced them and the listing prints them as `DFB` data. `$760F`, the read that takes 6 of the 19 keys in the hires scenario, is visible *only* in the generated C. Grep the C, not the listing, for anything on a `--code-at` path. |
| `--ret-addr` is a cosmetic verification flag | It is on for a correctness reason. Without it a `JSR` pushes the sentinel `$FFFE`, and the inline-data-after-`JSR` idiom — which Snake Byte uses — finds its data by *reading* that address. It costs nothing (same `push16`, different compile-time constant). But it is **not** for a shipped artifact: that wants `ret_addr == 0`, no emulated stack at all. |
| Every `CYCLES`-shaped call site is a program location | It is not. The taken-branch penalty is charged on the *edge*, in a block carrying the branch's address that the program is never actually *at* — so it must not trace or dispatch probes, or one execution of that branch gets reported twice. Hence `AddEdgeCycles`/`CYCLES_EDGE`. 698 ROM addresses are edges and 121 of them are also real block heads, so the two are not distinguishable by address. |
| A plan's list of keyboard-read sites is complete because it was grepped once | Snake Byte's coordinate plan named three sites (`$FD1B`, `$741F`, `$7890`); a fourth, `$6217` — the in-game ingest that clears the strobe and fills the ring buffer at `$623C` — was missing. Recording with only the three captured 11 of 23 keys, not 11 cycle-quantised ones: once a script delivers via `key` at all, the host's per-frame drain stands down entirely (`probe_uses_key()`), so an uncovered site's keys are never delivered, not merely mis-timed. Found by recording and counting, not by reading the disassembly harder. |
| A test that fails proves the check it names | Task 3's own drain-guard regression test specified `--frames=10`. At that frame count the buggy (unguarded) and the fixed (guarded) build produce byte-identical output — the installed keyboard site isn't even reached until roughly frame 8.3, and the one key that would distinguish the two builds is stamped for a point roughly 59 frames further out — so the test failed identically before and after the fix and proved nothing either way. Needed `--frames=100`. A red result is only evidence once you have also seen it turn green for the right reason. |

---

## Open questions

- **The `$93`/`$83` Ctrl-S handshake** in `rom_coutz`: reachable at runtime
  (needs a key pending when a CR is output) but not on this trace. Transcribed
  faithfully, unvalidated by execution.
- **`$66A6`**: `JMP $6655`, an alternate entry to the glyph blitter that skips
  the control-character filter. Nothing references it. Left unimplemented.
