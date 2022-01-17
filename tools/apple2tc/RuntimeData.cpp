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

const Regs *RuntimeData::getStartRegs() const {
  if (baseStats)
    return &baseStats->startRegs;
  if (!generations.empty())
    return &generations[0].regs;
  return nullptr;
}

static Regs loadRegs(const json &jsonRegs) {
  return Regs{
      .pc = jsonRegs["pc"],
      .a = jsonRegs["a"],
      .x = jsonRegs["x"],
      .y = jsonRegs["y"],
      .status = jsonRegs["status"],
      .sp = jsonRegs["sp"],
  };
}

std::unique_ptr<RuntimeData> RuntimeData::load(const std::string &path) {
  json root;
  {
    std::ifstream is(path);
    is >> root;
    if (is.bad())
      throw std::runtime_error("Error reading from " + path);
  }

  auto res = std::make_unique<RuntimeData>();

  auto it = root.find("BaseStats");
  if (it != root.end()) {
    const json &bs = *it;
    auto baseStats = std::make_unique<BaseStats>();
    baseStats->limit = bs["limit"];
    baseStats->startRegs = loadRegs(bs["startRegs"]);
    baseStats->decimalSet = bs["decimalSet"];
    baseStats->decimalADC = bs["decimalADC"];
    baseStats->decimalSBC = bs["decimalSBC"];
    baseStats->stackOverflow = bs["stackOverflow"];
    baseStats->stackUnderflow = bs["stackUnderflow"];

    res->baseStats = std::move(baseStats);
  }

  res->branchTargets = static_cast<std::vector<uint16_t>>(root["BranchTargets"]);

  for (const json &jgen : root["generations"]) {
    res->generations.emplace_back();

    const auto &jregs = jgen["regs"];
    res->generations.back().regs = loadRegs(jgen["regs"]);

    for (const json &jseg : jgen["code"]) {
      res->generations.back().code.push_back(Segment{.addr = jseg["addr"], .bytes = jseg["bytes"]});
    }
  }

  return res;
}
