/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "ValueList.h"

#include "apple2tc/ArrayRef.h"
#include "apple2tc/IteratorRange.h"

#include <optional>
#include <unordered_map>
#include <vector>

class Disas;

namespace ir {

class Value;
class Instruction;
class BasicBlock;
class Function;
class Module;
class InstDestroyer;

template <class T, class B>
bool isa(const B *b) {
  return T::classof(b);
}
template <class T, class B>
T *cast(B *b) {
  assert(T::classof(b) && "Invalid ir static cast");
  return static_cast<T *>(b);
}
template <class T, class B>
T *dyn_cast(B *b) {
  return T::classof(b) ? static_cast<T *>(b) : nullptr;
}

enum class TypeKind : uint8_t {
#define IR_TYPE(name) name,
#include "Values.def"
  _last
};

const char *getTypeKindName(TypeKind kind);

class Type {
public:
  explicit Type(TypeKind kind) : kind_(kind) {}

  TypeKind getKind() const {
    return kind_;
  }
  bool isVoid() const {
    return kind_ == TypeKind::Void;
  }

private:
  TypeKind const kind_;
};

enum class ValueKind : uint8_t {
#define IR_VALUE(name, ...) name,
#define IR_RANGE(name, first, last) _##name##_First = first, _##name##_Last = last,
#include "Values.def"
  _last
};

const char *getValueKindName(ValueKind kind);

enum class CPURegKind : uint8_t {
#define IR_CPUREG(name, ...) name,
#include "Values.def"
  _last
};

const char *getCPURegKindName(CPURegKind kind);

template <ValueKind FIRST, ValueKind LAST>
class ValueRange;
template <class Base, class NumType, ValueKind VK, TypeKind TK>
class ConstValueImpl;

using LiteralNumber = ValueRange<ValueKind::_LiteralNumber_First, ValueKind::_LiteralNumber_Last>;
using LiteralU8 = ConstValueImpl<LiteralNumber, uint8_t, ValueKind::LiteralU8, TypeKind::U8>;
using LiteralU16 = ConstValueImpl<LiteralNumber, uint16_t, ValueKind::LiteralU16, TypeKind::U16>;
using LiteralU32 = ConstValueImpl<LiteralNumber, uint32_t, ValueKind::LiteralU32, TypeKind::U32>;

using CPUReg = ValueRange<ValueKind::_CPUReg_First, ValueKind::_CPUReg_Last>;
using CPUReg8 = ConstValueImpl<CPUReg, CPURegKind, ValueKind::CPUReg8, TypeKind::CPUReg8>;
using CPUReg16 = ConstValueImpl<CPUReg, CPURegKind, ValueKind::CPUReg16, TypeKind::CPUReg16>;

class IRContext {
public:
  IRContext();
  ~IRContext();

  Module *createModule(const std::shared_ptr<Disas> &disas);

  Type *getType(TypeKind kind) {
    assert(kind < TypeKind::_last);
    return &types_[(unsigned)kind];
  }

  Type *getVoidType() {
    return getType(TypeKind::Void);
  }
  Type *getU8Type() {
    return getType(TypeKind::U8);
  }
  Type *getU16Type() {
    return getType(TypeKind::U16);
  }

  LiteralU8 *getLiteralU8(uint8_t v);
  LiteralU16 *getLiteralU16(uint16_t v);
  LiteralU32 *getLiteralU32(uint32_t v);

  CPUReg *getCPUReg(CPURegKind kind) {
    assert(kind < CPURegKind::_last);
    return cpuRegs_[(unsigned)kind].get();
  }

private:
  CircularList<Module> moduleList_{};
  std::vector<Type> types_{};
  std::unordered_map<uint8_t, LiteralU8> u8_{};
  std::unordered_map<uint16_t, LiteralU16> u16_{};
  std::unordered_map<uint32_t, LiteralU32> u32_{};
  std::vector<std::unique_ptr<CPUReg>> cpuRegs_{};
};

using OperandList = ValueListBase<Value, Instruction>;

class Value : public ListEntry {
protected:
  explicit Value(ValueKind kind, Type *type) : kind_(kind), type_(type) {}

public:
  virtual ~Value();

  Value(const Value &) = delete;
  void operator=(const Value &) = delete;

  bool inRange(ValueKind first, ValueKind last) const {
    return kind_ >= first && kind_ <= last;
  }
  static bool classof(const Value *) {
    return true;
  }

  ValueKind getKind() const {
    return kind_;
  }
  Type *getType() const {
    return type_;
  }

  bool hasUsers() const {
    return !userList_.empty();
  }
  /// Count and return the number of users. This method is O(number-of-users)!
  size_t countUsers() const {
    return userList_.countElements();
  }
  /// Count and return the number of users up to \p maxCount. This method is O(number-of-users)!
  size_t countUsersUpTo(size_t maxCount) const {
    return userList_.countElementsUpTo(maxCount);
  }

  /// If the instruction has exactly one user, return it, otherwise return nullptr.
  Instruction *getOnlyUser() {
    return !userList_.empty() && &userList_.front() == &userList_.back() ? userList_.front().owner()
                                                                         : nullptr;
  }

  auto users() {
    return makeIteratorRange(userList_.begin(), userList_.end());
  }

  void replaceAllUsesWith(Value *v);

#define IR_RANGE(name, first, last)                                        \
  bool is##name() const {                                                  \
    return inRange(ValueKind::_##name##_First, ValueKind::_##name##_Last); \
  }
#include "Values.def"

private:
  friend OperandList;

  const ValueKind kind_;
  Type *const type_;
  OperandList::UserListType userList_{};
};

template <ValueKind FIRST, ValueKind LAST>
class ValueRange : public Value {
protected:
  ValueRange(ValueKind kind, Type *type) : Value(kind, type) {}

public:
  static bool classof(const Value *v) {
    return v->inRange(FIRST, LAST);
  }
};

template <class Base, class StorageType, ValueKind VK, TypeKind TK>
class ConstValueImpl : public Base {
public:
  using ValueType = StorageType;

  ConstValueImpl(IRContext *ctx, ValueType num) : Base(VK, ctx->getType(TK)), storage_(num) {}

  static bool classof(const Value *v) {
    return v->getKind() == VK;
  }

  ValueType getValue() const {
    return storage_;
  }

private:
  ValueType storage_;
};

class Instruction : public Value {
  friend InstDestroyer;

protected:
  explicit Instruction(ValueKind kind, Type *type, BasicBlock *block)
      : Value(kind, type), basicBlock_(block) {
    assert(block != nullptr && "Every instruction must belong to a block");
  }

  void pushOperand(Value *v) {
    getOperands().push_back(v);
  }

  inline OperandList &getOperands();
  inline const OperandList &getOperands() const;

  /// Clear all operands. This should only be done when the instruction is about to be
  /// destroyed, because it puts it in an inconsistent state.
  void clearOperands();

public:
  static bool classof(const Value *v) {
    return v->isInstruction();
  }

  void eraseFromBasicBlock();

  void setAddress(uint32_t address) {
    address_ = address;
  }
  const std::optional<uint32_t> &getAddress() {
    return address_;
  }

  BasicBlock *getBasicBlock() const {
    return basicBlock_;
  }

  bool hasSideEffects() const;
  bool readsMemory() const;
  bool writesMemory() const;
  /// Return the memory address and width accessed by the specified instruction or nullptr.
  std::pair<Value *, unsigned> memoryAddress();

  unsigned getNumOperands() const {
    return getOperands().size();
  }
  Value *getOperand(unsigned op) const {
    return getOperands()[op].value();
  }
  void setOperand(unsigned op, Value *value) {
    getOperands()[op].setValue(value);
  }

  class OperandIterator {
    friend class Instruction;
    explicit OperandIterator(OperandList::iterator it) : it_(it) {}

  public:
    OperandIterator() : it_() {}

    bool operator==(const OperandIterator &other) const {
      return this->it_ == other.it_;
    }
    bool operator!=(const OperandIterator &other) const {
      return this->it_ != other.it_;
    }
    OperandIterator &operator++() {
      ++it_;
      return *this;
    }

    Value *operator->() const {
      return it_->value();
    }
    Value &operator*() const {
      return *it_->value();
    }

  private:
    OperandList::iterator it_;
  };

  /// \return a range for iterating over operand values.
  auto operands() {
    return makeIteratorRange(
        OperandIterator(getOperands().begin()), OperandIterator(getOperands().end()));
  }

private:
  void setBasicBlock(BasicBlock *block) {
    basicBlock_ = block;
  }

private:
  friend BasicBlock;
  BasicBlock *basicBlock_;
  std::optional<uint32_t> address_{};
};

template <unsigned CAP>
class InstructionBase : public Instruction {
protected:
  InstructionBase(ValueKind kind, Type *type, BasicBlock *block)
      : Instruction(kind, type, block), operands_(this) {}

private:
  friend class Instruction;
  ValueList<Value, Instruction, CAP> operands_;
};

inline OperandList &Instruction::getOperands() {
  return static_cast<InstructionBase<0> *>(this)->operands_;
}

inline const OperandList &Instruction::getOperands() const {
  return static_cast<const InstructionBase<0> *>(this)->operands_;
}

class Inst0 : public InstructionBase<0> {
  friend class IRBuilder;
  Inst0(ValueKind kind, Type *type, BasicBlock *block) : InstructionBase<0>(kind, type, block) {}
};
class Inst1 : public InstructionBase<1> {
  friend class IRBuilder;
  Inst1(ValueKind kind, Type *type, BasicBlock *block, Value *v1)
      : InstructionBase<1>(kind, type, block) {
    pushOperand(v1);
  }
};
class Inst2 : public InstructionBase<2> {
  friend class IRBuilder;
  Inst2(ValueKind kind, Type *type, BasicBlock *block, Value *v1, Value *v2)
      : InstructionBase<2>(kind, type, block) {
    pushOperand(v1);
    pushOperand(v2);
  }
};
class Inst3 : public InstructionBase<3> {
  friend class IRBuilder;
  Inst3(ValueKind kind, Type *type, BasicBlock *block, Value *v1, Value *v2, Value *v3)
      : InstructionBase<3>(kind, type, block) {
    pushOperand(v1);
    pushOperand(v2);
    pushOperand(v3);
  }
};
template <unsigned CAP>
class InstN : public InstructionBase<CAP> {
  friend class IRBuilder;
  InstN(ValueKind kind, Type *type, BasicBlock *block, ArrayRef<Value *> operands)
      : InstructionBase<CAP>(kind, type, block) {
    for (auto *op : operands)
      InstructionBase<CAP>::pushOperand(op);
  }
};

class BasicBlock : public Value {
  friend class Function;
  friend class IRBuilder;

  explicit BasicBlock(IRContext *ctx, Function *function)
      : Value(ValueKind::BasicBlock, ctx->getType(TypeKind::BasicBlock)), function_(function) {}

public:
  ~BasicBlock();

  static bool classof(const Value *v) {
    return v->getKind() == ValueKind::BasicBlock;
  }

  void dump();

  Function *getFunction() const {
    return function_;
  }

  /// \return a pointer to the terminator instruction, or nullptr. Completed blocks
  /// participating in the CFG should always have a terminator.
  Instruction *getTerminator();

  class PredIterator;
  class SuccIterator;

  auto instructions() {
    return makeIteratorRange(instList_.begin(), instList_.end());
  }
  auto reverse_instructions() {
    return makeIteratorRange(instList_.rbegin(), instList_.rend());
  }

  void eraseInstruction(Instruction *inst) {
    assert(inst->getBasicBlock() == this && "Erased instruction must belong to block");
    assert(!inst->hasUsers() && "Instruction with users cannot be erased");
    instList_.remove(inst);
    inst->setBasicBlock(nullptr);
    delete inst;
  }

  std::optional<uint32_t> const &getAddress() const {
    return address_;
  }
  /// Associate a CPU address with this block.
  /// \param address the address
  /// \param real whether the address is "real" or synthesized for sorting purposes.
  void setAddress(uint32_t address, bool real) {
    address_ = address;
    realAddress_ = real;
  }
  bool isRealAddress() const {
    return realAddress_;
  }

private:
  void insertBefore(const CircularList<Instruction>::iterator &before, Instruction *inst) {
    instList_.insertBefore(before, inst);
  }

private:
  Function *function_;
  CircularList<Instruction> instList_{};
  std::optional<uint32_t> address_{};
  /// Whether the address is "real" or it was synthesized just for informational purposes.
  bool realAddress_ = false;
};

/// Iterator over the terminator instructions that are predecessors of this basic block.
class BasicBlock::PredIterator {
public:
  PredIterator(
      OperandList::UserListType::iterator const &begin,
      OperandList::UserListType::iterator const &end)
      : begin_(begin), end_(end) {
    skipNonTerminators();
  }
  explicit PredIterator(OperandList::UserListType::iterator const &end) : begin_(end), end_(end) {}

  bool operator==(const PredIterator &other) const {
    return begin_ == other.begin_;
  }
  bool operator!=(const PredIterator &other) const {
    return begin_ != other.begin_;
  }

  PredIterator &operator++() {
    assert(begin_ != end_ && "++ past end iterator");
    ++begin_;
    skipNonTerminators();
    return *this;
  }

  BasicBlock *operator->() const {
    assert(begin_ != end_ && "deref of end iterator");
    return begin_->owner()->getBasicBlock();
  }
  BasicBlock &operator*() const {
    assert(begin_ != end_ && "deref of end iterator");
    return *begin_->owner()->getBasicBlock();
  }

private:
  /// Skip over non-terminator instruction values.
  void skipNonTerminators() {
    while (begin_ != end_ && !begin_->owner()->isTerminator())
      ++begin_;
  }

private:
  OperandList::UserListType::iterator begin_;
  OperandList::UserListType::iterator end_;
};

/// Iterator over basic block successors of the current basic block.
class BasicBlock::SuccIterator {
public:
  SuccIterator(Instruction::OperandIterator begin, Instruction::OperandIterator end)
      : begin_(begin), end_(end) {
    skipNonBasicBlocks();
  }
  explicit SuccIterator(Instruction::OperandIterator end) : begin_(end), end_(end) {}

  bool operator==(const SuccIterator &other) const {
    return begin_ == other.begin_;
  }
  bool operator!=(const SuccIterator &other) const {
    return begin_ != other.begin_;
  }

  SuccIterator &operator++() {
    assert(begin_ != end_ && "++ past end iterator");
    ++begin_;
    skipNonBasicBlocks();
    return *this;
  }

  BasicBlock *operator->() const {
    assert(begin_ != end_ && "deref of end iterator");
    return cast<BasicBlock>(&*begin_);
  }
  BasicBlock &operator*() const {
    assert(begin_ != end_ && "deref of end iterator");
    return *cast<BasicBlock>(&*begin_);
  }

private:
  // Skip non-basic block operands.
  void skipNonBasicBlocks() {
    while (begin_ != end_ && begin_->getKind() != ValueKind::BasicBlock)
      ++begin_;
  }

private:
  Instruction::OperandIterator begin_;
  Instruction::OperandIterator end_;
};

IteratorRange<BasicBlock::PredIterator> predecessors(BasicBlock &bb);
IteratorRange<BasicBlock::SuccIterator> successors(BasicBlock &bb);

class Function : public Value {
  friend class Module;
  explicit Function(IRContext *ctx, Module *module)
      : Value(ValueKind::Function, ctx->getVoidType()), module_(module) {}

public:
  ~Function();

  static bool classof(const Value *v) {
    return v->getKind() == ValueKind::Function;
  }

  Module *getModule() const {
    return module_;
  }

  BasicBlock *createBasicBlock();

  BasicBlock *getEntryBlock() {
    assert(!bbList_.empty() && "getEntryPoint() can only be invoked on non-empty block");
    return &bbList_.front();
  }

  auto basicBlocks() {
    return makeIteratorRange(bbList_.begin(), bbList_.end());
  }

private:
  Module *module_;
  CircularList<BasicBlock> bbList_{};
};

/// Sort a sequence of blocks using their addresses, when available. Block 0
/// is assumed to be the entry point and is not sorted.
template <class R>
std::vector<BasicBlock *> sortBasicBlocksByAddress(R range) {
  std::vector<BasicBlock *> sortedBlocks{};

  // Sort the blocks by starting address using a stable sort.
  for (auto &bb : range)
    sortedBlocks.push_back(&bb);

  // Note that we are not sorting the entry block.
  if (sortedBlocks.size() > 2) {
    std::stable_sort(
        sortedBlocks.begin() + 1, sortedBlocks.end(), [](BasicBlock *a, BasicBlock *b) -> bool {
          // No address is less than address.
          if (!a->getAddress())
            return b->getAddress().has_value();
          if (!b->getAddress())
            return false;
          return a->getAddress() < b->getAddress();
        });
  }
  return sortedBlocks;
}

class Module : public Value {
  friend class IRContext;
  explicit Module(IRContext *ctx, const std::shared_ptr<Disas> &disas)
      : Value(ValueKind::Module, ctx->getVoidType()), context_(ctx), disas_(disas) {}

public:
  ~Module();

  void dump();

  IRContext *getContext() const {
    return context_;
  }

  Disas *getDisas() const {
    return disas_.get();
  }

  Function *createFunction() {
    auto *func = new Function(context_, this);
    functionList_.push_back(func);
    return func;
  }

  auto functions() {
    return makeIteratorRange(functionList_.begin(), functionList_.end());
  }

private:
  IRContext *context_ = nullptr;
  std::shared_ptr<Disas> disas_;
  CircularList<Function> functionList_{};
};

class InstDestroyer {
  std::vector<Instruction *> toDestroy_{};

public:
  InstDestroyer(const InstDestroyer &) = delete;
  void operator=(const InstDestroyer &) = delete;

  InstDestroyer();
  ~InstDestroyer();

  /// Record the instruction for destruction. This operation cannot be undone.
  /// The instruction's operand list is cleared, so it is removed as a dependant
  /// on its operands.
  void add(Instruction *inst) {
    inst->clearOperands();
    toDestroy_.push_back(inst);
  }
};

class IRBuilder {
public:
  explicit IRBuilder(IRContext *ctx) : ctx_(ctx) {}

  IRBuilder(const IRBuilder &) = delete;
  void operator=(const IRBuilder &) = delete;

  void setAddress(uint32_t addr) {
    address_ = addr;
  }
  void setAddress(const std::optional<uint32_t> &addr) {
    address_ = addr;
  }
  void clearAddress() {
    address_.reset();
  }

  /// Set insertion point at the end of the specified block.
  void setInsertionBlock(BasicBlock *block) {
    block_ = block;
    insertionPoint_ = block->instList_.end();
  }
  /// Set insertion point before this instruction, in other words, this instruction
  /// will be pushed down as new ones are inserted.
  void setInsertionPointBefore(Instruction *inst) {
    block_ = inst->getBasicBlock();
    insertionPoint_ = CircularList<Instruction>::iterator(inst);
  }
  void setInsertionPointAfter(Instruction *inst) {
    block_ = inst->getBasicBlock();
    insertionPoint_ = CircularList<Instruction>::iterator(inst);
    ++insertionPoint_;
  }

  BasicBlock *getCurBasicBlock() {
    return block_;
  }

  LiteralU8 *getLiteralU8(uint8_t v) {
    return ctx_->getLiteralU8(v);
  }
  LiteralU16 *getLiteralU16(uint16_t v) {
    return ctx_->getLiteralU16(v);
  }
  LiteralU32 *getLiteralU32(uint32_t v) {
    return ctx_->getLiteralU32(v);
  }
  CPUReg *getCPUReg(CPURegKind kind) {
    return ctx_->getCPUReg(kind);
  }

  class SaveStateRAII;
  SaveStateRAII saveState();

#define IR_INST0(name, type) Inst0 *create##name();
#define IR_INST1(name, type, op1type) Inst1 *create##name(Value *op1);
#define IR_INST2(name, type, op1type, op2type) Inst2 *create##name(Value *op1, Value *op2);
#define IR_INST3(name, type, op1type, op2type, op3type) \
  Inst3 *create##name(Value *op1, Value *op2, Value *op3);
#define IR_INST_N(name, type, opcount, optype) \
  InstN<opcount> *create##name##_##opcount##op(ArrayRef<Value *> operands);
#include "Values.def"

private:
  void insert(Instruction *inst);

private:
  IRContext *const ctx_;
  BasicBlock *block_ = nullptr;
  CircularList<Instruction>::iterator insertionPoint_{};
  std::optional<uint32_t> address_{};
};

class IRBuilder::SaveStateRAII {
  friend class IRBuilder;

  IRBuilder *bld_;
  BasicBlock *block_;
  CircularList<Instruction>::iterator insertionPoint_;
  std::optional<uint32_t> address_;

public:
  SaveStateRAII(const SaveStateRAII &) = delete;
  void operator=(const SaveStateRAII &) = delete;
  void operator=(SaveStateRAII &&) = delete;

  explicit SaveStateRAII(IRBuilder *bld);
  SaveStateRAII(SaveStateRAII &&other) noexcept;
  ~SaveStateRAII();
};

inline IRBuilder::SaveStateRAII IRBuilder::saveState() {
  return SaveStateRAII(this);
}

} // namespace ir
