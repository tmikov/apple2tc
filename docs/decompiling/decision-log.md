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
