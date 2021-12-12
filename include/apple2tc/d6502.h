/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <string>

/// 6502 Address Modes.
enum class CPUAddrMode : uint8_t {
  /// OPC A           operand is AC (implied single byte instruction).
  A,
  /// OPC $LLHH       operand is address $HHLL.
  Abs,
  /// OPC $LLHH,X     operand is address; effective address is address incremented by X with carry.
  Abs_X,
  /// OPC $LLHH,Y     operand is address; effective address is address incremented by Y with carry.
  Abs_Y,
  /// OPC #$BB        operand is byte BB.
  Imm,
  /// OPC             operand implied.
  Implied,
  /// OPC ($LLHH)     operand is address; effective address is contents of word at address:
  ///                 C.w($HHLL).
  Ind,
  /// OPC ($LL,X)     operand is zeropage address; effective address is word in (LL + X,LL + X + 1),
  ///                 inc. without carry: C.w($00LL + X).
  X_Ind,
  /// OPC ($LL),Y     operand is zeropage address; effective address is word in (LL, LL + 1)
  ///                 incremented by Y with carry: C.w($00LL) + Y.
  Ind_Y,
  /// OPC $BB         branch target is PC + signed offset BB.
  Rel,
  /// OPC $LL         operand is zeropage address (hi-byte is zero, address = $00LL).
  Zpg,
  /// OPC $LL,X       operand is zeropage address; effective address is address incremented by X
  ///                 without carry.
  Zpg_X,
  /// OPC $LL,Y       operand is zeropage address; effective address is address incremented by Y
  ///                 without carry.
  Zpg_Y,

  /// For validation and counting.
  _last,
  /// Used internally.
  _invalid,
};

/// Return the size of an instruction with the specified address mode.
inline unsigned cpuInstSize(CPUAddrMode am) {
  /// Instruction sizes of each address mode.
  static constexpr uint8_t s_instSize[] = {1, 3, 3, 3, 2, 1, 3, 2, 2, 2, 2, 2, 2};
  static_assert(sizeof(s_instSize) == (unsigned)CPUAddrMode::_last, "Missing address mode");
  return s_instSize[(unsigned)am];
}

/// Return true if the instruction has an explicit operand encoded in memory.
inline bool cpuAddrModeHasOperand(CPUAddrMode am) {
  return am != CPUAddrMode::A && am != CPUAddrMode::Implied;
}

/// Return true if the resolved operand is 8-bit. What does "resolved" mean
/// here? The difference is that while relative branches have an 8-bit operand,
/// the resolved branch target is 16-bit and would return false here.
inline bool cpuAddrModeResolvedOperand8Bit(CPUAddrMode m) {
  switch (m) {
  case CPUAddrMode::Imm:
  case CPUAddrMode::X_Ind:
  case CPUAddrMode::Ind_Y:
  case CPUAddrMode::Zpg:
  case CPUAddrMode::Zpg_X:
  case CPUAddrMode::Zpg_Y:
    return true;
  default:
    return false;
  }
}

/// Return true if the resolved operand is 16-bit. What does "resolved" mean
/// here? The difference is that while relative branches have an 8-bit operand,
/// the resolved branch target is 16-bit and would return true here.
inline bool cpuAddrModeResolvedOperand16Bit(CPUAddrMode m) {
  switch (m) {
  case CPUAddrMode::Abs:
  case CPUAddrMode::Abs_X:
  case CPUAddrMode::Abs_Y:
  case CPUAddrMode::Ind:
  case CPUAddrMode::Rel:
    return true;
  default:
    return false;
  }
}

/// Instructions.
enum class CPUInstKind : uint8_t {
  // clang-format off
  ADC, AND, ASL, BCC, BCS, BEQ, BIT, BMI, BNE, BPL, BRK, BVC, BVS, CLC, CLD,
  CLI, CLV, CMP, CPX, CPY, DEC, DEX, DEY, EOR, INC, INX, INY, JMP, JSR, LDA,
  LDX, LDY, LSR, NOP, ORA, PHA, PHP, PLA, PLP, ROL, ROR, RTI, RTS, SBC, SEC,
  SED, SEI, STA, STX, STY, TAX, TAY, TSX, TXA, TXS, TYA,
  INVALID,
  _last = INVALID,
  // clang-format on
};

struct CPUOpcode {
  CPUInstKind kind;
  CPUAddrMode addrMode;

  static constexpr CPUOpcode invalid() {
    return {CPUInstKind::INVALID, CPUAddrMode::A};
  }
};

/// A struct describing a valid instruction and its encoding. Used for assembly.
struct AsmEncoding {
  CPUOpcode opcode;
  uint8_t encoding;
};

struct CPUInst {
  CPUInstKind kind;
  CPUAddrMode addrMode;
  uint16_t operand;
  uint8_t size;

  [[nodiscard]] bool isInvalid() const {
    return kind == CPUInstKind::INVALID;
  }

  static constexpr CPUInst invalid() {
    return {CPUInstKind::INVALID, CPUAddrMode::A, 0, 1};
  }
};

struct ThreeBytes {
  uint8_t d[3];
};

/// Formatted fields of a decoded instruction for easy printing.
struct FormattedInst {
  /// Up to three instruction bytes as hex numbers.
  char bytes[9];
  /// The instruction name.
  char inst[4];
  /// Instruction operand.
  std::string operand{};

  FormattedInst() {
    bytes[0] = 0;
    inst[0] = 0;
  }
};

/// Return the name of the address mode.
const char *cpuAddrModeName(CPUAddrMode am);
/// Return the instruction name as a C string.
const char *cpuInstName(CPUInstKind kind);
/// Decode an opcode into an instruction and addressing mode. If invalid, the
/// instruction is set to INVALID.
/// This is the "slow" implementation using rules.
CPUOpcode decodeOpcodeSlow(uint8_t opcode);
/// Decode an opcode into an instruction and addressing mode. If invalid, the
/// instruction is set to INVALID.
/// This is the "fast" implementation using a table generated by the "slow" implementation.
CPUOpcode decodeOpcode(uint8_t opcode);
/// Decode the specified bytes into an instruction, addressing mode and instruction
/// length. If there is an instruction operand, extract and store its value.
/// Note that the maximum instruction length is three bytes.
CPUInst decodeInst(uint16_t pc, ThreeBytes bytes);
/// Format a decoded instruction for easy printing.
/// An optional resolver can be supplied to resolve addresses into symbols.
FormattedInst
formatInst(CPUInst inst, ThreeBytes bytes, const std::function<std::string(CPUInst)> &resolve = {});


/// Convert from a string to CPUInstKind.
std::optional<CPUInstKind> findInstKind(const char *name);

/// Return the instruction encoding of the specified instruction kind with the
/// specified address mode.
std::optional<uint8_t> encodeInst(CPUOpcode opcode);
