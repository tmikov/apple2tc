/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "IRC1.h"

#include "apple2tc/SaveAndRestore.h"
#include "apple2tc/apple2iodefs.h"
#include "apple2tc/d6502.h"
#include "apple2tc/support.h"

#include <cstdarg>

static inline constexpr bool BLOCKMAP_BSEARCH = true;

void IRC1Mod::printPrologue() {
  fprintf(os_, "\n#include \"apple2tc/system2-inc.h\"\n\n");

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
  struct ResAndStatus res = adc_decimal(a, b, cf);
  return res.result | (res.status << 8);
}
static uint16_t sbc_dec16(uint8_t a, uint8_t b, uint8_t cf) {
  struct ResAndStatus res = sbc_decimal(a, b, cf);
  return res.result | (res.status << 8);
}
)");
  if (BLOCKMAP_BSEARCH) {
    fprintf(os_, "%s", R"(
static int cmp_map_addr(const void *a, const void *b) {
  return *((const int *)a) - *((const int *)b);
}

static unsigned
addr_to_block_id(uint16_t from_pc, uint16_t addr, const unsigned *block_map, size_t length) {
  unsigned uaddr = addr;
  const unsigned *p =
      (const unsigned *)bsearch(&uaddr, block_map, length, sizeof(unsigned) * 2, cmp_map_addr);
  if (p)
    return p[1];
  fprintf(stderr, "Unknown address $%04X\n", addr);
  error_handler(from_pc);
  abort();
};
)");
  }
}

void IRC1Mod::printEpilogue() {
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

void IRC1Mod::run() {
  unsigned numFunctions = 0;

  // Name all functions.
  for (auto &fRef : mod_->functions()) {
    namer_.getName(&fRef, [&numFunctions](Value *v) {
      auto *func = cast<Function>(v);
      if (!func->getName().empty())
        return format("FUNC_%s", func->getName().c_str());
      else if (func->getAddress().has_value())
        return format("func_%04x", *func->getAddress());
      else
        return format("func_t%03u", ++numFunctions);
    });
  }

  printPrologue();

  fprintf(os_, "\n");
  for (auto &func : mod_->functions())
    fprintf(os_, "void %s(uint16_t ret_addr);\n", getName(&func));

  fprintf(os_, "\n");
  fprintf(os_, "static void emulated_entry_point(void) {\n");
  fprintf(os_, "  %s(false);\n", getName(mod_->getStartFunction()));
  fprintf(os_, "}\n");

  for (auto &func : mod_->functions()) {
    fprintf(os_, "\n");
    IRC1(this, &func, os_, trees_).runFunc();
  }

  printEpilogue();
}

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

void IRC1::regAlloc() {
  for (auto &bb : func_->basicBlocks()) {
    if (trees_)
      regAllocBBTrees(&bb);
    else
      regAllocBB(&bb);
  }
}

class IRC1::BBAllocator {
  IRC1 &irc1_;
  // Keep track of the variable allocated to existing instructions and the
  // number of users remaining. When the user count is 0, the variable can be
  // freed.
  std::unordered_map<const Instruction *, std::pair<const TmpVar *, size_t>> allocated_{};
  bool trace_ = false;

public:
  explicit BBAllocator(IRC1 &irc1) : irc1_(irc1) {}

  bool isTrace() const {
    return trace_;
  }
  void setTrace(bool trace) {
    trace_ = trace;
  }

  /// Allocate a register for the instruction result.
  void allocInst(Instruction *inst) {
    if (inst->getType()->getKind() == TypeKind::Void)
      return;

    size_t numUsers = inst->countUsers();
    auto *tmpVar = irc1_.getTmpVar(inst->getType()->getKind());
    irc1_.varMapping_[inst] = tmpVar;
    if (!numUsers)
      irc1_.freeTmpVar(tmpVar);
    else
      allocated_.try_emplace(inst, tmpVar, numUsers);

    if (trace_) {
      fprintf(
          stderr,
          "$%04x %-10s: allocated %s with %zu users\n",
          inst->getAddress().value_or(0),
          getValueKindName(inst->getKind()),
          tmpVar->name.c_str(),
          numUsers);
    }
  }

  /// Free a single use of the specified instruction.
  void freeInstUse(const Instruction *inst, const Instruction *root) {
    auto it = allocated_.find(inst);
    if (it == allocated_.end())
      return;

    if (trace_) {
      fprintf(
          stderr,
          "$%04x %-10s: freeing use of node $%04x:%s %s with %zu users\n",
          root->getAddress().value_or(0),
          getValueKindName(root->getKind()),
          inst->getAddress().value_or(0),
          getValueKindName(inst->getKind()),
          it->second.first->name.c_str(),
          it->second.second);
    }

    // Decrease the number of remaining users. If zero, free the variable.
    if (--it->second.second == 0) {
      irc1_.freeTmpVar(it->second.first);
      allocated_.erase(it);
    }
  }

  bool empty() const {
    return allocated_.empty();
  }
};

// Perform simple linear scan register allocation only within the block.
// Variables used in other blocks are never freed.
void IRC1::regAllocBB(BasicBlock *bb) {
  BBAllocator allocator{*this};

  // Keep track of the variable allocated to existing instructions and the
  // number of users remaining. When the user count is 0, the variable can be
  // freed.
  std::unordered_map<const Value *, std::pair<const TmpVar *, size_t>> allocated{};

  for (auto &inst : bb->instructions()) {
    // Release one usage of every operand.
    for (const Value &operand : inst.operands()) {
      if (auto *opInst = dyn_cast<const Instruction>(&operand))
        allocator.freeInstUse(opInst, &inst);
    }

    allocator.allocInst(&inst);
  }
  assert(allocator.empty() && "For now we don't keep values alive across blocks");
}

void IRC1::regAllocBBTrees(BasicBlock *bb) {
  markExpressionTrees(bb, validTrees_);

  // Perform register allocation, now that trees have been identified.
  BBAllocator allocator{*this};
  allocator.setTrace(bb->getAddress().value_or(0) == 0x300 && bb->isRealAddress());
  // allocator.setTrace(true);

  std::vector<Instruction *> stack{};

  for (auto &iRef : bb->instructions()) {
    Instruction *inst = &iRef;
    // Skip instructions that are part of an expression tree.
    if (validTrees_.count(inst))
      continue;

    // Free all register uses in the operands or trees rooted by them.
    stack.push_back(inst);
    do {
      Instruction *toClear = stack.back();
      stack.pop_back();

      for (auto &op : toClear->operands()) {
        if (auto *opInst = dyn_cast<Instruction>(&op)) {
          // Is the operand instruction part of a tree?
          if (validTrees_.count(opInst)) {
            // Part of a tree doesn't have an allocated register, so recursively process its
            // children until a leaf with an allocated register is reached.
            stack.push_back(opInst);
          } else {
            // Not part of a tree, so free one use of its register.
            allocator.freeInstUse(opInst, inst);
          }
        }
      }
    } while (!stack.empty());

    allocator.allocInst(inst);
  }

  assert(allocator.empty() && "For now we don't keep values alive across blocks");
}

void IRC1::printAddr2BlockMap() {
  if (sortedDynamicBlocks_.empty())
    return;

  std::vector<std::pair<uint16_t, unsigned>> map{};
  std::optional<uint32_t> lastAddr;
  for (auto *bb : sortedDynamicBlocks_) {
    if (!bb->getAddress() || !bb->isRealAddress())
      continue;
    uint32_t addr = *bb->getAddress();
    if (addr > 0xFFFF)
      continue;
    PANIC_ASSERT_MSG(!lastAddr || addr > *lastAddr, "Duplicated BB address");
    lastAddr = addr;
    map.emplace_back(addr, blockID(bb));
  }

  if (map.size() > 8) {
    fprintf(os_, "static const unsigned s_block_map_%s[] = {", name_);
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
    fprintf(
        os_,
        R"(
static unsigned find_block_id_%s(uint16_t from_pc, uint16_t addr) {
  return addr_to_block_id(from_pc, addr, s_block_map_%s, sizeof(s_block_map_%s) / (sizeof(unsigned) * 2));
};
)",
        name_,
        name_,
        name_);
  } else {
    fprintf(os_, "static unsigned find_block_id_%s(uint16_t from_pc, uint16_t addr) {\n", name_);
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
  case TypeKind::Function:
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

IRC1::IRC1(IRC1Mod *c1mod, Function *func, FILE *os, bool trees)
    : c1mod_(c1mod),
      ctx_(func->getModule()->getContext()),
      mod_(func->getModule()),
      func_(func),
      os_(os),
      trees_(trees),
      name_(c1mod->getName(func)) {}

IRC1::~IRC1() = default;

void IRC1::runFunc() {
  scanForDynamicBlocks();

  std::vector<BasicBlock *> sortedBlocks{};
  for (auto &rBB : func_->basicBlocks())
    if (&rBB != func_->getExitBlock())
      sortedBlocks.push_back(&rBB);
  sortInplaceByAddress<BasicBlock>(sortedBlocks, true);

  // "name" the blocks consistently.
  for (auto *bb : sortedBlocks)
    blockID(bb);

  // Make sure the entry block is 0.
  PANIC_ASSERT(blockID(sortedBlocks[0]) == 0);
  if (!sortedDynamicBlocks_.empty())
    fprintf(os_, "static unsigned find_block_id_%s(uint16_t from_pc, uint16_t addr);\n\n", name_);

  regAlloc();

  fprintf(os_, "void %s(uint16_t ret_addr) {\n", name_);
  if (needDynamicBlocks())
    fprintf(os_, "  unsigned block_id = 0;\n");
  fprintf(os_, "  bool branchTarget = true;\n");

  for (const auto &tmpVar : tmpVars_) {
    fprintf(os_, "  %s %s;\n", getCTypeName(tmpVar.type), tmpVar.name.c_str());
  }
  fprintf(os_, "\n");

  fprintf(os_, "  if (ret_addr)\n");
  fprintf(os_, "    push16(ret_addr); // Fake return address.\n");
  fprintf(os_, "\n");
  if (needDynamicBlocks()) {
    fprintf(os_, "  for(;;) {\n");
    fprintf(os_, "    switch (block_id) {\n");
  }

  for (auto it = sortedBlocks.begin(), end = sortedBlocks.end(); it != end; ++it) {
    nextBB_ = it + 1 != end ? *(it + 1) : nullptr;
    printBB(*it);
  }

  if (needDynamicBlocks()) {
    fprintf(os_, "    default:\n");
    fprintf(os_, "      fprintf(stderr, \"panic: unknown block_id: %%u\\n\", block_id);\n");
    fprintf(os_, "      abort();\n");
    fprintf(os_, "    }\n");
    fprintf(os_, "  }\n");
  }
  fprintf(os_, "}\n\n");

  printAddr2BlockMap();
}

void IRC1::scanForDynamicBlocks() {
  bool haveCPUAddr2BB = false;
  std::unordered_set<BasicBlock *> dynamicBlocks{};

  for (auto &bb : func_->basicBlocks()) {
    for (auto &inst : bb.instructions()) {
      if (inst.getKind() == ValueKind::CPUAddr2BB || inst.getKind() == ValueKind::RTS)
        haveCPUAddr2BB = true;

      int opIndex = -1;
      switch (inst.getKind()) {
      case ValueKind::JSR:
      case ValueKind::JmpInd:
      case ValueKind::RTS:
        opIndex = 1;
        break;
      case ValueKind::JSRInd:
        opIndex = 2;
        break;
      default:
        break;
      }
      if (opIndex >= 0) {
        for (unsigned numOps = inst.getNumOperands(); opIndex < numOps; ++opIndex)
          dynamicBlocks.insert(cast<BasicBlock>(inst.getOperand(opIndex)));
      }
    }
  }

  if (haveCPUAddr2BB)
    sortedDynamicBlocks_ = sortByAddress<BasicBlock>(dynamicBlocks, false);
}

void IRC1::printBB(BasicBlock *bb) {
  if (needDynamicBlocks()) {
    fprintf(os_, "    case %u:", blockID(bb));
    if (bb->getAddress())
      fprintf(os_, "  // $%04X", *bb->getAddress());
  } else {
    fprintf(os_, "bb_%u:", blockID(bb));
  }
  fprintf(os_, "\n");

  std::optional<uint32_t> lastAddr{};
  for (auto &rInst : bb->instructions()) {
    auto *inst = &rInst;
    if (trees_ && validTrees_.count(inst))
      continue;

    // A little hack. Let the instruction print its own line, but only in
    // a couple of places.
    bool printOwnLine = false;
    if (!needDynamicBlocks()) {
      switch (inst->getKind()) {
      case ValueKind::Jmp:
      case ValueKind::JTrue:
      case ValueKind::JFalse:
        printOwnLine = true;
      default:;
      }
    }

    if (!printOwnLine) {
      fprintf(os_, needDynamicBlocks() ? "      " : "  ");
      if (inst->getAddress() && inst->getAddress() != lastAddr)
        fprintf(os_, "/*$%04X*/ ", *inst->getAddress());
      else
        fprintf(os_, "          ");
      lastAddr = inst->getAddress();
      if (auto *tmpVar = varFor(inst))
        fprintf(os_, "%s = ", tmpVar->name.c_str());
    }
    printInst(inst);
    if (!printOwnLine)
      fprintf(os_, ";\n");
  }

  if (needDynamicBlocks())
    fprintf(os_, "      break;\n");
}

std::string IRC1::formatInst(Instruction *inst) {
  SaveAndRestore saveObuf(obuf_, {});
  switch (inst->getKind()) {
#define IR_INST(name, type) \
  case ValueKind::name:     \
    print##name(inst);      \
    break;
#include "ir/Values.def"
  default:
    panicAbort("unsupported instruction %s", getValueKindName(inst->getKind()));
  }
  return std::move(obuf_);
}

void IRC1::printInst(Instruction *inst) {
  fprintf(os_, "%s", formatInst(inst).c_str());
}

static bool needParens(const char *s) {
  // A number doesn't need parens, but an expression starting with one,
  // does (according to our simplified rules here).
  if (isdigit(*s)) {
    // TODO: check for hex, etc.
    do
      ++s;
    while (isdigit(*s));
    return *s != 0;
  }

  // Expressions of the form "ident(...)" or "(...)" don't need parens.
  // Skip a starting identifier.
  if (isalpha(*s) || *s == '_') {
    do
      ++s;
    while (isalnum(*s) || *s == '_');
    // An expression consisting of an identifier doesn't need parens.
    if (*s == 0)
      return false;
  }
  if (*s != '(')
    return true;
  int pcount = 1;
  ++s;
  while (*s) {
    if (*s == '(')
      ++pcount;
    else if (*s == ')')
      --pcount;
    else if (*s == '"') // Can't skip strings, which may contain parens.
      return true;
    else if (pcount == 0) // Last paren closed before end of string.
      return true;
    ++s;
  }
  assert(pcount == 0 && "Unmatched parens in input");
  return false;
}

/// Wrap the input string with parens unless it obviously doesn't need them.
static std::string parens(std::string &&str) {
  return needParens(str.c_str()) ? '(' + str + ')' : std::move(str);
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
    if (trees_ && validTrees_.count(inst)) {
      return parens(formatInst(inst));
    } else {
      auto *var = varFor(inst);
      PANIC_ASSERT_MSG(var != nullptr, "instruction hasn't been allocated a register");
      return var->name;
    }
  } else {
    PANIC_ABORT("Unsupported operand kind %s", getValueKindName(operand->getKind()));
  }
}

static void bprintf(std::string &buf, const char *msg, ...)
    __attribute__((__format__(__printf__, 2, 3)));
static void bprintf(std::string &buf, const char *msg, ...) {
  va_list ap;
  va_start(ap, msg);
  buf += vformat(msg, ap);
  va_end(ap);
}

bool IRC1::needBranchTarget(Instruction *inst) {
  if (!inst->getAddress())
    return false;
  if (*inst->getAddress() > 0xFFFF)
    return false;
  uint16_t addr = *inst->getAddress();
  if (!mod_->getDisas()->cfindAsmBlockContaining(addr))
    return false;
  CPUOpcode opc = decodeOpcode(mod_->getDisas()->peek(addr));
  if (!instIsBranch(opc.kind, opc.addrMode))
    return false;
  return true;
}

bool IRC1::printBranchTarget(Instruction *inst) {
  if (!needBranchTarget(inst))
    return false;
  bprintf(obuf_, "branchTarget = true; ");
  return true;
}

void IRC1::printLoadR8(Instruction *inst) {
  switch (cast<CPUReg8>(inst->getOperand(0))->getValue()) {
  case CPURegKind::A:
    bprintf(obuf_, "s_a");
    break;
  case CPURegKind::X:
    bprintf(obuf_, "s_x");
    break;
  case CPURegKind::Y:
    bprintf(obuf_, "s_y");
    break;
  case CPURegKind::STATUS_N:
    bprintf(obuf_, "s_status & STATUS_N");
    break;
  case CPURegKind::STATUS_V:
    bprintf(obuf_, "(s_status & STATUS_V) != 0");
    break;
  case CPURegKind::STATUS_B:
    bprintf(obuf_, "(s_status & STATUS_B) != 0");
    break;
  case CPURegKind::STATUS_D:
    bprintf(obuf_, "(s_status & STATUS_D) != 0");
    break;
  case CPURegKind::STATUS_I:
    bprintf(obuf_, "(s_status & STATUS_I) != 0");
    break;
  case CPURegKind::STATUS_NotZ:
    bprintf(obuf_, "(~s_status & STATUS_Z)");
    break;
  case CPURegKind::STATUS_C:
    bprintf(obuf_, "s_status & STATUS_C");
    break;
  case CPURegKind::_last:
    PANIC_ABORT("Invalid CPU register");
  }
}

void IRC1::printStoreR8(Instruction *inst) {
  Value *operand = inst->getOperand(1);
  switch (cast<CPUReg8>(inst->getOperand(0))->getValue()) {
  case CPURegKind::A:
    bprintf(obuf_, "s_a = %s", formatOperand(operand).c_str());
    break;
  case CPURegKind::X:
    bprintf(obuf_, "s_x = %s", formatOperand(operand).c_str());
    break;
  case CPURegKind::Y:
    bprintf(obuf_, "s_y = %s", formatOperand(operand).c_str());
    break;
  case CPURegKind::STATUS_N:
    bprintf(obuf_, "s_status = (s_status & ~STATUS_N) | %s", formatOperand(operand).c_str());
    break;
  case CPURegKind::STATUS_V:
    bprintf(obuf_, "s_status = (s_status & ~STATUS_V) | (%s << 6)", formatOperand(operand).c_str());
    break;
  case CPURegKind::STATUS_B:
    bprintf(obuf_, "s_status = (s_status & ~STATUS_B) | (%s << 4)", formatOperand(operand).c_str());
    break;
  case CPURegKind::STATUS_D:
    bprintf(obuf_, "s_status = (s_status & ~STATUS_D) | (%s << 3)", formatOperand(operand).c_str());
    break;
  case CPURegKind::STATUS_I:
    bprintf(obuf_, "s_status = (s_status & ~STATUS_I) | (%s << 2)", formatOperand(operand).c_str());
    break;
  case CPURegKind::STATUS_NotZ:
    bprintf(
        obuf_,
        "s_status = (s_status & ~STATUS_Z) | (%s ? 0 : STATUS_Z)",
        formatOperand(operand).c_str());
    break;
  case CPURegKind::STATUS_C:
    bprintf(obuf_, "s_status = (s_status & ~STATUS_C) | %s", formatOperand(operand).c_str());
    break;
  case CPURegKind::_last:
    PANIC_ABORT("Invalid CPU register");
  }
}

void IRC1::printLoadSP(Instruction *inst) {
  bprintf(obuf_, "s_sp");
}
void IRC1::printStoreSP(Instruction *inst) {
  bprintf(obuf_, "s_sp = %s", formatOperand(inst->getOperand(0)).c_str());
}

void IRC1::printVoidNop(Instruction *inst) {
  bprintf(obuf_, "(void)0");
}
void IRC1::printNop8(Instruction *inst) {
  bprintf(obuf_, "0");
}

void IRC1::printPeek8(Instruction *inst) {
  const char *func = "peek";
  if (auto *u16 = dyn_cast<LiteralU16>(inst->getOperand(0)))
    if (u16->getValue() >= A2_IO_RANGE_START && u16->getValue() <= A2_IO_RANGE_END)
      func = "io_peek";
  bprintf(obuf_, "%s(%s)", func, formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printPoke8(Instruction *inst) {
  const char *func = "poke";
  if (auto *u16 = dyn_cast<LiteralU16>(inst->getOperand(0)))
    if (u16->getValue() >= A2_IO_RANGE_START && u16->getValue() <= A2_IO_RANGE_END)
      func = "io_poke";

  bprintf(
      obuf_,
      "%s(%s, %s)",
      func,
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printRamPeek8(Instruction *inst) {
  bprintf(obuf_, "ram_peek(%s)", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printRamPoke8(Instruction *inst) {
  bprintf(
      obuf_,
      "ram_poke(%s, %s)",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printPeek16al(Instruction *inst) {
  bprintf(obuf_, "peek16al(%s)", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printPeek16un(Instruction *inst) {
  bprintf(obuf_, "peek16(%s)", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printRamPeek16al(Instruction *inst) {
  bprintf(obuf_, "ram_peek16al(%s)", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printRamPeek16un(Instruction *inst) {
  bprintf(obuf_, "ram_peek16(%s)", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printSExt8t16(Instruction *inst) {
  bprintf(obuf_, "(int8_t)%s", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printZExt8t16(Instruction *inst) {
  bprintf(obuf_, "%s", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printTrunc16t8(Instruction *inst) {
  bprintf(obuf_, "(uint8_t)%s", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printHigh8(Instruction *inst) {
  bprintf(obuf_, "(uint8_t)(%s >> 8)", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printMake16(Instruction *inst) {
  if (auto *high = dyn_cast<LiteralU8>(inst->getOperand(1))) {
    // For readability.
    bprintf(
        obuf_, "%s + 0x%04x", formatOperand(inst->getOperand(0)).c_str(), high->getValue() << 8);
  } else {
    bprintf(
        obuf_,
        "%s + (%s << 8)",
        formatOperand(inst->getOperand(0)).c_str(),
        formatOperand(inst->getOperand(1)).c_str());
  }
}
void IRC1::printShl8(Instruction *inst) {
  bprintf(
      obuf_,
      "(uint8_t)(%s << %s)",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printShl16(Instruction *inst) {
  bprintf(
      obuf_,
      "%s << %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printShr8(Instruction *inst) {
  bprintf(
      obuf_,
      "%s >> %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printShr16(Instruction *inst) {
  bprintf(
      obuf_,
      "%s >> %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printAnd8(Instruction *inst) {
  bprintf(
      obuf_,
      "%s & %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printAnd16(Instruction *inst) {
  bprintf(
      obuf_,
      "%s & %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printOr8(Instruction *inst) {
  bprintf(
      obuf_,
      "%s | %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printOr16(Instruction *inst) {
  bprintf(
      obuf_,
      "%s | %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printXor8(Instruction *inst) {
  bprintf(
      obuf_,
      "%s ^ %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printXor16(Instruction *inst) {
  bprintf(
      obuf_,
      "%s ^ %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printAdd8(Instruction *inst) {
  bprintf(
      obuf_,
      "(uint8_t)(%s + %s)",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printAdd16(Instruction *inst) {
  bprintf(
      obuf_,
      "%s + %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printSub8(Instruction *inst) {
  bprintf(
      obuf_,
      "(uint8_t)(%s - %s)",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printSub16(Instruction *inst) {
  bprintf(
      obuf_,
      "%s - %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printOvf8(Instruction *inst) {
  bprintf(
      obuf_,
      "ovf8((uint8_t)%s, (uint8_t)%s, (uint8_t)%s)",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str(),
      formatOperand(inst->getOperand(2)).c_str());
}
void IRC1::printAdcDec16(Instruction *inst) {
  bprintf(
      obuf_,
      "adc_dec16(%s, %s, %s)",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str(),
      formatOperand(inst->getOperand(2)).c_str());
}
void IRC1::printSbcDec16(Instruction *inst) {
  bprintf(
      obuf_,
      "sbc_dec16(%s, %s, %s)",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str(),
      formatOperand(inst->getOperand(2)).c_str());
}
void IRC1::printCmp8eq(Instruction *inst) {
  bprintf(
      obuf_,
      "%s == %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printCmp8ne(Instruction *inst) {
  bprintf(
      obuf_,
      "%s != %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printCmp8ge(Instruction *inst) {
  bprintf(
      obuf_,
      "(int8_t)%s >= (int8_t)%s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printCmp8lt(Instruction *inst) {
  bprintf(
      obuf_,
      "(int8_t)%s < (int8_t)%s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printCmp8ae(Instruction *inst) {
  bprintf(
      obuf_,
      "%s >= %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printCmp8bt(Instruction *inst) {
  bprintf(
      obuf_,
      "%s < %s",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printNot16(Instruction *inst) {
  bprintf(obuf_, "~%s", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printAddCycles(Instruction *inst) {
  uint16_t pc;
  if (inst->getAddress()) {
    pc = *inst->getAddress();
    bprintf(obuf_, "s_pc = 0x%04x; ", pc);
  } else {
    pc = 0xFFFF;
  }
  bprintf(obuf_, "CYCLES(0x%04x, %s)", pc, formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printCycles(Instruction *inst) {
  bprintf(obuf_, "s_cycles");
}
void IRC1::printPush8(Instruction *inst) {
  bprintf(obuf_, "push8(%s)", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printPop8(Instruction *inst) {
  bprintf(obuf_, "pop8()");
}
void IRC1::printEncodeFlags(Instruction *inst) {
  bprintf(obuf_, "%s", formatOperand(inst->getOperand(0)).c_str());
  bprintf(obuf_, " | ((%s == 0) << 1)", formatOperand(inst->getOperand(1)).c_str());
  bprintf(obuf_, " | (%s << 2)", formatOperand(inst->getOperand(2)).c_str());
  bprintf(obuf_, " | (%s << 3)", formatOperand(inst->getOperand(3)).c_str());
  bprintf(obuf_, " | STATUS_B");
  bprintf(obuf_, " | (%s << 6)", formatOperand(inst->getOperand(4)).c_str());
  bprintf(obuf_, " | %s", formatOperand(inst->getOperand(5)).c_str());
}
void IRC1::printDecodeFlag(Instruction *inst) {
  assert(isa<LiteralU8>(inst->getOperand(1)) && "DecodeFlag operand2 must be a literal");
  uint8_t bitIndex = cast<LiteralU8>(inst->getOperand(1))->getValue();
  assert(bitIndex < 8);

  if (bitIndex == 0 || bitIndex == 7) {
    // C and N require just a mask.
    bprintf(obuf_, "%s & 0x%02x", formatOperand(inst->getOperand(0)).c_str(), 1 << bitIndex);
  } else if (bitIndex == 1) {
    // Z needs to be inverted into NotZ
    bprintf(obuf_, "(~%s & 2)", formatOperand(inst->getOperand(0)).c_str());
  } else {
    // The rest are a mask and a check.
    bprintf(obuf_, "(%s & 0x%02x) != 0", formatOperand(inst->getOperand(0)).c_str(), 1 << bitIndex);
  }
}
void IRC1::printCPUAddr2BB(Instruction *inst) {
  bprintf(
      obuf_,
      "find_block_id_%s(0x%04x, %s)",
      name_,
      inst->getAddress().value_or(0xFFFF),
      formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printCall(Instruction *inst) {
  bprintf(
      obuf_,
      "%s(%s)",
      c1mod_->getName(cast<Function>(inst->getOperand(0))),
      formatOperand(inst->getOperand(1)).c_str());
}
void IRC1::printReturn(Instruction *inst) {
  bprintf(obuf_, "if (ret_addr) pop16(); return");
}
void IRC1::printExit(Instruction *inst) {}
void IRC1::printJmp(Instruction *inst) {
  if (needDynamicBlocks()) {
    printBranchTarget(inst);
    bprintf(obuf_, "block_id = %s", formatOperand(inst->getOperand(0)).c_str());
  } else {
    if (needBranchTarget(inst)) {
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "branchTarget = true;\n");
    }
    if (inst->getOperand(0) != nextBB_) {
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "goto bb_%s;\n", formatOperand(inst->getOperand(0)).c_str());
    }
  }
}
void IRC1::printJmpInd(Instruction *inst) {
  printBranchTarget(inst);
  bprintf(obuf_, "block_id = %s", formatOperand(inst->getOperand(0)).c_str());
}
void IRC1::printJTrue(Instruction *inst) {
  if (needDynamicBlocks()) {
    printBranchTarget(inst);
    bprintf(
        obuf_,
        "block_id = %s ? %s : %s",
        formatOperand(inst->getOperand(0)).c_str(),
        formatOperand(inst->getOperand(1)).c_str(),
        formatOperand(inst->getOperand(2)).c_str());
  } else {
    if (needBranchTarget(inst)) {
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "branchTarget = true;\n");
    }
    if (nextBB_ == inst->getOperand(1)) {
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "if (!%s)\n", formatOperand(inst->getOperand(0)).c_str());
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "  goto bb_%s;\n", formatOperand(inst->getOperand(2)).c_str());
    } else if (nextBB_ == inst->getOperand(2)) {
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "if (%s)\n", formatOperand(inst->getOperand(0)).c_str());
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "  goto bb_%s;\n", formatOperand(inst->getOperand(1)).c_str());
    } else {
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "if (%s)\n", formatOperand(inst->getOperand(0)).c_str());
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "  goto bb_%s;\n", formatOperand(inst->getOperand(1)).c_str());
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "else\n");
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "  goto bb_%s;\n", formatOperand(inst->getOperand(2)).c_str());
    }
  }
}
void IRC1::printJFalse(Instruction *inst) {
  if (needDynamicBlocks()) {
    printBranchTarget(inst);
    bprintf(
        obuf_,
        "block_id = !%s ? %s : %s",
        formatOperand(inst->getOperand(0)).c_str(),
        formatOperand(inst->getOperand(1)).c_str(),
        formatOperand(inst->getOperand(2)).c_str());
  } else {
    if (needBranchTarget(inst)) {
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "branchTarget = true;\n");
    }
    if (nextBB_ == inst->getOperand(1)) {
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "if (%s)\n", formatOperand(inst->getOperand(0)).c_str());
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "  goto bb_%s;\n", formatOperand(inst->getOperand(2)).c_str());
    } else if (nextBB_ == inst->getOperand(2)) {
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "if (!%s)\n", formatOperand(inst->getOperand(0)).c_str());
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "  goto bb_%s;\n", formatOperand(inst->getOperand(1)).c_str());
    } else {
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "if (!%s)\n", formatOperand(inst->getOperand(0)).c_str());
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "  goto bb_%s;\n", formatOperand(inst->getOperand(1)).c_str());
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "else\n");
      bprintf(obuf_, "%12s", "");
      bprintf(obuf_, "  goto bb_%s;\n", formatOperand(inst->getOperand(2)).c_str());
    }
  }
}
void IRC1::printJSR(Instruction *inst) {
  auto *target = cast<BasicBlock>(inst->getOperand(0));
  auto *fall = cast<BasicBlock>(inst->getOperand(1));

  PANIC_ASSERT_MSG(fall->getAddress(), "JSR return block must have an address");
  // Note that the 6502 pushes pc + 2 instead of pc + 3, so we decrement the return address.
  uint16_t retAddr = *fall->getAddress() - 1;
  printBranchTarget(inst);
  bprintf(obuf_, "push16(0x%04x); block_id = %s", retAddr, formatOperand(target).c_str());
}
void IRC1::printJSRInd(Instruction *inst) {
  auto *target = inst->getOperand(0);
  auto *fall = cast<BasicBlock>(inst->getOperand(1));

  PANIC_ASSERT_MSG(fall->getAddress(), "JSRInd return block must have an address");
  // Note that the 6502 pushes pc + 2 instead of pc + 3, so we decrement the return address.
  uint16_t retAddr = *fall->getAddress() - 1;
  printBranchTarget(inst);
  bprintf(obuf_, "push16(0x%04x); block_id = %s", retAddr, formatOperand(target).c_str());
}
void IRC1::printRTS(Instruction *inst) {
  printBranchTarget(inst);
  bprintf(
      obuf_,
      "block_id = find_block_id_%s(0x%04x, pop16() + 1);",
      name_,
      inst->getAddress().value_or(0xFFFF));
}
void IRC1::printAbort(Instruction *inst) {
  bprintf(
      obuf_,
      "fprintf(stderr, \"abort: pc=$%%04X, target=$%%04X, reason=%%u\", %s, %s, %s); error_handler(%s)",
      formatOperand(inst->getOperand(0)).c_str(),
      formatOperand(inst->getOperand(1)).c_str(),
      formatOperand(inst->getOperand(2)).c_str(),
      formatOperand(inst->getOperand(0)).c_str());
}

void printIRC1(ir::Module *mod, FILE *os, bool trees) {
  IRC1Mod(mod, os, trees).run();
}
