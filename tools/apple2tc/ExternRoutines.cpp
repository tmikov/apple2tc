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

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

using namespace ir;

namespace {

/// Collect every block reachable from \p entry by following successors. Note
/// that successors() yields all BasicBlock operands of the terminator, so the
/// dynamic edges of RTS/JmpInd/JSRInd are included.
std::unordered_set<BasicBlock *> reachableFrom(BasicBlock *entry) {
  std::unordered_set<BasicBlock *> visited{};
  std::vector<BasicBlock *> stack{entry};
  while (!stack.empty()) {
    auto *bb = stack.back();
    stack.pop_back();
    if (!visited.insert(bb).second)
      continue;
    for (auto &succ : successors(*bb))
      stack.push_back(&succ);
  }
  return visited;
}

/// Delete every block that is not reachable from the function entry. We have to
/// do this ourselves: dce() only removes instructions, and every terminator has
/// side effects by definition, so no amount of instruction DCE will ever make a
/// block disappear.
size_t removeUnreachableBlocks(Function *func) {
  auto reachable = reachableFrom(func->getEntryBlock());
  // A DecompileLevel::Low function has no Return, so its exit block is
  // unreachable by construction and must be kept explicitly.
  reachable.insert(func->getExitBlock());

  std::vector<BasicBlock *> dead{};
  for (auto &bb : func->basicBlocks())
    if (!reachable.count(&bb))
      dead.push_back(&bb);

  // Both eraseInstruction() and eraseBasicBlock() refuse to erase a value that
  // still has users, so clear every dead operand list before erasing anything.
  // InstDestroyer::add() clears operands immediately and erases on destruction,
  // which gives us exactly that ordering. This is safe because values never
  // cross basic block boundaries in this IR.
  {
    InstDestroyer destroyer;
    for (auto *bb : dead)
      for (auto &inst : bb->instructions())
        destroyer.add(&inst);
  }
  for (auto *bb : dead)
    func->eraseBasicBlock(bb);

  return dead.size();
}

class ExternRoutines {
  Module *const mod_;
  Function *const start_;
  IRContext *const ctx_;
  IRBuilder builder_;

  /// Dynamic branches synthesized for tail calls, with the return targets we
  /// would like to give them. The targets are attached only once we know which
  /// blocks survive on their own; see run().
  std::vector<std::pair<Instruction *, PrimitiveSetVector<BasicBlock *>>> pending_{};

public:
  explicit ExternRoutines(Module *mod)
      : mod_(mod),
        start_(mod->getStartFunction()),
        ctx_(mod->getContext()),
        builder_(mod->getContext()) {}

  bool run(const std::vector<std::pair<uint16_t, std::string>> &externs);

private:
  /// Where could a synthesized RTS following a tail call to \p entry return to?
  /// An external routine has no RTS instructions of its own, so recover the
  /// equivalent information from the callee body we are about to delete: walk
  /// it, stopping at RTS instead of following its return edges, and collect the
  /// return targets we find.
  PrimitiveSetVector<BasicBlock *> dynamicReturnBlocks(BasicBlock *entry);

  /// Find the alternate exits of \p entry: blocks that discard the routine's
  /// own return address and jump into the caller. Returns their targets.
  ///
  /// An external routine's body is deleted and replaced by hand-written C,
  /// which has no way to say "resume over there" -- so finding any of these is
  /// a refusal, not a feature. Doing it here rather than leaving it to
  /// identifySimpleRoutines() is the whole point: by the time that pass runs,
  /// the body carrying the evidence is gone and the call site looks like an
  /// ordinary one that simply returns.
  std::vector<BasicBlock *> altExits(BasicBlock *entry);

  /// Rewrite a single invocation of \p entry into a call of \p ef. Return true
  /// if \p inst should be erased.
  bool convert(
      BasicBlock *entry,
      Function *ef,
      Instruction *inst,
      const PrimitiveSetVector<BasicBlock *> &dynRet);
};

PrimitiveSetVector<BasicBlock *> ExternRoutines::dynamicReturnBlocks(BasicBlock *entry) {
  PrimitiveSetVector<BasicBlock *> res{};

  std::unordered_set<BasicBlock *> visited{};
  std::vector<BasicBlock *> stack{entry};
  while (!stack.empty()) {
    auto *bb = stack.back();
    stack.pop_back();
    if (!visited.insert(bb).second)
      continue;
    auto *terminator = bb->getTerminator();
    if (terminator && terminator->getKind() == ValueKind::RTS) {
      for (unsigned i = 1, count = terminator->getNumOperands(); i != count; ++i)
        res.insert(cast<BasicBlock>(terminator->getOperand(i)));
      continue;
    }
    for (auto &succ : successors(*bb))
      stack.push_back(&succ);
  }

  // The "fall" block of a JSR doesn't need to be considered a dynamic address.
  for (auto &iRef : predecessorInsts(*entry)) {
    if (iRef.getKind() == ValueKind::JSR && iRef.getOperand(1) != entry)
      res.erase(cast<BasicBlock>(iRef.getOperand(1)));
  }

  return res;
}

std::vector<BasicBlock *> ExternRoutines::altExits(BasicBlock *entry) {
  std::vector<BasicBlock *> res{};
  std::unordered_map<BasicBlock *, int> depthAt{};
  std::vector<std::pair<BasicBlock *, int>> stack{{entry, 0}};

  while (!stack.empty()) {
    auto [bb, entryDepth] = stack.back();
    stack.pop_back();
    if (!depthAt.try_emplace(bb, entryDepth).second)
      continue;

    auto *terminator = bb->getTerminator();
    if (!terminator || terminator->getKind() == ValueKind::RTS)
      continue;

    // Track the stack the way identifySimpleRoutines() does, but give up on a
    // path rather than complaining about it. A declared routine is not required
    // to be a well-behaved one -- most of the ROM is not -- and this walk exists
    // only to find the one idiom, so anything it cannot model is a path that
    // simply does not report an alternate exit.
    int depth = entryDepth;
    BasicBlock *altTarget = nullptr;
    bool bail = false;
    for (auto &iRef : bb->instructions()) {
      if (iRef.getKind() == ValueKind::Push8) {
        ++depth;
      } else if (iRef.getKind() == ValueKind::Pop8) {
        if (--depth < 0) {
          altTarget = matchAltExit(bb, &iRef);
          bail = true;
          break;
        }
      } else if (iRef.getKind() != ValueKind::JSR && iRef.modifiesSP()) {
        bail = true;
        break;
      }
    }

    if (altTarget) {
      res.push_back(altTarget);
      continue;
    }
    if (bail)
      continue;

    if (terminator->getKind() == ValueKind::JSR) {
      // A JSR/RTS pair is depth-neutral, so continue in the fall block only.
      stack.emplace_back(cast<BasicBlock>(terminator->getOperand(1)), depth);
    } else {
      for (auto &succ : successors(*bb))
        stack.emplace_back(&succ, depth);
    }
  }

  // Discovery order depends on the walk; the message should not.
  std::sort(res.begin(), res.end(), [](BasicBlock *a, BasicBlock *b) {
    auto addrA = a->getAddress().value_or(0x10000);
    auto addrB = b->getAddress().value_or(0x10000);
    return addrA != addrB ? addrA < addrB : a->getUniqueId() < b->getUniqueId();
  });
  res.erase(std::unique(res.begin(), res.end()), res.end());
  return res;
}

bool ExternRoutines::convert(
    BasicBlock *entry,
    Function *ef,
    Instruction *inst,
    const PrimitiveSetVector<BasicBlock *> &dynRet) {
  // Some cases set this to the IR instruction that must get all dynamic return
  // blocks as successors.
  Instruction *dynamicBranch = nullptr;

  // Create a new basic block that calls the routine and RTS-es.
  auto createCallBlock = [this, ef, &dynamicBranch, inst]() {
    BasicBlock *callBlock = start_->createBasicBlock();
    builder_.setInsertionBlock(callBlock);
    builder_.setAddress(inst->getAddress());
    builder_.createCall(ef, builder_.getLiteralU16(0));
    dynamicBranch = builder_.createRTS(builder_.getLiteralU8(0));
    return callBlock;
  };

  switch (inst->getKind()) {
  case ValueKind::JSR:
    if (inst->getOperand(0) == entry) {
      // The normal case: "JSR extern" becomes "call extern; jmp fall".
      builder_.setInsertionPointAfter(inst);
      builder_.setAddress(inst->getAddress());
      builder_.createCall(
          ef,
          builder_.getLiteralU16(
              ctx_->getPreserveReturnAddress() ? inst->getAddress().value_or(0xFFFC) + 2 : 0xFFFE));
      builder_.createJmp(inst->getOperand(1));
    } else {
      // A JSR to somewhere else, whose fall block is the extern entry point.
      assert(inst->getOperand(1) == entry);
      BasicBlock *callBlock = createCallBlock();

      builder_.setInsertionPointAfter(inst);
      builder_.setAddress(inst->getAddress());
      builder_.createJSR(inst->getOperand(0), callBlock);
    }
    break;

  case ValueKind::JTrue:
  case ValueKind::JFalse: {
    BasicBlock *callBlock = createCallBlock();

    // Redirect the branch into the routine to the new block calling it.
    Value *op1 = inst->getOperand(1);
    Value *op2 = inst->getOperand(2);
    if (op1 == entry)
      op1 = callBlock;
    if (op2 == entry)
      op2 = callBlock;

    builder_.setInsertionPointAfter(inst);
    builder_.setAddress(inst->getAddress());
    builder_.createInst(inst->getKind(), {inst->getOperand(0), op1, op2});
    break;
  }

  case ValueKind::Jmp:
    if (auto pushJmp = isSimplePushJmp(inst)) {
      // A hand-rolled JSR: push the return address, then jmp. Replace the
      // pushed address with a dummy the callee cannot observe (unless we are
      // preserving return addresses), call the routine, pop the dummy, and
      // branch to the address that was pushed, plus one.
      auto [pushHi, pushLo] = *pushJmp;
      builder_.setInsertionPointAfter(pushHi);
      builder_.setAddress(pushHi->getAddress());
      builder_.createPush8(
          ctx_->getPreserveReturnAddress() ? pushHi->getOperand(0) : builder_.getLiteralU8(0xFF));
      builder_.setInsertionPointAfter(pushLo);
      builder_.setAddress(pushLo->getAddress());
      builder_.createPush8(
          ctx_->getPreserveReturnAddress() ? pushLo->getOperand(0) : builder_.getLiteralU8(0xFE));
      auto *addr = builder_.createCPUAddr2BB(builder_.createAdd16(
          builder_.createMake16(pushLo->getOperand(0), pushHi->getOperand(0)),
          builder_.getLiteralU16(1)));

      builder_.setInsertionPointAfter(inst);
      builder_.setAddress(inst->getAddress());
      builder_.createCall(ef, builder_.getLiteralU16(0));
      builder_.createPop8();
      builder_.createPop8();

      dynamicBranch = builder_.createJmpInd(addr);

      pushHi->eraseFromBasicBlock();
      pushLo->eraseFromBasicBlock();
    } else {
      // A tail call: "JMP extern" becomes "call extern; rts".
      builder_.setInsertionPointAfter(inst);
      builder_.setAddress(inst->getAddress());
      builder_.createCall(ef, builder_.getLiteralU16(0));
      dynamicBranch = builder_.createRTS(builder_.getLiteralU8(0));
    }
    break;

  case ValueKind::JmpInd:
  case ValueKind::JSRInd:
  case ValueKind::RTS:
    // The routine is entered dynamically, so we cannot tell which of the
    // branch's targets this edge represents. Leave the invocation alone, which
    // also leaves the routine body reachable.
    fprintf(
        stderr,
        "extern: $%04x is entered dynamically by %s at $%04x; body cannot be removed\n",
        entry->getAddress().value_or(0x10000),
        getValueKindName(inst->getKind()),
        inst->getAddress().value_or(0x10000));
    return false;

  default:
    PANIC_ABORT("Invalid extern invocation instruction %s", getValueKindName(inst->getKind()));
  }

  if (dynamicBranch)
    pending_.emplace_back(dynamicBranch, dynRet);

  return true;
}

bool ExternRoutines::run(const std::vector<std::pair<uint16_t, std::string>> &externs) {
  bool changed = false;
  /// Declared routines that return into their caller, recorded here while
  /// their bodies still exist. Whether that is fatal is not knowable yet: it
  /// depends on whether any *generated* call site survives, and a call site
  /// inside another routine being declared here disappears along with it. So
  /// the decision waits until the dead blocks are gone -- see the end of this
  /// function.
  std::vector<std::pair<Function *, std::vector<BasicBlock *>>> returnIntoCaller{};

  for (auto &[addr, name] : externs) {
    BasicBlock *entry = start_->findBasicBlock(addr);
    if (!entry)
      throw std::runtime_error(format("extern routine $%04X: no basic block at this address", addr));

    auto alt = altExits(entry);
    auto dynRet = dynamicReturnBlocks(entry);

    Function *ef = mod_->createFunction();
    ef->setName(name);
    ef->setDecompileLevel(Function::DecompileLevel::Normal);
    ef->setExternal(addr);
    if (!alt.empty())
      returnIntoCaller.emplace_back(ef, std::move(alt));

    // A predecessor can be present twice (JSR func, func), so use a set.
    PrimitiveSetVector<Instruction *> preds{};
    for (auto &iRef : predecessorInsts(*entry))
      preds.insert(&iRef);
    for (auto *inst : preds) {
      if (convert(entry, ef, inst, dynRet)) {
        inst->eraseFromBasicBlock();
        changed = true;
      }
    }

    if (ctx_->getVerbosity() > 0)
      fprintf(stderr, "extern: $%04X '%s', %zu call sites\n", addr, name.c_str(), preds.size());
  }

  if (!changed)
    return false;

  // Attach the return targets of the synthesized tail-call branches, but only
  // the ones that survive without being propped up by the branch itself.
  // Otherwise a single tail call into a routine that is also called from the
  // ROM would resurrect the entire ROM.
  auto reachable = reachableFrom(start_->getEntryBlock());
  for (auto &[inst, targets] : pending_)
    for (auto *bb : targets)
      if (reachable.count(bb))
        inst->pushOperand(bb);

  size_t removed = removeUnreachableBlocks(start_);
  if (ctx_->getVerbosity() > 0)
    fprintf(stderr, "extern: removed %zu unreachable blocks\n", removed);

  // Now the dead blocks are gone, so a call that is still here is one that will
  // be generated. A routine that returns into its caller cannot be written by
  // hand -- C returns to the call site or not at all -- so a generated caller
  // would silently lose that exit. If every caller is being hand-written too,
  // there is no such call left and nothing to lose: the relationship between
  // the two bodies is then the author's to express in whatever way reads best.
  for (auto &[ef, targets] : returnIntoCaller) {
    PrimitiveSetVector<uint32_t> sites{};
    for (auto &user : ef->users()) {
      Instruction *inst = user.owner();
      if (inst->isCall())
        sites.insert(inst->getAddress().value_or(0x10000));
    }
    if (sites.empty())
      continue;

    std::string targetList{};
    for (BasicBlock *bb : targets)
      targetList += format(" $%04X", bb->getAddress().value_or(0x10000));
    std::string siteList{};
    for (uint32_t a : sites)
      siteList += format(" $%04X", a);
    throw std::runtime_error(format(
        "extern routine $%04X '%s' returns into its caller, at%s, and is still\n"
        "called by generated code at%s. A hand-written body returns to its call site\n"
        "or not at all, so that exit would be dropped with nothing to notice it.\n"
        "Declare those callers here as well, or leave this routine generated.",
        ef->getAddress().value_or(0x10000),
        ef->getName().c_str(),
        targetList.c_str(),
        siteList.c_str()));
  }

  return true;
}

} // namespace

bool externRoutines(Module *mod, const std::vector<std::pair<uint16_t, std::string>> &externs) {
  if (externs.empty())
    return false;
  return ExternRoutines(mod).run(externs);
}

std::vector<std::pair<uint16_t, std::string>> loadExternRoutines(
    Module *mod,
    const std::string &path) {
  FILE *f = fopen(path.c_str(), "rt");
  if (!f)
    throw std::runtime_error(format("%s: %s", path.c_str(), strerror(errno)));
  auto contents = readAll<std::string>(f);
  fclose(f);

  std::vector<std::pair<uint16_t, std::string>> res{};
  unsigned lineNum = 0;
  for (size_t pos = 0; pos <= contents.size();) {
    size_t eol = contents.find('\n', pos);
    if (eol == std::string::npos)
      eol = contents.size();
    std::string line = contents.substr(pos, eol - pos);
    pos = eol + 1;
    ++lineNum;

    auto fail = [&path, lineNum](const char *what) {
      throw std::runtime_error(format("%s:%u: %s", path.c_str(), lineNum, what));
    };

    if (auto comment = line.find('#'); comment != std::string::npos)
      line.erase(comment);

    // Split into whitespace separated words.
    std::vector<std::string> words{};
    for (size_t i = 0; i != line.size();) {
      if (isspace((unsigned char)line[i])) {
        ++i;
        continue;
      }
      size_t start = i;
      while (i != line.size() && !isspace((unsigned char)line[i]))
        ++i;
      words.push_back(line.substr(start, i - start));
    }

    if (words.empty())
      continue;
    if (words.size() != 2)
      fail("expected '<hex-address> <name>'");

    char *end;
    unsigned long addr = strtoul(words[0].c_str(), &end, 16);
    if (*end || words[0].empty())
      fail("invalid hexadecimal address");
    if (addr > 0xFFFF)
      fail("address is not a 16-bit value");
    if (!mod->getStartFunction()->findBasicBlock((uint16_t)addr))
      fail(format("no code at address $%04X", (unsigned)addr).c_str());

    res.emplace_back((uint16_t)addr, words[1]);
  }

  return res;
}
