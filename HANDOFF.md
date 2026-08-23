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

Two builds, both verified — do not collapse them into one:

| Target | Source | Role |
| --- | --- | --- |
| `snake-bytec1` | `snake-bytec1.c` | Self-contained reference. ROM decompiled alongside the game; links alone. `play.frames` was recorded from it. |
| `snake-bytec1-ext` | `snake-byte-ext.c` | ROM entry points supplied by hand-written `a2rom.c` + `game.c`. |

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
cd decoded/snake-byte && ./verify.sh                # expect: 4x PASS
./decompile.sh                                      # regenerates both c1 variants
```

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
4. **Phase 1b: retarget the entry to `$3750`.** This is what actually cuts the
   ROM — measured at 1,530 blocks deleted, leaving 4. Needs an entry-state
   snapshot and a re-based trace, because a cold start skips the 168 boot frames
   `play.frames` opens with.

   **There is no `--snapshot-at`, and no format for one.** Stage 7 of the
   host/engine split design was never started; the option does not exist
   anywhere in the tree — re-confirmed 2026-08-23, `grep -rn 'snapshot-at'
   lib/ tools/ include/` returns nothing. Phase 1b is still blocked on it, and
   whoever picks it up is designing it from scratch, not using something
   already built.

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
