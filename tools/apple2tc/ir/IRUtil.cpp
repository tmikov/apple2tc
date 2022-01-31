/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "IRUtil.h"

namespace ir {

bool Range32::overlap16WithWrap(Range32 r) const {
  return this->overlap32(r) || Range32(this->begin + k64K, this->end + k64K).overlap32(r) ||
      Range32(r.begin + k64K, r.end + k64K).overlap32(*this);
}

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
        return {base, base + 255 + width};
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

void markExpressionTrees(BasicBlock *bb, InstSet &validTrees) {
  // Keeps track of all instructions that are part of not generated expression tree.
  // As soon as a tree is generated (after reaching an instruction that uses it),
  // all of its nodes are removed from here.
  InstSet trees{};
  // Explicit stack used to avoid recursion when removing a tree from `trees` above.
  std::vector<Instruction *> clearStack{};

  /// Pop every element from the stack, clear its operands from the tree set and
  /// recursively process their operands by pushing them on the stack.
  auto drainClearStack = [&clearStack, &trees]() {
    while (!clearStack.empty()) {
      auto *toClear = clearStack.back();
      clearStack.pop_back();
      for (auto &op : toClear->operands()) {
        if (auto *opInst = dyn_cast<Instruction>(&op)) {
          if (trees.erase(opInst))
            clearStack.push_back(opInst);
        }
      }
    }
  };

  // An instruction can't be a part of an expression tree. It has to be
  // emitted. All trees referenced by it are cleared.
  auto emitInstruction = [&clearStack, &drainClearStack](Instruction *inst) {
    clearStack.push_back(inst);
    drainClearStack();
  };

  // Invoke the specified callback with every instruction in 'trees'. If the callback returns true,
  // the instruction is "invalidated": removed from trees and validTrees.
  auto invalidateTreesIf =
      [&trees, &validTrees, &clearStack, &drainClearStack](auto checkInvalidate) {
        for (auto begin = trees.begin(), end = trees.end(); begin != end;) {
          auto cur = begin++;
          auto *tInst = *cur;
          if (checkInvalidate(tInst)) {
            trees.erase(cur);
            validTrees.erase(tInst);
            // Push the instruction onto the "clear" stack, so its operands will
            // be recursively cleared in the end.
            clearStack.push_back(tInst);
          }
        }
        drainClearStack();
      };

  for (auto &rInst : bb->instructions()) {
    auto *inst = &rInst;
    Instruction *onlyUser;
    if (!inst->getType()->isVoid() && !inst->hasSideEffects() &&
        (onlyUser = inst->getOnlyUser()) != nullptr && onlyUser->getBasicBlock() == bb) {
      // Is this instruction suitable for adding to an expression tree?
      // A non-void instruction without side effects, with exactly one user in
      // the same basic block.
      trees.insert(inst);
      validTrees.insert(inst);
    } else {
      // This instruction can't be a part of an expression tree. It has to be
      // emitted. All referenced trees are cleared.
      emitInstruction(inst);

      // Check if it invalidated any tree leaves.
      if (inst->getKind() == ValueKind::StoreR8) {
        // Invalidate register leaves.
        invalidateTreesIf([cpuReg = inst->getOperand(0)](Instruction *tInst) {
          return tInst->getKind() == ValueKind::LoadR8 && tInst->getOperand(0) == cpuReg;
        });
      }
      if (inst->modifiesSP()) {
        // Invalidate SP leaves.
        invalidateTreesIf([](Instruction *tInst) { return tInst->modifiesSP(); });
      }
      if (inst->writesMemory()) {
        auto [writeAddr, writeWidth] = inst->memoryAddress();
        auto writeRange = classifyMemoryAddr(writeAddr, writeWidth);

        // Invalidate memory read leaves.
        invalidateTreesIf([writeRange](Instruction *tInst) {
          if (tInst->readsMemory()) {
            auto [readAddr, readWidth] = tInst->memoryAddress();
            auto readRange = classifyMemoryAddr(readAddr, readWidth);
            if (writeRange.overlap16WithWrap(readRange))
              return true;
          }
          return false;
        });
      }
    }
  }
  assert(trees.empty() && "All trees must have been consumed by the end of the basic block");
}

} // namespace ir
