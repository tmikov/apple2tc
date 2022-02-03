/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "IRDump.h"

#include "IRUtil.h"

#include "apple2tc/support.h"

namespace ir {

IRDumper::IRDumper(FILE *os, bool trees) : os_(os), trees_(trees) {}
IRDumper::~IRDumper() = default;

const std::string &IRDumper::name(Value *v) {
  if (isa<Function>(v)) {
    return namer_.getName(v, [this](Value *_v) {
      auto *func = cast<Function>(_v);
      if (!func->getName().empty())
        return format("FUNC_%s", func->getName().c_str());
      else if (func->getAddress().has_value() && func->isRealAddress())
        return format("func_%04x", *func->getAddress());
      else
        return format("func%zu", ++anonFuncCount_);
    });
  } else if (isa<BasicBlock>(v)) {
    return perFunction_.namer.getName(v, [this](Value *_v) {
      auto *bb = cast<BasicBlock>(_v);
      if (!bb->getName().empty())
        return format("%%BB_%s", bb->getName().c_str());
      else if (bb->getAddress().has_value() && bb->isRealAddress())
        return format("%%bb_%04x", *bb->getAddress());
      else
        return format("%%bb%zu", ++perFunction_.anonBlockCount);
    });
  } else if (isa<Instruction>(v)) {
    return perFunction_.namer.getName(v, [this](Value *v) {
      auto res = format("%%%zu", ++perFunction_.instCount);
      if (res.size() > perFunction_.instNameWidth)
        perFunction_.instNameWidth = res.size();
      return res;
    });
  } else {
    assert(false && "Invalid value to name");
    static std::string empty{};
    return empty;
  }
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
  auto sortedBlocks = sortByAddress<BasicBlock>(func->basicBlocks(), true);

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
  InstSet validTrees{};
  markExpressionTrees(bb, validTrees);
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
