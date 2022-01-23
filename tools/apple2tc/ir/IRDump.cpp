/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "IRDump.h"

#include "IRUtil.h"

namespace ir {

IRDumper::IRDumper(FILE *os, bool trees) : os_(os), trees_(trees) {}
IRDumper::~IRDumper() = default;

const std::string &IRDumper::name(Value *v) {
  char buf[32];
  std::unordered_map<Value *, std::string> *map;

  if (isa<Function>(v)) {
    auto it = globalNames_.find(v);
    if (it != globalNames_.end())
      return it->second;
    snprintf(buf, sizeof(buf), "Func%04zu", ++funcCount_);
    map = &globalNames_;
  } else if (isa<Instruction>(v)) {
    auto it = perFunction_.names.find(v);
    if (it != perFunction_.names.end())
      return it->second;
    int len = snprintf(buf, sizeof(buf), "%%%zu", ++perFunction_.instCount);
    if (len > perFunction_.instNameWidth)
      perFunction_.instNameWidth = len;
    map = &perFunction_.names;
  } else if (isa<BasicBlock>(v)) {
    auto it = perFunction_.names.find(v);
    if (it != perFunction_.names.end())
      return it->second;
    snprintf(buf, sizeof(buf), "%%BB%zu", ++perFunction_.blockCount);
    map = &perFunction_.names;
  } else {
    assert(false && "Invalid value to name");
    static std::string empty{};
    return empty;
  }

  return map->try_emplace(v, buf).first->second;
}

void IRDumper::dump(Module *mod) {
  // First visit all functions to ensure the names are properly ordered.
  for (auto &func : mod->functions())
    name(&func);
  for (auto &func : mod->functions()) {
    dump(&func);
    fprintf(os_, "\n");
  }
}

void IRDumper::dump(Function *func) {
  auto sortedBlocks = sortBasicBlocksByAddress(func->basicBlocks());

  // Name all basic blocks and instructions consistently first.
  for (auto *bb : sortedBlocks) {
    name(bb);
    for (auto &inst : bb->instructions())
      name(&inst);
  }

  fprintf(os_, "function %s\n", name(func).c_str());
  for (auto *bb : sortedBlocks)
    dumpImpl(bb);
  fprintf(os_, "end %s\n", name(func).c_str());
}

void IRDumper::dump(BasicBlock *bb) {
  // Name all basic blocks and instructions consistently first.
  for (auto &inst : bb->instructions())
    name(&inst);
  dumpImpl(bb);
}

void IRDumper::dumpImpl(BasicBlock *bb) {
  fprintf(os_, "%s:  // ", name(bb).c_str());
  if (bb->getAddress())
    fprintf(os_, "[$%04X] ", *bb->getAddress());
  // Predecessors and successors.
  {
    unsigned i;

    fprintf(os_, "Pred(");
    i = 0;
    for (auto &pred : predecessors(*bb)) {
      if (i++)
        fprintf(os_, ", ");
      fprintf(os_, "%s", name(&pred).c_str());
    }
    fprintf(os_, ") ");
    fprintf(os_, "Succ(");
    i = 0;
    for (auto &succ : successors(*bb)) {
      if (i++)
        fprintf(os_, ", ");
      fprintf(os_, "%s", name(&succ).c_str());
    }
    fprintf(os_, ")");
  }

  fprintf(os_, "\n");
  preBasicBlock(bb);
  if (trees_) {
    dumpBlockTrees(bb);
  } else {
    for (auto &inst : bb->instructions())
      dumpInstruction(&inst);
  }
  postBasicBlock(bb);
}

void IRDumper::preBasicBlock(BasicBlock *bb) {}
void IRDumper::postBasicBlock(BasicBlock *bb) {}

namespace {} // namespace

void IRDumper::dumpBlockTrees(BasicBlock *bb) {
  InstSet trees{};
  InstSet validTrees{};
  std::vector<Instruction *> stack{};

  for (auto &rInst : bb->instructions()) {
    auto *inst = &rInst;
    Instruction *onlyUser;
    if (!inst->getType()->isVoid() && !inst->hasSideEffects() &&
        (onlyUser = inst->getOnlyUser()) != nullptr && onlyUser->getBasicBlock() == bb) {
      // Is this instruction suitable for adding to an expression tree?
      // A non-void instruction without side effects, with exactly one user in
      // the same basic block.
      trees.insert(inst);
      validTrees.insert(inst);
    } else {
      // This instruction can't be a part of an expression tree. It has to be
      // emitted. All referenced trees are cleared.
      stack.push_back(inst);
      do {
        auto *toClear = stack.back();
        stack.pop_back();
        trees.erase(toClear);
        for (auto &op : toClear->operands())
          if (auto *opInst = dyn_cast<Instruction>(&op))
            stack.push_back(opInst);
      } while (!stack.empty());

      // Check if it invalidated any tree leaves.
      if (inst->getKind() == ValueKind::StoreR8) {
        // Invalidate register leaves.
        auto *cpuReg = inst->getOperand(0);
        for (auto begin = trees.begin(), end = trees.end(); begin != end;) {
          auto cur = begin++;
          auto *tInst = *cur;
          if (tInst->getKind() == ValueKind::LoadR8 && tInst->getOperand(0) == cpuReg) {
            trees.erase(cur);
            validTrees.erase(tInst);
          }
        }
      } else if (inst->writesMemory()) {
        auto [writeAddr, writeWidth] = inst->memoryAddress();
        auto writeRange = classifyMemoryAddr(writeAddr, writeWidth);

        // Invalidate memory read leaves.
        for (auto begin = trees.begin(), end = trees.end(); begin != end;) {
          auto cur = begin++;
          auto *tInst = *cur;
          if (tInst->readsMemory()) {
            auto [readAddr, readWidth] = tInst->memoryAddress();
            auto readRange = classifyMemoryAddr(readAddr, readWidth);
            if (writeRange.overlaps(readRange)) {
              trees.erase(cur);
              validTrees.erase(tInst);
            }
          }
        }
      }
    }
  }
  assert(trees.empty() && "All trees must have been consumed by the end of the basic block");

  for (auto &rInst : bb->instructions()) {
    auto *inst = &rInst;
    // Part of a tree? Skip it.
    if (validTrees.count(inst))
      continue;
    dumpInstructionImpl(&validTrees, inst);
  }
}

void IRDumper::printTree(const InstSet &validTrees, Value *value) {
  Instruction *inst;
  if (!(inst = dyn_cast<Instruction>(value)) || !validTrees.count(inst))
    return printOperand(value);

  fprintf(os_, "(%s", getValueKindName(inst->getKind()));
  for (auto &op : inst->operands()) {
    fputc(' ', os_);
    printTree(validTrees, &op);
  }
  fputc(')', os_);
}

void IRDumper::dumpInstructionImpl(const InstSet *validTrees, Instruction *inst) {
  if (inst->getAddress() && *inst->getAddress() != perFunction_.lastAddr) {
    perFunction_.lastAddr = *inst->getAddress();
    fprintf(os_, "/*$%04X*/", perFunction_.lastAddr);
  } else {
    fprintf(os_, "         ");
  }

  if (inst->getType()->getKind() != TypeKind::Void) {
    if (inst->hasUsers())
      fprintf(os_, " %-*s = ", perFunction_.instNameWidth, name(inst).c_str());
    else
      fprintf(os_, " %-*s = ", perFunction_.instNameWidth, "%_");
  } else {
    fprintf(os_, " %-*s   ", perFunction_.instNameWidth, "");
  }
  fprintf(os_, "%-*s", inst->getNumOperands() ? 10 : 0, getValueKindName(inst->getKind()));
  unsigned index = 0;
  for (auto &operand : inst->operands()) {
    if (index++ == 0)
      fprintf(os_, "    ");
    else
      fprintf(os_, ", ");

    if (validTrees)
      printTree(*validTrees, &operand);
    else
      printOperand(&operand);
  }
  fprintf(os_, "\n");
}

void IRDumper::printOperand(Value *operand) {
  if (auto *u8 = dyn_cast<LiteralU8>(operand)) {
    fprintf(os_, "0x%02x", u8->getValue());
  } else if (auto *u16 = dyn_cast<LiteralU16>(operand)) {
    fprintf(os_, "0x%04x", u16->getValue());
  } else if (auto *u32 = dyn_cast<LiteralU32>(operand)) {
    fprintf(os_, "%u", u32->getValue());
  } else if (auto *reg8 = dyn_cast<CPUReg8>(operand)) {
    fprintf(os_, "%s", getCPURegKindName(reg8->getValue()));
  } else if (auto *reg16 = dyn_cast<CPUReg16>(operand)) {
    fprintf(os_, "%s", getCPURegKindName(reg16->getValue()));
  } else {
    fprintf(os_, "%s", name(operand).c_str());
  }
}

} // namespace ir
