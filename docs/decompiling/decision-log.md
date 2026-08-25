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

---

## 2026-08-07 — A routine entry can also be a call's return point

**Scope:** apple2tc · **Status:** validated

**Decision:** `scanCandidate` now accepts an `RTS` predecessor of a routine
entry when that entry is also the fall block of a `JSR` from outside the
routine, and the extraction retargets those return edges.

**Evidence:** The shape is a caller that does `JSR <helper>` and falls straight
into a block which is itself a subroutine. Snake Byte's `$60E4` calls `$6127`
and falls into `$60E7`, while `$6148` and `$615A` call `$60E7` directly. That
gives `$60E7` two predecessor edges for *one* control transfer — the JSR, whose
fall operand is `$60E7`, and `$6127`'s `RTS` at `$6147`, which returns there.
The JSR leg was already accepted as a jump-in; rejecting the RTS leg cost six
routines through the invalid-JSR cascade. 74 → 81 identified, game-range
rejections 12 → 7.

**Rejected:** Deleting the redundant `RTS → fall-block` operand instead. It
looks like pure CFG cleanup, but that operand is what puts the address in the
generated block map; without it a real return to that address fails at run time
with `Unknown address`.

**What the fix actually cost.** Accepting the candidate was one line; three
further defects only surfaced afterwards, each caught by running rather than by
reading:

1. The predecessor snapshot is stale by the time the edges are retargeted,
   because converting a predecessor erases it. Reusing it walks freed memory.
2. The block to retarget *to* cannot be assumed to exist. `scanCandidate` runs
   over the whole function before anything is split out, so an earlier
   extraction can rewrite away the JSR that would have built it. Visible only
   in ROM code (`$FD8E`, `$DB5C`) — the game range never hit it.
3. That block has to take over the entry's address, or a JSR falling into it
   cannot compute what it pushes and a return to it is not in the block map.
   This path had simply never executed before, since no candidate of this shape
   ever got past the predecessor check.

**Method note:** the reproduction, `tests/retpoint.s`, needs `--code-at` to
supply the return edges. An `RTS` only gets successor operands from observed
runtime data, so with no recording the edges do not exist, every routine is
trivially accepted, and the bug does not reproduce at all. The tool built for
the `$7541` work turned out to be what made this one testable in isolation.

**Coverage note:** the two roots left in the game range are `$6A32` (`Pop8`
underflow at `$6AB3`, blocks `$6256` and `$6288`) and `$7230` (the inline-string
idiom, blocks `$72CE`, `$78B3`, `$7980`). Everything else rejected is ROM, which
Phase 1b deletes wholesale — worth measuring before treating any of it as work.

---

## 2026-08-07 — `$6A32` is correctly rejected, not a decompiler defect

**Scope:** this game · **Status:** validated

**Decision:** `$6A32` stays rejected. It is not over-strictness, so there is
nothing to fix in `routines.cpp`; it belongs on the manual-rewrite list.

**Evidence:** the block the analysis complains about, `$6AB3`, is:

```
6AB3: PLA / PLA / JMP $6315
```

a non-local exit. Having exhausted its options the routine pops *its own* return
address and tail-jumps, so control never comes back to the `JSR $6A32` at
`$6256` or `$6288`; `$6315` runs in place of the rest of the caller and its
eventual `RTS` returns to the caller's caller. "`Pop8` block `$6AB3` stack level
underflow" is an accurate description: the routine really does read below its
own frame. A simple C function cannot express that, and the two dependents are
rejected for the same underlying reason rather than by a cascade artefact.

**Consequence:** of the three game-range roots named on 2026-08-04, one was a
genuine bug (`$60E7`, fixed), one is a genuine non-routine (`$6A32`, here), and
one is an idiom needing a source-level rewrite rather than recovery (`$7230`).
A rejection reason is a hypothesis about the decompiler; check it against the
6502 before assuming the tool is wrong.

---

## 2026-08-07 — The inline-string call sites, enumerated

**Scope:** this game · **Status:** validated

**Decision:** Added `7251 794D` to `code-at.txt` — the last unreached
continuation of an inline string.

**Evidence:** rather than discovering these one crash at a time, all ten
`JSR $7230` sites were enumerated by scanning the binary for `20 30 72` and
walking each following `$00`-terminated string:

```
$72D6 "SCORE: "        -> $72E2     $73B6 "LEVEL: "              -> $73C2
$7305 "HI SCORE: "     -> $7314     $73F1 "HOW MANY PLUMS (0-2)?"-> $7414
$733B "APPLES LEFT: "  -> $734D     $748C the key-redef screen   -> $7541
$7369 "VALUE: "        -> $7375     $7865 "PRESS SPACE TO..."    -> $7886
$7390 "SNAKES LEFT: "  -> $73A2     $7942 "BONUS: "              -> $794D
```

Nine of the ten resume points were already decompiled; the recording simply
never displayed a bonus. This also confirms after the fact that the `$748C ->
$7541` edge asserted on 2026-08-05 was the right one. Newly covered:
`$794D-$797D`, 22 instruction addresses.

**Method note:** enumerating the idiom's call sites is cheap and bounds the
question exactly. Do that before asserting edges one at a time — the scan tells
you both how many there are and which ones the recording already covered.

---

## 2026-08-07 — `$8000-$853D` is a vector display list, not untraced code

**Scope:** this game · **Status:** validated

**Decision:** The largest remaining unknown region is **data**, and the open
question from 2026-08-04 is closed. Nothing reaches it because nothing jumps
into it; it is read as a byte stream.

**Evidence:** `id xref $8000 $84A4` produced five code candidates, and every one
was a false positive — three were font bytes inside `$66A9-$69A8`, and `$6C22`
decodes `JMP ($80C9)` only because `$6C20` is `LDA $6C46` and the scan reads
mid-instruction. The one real hit came from the data scan: `$7118`/`$711C` store
`$00`/`$80` into `$0A`/`$0B`, and `$7019` is

```
7019: LDY #$00 / LDA ($0A),Y / INC $0A / BNE +2 / INC $0B / RTS
```

a stream reader. `$7100` interprets what it returns as ASCII commands:

| cmd | bytes | action |
| --- | --- | --- |
| `H` | +4 | colour, then `HLINE` (lo-res) and `hgr_draw_hline` (hi-res) |
| `V` | +4 | colour, then `hgr_draw_vline` |
| `P` | +3 | colour, then `MON_PLOT` and `hgr_draw` |
| `T` | +1 | stored to `$0304` |
| `E` | — | reset `$0303` to 1 and restart |
| `*` | — | end of this script |

`$7113` skips `$0303 - 1` `*`-terminated scripts before interpreting, so `$0303`
is a 1-based level index — which is what `init_0300` at `$376E` sets to 1.

**Confirmation:** the grammar parses `$8000-$853D` with no leftovers — 29
scripts, 133 `H`, 129 `V`, 1 `T`, 29 `*`, and a final `E` at `$853D` that loops
back to the first, followed by zero fill to `$854E`. A grammar derived from the
interpreter consuming the whole region exactly is much stronger evidence than
any individual byte would be.

Note it draws each shape *twice*, in lo-res and hi-res. That matches the earlier
finding that the never-displayed lo-res page is the logical game board read back
via `SCRN` for collision detection: the display list builds the board and the
picture of it from one description.

**Coverage:** ~1,035 bytes move from "unknown nonzero" to a known asset,
leaving roughly 1,400. What is left is `$606C-$60E3` and `$600E-$6063` near the
hi-res tables, and scattered smaller gaps.

**Method note:** `xref` is worth reaching for before hand-reading a disassembly,
but restrict the search range. Unrestricted it drowns in a data region's noise —
`xref $000A $000B` over the whole binary is unreadable, and over `$69A9-$7FFF`
it returns exactly the five instructions that use the pointer.

---

## 2026-08-07 — `$6000-$60E3` is table, with derived extents

**Scope:** this game · **Status:** validated

**Decision:** The last two "may be data" ranges, `$600E-$6063` and
`$606C-$60E3`, are the hi-res drawing tables. They were only ever "unknown"
because the coverage measurement counted anything neither decompiled nor on the
known-asset list, and these tables were labelled without extents.

**Evidence, derived rather than assumed:**

| range | bytes | what |
| --- | --- | --- |
| `$6000-$602F` | 48 | line address low bytes |
| `$6030-$605F` | 48 | line address high bytes |
| `$6060-$6063` | 4 | mask, rewritten at run time by `hgr_set_masks` |
| `$6064-$60E3` | 128 | pattern table |

228 bytes, no gaps, ending exactly where `hgr_draw` begins at `$60E4`.

The 48 pairs decode to `$2000 $3000 $2080 $3080 …`, the classic interleaved
hi-res layout, every one inside page 1 (`$2000-$3FFF`). The pattern table's size
comes from its index: `$60F7` builds `$06` as `$01*8 + bit*4 + ($02 & 3)`, and
scanning the display list shows `$01` takes every value `0..15`, so the index
reaches 127 exactly — 128 entries, which lands on `$60E4`.

**Method note:** two independent facts agreeing is what makes this solid — the
arithmetic bounds the index at 127, and the table happens to end where the next
routine starts. Either alone would be suggestive; together they are conclusive.
Deriving a table's extent from the range of its index is generally cheaper than
staring at the bytes, and it produces an argument rather than an impression.

---

## 2026-08-07 — Coverage becomes a generated report, not a hand count

**Scope:** apple2tc · **Status:** validated

**Decision:** `--coverage` plus `--known-data` replace the hand-written coverage
table, and `decompile.sh` regenerates `coverage.txt` alongside the C.

**Evidence for doing it at all:** the 2026-08-04 count was already wrong three
days later, and re-deriving it meant redoing the work. It had also missed a
region outright — `$799B-$7FFF`, 1,637 bytes — which the report surfaced on its
first run. A number that bounds every remaining phase should not be a number
somebody typed.

**The design point:** counting "not code" is useless for a game, where most of
the binary legitimately is not code. The report is only informative because
`known-data.txt` declares the identified regions, splitting the remainder into
*unknown zero* (buffers, essentially never work) and *unknown nonzero* (the
actual queue). Snake Byte: 4,244 code, 12,883 declared data, 2,389 unknown zero,
and **451 unknown nonzero** — against the 2,657 previously believed.

**The check that keeps it honest:** a declared range the disassembler also
reached as code is flagged, since one of the two claims must be wrong.
Declarations are conclusions about the program and will drift as understanding
changes; without that check the report would quietly report whatever it was
told. The test asserts the warning fires.

**Method note, generally:** a measurement that bounds the work should be
regenerated by the build, and the categories should match the *decisions* they
inform. "Not code" answers nothing. "Unknown nonzero" is a work queue.

---

## 2026-08-08 — The interpreter and the generated code, compared at last

**Scope:** apple2tc · **Status:** validated (the measurement; the divergence is open)

**Decision:** `a2run --preload` mirrors how a decompiled build starts, and
`engine6502` carries its cycle debt across calls. Together these make the two
engines comparable, which they were not.

**Why `--preload`.** A decompiled build has the program baked into
`init_emulated()`, so from cycle zero it is simply in RAM and the ROM boots
around it; `play.keys` then types `CALL 14160` at the BASIC prompt like a
person. The interpreter was instead loading the binary at a warm-restart
breakpoint and injecting its own `CALL` — so it both started from a different
memory image and typed the command twice. The memory image matters
concretely: the binary occupies `$3750-$854E`, and **2,224 bytes of that fall
inside hi-res page 1**, which the frame hash covers. The two could not agree on
frame 0, let alone later.

**Why the cycle debt.** `Emu6502::runFor()` re-baselines on entry —
`for (unsigned startCycles = cycles_; cycles_ - startCycles < runCycles; ...)`
— so it overshoots by the last instruction and forgets. The generated engine
carries the deficit instead (`s_remaining_cycles += run_cycles`). Left alone
that is a steady drift; carried, the two stay within a bounded distance. Done in
the engine adapter rather than in `Emu6502`, so `a2emu` and `textemu` pacing is
untouched.

**What the comparison shows.** 1,300 frames of `play.keys`:

- Cycle delta stays in **[-64, +2]** across 8 million cycles. The two cycle
  models effectively agree; what remains is granularity, not arithmetic. -64 is
  about one basic block, which is exactly the generated engine's yield
  granularity — it can only stop at a block boundary, where the interpreter
  stops between instructions.
- **Frames 0-7 match exactly.** Thereafter the engines agree in long runs — 23,
  20, 24, 25, and twice 100 consecutive frames — and disagree in bursts of 1-3
  frames around screen transitions, then *re-converge*. That pattern is the
  sampling artefact and is benign: the two are a few cycles apart mid-update.
- **At frame 472 they diverge permanently**, 828 frames to the end. Neither
  hangs: 106 and 149 distinct hashes follow. They are in different game states.

**What is established, and what is not.** Established: the engines agree on
cycle cost to within a block over millions of cycles, and agree on video state
whenever the machine settles. That is a real cross-check and it mostly passes.
Not established: what happens at frame 472. It is reproducible — two `a2run`
runs are byte-identical — so it is a genuine difference between the two
implementations, not noise. It is *not* a gross semantic difference, or it would
have shown far earlier than 8 million cycles in.

**Deliberately not chased here.** The design scheduled this stage as discovery
rather than a gate, precisely so the refactor's value would already be banked
before anyone went looking for a bug in one of two 6502 implementations. It is
the obvious next investigation, and it now has a reproducible 1,300-frame
handle and a first divergent frame to bisect from.

## 2026-08-11 — The host and the engine, split; and a correction to why

**Scope:** apple2tc · **Status:** landed (stages 0-6); the justification below is
partly retracted

**Decision:** `system.h` was already a contract between the Apple II machine and
whatever executes 6502. It is now two: `a2engine.h` (what an engine provides,
the host calls) and `a2host.h` (what the host provides, an engine calls), with
`system.h` an umbrella over both. The host library became sokol-free, and
`a2run` — a console front end — joined `a2emu`, which was rebuilt on the same
pieces.

**Why two executables rather than one with a flag.** On Windows, console versus
GUI is a link-time subsystem property; no runtime flag bridges it. So `a2run`
and `a2emu` are separate binaries over one host, and every decompiled game gains
a `<name>-run` alongside `<name>`. This is recorded because it looks like
over-engineering until you know the reason.

**What it bought, measured:**

- **Run data can be regenerated headlessly.** This was impossible before:
  `snake-byte.json` is a 2022 artefact of a windowed `a2emu`, and nothing in the
  tree reproduced it. `decoded/snake-byte/README.md` has the recipe, and is
  explicit that it does not reproduce that file byte for byte.
- **The verification binaries link no graphics at all.** `verify.sh` drives
  `snake-bytec1-run` and `snake-bytec1-ext-run`, and no longer depends on a
  display stack existing or behaving.
- **One host instead of two.** `a2emu` went from 748 lines to 131 and gained its
  first test of any kind: sharing everything but the window, `a2emu --headless`
  must byte-match `a2run`. That test is in `tests/run-tests.sh`.

**The third justification was overstated, and this is the retraction.** The
design argued the split would give "an independent oracle" — the interpreter
checking the generated C. Three things came out of trying it:

1. **Cycle accuracy is a property to preserve *within* an implementation, not to
   match *between* two.** Frame hashing asks "is the screen identical at
   host-observation moment N", which presumes a shared sampling clock. The
   generated engine yields only at block boundaries, an interpreter between
   instructions, so no such clock exists — and *two perfectly equivalent
   implementations still fail that test*.
2. **Registers cannot be compared.** `CPURegLiveness` and `dce` deliberately
   drop stores to dead registers, so the generated code does not maintain `Y` or
   the flags where nothing reads them. Traces diverge on line 2.
3. **PC-only control flow, with no input, does work** — 1,991 of 1,991 branch
   targets over 120 frames of ROM boot, no drift. Reaching *game* code needs
   both engines to receive identical input, which they do not: key delivery is
   `get_cycles() >= stamp` at frame boundaries, and 21 of 23 keys land a frame
   apart.

**`--snapshot-at` does not exist.** Stage 7 of the split design was never
started, so there is no snapshot option and no format for one. Anything that was
waiting on it still is.

**Open:** the frame-472 divergence from the 2026-08-08 entry is unchanged and
unattributed. It is reproducible, so it is a genuine difference between two 6502
implementations rather than noise.

## 2026-08-12 — Probes: let the program supply the coordinate

**Scope:** apple2tc · **Status:** compiler landed and tested; nothing executes yet

**Decision:** compare the two implementations at points *the program* defines,
not at moments the host defines. A **probe** is a small program bound to one or
more install sites; both engines reach a site in the same order the same number
of times, because that is what equivalence means. Design:
`docs/plans/2026-08-11-probes-design.md`. Language reference: `docs/probes.md`.

**Why, given the entry above.** Every observable tried had failed the same way —
registers, cycle counts, frame hashes, key-driven PC traces — and the common
cause was sampling on a clock the two programs do not share. Frame boundaries,
cycle counts and wall time are all coordinates external to the program. A probe
has no such coordinate to disagree about.

**Three questions, not one.** The retraction above collapsed because "is it
correct" was being asked of three different subjects at once:

| | question | subject | instrument |
|---|---|---|---|
| 1 | is apple2tc correct? | the **tool** | control flow, cross-engine, timing-free |
| 2 | did a refactor break the game? | the **hand-written C** | frame hash + cycles, same engine |
| 3 | does the hand-written C match the original? | the **hand-written C** | same, against a recorded trace |

Only question 1 can use registers or block traces — a decompiled game is *meant*
to end with neither. Question 3 has no address correspondence at all between the
two programs, so nothing automatic can compare them; a probe is a **manually
declared correspondence**, which is exactly and only what it needs.

**Design points worth not rediscovering:**

- **A probe's parameter list is its signature** — name, arity, order — and that
  is what both implementations must satisfy. The initializers are merely a
  default way to obtain each value, correct in the emulator; in generated C an
  individual one is overridden where a register is dead or the value lives in a
  C variable. That shrinks the divergence surface from "a body per side" to "one
  expression, only where the default fails".
- **One program, two entry offsets.** Initializers, or straight into the body
  with values supplied by a caller. So the body is byte-identical bytecode on
  both sides and counters, gating, formatting and key-fetch cannot diverge.
- **`CYCLES` is emitted per block, not per instruction.** The interpreter can
  probe any address; a generated program can only probe block heads. A probe on
  a non-block-head fires under `a2emu` and does not exist in the generated C —
  **and the report still reads as agreement.** This is the design's main hazard.
- **Hence `@"file"` installs.** With apple2tc emitting the block-head list, both
  sides install at an identical finite set, the reports have equal length, and
  `diff` compares them directly. That retires the subsequence matcher, which
  during the 2026-08-08 investigation drifted and reported a meaningless "50%
  match" — a tool that degrades gracefully on divergent input manufactures
  agreement.

**What landed:** the compiler only — lexer, precedence expression parser,
statements, install sites, an address→probe hash, a disassembler behind
`--probe-dump`, 63 rejection assertions and 4 baseline diffs. **No VM**;
`probe_dispatch` is a stub with no caller and nothing is wired into `CYCLES`.

**Deliberately not chased here.** Key fetch in counter coordinates, and the
frame-472 attribution it unblocks, both wait on the VM. So does the acceptance
test — replicating the trusted 1,991/1,991 ROM boot through probes, which
validates the mechanism against a result already believed rather than merely
exercising it.

## 2026-08-12 — The probe acceptance test: coverage agrees, dwell time does not

**Scope:** apple2tc · **Status:** validated (the mechanism); a real divergence,
diagnosed and left unfixed

**Decision:** `decoded/rom/probe-acceptance.sh` installs `trace.probe` — one
`printf("%04X\n", pc)` probe — at every address in `decoded/rom/blocks.txt`
(1,718 addresses, `grep`'d live from `romc1.c`'s `CYCLES(` calls) on both
`a2run` and `romc1-run`, over 120 frames of ROM boot with no key pressed, and
diffs the two reports. **It fails, on purpose left failing**: the task that
specified this test was explicit that a real divergence here is a more
valuable result than a pass, and this is one.

**The result.** Over 120 frames: `a2run` produces 336,825 probe hits;
`romc1-run` produces 256,616. `diff` reports exactly one hunk —
`256617,336825d256616` — meaning `romc1-run`'s report is a **byte-exact
prefix** of `a2run`'s, not a scatter of mismatches. Both engines' `stderr`
report no probe as never-fired, so this is not the install-site hazard the
whole design exists to catch.

**What was ruled out, each with its own measurement:**

- **Different install sites.** Both binaries installed at the same 1,718
  addresses; neither ever reported a probe that didn't fire.
- **A different boot path.** `--hash-frames` on both engines shows the video
  state hash matches at all 120 frame boundaries (identical mode, page and
  content), and the *set* of distinct addresses either engine actually
  dispatched through is identical — 121 of 121 — in identical first-occurrence
  order (`awk '!seen[$0]++'` on each report, then `diff`, empty). That set is
  this test's direct analogue of the 2026-08-08 entry's "1,991 of 1,991 branch
  targets match": at the level of *which places the program goes*, the two
  engines still agree completely. The divergence is not in control flow.
- **Blocks apple2tc dropped as unreachable.** The 24,366 probe hits that occur
  before the machine settles into the loop below match exactly between the two
  reports, hit for hit. Nothing decompiled-but-unreached is hiding here.
- **Total elapsed cycles.** `get_cycles()` at frame 119 is 2,028,951
  (`a2run`) vs. 2,028,954 (`romc1-run`) — 3 cycles apart out of ~2.03M. The two
  engines were given, and consumed, essentially the same amount of emulated
  6502 time.

**What actually diverges: dwell time in one idle loop.** Every extra line past
byte 256,616 is `FD1B`/`FD21` alternating — the ROM's keyboard-wait spin at
`$FD1B-$FD26` (`INC $4E` / `BNE $FD21` / `BIT $C000` / `BPL $FD1B`), which the
machine parks in for the rest of the run once no key arrives. `a2run` executes
it 155,925 times before frame 120; `romc1-run` executes it only 115,899 times
— for the *same* total elapsed cycles. Root cause, found by reading rather than
guessing: **neither engine's cycle charge is real 6502 timing, and the two
approximations disagree on this specific instruction mix.**
`Emu6502::runFor` (`lib/cpuemu/emu6502.cpp:112`) charges a flat `cycles_ += 3`
per instruction executed, regardless of opcode — so one loop pass (`INC`,
`BNE` taken, `BIT`, `BPL` taken: 4 instructions) costs 12. The generated
engine's `CYCLES()` argument is computed once per basic block at decompile
time as `lround(block.size() * 1.7 + 0.5)` bytes-to-cycles
(`tools/apple2tc/GenIR.cpp:77`, mirrored in `PrintSimpleC.cpp:103`) — so the
same loop pass, split into the `$FD1B` (4 bytes → 7) and `$FD21` (5 bytes → 9)
blocks, costs 16. 16/12 ≈ 1.33, and the measured hit ratio is 336,825/256,616
≈ 1.313 — the same effect, attenuated only by the shared, matching prefix
before the loop. Both figures were already on record as approximations (the
2026-08-08 and 2026-08-11 entries above call the two cycle models
"effectively agree[ing]" only in aggregate, "granularity, not arithmetic");
this test is the first place a 100,000-iteration loop turned that granularity
into a visible, six-figure divergence instead of a bounded few-cycle one.

**Does not stabilize.** `--frames=60`/`120`/`240` give hit-count ratios
1.292 / 1.313 / 1.323 and absolute gaps 37,543 / 80,209 / 165,543 — growing,
not converging, roughly doubling as frames double. There is no frame count
that makes this pass; every count past the point the ROM parks in the loop
(around frame 7-8) only widens the gap. 120 was not a bad choice — the
divergence is already fully present by then — it is simply not a number this
mechanism can be tuned around.

**Answering the plan's open questions:**

- *Is `--frames=120` right, and does the report stabilize?* It does not
  stabilize, per above, and no frame count would make it. 120 is fine as a
  boot-length sample; it is not a knob that fixes this.
- *Should `blocks.txt` be committed?* Yes — committed alongside the script.
  It is regenerated by the script on every run (so it can never go stale
  against the `romc1.c` under test), but a committed copy is what lets a
  future failure be diffed against today's site list instead of merely
  today's report.
- *Should the two sides produce the same hit count at all, given apple2tc may
  eliminate code the interpreter still executes as unreachable?* Not
  necessarily in general, but it was not the cause here: the pre-loop hit
  counts match exactly (24,366 = 24,366), and the reachable-block-set matches
  (121/121). This run's divergence is entirely dwell time in one loop, not
  missing coverage.
- *Does `romc1-run` boot from the same initial state as `a2run`?* Yes,
  confirmed beyond the one `hello.probe` data point: video-state hashes match
  at all 120 frame boundaries, and first-occurrence order over the full
  address set matches too.

**What this validates, and what it doesn't.** The probe mechanism itself
checks out: dispatch fires from both engines, `@"file"` installs land on an
identical, complete site set, and — at the coverage granularity the
2026-08-08 comparison used — the trusted result reproduces exactly (121/121,
same order). What the mechanism additionally exposes, which the coarser
comparison could not, is that the two engines' *notions of elapsed time*
diverge on a narrow, highly-repetitive instruction mix even while agreeing on
totals and on every other measure tried. That is a genuine property of
`Emu6502`'s flat per-instruction charge and the generated engine's
per-block byte heuristic, not a defect in probes or in `apple2tc`'s
control-flow recovery — and not touched here, since fixing either cycle model
was out of scope for this task.

## 2026-08-13 — Real cycle costs: the acceptance test passes, the seed agrees, frame 472 moves

**Scope:** apple2tc · **Status:** the payoff task — measured, not engineered to pass

**Decision:** with the shared per-opcode cycle table landed in both engines
(Tasks 1-5 of `docs/plans/2026-08-12-accurate-cycles-plan.md`), re-run the
probe acceptance test, the `$4E` RNG-seed check, and the frame-472
reconstruction that the 2026-08-12 entry left open. All three are reported
below exactly as measured, including the one that did not fully resolve.

### `romc1.c` is still blocked

> **Superseded the same day — see "The `romc1.c` blocker was ours" below.** The
> "pre-existing and out of scope" call in this section is wrong: the crash was
> introduced in `ea29cdc`, four days before this entry. It is fixed and
> `romc1.c` is regenerated.

`decoded/rom/decompile.sh`'s `--irc1` line still crashes apple2tc
(`CPURegLiveness.h:89: Assertion 'it != funcData_.end()' failed`), reproduced
again today, byte-identical to the 2026-08-12 report. It is pre-existing and
out of scope here, so `romc1.c` remains uncommitted-stale and the acceptance
test was switched to the other working back end instead, per the plan's
documented workaround.

`decoded/rom/rom.c` (the `--simple-c` back end, previously stale since a 2022
regeneration) was regenerated and committed; its line in `decompile.sh` was
uncommented. `decoded/rom/probe-acceptance.sh` now greps `blocks.txt` from
`rom.c` and runs `a2run` against `rom-run` instead of `romc1-run`.

### 1. The acceptance test

`./decoded/rom/probe-acceptance.sh cmake-build-debug`, 120 frames of ROM boot,
1,718 block-head sites:

```
PASS: 276255 probe hits match over 120 frames
```

Exit 0. Both sides' stderr show only the pre-existing "Unsupported IO location
read $CFFF/$C107.../$C707" warnings (unrelated peripheral-detection reads);
neither reports a probe that never fired, so this is not the coarser-report
hazard the design exists to catch.

Beyond the script itself, `--hash-frames` on both `a2run` and `rom-run` over
the same 120 frames shows video-state hashes matching at **all 120 frame
boundaries** (0 of 120 differ), and elapsed cycles are **exactly equal** at
frame 119 — 2,028,950 on both sides, not merely close. Cycles differ by up to
15 during frames 1-14 (the two engines yield at different granularities while
memory is still initializing) and are bit-for-bit equal from frame ~15 through
119. This is a materially tighter result than the 2026-08-12 entry's "3
cycles apart out of ~2.03M" — here, over the same stretch, the gap closes to
zero and stays there.

### 2. The `$4E` seed

A scratch probe (`probe seed(site = pc, lo = peek8($4E), hi = peek8($4F))`,
installed at the same `blocks.txt` site list as the acceptance test, not
committed) sampled the seed at every one of the 276,255 block-head hits over
the same 120-frame boot, on both `a2run` and `rom-run`.

**The two outputs are byte-for-byte identical** — not just at the end, at
every single one of the 276,255 sampled instants. Final sample: PC=`$FD1B`,
`$4E`=`$01`, `$4F`=`$EB`, identical on both sides.

This is the strong form of the hypothesis in the 2026-08-12 design doc: the
old flat-cost-vs-bytes-heuristic mismatch was not a measurement artefact, it
was really incrementing the seed a different number of times on each side
(155,925 vs 115,899 over the same budget), and with a shared cycle table that
is gone — both engines now walk through RNG state in lockstep.

### 3. Frame 472

**Method, reconstructed from the 2026-08-08 entry.** That entry names its
recipe precisely enough to reproduce: `a2run --preload` (so the interpreter
starts from cycle zero with the program already in RAM, exactly as a
decompiled build's `init_emulated()` does, and the same `play.keys` key file
types `CALL 14160` for both sides), the cycle-debt-carrying adapter (landed
as default behaviour, not a flag, so no extra step is needed), 1,300 frames,
compared against "a generated build." The entry does not name the binary; it
is identified here as `snake-bytec1-run`, `decompile.sh`'s own "reference
build" — the only generated snake-byte binary that boots the ROM itself from
cycle zero, which `--preload` requires as a counterpart. That identification
is an inference, not something the log records, and is the one part of the
method that could not be pinned down exactly.

```
a2run --preload decoded/snake-byte/snake-byte.b33 \
      --key-file=decoded/snake-byte/play.keys --frames=1300 --headless \
      --hash-frames=interp.frames
decoded/snake-byte/snake-bytec1-run \
      --key-file=decoded/snake-byte/play.keys --frames=1300 --headless \
      --hash-frames=gen.frames
```

Both runs were confirmed deterministic (two runs of each binary byte-identical
in their `--hash-frames` output) before comparing them to each other, as the
2026-08-08 entry did.

**Corroboration that this is the same kind of measurement.** The qualitative
shape matches the entry's description closely: long exact-agreement runs (up
to 97 and 85 frames here, the same order as the entry's "23, 20, 24, 25, twice
100") punctuated by 1-3-frame disagreement bursts around screen transitions
that then re-converge, and a bounded residual cycle gap of the same order as
before (see the push-back answer below). That the shape survived a real
change to both cycle models is itself evidence the method was reproduced
correctly, not merely that a number was found that happened to match.

**Frame 472 specifically:** it is now a single-frame burst, not a permanent
split. Frames 467-471 match; frame 472 is one frame off (the interpreter
reaches a screen-transition hash one frame before the generated build does);
frame 473 re-converges and both sides stay in lockstep through frame 622. That
is structurally identical to every other transient burst in this run (at
frames 55, 71, 169-178, 264, 271-272, ..., 616) — nothing distinguishes 472
from them anymore.

**But a new permanent split appears at frame 623**, and holds through the end
of the recorded 1,300 frames (677 of 1,300 frames, 72 distinct hashes on the
interpreter side thereafter, 74 on the generated side — the same
neither-side-hangs shape as the old entry's "106 and 149 distinct hashes,"
just at a different address and a different frame). Frame 623 is roughly
10.6M cycles in, about 2.6M cycles (151 frames) further into the run than
frame 472 was.

**So: it moved. It did not disappear.** The specific 8-million-cycle-in split
reported on 2026-08-08 is gone — real, permanent 151 frames (2.6M cycles) of
extra correct agreement were gained by fixing the cycle model, exactly as the
RNG-seed hypothesis predicted a shared table would buy. But a new, equally
permanent split exists a little further on, at frame 623. This does **not**
mean "a two-year-old placeholder in the cycle accounting produced the one
unexplained result in this project" — that would require the divergence to
disappear outright, and it has not. What it does mean: the cycle-accounting
fix was real and moved the goalposts substantially, and whatever causes a
permanent split between these two implementations is not fully explained by
cycle accounting alone. Root-causing frame 623 is future work, out of scope
for this measurement task.

### Push-back

**Is `rom.c` an equivalent substitute for `romc1.c`?** Same in substance,
narrower in coverage — not a free swap, but a legitimate one for what this
test checks. Concretely:

- Both back ends compute a block's base cost through the same shared helper
  (`AsmBlock::baseCycles`, called from `GenIR.cpp:75` and
  `PrintSimpleC.cpp:101`) and both add the taken-branch `+1` exactly on the
  taken edge — `PrintSimpleC` folds it into the branch ternary's comma
  operator (`s_status & STATUS_Z ? (s_cycles++, s_remaining_cycles--, 0x032f)
  : ...`, visible throughout the `rom.c` diff), `GenIR` instead emits a
  one-instruction trampoline block. So the thing actually under test here —
  the shared `cpuInstCycles()` table plus the taken-branch-on-edge convention
  — is identical between them; only how the `+1` is textually attached
  differs.
- The distinct block-head address **sets** are identical: the (still-stale)
  committed `romc1.c` has 1,977 `CYCLES(` call sites at only 1,718 distinct
  addresses (259 addresses appear twice, apparently from `-O3` code
  duplication specific to the `--irc1` pipeline), `rom.c` has exactly 1,718
  call sites at 1,718 distinct addresses, and the two address sets are the
  same 1,718 (`comm -12`, verified). So switching the site-list source from
  `romc1.c` to `rom.c` did not shrink or shift `blocks.txt` at all — same
  install points, same test.
- What differs structurally: `system-inc.h` (`rom.c`) keeps CPU status as one
  packed byte; `system2-inc.h` (`romc1.c`) splits it into seven per-flag
  booleans specifically so `CPURegLiveness` can DCE dead flag stores — the
  very optimization pass that crashes on this input. Routing around the crash
  by using `rom.c` therefore also routes around ever exercising that pass, or
  `GenIR`'s trampoline-block mechanism (Task 3's "part most likely to be
  subtly wrong"), against the interpreter. Those remain checked only by the
  four `.ir` baselines with trampoline blocks — a same-tool unit test, never
  cross-engine.

So: this test now validates the shared cycle table and the taken-branch
convention, over real ROM code including the keyboard-wait loop's branches,
end to end, cross-engine — which is what it is for. It does not, and cannot
yet, cross-check the `--irc1`-specific code shape against the interpreter,
because that back end cannot build this program at all.

**Is the 2026-08-08 comparison reconstructible?** Close to exactly, with one
named inference (which binary is "a generated build" — see above). Everything
else in the entry's own description of its method — `--preload`, the cycle-debt
adapter, `play.keys`, 1,300 frames — is specific enough to reproduce
mechanically, and the reproduction was cross-checked, not just trusted: two
independent determinism checks, and a qualitative match (burst-then-reconverge
shape, bounded residual gap) against a result produced under a completely
different cycle model.

**Equal-budget runs — is the residual gone?** No, and it was never expected
to close: `Emu6502::runFor` can stop between any two instructions;
`s_remaining_cycles` in a generated program can only go negative at a block
boundary and gets carried to the next call, so the generated engine is always
up to one block's worth of cycles ahead or behind at any given sampling
instant. Measured on the 1,300-frame `play.keys` run: cycle delta ranges from
**-61 to +4** across 22.1M cycles (interpreter 22,147,951, generated
22,147,957 at the final frame) — the same order of magnitude as the
2026-08-08 entry's pre-fix "[-64, +2]... granularity, not arithmetic."
The shared table fixed *what* each instruction costs; it was never going to
fix *when* either engine is allowed to stop counting, which is a scheduling
property, not a cycle-cost one. The ROM-boot-only comparison above (no game,
no key input) shows the same gap closing to exactly zero by frame ~15 and
staying there — the residual is real but small and bounded, exactly as
designed.

### What this does and does not settle

**Settled:** the shared cycle table works as intended. The interpreter and a
generated build agree, hit for hit, on 276,255 samples of a real ROM boot,
including its most repetition-sensitive loop; the RNG seed the two engines
previously diverged on now walks in lockstep at every sampled instant; and a
genuine 151-frame, 2.6M-cycle stretch of previously-wrong disagreement (frames
472-622) is now correct agreement. `romc1.c` remains unregenerable pending a
fix to the pre-existing `CPURegLiveness` crash, tracked separately.

**Not settled:** a permanent divergence between the interpreter and the
generated `snake-byte` build still exists, now at frame 623 instead of 472.
It is reproducible (both sides deterministic) and therefore real, not noise.
Whether it has the same root cause as the old frame-472 split, moved by the
more accurate accounting, or a different cause entirely, is unknown and
unexamined here — this task was scoped to measure, not to chase it.


## 2026-08-13 — The `romc1.c` blocker was ours: a cyclic call graph in liveness

**Scope:** apple2tc · **Status:** fixed, with a correction to two earlier entries

**Decision:** stop treating the `CPURegLiveness` crash as pre-existing
background and find out when it actually started. It was introduced by our own
`ea29cdc`, four days earlier.

### The correction first

Both the 2026-08-12 and the 2026-08-13 entries above call this crash
"pre-existing" and scope it out. That was wrong, and the way it went wrong is
worth recording: the claim was checked by reproducing the crash at the fork
point of the session doing the work, which answers *"did today's changes cause
it?"* — not *"is it ours?"*. The bracket needed for the second question was in
the tree the whole time: `romc1.c` was committed at `d84b0e6`, so some earlier
tool could produce it. Building that commit (it configures fine; it needs four
`-include` flags to compile against a current libstdc++) shows the ROM
decompiling cleanly at `-O2`, `-O3`, `--ir` and `--irc1`.

`git bisect` over the 86 commits between then and now, driven by hand because
the failure has two shapes, lands on:

```
ea29cdc apple2tc: track 6502 stack depth per routine, not per basic block
```

Its parent `83e6584` is clean; `ea29cdc` fails. It changed one non-test file,
`routines.cpp`, +67/-20.

### Root cause

`ea29cdc` made `identifySimpleRoutines` accept the PHA-at-entry/PLA-before-RTS
idiom it had been rejecting, which is correct — but it also meant routines that
had never been extracted before now were, and some of them call each other in
cycles. The ROM's HGR line-drawing code is a tangle of shared entry points:

```
MVLFTRGT ($F465) -> MVUPDWN0 ($F4D3) -> HGLIN ($F53A) -> MVLFTRGT
```

`CPURegLiveness::calcGenKillMod()` walked functions in `calleeFirstOrder()` — a
reverse post order of the inverse call graph, which orders callees before
callers **only when that graph is acyclic** — and relied on the ordering twice:

1. It created each function's `FuncData` as it went, so the edge closing a cycle
   looked up a callee that had no entry yet and tripped the assert.
2. It accumulated each function's `modified` set in the same pass, where a
   not-yet-computed callee contributes an **empty kill set**.

The second was the more dangerous one and was found only while fixing the
first. `calcLiveness()` seeds `liveOut` from `modified` and thereafter only
*intersects* it, so a `modified` that starts too small silently licenses `dce()`
to delete stores that are still live. It produces no crash and no warning.

### Fix

Seed every `FuncData` before any `initBB()` runs, so the lookup no longer
depends on order; the existing fixpoint loop already handled convergence. Then
recompute `modified` after that fixpoint, when the kill sets are final.

`tests/callcycle.s` covers both halves. `funcb` writes `X` and `Y`, which
`funca` never touches, so computing `modified` too early drops the entry
function's `X` and `Y` stores — which the baseline pins. Mutation-tested: the
pre-fix binary aborts on it, and a seeding-only build produces output that
differs from the baseline.

### What regenerating showed

`romc1.c` is regenerated and `decompile.sh`'s `--irc1` line is uncommented.
Everything else is **byte-identical** — `robotronc1.c`, `snake-bytec1.c`,
`snake-bytec1-ext.c`, `coverage.txt`, `rom.c`, `bolo.c` all regenerate
unchanged, which is the expected shape: the fix only changes inputs whose call
graph has a cycle, and only the ROM's does.

The regenerated `romc1` is checked against the `--simple-c` build over 120
frames of ROM boot: **all 120 screen hashes identical**, maximum cycle
divergence 1. That residual is the known block-boundary granularity difference —
`--irc1` runs `simplifyCFG` and so yields at coarser blocks — not a behavioural
one. `probe-acceptance.sh` still passes (276,255 probe hits), `verify.sh` is
4/4, and the decompiler suite is green.

### Left open, deliberately

- The acceptance test still probes `rom-run`. Probing `romc1-run` needs its own
  site list grepped from `romc1.c` because `--irc1` blocks are coarser. It is a
  genuine extra check — `romc1` is the only build exercising the
  `system2-inc.h` trampoline engine — and is future work, not folded in here.

  > **Done the next day — see "The acceptance test now covers `--irc1`" below.**
  > "Because `--irc1` blocks are coarser" was wrong, incidentally: the two back
  > ends emit block heads at exactly the same 1,718 addresses, which the
  > 2026-08-12 entry above had already measured.
- At `ea29cdc` itself the ROM **hangs** in `splitRoutines()` rather than
  asserting. Later commits reworked that code and the hang is not reachable at
  HEAD, but it was never root-caused, and "fixed" would overstate it.


## 2026-08-14 — The acceptance test now covers `--irc1`, and found a probe firing on an edge

**Scope:** apple2tc · **Status:** fixed

**Decision:** close the last gap in Question 1's evidence by pointing the probe
acceptance test at `romc1-run` as well as `rom-run`. Doing so exposed a real
defect: the taken-branch trampoline was dispatching probes.

### What the gap was

`romc1` is the only build compiled against `system2-inc.h`, and `--irc1` is the
only back end that emits the taken-branch `+1` as a real trampoline block
rather than folding it into a dispatch ternary. Testing `rom.c` alone left both
— the per-flag liveness DCE and `GenIR`'s trampoline mechanism — with no
cross-engine coverage at all, only same-tool `.ir` baselines.

### The defect

Running `romc1-run` against the existing site list gave 276,256 hits against
the interpreter's 276,255. One extra line, `FC56`.

`$FC56` appears twice in `romc1.c`: once as a real block (`CYCLES(0xfc56, 2)`)
and once as the trampoline charging the taken-branch penalty
(`CYCLES(0xfc56, 1)`). Both spellings end in `probe_dispatch(pc)`, so a single
execution of that branch was observed twice. The interpreter dispatches once
per instruction and reports it once.

This is latent at every branch that is also a branch target: **698 distinct
trampoline addresses in the ROM, 121 of which are also genuine block heads**.
Boot happens to reach one of the 121.

The same bug was in the tracer, for the same reason and at the same sites.

### The fix: an edge is not a location

The trampoline block carries the branch's address but is not a place the
program is ever *at* — the branch was already accounted for by the block that
ends with it. That is a property of the IR, so the IR now says it:
`AddEdgeCycles` alongside `AddCycles` (`ir/Values.def`), created by
`GenIR::emitJCond` for both taken-edge charges, printed by `IRC1` as
`CYCLES_EDGE`, which charges cycles and runs neither `debug_asm` nor
`probe_dispatch`.

A structural rule would have been wrong. `tests/func.ir` has a case where
`simplifyCFG` merged the trampoline into its target, leaving `AddEdgeCycles` at
the head of a block and that block's own `AddCycles` second — so "the first
`AddCycles` in a block is the block head" does not hold, in either direction.

`CYCLES_EDGE` is defined in both system headers, because `--irc1` output is
compiled against either (`robotronc1.c` uses `system-inc.h`, `romc1.c` uses
`system2-inc.h`). The `system2-inc.h` version keeps `s_pc` and the
`cycles_expired()` yield: dropping the yield would move where the host regains
control, and this macro is meant to remove the two observation facilities and
nothing else.

Hand-written decoded C had the same bug — 35 trampolines in
`decoded/snake-byte/a2rom.c` and 2 in `game.c`, all converted.

### Measurements

- **`romc1-run` now matches the interpreter exactly**: 276,255 probe hits,
  byte-identical, over 120 frames. `rom-run` is unchanged at the same number.
- The regenerated `c1` diffs are **only** `CYCLES` → `CYCLES_EDGE` on cost-1
  charges: 4,023 lines changed across four files, exactly the trampoline counts
  (895 + 1,438 + 856 + 834). No other generated output moved.
- The `.ir` baseline diff is 11 lines, every one of them an edge charge.
- Gates: decompiler suite green · `verify.sh` 4/4 · both back ends pass
  acceptance · full build clean. Cycle totals are unchanged, which is why
  `verify.sh`'s frame hashes did not move.

**The two back ends emit block heads at exactly the same 1,718 addresses** —
`grep`'d independently from `rom.c` and `romc1.c`. The "`--irc1` blocks are
coarser" claim in the entry above is wrong. The test still derives and keeps a
list per back end rather than sharing one, so a future `simplifyCFG` change
shows up as a different site count instead of a spurious failure.

### Mutation tests

- Reverting `CYCLES_EDGE` → `CYCLES` in `romc1.c` and rebuilding: the test
  fails, exit 1. (It fails partly through the contaminated site list — 2,295
  addresses instead of 1,718 — since the two spellings are what separates edges
  from block heads. The pure double-fire is the 276,256-vs-276,255 measurement
  above, taken before the fix.)
- Perturbing `tracec1.probe`'s body: the drift check fails. The two probe
  scripts must stay the same program apart from their site list, or each half
  stays individually valid while quietly testing something different.

### Left open

- **Frame 623**, the permanent interpreter-vs-generated snake-byte divergence,
  is still untouched and still unexplained.
- Nothing now guards against a *new* back end reintroducing the same class of
  bug by emitting an observation for an edge. The IR says which is which; only
  this test would catch a back end ignoring it.


## 2026-08-14 — Probe-stamped key recording: `record`, and what frame 623 was

**Scope:** apple2tc (`lib/a2host`), snake-byte · **Status:** built
(`docs/plans/2026-08-14-probe-stamped-keys-plan.md`, all 7 tasks)

**Decision:** replay recorded keyboard input on a coordinate the *program*
defines instead of one the host's cycle counter defines, so the interpreter
and a generated build receive identical input at identical program points.
Add `record <expr>`, the recording counterpart of `key <expr>`; convert
`play.keys` to a probe-stamped `play.pkeys`; use the result as the oracle
Question 1 (frame 623) needed.

### Why cycle stamps could not work

`drain_key_presses()` compares each key's stamp against `get_cycles()` —
*each engine's own counter*. The 2026-08-13 "Real cycle costs" entry above
already measured how far those counters drift apart on this exact 1,300-frame
`play.keys` run: **-61 to +4 cycles** over 22.1M cycles, a scheduling artifact
(the generated engine can only go negative on `s_remaining_cycles` at a block
boundary; the interpreter can stop between any two instructions), not
something a more accurate cycle table could ever close.

Snake Byte polls the keyboard inside a spin loop at `$741C`, so the iteration
at which a key lands is a function of that drifting counter, and the two
engines can land on different iterations. Measured directly: with `play.keys`
truncated to its first 16 keys, the two engines agree on all 1,300 frames;
adding key 17 splits them permanently from frame 623 onward. Key 17's stamp is
`10,622,152`, which is *exactly* the generated engine's frame-623 boundary —
unsurprising once stated plainly: `play.keys` was recorded from the generated
build, so every stamp is that engine's own coordinate, replayed against a
different one. Frame 623 was never a decompiler bug; it was the recording
disagreeing with itself about whose clock it was reading.

### What the coordinate change buys

`record <expr>` compiles to `OP_RECORD` (`123ac63`), the mirror image of
`key <expr>`'s `OP_KEY`. With `--record-keys=<path>` given (`d8c0c8b`), the
host stops handing incoming keys straight to the machine and instead holds
them in a small pending queue; `record`'s call site is what releases them —
into the machine and, stamped with `<expr>`'s value, into `<path>`. That
split is deliberate: if the host pushed keys the moment they arrived and
`record` only wrote the stamp, the recorded session would have used
unquantised timing while its replay uses quantised, and the two would not be
the same run.

The host's per-frame cycle drain (`drain_key_presses()`) would otherwise race
this: counter stamps are small integers, so against `get_cycles()` every one
is already "due," and the drain would deliver the whole key file during the
first simulated frame, leaving the script's `key` nothing to do. `f1eb6e2`
adds `probe_uses_key()` — true when the loaded script contains at least one
`OP_KEY` — and the drain stands down whenever it does.

Two review-round fixes were needed before recording actually worked
(`4f843b1`): `push_key()`'s diversion into the pending queue meant `io_`'s own
hardware queue never filled, so the code that decided how far ahead to read
from `--kbd-file=` kept using the hardware queue's occupancy and read straight
past the pending queue's capacity; and `drain_key_presses()` /
`probe_deliver_keys()` were calling `a2_io_push_key()` directly, bypassing the
one place the recording diversion happens, so `--key-file=` combined with
`--record-keys=` — the exact operation `play.keys` → `play.pkeys` depends on
— recorded zero keys every time.

The payoff: once input rides a coordinate both engines compute identically,
any surviving cross-engine divergence is a real difference between the two
engines, not a sampling artifact of when each one happened to notice a key.
That is what neither frame hashes nor cycle stamps could give this project
before now.

### The conversion method

`decoded/snake-byte/rec.probe` and `play.probe` (`c5cf552`) share one counter,
incremented and either `record`ed or `key`ed at every site that reads the
keyboard. The plan that specified this task named three sites — `$FD1B` (the
ROM's KEYIN wait loop, BASIC-prompt keys), `$741F` and `$7890` (the game's own
setup-screen polls). **That list was wrong, and the gap was found only by
running the recording, not by reading the disassembly harder:** recording
`play.keys` (23 keystrokes) against those three sites captured 11 of 23 keys.
The missing 12 are the in-game `I`/`J`/`K`/`M` movement keys, consumed at
`$6217` — `LDA $C000` followed by `STA $C010` (clear the strobe) and
`STA $623C,X` (push into a 16-entry ring buffer) — the real-time ingest that
feeds the game's movement dispatch, not a one-shot setup read. Adding `$6217`
as a fourth site captured all 23. Per-site hit counts over the full 1,300-frame
run: `$FD1B` 181,193 hits / 11 keys, `$741F` 325 hits / 0 keys, `$7890`
364,211 hits / 0 keys, `$6217` 6,808 hits / 12 keys.

This was not a cosmetic gap. Because `probe_uses_key()` makes the frame drain
stand down entirely once a script delivers via `key`, a keyboard read at an
uncovered site does not fall back to cycle-quantised delivery — it gets
nothing, ever. Twelve of twenty-three keys would have gone permanently
undelivered on replay, the kind of failure that looks like a hung or
badly-broken decompiled game rather than a timing artifact.

`play.pkeys` (`0af108b`) was generated by running `snake-bytec1-run
--key-file=play.keys --probe=rec.probe --record-keys=play.pkeys` over 1,300
frames, then verified two ways: 24 lines (one header, 23 keys — the line
count that would have caught the missing-site gap on its own, had it not
already been caught by the recorded-key count above), and a replay
(`--key-file=play.pkeys --probe=play.probe`) reproducing the conversion run's
frame hashes exactly. `play.keys` itself was never touched.

### The plan's own test defect

Task 3 (the drain-stand-down guard) shipped a regression test specifying
`--frames=10`. That count cannot discriminate a fixed build from a broken
one: the installed keyboard site (`$FD1B`) is not reached at all until
roughly frame 8.3, and the second of the two test keys — the one that
actually distinguishes "delivered by the drain" from "delivered by `key`" —
is stamped almost 59 frames further out. At `--frames=10` the guarded and the
unguarded build produce byte-identical output, so the test failed the same
way whether the fix was present or absent, and very nearly shipped in that
state. Diagnosed by tracing every `$C000`/`$C010` access with its cycle count
across a `--frames=100` run and confirming the divergence is real and stable
there (3/3 runs); the test was raised to `--frames=100`, with headroom over
the measured ~59-frame threshold, and red-without-fix evidence was captured
at the new count before committing. A test that fails is not evidence unless
it has also been seen to fail *for the reason it claims to check* — the same
lesson the probe work has hit before with `grep FATAL` rejection tests.

### Task 6's result

With input on the shared coordinate, `decoded/snake-byte/probe-acceptance.sh`
(`2a76b62`) replays `play.pkeys` against the interpreter and both generated
back ends — `snake-bytec1-run` and `snake-bytec1-ext-run` — over the full
1,300 frames and compares two things.

**The block-head control-flow trace is byte-identical.** 2,744,938 probe hits
against every `CYCLES(` block head, matching exactly between the interpreter
and *both* generated builds (site lists of 1,694 and 1,669 block heads
respectively — the two builds' block-head sets differ because the `-ext`
build replaces some ROM/game routines with hand-written C, not because of
anything to do with input). Every basic block dispatched, in order, for the
entire run, is the same sequence on all three. Control flow is therefore
provably identical for this run, not merely "probably" — a much stronger
statement than a frame hash can make on its own.

**Frame 623 now matches.** It was the input coordinate, as the analysis above
predicted, and is not treated as resolved on inference alone: this is a direct
measurement, over the actual replay, with the actual generated builds.

**8 of 1,300 frame hashes still differ**, and this residual is **not
root-caused**: frames 172, 173, 271, 596, 761, 823, 871, 933 (172 and 173 are
one two-frame transient, so 7 distinct events, not 8), identically for both
generated back ends. Every differing frame re-converges on the very next
frame. For 6 of the 7 events, both engines' hashes at the differing frame
match *neither* the preceding nor the following stable value — both are
mid-write at slightly different points, not one engine lagging the other by a
frame. (Frame 871 is the one clean "interpreter is a frame behind" case.)

Why this is not a control-flow difference: the block-head trace is
byte-identical over the whole 1,300-frame run, so control flow is provably
identical throughout; what differs is only *where* a fixed-interval
video-hash sample lands relative to an in-progress screen write. That framing
is strongly supported — by the byte-identical trace and by the immediate
re-convergence on every one of the 7 events — but it is not proven. No attempt
was made to trace a specific differing frame down to the instruction where the
two engines' screen-write progress actually differs; that is future work.

### Left open

- **The 8 residual frames are not root-caused.** The sampling-artifact
  explanation above is well supported but unverified at the instruction
  level; nobody has traced a single one of the 7 events to the actual write
  in progress.
- **`play-hires.keys` is not converted.** It should follow the same recipe
  once needed, but needs its own site check — the hi-res scenario reaches
  `$664A`, a site not in `rec.probe`/`play.probe`.
- **`--trace-keys` still writes cycle stamps**, kept deliberately as the
  recording path for a session that has no probe script at all — but it is
  not otherwise unchanged: routing every key source through `push_key()`
  (the fix that lets `--record-keys=` convert a `--key-file=` recording,
  above) means `--trace-keys` now also dumps `--key-file=`-sourced keys, not
  just `--kbd-file=`/live typing. See `push_key()`'s comment in
  `lib/a2host/a2host.c` and `docs/probes.md`'s Execution status section.
- **Probe phase 3 is still unbuilt.** apple2tc does not emit `PROBE_x(...)`
  placeholder sites, so a probe observes machine state, not the generated
  C's own variables.


## 2026-08-15 — What was left of frame 623: synthetic return addresses and dead stack

**Scope:** decoded/snake-byte · **Status:** measured; the residual is fully accounted for

**Decision:** finish the frame-623 question rather than leave it at "strongly
supported." Probe-stamped input removed the divergence itself; what remained was
8 of 1,300 host frame hashes still differing, explained but not measured.

### The method the previous entry could not use

The block-head trace is byte-identical between the interpreter and both
generated builds. That makes the probe *counter* a coordinate the two engines
provably share, so state can be compared at program-defined instants instead of
at the host's frame timer — which is the thing frame hashes were never able to
do.

Two sampling attempts were wrong before one was right, and both are worth
recording because each looked fine:

- **`$3752`, 2,048 hits.** A self-modifying copy loop. All 2,048 hits fall in
  trace lines 389,941–392,002 of 2,744,938 — a startup window — yielding 8
  distinct screen states. "Identical at 2,048 samples" meant almost nothing.
- **`n % 512` on the coordinate counter.** Spread across the run, but biased:
  `$7890` (the wait-for-space loop) is ~66% of the counter's growth, so most
  samples caught a static screen. 1,079 samples, 26 distinct states.
- **Every hit of `$6217`**, the in-game keyboard ingest: 6,808 samples, reached
  only while playing, 6,808 distinct memory states.

### What the engines actually share

At all 6,808 in-game samples, interpreter vs `snake-bytec1` and vs
`snake-bytec1-ext`:

| region | result |
|---|---|
| `$0000-$00FF` zero page | identical |
| `$0100-$01FF` stack | **differs** |
| `$0200-$03FF` | identical |
| `$0400-$07FF` text page | identical |
| `$0800-$1FFF` | identical |
| `$2000-$5FFF` both hi-res pages | identical |
| `$6000-$BFFF` the game itself | identical |
| stack pointer | identical |

### The stack difference is entirely synthetic return addresses

Decomposed byte by byte over the same 6,808 samples, across `$01F0-$01FF`
(minimum SP observed was `$F1`, so the window covers every live byte):

- **27,232 live stack bytes differ, and every one holds `$FE` or `$FF` on the
  generated side. Zero hold anything else.**
- 13,700 differing bytes are below SP — dead residue, different reset fill.

The mechanism, confirmed at byte level rather than inferred: a generated
program's `RTS` is a C `return`, not a pop of a real address, so each entry
pushes a caller-supplied `ret_addr` — `$FFFE` for an entry point (82
`push16(ret_addr)` sites in `snake-bytec1.c`; `game.c` calls
`rom_cout1(0xfffe)`). At the first in-game sample, `$01FE/$01FF` holds `A8 FA`
= `$FAA8` on the interpreter and `FE FF` = `$FFFE` on the generated build;
`$01F3/$01F4` holds `8D 62` = `$628D`, the return address from the `JSR $6217`
at `$628B`, against `FE FF` again. Every byte between them matches.

### So the residual 8 frames

Control flow is identical, all memory outside the stack page is identical, SP
is identical, and the stack differs only where the decompilation puts a
synthetic value by construction. Nothing in the machine differs. The 8 frame
hashes differ because the host samples video on a fixed cycle interval and the
two engines' cycle accounting differs slightly, so the sample lands at
different points inside an in-progress screen write. Every one re-converges on
the next frame.

That is now measured rather than argued.

### `ram.probe` and the gate

`decoded/snake-byte/probe-acceptance.sh` gained a memory-equivalence check. It
is not redundant with the trace check: a wrong byte written to memory need not
change control flow within 1,300 frames.

Mutation-tested, and the first attempt was ineffective in the way this project
keeps rediscovering — a stray `ram_poke(0x1f00, 0x42)` inserted into the first
function in the file changed nothing, because that function never runs. Moved
to the `$6217` block, which the trace proves executes 6,808 times, it produces
exactly the discriminating result: **`[trace] PASS` and `FAIL [ram/ref]`**. The
memory check catches what the control-flow check cannot.

The stack page is excluded from the hash, and that costs nothing: the stack
pointer is compared, and any live stack value the program reads back would have
to change control flow to matter, which the trace check covers.

### Left open

- `play-hires.keys` is still cycle-stamped. Converting it needs its own site
  check — the hi-res scenario reaches `$664A`, which the four sites here were
  never shown to cover.
- Nothing compares the interpreter against a generated build for `robotron` or
  `bolo`; the machinery is now generic, but no coordinate script exists for
  either.


## 2026-08-15 — `--ret-addr` for verification, and how much the cycle phase matters

**Scope:** decoded/* · **Status:** applied; the phase question is measured and bounded

**Decision:** decompile the games with `--ret-addr` and record, loudly, that it
is a verification setting rather than something a shipped decompilation wants.

### The flag already existed

`routines.cpp:652` has always had it: with `--ret-addr` a `JSR` pushes
`inst->getAddress() + 2`, which is exactly what a 6502 pushes; without it, the
sentinel `$FFFE`. Only `decoded/rom` was using it. The other games were not,
which is why the previous entry found the emulated stack full of `$FFFE`.

The reason to turn it on is not the stack comparison. It is that the
inline-data-after-`JSR` idiom locates its data by *reading* the pushed address,
Snake Byte uses that idiom (the `"VALUE: "` string, already in HANDOFF.md's
traps table), and under `$FFFE` such code reads from the wrong place. It does
not bite on the recorded path — 1,300 frames pass either way — but "the
sentinel happens not to be read here" is a much weaker property than "the
address is real."

It is free. The address is a compile-time constant at each call site, so it is
the same `push16` of a different literal: identical line counts, 638 changed
constants in `snake-bytec1.c`, and `verify.sh` 4/4 against baselines recorded
*before* the flag. `bolo` is untouched — its active line is `--simple-c`, which
has no `ret_addr` concept at all (verified: the flag changes its output not at
all).

**Not for a shipped artifact.** A final decompilation wants `ret_addr == 0` —
no emulated stack maintenance whatsoever, since native C has no use for one.
That is a different setting, and `decoded/snake-byte/decompile.sh` carries the
distinction in a comment.

### The stack is now compared

`ram.probe` hashes everything above SP. Live stack: 27,232 differing bytes
before the flag, **zero** after, across the same 6,808 in-game samples. Bytes
below SP stay excluded — dead residue, 13,700 differing before and 2 after;
hashing them would compare reset fill patterns.

### How much does the cycle phase matter

Measured over the full run: **interpreter 22,147,953 cycles, generated
22,147,957** — four cycles apart in 22.1 million. Per-frame delta ranges −1 to
+57, and the 100-frame bucket means wander (1.1, 1.1, 4.1, 3.9, 4.9, 6.5, 8.7,
6.2, 8.4, 4.5, 1.5, 1.5, 1.5) and return. **The rate is identical; only the
phase differs.**

The mechanism is the yield granularity: `CYCLES(pc, n)` charges a whole basic
block at its head, before the block runs, while the interpreter charges each
instruction before that instruction runs. Mid-block the generated counter is
ahead by up to one block. Largest block constants: 85 cycles (snake-byte), 80
(rom), 118 (robotron) — 0.50%, 0.47% and 0.69% of a 17,050-cycle frame.

Where that can and cannot matter:

- **The program's own behaviour: never.** A 6502 has no readable cycle counter.
  Phase is only visible where the *host* reads the counter.
- **Host video sampling: visible, and it is the 8-of-1300 frame-hash residue.**
  A property of the measuring apparatus, not the program — which is why
  probe-sampled comparison replaced it.
- **Key delivery: eliminated.** Now on a probe coordinate, not cycles.
- **Speaker timing: bounded and negligible here.** Snake Byte reaches `$C030`
  at exactly one address, `$FBE9` in the ROM's BELL, and that address is itself
  a block head costing 8 cycles — so the toggle's timestamp can be off by at
  most ~8 cycles (~8 us). Against a 1 kHz tone's 500 us half-period that is
  1.6%, inaudible. A game with speaker writes buried inside large blocks would
  have a proportionally larger bound; none of these do.
- **Timing-sensitive hardware: not used.** No paddle (`$C064-$C067`, `$C070`)
  or cassette (`$C060`) access anywhere in Snake Byte, which is where a
  cycle-counted read loop would have made phase behavioural rather than
  cosmetic.

So: significant for anything that samples the machine on a fixed cycle
interval, and the answer there is to sample on a program coordinate instead.
Not significant for the decompiled program's behaviour, and not significant for
this game's audio. It would need re-examining for a title that reads the
paddles or generates tone from inside large basic blocks.


## 2026-08-15 — Converting `play-hires.keys`, and a read site the listing cannot show

**Scope:** decoded/snake-byte · **Status:** done; both scenarios gated

**Decision:** convert the second recording too, so the cross-engine gate covers
the hi-res path. It was the last scenario running on cycle-stamped input.

### Why this one was worth doing

It is not a second helping of the same test. The hires scenario reaches `$664A`
(the game's own hi-res COUT hook) and `$7541`, code the recorded session never
took and that exists only because `code-at.txt` asserts the edges. And in the
`-ext` build `$664A` is not decompiler output at all — it is hand-written C in
`game.c`. So hires/ext is the only cross-engine check that hand-written
replacement has ever had.

### The listing could not answer the site question

The coordinate has to cover every place the program takes a key, or the key is
dropped outright — `probe_uses_key()` stands the cycle drain down, so an
uncovered ingest is not a mis-timed key but a missing one.

Checking `snake-byte.lst` for `$C000` reads on the hires path found nothing new,
and that was misleading: `$7541` and `$664A` appear in the listing as `DFB`,
because the disassembler never traced them. **A `--code-at` region is invisible
to the listing by construction.** Grepping the generated C instead — the thing
that actually runs — turned up `$760F`, `$FB7C` and `$FD26`, none of them in the
coordinate. Which of those fire on the hires path, measured rather than
reasoned: `$FD1B` 181,193, `$6217` 8,465, `$760F` 1,059, `$741F` 536, `$FB7C`
20, `$FD26` 11, and `$7890` not at all (it is play-only).

### Which sites the keys actually need

Against `play-hires.keys`, 19 keys:

| coordinate | captured |
|---|---|
| the four sites from the play scenario | 13 of 19 |
| four + `$760F` | **19 of 19** |
| four + `$FB7C` | 13 of 19 |
| four + `$FD26` | 13 of 19 |

`$760F` is the one that matters. `$FB7C` and `$FD26` are in the set anyway: a
site that fires without consuming costs one counter increment, both engines
count it identically, and a read site nobody thought about turning out to
consume is exactly the failure being guarded against — it has now happened
twice, `$6217` for play and `$760F` for hires. `$FD26` is in fact where KEYIN
fetches its key; it takes none here only because `$FD1B` always fires first.

All seven are block heads in both builds, checked before use.

`play.pkeys` was re-derived on the seven-site coordinate. Both files replay to
their conversion run's frames exactly, 1300/1300.

### Result

Interpreter against both generated builds:

| scenario | block-head trace | memory |
|---|---|---|
| play | 2,744,938 hits identical | identical at 6,808 samples |
| hires | 2,864,242 hits identical | identical at 8,465 samples |

### One hazard closed on the way

The coordinate is duplicated across five scripts, because the probe language
has no include. That is the `trace.probe`/`trace-ext.probe` drift hazard
multiplied, and worse in kind: a stamp file would silently mean different
things to different scripts while each stayed individually valid. The gate now
asserts all five `install kb` lines are identical and prints the coordinate it
ran with.

### Left open

- `robotron` and `bolo` have no cross-engine comparison. The machinery is
  generic now — a coordinate script and a converted key file are all either
  needs — but `bolo`'s `--irc1` line is still commented out in its
  `decompile.sh`, so it has no `c1` build to compare against.

---

## 2026-08-15 — Hand-decompiling the game, and what it exposed about the gate

Sixteen of Snake Byte's own routines now have hand-written bodies in `game.c`
rather than generated ones, listed in `rom.externs` beside the ROM entry points.
In order: the hi-res cell plotter (`$6127`/`$60E7`/`$60E4`), the cell-run
plotters (`$6148`/`$615A`), the screen-script primitives (`$7019`/`$7024`/
`$7000`), the score (`$71F3`/`$7226`/`$7267`/`$702B`), and the RNG and merging
plotter (`$6C4B`/`$6B93`).

### What the game turned out to be

Hi-res page 1 is a grid of 48 cell rows, four scanlines tall and one byte wide.
Every draw happens twice: once with the ROM's lo-res `PLOT` and once as hi-res
cells. The hi-res page is the display; the lo-res page is a 40x48 occupancy map
the game reads back with `SCRN`. `$62D7` adds the direction deltas from
`$6232`/`$6237` to the snake's head, `SCRN`s the result, and learns what it is
about to hit. Collision detection for free, at the cost of drawing everything
twice.

`$7113` runs a byte-coded display list — `'H'`, `'V'`, `'P'`, `'T'` — which is
why `$7019`, its byte fetcher, has fourteen call sites. The score is four BCD
bytes at `$7252`, printed nibble by nibble with `$002C` as a leading-zero flag;
`$002C` is also the ROM's `H2`, reused because drawing and score printing never
interleave. The pseudo-random source is not a generator: it walks a pointer
through the game's own memory and returns the first byte with bit 7 clear.

### The gate was weaker than it looked

Mutating `game_next_byte`'s page-crossing branch passed all eight checks. `$7021`
never executes — the display lists these recordings run never straddle a page —
and the "a probe never fired" guard could not see it, because `trace` is one
probe installed at 1,669 addresses and it fires constantly.

So `probe-acceptance.sh` now reports per-site coverage, and the numbers are the
finding:

| | |
|---|---|
| block heads reached, either scenario | 744 of 1,669 — **45%** |
| hand-written blocks reached | 112 of 134 |
| hand-written blocks reached by nothing | 22, asserted as a baseline |

Twenty of the 22 are in `a2rom.c`, ROM paths for arguments the game never
passes. The other two — `$7021` and `$6C4F` — are the high-byte carries of the
display-list and pseudo-random pointers. The baseline is asserted so the set
cannot grow quietly; `$6C4F` was caught that way, by the gate failing rather
than by anyone noticing.

Mutation-tested by adding an unreachable `CYCLES` site to `game.c`: trace and
memory both still passed, coverage failed.

### And coverage is not the whole story either

`game_add_score` has full block coverage. Replacing its BCD addition with binary
addition on the score's **second** byte fails nothing — the score never reaches
100 in either recording, so that byte never needs decimal correction. The low
byte's equivalent mutation is caught, and only by `hires`. Block coverage is not
value coverage, and the new report cannot see this class of gap.

### Two smaller results worth keeping

**A flag apple2tc drops is not a flag the CPU drops.** Forcing `$6148`'s carry to
zero passes all eight checks: the generated code computes it, nothing observes
it. That is whole-program DCE proving a fact about the current call graph. The
hand-written routines set what the 6502 sets, because re-deriving that proof on
every change costs more than the assignment.

**An instruction that looks like a typo may still be load-bearing.** `$60F7` and
`$6B93` build the same table index from the same operands, one with `ROL $06`
and one with `ROR $06`. The `ROR` puts the scanline parity in bit 7, where the
two following `ASL`s discard it, degenerating the index from
`ink*8 + parity*4 + col&3` to `(ink>>1)*4 + col&3`. Changing it to `ROL` fails
`[hires/ext]`. It is on screen, whatever the author meant.

### Corrected

The `$6064` comment from the plotter commit said the dot-pattern table was
sixteen bytes. It is 128, `$6064`-`$60E3`, sixteen inks of eight — ink 0 all
zeros (erase), ink 15 all `$7F` (solid), the rest dithers. The error surfaced
only when `$7642` turned up passing ink 9.

### Left open

- The display-list interpreter itself (`$7113`-`$71C0`) is decodable and highly
  structured, but almost every opcode case is dead in both recordings. Hand-
  writing it would add a dozen blocks to the unverified baseline.
- `$7642`, `$6AB8`, `$64C8` are the next covered routines by call count.
- There is a stray empty `snake-bytec1-ext.c` at the repo root, committed by
  accident in `396c05d` and referenced by nothing. It shadows reads of the real
  file when a command runs from the repo root; worth deleting.

---

## 2026-08-15 (later) — Every game routine is hand-written now

Twenty-eight of Snake Byte's own routines have hand-written bodies in `game.c`,
2,837 lines. What is left in the generated file is `func_t001`, the
switch-dispatch thunk, and the blocks it dispatches to.

### The game, as it now reads

Hi-res page 1 is a 40x48 grid of cells; the lo-res page is the occupancy map
the game reads back with `SCRN`, so everything is drawn twice. `$0301` is the
difficulty, 0-2, and it is a count: it indexes `$71C8` = {$10,$15,$20} to set
the per-apple score, decides how many bouncers `$6594` steps, and opens exactly
that many wall gaps in `$7045` -- one per bouncer, which is how they get in.
The score is four BCD bytes; an apple is worth base[difficulty] * level.

Two pieces of 6502 craft worth keeping. `$6C49` is the mute: every sound
routine clicks with `LDA $C000,Y` and that byte is `$30` (speaker) or `$20`
(cassette out), so muting routes the identical click somewhere nobody listens
and cannot change the timing. And a bouncer reflects with `EOR #$FE`, which
swaps `$01` and `$FF` -- a wall bounce with no compare.

### The gate got one thing badly wrong, and it was mine

I wrote `$728D`'s four BCD compares as a loop over a table of addresses:
`CYCLES(kCmp[i], 10)`. It compiled, ran, and passed everything, while dropping
eight block heads out of the site list -- 1,669 to 1,661. The probes were never
installed there, so neither engine reported those addresses and the traces
agreed about nothing.

Nothing in the gate caught it. The site-count floor is 1,600, far too coarse
to see eight disappear, and coverage is measured *relative to* the site list,
so a missing site is invisible there by construction. I noticed because the
number is printed and I happened to read it.

`check_literal_sites` now runs first and fails on any `CYCLES` in a
hand-written source whose address is not a hex literal. Mutation-tested.

### What 60 unverified blocks actually means

The baseline grew 21 -> 60 across this pass. Grouped, it is a description of
what the recordings do not do:

| group | blocks |
|---|---|
| the joystick | 15 |
| ROM paths for arguments never passed | 20 |
| the `'P'` display-list opcode | 7 |
| pause and mute (ESC, Ctrl-S) | 6 |
| arrow keys on the redefinition screen | 3 |
| difficulty 2: second bouncer, second gap | 3 |
| bouncer off the board | 2 |
| dead end, level 30, bad opcode, page carry | 4 |

Two thirds of the growth is the joystick and one unused opcode. Both are
decoded from the binary alone. A 9,000-frame run of the `easy` fixture was
measured and adds nothing to any of them.

### Three more oracle asymmetries

- A byte written at snake reset and never read again (`$663B`) moves no frame
  hash. Only `[ram/ext]` catches it.
- A wrong glyph on the key-redefinition screen passes the whole cross-engine
  gate. `ram.probe` samples at `$6217`, which does not fire outside play, so
  that screen is frame-hash territory only. The two checks are complementary in
  *both* directions.
- A sound loop running half its iterations moves no frame hash either, but
  moves the block-head count immediately.

And a third class the coverage report cannot see at all: **value gaps**.
Hardcoding `$6C49` to the speaker, widening the key ring's `$0F` mask, or
switching the score's second BCD byte to binary all pass every check, with full
block coverage, because no recording varies the value that would expose them.

### Left open

- `func_t001` and the switch dispatch are the remaining generated code.
- A joystick recording and a difficulty-2 recording would together retire ~18
  of the 60.

## 2026-08-15 (later still) — The conversion to real C, and what it needed

`game.c` 2,509 lines and `game_native.c` 488 at the start of this stretch;
1,090 and 2,215 at the end. Every game routine that has a body now has it in
real C except one, and the exception is deliberate.

The trace site list went 1,694 → 1,390 for the extern build, every step of it
pinned exactly and written down in the same commit that spent it. The pin
earned its place several times over: it is the only thing that makes "this
conversion cost 17 sites" a number in a diff rather than an observation nobody
makes.

### The tool already answered the question I kept guessing at

Every adapter has to decide which registers the original left behind that its
caller actually reads. I traced that by hand once — four routines deep past a
`JSR`, inconclusive — before noticing that `apple2tc --ir` prints
`LiveIn`/`LiveOut` above every function, computed over the whole call graph.

    // LiveIn : Y, STATUS_D, STATUS_I
    // LiveOut: A, Y, STATUS_N, STATUS_D, STATUS_I
    function func_6c72

No X. So `$6C72`'s converted form does not write one, and `$6594`'s does,
because that one's `LiveOut` says `A, X`. It decided the flag sets for six more
routines the same way, and in each case the emulator-shaped version had been
writing three or four registers that nothing reads.

This sits against the older rule — *match what the 6502 sets, not what DCE
kept* — and the resolution is cost. A status flag is one line and always right,
so write it. A register write inside real C costs the abstraction the whole
exercise is buying, so it is worth asking, and now the asking is a command.

### Three spellings, because there are three reasons

Converted code charges its cycles with `GAME_CYCLES`, which does not probe. That
is right only when nothing else probes the address either, and twice it was not:

- **`$6216`** is an `RTS` two routines share. One converted, one did not, and
  the interpreter kept reporting an address the generated build had stopped
  emitting. Three divergences in 400,000 hits.
- **`$720E`** is stranger. The low half of `$71F3` survives in the generated C
  as an orphan — its only predecessors were in the extern-replaced region, so
  nothing can reach it — but it is still text in the file, so it is still on the
  site list.

And a third reason with a different shape: **`$760F` and `$6217`** are two of the
seven addresses the replay coordinate counts. Dropping their probes does not
make the engines disagree about the program, it makes them disagree about *when
the keys arrived*. Measured: converting `$760F` fails the trace at line 640,983
of the diff, naming four addresses on a screen that is not the problem, and
`verify.sh` does not notice at all.

So `GAME_CYCLES_SHARED` and `GAME_CYCLES_COORD` keep their probes, and
probe-acceptance.sh checks all three spellings mechanically: plain must appear
in no other source, `_SHARED` must, `_COORD` must be on the coordinate. One
exemption, recognised by the zero — `CYCLES(addr, 0)` in an adapter, which
probes without charging while the native charges without probing. Turning the
lint on immediately named six addresses; all six were that pattern, which is how
the exemption came to be written. Four failure modes mutation-tested.

### What the mutation tests found that the conversions did not

Three gaps, all in code with full block coverage:

- **The key table's indirection.** Six bindings map to six commands through two
  parallel arrays. Reading the wrong array passes everything — `play.pkeys`
  presses the *default* bindings, where the arrays are byte-identical, and
  `play-hires.pkeys` rebinds and then ends without playing.
- **The score's BCD carry.** Breaking the propagation between score bytes passes
  verify.sh 4/4, both 1300-frame traces, memory and screen. Only the 3,000-frame
  `easy` run fails: neither committed recording scores enough to cross a byte
  boundary. The fixture built to make the display list reachable turns out to be
  the only thing checking carry at all.
- **The key ring's `$0F`.** Still open, still unreachable: no recording presses
  sixteen keys faster than the game reads them.

Each is noted at the line, because each now looks like something a reader would
tidy.

### One routine deliberately not converted

`$69A9`, pause and mute, is six blocks and all six are on the unverified list —
no recording presses ESC or Ctrl-S. Converting it would be a rewrite nothing
checks, which also removed the last accounting of itself. It stays in the
emulator-shaped file until a recording reaches it.

### And one gate that was not gating

`verify.sh` read `$BIN` and ignored its arguments, so
`./verify.sh ../../cmake-build-release` — exactly how the sibling
probe-acceptance.sh is invoked — tested `cmake-build-debug` instead. A mutation
run came back four false PASSes before that surfaced. It now takes a build
directory and rejects arguments it does not understand.

### Left open

- `$69A9`, above, waiting on a recording.
- A recording that rebinds the keys **and then plays** would close the key-table
  gap.
- The unverified baseline is down to 26 from 60, and almost none of that is
  progress: the blocks left the site list with their routines. The pinned site
  count is the honest number.

## 2026-08-22 — `$69A9` converted; waiting on a recording is not a plan

**Superseding note first.** The "Left open" list closing the 2026-08-15 entry
above is stale and was already stale when this was written. It says `$69A9` is
waiting on a recording and that a rebind-then-play recording would close the
key-table gap; both were done on 2026-08-16 (`3648097`, `play-rebind.pkeys`).
Between 08-16 and 08-22 the lessons went into `playbook.md` and this file got
no entries, so its tail reads as the current state and is six days behind. It
is left in place per the append-only rule. Read the playbook and
`probe-acceptance.sh`'s baseline comment for where things actually stand.

### The decision

`$69A9`, pause and mute, was hand-decoded into `game.c` but deliberately not
converted to real C, on the rule recorded in the playbook: a conversion is
checked by the recordings running it, and no recording presses ESC or Ctrl-S.
`play-rebind` later reached `$69A9` itself and both its fall-through blocks,
which made the routine half-covered and the rule half-applicable, and it stayed
unconverted on that basis.

That is now overruled as a project decision, not a routine-level one: **no
further recordings will be made**. The rule priced waiting at zero, and with
nobody going to record an ESC press, "convert it later" meant "never". The
decode is checkable against the binary regardless — `id` for the instructions,
the opcode table for the cycle charges, `--ir` for the liveness — which is the
same evidence every converted routine already rests on for the branches its
recording did not take.

So `game_pause_or_toggle_sound_native()` is in `game_native.c`, with a five-line
adapter left in `game.c`. Cost: 5 of the 6 block heads, 1176 → 1171 pinned
sites, exactly as predicted from the block count.

### What the binary settled that reading the C could not

Two questions came up, and both were answered by enumeration rather than by
argument:

**The caller's key.** `game_play_loop_native` tracks the key in a local `code`
while the old adapter read `s_a`, and those are written independently — the
auto-steer path sets `code = proposal` and leaves `s_a` stale. If `$639B` were
reachable that way the adapter was already wrong. It is not: the substitution
tables at `$6387`-`$6396` hold only `$00`, `$88` and `$95`, and `$00` fails the
high-bit test at `$6291` while `$88`/`$95` match at `$6293`; the auto-steer's
proposals come from `tbl_6A55` = `00 CB C9 CA CD` with the direction always
1..4, and all four of those match earlier in the chain. Nothing substituted
survives to `$639B`, so `code == s_a` there and the parameter is exact.

**What to write back.** `--ir` gives `func_69a9` `LiveOut: STATUS_C` and nothing
else. A is dead — which incidentally disposes of a divergence in the old hand
decode, where the Ctrl-S path left the key in A and the original leaves the
toggled flag there. Nothing ever read it. The adapter now writes the flags and
not A, and says so at the line.

### What mutation found, and it is about the oracles

Inverting the Ctrl-S test is caught: `trace-ext` fails on the play-rebind
scenario. Mis-charging the entry block is **not**. 4 cycles written as 5 passes
verify.sh 4/4 and every probe scenario; written as 4000 it fails on
frame-boundary drift.

The reason is structural and worth keeping. verify.sh is the gate that compares
cycles, and its two scenarios never press an unrecognised key, so they never
enter the routine. probe-acceptance.sh does enter it, and stamps input on a
probe counter rather than on cycles — deliberately, since that independence is
what made the two engines comparable at all (2026-08-14). Being cycle-blind is
the property that makes it useless for this. The cycle charges here therefore
rest on the opcode timings and not on a passing test, which is written above the
routine.

Nothing to fix: the counter's independence is load-bearing and a third gate is
not worth its weight for one routine. What changes is how the coverage number is
read. "84/104 hand-written blocks run" reports control flow and timing as one
figure, and for this routine one of the two is checked.

### Also corrected here

`probe-acceptance.sh`'s baseline comment had drifted badly. Its arithmetic cited
1,519 and 1,669, neither of which the file has produced since `--prune-returns`,
and four of its six named groups of unverified blocks — the `'P'` opcode,
difficulty 2, level 30 and the unrecognised opcode — had left the site list with
their routines' conversions without being marked. With `$69A9` converted the
baseline is **20, all of it ROM in `a2rom.c`**, and the number can no longer say
anything about the game at all. The per-routine comments took over that job, and
they are prose that nothing checks. That is now stated in the file rather than
implied by it.

## 2026-08-23 — Step 2 begins: naming the scoreboard, and two comments it caught

The cleanup plan's step 2 is a pure rename — 115 addresses that stand in for
variables become names — chosen to go first because the block-head trace checks
it at full strength and nothing about storage moves. The first slice is the
scoreboard, `$7252-$7266` plus `$71CB`, `$78B0-$78B2`, `$0300`, `$0301` and
`$0304`: 19 fields, 92 substitutions.

### The oracle for a rename is not the gate

The gate was run and is green, but it is not the interesting check. A rename
that is genuinely a rename must compile to the *same object*, so that is what
was checked: `-O2`, where the new one-line accessors inline away, and the
disassembly diffed against `HEAD`. Four lines differ, and all four are the same
two `__LINE__` immediates for assertions, displaced by the lines the accessor
block added.

This is strictly stronger than any behavioural gate, it costs one compile, and
it applies to every remaining slice of step 2. It stops applying at step 3,
which is where storage starts moving — and that is the point at which the memory
probe added in step 0 becomes the oracle instead.

The same check was what made the earlier `.inc` split defensible: moving 2,020
lines of hex out of `snake-byte-cold.c` should be provably inert, and it was.

### Naming forced two comments to be checked, and both were wrong

Neither would have been caught by any gate, because a comment cannot fail a
test. Both had been sitting in converted, verified, hand-written C.

**`$725F/$7260` was commented "apples until the next one appears".** It is the
opposite: the number of apples on the playfield *right now*. `game_place_apple`
ends by BCD-incrementing the pair at `$766C`, `$7743` decrements it when one is
eaten, and `$77D0` places a replacement exactly when it reaches zero. Read the
old way the arithmetic is nonsense — the pair is zeroed at `start_round`, so the
first apple eaten would take it to `$9999` and no apple would ever be placed
again.

That reading was tested rather than argued. A probe at `$7743`, `$77D0` and
`$7642` over all three scenarios: `play` and `play-rebind` never eat an apple at
all, and `play-hires` eats four — `$725F` is `$01` at every one of them, and
`$77D0` fires and places a replacement every time. The disassembly at `$766C`
then says why.

Worth noting how close the wrong version came to surviving: the *converted*
`game_place_apple_native` already carries the correct comment ("One more apple
on screen"), three thousand lines away from the top level's wrong one. Two
comments about one pair of bytes, disagreeing, both hand-written.

**`$0304` was commented "apple value" in two places.** It is the level's time
allowance: the display list's `'T'` command writes it, it seeds `$7266` which
seeds `$6255`, and `$6255` is the life timer that `$641C` runs out. The apple
value is `$71CB`, computed by `$71CD` from the difficulty and the level number
— and `$71CD` never reads `$0304`. The confusion is understandable, since
`start_round` copies `$0304` and then immediately calls `$71CD`.

A third comment said scoring stops after "the first `$110` apples". `$777B`
tests the high byte for zero and `$7780` compares the low against `$11`, so it
is the first `$11`.

### One thing named but not verified

`$77BC` sets `$7266` to `$FF` once the round's last apple is gone. That does not
lengthen the timer, it stops it: `$6255` counts down one per pace, but
`game_draw_side_walls` treats any value with bit 7 set as out of range, clamps
the wall height, and writes `$FF` back at `$6B55` — so the count never reaches
zero and the run to the gate is untimed.

This is decoded from the two call sites, not observed. A probe at `$6B55` and
`$641C` shows all three recordings seed the timer `$64` and never reach the
clamp. Under the standing no-new-recordings agreement that is fine, and the
comment on `kLifeTime` says which half is which.

## 2026-08-23 (later) — Step 2 finished: 115 addresses, and four wrong names

All 115 are named, in six slices: the scoreboard, the snake, the plotter's
argument block, the monitor's zero page, the settings block and pointers, and
the tables. `ram_peek(0x...)` does not appear in `snake-byte-cold.c` any more.

### The check held for every slice

Each slice was proven inert by compiling at `-O2` and diffing the disassembly
against `HEAD`, permitting only `__LINE__` immediates to move. That caught
nothing, which is the point: it means the six commits are a rename and the
gates were never the thing standing between a mistake and the tree. One slice
needed the symbol name normalised out of the disassembly first, because it
renamed a function rather than an address.

The gates ran on every slice too, and were green every time.

### Unions are real and must not be tidied

Two blocks turned out to be hand-written unions, and naming them uniformly
would have been a lie in both cases.

`$0000-$0008` is the plotter's argument block — shape, ink, cell, the hi-res
destination, the run end. `game_cout_hook_native` reuses all nine bytes for an
unrelated job: `$0000` is a pointer into the font, `$0002/$0003` hold the
caller's X and Y (`$669F` restores them from there), `$0008` is the character.
Only `$0004` means the same thing in both. So there are two enums over the same
addresses and the substitution went per routine.

`$002C` is the same story across a smaller gap: H2, the right-hand end of a
lo-res HLINE, and separately the flag `game_print_bcd` raises when it prints a
digit. Drawing and printing never overlap, so the original's reuse is safe.

That second one paid for itself immediately. `$794D` in `game_bonus_screen`
zeroes `$002C` right before printing two BCD bytes, so it is the digit flag —
the same thing `clear_leading_zero_flag()` does for every other field — and a
uniform substitution would have called it H2 and left it looking like a drawing
argument for good.

### Four names were wrong, and only reading for names could find them

None of these can fail a test. All were in hand-written C that passes every
gate, and two of them had a *correct* comment about the same bytes elsewhere in
the same file.

- **`$725F/$7260`** was "apples until the next one appears". It is apples on the
  playfield now.
- **`$0304`** was "the apple value", twice. It is the level's time allowance.
- **`poll_and_discard()`** was neither. It reads `$C000 + $6C49`, and `$6C49` is
  the click port — `$30` the speaker, `$20` the cassette output nobody can hear,
  which is how muting works. One speaker click. What hid it is that the symbol
  database resolves `$C000` to KBD, so the disassembly reads `LDA KBD,Y` and
  only the index says otherwise. `game_sound_sweep` does the identical thing
  fourteen lines away and its comment was right all along.
- **`game_eat_apple`** awards an extra life. `$7633` BCD-increments `$725E`,
  which the status panel prints as SNAKES LEFT, and its one caller is the
  round-cleared path straight after the bonus screen. This one was not just a
  comment: the name came from `rom.externs`, so the generated C called
  `game_eat_apple()` too, and fixing it meant changing the map and
  regenerating.

The pattern is worth naming. Each wrong name survived *because* the address was
spelled in hex everywhere it was used: nothing ever put the claim and the
arithmetic side by side. `kLives` and `adc_dec16(lives(), 1, 0)` in a routine
called `eat_apple` cannot be read past. `ram_poke(0x725e, ...)` can, and was,
for months.

### What this costs the next step

Step 2 spent no oracle at all. Step 3 moves storage out of emulated RAM, and
that is where `ram-cold.probe` — added in step 0 for exactly this — stops being
redundant and starts being the only thing checking that a value still means
what it did. The `-O2` check does not survive: moving a variable changes the
generated code by design.

## 2026-08-23 (later still) — Step 3a: the plotter's block leaves RAM, and the first oracle is spent

`$0000-$0008` no longer lives in `s_ram`. It is `s_plot[9]`, a C object, and
196 accesses moved with it. This is the first change in the cleanup that a
behavioural gate cannot fully check, and the memory half of the cold gate was
narrowed to say so.

### The union survives the move, deliberately

Step 3 in the plan is "real parameters", and `$0000-$0003` is exactly what that
means: shape, ink, column, row, set by a caller and read by the plotter. But
two of the nine bytes are not that, and one of them is load-bearing.

`bouncer_step` erases where the bouncer was using "whatever the caller last
left in `$00`" — its own comment, and true. `game_cout_hook_native` writes
`$0000-$0005` and `$0008` as a font pointer and the caller's saved X/Y. So a
COUT through the hi-res hook, between a shape write and that erase, changes
which cells get erased.

Measured over all three recordings: the erase only ever sees `$01` or `$15`,
both real shapes, never a font-pointer low byte. That is *not* enough. None of
the recordings reaches the bonus screen, and the bonus screen is the one place
the hi-res hook is installed while the play loop is running. The measurement
covers the case that cannot happen and misses the one that can.

Under the no-new-recordings agreement the answer is not another run: it is an
argument from the binary that the hook cannot be installed across that window.
That argument has not been made, so the nine bytes moved *together*, still
aliased, still indexed by the same two enums. The move is bit-identical by
construction and needs no argument at all.

Splitting them into real parameters is still the goal. It is now a separate
step with a stated precondition, which is better than a step that quietly
assumed one.

### Spending the oracle, and proving it was worth something

The retirement was done in the order that produces evidence rather than
assertions.

First the move landed with the probe untouched, and the gate was run: **trace
PASS, screen PASS, memory FAIL.** That is the memory oracle doing exactly what
step 0 built it for — the values still drive the same drawing, so nothing
visible changed, and only the memory hash could see that the bytes had gone.

Then `ram-cold.probe` was narrowed to skip `$0000-$0008`, and all three pass.

Then the narrowed probe was mutation-tested, because a probe that has just been
made weaker should be made to fire before it is trusted: a stray
`ram_poke(0x0040, 0x99)` in `game_draw_head_native` is caught by **memory
alone** — trace and screen both pass it. So the remaining range is still
covered, and the class of bug the memory check exists for is still the class it
catches.

### What is no longer checked

A wrong shape, ink, column or row still changes pixels, and the screen compares
all 6,808 samples. What left with the hash is a value that is wrong but never
drawn.

Probe phase 3 — apple2tc emitting `PROBE_x(...)` sites so a probe can read the
generated C's own variables — is what would restore it, and is unbuilt. That is
now the second thing on its list of justifications, after reading the generated
C's block state.

## 2026-08-23 (last) — The cold gate was blind to a third of the build

Step 3b was going to be splitting the plotter's block apart into real
parameters. Making the argument that the split is safe meant asking what the
gate would catch if it were wrong, and the answer turned out to be nothing.

### Two holes, found by asking rather than by failing

**The cold gate ran one scenario.** `play` never presses `C`, so it never
reaches the hi-res text screen, the key-redefinition screen, or
`game_cout_hook_native`. Measured: the cold build runs `$664A` **zero** times
under the gated scenario. Every byte of the glyph blitter was ungated, which is
precisely the code the union argument is about.

`play-hires-cold.pkeys` closes it, and needed no new recording — the
no-new-recordings agreement already anticipated this. `make-cold-keys.sh`
shifts `play-hires.pkeys` down by the coordinate of the first `$3750` arrival;
the offset is 181,207, re-measured rather than trusted. The cold build now runs
`$664A` 205 times per gate run.

**Adding the scenario was not enough, and the mutation said so.** With
`hires` in the gate, shifting the glyph blitter's source by one row — 205 wrong
glyphs — still passed all six checks. `screen.probe` and `ram-cold.probe`
sampled only at `$6217`, the in-game keyboard ingest, and the redefinition
screen reads keys at `$760F` instead. So the new scenario bought trace coverage
and no state coverage at all: the samples were still taken exclusively during
the part of the run that already worked.

Both probes now install at `$6217, $760F`. The same mutation then fails
`[cold/hires]` on the screen.

This is the trap table's "a probe that produces no output says nothing about
agreement", one level up: a probe that produces plenty of output, from only the
states you were not worried about.

### Two bugs in the comparison itself

**A magnitude proxy standing in for the property.** The alignment check was "at
least 100,000 blocks after aligning at `$3750`". That is not what alignment
means, and `hires` produces 95,468 — it spends most of its run in the
redefinition screen, which is converted C and barely probed. The check now
tests the property directly: the first line of the aligned trace must be
`3750`.

**Comparing a long file against a short one.** The screen and memory checks
took the *cold* build's sample count and truncated the booting build to it.
Cold skips the boot, so the same frame budget carries it further, and in `hires`
it produces 8,731 samples against 8,465. `cmp` then failed on length and
reported "screen differs from the booting build" — a real-looking failure with
nothing behind it. Both sides are now truncated to the common prefix. The
`play` scenario never showed this because cold happened to produce fewer samples
there.

### What this says about the step it interrupted

The union split is still not done, and it is now clear it should not have been
attempted first. The reasoning that would have justified it — "every path from
a hook install to a shape read passes through `start_round`" — is sound as far
as it goes, but it would have been checked by a gate that could not see the
routine in question at all. Strengthen the oracle, then spend it.

## 2026-08-24 — Step 3d: the cursor, and a category that was never real

`$0024/$0025` — CH and CV — are `s_ch` and `s_cv`. 43 accesses, no 16-bit or
computed reads, so the move was mechanical.

What is worth recording is the reasoning that nearly deferred it. The cursor was
set apart from the rest of step 3 on the grounds that it belongs to the ROM
rather than to the game: the monitor's routines compute with it, five of them
are still decompiler-generated in this file, and the game writes it as the
documented interface to COUT. The conclusion drawn from that was to wait until
those five were hand-written, so the change would land in "owned" code.

That is wrong, and the error is worth naming because it will recur. **There is
no such category as decompiler-generated code here.** Everything in this
artifact is a decompilation of the same binary; a routine still emitted as a
switch over block ids is a less finished one, not a different kind. Treating
the ROM as someone else's code invents a boundary the project does not have,
and the plan reads better without it — the five remaining generated helpers are
work items, not a reason to route around them.

### The oracle, third narrowing

`ram-cold.probe`'s zero page is three pieces now: `$0009-$0023`, `$0026-$004D`,
`$0050-$00FF`. Same order as before — move first, watch the gate read *trace
PASS, screen PASS, memory FAIL*, then narrow, then mutate.

Both new edges were mutation-tested and one of them says something useful.
`$0023` is caught. `$0040` is caught. **`$0026` is not**, and that is not a
hole in the hash: GBASL is recomputed by GBASCALC before every use, so a stray
write is overwritten long before either sample point sees it. The byte is
covered and the value is simply not observable at rest. Widening the hash would
not change that; only sampling more often would, and there is nothing to catch.

What still covers the cursor is the screen, at both sample sites. A cursor that
is wrong puts characters in the wrong place, which is the whole of what CH and
CV do.

## 2026-08-24 (later) — The last five decompiler-shaped routines become C

`FUNC_BASCALC`, `FUNC_VTABZ`, `FUNC_CLREOL`, `FUNC_CLREOLZ` and `FUNC_MON_WAIT`
are `rom_bascalc`, `rom_vtabz`, `rom_clreol`, `rom_clreolz` and `rom_wait`.
214 lines of `bb_N:`/`goto`/`tmpN_U8` became loops and ifs, and the `bb_` label
count in the file fell from 141 to 112 — what is left is inside the other ROM
entry points, which are the same job not yet done.

They were the routines the previous entry's category error was protecting.
There was nothing to protect: they are BASCALC, VTABZ, CLREOL and WAIT, four of
the best-documented routines on the machine, and reading them as C takes about
as long as reading the switch did.

### Decimal mode is not asserted away here

The converted game routines assert `D` is clear and drop the decompiler's
decimal arms, on the argument that the game never enters them in decimal mode.
That argument does not transfer. `game_bonus_screen` *is* entered with `D` set —
the playbook already says so — and it prints, so COUT can reach BASCALC and
VTABZ with decimal mode on. Both arms are kept, written as an `if (s_status_d)`
over the same `adc_dec16`/`sbc_dec16` helpers the rest of the file uses.

Keeping them costs four `if`s and needs no argument at all, which is the right
trade for code on a path nothing measures.

### Two of the five are decoded and not verified, and say so

Coverage was measured rather than assumed, which is the rule the cold-gate
episode produced. Probed at their entry addresses across both cold scenarios:

| routine | play | hires |
| --- | --- | --- |
| `rom_bascalc` | 26 | 119 |
| `rom_vtabz` | 26 | 119 |
| `rom_clreolz` | 8 | 24 |
| `rom_clreol` | **0** | **0** |
| `rom_wait` | **0** | **0** |

`rom_clreol`'s only caller is `$FC9A` on the scroll path and nothing in the
recordings scrolls; `rom_wait`'s are inside BELL1 and nothing outputs a Ctrl-G.
Both carry a comment saying so. No coverage was lost — the generated versions
were equally unreached — but an error in either transcription would not be
caught, and that is now written where someone will read it rather than inferred
from a table of block counts.

## 2026-08-24 (last) — SETVID had been writing to nowhere for four commits

Converting `rom_setkbd` and `rom_setvid` out of `bb_N` form turned up a bug the
conversion did not cause and the gate could not see.

Both routines end in the shared SETIO tail, which stores the vector with
`ram_poke(s_x, s_y)` / `ram_poke(s_x + 1, s_a)` — X being `$36` for SETVID and
`$38` for SETKBD. When the monitor's zero page moved out of emulated RAM,
`$36/$37` became `s_cswl`/`s_cswh` and every *named* use moved with them. This
one is not named: the address is in a register. So SETVID kept writing RAM `$36`
and `rom_cout` kept reading `s_cswl`, and the two had nothing to do with each
other.

### Why nothing caught it

Two accidents, both of which will recur:

The entry snapshot already holds `$FDF0` at `$36/$37`, so `s_cswl` was correct
before SETVID ran and correct after it did nothing. And the game calls SETVID
exactly once, at `$376A`, before it has repointed COUT at anything. The dead
store therefore had no effect *on this program* — but it is the routine whose
whole job is getting out of the hi-res hook, so a second call after
`game_install_cout_vector` would have left COUT hooked forever.

The general shape is worth naming: **moving storage out of an addressed space
is only mechanical for the references that name the address.** A store through a
register, an indexed table, or a computed pointer keeps compiling and stops
meaning anything. Grep found all twenty-one named uses and could not have found
this one.

### The mutation that shows it

Direct assertion, not inference. Break the entry state so only SETVID can put
the vector right — `s_cswl = 0x00` instead of the snapshot byte:

| | broken entry state |
| --- | --- |
| with the fix (writes `s_cswl`) | **6/6 cold checks pass** — SETVID repairs it |
| with the dead store (`ram_poke(0x36, …)`) | **0/6 pass** |

So the store is live now and demonstrably was not before. The same trick is the
general way to test a store nothing currently depends on: remove the other
source of the value and see whether the store can stand alone.

## 2026-08-24 (very last) — No decompiler-shaped code left, anywhere

`bb_N:` appears zero times in `snake-byte-cold.c`. The twelve ROM entry points
that were still switches over block ids are C: PLOT1, SETCOL, SCRN, GBASCALC,
PLOT, COUT1, HLINE/VLINEZ, HOME, the `$FC68` line-feed tail, SETKBD, SETVID and
COUTZ.

That the ROM was ever a separate question is the thing this session got wrong
and then fixed. The category did not exist; the work did.

### Labels are not always a failure

Four of these keep named labels rather than becoming nested loops, and that is
the right answer rather than an unfinished one.

`rom_hline` is two entry points sharing one body: HLINE walks columns to H2 and
falls out of the bottom of its loop into VLINEZ's, which walks rows to V2. They
are entered at different depths and leave through each other. `rom_coutz` is a
dispatcher whose arms rejoin at three depths — a wrapped line falls into the
carriage return, which falls into the line feed; a backspace off the left edge
falls into the cursor-up path. `rom_home` has a `goto` back to its own entry
that cannot execute, kept because the decompiler kept it and for the same
reason: nothing here does cross-instruction flag proofs.

Writing any of those as loops needs a flag the original does not have. `across`
/ `down` / `store` / `dispatch` / `bell` beat `bb_1` / `bb_3` / `bb_8` without
inventing structure. `game_cold_start` made this call first; it generalises.

### Coverage per routine, measured

The rule from the cold-gate episode — check that a scenario runs it *and* that
a sample point fires while it does — was applied to every conversion. Probing
entry addresses across both cold scenarios found five arms that never run:
`rom_clreol` (its caller is the scroll path, and nothing scrolls), `rom_wait`
(BELL1 only, and nothing emits Ctrl-G), and COUTZ's Ctrl-S handshake, bell and
backspace (the game prints only printable characters, returns and line feeds).

All five carry a comment saying so. None of them lost coverage in the
conversion — the generated versions were equally unreached — but an error in
any would not be caught, and that is worth more written down than inferred from
a block count.

## 2026-08-24 — Step 5 done, and step 6 rests on a false premise

`ret_addr` is gone from 36 functions, along with the `push16`/`pop16` pair
each carried. `push16` and `pop16` are unused in this build: there is no
emulated call stack any more.

Nothing was spent for it. `ram-cold.probe` has never hashed `$0100-$01FF` —
it dropped the stack when it was written, on the argument that comparing
scaffolding would go red on the first adapter deleted rather than on the first
bug. That call, made before any of this, is what made this step free.

The documented trap did not bite either. `game_print_inline_str` finds its
string by address, and that address has been an ordinary parameter since the
routine was converted; what it reads is the game's own image, not the stack.

### Step 6 is not what the plan says it is

The plan reads: "**Step 6 — `CYCLES`.** Last. It is what the trace oracle is
made of, so after this the screen is the only check left." That is half true and
the missing half changes the step.

`CYCLES` is also the **clock**. The macro sets `s_pc`, adds to `s_cycles`, and
calls `cycles_expired()` when the budget runs out — which is what advances the
host's frames. Delete the sites and time stops: `--frames=N` never terminates,
the delay loops cost nothing, and the speaker never clicks.

Measured, the 851 sites are two different populations:

| | count | what it is |
| --- | --- | --- |
| probing `CYCLES` / `GAME_CYCLES_*` | 130 (120 distinct) | the trace oracle *and* the clock |
| charge-only `CYCLES_EDGE` / `GAME_CYCLES` | 721 | the clock alone |

So the step decomposes:

- **6a, retiring the trace** — stop the 130 from probing. Cheap, reversible in
  principle, and it is the trade the plan describes. But on its own it spends
  the strongest oracle and improves no code: the sites stay, they just stop
  being watched.
- **6b, removing the clock** — delete the charges. This needs the artifact to
  pace itself some other way, which nothing has designed, and it puts the cold
  build beyond comparison with the booting one, because the gate's whole method
  is running the same emulated program twice.

**6b is what the artifact eventually wants and it cannot be verified by this
gate.** That is not an argument against doing it; it is an argument for doing it
when the artifact stops being checked by comparison, and not before.

### What that means for the adapters

Step 4's remaining adapters were thought to be blocked on step 6, because they
hold `CYCLES` sites and deleting one "silently" drops a comparison point. It is
not silent: `probe-acceptance.sh` pins the count at 120 and fails loudly until
the number is changed on purpose. The script already says so — "converting a
routine lowers this deliberately; anything else is a regression."

So they can go one at a time, each lowering the pinned count with a reason, and
the trace stays intact for everything else. Four of them are already charged
`CYCLES(addr, 0)`, so deleting those costs no cycles at all — only the site.
That is the route, and it does not need step 6 first.

---

## 2026-08-24 (after step 6's entry) — The game's state leaves RAM, and the flags get classified

**Status:** validated · **Scope:** apple2tc

The three things HANDOFF listed as untouched by the six cleanup steps — the
game's own variables, the plotting state, and the status flags — are done. The
counts (314 `ram_peek`/`ram_poke` down to 16, 222 flag references down to 100)
are the least interesting part. Four things are worth carrying to the next
binary.

### The order of a storage move is what makes the gate evidence

Established at step 3a and used unchanged for all four slices here. Move the
storage **with the probe untouched first**, and require the gate to read *trace
PASS, screen PASS, memory FAIL*. That failure is the only proof that the oracle
was watching those bytes; a narrowing applied first would have been
indistinguishable from a narrowing over bytes nothing ever wrote. Then narrow,
then mutate each new edge address and watch memory alone catch it.

Thirty-four edges were added across the four slices. All thirty-four were
mutation-tested and all thirty-four were caught. That number is the point: it
is cheap, it is mechanical, and it is what stops a probe from quietly becoming
a hash over a range nobody touches.

The `game` field is XOR-folded over its fourteen surviving pieces rather than
given a parameter each, because `PROBE_MAX_PARAMS` is 16. One wrong byte
changes exactly one sub-hash and nothing cancels it.

### A store becomes dead the moment its byte leaves RAM, and nothing tells you

Seven of them here: `$6637`/`$6638`, `$6C4A`, `$6B39`/`$6B3A` and
`$3754`/`$3757`. In every case the routine already computed the value in a
local and committed it through the struct or counter it was handed; the store
existed *only* because the memory oracle hashed the address. They read as
faithfulness and they were scaffolding.

`-Wall` cannot help. It says nothing about a dead `ram_poke`, and — the part
that is new — it says nothing about a **file-scope static** that is written and
never read either. `s_ink` was written at fourteen sites and read at none for
an entire build, with `-Wall`, `-Wextra` and all six cold checks green. The
existing note about `-Wunused-but-set-variable` covers locals only.

That is why threading and deleting were done as two commits: thread, gate,
*then* delete. If the deletion had gone in with the threading, a mistake in
either would have been attributed to the other.

### "In `rom_*`, registers are the algorithm" is a hypothesis, not a fact

It had been repeated across several documents as a reason not to touch the
flags. Checked, by classifying all 222 sites as reads or writes:

- `b` — 3 writes, **0 reads**. Never read anywhere. The PHP bytes use the
  `STATUS_B` constant, not the variable.
- `i` and `v` — 2 reads each, and both feed the PHP byte in `rom_plot` and
  `rom_scrn`, whose only reader is the PLP restoring `i` and `v` from bits 2
  and 6. A value pushed so that it can be popped back into itself is not a
  value. Bit 3 (`d`) and bit 0 (the LSR's carry) stay, because those are read.
- `n` and `not_z` — read in one routine each (plus `steer_try`), on paths a
  local write dominates. Locals now; 87 dead stores gone.
- `c` — **stays**, and this is measured: it is read on entry to `rom_home`
  (CLRSC2's first pass, before `$FC52` sets it) and to `rom_fc68`, so it
  genuinely crosses a call. **[Superseded 2026-08-25: this is wrong. Both
  reads are preceded by a call whose callee writes the carry. See the
  2026-08-25 entry.]**
- `d` — **stays**. Every BCD region and every `assert_binary_mode`.

So the claim held for two of seven and was false for five, covering 115 of the
222 references. **Do not extend it to `s_a`/`s_x`/`s_y` without running the same
classification** — that is the remainder of step 4, and it is the same shape of
work.

### Inheritance can be written down, or it can be a global — check which

`s_ink` and `s_shape` were the same kind of thing on paper: both inherited
across calls, both documented as globals for that reason. They are not the same
thing. Every reader of `s_ink` was one of two lines and every writer's value
was readable at its call site, so the run helpers take it as an argument and
`draw_border(0x0d)` states what it used to inherit. `s_shape` does not thread:
`plot_at`'s two callers erase with whatever shape was last used, and passing
one would invent a value the original does not have.

Both inferred inherited inks were mutation-tested rather than argued.
`draw_border`'s is caught by the play screen; the redefinition screen's stem is
caught by the hires screen **and by nothing else** — which is the scenario that
was added on 2026-08-23 for exactly this class of thing.

### What is not checked by running it, and says so

`rom_fc68`'s scroll and `rom_wait`'s countdown loops. Inverting either test
passes all six cold checks, because nothing scrolls and nothing emits Ctrl-G.
`$FC8C` and `$FC93` are already on `probe-acceptance.sh`'s unverified list.
Both localisations rest on the dominance argument and each records that above
itself. `steer_try`'s is checked: inverting it fails the play screen.

---

## 2026-08-25 — The carry was not live across calls, and the ROM bodies convert

**Status:** validated · **Scope:** apple2tc

Supersedes the carry finding in the 2026-08-24 entry, which is marked there.

### The correction

That entry recorded, as measured, that `C` is read on entry to `rom_home` and
`rom_fc68` and therefore genuinely crosses a call. It is not. Both reads sit
after a call -- `rom_clreolz` in HOME's CLRSC2 loop, `rom_vtabz` in the scroll
-- and both callees write the carry before returning. I read each routine's own
body and stopped there.

That is the *same* mistake the `rom_cout` parameter list had already caught,
three days of work earlier: reading a body instead of following the calls.
Knowing the failure mode did not prevent it. The lesson is not "be careful", it
is that **a liveness claim about a register must be produced by a tool, not by
reading** -- the read-before-write fixpoint over the call graph takes a dozen
lines and would have said so both times.

Consequence: all ten game-side carry writes were dead, and inverting every one
of them passes all six cold checks. Eight more inside `rom_*` were dead for the
ordinary reason -- the branch after them tests the value, not the flag. 36
writes are 12.

### What the carry actually is

Three edges, and they are a real 6502 idiom worth naming: a routine ends on a
`CMP`, and its caller reaches an `ADC` with no `CLC` in front of it, so the
compare's carry *is* the +1.

- `rom_clreolz` returns it; HOME steps a line with it and `$FC9A` branches on it.
- `rom_vtabz` returns it; the scroll at `$FC81` does the same.
- `rom_bascalc` hands back its second `ASL`'s carry, which VTABZ adds in.

They are return values now. A global flag was hiding a genuine data dependency
behind something that looked like residue -- which is precisely why the wrong
conclusion above was easy to reach and hard to notice.

### The ROM bodies convert once the entry points have signatures

144 register references inside `rom_*` down to 11, in two passes. Nothing
clever: give each routine parameters for what it reads, a return value for what
a caller reads, and turn the rest into locals.

Two things fell out that reading had not produced. SETIO's `X` and `Y` are the
built-in device's vector -- `$FD1B` for KEYIN, `$FDF0` for COUT1 -- and SETIO
keeps the low half while replacing the page; `rom_setkbd` and `rom_setvid` say
so now. And `rom_home`'s two `s_y = 0` were stating something the code can say
directly: CLREOLZ is the only thing that moves Y, so every pass blanks from
column 0.

### The floor, and what it is made of

15 register references remain. Eleven are COUT's promise to preserve X and Y:
the game repoints CSWL/CSWH at its own hi-res renderer, so the hook is entered
through `JMP ($36)` and **cannot** take arguments -- it reads its slot out of X
and restores the caller's X and Y on the way out. Three are the entry-state
load. That is not a tail to be chipped at; it is the machine's own dispatch
being the interface.

### Comments outlive the code they describe

The `\file` block heading the converted half still described the
`game.c`/`game_native.c` split -- adapters marshalling machine state, variables
"still living at their original addresses", accessors. Every clause of it had
been false since the merge, and it sat directly above the code that disproved
it. Worth a sweep whenever a file absorbs another one.
