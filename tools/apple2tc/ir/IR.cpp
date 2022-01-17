/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "IR.h"

#include "IRDump.h"

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

Module *IRContext::createModule() {
  auto *res = new Module(this);
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

void Instruction::clearOperands() {
  getOperands().clear();
}

void Instruction::eraseFromBasicBlock() {
  basicBlock_->eraseInstruction(this);
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
#include "Values.def"

} // namespace ir
