# Neptune

An Apple II game from 1982, by Nasir Gebelli for Gebelli Software. This
directory is a decompilation in progress, following
`docs/decompiling/playbook.md`.

`docs/neptune.md` is the other half: how the game plays, how to drive it, and
what has been learned about it as a *game*. This file is about turning it into
C.

## Start here

```
testdata/neptune.b33      the game, already loaded, as a self-contained binary
testdata/neptune.json     the recorded play session, for --run-data
testdata/play2.keys       that session as recorded against a disk boot
testdata/play2-cold.keys  the same, rebased onto the b33
testdata/known-data.txt   regions declared to be data, each with its derivation
testdata/code-at.txt      hand-asserted control-flow edges, each with its argument
testdata/coverage.txt     what is identified and what is not

as-generated/neptune.lst          the disassembly
as-generated/neptune-simple-c.c   the --simple-c transliteration (builds, runs)
as-generated/neptunec1.c          the -O3 --irc1 decompilation (builds, runs)

scripts/make-b33.sh    capture the machine and build the b33
scripts/collect.sh     replay the recording against it and collect run data
scripts/decompile.sh   generate the listing and both C outputs
scripts/scope.sh       the playbook's step 1 measurements
```

Three builds, all verified to reach `$3535` with byte-identical RAM across
`$0000-$BFFF` after 5.75 million cycles:

| build | what it is |
| --- | --- |
| `a2emu testdata/neptune.b33` | the interpreter running the captured image |
| `neptune-as-generated` | `--simple-c`, a direct 6502→C transliteration |
| `neptunec1` | `-O3 --irc1`, through the IR with SSA, DCE and simplification |

## Step 1: the boundary

*Reproduce with `scripts/scope.sh`. Derived from the instruction stream, not
from `Code range` markers — the playbook is explicit that those measure
fragmentation rather than quantity.*

| | Snake Byte | Neptune |
| --- | --- | --- |
| game's share of disassembled instructions | the minority | **98.8%** (4032 of 4080) |
| distinct ROM entry points called | 9 | **1** |
| hardware surface | 7 addresses | 10 addresses |

**The ROM is already gone, and that is the b33's doing.** The playbook's step 1
warns that entry-point choice dominates output size: Snake Byte traces from the
reset vector, so it boots Applesoft and most of its output is ROM. It also
warns that removing the ROM needs *two* cuts — externalizing the entry points
and retargeting the start PC — which are "strongly non-additive: cutting either
alone removes little, cutting both removes nearly all of it."

Neptune arrives with the second cut already made. `neptune.b33` starts at
`$3523`, inside the game, with the machine state a completed disk load leaves.
So 48 ROM instructions remain out of 4080, against Snake Byte's ~1,430 blocks.

**The one entry point is `$FCA8`, `MON_WAIT`, called from 12 sites.** It is a
delay loop and nothing else, which makes step 4a unusually cheap here.

**Most of the remaining ROM was never executed.** The listing is a static
trace, so it follows `BRK` into the interrupt handler and reaches
`$DB5C`, `$FA40-$FA56`, `$FDED` and `$FF4C-$FF58`. The recording reached only
`$FCA8-$FCAA` and `$DAFB`, and `$DAFB` is not Neptune's either: its recorded
predecessor is `$D43C`, the BASIC warm restart the interpreter breakpoints in
order to load the b33. It is an artifact of *how the b33 is loaded*, and the
c1 build — which starts at `$0000` directly — never touches it.

### The hardware surface

| address | | sites |
| --- | --- | --- |
| `$C000` | keyboard latch | 7 |
| `$C010` | keyboard strobe | 11 |
| `$C030` | speaker | 21 |
| `$C050` `$C051` `$C052` `$C055` `$C057` | video mode, page, mixed | 12 |
| `$C061` `$C062` | joystick buttons 0 and 1 | 4 |

Ten addresses across a 48KB image, so most of the work is pure logic with no
emulation dependency. Two notes on the ends of that list:

- **`$C030`, the speaker, is the busiest thing here at 21 sites.** Snake Byte
  touches no speaker at all. Sound is going to be a real part of this
  conversion rather than a footnote, and `$C030` is timing-sensitive by
  construction — a speaker click is a *cycle*, not a value.
- **`$C061`/`$C062` are not emulated.** `lib/a2io` returns 0 for the game I/O
  strobes, which reads as "no button held" (dz `01a074d9`). Neptune reads them
  about once per attract cycle. Nothing observed so far depends on the answer,
  but the code that does is in scope and cannot be exercised.

## Step 2: recover procedures

*Reproduce with `apple2tc ... -O3 --irc1 --routines-report=<path>`. The report
already says why every candidate was rejected; the playbook is explicit that
you read it rather than write a script to infer it.*

**85 accepted, 13 rejected.** All 13 triage cleanly, and none of them is
decompiler over-strictness:

| rejected | why | call sites |
| --- | --- | --- |
| `$1100` | `terminator JmpInd` | 8 |
| `$0E00` `$1300` `$1380` | `terminator JmpInd` | 3 each |
| `$0F00` `$1060` `$1B80` `$24D0` | `terminator JmpInd` | 1 each |
| `$2150` | cascade: calls `$0F00` | 1 |
| `$3340` | cascade: calls the synthetic interrupt block | 1 |
| `$DB5C` `$FDED` | ROM; `$DB5C` is the b33-loading artifact | — |
| `$10100` | the synthetic interrupt-entry block | — |

Measured, not assumed: `--alt-exit`, `--prune-returns` and `--ret-addr` in
every combination all leave it at exactly 85/13. `--alt-exit` is the playbook's
remedy for Snake Byte's `$6A32`, and this is not that shape.

### The shape the rejections describe

**Neptune's top level is four main loops, not one.** `$2000`, `$2080`, `$2100`
and `$2180`, each ending by jumping through a zero-page vector:

| vector | value | loop |
| --- | --- | --- |
| `$36/$37` | `$2000` | reached from `$1F9A`, `$2062` |
| `$38/$39` | `$2080` | reached from `$20D4` |
| `$4E/$4F` | `$2100` | reached from `$2146` |
| `$3A/$3B` | `$2180` | reached from `$218F` |

The eight `JmpInd` rejections are the subsystems those loops call. They are
rejected because they do not cleanly return: their control flow runs past their
own `RTS` — `$1380`'s exits at `$13B7`, but blocks after it continue into
shared tails — and reaches the loops' indirect dispatch. That is a real
property of the program, not a classifier being fussy, and it is what step 5
will have to express.

### A finding worth having, deliberately not acted on

`BNE X` immediately followed by `BEQ X` is an unconditional transfer — one of
the two always fires, and nothing between them can change the flags. Neptune
uses the idiom 8 times (`$0E72` `$0E77` `$0E8E` `$0F72` `$0F77` `$0F8E`
`$1B5C` `$22BF`), and the tracer does not model it, so it decodes the padding
that follows as code:

- 11 bogus `BRK` instructions,
- **40 of the 48 ROM instructions** in the listing, because `BRK` is modelled
  as a call into a synthesised interrupt block — none of it ever executed,
- 2 of the 13 rejections above (`$10100` and its cascade `$3340`).

Filed as dz `01a07f1a-09bb` and left alone. Suppressing the fall-through is
only sound when nothing else can reach the second branch — a `JMP` into it
would make the fall-through live again — which needs predecessor information
the tracer does not have until after the padding is already decoded. Step 4
removes that ROM anyway. Snake Byte has zero such pairs, so nothing there is
affected either way.

## Step 3: the oracle

`scripts/verify.sh` is the gate. Everything after this point is trusted because
it still passes.

```
./scripts/verify.sh              check the build against the golden traces
./scripts/verify.sh --record     re-record them (refuses unless two runs agree)
./scripts/verify.sh --coverage   report what the scenarios actually reach
```

It compares per-frame hashes of video *memory* — mode bits plus the text and
hi-res pages — not rendered pixels: memory hashing is more sensitive, needs no
graphics context so it runs headless, and is reproducible where rendering is
not (blink phase comes from wall-clock time). Each line also carries the cycle
count, so the trace is a timing check and any restructuring that changes a
`CYCLES()` total is caught here.

### It can fail, which was checked three ways

| mutation | result |
| --- | --- |
| one hash corrupted in `play.frames` | **FAIL**, naming the frame |
| `s_y = 0x30` → `0x31` at `$133D`, a block the recording executes | **FAIL** at frame 1527 |
| `s_a = 0x40` → `0x41` at `$28E6` | **PASS** — and correctly so |

The third is the useful one. `$28E6` is never executed: nothing in
`$2800-$2A00` appears in the recording at all. A green result there is the
oracle telling the truth about its own reach, which is why the coverage numbers
below are measured rather than assumed.

### What it covers

| scenario | frames | distinct states | blocks reached |
| --- | --- | --- | --- |
| attract | 1200 | 956 (80%) | 314 of 1085 (29%) |
| play | 1929 | 1730 (90%) | 465 of 1085 (43%) |
| union | | | **467 of 1085 (43%)** |

At function level, **40 of the 83** generated functions have their entry block
reached.

*(These figures were wrong when first measured, and the way they were wrong is
the point of `--coverage` now taking its sites from `CYCLES()` calls. The first
attempt listed the dispatch switch's `case N: // $XXXX` labels, which excludes
every block inside the 84 functions — so those addresses reported zero hits
while executing, and "never runs" got concluded about code that runs. A probe
that was never installed cannot fail, and an uninstalled probe reads exactly
like a covered one.)*

Two things that surprise and are worth stating rather than hiding:

- **`attract` adds only 2 blocks over `play`.** It is almost a subset. Its
  value is not coverage: it is the only scenario that needs no key file, so it
  keeps working if the key coordinate ever breaks — which, given cycle stamps
  do not survive a change of engine, is a live risk rather than a hypothetical.
- **57% of the decompiled blocks are never executed by the gate.** The
  unreached functions cluster in `$2200-$3800` and `$AC00-$B9A0`. Every
  later step has to be read in that light: a change to code in those ranges is
  transcription that the oracle cannot check, and the playbook's rule for that
  is to decode by hand and validate against a *scratch* run-data rather than
  edit the committed recording.

The traces are dense by Snake Byte's standards — its 1,300-frame trace has few
distinct hashes, dominated by BASIC-prompt keystrokes and title plateaus, where
Neptune's are 80% and 90% distinct with the tail still moving in both. That is
the b33 again: the trace starts inside the game instead of spending its first
seconds at a prompt.

The one static stretch worth naming is `play` frames 594–677, 84 frames of
frozen screen. Rendering that moment shows the terrain and one enemy with **no
player ship**, so it is a death-and-respawn pause rather than a hang.

## Step 4: cut the library boundary

### 4a. Externalize the entry points

One line in `testdata/rom.externs`, because step 1 found one entry point:

```
FCA8 rom_mon_wait
```

`reference/rom.c` supplies it by hand; `reference/neptune-ext.c` stitches it
into the generated code's translation unit, which is necessary because
`system2-inc.h` *defines* the machine with internal linkage. Built as
`neptunec1-ext`.

| | blocks | distinct ROM addresses |
| --- | --- | --- |
| generated | 1904 | 46 |
| `--extern-routines` + `--prune-returns` | **1853** | **22** |

`--prune-returns` is not optional here and the playbook says why: externalizing
a routine erases the `JSR`s that called it, but an `RTS` carries its whole
observed return set as successors, so the ROM stays reachable through returns
nothing can produce any more.

**The 22 ROM addresses that remain are `$FA40-$FA56` and `$FF4C-$FF58` — the
BRK/interrupt path, entirely.** So externalizing `$FCA8` removes every piece of
ROM the game actually calls, and what is left is the tracing artifact from
dz `01a07f1a-09bb`, not a dependency.

`rom_mon_wait` keeps the 6502's loop structure rather than computing the delay
in closed form, and that is deliberate: `CYCLES` yields to the host when the
budget runs out, so *when* cycles are charged decides where a frame boundary
falls. Charging 20,000 at once would move every subsequent frame hash.

`verify.sh` now gates both builds against the same traces, which is what proves
the hand-written replacement is equivalent — cycle counts included, since those
are on every line. Both pass all 3129 frames. Mutating one `CYCLES` value in
`rom.c` from 4 to 5 is caught at **frame 1**, with the reference build still
passing.

### 4b. Retarget the entry point — already done

This is the step Snake Byte found expensive, and Neptune arrived with it
finished: `neptune.b33` is a snapshot taken at `$3523` *from the disk boot's own
arrival there*, soft switches included, which is exactly what 4b prescribes. See
`scripts/make-b33.sh`.

The playbook lists three traps in that step. One of them applies here and
resolves in Neptune's favour:

> "The host injects state the boot used to hide. `a2host` pushes a dummy
> keystroke whenever a key source is present... A cold-started program reads it
> as the player's first keypress."

Snake Byte's cold build ingested that dummy and diverged immediately. Neptune
does not, and the reason is the capture point: **`$3523` is `STA KBDSTRB`**, so
the game's very first instruction clears the latch the host just filled.
Checked rather than assumed — replaying `attract` with an empty key file
present, which is enough to trigger the injection, produces a byte-identical
trace.

That was luck. `$3523` was chosen because it is where space starts a game, not
for this.

## Step 5: structural conversion — started, not finished

`neptune.c` is the artifact, gated alongside the two generated builds by
`scripts/verify.sh`. Two mechanical passes have run, both verified:

| pass | effect |
| --- | --- |
| dead `bb_N:` labels removed | 453 labels; `same-code.sh` confirms identical instructions |
| simple-if and if/else diamond relooped | 9 sites, then 7 more labels fell out |

**18,886 → 18,431 lines.** That is a small dent, and the reason is worth
stating plainly rather than working around: a relooper that is *obviously*
right only fires where the target is the next label, the body has no labels of
its own, and nothing else jumps in. The generated code rarely looks like that —
`branchTarget` assignments and `CYCLES_EDGE` charges interleave with the
control flow, and the playbook's own warning applies at every site:

> "Which side of a branch pays the extra cycle depends on the branch... Writing
> all eight the same way was one decision and seven bugs."

That is a judgement per site, not a pattern match.

### What remains, measured

| | |
| --- | --- |
| live `bb_N:` labels across 84 functions | **727** |
| cases in the `run_emulated` dispatch switch | **1,853** |
| functions with 16+ labels | 21, led by `func_1490` at 123 |

The switch is the bulk, and shrinking it means recovering the eight routines
step 2 rejected — the ones that do not cleanly return, because control runs
past their `RTS` into the four main loops' indirect dispatch. That is hand
work, and it is what the rest of this step is.

### `testdata/labels.txt`

The names earned so far, each with its evidence, following the playbook's rule
that "a name asserting something unproven is worse than no name". It makes the
disassembly readable now and is the working list for step 6.1:

```
title_wait_for_input:
3523: STA KBDSTRB
3526: LDX #$00
title_wait_loop:
3528: JSR poll_input_select_mode
352B: BMI title_input_seen
352F: JSR delay_one_tick
3533: BNE title_wait_loop
title_timed_out:
```

The substantive find behind those names is `$3584`, which reads the joystick
buttons and the keyboard and then *writes code* into three input sites,
choosing the game's input device once at the title screen. That is the
self-modification the collector reports as 21 generations. See the *Why it
reads the joystick buttons* section of `docs/neptune.md`, including the one
part of it that does not yet add up.
