/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Disas.h"

#include "nlohmann/json.hpp"

#include <fstream>
#include <stdexcept>

using json = nlohmann::json;

std::unique_ptr<RuntimeData> RuntimeData::load(const std::string &path) {
  json root;
  {
    std::ifstream is(path);
    is >> root;
    if (is.bad())
      throw std::runtime_error("Error reading from " + path);
  }

  auto res = std::make_unique<RuntimeData>();
  res->branchTargets = static_cast<std::vector<uint16_t>>(root["BranchTargets"]);

  for (const json &jgen : root["generations"]) {
    res->generations.emplace_back();

    const auto &jregs = jgen["regs"];
    res->generations.back().regs = Regs{
        .pc = jregs["pc"],
        .a = jregs["a"],
        .x = jregs["x"],
        .y = jregs["y"],
        .status = jregs["status"],
        .sp = jregs["sp"],
    };

    for (const json &jseg : jgen["code"]) {
      res->generations.back().code.push_back(Segment{.addr = jseg["addr"], .bytes = jseg["bytes"]});
    }
  }

  return res;
}
