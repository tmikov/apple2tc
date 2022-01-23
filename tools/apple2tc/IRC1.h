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

// Generates a straight-forward 1-to1 mapping from IR to C.
class IRC1 {
  IRContext *const ctx_;
  Module *const mod_;
  FILE *const os_;
  bool const trees_;

  std::string obuf_{};

  /// Map basic blocks to integer IDs.
  std::unordered_map<const BasicBlock *, unsigned> blockIDs_{};
  /// Next blockID to assign.
  unsigned nextBlockID_ = 0;

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

public:
  IRC1(Module *mod, FILE *os, bool trees)
      : ctx_(mod->getContext()), mod_(mod), os_(os), trees_(trees) {}
  ~IRC1() = default;

  void run();

private:
  /// Return an existing or assign a new block ID.
  unsigned blockID(const Value *bb);

  const TmpVar *getTmpVar(TypeKind type);
  void freeTmpVar(const TmpVar *);

  const TmpVar *varFor(const Instruction *inst);

  /// Allocate temporaries to all values.
  void regAlloc(Function *func);
  /// Register allocation in a basic block in non-tree mode.
  void regAllocBB(BasicBlock *bb);
  /// Register allocation in a basic block in tree mode.
  void regAllocBBTrees(BasicBlock *bb);

  void printPrologue();
  void printEpilogue();
  void printAddr2BlockMap(const std::vector<BasicBlock *> &sortedBlocks);
  void printBB(BasicBlock *bb);
  std::string formatInst(Instruction *inst);
  void printInst(Instruction *inst);
  std::string formatOperand(Value *operand);

  /// Print setting of the branchTarget flag, if necessary.
  void printBranchTarget(Instruction *inst);

#define IR_INST(name, type) void print##name(Instruction *inst);
#include "ir/Values.def"
};