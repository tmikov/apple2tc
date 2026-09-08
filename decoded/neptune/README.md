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
