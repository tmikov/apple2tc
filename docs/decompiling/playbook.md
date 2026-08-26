# Playbook: converting apple2tc output into maintainable C

Distilled, reorganized, and rewritten freely as understanding improves. The
append-only rationale lives in `decision-log.md`; this file holds only what
transfers.

Written in skill shape (when to use → procedure → red flags) so it can be
promoted to a `.claude/skills/` skill once it has survived a second game.

**Maturity (Snake Byte, 2026-08-02 to 08-25).** Every step has now been
executed once, including step 6, which was the last to go and took the longest.
The findings below are measured rather than predicted — but *once*, on one
binary, so they are a strong prior and not a law. The per-step status is on each
step in the Procedure section, which is the copy to keep current.

The one thing a second game should expect to differ on is **cost**. The order
of the work turned out to be game-independent; how much oracle each step spends
did not.

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

### Hand-decompiling, once the boundary is cut

**`[process]` Measure what the oracle covers, per site, or it will flatter you.**
Snake Byte's cross-engine gate compares ~2.7 million block-head hits between the
interpreter and the generated build and passes. It reaches **744 of 1,669 block
heads — 45%**. Everything else is compared against nothing, and a mutation there
passes every check. The "a probe never fired" guard does not catch this: it is
per *probe*, and the trace is one probe installed at every address, so it fires
constantly while individual sites stay dead. Derive coverage by intersecting the
site list with the addresses the trace actually emitted, and assert a baseline
for the hand-written subset specifically — those are the blocks where a decode
error has no other net under it.

**`[process]` Block coverage is not value coverage.** Every block of Snake Byte's
`game_add_score` executes, and replacing its BCD addition with binary addition on
the score's *second* byte fails nothing at all: the score never reaches 100 in
either recording, so that byte never needs decimal correction. The low byte's
equivalent mutation is caught, and only by one of the two scenarios. Full block
coverage of an arithmetic routine says nothing about the values that flowed
through it.

**`[6502]` An instruction that looks like a typo is still load-bearing until you
prove otherwise.** Snake Byte's two cell plotters build the same table index from
the same operands, one with `ROL $06` and one with `ROR $06`; the `ROR` puts the
scanline parity in bit 7 where the following two `ASL`s discard it. It reads
exactly like a slip. "Correcting" it to `ROL` changes the screen and fails the
frame oracle. Reproduce what the bytes say, note the oddity, and let a mutation
test decide whether it matters.

**`[apple2tc]` Set the flags the 6502 sets, not the ones the generated code
keeps.** apple2tc's output is whole-program DCE'd, so a flag missing from a
generated routine is a proof that *the current call graph* never reads it — not
that the hardware does not set it. Dropping `$6148`'s carry to match passes all
eight checks today. Re-deriving that proof on every change costs more than the
assignment, and the assignment is never wrong.

**`[apple2tc]` A hand-written CYCLES site must carry a literal address.** The
site lists are built by grepping the C for `CYCLES(0x`, so an address that is
any other expression -- a table lookup, a variable -- still compiles, still
counts cycles, still runs, and never reaches the list. The probe is not
installed, neither engine reports it, and the diff is clean because both sides
say nothing. Measured: one routine written as a loop over a table of addresses
took eight sites out of Snake Byte's gate silently, and the site-count floor was
far too coarse to notice 1,669 become 1,661. Lint for it.

**`[process]` Coverage clusters by feature, so report it that way.** Snake
Byte's 60 unverified hand-written blocks are not 60 scattered branches; they are
the joystick (15), one unused display-list opcode (7), ROM paths for arguments
the game never passes (20), pause and mute (6), and five smaller groups. Grouped
like that the list is a description of what the recordings do not do, which is
actionable -- record a joystick session, play at another difficulty. Listed as
addresses it is noise nobody reads.

**`[process]` The frame oracle and the memory probe each catch what the other
misses, in both directions.** Snake Byte's memory check caught a byte written at
reset and never read, which moved no frame hash. The frame oracle caught a wrong
glyph on a menu screen, which the memory check could not see because it samples
at an in-game address that does not fire there. Neither is redundant, and
neither is a superset -- pick sample points that cover the phases you care
about, or accept that whole screens are checked by only one of the two.

### Converting emulator-shaped C into real C

**`[process]` The oracles decide the conversion order, not taste.** Three checks
watch Snake Byte, and they do not survive equally. Frame and screen hashes
survive to the end -- they look at video memory, which is the machine's no
matter who wrote it. Memory hashes survive until a variable's *storage* leaves
emulated RAM, because until then an adapter can write everything back before
returning. The block-head trace dies first, per routine, the moment a branch
moves into real C. So a single-block routine converts for free and a 62-block
one costs 61 sites; sort by payoff per site and the order writes itself.

**`[6502]` Timing is not part of what you are allowed to drop.** A frame is a
cycle budget, so converted code that runs at a different speed moves every later
frame boundary and every later frame hash. Charge the original's cycles with a
primitive that does *not* register a probe site (`CYCLES_EDGE`), and give up
observability rather than fidelity. Measured: folding three `CMP` blocks into a
helper lost 4 cycles three times and diverged every oracle at once.

**`[apple2tc]` An adapter keeps its entry probe site for nothing.** `CYCLES(addr,
0)` still sets the PC and dispatches; charge the block's real cycles inside the
converted function. Every conversion then costs one site fewer than its block
count.

**`[apple2tc]` A block head shared by two paths must convert in one move.**
Snake Byte's `$6216` is the `RTS` both the key dequeue and the keyboard poll end
on. Converting only the dequeue left the address in the site list -- the
unconverted routine still emits it -- while the generated build stopped firing
it there. The site count was still correct; only the trace comparison caught it.

**`[process]` A coverage number that can fall for two opposite reasons needs
saying so.** Snake Byte's unverified-block baseline dropped 60 -> 47 without a
single one becoming verified: they left the site list when their routines
converted. Unexercised code that stops being probed stops being counted. Pin the
site count exactly alongside it — that number only moves deliberately, so it is
the honest measure of progress.

**`[process]` Build the last oracle while the first ones can still check it.**
The screen-state check was added while the trace and memory checks were intact,
so it could be shown to catch a defect they caught -- a display-list operand
swap, failing at sample 7,503 of 26,111. Added after they were gone, it would
have been an assumption.

**`[tool]` The decompiler already knows which registers an adapter must write
back.** `apple2tc --ir` prints `LiveIn`/`LiveOut` above every function, computed
over the whole call graph. Converting $6C72 raised the question directly: the
original leaves the matched table slot in X, and whether the C has to keep doing
so is not answerable by reading the caller -- the chain runs four routines deep.
The dump answers it in one line (`func_6c72` LiveOut: `A, Y, STATUS_N,
STATUS_D, STATUS_I` -- no X, but Y, so the joystick path does maintain Y), and
answers it for $6594 the other way (`A, X, ...`, so the key dequeue's X write
stays). Deleting the two X writes and rerunning every oracle agreed, which is
corroboration; the dump is the reason.

This sits against "match what the 6502 sets, not what DCE kept" further up, and
the resolution is cost. A status flag is one line and always right, so write it.
A register write inside real C costs the abstraction the conversion exists to
buy, so it is worth checking whether anything reads it -- and the check is a
command, re-runnable whenever the decompilation changes.

**`[process]` A few addresses are load-bearing for *input*, not just for
comparison.** Probe-stamped replay counts hits at seven named addresses, and
keystrokes are stamped on that counter. Two of the seven sit inside game
routines. Converting one -- charging its cycles but dropping its probe -- stops
the counter advancing there, and every key recorded after that point arrives at
a different instant. It is caught, because only the generated side drifts (the
interpreter always runs the original binary), but caught as a 640,983-line
block-head diff naming four addresses in an unrelated screen; verify.sh, which
replays cycle-stamped keys, does not notice at all. Give those sites their own
spelling (`GAME_CYCLES_COORD`), assert the set of addresses using it equals the
coordinate's, and the same mistake is one line naming the address.

**`[process]` A relaxation that passes every oracle can still be wrong, and
the way to tell is structural.** Snake Byte's `$6A32` is rejected as a routine
because `$6AB3` is `PLA / PLA / JMP` -- it discards the caller's frame and jumps
into the main loop. Switching off the two stack-depth checks that reject it gets
the main loop identified as a routine at last, and passes everything: four
1300-frame scenarios, three block-head traces, memory including the stack
pointer, and the screen.

It is still wrong. The routine's block set is computed by following successors,
so the unwind edge pulls the *caller's* loop into the callee: the emitted
`func_6a32` contains `$6291`, the main loop head, and calls `func_6a32`. Every
unwind would add a C frame that never returns. The reason nothing failed is
measurable -- `$6A32` runs 143 times across the three recordings and `$6AB3`
runs zero -- so the recursion is latent, and no recording anyone is likely to
make would reach it either.

Two checks would have caught it and neither is a test: does any generated
function now call itself, and does a routine's block set contain an address
that belongs to its caller. Both are one grep. Run them on any change that
makes the decompiler accept something it used to reject.

The right shape turned out to be a calling convention rather than a relaxed
check, and it is `--alt-exit` (`routines.cpp`, `CallAlt`/`ReturnAlt`): the walk
*stops* at the unwinding block instead of following it, the routine returns
which exit it took, and the caller switches on that. `func_6a32` is then 50
blocks spanning `$6A32..$6AB5` and the three structural checks come back clean.
The lesson generalises past this one idiom -- when the classifier rejects
something, the question is what shape would let it be expressed, not which
check to disable.

**`[process]` A check that no test can make fail is not a check.** Six
conditions went into `--alt-exit`, and mutation-testing each one -- delete it,
confirm a test fails -- found three that nothing covered. One was genuinely
unreachable (the pops cannot have users given they are adjacent to the
terminator) and became an assertion. The other two were real holes: the fixture
called the alt-exit routine from top-level code, so nothing ever *cloned* the
`CallAlt`, which is where a release build was silently dropping its alternate
targets; and nothing reached a block that only exists as a callee's alternate
exit, which is the case that has to be re-walked and validated. Both needed the
fixture widened, not the code changed. Do this before the commit, not after:
the two holes were in the parts of the design that felt most obviously correct.

**`[process]` "Convert it when a recording reaches it" is a hold, not a plan.**
*(revised 2026-08-22; this entry used to say the opposite, and the reasoning it
was built on is kept below because the reasoning was not the part that was
wrong.)*

The original rule: a conversion is checked by exactly one thing, the recordings
executing it and the oracles agreeing; for a routine nothing executes there is
no check at all, and converting it also takes its addresses off the site list,
removing the unverified-block accounting that was the only remaining statement
about it. So Snake Byte's pause/mute handler at $69A9 -- six blocks, entirely
straightforward, entirely unexercised -- stayed in the emulator-shaped file
with a note saying why, to be converted when a recording pressed ESC.

Both halves of that are true and it was still the wrong call, because it
priced the waiting at zero. No recording was going to arrive: making one is a
deliberate act that nobody had a reason to perform, so "later" meant never, and
the routine sat as the last emulator-shaped thing in the file for its own sake.
Against that, what deferral actually buys is small. The decode is checkable
against the binary whether or not anything runs it -- `id` disassembles it, the
opcode timings give the cycle charges, and `--ir` gives the liveness -- and
that is the same evidence the conversion of any *reached* routine rests on for
the branches its recording happens not to take.

So: convert it, and pay the two real costs deliberately rather than avoid them
indefinitely. Name the unexercised blocks in a comment above the routine, which
is the only surviving record once they leave the site list. And find out by
mutation which of the *exercised* blocks are actually covered, because that is
usually less than the coverage number implies -- see the next entry.

What survives of the original rule is narrower and still worth obeying: do not
convert unexercised code *and* let the unverified count fall silently, and do
not describe the result as verified. Converting is a decision about where the
code should live. It is not evidence.
When it does bend, put the addresses in a comment above the routine, because
that comment becomes the only surviving record: converting takes them off the
site list, so probe-acceptance.sh stops counting them as unverified and the
number that used to say "these are unchecked" simply gets smaller.

**`[process]` An oracle built to ignore cycles cannot check cycles, and the
one that checks them may not run the code.** Snake Byte has two cross-engine
gates and neither covers $69A9's cycle charges. verify.sh compares per-frame
cycle counts, but its two scenarios never press an unrecognised key, so they
never enter the routine. probe-acceptance.sh does enter it, via play-rebind --
and stamps its input on a probe counter rather than on cycles, deliberately, so
that the two engines' cycle phase cannot perturb replay. Being cycle-independent
is exactly what makes it useless here.

Measured: 4 cycles written as 5 passes verify.sh 4/4, all three block-head
traces, memory and screen. Written as 4000 it fails, on frame-boundary drift.
So the check that does exist has a resolution of roughly a frame, and a
plausible arithmetic slip lands far inside it.

The lesson is not "add a cycle oracle" -- the probe counter's independence is
load-bearing. It is that a routine's coverage has to be read per *property*,
not as one number: this one's control flow is checked (inverting the Ctrl-S
test fails trace-ext on play-rebind) and its timing is not, and the same
"84/104 hand-written blocks run" reports both.

**`[process]` Dump the entry state; do not reason about it.** Retargeting an
entry point needs the machine state the boot would have produced, and that
state is small and knowable — for the Apple II it is `$0000-$0802`, the
registers, and one video soft-switch byte. Everything else is the program
image, the ROM, and uninitialised RAM reading as `$FF`. Reasoning about which
zero-page locations matter is slower and less reliable than adding a
capture-at-PC option and diffing two images.

Three traps inside that, each of which cost a debugging cycle:

- **The pristine boot state is the wrong state.** The machine at the BASIC
  prompt with nothing typed is not the machine at the program's entry: getting
  there means typing a command, and that leaves the parse state, the stack
  frames, the input buffer and the echoed text behind — 74 bytes for Snake
  Byte. Capture from the *booting build's own arrival* at the target address,
  not from a clean boot.
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

**`[tool]` For reachability, walk the IR, not the emitted C.** `apple2tc --ir`
prints `Succ(...)` on every block header and every `Call` with its target, which
is a complete CFG and call graph in text; a walk from any address is a dozen
lines and needs nothing new from the decompiler. Snake Byte: 1,776 blocks reduce
to 89 from `$3750`, and all 89 are game code.

The emitted C looks like it would do as well and does not. Scanning it for
`block_id = N` picks up the digits inside `pop16`, `tmp3_U16` and `sbc_dec16`,
inventing edges to block 16 and reporting 104 live cases against the true 89. The
C is a rendering of the IR; the IR is the thing with the edges in it.

**`[process]` To debug converted code, put its probes back temporarily.**
Converting a routine deletes the block-head trace for it -- that is the trade --
which also deletes the only tool that can say *where* the new C diverges from
the old. Snake Byte's top-level conversion came out with two inverted
conditions and the failure presented as "the game plays on instead of pausing",
about 57,000 blocks after the mistake.

The fix is to respell every cycle charge in the converted file with the variant
that keeps its probe, rebuild, and diff against the generated original
block-for-block. Both inversions were pinned in one run. One caveat, learned by
tripping over it: the original's *edge* charges must stay unprobed. Respelling
those too reports the edges as differences and buries the real one.

What it caught, and both are the same shape -- a branch whose sense reads
backwards from the C:

- `CMP #$FE / BNE` at `$7851`: the original branches *away* when the value is
  not `$FE`, so equality is the fall-through. Writing `if (x != 0xFE) goto
  fallthrough` inverts the routine.
- The paddle button at `$788E` reads with bit 7 *clear* when pressed.

**`[process]` The artifact should not share files with its own scaffolding.**
Snake Byte's converted game lived in files that three builds included: the
cold-start artifact, the booting reference it is checked against, and a
fixture. Every change to the game then had to stay safe for two builds still
running a generated dispatch over the same addresses -- which is a real
constraint, not a theoretical one. It first bit when a converted routine and
the generated dispatch both claimed the same block head and the site-list lint
refused them; the workaround was a separate file, which is the same problem one
step later.

The resolution is that the artifact gets its own copy of everything and shares
nothing. It costs duplication against builds that exist only to check it, and
it buys the freedom to change the thing being decompiled. Do it as soon as the
artifact exists, not after the third workaround.

**`[process]` A fixture built to reach new code also deepens the code you
already had.** Snake Byte's `easy` build exists to make the display list
reachable -- the apple quota lowered so levels change. It turns out to be the
only oracle that checks the score's BCD carry: breaking the propagation between
score bytes passes verify.sh 4/4, both 1300-frame block-head traces, memory and
screen, and fails only the 3000-frame run against the fixture. Neither
committed recording scores enough to cross a byte boundary. A fixture that
plays *longer* is worth as much as one that plays *elsewhere*, and the second
benefit is the one nobody plans for.

**`[process]` A block that runs is not a behaviour that was tested.** Snake
Byte's key table maps a *binding* to a *command* through two parallel arrays,
and both blocks that do the mapping execute constantly -- play.pkeys presses
I, J, K and M all round. But it presses the default bindings, where the two
arrays are byte-identical, so reading the wrong one changes nothing.
play-hires.pkeys does rebind, to W A D X Q E, and then stops: it never plays
afterwards. Swapping the two arrays in the converted C passes verify.sh 4/4,
all three block-head traces, memory and screen. Coverage counts the block and
reports it green. What a routine *distinguishes* has to be exercised, not just
entered, and no coverage number will say which recordings are missing.

**`[process]` Read data tables out of the binary, and derive their extent.** The
listing prints `--code-at` regions as `DFB` and does not delimit tables. A first
pass at Snake Byte's dot-pattern table described it as "sixteen bytes"; it is
128, sixteen inks of eight, and the error only surfaced when a caller passed ink
9. Dump from the `.b33` (4-byte header, then load address) and find the table's
end — usually the next code address — before writing down its shape.

---

### Getting the machine out of the code

Step 6's real content. The machine shows up in four disguises — storage at
addresses, status flags, registers, and lookup tables at addresses — and each
one has a method that works and an approach that feels right and is wrong.

**`[process]` The order of a storage move is what makes the gate evidence.**
Move the storage with the memory probe *untouched* first, and require the gate
to read *trace PASS, screen PASS, memory FAIL*. That failure is the only proof
the oracle was watching those bytes. Narrow the probe afterwards, then mutate
each new edge address and watch memory alone catch it. Narrowing first is
indistinguishable from narrowing over bytes nothing writes, and you will never
find out which you did. Snake Byte: 34 edges across four slices, all 34 caught.

**`[process]` Classify every site; do not inspect them.** For a flag or a
register, split all its occurrences into reads and writes, then ask per name
which reads a write can reach. This is a script, and it is the difference
between an answer and an impression. On Snake Byte it retired five of seven
status flags and 290 of 307 register references — and the two impressions that
survived contact were both wrong.

**`[tool]` A liveness claim must come from the fixpoint, not from reading.** To
find what a routine reads on entry, run read-before-write to a fixpoint *over
the call graph*. Two claims here were made by reading a body, and both were
wrong the same way — the answer was in a callee. `rom_cout` reads X and Y only
through the vector it dispatches through. The carry looked live across a call
and was written by the callee every time. Knowing the failure mode after the
first did not prevent the second. It is a dozen lines of script.

**`[process]` Poison, do not delete, to test whether a store is dead.** Assign a
*wrong* value and run the gate. Deleting leaves the previous value in place,
which can coincidentally be the one a reader wanted; poisoning cannot. 42
register writes and 18 flag writes were cleared this way in batches, which also
makes it cheap: one build and one gate run per batch, not per site.

**`[process]` A store becomes dead the moment its storage leaves RAM, and no
warning fires.** These read as faithfulness and are scaffolding: the routine
already computed the value in a local and committed it through the struct it was
handed, and the store existed only because a memory oracle hashed the address.
Snake Byte had seven. `-Wall` says nothing about a dead store to an emulated-RAM
array, and — separately — nothing about a *file-scope static* that is written
and never read. One was written at fourteen sites and read at none for an entire
build with every warning and every check green. `-Wunused-but-set-variable`
covers locals only.

**`[process]` Thread a global and delete its writes in two commits.** The
intermediate state — written, never read — is exactly what no warning catches,
so gate it on its own. Otherwise a mistake in either half gets attributed to the
other.

**`[6502]` Inheritance is sometimes a value the caller can state, and sometimes
not.** Two globals can look identical on paper — both inherited across calls,
both documented as globals for that reason — and be different things. Check each
reader: if every writer's value is readable at the call site, it is an argument
and the inheritance becomes something the code says out loud. If a caller
genuinely means "whatever was last used", it is a global and passing one would
invent a value the original does not have. Snake Byte's ink threaded; its shape
did not.

**`[process]` A value the code cannot state can still be measured.** Delay loops
that count down from an uninitialised register are the common case. Do not
delete the dependency and do not preserve it blindly: assert what you think the
value is over full runs of every scenario, then pass it as an argument with a
comment saying it was measured rather than proved. Two of Snake Byte's were
zero, every time, and one of them was zero because the loop before it had just
wrapped Y.

**`[6502]` A flag that survives an audit may still be three edges of one
idiom.** Snake Byte's carry survived one audit as "genuinely crosses a call",
then turned out to be exactly this: a routine ending on a `CMP` whose caller
reaches an `ADC` with no `CLC` in front of it, so the compare's carry *is* the
+1. Those are return values. A global flag was hiding a real data dependency
behind something that looked like residue, which is why the wrong conclusion was
easy to reach and hard to notice.

**`[apple2tc]` A live-out claim sourced to the decompiler expires.** `--ir`
liveness is true of the *generated* program. When the caller that did the
reading becomes hand-written C, the claim stops holding and the comment
asserting it stays. Three of Snake Byte's did.

**`[process]` The floor is where the machine's own dispatch is the interface.**
Not every register reference goes. Snake Byte's last fifteen are one thing: the
game repoints the ROM's output vector at its own renderer, so the hook is
entered through `JMP ($36)` and *cannot* take arguments — it reads its slot out
of X and restores the caller's X and Y on the way out. Removing those would mean
changing what the machine's dispatch is, not what the file says. Recognising the
floor is part of finishing; chipping at it is not.

**`[process]` Comments outlive the code they describe, especially after a
merge.** A `\file` block here still explained an adapter split — marshalling,
accessors, "variables live at their original addresses" — directly above the
code that had eliminated all three. Sweep the header whenever a file absorbs
another one, and re-read every comment naming a variable you just deleted.

### What "converted" is worth, and how to report it

*(added 2026-08-24, after taking one game through the whole cleanup)*

The measures that make a conversion look finished are mostly measures of the
wrong thing. Four that were used here, and what each is actually worth:

| Measure | Worth |
| --- | --- |
| `bb_N:` labels remaining | real — it is the decompiler's shape, and zero means every routine has been read |
| `ram_peek(0x...)` with a hex literal | **almost nothing** — it goes to zero the moment addresses get names, and `ram_peek(kApplesLeft)` is the same emulated-RAM access |
| adapters remaining | real, but only if they are *merged* rather than deleted; see below |
| `CYCLES` sites remaining | depends entirely on which kind, and the two kinds do different jobs |

Report the count that would embarrass you, not the one that reads well. "Zero
`ram_peek(0x...)`" was stated here while 314 `ram_peek(kFoo)` calls remained;
the number was true and the impression it gave was false.

**The order that actually matters** is not the order the plan had. Naming
addresses, giving routines parameters and deleting adapters are all worth
doing, and none of them moves the game's own state out of the emulated machine.
That is the thing a reader notices, it is bigger than everything else combined,
and it is easy to leave until last by accident because every other step
produces a satisfying number.

### Merging an adapter beats deleting one

An adapter exists to marshal machine state for a caller that no longer exists.
The instinct is to delete it, and deleting it costs the block-head trace the
adapter's `CYCLES` site — a real comparison point, gone.

Merging it into the routine it describes costs nothing: the charge, the site
and the write-back all move inside, the site is still emitted and still probed,
and the pinned count does not change. Forty-two adapters went this way with the
pin fixed and the trace hit counts identical to the digit.

The question an adapter poses is therefore not "can I afford to drop this
site" but "where does this write-back belong", and the answer is nearly always
"inside the routine it describes".

### `CYCLES` is two mechanisms, and a plan that says otherwise will mislead you

A per-block cycle macro in generated code does two unrelated jobs: it dispatches
the probe that makes the block-head trace, and it advances the emulated clock
that the host's frame loop runs on. A plan that describes it as one of those
will produce a step that cannot be executed as written.

Separate them before touching either:

- **Retiring the trace** — stop the sites probing. Cheap, and it spends the
  strongest oracle you have while changing no code.
- **Removing the addresses** — replace `CYCLES(addr, n)` with a charge that
  names no address. The clock needs the *counts*; nothing needs the address once
  the trace is gone. Timing comes out bit-identical, which is exactly what makes
  the change checkable against the build you are comparing with.
- **Removing the clock** — needs the artifact to pace itself some other way, and
  it ends the comparison method, because comparison means running the same
  emulated program twice.

The middle one is where the maintenance burden actually lives, and it is safe.
It was nearly skipped here on the belief that the charges could not go — which
was true of the *counts* and false of the *addresses*.

### Deriving a scenario instead of recording one

A gate covers what its scenarios execute. Ask which routines a scenario never
reaches before trusting it: here the cold-start gate ran one scenario that never
pressed the key leading to half the program, so an entire routine ran **zero**
times under it and a mutation there was caught by nothing.

Adding a scenario need not mean recording one. If replay is stamped on a
program-defined coordinate rather than on cycles, an existing recording can be
re-stamped for a different entry point by a constant — measure the coordinate at
the new entry, drop the keys below it, subtract it from the rest.

And adding the scenario is only half. Check that a *sample point fires while the
new code runs*: the probes here sampled at an address that path never reached,
so the new scenario bought trace coverage and no state coverage at all, and the
same mutation still passed everything.

## Procedure

Each step carries its own status. Executed steps have been revised from what
actually happened; untested ones are hypothesis, so correct them from experience
rather than preserving them.

Step 4 is deliberately two steps, and only the first has been run. That split is
the whole point of it — see the step itself.

The cleanup that follows conversion has its own six steps, and one game has now
been through all of them and out the other side; they live in `HANDOFF.md`
rather than here because their order turned out to be game-independent but their
*costs* were not. What transfers is in "Getting the machine out of the code" and
"What 'converted' is worth" above.

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
   2. *(executed 2026-08-23)* Only then retarget the entry point to the
      program's real start.

      This was predicted to need "an entry-state snapshot and a re-based
      trace". The snapshot half was right. **The re-based trace was not needed
      at all**, and expecting it is what made this look expensive for months:
      the *existing build is the oracle*. Snapshot it at the target address,
      start the new build from that, and compare the two directly — block-head
      trace and screen, at program-defined instants. No golden file is
      re-recorded, and the comparison is stronger than a golden trace because
      it is differential.

      Do it in two commits even so: retarget first and delete nothing, so the
      entry change can be shown to be behaviour-preserving on its own; then
      delete what is unreachable. Snake Byte's first commit changed one
      initializer and installed 2KB of state; the second removed 1,686 of 1,775
      dispatch cases.

   Doing both at once forfeits verification for the duration, which is the
   entire reason this is two steps and not one.
5. **Structural conversion.** *(executed)* Recover the remaining procedures by
   hand, rewriting idiom-based ones. Reloop each function — dominator tree, back
   edges for natural loops, then iterative region matching (sequence / if-then /
   if-then-else / while / do-while). Keep address-derived names throughout.
6. **Vertical reverse engineering.** *(executed 2026-08-24/25)* One subsystem
   at a time: promote zero page to locals and parameters, extract structs from
   data tables, apply meaningful names. Verify and commit each slice
   independently.

   That was right as far as it went, and it understated the step. What it
   describes is the *first* disguise the machine wears. There are four, and
   they come off in this order because each one makes the next legible:

   1. **Names**, while the trace oracle is still at full strength. A rename's
      oracle is a compile, not the gate: build at `-O2` before and after and
      diff the disassembly.
   2. **Storage** — variables out of emulated RAM, in slices, each spending one
      narrowing of the memory oracle. This is the step with a required order;
      see "Getting the machine out of the code".
   3. **Status flags and registers** — classified, not inspected, and tested by
      poisoning rather than deleting.
   4. **Lookup tables** — arrays, once you have *derived* each extent from the
      next code address rather than from the data you can see.

   Expect roughly half of what you find in steps 2 and 3 to be dead: stores that
   existed only because an oracle hashed the address, or because the original
   left a register set and nobody has checked since. Expect a floor, and expect
   it to be the machine's own dispatch rather than a tail of stragglers.

---

## Red flags

**`[apple2tc]` Converting a routine can drag its callee in behind it.** $6A32's
only call site is $630D, which is inside $6288. Externalize $6288 and $6A32 has
no generated caller left, so it is removed and the link fails asking for it by
name. The two are one conversion whether or not you planned them that way -- and
that decided a question that had been sitting open for days, because $6288 is
fully covered and $6A32 has seven blocks nothing runs. There was no version of
the work that took the first without the second.

Check before starting, not after: `--routines-report` prints each routine's call
sites, and a callee whose sites are all inside the routine you are converting is
coming with you.

**`[apple2tc]` Call the adapter the generated code called, not the native
behind it.** An adapter usually just marshals machine state, which makes it
tempting to skip. Three of Snake Byte's eleven do real work: `game_plot_hline`
charges 6 cycles and calls `game_load_shape`, `game_find_apple` carries the
entry probe, and `game_read_direction` charges 6 cycles *and steps the
bouncers*. Skipping that last one was a wrong screen and a 6-cycle offset from a
single substitution, and the two symptoms looked like two bugs.

The rule that costs nothing: whatever the generated C called at that site, call
that, with the same return address. It is also what keeps the callee's probe
sites firing, and the return address is what keeps the emulated stack the same
if a probe fires inside.

**`[6502]` `DEY`/`BNE` counts a zero as 256.** The loop tests after
decrementing, so `LDY #0` is the longest loop in the instruction set, not the
shortest. Writing it as `for (y = n; y != 0; --y)` silently runs it zero times.

That is not a corner case to note and move past: Snake Byte's death pause takes
its delay lengths from ROM bytes at $E000, and **ten of the 255 it reads are
zero**. Getting it wrong cost 12,790 cycles -- three quarters of a frame -- on a
path that runs once per life. Write these as `do { ... } while (--n);` and the
question does not arise.

**`[6502]` Which side of a branch pays the extra cycle depends on the branch.**
A taken branch costs one more than an untaken one, so a converted routine has to
charge that cycle on the same side the original does -- and *which* side that is
flips with the condition. $6A32 tries eight directions: the first test is a
`BNE` to the retry path, so its edge belongs to the refusal; the other seven are
`BEQ`s to the accept path, so theirs belong to the acceptance. Writing all eight
the same way was one decision and seven bugs.

Read the polarity off the generated C rather than the mnemonic -- it says
`if (cond) goto bb_N;` with the `CYCLES_EDGE` sitting in whichever successor
block pays it, which cannot be misread.

**`[process]` Memory agreeing before the cycles do tells you which bug is
left.** The memory probe compares every byte the routine writes; the frame
oracle compares when it finished. When the first passes and the second does not,
the logic is right and what remains is cycle accounting -- so stop reading the
algorithm and start reading branch edges. That ordering turned the last four
bugs from a search into a checklist, and it is worth running the memory oracle
alone, early, for exactly that reason.

Related: the artifacts these oracles leave in /tmp are only as fresh as the last
run that reached them. A suite that aborts early leaves the *previous* run's
files in place, and reading them cost an hour of chasing a routine that the
stale trace said never executed and the current one says runs 110 times.
Regenerate before believing.


**`[process]` The literal-address lint does not cover the file that needs it
most.** `check_literal_sites()` in probe-acceptance.sh refuses a `CYCLES` whose
address is not a hex literal, because the site lists are built by grepping for
that literal -- an address arriving through a variable still compiles, still
charges, and is never probed. Its own comment records the mistake being made
once, in a draft that looped over a table of addresses and quietly dropped eight
sites.

It runs on `a2rom.c` and `game.c`. It does *not* run on `game_native.c`, which
is where converted code lives and where every new `GAME_CYCLES` is written. A
helper taking the address as a parameter is therefore invisible there, and
converting $72CE hid nine addresses that way -- addresses that genuinely needed
`GAME_CYCLES_SHARED`, so the traces would have disagreed with no line naming
why.

Extending the lint is one argument, and it fails immediately: fourteen sites in
`game_native.c` are table-driven today, ten of them predating the routine that
found this. Until those are written out longhand the check cannot be turned on,
so write new conversions with literal addresses and do not introduce more.

The consolation is that nothing is silently *wrong*: the trace comparison still
catches a missing probe. It catches it as a several-hundred-thousand-line diff
rather than as one line naming the address, which is the whole reason the
spelling distinction exists.


**`[apple2tc]` An RTS keeps alive everything the recording ever saw it return
to.** Externalizing a routine erases the JSRs that called it -- and with them the
only instructions that pushed those return addresses. The edges remain, because
an RTS carries its whole observed return set as successors, and each surviving
edge keeps its target reachable and everything reachable from the target with it.

Snake Byte's inline-string printer at $7230 survived externalization completely
for this reason, and it took two wrong diagnoses to see why. $7239 is where its
own `JSR $FC68` came back to; $FC68 is external, so nothing pushes $7239 any
more, yet a ROM RTS still listed it. That one edge held the printer's body, its
RTS, and through that RTS the ten blocks its ten callers resume at -- which
presented as "the inline-string idiom leaves orphans" and is nothing of the kind.

`--prune-returns` drops them: the fall block of every erased JSR is a candidate,
and any RTS still naming one loses it. On Snake Byte, 64 edges and 298 further
blocks.

The reason to reach for this before anything cleverer is that it checks itself.
Deleting unreachable text cannot move a frame hash, a memory sample or a trace,
so every oracle must stay exactly where it was -- and every `GAME_CYCLES_SHARED`
that existed only because of an orphan becomes wrong, which probe-acceptance.sh
already tests for from the other side. 74 spellings flipped, each one named.


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
| "No probe never fired" read as coverage | Per-probe, not per-address. One probe at 1,669 sites fires constantly while most sites stay dead. Intersect the site list with the trace. |
| An odd instruction assumed to be a typo | `ROR` where `ROL` was clearly meant still drove the display. Mutate the "fix" and watch it fail before believing the author slipped. |
| A table's size stated rather than derived | The listing does not delimit tables. Find the next code address. A 16-byte table that is really 128 is invisible until a caller indexes past 16. |
| Matching the generated code's flag set | That set is a DCE result for today's call graph, not what the CPU does. Set what the 6502 sets. |
| Full block coverage of an arithmetic routine | Says nothing about the values. BCD-vs-binary on a byte the score never reaches fails no check. |
| A `CYCLES` address that is not a hex literal | It vanishes from the site list. Compiles, runs, counts, and is never probed. Lint for it -- and note the lint does not yet cover `game_native.c`. |
| A mute or config byte read in a hot path | Hardcoding its recorded value passes every check. Whole features hide behind one byte nothing varies. |
| Dropping `CYCLES` when code moves to real C | Frames are a cycle budget. Charge without probing (`CYCLES_EDGE`) or every later frame hash moves. |
| Converting one path of a shared block head | The other path keeps it in the site list; the engines then disagree and the count looks fine. |
| A hand-written file outside the site-list grep | Fine for real C, fatal for a `CYCLES`. Lint that the two files use different spellings. |
| Coverage reported as a list of addresses | Group by feature. "The joystick" is actionable; forty hex numbers are not. |
| Guessing whether a register is live after a call | `apple2tc --ir` prints per-function `LiveIn`/`LiveOut`. Reading the caller by hand is four routines of tracing for a fact the tool already computed. |
| A block covered by a recording that never varies its inputs | Coverage says entered, not tested. Two identical lookup tables make reading the wrong one free. |
| Converting a routine that contains an input-coordinate address | The replay counter stops advancing there and every later keystroke lands elsewhere. Keep the probe, and lint that only coordinate addresses do. |
| `$(cmd | grep ...)` under `set -e` with `pipefail` | No match exits 1 and kills the script with no message. A check that legitimately finds nothing needs `|| true`. |
| A verification script that ignores its arguments | `./verify.sh <build-dir>` read `$BIN` only and silently tested a stale directory -- four false PASSes. Scripts that gate anything must reject arguments they do not understand. |
| Narrowing a probe and a storage move in one step | The narrowing then proves nothing: it is indistinguishable from a hash over bytes nobody writes. Move first with the probe untouched and require *trace PASS, screen PASS, memory FAIL* -- that failure is the evidence the oracle was watching. Then narrow, then mutate the new edges. |
| A store kept "for faithfulness" once its byte leaves RAM | Often it was scaffolding. Seven here were computed in a local, committed through the struct the routine was handed, and mirrored to an address nothing read. Grep for readers before keeping one. |
| `-Wall` treated as the dead-store detector | It says nothing about a dead `ram_poke`, and nothing about a **file-scope static** written and never read. One was written at fourteen sites, read at none, and every warning and every gate stayed green. `-Wunused-but-set-variable` covers locals only. |
| "In the ROM, registers are the algorithm" | A hypothesis. Checked on Snake Byte's flags it held for `C` and `D` and was false for `B`, `I`, `V`, `N` and `Z` -- 115 of 222 references. Classify every site as a read or a write and ask which reads each write can reach. |
| A flag pushed by PHP and restored by PLP | Not necessarily live. If the only reader of a bit is the PLP putting it back into the flag it came from, the bit is dead however faithful it looks. Check each bit of the pushed byte separately: they do not all have the same answer. |
| Threading a global and deleting its writes in one commit | A mistake in either gets attributed to the other, and the intermediate state -- written, never read -- is exactly what no warning catches. Thread, gate, then delete. |
| An inherited value inferred from the call site | Mutate it. Two inherited inks were inferred here; one is caught only by the scenario that presses `C`, and would have been wrong in silence under the other. |
| Deleting a store to test whether it is dead | Poison it instead -- assign a wrong value and run the gate. A deleted store leaves the *previous* value behind, which can coincidentally be the one a reader wanted; a poisoned one cannot. 42 residue writes were cleared this way in two batches. |
| A live-out claim sourced to `apple2tc --ir` | It was true of the *generated* program. Once the caller that did the reading becomes hand-written C, the claim expires silently and the comment asserting it stays. Re-derive against the C. |
| Reading a routine's body to find its parameters | Follow the calls. `rom_cout` reads X and Y as well as A, because it dispatches through `JMP ($36)` into a hook that reads them -- invisible in its own body. A read-before-write fixpoint over the call graph is a dozen lines and gets it right. |
| A counter the original never initialises | It still has a value. Assert what you think it is over full runs of every scenario rather than deleting the dependency or preserving it blindly, then pass it as an argument and record that it was measured, not proved. |
| A register-liveness claim you produced by reading | Produce it with the fixpoint instead. Two claims here were made by reading a routine's body and both were wrong the same way -- the answer was in a callee. `rom_cout` reads X and Y only through the vector it dispatches through; the carry looked live across a call and was written by the callee every time. Knowing the failure mode did not prevent the second one. |
| A flag that survives an audit | Ask what it is *for* before keeping it. Snake Byte's carry survived one audit as "genuinely crosses a call", then turned out to be three specific edges of one idiom: a routine ending on a CMP whose caller reaches an ADC with no CLC. Those are return values. The global was hiding a data dependency behind something that looked like residue. |
| A `\file` comment after a merge | It describes the file it was written for. One here still explained the adapter split -- marshalling, accessors, "variables live at their original addresses" -- directly above the code that had eliminated all three. Sweep the header whenever a file absorbs another. |
| A table sized to the data it visibly holds | Size it to the next code address. Snake Byte's shape masks hold 27 shapes ending at $61DF, and one call site computes a shape from a lo-res cell value -- reaching four entries further, into the zero padding before the code at $6200. Sizing to the data, or to the maximum a recording reaches, is an out-of-bounds read waiting for an unexercised path. |
| The gate as a check on lookup-table contents | It only covers entries something reads. Compare the array against the binary byte for byte; it is a dozen lines of script and it is the only thing that covers the rest. |
| "It is the image, not a variable" as a reason to leave an address | Check whether it is really a reason. Here it was standing in for "I have not derived the extent" -- a lookup indexed by a small integer is an array, and nine of the twelve extents were already written down in known-data.txt. |
