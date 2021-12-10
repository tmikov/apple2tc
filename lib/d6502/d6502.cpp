/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/d6502.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Code-generated tables.
#include "opcodes.h"

const char *cpuAddrModeName(CPUAddrMode am) {
  static const char s_amName[][8] = {
      "A",
      "Abs",
      "Abs_X",
      "Abs_Y",
      "Imm",
      "Implied",
      "Ind",
      "X_Ind",
      "Ind_Y",
      "Rel",
      "Zpg",
      "Zpg_X",
      "Zpg_Y"};
  static_assert(
      sizeof(s_amName) / sizeof(s_amName[0]) == (unsigned)CPUAddrMode::_last,
      "string table incorrect");
  assert(am < CPUAddrMode::_last);
  return s_amName[(unsigned)am];
}

const char *cpuInstName(CPUInstKind kind) {
  static const char s_instName[][4] = {
      "ADC", "AND", "ASL", "BCC", "BCS", "BEQ", "BIT", "BMI", "BNE", "BPL", "BRK", "BVC",
      "BVS", "CLC", "CLD", "CLI", "CLV", "CMP", "CPX", "CPY", "DEC", "DEX", "DEY", "EOR",
      "INC", "INX", "INY", "JMP", "JSR", "LDA", "LDX", "LDY", "LSR", "NOP", "ORA", "PHA",
      "PHP", "PLA", "PLP", "ROL", "ROR", "RTI", "RTS", "SBC", "SEC", "SED", "SEI", "STA",
      "STX", "STY", "TAX", "TAY", "TSX", "TXA", "TXS", "TYA", "???"};

  return s_instName[(unsigned)kind];
}

CPUOpcode decodeOpcodeSlow(uint8_t opcode) {
  // The 6502 instruction table is laid out according to a pattern a-b-c, where
  // a and b are an octal number each, followed by a group of two binary digits c,
  // as in the bit-vector "aaabbbcc".

  // clang-format off
  static const CPUInstKind a_to_kind_c1[8] = {
      CPUInstKind::ORA, CPUInstKind::AND, CPUInstKind::EOR, CPUInstKind::ADC,
      CPUInstKind::STA, CPUInstKind::LDA, CPUInstKind::CMP, CPUInstKind::SBC
  };
  static const CPUAddrMode b_to_mode_c1[8] = {
      CPUAddrMode::X_Ind, CPUAddrMode::Zpg, CPUAddrMode::Imm, CPUAddrMode::Abs,
      CPUAddrMode::Ind_Y, CPUAddrMode::Zpg_X, CPUAddrMode::Abs_Y, CPUAddrMode::Abs_X
  };

  static const CPUInstKind a_to_kind_c2[8] = {
      CPUInstKind::ASL, CPUInstKind::ROL, CPUInstKind::LSR, CPUInstKind::ROR,
      CPUInstKind::STX, CPUInstKind::LDX, CPUInstKind::DEC, CPUInstKind::INC
  };
  static const CPUAddrMode b_to_mode_c2[8] = {
      CPUAddrMode::_invalid, CPUAddrMode::Zpg, CPUAddrMode::A, CPUAddrMode::Abs,
      CPUAddrMode::_invalid, CPUAddrMode::Zpg_X, CPUAddrMode::_invalid, CPUAddrMode::Abs_X
  };

  static const CPUOpcode ab_to_inst_c0[8][8] = {
      { {CPUInstKind::BRK, CPUAddrMode::Implied}, {CPUInstKind::INVALID, CPUAddrMode::A},
          {CPUInstKind::PHP, CPUAddrMode::Implied}, {CPUInstKind::INVALID, CPUAddrMode::A},
          {CPUInstKind::BPL, CPUAddrMode::Rel}, {CPUInstKind::INVALID, CPUAddrMode::A},
          {CPUInstKind::CLC, CPUAddrMode::Implied}, {CPUInstKind::INVALID, CPUAddrMode::A} },
      { {CPUInstKind::JSR, CPUAddrMode::Abs}, {CPUInstKind::BIT, CPUAddrMode::Zpg},
          {CPUInstKind::PLP, CPUAddrMode::Implied}, {CPUInstKind::BIT, CPUAddrMode::Abs},
          {CPUInstKind::BMI, CPUAddrMode::Rel}, {CPUInstKind::INVALID, CPUAddrMode::A},
          {CPUInstKind::SEC, CPUAddrMode::Implied}, {CPUInstKind::INVALID, CPUAddrMode::A} },
      { {CPUInstKind::RTI, CPUAddrMode::Implied}, {CPUInstKind::INVALID, CPUAddrMode::A},
          {CPUInstKind::PHA, CPUAddrMode::Implied}, {CPUInstKind::JMP, CPUAddrMode::Abs},
          {CPUInstKind::BVC, CPUAddrMode::Rel}, {CPUInstKind::INVALID, CPUAddrMode::A},
          {CPUInstKind::CLI, CPUAddrMode::Implied}, {CPUInstKind::INVALID, CPUAddrMode::A} },
      { {CPUInstKind::RTS, CPUAddrMode::Implied}, {CPUInstKind::INVALID, CPUAddrMode::A},
          {CPUInstKind::PLA, CPUAddrMode::Implied}, {CPUInstKind::JMP, CPUAddrMode::Ind},
          {CPUInstKind::BVS, CPUAddrMode::Rel}, {CPUInstKind::INVALID, CPUAddrMode::A},
          {CPUInstKind::SEI, CPUAddrMode::Implied}, {CPUInstKind::INVALID, CPUAddrMode::A} },
      { {CPUInstKind::INVALID, CPUAddrMode::A}, {CPUInstKind::STY, CPUAddrMode::Zpg},
          {CPUInstKind::DEY, CPUAddrMode::Implied}, {CPUInstKind::STY, CPUAddrMode::Abs},
          {CPUInstKind::BCC, CPUAddrMode::Rel}, {CPUInstKind::STY, CPUAddrMode::Zpg_X},
          {CPUInstKind::TYA, CPUAddrMode::Implied}, {CPUInstKind::INVALID, CPUAddrMode::A} },
      { {CPUInstKind::LDY, CPUAddrMode::Imm}, {CPUInstKind::LDY, CPUAddrMode::Zpg},
          {CPUInstKind::TAY, CPUAddrMode::Implied}, {CPUInstKind::LDY, CPUAddrMode::Abs},
          {CPUInstKind::BCS, CPUAddrMode::Rel}, {CPUInstKind::LDY, CPUAddrMode::Zpg_X},
          {CPUInstKind::CLV, CPUAddrMode::Implied}, {CPUInstKind::LDY, CPUAddrMode::Abs_X} },
      { {CPUInstKind::CPY, CPUAddrMode::Imm}, {CPUInstKind::CPY, CPUAddrMode::Zpg},
          {CPUInstKind::INY, CPUAddrMode::Implied}, {CPUInstKind::CPY, CPUAddrMode::Abs},
          {CPUInstKind::BNE, CPUAddrMode::Rel}, {CPUInstKind::INVALID, CPUAddrMode::A},
          {CPUInstKind::CLD, CPUAddrMode::Implied}, {CPUInstKind::INVALID, CPUAddrMode::A} },
      { {CPUInstKind::CPX, CPUAddrMode::Imm}, {CPUInstKind::CPX, CPUAddrMode::Zpg},
          {CPUInstKind::INX, CPUAddrMode::Implied}, {CPUInstKind::CPX, CPUAddrMode::Abs},
          {CPUInstKind::BEQ, CPUAddrMode::Rel}, {CPUInstKind::INVALID, CPUAddrMode::A},
          {CPUInstKind::SED, CPUAddrMode::Implied}, {CPUInstKind::INVALID, CPUAddrMode::A} },
  };
  // clang-format on

  uint8_t const a = (opcode >> 5) & 7;
  uint8_t const b = (opcode >> 2) & 7;

  // Switch on 'c'.
  switch (opcode & 3) {
  case 0:
    return ab_to_inst_c0[a][b];
  case 1:
    return (a == 4 && b == 2) ? CPUOpcode{CPUInstKind::INVALID, CPUAddrMode::A}
                              : CPUOpcode{a_to_kind_c1[a], b_to_mode_c1[b]};

  case 2:
    // A few more exceptions here.
    switch (a * 8 + b) {
      // a=4, b=2 => TXA impl
    case 4 * 8 + 2:
      return CPUOpcode{CPUInstKind::TXA, CPUAddrMode::Implied};
      // a=4, b=5 => STX zpg,Y
    case 4 * 8 + 5:
      return CPUOpcode{CPUInstKind::STX, CPUAddrMode::Zpg_Y};
      // a=4, b=6 => TXS impl
    case 4 * 8 + 6:
      return CPUOpcode{CPUInstKind::TXS, CPUAddrMode::Implied};
      // a=4, b=7 => _invalid
    case 4 * 8 + 7:
      return CPUOpcode::invalid();
      // a=5, b=0 => LDX #
    case 5 * 8 + 0:
      return CPUOpcode{CPUInstKind::LDX, CPUAddrMode::Imm};
      // a=5, b=2 => TAX impl
    case 5 * 8 + 2:
      return CPUOpcode{CPUInstKind::TAX, CPUAddrMode::Implied};
      // a=5, b=5 => LDX zpg,Y
    case 5 * 8 + 5:
      return CPUOpcode{CPUInstKind::LDX, CPUAddrMode::Zpg_Y};
      // a=5, b=6 => TSX impl
    case 5 * 8 + 6:
      return CPUOpcode{CPUInstKind::TSX, CPUAddrMode::Implied};
      // a=5, b=7 => LDX abs,Y
    case 5 * 8 + 7:
      return CPUOpcode{CPUInstKind::LDX, CPUAddrMode::Abs_Y};
      // a=6, b=2 => DEX impl
    case 6 * 8 + 2:
      return CPUOpcode{CPUInstKind::DEX, CPUAddrMode::Implied};
      // a=7, b=2 => NOP impl
    case 7 * 8 + 2:
      return CPUOpcode{CPUInstKind::NOP, CPUAddrMode::Implied};
    }

    return b_to_mode_c2[b] == CPUAddrMode::_invalid ? CPUOpcode::invalid()
                                                    : CPUOpcode{a_to_kind_c2[a], b_to_mode_c2[b]};
  }

  return CPUOpcode::invalid();
}

CPUOpcode decodeOpcode(uint8_t opcode) {
  return s_opcodes[opcode];
}

CPUInst decodeInst(uint16_t pc, ThreeBytes bytes) {
  CPUOpcode opcode = decodeOpcode(bytes.d[0]);
  if (opcode.kind == CPUInstKind::INVALID) {
    return CPUInst::invalid();
  }

  uint16_t operand = 0;
  uint8_t size = 1;
  switch (opcode.addrMode) {
  case CPUAddrMode::A:
  case CPUAddrMode::Implied:
    break;

  case CPUAddrMode::Abs:
  case CPUAddrMode::Abs_X:
  case CPUAddrMode::Abs_Y:
  case CPUAddrMode::Ind:
    operand = bytes.d[1] + bytes.d[2] * 256;
    size = 3;
    break;

  case CPUAddrMode::Imm:
    operand = bytes.d[1];
    size = 2;
    break;

  case CPUAddrMode::Rel:
    operand = pc + 2 + (int8_t)bytes.d[1];
    size = 2;
    break;

  case CPUAddrMode::Zpg:
  case CPUAddrMode::Zpg_X:
  case CPUAddrMode::Zpg_Y:
  case CPUAddrMode::X_Ind:
  case CPUAddrMode::Ind_Y:
    operand = bytes.d[1];
    size = 2;
    break;
  default:
    assert(false && "Invalid addressing mode");
  }

  return CPUInst{opcode.kind, opcode.addrMode, operand, size};
}

/// A default operand resolve producing only numbers.
static std::string defaultOperandResolver(CPUInst inst) {
  char operand[8];
  if (cpuAddrModeResolvedOperand16Bit(inst.addrMode))
    snprintf(operand, sizeof(operand), "$%04X", inst.operand);
  else if (cpuAddrModeResolvedOperand8Bit(inst.addrMode))
    snprintf(operand, sizeof(operand), "$%02X", inst.operand);
  else
    return {};
  return operand;
}

FormattedInst
formatInst(CPUInst inst, ThreeBytes bytes, const std::function<std::string(CPUInst)> &resolve) {
  FormattedInst res;

  // Format the instruction bytes.
  for (unsigned i = 0, len = 0; i != inst.size; ++i) {
    if (i != 0)
      res.bytes[len++] = ' ';
    len += snprintf(res.bytes + len, sizeof(res.bytes) - len, "%02X", bytes.d[i]);
  }

  if (inst.kind == CPUInstKind::INVALID) {
    strcpy(res.inst, "???");
    return res;
  }

  snprintf(res.inst, sizeof(res.inst), "%s", cpuInstName(inst.kind));

  std::string resolved;
  if (resolve)
    resolved = resolve(inst);
  if (resolved.empty())
    resolved = defaultOperandResolver(inst);

  switch (inst.addrMode) {
  case CPUAddrMode::A:
    res.operand = "A";
    break;
  case CPUAddrMode::Abs:
    res.operand = std::move(resolved);
    break;
  case CPUAddrMode::Abs_X:
    res.operand = resolved + ",X";
    break;
  case CPUAddrMode::Abs_Y:
    res.operand = resolved + ",Y";
    break;
  case CPUAddrMode::Imm:
    res.operand = "#" + resolved;
    break;
  case CPUAddrMode::Implied:
    break;
  case CPUAddrMode::Ind:
    res.operand = "(" + resolved + ")";
    break;
  case CPUAddrMode::X_Ind:
    res.operand = "(" + resolved + ",X)";
    break;
  case CPUAddrMode::Ind_Y:
    res.operand = "(" + resolved + "),Y";
    break;
  case CPUAddrMode::Rel:
    res.operand = std::move(resolved);
    break;
  case CPUAddrMode::Zpg:
    res.operand = std::move(resolved);
    break;
  case CPUAddrMode::Zpg_X:
    res.operand = resolved + ",X";
    break;
  case CPUAddrMode::Zpg_Y:
    res.operand = resolved + ",Y";
    break;
  default:
    break;
  }

  return res;
}

std::optional<CPUInstKind> findInstKind(const char *name) {
  if (strlen(name) != 3)
    return std::nullopt;
  char uname[4];
  for (unsigned i = 0; i != 3; ++i)
    uname[i] = (char)toupper(name[i]);
  uname[3] = 0;

  const auto *res = (const char(*)[4])bsearch(
      uname,
      s_sortedNames,
      (unsigned)CPUInstKind::_last,
      sizeof(s_sortedNames[0]),
      [](const void *key, const void *elem) -> int { return memcmp(key, elem, 4); });
  if (!res)
    return std::nullopt;

  return s_sortedNameKinds[res - s_sortedNames];
}

std::optional<uint8_t> encodeInst(CPUOpcode opcode) {
  const auto *encoding = (const AsmEncoding *)bsearch(
      &opcode,
      s_encodings,
      s_encodings_len,
      sizeof(s_encodings[0]),
      [](const void *_key, const void *_elem) -> int {
        const auto *key = (const CPUOpcode *)_key;
        const auto *elem = (const AsmEncoding *)_elem;
        return (int)key->kind * ((int)CPUAddrMode::_invalid + 2) + (int)key->addrMode -
            ((int)elem->opcode.kind * ((int)CPUAddrMode::_invalid + 2) +
             (int)elem->opcode.addrMode);
      });
  if (!encoding)
    return std::nullopt;
  return encoding->encoding;
}