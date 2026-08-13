# Accurate 6502 cycle counts, shared by both engines

Date: 2026-08-12

## The problem

Both engines invent cycle costs, and they invent them differently.

| | how a cycle charge is computed | where |
|---|---|---|
| interpreter | flat `cycles_ += 3` per instruction, whatever the opcode | `lib/cpuemu/emu6502.cpp:112` |
| generated C | `lround(block.size() * 1.7 + 0.5)`, fixed at decompile time | `tools/apple2tc/GenIR.cpp:77`, `tools/apple2tc/PrintSimpleC.cpp:103` |

Both were deliberate placeholders. Neither was revisited.

### What it costs, measured

The ROM's keyboard-wait loop:

```
FD1B: INC $4E     zp INC        5 cycles
FD1D: BNE FD21    taken         3
FD21: BIT $C000   abs BIT       4
FD24: BPL FD1B    taken         3
                             = 15 real
```

The interpreter charges 12 (−20%); the generated engine charges 16 (+6.7%). Note
which way round that is: **the byte-based estimate is the better of the two**,
because cycles track operand size and a flat constant carries no signal at all.

The consequence is not a rounding error. `a2host` runs each engine for a fixed
budget of *emulated cycles* — 17,051 per frame. Both engines consume that budget
to within 3 cycles out of 2,028,951. But the same budget buys different amounts
of execution, so over 120 frames of ROM boot the interpreter spins that loop
155,925 times and the generated program 115,899.

That is what makes the probe acceptance test fail
(`decoded/rom/probe-acceptance.sh`, and the 2026-08-12 decision-log entry). The
generated report is a byte-exact **prefix** of the interpreter's: 121 of 121
distinct addresses, in identical first-occurrence order — control flow agrees
completely — and the entire difference is dwell time in one loop.

### Why it is not just a measurement artefact

Code that terminates on its own is unaffected: both engines do the same work and
stop in the same place whatever they charged for it. The boot prefix matches hit
for hit, 24,366 of them.

Code that spins until interrupted is *entirely* determined by budget ÷ price. So
**any program with a main loop that never returns will show this** — which is
every game.

Worse, it leaks into program state. `$FD1B` is `INC $4E`, and `$4E`/`$4F` is the
Apple II's canonical pseudo-random seed. Same program point, both engines agree
(measured: `4E=1E 4F=4E` at the 20,000th hit on both). Same *cycle budget*, they
have incremented it a different number of times. A game that seeds from `$4E`
therefore starts from different random state in the two engines and diverges for
real, in control flow, permanently — not as an artefact of how we are looking.

**That is a live hypothesis for the frame-472 divergence** recorded on
2026-08-08: a permanent split 8 million cycles in, after long stretches of exact
agreement, is what a diverged RNG seed looks like. Untested.

## The decision

One cycle table, in `d6502`, from which both engines derive.

**Base cost per opcode** becomes a third field on `CPUOpcode`, alongside `kind`
and `addrMode`. `lib/d6502/opcodes.h` is already a *generated* table —
`genDisasmTable()` in `lib/d6502/gen.cpp` materialises it from
`decodeOpcodeSlow()` — so this extends machinery that exists rather than adding
a hand-maintained table that can rot.

Cost is nearly a function of `(kind, addrMode)`, with the exceptions that make it
worth deriving rather than tabulating by hand: read-modify-write instructions
cost more than loads at the same addressing mode (`ASL $nnnn,X` is 7, `LDA
$nnnn,X` is 4), and stores at indexed modes unconditionally pay the cycle that
loads pay only on a page crossing (`STA $nnnn,X` is 5).

**Branch penalties are modelled exactly, and cost nothing.** The table holds the
not-taken cost; the +1 for a taken branch is added where taken-ness is known —
at runtime in the interpreter, per edge in the decompiler, where it lands on the
taken arm of the emitted `if` as a constant. No runtime test on either side.

**Indexed page crossings are deliberately not modelled.** Both engines charge the
base cost. This is the one genuinely dynamic penalty left, and the reason to
leave it is specific: modelling it in generated C means emitting
`if (((base & 0xFF) + s_x) > 0xFF) s_cycles++;` at every indexed access —
reintroducing per-instruction runtime work, which is the exact thing
decompilation exists to remove. For scale, the single predictable branch added
to `CYCLES` for probe dispatch costs about 6% of host CPU.

The agreement between the engines does not depend on this being *right*. It
depends on it being *the same*, which a shared convention guarantees by
construction.

## What this is and is not

It is not cycle-exact emulation. After this change the model is exact except for
one cycle on indexed reads that cross a page — a minority of instructions,
bounded at 1 cycle each. Against a 20% error on a four-instruction loop the
machine spends most of its life in, that is a different regime.

It is also the first time `a2io`'s timing means anything. `DISK2_CYCLES_PER_NIBBLE`
is 32, derived in a comment from 1,023,000 Hz and 5 revolutions per second; the
speaker callback takes a cycle count and pitch is the delta between toggles.
Both are currently fed a number with no relationship to elapsed time.

## The structural risk, and the guard

Two consumers, one table: the decompiler sums it statically at decompile time,
the interpreter accumulates it per instruction at runtime. **If they ever drift,
today's bug returns silently** — there is no natural place it would surface.

The guard already exists: `decoded/rom/probe-acceptance.sh`. A probe at every
block head, run on both engines, reports diffed. It fails today for exactly this
reason, and it should pass when this lands. Keeping a cycle-printing probe in
that test makes the invariant checkable rather than aspirational.

## The one place to be careful

`emu6502.cpp:112` applies its charge in the **loop increment**:

```c
for (unsigned startCycles = cycles_; cycles_ - startCycles < runCycles; cycles_ += 3) {
```

which runs after the body, when `pc_` has already advanced. Rewriting it as
`cycles_ += table[ram_[pc_]]` would bill every instruction at its *successor's*
price. That is a plausible-looking model which is systematically wrong, and the
only symptom would be the acceptance test failing again, differently. The charge
has to move into the body against an opcode captured before dispatch.

## What regenerates

Everything that encodes a cycle count:

- 10 `.ir` regression baselines in `tests/` (54 `AddCycles` constants).
- The committed generated C: 1,977 `CYCLES()` calls in `decoded/rom/romc1.c`,
  2,203 in `decoded/snake-byte/snake-bytec1.c`, and the other decoded programs.
- `decoded/snake-byte/*.frames` — all four `verify.sh` scenarios.
- The cycle-stamped `.keys` recordings.
- `decoded/snake-byte/snake-byte.json`, the run data. The standing rule is never
  to *edit* it; regenerating it headlessly is precisely what the host/engine
  split made possible.

### The part that cannot be regenerated

`decoded/snake-byte/a2rom.c` is, by its own header, "hand-written replacements
for the Apple II ROM entry points", and it carries **82 hand-written cycle
constants** (`/*$F847*/ CYCLES(0xf847, 23);`). `game.c` carries 6 more. These
were transcribed from decompiler output and there is no generator to re-run.

They are, however, **checkable for free**, because `verify.sh` already exists to
catch exactly this class of drift. It compares per-frame cycle counts as well as
video hashes, and `--record` re-records the traces *from the reference build* —
the one whose ROM the decompiler generates. So the sequence is: regenerate
everything, re-record from `ref`, and then require `ext` — the build that uses
the hand-written `a2rom.c` — to still match. Any constant transcribed wrongly
shows up as `FAIL [play/ext]`.

That is the same argument the 2026-08-02 log entry makes for keeping both builds
rather than collapsing them: they run different code and must agree.

### One test whose expectations will move

`tools/textemu/disk_test.cpp` measures elapsed cycles around `runFor` while
watching disk state. Its behaviour changes — very likely for the better, since
`DISK2_CYCLES_PER_NIBBLE` is derived from real hardware timing — but any
threshold in it tuned against flat-3 accounting needs rechecking rather than
adjusting until it passes.

The frame budget itself (`(1.0/60.0) * clock_freq_`) needs no change — it was
always denominated in real cycles, and only now will be spent in them.

Frame 472 will move, change character, or disappear. If it disappears, that is
the strongest available evidence for the RNG-seed hypothesis above, and it should
be recorded either way.
