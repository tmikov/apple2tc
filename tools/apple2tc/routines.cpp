/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "PubIR.h"
#include "ir/IR.h"
#include "ir/IRUtil.h"

#include <deque>

using namespace ir;

namespace {

class IdentifySimpleRoutines {
  Function *const func_;
  IRContext *const ctx_;
  std::unordered_set<BasicBlock *> previouslyChecked_{};

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
  void run();

private:
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
};

void IdentifySimpleRoutines::run() {
  // Instead of scanning every instruction to see whether it is JSR, we scan
  // every basic block to see whether all of its predecessors are JSR.
  for (auto &bb : func_->basicBlocks()) {
    // 0: not only JSR, -1: nothing, 1: only JSR
    int onlyJSR = -1;
    for (Instruction &iRef : predecessorInsts(bb)) {
      Instruction *inst = &iRef;
      assert(inst->isTerminator());
      if (inst->getKind() != ValueKind::JSR || inst->getOperand(0) != &bb) {
        onlyJSR = 0;
        break;
      }
      onlyJSR = 1;
    }
    if (onlyJSR <= 0)
      continue;

    if (ctx_->getVerbosity() > 1)
      fprintf(stderr, "$%04x: potential simple routine\n", bb.getAddress().value_or(0));
    scanCandidate(&bb);
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

  if (ctx_->getVerbosity() > 0)
    fprintf(stderr, "%zu simple routines identified\n", candidates_.size());
  splitRoutines();
}

void IdentifySimpleRoutines::scanCandidate(BasicBlock *entry) {
  assert(
      !previouslyChecked_.count(entry) &&
      "A routine entry can't have been checked if all preds are JSR");
  std::unordered_set<BasicBlock *> visited{};
  std::unordered_set<BasicBlock *> jsrTargets{};
  std::unordered_set<Instruction *> rts{};
  std::deque<BasicBlock *> workList{};

  workList.push_back(entry);
  do {
    BasicBlock *bb = workList.front();
    workList.pop_front();
    // Already visited?
    if (!visited.insert(bb).second)
      continue;

    if (!previouslyChecked_.insert(bb).second) {
      if (ctx_->getVerbosity() > 1)
        fprintf(stderr, "fail: bb $%04x previously checked\n", bb->getAddress().value_or(0));
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

  candidates_.try_emplace(entry, std::move(visited), std::move(jsrTargets), std::move(rts));

  if (ctx_->getVerbosity() > 1)
    fprintf(stderr, "$%04x: created candidate\n", entry->getAddress().value_or(0));
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
  Module *mod = func_->getModule();
  IRBuilder builder(ctx_);
  std::vector<Instruction *> jsrs{};

  for (auto &[entry, cand] : candidates_) {
    cand.func = mod->createFunction();

    // Replace all RTS with Return.
    for (auto *inst : cand.rts) {
      builder.setInsertionPointAfter(inst);
      builder.createReturn();
      inst->eraseFromBasicBlock();
    }
    cand.rts.clear();

    // Replace all JSRs with Call and Jmp.
    for (auto &iRef : predecessorInsts(*entry)) {
      Instruction *inst = &iRef;
      if (inst->getKind() == ValueKind::JSR || inst->getOperand(0) == entry)
        jsrs.push_back(inst);
    }

    for (auto *inst : jsrs) {
      builder.setInsertionPointAfter(inst);
      builder.createCall(cand.func);
      builder.createJmp(inst->getOperand(1));
      inst->eraseFromBasicBlock();
    }
    jsrs.clear();

    assert(!entry->hasUsers() && "All users of the entry block should have been eliminated");

    cand.func->importBasicBlock(entry);
    for (auto *bb : cand.blocks)
      if (bb != entry)
        cand.func->importBasicBlock(bb);
  }
}

} // namespace

/// "Simple leaf" routines means:
/// - Leaf routines
/// - Single entry
/// - Always entered via a JSR instruction
/// - Contain no indirect jumps
/// - Contain no stack pointer manipulation
void identifySimpleRoutines(Module *mod) {
  for (auto &func : mod->functions()) {
    IdentifySimpleRoutines(&func).run();
    // Only run it on the global function.
    break;
  }
}