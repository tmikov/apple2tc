/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "IR.h"
#include "IRUtil.h"

#include <cstdio>
#include <string>

namespace ir {

class IRDumper {
public:
  explicit IRDumper(FILE *os, bool trees = true);
  ~IRDumper();

  void dump(Module *mod);
  void dump(Function *func);
  void dump(BasicBlock *block);
  void dumpInstruction(Instruction *inst) {
    dumpInstructionImpl(nullptr, inst);
  }

  const std::string &name(Value *v);

private:
  void dumpImpl(BasicBlock *bb);
  /// Dump a basic block using tree representation.
  void dumpBlockTrees(BasicBlock *bb);
  void printTree(const InstSet &validTrees, Value *inst);
  void dumpInstructionImpl(const InstSet *validTrees, Instruction *inst);
  void printOperand(Value *operand);

protected:
  virtual void preFunction(Function *func);
  virtual void postFunction(Function *func);
  virtual void preBasicBlock(BasicBlock *bb);
  virtual void postBasicBlock(BasicBlock *bb);

protected:
  FILE *os_;
  bool trees_ = true;

private:
  struct PerFunction {
    IRNamer namer{};
    int instNameWidth = 0;
    size_t instCount = 0;
    size_t anonBlockCount = 0;
    uint32_t lastAddr = ~(uint32_t)0;

    void clear() {
      namer.clear();
      instNameWidth = 0;
      instCount = 0;
      anonBlockCount = 0;
      lastAddr = ~(uint32_t)0;
    }
  };

  PerFunction perFunction_{};
  IRNamer namer_{};
  size_t anonFuncCount_ = 0;
};

} // namespace ir
