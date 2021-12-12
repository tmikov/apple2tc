/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "apple2tc/d6502.h"

#include <deque>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <vector>

struct Range {
  uint16_t from;
  /// Inclusive end of range.
  uint16_t to;

  Range(uint16_t from, uint16_t to) : from(from), to(to) {}

  [[nodiscard]] bool empty() const {
    return from > to;
  }

  [[nodiscard]] bool inside(uint16_t addr) const {
    return addr >= from && addr <= to;
  }

  void operator|=(const Range &o) {
    from = std::min(from, o.from);
    to = std::max(to, o.to);
  }
};

inline bool operator<(const Range &a, const Range &b) {
  return a.from < b.from;
}

Range intersect(const Range &a, const Range &b);

struct CompareRange {
  using is_transparent = void;
  bool operator()(const Range &a, const Range &b) const {
    return a.from < b.from;
  }
  bool operator()(const Range &a, uint16_t b) const {
    return a.from < b;
  }
  bool operator()(uint16_t a, const Range &b) const {
    return a < b.from;
  }
};

struct MemRange : public Range {
  bool writable;

  MemRange(uint16_t from, uint16_t to, bool writable) : Range(from, to), writable(writable) {}
};

class Disas {
public:
  explicit Disas();

  void loadBinary(uint16_t addr, const uint8_t *data, size_t len);
  void loadROM(const uint8_t *data, size_t len);

  void run(uint16_t start);
  void printAsmListing();
  void printSimpleC(FILE *f);

  uint8_t peek(uint16_t addr) {
    return memory_[addr];
  }
  ThreeBytes peek3(uint16_t addr) {
    return {{peek(addr), peek(addr + 1), peek(addr + 2)}};
  }

private:
  void addMemRange(MemRange range);
  /// Return an iterator to the memory range containing the specified address.
  [[nodiscard]] std::vector<MemRange>::const_iterator findMemRange(uint16_t addr) const;

  std::set<Range, CompareRange>::iterator findCodeRange(uint16_t addr);
  void addCodeRange(Range range);
  void addLabel(uint16_t target, uint16_t comingFrom);
  void addImplicitLabel(uint16_t addr);

  void identifyCodeRanges(uint16_t start);

  void printAsmCodeRange(Range r);
  void printAsmDataRange(Range r);

  void printSimpleCPrologue(FILE *f);
  void printSimpleCEpilogue(FILE *f);
  void printSimpleCInst(FILE *f, uint16_t pc, CPUInst inst);
  std::string simpleCRead(CPUInst inst) const;
  static const char *simpleCReadOp(CPUInst inst);
  static const char *simpleCWriteOp(CPUInst inst);
  std::string simpleCAddr(CPUInst inst) const;

private:
  /// Current PC in simple C mode.
  uint16_t scPC_ = 0;
  /// Whether the current operand supports self modification.
  bool scSelfModOperand_ = false;

private:
  /// Whether to name labels by their address or their order of definition.
  bool labelsByAddr_ = true;
  std::vector<MemRange> memRanges_{};

  struct LabelDesc {
    char name[8];
    bool implicit = false;
    std::set<uint16_t> comingFrom{};

    LabelDesc() {name[0] = 0;}
  };

  uint16_t start_ = 0;
  std::set<Range, CompareRange> codeRanges_{};
  /// NOTE: we rely in the labels being sorted by address.
  std::map<uint16_t, LabelDesc> codeLabels_{};
  std::map<uint16_t, LabelDesc> dataLabels_{};
  std::deque<uint16_t> work_{};

  /// Instructions that modify code.
  std::unordered_set<uint16_t> selfModifers_{};
  /// Code addresses that are being modified. The value indicates whether it is
  /// certain (true), or is indexed (false).
  std::unordered_map<uint16_t, bool> selfModified_{};

  uint8_t memory_[0x10000];
};
