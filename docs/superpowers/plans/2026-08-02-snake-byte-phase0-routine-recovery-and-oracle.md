# Snake Byte Phase 0: Routine Recovery and Oracle Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Fix apple2tc's over-strict procedure recovery, add a routines report that Phase 1 will be written from, and stand up a deterministic frame-hash oracle for the Snake Byte conversion.

**Architecture:** Three independent pieces. (1) `scanCandidate` in `routines.cpp` currently balances the 6502 stack per basic block; it becomes a depth-propagating dataflow over the candidate's block set, which accepts the standard `PHA`-at-entry / `PLA`-before-`RTS` shape. (2) A new `--routines-report` mode emits per-candidate status, block sets, call sites, dominator trees and natural loops — requiring a dominator implementation, which the codebase does not currently have. (3) `decapplib` gains per-frame hashing of video memory, a frame limit, and a headless mode, which turns the existing cycle-locked `--key-file` replay into a reproducible behavioral oracle that needs no display. Hashing memory rather than rendered pixels is what makes headless possible at all — no graphics context is required.

**Tech Stack:** C++17 (decompiler), C11 (runtime library), CMake + Ninja, `a6502` for test fixtures, golden-file diffing for regression.

**Scope note:** This plan covers Phase 0 and oracle standup from `docs/superpowers/specs/2026-08-02-snake-byte-proper-c-design.md`. Phases 1 and 2 get their own plans, because Phase 1's shape depends on how many routines Phase 0 actually recovers.

---

## File Structure

| Path | Status | Responsibility |
| --- | --- | --- |
| `tests/phapla.s` | create | Fixture: routine with `PHA` and `PLA` in different basic blocks |
| `tests/phapla.ir` | create | Frozen baseline for the above |
| `tests/run-tests.sh` | modify | Add the `phapla` case |
| `tools/apple2tc/routines.cpp` | modify | Stack depth dataflow; record rejections; emit report |
| `tools/apple2tc/Dominators.h` | create | Dominator tree + natural loop interface |
| `tools/apple2tc/Dominators.cpp` | create | Cooper-Harvey-Kennedy iterative dominators; natural loops |
| `tools/apple2tc/PubIR.h` | modify | `identifySimpleRoutines` gains a report `FILE *` |
| `tools/apple2tc/apple2tc.cpp` | modify | `--routines-report=<path>` option |
| `tools/apple2tc/CMakeLists.txt` | modify | Add `Dominators.cpp` |
| `lib/decapplib/decapplib.c` | modify | `--hash-frames=<path>`, `--frames=<n>`, `--headless` |
| `decoded/snake-byte/play.frames` | create | Golden per-frame trace |
| `decoded/snake-byte/verify.sh` | create | Record + diff harness |

`Dominators` is a separate file rather than more code in `routines.cpp` because it is general graph machinery with no knowledge of routines, and Phase 1's relooping will use it directly.

---

## Task 1: Failing test — cross-block PHA/PLA is rejected

**Files:**
- Create: `tests/phapla.s`

- [ ] **Step 1: Write the test fixture**

This is a well-behaved subroutine: it saves `A`, does conditional work, restores `A`, and returns. `PHA` is in the entry block and `PLA` is in a different block, which is what current recovery cannot handle.

Create `tests/phapla.s`:

```
        org     $300

        ldx     #10
        jsr     func1
        brk     #1

        org     $320
func1:
        pha
        lda     $1001
        beq     skip
        sta     $1000
skip:
        pla
        rts
```

- [ ] **Step 2: Build the current tools**

Run:
```bash
cd /home/tmikov/work/apple2tc
cmake -G Ninja -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
ninja -C cmake-build-debug
```
Expected: builds cleanly.

- [ ] **Step 3: Run the assertion and verify it FAILS**

Run:
```bash
cd /home/tmikov/work/apple2tc/tests
../cmake-build-debug/tools/a6502/a6502 phapla.s phapla.b33
../cmake-build-debug/tools/apple2tc/apple2tc -O3 --ir phapla.b33 | grep -c '^function func_0320'
```
Expected: prints `0` — the routine was NOT extracted. This is the bug.

- [ ] **Step 4: Confirm the reason**

Run:
```bash
cd /home/tmikov/work/apple2tc/tests
../cmake-build-debug/tools/apple2tc/apple2tc -O3 --ir phapla.b33 -v2 2>&1 >/dev/null | grep -i 'fail\|candidate'
```
Expected: output contains `fail: block $0320 stack level not zero`.

- [ ] **Step 5: Commit the fixture**

```bash
cd /home/tmikov/work/apple2tc
git add tests/phapla.s
git commit -m "tests: add fixture for cross-block PHA/PLA routine recovery"
```

---

## Task 2: Track stack depth across the candidate's block set

**Files:**
- Modify: `tools/apple2tc/routines.cpp:124-215` (the body of `scanCandidate`)

- [ ] **Step 1: Replace the traversal state**

In `routines.cpp`, replace the whole body of `IdentifySimpleRoutines::scanCandidate` from its opening brace down to (but NOT including) the comment `// Now check whether all predecessors are either JSR, Jmp or fall.` with the following.

The change: depth is carried along CFG edges instead of resetting to 0 per block, a block reached at two different depths is rejected as inconsistent, and `RTS` requires depth exactly 0 (the return address on top).

```cpp
void IdentifySimpleRoutines::scanCandidate(BasicBlock *entry) {
  // Stack depth relative to routine entry, at the start of each visited block.
  std::unordered_map<BasicBlock *, int> depthAt{};
  std::unordered_set<BasicBlock *> jsrTargets{};
  std::unordered_set<Instruction *> rts{};
  std::deque<std::pair<BasicBlock *, int>> workList{};

  if (ctx_->getVerbosity() > 1)
    fprintf(stderr, "$%04x: scan candidate\n", entry->getAddress().value_or(0x10000));

  workList.emplace_back(entry, 0);
  do {
    auto [bb, entryDepth] = workList.front();
    workList.pop_front();

    // Already visited? Every path here must agree on the stack depth.
    auto [it, inserted] = depthAt.try_emplace(bb, entryDepth);
    if (!inserted) {
      if (it->second != entryDepth) {
        if (ctx_->getVerbosity() > 1)
          fprintf(
              stderr,
              "fail: block $%04x reached at stack level %d and %d\n",
              bb->getAddress().value_or(0),
              it->second,
              entryDepth);
        return;
      }
      continue;
    }

    // Indirect branches except RTS are not allowed.
    auto *terminator = bb->getTerminator();
    if (terminator->getKind() != ValueKind::RTS && terminator->isIndirectBranch()) {
      if (ctx_->getVerbosity() > 1)
        fprintf(stderr, "fail: terminator %s\n", getValueKindName(terminator->getKind()));
      return;
    }

    // Check for stack operations. RTS and JSR are allowed. push8/pop8 adjust the
    // depth, which is tracked across the whole routine rather than per block.
    // Others are not allowed.
    int depth = entryDepth;
    for (auto &iRef : bb->instructions()) {
      if (iRef.getKind() == ValueKind::JSR) {
        jsrTargets.insert(cast<BasicBlock>(iRef.getOperand(0)));
      } else if (iRef.getKind() == ValueKind::RTS) {
        rts.insert(&iRef);
      } else if (iRef.getKind() == ValueKind::Push8) {
        ++depth;
      } else if (iRef.getKind() == ValueKind::Pop8) {
        --depth;
        if (depth < 0) {
          // Popping below routine entry means the routine is manipulating its
          // own return address.
          if (ctx_->getVerbosity() > 1)
            fprintf(
                stderr,
                "fail: %s block $%04x stack level underflow\n",
                getValueKindName(iRef.getKind()),
                bb->getAddress().value_or(0));
          return;
        }
      } else if (iRef.modifiesSP()) {
        if (ctx_->getVerbosity() > 1)
          fprintf(stderr, "fail: %s\n", getValueKindName(iRef.getKind()));
        return;
      }
    }

    if (terminator->getKind() == ValueKind::RTS) {
      // The return address must be on top of the stack when we return.
      if (depth != 0) {
        if (ctx_->getVerbosity() > 1)
          fprintf(
              stderr,
              "fail: block $%04x stack level %d at RTS\n",
              bb->getAddress().value_or(0),
              depth);
        return;
      }
    } else if (terminator->getKind() == ValueKind::JSR) {
      // Optimistically continue in the "fall" block. A JSR/RTS pair is
      // depth-neutral from this routine's point of view.
      workList.emplace_back(cast<BasicBlock>(terminator->getOperand(1)), depth);
    } else {
      for (auto &succ : successors(*bb))
        workList.emplace_back(&succ, depth);
    }
  } while (!workList.empty());

  std::unordered_set<BasicBlock *> visited{};
  visited.reserve(depthAt.size());
  for (auto &p : depthAt)
    visited.insert(p.first);

```

The existing code from `// Now check whether all predecessors are either JSR, Jmp or fall.` onward is unchanged and continues to use `visited`, `jsrTargets` and `rts`.

- [ ] **Step 2: Build**

Run:
```bash
ninja -C /home/tmikov/work/apple2tc/cmake-build-debug
```
Expected: builds cleanly.

- [ ] **Step 3: Run the assertion and verify it PASSES**

Run:
```bash
cd /home/tmikov/work/apple2tc/tests
../cmake-build-debug/tools/apple2tc/apple2tc -O3 --ir phapla.b33 | grep -c '^function func_0320'
```
Expected: prints `1` — the routine is now extracted.

- [ ] **Step 4: Verify no existing test regressed**

Run:
```bash
cd /home/tmikov/work/apple2tc/tests
./run-tests.sh ../cmake-build-debug
```
Expected: `Success!`

If any `.ir` baseline now differs, inspect the diff before proceeding. A baseline change is legitimate only if it shows a routine that is now correctly extracted; regenerate that baseline and note it in the commit message. Any other difference means the fix is wrong — stop and investigate.

- [ ] **Step 5: Freeze the new baseline and register the test**

Run:
```bash
cd /home/tmikov/work/apple2tc/tests
../cmake-build-debug/tools/apple2tc/apple2tc -O3 --ir phapla.b33 > phapla.ir
rm phapla.b33
```

Then in `tests/run-tests.sh`, immediately after the `func.s` block and before the `echo "Success!"` line, add:

```bash
$a6502 phapla.s phapla.b33 && $apple2tc -O3 --ir phapla.b33 > phapla-test.ir
diff -q phapla.ir phapla-test.ir
rm phapla-test.ir phapla.b33
```

- [ ] **Step 6: Run the full suite again**

Run:
```bash
cd /home/tmikov/work/apple2tc/tests
./run-tests.sh ../cmake-build-debug
```
Expected: `Success!`

- [ ] **Step 7: Commit**

```bash
cd /home/tmikov/work/apple2tc
git add tools/apple2tc/routines.cpp tests/phapla.ir tests/run-tests.sh
git commit -m "apple2tc: track 6502 stack depth per routine, not per basic block

scanCandidate reset stackLevel to 0 for every basic block, so the standard
PHA-at-entry / PLA-before-RTS idiom was rejected as unbalanced. Depth is now
propagated along CFG edges from routine entry; a block reached at two different
depths is rejected as inconsistent, and RTS requires depth 0."
```

---

## Task 3: Measure the effect on Snake Byte

This task produces no code. It records what the fix bought, which the Phase 1 plan needs.

**Files:** none modified.

- [ ] **Step 1: Count recovered routines before and after**

Run:
```bash
cd /home/tmikov/work/apple2tc/decoded/snake-byte
../../cmake-build-debug/tools/apple2tc/apple2tc snake-byte.b33 \
  --run-data=snake-byte.json -O3 --irc1 -v2 > /tmp/new-c1.c 2>/tmp/new-v2.log
grep -c '^void func_\|^void FUNC_' /tmp/new-c1.c
grep 'simple routines identified' /tmp/new-v2.log
```
Expected: more than the current 108 declaration+definition lines, and a first-pass count above 53.

- [ ] **Step 2: List which game routines are still rejected**

Run:
```bash
cd /home/tmikov/work/apple2tc
python3 - <<'EOF'
import re, collections
cur=None; out=[]
for line in open('/tmp/new-v2.log'):
    m=re.match(r'\$([0-9a-f]{4}): scan candidate', line)
    if m: cur=int(m.group(1),16); continue
    m=re.match(r'fail: (.*)', line)
    if m and cur is not None:
        out.append((cur, re.sub(r'\$[0-9a-f]+','$X',m.group(1)).strip())); cur=None
game=sorted({(a,r) for a,r in out if 0x3750<=a<=0x854E})
print("rejected game routines:", len(game))
for a,r in game: print("  $%04X  %s"%(a,r))
EOF
```
Expected: the list should have shrunk from 10. Per the spec, `$7230` and `$6A32` (`Pop8` underflow) and `$7226` (`JmpInd`) should remain.

- [ ] **Step 3: Record the result in the decision log**

Append a new entry to `docs/decompiling/decision-log.md`. Do not edit existing entries — the file is append-only. Use this shape, filling in the measured numbers:

```markdown
## 2026-08-02 — Per-routine stack tracking: measured effect

**Scope:** apple2tc · **Status:** validated

**Decision:** (records outcome of the 2026-08-02 "Improve routines.cpp" entry)

**Evidence:** After propagating stack depth along CFG edges, Snake Byte recovers
N routines in the first pass, up from 53. Game-range rejections fall from 10 to
M: <list>. The predicted 7-of-10 recovery was <accurate / an overestimate>.
```

If the number recovered is materially below 7, also record *why* — the spec flagged this estimate as unverified, and the reason is the reusable knowledge.

- [ ] **Step 4: Regenerate the checked-in c1 output**

Run:
```bash
cd /home/tmikov/work/apple2tc/decoded/snake-byte
./decompile.sh
```
Expected: `snake-bytec1.c` is rewritten with more promoted routines.

- [ ] **Step 5: Commit**

```bash
cd /home/tmikov/work/apple2tc
git add decoded/snake-byte/snake-bytec1.c docs/decompiling/decision-log.md
git commit -m "snake-byte: regenerate c1 output after routine recovery fix"
```

Note: `decoded/robotron/robotronc1.c` and `decoded/bolo/bolo.c` are also stale after this change. Leave them — regenerating them is out of scope here and belongs with whoever next works on those targets.

---

## Task 4: Record rejection reasons instead of only printing them

The report needs rejected candidates, but `scanCandidate` currently discards them with a bare `return`.

**Files:**
- Modify: `tools/apple2tc/routines.cpp`

- [ ] **Step 1: Add the rejection map**

In the `IdentifySimpleRoutines` class declaration, immediately after the line `std::multimap<BasicBlock *, Candidate *> blocks_{};`, add:

```cpp
  /// Entry blocks that were considered and rejected, with the reason.
  std::map<uint32_t, std::string> rejected_{};
```

Keyed by address rather than pointer so the report comes out in a stable order.

- [ ] **Step 2: Add a rejection helper**

In the same class, in the `private:` section immediately before `void scanCandidate(BasicBlock *entry);`, add:

```cpp
  /// Record why a candidate was rejected, and log it at high verbosity.
  void reject(BasicBlock *entry, const std::string &reason);
```

Then add the definition in the file, immediately before `void IdentifySimpleRoutines::scanCandidate(BasicBlock *entry) {`:

```cpp
void IdentifySimpleRoutines::reject(BasicBlock *entry, const std::string &reason) {
  rejected_.emplace(entry->getAddress().value_or(0x10000), reason);
  if (ctx_->getVerbosity() > 1)
    fprintf(stderr, "fail: %s\n", reason.c_str());
}
```

- [ ] **Step 3: Route every rejection through the helper**

In `scanCandidate`, replace each of the five `if (ctx_->getVerbosity() > 1) fprintf(stderr, "fail: ...")` blocks with a `reject(entry, ...)` call. Use `stringPrintf` from `apple2tc/support.h`, which is already included. The five sites, in order:

```cpp
    // Inconsistent depth on re-entry:
        reject(
            entry,
            stringPrintf(
                "block $%04x reached at stack level %d and %d",
                bb->getAddress().value_or(0),
                it->second,
                entryDepth));
        return;

    // Indirect terminator:
      reject(entry, stringPrintf("terminator %s", getValueKindName(terminator->getKind())));
      return;

    // Pop8 underflow:
          reject(
              entry,
              stringPrintf(
                  "%s block $%04x stack level underflow",
                  getValueKindName(iRef.getKind()),
                  bb->getAddress().value_or(0)));
          return;

    // Other SP modification:
        reject(entry, stringPrintf("%s", getValueKindName(iRef.getKind())));
        return;

    // Non-zero depth at RTS:
          reject(
              entry,
              stringPrintf(
                  "block $%04x stack level %d at RTS",
                  bb->getAddress().value_or(0),
                  depth));
        return;
```

Also route the predecessor rejection near the end of `scanCandidate` (currently printing `"Invalid predecessor inst %s at $%04x"`):

```cpp
    reject(
        entry,
        stringPrintf(
            "invalid predecessor inst %s at $%04x",
            getValueKindName(inst->getKind()),
            inst->getAddress().value_or(0x10000)));
    return;
```

- [ ] **Step 4: Record cascade removals too**

A candidate dropped by `removeInvalidJSRs()` is accepted by `scanCandidate` but
removed later, so without this it appears in neither list. In
`removeInvalidJSRs()`, immediately before the existing `candidates_.erase(cur);`,
add:

```cpp
        rejected_.emplace(
            cur->first->getAddress().value_or(0x10000),
            stringPrintf(
                "invalid JSR to $%04x", jsrTarget->getAddress().value_or(0x10000)));
```

- [ ] **Step 5: Verify `stringPrintf` exists with this signature**

Run:
```bash
grep -n 'stringPrintf' /home/tmikov/work/apple2tc/include/apple2tc/support.h
```
Expected: a declaration returning `std::string` and taking a printf-style format. If it does not exist or differs, use `snprintf` into a `char buf[128]` and construct the `std::string` from it instead.

- [ ] **Step 6: Build and confirm behavior is unchanged**

Run:
```bash
ninja -C /home/tmikov/work/apple2tc/cmake-build-debug
cd /home/tmikov/work/apple2tc/tests && ./run-tests.sh ../cmake-build-debug
```
Expected: `Success!` — this task is pure refactoring, no output should change.

- [ ] **Step 7: Commit**

```bash
cd /home/tmikov/work/apple2tc
git add tools/apple2tc/routines.cpp
git commit -m "apple2tc: record routine rejection reasons for reporting"
```

---

## Task 5: Dominator tree and natural loops

The codebase has no dominator implementation. Phase 1's relooping needs one, and so does the report.

**Files:**
- Create: `tools/apple2tc/Dominators.h`
- Create: `tools/apple2tc/Dominators.cpp`
- Modify: `tools/apple2tc/CMakeLists.txt`

- [ ] **Step 1: Write the interface**

Create `tools/apple2tc/Dominators.h`:

```cpp
/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "ir/IR.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

/// Dominator tree over an explicit subset of a function's basic blocks. The
/// subset form is what routine candidates need: a candidate owns a set of
/// blocks that is generally smaller than the enclosing IR function.
struct DomTree {
  /// Blocks in reverse postorder from the entry. Blocks in the input set that
  /// are unreachable from the entry do not appear.
  std::vector<ir::BasicBlock *> rpo{};
  /// Index into rpo.
  std::unordered_map<ir::BasicBlock *, unsigned> rpoIndex{};
  /// Immediate dominator. The entry maps to itself.
  std::unordered_map<ir::BasicBlock *, ir::BasicBlock *> idom{};

  /// True if a dominates b. A block dominates itself.
  bool dominates(ir::BasicBlock *a, ir::BasicBlock *b) const;
};

/// A natural loop: a header plus every block that can reach a back edge tail
/// without leaving through the header.
struct NaturalLoop {
  ir::BasicBlock *header = nullptr;
  std::unordered_set<ir::BasicBlock *> body{};
};

/// Compute the dominator tree for `entry` restricted to `blocks`. Successor
/// edges leaving `blocks` are ignored.
DomTree computeDomTree(ir::BasicBlock *entry, const std::unordered_set<ir::BasicBlock *> &blocks);

/// Find natural loops. One entry per back edge; loops sharing a header are NOT
/// merged, so a header with two back edges yields two entries.
std::vector<NaturalLoop> findNaturalLoops(const DomTree &dt);
```

- [ ] **Step 2: Write the implementation**

Create `tools/apple2tc/Dominators.cpp`:

```cpp
/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Dominators.h"

#include <algorithm>
#include <vector>

using namespace ir;

bool DomTree::dominates(BasicBlock *a, BasicBlock *b) const {
  if (a == b)
    return true;
  auto it = idom.find(b);
  if (it == idom.end())
    return false;
  // Walk up from b to the root. The root maps to itself, so stop there.
  BasicBlock *cur = b;
  for (;;) {
    auto i = idom.find(cur);
    if (i == idom.end() || i->second == cur)
      return false;
    cur = i->second;
    if (cur == a)
      return true;
  }
}

DomTree computeDomTree(BasicBlock *entry, const std::unordered_set<BasicBlock *> &blocks) {
  DomTree dt{};

  // Iterative postorder DFS, restricted to `blocks`.
  std::vector<BasicBlock *> postorder{};
  std::unordered_set<BasicBlock *> visited{};
  std::vector<std::pair<BasicBlock *, bool>> stack{};
  stack.emplace_back(entry, false);
  while (!stack.empty()) {
    auto [bb, expanded] = stack.back();
    stack.pop_back();
    if (expanded) {
      postorder.push_back(bb);
      continue;
    }
    if (!visited.insert(bb).second)
      continue;
    stack.emplace_back(bb, true);
    for (auto &succ : successors(*bb)) {
      if (blocks.count(&succ) && !visited.count(&succ))
        stack.emplace_back(&succ, false);
    }
  }

  dt.rpo.assign(postorder.rbegin(), postorder.rend());
  for (unsigned i = 0; i < dt.rpo.size(); ++i)
    dt.rpoIndex[dt.rpo[i]] = i;

  if (dt.rpo.empty())
    return dt;

  // Cooper, Harvey and Kennedy, "A Simple, Fast Dominance Algorithm".
  dt.idom[entry] = entry;

  auto intersect = [&dt](BasicBlock *a, BasicBlock *b) {
    while (a != b) {
      while (dt.rpoIndex[a] > dt.rpoIndex[b])
        a = dt.idom[a];
      while (dt.rpoIndex[b] > dt.rpoIndex[a])
        b = dt.idom[b];
    }
    return a;
  };

  bool changed = true;
  while (changed) {
    changed = false;
    for (BasicBlock *bb : dt.rpo) {
      if (bb == entry)
        continue;
      BasicBlock *newIdom = nullptr;
      for (auto &predRef : predecessors(*bb)) {
        BasicBlock *pred = &predRef;
        if (!blocks.count(pred) || !dt.rpoIndex.count(pred))
          continue;
        if (!dt.idom.count(pred))
          continue; // Not processed yet on this sweep.
        newIdom = newIdom ? intersect(pred, newIdom) : pred;
      }
      if (newIdom && dt.idom[bb] != newIdom) {
        dt.idom[bb] = newIdom;
        changed = true;
      }
    }
  }

  return dt;
}

std::vector<NaturalLoop> findNaturalLoops(const DomTree &dt) {
  std::vector<NaturalLoop> loops{};

  for (BasicBlock *tail : dt.rpo) {
    for (auto &succRef : successors(*tail)) {
      BasicBlock *header = &succRef;
      if (!dt.rpoIndex.count(header))
        continue;
      // A back edge is tail -> header where header dominates tail.
      if (!dt.dominates(header, tail))
        continue;

      NaturalLoop loop{};
      loop.header = header;
      loop.body.insert(header);

      // Everything that reaches `tail` without passing through `header`.
      std::vector<BasicBlock *> stack{};
      if (tail != header) {
        loop.body.insert(tail);
        stack.push_back(tail);
      }
      while (!stack.empty()) {
        BasicBlock *bb = stack.back();
        stack.pop_back();
        for (auto &predRef : predecessors(*bb)) {
          BasicBlock *pred = &predRef;
          if (!dt.rpoIndex.count(pred))
            continue;
          if (loop.body.insert(pred).second)
            stack.push_back(pred);
        }
      }
      loops.push_back(std::move(loop));
    }
  }

  return loops;
}
```

- [ ] **Step 3: Add to the build**

In `tools/apple2tc/CMakeLists.txt`, add `Dominators.cpp` and `Dominators.h` to the source list alongside the existing `routines.cpp` entry. Read the file first to match its exact formatting.

- [ ] **Step 4: Build**

Run:
```bash
ninja -C /home/tmikov/work/apple2tc/cmake-build-debug
```
Expected: builds cleanly. Nothing calls the new code yet.

- [ ] **Step 5: Commit**

```bash
cd /home/tmikov/work/apple2tc
git add tools/apple2tc/Dominators.h tools/apple2tc/Dominators.cpp tools/apple2tc/CMakeLists.txt
git commit -m "apple2tc: add dominator tree and natural loop analysis"
```

---

## Task 6: Emit the routines report

**Files:**
- Modify: `tools/apple2tc/PubIR.h`
- Modify: `tools/apple2tc/routines.cpp`
- Modify: `tools/apple2tc/apple2tc.cpp`

- [ ] **Step 1: Widen the entry point**

In `tools/apple2tc/PubIR.h`, replace:

```cpp
bool identifySimpleRoutines(ir::Module *mod);
```

with:

```cpp
/// \param report if non-null, write a human-readable analysis of every routine
///     candidate — accepted and rejected — to this stream.
bool identifySimpleRoutines(ir::Module *mod, FILE *report = nullptr);
```

- [ ] **Step 2: Thread it through**

In `routines.cpp`, find the free function `identifySimpleRoutines` near the end of the file (outside the anonymous namespace). Change its signature to accept `FILE *report` and pass it to the `IdentifySimpleRoutines` constructor. Add a `FILE *const report_;` member to the class, initialize it in the constructor, and change the constructor to:

```cpp
  explicit IdentifySimpleRoutines(Function *func, FILE *report)
      : func_(func), ctx_(func->getModule()->getContext()), report_(report) {}
```

- [ ] **Step 3: Write the report**

In `routines.cpp`, add `#include "Dominators.h"` to the include block. Then add this method, and call it from `run()` immediately before `splitRoutines();`:

**Note:** `apple2tc.cpp` calls `identifySimpleRoutines` in a loop until it returns
false, so the report file will contain **one section per pass** — for Snake Byte,
a first section with all the accepted routines and a second reporting 0 accepted.
The first section is the useful one. The `===` header line makes them
distinguishable; do not add pass-numbering complexity for this.

```cpp
void IdentifySimpleRoutines::emitReport() {
  if (!report_)
    return;

  // Accepted candidates, sorted by address for stable output.
  std::vector<std::pair<uint32_t, BasicBlock *>> accepted{};
  accepted.reserve(candidates_.size());
  for (auto &p : candidates_)
    accepted.emplace_back(p.first->getAddress().value_or(0x10000), p.first);
  std::sort(accepted.begin(), accepted.end());

  fprintf(report_, "=== routine candidates: %zu accepted, %zu rejected ===\n\n",
          accepted.size(), rejected_.size());

  for (auto [addr, entry] : accepted) {
    Candidate &cand = candidates_.at(entry);
    fprintf(report_, "ACCEPT $%04X  %zu blocks\n", addr, cand.blocks.size());

    // Call sites.
    fprintf(report_, "  called from:");
    std::vector<uint32_t> callers{};
    for (Instruction &iRef : predecessorInsts(*entry)) {
      if (iRef.getKind() == ValueKind::JSR && iRef.getOperand(0) == entry)
        callers.push_back(iRef.getAddress().value_or(0x10000));
    }
    std::sort(callers.begin(), callers.end());
    for (uint32_t c : callers)
      fprintf(report_, " $%04X", c);
    fprintf(report_, "  (%zu sites)\n", callers.size());

    // Blocks in reverse postorder, with immediate dominators.
    DomTree dt = computeDomTree(entry, cand.blocks);
    fprintf(report_, "  blocks (rpo, idom):");
    for (BasicBlock *bb : dt.rpo) {
      auto it = dt.idom.find(bb);
      fprintf(
          report_,
          " $%04X<-$%04X",
          bb->getAddress().value_or(0xFFFF),
          it == dt.idom.end() ? 0xFFFF : it->second->getAddress().value_or(0xFFFF));
    }
    fprintf(report_, "\n");

    // Natural loops.
    std::vector<NaturalLoop> loops = findNaturalLoops(dt);
    if (loops.empty()) {
      fprintf(report_, "  loops: none\n");
    } else {
      for (const NaturalLoop &loop : loops) {
        std::vector<uint32_t> body{};
        for (BasicBlock *bb : loop.body)
          body.push_back(bb->getAddress().value_or(0xFFFF));
        std::sort(body.begin(), body.end());
        fprintf(report_, "  loop header $%04X body:", loop.header->getAddress().value_or(0xFFFF));
        for (uint32_t a : body)
          fprintf(report_, " $%04X", a);
        fprintf(report_, "\n");
      }
    }
    fprintf(report_, "\n");
  }

  for (auto &[addr, reason] : rejected_)
    fprintf(report_, "REJECT $%04X  %s\n", addr, reason.c_str());
  fprintf(report_, "\n");
}
```

Declare it in the class `private:` section as `void emitReport();`.

- [ ] **Step 4: Add the CLI option**

In `tools/apple2tc/apple2tc.cpp`:

Add to `printHelp()`, after the `--run-data` line:
```cpp
  fprintf(stderr, "  --routines-report=f Write routine candidate analysis to file\n");
```

Add a local next to `std::string runDataPath;`:
```cpp
  std::string routinesReportPath;
```

Add to the option loop, before the `if (argv[i][0] == '-')` fallback:
```cpp
    if (strncmp(argv[i], "--routines-report=", 18) == 0) {
      routinesReportPath = argv[i] + 18;
      continue;
    }
```

Replace the routine identification loop:
```cpp
      if (optLevel > 1) {
        while (identifySimpleRoutines(mod)) {
        }
      }
```
with:
```cpp
      if (optLevel > 1) {
        FILE *report = nullptr;
        if (!routinesReportPath.empty()) {
          if ((report = fopen(routinesReportPath.c_str(), "wt")) == nullptr) {
            perror(routinesReportPath.c_str());
            return 2;
          }
        }
        while (identifySimpleRoutines(mod, report)) {
        }
        if (report)
          fclose(report);
      }
```

- [ ] **Step 5: Build**

Run:
```bash
ninja -C /home/tmikov/work/apple2tc/cmake-build-debug
```
Expected: builds cleanly.

- [ ] **Step 6: Generate the Snake Byte report and sanity-check it**

Run:
```bash
cd /home/tmikov/work/apple2tc/decoded/snake-byte
../../cmake-build-debug/tools/apple2tc/apple2tc snake-byte.b33 \
  --run-data=snake-byte.json -O3 --irc1 \
  --routines-report=/tmp/snake-routines.txt > /dev/null
head -40 /tmp/snake-routines.txt
grep -c '^ACCEPT' /tmp/snake-routines.txt
grep '^REJECT' /tmp/snake-routines.txt
```
Expected: `ACCEPT` entries with block lists, idom chains and loop bodies; `REJECT` lines naming `$7230`, `$6A32`, `$7226` among others.

- [ ] **Step 7: Spot-check one loop against the listing**

Pick any `loop header $XXXX` line for a game-range routine and confirm against `snake-byte.lst` that the header really is a branch target from inside its own body. A routine known to contain a loop is `$6217` (the keyboard scan at `$621F`).

Run:
```bash
grep -A3 'ACCEPT \$6217' /tmp/snake-routines.txt
grep -n '^/\*621F\*/\|^/\*6229\*/\|^/\*622E\*/' /home/tmikov/work/apple2tc/decoded/snake-byte/snake-byte.lst
```
Expected: the reported loop body corresponds to blocks that branch backwards in the listing. If the loop set looks wrong, stop — the dominator code is incorrect and Phase 1 would inherit the error.

- [ ] **Step 8: Run the regression suite**

Run:
```bash
cd /home/tmikov/work/apple2tc/tests && ./run-tests.sh ../cmake-build-debug
```
Expected: `Success!` — the report is write-only and must not alter IR output.

- [ ] **Step 9: Commit**

```bash
cd /home/tmikov/work/apple2tc
git add tools/apple2tc/PubIR.h tools/apple2tc/routines.cpp tools/apple2tc/apple2tc.cpp
git commit -m "apple2tc: add --routines-report with block sets, dominators and loops"
```

---

## Task 7: Per-frame hashing in decapplib

**Design note — deviation from the spec, deliberate.** The spec says "framebuffer hash". This hashes **video memory** (text page + hi-res page + video mode) rather than the rendered RGB screen. Three reasons: `update_screen()` derives its blink phase from wall-clock `stm_ms(...)`, so a rendered hash is not reproducible; hashing memory is strictly more sensitive; and Snake Byte's lo-res shadow grid lives in the text page, so this covers the game's collision state, not just its pixels. The cycle count is included per frame, which makes the trace a timing check as well.

**Caveat to carry into Phase 2:** changing the lo-res grid representation will change these hashes by design. That slice needs a different comparison — note it when the Phase 2 plan is written.

**Files:**
- Modify: `lib/decapplib/decapplib.c`

- [ ] **Step 1: Add the globals**

In `decapplib.c`, after the line `static bool trace_keys_ = false;`, add:

```c
/// If set, write a per-frame hash of video state to this file.
static FILE *hash_file_ = NULL;
/// If non-zero, quit after this many frames.
static unsigned frame_limit_ = 0;
/// Frames simulated so far.
static unsigned frame_no_ = 0;
```

- [ ] **Step 2: Add the hash function**

Add immediately before `static void update_screen(void) {`:

```c
/// FNV-1a over the video state: mode, mixed flag, text page and hires page.
/// Deliberately hashes memory rather than rendered pixels — rendering depends on
/// wall-clock time for the blink phase and would not be reproducible.
static uint64_t hash_video_state(void) {
  uint64_t h = 1469598103934665603ULL;
  const uint8_t *ram = get_ram();

  uint8_t header[2] = {
      (uint8_t)a2_io_get_vidmode(&io_), (uint8_t)a2_io_is_vidmode_mixed(&io_)};
  for (unsigned i = 0; i < sizeof(header); ++i) {
    h ^= header[i];
    h *= 1099511628211ULL;
  }

  const uint8_t *text = ram + a2_io_get_text_page_offset(&io_);
  for (unsigned i = 0; i < 0x400; ++i) {
    h ^= text[i];
    h *= 1099511628211ULL;
  }

  const uint8_t *hires = ram + a2_io_get_hires_page_offset(&io_);
  for (unsigned i = 0; i < 0x2000; ++i) {
    h ^= hires[i];
    h *= 1099511628211ULL;
  }

  return h;
}
```

- [ ] **Step 3: Emit one line per frame and honour the limit**

In `frame_cb()`, replace:

```c
  curFrameTick_ = stm_now();
  simulate_frame();
  update_screen();
```

with:

```c
  curFrameTick_ = stm_now();
  simulate_frame();

  if (hash_file_) {
    fprintf(hash_file_, "%u %u %016llx\n", frame_no_, get_cycles(),
            (unsigned long long)hash_video_state());
  }
  ++frame_no_;
  if (frame_limit_ && frame_no_ >= frame_limit_) {
    if (hash_file_) {
      fclose(hash_file_);
      hash_file_ = NULL;
    }
    sapp_request_quit();
  }

  update_screen();
```

- [ ] **Step 4: Add the CLI options**

In `print_help()`, after the `--trace-keys` line:

```c
  printf(" --hash-frames=p  Write per-frame video state hashes to the given file\n");
  printf(" --frames=n       Quit after simulating n frames\n");
```

In `parse_args()`, before the final unknown-option handling:

```c
    if (strncmp(arg, "--hash-frames=", 14) == 0) {
      const char *path = arg + 14;
      if ((hash_file_ = fopen(path, "wt")) == NULL) {
        perror(path);
        exit(2);
      }
      continue;
    }
    if (strncmp(arg, "--frames=", 9) == 0) {
      frame_limit_ = (unsigned)strtoul(arg + 9, NULL, 10);
      continue;
    }
```

- [ ] **Step 5: Force deterministic pacing when hashing**

Hashing is only meaningful under fixed cycles-per-frame. In `simulate_frame()`, change:

```c
    if (trace_keys_ || key_presses_ || (g_debug & (DebugASM | DebugMem)) != 0) {
```
to:
```c
    if (trace_keys_ || key_presses_ || hash_file_ || (g_debug & (DebugASM | DebugMem)) != 0) {
```

- [ ] **Step 6: Build**

Run:
```bash
ninja -C /home/tmikov/work/apple2tc/cmake-build-debug
```
Expected: builds cleanly.

- [ ] **Step 7: Commit**

```bash
cd /home/tmikov/work/apple2tc
git add lib/decapplib/decapplib.c
git commit -m "decapplib: add --hash-frames and --frames for deterministic replay traces"
```

---

## Task 8: Headless mode

Hashing video memory rather than rendered pixels means the oracle needs no
graphics context at all. `sokol_main()` runs before any window exists, so a
headless path can run the whole simulation there and exit — no `SOKOL_NO_ENTRY`,
no build changes.

This task extracts the shared setup and per-frame logic rather than copying it,
so the GUI and headless paths cannot drift apart.

**Files:**
- Modify: `lib/decapplib/decapplib.c`

- [ ] **Step 1: Add the flag**

After the `static unsigned frame_no_ = 0;` line added in Task 7:

```c
/// If true, run without opening a window or initialising graphics/audio.
static bool headless_ = false;
```

- [ ] **Step 2: Extract emulation setup from `init_cb`**

`init_cb()` currently mixes graphics setup with emulator setup. Split it. Add
immediately before `static void init_cb(void) {`:

```c
/// Everything needed to start the emulated program, with no graphics or audio.
/// Shared by the windowed and headless paths.
static void init_emulation(void) {
  a2_io_init(&io_);
  io_.debug = 0;

  add_default_nondebug();
  reset_regs();
  // SP is 0xF0 in BASIC.
  regs_t r = get_regs();
  r.sp = 0xF0;
  set_regs(r);

  init_emulated();

  if (kbd_file_ || key_presses_) {
    // The first key pressed before initialization is lost, so just add a dummy
    // keypress.
    a2_io_push_key(&io_, '\r');
    if (key_presses_)
      drain_key_presses();
    else
      drain_kbd_file();
  }
}
```

Then rewrite `init_cb()` to:

```c
static void init_cb(void) {
  init_window();
  stm_setup();

  a2_sound_init(&sound_);
  init_emulation();
  a2_io_set_spkr_cb(&io_, &sound_, speaker_cb);

  if (sound_enabled_) {
    saudio_desc audioDesc = {
        .num_channels = 1,
        .stream_userdata_cb = stream_userdata_cb,
        .user_data = &sound_,
    };
    saudio_setup(&audioDesc);
  }
}
```

Note the reordering: `a2_sound_init` must precede `a2_io_set_spkr_cb`, and
`init_emulation` calls `a2_io_init`, which must precede the speaker callback
being attached. Preserve that order.

- [ ] **Step 3: Extract the per-frame hash emission**

Replace the block added to `frame_cb()` in Task 7 with a call to a shared
helper. Add before `static void frame_cb(void) {`:

```c
/// Emit this frame's hash if requested and advance the frame counter.
/// Returns true when the frame limit has been reached.
static bool record_frame(void) {
  if (hash_file_) {
    fprintf(hash_file_, "%u %u %016llx\n", frame_no_, get_cycles(),
            (unsigned long long)hash_video_state());
  }
  ++frame_no_;
  if (frame_limit_ && frame_no_ >= frame_limit_) {
    if (hash_file_) {
      fclose(hash_file_);
      hash_file_ = NULL;
    }
    return true;
  }
  return false;
}
```

Then in `frame_cb()`, the code added in Task 7 becomes:

```c
  curFrameTick_ = stm_now();
  simulate_frame();

  if (record_frame())
    sapp_request_quit();

  update_screen();
```

- [ ] **Step 4: Guard the audio submission**

`simulate_frame()` calls `a2_sound_submit(..., saudio_sample_rate(), ...)`
unconditionally, but `saudio_setup()` only runs when sound is enabled. In
headless mode saudio is never initialised at all. Change the line in
`simulate_frame()` from:

```c
    a2_sound_submit(&sound_, A2_CLOCK_FREQ, saudio_sample_rate(), get_cycles());
```
to:
```c
    if (sound_enabled_)
      a2_sound_submit(&sound_, A2_CLOCK_FREQ, saudio_sample_rate(), get_cycles());
```

This also fixes the existing `--no-sound` path, which was calling
`saudio_sample_rate()` on an uninitialised audio backend.

- [ ] **Step 5: Add the headless loop**

Add immediately before `sapp_desc sokol_main(int argc, char *argv[]) {`:

```c
/// Run the emulated program with no window, hashing frames as we go. Never
/// returns — exits the process when the frame limit is reached.
static void run_headless(void) {
  if (!frame_limit_) {
    fprintf(stderr, "--headless requires --frames=<n>\n");
    exit(2);
  }

  stm_setup();
  a2_sound_init(&sound_);
  init_emulation();

  for (;;) {
    curFrameTick_ = stm_now();
    simulate_frame();
    if (record_frame())
      break;
  }

  shutdown_emulated();
  a2_io_done(&io_);
  a2_sound_done(&sound_);
  exit(0);
}
```

`sound_enabled_` is irrelevant here because Step 4 guards the only audio call,
but `a2_sound_init`/`a2_sound_done` are still paired so the speaker queue is
valid if the emulated code touches `$C030`.

- [ ] **Step 6: Wire up the option**

In `print_help()`, after the `--frames` line added in Task 7:

```c
  printf(" --headless       Run with no window. Requires --frames\n");
```

In `parse_args()`, alongside the other options:

```c
    if (strcmp(arg, "--headless") == 0) {
      headless_ = true;
      sound_enabled_ = false;
      continue;
    }
```

In `sokol_main()`, immediately after `parse_args(argc, argv);`:

```c
  if (headless_)
    run_headless(); // Does not return.
```

- [ ] **Step 7: Build**

Run:
```bash
ninja -C /home/tmikov/work/apple2tc/cmake-build-debug
```
Expected: builds cleanly.

- [ ] **Step 8: Verify it runs with no display**

Run:
```bash
cd /home/tmikov/work/apple2tc/decoded/snake-byte
env -u DISPLAY -u WAYLAND_DISPLAY \
  ../../cmake-build-debug/decoded/snake-byte/snake-bytec1 \
  --headless --key-file=play.keys --frames=100 --hash-frames=/tmp/hl.frames
wc -l /tmp/hl.frames
awk '{print $3}' /tmp/hl.frames | sort -u | wc -l
```
Expected: exits 0 with no window, `/tmp/hl.frames` has 100 lines, and more than
one distinct hash.

If it fails with a graphics or display error, something in the headless path is
still reaching sokol_gfx or sokol_app — check that `run_headless` is called
before `sokol_main` returns its `sapp_desc`.

- [ ] **Step 9: Verify headless and windowed agree**

The two paths must produce identical traces, or the oracle means different things
in each mode. This needs a display.

Run:
```bash
cd /home/tmikov/work/apple2tc/decoded/snake-byte
bin=../../cmake-build-debug/decoded/snake-byte/snake-bytec1
$bin --headless --key-file=play.keys --frames=100 --hash-frames=/tmp/a.frames
$bin --no-sound --key-file=play.keys --frames=100 --hash-frames=/tmp/b.frames
diff /tmp/a.frames /tmp/b.frames && echo "MATCH"
```
Expected: `MATCH`.

If they differ, the windowed path is perturbing emulation — most likely through
`event_cb` injecting keys, or audio timing. Resolve before continuing: the
golden trace is recorded headless and would otherwise be uncomparable to
anything run interactively.

- [ ] **Step 10: Commit**

```bash
cd /home/tmikov/work/apple2tc
git add lib/decapplib/decapplib.c
git commit -m "decapplib: add --headless for display-free replay

Extracts init_emulation() and record_frame() so the windowed and headless
paths share setup and per-frame logic. Also guards a2_sound_submit on
sound_enabled_, which was calling saudio_sample_rate() on an uninitialised
backend under --no-sound."
```

---

## Task 9: Prove the oracle is deterministic, then record it

An oracle that is not reproducible is worse than none. Verify before trusting.

**Files:**
- Create: `decoded/snake-byte/verify.sh`
- Create: `decoded/snake-byte/play.frames`

**Prerequisite:** none — Task 8 made this runnable without a display.

- [ ] **Step 1: Determine the frame count**

`play.keys` ends at cycle 20033808. At `1/60 * A2_CLOCK_FREQ` cycles per frame, the replay needs roughly 1180 frames. Use 1300 to cover the tail.

Run:
```bash
grep -n 'A2_CLOCK_FREQ' /home/tmikov/work/apple2tc/include/apple2tc/apple2iodefs.h
tail -1 /home/tmikov/work/apple2tc/decoded/snake-byte/play.keys
```
Expected: a clock frequency near 1020484, and a final key stamp of 20033808. Confirm `20033808 / (clock_freq/60)` is below 1300; adjust the frame count in the next step if not.

- [ ] **Step 2: Write the verification script**

Create `decoded/snake-byte/verify.sh`:

```bash
#!/bin/bash
# Replay play.keys against the decompiled builds and compare per-frame video
# state hashes. With one argument, records a new golden trace. With none,
# checks the existing builds against play.frames.
set -e

bin=${BIN:-../../cmake-build-debug}
frames=1300
here=$(cd "$(dirname "$0")" && pwd)
cd "$here"

run() {
  # $1: executable, $2: output path
  "$1" --headless --key-file=play.keys --frames=$frames --hash-frames="$2" >/dev/null
}

if [ "$1" = "--record" ]; then
  run "$bin/decoded/snake-byte/snake-bytec1" /tmp/sb-record-a.frames
  run "$bin/decoded/snake-byte/snake-bytec1" /tmp/sb-record-b.frames
  if ! diff -q /tmp/sb-record-a.frames /tmp/sb-record-b.frames; then
    echo "ERROR: oracle is not reproducible - two runs of the same binary differ" >&2
    diff /tmp/sb-record-a.frames /tmp/sb-record-b.frames | head -20 >&2
    exit 1
  fi
  cp /tmp/sb-record-a.frames play.frames
  echo "Recorded $(wc -l < play.frames) frames to play.frames"
  exit 0
fi

run "$bin/decoded/snake-byte/snake-bytec1" /tmp/sb-check.frames
if diff -u play.frames /tmp/sb-check.frames > /tmp/sb-diff.txt; then
  echo "PASS: $(wc -l < play.frames) frames match"
else
  echo "FAIL: first divergence:" >&2
  head -10 /tmp/sb-diff.txt >&2
  exit 1
fi
```

Then:
```bash
chmod +x /home/tmikov/work/apple2tc/decoded/snake-byte/verify.sh
```

- [ ] **Step 3: Record, which also proves reproducibility**

Run:
```bash
cd /home/tmikov/work/apple2tc/decoded/snake-byte
./verify.sh --record
```
Expected: `Recorded 1300 frames to play.frames`.

If it reports the oracle is not reproducible, **stop**. Likely causes, in order of probability: a nondeterministic cycle path (check that `hash_file_` was added to the `simulate_frame()` condition in Task 7 Step 5), or genuine nondeterminism in the emulated thread handoff between `run_emulated` and `cycles_expired`. Audio is already excluded — `--headless` forces `sound_enabled_ = false`. Diagnose before continuing — every later task depends on this.

- [ ] **Step 4: Sanity-check the trace is not degenerate**

Run:
```bash
cd /home/tmikov/work/apple2tc/decoded/snake-byte
awk '{print $3}' play.frames | sort -u | wc -l
head -3 play.frames
tail -3 play.frames
```
Expected: several hundred distinct hashes, not 1. A single repeated hash means the game never rendered — check that the replay actually reached the game rather than sitting at the BASIC prompt.

- [ ] **Step 5: Confirm the check path passes**

Run:
```bash
cd /home/tmikov/work/apple2tc/decoded/snake-byte
./verify.sh
```
Expected: `PASS: 1300 frames match`.

- [ ] **Step 6: Commit**

```bash
cd /home/tmikov/work/apple2tc
git add decoded/snake-byte/verify.sh decoded/snake-byte/play.frames
git commit -m "snake-byte: add frame-hash oracle and golden trace

verify.sh --record runs the c1 build twice and refuses to record unless both
runs agree, so the trace is only written once reproducibility is proven."
```

---

## Task 10: Update the playbook from what actually happened

The playbook's *Procedure* section is explicitly marked untested. Phase 0 is the first evidence.

**Files:**
- Modify: `docs/decompiling/playbook.md`

- [ ] **Step 1: Correct the findings**

Update these entries in `docs/decompiling/playbook.md` with measured results:

- Under *Procedure recovery*, the `[apple2tc]` entry about per-block stack balance: change "Check whether this has been fixed" to state that it **was** fixed, note the commit, and record the actual number of routines recovered from Task 3.
- Under *Verification*, add whether the frame-hash oracle proved reproducible on the first attempt, and if not, what made it nondeterministic. That is high-value transferable knowledge.
- Change the maturity note from "Everything under *Procedure* is a plan that has not yet been tested" to reflect that steps 1–3 have now been executed once, leaving 4–5 untested.

- [ ] **Step 2: Add any new red flags**

If anything in Phase 0 surprised you — a rejection reason not in the spec, a nondeterminism source, a dominator edge case — add a row to the red-flag table.

- [ ] **Step 3: Commit**

```bash
cd /home/tmikov/work/apple2tc
git add docs/decompiling/playbook.md
git commit -m "docs: update decompilation playbook from Phase 0 results"
```

---

## Done criteria

- [ ] `tests/run-tests.sh ../cmake-build-debug` prints `Success!`
- [ ] `tests/phapla.ir` exists and shows `function func_0320` extracted
- [ ] Snake Byte recovers more routines than the previous 53; the count is recorded in the decision log
- [ ] `--routines-report` emits accepted candidates with block sets, call sites, idom chains and natural loops, and rejected candidates with reasons
- [ ] `--headless --frames=n` runs to completion with `DISPLAY` unset
- [ ] Headless and windowed runs produce byte-identical traces
- [ ] `decoded/snake-byte/verify.sh --record` proves reproducibility before writing `play.frames`
- [ ] `decoded/snake-byte/verify.sh` passes against the committed trace
- [ ] The playbook reflects measured outcomes, not predictions

## Explicitly out of scope

- Regenerating `decoded/robotron/robotronc1.c` and `decoded/bolo/bolo.c`, which are stale after the routines fix
- Any change to Snake Byte game code — Phase 1
- Special handling for `$7230`, `$6A32` (`Pop8` underflow) or `$7226` (`JmpInd`); these stay rejected and are handled by hand in Phase 1
