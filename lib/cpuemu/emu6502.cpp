/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/emu6502.h"

#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static constexpr uint16_t NMI_VEC = 0xFFFA;
static constexpr uint16_t RESET_VEC = 0xFFFC;
static constexpr uint16_t IRQ_VEC = 0xFFFE;

/// Terminate with a fatal error.
static void fatal(const char *msg, ...) {
  va_list ap;
  va_start(ap, msg);
  fprintf(stderr, "*** FATAL ERROR: ");
  vfprintf(stderr, msg, ap);
  fprintf(stderr, "\n");
  va_end(ap);
  abort();
}

#define release_assert(cond, args...) \
  do {                                \
    if (!(cond))                      \
      fatal(args);                    \
  } while (0)

Emu6502::Emu6502(unsigned int ioRangeStart, unsigned int ioRangeEnd)
    : ioRangeStart_(ioRangeStart), ioRangeEnd_(ioRangeEnd) {
  memset(ram_, 0xFF, 0x10000);
}

void Emu6502::loadROM(const uint8_t *rom, unsigned int size) {
  release_assert(size <= 0x10000, "ROM larger than 64KB");
  release_assert(romStart_ == 0x10000, "ROM already loaded");
  romStart_ = 0x10000 - size;
  memcpy(ram_ + romStart_, rom, size);
  reset();
}

void Emu6502::reset() {
  a_ = 0;
  x_ = 0;
  y_ = 0;
  status_ = STATUS_IGNORED;
  sp_ = 0xFF;
  pc_ = peek16(RESET_VEC);
}

uint8_t Emu6502::adcDecimal(uint8_t b) {
  uint8_t c = status_ & STATUS_C;
  uint8_t al = (a_ & 15) + (b & 15) + c;
  if (al >= 10)
    al += 6;
  uint8_t ah = (a_ >> 4) + (b >> 4) + (al > 15);

  status_ &= ~(STATUS_N | STATUS_V | STATUS_Z | STATUS_C);
  if (!(uint8_t)(a_ + b + c))
    status_ |= STATUS_Z;
  else if (ah & 8)
    status_ |= STATUS_N;

  if (~(a_ ^ b) & (a_ ^ (ah << 4)) & 0x80)
    status_ |= STATUS_V;

  if (ah >= 10)
    ah += 6;
  if (ah > 15)
    status_ |= STATUS_C;

  return (ah << 4) | (al & 15);
}

uint8_t Emu6502::sbcDecimal(uint8_t b) {
  uint8_t c = ~status_ & STATUS_C;
  uint8_t al = (a_ & 15) - (b & 15) - c;
  if ((int8_t)(al) < 0)
    al -= 6;
  uint8_t ah = (a_ >> 4) - (b >> 4) - (int8_t(al) < 0);

  status_ &= ~(STATUS_N | STATUS_V | STATUS_Z | STATUS_C);
  uint16_t diff = a_ - b - c;
  if (!(uint8_t)diff)
    status_ |= STATUS_Z;
  else if (diff & 0x80)
    status_ |= STATUS_N;

  if ((a_ ^ b) & (a_ ^ diff) & 0x80)
    status_ |= STATUS_V;
  if (!(diff & 0xff00))
    status_ |= STATUS_C;
  if ((int8_t)ah < 0)
    ah -= 6;
  return (ah << 4) | (al & 15);
}

Emu6502::StopReason Emu6502::runFor(unsigned runCycles) {
#define OP8() peek(pc_ + 1)
#define OP16() peek16(pc_ + 1)
#define BR_ABS(x) (pc_ = (x))
#define BR_REL(x) (pc_ += (x))

  for (unsigned startCycles = cycles_; cycles_ - startCycles < runCycles; cycles_ += 3) {
    if (debug_ & DebugASM) {
      if (debugStateCB_ && debugStateCB_(debugStateCBCtx_, this, pc_) == StopReason::StopRequesed)
        return StopReason::StopRequesed;
    }

    switch (ram_[pc_]) {
    case 0x69: { // ADC #imm
      uint8_t m = OP8();
      if (!(status_ & STATUS_D))
        a_ = updateNZVC(a_ + m + (status_ & STATUS_C), a_, m);
      else
        a_ = adcDecimal(m);
      pc_ += 2;
      break;
    }
    case 0x65: { // ADC zpg
      uint8_t m = peek_zpg(OP8());
      if (!(status_ & STATUS_D))
        a_ = updateNZVC(a_ + m + (status_ & STATUS_C), a_, m);
      else
        a_ = adcDecimal(m);
      pc_ += 2;
      break;
    }
    case 0x75: { // ADC zpg,X
      uint8_t m = peek_zpg_x(OP8());
      if (!(status_ & STATUS_D))
        a_ = updateNZVC(a_ + m + (status_ & STATUS_C), a_, m);
      else
        a_ = adcDecimal(m);
      pc_ += 2;
      break;
    }
    case 0x6D: { // ADC abs
      uint8_t m = peek(OP16());
      if (!(status_ & STATUS_D))
        a_ = updateNZVC(a_ + m + (status_ & STATUS_C), a_, m);
      else
        a_ = adcDecimal(m);
      pc_ += 3;
      break;
    }
    case 0x7D: { // ADC abs,X
      uint8_t m = peek_abs_x(OP16());
      if (!(status_ & STATUS_D))
        a_ = updateNZVC(a_ + m + (status_ & STATUS_C), a_, m);
      else
        a_ = adcDecimal(m);
      pc_ += 3;
      break;
    }
    case 0x79: { // ADC abs,Y
      uint8_t m = peek_abs_y(OP16());
      if (!(status_ & STATUS_D))
        a_ = updateNZVC(a_ + m + (status_ & STATUS_C), a_, m);
      else
        a_ = adcDecimal(m);
      pc_ += 3;
      break;
    }
    case 0x61: { // ADC (ind,X)
      uint8_t m = peek_x_ind(OP8());
      if (!(status_ & STATUS_D))
        a_ = updateNZVC(a_ + m + (status_ & STATUS_C), a_, m);
      else
        a_ = adcDecimal(m);
      pc_ += 2;
      break;
    }
    case 0x71: { // ADC (ind),Y
      uint8_t m = peek_ind_y(OP8());
      if (!(status_ & STATUS_D))
        a_ = updateNZVC(a_ + m + (status_ & STATUS_C), a_, m);
      else
        a_ = adcDecimal(m);
      pc_ += 2;
      break;
    }

    case 0xE9: { // SBC #imm
      uint8_t m = OP8();
      if (!(status_ & STATUS_D))
        a_ = updateNZVInvC(a_ - m - (~status_ & STATUS_C), a_, ~m);
      else
        a_ = sbcDecimal(m);
      pc_ += 2;
      break;
    }
    case 0xE5: { // SBC zpg
      uint8_t m = peek_zpg(OP8());
      if (!(status_ & STATUS_D))
        a_ = updateNZVInvC(a_ - m - (~status_ & STATUS_C), a_, ~m);
      else
        a_ = sbcDecimal(m);
      pc_ += 2;
      break;
    }
    case 0xF5: { // SBC zpg,X
      uint8_t m = peek_zpg_x(OP8());
      if (!(status_ & STATUS_D))
        a_ = updateNZVInvC(a_ - m - (~status_ & STATUS_C), a_, ~m);
      else
        a_ = sbcDecimal(m);
      pc_ += 2;
      break;
    }
    case 0xED: { // SBC abs
      uint8_t m = peek(OP16());
      if (!(status_ & STATUS_D))
        a_ = updateNZVInvC(a_ - m - (~status_ & STATUS_C), a_, ~m);
      else
        a_ = sbcDecimal(m);
      pc_ += 3;
      break;
    }
    case 0xFD: { // SBC abs,X
      uint8_t m = peek_abs_x(OP16());
      if (!(status_ & STATUS_D))
        a_ = updateNZVInvC(a_ - m - (~status_ & STATUS_C), a_, ~m);
      else
        a_ = sbcDecimal(m);
      pc_ += 3;
      break;
    }
    case 0xF9: { // SBC abs,Y
      uint8_t m = peek_abs_y(OP16());
      if (!(status_ & STATUS_D))
        a_ = updateNZVInvC(a_ - m - (~status_ & STATUS_C), a_, ~m);
      else
        a_ = sbcDecimal(m);
      pc_ += 3;
      break;
    }
    case 0xE1: { // SBC (ind,X)
      uint8_t m = peek_x_ind(OP8());
      if (!(status_ & STATUS_D))
        a_ = updateNZVInvC(a_ - m - (~status_ & STATUS_C), a_, ~m);
      else
        a_ = sbcDecimal(m);
      pc_ += 2;
      break;
    }
    case 0xF1: { // SBC (ind),Y
      uint8_t m = peek_ind_y(OP8());
      if (!(status_ & STATUS_D))
        a_ = updateNZVInvC(a_ - m - (~status_ & STATUS_C), a_, ~m);
      else
        a_ = sbcDecimal(m);
      pc_ += 2;
      break;
    }

    case 0xC9: // CMP #imm
      updateNZInvC(a_ - OP8());
      pc_ += 2;
      break;
    case 0xC5: // CMP zpg
      updateNZInvC(a_ - peek_zpg(OP8()));
      pc_ += 2;
      break;
    case 0xD5: // CMP zpg,X
      updateNZInvC(a_ - peek_zpg_x(OP8()));
      pc_ += 2;
      break;
    case 0xCD: // CMP abs
      updateNZInvC(a_ - peek(OP16()));
      pc_ += 3;
      break;
    case 0xDD: // CMP abs,X
      updateNZInvC(a_ - peek_abs_x(OP16()));
      pc_ += 3;
      break;
    case 0xD9: // CMP abs,Y
      updateNZInvC(a_ - peek_abs_y(OP16()));
      pc_ += 3;
      break;
    case 0xC1: // CMP (ind,X)
      updateNZInvC(a_ - peek_x_ind(OP8()));
      pc_ += 2;
      break;
    case 0xD1: // CMP (ind),Y
      updateNZInvC(a_ - peek_ind_y(OP8()));
      pc_ += 2;
      break;

    case 0xE0: // CPX #imm
      updateNZInvC(x_ - OP8());
      pc_ += 2;
      break;
    case 0xE4: // CPX zpg
      updateNZInvC(x_ - peek_zpg(OP8()));
      pc_ += 2;
      break;
    case 0xEC: // CPX zpg
      updateNZInvC(x_ - peek(OP16()));
      pc_ += 3;
      break;

    case 0xC0: // CPY #imm
      updateNZInvC(y_ - OP8());
      pc_ += 2;
      break;
    case 0xC4: // CPY zpg
      updateNZInvC(y_ - peek_zpg(OP8()));
      pc_ += 2;
      break;
    case 0xCC: // CPY zpg
      updateNZInvC(y_ - peek(OP16()));
      pc_ += 3;
      break;

    case 0x29: // AND #imm
      a_ = updateNZ(a_ & OP8());
      pc_ += 2;
      break;
    case 0x25: // AND zpg
      a_ = updateNZ(a_ & peek_zpg(OP8()));
      pc_ += 2;
      break;
    case 0x35: // AND zpg,X
      a_ = updateNZ(a_ & peek_zpg_x(OP8()));
      pc_ += 2;
      break;
    case 0x2D: // AND abs
      a_ = updateNZ(a_ & peek(OP16()));
      pc_ += 3;
      break;
    case 0x3D: // AND abs,X
      a_ = updateNZ(a_ & peek_abs_x(OP16()));
      pc_ += 3;
      break;
    case 0x39: // AND abs,Y
      a_ = updateNZ(a_ & peek_abs_y(OP16()));
      pc_ += 3;
      break;
    case 0x21: // AND (ind,X)
      a_ = updateNZ(a_ & peek_x_ind(OP8()));
      pc_ += 2;
      break;
    case 0x31: // AND (ind),Y
      a_ = updateNZ(a_ & peek_ind_y(OP8()));
      pc_ += 2;
      break;

    case 0x09: // ORA #imm
      a_ = updateNZ(a_ | OP8());
      pc_ += 2;
      break;
    case 0x05: // ORA zpg
      a_ = updateNZ(a_ | peek_zpg(OP8()));
      pc_ += 2;
      break;
    case 0x15: // ORA zpg,X
      a_ = updateNZ(a_ | peek_zpg_x(OP8()));
      pc_ += 2;
      break;
    case 0x0D: // ORA abs
      a_ = updateNZ(a_ | peek(OP16()));
      pc_ += 3;
      break;
    case 0x1D: // ORA abs,X
      a_ = updateNZ(a_ | peek_abs_x(OP16()));
      pc_ += 3;
      break;
    case 0x19: // ORA abs,Y
      a_ = updateNZ(a_ | peek_abs_y(OP16()));
      pc_ += 3;
      break;
    case 0x01: // ORA (ind,X)
      a_ = updateNZ(a_ | peek_x_ind(OP8()));
      pc_ += 2;
      break;
    case 0x11: // ORA (ind),Y
      a_ = updateNZ(a_ | peek_ind_y(OP8()));
      pc_ += 2;
      break;

    case 0x49: // EOR #imm
      a_ = updateNZ(a_ ^ OP8());
      pc_ += 2;
      break;
    case 0x45: // EOR zpg
      a_ = updateNZ(a_ ^ peek_zpg(OP8()));
      pc_ += 2;
      break;
    case 0x55: // EOR zpg,X
      a_ = updateNZ(a_ ^ peek_zpg_x(OP8()));
      pc_ += 2;
      break;
    case 0x4D: // EOR abs
      a_ = updateNZ(a_ ^ peek(OP16()));
      pc_ += 3;
      break;
    case 0x5D: // EOR abs,X
      a_ = updateNZ(a_ ^ peek_abs_x(OP16()));
      pc_ += 3;
      break;
    case 0x59: // EOR abs,Y
      a_ = updateNZ(a_ ^ peek_abs_y(OP16()));
      pc_ += 3;
      break;
    case 0x41: // EOR (ind,X)
      a_ = updateNZ(a_ ^ peek_x_ind(OP8()));
      pc_ += 2;
      break;
    case 0x51: // EOR (ind),Y
      a_ = updateNZ(a_ ^ peek_ind_y(OP8()));
      pc_ += 2;
      break;

    case 0x0A: { // ASL A
      a_ = updateNZC(a_ << 1);
      pc_ += 1;
      break;
    }
    case 0x06: { // ASL zpg
      uint8_t op8 = OP8();
      poke_zpg(op8, updateNZC(peek_zpg(op8) << 1));
      pc_ += 2;
      break;
    }
    case 0x16: { // ASL zpg,X
      uint8_t op8 = OP8();
      poke_zpg_x(op8, updateNZC(peek_zpg_x(op8) << 1));
      pc_ += 2;
      break;
    }
    case 0x0E: { // ASL abs
      uint16_t op16 = OP16();
      poke(op16, updateNZC(peek(op16) << 1));
      pc_ += 3;
      break;
    }
    case 0x1E: { // ASL abs,X
      uint16_t op16 = OP16();
      poke_abs_x(op16, updateNZC(peek_abs_x(op16) << 1));
      pc_ += 3;
      break;
    }

    case 0x90: // BCC
      BR_REL(2 + (status_ & STATUS_C ? 0 : (int8_t)OP8()));
      break;
    case 0xB0: // BCS
      BR_REL(2 + (status_ & STATUS_C ? (int8_t)OP8() : 0));
      break;
    case 0xF0: // BEQ
      BR_REL(2 + (status_ & STATUS_Z ? (int8_t)OP8() : 0));
      break;
    case 0x30: // BMI
      BR_REL(2 + (status_ & STATUS_N ? (int8_t)OP8() : 0));
      break;
    case 0xD0: // BNE
      BR_REL(2 + (status_ & STATUS_Z ? 0 : (int8_t)OP8()));
      break;
    case 0x10: // BPL
      BR_REL(2 + (status_ & STATUS_N ? 0 : (int8_t)OP8()));
      break;
    case 0x50: // BVC
      BR_REL(2 + (status_ & STATUS_V ? 0 : (int8_t)OP8()));
      break;
    case 0x70: // BVS
      BR_REL(2 + (status_ & STATUS_V ? (int8_t)OP8() : 0));
      break;

    case 0x24: { // BIT zpg
      uint8_t m = peek_zpg(OP8());
      status_ = (status_ & ~(0xC0 | STATUS_Z)) | (m & 0xC0) | (a_ & m ? 0 : STATUS_Z);
      pc_ += 2;
      break;
    }
    case 0x2C: { // BIT abs
      uint8_t m = peek(OP16());
      status_ = (status_ & ~(0xC0 | STATUS_Z)) | (m & 0xC0) | (a_ & m ? 0 : STATUS_Z);
      pc_ += 3;
      break;
    }

    case 0x00: // BRK
      push16(pc_ + 2);
      push8(status_ | STATUS_B);
      BR_ABS(peek16(IRQ_VEC));
      break;

    case 0x18: // CLC
      status_ &= ~STATUS_C;
      pc_ += 1;
      break;
    case 0xD8: // CLD
      status_ &= ~STATUS_D;
      pc_ += 1;
      break;
    case 0x58: // CLI
      status_ &= ~STATUS_I;
      pc_ += 1;
      break;
    case 0xB8: // CLV
      status_ &= ~STATUS_V;
      pc_ += 1;
      break;

    case 0x38: // SEC
      status_ |= STATUS_C;
      pc_ += 1;
      break;
    case 0xF8: // SED
      status_ |= STATUS_D;
      pc_ += 1;
      break;
    case 0x78: // SEI
      status_ |= STATUS_I;
      pc_ += 1;
      break;

    case 0xE6: { // INC zpg
      uint8_t op8 = OP8();
      poke_zpg(op8, updateNZ(peek_zpg(op8) + 1));
      pc_ += 2;
      break;
    }
    case 0xF6: { // INC zpg,X
      uint8_t op8 = OP8();
      poke_zpg_x(op8, updateNZ(peek_zpg_x(op8) + 1));
      pc_ += 2;
      break;
    }
    case 0xEE: { // INC abs
      uint16_t op16 = OP16();
      poke(op16, updateNZ(peek(op16) + 1));
      pc_ += 3;
      break;
    }
    case 0xFE: { // INC abs,X
      uint16_t op16 = OP16();
      poke_abs_x(op16, updateNZ(peek_abs_x(op16) + 1));
      pc_ += 3;
      break;
    }

    case 0xC6: { // DEC zpg
      uint8_t op8 = OP8();
      poke_zpg(op8, updateNZ(peek_zpg(op8) - 1));
      pc_ += 2;
      break;
    }
    case 0xD6: { // DEC zpg,X
      uint8_t op8 = OP8();
      poke_zpg_x(op8, updateNZ(peek_zpg_x(op8) - 1));
      pc_ += 2;
      break;
    }
    case 0xCE: { // DEC abs
      uint16_t op16 = OP16();
      poke(op16, updateNZ(peek(op16) - 1));
      pc_ += 3;
      break;
    }
    case 0xDE: { // DEC abs,X
      uint16_t op16 = OP16();
      poke_abs_x(op16, updateNZ(peek_abs_x(op16) - 1));
      pc_ += 3;
      break;
    }

    case 0xE8: // INX
      x_ = updateNZ(x_ + 1);
      pc_ += 1;
      break;
    case 0xC8: // INY
      y_ = updateNZ(y_ + 1);
      pc_ += 1;
      break;
    case 0xCA: // DEX
      x_ = updateNZ(x_ - 1);
      pc_ += 1;
      break;
    case 0x88: // DEY
      y_ = updateNZ(y_ - 1);
      pc_ += 1;
      break;

    case 0x4C: // JMP abs
      BR_ABS(OP16());
      break;
    case 0x6C: // JMP (abs)
      BR_ABS(peek16(OP16()));
      break;

    case 0x20: // JSR abs
      push16(pc_ + 2);
      BR_ABS(OP16());
      break;

    case 0xA9: // LDA #.
      a_ = updateNZ(OP8());
      pc_ += 2;
      break;
    case 0xA5: // LDA zpg.
      a_ = updateNZ(peek_zpg(OP8()));
      pc_ += 2;
      break;
    case 0xB5: // LDA zpg,X.
      a_ = updateNZ(peek_zpg_x(OP8()));
      pc_ += 2;
      break;
    case 0xAD: // LDA abs.
      a_ = updateNZ(peek(OP16()));
      pc_ += 3;
      break;
    case 0xBD: // LDA abs,X.
      a_ = updateNZ(peek_abs_x(OP16()));
      pc_ += 3;
      break;
    case 0xB9: // LDA abs,Y.
      a_ = updateNZ(peek_abs_y(OP16()));
      pc_ += 3;
      break;
    case 0xA1: // LDA (ind,X)
      a_ = updateNZ(peek_x_ind(OP8()));
      pc_ += 2;
      break;
    case 0xB1: // LDA (ind),Y
      a_ = updateNZ(peek_ind_y(OP8()));
      pc_ += 2;
      break;

    case 0xA2: // LDX #.
      x_ = updateNZ(OP8());
      pc_ += 2;
      break;
    case 0xA6: // LDX zpg.
      x_ = updateNZ(peek_zpg(OP8()));
      pc_ += 2;
      break;
    case 0xB6: // LDX zpg,Y.
      x_ = updateNZ(peek_zpg_y(OP8()));
      pc_ += 2;
      break;
    case 0xAE: // LDX abs.
      x_ = updateNZ(peek(OP16()));
      pc_ += 3;
      break;
    case 0xBE: // LDX abs,Y.
      x_ = updateNZ(peek_abs_y(OP16()));
      pc_ += 3;
      break;

    case 0xA0: // LDY #.
      y_ = updateNZ(OP8());
      pc_ += 2;
      break;
    case 0xA4: // LDY zpg.
      y_ = updateNZ(peek_zpg(OP8()));
      pc_ += 2;
      break;
    case 0xB4: // LDY zpg,X.
      y_ = updateNZ(peek_zpg_x(OP8()));
      pc_ += 2;
      break;
    case 0xAC: // LDY abs.
      y_ = updateNZ(peek(OP16()));
      pc_ += 3;
      break;
    case 0xBC: // LDY abs,X.
      y_ = updateNZ(peek_abs_x(OP16()));
      pc_ += 3;
      break;

    case 0x4A: // LSR A
      setCToBit0(a_);
      a_ = updateNZ(a_ >> 1);
      pc_ += 1;
      break;
    case 0x46: { // LSR zpg
      uint8_t op8 = OP8();
      uint8_t m = peek_zpg(op8);
      setCToBit0(m);
      poke_zpg(op8, updateNZ(m >> 1));
      pc_ += 2;
      break;
    }
    case 0x56: { // LSR zpg,X
      uint8_t op8 = OP8();
      uint8_t m = peek_zpg_x(op8);
      setCToBit0(m);
      poke_zpg_x(op8, updateNZ(m >> 1));
      pc_ += 2;
      break;
    }
    case 0x4E: { // LSR abs
      uint16_t op16 = OP16();
      uint8_t m = peek(op16);
      setCToBit0(m);
      poke(op16, updateNZ(m >> 1));
      pc_ += 3;
      break;
    }
    case 0x5E: { // LSR abs,X
      uint16_t op16 = OP16();
      uint8_t m = peek_abs_x(op16);
      setCToBit0(m);
      poke_abs_x(op16, updateNZ(m >> 1));
      pc_ += 3;
      break;
    }

    case 0x2A: { // ROL A
      uint8_t m = a_;
      a_ = updateNZ((m << 1) | (status_ & STATUS_C));
      setCToBit0(m >> 7);
      pc_ += 1;
      break;
    }
    case 0x26: { // ROL zpg
      uint8_t op8 = OP8();
      uint8_t m = peek_zpg(op8);
      poke_zpg(op8, updateNZ((m << 1) | (status_ & STATUS_C)));
      setCToBit0(m >> 7);
      pc_ += 2;
      break;
    }
    case 0x36: { // ROL zpg,X
      uint8_t op8 = OP8();
      uint8_t m = peek_zpg_x(op8);
      poke_zpg_x(op8, updateNZ((m << 1) | (status_ & STATUS_C)));
      setCToBit0(m >> 7);
      pc_ += 2;
      break;
    }
    case 0x2E: { // ROL abs
      uint16_t op16 = OP16();
      uint8_t m = peek(op16);
      poke(op16, updateNZ((m << 1) | (status_ & STATUS_C)));
      setCToBit0(m >> 7);
      pc_ += 3;
      break;
    }
    case 0x3E: { // ROL abs,X
      uint16_t op16 = OP16();
      uint8_t m = peek_abs_x(op16);
      poke_abs_x(op16, updateNZ((m << 1) | (status_ & STATUS_C)));
      setCToBit0(m >> 7);
      pc_ += 3;
      break;
    }

    case 0x6A: { // ROR A
      uint8_t m = a_;
      a_ = updateNZ((m >> 1) | ((status_ & STATUS_C) << 7));
      setCToBit0(m);
      pc_ += 1;
      break;
    }
    case 0x66: { // ROR zpg
      uint8_t op8 = OP8();
      uint8_t m = peek_zpg(op8);
      poke_zpg(op8, updateNZ((m >> 1) | ((status_ & STATUS_C) << 7)));
      setCToBit0(m);
      pc_ += 2;
      break;
    }
    case 0x76: { // ROR zpg,X
      uint8_t op8 = OP8();
      uint8_t m = peek_zpg_x(op8);
      poke_zpg_x(op8, updateNZ((m >> 1) | ((status_ & STATUS_C) << 7)));
      setCToBit0(m);
      pc_ += 2;
      break;
    }
    case 0x6E: { // ROR abs
      uint16_t op16 = OP16();
      uint8_t m = peek(op16);
      poke(op16, updateNZ((m >> 1) | ((status_ & STATUS_C) << 7)));
      setCToBit0(m);
      pc_ += 3;
      break;
    }
    case 0x7E: { // ROR abs,X
      uint16_t op16 = OP16();
      uint8_t m = peek_abs_x(op16);
      poke_abs_x(op16, updateNZ((m >> 1) | ((status_ & STATUS_C) << 7)));
      setCToBit0(m);
      pc_ += 3;
      break;
    }

    case 0xEA: // NOP
      pc_ += 1;
      break;

    case 0x48: // PHA
      push8(a_);
      pc_ += 1;
      break;
    case 0x08: // PHP
      push8(status_ | STATUS_B);
      pc_ += 1;
      break;
    case 0x68: // PLA
      a_ = updateNZ(pop8());
      pc_ += 1;
      break;
    case 0x28: // PLP
      status_ = pop8() & ~STATUS_B;
      pc_ += 1;
      break;

    case 0x40: // RTI
      status_ = pop8() & ~STATUS_B;
      BR_ABS(pop16());
      break;
    case 0x60: // RTS
      BR_ABS(pop16() + 1);
      break;

    case 0x85: // STA zpg.
      poke_zpg(OP8(), a_);
      pc_ += 2;
      break;
    case 0x95: // STA zpg,X.
      poke_zpg_x(OP8(), a_);
      pc_ += 2;
      break;
    case 0x8D: // STA abs.
      poke(OP16(), a_);
      pc_ += 3;
      break;
    case 0x9D: // STA abs,X.
      poke_abs_x(OP16(), a_);
      pc_ += 3;
      break;
    case 0x99: // STA abs,Y.
      poke_abs_y(OP16(), a_);
      pc_ += 3;
      break;
    case 0x81: // STA (ind,X)
      poke_x_ind(OP8(), a_);
      pc_ += 2;
      break;
    case 0x91: // STA (ind),Y
      poke_ind_y(OP8(), a_);
      pc_ += 2;
      break;

    case 0x86: // STX zpg.
      poke_zpg(OP8(), x_);
      pc_ += 2;
      break;
    case 0x96: // STX zpg,Y.
      poke_zpg_y(OP8(), x_);
      pc_ += 2;
      break;
    case 0x8E: // STX abs.
      poke(OP16(), x_);
      pc_ += 3;
      break;

    case 0x84: // STY zpg.
      poke_zpg(OP8(), y_);
      pc_ += 2;
      break;
    case 0x94: // STY zpg,X.
      poke_zpg_x(OP8(), y_);
      pc_ += 2;
      break;
    case 0x8C: // STY abs.
      poke(OP16(), y_);
      pc_ += 3;
      break;

    case 0xAA: // TAX
      x_ = updateNZ(a_);
      pc_ += 1;
      break;
    case 0xA8: // TAY
      y_ = updateNZ(a_);
      pc_ += 1;
      break;
    case 0xBA: // TSX
      x_ = updateNZ(sp_);
      pc_ += 1;
      break;
    case 0x8A: // TXA
      a_ = updateNZ(x_);
      pc_ += 1;
      break;
    case 0x9A: // TXS
      sp_ = x_;
      pc_ += 1;
      break;
    case 0x98: // TYA
      a_ = updateNZ(y_);
      pc_ += 1;
      break;

    default:
      fprintf(stderr, "Invalid instruction $%02X\n", ram_[pc_]);
      // TODO: we might want to decode the invalid instructions the way the
      //       CPU would. Only if we find that it makes a difference.
      pc_ += 1;
      break;
    }
  }

  return StopReason::CyclesExpired;

#undef BR_ABS
#undef OP16
#undef OP8
}

uint8_t Emu6502::ioPeek(uint16_t addr) {
  return 0;
}

void Emu6502::ioPoke(uint16_t addr, uint8_t value) {
  ioPeek(addr);
}
