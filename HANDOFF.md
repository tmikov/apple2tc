# Handoff: converting Snake Byte into maintainable C

Read this first. It is the entry point for resuming the work on branch
`snake-byte`. Everything below is measured or committed — where something is a
guess, it says so.

**Last commit:** `7479aed`. 38 commits on `snake-byte`, nothing pushed, tree
clean. The `$7541` work landed in five commits (two for `--code-at` in
`tools/apple2tc/`, three for the game); the `$60E7` routine recovery in two.

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
- **`tests/` is decompiler regression only** — hand-written `.s` assembled by
  `a6502`, decompiled, diffed against `.ir` baselines. Game data lives in
  `decoded/<game>/`.

---

## Read these, in this order

| File | What it holds |
| --- | --- |
| `docs/decompiling/playbook.md` | The transferable method: findings, procedure, red-flag table. Maturity is honestly marked — steps 1-4 have been executed, 5-6 have not. |
| `docs/decompiling/decision-log.md` | Append-only rationale. **Never edit existing entries**; add new ones and mark old ones `superseded`. Ends with the coverage measurement and the ordered next steps. |
| `decoded/snake-byte/labels.txt` | Established names, each with its evidence in a comment. |
| `decoded/snake-byte/code-at.txt` | The two hand-asserted reachability claims, each with its argument. Read before adding a third. |
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
- **`--code-at=<file>`** — hand-asserted `ORIGIN TARGET` dynamic branch edges,
  merged into the runtime data before disassembly. `TARGET` is disassembled
  *and* becomes a successor of the branch at `ORIGIN`; the latter is what earns
  it an entry in the generated address-to-block map, without which it would be
  decompiled but unreachable. This is how `$7541` and the reference build's
  `$664A` are reached.
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
cd decoded/snake-byte && ./verify.sh                # expect: 4x PASS
./decompile.sh                                      # regenerates both c1 variants
```

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

Of 19,967 bytes at `$3750-$854E`:

| | bytes | |
| --- | --- | --- |
| Decompiled as code | ~3,931 | 1,696 instructions, 19.7% |
| Known assets | 10,139 | 8KB hi-res title image `$4000-$5FFF`; 2KB level data `$3800-$3FFF` → `$1800`; 768B font `$66A9` |
| Zero-filled buffers | 3,284 | |
| **Unknown nonzero** | **2,657** | untraced code + unidentified tables |

**The remaining job is bounded at ~2,445 bytes** (2,657 less the 212 newly
covered at `$7541-$75B2` and `$75D1-$7632`; the full table has not been
re-measured). Where the rest is:

```
$8390-$84A4  277   $8000-$80D9  218        <- end of binary, nothing known
$823C-$82F0  181   $80FF-$8190  146           reaches it
$831B-$838E  116   $81D5-$8235   97
$606C-$60E3  120   $600E-$6063   86        <- near the hi-res tables; may be data
$7499-$753F  167                           <- inline string data, correctly so
```

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

1. **The two remaining rejection roots.** `$60E7` is done (2026-08-07); it took
   six routines with it, 74 → 81. Left, both blocking 3 apiece:
   - `$6A32` — `Pop8` block `$6AB3` stack level underflow, blocks `$6256`,
     `$6288`. Not yet diagnosed.
   - `$7230` — the inline-string printer. Not recovered but *rewritten*:
     `print_str(const char *)`, with each call site's inline bytes lifted into a
     real string literal. Blocks `$72CE`, `$78B3`, `$7980`.

   **Measure before treating any other rejection as work.** Of 74 rejections in
   the extern build only **7 are in the game range**; the rest is ROM that Phase
   1b deletes wholesale. Filter to `$3750-$854E` first — see the 2026-08-07 log
   entry for the one-liner.
2. **Teach the disassembler the inline-string idiom.** The higher-leverage
   version of what `--code-at` now does by hand: recognise the
   `PLA/PLA … PHA/PHA/RTS` shape, mark the bytes after the `JSR` as a string,
   and resume disassembly past the terminator. `$7230` has 15 call sites and the
   idiom is ubiquitous in Apple II games, so this serves the "repeatable method"
   aim directly. Weigh against item 1 — recovering `$7230` may subsume it.
3. **`$8000-$84A4`** — the largest unknown region. Find what reaches it first.
   Worth trying `--code-at` from candidate dynamic branches once something is
   known to reach it.
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
| Adding a `--code-at` target is enough to reach it | It also needs the `ORIGIN` half. Only a block that is a successor of a *dynamic* branch gets an address-to-block map entry; without one the code is decompiled but unreachable, and you still get `Unknown address`. |
| `--code-at` will tell you if you point it at data | It cannot. That untraced bytes are code is exactly what you are asserting. It only checks the target starts a block and the origin is a branch, and warns if the target decodes to an invalid instruction. The real check is that the surrounding data boundaries come out right. |
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
