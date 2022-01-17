/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "IRDump.h"

namespace ir {

IRDumper::IRDumper(FILE *os) : os_(os) {}

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
  std::vector<BasicBlock *> sortedBlocks{};

  // Sort the blocks by starting address using a stable sort.
  for (auto &bb : func->basicBlocks())
    sortedBlocks.push_back(&bb);

  // Note that we are not sorting the entry block.
  std::stable_sort(
      sortedBlocks.begin() + 1, sortedBlocks.end(), [](BasicBlock *a, BasicBlock *b) -> bool {
        // No address is less than address.
        if (!a->getAddress())
          return b->getAddress().has_value();
        if (!b->getAddress())
          return false;
        return a->getAddress() < b->getAddress();
      });

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
  fprintf(os_, "%s:", name(bb).c_str());
  if (bb->getAddress())
    fprintf(os_, "  // $%04X", *bb->getAddress());
  fprintf(os_, "\n");
  for (auto &inst : bb->instructions())
    dump(&inst);
}

void IRDumper::dump(Instruction *inst) {
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
  fprintf(os_, "%-10s", getValueKindName(inst->getKind()));
  unsigned index = 0;
  for (auto &operand : inst->operands()) {
    if (index++ == 0)
      fprintf(os_, "    ");
    else
      fprintf(os_, ", ");

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
