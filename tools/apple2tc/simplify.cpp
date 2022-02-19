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
template <>
Instruction *create<LiteralU8, ValueKind::And8>(IRBuilder &builder, Value *op1, Value *op2) {
  return builder.createAnd8(op1, op2);
}
template <>
Instruction *create<LiteralU16, ValueKind::And8>(IRBuilder &builder, Value *op1, Value *op2) {
  return builder.createAnd16(op1, op2);
}
template <>
Instruction *create<LiteralU8, ValueKind::Shl8>(IRBuilder &builder, Value *op1, Value *op2) {
  return builder.createShl8(op1, op2);
}
template <>
Instruction *create<LiteralU16, ValueKind::Shl8>(IRBuilder &builder, Value *op1, Value *op2) {
  return builder.createShl16(op1, op2);
}
template <>
Instruction *create<LiteralU8, ValueKind::Shr8>(IRBuilder &builder, Value *op1, Value *op2) {
  return builder.createShr8(op1, op2);
}
template <>
Instruction *create<LiteralU16, ValueKind::Shr8>(IRBuilder &builder, Value *op1, Value *op2) {
  return builder.createShr16(op1, op2);
}
template <>
Instruction *create<LiteralU16, ValueKind::Trunc16t8>(IRBuilder &builder, Value *op1, Value *op2) {
  assert(isLiteral<LiteralU16>(op2, 0xFF));
  return builder.createTrunc16t8(op1);
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

template <class LiteralT>
inline Value *simplifyAnd(IRBuilder &builder, Instruction *inst) {
  constexpr typename LiteralT::ValueType kAllOnes = ~static_cast<typename LiteralT::ValueType>(0);

  auto litFirst = literalFirst<LiteralT>(inst);
  if (!litFirst)
    return nullptr;

  LiteralT *lit1 = litFirst->first;
  Value *val = litFirst->second;

  // (And const1 const2)
  if (auto *lit2 = dyn_cast<LiteralT>(val))
    return getLiteral<LiteralT>(builder, lit1->getValue() & lit2->getValue());
  // (And 0 x) => 0
  if (lit1->getValue() == 0)
    return lit1;
  // (And ~0 x) => x
  if (lit1->getValue() == kAllOnes)
    return val;

  //(And const1 (And const2 x)) => (And (And const1 const2) x)
  if (auto childPair = literalFirst<LiteralT>(val, ValueKind::And8)) {
    builder.setInsertionPointAfter(inst);
    return create<LiteralT, ValueKind::And8>(
        builder,
        childPair->second,
        getLiteral<LiteralT>(builder, lit1->getValue() & childPair->first->getValue()));
  }

  return nullptr;
}

/// Nested And16 can be collapsed into a top And16
/// (And16 const1 (Op (And16 const2 x) y)) => (And16 const1 (Op x y))
///    if (const1 & const2 == Const1)
///    if Op is one of (Shl, Add, Sub, Mul, Or)
///
/// This is a separate function because the "top" And16 can be a Trunc16to8.
template <class LiteralT, ValueKind INST_KIND>
Value *simplifyNestedAnd(IRBuilder &builder, Instruction *inst, Value *val, uint16_t const1) {
  switch (val->getKind()) {
  case ValueKind::Shl16:
  case ValueKind::Add16:
  case ValueKind::Sub16:
  case ValueKind::Or16:
    break;
  default:
    return nullptr;
  }
  auto opInst = cast<Instruction>(val);

  // Check the left operand.
  // (Op (And16 const2 x) y)
  if (auto andPair = literalFirst<LiteralT>(opInst->getOperand(0), ValueKind::And8)) {
    LiteralT *lit2 = andPair->first;
    Value *x = andPair->second;
    Value *y = opInst->getOperand(1);

    if ((const1 & lit2->getValue()) == const1) {
      builder.setInsertionPointAfter(inst);
      return create<LiteralT, INST_KIND>(
          builder,
          builder.createInst(opInst->getKind(), {x, y}),
          getLiteral<LiteralT>(builder, const1));
    }
  }

  // We can't optimize the second operand of Shl
  if (val->getKind() == ValueKind::Shl16)
    return nullptr;

  // Check the right operand.
  // (Op x (And16 const2 y))
  if (auto andPair = literalFirst<LiteralT>(opInst->getOperand(1), ValueKind::And8)) {
    Value *x = opInst->getOperand(0);
    LiteralT *lit2 = andPair->first;
    Value *y = andPair->second;

    if ((const1 & lit2->getValue()) == const1) {
      builder.setInsertionPointAfter(inst);
      return create<LiteralT, INST_KIND>(
          builder,
          builder.createInst(opInst->getKind(), {x, y}),
          getLiteral<LiteralT>(builder, const1));
    }
  }

  return nullptr;
}

Value *simplifyAnd16(IRBuilder &builder, Instruction *inst) {
  using LiteralT = LiteralU16;
  if (auto *res = simplifyAnd<LiteralU16>(builder, inst))
    return res;

  auto litFirst = literalFirst<LiteralT>(inst);
  if (!litFirst)
    return nullptr;

  LiteralT *lit1 = litFirst->first;
  Value *val = litFirst->second;

  return simplifyNestedAnd<LiteralT, ValueKind::And8>(builder, inst, val, lit1->getValue());
}

Value *simplifyTrunc16to8(IRBuilder &builder, Instruction *inst) {
  // (Trunc16t8 Literal16)
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

  // (Trunc16t8 (And16 0x__FF x)) => (Trunc16t8 x)
  if (auto andPair = literalFirst<LiteralU16>(inst->getOperand(0), ValueKind::And8)) {
    if ((andPair->first->getValue() & 0xFF) == 0xFF)
      return builder.createTrunc16t8(andPair->second);
  }

  return simplifyNestedAnd<LiteralU16, ValueKind::Trunc16t8>(
      builder, inst, inst->getOperand(0), 0x00FF);
}

template <class LiteralT>
Value *simplifyShl(IRBuilder &builder, Instruction *inst) {
  constexpr typename LiteralT::ValueType kAllOnes = ~static_cast<typename LiteralT::ValueType>(0);
  constexpr unsigned kBitWidth = sizeof(typename LiteralT::ValueType) * 8;

  auto *lit1 = dyn_cast<LiteralT>(inst->getOperand(0));
  auto *lit2 = dyn_cast<LiteralU8>(inst->getOperand(1));

  // (Shl const1 const2)
  if (lit1 && lit2) {
    return lit2->getValue() < kBitWidth
        ? getLiteral<LiteralT>(builder, lit1->getValue() << lit2->getValue())
        : getLiteral<LiteralT>(builder, 0);
  }
  // (Shl 0 x) => 0
  if (lit1 && lit1->getValue() == 0)
    return lit1;
  // (Shl x 0) => x
  if (lit2 && lit2->getValue() == 0)
    return inst->getOperand(0);
  // (Shl x kBitWidth) => 0
  if (lit2 && lit2->getValue() >= kBitWidth)
    return getLiteral<LiteralT>(builder, 0);

  //(Shl (Shl x const1) const2) => (Shl x (Add const1 const2))
  if (lit2 && inst->getOperand(0)->getKind() == arithKindForType<LiteralT>(ValueKind::Shl8)) {
    auto *childInst = cast<Instruction>(inst->getOperand(0));
    if (auto *childLit = dyn_cast<LiteralU8>(childInst->getOperand(1))) {
      builder.setInsertionPointAfter(inst);
      unsigned count = childLit->getValue() + lit2->getValue();
      if (count >= kBitWidth)
        getLiteral<LiteralT>(builder, 0);
      return create<LiteralT, ValueKind::Shl8>(
          builder, childInst->getOperand(0), getLiteral<LiteralU8>(builder, count));
    }
  }

  // (Shl (And x const1) const2) => (And (Shl x const2) (const1 << const2))
  if (lit2) {
    if (auto childPair = literalFirst<LiteralT>(inst->getOperand(0), ValueKind::And8)) {
      LiteralT *nestedLit = childPair->first;
      Value *x = childPair->second;
      unsigned count = lit2->getValue();
      builder.setInsertionPointAfter(inst);
      return create<LiteralT, ValueKind::And8>(
          builder,
          create<LiteralT, ValueKind::Shl8>(builder, x, lit2),
          getLiteral<LiteralT>(
              builder,
              count < kBitWidth ? (nestedLit->getValue() << count) | ~(kAllOnes << count) : 0));
    }
  }

  return nullptr;
}

template <class LiteralT>
Value *simplifyShr(IRBuilder &builder, Instruction *inst) {
  constexpr unsigned kBitWidth = sizeof(typename LiteralT::ValueType) * 8;

  auto *lit1 = dyn_cast<LiteralT>(inst->getOperand(0));
  auto *lit2 = dyn_cast<LiteralU8>(inst->getOperand(1));

  // (Shl const1 const2)
  if (lit1 && lit2) {
    return lit2->getValue() < kBitWidth
        ? getLiteral<LiteralT>(builder, lit1->getValue() >> lit2->getValue())
        : getLiteral<LiteralT>(builder, 0);
  }
  // (Shr 0 x) => 0
  if (lit1 && lit1->getValue() == 0)
    return lit1;
  // (Shr x 0) => x
  if (lit2 && lit2->getValue() == 0)
    return inst->getOperand(0);
  // (Shr x kBitWidth) => 0
  if (lit2 && lit2->getValue() >= kBitWidth)
    return getLiteral<LiteralT>(builder, 0);

  //(Shr (Shr x const1) const2) => (Shl x (Add const1 const2))
  if (lit2 && inst->getOperand(0)->getKind() == arithKindForType<LiteralT>(ValueKind::Shr8)) {
    auto *childInst = cast<Instruction>(inst->getOperand(0));
    if (auto *childLit = dyn_cast<LiteralU8>(childInst->getOperand(1))) {
      builder.setInsertionPointAfter(inst);
      unsigned count = childLit->getValue() + lit2->getValue();
      if (count >= kBitWidth)
        getLiteral<LiteralT>(builder, 0);
      return create<LiteralT, ValueKind::Shr8>(
          builder, childInst->getOperand(0), getLiteral<LiteralU8>(builder, count));
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
    return simplifyTrunc16to8(builder, inst);
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
    if (auto lo = dyn_cast<LiteralU8>(inst->getOperand(0)))
      if (auto hi = dyn_cast<LiteralU8>(inst->getOperand(1)))
        return builder.getLiteralU16(hi->getValue() * 256 + lo->getValue());
    break;
  case ValueKind::Shl8:
    return simplifyShl<LiteralU8>(builder, inst);
  case ValueKind::Shl16:
    return simplifyShl<LiteralU16>(builder, inst);
  case ValueKind::Shr8:
    return simplifyShr<LiteralU8>(builder, inst);
  case ValueKind::Shr16:
    return simplifyShr<LiteralU16>(builder, inst);
  case ValueKind::And8:
    return simplifyAnd<LiteralU8>(builder, inst);
  case ValueKind::And16:
    return simplifyAnd16(builder, inst);
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

  case ValueKind::JmpInd:
    // (JmpInd (CPUAddr2BB const)) => (Jmp bb)
    if (inst->getOperand(0)->getKind() == ValueKind::CPUAddr2BB) {
      auto *addr2bb = cast<Instruction>(inst->getOperand(0));
      if (auto *addr = dyn_cast<LiteralU16>(addr2bb->getOperand(0))) {
        if (auto *bb = inst->getBasicBlock()->getFunction()->findBasicBlock(addr->getValue())) {
          builder.setInsertionPointAfter(inst);
          return builder.createJmp(bb);
        }
      }
    }

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
    for (;;) {
      Instruction *terminator = bb->getTerminator();
      // Look for blocks ending with an unconditional Jmp, where the successor has a single
      // predecessor.
      if (!(terminator->getKind() == ValueKind::Jmp && terminator->getOperand(0)->getOnlyUser()))
        break;

      auto *nextBlock = cast<BasicBlock>(terminator->getOperand(0));
      // Can't touch the entry or exit block.
      if (nextBlock == func->getExitBlock() || nextBlock == func->getEntryBlock() ||
          nextBlock == bb) {
        break;
      }

      auto insts = nextBlock->instructions();
      for (auto it = insts.begin(); it != insts.end();) {
        auto cur = it++;
        bb->importInstruction(&*cur);
      }

      bb->eraseInstruction(terminator);
      func->eraseBasicBlock(nextBlock);
      changed = true;
    }
  }
  return changed;
}

bool simplifyCFG(Module *mod) {
  bool changed = false;
  for (auto &func : mod->functions())
    changed |= simplifyCFG(&func);
  return changed;
}
