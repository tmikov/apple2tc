/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "CPURegLiveness.h"

#include "Analysis.h"
#include "PubIR.h"
#include "ir/IRUtil.h"

#include "apple2tc/support.h"

#include <deque>

/// Return reverse post order of the inverse call graph.
/// Callees appear topologically sorted before callers.
static std::vector<Function *> calleeFirstOrder(Module *mod, const CallGraph &cg) {
  auto order = genPostOrder<Function>(
      VisitUnreachable::All, mod->functions(), nullptr, InverseCallGraphTraits(cg));
  std::reverse(order.begin(), order.end());
  return order;
}

/// Return reverse post order of the call graph.
/// Callers appear topologically sorted before callees.
static std::vector<Function *> callerFirstOrder(Module *mod, const CallGraph &cg) {
  auto order = genPostOrder(
      VisitUnreachable::All, mod->functions(), mod->getStartFunction(), CallGraphTraits(cg));
  std::reverse(order.begin(), order.end());
  return order;
}

static std::string funcName(const Function *func) {
  return func->debuggingName();
};

CPURegLiveness::CPURegLiveness(Module *mod) : mod_(mod), cg_(mod) {}
CPURegLiveness::~CPURegLiveness() = default;

void CPURegLiveness::run() {
  if (mod_->getContext()->getVerbosity() >= 2) {
    fprintf(stderr, "Call graph:\n");
    for (auto &func : mod_->functions()) {
      fprintf(stderr, "  %s:", funcName(&func).c_str());
      unsigned index = 0;
      for (auto *callee : cg_.funcNode(&func).succs)
        fprintf(stderr, "%s%s", index++ == 0 ? " " : ", ", funcName(callee).c_str());
      fprintf(stderr, "\n");
    }
  }

  // First, we calculate the Gen, Kill and Modified sets of every function.
  calcGenKillMod();

  // Calculate global liveness.
  calcLiveness();
}

void CPURegLiveness::calcLiveness() {
  auto order = callerFirstOrder(mod_, cg_);
  if (mod_->getContext()->getVerbosity() >= 2) {
    fprintf(stderr, "Reverse post-order of the call graph:\n");
    for (auto *func : order)
      fprintf(stderr, "  %s\n", funcName(func).c_str());
  }

  // First pass assuming the worst case for every function LiveOut.
  for (auto *func : order) {
    // We start out by assuming everything is live on function return, since
    // we don't know any better.
    auto &fData = funcData_[func];
    fData.sets.liveOut = fData.modified | fData.sets.gen;
    bbSets_[func->getExitBlock()].liveOut = fData.sets.liveOut;
    calcFuncLiveIn(func);
  }

  unsigned funcCount = 0;
  unsigned iter = 0;
  bool changed;
  do {
    changed = false;
    ++iter;

    // Update every function's LiveOut based on the LiveOut of all calls to it.
    for (auto *func : order) {
      // No need to bother with functions without known callers.
      if (cg_.funcNode(func).preds.empty())
        continue;

      SetType liveOut = 0;
      for (auto &user : func->users()) {
        Instruction *inst = user.owner();
        if (inst->getKind() != ValueKind::Call)
          continue;

        liveOut |= deriveInstLiveOut(inst);
      }

      auto &fData = funcData_[func];
      liveOut &= fData.sets.liveOut;
      if (fData.sets.liveOut == liveOut)
        continue;
      fData.sets.liveOut = liveOut;
      bbSets_[func->getExitBlock()].liveOut = liveOut;

      ++funcCount;
      resetBlocks(func);
      changed |= calcFuncLiveIn(func);
    }
  } while (changed);

  if (mod_->getContext()->getVerbosity() >= 1)
    fprintf(stderr, "%u function liveness recalced, %u iterations\n", funcCount, iter);
}

void CPURegLiveness::resetBlocks(Function *func) {
  for (auto &bb : func->basicBlocks())
    resetBB(&bb);
}

bool CPURegLiveness::calcFuncLiveIn(Function *func) {
  bool changed = livenessInverseRPO(func);
  funcData_[func].sets.liveIn = bbSets(func->getEntryBlock()).liveIn;
  return changed;
}

void CPURegLiveness::calcGenKillMod() {
  auto order = calleeFirstOrder(mod_, cg_);
  if (mod_->getContext()->getVerbosity() >= 2) {
    fprintf(stderr, "Reverse post-order of the inverse call graph:\n");
    for (auto *func : order)
      fprintf(stderr, "  %s\n", funcName(func).c_str());
  }

  for (auto *func : order) {
    FuncData &fdata = funcData_[func];
    for (auto &rBB : func->basicBlocks()) {
      auto *bb = &rBB;
      initBB(bb);
      fdata.modified |= bbSets_[bb].kill;
    }
    calcGenKill(func);
  }

  unsigned funcCount = 0;
  unsigned iter = 0;
  bool changed;
  do {
    changed = false;
    ++iter;
    for (auto *func : order) {
      // Only process functions that call other functions because only they
      // can change.
      if (cg_.funcNode(func).succs.empty())
        continue;
      ++funcCount;
      for (auto &rBB : func->basicBlocks())
        initBB(&rBB);
      changed |= calcGenKill(func);
    }
  } while (changed);

  if (mod_->getContext()->getVerbosity() >= 1)
    fprintf(stderr, "%u gen/kill functions recalced, %u iterations\n", funcCount, iter);
}

bool CPURegLiveness::calcGenKill(Function *func) {
  // For a forward data flow problem we use RPO of the CFG. It is not reversed
  // here, but we iterate it in reverse.
  auto order = genPostOrder<BasicBlock>(
      func->getDecompileLevel() < Function::DecompileLevel::Normal ? VisitUnreachable::All
                                                                   : VisitUnreachable::No,
      func->basicBlocks(),
      func->getEntryBlock());

  unsigned bbCount = 0;
  unsigned iter = 0;
  bool changed;
  do {
    changed = false;
    ++iter;
    for (auto *bb : makeReverseRange(order)) {
      ++bbCount;
      changed |= updateBlockGenKillOut(bb);
    }
  } while (changed);

  if (mod_->getContext()->getVerbosity() >= 2)
    fprintf(stderr, "%u gen/kill calculations, %u iterations\n", bbCount, iter);

  auto &fdata = funcData_[func];
  const auto &bbData = bbSets(func->getExitBlock());
  changed = (fdata.sets.gen ^ bbData.genOut) | (fdata.sets.kill ^ bbData.killOut);
  fdata.sets.gen = bbData.genOut;
  fdata.sets.kill = bbData.killOut;
  return changed;
}

inline auto CPURegLiveness::instGenKill(Instruction *inst) const -> std::pair<SetType, SetType> {
  if (inst->getKind() == ValueKind::LoadR8)
    return {m(inst->getOperand(0)), 0};
  else if (inst->getKind() == ValueKind::StoreR8)
    return {0, m(inst->getOperand(0))};
  else if (inst->getKind() == ValueKind::Call) {
    const auto &fData = getFuncData(cast<Function>(inst->getOperand(0)));
    return {fData.sets.gen, fData.sets.kill};
  } else {
    return {0, 0};
  }
}

void CPURegLiveness::initBB(BasicBlock *bb) {
  LivenessSets bbs{};

  for (auto &inst : bb->instructions()) {
    auto [gen, kill] = instGenKill(&inst);
    bbs.gen |= ~bbs.kill & gen;
    bbs.kill |= kill;
  }
  auto *term = bb->getTerminator();
  assert(term && "Every basic block must have a terminator");
  if (term->isIndirectBranch())
    bbs.liveOut = LivenessSets::kAll;
  else if (term->getKind() == ValueKind::Exit)
    bbs.liveOut = getFuncData(bb->getFunction()).sets.liveOut;

  bbSets_[bb] = bbs;
}

void CPURegLiveness::resetBB(BasicBlock *bb) {
  auto &bbs = bbSets_[bb];
  bbs.liveIn = LivenessSets::kInvalidSet;

  auto *term = bb->getTerminator();
  if (term->isIndirectBranch())
    bbs.liveOut = LivenessSets::kAll;
  else if (term->getKind() == ValueKind::Exit)
    bbs.liveOut = getFuncData(bb->getFunction()).sets.liveOut;
  else
    bbs.liveOut = 0;
}

bool CPURegLiveness::updateBlockLiveInOut(BasicBlock *bb) {
  auto &bbs = bbSets_[bb];
  for (auto &succ : successors(*bb))
    bbs.liveOut |= bbSets_[&succ].liveIn;
  SetType in = bbs.gen | (bbs.liveOut & ~bbs.kill);
  bool changed = bbs.liveIn ^ in;
  bbs.liveIn = in;
  return changed;
}

CPURegLiveness::SetType CPURegLiveness::deriveInstLiveOut(Instruction *inst) {
  BasicBlock *bb = inst->getBasicBlock();
  SetType liveOut = bbSets(bb).liveOut;

  for (auto &iRef : bb->reverse_instructions()) {
    Instruction *curInst = &iRef;
    if (curInst == inst)
      break;

    auto [gen, kill] = instGenKill(curInst);
    liveOut = gen | (liveOut & ~kill);
  }

  return liveOut;
}

bool CPURegLiveness::updateBlockGenKillOut(BasicBlock *bb) {
  auto &bbs = bbSets_[bb];

  auto &&preds = predecessors(*bb);
  SetType genIn = 0;
  SetType killIn = preds.empty() ? 0 : LivenessSets::kAll;

  for (auto &pred : preds) {
    auto &predSets = bbSets_[&pred];
    genIn |= predSets.genOut;
    killIn &= predSets.killOut;
  }

  SetType genOut = genIn | bbs.gen & ~killIn;
  SetType killOut = killIn | bbs.kill;
  bool changed = (genOut ^ bbs.genOut) | (killOut ^ bbs.killOut);
  bbs.genOut = genOut;
  bbs.killOut = killOut;
  return changed;
}

bool CPURegLiveness::livenessInverseRPO(Function *func) {
  // Generate the block visit order for backwards data flow. It is not reversed
  // here, but we will iterate it in reverse.
  auto order = genPostOrder(
      func->getDecompileLevel() < Function::DecompileLevel::Normal ? VisitUnreachable::All
                                                                   : VisitUnreachable::No,
      func->basicBlocks(),
      func->getExitBlock(),
      InverseGraphTraits());

  unsigned bbCount = 0;
  unsigned iter = 0;
  bool changed;
  do {
    changed = false;
    ++iter;
    for (auto *bb : makeReverseRange(order)) {
      ++bbCount;
      changed |= updateBlockLiveInOut(bb);
    }
  } while (changed);

  if (mod_->getContext()->getVerbosity() >= 2)
    fprintf(stderr, "%u liveness calculations, %u iterations\n", bbCount, iter);

  return iter > 1;
}

void CPURegLiveness::livenessWorkList(Function *func) {
  unsigned blockCount = 0;
  for (auto &rbb : func->basicBlocks()) {
    ++blockCount;
    initBB(&rbb);
  }

  auto exitBlocks = genEntryBlocks<BasicBlock>(
      func->getDecompileLevel() < Function::DecompileLevel::Normal ? VisitUnreachable::All
                                                                   : VisitUnreachable::No,
      func->basicBlocks(),
      nullptr,
      InverseGraphTraits());

  std::unordered_set<BasicBlock *> inList{};
  std::deque<BasicBlock *> workList{};

  auto addToList = [&inList, &workList](BasicBlock *bb) {
    if (inList.insert(bb).second)
      workList.push_back(bb);
  };
  auto popFromList = [&inList, &workList]() -> BasicBlock * {
    auto *bb = workList.front();
    workList.pop_front();
    bool erased = inList.erase(bb);
    (void)erased;
    assert(erased && "The popped element must be marked as in-list");
    return bb;
  };

  unsigned numCalc = 0;
  for (auto *bb : exitBlocks)
    addToList(bb);

  while (!workList.empty()) {
    auto *bb = popFromList();
    ++numCalc;
    if (updateBlockLiveInOut(bb)) {
      for (auto &pred : predecessors(*bb))
        addToList(&pred);
    }
  }

  fprintf(stderr, "%u blocks, %u liveness calculations\n", blockCount, numCalc);
}

std::shared_ptr<CPURegLiveness> liveness(ir::Module *mod) {
  return CPURegLiveness::calc(mod);
}

void dumpModule(ir::Module *mod, bool irTrees, CPURegLiveness *liveness) {
  if (liveness) {
    CPULivenessDumper(stdout, irTrees, *liveness, CPULivenessDumper::Flags{.dumpFuncs = true})
        .dump(mod);
  } else {
    ir::IRDumper(stdout, irTrees).dump(mod);
  }
}
