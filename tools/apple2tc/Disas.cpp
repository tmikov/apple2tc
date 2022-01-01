/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "Disas.h"

#include "apple2tc/apple2iodefs.h"

#include <stdexcept>
#include <utility>

void AsmBlock::splitInto(AsmBlock *nextBlock) {
  assert(nextBlock != this);
  assert(addrInside(nextBlock->addr()) && "Block to split into must overlap this one");
  assert(nextBlock->addr() != addr() && "Block to split into must have a greater start addr");
  assert(size() && "This block must have non-zero size (otherwise it wouldn't overlap anything)");
  assert(!nextBlock->size() && "Block to split into must not have a size");

  nextBlock->size_ = size_ + addr_ - nextBlock->addr_;
  size_ = nextBlock->addr_ - addr_;

  nextBlock->successors_.moveFrom(std::move(successors_));

  // Set our only successor as the next block.
  successors_.push_back(nextBlock);
}

AsmBlock::InstIterator::InstIterator(const Disas *disas, uint32_t addr)
    : EndIterator(addr), disas_(disas) {
  inst_ = std::make_pair((uint16_t)addr, decodeInst(addr, disas->peek3(addr)));
}

AsmBlock::InstIterator &AsmBlock::InstIterator::operator++() {
  addr_ += inst_.second.size;
  inst_ = std::make_pair((uint16_t)addr_, decodeInst(addr_, disas_->peek3(addr_)));
  return *this;
}

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

AsmBlock *Disas::addWork(uint16_t addr, AsmBlock **curBlock) {
  auto res = asmBlocks_.try_emplace(addr, addr);
  AsmBlock *block = &res.first->second;
  // Already exists? We are done.
  if (!res.second)
    return block;

  // Iterate while the previous block overlaps addr.
  for (auto it = res.first;;) {
    if (it == asmBlocks_.begin())
      break;
    --it;
    if (!it->second.addrInside(addr))
      break;

    AsmBlock *prevBlock = &it->second;
    uint32_t iaddr = prevBlock->addr();
    assert(iaddr < addr && "A previous block must start at smaller address");
    do
      iaddr += cpuInstSize(decodeOpcode(peek(iaddr)).addrMode);
    while (iaddr < addr);

    if (iaddr == addr) {
      // addr matches an instruction start in prevBlock. So, we must split
      // prevBlock in two at that point.
      prevBlock->splitInto(block);
      if (curBlock && *curBlock == prevBlock)
        *curBlock = block;
      return block;
    }

    // addr does not correspond to an instruction start in prevBlock. Nothing
    // more we can do. Later when we finish `block`, we can try to see if they
    // get back into alignment later (they often do after one instruction).
  }

  // We created a new block and it didn't split an existing one, so enqueue it
  // for processing.
  work_.push_back(block);
  return block;
}

void Disas::identifyAsmBlocks() {
  while (!work_.empty()) {
    AsmBlock *block = work_.front();
    work_.pop_front();

    uint32_t addr = block->addr();
    for (;;) {
      if (findMemRange(addr) == memRanges_.end()) {
        fprintf(stderr, "warning: accessing undefined memory at $%04X\n", addr);
      }
      // If we already started a basic block and encounter another one, we must
      // terminate the current one and "fall" into the found one.
      if (addr != block->addr()) {
        if (auto *next = findAsmBlockAt(addr)) {
          block->setEndAddress(addr);
          block->finish(next, nullptr, next->invalid());
          break;
        }
      }

      CPUInst inst = decodeInst(addr, peek3(addr));
      addr += inst.size;
      if (inst.isInvalid()) {
        fprintf(stderr, "warning: invalid instruction at $%04X\n", addr - 1);
        block->setEndAddress(addr);
        block->finish(nullptr, nullptr, true);
        break;
      }

      if (inst.kind == CPUInstKind::JMP) {
        block->setEndAddress(addr);
        AsmBlock *branch =
            inst.addrMode == CPUAddrMode::Abs ? addWork(inst.operand, &block) : nullptr;
        block->finish(nullptr, branch);
        break;
      } else if (inst.kind == CPUInstKind::JSR) {
        block->setEndAddress(addr);
        // Add the next address to the queue, but do not connect it as a "fall"
        // target.
        addWork(addr, nullptr);
        AsmBlock *branch = addWork(inst.operand, &block);
        block->finish(nullptr, branch);
        break;
      } else if (inst.addrMode == CPUAddrMode::Rel) {
        block->setEndAddress(addr);
        AsmBlock *fall = addWork(addr, nullptr);
        AsmBlock *branch = addWork(inst.operand, &block);
        block->finish(fall, branch);
        break;
      } else if (inst.kind == CPUInstKind::BRK) {
        block->setEndAddress(addr);
        // Add the next address to the queue, but do not connect it as a "fall"
        // target.
        addWork(addr, nullptr);
        AsmBlock *branch = addWork(peek16(A2_IRQ_VEC), &block);
        block->finish(nullptr, branch, true);
        break;
      } else if (inst.kind == CPUInstKind::RTS || inst.kind == CPUInstKind::RTI) {
        block->setEndAddress(addr);
        block->finish(nullptr, nullptr);
        break;
      }

      // If we wrapped around.
      if (addr >= 0x10000) {
        block->setEndAddress(0x10000);
        block->finish(nullptr, nullptr, true);
        break;
      }
    }
  }
}

void Disas::run() {
  if (!start_.has_value())
    throw std::logic_error("starting address not set");
  addWork(*start_, nullptr)->setName("START");

  if (!runDataPath_.empty())
    runData_ = RuntimeData::load(runDataPath_);

  // Just naively load all runtime data generations.
  if (runData_) {
    for (const auto &gen : runData_->generations) {
      for (const auto &seg : gen.code) {
        // FIXME: add memory and code ranges.
        memcpy(memory_ + seg.addr, seg.bytes.data(), seg.bytes.size());
      }
    }
  }

  identifyAsmBlocks();

  if (runData_) {
    unsigned newBlocks = 0;
    for (auto addr : runData_->branchTargets) {
      AsmBlock *block = addWork(addr, nullptr);
      // Count how many unfinished blocks are created.
      newBlocks += !block->size();
    }
    if (newBlocks)
      printf("// %u new runtime blocks added\n", newBlocks);
    // Identify additional asm blocks.
    identifyAsmBlocks();
  }

  // TODO: identify and coalesce misaligned blocks which come back in sync after
  //       one instruction.

  // Collect all data labels.
  for (const auto &block : asmBlocks_) {
    for (auto [addr, inst] : block.second.instructions(this)) {
      if (instAccessesData(inst.addrMode)) {
        // Can we reason about the operand at all?
        if (operandIsEA(inst.addrMode)) {
          // Check for self-modifying code.
          if (instWritesMemNormal(inst.kind, inst.addrMode)) {
            if (findAsmBlockContaining(inst.operand)) {
              selfModifers_.insert(addr);
              selfModified_[inst.operand] |= !operandIsIndexed(inst.addrMode);
            }
          }
        }

        if (!(inst.operand >= A2_IO_RANGE_START && inst.operand <= A2_IO_RANGE_END) &&
            !asmBlocks_.count(inst.operand)) {
          dataLabels_[inst.operand].comingFrom.insert(addr);
        }
      }
    }
  }

  fflush(stderr);
  printf("// code labels: %zu\n", asmBlocks_.size());
  printf("// data labels: %zu\n", dataLabels_.size());
}

void Disas::printAsmListing() {
  // Name all labels by address.
  unsigned labelNumber = 0;
  for (auto &block : asmBlocks_) {
    if (block.second.name()[0])
      continue;

    char buf[AsmBlock::kNameSize];
    if (labelsByAddr_)
      snprintf(buf, sizeof(buf), "L_%04X", block.first);
    else if (!block.second.implicit())
      snprintf(buf, sizeof(buf), "L_%04u", ++labelNumber);
    block.second.setName(buf);
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
  std::optional<uint32_t> lastAddr;
  for (const auto &[_, block] : asmBlocks_) {
    if (nextData < block.addr())
      printAsmDataRange(Range(nextData, block.addr() - 1));
    printAsmCodeRange(block, lastAddr);
    nextData = block.endAddr();
  }
  if (nextData != 0x10000)
    printAsmDataRange(Range(nextData, 0xFFFF));
}

void Disas::printAsmCodeRange(const AsmBlock &block, std::optional<uint32_t> &lastAddr) {
  if (!lastAddr || *lastAddr != block.addr()) {
    printf("; Code range [$%04X-$%04X]\n", block.addr(), block.endAddr() - 1);
    printf("/*%04X*/ ", block.addr());
    printf("            org    $%04X\n", block.addr());
  }
  lastAddr = block.endAddr();
  if (block.misaligned())
    printf("; WARNING: misaligned\n");

  if (block.name()[0]) {
    printf("/*%04X*/ ", block.addr());
    printf("%s:                             ; xref", block.name());
    for (const auto &node : block.userList()) {
      printf(" %s", node.owner()->name());
    }
    printf("\n");
  }

  for (auto const [addr, inst] : block.instructions(this)) {
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

    // Prepare the disassembled instruction string.
    FormattedInst fmt = formatInst(inst, peek3(addr), [this](CPUInst inst) -> std::string {
      if (inst.addrMode != CPUAddrMode::Imm) {
        if (AsmBlock *block = findAsmBlockAt(inst.operand))
          return block->name();
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
