/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/DebugState6502.h"

#include "apple2tc/a2symbols.h"

/// Reset all debugging.
void DebugState6502::reset() {
  setCollect(false);
  setDebugBB(false);
  setBuffering(false);
  setLimit(0);
  clearHistory();
  clearWatches();
  resetCollectedData();
}

void DebugState6502::resetCollectedData() {
  branchTargets_.clear();
}

void DebugState6502::setBuffering(bool buffering) {
  if (!buffering && buffering_) {
    history_.clear();
    history_.shrink_to_fit();
  }
  buffering_ = buffering;
}

void DebugState6502::setMaxHistory(unsigned maxHistory) {
  if (maxHistory < maxHistory_) {
    history_.resize(maxHistory);
    history_.shrink_to_fit();
  }
  maxHistory_ = maxHistory;
}

void DebugState6502::clearHistory() {
  history_.clear();
}

void DebugState6502::printHistory() {
  for (const auto &rec : history_) {
    printRecord(rec, true);
    printf("\n");
  }
}

void DebugState6502::addWatch(std::string name, uint16_t addr, uint8_t size) {
  auto it = std::find_if(watches_.begin(), watches_.end(), [addr, size](const Watch &w) {
    return w.addr == addr && w.size == size;
  });

  if (it != watches_.end()) {
    it->name = name;
  } else {
    watches_.emplace_back(std::move(name), addr, size);
  }
}

void DebugState6502::removeWatch(const char *name) {
  auto it = std::find_if(
      watches_.begin(), watches_.end(), [&name](const Watch &w) { return w.name == name; });
  if (it != watches_.end())
    watches_.erase(it);
}

void DebugState6502::addNonDebug(uint16_t from, uint16_t to) {
  nonDebug_.emplace_back(from, to);
}

Emu6502::StopReason DebugState6502::debugStateCB(void *ctx, Emu6502 *emu, uint16_t pc) {
  return static_cast<DebugState6502 *>(ctx)->debugState(emu, pc);
}

void DebugState6502::printRecord(const InstRecord &rec, bool showInst) {
  auto r = rec.regs;
  // Address.
  {
    const char *name = findApple2Symbol(r.pc);
    printf("%04X: %-8s  ", r.pc, name ? name : "");
  }

  // Dump the registers.
  printf("A=%02X X=%02X Y=%02X SP=%02X SR=", r.a, r.x, r.y, r.sp);
  // Dump the flags.
  static const char names[9] = "NV.BDIZC";
  for (unsigned i = 0; i != 8; ++i)
    putchar((r.status & (0x80 >> i)) ? names[i] : '.');

  if (showInst) {
    // The PC again for convenience.
    printf(" PC=%04X  ", r.pc);
    // Dump the next instruction.
    ThreeBytes bytes = rec.bytes;
    auto inst = decodeInst(r.pc, bytes);
    auto fmt = formatInst(inst, bytes, apple2SymbolResolver);
    printf("  %-8s    %s", fmt.bytes, fmt.inst);
    if (fmt.operand[0]) {
      printf("  %s", fmt.operand.c_str());
      if (inst.addrMode == CPUAddrMode::Rel)
        printf(" (%d)", (int8_t)bytes.d[1]);
    }
  }
}

void DebugState6502::addRecord(const InstRecord &rec) {
  if (history_.size() >= maxHistory_)
    history_.pop_front();
  history_.push_back(rec);
}

Emu6502::StopReason DebugState6502::debugState(Emu6502 *emu, uint16_t pc) {
  // Don't debug in areas that have been excluded.
  for (auto p : nonDebug_) {
    if (pc >= p.first && pc <= p.second)
      return Emu6502::StopReason::None;
  }

  bool wasBranchTarget = branchTarget_;
  if (debugBB_) {
    // Determine whether the next instruction is a branch target.
    CPUOpcode opc = decodeOpcode(emu->ram_peek(pc));
    switch (opc.kind) {
    case CPUInstKind::BRK:
    case CPUInstKind::JMP:
    case CPUInstKind::JSR:
    case CPUInstKind::RTI:
    case CPUInstKind::RTS:
      branchTarget_ = true;
      break;
    default:
      // Only relative branches use relative address mode.
      branchTarget_ = opc.addrMode == CPUAddrMode::Rel;
      break;
    }

    // If the current instruction was not a branch, leave.
    if (!wasBranchTarget)
      return Emu6502::StopReason::None;
  }

  if (limit_ && icount_ >= limit_)
    return Emu6502::StopReason::StopRequesed;
  ++icount_;

  if (collect_) {
    if (wasBranchTarget)
      branchTargets_.insert(pc);
    return Emu6502::StopReason::None;
  }

  Emu6502::Regs r = emu->getRegs();
  InstRecord rec = {.regs = r};
  for (unsigned i = 0; i != 3; ++i)
    rec.bytes.d[i] = emu->ram_peek(pc + i);

  if (buffering_) {
    addRecord(rec);

    if (decodeOpcode(rec.bytes.d[0]).kind == CPUInstKind::INVALID) {
      printf("*** INVALID OPCODE! Dumping history:\n");
      printHistory();
      return Emu6502::StopReason::StopRequesed;
    }

    return Emu6502::StopReason::None;
  }

  printRecord(rec, watches_.empty());

  // Dump watches
  for (const auto &watch : watches_) {
    putchar(' ');
    if (!watch.name.empty())
      printf("%s", watch.name.c_str());
    if (watch.addr < 256)
      printf("($%02X)=", watch.addr);
    else
      printf("($%04X)=", watch.addr);
    if (watch.size == 1)
      printf("$%02X", emu->ram_peek(watch.addr));
    else
      printf("$%04X", emu->ram_peek16(watch.addr));
  }
  putchar('\n');

  return Emu6502::StopReason::None;
}
