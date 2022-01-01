/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "apple2tc/CircularList.h"
#include "apple2tc/IteratorRange.h"
#include "apple2tc/d6502.h"

#include <deque>
#include <map>
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

template <class ValueT, class OwnerT, unsigned SMALL_CAP = 2>
class ValueList {
public:
  using ValueType = ValueT;
  using OwnerType = OwnerT;

  class UseNode : public ListEntry {
  public:
    explicit UseNode(ValueType *value, OwnerType *owner) : value_(value), owner_(owner) {}
    [[nodiscard]] OwnerType *owner() const {
      return owner_;
    }
    [[nodiscard]] ValueType *value() const {
      return value_;
    }

  private:
    friend class ValueList;
    ValueType *value_;
    // TODO: we should remove this and use bit tricks.
    OwnerType *owner_;
  };

  using UserListType = CircularList<UseNode>;

  ValueList(const ValueList &) = delete;
  void operator=(const ValueList &) = delete;

  explicit ValueList(OwnerType *owner) : owner_(owner) {
    initToSmall();
  }

  ~ValueList() {
    clear();
    deallocate();
  }

  void clear() {
    // Destroy in reverse order.
    for (UseNode *e = data_ + size_, *s = data_; e != s; --e) {
      auto *cur = e - 1;
      if (cur->value_)
        cur->value_->userList_.remove(cur);
      cur->~UseNode();
    }
    size_ = 0;
  }

  /// Move all values from the specified list onto our list, resetting the owner.
  void moveFrom(ValueList &&other) {
    // If the other object is not in small mode and we are empty, we can just
    // "steal" its data.
    if (!other.isSmall() && empty()) {
      // Free our buffer.
      deallocate();
      // Steal the other buffer.
      data_ = other.data_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      // Reset the other to empty.
      other.initToSmall();

      // Reset the owner of the stolen items.
      for (UseNode *cur = data_, *e = data_ + size_; cur != e; ++cur)
        cur->owner_ = owner_;

      return;
    }

    // Slow path - just copy all nodes and reset the other.
    for (const auto &node : other)
      push_back(node.value());

    other.clear();
  }

  [[nodiscard]] bool empty() const {
    return size_ == 0;
  }
  [[nodiscard]] size_t size() const {
    return size_;
  }

  [[nodiscard]] const UseNode &operator[](size_t index) const {
    assert(index < size_);
    return data_[index];
  }
  [[nodiscard]] const UseNode *begin() const {
    return data_;
  }
  [[nodiscard]] const UseNode *end() const {
    return data_ + size_;
  }

  void push_back(ValueType *value) {
    if (size_ == capacity_)
      grow();
    auto *n = new (data_ + size_++) UseNode(value, owner_);
    if (value)
      value->userList_.push_back(n);
  }

private:
  bool isSmall() const {
    return (void *)data_ == small_;
  }

  /// Init the fields to small mode, without any allocation/deallocation.
  void initToSmall() {
    data_ = reinterpret_cast<UseNode *>(small_);
    size_ = 0;
    capacity_ = SMALL_CAP;
  }

  /// Deallocate the buffer, if not small.
  void deallocate() {
    if (!isSmall())
      std::allocator<UseNode>().deallocate(data_, capacity_);
  }

  void grow() {
    // Calculate new capacity, with overflow checking.
    size_t newCap;
    // Capacity can't be zero, since we always start with kSmallCap.
    assert(capacity_ != 0);
    if (capacity_ <= SIZE_T_MAX / 2)
      newCap = capacity_ * 2;
    else if (capacity_ != SIZE_T_MAX)
      newCap = SIZE_T_MAX;
    else
      throw std::bad_array_new_length();

    // Allocate new buffer.
    UseNode *newData = std::allocator<UseNode>().allocate(newCap);

    // Move the nodes.
    moveNodes(data_, data_ + size_, newData, owner_);

    // Deallocate the old buffer.
    deallocate();
    data_ = newData;
    capacity_ = newCap;
  }

  /// Copy the nodes, removing the old ones from the list and adding the new ones.
  static void moveNodes(UseNode *from, UseNode *end, UseNode *dest, OwnerType *newOwner) {
    for (; from != end; ++dest, ++from) {
      auto *value = from->value_;
      new (dest) UseNode(value, newOwner);
      if (value)
        value->userList_.replace(from, dest);
    }
  }

private:
  UseNode *data_;
  unsigned size_;
  unsigned capacity_;
  OwnerType *owner_;

  alignas(UseNode) char small_[sizeof(UseNode) * SMALL_CAP];
};

/// A "Basic Block" of 6502 Assembler instructions. It contains a maximum of one
/// "terminator instruction" - a branch, subroutine call, return, BRK, or invalid
/// instruction - which, if present, must be last.
///
/// It can optionally "fall" to a "next block", with or without a terminator
/// instruction.
class AsmBlock {
public:
  using BlockList = ValueList<AsmBlock, AsmBlock>;
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
  friend class ValueList<AsmBlock, AsmBlock>;

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
  struct Segment {
    uint16_t addr;
    std::vector<uint8_t> bytes;
  };
  struct Generation {
    Regs regs{};
    std::vector<Segment> code{};
    Generation(){};
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
  /// Code addresses that are being modified. The value indicates whether it is
  /// certain (true), or is indexed (false).
  std::unordered_map<uint16_t, bool> selfModified_{};

  uint8_t memory_[0x10000];
};
