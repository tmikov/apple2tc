# Snake Byte Phase 1a: Cut the ROM Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Replace the 9 Apple II ROM entry points the game calls with hand-written C, so the decompiled reference loses the ROM subtree and `$71F3` becomes recoverable.

**Architecture:** Teach `apple2tc` that a list of addresses are *external* routines: calls to them become calls to declared-but-undefined C functions, their bodies are dropped, and DCE removes whatever becomes unreachable. Hand-written implementations live in `decoded/snake-byte/a2rom.c`. The program still boots through ROM into BASIC, so the only behavioral change is those 9 routines — which means the existing 1300-frame `play.frames` verifies the whole thing unchanged.

**Tech Stack:** C++17 (decompiler), C11 (runtime), CMake + Ninja, golden-frame-hash oracle.

**Scope note:** This is Phase 1a only. Moving the entry point to `$3750` (which needs an entry-state snapshot and a re-based trace) and hand-writing the 620 remaining game blocks are Phase 1b and 1c, each with its own plan.

---

## Why this is verifiable where the spec's framing was not

The spec described Phase 1 as hand-writing a new source file that simply never contains the ROM. That is a cliff: nothing runs until it is substantially done, and it cannot be checked against `play.frames` at all, because a program starting at `$3750` skips the 168 frames of BASIC boot the trace begins with.

Keeping the boot path and swapping only the 9 routines makes the change checkable against the existing trace on day one. If `PLOT`, `SCRN`, `COUT` and friends are implemented correctly, 1300 frames still match. If they are not, the diff names the first frame that diverges.

---

## Findings this plan is built on

Measured from `--routines-report` on the current build:

| Fact | Value |
| --- | --- |
| Game routines recovered | 26, covering 238 blocks |
| Game blocks still in `func_t001` | **620** |
| ROM blocks still in `func_t001` | **1429** |
| Game candidates rejected | 14, from **5** root causes |

The rejection tree:

| Root | Reason | Transitively blocks |
| --- | --- | --- |
| `$FDED` (`COUT`) | `terminator JmpInd` — it is `JMP (CSWL)` | `$71F3` (15 call sites) |
| `$7230` | `Pop8` underflow — inline-string printer | `$72CE`, `$78B3`, `$7980` |
| `$60E7` | `invalid predecessor inst RTS at $6147` | `$6148`, `$615A` → `$6B3D`, `$7045` |
| `$6A32` | `Pop8` underflow | `$6256`, `$6288` |
| `$7226` | `terminator JmpInd` | — |

Externalizing `$FDED` removes the `JmpInd` that blocks `$71F3`, so `$71F3` should recover for free. That is the single highest-value item in this plan and Task 6 measures whether it actually happens.

### The `COUT` complication — read before implementing

`COUT` is `JMP (CSWL)`, dispatching through the output vector at `$36/$37`. **The game hooks it**: `$6641` (called from `$7485` and `$793F`) sets `CSWL/CSWH` to `$664A`, installing its own character handler.

`$664A` is classified as **data** in `snake-byte.lst` and is **not** a block in `snake-bytec1.c`, and `$664A` does not appear in `snake-byte.json`'s `BranchTargets`. So in the recorded session the hook is installed but never dispatched through. Its bytes decode as real code: `48 PHA / 29 7F AND #$7F / C9 20 CMP #$20 / B0 04 BCS …`.

Consequences:

1. A `COUT` replacement **must honour the vector**, not hardcode the ROM behaviour, or it will diverge the moment `$664A` is reached.
2. `$664A` is a genuine blind spot — reachable code the trace never exercised. This plan does not fix that; it records it. Phase 1c must decide whether to hand-decode it or inherit the same gap.

---

## File Structure

| Path | Status | Responsibility |
| --- | --- | --- |
| `tools/apple2tc/ExternRoutines.cpp` | create | The pass: rewrite calls to listed addresses into calls to bodyless `Function`s |
| `tools/apple2tc/PubIR.h` | modify | Declare the pass |
| `tools/apple2tc/apple2tc.cpp` | modify | `--extern-routines=<file>`, parse the list, run the pass |
| `tools/apple2tc/IRC1.cpp` | modify | A `Function` with no basic blocks emits a declaration only |
| `tools/apple2tc/CMakeLists.txt` | modify | Add the new file |
| `decoded/snake-byte/rom.externs` | create | The 9 `address name` pairs |
| `decoded/snake-byte/a2rom.h` | create | Prototypes for the 9 helpers |
| `decoded/snake-byte/a2rom.c` | create | Their implementations |
| `decoded/snake-byte/decompile.sh` | modify | Pass `--extern-routines` |
| `decoded/snake-byte/CMakeLists.txt` | modify | Compile `a2rom.c` into `snake-bytec1` |

---

## Task 1: Spike — determine the extern mechanism

The rest of the plan depends on details of `IdentifySimpleRoutines`'s call-rewriting that must be read, not guessed. This task writes **no production code**; its deliverable is a written design that Tasks 2–3 are then adjusted against.

Doing this as a spike is deliberate. Specifying the pass without reading `convertInvocations` would put plausible-but-wrong code in this plan, which is worse than an honest investigation step.

**Files:**
- Create: `/tmp/extern-design.md` (scratch, not committed)

- [ ] **Step 1: Read the existing call-rewriting machinery**

Read `tools/apple2tc/routines.cpp`, specifically `IdentifySimpleRoutines::convertInvocations` and `convertRoutineInvocation`, plus `extractRoutine`. Also read `Values.def:186` (`IR_INST2(Call, Void, Function, U16)`) and `IR_INST2(JSR, Void, BasicBlock, BasicBlock)` at line 196.

- [ ] **Step 2: Answer these five questions in writing**

Write `/tmp/extern-design.md` answering each with specific file:line references and code excerpts:

1. **How is a `Function` created and named?** `convertInvocations` does `mod->createFunction()` then `setName()` and `setDecompileLevel()`. What is the minimum needed for a `Function` that will never have a body?
2. **How is a `JSR` rewritten into a `Call`?** Show the exact `IRBuilder` sequence from `convertRoutineInvocation`, including what happens to the JSR's second operand (the "fall" block) and how control resumes after the call.
3. **What happens to the callee's blocks?** After rewriting, are the original ROM blocks unreachable? Does existing `dce` remove them, or does something else keep them alive (e.g. the `s_block_map` dynamic-dispatch table)?
4. **Where must the pass run?** Before or after `identifySimpleRoutines`, and before or after `simplifyCFG`? Note that `identifySimpleRoutines` runs at `-O2` in a loop, `simplifyCFG` at `-O3`.
5. **Does `Function::getAddress()` exist and can it be set?** `IRC1Mod::run()` uses `func->getAddress()` to name functions `func_%04x`. Confirm how an externally-created `Function` gets an address, since `rom.externs` supplies names and the report identifies routines by address.

- [ ] **Step 3: Propose the pass signature and sketch its body**

Based on the answers, write the concrete signature and a code sketch for:

```cpp
bool externRoutines(ir::Module *mod, const std::vector<std::pair<uint16_t, std::string>> &externs);
```

State explicitly whether "a `Function` with no basic blocks" is a viable representation for an external routine, or whether an explicit flag on `Function` is needed instead. `IRC1Mod::run()` emits declarations for all functions at `IRC1.cpp:106` and bodies at `IRC1.cpp:115`, so the empty-body convention would require no `IR.h` change — confirm that nothing else iterates `basicBlocks()` in a way that would break on an empty function.

- [ ] **Step 4: Flag anything that makes the approach unworkable**

If the mechanism turns out to be substantially harder than sketched — for example if `JSR` rewriting depends on the callee's `RTS` instructions being known, which an external routine has none of — say so plainly and propose the alternative. Report `BLOCKED` in that case rather than proceeding.

- [ ] **Step 5: Report**

No commit. Report the design document contents in full.

---

## Task 2: Implement the extern-routines pass

**Adjust the code below against Task 1's findings before writing it.** The structure is settled; the exact `IRBuilder` calls are what Task 1 pins down.

**Files:**
- Create: `tools/apple2tc/ExternRoutines.cpp`
- Modify: `tools/apple2tc/PubIR.h`, `tools/apple2tc/CMakeLists.txt`

- [ ] **Step 1: Declare the pass**

In `PubIR.h`, after the `identifySimpleRoutines` declaration:

```cpp
/// Replace calls to the listed addresses with calls to bodyless external
/// functions, so their implementations can be supplied by hand-written C.
/// \param externs (address, C function name) pairs.
/// \return true if anything was rewritten.
bool externRoutines(
    ir::Module *mod,
    const std::vector<std::pair<uint16_t, std::string>> &externs);
```

Add `#include <string>`, `<utility>` and `<vector>` to `PubIR.h` if not already present.

- [ ] **Step 2: Implement it**

Create `tools/apple2tc/ExternRoutines.cpp` following the design from Task 1. It must:

1. Build an address → name map from `externs`.
2. Walk every function's blocks, finding `JSR` instructions whose target block's address is in the map.
3. For each distinct target address, create one bodyless `Function` with the mapped name, reusing it across call sites.
4. Rewrite each such `JSR` into a `Call` to that function, following the sequence Task 1 documented from `convertRoutineInvocation`.
5. Return whether anything changed.

Match the file header, `using namespace ir;`, and anonymous-namespace conventions of `routines.cpp`.

- [ ] **Step 3: Add to the build**

Add `ExternRoutines.cpp` to `tools/apple2tc/CMakeLists.txt` next to `Dominators.cpp Dominators.h`.

- [ ] **Step 4: Build**

```bash
ninja -C /home/tmikov/work/apple2tc/cmake-build-debug
```
Expected: clean. Nothing calls the pass yet.

- [ ] **Step 5: Confirm no behaviour change**

```bash
cd /home/tmikov/work/apple2tc/tests && ./run-tests.sh ../cmake-build-debug
```
Expected: `Success!` — the pass is not wired in, so nothing may change.

- [ ] **Step 6: Commit**

```bash
cd /home/tmikov/work/apple2tc
git add tools/apple2tc/ExternRoutines.cpp tools/apple2tc/PubIR.h tools/apple2tc/CMakeLists.txt
git commit -m "apple2tc: add extern-routines pass"
```

---

## Task 3: Emit declarations only for bodyless functions

**Files:**
- Modify: `tools/apple2tc/IRC1.cpp`

- [ ] **Step 1: Skip body emission**

In `IRC1Mod::run()`, the loop at `IRC1.cpp:114-121` emits a body for every function. Skip functions with no basic blocks — the forward declaration at line 106 already covers them, and the hand-written C supplies the definition.

```cpp
  // Generate all functions in order, but generate the start function last.
  for (auto &func : mod_->functions()) {
    if (&func == mod_->getStartFunction())
      continue;
    // Bodyless functions are external: the forward declaration above is the
    // whole contract, and the implementation is supplied by hand-written C.
    if (func.basicBlocks().empty())
      continue;
    fprintf(os_, "\n");
    IRC1(this, &func, os_, trees_).runFunc();
  }
```

- [ ] **Step 2: Verify the declaration still appears**

The declaration loop at line 106 iterates all functions unconditionally, so externals still get `void NAME(uint16_t ret_addr);`. Confirm by reading, and state in your report whether the emitted prototype matches what `a2rom.h` will need to provide.

- [ ] **Step 3: Build and confirm nothing changed**

```bash
ninja -C /home/tmikov/work/apple2tc/cmake-build-debug
cd /home/tmikov/work/apple2tc/tests && ./run-tests.sh ../cmake-build-debug
cd /home/tmikov/work/apple2tc/decoded/snake-byte
../../cmake-build-debug/tools/apple2tc/apple2tc snake-byte.b33 --run-data=snake-byte.json -O3 --irc1 > /tmp/c1.c
diff -q snake-bytec1.c /tmp/c1.c && echo "UNCHANGED"
```
Expected: `Success!` and `UNCHANGED` — no function is bodyless yet.

- [ ] **Step 4: Commit**

```bash
cd /home/tmikov/work/apple2tc
git add tools/apple2tc/IRC1.cpp
git commit -m "apple2tc: emit declaration only for bodyless (external) functions"
```

---

## Task 4: Wire up `--extern-routines`

**Files:**
- Modify: `tools/apple2tc/apple2tc.cpp`
- Create: `decoded/snake-byte/rom.externs`

- [ ] **Step 1: Write the externs file**

Create `decoded/snake-byte/rom.externs`. Format: one `HEXADDR name` pair per line, `#` for comments.

```
# Apple II ROM entry points called by Snake Byte, replaced by a2rom.c.
# See docs/superpowers/plans/2026-08-02-snake-byte-phase1a-cut-the-rom.md
F800 rom_plot
F819 rom_hline
F864 rom_setcol
F871 rom_scrn
FC58 rom_home
FC68 rom_fc68
FDED rom_cout
FE89 rom_setkbd
FE93 rom_setvid
```

`$FC68` keeps an address-derived name deliberately: it is the monitor routine the game's inline-string printer calls, and its role is not yet established. Naming it something confident would be a guess.

- [ ] **Step 2: Add the option**

In `apple2tc.cpp`, add to `printHelp()` after the `--routines-report` line:

```cpp
  fprintf(stderr, "  --extern-routines=f Treat listed addresses as external C functions\n");
```

Add a local next to `routinesReportPath`:

```cpp
  std::string externRoutinesPath;
```

Add to the option loop before the `if (argv[i][0] == '-')` fallback:

```cpp
    if (strncmp(argv[i], "--extern-routines=", 18) == 0) {
      externRoutinesPath = argv[i] + 18;
      continue;
    }
```

- [ ] **Step 3: Parse the file and run the pass**

Add a helper above `main()`:

```cpp
static std::vector<std::pair<uint16_t, std::string>> loadExternRoutines(const char *path) {
  std::vector<std::pair<uint16_t, std::string>> res;
  FILE *f = fopen(path, "rt");
  if (!f) {
    perror(path);
    exit(2);
  }
  char line[256];
  unsigned lineNo = 0;
  while (fgets(line, sizeof(line), f)) {
    ++lineNo;
    char *p = line;
    while (*p == ' ' || *p == '\t')
      ++p;
    if (*p == '#' || *p == '\n' || *p == '\r' || *p == 0)
      continue;
    unsigned addr;
    char name[128];
    if (sscanf(p, "%x %127s", &addr, name) != 2 || addr > 0xFFFF) {
      fprintf(stderr, "%s:%u: expected \"HEXADDR name\"\n", path, lineNo);
      exit(3);
    }
    res.emplace_back((uint16_t)addr, std::string(name));
  }
  fclose(f);
  return res;
}
```

Then run the pass. Placement comes from Task 1's answer to question 4; the default assumption is immediately before the `identifySimpleRoutines` loop, so recovery sees the simplified call graph:

```cpp
      if (!externRoutinesPath.empty())
        externRoutines(mod, loadExternRoutines(externRoutinesPath.c_str()));
```

- [ ] **Step 4: Try it**

```bash
cd /home/tmikov/work/apple2tc/decoded/snake-byte
../../cmake-build-debug/tools/apple2tc/apple2tc snake-byte.b33 \
  --run-data=snake-byte.json -O3 --irc1 \
  --extern-routines=rom.externs --routines-report=/tmp/ext.txt > /tmp/ext-c1.c
grep -c 'void rom_' /tmp/ext-c1.c
grep -n 'void rom_cout' /tmp/ext-c1.c
grep 'simple routines identified' /dev/null
grep -c '^ACCEPT' /tmp/ext.txt
grep '^REJECT \$71F3' /tmp/ext.txt || echo '$71F3 NO LONGER REJECTED'
```
Expected: 9 `rom_*` declarations and **no** `rom_*` definitions; `$71F3` no longer rejected.

**If `$71F3` is still rejected, report the new reason.** That is the headline hypothesis of this plan and a negative result is a real finding, not a failure.

- [ ] **Step 5: Commit**

```bash
cd /home/tmikov/work/apple2tc
git add tools/apple2tc/apple2tc.cpp decoded/snake-byte/rom.externs
git commit -m "apple2tc: add --extern-routines; declare Snake Byte's 9 ROM entry points"
```

---

## Task 5: Implement the 9 ROM helpers

**Files:**
- Create: `decoded/snake-byte/a2rom.h`, `decoded/snake-byte/a2rom.c`

Do **not** write these from memory of what Apple II ROM routines do. Seven of the nine are already decompiled into C in the current `snake-bytec1.c` (`$F800`, `$F819`, `$F864`, `$F871`, `$FC58`, `$FE89`, `$FE93` are promoted functions). Lift those, then clean them up. `$FC68` and `$FDED` live inside `func_t001` and must be read out of `snake-byte.lst`.

- [ ] **Step 1: Extract the seven promoted routines**

```bash
cd /home/tmikov/work/apple2tc/decoded/snake-byte
for n in FUNC_PLOT1 FUNC_SETCOL FUNC_GBASCALC; do
  echo "=== $n ==="; sed -n "/^void $n(uint16_t ret_addr) {/,/^}/p" snake-bytec1.c
done
```
Then locate the generated names for `$F800`, `$F819`, `$F871`, `$FC58`, `$FE89`, `$FE93` (they are `func_XXXX` or `FUNC_<symbol>`) and extract each.

- [ ] **Step 2: Read `$FDED` and `$FC68` from the listing**

```bash
cd /home/tmikov/work/apple2tc/decoded/snake-byte
L=$(grep -n '^/\*FDED\*/' snake-byte.lst|head -1|cut -d: -f1); sed -n "${L},$((L+6))p" snake-byte.lst
L=$(grep -n '^/\*FC68\*/' snake-byte.lst|head -1|cut -d: -f1); sed -n "${L},$((L+40))p" snake-byte.lst
```

**`rom_cout` must dispatch through the output vector at `$36/$37`, not hardcode the ROM path.** The game installs its own handler at `$664A` via `$6641`. If the vector points at the ROM's own `COUT1`, call the C implementation of that; otherwise it points into game code, which this build still has in `func_t001` — so `rom_cout` needs a way to transfer there. Determine how, and if there is no clean mechanism, **report `DONE_WITH_CONCERNS` and describe the options** rather than silently hardcoding ROM behaviour. Since the recorded trace never dispatches through `$664A`, a wrong choice here will still pass the oracle — which is exactly why it must be reasoned about rather than tested into place.

- [ ] **Step 3: Write the header**

Create `decoded/snake-byte/a2rom.h`. Every function must match the prototype `IRC1` emits — confirm the exact form from Task 3 Step 2, expected to be:

```c
/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdint.h>

/// Hand-written replacements for the Apple II ROM entry points Snake Byte
/// calls. Declared by the generated C via --extern-routines; see rom.externs.
/// The uint16_t parameter is the emulated return address, unused by these
/// implementations but required by the generated calling convention.

void rom_plot(uint16_t ret_addr);   ///< $F800 PLOT     - plot a lo-res point
void rom_hline(uint16_t ret_addr);  ///< $F819 HLINE    - lo-res horizontal line
void rom_setcol(uint16_t ret_addr); ///< $F864 SETCOL   - set lo-res colour
void rom_scrn(uint16_t ret_addr);   ///< $F871 SCRN     - read a lo-res point
void rom_home(uint16_t ret_addr);   ///< $FC58 HOME     - clear the text screen
void rom_fc68(uint16_t ret_addr);   ///< $FC68 monitor  - used by the $7230 string printer
void rom_cout(uint16_t ret_addr);   ///< $FDED COUT     - JMP (CSWL); honours the output vector
void rom_setkbd(uint16_t ret_addr); ///< $FE89 SETKBD   - reset the keyboard hook
void rom_setvid(uint16_t ret_addr); ///< $FE93 SETVID   - reset the video hook
```

- [ ] **Step 4: Write the implementations**

Create `decoded/snake-byte/a2rom.c`, `#include "a2rom.h"`. Each implementation is the cleaned-up lift from Step 1 or the hand-translation from Step 2. Keep them operating on the same globals the generated code uses (`s_a`, `s_x`, `s_y`, the status flags, `s_ram` via `ram_peek`/`ram_poke`) — this phase is not changing the state model.

Preserve the `CYCLES()` accounting from the lifted code. The oracle compares cycle counts per frame, so dropping it will diverge the trace.

- [ ] **Step 5: Report before wiring up**

Do not commit yet. Report what each implementation does, which were lifted vs. hand-translated, and your resolution of the `rom_cout` vector question. Wiring and verification are Task 6.

---

## Task 6: Wire up, verify against the golden trace

**Files:**
- Modify: `decoded/snake-byte/decompile.sh`, `decoded/snake-byte/CMakeLists.txt`

- [ ] **Step 1: Regenerate with externs**

Edit `decompile.sh` to add `--extern-routines=rom.externs` to the `--irc1` invocation, then run it.

- [ ] **Step 2: Compile `a2rom.c` into the target**

In `decoded/snake-byte/CMakeLists.txt`, add `a2rom.c a2rom.h` to the `snake-bytec1` target's source list. Leave the `snake-byte` target (the `--simple-c` build) alone — it does not use externs.

- [ ] **Step 3: Build**

```bash
ninja -C /home/tmikov/work/apple2tc/cmake-build-debug
```
Expected: clean. Link errors here mean a prototype mismatch between `a2rom.h` and what `IRC1` emitted.

- [ ] **Step 4: THE TEST — verify against the existing golden trace**

```bash
cd /home/tmikov/work/apple2tc/decoded/snake-byte
./verify.sh
```
Expected: `PASS: 1300 frames match`.

This is the whole point of the phase. A pass means the 9 hand-written routines are behaviourally identical to the ROM across the entire recorded session, cycle counts included.

**If it fails, the output names the first diverging frame.** Convert that frame number to a cycle count (`frame * 17050`) and cross-reference `play.frames` to find what the machine was doing. Do not adjust `play.frames` to make the test pass — the trace is the reference, and changing it destroys the only evidence that the replacement is correct.

- [ ] **Step 5: Commit**

```bash
cd /home/tmikov/work/apple2tc
git add decoded/snake-byte/a2rom.c decoded/snake-byte/a2rom.h \
        decoded/snake-byte/decompile.sh decoded/snake-byte/CMakeLists.txt \
        decoded/snake-byte/snake-bytec1.c
git commit -m "snake-byte: replace 9 ROM entry points with hand-written C

Verified frame-identical against play.frames across the full 1300-frame
recorded session, cycle counts included."
```

---

## Task 7: Measure what the cut bought

- [ ] **Step 1: Count what moved**

```bash
cd /home/tmikov/work/apple2tc/decoded/snake-byte
../../cmake-build-debug/tools/apple2tc/apple2tc snake-byte.b33 \
  --run-data=snake-byte.json -O3 --irc1 --extern-routines=rom.externs \
  --routines-report=/tmp/after.txt -v2 2>/tmp/after-v2.log >/dev/null
grep 'simple routines identified' /tmp/after-v2.log
grep -o '// \$[0-9A-F]\{4\}$' snake-bytec1.c | grep -o '[0-9A-F]\{4\}' | python3 -c "
import sys
g=r=0
for l in sys.stdin:
    a=int(l.strip(),16)
    if a>=0xd000: r+=1
    elif a>=0x3750: g+=1
print('func_t001 game blocks:',g,' ROM blocks:',r)"
grep -c '^ACCEPT' /tmp/after.txt
grep '^REJECT' /tmp/after.txt | grep -E '\$(3[7-9]|[4-7][0-9A-F]|8[0-4])'
```

Baseline to compare against: 75 routines, 620 game blocks and **1429 ROM blocks** in `func_t001`, 14 rejected game candidates.

- [ ] **Step 2: Append to the decision log**

Append a new entry to `docs/decompiling/decision-log.md` — **append only, never edit existing entries**. Read the file first to match the established format (`**Scope:** … · **Status:** …`, `**Decision:**`, `**Evidence:**`).

Record: the measured before/after numbers; whether `$71F3` recovered; whether the ROM block count dropped as expected or whether ROM blocks survive via paths other than the 9 entry points; and the `$664A` blind spot, since Phase 1c will have to deal with it.

- [ ] **Step 3: Commit**

```bash
cd /home/tmikov/work/apple2tc
git add docs/decompiling/decision-log.md
git commit -m "docs: record measured effect of cutting the ROM"
```

---

## Task 8: Update the playbook

**Files:**
- Modify: `docs/decompiling/playbook.md`

- [ ] **Step 1: Add what generalises**

Add findings under the appropriate existing headings, tagged with scope (`[6502]` / `[apple2tc]` / `[game]`):

- Externalizing library/ROM entry points is a *decompiler* capability worth having, not a hand-editing chore — and cutting them can unblock procedure recovery elsewhere, because an unrecoverable callee poisons its callers.
- Keeping the original boot path while swapping only the leaf routines makes the change verifiable against an existing trace. Changing the entry point at the same time would have forfeited that.
- `[6502]` A game may hook `COUT` through `CSWL/CSWH`. A replacement must dispatch through the vector, not reimplement the ROM path.
- `[6502]` Runtime-trace-driven decompilation only covers what was recorded. Snake Byte's own `COUT` handler at `$664A` is installed but never dispatched in the recorded session, so it sits in the output as data. Look for vector writes (`$36/$37`, `$38/$39`) and check whether the targets were ever traced.

- [ ] **Step 2: Update procedure step 4**

Step 4 currently says "Replace the ROM boundary" as one clause. Split out what was learned: externalize in the tool first, verify against the existing trace with the boot path intact, and only then move the entry point.

- [ ] **Step 3: Add red flags**

- Replacing a ROM routine that dispatches through a vector by reimplementing the ROM path — the vector may point at game code.
- Assuming the decompiled output covers all reachable code. It covers what the trace exercised.

- [ ] **Step 4: Commit**

```bash
cd /home/tmikov/work/apple2tc
git add docs/decompiling/playbook.md
git commit -m "docs: update playbook from Phase 1a"
```

---

## Done criteria

- [ ] `tests/run-tests.sh ../cmake-build-debug` prints `Success!`
- [ ] `--extern-routines=rom.externs` produces 9 `rom_*` declarations and no `rom_*` definitions
- [ ] `snake-bytec1` links against `a2rom.c` and builds clean
- [ ] `decoded/snake-byte/verify.sh` prints `PASS: 1300 frames match`
- [ ] ROM blocks in `func_t001` measurably reduced from 1429, with the number recorded
- [ ] `$71F3`'s status after the cut recorded either way
- [ ] Decision log and playbook updated from measurements, not predictions

## Explicitly out of scope

- Moving the entry point to `$3750` — Phase 1b; needs an entry-state snapshot and a re-based trace
- Hand-writing the 620 remaining game blocks — Phase 1c
- `$7230`, `$60E7`, `$6A32`, `$7226` — the other four rejection roots, untouched here
- Hand-decoding `$664A` — recorded as a blind spot, not resolved
- Regenerating `decoded/robotron/**` and `decoded/bolo/**`
- The deferred headless-vs-windowed trace comparison
