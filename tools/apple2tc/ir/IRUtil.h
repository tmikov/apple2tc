/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "IR.h"

#include <functional>
#include <unordered_map>
#include <unordered_set>

namespace ir {

namespace detail {
template <class T>
T *toPointer(T *x) {
  return x;
}
template <class T>
T *toPointer(T &x) {
  return &x;
}

template <class T>
inline auto sizeIfPresent(const T *t) -> decltype(t->size()) {
  return t->size();
}
inline auto sizeIfPresent(const void *) -> size_t {
  return 0;
}

} // namespace detail

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

/// A setting informing algorithms whether to process unreachable blocks.
/// Sometimes it is necessary to visit unreachable blocks. For example:
///     - when the control flow is not fully known
///     - when there are multiple exits and we are operating on the inverse graph.
enum class VisitUnreachable {
  /// Do not visit unreachable blocks.
  No,
  /// Visit blocks without predecessors.
  ExceptLoops,
  /// Visit all remaining unvisited blocks. They might have predecessors and
  /// still be unreachable if they belong to unreachable loops.
  All,
};

template <class T, class Range, class Traits = GraphTraits>
class GenPostOrder {
  VisitUnreachable const vu_;
  const Traits &traits_;
  std::unordered_set<T *> visited_{};
  std::vector<T *> order_{};

  explicit GenPostOrder(VisitUnreachable vu, Traits const &traits) : vu_(vu), traits_(traits) {}

public:
  /// \param entryBlock optional entry point to visit virst.
  static std::vector<T *>
  run(VisitUnreachable vu, const Range &range, T *entryBlock, const Traits &traits = Traits()) {
    GenPostOrder obj(vu, traits);
    obj.runImpl(range, entryBlock);
    return std::move(obj.order_);
  }

private:
  void runImpl(const Range &range, T *entryBlock) {
    // Visit all blocks directly reachable from the entry point.
    if (entryBlock)
      visit(entryBlock);
    if (vu_ >= VisitUnreachable::ExceptLoops) {
      // Visit all blocks without known predecessors.
      for (auto &rbb : range) {
        if (traits_.predecessors(&rbb).empty())
          visit(&rbb);
      }
    }
    if (vu_ >= VisitUnreachable::All) {
      // Visit all remaining unvisited blocks. They are parts of unreachable loops.
      for (auto &rbb : range)
        visit(&rbb);
    }
  }

  inline void visit(T *bb) {
    if (!visited_.insert(bb).second)
      return;
    visitSlowPath(bb);
  }

  void visitSlowPath(T *bb) {
    for (auto &succ : traits_.successors(bb))
      visit(detail::toPointer(succ));
    order_.push_back(bb);
  }
};

template <class T, class Range, class Traits = GraphTraits>
std::vector<T *>
genPostOrder(VisitUnreachable vu, const Range &range, T *entry, const Traits &traits = Traits()) {
  return GenPostOrder<T, Range, Traits>::run(vu, range, entry, traits);
}

template <class T, class Range, class Traits = GraphTraits>
class GenEntryBlocks {
  VisitUnreachable const vu_;
  const Traits &traits_;
  std::unordered_set<T *> visited_{};
  std::vector<T *> entryPoins_{};

  explicit GenEntryBlocks(VisitUnreachable vu, Traits const &traits) : vu_(vu), traits_(traits) {}

public:
  /// \param entryBlock optional entry point to visit virst.
  static std::vector<T *>
  run(VisitUnreachable vu, const Range &range, T *entryBlock, const Traits &traits = Traits()) {
    GenEntryBlocks obj(vu, traits);
    obj.runImpl(range, entryBlock);
    return std::move(obj.entryPoins_);
  }

private:
  void runImpl(const Range &range, T *entryPoint) {
    // Visit all blocks directly reachable from the entry point.
    if (entryPoint)
      visit(entryPoint, true);
    if (vu_ >= VisitUnreachable::ExceptLoops) {
      // Visit all blocks without known predecessors.
      for (auto &rbb : range) {
        if (traits_.predecessors(&rbb).empty())
          visit(&rbb, true);
      }
    }
    if (vu_ >= VisitUnreachable::All) {
      // Visit all remaining unvisited blocks. They are parts of unreachable loops.
      for (auto &rbb : range)
        visit(&rbb, true);
    }
  }

  inline bool visit(T *bb, bool addToEPs) {
    if (!visited_.insert(bb).second)
      return false;
    visitSlowPath(bb, addToEPs);
    return true;
  }

  void visitSlowPath(T *bb, bool addToEPs) {
    if (addToEPs)
      entryPoins_.push_back(bb);
    for (auto &succ : traits_.successors(bb))
      visit(detail::toPointer(succ), false);
  }
};

template <class T, class Range, class Traits = GraphTraits>
std::vector<T *> genEntryBlocks(
    VisitUnreachable vu,
    const Range &range,
    T *entryBlock,
    const Traits &traits = Traits()) {
  return GenEntryBlocks<T, Range, Traits>::run(vu, range, entryBlock, traits);
}

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

class IRNamer {
  std::unordered_set<std::string> nameSet_{};
  std::unordered_map<const Value *, const std::string *> valueMap_{};

public:
  void clear() {
    nameSet_.clear();
    valueMap_.clear();
  }

  const std::string &getName(Value *v, const std::function<std::string(Value *v)> &genName);

  const std::string &getExistingName(Value *v) const;
};

/// Sort a sequence of blocks using their addresses, when available. Block 0
/// is assumed to be the entry point and is optionally not sorted.
template <class T, class R>
void sortInplaceByAddress(R &blocks, bool excludeFirst) {
  // Note that we are optionally not sorting the entry block.
  if (blocks.size() > 1 + excludeFirst) {
    std::sort(blocks.begin() + excludeFirst, blocks.end(), [](T *a, T *b) -> bool {
      auto addrA = a->getAddress().value_or(0x10000);
      auto addrB = b->getAddress().value_or(0x10000);
      if (addrA < addrB)
        return true;
      else if (addrA == addrB)
        return a->getUniqueId() < b->getUniqueId();
      else
        return false;
    });
  }
}

/// Sort a sequence of blocks using their addresses, when available. Block 0
/// is assumed to be the entry point and is optionally not sorted.
template <class T, class R>
std::vector<T *> sortByAddress(const R &range, bool excludeFirst) {
  std::vector<T *> sortedBlocks{};
  sortedBlocks.reserve(detail::sizeIfPresent(&range));
  for (auto &x : range)
    sortedBlocks.push_back(detail::toPointer(x));
  sortInplaceByAddress<T>(sortedBlocks, excludeFirst);
  return sortedBlocks;
}

} // namespace ir
