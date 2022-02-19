/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "PubIR.h"
#include "ir/IR.h"
#include "ir/IRUtil.h"

#include "apple2tc/SetVector.h"
#include "apple2tc/support.h"

#include <deque>
#include <map>

using namespace ir;

namespace {

class IdentifySimpleRoutines {
  Function *const func_;
  IRContext *const ctx_;

  struct Routine {
    /// The blocks of this routine in the original main routine and how they
    /// map to the cloned blocks.
    std::unordered_map<BasicBlock *, BasicBlock *> blocks;
    /// The function blocks and their mapping back to original blocks.
    std::unordered_map<BasicBlock *, BasicBlock *> funcBlocks{};
    std::unordered_set<BasicBlock *> jsrTargets;
    std::unordered_set<Instruction *> rts;
    Function *func;
    /// If true, this is the main routine, so there are no cloned blocks.
    /// Blocks is a set, clonenBlocks is empty.
    bool root;
    /// If this flag is set, the routines contains extra blocks that are not
    /// tracked. So it cannot be outlines from larger routines. However smaller
    /// routines can still be outlines from it.
    bool dirty = false;

    Routine(
        std::unordered_map<BasicBlock *, BasicBlock *> &&blocks,
        std::unordered_set<BasicBlock *> &&jsrTargets,
        std::unordered_set<Instruction *> &&rts,
        Function *func = nullptr,
        bool root = false)
        : blocks(std::move(blocks)),
          jsrTargets(std::move(jsrTargets)),
          rts(std::move(rts)),
          func(func),
          root(root) {}

    void addFuncBlock(BasicBlock *newBB, BasicBlock *oldBB) {
      assert(!blocks[oldBB]);
      assert(!funcBlocks[newBB]);
      blocks[oldBB] = newBB;
      funcBlocks[newBB] = oldBB;
    }

    BasicBlock *funcBlockFromOriginal(BasicBlock *original) {
      assert(blocks[original]);
      return blocks[original];
    }

    BasicBlock *originalFromFuncBlock(BasicBlock *funcBlock) {
      if (root) {
        assert(blocks[funcBlock]);
        return funcBlock;
      } else {
        assert(funcBlocks[funcBlock]);
        return funcBlocks[funcBlock];
      }
    }

    void removeFuncBlock(BasicBlock *funcBlock) {
      if (root) {
        assert(blocks.count(funcBlock));
        blocks.erase(funcBlock);
      } else {
        auto it = funcBlocks.find(funcBlock);
        assert(it != funcBlocks.end());
        assert(blocks.count(it->second));
        blocks.erase(it->second);
        funcBlocks.erase(it);
      }
      // Can't be used as small func anymore.
      dirty = true;
    }
  };

  using RoutineMapT = std::unordered_map<BasicBlock *, Routine>;
  using RoutinePair = RoutineMapT::value_type;
  RoutineMapT routines_{};
  std::multimap<BasicBlock *, Routine *> blocks_{};

public:
  explicit IdentifySimpleRoutines(Function *func)
      : func_(func), ctx_(func->getModule()->getContext()) {}
  bool run();

private:
  /// Collect the basic block of the candidate.
  void scanCandidate(BasicBlock *entry);
  /// Remove candidates that JSR into a non-candidate. Return true if anything
  /// changed.
  bool removeInvalidJSRs();

  /// Actually split the candidates into new IR routines.
  void splitRoutines();

  void convertInvocations(BasicBlock *entry, Routine &cand);

  bool convertRoutineInvocation(
      IRBuilder &builder,
      BasicBlock *entry,
      const Routine &cand,
      Instruction *inst,
      const PrimitiveSetVector<BasicBlock *> &dynamicReturnBlocks);

  static void extractRoutine(IRBuilder &builder, BasicBlock *entry, Routine &cand);

  /// Create a Routine structure for the main routine.
  RoutinePair wrapMainRoutine();
  void outlineRoutines();
  void outline(RoutinePair &small, RoutinePair &large);
};

} // namespace
