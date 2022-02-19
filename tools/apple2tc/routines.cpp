/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "routines.h"

namespace {

bool IdentifySimpleRoutines::run() {
  // Instead of scanning every instruction to see whether it is JSR, we scan
  // every basic block to see whether any of its predecessors are JSR.
  for (auto &bb : func_->basicBlocks()) {
    bool jsr = false;
    for (Instruction &inst : predecessorInsts(bb)) {
      assert(inst.isTerminator());
      if (inst.getKind() == ValueKind::JSR && inst.getOperand(0) == &bb) {
        jsr = true;
        break;
      }
    }
    if (jsr)
      scanCandidate(&bb);
  }

  // Make sure that candidates only JSR into other candidates.
  bool changed;
  do {
    changed = false;
    while (removeInvalidJSRs())
      changed = true;
    if (ctx_->getVerbosity() > 1)
      fprintf(stderr, "%zu candidates remaining after removing invalid JSRs\n", routines_.size());
  } while (changed);

  auto numIdentified = routines_.size();
  if (ctx_->getVerbosity() > 0)
    fprintf(stderr, "%zu simple routines identified\n", numIdentified);
  splitRoutines();
  outlineRoutines();
  return numIdentified != 0;
}

/// Check whether this is a JMP preceded by two pushes. Return the two pushes in
/// order "hi" then "lo".
std::optional<std::tuple<Instruction *, Instruction *>> isSimplePushJmp(Instruction *inst) {
  if (inst->getKind() != ValueKind::Jmp)
    return std::nullopt;

  auto *bb = inst->getBasicBlock();
  int pushCount = 0;
  Instruction *pushes[2];
  for (auto it = bb->instructionToIterator(inst), begin = bb->instructions().begin();
       it != begin;) {
    --it;
    if (it->getKind() == ValueKind::Push8) {
      pushes[pushCount++] = &*it;
      if (pushCount == 2)
        return std::make_tuple(pushes[1], pushes[0]);
    } else if (it->modifiesSP()) {
      return std::nullopt;
    }
  }
  return std::nullopt;
}

void IdentifySimpleRoutines::scanCandidate(BasicBlock *entry) {
  std::unordered_map<BasicBlock *, BasicBlock *> visited{};
  std::unordered_set<BasicBlock *> jsrTargets{};
  std::unordered_set<Instruction *> rts{};
  std::deque<BasicBlock *> workList{};

  if (ctx_->getVerbosity() > 1)
    fprintf(stderr, "$%04x: scan candidate\n", entry->getAddress().value_or(0x10000));

  workList.push_back(entry);
  do {
    BasicBlock *bb = workList.front();
    workList.pop_front();
    // Already visited?
    if (!visited.try_emplace(bb, nullptr).second)
      continue;

    // Indirect branches except RTS are not allowed.
    auto *terminator = bb->getTerminator();
    if (terminator->getKind() != ValueKind::RTS && terminator->isIndirectBranch()) {
      if (ctx_->getVerbosity() > 1)
        fprintf(stderr, "fail: terminator %s\n", getValueKindName(terminator->getKind()));
      return;
    }

    // Check for stack operations. RTS and JSR are allowed. push8/pop8 must match.
    // Others are not allowed.
    int stackLevel = 0;
    for (auto &iRef : bb->instructions()) {
      if (iRef.getKind() == ValueKind::JSR) {
        jsrTargets.insert(cast<BasicBlock>(iRef.getOperand(0)));
      } else if (iRef.getKind() == ValueKind::RTS) {
        rts.insert(&iRef);
      } else if (iRef.getKind() == ValueKind::Push8) {
        ++stackLevel;
      } else if (iRef.getKind() == ValueKind::Pop8) {
        --stackLevel;
        if (stackLevel < 0) {
          if (ctx_->getVerbosity() > 1)
            fprintf(
                stderr,
                "fail: %s block $%04x stack level underflow\n",
                getValueKindName(iRef.getKind()),
                bb->getAddress().value_or(0));
          return;
        }
      } else if (iRef.modifiesSP()) {
        if (ctx_->getVerbosity() > 1)
          fprintf(stderr, "fail: %s\n", getValueKindName(iRef.getKind()));
        return;
      }
    }
    if (stackLevel > 0) {
      if (ctx_->getVerbosity() > 1)
        fprintf(stderr, "fail: block $%04x stack level not zero\n", bb->getAddress().value_or(0));
      return;
    }

    if (terminator->getKind() == ValueKind::RTS) {
      // Do nothing.
    } else if (terminator->getKind() == ValueKind::JSR) {
      // Optimistically continue in the "fall" block.
      workList.push_back(cast<BasicBlock>(terminator->getOperand(1)));
    } else {
      for (auto &succ : successors(*bb))
        workList.push_back(&succ);
    }
  } while (!workList.empty());

  // Now check whether all predecessors are either JSR, Jmp or fall.
  // entry point.
  bool jmp = false;
  for (Instruction &iRef : predecessorInsts(*entry)) {
    auto *inst = &iRef;
    assert(inst->isTerminator());
    // A regular JSR?
    if (inst->getKind() == ValueKind::JSR && inst->getOperand(0) == entry)
      continue;
    // A jmp to the subroutine?
    switch (inst->getKind()) {
    case ValueKind::JSR:
      assert(inst->getOperand(1) == entry);
    case ValueKind::Jmp:
    case ValueKind::JTrue:
    case ValueKind::JFalse:
      jmp = true;
      continue;
    default:;
    }
    // We found a predecessor that is neither of these.
    if (ctx_->getVerbosity() > 1) {
      fprintf(
          stderr,
          "Invalid predecessor inst %s at $%04x\n",
          getValueKindName(inst->getKind()),
          inst->getAddress().value_or(0x10000));
    }
    return;
  }

  routines_.try_emplace(entry, std::move(visited), std::move(jsrTargets), std::move(rts));

  if (ctx_->getVerbosity() > 1) {
    fprintf(
        stderr, "$%04x: created candidate%s\n", entry->getAddress().value_or(0), jmp ? " jmp" : "");
  }
}

bool IdentifySimpleRoutines::removeInvalidJSRs() {
  bool changed = false;
  for (auto it = routines_.begin(), end = routines_.end(); it != end;) {
    auto cur = it++;
    for (BasicBlock *jsrTarget : cur->second.jsrTargets) {
      if (!routines_.count(jsrTarget)) {
        if (ctx_->getVerbosity() > 1)
          fprintf(
              stderr,
              "Removing candidate $%04x because of invalid JSR to $%04x\n",
              cur->first->getAddress().value_or(0),
              jsrTarget->getAddress().value_or(0));
        routines_.erase(cur);
        changed = true;
        break;
      }
    }
  }
  return changed;
}

void IdentifySimpleRoutines::splitRoutines() {
  // Sort all routines reproducibly.
  std::vector<std::pair<BasicBlock *, Routine *>> sortedCandidates{};
  sortedCandidates.reserve(routines_.size());
  for (auto &p : routines_)
    sortedCandidates.emplace_back(p.first, &p.second);
  std::sort(sortedCandidates.begin(), sortedCandidates.end(), [](const auto &a, const auto &b) {
    auto addrA = a.first->getAddress().value_or(0x10000);
    auto addrB = b.first->getAddress().value_or(0x10000);
    if (addrA < addrB)
      return true;
    else if (addrA == addrB)
      return a.first->getUniqueId() < b.first->getUniqueId();
    else
      return false;
  });

  // Create a map of all blocks and which routine they belong to.
  for (auto [_, pCand] : sortedCandidates)
    for (auto [bb, _] : pCand->blocks)
      blocks_.emplace(bb, pCand);

  for (auto [entry, pCand] : sortedCandidates)
    convertInvocations(entry, *pCand);

  IRBuilder builder(ctx_);
  for (auto [entry, pCand] : sortedCandidates)
    extractRoutine(builder, entry, *pCand);

  // Not needed anymore.
  blocks_.clear();
}

void IdentifySimpleRoutines::convertInvocations(BasicBlock *entry, Routine &cand) {
  Module *mod = func_->getModule();
  IRBuilder builder(ctx_);

  cand.func = mod->createFunction();
  cand.func->setName(entry->getName());
  cand.func->setDecompileLevel(Function::DecompileLevel::Normal);

  // Record all addresses this routine returns to as "dynamic blocks".
  PrimitiveSetVector<BasicBlock *> dynamicReturnBlocks{};
  for (auto *inst : cand.rts) {
    for (unsigned i = 1, count = inst->getNumOperands(); i < count; ++i)
      dynamicReturnBlocks.insert(cast<BasicBlock>(inst->getOperand(i)));
  }

  // The "fall" block of a JSR doesn't need to be considered a dynamic address,
  // so remove all of those.
  for (auto &iRef : predecessorInsts(*entry)) {
    if (iRef.getKind() == ValueKind::JSR && iRef.getOperand(1) != entry)
      dynamicReturnBlocks.erase(cast<BasicBlock>(iRef.getOperand(1)));
  }

  // Convert all subroutine invocations into Call instructions.
  // A predecessor can be present twice (JSR func, func), so we need to use a set.
  {
    PrimitiveSetVector<Instruction *> preds{};
    for (auto &iRef : predecessorInsts(*entry))
      preds.insert(&iRef);
    for (auto *inst : preds)
      if (convertRoutineInvocation(builder, entry, cand, inst, dynamicReturnBlocks))
        inst->eraseFromBasicBlock();
  }

#ifndef NDEBUG
  // All users of the entry block must be internal.
  for (auto &user : entry->users()) {
    assert(
        cand.blocks.count(cast<Instruction>(user.owner())->getBasicBlock()) &&
        "All external users of the entry block should have been eliminated");
  }
#endif
}

std::vector<BasicBlock *> dfsOrder(BasicBlock *entry) {
  std::vector<BasicBlock *> stack{};
  std::unordered_set<BasicBlock *> visited{};
  std::vector<BasicBlock *> order{};

  stack.push_back(entry);
  while (!stack.empty()) {
    auto *bb = stack.back();
    stack.pop_back();
    if (!visited.insert(bb).second)
      continue;
    order.push_back(bb);
    if (bb->getTerminator()->getKind() == ValueKind::RTS)
      continue;
    for (auto &succ : successors(*bb))
      stack.push_back(&succ);
  }

  return order;
}

void IdentifySimpleRoutines::extractRoutine(IRBuilder &builder, BasicBlock *entry, Routine &cand) {
  // Map from old block to new block.
  std::unordered_map<Value *, Value *> valueMap{};
  auto order = dfsOrder(entry);

  assert(order[0] == entry);
  for (auto *oldBB : order) {
    auto *newBB = cand.func->createBasicBlock();
    newBB->setAddress(oldBB->getAddress(), oldBB->isRealAddress());
    cand.addFuncBlock(newBB, oldBB);
    auto res = valueMap.try_emplace(oldBB, newBB);
    (void)res;
    assert(res.second && "Blocks cannot be repeated");
  }
  cand.func->createExitBlock();

  // TODO: deal with Phi instructions when we introduce them?

  std::vector<Value *> operands{};

  for (auto *oldBB : order) {
    auto *newBB = cast<BasicBlock>(valueMap[oldBB]);

    builder.setInsertionBlock(newBB);
    for (auto &iRef : oldBB->instructions()) {
      Instruction *oldInst = &iRef;
      builder.setAddress(oldInst->getAddress());

      if (oldInst->getKind() == ValueKind::RTS) {
        builder.createReturn(cand.func->getExitBlock());
        continue;
      }

      operands.clear();
      for (auto &rOp : oldInst->operands()) {
        auto *op = &rOp;
        if (op->getKind() == ValueKind::Function || op->isLiteralNumber() || op->isCPUReg()) {
          operands.push_back(op);
        } else {
          auto *newOp = valueMap[op];
          assert(newOp && "Operand must be visited before instruction using it");
          operands.push_back(newOp);
        }
      }

      auto *newInst = builder.createInst(oldInst->getKind(), operands);
      valueMap[oldInst] = newInst;
    }
  }
}

bool IdentifySimpleRoutines::convertRoutineInvocation(
    IRBuilder &builder,
    BasicBlock *entry,
    const Routine &cand,
    Instruction *inst,
    const PrimitiveSetVector<BasicBlock *> &dynamicReturnBlocks) {
  // Some cases set this to the IR instruction that must get all dynamic return blocks
  // as successors.
  Instruction *dynamicBranch = nullptr;

  // Create a new basic block that calls the routine and RTS-es.
  auto createCallBlock = [this, &builder, &dynamicBranch, &cand, inst]() {
    BasicBlock *callBlock = inst->getBasicBlock()->getFunction()->createBasicBlock();

    // Record that the new block belongs to all routines that owned the original
    // block.
    auto range = blocks_.equal_range(inst->getBasicBlock());
    for (auto it = range.first; it != range.second; ++it) {
      Routine *bCand = it->second;
      bCand->blocks.emplace(callBlock, nullptr);
      blocks_.emplace(callBlock, bCand);
    }

    builder.setInsertionBlock(callBlock);
    builder.setAddress(inst->getAddress());
    builder.createCall(cand.func, builder.getLiteralU16(0));
    dynamicBranch = builder.createRTS(builder.getLiteralU8(0));
    return callBlock;
  };

  switch (inst->getKind()) {
  case ValueKind::JSR: {
    // Check for this case, where the JSR is in a different function.
    //       JSR func
    // func:
    bool fallFromAnother =
        inst->getOperand(1) == entry && !cand.blocks.count(inst->getBasicBlock());

    if (inst->getOperand(0) == entry) {
      // This is the "normal" case: a JSR to the subroutine.
      builder.setInsertionPointAfter(inst);
      builder.setAddress(inst->getAddress());
      builder.createCall(
          cand.func,
          builder.getLiteralU16(
              ctx_->getPreserveReturnAddress() ? inst->getAddress().value_or(0xFFFC) + 2 : 0xFFFE));
      if (!fallFromAnother) {
        builder.createJmp(inst->getOperand(1));
      } else {
        builder.createCall(cand.func, builder.getLiteralU16(0));
        builder.createRTS(builder.getLiteralU8(0));
      }
    } else {
      // A JSR followed by the subroutine entry point.
      assert(inst->getOperand(1) == entry);
      if (!fallFromAnother)
        return false;

      BasicBlock *callBlock = createCallBlock();

      builder.setInsertionPointAfter(inst);
      builder.setAddress(inst->getAddress());
      builder.createJSR(inst->getOperand(0), callBlock);
    }
    break;
  }

  case ValueKind::JTrue:
  case ValueKind::JFalse: {
    if (cand.blocks.count(inst->getBasicBlock()))
      return false;

    BasicBlock *callBlock = createCallBlock();

    // Redirect the jmp to the subroutine to the new block calling the subroutine.
    Value *op1 = inst->getOperand(1);
    Value *op2 = inst->getOperand(2);
    if (op1 == entry)
      op1 = callBlock;
    if (op2 == entry)
      op2 = callBlock;

    builder.setInsertionPointAfter(inst);
    builder.setAddress(inst->getAddress());
    builder.createInst(inst->getKind(), {inst->getOperand(0), op1, op2});
    break;
  }

  case ValueKind::Jmp:
    if (auto pushJmp = isSimplePushJmp(inst)) {
      // push-jmp.
      auto [pushHi, pushLo] = *pushJmp;
      builder.setInsertionPointAfter(pushHi);
      builder.setAddress(pushHi->getAddress());
      builder.createPush8(
          ctx_->getPreserveReturnAddress() ? pushHi->getOperand(0) : builder.getLiteralU8(0xFF));
      builder.setInsertionPointAfter(pushLo);
      builder.setAddress(pushLo->getAddress());
      builder.createPush8(
          ctx_->getPreserveReturnAddress() ? pushLo->getOperand(0) : builder.getLiteralU8(0xFE));
      auto *addr = builder.createCPUAddr2BB(builder.createAdd16(
          builder.createMake16(pushLo->getOperand(0), pushHi->getOperand(0)),
          builder.getLiteralU16(1)));

      builder.setInsertionPointAfter(inst);
      builder.setAddress(inst->getAddress());
      builder.createCall(cand.func, builder.getLiteralU16(0));
      builder.createPop8();
      builder.createPop8();

      dynamicBranch = builder.createJmpInd(addr);

      pushHi->eraseFromBasicBlock();
      pushLo->eraseFromBasicBlock();
    } else {
      // Just a jmp.
      if (cand.blocks.count(inst->getBasicBlock()))
        return false;

      builder.setInsertionPointAfter(inst);
      builder.setAddress(inst->getAddress());
      builder.createCall(cand.func, builder.getLiteralU16(0));
      dynamicBranch = builder.createRTS(builder.getLiteralU8(0));
    }
    break;

  default:
    PANIC_ABORT("Invalid routine invocation instruction %s", getValueKindName(inst->getKind()));
  }

  if (dynamicBranch)
    for (auto *bb : dynamicReturnBlocks)
      dynamicBranch->pushOperand(bb);

  return true;
}

template <class K, class V>
bool isSubset(const std::unordered_map<K, V> &small, const std::unordered_map<K, V> &large) {
  return std::all_of(
      small.begin(), small.end(), [&large](const auto &pair) { return large.count(pair.first); });
}

IdentifySimpleRoutines::RoutinePair IdentifySimpleRoutines::wrapMainRoutine() {
  std::unordered_map<BasicBlock *, BasicBlock *> blocks{};
  for (auto &bb : func_->basicBlocks())
    blocks.emplace(&bb, &bb);

  return {func_->getEntryBlock(), Routine(std::move(blocks), {}, {}, func_, true)};
}

void IdentifySimpleRoutines::outlineRoutines() {
  func_->getModule()->dump();
  auto mainRoutine = wrapMainRoutine();

  // Routines sorted by number of blocks in decreasing order.
  std::vector<RoutinePair *> sortedRoutines{};
  sortedRoutines.reserve(routines_.size() + 1);
  // The main routine is always at index 0.
  sortedRoutines.push_back(&mainRoutine);
  for (auto &pair : routines_)
    sortedRoutines.push_back(&pair);
  std::sort(
      sortedRoutines.begin() + 1,
      sortedRoutines.end(),
      [](const RoutinePair *a, const RoutinePair *b) {
        auto sizeA = a->second.blocks.size();
        auto sizeB = b->second.blocks.size();
        return sizeA > sizeB ||
            (sizeA == sizeB &&
             a->first->getAddress().value_or(0x10000) < b->first->getAddress().value_or(0x10000));
      });

  for (;;) {
    // Find the largest routine that is a subset of another one.
    size_t small = sortedRoutines.size();
    size_t large;
    for (large = 0; large < sortedRoutines.size() - 1; ++large) {
      for (small = large + 1; small < sortedRoutines.size(); ++small) {
        if (!sortedRoutines[small]->second.dirty &&
            isSubset(sortedRoutines[small]->second.blocks, sortedRoutines[large]->second.blocks)) {
          if (ctx_->getVerbosity() > 0) {
            fprintf(
                stderr,
                "%s is a subset of %s\n",
                sortedRoutines[small]->second.func->debuggingName().c_str(),
                sortedRoutines[large]->second.func->debuggingName().c_str());
          }
          goto foundOne;
        }
      }
    }
    // Couldn't find one.
    break;

  foundOne:
    outline(*sortedRoutines[small], *sortedRoutines[large]);
    func_->getModule()->dump();

    // The large routine has become smaller. Move it to its new position unless
    // it is the main routine.
    if (large > 0) {
      while (large < sortedRoutines.size() - 1 &&
             sortedRoutines[large]->second.blocks.size() <
                 sortedRoutines[large + 1]->second.blocks.size()) {
        std::swap(sortedRoutines[large], sortedRoutines[large + 1]);
        ++large;
      }
    }
  }
}

void IdentifySimpleRoutines::outline(RoutinePair &small, RoutinePair &large) {
  IRBuilder builder(ctx_);
  Function *largeFunc = large.second.func;
  Function *smallFunc = small.second.func;

  {
    // Create a new entry block in "large func" and have it jump to the old entry
    // block. We need this in case the old entry also happens to be part of "small func".
    // Then it would get an additional predecessor and would get cloned.
    BasicBlock *largeOldEntry = largeFunc->getEntryBlock();
    BasicBlock *largeNewEntry = largeFunc->createBasicBlock();
    builder.setInsertionBlock(largeNewEntry);
    builder.createJmp(largeOldEntry);
    largeFunc->setEntryBlock(largeNewEntry);
  }

  // The block in large func where we are "invoking" small func.
  BasicBlock *largeInvokeBlock = large.second.funcBlockFromOriginal(
      small.second.originalFromFuncBlock(smallFunc->getEntryBlock()));

  // The new invoke block is a call.
  BasicBlock *newInvokeBlock = largeFunc->createBasicBlock();
  builder.setInsertionBlock(newInvokeBlock);
  builder.createCall(smallFunc, builder.getLiteralU16(0));
  // FIXME: this is a temporary hack to get something out.
  builder.createReturn(largeFunc->getExitBlock());

  // Point all users in large func to the new block.
  {
    std::vector<decltype(&*largeInvokeBlock->users().begin())> largeUsers{};
    for (auto &user : largeInvokeBlock->users()) {
      // If the user basic block that doesn't belong to small func, save it.
      if (!small.second.blocks.count(
              large.second.originalFromFuncBlock(user.owner()->getBasicBlock()))) {
        largeUsers.push_back(&user);
      }
    }
    for (auto *user : largeUsers)
      user->setValue(newInvokeBlock);
  }
  largeInvokeBlock->clearAllOperands();
  largeFunc->eraseBasicBlock(largeInvokeBlock);
  large.second.removeFuncBlock(largeInvokeBlock);

  // A set of all blocks in small func that need to be kept after the outlining process.
  std::unordered_set<BasicBlock *> smallKeep{};
  // General purpose basic block stack.
  std::vector<BasicBlock *> bbStack{};

  // Mark the specified block and all blocks reachable from it as "to be kept".
  auto markAllReachableAsKeep = [&smallKeep, &bbStack](BasicBlock *smallBB) {
    bbStack.push_back(smallBB);
    do {
      BasicBlock *bb = bbStack.back();
      bbStack.pop_back();
      if (!smallKeep.insert(bb).second)
        continue;
      for (auto &succ : successors(*bb))
        bbStack.push_back(&succ);
    } while (!bbStack.empty());
  };

  // Mark all cloned blocks. We simply walk all blocks in any order and check
  // whether a block has a predecessor in large func that isn't in small func.
  // If it does, we mark it and all blocks reachable from it as "to be kept".
  for (auto &bbRef : smallFunc->basicBlocks()) {
    BasicBlock *smallBB = &bbRef;
    // Skip the entry and exit blocks.
    if (smallBB == smallFunc->getEntryBlock() || smallBB == smallFunc->getExitBlock())
      continue;
    // If already marked, skip it.
    if (smallKeep.count(smallBB))
      continue;

    BasicBlock *originalBB = small.second.originalFromFuncBlock(smallBB);
    BasicBlock *largeBB = large.second.funcBlockFromOriginal(originalBB);

    // Does the largeBB have any predecessors that are not in small func?
    auto &&preds = predecessors(*largeBB);
    bool extraPreds =
        !std::all_of(preds.begin(), preds.end(), [&small, &large](BasicBlock &predBB) {
          return small.second.blocks.count(large.second.originalFromFuncBlock(&predBB)) != 0;
        });
    if (extraPreds)
      markAllReachableAsKeep(smallBB);
  }

  // Delete all blocks that should not be kept.
  // First clear all their operands in one pass to remove dependencies.
  std::vector<BasicBlock *> largeErase{};
  largeErase.reserve(small.second.blocks.size() - smallKeep.size());
  for (auto &bbRef : smallFunc->basicBlocks()) {
    BasicBlock *smallBB = &bbRef;
    if (smallBB == smallFunc->getEntryBlock() || smallBB == smallFunc->getExitBlock())
      continue;
    if (smallKeep.count(smallBB))
      continue;
    BasicBlock *originalBB = small.second.originalFromFuncBlock(smallBB);
    BasicBlock *largeBB = large.second.funcBlockFromOriginal(originalBB);
    largeBB->clearAllOperands();
    largeErase.push_back(largeBB);
  }
  // Now delete them all in a second pass.
  for (auto *largeBB : largeErase) {
    largeFunc->eraseBasicBlock(largeBB);
    large.second.removeFuncBlock(largeBB);
  }

  // Remove the temporary entry block. Keep in mind that the old entry
  // block may have been deleted, so obtain the successor of the temp one.
  BasicBlock *tempEntryBlock = largeFunc->getEntryBlock();
  Instruction *jmp = tempEntryBlock->getTerminator();
  assert(jmp->getKind() == ValueKind::Jmp);
  largeFunc->setEntryBlock(cast<BasicBlock>(jmp->getOperand(0)));
  tempEntryBlock->clearAllOperands();
  largeFunc->eraseBasicBlock(tempEntryBlock);
}

} // namespace

/// - Single entry
/// - Contain no indirect jumps
/// - Contain no stack pointer manipulation
bool identifySimpleRoutines(Module *mod) {
  for (auto &func : mod->functions()) {
    // Only run it on the global function.
    return IdentifySimpleRoutines(&func).run();
  }
  return false;
}