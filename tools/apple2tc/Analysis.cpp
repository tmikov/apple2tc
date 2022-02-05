/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "Analysis.h"

CallGraph::CallGraph(Module *mod) : startFunc_(mod->getStartFunction()) {
  for (auto &func : mod->functions()) {
    Node &curFuncNode = nodes_[&func];
    for (auto &user : func.users()) {
      Function *userFunc = user.owner()->getBasicBlock()->getFunction();
      curFuncNode.preds.insert(userFunc);
      nodes_[userFunc].succs.insert(&func);
    }
  }
}