/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "ir/IR.h"

#include "apple2tc/SetVector.h"

#include <unordered_map>

using namespace ir;

class CallGraph {
public:
  struct Node {
    /// Functions that call this function.
    PrimitiveSetVector<Function *> preds{};
    /// Functions that are called by this function.
    PrimitiveSetVector<Function *> succs{};
  };

private:
  Function *startFunc_;
  std::unordered_map<const Function *, Node> nodes_{};

public:
  explicit CallGraph(Module *mod);

  Function *getStartFunc() const {
    return startFunc_;
  }

  const Node &funcNode(const Function *func) const {
    auto it = nodes_.find(func);
    assert(it != nodes_.end());
    return it->second;
  }
};

class CallGraphTraits {
  const CallGraph &cg_;

public:
  explicit CallGraphTraits(CallGraph const &cg) : cg_(cg) {}
  const auto &predecessors(const Function *func) const {
    return cg_.funcNode(func).preds;
  }
  const auto &successors(const Function *func) const {
    return cg_.funcNode(func).succs;
  }
};

class InverseCallGraphTraits {
  const CallGraph &cg_;

public:
  explicit InverseCallGraphTraits(CallGraph const &cg) : cg_(cg) {}
  const auto &predecessors(const Function *func) const {
    return cg_.funcNode(func).succs;
  }
  const auto &successors(const Function *func) const {
    return cg_.funcNode(func).preds;
  }
};
