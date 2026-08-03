# Playbook: converting apple2tc output into maintainable C

Distilled, reorganized, and rewritten freely as understanding improves. The
append-only rationale lives in `decision-log.md`; this file holds only what
transfers.

Written in skill shape (when to use → procedure → red flags) so it can be
promoted to a `.claude/skills/` skill once it has survived a second game.

**Maturity: early.** Snake Byte is the first target and its conversion has not
started. Everything below under *Findings* is evidence-backed observation.
Everything under *Procedure* is a plan that has not yet been tested — treat it as
a hypothesis, and correct it here as reality intervenes.

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

**`[6502]` The ROM boundary is usually far smaller than the ROM.** Snake Byte
pulls in 1,442 ROM blocks but calls only **9 distinct ROM entry points**. Count
the distinct `JSR`/`JMP` targets crossing into ROM before assuming the ROM is
expensive to remove — roughly 150 lines of C displaced all of it.

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

**`[apple2tc]` `routines.cpp` balances the stack per basic block, not per
routine.** `int stackLevel = 0;` is declared inside the per-block loop, so the
universal `PHA`-at-entry / `PLA`-before-`RTS` idiom is rejected as unbalanced.
On Snake Byte this alone accounts for 7 of 10 rejected game routines, including
the most-called one. Check whether this has been fixed before assuming a
rejection is meaningful.

**`[apple2tc]` `--irc1 -v2` already reports why every candidate was rejected.**
Do not write a script to infer it. Reasons seen: `block $X stack level not zero`,
`Pop8 block $X stack level underflow`, `terminator JmpInd`.

**`[apple2tc]` The `removeInvalidJSRs()` fixpoint can cascade rejections up the
call graph, but often does not.** On Snake Byte it converged immediately and
removed nothing. Confirm from the `-v2` candidate counts rather than assuming
either way.

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

**`[6502]` Per-frame framebuffer hashing over that replay is a cheap behavioral
oracle.** Record a golden trace from the known-good build, then require the
rewrite to match frame for frame.

**`[6502]` The oracle only stays exact if timing does.** Visible behavior depends
on cycle counts through spin loops and delay loops, so approximate timing lets
the game drift and the trace diverges for reasons unrelated to correctness. If
you intend to verify this way, carry the exact per-block `CYCLES()` totals
through the restructuring — c1 supplies them, so this costs bookkeeping, not
analysis.

**`[6502]` Verify in vertical slices, not horizontal phases.** A frame-hash
mismatch tells you *that* something broke, not *where*. Converting one subsystem
at a time keeps every failure attributable to one change.

### Apple II idioms worth checking for

**`[game]` An undisplayed video page may be a data structure, not a display.**
Snake Byte renders in both lo-res and hi-res: hi-res is what the player sees,
while the never-displayed lo-res page is the logical game board, read back via
`SCRN` for collision detection. If you own every primitive that touches such a
page, its representation is yours to change — but verify nothing else accesses
the address range first.

**`[6502]` Self-modifying code may be rarer than the warnings suggest.** Snake
Byte reported three self-modifying game blocks; two are a one-shot startup
relocator (a block copy) and one is a false positive from the inline-string
idiom. Net: none in the steady-state game. Low self-modification makes promoting
memory to real C variables much safer than it would otherwise be.

---

## Procedure

Untested. Correct this section from experience rather than preserving it.

1. **Scope.** Count blocks by address range. Identify the ROM/library share and
   the distinct entry points crossing into it. Decide the boundary before
   writing anything.
2. **Fix and run procedure recovery.** Use `-v2` to get per-candidate rejection
   reasons. Fix decompiler over-strictness where it is genuinely over-strict.
   Reduce the remainder to a named, enumerable manual list.
3. **Stand up the oracle.** Golden per-frame hash trace from the known-good
   build, plus a script that replays and diffs. Do this *before* changing
   anything, so the baseline is trustworthy.
4. **Structural conversion.** Replace the ROM boundary. Move the entry point to
   the program's real start. Recover remaining procedures by hand, rewriting
   idiom-based ones. Reloop each function — dominator tree, back edges for
   natural loops, then iterative region matching (sequence / if-then /
   if-then-else / while / do-while). Keep address-derived names throughout.
5. **Vertical reverse engineering.** One subsystem at a time: promote zero page
   to locals and parameters, extract structs from data tables, apply meaningful
   names. Verify and commit each slice independently.

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
