/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "CPURegLiveness.h"
#include "PubIR.h"
#include "ir/IR.h"
#include "ir/IRDump.h"
#include "ir/IRUtil.h"

#include "apple2tc/support.h"

#include <deque>
#include <unordered_set>

using namespace ir;

/// Replace register loads with a previous load or a previously stored value.
static bool removeRegLoads(BasicBlock *bb, const CPURegLiveness &liveness) {
  // Map from CPU register to its last known value.
  std::unordered_map<Value *, Value *> knownRegs{};
  InstDestroyer destroyer;
  bool changed = false;
  IRContext *ctx = bb->getFunction()->getModule()->getContext();

  for (auto &iRef : bb->instructions()) {
    auto *inst = &iRef;
    if (inst->getKind() == ValueKind::StoreR8) {
      // Remember the latest stored value.
      knownRegs[inst->getOperand(0)] = inst->getOperand(1);
    } else if (inst->getKind() == ValueKind::LoadR8) {
      auto &entry = knownRegs[inst->getOperand(0)];
      if (entry) {
        inst->replaceAllUsesWith(entry);
        destroyer.add(inst);
        changed = true;
      } else {
        // Record the load itself as the latest value.
        entry = inst;
      }
    } else if (inst->getKind() == ValueKind::Call) {
      // Erase all registers modified by the call.
      CPURegLiveness::forEachInSet(
          liveness.getFuncData(cast<Function>(inst->getOperand(0))).modified,
          [&knownRegs, ctx](CPURegKind reg) { knownRegs.erase(ctx->getCPUReg(reg)); });
    }
  }
  return changed;
}

/// Remove register stores if they are overwritten by a second store.
static bool removeRegStores(BasicBlock *bb, const CPURegLiveness &liveness) {
  // Map from CPU register to the last store.
  std::unordered_map<Value *, Instruction *> lastStore{};
  InstDestroyer destroyer;
  bool changed = false;
  IRContext *ctx = bb->getFunction()->getModule()->getContext();

  auto clearRegsInSet = [&lastStore, ctx](CPURegLiveness::SetType set) {
    CPURegLiveness::forEachInSet(
        set, [&lastStore, ctx](CPURegKind reg) { lastStore[ctx->getCPUReg(reg)] = nullptr; });
  };

  for (auto &iRef : bb->instructions()) {
    auto *inst = &iRef;

    if (inst->getKind() == ValueKind::StoreR8) {
      auto &entry = lastStore[inst->getOperand(0)];
      // If there was a previous store, delete it.
      if (entry) {
        destroyer.add(entry);
        changed = true;
      }
      // Remember this as the last store.
      entry = inst;
    } else if (inst->getKind() == ValueKind::LoadR8) {
      // A load "validates" any prior store.
      lastStore[inst->getOperand(0)] = nullptr;
    } else if (inst->getKind() == ValueKind::Call) {
      auto &fData = liveness.getFuncData(cast<Function>(inst->getOperand(0)));
      // The function "loads" its Gen set, so all prior stores to those registers
      // are "validated".
      clearRegsInSet(fData.sets.gen);

      // Registers that are "killed" are guaranteed to have been overwritten by
      // the call. So, their prior stores can be deleted.
      CPURegLiveness::SetType killed = fData.sets.kill & ~fData.sets.gen;
      CPURegLiveness::forEachInSet(killed, [&lastStore, ctx, &destroyer, &changed](CPURegKind reg) {
        auto it = lastStore.find(ctx->getCPUReg(reg));
        if (it != lastStore.end() && it->second) {
          destroyer.add(it->second);
          changed = true;
          // Since we can't record ourselves as a last store, erase the previous one.
          it->second = nullptr;
        }
      });

      // Clear the modified set, since we don't know what happened to it.
      clearRegsInSet(fData.modified);
    }
  }

  // Eliminate stores to registers that are not live out.
  const auto &bbSets = liveness.bbSets(bb);
  for (auto [cpuRegVal, inst] : lastStore) {
    if (!inst)
      continue;
    if ((bbSets.liveOut & CPURegLiveness::m(cpuRegVal)) == 0) {
      destroyer.add(inst);
      changed = true;
    }
  }

  return changed;
}

bool localCPURegSSA(Module *mod) {
  auto liveness = CPURegLiveness::calc(mod);
  bool changed = false;
  for (auto &func : mod->functions()) {
    for (auto &bb : func.basicBlocks()) {
      changed |= removeRegLoads(&bb, *liveness);
      changed |= removeRegStores(&bb, *liveness);
    }
  }
  return changed;
}
