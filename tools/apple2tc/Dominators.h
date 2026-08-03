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
