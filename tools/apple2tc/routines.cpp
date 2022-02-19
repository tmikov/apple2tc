/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "PubIR.h"
#include "ir/IR.h"
#include "ir/IRUtil.h"

#include "apple2tc/SetVector.h"
#include "apple2tc/support.h"

#include <deque>

using namespace ir;

namespace {

class IdentifySimpleRoutines {
  Function *const func_;
  IRContext *const ctx_;
  /// Blocks of all identified candidates so far. We need this to ensure that
  /// they don't overlap.
  std::unordered_set<BasicBlock *> discoveredBlocks_{};

  struct Candidate {
    std::unordered_set<BasicBlock *> blocks;
    std::unordered_set<BasicBlock *> jsrTargets;
    std::unordered_set<Instruction *> rts;
    Function *func = nullptr;

    Candidate(
        std::unordered_set<BasicBlock *> &&blocks,
        std::unordered_set<BasicBlock *> &&jsrTargets,
        std::unordered_set<Instruction *> &&rts)
        : blocks(std::move(blocks)), jsrTargets(std::move(jsrTargets)), rts(std::move(rts)) {}
  };

  std::unordered_map<BasicBlock *, Candidate> candidates_{};

public:
  explicit IdentifySimpleRoutines(Function *func)
      : func_(func), ctx_(func->getModule()->getContext()) {}
  bool run();

private:
  /// Collect the basic block of the candidate.
  void scanCandidate(BasicBlock *entry);
  /// Remove candidates that JSR into a non-candidate. Return true if anything
  /// changed.
  bool removeInvalidJSRs();
  /// Remove candidates containing blocks with predecessors not belonging to the
  /// candidate.
  bool removeInvalidPreds();
  /// Return the candidate owning this block, or nullptr.
  BasicBlock *findOwningCandidate(BasicBlock *block);

  /// Actually split the candidates into new IR routines.
  void splitRoutines();

  /// Split a single routine.
  void splitARoutine(BasicBlock *entry, Candidate &cand);

  bool convertRoutineInvocation(
      IRBuilder &builder,
      BasicBlock *entry,
      const Candidate &cand,
      Instruction *inst,
      const PrimitiveSetVector<BasicBlock *> &dynamicReturnBlocks);
};

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
    if (jsr) {
      if (discoveredBlocks_.count(&bb)) {
        if (ctx_->getVerbosity() > 1)
          fprintf(stderr, "$%04x: ignore candidate\n", bb.getAddress().value_or(0x10000));
        continue;
      }

      scanCandidate(&bb);
    }
  }

  // Make sure that candidates only JSR into other candidates.
  bool changed;
  do {
    changed = false;
    while (removeInvalidJSRs())
      changed = true;
    if (ctx_->getVerbosity() > 1)
      fprintf(stderr, "%zu candidates remaining after removing invalid JSRs\n", candidates_.size());

    while (removeInvalidPreds())
      changed = true;
    if (ctx_->getVerbosity() > 1)
      fprintf(
          stderr, "%zu candidates remaining after removing invalid preds\n", candidates_.size());
  } while (changed);

  auto numIdentified = candidates_.size();
  if (ctx_->getVerbosity() > 0)
    fprintf(stderr, "%zu simple routines identified\n", numIdentified);
  splitRoutines();
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
  std::unordered_set<BasicBlock *> visited{};
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
    if (!visited.insert(bb).second)
      continue;

    if (discoveredBlocks_.count(bb)) {
      if (ctx_->getVerbosity() > 1)
        fprintf(
            stderr, "fail: bb $%04x already discovered checked\n", bb->getAddress().value_or(0));
      return;
    }

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

  // Now check whether all predecessors are either JSR, push+jmp, or are internal branches to the
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
    if (visited.count(inst->getBasicBlock())) {
      // If the predecessor is part of the same function, it is an internal loop
      // to the entry point.
      continue;
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

  // Add all blocks to discovered.
  for (auto *bb : visited)
    discoveredBlocks_.insert(bb);

  candidates_.try_emplace(entry, std::move(visited), std::move(jsrTargets), std::move(rts));

  if (ctx_->getVerbosity() > 1) {
    fprintf(
        stderr, "$%04x: created candidate%s\n", entry->getAddress().value_or(0), jmp ? " jmp" : "");
  }
}

bool IdentifySimpleRoutines::removeInvalidJSRs() {
  bool changed = false;
  for (auto it = candidates_.begin(), end = candidates_.end(); it != end;) {
    auto cur = it++;
    for (BasicBlock *jsrTarget : cur->second.jsrTargets) {
      if (!candidates_.count(jsrTarget)) {
        if (ctx_->getVerbosity() > 1)
          fprintf(
              stderr,
              "Removing candidate $%04x because of invalid JSR to $%04x\n",
              cur->first->getAddress().value_or(0),
              jsrTarget->getAddress().value_or(0));
        candidates_.erase(cur);
        changed = true;
        break;
      }
    }
  }
  return changed;
}

bool IdentifySimpleRoutines::removeInvalidPreds() {
  bool changed = false;
  for (auto it = candidates_.begin(), end = candidates_.end(); it != end;) {
    auto cur = it++;
    BasicBlock *entry = cur->first;
    Candidate &cand = cur->second;

    for (auto *bb : cand.blocks) {
      // Skip the entry block.
      if (bb == entry)
        continue;

      for (auto &pred : predecessors(*bb)) {
        if (!cand.blocks.count(&pred)) {
          // It could be an RTS in another candidate.
          if (!(pred.getTerminator()->getKind() == ValueKind::RTS && findOwningCandidate(&pred))) {
            if (ctx_->getVerbosity() > 1) {
              fprintf(
                  stderr,
                  "fail: predecessor $%04x of $%04x not in routine $%04x\n",
                  pred.getAddress().value_or(0),
                  bb->getAddress().value_or(0),
                  entry->getAddress().value_or(0));
            }
            candidates_.erase(cur);
            changed = true;
            goto endLoop;
          }
        }
      }
    }
  endLoop:;
  }
  return changed;
}

BasicBlock *IdentifySimpleRoutines::findOwningCandidate(BasicBlock *block) {
  for (auto &[entry, cand] : candidates_) {
    if (cand.blocks.count(block))
      return entry;
  }
  return nullptr;
}

void IdentifySimpleRoutines::splitRoutines() {
  // Sort all routines reproducibly.
  std::vector<std::pair<BasicBlock *, Candidate *>> sortedCandidates{};
  sortedCandidates.reserve(candidates_.size());
  for (auto &p : candidates_)
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

  for (auto [entry, pCand] : sortedCandidates)
    splitARoutine(entry, *pCand);
}

bool IdentifySimpleRoutines::convertRoutineInvocation(
    IRBuilder &builder,
    BasicBlock *entry,
    const Candidate &cand,
    Instruction *inst,
    const PrimitiveSetVector<BasicBlock *> &dynamicReturnBlocks) {
  // Some cases set this to the IR instruction that must get all dynamic return blocks
  // as successors.
  Instruction *dynamicBranch = nullptr;

  // Create a new basic block that calls the routine and RTS-es.
  auto createCallBlock = [&builder, &dynamicBranch, func = cand.func, inst]() {
    BasicBlock *callBlock = inst->getBasicBlock()->getFunction()->createBasicBlock();
    builder.setInsertionBlock(callBlock);
    builder.setAddress(inst->getAddress());
    builder.createCall(func, builder.getLiteralU8(0));
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
      builder.createCall(cand.func, builder.getLiteralU8(1));
      if (!fallFromAnother) {
        builder.createJmp(inst->getOperand(1));
      } else {
        builder.createCall(cand.func, builder.getLiteralU8(0));
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
      builder.createPush8(builder.getLiteralU8(0xFF));
      builder.setInsertionPointAfter(pushLo);
      builder.setAddress(pushLo->getAddress());
      builder.createPush8(builder.getLiteralU8(0xFE));
      auto *addr = builder.createCPUAddr2BB(builder.createAdd16(
          builder.createMake16(pushLo->getOperand(0), pushHi->getOperand(0)),
          builder.getLiteralU16(1)));

      builder.setInsertionPointAfter(inst);
      builder.setAddress(inst->getAddress());
      builder.createCall(cand.func, builder.getLiteralU8(0));
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
      builder.createCall(cand.func, builder.getLiteralU8(0));
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

void IdentifySimpleRoutines::splitARoutine(BasicBlock *entry, Candidate &cand) {
  Module *mod = func_->getModule();
  IRBuilder builder(ctx_);

  cand.func = mod->createFunction();
  cand.func->setName(entry->getName());
  cand.func->setDecompileLevel(Function::DecompileLevel::Normal);

  // Replace all RTS with Return. Record all return addresses.
  PrimitiveSetVector<BasicBlock *> dynamicReturnBlocks{};

  // Extract all return addresses.
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

  cand.func->importBasicBlock(entry);
  auto sortedBlocks = sortByAddress<BasicBlock>(cand.blocks, false);
  for (auto *bb : sortedBlocks)
    if (bb != entry)
      cand.func->importBasicBlock(bb);

  cand.func->createExitBlock();

  // Replace all RTS with Return.
  for (auto *inst : cand.rts) {
    builder.setInsertionPointAfter(inst);
    builder.createReturn(cand.func->getExitBlock());
    inst->eraseFromBasicBlock();
  }
  cand.rts.clear();
}

} // namespace

/// "Simple leaf" routines means:
/// - Leaf routines
/// - Single entry
/// - Always entered via a JSR instruction
/// - Contain no indirect jumps
/// - Contain no stack pointer manipulation
bool identifySimpleRoutines(Module *mod) {
  for (auto &func : mod->functions()) {
    // Only run it on the global function.
    return IdentifySimpleRoutines(&func).run();
  }
  return false;
}