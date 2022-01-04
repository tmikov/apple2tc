/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "apple2tc/BitSet.h"
#include "apple2tc/d6502.h"
#include "apple2tc/emu6502.h"

#include <deque>
#include <functional>
#include <ostream>
#include <string>
#include <unordered_set>
#include <vector>

/// A helper class for dumping debug state to stdout.
class DebugState6502 {
public:
  /// This callback should be installed in Emu6502 with setDebugStateCB(),
  /// passing the address of DebugState6502 as `ctx`.
  static Emu6502::StopReason debugStateCB(void *ctx, Emu6502 *emu, uint16_t pc);

  enum class Mode {
    /// Nothing but breakpoints.
    None,
    /// Collect runtime data for Apple2tc.
    Collect,
    /// Trace instructions.
    Trace,
  };

  /// Set the callback to be invoked when a breakpoint is hit. Call with {} to
  /// remove the callback and disable breakpoint checking.
  void setBreakpointCB(std::function<Emu6502::StopReason(uint16_t)> cb) {
    breakpointCB_ = std::move(cb);
  }

  /// Whether to attempt to resolve operand addresses to well known AppleII symbols.
  void setResolveApple2Symbols(bool resolveApple2Symbols) {
    resolveApple2Symbols_ = resolveApple2Symbols;
  }

  void setModeNone();
  void setModeCollect(Emu6502 *emu, unsigned limit);
  void setModeTrace(unsigned limit, bool btOnly);
  Mode getMode() const {
    return mode_;
  }

  void enableHistory(bool on);
  void setMaxHistory(unsigned maxHistory);
  void clearHistory();
  void printHistory();

  /// Reset all collected data.
  void clearCollectedData();

  /// Record the collected data as JSON to a stream.
  void finishCollection(const Emu6502 *emu, std::ostream &os);

  /// Add some well known regions of code to exclude from debugging.
  void addDefaultNonDebug() {
    addNonDebug(0xFCA8, 0xFCB3); // MONWAIT
    addNonDebug(0xFD0C, 0xFD3C); // Keyboard
  }

  /// Execution in the following area will not be debugged. The range is
  /// inclusive.
  void addNonDebug(uint16_t from, uint16_t to);

  /// Clear all regions excluded from debugging.
  void clearAllNonDebug() {
    nonDebug_.clear();
  }

  void setBreakpoint(uint16_t addr) {
    breakpoints_.insert(addr);
  }
  void clearBreakpoint(uint16_t addr) {
    breakpoints_.erase(addr);
  }
  void clearAllBreakpoints() {
    breakpoints_.clear();
  }

  /// Add a watch to be printed during debugging.
  void addWatch(std::string name, uint16_t addr, uint8_t size);
  /// Remove a watch.
  void removeWatch(const char *name);
  /// Remove all watches.
  void clearWatches() {
    watches_.clear();
  }

private:
  struct InstRecord {
    /// Registers before executing the instruction.
    Emu6502::Regs regs;
    /// The instruction bytes.
    ThreeBytes bytes;
  };

  /// Print a single InstRecord.
  /// \param showInst - whether to show the instruction bytes and disassembly.
  void printRecord(const InstRecord &rec, bool showInst) const;

private:
  void addRecord(const InstRecord &rec);
  Emu6502::StopReason debugState(Emu6502 *emu, uint16_t pc);
  Emu6502::StopReason collectData(const Emu6502 *emu, uint16_t pc);
  void saveGeneration(const Emu6502 *emu, Emu6502::Regs regs);

private:
  /// Debugging mode.
  Mode mode_ = Mode::None;

  /// Whether to attempt to resolve operand addresses to well known AppleII symbols.
  bool resolveApple2Symbols_ = true;

  /// Number of instructions or branch targets to collect/trace.
  unsigned limit_ = 0;
  /// Current instruction number executing.
  unsigned icount_ = 0;

  /// Whether to trace only branch targets.
  bool traceOnlyBT_ = false;

  /// Breakpoints.
  std::unordered_set<uint16_t> breakpoints_;
  /// Callback on breakpoint.
  std::function<Emu6502::StopReason(uint16_t)> breakpointCB_{};

  /// Memory areas where we don't print debugging info. The ranges are
  /// inclusive.
  std::vector<std::pair<uint16_t, uint16_t>> nonDebug_;

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

  /// Buffer instructions when tracing instead of printing them.
  bool buffering_ = false;
  unsigned maxHistory_ = 16384;
  std::deque<InstRecord> history_{};

  /// Miscellaneous collected data.
  struct Collected {
    /// Registers when start of collecting.
    Emu6502::Regs startRegs = {};
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

  /// Miscellaneous collected data.
  Collected collected_ = {};

  /// When collecting, record the "unwrapped" value of the SP.
  int virtualSP_ = 0;

  /// Set by every branch instruction so the next one can be treated as a branch
  /// target.
  bool branchTarget_ = false;

  /// Collected branch targets.
  std::unordered_set<uint16_t> branchTargets_{};

  /// Keep track of addresses that are written to in the current generation.
  BitSet memWritten_{0x10000};
  /// Starts of instructions written to and executed but not written again.
  BitSet memExecStart_{0x10000};
  /// The entire length of instructions written to and executed but not written
  /// again.
  BitSet memExecFull_{0x10000};

  /// A descriptor of a range of memory.
  struct MemDesc {
    uint16_t addr;
    uint16_t len;
  };

  struct Generation {
    Emu6502::Regs regs;
    /// Descriptors of memory stored in data.
    std::vector<MemDesc> descs{};
    std::vector<uint8_t> data{};
    explicit Generation(Emu6502::Regs regs) : regs(regs) {}

    void addRange(uint16_t addr, uint16_t len, const uint8_t *d) {
      this->descs.push_back(MemDesc{.addr = addr, .len = len});
      this->data.insert(this->data.end(), d, d + len);
    }
  };

  std::vector<Generation> generations_{};
};
