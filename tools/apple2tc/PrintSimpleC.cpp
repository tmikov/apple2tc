/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// This module generates a straight-forward 1-to1 mapping from Asm to C, without
/// any optimizations.

#include "Disas.h"

#include <cmath>

void Disas::printSimpleCPrologue(FILE *f) {
  fprintf(f, "\n#include \"apple2tc/system-inc.h\"\n\n");

  for (auto const &mr : memRanges_) {
    unsigned len = mr.to - mr.from + 1;
    fprintf(f, "static const uint8_t s_mem_%04x[0x%04x];\n", mr.from, len);
  }

  fprintf(f, "\nvoid init_emulated(void) {\n");
  for (auto const &mr : memRanges_) {
    unsigned len = mr.to - mr.from + 1;
    fprintf(f, "  memcpy(s_ram + 0x%04x, s_mem_%04x, 0x%04x);\n", mr.from, mr.from, len);
  }
  fprintf(f, "  s_pc = 0x%04x;\n", start_);
  fprintf(f, "}\n");
}

void Disas::printSimpleCEpilogue(FILE *f) {
  for (auto const &mr : memRanges_) {
    unsigned len = mr.to - mr.from + 1;
    fprintf(f, "static const uint8_t s_mem_%04x[0x%04x] = {", mr.from, len);
    for (unsigned i = 0; i != len; ++i) {
      if (i % 16 == 0)
        fprintf(f, "\n  ");
      else
        fputc(' ', f);
      fprintf(f, "0x%02X", peek(mr.from + i));
      if (i != len - 1)
        fputc(',', f);
    }
    fputc('\n', f);
    fprintf(f, "};\n");
  }
}

void Disas::printSimpleC(FILE *f) {
  printSimpleCPrologue(f);

  fprintf(f, "\n");
  fprintf(f, "void run_emulated(void) {\n");
  fprintf(f, "  bool branchTarget = true;\n");
  fprintf(f, "  for(;;) {\n");
  fprintf(f, "    uint16_t tmp16;\n");
  fprintf(f, "    uint8_t tmp;\n");
  fprintf(f, "    switch (s_pc) {\n");

  for (Range cr : codeRanges_) {
    for (uint16_t addr = cr.from; addr - 1 != cr.to;) {
      auto it = codeLabels_.find(addr);
      if (it != codeLabels_.end()) {
        // Find the end of this BB in order to approximate execution cycles.
        ++it;
        uint16_t bbLast = it != codeLabels_.end() && it->first - 1 < cr.to ? it->first - 1 : cr.to;
        printf(
            "    case 0x%04x: // [$%04X..$%04X] %4u bytes\n",
            addr,
            addr,
            bbLast,
            bbLast - addr + 1);
        printf("      CYCLES(0x%04x, %ld);\n", addr, lround((bbLast - addr + 1) * 1.7 + 0.5));
      }

      CPUInst inst = decodeInst(addr, peek3(addr));
      printSimpleCInst(f, addr, inst);
      addr += inst.size;
    }
  }

  fprintf(f, "    default:\n");
  fprintf(f, "      fprintf(stderr, \"Unknown code address: $%%04X\\n\", s_pc);\n");
  fprintf(f, "      error_handler(s_pc);\n");
  fprintf(f, "    }\n");
  fprintf(f, "  }\n");
  fprintf(f, "}\n");

  printSimpleCEpilogue(f);
}

void func() {}

void Disas::printSimpleCInst(FILE *f, uint16_t pc, CPUInst inst) {
  scSelfModOperand_ = false;
  scPC_ = pc;

  if (selfModifers_.count(pc))
    fprintf(f, "      // WARNING: performs self modification.\n");

  for (unsigned i = 0, e = cpuInstSize(inst.addrMode); i != e; ++i) {
    auto it = selfModified_.find((uint16_t)(pc + i));
    if (it == selfModified_.end())
      continue;
    if (i == 0) {
      fprintf(f, "      // ERROR: opcode self modification.\n");
      break;
    }
    fprintf(f, "      // WARNING: operand self modification.\n");
    scSelfModOperand_ = true;
    break;
  }

  fprintf(f, "      /* $%04X %s */ ", pc, cpuInstName(inst.kind));
  switch (inst.kind) {
  case CPUInstKind::ADC:
    fprintf(
        f,
        "tmp = %s, s_a = "
        "s_status & STATUS_D"
        " ? adc_decimal(tmp)"
        " : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);",
        simpleCRead(inst).c_str());
    break;
  case CPUInstKind::SBC:
    fprintf(
        f,
        "tmp = %s, s_a = "
        "s_status & STATUS_D"
        " ? sbc_decimal(tmp)"
        " : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);",
        simpleCRead(inst).c_str());
    break;
  case CPUInstKind::AND:
    fprintf(f, "s_a = update_nz(s_a & %s);", simpleCRead(inst).c_str());
    break;
  case CPUInstKind::ORA:
    fprintf(f, "s_a = update_nz(s_a | %s);", simpleCRead(inst).c_str());
    break;
  case CPUInstKind::ASL:
    fprintf(f, "s_a = update_nzc(%s << 1);", simpleCRead(inst).c_str());
    break;
  case CPUInstKind::BCC:
    fprintf(f, "s_pc = !(s_status & STATUS_C) ? 0x%04x : 0x%04x;\n", inst.operand, pc + 2);
    fprintf(f, "      branchTarget = true;\n");
    fprintf(f, "      break;");
    break;
  case CPUInstKind::BCS:
    fprintf(f, "s_pc = s_status & STATUS_C ? 0x%04x : 0x%04x;\n", inst.operand, pc + 2);
    fprintf(f, "      branchTarget = true;\n");
    fprintf(f, "      break;");
    break;
  case CPUInstKind::BEQ:
    fprintf(f, "s_pc = s_status & STATUS_Z ? 0x%04x : 0x%04x;\n", inst.operand, pc + 2);
    fprintf(f, "      branchTarget = true;\n");
    fprintf(f, "      break;");
    break;
  case CPUInstKind::BNE:
    fprintf(f, "s_pc = !(s_status & STATUS_Z) ? 0x%04x : 0x%04x;\n", inst.operand, pc + 2);
    fprintf(f, "      branchTarget = true;\n");
    fprintf(f, "      break;");
    break;
  case CPUInstKind::BMI:
    fprintf(f, "s_pc = s_status & STATUS_N ? 0x%04x : 0x%04x;\n", inst.operand, pc + 2);
    fprintf(f, "      branchTarget = true;\n");
    fprintf(f, "      break;");
    break;
  case CPUInstKind::BPL:
    fprintf(f, "s_pc = !(s_status & STATUS_N) ? 0x%04x : 0x%04x;\n", inst.operand, pc + 2);
    fprintf(f, "      branchTarget = true;\n");
    fprintf(f, "      break;");
    break;
  case CPUInstKind::BVC:
    fprintf(f, "s_pc = !(s_status & STATUS_V) ? 0x%04x : 0x%04x;\n", inst.operand, pc + 2);
    fprintf(f, "      branchTarget = true;\n");
    fprintf(f, "      break;");
    break;
  case CPUInstKind::BVS:
    fprintf(f, "s_pc = s_status & STATUS_V ? 0x%04x : 0x%04x;\n", inst.operand, pc + 2);
    fprintf(f, "      branchTarget = true;\n");
    fprintf(f, "      break;");
    break;
  case CPUInstKind::BIT:
    fprintf(
        f,
        "tmp = %s, "
        "s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);",
        simpleCRead(inst).c_str());
    break;
  case CPUInstKind::BRK:
    fprintf(
        f,
        R"(fprintf(stderr, "Warning: BRK at $%%04X\n", 0x%04x);)"
        "\n",
        pc);
    fprintf(f, "%21s push16(0x%04x);\n", "", (uint16_t)(pc + 2));
    fprintf(f, "%21s push8(s_status | STATUS_B);\n", "");
    fprintf(f, "%21s s_pc = peek16(0xFFFE);\n", "");
    fprintf(f, "      branchTarget = true;\n");
    fprintf(f, "      break;");
    break;

  case CPUInstKind::CLC:
    fprintf(f, "s_status &= ~STATUS_C;");
    break;
  case CPUInstKind::CLD:
    fprintf(f, "s_status &= ~STATUS_D;");
    break;
  case CPUInstKind::CLI:
    fprintf(f, "s_status &= ~STATUS_I;");
    break;
  case CPUInstKind::CLV:
    fprintf(f, "s_status &= ~STATUS_V;");
    break;
  case CPUInstKind::SEC:
    fprintf(f, "s_status |= STATUS_C;");
    break;
  case CPUInstKind::SED:
    fprintf(f, "s_status |= STATUS_D;");
    break;
  case CPUInstKind::SEI:
    fprintf(f, "s_status |= STATUS_I;");
    break;

  case CPUInstKind::CMP:
    fprintf(f, "update_nz_inv_c(s_a - %s);", simpleCRead(inst).c_str());
    break;
  case CPUInstKind::CPX:
    fprintf(f, "update_nz_inv_c(s_x - %s);", simpleCRead(inst).c_str());
    break;
  case CPUInstKind::CPY:
    fprintf(f, "update_nz_inv_c(s_y - %s);", simpleCRead(inst).c_str());
    break;
  case CPUInstKind::DEC:
    fprintf(
        f,
        "tmp16 = %s, %s(tmp16, update_nz(%s(tmp16) - 1));",
        simpleCAddr(inst).c_str(),
        simpleCWriteOp(inst),
        simpleCReadOp(inst));
    break;
  case CPUInstKind::INC:
    fprintf(
        f,
        "tmp16 = %s, %s(tmp16, update_nz(%s(tmp16) + 1));",
        simpleCAddr(inst).c_str(),
        simpleCWriteOp(inst),
        simpleCReadOp(inst));
    break;
  case CPUInstKind::DEX:
    fprintf(f, "s_x = update_nz(s_x - 1);");
    break;
  case CPUInstKind::DEY:
    fprintf(f, "s_y = update_nz(s_y - 1);");
    break;
  case CPUInstKind::INX:
    fprintf(f, "s_x = update_nz(s_x + 1);");
    break;
  case CPUInstKind::INY:
    fprintf(f, "s_y = update_nz(s_y + 1);");
    break;
  case CPUInstKind::EOR:
    fprintf(f, "s_a = update_nz(s_a ^ %s);", simpleCRead(inst).c_str());
    break;

  case CPUInstKind::JMP:
    fprintf(f, "s_pc = %s;\n", simpleCAddr(inst).c_str());
    fprintf(f, "      branchTarget = true;\n");
    fprintf(f, "      break;");
    break;
  case CPUInstKind::JSR:
    fprintf(f, "push16(0x%04x), s_pc = %s;\n", pc + 2, simpleCAddr(inst).c_str());
    fprintf(f, "      branchTarget = true;\n");
    fprintf(f, "      break;");
    break;

  case CPUInstKind::LDA:
    fprintf(f, "s_a = update_nz(%s);", simpleCRead(inst).c_str());
    break;
  case CPUInstKind::LDX:
    fprintf(f, "s_x = update_nz(%s);", simpleCRead(inst).c_str());
    break;
  case CPUInstKind::LDY:
    fprintf(f, "s_y = update_nz(%s);", simpleCRead(inst).c_str());
    break;

  case CPUInstKind::LSR:
    if (inst.addrMode == CPUAddrMode::A) {
      fprintf(f, "set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);");
    } else {
      fprintf(
          f,
          "tmp16 = %s, tmp = %s(tmp16), set_c_to_bit0(tmp), %s(tmp16, update_nz(tmp >> 1));",
          simpleCAddr(inst).c_str(),
          simpleCReadOp(inst),
          simpleCWriteOp(inst));
    }
    break;
  case CPUInstKind::ROL:
    if (inst.addrMode == CPUAddrMode::A) {
      fprintf(
          f,
          "tmp = s_a, "
          "s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), "
          "set_c_to_bit0(tmp >> 7);");
    } else {
      fprintf(
          f,
          "tmp16 = %s, "
          "tmp = %s(tmp16), "
          "%s(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), "
          "set_c_to_bit0(tmp >> 7);",
          simpleCAddr(inst).c_str(),
          simpleCReadOp(inst),
          simpleCWriteOp(inst));
    }
    break;
  case CPUInstKind::ROR:
    if (inst.addrMode == CPUAddrMode::A) {
      fprintf(
          f,
          "tmp = s_a, "
          "s_a = update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7)), "
          "set_c_to_bit0(tmp);");
    } else {
      fprintf(
          f,
          "tmp16 = %s, "
          "tmp = %s(tmp16), "
          "%s(tmp16, update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7))), "
          "set_c_to_bit0(tmp);",
          simpleCAddr(inst).c_str(),
          simpleCReadOp(inst),
          simpleCWriteOp(inst));
    }
    break;

  case CPUInstKind::NOP:
    fprintf(f, "(void)0;");
    break;

  case CPUInstKind::PHA:
    fprintf(f, "push8(s_a);");
    break;
  case CPUInstKind::PHP:
    fprintf(f, "push8(s_status | STATUS_B);");
    break;
  case CPUInstKind::PLA:
    fprintf(f, "s_a = update_nz(pop8());");
    break;
  case CPUInstKind::PLP:
    fprintf(f, "s_status = pop8() & ~STATUS_B;");
    break;

  case CPUInstKind::RTI:
    fprintf(f, "s_status = pop8() & ~STATUS_B, s_pc = pop16();\n");
    fprintf(f, "      branchTarget = true;\n");
    fprintf(f, "      break;");
    break;
  case CPUInstKind::RTS:
    fprintf(f, "s_pc = pop16() + 1;\n");
    fprintf(f, "      branchTarget = true;\n");
    fprintf(f, "      break;");
    break;

  case CPUInstKind::STA:
    fprintf(f, "%s(%s, s_a);", simpleCWriteOp(inst), simpleCAddr(inst).c_str());
    break;
  case CPUInstKind::STX:
    fprintf(f, "%s(%s, s_x);", simpleCWriteOp(inst), simpleCAddr(inst).c_str());
    break;
  case CPUInstKind::STY:
    fprintf(f, "%s(%s, s_y);", simpleCWriteOp(inst), simpleCAddr(inst).c_str());
    break;
  case CPUInstKind::TAX:
    fprintf(f, "s_x = update_nz(s_a);");
    break;
  case CPUInstKind::TAY:
    fprintf(f, "s_y = update_nz(s_a);");
    break;
  case CPUInstKind::TSX:
    fprintf(f, "s_x = update_nz(s_sp);");
    break;
  case CPUInstKind::TXA:
    fprintf(f, "s_a = update_nz(s_x);");
    break;
  case CPUInstKind::TXS:
    fprintf(f, "s_sp = s_x;");
    break;
  case CPUInstKind::TYA:
    fprintf(f, "s_a = update_nz(s_y);");
    break;

  case CPUInstKind::INVALID:
    fprintf(
        f,
        R"(fprintf(stderr, "Warning: INVALID at $%%04X\n", 0x%04x);)"
        "\n",
        pc);
    fprintf(f, "%21s abort();\n", "");
    break;
  }
  fputc('\n', f);
}

std::string Disas::simpleCRead(CPUInst inst) const {
  char buf[32];
  switch (inst.addrMode) {
  case CPUAddrMode::A:
  case CPUAddrMode::Imm:
    return simpleCAddr(inst);
  case CPUAddrMode::Rel:
  case CPUAddrMode::Implied:
    abort();

  case CPUAddrMode::Abs:
  case CPUAddrMode::Abs_X:
  case CPUAddrMode::Abs_Y:
  case CPUAddrMode::Ind:
  case CPUAddrMode::X_Ind:
  case CPUAddrMode::Ind_Y:
    snprintf(buf, sizeof(buf), "peek(%s)", simpleCAddr(inst).c_str());
    return buf;

  case CPUAddrMode::Zpg:
  case CPUAddrMode::Zpg_X:
  case CPUAddrMode::Zpg_Y:
    snprintf(buf, sizeof(buf), "peek_zpg(%s)", simpleCAddr(inst).c_str());
    return buf;

  case CPUAddrMode::_last:
  case CPUAddrMode::_invalid:
  default:
    abort();
  }
}

const char *Disas::simpleCReadOp(CPUInst inst) {
  switch (inst.addrMode) {
  case CPUAddrMode::A:
    return "s_a";

  case CPUAddrMode::Imm:
  case CPUAddrMode::Rel:
  case CPUAddrMode::Implied:
    abort();

  case CPUAddrMode::Abs:
  case CPUAddrMode::Abs_X:
  case CPUAddrMode::Abs_Y:
  case CPUAddrMode::Ind:
  case CPUAddrMode::X_Ind:
  case CPUAddrMode::Ind_Y:
    return "peek";

  case CPUAddrMode::Zpg:
  case CPUAddrMode::Zpg_X:
  case CPUAddrMode::Zpg_Y:
    return "peek_zpg";

  case CPUAddrMode::_last:
  case CPUAddrMode::_invalid:
  default:
    abort();
  }
}

const char *Disas::simpleCWriteOp(CPUInst inst) {
  switch (inst.addrMode) {
  case CPUAddrMode::A:
    return "s_a = ";

  case CPUAddrMode::Imm:
  case CPUAddrMode::Rel:
  case CPUAddrMode::Implied:
    abort();

  case CPUAddrMode::Abs:
  case CPUAddrMode::Abs_X:
  case CPUAddrMode::Abs_Y:
  case CPUAddrMode::Ind:
  case CPUAddrMode::X_Ind:
  case CPUAddrMode::Ind_Y:
    return "poke";

  case CPUAddrMode::Zpg:
  case CPUAddrMode::Zpg_X:
  case CPUAddrMode::Zpg_Y:
    return "poke_zpg";

  case CPUAddrMode::_last:
  case CPUAddrMode::_invalid:
  default:
    abort();
  }
}

std::string Disas::simpleCAddr(CPUInst inst) const {
  char buf[64];
  if (!scSelfModOperand_) {
    switch (inst.addrMode) {
    case CPUAddrMode::A:
      return "s_a";
    case CPUAddrMode::Abs:
      snprintf(buf, sizeof(buf), "0x%04x", inst.operand);
      break;
    case CPUAddrMode::Abs_X:
      snprintf(buf, sizeof(buf), "0x%04x + s_x", inst.operand);
      break;
    case CPUAddrMode::Abs_Y:
      snprintf(buf, sizeof(buf), "0x%04x + s_y", inst.operand);
      break;
    case CPUAddrMode::Imm:
      snprintf(buf, sizeof(buf), "0x%02x", inst.operand);
      break;
    case CPUAddrMode::Implied:
      return "";
    case CPUAddrMode::Ind:
      snprintf(buf, sizeof(buf), "peek16(0x%04x)", inst.operand);
      break;
    case CPUAddrMode::X_Ind:
      snprintf(buf, sizeof(buf), "peek16_zpg(0x%02x + s_x)", inst.operand);
      break;
    case CPUAddrMode::Ind_Y:
      snprintf(buf, sizeof(buf), "peek16_zpg(0x%02x) + s_y", inst.operand);
      break;
    case CPUAddrMode::Rel:
      snprintf(buf, sizeof(buf), "0x%04x", inst.operand);
      break;
    case CPUAddrMode::Zpg:
      snprintf(buf, sizeof(buf), "0x%02x", inst.operand);
      break;
    case CPUAddrMode::Zpg_X:
      snprintf(buf, sizeof(buf), "(uint8_t)(0x%02x + s_x)", inst.operand);
      break;
    case CPUAddrMode::Zpg_Y:
      snprintf(buf, sizeof(buf), "(uint8_t)(0x%02x + s_y)", inst.operand);
      break;
    case CPUAddrMode::_last:
    case CPUAddrMode::_invalid:
    default:
      abort();
    }
  } else {
    switch (inst.addrMode) {
    case CPUAddrMode::A:
      return "s_a";
    case CPUAddrMode::Abs:
      snprintf(buf, sizeof(buf), "ram_peek16(0x%04x)", scPC_ + 1);
      break;
    case CPUAddrMode::Abs_X:
      snprintf(buf, sizeof(buf), "ram_peek16(0x%04x) + s_x", scPC_ + 1);
      break;
    case CPUAddrMode::Abs_Y:
      snprintf(buf, sizeof(buf), "ram_peek16(0x%04x) + s_y", scPC_ + 1);
      break;
    case CPUAddrMode::Imm:
      snprintf(buf, sizeof(buf), "ram_peek(0x%04x)", scPC_ + 1);
      break;
    case CPUAddrMode::Implied:
      return "";
    case CPUAddrMode::Ind:
      snprintf(buf, sizeof(buf), "peek16(ram_peek16(0x%04x))", scPC_ + 1);
      break;
    case CPUAddrMode::X_Ind:
      snprintf(buf, sizeof(buf), "peek16_zpg(ram_peek(0x%04x) + s_x)", scPC_ + 1);
      break;
    case CPUAddrMode::Ind_Y:
      snprintf(buf, sizeof(buf), "peek16_zpg(ram_peek(0x%04x)) + s_y", scPC_ + 1);
      break;
    case CPUAddrMode::Rel:
      snprintf(
          buf,
          sizeof(buf),
          "(uint16_t)(0x%04x + (int8_t)ram_peek(0x%04x))",
          (uint16_t)(scPC_ + 2),
          scPC_ + 1);
      break;
    case CPUAddrMode::Zpg:
      snprintf(buf, sizeof(buf), "ram_peek(0x%04x)", scPC_ + 1);
      break;
    case CPUAddrMode::Zpg_X:
      snprintf(buf, sizeof(buf), "(uint8_t)(ram_peek(0x%04x) + s_x)", scPC_ + 1);
      break;
    case CPUAddrMode::Zpg_Y:
      snprintf(buf, sizeof(buf), "(uint8_t)(ram_peek(0x%04x) + s_y)", scPC_ + 1);
      break;
    case CPUAddrMode::_last:
    case CPUAddrMode::_invalid:
    default:
      abort();
    }
  }
  return buf;
}
