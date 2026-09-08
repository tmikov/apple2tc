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

#include <algorithm>
#include <deque>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/// Return reverse post order of the inverse call graph.
/// Callees appear topologically sorted before callers -- but only where the
/// call graph is acyclic. Mutually recursive routines have no such order, so
/// this is a useful visit order, not an invariant anything may rely on.
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
  return format("func '%s' $%04x", func->getName().c_str(), func->getAddress().value_or(0x10000));
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
    // External functions have no blocks; their sets were fixed in calcGenKillMod().
    if (func->isExternal())
      continue;
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
      // No need to bother with external functions, or with functions without
      // known callers.
      if (func->isExternal() || cg_.funcNode(func).preds.empty())
        continue;

      SetType liveOut = 0;
      for (auto &user : func->users()) {
        Instruction *inst = user.owner();
        if (!inst->isCall())
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

  // Every function needs its FuncData before any initBB() runs, because a Call
  // looks up its callee's sets. `order` puts callees first, but only a DAG can
  // be topologically sorted, and the call graph need not be one: routines
  // extracted from a tangle of shared ROM entry points call each other in
  // cycles. Seeding up front makes the lookup independent of the order.
  for (auto *func : order) {
    FuncData &fdata = funcData_[func];
    if (func->isExternal()) {
      // We know nothing about an external routine, so assume it reads and
      // modifies every register, and kills none of them.
      fdata.sets.gen = LivenessSets::kAll;
      fdata.sets.kill = 0;
      fdata.modified = LivenessSets::kAll;
    }
  }

  for (auto *func : order) {
    if (func->isExternal())
      continue;
    for (auto &rBB : func->basicBlocks())
      initBB(&rBB);
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
      // can change. External functions never change.
      if (func->isExternal() || cg_.funcNode(func).succs.empty())
        continue;
      ++funcCount;
      for (auto &rBB : func->basicBlocks())
        initBB(&rBB);
      changed |= calcGenKill(func);
    }
  } while (changed);

  // Only now are the block kill sets final. Along a cycle the first pass sees a
  // callee that has not been computed yet and contributes an empty kill, so
  // accumulating `modified` there would under-approximate it -- and since
  // calcLiveness() seeds liveOut from `modified` and then only ever intersects
  // it, too small a value here silently drops registers that are still live.
  for (auto *func : order) {
    if (func->isExternal())
      continue;
    FuncData &fdata = funcData_[func];
    fdata.modified = 0;
    for (auto &rBB : func->basicBlocks())
      fdata.modified |= bbSets_[&rBB].kill;
  }

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
  else if (inst->isCall()) {
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

//===----------------------------------------------------------------------===//
// --dump-reg-liveness
//===----------------------------------------------------------------------===//

namespace {

/// The letter printed for each CPURegKind, in enum order. A register set is
/// rendered as one character per register -- this letter when the register is
/// in the set, '.' when it is not -- so that columns line up and a mask can be
/// grepped for.
const char kRegLetters[] = "AXYNVBDIZC";
static_assert(
    sizeof(kRegLetters) - 1 == (unsigned)CPURegKind::_last,
    "every CPURegKind needs a letter in the printed mask");

/// The subset of kRegLetters that are status flags, i.e. everything but A/X/Y.
constexpr CPURegLiveness::SetType kFlagMask =
    CPURegLiveness::LivenessSets::kAll & ~(CPURegLiveness::SetType)0x7;

std::string regMask(CPURegLiveness::SetType set) {
  std::string res((size_t)CPURegKind::_last, '.');
  for (unsigned i = 0; i != (unsigned)CPURegKind::_last; ++i)
    if (set & (1u << i))
      res[i] = kRegLetters[i];
  return res;
}

/// Everything in a function that makes the reported liveness coarser than the
/// 6502 code itself would suggest. Each is a barrier to narrowing a call
/// boundary down to individual registers, and each is printed as a letter in a
/// fixed-width mask.
struct Barriers {
  /// 'P': an EncodeFlags instruction -- PHP or BRK. It reads C, Z, I, D, V and
  /// N in one go, so all six become live at once.
  bool encodeFlags = false;
  /// 'L': PLP or RTI, which write the same six at once. Both lower to
  /// DecodeFlag over the byte a Pop8 produced -- and DecodeFlag alone is not
  /// the marker, because decimal-mode ADC and SBC use it too, to pull N, V, Z
  /// and C out of the opaque result of AdcDec/SbcDec. Those are ordinary
  /// arithmetic and no barrier at all; in Bolo they outnumber the real thing
  /// by an order of magnitude. The Pop8 operand is what tells them apart.
  bool decodeFlag = false;
  /// 'J': JmpInd or JSRInd. initBB() forces such a block's liveOut to kAll,
  /// which floods backwards through the function's liveIn.
  bool indBranch = false;
  /// 'R': a raw RTS terminator. RTS is inside the IndirectBranch range, so it
  /// forces the same kAll liveOut. Only functions still at DecompileLevel::Low
  /// -- the ones routine identification could not extract -- have one.
  bool rawRTS = false;
  /// 'X': calls an external routine. calcGenKillMod() gives an external gen ==
  /// modified == kAll, since nothing is known about a hand-written body.
  bool callsExtern = false;
  /// 'U': no known callers. calcLiveness() narrows a function's liveOut by
  /// intersecting what every call site needs -- so with no call sites it skips
  /// the function entirely and liveOut stays at its seed, `modified | gen`.
  /// Such a row's live-out is an upper bound, not a measurement.
  bool uncalled = false;
  /// 'K': at least one call to this function sits in a block whose terminator
  /// is an IndirectBranch, which initBB() gives a liveOut of kAll. Such a call
  /// site cannot narrow the callee's live-out at all -- it reports that
  /// everything the callee touches is still needed. Unlike the others this is
  /// a property of the function's callers, not of its own body.
  bool blindCaller = false;

  std::string mask() const {
    std::string res = "-------";
    if (encodeFlags)
      res[0] = 'P';
    if (decodeFlag)
      res[1] = 'L';
    if (indBranch)
      res[2] = 'J';
    if (rawRTS)
      res[3] = 'R';
    if (callsExtern)
      res[4] = 'X';
    if (uncalled)
      res[5] = 'U';
    if (blindCaller)
      res[6] = 'K';
    return res;
  }
};

Barriers findBarriers(
    Function *func,
    const CallGraph &cg,
    const std::unordered_set<const Function *> &blindCallees) {
  Barriers b{};
  b.uncalled = cg.funcNode(func).preds.empty();
  b.blindCaller = blindCallees.count(func) != 0;
  for (auto &bb : func->basicBlocks()) {
    for (auto &inst : bb.instructions()) {
      switch (inst.getKind()) {
      case ValueKind::EncodeFlags:
        b.encodeFlags = true;
        break;
      case ValueKind::DecodeFlag: {
        auto *opaque = dyn_cast<Instruction>(inst.getOperand(0));
        if (opaque && opaque->getKind() == ValueKind::Pop8)
          b.decodeFlag = true;
        break;
      }
      case ValueKind::JmpInd:
      case ValueKind::JSRInd:
        b.indBranch = true;
        break;
      case ValueKind::RTS:
        b.rawRTS = true;
        break;
      default:
        break;
      }
      if (inst.isCall() && cast<Function>(inst.getOperand(0))->isExternal())
        b.callsExtern = true;
    }
  }
  return b;
}

/// True when the function has an address that means something in the original
/// binary. Anonymous functions -- the entry point and anything routine
/// identification split off without a real origin -- do not.
bool hasRealAddress(Function *func) {
  return func->getAddress().has_value() && func->isRealAddress();
}

const char *levelName(Function *func) {
  if (func->isExternal())
    return "ext";
  return func->getDecompileLevel() < Function::DecompileLevel::Normal ? "low" : "norm";
}

} // anonymous namespace

void dumpRegLiveness(ir::Module *mod, CPURegLiveness *lv, FILE *os) {
  fprintf(os, "# apple2tc --dump-reg-liveness\n");
  fprintf(os, "#\n");
  fprintf(os, "# One FUNC row per function, then a summary. Every row has the same fields, so\n");
  fprintf(os, "# `grep '^FUNC'` and awk on $2..$8 aggregate it; `sort -k3` groups by live-in.\n");
  fprintf(os, "#\n");
  fprintf(
      os, "# Register mask, always %u characters, in this order:\n", (unsigned)CPURegKind::_last);
  fprintf(os, "#   %s\n", kRegLetters);
  fprintf(os, "#   A X Y      accumulator and the two index registers\n");
  fprintf(os, "#   N V B D I  status N, V, B, D, I\n");
  fprintf(
      os, "#   Z          STATUS_NotZ, the zero flag (stored inverted; liveness is the same)\n");
  fprintf(os, "#   C          carry\n");
  fprintf(os, "# A letter means the register is in the set, '.' means it is not.\n");
  fprintf(os, "#\n");
  fprintf(os, "# B is not a real machine flag: it exists only in the byte PHP pushes, and\n");
  fprintf(os, "# emitEncodeFlags()/emitDecodeFlags() do not even include it. Anything reported\n");
  fprintf(os, "# live for B comes from code that stores it explicitly, and is an artifact.\n");
  fprintf(os, "#\n");
  fprintf(os, "# kall: whether `modified` is the conservative all-ones set, and what forced it.\n");
  fprintf(os, "#   -       modified is a real, computed set\n");
  fprintf(os, "#   extern  the function is external, so nothing is known about it\n");
  fprintf(os, "#   body    its own blocks genuinely kill every register\n");
  fprintf(os, "#\n");
  fprintf(os, "# barriers, always 7 characters, in this order:\n");
  fprintf(os, "#   PLJRXUK\n");
  fprintf(os, "#   P  EncodeFlags (PHP, BRK): reads C,Z,I,D,V,N in one instruction\n");
  fprintf(os, "#   L  PLP or RTI: writes C,Z,I,D,V,N in one instruction. Recognised as a\n");
  fprintf(os, "#      DecodeFlag over a Pop8, since decimal ADC/SBC also use DecodeFlag\n");
  fprintf(os, "#   J  JmpInd or JSRInd: the block's liveOut is forced to all-ones\n");
  fprintf(os, "#   R  a raw RTS terminator: also an IndirectBranch, same forcing\n");
  fprintf(os, "#   X  calls an external routine, whose gen and modified are all-ones\n");
  fprintf(os, "#   U  no known callers, so live-out was never narrowed below modified|gen\n");
  fprintf(os, "#   K  called from a block whose liveOut is all-ones (see J and R): that call\n");
  fprintf(os, "#      site could not narrow this function's live-out at all\n");
  fprintf(os, "#\n");
  fprintf(os, "# lvl: ext (no body), low (control flow not resolved), norm (a real routine).\n");
  fprintf(os, "#\n");
  fprintf(
      os,
      "# %-5s %-10s %-10s %-10s %-6s %-7s %-4s %s\n",
      "addr",
      "live-in",
      "live-out",
      "modified",
      "kall",
      "barrs",
      "lvl",
      "name");

  // A call inside a block whose terminator is an IndirectBranch sits under the
  // liveOut initBB() forced to kAll, so it tells its callee nothing. These are
  // the call sites that keep the whole analysis conservative, and they are
  // concentrated in whichever functions routine identification could not
  // extract -- see the 'R' rows.
  std::unordered_set<const Function *> blindCallees;
  unsigned callSites = 0, blindCallSites = 0;
  for (auto &func : mod->functions()) {
    for (auto &bb : func.basicBlocks()) {
      bool blind = bb.getTerminator()->isIndirectBranch();
      for (auto &inst : bb.instructions()) {
        if (!inst.isCall())
          continue;
        ++callSites;
        if (blind) {
          ++blindCallSites;
          blindCallees.insert(cast<Function>(inst.getOperand(0)));
        }
      }
    }
  }

  // Names follow IRDumper's scheme, so a row can be matched against an --ir
  // dump of the same module. Anonymous functions are numbered in module order,
  // which is what IRDumper numbers them in too.
  std::vector<Function *> funcs;
  std::unordered_map<const Function *, std::string> names;
  unsigned anonCount = 0;
  for (auto &func : mod->functions()) {
    funcs.push_back(&func);
    if (!func.getName().empty())
      names[&func] = func.getName();
    else if (hasRealAddress(&func))
      names[&func] = format("func_%04x", *func.getAddress());
    else
      names[&func] = format("func%u", ++anonCount);
  }
  // Address order, so two dumps of the same module are comparable line by line
  // and a reader can find a routine they already know the address of. Functions
  // without a real address sort last, in module order.
  std::stable_sort(funcs.begin(), funcs.end(), [](Function *a, Function *b) {
    return (hasRealAddress(a) ? *a->getAddress() : 0x10000u) <
        (hasRealAddress(b) ? *b->getAddress() : 0x10000u);
  });

  unsigned total = 0, numExternal = 0;
  unsigned numKAll = 0, numKAllExtern = 0, numKAllBody = 0;
  unsigned numInAnyFlag = 0, numOutAnyFlag = 0, numInAnyReg = 0, numOutAnyReg = 0;
  unsigned numEncode = 0, numDecode = 0, numFlagOp = 0, numIndBranch = 0, numRawRTS = 0,
           numCallsExtern = 0, numUncalled = 0, numBlindCaller = 0;
  // Counted over called functions only. A row marked 'U' has a live-out set
  // nothing narrowed, so including it would inflate every live-out tally.
  unsigned calledTotal = 0, calledOutAnyFlag = 0;
  unsigned calledOutCount[(unsigned)CPURegKind::_last] = {};
  unsigned inCount[(unsigned)CPURegKind::_last] = {};
  unsigned outCount[(unsigned)CPURegKind::_last] = {};
  unsigned modCount[(unsigned)CPURegKind::_last] = {};

  for (Function *func : funcs) {
    const auto &fd = lv->getFuncData(func);
    // liveIn is seeded with kInvalidSet, which is outside the register range.
    auto liveIn = (CPURegLiveness::SetType)(fd.sets.liveIn & CPURegLiveness::LivenessSets::kAll);
    auto liveOut = (CPURegLiveness::SetType)(fd.sets.liveOut & CPURegLiveness::LivenessSets::kAll);
    auto modified = (CPURegLiveness::SetType)(fd.modified & CPURegLiveness::LivenessSets::kAll);

    Barriers barr = findBarriers(func, lv->getCallGraph(), blindCallees);
    const char *kall = "-";
    if (modified == CPURegLiveness::LivenessSets::kAll)
      kall = func->isExternal() ? "extern" : "body";

    std::string addr =
        hasRealAddress(func) ? format("$%04x", *func->getAddress() & 0xFFFF) : std::string("-----");
    fprintf(
        os,
        "FUNC  %s %s %s %s %-6s %s %-4s %s\n",
        addr.c_str(),
        regMask(liveIn).c_str(),
        regMask(liveOut).c_str(),
        regMask(modified).c_str(),
        kall,
        barr.mask().c_str(),
        levelName(func),
        names[func].c_str());

    ++total;
    numExternal += func->isExternal();
    if (modified == CPURegLiveness::LivenessSets::kAll) {
      ++numKAll;
      if (func->isExternal())
        ++numKAllExtern;
      else
        ++numKAllBody;
    }
    numInAnyFlag += (liveIn & kFlagMask) != 0;
    numOutAnyFlag += (liveOut & kFlagMask) != 0;
    numInAnyReg += liveIn != 0;
    numOutAnyReg += liveOut != 0;
    numEncode += barr.encodeFlags;
    numDecode += barr.decodeFlag;
    numFlagOp += barr.encodeFlags || barr.decodeFlag;
    numIndBranch += barr.indBranch;
    numRawRTS += barr.rawRTS;
    numCallsExtern += barr.callsExtern;
    numUncalled += barr.uncalled;
    numBlindCaller += barr.blindCaller;
    if (!barr.uncalled) {
      ++calledTotal;
      calledOutAnyFlag += (liveOut & kFlagMask) != 0;
    }
    for (unsigned i = 0; i != (unsigned)CPURegKind::_last; ++i) {
      inCount[i] += (liveIn >> i) & 1;
      outCount[i] += (liveOut >> i) & 1;
      modCount[i] += (modified >> i) & 1;
      if (!barr.uncalled)
        calledOutCount[i] += (liveOut >> i) & 1;
    }
  }

  fprintf(os, "#\n# summary\n");
  fprintf(os, "SUM functions %u\n", total);
  fprintf(os, "SUM external %u\n", numExternal);
  fprintf(os, "SUM kall %u\n", numKAll);
  fprintf(os, "SUM kall-extern %u\n", numKAllExtern);
  fprintf(os, "SUM kall-body %u\n", numKAllBody);
  fprintf(os, "SUM live-in-any-reg %u\n", numInAnyReg);
  fprintf(os, "SUM live-out-any-reg %u\n", numOutAnyReg);
  fprintf(os, "SUM live-in-any-flag %u\n", numInAnyFlag);
  fprintf(os, "SUM live-out-any-flag %u\n", numOutAnyFlag);
  fprintf(os, "SUM barrier-encodeflags %u\n", numEncode);
  fprintf(os, "SUM barrier-decodeflag %u\n", numDecode);
  fprintf(os, "SUM barrier-flagop %u\n", numFlagOp);
  fprintf(os, "SUM barrier-indbranch %u\n", numIndBranch);
  fprintf(os, "SUM barrier-rawrts %u\n", numRawRTS);
  fprintf(os, "SUM barrier-callsextern %u\n", numCallsExtern);
  fprintf(os, "SUM barrier-uncalled %u\n", numUncalled);
  fprintf(os, "SUM barrier-blindcaller %u\n", numBlindCaller);
  fprintf(os, "SUM call-sites %u\n", callSites);
  fprintf(os, "SUM call-sites-blind %u\n", blindCallSites);
  fprintf(os, "SUM called-functions %u\n", calledTotal);
  fprintf(os, "SUM called-live-out-any-flag %u\n", calledOutAnyFlag);
  fprintf(os, "# REG <letter> in <n> out <n> mod <n> calledout <n>\n");
  fprintf(os, "# `calledout` counts live-out over called functions only -- the ones whose\n");
  fprintf(os, "# live-out an actual call site narrowed. `out` includes the 'U' rows.\n");
  for (unsigned i = 0; i != (unsigned)CPURegKind::_last; ++i) {
    fprintf(
        os,
        "REG %c in %u out %u mod %u calledout %u\n",
        kRegLetters[i],
        inCount[i],
        outCount[i],
        modCount[i],
        calledOutCount[i]);
  }
}
