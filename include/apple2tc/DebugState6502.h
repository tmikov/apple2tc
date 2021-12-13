/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "apple2tc/d6502.h"
#include "apple2tc/emu6502.h"

#include <deque>
#include <string>
#include <vector>

/// A helper class for dumping debug state to stdout.
class DebugState6502 {
public:
  /// This callback should be installed in Emu6502 with setDebugStateCB(),
  /// passing the address of DebugState6502 as `ctx`.
  static Emu6502::StopReason debugStateCB(void *ctx, Emu6502 *emu, uint16_t pc);

  /// Enable/disable basic block debugging, where only the instruction at the
  /// start of every basic block is printed.
  void setDebugBB(bool on) {
    // When we turn basic block debugging on for the first time, treat the next
    // instruction as a branch target so it will get printed.
    if (!debugBB_ && on)
      branchTarget_ = true;
    debugBB_ = true;
  }

  void setBuffering(bool buffering);
  void setMaxHistory(unsigned maxHistory);
  void clearHistory();
  void printHistory();

  /// Add some well known regions of code to exclude from debugging.
  void addDefaultNonDebug() {
    addNonDebug(0xFCA8, 0xFCB3); // MONWAIT
    addNonDebug(0xFD0C, 0xFD3C); // Keyboard
  }

  /// Set maximum number of instructions to execute. 0 means unlimited.
  void setLimit(unsigned limit) {
    limit_ = limit;
  }

  /// Add a watch to be printed during debugging.
  void addWatch(std::string name, uint16_t addr, uint8_t size);
  /// Remove a watch.
  void removeWatch(const char *name);
  /// Remove all watches.
  void clearWatches() {
    watches_.clear();
  }
  /// Execution in the following area will not be debugged. The range is
  /// inclusive.
  void addNonDebug(uint16_t from, uint16_t to);

private:
  struct InstRecord {
    /// Registers before executing the instruction.
    Emu6502::Regs regs;
    /// The instruction bytes.
    ThreeBytes bytes;
  };

  /// Print a single InstRecord.
  /// \param showInst - whether to show the instruction bytes and disassembly.
  static void printRecord(const InstRecord &rec, bool showInst);

private:
  void addRecord(const InstRecord &rec);
  Emu6502::StopReason debugState(Emu6502 *emu, uint16_t pc);

private:
  /// Number of instruction to execute.
  unsigned limit_ = 0;
  /// Current instruction number executing.
  unsigned icount_ = 0;

  /// A memory location to be printed during debugging.
  struct Watch {
    std::string name;
    uint16_t addr;
    uint8_t size;

    Watch(std::string &&name, uint16_t addr, uint8_t size)
        : name(std::move(name)), addr(addr), size(size) {}
  };

  /// All memory watches.
  std::vector<Watch> watches_;
  /// Memory areas where we don't print debugging info. The ranges are
  /// inclusive.
  std::vector<std::pair<uint16_t, uint16_t>> nonDebug_;

  bool buffering_ = false;
  unsigned maxHistory_ = 16384;
  std::deque<InstRecord> history_{};

  /// When true, we print the instruction at the beginning of every basic block.
  bool debugBB_ = false;
  /// Set by every branch instruction so the next one can be treated as a branch
  /// target.
  bool branchTarget_ = false;
};
