/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/DebugState6502.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

void DebugState6502::finishCollection(std::ostream &os) {
  std::vector<uint16_t> branchTargets;
  branchTargets.reserve(branchTargets_.size());
  branchTargets.insert(branchTargets.begin(), branchTargets_.begin(), branchTargets_.end());
  std::sort(branchTargets.begin(), branchTargets.end());

  json root;
  root["BranchTargets"] = json(branchTargets);

  os << root;
}