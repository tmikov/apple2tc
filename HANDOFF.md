# Handoff: converting Snake Byte into maintainable C

Read this first. It is the entry point for resuming the work on branch
`snake-byte`. Everything below is measured or committed — where something is a
guess, it says so.

**Last commit:** `4267ab0`. 30 commits on `snake-byte`, nothing pushed, tree clean.

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
- **Hand-decompile what the tool cannot reach.** Do not feed the decompiler more
  input to route around a gap.
- **Never edit `snake-byte.json`.** It is a recording of what actually happened
  during a play session. Editing it to assert reachability the run never
  observed destroys its value as evidence. Use a *scratch* copy in `/tmp` when
  you want the tool's opinion on something it did not trace.
- **`tests/` is decompiler regression only** — hand-written `.s` assembled by
  `a6502`, decompiled, diffed against `.ir` baselines. Game data lives in
  `decoded/<game>/`.

---

## Read these, in this order

| File | What it holds |
| --- | --- |
| `docs/decompiling/playbook.md` | The transferable method: findings, procedure, red-flag table. Maturity is honestly marked — steps 1-4 have been executed, 5-6 have not. |
| `docs/decompiling/decision-log.md` | Append-only rationale. **Never edit existing entries**; add new ones and mark old ones `superseded`. Ends with the coverage measurement and the ordered next steps. |
| `decoded/snake-byte/labels.txt` | 27 established names, each with its evidence in a comment. |
| `docs/superpowers/specs/2026-08-02-snake-byte-proper-c-design.md` | The original design. Parts are superseded — the log says which. |

The two plans in `docs/superpowers/plans/` are executed and historical.

**Read the log before acting on anything that seems obvious.** It deliberately
preserves the wrong turns so they are not repeated. See "Traps" below.

---

## Where things stand

### Decompiler (`tools/apple2tc/`)

- **Per-routine stack tracking.** `scanCandidate` used to balance the 6502 stack
  per *basic block*, rejecting the universal `PHA`-at-entry / `PLA`-before-`RTS`
  idiom. Depth now propagates along CFG edges, required zero only at `RTS`.
  Snake Byte: **53 → 75** routines.
- **`--routines-report=<path>`** — every candidate, accepted or rejected, with
  block sets, call sites, dominator chains and natural loops. This is the input
  for hand-writing structured C. Note it reflects the CFG *before* `simplifyCFG`,
  so its blocks are more granular than `--irc1` output.
- **`--extern-routines=<file>`** — maps addresses to C function names; calls
  become calls to bodyless `Function`s and unreachable blocks are deleted.
- **`Dominators.{h,cpp}`** — Cooper-Harvey-Kennedy, validated against a DAG, a
  single-loop routine and a nested delay loop. The codebase had none before.

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
cd decoded/snake-byte && ./verify.sh                # expect: 3x PASS
./decompile.sh                                      # regenerates both c1 variants
```

`verify.sh` runs three scenarios:

```
PASS [ref]:   1300 frames match     self-contained build
PASS [ext]:   1300 frames match     hand-written ROM routines
PASS [hires]:  390 frames match     $664A path (177 calls)
```

It replays cycle-stamped keys headless and compares per-frame FNV-1a hashes of
video memory (mode + text page + hi-res page) *and* cycle counts. `--record`
runs the reference build twice and refuses to write unless both agree.

**Never edit a `.frames` file to make a test pass.** The trace is the reference.

The `[hires]` check is a regression test, not an independent oracle: only the ext
build can run it, since the reference build has no `$664A`. Its authority comes
from a one-time cross-check against a decompiler-generated version.

---

## Coverage — what is left

Of 19,967 bytes at `$3750-$854E`:

| | bytes | |
| --- | --- | --- |
| Decompiled as code | ~3,931 | 1,696 instructions, 19.7% |
| Known assets | 10,139 | 8KB hi-res title image `$4000-$5FFF`; 2KB level data `$3800-$3FFF` → `$1800`; 768B font `$66A9` |
| Zero-filled buffers | 3,284 | |
| **Unknown nonzero** | **2,657** | untraced code + unidentified tables |

**The remaining hand-decompilation job is bounded at ~2,657 bytes** — roughly
900-1,300 instructions, against 3,931 already done. Two clusters:

```
$7499-$753F  167   $7579-$75E7  111        <- the screen reached by pressing C
$8390-$84A4  277   $8000-$80D9  218        <- end of binary, nothing known
$823C-$82F0  181   $80FF-$8190  146           reaches it
$831B-$838E  116   $81D5-$8235   97
$606C-$60E3  120   $600E-$6063   86        <- near the hi-res tables; may be data
```

---

## Next task: `$7541`

Hand-decompile it into `game.c`.

- **Reachable**: `decoded/snake-byte/play-hires.keys` (SPACE at 5,728,811 then
  `C` at 6,500,000). Today both builds stop at frame 393 with
  `Unknown address $7541`, which is why `play-hires.frames` is capped at 390.
- **What it is**: `LDX #$00 / LDA $6C63,X / JSR $7590 / INX / CPX #$06 / ...` —
  a six-iteration loop over a table at `$6C63`, calling `$7590`. Expect it to
  pull in a small cluster.
- **How**: disassemble with `id` (do not decode by hand), write C into `game.c`
  matching the conventions already there, then cross-check against a
  decompiler-generated version built from a **scratch** run-data copy with the
  address added to `BranchTargets` and to `$FDED`'s `Branches` entry. That is how
  `$664A` was validated: identical frame traces, 177 executions.
- **Then**: extend `play-hires.frames` past 390 frames and re-record.

### After that

2. **The three rejection roots.** `$7230` (inline-string printer, `Pop8`
   underflow, transitively blocks 3 game routines), `$60E7` ("invalid predecessor
   inst RTS at `$6147`", blocks 4), `$6A32` (`Pop8` underflow, blocks 2).
   Decompiler-side work on already-traced code; recovers ~9 routines and improves
   the reference everything else is written from. Cheap — do it before item 3.
   `$7230` is not recovered but *rewritten*: `print_str(const char *)`, with each
   call site's inline bytes lifted into a real string literal.
3. **`$8000-$84A4`** — the largest unknown region. Find what reaches it first.
4. **Phase 1b: retarget the entry to `$3750`.** This is what actually cuts the
   ROM — measured at 1,530 blocks deleted, leaving 4. Needs an entry-state
   snapshot and a re-based trace, because a cold start skips the 168 boot frames
   `play.frames` opens with.

**Deferred:** the headless-vs-windowed trace comparison (needs a display; this
environment has no usable X server), and regenerating `decoded/robotron/**` and
`decoded/bolo/**`, which would pick up both the recovery fix and
`--extern-routines`.

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
| The compiler will DCE unreachable blocks | It will not. `dce()` removes only instructions, and every `Void`-typed instruction counts as having side effects. A pass that orphans blocks must delete them itself. |

---

## Open questions

- **`$8000-$84A4`** (~1,035 bytes): nothing in the recording reaches it. Is it
  code for an unplayed game mode, or data? Unknown.
- **`$600E-$6063` and `$606C-$60E3`**: adjacent to the hi-res address tables.
  Probably more tables, not verified.
- **The `$93`/`$83` Ctrl-S handshake** in `rom_coutz`: reachable at runtime
  (needs a key pending when a CR is output) but not on this trace. Transcribed
  faithfully, unvalidated by execution.
- **`$66A6`**: `JMP $6655`, an alternate entry to the glyph blitter that skips
  the control-character filter. Nothing references it. Left unimplemented.
