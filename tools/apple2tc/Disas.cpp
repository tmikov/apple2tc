/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Disas.h"

#include <stdexcept>
#include <utility>

Range intersect(const Range &a, const Range &b) {
  return {std::max(a.from, b.from), std::min(a.to, b.to)};
}

Disas::Disas(std::string runDataPath) : runDataPath_(std::move(runDataPath)) {
  memset(memory_, 0xFF, sizeof(memory_));
}

void Disas::loadBinary(uint16_t addr, const uint8_t *data, size_t len) {
  if (len == 0)
    return;
  if (addr + len > 0x10000)
    throw std::range_error("binary is too large");
  addMemRange(MemRange(addr, (uint16_t)(addr + len - 1), true));
  memcpy(memory_ + addr, data, len);

  printf("// Loaded binary at [$%04X..$%04X]\n", addr, (uint16_t)(addr + len - 1));
}

void Disas::loadROM(const uint8_t *data, size_t len) {
  if (len == 0)
    return;
  if (len > 0x10000)
    throw std::range_error("ROM is too large");
  uint16_t start = 0x10000 - len;
  addMemRange(MemRange(start, 0xFFFF, false));
  memcpy(memory_ + start, data, len);
}

void Disas::addMemRange(MemRange range) {
  if (range.from > range.to)
    return;
  // Point `it` to the first range with starting address larger than this range's ending address.
  auto it = std::upper_bound(
      memRanges_.begin(), memRanges_.end(), range.to, [](uint16_t to, const MemRange &b) {
        return to < b.from;
      });

  // There is either no next range, or it doesn't overlap us. The only possible overlap is with
  // the previous range.
  if (it != memRanges_.begin()) {
    auto r = intersect(range, *(it - 1));
    if (r.empty())
      throw std::logic_error("MemRange overlap");
  }

  memRanges_.insert(it, range);
}

std::vector<MemRange>::const_iterator Disas::findMemRange(uint16_t addr) const {
  auto it = std::upper_bound(
      memRanges_.begin(), memRanges_.end(), addr, [](uint16_t addr, const MemRange &b) {
        return addr < b.from;
      });
  // Check if the target overlaps the previous range.
  if (it != memRanges_.begin()) {
    --it;
    if (it->inside(addr))
      return it;
  }
  return memRanges_.end();
}

std::set<Range, CompareRange>::iterator Disas::findCodeRange(uint16_t addr) {
  auto it = codeRanges_.upper_bound(addr);
  // Check if the target overlaps the previous range.
  if (it != codeRanges_.begin()) {
    --it;
    if (it->inside(addr))
      return it;
  }
  return codeRanges_.end();
}

void Disas::addCodeRange(Range range) {
  auto next = range.to != 0xFFFF ? findCodeRange(range.to + 1) : codeRanges_.end();
  if (next != codeRanges_.end()) {
    range |= *next;
    codeRanges_.erase(next);
  }
  auto prev = range.from != 0 ? findCodeRange(range.from - 1) : codeRanges_.end();
  if (prev != codeRanges_.end()) {
    range |= *prev;
    codeRanges_.erase(prev);
  }

  codeRanges_.insert(range);
}

// Add a branch target to the work queue, if it is new.
bool Disas::addLabel(uint16_t target, std::optional<uint16_t> comingFrom, const char *name) {
  auto res = codeLabels_.try_emplace(target);
  if (comingFrom.has_value())
    res.first->second.comingFrom.insert(*comingFrom);
  if (name && !res.first->second.name[0])
    snprintf(res.first->second.name, sizeof(res.first->second.name), "%s", name);
  if (!res.second)
    return false;
  if (findCodeRange(target) != codeRanges_.end())
    return false;
  work_.push_back(target);
  return true;
};

void Disas::addImplicitLabel(uint16_t addr) {
  auto res = codeLabels_.try_emplace(addr);
  if (!res.second)
    return;
}

void Disas::identifyCodeRanges() {
  while (!work_.empty()) {
    uint16_t const target = work_.front();
    work_.pop_front();

    uint16_t addr = target;
    for (;;) {
      if (findCodeRange(addr) != codeRanges_.end())
        break;
      if (findMemRange(addr) == memRanges_.end()) {
        fprintf(stderr, "accessing undefined memory at $%04X\n", addr);
      }
      CPUInst inst = decodeInst(addr, peek3(addr));
      if (inst.isInvalid()) {
        fprintf(stderr, "invalid instruction at $%04X\n", addr);
        ++addr;
        break;
      }
      uint16_t curAddr = addr;
      addr += cpuInstSize(inst.addrMode);

      if (inst.kind == CPUInstKind::JMP) {
        if (inst.addrMode == CPUAddrMode::Abs)
          addLabel(inst.operand, curAddr);
        break;
      } else if (inst.kind == CPUInstKind::JSR) {
        addLabel(inst.operand, curAddr);
        addImplicitLabel(addr);
      } else if (inst.addrMode == CPUAddrMode::Rel) {
        addLabel(inst.operand, curAddr);
        addImplicitLabel(addr);
      } else if (inst.kind == CPUInstKind::BRK) {
        ++addr;
        addImplicitLabel(addr);
        break;
      } else if (inst.kind == CPUInstKind::RTS || inst.kind == CPUInstKind::RTI) {
        break;
      }

      // If we wrapped around.
      if (addr < curAddr)
        break;
    }

    if (addr != target)
      addCodeRange(Range(target, addr - 1));
  }
}

static inline bool instAccessesData(CPUAddrMode am) {
  return cpuAddrModeHasOperand(am) && am != CPUAddrMode::Imm;
}

static bool instWritesMemNormal(CPUInstKind kind, CPUAddrMode am) {
  switch (kind) {
  case CPUInstKind::ASL:
  case CPUInstKind::LSR:
  case CPUInstKind::ROL:
  case CPUInstKind::ROR:
    return am != CPUAddrMode::A;

  case CPUInstKind::DEC:
  case CPUInstKind::INC:
  case CPUInstKind::STA:
  case CPUInstKind::STX:
  case CPUInstKind::STY:
    return true;

  default:
    return false;
  }
}

/// Return true if the operand is the effective address the instruction is accessing.
static bool operandIsEA(CPUAddrMode am) {
  switch (am) {
  case CPUAddrMode::Abs:
  case CPUAddrMode::Abs_X:
  case CPUAddrMode::Abs_Y:
  case CPUAddrMode::Zpg:
  case CPUAddrMode::Zpg_X:
  case CPUAddrMode::Zpg_Y:
    return true;

  default:
    return false;
  }
}

static bool operandIsIndexed(CPUAddrMode am) {
  switch (am) {
  case CPUAddrMode::Abs_X:
  case CPUAddrMode::Abs_Y:
  case CPUAddrMode::Ind:
  case CPUAddrMode::X_Ind:
  case CPUAddrMode::Ind_Y:
  case CPUAddrMode::Zpg_X:
  case CPUAddrMode::Zpg_Y:
    return true;

  default:
    return false;
  }
}

void Disas::run() {
  if (!start_.has_value())
    throw std::logic_error("starting address not set");
  addLabel(*start_, *start_, "START");
  identifyCodeRanges();

  if (!runDataPath_.empty()) {
    runData_ = RuntimeData::load(runDataPath_);
    unsigned newLabelCount = 0;
    for (auto addr : runData_->branchTargets) {
      newLabelCount += addLabel(addr, std::nullopt);
    }
    if (newLabelCount)
      printf("// %u new runtime labels added\n", newLabelCount);
    // Identify additional code ranges.
    identifyCodeRanges();
  }

  // Collect all data labels.
  for (auto r : codeRanges_) {
    for (uint16_t addr = r.from; addr - 1 != r.to;) {
      CPUInst inst = decodeInst(addr, peek3(addr));
      if (instAccessesData(inst.addrMode)) {
        // Can we reason about the operand at all?
        if (operandIsEA(inst.addrMode)) {
          // Check for self-modifying code.
          if (instWritesMemNormal(inst.kind, inst.addrMode)) {
            if (findCodeRange(inst.operand) != codeRanges_.end()) {
              selfModifers_.insert(addr);
              selfModified_[inst.operand] |= !operandIsIndexed(inst.addrMode);
            }
          }
        }

        if (!(inst.operand >= 0xC000 && inst.operand <= 0xCFFF) &&
            !codeLabels_.count(inst.operand)) {
          dataLabels_[inst.operand].comingFrom.insert(addr);
        }
      }

      addr += inst.size;
    }
  }

  fflush(stderr);
  printf("// ranges: %zu\n", codeRanges_.size());
  printf("// code labels: %zu\n", codeLabels_.size());
  printf("// data labels: %zu\n", dataLabels_.size());
}

void Disas::printAsmListing() {
  // Name all labels by address.
  unsigned labelNumber = 0;
  for (auto &l : codeLabels_) {
    if (l.second.name[0])
      continue;
    if (labelsByAddr_)
      snprintf(l.second.name, sizeof(l.second.name), "L_%04X", l.first);
    else if (!l.second.implicit())
      snprintf(l.second.name, sizeof(l.second.name), "L_%04u", ++labelNumber);
  }
  labelNumber = 0;
  for (auto &l : dataLabels_) {
    if (l.second.name[0])
      continue;
    if (labelsByAddr_) {
      if (l.first < 0x100)
        snprintf(l.second.name, sizeof(l.second.name), "M_%02X", l.first);
      else
        snprintf(l.second.name, sizeof(l.second.name), "M_%04X", l.first);
    } else if (!l.second.implicit())
      snprintf(l.second.name, sizeof(l.second.name), "M_%04u", ++labelNumber);
  }

  for (auto &l : dataLabels_) {
    if (findMemRange(l.first) == memRanges_.end()) {
      printf("%s          EQU    $%04X", l.second.name, l.first);
      if (!l.second.comingFrom.empty()) {
        printf(" ; xref");
        unsigned xrc = 0;
        for (auto from : l.second.comingFrom) {
          if (++xrc == 9) {
            printf("...");
            break;
          }
          printf(" $%04X", from);
        }
      }
      putchar('\n');
    }
  }

  uint32_t nextData = 0;
  for (auto r : codeRanges_) {
    if (nextData < r.from)
      printAsmDataRange(Range(nextData, r.from - 1));
    printAsmCodeRange(r);
    nextData = r.to + 1;
  }
  if (nextData != 0x10000)
    printAsmDataRange(Range(nextData, 0xFFFF));
}

void Disas::printAsmCodeRange(Range r) {
  printf("; Code range [$%04x-$%04X]\n", r.from, r.to);

  for (uint16_t addr = r.from; addr - 1 != r.to;) {
    ThreeBytes bytes = peek3(addr);
    CPUInst inst = decodeInst(addr, bytes);
    if (inst.isInvalid())
      printf("; ERROR: Invalid instruction\n");

    if (selfModifers_.count(addr))
      printf("; WARNING: Instruction performs code modification\n");
    for (unsigned i = 0, e = cpuInstSize(inst.addrMode); i != e; ++i) {
      auto it = selfModified_.find((uint16_t)(addr + i));
      if (it != selfModified_.end())
        printf("; WARNING: Instruction byte %u %s modified\n", i, it->second ? "is" : "may be");
    }

    printf("/*%04X*/ ", addr);

    auto label = codeLabels_.find(addr);
    if (label != codeLabels_.end() && !label->second.implicit()) {
      printf("%s:                  ; xref", label->second.name);
      for (auto from : label->second.comingFrom)
        printf(" $%04X", from);
      printf("\n");
      printf("/*%04X*/ ", addr);
    }

    FormattedInst fmt = formatInst(inst, bytes, [this](CPUInst inst) -> std::string {
      if (inst.addrMode != CPUAddrMode::Imm) {
        auto it = codeLabels_.find(inst.operand);
        if (it != codeLabels_.end())
          return it->second.name;
      }
      auto it = dataLabels_.find(inst.operand);
      if (it != dataLabels_.end())
        return it->second.name;
      return {};
    });

    printf("            %-3s", fmt.inst);
    if (!fmt.operand.empty()) {
      printf("    %-16s ; $%04X", fmt.operand.c_str(), inst.operand);
    }
    putchar('\n');

    addr += inst.size;
  }
}

void Disas::printAsmDataRange(Range r) {
  for (const auto &mr : memRanges_) {
    auto cross = intersect(mr, r);
    if (!cross.empty()) {
      printf("; Data range [$%04x-$%04X]\n", cross.from, cross.to);

      for (uint16_t addr = cross.from; (uint16_t)(addr - 1) != cross.to; ++addr) {
        printf("/*%04X*/ ", addr);

        auto label = dataLabels_.find(addr);
        printf(
            "%-11s DFB    $%02X", label != dataLabels_.end() ? label->second.name : "", peek(addr));
        if (label != dataLabels_.end() && !label->second.comingFrom.empty()) {
          printf(" ; xref");
          unsigned xrc = 0;
          for (auto from : label->second.comingFrom) {
            if (++xrc == 9) {
              printf("...");
              break;
            }
            printf(" $%04X", from);
          }
        }

        putchar('\n');
      }
    }
  }
}
