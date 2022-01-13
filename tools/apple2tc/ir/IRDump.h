/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "IR.h"

#include <cstdio>
#include <string>
#include <unordered_map>

namespace ir {

class IRDumper {
public:
  explicit IRDumper(FILE *os);

  void dump(Module *mod);
  void dump(Function *func);
  void dump(BasicBlock *block);
  void dump(Instruction *inst);

  const std::string &name(Value *v);

private:
  void dumpImpl(BasicBlock *bb);
  void printOperand(Value *operand);

private:
  FILE *os_;

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
