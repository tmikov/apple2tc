# Accurate Cycle Counts Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Both engines charge the same, near-accurate 6502 cycle costs, derived
from one table — so that the probe acceptance test passes for the right reason.

**Architecture:** A per-opcode base cost becomes a third field on `CPUOpcode`,
generated into `lib/d6502/opcodes.h` by the generator that already produces that
file. The interpreter charges it per instruction; the decompiler sums it per
basic block. Branch-taken penalties are added where taken-ness is known — at
runtime in one engine, per edge in the other. Indexed page crossings are
deliberately not modelled by either.

**Tech Stack:** C++17 (`d6502`, `cpuemu`, `apple2tc`), C11 (`a2host`). Tests are
shell + `diff` via `tests/run-tests.sh` and `decoded/snake-byte/verify.sh`.

**Spec:** `docs/plans/2026-08-12-accurate-cycles-design.md`

---

## Ordering, and why each task stays green

The two engines are only compared by `decoded/rom/probe-acceptance.sh`, which is
already red. Everything else compares like with like:

- `verify.sh` runs two **generated** builds against recorded traces, so changing
  the interpreter cannot affect it.
- `run-tests.sh`'s front-end check runs `a2emu` against `a2run` — both the
  **interpreter** — so changing the decompiler cannot affect it.
- The `.ir` baselines change only when the decompiler changes.

So the engines can be converted one at a time without a task in which everything
is broken.

| file | what changes |
|---|---|
| `include/apple2tc/d6502.h` | `CPUOpcode` gains `cycles`; `cpuInstCycles()` declared |
| `lib/d6502/d6502.cpp` | `cpuInstCycles()`, the authoritative derivation |
| `lib/d6502/gen.cpp` | emits the new field |
| `lib/d6502/opcodes.h` | regenerated |
| `tools/id/` | a `cycles` command, so the table is testable and readable |
| `lib/cpuemu/emu6502.cpp` | per-opcode charge, branch +1 |
| `tools/apple2tc/GenIR.cpp`, `PrintSimpleC.cpp` | per-block sum, branch +1 on edges |
| `tests/**`, `decoded/**` | regenerated and re-recorded |

---

## Task 1: The table

**Files:** modify `include/apple2tc/d6502.h`, `lib/d6502/d6502.cpp`,
`lib/d6502/gen.cpp`, `lib/d6502/opcodes.h`, `tools/id/id.cpp`,
`tests/run-tests.sh`; create `tests/cycles.expected`.

- [ ] **Step 1: Write the failing test**

`id` is a REPL over stdin, so it scripts directly. Add to the probe-free part of
`tests/run-tests.sh`, beside the existing `xref` test:

```sh
# The per-opcode cycle table, dumped in full. Both engines derive from this one
# table -- if they ever diverge, today's flat-3-versus-bytes bug returns
# silently -- so it is worth pinning all 256 entries, not a sample.
printf 'cycles\n' | $id > cycles-test.txt 2>&1
diff -q cycles.expected cycles-test.txt
rm cycles-test.txt
```

- [ ] **Step 2: Run it and see it fail** — `Unknown command: cycles`.

- [ ] **Step 3: Derive the costs**

In `lib/d6502/d6502.cpp`, beside `decodeOpcodeSlow`:

```cpp
/// Base cycle cost, not counting the +1 a taken branch adds or the +1 an
/// indexed read pays when it crosses a page. Neither engine models the page
/// crossing -- see the design doc -- so this is the whole of the shared model
/// apart from branches.
///
/// Cost is *nearly* a function of (kind, addrMode), and the exceptions are why
/// this is derived rather than typed out: a read-modify-write costs more than a
/// load at the same mode (ASL $nnnn,X is 7, LDA $nnnn,X is 4), and a store at an
/// indexed mode unconditionally pays the cycle a load pays only on a page cross
/// (STA $nnnn,X is 5, LDA $nnnn,X is 4).
unsigned cpuInstCycles(CPUInstKind kind, CPUAddrMode addrMode);
```

Implement it by classifying `kind` into read / write / read-modify-write /
control / implied, then switching on `addrMode` within each class. Take the
numbers from a standard 6502 reference; do not derive them from the current
code, which is what this plan exists to replace.

The entries to be most careful with, because they are the ones the current
models get most wrong and the ones a test will notice:

| instruction | cycles |
|---|---|
| `INC $nn` (zp RMW) | 5 |
| `BIT $nnnn` (abs read) | 4 |
| branches, not taken | 2 |
| `JSR`, `RTS`, `RTI`, `BRK` | 6, 6, 6, 7 |
| `LDA $nnnn,X` (indexed read) | 4 |
| `STA $nnnn,X` (indexed write) | 5 |
| `ASL $nnnn,X` (indexed RMW) | 7 |
| `LDA ($nn,X)` / `LDA ($nn),Y` | 6 / 5 |
| `PHA` / `PLA` | 3 / 4 |
| implied (`INX`, `CLC`, …) | 2 |

- [ ] **Step 4: Add the field and regenerate**

Add `uint8_t cycles;` to `CPUOpcode` in `d6502.h`, emit it from
`genDisasmTable()` in `gen.cpp`, and regenerate `opcodes.h` the way the file was
originally produced. Confirm the regenerated file differs from the committed one
*only* by the new field — a whole-file rewrite that also reflows unrelated
entries makes the diff unreviewable.

- [ ] **Step 5: Add the `cycles` command to `id`**

One line per opcode: `$%02X %s %s %u` — opcode, mnemonic, addressing mode,
cycles. Invalid opcodes print `- - 0`. This is the human-readable form of the
table as well as the test's input.

- [ ] **Step 6: Generate and inspect the baseline**

```
ninja -C cmake-build-debug
printf 'cycles\n' | cmake-build-debug/tools/id/id > tests/cycles.expected 2>&1
```

**Read it.** Spot-check every row in the table above by hand, and check the four
instructions of the ROM's keyboard-wait loop specifically — `INC $4E` 5,
`BNE` 2, `BIT $nnnn` 4, `BPL` 2 — because the design's whole worked example is
that they sum to 15 with the taken-branch penalties.

- [ ] **Step 7: Tests and commit**

```
cd tests && ./run-tests.sh ../cmake-build-debug     # Success!
```

```bash
git add include/apple2tc/d6502.h lib/d6502/ tools/id/ tests/cycles.expected \
        tests/run-tests.sh
git commit -m "d6502: per-opcode cycle costs"
```

---

## Task 2: The interpreter

**Files:** modify `lib/cpuemu/emu6502.cpp`; regenerate
`tests/probe/stopat.expected`.

- [ ] **Step 1: Understand what must not go wrong**

`emu6502.cpp:112` applies its charge in the **loop increment**:

```c
for (unsigned startCycles = cycles_; cycles_ - startCycles < runCycles; cycles_ += 3) {
```

which runs *after* the body, when `pc_` has already advanced. Writing
`cycles_ += s_opcodes[ram_[pc_]].cycles` there bills every instruction at its
successor's price — a wrong model that looks entirely plausible and whose only
symptom is a comparison failing somewhere else. Capture the opcode before
dispatch and charge against that.

- [ ] **Step 2: Convert**

Move the charge into the body, against an opcode captured before the switch, and
add the taken-branch penalty in each branch case. There are 8 branch
instructions; the +1 belongs on the path that actually branches, not on the
decode.

Do **not** add the page-crossing penalty, even though the interpreter could
compute it exactly. The design is explicit: both engines charge the base so they
agree by construction, and making one of them "more correct" reintroduces the
divergence this plan exists to remove. Say that in a comment at the site where
someone would be tempted.

- [ ] **Step 3: Regenerate the one affected probe baseline**

`tests/probe/stopat.expected` counts hits in the ROM's idle loop, so its length
depends on the cycle model. Everything else under `tests/probe/` is
count-independent — `hello` fires once, `arith`/`format`/`mem`/`chain` are
straight-line, `flow` fires three times at instruction starts.

```
cmake-build-debug/tools/a2run/a2run --frames=20 --probe=tests/probe/stopat.probe \
  --probe-out=tests/probe/stopat.expected
```

Read it: it must still be `hit 1` … `hit N` with no gaps, and **N must change**
from 129 — if it does not, the interpreter is not using the new table.

- [ ] **Step 4: Tests and commit**

`run-tests.sh` must end `Success!`. `verify.sh` must still be 4/4 — it runs two
generated builds and cannot be affected by this task; if it changes, something
is wrong beyond this task's scope, so report rather than proceed.

```bash
git add lib/cpuemu/emu6502.cpp tests/probe/stopat.expected
git commit -m "cpuemu: charge real per-opcode cycles"
```

---

## Task 3: The decompiler

**Files:** modify `tools/apple2tc/GenIR.cpp`, `tools/apple2tc/PrintSimpleC.cpp`;
regenerate the 10 `.ir` baselines in `tests/`.

- [ ] **Step 1: Replace the estimate**

`GenIR.cpp:77` and `PrintSimpleC.cpp:103` both compute
`lround(asmBlock.size() * 1.7 + 0.5)`. Replace both with a sum of
`cpuInstCycles()` over the block's instructions. The two sites must agree — they
are the IR and the C printer for the same thing — so factor the sum into one
helper rather than writing it twice.

- [ ] **Step 2: Attribute the branch penalty to edges**

A conditional branch's +1 is paid only when taken. The decompiler knows the
edges, so the cost lands on the taken successor rather than in the block's own
constant. Concretely: the block ending in a conditional branch charges the
not-taken cost, and the taken edge adds 1.

**This is the part most likely to be subtly wrong**, because getting it backwards
produces a model that is off by exactly one cycle per taken branch — small
enough to look like noise and large enough to fail the acceptance test in a loop.
Derive the emitted code for a two-way branch by hand before implementing, and
put that derivation in your report.

- [ ] **Step 3: Regenerate the `.ir` baselines**

All 10 contain `AddCycles` constants — 54 in total. Each is produced by the
command in `tests/run-tests.sh` that checks it, redirected to the baseline
instead of to a `-test` file:

```sh
cd tests
bin=../cmake-build-debug
a6502=$bin/tools/a6502/a6502
apple2tc=$bin/tools/apple2tc/apple2tc

for t in trees trees1 ands func phapla stackmerge; do
  $a6502 $t.s $t.b33 && $apple2tc $t.b33 -O3 --ir > $t.ir && rm $t.b33
done

$a6502 subs.s subs.b33 && $apple2tc --run-data=dclear.json -O3 --ir subs.b33 > subs.ir
rm subs.b33

$a6502 codeat.s codeat.b33
$apple2tc codeat.b33 --code-at=codeat.txt -O3 --ir > codeat.ir
$apple2tc codeat.b33 -O3 --ir > codeat-noedges.ir
rm codeat.b33

$a6502 retpoint.s retpoint.b33
$apple2tc retpoint.b33 --code-at=retpoint.txt -O3 --ir \
  --routines-report=/dev/null > retpoint.ir
rm retpoint.b33
```

Check these against `run-tests.sh` before running them — if that file has moved
on, it is authoritative, not this snippet.

**Then diff the old and new baselines and read the diff.**

**Correction, from executing this task:** the instruction here originally read
"every change must be an `AddCycles` constant and nothing else — a change to
block structure means this task altered control flow." That is wrong, and
following it literally would have forced an incorrect approximation.

A branch target routinely has predecessors that arrive *without* owing the
taken-branch penalty — by fall-through, or by an unconditional `Jmp`. `trees.ir`
has exactly this: `%bb_032f` is reached from two taken branches **and** from
`%bb_0321` by a plain `Jmp`. Folding the +1 into the target's own `AddCycles`
would overcharge that third path. The penalty therefore has to live on the
**edge**, which in the IR means a one-instruction trampoline block
(`AddCycles 1; Jmp target`) interposed on the taken arm.

So the four baselines with conditional branches — `trees`, `func`, `phapla`,
`stackmerge` — legitimately gain small blocks. The other six contain no
conditional branches and their diffs are constants only. What must *not* change
is which real instructions live in which block; a trampoline carries no
instructions and costs nothing at runtime, since it is reached by an
unconditional jump rather than a test.

In the C printer there are no separate blocks, so the +1 folds into the taken
arm of the ternary that already tests the condition:

```c
/* $0311 BEQ */ s_pc = s_status & STATUS_Z ? (s_cycles++, s_remaining_cycles--, 0x032f) : 0x0313;
```

Both counters, because that is what `CYCLES` updates.

- [ ] **Step 4: Hand-check one**

Pick the smallest baseline with a branch, compute its block costs from the table
by hand, and confirm the emitted constants match. Put the derivation in your
report.

- [ ] **Step 5: Tests and commit**

`run-tests.sh` must end `Success!`. `verify.sh` will still be 4/4 because the
committed generated C has not been regenerated yet — that is Task 4.

```bash
git add tools/apple2tc/GenIR.cpp tools/apple2tc/PrintSimpleC.cpp tests/*.ir
git commit -m "apple2tc: emit real per-block cycle costs"
```

---

## Task 4: Regenerate the decoded programs

This is the task where the two engines become consistent with each other, and
where `verify.sh` is the gate.

**Files:** regenerate `decoded/*/​*.c`; patch the hand-written constants in
`decoded/snake-byte/a2rom.c` and `game.c`; re-record
`decoded/snake-byte/*.frames`.

- [ ] **Step 1: Regenerate the generated C**

`decoded/snake-byte/decompile.sh` regenerates both snake-byte variants; the other
decoded programs have their own scripts or are produced by the same tool. Between
them, ~18,900 `CYCLES()` constants change across 11 files.

- [ ] **Step 2: Patch the hand-written constants**

`a2rom.c` carries **82** hand-written `CYCLES(0xf847, 23)` constants and `game.c`
carries 6. There is no generator for them — they were transcribed.

They can be derived rather than recomputed by hand: the same ROM blocks are
decompiled into `snake-bytec1.c` (the self-contained reference build), so for
each address in `a2rom.c` the new constant is whatever `snake-bytec1.c` now emits
for that same address. Extract and patch mechanically, then read the diff.

- [ ] **Step 3: Re-record the traces, from the reference build only**

```
cd decoded/snake-byte && ./verify.sh --record
```

`--record` re-records from `ref` twice and refuses to write unless the two runs
agree. **Do not re-record from `ext`** — the whole value of the `[hires]`
scenario is that `ext` runs different code and must independently match.

- [ ] **Step 4: The gate**

```
cd decoded/snake-byte && ./verify.sh
```

Must be 4/4. `PASS [play/ref]` and `PASS [hires/ref]` are near-tautological after
re-recording. **`PASS [play/ext]` and `PASS [hires/ext]` are the real result**:
they say the 88 hand-patched constants are right, because `ext` was not what the
traces were recorded from.

If `ext` fails, a constant is wrong. Find it — the first differing frame bounds
where to look — rather than re-recording from `ext` to make it green.

- [ ] **Step 5: Commit**

Tool changes are already committed; this is game data, so it is its own commit.

```bash
git add decoded/
git commit -m "decoded: regenerate against real cycle costs"
```

---

## Task 5: Re-record the run data and key traces

**Files:** `decoded/snake-byte/snake-byte.json`, `decoded/snake-byte/*.keys`.

- [ ] **Step 1: Understand what is stale**

`snake-byte.json` is runtime data collected by the emulator, and the `.keys`
files are cycle-stamped. Both encode the old cycle model. The standing rule is
never to *edit* `snake-byte.json`; regenerating it headlessly is what the
host/engine split made possible, and `decoded/snake-byte/README.md` has the
recipe — including the warning that it does not reproduce the committed file
byte for byte.

- [ ] **Step 2: Decide whether to re-record at all, and say why**

This is a judgement call, not a mechanical step. The run data drives
`--run-data=`, which supplies code coverage to the disassembler; its cycle stamps
may not matter to that use at all. The `.keys` stamps definitely do matter, since
`get_cycles() >= stamp` decides delivery.

Check what actually consumes the cycle fields before regenerating anything. If
the run data's coverage is unaffected, leaving it alone is better than churning
a 2022 artefact for no reason — but say so explicitly rather than skipping it
silently.

- [ ] **Step 3: Re-record what needs it, and verify**

If the `.keys` stamps move, `verify.sh` must be re-recorded again after them, in
that order. Finish with `run-tests.sh` Success! and `verify.sh` 4/4.

- [ ] **Step 4: Commit**

```bash
git add decoded/
git commit -m "decoded: re-record cycle-stamped inputs"
```

---

## Task 6: The acceptance test, and frame 472

The payoff, and the only task whose result is not known in advance.

**Files:** possibly `decoded/rom/probe-acceptance.sh`; append to
`docs/decompiling/decision-log.md`.

- [ ] **Step 1: Run it**

```
./decoded/rom/probe-acceptance.sh cmake-build-debug
```

It has been failing with 336,825 versus 256,616 hits — a byte-exact prefix, all
of the difference being dwell time in `$FD1B`/`$FD21`. **It should now pass.**

If it does not, characterise the difference exactly as before — first differing
line, hit counts, whether one is a prefix of the other, whether the distinct
address sets still match — and diagnose before changing anything. Do not adjust
frame counts or trim the site list.

- [ ] **Step 2: Check the RNG hypothesis**

The design argues the two engines previously diverged in *program state*, not
only in measurement: `$FD1B` is `INC $4E`, `$4E`/`$4F` is the Apple II random
seed, and the two engines incremented it 155,925 versus 115,899 times over the
same budget. Measure it now — sample `peek8($004E)` and `peek8($004F)` at the
end of an equal-budget run on both engines and report whether they agree.

- [ ] **Step 3: Check frame 472**

`decoded/snake-byte/verify.sh` compares each build against a recorded trace, so
it cannot see this. The frame-472 divergence (2026-08-08 entry) is between the
**interpreter and a generated build**, over 1,300 frames of `play.keys`.

Re-run that comparison. Report whether the divergence moved, changed character,
or disappeared. **If it disappeared, say so plainly and state what that implies**
— it would mean a two-year-old placeholder in the cycle accounting, not a
decompiler defect, produced the one unexplained result in this project.

- [ ] **Step 4: Record the outcome**

Append to `docs/decompiling/decision-log.md`, dated today, in the format the
existing entries use. That log is append-only — never edit an existing entry.
State the acceptance-test result, the `$4E` measurement, and what happened to
frame 472, whichever way each went.

- [ ] **Step 5: Commit**

```bash
git add decoded/rom/ docs/decompiling/decision-log.md
git commit -m "probe: the acceptance test passes on real cycle costs"
```

Adjust the message to what actually happened.

---

## Self-review against the spec

**Covered:** the shared table and its generation; both engines converted; branch
penalties attributed exactly; page crossings deliberately excluded, in both
engines, with the reasoning at the tempting site; every regenerated artefact
including the 88 hand-written constants and the mechanism that checks them; the
acceptance test as the guard that the two consumers have not drifted.

**Deliberately left open:** Task 5 asks for a judgement about whether
`snake-byte.json` needs re-recording at all, rather than prescribing it. Churning
a 2022 recording that nothing may depend on is worse than leaving it, and the
plan cannot know which without reading what consumes the cycle fields.

**Two things this plan does not do:**

1. **It does not make the emulator cycle-exact.** Indexed page crossings remain
   unmodelled by both engines. That is a deliberate, shared inaccuracy, and it is
   the only one left.
2. **It does not touch `tools/textemu/disk_test.cpp`**, whose measurements will
   move because disk timing is derived from real hardware numbers and will now be
   fed real cycles. Any threshold in it tuned against flat-3 needs rechecking
   rather than adjusting until green — but that is its own task, once this lands
   and there is something correct to tune against.

**The risk to watch:** Tasks 2 and 3 convert one engine each, and nothing
compares the two until Task 6. A discrepancy introduced in either is invisible
for three tasks. The mitigation is the hand-derivations demanded in Task 2 step 3
and Task 3 steps 2 and 4 — if those are skipped, the first signal is a failing
acceptance test with two candidate causes.
