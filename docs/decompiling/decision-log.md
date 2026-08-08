# Decompilation decision log

Append-only. Entries are never edited once written — if a decision changes, add a
new entry and set the old one's status to `superseded`, naming the entry that
replaced it. The value of this file is the record of *why*, including decisions
that later turned out wrong.

**Status values:** `proposed` (decided, not yet tested by experience) ·
`validated` (survived contact with the work) · `superseded` (replaced; names its
successor)

**Scope values:** `this game` (specific to one binary) · `apple2tc` (true of this
decompiler) · `6502` (true of 6502 / Apple II work generally)

Decisions that reach `validated` and are scoped `apple2tc` or `6502` are
candidates for promotion into `playbook.md`.

---

## 2026-08-02 — Recover procedures before relooping

**Scope:** 6502 · **Status:** proposed

**Decision:** Procedure recovery must complete before any control-flow
structuring is attempted.

**Evidence:** In `decoded/snake-byte/snake-bytec1.c`, `func_t001` fuses 2,097
basic blocks from unrelated routines into one `for(;;) switch (block_id)`
dispatch. Inside it, `RTS` compiles to a computed edge —
`block_id = find_block_id_func_t001(pop16() + 1)` — and there are 165 such sites.
Statically each is an edge from that `RTS` to every possible return site, making
the CFG near-complete. `block_id` *is* the program counter, and a program whose
PC is data cannot be structured.

**Rejected:** Relooping the mega-switch directly. It would succeed and produce
the same dispatch loop in structured syntax — correct and useless.

---

## 2026-08-02 — Hand-written C, decompiler output as oracle

**Scope:** this game · **Status:** proposed

**Decision:** Write a new Snake Byte source by hand, keeping `snake-bytec1.c`
unchanged as a behavioral reference to diff against. The hand-written file is the
maintained artifact.

**Evidence:** The goal is source worth maintaining and porting. Reaching that
automatically would require the decompiler to infer intent — variable meaning,
struct boundaries, subsystem names — which is far beyond what static analysis
plus runtime tracing can supply.

**Rejected:** (a) Improving the decompiler until its output is readable — helps
robotron and bolo, but readable *game logic* is not an achievable automated
target. (b) A staged mix — deferred rather than rejected; see the tooling entry
below, which takes the mechanical part of it.

---

## 2026-08-02 — c1 output is the primary source, listing is secondary

**Scope:** apple2tc · **Status:** proposed

**Decision:** Work from the `-O3 --irc1` C output. Consult the `--asm` listing
only as reference.

**Evidence:** c1 is a superset of the listing. It has already resolved flag
liveness, introduced SSA temporaries, recovered 53 procedures, resolved block
IDs, and carries exact per-block cycle totals. The listing has none of that.
Using it as primary discards work already done.

**Rejected:** Driving from `snake-byte.lst` because its mnemonics read more
easily than c1's block soup. Readability of the source material is worth less
than the analysis already embedded in c1.

---

## 2026-08-02 — Keep `s_ram` and `a2io`; drop the ROM

**Scope:** this game · **Status:** proposed

**Decision:** Game logic becomes real C, but continues to reach the screen
through `s_ram` and the host through `a2io`. The `$D000–$FFFF` blocks are removed
and replaced with hand-written helpers.

**Evidence:** Of `func_t001`'s 2,097 blocks, 1,442 are Applesoft and Monitor ROM
and only 641 are the game. Yet the game calls just **9 distinct ROM entry
points** (`PLOT`, `HLINE`, `SETCOL`, `SCRN`, `HOME`, `$FC68`, `COUT`, `SETKBD`,
`SETVID`). Roughly 150 lines of C displaces all 1,442 blocks.

**Rejected:** (a) Full native conversion with no `s_ram` — requires reverse
engineering every memory layout, including HGR interleaving, before anything
runs at all. (b) Hand-converting the ROM too — maintaining Applesoft in C for no
gameplay benefit.

---

## 2026-08-02 — Verify by per-frame framebuffer hash

**Scope:** 6502 · **Status:** proposed

**Decision:** Record a golden trace of per-frame framebuffer hashes from the c1
build replaying `play.keys`, and require the rewrite to reproduce it frame for
frame.

**Evidence:** `lib/decapplib/decapplib.c:380` already locks the run to exactly
`1/60 * clock_freq` cycles per frame whenever `--key-file` is supplied, and
`play.keys` is a cycle-stamped input recording. The deterministic harness exists;
only the hashing needs adding.

**Rejected:** (a) Diffing memory-write traces with cycle stamps stripped —
stricter, but it flags every legitimate restructuring, so it only works while the
rewrite still writes in the original order. (b) Swapping one routine at a time
and playing the game — no automated signal.

---

## 2026-08-02 — Keep the emulator thread and exact cycle counts

**Scope:** this game · **Status:** proposed

**Decision:** Retain the emulator thread and its `cycles_expired()` yield, and
carry the exact per-block `CYCLES()` totals through the restructuring.

**Evidence:** The game blocks on input — `$69AD: LDA $C000 / BPL $69AD` and
`$7890` are spin loops that terminate only because the CPU yields to the host
when its cycle budget expires. Without the yield they hang. Separately, the
frame-hash oracle only stays exact if timing does: the game's visible behavior
depends on cycle counts through its spin and delay loops, so approximate timing
would let the game drift and the oracle would stop working. c1 supplies the exact
per-block numbers, so exactness costs bookkeeping rather than analysis.

**Rejected:** (a) Approximate cycle counts — much less bookkeeping, but
verification degrades to playing the game and watching. (b) Inverting control
flow into a host-driven frame callback — the most idiomatic end state, but the
largest single restructuring, and it breaks the oracle while in progress.

---

## 2026-08-02 — Structural conversion globally, then vertical RE slices

**Scope:** 6502 · **Status:** proposed

**Decision:** One global pass converting structure with address-derived names,
then reverse engineering one subsystem at a time, each taken to finished named C
and verified independently.

**Evidence:** The frame-hash oracle reports *that* something broke, not *where*.
Restructuring and renaming simultaneously gives every failure two candidate
causes. Doing structure first keeps behavior mechanically traceable to the c1
source; doing RE in vertical slices afterwards keeps each failure attributable to
one subsystem.

**Correction recorded:** An earlier framing of this called the structural pass
purely mechanical. It is not — procedure recovery carries a judgment tail
(routines the analysis cannot prove, the inline-string idiom, tail calls). The
tail is bounded and enumerable, but it is not zero.

**Rejected:** (a) Stopping at structural conversion with address-derived names —
structurally sound but not the stated goal. (b) Understanding each routine before
writing it, in a single pass — avoids writing code twice, but leaves no runnable,
verifiable intermediate state.

---

## 2026-08-02 — Improve `routines.cpp` rather than write a throwaway script

**Scope:** apple2tc · **Status:** proposed

**Decision:** Fix procedure recovery inside `apple2tc` and add a report mode,
instead of writing an external script to analyze its output.

**Evidence:** `IdentifySimpleRoutines` in `tools/apple2tc/routines.cpp` already
performs procedure recovery, and the IR already holds the CFG, call graph, and
liveness. An external script would re-parse generated C to reconstruct data the
generating tool already has. The fix also benefits robotron and bolo.

Running `--irc1 -v2` on Snake Byte rejects 10 distinct game-range candidates:
seven for "block stack level not zero", two for `Pop8` underflow, one for a
`JmpInd` terminator. The dominant cause is that `int stackLevel = 0;` sits
*inside* the per-block loop (`routines.cpp:149`), so pushes and pops must balance
within a single basic block — rejecting the universal `PHA`-at-entry /
`PLA`-before-`RTS` shape. Among the casualties is `$71F3`, the most-called
routine in the game (15 sites).

**Note:** An initial hypothesis that the `removeInvalidJSRs()` fixpoint
(`routines.cpp:86-92`) was cascading rejections up the call graph proved **wrong**
— the count converges immediately at 53. All losses occur in `scanCandidate`. The
cascade mechanism is real but does not fire on this binary.

**Rejected:** A throwaway script over `snake-byte.lst`. Cheaper once, but
discards the IR and helps no other target.

---

## 2026-08-02 — Game data lives beside the game, not in `tests/`

**Scope:** apple2tc · **Status:** validated

**Decision:** Golden traces, input recordings, and verification scripts go in
`decoded/<game>/`. `tests/` holds only decompiler regression cases.

**Evidence:** `tests/` contains five hand-written `.s` files assembled by
`a6502`, decompiled, and diffed against `.ir` baselines. `decoded/snake-byte/`
already holds `play.keys`, `play.kbd`, and `decompile.sh` — the precedent is
established. A game's golden trace tests the conversion, not the decompiler.

Marked `validated` because it reflects existing repository structure rather than
a prediction.

**Consequence:** A change to *decompiler behavior* still warrants a `tests/`
case. The `routines.cpp` fix alters which routines get promoted, which changes
`.ir` output for every target, so it needs one.

---

## 2026-08-02 — Per-routine stack tracking: measured effect

**Scope:** apple2tc · **Status:** validated

**Decision:** (records outcome of the 2026-08-02 "Improve `routines.cpp`"
entry, commit `ea29cdc`)

**Evidence:** After propagating stack depth along CFG edges instead of
resetting it per basic block, Snake Byte's first pass identifies **75**
routines, up from 53 (`snake-bytec1.c` grows from 108 `func_`/`FUNC_`
declaration+definition lines to **152**). Using the same measure the original
"10 distinct game-range candidates" count used — counting only candidates
that fail `scanCandidate` directly — game-range rejections fall from 10 to
**3**: `$6A32` and `$7230` (`Pop8` stack underflow) and `$7226` (`JmpInd`
terminator). That is exactly the set the earlier entry predicted would
survive, so by that narrow count the predicted 7-of-10 recovery was
**accurate**.

Identity is a different story. Of the 7 candidates that previously failed
their own stack-depth check (`$71F3`, `$6256`, `$6288`, `$6B3D`, `$6C72`,
`$7000`, `$72CE`), only `$6C72` and `$7000` are actually in the new routine
set. The other five now pass `scanCandidate` (the log shows `created
candidate` for all of them) but are removed afterward by the
`removeInvalidJSRs` fixpoint: they `JSR` into `$6A32`, `$7230`, or the
`$60E7`/`$6148`/`$615A` chain, which remain rejected for unrelated reasons
(`Pop8` underflow, and `$60E7`'s own "invalid predecessor" structural issue —
untouched by this fix). The prior entry's note that "the cascade mechanism is
real but does not fire on this binary" no longer holds after the fix — it now
fires both ways.

Counting the cascade properly (all 40 game-range `scanCandidate` attempts vs.
the final routine set, not just direct failures) gives the fuller picture:
game-range rejections fall from **22 to 14**, i.e. **8** routines recovered
net, not 7. Six of those eight were never predicted (`$64C8`, `$6594`,
`$69C3`, `$6AB8`, `$6BEF`, `$7642`): they were unblocked because two ROM
Monitor entry points the game calls, `$F800` (`MON_PLOT`) and `$F871`
(`SCRN`), were themselves victims of the identical per-block stack bug and
are now recovered directly, which cascades to their game-range callers
through the same mechanism that used to cascade rejections. So the estimate's
*count* held, but for the wrong reason and the wrong routines — worth
recording because a future estimate of this shape should account for the
cascade, not just the direct failures.

**Junk check:** Of the 22 newly-promoted routines (8 game-range: `$64C8
$6594 $69C3 $6AB8 $6BEF $6C72 $7000 $7642`; 14 ROM-range: `$E484 $E5D4 $E5E2
$E5E6 $E600 $E604 $F800 $F819 $F828 $F836 $F871 $FB60 $FC58 $FCA8`), every
one contains a real `return;` in `snake-bytec1.c` — none are never-returning
stubs. Across the whole 75-routine set there is exactly one function with no
`return;`: `FUNC_BCC` at `$0090`, an invalid-instruction trap that compiles
to `fprintf(...); error_handler(...)` and nothing else. That routine was
already promoted in the pre-fix 53-routine baseline (confirmed by rebuilding
the old `routines.cpp` and rerunning), so it is not new noise from this
change. On this binary the fix does not appear to promote any never-returning
junk; the 22 newly-promoted routines look like real subroutines, including
two well-known ROM entry points (`MON_PLOT`, `SCRN`) that were previously
stuck inside the mega-switch dispatch.

---

## 2026-08-03 — Externalizing ROM entry points: measured effect

**Scope:** apple2tc · **Status:** validated

**Decision:** (records the outcome of Phase 1a, "cut the ROM")

**Evidence:** Snake Byte calls 9 Apple II ROM entry points. Declaring them
external via a new `--extern-routines` pass, and supplying them from a
hand-written `a2rom.c`, gives:

| Metric | Before | After |
| --- | --- | --- |
| Blocks in the start function | 2064 | 1952 (112 deleted) |
| ROM blocks in `func_t001` | 1430 | 1334 |
| Game blocks in `func_t001` | 620 | **564** |
| Routines identified | 75 | 72 |
| Game routines accepted | 26 | **28** |
| Rejected game candidates | 14 | **12** |
| Rejection roots | 5 | **3** |

`$71F3`, the game's most-called routine, recovered with its 15 call sites — it
had been blocked transitively by `COUT`'s `JMP (CSWL)` indirect terminator. That
was the phase's headline hypothesis and it held. Verified frame-identical against
`play.frames` across the full 1300-frame session, cycle counts included.

**The plan's central premise was wrong.** It predicted externalizing the 9 would
prune ~1429 ROM blocks. It prunes **112**. The ROM is reachable by two
independent routes — the 9 entry points, and the BASIC boot path (start PC is
`$FA62` RESET; `$00C8`, Applesoft's `CHRGET` RTS dispatch, reaches deep into the
interpreter). Severing both takes externs *and* retargeting the entry to `$3750`,
and they are strongly non-additive: measured separately at 112 and 48 blocks, but
1530 together. The ROM cut is therefore Phase 1b, not this phase.

**Three things the spike found that were not anticipated:**

1. **The compiler has no dead-block elimination.** `dce()` removes only
   user-less, side-effect-free *instructions*, and `hasSideEffects()` is true for
   every `Void`-typed instruction including all terminators. The only
   `eraseBasicBlock` call in the tree is inside `simplifyCFG`'s straight-line
   merge. Any pass that makes blocks unreachable must delete them itself.
2. **`Function::getAddress()` derives from `getEntryBlock()`**, which `assert`s
   on an empty block list — so a bodyless function is silent UB under `NDEBUG`,
   and `CPURegLiveness` calls it unconditionally at `-v2`. Externals need an
   explicit flag with a stored address, not just an empty block list.
3. **Externalizing a routine deletes everything only it reached.** `GBASCALC`,
   `PLOT1` and the entire `$FB78` `COUTZ` subtree disappeared along with the 9,
   so `a2rom.c` had to supply them too. Correct behaviour, but it triples the
   hand-written surface over the naive estimate.

**Rejected:** the spec's framing, which replaced the ROM only in a hand-written
source file starting at `$3750`. That cannot be checked against `play.frames` at
all — a cold start skips the 168 frames of BASIC boot the trace opens with — so
nothing would have been verifiable until the phase was substantially complete.
Keeping the boot path and swapping only the 9 leaf routines made every step
checkable against the existing trace.

---

## 2026-08-03 — `rom_cout` dispatches through the vector and aborts on unknown targets

**Scope:** 6502 · **Status:** proposed

**Decision:** `rom_cout` reads `CSWL/CSWH` (`$0036/$0037`); if the vector is
`$FDF0` it runs the `COUT1` equivalent, and for any other target it prints the
address and calls `error_handler`. No silent fallback.

**Evidence:** `COUT` is `JMP ($36)`. Snake Byte hooks it: `$6641` points the
vector at `$664A`, its own hi-res text renderer — hand-decoded from the binary as
a glyph blitter that maps the text cursor (`BASL/BASH` + `CH`) to a hi-res
address by adding `$1C` to the high byte, indexing an 8-byte-per-glyph font at
`$66A9`.

The run-data records `$FDED -> $FDF0` only, and `$6641` never executed at all —
neither it nor its callers `$7485`/`$793F` appear in `BranchTargets`. So the
tracer classified `$664A` as *data* and it is absent from the generated C
entirely.

**The oracle structurally cannot check this path.** A wrong choice here would
pass all 1300 frames. Aborting loudly at the moment it first matters is the only
honest option; a silent fallback to `COUT1` would render text with the wrong
font, on the wrong page, undetectably.

**Known gap:** `$664A` remains unimplemented. Phase 1c must either hand-decode it
into the rewrite or add it to the run-data's `BranchTargets` so the decompiler
emits it as code.

---

## 2026-08-03 — Coverage measured by instrumentation, not by the trace file

**Scope:** apple2tc · **Status:** validated

**Decision:** To determine whether a code path actually executes, instrument and
count. Do not infer it from `BranchTargets` in the run-data.

**Evidence:** While closing out which `COUTZ` paths Snake Byte exercises, reading
`BranchTargets` gave wrong answers twice. The list is capped at 500 entries and
records only *branch targets*, so a fall-through block reads as absent whether or
not it ran — `$FBDD` shows absent purely because `$FBDB BNE` falls into it.
Temporarily adding per-block counters and replaying the session gave the truth:

- The **bell** path is live — 192 speaker clicks. Not from the game: ROM RESET
  reaches it via `$FF3A` (`LDA #$87; JMP $FDED`). An earlier report had called
  this probably-unreachable; instrumentation overturned that.
- **Backspace is provably unreachable from this game**, by argument rather than
  absence of evidence: no byte Snake Byte emits is `$88`. The `$08` inside the
  inverse-"CRASH" string at `$7868` has bit 7 clear, so `$FC01` routes it to the
  glyph path long before the `CMP #$88` at `$FC0C`.
- The `$93`/`$83` **Ctrl-S handshake** is genuinely reachable at runtime but not
  on this trace — it needs a key pending when a CR is output.

"Absent from the trace file" and "unreachable" are different claims. Only the
second is worth relying on, and it takes an argument to establish.

---

## 2026-08-03 — Keep the self-contained build; the extern build is additional

**Scope:** apple2tc · **Status:** validated

**Decision:** `decompile.sh` generates **two** variants and CMake builds both.
`snake-bytec1.c` is generated with no externs — self-contained, links on its own,
and remains the reference build. `snake-bytec1-ext.c` is the extern variant, and
is compiled only via the `snake-byte-ext.c` wrapper together with `a2rom.c`.
`verify.sh` checks both against `play.frames`.

**Evidence:** Phase 1a initially made `--extern-routines` unconditional in
`decompile.sh` and repointed the existing `snake-bytec1` target at the wrapper.
That silently destroyed a property the project had always had — the generated C
being a standalone file that compiles and runs the game — and left **no build of
the pristine decompilation at all**. It also contradicted the Phase 0 spec's own
commitment that "both binaries stay in `CMakeLists.txt` throughout, so the oracle
is always buildable".

The verification that passed was still meaningful, because `play.frames` had been
recorded from the pre-extern build. But the reference existed only as a
historical artifact, not as something rebuildable — so re-recording a trace or
bisecting a divergence would have had nothing to compare against.

Keeping both is also a strictly stronger test: the reference build is now a live
control rather than a historical one, and the two binaries producing byte-identical
1300-frame traces is what actually proves the hand-written ROM replacements match
the decompiled ROM.

**Correction recorded:** the mistake was replacing a build target rather than
adding one. When a change makes generated output depend on hand-written code,
keep the independent variant building — the cost is one extra generated file and
one target.

---

## 2026-08-04 — Hand-decompile what the tracer never saw; cross-check with the tool

**Scope:** 6502 · **Status:** validated

**Decision:** Routines the recording never reached are decompiled **by hand**
from the binary. The committed run-data is not edited to make the tool emit them.

**Evidence:** `$664A` is Snake Byte's own `COUT` handler, installed by `$6641`
pointing `CSWL/CSWH` at it. The recorded session never runs `$6641`, so apple2tc
classified the bytes as data and emitted nothing — yet they decode cleanly as a
hi-res glyph blitter, and the routine is genuinely reachable: pressing **C** at
the attract screen reaches it, 177 times in 390 frames.

Decoded by hand into `decoded/snake-byte/game.c`. The alternative — adding
`$664A` to `snake-byte.json`'s `BranchTargets` — was rejected: the run-data is a
recording of what happened, not a config file, and editing it to assert
reachability the run never observed destroys its value as evidence for all later
work.

**The tool is still useful as a cross-check.** A *scratch* copy of the run-data
(in `/tmp`, never committed) with `$664A` added produced an independently
generated implementation. Building both and replaying the same scenario gave
byte-identical frame traces for 393 frames, with 177 executions of the routine.
That validates the hand decode against the tool without corrupting the recording,
and it supplied the exact `CYCLES()` constants.

**Two things instrumentation caught that inference got wrong:**

1. A first attempt pressed `C` at nine different cycles and the trace *changed*
   each time, which looked like success. A call counter showed `$664A` ran **0**
   times — the keypress was landing in a different poll loop entirely. Only
   SPACE-then-`C` reaches it. A changed trace is not evidence that the intended
   code ran.
2. An exit-status check appeared to show the reference build surviving the
   scenario. It was reading `grep`'s status through a pipe, not the binary's.

**Consequence:** `verify.sh` gained a third check, `play-hires.keys` /
`play-hires.frames`, capped at 390 frames — at 393 the same screen reaches
`$7541`, which the recording also never covered, and both builds stop with
`Unknown address $7541`. It is a regression test rather than an oracle: only the
ext build can run it, so its authority rests on the one-time cross-check above.
`$7541` is the next hand-decompilation target.

---

## 2026-08-04 — How much of Snake Byte is actually decompiled

**Scope:** this game · **Status:** validated

**Decision:** (scoping measurement, not a decision — recorded because it bounds
every remaining phase)

**Evidence:** Of the 19,967 bytes at `$3750-$854E`:

| | bytes | |
| --- | --- | --- |
| Decompiled as code | ~3,931 | 1,696 instructions, 19.7% |
| Known assets | 10,139 | see below |
| Zero-filled buffers | 3,284 | |
| **Unknown nonzero** | **2,657** | untraced code + unidentified tables |

Assets identified by inspection rather than assumption:

- `$4000-$5FFF` (8,192 bytes) — a hi-res title image. 28% of it is the single
  byte `$2A` (`0101010`, the classic hi-res dither), only 1.7% has the colour
  bit set.
- `$3800-$3FFF` (2,048 bytes) — level/map data, copied to `$1800` by the startup
  relocator at `$3750`. Disassembling it yields mostly invalid opcodes and the
  values cluster in `$02-$1D`, consistent with tile indices. **Nothing is
  decompiled at `$1800-$1FFF`**, which is correct: it is data, not code.
- `$66A9-$69A8` (768 bytes) — the font used by `$664A`, 8 bytes per glyph.

**So the remaining hand-decompilation job is bounded at ~2,657 bytes**, roughly
900-1,300 instructions, against 3,931 already done. The game is about 60%
code-complete and finishing it is finite work — not the 80% figure a naive
code-vs-data count suggests.

Where the unknown bytes are:

```
$8390-$84A4  277   $8000-$80D9  218   $823C-$82F0  181
$7499-$753F  167   $80FF-$8190  146   $606C-$60E3  120
$831B-$838E  116   $7579-$75E7  111   $81D5-$8235   97   $600E-$6063   86
```

Two clusters: `$7499-$75E7` (the screen reached by pressing C, containing
`$7541`) and `$8000-$84A4` (~1,035 bytes at the end of the binary, entirely
untraced — nothing yet known to reach it).

**Method note:** the `--asm` listing's `Code range` markers are not a coverage
measure — they only appear at discontinuities and totalled 260 bytes here
against 3,931 bytes of actual instructions. Count instructions, not range
markers.

---

## 2026-08-04 — Next steps, in order

**Scope:** this game · **Status:** proposed

1. **`$7541`** — hand-decompile. Smallest target, already reachable (SPACE then
   C), and unblocks `play-hires.frames` past its 390-frame cap. It is a
   six-iteration loop over `$6C63,X` calling `$7590`, so it pulls in a small
   cluster with it.
2. **The three remaining rejection roots** — `$7230` (inline-string printer,
   `Pop8` underflow, blocks 3 game routines), `$60E7` ("invalid predecessor inst
   RTS at $6147", blocks 4), `$6A32` (`Pop8` underflow, blocks 2). Decompiler-side
   work on already-traced code; recovers ~9 routines and improves the reference
   everything else is written from. Cheap, so do it before the big unknown block.
3. **`$8000-$84A4`** — the largest unknown region. Requires first finding what
   reaches it; nothing in the recording does.
4. **Phase 1b** — retarget the entry to `$3750`. This is what actually cuts the
   ROM (measured: 1,530 blocks, leaving 4). Needs an entry-state snapshot and a
   re-based trace, because a cold start skips the 168 boot frames `play.frames`
   opens with.

Deferred and still open: the headless-vs-windowed trace comparison (needs a
display), and regenerating `decoded/robotron/**` and `decoded/bolo/**`, which
would now pick up both the recovery fix and `--extern-routines`.

---

## 2026-08-05 — `--code-at`: hand-asserted branch edges as a separate input

**Scope:** apple2tc · **Status:** validated

**Decision:** Addresses the recording never reached are made reachable with a
new `apple2tc --code-at=<file>` option. Each line is `ORIGIN TARGET`: the
dynamic branch at `ORIGIN` can transfer control to `TARGET`. The edge is merged
into the runtime data before disassembly — `TARGET` is added to `branchTargets`
so it gets disassembled, and to `allBranches[ORIGIN]` so it becomes a successor
of that branch. The second half is the load-bearing one: only a *dynamic block*
gets an entry in the generated address-to-block map, and without a map entry the
target is decompiled but unreachable.

**Why a separate file and not `snake-byte.json`:** the JSON is a recording of
what actually happened. Editing it to assert reachability the run never observed
destroys its value as evidence. These edges are asserted, so they live in
`decoded/snake-byte/code-at.txt`, where each one carries the argument for why it
is real and can be reviewed independently.

**Tension, recorded deliberately:** the working agreement in `HANDOFF.md` says
"hand-decompile what the tool cannot reach; do not feed the decompiler more
input to route around a gap." This option *is* more input. It was chosen over
hand-decompiling `$7541` after the alternatives were costed:

- Hand-decompiling needed ~300 bytes of C (`$7541`, `$7590`, `$75D1`, `$615A`,
  `$60E7`) **and** a new integration mechanism, because there was no way to call
  hand-written code at an untraced address. `$664A` only works because `$FDED`
  is an externalised ROM vector that hand-written `rom_cout` dispatches;
  `--extern-routines` cannot help in general, since `loadExternRoutines` rejects
  any address that is not already a known block.
- Teaching the disassembler the inline-string idiom directly was judged higher
  leverage but heuristic.

**Evidence:** `7251 7541` alone moved the game from 1,490 to 1,576 distinct
decompiled instruction addresses in `$3750-$854E`, newly covering `$7541-$75B2`
and `$75D1-$7632`. The gap between them, `$75B3-$75D0`, is the CH/CV cursor
tables and the direction-glyph table — the disassembler classified them as data
unaided, which is the check that the edge was pointed at real code.

**Limits, honestly:** whether untraced bytes are code is exactly what the option
asserts, so a target pointing at data cannot be diagnosed in general. What *is*
checked and fatal: the target must start a basic block, the origin must be the
start of a disassembled instruction, and that instruction must be one GenIR
consults branch targets for (RTS, RTI, JMP, JSR, conditional). A target that
does not itself decode to a valid instruction warns but does not fail — and it
is deliberately the first instruction, not "the block ends invalid", which fires
on real code that runs into undecodable bytes past a `BRK`.


---

## 2026-08-05 — `[hires]` promoted from regression test to cross-check

**Scope:** this game · **Status:** validated

**Decision:** `verify.sh` now replays both scenarios against both builds, and
`play-hires.frames` is recorded from the reference build like `play.frames`.

**Why this became possible:** the `[hires]` scenario used to run only on the
extern build, because `$664A` existed only as hand-written C in `game.c`. Its
authority came from a one-time manual cross-check. Adding `FDED 664A` to
`code-at.txt` gives the *reference* build a decompiler-generated `$664A`
(`$664A-$66A3`, correctly stopping before the font at `$66A9`). In the extern
build `$FDED` is replaced by hand-written `rom_cout`, so the generated `$664A`
is deleted as unreachable and `game_cout_hook` runs instead. The two builds
therefore execute genuinely different code for the same scenario and are
compared frame for frame on every run.

**Evidence:** 1,300 frames, identical video hashes and cycle counts on both
builds. `play.frames` was re-recorded and is byte-identical to the committed
version, confirming the added edges changed no existing behaviour.

**Scenario extended:** `play-hires.keys` now presses six keys after `C`,
reassigning all six directions to `W A D X Q E`. Verified via `--trace-mem`:
`$757C` writes `$D7 $C1 $C4 $D8 $D1 $C5` to `$6C63-$6C68` and `$7587` restores
CSWL/CSWH to `$FDF0`. Without those keys the trace only covers the screen draw
and the blink loop, never the accept/write-back/exit path.

**Coverage note:** the byte table in the 2026-08-04 entry is now stale by the
212 bytes of `$7541-$75B2` and `$75D1-$7632`. It has not been re-measured.
