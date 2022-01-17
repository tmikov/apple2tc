/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "ir/ValueList.h"

#include "apple2tc/IteratorRange.h"
#include "apple2tc/d6502.h"

#include <deque>
#include <map>
#include <optional>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Forward declarations.
class Disas;

struct Range {
  uint16_t from;
  /// Inclusive end of range.
  uint16_t to;

  Range(uint16_t from, uint16_t to) : from(from), to(to) {}

  [[nodiscard]] bool empty() const {
    return from > to;
  }

  [[nodiscard]] uint32_t byte_length() const {
    return (uint32_t)to - from + 1;
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

/// A "Basic Block" of 6502 Assembler instructions. It contains a maximum of one
/// "terminator instruction" - a branch, subroutine call, return, BRK, or invalid
/// instruction - which, if present, must be last.
///
/// It can optionally "fall" to a "next block", with or without a terminator
/// instruction.
class AsmBlock {
public:
  using BlockList = ir::ValueList<AsmBlock, AsmBlock>;
  static constexpr unsigned kNameSize = 8;

  explicit AsmBlock(uint16_t addr) : addr_(addr), successors_(this) {
    name_[0] = 0;
  }

  /// Set end address of the block (exclusive), before it is finished.
  /// We need this as a separate step before calling `finish()`, because adding
  /// backwards branches might split the block in two.
  void setEndAddress(uint32_t end) {
    assert(size_ == 0 && "block end can only be set once");
    size_ = end - addr_;
  }

  /// "Finish" the block by assigning the terminating branches. The end address
  /// must already have been set.
  void finish(AsmBlock *fall, AsmBlock *branch, bool invalid = false) {
    assert(size_ != 0 && "Block size must have already been set");
    assert(successors_.empty() && "Successors must not have been initialized");
    successors_.push_back(fall);
    if (branch)
      successors_.push_back(branch);
    invalid_ = invalid;
  }

  uint32_t addr() const {
    return addr_;
  }
  /// Exclusive end address (start + size).
  uint32_t endAddr() const {
    return addr_ + size_;
  }
  uint32_t size() const {
    return size_;
  }
  bool invalid() const {
    return invalid_;
  }

  [[nodiscard]] bool misaligned() const {
    return misaligned_;
  }
  void setMisaligned(bool misaligned) {
    misaligned_ = misaligned;
  }

  [[nodiscard]] bool implicit() const {
    return userList_.empty();
  }
  [[nodiscard]] const BlockList::UserListType &userList() const {
    return userList_;
  }

  [[nodiscard]] const char *name() const {
    return name_;
  }
  void setName(const char *name) {
    snprintf(name_, sizeof(name_), "%s", name);
  }

  [[nodiscard]] bool addrInside(uint32_t addr) const {
    return addr >= addr_ && addr < addr_ + size_;
  }

  /// \p nextBlock must be an unfinished empty block (\c size_ == 0). Its address
  /// must correspond to an instruction in this block, other than the first one.
  /// Split this block in two parts at that address and have it "fall" into
  /// \p nextBlock. Move all successors into nextBlock.
  ///
  /// This is essentially what happens when we discover a label in the middle
  /// of an existing block.
  void splitInto(AsmBlock *nextBlock);

  const AsmBlock *getFallBlock() const {
    return successors_.empty() ? nullptr : successors_[0].value();
  }

  class EndIterator {
    friend class AsmBlock;
    explicit EndIterator(uint32_t addr) : addr_(addr) {}

  public:
    bool operator<(const EndIterator &other) const {
      return addr_ < other.addr_;
    }
    bool operator>(const EndIterator &other) const {
      return addr_ > other.addr_;
    }
    bool operator==(const EndIterator &other) const {
      return addr_ == other.addr_;
    }
    bool operator!=(const EndIterator &other) const {
      return addr_ != other.addr_;
    }

  protected:
    uint32_t addr_;
  };

  class InstIterator : public EndIterator {
    friend class AsmBlock;

    /// Constructor used for begin().
    explicit InstIterator(const Disas *disas, uint32_t addr);

  public:
    using Inst = std::pair<uint16_t, CPUInst>;

    InstIterator &operator++();
    const Inst &operator*() {
      return inst_;
    }
    const Inst *operator->() {
      return &inst_;
    }

  private:
    const Disas *disas_;
    Inst inst_;
  };

  [[nodiscard]] auto instructions(const Disas *disas) const {
    return makeIteratorRange(InstIterator(disas, addr_), EndIterator(addr_ + size_));
  }

private:
  friend class ir::ValueListBase<AsmBlock, AsmBlock>;

  BlockList::UserListType userList_{};
  uint32_t addr_;
  uint32_t size_ = 0;
  bool invalid_ = false;
  /// This block starts at the middle of an instruction in an earlier overlapping
  /// block.
  bool misaligned_ = false;
  /// Successor 0 is always the fall block. It may be empty.
  BlockList successors_;
  char name_[kNameSize];
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
  struct BaseStats {
    /// The limit on collection of branch target.
    unsigned limit;
    /// Registers when start of collecting.
    Regs startRegs = {};
    /// Was any instruction executed with the D flag set.
    bool decimalSet = false;
    /// Was ADC ever executed with D flag set.
    bool decimalADC = false;
    /// Was SBC ever executed with D flag set.
    bool decimalSBC = false;
    /// Did the stack ever overflow?
    bool stackOverflow = false;
    /// Did the stack ever underflow?
    bool stackUnderflow = false;
  };
  struct Segment {
    uint16_t addr;
    std::vector<uint8_t> bytes;
  };
  struct Generation {
    Regs regs{};
    std::vector<Segment> code{};
    Generation(){};
  };

  /// Statistics.
  std::unique_ptr<BaseStats> baseStats{};
  /// All branch targets seen at runtime, sorted by address.
  std::vector<uint16_t> branchTargets;
  /// All executable generations.
  std::vector<Generation> generations;

  /// Return the start regs if present or nullptr.
  const Regs *getStartRegs() const;

  static std::unique_ptr<RuntimeData> load(const std::string &path);
};

class Disas {
public:
  enum class SelfModifiedKind : uint8_t {
    Indexed,
    Certain,
  };

  explicit Disas(std::string runDataPath);
  ~Disas();

  void loadBinary(uint16_t addr, const uint8_t *data, size_t len);
  void loadROM(const uint8_t *data, size_t len);

  [[nodiscard]] std::optional<uint16_t> getStart() const {
    return start_;
  }
  void setStart(uint16_t start) {
    start_ = start;
  }

  const RuntimeData *getRunData() const {
    return runData_.get();
  }

  auto asmBlocks() const {
    return makeIteratorRange(asmBlocks_.cbegin(), asmBlocks_.cend());
  }
  const AsmBlock *cfindAsmBlockContaining(uint16_t addr) const {
    auto it = asmBlocks_.upper_bound(addr);
    // Check if the target overlaps the previous range.
    if (it != asmBlocks_.begin()) {
      --it;
      if (it->second.addrInside(addr))
        return &it->second;
    }
    return nullptr;
  }
  const AsmBlock *cfindAsmBlockAt(uint16_t addr) const {
    auto it = asmBlocks_.find(addr);
    return it != asmBlocks_.end() ? &it->second : nullptr;
  }

  std::optional<SelfModifiedKind> checkSelfModified(uint16_t addr) const {
    auto it = selfModified_.find(addr);
    if (it != selfModified_.end())
      return it->second;
    return std::nullopt;
  }

  void run(bool noGenerations);
  void printAsmListing();
  void printSimpleC(FILE *f);

  uint8_t peek(uint16_t addr) const {
    return memory_[addr];
  }
  uint16_t peek16(uint16_t addr) const {
    return peek(addr) + peek(addr + 1) * 256;
  }
  ThreeBytes peek3(uint16_t addr) const {
    return {{peek(addr), peek(addr + 1), peek(addr + 2)}};
  }

  /// Return a view of the memory ranges.
  auto memRanges() {
    return makeIteratorRange(memRanges_.cbegin(), memRanges_.cend());
  }

private:
  void addMemRange(MemRange range);
  /// Return an iterator to the memory range containing the specified address.
  [[nodiscard]] std::vector<MemRange>::const_iterator findMemRange(uint16_t addr) const;

  AsmBlock *findAsmBlockContaining(uint16_t addr) {
    auto it = asmBlocks_.upper_bound(addr);
    // Check if the target overlaps the previous range.
    if (it != asmBlocks_.begin()) {
      --it;
      if (it->second.addrInside(addr))
        return &it->second;
    }
    return nullptr;
  }
  AsmBlock *findAsmBlockAt(uint16_t addr) {
    auto it = asmBlocks_.find(addr);
    return it != asmBlocks_.end() ? &it->second : nullptr;
  }
  /// If an AsmBlock for the specified address doesn't exist, create a new one
  /// and add it to the work queue. If the address is in the middle of an existing
  /// block and it aligns with an existing instruction, the existing block is
  /// split in two at the address.
  ///
  /// \param addr  the address of the block
  /// \param curBlock if non-null, points to the current block we are working on.
  ///     It might happen that the current block splits it two (if it ends with
  ///     a branch pointing inside it), in which case this value will be updated.
  /// \return the block at that address, whether it was just created or already existed.
  AsmBlock *addWork(uint16_t addr, AsmBlock **curBlock);

  void identifyAsmBlocks();

  void printAsmCodeRange(const AsmBlock &block, std::optional<uint32_t> &lastAddr);
  void printAsmDataRange(Range r);

  void printSimpleCPrologue(FILE *f);
  void printSimpleCEpilogue(FILE *f);
  void printSimpleCRange(FILE *f, const AsmBlock &block, const AsmBlock *nextBlock);
  /// \return true if the generated instruction "falls through" to whatever
  ///     follows.
  bool printSimpleCInst(FILE *f, uint16_t pc, CPUInst inst);
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

  /// Optional runtime execution data.
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

    [[nodiscard]] bool implicit() const {
      return comingFrom.empty();
    }
  };

  std::optional<uint16_t> start_{};
  /// AsmBlocks ordered in increasing starting address. Their ranges may overlap,
  /// if a second block refers to a misaligned offset within the first.
  std::map<uint16_t, AsmBlock> asmBlocks_{};
  std::deque<AsmBlock *> work_{};
  /// NOTE: we rely in the labels being sorted by address.
  std::map<uint16_t, LabelDesc> dataLabels_{};

  /// Instructions that modify code.
  std::unordered_set<uint16_t> selfModifers_{};
  /// Code addresses that are being modified.
  std::unordered_map<uint16_t, SelfModifiedKind> selfModified_{};

  uint8_t memory_[0x10000];
};
