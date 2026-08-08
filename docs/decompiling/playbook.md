# Playbook: converting apple2tc output into maintainable C

Distilled, reorganized, and rewritten freely as understanding improves. The
append-only rationale lives in `decision-log.md`; this file holds only what
transfers.

Written in skill shape (when to use → procedure → red flags) so it can be
promoted to a `.claude/skills/` skill once it has survived a second game.

**Maturity (Snake Byte, 2026-08-02/07).** Steps 1–3 have been executed and
step 4a with them; their findings below are measured, not predicted. Step 4b —
retargeting the entry — and steps 5–6 remain untested: treat those as hypothesis
and correct them here as reality intervenes. The per-step status is on each step
in the Procedure section, which is the copy to keep current.

**Scope tags:** `[6502]` true of 6502 / Apple II work generally ·
`[apple2tc]` true of this decompiler · `[game]` observed once, may not generalize

---

## When this applies

You have a working apple2tc decompilation — a binary, a `--run-data` JSON trace,
and `-O3 --irc1` output that builds and runs — and you want source worth
maintaining rather than emulator-shaped C.

If the decompilation does not yet run, this is the wrong document; get it running
first, because a runnable build is the oracle everything here depends on.

---

## Findings

### Scope control comes first

**`[apple2tc]` Entry-point choice dominates output size.** Snake Byte traces from
the reset vector `$FA62`, so the program boots ROM into BASIC and the recorded
keystrokes type `CALL 14160`. Result: 1,442 of 2,097 basic blocks in the output
are Applesoft and Monitor ROM, against 641 blocks of actual game.

**`[6502]` The ROM boundary is far smaller than the ROM — but cutting it is not.**
Snake Byte pulls in ~1430 ROM blocks and calls only **9 distinct entry points**.
Count the distinct `JSR`/`JMP` targets crossing into ROM first. But do not assume
that externalizing them removes the ROM: measured, it removed **112 blocks**, not
1430. Library code is typically reachable by two independent routes — through its
public entry points, *and* through whatever the program's start PC leads to. Both
must be severed, and they are strongly non-additive (112 and 48 separately, 1530
together). Retargeting the entry point is the other half.

**`[apple2tc]` Externalizing routines is a decompiler capability, not a hand-editing
chore.** `--extern-routines=<file>` maps addresses to C function names; calls
become calls to bodyless `Function`s and unreachable blocks are deleted. Two
payoffs beyond the block count: an unrecoverable callee poisons its callers, so
externing one can unblock procedure recovery elsewhere (`$FDED`'s `JMP (CSWL)`
was the sole reason `$71F3`, the game's most-called routine, stayed unrecovered);
and the hand-written replacements are verifiable against an existing trace.

**`[6502]` Externalizing a routine deletes everything only it reached.** Cutting
Snake Byte's 9 also removed `GBASCALC`, `PLOT1` and the whole `$FB78` `COUTZ`
subtree, which the hand-written file then had to supply. Correct behaviour, but
it multiplies the hand-written surface over the naive estimate — size the work by
the reachable subtree, not by the entry-point count.

**`[6502]` Keep the original boot path while swapping leaf routines.** Changing
the entry point at the same time forfeits the ability to verify: a cold start
skips whatever boot frames the golden trace opens with, so nothing matches.
Externalize first, verify against the existing trace, retarget the entry
afterwards as a separate step.

**`[6502]` Measure the hardware surface early.** Snake Byte touches only
`$C000`/`$C061` (keyboard, joystick button), `$C010` (strobe), and
`$C050/$C052/$C055/$C057/$C053` (video mode), across a 20KB binary. A small
surface means most of the work is pure logic with no emulation dependency.

### Procedure recovery

**`[6502]` Recover procedures before attempting any control-flow structuring.**
This is not a preference, it is a hard ordering constraint. Inside a fused
mega-function, `RTS` becomes a *computed* branch to every possible return site,
so the CFG is near-complete and cannot be structured. See the decision log entry
of 2026-08-02 for the full argument.

**`[apple2tc]` `routines.cpp` used to balance the stack per basic block —
fixed 2026-08-02 in commit `ea29cdc`.** `int stackLevel = 0;` sat inside the
per-block loop, so the universal `PHA`-at-entry / `PLA`-before-`RTS` idiom was
rejected as unbalanced. Depth is now propagated along CFG edges and required to
be zero only at `RTS`. Measured effect on Snake Byte: **53 → 75** routines
identified, `snake-bytec1.c` from 108 to 152 function lines.

**`[apple2tc]` Recovering a ROM routine can unblock game routines.** The
predicted 7 game routines were *not* the ones recovered. Only 2 of them made it;
6 unpredicted ones did, because ROM `$F800` (`PLOT`) and `$F871` (`SCRN`)
suffered the identical stack bug, and recovering them cascaded *forward* to their
game-range callers. Net was 8, not 7 — the count held for entirely the wrong
reasons. Estimate recovery by reasoning about the call graph, not by counting
direct failures.

**`[apple2tc]` `--irc1 -v2` already reports why every candidate was rejected.**
Do not write a script to infer it. Reasons seen: `block $X stack level not zero`,
`Pop8 block $X stack level underflow`, `terminator JmpInd`. Since 2026-08-02
`--routines-report=<path>` emits the same information plus block sets, call
sites, dominator chains and natural loops.

**`[apple2tc]` The `removeInvalidJSRs()` cascade fires only once candidates get
far enough to reach it.** On Snake Byte pre-fix it removed nothing — the affected
routines were already dying earlier in `scanCandidate`. Post-fix it removes 5
game routines that now pass their own checks but `JSR` into still-rejected ones.
Fixing an early filter can therefore *expose* a cascade that previously looked
inert. Re-measure after every recovery change.

**`[6502]` One unrecoverable routine can block many.** Snake Byte's `$7230`
inline-string printer is not merely 10 unrecovered call sites — it transitively
blocks 5 other game routines through the cascade above. When triaging which
rejections to fix first, weight them by how many callers they block, not by their
own call count.

**`[apple2tc]` The routines report reflects a pre-`simplifyCFG` CFG.**
`identifySimpleRoutines` runs at `-O2`, `simplifyCFG` at `-O3`, so the report's
blocks are more granular than the final `--irc1` output and one source address
can appear as several blocks — e.g. `ADC` splits into binary-mode and
decimal-mode blocks sharing an address. Expect the report and the C to disagree
on block structure; that is not a bug.

**`[6502]` The inline-string-after-`JSR` idiom defeats procedure recovery.** A
routine that does `PLA/PLA`, walks a null-terminated string following its call
site, then `PHA/PHA/RTS` to resume past it, will be rejected for stack imbalance
at both ends. It also produces **phantom self-modifying-code warnings**, because
the disassembler traces into the string and decodes text as instructions. Check
whether a reported self-modification is really a string before believing it.
Such routines are not recovered but *rewritten*: `print_str(const char *)`, with
each call site's inline bytes lifted into a real string literal.

### Verification

**`[apple2tc]` A deterministic replay harness already exists.**
`lib/decapplib/decapplib.c` locks the run to exactly `1/60 * clock_freq` cycles
per frame whenever `--key-file` is supplied, so a cycle-stamped key recording
replays identically every time.

**`[6502]` Hash video *memory*, not the rendered framebuffer.** Three payoffs,
all confirmed on Snake Byte. Rendering derives its blink phase from wall-clock
time and is therefore not reproducible. Memory hashing is strictly more
sensitive. And it needs no graphics context at all — which is what makes headless
replay possible, and headless is what let the oracle run in an environment with
no usable X display. Hash the mode byte, the mixed flag, the text page and the
hi-res page; include the cycle count per line so the trace doubles as a timing
check.

**`[6502]` Prove the oracle is reproducible before recording it.** Have the
record path run the known-good build *twice* and refuse to write the trace unless
both runs agree. An oracle nobody has shown to be deterministic is worse than
none: it produces failures that look like real bugs. On Snake Byte this passed
first time, but the cost of checking is one extra run.

**`[6502]` Prove the oracle can fail, too.** Corrupt one hash in the golden trace
and confirm the checker reports failure and exits non-zero. A check that cannot
fail is not a check.

**`[6502]` Measure the trace's coverage; do not assume it.** Distinct-hash count
is dominated by static stretches, not gameplay. Snake Byte's 1300-frame trace has
only **191** distinct hashes: frames 0–168 are keystrokes at the BASIC prompt
(one stable run per key), 177–278 and 371–472 are identical 101-frame title/attract
plateaus, and continuous gameplay only runs from ~472 to the end — about 828
frames, 14 seconds. Correlate hash-run boundaries against the key file's cycle
stamps to find where gameplay actually starts, and check the tail is still
changing (a long frozen tail means a crash or an early-exhausted key file).

**`[6502]` The oracle only stays exact if timing does.** Visible behavior depends
on cycle counts through spin loops and delay loops, so approximate timing lets
the game drift and the trace diverges for reasons unrelated to correctness. If
you intend to verify this way, carry the exact per-block `CYCLES()` totals
through the restructuring — c1 supplies them, so this costs bookkeeping, not
analysis.

**`[6502]` Verify in vertical slices, not horizontal phases.** A frame-hash
mismatch tells you *that* something broke, not *where*. Converting one subsystem
at a time keeps every failure attributable to one change.

### Hand-decompiling

**`[apple2tc]` `id` is a scriptable disassembler; drive it from stdin.** It is a
REPL with no argv parsing, but it reads commands from standard input, so a
one-liner disassembles any range:

    printf 'loadb33 game.b33\nlabels labels.txt\ndis $664A $66A8\n' | id

It resolves the built-in Apple II symbol database automatically (`COUT1`,
`BASH`, `CSWL`), so you get named operands, not bare addresses. `dis` takes an
instruction count or a start/end range; `labels <file>` loads `ADDR name` pairs
that override the built-in names.

**`[6502]` Keep a labels file next to the game and grow it as names are
established.** It makes every later disassembly more readable, and it is the
natural place to park what has been *proven* about an address. Record the
evidence in a comment; a name asserting something unproven is worse than no
name.

**`[6502]` Use the decompiler to check hand work, not to do it.** When a routine
is missing because the trace never reached it, decode it by hand -- then build a
*scratch* copy of the run-data with the address added as a branch target, and
diff the two implementations' behaviour. That validates the hand decode against
an independent one and yields exact `CYCLES()` constants, without editing the
committed recording. The recording is evidence of what happened; asserting
reachability into it destroys that.

### Apple II idioms worth checking for

**`[game]` An undisplayed video page may be a data structure, not a display.**
Snake Byte renders in both lo-res and hi-res: hi-res is what the player sees,
while the never-displayed lo-res page is the logical game board, read back via
`SCRN` for collision detection. If you own every primitive that touches such a
page, its representation is yours to change — but verify nothing else accesses
the address range first.

**`[6502]` A game may hook `COUT` through `CSWL`/`CSWH`.** Snake Byte's `$6641`
repoints `$36/$37` at its own hi-res text renderer at `$664A`. A replacement for
`COUT` must dispatch through the vector, never reimplement the ROM path — and
should fail loudly on an unrecognised target rather than falling back, because a
trace that never installs the hook cannot catch the mistake. Check for writes to
`$36/$37` and `$38/$39` before replacing any character I/O routine.

**`[6502]` The decompilation covers what the trace exercised, not what is
reachable.** Snake Byte's own `COUT` handler at `$664A` is installed by code the
recorded session never ran, so the tracer classified it as *data* and it is
absent from the generated C entirely — despite decoding cleanly as a glyph
blitter. Look for vector writes whose targets were never traced. The decompiler's
code/data classification is evidence, not a verdict; decode the bytes yourself.

**`[6502]` The inline-string-after-`JSR` idiom is a reachability barrier, not
just a disassembly nuisance.** The printer pops its own return address, walks the
`$00`-terminated bytes following the call, then pushes the terminator's address
and `RTS`es past it. The continuation is therefore a *computed* target: the
static tracer cannot follow it, so any call site the recording did not execute
leaves everything after its string classified as data. In Snake Byte one such
site (`$7230`'s `RTS` at `$7251`) hid the entire key-redefinition screen,
`$7541-$7632`. When a region looks like unreachable data, check whether an inline
string ends just before it.

**`[apple2tc]` Reaching untraced code takes an *edge*, not just an address.**
`--code-at=<file>` takes `ORIGIN TARGET` lines and merges them into the runtime
data before disassembly. Adding the target to `branchTargets` only gets it
disassembled; it must also become a successor of the branch at `ORIGIN`, because
only a *dynamic* block earns an entry in the generated address-to-block map — and
without a map entry the code is decompiled but still reports `Unknown address` at
runtime. Keep these claims in their own file, never in the recording: the
recording's worth is that it is a faithful record of what happened, and every
asserted edge needs its own written argument.

**`[apple2tc]` The check that an asserted edge points at code is where the
*data* boundaries land.** Nothing can tell you untraced bytes are code — that is
what you are asserting. But if the disassembler, following your edge, stops
exactly at the tables and font you expected (`$75B3-$75D0`, `$66A9`) rather than
running through them, the edge was pointed at something real.

**`[6502]` Self-modifying code may be rarer than the warnings suggest.** Snake
Byte reported three self-modifying game blocks; two are a one-shot startup
relocator (a block copy) and one is a false positive from the inline-string
idiom. Net: none in the steady-state game. Low self-modification makes promoting
memory to real C variables much safer than it would otherwise be.

---

## Procedure

Each step carries its own status. Executed steps have been revised from what
actually happened; untested ones are hypothesis, so correct them from experience
rather than preserving them.

Step 4 is deliberately two steps, and only the first has been run. That split is
the whole point of it — see the step itself.

1. **Scope.** *(executed)* Count blocks by address range. Identify the
   ROM/library share and the distinct entry points crossing into it. Decide the
   boundary before writing anything.
2. **Fix and run procedure recovery.** *(executed)* Use `-v2` (or
   `--routines-report`) to get per-candidate rejection reasons. Filter them to
   the program's own address range first — most of the list is usually library
   code that step 4 deletes wholesale. Fix decompiler over-strictness where it
   is genuinely over-strict, then **re-measure**: fixing one filter can expose a
   cascade that previously looked inert, and the routines actually recovered may
   not be the ones predicted. Reduce the remainder to a named, enumerable manual
   list, ranked by how many other routines each one blocks.
3. **Stand up the oracle.** *(executed)* Golden per-frame hash trace from the
   known-good build, plus a script that replays and diffs. Do this *before*
   changing anything, so the baseline is trustworthy. Prove it both reproducible
   (record twice, refuse to write unless they agree) and capable of failing
   (corrupt a hash, confirm non-zero exit). Then measure what the trace actually
   covers.
4. **Cut the library boundary, in two separate steps.**
   1. *(executed)* Externalize the entry points the program calls and supply
      them by hand, keeping the original boot path so the existing golden trace
      still verifies every change.
   2. *(untested)* Only then retarget the entry point to the program's real
      start. This needs an entry-state snapshot and a re-based trace, because a
      cold start skips the boot frames the trace opens with.

   Doing both at once forfeits verification for the duration, which is the
   entire reason this is two steps and not one.
5. **Structural conversion.** *(untested)* Recover the remaining procedures by
   hand, rewriting idiom-based ones. Reloop each function — dominator tree, back
   edges for natural loops, then iterative region matching (sequence / if-then /
   if-then-else / while / do-while). Keep address-derived names throughout.
6. **Vertical reverse engineering.** *(untested)* One subsystem at a time:
   promote zero page to locals and parameters, extract structs from data tables,
   apply meaningful names. Verify and commit each slice independently.

---

## Red flags

| Signal | What it means |
| --- | --- |
| Planning to reloop before recovering procedures | Blocked, not merely harder. The dispatch variable is the PC. |
| Writing a script to parse generated C | The generating tool has the data in its IR. Extend it instead. |
| "This pass is purely mechanical" | Procedure recovery always has a judgment tail. Bound it, don't deny it. |
| Self-modification warning in game code | Check for the inline-string idiom before believing it. |
| Approximate cycle counts + frame-hash oracle | Mutually exclusive. Pick one. |
| Renaming and restructuring in the same step | Every failure now has two candidate causes. |
| Golden trace proposed for `tests/` | `tests/` is decompiler regression. Game data goes in `decoded/<game>/`. |
| A golden trace recorded from a single run | Not shown deterministic. Record twice and diff, or it will produce phantom bugs later. |
| A checker never observed failing | Corrupt an input and watch it fail. A check that cannot fail is not a check. |
| Predicting *which* routines a recovery fix will unblock | The call graph decides, not the direct failure list. Predict counts at most, then measure. |
| Expecting "several hundred" of anything | Static stretches dominate replay traces. Measure the number; explain it if it surprises you. |
| Trusting IDE/clangd diagnostics on this repo | clangd lacks the include paths and reports cascading phantom errors. Trust `ninja` and the test suite. |
| Hand-decoding bytes without disassembling them | `id` will do it in one command, with ROM symbols resolved. Decoding by eye invites transcription errors. |
| A changed frame trace read as "the code I wanted ran" | It only proves *something* changed. Add a counter and confirm the routine was entered. |
| Citing a helper function from memory | `support.h` has `format()`, not `stringPrintf()`. Grep before specifying an API in a plan. |
| Inferring "this code never runs" from the trace file | Branch-target lists are capped and record only targets, so fall-through blocks read as absent either way. Instrument and count. |
| "Absent from the trace" treated as "unreachable" | Different claims. Only the second is worth relying on, and it takes an argument, not an absence of evidence. |
| Replacing a routine that dispatches through a vector | The vector may point at game code. Honour it; abort loudly on an unknown target. |
| Externalizing entry points to shrink output | You also delete everything only they reached — and the library stays reachable via the start PC. Measure both routes. |
| A bodyless `ir::Function` | `getAddress()` derives from `getEntryBlock()`, which asserts on an empty block list: silent UB under NDEBUG. Use the explicit `isExternal()` flag. |
| Region of "data" sitting right after an inline string | The string's `RTS` continuation is a computed target. If the recording never ran that call site, real code reads as data. |
| Asserting a code address without the branch it comes from | Half the edge. The target gets disassembled but never reaches the address-to-block map, so it stays unreachable at runtime. |
| A hand-written replacement only one build can run | It is a regression test against itself, not an oracle. Arrange for the other build to run generated code for the same address and compare. |
