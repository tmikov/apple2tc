/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "PubIR.h"
#include "ir/IR.h"

using namespace ir;

/// Replace register loads with a previous load or a previously stored value.
static bool removeRegLoads(BasicBlock *bb) {
  // Map from CPU register to its last known value.
  std::unordered_map<Value *, Value *> knownRegs{};
  InstDestroyer destroyer;
  bool changed = false;

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
    }
  }
  return changed;
}

/// Remove register stores if they are overwritten by a second store.
static bool removeRegStores(BasicBlock *bb) {
  // Map from CPU register to the last store.
  std::unordered_map<Value *, Instruction *> lastStore{};
  InstDestroyer destroyer;
  bool changed = false;

  for (auto &iRef : bb->instructions()) {
    auto *inst = &iRef;

    if (inst->getKind() == ValueKind::StoreR8) {
      auto &entry = lastStore[inst->getOperand(0)];
      // If there was a previous store, delete it.
      if (entry) {
        destroyer.add(entry);
        changed = false;
      }
      // Remember this as the last store.
      entry = inst;
    } else if (inst->getKind() == ValueKind::LoadR8) {
      // A load "validates" any prior store.
      lastStore[inst->getOperand(0)] = nullptr;
    }
  }

  return changed;
}

bool mem2reg(Module *mod) {
  bool changed = false;
  for (auto &func : mod->functions()) {
    for (auto &bb : func.basicBlocks()) {
      changed |= removeRegLoads(&bb);
      changed |= removeRegStores(&bb);
    }
  }
  return changed;
}
