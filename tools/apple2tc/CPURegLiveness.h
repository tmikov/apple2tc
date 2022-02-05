/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "Analysis.h"
#include "ir/IR.h"
#include "ir/IRDump.h"

#include "apple2tc/SetVector.h"

using namespace ir;

class CPURegLiveness {
public:
  using SetType = uint16_t;
  static constexpr unsigned kSetWidth = 16;
  struct LivenessSets {
    static constexpr SetType kInvalidSet = 1 << (kSetWidth - 1);
    static_assert((unsigned)CPURegKind::_last < kSetWidth, "We need one extra bit for invalid");
    static constexpr SetType kAll = (1 << (unsigned)CPURegKind::_last) - 1;

    /// Registers used in the block before being assigned.
    SetType gen = 0;
    /// Registers assigned in the block.
    SetType kill = 0;
    /// We need to initialize liveIn to the invalid set, so it will always change
    /// at least once.
    SetType liveIn = kInvalidSet;
    SetType liveOut = 0;

    SetType genOut = 0;
    SetType killOut = 0;
  };

  static inline SetType m(const CPUReg8 *reg) {
    return 1 << static_cast<unsigned>(reg->getValue());
  }
  static inline SetType m(const Value *reg) {
    return m(cast<const CPUReg8>(reg));
  }

  template <typename F>
  static void forEachInSet(SetType set, F f) {
    for (unsigned i = 0; i < (unsigned)CPURegKind::_last; ++i)
      if (set & (1 << i))
        f(static_cast<CPURegKind>(i));
  }

private:
  struct FuncData {
    LivenessSets sets{};
    /// Registers modified by the function.
    SetType modified = 0;
  };

  Module *const mod_;
  CallGraph const cg_;
  std::unordered_map<const BasicBlock *, LivenessSets> bbSets_{};
  std::unordered_map<const Function *, FuncData> funcData_{};

  explicit CPURegLiveness(Module *mod);

public:
  ~CPURegLiveness();

  static std::unique_ptr<CPURegLiveness> calc(Module *mod) {
    std::unique_ptr<CPURegLiveness> obj(new CPURegLiveness(mod));
    obj->run();
    return obj;
  }

  CallGraph const &getCallGraph() const {
    return cg_;
  }

  const LivenessSets &bbSets(const BasicBlock *bb) const {
    auto it = bbSets_.find(bb);
    assert(it != bbSets_.end());
    return it->second;
  }

  const FuncData &getFuncData(const Function *func) const {
    auto it = funcData_.find(func);
    assert(it != funcData_.end());
    return it->second;
  }

private:
  void run();

  /// Calculate Gen/Kill sets for the module.
  void calcGenKillMod();

  void calcLiveness();

  /// Calculate the Gen/Kill sets for the entire function.
  bool calcGenKill(Function *func);

  /// Reset all blocks LiveIn/LiveOut sets after liveness has been run once,
  /// so it can be run again.
  void resetBlocks(Function *func);

  /// Calculate liveness and update the function liveIn, liveOut. Return true
  /// if they changed.
  bool calcFuncLiveIn(Function *func);

  /// Return the Gen and Kill sets (in that order) of an instruction.
  std::pair<SetType, SetType> instGenKill(Instruction *inst) const;
  /// Initialize as basic block's Gen, Kill and LiveOut sets.
  void initBB(BasicBlock *bb);
  /// Reset a basic block LiveIn/LiveOut sets after liveness has been run once,
  /// so it can be run again.
  void resetBB(BasicBlock *bb);
  bool updateBlockGenKillOut(BasicBlock *bb);
  bool updateBlockLiveInOut(BasicBlock *bb);
  /// Derive the live out set for a given instruction from the values of the
  /// basic block.
  SetType deriveInstLiveOut(Instruction *inst);

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

  /// Return true if changed.
  bool livenessInverseRPO(Function *func);

  /// Alternate implementation of liveness calculation using a work list. Not
  /// used, but kept around for reference.
  void livenessWorkList(Function *func);
};

class CPULivenessDumper : public IRDumper {
public:
  struct Flags {
    bool dumpBlocks;
    bool dumpFuncs;
  };

private:
  const CPURegLiveness &liveness_;
  Flags const flags_;

public:
  CPULivenessDumper(FILE *os, bool trees, CPURegLiveness const &liveness, Flags flags)
      : IRDumper(os, trees), liveness_(liveness), flags_(flags) {}

protected:
  void preFunction(Function *func) override {
    if (!flags_.dumpFuncs)
      return;
    fprintf(os_, "// Gen    :");
    printVarList(liveness_.getFuncData(func).sets.gen);
    fprintf(os_, "\n");
    fprintf(os_, "// Kill   :");
    printVarList(liveness_.getFuncData(func).sets.kill);
    fprintf(os_, "\n");
    fprintf(os_, "// Mod    :");
    printVarList(liveness_.getFuncData(func).modified);
    fprintf(os_, "\n");
    fprintf(os_, "// LiveIn :");
    printVarList(liveness_.getFuncData(func).sets.liveIn);
    fprintf(os_, "\n");
    fprintf(os_, "// LiveOut:");
    printVarList(liveness_.getFuncData(func).sets.liveOut);
    fprintf(os_, "\n");
  }

  void preBasicBlock(BasicBlock *bb) override {
    if (!flags_.dumpBlocks)
      return;
    fprintf(os_, "// Gen    :");
    printVarList(liveness_.bbSets(bb).gen);
    fprintf(os_, "\n");
    fprintf(os_, "// Kill   :");
    printVarList(liveness_.bbSets(bb).kill);
    fprintf(os_, "\n");
    fprintf(os_, "// LiveIn :");
    printVarList(liveness_.bbSets(bb).liveIn);
    fprintf(os_, "\n");
  }
  void postBasicBlock(BasicBlock *bb) override {
    if (!flags_.dumpBlocks)
      return;
    fprintf(os_, "// LiveOut:");
    printVarList(liveness_.bbSets(bb).liveOut);
    fprintf(os_, "\n");
  }

private:
  void printVarList(const CPURegLiveness::SetType &set) {
    bool first = true;
    for (unsigned i = 0; i != (unsigned)CPURegKind::_last; ++i) {
      if (set & (1 << i)) {
        if (first)
          fprintf(os_, " ");
        else
          fprintf(os_, ", ");
        first = false;
        fprintf(os_, "%s", getCPURegKindName((CPURegKind)i));
      }
    }
  }
};
