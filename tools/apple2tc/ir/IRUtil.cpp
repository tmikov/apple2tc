/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "IRUtil.h"

namespace ir {

Range32 classifyMemoryAddr(Value *addr, unsigned width) {
  // Treat implicit stack instruction specially.
  switch (addr->getKind()) {
  case ValueKind::Pop8:
  case ValueKind::Push8:
  case ValueKind::RTS:
  case ValueKind::JSR:
  case ValueKind::JSRInd:
    return {256, 512};
  default:
    break;
  }

  assert(addr->getType()->getKind() == TypeKind::U16 && "Address must be 16-bit");

  if (auto *u16 = dyn_cast<LiteralU16>(addr))
    return {u16->getValue(), u16->getValue() + width};
  if (addr->getKind() == ValueKind::ZExt8t16) {
    return {0, 255 + width};
  }

  if (addr->getKind() == ValueKind::Add16) {
    auto *inst = cast<Instruction>(addr);
    auto *op1 = inst->getOperand(0);
    auto *op2 = inst->getOperand(1);
    // If we have a zext, it will be in operand2.
    if (op1->getKind() == ValueKind::ZExt8t16)
      std::swap(op1, op2);
    // zext + zext.
    if (op2->getKind() == ValueKind::ZExt8t16) {
      // Zext + Zext <= 255*2.
      if (op1->getKind() == ValueKind::ZExt8t16)
        return {0, 255 * 2 + width};
      // literal16 + [0..255] + ofs
      if (auto *op1u16 = dyn_cast<LiteralU16>(op1)) {
        unsigned base = op1u16->getValue();
        return {base, ((base + 255) & 0xFFFF) + width};
      }
    }
  }

  return {0, 0x10000};
}

ValueKind negateComparison(ValueKind kind) {
  assert(kind >= ValueKind::_Comparison_First && kind <= ValueKind::_Comparison_Last);
  constexpr auto kFirst = (unsigned)ValueKind::_Comparison_First;
  return (ValueKind)((((unsigned)kind - kFirst) ^ 1) + kFirst);
}

} // namespace ir
