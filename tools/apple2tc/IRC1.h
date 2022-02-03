/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "Disas.h"
#include "ir/IR.h"
#include "ir/IRUtil.h"

#include "apple2tc/support.h"

#include <cstdio>
#include <unordered_map>
#include <unordered_set>

using namespace ir;

class IRC1Mod;

// Generates a straight-forward 1-to1 mapping from IR to C.
class IRC1 {
  IRC1Mod *const c1mod_;
  IRContext *const ctx_;
  Module *const mod_;
  Function *const func_;
  FILE *const os_;
  bool const trees_;
  const char *name_;

  std::string obuf_{};

  /// Map basic blocks to integer IDs.
  std::unordered_map<const BasicBlock *, unsigned> blockIDs_{};
  /// Next blockID to assign.
  unsigned nextBlockID_ = 0;

  /// Blocks for which we require CPU address mapping.
  std::vector<BasicBlock *> sortedDynamicBlocks_{};

  /// Descriptor of a single temporary variable.
  struct TmpVar {
    TypeKind type;
    unsigned num;
    std::string name;

    TmpVar(TypeKind type, unsigned int num)
        : type(type), num(num), name(format("tmp%u_%s", num, getTypeKindName(type))) {}
  };

  /// All temporary variables used by the register allocator.
  std::deque<TmpVar> tmpVars_{};
  /// Lists of available (free) temporary variables by type.
  std::vector<std::vector<const TmpVar *>> freeTmps_{(unsigned)TypeKind::_last};

  /// Temporary variable allocated to each instruction.
  std::unordered_map<const Instruction *, const TmpVar *> varMapping_{};

  /// When in tree mode, contains all instructions that are part of expression trees
  /// and should not be emitted sequentially as part of the instruction list.
  InstSet validTrees_{};

  // A helper class for allocating registers in a basic block.
  class BBAllocator;

  /// The next basic block when printing the current one.
  BasicBlock *nextBB_ = nullptr;

public:
  IRC1(IRC1Mod *c1mod, Function *func, FILE *os, bool trees);
  ~IRC1();

  void runFunc();

private:
  void scanForDynamicBlocks();

  /// Whether the function needs "dynamic basic blocks".
  bool needDynamicBlocks() const {
    return !sortedDynamicBlocks_.empty();
  }

  /// Return an existing or assign a new block ID.
  unsigned blockID(const Value *bb);

  const TmpVar *getTmpVar(TypeKind type);
  void freeTmpVar(const TmpVar *);

  const TmpVar *varFor(const Instruction *inst);

  /// Allocate temporaries to all values.
  void regAlloc();
  /// Register allocation in a basic block in non-tree mode.
  void regAllocBB(BasicBlock *bb);
  /// Register allocation in a basic block in tree mode.
  void regAllocBBTrees(BasicBlock *bb);

  void printAddr2BlockMap();
  void printBB(BasicBlock *bb);
  std::string formatInst(Instruction *inst);
  void printInst(Instruction *inst);
  std::string formatOperand(Value *operand);

  /// Return true if this instruction needs to set branchTarget = true.
  bool needBranchTarget(Instruction *inst);
  /// Print setting of the branchTarget flag, if necessary. Return true if
  /// printed.
  bool printBranchTarget(Instruction *inst);

#define IR_INST(name, type) void print##name(Instruction *inst);
#include "ir/Values.def"
};

class IRC1Mod {
  IRContext *const ctx_;
  Module *const mod_;
  FILE *const os_;
  bool const trees_;

  IRNamer namer_{};

public:
  IRC1Mod(Module *mod, FILE *os, bool trees)
      : ctx_(mod->getContext()), mod_(mod), os_(os), trees_(trees) {}
  ~IRC1Mod() = default;

  void run();

  const char *getName(Function *func) const {
    return namer_.getExistingName(func).c_str();
  }

private:
  void printPrologue();
  void printEpilogue();
};