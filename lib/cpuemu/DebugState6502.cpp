/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/DebugState6502.h"

#include "apple2tc/a2symbols.h"

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
  for (const auto &rec : history_)
    printRecord(rec);
}

void DebugState6502::addWatch(std::string name, uint16_t addr, uint8_t size) {
  auto it = std::find_if(
      watches_.begin(), watches_.end(), [&name](const Watch &w) { return w.name == name; });

  if (it != watches_.end()) {
    it->addr = addr;
    it->size = size;
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

Emu6502::StopReason DebugState6502::debugStateCB(void *ctx, Emu6502 *emu) {
  return static_cast<DebugState6502 *>(ctx)->debugState(emu);
}

void DebugState6502::printRecord(const InstRecord &rec) {
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
  printf("\n");
}

void DebugState6502::addRecord(const InstRecord &rec) {
  if (history_.size() >= maxHistory_)
    history_.pop_front();
  history_.push_back(rec);
}

Emu6502::StopReason DebugState6502::debugState(Emu6502 *emu) {
  Emu6502::Regs r = emu->getRegs();

  // Don't debug in areas that have been excluded.
  for (auto p : nonDebug_) {
    if (r.pc >= p.first && r.pc <= p.second)
      return Emu6502::StopReason::None;
  }

  if (limit_ && icount_ >= limit_)
    return Emu6502::StopReason::StopRequesed;
  ++icount_;

  InstRecord rec = {.regs = r};
  for (unsigned i = 0; i != 3; ++i)
    rec.bytes.d[i] = emu->ram_peek(r.pc + i);

  if (buffering_) {
    addRecord(rec);

    if (decodeOpcode(rec.bytes.d[0]).kind == CPUInstKind::INVALID) {
      printf("*** INVALID OPCODE! Dumping history:\n");
      printHistory();
      return Emu6502::StopReason::StopRequesed;
    }

    return Emu6502::StopReason::None;
  }

  printRecord(rec);

  // Dump watches
  for (const auto &watch : watches_) {
    printf("  %-8s", watch.name.c_str());
    if (watch.addr < 256)
      printf("($%02X)  = ", watch.addr);
    else
      printf("($%04X)= ", watch.addr);
    if (watch.size == 1)
      printf("$%02X (%u)\n", emu->peek(watch.addr), emu->peek(watch.addr));
    else
      printf("$%04X (%u)\n", emu->peek16(watch.addr), emu->peek16(watch.addr));
  }

  return Emu6502::StopReason::None;
}
