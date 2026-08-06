/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Disas.h"

#include "apple2tc/support.h"

#include "nlohmann/json.hpp"

#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <cstring>
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

  it = root.find("Branches");
  if (it != root.end()) {
    for (auto &[originStr, jsonTargets] : it->items()) {
      auto origin = (uint16_t)std::stoul(originStr);
      std::vector<uint16_t> targets = jsonTargets;
      // Make sure the targets are sorted.
      std::sort(targets.begin(), targets.end());
      res->allBranches.try_emplace(origin, std::move(targets));
    }
  }

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

std::vector<CodeAtEdge> loadCodeAt(const std::string &path) {
  FILE *f = fopen(path.c_str(), "rt");
  if (!f)
    throw std::runtime_error(format("%s: %s", path.c_str(), strerror(errno)));
  auto contents = readAll<std::string>(f);
  fclose(f);

  std::vector<CodeAtEdge> res{};
  unsigned lineNum = 0;
  for (size_t pos = 0; pos <= contents.size();) {
    size_t eol = contents.find('\n', pos);
    if (eol == std::string::npos)
      eol = contents.size();
    std::string line = contents.substr(pos, eol - pos);
    pos = eol + 1;
    ++lineNum;

    auto fail = [&path, lineNum](const std::string &what) {
      throw std::runtime_error(format("%s:%u: %s", path.c_str(), lineNum, what.c_str()));
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
      fail("expected '<hex-origin> <hex-target>'");

    uint16_t addr[2] = {0, 0};
    for (unsigned i = 0; i != 2; ++i) {
      // A '$' prefix is optional, so the file can look like 6502 source.
      std::string word = words[i][0] == '$' ? words[i].substr(1) : words[i];
      char *end;
      unsigned long value = strtoul(word.c_str(), &end, 16);
      if (word.empty() || *end)
        fail(format("invalid hexadecimal address '%s'", words[i].c_str()));
      if (value > 0xFFFF)
        fail(format("address '%s' is not a 16-bit value", words[i].c_str()));
      addr[i] = (uint16_t)value;
    }

    res.push_back(CodeAtEdge{.origin = addr[0], .target = addr[1]});
  }

  return res;
}
