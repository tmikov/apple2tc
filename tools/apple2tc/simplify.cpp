/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "PubIR.h"
#include "ir/IR.h"

using namespace ir;

template <class T, class C>
static bool isLiteral(Value *v, C c) {
  if (auto *lit = dyn_cast<T>(v))
    return lit->getValue() == c;
  return false;
}

// static Instruction *simplifyPeekPoke(Instruction *inst) {}

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
    // (Add8 0 x) => x
    if (isLiteral<LiteralU8>(inst->getOperand(0), 0))
      return inst->getOperand(1);
    // (Add8 x 0) => x
    if (isLiteral<LiteralU8>(inst->getOperand(1), 0))
      return inst->getOperand(0);
    // (Add8 const1 const2)
    if (auto *left = dyn_cast<LiteralU8>(inst->getOperand(0)))
      if (auto *right = dyn_cast<LiteralU8>(inst->getOperand(1)))
        return builder.getLiteralU8(left->getValue() + right->getValue());
    break;
  case ValueKind::Add16:
    // (Add16 0 x) => x
    if (isLiteral<LiteralU16>(inst->getOperand(0), 0))
      return inst->getOperand(1);
    // (Add16 x 0) => x
    if (isLiteral<LiteralU16>(inst->getOperand(1), 0))
      return inst->getOperand(0);
    // (Add16 const1 const2)
    if (auto *left = dyn_cast<LiteralU16>(inst->getOperand(0)))
      if (auto *right = dyn_cast<LiteralU16>(inst->getOperand(1)))
        return builder.getLiteralU16(left->getValue() + right->getValue());
    break;
  case ValueKind::Sub8:
    // (Sub8 x 0) => x
    if (isLiteral<LiteralU8>(inst->getOperand(1), 0))
      return inst->getOperand(0);
    // (Sub8 const1 const2)
    if (auto *left = dyn_cast<LiteralU8>(inst->getOperand(0)))
      if (auto *right = dyn_cast<LiteralU8>(inst->getOperand(1)))
        return builder.getLiteralU8(left->getValue() - right->getValue());
    break;
  case ValueKind::Sub16:
    // (Sub16 x 0) => x
    if (isLiteral<LiteralU16>(inst->getOperand(1), 0))
      return inst->getOperand(0);
    // (Sub16 const1 const2)
    if (auto *left = dyn_cast<LiteralU16>(inst->getOperand(0)))
      if (auto *right = dyn_cast<LiteralU16>(inst->getOperand(1)))
        return builder.getLiteralU16(left->getValue() - right->getValue());
    break;
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
  case ValueKind::EncodeFlags:
    break;
  case ValueKind::DecodeFlags:
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
