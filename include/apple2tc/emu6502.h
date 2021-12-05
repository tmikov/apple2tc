/*
* Copyright (c) Tzvetan Mikov.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstdint>

class Emu6502 {
public:
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

  struct Regs {
    uint16_t pc = 0;
    uint8_t a = 0;
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t status = 0;
    uint8_t sp = 0;
  };

  /// The address of the stack page.
  static constexpr uint16_t STACK_PAGE_ADDR = 0x100;
  /// CPU clock frequency.
  static constexpr uint32_t CLOCK_FREQ = 1023000;

  enum class StopReason {
    None,
    CyclesExpired,
    StopRequesed,
  };

public:
  explicit Emu6502(unsigned ioRangeStart, unsigned ioRangeEnd);

  /// Load ROM data at the end of address space and mark it as read-only.
  void loadROM(const uint8_t *rom, unsigned size);
  /// Reset the CPU counter to the RESET vector.
  void reset();
  /// Run the CPU for at least this many cycles.
  StopReason runFor(unsigned runCycles);

  enum DebugFlags : uint8_t {
    DebugASM = 1,
    DebugIO1 = 2,
    DebugIO2 = 4,
    DebugKbdin = 8,
    DebugStdout = 16,
  };

  void addDebugFlags(uint8_t flags) {
    debug_ |= flags;
  }
  void setDebugFlags(uint8_t flags) {
    debug_ = flags;
  }
  uint8_t getDebugFlags() const {
    return debug_;
  }

  /// Return the emulated CPU registers.
  [[nodiscard]] Regs getRegs() const {
    return Regs{pc_, a_, x_, y_, status_, sp_};
  };

  /// Set the emulated CPU registers.
  void setRegs(Regs r) {
    pc_ = r.pc;
    a_ = r.a;
    x_ = r.x;
    y_ = r.y;
    status_ = r.status;
    sp_ = r.sp;
  }

  /// Return number of cycles since the emulator was created. This number wraps
  /// around so only differences should be calculated.
  [[nodiscard]] unsigned getCycles() const {
    return cycles_;
  }

  /// Return a pointer to a 64KB buffer of RAM. Not all of that RAM is actually
  /// usable.
  [[nodiscard]] const uint8_t *getMainRAM() const {
    return ram_;
  }

  /// Read a byte from the RAM buffer. The RAM buffer is not necessarily what
  /// the CPU sees, as it may be overlapped by IO, etc.
  [[nodiscard]] uint8_t ram_peek(uint16_t addr) const {
    return ram_[addr];
  }
  /// Read a 16-bit word from the RAM buffer. The RAM buffer is not necessarily
  /// what the CPU sees, as it may be overlapped by IO, etc.
  [[nodiscard]] uint16_t ram_peek16(uint16_t addr) const {
    return ram_peek(addr) + (ram_peek(addr + 1) << 8);
  }
  /// Write a byte into the RAM buffer. This is not the same as writing it into
  /// the address space: no IO decoding is performed, etc.
  void ram_poke(uint16_t addr, uint8_t value) {
    ram_[addr] = value;
  }

  /// Read a byte from memory, iospace, swoft switches, etc. Note that there
  /// coudl be side effects, so the method isn't const.
  /// Note that the type of the parameter is uint16_t,
  /// so when invoked with an integer expression, it will be truncated to 16
  /// bits. This is deliberate.
  uint8_t peek(uint16_t addr) {
    if (addr >= ioRangeStart_ && addr <= ioRangeEnd_)
      return ioPeek(addr);
    else
      return ram_[addr];
  }
  /// Read a 16-bit from memory, iospace, swoft switches, etc. Note that there
  //  /// coudl be side effects, so the method isn't const.
  /// Note that the type of the parameter is
  /// uint16_t,so when invoked with an integer expression, it will be truncated
  /// to 16 bits. This is deliberate.
  uint16_t peek16(uint16_t addr) {
    return peek(addr) + (peek(addr + 1) << 8);
  }

  /// Write a 16-bit into memory, iospace, swoft switches, etc.
  void poke(uint16_t addr, uint8_t value) {
    if (addr >= ioRangeStart_ && addr <= ioRangeEnd_)
      ioPoke(addr, value);
    else if (addr < romStart_)
      ram_[addr] = value;
  }

protected:
  /// If debugging is activated, invoked before every instruction. Can cause
  /// the execution loop to terminated by returning StopRequested.
  virtual StopReason debugState();

  /// Perform a read in the IO range.
  virtual uint8_t ioPeek(uint16_t addr);
  /// Perform a write in the IO range.
  virtual void ioPoke(uint16_t addr, uint8_t value);

private:
  void push8(uint8_t v) {
    ram_[STACK_PAGE_ADDR + sp_--] = v;
  }
  uint8_t pop8() {
    return ram_[STACK_PAGE_ADDR + ++sp_];
  }
  void push16(uint16_t v) {
    push8(v >> 8);
    push8(v);
  }
  uint16_t pop16() {
    return pop8() + (pop8() << 8);
  }

  /// Read from the zero page.
  uint8_t peek_zpg(uint8_t addr) {
    return ram_[addr];
  }
  /// Read from the zero page.
  uint16_t peek16_zpg(uint8_t addr) {
    return peek_zpg(addr) + (peek_zpg(addr + 1) << 8);
  }
  void poke_zpg(uint8_t addr, uint8_t value) {
    ram_[addr] = value;
  }

  uint8_t peek_abs_x(uint16_t addr) {
    return peek(addr + x_);
  }
  void poke_abs_x(uint16_t addr, uint8_t value) {
    poke(addr + x_, value);
  }
  uint8_t peek_abs_y(uint16_t addr) {
    return peek(addr + y_);
  }
  void poke_abs_y(uint16_t addr, uint8_t value) {
    poke(addr + y_, value);
  }

  /// Read from the zero page addr + X.
  uint8_t peek_zpg_x(uint8_t addr) {
    return peek_zpg(addr + x_);
  }
  void poke_zpg_x(uint8_t addr, uint8_t value) {
    poke_zpg(addr + x_, value);
  }
  /// Read from the zero page addr + Y.
  uint8_t peek_zpg_y(uint8_t addr) {
    return peek_zpg(addr + y_);
  }
  void poke_zpg_y(uint8_t addr, uint8_t value) {
    poke_zpg(addr + y_, value);
  }

  /// X_Ind addressing mode LDA ($addr,X).
  /// Zeropage address (addr + x) contains the effective address.
  uint8_t peek_x_ind(uint16_t addr) {
    return peek(peek16_zpg(addr + x_));
  }
  /// X_Ind addressing mode STA ($addr,X).
  /// Zeropage address (addr + x) contains the effective address.
  void poke_x_ind(uint8_t addr, uint8_t value) {
    poke(peek16_zpg(addr + x_), value);
  }

  /// Ind_Y addressing mode LDA ($addr),Y.
  /// Effective address is peek_zpg(addr) + Y.
  uint8_t peek_ind_y(uint8_t addr) {
    return peek(peek16_zpg(addr) + y_);
  }
  /// Ind_Y addressing mode LDA ($addr),Y.
  /// Effective address is peek_zpg(addr) + Y.
  void poke_ind_y(uint16_t addr, uint8_t value) {
    poke(peek16_zpg(addr) + y_, value);
  }

  /// Update the N and Z flags based on the passed value and return the passed
  /// value for convenience.
  uint8_t updateNZ(uint8_t v) {
    static_assert(STATUS_N == 0x80, "The sign flag must be 0x80");
    status_ = (status_ & ~(STATUS_N | STATUS_Z)) | (v & STATUS_N) | (v == 0 ? STATUS_Z : 0);
    return v;
  }

  /// Update the N, Z, C flags.
  uint8_t updateNZC(unsigned v) {
    static_assert(STATUS_N == 0x80, "The sign flag must be 0x80");
    static_assert(STATUS_C == 0x01, "The carry flag must be 0x01");
    static_assert(STATUS_V == 0x40, "The overflow flag must be 0x40");

    status_ = (status_ & ~(STATUS_N | STATUS_Z | STATUS_C)) | (v & STATUS_N) |
        ((uint8_t)v == 0 ? STATUS_Z : 0) | ((v >> 8) & STATUS_C);
    return (uint8_t)v;
  }

  /// Same as updateNZC(), but C is inverted. This is used after subtractions
  /// to reflect the nature of 6502's C flag as true carry and not "borrow".
  uint8_t updateNZInvC(unsigned v) {
    return updateNZC(v ^ 0x100);
  }

  /// Update the N, Z, V and C flags as a result from addition. op1 and op2 are
  /// the addition operands, but only their sign bits are examined.
  uint8_t updateNZVC(unsigned v, uint8_t op1, uint8_t op2) {
    static_assert(STATUS_N == 0x80, "The sign flag must be 0x80");
    static_assert(STATUS_C == 0x01, "The carry flag must be 0x01");
    static_assert(STATUS_V == 0x40, "The overflow flag must be 0x40");

    status_ = (status_ & ~(STATUS_N | STATUS_Z | STATUS_C | STATUS_V)) | (v & STATUS_N) |
        ((uint8_t)v == 0 ? STATUS_Z : 0) | ((v >> 8) & STATUS_C) |
        (((~(op1 ^ op2) & (op1 ^ v)) >> 1) & STATUS_V);
    return (uint8_t)v;
  }

  /// Same as updateNZVC(), but C is inverted. This is used after subtractions
  /// to reflect the nature of 6502's C flag as true carry and not "borrow".
  uint8_t updateNZVInvC(unsigned v, uint8_t op1, uint8_t op2) {
    return updateNZVC(v ^ 0x100, op1, op2);
  }

  /// Set the carry flag to bit 0 of the specified value.
  void setCToBit0(uint8_t value) {
    static_assert(STATUS_C == 1, "Carry flag should be 0x01");
    status_ = (status_ & ~STATUS_C) | (value & STATUS_C);
  }

  /// Perform A = A + b + Carry in decimal mode and update the flags.
  void adcDecimal(uint8_t b);
  /// Perform A = A - b - Carry in decimal mode and update the flags.
  void sbcDecimal(uint8_t b);

protected:
  /// A combination of DebugFlags.
  uint8_t debug_ = 0;

private:
  uint16_t pc_ = 0;
  uint8_t a_ = 0;
  uint8_t x_ = 0;
  uint8_t y_ = 0;
  uint8_t status_ = 0;
  uint8_t sp_ = 0;

  /// IO range start.
  unsigned const ioRangeStart_ = 1;
  /// IO rang end, inclusive.
  unsigned const ioRangeEnd_ = 0;

  /// Offset in memory where the ROM starts. It is write only after.
  unsigned romStart_ = 0x10000;

  /// Number of processor cycles.
  unsigned cycles_ = 0;

  /// 64Kb of RAM.
  uint8_t ram_[0x10000];
};