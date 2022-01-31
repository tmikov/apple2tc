/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "IR.h"

#include <unordered_set>

namespace ir {

struct GraphTraits {
  static auto predecessors(BasicBlock *bb) {
    return ::ir::predecessors(*bb);
  }
  static auto successors(BasicBlock *bb) {
    return ::ir::successors(*bb);
  }
};

struct InverseGraphTraits {
  static auto predecessors(BasicBlock *bb) {
    return ::ir::successors(*bb);
  }
  static auto successors(BasicBlock *bb) {
    return ::ir::predecessors(*bb);
  }
};

template <class Traits = GraphTraits>
class GenPostOrder {
  std::unordered_set<BasicBlock *> visited_{};
  std::vector<BasicBlock *> order_{};

public:
  /// \param entryBlock optional entry point to visit virst.
  static std::vector<BasicBlock *> run(Function *func, BasicBlock *entryBlock) {
    GenPostOrder obj;
    obj.runImpl(func, entryBlock);
    return std::move(obj.order_);
  }

private:
  void runImpl(Function *func, BasicBlock *entryBlock) {
    // Visit all blocks directly reachable from the entry point.
    if (entryBlock)
      visit(entryBlock);
    // Count how many blocks there are, for debugging.
    unsigned bbCount = 0;
    // Visit all blocks without known predecessors.
    for (auto &rbb : func->basicBlocks()) {
      if (Traits::predecessors(&rbb).empty())
        visit(&rbb);
      ++bbCount;
    }
    // Visit all remaining unvisited blocks. They are parts of unreachable loops.
    for (auto &rbb : func->basicBlocks())
      visit(&rbb);
    assert(bbCount == order_.size() && "All blocks must have been visited");
  }

  inline void visit(BasicBlock *bb) {
    if (!visited_.insert(bb).second)
      return;
    visitSlowPath(bb);
  }

  void visitSlowPath(BasicBlock *bb) {
    for (auto &succ : Traits::successors(bb))
      visit(&succ);
    order_.push_back(bb);
  }
};

template <class Traits = GraphTraits>
class GenEntryBlocks {
  std::unordered_set<BasicBlock *> visited_{};
  std::vector<BasicBlock *> entryPoins_{};

public:
  /// \param entryBlock optional entry point to visit virst.
  static std::vector<BasicBlock *> run(Function *func, BasicBlock *entryBlock) {
    GenEntryBlocks obj;
    obj.runImpl(func, entryBlock);
    return std::move(obj.entryPoins_);
  }

private:
  void runImpl(Function *func, BasicBlock *entryPoint) {
    // Visit all blocks directly reachable from the entry point.
    if (entryPoint)
      visit(entryPoint, true);
    // Visit all blocks without known predecessors.
    for (auto &rbb : func->basicBlocks()) {
      if (Traits::predecessors(&rbb).empty())
        visit(&rbb, true);
    }
    // Visit all remaining unvisited blocks. They are parts of unreachable loops.
    for (auto &rbb : func->basicBlocks())
      visit(&rbb, true);
  }

  inline bool visit(BasicBlock *bb, bool addToEPs) {
    if (!visited_.insert(bb).second)
      return false;
    visitSlowPath(bb, addToEPs);
    return true;
  }

  void visitSlowPath(BasicBlock *bb, bool addToEPs) {
    if (addToEPs)
      entryPoins_.push_back(bb);
    for (auto &succ : Traits::successors(bb))
      visit(&succ, false);
  }
};

/// Range32 takes into consideration 16-bit wraparound, where the end of the
/// range is larger than 0x10000, meaning it is actually the value & 0xFFFF.
/// This allows to encode essentially two 16-bit ranges into one.
struct Range32 {
  uint32_t begin;
  /// Exclusive.
  uint32_t end;

  static constexpr uint32_t k64K = 0x10000;

  constexpr Range32(uint32_t begin, uint32_t anEnd) : begin(begin), end(anEnd) {}

  bool wrapsAround() const {
    return this->end > k64K;
  }

  /// Checks whether the range overlaps, ignoring 16-bit wraparound.
  bool overlap32(Range32 r) const {
    return std::max(begin, r.begin) < std::min(end, r.end);
  }

  /// Check if the two ranges overlap, taking 16-bit wraparound in consideration.
  bool overlap16WithWrap(Range32 r) const;
};

/// Return a conservative range of the memory pointed by \p addr.
Range32 classifyMemoryAddr(Value *addr, unsigned width);

ValueKind negateComparison(ValueKind kind);

using InstSet = std::unordered_set<Instruction *>;

/// For a given basic block \p bb, populate a set of instructions that are
/// part of an expression tree and should not be emitted when encountered
/// sequentially in the block instruction list.
///
/// \param bb the basic block we are marking.
/// \param validTrees instructions that are part of a tree are recorded here
void markExpressionTrees(BasicBlock *bb, InstSet &validTrees);

} // namespace ir
