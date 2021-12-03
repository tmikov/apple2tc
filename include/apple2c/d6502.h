#pragma once

#include <cstdint>

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

/// Instructions.
enum class CPUInstKind : uint8_t {
  // clang-format off
  ADC, AND, ASL, BCC, BCS, BEQ, BIT, BMI, BNE, BPL, BRK, BVC, BVS, CLC, CLD,
  CLI, CLV, CMP, CPX, CPY, DEC, DEX, DEY, EOR, INC, INX, INY, JMP, JSR, LDA,
  LDX, LDY, LSR, NOP, ORA, PHA, PHP, PLA, PLP, ROL, ROR, RTI, RTS, SBC, SEC,
  SED, SEI, STA, STX, STY, TAX, TAY, TSX, TXA, TXS, TYA,
  INVALID
  // clang-format on
};

struct CPUOpcode {
  CPUInstKind kind;
  CPUAddrMode addrMode;

  static constexpr CPUOpcode invalid() {
    return {CPUInstKind::INVALID, CPUAddrMode::A};
  }
};

struct CPUInst {
  CPUInstKind kind;
  CPUAddrMode addrMode;
  uint16_t operand;
  uint8_t size;

  static constexpr CPUInst invalid() {
    return {CPUInstKind::INVALID, CPUAddrMode::A, 0, 1};
  }
};

struct ThreeBytes {
  uint8_t d[3];
};

/// Formatted fields of a decoded instruction for easy printing.
struct FormattedInst {
  /// The instruction size in bytes.
  uint8_t size;
  /// Up to three instruction bytes as hex numbers.
  char bytes[9];
  /// The instruction name.
  char inst[4];
  /// Instruction operand.
  char operand[8];

  FormattedInst() {
    size = 0;
    bytes[0] = 0;
    inst[0] = 0;
    operand[0] = 0;
  }
};

/// Return the instruction name as a C string.
const char *cpuInstName(CPUInstKind kind);
/// Decode an opcode into an instruction and addressing mode. If invalid, the
/// instruction is set to INVALID.
CPUOpcode decodeOpcode(uint8_t opcode);
/// Decode the specified bytes into an instruction, addressing mode and instruction
/// length. If there is an instruction operand, extract and store its value.
/// Note that the maximum instruction length is three bytes.
CPUInst decodeInst(uint16_t pc, ThreeBytes bytes);
/// Format an instruction for easy printing.
FormattedInst formatInst(uint16_t pc, ThreeBytes bytes);
