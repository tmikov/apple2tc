/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "IR.h"

#include "IRDump.h"

#include "apple2tc/support.h"

namespace ir {

const char *getTypeKindName(TypeKind kind) {
  static const char *const names[] = {
#define IR_TYPE(name) #name,
#include "Values.def"
  };
  assert(kind < TypeKind::_last);
  return names[(unsigned)kind];
}

const char *getValueKindName(ValueKind kind) {
  static const char *const names[] = {
#define IR_VALUE(name, ...) #name,
#include "Values.def"
  };
  assert(kind < ValueKind::_last);
  return names[(unsigned)kind];
}

const char *getCPURegKindName(CPURegKind kind) {
  static const char *const names[] = {
#define IR_CPUREG(name, ...) #name,
#include "Values.def"
  };
  assert(kind < CPURegKind::_last);
  return names[(unsigned)kind];
}

IRContext::IRContext() {
#define IR_TYPE(name) types_.emplace_back(TypeKind::name);
#define IR_CPUREG(name, type) cpuRegs_.emplace_back(new type(this, CPURegKind::name));
#include "Values.def"
}

IRContext::~IRContext() {
  moduleList_.destroyAll();
}

Module *IRContext::createModule(const std::shared_ptr<Disas> &disas) {
  auto *res = new Module(this, disas);
  moduleList_.push_back(res);
  return res;
}

LiteralU8 *IRContext::getLiteralU8(uint8_t v) {
  return &u8_.try_emplace(v, this, v).first->second;
}
LiteralU16 *IRContext::getLiteralU16(uint16_t v) {
  return &u16_.try_emplace(v, this, v).first->second;
}
LiteralU32 *IRContext::getLiteralU32(uint32_t v) {
  return &u32_.try_emplace(v, this, v).first->second;
}

Value::~Value() = default;

void Value::replaceAllUsesWith(Value *v) {
  while (!userList_.empty())
    userList_.back().setValue(v);
}

void Instruction::dump() {
  FILE *os = stdout;

  fprintf(os, "%s", getValueKindName(getKind()));
  if (unsigned numOperands = getNumOperands()) {
    fprintf(os, " ");
    for (unsigned i = 0; i < numOperands; ++i) {
      if (i)
        fprintf(os, ", ");
      fprintf(os, "%s", getValueKindName(getOperand(i)->getKind()));
    }
  }
  fprintf(os, "\n");
}

void Instruction::clearOperands() {
  getOperands().clear();
}

void Instruction::eraseFromBasicBlock() {
  basicBlock_->eraseInstruction(this);
}

bool Instruction::hasSideEffects() const {
  // Void type instructions have side effects by definition.
  if (getType()->getKind() == TypeKind::Void)
    return true;
  // Check for side effects;
  switch (getKind()) {
  case ValueKind::Peek8:
  case ValueKind::Peek16al:
  case ValueKind::Peek16un:
  case ValueKind::Pop8:
  case ValueKind::Nop8:
    return true;
  default:
    return false;
  }
}

bool Instruction::modifiesSP() const {
  switch (getKind()) {
  case ValueKind::StoreSP:
  case ValueKind::Push8:
  case ValueKind::Pop8:
  case ValueKind::JSR:
  case ValueKind::JSRInd:
  case ValueKind::RTS:
    return true;
  default:
    return false;
  }
}

bool Instruction::readsMemory() const {
  switch (getKind()) {
  case ValueKind::Peek8:
  case ValueKind::RamPeek8:
  case ValueKind::Peek16al:
  case ValueKind::Peek16un:
  case ValueKind::RamPeek16al:
  case ValueKind::RamPeek16un:
  case ValueKind::Pop8:
  case ValueKind::RTS:
    return true;
  default:
    return false;
  }
}

bool Instruction::writesMemory() const {
  switch (getKind()) {
  case ValueKind::Poke8:
  case ValueKind::RamPoke8:
  case ValueKind::Push8:
  case ValueKind::JSR:
  case ValueKind::JSRInd:
    return true;
  default:
    return false;
  }
}

std::pair<Value *, unsigned> Instruction::memoryAddress() {
  switch (getKind()) {
  case ValueKind::Peek8:
  case ValueKind::RamPeek8:
  case ValueKind::Poke8:
  case ValueKind::RamPoke8:
    return {getOperand(0), 1};

  case ValueKind::Peek16al:
  case ValueKind::Peek16un:
  case ValueKind::RamPeek16al:
  case ValueKind::RamPeek16un:
    return {getOperand(0), 2};

  // Ideally we would return an expression involving SP, but who owns it?
  // So, we treat implicit stack instruction specially.
  case ValueKind::Pop8:
  case ValueKind::Push8:
    return {this, 1};
  case ValueKind::RTS:
  case ValueKind::JSR:
  case ValueKind::JSRInd:
    return {this, 2};

  default:
    return {nullptr, 0};
  };
}

BasicBlock::~BasicBlock() {
  instList_.destroyAll();
}

void BasicBlock::dump() {
  IRDumper d(stdout);
  d.dump(this);
}

Instruction *BasicBlock::getTerminator() {
  return !instList_.empty() && instList_.back().isTerminator() ? &instList_.back() : nullptr;
}

void BasicBlock::eraseInstruction(Instruction *inst) {
  assert(inst->getBasicBlock() == this && "Erased instruction must belong to block");
  assert(!inst->hasUsers() && "Instruction with users cannot be erased");
  instList_.remove(inst);
  inst->setBasicBlock(nullptr);
  delete inst;
}

void BasicBlock::importInstruction(Instruction *inst) {
  inst->getBasicBlock()->instList_.remove(inst);
  instList_.push_back(inst);
  inst->setBasicBlock(this);
}

IteratorRange<BasicBlock::PredInstIterator> predecessorInsts(BasicBlock &bb) {
  auto users = bb.users();
  return makeIteratorRange(
      BasicBlock::PredInstIterator(users.begin(), users.end()),
      BasicBlock::PredInstIterator(users.end()));
}

IteratorRange<BasicBlock::PredIterator> predecessors(BasicBlock &bb) {
  auto users = bb.users();
  return makeIteratorRange(
      BasicBlock::PredIterator(users.begin(), users.end()), BasicBlock::PredIterator(users.end()));
}

IteratorRange<BasicBlock::SuccIterator> successors(BasicBlock &bb) {
  if (auto *term = bb.getTerminator()) {
    auto operands = term->operands();
    return makeIteratorRange(
        BasicBlock::SuccIterator(operands.begin(), operands.end()),
        BasicBlock::SuccIterator(operands.end()));
  } else {
    return makeIteratorRange(
        BasicBlock::SuccIterator(Instruction::OperandIterator()),
        BasicBlock::SuccIterator(Instruction::OperandIterator()));
  }
}

Function::~Function() {
  bbList_.destroyAll();
}

BasicBlock *Function::createBasicBlock() {
  auto *block = new BasicBlock(module_->getContext(), this);
  bbList_.push_back(block);
  return block;
}

void Function::importBasicBlock(BasicBlock *bb) {
  bb->getFunction()->bbList_.remove(bb);
  bbList_.push_back(bb);
  bb->function_ = this;
}

void Function::eraseBasicBlock(BasicBlock *bb) {
  assert(bb->getFunction() == this && "Erased BasicBlock must belong to function");
  assert(!bb->hasUsers() && "BasicBlock with users cannot be erased");
  bbList_.remove(bb);
  bb->function_ = nullptr;
  delete bb;
}

void Module::dump() {
  IRDumper d(stdout);
  d.dump(this);
}

Module::~Module() {
  functionList_.destroyAll();
}

IRBuilder::SaveStateRAII::SaveStateRAII(IRBuilder *bld)
    : bld_(bld),
      block_(bld->block_),
      insertionPoint_(bld->insertionPoint_),
      address_(bld->address_) {}

IRBuilder::SaveStateRAII::SaveStateRAII(SaveStateRAII &&other) noexcept
    : bld_(other.bld_),
      block_(other.block_),
      insertionPoint_(other.insertionPoint_),
      address_(other.address_) {
  other.bld_ = nullptr;
}

IRBuilder::SaveStateRAII::~SaveStateRAII() {
  if (!bld_)
    return;
  bld_->block_ = block_;
  bld_->insertionPoint_ = insertionPoint_;
  bld_->address_ = address_;
}

InstDestroyer::InstDestroyer() = default;

InstDestroyer::~InstDestroyer() {
  for (auto *inst : toDestroy_)
    inst->eraseFromBasicBlock();
}

void IRBuilder::insert(Instruction *inst) {
  assert(block_ && "BasicBlock must be set");
  assert(inst->getBasicBlock() == block_);
  block_->insertBefore(insertionPoint_, inst);
  if (address_)
    inst->setAddress(*address_);
}

Instruction *IRBuilder::createInst(ValueKind kind, ArrayRef<Value *> operands) {
#define IR_INST0(name, type)      \
  case ValueKind::name:           \
    assert(operands.size() == 0); \
    return create##name();
#define IR_INST1(name, type, op1type) \
  case ValueKind::name:               \
    assert(operands.size() == 1);     \
    return create##name(operands[0]);
#define IR_INST2(name, type, op1type, op2type) \
  case ValueKind::name:                        \
    assert(operands.size() == 2);              \
    return create##name(operands[0], operands[1]);
#define IR_INST3(name, type, op1type, op2type, op3type) \
  case ValueKind::name:                                 \
    assert(operands.size() == 3);                       \
    return create##name(operands[0], operands[1], operands[2]);
#define IR_INST_N(name, type, opcount, optype) \
  case ValueKind::name:                        \
    return create##name##_##opcount##op(operands);
  switch (kind) {
#include "Values.def"
  default:
    PANIC_ABORT("Invalid instruction kind");
  }
}

#define IR_INST0(name, type)                                                       \
  Inst0 *IRBuilder::create##name() {                                               \
    auto *res = new Inst0(ValueKind::name, ctx_->getType(TypeKind::type), block_); \
    insert(res);                                                                   \
    return res;                                                                    \
  }
#define IR_INST1(name, type, op1type)                                                   \
  Inst1 *IRBuilder::create##name(Value *op1) {                                          \
    assert(op1->getType()->getKind() == TypeKind::op1type);                             \
    auto *res = new Inst1(ValueKind::name, ctx_->getType(TypeKind::type), block_, op1); \
    insert(res);                                                                        \
    return res;                                                                         \
  }
#define IR_INST2(name, type, op1type, op2type)                                               \
  Inst2 *IRBuilder::create##name(Value *op1, Value *op2) {                                   \
    assert(op1->getType()->getKind() == TypeKind::op1type);                                  \
    assert(op2->getType()->getKind() == TypeKind::op2type);                                  \
    auto *res = new Inst2(ValueKind::name, ctx_->getType(TypeKind::type), block_, op1, op2); \
    insert(res);                                                                             \
    return res;                                                                              \
  }
#define IR_INST3(name, type, op1type, op2type, op3type)                                           \
  Inst3 *IRBuilder::create##name(Value *op1, Value *op2, Value *op3) {                            \
    assert(op1->getType()->getKind() == TypeKind::op1type);                                       \
    assert(op2->getType()->getKind() == TypeKind::op2type);                                       \
    assert(op3->getType()->getKind() == TypeKind::op3type);                                       \
    auto *res = new Inst3(ValueKind::name, ctx_->getType(TypeKind::type), block_, op1, op2, op3); \
    insert(res);                                                                                  \
    return res;                                                                                   \
  }
#define IR_INST_N(name, type, opcount, optype)                                                \
  InstN<opcount> *IRBuilder::create##name##_##opcount##op(ArrayRef<Value *> operands) {       \
    assert(operands.size() == opcount);                                                       \
    assert(std::all_of(operands.begin(), operands.end(), [](auto *p) {                        \
      return p->getType()->getKind() == TypeKind::optype;                                     \
    }));                                                                                      \
    auto *res =                                                                               \
        new InstN<opcount>(ValueKind::name, ctx_->getType(TypeKind::type), block_, operands); \
    insert(res);                                                                              \
    return res;                                                                               \
  }
#include "Values.def"

} // namespace ir
