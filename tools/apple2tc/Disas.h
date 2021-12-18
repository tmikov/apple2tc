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
#include <unordered_map>
#include <unordered_set>
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


struct Regs {
  /// Negative.
  static constexpr uint8_t STATUS_N = 0x80;
  /// Overflow.
  static constexpr uint8_t STATUS_V = 0x40;
  /// Ignored..
  static constexpr uint8_t STATUS_IGNORED = 0x20;
  /// Break.
  static constexpr uint8_t STATUS_B = 0x10;
  /// Decimal.
  static constexpr uint8_t STATUS_D = 0x08;
  /// Interrupt.
  static constexpr uint8_t STATUS_I = 0x04;
  /// Zero.
  static constexpr uint8_t STATUS_Z = 0x02;
  /// Carry.
  static constexpr uint8_t STATUS_C = 0x01;

  uint16_t pc = 0;
  uint8_t a = 0;
  uint8_t x = 0;
  uint8_t y = 0;
  uint8_t status = 0;
  uint8_t sp = 0;
};

struct RuntimeData {
  struct Segment {
    uint16_t addr;
    std::vector<uint8_t> bytes;
  };
  struct Generation {
    Regs regs{};
    std::vector<Segment> code{};
    Generation() {};
  };

  /// All branch targets seen at runtime, sorted by address.
  std::vector<uint16_t> branchTargets;
  /// All executable generations.
  std::vector<Generation> generations;

  static std::unique_ptr<RuntimeData> load(const std::string &path);
};

class Disas {
public:
  explicit Disas(std::string runDataPath);

  void loadBinary(uint16_t addr, const uint8_t *data, size_t len);
  void loadROM(const uint8_t *data, size_t len);

  [[nodiscard]] std::optional<uint16_t> getStart() const {
    return start_;
  }
  void setStart(uint16_t start) {
    start_ = start;
  }

  void run();
  void printAsmListing();
  void printSimpleC(FILE *f);

  uint8_t peek(uint16_t addr) {
    return memory_[addr];
  }
  uint16_t peek16(uint16_t addr) {
    return peek(addr) + peek(addr + 1) * 256;
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
  /// Add a new label with an optional xref.
  /// \return true if this is a new code range and the label was added to the work list.
  bool addLabel(uint16_t target, std::optional<uint16_t> comingFrom, const char *name = nullptr);
  void addImplicitLabel(uint16_t addr);

  void identifyCodeRanges();

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
  /// Optional path to a JSON file with runtime data.
  std::string runDataPath_;

  /// Optional runtimt execution data.
  std::unique_ptr<RuntimeData> runData_;

  /// Whether to name labels by their address or their order of definition.
  bool labelsByAddr_ = true;
  std::vector<MemRange> memRanges_{};

  struct LabelDesc {
    char name[8];
    std::set<uint16_t> comingFrom{};

    LabelDesc() {
      name[0] = 0;
    }

    bool implicit() const {
      return comingFrom.empty();
    }
  };

  std::optional<uint16_t> start_{};
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

  /// Current generation from RuntimeData.
  unsigned curGeneration_ = 0;

  uint8_t memory_[0x10000];
};
