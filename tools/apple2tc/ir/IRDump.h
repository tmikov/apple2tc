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
#include <unordered_map>
#include <unordered_set>

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
  virtual void preBasicBlock(BasicBlock *bb);
  virtual void postBasicBlock(BasicBlock *bb);

protected:
  FILE *os_;
  bool trees_ = true;

private:
  struct PerFunction {
    std::unordered_map<Value *, std::string> names{};
    int instNameWidth = 0;
    size_t instCount = 0;
    size_t blockCount = 0;
    uint32_t lastAddr = ~(uint32_t)0;

    void clear() {
      names.clear();
      instNameWidth = 0;
      instCount = 0;
      blockCount = 0;
      lastAddr = ~(uint32_t)0;
    }
  };

  PerFunction perFunction_{};
  std::unordered_map<Value *, std::string> globalNames_;
  size_t funcCount_ = 0;
};

} // namespace ir
