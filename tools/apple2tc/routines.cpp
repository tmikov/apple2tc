/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Dominators.h"
#include "PubIR.h"
#include "ir/IR.h"
#include "ir/IRUtil.h"

#include "apple2tc/SetVector.h"
#include "apple2tc/support.h"

#include <algorithm>
#include <deque>
#include <map>

using namespace ir;

namespace {

class IdentifySimpleRoutines {
  Function *const func_;
  IRContext *const ctx_;

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
  std::multimap<BasicBlock *, Candidate *> blocks_{};
  /// Entry blocks that were considered and rejected, with the reason. A multimap
  /// because distinct blocks can share a source address, and each rejection is
  /// worth reporting.
  std::multimap<uint32_t, std::string> rejected_{};
  FILE *const report_;

public:
  explicit IdentifySimpleRoutines(Function *func, FILE *report)
      : func_(func), ctx_(func->getModule()->getContext()), report_(report) {}
  bool run();

private:
  /// Record why a candidate was rejected, and log it at high verbosity.
  void reject(BasicBlock *entry, const std::string &reason);

  /// Write a human-readable analysis of every routine candidate — accepted
  /// and rejected — to `report_`, if non-null.
  void emitReport();

  /// Collect the basic block of the candidate.
  void scanCandidate(BasicBlock *entry);
  /// Remove candidates that JSR into a non-candidate. Return true if anything
  /// changed.
  bool removeInvalidJSRs();

  /// Actually split the candidates into new IR routines.
  void splitRoutines();

  void convertInvocations(BasicBlock *entry, Candidate &cand);

  BasicBlock *makeCallBlock(
      IRBuilder &builder,
      const Candidate &cand,
      Instruction *ctxInst,
      const PrimitiveSetVector<BasicBlock *> &dynamicReturnBlocks,
      BasicBlock *standsInFor = nullptr);

  bool convertRoutineInvocation(
      IRBuilder &builder,
      BasicBlock *entry,
      const Candidate &cand,
      Instruction *inst,
      const PrimitiveSetVector<BasicBlock *> &dynamicReturnBlocks,
      BasicBlock **fallCallBlock);

  static void extractRoutine(IRBuilder &builder, BasicBlock *entry, Candidate &cand);
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
      fprintf(stderr, "%zu candidates remaining after removing invalid JSRs\n", candidates_.size());
  } while (changed);

  auto numIdentified = candidates_.size();
  if (ctx_->getVerbosity() > 0)
    fprintf(stderr, "%zu simple routines identified\n", numIdentified);
  // Only the productive passes are worth reporting. apple2tc calls this in a
  // loop until it stops finding routines, and the final no-op pass would
  // otherwise append a redundant, confusingly-worded duplicate section.
  if (numIdentified != 0)
    emitReport();
  splitRoutines();
  return numIdentified != 0;
}

void IdentifySimpleRoutines::emitReport() {
  if (!report_)
    return;

  // Accepted candidates, sorted by address for stable output.
  std::vector<std::pair<uint32_t, BasicBlock *>> accepted{};
  accepted.reserve(candidates_.size());
  for (auto &p : candidates_)
    accepted.emplace_back(p.first->getAddress().value_or(0x10000), p.first);
  std::sort(accepted.begin(), accepted.end());

  fprintf(report_, "=== routine candidates: %zu accepted, %zu rejected ===\n",
          accepted.size(), rejected_.size());
  // This pass runs at -O2, before simplifyCFG (-O3), so the CFG reported here is
  // more granular than the one behind the final --irc1 output. A single source
  // address can appear as several blocks -- e.g. ADC splits into binary-mode and
  // decimal-mode blocks that share one address.
  fprintf(
      report_,
      "Blocks reflect the CFG before simplifyCFG; they are more granular than\n"
      "--irc1 output, and one address may appear as several blocks.\n\n");

  for (auto [addr, entry] : accepted) {
    Candidate &cand = candidates_.at(entry);
    fprintf(report_, "ACCEPT $%04X  %zu blocks\n", addr, cand.blocks.size());

    // Call sites.
    fprintf(report_, "  called from:");
    std::vector<uint32_t> callers{};
    for (Instruction &iRef : predecessorInsts(*entry)) {
      if (iRef.getKind() == ValueKind::JSR && iRef.getOperand(0) == entry)
        callers.push_back(iRef.getAddress().value_or(0x10000));
    }
    std::sort(callers.begin(), callers.end());
    for (uint32_t c : callers)
      fprintf(report_, " $%04X", c);
    fprintf(report_, "  (%zu sites)\n", callers.size());

    // Blocks in reverse postorder, with immediate dominators.
    DomTree dt = computeDomTree(entry, cand.blocks);
    fprintf(report_, "  blocks (rpo, idom):");
    for (BasicBlock *bb : dt.rpo) {
      auto it = dt.idom.find(bb);
      fprintf(
          report_,
          " $%04X<-$%04X",
          bb->getAddress().value_or(0xFFFF),
          it == dt.idom.end() ? 0xFFFF : it->second->getAddress().value_or(0xFFFF));
    }
    fprintf(report_, "\n");

    // Natural loops.
    std::vector<NaturalLoop> loops = findNaturalLoops(dt);
    if (loops.empty()) {
      fprintf(report_, "  loops: none\n");
    } else {
      for (const NaturalLoop &loop : loops) {
        std::vector<uint32_t> body{};
        for (BasicBlock *bb : loop.body)
          body.push_back(bb->getAddress().value_or(0xFFFF));
        std::sort(body.begin(), body.end());
        fprintf(report_, "  loop header $%04X body:", loop.header->getAddress().value_or(0xFFFF));
        for (uint32_t a : body)
          fprintf(report_, " $%04X", a);
        fprintf(report_, "\n");
      }
    }
    fprintf(report_, "\n");
  }

  for (auto &[addr, reason] : rejected_)
    fprintf(report_, "REJECT $%04X  %s\n", addr, reason.c_str());
  fprintf(report_, "\n");
}

void IdentifySimpleRoutines::reject(BasicBlock *entry, const std::string &reason) {
  rejected_.emplace(entry->getAddress().value_or(0x10000), reason);
  if (ctx_->getVerbosity() > 1)
    fprintf(stderr, "fail: %s\n", reason.c_str());
}

void IdentifySimpleRoutines::scanCandidate(BasicBlock *entry) {
  // Stack depth relative to routine entry, at the start of each visited block.
  std::unordered_map<BasicBlock *, int> depthAt{};
  std::unordered_set<BasicBlock *> jsrTargets{};
  std::unordered_set<Instruction *> rts{};
  std::deque<std::pair<BasicBlock *, int>> workList{};

  if (ctx_->getVerbosity() > 1)
    fprintf(stderr, "$%04x: scan candidate\n", entry->getAddress().value_or(0x10000));

  workList.emplace_back(entry, 0);
  do {
    auto [bb, entryDepth] = workList.front();
    workList.pop_front();

    // Already visited? Every path here must agree on the stack depth.
    auto [it, inserted] = depthAt.try_emplace(bb, entryDepth);
    if (!inserted) {
      if (it->second != entryDepth) {
        reject(
            entry,
            format(
                "block $%04x reached at stack level %d and %d",
                bb->getAddress().value_or(0),
                it->second,
                entryDepth));
        return;
      }
      continue;
    }

    // Indirect branches except RTS are not allowed.
    auto *terminator = bb->getTerminator();
    if (terminator->getKind() != ValueKind::RTS && terminator->isIndirectBranch()) {
      reject(entry, format("terminator %s", getValueKindName(terminator->getKind())));
      return;
    }

    // Check for stack operations. RTS and JSR are allowed. push8/pop8 adjust the
    // depth, which is tracked across the whole routine rather than per block.
    // Others are not allowed.
    int depth = entryDepth;
    for (auto &iRef : bb->instructions()) {
      if (iRef.getKind() == ValueKind::JSR) {
        jsrTargets.insert(cast<BasicBlock>(iRef.getOperand(0)));
      } else if (iRef.getKind() == ValueKind::RTS) {
        rts.insert(&iRef);
      } else if (iRef.getKind() == ValueKind::Push8) {
        ++depth;
      } else if (iRef.getKind() == ValueKind::Pop8) {
        --depth;
        if (depth < 0) {
          // Popping below routine entry means the routine is manipulating its
          // own return address.
          reject(
              entry,
              format(
                  "%s block $%04x stack level underflow",
                  getValueKindName(iRef.getKind()),
                  bb->getAddress().value_or(0)));
          return;
        }
      } else if (iRef.modifiesSP()) {
        reject(entry, format("%s", getValueKindName(iRef.getKind())));
        return;
      }
    }

    if (terminator->getKind() == ValueKind::RTS) {
      // The return address must be on top of the stack when we return.
      if (depth != 0) {
        reject(
            entry,
            format(
                "block $%04x stack level %d at RTS",
                bb->getAddress().value_or(0),
                depth));
        return;
      }
    } else if (terminator->getKind() == ValueKind::JSR) {
      // Optimistically continue in the "fall" block. A JSR/RTS pair is
      // depth-neutral from this routine's point of view.
      workList.emplace_back(cast<BasicBlock>(terminator->getOperand(1)), depth);
    } else {
      for (auto &succ : successors(*bb))
        workList.emplace_back(&succ, depth);
    }
  } while (!workList.empty());

  std::unordered_set<BasicBlock *> visited{};
  visited.reserve(depthAt.size());
  for (auto &p : depthAt)
    visited.insert(p.first);

  // Is this entry the "fall" block of a JSR, i.e. the address a call returns
  // to? Then the RTS edges arriving here are the return leg of that very call,
  // and carry no control flow that the JSR edge below does not already carry.
  //
  // The shape is a caller that does JSR <helper> and then falls straight into
  // a block which is itself a subroutine other callers JSR directly. Snake
  // Byte's $60E4 does exactly that: it calls $6127 and falls into $60E7, while
  // $6148 and $615A call $60E7 on its own. Rejecting the return leg cost
  // $60E7 and, through the invalid-JSR cascade, five more routines.
  // The JSR must live outside the candidate, matching the `fallFromAnother`
  // condition in convertRoutineInvocation(): that is the case which builds the
  // block the RTS edges are later redirected to.
  bool isReturnPoint = false;
  for (Instruction &iRef : predecessorInsts(*entry)) {
    if (iRef.getKind() == ValueKind::JSR && iRef.getOperand(1) == entry &&
        !visited.count(iRef.getBasicBlock())) {
      isReturnPoint = true;
      break;
    }
  }

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
    case ValueKind::RTS:
      // Only when something really does return here; an RTS edge into an entry
      // that is not a return point is a computed jump and stays rejected.
      if (isReturnPoint) {
        jmp = true;
        continue;
      }
      break;
    default:;
    }
    // We found a predecessor that is neither of these.
    reject(
        entry,
        format(
            "invalid predecessor inst %s at $%04x",
            getValueKindName(inst->getKind()),
            inst->getAddress().value_or(0x10000)));
    return;
  }

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
        rejected_.emplace(
            cur->first->getAddress().value_or(0x10000),
            format("invalid JSR to $%04x", jsrTarget->getAddress().value_or(0x10000)));
        candidates_.erase(cur);
        changed = true;
        break;
      }
    }
  }
  return changed;
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

  // Create a map of all blocks and which routine they belong to.
  for (auto [_, pCand] : sortedCandidates)
    for (auto *bb : pCand->blocks)
      blocks_.emplace(bb, pCand);

  for (auto [entry, pCand] : sortedCandidates)
    convertInvocations(entry, *pCand);

  IRBuilder builder(ctx_);
  for (auto [entry, pCand] : sortedCandidates)
    extractRoutine(builder, entry, *pCand);
}

void IdentifySimpleRoutines::convertInvocations(BasicBlock *entry, Candidate &cand) {
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

    // Set when the entry turns out to be the "fall" block of a JSR from outside
    // the routine, which is the only way an RTS predecessor is accepted.
    BasicBlock *fallCallBlock = nullptr;
    for (auto *inst : preds)
      if (convertRoutineInvocation(builder, entry, cand, inst, dynamicReturnBlocks, &fallCallBlock))
        inst->eraseFromBasicBlock();

    // Whatever still returns to the entry must return to a block that calls the
    // extracted routine instead, so that the entry is left with no users
    // outside the routine.
    //
    // Recomputed live rather than reusing `preds`: the loop above erases the
    // predecessors it converts, so that snapshot now holds dangling pointers.
    PrimitiveSetVector<Instruction *> rtsPreds{};
    for (auto &iRef : predecessorInsts(*entry))
      if (iRef.getKind() == ValueKind::RTS)
        rtsPreds.insert(&iRef);

    if (!rtsPreds.empty()) {
      // Usually the JSR that falls into the entry has already built the block.
      // Not always: scanCandidate() runs over the whole function before any
      // routine is split out, so by the time this one is extracted an earlier
      // extraction may have rewritten that JSR away. Build it here in that case
      // rather than depending on an ordering that does not hold.
      if (!fallCallBlock)
        fallCallBlock = makeCallBlock(builder, cand, *rtsPreds.begin(), dynamicReturnBlocks, entry);
      for (auto *inst : rtsPreds)
        for (unsigned i = 1, count = inst->getNumOperands(); i != count; ++i)
          if (inst->getOperand(i) == entry)
            inst->setOperand(i, fallCallBlock);
    }
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

void IdentifySimpleRoutines::extractRoutine(
    IRBuilder &builder,
    BasicBlock *entry,
    Candidate &cand) {
  // Map from old block to new block.
  std::unordered_map<Value *, Value *> valueMap{};
  auto order = dfsOrder(entry);

  assert(order[0] == entry);
  for (auto *oldBB : order) {
    auto *newBB = cand.func->createBasicBlock();
    newBB->setAddress(oldBB->getAddress(), oldBB->isRealAddress());
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

/// Build a block that calls \p cand's routine and then returns to whichever of
/// \p dynamicReturnBlocks is live, and register it with every candidate that
/// owned \p ctxInst's block. Used wherever control reaches the routine by some
/// means other than a plain JSR -- a jump into it, or a return into it.
///
/// \p standsInFor, when given, is an entry block whose address the new block
/// takes over. That is needed when control still arrives here *by address*
/// after the entry has moved into the extracted routine: a JSR falling through
/// to it needs the address to compute what it pushes, and a return to it needs
/// the address in the generated address-to-block map. The entry itself leaves
/// this function, so the address does not end up duplicated.
BasicBlock *IdentifySimpleRoutines::makeCallBlock(
    IRBuilder &builder,
    const Candidate &cand,
    Instruction *ctxInst,
    const PrimitiveSetVector<BasicBlock *> &dynamicReturnBlocks,
    BasicBlock *standsInFor) {
  BasicBlock *callBlock = ctxInst->getBasicBlock()->getFunction()->createBasicBlock();
  if (standsInFor && standsInFor->getAddress())
    callBlock->setAddress(standsInFor->getAddress(), standsInFor->isRealAddress());

  // Record that the new block belongs to all routines that owned the original
  // block.
  auto range = blocks_.equal_range(ctxInst->getBasicBlock());
  for (auto it = range.first; it != range.second; ++it) {
    Candidate *bCand = it->second;
    bCand->blocks.insert(callBlock);
    blocks_.emplace(callBlock, bCand);
  }

  builder.setInsertionBlock(callBlock);
  builder.setAddress(ctxInst->getAddress());
  builder.createCall(cand.func, builder.getLiteralU16(0));
  auto *rts = builder.createRTS(builder.getLiteralU8(0));
  for (auto *bb : dynamicReturnBlocks)
    rts->pushOperand(bb);
  return callBlock;
}

bool IdentifySimpleRoutines::convertRoutineInvocation(
    IRBuilder &builder,
    BasicBlock *entry,
    const Candidate &cand,
    Instruction *inst,
    const PrimitiveSetVector<BasicBlock *> &dynamicReturnBlocks,
    BasicBlock **fallCallBlock) {
  // Some cases set this to the IR instruction that must get all dynamic return blocks
  // as successors.
  Instruction *dynamicBranch = nullptr;

  auto createCallBlock = [this, &builder, &cand, inst, &dynamicReturnBlocks](
                             BasicBlock *standsInFor = nullptr) {
    return makeCallBlock(builder, cand, inst, dynamicReturnBlocks, standsInFor);
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

      BasicBlock *callBlock = createCallBlock(entry);
      // Anything that *returns* to the entry is really returning to this new
      // block now. convertInvocations() redirects those RTS edges once every
      // predecessor has been visited, since they may be visited in any order.
      *fallCallBlock = callBlock;

      builder.setInsertionPointAfter(inst);
      builder.setAddress(inst->getAddress());
      builder.createJSR(inst->getOperand(0), callBlock);
    }
    break;
  }

  case ValueKind::RTS:
    // A call that returns to the entry. The JSR that made that call is a
    // predecessor too, and building the block this should retarget to is its
    // job; see the *fallCallBlock assignment above. Leave the instruction in
    // place -- only its operand changes, and convertInvocations() does that.
    return false;

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

} // namespace

/// "Simple leaf" routines means:
/// - Leaf routines
/// - Single entry
/// - Always entered via a JSR instruction
/// - Contain no indirect jumps
/// - Contain no stack pointer manipulation
bool identifySimpleRoutines(Module *mod, FILE *report) {
  for (auto &func : mod->functions()) {
    // Only run it on the global function.
    return IdentifySimpleRoutines(&func, report).run();
  }
  return false;
}