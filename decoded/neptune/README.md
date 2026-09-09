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
| hardware surface | 7 addresses | 12 addresses |

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
| `$C070` `$C064` | PTRIG and PADDL0, in the paddle reader | 2 |

**The last row was missing, and reading the table as the surface was a
mistake.** It is derived from the disassembly, and the disassembly only covers
code the tracer reached. Neptune picks its input device once at the title
screen and *writes* its input routines; the joystick arm is never taken in any
scenario here, so the paddle reader at `$36B0` was never decoded, and the
surface was published as ten addresses and "no paddle reads".

Two things fixed it, and the order matters. First `scripts/scope.sh` learned to
scan the raw `.b33` as well — absolute operands in `$C000-$C0FF`, matched
against a byte-pattern table of every absolute load, store, compare and ALU
opcode. That over-reports, since misaligned data matches too, which is the
right direction for a bound to err in. It finds **25** and named the gap.

Then the gap was closed rather than annotated: `--code-at` grew a *root* form
(dz `01a08403-7af0`), a lone address asserting that bytes are code without
claiming what reaches them, which is the only honest thing to say about a
routine whose only caller is a byte pattern another routine stores. `$36B0` is
now disassembled, and the listing-derived surface is 12. What the image scan
still reports beyond that is accounted for:

| | |
| --- | --- |
| `$C081` `$C083` `$C088`-`$C08F` | real, at `$BC00-$BFFF` — the disk loader, `LDA $C08C,X` with X = slot×16; dead once the b33 enters at `$3523` |
| `$C01F` `$C04C` `$C0C8` | data. `$1E31` runs into `BRK` and zeros, `$7616` into `$FF $FF` |

The scan stays in `scope.sh`. It is what would have caught this, and the next
game will have its own untraced arm.

Two notes on the rest of the list:

- **`$C030`, the speaker, is the busiest thing here at 21 sites.** Snake Byte
  touches no speaker at all. Sound is going to be a real part of this
  conversion rather than a footnote, and `$C030` is timing-sensitive by
  construction — a speaker click is a *cycle*, not a value.
- **`$C061`/`$C062` were not emulated, and now are.** `lib/a2io` used to return
  0 for the game I/O strobes, reading as "no button held" (dz `01a074d9`), so
  the joystick arm was in scope and could not be exercised. The game port is
  emulated on `master` as of `lib/a2host/joystick.c`.

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

| scenario | frames | blocks reached |
| --- | --- | --- |
| attract | 1200 | 314 of 1085 (28%) |
| play | 1929 | 465 of 1085 (42%) |
| death | 8000 | 539 of 1085 (49%) |
| union | | **539 of 1085 (49%)** |

**`death` was derived, not recorded.** It is the same key file run four times
as long: the recording simply stops, the player stops acting, and the game
kills them. `SH:0` and an explosion arrive around frame 8000, then the game
returns to the title and restarts the attract cycle — `$3523` fires 11 times
over 20,000 frames. That is +74 blocks for the cost of running longer, and it
needed nobody to play anything.

It was calibrated the way the playbook insists a new scenario must be, because
a scenario can buy trace coverage and no state coverage at all: mutating
`s_x = 0x04` at `$3642`, a block only `death` reaches, fails `death` and passes
`attract` and `play`.

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
- **51% of the decompiled blocks are never executed by the gate**, and
  coverage *saturates* there: running 20,000 frames instead of 8,000 adds
  nothing at all. The
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
| cases in the `run_emulated` dispatch switch | **1,853** (1,620 distinct addresses) |
| functions with 16+ labels | 21, led by `func_1490` at 123 |

### The switch is not what it looks like

The obvious reading is that the switch holds the code no routine was recovered
for. Measured, that is only a third of it:

| | |
| --- | --- |
| addresses **also** inside a recovered function | **1,006** (62%) |
| addresses only in the switch | 614 (38%) |
| duplicate cases, same address more than once | 231 |

`$1000` is an accepted routine, is emitted as `func_1000`, **and** has all 15
of its blocks emitted again as switch cases. Every recovered routine is in
there twice.

The reason is dynamic dispatch. `func_t001` — the switch — is a fallback
interpreter, and 147 sites in the program compute their next block through
`find_block_id_func_t001()`. **134 of those are `RTS`**: a return whose address
is not statically known has to be able to land anywhere, so the address-to-block
map must cover every block, including the ones already extracted into functions.

That changes what the remaining work buys. Recovering the eight rejected
routines removes the 614 switch-only addresses — the smaller half, and the
ones concentrated in `$0E00-$1Bxx` and `$2000-$21xx`, which is exactly those
routines and the four main loops. **The larger half goes when `RTS` becomes a
real C `return`**, because then the map has nothing to look up: the only
remaining computed jumps are the six indirect ones, and step 2 established
that all six resolve to fixed addresses.

So the two halves of step 5 are not independent, and relooping pays twice —
once in the function it structures, and again in the duplicate copy of that
function it lets the switch drop.

### What actually holds the switch up: 90 addresses

`find_block_id_func_t001()` is the address-to-block map, and it has **90
entries**. Those are the only addresses control can enter the switch at from
outside; every other case is reached by chaining (`block_id = N; break;`) from
one of them. They are `JSR` return sites — `$352B` follows
`JSR poll_input_select_mode` at `$3528`, `$3532` follows `JSR delay_one_tick`
at `$352F`.

| | |
| --- | --- |
| entry points inside a recovered function | 45 |
| entry points outside any function | **45** |
| of those, in the four main loops `$2000-$2186` | **24** |

That is the whole remaining problem stated in one number. It is not "reloop 84
functions" — those are already real C. It is: **make 45 return sites static**,
and the map has nothing to look up, and the switch collapses the way Snake
Byte's did (23,634 lines → 4,206, with zero cases and zero `find_block_id`).

### The order it has to be done in, and the cycle in it

The four main loops are short and readable. `$2080` is a per-frame update that
runs two ticks and a slower counter:

```
2080: JSR $0E00 / JSR $1100 / DEC $D4 ... STA SPKR / JSR $1380
20A4: JSR $1100 / DEC $D4 ... STA SPKR / JSR $1380      <- second tick
20C2: DEC $20 ... JSR $1B80 / JSR $1738                 <- every other frame
20D4: JMP ($38)                                         -> itself
```

But they cannot be written as C until their callees are C functions, and five
of those — `$0E00`, `$1100`, `$1300`, `$1380`, `$1B80` — are exactly the ones
step 2 rejected. That is the cycle: the callees are rejected *because* control
runs past their `RTS` into the loops' indirect dispatch, and the loops cannot
be converted while the callees are switch blocks.

Breaking it means hand-recovering those five, which is the playbook's "recover
the remaining procedures by hand, rewriting idiom-based ones" and the reason
that sentence comes before "reloop each function". They are not simple
transcription: `$1380` branches *forward past its own `RTS`* at `$13B7` into
shared tails, so deciding what its C signature even is — does it return, or
does it transfer? — is the work.

**So the remaining sequence is:** recover the five rejected callees by hand →
convert the four main loops → the 24 top-level entry points go → repeat for the
remaining 21 outside-function entry points → the map empties and the switch
goes with it.

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
reads the joystick buttons* section of `docs/neptune.md` for the derivation,
and for the reading that was inverted there until it was checked: **one**
button held selects the stick, not both.

### A lead worth checking before the hand work starts

The 90-entry map may be larger than it needs to be. A `JSR` from a switch block
to a recovered function is emitted with a **static** continuation:

```c
case 409:  // $141B
  func_1490(0x141d);
  branchTarget = true; block_id = 410;   // static, not a map lookup
```

and yet `$141E` — block 410 — still has a map entry. Nothing in that sequence
needs one. The map looks to be built from every *observed* return site rather
than from the sites that actually require dynamic lookup, in which case some of
the 90 are removable without any hand conversion at all, and the switch cases
they keep alive go with them.

Worth measuring before starting the five routines, because if it is true it is
a decompiler change that does part of the job mechanically, and the playbook's
own advice applies: "deleting unreachable text cannot move a frame hash, a
memory sample or a trace, so every oracle must stay exactly where it was."

**Measured.** Logging every `addr_to_block_id()` lookup in the artifact and
replaying both scenarios: **12 of the 90 entries are ever used**, from 7,528
lookups. And they are almost all one thing:

| entry | lookups | what |
| --- | --- | --- |
| `$2000` `$2003` `$2006` `$201B` `$2021` | ~1,200 each | `main_loop_via_36` and its per-frame call returns |
| `$1132` | 1,218 | a return inside `$1100` |
| `$10AE` | 424 | a return inside `$1060` |
| `$132F` `$13B7` | ~200 each | the `RTS` of `$1300` and of `$1380`, the gun |
| `$0100` `$1D00` `$1F6E` | 1–2 | one-offs, including the b33 stub's entry |

So the live dynamic dispatch is **one main loop and the four rejected routines
it calls**. Converting `$2000` plus `$1060`, `$1100`, `$1300` and `$1380`
removes 9 of the 12. The other three main loops never execute under the gate at
all — they are other game states.

That does not mean the remaining 78 entries are dead: the gate reaches 43% of
blocks, and the unused entries are presumably the other states' returns. It
means the conversion has an order that traffic, not structure, chose — and the
first five things to convert are now named rather than estimated.

## The first routine, analysed to the point of conversion

`$1380` — `fire_control`, the player's gun — is the smallest of the five and
was taken as far as the edit before stopping. It is worth writing down because
what it found is not specific to this routine.

**Its block set is perfectly closed.** 60 switch cases, `$1380-$1487`, and
measured rather than assumed:

| | |
| --- | --- |
| `block_id` targets leaving the set | **0** |
| calls out | `func_1490` only, already a real function |
| call sites in | 3 — `$201B`, `$209B`, `$20BC`, i.e. the main loops |
| dynamic-return blocks | `$13B7`, `$1487` (`pop16() + 1`) |

So the classifier's rejection was wrong about the shape: this *is* a routine
with two returns, and extracting it into a C function is mechanical.

**Except for one block, and that block is the whole lesson.** `$1391` also
dispatches dynamically, but not through `pop16()`:

```c
block_id = find_block_id_func_t001(0x1391, (0x1393 + (int8_t)ram_peek(0x1392)));
```

`$1392` is a branch's *operand byte*, and `poll_input_select_mode` writes it —
`ram_poke(0x1392, 0x24)` in the keyboard arm, while the joystick arm rewrites
`$138F-$1391` into different instructions altogether. **The routine's control
flow shape depends on which input device was chosen at the title screen.**

In keyboard mode the target computes to `$13B7`, inside the routine, and the
extraction closes. In joystick mode it does not — so an extraction that assumed
the keyboard shape would pass every oracle and still be wrong for the other
mode.

That is the playbook's own warning arriving in a new form: *"a relaxation that
passes every oracle can still be wrong, and the way to tell is structural."*
The honest shapes are to keep `$1391` as a dispatch the function can exit
through, or to make the input mode an explicit parameter of the program rather
than a property of its instruction bytes.

**When this was written the second shape could not be measured at all**, because
`$C061`/`$C062` returned 0 and the joystick arm was unreachable under any
recording. That is no longer true: the emulated game port on `master` makes the
mode selectable, so the right move is a fourth scenario that holds one button at
the title screen and pins the joystick shape against a real oracle, rather than
choosing between the two shapes on judgement.

## What the top level actually is: a state machine

This is the finding that decides the shape of the C, and it came out of
`--routines-report` once it started printing the blocks a *rejected* candidate
had walked.

| rejected | blocks walked | |
| --- | --- | --- |
| `$1060` | 3 | is the patched input site |
| `$1300` | 3 | is the patched input site |
| `$1380` | 14 | stops at the computed dispatch `$1391` |
| `$1100` | 40 | ends at `$11F0`, `JMP ($0200)` |
| `$0E00` | **152** | includes `$2080`, `$2100`, `$2180` |
| `$0F00` | **136** | the same |

`$0E00`'s walk reaching three different main loops is not the walk leaking. It
is real, and traced to a single instruction:

```
1A4C: JMP $2079          <- inside $0E00's block set
...
2079: LDA #$01
207B: STA $A0            <- the state variable
207D: JSR $3800
main_loop_via_38:
2080: JSR $0E00 ...      <- and now this loop runs instead
```

So `$0E00` does not fail to return. **It hands over to a different top-level
loop**, and never comes back. The four loops at `$2000`, `$2080`, `$2100` and
`$2180` are game states; `$A0` is a state variable they set; and the
"subsystems" the loops call can each end the current state and start another.

Three things follow, and together they are why this step is not transcription.

**`--alt-exit` was never going to help.** That flag is for a routine that
unwinds into its *caller* — Snake Byte's `$6A32` doing `PLA / PLA / JMP`. This
is a routine that transfers to a different *top level*. Measuring that the flag
changed nothing was right; the reason is that it is a different phenomenon.

**These are not functions and should not be written as functions.** A C shape
that makes `$0E00` return a value the caller switches on would be inventing a
contract the 6502 does not have — `$1A4C` is an unconditional jump, so there is
no caller left to return to. The honest shapes are a state machine
(`for (;;) state = run_state(state);`) or explicit tail calls, and choosing
between them is a design decision about the artifact rather than a
transcription of the binary.

**It explains the dispatch switch.** A `JMP` into the middle of another loop is
exactly what an address-to-block map is for, and it is why the switch cannot be
dismantled routine by routine from the bottom up: the top level has to be
expressed first, because the "leaf" routines jump into it.

## Why half the program is unreachable: the state machine is never entered

Coverage saturates at 49% and the missing half is not scattered — it is three
of the four main loops and everything they call. This is the account of why,
and it ends in a question for someone who has played the game.

**`$A0` selects the state**, at `$1F96`:

```
1F96: LDA $A0
1F98: BNE $1F9D
1F9A: JMP ($36)     ; $A0 == 0 -> main_loop_via_36     <- the only one ever seen
1F9D: CMP #$01
1F9F: BNE $1FC2     ; $A0 >= 2 -> $1FC2, which sets $F5 = 2
1FA1: ...           ; $A0 == 1 -> $1FB0 -> $1A20 -> main_loop_via_38
```

**Every writer of `$A0` outside the loops sets it to zero.** There are three —
`$355F`, `$2869`, `$ACC0` — and all three are `LDA #$00 / STA $A0 / JMP $1F00`.
The five writers that set 1 or 2 are all *inside* the loops they perpetuate, so
they cannot start a state, only continue it.

That leaves exactly one non-circular way into state 1: `$0EA6`, inside `$0E00`,
whose guard is

```
0E9A: LDA $F0 / CMP $F2 / BNE $0EA9      ; a progress counter against a target
0EA0: LDA $F1 / CMP $F3 / BNE $0EA9
0EA6: JMP $1A20                          ; equal -> end of level, next state
```

`$F0/$F1` advances as the game runs; `$F2/$F3` is the target it is compared
against. **At the b33's capture point `$F0`, `$F1`, `$F2`, `$F3` and `$F5` are
all `$FF`** — none of them is in the live zero-page set, so nothing wrote them.
The code that *would* set the target, `$1A3C` and `$1A40` writing `$48` and
`$89`, sits inside `$1A20` — which only runs once a level has already ended.

So the level-end target is whatever uninitialised memory happens to hold, and
this emulator makes that `$FFFF`: `lib/cpuemu/emu6502.cpp` does
`memset(ram_, 0xFF, 0x10000)`, and `system-inc.h` does the same for the
generated engine.

**That looked like the cause and is not.** Rebuilding with
`memset(ram_, 0x00, 0x10000)` and booting the disk with the same recording
produces `$F0`-`$FA` all zero — `$F5` included, so the guard at `$1A22` would
fall through — and changes nothing at all: `$1A20`, `$1A4C`, `$2079`, `$2080`,
`$2100` and `$2180` still have zero hits, and the run reaches **511 blocks
under either fill**. The blocker is the path *to* `$1A20`, not the guard inside
it. The hypothesis was worth testing and it is dead; the RAM fill is not why
half the program is unreachable.

Measured, so that none of this rests on the reading alone:

| | |
| --- | --- |
| `$1A20`, `$1A50`, `$1FB0`, `$1FC2`, `$1A4C`, `$2079` | 0 hits, all three scenarios |
| `$2080`, `$2100`, `$2180` | 0 hits |
| `$0E00` | 630 / 1219 / more — the routine itself runs constantly |
| patching `$F5` to `$00` in the b33 | changes nothing; `$1A20` is not reached to test it |

### The question this raises

**Does a level ever end?** Every scenario here plays until the ship dies, and
none of them finishes a level — the progress counter at `$F0/$F1` never meets
its target. So the honest question is not about hardware or RAM fills, both of
which have been tested, but about the game: is level 1 completable, and if so
what completes it — surviving a distance, a score, clearing a wave?

Whatever the answer, it names an input sequence rather than a machine
configuration, which means it can be turned into a fourth scenario and the
gate can then reach the other three states. Until then the code behind them is
transcription that no oracle can check.
