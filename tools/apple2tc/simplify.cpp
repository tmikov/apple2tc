/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "PubIR.h"
#include "ir/IR.h"
#include "ir/IRUtil.h"

using namespace ir;

namespace {

template <class T>
bool isLiteral(Value *v, typename T::ValueType c) {
  if (auto *lit = dyn_cast<T>(v))
    return lit->getValue() == c;
  return false;
}

template <class T>
T *getLiteral(IRBuilder &builder, typename T::ValueType c);

template <>
inline LiteralU8 *getLiteral<LiteralU8>(IRBuilder &builder, typename LiteralU8::ValueType c) {
  return builder.getLiteralU8(c);
}

template <>
inline LiteralU16 *getLiteral<LiteralU16>(IRBuilder &builder, typename LiteralU16::ValueType c) {
  return builder.getLiteralU16(c);
}

template <>
inline LiteralU32 *getLiteral<LiteralU32>(IRBuilder &builder, typename LiteralU32::ValueType c) {
  return builder.getLiteralU32(c);
}

template <class LiteralT>
ValueKind arithKindForType(ValueKind arith8);

template <>
constexpr ValueKind arithKindForType<LiteralU8>(ValueKind arith8) {
  return arith8;
}
template <>
constexpr ValueKind arithKindForType<LiteralU16>(ValueKind arith8) {
  return static_cast<ValueKind>(static_cast<unsigned>(arith8) + 1);
}

template <class LiteralT>
std::optional<std::pair<LiteralT *, Value *>> literalFirst(Instruction *inst) {
  LiteralT *lit;
  Value *op;
  if ((lit = dyn_cast<LiteralT>(inst->getOperand(0))) != nullptr)
    op = inst->getOperand(1);
  else if ((lit = dyn_cast<LiteralT>(inst->getOperand(1))) != nullptr)
    op = inst->getOperand(0);
  else
    return std::nullopt;
  return std::make_pair(lit, op);
}
template <class LiteralT>
std::optional<std::pair<LiteralT *, Value *>> literalFirst(Value *value, ValueKind arith8) {
  if (value->getKind() == arithKindForType<LiteralT>(arith8))
    return literalFirst<LiteralT>(cast<Instruction>(value));
  else
    return std::nullopt;
}

template <class LiteralT, ValueKind kind>
Instruction *create(IRBuilder &builder, Value *op1, Value *op2);

template <>
Instruction *create<LiteralU8, ValueKind::Add8>(IRBuilder &builder, Value *op1, Value *op2) {
  return builder.createAdd8(op1, op2);
}
template <>
Instruction *create<LiteralU16, ValueKind::Add8>(IRBuilder &builder, Value *op1, Value *op2) {
  return builder.createAdd16(op1, op2);
}
template <>
Instruction *create<LiteralU8, ValueKind::Sub8>(IRBuilder &builder, Value *op1, Value *op2) {
  return builder.createSub8(op1, op2);
}
template <>
Instruction *create<LiteralU16, ValueKind::Sub8>(IRBuilder &builder, Value *op1, Value *op2) {
  return builder.createSub16(op1, op2);
}

template <class LiteralT>
inline Value *simplifyAdd(IRBuilder &builder, Instruction *inst) {
  // (Add 0 x) => x
  if (isLiteral<LiteralT>(inst->getOperand(0), 0))
    return inst->getOperand(1);
  // (Add x 0) => x
  if (isLiteral<LiteralT>(inst->getOperand(1), 0))
    return inst->getOperand(0);
  // (Add const1 const2)
  if (auto *left = dyn_cast<LiteralT>(inst->getOperand(0)))
    if (auto *right = dyn_cast<LiteralT>(inst->getOperand(1)))
      return getLiteral<LiteralT>(builder, left->getValue() + right->getValue());

  // (Add lit1 (Add lit2 op2)) => (Add op2 (lit1 + lit2))
  if (auto optPair = literalFirst<LiteralT>(inst)) {
    // (Add lit1 op1)
    auto [lit1, op1] = *optPair;
    if (auto childPair = literalFirst<LiteralT>(op1, ValueKind::Add8)) {
      // (Add lit1 (Add lit2 op2))
      auto [lit2, op2] = *childPair;
      builder.setInsertionPointAfter(inst);
      return create<LiteralT, ValueKind::Add8>(
          builder, op2, getLiteral<LiteralT>(builder, lit1->getValue() + lit2->getValue()));
    }
  }

  return nullptr;
}

template <class LiteralT>
inline Value *simplifySub(IRBuilder &builder, Instruction *inst) {
  constexpr unsigned kWidth = sizeof(typename LiteralT::ValueType);

  // (Sub x 0) => x
  if (isLiteral<LiteralT>(inst->getOperand(1), 0))
    return inst->getOperand(0);

  auto *leftLit = dyn_cast<LiteralT>(inst->getOperand(0));
  auto *rightLit = dyn_cast<LiteralT>(inst->getOperand(1));

  // (Sub const1 const2)
  if (leftLit && rightLit)
    return getLiteral<LiteralT>(builder, leftLit->getValue() - rightLit->getValue());

  if (!leftLit && !rightLit)
    return nullptr;

  builder.setInsertionPointAfter(inst);

  if (leftLit) {
    // (Sub leftLit (Sub lit2 x)) => (Add (leftLit - lit2) x)
    // (Sub leftLit (Sub x lit2)) => (Sub (leftLit + lit2) x)
    // (Sub leftLit (Add lit2 x)) => (Sub (leftLit - lit2) x)

    if (inst->getOperand(1)->getKind() == arithKindForType<LiteralT>(ValueKind::Sub8)) {
      // (Sub leftLit (Sub _ _))
      auto *op = cast<Instruction>(inst->getOperand(1));
      LiteralT *lit2;
      if ((lit2 = dyn_cast<LiteralT>(op->getOperand(0))) != nullptr) {
        // (Sub leftLit (Sub lit2 x)) => (Add (leftLit - lit2) x)
        auto *x = op->getOperand(1);
        return create<LiteralT, ValueKind::Add8>(
            builder, getLiteral<LiteralT>(builder, leftLit->getValue() - lit2->getValue()), x);
      } else if ((lit2 = dyn_cast<LiteralT>(op->getOperand(1))) != nullptr) {
        // (Sub leftLit (Sub x lit2)) => (Sub (leftLit + lit2) x)
        auto *x = op->getOperand(0);
        return create<LiteralT, ValueKind::Sub8>(
            builder, getLiteral<LiteralT>(builder, leftLit->getValue() + lit2->getValue()), x);
      }
    } else if (auto childPair = literalFirst<LiteralT>(inst->getOperand(1), ValueKind::Add8)) {
      // (Sub leftLit (Add lit2 x)) => (Sub (leftLit - lit2) x)
      auto [lit2, x] = *childPair;
      return create<LiteralT, ValueKind::Sub8>(
          builder, getLiteral<LiteralT>(builder, leftLit->getValue() - lit2->getValue()), x);
    } else if (kWidth == 1 && leftLit->getValue() == 1 && inst->getOperand(1)->isComparison()) {
      // (Sub8 1 (cmp a b)) => (!cmp a b).
      auto *cmp = cast<Instruction>(inst->getOperand(1));
      return builder.createInst(
          negateComparison(cmp->getKind()), {cmp->getOperand(0), cmp->getOperand(1)});
    }
  } else {
    assert(rightLit);
    // (Sub (Sub lit2 x) rightLit) -> (Sub (lit2 - rightLit) x)
    // (Sub (Sub x lit2) rightLit) -> (Sub x (lit2 + rightLit))
    // (Sub (Add lit2 x) rightLit) -> (Add (lit2 - rightLit) x)

    if (inst->getOperand(0)->getKind() == arithKindForType<LiteralT>(ValueKind::Sub8)) {
      // (Sub (Sub _ _) rightLit)
      auto *op = cast<Instruction>(inst->getOperand(0));
      LiteralT *lit2;
      if ((lit2 = dyn_cast<LiteralT>(op->getOperand(0))) != nullptr) {
        // (Sub (Sub lit2 x) rightLit) -> (Sub (lit2 - rightLit) x)
        auto *x = op->getOperand(1);
        return create<LiteralT, ValueKind::Sub8>(
            builder, getLiteral<LiteralT>(builder, lit2->getValue() + rightLit->getValue()), x);
      } else if ((lit2 = dyn_cast<LiteralT>(op->getOperand(1))) != nullptr) {
        // (Sub (Sub x lit2) rightLit) -> (Sub x (lit2 + rightLit))
        auto *x = op->getOperand(0);
        return create<LiteralT, ValueKind::Sub8>(
            builder, x, getLiteral<LiteralT>(builder, lit2->getValue() + rightLit->getValue()));
      }
    } else if (auto childPair = literalFirst<LiteralT>(inst->getOperand(0), ValueKind::Add8)) {
      // (Sub (Add lit2 x) rightLit) -> (Sub x (rightLit - lit2))
      auto [lit2, x] = *childPair;
      return create<LiteralT, ValueKind::Sub8>(
          builder, x, getLiteral<LiteralT>(builder, rightLit->getValue() - lit2->getValue()));
    }
  }

  return nullptr;
}

} // namespace

/// Return a replacement value or nullptr.
static Value *simplifyInst(IRBuilder &builder, Instruction *inst) {
  switch (inst->getKind()) {
  case ValueKind::Peek8:
    break;
  case ValueKind::Poke8:
    break;
  case ValueKind::Peek16al:
    break;
  case ValueKind::Peek16un:
    break;
  case ValueKind::RamPeek16al:
    break;
  case ValueKind::RamPeek16un:
    break;

  case ValueKind::SExt8t16:
    if (auto *u8 = dyn_cast<LiteralU8>(inst->getOperand(0)))
      return builder.getLiteralU16((int8_t)u8->getValue());
    break;
  case ValueKind::ZExt8t16:
    if (auto *u8 = dyn_cast<LiteralU8>(inst->getOperand(0)))
      return builder.getLiteralU16((uint8_t)u8->getValue());
    // (ZExt8t16 (Trunc16t8 x)) => (And16 x 0x00FF)
    if (inst->getOperand(0)->getKind() == ValueKind::Trunc16t8) {
      builder.setInsertionPointAfter(inst);
      return builder.createAnd16(
          cast<Instruction>(inst->getOperand(0))->getOperand(0), builder.getLiteralU16(0x00FF));
    }
    break;
  case ValueKind::Trunc16t8:
    // (Trunc168t Literal16)
    if (auto *u16 = dyn_cast<LiteralU16>(inst->getOperand(0)))
      return builder.getLiteralU8((uint8_t)u16->getValue());
    switch (inst->getOperand(0)->getKind()) {
    // (Trunc16t8 (Make16 lo hi)) => lo
    // (Trunc16t8 (Ext8t16 v8)) => v8
    case ValueKind::Make16:
    case ValueKind::SExt8t16:
    case ValueKind::ZExt8t16:
      return cast<Instruction>(inst->getOperand(0))->getOperand(0);
    default:
      break;
    }
    break;
  case ValueKind::High8:
    // (High8 Literal16)
    if (auto *u16 = dyn_cast<LiteralU16>(inst->getOperand(0)))
      return builder.getLiteralU8(u16->getValue() >> 8);
    switch (inst->getOperand(0)->getKind()) {
    // (High8 (Make16 lo hi)) => hi
    case ValueKind::Make16:
      return cast<Instruction>(inst->getOperand(0))->getOperand(1);
    // (High8 (ZExt8t16 v8)) => 0
    case ValueKind::ZExt8t16:
      return builder.getLiteralU8(0);
    default:
      break;
    }
    break;
  case ValueKind::Make16:
    break;
  case ValueKind::Shl8:
    break;
  case ValueKind::Shl16:
    break;
  case ValueKind::Shr8:
    break;
  case ValueKind::Shr16:
    break;
  case ValueKind::And8:
    // (And8 const1 const2)
    if (auto *left = dyn_cast<LiteralU8>(inst->getOperand(0)))
      if (auto *right = dyn_cast<LiteralU8>(inst->getOperand(1)))
        return builder.getLiteralU8(left->getValue() & right->getValue());
    break;
  case ValueKind::And16:
    // (And16 const1 const2)
    if (auto *left = dyn_cast<LiteralU16>(inst->getOperand(0)))
      if (auto *right = dyn_cast<LiteralU16>(inst->getOperand(1)))
        return builder.getLiteralU16(left->getValue() & right->getValue());
    break;
  case ValueKind::Or8:
    break;
  case ValueKind::Or16:
    break;
  case ValueKind::Xor8:
    break;
  case ValueKind::Xor16:
    break;
  case ValueKind::Add8:
    return simplifyAdd<LiteralU8>(builder, inst);
  case ValueKind::Add16:
    return simplifyAdd<LiteralU16>(builder, inst);
  case ValueKind::Sub8:
    return simplifySub<LiteralU8>(builder, inst);
  case ValueKind::Sub16:
    return simplifySub<LiteralU16>(builder, inst);
  case ValueKind::Ovf8:
    break;
  case ValueKind::Cmp8eq:
    break;
  case ValueKind::Cmp8ne:
    break;
  case ValueKind::Cmp8lt:
    break;
  case ValueKind::Cmp8ge:
    break;
  case ValueKind::Cmp8ae:
    break;
  case ValueKind::Not16:
    // (Not16 literal)
    if (auto u16 = dyn_cast<LiteralU16>(inst->getOperand(0)))
      return builder.getLiteralU16(~u16->getValue());
    // (Not16 (Not16 x)) => x
    if (inst->getOperand(0)->getKind() == ValueKind::Not16)
      return cast<Instruction>(inst->getOperand(0))->getOperand(0);
    break;
  case ValueKind::EncodeFlags:
    break;
  case ValueKind::DecodeFlag:
    break;
  case ValueKind::JTrue:
    // jtrue const, label1, label2 => jmp
    if (auto *u8 = dyn_cast<LiteralU8>(inst->getOperand(0))) {
      builder.setInsertionPointAfter(inst);
      return builder.createJmp(u8->getValue() ? inst->getOperand(1) : inst->getOperand(2));
    }
    break;
  case ValueKind::JFalse:
    // jfalse const, label1, label2 => jmp
    if (auto *u8 = dyn_cast<LiteralU8>(inst->getOperand(0))) {
      builder.setInsertionPointAfter(inst);
      return builder.createJmp(!u8->getValue() ? inst->getOperand(1) : inst->getOperand(2));
    }
    break;

  default:
    break;
  }
  return nullptr;
}

bool simplify(Module *mod) {
  IRBuilder builder(mod->getContext());

  bool changed = false;
  for (auto &func : mod->functions()) {
    bool funcChanged;
    do {
      funcChanged = false;
      for (auto &bb : func.basicBlocks()) {
        InstDestroyer destroyer;
        for (auto &iRef : bb.instructions()) {
          auto *inst = &iRef;
          builder.setAddress(inst->getAddress());
          if (auto *replacement = simplifyInst(builder, inst)) {
            inst->replaceAllUsesWith(replacement);
            destroyer.add(inst);
            funcChanged = true;
          }
        }
      }
      changed |= funcChanged;
    } while (funcChanged);
  }
  return changed;
}

static bool simplifyCFG(Function *func) {
  bool changed = false;
  for (auto &rbb : func->basicBlocks()) {
    BasicBlock *bb = &rbb;
    Instruction *terminator = bb->getTerminator();
    // Look for blocks ending with an unconditional Jmp, where the successor has a single
    // predecessor.
    if (!(terminator->getKind() == ValueKind::Jmp && terminator->getOperand(0)->getOnlyUser()))
      continue;

    auto *nextBlock = cast<BasicBlock>(terminator->getOperand(0));
    auto insts = nextBlock->instructions();
    for (auto it = insts.begin(); it != insts.end();) {
      auto cur = it++;
      bb->importInstruction(&*cur);
    }

    bb->eraseInstruction(terminator);
    func->eraseBasicBlock(nextBlock);
    changed = true;
  }
  return changed;
}

bool simplifyCFG(Module *mod) {
  bool changed = false;
  for (auto &func : mod->functions())
    changed |= simplifyCFG(&func);
  return changed;
}
