/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "IRC1.h"

#include "apple2tc/apple2iodefs.h"
#include "apple2tc/d6502.h"
#include "apple2tc/support.h"

unsigned IRC1::blockID(const Value *bb) {
  auto res = blockIDs_.try_emplace(cast<const BasicBlock>(bb), 0);
  if (res.second)
    res.first->second = nextBlockID_++;
  return res.first->second;
}

const IRC1::TmpVar *IRC1::getTmpVar(TypeKind type) {
  auto &stack = freeTmps_[(unsigned)type];
  if (!stack.empty()) {
    auto *res = stack.back();
    stack.pop_back();
    return res;
  }

  tmpVars_.emplace_back(type, (unsigned)tmpVars_.size() + 1);
  return &tmpVars_.back();
}

void IRC1::freeTmpVar(const TmpVar *tmpVar) {
  freeTmps_[(unsigned)tmpVar->type].push_back(tmpVar);
}

const IRC1::TmpVar *IRC1::varFor(const Instruction *inst) {
  auto it = varMapping_.find(inst);
  if (it == varMapping_.end())
    return nullptr;
  return it->second;
}

void IRC1::regAlloc(Function *func) {
  for (auto &bb : func->basicBlocks())
    regAllocBB(&bb);
}

// Perform simple linear scan register allocation only within the block.
// Variables used in other blocks are never freed.
void IRC1::regAllocBB(BasicBlock *bb) {
  // Keep track of the variable allocated to existing instructions and the
  // number of users remaining. When the user count is 0, the variable can be
  // freed.
  std::unordered_map<const Value *, std::pair<const TmpVar *, size_t>> allocated{};

  for (auto &iRef : bb->instructions()) {
    auto *inst = &iRef;

    for (const Value &operand : inst->operands()) {
      auto it = allocated.find(&operand);
      if (it == allocated.end())
        continue;
      // Decrease the number of remaining users. If zero, free the variable.
      if (--it->second.second == 0) {
        freeTmpVar(it->second.first);
        allocated.erase(it);
      }
    }

    if (inst->getType()->getKind() == TypeKind::Void)
      continue;

    size_t numUsers = inst->countUsers();
    auto *tmpVar = getTmpVar(inst->getType()->getKind());
    varMapping_[inst] = tmpVar;
    if (!numUsers)
      freeTmpVar(tmpVar);
    else
      allocated.try_emplace(inst, tmpVar, numUsers);
  }
  assert(allocated.empty());
}

void IRC1::printPrologue() {
  fprintf(os_, "\n#include \"apple2tc/system-inc.h\"\n\n");

  for (auto const &mr : mod_->getDisas()->memRanges()) {
    unsigned len = mr.to - mr.from + 1;
    fprintf(os_, "static const uint8_t s_mem_%04x[0x%04x];\n", mr.from, len);
  }

  fprintf(os_, "\nvoid init_emulated(void) {\n");
  for (auto const &mr : mod_->getDisas()->memRanges()) {
    unsigned len = mr.to - mr.from + 1;
    fprintf(os_, "  memcpy(s_ram + 0x%04x, s_mem_%04x, 0x%04x);\n", mr.from, mr.from, len);
  }

  fprintf(os_, "}\n");

  fprintf(os_, R"(
static inline uint8_t ovf8(uint8_t res, uint8_t a, uint8_t b) {
  return (~(a ^ b) & (a ^ res)) >> 7;
}
static uint16_t adc_dec16(uint8_t a, uint8_t b, uint8_t cf) {
  uint8_t saveStatus = s_status;
  s_status = cf;
  uint16_t res = adc_decimal(a, b);
  res |= s_status << 8;
  s_status = saveStatus;
  return res;
}
static uint16_t sbc_dec16(uint8_t a, uint8_t b, uint8_t cf) {
  uint8_t saveStatus = s_status;
  s_status = cf;
  uint16_t res = sbc_decimal(a, b);
  res |= s_status << 8;
  s_status = saveStatus;
  return res;
}
)");
}

void IRC1::printEpilogue() {
  for (auto const &mr : mod_->getDisas()->memRanges()) {
    unsigned len = mr.to - mr.from + 1;
    fprintf(os_, "static const uint8_t s_mem_%04x[0x%04x] = {", mr.from, len);
    for (unsigned i = 0; i != len; ++i) {
      if (i % 16 == 0)
        fprintf(os_, "\n  ");
      else
        fputc(' ', os_);
      fprintf(os_, "0x%02X", mod_->getDisas()->peek(mr.from + i));
      if (i != len - 1)
        fputc(',', os_);
    }
    fputc('\n', os_);
    fprintf(os_, "};\n");
  }
}

static inline constexpr bool BLOCKMAP_BSEARCH = true;

void IRC1::printAddr2BlockMap(const std::vector<BasicBlock *> &sortedBlocks) {
  std::vector<std::pair<uint16_t, unsigned>> map{};
  std::optional<uint32_t> lastAddr;
  for (auto *bb : sortedBlocks) {
    if (!bb->getAddress() || !bb->isRealAddress())
      continue;
    uint32_t addr = *bb->getAddress();
    if (addr > 0xFFFF)
      continue;
    PANIC_ASSERT_MSG(!lastAddr || addr > *lastAddr, "Duplicated BB address");
    lastAddr = addr;
    map.emplace_back(addr, blockID(bb));
  }

  if (BLOCKMAP_BSEARCH) {
    fprintf(os_, "static const unsigned s_block_map[] = {");
    static constexpr unsigned ROW = 5;
    for (unsigned i = 0; i != map.size(); ++i) {
      if (i % ROW == 0)
        fprintf(os_, "\n    ");
      else if (i)
        fprintf(os_, ", ");
      fprintf(os_, "0x%04x, %4u", map[i].first, map[i].second);

      if (i == map.size() - 1)
        fprintf(os_, "\n");
      else if (i % ROW == ROW - 1)
        fprintf(os_, ",");
    }
    fprintf(os_, "};\n");
    fprintf(os_, "%s", R"(
static int cmp_map_addr(const void *a, const void *b) {
  return *((const int *)a) - *((const int *)b);
}

static unsigned addr_to_block_id(uint16_t from_pc, uint16_t addr) {
  unsigned uaddr = addr;
  const unsigned *p = (const unsigned *)bsearch(
      &uaddr,
      s_block_map,
      sizeof(s_block_map) / (sizeof(unsigned) * 2),
      sizeof(unsigned) * 2,
      cmp_map_addr);
  if (p)
    return p[1];
  fprintf(stderr, "Unknown address $%04X\n", addr);
  error_handler(from_pc);
  abort();
};
)");
  } else {
    fprintf(os_, "static unsigned addr_to_block_id(uint16_t from_pc, uint16_t addr) {\n");
    fprintf(os_, "  switch(addr) {\n");
    for (const auto &p : map)
      fprintf(os_, "  case 0x%04x: return %u;\n", p.first, p.second);
    fprintf(os_, "  default:\n");
    fprintf(os_, "    fprintf(stderr, \"Unknown address $%%04X\\n\", addr);\n");
    fprintf(os_, "    error_handler(from_pc);\n");
    fprintf(os_, "  }\n");
    fprintf(os_, "}\n\n");
  }
}

static const char *getCTypeName(TypeKind type) {
  switch (type) {
  case TypeKind::Void:
  case TypeKind::BasicBlock:
  case TypeKind::CPUReg8:
  case TypeKind::CPUReg16:
    PANIC_ABORT("Invalid type");
    break;
  case TypeKind::IndBasicBlock:
    return "unsigned";
  case TypeKind::U8:
    return "uint8_t";
  case TypeKind::U16:
    return "uint16_t";
  case TypeKind::U32:
    return "uint32_t";
  case TypeKind::_last:
    PANIC_ABORT("Invalid type");
    break;
  }
}

void IRC1::run() {
  printPrologue();
  fprintf(os_, "\n");
  unsigned numFunctions = 0;
  for (auto &func : mod_->functions()) {
    ++numFunctions;
    PANIC_ASSERT_MSG(numFunctions < 2, "irc1 doesn't support more than one function");

    auto sortedBlocks = sortBasicBlocksByAddress(func.basicBlocks());
    // "name" the blocks consistently.
    for (auto *bb : sortedBlocks)
      blockID(bb);

    // Make sure the entry block is 0.
    PANIC_ASSERT(blockID(sortedBlocks[0]) == 0);
    fprintf(os_, "static unsigned addr_to_block_id(uint16_t from_pc, uint16_t addr);\n\n");

    regAlloc(&func);

    fprintf(os_, "void run_emulated(unsigned run_cycles) {\n");
    fprintf(os_, "  static unsigned block_id = 0;\n");
    fprintf(os_, "  bool branchTarget = true;\n");

    for (const auto &tmpVar : tmpVars_) {
      fprintf(os_, "  %s %s;\n", getCTypeName(tmpVar.type), tmpVar.name.c_str());
    }
    fprintf(os_, "\n");

    fprintf(
        os_, "  for(unsigned start_cycles = s_cycles; s_cycles - start_cycles < run_cycles;) {\n");
    fprintf(os_, "    switch (block_id) {\n");

    for (auto *bb : sortedBlocks)
      printBB(bb);

    fprintf(os_, "    default:\n");
    fprintf(os_, "      fprintf(stderr, \"panic: unknown block_id: %%u\\n\", block_id);\n");
    fprintf(os_, "      abort();\n");
    fprintf(os_, "    }\n");
    fprintf(os_, "  }\n");
    fprintf(os_, "}\n\n");

    printAddr2BlockMap(sortedBlocks);
  }

  printEpilogue();
}

void IRC1::printBB(BasicBlock *bb) {
  fprintf(os_, "    case %u:", blockID(bb));
  if (bb->getAddress())
    fprintf(os_, "  // $%04X", *bb->getAddress());
  fprintf(os_, "\n");

  std::optional<uint32_t> lastAddr{};
  for (auto &inst : bb->instructions()) {
    fprintf(os_, "      ");
    if (inst.getAddress() && inst.getAddress() != lastAddr)
      fprintf(os_, "/*$%04X*/", *inst.getAddress());
    else
      fprintf(os_, "         ");
    lastAddr = inst.getAddress();
    printInst(&inst);
    fprintf(os_, "\n");
  }

  fprintf(os_, "      break;\n");
}

void IRC1::printInst(Instruction *inst) {
  if (auto *tmpVar = varFor(inst))
    fprintf(os_, "%s = ", tmpVar->name.c_str());

  switch (inst->getKind()) {
#define IR_INST(name, type) \
  case ValueKind::name:     \
    print##name(inst);      \
    break;
#include "ir/Values.def"
  default:
    panicAbort("unsupported instruction %s", getValueKindName(inst->getKind()));
  }

  fprintf(os_, ";");
}

std::string IRC1::formatOperand(Value *operand) {
  if (auto *u8 = dyn_cast<LiteralU8>(operand)) {
    return format("0x%02x", u8->getValue());
  } else if (auto *u16 = dyn_cast<LiteralU16>(operand)) {
    return format("0x%04x", u16->getValue());
  } else if (auto *u32 = dyn_cast<LiteralU32>(operand)) {
    return format("%u", u32->getValue());
  } else if (auto *bb = dyn_cast<BasicBlock>(operand)) {
    return format("%u", blockID(bb));
  } else if (auto *inst = dyn_cast<Instruction>(operand)) {
    auto *var = varFor(inst);
    PANIC_ASSERT_MSG(var != nullptr, "instruction hasn't been allocated a register");
    return var->name;
  } else {
    PANIC_ABORT("Unsupported operand kind %s", getValueKindName(operand->getKind()));
  }
}

void IRC1::printBranchTarget(Instruction *inst) {
  if (!inst->getAddress())
    return;
  if (*inst->getAddress() > 0xFFFF)
    return;
  uint16_t addr = *inst->getAddress();
  if (!mod_->getDisas()->cfindAsmBlockContaining(addr))
    return;
  CPUOpcode opc = decodeOpcode(mod_->getDisas()->peek(addr));
  if (!instIsBranch(opc.kind, opc.addrMode))
    return;
  fprintf(os_, "branchTarget = true; ");
}

void IRC1::printLoadR8(Instruction *inst) {
  switch (cast<CPUReg8>(inst->getOperand(0))->getValue()) {
  case CPURegKind::A:
    fprintf(os_, "s_a");
    break;
  case CPURegKind::X:
    fprintf(os_, "s_x");
    break;
  case CPURegKind::Y:
    fprintf(os_, "s_y");
    break;
  case CPURegKind::SP:
    fprintf(os_, "s_sp");
    break;
  case CPURegKind::STATUS_N:
    fprintf(os_, "s_status & STATUS_N");
    break;
  case CPURegKind::STATUS_V:
    fprintf(os_, "(s_status & STATUS_V) != 0");
    break;
  case CPURegKind::STATUS_B:
    fprintf(os_, "(s_status & STATUS_B) != 0");
    break;
  case CPURegKind::STATUS_D:
    fprintf(os_, "(s_status & STATUS_D) != 0");
    break;
  case CPURegKind::STATUS_I:
    fprintf(os_, "(s_status & STATUS_I) != 0");
    break;
  case CPURegKind::STATUS_NotZ:
    fprintf(os_, "(~s_status & STATUS_Z)");
    break;
  case CPURegKind::STATUS_C:
    fprintf(os_, "s_status & STATUS_C");
    break;
  case CPURegKind::_last:
    PANIC_ABORT("Invalid CPU register");
  }
}

void IRC1::printStoreR8(Instruction *inst) {
  Value *operand = inst->getOperand(1);
  switch (cast<CPUReg8>(inst->getOperand(0))->getValue()) {
  case CPURegKind::A:
    fprintf(os_, "s_a = %s", formatOperand(operand).c_str());
    break;
  case CPURegKind::X:
    fprintf(os_, "s_x = %s", formatOperand(operand).c_str());
    break;
  case CPURegKind::Y:
    fprintf(os_, "s_y = %s", formatOperand(operand).c_str());
    break;
  case CPURegKind::SP:
    fprintf(os_, "s_sp = %s", formatOperand(operand).c_str());
    break;
  case CPURegKind::STATUS_N:
    fprintf(os_, "s_status = (s_status & ~STATUS_N) | %s", formatOperand(operand).c_str());
    break;
  case CPURegKind::STATUS_V:
    fprintf(os_, "s_status = (s_status & ~STATUS_V) | (%s << 6)", formatOperand(operand).c_str());
    break;
  case CPURegKind::STATUS_B:
    fprintf(os_, "s_status = (s_status & ~STATUS_B) | (%s << 4)", formatOperand(operand).c_str());
    break;
  case CPURegKind::STATUS_D:
    fprintf(os_, "s_status = (s_status & ~STATUS_D) | (%s << 3)", formatOperand(operand).c_str());
    break;
  case CPURegKind::STATUS_I:
    fprintf(os_, "s_status = (s_status & ~STATUS_I) | (%s << 2)", formatOperand(operand).c_str());
    break;
  case CPURegKind::STATUS_NotZ:
    fprintf(
        os_,
        "s_status = (s_status & ~STATUS_Z) | (%s ? 0 : STATUS_Z)",
        formatOperand(operand).c_str());
    break;
  case CPURegKind::STATUS_C:
    fprintf(os_, "s_status = (s_status & ~STATUS_C) | %s", formatOperand(operand).c_str());
    break;
  case CPURegKind::_last:
    PANIC_ABORT("Invalid CPU register");
  }
}

void IRC1::printVoidNop(Instruction *inst) {
  fprintf(os_, "(void)0");
}
void IRC1::printNop8(Instruction *inst) {
  fprintf(os_, "0");
}

void IRC1::printPeek8(Instruction *inst) {
  const char *func = "peek";
  if (auto *u16 = dyn_cast<LiteralU16>(inst->getOperand(0)))
    if (u16->getValue() >= A2_IO_RANGE_START && u16->getValue() <= A2_IO_RANGE_END)
      func = "io_peek";
  fprintf(os_, "%s(%s)", func, formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printPoke8(Instruction *inst) {
  const char *func = "poke";
  if (auto *u16 = dyn_cast<LiteralU16>(inst->getOperand(0)))
    if (u16->getValue() >= A2_IO_RANGE_START && u16->getValue() <= A2_IO_RANGE_END)
      func = "io_poke";

  fprintf(
      os_,
      "%s(%s, %s)",
      func,
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printRamPeek8(Instruction *inst) {
  fprintf(os_, "ram_peek(%s)", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printRamPoke8(Instruction *inst) {
  fprintf(
      os_,
      "ram_poke(%s, %s)",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printPeek16al(Instruction *inst) {
  fprintf(os_, "peek16al(%s)", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printPeek16un(Instruction *inst) {
  fprintf(os_, "peek16(%s)", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printRamPeek16al(Instruction *inst) {
  fprintf(os_, "ram_peek16al(%s)", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printRamPeek16un(Instruction *inst) {
  fprintf(os_, "ram_peek16(%s)", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printSExt8t16(Instruction *inst) {
  fprintf(os_, "(int8_t)%s", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printZExt8t16(Instruction *inst) {
  fprintf(os_, "%s", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printTrunc16t8(Instruction *inst) {
  fprintf(os_, "(uint8_t)%s", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printHigh8(Instruction *inst) {
  fprintf(os_, "(uint8_t)(%s >> 8)", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printMake16(Instruction *inst) {
  fprintf(
      os_,
      "%s + (%s << 8)",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printShl8(Instruction *inst) {
  fprintf(
      os_,
      "%s << %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printShl16(Instruction *inst) {
  fprintf(
      os_,
      "%s << %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printShr8(Instruction *inst) {
  fprintf(
      os_,
      "%s >> %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printShr16(Instruction *inst) {
  fprintf(
      os_,
      "%s >> %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printAnd8(Instruction *inst) {
  fprintf(
      os_,
      "%s & %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printAnd16(Instruction *inst) {
  fprintf(
      os_,
      "%s & %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printOr8(Instruction *inst) {
  fprintf(
      os_,
      "%s | %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printOr16(Instruction *inst) {
  fprintf(
      os_,
      "%s | %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printXor8(Instruction *inst) {
  fprintf(
      os_,
      "%s ^ %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printXor16(Instruction *inst) {
  fprintf(
      os_,
      "%s ^ %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printAdd8(Instruction *inst) {
  fprintf(
      os_,
      "%s + %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printAdd16(Instruction *inst) {
  fprintf(
      os_,
      "%s + %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printSub8(Instruction *inst) {
  fprintf(
      os_,
      "%s - %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printSub16(Instruction *inst) {
  fprintf(
      os_,
      "%s - %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printOvf8(Instruction *inst) {
  fprintf(
      os_,
      "ovf8((uint8_t)%s, (uint8_t)%s, (uint8_t)%s)",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str(),
      formatOperand(inst->getOperand(2)).c_str());
}
void IRC1::printAdcDec16(Instruction *inst) {
  fprintf(
      os_,
      "adc_dec16(%s, %s, %s)",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str(),
      formatOperand(inst->getOperand(2)).c_str());
}
void IRC1::printSbcDec16(Instruction *inst) {
  fprintf(
      os_,
      "sbc_dec16(%s, %s, %s)",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str(),
      formatOperand(inst->getOperand(2)).c_str());
}
void IRC1::printCmp8eq(Instruction *inst) {
  fprintf(
      os_,
      "%s == %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printCmp8ne(Instruction *inst) {
  fprintf(
      os_,
      "%s != %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printCmp8ge(Instruction *inst) {
  fprintf(
      os_,
      "(int8_t)%s >= (int8_t)%s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printCmp8lt(Instruction *inst) {
  fprintf(
      os_,
      "(int8_t)%s < (int8_t)%s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printCmp8ae(Instruction *inst) {
  fprintf(
      os_,
      "%s >= %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printCmp8bt(Instruction *inst) {
  fprintf(
      os_,
      "%s < %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printNot16(Instruction *inst) {
  fprintf(os_, "~%s", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printAddCycles(Instruction *inst) {
  uint16_t pc;
  if (inst->getAddress()) {
    pc = *inst->getAddress();
    fprintf(os_, "s_pc = 0x%04x; ", pc);
  } else {
    pc = 0xFFFF;
  }
  fprintf(os_, "CYCLES(0x%04x, %s)", pc, formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printCycles(Instruction *inst) {
  fprintf(os_, "s_cycles");
}
void IRC1::printPush8(Instruction *inst) {
  fprintf(os_, "push8(%s)", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printPop8(Instruction *inst) {
  fprintf(os_, "pop8()");
}
void IRC1::printEncodeFlags(Instruction *inst) {
  fprintf(os_, "%s", formatOperand(inst->getOperand(0)).c_str());
  fprintf(os_, " | ((%s == 0) << 1)", formatOperand(inst->getOperand(1)).c_str());
  fprintf(os_, " | (%s << 2)", formatOperand(inst->getOperand(2)).c_str());
  fprintf(os_, " | (%s << 3)", formatOperand(inst->getOperand(3)).c_str());
  fprintf(os_, " | STATUS_B");
  fprintf(os_, " | (%s << 6)", formatOperand(inst->getOperand(4)).c_str());
  fprintf(os_, " | %s", formatOperand(inst->getOperand(5)).c_str());
}
void IRC1::printDecodeFlag(Instruction *inst) {
  assert(isa<LiteralU8>(inst->getOperand(1)) && "DecodeFlag operand2 must be a literal");
  uint8_t bitIndex = cast<LiteralU8>(inst->getOperand(1))->getValue();
  assert(bitIndex < 8);

  if (bitIndex == 0 || bitIndex == 7) {
    // C and N require just a mask.
    fprintf(os_, "%s & 0x%02x", formatOperand(inst->getOperand(0)).c_str(), 1 << bitIndex);
  } else if (bitIndex == 1) {
    // Z needs to be inverted into NotZ
    fprintf(os_, "(~%s & 2)", formatOperand(inst->getOperand(0)).c_str());
  } else {
    // The rest are a mask and a check.
    fprintf(os_, "(%s & 0x%02x) != 0", formatOperand(inst->getOperand(0)).c_str(), 1 << bitIndex);
  }
}
void IRC1::printCPUAddr2BB(Instruction *inst) {
  fprintf(
      os_,
      "addr_to_block_id(0x%04x, %s)",
      inst->getAddress().value_or(0xFFFF),
      formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printJmp(Instruction *inst) {
  printBranchTarget(inst);
  fprintf(os_, "block_id = %s", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printJmpInd(Instruction *inst) {
  printBranchTarget(inst);
  fprintf(os_, "block_id = %s", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printJTrue(Instruction *inst) {
  printBranchTarget(inst);
  fprintf(
      os_,
      "block_id = %s ? %s : %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str(),
      formatOperand(inst->getOperand(2)).c_str());
}
void IRC1::printJFalse(Instruction *inst) {
  printBranchTarget(inst);
  fprintf(
      os_,
      "block_id = !%s ? %s : %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str(),
      formatOperand(inst->getOperand(2)).c_str());
}
void IRC1::printJSR(Instruction *inst) {
  auto *target = cast<BasicBlock>(inst->getOperand(0));
  auto *fall = cast<BasicBlock>(inst->getOperand(1));

  PANIC_ASSERT_MSG(fall->getAddress(), "JSR return block must have an address");
  // Note that the 6502 pushes pc + 2 instead of pc + 3, so we decrement the return address.
  uint16_t retAddr = *fall->getAddress() - 1;
  printBranchTarget(inst);
  fprintf(os_, "push16(0x%04x); block_id = %s", retAddr, formatOperand(target).c_str());
}
void IRC1::printJSRInd(Instruction *inst) {
  auto *target = inst->getOperand(0);
  auto *fall = cast<BasicBlock>(inst->getOperand(1));

  PANIC_ASSERT_MSG(fall->getAddress(), "JSRInd return block must have an address");
  // Note that the 6502 pushes pc + 2 instead of pc + 3, so we decrement the return address.
  uint16_t retAddr = *fall->getAddress() - 1;
  printBranchTarget(inst);
  fprintf(os_, "push16(0x%04x); block_id = %s", retAddr, formatOperand(target).c_str());
}
void IRC1::printRTS(Instruction *inst) {
  printBranchTarget(inst);
  fprintf(
      os_,
      "block_id = addr_to_block_id(0x%04x, pop16() + 1);",
      inst->getAddress().value_or(0xFFFF));
}
void IRC1::printAbort(Instruction *inst) {
  fprintf(
      os_,
      "fprintf(stderr, \"abort: pc=$%%04X, target=$%%04X, reason=%%u\", %s, %s, %s); error_handler(%s)",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str(),
      formatOperand(inst->getOperand(2)).c_str(),
      formatOperand(inst->getOperand(0)).c_str());
}

void printIRC1(ir::Module *mod, FILE *os) {
  IRC1(mod, os).run();
}
