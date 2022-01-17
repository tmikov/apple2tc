/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "PubIR.h"
#include "ir/IR.h"
#include "ir/IRDump.h"
#include "ir/IRUtil.h"

#include <deque>
#include <unordered_set>

using namespace ir;

namespace {

class CalcCPURegLiveness {
public:
  using SetType = uint16_t;
  static constexpr unsigned kSetWidth = 16;
  struct BBSets {
    /// Registers used in the block before being assigned.
    SetType gen = 0;
    /// Registers assigned in the block.
    SetType kill = 0;
    /// We need to initialize liveIn to the invalid set, so it will always change
    /// at least once.
    SetType liveIn = kInvalidSet;
    SetType liveOut = 0;

    static constexpr SetType kInvalidSet = 1 << (kSetWidth - 1);
    static_assert((unsigned)CPURegKind::_last < kSetWidth, "We need one extra bit for invalid");
    static constexpr SetType kAll = (1 << (unsigned)CPURegKind::_last) - 1;
  };

  static inline SetType m(const CPUReg8 *reg) {
    return 1 << static_cast<unsigned>(reg->getValue());
  }
  static inline SetType m(const Value *reg) {
    return m(cast<const CPUReg8>(reg));
  }

private:
  std::unordered_map<const BasicBlock *, BBSets> bbSets_{};
  Function *const func_;

public:
  static CalcCPURegLiveness calc(Function *func) {
    CalcCPURegLiveness obj(func);
    if (true)
      obj.runInverseRPO();
    else
      obj.runWorkList();
    return obj;
  }

  const BBSets &bbSets(const BasicBlock *bb) const {
    auto it = bbSets_.find(bb);
    assert(it != bbSets_.end());
    return it->second;
  }

private:
  explicit CalcCPURegLiveness(Function *func) : func_(func) {}

  bool updateBlock(BasicBlock *bb);

  // Iterative liveness of the blocks in order, as expected, is quite inefficient.
  //    1723 blocks, 29291 liveness calculations, 17 iterations
  // Using post order results in a significant improvement:
  //    1723 blocks, 6892 liveness calculations, 4 iterations
  // RPO on inverse graph is the same:
  //    1723 blocks, 6892 liveness calculations, 4 iterations
  // Work list:
  //    1723 blocks, 3170 liveness calculations
  //
  // Keeping the last two for reference. The work list performs fewer (1/2)
  // calculations, but is significantly more complicated, and I suspect it is
  // not actually faster in practice.

  void runInverseRPO();
  void runWorkList();
  void initBB(BasicBlock *bb);
};

void CalcCPURegLiveness::initBB(BasicBlock *bb) {
  BBSets bbs{};

  for (auto &iRef : bb->instructions()) {
    Instruction *inst = &iRef;
    if (inst->getKind() == ValueKind::LoadR8)
      bbs.gen |= ~bbs.kill & m(inst->getOperand(0));
    else if (inst->getKind() == ValueKind::StoreR8)
      bbs.kill |= m(inst->getOperand(0));
  }
  auto *term = bb->getTerminator();
  assert(term && "Every basic block must have a terminator");
  if (term->isIndirectBranch())
    bbs.liveOut = BBSets::kAll;

  bbSets_[bb] = bbs;
}

bool CalcCPURegLiveness::updateBlock(BasicBlock *bb) {
  auto &bbs = bbSets_[bb];
  for (auto &succ : successors(*bb))
    bbs.liveOut |= bbSets_[&succ].liveIn;
  SetType in = bbs.gen | (bbs.liveOut & ~bbs.kill);
  bool changed = bbs.liveIn ^ in;
  bbs.liveIn = in;
  return changed;
}

void CalcCPURegLiveness::runInverseRPO() {
  for (auto &rbb : func_->basicBlocks())
    initBB(&rbb);

  auto order = GenPostOrder<InverseGraphTraits>::run(func_, nullptr);

  unsigned bbCount = 0;
  unsigned iter = 0;
  bool changed;
  do {
    changed = false;
    ++iter;
    for (auto *bb : makeReverseRange(order)) {
      ++bbCount;
      changed |= updateBlock(bb);
    }
  } while (changed);

  fprintf(stderr, "%u liveness calculations, %u iterations\n", bbCount, iter);
}

void CalcCPURegLiveness::runWorkList() {
  unsigned blockCount = 0;
  for (auto &rbb : func_->basicBlocks()) {
    ++blockCount;
    initBB(&rbb);
  }

  auto exitBlocks = GenEntryBlocks<InverseGraphTraits>::run(func_, nullptr);

  std::unordered_set<BasicBlock *> inList{};
  std::deque<BasicBlock *> workList{};

  auto addToList = [&inList, &workList](BasicBlock *bb) {
    if (inList.insert(bb).second)
      workList.push_back(bb);
  };
  auto popFromList = [&inList, &workList]() -> BasicBlock * {
    auto *bb = workList.front();
    workList.pop_front();
    bool erased = inList.erase(bb);
    (void)erased;
    assert(erased && "The popped element must be marked as in-list");
    return bb;
  };

  unsigned numCalc = 0;
  for (auto *bb : exitBlocks)
    addToList(bb);

  while (!workList.empty()) {
    auto *bb = popFromList();
    ++numCalc;
    if (updateBlock(bb)) {
      for (auto &pred : predecessors(*bb))
        addToList(&pred);
    }
  }

  fprintf(stderr, "%u blocks, %u liveness calculations\n", blockCount, numCalc);
}

class LivenessDumper : public IRDumper {
  const CalcCPURegLiveness &liveness_;

public:
  LivenessDumper(FILE *os, CalcCPURegLiveness const &liveness)
      : IRDumper(os), liveness_(liveness) {}

protected:
  void preBasicBlock(BasicBlock *bb) override {
    fprintf(os_, "// Gen    : ");
    printVarList(liveness_.bbSets(bb).gen);
    fprintf(os_, "\n");
    fprintf(os_, "// Kill   : ");
    printVarList(liveness_.bbSets(bb).kill);
    fprintf(os_, "\n");
    fprintf(os_, "// LiveIn : ");
    printVarList(liveness_.bbSets(bb).liveIn);
    fprintf(os_, "\n");
  }
  void postBasicBlock(BasicBlock *bb) override {
    fprintf(os_, "// LiveOut: ");
    printVarList(liveness_.bbSets(bb).liveOut);
    fprintf(os_, "\n");
  }

private:
  void printVarList(const CalcCPURegLiveness::SetType &set) {
    bool first = true;
    for (unsigned i = 0; i != (unsigned)CPURegKind::_last; ++i) {
      if (set & (1 << i)) {
        if (!first)
          fprintf(os_, ", ");
        first = false;
        fprintf(os_, "%s", getCPURegKindName((CPURegKind)i));
      }
    }
  }
};

} // namespace

/// Replace register loads with a previous load or a previously stored value.
static bool removeRegLoads(BasicBlock *bb) {
  // Map from CPU register to its last known value.
  std::unordered_map<Value *, Value *> knownRegs{};
  InstDestroyer destroyer;
  bool changed = false;

  for (auto &iRef : bb->instructions()) {
    auto *inst = &iRef;
    if (inst->getKind() == ValueKind::StoreR8) {
      // Remember the latest stored value.
      knownRegs[inst->getOperand(0)] = inst->getOperand(1);
    } else if (inst->getKind() == ValueKind::LoadR8) {
      auto &entry = knownRegs[inst->getOperand(0)];
      if (entry) {
        inst->replaceAllUsesWith(entry);
        destroyer.add(inst);
        changed = true;
      } else {
        // Record the load itself as the latest value.
        entry = inst;
      }
    }
  }
  return changed;
}

/// Remove register stores if they are overwritten by a second store.
static bool removeRegStores(BasicBlock *bb, const CalcCPURegLiveness &liveness) {
  // Map from CPU register to the last store.
  std::unordered_map<Value *, Instruction *> lastStore{};
  InstDestroyer destroyer;
  bool changed = false;

  for (auto &iRef : bb->instructions()) {
    auto *inst = &iRef;

    if (inst->getKind() == ValueKind::StoreR8) {
      auto &entry = lastStore[inst->getOperand(0)];
      // If there was a previous store, delete it.
      if (entry) {
        destroyer.add(entry);
        changed = true;
      }
      // Remember this as the last store.
      entry = inst;
    } else if (inst->getKind() == ValueKind::LoadR8) {
      // A load "validates" any prior store.
      lastStore[inst->getOperand(0)] = nullptr;
    }
  }

  // Eliminate stores to registers that are not live out.
  const auto &bbSets = liveness.bbSets(bb);
  for (auto [cpuRegVal, inst] : lastStore) {
    if (!inst)
      continue;
    if ((bbSets.liveOut & CalcCPURegLiveness::m(cpuRegVal)) == 0) {
      destroyer.add(inst);
      changed = true;
    }
  }

  return changed;
}

bool localCPURegSSA(Module *mod) {
  bool changed = false;
  for (auto &func : mod->functions()) {
    auto liveness = CalcCPURegLiveness::calc(&func);
    if (false)
      LivenessDumper(stdout, liveness).dump(&func);
    for (auto &bb : func.basicBlocks()) {
      changed |= removeRegLoads(&bb);
      changed |= removeRegStores(&bb, liveness);
    }
  }
  return changed;
}
