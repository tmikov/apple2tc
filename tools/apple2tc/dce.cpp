/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "PubIR.h"
#include "ir/IR.h"

using namespace ir;

/// Remove instructions with zero users from a block.
static bool dceBlock(BasicBlock *bb) {
  InstDestroyer destroyer;
  bool changed = false;
  for (auto &iRef : bb->reverse_instructions()) {
    Instruction *inst = &iRef;

    if (inst->hasUsers())
      continue;
    if (inst->hasSideEffects())
      continue;

    destroyer.add(inst);
    changed = true;
  }

  return changed;
}

bool dce(Module *mod) {
  bool changed = false;
  for (auto &func : mod->functions()) {
    bool funcChanged;
    do {
      funcChanged = false;
      // FIXME: we need to do this in reverse post order.
      for (auto &bb : func.basicBlocks()) {
        funcChanged |= dceBlock(&bb);
        changed |= funcChanged;
      }
    } while (funcChanged);
  }
  return changed;
}
