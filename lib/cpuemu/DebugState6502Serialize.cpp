/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/DebugState6502.h"

#include "nlohmann/json.hpp"

#include <stdexcept>

using json = nlohmann::json;

static json saveRegs(const Emu6502::Regs &regs) {
  json jsonRegs;
  jsonRegs["pc"] = regs.pc;
  jsonRegs["a"] = regs.a;
  jsonRegs["x"] = regs.x;
  jsonRegs["y"] = regs.y;
  jsonRegs["status"] = regs.status;
  jsonRegs["sp"] = regs.sp;
  return jsonRegs;
}

void DebugState6502::finishCollection(const Emu6502 *emu, std::ostream &os) {
  if (mode_ == Mode::None)
    return;
  if (mode_ != Mode::Collect) {
    throw std::logic_error("Not currently collecting");
  }
  mode_ = Mode::None;

  saveGeneration(emu, emu->getRegs());
  std::vector<uint16_t> branchTargets;
  branchTargets.reserve(branchTargets_.size());
  branchTargets.insert(branchTargets.begin(), branchTargets_.begin(), branchTargets_.end());
  std::sort(branchTargets.begin(), branchTargets.end());

  json root;

  json stats;
  stats["limit"] = icount_;
  stats["startRegs"] = saveRegs(collected_.startRegs);
  stats["decimalSet"] = collected_.decimalSet;
  stats["decimalADC"] = collected_.decimalADC;
  stats["decimalSBC"] = collected_.decimalSBC;
  stats["stackOverflow"] = collected_.stackOverflow;
  stats["stackUnderflow"] = collected_.stackUnderflow;

  root["BaseStats"] = stats;
  root["BranchTargets"] = json(branchTargets);

  json jsonGens;
  for (const auto &gen : generations_) {
    json jsonGen;
    jsonGen["regs"] = saveRegs(gen.regs);

    json jsonCode = json::array();
    auto dataIt = gen.data.begin();
    for (const auto &desc : gen.descs) {
      json jsonDesc;
      jsonDesc["addr"] = desc.addr;
      jsonDesc["bytes"] = std::vector<uint8_t>(dataIt, dataIt + desc.len);
      dataIt += desc.len;

      jsonCode.push_back(std::move(jsonDesc));
    }
    jsonGen["code"] = std::move(jsonCode);

    jsonGens.push_back(std::move(jsonGen));
  }

  root["generations"] = std::move(jsonGens);

  os << root;
}