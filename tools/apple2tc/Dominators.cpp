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
