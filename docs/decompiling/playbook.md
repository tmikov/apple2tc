# Playbook: converting apple2tc output into maintainable C

The transferable method for turning a decompilation into source worth
maintaining. The append-only rationale — including the wrong turns and when
each thing was learned — lives in `decision-log.md`. This file holds only what
transfers, stated as the conclusion rather than as the history of arriving at
it.

## When this applies

You have a working apple2tc decompilation — a binary, a `--run-data` JSON
trace, and `-O3 --irc1` output that builds and runs — and you want source worth
maintaining rather than emulator-shaped C.

If the decompilation does not yet run, this is the wrong document; get it
running first, because a runnable build is the oracle everything here depends
on.

## How to read this

**The seven steps are the spine.** They are in the order the work is done, and
the order turned out to be game-independent. Each step carries the same four
things:

- **What you do** — the actions.
- **What it costs** — which oracle this step spends, and what stops being
  checkable once it has. This is the through-line of the whole method: the
  oracles decide the conversion order, not taste.
- **What bites** — the findings, each with the evidence behind it.
- **Status** — whether it has been executed, and on what.

Two things sit outside the spine. **Techniques and idioms** holds the reusable
procedures and the 6502 patterns that apply at more than one step; every step
that needs one names it. **The hazard index** at the end is a lookup: one row
per failure mode, each naming the step where it bites.

**Scope tags:**

| tag | means |
| --- | --- |
| `[6502]` | true of 6502 / Apple II work generally |
| `[apple2tc]` | true of this decompiler and its tools |
| `[process]` | true of how the work is verified and sequenced |
| `[game]` | observed once, may not generalize |
| `[general]` | true of software work, not of decompilation especially |

**How a rule is written.** Three fields, in this order:

> **`[tag]` The rule, in one bolded sentence.** The evidence: what was measured,
> on what, and what it cost when it was got wrong. Then —
>
> **Check:** the thing you run or verify.

The first two were always here; the third was added on 2026-09-02, after a
review pointed out that the document was hard to use while working rather than
while reading. The checks were mostly present already and buried mid-paragraph
— only 17 of 107 rules ended on an imperative — so this surfaces them rather
than inventing them. `lint-playbook.sh --checks [step]` prints just the checks,
which is the pre-flight list for the step you are on:

```bash
docs/decompiling/lint-playbook.sh --checks 6.6   # the 8 checks for the clock
```

The lint fails if any rule lacks one. **Evidence is deliberately not labelled**
— a bolded rule followed by prose is already two fields, and naming the second
one would be ceremony.

**Checking this document.** `docs/decompiling/lint-playbook.sh` verifies the
tag vocabulary, the internal links and the hazard index's step references, and
prints where the index has fallen behind the rules. `docs/decompiling/
same-code.sh` is the codegen oracle the steps below refer to. Both exist
because this file asserted things about itself that had quietly stopped being
true.

**Maturity.** Every step has been executed once, on Snake Byte, between
2026-08-02 and 2026-08-27. The findings are measured rather than predicted, but
measured *once*, on one binary — a strong prior, not a law. The one thing a
second game should expect to differ on is **cost**: the order of the work was
game-independent and how much oracle each step spends was not.

A step whose "what bites" is thin is a step nobody has worked twice. That is
information, not an omission to be filled in.

---

# The procedure

## 1. Scope the boundary

*Status: executed.*

**What you do.** Count blocks by address range. Identify the ROM/library share
and the distinct entry points crossing into it. Measure the hardware surface.
Decide the boundary before writing anything.

**What it costs.** Nothing. This step only measures, which is why it is first —
every later decision about how much work there is depends on numbers that are
cheap now and awkward later.

### What bites

**`[apple2tc]` Entry-point choice dominates output size.** Snake Byte traces
from the reset vector `$FA62`, so the program boots ROM into BASIC and the
recorded keystrokes type `CALL 14160`. Result: 1,442 of 2,097 basic blocks in
the output are Applesoft and Monitor ROM, against 641 blocks of actual game.

**Check:** Count blocks inside the program's own address range against the
total, before planning any conversion work.

**`[6502]` The ROM boundary is far smaller than the ROM — but cutting it is
not.** Snake Byte pulls in ~1,430 ROM blocks and calls only **9 distinct entry
points**. Count the distinct `JSR`/`JMP` targets crossing into ROM first. But
do not assume that externalizing them removes the ROM: measured, it removed
**112 blocks**, not 1,430. Library code is typically reachable by two
independent routes — through its public entry points, *and* through whatever
the program's start PC leads to. Both must be severed, and they are strongly
non-additive (112 and 48 separately, 1,530 together). Retargeting the entry
point is the other half, and it is step 4b.

**Check:** Count the distinct `JSR`/`JMP` targets crossing the boundary; then
externalize and re-count blocks. The two numbers are unrelated.

**`[6502]` Measure the hardware surface early.** Snake Byte touches only
`$C000`/`$C061` (keyboard, joystick button), `$C010` (strobe), and
`$C050/$C052/$C055/$C057/$C053` (video mode), across a 20KB binary. A small
surface means most of the work is pure logic with no emulation dependency.

**Check:** Grep the disassembly for IO addresses and list the distinct ones.

**`[apple2tc]` Count instructions, not code-range markers.** `--asm` prints a
`Code range` marker only at a discontinuity, so the markers are a measure of
how *fragmented* the code is and not of how much there is. On Snake Byte they
covered 260 bytes against 3,931 bytes of real instructions. Anything you intend
to report as coverage has to be derived from the instruction stream.

---

**Check:** Derive coverage from the disassembled instructions, never from `Code
range` markers.

## 2. Recover procedures

*Status: executed.*

**What you do.** Run the routine classifier and read its rejections. Filter them
to the program's own address range first — most of the list is usually library
code that step 4 deletes wholesale. Fix decompiler over-strictness where it is
genuinely over-strict, then **re-measure**. Reduce the remainder to a named,
enumerable manual list, ranked by how many other routines each one blocks.

**What it costs.** Nothing yet, and this is the reason it comes before
structuring rather than after. It is also the last step at which the
decompiler's own output is the only thing you have to be right about.

### What bites

**`[6502]` Recover procedures before attempting any control-flow structuring.**
This is not a preference, it is a hard ordering constraint. Inside a fused
mega-function, `RTS` becomes a *computed* branch to every possible return site,
so the CFG is near-complete and cannot be structured. `block_id` *is* the
program counter, and a program whose PC is data cannot be relooped. Relooping
the mega-switch directly would succeed and produce the same dispatch loop in
structured syntax — correct and useless.

**Check:** Confirm the routine report accepts a routine before planning to
structure it.

**`[apple2tc]` `routines.cpp` used to balance the stack per basic block.**
Fixed 2026-08-02 in commit `ea29cdc`: `int stackLevel = 0;` sat inside the
per-block loop, so the universal `PHA`-at-entry / `PLA`-before-`RTS` idiom was
rejected as unbalanced. Depth is now propagated along CFG edges and required to
be zero only at `RTS`. Measured effect on Snake Byte: **53 → 75** routines
identified, and the generated C from 108 to 152 function lines.

**Check:** Count accepted routines before and after any `scanCandidate` change;
the delta is the measurement.

**`[apple2tc]` Recovering a ROM routine can unblock game routines.** The
predicted 7 game routines were *not* the ones recovered. Only 2 of them made
it; 6 unpredicted ones did, because ROM `$F800` (`PLOT`) and `$F871` (`SCRN`)
suffered the identical stack bug, and recovering them cascaded *forward* to
their game-range callers. Net was 8, not 7 — the count held for entirely the
wrong reasons. Estimate recovery by reasoning about the call graph, not by
counting direct failures.

**Check:** After a recovery fix, re-run the report and diff the accepted set.
Do not predict which routines it frees.

**`[apple2tc]` `--irc1 -v2` already reports why every candidate was rejected.**
Do not write a script to infer it. Reasons seen: `block $X stack level not
zero`, `Pop8 block $X stack level underflow`, `terminator JmpInd`. Since
2026-08-02 `--routines-report=<path>` emits the same information plus block
sets, call sites, dominator chains and natural loops.

**Check:** Read `--routines-report`. Do not write a script to infer rejection
reasons.

**`[apple2tc]` The `removeInvalidJSRs()` cascade fires only once candidates get
far enough to reach it.** On Snake Byte pre-fix it removed nothing — the
affected routines were already dying earlier in `scanCandidate`. Post-fix it
removes 5 game routines that now pass their own checks but `JSR` into
still-rejected ones. Fixing an early filter can therefore *expose* a cascade
that previously looked inert. Re-measure after every recovery change.

**Check:** Re-run the report after fixing an early filter, to see what the
cascade now removes.

**`[6502]` One unrecoverable routine can block many.** Snake Byte's `$7230`
inline-string printer is not merely 10 unrecovered call sites — it transitively
blocks 5 other game routines through the cascade above. When triaging which
rejections to fix first, weight them by how many callers they block, not by
their own call count. The idiom itself is in
[Techniques: the inline-string idiom](#the-inline-string-after-jsr-idiom).

**Check:** Weight rejections by how many callers they transitively block, not
by their own call count.

**`[apple2tc]` The routines report reflects a pre-`simplifyCFG` CFG.**
`identifySimpleRoutines` runs at `-O2`, `simplifyCFG` at `-O3`, so the report's
blocks are more granular than the final `--irc1` output and one source address
can appear as several blocks — e.g. `ADC` splits into binary-mode and
decimal-mode blocks sharing an address. Expect the report and the C to disagree
on block structure; that is not a bug.

**Check:** Expect the report's blocks to be more granular than the final C;
match on addresses, not on counts.

**`[process]` A relaxation that passes every oracle can still be wrong, and the
way to tell is structural.** Snake Byte's `$6A32` is rejected because `$6AB3`
is `PLA / PLA / JMP` — it discards the caller's frame and jumps into the main
loop. Switching off the two stack-depth checks that reject it gets the main
loop identified as a routine at last, and passes everything: four 1,300-frame
scenarios, three block-head traces, memory including the stack pointer, and the
screen.

It is still wrong. The routine's block set is computed by following successors,
so the unwind edge pulls the *caller's* loop into the callee: the emitted
`func_6a32` contains `$6291`, the main loop head, and calls `func_6a32`. Every
unwind would add a C frame that never returns. The reason nothing failed is
measurable — `$6A32` runs 143 times across the three recordings and `$6AB3`
runs zero — so the recursion is latent, and no recording anyone is likely to
make would reach it either.

Two checks would have caught it and neither is a test: does any generated
function now call itself, and does a routine's block set contain an address
belonging to its caller. Both are one grep. Run them on any change that makes
the decompiler accept something it used to reject.

The right shape turned out to be a calling convention rather than a relaxed
check: `--alt-exit` (`routines.cpp`, `CallAlt`/`ReturnAlt`) stops the walk *at*
the unwinding block instead of following it, the routine returns which exit it
took, and the caller switches on that. `func_6a32` is then 50 blocks spanning
`$6A32..$6AB5` and the three structural checks come back clean. The lesson
generalises past this one idiom — **when the classifier rejects something, the
question is what shape would let it be expressed, not which check to disable.**

---

**Check:** Check the *shape* of what a relaxation produces — a routine that
calls itself, a caller that vanished — not just whether the oracles pass.

## 3. Stand up the oracle

*Status: executed.*

**What you do.** Build a golden per-frame hash trace from the known-good build,
plus a script that replays and diffs. Do this *before* changing anything, so
the baseline is trustworthy. Prove it reproducible and prove it capable of
failing. Then measure what it actually covers, and write down what it cannot
see.

**What it costs.** Nothing, and it buys everything. This is the step the rest of
the method is denominated in: every later step is described by which oracle it
spends. Build the strongest thing you can afford here, because the strong ones
die first — see step 5.

This step is four things, and they are usually done in this order.

### 3a. Build it

**`[apple2tc]` A deterministic replay harness already exists.**
`lib/decapplib/decapplib.c` locks the run to exactly `1/60 * clock_freq` cycles
per frame whenever `--key-file` is supplied, so a cycle-stamped key recording
replays identically every time.

**Check:** Look for an existing deterministic replay path before building one.

**`[6502]` Hash video *memory*, not the rendered framebuffer.** Three payoffs,
all confirmed on Snake Byte. Rendering derives its blink phase from wall-clock
time and is therefore not reproducible. Memory hashing is strictly more
sensitive. And it needs no graphics context at all — which is what makes
headless replay possible, and headless is what let the oracle run in an
environment with no usable X display. Hash the mode byte, the mixed flag, the
text page and the hi-res page; include the cycle count per line so the trace
doubles as a timing check.

**Check:** Hash video memory. If a hash needs a graphics context, it cannot run
headless and cannot be a gate.

**`[6502]` The oracle only stays exact if timing does.** Visible behavior
depends on cycle counts through spin loops and delay loops, so approximate
timing lets the game drift and the trace diverges for reasons unrelated to
correctness. If you intend to verify this way, carry the exact per-block
`CYCLES()` totals through the restructuring — c1 supplies them, so this costs
bookkeeping, not analysis. Approximate cycle counts and a frame-hash oracle are
mutually exclusive; pick one.

**Check:** Carry exact per-block cycle totals through any restructuring, and
compare cycle counts alongside hashes.

**`[process]` A gate is a program, and it fails like one.** Two failure modes
cost real time here and both are script bugs rather than method bugs. A
`$(cmd | grep ...)` under `set -e` with `pipefail` exits 1 when there is no
match and kills the script with no message, so a check that legitimately finds
nothing needs `|| true`. And a verification script must reject arguments it
does not understand: `./verify.sh <build-dir>` read `$BIN` only and silently
tested a stale directory, producing four false PASSes.

Golden traces belong next to the game, in `decoded/<game>/`. `tests/` is
decompiler regression — hand-written `.s` assembled by `a6502`, decompiled,
diffed against `.ir` baselines.

**Check:** Run the gate against a knowingly-broken build before trusting a
green one, and add `|| true` to any `$(… | grep …)` that may legitimately match
nothing.

### 3b. Prove it can fail

**`[6502]` Prove the oracle is reproducible before recording it.** Have the
record path run the known-good build *twice* and refuse to write the trace
unless both runs agree. An oracle nobody has shown to be deterministic is worse
than none: it produces failures that look like real bugs. On Snake Byte this
passed first time, but the cost of checking is one extra run.

**Check:** Have the record path run the known-good build twice and refuse to
write unless both agree.

**`[6502]` Prove the oracle can fail, too.** Corrupt one hash in the golden
trace and confirm the checker reports failure and exits non-zero. A check that
cannot fail is not a check.

**Check:** Corrupt one entry of the golden trace and confirm the checker exits
non-zero.

**`[process]` A check that no test can make fail is not a check, and this
applies to the decompiler's own conditions.** Six conditions went into
`--alt-exit`, and mutation-testing each one — delete it, confirm a test fails —
found three that nothing covered. One was genuinely unreachable and became an
assertion. The other two were real holes: the fixture called the alt-exit
routine from top-level code, so nothing ever *cloned* the `CallAlt`, which is
where a release build was silently dropping its alternate targets; and nothing
reached a block that only exists as a callee's alternate exit. Both needed the
fixture widened, not the code changed. Do this before the commit, not after:
the two holes were in the parts of the design that felt most obviously correct.

**Check:** Mutation-test every condition, including the decompiler's own:
delete it and confirm a test fails.

**`[process]` Assert the specific message, not a substring that anything can
satisfy.** Twice during the probe work a rejection test passed while covering
nothing, because a *different* check fired first and satisfied a grep for
`FATAL`. Assert the message the check under test emits, and prove the test can
fail by deleting that check and watching the suite go red.

**Check:** Assert the exact message the check under test emits, then delete
that check and watch the suite go red.

**`[process]` A red result is only evidence once you have seen it turn green for
the right reason.** A drain-guard regression test here specified `--frames=10`.
At that budget the buggy and the fixed build produce byte-identical output —
the installed keyboard site is not reached until roughly frame 8.3, and the one
key that would distinguish them is stamped for a point ~59 frames further out —
so the test failed identically before and after the fix and proved nothing
either way. It needed `--frames=100`.

**Check:** Before believing a red result, make it go green by fixing the cause
— not by changing the budget.

### 3c. Measure what it covers

**`[6502]` Measure the trace's coverage; do not assume it.** Distinct-hash
count is dominated by static stretches, not gameplay. Snake Byte's 1,300-frame
trace has only **191** distinct hashes: frames 0–168 are keystrokes at the
BASIC prompt, 177–278 and 371–472 are identical 101-frame title/attract
plateaus, and continuous gameplay only runs from ~472 to the end — about 828
frames, 14 seconds. Correlate hash-run boundaries against the key file's cycle
stamps to find where gameplay actually starts, and check the tail is still
changing: a long frozen tail means a crash or an early-exhausted key file.
Expecting "several hundred" of anything is how this goes wrong — static
stretches dominate replay traces, so measure the number and explain it if it
surprises you.

**Check:** Count *distinct* states in the trace, and say which stretches are
static.

**`[process]` Measure what the oracle covers, per site, or it will flatter
you.** Snake Byte's cross-engine gate compares ~2.7 million block-head hits
between the interpreter and the generated build, and passes. It reaches **744
of 1,669 block heads — 45%**. Everything else is compared against nothing, and
a mutation there passes every check. The "a probe never fired" guard does not
catch this: it is per *probe*, and the trace is one probe installed at every
address, so it fires constantly while individual sites stay dead. Derive
coverage by intersecting the site list with the addresses the trace actually
emitted, and assert a baseline for the hand-written subset specifically — those
are the blocks where a decode error has no other net under it.

**Check:** Report what fraction of sites the comparison reaches, alongside the
hit count.

**`[process]` A big number is not coverage until you know its distribution.** A
rewrite here cited "20,298,539 block heads match". Of one scenario's 260,128
hits, 252,364 were a single address, and the address most relevant to the
change fired zero times. Before quoting a count as evidence, sort it by what
produced it. A number that large is *reassuring* in a way that is very hard to
argue with, which is exactly why it should be checked.

**Check:** Sort the hits by address before quoting a total, and name what the
top one is.

**`[process]` Block coverage is not value coverage.** Every block of Snake
Byte's `game_add_score` executes, and replacing its BCD addition with binary
addition on the score's *second* byte fails nothing at all: the score never
reaches 100 in either recording, so that byte never needs decimal correction.
The low byte's equivalent mutation is caught, and only by one of the two
scenarios. Full block coverage of an arithmetic routine says nothing about the
values that flowed through it.

**Check:** Mutate a value the blocks compute, not just the path through them,
and see whether anything notices.

**`[process]` A block that runs is not a behaviour that was tested.** Snake
Byte's key table maps a *binding* to a *command* through two parallel arrays,
and both blocks that do the mapping execute constantly. But the recording
presses the default bindings, where the two arrays are byte-identical, so
reading the wrong one changes nothing. The rebinding scenario does rebind — and
then stops, never playing afterwards. Swapping the two arrays in the converted
C passes every check. What a routine *distinguishes* has to be exercised, not
just entered, and no coverage number will say which recordings are missing. The
same shape hides whole features: a mute or config byte read in a hot path can
have its recorded value hardcoded and nothing notices.

**Check:** Check that a scenario exercises the *difference* between the arrays,
not just the code that reads them.

**`[process]` A coverage number that can fall for two opposite reasons needs
saying so.** Snake Byte's unverified-block baseline dropped 60 → 47 without a
single one becoming verified: they left the site list when their routines
converted. Unexercised code that stops being probed stops being counted. Pin
the site count exactly alongside it — that number only moves deliberately, so
it is the honest measure of progress.

**Check:** Pin the site count so it cannot fall silently, and re-derive the
baseline when it does change.

**`[process]` Coverage clusters by feature, so report it that way.** Snake
Byte's 60 unverified hand-written blocks are not 60 scattered branches; they
are the joystick (15), one unused display-list opcode (7), ROM paths for
arguments the game never passes (20), pause and mute (6), and five smaller
groups. Grouped like that the list is a description of what the recordings do
not do, which is actionable. Listed as addresses it is noise nobody reads.

**Check:** Group the uncovered blocks by feature and report the groups.

**`[process]` "Absent from the trace" and "unreachable" are different claims.**
Only the second is worth relying on, and it takes an argument rather than an
absence of evidence. Branch-target lists are capped — apple2tc's at 500 entries
— and record only *targets*, so fall-through blocks read as absent either way.
Instrument and count.

**Check:** Treat absence from a trace as a question. Only 'unreachable' is
worth relying on, and it takes an argument.

**`[process]` Derive the set of sites; a list that was grepped once is a
starting point.** Snake Byte's coordinate plan named three keyboard-read sites
(`$FD1B`, `$741F`, `$7890`). A fourth, `$6217` — the in-game ingest that clears
the strobe and fills the ring buffer — was missing. Recording with only the
three captured 11 of 23 keys, and not 11 mis-timed ones: once a script delivers
via `key` at all, the host's per-frame drain stands down entirely, so an
uncovered site's keys are never delivered. Found by recording and counting, not
by reading the disassembly harder.

**Check:** Record and count what actually fires. Do not read the disassembly
harder.

### 3d. What it cannot see

This is the part worth writing down, because a green gate reads identically
whether or not it was looking.

**`[process]` A green gate is a claim about the gate, not about the code.**
Before citing one, ask what it would take for it to be green and the code
wrong — then build that and check. See
[Techniques: calibrate by mutating](#calibrate-the-oracle-by-mutating-what-you-just-changed);
three of four plausible breakages of a rewritten function survived the gate
that had just approved it.

**Check:** Build the thing that would be green-and-wrong, and check whether the
gate catches it.

**`[process]` A set of oracles that all agree may share one assumption.** Ask
what they all *assume*, not what each one covers. This repo had four
independent-looking checks and every one was fixed-step, because reproducibility
is what each exists for — so the wall-clock path a person actually runs was
covered by nothing, and a total freeze went unnoticed for nine days. The blind
spot is in the union, not in any member.

**Check:** Ask what every oracle *assumes*, not what each covers. The blind
spot is in the union.

**`[process]` The frame oracle and the memory probe each catch what the other
misses, in both directions.** Snake Byte's memory check caught a byte written
at reset and never read, which moved no frame hash. The frame oracle caught a
wrong glyph on a menu screen, which the memory check could not see because it
samples at an in-game address that does not fire there. Neither is redundant
and neither is a superset — pick sample points that cover the phases you care
about, or accept that whole screens are checked by only one of the two.

**Check:** Keep both, and pick sample points covering the phases you care about
— or state which screens only one of the two checks.

**`[process]` Build the last oracle while the first ones can still check it.**
The screen-state check was added while the trace and memory checks were intact,
so it could be *shown* to catch a defect they caught — a display-list operand
swap, failing at sample 7,503 of 26,111. Added after they were gone, it would
have been an assumption.

**Check:** Add a new oracle while the existing ones still work, and show it
catching something they caught.

**`[process]` An oracle built to ignore cycles cannot check cycles, and the one
that checks them may not run the code.** Snake Byte has two cross-engine gates
and neither covers `$69A9`'s cycle charges. `verify.sh` compares per-frame
cycle counts, but its scenarios never press an unrecognised key, so they never
enter the routine. `probe-acceptance.sh` does enter it and stamps its input on
a probe counter rather than on cycles, deliberately, so that the two engines'
cycle phase cannot perturb replay — being cycle-independent is exactly what
makes it useless here. Measured: 4 cycles written as 5 passes everything;
written as 4,000 it fails on frame-boundary drift. **Read a routine's coverage
per property, not as one number** — the same "84/104 hand-written blocks run"
reports the checked control flow and the unchecked timing alike.

**Check:** Report coverage per property — control flow, timing, state — never
as one number.

**`[process]` A scenario that never presses the key covers nothing behind it.**
Snake Byte's cold gate ran one scenario, which never pressed `C`, so an entire
routine ran **zero** times under it and a mutation there was caught by nothing.
Adding a scenario was half the fix; the other half was that both state probes
sampled at an address that path never reaches, so the new scenario bought trace
coverage and *no state coverage at all*. **Check that a sample point fires
while the new code runs.** See
[Techniques: deriving a scenario](#deriving-a-scenario-instead-of-recording-one).

**Check:** Confirm a scenario runs the routine *and* that a sample point fires
while it does. Count both.

**`[process]` A fixture built to reach new code also deepens the code you
already had.** Snake Byte's `easy` build exists to make the display list
reachable — the apple quota lowered so levels change. It turns out to be the
only oracle that checks the score's BCD carry: breaking the propagation between
score bytes passes `verify.sh` 4/4, both block-head traces, memory and screen,
and fails only the 3,000-frame run against the fixture. A fixture that plays
*longer* is worth as much as one that plays *elsewhere*, and the second benefit
is the one nobody plans for.

**Check:** When adding a fixture for new code, re-measure what it covers in the
old code too.

**`[process]` A changed trace only proves something changed.** Pressing a key at
nine different cycles changed the trace every time while a call counter showed
the target routine ran **zero** times. Instrument and count before reading a
diff as "the code I wanted ran".

**Check:** Instrument and count before reading a changed trace as 'my code
ran'.

**`[6502]` Two frame-hash traces that disagree do not mean one engine is
wrong.** Not between *different* engines. Frame hashing presumes a shared
sampling clock; a generated engine yields only at block boundaries and an
interpreter between instructions, so two perfectly equivalent implementations
still disagree in 1–3 frame bursts around transitions. Same-engine comparison
is fine and is what a golden-trace script does. Across engines, compare at
instants the *program* defines.

**Check:** Compare frame hashes only within one engine. Across engines, compare
at program-defined points.

**`[apple2tc]` Register traces are not comparable across engines at all.**
`CPURegLiveness` and `dce` drop stores to dead registers by design, so
generated code does not maintain `Y` or the flags where nothing reads them.
Traces diverge on line 2. `--compat` makes the format diffable, not the
content.

**Check:** Do not compare register traces across engines at all.

**`[apple2tc]` A probe that produces no output says nothing about agreement.**
Cycle charges are emitted per basic block, so a probe installed at a
non-block-head address fires under an interpreter and *does not exist* in a
generated program — and the report still reads as agreement. Cross-engine
comparison must install from a block-head list, grepped out of the generated C.

**Check:** Install probes from a block-head list grepped out of the generated
code, and check the hit count is non-zero.

**`[process]` A green gate proves the code you just wrote only if the binary was
rebuilt.** Twice in one day `probe-acceptance.sh` reported six green checks
against a stale executable — once because ninja saw no work for a file it had
just been handed (same-second mtime), once because the build had failed and the
old binary was still there. Make the gate refuse to run if any program is older
than its sources. Before that check existed, the only defence was reading the
`Linking` line above the result. Relatedly, the artifacts these oracles leave in
`/tmp` are only as fresh as the last run that reached them: a suite that aborts
early leaves the *previous* run's files in place, and reading them cost an hour
chasing a routine the stale trace said never executed and the current one says
runs 110 times.

**Check:** Fail the gate if any binary is older than its sources.

**`[process]` An ad-hoc run against a recorded input file needs whatever the
replay needs.** Snake Byte's `.pkeys` stamps are values of a counter the *probe
script* defines, so `--key-file` without `--probe` silently ignores every
stamp: keys arrive at the wrong moments, the program takes a different path,
and the numbers you collect look entirely plausible. Two "findings" were
produced this way in one afternoon, both contradicting documentation that was
correct. Copy the gate's invocation rather than composing your own.

**Check:** Give an ad-hoc run every flag the replay needs, and confirm the
input actually landed.

**`[process]` Claiming a build is playable from headless evidence.** Launch it.
"Never actually launched" stated as a caveat is not a caveat, it is the
finding. Every check can be green against a binary no one has watched run — and
when it is finally run, separate the bug from the expected: a cold-start build
legitimately opens on whatever text page its entry snapshot captured, so "it
shows the BASIC screen and hangs" was one true observation and one real fault
welded together.

---

**Check:** Launch it and play it. Headless green is not playable.

## 4. Cut the library boundary

*Status: executed, both halves.*

**What you do.** Two separate steps, in this order. Doing both at once forfeits
verification for the duration, which is the entire reason this is two steps and
not one.

### 4a. Externalize the entry points

**What you do.** Map the ROM entry points the program calls to C function names,
supply them by hand, and keep the original boot path so the existing golden
trace still verifies every change.

**What it costs.** Nothing — the boot path is intact, so every oracle still
applies. This is the last step at which that is true for free.

**`[apple2tc]` Externalizing routines is a decompiler capability, not a
hand-editing chore.** `--extern-routines=<file>` maps addresses to C function
names; calls become calls to bodyless `Function`s and unreachable blocks are
deleted. Two payoffs beyond the block count: an unrecoverable callee poisons
its callers, so externing one can unblock procedure recovery elsewhere
(`$FDED`'s `JMP (CSWL)` was the sole reason `$71F3`, the game's most-called
routine, stayed unrecovered); and the hand-written replacements are verifiable
against an existing trace.

**Check:** Use `--extern-routines` rather than editing output by hand.

**`[6502]` Externalizing a routine deletes everything only it reached.** Cutting
Snake Byte's 9 also removed `GBASCALC`, `PLOT1` and the whole `$FB78` `COUTZ`
subtree, which the hand-written file then had to supply. Correct behaviour, but
it multiplies the hand-written surface over the naive estimate — size the work
by the reachable subtree, not by the entry-point count.

**Check:** After externalizing, list what disappeared — the hand-written side
has to supply all of it.

**`[6502]` Keep the original boot path while swapping leaf routines.** Changing
the entry point at the same time forfeits the ability to verify: a cold start
skips whatever boot frames the golden trace opens with, so nothing matches.

**Check:** Change leaf routines or the entry point, never both in one step.

**`[apple2tc]` An `RTS` keeps alive everything the recording ever saw it return
to.** Externalizing a routine erases the `JSR`s that called it — and with them
the only instructions that pushed those return addresses. The edges remain,
because an `RTS` carries its whole observed return set as successors, and each
surviving edge keeps its target reachable and everything reachable from the
target with it.

Snake Byte's inline-string printer at `$7230` survived externalization
completely for this reason, and it took two wrong diagnoses to see why. `$7239`
is where its own `JSR $FC68` came back to; `$FC68` is external, so nothing
pushes `$7239` any more, yet a ROM `RTS` still listed it. That one edge held
the printer's body, its `RTS`, and through that `RTS` the ten blocks its ten
callers resume at — which presented as "the inline-string idiom leaves orphans"
and is nothing of the kind.

`--prune-returns` drops them: the fall block of every erased `JSR` is a
candidate, and any `RTS` still naming one loses it. On Snake Byte, 64 edges and
298 further blocks. Reach for this before anything cleverer because it checks
itself — deleting unreachable text cannot move a frame hash, a memory sample or
a trace, so every oracle must stay exactly where it was.

**Check:** Prune return edges no surviving call can produce, and count the
blocks that go with them.

**`[apple2tc]` The compiler will not delete unreachable blocks for you.**
`dce()` removes only instructions, and every `Void`-typed instruction counts as
having side effects. A pass that orphans blocks must delete them itself.

**Check:** Delete orphaned blocks in the pass that orphans them; `dce()` will
not.

### 4b. Retarget the entry point

**What you do.** Snapshot the machine state at the program's real entry address
from the *booting build's own arrival* there, start the new build from that,
and compare the two directly.

**What it costs.** The golden trace's boot frames, which no longer exist. It
does not cost the comparison — and expecting it to is what made this step look
expensive for months.

**The re-based trace is not needed.** The prediction was that this requires "an
entry-state snapshot and a re-based trace". The snapshot half was right. The
*existing build is the oracle*: snapshot it at the target address, start the new
build from that, and compare block-head trace and screen at program-defined
instants. No golden file is re-recorded, and the comparison is stronger than a
golden trace because it is differential.

Do it in two commits even so: retarget first and delete nothing, so the entry
change can be shown to be behaviour-preserving on its own; then delete what is
unreachable. Snake Byte's first commit changed one initializer and installed
2KB of state; the second removed 1,686 of 1,775 dispatch cases. Reachability
comes from [walking the IR](#for-reachability-walk-the-ir-not-the-emitted-c),
not from the emitted C.

**`[process]` Dump the entry state; do not reason about it.** The state a boot
would have produced is small and knowable — for the Apple II it is
`$0000-$0802`, the registers, and one video soft-switch byte. Everything else
is the program image, the ROM, and uninitialised RAM reading as `$FF`.
Reasoning about which zero-page locations matter is slower and less reliable
than adding a capture-at-PC option and diffing two images.

Three traps inside that, each of which cost a debugging cycle:

- **The pristine boot state is the wrong state.** The machine at the BASIC
  prompt with nothing typed is not the machine at the program's entry: getting
  there means typing a command, and that leaves the parse state, the stack
  frames, the input buffer and the echoed text behind — 74 bytes for Snake
  Byte. Capture from the booting build's own arrival at the target address.
- **The host injects state the boot used to hide.** `a2host` pushes a dummy
  keystroke whenever a key source is present, because the first key before
  initialisation is otherwise lost. A booting program never notices; the ROM
  swallows it at the prompt. A cold-started one reads it as the player's first
  keypress. Anything the *host* does at startup on a program's behalf is part
  of the state being replaced.
- **Two boots of the same build differ.** `$4E/$4F` (RNDL/RNDH) count how long
  KEYIN spun, so they depend on when keys arrived. Harmless here — only the ROM
  reads them — but it means an entry state is not quite a constant, and a
  byte-exact diff will show it.

Capture the soft switches too. They are the half of the machine a RAM image
cannot carry, being host IO state rather than memory, and a rebased entry has
to set them explicitly.

---

**Check:** Capture the entry state with a snapshot flag; do not reason about
what a boot leaves.

## 5. Structural conversion

*Status: executed.*

**What you do.** Recover the remaining procedures by hand, rewriting
idiom-based ones. Reloop each function — dominator tree, back edges for natural
loops, then iterative region matching. Keep address-derived names throughout;
naming is step 6.1 and doing both at once means every failure has two candidate
causes.

**What it costs.** The block-head trace, per routine, the moment a branch moves
into real C. This is the step that spends the strongest oracle you have, and it
spends it incrementally, which is why the order within it matters.

**`[process]` The oracles decide the conversion order, not taste.** Three checks
watch a decompilation and they do not survive equally. Frame and screen hashes
survive to the end — they look at video memory, which is the machine's no
matter who wrote it. Memory hashes survive until a variable's *storage* leaves
emulated RAM, because until then an adapter can write everything back before
returning. The block-head trace dies first, per routine, the moment a branch
moves into real C. So a single-block routine converts for free and a 62-block
one costs 61 sites; sort by payoff per site and the order writes itself.

**Check:** Order conversions by which oracle each spends, and retire each one
deliberately.

### What bites

**`[6502]` Timing is not part of what you are allowed to drop.** A frame is a
cycle budget, so converted code that runs at a different speed moves every later
frame boundary and every later frame hash. Charge the original's cycles with a
primitive that does *not* register a probe site, and give up observability
rather than fidelity. Measured: folding three `CMP` blocks into a helper lost 4
cycles three times and diverged every oracle at once.

**Check:** Charge the original's cycles from a primitive that does not also
trace.

**`[6502]` Which side of a branch pays the extra cycle depends on the branch.**
A taken branch costs one more than an untaken one, so a converted routine has
to charge that cycle on the same side the original does — and *which* side that
is flips with the condition. `$6A32` tries eight directions: the first test is a
`BNE` to the retry path, so its edge belongs to the refusal; the other seven are
`BEQ`s to the accept path, so theirs belong to the acceptance. Writing all eight
the same way was one decision and seven bugs. Read the polarity off the
generated C rather than the mnemonic — it says `if (cond) goto bb_N;` with the
edge charge sitting in whichever successor block pays it.

**Check:** Check which side of each branch the original charges; it flips with
the condition.

**`[apple2tc]` Not every cycle-charging call site is a program location.** The
taken-branch penalty is charged on the *edge*, in a block carrying the branch's
address that the program is never actually *at* — so it must not trace or
dispatch probes, or one execution of that branch gets reported twice. Hence the
separate edge spelling. On Snake Byte 698 ROM addresses are edges and 121 of
them are also real block heads, so the two are not distinguishable by address.

**Check:** Charge edge penalties from a spelling that cannot trace or dispatch
a probe.

**`[apple2tc]` An adapter keeps its entry probe site for nothing.** A
`CYCLES(addr, 0)` still sets the PC and dispatches; charge the block's real
cycles inside the converted function. Every conversion then costs one site
fewer than its block count.

**Check:** Charge the block's real cycles inside the converted function, not in
a zero-cycle adapter site.

**`[apple2tc]` Merging an adapter beats deleting one.** An adapter exists to
marshal machine state for a caller that no longer exists, and deleting it costs
the block-head trace the adapter's site — a real comparison point, gone. Merging
it into the routine it describes costs nothing: the charge, the site and the
write-back all move inside, the site is still emitted and still probed, and the
pinned count does not change. Forty-two adapters went this way with the pin
fixed and the trace hit counts identical to the digit. The question an adapter
poses is not "can I afford to drop this site" but "where does this write-back
belong", and the answer is nearly always "inside the routine it describes".

**Check:** Merge an adapter into the routine it describes rather than deleting
it; the site moves instead of vanishing.

**`[apple2tc]` Call the adapter the generated code called, not the native behind
it.** Three of Snake Byte's eleven do real work: one charges 6 cycles and calls
a loader, one carries the entry probe, and one charges 6 cycles *and steps the
bouncers*. Skipping that last one was a wrong screen and a 6-cycle offset from
a single substitution, and the two symptoms looked like two bugs. Whatever the
generated C called at that site, call that, with the same return address.

**Check:** Read each adapter's body before assuming it marshals. Some do real
work.

**`[apple2tc]` Converting a routine can drag its callee in behind it.**
`$6A32`'s only call site is `$630D`, which is inside `$6288`. Externalize `$6288` and `$6A32` has
no generated caller left, so it is removed and the link fails asking for it by
name. The two are one conversion whether or not you planned them that way.
Check before starting: `--routines-report` prints each routine's call sites,
and a callee whose sites are all inside the routine you are converting is
coming with you.

**Check:** Externalize a routine and its now-callerless callees in one move.

**`[apple2tc]` A block head shared by two paths must convert in one move.**
Snake Byte's `$6216` is the `RTS` both the key dequeue and the keyboard poll end
on. Converting only the dequeue left the address in the site list — the
unconverted routine still emits it — while the generated build stopped firing
it there. The site count was still correct; only the trace comparison caught it.

**Check:** Convert every path through a shared block head in one commit.

**`[apple2tc]` A hand-written cycle site must carry a literal address.** Site
lists are built by grepping the C for `CYCLES(0x`, so an address arriving as
any other expression — a table lookup, a variable — still compiles, still counts
cycles, still runs, and never reaches the list. The probe is not installed,
neither engine reports it, and the diff is clean because both sides say nothing.
Measured: one routine written as a loop over a table of addresses took eight
sites out of the gate silently, and the site-count floor was far too coarse to
notice 1,669 become 1,661. Lint for it.

**Check:** Lint that every hand-written cycle site carries a literal address.

**`[apple2tc]` The literal-address lint does not cover the file that needs it
most.** Snake Byte's `check_literal_sites()` runs on the hand-written ROM and adapter
files. It does
*not* run on the file where converted code lives and where every new charge is
written, so a helper taking the address as a parameter is invisible there —
converting `$72CE` hid nine addresses that way. Extending the lint fails
immediately: fourteen sites in that file are table-driven today, ten of them
predating the routine that found this. Until those are written out longhand the
check cannot be turned on, so write new conversions with literal addresses and
do not introduce more. Nothing is silently *wrong* — the trace comparison still
catches a missing probe, but as a several-hundred-thousand-line diff rather
than one line naming the address.

**Check:** Point the literal-address lint at the file where new charges are
actually written.

**`[process]` A few addresses are load-bearing for *input*, not just for
comparison.** Probe-stamped replay counts hits at named addresses and stamps
keystrokes on that counter. Some of them sit inside game routines. Converting
one — charging its cycles but dropping its probe — stops the counter advancing
there, and every key recorded after that point arrives at a different instant.
It is caught, because only the generated side drifts, but caught as a
640,983-line block-head diff naming four addresses in an unrelated screen. Give
those sites their own spelling and assert that the set of addresses using it
equals the coordinate's.

**Check:** Keep the probe on any address the replay coordinate counts, and
re-check the stamp after converting it.

**`[apple2tc]` `--ret-addr` is on for a correctness reason, not a cosmetic
one.** Without it a `JSR` pushes a sentinel, and the inline-data-after-`JSR`
idiom finds its data by *reading* that address. It costs nothing — same push,
different compile-time constant. But it is **not** for a shipped artifact: that
wants no emulated stack at all.

**Check:** Turn `--ret-addr` on wherever the inline-data idiom is used; turn it
off for a shipped artifact.

**`[apple2tc]` Set the flags the 6502 sets, not the ones the generated code
keeps.** apple2tc's output is whole-program DCE'd, so a flag missing from a
generated routine is a proof that *the current call graph* never reads it — not
that the hardware does not set it. Dropping `$6148`'s carry to match passes
every check today. Re-deriving that proof on every change costs more than the
assignment, and the assignment is never wrong.

This sits against
[the liveness fixpoint](#a-liveness-claim-must-come-from-the-fixpoint-not-from-reading),
and the resolution is cost. A status flag is one line and always right, so
write it. A *register* write inside real C costs the abstraction the conversion
exists to buy, so check whether anything reads it — and the check is a command,
re-runnable whenever the decompilation changes.

**Check:** Set the flags the hardware sets. A flag missing from generated code
is a claim about the current call graph only.

**`[6502]` An instruction that looks like a typo is still load-bearing until you
prove otherwise.** Snake Byte's two cell plotters build the same table index
from the same operands, one with `ROL $06` and one with `ROR $06`; the `ROR`
puts the scanline parity in bit 7 where the following two `ASL`s discard it. It
reads exactly like a slip. "Correcting" it to `ROL` changes the screen and
fails the frame oracle. Reproduce what the bytes say, note the oddity, and let
a mutation test decide whether it matters.

**Check:** Assume an odd-looking instruction is load-bearing until a mutation
proves otherwise.

**`[process]` Memory agreeing before the cycles do tells you which bug is
left.** The memory probe compares every byte the routine writes; the frame
oracle compares when it finished. When the first passes and the second does
not, the logic is right and what remains is cycle accounting — so stop reading
the algorithm and start reading branch edges. That ordering turned the last four
bugs from a search into a checklist, and it is worth running the memory oracle
alone, early, for exactly that reason.

**Check:** When memory passes and cycles fail, stop looking for a logic bug.

**`[process]` "Convert it when a recording reaches it" is a hold, not a plan.**
*(This entry used to say the opposite. The reasoning it was built on was not
the part that was wrong.)*

The original rule: a conversion is checked by the recordings executing it and
the oracles agreeing; for a routine nothing executes there is no check at all,
and converting it also takes its addresses off the site list, removing the
unverified-block accounting that was the only remaining statement about it.

Both halves are true and it was still the wrong call, because it priced the
waiting at zero. No recording was going to arrive: making one is a deliberate
act nobody had a reason to perform, so "later" meant never. Against that, what
deferral buys is small. The decode is checkable against the binary whether or
not anything runs it — `id` disassembles it, the opcode timings give the cycle
charges, `--ir` gives the liveness — and that is the same evidence the
conversion of any *reached* routine rests on for the branches its recording
happens not to take.

So convert it, and pay the two real costs deliberately. **Name the unexercised
blocks in a comment above the routine**, which is the only surviving record once
they leave the site list. And do not describe the result as verified.

**Check:** Decode unexercised code from the binary, name the unrun blocks in a
comment, and do not call it verified.

**`[process]` The artifact should not share files with its own scaffolding.**
Snake Byte's converted game lived in files three builds included: the cold-start
artifact, the booting reference it is checked against, and a fixture. Every
change then had to stay safe for two builds still running a generated dispatch
over the same addresses. It first bit when a converted routine and the
generated dispatch both claimed the same block head and the site-list lint
refused them; the workaround was a separate file, which is the same problem one
step later. The resolution is that the artifact gets its own copy of everything
and shares nothing. Do it as soon as the artifact exists, not after the third
workaround — the packaging consequences are step 7.

**Check:** Give the artifact its own copy of every file before restructuring
it.

### Reporting it

**`[process]` The measures that make a conversion look finished are mostly
measures of the wrong thing.** Four used here, and what each is worth:

| Measure | Worth |
| --- | --- |
| `bb_N:` labels remaining | real — it is the decompiler's shape, and zero means every routine has been read |
| `ram_peek(0x...)` with a hex literal | **almost nothing** — it goes to zero the moment addresses get names, and `ram_peek(kApplesLeft)` is the same emulated-RAM access |
| adapters remaining | real, but only if they are *merged* rather than deleted |
| cycle sites remaining | depends entirely on which kind, and the two kinds do different jobs (see 6.6) |

Report the count that would embarrass you, not the one that reads well. "Zero
`ram_peek(0x...)`" was stated here while 314 `ram_peek(kFoo)` calls remained;
the number was true and the impression it gave was false.

**The order that actually matters** is not the order the plan had. Naming
addresses, giving routines parameters and deleting adapters are all worth
doing, and none of them moves the game's own state out of the emulated machine.
That is the thing a reader notices, it is bigger than everything else combined,
and it is easy to leave until last by accident because every other step produces
a satisfying number. It is step 6.

---

**Check:** Pick measures that a rename or a different spelling cannot satisfy.

## 6. Get the machine out of the code

*Status: executed 2026-08-24/25.*

**What you do.** The machine shows up in six disguises, and they come off in
this order because each one makes the next legible: **names**, **storage**,
**flags and registers**, **lookup tables**, **byte pairs**, and last **the
clock**.

**What it costs.** Progressively: the memory oracle narrows at every storage
slice, and by the end the screen is most of what is left. Each sub-step below
says what it spends.

Snake Byte ended with none of the six: no register, no flag, no emulated stack,
no address standing in for a variable, no split pointer, and 23 of 838 cycle
charges. That is worth stating because the estimate along the way was always "a
handful of these are irreducible", and it was wrong every time.

**`[process]` There was no floor, and predicting one was the mistake.** This
entry used to say the opposite: that Snake Byte's last fifteen register
references were irreducible, because the game repoints the ROM's output vector
at its own renderer, so the hook is entered through `JMP ($36)` and cannot take
arguments. Every clause of that is true and the conclusion was still wrong.

Y was saved and restored by two routines and read for a value by none — a
promise with nobody to keep it to. X was the slot the *caller* had just put
there: the ROM's COUT1 never touches X, and the hook saves and restores it
without looking, so "whatever COUT left behind" is whatever you left behind,
and every caller already had it as a parameter. The stack went the same way: a
6502 pushes because JSR clobbers registers, and a C call cannot touch a
caller's local, so all 21 PHA/PLA pairs were protecting values from nothing.

The general point is not "there is never a floor" — it is that **a floor is a
claim and gets the same treatment as any other**. Two wrong liveness claims in
this file were produced by reading a routine's body; this one was produced by
reading a *dispatch mechanism* and stopping at the first true sentence about
it. Keep going until the answer is a measurement or a closed argument. Expect
roughly half of what you find in 6.2 and 6.3 to be dead.

**Check:** Treat any claimed floor as a claim, and check it the same way you
would check a rule.

### 6.1 Names

**What it costs.** Nothing, which is why it is first — do it while the trace
oracle is at full strength.

**The oracle for a rename is a compile, not the gate.** See
[Techniques: proving an edit changed no code](#proving-an-edit-changed-no-code).
Do not rename and restructure in the same step, or every failure has two
candidate causes.

**`[process]` Naming a domain partially is worse than not naming it.** Once
`INK_SNAKE` exists, a reader infers that a bare `0x0c` nearby is *not* an ink —
so every member of the domain must be named, or the exceptions marked as
deliberate. The same trap caught a helper introduced for "the three routines
that write the screen" when there were four: three call sites named, one not,
and the name now actively misleads. Either cover the domain or annotate what
you left out.

**Check:** Name every member of a domain, or mark the exceptions deliberately.

**`[process]` Group the globals before renaming them.** Decompiled code arrives
as a flat sheet of file-scope variables, and no function signature says what it
touches. The groups are usually already implied by a comment ("these belong to
the ROM routines, not the game") — that sentence should be a type. Grouping also
separates the program's own state from the emulator's leftovers, which is the
distinction a reader most needs and the one flat naming hides.

**Naming is what audits the names.** Four of Snake Byte's were wrong, all in
converted C that passed every gate: a pair read as a countdown and was a
population; one address was the level's time allowance and not the apple's
value; a "poll and discard" was a speaker click; and an "eat apple" awards an
extra life. Writing a name down forces the claim to be checkable in a way that
using the address never did.

**Check:** Group globals into structs before renaming them; the comment that
says 'these belong together' should be a type.

### 6.2 Storage

**What it costs.** One narrowing of the memory oracle per slice — and the order
of the two acts is what makes the narrowing mean anything.

**`[process]` The order of a storage move is what makes the gate evidence.**
Move the storage with the memory probe *untouched* first, and require the gate
to read **trace PASS, screen PASS, memory FAIL**. That failure is the only proof
the oracle was watching those bytes. Narrow the probe afterwards, then mutate
each new edge address and watch memory alone catch it. Narrowing first is
indistinguishable from narrowing over bytes nothing writes, and you will never
find out which you did. Snake Byte: 34 edges across four slices, all 34 caught.

Not every edge that fails to be caught is a hole. One address here is
recomputed before every use, so a stray write is overwritten before the next
sample can see it. Do not widen the hash to "fix" that.

**Check:** Move storage with the memory probe untouched and require the gate to
read trace PASS, screen PASS, memory FAIL.

**`[process]` A store becomes dead the moment its storage leaves RAM, and no
warning fires.** These read as faithfulness and are scaffolding: the routine
already computed the value in a local and committed it through the struct it
was handed, and the store existed only because a memory oracle hashed the
address. Snake Byte had seven. `-Wall` says nothing about a dead store to an
emulated-RAM array, and — separately — nothing about a *file-scope static* that
is written and never read. One was written at fourteen sites and read at none
for an entire build with every warning and every check green.
`-Wunused-but-set-variable` covers locals only, and is a different warning from
`-Wunused-variable`, so a grep for the latter misses it.

**Check:** Classify every store after a storage move. `-Wall` says nothing
about a dead write into an emulated array.

**`[process]` Thread a global and delete its writes in two commits.** The
intermediate state — written, never read — is exactly what no warning catches,
so gate it on its own. Otherwise a mistake in either half gets attributed to
the other.

**Check:** Split threading and deletion into two commits so a mistake in either
is attributable.

**`[6502]` Inheritance is sometimes a value the caller can state, and sometimes
not.** Two globals can look identical on paper — both inherited across calls,
both documented as globals for that reason — and be different things. Check each
reader: if every writer's value is readable at the call site, it is an argument
and the inheritance becomes something the code says out loud. If a caller
genuinely means "whatever was last used", it is a global and passing one would
invent a value the original does not have. Snake Byte's ink threaded; its shape
did not. **Mutate an inherited value you inferred** — one of the two inferred
inks here is caught only by the scenario that presses `C`, and would have been
wrong in silence under the other.

Writing the inherited values out is where this stops being cosmetic. Three were
load-bearing and none looked it: a box that reads as four edges at one column
is not, because an hline leaves the cursor at its own endpoint; a wall routine
computed its seam from a variable still holding a value from two calls earlier;
and two glyphs inherit their column from the arrow plotted above them.

**Check:** Check every reader: thread the value if each writer's is readable at
the call site, and keep the global if it genuinely inherits.

**`[process]` The emulated stack goes with the storage.** Every PHA/PLA
bracketing a *call* is protecting a value from nothing, because a C call cannot
touch a caller's local. Check balance per routine before removing — a loop that
pops at the top and pushes at the bottom looks unbalanced statically and is not.

**Check:** Check stack balance per routine before removing the emulated stack,
not per block.

**`[process]` Count the effect, not the identifier.** Snake Byte's memory row
read "3 remaining" because it counted the externally-linked accessor pair; the
file also reads through a `static` pair, which was 20 more calls. The same trap
as counting hex literals, from a new direction. Grep for every read of the RAM
array, and check whether the accessor routes some addresses elsewhere — four of
those calls were `$C0xx`, which is IO, not memory.

**Check:** Count the effect, not the identifier — every spelling that reaches
the same storage.

### 6.3 Flags and registers

**What it costs.** Nothing new, if 6.2 is done. The technique is classification,
and the test is poisoning.

**`[process]` Classify every site; do not inspect them.** For a flag or a
register, split all its occurrences into reads and writes, then ask per name
which reads a write can reach. This is a script, and it is the difference
between an answer and an impression. On Snake Byte it retired five of seven
status flags and 290 of 307 register references — and the two impressions that
survived contact were both wrong.

"In the ROM, registers are the algorithm" is a hypothesis. Checked on Snake
Byte's flags it held for `C` and `D` and was false for `B`, `I`, `V`, `N` and
`Z` — 115 of 222 references. `B` was never read at all; `I` and `V` were only
ever pushed so they could be popped back into themselves.

**Check:** Split every occurrence into reads and writes, then ask per name
which reads a write can reach. This is a script.

**`[6502]` A flag pushed by PHP and restored by PLP is not necessarily live.**
If the only reader of a bit is the PLP putting it back into the flag it came
from, the bit is dead however faithful it looks. Check each bit of the pushed
byte separately: they do not all have the same answer.

**Check:** Check each bit of a pushed status byte separately.

**`[6502]` A flag that survives an audit may still be three edges of one
idiom.** Snake Byte's carry survived one audit as "genuinely crosses a call",
then turned out to be exactly this: a routine ending on a `CMP` whose caller
reaches an `ADC` with no `CLC` in front of it, so the compare's carry *is* the
+1. Those are return values. A global flag was hiding a real data dependency
behind something that looked like residue, which is why the wrong conclusion was
easy to reach and hard to notice. **Ask what a surviving flag is *for* before
keeping it.**

**Check:** Follow the value across the call, not down the routine's own body.

**`[process]` The last flag out is the one with two arms.** Deleting an arm
because a run never took it is the unexercised-path trap. Deleting it because
the regions that set the flag are *closed* is sound: Snake Byte's six SED/CLD
regions were checked mechanically and contain only helpers that take their
decimal-ness as an argument, and no call into the code with the other arm. Then
the arm is unreachable by construction and the measurement — 3,942 tests, all
binary — is corroboration rather than evidence.

**Check:** Delete an arm because the regions that set the flag are closed,
never because a run never took it.

**`[apple2tc]` A live-out claim sourced to the decompiler expires.** `--ir`
liveness is true of the *generated* program. When the caller that did the
reading becomes hand-written C, the claim stops holding and the comment
asserting it stays. Three of Snake Byte's did. Re-derive; do not inherit.

**Check:** Re-derive liveness after the reader becomes hand-written. Do not
inherit the claim.

**`[process]` A value the code cannot state can still be measured.** Delay loops
that count down from an uninitialised register are the common case. Do not
delete the dependency and do not preserve it blindly: assert what you think the
value is over full runs of every scenario, then pass it as an argument with a
comment saying it was measured rather than proved. Two of Snake Byte's were
zero, every time, and one of them was zero because the loop before it had just
wrapped Y.

Use [poisoning](#poison-do-not-delete-to-test-whether-a-store-is-dead) to test a
suspected-dead write, and
[the fixpoint](#a-liveness-claim-must-come-from-the-fixpoint-not-from-reading)
to find what a routine reads on entry. Both wrong liveness claims this work
produced were produced by reading.

**Check:** Assert the value you believe over full runs, take it as a parameter,
and record which of the two you did.

### 6.4 Lookup tables

**What it costs.** Nothing the gate can supply — which is the point of the last
entry here.

**`[process]` Read data tables out of the binary, and derive their extent.** A
listing prints `--code-at` regions as `DFB` and does not delimit tables. A first
pass at Snake Byte's dot-pattern table described it as "sixteen bytes"; it is
128, sixteen inks of eight, and the error only surfaced when a caller passed ink
9. Dump from the `.b33` (4-byte header, then load address) and find the table's
end — usually the next code address — before writing down its shape.

**Check:** Read the table out of the binary and derive its extent from the next
code address.

**`[process]` Size a table to the next code address, not to the data it visibly
holds.** Snake Byte's shape masks hold 27 shapes ending at `$61DF`, the last
one the code names. But the tail is drawn with shape `ahead + $0C` where
`ahead` is a lo-res cell, so 0–15, which reaches shape `$1B` and index 111 —
past the data, into the zero padding that runs to the code at `$6200`. An array
sized to the data, *or to the maximum a recording reaches*, is an out-of-bounds
read waiting for an unexercised path.

**Check:** Size the array to what the *index* can reach, not to the data you
can see or the entries a run touches.

**`[process]` The gate cannot check a table entry nothing reads.** Compare the
array against the binary byte for byte instead; it is a dozen lines of script
and it is the only thing that covers the rest. Worth repeating after any edit
to them.

**Check:** Compare every array against the binary byte for byte, and repeat it
after any edit.

**`[process]` "It is the image, not a variable" is usually not a reason.** Check
whether it is really one. Here it was standing in for "I have not derived the
extent" — a lookup indexed by a small integer is an array, and nine of the
twelve extents were already written down in the known-data file.

**Check:** Derive the extent. 'It is the image' is usually 'I have not measured
it'.

### 6.5 Byte pairs

**What it costs.** Nothing.

**`[6502]` A byte pair is a 16-bit value unless it is an arithmetic one.** The
tell that a pair should merge is a helper recombining it at every use. The tell
that it should *not* is arithmetic that carries between the halves explicitly:
Snake Byte's BCD counters stay as byte arrays because the game adds them a byte
at a time with its own carry, and folding them into one integer would invent a
meaning the game never uses. Merge the pointers, keep the counters, and where a
routine writes one half of a merged value, spell it as a mask or a shift — that
distinction was hidden by the split, not expressed by it.

**Check:** Merge a byte pair when a helper recombines it at every use; keep it
split when arithmetic carries between the halves.

### 6.6 The clock

**What it costs.** Potentially everything, and this is the one whose oracle you
have to reason about *before* you start. It comes last for that reason.

A per-block cycle macro in generated code does two unrelated jobs: it dispatches
the probe that makes the block-head trace, and it advances the emulated clock
the host's frame loop runs on. **A plan that describes it as one of those will
produce a step that cannot be executed as written.** Separate them first:

- **Retiring the trace** — stop the sites probing. Cheap, and it spends the
  strongest oracle you have while changing no code.
- **Removing the addresses** — replace the address-carrying charge with one that
  names none. The clock needs the *counts*; nothing needs the address once the
  trace is gone. Timing comes out bit-identical, which is what makes the change
  checkable against the build you are comparing with.
- **Removing the clock** — mostly possible, and it does *not* end the comparison
  method. That was the prediction here and it was wrong.

**And the addresses cost more than the charges do.** A charge is one line. An
address it needs is a *table*, a struct field, or a parameter — and those
propagate to every call site. Snake Byte's last fifteen charging sites were
holding up four tables, six of one struct's eight fields, and eight function
parameters across ten call sites; removing the charges collapsed a routine that
compares two numbers from three parallel address tables down to a loop, and
turned a struct of five address fields wrapped around one key into an array of
four keys. Count the *consequences* of a charge, not the charges.

**Most of the clock is not the clock.** Sort the charges into two piles:
**incidental**, paying for arithmetic nobody can hear or see; and
**load-bearing** — it sets a pitch, it is a pause someone can see, or it is the
only point at which a waiting loop can hand control back. Snake Byte's split
was 838 to 23. Deleting the first pile is most of what makes the file readable.

**`[process]` Check what your oracles are stamped on before you believe any of
this is safe.** If probes sample on cycles or frames, deleting the clock
desynchronises every comparison immediately. If they sample at *code addresses*,
and replay stamps its input on a counter incremented at those same addresses,
the entire gate is already time-independent and survives untouched. That was
true here and it is what made the step possible — the probe language had been
built that way for unrelated reasons, and nothing in the design said so. Read
the probe scripts; do not reason about it.

**Check:** Check what the oracles are stamped on — cycles, frames, or code
addresses — before touching the clock.

**`[process]` Sweep for every spelling before believing you are done.** A cycle
charge usually has more than one name — one that probes and one that does not,
one for edges, one for the sites carrying replay coordinates. Snake Byte's
collapse went from 838 to 23 and left fifteen behind, purely because they were
spelled differently from the name being grepped, and they turned out to be the
ones holding all the address tables. Grep for the macro that *expands* to the
charge, not for the name you have been typing.

**Check:** Grep for every spelling a charge has, not the one you remember.

**`[process]` Calibrate each survivor to the region it stands for** — the
measured cost from that charge to the next one, not what the original charge
was. Deleting the incidental pile shortens every interval, so a naive collapse
makes the tone go sharp. Three ways the measurement lies:

- **A routine's total is not its own cost.** It includes everything it calls,
  and those callees still charge. Measure the routine's *own* charges with an
  accumulator saved and zeroed on entry and restored on exit — and use
  `__attribute__((cleanup))` on it, or every early return goes unreported. One
  routine here would have been charged 73,939 instead of 148.
- **A region can contain charges that survive for other reasons.**
  Probe-carrying sites charge for themselves. Counting them in the calibrated
  value double-charges the loop; here that made a wait loop 50% slow and only
  the block-head trace noticed, with a 5% margin.
- **Constant across your recordings is not constant.** Check whether a loop's
  trip count depends on an argument the scenarios never vary. Two routines
  measured identical at every call and were left alone for exactly this reason;
  three others carry a formula in a comment rather than the number.

**Check:** Measure each survivor's region from that charge to the next, and
subtract any charge inside it that pays for itself.

**`[process]` Drawing time and pacing time are different questions.** Making a
fill instant gives up the *sight* of it, which is usually a fine trade. But if
the main loop's tempo is set by how long a redraw takes — and it often is on
hardware this slow — then that redraw's duration is gameplay. Snake Byte's walls
cost 28,848 cycles once per step; deleting them silently made the game a
quarter faster. This is the site the design's own survivor list did not have.

**Check:** Measure whether the main loop's tempo depends on the redraw before
collapsing it.

**`[process]` A loop that waits for input and cannot yield hangs the program,
and every check stays green.** Emulated code usually runs as a coroutine: the
cycle charge is the suspend point, and the host draws a frame and polls the
keyboard in the window it opens. Delete the last charge inside a loop that spins
on the keyboard and the host is never given a turn, so the key that would end
the loop can never be delivered.

Under probe-stamped replay this is invisible in the worst possible way. The
replay coordinate advances at the keyboard *read*, so a spinning loop keeps
incrementing it and keeps being handed keys — without a frame ever happening.
Every oracle passes while the real program is frozen. Two loops shipped like
this here and were found by a person playing the game.

So **playing it is a verification step, not a courtesy**, and it is the only one
that covers this. Where it can be automated, automate the *property* rather than
the instance: a static check that every loop reading an input port contains a
charge finds the whole class, takes twenty lines of `awk`, and does not decay
when timing changes. The set has to be
[derived, not listed](#derive-the-set-a-list-written-before-the-work-is-a-starting-point).

**Check:** Lint every loop that reads input for a charge that can suspend, and
derive the set rather than listing it.

**`[6502]` Merging adjacent cycle charges is not free, and the cheap oracle
cannot see why.** Consecutive charges look obviously mergeable and the
arithmetic is identical, but a charging macro usually also tests whether the
budget is spent and yields if it is. Merging removes one such test, so a frame
boundary lands later.

Snake Byte tried this, measured it, and did not keep it — which is the useful
part. Over two full scenarios: 4 frames of 1,300 and 3 of 1,300 changed their
video hash, frame cycle counts moved by up to 56, and the run drifted 4 cycles
in 22.1 million. The program-defined probes were byte identical at all 16,332
samples, so nothing about the program changed. It buys 49 fewer statements and
costs a permanent, unrecoverable difference in a real oracle: anyone who later
gates on frame hashes would inherit the drift with no way to tell it from a bug.
**The trade is 49 statements against an oracle you keep.** Cosmetic
consolidation is the cheapest kind of change to give up, so give it up first.

**Check:** Do not merge adjacent charges. The test one of them performs is not
redundant.

**`[6502]` Some charges carry the loop counter, and deleting them stops the
loop counting.** Collapsing charges is a deletion pass, and one shape the
decompiler emits makes deletion unsafe:

```c
do {
  TICK(4);
  if (--y)      /* the loop counter lives in the charge's condition */
    TICK(1);
} while (y);
```

That is how `DEY / BNE` comes out: the branch costs a cycle more when taken, so
the charge is guarded by the test and the *decrement* sits in the guard. Delete
the charge as a charge and the statement goes, and the counter with it. Snake
Byte's `game_sound_sweep` lost all four of its counters this way and played 2
clicks where it should play 512, undetected for four days.

**The tell is that the `if`'s entire body is a charge.** Where the body does
something as well — `{ TICK(1); continue; }`, `{ TICK(3); break; }` — deleting
the charge leaves the control flow standing. Snake Byte had nine such
conditions and only the four bare ones were dangerous, so this is a grep before
you start, not a review afterwards: find `if ((--|++)` and check what is in the
body besides the charge.

**Check:** Grep `if ((--|++)` before collapsing; the dangerous ones are those
whose entire body is the charge.

**`[general]` An oracle's frame budget is part of the oracle.** The bug above
survived the check built to catch exactly it. The speaker's toggle timeline
*is* the waveform and goes red on any timing change — but it ran 1,300 frames
and the routine is first reached at frame 3,942, so the one oracle aimed at
sound never entered the one routine that is nothing but sound. The checks that
did run long enough could not see it: the routine writes no memory, draws
nothing, and those comparisons are deliberately cycle-blind. Every check was
green and every check was honest.

**Ask of each oracle not what it compares but what it reaches.** A budget is a
silent scope limit — nothing reports "this check ended before your code ran".
Derive the number from the latest thing it must cover and name that frame in a
comment beside it, rather than inheriting whatever the check was first written
with. When a routine's only observable effect is one that some oracle covers,
count the entries before believing the green.

---

**Check:** Derive each oracle's budget from the latest thing it must reach, and
name that frame beside it.

## 7. Make it an artifact

*Status: executed 2026-08-27.*

**What you do.** A decompilation that passes its oracles is not yet something a
person will tinker with. That is a separate pass, and it is mostly about
deleting: comments, casts, spellings, and the file boundaries the runtime was
forcing on you.

**What it costs.** It depends which of two kinds of change you are making, and
conflating them is how a step that "should emit identical instructions" gets
claimed for one that cannot.

- **Cosmetic edits** -- comments, casts, spellings, names. Confirmed by
  [the codegen oracle](#proving-an-edit-changed-no-code) alone, one at a time,
  no gate run needed. These *must* emit identical instructions; an exception is
  a finding, not a tolerance.
- **Structural edits** -- splitting the runtime header, splitting the program
  into translation units, extracting a function. These change what the compiler
  sees by design, so the codegen oracle does not apply and the gate is the
  check. Snake Byte's TU split was verified this way and also by comparing
  before/after sections, which agreed except for two `__LINE__` immediates --
  worth doing, but that is evidence, not a guarantee the category carries.

Function extraction sits in the second group and is the one to be careful with,
because the trace half of a gate can be blind to it: see
[calibrating by mutation](#calibrate-the-oracle-by-mutating-what-you-just-changed).

### The runtime header

**`[apple2tc]` A runtime header that *defines* the machine costs you separate
compilation.** Decompiler runtimes tend to ship as one header that both declares
and defines the emulated machine — registers, RAM, the accessors — with
everything `static` so that including it "just works". It does, once. It also
means every program built on it is a single translation unit, and the
decompiled output is usually the largest file anyone will ever have to read.

**It is probably already broken, half way.** Check whether any of the header's
functions are non-`static` definitions. Here fourteen were — the host-facing ABI
— so two includers would have collided at link time regardless. The `static` on
the remainder was buying per-TU privacy that nothing wanted, on top of a header
that could not be included twice anyway.

**The split is smaller than it looks.** Declarations header plus an
implementation fragment; the state becomes `extern`; the one-to-three-line
accessors stay in the header as `static inline`, so nothing at a call site
changes. A program that wants one TU includes both — which is what the old
header becomes, a two-line shim — and a program that wants more compiles the
fragment once in its own `.c`.

Measure before assuming the inlining matters: forcing the eleven hot accessors
out of line moved a 40,000-frame run by 0.01 s, because the emulated machine was
never the bottleneck. That measurement is not the reason to keep them inline —
they are accessors — but it is the reason not to argue about the rest.

Expect exactly one interface change and look for it early: some hook the
*program* supplies and the runtime calls. Here it was the coroutine's entry
point, `static` in the header and defined by each decompiled program; once the
thread lives in another TU it cannot be. That one word touches the emitter and
every generated file, so do it alone, first, and confirm the regeneration diff
is only that word.

**What it unlocks is the thing you actually wanted.** Once the runtime stops
forcing one file, the decompiled program can be split along its own seams. The
ROM entry points came out of Snake Byte as a normal `.c` — 682 lines, 12% of
the file.

**Count the interface in both directions, and count it from the header.** This
paragraph claimed "four symbols wide" until 2026-09-01, which was the count of
what the ROM needs *from the game* -- `s_mon`, `advance`, `GAME_CYCLES_COORD`,
one callback -- read as though it were the whole boundary. `rom.h` actually
publishes twelve: the monitor's state, ten ROM entry points and the game's
callback. The seam is still a good one, but a number taken from one direction
of a two-way interface will flatter it.

**Check:** Split the runtime into a declaring header and one compiled
definition before splitting the program.

### Comments

**`[process]` Comments in decompiled code sort into three piles, and only one
should go.**

- **Constraints stay.** "Computing the wrap early would be tidier and would not
  match." This is the most valuable kind of comment in a decompilation and there
  will be dozens: each one stops a future reader making a change that looks
  obviously correct. Losing one costs a bug that no test catches.
- **Narration goes.** "`$624E` is left one below range here and normalised at
  `$62B8`, which is the order the samples see." The reader cannot check it,
  cannot act on it, and it stands between them and the code.
- **Unverified-path warnings stay.** Routines transcribed from the binary that
  no scenario executes. That is a fact about how far the code can be trusted,
  and usually nothing else records it.

Bias toward keeping. Deleting one narration comment too few costs nothing.

**Check:** Keep constraints and unverified-path warnings; delete narration.
Bias toward keeping.

**`[process]` Comments outlive the code they describe, especially after a
merge.** A `\file` block here still explained an adapter split — marshalling,
accessors, "variables live at their original addresses" — directly above the
code that had eliminated all three. Sweep the header whenever a file absorbs
another one, and re-read every comment naming a variable you just deleted.

**Check:** Re-read the file-level comment after any merge; it describes the
code that used to be there.

**`[process]` Names that encode a distinction which has since dissolved are pure
cost.** A `_native` suffix separated hand-written from generated code; once
nothing is generated it is noise on every call site, and here it was 140 of
them. The tell is a qualifier that is true of everything.

**Check:** Drop a qualifier once it is true of everything.

### Casts and spellings

**`[apple2tc]` A decompiler's casts are noise, with three exceptions, and the
compiler will sort them for you.** Snake Byte's two hand-owned files carried 165
`(uint8_t)`/`(uint16_t)` casts; 161 of them said nothing C does not already do —
a cast to the type of the variable being assigned, or of the value being
returned. The three that survive are the ones whose truncated value *escapes*
before anything else narrows it: into an array index, or into the low half of a
16-bit word whose high half comes from elsewhere. The rule generalises —
`(uint8_t)` composes away through `+ - * | & ^ <<`, and does not through `>>`,
comparison, division, or an index.

Do not decide these by reading. Remove one cast, recompile at `-O2`, and compare
the disassembly against the untouched file: a cast that carries a value cannot
leave codegen alone. Removing them one at a time and reverting any that moves
the output settles the great majority mechanically, and *keeps* every one that
matters without anybody adjudicating it. Since only casts are being deleted, no
line numbers move, so `__LINE__` inside `assert` does not perturb the comparison
and `-DNDEBUG` is unnecessary — unlike the rename check.

**Then expect the oracle to be conservative, and finish the job by hand.** Six
of the 165 moved the codegen; only three of those moved a *value*. The others
blocked an optimisation: dropping a cast let gcc prove an address never wraps
and pick a cheaper induction variable, or drop an `sbb` for an `lea`. The way to
tell them apart is not more reading either — write the old and new expressions
side by side in a throwaway program and enumerate the whole input domain. Every
one of these had a domain small enough to exhaust outright (2^24 at worst),
which turns "I believe this is equivalent" into a count. **Include two or three
casts you already know are load-bearing as controls**, so a program that reports
"identical" everywhere has been shown able to report anything else.

**Sweep the spellings afterwards, in two passes, because they are two different
problems.** The first is cast residue: a `(uint16_t)` came with `0x0001` and
`0x00e0`, so deleting it strands a four-digit literal in byte-wide arithmetic —
the same 6502 `ADC #$7F` appeared as `0x7f` in one routine and `0x007f` in
another for exactly that reason. The parenthesis the cast needed strands with
it, leaving one associative chain wearing a grouping nothing uses any more.

The second is not residue at all and will be there whether or not you touch the
casts: **the decompiler prints every immediate in hex, including the ones that
are not immediates.** A shift count is a repetition count, so `band << 0x02`
sitting above `page << 8` is two spellings of one idea. Normalising them is what
finally made Snake Byte's BASCALC and GBASCALC look like the twins the comment
says they are — `((line >> 1) & 0x03) | 0x04` against `((row >> 1) & 0x03) |
0x04`, where one of them had worn `>> 0x01`.

Neither pass is a semantic change, so the same codegen oracle confirms each
outright.

**Check:** Remove one cast, recompile at `-O2`, diff the disassembly, revert if
it moved — `same-code.sh` does this.

### Warnings

**`[process]` A warning you cannot see is still a warning.** gcc and clang
disagree about which C constructs deserve a diagnostic — labels with nothing
after them, a label before a declaration — and a cleanup that deletes statements
produces exactly those. Build under **every compiler you can find**, not the one
your build directory happens to be configured with, or the person on the other
platform finds them for you. Cheap to gate: compile the hand-owned file
`-fsyntax-only` under each and fail on any diagnostic naming it. Use default
warnings rather than `-Wall` if that is what the real build uses, and record
what you deliberately left alone.

Note what `-fsyntax-only` does *not* report: unused functions. It comes back
clean on a file with three dead ones, which is how they survived a check that
looked like it covered this.

**Check:** Compile the hand-owned files under every compiler you can find, not
the configured one.

**`[process]` And `-Wall` only reports an unused function that is `static`**, so
the warning you want depends on linkage you may not have. The check that does
not depend on it is `nm`, against a written-down list:

```bash
nm -g --defined-only build/…/prog.c.o | awk '$2 ~ /^[TDB]$/ { print $3 }' | sort
```

Compare that to the set the program is *supposed* to export and fail on any
difference. Snake Byte's is three names -- two the runtime calls, one the ROM
file calls -- so a fourth is either a new interface, which belongs in the list
with a reason, or a missing `static`. This is `[exports]` in
probe-acceptance.sh, and it exists because the pass that made 98 functions
static missed one: it drove off a hand-written list of return types and
`SteerChoice` was not on it. Two days later the symbol was still exported, the
gate was green, and `-Wall` had nothing to say because the function was not
`static` -- the two failures protect each other in exactly the wrong direction.
[Derive the set](#derive-the-set-a-list-written-before-the-work-is-a-starting-point);
`nm` is the derivation.

**Check:** Compare `nm -g --defined-only` against a written-down export list
and fail on any difference.

**`[process]` An empty diagnostic list is not a clean one.** Check that the
comparison you are relying on actually ran. A "before and after" here reported
zero warnings for the old file because it had been copied out of its directory,
a relative `#include` failed, and the compiler aborted before producing any
diagnostic at all. Silence from a tool that never started looks exactly like
success. Assert on something the run must have produced — a line count, an exit
code, a marker — rather than on the absence of complaints.

**Check:** Assert on something the run must have produced — a line count, an
exit code — not on the absence of complaints.

**`[process]` Trust the build, not the IDE.** clangd in a repo like this lacks
the include paths and reports cascading phantom errors. Trust `ninja` and the
test suite.

**Check:** Trust `ninja` and the test suite over IDE diagnostics.

### Host-side design, if you own the host

**`[process]` A value fetched by callback at a moment the caller controls should
be handed over by value instead.** A host asked its front end "how much time has
passed?" from inside the frame it was sizing; the front end had already advanced
the marker that answered, so every frame measured zero. One assignment's
position was the whole bug. Passing the number at the moment it is known makes
the mistake unrepresentable rather than fixed.

---

**Check:** Hand the value over at the moment its owner knows it, rather than
fetching it by callback later.

# Techniques and idioms

Reusable procedures and 6502 patterns that apply at more than one step. Every
entry here is named from the steps that use it; an entry nothing names should be
deleted rather than kept for completeness.

## Proving an edit changed no code

*Used by 6.1 (names) and 7 (casts, spellings).*

Renames and comment deletions should emit identical instructions, and comparing
compiler output is the cheapest way to know. Compare `-S` output, not binaries,
and filter three things that move for reasons that are not code changes:

- **`-g0`** — debug info carries line numbers, so any edit moves it.
- **`-DNDEBUG`** — `assert()` bakes `__LINE__` into the text section, so
  deleting a *comment* changes the emitted code. Note what this also hides: the
  assert's whole argument is elided, so a change to an assert's condition is
  invisible to the check.
- **label renumbering** — compilers number internal labels sequentially, so
  adding or removing a function renumbers everything after it. Normalise
  `.L<n>` to a placeholder.

**A rename still changes the hash, and that is not a failure.** Assembly text
contains symbol names. The check for a rename is: identical *after* normalising
the old names to the new ones in the old output.

One trap that will cost you an afternoon: compile the old version **in its own
directory**, or a relative `#include` fails, the compiler aborts before emitting
anything, and the empty output compares equal to nothing at all. That happened
here on 2026-08-27 -- three warnings were reported as a regression on the
strength of a comparison that had never run.

`docs/decompiling/same-code.sh` is the recipe, because a recipe you retype is a
recipe you get wrong: this section carried a five-line shell snippet until
2026-09-01 that reproduced the very trap named above, and printed two
disassemblies without diffing them.

```bash
docs/decompiling/same-code.sh decoded/snake-byte/rom.c        # vs HEAD
docs/decompiling/same-code.sh decoded/snake-byte/rom.c <rev>  # vs anything
```

It builds the old copy beside the new one in a directory of symlinks to the
file's siblings, strips addresses and label numbers, diffs, and exits 0 on
identical. **It refuses rather than guesses**: if either side does not compile
it says the comparison is void and exits 2. Since it takes the old file against
*current* siblings, a header that changed in the same commit will trip that --
which is a real limit, not a bug, and it says so.

## Calibrate the oracle by mutating what you just changed

*Used by 3d, 5, 6.2, 6.3.*

A gate tells you it is green. It does not tell you whether it was *looking*. The
cheap way to find out is to break the thing you just changed, on purpose, in the
specific ways it could plausibly be wrong, and see which checks notice.

Done here after a 218-line, 21-`goto` state machine was re-nested into four
loops. Four structural mutations were planted:

| planted bug | 1,300-frame gate | 20,000-frame trace | 40,000-frame screen |
| --- | --- | --- | --- |
| loop init hoisted to the top | **caught** | — | — |
| exit unwinds too far | missed | missed | **caught** |
| first init deleted | missed | missed | **caught** |
| second init deleted | missed | missed | **caught** |

Three of four survived everything the work had cited as evidence. The code was
right — but nobody knew that from the gate, they knew it from having reasoned
carefully. Raising the budget turned reasoning into measurement.

**Do not let a check's budget encode a derived constant.** Two builds were
compared at 1,300 and 1,150 frames, the 150 being how long one spends booting
before the comparison can start. Raise both proportionally and it breaks,
because the offset is absolute, not proportional. The fix was to give both sides
the *same* budget and rely on "longer side is truncated to the shorter" — the
un-booted build always gets further per frame, so the invariant holds without
anyone computing the offset. Prefer over-provisioning plus a truncating
comparison over a tuned number that has to be re-derived whenever anything
moves.

## Poison, do not delete, to test whether a store is dead

*Used by 6.2 and 6.3.*

Assign a *wrong* value and run the gate. Deleting leaves the previous value in
place, which can coincidentally be the one a reader wanted; poisoning cannot. 42
register writes and 18 flag writes were cleared this way in batches, which also
makes it cheap: one build and one gate run per batch, not per site.

## A liveness claim must come from the fixpoint, not from reading

*Used by 5, 6.2, 6.3.*

To find what a routine reads on entry, run read-before-write to a fixpoint *over
the call graph*. Two claims here were made by reading a body, and both were wrong
the same way — the answer was in a callee. `rom_cout` reads X and Y only through
the vector it dispatches through. The carry looked live across a call and was
written by the callee every time. Knowing the failure mode after the first did
not prevent the second. It is a dozen lines of script.

Where the decompiler can answer it directly, let it: `apple2tc --ir` prints
`LiveIn`/`LiveOut` above every function, computed over the whole call graph.
Converting `$6C72` raised the question directly — the original leaves the matched
table slot in X, and whether the C has to keep doing so is not answerable by
reading the caller, because the chain runs four routines deep. The dump answers
it in one line (`func_6c72` LiveOut: `A, Y, STATUS_N, STATUS_D, STATUS_I` — no X,
but Y, so the joystick path does maintain Y), and answers it for `$6594` the
other way (`A, X, ...`, so the key dequeue's X write stays).

## For reachability, walk the IR, not the emitted C

*Used by 4b and 5.*

`apple2tc --ir` prints `Succ(...)` on every block header and every `Call` with
its target, which is a complete CFG and call graph in text; a walk from any
address is a dozen lines and needs nothing new from the decompiler. Snake Byte:
1,776 blocks reduce to 89 from `$3750`, and all 89 are game code.

The emitted C looks like it would do as well and does not. Scanning it for
`block_id = N` picks up the digits inside `pop16`, `tmp3_U16` and `sbc_dec16`,
inventing edges to block 16 and reporting 104 live cases against the true 89.
The C is a rendering of the IR; the IR is the thing with the edges in it.

The general form: when you want to write a script that parses generated C, check
whether the generating tool already has the data in its IR.

## Put the probes back to debug converted code

*Used by 5 and 6.*

Converting a routine deletes the block-head trace for it — that is the trade —
which also deletes the only tool that can say *where* the new C diverges from
the old. Snake Byte's top-level conversion came out with two inverted conditions
and the failure presented as "the game plays on instead of pausing", about
57,000 blocks after the mistake.

The fix is to respell every cycle charge in the converted file with the variant
that keeps its probe, rebuild, and diff against the generated original
block-for-block. Both inversions were pinned in one run. One caveat, learned by
tripping over it: the original's *edge* charges must stay unprobed. Respelling
those too reports the edges as differences and buries the real one.

What it caught, and both are the same shape — a branch whose sense reads
backwards from the C:

- `CMP #$FE / BNE` at `$7851`: the original branches *away* when the value is
  not `$FE`, so equality is the fall-through.
- The paddle button at `$788E` reads with bit 7 *clear* when pressed.

## Drive `id` from stdin

*Used by 2, 4b, 5, 6.4.*

`id` is a REPL with no argv parsing, but it reads commands from standard input,
so a one-liner disassembles any range:

```bash
printf 'loadb33 game.b33\nlabels labels.txt\ndis $664A $66A8\n' \
  | <build-dir>/tools/id/id
```

**Spell the path.** Bare `id` is coreutils' user-identity command, on every
`PATH` there is, and it exits 0 printing `uid=...` -- so the pipeline appears to
work and produces no disassembly at all.

It resolves the built-in Apple II symbol database automatically (`COUT1`,
`BASH`, `CSWL`), so you get named operands, not bare addresses. `dis` takes an
instruction count or a start/end range; `labels <file>` loads `ADDR name` pairs
that override the built-in names. Use this instead of decoding bytes by eye —
decoding by eye invites transcription errors.

## Keep a labels file next to the game

*Used by 5 and 6.1.*

It makes every later disassembly more readable, and it is the natural place to
park what has been *proven* about an address. Record the evidence in a comment;
a name asserting something unproven is worse than no name.

## Use the decompiler to check hand work, not to do it

*Used by 2 and 5.*

When a routine is missing because the trace never reached it, decode it by hand
— then build a *scratch* copy of the run-data with the address added as a branch
target, and diff the two implementations' behaviour. That validates the hand
decode against an independent one and yields exact cycle constants, without
editing the committed recording. The recording is evidence of what happened;
asserting reachability into it destroys that.

The decompiler is a bootstrap and an accelerator, not the thing doing the work.
Its rejections and its code/data classifications are evidence, not verdicts.

## Derive the set; a list written before the work is a starting point

*Used by 3c, 6.6, and any plan built from a design.*

A design that enumerates the things to keep is telling you what the author could
think of in advance. Treat it as a prior and then derive the real set from the
code — ideally as a check that re-derives it on every run.

This cost two defects in one session on one file. The design's table of
perceptible cycle sites omitted the redraw that paces the main loop, and omitted
both loops whose only charge was their yield. Each omission was found separately
and expensively; the twenty-line lint that replaces the table finds all of them
and would have found them on day one.

The same applies to a *plan* built from that design. A plan's task list bounds
what the author scheduled, not what the idea requires. If the tasks stop short of
the design's own statement of intent, finish the intent.

## Deriving a scenario instead of recording one

*Used by 3c and 3d.*

Adding a scenario need not mean recording one. If replay is stamped on a
program-defined coordinate rather than on cycles, an existing recording can be
re-stamped for a different entry point by a constant — measure the coordinate at
the new entry, drop the keys below it, subtract it from the rest.

And adding the scenario is only half. Check that a *sample point fires while the
new code runs*: the probes here sampled at an address the new path never
reached, so the new scenario bought trace coverage and no state coverage at all,
and the same mutation still passed everything.

## Verify in vertical slices, not horizontal phases

*Used by 5, 6, 7.*

A frame-hash mismatch tells you *that* something broke, not *where*. Converting
one subsystem at a time keeps every failure attributable to one change. Verify
and commit each slice independently.

## The inline-string-after-`JSR` idiom

*Used by 2, 4a, 5.*

A routine that does `PLA/PLA`, walks a `$00`-terminated string following its call
site, then `PHA/PHA/RTS` to resume past it, will be rejected for stack imbalance
at both ends.

**Two separate jobs, and only the first is required.** Recovering the *control
flow* -- teaching the tracer that the bytes after the `JSR` are data and
execution resumes past them -- is what unblocks the decompilation, and it is
what `--inline-str` does. Lifting each call site's bytes into a C string
literal is a readability cleanup on top, and Snake Byte never did it: its
`game_print_inline_str(uint16_t ret_addr)` still takes an address and reads the
game's own image through `peek`, because the strings live in the memory image
the program already carries and hoisting them would duplicate those bytes.
Take the first; decide on the second per binary.

**It also produces phantom self-modifying-code warnings**, because the
disassembler traces into the string and decodes text as instructions.

**And it is a reachability barrier, not just a disassembly nuisance.** The
continuation is a *computed* target: the static tracer cannot follow it, so any
call site the recording did not execute leaves everything after its string
classified as data. In Snake Byte one such site — `$7230`'s `RTS` at `$7251` — hid the entire
key-redefinition screen, `$7541-$7632`. When a region looks like unreachable data, check whether
an inline string ends just before it.

apple2tc handles the declaration side with `--inline-str=<file>`. Declared
routines must also be in `--extern-routines`, because a *generated* one would
still pop a return address the option has moved.

## Self-modification is rarer than the warnings suggest

*Used by 2 and 5.*

Snake Byte reported three self-modifying game blocks; two are a one-shot startup
relocator (a block copy) and one is a false positive from the inline-string
idiom. Net: none in the steady-state game. Low self-modification makes promoting
memory to real C variables much safer than it would otherwise be. Check for the
inline-string idiom before believing a warning.

## A game may hook `COUT` through `CSWL`/`CSWH`

*Used by 4a and 5.*

Snake Byte's `$6641` repoints `$36/$37` at its own hi-res text renderer at
`$664A`. A replacement for `COUT` must dispatch through the vector, never
reimplement the ROM path — and should fail loudly on an unrecognised target
rather than falling back, because a trace that never installs the hook cannot
catch the mistake. Check for writes to `$36/$37` and `$38/$39` before replacing
any character I/O routine.

## An undisplayed video page may be a data structure

*`[game]`. Used by 1 and 5.*

Snake Byte renders in both lo-res and hi-res: hi-res is what the player sees,
while the never-displayed lo-res page is the logical game board, read back via
`SCRN` for collision detection. If you own every primitive that touches such a
page, its representation is yours to change — but verify nothing else accesses
the address range first.

## The decompilation covers what the trace exercised, not what is reachable

*Used by 2, 4b, 5.*

Snake Byte's own `COUT` handler at `$664A` is installed by code the recorded
session never ran, so the tracer classified it as *data* and it is absent from
the generated C entirely — despite decoding cleanly as a glyph blitter. Look for
vector writes whose targets were never traced.

**And a listing cannot show you code on such a path.** Addresses reached only
through an asserted edge are never traced, so the disassembly listing prints
them as `DFB` data. One keyboard read that takes 6 of 19 keys in a scenario is
visible *only* in the generated C. Grep the C, not the listing, for anything on
an asserted-reachability path.

## Reaching untraced code takes an *edge*, not just an address

*Used by 4b and 5.*

`--code-at=<file>` takes `ORIGIN TARGET` lines and merges them into the runtime
data before disassembly. Adding the target to `branchTargets` only gets it
disassembled; it must also become a successor of the branch at `ORIGIN`, because
only a *dynamic* block earns an entry in the generated address-to-block map —
and without a map entry the code is decompiled but still reports `Unknown
address` at runtime.

Keep these claims in their own file, never in the recording: the recording's
worth is that it is a faithful record of what happened, and every asserted edge
needs its own written argument.

**`--code-at` cannot tell you whether you pointed it at data.** That untraced
bytes are code is exactly what you are asserting. It checks only that the target
starts a block and the origin is a branch, and warns if the target decodes to an
invalid instruction. **The real check is where the data boundaries land**: if the
disassembler, following your edge, stops exactly at the tables and font you
expected — `$75B3-$75D0` and `$66A9` here — rather than running through them,
the edge was pointed at something real.

## `DEY`/`BNE` counts a zero as 256

*Used by 5 and 6.6.*

The loop tests after decrementing, so `LDY #0` is the longest loop in the
instruction set, not the shortest. Writing it as `for (y = n; y != 0; --y)`
silently runs it zero times.

That is not a corner case to note and move past: Snake Byte's death pause takes
its delay lengths from ROM bytes at `$E000`, and **ten of the 255 it reads are
zero**. Getting it wrong cost 12,790 cycles — three quarters of a frame — on a
path that runs once per life. Write these as `do { ... } while (--n);` and the
question does not arise.

---

# Hazard index

One row per failure mode, with the step where it bites. Sorted by step, so
reading the rows for the step you are on is a pre-flight check. `T` files a
hazard that belongs to the tooling rather than to any one step.

**This table is maintained by hand and drifts.** Three rows for step 6.6 were
missing for two days after the rules were written, including the one that cost
a shipped sound bug. `lint-playbook.sh` will not catch that -- no script can
tell which rule is a hazard -- but it prints the steps where rules outnumber
rows, which is where to look. Run it after adding a rule.

| Step | Signal | What it means |
| --- | --- | --- |
| 1 | Externalizing entry points to shrink output | You also delete everything only they reached — and the library stays reachable via the start PC. Measure both routes. |
| 1 | `--asm` `Code range` markers as a coverage measure | They appear only at discontinuities — 260 bytes against 3,931 bytes of real instructions on Snake Byte. Count instructions. |
| 2 | Planning to reloop before recovering procedures | Blocked, not merely harder. The dispatch variable is the PC. |
| 2 | "This pass is purely mechanical" | Procedure recovery always has a judgment tail. Bound it, don't deny it. |
| 2 | Predicting *which* routines a recovery fix will unblock | The call graph decides, not the direct failure list. Predict counts at most, then measure. |
| 3a | Approximate cycle counts + frame-hash oracle | Mutually exclusive. Pick one. |
| 3a | Golden trace proposed for `tests/` | `tests/` is decompiler regression. Game data goes in `decoded/<game>/`. |
| 3a | `$(cmd \| grep ...)` under `set -e` with `pipefail` | No match exits 1 and kills the script with no message. A check that legitimately finds nothing needs `\|\| true`. |
| 3a | A verification script that ignores its arguments | `./verify.sh <build-dir>` read `$BIN` only and silently tested a stale directory — four false PASSes. Scripts that gate anything must reject arguments they do not understand. |
| 3b | A golden trace recorded from a single run | Not shown deterministic. Record twice and diff, or it will produce phantom bugs later. |
| 3b | A checker never observed failing | Corrupt an input and watch it fail. A check that cannot fail is not a check. |
| 3b | A rejection test that greps for `FATAL` | Twice a test passed while covering nothing, because a *different* check fired first and satisfied the grep. Assert the specific message, and prove the test can fail by deleting the check it covers. |
| 3b | A test that fails proves the check it names | Not until you have seen it pass for the right reason. One here specified `--frames=10`, at which the buggy and fixed builds are byte-identical; it failed the same way before and after and proved nothing. |
| 3c | Expecting "several hundred" of anything | Static stretches dominate replay traces. Measure the number; explain it if it surprises you. |
| 3c | Inferring "this code never runs" from the trace file | Branch-target lists are capped and record only targets, so fall-through blocks read as absent either way. Instrument and count. |
| 3c | "Absent from the trace" treated as "unreachable" | Different claims. Only the second is worth relying on, and it takes an argument, not an absence of evidence. |
| 3c | "No probe never fired" read as coverage | Per-probe, not per-address. One probe at 1,669 sites fires constantly while most sites stay dead. Intersect the site list with the trace. |
| 3c | Full block coverage of an arithmetic routine | Says nothing about the values. BCD-vs-binary on a byte the score never reaches fails no check. |
| 3c | A block covered by a recording that never varies its inputs | Coverage says entered, not tested. Two identical lookup tables make reading the wrong one free. |
| 3c | Coverage reported as a list of addresses | Group by feature. "The joystick" is actionable; forty hex numbers are not. |
| 3c | A plan's list of read sites is complete because it was grepped once | Snake Byte's coordinate plan named three keyboard sites and missed a fourth, capturing 11 of 23 keys. Once a script delivers keys at all, an uncovered site's keys are never delivered — not merely mis-timed. |
| 3d | A changed frame trace read as "the code I wanted ran" | It only proves *something* changed. Add a counter and confirm the routine was entered. |
| 3d | A set of oracles that all agree | Ask what they all *assume*, not what each covers. Four independent-looking checks here were all fixed-step, so the wall-clock path was covered by nothing for nine days. The blind spot is in the union. |
| 3d | A mute or config byte read in a hot path | Hardcoding its recorded value passes every check. Whole features hide behind one byte nothing varies. |
| 3d | Two frame-hash traces that disagree mean one engine is wrong | Not between *different* engines. Frame hashing presumes a shared sampling clock; equivalent implementations disagree in 1–3 frame bursts around transitions. Same-engine comparison is fine. |
| 3d | Comparing register traces across engines | `CPURegLiveness` and `dce` drop stores to dead registers by design, so generated code does not maintain `Y` or the flags where nothing reads them. Traces diverge on line 2. `--compat` makes the format diffable, not the content. |
| 3d | A probe that produces no output | Says nothing about agreement. A probe on a non-block-head address fires in the interpreter and does not exist in the generated program; the report then reads as agreement. |
| 3d | A green gate proves the code you just wrote | Only if the binary was rebuilt. Twice a gate reported six green checks against a stale executable — same-second mtime once, a failed build once. Make it refuse to run on binaries older than their sources. |
| 3d | Claiming a build is playable from headless evidence | Launch it. "Never actually launched" stated as a caveat is not a caveat, it is the finding. |
| 3d | A bug report that describes something expected | Separate the two before debugging. "It shows the BASIC screen and hangs" was one true observation about a cold-start snapshot and one real fault, welded together. |
| 3d | An ad-hoc run against a recorded input file | Check what else the replay needs. `.pkeys` stamps are values of a counter the *probe script* defines, so `--key-file` without `--probe` silently ignores every stamp and the numbers you collect look entirely plausible. Copy the gate's invocation. |
| 4a | A bodyless `ir::Function` | `getAddress()` derives from `getEntryBlock()`, which asserts on an empty block list: silent UB under NDEBUG. Use the explicit `isExternal()` flag. |
| 4a | A hand-written replacement only one build can run | It is a regression test against itself, not an oracle. Arrange for the other build to run generated code for the same address and compare. |
| 4a | The compiler will DCE unreachable blocks | It will not. `dce()` removes only instructions, and every `Void`-typed instruction counts as having side effects. A pass that orphans blocks must delete them itself. |
| 5 | Citing a helper function from memory | `support.h` has `format()`, not `stringPrintf()`. Grep before specifying an API. |
| 5 | An odd instruction assumed to be a typo | `ROR` where `ROL` was clearly meant still drove the display. Mutate the "fix" and watch it fail before believing the author slipped. |
| 5 | Matching the generated code's flag set | That set is a DCE result for today's call graph, not what the CPU does. Set what the 6502 sets. |
| 5 | Dropping cycle charges when code moves to real C | Frames are a cycle budget. Charge without probing or every later frame hash moves. |
| 5 | Converting one path of a shared block head | The other path keeps it in the site list; the engines then disagree and the count looks fine. |
| 5 | A `CYCLES` address that is not a hex literal | It vanishes from the site list. Compiles, runs, counts, and is never probed. Lint for it — and note the lint does not cover the file where converted code lives. |
| 5 | A hand-written file outside the site-list grep | Fine for real C, fatal for a cycle site. Lint that the two kinds of file use different spellings. |
| 5 | Converting a routine that contains an input-coordinate address | The replay counter stops advancing there and every later keystroke lands elsewhere. Keep the probe, and lint that only coordinate addresses do. |
| 5 | Every cycle-charging call site treated as a program location | The taken-branch penalty is charged on the *edge*, in a block the program is never at — so it must not trace or dispatch probes. 698 ROM addresses here are edges and 121 are also real block heads, so address alone cannot distinguish them. |
| 5 | `--ret-addr` treated as a cosmetic verification flag | It is on for correctness: without it a `JSR` pushes a sentinel, and the inline-data-after-`JSR` idiom finds its data by *reading* that address. But it is not for a shipped artifact, which wants no emulated stack. |
| 6 | Calling something "the floor" | It is a claim like any other and deserves the same check. The last 15 register uses here were declared irreducible; Y was read by nobody and X was already a parameter at every call site. |
| 6.1 | Renaming and restructuring in the same step | Every failure now has two candidate causes. |
| 6.2 | Narrowing a probe and a storage move in one step | The narrowing then proves nothing: it is indistinguishable from a hash over bytes nobody writes. Move first with the probe untouched and require *trace PASS, screen PASS, memory FAIL*. |
| 6.2 | A store kept "for faithfulness" once its byte leaves RAM | Often it was scaffolding. Seven here were computed in a local, committed through the struct the routine was handed, and mirrored to an address nothing read. |
| 6.2 | `-Wall` treated as the dead-store detector | It says nothing about a dead `ram_poke`, and nothing about a **file-scope static** written and never read. One was written at fourteen sites, read at none, and every warning and every gate stayed green. `-Wunused-but-set-variable` covers locals only. |
| 6.2 | Threading a global and deleting its writes in one commit | A mistake in either gets attributed to the other, and the intermediate state — written, never read — is exactly what no warning catches. Thread, gate, then delete. |
| 6.2 | An inherited value inferred from the call site | Mutate it. Two inherited inks were inferred here; one is caught only by the scenario that presses `C`, and would have been wrong in silence under the other. |
| 6.2 | An emulated stack around a C call | A 6502 pushes because JSR clobbers registers. A C call cannot touch a caller's local. Check balance per routine before removing — a loop that pops at the top and pushes at the bottom looks unbalanced statically and is not. |
| 6.2 | Counting one spelling of an accessor | The memory row read "3 remaining" because it counted the externally-linked pair; the `static` pair was 20 more calls. Grep for the *effect*, and check whether the accessor routes some addresses elsewhere — four were `$C0xx`, which is IO. |
| 6.3 | "In the ROM, registers are the algorithm" | A hypothesis. Checked on Snake Byte's flags it held for `C` and `D` and was false for `B`, `I`, `V`, `N` and `Z` — 115 of 222 references. Classify every site as a read or a write. |
| 6.3 | A flag pushed by PHP and restored by PLP | Not necessarily live. If the only reader of a bit is the PLP putting it back into the flag it came from, the bit is dead however faithful it looks. Check each bit separately. |
| 6.3 | A live-out claim sourced to `apple2tc --ir` | It was true of the *generated* program. Once the caller that did the reading becomes hand-written C, the claim expires silently and the comment asserting it stays. |
| 6.3 | A counter the original never initialises | It still has a value. Assert what you think it is over full runs of every scenario, then pass it as an argument and record that it was measured, not proved. |
| 6.3 | A flag that survives an audit | Ask what it is *for*. The carry survived one audit as "genuinely crosses a call", then turned out to be three edges of one idiom: a routine ending on a CMP whose caller reaches an ADC with no CLC. Those are return values. |
| 6.3 | A flag with two arms, one of which never runs | Do not delete the arm because a run never took it. Delete it when the regions that set the flag are *closed* — then it is unreachable by construction and the measurement is corroboration rather than evidence. |
| 6.4 | A table's size stated rather than derived | The listing does not delimit tables. Find the next code address. A 16-byte table that is really 128 is invisible until a caller indexes past 16 — and sizing to the maximum a *recording* reaches has the same failure mode. |
| 6.4 | The gate as a check on lookup-table contents | It only covers entries something reads. Compare the array against the binary byte for byte; it is a dozen lines of script and the only thing that covers the rest. |
| 6.4 | "It is the image, not a variable" as a reason to leave an address | Check whether it is really a reason. Here it stood in for "I have not derived the extent" — a lookup indexed by a small integer is an array. |
| 6.6 | A cycle charge whose condition has a side effect | `if (--y) TICK(1);` is how `DEY / BNE` decompiles. Delete the charge and the counter goes with it: Snake Byte's sound sweep played 2 clicks instead of 512 for four days. Grep `if ((--\|++)` before collapsing; the dangerous ones are those whose whole body is the charge. |
| 6.6 | An oracle that reaches the code it is aimed at | A budget is a silent scope limit. The speaker timeline ran 1,300 frames while the routine that is nothing but sound is first reached at 3,942, so the one check for it never entered it. Derive the number from the latest thing it must cover, and name that frame beside it. |
| 6.6 | A charge deleted from a loop that polls for input | It was the coroutine's only suspend point, so the loop now spins forever against a host that never runs. Derive the set with a lint over every loop that reads the keyboard; a survivor list written before the work will miss some. |
| 6.6 | Consecutive cycle charges treated as obviously mergeable | The arithmetic is identical; the control flow is not. A charging macro that also tests the frame budget loses a test when two merge, so frame boundaries move — 4 frames of 1,300 here. Tried and not kept. |
| 7 | Trusting IDE/clangd diagnostics on this repo | clangd lacks the include paths and reports cascading phantom errors. Trust `ninja` and the test suite. |
| 7 | A `\file` comment after a merge | It describes the file it was written for. One here still explained an adapter split directly above the code that had eliminated all three of its claims. Sweep the header whenever a file absorbs another. |
| 7 | A value fetched by callback at a moment the caller controls | Hand it over by value instead. A host asked its front end "how much time has passed?" from inside the frame it was sizing; the front end had already advanced the marker, so every frame measured zero. |
| T | Writing a script to parse generated C | The generating tool has the data in its IR. Extend it instead. See *For reachability, walk the IR*. |
| T | Self-modification warning in game code | Check for the inline-string idiom before believing it. |
| T | Hand-decoding bytes without disassembling them | `id` will do it in one command, with ROM symbols resolved. Decoding by eye invites transcription errors. |
| T | Replacing a routine that dispatches through a vector | The vector may point at game code. Honour it; abort loudly on an unknown target. |
| T | Region of "data" sitting right after an inline string | The string's `RTS` continuation is a computed target. If the recording never ran that call site, real code reads as data. |
| T | Asserting a code address without the branch it comes from | Half the edge. The target gets disassembled but never reaches the address-to-block map, so it stays unreachable at runtime. |
| T | `--code-at` will tell you if you point it at data | It cannot. That untraced bytes are code is exactly what you are asserting. The real check is that the surrounding data boundaries come out right. |
| T | Guessing whether a register is live after a call | `apple2tc --ir` prints per-function `LiveIn`/`LiveOut`. Reading the caller by hand is four routines of tracing for a fact the tool already computed. |
| T | Reading a routine's body to find its parameters | Follow the calls. `rom_cout` reads X and Y as well as A, because it dispatches through `JMP ($36)` into a hook that reads them — invisible in its own body. Both wrong liveness claims here were produced by reading. |
| T | Deleting a store to test whether it is dead | Poison it instead — assign a wrong value and run the gate. A deleted store leaves the *previous* value behind, which can coincidentally be the one a reader wanted. |
| T | A disassembly listing read as showing every read site | It cannot show code reached only through an asserted edge — the listing prints it as `DFB`. One keyboard read taking 6 of 19 keys in a scenario was visible only in the generated C. Grep the C. |
