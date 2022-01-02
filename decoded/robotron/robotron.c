// Loaded binary at [$2DFD..$8FFC]
// 17 new runtime blocks added
// code labels: 1899
// data labels: 502

#include "apple2tc/system-inc.h"

static const uint8_t s_mem_2dfd[0x6200];

void init_emulated(void) {
  memcpy(s_ram + 0x2dfd, s_mem_2dfd, 0x6200);
  s_pc = 0x2dfd;
}

void run_emulated(unsigned run_cycles) {
  bool branchTarget = true;
  for(unsigned start_cycles = s_cycles; s_cycles - start_cycles < run_cycles;) {
    uint16_t tmp16;
    uint8_t tmp;
    switch (s_pc) {
    case 0x0140: // [$0140..$0140]    1 bytes
      CYCLES(0x0140, 2);
      /* $0140 ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x0140);
                      error_handler(s_pc);
      break;
    case 0x0143: // [$0143..$0143]    1 bytes
      CYCLES(0x0143, 2);
      /* $0143 ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x0143);
                      error_handler(s_pc);
      break;
    case 0x0146: // [$0146..$0146]    1 bytes
      CYCLES(0x0146, 2);
      /* $0146 ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x0146);
                      error_handler(s_pc);
      break;
    case 0x0149: // [$0149..$0149]    1 bytes
      CYCLES(0x0149, 2);
      /* $0149 ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x0149);
                      error_handler(s_pc);
      break;
    case 0x014c: // [$014C..$014C]    1 bytes
      CYCLES(0x014c, 2);
      /* $014C ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x014c);
                      error_handler(s_pc);
      break;
    case 0x014f: // [$014F..$014F]    1 bytes
      CYCLES(0x014f, 2);
      /* $014F ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x014f);
                      error_handler(s_pc);
      break;
    case 0x0152: // [$0152..$0152]    1 bytes
      CYCLES(0x0152, 2);
      /* $0152 ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x0152);
                      error_handler(s_pc);
      break;
    case 0x0155: // [$0155..$0155]    1 bytes
      CYCLES(0x0155, 2);
      /* $0155 ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x0155);
                      error_handler(s_pc);
      break;
    case 0x2dfd: // [$2DFD..$2DFF]    3 bytes
      CYCLES(0x2dfd, 6);
      /* $2DFD JMP */ s_pc = 0x4000;
      branchTarget = true;
      break;
    case 0x4000: // [$4000..$400B]   12 bytes
      CYCLES(0x4000, 21);
      // ERROR: opcode self modification.
      /* $4000 LDX */ s_x = update_nz(0x3f);
      /* $4002 TXS */ s_sp = s_x;
      /* $4003 LDA */ s_a = update_nz(0x2e);
      /* $4005 LDX */ s_x = update_nz(0x0c);
      /* $4007 LDY */ s_y = update_nz(0x08);
      /* $4009 JSR */ push16(0x400b), s_pc = 0x449d;
      branchTarget = true;
      break;
    case 0x400c: // [$400C..$4014]    9 bytes
      CYCLES(0x400c, 16);
      /* $400C LDA */ s_a = update_nz(0x3a);
      /* $400E LDX */ s_x = update_nz(0x06);
      /* $4010 LDY */ s_y = update_nz(0x01);
      /* $4012 JSR */ push16(0x4014), s_pc = 0x449d;
      branchTarget = true;
      break;
    case 0x4015: // [$4015..$401A]    6 bytes
      CYCLES(0x4015, 11);
      /* $4015 LDX */ s_x = update_nz(0x3f);
      /* $4017 TXS */ s_sp = s_x;
      /* $4018 JSR */ push16(0x401a), s_pc = 0x512f;
      branchTarget = true;
      break;
    case 0x401b: // [$401B..$401E]    4 bytes
      CYCLES(0x401b, 7);
      /* $401B LDA */ s_a = update_nz(0xea);
      /* $401D LDX */ s_x = update_nz(0x14);
    case 0x401f: // [$401F..$4024]    6 bytes
      CYCLES(0x401f, 11);
      // WARNING: performs self modification.
      /* $401F STA */ poke(0x4000 + s_x, s_a);
      /* $4022 DEX */ s_x = update_nz(s_x - 1);
      /* $4023 BPL */ s_pc = !(s_status & STATUS_N) ? 0x401f : 0x4025;
      branchTarget = true;
      break;
    case 0x4025: // [$4025..$405F]   59 bytes
      CYCLES(0x4025, 101);
      /* $4025 LDA */ s_a = update_nz(0x12);
      /* $4027 STA */ poke_zpg(0xfd, s_a);
      /* $4029 LDA */ s_a = update_nz(0x13);
      /* $402B STA */ poke_zpg(0xff, s_a);
      /* $402D LDA */ s_a = update_nz(peek(0xc050));
      /* $4030 LDA */ s_a = update_nz(peek(0xc052));
      /* $4033 LDA */ s_a = update_nz(peek(0xc054));
      /* $4036 LDA */ s_a = update_nz(peek(0xc057));
      /* $4039 LDA */ s_a = update_nz(0x00);
      /* $403B STA */ poke(0x1407, s_a);
      /* $403E STA */ poke(0x150c, s_a);
      /* $4041 STA */ poke(0x1432, s_a);
      /* $4044 STA */ poke(0x1433, s_a);
      /* $4047 STA */ poke(0x1434, s_a);
      /* $404A LDA */ s_a = update_nz(peek(0x0c10));
      /* $404D STA */ poke(0x150a, s_a);
      /* $4050 LDA */ s_a = update_nz(peek(0x0c11));
      /* $4053 STA */ poke(0x150b, s_a);
      /* $4056 LDA */ s_a = update_nz(0x01);
      /* $4058 STA */ poke(0x1417, s_a);
      /* $405B LDA */ s_a = update_nz(0x06);
      /* $405D STA */ poke(0x1509, s_a);
    case 0x4060: // [$4060..$4065]    6 bytes
      CYCLES(0x4060, 11);
      /* $4060 LDX */ s_x = update_nz(0x3f);
      /* $4062 TXS */ s_sp = s_x;
      /* $4063 JSR */ push16(0x4065), s_pc = 0x4504;
      branchTarget = true;
      break;
    case 0x4066: // [$4066..$4068]    3 bytes
      CYCLES(0x4066, 6);
      /* $4066 JSR */ push16(0x4068), s_pc = 0x4b67;
      branchTarget = true;
      break;
    case 0x4069: // [$4069..$4089]   33 bytes
      CYCLES(0x4069, 57);
      /* $4069 LDA */ s_a = update_nz(peek(0x0c6a));
      /* $406C STA */ poke(0x1509, s_a);
      /* $406F LDA */ s_a = update_nz(0x00);
      /* $4071 STA */ poke(0x1407, s_a);
      /* $4074 STA */ poke(0x150a, s_a);
      /* $4077 STA */ poke(0x150b, s_a);
      /* $407A STA */ poke(0x150c, s_a);
      /* $407D LDX */ s_x = update_nz(peek(0x0c10));
      /* $4080 DEX */ s_x = update_nz(s_x - 1);
      /* $4081 STX */ poke(0x150d, s_x);
      /* $4084 LDX */ s_x = update_nz(peek(0x0c11));
      /* $4087 STX */ poke(0x150e, s_x);
    case 0x408a: // [$408A..$408C]    3 bytes
      CYCLES(0x408a, 6);
      /* $408A JSR */ push16(0x408c), s_pc = 0x442a;
      branchTarget = true;
      break;
    case 0x408d: // [$408D..$408F]    3 bytes
      CYCLES(0x408d, 6);
      /* $408D JSR */ push16(0x408f), s_pc = 0x43c0;
      branchTarget = true;
      break;
    case 0x4090: // [$4090..$4095]    6 bytes
      CYCLES(0x4090, 11);
      /* $4090 LDX */ s_x = update_nz(0x3f);
      /* $4092 TXS */ s_sp = s_x;
      /* $4093 JSR */ push16(0x4095), s_pc = 0x426b;
      branchTarget = true;
      break;
    case 0x4096: // [$4096..$4097]    2 bytes
      CYCLES(0x4096, 4);
      /* $4096 BCC */ s_pc = !(s_status & STATUS_C) ? 0x40ac : 0x4098;
      branchTarget = true;
      break;
    case 0x4098: // [$4098..$409A]    3 bytes
      CYCLES(0x4098, 6);
      /* $4098 JSR */ push16(0x409a), s_pc = 0x4d4a;
      branchTarget = true;
      break;
    case 0x409b: // [$409B..$40A4]   10 bytes
      CYCLES(0x409b, 18);
      /* $409B INC */ tmp16 = 0x1407, poke(tmp16, update_nz(peek(tmp16) + 1));
      /* $409E LDA */ s_a = update_nz(peek(0x1407));
      /* $40A1 CMP */ update_nz_inv_c(s_a - 0x64);
      /* $40A3 BCC */ s_pc = !(s_status & STATUS_C) ? 0x408a : 0x40a5;
      branchTarget = true;
      break;
    case 0x40a5: // [$40A5..$40AB]    7 bytes
      CYCLES(0x40a5, 12);
      /* $40A5 LDA */ s_a = update_nz(0x00);
      /* $40A7 STA */ poke(0x1407, s_a);
      /* $40AA BEQ */ s_pc = s_status & STATUS_Z ? 0x408a : 0x40ac;
      branchTarget = true;
      break;
    case 0x40ac: // [$40AC..$40AE]    3 bytes
      CYCLES(0x40ac, 6);
      /* $40AC JSR */ push16(0x40ae), s_pc = 0x430e;
      branchTarget = true;
      break;
    case 0x40af: // [$40AF..$40B1]    3 bytes
      CYCLES(0x40af, 6);
      /* $40AF JSR */ push16(0x40b1), s_pc = 0x4110;
      branchTarget = true;
      break;
    case 0x40b2: // [$40B2..$40B4]    3 bytes
      CYCLES(0x40b2, 6);
      /* $40B2 JSR */ push16(0x40b4), s_pc = 0x425b;
      branchTarget = true;
      break;
    case 0x40b5: // [$40B5..$40B7]    3 bytes
      CYCLES(0x40b5, 6);
      /* $40B5 JSR */ push16(0x40b7), s_pc = 0x4110;
      branchTarget = true;
      break;
    case 0x40b8: // [$40B8..$40BA]    3 bytes
      CYCLES(0x40b8, 6);
      /* $40B8 JSR */ push16(0x40ba), s_pc = 0x4171;
      branchTarget = true;
      break;
    case 0x40bb: // [$40BB..$40BD]    3 bytes
      CYCLES(0x40bb, 6);
      /* $40BB JSR */ push16(0x40bd), s_pc = 0x5209;
      branchTarget = true;
      break;
    case 0x40be: // [$40BE..$40C0]    3 bytes
      CYCLES(0x40be, 6);
      /* $40BE JSR */ push16(0x40c0), s_pc = 0x5a1b;
      branchTarget = true;
      break;
    case 0x40c1: // [$40C1..$40C3]    3 bytes
      CYCLES(0x40c1, 6);
      /* $40C1 JSR */ push16(0x40c3), s_pc = 0x5206;
      branchTarget = true;
      break;
    case 0x40c4: // [$40C4..$40C6]    3 bytes
      CYCLES(0x40c4, 6);
      /* $40C4 JSR */ push16(0x40c6), s_pc = 0x5203;
      branchTarget = true;
      break;
    case 0x40c7: // [$40C7..$40C8]    2 bytes
      CYCLES(0x40c7, 4);
      /* $40C7 BCC */ s_pc = !(s_status & STATUS_C) ? 0x40d7 : 0x40c9;
      branchTarget = true;
      break;
    case 0x40c9: // [$40C9..$40CB]    3 bytes
      CYCLES(0x40c9, 6);
      /* $40C9 JSR */ push16(0x40cb), s_pc = 0x4ef1;
      branchTarget = true;
      break;
    case 0x40cc: // [$40CC..$40D0]    5 bytes
      CYCLES(0x40cc, 9);
      /* $40CC DEC */ tmp16 = 0x1509, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $40CF BNE */ s_pc = !(s_status & STATUS_Z) ? 0x408d : 0x40d1;
      branchTarget = true;
      break;
    case 0x40d1: // [$40D1..$40D3]    3 bytes
      CYCLES(0x40d1, 6);
      /* $40D1 JSR */ push16(0x40d3), s_pc = 0x6215;
      branchTarget = true;
      break;
    case 0x40d4: // [$40D4..$40D6]    3 bytes
      CYCLES(0x40d4, 6);
      /* $40D4 JMP */ s_pc = 0x4060;
      branchTarget = true;
      break;
    case 0x40d7: // [$40D7..$40D9]    3 bytes
      CYCLES(0x40d7, 6);
      /* $40D7 JSR */ push16(0x40d9), s_pc = 0x5a1e;
      branchTarget = true;
      break;
    case 0x40da: // [$40DA..$40DC]    3 bytes
      CYCLES(0x40da, 6);
      /* $40DA JSR */ push16(0x40dc), s_pc = 0x4110;
      branchTarget = true;
      break;
    case 0x40dd: // [$40DD..$40DF]    3 bytes
      CYCLES(0x40dd, 6);
      /* $40DD JSR */ push16(0x40df), s_pc = 0x5a09;
      branchTarget = true;
      break;
    case 0x40e0: // [$40E0..$40E2]    3 bytes
      CYCLES(0x40e0, 6);
      /* $40E0 JSR */ push16(0x40e2), s_pc = 0x5a0c;
      branchTarget = true;
      break;
    case 0x40e3: // [$40E3..$40E5]    3 bytes
      CYCLES(0x40e3, 6);
      /* $40E3 JSR */ push16(0x40e5), s_pc = 0x5a0f;
      branchTarget = true;
      break;
    case 0x40e6: // [$40E6..$40E8]    3 bytes
      CYCLES(0x40e6, 6);
      /* $40E6 JSR */ push16(0x40e8), s_pc = 0x4110;
      branchTarget = true;
      break;
    case 0x40e9: // [$40E9..$40EB]    3 bytes
      CYCLES(0x40e9, 6);
      /* $40E9 JSR */ push16(0x40eb), s_pc = 0x6203;
      branchTarget = true;
      break;
    case 0x40ec: // [$40EC..$40EE]    3 bytes
      CYCLES(0x40ec, 6);
      /* $40EC JSR */ push16(0x40ee), s_pc = 0x6206;
      branchTarget = true;
      break;
    case 0x40ef: // [$40EF..$40F1]    3 bytes
      CYCLES(0x40ef, 6);
      /* $40EF JSR */ push16(0x40f1), s_pc = 0x6209;
      branchTarget = true;
      break;
    case 0x40f2: // [$40F2..$40F4]    3 bytes
      CYCLES(0x40f2, 6);
      /* $40F2 JSR */ push16(0x40f4), s_pc = 0x4110;
      branchTarget = true;
      break;
    case 0x40f5: // [$40F5..$40F7]    3 bytes
      CYCLES(0x40f5, 6);
      /* $40F5 JSR */ push16(0x40f7), s_pc = 0x7003;
      branchTarget = true;
      break;
    case 0x40f8: // [$40F8..$40FA]    3 bytes
      CYCLES(0x40f8, 6);
      /* $40F8 JSR */ push16(0x40fa), s_pc = 0x7006;
      branchTarget = true;
      break;
    case 0x40fb: // [$40FB..$40FD]    3 bytes
      CYCLES(0x40fb, 6);
      /* $40FB JSR */ push16(0x40fd), s_pc = 0x7009;
      branchTarget = true;
      break;
    case 0x40fe: // [$40FE..$4100]    3 bytes
      CYCLES(0x40fe, 6);
      /* $40FE JSR */ push16(0x4100), s_pc = 0x7009;
      branchTarget = true;
      break;
    case 0x4101: // [$4101..$4103]    3 bytes
      CYCLES(0x4101, 6);
      /* $4101 JSR */ push16(0x4103), s_pc = 0x4110;
      branchTarget = true;
      break;
    case 0x4104: // [$4104..$4106]    3 bytes
      CYCLES(0x4104, 6);
      /* $4104 JSR */ push16(0x4106), s_pc = 0x6903;
      branchTarget = true;
      break;
    case 0x4107: // [$4107..$4109]    3 bytes
      CYCLES(0x4107, 6);
      /* $4107 JSR */ push16(0x4109), s_pc = 0x6906;
      branchTarget = true;
      break;
    case 0x410a: // [$410A..$410C]    3 bytes
      CYCLES(0x410a, 6);
      /* $410A JSR */ push16(0x410c), s_pc = 0x6909;
      branchTarget = true;
      break;
    case 0x410d: // [$410D..$410F]    3 bytes
      CYCLES(0x410d, 6);
      /* $410D JMP */ s_pc = 0x4090;
      branchTarget = true;
      break;
    case 0x4110: // [$4110..$4116]    7 bytes
      CYCLES(0x4110, 12);
      /* $4110 LDA */ s_a = update_nz(peek(0x1417));
      /* $4113 CMP */ update_nz_inv_c(s_a - 0x01);
      /* $4115 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x412b : 0x4117;
      branchTarget = true;
      break;
    case 0x4117: // [$4117..$4119]    3 bytes
      CYCLES(0x4117, 6);
      /* $4117 JSR */ push16(0x4119), s_pc = 0x41e3;
      branchTarget = true;
      break;
    case 0x411a: // [$411A..$411E]    5 bytes
      CYCLES(0x411a, 9);
      /* $411A LDA */ s_a = update_nz(peek(0x0a00 + s_y));
      /* $411D BMI */ s_pc = s_status & STATUS_N ? 0x4122 : 0x411f;
      branchTarget = true;
      break;
    case 0x411f: // [$411F..$4121]    3 bytes
      CYCLES(0x411f, 6);
      /* $411F STA */ poke(0x1506, s_a);
    case 0x4122: // [$4122..$4126]    5 bytes
      CYCLES(0x4122, 9);
      /* $4122 LDA */ s_a = update_nz(peek(0x0a80 + s_y));
      /* $4125 BMI */ s_pc = s_status & STATUS_N ? 0x412a : 0x4127;
      branchTarget = true;
      break;
    case 0x4127: // [$4127..$4129]    3 bytes
      CYCLES(0x4127, 6);
      /* $4127 STA */ poke(0x1416, s_a);
    case 0x412a: // [$412A..$412A]    1 bytes
      CYCLES(0x412a, 2);
      /* $412A RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x412b: // [$412B..$412D]    3 bytes
      CYCLES(0x412b, 6);
      /* $412B TAX */ s_x = update_nz(s_a);
      /* $412C BNE */ s_pc = !(s_status & STATUS_Z) ? 0x416e : 0x412e;
      branchTarget = true;
      break;
    case 0x412e: // [$412E..$4132]    5 bytes
      CYCLES(0x412e, 9);
      /* $412E LDA */ s_a = update_nz(peek(0xc062));
      /* $4131 BPL */ s_pc = !(s_status & STATUS_N) ? 0x4150 : 0x4133;
      branchTarget = true;
      break;
    case 0x4133: // [$4133..$413E]   12 bytes
      CYCLES(0x4133, 21);
      /* $4133 LDA */ s_a = update_nz(peek(0x1435));
      /* $4136 CLC */ s_status &= ~STATUS_C;
      /* $4137 ADC */ tmp = peek(0x0c6b), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $413A STA */ poke(0x1435, s_a);
      /* $413D BCC */ s_pc = !(s_status & STATUS_C) ? 0x4150 : 0x413f;
      branchTarget = true;
      break;
    case 0x413f: // [$413F..$414F]   17 bytes
      CYCLES(0x413f, 29);
      /* $413F LDX */ s_x = update_nz(peek(0x1436));
      /* $4142 LDA */ s_a = update_nz(peek(0x0d5e + s_x));
      /* $4145 ADC */ tmp = 0x00, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4147 AND */ s_a = update_nz(s_a & 0x07);
      /* $4149 TAX */ s_x = update_nz(s_a);
      /* $414A LDA */ s_a = update_nz(peek(0x0d1e + s_x));
      /* $414D STA */ poke(0x1436, s_a);
    case 0x4150: // [$4150..$4156]    7 bytes
      CYCLES(0x4150, 12);
      /* $4150 LDA */ s_a = update_nz(0x00);
      /* $4152 LDY */ s_y = update_nz(peek(0xc061));
      /* $4155 BPL */ s_pc = !(s_status & STATUS_N) ? 0x4168 : 0x4157;
      branchTarget = true;
      break;
    case 0x4157: // [$4157..$415E]    8 bytes
      CYCLES(0x4157, 14);
      /* $4157 LDA */ s_a = update_nz(peek(0x1436));
      /* $415A LDX */ s_x = update_nz(peek(0x1431));
      /* $415D BMI */ s_pc = s_status & STATUS_N ? 0x4168 : 0x415f;
      branchTarget = true;
      break;
    case 0x415f: // [$415F..$4163]    5 bytes
      CYCLES(0x415f, 9);
      /* $415F LDX */ s_x = update_nz(peek(0x1506));
      /* $4162 BEQ */ s_pc = s_status & STATUS_Z ? 0x4168 : 0x4164;
      branchTarget = true;
      break;
    case 0x4164: // [$4164..$4167]    4 bytes
      CYCLES(0x4164, 7);
      /* $4164 STX */ poke(0x1436, s_x);
      /* $4167 TXA */ s_a = update_nz(s_x);
    case 0x4168: // [$4168..$416D]    6 bytes
      CYCLES(0x4168, 11);
      /* $4168 STA */ poke(0x1416, s_a);
      /* $416B STY */ poke(0x1431, s_y);
    case 0x416e: // [$416E..$4170]    3 bytes
      CYCLES(0x416e, 6);
      /* $416E JMP */ s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x4171: // [$4171..$4177]    7 bytes
      CYCLES(0x4171, 12);
      /* $4171 LDA */ s_a = update_nz(peek(0x1417));
      /* $4174 CMP */ update_nz_inv_c(s_a - 0x03);
      /* $4176 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x41b4 : 0x4178;
      branchTarget = true;
      break;
    case 0x4178: // [$4178..$417A]    3 bytes
      CYCLES(0x4178, 6);
      /* $4178 JSR */ push16(0x417a), s_pc = 0x4185;
      branchTarget = true;
      break;
    case 0x417b: // [$417B..$417D]    3 bytes
      CYCLES(0x417b, 6);
      /* $417B JSR */ push16(0x417d), s_pc = 0x41e3;
      branchTarget = true;
      break;
    case 0x417e: // [$417E..$4184]    7 bytes
      CYCLES(0x417e, 12);
      /* $417E LDA */ s_a = update_nz(peek(0x0b00 + s_y));
      /* $4181 STA */ poke(0x1416, s_a);
      /* $4184 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4185: // [$4185..$418B]    7 bytes
      CYCLES(0x4185, 12);
      /* $4185 LDA */ s_a = update_nz(0x03);
      /* $4187 LDX */ s_x = update_nz(peek(0x1508));
      /* $418A BNE */ s_pc = !(s_status & STATUS_Z) ? 0x418e : 0x418c;
      branchTarget = true;
      break;
    case 0x418c: // [$418C..$418D]    2 bytes
      CYCLES(0x418c, 4);
      /* $418C EOR */ s_a = update_nz(s_a ^ 0x0f);
    case 0x418e: // [$418E..$4196]    9 bytes
      CYCLES(0x418e, 16);
      /* $418E AND */ s_a = update_nz(s_a & peek(0x1506));
      /* $4191 STA */ poke(0x1506, s_a);
      /* $4194 JSR */ push16(0x4196), s_pc = 0xfb1e;
      branchTarget = true;
      break;
    case 0x4197: // [$4197..$419C]    6 bytes
      CYCLES(0x4197, 11);
      /* $4197 LDX */ s_x = update_nz(0x00);
      /* $4199 CPY */ update_nz_inv_c(s_y - 0xc0);
      /* $419B BCC */ s_pc = !(s_status & STATUS_C) ? 0x419e : 0x419d;
      branchTarget = true;
      break;
    case 0x419d: // [$419D..$419D]    1 bytes
      CYCLES(0x419d, 2);
      /* $419D INX */ s_x = update_nz(s_x + 1);
    case 0x419e: // [$419E..$41A1]    4 bytes
      CYCLES(0x419e, 7);
      /* $419E CPY */ update_nz_inv_c(s_y - 0x40);
      /* $41A0 BCS */ s_pc = s_status & STATUS_C ? 0x41a3 : 0x41a2;
      branchTarget = true;
      break;
    case 0x41a2: // [$41A2..$41A2]    1 bytes
      CYCLES(0x41a2, 2);
      /* $41A2 DEX */ s_x = update_nz(s_x - 1);
    case 0x41a3: // [$41A3..$41AA]    8 bytes
      CYCLES(0x41a3, 14);
      /* $41A3 TXA */ s_a = update_nz(s_x);
      /* $41A4 AND */ s_a = update_nz(s_a & 0x03);
      /* $41A6 LDX */ s_x = update_nz(peek(0x1508));
      /* $41A9 BEQ */ s_pc = s_status & STATUS_Z ? 0x41ad : 0x41ab;
      branchTarget = true;
      break;
    case 0x41ab: // [$41AB..$41AC]    2 bytes
      CYCLES(0x41ab, 4);
      /* $41AB ASL */ s_a = update_nzc(s_a << 1);
      /* $41AC ASL */ s_a = update_nzc(s_a << 1);
    case 0x41ad: // [$41AD..$41B3]    7 bytes
      CYCLES(0x41ad, 12);
      /* $41AD ORA */ s_a = update_nz(s_a | peek(0x1506));
      /* $41B0 STA */ poke(0x1506, s_a);
      /* $41B3 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x41b4: // [$41B4..$41B7]    4 bytes
      CYCLES(0x41b4, 7);
      /* $41B4 CMP */ update_nz_inv_c(s_a - 0x02);
      /* $41B6 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x41d9 : 0x41b8;
      branchTarget = true;
      break;
    case 0x41b8: // [$41B8..$41BD]    6 bytes
      CYCLES(0x41b8, 11);
      /* $41B8 LDX */ s_x = update_nz(peek(0x1508));
      /* $41BB JSR */ push16(0x41bd), s_pc = 0xfb1e;
      branchTarget = true;
      break;
    case 0x41be: // [$41BE..$41CC]   15 bytes
      CYCLES(0x41be, 26);
      /* $41BE TYA */ s_a = update_nz(s_y);
      /* $41BF LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $41C0 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $41C1 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $41C2 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $41C3 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $41C4 TAY */ s_y = update_nz(s_a);
      /* $41C5 LDA */ s_a = update_nz(peek(0x0d1e + s_y));
      /* $41C8 LDX */ s_x = update_nz(peek(0x1508));
      /* $41CB BNE */ s_pc = !(s_status & STATUS_Z) ? 0x41d2 : 0x41cd;
      branchTarget = true;
      break;
    case 0x41cd: // [$41CD..$41D1]    5 bytes
      CYCLES(0x41cd, 9);
      /* $41CD STA */ poke(0x1506, s_a);
      /* $41D0 BEQ */ s_pc = s_status & STATUS_Z ? 0x41d5 : 0x41d2;
      branchTarget = true;
      break;
    case 0x41d2: // [$41D2..$41D4]    3 bytes
      CYCLES(0x41d2, 6);
      /* $41D2 STA */ poke(0x1416, s_a);
    case 0x41d5: // [$41D5..$41D7]    3 bytes
      CYCLES(0x41d5, 6);
      /* $41D5 JSR */ push16(0x41d7), s_pc = 0x41e3;
      branchTarget = true;
      break;
    case 0x41d8: // [$41D8..$41D8]    1 bytes
      CYCLES(0x41d8, 2);
      /* $41D8 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x41d9: // [$41D9..$41DB]    3 bytes
      CYCLES(0x41d9, 6);
      /* $41D9 TAX */ s_x = update_nz(s_a);
      /* $41DA BNE */ s_pc = !(s_status & STATUS_Z) ? 0x41e2 : 0x41dc;
      branchTarget = true;
      break;
    case 0x41dc: // [$41DC..$41DE]    3 bytes
      CYCLES(0x41dc, 6);
      /* $41DC JSR */ push16(0x41de), s_pc = 0x4185;
      branchTarget = true;
      break;
    case 0x41df: // [$41DF..$41E1]    3 bytes
      CYCLES(0x41df, 6);
      /* $41DF JSR */ push16(0x41e1), s_pc = 0x41e3;
      branchTarget = true;
      break;
    case 0x41e2: // [$41E2..$41E2]    1 bytes
      CYCLES(0x41e2, 2);
      /* $41E2 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x41e3: // [$41E3..$41E7]    5 bytes
      CYCLES(0x41e3, 9);
      /* $41E3 LDA */ s_a = update_nz(peek(0xc000));
      /* $41E6 BMI */ s_pc = s_status & STATUS_N ? 0x41eb : 0x41e8;
      branchTarget = true;
      break;
    case 0x41e8: // [$41E8..$41EA]    3 bytes
      CYCLES(0x41e8, 6);
      /* $41E8 PLA */ s_a = update_nz(pop8());
      /* $41E9 PLA */ s_a = update_nz(pop8());
      /* $41EA RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x41eb: // [$41EB..$41ED]    3 bytes
      CYCLES(0x41eb, 6);
      /* $41EB JSR */ push16(0x41ed), s_pc = 0x4242;
      branchTarget = true;
      break;
    case 0x41ee: // [$41EE..$41F1]    4 bytes
      CYCLES(0x41ee, 7);
      /* $41EE CPY */ update_nz_inv_c(s_y - 0x1b);
      /* $41F0 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x41f9 : 0x41f2;
      branchTarget = true;
      break;
    case 0x41f2: // [$41F2..$41F6]    5 bytes
      CYCLES(0x41f2, 9);
      /* $41F2 LDA */ s_a = update_nz(peek(0xc000));
      /* $41F5 BPL */ s_pc = !(s_status & STATUS_N) ? 0x41f2 : 0x41f7;
      branchTarget = true;
      break;
    case 0x41f7: // [$41F7..$41F8]    2 bytes
      CYCLES(0x41f7, 4);
      /* $41F7 BMI */ s_pc = s_status & STATUS_N ? 0x41e8 : 0x41f9;
      branchTarget = true;
      break;
    case 0x41f9: // [$41F9..$41FC]    4 bytes
      CYCLES(0x41f9, 7);
      /* $41F9 CPY */ update_nz_inv_c(s_y - 0x13);
      /* $41FB BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4207 : 0x41fd;
      branchTarget = true;
      break;
    case 0x41fd: // [$41FD..$4206]   10 bytes
      CYCLES(0x41fd, 18);
      /* $41FD LDA */ s_a = update_nz(peek(0x4c6e));
      /* $4200 EOR */ s_a = update_nz(s_a ^ 0xc5);
      // WARNING: performs self modification.
      /* $4202 STA */ poke(0x4c6e, s_a);
      /* $4205 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x41e8 : 0x4207;
      branchTarget = true;
      break;
    case 0x4207: // [$4207..$420A]    4 bytes
      CYCLES(0x4207, 7);
      /* $4207 CPY */ update_nz_inv_c(s_y - 0x12);
      /* $4209 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4236 : 0x420b;
      branchTarget = true;
      break;
    case 0x420b: // [$420B..$420D]    3 bytes
      CYCLES(0x420b, 6);
      /* $420B JSR */ push16(0x420d), s_pc = 0x424e;
      branchTarget = true;
      break;
    case 0x420e: // [$420E..$420F]    2 bytes
      CYCLES(0x420e, 4);
      /* $420E BCS */ s_pc = s_status & STATUS_C ? 0x41e8 : 0x4210;
      branchTarget = true;
      break;
    case 0x4210: // [$4210..$4216]    7 bytes
      CYCLES(0x4210, 12);
      /* $4210 STA */ poke_zpg(0x07, s_a);
      /* $4212 LDX */ s_x = update_nz(0x0a);
      /* $4214 JSR */ push16(0x4216), s_pc = 0x4c4f;
      branchTarget = true;
      break;
    case 0x4217: // [$4217..$421B]    5 bytes
      CYCLES(0x4217, 9);
      /* $4217 STX */ poke_zpg(0x06, s_x);
      /* $4219 JSR */ push16(0x421b), s_pc = 0x424e;
      branchTarget = true;
      break;
    case 0x421c: // [$421C..$421D]    2 bytes
      CYCLES(0x421c, 4);
      /* $421C BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4222 : 0x421e;
      branchTarget = true;
      break;
    case 0x421e: // [$421E..$4221]    4 bytes
      CYCLES(0x421e, 7);
      /* $421E LDA */ s_a = update_nz(peek_zpg(0x07));
      /* $4220 BPL */ s_pc = !(s_status & STATUS_N) ? 0x4227 : 0x4222;
      branchTarget = true;
      break;
    case 0x4222: // [$4222..$4223]    2 bytes
      CYCLES(0x4222, 4);
      /* $4222 BCS */ s_pc = s_status & STATUS_C ? 0x41e8 : 0x4224;
      branchTarget = true;
      break;
    case 0x4224: // [$4224..$4226]    3 bytes
      CYCLES(0x4224, 6);
      /* $4224 CLC */ s_status &= ~STATUS_C;
      /* $4225 ADC */ tmp = peek_zpg(0x06), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0x4227: // [$4227..$422B]    5 bytes
      CYCLES(0x4227, 9);
      /* $4227 SEC */ s_status |= STATUS_C;
      /* $4228 SBC */ tmp = 0x01, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $422A BCC */ s_pc = !(s_status & STATUS_C) ? 0x41e8 : 0x422c;
      branchTarget = true;
      break;
    case 0x422c: // [$422C..$4235]   10 bytes
      CYCLES(0x422c, 18);
      /* $422C STA */ poke(0x1407, s_a);
      /* $422F PLA */ s_a = update_nz(pop8());
      /* $4230 PLA */ s_a = update_nz(pop8());
      /* $4231 PLA */ s_a = update_nz(pop8());
      /* $4232 PLA */ s_a = update_nz(pop8());
      /* $4233 JMP */ s_pc = 0x408a;
      branchTarget = true;
      break;
    case 0x4236: // [$4236..$4239]    4 bytes
      CYCLES(0x4236, 7);
      /* $4236 CMP */ update_nz_inv_c(s_a - 0x11);
      /* $4238 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4241 : 0x423a;
      branchTarget = true;
      break;
    case 0x423a: // [$423A..$4240]    7 bytes
      CYCLES(0x423a, 12);
      /* $423A PLA */ s_a = update_nz(pop8());
      /* $423B PLA */ s_a = update_nz(pop8());
      /* $423C PLA */ s_a = update_nz(pop8());
      /* $423D PLA */ s_a = update_nz(pop8());
      /* $423E JMP */ s_pc = 0x4060;
      branchTarget = true;
      break;
    case 0x4241: // [$4241..$4241]    1 bytes
      CYCLES(0x4241, 2);
      /* $4241 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4242: // [$4242..$4246]    5 bytes
      CYCLES(0x4242, 9);
      /* $4242 LDA */ s_a = update_nz(peek(0xc000));
      /* $4245 BPL */ s_pc = !(s_status & STATUS_N) ? 0x4242 : 0x4247;
      branchTarget = true;
      break;
    case 0x4247: // [$4247..$424D]    7 bytes
      CYCLES(0x4247, 12);
      /* $4247 STA */ poke(0xc010, s_a);
      /* $424A AND */ s_a = update_nz(s_a & 0x7f);
      /* $424C TAY */ s_y = update_nz(s_a);
      /* $424D RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x424e: // [$424E..$4250]    3 bytes
      CYCLES(0x424e, 6);
      /* $424E JSR */ push16(0x4250), s_pc = 0x4242;
      branchTarget = true;
      break;
    case 0x4251: // [$4251..$4254]    4 bytes
      CYCLES(0x4251, 7);
      /* $4251 CMP */ update_nz_inv_c(s_a - 0x0d);
      /* $4253 BEQ */ s_pc = s_status & STATUS_Z ? 0x425a : 0x4255;
      branchTarget = true;
      break;
    case 0x4255: // [$4255..$4259]    5 bytes
      CYCLES(0x4255, 9);
      /* $4255 SEC */ s_status |= STATUS_C;
      /* $4256 SBC */ tmp = 0x30, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $4258 CMP */ update_nz_inv_c(s_a - 0x0a);
    case 0x425a: // [$425A..$425A]    1 bytes
      CYCLES(0x425a, 2);
      /* $425A RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x425b: // [$425B..$425E]    4 bytes
      CYCLES(0x425b, 7);
      /* $425B LDX */ s_x = update_nz(peek(0x1411));
      /* $425E INX */ s_x = update_nz(s_x + 1);
    case 0x425f: // [$425F..$4260]    2 bytes
      CYCLES(0x425f, 4);
      /* $425F LDY */ s_y = update_nz(0x40);
    case 0x4261: // [$4261..$4263]    3 bytes
      CYCLES(0x4261, 6);
      /* $4261 DEY */ s_y = update_nz(s_y - 1);
      /* $4262 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4261 : 0x4264;
      branchTarget = true;
      break;
    case 0x4264: // [$4264..$4266]    3 bytes
      CYCLES(0x4264, 6);
      /* $4264 JSR */ push16(0x4266), s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x4267: // [$4267..$4269]    3 bytes
      CYCLES(0x4267, 6);
      /* $4267 DEX */ s_x = update_nz(s_x - 1);
      /* $4268 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x425f : 0x426a;
      branchTarget = true;
      break;
    case 0x426a: // [$426A..$426A]    1 bytes
      CYCLES(0x426a, 2);
      /* $426A RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x426b: // [$426B..$426F]    5 bytes
      CYCLES(0x426b, 9);
      /* $426B DEC */ tmp16 = 0x1404, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $426E BMI */ s_pc = s_status & STATUS_N ? 0x4272 : 0x4270;
      branchTarget = true;
      break;
    case 0x4270: // [$4270..$4271]    2 bytes
      CYCLES(0x4270, 4);
      /* $4270 CLC */ s_status &= ~STATUS_C;
      /* $4271 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4272: // [$4272..$427F]   14 bytes
      CYCLES(0x4272, 24);
      /* $4272 LDA */ s_a = update_nz(0x0a);
      /* $4274 STA */ poke(0x1404, s_a);
      /* $4277 LDA */ s_a = update_nz(peek(0x1402));
      /* $427A LDX */ s_x = update_nz(peek(0x1403));
      /* $427D JSR */ push16(0x427f), s_pc = 0x4c00;
      branchTarget = true;
      break;
    case 0x4280: // [$4280..$42B5]   54 bytes
      CYCLES(0x4280, 92);
      /* $4280 ASL */ s_a = update_nzc(s_a << 1);
      /* $4281 STA */ poke_zpg(0x00, s_a);
      /* $4283 LDA */ s_a = update_nz(peek(0x140e));
      /* $4286 ADC */ tmp = peek(0x140f), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4289 ADC */ tmp = peek(0x141b), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $428C ADC */ tmp = peek(0x141d), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $428F ASL */ s_a = update_nzc(s_a << 1);
      /* $4290 ADC */ tmp = peek_zpg(0x00), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4292 STA */ poke_zpg(0x00, s_a);
      /* $4294 LDA */ s_a = update_nz(peek(0x1409));
      /* $4297 ADC */ tmp = peek(0x1410), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $429A ADC */ tmp = peek(0x1418), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $429D ADC */ tmp = peek(0x1427), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $42A0 ADC */ tmp = peek(0x1428), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $42A3 ADC */ tmp = peek(0x1429), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $42A6 ADC */ tmp = peek_zpg(0x00), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $42A8 STA */ poke_zpg(0x00, s_a);
      /* $42AA LDA */ s_a = update_nz(peek(0x140a));
      /* $42AD LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $42AE CLC */ s_status &= ~STATUS_C;
      /* $42AF ADC */ tmp = peek_zpg(0x00), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $42B1 LDX */ s_x = update_nz(peek(0x141c));
      /* $42B4 BEQ */ s_pc = s_status & STATUS_Z ? 0x42b9 : 0x42b6;
      branchTarget = true;
      break;
    case 0x42b6: // [$42B6..$42B8]    3 bytes
      CYCLES(0x42b6, 6);
      /* $42B6 ADC */ tmp = peek(0x1418), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0x42b9: // [$42B9..$42C2]   10 bytes
      CYCLES(0x42b9, 18);
      /* $42B9 STA */ poke_zpg(0x00, s_a);
      /* $42BB LDA */ s_a = update_nz(peek_zpg(0x00));
      /* $42BD LDX */ s_x = update_nz(peek(0x0c57));
      /* $42C0 JSR */ push16(0x42c2), s_pc = 0x4c4f;
      branchTarget = true;
      break;
    case 0x42c3: // [$42C3..$42CC]   10 bytes
      CYCLES(0x42c3, 18);
      /* $42C3 STX */ poke_zpg(0x00, s_x);
      /* $42C5 LDA */ s_a = update_nz(peek(0x0c01));
      /* $42C8 SEC */ s_status |= STATUS_C;
      /* $42C9 SBC */ tmp = peek_zpg(0x00), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $42CB BCS */ s_pc = s_status & STATUS_C ? 0x42cf : 0x42cd;
      branchTarget = true;
      break;
    case 0x42cd: // [$42CD..$42CE]    2 bytes
      CYCLES(0x42cd, 4);
      /* $42CD LDA */ s_a = update_nz(0x00);
    case 0x42cf: // [$42CF..$42D9]   11 bytes
      CYCLES(0x42cf, 19);
      /* $42CF STA */ poke(0x1411, s_a);
      /* $42D2 LDA */ s_a = update_nz(peek(0x1403));
      /* $42D5 CMP */ update_nz_inv_c(s_a - peek(0x0c0d));
      /* $42D8 BCC */ s_pc = !(s_status & STATUS_C) ? 0x42dd : 0x42da;
      branchTarget = true;
      break;
    case 0x42da: // [$42DA..$42DC]    3 bytes
      CYCLES(0x42da, 6);
      /* $42DA DEC */ tmp16 = 0x1403, poke(tmp16, update_nz(peek(tmp16) - 1));
    case 0x42dd: // [$42DD..$42E6]   10 bytes
      CYCLES(0x42dd, 18);
      /* $42DD LDA */ s_a = update_nz(peek(0x0c14));
      /* $42E0 LDX */ s_x = update_nz(peek(0x140f));
      /* $42E3 INX */ s_x = update_nz(s_x + 1);
      /* $42E4 JSR */ push16(0x42e6), s_pc = 0x4c00;
      branchTarget = true;
      break;
    case 0x42e7: // [$42E7..$42F3]   13 bytes
      CYCLES(0x42e7, 23);
      /* $42E7 STA */ poke(0x1425, s_a);
      /* $42EA LDA */ s_a = update_nz(peek(0x0c55));
      /* $42ED LDX */ s_x = update_nz(peek(0x1428));
      /* $42F0 INX */ s_x = update_nz(s_x + 1);
      /* $42F1 JSR */ push16(0x42f3), s_pc = 0x4c00;
      branchTarget = true;
      break;
    case 0x42f4: // [$42F4..$430B]   24 bytes
      CYCLES(0x42f4, 41);
      /* $42F4 STA */ poke(0x142b, s_a);
      /* $42F7 CLC */ s_status &= ~STATUS_C;
      /* $42F8 LDA */ s_a = update_nz(peek(0x1402));
      /* $42FB ORA */ s_a = update_nz(s_a | peek(0x140f));
      /* $42FE ORA */ s_a = update_nz(s_a | peek(0x140e));
      /* $4301 ORA */ s_a = update_nz(s_a | peek(0x1418));
      /* $4304 ORA */ s_a = update_nz(s_a | peek(0x1427));
      /* $4307 ORA */ s_a = update_nz(s_a | peek(0x1428));
      /* $430A BNE */ s_pc = !(s_status & STATUS_Z) ? 0x430d : 0x430c;
      branchTarget = true;
      break;
    case 0x430c: // [$430C..$430C]    1 bytes
      CYCLES(0x430c, 2);
      /* $430C SEC */ s_status |= STATUS_C;
    case 0x430d: // [$430D..$430D]    1 bytes
      CYCLES(0x430d, 2);
      /* $430D RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x430e: // [$430E..$4317]   10 bytes
      CYCLES(0x430e, 18);
      /* $430E LDX */ s_x = update_nz(0x00);
      /* $4310 LDA */ s_a = update_nz(peek(0x1406));
      /* $4313 CMP */ update_nz_inv_c(s_a - peek(0x1423));
      /* $4316 BCC */ s_pc = !(s_status & STATUS_C) ? 0x431f : 0x4318;
      branchTarget = true;
      break;
    case 0x4318: // [$4318..$431E]    7 bytes
      CYCLES(0x4318, 12);
      /* $4318 SBC */ tmp = peek(0x1422), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $431B STA */ poke(0x1406, s_a);
      /* $431E TAX */ s_x = update_nz(s_a);
    case 0x431f: // [$431F..$4323]    5 bytes
      CYCLES(0x431f, 9);
      /* $431F LDA */ s_a = update_nz(peek(0x1412));
      /* $4322 BMI */ s_pc = s_status & STATUS_N ? 0x432a : 0x4324;
      branchTarget = true;
      break;
    case 0x4324: // [$4324..$4329]    6 bytes
      CYCLES(0x4324, 11);
      /* $4324 DEC */ tmp16 = 0x1412, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $4327 LDX */ s_x = update_nz(peek(0x1413));
    case 0x432a: // [$432A..$432E]    5 bytes
      CYCLES(0x432a, 9);
      /* $432A LDA */ s_a = update_nz(peek(0x1414));
      /* $432D BMI */ s_pc = s_status & STATUS_N ? 0x433c : 0x432f;
      branchTarget = true;
      break;
    case 0x432f: // [$432F..$433B]   13 bytes
      CYCLES(0x432f, 23);
      /* $432F DEC */ tmp16 = 0x1414, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $4332 LDA */ s_a = update_nz(peek(0x1415));
      /* $4335 ADC */ tmp = peek(0x0c21), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4338 STA */ poke(0x1415, s_a);
      /* $433B TAX */ s_x = update_nz(s_a);
    case 0x433c: // [$433C..$4340]    5 bytes
      CYCLES(0x433c, 9);
      /* $433C LDA */ s_a = update_nz(peek(0x142e));
      /* $433F BMI */ s_pc = s_status & STATUS_N ? 0x435b : 0x4341;
      branchTarget = true;
      break;
    case 0x4341: // [$4341..$434F]   15 bytes
      CYCLES(0x4341, 26);
      /* $4341 LDA */ s_a = update_nz(peek(0x142f));
      /* $4344 TAX */ s_x = update_nz(s_a);
      /* $4345 SBC */ tmp = peek(0x0c65), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $4348 STA */ poke(0x142f, s_a);
      /* $434B CMP */ update_nz_inv_c(s_a - peek(0x0c66));
      /* $434E BCS */ s_pc = s_status & STATUS_C ? 0x435b : 0x4350;
      branchTarget = true;
      break;
    case 0x4350: // [$4350..$4352]    3 bytes
      CYCLES(0x4350, 6);
      /* $4350 LDA */ s_a = update_nz(peek(0x0c64));
    case 0x4353: // [$4353..$435A]    8 bytes
      CYCLES(0x4353, 14);
      /* $4353 STA */ poke(0x142f, s_a);
      /* $4356 DEC */ tmp16 = 0x142e, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $4359 BPL */ s_pc = !(s_status & STATUS_N) ? 0x4341 : 0x435b;
      branchTarget = true;
      break;
    case 0x435b: // [$435B..$435F]    5 bytes
      CYCLES(0x435b, 9);
      /* $435B LDA */ s_a = update_nz(peek(0x141e));
      /* $435E BEQ */ s_pc = s_status & STATUS_Z ? 0x437e : 0x4360;
      branchTarget = true;
      break;
    case 0x4360: // [$4360..$436D]   14 bytes
      CYCLES(0x4360, 24);
      /* $4360 LDA */ s_a = update_nz(peek(0x141f));
      /* $4363 SBC */ tmp = peek(0x0c30), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $4366 STA */ poke(0x141f, s_a);
      /* $4369 CMP */ update_nz_inv_c(s_a - peek(0x0c3f));
      /* $436C BCS */ s_pc = s_status & STATUS_C ? 0x437d : 0x436e;
      branchTarget = true;
      break;
    case 0x436e: // [$436E..$4372]    5 bytes
      CYCLES(0x436e, 9);
      /* $436E DEC */ tmp16 = 0x141e, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $4371 BEQ */ s_pc = s_status & STATUS_Z ? 0x437e : 0x4373;
      branchTarget = true;
      break;
    case 0x4373: // [$4373..$437C]   10 bytes
      CYCLES(0x4373, 18);
      /* $4373 LDA */ s_a = update_nz(peek(0x0c31));
      /* $4376 SEC */ s_status |= STATUS_C;
      /* $4377 SBC */ tmp = peek(0x0c2c), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $437A STA */ poke(0x141f, s_a);
    case 0x437d: // [$437D..$437D]    1 bytes
      CYCLES(0x437d, 2);
      /* $437D TAX */ s_x = update_nz(s_a);
    case 0x437e: // [$437E..$4382]    5 bytes
      CYCLES(0x437e, 9);
      /* $437E LDA */ s_a = update_nz(peek(0x1419));
      /* $4381 BEQ */ s_pc = s_status & STATUS_Z ? 0x4390 : 0x4383;
      branchTarget = true;
      break;
    case 0x4383: // [$4383..$438F]   13 bytes
      CYCLES(0x4383, 23);
      /* $4383 DEC */ tmp16 = 0x1419, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $4386 LDA */ s_a = update_nz(peek(0x141a));
      /* $4389 EOR */ s_a = update_nz(s_a ^ peek(0x0c27));
      /* $438C STA */ poke(0x141a, s_a);
      /* $438F TAX */ s_x = update_nz(s_a);
    case 0x4390: // [$4390..$4394]    5 bytes
      CYCLES(0x4390, 9);
      /* $4390 LDA */ s_a = update_nz(peek(0x140b));
      /* $4393 BPL */ s_pc = !(s_status & STATUS_N) ? 0x439b : 0x4395;
      branchTarget = true;
      break;
    case 0x4395: // [$4395..$439A]    6 bytes
      CYCLES(0x4395, 11);
      /* $4395 ADC */ tmp = 0x06, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4397 STA */ poke(0x140b, s_a);
      /* $439A TAX */ s_x = update_nz(s_a);
    case 0x439b: // [$439B..$43A1]    7 bytes
      CYCLES(0x439b, 12);
      /* $439B LDA */ s_a = update_nz(peek(0x140c));
      /* $439E CMP */ update_nz_inv_c(s_a - 0xc0);
      /* $43A0 BCC */ s_pc = !(s_status & STATUS_C) ? 0x43a8 : 0x43a2;
      branchTarget = true;
      break;
    case 0x43a2: // [$43A2..$43A7]    6 bytes
      CYCLES(0x43a2, 11);
      /* $43A2 SBC */ tmp = 0x02, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $43A4 STA */ poke(0x140c, s_a);
      /* $43A7 TAX */ s_x = update_nz(s_a);
    case 0x43a8: // [$43A8..$43AC]    5 bytes
      CYCLES(0x43a8, 9);
      /* $43A8 LDA */ s_a = update_nz(peek(0x141c));
      /* $43AB BEQ */ s_pc = s_status & STATUS_Z ? 0x43b0 : 0x43ad;
      branchTarget = true;
      break;
    case 0x43ad: // [$43AD..$43AF]    3 bytes
      CYCLES(0x43ad, 6);
      /* $43AD EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $43AF TAX */ s_x = update_nz(s_a);
    case 0x43b0: // [$43B0..$43B4]    5 bytes
      CYCLES(0x43b0, 9);
      /* $43B0 LDY */ s_y = update_nz(peek(0x1430));
      /* $43B3 BMI */ s_pc = s_status & STATUS_N ? 0x43bb : 0x43b5;
      branchTarget = true;
      break;
    case 0x43b5: // [$43B5..$43BA]    6 bytes
      CYCLES(0x43b5, 11);
      /* $43B5 LDX */ s_x = update_nz(peek(0x0d46 + s_y));
      /* $43B8 DEC */ tmp16 = 0x1430, poke(tmp16, update_nz(peek(tmp16) - 1));
    case 0x43bb: // [$43BB..$43BF]    5 bytes
      CYCLES(0x43bb, 9);
      /* $43BB STX */ poke_zpg(0xca, s_x);
      /* $43BD JMP */ s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x43c0: // [$43C0..$43C2]    3 bytes
      CYCLES(0x43c0, 6);
      /* $43C0 JSR */ push16(0x43c2), s_pc = 0x4f42;
      branchTarget = true;
      break;
    case 0x43c3: // [$43C3..$43C4]    2 bytes
      CYCLES(0x43c3, 4);
      /* $43C3 LDX */ s_x = update_nz(0x00);
    case 0x43c5: // [$43C5..$43C9]    5 bytes
      CYCLES(0x43c5, 9);
      /* $43C5 LDY */ s_y = update_nz(peek(0x0c80 + s_x));
      /* $43C8 BMI */ s_pc = s_status & STATUS_N ? 0x43e5 : 0x43ca;
      branchTarget = true;
      break;
    case 0x43ca: // [$43CA..$43CA]    1 bytes
      CYCLES(0x43ca, 2);
      /* $43CA INX */ s_x = update_nz(s_x + 1);
    case 0x43cb: // [$43CB..$43D2]    8 bytes
      CYCLES(0x43cb, 14);
      /* $43CB LDA */ s_a = update_nz(peek(0x0c80 + s_x));
      /* $43CE INX */ s_x = update_nz(s_x + 1);
      /* $43CF CMP */ update_nz_inv_c(s_a - 0x80);
      /* $43D1 BCS */ s_pc = s_status & STATUS_C ? 0x43c5 : 0x43d3;
      branchTarget = true;
      break;
    case 0x43d3: // [$43D3..$43D9]    7 bytes
      CYCLES(0x43d3, 12);
      /* $43D3 SBC */ tmp = 0x00, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $43D5 CMP */ update_nz_inv_c(s_a - peek(0x1407));
      /* $43D8 BEQ */ s_pc = s_status & STATUS_Z ? 0x43dc : 0x43da;
      branchTarget = true;
      break;
    case 0x43da: // [$43DA..$43DB]    2 bytes
      CYCLES(0x43da, 4);
      /* $43DA BCS */ s_pc = s_status & STATUS_C ? 0x43e2 : 0x43dc;
      branchTarget = true;
      break;
    case 0x43dc: // [$43DC..$43E1]    6 bytes
      CYCLES(0x43dc, 11);
      /* $43DC LDA */ s_a = update_nz(peek(0x0c80 + s_x));
      /* $43DF STA */ poke(0x0c00 + s_y, s_a);
    case 0x43e2: // [$43E2..$43E4]    3 bytes
      CYCLES(0x43e2, 6);
      /* $43E2 INX */ s_x = update_nz(s_x + 1);
      /* $43E3 BPL */ s_pc = !(s_status & STATUS_N) ? 0x43cb : 0x43e5;
      branchTarget = true;
      break;
    case 0x43e5: // [$43E5..$440A]   38 bytes
      CYCLES(0x43e5, 65);
      /* $43E5 LDA */ s_a = update_nz(0x00);
      /* $43E7 STA */ poke(0x1400, s_a);
      /* $43EA STA */ poke_zpg(0xca, s_a);
      /* $43EC STA */ poke(0x1406, s_a);
      /* $43EF STA */ poke(0x1401, s_a);
      /* $43F2 STA */ poke(0x1416, s_a);
      /* $43F5 STA */ poke(0x1404, s_a);
      /* $43F8 STA */ poke(0x1435, s_a);
      /* $43FB LDA */ s_a = update_nz(0x0c);
      /* $43FD STA */ poke(0x1436, s_a);
      /* $4400 LDA */ s_a = update_nz(0xff);
      /* $4402 STA */ poke(0x1412, s_a);
      /* $4405 STA */ poke(0x1430, s_a);
      /* $4408 JSR */ push16(0x440a), s_pc = 0x5200;
      branchTarget = true;
      break;
    case 0x440b: // [$440B..$440D]    3 bytes
      CYCLES(0x440b, 6);
      /* $440B JSR */ push16(0x440d), s_pc = 0x5a00;
      branchTarget = true;
      break;
    case 0x440e: // [$440E..$4410]    3 bytes
      CYCLES(0x440e, 6);
      /* $440E JSR */ push16(0x4410), s_pc = 0x7000;
      branchTarget = true;
      break;
    case 0x4411: // [$4411..$4418]    8 bytes
      CYCLES(0x4411, 14);
      /* $4411 LDA */ s_a = update_nz(peek(0x0c02));
      /* $4414 CMP */ update_nz_inv_c(s_a - peek(0x1421));
      /* $4417 BCC */ s_pc = !(s_status & STATUS_C) ? 0x441e : 0x4419;
      branchTarget = true;
      break;
    case 0x4419: // [$4419..$441D]    5 bytes
      CYCLES(0x4419, 9);
      /* $4419 ADC */ tmp = 0x00, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $441B STA */ poke(0x1421, s_a);
    case 0x441e: // [$441E..$4420]    3 bytes
      CYCLES(0x441e, 6);
      /* $441E JSR */ push16(0x4420), s_pc = 0x5a03;
      branchTarget = true;
      break;
    case 0x4421: // [$4421..$4423]    3 bytes
      CYCLES(0x4421, 6);
      /* $4421 JSR */ push16(0x4423), s_pc = 0x5a06;
      branchTarget = true;
      break;
    case 0x4424: // [$4424..$4426]    3 bytes
      CYCLES(0x4424, 6);
      /* $4424 JSR */ push16(0x4426), s_pc = 0x6200;
      branchTarget = true;
      break;
    case 0x4427: // [$4427..$4429]    3 bytes
      CYCLES(0x4427, 6);
      /* $4427 JMP */ s_pc = 0x6900;
      branchTarget = true;
      break;
    case 0x442a: // [$442A..$4448]   31 bytes
      CYCLES(0x442a, 53);
      /* $442A LDY */ s_y = update_nz(peek(0x1407));
      /* $442D LDA */ s_a = update_nz(peek(0x0e00 + s_y));
      /* $4430 STA */ poke(0x1402, s_a);
      /* $4433 LDA */ s_a = update_nz(peek(0x0e80 + s_y));
      /* $4436 STA */ poke(0x1409, s_a);
      /* $4439 LDA */ s_a = update_nz(peek(0x0f80 + s_y));
      /* $443C STA */ poke_zpg(0x01, s_a);
      /* $443E LDA */ s_a = update_nz(peek(0x1000 + s_y));
      /* $4441 STA */ poke_zpg(0x02, s_a);
      /* $4443 LDX */ s_x = update_nz(peek(0x0f00 + s_y));
      /* $4446 LDY */ s_y = update_nz(0x00);
      /* $4448 TYA */ s_a = update_nz(s_y);
    case 0x4449: // [$4449..$444B]    3 bytes
      CYCLES(0x4449, 6);
      /* $4449 DEX */ s_x = update_nz(s_x - 1);
      /* $444A BMI */ s_pc = s_status & STATUS_N ? 0x4452 : 0x444c;
      branchTarget = true;
      break;
    case 0x444c: // [$444C..$4451]    6 bytes
      CYCLES(0x444c, 11);
      /* $444C STA */ poke(0x1990 + s_y, s_a);
      /* $444F INY */ s_y = update_nz(s_y + 1);
      /* $4450 BPL */ s_pc = !(s_status & STATUS_N) ? 0x4449 : 0x4452;
      branchTarget = true;
      break;
    case 0x4452: // [$4452..$4455]    4 bytes
      CYCLES(0x4452, 7);
      /* $4452 LDX */ s_x = update_nz(peek_zpg(0x01));
      /* $4454 LDA */ s_a = update_nz(0x01);
    case 0x4456: // [$4456..$4458]    3 bytes
      CYCLES(0x4456, 6);
      /* $4456 DEX */ s_x = update_nz(s_x - 1);
      /* $4457 BMI */ s_pc = s_status & STATUS_N ? 0x445f : 0x4459;
      branchTarget = true;
      break;
    case 0x4459: // [$4459..$445E]    6 bytes
      CYCLES(0x4459, 11);
      /* $4459 STA */ poke(0x1990 + s_y, s_a);
      /* $445C INY */ s_y = update_nz(s_y + 1);
      /* $445D BPL */ s_pc = !(s_status & STATUS_N) ? 0x4456 : 0x445f;
      branchTarget = true;
      break;
    case 0x445f: // [$445F..$4462]    4 bytes
      CYCLES(0x445f, 7);
      /* $445F LDX */ s_x = update_nz(peek_zpg(0x02));
      /* $4461 LDA */ s_a = update_nz(0x02);
    case 0x4463: // [$4463..$4465]    3 bytes
      CYCLES(0x4463, 6);
      /* $4463 DEX */ s_x = update_nz(s_x - 1);
      /* $4464 BMI */ s_pc = s_status & STATUS_N ? 0x446c : 0x4466;
      branchTarget = true;
      break;
    case 0x4466: // [$4466..$446B]    6 bytes
      CYCLES(0x4466, 11);
      /* $4466 STA */ poke(0x1990 + s_y, s_a);
      /* $4469 INY */ s_y = update_nz(s_y + 1);
      /* $446A BPL */ s_pc = !(s_status & STATUS_N) ? 0x4463 : 0x446c;
      branchTarget = true;
      break;
    case 0x446c: // [$446C..$448D]   34 bytes
      CYCLES(0x446c, 58);
      /* $446C STY */ poke(0x140a, s_y);
      /* $446F LDY */ s_y = update_nz(peek(0x1407));
      /* $4472 LDA */ s_a = update_nz(peek(0x1080 + s_y));
      /* $4475 STA */ poke(0x140f, s_a);
      /* $4478 LDA */ s_a = update_nz(0x00);
      /* $447A STA */ poke(0x140e, s_a);
      /* $447D LDA */ s_a = update_nz(peek(0x1100 + s_y));
      /* $4480 STA */ poke(0x1418, s_a);
      /* $4483 LDA */ s_a = update_nz(peek(0x1180 + s_y));
      /* $4486 STA */ poke(0x1427, s_a);
      /* $4489 LDX */ s_x = update_nz(0x00);
      /* $448B LDA */ s_a = update_nz(peek(0x0c51));
    case 0x448e: // [$448E..$4496]    9 bytes
      CYCLES(0x448e, 16);
      /* $448E STA */ poke(0x1d40 + s_x, s_a);
      /* $4491 INX */ s_x = update_nz(s_x + 1);
      /* $4492 CPX */ update_nz_inv_c(s_x - peek(0x1427));
      /* $4495 BCC */ s_pc = !(s_status & STATUS_C) ? 0x448e : 0x4497;
      branchTarget = true;
      break;
    case 0x4497: // [$4497..$449C]    6 bytes
      CYCLES(0x4497, 11);
      /* $4497 LDA */ s_a = update_nz(0x00);
      /* $4499 STA */ poke(0x1428, s_a);
      /* $449C RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x449d: // [$449D..$44AC]   16 bytes
      CYCLES(0x449d, 28);
      /* $449D STA */ poke_zpg(0x07, s_a);
      /* $449F STY */ poke_zpg(0x09, s_y);
      /* $44A1 LDY */ s_y = update_nz(0x00);
      /* $44A3 STY */ poke_zpg(0x06, s_y);
      /* $44A5 STY */ poke_zpg(0x08, s_y);
      /* $44A7 PLA */ s_a = update_nz(pop8());
      /* $44A8 STA */ poke_zpg(0x01, s_a);
      /* $44AA PLA */ s_a = update_nz(pop8());
      /* $44AB STA */ poke_zpg(0x00, s_a);
    case 0x44ad: // [$44AD..$44B3]    7 bytes
      CYCLES(0x44ad, 12);
      /* $44AD LDA */ s_a = update_nz(peek(peek16_zpg(0x06) + s_y));
      /* $44AF STA */ poke(peek16_zpg(0x08) + s_y, s_a);
      /* $44B1 INY */ s_y = update_nz(s_y + 1);
      /* $44B2 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x44ad : 0x44b4;
      branchTarget = true;
      break;
    case 0x44b4: // [$44B4..$44BA]    7 bytes
      CYCLES(0x44b4, 12);
      /* $44B4 INC */ tmp16 = 0x07, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $44B6 INC */ tmp16 = 0x09, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $44B8 DEX */ s_x = update_nz(s_x - 1);
      /* $44B9 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x44ad : 0x44bb;
      branchTarget = true;
      break;
    case 0x44bb: // [$44BB..$44C1]    7 bytes
      CYCLES(0x44bb, 12);
      /* $44BB LDA */ s_a = update_nz(peek_zpg(0x00));
      /* $44BD PHA */ push8(s_a);
      /* $44BE LDA */ s_a = update_nz(peek_zpg(0x01));
      /* $44C0 PHA */ push8(s_a);
      /* $44C1 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x44c2: // [$44C2..$44C4]    3 bytes
      CYCLES(0x44c2, 6);
      /* $44C2 JSR */ push16(0x44c4), s_pc = 0x426b;
      branchTarget = true;
      break;
    case 0x44c5: // [$44C5..$44C7]    3 bytes
      CYCLES(0x44c5, 6);
      /* $44C5 JSR */ push16(0x44c7), s_pc = 0x425b;
      branchTarget = true;
      break;
    case 0x44c8: // [$44C8..$44CA]    3 bytes
      CYCLES(0x44c8, 6);
      /* $44C8 JSR */ push16(0x44ca), s_pc = 0x5209;
      branchTarget = true;
      break;
    case 0x44cb: // [$44CB..$44CD]    3 bytes
      CYCLES(0x44cb, 6);
      /* $44CB JSR */ push16(0x44cd), s_pc = 0x5a1b;
      branchTarget = true;
      break;
    case 0x44ce: // [$44CE..$44D0]    3 bytes
      CYCLES(0x44ce, 6);
      /* $44CE JSR */ push16(0x44d0), s_pc = 0x5206;
      branchTarget = true;
      break;
    case 0x44d1: // [$44D1..$44D3]    3 bytes
      CYCLES(0x44d1, 6);
      /* $44D1 JSR */ push16(0x44d3), s_pc = 0x5203;
      branchTarget = true;
      break;
    case 0x44d4: // [$44D4..$44D6]    3 bytes
      CYCLES(0x44d4, 6);
      /* $44D4 JSR */ push16(0x44d6), s_pc = 0x5a1e;
      branchTarget = true;
      break;
    case 0x44d7: // [$44D7..$44D9]    3 bytes
      CYCLES(0x44d7, 6);
      /* $44D7 JSR */ push16(0x44d9), s_pc = 0x5a09;
      branchTarget = true;
      break;
    case 0x44da: // [$44DA..$44DC]    3 bytes
      CYCLES(0x44da, 6);
      /* $44DA JSR */ push16(0x44dc), s_pc = 0x5a0c;
      branchTarget = true;
      break;
    case 0x44dd: // [$44DD..$44DF]    3 bytes
      CYCLES(0x44dd, 6);
      /* $44DD JSR */ push16(0x44df), s_pc = 0x5a0f;
      branchTarget = true;
      break;
    case 0x44e0: // [$44E0..$44E2]    3 bytes
      CYCLES(0x44e0, 6);
      /* $44E0 JSR */ push16(0x44e2), s_pc = 0x6203;
      branchTarget = true;
      break;
    case 0x44e3: // [$44E3..$44E5]    3 bytes
      CYCLES(0x44e3, 6);
      /* $44E3 JSR */ push16(0x44e5), s_pc = 0x6206;
      branchTarget = true;
      break;
    case 0x44e6: // [$44E6..$44E8]    3 bytes
      CYCLES(0x44e6, 6);
      /* $44E6 JSR */ push16(0x44e8), s_pc = 0x6209;
      branchTarget = true;
      break;
    case 0x44e9: // [$44E9..$44EB]    3 bytes
      CYCLES(0x44e9, 6);
      /* $44E9 JSR */ push16(0x44eb), s_pc = 0x7003;
      branchTarget = true;
      break;
    case 0x44ec: // [$44EC..$44EE]    3 bytes
      CYCLES(0x44ec, 6);
      /* $44EC JSR */ push16(0x44ee), s_pc = 0x7006;
      branchTarget = true;
      break;
    case 0x44ef: // [$44EF..$44F1]    3 bytes
      CYCLES(0x44ef, 6);
      /* $44EF JSR */ push16(0x44f1), s_pc = 0x7009;
      branchTarget = true;
      break;
    case 0x44f2: // [$44F2..$44F4]    3 bytes
      CYCLES(0x44f2, 6);
      /* $44F2 JSR */ push16(0x44f4), s_pc = 0x7009;
      branchTarget = true;
      break;
    case 0x44f5: // [$44F5..$44F7]    3 bytes
      CYCLES(0x44f5, 6);
      /* $44F5 JSR */ push16(0x44f7), s_pc = 0x6903;
      branchTarget = true;
      break;
    case 0x44f8: // [$44F8..$44FA]    3 bytes
      CYCLES(0x44f8, 6);
      /* $44F8 JSR */ push16(0x44fa), s_pc = 0x6906;
      branchTarget = true;
      break;
    case 0x44fb: // [$44FB..$44FD]    3 bytes
      CYCLES(0x44fb, 6);
      /* $44FB JSR */ push16(0x44fd), s_pc = 0x6909;
      branchTarget = true;
      break;
    case 0x44fe: // [$44FE..$4500]    3 bytes
      CYCLES(0x44fe, 6);
      /* $44FE JSR */ push16(0x4500), s_pc = 0x0152;
      branchTarget = true;
      break;
    case 0x4501: // [$4501..$4503]    3 bytes
      CYCLES(0x4501, 6);
      /* $4501 JMP */ s_pc = 0x453d;
      branchTarget = true;
      break;
    case 0x4504: // [$4504..$4516]   19 bytes
      CYCLES(0x4504, 33);
      /* $4504 STA */ poke(0xc010, s_a);
      /* $4507 LDA */ s_a = update_nz(peek(0x4c6e));
      /* $450A STA */ poke_zpg(0x2f, s_a);
      /* $450C LDA */ s_a = update_nz(0x60);
      // WARNING: performs self modification.
      /* $450E STA */ poke(0x4c6e, s_a);
      // WARNING: performs self modification.
      /* $4511 STA */ poke(0x4dd0, s_a);
      /* $4514 JSR */ push16(0x4516), s_pc = 0x4522;
      branchTarget = true;
      break;
    case 0x4517: // [$4517..$4521]   11 bytes
      CYCLES(0x4517, 19);
      /* $4517 LDA */ s_a = update_nz(peek_zpg(0x2f));
      // WARNING: performs self modification.
      /* $4519 STA */ poke(0x4c6e, s_a);
      /* $451C LDA */ s_a = update_nz(0x86);
      // WARNING: performs self modification.
      /* $451E STA */ poke(0x4dd0, s_a);
      /* $4521 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4522: // [$4522..$4524]    3 bytes
      CYCLES(0x4522, 6);
      /* $4522 JSR */ push16(0x4524), s_pc = 0x455e;
      branchTarget = true;
      break;
    case 0x4525: // [$4525..$4527]    3 bytes
      CYCLES(0x4525, 6);
      /* $4525 JSR */ push16(0x4527), s_pc = 0x462a;
      branchTarget = true;
      break;
    case 0x4528: // [$4528..$452A]    3 bytes
      CYCLES(0x4528, 6);
      /* $4528 JSR */ push16(0x452a), s_pc = 0x4728;
      branchTarget = true;
      break;
    case 0x452b: // [$452B..$452D]    3 bytes
      CYCLES(0x452b, 6);
      /* $452B JSR */ push16(0x452d), s_pc = 0x467f;
      branchTarget = true;
      break;
    case 0x452e: // [$452E..$4530]    3 bytes
      CYCLES(0x452e, 6);
      /* $452E JSR */ push16(0x4530), s_pc = 0x47bc;
      branchTarget = true;
      break;
    case 0x4531: // [$4531..$4533]    3 bytes
      CYCLES(0x4531, 6);
      /* $4531 JSR */ push16(0x4533), s_pc = 0x4888;
      branchTarget = true;
      break;
    case 0x4534: // [$4534..$4536]    3 bytes
      CYCLES(0x4534, 6);
      /* $4534 JSR */ push16(0x4536), s_pc = 0x4a52;
      branchTarget = true;
      break;
    case 0x4537: // [$4537..$4539]    3 bytes
      CYCLES(0x4537, 6);
      /* $4537 JSR */ push16(0x4539), s_pc = 0x483e;
      branchTarget = true;
      break;
    case 0x453a: // [$453A..$453C]    3 bytes
      CYCLES(0x453a, 6);
      /* $453A JMP */ s_pc = 0x4522;
      branchTarget = true;
      break;
    case 0x453d: // [$453D..$4541]    5 bytes
      CYCLES(0x453d, 9);
      /* $453D LDY */ s_y = update_nz(peek(0xc000));
      /* $4540 BPL */ s_pc = !(s_status & STATUS_N) ? 0x455d : 0x4542;
      branchTarget = true;
      break;
    case 0x4542: // [$4542..$454A]    9 bytes
      CYCLES(0x4542, 16);
      /* $4542 STY */ poke(0xc010, s_y);
      /* $4545 PLA */ s_a = update_nz(pop8());
      /* $4546 PLA */ s_a = update_nz(pop8());
      /* $4547 CPY */ update_nz_inv_c(s_y - 0x8d);
      /* $4549 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x454c : 0x454b;
      branchTarget = true;
      break;
    case 0x454b: // [$454B..$454B]    1 bytes
      CYCLES(0x454b, 2);
      /* $454B RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x454c: // [$454C..$4551]    6 bytes
      CYCLES(0x454c, 11);
      /* $454C PLA */ s_a = update_nz(pop8());
      /* $454D PLA */ s_a = update_nz(pop8());
      /* $454E CPY */ update_nz_inv_c(s_y - 0xa0);
      /* $4550 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4553 : 0x4552;
      branchTarget = true;
      break;
    case 0x4552: // [$4552..$4552]    1 bytes
      CYCLES(0x4552, 2);
      /* $4552 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4553: // [$4553..$4556]    4 bytes
      CYCLES(0x4553, 7);
      /* $4553 CPY */ update_nz_inv_c(s_y - 0x9b);
      /* $4555 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x455a : 0x4557;
      branchTarget = true;
      break;
    case 0x4557: // [$4557..$4559]    3 bytes
      CYCLES(0x4557, 6);
      /* $4557 JMP */ s_pc = 0x4537;
      branchTarget = true;
      break;
    case 0x455a: // [$455A..$455C]    3 bytes
      CYCLES(0x455a, 6);
      /* $455A JMP */ s_pc = 0x4522;
      branchTarget = true;
      break;
    case 0x455d: // [$455D..$455D]    1 bytes
      CYCLES(0x455d, 2);
      /* $455D RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x455e: // [$455E..$4560]    3 bytes
      CYCLES(0x455e, 6);
      /* $455E JSR */ push16(0x4560), s_pc = 0x4f42;
      branchTarget = true;
      break;
    case 0x4561: // [$4561..$4568]    8 bytes
      CYCLES(0x4561, 14);
      /* $4561 LDA */ s_a = update_nz(peek(0x1432));
      /* $4564 ORA */ s_a = update_nz(s_a | peek(0x1433));
      /* $4567 BEQ */ s_pc = s_status & STATUS_Z ? 0x456c : 0x4569;
      branchTarget = true;
      break;
    case 0x4569: // [$4569..$456B]    3 bytes
      CYCLES(0x4569, 6);
      /* $4569 JSR */ push16(0x456b), s_pc = 0x6218;
      branchTarget = true;
      break;
    case 0x456c: // [$456C..$456E]    3 bytes
      CYCLES(0x456c, 6);
      /* $456C JSR */ push16(0x456e), s_pc = 0x51b8;
      branchTarget = true;
      break;
    case 0x456f: // [$456F..$4579]   11 bytes
      CYCLES(0x456f, 19);
      /* $456F ORA */ s_a = update_nz(s_a | peek(0x120c));
      /* $4572 EOR */ s_a = update_nz(s_a ^ peek(peek16_zpg(0x54 + s_x)));
      /* $4574 EOR */ s_a = update_nz(s_a ^ peek(peek16_zpg(0x52 + s_x)));
      /* $4576 EOR */ s_a = update_nz(s_a ^ 0x20);
      /* $4578 BVC */ s_pc = !(s_status & STATUS_V) ? 0x45cc : 0x457a;
      branchTarget = true;
      break;
    case 0x457a: // [$457A..$457E]    5 bytes
      CYCLES(0x457a, 9);
      /* $457A EOR */ s_a = update_nz(s_a ^ peek_zpg(0x53));
      /* $457C EOR */ s_a = update_nz(s_a ^ peek_zpg(0x4e));
      /* $457E ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x457e);
                      error_handler(s_pc);
      break;
    case 0x4582: // [$4582..$4590]   15 bytes
      CYCLES(0x4582, 26);
      /* $4582 LDA */ s_a = update_nz(0x03);
      /* $4584 STA */ poke(0x0c5a, s_a);
      /* $4587 LDA */ s_a = update_nz(0x28);
      /* $4589 STA */ poke(0x0c5b, s_a);
      /* $458C LDX */ s_x = update_nz(0x00);
      /* $458E JSR */ push16(0x4590), s_pc = 0x5081;
      branchTarget = true;
      break;
    case 0x4591: // [$4591..$4594]    4 bytes
      CYCLES(0x4591, 7);
      /* $4591 LDA */ s_a = update_nz(0xff);
      /* $4593 STA */ poke_zpg(0x0a, s_a);
    case 0x4595: // [$4595..$4597]    3 bytes
      CYCLES(0x4595, 6);
      /* $4595 JSR */ push16(0x4597), s_pc = 0x50c5;
      branchTarget = true;
      break;
    case 0x4598: // [$4598..$459A]    3 bytes
      CYCLES(0x4598, 6);
      /* $4598 JSR */ push16(0x459a), s_pc = 0x453d;
      branchTarget = true;
      break;
    case 0x459b: // [$459B..$459E]    4 bytes
      CYCLES(0x459b, 7);
      /* $459B LDA */ s_a = update_nz(peek_zpg(0x0a));
      /* $459D BEQ */ s_pc = s_status & STATUS_Z ? 0x45ac : 0x459f;
      branchTarget = true;
      break;
    case 0x459f: // [$459F..$45A5]    7 bytes
      CYCLES(0x459f, 12);
      /* $459F SEC */ s_status |= STATUS_C;
      /* $45A0 SBC */ tmp = 0x06, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $45A2 STA */ poke_zpg(0x0a, s_a);
      /* $45A4 BCS */ s_pc = s_status & STATUS_C ? 0x4595 : 0x45a6;
      branchTarget = true;
      break;
    case 0x45a6: // [$45A6..$45AB]    6 bytes
      CYCLES(0x45a6, 11);
      /* $45A6 LDA */ s_a = update_nz(0x00);
      /* $45A8 STA */ poke_zpg(0x0a, s_a);
      /* $45AA BEQ */ s_pc = s_status & STATUS_Z ? 0x4595 : 0x45ac;
      branchTarget = true;
      break;
    case 0x45ac: // [$45AC..$45BA]   15 bytes
      CYCLES(0x45ac, 26);
      /* $45AC LDA */ s_a = update_nz(0x0f);
      /* $45AE STA */ poke(0x0c5a, s_a);
      /* $45B1 LDA */ s_a = update_nz(0x60);
      /* $45B3 STA */ poke(0x0c5b, s_a);
      /* $45B6 LDX */ s_x = update_nz(0x01);
      /* $45B8 JSR */ push16(0x45ba), s_pc = 0x5081;
      branchTarget = true;
      break;
    case 0x45bb: // [$45BB..$45BE]    4 bytes
      CYCLES(0x45bb, 7);
      /* $45BB LDA */ s_a = update_nz(0xff);
      /* $45BD LDY */ s_y = update_nz(0x00);
    case 0x45bf: // [$45BF..$45C4]    6 bytes
      CYCLES(0x45bf, 11);
      /* $45BF STA */ poke(0x0700 + s_y, s_a);
      /* $45C2 INY */ s_y = update_nz(s_y + 1);
      /* $45C3 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x45bf : 0x45c5;
      branchTarget = true;
      break;
    case 0x45c5: // [$45C5..$45C7]    3 bytes
      CYCLES(0x45c5, 6);
      /* $45C5 JSR */ push16(0x45c7), s_pc = 0x50ca;
      branchTarget = true;
      break;
    case 0x45c8: // [$45C8..$45D6]   15 bytes
      CYCLES(0x45c8, 26);
      /* $45C8 LDA */ s_a = update_nz(0x03);
      /* $45CA STA */ poke(0x0c5a, s_a);
      /* $45CD LDA */ s_a = update_nz(0x28);
      /* $45CF STA */ poke(0x0c5b, s_a);
      /* $45D2 LDX */ s_x = update_nz(0x00);
      /* $45D4 JSR */ push16(0x45d6), s_pc = 0x5081;
      branchTarget = true;
      break;
    case 0x45cc: // [$45CC..$45CC]    1 bytes
      CYCLES(0x45cc, 2);
      /* $45CC ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x45cc);
                      error_handler(s_pc);
      break;
    case 0x45d7: // [$45D7..$45D9]    3 bytes
      CYCLES(0x45d7, 6);
      /* $45D7 JSR */ push16(0x45d9), s_pc = 0x0155;
      branchTarget = true;
      break;
    case 0x45da: // [$45DA..$45DB]    2 bytes
      CYCLES(0x45da, 4);
      /* $45DA LDX */ s_x = update_nz(0x05);
    case 0x45dc: // [$45DC..$45E0]    5 bytes
      CYCLES(0x45dc, 9);
      /* $45DC LDA */ s_a = update_nz(0x00);
      /* $45DE JSR */ push16(0x45e0), s_pc = 0xfca8;
      branchTarget = true;
      break;
    case 0x45e1: // [$45E1..$45E3]    3 bytes
      CYCLES(0x45e1, 6);
      /* $45E1 DEX */ s_x = update_nz(s_x - 1);
      /* $45E2 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x45dc : 0x45e4;
      branchTarget = true;
      break;
    case 0x45e4: // [$45E4..$45EB]    8 bytes
      CYCLES(0x45e4, 14);
      /* $45E4 LDA */ s_a = update_nz(0x80);
      /* $45E6 STA */ poke_zpg(0xfa, s_a);
      /* $45E8 LDA */ s_a = update_nz(0x00);
      /* $45EA STA */ poke_zpg(0x0a, s_a);
    case 0x45ec: // [$45EC..$45F7]   12 bytes
      CYCLES(0x45ec, 21);
      /* $45EC LDX */ s_x = update_nz(peek_zpg(0x0a));
      /* $45EE LDA */ s_a = update_nz(peek(0x461e + s_x));
      /* $45F1 STA */ poke_zpg(0x00, s_a);
      /* $45F3 LDY */ s_y = update_nz(peek(0x461f + s_x));
      /* $45F6 LDX */ s_x = update_nz(0x00);
    case 0x45f8: // [$45F8..$4604]   13 bytes
      CYCLES(0x45f8, 23);
      /* $45F8 LDA */ s_a = update_nz(peek_zpg(0x00));
      /* $45FA STA */ poke(0x0700 + s_x, s_a);
      /* $45FD TYA */ s_a = update_nz(s_y);
      /* $45FE STA */ poke(0x0701 + s_x, s_a);
      /* $4601 INX */ s_x = update_nz(s_x + 1);
      /* $4602 INX */ s_x = update_nz(s_x + 1);
      /* $4603 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x45f8 : 0x4605;
      branchTarget = true;
      break;
    case 0x4605: // [$4605..$4607]    3 bytes
      CYCLES(0x4605, 6);
      /* $4605 JSR */ push16(0x4607), s_pc = 0x50ca;
      branchTarget = true;
      break;
    case 0x4608: // [$4608..$4611]   10 bytes
      CYCLES(0x4608, 18);
      /* $4608 INC */ tmp16 = 0x0a, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $460A INC */ tmp16 = 0x0a, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $460C LDA */ s_a = update_nz(peek_zpg(0x0a));
      /* $460E CMP */ update_nz_inv_c(s_a - 0x0c);
      /* $4610 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4616 : 0x4612;
      branchTarget = true;
      break;
    case 0x4612: // [$4612..$4615]    4 bytes
      CYCLES(0x4612, 7);
      /* $4612 LDA */ s_a = update_nz(0x00);
      /* $4614 STA */ poke_zpg(0x0a, s_a);
    case 0x4616: // [$4616..$4618]    3 bytes
      CYCLES(0x4616, 6);
      /* $4616 JSR */ push16(0x4618), s_pc = 0x453d;
      branchTarget = true;
      break;
    case 0x4619: // [$4619..$461C]    4 bytes
      CYCLES(0x4619, 7);
      /* $4619 DEC */ tmp16 = 0xfa, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $461B BNE */ s_pc = !(s_status & STATUS_Z) ? 0x45ec : 0x461d;
      branchTarget = true;
      break;
    case 0x461d: // [$461D..$461D]    1 bytes
      CYCLES(0x461d, 2);
      /* $461D RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x462a: // [$462A..$462C]    3 bytes
      CYCLES(0x462a, 6);
      /* $462A JSR */ push16(0x462c), s_pc = 0x4666;
      branchTarget = true;
      break;
    case 0x462d: // [$462D..$462F]    3 bytes
      CYCLES(0x462d, 6);
      /* $462D JSR */ push16(0x462f), s_pc = 0x0140;
      branchTarget = true;
      break;
    case 0x4630: // [$4630..$4632]    3 bytes
      CYCLES(0x4630, 6);
      /* $4630 JSR */ push16(0x4632), s_pc = 0x426b;
      branchTarget = true;
      break;
    case 0x4633: // [$4633..$4635]    3 bytes
      CYCLES(0x4633, 6);
      /* $4633 JSR */ push16(0x4635), s_pc = 0x425b;
      branchTarget = true;
      break;
    case 0x4636: // [$4636..$4638]    3 bytes
      CYCLES(0x4636, 6);
      /* $4636 JSR */ push16(0x4638), s_pc = 0x453d;
      branchTarget = true;
      break;
    case 0x4639: // [$4639..$463B]    3 bytes
      CYCLES(0x4639, 6);
      /* $4639 JSR */ push16(0x463b), s_pc = 0x0152;
      branchTarget = true;
      break;
    case 0x463c: // [$463C..$4641]    6 bytes
      CYCLES(0x463c, 11);
      /* $463C LDY */ s_y = update_nz(0x00);
      /* $463E LDA */ s_a = update_nz(peek(peek16_zpg(0x28) + s_y));
      /* $4640 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4630 : 0x4642;
      branchTarget = true;
      break;
    case 0x4642: // [$4642..$4642]    1 bytes
      CYCLES(0x4642, 2);
      /* $4642 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4643: // [$4643..$465F]   29 bytes
      CYCLES(0x4643, 50);
      /* $4643 LDA */ s_a = update_nz(0x00);
      /* $4645 STA */ poke(0x1402, s_a);
      /* $4648 STA */ poke(0x1409, s_a);
      /* $464B STA */ poke(0x140a, s_a);
      /* $464E STA */ poke(0x140e, s_a);
      /* $4651 STA */ poke(0x140f, s_a);
      /* $4654 STA */ poke(0x1418, s_a);
      /* $4657 STA */ poke(0x1427, s_a);
      /* $465A STA */ poke(0x1428, s_a);
      /* $465D JSR */ push16(0x465f), s_pc = 0x43c0;
      branchTarget = true;
      break;
    case 0x4660: // [$4660..$4665]    6 bytes
      CYCLES(0x4660, 11);
      /* $4660 LDA */ s_a = update_nz(0x00);
      /* $4662 STA */ poke(0x1421, s_a);
      /* $4665 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4666: // [$4666..$4668]    3 bytes
      CYCLES(0x4666, 6);
      /* $4666 JSR */ push16(0x4668), s_pc = 0x4643;
      branchTarget = true;
      break;
    case 0x4669: // [$4669..$466B]    3 bytes
      CYCLES(0x4669, 6);
      /* $4669 JSR */ push16(0x466b), s_pc = 0x51b8;
      branchTarget = true;
      break;
    case 0x466c: // [$466C..$466F]    4 bytes
      CYCLES(0x466c, 7);
      /* $466C ORA */ s_a = update_nz(s_a | peek(0x000d));
      /* $466F ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x466f);
                      error_handler(s_pc);
      break;
    case 0x467f: // [$467F..$4681]    3 bytes
      CYCLES(0x467f, 6);
      /* $467F JSR */ push16(0x4681), s_pc = 0x4666;
      branchTarget = true;
      break;
    case 0x4682: // [$4682..$4684]    3 bytes
      CYCLES(0x4682, 6);
      /* $4682 JSR */ push16(0x4684), s_pc = 0x0143;
      branchTarget = true;
      break;
    case 0x4685: // [$4685..$468B]    7 bytes
      CYCLES(0x4685, 12);
      /* $4685 LDY */ s_y = update_nz(0x90);
      /* $4687 LDX */ s_x = update_nz(0x06);
      /* $4689 JSR */ push16(0x468b), s_pc = 0x4831;
      branchTarget = true;
      break;
    case 0x468c: // [$468C..$4699]   14 bytes
      CYCLES(0x468c, 24);
      /* $468C LDY */ s_y = update_nz(0x14);
      /* $468E STY */ poke(0x1402, s_y);
      /* $4691 STY */ poke(0x1408, s_y);
      /* $4694 LDA */ s_a = update_nz(peek(0x0c0c));
      /* $4697 STA */ poke(0x1403, s_a);
    case 0x469a: // [$469A..$469C]    3 bytes
      CYCLES(0x469a, 6);
      /* $469A DEY */ s_y = update_nz(s_y - 1);
      /* $469B BMI */ s_pc = s_status & STATUS_N ? 0x46c5 : 0x469d;
      branchTarget = true;
      break;
    case 0x469d: // [$469D..$46A1]    5 bytes
      CYCLES(0x469d, 9);
      /* $469D LDA */ s_a = update_nz(0x56);
      /* $469F JSR */ push16(0x46a1), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x46a2: // [$46A2..$46AB]   10 bytes
      CYCLES(0x46a2, 18);
      /* $46A2 ADC */ tmp = 0x60, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $46A4 STA */ poke(0x15f0 + s_y, s_a);
      /* $46A7 LDA */ s_a = update_nz(0x10);
      /* $46A9 JSR */ push16(0x46ab), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x46ac: // [$46AC..$46B9]   14 bytes
      CYCLES(0x46ac, 24);
      /* $46AC CLC */ s_status &= ~STATUS_C;
      /* $46AD ADC */ tmp = 0xe8, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $46AF AND */ s_a = update_nz(s_a & 0xfe);
      /* $46B1 STA */ poke(0x1570 + s_y, s_a);
      /* $46B4 TYA */ s_a = update_nz(s_y);
      /* $46B5 LDX */ s_x = update_nz(0x08);
      /* $46B7 JSR */ push16(0x46b9), s_pc = 0x4c4f;
      branchTarget = true;
      break;
    case 0x46ba: // [$46BA..$46C4]   11 bytes
      CYCLES(0x46ba, 19);
      /* $46BA TXA */ s_a = update_nz(s_x);
      /* $46BB STA */ poke(0x16f0 + s_y, s_a);
      /* $46BE LDA */ s_a = update_nz(0x00);
      /* $46C0 STA */ poke(0x1670 + s_y, s_a);
      /* $46C3 BEQ */ s_pc = s_status & STATUS_Z ? 0x469a : 0x46c5;
      branchTarget = true;
      break;
    case 0x46c5: // [$46C5..$46CB]    7 bytes
      CYCLES(0x46c5, 12);
      /* $46C5 LDX */ s_x = update_nz(0xdc);
      /* $46C7 LDA */ s_a = update_nz(0x46);
      /* $46C9 JSR */ push16(0x46cb), s_pc = 0x4824;
      branchTarget = true;
      break;
    case 0x46cc: // [$46CC..$46CE]    3 bytes
      CYCLES(0x46cc, 6);
      /* $46CC JSR */ push16(0x46ce), s_pc = 0x4706;
      branchTarget = true;
      break;
    case 0x46cf: // [$46CF..$46D1]    3 bytes
      CYCLES(0x46cf, 6);
      /* $46CF JSR */ push16(0x46d1), s_pc = 0x44c2;
      branchTarget = true;
      break;
    case 0x46d2: // [$46D2..$46D8]    7 bytes
      CYCLES(0x46d2, 12);
      /* $46D2 LDA */ s_a = update_nz(0x68);
      /* $46D4 LDX */ s_x = update_nz(0xc0);
      /* $46D6 JSR */ push16(0x46d8), s_pc = 0x46e9;
      branchTarget = true;
      break;
    case 0x46d9: // [$46D9..$46DB]    3 bytes
      CYCLES(0x46d9, 6);
      /* $46D9 JMP */ s_pc = 0x46cc;
      branchTarget = true;
      break;
    case 0x46e9: // [$46E9..$46EF]    7 bytes
      CYCLES(0x46e9, 12);
      /* $46E9 STA */ poke_zpg(0x06, s_a);
      /* $46EB STX */ poke_zpg(0x07, s_x);
      /* $46ED LDY */ s_y = update_nz(peek(0x1405));
    case 0x46f0: // [$46F0..$46F2]    3 bytes
      CYCLES(0x46f0, 6);
      /* $46F0 DEY */ s_y = update_nz(s_y - 1);
      /* $46F1 BMI */ s_pc = s_status & STATUS_N ? 0x4705 : 0x46f3;
      branchTarget = true;
      break;
    case 0x46f3: // [$46F3..$46F9]    7 bytes
      CYCLES(0x46f3, 12);
      /* $46F3 LDA */ s_a = update_nz(peek(0x1770 + s_y));
      /* $46F6 CMP */ update_nz_inv_c(s_a - peek_zpg(0x06));
      /* $46F8 BCC */ s_pc = !(s_status & STATUS_C) ? 0x46fe : 0x46fa;
      branchTarget = true;
      break;
    case 0x46fa: // [$46FA..$46FD]    4 bytes
      CYCLES(0x46fa, 7);
      /* $46FA CMP */ update_nz_inv_c(s_a - peek_zpg(0x07));
      /* $46FC BCC */ s_pc = !(s_status & STATUS_C) ? 0x46f0 : 0x46fe;
      branchTarget = true;
      break;
    case 0x46fe: // [$46FE..$4704]    7 bytes
      CYCLES(0x46fe, 12);
      /* $46FE LDA */ s_a = update_nz(0x01);
      /* $4700 STA */ poke(0x18b0 + s_y, s_a);
      /* $4703 BPL */ s_pc = !(s_status & STATUS_N) ? 0x46f0 : 0x4705;
      branchTarget = true;
      break;
    case 0x4705: // [$4705..$4705]    1 bytes
      CYCLES(0x4705, 2);
      /* $4705 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4706: // [$4706..$4709]    4 bytes
      CYCLES(0x4706, 7);
      /* $4706 DEC */ tmp16 = 0xfa, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $4708 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4727 : 0x470a;
      branchTarget = true;
      break;
    case 0x470a: // [$470A..$470F]    6 bytes
      CYCLES(0x470a, 11);
      /* $470A LDY */ s_y = update_nz(peek_zpg(0xfb));
      /* $470C LDA */ s_a = update_nz(peek(peek16_zpg(0x22) + s_y));
      /* $470E BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4713 : 0x4710;
      branchTarget = true;
      break;
    case 0x4710: // [$4710..$4712]    3 bytes
      CYCLES(0x4710, 6);
      /* $4710 PLA */ s_a = update_nz(pop8());
      /* $4711 PLA */ s_a = update_nz(pop8());
      /* $4712 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4713: // [$4713..$4719]    7 bytes
      CYCLES(0x4713, 12);
      /* $4713 STA */ poke_zpg(0xfa, s_a);
      /* $4715 INY */ s_y = update_nz(s_y + 1);
      /* $4716 LDA */ s_a = update_nz(peek(peek16_zpg(0x22) + s_y));
      /* $4718 BPL */ s_pc = !(s_status & STATUS_N) ? 0x4721 : 0x471a;
      branchTarget = true;
      break;
    case 0x471a: // [$471A..$4720]    7 bytes
      CYCLES(0x471a, 12);
      /* $471A AND */ s_a = update_nz(s_a & 0x0f);
      /* $471C STA */ poke(0x1416, s_a);
      /* $471F BPL */ s_pc = !(s_status & STATUS_N) ? 0x4724 : 0x4721;
      branchTarget = true;
      break;
    case 0x4721: // [$4721..$4723]    3 bytes
      CYCLES(0x4721, 6);
      /* $4721 STA */ poke(0x1506, s_a);
    case 0x4724: // [$4724..$4726]    3 bytes
      CYCLES(0x4724, 6);
      /* $4724 INY */ s_y = update_nz(s_y + 1);
      /* $4725 STY */ poke_zpg(0xfb, s_y);
    case 0x4727: // [$4727..$4727]    1 bytes
      CYCLES(0x4727, 2);
      /* $4727 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4728: // [$4728..$472A]    3 bytes
      CYCLES(0x4728, 6);
      /* $4728 JSR */ push16(0x472a), s_pc = 0x4666;
      branchTarget = true;
      break;
    case 0x472b: // [$472B..$472D]    3 bytes
      CYCLES(0x472b, 6);
      /* $472B JSR */ push16(0x472d), s_pc = 0x0146;
      branchTarget = true;
      break;
    case 0x472e: // [$472E..$4734]    7 bytes
      CYCLES(0x472e, 12);
      /* $472E LDY */ s_y = update_nz(0xa0);
      /* $4730 LDX */ s_x = update_nz(0x78);
      /* $4732 JSR */ push16(0x4734), s_pc = 0x4831;
      branchTarget = true;
      break;
    case 0x4735: // [$4735..$473F]   11 bytes
      CYCLES(0x4735, 19);
      /* $4735 LDA */ s_a = update_nz(0x6d);
      /* $4737 STA */ poke_zpg(0x20, s_a);
      /* $4739 LDA */ s_a = update_nz(0x47);
      /* $473B STA */ poke_zpg(0x21, s_a);
      /* $473D JSR */ push16(0x473f), s_pc = 0x4782;
      branchTarget = true;
      break;
    case 0x4740: // [$4740..$4746]    7 bytes
      CYCLES(0x4740, 12);
      /* $4740 LDX */ s_x = update_nz(0x50);
      /* $4742 LDA */ s_a = update_nz(0x47);
      /* $4744 JSR */ push16(0x4746), s_pc = 0x4824;
      branchTarget = true;
      break;
    case 0x4747: // [$4747..$4749]    3 bytes
      CYCLES(0x4747, 6);
      /* $4747 JSR */ push16(0x4749), s_pc = 0x4706;
      branchTarget = true;
      break;
    case 0x474a: // [$474A..$474C]    3 bytes
      CYCLES(0x474a, 6);
      /* $474A JSR */ push16(0x474c), s_pc = 0x44c2;
      branchTarget = true;
      break;
    case 0x474d: // [$474D..$474F]    3 bytes
      CYCLES(0x474d, 6);
      /* $474D JMP */ s_pc = 0x4747;
      branchTarget = true;
      break;
    case 0x4782: // [$4782..$478A]    9 bytes
      CYCLES(0x4782, 16);
      /* $4782 LDX */ s_x = update_nz(0x00);
      /* $4784 LDY */ s_y = update_nz(0x00);
      /* $4786 LDA */ s_a = update_nz(0x0a);
      /* $4788 STA */ poke(0x140d, s_a);
    case 0x478b: // [$478B..$478E]    4 bytes
      CYCLES(0x478b, 7);
      /* $478B LDA */ s_a = update_nz(peek(peek16_zpg(0x20) + s_y));
      /* $478D BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4793 : 0x478f;
      branchTarget = true;
      break;
    case 0x478f: // [$478F..$4792]    4 bytes
      CYCLES(0x478f, 7);
      /* $478F STX */ poke(0x140a, s_x);
      /* $4792 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4793: // [$4793..$47BB]   41 bytes
      CYCLES(0x4793, 70);
      /* $4793 STA */ poke(0x1950 + s_x, s_a);
      /* $4796 INY */ s_y = update_nz(s_y + 1);
      /* $4797 LDA */ s_a = update_nz(peek(peek16_zpg(0x20) + s_y));
      /* $4799 STA */ poke(0x1960 + s_x, s_a);
      /* $479C INY */ s_y = update_nz(s_y + 1);
      /* $479D LDA */ s_a = update_nz(peek(peek16_zpg(0x20) + s_y));
      /* $479F STA */ poke(0x1970 + s_x, s_a);
      /* $47A2 INY */ s_y = update_nz(s_y + 1);
      /* $47A3 LDA */ s_a = update_nz(peek(peek16_zpg(0x20) + s_y));
      /* $47A5 STA */ poke(0x1990 + s_x, s_a);
      /* $47A8 INY */ s_y = update_nz(s_y + 1);
      /* $47A9 LDA */ s_a = update_nz(0x00);
      /* $47AB STA */ poke(0x1980 + s_x, s_a);
      /* $47AE TXA */ s_a = update_nz(s_x);
      /* $47AF AND */ s_a = update_nz(s_a & 0x03);
      /* $47B1 STA */ poke(0x19a0 + s_x, s_a);
      /* $47B4 LDA */ s_a = update_nz(0x7f);
      /* $47B6 STA */ poke(0x19b0 + s_x, s_a);
      /* $47B9 INX */ s_x = update_nz(s_x + 1);
      /* $47BA BPL */ s_pc = !(s_status & STATUS_N) ? 0x478b : 0x47bc;
      branchTarget = true;
      break;
    case 0x47bc: // [$47BC..$47BE]    3 bytes
      CYCLES(0x47bc, 6);
      /* $47BC JSR */ push16(0x47be), s_pc = 0x4666;
      branchTarget = true;
      break;
    case 0x47bf: // [$47BF..$47C1]    3 bytes
      CYCLES(0x47bf, 6);
      /* $47BF JSR */ push16(0x47c1), s_pc = 0x0149;
      branchTarget = true;
      break;
    case 0x47c2: // [$47C2..$47C8]    7 bytes
      CYCLES(0x47c2, 12);
      /* $47C2 LDX */ s_x = update_nz(0xf0);
      /* $47C4 LDY */ s_y = update_nz(0xb0);
      /* $47C6 JSR */ push16(0x47c8), s_pc = 0x4831;
      branchTarget = true;
      break;
    case 0x47c9: // [$47C9..$47D3]   11 bytes
      CYCLES(0x47c9, 19);
      /* $47C9 LDA */ s_a = update_nz(0x17);
      /* $47CB STA */ poke_zpg(0x20, s_a);
      /* $47CD LDA */ s_a = update_nz(0x48);
      /* $47CF STA */ poke_zpg(0x21, s_a);
      /* $47D1 JSR */ push16(0x47d3), s_pc = 0x4782;
      branchTarget = true;
      break;
    case 0x47d4: // [$47D4..$47DA]    7 bytes
      CYCLES(0x47d4, 12);
      /* $47D4 LDX */ s_x = update_nz(0xfe);
      /* $47D6 LDA */ s_a = update_nz(0x47);
      /* $47D8 JSR */ push16(0x47da), s_pc = 0x4824;
      branchTarget = true;
      break;
    case 0x47db: // [$47DB..$47F4]   26 bytes
      CYCLES(0x47db, 45);
      /* $47DB LDA */ s_a = update_nz(0x09);
      /* $47DD STA */ poke(0x1900, s_a);
      /* $47E0 LDA */ s_a = update_nz(0x90);
      /* $47E2 STA */ poke(0x1910, s_a);
      /* $47E5 LDA */ s_a = update_nz(0x00);
      /* $47E7 STA */ poke(0x1920, s_a);
      /* $47EA STA */ poke(0x1930, s_a);
      /* $47ED LDA */ s_a = update_nz(0x7f);
      /* $47EF STA */ poke(0x1940, s_a);
      /* $47F2 INC */ tmp16 = 0x1409, poke(tmp16, update_nz(peek(tmp16) + 1));
    case 0x47f5: // [$47F5..$47F7]    3 bytes
      CYCLES(0x47f5, 6);
      /* $47F5 JSR */ push16(0x47f7), s_pc = 0x4706;
      branchTarget = true;
      break;
    case 0x47f8: // [$47F8..$47FA]    3 bytes
      CYCLES(0x47f8, 6);
      /* $47F8 JSR */ push16(0x47fa), s_pc = 0x44c2;
      branchTarget = true;
      break;
    case 0x47fb: // [$47FB..$47FD]    3 bytes
      CYCLES(0x47fb, 6);
      /* $47FB JMP */ s_pc = 0x47f5;
      branchTarget = true;
      break;
    case 0x4824: // [$4824..$4830]   13 bytes
      CYCLES(0x4824, 23);
      /* $4824 STX */ poke_zpg(0x22, s_x);
      /* $4826 STA */ poke_zpg(0x23, s_a);
      /* $4828 LDA */ s_a = update_nz(0x01);
      /* $482A STA */ poke_zpg(0xfa, s_a);
      /* $482C LDA */ s_a = update_nz(0x00);
      /* $482E STA */ poke_zpg(0xfb, s_a);
      /* $4830 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4831: // [$4831..$483D]   13 bytes
      CYCLES(0x4831, 23);
      /* $4831 STX */ poke(0x1500, s_x);
      /* $4834 STX */ poke(0x1502, s_x);
      /* $4837 STY */ poke(0x1501, s_y);
      /* $483A STY */ poke(0x1503, s_y);
      /* $483D RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x483e: // [$483E..$4840]    3 bytes
      CYCLES(0x483e, 6);
      /* $483E JSR */ push16(0x4840), s_pc = 0x4643;
      branchTarget = true;
      break;
    case 0x4841: // [$4841..$4843]    3 bytes
      CYCLES(0x4841, 6);
      /* $4841 JSR */ push16(0x4843), s_pc = 0x4930;
      branchTarget = true;
      break;
    case 0x4844: // [$4844..$484A]    7 bytes
      CYCLES(0x4844, 12);
      /* $4844 LDX */ s_x = update_nz(0x08);
      /* $4846 LDY */ s_y = update_nz(0x08);
      /* $4848 JSR */ push16(0x484a), s_pc = 0x4831;
      branchTarget = true;
      break;
    case 0x484b: // [$484B..$4851]    7 bytes
      CYCLES(0x484b, 12);
      /* $484B LDX */ s_x = update_nz(0x5f);
      /* $484D LDA */ s_a = update_nz(0x48);
      /* $484F JSR */ push16(0x4851), s_pc = 0x4824;
      branchTarget = true;
      break;
    case 0x4852: // [$4852..$4854]    3 bytes
      CYCLES(0x4852, 6);
      /* $4852 JSR */ push16(0x4854), s_pc = 0x4706;
      branchTarget = true;
      break;
    case 0x4855: // [$4855..$485B]    7 bytes
      CYCLES(0x4855, 12);
      /* $4855 LDA */ s_a = update_nz(0x7f);
      /* $4857 STA */ poke_zpg(0x2a, s_a);
      /* $4859 JSR */ push16(0x485b), s_pc = 0x44c2;
      branchTarget = true;
      break;
    case 0x485c: // [$485C..$485E]    3 bytes
      CYCLES(0x485c, 6);
      /* $485C JMP */ s_pc = 0x4852;
      branchTarget = true;
      break;
    case 0x4888: // [$4888..$488A]    3 bytes
      CYCLES(0x4888, 6);
      /* $4888 JSR */ push16(0x488a), s_pc = 0x4666;
      branchTarget = true;
      break;
    case 0x488b: // [$488B..$488D]    3 bytes
      CYCLES(0x488b, 6);
      /* $488B JSR */ push16(0x488d), s_pc = 0x014c;
      branchTarget = true;
      break;
    case 0x488e: // [$488E..$4894]    7 bytes
      CYCLES(0x488e, 12);
      /* $488E LDX */ s_x = update_nz(0xf0);
      /* $4890 LDY */ s_y = update_nz(0xa0);
      /* $4892 JSR */ push16(0x4894), s_pc = 0x4831;
      branchTarget = true;
      break;
    case 0x4895: // [$4895..$489F]   11 bytes
      CYCLES(0x4895, 19);
      /* $4895 LDA */ s_a = update_nz(0x2b);
      /* $4897 STA */ poke_zpg(0x20, s_a);
      /* $4899 LDA */ s_a = update_nz(0x49);
      /* $489B STA */ poke_zpg(0x21, s_a);
      /* $489D JSR */ push16(0x489f), s_pc = 0x4782;
      branchTarget = true;
      break;
    case 0x48a0: // [$48A0..$48A6]    7 bytes
      CYCLES(0x48a0, 12);
      /* $48A0 LDX */ s_x = update_nz(0x10);
      /* $48A2 LDA */ s_a = update_nz(0x49);
      /* $48A4 JSR */ push16(0x48a6), s_pc = 0x4824;
      branchTarget = true;
      break;
    case 0x48a7: // [$48A7..$48C8]   34 bytes
      CYCLES(0x48a7, 58);
      /* $48A7 LDA */ s_a = update_nz(0x08);
      /* $48A9 STA */ poke(0x1b20, s_a);
      /* $48AC LDA */ s_a = update_nz(0xa0);
      /* $48AE STA */ poke(0x1b30, s_a);
      /* $48B1 LDA */ s_a = update_nz(0x00);
      /* $48B3 STA */ poke(0x1b40, s_a);
      /* $48B6 STA */ poke(0x1b60, s_a);
      /* $48B9 STA */ poke(0x1b70, s_a);
      /* $48BC LDA */ s_a = update_nz(0x7f);
      /* $48BE STA */ poke(0x1b80, s_a);
      /* $48C1 LDA */ s_a = update_nz(0xff);
      /* $48C3 STA */ poke(0x1b90, s_a);
      /* $48C6 INC */ tmp16 = 0x1418, poke(tmp16, update_nz(peek(tmp16) + 1));
    case 0x48c9: // [$48C9..$48CB]    3 bytes
      CYCLES(0x48c9, 6);
      /* $48C9 JSR */ push16(0x48cb), s_pc = 0x4706;
      branchTarget = true;
      break;
    case 0x48cc: // [$48CC..$48CE]    3 bytes
      CYCLES(0x48cc, 6);
      /* $48CC JSR */ push16(0x48ce), s_pc = 0x44c2;
      branchTarget = true;
      break;
    case 0x48cf: // [$48CF..$48D5]    7 bytes
      CYCLES(0x48cf, 12);
      /* $48CF LDA */ s_a = update_nz(0x80);
      /* $48D1 LDX */ s_x = update_nz(0xc0);
      /* $48D3 JSR */ push16(0x48d5), s_pc = 0x46e9;
      branchTarget = true;
      break;
    case 0x48d6: // [$48D6..$48DA]    5 bytes
      CYCLES(0x48d6, 9);
      /* $48D6 LDA */ s_a = update_nz(peek(0x141d));
      /* $48D9 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x490d : 0x48db;
      branchTarget = true;
      break;
    case 0x48db: // [$48DB..$48E0]    6 bytes
      CYCLES(0x48db, 11);
      /* $48DB LDA */ s_a = update_nz(peek_zpg(0xfb));
      /* $48DD CMP */ update_nz_inv_c(s_a - 0x0a);
      /* $48DF BNE */ s_pc = !(s_status & STATUS_Z) ? 0x490d : 0x48e1;
      branchTarget = true;
      break;
    case 0x48e1: // [$48E1..$48E2]    2 bytes
      CYCLES(0x48e1, 4);
      /* $48E1 LDX */ s_x = update_nz(0x01);
    case 0x48e3: // [$48E3..$4903]   33 bytes
      CYCLES(0x48e3, 57);
      /* $48E3 LDA */ s_a = update_nz(peek(0x1b20));
      /* $48E6 CLC */ s_status &= ~STATUS_C;
      /* $48E7 ADC */ tmp = 0x06, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $48E9 STA */ poke(0x1c70 + s_x, s_a);
      /* $48EC LDA */ s_a = update_nz(peek(0x1b30));
      /* $48EF STA */ poke(0x1c80 + s_x, s_a);
      /* $48F2 LDA */ s_a = update_nz(0x02);
      /* $48F4 STA */ poke(0x1c90 + s_x, s_a);
      /* $48F7 LDA */ s_a = update_nz(0x00);
      /* $48F9 STA */ poke(0x1ca0 + s_x, s_a);
      /* $48FC LDA */ s_a = update_nz(0x7f);
      /* $48FE STA */ poke(0x1cb0 + s_x, s_a);
      /* $4901 DEX */ s_x = update_nz(s_x - 1);
      /* $4902 BEQ */ s_pc = s_status & STATUS_Z ? 0x48e3 : 0x4904;
      branchTarget = true;
      break;
    case 0x4904: // [$4904..$490C]    9 bytes
      CYCLES(0x4904, 16);
      /* $4904 LDA */ s_a = update_nz(peek(0x0c2e));
      /* $4907 STA */ poke(0x1cc1, s_a);
      /* $490A INC */ tmp16 = 0x141d, poke(tmp16, update_nz(peek(tmp16) + 1));
    case 0x490d: // [$490D..$490F]    3 bytes
      CYCLES(0x490d, 6);
      /* $490D JMP */ s_pc = 0x48c9;
      branchTarget = true;
      break;
    case 0x4930: // [$4930..$4932]    3 bytes
      CYCLES(0x4930, 6);
      /* $4930 JSR */ push16(0x4932), s_pc = 0x51b8;
      branchTarget = true;
      break;
    case 0x4933: // [$4933..$4936]    4 bytes
      CYCLES(0x4933, 7);
      /* $4933 ORA */ s_a = update_nz(s_a | peek(0x000f));
      /* $4936 ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x4936);
                      error_handler(s_pc);
      break;
    case 0x4a52: // [$4A52..$4A54]    3 bytes
      CYCLES(0x4a52, 6);
      /* $4A52 JSR */ push16(0x4a54), s_pc = 0x4666;
      branchTarget = true;
      break;
    case 0x4a55: // [$4A55..$4A57]    3 bytes
      CYCLES(0x4a55, 6);
      /* $4A55 JSR */ push16(0x4a57), s_pc = 0x014f;
      branchTarget = true;
      break;
    case 0x4a58: // [$4A58..$4A5E]    7 bytes
      CYCLES(0x4a58, 12);
      /* $4A58 LDX */ s_x = update_nz(0x08);
      /* $4A5A LDY */ s_y = update_nz(0x08);
      /* $4A5C JSR */ push16(0x4a5e), s_pc = 0x4831;
      branchTarget = true;
      break;
    case 0x4a5f: // [$4A5F..$4A65]    7 bytes
      CYCLES(0x4a5f, 12);
      /* $4A5F LDX */ s_x = update_nz(0x42);
      /* $4A61 LDA */ s_a = update_nz(0x4b);
      /* $4A63 JSR */ push16(0x4a65), s_pc = 0x4824;
      branchTarget = true;
      break;
    case 0x4a66: // [$4A66..$4AAA]   69 bytes
      CYCLES(0x4a66, 118);
      /* $4A66 LDA */ s_a = update_nz(0xf1);
      /* $4A68 STA */ poke(0x19c0, s_a);
      /* $4A6B LDA */ s_a = update_nz(0xf0);
      /* $4A6D STA */ poke(0x1ce0, s_a);
      /* $4A70 LDA */ s_a = update_nz(0x90);
      /* $4A72 STA */ poke(0x19d0, s_a);
      /* $4A75 LDA */ s_a = update_nz(0xa7);
      /* $4A77 STA */ poke(0x1cf0, s_a);
      /* $4A7A LDA */ s_a = update_nz(0x00);
      /* $4A7C STA */ poke(0x19e0, s_a);
      /* $4A7F STA */ poke(0x19f0, s_a);
      /* $4A82 STA */ poke(0x1d00, s_a);
      /* $4A85 STA */ poke(0x1d10, s_a);
      /* $4A88 STA */ poke(0x1a00, s_a);
      /* $4A8B STA */ poke(0x1d20, s_a);
      /* $4A8E STA */ poke(0x1a40, s_a);
      /* $4A91 STA */ poke(0x1d60, s_a);
      /* $4A94 STA */ poke(0x19c1, s_a);
      /* $4A97 STA */ poke(0x1ce1, s_a);
      /* $4A9A LDA */ s_a = update_nz(0x7f);
      /* $4A9C STA */ poke(0x1a20, s_a);
      /* $4A9F STA */ poke(0x1d50, s_a);
      /* $4AA2 STA */ poke(0x1d30, s_a);
      /* $4AA5 INC */ tmp16 = 0x140e, poke(tmp16, update_nz(peek(tmp16) + 1));
      /* $4AA8 INC */ tmp16 = 0x1427, poke(tmp16, update_nz(peek(tmp16) + 1));
    case 0x4aab: // [$4AAB..$4AAD]    3 bytes
      CYCLES(0x4aab, 6);
      /* $4AAB JSR */ push16(0x4aad), s_pc = 0x4706;
      branchTarget = true;
      break;
    case 0x4aae: // [$4AAE..$4AB4]    7 bytes
      CYCLES(0x4aae, 12);
      /* $4AAE LDA */ s_a = update_nz(peek(0x1d30));
      /* $4AB1 CMP */ update_nz_inv_c(s_a - 0x10);
      /* $4AB3 BCS */ s_pc = s_status & STATUS_C ? 0x4aba : 0x4ab5;
      branchTarget = true;
      break;
    case 0x4ab5: // [$4AB5..$4AB9]    5 bytes
      CYCLES(0x4ab5, 9);
      /* $4AB5 LDA */ s_a = update_nz(0x7f);
      /* $4AB7 STA */ poke(0x1d30, s_a);
    case 0x4aba: // [$4ABA..$4ACA]   17 bytes
      CYCLES(0x4aba, 29);
      /* $4ABA LDA */ s_a = update_nz(0x7f);
      /* $4ABC STA */ poke(0x1a10, s_a);
      /* $4ABF STA */ poke(0x1424, s_a);
      /* $4AC2 STA */ poke(0x142a, s_a);
      /* $4AC5 LDA */ s_a = update_nz(peek_zpg(0xfb));
      /* $4AC7 CMP */ update_nz_inv_c(s_a - 0x06);
      /* $4AC9 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4ad0 : 0x4acb;
      branchTarget = true;
      break;
    case 0x4acb: // [$4ACB..$4ACF]    5 bytes
      CYCLES(0x4acb, 9);
      /* $4ACB LDA */ s_a = update_nz(0xfc);
      /* $4ACD STA */ poke(0x19e0, s_a);
    case 0x4ad0: // [$4AD0..$4AD4]    5 bytes
      CYCLES(0x4ad0, 9);
      /* $4AD0 LDA */ s_a = update_nz(peek(0x140f));
      /* $4AD3 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4af8 : 0x4ad5;
      branchTarget = true;
      break;
    case 0x4ad5: // [$4AD5..$4ADB]    7 bytes
      CYCLES(0x4ad5, 12);
      /* $4AD5 LDA */ s_a = update_nz(peek(0x19c0));
      /* $4AD8 CMP */ update_nz_inv_c(s_a - 0xc1);
      /* $4ADA BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4af8 : 0x4adc;
      branchTarget = true;
      break;
    case 0x4adc: // [$4ADC..$4AF7]   28 bytes
      CYCLES(0x4adc, 48);
      /* $4ADC CLC */ s_status &= ~STATUS_C;
      /* $4ADD ADC */ tmp = 0x08, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4ADF STA */ poke(0x1a50, s_a);
      /* $4AE2 LDA */ s_a = update_nz(peek(0x19d0));
      /* $4AE5 STA */ poke(0x1a60, s_a);
      /* $4AE8 LDA */ s_a = update_nz(0x00);
      /* $4AEA STA */ poke(0x1a70, s_a);
      /* $4AED STA */ poke(0x1a90, s_a);
      /* $4AF0 LDA */ s_a = update_nz(0xfe);
      /* $4AF2 STA */ poke(0x1a80, s_a);
      /* $4AF5 INC */ tmp16 = 0x140f, poke(tmp16, update_nz(peek(tmp16) + 1));
    case 0x4af8: // [$4AF8..$4AFD]    6 bytes
      CYCLES(0x4af8, 11);
      /* $4AF8 LDA */ s_a = update_nz(peek_zpg(0xfb));
      /* $4AFA CMP */ update_nz_inv_c(s_a - 0x18);
      /* $4AFC BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4b03 : 0x4afe;
      branchTarget = true;
      break;
    case 0x4afe: // [$4AFE..$4B02]    5 bytes
      CYCLES(0x4afe, 9);
      /* $4AFE LDA */ s_a = update_nz(0xfc);
      /* $4B00 STA */ poke(0x1d00, s_a);
    case 0x4b03: // [$4B03..$4B07]    5 bytes
      CYCLES(0x4b03, 9);
      /* $4B03 LDA */ s_a = update_nz(peek(0x1428));
      /* $4B06 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4b35 : 0x4b08;
      branchTarget = true;
      break;
    case 0x4b08: // [$4B08..$4B0E]    7 bytes
      CYCLES(0x4b08, 12);
      /* $4B08 LDA */ s_a = update_nz(peek(0x1ce0));
      /* $4B0B CMP */ update_nz_inv_c(s_a - 0xc0);
      /* $4B0D BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4b35 : 0x4b0f;
      branchTarget = true;
      break;
    case 0x4b0f: // [$4B0F..$4B34]   38 bytes
      CYCLES(0x4b0f, 65);
      /* $4B0F CLC */ s_status &= ~STATUS_C;
      /* $4B10 ADC */ tmp = 0x08, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4B12 ORA */ s_a = update_nz(s_a | 0x01);
      /* $4B14 STA */ poke(0x1d70, s_a);
      /* $4B17 LDA */ s_a = update_nz(peek(0x1cf0));
      /* $4B1A STA */ poke(0x1d90, s_a);
      /* $4B1D LDA */ s_a = update_nz(0x00);
      /* $4B1F STA */ poke(0x1df0, s_a);
      /* $4B22 STA */ poke(0x1dd0, s_a);
      /* $4B25 STA */ poke(0x1e30, s_a);
      /* $4B28 LDA */ s_a = update_nz(0xfe);
      /* $4B2A STA */ poke(0x1db0, s_a);
      /* $4B2D LDA */ s_a = update_nz(0x7f);
      /* $4B2F STA */ poke(0x1e10, s_a);
      /* $4B32 INC */ tmp16 = 0x1428, poke(tmp16, update_nz(peek(tmp16) + 1));
    case 0x4b35: // [$4B35..$4B37]    3 bytes
      CYCLES(0x4b35, 6);
      /* $4B35 JSR */ push16(0x4b37), s_pc = 0x44c2;
      branchTarget = true;
      break;
    case 0x4b38: // [$4B38..$4B3E]    7 bytes
      CYCLES(0x4b38, 12);
      /* $4B38 LDA */ s_a = update_nz(0x80);
      /* $4B3A LDX */ s_x = update_nz(0xb0);
      /* $4B3C JSR */ push16(0x4b3e), s_pc = 0x46e9;
      branchTarget = true;
      break;
    case 0x4b3f: // [$4B3F..$4B41]    3 bytes
      CYCLES(0x4b3f, 6);
      /* $4B3F JMP */ s_pc = 0x4aab;
      branchTarget = true;
      break;
    case 0x4b67: // [$4B67..$4B69]    3 bytes
      CYCLES(0x4b67, 6);
      /* $4B67 JSR */ push16(0x4b69), s_pc = 0x4f25;
      branchTarget = true;
      break;
    case 0x4b6a: // [$4B6A..$4B6C]    3 bytes
      CYCLES(0x4b6a, 6);
      /* $4B6A JSR */ push16(0x4b6c), s_pc = 0x51b8;
      branchTarget = true;
      break;
    case 0x4b6d: // [$4B6D..$4B70]    4 bytes
      CYCLES(0x4b6d, 7);
      /* $4B6D ORA */ s_a = update_nz(s_a | peek(0x2809));
      /* $4B70 ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x4b70);
                      error_handler(s_pc);
      break;
    case 0x4bce: // [$4BCE..$4BD0]    3 bytes
      CYCLES(0x4bce, 6);
      /* $4BCE JSR */ push16(0x4bd0), s_pc = 0x4242;
      branchTarget = true;
      break;
    case 0x4bd1: // [$4BD1..$4BD4]    4 bytes
      CYCLES(0x4bd1, 7);
      /* $4BD1 CMP */ update_nz_inv_c(s_a - 0x20);
      /* $4BD3 BEQ */ s_pc = s_status & STATUS_Z ? 0x4bee : 0x4bd5;
      branchTarget = true;
      break;
    case 0x4bd5: // [$4BD5..$4BD6]    2 bytes
      CYCLES(0x4bd5, 4);
      /* $4BD5 LDX */ s_x = update_nz(0x00);
    case 0x4bd7: // [$4BD7..$4BDB]    5 bytes
      CYCLES(0x4bd7, 9);
      /* $4BD7 CMP */ update_nz_inv_c(s_a - peek(0x4bef + s_x));
      /* $4BDA BEQ */ s_pc = s_status & STATUS_Z ? 0x4beb : 0x4bdc;
      branchTarget = true;
      break;
    case 0x4bdc: // [$4BDC..$4BE0]    5 bytes
      CYCLES(0x4bdc, 9);
      /* $4BDC CMP */ update_nz_inv_c(s_a - peek(0x4bf3 + s_x));
      /* $4BDF BEQ */ s_pc = s_status & STATUS_Z ? 0x4beb : 0x4be1;
      branchTarget = true;
      break;
    case 0x4be1: // [$4BE1..$4BE5]    5 bytes
      CYCLES(0x4be1, 9);
      /* $4BE1 INX */ s_x = update_nz(s_x + 1);
      /* $4BE2 CPX */ update_nz_inv_c(s_x - 0x04);
      /* $4BE4 BCC */ s_pc = !(s_status & STATUS_C) ? 0x4bd7 : 0x4be6;
      branchTarget = true;
      break;
    case 0x4be6: // [$4BE6..$4BEA]    5 bytes
      CYCLES(0x4be6, 9);
      /* $4BE6 PLA */ s_a = update_nz(pop8());
      /* $4BE7 PLA */ s_a = update_nz(pop8());
      /* $4BE8 JMP */ s_pc = 0x4060;
      branchTarget = true;
      break;
    case 0x4beb: // [$4BEB..$4BED]    3 bytes
      CYCLES(0x4beb, 6);
      /* $4BEB STX */ poke(0x1417, s_x);
    case 0x4bee: // [$4BEE..$4BEE]    1 bytes
      CYCLES(0x4bee, 2);
      /* $4BEE RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4c00: // [$4C00..$4C09]   10 bytes
      CYCLES(0x4c00, 18);
      /* $4C00 STA */ poke_zpg(0xe0, s_a);
      /* $4C02 STX */ poke_zpg(0xe1, s_x);
      /* $4C04 LDA */ s_a = update_nz(0x00);
      /* $4C06 STA */ poke_zpg(0xe2, s_a);
      /* $4C08 LDX */ s_x = update_nz(0x08);
    case 0x4c0a: // [$4C0A..$4C10]    7 bytes
      CYCLES(0x4c0a, 12);
      /* $4C0A ASL */ tmp16 = 0xe0, poke_zpg(tmp16, update_nzc(peek_zpg(tmp16) << 1));
      /* $4C0C ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $4C0D CMP */ update_nz_inv_c(s_a - peek_zpg(0xe1));
      /* $4C0F BCC */ s_pc = !(s_status & STATUS_C) ? 0x4c13 : 0x4c11;
      branchTarget = true;
      break;
    case 0x4c11: // [$4C11..$4C12]    2 bytes
      CYCLES(0x4c11, 4);
      /* $4C11 SBC */ tmp = peek_zpg(0xe1), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
    case 0x4c13: // [$4C13..$4C17]    5 bytes
      CYCLES(0x4c13, 9);
      /* $4C13 ROL */ tmp16 = 0xe2, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $4C15 DEX */ s_x = update_nz(s_x - 1);
      /* $4C16 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4c0a : 0x4c18;
      branchTarget = true;
      break;
    case 0x4c18: // [$4C18..$4C1B]    4 bytes
      CYCLES(0x4c18, 7);
      /* $4C18 TAX */ s_x = update_nz(s_a);
      /* $4C19 LDA */ s_a = update_nz(peek_zpg(0xe2));
      /* $4C1B RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4c1c: // [$4C1C..$4C23]    8 bytes
      CYCLES(0x4c1c, 14);
      /* $4C1C STA */ poke_zpg(0xe0, s_a);
      /* $4C1E LDA */ s_a = update_nz(0x00);
      /* $4C20 STA */ poke_zpg(0xe2, s_a);
      /* $4C22 LDX */ s_x = update_nz(0x08);
    case 0x4c24: // [$4C24..$4C2A]    7 bytes
      CYCLES(0x4c24, 12);
      /* $4C24 ASL */ tmp16 = 0xe0, poke_zpg(tmp16, update_nzc(peek_zpg(tmp16) << 1));
      /* $4C26 ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $4C27 CMP */ update_nz_inv_c(s_a - 0x07);
      /* $4C29 BCC */ s_pc = !(s_status & STATUS_C) ? 0x4c2d : 0x4c2b;
      branchTarget = true;
      break;
    case 0x4c2b: // [$4C2B..$4C2C]    2 bytes
      CYCLES(0x4c2b, 4);
      /* $4C2B SBC */ tmp = 0x07, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
    case 0x4c2d: // [$4C2D..$4C31]    5 bytes
      CYCLES(0x4c2d, 9);
      /* $4C2D ROL */ tmp16 = 0xe2, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $4C2F DEX */ s_x = update_nz(s_x - 1);
      /* $4C30 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4c24 : 0x4c32;
      branchTarget = true;
      break;
    case 0x4c32: // [$4C32..$4C35]    4 bytes
      CYCLES(0x4c32, 7);
      /* $4C32 TAX */ s_x = update_nz(s_a);
      /* $4C33 LDA */ s_a = update_nz(peek_zpg(0xe2));
      /* $4C35 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4c36: // [$4C36..$4C4A]   21 bytes
      CYCLES(0x4c36, 36);
      /* $4C36 LDA */ s_a = update_nz(peek_zpg(0x4f));
      /* $4C38 ASL */ s_a = update_nzc(s_a << 1);
      /* $4C39 ADC */ tmp = peek_zpg(0xfc), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4C3B EOR */ s_a = update_nz(s_a ^ peek_zpg(0x4e));
      /* $4C3D PHA */ push8(s_a);
      /* $4C3E INC */ tmp16 = 0x4e, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $4C40 LDA */ s_a = update_nz(peek_zpg(0x4e));
      /* $4C42 EOR */ s_a = update_nz(s_a ^ peek(0x150a));
      /* $4C45 STA */ poke_zpg(0x4f, s_a);
      /* $4C47 PLA */ s_a = update_nz(pop8());
      /* $4C48 STA */ poke_zpg(0x4e, s_a);
      /* $4C4A RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4c4b: // [$4C4B..$4C4E]    4 bytes
      CYCLES(0x4c4b, 7);
      /* $4C4B TAX */ s_x = update_nz(s_a);
      /* $4C4C JSR */ push16(0x4c4e), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x4c4f: // [$4C4F..$4C58]   10 bytes
      CYCLES(0x4c4f, 18);
      /* $4C4F STA */ poke_zpg(0xe0, s_a);
      /* $4C51 STX */ poke_zpg(0xe1, s_x);
      /* $4C53 LDA */ s_a = update_nz(0x00);
      /* $4C55 STA */ poke_zpg(0xe2, s_a);
      /* $4C57 LDX */ s_x = update_nz(0x08);
    case 0x4c59: // [$4C59..$4C5F]    7 bytes
      CYCLES(0x4c59, 12);
      /* $4C59 ASL */ s_a = update_nzc(s_a << 1);
      /* $4C5A ROL */ tmp16 = 0xe2, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $4C5C ASL */ tmp16 = 0xe0, poke_zpg(tmp16, update_nzc(peek_zpg(tmp16) << 1));
      /* $4C5E BCC */ s_pc = !(s_status & STATUS_C) ? 0x4c67 : 0x4c60;
      branchTarget = true;
      break;
    case 0x4c60: // [$4C60..$4C64]    5 bytes
      CYCLES(0x4c60, 9);
      /* $4C60 CLC */ s_status &= ~STATUS_C;
      /* $4C61 ADC */ tmp = peek_zpg(0xe1), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4C63 BCC */ s_pc = !(s_status & STATUS_C) ? 0x4c67 : 0x4c65;
      branchTarget = true;
      break;
    case 0x4c65: // [$4C65..$4C66]    2 bytes
      CYCLES(0x4c65, 4);
      /* $4C65 INC */ tmp16 = 0xe2, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
    case 0x4c67: // [$4C67..$4C69]    3 bytes
      CYCLES(0x4c67, 6);
      /* $4C67 DEX */ s_x = update_nz(s_x - 1);
      /* $4C68 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4c59 : 0x4c6a;
      branchTarget = true;
      break;
    case 0x4c6a: // [$4C6A..$4C6D]    4 bytes
      CYCLES(0x4c6a, 7);
      /* $4C6A TAX */ s_x = update_nz(s_a);
      /* $4C6B LDA */ s_a = update_nz(peek_zpg(0xe2));
      /* $4C6D RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4c6e: // [$4C6E..$4C71]    4 bytes
      CYCLES(0x4c6e, 7);
      // ERROR: opcode self modification.
      /* $4C6E LDA */ s_a = update_nz(peek_zpg(0xca));
      /* $4C70 BEQ */ s_pc = s_status & STATUS_Z ? 0x4c7b : 0x4c72;
      branchTarget = true;
      break;
    case 0x4c72: // [$4C72..$4C77]    6 bytes
      CYCLES(0x4c72, 11);
      /* $4C72 ADC */ tmp = peek_zpg(0xcb), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4C74 STA */ poke_zpg(0xcb, s_a);
      /* $4C76 BCC */ s_pc = !(s_status & STATUS_C) ? 0x4c7b : 0x4c78;
      branchTarget = true;
      break;
    case 0x4c78: // [$4C78..$4C7A]    3 bytes
      CYCLES(0x4c78, 6);
      /* $4C78 LDA */ s_a = update_nz(peek(0xc030));
    case 0x4c7b: // [$4C7B..$4C7B]    1 bytes
      CYCLES(0x4c7b, 2);
      /* $4C7B RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4c7c: // [$4C7C..$4C87]   12 bytes
      CYCLES(0x4c7c, 21);
      /* $4C7C STY */ poke_zpg(0xfc, s_y);
      /* $4C7E STY */ poke_zpg(0xfe, s_y);
      /* $4C80 STX */ poke_zpg(0x05, s_x);
      /* $4C82 LDA */ s_a = update_nz(0x00);
      /* $4C84 STA */ poke_zpg(0x04, s_a);
      /* $4C86 LDX */ s_x = update_nz(0x08);
    case 0x4c88: // [$4C88..$4C8E]    7 bytes
      CYCLES(0x4c88, 12);
      /* $4C88 ASL */ tmp16 = 0x05, poke_zpg(tmp16, update_nzc(peek_zpg(tmp16) << 1));
      /* $4C8A ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $4C8B CMP */ update_nz_inv_c(s_a - 0x07);
      /* $4C8D BCC */ s_pc = !(s_status & STATUS_C) ? 0x4c91 : 0x4c8f;
      branchTarget = true;
      break;
    case 0x4c8f: // [$4C8F..$4C90]    2 bytes
      CYCLES(0x4c8f, 4);
      /* $4C8F SBC */ tmp = 0x07, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
    case 0x4c91: // [$4C91..$4C95]    5 bytes
      CYCLES(0x4c91, 9);
      /* $4C91 ROL */ tmp16 = 0x04, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $4C93 DEX */ s_x = update_nz(s_x - 1);
      /* $4C94 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4c88 : 0x4c96;
      branchTarget = true;
      break;
    case 0x4c96: // [$4C96..$4CAB]   22 bytes
      CYCLES(0x4c96, 38);
      /* $4C96 ADC */ tmp = 0x01, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4C98 ASL */ s_a = update_nzc(s_a << 1);
      /* $4C99 TAY */ s_y = update_nz(s_a);
      /* $4C9A LDA */ s_a = update_nz(peek(peek16_zpg(0x06) + s_y));
      /* $4C9C STA */ poke_zpg(0x08, s_a);
      /* $4C9E INY */ s_y = update_nz(s_y + 1);
      /* $4C9F LDA */ s_a = update_nz(peek(peek16_zpg(0x06) + s_y));
      /* $4CA1 STA */ poke_zpg(0x09, s_a);
      /* $4CA3 LDY */ s_y = update_nz(0x01);
      /* $4CA5 LDA */ s_a = update_nz(peek(peek16_zpg(0x06) + s_y));
      /* $4CA7 TAY */ s_y = update_nz(s_a);
      /* $4CA8 DEY */ s_y = update_nz(s_y - 1);
      /* $4CA9 LDX */ s_x = update_nz(0x00);
      /* $4CAB RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4cac: // [$4CAC..$4CAE]    3 bytes
      CYCLES(0x4cac, 6);
      /* $4CAC JSR */ push16(0x4cae), s_pc = 0x4c7c;
      branchTarget = true;
      break;
    case 0x4caf: // [$4CAF..$4CCC]   30 bytes
      CYCLES(0x4caf, 52);
      /* $4CAF LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $4CB1 STA */ poke_zpg(0x06, s_a);
      /* $4CB3 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $4CB5 STA */ poke_zpg(0x07, s_a);
      /* $4CB7 STY */ poke_zpg(0x05, s_y);
      /* $4CB9 LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $4CBB LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $4CBD STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $4CBF INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $4CC1 INY */ s_y = update_nz(s_y + 1);
      /* $4CC2 LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $4CC4 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $4CC6 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $4CC8 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $4CCA DEY */ s_y = update_nz(s_y - 1);
      /* $4CCB BPL */ s_pc = !(s_status & STATUS_N) ? 0x4caf : 0x4ccd;
      branchTarget = true;
      break;
    case 0x4ccd: // [$4CCD..$4CCF]    3 bytes
      CYCLES(0x4ccd, 6);
      /* $4CCD JMP */ s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x4cd0: // [$4CD0..$4CD6]    7 bytes
      CYCLES(0x4cd0, 12);
      /* $4CD0 STX */ poke_zpg(0xfc, s_x);
      /* $4CD2 STX */ poke_zpg(0xfe, s_x);
      /* $4CD4 JSR */ push16(0x4cd6), s_pc = 0x4c1c;
      branchTarget = true;
      break;
    case 0x4cd7: // [$4CD7..$4CDB]    5 bytes
      CYCLES(0x4cd7, 9);
      /* $4CD7 STA */ poke_zpg(0x04, s_a);
      /* $4CD9 DEY */ s_y = update_nz(s_y - 1);
      /* $4CDA LDX */ s_x = update_nz(0x00);
    case 0x4cdc: // [$4CDC..$4CF2]   23 bytes
      CYCLES(0x4cdc, 40);
      /* $4CDC LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $4CDE STA */ poke_zpg(0x06, s_a);
      /* $4CE0 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $4CE2 STA */ poke_zpg(0x07, s_a);
      /* $4CE4 STY */ poke_zpg(0x05, s_y);
      /* $4CE6 LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $4CE8 TXA */ s_a = update_nz(s_x);
      /* $4CE9 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $4CEB INY */ s_y = update_nz(s_y + 1);
      /* $4CEC STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $4CEE LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $4CF0 DEY */ s_y = update_nz(s_y - 1);
      /* $4CF1 BPL */ s_pc = !(s_status & STATUS_N) ? 0x4cdc : 0x4cf3;
      branchTarget = true;
      break;
    case 0x4cf3: // [$4CF3..$4CF5]    3 bytes
      CYCLES(0x4cf3, 6);
      /* $4CF3 JMP */ s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x4cf6: // [$4CF6..$4CF8]    3 bytes
      CYCLES(0x4cf6, 6);
      /* $4CF6 JSR */ push16(0x4cf8), s_pc = 0x4c7c;
      branchTarget = true;
      break;
    case 0x4cf9: // [$4CF9..$4D1D]   37 bytes
      CYCLES(0x4cf9, 63);
      /* $4CF9 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $4CFB STA */ poke_zpg(0x06, s_a);
      /* $4CFD LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $4CFF STA */ poke_zpg(0x07, s_a);
      /* $4D01 STY */ poke_zpg(0x05, s_y);
      /* $4D03 LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $4D05 LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $4D07 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $4D09 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $4D0B INY */ s_y = update_nz(s_y + 1);
      /* $4D0C LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $4D0E STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $4D10 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $4D12 INY */ s_y = update_nz(s_y + 1);
      /* $4D13 LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $4D15 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $4D17 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $4D19 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $4D1B DEY */ s_y = update_nz(s_y - 1);
      /* $4D1C BPL */ s_pc = !(s_status & STATUS_N) ? 0x4cf9 : 0x4d1e;
      branchTarget = true;
      break;
    case 0x4d1e: // [$4D1E..$4D20]    3 bytes
      CYCLES(0x4d1e, 6);
      /* $4D1E JMP */ s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x4d21: // [$4D21..$4D27]    7 bytes
      CYCLES(0x4d21, 12);
      /* $4D21 STX */ poke_zpg(0xfc, s_x);
      /* $4D23 STX */ poke_zpg(0xfe, s_x);
      /* $4D25 JSR */ push16(0x4d27), s_pc = 0x4c1c;
      branchTarget = true;
      break;
    case 0x4d28: // [$4D28..$4D2C]    5 bytes
      CYCLES(0x4d28, 9);
      /* $4D28 STA */ poke_zpg(0x04, s_a);
      /* $4D2A DEY */ s_y = update_nz(s_y - 1);
      /* $4D2B LDX */ s_x = update_nz(0x00);
    case 0x4d2d: // [$4D2D..$4D46]   26 bytes
      CYCLES(0x4d2d, 45);
      /* $4D2D LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $4D2F STA */ poke_zpg(0x06, s_a);
      /* $4D31 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $4D33 STA */ poke_zpg(0x07, s_a);
      /* $4D35 STY */ poke_zpg(0x05, s_y);
      /* $4D37 LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $4D39 TXA */ s_a = update_nz(s_x);
      /* $4D3A STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $4D3C INY */ s_y = update_nz(s_y + 1);
      /* $4D3D STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $4D3F INY */ s_y = update_nz(s_y + 1);
      /* $4D40 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $4D42 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $4D44 DEY */ s_y = update_nz(s_y - 1);
      /* $4D45 BPL */ s_pc = !(s_status & STATUS_N) ? 0x4d2d : 0x4d47;
      branchTarget = true;
      break;
    case 0x4d47: // [$4D47..$4D49]    3 bytes
      CYCLES(0x4d47, 6);
      /* $4D47 JMP */ s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x4d4a: // [$4D4A..$4D4B]    2 bytes
      CYCLES(0x4d4a, 4);
      /* $4D4A LDX */ s_x = update_nz(0x0b);
    case 0x4d4c: // [$4D4C..$4D4E]    3 bytes
      CYCLES(0x4d4c, 6);
      /* $4D4C JSR */ push16(0x4d4e), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x4d4f: // [$4D4F..$4D54]    6 bytes
      CYCLES(0x4d4f, 11);
      /* $4D4F STA */ poke(0x0700 + s_x, s_a);
      /* $4D52 JSR */ push16(0x4d54), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x4d55: // [$4D55..$4D5A]    6 bytes
      CYCLES(0x4d55, 11);
      /* $4D55 STA */ poke(0x0780 + s_x, s_a);
      /* $4D58 DEX */ s_x = update_nz(s_x - 1);
      /* $4D59 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4d4c : 0x4d5b;
      branchTarget = true;
      break;
    case 0x4d5b: // [$4D5B..$4D66]   12 bytes
      CYCLES(0x4d5b, 21);
      /* $4D5B STX */ poke_zpg(0x00, s_x);
      /* $4D5D STX */ poke_zpg(0xfc, s_x);
      /* $4D5F STX */ poke_zpg(0xfe, s_x);
      /* $4D61 STX */ poke(0x0700, s_x);
      /* $4D64 STX */ poke(0x0780, s_x);
    case 0x4d67: // [$4D67..$4D72]   12 bytes
      CYCLES(0x4d67, 21);
      /* $4D67 LDA */ s_a = update_nz(peek_zpg(0x00));
      /* $4D69 STA */ poke_zpg(0x01, s_a);
      /* $4D6B CLC */ s_status &= ~STATUS_C;
      /* $4D6C ADC */ tmp = 0x60, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4D6E LDX */ s_x = update_nz(0xff);
      /* $4D70 JSR */ push16(0x4d72), s_pc = 0x4c4f;
      branchTarget = true;
      break;
    case 0x4d73: // [$4D73..$4D74]    2 bytes
      CYCLES(0x4d73, 4);
      /* $4D73 STA */ poke_zpg(0xca, s_a);
    case 0x4d75: // [$4D75..$4D85]   17 bytes
      CYCLES(0x4d75, 29);
      /* $4D75 LDA */ s_a = update_nz(peek_zpg(0x01));
      /* $4D77 SEC */ s_status |= STATUS_C;
      /* $4D78 SBC */ tmp = peek_zpg(0x00), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $4D7A LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $4D7B LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $4D7C LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $4D7D TAX */ s_x = update_nz(s_a);
      /* $4D7E LDA */ s_a = update_nz(0x60);
      /* $4D80 CLC */ s_status &= ~STATUS_C;
      /* $4D81 ADC */ tmp = peek_zpg(0x01), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4D83 JSR */ push16(0x4d85), s_pc = 0x4da2;
      branchTarget = true;
      break;
    case 0x4d86: // [$4D86..$4D8D]    8 bytes
      CYCLES(0x4d86, 14);
      /* $4D86 LDA */ s_a = update_nz(0x5f);
      /* $4D88 SEC */ s_status |= STATUS_C;
      /* $4D89 SBC */ tmp = peek_zpg(0x01), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $4D8B JSR */ push16(0x4d8d), s_pc = 0x4da2;
      branchTarget = true;
      break;
    case 0x4d8e: // [$4D8E..$4D98]   11 bytes
      CYCLES(0x4d8e, 19);
      /* $4D8E LDA */ s_a = update_nz(peek_zpg(0x01));
      /* $4D90 CLC */ s_status &= ~STATUS_C;
      /* $4D91 ADC */ tmp = 0x08, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4D93 STA */ poke_zpg(0x01, s_a);
      /* $4D95 CMP */ update_nz_inv_c(s_a - 0x60);
      /* $4D97 BCC */ s_pc = !(s_status & STATUS_C) ? 0x4d75 : 0x4d99;
      branchTarget = true;
      break;
    case 0x4d99: // [$4D99..$4DA0]    8 bytes
      CYCLES(0x4d99, 14);
      /* $4D99 INC */ tmp16 = 0x00, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $4D9B LDA */ s_a = update_nz(peek_zpg(0x00));
      /* $4D9D CMP */ update_nz_inv_c(s_a - 0x60);
      /* $4D9F BCC */ s_pc = !(s_status & STATUS_C) ? 0x4d67 : 0x4da1;
      branchTarget = true;
      break;
    case 0x4da1: // [$4DA1..$4DA1]    1 bytes
      CYCLES(0x4da1, 2);
      /* $4DA1 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4da2: // [$4DA2..$4DAF]   14 bytes
      CYCLES(0x4da2, 24);
      /* $4DA2 TAY */ s_y = update_nz(s_a);
      /* $4DA3 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $4DA5 STA */ poke_zpg(0x06, s_a);
      /* $4DA7 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $4DA9 STA */ poke_zpg(0x07, s_a);
      /* $4DAB LDY */ s_y = update_nz(0x26);
      /* $4DAD LDA */ s_a = update_nz(peek(0x0700 + s_x));
    case 0x4db0: // [$4DB0..$4DB5]    6 bytes
      CYCLES(0x4db0, 11);
      /* $4DB0 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $4DB2 DEY */ s_y = update_nz(s_y - 1);
      /* $4DB3 DEY */ s_y = update_nz(s_y - 1);
      /* $4DB4 BPL */ s_pc = !(s_status & STATUS_N) ? 0x4db0 : 0x4db6;
      branchTarget = true;
      break;
    case 0x4db6: // [$4DB6..$4DB8]    3 bytes
      CYCLES(0x4db6, 6);
      /* $4DB6 JSR */ push16(0x4db8), s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x4db9: // [$4DB9..$4DBD]    5 bytes
      CYCLES(0x4db9, 9);
      /* $4DB9 LDY */ s_y = update_nz(0x27);
      /* $4DBB LDA */ s_a = update_nz(peek(0x0780 + s_x));
    case 0x4dbe: // [$4DBE..$4DC3]    6 bytes
      CYCLES(0x4dbe, 11);
      /* $4DBE STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $4DC0 DEY */ s_y = update_nz(s_y - 1);
      /* $4DC1 DEY */ s_y = update_nz(s_y - 1);
      /* $4DC2 BPL */ s_pc = !(s_status & STATUS_N) ? 0x4dbe : 0x4dc4;
      branchTarget = true;
      break;
    case 0x4dc4: // [$4DC4..$4DC6]    3 bytes
      CYCLES(0x4dc4, 6);
      /* $4DC4 JSR */ push16(0x4dc6), s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x4dc7: // [$4DC7..$4DC8]    2 bytes
      CYCLES(0x4dc7, 4);
      /* $4DC7 LDY */ s_y = update_nz(0xff);
    case 0x4dc9: // [$4DC9..$4DCB]    3 bytes
      CYCLES(0x4dc9, 6);
      /* $4DC9 DEY */ s_y = update_nz(s_y - 1);
      /* $4DCA BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4dc9 : 0x4dcc;
      branchTarget = true;
      break;
    case 0x4dcc: // [$4DCC..$4DCE]    3 bytes
      CYCLES(0x4dcc, 6);
      /* $4DCC JSR */ push16(0x4dce), s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x4dcf: // [$4DCF..$4DCF]    1 bytes
      CYCLES(0x4dcf, 2);
      /* $4DCF RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4dd0: // [$4DD0..$4DFF]   48 bytes
      CYCLES(0x4dd0, 82);
      // ERROR: opcode self modification.
      /* $4DD0 STX */ poke_zpg(0x06, s_x);
      /* $4DD2 STA */ poke_zpg(0x07, s_a);
      /* $4DD4 LDA */ s_a = update_nz(peek(0x150a));
      /* $4DD7 CLC */ s_status &= ~STATUS_C;
      /* $4DD8 ADC */ tmp = peek_zpg(0x06), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4DDA STA */ poke(0x150a, s_a);
      /* $4DDD LDA */ s_a = update_nz(peek(0x150b));
      /* $4DE0 ADC */ tmp = peek_zpg(0x07), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4DE2 STA */ poke(0x150b, s_a);
      /* $4DE5 LDA */ s_a = update_nz(peek(0x150c));
      /* $4DE8 ADC */ tmp = 0x00, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4DEA STA */ poke(0x150c, s_a);
      /* $4DED LDA */ s_a = update_nz(peek(0x150d));
      /* $4DF0 SEC */ s_status |= STATUS_C;
      /* $4DF1 SBC */ tmp = peek_zpg(0x06), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $4DF3 STA */ poke(0x150d, s_a);
      /* $4DF6 LDA */ s_a = update_nz(peek(0x150e));
      /* $4DF9 SBC */ tmp = peek_zpg(0x07), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $4DFB STA */ poke(0x150e, s_a);
      /* $4DFE BPL */ s_pc = !(s_status & STATUS_N) ? 0x4e1f : 0x4e00;
      branchTarget = true;
      break;
    case 0x4e00: // [$4E00..$4E05]    6 bytes
      CYCLES(0x4e00, 11);
      /* $4E00 INC */ tmp16 = 0x1509, poke(tmp16, update_nz(peek(tmp16) + 1));
      /* $4E03 JSR */ push16(0x4e05), s_pc = 0x4e4f;
      branchTarget = true;
      break;
    case 0x4e06: // [$4E06..$4E1E]   25 bytes
      CYCLES(0x4e06, 43);
      /* $4E06 LDA */ s_a = update_nz(peek(0x150d));
      /* $4E09 CLC */ s_status &= ~STATUS_C;
      /* $4E0A ADC */ tmp = peek(0x0c10), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4E0D STA */ poke(0x150d, s_a);
      /* $4E10 LDA */ s_a = update_nz(peek(0x150e));
      /* $4E13 ADC */ tmp = peek(0x0c11), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4E16 STA */ poke(0x150e, s_a);
      /* $4E19 LDA */ s_a = update_nz(peek(0x0c69));
      /* $4E1C STA */ poke(0x1430, s_a);
    case 0x4e1f: // [$4E1F..$4E31]   19 bytes
      CYCLES(0x4e1f, 33);
      /* $4E1F LDA */ s_a = update_nz(0x38);
      /* $4E21 STA */ poke_zpg(0xfc, s_a);
      /* $4E23 LDA */ s_a = update_nz(peek(0x150a));
      /* $4E26 STA */ poke_zpg(0x00, s_a);
      /* $4E28 LDA */ s_a = update_nz(peek(0x150b));
      /* $4E2B STA */ poke_zpg(0x01, s_a);
      /* $4E2D LDA */ s_a = update_nz(peek(0x150c));
      /* $4E30 STA */ poke_zpg(0x02, s_a);
    case 0x4e32: // [$4E32..$4E34]    3 bytes
      CYCLES(0x4e32, 6);
      /* $4E32 JSR */ push16(0x4e34), s_pc = 0x4ec4;
      branchTarget = true;
      break;
    case 0x4e35: // [$4E35..$4E3E]   10 bytes
      CYCLES(0x4e35, 18);
      /* $4E35 CLC */ s_status &= ~STATUS_C;
      /* $4E36 ADC */ tmp = 0xb0, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4E38 LDX */ s_x = update_nz(0x27);
      /* $4E3A LDY */ s_y = update_nz(peek_zpg(0xfc));
      /* $4E3C JSR */ push16(0x4e3e), s_pc = 0x5108;
      branchTarget = true;
      break;
    case 0x4e3f: // [$4E3F..$4E4D]   15 bytes
      CYCLES(0x4e3f, 26);
      /* $4E3F LDA */ s_a = update_nz(peek_zpg(0xfc));
      /* $4E41 SEC */ s_status |= STATUS_C;
      /* $4E42 SBC */ tmp = 0x08, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $4E44 STA */ poke_zpg(0xfc, s_a);
      /* $4E46 LDA */ s_a = update_nz(peek_zpg(0x00));
      /* $4E48 ORA */ s_a = update_nz(s_a | peek_zpg(0x01));
      /* $4E4A ORA */ s_a = update_nz(s_a | peek_zpg(0x02));
      /* $4E4C BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4e32 : 0x4e4e;
      branchTarget = true;
      break;
    case 0x4e4e: // [$4E4E..$4E4E]    1 bytes
      CYCLES(0x4e4e, 2);
      /* $4E4E RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4e4f: // [$4E4F..$4E6F]   33 bytes
      CYCLES(0x4e4f, 57);
      /* $4E4F LDA */ s_a = update_nz(peek(0x0800));
      /* $4E52 STA */ poke_zpg(0x06, s_a);
      /* $4E54 LDA */ s_a = update_nz(peek(0x0820));
      /* $4E57 STA */ poke_zpg(0x07, s_a);
      /* $4E59 LDY */ s_y = update_nz(0x02);
      /* $4E5B LDA */ s_a = update_nz(peek(peek16_zpg(0x06) + s_y));
      /* $4E5D STA */ poke_zpg(0x0a, s_a);
      /* $4E5F INY */ s_y = update_nz(s_y + 1);
      /* $4E60 LDA */ s_a = update_nz(peek(peek16_zpg(0x06) + s_y));
      /* $4E62 STA */ poke_zpg(0x0b, s_a);
      /* $4E64 LDX */ s_x = update_nz(peek(0x1509));
      /* $4E67 DEX */ s_x = update_nz(s_x - 1);
      /* $4E68 STX */ poke_zpg(0x04, s_x);
      /* $4E6A LDA */ s_a = update_nz(0x60);
      /* $4E6C STA */ poke_zpg(0xfc, s_a);
      /* $4E6E STA */ poke_zpg(0xfe, s_a);
    case 0x4e70: // [$4E70..$4E73]    4 bytes
      CYCLES(0x4e70, 7);
      /* $4E70 DEC */ tmp16 = 0x04, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $4E72 BMI */ s_pc = s_status & STATUS_N ? 0x4ea7 : 0x4e74;
      branchTarget = true;
      break;
    case 0x4e74: // [$4E74..$4E7F]   12 bytes
      CYCLES(0x4e74, 21);
      /* $4E74 LDA */ s_a = update_nz(peek_zpg(0x0a));
      /* $4E76 STA */ poke_zpg(0x08, s_a);
      /* $4E78 LDA */ s_a = update_nz(peek_zpg(0x0b));
      /* $4E7A STA */ poke_zpg(0x09, s_a);
      /* $4E7C LDY */ s_y = update_nz(0x09);
      /* $4E7E LDX */ s_x = update_nz(0x00);
    case 0x4e80: // [$4E80..$4E98]   25 bytes
      CYCLES(0x4e80, 43);
      /* $4E80 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $4E82 STA */ poke_zpg(0x06, s_a);
      /* $4E84 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $4E86 STA */ poke_zpg(0x07, s_a);
      /* $4E88 STY */ poke_zpg(0x05, s_y);
      /* $4E8A LDY */ s_y = update_nz(0x27);
      /* $4E8C LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $4E8E STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $4E90 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $4E92 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $4E94 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $4E96 DEY */ s_y = update_nz(s_y - 1);
      /* $4E97 BPL */ s_pc = !(s_status & STATUS_N) ? 0x4e80 : 0x4e99;
      branchTarget = true;
      break;
    case 0x4e99: // [$4E99..$4EA5]   13 bytes
      CYCLES(0x4e99, 23);
      /* $4E99 LDA */ s_a = update_nz(peek_zpg(0xfc));
      /* $4E9B CLC */ s_status &= ~STATUS_C;
      /* $4E9C ADC */ tmp = 0x0b, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4E9E STA */ poke_zpg(0xfc, s_a);
      /* $4EA0 STA */ poke_zpg(0xfe, s_a);
      /* $4EA2 CMP */ update_nz_inv_c(s_a - 0xa5);
      /* $4EA4 BCC */ s_pc = !(s_status & STATUS_C) ? 0x4e70 : 0x4ea6;
      branchTarget = true;
      break;
    case 0x4ea6: // [$4EA6..$4EA6]    1 bytes
      CYCLES(0x4ea6, 2);
      /* $4EA6 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4ea7: // [$4EA7..$4EAC]    6 bytes
      CYCLES(0x4ea7, 11);
      /* $4EA7 LDA */ s_a = update_nz(peek_zpg(0xfc));
      /* $4EA9 CMP */ update_nz_inv_c(s_a - 0xa5);
      /* $4EAB BCS */ s_pc = s_status & STATUS_C ? 0x4ea6 : 0x4ead;
      branchTarget = true;
      break;
    case 0x4ead: // [$4EAD..$4EB0]    4 bytes
      CYCLES(0x4ead, 7);
      /* $4EAD LDY */ s_y = update_nz(0x09);
      /* $4EAF LDX */ s_x = update_nz(0x00);
    case 0x4eb1: // [$4EB1..$4EC2]   18 bytes
      CYCLES(0x4eb1, 31);
      /* $4EB1 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $4EB3 STA */ poke_zpg(0x06, s_a);
      /* $4EB5 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $4EB7 STA */ poke_zpg(0x07, s_a);
      /* $4EB9 LDY */ s_y = update_nz(0x27);
      /* $4EBB TXA */ s_a = update_nz(s_x);
      /* $4EBC LDA */ s_a = update_nz(peek(peek16_zpg(0x06) + s_y));
      /* $4EBE LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $4EC0 DEY */ s_y = update_nz(s_y - 1);
      /* $4EC1 BPL */ s_pc = !(s_status & STATUS_N) ? 0x4eb1 : 0x4ec3;
      branchTarget = true;
      break;
    case 0x4ec3: // [$4EC3..$4EC3]    1 bytes
      CYCLES(0x4ec3, 2);
      /* $4EC3 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4ec4: // [$4EC4..$4ECD]   10 bytes
      CYCLES(0x4ec4, 18);
      /* $4EC4 LDA */ s_a = update_nz(0x00);
      /* $4EC6 STA */ poke_zpg(0x03, s_a);
      /* $4EC8 STA */ poke_zpg(0x04, s_a);
      /* $4ECA STA */ poke_zpg(0x05, s_a);
      /* $4ECC LDX */ s_x = update_nz(0x18);
    case 0x4ece: // [$4ECE..$4ED8]   11 bytes
      CYCLES(0x4ece, 19);
      /* $4ECE ASL */ tmp16 = 0x00, poke_zpg(tmp16, update_nzc(peek_zpg(tmp16) << 1));
      /* $4ED0 ROL */ tmp16 = 0x01, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $4ED2 ROL */ tmp16 = 0x02, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $4ED4 ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $4ED5 CMP */ update_nz_inv_c(s_a - 0x0a);
      /* $4ED7 BCC */ s_pc = !(s_status & STATUS_C) ? 0x4edb : 0x4ed9;
      branchTarget = true;
      break;
    case 0x4ed9: // [$4ED9..$4EDA]    2 bytes
      CYCLES(0x4ed9, 4);
      /* $4ED9 SBC */ tmp = 0x0a, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
    case 0x4edb: // [$4EDB..$4EE3]    9 bytes
      CYCLES(0x4edb, 16);
      /* $4EDB ROL */ tmp16 = 0x03, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $4EDD ROL */ tmp16 = 0x04, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $4EDF ROL */ tmp16 = 0x05, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $4EE1 DEX */ s_x = update_nz(s_x - 1);
      /* $4EE2 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4ece : 0x4ee4;
      branchTarget = true;
      break;
    case 0x4ee4: // [$4EE4..$4EF0]   13 bytes
      CYCLES(0x4ee4, 23);
      /* $4EE4 LDX */ s_x = update_nz(peek_zpg(0x03));
      /* $4EE6 STX */ poke_zpg(0x00, s_x);
      /* $4EE8 LDX */ s_x = update_nz(peek_zpg(0x04));
      /* $4EEA STX */ poke_zpg(0x01, s_x);
      /* $4EEC LDX */ s_x = update_nz(peek_zpg(0x05));
      /* $4EEE STX */ poke_zpg(0x02, s_x);
      /* $4EF0 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4ef1: // [$4EF1..$4EF7]    7 bytes
      CYCLES(0x4ef1, 12);
      /* $4EF1 LDA */ s_a = update_nz(0x06);
      /* $4EF3 STA */ poke_zpg(0x00, s_a);
      /* $4EF5 JSR */ push16(0x4ef7), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x4ef8: // [$4EF8..$4EFF]    8 bytes
      CYCLES(0x4ef8, 14);
      /* $4EF8 STA */ poke_zpg(0xca, s_a);
      /* $4EFA LDY */ s_y = update_nz(0x00);
      /* $4EFC STY */ poke_zpg(0xfc, s_y);
      /* $4EFE STY */ poke_zpg(0xfe, s_y);
    case 0x4f00: // [$4F00..$4F09]   10 bytes
      CYCLES(0x4f00, 18);
      /* $4F00 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $4F02 STA */ poke_zpg(0x06, s_a);
      /* $4F04 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $4F06 STA */ poke_zpg(0x07, s_a);
      /* $4F08 LDX */ s_x = update_nz(0x20);
    case 0x4f0a: // [$4F0A..$4F0C]    3 bytes
      CYCLES(0x4f0a, 6);
      /* $4F0A JSR */ push16(0x4f0c), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x4f0d: // [$4F0D..$4F0E]    2 bytes
      CYCLES(0x4f0d, 4);
      /* $4F0D STA */ poke_zpg(0xcb, s_a);
    case 0x4f0f: // [$4F0F..$4F17]    9 bytes
      CYCLES(0x4f0f, 16);
      /* $4F0F LDA */ s_a = update_nz(peek(peek16_zpg(0x06) + s_y));
      /* $4F11 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $4F13 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $4F15 JSR */ push16(0x4f17), s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x4f18: // [$4F18..$4F1A]    3 bytes
      CYCLES(0x4f18, 6);
      /* $4F18 INY */ s_y = update_nz(s_y + 1);
      /* $4F19 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4f0f : 0x4f1b;
      branchTarget = true;
      break;
    case 0x4f1b: // [$4F1B..$4F1F]    5 bytes
      CYCLES(0x4f1b, 9);
      /* $4F1B INC */ tmp16 = 0x07, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $4F1D DEX */ s_x = update_nz(s_x - 1);
      /* $4F1E BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4f0a : 0x4f20;
      branchTarget = true;
      break;
    case 0x4f20: // [$4F20..$4F23]    4 bytes
      CYCLES(0x4f20, 7);
      /* $4F20 DEC */ tmp16 = 0x00, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $4F22 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4f00 : 0x4f24;
      branchTarget = true;
      break;
    case 0x4f24: // [$4F24..$4F24]    1 bytes
      CYCLES(0x4f24, 2);
      /* $4F24 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4f25: // [$4F25..$4F36]   18 bytes
      CYCLES(0x4f25, 31);
      /* $4F25 LDY */ s_y = update_nz(0x00);
      /* $4F27 STY */ poke_zpg(0xfc, s_y);
      /* $4F29 STY */ poke_zpg(0xfe, s_y);
      /* $4F2B LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $4F2D STA */ poke_zpg(0x06, s_a);
      /* $4F2F LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $4F31 STA */ poke_zpg(0x07, s_a);
      /* $4F33 LDX */ s_x = update_nz(0x20);
      /* $4F35 LDA */ s_a = update_nz(0x00);
    case 0x4f37: // [$4F37..$4F3B]    5 bytes
      CYCLES(0x4f37, 9);
      /* $4F37 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $4F39 INY */ s_y = update_nz(s_y + 1);
      /* $4F3A BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4f37 : 0x4f3c;
      branchTarget = true;
      break;
    case 0x4f3c: // [$4F3C..$4F40]    5 bytes
      CYCLES(0x4f3c, 9);
      /* $4F3C INC */ tmp16 = 0x07, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $4F3E DEX */ s_x = update_nz(s_x - 1);
      /* $4F3F BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4f37 : 0x4f41;
      branchTarget = true;
      break;
    case 0x4f41: // [$4F41..$4F41]    1 bytes
      CYCLES(0x4f41, 2);
      /* $4F41 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4f42: // [$4F42..$4F44]    3 bytes
      CYCLES(0x4f42, 6);
      /* $4F42 JSR */ push16(0x4f44), s_pc = 0x4f25;
      branchTarget = true;
      break;
    case 0x4f45: // [$4F45..$4F47]    3 bytes
      CYCLES(0x4f45, 6);
      /* $4F45 JSR */ push16(0x4f47), s_pc = 0x4e1f;
      branchTarget = true;
      break;
    case 0x4f48: // [$4F48..$4F4A]    3 bytes
      CYCLES(0x4f48, 6);
      /* $4F48 JSR */ push16(0x4f4a), s_pc = 0x4e4f;
      branchTarget = true;
      break;
    case 0x4f4b: // [$4F4B..$4F5A]   16 bytes
      CYCLES(0x4f4b, 28);
      /* $4F4B LDX */ s_x = update_nz(peek(0x1407));
      /* $4F4E INX */ s_x = update_nz(s_x + 1);
      /* $4F4F STX */ poke_zpg(0x00, s_x);
      /* $4F51 LDA */ s_a = update_nz(0x00);
      /* $4F53 STA */ poke_zpg(0x01, s_a);
      /* $4F55 STA */ poke_zpg(0x02, s_a);
      /* $4F57 LDA */ s_a = update_nz(0xb8);
      /* $4F59 STA */ poke_zpg(0xfc, s_a);
    case 0x4f5b: // [$4F5B..$4F5D]    3 bytes
      CYCLES(0x4f5b, 6);
      /* $4F5B JSR */ push16(0x4f5d), s_pc = 0x4ec4;
      branchTarget = true;
      break;
    case 0x4f5e: // [$4F5E..$4F67]   10 bytes
      CYCLES(0x4f5e, 18);
      /* $4F5E CLC */ s_status &= ~STATUS_C;
      /* $4F5F ADC */ tmp = 0xb0, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $4F61 LDY */ s_y = update_nz(peek_zpg(0xfc));
      /* $4F63 LDX */ s_x = update_nz(0x27);
      /* $4F65 JSR */ push16(0x4f67), s_pc = 0x5108;
      branchTarget = true;
      break;
    case 0x4f68: // [$4F68..$4F72]   11 bytes
      CYCLES(0x4f68, 19);
      /* $4F68 LDA */ s_a = update_nz(peek_zpg(0xfc));
      /* $4F6A SEC */ s_status |= STATUS_C;
      /* $4F6B SBC */ tmp = 0x08, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $4F6D STA */ poke_zpg(0xfc, s_a);
      /* $4F6F LDA */ s_a = update_nz(peek_zpg(0x00));
      /* $4F71 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x4f5b : 0x4f73;
      branchTarget = true;
      break;
    case 0x4f73: // [$4F73..$4F73]    1 bytes
      CYCLES(0x4f73, 2);
      /* $4F73 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4f74: // [$4F74..$4F80]   13 bytes
      CYCLES(0x4f74, 23);
      /* $4F74 PHA */ push8(s_a);
      /* $4F75 LDA */ s_a = update_nz(peek(0x0c03));
      /* $4F78 SEC */ s_status |= STATUS_C;
      /* $4F79 SBC */ tmp = peek(0x1405), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $4F7C CMP */ update_nz_inv_c(s_a - peek(0x0c0b));
      /* $4F7F BCS */ s_pc = s_status & STATUS_C ? 0x4f83 : 0x4f81;
      branchTarget = true;
      break;
    case 0x4f81: // [$4F81..$4F82]    2 bytes
      CYCLES(0x4f81, 4);
      /* $4F81 PLA */ s_a = update_nz(pop8());
      /* $4F82 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4f83: // [$4F83..$4F86]    4 bytes
      CYCLES(0x4f83, 7);
      /* $4F83 TXA */ s_a = update_nz(s_x);
      /* $4F84 JSR */ push16(0x4f86), s_pc = 0x4c1c;
      branchTarget = true;
      break;
    case 0x4f87: // [$4F87..$4F8C]    6 bytes
      CYCLES(0x4f87, 11);
      /* $4F87 STA */ poke_zpg(0x04, s_a);
      /* $4F89 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $4F8A PLA */ s_a = update_nz(pop8());
      /* $4F8B BCC */ s_pc = !(s_status & STATUS_C) ? 0x4f9c : 0x4f8d;
      branchTarget = true;
      break;
    case 0x4f8d: // [$4F8D..$4F97]   11 bytes
      CYCLES(0x4f8d, 19);
      /* $4F8D PHA */ push8(s_a);
      /* $4F8E AND */ s_a = update_nz(s_a & 0x80);
      /* $4F90 STA */ poke_zpg(0x06, s_a);
      /* $4F92 PLA */ s_a = update_nz(pop8());
      /* $4F93 AND */ s_a = update_nz(s_a & 0x7f);
      /* $4F95 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $4F96 BCS */ s_pc = s_status & STATUS_C ? 0x4f9a : 0x4f98;
      branchTarget = true;
      break;
    case 0x4f98: // [$4F98..$4F99]    2 bytes
      CYCLES(0x4f98, 4);
      /* $4F98 ORA */ s_a = update_nz(s_a | 0x40);
    case 0x4f9a: // [$4F9A..$4F9B]    2 bytes
      CYCLES(0x4f9a, 4);
      /* $4F9A ORA */ s_a = update_nz(s_a | peek_zpg(0x06));
    case 0x4f9c: // [$4F9C..$4FA6]   11 bytes
      CYCLES(0x4f9c, 19);
      /* $4F9C STA */ poke_zpg(0x06, s_a);
      /* $4F9E STY */ poke_zpg(0x05, s_y);
      /* $4FA0 LDY */ s_y = update_nz(peek(0x0c0b));
      /* $4FA3 DEY */ s_y = update_nz(s_y - 1);
      /* $4FA4 LDX */ s_x = update_nz(peek(0x1405));
    case 0x4fa7: // [$4FA7..$4FC8]   34 bytes
      CYCLES(0x4fa7, 58);
      /* $4FA7 LDA */ s_a = update_nz(peek_zpg(0x05));
      /* $4FA9 STA */ poke(0x1770 + s_x, s_a);
      /* $4FAC LDA */ s_a = update_nz(peek_zpg(0x04));
      /* $4FAE STA */ poke(0x17c0 + s_x, s_a);
      /* $4FB1 LDA */ s_a = update_nz(peek_zpg(0x06));
      /* $4FB3 AND */ s_a = update_nz(s_a & peek(0x0d0c + s_y));
      /* $4FB6 STA */ poke(0x1810 + s_x, s_a);
      /* $4FB9 LDA */ s_a = update_nz(peek(0x0d04 + s_y));
      /* $4FBC STA */ poke(0x1860 + s_x, s_a);
      /* $4FBF LDA */ s_a = update_nz(peek(0x0c07));
      /* $4FC2 STA */ poke(0x18b0 + s_x, s_a);
      /* $4FC5 INX */ s_x = update_nz(s_x + 1);
      /* $4FC6 DEY */ s_y = update_nz(s_y - 1);
      /* $4FC7 BPL */ s_pc = !(s_status & STATUS_N) ? 0x4fa7 : 0x4fc9;
      branchTarget = true;
      break;
    case 0x4fc9: // [$4FC9..$4FCC]    4 bytes
      CYCLES(0x4fc9, 7);
      /* $4FC9 STX */ poke(0x1405, s_x);
      /* $4FCC RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x4fcd: // [$4FCD..$4FCF]    3 bytes
      CYCLES(0x4fcd, 6);
      /* $4FCD JSR */ push16(0x4fcf), s_pc = 0x4c7c;
      branchTarget = true;
      break;
    case 0x4fd0: // [$4FD0..$4FFA]   43 bytes
      CYCLES(0x4fd0, 74);
      /* $4FD0 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $4FD2 STA */ poke_zpg(0x06, s_a);
      /* $4FD4 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $4FD6 STA */ poke_zpg(0x07, s_a);
      /* $4FD8 STY */ poke_zpg(0x05, s_y);
      /* $4FDA LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $4FDC LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $4FDE AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $4FE0 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $4FE2 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $4FE4 INY */ s_y = update_nz(s_y + 1);
      /* $4FE5 LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $4FE7 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $4FE9 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $4FEB INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $4FED INY */ s_y = update_nz(s_y + 1);
      /* $4FEE LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $4FF0 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $4FF2 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $4FF4 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $4FF6 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $4FF8 DEY */ s_y = update_nz(s_y - 1);
      /* $4FF9 BPL */ s_pc = !(s_status & STATUS_N) ? 0x4fd0 : 0x4ffb;
      branchTarget = true;
      break;
    case 0x4ffb: // [$4FFB..$4FFD]    3 bytes
      CYCLES(0x4ffb, 6);
      /* $4FFB JMP */ s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x4ffe: // [$4FFE..$5000]    3 bytes
      CYCLES(0x4ffe, 6);
      /* $4FFE JSR */ push16(0x5000), s_pc = 0x4c7c;
      branchTarget = true;
      break;
    case 0x5001: // [$5001..$5029]   41 bytes
      CYCLES(0x5001, 70);
      /* $5001 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $5003 STA */ poke_zpg(0x06, s_a);
      /* $5005 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $5007 STA */ poke_zpg(0x07, s_a);
      /* $5009 STY */ poke_zpg(0x05, s_y);
      /* $500B LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $500D LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $500F STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $5011 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $5013 INY */ s_y = update_nz(s_y + 1);
      /* $5014 LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $5016 ORA */ s_a = update_nz(s_a | peek(peek16_zpg(0x06) + s_y));
      /* $5018 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $501A INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $501C INY */ s_y = update_nz(s_y + 1);
      /* $501D LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $501F ORA */ s_a = update_nz(s_a | peek(peek16_zpg(0x06) + s_y));
      /* $5021 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $5023 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $5025 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $5027 DEY */ s_y = update_nz(s_y - 1);
      /* $5028 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5001 : 0x502a;
      branchTarget = true;
      break;
    case 0x502a: // [$502A..$502C]    3 bytes
      CYCLES(0x502a, 6);
      /* $502A JMP */ s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x502d: // [$502D..$502F]    3 bytes
      CYCLES(0x502d, 6);
      /* $502D JSR */ push16(0x502f), s_pc = 0x4c7c;
      branchTarget = true;
      break;
    case 0x5030: // [$5030..$5055]   38 bytes
      CYCLES(0x5030, 65);
      /* $5030 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $5032 STA */ poke_zpg(0x06, s_a);
      /* $5034 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $5036 STA */ poke_zpg(0x07, s_a);
      /* $5038 STY */ poke_zpg(0x05, s_y);
      /* $503A LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $503C LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $503E EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $5040 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $5042 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $5044 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $5046 INY */ s_y = update_nz(s_y + 1);
      /* $5047 LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $5049 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $504B AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $504D STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $504F INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $5051 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $5053 DEY */ s_y = update_nz(s_y - 1);
      /* $5054 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5030 : 0x5056;
      branchTarget = true;
      break;
    case 0x5056: // [$5056..$5058]    3 bytes
      CYCLES(0x5056, 6);
      /* $5056 JMP */ s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x5059: // [$5059..$505B]    3 bytes
      CYCLES(0x5059, 6);
      /* $5059 JSR */ push16(0x505b), s_pc = 0x4c7c;
      branchTarget = true;
      break;
    case 0x505c: // [$505C..$507D]   34 bytes
      CYCLES(0x505c, 58);
      /* $505C LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $505E STA */ poke_zpg(0x06, s_a);
      /* $5060 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $5062 STA */ poke_zpg(0x07, s_a);
      /* $5064 STY */ poke_zpg(0x05, s_y);
      /* $5066 LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $5068 LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $506A ORA */ s_a = update_nz(s_a | peek(peek16_zpg(0x06) + s_y));
      /* $506C STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $506E INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $5070 INY */ s_y = update_nz(s_y + 1);
      /* $5071 LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $5073 ORA */ s_a = update_nz(s_a | peek(peek16_zpg(0x06) + s_y));
      /* $5075 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $5077 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $5079 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $507B DEY */ s_y = update_nz(s_y - 1);
      /* $507C BPL */ s_pc = !(s_status & STATUS_N) ? 0x505c : 0x507e;
      branchTarget = true;
      break;
    case 0x507e: // [$507E..$5080]    3 bytes
      CYCLES(0x507e, 6);
      /* $507E JMP */ s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x5081: // [$5081..$50A0]   32 bytes
      CYCLES(0x5081, 55);
      /* $5081 LDA */ s_a = update_nz(peek(0x0960 + s_x));
      /* $5084 STA */ poke_zpg(0x06, s_a);
      /* $5086 LDA */ s_a = update_nz(peek(0x0964 + s_x));
      /* $5089 STA */ poke_zpg(0x07, s_a);
      /* $508B LDY */ s_y = update_nz(0x00);
      /* $508D LDA */ s_a = update_nz(peek(peek16_zpg(0x06) + s_y));
      /* $508F STA */ poke_zpg(0xe6, s_a);
      /* $5091 INY */ s_y = update_nz(s_y + 1);
      /* $5092 LDA */ s_a = update_nz(peek(peek16_zpg(0x06) + s_y));
      /* $5094 STA */ poke_zpg(0xe7, s_a);
      /* $5096 INY */ s_y = update_nz(s_y + 1);
      /* $5097 LDA */ s_a = update_nz(peek(peek16_zpg(0x06) + s_y));
      /* $5099 STA */ poke_zpg(0xe8, s_a);
      /* $509B INY */ s_y = update_nz(s_y + 1);
      /* $509C LDA */ s_a = update_nz(peek(peek16_zpg(0x06) + s_y));
      /* $509E STA */ poke_zpg(0xe9, s_a);
      /* $50A0 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x50a1: // [$50A1..$50AA]   10 bytes
      CYCLES(0x50a1, 18);
      /* $50A1 STY */ poke_zpg(0x07, s_y);
      /* $50A3 LDY */ s_y = update_nz(0x00);
      /* $50A5 STY */ poke_zpg(0x20, s_y);
      /* $50A7 LDA */ s_a = update_nz(0x07);
      /* $50A9 STA */ poke_zpg(0x21, s_a);
    case 0x50ab: // [$50AB..$50B0]    6 bytes
      CYCLES(0x50ab, 11);
      /* $50AB LDA */ s_a = update_nz(peek_zpg(0x4e));
      /* $50AD STA */ poke_zpg(0x06, s_a);
      /* $50AF LDX */ s_x = update_nz(0x07);
    case 0x50b1: // [$50B1..$50B3]    3 bytes
      CYCLES(0x50b1, 6);
      /* $50B1 JSR */ push16(0x50b3), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x50b4: // [$50B4..$50BA]    7 bytes
      CYCLES(0x50b4, 12);
      /* $50B4 CMP */ update_nz_inv_c(s_a - peek_zpg(0x0a));
      /* $50B6 ROL */ tmp16 = 0x06, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $50B8 DEX */ s_x = update_nz(s_x - 1);
      /* $50B9 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x50b1 : 0x50bb;
      branchTarget = true;
      break;
    case 0x50bb: // [$50BB..$50C3]    9 bytes
      CYCLES(0x50bb, 16);
      /* $50BB LDA */ s_a = update_nz(peek_zpg(0x06));
      /* $50BD STA */ poke(peek16_zpg(0x20) + s_y, s_a);
      /* $50BF INY */ s_y = update_nz(s_y + 1);
      /* $50C0 CPY */ update_nz_inv_c(s_y - peek_zpg(0x07));
      /* $50C2 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x50ab : 0x50c4;
      branchTarget = true;
      break;
    case 0x50c4: // [$50C4..$50C4]    1 bytes
      CYCLES(0x50c4, 2);
      /* $50C4 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x50c5: // [$50C5..$50C9]    5 bytes
      CYCLES(0x50c5, 9);
      /* $50C5 LDY */ s_y = update_nz(0x00);
      /* $50C7 JSR */ push16(0x50c9), s_pc = 0x50a1;
      branchTarget = true;
      break;
    case 0x50ca: // [$50CA..$50DD]   20 bytes
      CYCLES(0x50ca, 35);
      /* $50CA LDA */ s_a = update_nz(peek(0x0c5b));
      /* $50CD STA */ poke_zpg(0xfc, s_a);
      /* $50CF STA */ poke_zpg(0xfe, s_a);
      /* $50D1 LDA */ s_a = update_nz(peek_zpg(0xe8));
      /* $50D3 STA */ poke_zpg(0x08, s_a);
      /* $50D5 LDA */ s_a = update_nz(peek_zpg(0xe9));
      /* $50D7 STA */ poke_zpg(0x09, s_a);
      /* $50D9 LDY */ s_y = update_nz(peek_zpg(0xe7));
      /* $50DB DEY */ s_y = update_nz(s_y - 1);
      /* $50DC LDX */ s_x = update_nz(0x00);
    case 0x50de: // [$50DE..$50EE]   17 bytes
      CYCLES(0x50de, 29);
      /* $50DE LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $50E0 STA */ poke_zpg(0x06, s_a);
      /* $50E2 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $50E4 STA */ poke_zpg(0x07, s_a);
      /* $50E6 STY */ poke_zpg(0x01, s_y);
      /* $50E8 LDA */ s_a = update_nz(peek_zpg(0xe6));
      /* $50EA STA */ poke_zpg(0x00, s_a);
      /* $50EC LDY */ s_y = update_nz(peek(0x0c5a));
    case 0x50ef: // [$50EF..$50FA]   12 bytes
      CYCLES(0x50ef, 21);
      /* $50EF LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $50F1 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x20 + s_x)));
      /* $50F3 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $50F5 INC */ tmp16 = 0x20, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $50F7 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $50F9 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x50fd : 0x50fb;
      branchTarget = true;
      break;
    case 0x50fb: // [$50FB..$50FC]    2 bytes
      CYCLES(0x50fb, 4);
      /* $50FB INC */ tmp16 = 0x09, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
    case 0x50fd: // [$50FD..$5101]    5 bytes
      CYCLES(0x50fd, 9);
      /* $50FD INY */ s_y = update_nz(s_y + 1);
      /* $50FE DEC */ tmp16 = 0x00, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $5100 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x50ef : 0x5102;
      branchTarget = true;
      break;
    case 0x5102: // [$5102..$5106]    5 bytes
      CYCLES(0x5102, 9);
      /* $5102 LDY */ s_y = update_nz(peek_zpg(0x01));
      /* $5104 DEY */ s_y = update_nz(s_y - 1);
      /* $5105 BPL */ s_pc = !(s_status & STATUS_N) ? 0x50de : 0x5107;
      branchTarget = true;
      break;
    case 0x5107: // [$5107..$5107]    1 bytes
      CYCLES(0x5107, 2);
      /* $5107 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5108: // [$5108..$511B]   20 bytes
      CYCLES(0x5108, 35);
      /* $5108 STY */ poke_zpg(0xfc, s_y);
      /* $510A STY */ poke_zpg(0xfe, s_y);
      /* $510C LDY */ s_y = update_nz(0x40);
      /* $510E STY */ poke_zpg(0x09, s_y);
      /* $5110 SEC */ s_status |= STATUS_C;
      /* $5111 SBC */ tmp = 0x20, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $5113 ASL */ s_a = update_nzc(s_a << 1);
      /* $5114 ASL */ s_a = update_nzc(s_a << 1);
      /* $5115 ASL */ s_a = update_nzc(s_a << 1);
      /* $5116 ROL */ tmp16 = 0x09, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $5118 STA */ poke_zpg(0x08, s_a);
      /* $511A LDY */ s_y = update_nz(0x07);
    case 0x511c: // [$511C..$512D]   18 bytes
      CYCLES(0x511c, 31);
      /* $511C LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      // WARNING: performs self modification.
      /* $511E STA */ poke(0x5129, s_a);
      /* $5121 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      // WARNING: performs self modification.
      /* $5123 STA */ poke(0x512a, s_a);
      /* $5126 LDA */ s_a = update_nz(peek(peek16_zpg(0x08) + s_y));
      // WARNING: operand self modification.
      /* $5128 STA */ poke(ram_peek16(0x5129) + s_x, s_a);
      /* $512B DEY */ s_y = update_nz(s_y - 1);
      /* $512C BPL */ s_pc = !(s_status & STATUS_N) ? 0x511c : 0x512e;
      branchTarget = true;
      break;
    case 0x512e: // [$512E..$512E]    1 bytes
      CYCLES(0x512e, 2);
      /* $512E RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x512f: // [$512F..$513C]   14 bytes
      CYCLES(0x512f, 24);
      /* $512F LDA */ s_a = update_nz(0x00);
      /* $5131 STA */ poke_zpg(0x00, s_a);
      /* $5133 STA */ poke_zpg(0x08, s_a);
      /* $5135 LDA */ s_a = update_nz(0x7a);
      /* $5137 STA */ poke_zpg(0x01, s_a);
      /* $5139 LDA */ s_a = update_nz(0x90);
      /* $513B STA */ poke_zpg(0x09, s_a);
    case 0x513d: // [$513D..$5142]    6 bytes
      CYCLES(0x513d, 11);
      /* $513D LDY */ s_y = update_nz(0x00);
      /* $513F LDA */ s_a = update_nz(peek(peek16_zpg(0x00) + s_y));
      /* $5141 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5144 : 0x5143;
      branchTarget = true;
      break;
    case 0x5143: // [$5143..$5143]    1 bytes
      CYCLES(0x5143, 2);
      /* $5143 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5144: // [$5144..$514A]    7 bytes
      CYCLES(0x5144, 12);
      /* $5144 TAX */ s_x = update_nz(s_a);
      /* $5145 INY */ s_y = update_nz(s_y + 1);
      /* $5146 LDA */ s_a = update_nz(peek(peek16_zpg(0x00) + s_y));
      /* $5148 JSR */ push16(0x514a), s_pc = 0x4c4f;
      branchTarget = true;
      break;
    case 0x514b: // [$514B..$5150]    6 bytes
      CYCLES(0x514b, 11);
      /* $514B STX */ poke_zpg(0x04, s_x);
      /* $514D LDA */ s_a = update_nz(0x02);
      /* $514F STA */ poke_zpg(0x02, s_a);
    case 0x5151: // [$5151..$5162]   18 bytes
      CYCLES(0x5151, 31);
      /* $5151 LDY */ s_y = update_nz(peek_zpg(0x02));
      /* $5153 LDA */ s_a = update_nz(peek(peek16_zpg(0x00) + s_y));
      /* $5155 STA */ poke_zpg(0x06, s_a);
      /* $5157 INY */ s_y = update_nz(s_y + 1);
      /* $5158 LDA */ s_a = update_nz(peek(peek16_zpg(0x00) + s_y));
      /* $515A STA */ poke_zpg(0x07, s_a);
      /* $515C INY */ s_y = update_nz(s_y + 1);
      /* $515D STY */ poke_zpg(0x02, s_y);
      /* $515F CPY */ update_nz_inv_c(s_y - 0x10);
      /* $5161 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5173 : 0x5163;
      branchTarget = true;
      break;
    case 0x5163: // [$5163..$5172]   16 bytes
      CYCLES(0x5163, 28);
      /* $5163 LDA */ s_a = update_nz(peek_zpg(0x00));
      /* $5165 CLC */ s_status &= ~STATUS_C;
      /* $5166 ADC */ tmp = 0x10, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5168 STA */ poke_zpg(0x00, s_a);
      /* $516A LDA */ s_a = update_nz(peek_zpg(0x01));
      /* $516C ADC */ tmp = 0x00, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $516E STA */ poke_zpg(0x01, s_a);
      /* $5170 SEC */ s_status |= STATUS_C;
      /* $5171 BCS */ s_pc = s_status & STATUS_C ? 0x513d : 0x5173;
      branchTarget = true;
      break;
    case 0x5173: // [$5173..$5179]    7 bytes
      CYCLES(0x5173, 12);
      /* $5173 LDA */ s_a = update_nz(peek_zpg(0x08));
      /* $5175 CLC */ s_status &= ~STATUS_C;
      /* $5176 ADC */ tmp = peek_zpg(0x04), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5178 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5180 : 0x517a;
      branchTarget = true;
      break;
    case 0x517a: // [$517A..$517F]    6 bytes
      CYCLES(0x517a, 11);
      /* $517A INC */ tmp16 = 0x09, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $517C LDA */ s_a = update_nz(0x00);
      /* $517E STA */ poke_zpg(0x08, s_a);
    case 0x5180: // [$5180..$5190]   17 bytes
      CYCLES(0x5180, 29);
      /* $5180 LDA */ s_a = update_nz(peek_zpg(0x08));
      /* $5182 STA */ poke(peek16_zpg(0x00) + s_y, s_a);
      /* $5184 INY */ s_y = update_nz(s_y + 1);
      /* $5185 LDA */ s_a = update_nz(peek_zpg(0x09));
      /* $5187 STA */ poke(peek16_zpg(0x00) + s_y, s_a);
      /* $5189 LDY */ s_y = update_nz(0x00);
      /* $518B CLC */ s_status &= ~STATUS_C;
      /* $518C PHP */ push8(s_status | STATUS_B);
      /* $518D LDA */ s_a = update_nz(peek_zpg(0x04));
      /* $518F STA */ poke_zpg(0x05, s_a);
    case 0x5191: // [$5191..$51A6]   22 bytes
      CYCLES(0x5191, 38);
      /* $5191 LDA */ s_a = update_nz(peek(peek16_zpg(0x06) + s_y));
      /* $5193 AND */ s_a = update_nz(s_a & 0x80);
      /* $5195 STA */ poke_zpg(0x0a, s_a);
      /* $5197 LDA */ s_a = update_nz(peek(peek16_zpg(0x06) + s_y));
      /* $5199 PLP */ s_status = pop8() & ~STATUS_B;
      /* $519A ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $519B ASL */ s_a = update_nzc(s_a << 1);
      /* $519C PHP */ push8(s_status | STATUS_B);
      /* $519D LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $519E ORA */ s_a = update_nz(s_a | peek_zpg(0x0a));
      /* $51A0 STA */ poke(peek16_zpg(0x08) + s_y, s_a);
      /* $51A2 INY */ s_y = update_nz(s_y + 1);
      /* $51A3 DEC */ tmp16 = 0x05, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $51A5 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5191 : 0x51a7;
      branchTarget = true;
      break;
    case 0x51a7: // [$51A7..$51B7]   17 bytes
      CYCLES(0x51a7, 29);
      /* $51A7 PLP */ s_status = pop8() & ~STATUS_B;
      /* $51A8 LDA */ s_a = update_nz(peek_zpg(0x08));
      /* $51AA CLC */ s_status &= ~STATUS_C;
      /* $51AB ADC */ tmp = peek_zpg(0x04), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $51AD STA */ poke_zpg(0x08, s_a);
      /* $51AF LDA */ s_a = update_nz(peek_zpg(0x09));
      /* $51B1 ADC */ tmp = 0x00, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $51B3 STA */ poke_zpg(0x09, s_a);
      /* $51B5 SEC */ s_status |= STATUS_C;
      /* $51B6 BCS */ s_pc = s_status & STATUS_C ? 0x5151 : 0x51b8;
      branchTarget = true;
      break;
    case 0x51b8: // [$51B8..$51BD]    6 bytes
      CYCLES(0x51b8, 11);
      /* $51B8 PLA */ s_a = update_nz(pop8());
      /* $51B9 STA */ poke_zpg(0x22, s_a);
      /* $51BB PLA */ s_a = update_nz(pop8());
      /* $51BC STA */ poke_zpg(0x23, s_a);
    case 0x51be: // [$51BE..$51C0]    3 bytes
      CYCLES(0x51be, 6);
      /* $51BE JSR */ push16(0x51c0), s_pc = 0x51e8;
      branchTarget = true;
      break;
    case 0x51c1: // [$51C1..$51C3]    3 bytes
      CYCLES(0x51c1, 6);
      /* $51C1 TAX */ s_x = update_nz(s_a);
      /* $51C2 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x51cb : 0x51c4;
      branchTarget = true;
      break;
    case 0x51c4: // [$51C4..$51CA]    7 bytes
      CYCLES(0x51c4, 12);
      /* $51C4 LDA */ s_a = update_nz(peek_zpg(0x23));
      /* $51C6 PHA */ push8(s_a);
      /* $51C7 LDA */ s_a = update_nz(peek_zpg(0x22));
      /* $51C9 PHA */ push8(s_a);
      /* $51CA RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x51cb: // [$51CB..$51CE]    4 bytes
      CYCLES(0x51cb, 7);
      /* $51CB CMP */ update_nz_inv_c(s_a - 0x0d);
      /* $51CD BNE */ s_pc = !(s_status & STATUS_Z) ? 0x51dc : 0x51cf;
      branchTarget = true;
      break;
    case 0x51cf: // [$51CF..$51D1]    3 bytes
      CYCLES(0x51cf, 6);
      /* $51CF JSR */ push16(0x51d1), s_pc = 0x51e8;
      branchTarget = true;
      break;
    case 0x51d2: // [$51D2..$51D6]    5 bytes
      CYCLES(0x51d2, 9);
      /* $51D2 STA */ poke_zpg(0xe0, s_a);
      /* $51D4 JSR */ push16(0x51d6), s_pc = 0x51e8;
      branchTarget = true;
      break;
    case 0x51d7: // [$51D7..$51DB]    5 bytes
      CYCLES(0x51d7, 9);
      /* $51D7 STA */ poke_zpg(0xe1, s_a);
      /* $51D9 JMP */ s_pc = 0x51be;
      branchTarget = true;
      break;
    case 0x51dc: // [$51DC..$51E2]    7 bytes
      CYCLES(0x51dc, 12);
      /* $51DC LDX */ s_x = update_nz(peek_zpg(0xe0));
      /* $51DE LDY */ s_y = update_nz(peek_zpg(0xe1));
      /* $51E0 JSR */ push16(0x51e2), s_pc = 0x5108;
      branchTarget = true;
      break;
    case 0x51e3: // [$51E3..$51E7]    5 bytes
      CYCLES(0x51e3, 9);
      /* $51E3 INC */ tmp16 = 0xe0, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $51E5 JMP */ s_pc = 0x51be;
      branchTarget = true;
      break;
    case 0x51e8: // [$51E8..$51EB]    4 bytes
      CYCLES(0x51e8, 7);
      /* $51E8 INC */ tmp16 = 0x22, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $51EA BNE */ s_pc = !(s_status & STATUS_Z) ? 0x51ee : 0x51ec;
      branchTarget = true;
      break;
    case 0x51ec: // [$51EC..$51ED]    2 bytes
      CYCLES(0x51ec, 4);
      /* $51EC INC */ tmp16 = 0x23, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
    case 0x51ee: // [$51EE..$51F2]    5 bytes
      CYCLES(0x51ee, 9);
      /* $51EE LDY */ s_y = update_nz(0x00);
      /* $51F0 LDA */ s_a = update_nz(peek(peek16_zpg(0x22) + s_y));
      /* $51F2 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5200: // [$5200..$5202]    3 bytes
      CYCLES(0x5200, 6);
      /* $5200 JMP */ s_pc = 0x520c;
      branchTarget = true;
      break;
    case 0x5203: // [$5203..$5205]    3 bytes
      CYCLES(0x5203, 6);
      /* $5203 JMP */ s_pc = 0x522e;
      branchTarget = true;
      break;
    case 0x5206: // [$5206..$5208]    3 bytes
      CYCLES(0x5206, 6);
      /* $5206 JMP */ s_pc = 0x551d;
      branchTarget = true;
      break;
    case 0x5209: // [$5209..$520B]    3 bytes
      CYCLES(0x5209, 6);
      /* $5209 JMP */ s_pc = 0x53ec;
      branchTarget = true;
      break;
    case 0x520c: // [$520C..$522D]   34 bytes
      CYCLES(0x520c, 58);
      /* $520C LDA */ s_a = update_nz(0x78);
      /* $520E STA */ poke(0x1500, s_a);
      /* $5211 STA */ poke(0x1502, s_a);
      /* $5214 LDA */ s_a = update_nz(0x5a);
      /* $5216 STA */ poke(0x1501, s_a);
      /* $5219 STA */ poke(0x1503, s_a);
      /* $521C LDA */ s_a = update_nz(0x00);
      /* $521E STA */ poke(0x1504, s_a);
      /* $5221 STA */ poke(0x1505, s_a);
      /* $5224 STA */ poke(0x1506, s_a);
      /* $5227 STA */ poke(0x1507, s_a);
      /* $522A STA */ poke(0x1508, s_a);
      /* $522D RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x522e: // [$522E..$5232]    5 bytes
      CYCLES(0x522e, 9);
      /* $522E LDA */ s_a = update_nz(peek(0x1421));
      /* $5231 BEQ */ s_pc = s_status & STATUS_Z ? 0x527d : 0x5233;
      branchTarget = true;
      break;
    case 0x5233: // [$5233..$523A]    8 bytes
      CYCLES(0x5233, 14);
      /* $5233 DEC */ tmp16 = 0x1421, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $5236 CMP */ update_nz_inv_c(s_a - peek(0x0c02));
      /* $5239 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x527b : 0x523b;
      branchTarget = true;
      break;
    case 0x523b: // [$523B..$5247]   13 bytes
      CYCLES(0x523b, 23);
      /* $523B LDY */ s_y = update_nz(peek(0x1405));
      /* $523E LDA */ s_a = update_nz(0x0b);
      /* $5240 STA */ poke_zpg(0x00, s_a);
      /* $5242 LDA */ s_a = update_nz(peek(0x1500));
      /* $5245 JSR */ push16(0x5247), s_pc = 0x4c1c;
      branchTarget = true;
      break;
    case 0x5248: // [$5248..$5249]    2 bytes
      CYCLES(0x5248, 4);
      /* $5248 STA */ poke_zpg(0x02, s_a);
    case 0x524a: // [$524A..$5262]   25 bytes
      CYCLES(0x524a, 43);
      /* $524A LDA */ s_a = update_nz(peek_zpg(0x02));
      /* $524C STA */ poke(0x17c0 + s_y, s_a);
      /* $524F LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $5251 LDA */ s_a = update_nz(peek(0x0d26 + s_x));
      /* $5254 STA */ poke(0x1810 + s_y, s_a);
      /* $5257 LDA */ s_a = update_nz(peek(0x0d32 + s_x));
      /* $525A STA */ poke(0x1860 + s_y, s_a);
      /* $525D LDX */ s_x = update_nz(peek(0x0c02));
      /* $5260 JSR */ push16(0x5262), s_pc = 0x4c4f;
      branchTarget = true;
      break;
    case 0x5263: // [$5263..$5277]   21 bytes
      CYCLES(0x5263, 36);
      /* $5263 STX */ poke_zpg(0x06, s_x);
      /* $5265 LDA */ s_a = update_nz(0x60);
      /* $5267 SEC */ s_status |= STATUS_C;
      /* $5268 SBC */ tmp = peek_zpg(0x06), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $526A STA */ poke(0x1770 + s_y, s_a);
      /* $526D LDA */ s_a = update_nz(peek(0x0c02));
      /* $5270 STA */ poke(0x18b0 + s_y, s_a);
      /* $5273 INY */ s_y = update_nz(s_y + 1);
      /* $5274 DEC */ tmp16 = 0x00, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $5276 BPL */ s_pc = !(s_status & STATUS_N) ? 0x524a : 0x5278;
      branchTarget = true;
      break;
    case 0x5278: // [$5278..$527A]    3 bytes
      CYCLES(0x5278, 6);
      /* $5278 STY */ poke(0x1405, s_y);
    case 0x527b: // [$527B..$527C]    2 bytes
      CYCLES(0x527b, 4);
      /* $527B CLC */ s_status &= ~STATUS_C;
      /* $527C RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x527d: // [$527D..$529A]   30 bytes
      CYCLES(0x527d, 52);
      /* $527D LDA */ s_a = update_nz(peek(0x1506));
      /* $5280 AND */ s_a = update_nz(s_a & 0x03);
      /* $5282 STA */ poke(0x1504, s_a);
      /* $5285 LDA */ s_a = update_nz(peek(0x1506));
      /* $5288 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $5289 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $528A STA */ poke(0x1505, s_a);
      /* $528D LDA */ s_a = update_nz(peek(0x1500));
      /* $5290 LDX */ s_x = update_nz(peek(0x1504));
      /* $5293 CLC */ s_status &= ~STATUS_C;
      /* $5294 ADC */ tmp = peek(0x0d00 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5297 CMP */ update_nz_inv_c(s_a - 0x05);
      /* $5299 BCC */ s_pc = !(s_status & STATUS_C) ? 0x529f : 0x529b;
      branchTarget = true;
      break;
    case 0x529b: // [$529B..$529E]    4 bytes
      CYCLES(0x529b, 7);
      /* $529B CMP */ update_nz_inv_c(s_a - 0xf4);
      /* $529D BCC */ s_pc = !(s_status & STATUS_C) ? 0x52a2 : 0x529f;
      branchTarget = true;
      break;
    case 0x529f: // [$529F..$52A1]    3 bytes
      CYCLES(0x529f, 6);
      /* $529F LDA */ s_a = update_nz(peek(0x1500));
    case 0x52a2: // [$52A2..$52B2]   17 bytes
      CYCLES(0x52a2, 29);
      /* $52A2 STA */ poke(0x1502, s_a);
      /* $52A5 LDA */ s_a = update_nz(peek(0x1501));
      /* $52A8 LDX */ s_x = update_nz(peek(0x1505));
      /* $52AB CLC */ s_status &= ~STATUS_C;
      /* $52AC ADC */ tmp = peek(0x0d00 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $52AF CMP */ update_nz_inv_c(s_a - 0x05);
      /* $52B1 BCC */ s_pc = !(s_status & STATUS_C) ? 0x52b7 : 0x52b3;
      branchTarget = true;
      break;
    case 0x52b3: // [$52B3..$52B6]    4 bytes
      CYCLES(0x52b3, 7);
      /* $52B3 CMP */ update_nz_inv_c(s_a - 0xb1);
      /* $52B5 BCC */ s_pc = !(s_status & STATUS_C) ? 0x52ba : 0x52b7;
      branchTarget = true;
      break;
    case 0x52b7: // [$52B7..$52B9]    3 bytes
      CYCLES(0x52b7, 6);
      /* $52B7 LDA */ s_a = update_nz(peek(0x1501));
    case 0x52ba: // [$52BA..$52D8]   31 bytes
      CYCLES(0x52ba, 53);
      /* $52BA STA */ poke(0x1503, s_a);
      /* $52BD LDA */ s_a = update_nz(peek(0x1507));
      /* $52C0 ASL */ s_a = update_nzc(s_a << 1);
      /* $52C1 ORA */ s_a = update_nz(s_a | peek(0x1508));
      /* $52C4 PHA */ push8(s_a);
      /* $52C5 TAX */ s_x = update_nz(s_a);
      /* $52C6 LDA */ s_a = update_nz(peek(0x0800 + s_x));
      /* $52C9 STA */ poke_zpg(0x06, s_a);
      /* $52CB LDA */ s_a = update_nz(peek(0x0820 + s_x));
      /* $52CE STA */ poke_zpg(0x07, s_a);
      /* $52D0 LDY */ s_y = update_nz(peek(0x1501));
      /* $52D3 LDX */ s_x = update_nz(peek(0x1500));
      /* $52D6 JSR */ push16(0x52d8), s_pc = 0x502d;
      branchTarget = true;
      break;
    case 0x52d9: // [$52D9..$52F0]   24 bytes
      CYCLES(0x52d9, 41);
      /* $52D9 PLA */ s_a = update_nz(pop8());
      /* $52DA TAX */ s_x = update_nz(s_a);
      /* $52DB LDA */ s_a = update_nz(peek(0x0840 + s_x));
      /* $52DE STA */ poke_zpg(0x06, s_a);
      /* $52E0 LDA */ s_a = update_nz(peek(0x0860 + s_x));
      /* $52E3 STA */ poke_zpg(0x07, s_a);
      /* $52E5 LDY */ s_y = update_nz(peek(0x1501));
      /* $52E8 DEY */ s_y = update_nz(s_y - 1);
      /* $52E9 LDX */ s_x = update_nz(peek(0x1500));
      /* $52EC DEX */ s_x = update_nz(s_x - 1);
      /* $52ED DEX */ s_x = update_nz(s_x - 1);
      /* $52EE JSR */ push16(0x52f0), s_pc = 0x4c7c;
      branchTarget = true;
      break;
    case 0x52f1: // [$52F1..$5302]   18 bytes
      CYCLES(0x52f1, 31);
      /* $52F1 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $52F3 STA */ poke_zpg(0x06, s_a);
      /* $52F5 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $52F7 STA */ poke_zpg(0x07, s_a);
      /* $52F9 STY */ poke_zpg(0x05, s_y);
      /* $52FB LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $52FD LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $52FF AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $5301 BEQ */ s_pc = s_status & STATUS_Z ? 0x5306 : 0x5303;
      branchTarget = true;
      break;
    case 0x5303: // [$5303..$5305]    3 bytes
      CYCLES(0x5303, 6);
      /* $5303 JSR */ push16(0x5305), s_pc = 0x538d;
      branchTarget = true;
      break;
    case 0x5306: // [$5306..$530E]    9 bytes
      CYCLES(0x5306, 16);
      /* $5306 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $5308 INY */ s_y = update_nz(s_y + 1);
      /* $5309 LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $530B AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $530D BEQ */ s_pc = s_status & STATUS_Z ? 0x5312 : 0x530f;
      branchTarget = true;
      break;
    case 0x530f: // [$530F..$5311]    3 bytes
      CYCLES(0x530f, 6);
      /* $530F JSR */ push16(0x5311), s_pc = 0x538d;
      branchTarget = true;
      break;
    case 0x5312: // [$5312..$531A]    9 bytes
      CYCLES(0x5312, 16);
      /* $5312 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $5314 INY */ s_y = update_nz(s_y + 1);
      /* $5315 LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $5317 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $5319 BEQ */ s_pc = s_status & STATUS_Z ? 0x531e : 0x531b;
      branchTarget = true;
      break;
    case 0x531b: // [$531B..$531D]    3 bytes
      CYCLES(0x531b, 6);
      /* $531B JSR */ push16(0x531d), s_pc = 0x538d;
      branchTarget = true;
      break;
    case 0x531e: // [$531E..$5324]    7 bytes
      CYCLES(0x531e, 12);
      /* $531E INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $5320 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $5322 DEY */ s_y = update_nz(s_y - 1);
      /* $5323 BPL */ s_pc = !(s_status & STATUS_N) ? 0x52f1 : 0x5325;
      branchTarget = true;
      break;
    case 0x5325: // [$5325..$5327]    3 bytes
      CYCLES(0x5325, 6);
      /* $5325 JSR */ push16(0x5327), s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x5328: // [$5328..$5353]   44 bytes
      CYCLES(0x5328, 75);
      /* $5328 LDA */ s_a = update_nz(peek(0x1508));
      /* $532B EOR */ s_a = update_nz(s_a ^ 0x01);
      /* $532D STA */ poke(0x1508, s_a);
      /* $5330 LDA */ s_a = update_nz(peek(0x1506));
      /* $5333 STA */ poke(0x1507, s_a);
      /* $5336 ASL */ s_a = update_nzc(s_a << 1);
      /* $5337 ORA */ s_a = update_nz(s_a | peek(0x1508));
      /* $533A TAX */ s_x = update_nz(s_a);
      /* $533B LDA */ s_a = update_nz(peek(0x0800 + s_x));
      /* $533E STA */ poke_zpg(0x06, s_a);
      /* $5340 LDA */ s_a = update_nz(peek(0x0820 + s_x));
      /* $5343 STA */ poke_zpg(0x07, s_a);
      /* $5345 LDY */ s_y = update_nz(peek(0x1503));
      /* $5348 STY */ poke(0x1501, s_y);
      /* $534B LDX */ s_x = update_nz(peek(0x1502));
      /* $534E STX */ poke(0x1500, s_x);
      /* $5351 JSR */ push16(0x5353), s_pc = 0x4c7c;
      branchTarget = true;
      break;
    case 0x5354: // [$5354..$5365]   18 bytes
      CYCLES(0x5354, 31);
      /* $5354 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $5356 STA */ poke_zpg(0x06, s_a);
      /* $5358 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $535A STA */ poke_zpg(0x07, s_a);
      /* $535C STY */ poke_zpg(0x05, s_y);
      /* $535E LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $5360 LDA */ s_a = update_nz(peek(peek16_zpg(0x06) + s_y));
      /* $5362 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x08 + s_x)));
      /* $5364 BEQ */ s_pc = s_status & STATUS_Z ? 0x5369 : 0x5366;
      branchTarget = true;
      break;
    case 0x5366: // [$5366..$5368]    3 bytes
      CYCLES(0x5366, 6);
      /* $5366 JSR */ push16(0x5368), s_pc = 0x538d;
      branchTarget = true;
      break;
    case 0x5369: // [$5369..$5377]   15 bytes
      CYCLES(0x5369, 26);
      /* $5369 LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $536B ORA */ s_a = update_nz(s_a | peek(peek16_zpg(0x06) + s_y));
      /* $536D STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $536F INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $5371 INY */ s_y = update_nz(s_y + 1);
      /* $5372 LDA */ s_a = update_nz(peek(peek16_zpg(0x06) + s_y));
      /* $5374 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x08 + s_x)));
      /* $5376 BEQ */ s_pc = s_status & STATUS_Z ? 0x537b : 0x5378;
      branchTarget = true;
      break;
    case 0x5378: // [$5378..$537A]    3 bytes
      CYCLES(0x5378, 6);
      /* $5378 JSR */ push16(0x537a), s_pc = 0x538d;
      branchTarget = true;
      break;
    case 0x537b: // [$537B..$5387]   13 bytes
      CYCLES(0x537b, 23);
      /* $537B LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $537D ORA */ s_a = update_nz(s_a | peek(peek16_zpg(0x06) + s_y));
      /* $537F STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $5381 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $5383 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $5385 DEY */ s_y = update_nz(s_y - 1);
      /* $5386 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5354 : 0x5388;
      branchTarget = true;
      break;
    case 0x5388: // [$5388..$538A]    3 bytes
      CYCLES(0x5388, 6);
      /* $5388 JSR */ push16(0x538a), s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x538b: // [$538B..$538C]    2 bytes
      CYCLES(0x538b, 4);
      /* $538B CLC */ s_status &= ~STATUS_C;
      /* $538C RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x538d: // [$538D..$539A]   14 bytes
      CYCLES(0x538d, 24);
      /* $538D PHA */ push8(s_a);
      /* $538E STX */ poke(0x0700, s_x);
      /* $5391 STY */ poke(0x0701, s_y);
      /* $5394 LDA */ s_a = update_nz(peek_zpg(0xfc));
      /* $5396 STA */ poke(0x0702, s_a);
      /* $5399 LDX */ s_x = update_nz(0x05);
    case 0x539b: // [$539B..$53A2]    8 bytes
      CYCLES(0x539b, 14);
      /* $539B LDA */ s_a = update_nz(peek_zpg((uint8_t)(0x04 + s_x)));
      /* $539D STA */ poke(0x0703 + s_x, s_a);
      /* $53A0 DEX */ s_x = update_nz(s_x - 1);
      /* $53A1 BPL */ s_pc = !(s_status & STATUS_N) ? 0x539b : 0x53a3;
      branchTarget = true;
      break;
    case 0x53a3: // [$53A3..$53A6]    4 bytes
      CYCLES(0x53a3, 7);
      /* $53A3 PLA */ s_a = update_nz(pop8());
      /* $53A4 JSR */ push16(0x53a6), s_pc = 0x598d;
      branchTarget = true;
      break;
    case 0x53a7: // [$53A7..$53AD]    7 bytes
      CYCLES(0x53a7, 12);
      /* $53A7 LDA */ s_a = update_nz(0x01);
      /* $53A9 STA */ poke_zpg(0xe8, s_a);
      /* $53AB JSR */ push16(0x53ad), s_pc = 0x5a18;
      branchTarget = true;
      break;
    case 0x53ae: // [$53AE..$53AF]    2 bytes
      CYCLES(0x53ae, 4);
      /* $53AE BCS */ s_pc = s_status & STATUS_C ? 0x53d4 : 0x53b0;
      branchTarget = true;
      break;
    case 0x53b0: // [$53B0..$53B2]    3 bytes
      CYCLES(0x53b0, 6);
      /* $53B0 JSR */ push16(0x53b2), s_pc = 0x59b5;
      branchTarget = true;
      break;
    case 0x53b3: // [$53B3..$53B4]    2 bytes
      CYCLES(0x53b3, 4);
      /* $53B3 BCC */ s_pc = !(s_status & STATUS_C) ? 0x53d4 : 0x53b5;
      branchTarget = true;
      break;
    case 0x53b5: // [$53B5..$53CF]   27 bytes
      CYCLES(0x53b5, 46);
      /* $53B5 LDA */ s_a = update_nz(peek(0x1507));
      /* $53B8 ASL */ s_a = update_nzc(s_a << 1);
      /* $53B9 ORA */ s_a = update_nz(s_a | peek(0x1508));
      /* $53BC TAX */ s_x = update_nz(s_a);
      /* $53BD LDA */ s_a = update_nz(peek(0x0800 + s_x));
      /* $53C0 STA */ poke_zpg(0x06, s_a);
      /* $53C2 LDA */ s_a = update_nz(peek(0x0820 + s_x));
      /* $53C5 STA */ poke_zpg(0x07, s_a);
      /* $53C7 LDX */ s_x = update_nz(peek(0x1502));
      /* $53CA LDY */ s_y = update_nz(peek(0x1503));
      /* $53CD JSR */ push16(0x53cf), s_pc = 0x5059;
      branchTarget = true;
      break;
    case 0x53d0: // [$53D0..$53D3]    4 bytes
      CYCLES(0x53d0, 7);
      /* $53D0 PLA */ s_a = update_nz(pop8());
      /* $53D1 PLA */ s_a = update_nz(pop8());
      /* $53D2 SEC */ s_status |= STATUS_C;
      /* $53D3 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x53d4: // [$53D4..$53D5]    2 bytes
      CYCLES(0x53d4, 4);
      /* $53D4 LDX */ s_x = update_nz(0x05);
    case 0x53d6: // [$53D6..$53DD]    8 bytes
      CYCLES(0x53d6, 14);
      /* $53D6 LDA */ s_a = update_nz(peek(0x0703 + s_x));
      /* $53D9 STA */ poke_zpg((uint8_t)(0x04 + s_x), s_a);
      /* $53DB DEX */ s_x = update_nz(s_x - 1);
      /* $53DC BPL */ s_pc = !(s_status & STATUS_N) ? 0x53d6 : 0x53de;
      branchTarget = true;
      break;
    case 0x53de: // [$53DE..$53EB]   14 bytes
      CYCLES(0x53de, 24);
      /* $53DE LDA */ s_a = update_nz(peek(0x0702));
      /* $53E1 STA */ poke_zpg(0xfc, s_a);
      /* $53E3 STA */ poke_zpg(0xfe, s_a);
      /* $53E5 LDY */ s_y = update_nz(peek(0x0701));
      /* $53E8 LDX */ s_x = update_nz(peek(0x0700));
      /* $53EB RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x53ec: // [$53EC..$53F0]    5 bytes
      CYCLES(0x53ec, 9);
      /* $53EC LDA */ s_a = update_nz(peek(0x1421));
      /* $53EF BEQ */ s_pc = s_status & STATUS_Z ? 0x53f2 : 0x53f1;
      branchTarget = true;
      break;
    case 0x53f1: // [$53F1..$53F1]    1 bytes
      CYCLES(0x53f1, 2);
      /* $53F1 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x53f2: // [$53F2..$53F6]    5 bytes
      CYCLES(0x53f2, 9);
      /* $53F2 DEC */ tmp16 = 0x1401, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $53F5 BPL */ s_pc = !(s_status & STATUS_N) ? 0x53f1 : 0x53f7;
      branchTarget = true;
      break;
    case 0x53f7: // [$53F7..$53FE]    8 bytes
      CYCLES(0x53f7, 14);
      /* $53F7 INC */ tmp16 = 0x1401, poke(tmp16, update_nz(peek(tmp16) + 1));
      /* $53FA LDA */ s_a = update_nz(peek(0x1416));
      /* $53FD BEQ */ s_pc = s_status & STATUS_Z ? 0x53f1 : 0x53ff;
      branchTarget = true;
      break;
    case 0x53ff: // [$53FF..$5406]    8 bytes
      CYCLES(0x53ff, 14);
      /* $53FF LDY */ s_y = update_nz(peek(0x1400));
      /* $5402 CPY */ update_nz_inv_c(s_y - peek(0x0c00));
      /* $5405 BCS */ s_pc = s_status & STATUS_C ? 0x53f1 : 0x5407;
      branchTarget = true;
      break;
    case 0x5407: // [$5407..$541B]   21 bytes
      CYCLES(0x5407, 36);
      /* $5407 STA */ poke(0x1530 + s_y, s_a);
      /* $540A TAX */ s_x = update_nz(s_a);
      /* $540B LDA */ s_a = update_nz(0x01);
      /* $540D STA */ poke(0x1540 + s_y, s_a);
      /* $5410 LDA */ s_a = update_nz(0x02);
      /* $5412 STA */ poke(0x1401, s_a);
      /* $5415 INC */ tmp16 = 0x1400, poke(tmp16, update_nz(peek(tmp16) + 1));
      /* $5418 CPX */ update_nz_inv_c(s_x - 0x01);
      /* $541A BNE */ s_pc = !(s_status & STATUS_Z) ? 0x543c : 0x541c;
      branchTarget = true;
      break;
    case 0x541c: // [$541C..$5424]    9 bytes
      CYCLES(0x541c, 16);
      /* $541C LDA */ s_a = update_nz(peek(0x1500));
      /* $541F CLC */ s_status &= ~STATUS_C;
      /* $5420 ADC */ tmp = 0x0b, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5422 JSR */ push16(0x5424), s_pc = 0x4c1c;
      branchTarget = true;
      break;
    case 0x5425: // [$5425..$5429]    5 bytes
      CYCLES(0x5425, 9);
      /* $5425 STA */ poke(0x1520 + s_y, s_a);
      /* $5428 LDA */ s_a = update_nz(0xff);
    case 0x542a: // [$542A..$542D]    4 bytes
      CYCLES(0x542a, 7);
      /* $542A ASL */ s_a = update_nzc(s_a << 1);
      /* $542B DEX */ s_x = update_nz(s_x - 1);
      /* $542C BPL */ s_pc = !(s_status & STATUS_N) ? 0x542a : 0x542e;
      branchTarget = true;
      break;
    case 0x542e: // [$542E..$5431]    4 bytes
      CYCLES(0x542e, 7);
      /* $542E LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $542F STA */ poke(0x1550 + s_y, s_a);
    case 0x5432: // [$5432..$543B]   10 bytes
      CYCLES(0x5432, 18);
      /* $5432 LDA */ s_a = update_nz(peek(0x1501));
      /* $5435 CLC */ s_status &= ~STATUS_C;
      /* $5436 ADC */ tmp = 0x05, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5438 STA */ poke(0x1510 + s_y, s_a);
      /* $543B RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x543c: // [$543C..$543F]    4 bytes
      CYCLES(0x543c, 7);
      /* $543C CPX */ update_nz_inv_c(s_x - 0x03);
      /* $543E BNE */ s_pc = !(s_status & STATUS_Z) ? 0x546a : 0x5440;
      branchTarget = true;
      break;
    case 0x5440: // [$5440..$5448]    9 bytes
      CYCLES(0x5440, 16);
      /* $5440 LDA */ s_a = update_nz(peek(0x1500));
      /* $5443 SEC */ s_status |= STATUS_C;
      /* $5444 SBC */ tmp = 0x04, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $5446 JSR */ push16(0x5448), s_pc = 0x4c1c;
      branchTarget = true;
      break;
    case 0x5449: // [$5449..$544E]    6 bytes
      CYCLES(0x5449, 11);
      /* $5449 STA */ poke(0x1520 + s_y, s_a);
      /* $544C TXA */ s_a = update_nz(s_x);
      /* $544D BEQ */ s_pc = s_status & STATUS_Z ? 0x545b : 0x544f;
      branchTarget = true;
      break;
    case 0x544f: // [$544F..$5450]    2 bytes
      CYCLES(0x544f, 4);
      /* $544F LDA */ s_a = update_nz(0x00);
    case 0x5451: // [$5451..$5455]    5 bytes
      CYCLES(0x5451, 9);
      /* $5451 SEC */ s_status |= STATUS_C;
      /* $5452 ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $5453 DEX */ s_x = update_nz(s_x - 1);
      /* $5454 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5451 : 0x5456;
      branchTarget = true;
      break;
    case 0x5456: // [$5456..$545A]    5 bytes
      CYCLES(0x5456, 9);
      /* $5456 STA */ poke(0x1550 + s_y, s_a);
      /* $5459 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5432 : 0x545b;
      branchTarget = true;
      break;
    case 0x545b: // [$545B..$5469]   15 bytes
      CYCLES(0x545b, 26);
      /* $545B LDA */ s_a = update_nz(0x7f);
      /* $545D STA */ poke(0x1550 + s_y, s_a);
      /* $5460 LDX */ s_x = update_nz(peek(0x1520 + s_y));
      /* $5463 DEX */ s_x = update_nz(s_x - 1);
      /* $5464 TXA */ s_a = update_nz(s_x);
      /* $5465 STA */ poke(0x1520 + s_y, s_a);
      /* $5468 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5432 : 0x546a;
      branchTarget = true;
      break;
    case 0x546a: // [$546A..$546D]    4 bytes
      CYCLES(0x546a, 7);
      /* $546A CPX */ update_nz_inv_c(s_x - 0x04);
      /* $546C BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5486 : 0x546e;
      branchTarget = true;
      break;
    case 0x546e: // [$546E..$5479]   12 bytes
      CYCLES(0x546e, 21);
      /* $546E LDA */ s_a = update_nz(peek(0x1501));
      /* $5471 CLC */ s_status &= ~STATUS_C;
      /* $5472 ADC */ tmp = 0x0e, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5474 STA */ poke(0x1510 + s_y, s_a);
      /* $5477 JSR */ push16(0x5479), s_pc = 0x4c1c;
      branchTarget = true;
      break;
    case 0x547a: // [$547A..$5485]   12 bytes
      CYCLES(0x547a, 21);
      /* $547A STX */ poke_zpg(0x00, s_x);
      /* $547C LDA */ s_a = update_nz(0x06);
      /* $547E SEC */ s_status |= STATUS_C;
      /* $547F SBC */ tmp = peek_zpg(0x00), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $5481 STA */ poke(0x1560 + s_y, s_a);
      /* $5484 BPL */ s_pc = !(s_status & STATUS_N) ? 0x54a2 : 0x5486;
      branchTarget = true;
      break;
    case 0x5486: // [$5486..$5489]    4 bytes
      CYCLES(0x5486, 7);
      /* $5486 CPX */ update_nz_inv_c(s_x - 0x0c);
      /* $5488 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x54b9 : 0x548a;
      branchTarget = true;
      break;
    case 0x548a: // [$548A..$5492]    9 bytes
      CYCLES(0x548a, 16);
      /* $548A LDA */ s_a = update_nz(peek(0x1501));
      /* $548D SEC */ s_status |= STATUS_C;
      /* $548E SBC */ tmp = 0x04, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $5490 JSR */ push16(0x5492), s_pc = 0x4c1c;
      branchTarget = true;
      break;
    case 0x5493: // [$5493..$54A1]   15 bytes
      CYCLES(0x5493, 26);
      /* $5493 STA */ poke_zpg(0x00, s_a);
      /* $5495 ASL */ s_a = update_nzc(s_a << 1);
      /* $5496 ADC */ tmp = peek_zpg(0x00), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5498 ASL */ s_a = update_nzc(s_a << 1);
      /* $5499 ADC */ tmp = peek_zpg(0x00), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $549B STA */ poke(0x1510 + s_y, s_a);
      /* $549E TXA */ s_a = update_nz(s_x);
      /* $549F STA */ poke(0x1560 + s_y, s_a);
    case 0x54a2: // [$54A2..$54AA]    9 bytes
      CYCLES(0x54a2, 16);
      /* $54A2 LDA */ s_a = update_nz(peek(0x1500));
      /* $54A5 CLC */ s_status &= ~STATUS_C;
      /* $54A6 ADC */ tmp = 0x03, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $54A8 JSR */ push16(0x54aa), s_pc = 0x4c1c;
      branchTarget = true;
      break;
    case 0x54ab: // [$54AB..$54B0]    6 bytes
      CYCLES(0x54ab, 11);
      /* $54AB STA */ poke(0x1520 + s_y, s_a);
      /* $54AE LDA */ s_a = update_nz(0x00);
      /* $54B0 SEC */ s_status |= STATUS_C;
    case 0x54b1: // [$54B1..$54B4]    4 bytes
      CYCLES(0x54b1, 7);
      /* $54B1 ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $54B2 DEX */ s_x = update_nz(s_x - 1);
      /* $54B3 BPL */ s_pc = !(s_status & STATUS_N) ? 0x54b1 : 0x54b5;
      branchTarget = true;
      break;
    case 0x54b5: // [$54B5..$54B8]    4 bytes
      CYCLES(0x54b5, 7);
      /* $54B5 STA */ poke(0x1550 + s_y, s_a);
      /* $54B8 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x54b9: // [$54B9..$54BC]    4 bytes
      CYCLES(0x54b9, 7);
      /* $54B9 CPX */ update_nz_inv_c(s_x - 0x05);
      /* $54BB BNE */ s_pc = !(s_status & STATUS_Z) ? 0x54c3 : 0x54bd;
      branchTarget = true;
      break;
    case 0x54bd: // [$54BD..$54BF]    3 bytes
      CYCLES(0x54bd, 6);
      /* $54BD JSR */ push16(0x54bf), s_pc = 0x54dd;
      branchTarget = true;
      break;
    case 0x54c0: // [$54C0..$54C2]    3 bytes
      CYCLES(0x54c0, 6);
      /* $54C0 JMP */ s_pc = 0x54ef;
      branchTarget = true;
      break;
    case 0x54c3: // [$54C3..$54C6]    4 bytes
      CYCLES(0x54c3, 7);
      /* $54C3 CPX */ update_nz_inv_c(s_x - 0x07);
      /* $54C5 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x54cd : 0x54c7;
      branchTarget = true;
      break;
    case 0x54c7: // [$54C7..$54C9]    3 bytes
      CYCLES(0x54c7, 6);
      /* $54C7 JSR */ push16(0x54c9), s_pc = 0x54dd;
      branchTarget = true;
      break;
    case 0x54ca: // [$54CA..$54CC]    3 bytes
      CYCLES(0x54ca, 6);
      /* $54CA JMP */ s_pc = 0x5506;
      branchTarget = true;
      break;
    case 0x54cd: // [$54CD..$54D0]    4 bytes
      CYCLES(0x54cd, 7);
      /* $54CD CPX */ update_nz_inv_c(s_x - 0x0d);
      /* $54CF BNE */ s_pc = !(s_status & STATUS_Z) ? 0x54d7 : 0x54d1;
      branchTarget = true;
      break;
    case 0x54d1: // [$54D1..$54D3]    3 bytes
      CYCLES(0x54d1, 6);
      /* $54D1 JSR */ push16(0x54d3), s_pc = 0x54e5;
      branchTarget = true;
      break;
    case 0x54d4: // [$54D4..$54D6]    3 bytes
      CYCLES(0x54d4, 6);
      /* $54D4 JMP */ s_pc = 0x54ef;
      branchTarget = true;
      break;
    case 0x54d7: // [$54D7..$54D9]    3 bytes
      CYCLES(0x54d7, 6);
      /* $54D7 JSR */ push16(0x54d9), s_pc = 0x54e5;
      branchTarget = true;
      break;
    case 0x54da: // [$54DA..$54DC]    3 bytes
      CYCLES(0x54da, 6);
      /* $54DA JMP */ s_pc = 0x5506;
      branchTarget = true;
      break;
    case 0x54dd: // [$54DD..$54E4]    8 bytes
      CYCLES(0x54dd, 14);
      /* $54DD LDA */ s_a = update_nz(peek(0x1501));
      /* $54E0 CLC */ s_status &= ~STATUS_C;
      /* $54E1 ADC */ tmp = 0x0c, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $54E3 BCC */ s_pc = !(s_status & STATUS_C) ? 0x54eb : 0x54e5;
      branchTarget = true;
      break;
    case 0x54e5: // [$54E5..$54EA]    6 bytes
      CYCLES(0x54e5, 11);
      /* $54E5 LDA */ s_a = update_nz(peek(0x1501));
      /* $54E8 SEC */ s_status |= STATUS_C;
      /* $54E9 SBC */ tmp = 0x02, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
    case 0x54eb: // [$54EB..$54EE]    4 bytes
      CYCLES(0x54eb, 7);
      /* $54EB STA */ poke(0x1510 + s_y, s_a);
      /* $54EE RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x54ef: // [$54EF..$54F4]    6 bytes
      CYCLES(0x54ef, 11);
      /* $54EF LDA */ s_a = update_nz(peek(0x1500));
      /* $54F2 CLC */ s_status &= ~STATUS_C;
      /* $54F3 ADC */ tmp = 0x09, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0x54f5: // [$54F5..$54F7]    3 bytes
      CYCLES(0x54f5, 6);
      /* $54F5 JSR */ push16(0x54f7), s_pc = 0x4c1c;
      branchTarget = true;
      break;
    case 0x54f8: // [$54F8..$54FD]    6 bytes
      CYCLES(0x54f8, 11);
      /* $54F8 STA */ poke(0x1520 + s_y, s_a);
      /* $54FB LDA */ s_a = update_nz(0x00);
      /* $54FD SEC */ s_status |= STATUS_C;
    case 0x54fe: // [$54FE..$5501]    4 bytes
      CYCLES(0x54fe, 7);
      /* $54FE ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $54FF DEX */ s_x = update_nz(s_x - 1);
      /* $5500 BPL */ s_pc = !(s_status & STATUS_N) ? 0x54fe : 0x5502;
      branchTarget = true;
      break;
    case 0x5502: // [$5502..$5505]    4 bytes
      CYCLES(0x5502, 7);
      /* $5502 STA */ poke(0x1550 + s_y, s_a);
      /* $5505 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5506: // [$5506..$550E]    9 bytes
      CYCLES(0x5506, 16);
      /* $5506 LDA */ s_a = update_nz(peek(0x1500));
      /* $5509 SEC */ s_status |= STATUS_C;
      /* $550A SBC */ tmp = 0x02, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $550C JSR */ push16(0x550e), s_pc = 0x54f5;
      branchTarget = true;
      break;
    case 0x550f: // [$550F..$5511]    3 bytes
      CYCLES(0x550f, 6);
      /* $550F LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $5510 BCC */ s_pc = !(s_status & STATUS_C) ? 0x551c : 0x5512;
      branchTarget = true;
      break;
    case 0x5512: // [$5512..$551B]   10 bytes
      CYCLES(0x5512, 18);
      /* $5512 TYA */ s_a = update_nz(s_y);
      /* $5513 TAX */ s_x = update_nz(s_a);
      /* $5514 DEC */ tmp16 = 0x1520 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $5517 LDA */ s_a = update_nz(0x40);
      /* $5519 STA */ poke(0x1550 + s_y, s_a);
    case 0x551c: // [$551C..$551C]    1 bytes
      CYCLES(0x551c, 2);
      /* $551C RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x551d: // [$551D..$5524]    8 bytes
      CYCLES(0x551d, 14);
      /* $551D LDX */ s_x = update_nz(peek(0x1400));
      /* $5520 LDA */ s_a = update_nz(peek(0x1421));
      /* $5523 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5528 : 0x5525;
      branchTarget = true;
      break;
    case 0x5525: // [$5525..$5527]    3 bytes
      CYCLES(0x5525, 6);
      /* $5525 DEX */ s_x = update_nz(s_x - 1);
      /* $5526 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5529 : 0x5528;
      branchTarget = true;
      break;
    case 0x5528: // [$5528..$5528]    1 bytes
      CYCLES(0x5528, 2);
      /* $5528 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5529: // [$5529..$5531]    9 bytes
      CYCLES(0x5529, 16);
      /* $5529 STX */ poke_zpg(0x00, s_x);
      /* $552B LDA */ s_a = update_nz(peek(0x1530 + s_x));
      /* $552E CMP */ update_nz_inv_c(s_a - 0x04);
      /* $5530 BCS */ s_pc = s_status & STATUS_C ? 0x558c : 0x5532;
      branchTarget = true;
      break;
    case 0x5532: // [$5532..$553C]   11 bytes
      CYCLES(0x5532, 19);
      /* $5532 EOR */ s_a = update_nz(s_a ^ 0x03);
      /* $5534 STA */ poke_zpg(0x04, s_a);
      /* $5536 DEC */ tmp16 = 0x04, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $5538 LDA */ s_a = update_nz(peek(0x1540 + s_x));
      /* $553B BEQ */ s_pc = s_status & STATUS_Z ? 0x5542 : 0x553d;
      branchTarget = true;
      break;
    case 0x553d: // [$553D..$5541]    5 bytes
      CYCLES(0x553d, 9);
      /* $553D DEC */ tmp16 = 0x1540 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $5540 BPL */ s_pc = !(s_status & STATUS_N) ? 0x555a : 0x5542;
      branchTarget = true;
      break;
    case 0x5542: // [$5542..$5544]    3 bytes
      CYCLES(0x5542, 6);
      /* $5542 JSR */ push16(0x5544), s_pc = 0x5837;
      branchTarget = true;
      break;
    case 0x5545: // [$5545..$5556]   18 bytes
      CYCLES(0x5545, 31);
      /* $5545 LDA */ s_a = update_nz(0x7f);
      /* $5547 STA */ poke(0x1550 + s_x, s_a);
      /* $554A LDA */ s_a = update_nz(peek(0x1520 + s_x));
      /* $554D CLC */ s_status &= ~STATUS_C;
      /* $554E ADC */ tmp = peek_zpg(0x04), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5550 STA */ poke(0x1520 + s_x, s_a);
      /* $5553 CMP */ update_nz_inv_c(s_a - 0x25);
      /* $5555 BCC */ s_pc = !(s_status & STATUS_C) ? 0x555a : 0x5557;
      branchTarget = true;
      break;
    case 0x5557: // [$5557..$5559]    3 bytes
      CYCLES(0x5557, 6);
      /* $5557 JMP */ s_pc = 0x5959;
      branchTarget = true;
      break;
    case 0x555a: // [$555A..$5576]   29 bytes
      CYCLES(0x555a, 50);
      /* $555A LDA */ s_a = update_nz(peek(0x1510 + s_x));
      /* $555D STA */ poke_zpg(0xfc, s_a);
      /* $555F STA */ poke_zpg(0xfe, s_a);
      /* $5561 LDY */ s_y = update_nz(0x00);
      /* $5563 STY */ poke_zpg(0x05, s_y);
      /* $5565 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $5567 STA */ poke_zpg(0x06, s_a);
      /* $5569 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $556B STA */ poke_zpg(0x07, s_a);
      /* $556D LDY */ s_y = update_nz(peek(0x1520 + s_x));
      /* $5570 LDA */ s_a = update_nz(peek(0x1550 + s_x));
      /* $5573 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $5575 BEQ */ s_pc = s_status & STATUS_Z ? 0x5582 : 0x5577;
      branchTarget = true;
      break;
    case 0x5577: // [$5577..$5579]    3 bytes
      CYCLES(0x5577, 6);
      /* $5577 JSR */ push16(0x5579), s_pc = 0x59aa;
      branchTarget = true;
      break;
    case 0x557a: // [$557A..$557B]    2 bytes
      CYCLES(0x557a, 4);
      /* $557A BCC */ s_pc = !(s_status & STATUS_C) ? 0x5582 : 0x557c;
      branchTarget = true;
      break;
    case 0x557c: // [$557C..$557E]    3 bytes
      CYCLES(0x557c, 6);
      /* $557C JSR */ push16(0x557e), s_pc = 0x5837;
      branchTarget = true;
      break;
    case 0x557f: // [$557F..$5581]    3 bytes
      CYCLES(0x557f, 6);
      /* $557F JMP */ s_pc = 0x5959;
      branchTarget = true;
      break;
    case 0x5582: // [$5582..$558B]   10 bytes
      CYCLES(0x5582, 18);
      /* $5582 LDA */ s_a = update_nz(peek(0x1550 + s_x));
      /* $5585 ORA */ s_a = update_nz(s_a | peek(peek16_zpg(0x06) + s_y));
      /* $5587 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $5589 JMP */ s_pc = 0x5525;
      branchTarget = true;
      break;
    case 0x558c: // [$558C..$558F]    4 bytes
      CYCLES(0x558c, 7);
      /* $558C CMP */ update_nz_inv_c(s_a - 0x04);
      /* $558E BNE */ s_pc = !(s_status & STATUS_Z) ? 0x55fb : 0x5590;
      branchTarget = true;
      break;
    case 0x5590: // [$5590..$5594]    5 bytes
      CYCLES(0x5590, 9);
      /* $5590 LDA */ s_a = update_nz(peek(0x1540 + s_x));
      /* $5593 BEQ */ s_pc = s_status & STATUS_Z ? 0x559a : 0x5595;
      branchTarget = true;
      break;
    case 0x5595: // [$5595..$5599]    5 bytes
      CYCLES(0x5595, 9);
      /* $5595 DEC */ tmp16 = 0x1540 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $5598 BPL */ s_pc = !(s_status & STATUS_N) ? 0x55b5 : 0x559a;
      branchTarget = true;
      break;
    case 0x559a: // [$559A..$559C]    3 bytes
      CYCLES(0x559a, 6);
      /* $559A JSR */ push16(0x559c), s_pc = 0x5855;
      branchTarget = true;
      break;
    case 0x559d: // [$559D..$55AC]   16 bytes
      CYCLES(0x559d, 28);
      /* $559D LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $559F LDA */ s_a = update_nz(peek(0x1510 + s_x));
      /* $55A2 SEC */ s_status |= STATUS_C;
      /* $55A3 ADC */ tmp = peek(0x1560 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $55A6 STA */ poke(0x1510 + s_x, s_a);
      /* $55A9 CMP */ update_nz_inv_c(s_a - 0xbd);
      /* $55AB BCC */ s_pc = !(s_status & STATUS_C) ? 0x55b0 : 0x55ad;
      branchTarget = true;
      break;
    case 0x55ad: // [$55AD..$55AF]    3 bytes
      CYCLES(0x55ad, 6);
      /* $55AD JMP */ s_pc = 0x5959;
      branchTarget = true;
      break;
    case 0x55b0: // [$55B0..$55B4]    5 bytes
      CYCLES(0x55b0, 9);
      /* $55B0 LDA */ s_a = update_nz(0x06);
      /* $55B2 STA */ poke(0x1560 + s_x, s_a);
    case 0x55b5: // [$55B5..$55C7]   19 bytes
      CYCLES(0x55b5, 33);
      /* $55B5 LDA */ s_a = update_nz(peek(0x1510 + s_x));
      /* $55B8 STA */ poke_zpg(0xfc, s_a);
      /* $55BA STA */ poke_zpg(0xfe, s_a);
      /* $55BC LDY */ s_y = update_nz(peek(0x1560 + s_x));
      /* $55BF LDA */ s_a = update_nz(peek(0x1520 + s_x));
      /* $55C2 STA */ poke_zpg(0x04, s_a);
      /* $55C4 LDA */ s_a = update_nz(peek(0x1550 + s_x));
      /* $55C7 TAX */ s_x = update_nz(s_a);
    case 0x55c8: // [$55C8..$55D8]   17 bytes
      CYCLES(0x55c8, 29);
      /* $55C8 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $55CA STA */ poke_zpg(0x06, s_a);
      /* $55CC LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $55CE STA */ poke_zpg(0x07, s_a);
      /* $55D0 STY */ poke_zpg(0x05, s_y);
      /* $55D2 LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $55D4 TXA */ s_a = update_nz(s_x);
      /* $55D5 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $55D7 BEQ */ s_pc = s_status & STATUS_Z ? 0x55ec : 0x55d9;
      branchTarget = true;
      break;
    case 0x55d9: // [$55D9..$55DF]    7 bytes
      CYCLES(0x55d9, 12);
      /* $55D9 STX */ poke_zpg(0x01, s_x);
      /* $55DB LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $55DD JSR */ push16(0x55df), s_pc = 0x59aa;
      branchTarget = true;
      break;
    case 0x55e0: // [$55E0..$55E1]    2 bytes
      CYCLES(0x55e0, 4);
      /* $55E0 BCC */ s_pc = !(s_status & STATUS_C) ? 0x55ea : 0x55e2;
      branchTarget = true;
      break;
    case 0x55e2: // [$55E2..$55E4]    3 bytes
      CYCLES(0x55e2, 6);
      /* $55E2 JSR */ push16(0x55e4), s_pc = 0x5855;
      branchTarget = true;
      break;
    case 0x55e5: // [$55E5..$55E9]    5 bytes
      CYCLES(0x55e5, 9);
      /* $55E5 LDX */ s_x = update_nz(peek_zpg(0x1f));
      /* $55E7 JMP */ s_pc = 0x5959;
      branchTarget = true;
      break;
    case 0x55ea: // [$55EA..$55EB]    2 bytes
      CYCLES(0x55ea, 4);
      /* $55EA LDX */ s_x = update_nz(peek_zpg(0x01));
    case 0x55ec: // [$55EC..$55F5]   10 bytes
      CYCLES(0x55ec, 18);
      /* $55EC TXA */ s_a = update_nz(s_x);
      /* $55ED ORA */ s_a = update_nz(s_a | peek(peek16_zpg(0x06) + s_y));
      /* $55EF STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $55F1 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $55F3 DEY */ s_y = update_nz(s_y - 1);
      /* $55F4 BPL */ s_pc = !(s_status & STATUS_N) ? 0x55c8 : 0x55f6;
      branchTarget = true;
      break;
    case 0x55f6: // [$55F6..$55FA]    5 bytes
      CYCLES(0x55f6, 9);
      /* $55F6 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $55F8 JMP */ s_pc = 0x5525;
      branchTarget = true;
      break;
    case 0x55fb: // [$55FB..$55FE]    4 bytes
      CYCLES(0x55fb, 7);
      /* $55FB CMP */ update_nz_inv_c(s_a - 0x0c);
      /* $55FD BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5667 : 0x55ff;
      branchTarget = true;
      break;
    case 0x55ff: // [$55FF..$5603]    5 bytes
      CYCLES(0x55ff, 9);
      /* $55FF LDA */ s_a = update_nz(peek(0x1540 + s_x));
      /* $5602 BEQ */ s_pc = s_status & STATUS_Z ? 0x5609 : 0x5604;
      branchTarget = true;
      break;
    case 0x5604: // [$5604..$5608]    5 bytes
      CYCLES(0x5604, 9);
      /* $5604 DEC */ tmp16 = 0x1540 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $5607 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5621 : 0x5609;
      branchTarget = true;
      break;
    case 0x5609: // [$5609..$560B]    3 bytes
      CYCLES(0x5609, 6);
      /* $5609 JSR */ push16(0x560b), s_pc = 0x5855;
      branchTarget = true;
      break;
    case 0x560c: // [$560C..$5618]   13 bytes
      CYCLES(0x560c, 23);
      /* $560C LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $560E LDA */ s_a = update_nz(peek(0x1510 + s_x));
      /* $5611 SEC */ s_status |= STATUS_C;
      /* $5612 SBC */ tmp = 0x07, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $5614 STA */ poke(0x1510 + s_x, s_a);
      /* $5617 BCS */ s_pc = s_status & STATUS_C ? 0x561c : 0x5619;
      branchTarget = true;
      break;
    case 0x5619: // [$5619..$561B]    3 bytes
      CYCLES(0x5619, 6);
      /* $5619 JMP */ s_pc = 0x5959;
      branchTarget = true;
      break;
    case 0x561c: // [$561C..$5620]    5 bytes
      CYCLES(0x561c, 9);
      /* $561C LDA */ s_a = update_nz(0x06);
      /* $561E STA */ poke(0x1560 + s_x, s_a);
    case 0x5621: // [$5621..$5633]   19 bytes
      CYCLES(0x5621, 33);
      /* $5621 LDA */ s_a = update_nz(peek(0x1510 + s_x));
      /* $5624 STA */ poke_zpg(0xfc, s_a);
      /* $5626 STA */ poke_zpg(0xfe, s_a);
      /* $5628 LDY */ s_y = update_nz(peek(0x1560 + s_x));
      /* $562B LDA */ s_a = update_nz(peek(0x1520 + s_x));
      /* $562E STA */ poke_zpg(0x04, s_a);
      /* $5630 LDA */ s_a = update_nz(peek(0x1550 + s_x));
      /* $5633 TAX */ s_x = update_nz(s_a);
    case 0x5634: // [$5634..$5644]   17 bytes
      CYCLES(0x5634, 29);
      /* $5634 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $5636 STA */ poke_zpg(0x06, s_a);
      /* $5638 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $563A STA */ poke_zpg(0x07, s_a);
      /* $563C STY */ poke_zpg(0x05, s_y);
      /* $563E LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $5640 TXA */ s_a = update_nz(s_x);
      /* $5641 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $5643 BEQ */ s_pc = s_status & STATUS_Z ? 0x5658 : 0x5645;
      branchTarget = true;
      break;
    case 0x5645: // [$5645..$564B]    7 bytes
      CYCLES(0x5645, 12);
      /* $5645 STX */ poke_zpg(0x01, s_x);
      /* $5647 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $5649 JSR */ push16(0x564b), s_pc = 0x59aa;
      branchTarget = true;
      break;
    case 0x564c: // [$564C..$564D]    2 bytes
      CYCLES(0x564c, 4);
      /* $564C BCC */ s_pc = !(s_status & STATUS_C) ? 0x5656 : 0x564e;
      branchTarget = true;
      break;
    case 0x564e: // [$564E..$5650]    3 bytes
      CYCLES(0x564e, 6);
      /* $564E JSR */ push16(0x5650), s_pc = 0x5855;
      branchTarget = true;
      break;
    case 0x5651: // [$5651..$5655]    5 bytes
      CYCLES(0x5651, 9);
      /* $5651 LDX */ s_x = update_nz(peek_zpg(0x1f));
      /* $5653 JMP */ s_pc = 0x5959;
      branchTarget = true;
      break;
    case 0x5656: // [$5656..$5657]    2 bytes
      CYCLES(0x5656, 4);
      /* $5656 LDX */ s_x = update_nz(peek_zpg(0x01));
    case 0x5658: // [$5658..$5661]   10 bytes
      CYCLES(0x5658, 18);
      /* $5658 TXA */ s_a = update_nz(s_x);
      /* $5659 ORA */ s_a = update_nz(s_a | peek(peek16_zpg(0x06) + s_y));
      /* $565B STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $565D LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $565F DEY */ s_y = update_nz(s_y - 1);
      /* $5660 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5634 : 0x5662;
      branchTarget = true;
      break;
    case 0x5662: // [$5662..$5666]    5 bytes
      CYCLES(0x5662, 9);
      /* $5662 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $5664 JMP */ s_pc = 0x5525;
      branchTarget = true;
      break;
    case 0x5667: // [$5667..$566A]    4 bytes
      CYCLES(0x5667, 7);
      /* $5667 CMP */ update_nz_inv_c(s_a - 0x05);
      /* $5669 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x56e2 : 0x566b;
      branchTarget = true;
      break;
    case 0x566b: // [$566B..$566F]    5 bytes
      CYCLES(0x566b, 9);
      /* $566B LDA */ s_a = update_nz(peek(0x1540 + s_x));
      /* $566E BEQ */ s_pc = s_status & STATUS_Z ? 0x5675 : 0x5670;
      branchTarget = true;
      break;
    case 0x5670: // [$5670..$5674]    5 bytes
      CYCLES(0x5670, 9);
      /* $5670 DEC */ tmp16 = 0x1540 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $5673 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5698 : 0x5675;
      branchTarget = true;
      break;
    case 0x5675: // [$5675..$5677]    3 bytes
      CYCLES(0x5675, 6);
      /* $5675 JSR */ push16(0x5677), s_pc = 0x5883;
      branchTarget = true;
      break;
    case 0x5678: // [$5678..$5688]   17 bytes
      CYCLES(0x5678, 29);
      /* $5678 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $567A LDA */ s_a = update_nz(0x01);
      /* $567C STA */ poke(0x1550 + s_x, s_a);
      /* $567F INC */ tmp16 = 0x1520 + s_x, poke(tmp16, update_nz(peek(tmp16) + 1));
      /* $5682 LDA */ s_a = update_nz(peek(0x1520 + s_x));
      /* $5685 CMP */ update_nz_inv_c(s_a - 0x25);
      /* $5687 BCS */ s_pc = s_status & STATUS_C ? 0x5695 : 0x5689;
      branchTarget = true;
      break;
    case 0x5689: // [$5689..$5694]   12 bytes
      CYCLES(0x5689, 21);
      /* $5689 TYA */ s_a = update_nz(s_y);
      /* $568A CLC */ s_status &= ~STATUS_C;
      /* $568B ADC */ tmp = peek(0x1510 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $568E STA */ poke(0x1510 + s_x, s_a);
      /* $5691 CMP */ update_nz_inv_c(s_a - 0xbd);
      /* $5693 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5698 : 0x5695;
      branchTarget = true;
      break;
    case 0x5695: // [$5695..$5697]    3 bytes
      CYCLES(0x5695, 6);
      /* $5695 JMP */ s_pc = 0x5959;
      branchTarget = true;
      break;
    case 0x5698: // [$5698..$56A9]   18 bytes
      CYCLES(0x5698, 31);
      /* $5698 LDA */ s_a = update_nz(peek(0x1520 + s_x));
      /* $569B STA */ poke_zpg(0x04, s_a);
      /* $569D LDA */ s_a = update_nz(peek(0x1510 + s_x));
      /* $56A0 STA */ poke_zpg(0xfc, s_a);
      /* $56A2 STA */ poke_zpg(0xfe, s_a);
      /* $56A4 LDA */ s_a = update_nz(peek(0x1550 + s_x));
      /* $56A7 TAX */ s_x = update_nz(s_a);
      /* $56A8 LDY */ s_y = update_nz(0x00);
    case 0x56aa: // [$56AA..$56BA]   17 bytes
      CYCLES(0x56aa, 29);
      /* $56AA LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $56AC STA */ poke_zpg(0x06, s_a);
      /* $56AE LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $56B0 STA */ poke_zpg(0x07, s_a);
      /* $56B2 STY */ poke_zpg(0x05, s_y);
      /* $56B4 LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $56B6 TXA */ s_a = update_nz(s_x);
      /* $56B7 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $56B9 BEQ */ s_pc = s_status & STATUS_Z ? 0x56ce : 0x56bb;
      branchTarget = true;
      break;
    case 0x56bb: // [$56BB..$56C1]    7 bytes
      CYCLES(0x56bb, 12);
      /* $56BB STX */ poke_zpg(0x01, s_x);
      /* $56BD LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $56BF JSR */ push16(0x56c1), s_pc = 0x59aa;
      branchTarget = true;
      break;
    case 0x56c2: // [$56C2..$56C3]    2 bytes
      CYCLES(0x56c2, 4);
      /* $56C2 BCC */ s_pc = !(s_status & STATUS_C) ? 0x56cc : 0x56c4;
      branchTarget = true;
      break;
    case 0x56c4: // [$56C4..$56C6]    3 bytes
      CYCLES(0x56c4, 6);
      /* $56C4 JSR */ push16(0x56c6), s_pc = 0x5883;
      branchTarget = true;
      break;
    case 0x56c7: // [$56C7..$56CB]    5 bytes
      CYCLES(0x56c7, 9);
      /* $56C7 LDX */ s_x = update_nz(peek_zpg(0x1f));
      /* $56C9 JMP */ s_pc = 0x5959;
      branchTarget = true;
      break;
    case 0x56cc: // [$56CC..$56CD]    2 bytes
      CYCLES(0x56cc, 4);
      /* $56CC LDX */ s_x = update_nz(peek_zpg(0x01));
    case 0x56ce: // [$56CE..$56D6]    9 bytes
      CYCLES(0x56ce, 16);
      /* $56CE TXA */ s_a = update_nz(s_x);
      /* $56CF ORA */ s_a = update_nz(s_a | peek(peek16_zpg(0x06) + s_y));
      /* $56D1 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $56D3 TXA */ s_a = update_nz(s_x);
      /* $56D4 ASL */ s_a = update_nzc(s_a << 1);
      /* $56D5 BMI */ s_pc = s_status & STATUS_N ? 0x56dd : 0x56d7;
      branchTarget = true;
      break;
    case 0x56d7: // [$56D7..$56DC]    6 bytes
      CYCLES(0x56d7, 11);
      /* $56D7 TAX */ s_x = update_nz(s_a);
      /* $56D8 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $56DA INY */ s_y = update_nz(s_y + 1);
      /* $56DB BNE */ s_pc = !(s_status & STATUS_Z) ? 0x56aa : 0x56dd;
      branchTarget = true;
      break;
    case 0x56dd: // [$56DD..$56E1]    5 bytes
      CYCLES(0x56dd, 9);
      /* $56DD LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $56DF JMP */ s_pc = 0x5525;
      branchTarget = true;
      break;
    case 0x56e2: // [$56E2..$56E5]    4 bytes
      CYCLES(0x56e2, 7);
      /* $56E2 CMP */ update_nz_inv_c(s_a - 0x07);
      /* $56E4 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5757 : 0x56e6;
      branchTarget = true;
      break;
    case 0x56e6: // [$56E6..$56EA]    5 bytes
      CYCLES(0x56e6, 9);
      /* $56E6 LDA */ s_a = update_nz(peek(0x1540 + s_x));
      /* $56E9 BEQ */ s_pc = s_status & STATUS_Z ? 0x56f0 : 0x56eb;
      branchTarget = true;
      break;
    case 0x56eb: // [$56EB..$56EF]    5 bytes
      CYCLES(0x56eb, 9);
      /* $56EB DEC */ tmp16 = 0x1540 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $56EE BPL */ s_pc = !(s_status & STATUS_N) ? 0x570e : 0x56f0;
      branchTarget = true;
      break;
    case 0x56f0: // [$56F0..$56F2]    3 bytes
      CYCLES(0x56f0, 6);
      /* $56F0 JSR */ push16(0x56f2), s_pc = 0x58b9;
      branchTarget = true;
      break;
    case 0x56f3: // [$56F3..$56FE]   12 bytes
      CYCLES(0x56f3, 21);
      /* $56F3 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $56F5 LDA */ s_a = update_nz(0x80);
      /* $56F7 STA */ poke(0x1550 + s_x, s_a);
      /* $56FA DEC */ tmp16 = 0x1520 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $56FD BMI */ s_pc = s_status & STATUS_N ? 0x570b : 0x56ff;
      branchTarget = true;
      break;
    case 0x56ff: // [$56FF..$570A]   12 bytes
      CYCLES(0x56ff, 21);
      /* $56FF TYA */ s_a = update_nz(s_y);
      /* $5700 CLC */ s_status &= ~STATUS_C;
      /* $5701 ADC */ tmp = peek(0x1510 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5704 STA */ poke(0x1510 + s_x, s_a);
      /* $5707 CMP */ update_nz_inv_c(s_a - 0xbd);
      /* $5709 BCC */ s_pc = !(s_status & STATUS_C) ? 0x570e : 0x570b;
      branchTarget = true;
      break;
    case 0x570b: // [$570B..$570D]    3 bytes
      CYCLES(0x570b, 6);
      /* $570B JMP */ s_pc = 0x5959;
      branchTarget = true;
      break;
    case 0x570e: // [$570E..$571F]   18 bytes
      CYCLES(0x570e, 31);
      /* $570E LDA */ s_a = update_nz(peek(0x1520 + s_x));
      /* $5711 STA */ poke_zpg(0x04, s_a);
      /* $5713 LDA */ s_a = update_nz(peek(0x1510 + s_x));
      /* $5716 STA */ poke_zpg(0xfc, s_a);
      /* $5718 STA */ poke_zpg(0xfe, s_a);
      /* $571A LDA */ s_a = update_nz(peek(0x1550 + s_x));
      /* $571D TAX */ s_x = update_nz(s_a);
      /* $571E LDY */ s_y = update_nz(0x00);
    case 0x5720: // [$5720..$572F]   16 bytes
      CYCLES(0x5720, 28);
      /* $5720 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $5722 STA */ poke_zpg(0x06, s_a);
      /* $5724 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $5726 STA */ poke_zpg(0x07, s_a);
      /* $5728 STY */ poke_zpg(0x05, s_y);
      /* $572A LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $572C TXA */ s_a = update_nz(s_x);
      /* $572D LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $572E BCS */ s_pc = s_status & STATUS_C ? 0x5752 : 0x5730;
      branchTarget = true;
      break;
    case 0x5730: // [$5730..$5734]    5 bytes
      CYCLES(0x5730, 9);
      /* $5730 TAX */ s_x = update_nz(s_a);
      /* $5731 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $5733 BEQ */ s_pc = s_status & STATUS_Z ? 0x5748 : 0x5735;
      branchTarget = true;
      break;
    case 0x5735: // [$5735..$573B]    7 bytes
      CYCLES(0x5735, 12);
      /* $5735 STX */ poke_zpg(0x01, s_x);
      /* $5737 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $5739 JSR */ push16(0x573b), s_pc = 0x59aa;
      branchTarget = true;
      break;
    case 0x573c: // [$573C..$573D]    2 bytes
      CYCLES(0x573c, 4);
      /* $573C BCC */ s_pc = !(s_status & STATUS_C) ? 0x5746 : 0x573e;
      branchTarget = true;
      break;
    case 0x573e: // [$573E..$5740]    3 bytes
      CYCLES(0x573e, 6);
      /* $573E JSR */ push16(0x5740), s_pc = 0x58b9;
      branchTarget = true;
      break;
    case 0x5741: // [$5741..$5745]    5 bytes
      CYCLES(0x5741, 9);
      /* $5741 LDX */ s_x = update_nz(peek_zpg(0x1f));
      /* $5743 JMP */ s_pc = 0x5959;
      branchTarget = true;
      break;
    case 0x5746: // [$5746..$5747]    2 bytes
      CYCLES(0x5746, 4);
      /* $5746 LDX */ s_x = update_nz(peek_zpg(0x01));
    case 0x5748: // [$5748..$5751]   10 bytes
      CYCLES(0x5748, 18);
      /* $5748 TXA */ s_a = update_nz(s_x);
      /* $5749 ORA */ s_a = update_nz(s_a | peek(peek16_zpg(0x06) + s_y));
      /* $574B STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $574D LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $574F INY */ s_y = update_nz(s_y + 1);
      /* $5750 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5720 : 0x5752;
      branchTarget = true;
      break;
    case 0x5752: // [$5752..$5756]    5 bytes
      CYCLES(0x5752, 9);
      /* $5752 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $5754 JMP */ s_pc = 0x5525;
      branchTarget = true;
      break;
    case 0x5757: // [$5757..$575A]    4 bytes
      CYCLES(0x5757, 7);
      /* $5757 CMP */ update_nz_inv_c(s_a - 0x0d);
      /* $5759 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x57cc : 0x575b;
      branchTarget = true;
      break;
    case 0x575b: // [$575B..$575F]    5 bytes
      CYCLES(0x575b, 9);
      /* $575B LDA */ s_a = update_nz(peek(0x1540 + s_x));
      /* $575E BEQ */ s_pc = s_status & STATUS_Z ? 0x5765 : 0x5760;
      branchTarget = true;
      break;
    case 0x5760: // [$5760..$5764]    5 bytes
      CYCLES(0x5760, 9);
      /* $5760 DEC */ tmp16 = 0x1540 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $5763 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5782 : 0x5765;
      branchTarget = true;
      break;
    case 0x5765: // [$5765..$5767]    3 bytes
      CYCLES(0x5765, 6);
      /* $5765 JSR */ push16(0x5767), s_pc = 0x58ee;
      branchTarget = true;
      break;
    case 0x5768: // [$5768..$5778]   17 bytes
      CYCLES(0x5768, 29);
      /* $5768 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $576A LDA */ s_a = update_nz(0x01);
      /* $576C STA */ poke(0x1550 + s_x, s_a);
      /* $576F INC */ tmp16 = 0x1520 + s_x, poke(tmp16, update_nz(peek(tmp16) + 1));
      /* $5772 LDA */ s_a = update_nz(peek(0x1520 + s_x));
      /* $5775 CMP */ update_nz_inv_c(s_a - 0x25);
      /* $5777 BCS */ s_pc = s_status & STATUS_C ? 0x577f : 0x5779;
      branchTarget = true;
      break;
    case 0x5779: // [$5779..$577E]    6 bytes
      CYCLES(0x5779, 11);
      /* $5779 TYA */ s_a = update_nz(s_y);
      /* $577A STA */ poke(0x1510 + s_x, s_a);
      /* $577D BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5782 : 0x577f;
      branchTarget = true;
      break;
    case 0x577f: // [$577F..$5781]    3 bytes
      CYCLES(0x577f, 6);
      /* $577F JMP */ s_pc = 0x5959;
      branchTarget = true;
      break;
    case 0x5782: // [$5782..$5793]   18 bytes
      CYCLES(0x5782, 31);
      /* $5782 LDA */ s_a = update_nz(peek(0x1520 + s_x));
      /* $5785 STA */ poke_zpg(0x04, s_a);
      /* $5787 LDA */ s_a = update_nz(0x00);
      /* $5789 STA */ poke_zpg(0xfc, s_a);
      /* $578B STA */ poke_zpg(0xfe, s_a);
      /* $578D LDY */ s_y = update_nz(peek(0x1510 + s_x));
      /* $5790 LDA */ s_a = update_nz(peek(0x1550 + s_x));
      /* $5793 TAX */ s_x = update_nz(s_a);
    case 0x5794: // [$5794..$57A4]   17 bytes
      CYCLES(0x5794, 29);
      /* $5794 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $5796 STA */ poke_zpg(0x06, s_a);
      /* $5798 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $579A STA */ poke_zpg(0x07, s_a);
      /* $579C STY */ poke_zpg(0x05, s_y);
      /* $579E LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $57A0 TXA */ s_a = update_nz(s_x);
      /* $57A1 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $57A3 BEQ */ s_pc = s_status & STATUS_Z ? 0x57b8 : 0x57a5;
      branchTarget = true;
      break;
    case 0x57a5: // [$57A5..$57AB]    7 bytes
      CYCLES(0x57a5, 12);
      /* $57A5 STX */ poke_zpg(0x01, s_x);
      /* $57A7 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $57A9 JSR */ push16(0x57ab), s_pc = 0x59aa;
      branchTarget = true;
      break;
    case 0x57ac: // [$57AC..$57AD]    2 bytes
      CYCLES(0x57ac, 4);
      /* $57AC BCC */ s_pc = !(s_status & STATUS_C) ? 0x57b6 : 0x57ae;
      branchTarget = true;
      break;
    case 0x57ae: // [$57AE..$57B0]    3 bytes
      CYCLES(0x57ae, 6);
      /* $57AE JSR */ push16(0x57b0), s_pc = 0x58ee;
      branchTarget = true;
      break;
    case 0x57b1: // [$57B1..$57B5]    5 bytes
      CYCLES(0x57b1, 9);
      /* $57B1 LDX */ s_x = update_nz(peek_zpg(0x1f));
      /* $57B3 JMP */ s_pc = 0x5959;
      branchTarget = true;
      break;
    case 0x57b6: // [$57B6..$57B7]    2 bytes
      CYCLES(0x57b6, 4);
      /* $57B6 LDX */ s_x = update_nz(peek_zpg(0x01));
    case 0x57b8: // [$57B8..$57C0]    9 bytes
      CYCLES(0x57b8, 16);
      /* $57B8 TXA */ s_a = update_nz(s_x);
      /* $57B9 ORA */ s_a = update_nz(s_a | peek(peek16_zpg(0x06) + s_y));
      /* $57BB STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $57BD TXA */ s_a = update_nz(s_x);
      /* $57BE ASL */ s_a = update_nzc(s_a << 1);
      /* $57BF BMI */ s_pc = s_status & STATUS_N ? 0x57c7 : 0x57c1;
      branchTarget = true;
      break;
    case 0x57c1: // [$57C1..$57C6]    6 bytes
      CYCLES(0x57c1, 11);
      /* $57C1 TAX */ s_x = update_nz(s_a);
      /* $57C2 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $57C4 DEY */ s_y = update_nz(s_y - 1);
      /* $57C5 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5794 : 0x57c7;
      branchTarget = true;
      break;
    case 0x57c7: // [$57C7..$57CB]    5 bytes
      CYCLES(0x57c7, 9);
      /* $57C7 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $57C9 JMP */ s_pc = 0x5525;
      branchTarget = true;
      break;
    case 0x57cc: // [$57CC..$57D0]    5 bytes
      CYCLES(0x57cc, 9);
      /* $57CC LDA */ s_a = update_nz(peek(0x1540 + s_x));
      /* $57CF BEQ */ s_pc = s_status & STATUS_Z ? 0x57d6 : 0x57d1;
      branchTarget = true;
      break;
    case 0x57d1: // [$57D1..$57D5]    5 bytes
      CYCLES(0x57d1, 9);
      /* $57D1 DEC */ tmp16 = 0x1540 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $57D4 BPL */ s_pc = !(s_status & STATUS_N) ? 0x57ee : 0x57d6;
      branchTarget = true;
      break;
    case 0x57d6: // [$57D6..$57D8]    3 bytes
      CYCLES(0x57d6, 6);
      /* $57D6 JSR */ push16(0x57d8), s_pc = 0x5924;
      branchTarget = true;
      break;
    case 0x57d9: // [$57D9..$57E4]   12 bytes
      CYCLES(0x57d9, 21);
      /* $57D9 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $57DB LDA */ s_a = update_nz(0x80);
      /* $57DD STA */ poke(0x1550 + s_x, s_a);
      /* $57E0 DEC */ tmp16 = 0x1520 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $57E3 BMI */ s_pc = s_status & STATUS_N ? 0x57eb : 0x57e5;
      branchTarget = true;
      break;
    case 0x57e5: // [$57E5..$57EA]    6 bytes
      CYCLES(0x57e5, 11);
      /* $57E5 TYA */ s_a = update_nz(s_y);
      /* $57E6 STA */ poke(0x1510 + s_x, s_a);
      /* $57E9 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x57ee : 0x57eb;
      branchTarget = true;
      break;
    case 0x57eb: // [$57EB..$57ED]    3 bytes
      CYCLES(0x57eb, 6);
      /* $57EB JMP */ s_pc = 0x5959;
      branchTarget = true;
      break;
    case 0x57ee: // [$57EE..$57FF]   18 bytes
      CYCLES(0x57ee, 31);
      /* $57EE LDA */ s_a = update_nz(peek(0x1520 + s_x));
      /* $57F1 STA */ poke_zpg(0x04, s_a);
      /* $57F3 LDA */ s_a = update_nz(0x00);
      /* $57F5 STA */ poke_zpg(0xfc, s_a);
      /* $57F7 STA */ poke_zpg(0xfe, s_a);
      /* $57F9 LDY */ s_y = update_nz(peek(0x1510 + s_x));
      /* $57FC LDA */ s_a = update_nz(peek(0x1550 + s_x));
      /* $57FF TAX */ s_x = update_nz(s_a);
    case 0x5800: // [$5800..$580F]   16 bytes
      CYCLES(0x5800, 28);
      /* $5800 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $5802 STA */ poke_zpg(0x06, s_a);
      /* $5804 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $5806 STA */ poke_zpg(0x07, s_a);
      /* $5808 STY */ poke_zpg(0x05, s_y);
      /* $580A LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $580C TXA */ s_a = update_nz(s_x);
      /* $580D LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $580E BCS */ s_pc = s_status & STATUS_C ? 0x5832 : 0x5810;
      branchTarget = true;
      break;
    case 0x5810: // [$5810..$5814]    5 bytes
      CYCLES(0x5810, 9);
      /* $5810 TAX */ s_x = update_nz(s_a);
      /* $5811 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $5813 BEQ */ s_pc = s_status & STATUS_Z ? 0x5828 : 0x5815;
      branchTarget = true;
      break;
    case 0x5815: // [$5815..$581B]    7 bytes
      CYCLES(0x5815, 12);
      /* $5815 STX */ poke_zpg(0x01, s_x);
      /* $5817 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $5819 JSR */ push16(0x581b), s_pc = 0x59aa;
      branchTarget = true;
      break;
    case 0x581c: // [$581C..$581D]    2 bytes
      CYCLES(0x581c, 4);
      /* $581C BCC */ s_pc = !(s_status & STATUS_C) ? 0x5826 : 0x581e;
      branchTarget = true;
      break;
    case 0x581e: // [$581E..$5820]    3 bytes
      CYCLES(0x581e, 6);
      /* $581E JSR */ push16(0x5820), s_pc = 0x5924;
      branchTarget = true;
      break;
    case 0x5821: // [$5821..$5825]    5 bytes
      CYCLES(0x5821, 9);
      /* $5821 LDX */ s_x = update_nz(peek_zpg(0x1f));
      /* $5823 JMP */ s_pc = 0x5959;
      branchTarget = true;
      break;
    case 0x5826: // [$5826..$5827]    2 bytes
      CYCLES(0x5826, 4);
      /* $5826 LDX */ s_x = update_nz(peek_zpg(0x01));
    case 0x5828: // [$5828..$5831]   10 bytes
      CYCLES(0x5828, 18);
      /* $5828 TXA */ s_a = update_nz(s_x);
      /* $5829 ORA */ s_a = update_nz(s_a | peek(peek16_zpg(0x06) + s_y));
      /* $582B STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $582D LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $582F DEY */ s_y = update_nz(s_y - 1);
      /* $5830 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5800 : 0x5832;
      branchTarget = true;
      break;
    case 0x5832: // [$5832..$5836]    5 bytes
      CYCLES(0x5832, 9);
      /* $5832 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $5834 JMP */ s_pc = 0x5525;
      branchTarget = true;
      break;
    case 0x5837: // [$5837..$5854]   30 bytes
      CYCLES(0x5837, 52);
      /* $5837 LDA */ s_a = update_nz(peek(0x1510 + s_x));
      /* $583A STA */ poke_zpg(0xfc, s_a);
      /* $583C STA */ poke_zpg(0xfe, s_a);
      /* $583E LDY */ s_y = update_nz(0x00);
      /* $5840 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $5842 STA */ poke_zpg(0x06, s_a);
      /* $5844 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $5846 STA */ poke_zpg(0x07, s_a);
      /* $5848 LDY */ s_y = update_nz(peek(0x1520 + s_x));
      /* $584B LDA */ s_a = update_nz(peek(0x1550 + s_x));
      /* $584E EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $5850 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $5852 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $5854 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5855: // [$5855..$5869]   21 bytes
      CYCLES(0x5855, 36);
      /* $5855 LDA */ s_a = update_nz(peek(0x1510 + s_x));
      /* $5858 STA */ poke_zpg(0xfc, s_a);
      /* $585A STA */ poke_zpg(0xfe, s_a);
      /* $585C LDY */ s_y = update_nz(peek(0x1560 + s_x));
      /* $585F LDA */ s_a = update_nz(peek(0x1520 + s_x));
      /* $5862 STA */ poke_zpg(0x04, s_a);
      /* $5864 LDA */ s_a = update_nz(peek(0x1550 + s_x));
      /* $5867 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $5869 TAX */ s_x = update_nz(s_a);
    case 0x586a: // [$586A..$587F]   22 bytes
      CYCLES(0x586a, 38);
      /* $586A LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $586C STA */ poke_zpg(0x06, s_a);
      /* $586E LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $5870 STA */ poke_zpg(0x07, s_a);
      /* $5872 STY */ poke_zpg(0x05, s_y);
      /* $5874 LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $5876 TXA */ s_a = update_nz(s_x);
      /* $5877 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $5879 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $587B LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $587D DEY */ s_y = update_nz(s_y - 1);
      /* $587E BPL */ s_pc = !(s_status & STATUS_N) ? 0x586a : 0x5880;
      branchTarget = true;
      break;
    case 0x5880: // [$5880..$5882]    3 bytes
      CYCLES(0x5880, 6);
      /* $5880 JMP */ s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x5883: // [$5883..$5896]   20 bytes
      CYCLES(0x5883, 35);
      /* $5883 LDA */ s_a = update_nz(peek(0x1520 + s_x));
      /* $5886 STA */ poke_zpg(0x04, s_a);
      /* $5888 LDA */ s_a = update_nz(peek(0x1510 + s_x));
      /* $588B STA */ poke_zpg(0xfc, s_a);
      /* $588D STA */ poke_zpg(0xfe, s_a);
      /* $588F LDA */ s_a = update_nz(peek(0x1550 + s_x));
      /* $5892 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $5894 TAX */ s_x = update_nz(s_a);
      /* $5895 LDY */ s_y = update_nz(0x00);
    case 0x5897: // [$5897..$58AC]   22 bytes
      CYCLES(0x5897, 38);
      /* $5897 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $5899 STA */ poke_zpg(0x06, s_a);
      /* $589B LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $589D STA */ poke_zpg(0x07, s_a);
      /* $589F STY */ poke_zpg(0x05, s_y);
      /* $58A1 LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $58A3 TXA */ s_a = update_nz(s_x);
      /* $58A4 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $58A6 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $58A8 TXA */ s_a = update_nz(s_x);
      /* $58A9 SEC */ s_status |= STATUS_C;
      /* $58AA ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $58AB BPL */ s_pc = !(s_status & STATUS_N) ? 0x58b3 : 0x58ad;
      branchTarget = true;
      break;
    case 0x58ad: // [$58AD..$58B2]    6 bytes
      CYCLES(0x58ad, 11);
      /* $58AD TAX */ s_x = update_nz(s_a);
      /* $58AE LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $58B0 INY */ s_y = update_nz(s_y + 1);
      /* $58B1 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5897 : 0x58b3;
      branchTarget = true;
      break;
    case 0x58b3: // [$58B3..$58B8]    6 bytes
      CYCLES(0x58b3, 11);
      /* $58B3 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $58B5 INY */ s_y = update_nz(s_y + 1);
      /* $58B6 JMP */ s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x58b9: // [$58B9..$58CC]   20 bytes
      CYCLES(0x58b9, 35);
      /* $58B9 LDA */ s_a = update_nz(peek(0x1520 + s_x));
      /* $58BC STA */ poke_zpg(0x04, s_a);
      /* $58BE LDA */ s_a = update_nz(peek(0x1510 + s_x));
      /* $58C1 STA */ poke_zpg(0xfc, s_a);
      /* $58C3 STA */ poke_zpg(0xfe, s_a);
      /* $58C5 LDA */ s_a = update_nz(peek(0x1550 + s_x));
      /* $58C8 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $58CA TAX */ s_x = update_nz(s_a);
      /* $58CB LDY */ s_y = update_nz(0x00);
    case 0x58cd: // [$58CD..$58DD]   17 bytes
      CYCLES(0x58cd, 29);
      /* $58CD LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $58CF STA */ poke_zpg(0x06, s_a);
      /* $58D1 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $58D3 STA */ poke_zpg(0x07, s_a);
      /* $58D5 STY */ poke_zpg(0x05, s_y);
      /* $58D7 LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $58D9 TXA */ s_a = update_nz(s_x);
      /* $58DA SEC */ s_status |= STATUS_C;
      /* $58DB ROR */ tmp = s_a, s_a = update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7)), set_c_to_bit0(tmp);
      /* $58DC BCC */ s_pc = !(s_status & STATUS_C) ? 0x58e8 : 0x58de;
      branchTarget = true;
      break;
    case 0x58de: // [$58DE..$58E7]   10 bytes
      CYCLES(0x58de, 18);
      /* $58DE TAX */ s_x = update_nz(s_a);
      /* $58DF AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $58E1 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $58E3 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $58E5 INY */ s_y = update_nz(s_y + 1);
      /* $58E6 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x58cd : 0x58e8;
      branchTarget = true;
      break;
    case 0x58e8: // [$58E8..$58ED]    6 bytes
      CYCLES(0x58e8, 11);
      /* $58E8 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $58EA INY */ s_y = update_nz(s_y + 1);
      /* $58EB JMP */ s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x58ee: // [$58EE..$5901]   20 bytes
      CYCLES(0x58ee, 35);
      /* $58EE LDA */ s_a = update_nz(peek(0x1520 + s_x));
      /* $58F1 STA */ poke_zpg(0x04, s_a);
      /* $58F3 LDA */ s_a = update_nz(0x00);
      /* $58F5 STA */ poke_zpg(0xfc, s_a);
      /* $58F7 STA */ poke_zpg(0xfe, s_a);
      /* $58F9 LDY */ s_y = update_nz(peek(0x1510 + s_x));
      /* $58FC LDA */ s_a = update_nz(peek(0x1550 + s_x));
      /* $58FF EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $5901 TAX */ s_x = update_nz(s_a);
    case 0x5902: // [$5902..$5917]   22 bytes
      CYCLES(0x5902, 38);
      /* $5902 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $5904 STA */ poke_zpg(0x06, s_a);
      /* $5906 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $5908 STA */ poke_zpg(0x07, s_a);
      /* $590A STY */ poke_zpg(0x05, s_y);
      /* $590C LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $590E TXA */ s_a = update_nz(s_x);
      /* $590F AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $5911 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $5913 TXA */ s_a = update_nz(s_x);
      /* $5914 SEC */ s_status |= STATUS_C;
      /* $5915 ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $5916 BPL */ s_pc = !(s_status & STATUS_N) ? 0x591e : 0x5918;
      branchTarget = true;
      break;
    case 0x5918: // [$5918..$591D]    6 bytes
      CYCLES(0x5918, 11);
      /* $5918 TAX */ s_x = update_nz(s_a);
      /* $5919 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $591B DEY */ s_y = update_nz(s_y - 1);
      /* $591C BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5902 : 0x591e;
      branchTarget = true;
      break;
    case 0x591e: // [$591E..$5923]    6 bytes
      CYCLES(0x591e, 11);
      /* $591E LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $5920 DEY */ s_y = update_nz(s_y - 1);
      /* $5921 JMP */ s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x5924: // [$5924..$5937]   20 bytes
      CYCLES(0x5924, 35);
      /* $5924 LDA */ s_a = update_nz(peek(0x1520 + s_x));
      /* $5927 STA */ poke_zpg(0x04, s_a);
      /* $5929 LDA */ s_a = update_nz(0x00);
      /* $592B STA */ poke_zpg(0xfc, s_a);
      /* $592D STA */ poke_zpg(0xfe, s_a);
      /* $592F LDY */ s_y = update_nz(peek(0x1510 + s_x));
      /* $5932 LDA */ s_a = update_nz(peek(0x1550 + s_x));
      /* $5935 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $5937 TAX */ s_x = update_nz(s_a);
    case 0x5938: // [$5938..$5948]   17 bytes
      CYCLES(0x5938, 29);
      /* $5938 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $593A STA */ poke_zpg(0x06, s_a);
      /* $593C LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $593E STA */ poke_zpg(0x07, s_a);
      /* $5940 STY */ poke_zpg(0x05, s_y);
      /* $5942 LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $5944 TXA */ s_a = update_nz(s_x);
      /* $5945 SEC */ s_status |= STATUS_C;
      /* $5946 ROR */ tmp = s_a, s_a = update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7)), set_c_to_bit0(tmp);
      /* $5947 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5953 : 0x5949;
      branchTarget = true;
      break;
    case 0x5949: // [$5949..$5952]   10 bytes
      CYCLES(0x5949, 18);
      /* $5949 TAX */ s_x = update_nz(s_a);
      /* $594A AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $594C STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $594E LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $5950 DEY */ s_y = update_nz(s_y - 1);
      /* $5951 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5938 : 0x5953;
      branchTarget = true;
      break;
    case 0x5953: // [$5953..$5958]    6 bytes
      CYCLES(0x5953, 11);
      /* $5953 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $5955 DEY */ s_y = update_nz(s_y - 1);
      /* $5956 JMP */ s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x5959: // [$5959..$595D]    5 bytes
      CYCLES(0x5959, 9);
      /* $5959 DEC */ tmp16 = 0x1400, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $595C TXA */ s_a = update_nz(s_x);
      /* $595D PHA */ push8(s_a);
    case 0x595e: // [$595E..$5987]   42 bytes
      CYCLES(0x595e, 72);
      /* $595E LDA */ s_a = update_nz(peek(0x1511 + s_x));
      /* $5961 STA */ poke(0x1510 + s_x, s_a);
      /* $5964 LDA */ s_a = update_nz(peek(0x1521 + s_x));
      /* $5967 STA */ poke(0x1520 + s_x, s_a);
      /* $596A LDA */ s_a = update_nz(peek(0x1531 + s_x));
      /* $596D STA */ poke(0x1530 + s_x, s_a);
      /* $5970 LDA */ s_a = update_nz(peek(0x1541 + s_x));
      /* $5973 STA */ poke(0x1540 + s_x, s_a);
      /* $5976 LDA */ s_a = update_nz(peek(0x1551 + s_x));
      /* $5979 STA */ poke(0x1550 + s_x, s_a);
      /* $597C LDA */ s_a = update_nz(peek(0x1561 + s_x));
      /* $597F STA */ poke(0x1560 + s_x, s_a);
      /* $5982 INX */ s_x = update_nz(s_x + 1);
      /* $5983 CPX */ update_nz_inv_c(s_x - peek(0x1400));
      /* $5986 BCC */ s_pc = !(s_status & STATUS_C) ? 0x595e : 0x5988;
      branchTarget = true;
      break;
    case 0x5988: // [$5988..$598C]    5 bytes
      CYCLES(0x5988, 9);
      /* $5988 PLA */ s_a = update_nz(pop8());
      /* $5989 TAX */ s_x = update_nz(s_a);
      /* $598A JMP */ s_pc = 0x5525;
      branchTarget = true;
      break;
    case 0x598d: // [$598D..$598E]    2 bytes
      CYCLES(0x598d, 4);
      /* $598D LDX */ s_x = update_nz(0xff);
    case 0x598f: // [$598F..$5992]    4 bytes
      CYCLES(0x598f, 7);
      /* $598F INX */ s_x = update_nz(s_x + 1);
      /* $5990 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $5991 BCC */ s_pc = !(s_status & STATUS_C) ? 0x598f : 0x5993;
      branchTarget = true;
      break;
    case 0x5993: // [$5993..$59A9]   23 bytes
      CYCLES(0x5993, 40);
      /* $5993 STX */ poke_zpg(0x19, s_x);
      /* $5995 TYA */ s_a = update_nz(s_y);
      /* $5996 STA */ poke_zpg(0x1a, s_a);
      /* $5998 ASL */ s_a = update_nzc(s_a << 1);
      /* $5999 ADC */ tmp = peek_zpg(0x1a), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $599B ASL */ s_a = update_nzc(s_a << 1);
      /* $599C ADC */ tmp = peek_zpg(0x1a), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $599E ADC */ tmp = peek_zpg(0x19), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $59A0 STA */ poke_zpg(0x1c, s_a);
      /* $59A2 LDA */ s_a = update_nz(peek_zpg(0xfc));
      /* $59A4 CLC */ s_status &= ~STATUS_C;
      /* $59A5 ADC */ tmp = peek_zpg(0x05), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $59A7 STA */ poke_zpg(0x1d, s_a);
      /* $59A9 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x59aa: // [$59AA..$59B0]    7 bytes
      CYCLES(0x59aa, 12);
      /* $59AA STX */ poke_zpg(0x1f, s_x);
      /* $59AC STY */ poke_zpg(0x1e, s_y);
      /* $59AE JSR */ push16(0x59b0), s_pc = 0x598d;
      branchTarget = true;
      break;
    case 0x59b1: // [$59B1..$59B4]    4 bytes
      CYCLES(0x59b1, 7);
      /* $59B1 LDA */ s_a = update_nz(0x00);
      /* $59B3 STA */ poke_zpg(0xe8, s_a);
    case 0x59b5: // [$59B5..$59B7]    3 bytes
      CYCLES(0x59b5, 6);
      /* $59B5 JSR */ push16(0x59b7), s_pc = 0x5a12;
      branchTarget = true;
      break;
    case 0x59b8: // [$59B8..$59B9]    2 bytes
      CYCLES(0x59b8, 4);
      /* $59B8 BCS */ s_pc = s_status & STATUS_C ? 0x59ec : 0x59ba;
      branchTarget = true;
      break;
    case 0x59ba: // [$59BA..$59BC]    3 bytes
      CYCLES(0x59ba, 6);
      /* $59BA JSR */ push16(0x59bc), s_pc = 0x5a15;
      branchTarget = true;
      break;
    case 0x59bd: // [$59BD..$59BE]    2 bytes
      CYCLES(0x59bd, 4);
      /* $59BD BCS */ s_pc = s_status & STATUS_C ? 0x59ec : 0x59bf;
      branchTarget = true;
      break;
    case 0x59bf: // [$59BF..$59C1]    3 bytes
      CYCLES(0x59bf, 6);
      /* $59BF JSR */ push16(0x59c1), s_pc = 0x620c;
      branchTarget = true;
      break;
    case 0x59c2: // [$59C2..$59C3]    2 bytes
      CYCLES(0x59c2, 4);
      /* $59C2 BCS */ s_pc = s_status & STATUS_C ? 0x59ec : 0x59c4;
      branchTarget = true;
      break;
    case 0x59c4: // [$59C4..$59C6]    3 bytes
      CYCLES(0x59c4, 6);
      /* $59C4 JSR */ push16(0x59c6), s_pc = 0x620f;
      branchTarget = true;
      break;
    case 0x59c7: // [$59C7..$59C8]    2 bytes
      CYCLES(0x59c7, 4);
      /* $59C7 BCS */ s_pc = s_status & STATUS_C ? 0x59ec : 0x59c9;
      branchTarget = true;
      break;
    case 0x59c9: // [$59C9..$59CB]    3 bytes
      CYCLES(0x59c9, 6);
      /* $59C9 JSR */ push16(0x59cb), s_pc = 0x6212;
      branchTarget = true;
      break;
    case 0x59cc: // [$59CC..$59CD]    2 bytes
      CYCLES(0x59cc, 4);
      /* $59CC BCS */ s_pc = s_status & STATUS_C ? 0x59ec : 0x59ce;
      branchTarget = true;
      break;
    case 0x59ce: // [$59CE..$59D0]    3 bytes
      CYCLES(0x59ce, 6);
      /* $59CE JSR */ push16(0x59d0), s_pc = 0x700c;
      branchTarget = true;
      break;
    case 0x59d1: // [$59D1..$59D2]    2 bytes
      CYCLES(0x59d1, 4);
      /* $59D1 BCS */ s_pc = s_status & STATUS_C ? 0x59ec : 0x59d3;
      branchTarget = true;
      break;
    case 0x59d3: // [$59D3..$59D5]    3 bytes
      CYCLES(0x59d3, 6);
      /* $59D3 JSR */ push16(0x59d5), s_pc = 0x700f;
      branchTarget = true;
      break;
    case 0x59d6: // [$59D6..$59D7]    2 bytes
      CYCLES(0x59d6, 4);
      /* $59D6 BCS */ s_pc = s_status & STATUS_C ? 0x59ec : 0x59d8;
      branchTarget = true;
      break;
    case 0x59d8: // [$59D8..$59DA]    3 bytes
      CYCLES(0x59d8, 6);
      /* $59D8 JSR */ push16(0x59da), s_pc = 0x7012;
      branchTarget = true;
      break;
    case 0x59db: // [$59DB..$59DC]    2 bytes
      CYCLES(0x59db, 4);
      /* $59DB BCS */ s_pc = s_status & STATUS_C ? 0x59ec : 0x59dd;
      branchTarget = true;
      break;
    case 0x59dd: // [$59DD..$59DF]    3 bytes
      CYCLES(0x59dd, 6);
      /* $59DD JSR */ push16(0x59df), s_pc = 0x690c;
      branchTarget = true;
      break;
    case 0x59e0: // [$59E0..$59E1]    2 bytes
      CYCLES(0x59e0, 4);
      /* $59E0 BCS */ s_pc = s_status & STATUS_C ? 0x59ec : 0x59e2;
      branchTarget = true;
      break;
    case 0x59e2: // [$59E2..$59E4]    3 bytes
      CYCLES(0x59e2, 6);
      /* $59E2 JSR */ push16(0x59e4), s_pc = 0x690f;
      branchTarget = true;
      break;
    case 0x59e5: // [$59E5..$59E6]    2 bytes
      CYCLES(0x59e5, 4);
      /* $59E5 BCS */ s_pc = s_status & STATUS_C ? 0x59ec : 0x59e7;
      branchTarget = true;
      break;
    case 0x59e7: // [$59E7..$59E9]    3 bytes
      CYCLES(0x59e7, 6);
      /* $59E7 JSR */ push16(0x59e9), s_pc = 0x6912;
      branchTarget = true;
      break;
    case 0x59ea: // [$59EA..$59EB]    2 bytes
      CYCLES(0x59ea, 4);
      /* $59EA BCS */ s_pc = s_status & STATUS_C ? 0x59ec : 0x59ec;
      branchTarget = true;
      break;
    case 0x59ec: // [$59EC..$59F0]    5 bytes
      CYCLES(0x59ec, 9);
      /* $59EC LDX */ s_x = update_nz(peek_zpg(0x1f));
      /* $59EE LDY */ s_y = update_nz(peek_zpg(0x1e));
      /* $59F0 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5a00: // [$5A00..$5A02]    3 bytes
      CYCLES(0x5a00, 6);
      /* $5A00 JMP */ s_pc = 0x5a2a;
      branchTarget = true;
      break;
    case 0x5a03: // [$5A03..$5A05]    3 bytes
      CYCLES(0x5a03, 6);
      /* $5A03 JMP */ s_pc = 0x5ab1;
      branchTarget = true;
      break;
    case 0x5a06: // [$5A06..$5A08]    3 bytes
      CYCLES(0x5a06, 6);
      /* $5A06 JMP */ s_pc = 0x5b26;
      branchTarget = true;
      break;
    case 0x5a09: // [$5A09..$5A0B]    3 bytes
      CYCLES(0x5a09, 6);
      /* $5A09 JMP */ s_pc = 0x5b7e;
      branchTarget = true;
      break;
    case 0x5a0c: // [$5A0C..$5A0E]    3 bytes
      CYCLES(0x5a0c, 6);
      /* $5A0C JMP */ s_pc = 0x5c9f;
      branchTarget = true;
      break;
    case 0x5a0f: // [$5A0F..$5A11]    3 bytes
      CYCLES(0x5a0f, 6);
      /* $5A0F JMP */ s_pc = 0x5e60;
      branchTarget = true;
      break;
    case 0x5a12: // [$5A12..$5A14]    3 bytes
      CYCLES(0x5a12, 6);
      /* $5A12 JMP */ s_pc = 0x5bf2;
      branchTarget = true;
      break;
    case 0x5a15: // [$5A15..$5A17]    3 bytes
      CYCLES(0x5a15, 6);
      /* $5A15 JMP */ s_pc = 0x5dbe;
      branchTarget = true;
      break;
    case 0x5a18: // [$5A18..$5A1A]    3 bytes
      CYCLES(0x5a18, 6);
      /* $5A18 JMP */ s_pc = 0x6073;
      branchTarget = true;
      break;
    case 0x5a1b: // [$5A1B..$5A1D]    3 bytes
      CYCLES(0x5a1b, 6);
      /* $5A1B JMP */ s_pc = 0x5f8a;
      branchTarget = true;
      break;
    case 0x5a1e: // [$5A1E..$5A20]    3 bytes
      CYCLES(0x5a1e, 6);
      /* $5A1E JMP */ s_pc = 0x5fe3;
      branchTarget = true;
      break;
    case 0x5a21: // [$5A21..$5A23]    3 bytes
      CYCLES(0x5a21, 6);
      /* $5A21 JMP */ s_pc = 0x5f52;
      branchTarget = true;
      break;
    case 0x5a24: // [$5A24..$5A26]    3 bytes
      CYCLES(0x5a24, 6);
      /* $5A24 JMP */ s_pc = 0x6127;
      branchTarget = true;
      break;
    case 0x5a27: // [$5A27..$5A29]    3 bytes
      CYCLES(0x5a27, 6);
      /* $5A27 JMP */ s_pc = 0x61b9;
      branchTarget = true;
      break;
    case 0x5a2a: // [$5A2A..$5A37]   14 bytes
      CYCLES(0x5a2a, 24);
      /* $5A2A LDA */ s_a = update_nz(0x00);
      /* $5A2C STA */ poke(0x1408, s_a);
      /* $5A2F LDY */ s_y = update_nz(peek(0x1402));
      /* $5A32 TYA */ s_a = update_nz(s_y);
      /* $5A33 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $5A34 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $5A35 STA */ poke(0x1421, s_a);
    case 0x5a38: // [$5A38..$5A3A]    3 bytes
      CYCLES(0x5a38, 6);
      /* $5A38 DEY */ s_y = update_nz(s_y - 1);
      /* $5A39 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5a51 : 0x5a3b;
      branchTarget = true;
      break;
    case 0x5a3b: // [$5A3B..$5A42]    8 bytes
      CYCLES(0x5a3b, 14);
      /* $5A3B LDA */ s_a = update_nz(peek(0x1407));
      /* $5A3E LDX */ s_x = update_nz(0x0a);
      /* $5A40 JSR */ push16(0x5a42), s_pc = 0x4c00;
      branchTarget = true;
      break;
    case 0x5a43: // [$5A43..$5A49]    7 bytes
      CYCLES(0x5a43, 12);
      /* $5A43 LDA */ s_a = update_nz(peek(0x0c0c));
      /* $5A46 CPX */ update_nz_inv_c(s_x - 0x08);
      /* $5A48 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5a4d : 0x5a4a;
      branchTarget = true;
      break;
    case 0x5a4a: // [$5A4A..$5A4C]    3 bytes
      CYCLES(0x5a4a, 6);
      /* $5A4A LDA */ s_a = update_nz(peek(0x0c0f));
    case 0x5a4d: // [$5A4D..$5A50]    4 bytes
      CYCLES(0x5a4d, 7);
      /* $5A4D STA */ poke(0x1403, s_a);
      /* $5A50 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5a51: // [$5A51..$5A53]    3 bytes
      CYCLES(0x5a51, 6);
      /* $5A51 JSR */ push16(0x5a53), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x5a54: // [$5A54..$5A57]    4 bytes
      CYCLES(0x5a54, 7);
      /* $5A54 CMP */ update_nz_inv_c(s_a - 0xa0);
      /* $5A56 BCS */ s_pc = s_status & STATUS_C ? 0x5a7c : 0x5a58;
      branchTarget = true;
      break;
    case 0x5a58: // [$5A58..$5A5C]    5 bytes
      CYCLES(0x5a58, 9);
      /* $5A58 LDA */ s_a = update_nz(0xf5);
      /* $5A5A JSR */ push16(0x5a5c), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x5a5d: // [$5A5D..$5A66]   10 bytes
      CYCLES(0x5a5d, 18);
      /* $5A5D AND */ s_a = update_nz(s_a & 0xfe);
      /* $5A5F STA */ poke(0x1570 + s_y, s_a);
      /* $5A62 LDA */ s_a = update_nz(0x22);
      /* $5A64 JSR */ push16(0x5a66), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x5a67: // [$5A67..$5A6D]    7 bytes
      CYCLES(0x5a67, 12);
      /* $5A67 STA */ poke_zpg(0x00, s_a);
      /* $5A69 LDX */ s_x = update_nz(0x00);
      /* $5A6B JSR */ push16(0x5a6d), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x5a6e: // [$5A6E..$5A6F]    2 bytes
      CYCLES(0x5a6e, 4);
      /* $5A6E BPL */ s_pc = !(s_status & STATUS_N) ? 0x5a72 : 0x5a70;
      branchTarget = true;
      break;
    case 0x5a70: // [$5A70..$5A71]    2 bytes
      CYCLES(0x5a70, 4);
      /* $5A70 LDX */ s_x = update_nz(0x90);
    case 0x5a72: // [$5A72..$5A7B]   10 bytes
      CYCLES(0x5a72, 18);
      /* $5A72 TXA */ s_a = update_nz(s_x);
      /* $5A73 CLC */ s_status &= ~STATUS_C;
      /* $5A74 ADC */ tmp = peek_zpg(0x00), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5A76 STA */ poke(0x15f0 + s_y, s_a);
      /* $5A79 SEC */ s_status |= STATUS_C;
      /* $5A7A BCS */ s_pc = s_status & STATUS_C ? 0x5a9d : 0x5a7c;
      branchTarget = true;
      break;
    case 0x5a7c: // [$5A7C..$5A80]    5 bytes
      CYCLES(0x5a7c, 9);
      /* $5A7C LDA */ s_a = update_nz(0xb2);
      /* $5A7E JSR */ push16(0x5a80), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x5a81: // [$5A81..$5A88]    8 bytes
      CYCLES(0x5a81, 14);
      /* $5A81 STA */ poke(0x15f0 + s_y, s_a);
      /* $5A84 LDA */ s_a = update_nz(0x45);
      /* $5A86 JSR */ push16(0x5a88), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x5a89: // [$5A89..$5A8F]    7 bytes
      CYCLES(0x5a89, 12);
      /* $5A89 STA */ poke_zpg(0x00, s_a);
      /* $5A8B LDX */ s_x = update_nz(0x00);
      /* $5A8D JSR */ push16(0x5a8f), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x5a90: // [$5A90..$5A91]    2 bytes
      CYCLES(0x5a90, 4);
      /* $5A90 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5a94 : 0x5a92;
      branchTarget = true;
      break;
    case 0x5a92: // [$5A92..$5A93]    2 bytes
      CYCLES(0x5a92, 4);
      /* $5A92 LDX */ s_x = update_nz(0xb0);
    case 0x5a94: // [$5A94..$5A9C]    9 bytes
      CYCLES(0x5a94, 16);
      /* $5A94 TXA */ s_a = update_nz(s_x);
      /* $5A95 CLC */ s_status &= ~STATUS_C;
      /* $5A96 ADC */ tmp = peek_zpg(0x00), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5A98 AND */ s_a = update_nz(s_a & 0xfe);
      /* $5A9A STA */ poke(0x1570 + s_y, s_a);
    case 0x5a9d: // [$5A9D..$5A9F]    3 bytes
      CYCLES(0x5a9d, 6);
      /* $5A9D JSR */ push16(0x5a9f), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x5aa0: // [$5AA0..$5AB0]   17 bytes
      CYCLES(0x5aa0, 29);
      /* $5AA0 AND */ s_a = update_nz(s_a & 0x01);
      /* $5AA2 STA */ poke(0x1670 + s_y, s_a);
      /* $5AA5 TYA */ s_a = update_nz(s_y);
      /* $5AA6 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $5AA7 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $5AA8 CLC */ s_status &= ~STATUS_C;
      /* $5AA9 ADC */ tmp = peek(0x0c07), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5AAC STA */ poke(0x16f0 + s_y, s_a);
      /* $5AAF BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5a38 : 0x5ab1;
      branchTarget = true;
      break;
    case 0x5ab1: // [$5AB1..$5AB3]    3 bytes
      CYCLES(0x5ab1, 6);
      /* $5AB1 LDY */ s_y = update_nz(peek(0x1409));
    case 0x5ab4: // [$5AB4..$5AB6]    3 bytes
      CYCLES(0x5ab4, 6);
      /* $5AB4 DEY */ s_y = update_nz(s_y - 1);
      /* $5AB5 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5ab8 : 0x5ab7;
      branchTarget = true;
      break;
    case 0x5ab7: // [$5AB7..$5AB7]    1 bytes
      CYCLES(0x5ab7, 2);
      /* $5AB7 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5ab8: // [$5AB8..$5ABA]    3 bytes
      CYCLES(0x5ab8, 6);
      /* $5AB8 JSR */ push16(0x5aba), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x5abb: // [$5ABB..$5ABE]    4 bytes
      CYCLES(0x5abb, 7);
      /* $5ABB CMP */ update_nz_inv_c(s_a - 0xa0);
      /* $5ABD BCS */ s_pc = s_status & STATUS_C ? 0x5ae4 : 0x5abf;
      branchTarget = true;
      break;
    case 0x5abf: // [$5ABF..$5AC3]    5 bytes
      CYCLES(0x5abf, 9);
      /* $5ABF LDA */ s_a = update_nz(0xe6);
      /* $5AC1 JSR */ push16(0x5ac3), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x5ac4: // [$5AC4..$5AD0]   13 bytes
      CYCLES(0x5ac4, 23);
      /* $5AC4 CLC */ s_status &= ~STATUS_C;
      /* $5AC5 ADC */ tmp = 0x06, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5AC7 ORA */ s_a = update_nz(s_a | 0x01);
      /* $5AC9 STA */ poke(0x1900 + s_y, s_a);
      /* $5ACC LDX */ s_x = update_nz(0x06);
      /* $5ACE JSR */ push16(0x5ad0), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x5ad1: // [$5AD1..$5AD2]    2 bytes
      CYCLES(0x5ad1, 4);
      /* $5AD1 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5ad5 : 0x5ad3;
      branchTarget = true;
      break;
    case 0x5ad3: // [$5AD3..$5AD4]    2 bytes
      CYCLES(0x5ad3, 4);
      /* $5AD3 LDX */ s_x = update_nz(0x90);
    case 0x5ad5: // [$5AD5..$5ADB]    7 bytes
      CYCLES(0x5ad5, 12);
      /* $5AD5 STX */ poke_zpg(0x00, s_x);
      /* $5AD7 LDA */ s_a = update_nz(0x1c);
      /* $5AD9 JSR */ push16(0x5adb), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x5adc: // [$5ADC..$5AE3]    8 bytes
      CYCLES(0x5adc, 14);
      /* $5ADC CLC */ s_status &= ~STATUS_C;
      /* $5ADD ADC */ tmp = peek_zpg(0x00), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5ADF STA */ poke(0x1910 + s_y, s_a);
      /* $5AE2 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5b07 : 0x5ae4;
      branchTarget = true;
      break;
    case 0x5ae4: // [$5AE4..$5AE8]    5 bytes
      CYCLES(0x5ae4, 9);
      /* $5AE4 LDA */ s_a = update_nz(0xa6);
      /* $5AE6 JSR */ push16(0x5ae8), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x5ae9: // [$5AE9..$5AF3]   11 bytes
      CYCLES(0x5ae9, 19);
      /* $5AE9 CLC */ s_status &= ~STATUS_C;
      /* $5AEA ADC */ tmp = 0x06, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5AEC STA */ poke(0x1910 + s_y, s_a);
      /* $5AEF LDX */ s_x = update_nz(0x06);
      /* $5AF1 JSR */ push16(0x5af3), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x5af4: // [$5AF4..$5AF5]    2 bytes
      CYCLES(0x5af4, 4);
      /* $5AF4 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5af8 : 0x5af6;
      branchTarget = true;
      break;
    case 0x5af6: // [$5AF6..$5AF7]    2 bytes
      CYCLES(0x5af6, 4);
      /* $5AF6 LDX */ s_x = update_nz(0xc0);
    case 0x5af8: // [$5AF8..$5AFE]    7 bytes
      CYCLES(0x5af8, 12);
      /* $5AF8 STX */ poke_zpg(0x00, s_x);
      /* $5AFA LDA */ s_a = update_nz(0x2c);
      /* $5AFC JSR */ push16(0x5afe), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x5aff: // [$5AFF..$5B06]    8 bytes
      CYCLES(0x5aff, 14);
      /* $5AFF CLC */ s_status &= ~STATUS_C;
      /* $5B00 ADC */ tmp = peek_zpg(0x00), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5B02 ORA */ s_a = update_nz(s_a | 0x01);
      /* $5B04 STA */ poke(0x1900 + s_y, s_a);
    case 0x5b07: // [$5B07..$5B09]    3 bytes
      CYCLES(0x5b07, 6);
      /* $5B07 JSR */ push16(0x5b09), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x5b0a: // [$5B0A..$5B1A]   17 bytes
      CYCLES(0x5b0a, 29);
      /* $5B0A AND */ s_a = update_nz(s_a & 0x01);
      /* $5B0C STA */ poke(0x1920 + s_y, s_a);
      /* $5B0F TYA */ s_a = update_nz(s_y);
      /* $5B10 AND */ s_a = update_nz(s_a & 0x03);
      /* $5B12 STA */ poke(0x1930 + s_y, s_a);
      /* $5B15 LDA */ s_a = update_nz(peek(0x0c0e));
      /* $5B18 JSR */ push16(0x5b1a), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x5b1b: // [$5B1B..$5B25]   11 bytes
      CYCLES(0x5b1b, 19);
      /* $5B1B CLC */ s_status &= ~STATUS_C;
      /* $5B1C ADC */ tmp = 0x03, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5B1E ORA */ s_a = update_nz(s_a | 0x01);
      /* $5B20 STA */ poke(0x1940 + s_y, s_a);
      /* $5B23 JMP */ s_pc = 0x5ab4;
      branchTarget = true;
      break;
    case 0x5b26: // [$5B26..$5B28]    3 bytes
      CYCLES(0x5b26, 6);
      /* $5B26 LDY */ s_y = update_nz(peek(0x140a));
    case 0x5b29: // [$5B29..$5B2B]    3 bytes
      CYCLES(0x5b29, 6);
      /* $5B29 DEY */ s_y = update_nz(s_y - 1);
      /* $5B2A BPL */ s_pc = !(s_status & STATUS_N) ? 0x5b45 : 0x5b2c;
      branchTarget = true;
      break;
    case 0x5b2c: // [$5B2C..$5B44]   25 bytes
      CYCLES(0x5b2c, 43);
      /* $5B2C LDA */ s_a = update_nz(0x00);
      /* $5B2E STA */ poke(0x140b, s_a);
      /* $5B31 STA */ poke(0x140c, s_a);
      /* $5B34 LDA */ s_a = update_nz(0x0a);
      /* $5B36 STA */ poke(0x140d, s_a);
      /* $5B39 LDA */ s_a = update_nz(0x00);
      /* $5B3B STA */ poke(0x1405, s_a);
      /* $5B3E STA */ poke(0x142c, s_a);
      /* $5B41 STA */ poke(0x142d, s_a);
      /* $5B44 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5b45: // [$5B45..$5B49]    5 bytes
      CYCLES(0x5b45, 9);
      /* $5B45 LDA */ s_a = update_nz(0xf1);
      /* $5B47 JSR */ push16(0x5b49), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x5b4a: // [$5B4A..$5B54]   11 bytes
      CYCLES(0x5b4a, 19);
      /* $5B4A CLC */ s_status &= ~STATUS_C;
      /* $5B4B ADC */ tmp = 0x04, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5B4D STA */ poke(0x1950 + s_y, s_a);
      /* $5B50 LDA */ s_a = update_nz(0xae);
      /* $5B52 JSR */ push16(0x5b54), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x5b55: // [$5B55..$5B5D]    9 bytes
      CYCLES(0x5b55, 16);
      /* $5B55 CLC */ s_status &= ~STATUS_C;
      /* $5B56 ADC */ tmp = 0x04, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5B58 STA */ poke(0x1960 + s_y, s_a);
      /* $5B5B JSR */ push16(0x5b5d), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x5b5e: // [$5B5E..$5B67]   10 bytes
      CYCLES(0x5b5e, 18);
      /* $5B5E AND */ s_a = update_nz(s_a & 0x01);
      /* $5B60 STA */ poke(0x1970 + s_y, s_a);
      /* $5B63 LDA */ s_a = update_nz(0x05);
      /* $5B65 JSR */ push16(0x5b67), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x5b68: // [$5B68..$5B78]   17 bytes
      CYCLES(0x5b68, 29);
      /* $5B68 SEC */ s_status |= STATUS_C;
      /* $5B69 SBC */ tmp = 0x02, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $5B6B STA */ poke(0x1980 + s_y, s_a);
      /* $5B6E TYA */ s_a = update_nz(s_y);
      /* $5B6F AND */ s_a = update_nz(s_a & 0x03);
      /* $5B71 STA */ poke(0x19a0 + s_y, s_a);
      /* $5B74 LDA */ s_a = update_nz(0x20);
      /* $5B76 JSR */ push16(0x5b78), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x5b79: // [$5B79..$5B7D]    5 bytes
      CYCLES(0x5b79, 9);
      /* $5B79 STA */ poke(0x19b0 + s_y, s_a);
      /* $5B7C BPL */ s_pc = !(s_status & STATUS_N) ? 0x5b29 : 0x5b7e;
      branchTarget = true;
      break;
    case 0x5b7e: // [$5B7E..$5B80]    3 bytes
      CYCLES(0x5b7e, 6);
      /* $5B7E LDX */ s_x = update_nz(peek(0x1402));
    case 0x5b81: // [$5B81..$5B83]    3 bytes
      CYCLES(0x5b81, 6);
      /* $5B81 DEX */ s_x = update_nz(s_x - 1);
      /* $5B82 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5b87 : 0x5b84;
      branchTarget = true;
      break;
    case 0x5b84: // [$5B84..$5B86]    3 bytes
      CYCLES(0x5b84, 6);
      /* $5B84 JMP */ s_pc = 0x6008;
      branchTarget = true;
      break;
    case 0x5b87: // [$5B87..$5B8B]    5 bytes
      CYCLES(0x5b87, 9);
      /* $5B87 DEC */ tmp16 = 0x16f0 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $5B8A BPL */ s_pc = !(s_status & STATUS_N) ? 0x5b81 : 0x5b8c;
      branchTarget = true;
      break;
    case 0x5b8c: // [$5B8C..$5BA4]   25 bytes
      CYCLES(0x5b8c, 43);
      /* $5B8C STX */ poke_zpg(0x00, s_x);
      /* $5B8E LDY */ s_y = update_nz(peek(0x1670 + s_x));
      /* $5B91 LDA */ s_a = update_nz(peek(0x0888 + s_y));
      /* $5B94 STA */ poke_zpg(0x06, s_a);
      /* $5B96 LDA */ s_a = update_nz(peek(0x088c + s_y));
      /* $5B99 STA */ poke_zpg(0x07, s_a);
      /* $5B9B LDY */ s_y = update_nz(peek(0x15f0 + s_x));
      /* $5B9E LDA */ s_a = update_nz(peek(0x1570 + s_x));
      /* $5BA1 TAX */ s_x = update_nz(s_a);
      /* $5BA2 JSR */ push16(0x5ba4), s_pc = 0x4fcd;
      branchTarget = true;
      break;
    case 0x5ba5: // [$5BA5..$5BC1]   29 bytes
      CYCLES(0x5ba5, 50);
      /* $5BA5 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $5BA7 LDA */ s_a = update_nz(peek(0x1670 + s_x));
      /* $5BAA EOR */ s_a = update_nz(s_a ^ 0x01);
      /* $5BAC STA */ poke(0x1670 + s_x, s_a);
      /* $5BAF TAY */ s_y = update_nz(s_a);
      /* $5BB0 LDA */ s_a = update_nz(peek(0x0880 + s_y));
      /* $5BB3 STA */ poke_zpg(0x06, s_a);
      /* $5BB5 LDA */ s_a = update_nz(peek(0x0884 + s_y));
      /* $5BB8 STA */ poke_zpg(0x07, s_a);
      /* $5BBA LDA */ s_a = update_nz(peek(0x15f0 + s_x));
      /* $5BBD CMP */ update_nz_inv_c(s_a - peek(0x1501));
      /* $5BC0 BCS */ s_pc = s_status & STATUS_C ? 0x5bc7 : 0x5bc2;
      branchTarget = true;
      break;
    case 0x5bc2: // [$5BC2..$5BC6]    5 bytes
      CYCLES(0x5bc2, 9);
      /* $5BC2 ADC */ tmp = peek(0x0c09), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5BC5 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5bca : 0x5bc7;
      branchTarget = true;
      break;
    case 0x5bc7: // [$5BC7..$5BC9]    3 bytes
      CYCLES(0x5bc7, 6);
      /* $5BC7 SBC */ tmp = peek(0x0c09), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
    case 0x5bca: // [$5BCA..$5BD5]   12 bytes
      CYCLES(0x5bca, 21);
      /* $5BCA STA */ poke(0x15f0 + s_x, s_a);
      /* $5BCD TAY */ s_y = update_nz(s_a);
      /* $5BCE LDA */ s_a = update_nz(peek(0x1570 + s_x));
      /* $5BD1 CMP */ update_nz_inv_c(s_a - peek(0x1500));
      /* $5BD4 BCS */ s_pc = s_status & STATUS_C ? 0x5bdb : 0x5bd6;
      branchTarget = true;
      break;
    case 0x5bd6: // [$5BD6..$5BDA]    5 bytes
      CYCLES(0x5bd6, 9);
      /* $5BD6 ADC */ tmp = peek(0x0c09), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5BD9 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5bde : 0x5bdb;
      branchTarget = true;
      break;
    case 0x5bdb: // [$5BDB..$5BDD]    3 bytes
      CYCLES(0x5bdb, 6);
      /* $5BDB SBC */ tmp = peek(0x0c09), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
    case 0x5bde: // [$5BDE..$5BE4]    7 bytes
      CYCLES(0x5bde, 12);
      /* $5BDE STA */ poke(0x1570 + s_x, s_a);
      /* $5BE1 TAX */ s_x = update_nz(s_a);
      /* $5BE2 JSR */ push16(0x5be4), s_pc = 0x4ffe;
      branchTarget = true;
      break;
    case 0x5be5: // [$5BE5..$5BEA]    6 bytes
      CYCLES(0x5be5, 11);
      /* $5BE5 LDA */ s_a = update_nz(peek(0x1403));
      /* $5BE8 JSR */ push16(0x5bea), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x5beb: // [$5BEB..$5BF1]    7 bytes
      CYCLES(0x5beb, 12);
      /* $5BEB LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $5BED STA */ poke(0x16f0 + s_x, s_a);
      /* $5BF0 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5b81 : 0x5bf2;
      branchTarget = true;
      break;
    case 0x5bf2: // [$5BF2..$5BFB]   10 bytes
      CYCLES(0x5bf2, 18);
      /* $5BF2 LDX */ s_x = update_nz(peek(0x1402));
      /* $5BF5 LDA */ s_a = update_nz(peek_zpg(0x1c));
      /* $5BF7 SEC */ s_status |= STATUS_C;
      /* $5BF8 SBC */ tmp = 0x0b, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $5BFA BCS */ s_pc = s_status & STATUS_C ? 0x5bfe : 0x5bfc;
      branchTarget = true;
      break;
    case 0x5bfc: // [$5BFC..$5BFD]    2 bytes
      CYCLES(0x5bfc, 4);
      /* $5BFC LDA */ s_a = update_nz(0x00);
    case 0x5bfe: // [$5BFE..$5BFE]    1 bytes
      CYCLES(0x5bfe, 2);
      /* $5BFE TAY */ s_y = update_nz(s_a);
    case 0x5bff: // [$5BFF..$5C00]    2 bytes
      CYCLES(0x5bff, 4);
      /* $5BFF LDA */ s_a = update_nz(peek_zpg(0x1c));
    case 0x5c01: // [$5C01..$5C03]    3 bytes
      CYCLES(0x5c01, 6);
      /* $5C01 DEX */ s_x = update_nz(s_x - 1);
      /* $5C02 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5c06 : 0x5c04;
      branchTarget = true;
      break;
    case 0x5c04: // [$5C04..$5C05]    2 bytes
      CYCLES(0x5c04, 4);
      /* $5C04 CLC */ s_status &= ~STATUS_C;
      /* $5C05 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5c06: // [$5C06..$5C0A]    5 bytes
      CYCLES(0x5c06, 9);
      /* $5C06 CMP */ update_nz_inv_c(s_a - peek(0x1570 + s_x));
      /* $5C09 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5c01 : 0x5c0b;
      branchTarget = true;
      break;
    case 0x5c0b: // [$5C0B..$5C10]    6 bytes
      CYCLES(0x5c0b, 11);
      /* $5C0B TYA */ s_a = update_nz(s_y);
      /* $5C0C CMP */ update_nz_inv_c(s_a - peek(0x1570 + s_x));
      /* $5C0F BCS */ s_pc = s_status & STATUS_C ? 0x5bff : 0x5c11;
      branchTarget = true;
      break;
    case 0x5c11: // [$5C11..$5C17]    7 bytes
      CYCLES(0x5c11, 12);
      /* $5C11 LDA */ s_a = update_nz(peek_zpg(0x1d));
      /* $5C13 CMP */ update_nz_inv_c(s_a - peek(0x15f0 + s_x));
      /* $5C16 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5bff : 0x5c18;
      branchTarget = true;
      break;
    case 0x5c18: // [$5C18..$5C1C]    5 bytes
      CYCLES(0x5c18, 9);
      /* $5C18 SEC */ s_status |= STATUS_C;
      /* $5C19 SBC */ tmp = 0x0e, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $5C1B BCC */ s_pc = !(s_status & STATUS_C) ? 0x5c22 : 0x5c1d;
      branchTarget = true;
      break;
    case 0x5c1d: // [$5C1D..$5C21]    5 bytes
      CYCLES(0x5c1d, 9);
      /* $5C1D CMP */ update_nz_inv_c(s_a - peek(0x15f0 + s_x));
      /* $5C20 BCS */ s_pc = s_status & STATUS_C ? 0x5bff : 0x5c22;
      branchTarget = true;
      break;
    case 0x5c22: // [$5C22..$5C25]    4 bytes
      CYCLES(0x5c22, 7);
      /* $5C22 LDA */ s_a = update_nz(peek_zpg(0xe8));
      /* $5C24 BEQ */ s_pc = s_status & STATUS_Z ? 0x5c28 : 0x5c26;
      branchTarget = true;
      break;
    case 0x5c26: // [$5C26..$5C27]    2 bytes
      CYCLES(0x5c26, 4);
      /* $5C26 SEC */ s_status |= STATUS_C;
      /* $5C27 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5c28: // [$5C28..$5C2A]    3 bytes
      CYCLES(0x5c28, 6);
      /* $5C28 JSR */ push16(0x5c2a), s_pc = 0x5c3f;
      branchTarget = true;
      break;
    case 0x5c2b: // [$5C2B..$5C3E]   20 bytes
      CYCLES(0x5c2b, 35);
      /* $5C2B LDA */ s_a = update_nz(peek(0x0c04));
      /* $5C2E STA */ poke(0x1406, s_a);
      /* $5C31 LDA */ s_a = update_nz(peek(0x0c05));
      /* $5C34 STA */ poke(0x1422, s_a);
      /* $5C37 LDA */ s_a = update_nz(peek(0x0c06));
      /* $5C3A STA */ poke(0x1423, s_a);
      /* $5C3D SEC */ s_status |= STATUS_C;
      /* $5C3E RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5c3f: // [$5C3F..$5C58]   26 bytes
      CYCLES(0x5c3f, 45);
      /* $5C3F TXA */ s_a = update_nz(s_x);
      /* $5C40 PHA */ push8(s_a);
      /* $5C41 TAX */ s_x = update_nz(s_a);
      /* $5C42 LDY */ s_y = update_nz(peek(0x1670 + s_x));
      /* $5C45 LDA */ s_a = update_nz(peek(0x0888 + s_y));
      /* $5C48 STA */ poke_zpg(0x06, s_a);
      /* $5C4A LDA */ s_a = update_nz(peek(0x088c + s_y));
      /* $5C4D STA */ poke_zpg(0x07, s_a);
      /* $5C4F LDY */ s_y = update_nz(peek(0x15f0 + s_x));
      /* $5C52 LDA */ s_a = update_nz(peek(0x1570 + s_x));
      /* $5C55 TAX */ s_x = update_nz(s_a);
      /* $5C56 JSR */ push16(0x5c58), s_pc = 0x4fcd;
      branchTarget = true;
      break;
    case 0x5c59: // [$5C59..$5C6E]   22 bytes
      CYCLES(0x5c59, 38);
      /* $5C59 PLA */ s_a = update_nz(pop8());
      /* $5C5A PHA */ push8(s_a);
      /* $5C5B TAX */ s_x = update_nz(s_a);
      /* $5C5C LDA */ s_a = update_nz(peek(0x15f0 + s_x));
      /* $5C5F CLC */ s_status &= ~STATUS_C;
      /* $5C60 ADC */ tmp = 0x07, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5C62 TAY */ s_y = update_nz(s_a);
      /* $5C63 LDA */ s_a = update_nz(peek(0x1570 + s_x));
      /* $5C66 CLC */ s_status &= ~STATUS_C;
      /* $5C67 ADC */ tmp = 0x03, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5C69 TAX */ s_x = update_nz(s_a);
      /* $5C6A LDA */ s_a = update_nz(0xaa);
      /* $5C6C JSR */ push16(0x5c6e), s_pc = 0x4f74;
      branchTarget = true;
      break;
    case 0x5c6f: // [$5C6F..$5C74]    6 bytes
      CYCLES(0x5c6f, 11);
      /* $5C6F PLA */ s_a = update_nz(pop8());
      /* $5C70 PHA */ push8(s_a);
      /* $5C71 TAX */ s_x = update_nz(s_a);
      /* $5C72 DEC */ tmp16 = 0x1402, poke(tmp16, update_nz(peek(tmp16) - 1));
    case 0x5c75: // [$5C75..$5C92]   30 bytes
      CYCLES(0x5c75, 52);
      /* $5C75 LDA */ s_a = update_nz(peek(0x1571 + s_x));
      /* $5C78 STA */ poke(0x1570 + s_x, s_a);
      /* $5C7B LDA */ s_a = update_nz(peek(0x15f1 + s_x));
      /* $5C7E STA */ poke(0x15f0 + s_x, s_a);
      /* $5C81 LDA */ s_a = update_nz(peek(0x1671 + s_x));
      /* $5C84 STA */ poke(0x1670 + s_x, s_a);
      /* $5C87 LDA */ s_a = update_nz(peek(0x16f1 + s_x));
      /* $5C8A STA */ poke(0x16f0 + s_x, s_a);
      /* $5C8D INX */ s_x = update_nz(s_x + 1);
      /* $5C8E CPX */ update_nz_inv_c(s_x - peek(0x1402));
      /* $5C91 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5c75 : 0x5c93;
      branchTarget = true;
      break;
    case 0x5c93: // [$5C93..$5C99]    7 bytes
      CYCLES(0x5c93, 12);
      /* $5C93 LDA */ s_a = update_nz(0x00);
      /* $5C95 LDX */ s_x = update_nz(0x64);
      /* $5C97 JSR */ push16(0x5c99), s_pc = 0x4dd0;
      branchTarget = true;
      break;
    case 0x5c9a: // [$5C9A..$5C9E]    5 bytes
      CYCLES(0x5c9a, 9);
      /* $5C9A PLA */ s_a = update_nz(pop8());
      /* $5C9B TAX */ s_x = update_nz(s_a);
      /* $5C9C JMP */ s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x5c9f: // [$5C9F..$5CA1]    3 bytes
      CYCLES(0x5c9f, 6);
      /* $5C9F LDX */ s_x = update_nz(peek(0x1409));
    case 0x5ca2: // [$5CA2..$5CA4]    3 bytes
      CYCLES(0x5ca2, 6);
      /* $5CA2 DEX */ s_x = update_nz(s_x - 1);
      /* $5CA3 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5ca6 : 0x5ca5;
      branchTarget = true;
      break;
    case 0x5ca5: // [$5CA5..$5CA5]    1 bytes
      CYCLES(0x5ca5, 2);
      /* $5CA5 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5ca6: // [$5CA6..$5CAA]    5 bytes
      CYCLES(0x5ca6, 9);
      /* $5CA6 DEC */ tmp16 = 0x1930 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $5CA9 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5ca2 : 0x5cab;
      branchTarget = true;
      break;
    case 0x5cab: // [$5CAB..$5CB6]   12 bytes
      CYCLES(0x5cab, 21);
      /* $5CAB STX */ poke_zpg(0x00, s_x);
      /* $5CAD LDA */ s_a = update_nz(0x03);
      /* $5CAF STA */ poke(0x1930 + s_x, s_a);
      /* $5CB2 DEC */ tmp16 = 0x1940 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $5CB5 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5cdf : 0x5cb7;
      branchTarget = true;
      break;
    case 0x5cb7: // [$5CB7..$5CBB]    5 bytes
      CYCLES(0x5cb7, 9);
      /* $5CB7 LDA */ s_a = update_nz(peek(0x1421));
      /* $5CBA BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5ccf : 0x5cbc;
      branchTarget = true;
      break;
    case 0x5cbc: // [$5CBC..$5CC7]   12 bytes
      CYCLES(0x5cbc, 21);
      /* $5CBC LDA */ s_a = update_nz(peek(0x1920 + s_x));
      /* $5CBF AND */ s_a = update_nz(s_a & 0x02);
      /* $5CC1 EOR */ s_a = update_nz(s_a ^ 0x02);
      /* $5CC3 STA */ poke_zpg(0x06, s_a);
      /* $5CC5 JSR */ push16(0x5cc7), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x5cc8: // [$5CC8..$5CCE]    7 bytes
      CYCLES(0x5cc8, 12);
      /* $5CC8 AND */ s_a = update_nz(s_a & 0x01);
      /* $5CCA ORA */ s_a = update_nz(s_a | peek_zpg(0x06));
      /* $5CCC STA */ poke(0x1920 + s_x, s_a);
    case 0x5ccf: // [$5CCF..$5CD4]    6 bytes
      CYCLES(0x5ccf, 11);
      /* $5CCF LDA */ s_a = update_nz(peek(0x0c0e));
      /* $5CD2 JSR */ push16(0x5cd4), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x5cd5: // [$5CD5..$5CDE]   10 bytes
      CYCLES(0x5cd5, 18);
      /* $5CD5 CLC */ s_status &= ~STATUS_C;
      /* $5CD6 ADC */ tmp = 0x03, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5CD8 ORA */ s_a = update_nz(s_a | 0x01);
      /* $5CDA LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $5CDC STA */ poke(0x1940 + s_y, s_a);
    case 0x5cdf: // [$5CDF..$5CEB]   13 bytes
      CYCLES(0x5cdf, 23);
      /* $5CDF LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $5CE1 LDA */ s_a = update_nz(peek(0x1900 + s_y));
      /* $5CE4 LDX */ s_x = update_nz(peek(0x1910 + s_y));
      /* $5CE7 LDY */ s_y = update_nz(0x0e);
      /* $5CE9 JSR */ push16(0x5ceb), s_pc = 0x4d21;
      branchTarget = true;
      break;
    case 0x5cec: // [$5CEC..$5CF2]    7 bytes
      CYCLES(0x5cec, 12);
      /* $5CEC LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $5CEE LDA */ s_a = update_nz(peek(0x1421));
      /* $5CF1 BEQ */ s_pc = s_status & STATUS_Z ? 0x5cfa : 0x5cf3;
      branchTarget = true;
      break;
    case 0x5cf3: // [$5CF3..$5CF9]    7 bytes
      CYCLES(0x5cf3, 12);
      /* $5CF3 LDA */ s_a = update_nz(peek(0x1920 + s_y));
      /* $5CF6 ASL */ s_a = update_nzc(s_a << 1);
      /* $5CF7 TAX */ s_x = update_nz(s_a);
      /* $5CF8 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5d5d : 0x5cfa;
      branchTarget = true;
      break;
    case 0x5cfa: // [$5CFA..$5D07]   14 bytes
      CYCLES(0x5cfa, 24);
      /* $5CFA LDX */ s_x = update_nz(peek(0x1920 + s_y));
      /* $5CFD LDA */ s_a = update_nz(peek(0x1900 + s_y));
      /* $5D00 CLC */ s_status &= ~STATUS_C;
      /* $5D01 ADC */ tmp = peek(0x0d14 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5D04 CMP */ update_nz_inv_c(s_a - 0x06);
      /* $5D06 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5d0c : 0x5d08;
      branchTarget = true;
      break;
    case 0x5d08: // [$5D08..$5D0B]    4 bytes
      CYCLES(0x5d08, 7);
      /* $5D08 CMP */ update_nz_inv_c(s_a - 0xed);
      /* $5D0A BCC */ s_pc = !(s_status & STATUS_C) ? 0x5d18 : 0x5d0c;
      branchTarget = true;
      break;
    case 0x5d0c: // [$5D0C..$5D0E]    3 bytes
      CYCLES(0x5d0c, 6);
      /* $5D0C JSR */ push16(0x5d0e), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x5d0f: // [$5D0F..$5D17]    9 bytes
      CYCLES(0x5d0f, 16);
      /* $5D0F AND */ s_a = update_nz(s_a & 0x01);
      /* $5D11 ORA */ s_a = update_nz(s_a | 0x02);
      /* $5D13 STA */ poke(0x1920 + s_y, s_a);
      /* $5D16 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5cfa : 0x5d18;
      branchTarget = true;
      break;
    case 0x5d18: // [$5D18..$5D23]   12 bytes
      CYCLES(0x5d18, 21);
      /* $5D18 STA */ poke(0x1900 + s_y, s_a);
      /* $5D1B CLC */ s_status &= ~STATUS_C;
      /* $5D1C ADC */ tmp = 0x10, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5D1E STA */ poke_zpg(0x1a, s_a);
      /* $5D20 SBC */ tmp = 0x19, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $5D22 BCS */ s_pc = s_status & STATUS_C ? 0x5d26 : 0x5d24;
      branchTarget = true;
      break;
    case 0x5d24: // [$5D24..$5D25]    2 bytes
      CYCLES(0x5d24, 4);
      /* $5D24 LDA */ s_a = update_nz(0x00);
    case 0x5d26: // [$5D26..$5D27]    2 bytes
      CYCLES(0x5d26, 4);
      /* $5D26 STA */ poke_zpg(0x1b, s_a);
    case 0x5d28: // [$5D28..$5D35]   14 bytes
      CYCLES(0x5d28, 24);
      /* $5D28 LDX */ s_x = update_nz(peek(0x1920 + s_y));
      /* $5D2B LDA */ s_a = update_nz(peek(0x1910 + s_y));
      /* $5D2E CLC */ s_status &= ~STATUS_C;
      /* $5D2F ADC */ tmp = peek(0x0d18 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5D32 CMP */ update_nz_inv_c(s_a - 0x06);
      /* $5D34 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5d3a : 0x5d36;
      branchTarget = true;
      break;
    case 0x5d36: // [$5D36..$5D39]    4 bytes
      CYCLES(0x5d36, 7);
      /* $5D36 CMP */ update_nz_inv_c(s_a - 0xac);
      /* $5D38 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5d44 : 0x5d3a;
      branchTarget = true;
      break;
    case 0x5d3a: // [$5D3A..$5D3C]    3 bytes
      CYCLES(0x5d3a, 6);
      /* $5D3A JSR */ push16(0x5d3c), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x5d3d: // [$5D3D..$5D43]    7 bytes
      CYCLES(0x5d3d, 12);
      /* $5D3D AND */ s_a = update_nz(s_a & 0x01);
      /* $5D3F STA */ poke(0x1920 + s_y, s_a);
      /* $5D42 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5d28 : 0x5d44;
      branchTarget = true;
      break;
    case 0x5d44: // [$5D44..$5D4F]   12 bytes
      CYCLES(0x5d44, 21);
      /* $5D44 STA */ poke(0x1910 + s_y, s_a);
      /* $5D47 CLC */ s_status &= ~STATUS_C;
      /* $5D48 ADC */ tmp = 0x11, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5D4A STA */ poke_zpg(0x1c, s_a);
      /* $5D4C SBC */ tmp = 0x1d, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $5D4E BCS */ s_pc = s_status & STATUS_C ? 0x5d52 : 0x5d50;
      branchTarget = true;
      break;
    case 0x5d50: // [$5D50..$5D51]    2 bytes
      CYCLES(0x5d50, 4);
      /* $5D50 LDA */ s_a = update_nz(0x00);
    case 0x5d52: // [$5D52..$5D5C]   11 bytes
      CYCLES(0x5d52, 19);
      /* $5D52 STA */ poke_zpg(0x1d, s_a);
      /* $5D54 LDA */ s_a = update_nz(peek(0x1940 + s_y));
      /* $5D57 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $5D58 LDA */ s_a = update_nz(peek(0x1920 + s_y));
      /* $5D5B ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $5D5C TAX */ s_x = update_nz(s_a);
    case 0x5d5d: // [$5D5D..$5D70]   20 bytes
      CYCLES(0x5d5d, 35);
      /* $5D5D LDA */ s_a = update_nz(peek(0x0890 + s_x));
      /* $5D60 STA */ poke_zpg(0x06, s_a);
      /* $5D62 LDA */ s_a = update_nz(peek(0x0898 + s_x));
      /* $5D65 STA */ poke_zpg(0x07, s_a);
      /* $5D67 LDX */ s_x = update_nz(peek(0x1900 + s_y));
      /* $5D6A LDA */ s_a = update_nz(peek(0x1910 + s_y));
      /* $5D6D TAY */ s_y = update_nz(s_a);
      /* $5D6E JSR */ push16(0x5d70), s_pc = 0x4cf6;
      branchTarget = true;
      break;
    case 0x5d71: // [$5D71..$5D75]    5 bytes
      CYCLES(0x5d71, 9);
      /* $5D71 LDA */ s_a = update_nz(peek(0x1421));
      /* $5D74 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5db9 : 0x5d76;
      branchTarget = true;
      break;
    case 0x5d76: // [$5D76..$5D7A]    5 bytes
      CYCLES(0x5d76, 9);
      /* $5D76 LDX */ s_x = update_nz(peek(0x140a));
      /* $5D79 LDY */ s_y = update_nz(peek_zpg(0x1b));
    case 0x5d7b: // [$5D7B..$5D7C]    2 bytes
      CYCLES(0x5d7b, 4);
      /* $5D7B LDA */ s_a = update_nz(peek_zpg(0x1a));
    case 0x5d7d: // [$5D7D..$5D7F]    3 bytes
      CYCLES(0x5d7d, 6);
      /* $5D7D DEX */ s_x = update_nz(s_x - 1);
      /* $5D7E BMI */ s_pc = s_status & STATUS_N ? 0x5db9 : 0x5d80;
      branchTarget = true;
      break;
    case 0x5d80: // [$5D80..$5D84]    5 bytes
      CYCLES(0x5d80, 9);
      /* $5D80 CMP */ update_nz_inv_c(s_a - peek(0x1950 + s_x));
      /* $5D83 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5d7d : 0x5d85;
      branchTarget = true;
      break;
    case 0x5d85: // [$5D85..$5D8A]    6 bytes
      CYCLES(0x5d85, 11);
      /* $5D85 TYA */ s_a = update_nz(s_y);
      /* $5D86 CMP */ update_nz_inv_c(s_a - peek(0x1950 + s_x));
      /* $5D89 BCS */ s_pc = s_status & STATUS_C ? 0x5d7b : 0x5d8b;
      branchTarget = true;
      break;
    case 0x5d8b: // [$5D8B..$5D91]    7 bytes
      CYCLES(0x5d8b, 12);
      /* $5D8B LDA */ s_a = update_nz(peek_zpg(0x1c));
      /* $5D8D CMP */ update_nz_inv_c(s_a - peek(0x1960 + s_x));
      /* $5D90 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5d7b : 0x5d92;
      branchTarget = true;
      break;
    case 0x5d92: // [$5D92..$5D98]    7 bytes
      CYCLES(0x5d92, 12);
      /* $5D92 LDA */ s_a = update_nz(peek_zpg(0x1d));
      /* $5D94 CMP */ update_nz_inv_c(s_a - peek(0x1960 + s_x));
      /* $5D97 BCS */ s_pc = s_status & STATUS_C ? 0x5d7b : 0x5d99;
      branchTarget = true;
      break;
    case 0x5d99: // [$5D99..$5DA6]   14 bytes
      CYCLES(0x5d99, 24);
      /* $5D99 TXA */ s_a = update_nz(s_x);
      /* $5D9A PHA */ push8(s_a);
      /* $5D9B TAY */ s_y = update_nz(s_a);
      /* $5D9C LDX */ s_x = update_nz(peek(0x1960 + s_y));
      /* $5D9F LDA */ s_a = update_nz(peek(0x1950 + s_y));
      /* $5DA2 LDY */ s_y = update_nz(0x0a);
      /* $5DA4 JSR */ push16(0x5da6), s_pc = 0x4cd0;
      branchTarget = true;
      break;
    case 0x5da7: // [$5DA7..$5DB3]   13 bytes
      CYCLES(0x5da7, 23);
      /* $5DA7 PLA */ s_a = update_nz(pop8());
      /* $5DA8 TAX */ s_x = update_nz(s_a);
      /* $5DA9 PHA */ push8(s_a);
      /* $5DAA LDY */ s_y = update_nz(peek(0x1960 + s_x));
      /* $5DAD LDA */ s_a = update_nz(peek(0x1950 + s_x));
      /* $5DB0 TAX */ s_x = update_nz(s_a);
      /* $5DB1 JSR */ push16(0x5db3), s_pc = 0x6127;
      branchTarget = true;
      break;
    case 0x5db4: // [$5DB4..$5DB8]    5 bytes
      CYCLES(0x5db4, 9);
      /* $5DB4 PLA */ s_a = update_nz(pop8());
      /* $5DB5 TAX */ s_x = update_nz(s_a);
      /* $5DB6 JSR */ push16(0x5db8), s_pc = 0x5f52;
      branchTarget = true;
      break;
    case 0x5db9: // [$5DB9..$5DBD]    5 bytes
      CYCLES(0x5db9, 9);
      /* $5DB9 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $5DBB JMP */ s_pc = 0x5ca2;
      branchTarget = true;
      break;
    case 0x5dbe: // [$5DBE..$5DC7]   10 bytes
      CYCLES(0x5dbe, 18);
      /* $5DBE LDX */ s_x = update_nz(peek(0x1409));
      /* $5DC1 LDA */ s_a = update_nz(peek_zpg(0x1c));
      /* $5DC3 SEC */ s_status |= STATUS_C;
      /* $5DC4 SBC */ tmp = 0x0d, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $5DC6 BCS */ s_pc = s_status & STATUS_C ? 0x5dca : 0x5dc8;
      branchTarget = true;
      break;
    case 0x5dc8: // [$5DC8..$5DC9]    2 bytes
      CYCLES(0x5dc8, 4);
      /* $5DC8 LDA */ s_a = update_nz(0x00);
    case 0x5dca: // [$5DCA..$5DCA]    1 bytes
      CYCLES(0x5dca, 2);
      /* $5DCA TAY */ s_y = update_nz(s_a);
    case 0x5dcb: // [$5DCB..$5DCC]    2 bytes
      CYCLES(0x5dcb, 4);
      /* $5DCB LDA */ s_a = update_nz(peek_zpg(0x1c));
    case 0x5dcd: // [$5DCD..$5DCF]    3 bytes
      CYCLES(0x5dcd, 6);
      /* $5DCD DEX */ s_x = update_nz(s_x - 1);
      /* $5DCE BPL */ s_pc = !(s_status & STATUS_N) ? 0x5dd2 : 0x5dd0;
      branchTarget = true;
      break;
    case 0x5dd0: // [$5DD0..$5DD1]    2 bytes
      CYCLES(0x5dd0, 4);
      /* $5DD0 CLC */ s_status &= ~STATUS_C;
      /* $5DD1 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5dd2: // [$5DD2..$5DD6]    5 bytes
      CYCLES(0x5dd2, 9);
      /* $5DD2 CMP */ update_nz_inv_c(s_a - peek(0x1900 + s_x));
      /* $5DD5 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5dcd : 0x5dd7;
      branchTarget = true;
      break;
    case 0x5dd7: // [$5DD7..$5DDC]    6 bytes
      CYCLES(0x5dd7, 11);
      /* $5DD7 TYA */ s_a = update_nz(s_y);
      /* $5DD8 CMP */ update_nz_inv_c(s_a - peek(0x1900 + s_x));
      /* $5DDB BCS */ s_pc = s_status & STATUS_C ? 0x5dcb : 0x5ddd;
      branchTarget = true;
      break;
    case 0x5ddd: // [$5DDD..$5DE3]    7 bytes
      CYCLES(0x5ddd, 12);
      /* $5DDD LDA */ s_a = update_nz(peek_zpg(0x1d));
      /* $5DDF CMP */ update_nz_inv_c(s_a - peek(0x1910 + s_x));
      /* $5DE2 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5dcb : 0x5de4;
      branchTarget = true;
      break;
    case 0x5de4: // [$5DE4..$5DE8]    5 bytes
      CYCLES(0x5de4, 9);
      /* $5DE4 SEC */ s_status |= STATUS_C;
      /* $5DE5 SBC */ tmp = 0x0e, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $5DE7 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5dee : 0x5de9;
      branchTarget = true;
      break;
    case 0x5de9: // [$5DE9..$5DED]    5 bytes
      CYCLES(0x5de9, 9);
      /* $5DE9 CMP */ update_nz_inv_c(s_a - peek(0x1910 + s_x));
      /* $5DEC BCS */ s_pc = s_status & STATUS_C ? 0x5dcb : 0x5dee;
      branchTarget = true;
      break;
    case 0x5dee: // [$5DEE..$5DF1]    4 bytes
      CYCLES(0x5dee, 7);
      /* $5DEE LDA */ s_a = update_nz(peek_zpg(0xe8));
      /* $5DF0 BEQ */ s_pc = s_status & STATUS_Z ? 0x5df4 : 0x5df2;
      branchTarget = true;
      break;
    case 0x5df2: // [$5DF2..$5DF3]    2 bytes
      CYCLES(0x5df2, 4);
      /* $5DF2 SEC */ s_status |= STATUS_C;
      /* $5DF3 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5df4: // [$5DF4..$5E01]   14 bytes
      CYCLES(0x5df4, 24);
      /* $5DF4 TXA */ s_a = update_nz(s_x);
      /* $5DF5 PHA */ push8(s_a);
      /* $5DF6 TAY */ s_y = update_nz(s_a);
      /* $5DF7 LDX */ s_x = update_nz(peek(0x1910 + s_y));
      /* $5DFA LDA */ s_a = update_nz(peek(0x1900 + s_y));
      /* $5DFD LDY */ s_y = update_nz(0x0e);
      /* $5DFF JSR */ push16(0x5e01), s_pc = 0x4d21;
      branchTarget = true;
      break;
    case 0x5e02: // [$5E02..$5E06]    5 bytes
      CYCLES(0x5e02, 9);
      /* $5E02 PLA */ s_a = update_nz(pop8());
      /* $5E03 TAX */ s_x = update_nz(s_a);
      /* $5E04 JSR */ push16(0x5e06), s_pc = 0x5e26;
      branchTarget = true;
      break;
    case 0x5e07: // [$5E07..$5E23]   29 bytes
      CYCLES(0x5e07, 50);
      /* $5E07 LDA */ s_a = update_nz(peek(0x1940 + s_x));
      /* $5E0A LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $5E0B LDA */ s_a = update_nz(peek(0x1920 + s_x));
      /* $5E0E ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $5E0F TAY */ s_y = update_nz(s_a);
      /* $5E10 LDA */ s_a = update_nz(peek(0x0890 + s_y));
      /* $5E13 STA */ poke_zpg(0x06, s_a);
      /* $5E15 LDA */ s_a = update_nz(peek(0x0898 + s_y));
      /* $5E18 STA */ poke_zpg(0x07, s_a);
      /* $5E1A LDY */ s_y = update_nz(peek(0x1910 + s_x));
      /* $5E1D LDA */ s_a = update_nz(peek(0x1900 + s_x));
      /* $5E20 TAX */ s_x = update_nz(s_a);
      /* $5E21 JSR */ push16(0x5e23), s_pc = 0x4cf6;
      branchTarget = true;
      break;
    case 0x5e24: // [$5E24..$5E25]    2 bytes
      CYCLES(0x5e24, 4);
      /* $5E24 SEC */ s_status |= STATUS_C;
      /* $5E25 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5e26: // [$5E26..$5E39]   20 bytes
      CYCLES(0x5e26, 35);
      /* $5E26 LDY */ s_y = update_nz(peek_zpg(0x1f));
      /* $5E28 LDA */ s_a = update_nz(peek(0x1530 + s_y));
      /* $5E2B PHA */ push8(s_a);
      /* $5E2C AND */ s_a = update_nz(s_a & 0x03);
      /* $5E2E TAY */ s_y = update_nz(s_a);
      /* $5E2F LDA */ s_a = update_nz(peek(0x1900 + s_x));
      /* $5E32 CLC */ s_status &= ~STATUS_C;
      /* $5E33 ADC */ tmp = peek(0x0d00 + s_y), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5E36 CMP */ update_nz_inv_c(s_a - 0x06);
      /* $5E38 BCS */ s_pc = s_status & STATUS_C ? 0x5e3c : 0x5e3a;
      branchTarget = true;
      break;
    case 0x5e3a: // [$5E3A..$5E3B]    2 bytes
      CYCLES(0x5e3a, 4);
      /* $5E3A ADC */ tmp = 0x04, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0x5e3c: // [$5E3C..$5E3F]    4 bytes
      CYCLES(0x5e3c, 7);
      /* $5E3C CMP */ update_nz_inv_c(s_a - 0xed);
      /* $5E3E BCC */ s_pc = !(s_status & STATUS_C) ? 0x5e42 : 0x5e40;
      branchTarget = true;
      break;
    case 0x5e40: // [$5E40..$5E41]    2 bytes
      CYCLES(0x5e40, 4);
      /* $5E40 SBC */ tmp = 0x04, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
    case 0x5e42: // [$5E42..$5E53]   18 bytes
      CYCLES(0x5e42, 31);
      /* $5E42 STA */ poke(0x1900 + s_x, s_a);
      /* $5E45 PLA */ s_a = update_nz(pop8());
      /* $5E46 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $5E47 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $5E48 TAY */ s_y = update_nz(s_a);
      /* $5E49 LDA */ s_a = update_nz(peek(0x1910 + s_x));
      /* $5E4C CLC */ s_status &= ~STATUS_C;
      /* $5E4D ADC */ tmp = peek(0x0d00 + s_y), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5E50 CMP */ update_nz_inv_c(s_a - 0x06);
      /* $5E52 BCS */ s_pc = s_status & STATUS_C ? 0x5e56 : 0x5e54;
      branchTarget = true;
      break;
    case 0x5e54: // [$5E54..$5E55]    2 bytes
      CYCLES(0x5e54, 4);
      /* $5E54 ADC */ tmp = 0x04, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0x5e56: // [$5E56..$5E59]    4 bytes
      CYCLES(0x5e56, 7);
      /* $5E56 CMP */ update_nz_inv_c(s_a - 0xac);
      /* $5E58 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5e5c : 0x5e5a;
      branchTarget = true;
      break;
    case 0x5e5a: // [$5E5A..$5E5B]    2 bytes
      CYCLES(0x5e5a, 4);
      /* $5E5A SBC */ tmp = 0x04, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
    case 0x5e5c: // [$5E5C..$5E5F]    4 bytes
      CYCLES(0x5e5c, 7);
      /* $5E5C STA */ poke(0x1910 + s_x, s_a);
      /* $5E5F RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5e60: // [$5E60..$5E62]    3 bytes
      CYCLES(0x5e60, 6);
      /* $5E60 LDX */ s_x = update_nz(peek(0x140a));
    case 0x5e63: // [$5E63..$5E65]    3 bytes
      CYCLES(0x5e63, 6);
      /* $5E63 DEX */ s_x = update_nz(s_x - 1);
      /* $5E64 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5e69 : 0x5e66;
      branchTarget = true;
      break;
    case 0x5e66: // [$5E66..$5E68]    3 bytes
      CYCLES(0x5e66, 6);
      /* $5E66 JMP */ s_pc = 0x60b0;
      branchTarget = true;
      break;
    case 0x5e69: // [$5E69..$5E6D]    5 bytes
      CYCLES(0x5e69, 9);
      /* $5E69 DEC */ tmp16 = 0x19a0 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $5E6C BPL */ s_pc = !(s_status & STATUS_N) ? 0x5e63 : 0x5e6e;
      branchTarget = true;
      break;
    case 0x5e6e: // [$5E6E..$5E7A]   13 bytes
      CYCLES(0x5e6e, 23);
      /* $5E6E STX */ poke_zpg(0x00, s_x);
      /* $5E70 LDA */ s_a = update_nz(peek(0x0c08));
      /* $5E73 STA */ poke(0x19a0 + s_x, s_a);
      /* $5E76 DEC */ tmp16 = 0x19b0 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $5E79 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5e99 : 0x5e7b;
      branchTarget = true;
      break;
    case 0x5e7b: // [$5E7B..$5E7D]    3 bytes
      CYCLES(0x5e7b, 6);
      /* $5E7B JSR */ push16(0x5e7d), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x5e7e: // [$5E7E..$5E87]   10 bytes
      CYCLES(0x5e7e, 18);
      /* $5E7E AND */ s_a = update_nz(s_a & 0x01);
      /* $5E80 STA */ poke(0x1970 + s_x, s_a);
      /* $5E83 LDA */ s_a = update_nz(0x05);
      /* $5E85 JSR */ push16(0x5e87), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x5e88: // [$5E88..$5E95]   14 bytes
      CYCLES(0x5e88, 24);
      /* $5E88 SEC */ s_status |= STATUS_C;
      /* $5E89 SBC */ tmp = 0x02, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $5E8B LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $5E8D STA */ poke(0x1980 + s_y, s_a);
      /* $5E90 LDA */ s_a = update_nz(peek(0x0c0a));
      /* $5E93 JSR */ push16(0x5e95), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x5e96: // [$5E96..$5E98]    3 bytes
      CYCLES(0x5e96, 6);
      /* $5E96 STA */ poke(0x19b0 + s_y, s_a);
    case 0x5e99: // [$5E99..$5EA5]   13 bytes
      CYCLES(0x5e99, 23);
      /* $5E99 LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $5E9B LDX */ s_x = update_nz(peek(0x1960 + s_y));
      /* $5E9E LDA */ s_a = update_nz(peek(0x1950 + s_y));
      /* $5EA1 LDY */ s_y = update_nz(0x0a);
      /* $5EA3 JSR */ push16(0x5ea5), s_pc = 0x4cd0;
      branchTarget = true;
      break;
    case 0x5ea6: // [$5EA6..$5EA7]    2 bytes
      CYCLES(0x5ea6, 4);
      /* $5EA6 LDY */ s_y = update_nz(peek_zpg(0x00));
    case 0x5ea8: // [$5EA8..$5EB5]   14 bytes
      CYCLES(0x5ea8, 24);
      /* $5EA8 LDA */ s_a = update_nz(peek(0x1950 + s_y));
      /* $5EAB LDX */ s_x = update_nz(peek(0x1970 + s_y));
      /* $5EAE CLC */ s_status &= ~STATUS_C;
      /* $5EAF ADC */ tmp = peek(0x0d1c + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5EB2 CMP */ update_nz_inv_c(s_a - 0x04);
      /* $5EB4 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5eba : 0x5eb6;
      branchTarget = true;
      break;
    case 0x5eb6: // [$5EB6..$5EB9]    4 bytes
      CYCLES(0x5eb6, 7);
      /* $5EB6 CMP */ update_nz_inv_c(s_a - 0xf5);
      /* $5EB8 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5ec2 : 0x5eba;
      branchTarget = true;
      break;
    case 0x5eba: // [$5EBA..$5EC1]    8 bytes
      CYCLES(0x5eba, 14);
      /* $5EBA TXA */ s_a = update_nz(s_x);
      /* $5EBB EOR */ s_a = update_nz(s_a ^ 0x01);
      /* $5EBD STA */ poke(0x1970 + s_y, s_a);
      /* $5EC0 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5ea8 : 0x5ec2;
      branchTarget = true;
      break;
    case 0x5ec2: // [$5EC2..$5ECF]   14 bytes
      CYCLES(0x5ec2, 24);
      /* $5EC2 STA */ poke(0x1950 + s_y, s_a);
      /* $5EC5 LDA */ s_a = update_nz(peek(0x1960 + s_y));
      /* $5EC8 CLC */ s_status &= ~STATUS_C;
      /* $5EC9 ADC */ tmp = peek(0x1980 + s_y), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5ECC CMP */ update_nz_inv_c(s_a - 0x04);
      /* $5ECE BCC */ s_pc = !(s_status & STATUS_C) ? 0x5ed4 : 0x5ed0;
      branchTarget = true;
      break;
    case 0x5ed0: // [$5ED0..$5ED3]    4 bytes
      CYCLES(0x5ed0, 7);
      /* $5ED0 CMP */ update_nz_inv_c(s_a - 0xb2);
      /* $5ED2 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5edc : 0x5ed4;
      branchTarget = true;
      break;
    case 0x5ed4: // [$5ED4..$5EDB]    8 bytes
      CYCLES(0x5ed4, 14);
      /* $5ED4 LDA */ s_a = update_nz(0x00);
      /* $5ED6 STA */ poke(0x1980 + s_y, s_a);
      /* $5ED9 LDA */ s_a = update_nz(peek(0x1960 + s_y));
    case 0x5edc: // [$5EDC..$5F04]   41 bytes
      CYCLES(0x5edc, 70);
      /* $5EDC STA */ poke(0x1960 + s_y, s_a);
      /* $5EDF LDA */ s_a = update_nz(peek(0x1990 + s_y));
      /* $5EE2 ASL */ s_a = update_nzc(s_a << 1);
      /* $5EE3 ORA */ s_a = update_nz(s_a | peek(0x1970 + s_y));
      /* $5EE6 ASL */ s_a = update_nzc(s_a << 1);
      /* $5EE7 STA */ poke_zpg(0x06, s_a);
      /* $5EE9 LDA */ s_a = update_nz(peek(0x19b0 + s_y));
      /* $5EEC AND */ s_a = update_nz(s_a & 0x01);
      /* $5EEE ORA */ s_a = update_nz(s_a | peek_zpg(0x06));
      /* $5EF0 TAX */ s_x = update_nz(s_a);
      /* $5EF1 LDA */ s_a = update_nz(peek(0x08a0 + s_x));
      /* $5EF4 STA */ poke_zpg(0x06, s_a);
      /* $5EF6 LDA */ s_a = update_nz(peek(0x08b0 + s_x));
      /* $5EF9 STA */ poke_zpg(0x07, s_a);
      /* $5EFB LDX */ s_x = update_nz(peek(0x1950 + s_y));
      /* $5EFE LDA */ s_a = update_nz(peek(0x1960 + s_y));
      /* $5F01 TAY */ s_y = update_nz(s_a);
      /* $5F02 JSR */ push16(0x5f04), s_pc = 0x4cac;
      branchTarget = true;
      break;
    case 0x5f05: // [$5F05..$5F09]    5 bytes
      CYCLES(0x5f05, 9);
      /* $5F05 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $5F07 JMP */ s_pc = 0x5e63;
      branchTarget = true;
      break;
    case 0x5f0a: // [$5F0A..$5F1A]   17 bytes
      CYCLES(0x5f0a, 29);
      /* $5F0A LDA */ s_a = update_nz(peek(0x1960 + s_x));
      /* $5F0D CLC */ s_status &= ~STATUS_C;
      /* $5F0E ADC */ tmp = 0x02, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5F10 PHA */ push8(s_a);
      /* $5F11 LDA */ s_a = update_nz(peek(0x1950 + s_x));
      /* $5F14 SEC */ s_status |= STATUS_C;
      /* $5F15 SBC */ tmp = 0x04, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $5F17 PHA */ push8(s_a);
      /* $5F18 JSR */ push16(0x5f1a), s_pc = 0x5f52;
      branchTarget = true;
      break;
    case 0x5f1b: // [$5F1B..$5F22]    8 bytes
      CYCLES(0x5f1b, 14);
      /* $5F1B LDA */ s_a = update_nz(peek(0x140d));
      /* $5F1E LDX */ s_x = update_nz(0x0a);
      /* $5F20 JSR */ push16(0x5f22), s_pc = 0x4c00;
      branchTarget = true;
      break;
    case 0x5f23: // [$5F23..$5F30]   14 bytes
      CYCLES(0x5f23, 24);
      /* $5F23 SEC */ s_status |= STATUS_C;
      /* $5F24 SBC */ tmp = 0x01, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $5F26 STA */ poke_zpg(0x06, s_a);
      /* $5F28 PLA */ s_a = update_nz(pop8());
      /* $5F29 TAX */ s_x = update_nz(s_a);
      /* $5F2A PLA */ s_a = update_nz(pop8());
      /* $5F2B TAY */ s_y = update_nz(s_a);
      /* $5F2C LDA */ s_a = update_nz(peek_zpg(0x06));
      /* $5F2E JSR */ push16(0x5f30), s_pc = 0x61b9;
      branchTarget = true;
      break;
    case 0x5f31: // [$5F31..$5F38]    8 bytes
      CYCLES(0x5f31, 14);
      /* $5F31 LDA */ s_a = update_nz(peek(0x140d));
      /* $5F34 LDX */ s_x = update_nz(0x64);
      /* $5F36 JSR */ push16(0x5f38), s_pc = 0x4c4f;
      branchTarget = true;
      break;
    case 0x5f39: // [$5F39..$5F3B]    3 bytes
      CYCLES(0x5f39, 6);
      /* $5F39 JSR */ push16(0x5f3b), s_pc = 0x4dd0;
      branchTarget = true;
      break;
    case 0x5f3c: // [$5F3C..$5F45]   10 bytes
      CYCLES(0x5f3c, 18);
      /* $5F3C LDA */ s_a = update_nz(peek(0x140d));
      /* $5F3F CLC */ s_status &= ~STATUS_C;
      /* $5F40 ADC */ tmp = 0x0a, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5F42 CMP */ update_nz_inv_c(s_a - 0x33);
      /* $5F44 BCS */ s_pc = s_status & STATUS_C ? 0x5f49 : 0x5f46;
      branchTarget = true;
      break;
    case 0x5f46: // [$5F46..$5F48]    3 bytes
      CYCLES(0x5f46, 6);
      /* $5F46 STA */ poke(0x140d, s_a);
    case 0x5f49: // [$5F49..$5F51]    9 bytes
      CYCLES(0x5f49, 16);
      /* $5F49 LDA */ s_a = update_nz(peek(0x0c68));
      /* $5F4C STA */ poke(0x140b, s_a);
      /* $5F4F JMP */ s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x5f52: // [$5F52..$5F56]    5 bytes
      CYCLES(0x5f52, 9);
      /* $5F52 TXA */ s_a = update_nz(s_x);
      /* $5F53 PHA */ push8(s_a);
      /* $5F54 DEC */ tmp16 = 0x140a, poke(tmp16, update_nz(peek(tmp16) - 1));
    case 0x5f57: // [$5F57..$5F86]   48 bytes
      CYCLES(0x5f57, 82);
      /* $5F57 LDA */ s_a = update_nz(peek(0x1951 + s_x));
      /* $5F5A STA */ poke(0x1950 + s_x, s_a);
      /* $5F5D LDA */ s_a = update_nz(peek(0x1961 + s_x));
      /* $5F60 STA */ poke(0x1960 + s_x, s_a);
      /* $5F63 LDA */ s_a = update_nz(peek(0x1971 + s_x));
      /* $5F66 STA */ poke(0x1970 + s_x, s_a);
      /* $5F69 LDA */ s_a = update_nz(peek(0x1981 + s_x));
      /* $5F6C STA */ poke(0x1980 + s_x, s_a);
      /* $5F6F LDA */ s_a = update_nz(peek(0x1991 + s_x));
      /* $5F72 STA */ poke(0x1990 + s_x, s_a);
      /* $5F75 LDA */ s_a = update_nz(peek(0x19a1 + s_x));
      /* $5F78 STA */ poke(0x19a0 + s_x, s_a);
      /* $5F7B LDA */ s_a = update_nz(peek(0x19b1 + s_x));
      /* $5F7E STA */ poke(0x19b0 + s_x, s_a);
      /* $5F81 INX */ s_x = update_nz(s_x + 1);
      /* $5F82 CPX */ update_nz_inv_c(s_x - peek(0x140a));
      /* $5F85 BCC */ s_pc = !(s_status & STATUS_C) ? 0x5f57 : 0x5f87;
      branchTarget = true;
      break;
    case 0x5f87: // [$5F87..$5F89]    3 bytes
      CYCLES(0x5f87, 6);
      /* $5F87 PLA */ s_a = update_nz(pop8());
      /* $5F88 TAX */ s_x = update_nz(s_a);
      /* $5F89 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5f8a: // [$5F8A..$5F92]    9 bytes
      CYCLES(0x5f8a, 16);
      /* $5F8A LDX */ s_x = update_nz(peek(0x1405));
      /* $5F8D LDA */ s_a = update_nz(0x00);
      /* $5F8F STA */ poke_zpg(0xfc, s_a);
      /* $5F91 STA */ poke_zpg(0xfe, s_a);
    case 0x5f93: // [$5F93..$5F95]    3 bytes
      CYCLES(0x5f93, 6);
      /* $5F93 DEX */ s_x = update_nz(s_x - 1);
      /* $5F94 BPL */ s_pc = !(s_status & STATUS_N) ? 0x5f97 : 0x5f96;
      branchTarget = true;
      break;
    case 0x5f96: // [$5F96..$5F96]    1 bytes
      CYCLES(0x5f96, 2);
      /* $5F96 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5f97: // [$5F97..$5FB7]   33 bytes
      CYCLES(0x5f97, 57);
      /* $5F97 LDY */ s_y = update_nz(peek(0x1770 + s_x));
      /* $5F9A LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $5F9C STA */ poke_zpg(0x06, s_a);
      /* $5F9E LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $5FA0 STA */ poke_zpg(0x07, s_a);
      /* $5FA2 LDY */ s_y = update_nz(peek(0x17c0 + s_x));
      /* $5FA5 LDA */ s_a = update_nz(0x00);
      /* $5FA7 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $5FA9 LDA */ s_a = update_nz(peek(0x1770 + s_x));
      /* $5FAC CLC */ s_status &= ~STATUS_C;
      /* $5FAD ADC */ tmp = peek(0x1860 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $5FB0 STA */ poke(0x1770 + s_x, s_a);
      /* $5FB3 DEC */ tmp16 = 0x18b0 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $5FB6 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5f93 : 0x5fb8;
      branchTarget = true;
      break;
    case 0x5fb8: // [$5FB8..$5FBC]    5 bytes
      CYCLES(0x5fb8, 9);
      /* $5FB8 TXA */ s_a = update_nz(s_x);
      /* $5FB9 TAY */ s_y = update_nz(s_a);
      /* $5FBA DEC */ tmp16 = 0x1405, poke(tmp16, update_nz(peek(tmp16) - 1));
    case 0x5fbd: // [$5FBD..$5FE0]   36 bytes
      CYCLES(0x5fbd, 62);
      /* $5FBD LDA */ s_a = update_nz(peek(0x1771 + s_y));
      /* $5FC0 STA */ poke(0x1770 + s_y, s_a);
      /* $5FC3 LDA */ s_a = update_nz(peek(0x17c1 + s_y));
      /* $5FC6 STA */ poke(0x17c0 + s_y, s_a);
      /* $5FC9 LDA */ s_a = update_nz(peek(0x1811 + s_y));
      /* $5FCC STA */ poke(0x1810 + s_y, s_a);
      /* $5FCF LDA */ s_a = update_nz(peek(0x1861 + s_y));
      /* $5FD2 STA */ poke(0x1860 + s_y, s_a);
      /* $5FD5 LDA */ s_a = update_nz(peek(0x18b1 + s_y));
      /* $5FD8 STA */ poke(0x18b0 + s_y, s_a);
      /* $5FDB INY */ s_y = update_nz(s_y + 1);
      /* $5FDC CPY */ update_nz_inv_c(s_y - peek(0x1405));
      /* $5FDF BCC */ s_pc = !(s_status & STATUS_C) ? 0x5fbd : 0x5fe1;
      branchTarget = true;
      break;
    case 0x5fe1: // [$5FE1..$5FE2]    2 bytes
      CYCLES(0x5fe1, 4);
      /* $5FE1 BCS */ s_pc = s_status & STATUS_C ? 0x5f93 : 0x5fe3;
      branchTarget = true;
      break;
    case 0x5fe3: // [$5FE3..$5FEB]    9 bytes
      CYCLES(0x5fe3, 16);
      /* $5FE3 LDX */ s_x = update_nz(peek(0x1405));
      /* $5FE6 LDA */ s_a = update_nz(0x00);
      /* $5FE8 STA */ poke_zpg(0xfc, s_a);
      /* $5FEA STA */ poke_zpg(0xfe, s_a);
    case 0x5fec: // [$5FEC..$5FEE]    3 bytes
      CYCLES(0x5fec, 6);
      /* $5FEC DEX */ s_x = update_nz(s_x - 1);
      /* $5FED BPL */ s_pc = !(s_status & STATUS_N) ? 0x5ff0 : 0x5fef;
      branchTarget = true;
      break;
    case 0x5fef: // [$5FEF..$5FEF]    1 bytes
      CYCLES(0x5fef, 2);
      /* $5FEF RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x5ff0: // [$5FF0..$6004]   21 bytes
      CYCLES(0x5ff0, 36);
      /* $5FF0 LDY */ s_y = update_nz(peek(0x1770 + s_x));
      /* $5FF3 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $5FF5 STA */ poke_zpg(0x06, s_a);
      /* $5FF7 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $5FF9 STA */ poke_zpg(0x07, s_a);
      /* $5FFB LDY */ s_y = update_nz(peek(0x17c0 + s_x));
      /* $5FFE LDA */ s_a = update_nz(peek(0x1810 + s_x));
      /* $6001 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $6003 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x5fec : 0x6005;
      branchTarget = true;
      break;
    case 0x6005: // [$6005..$6007]    3 bytes
      CYCLES(0x6005, 6);
      /* $6005 JMP */ s_pc = 0x5fec;
      branchTarget = true;
      break;
    case 0x6008: // [$6008..$600F]    8 bytes
      CYCLES(0x6008, 14);
      /* $6008 LDY */ s_y = update_nz(peek(0x1408));
      /* $600B CPY */ update_nz_inv_c(s_y - peek(0x1402));
      /* $600E BCC */ s_pc = !(s_status & STATUS_C) ? 0x6011 : 0x6010;
      branchTarget = true;
      break;
    case 0x6010: // [$6010..$6010]    1 bytes
      CYCLES(0x6010, 2);
      /* $6010 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6011: // [$6011..$6028]   24 bytes
      CYCLES(0x6011, 41);
      /* $6011 TYA */ s_a = update_nz(s_y);
      /* $6012 CLC */ s_status &= ~STATUS_C;
      /* $6013 ADC */ tmp = 0x04, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6015 STA */ poke(0x1408, s_a);
      /* $6018 LDA */ s_a = update_nz(peek(0x15f0 + s_y));
      /* $601B CLC */ s_status &= ~STATUS_C;
      /* $601C ADC */ tmp = 0x07, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $601E STA */ poke_zpg(0x05, s_a);
      /* $6020 LDA */ s_a = update_nz(peek(0x1570 + s_y));
      /* $6023 CLC */ s_status &= ~STATUS_C;
      /* $6024 ADC */ tmp = 0x03, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6026 JSR */ push16(0x6028), s_pc = 0x4c1c;
      branchTarget = true;
      break;
    case 0x6029: // [$6029..$6039]   17 bytes
      CYCLES(0x6029, 29);
      /* $6029 STA */ poke_zpg(0x04, s_a);
      /* $602B LDY */ s_y = update_nz(peek(0x0c0b));
      /* $602E DEY */ s_y = update_nz(s_y - 1);
      /* $602F LDA */ s_a = update_nz(peek(0x1405));
      /* $6032 TAX */ s_x = update_nz(s_a);
      /* $6033 CLC */ s_status &= ~STATUS_C;
      /* $6034 ADC */ tmp = peek(0x0c0b), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6037 STA */ poke(0x1405, s_a);
    case 0x603a: // [$603A..$6049]   16 bytes
      CYCLES(0x603a, 28);
      /* $603A LDA */ s_a = update_nz(peek(0x0c07));
      /* $603D STA */ poke(0x18b0 + s_x, s_a);
      /* $6040 LDA */ s_a = update_nz(peek_zpg(0x04));
      /* $6042 STA */ poke(0x17c0 + s_x, s_a);
      /* $6045 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $6046 LDA */ s_a = update_nz(0xaa);
      /* $6048 BCC */ s_pc = !(s_status & STATUS_C) ? 0x604c : 0x604a;
      branchTarget = true;
      break;
    case 0x604a: // [$604A..$604B]    2 bytes
      CYCLES(0x604a, 4);
      /* $604A LDA */ s_a = update_nz(0xd5);
    case 0x604c: // [$604C..$605F]   20 bytes
      CYCLES(0x604c, 35);
      /* $604C AND */ s_a = update_nz(s_a & peek(0x0d0c + s_y));
      /* $604F STA */ poke(0x1810 + s_x, s_a);
      /* $6052 LDA */ s_a = update_nz(peek(0x0d04 + s_y));
      /* $6055 STA */ poke(0x1860 + s_x, s_a);
      /* $6058 STX */ poke_zpg(0x00, s_x);
      /* $605A LDX */ s_x = update_nz(peek(0x0c07));
      /* $605D JSR */ push16(0x605f), s_pc = 0x4c4f;
      branchTarget = true;
      break;
    case 0x6060: // [$6060..$606F]   16 bytes
      CYCLES(0x6060, 28);
      /* $6060 STX */ poke_zpg(0x02, s_x);
      /* $6062 LDA */ s_a = update_nz(peek_zpg(0x05));
      /* $6064 SEC */ s_status |= STATUS_C;
      /* $6065 SBC */ tmp = peek_zpg(0x02), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6067 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $6069 STA */ poke(0x1770 + s_x, s_a);
      /* $606C INX */ s_x = update_nz(s_x + 1);
      /* $606D DEY */ s_y = update_nz(s_y - 1);
      /* $606E BPL */ s_pc = !(s_status & STATUS_N) ? 0x603a : 0x6070;
      branchTarget = true;
      break;
    case 0x6070: // [$6070..$6072]    3 bytes
      CYCLES(0x6070, 6);
      /* $6070 JMP */ s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x6073: // [$6073..$6075]    3 bytes
      CYCLES(0x6073, 6);
      /* $6073 LDX */ s_x = update_nz(peek(0x140a));
    case 0x6076: // [$6076..$6077]    2 bytes
      CYCLES(0x6076, 4);
      /* $6076 LDA */ s_a = update_nz(peek_zpg(0x1c));
    case 0x6078: // [$6078..$607A]    3 bytes
      CYCLES(0x6078, 6);
      /* $6078 DEX */ s_x = update_nz(s_x - 1);
      /* $6079 BPL */ s_pc = !(s_status & STATUS_N) ? 0x607d : 0x607b;
      branchTarget = true;
      break;
    case 0x607b: // [$607B..$607C]    2 bytes
      CYCLES(0x607b, 4);
      /* $607B CLC */ s_status &= ~STATUS_C;
      /* $607C RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x607d: // [$607D..$6081]    5 bytes
      CYCLES(0x607d, 9);
      /* $607D CMP */ update_nz_inv_c(s_a - peek(0x1950 + s_x));
      /* $6080 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6078 : 0x6082;
      branchTarget = true;
      break;
    case 0x6082: // [$6082..$6085]    4 bytes
      CYCLES(0x6082, 7);
      /* $6082 SBC */ tmp = 0x08, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6084 BCC */ s_pc = !(s_status & STATUS_C) ? 0x608b : 0x6086;
      branchTarget = true;
      break;
    case 0x6086: // [$6086..$608A]    5 bytes
      CYCLES(0x6086, 9);
      /* $6086 CMP */ update_nz_inv_c(s_a - peek(0x1950 + s_x));
      /* $6089 BCS */ s_pc = s_status & STATUS_C ? 0x6076 : 0x608b;
      branchTarget = true;
      break;
    case 0x608b: // [$608B..$6091]    7 bytes
      CYCLES(0x608b, 12);
      /* $608B LDA */ s_a = update_nz(peek_zpg(0x1d));
      /* $608D CMP */ update_nz_inv_c(s_a - peek(0x1960 + s_x));
      /* $6090 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6076 : 0x6092;
      branchTarget = true;
      break;
    case 0x6092: // [$6092..$6095]    4 bytes
      CYCLES(0x6092, 7);
      /* $6092 SBC */ tmp = 0x0b, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6094 BCC */ s_pc = !(s_status & STATUS_C) ? 0x609b : 0x6096;
      branchTarget = true;
      break;
    case 0x6096: // [$6096..$609A]    5 bytes
      CYCLES(0x6096, 9);
      /* $6096 CMP */ update_nz_inv_c(s_a - peek(0x1960 + s_x));
      /* $6099 BCS */ s_pc = s_status & STATUS_C ? 0x6076 : 0x609b;
      branchTarget = true;
      break;
    case 0x609b: // [$609B..$60A8]   14 bytes
      CYCLES(0x609b, 24);
      /* $609B TXA */ s_a = update_nz(s_x);
      /* $609C PHA */ push8(s_a);
      /* $609D TAY */ s_y = update_nz(s_a);
      /* $609E LDX */ s_x = update_nz(peek(0x1960 + s_y));
      /* $60A1 LDA */ s_a = update_nz(peek(0x1950 + s_y));
      /* $60A4 LDY */ s_y = update_nz(0x0a);
      /* $60A6 JSR */ push16(0x60a8), s_pc = 0x4cd0;
      branchTarget = true;
      break;
    case 0x60a9: // [$60A9..$60AD]    5 bytes
      CYCLES(0x60a9, 9);
      /* $60A9 PLA */ s_a = update_nz(pop8());
      /* $60AA TAX */ s_x = update_nz(s_a);
      /* $60AB JSR */ push16(0x60ad), s_pc = 0x5f0a;
      branchTarget = true;
      break;
    case 0x60ae: // [$60AE..$60AF]    2 bytes
      CYCLES(0x60ae, 4);
      /* $60AE SEC */ s_status |= STATUS_C;
      /* $60AF RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x60b0: // [$60B0..$60B2]    3 bytes
      CYCLES(0x60b0, 6);
      /* $60B0 LDX */ s_x = update_nz(peek(0x142c));
    case 0x60b3: // [$60B3..$60B5]    3 bytes
      CYCLES(0x60b3, 6);
      /* $60B3 DEX */ s_x = update_nz(s_x - 1);
      /* $60B4 BPL */ s_pc = !(s_status & STATUS_N) ? 0x60b9 : 0x60b6;
      branchTarget = true;
      break;
    case 0x60b6: // [$60B6..$60B8]    3 bytes
      CYCLES(0x60b6, 6);
      /* $60B6 JMP */ s_pc = 0x6150;
      branchTarget = true;
      break;
    case 0x60b9: // [$60B9..$60BD]    5 bytes
      CYCLES(0x60b9, 9);
      /* $60B9 DEC */ tmp16 = 0x1ed0 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $60BC BPL */ s_pc = !(s_status & STATUS_N) ? 0x60b3 : 0x60be;
      branchTarget = true;
      break;
    case 0x60be: // [$60BE..$60DC]   31 bytes
      CYCLES(0x60be, 53);
      /* $60BE LDA */ s_a = update_nz(peek(0x0c5e));
      /* $60C1 STA */ poke(0x1ed0 + s_x, s_a);
      /* $60C4 STX */ poke_zpg(0x00, s_x);
      /* $60C6 LDY */ s_y = update_nz(peek(0x1ef0 + s_x));
      /* $60C9 LDA */ s_a = update_nz(peek(0x0940 + s_y));
      /* $60CC STA */ poke_zpg(0x06, s_a);
      /* $60CE LDA */ s_a = update_nz(peek(0x0948 + s_y));
      /* $60D1 STA */ poke_zpg(0x07, s_a);
      /* $60D3 LDY */ s_y = update_nz(peek(0x1ec0 + s_x));
      /* $60D6 LDA */ s_a = update_nz(peek(0x1eb0 + s_x));
      /* $60D9 TAX */ s_x = update_nz(s_a);
      /* $60DA JSR */ push16(0x60dc), s_pc = 0x4cf6;
      branchTarget = true;
      break;
    case 0x60dd: // [$60DD..$60E3]    7 bytes
      CYCLES(0x60dd, 12);
      /* $60DD LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $60DF DEC */ tmp16 = 0x1ee0 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $60E2 BPL */ s_pc = !(s_status & STATUS_N) ? 0x60b3 : 0x60e4;
      branchTarget = true;
      break;
    case 0x60e4: // [$60E4..$60F4]   17 bytes
      CYCLES(0x60e4, 29);
      /* $60E4 LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $60E6 LDX */ s_x = update_nz(peek(0x1ec0 + s_y));
      /* $60E9 LDA */ s_a = update_nz(peek(0x1ef0 + s_y));
      /* $60EC CMP */ update_nz_inv_c(s_a - 0x05);
      /* $60EE LDA */ s_a = update_nz(peek(0x1eb0 + s_y));
      /* $60F1 LDY */ s_y = update_nz(0x05);
      /* $60F3 BCC */ s_pc = !(s_status & STATUS_C) ? 0x60f7 : 0x60f5;
      branchTarget = true;
      break;
    case 0x60f5: // [$60F5..$60F6]    2 bytes
      CYCLES(0x60f5, 4);
      /* $60F5 LDY */ s_y = update_nz(0x0d);
    case 0x60f7: // [$60F7..$60F9]    3 bytes
      CYCLES(0x60f7, 6);
      /* $60F7 JSR */ push16(0x60f9), s_pc = 0x4d21;
      branchTarget = true;
      break;
    case 0x60fa: // [$60FA..$60FE]    5 bytes
      CYCLES(0x60fa, 9);
      /* $60FA LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $60FC DEC */ tmp16 = 0x142c, poke(tmp16, update_nz(peek(tmp16) - 1));
    case 0x60ff: // [$60FF..$6122]   36 bytes
      CYCLES(0x60ff, 62);
      /* $60FF LDA */ s_a = update_nz(peek(0x1eb1 + s_x));
      /* $6102 STA */ poke(0x1eb0 + s_x, s_a);
      /* $6105 LDA */ s_a = update_nz(peek(0x1ec1 + s_x));
      /* $6108 STA */ poke(0x1ec0 + s_x, s_a);
      /* $610B LDA */ s_a = update_nz(peek(0x1ed1 + s_x));
      /* $610E STA */ poke(0x1ed0 + s_x, s_a);
      /* $6111 LDA */ s_a = update_nz(peek(0x1ee1 + s_x));
      /* $6114 STA */ poke(0x1ee0 + s_x, s_a);
      /* $6117 LDA */ s_a = update_nz(peek(0x1ef1 + s_x));
      /* $611A STA */ poke(0x1ef0 + s_x, s_a);
      /* $611D INX */ s_x = update_nz(s_x + 1);
      /* $611E CPX */ update_nz_inv_c(s_x - peek(0x142c));
      /* $6121 BCC */ s_pc = !(s_status & STATUS_C) ? 0x60ff : 0x6123;
      branchTarget = true;
      break;
    case 0x6123: // [$6123..$6126]    4 bytes
      CYCLES(0x6123, 7);
      /* $6123 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $6125 BPL */ s_pc = !(s_status & STATUS_N) ? 0x60b3 : 0x6127;
      branchTarget = true;
      break;
    case 0x6127: // [$6127..$6132]   12 bytes
      CYCLES(0x6127, 21);
      /* $6127 DEY */ s_y = update_nz(s_y - 1);
      /* $6128 TYA */ s_a = update_nz(s_y);
      /* $6129 LDY */ s_y = update_nz(peek(0x142d));
      /* $612C STA */ poke(0x1f10 + s_y, s_a);
      /* $612F TXA */ s_a = update_nz(s_x);
      /* $6130 JSR */ push16(0x6132), s_pc = 0x4c1c;
      branchTarget = true;
      break;
    case 0x6133: // [$6133..$6137]    5 bytes
      CYCLES(0x6133, 9);
      /* $6133 LDX */ s_x = update_nz(0x07);
      /* $6135 JSR */ push16(0x6137), s_pc = 0x4c4f;
      branchTarget = true;
      break;
    case 0x6138: // [$6138..$614F]   24 bytes
      CYCLES(0x6138, 41);
      /* $6138 TXA */ s_a = update_nz(s_x);
      /* $6139 STA */ poke(0x1f00 + s_y, s_a);
      /* $613C LDA */ s_a = update_nz(0x00);
      /* $613E STA */ poke(0x1f20 + s_y, s_a);
      /* $6141 LDA */ s_a = update_nz(peek(0x0c61));
      /* $6144 STA */ poke(0x1f30 + s_y, s_a);
      /* $6147 INC */ tmp16 = 0x142d, poke(tmp16, update_nz(peek(tmp16) + 1));
      /* $614A LDA */ s_a = update_nz(0xff);
      /* $614C STA */ poke(0x140c, s_a);
      /* $614F RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6150: // [$6150..$6152]    3 bytes
      CYCLES(0x6150, 6);
      /* $6150 LDX */ s_x = update_nz(peek(0x142d));
    case 0x6153: // [$6153..$6155]    3 bytes
      CYCLES(0x6153, 6);
      /* $6153 DEX */ s_x = update_nz(s_x - 1);
      /* $6154 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6157 : 0x6156;
      branchTarget = true;
      break;
    case 0x6156: // [$6156..$6156]    1 bytes
      CYCLES(0x6156, 2);
      /* $6156 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6157: // [$6157..$615B]    5 bytes
      CYCLES(0x6157, 9);
      /* $6157 DEC */ tmp16 = 0x1f20 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $615A BPL */ s_pc = !(s_status & STATUS_N) ? 0x6153 : 0x615c;
      branchTarget = true;
      break;
    case 0x615c: // [$615C..$617D]   34 bytes
      CYCLES(0x615c, 58);
      /* $615C STX */ poke_zpg(0x00, s_x);
      /* $615E LDA */ s_a = update_nz(peek(0x0c60));
      /* $6161 STA */ poke(0x1f20 + s_x, s_a);
      /* $6164 LDA */ s_a = update_nz(peek(0x1f30 + s_x));
      /* $6167 AND */ s_a = update_nz(s_a & 0x03);
      /* $6169 TAY */ s_y = update_nz(s_a);
      /* $616A LDA */ s_a = update_nz(peek(0x0950 + s_y));
      /* $616D STA */ poke_zpg(0x06, s_a);
      /* $616F LDA */ s_a = update_nz(peek(0x0958 + s_y));
      /* $6172 STA */ poke_zpg(0x07, s_a);
      /* $6174 LDY */ s_y = update_nz(peek(0x1f10 + s_x));
      /* $6177 LDA */ s_a = update_nz(peek(0x1f00 + s_x));
      /* $617A TAX */ s_x = update_nz(s_a);
      /* $617B JSR */ push16(0x617d), s_pc = 0x4cac;
      branchTarget = true;
      break;
    case 0x617e: // [$617E..$6184]    7 bytes
      CYCLES(0x617e, 12);
      /* $617E LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $6180 DEC */ tmp16 = 0x1f30 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $6183 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6153 : 0x6185;
      branchTarget = true;
      break;
    case 0x6185: // [$6185..$6191]   13 bytes
      CYCLES(0x6185, 23);
      /* $6185 LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $6187 LDX */ s_x = update_nz(peek(0x1f10 + s_y));
      /* $618A LDA */ s_a = update_nz(peek(0x1f00 + s_y));
      /* $618D LDY */ s_y = update_nz(0x0c);
      /* $618F JSR */ push16(0x6191), s_pc = 0x4cd0;
      branchTarget = true;
      break;
    case 0x6192: // [$6192..$6196]    5 bytes
      CYCLES(0x6192, 9);
      /* $6192 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $6194 DEC */ tmp16 = 0x142d, poke(tmp16, update_nz(peek(tmp16) - 1));
    case 0x6197: // [$6197..$61B4]   30 bytes
      CYCLES(0x6197, 52);
      /* $6197 LDA */ s_a = update_nz(peek(0x1f01 + s_x));
      /* $619A STA */ poke(0x1f00 + s_x, s_a);
      /* $619D LDA */ s_a = update_nz(peek(0x1f11 + s_x));
      /* $61A0 STA */ poke(0x1f10 + s_x, s_a);
      /* $61A3 LDA */ s_a = update_nz(peek(0x1f21 + s_x));
      /* $61A6 STA */ poke(0x1f20 + s_x, s_a);
      /* $61A9 LDA */ s_a = update_nz(peek(0x1f31 + s_x));
      /* $61AC STA */ poke(0x1f30 + s_x, s_a);
      /* $61AF INX */ s_x = update_nz(s_x + 1);
      /* $61B0 CPX */ update_nz_inv_c(s_x - peek(0x142d));
      /* $61B3 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6197 : 0x61b5;
      branchTarget = true;
      break;
    case 0x61b5: // [$61B5..$61B8]    4 bytes
      CYCLES(0x61b5, 7);
      /* $61B5 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $61B7 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6153 : 0x61b9;
      branchTarget = true;
      break;
    case 0x61b9: // [$61B9..$61BD]    5 bytes
      CYCLES(0x61b9, 9);
      /* $61B9 PHA */ push8(s_a);
      /* $61BA TXA */ s_a = update_nz(s_x);
      /* $61BB JSR */ push16(0x61bd), s_pc = 0x4c1c;
      branchTarget = true;
      break;
    case 0x61be: // [$61BE..$61C2]    5 bytes
      CYCLES(0x61be, 9);
      /* $61BE LDX */ s_x = update_nz(0x07);
      /* $61C0 JSR */ push16(0x61c2), s_pc = 0x4c4f;
      branchTarget = true;
      break;
    case 0x61c3: // [$61C3..$61DD]   27 bytes
      CYCLES(0x61c3, 46);
      /* $61C3 TXA */ s_a = update_nz(s_x);
      /* $61C4 LDX */ s_x = update_nz(peek(0x142c));
      /* $61C7 STA */ poke(0x1eb0 + s_x, s_a);
      /* $61CA TYA */ s_a = update_nz(s_y);
      /* $61CB STA */ poke(0x1ec0 + s_x, s_a);
      /* $61CE PLA */ s_a = update_nz(pop8());
      /* $61CF STA */ poke(0x1ef0 + s_x, s_a);
      /* $61D2 CMP */ update_nz_inv_c(s_a - 0x05);
      /* $61D4 LDA */ s_a = update_nz(0x00);
      /* $61D6 STA */ poke(0x1ed0 + s_x, s_a);
      /* $61D9 LDA */ s_a = update_nz(peek(0x0c5f));
      /* $61DC BCC */ s_pc = !(s_status & STATUS_C) ? 0x61e0 : 0x61de;
      branchTarget = true;
      break;
    case 0x61de: // [$61DE..$61DF]    2 bytes
      CYCLES(0x61de, 4);
      /* $61DE LDA */ s_a = update_nz(0x01);
    case 0x61e0: // [$61E0..$61E6]    7 bytes
      CYCLES(0x61e0, 12);
      /* $61E0 STA */ poke(0x1ee0 + s_x, s_a);
      /* $61E3 INC */ tmp16 = 0x142c, poke(tmp16, update_nz(peek(tmp16) + 1));
      /* $61E6 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6200: // [$6200..$6202]    3 bytes
      CYCLES(0x6200, 6);
      /* $6200 JMP */ s_pc = 0x621b;
      branchTarget = true;
      break;
    case 0x6203: // [$6203..$6205]    3 bytes
      CYCLES(0x6203, 6);
      /* $6203 JMP */ s_pc = 0x62be;
      branchTarget = true;
      break;
    case 0x6206: // [$6206..$6208]    3 bytes
      CYCLES(0x6206, 6);
      /* $6206 JMP */ s_pc = 0x6436;
      branchTarget = true;
      break;
    case 0x6209: // [$6209..$620B]    3 bytes
      CYCLES(0x6209, 6);
      /* $6209 JMP */ s_pc = 0x6523;
      branchTarget = true;
      break;
    case 0x620c: // [$620C..$620E]    3 bytes
      CYCLES(0x620c, 6);
      /* $620C JMP */ s_pc = 0x669f;
      branchTarget = true;
      break;
    case 0x620f: // [$620F..$6211]    3 bytes
      CYCLES(0x620f, 6);
      /* $620F JMP */ s_pc = 0x670e;
      branchTarget = true;
      break;
    case 0x6212: // [$6212..$6214]    3 bytes
      CYCLES(0x6212, 6);
      /* $6212 JMP */ s_pc = 0x6643;
      branchTarget = true;
      break;
    case 0x6215: // [$6215..$6217]    3 bytes
      CYCLES(0x6215, 6);
      /* $6215 JMP */ s_pc = 0x684e;
      branchTarget = true;
      break;
    case 0x6218: // [$6218..$621A]    3 bytes
      CYCLES(0x6218, 6);
      /* $6218 JMP */ s_pc = 0x686a;
      branchTarget = true;
      break;
    case 0x621b: // [$621B..$6220]    6 bytes
      CYCLES(0x621b, 11);
      /* $621B LDX */ s_x = update_nz(peek(0x140e));
      /* $621E LDA */ s_a = update_nz(0x00);
      /* $6220 CLC */ s_status &= ~STATUS_C;
    case 0x6221: // [$6221..$6223]    3 bytes
      CYCLES(0x6221, 6);
      /* $6221 DEX */ s_x = update_nz(s_x - 1);
      /* $6222 BMI */ s_pc = s_status & STATUS_N ? 0x6229 : 0x6224;
      branchTarget = true;
      break;
    case 0x6224: // [$6224..$6228]    5 bytes
      CYCLES(0x6224, 9);
      /* $6224 ADC */ tmp = peek(0x1a30 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6227 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6221 : 0x6229;
      branchTarget = true;
      break;
    case 0x6229: // [$6229..$622F]    7 bytes
      CYCLES(0x6229, 12);
      /* $6229 ADC */ tmp = peek(0x140f), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $622C STA */ poke_zpg(0x00, s_a);
      /* $622E LDX */ s_x = update_nz(0x00);
    case 0x6230: // [$6230..$6235]    6 bytes
      CYCLES(0x6230, 11);
      /* $6230 LDY */ s_y = update_nz(0x05);
      /* $6232 LDA */ s_a = update_nz(peek_zpg(0x00));
      /* $6234 BEQ */ s_pc = s_status & STATUS_Z ? 0x6248 : 0x6236;
      branchTarget = true;
      break;
    case 0x6236: // [$6236..$623A]    5 bytes
      CYCLES(0x6236, 9);
      /* $6236 SEC */ s_status |= STATUS_C;
      /* $6237 SBC */ tmp = 0x05, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6239 BCS */ s_pc = s_status & STATUS_C ? 0x623f : 0x623b;
      branchTarget = true;
      break;
    case 0x623b: // [$623B..$623E]    4 bytes
      CYCLES(0x623b, 7);
      /* $623B LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $623D LDA */ s_a = update_nz(0x00);
    case 0x623f: // [$623F..$6247]    9 bytes
      CYCLES(0x623f, 16);
      /* $623F STA */ poke_zpg(0x00, s_a);
      /* $6241 TYA */ s_a = update_nz(s_y);
      /* $6242 STA */ poke(0x1a30 + s_x, s_a);
      /* $6245 INX */ s_x = update_nz(s_x + 1);
      /* $6246 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x6230 : 0x6248;
      branchTarget = true;
      break;
    case 0x6248: // [$6248..$624D]    6 bytes
      CYCLES(0x6248, 11);
      /* $6248 STX */ poke(0x140e, s_x);
      /* $624B LDY */ s_y = update_nz(peek(0x140e));
    case 0x624e: // [$624E..$6250]    3 bytes
      CYCLES(0x624e, 6);
      /* $624E DEY */ s_y = update_nz(s_y - 1);
      /* $624F BPL */ s_pc = !(s_status & STATUS_N) ? 0x625f : 0x6251;
      branchTarget = true;
      break;
    case 0x6251: // [$6251..$625E]   14 bytes
      CYCLES(0x6251, 24);
      /* $6251 LDA */ s_a = update_nz(0x00);
      /* $6253 STA */ poke(0x140f, s_a);
      /* $6256 STA */ poke(0x1410, s_a);
      /* $6259 LDA */ s_a = update_nz(0xff);
      /* $625B STA */ poke(0x1414, s_a);
      /* $625E RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x625f: // [$625F..$6261]    3 bytes
      CYCLES(0x625f, 6);
      /* $625F JSR */ push16(0x6261), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x6262: // [$6262..$6263]    2 bytes
      CYCLES(0x6262, 4);
      /* $6262 BMI */ s_pc = s_status & STATUS_N ? 0x627e : 0x6264;
      branchTarget = true;
      break;
    case 0x6264: // [$6264..$6268]    5 bytes
      CYCLES(0x6264, 9);
      /* $6264 LDA */ s_a = update_nz(0xf3);
      /* $6266 JSR */ push16(0x6268), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x6269: // [$6269..$6272]   10 bytes
      CYCLES(0x6269, 18);
      /* $6269 ORA */ s_a = update_nz(s_a | 0x01);
      /* $626B STA */ poke(0x19c0 + s_y, s_a);
      /* $626E LDX */ s_x = update_nz(0x00);
      /* $6270 JSR */ push16(0x6272), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x6273: // [$6273..$6274]    2 bytes
      CYCLES(0x6273, 4);
      /* $6273 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6277 : 0x6275;
      branchTarget = true;
      break;
    case 0x6275: // [$6275..$6276]    2 bytes
      CYCLES(0x6275, 4);
      /* $6275 LDX */ s_x = update_nz(0xb3);
    case 0x6277: // [$6277..$627D]    7 bytes
      CYCLES(0x6277, 12);
      /* $6277 TXA */ s_a = update_nz(s_x);
      /* $6278 STA */ poke(0x19d0 + s_y, s_a);
      /* $627B SEC */ s_status |= STATUS_C;
      /* $627C BCS */ s_pc = s_status & STATUS_C ? 0x6293 : 0x627e;
      branchTarget = true;
      break;
    case 0x627e: // [$627E..$6282]    5 bytes
      CYCLES(0x627e, 9);
      /* $627E LDA */ s_a = update_nz(0xb3);
      /* $6280 JSR */ push16(0x6282), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x6283: // [$6283..$628A]    8 bytes
      CYCLES(0x6283, 14);
      /* $6283 STA */ poke(0x19d0 + s_y, s_a);
      /* $6286 LDX */ s_x = update_nz(0x01);
      /* $6288 JSR */ push16(0x628a), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x628b: // [$628B..$628C]    2 bytes
      CYCLES(0x628b, 4);
      /* $628B BPL */ s_pc = !(s_status & STATUS_N) ? 0x628f : 0x628d;
      branchTarget = true;
      break;
    case 0x628d: // [$628D..$628E]    2 bytes
      CYCLES(0x628d, 4);
      /* $628D LDX */ s_x = update_nz(0xf3);
    case 0x628f: // [$628F..$6292]    4 bytes
      CYCLES(0x628f, 7);
      /* $628F TXA */ s_a = update_nz(s_x);
      /* $6290 STA */ poke(0x19c0 + s_y, s_a);
    case 0x6293: // [$6293..$62AA]   24 bytes
      CYCLES(0x6293, 41);
      /* $6293 LDA */ s_a = update_nz(0x00);
      /* $6295 STA */ poke(0x19e0 + s_y, s_a);
      /* $6298 STA */ poke(0x19f0 + s_y, s_a);
      /* $629B STA */ poke(0x1a40 + s_y, s_a);
      /* $629E STA */ poke(0x1a00 + s_y, s_a);
      /* $62A1 LDA */ s_a = update_nz(peek(0x1421));
      /* $62A4 LDX */ s_x = update_nz(peek(0x0c18));
      /* $62A7 INX */ s_x = update_nz(s_x + 1);
      /* $62A8 JSR */ push16(0x62aa), s_pc = 0x4c00;
      branchTarget = true;
      break;
    case 0x62ab: // [$62AB..$62B3]    9 bytes
      CYCLES(0x62ab, 16);
      /* $62AB STA */ poke(0x1a10 + s_y, s_a);
      /* $62AE LDA */ s_a = update_nz(peek(0x0c13));
      /* $62B1 JSR */ push16(0x62b3), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x62b4: // [$62B4..$62BD]   10 bytes
      CYCLES(0x62b4, 18);
      /* $62B4 CLC */ s_status &= ~STATUS_C;
      /* $62B5 ADC */ tmp = peek(0x0c12), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $62B8 STA */ poke(0x1a20 + s_y, s_a);
      /* $62BB SEC */ s_status |= STATUS_C;
      /* $62BC BCS */ s_pc = s_status & STATUS_C ? 0x624e : 0x62be;
      branchTarget = true;
      break;
    case 0x62be: // [$62BE..$62C0]    3 bytes
      CYCLES(0x62be, 6);
      /* $62BE LDX */ s_x = update_nz(peek(0x140e));
    case 0x62c1: // [$62C1..$62C3]    3 bytes
      CYCLES(0x62c1, 6);
      /* $62C1 DEX */ s_x = update_nz(s_x - 1);
      /* $62C2 BPL */ s_pc = !(s_status & STATUS_N) ? 0x62c5 : 0x62c4;
      branchTarget = true;
      break;
    case 0x62c4: // [$62C4..$62C4]    1 bytes
      CYCLES(0x62c4, 2);
      /* $62C4 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x62c5: // [$62C5..$62C9]    5 bytes
      CYCLES(0x62c5, 9);
      /* $62C5 DEC */ tmp16 = 0x1a00 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $62C8 BPL */ s_pc = !(s_status & STATUS_N) ? 0x62c1 : 0x62ca;
      branchTarget = true;
      break;
    case 0x62ca: // [$62CA..$62DE]   21 bytes
      CYCLES(0x62ca, 36);
      /* $62CA LDA */ s_a = update_nz(peek(0x0c18));
      /* $62CD STA */ poke(0x1a00 + s_x, s_a);
      /* $62D0 STX */ poke_zpg(0x00, s_x);
      /* $62D2 LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $62D4 LDX */ s_x = update_nz(peek(0x19d0 + s_y));
      /* $62D7 LDA */ s_a = update_nz(peek(0x19c0 + s_y));
      /* $62DA LDY */ s_y = update_nz(0x0d);
      /* $62DC JSR */ push16(0x62de), s_pc = 0x4d21;
      branchTarget = true;
      break;
    case 0x62df: // [$62DF..$62E5]    7 bytes
      CYCLES(0x62df, 12);
      /* $62DF LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $62E1 DEC */ tmp16 = 0x1a10 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $62E4 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6322 : 0x62e6;
      branchTarget = true;
      break;
    case 0x62e6: // [$62E6..$62EB]    6 bytes
      CYCLES(0x62e6, 11);
      /* $62E6 LDA */ s_a = update_nz(peek(0x0c1b));
      /* $62E9 JSR */ push16(0x62eb), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x62ec: // [$62EC..$62F8]   13 bytes
      CYCLES(0x62ec, 23);
      /* $62EC LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $62EE STA */ poke(0x1a10 + s_y, s_a);
      /* $62F1 LDA */ s_a = update_nz(peek(0x0c17));
      /* $62F4 ASL */ s_a = update_nzc(s_a << 1);
      /* $62F5 PHA */ push8(s_a);
      /* $62F6 JSR */ push16(0x62f8), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x62f9: // [$62F9..$6305]   13 bytes
      CYCLES(0x62f9, 23);
      /* $62F9 SEC */ s_status |= STATUS_C;
      /* $62FA SBC */ tmp = peek(0x0c17), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $62FD AND */ s_a = update_nz(s_a & 0xfe);
      /* $62FF STA */ poke(0x19e0 + s_y, s_a);
      /* $6302 PLA */ s_a = update_nz(pop8());
      /* $6303 JSR */ push16(0x6305), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x6306: // [$6306..$6313]   14 bytes
      CYCLES(0x6306, 24);
      /* $6306 SEC */ s_status |= STATUS_C;
      /* $6307 SBC */ tmp = peek(0x0c17), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $630A STA */ poke(0x19f0 + s_y, s_a);
      /* $630D LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $630F DEC */ tmp16 = 0x1a20 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $6312 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6322 : 0x6314;
      branchTarget = true;
      break;
    case 0x6314: // [$6314..$6319]    6 bytes
      CYCLES(0x6314, 11);
      /* $6314 LDA */ s_a = update_nz(peek(0x0c13));
      /* $6317 JSR */ push16(0x6319), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x631a: // [$631A..$6321]    8 bytes
      CYCLES(0x631a, 14);
      /* $631A STA */ poke(0x1a20 + s_y, s_a);
      /* $631D LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $631F JSR */ push16(0x6321), s_pc = 0x63b3;
      branchTarget = true;
      break;
    case 0x6322: // [$6322..$6326]    5 bytes
      CYCLES(0x6322, 9);
      /* $6322 LDA */ s_a = update_nz(peek(0x1a20 + s_x));
      /* $6325 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x6353 : 0x6327;
      branchTarget = true;
      break;
    case 0x6327: // [$6327..$632E]    8 bytes
      CYCLES(0x6327, 14);
      /* $6327 LDA */ s_a = update_nz(peek(0x1a10 + s_x));
      /* $632A CMP */ update_nz_inv_c(s_a - peek(0x0c1c));
      /* $632D BCS */ s_pc = s_status & STATUS_C ? 0x6353 : 0x632f;
      branchTarget = true;
      break;
    case 0x632f: // [$632F..$6333]    5 bytes
      CYCLES(0x632f, 9);
      /* $632F LDA */ s_a = update_nz(peek(0x19e0 + s_x));
      /* $6332 BMI */ s_pc = s_status & STATUS_N ? 0x633b : 0x6334;
      branchTarget = true;
      break;
    case 0x6334: // [$6334..$6335]    2 bytes
      CYCLES(0x6334, 4);
      /* $6334 BEQ */ s_pc = s_status & STATUS_Z ? 0x633e : 0x6336;
      branchTarget = true;
      break;
    case 0x6336: // [$6336..$633A]    5 bytes
      CYCLES(0x6336, 9);
      /* $6336 SEC */ s_status |= STATUS_C;
      /* $6337 SBC */ tmp = 0x02, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6339 BCS */ s_pc = s_status & STATUS_C ? 0x633e : 0x633b;
      branchTarget = true;
      break;
    case 0x633b: // [$633B..$633D]    3 bytes
      CYCLES(0x633b, 6);
      /* $633B CLC */ s_status &= ~STATUS_C;
      /* $633C ADC */ tmp = 0x02, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0x633e: // [$633E..$6345]    8 bytes
      CYCLES(0x633e, 14);
      /* $633E STA */ poke(0x19e0 + s_x, s_a);
      /* $6341 LDA */ s_a = update_nz(peek(0x19f0 + s_x));
      /* $6344 BMI */ s_pc = s_status & STATUS_N ? 0x634d : 0x6346;
      branchTarget = true;
      break;
    case 0x6346: // [$6346..$6347]    2 bytes
      CYCLES(0x6346, 4);
      /* $6346 BEQ */ s_pc = s_status & STATUS_Z ? 0x6350 : 0x6348;
      branchTarget = true;
      break;
    case 0x6348: // [$6348..$634C]    5 bytes
      CYCLES(0x6348, 9);
      /* $6348 SEC */ s_status |= STATUS_C;
      /* $6349 SBC */ tmp = 0x02, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $634B BCS */ s_pc = s_status & STATUS_C ? 0x6350 : 0x634d;
      branchTarget = true;
      break;
    case 0x634d: // [$634D..$634F]    3 bytes
      CYCLES(0x634d, 6);
      /* $634D CLC */ s_status &= ~STATUS_C;
      /* $634E ADC */ tmp = 0x02, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0x6350: // [$6350..$6352]    3 bytes
      CYCLES(0x6350, 6);
      /* $6350 STA */ poke(0x19f0 + s_x, s_a);
    case 0x6353: // [$6353..$6357]    5 bytes
      CYCLES(0x6353, 9);
      /* $6353 DEC */ tmp16 = 0x1a40 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $6356 BPL */ s_pc = !(s_status & STATUS_N) ? 0x635d : 0x6358;
      branchTarget = true;
      break;
    case 0x6358: // [$6358..$635C]    5 bytes
      CYCLES(0x6358, 9);
      /* $6358 LDA */ s_a = update_nz(0x03);
      /* $635A STA */ poke(0x1a40 + s_x, s_a);
    case 0x635d: // [$635D..$6374]   24 bytes
      CYCLES(0x635d, 41);
      /* $635D LDY */ s_y = update_nz(peek(0x1a40 + s_x));
      /* $6360 LDA */ s_a = update_nz(peek(0x08c0 + s_y));
      /* $6363 STA */ poke_zpg(0x06, s_a);
      /* $6365 LDA */ s_a = update_nz(peek(0x08c8 + s_y));
      /* $6368 STA */ poke_zpg(0x07, s_a);
      /* $636A LDA */ s_a = update_nz(peek(0x19c0 + s_x));
      /* $636D CLC */ s_status &= ~STATUS_C;
      /* $636E ADC */ tmp = peek(0x19e0 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6371 CMP */ update_nz_inv_c(s_a - 0xf3);
      /* $6373 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6385 : 0x6375;
      branchTarget = true;
      break;
    case 0x6375: // [$6375..$637E]   10 bytes
      CYCLES(0x6375, 18);
      /* $6375 LDA */ s_a = update_nz(0x00);
      /* $6377 LDY */ s_y = update_nz(peek(0x19e0 + s_x));
      /* $637A STA */ poke(0x19e0 + s_x, s_a);
      /* $637D BMI */ s_pc = s_status & STATUS_N ? 0x6383 : 0x637f;
      branchTarget = true;
      break;
    case 0x637f: // [$637F..$6382]    4 bytes
      CYCLES(0x637f, 7);
      /* $637F LDA */ s_a = update_nz(0xf1);
      /* $6381 BMI */ s_pc = s_status & STATUS_N ? 0x6385 : 0x6383;
      branchTarget = true;
      break;
    case 0x6383: // [$6383..$6384]    2 bytes
      CYCLES(0x6383, 4);
      /* $6383 LDA */ s_a = update_nz(0x01);
    case 0x6385: // [$6385..$6392]   14 bytes
      CYCLES(0x6385, 24);
      /* $6385 STA */ poke(0x19c0 + s_x, s_a);
      /* $6388 LDA */ s_a = update_nz(peek(0x19d0 + s_x));
      /* $638B CLC */ s_status &= ~STATUS_C;
      /* $638C ADC */ tmp = peek(0x19f0 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $638F CMP */ update_nz_inv_c(s_a - 0xb3);
      /* $6391 BCC */ s_pc = !(s_status & STATUS_C) ? 0x63a3 : 0x6393;
      branchTarget = true;
      break;
    case 0x6393: // [$6393..$639C]   10 bytes
      CYCLES(0x6393, 18);
      /* $6393 LDA */ s_a = update_nz(0x00);
      /* $6395 LDY */ s_y = update_nz(peek(0x19f0 + s_x));
      /* $6398 STA */ poke(0x19f0 + s_x, s_a);
      /* $639B BMI */ s_pc = s_status & STATUS_N ? 0x63a1 : 0x639d;
      branchTarget = true;
      break;
    case 0x639d: // [$639D..$63A0]    4 bytes
      CYCLES(0x639d, 7);
      /* $639D LDA */ s_a = update_nz(0xb2);
      /* $639F BMI */ s_pc = s_status & STATUS_N ? 0x63a3 : 0x63a1;
      branchTarget = true;
      break;
    case 0x63a1: // [$63A1..$63A2]    2 bytes
      CYCLES(0x63a1, 4);
      /* $63A1 LDA */ s_a = update_nz(0x00);
    case 0x63a3: // [$63A3..$63AD]   11 bytes
      CYCLES(0x63a3, 19);
      /* $63A3 STA */ poke(0x19d0 + s_x, s_a);
      /* $63A6 TAY */ s_y = update_nz(s_a);
      /* $63A7 LDA */ s_a = update_nz(peek(0x19c0 + s_x));
      /* $63AA TAX */ s_x = update_nz(s_a);
      /* $63AB JSR */ push16(0x63ad), s_pc = 0x4cf6;
      branchTarget = true;
      break;
    case 0x63ae: // [$63AE..$63B2]    5 bytes
      CYCLES(0x63ae, 9);
      /* $63AE LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $63B0 JMP */ s_pc = 0x62c1;
      branchTarget = true;
      break;
    case 0x63b3: // [$63B3..$63B9]    7 bytes
      CYCLES(0x63b3, 12);
      /* $63B3 LDY */ s_y = update_nz(peek(0x140f));
      /* $63B6 CPY */ update_nz_inv_c(s_y - 0x10);
      /* $63B8 BCC */ s_pc = !(s_status & STATUS_C) ? 0x63bb : 0x63ba;
      branchTarget = true;
      break;
    case 0x63ba: // [$63BA..$63BA]    1 bytes
      CYCLES(0x63ba, 2);
      /* $63BA RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x63bb: // [$63BB..$63D2]   24 bytes
      CYCLES(0x63bb, 41);
      /* $63BB LDA */ s_a = update_nz(peek(0x19c0 + s_x));
      /* $63BE STA */ poke(0x1a50 + s_y, s_a);
      /* $63C1 LDA */ s_a = update_nz(peek(0x19d0 + s_x));
      /* $63C4 STA */ poke(0x1a60 + s_y, s_a);
      /* $63C7 LDA */ s_a = update_nz(0x01);
      /* $63C9 STA */ poke(0x1a70 + s_y, s_a);
      /* $63CC STX */ poke_zpg(0x00, s_x);
      /* $63CE TYA */ s_a = update_nz(s_y);
      /* $63CF TAX */ s_x = update_nz(s_a);
      /* $63D0 JSR */ push16(0x63d2), s_pc = 0x67b8;
      branchTarget = true;
      break;
    case 0x63d3: // [$63D3..$63E8]   22 bytes
      CYCLES(0x63d3, 38);
      /* $63D3 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $63D5 LDA */ s_a = update_nz(peek(0x0c1e));
      /* $63D8 STA */ poke(0x1412, s_a);
      /* $63DB LDA */ s_a = update_nz(peek(0x0c1d));
      /* $63DE STA */ poke(0x1413, s_a);
      /* $63E1 INC */ tmp16 = 0x140f, poke(tmp16, update_nz(peek(tmp16) + 1));
      /* $63E4 DEC */ tmp16 = 0x1a30 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $63E7 BEQ */ s_pc = s_status & STATUS_Z ? 0x63ea : 0x63e9;
      branchTarget = true;
      break;
    case 0x63e9: // [$63E9..$63E9]    1 bytes
      CYCLES(0x63e9, 2);
      /* $63E9 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x63ea: // [$63EA..$63EC]    3 bytes
      CYCLES(0x63ea, 6);
      /* $63EA JSR */ push16(0x63ec), s_pc = 0x63f2;
      branchTarget = true;
      break;
    case 0x63ed: // [$63ED..$63F1]    5 bytes
      CYCLES(0x63ed, 9);
      /* $63ED PLA */ s_a = update_nz(pop8());
      /* $63EE PLA */ s_a = update_nz(pop8());
      /* $63EF JMP */ s_pc = 0x62c1;
      branchTarget = true;
      break;
    case 0x63f2: // [$63F2..$63F6]    5 bytes
      CYCLES(0x63f2, 9);
      /* $63F2 TXA */ s_a = update_nz(s_x);
      /* $63F3 PHA */ push8(s_a);
      /* $63F4 DEC */ tmp16 = 0x140e, poke(tmp16, update_nz(peek(tmp16) - 1));
    case 0x63f7: // [$63F7..$6432]   60 bytes
      CYCLES(0x63f7, 103);
      /* $63F7 LDA */ s_a = update_nz(peek(0x19c1 + s_x));
      /* $63FA STA */ poke(0x19c0 + s_x, s_a);
      /* $63FD LDA */ s_a = update_nz(peek(0x19d1 + s_x));
      /* $6400 STA */ poke(0x19d0 + s_x, s_a);
      /* $6403 LDA */ s_a = update_nz(peek(0x19e1 + s_x));
      /* $6406 STA */ poke(0x19e0 + s_x, s_a);
      /* $6409 LDA */ s_a = update_nz(peek(0x19f1 + s_x));
      /* $640C STA */ poke(0x19f0 + s_x, s_a);
      /* $640F LDA */ s_a = update_nz(peek(0x1a01 + s_x));
      /* $6412 STA */ poke(0x1a00 + s_x, s_a);
      /* $6415 LDA */ s_a = update_nz(peek(0x1a11 + s_x));
      /* $6418 STA */ poke(0x1a10 + s_x, s_a);
      /* $641B LDA */ s_a = update_nz(peek(0x1a21 + s_x));
      /* $641E STA */ poke(0x1a20 + s_x, s_a);
      /* $6421 LDA */ s_a = update_nz(peek(0x1a31 + s_x));
      /* $6424 STA */ poke(0x1a30 + s_x, s_a);
      /* $6427 LDA */ s_a = update_nz(peek(0x1a41 + s_x));
      /* $642A STA */ poke(0x1a40 + s_x, s_a);
      /* $642D INX */ s_x = update_nz(s_x + 1);
      /* $642E CPX */ update_nz_inv_c(s_x - peek(0x140e));
      /* $6431 BCC */ s_pc = !(s_status & STATUS_C) ? 0x63f7 : 0x6433;
      branchTarget = true;
      break;
    case 0x6433: // [$6433..$6435]    3 bytes
      CYCLES(0x6433, 6);
      /* $6433 PLA */ s_a = update_nz(pop8());
      /* $6434 TAX */ s_x = update_nz(s_a);
      /* $6435 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6436: // [$6436..$643A]    5 bytes
      CYCLES(0x6436, 9);
      /* $6436 LDX */ s_x = update_nz(peek(0x140f));
      /* $6439 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x643c : 0x643b;
      branchTarget = true;
      break;
    case 0x643b: // [$643B..$643B]    1 bytes
      CYCLES(0x643b, 2);
      /* $643B RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x643c: // [$643C..$643E]    3 bytes
      CYCLES(0x643c, 6);
      /* $643C DEX */ s_x = update_nz(s_x - 1);
      /* $643D BPL */ s_pc = !(s_status & STATUS_N) ? 0x6442 : 0x643f;
      branchTarget = true;
      break;
    case 0x643f: // [$643F..$6441]    3 bytes
      CYCLES(0x643f, 6);
      /* $643F JMP */ s_pc = 0x6789;
      branchTarget = true;
      break;
    case 0x6442: // [$6442..$6446]    5 bytes
      CYCLES(0x6442, 9);
      /* $6442 DEC */ tmp16 = 0x1a70 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $6445 BPL */ s_pc = !(s_status & STATUS_N) ? 0x643c : 0x6447;
      branchTarget = true;
      break;
    case 0x6447: // [$6447..$645B]   21 bytes
      CYCLES(0x6447, 36);
      /* $6447 LDA */ s_a = update_nz(peek(0x0c19));
      /* $644A STA */ poke(0x1a70 + s_x, s_a);
      /* $644D STX */ poke_zpg(0x00, s_x);
      /* $644F LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $6451 LDX */ s_x = update_nz(peek(0x1a60 + s_y));
      /* $6454 LDA */ s_a = update_nz(peek(0x1a50 + s_y));
      /* $6457 LDY */ s_y = update_nz(0x0d);
      /* $6459 JSR */ push16(0x645b), s_pc = 0x4d21;
      branchTarget = true;
      break;
    case 0x645c: // [$645C..$6469]   14 bytes
      CYCLES(0x645c, 24);
      /* $645C LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $645E LDA */ s_a = update_nz(peek(0x1a50 + s_x));
      /* $6461 CLC */ s_status &= ~STATUS_C;
      /* $6462 ADC */ tmp = peek(0x1a80 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6465 LDY */ s_y = update_nz(peek(0x1a80 + s_x));
      /* $6468 BMI */ s_pc = s_status & STATUS_N ? 0x6474 : 0x646a;
      branchTarget = true;
      break;
    case 0x646a: // [$646A..$646B]    2 bytes
      CYCLES(0x646a, 4);
      /* $646A BCS */ s_pc = s_status & STATUS_C ? 0x6470 : 0x646c;
      branchTarget = true;
      break;
    case 0x646c: // [$646C..$646F]    4 bytes
      CYCLES(0x646c, 7);
      /* $646C CMP */ update_nz_inv_c(s_a - 0xf4);
      /* $646E BCC */ s_pc = !(s_status & STATUS_C) ? 0x6478 : 0x6470;
      branchTarget = true;
      break;
    case 0x6470: // [$6470..$6473]    4 bytes
      CYCLES(0x6470, 7);
      /* $6470 LDA */ s_a = update_nz(0xf3);
      /* $6472 BMI */ s_pc = s_status & STATUS_N ? 0x6478 : 0x6474;
      branchTarget = true;
      break;
    case 0x6474: // [$6474..$6475]    2 bytes
      CYCLES(0x6474, 4);
      /* $6474 BCS */ s_pc = s_status & STATUS_C ? 0x6478 : 0x6476;
      branchTarget = true;
      break;
    case 0x6476: // [$6476..$6477]    2 bytes
      CYCLES(0x6476, 4);
      /* $6476 LDA */ s_a = update_nz(0x00);
    case 0x6478: // [$6478..$6485]   14 bytes
      CYCLES(0x6478, 24);
      /* $6478 STA */ poke(0x1a50 + s_x, s_a);
      /* $647B LDA */ s_a = update_nz(peek(0x1a60 + s_x));
      /* $647E CLC */ s_status &= ~STATUS_C;
      /* $647F ADC */ tmp = peek(0x1a90 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6482 CMP */ update_nz_inv_c(s_a - 0xb3);
      /* $6484 BCC */ s_pc = !(s_status & STATUS_C) ? 0x648f : 0x6486;
      branchTarget = true;
      break;
    case 0x6486: // [$6486..$648C]    7 bytes
      CYCLES(0x6486, 12);
      /* $6486 LDA */ s_a = update_nz(0xb2);
      /* $6488 LDY */ s_y = update_nz(peek(0x1a90 + s_x));
      /* $648B BPL */ s_pc = !(s_status & STATUS_N) ? 0x648f : 0x648d;
      branchTarget = true;
      break;
    case 0x648d: // [$648D..$648E]    2 bytes
      CYCLES(0x648d, 4);
      /* $648D LDA */ s_a = update_nz(0x00);
    case 0x648f: // [$648F..$64A3]   21 bytes
      CYCLES(0x648f, 36);
      /* $648F STA */ poke(0x1a60 + s_x, s_a);
      /* $6492 TAY */ s_y = update_nz(s_a);
      /* $6493 LDA */ s_a = update_nz(peek(0x1a50 + s_x));
      /* $6496 TAX */ s_x = update_nz(s_a);
      /* $6497 LDA */ s_a = update_nz(peek(0x08d0));
      /* $649A STA */ poke_zpg(0x06, s_a);
      /* $649C LDA */ s_a = update_nz(peek(0x08d4));
      /* $649F STA */ poke_zpg(0x07, s_a);
      /* $64A1 JSR */ push16(0x64a3), s_pc = 0x4cf6;
      branchTarget = true;
      break;
    case 0x64a4: // [$64A4..$64A7]    4 bytes
      CYCLES(0x64a4, 7);
      /* $64A4 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $64A6 BPL */ s_pc = !(s_status & STATUS_N) ? 0x643c : 0x64a8;
      branchTarget = true;
      break;
    case 0x64a8: // [$64A8..$64AE]    7 bytes
      CYCLES(0x64a8, 12);
      /* $64A8 LDY */ s_y = update_nz(peek(0x1410));
      /* $64AB CPY */ update_nz_inv_c(s_y - 0x10);
      /* $64AD BCS */ s_pc = s_status & STATUS_C ? 0x64ff : 0x64af;
      branchTarget = true;
      break;
    case 0x64af: // [$64AF..$64C4]   22 bytes
      CYCLES(0x64af, 38);
      /* $64AF LDA */ s_a = update_nz(peek(0x1a60 + s_x));
      /* $64B2 ADC */ tmp = 0x03, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $64B4 STA */ poke(0x1ab0 + s_y, s_a);
      /* $64B7 LDA */ s_a = update_nz(peek(0x1a50 + s_x));
      /* $64BA ADC */ tmp = 0x04, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $64BC STA */ poke(0x1aa0 + s_y, s_a);
      /* $64BF LDX */ s_x = update_nz(peek(0x1500));
      /* $64C2 JSR */ push16(0x64c4), s_pc = 0x6500;
      branchTarget = true;
      break;
    case 0x64c5: // [$64C5..$64D0]   12 bytes
      CYCLES(0x64c5, 21);
      /* $64C5 STA */ poke(0x1ac0 + s_y, s_a);
      /* $64C8 LDA */ s_a = update_nz(peek(0x1ab0 + s_y));
      /* $64CB LDX */ s_x = update_nz(peek(0x1501));
      /* $64CE JSR */ push16(0x64d0), s_pc = 0x6500;
      branchTarget = true;
      break;
    case 0x64d1: // [$64D1..$64E5]   21 bytes
      CYCLES(0x64d1, 36);
      /* $64D1 STA */ poke(0x1ad0 + s_y, s_a);
      /* $64D4 LDA */ s_a = update_nz(peek(0x0c16));
      /* $64D7 STA */ poke(0x1af0 + s_y, s_a);
      /* $64DA LDA */ s_a = update_nz(peek_zpg(0x4e));
      /* $64DC AND */ s_a = update_nz(s_a & 0x01);
      /* $64DE STA */ poke(0x1ae0 + s_y, s_a);
      /* $64E1 LDA */ s_a = update_nz(0x03);
      /* $64E3 JSR */ push16(0x64e5), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x64e6: // [$64E6..$64EB]    6 bytes
      CYCLES(0x64e6, 11);
      /* $64E6 SEC */ s_status |= STATUS_C;
      /* $64E7 SBC */ tmp = 0x01, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $64E9 STA */ poke(0x1b00 + s_y, s_a);
    case 0x64ec: // [$64EC..$64F0]    5 bytes
      CYCLES(0x64ec, 9);
      /* $64EC LDA */ s_a = update_nz(0x03);
      /* $64EE JSR */ push16(0x64f0), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x64f1: // [$64F1..$64FB]   11 bytes
      CYCLES(0x64f1, 19);
      /* $64F1 SEC */ s_status |= STATUS_C;
      /* $64F2 SBC */ tmp = 0x01, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $64F4 STA */ poke(0x1b10 + s_y, s_a);
      /* $64F7 ORA */ s_a = update_nz(s_a | peek(0x1b00 + s_y));
      /* $64FA BEQ */ s_pc = s_status & STATUS_Z ? 0x64ec : 0x64fc;
      branchTarget = true;
      break;
    case 0x64fc: // [$64FC..$64FE]    3 bytes
      CYCLES(0x64fc, 6);
      /* $64FC INC */ tmp16 = 0x1410, poke(tmp16, update_nz(peek(tmp16) + 1));
    case 0x64ff: // [$64FF..$64FF]    1 bytes
      CYCLES(0x64ff, 2);
      /* $64FF RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6500: // [$6500..$6516]   23 bytes
      CYCLES(0x6500, 40);
      /* $6500 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $6501 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $6502 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $6503 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $6504 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $6505 STA */ poke_zpg(0x06, s_a);
      /* $6507 TXA */ s_a = update_nz(s_x);
      /* $6508 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $6509 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $650A LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $650B LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $650C LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $650D SEC */ s_status |= STATUS_C;
      /* $650E SBC */ tmp = peek_zpg(0x06), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6510 STA */ poke_zpg(0x06, s_a);
      /* $6512 LDA */ s_a = update_nz(0x05);
      /* $6514 JSR */ push16(0x6516), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x6517: // [$6517..$651E]    8 bytes
      CYCLES(0x6517, 14);
      /* $6517 SEC */ s_status |= STATUS_C;
      /* $6518 SBC */ tmp = 0x02, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $651A CLC */ s_status &= ~STATUS_C;
      /* $651B ADC */ tmp = peek_zpg(0x06), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $651D BNE */ s_pc = !(s_status & STATUS_Z) ? 0x6522 : 0x651f;
      branchTarget = true;
      break;
    case 0x651f: // [$651F..$6521]    3 bytes
      CYCLES(0x651f, 6);
      /* $651F CLC */ s_status &= ~STATUS_C;
      /* $6520 ADC */ tmp = 0x01, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0x6522: // [$6522..$6522]    1 bytes
      CYCLES(0x6522, 2);
      /* $6522 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6523: // [$6523..$6525]    3 bytes
      CYCLES(0x6523, 6);
      /* $6523 LDX */ s_x = update_nz(peek(0x1410));
    case 0x6526: // [$6526..$6528]    3 bytes
      CYCLES(0x6526, 6);
      /* $6526 DEX */ s_x = update_nz(s_x - 1);
      /* $6527 BPL */ s_pc = !(s_status & STATUS_N) ? 0x652a : 0x6529;
      branchTarget = true;
      break;
    case 0x6529: // [$6529..$6529]    1 bytes
      CYCLES(0x6529, 2);
      /* $6529 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x652a: // [$652A..$652E]    5 bytes
      CYCLES(0x652a, 9);
      /* $652A DEC */ tmp16 = 0x1ae0 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $652D BPL */ s_pc = !(s_status & STATUS_N) ? 0x6526 : 0x652f;
      branchTarget = true;
      break;
    case 0x652f: // [$652F..$6543]   21 bytes
      CYCLES(0x652f, 36);
      /* $652F LDA */ s_a = update_nz(peek(0x0c1a));
      /* $6532 STA */ poke(0x1ae0 + s_x, s_a);
      /* $6535 STX */ poke_zpg(0x00, s_x);
      /* $6537 LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $6539 LDX */ s_x = update_nz(peek(0x1ab0 + s_y));
      /* $653C LDA */ s_a = update_nz(peek(0x1aa0 + s_y));
      /* $653F LDY */ s_y = update_nz(0x07);
      /* $6541 JSR */ push16(0x6543), s_pc = 0x4cd0;
      branchTarget = true;
      break;
    case 0x6544: // [$6544..$654A]    7 bytes
      CYCLES(0x6544, 12);
      /* $6544 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $6546 DEC */ tmp16 = 0x1af0 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $6549 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x6551 : 0x654b;
      branchTarget = true;
      break;
    case 0x654b: // [$654B..$654D]    3 bytes
      CYCLES(0x654b, 6);
      /* $654B JSR */ push16(0x654d), s_pc = 0x65d9;
      branchTarget = true;
      break;
    case 0x654e: // [$654E..$6550]    3 bytes
      CYCLES(0x654e, 6);
      /* $654E JMP */ s_pc = 0x6526;
      branchTarget = true;
      break;
    case 0x6551: // [$6551..$656D]   29 bytes
      CYCLES(0x6551, 50);
      /* $6551 LDA */ s_a = update_nz(peek(0x1af0 + s_x));
      /* $6554 AND */ s_a = update_nz(s_a & 0x01);
      /* $6556 STA */ poke_zpg(0x08, s_a);
      /* $6558 TAY */ s_y = update_nz(s_a);
      /* $6559 LDA */ s_a = update_nz(peek(0x08d8 + s_y));
      /* $655C STA */ poke_zpg(0x06, s_a);
      /* $655E LDA */ s_a = update_nz(peek(0x08dc + s_y));
      /* $6561 STA */ poke_zpg(0x07, s_a);
      /* $6563 LDA */ s_a = update_nz(peek(0x1ab0 + s_x));
      /* $6566 CLC */ s_status &= ~STATUS_C;
      /* $6567 ADC */ tmp = peek(0x1ad0 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $656A CMP */ update_nz_inv_c(s_a - 0xb9);
      /* $656C BCC */ s_pc = !(s_status & STATUS_C) ? 0x657d : 0x656e;
      branchTarget = true;
      break;
    case 0x656e: // [$656E..$6574]    7 bytes
      CYCLES(0x656e, 12);
      /* $656E LDA */ s_a = update_nz(0xb8);
      /* $6570 LDY */ s_y = update_nz(peek(0x1ad0 + s_x));
      /* $6573 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6577 : 0x6575;
      branchTarget = true;
      break;
    case 0x6575: // [$6575..$6576]    2 bytes
      CYCLES(0x6575, 4);
      /* $6575 LDA */ s_a = update_nz(0x00);
    case 0x6577: // [$6577..$657C]    6 bytes
      CYCLES(0x6577, 11);
      /* $6577 STA */ poke(0x1ab0 + s_x, s_a);
      /* $657A PHA */ push8(s_a);
      /* $657B BPL */ s_pc = !(s_status & STATUS_N) ? 0x6596 : 0x657d;
      branchTarget = true;
      break;
    case 0x657d: // [$657D..$6584]    8 bytes
      CYCLES(0x657d, 14);
      /* $657D STA */ poke(0x1ab0 + s_x, s_a);
      /* $6580 PHA */ push8(s_a);
      /* $6581 LDA */ s_a = update_nz(peek_zpg(0x08));
      /* $6583 BEQ */ s_pc = s_status & STATUS_Z ? 0x6596 : 0x6585;
      branchTarget = true;
      break;
    case 0x6585: // [$6585..$658E]   10 bytes
      CYCLES(0x6585, 18);
      /* $6585 LDA */ s_a = update_nz(peek(0x1ad0 + s_x));
      /* $6588 ADC */ tmp = peek(0x1b10 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $658B CMP */ update_nz_inv_c(s_a - 0xf6);
      /* $658D BCS */ s_pc = s_status & STATUS_C ? 0x6593 : 0x658f;
      branchTarget = true;
      break;
    case 0x658f: // [$658F..$6592]    4 bytes
      CYCLES(0x658f, 7);
      /* $658F CMP */ update_nz_inv_c(s_a - 0x0a);
      /* $6591 BCS */ s_pc = s_status & STATUS_C ? 0x6596 : 0x6593;
      branchTarget = true;
      break;
    case 0x6593: // [$6593..$6595]    3 bytes
      CYCLES(0x6593, 6);
      /* $6593 STA */ poke(0x1ad0 + s_x, s_a);
    case 0x6596: // [$6596..$65A1]   12 bytes
      CYCLES(0x6596, 21);
      /* $6596 LDA */ s_a = update_nz(peek(0x1aa0 + s_x));
      /* $6599 CLC */ s_status &= ~STATUS_C;
      /* $659A ADC */ tmp = peek(0x1ac0 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $659D LDY */ s_y = update_nz(peek(0x1ac0 + s_x));
      /* $65A0 BMI */ s_pc = s_status & STATUS_N ? 0x65a8 : 0x65a2;
      branchTarget = true;
      break;
    case 0x65a2: // [$65A2..$65A3]    2 bytes
      CYCLES(0x65a2, 4);
      /* $65A2 BCC */ s_pc = !(s_status & STATUS_C) ? 0x65b3 : 0x65a4;
      branchTarget = true;
      break;
    case 0x65a4: // [$65A4..$65A7]    4 bytes
      CYCLES(0x65a4, 7);
      /* $65A4 LDA */ s_a = update_nz(0xf9);
      /* $65A6 BMI */ s_pc = s_status & STATUS_N ? 0x65ac : 0x65a8;
      branchTarget = true;
      break;
    case 0x65a8: // [$65A8..$65A9]    2 bytes
      CYCLES(0x65a8, 4);
      /* $65A8 BCS */ s_pc = s_status & STATUS_C ? 0x65b3 : 0x65aa;
      branchTarget = true;
      break;
    case 0x65aa: // [$65AA..$65AB]    2 bytes
      CYCLES(0x65aa, 4);
      /* $65AA LDA */ s_a = update_nz(0x00);
    case 0x65ac: // [$65AC..$65B2]    7 bytes
      CYCLES(0x65ac, 12);
      /* $65AC STA */ poke(0x1aa0 + s_x, s_a);
      /* $65AF PHA */ push8(s_a);
      /* $65B0 SEC */ s_status |= STATUS_C;
      /* $65B1 BCS */ s_pc = s_status & STATUS_C ? 0x65cd : 0x65b3;
      branchTarget = true;
      break;
    case 0x65b3: // [$65B3..$65BA]    8 bytes
      CYCLES(0x65b3, 14);
      /* $65B3 STA */ poke(0x1aa0 + s_x, s_a);
      /* $65B6 PHA */ push8(s_a);
      /* $65B7 LDA */ s_a = update_nz(peek_zpg(0x08));
      /* $65B9 BEQ */ s_pc = s_status & STATUS_Z ? 0x65cd : 0x65bb;
      branchTarget = true;
      break;
    case 0x65bb: // [$65BB..$65C5]   11 bytes
      CYCLES(0x65bb, 19);
      /* $65BB LDA */ s_a = update_nz(peek(0x1ac0 + s_x));
      /* $65BE CLC */ s_status &= ~STATUS_C;
      /* $65BF ADC */ tmp = peek(0x1b00 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $65C2 CMP */ update_nz_inv_c(s_a - 0xf6);
      /* $65C4 BCS */ s_pc = s_status & STATUS_C ? 0x65ca : 0x65c6;
      branchTarget = true;
      break;
    case 0x65c6: // [$65C6..$65C9]    4 bytes
      CYCLES(0x65c6, 7);
      /* $65C6 CMP */ update_nz_inv_c(s_a - 0x0a);
      /* $65C8 BCS */ s_pc = s_status & STATUS_C ? 0x65cd : 0x65ca;
      branchTarget = true;
      break;
    case 0x65ca: // [$65CA..$65CC]    3 bytes
      CYCLES(0x65ca, 6);
      /* $65CA STA */ poke(0x1ac0 + s_x, s_a);
    case 0x65cd: // [$65CD..$65D3]    7 bytes
      CYCLES(0x65cd, 12);
      /* $65CD PLA */ s_a = update_nz(pop8());
      /* $65CE TAX */ s_x = update_nz(s_a);
      /* $65CF PLA */ s_a = update_nz(pop8());
      /* $65D0 TAY */ s_y = update_nz(s_a);
      /* $65D1 JSR */ push16(0x65d3), s_pc = 0x4cac;
      branchTarget = true;
      break;
    case 0x65d4: // [$65D4..$65D8]    5 bytes
      CYCLES(0x65d4, 9);
      /* $65D4 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $65D6 JMP */ s_pc = 0x6526;
      branchTarget = true;
      break;
    case 0x65d9: // [$65D9..$65DD]    5 bytes
      CYCLES(0x65d9, 9);
      /* $65D9 TXA */ s_a = update_nz(s_x);
      /* $65DA PHA */ push8(s_a);
      /* $65DB DEC */ tmp16 = 0x1410, poke(tmp16, update_nz(peek(tmp16) - 1));
    case 0x65de: // [$65DE..$6613]   54 bytes
      CYCLES(0x65de, 92);
      /* $65DE LDA */ s_a = update_nz(peek(0x1aa1 + s_x));
      /* $65E1 STA */ poke(0x1aa0 + s_x, s_a);
      /* $65E4 LDA */ s_a = update_nz(peek(0x1ab1 + s_x));
      /* $65E7 STA */ poke(0x1ab0 + s_x, s_a);
      /* $65EA LDA */ s_a = update_nz(peek(0x1ac1 + s_x));
      /* $65ED STA */ poke(0x1ac0 + s_x, s_a);
      /* $65F0 LDA */ s_a = update_nz(peek(0x1ad1 + s_x));
      /* $65F3 STA */ poke(0x1ad0 + s_x, s_a);
      /* $65F6 LDA */ s_a = update_nz(peek(0x1ae1 + s_x));
      /* $65F9 STA */ poke(0x1ae0 + s_x, s_a);
      /* $65FC LDA */ s_a = update_nz(peek(0x1af1 + s_x));
      /* $65FF STA */ poke(0x1af0 + s_x, s_a);
      /* $6602 LDA */ s_a = update_nz(peek(0x1b01 + s_x));
      /* $6605 STA */ poke(0x1b00 + s_x, s_a);
      /* $6608 LDA */ s_a = update_nz(peek(0x1b11 + s_x));
      /* $660B STA */ poke(0x1b10 + s_x, s_a);
      /* $660E INX */ s_x = update_nz(s_x + 1);
      /* $660F CPX */ update_nz_inv_c(s_x - peek(0x1410));
      /* $6612 BCC */ s_pc = !(s_status & STATUS_C) ? 0x65de : 0x6614;
      branchTarget = true;
      break;
    case 0x6614: // [$6614..$6616]    3 bytes
      CYCLES(0x6614, 6);
      /* $6614 PLA */ s_a = update_nz(pop8());
      /* $6615 TAX */ s_x = update_nz(s_a);
      /* $6616 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6617: // [$6617..$661B]    5 bytes
      CYCLES(0x6617, 9);
      /* $6617 TXA */ s_a = update_nz(s_x);
      /* $6618 PHA */ push8(s_a);
      /* $6619 DEC */ tmp16 = 0x140f, poke(tmp16, update_nz(peek(tmp16) - 1));
    case 0x661c: // [$661C..$663F]   36 bytes
      CYCLES(0x661c, 62);
      /* $661C LDA */ s_a = update_nz(peek(0x1a51 + s_x));
      /* $661F STA */ poke(0x1a50 + s_x, s_a);
      /* $6622 LDA */ s_a = update_nz(peek(0x1a61 + s_x));
      /* $6625 STA */ poke(0x1a60 + s_x, s_a);
      /* $6628 LDA */ s_a = update_nz(peek(0x1a71 + s_x));
      /* $662B STA */ poke(0x1a70 + s_x, s_a);
      /* $662E LDA */ s_a = update_nz(peek(0x1a81 + s_x));
      /* $6631 STA */ poke(0x1a80 + s_x, s_a);
      /* $6634 LDA */ s_a = update_nz(peek(0x1a91 + s_x));
      /* $6637 STA */ poke(0x1a90 + s_x, s_a);
      /* $663A INX */ s_x = update_nz(s_x + 1);
      /* $663B CPX */ update_nz_inv_c(s_x - peek(0x140f));
      /* $663E BCC */ s_pc = !(s_status & STATUS_C) ? 0x661c : 0x6640;
      branchTarget = true;
      break;
    case 0x6640: // [$6640..$6642]    3 bytes
      CYCLES(0x6640, 6);
      /* $6640 PLA */ s_a = update_nz(pop8());
      /* $6641 TAX */ s_x = update_nz(s_a);
      /* $6642 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6643: // [$6643..$664C]   10 bytes
      CYCLES(0x6643, 18);
      /* $6643 LDX */ s_x = update_nz(peek(0x1410));
      /* $6646 LDA */ s_a = update_nz(peek_zpg(0x1c));
      /* $6648 SEC */ s_status |= STATUS_C;
      /* $6649 SBC */ tmp = 0x07, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $664B BCS */ s_pc = s_status & STATUS_C ? 0x664f : 0x664d;
      branchTarget = true;
      break;
    case 0x664d: // [$664D..$664E]    2 bytes
      CYCLES(0x664d, 4);
      /* $664D LDA */ s_a = update_nz(0x00);
    case 0x664f: // [$664F..$664F]    1 bytes
      CYCLES(0x664f, 2);
      /* $664F TAY */ s_y = update_nz(s_a);
    case 0x6650: // [$6650..$6651]    2 bytes
      CYCLES(0x6650, 4);
      /* $6650 LDA */ s_a = update_nz(peek_zpg(0x1c));
    case 0x6652: // [$6652..$6654]    3 bytes
      CYCLES(0x6652, 6);
      /* $6652 DEX */ s_x = update_nz(s_x - 1);
      /* $6653 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6657 : 0x6655;
      branchTarget = true;
      break;
    case 0x6655: // [$6655..$6656]    2 bytes
      CYCLES(0x6655, 4);
      /* $6655 CLC */ s_status &= ~STATUS_C;
      /* $6656 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6657: // [$6657..$665B]    5 bytes
      CYCLES(0x6657, 9);
      /* $6657 CMP */ update_nz_inv_c(s_a - peek(0x1aa0 + s_x));
      /* $665A BCC */ s_pc = !(s_status & STATUS_C) ? 0x6652 : 0x665c;
      branchTarget = true;
      break;
    case 0x665c: // [$665C..$6661]    6 bytes
      CYCLES(0x665c, 11);
      /* $665C TYA */ s_a = update_nz(s_y);
      /* $665D CMP */ update_nz_inv_c(s_a - peek(0x1aa0 + s_x));
      /* $6660 BCS */ s_pc = s_status & STATUS_C ? 0x6650 : 0x6662;
      branchTarget = true;
      break;
    case 0x6662: // [$6662..$6668]    7 bytes
      CYCLES(0x6662, 12);
      /* $6662 LDA */ s_a = update_nz(peek_zpg(0x1d));
      /* $6664 CMP */ update_nz_inv_c(s_a - peek(0x1ab0 + s_x));
      /* $6667 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6650 : 0x6669;
      branchTarget = true;
      break;
    case 0x6669: // [$6669..$666C]    4 bytes
      CYCLES(0x6669, 7);
      /* $6669 SBC */ tmp = 0x07, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $666B BCC */ s_pc = !(s_status & STATUS_C) ? 0x6672 : 0x666d;
      branchTarget = true;
      break;
    case 0x666d: // [$666D..$6671]    5 bytes
      CYCLES(0x666d, 9);
      /* $666D CMP */ update_nz_inv_c(s_a - peek(0x1ab0 + s_x));
      /* $6670 BCS */ s_pc = s_status & STATUS_C ? 0x6650 : 0x6672;
      branchTarget = true;
      break;
    case 0x6672: // [$6672..$6675]    4 bytes
      CYCLES(0x6672, 7);
      /* $6672 LDA */ s_a = update_nz(peek_zpg(0xe8));
      /* $6674 BEQ */ s_pc = s_status & STATUS_Z ? 0x6678 : 0x6676;
      branchTarget = true;
      break;
    case 0x6676: // [$6676..$6677]    2 bytes
      CYCLES(0x6676, 4);
      /* $6676 SEC */ s_status |= STATUS_C;
      /* $6677 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6678: // [$6678..$6685]   14 bytes
      CYCLES(0x6678, 24);
      /* $6678 TXA */ s_a = update_nz(s_x);
      /* $6679 PHA */ push8(s_a);
      /* $667A TAY */ s_y = update_nz(s_a);
      /* $667B LDX */ s_x = update_nz(peek(0x1ab0 + s_y));
      /* $667E LDA */ s_a = update_nz(peek(0x1aa0 + s_y));
      /* $6681 LDY */ s_y = update_nz(0x07);
      /* $6683 JSR */ push16(0x6685), s_pc = 0x4cd0;
      branchTarget = true;
      break;
    case 0x6686: // [$6686..$668A]    5 bytes
      CYCLES(0x6686, 9);
      /* $6686 PLA */ s_a = update_nz(pop8());
      /* $6687 TAX */ s_x = update_nz(s_a);
      /* $6688 JSR */ push16(0x668a), s_pc = 0x65d9;
      branchTarget = true;
      break;
    case 0x668b: // [$668B..$669C]   18 bytes
      CYCLES(0x668b, 31);
      /* $668B LDA */ s_a = update_nz(peek(0x0c20));
      /* $668E STA */ poke(0x1413, s_a);
      /* $6691 LDA */ s_a = update_nz(0x01);
      /* $6693 STA */ poke(0x1412, s_a);
      /* $6696 LDA */ s_a = update_nz(0x00);
      /* $6698 LDX */ s_x = update_nz(0x19);
      /* $669A JSR */ push16(0x669c), s_pc = 0x4dd0;
      branchTarget = true;
      break;
    case 0x669d: // [$669D..$669E]    2 bytes
      CYCLES(0x669d, 4);
      /* $669D SEC */ s_status |= STATUS_C;
      /* $669E RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x669f: // [$669F..$66A8]   10 bytes
      CYCLES(0x669f, 18);
      /* $669F LDX */ s_x = update_nz(peek(0x140e));
      /* $66A2 LDA */ s_a = update_nz(peek_zpg(0x1c));
      /* $66A4 SEC */ s_status |= STATUS_C;
      /* $66A5 SBC */ tmp = 0x0d, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $66A7 BCS */ s_pc = s_status & STATUS_C ? 0x66ab : 0x66a9;
      branchTarget = true;
      break;
    case 0x66a9: // [$66A9..$66AA]    2 bytes
      CYCLES(0x66a9, 4);
      /* $66A9 LDA */ s_a = update_nz(0x00);
    case 0x66ab: // [$66AB..$66AB]    1 bytes
      CYCLES(0x66ab, 2);
      /* $66AB TAY */ s_y = update_nz(s_a);
    case 0x66ac: // [$66AC..$66AD]    2 bytes
      CYCLES(0x66ac, 4);
      /* $66AC LDA */ s_a = update_nz(peek_zpg(0x1c));
    case 0x66ae: // [$66AE..$66B0]    3 bytes
      CYCLES(0x66ae, 6);
      /* $66AE DEX */ s_x = update_nz(s_x - 1);
      /* $66AF BPL */ s_pc = !(s_status & STATUS_N) ? 0x66b3 : 0x66b1;
      branchTarget = true;
      break;
    case 0x66b1: // [$66B1..$66B2]    2 bytes
      CYCLES(0x66b1, 4);
      /* $66B1 CLC */ s_status &= ~STATUS_C;
      /* $66B2 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x66b3: // [$66B3..$66B7]    5 bytes
      CYCLES(0x66b3, 9);
      /* $66B3 CMP */ update_nz_inv_c(s_a - peek(0x19c0 + s_x));
      /* $66B6 BCC */ s_pc = !(s_status & STATUS_C) ? 0x66ae : 0x66b8;
      branchTarget = true;
      break;
    case 0x66b8: // [$66B8..$66BD]    6 bytes
      CYCLES(0x66b8, 11);
      /* $66B8 TYA */ s_a = update_nz(s_y);
      /* $66B9 CMP */ update_nz_inv_c(s_a - peek(0x19c0 + s_x));
      /* $66BC BCS */ s_pc = s_status & STATUS_C ? 0x66ac : 0x66be;
      branchTarget = true;
      break;
    case 0x66be: // [$66BE..$66C4]    7 bytes
      CYCLES(0x66be, 12);
      /* $66BE LDA */ s_a = update_nz(peek_zpg(0x1d));
      /* $66C0 CMP */ update_nz_inv_c(s_a - peek(0x19d0 + s_x));
      /* $66C3 BCC */ s_pc = !(s_status & STATUS_C) ? 0x66ac : 0x66c5;
      branchTarget = true;
      break;
    case 0x66c5: // [$66C5..$66C8]    4 bytes
      CYCLES(0x66c5, 7);
      /* $66C5 SBC */ tmp = 0x0d, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $66C7 BCC */ s_pc = !(s_status & STATUS_C) ? 0x66ce : 0x66c9;
      branchTarget = true;
      break;
    case 0x66c9: // [$66C9..$66CD]    5 bytes
      CYCLES(0x66c9, 9);
      /* $66C9 CMP */ update_nz_inv_c(s_a - peek(0x19d0 + s_x));
      /* $66CC BCS */ s_pc = s_status & STATUS_C ? 0x66ac : 0x66ce;
      branchTarget = true;
      break;
    case 0x66ce: // [$66CE..$66D1]    4 bytes
      CYCLES(0x66ce, 7);
      /* $66CE LDA */ s_a = update_nz(peek_zpg(0xe8));
      /* $66D0 BEQ */ s_pc = s_status & STATUS_Z ? 0x66d4 : 0x66d2;
      branchTarget = true;
      break;
    case 0x66d2: // [$66D2..$66D3]    2 bytes
      CYCLES(0x66d2, 4);
      /* $66D2 SEC */ s_status |= STATUS_C;
      /* $66D3 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x66d4: // [$66D4..$66E1]   14 bytes
      CYCLES(0x66d4, 24);
      /* $66D4 TXA */ s_a = update_nz(s_x);
      /* $66D5 PHA */ push8(s_a);
      /* $66D6 TAY */ s_y = update_nz(s_a);
      /* $66D7 LDX */ s_x = update_nz(peek(0x19d0 + s_y));
      /* $66DA LDA */ s_a = update_nz(peek(0x19c0 + s_y));
      /* $66DD LDY */ s_y = update_nz(0x0d);
      /* $66DF JSR */ push16(0x66e1), s_pc = 0x4d21;
      branchTarget = true;
      break;
    case 0x66e2: // [$66E2..$66F4]   19 bytes
      CYCLES(0x66e2, 33);
      /* $66E2 PLA */ s_a = update_nz(pop8());
      /* $66E3 PHA */ push8(s_a);
      /* $66E4 TAY */ s_y = update_nz(s_a);
      /* $66E5 LDA */ s_a = update_nz(peek(0x19c0 + s_y));
      /* $66E8 CLC */ s_status &= ~STATUS_C;
      /* $66E9 ADC */ tmp = 0x03, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $66EB TAX */ s_x = update_nz(s_a);
      /* $66EC LDA */ s_a = update_nz(peek(0x19d0 + s_y));
      /* $66EF TAY */ s_y = update_nz(s_a);
      /* $66F0 LDA */ s_a = update_nz(0x05);
      /* $66F2 JSR */ push16(0x66f4), s_pc = 0x5a27;
      branchTarget = true;
      break;
    case 0x66f5: // [$66F5..$66F9]    5 bytes
      CYCLES(0x66f5, 9);
      /* $66F5 PLA */ s_a = update_nz(pop8());
      /* $66F6 TAX */ s_x = update_nz(s_a);
      /* $66F7 JSR */ push16(0x66f9), s_pc = 0x63f2;
      branchTarget = true;
      break;
    case 0x66fa: // [$66FA..$6700]    7 bytes
      CYCLES(0x66fa, 12);
      /* $66FA LDA */ s_a = update_nz(0x03);
      /* $66FC LDX */ s_x = update_nz(0xe8);
      /* $66FE JSR */ push16(0x6700), s_pc = 0x4dd0;
      branchTarget = true;
      break;
    case 0x6701: // [$6701..$670D]   13 bytes
      CYCLES(0x6701, 23);
      /* $6701 LDA */ s_a = update_nz(peek(0x0c22));
      /* $6704 STA */ poke(0x1414, s_a);
      /* $6707 LDA */ s_a = update_nz(0x00);
      /* $6709 STA */ poke(0x1415, s_a);
      /* $670C SEC */ s_status |= STATUS_C;
      /* $670D RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x670e: // [$670E..$6717]   10 bytes
      CYCLES(0x670e, 18);
      /* $670E LDX */ s_x = update_nz(peek(0x140f));
      /* $6711 LDA */ s_a = update_nz(peek_zpg(0x1c));
      /* $6713 SEC */ s_status |= STATUS_C;
      /* $6714 SBC */ tmp = 0x0a, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6716 BCS */ s_pc = s_status & STATUS_C ? 0x671a : 0x6718;
      branchTarget = true;
      break;
    case 0x6718: // [$6718..$6719]    2 bytes
      CYCLES(0x6718, 4);
      /* $6718 LDA */ s_a = update_nz(0x00);
    case 0x671a: // [$671A..$671A]    1 bytes
      CYCLES(0x671a, 2);
      /* $671A TAY */ s_y = update_nz(s_a);
    case 0x671b: // [$671B..$671C]    2 bytes
      CYCLES(0x671b, 4);
      /* $671B LDA */ s_a = update_nz(peek_zpg(0x1c));
    case 0x671d: // [$671D..$671F]    3 bytes
      CYCLES(0x671d, 6);
      /* $671D DEX */ s_x = update_nz(s_x - 1);
      /* $671E BPL */ s_pc = !(s_status & STATUS_N) ? 0x6722 : 0x6720;
      branchTarget = true;
      break;
    case 0x6720: // [$6720..$6721]    2 bytes
      CYCLES(0x6720, 4);
      /* $6720 CLC */ s_status &= ~STATUS_C;
      /* $6721 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6722: // [$6722..$6726]    5 bytes
      CYCLES(0x6722, 9);
      /* $6722 CMP */ update_nz_inv_c(s_a - peek(0x1a50 + s_x));
      /* $6725 BCC */ s_pc = !(s_status & STATUS_C) ? 0x671d : 0x6727;
      branchTarget = true;
      break;
    case 0x6727: // [$6727..$672C]    6 bytes
      CYCLES(0x6727, 11);
      /* $6727 TYA */ s_a = update_nz(s_y);
      /* $6728 CMP */ update_nz_inv_c(s_a - peek(0x1a50 + s_x));
      /* $672B BEQ */ s_pc = s_status & STATUS_Z ? 0x672f : 0x672d;
      branchTarget = true;
      break;
    case 0x672d: // [$672D..$672E]    2 bytes
      CYCLES(0x672d, 4);
      /* $672D BCS */ s_pc = s_status & STATUS_C ? 0x671b : 0x672f;
      branchTarget = true;
      break;
    case 0x672f: // [$672F..$6735]    7 bytes
      CYCLES(0x672f, 12);
      /* $672F LDA */ s_a = update_nz(peek_zpg(0x1d));
      /* $6731 CMP */ update_nz_inv_c(s_a - peek(0x1a60 + s_x));
      /* $6734 BCC */ s_pc = !(s_status & STATUS_C) ? 0x671b : 0x6736;
      branchTarget = true;
      break;
    case 0x6736: // [$6736..$6739]    4 bytes
      CYCLES(0x6736, 7);
      /* $6736 SBC */ tmp = 0x0d, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6738 BCC */ s_pc = !(s_status & STATUS_C) ? 0x673f : 0x673a;
      branchTarget = true;
      break;
    case 0x673a: // [$673A..$673E]    5 bytes
      CYCLES(0x673a, 9);
      /* $673A CMP */ update_nz_inv_c(s_a - peek(0x1a60 + s_x));
      /* $673D BCS */ s_pc = s_status & STATUS_C ? 0x671b : 0x673f;
      branchTarget = true;
      break;
    case 0x673f: // [$673F..$6742]    4 bytes
      CYCLES(0x673f, 7);
      /* $673F LDA */ s_a = update_nz(peek_zpg(0xe8));
      /* $6741 BEQ */ s_pc = s_status & STATUS_Z ? 0x6745 : 0x6743;
      branchTarget = true;
      break;
    case 0x6743: // [$6743..$6744]    2 bytes
      CYCLES(0x6743, 4);
      /* $6743 SEC */ s_status |= STATUS_C;
      /* $6744 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6745: // [$6745..$6752]   14 bytes
      CYCLES(0x6745, 24);
      /* $6745 TXA */ s_a = update_nz(s_x);
      /* $6746 PHA */ push8(s_a);
      /* $6747 TAY */ s_y = update_nz(s_a);
      /* $6748 LDX */ s_x = update_nz(peek(0x1a60 + s_y));
      /* $674B LDA */ s_a = update_nz(peek(0x1a50 + s_y));
      /* $674E LDY */ s_y = update_nz(0x0d);
      /* $6750 JSR */ push16(0x6752), s_pc = 0x4d21;
      branchTarget = true;
      break;
    case 0x6753: // [$6753..$6768]   22 bytes
      CYCLES(0x6753, 38);
      /* $6753 PLA */ s_a = update_nz(pop8());
      /* $6754 TAX */ s_x = update_nz(s_a);
      /* $6755 PHA */ push8(s_a);
      /* $6756 LDA */ s_a = update_nz(peek(0x1a60 + s_x));
      /* $6759 CLC */ s_status &= ~STATUS_C;
      /* $675A ADC */ tmp = 0x06, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $675C TAY */ s_y = update_nz(s_a);
      /* $675D LDA */ s_a = update_nz(peek(0x1a50 + s_x));
      /* $6760 CLC */ s_status &= ~STATUS_C;
      /* $6761 ADC */ tmp = 0x03, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6763 TAX */ s_x = update_nz(s_a);
      /* $6764 LDA */ s_a = update_nz(0x55);
      /* $6766 JSR */ push16(0x6768), s_pc = 0x4f74;
      branchTarget = true;
      break;
    case 0x6769: // [$6769..$676D]    5 bytes
      CYCLES(0x6769, 9);
      /* $6769 PLA */ s_a = update_nz(pop8());
      /* $676A TAX */ s_x = update_nz(s_a);
      /* $676B JSR */ push16(0x676d), s_pc = 0x6617;
      branchTarget = true;
      break;
    case 0x676e: // [$676E..$6774]    7 bytes
      CYCLES(0x676e, 12);
      /* $676E LDA */ s_a = update_nz(0x00);
      /* $6770 LDX */ s_x = update_nz(0xc8);
      /* $6772 JSR */ push16(0x6774), s_pc = 0x4dd0;
      branchTarget = true;
      break;
    case 0x6775: // [$6775..$6788]   20 bytes
      CYCLES(0x6775, 35);
      /* $6775 LDA */ s_a = update_nz(peek(0x0c34));
      /* $6778 STA */ poke(0x1406, s_a);
      /* $677B LDA */ s_a = update_nz(peek(0x0c35));
      /* $677E STA */ poke(0x1422, s_a);
      /* $6781 LDA */ s_a = update_nz(peek(0x0c36));
      /* $6784 STA */ poke(0x1423, s_a);
      /* $6787 SEC */ s_status |= STATUS_C;
      /* $6788 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6789: // [$6789..$678D]    5 bytes
      CYCLES(0x6789, 9);
      /* $6789 DEC */ tmp16 = 0x1424, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $678C BMI */ s_pc = s_status & STATUS_N ? 0x678f : 0x678e;
      branchTarget = true;
      break;
    case 0x678e: // [$678E..$678E]    1 bytes
      CYCLES(0x678e, 2);
      /* $678E RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x678f: // [$678F..$6794]    6 bytes
      CYCLES(0x678f, 11);
      /* $678F LDA */ s_a = update_nz(peek(0x1425));
      /* $6792 JSR */ push16(0x6794), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x6795: // [$6795..$679D]    9 bytes
      CYCLES(0x6795, 16);
      /* $6795 STA */ poke(0x1424, s_a);
      /* $6798 LDA */ s_a = update_nz(peek(0x140f));
      /* $679B JSR */ push16(0x679d), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x679e: // [$679E..$67A1]    4 bytes
      CYCLES(0x679e, 7);
      /* $679E TAX */ s_x = update_nz(s_a);
      /* $679F JSR */ push16(0x67a1), s_pc = 0x64a8;
      branchTarget = true;
      break;
    case 0x67a2: // [$67A2..$67A7]    6 bytes
      CYCLES(0x67a2, 11);
      /* $67A2 LDA */ s_a = update_nz(peek(0x140f));
      /* $67A5 JSR */ push16(0x67a7), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x67a8: // [$67A8..$67AB]    4 bytes
      CYCLES(0x67a8, 7);
      /* $67A8 TAX */ s_x = update_nz(s_a);
      /* $67A9 JSR */ push16(0x67ab), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x67ac: // [$67AC..$67B0]    5 bytes
      CYCLES(0x67ac, 9);
      /* $67AC CMP */ update_nz_inv_c(s_a - peek(0x0c3a));
      /* $67AF BCC */ s_pc = !(s_status & STATUS_C) ? 0x67b8 : 0x67b1;
      branchTarget = true;
      break;
    case 0x67b1: // [$67B1..$67B5]    5 bytes
      CYCLES(0x67b1, 9);
      /* $67B1 CMP */ update_nz_inv_c(s_a - peek(0x0c3b));
      /* $67B4 BCC */ s_pc = !(s_status & STATUS_C) ? 0x67ee : 0x67b6;
      branchTarget = true;
      break;
    case 0x67b6: // [$67B6..$67B7]    2 bytes
      CYCLES(0x67b6, 4);
      /* $67B6 BCS */ s_pc = s_status & STATUS_C ? 0x67d1 : 0x67b8;
      branchTarget = true;
      break;
    case 0x67b8: // [$67B8..$67C0]    9 bytes
      CYCLES(0x67b8, 16);
      /* $67B8 LDA */ s_a = update_nz(peek(0x1a50 + s_x));
      /* $67BB LDY */ s_y = update_nz(peek(0x1500));
      /* $67BE JSR */ push16(0x67c0), s_pc = 0x6832;
      branchTarget = true;
      break;
    case 0x67c1: // [$67C1..$67CC]   12 bytes
      CYCLES(0x67c1, 21);
      /* $67C1 STA */ poke(0x1a80 + s_x, s_a);
      /* $67C4 LDA */ s_a = update_nz(peek(0x1a60 + s_x));
      /* $67C7 LDY */ s_y = update_nz(peek(0x1501));
      /* $67CA JSR */ push16(0x67cc), s_pc = 0x6832;
      branchTarget = true;
      break;
    case 0x67cd: // [$67CD..$67D0]    4 bytes
      CYCLES(0x67cd, 7);
      /* $67CD STA */ poke(0x1a90 + s_x, s_a);
      /* $67D0 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x67d1: // [$67D1..$67DA]   10 bytes
      CYCLES(0x67d1, 18);
      /* $67D1 TXA */ s_a = update_nz(s_x);
      /* $67D2 TAY */ s_y = update_nz(s_a);
      /* $67D3 LDA */ s_a = update_nz(peek(0x0c15));
      /* $67D6 ASL */ s_a = update_nzc(s_a << 1);
      /* $67D7 PHA */ push8(s_a);
      /* $67D8 JSR */ push16(0x67da), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x67db: // [$67DB..$67E5]   11 bytes
      CYCLES(0x67db, 19);
      /* $67DB SEC */ s_status |= STATUS_C;
      /* $67DC SBC */ tmp = peek(0x0c15), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $67DF STA */ poke(0x1a80 + s_y, s_a);
      /* $67E2 PLA */ s_a = update_nz(pop8());
      /* $67E3 JSR */ push16(0x67e5), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x67e6: // [$67E6..$67ED]    8 bytes
      CYCLES(0x67e6, 14);
      /* $67E6 SEC */ s_status |= STATUS_C;
      /* $67E7 SBC */ tmp = peek(0x0c15), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $67EA STA */ poke(0x1a90 + s_y, s_a);
      /* $67ED RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x67ee: // [$67EE..$6801]   20 bytes
      CYCLES(0x67ee, 35);
      /* $67EE LDA */ s_a = update_nz(peek(0x1a50 + s_x));
      /* $67F1 STA */ poke_zpg(0x02, s_a);
      /* $67F3 LDA */ s_a = update_nz(peek(0x1a60));
      /* $67F6 STA */ poke_zpg(0x03, s_a);
      /* $67F8 LDA */ s_a = update_nz(peek(0x1500));
      /* $67FB STA */ poke_zpg(0x04, s_a);
      /* $67FD LDA */ s_a = update_nz(peek(0x1501));
      /* $6800 STA */ poke_zpg(0x05, s_a);
    case 0x6802: // [$6802..$6810]   15 bytes
      CYCLES(0x6802, 26);
      /* $6802 LSR */ tmp16 = 0x02, tmp = peek_zpg(tmp16), set_c_to_bit0(tmp), poke_zpg(tmp16, update_nz(tmp >> 1));
      /* $6804 LSR */ tmp16 = 0x03, tmp = peek_zpg(tmp16), set_c_to_bit0(tmp), poke_zpg(tmp16, update_nz(tmp >> 1));
      /* $6806 LSR */ tmp16 = 0x04, tmp = peek_zpg(tmp16), set_c_to_bit0(tmp), poke_zpg(tmp16, update_nz(tmp >> 1));
      /* $6808 LSR */ tmp16 = 0x05, tmp = peek_zpg(tmp16), set_c_to_bit0(tmp), poke_zpg(tmp16, update_nz(tmp >> 1));
      /* $680A LDA */ s_a = update_nz(peek_zpg(0x02));
      /* $680C SBC */ tmp = peek_zpg(0x04), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $680E TAY */ s_y = update_nz(s_a);
      /* $680F BCS */ s_pc = s_status & STATUS_C ? 0x6813 : 0x6811;
      branchTarget = true;
      break;
    case 0x6811: // [$6811..$6812]    2 bytes
      CYCLES(0x6811, 4);
      /* $6811 EOR */ s_a = update_nz(s_a ^ 0xff);
    case 0x6813: // [$6813..$6817]    5 bytes
      CYCLES(0x6813, 9);
      /* $6813 CMP */ update_nz_inv_c(s_a - peek(0x0c15));
      /* $6816 BCS */ s_pc = s_status & STATUS_C ? 0x6802 : 0x6818;
      branchTarget = true;
      break;
    case 0x6818: // [$6818..$6820]    9 bytes
      CYCLES(0x6818, 16);
      /* $6818 STY */ poke_zpg(0x06, s_y);
      /* $681A LDA */ s_a = update_nz(peek_zpg(0x03));
      /* $681C SBC */ tmp = peek_zpg(0x05), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $681E TAY */ s_y = update_nz(s_a);
      /* $681F BCS */ s_pc = s_status & STATUS_C ? 0x6823 : 0x6821;
      branchTarget = true;
      break;
    case 0x6821: // [$6821..$6822]    2 bytes
      CYCLES(0x6821, 4);
      /* $6821 EOR */ s_a = update_nz(s_a ^ 0xff);
    case 0x6823: // [$6823..$6827]    5 bytes
      CYCLES(0x6823, 9);
      /* $6823 CMP */ update_nz_inv_c(s_a - peek(0x0c15));
      /* $6826 BCS */ s_pc = s_status & STATUS_C ? 0x6802 : 0x6828;
      branchTarget = true;
      break;
    case 0x6828: // [$6828..$6831]   10 bytes
      CYCLES(0x6828, 18);
      /* $6828 LDA */ s_a = update_nz(peek_zpg(0x06));
      /* $682A STA */ poke(0x1a80 + s_x, s_a);
      /* $682D TYA */ s_a = update_nz(s_y);
      /* $682E STA */ poke(0x1a90 + s_x, s_a);
      /* $6831 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6832: // [$6832..$6844]   19 bytes
      CYCLES(0x6832, 33);
      /* $6832 STA */ poke_zpg(0x03, s_a);
      /* $6834 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $6835 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $6836 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $6837 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $6838 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $6839 STA */ poke_zpg(0x02, s_a);
      /* $683B TYA */ s_a = update_nz(s_y);
      /* $683C LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $683D LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $683E LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $683F LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $6840 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $6841 SBC */ tmp = peek_zpg(0x02), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6843 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x684d : 0x6845;
      branchTarget = true;
      break;
    case 0x6845: // [$6845..$684A]    6 bytes
      CYCLES(0x6845, 11);
      /* $6845 LDA */ s_a = update_nz(0x01);
      /* $6847 CPY */ update_nz_inv_c(s_y - peek_zpg(0x03));
      /* $6849 BCS */ s_pc = s_status & STATUS_C ? 0x684d : 0x684b;
      branchTarget = true;
      break;
    case 0x684b: // [$684B..$684C]    2 bytes
      CYCLES(0x684b, 4);
      /* $684B LDA */ s_a = update_nz(0xff);
    case 0x684d: // [$684D..$684D]    1 bytes
      CYCLES(0x684d, 2);
      /* $684D RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x684e: // [$684E..$684F]    2 bytes
      CYCLES(0x684e, 4);
      /* $684E LDX */ s_x = update_nz(0x02);
    case 0x6850: // [$6850..$6857]    8 bytes
      CYCLES(0x6850, 14);
      /* $6850 LDA */ s_a = update_nz(peek(0x150a + s_x));
      /* $6853 CMP */ update_nz_inv_c(s_a - peek(0x1432 + s_x));
      /* $6856 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6869 : 0x6858;
      branchTarget = true;
      break;
    case 0x6858: // [$6858..$6859]    2 bytes
      CYCLES(0x6858, 4);
      /* $6858 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x6860 : 0x685a;
      branchTarget = true;
      break;
    case 0x685a: // [$685A..$685C]    3 bytes
      CYCLES(0x685a, 6);
      /* $685A DEX */ s_x = update_nz(s_x - 1);
      /* $685B BPL */ s_pc = !(s_status & STATUS_N) ? 0x6850 : 0x685d;
      branchTarget = true;
      break;
    case 0x685d: // [$685D..$685F]    3 bytes
      CYCLES(0x685d, 6);
      /* $685D JMP */ s_pc = 0x6869;
      branchTarget = true;
      break;
    case 0x6860: // [$6860..$6868]    9 bytes
      CYCLES(0x6860, 16);
      /* $6860 LDA */ s_a = update_nz(peek(0x150a + s_x));
      /* $6863 STA */ poke(0x1432 + s_x, s_a);
      /* $6866 DEX */ s_x = update_nz(s_x - 1);
      /* $6867 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6860 : 0x6869;
      branchTarget = true;
      break;
    case 0x6869: // [$6869..$6869]    1 bytes
      CYCLES(0x6869, 2);
      /* $6869 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x686a: // [$686A..$686C]    3 bytes
      CYCLES(0x686a, 6);
      /* $686A JSR */ push16(0x686c), s_pc = 0x51b8;
      branchTarget = true;
      break;
    case 0x686d: // [$686D..$6876]   10 bytes
      CYCLES(0x686d, 18);
      /* $686D ORA */ s_a = update_nz(s_a | peek(0x000c));
      /* $6870 PHA */ push8(s_a);
      /* $6871 EOR */ s_a = update_nz(s_a ^ 0x47);
      /* $6873 PHA */ push8(s_a);
      /* $6874 JSR */ push16(0x6876), s_pc = 0x4353;
      branchTarget = true;
      break;
    case 0x6877: // [$6877..$6877]    1 bytes
      CYCLES(0x6877, 2);
      /* $6877 ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x6877);
                      error_handler(s_pc);
      break;
    case 0x6900: // [$6900..$6902]    3 bytes
      CYCLES(0x6900, 6);
      /* $6900 JMP */ s_pc = 0x6915;
      branchTarget = true;
      break;
    case 0x6903: // [$6903..$6905]    3 bytes
      CYCLES(0x6903, 6);
      /* $6903 JMP */ s_pc = 0x6a0f;
      branchTarget = true;
      break;
    case 0x6906: // [$6906..$6908]    3 bytes
      CYCLES(0x6906, 6);
      /* $6906 JMP */ s_pc = 0x6b7b;
      branchTarget = true;
      break;
    case 0x6909: // [$6909..$690B]    3 bytes
      CYCLES(0x6909, 6);
      /* $6909 JMP */ s_pc = 0x6c67;
      branchTarget = true;
      break;
    case 0x690c: // [$690C..$690E]    3 bytes
      CYCLES(0x690c, 6);
      /* $690C JMP */ s_pc = 0x6e34;
      branchTarget = true;
      break;
    case 0x690f: // [$690F..$6911]    3 bytes
      CYCLES(0x690f, 6);
      /* $690F JMP */ s_pc = 0x6e98;
      branchTarget = true;
      break;
    case 0x6912: // [$6912..$6914]    3 bytes
      CYCLES(0x6912, 6);
      /* $6912 JMP */ s_pc = 0x6f13;
      branchTarget = true;
      break;
    case 0x6915: // [$6915..$691E]   10 bytes
      CYCLES(0x6915, 18);
      /* $6915 LDX */ s_x = update_nz(peek(0x0c4d));
      /* $6918 INX */ s_x = update_nz(s_x + 1);
      /* $6919 LDA */ s_a = update_nz(peek(0x1421));
      /* $691C JSR */ push16(0x691e), s_pc = 0x4c00;
      branchTarget = true;
      break;
    case 0x691f: // [$691F..$6923]    5 bytes
      CYCLES(0x691f, 9);
      /* $691F STA */ poke_zpg(0x08, s_a);
      /* $6921 LDY */ s_y = update_nz(peek(0x1427));
    case 0x6924: // [$6924..$6926]    3 bytes
      CYCLES(0x6924, 6);
      /* $6924 DEY */ s_y = update_nz(s_y - 1);
      /* $6925 BPL */ s_pc = !(s_status & STATUS_N) ? 0x692a : 0x6927;
      branchTarget = true;
      break;
    case 0x6927: // [$6927..$6929]    3 bytes
      CYCLES(0x6927, 6);
      /* $6927 JMP */ s_pc = 0x697d;
      branchTarget = true;
      break;
    case 0x692a: // [$692A..$6931]    8 bytes
      CYCLES(0x692a, 14);
      /* $692A LDA */ s_a = update_nz(0x02);
      /* $692C ASL */ tmp16 = 0x4e, poke_zpg(tmp16, update_nzc(peek_zpg(tmp16) << 1));
      /* $692E LDX */ s_x = update_nz(peek_zpg(0x4f));
      /* $6930 BCS */ s_pc = s_status & STATUS_C ? 0x6949 : 0x6932;
      branchTarget = true;
      break;
    case 0x6932: // [$6932..$6933]    2 bytes
      CYCLES(0x6932, 4);
      /* $6932 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6936 : 0x6934;
      branchTarget = true;
      break;
    case 0x6934: // [$6934..$6935]    2 bytes
      CYCLES(0x6934, 4);
      /* $6934 LDA */ s_a = update_nz(0xb2);
    case 0x6936: // [$6936..$693D]    8 bytes
      CYCLES(0x6936, 14);
      /* $6936 STA */ poke(0x1cf0 + s_y, s_a);
      /* $6939 LDA */ s_a = update_nz(0xf1);
      /* $693B JSR */ push16(0x693d), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x693e: // [$693E..$6948]   11 bytes
      CYCLES(0x693e, 19);
      /* $693E CLC */ s_status &= ~STATUS_C;
      /* $693F ADC */ tmp = 0x02, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6941 AND */ s_a = update_nz(s_a & 0xfe);
      /* $6943 STA */ poke(0x1ce0 + s_y, s_a);
      /* $6946 JMP */ s_pc = 0x6958;
      branchTarget = true;
      break;
    case 0x6949: // [$6949..$694A]    2 bytes
      CYCLES(0x6949, 4);
      /* $6949 BPL */ s_pc = !(s_status & STATUS_N) ? 0x694d : 0x694b;
      branchTarget = true;
      break;
    case 0x694b: // [$694B..$694C]    2 bytes
      CYCLES(0x694b, 4);
      /* $694B LDA */ s_a = update_nz(0xf2);
    case 0x694d: // [$694D..$6954]    8 bytes
      CYCLES(0x694d, 14);
      /* $694D STA */ poke(0x1ce0 + s_y, s_a);
      /* $6950 LDA */ s_a = update_nz(0xb3);
      /* $6952 JSR */ push16(0x6954), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x6955: // [$6955..$6957]    3 bytes
      CYCLES(0x6955, 6);
      /* $6955 STA */ poke(0x1cf0 + s_y, s_a);
    case 0x6958: // [$6958..$6973]   28 bytes
      CYCLES(0x6958, 48);
      /* $6958 LDA */ s_a = update_nz(0x00);
      /* $695A STA */ poke(0x1d00 + s_y, s_a);
      /* $695D STA */ poke(0x1d10 + s_y, s_a);
      /* $6960 STA */ poke(0x1d60 + s_y, s_a);
      /* $6963 TYA */ s_a = update_nz(s_y);
      /* $6964 AND */ s_a = update_nz(s_a & 0x01);
      /* $6966 STA */ poke(0x1d20 + s_y, s_a);
      /* $6969 LDA */ s_a = update_nz(peek_zpg(0x08));
      /* $696B STA */ poke(0x1d30 + s_y, s_a);
      /* $696E LDA */ s_a = update_nz(peek(0x0c52));
      /* $6971 JSR */ push16(0x6973), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x6974: // [$6974..$697C]    9 bytes
      CYCLES(0x6974, 16);
      /* $6974 CLC */ s_status &= ~STATUS_C;
      /* $6975 ADC */ tmp = peek(0x0c53), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6978 STA */ poke(0x1d50 + s_y, s_a);
      /* $697B BPL */ s_pc = !(s_status & STATUS_N) ? 0x6924 : 0x697d;
      branchTarget = true;
      break;
    case 0x697d: // [$697D..$6986]   10 bytes
      CYCLES(0x697d, 18);
      /* $697D LDA */ s_a = update_nz(peek(0x1421));
      /* $6980 LDX */ s_x = update_nz(peek(0x0c4e));
      /* $6983 INX */ s_x = update_nz(s_x + 1);
      /* $6984 JSR */ push16(0x6986), s_pc = 0x4c00;
      branchTarget = true;
      break;
    case 0x6987: // [$6987..$698B]    5 bytes
      CYCLES(0x6987, 9);
      /* $6987 STA */ poke_zpg(0x08, s_a);
      /* $6989 LDY */ s_y = update_nz(peek(0x1428));
    case 0x698c: // [$698C..$698E]    3 bytes
      CYCLES(0x698c, 6);
      /* $698C DEY */ s_y = update_nz(s_y - 1);
      /* $698D BPL */ s_pc = !(s_status & STATUS_N) ? 0x69a4 : 0x698f;
      branchTarget = true;
      break;
    case 0x698f: // [$698F..$69A3]   21 bytes
      CYCLES(0x698f, 36);
      /* $698F LDA */ s_a = update_nz(0x00);
      /* $6991 STA */ poke(0x1429, s_a);
      /* $6994 LDA */ s_a = update_nz(0xff);
      /* $6996 STA */ poke(0x142e, s_a);
      /* $6999 LDA */ s_a = update_nz(peek(0x1421));
      /* $699C CLC */ s_status &= ~STATUS_C;
      /* $699D ADC */ tmp = peek(0x0c59), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $69A0 STA */ poke(0x142a, s_a);
      /* $69A3 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x69a4: // [$69A4..$69A9]    6 bytes
      CYCLES(0x69a4, 11);
      /* $69A4 ASL */ tmp16 = 0x4e, poke_zpg(tmp16, update_nzc(peek_zpg(tmp16) << 1));
      /* $69A6 LDA */ s_a = update_nz(peek_zpg(0x4f));
      /* $69A8 BMI */ s_pc = s_status & STATUS_N ? 0x69cc : 0x69aa;
      branchTarget = true;
      break;
    case 0x69aa: // [$69AA..$69AD]    4 bytes
      CYCLES(0x69aa, 7);
      /* $69AA LDA */ s_a = update_nz(0x10);
      /* $69AC BCC */ s_pc = !(s_status & STATUS_C) ? 0x69b0 : 0x69ae;
      branchTarget = true;
      break;
    case 0x69ae: // [$69AE..$69AF]    2 bytes
      CYCLES(0x69ae, 4);
      /* $69AE LDA */ s_a = update_nz(0xa0);
    case 0x69b0: // [$69B0..$69B6]    7 bytes
      CYCLES(0x69b0, 12);
      /* $69B0 STA */ poke_zpg(0x06, s_a);
      /* $69B2 LDA */ s_a = update_nz(0x3e);
      /* $69B4 JSR */ push16(0x69b6), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x69b7: // [$69B7..$69C3]   13 bytes
      CYCLES(0x69b7, 23);
      /* $69B7 CLC */ s_status &= ~STATUS_C;
      /* $69B8 ADC */ tmp = peek_zpg(0x06), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $69BA ORA */ s_a = update_nz(s_a | 0x01);
      /* $69BC STA */ poke(0x1d70 + s_y, s_a);
      /* $69BF LDA */ s_a = update_nz(0x90);
      /* $69C1 JSR */ push16(0x69c3), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x69c4: // [$69C4..$69CB]    8 bytes
      CYCLES(0x69c4, 14);
      /* $69C4 CLC */ s_status &= ~STATUS_C;
      /* $69C5 ADC */ tmp = 0x10, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $69C7 STA */ poke(0x1d90 + s_y, s_a);
      /* $69CA BNE */ s_pc = !(s_status & STATUS_Z) ? 0x69ec : 0x69cc;
      branchTarget = true;
      break;
    case 0x69cc: // [$69CC..$69CF]    4 bytes
      CYCLES(0x69cc, 7);
      /* $69CC LDA */ s_a = update_nz(0x10);
      /* $69CE BCC */ s_pc = !(s_status & STATUS_C) ? 0x69d2 : 0x69d0;
      branchTarget = true;
      break;
    case 0x69d0: // [$69D0..$69D1]    2 bytes
      CYCLES(0x69d0, 4);
      /* $69D0 LDA */ s_a = update_nz(0x80);
    case 0x69d2: // [$69D2..$69D8]    7 bytes
      CYCLES(0x69d2, 12);
      /* $69D2 STA */ poke_zpg(0x06, s_a);
      /* $69D4 LDA */ s_a = update_nz(0x20);
      /* $69D6 JSR */ push16(0x69d8), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x69d9: // [$69D9..$69E3]   11 bytes
      CYCLES(0x69d9, 19);
      /* $69D9 CLC */ s_status &= ~STATUS_C;
      /* $69DA ADC */ tmp = peek_zpg(0x06), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $69DC STA */ poke(0x1d90 + s_y, s_a);
      /* $69DF LDA */ s_a = update_nz(0xce);
      /* $69E1 JSR */ push16(0x69e3), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x69e4: // [$69E4..$69EB]    8 bytes
      CYCLES(0x69e4, 14);
      /* $69E4 CLC */ s_status &= ~STATUS_C;
      /* $69E5 ADC */ tmp = 0x10, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $69E7 ORA */ s_a = update_nz(s_a | 0x01);
      /* $69E9 STA */ poke(0x1d70 + s_y, s_a);
    case 0x69ec: // [$69EC..$69FD]   18 bytes
      CYCLES(0x69ec, 31);
      /* $69EC LDA */ s_a = update_nz(0x00);
      /* $69EE STA */ poke(0x1db0 + s_y, s_a);
      /* $69F1 STA */ poke(0x1dd0 + s_y, s_a);
      /* $69F4 LDA */ s_a = update_nz(peek_zpg(0x08));
      /* $69F6 STA */ poke(0x1e10 + s_y, s_a);
      /* $69F9 LDX */ s_x = update_nz(0x00);
      /* $69FB JSR */ push16(0x69fd), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x69fe: // [$69FE..$69FF]    2 bytes
      CYCLES(0x69fe, 4);
      /* $69FE BPL */ s_pc = !(s_status & STATUS_N) ? 0x6a02 : 0x6a00;
      branchTarget = true;
      break;
    case 0x6a00: // [$6A00..$6A01]    2 bytes
      CYCLES(0x6a00, 4);
      /* $6A00 LDX */ s_x = update_nz(0x03);
    case 0x6a02: // [$6A02..$6A0E]   13 bytes
      CYCLES(0x6a02, 23);
      /* $6A02 TXA */ s_a = update_nz(s_x);
      /* $6A03 STA */ poke(0x1e30 + s_y, s_a);
      /* $6A06 TYA */ s_a = update_nz(s_y);
      /* $6A07 AND */ s_a = update_nz(s_a & 0x03);
      /* $6A09 STA */ poke(0x1df0 + s_y, s_a);
      /* $6A0C JMP */ s_pc = 0x698c;
      branchTarget = true;
      break;
    case 0x6a0f: // [$6A0F..$6A11]    3 bytes
      CYCLES(0x6a0f, 6);
      /* $6A0F LDX */ s_x = update_nz(peek(0x1427));
    case 0x6a12: // [$6A12..$6A14]    3 bytes
      CYCLES(0x6a12, 6);
      /* $6A12 DEX */ s_x = update_nz(s_x - 1);
      /* $6A13 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6a16 : 0x6a15;
      branchTarget = true;
      break;
    case 0x6a15: // [$6A15..$6A15]    1 bytes
      CYCLES(0x6a15, 2);
      /* $6A15 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6a16: // [$6A16..$6A1A]    5 bytes
      CYCLES(0x6a16, 9);
      /* $6A16 DEC */ tmp16 = 0x1d20 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $6A19 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6a12 : 0x6a1b;
      branchTarget = true;
      break;
    case 0x6a1b: // [$6A1B..$6A2F]   21 bytes
      CYCLES(0x6a1b, 36);
      /* $6A1B LDA */ s_a = update_nz(peek(0x0c4d));
      /* $6A1E STA */ poke(0x1d20 + s_x, s_a);
      /* $6A21 STX */ poke_zpg(0x00, s_x);
      /* $6A23 LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $6A25 LDX */ s_x = update_nz(peek(0x1cf0 + s_y));
      /* $6A28 LDA */ s_a = update_nz(peek(0x1ce0 + s_y));
      /* $6A2B LDY */ s_y = update_nz(0x0d);
      /* $6A2D JSR */ push16(0x6a2f), s_pc = 0x4d21;
      branchTarget = true;
      break;
    case 0x6a30: // [$6A30..$6A3A]   11 bytes
      CYCLES(0x6a30, 19);
      /* $6A30 LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $6A32 LDA */ s_a = update_nz(peek(0x1ce0 + s_y));
      /* $6A35 CLC */ s_status &= ~STATUS_C;
      /* $6A36 ADC */ tmp = peek(0x1d00 + s_y), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6A39 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x6a3d : 0x6a3b;
      branchTarget = true;
      break;
    case 0x6a3b: // [$6A3B..$6A3C]    2 bytes
      CYCLES(0x6a3b, 4);
      /* $6A3B LDA */ s_a = update_nz(0x02);
    case 0x6a3d: // [$6A3D..$6A40]    4 bytes
      CYCLES(0x6a3d, 7);
      /* $6A3D CMP */ update_nz_inv_c(s_a - 0xf3);
      /* $6A3F BCC */ s_pc = !(s_status & STATUS_C) ? 0x6a51 : 0x6a41;
      branchTarget = true;
      break;
    case 0x6a41: // [$6A41..$6A47]    7 bytes
      CYCLES(0x6a41, 12);
      /* $6A41 LDA */ s_a = update_nz(0xf2);
      /* $6A43 LDX */ s_x = update_nz(peek(0x1d00 + s_y));
      /* $6A46 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6a4a : 0x6a48;
      branchTarget = true;
      break;
    case 0x6a48: // [$6A48..$6A49]    2 bytes
      CYCLES(0x6a48, 4);
      /* $6A48 LDA */ s_a = update_nz(0x02);
    case 0x6a4a: // [$6A4A..$6A50]    7 bytes
      CYCLES(0x6a4a, 12);
      /* $6A4A PHA */ push8(s_a);
      /* $6A4B LDA */ s_a = update_nz(0x00);
      /* $6A4D STA */ poke(0x1d00 + s_y, s_a);
      /* $6A50 PLA */ s_a = update_nz(pop8());
    case 0x6a51: // [$6A51..$6A5E]   14 bytes
      CYCLES(0x6a51, 24);
      /* $6A51 STA */ poke(0x1ce0 + s_y, s_a);
      /* $6A54 LDA */ s_a = update_nz(peek(0x1cf0 + s_y));
      /* $6A57 CLC */ s_status &= ~STATUS_C;
      /* $6A58 ADC */ tmp = peek(0x1d10 + s_y), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6A5B CMP */ update_nz_inv_c(s_a - 0xb3);
      /* $6A5D BCC */ s_pc = !(s_status & STATUS_C) ? 0x6a6f : 0x6a5f;
      branchTarget = true;
      break;
    case 0x6a5f: // [$6A5F..$6A65]    7 bytes
      CYCLES(0x6a5f, 12);
      /* $6A5F LDA */ s_a = update_nz(0xb2);
      /* $6A61 LDX */ s_x = update_nz(peek(0x1d10 + s_y));
      /* $6A64 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6a68 : 0x6a66;
      branchTarget = true;
      break;
    case 0x6a66: // [$6A66..$6A67]    2 bytes
      CYCLES(0x6a66, 4);
      /* $6A66 LDA */ s_a = update_nz(0x00);
    case 0x6a68: // [$6A68..$6A6E]    7 bytes
      CYCLES(0x6a68, 12);
      /* $6A68 PHA */ push8(s_a);
      /* $6A69 LDA */ s_a = update_nz(0x00);
      /* $6A6B STA */ poke(0x1d10 + s_y, s_a);
      /* $6A6E PLA */ s_a = update_nz(pop8());
    case 0x6a6f: // [$6A6F..$6A8B]   29 bytes
      CYCLES(0x6a6f, 50);
      /* $6A6F STA */ poke(0x1cf0 + s_y, s_a);
      /* $6A72 LDA */ s_a = update_nz(peek(0x1d30 + s_y));
      /* $6A75 AND */ s_a = update_nz(s_a & 0x03);
      /* $6A77 TAX */ s_x = update_nz(s_a);
      /* $6A78 LDA */ s_a = update_nz(peek(0x0910 + s_x));
      /* $6A7B STA */ poke_zpg(0x06, s_a);
      /* $6A7D LDA */ s_a = update_nz(peek(0x0918 + s_x));
      /* $6A80 STA */ poke_zpg(0x07, s_a);
      /* $6A82 LDX */ s_x = update_nz(peek(0x1ce0 + s_y));
      /* $6A85 LDA */ s_a = update_nz(peek(0x1cf0 + s_y));
      /* $6A88 TAY */ s_y = update_nz(s_a);
      /* $6A89 JSR */ push16(0x6a8b), s_pc = 0x4cf6;
      branchTarget = true;
      break;
    case 0x6a8c: // [$6A8C..$6A92]    7 bytes
      CYCLES(0x6a8c, 12);
      /* $6A8C LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $6A8E DEC */ tmp16 = 0x1d30 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $6A91 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6ad2 : 0x6a93;
      branchTarget = true;
      break;
    case 0x6a93: // [$6A93..$6A98]    6 bytes
      CYCLES(0x6a93, 11);
      /* $6A93 LDA */ s_a = update_nz(peek(0x0c4a));
      /* $6A96 JSR */ push16(0x6a98), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x6a99: // [$6A99..$6AA8]   16 bytes
      CYCLES(0x6a99, 28);
      /* $6A99 ORA */ s_a = update_nz(s_a | 0x03);
      /* $6A9B LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $6A9D STA */ poke(0x1d30 + s_x, s_a);
      /* $6AA0 LDA */ s_a = update_nz(peek(0x1ce0 + s_x));
      /* $6AA3 LDY */ s_y = update_nz(peek(0x1500));
      /* $6AA6 JSR */ push16(0x6aa8), s_pc = 0x6b3a;
      branchTarget = true;
      break;
    case 0x6aa9: // [$6AA9..$6AB4]   12 bytes
      CYCLES(0x6aa9, 21);
      /* $6AA9 STA */ poke(0x1d00 + s_x, s_a);
      /* $6AAC LDA */ s_a = update_nz(peek(0x1cf0 + s_x));
      /* $6AAF LDY */ s_y = update_nz(peek(0x1501));
      /* $6AB2 JSR */ push16(0x6ab4), s_pc = 0x6b3a;
      branchTarget = true;
      break;
    case 0x6ab5: // [$6AB5..$6AC1]   13 bytes
      CYCLES(0x6ab5, 23);
      /* $6AB5 STA */ poke(0x1d10 + s_x, s_a);
      /* $6AB8 LDA */ s_a = update_nz(0x00);
      /* $6ABA STA */ poke(0x1d60 + s_x, s_a);
      /* $6ABD DEC */ tmp16 = 0x1d50 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $6AC0 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6ad2 : 0x6ac2;
      branchTarget = true;
      break;
    case 0x6ac2: // [$6AC2..$6AC7]    6 bytes
      CYCLES(0x6ac2, 11);
      /* $6AC2 LDA */ s_a = update_nz(peek(0x0c52));
      /* $6AC5 JSR */ push16(0x6ac7), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x6ac8: // [$6AC8..$6AD1]   10 bytes
      CYCLES(0x6ac8, 18);
      /* $6AC8 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $6ACA STA */ poke(0x1d50 + s_x, s_a);
      /* $6ACD LDA */ s_a = update_nz(0x01);
      /* $6ACF STA */ poke(0x1d60 + s_x, s_a);
    case 0x6ad2: // [$6AD2..$6AD6]    5 bytes
      CYCLES(0x6ad2, 9);
      /* $6AD2 LDA */ s_a = update_nz(peek(0x1d60 + s_x));
      /* $6AD5 BEQ */ s_pc = s_status & STATUS_Z ? 0x6b37 : 0x6ad7;
      branchTarget = true;
      break;
    case 0x6ad7: // [$6AD7..$6ADD]    7 bytes
      CYCLES(0x6ad7, 12);
      /* $6AD7 LDA */ s_a = update_nz(peek(0x1d30 + s_x));
      /* $6ADA AND */ s_a = update_nz(s_a & 0x03);
      /* $6ADC BNE */ s_pc = !(s_status & STATUS_Z) ? 0x6b37 : 0x6ade;
      branchTarget = true;
      break;
    case 0x6ade: // [$6ADE..$6AE5]    8 bytes
      CYCLES(0x6ade, 14);
      /* $6ADE LDY */ s_y = update_nz(peek(0x1428));
      /* $6AE1 CPY */ update_nz_inv_c(s_y - peek(0x0c58));
      /* $6AE4 BCS */ s_pc = s_status & STATUS_C ? 0x6b37 : 0x6ae6;
      branchTarget = true;
      break;
    case 0x6ae6: // [$6AE6..$6AEE]    9 bytes
      CYCLES(0x6ae6, 16);
      /* $6AE6 LDA */ s_a = update_nz(peek(0x1ce0 + s_x));
      /* $6AE9 ORA */ s_a = update_nz(s_a | 0x01);
      /* $6AEB CMP */ update_nz_inv_c(s_a - 0x11);
      /* $6AED BCS */ s_pc = s_status & STATUS_C ? 0x6af1 : 0x6aef;
      branchTarget = true;
      break;
    case 0x6aef: // [$6AEF..$6AF0]    2 bytes
      CYCLES(0x6aef, 4);
      /* $6AEF LDA */ s_a = update_nz(0x11);
    case 0x6af1: // [$6AF1..$6AF4]    4 bytes
      CYCLES(0x6af1, 7);
      /* $6AF1 CMP */ update_nz_inv_c(s_a - 0xe0);
      /* $6AF3 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6af7 : 0x6af5;
      branchTarget = true;
      break;
    case 0x6af5: // [$6AF5..$6AF6]    2 bytes
      CYCLES(0x6af5, 4);
      /* $6AF5 LDA */ s_a = update_nz(0xdf);
    case 0x6af7: // [$6AF7..$6B00]   10 bytes
      CYCLES(0x6af7, 18);
      /* $6AF7 STA */ poke(0x1d70 + s_y, s_a);
      /* $6AFA LDA */ s_a = update_nz(peek(0x1cf0 + s_x));
      /* $6AFD CMP */ update_nz_inv_c(s_a - 0x11);
      /* $6AFF BCS */ s_pc = s_status & STATUS_C ? 0x6b03 : 0x6b01;
      branchTarget = true;
      break;
    case 0x6b01: // [$6B01..$6B02]    2 bytes
      CYCLES(0x6b01, 4);
      /* $6B01 LDA */ s_a = update_nz(0x11);
    case 0x6b03: // [$6B03..$6B06]    4 bytes
      CYCLES(0x6b03, 7);
      /* $6B03 CMP */ update_nz_inv_c(s_a - 0xa0);
      /* $6B05 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6b09 : 0x6b07;
      branchTarget = true;
      break;
    case 0x6b07: // [$6B07..$6B08]    2 bytes
      CYCLES(0x6b07, 4);
      /* $6B07 LDA */ s_a = update_nz(0x9f);
    case 0x6b09: // [$6B09..$6B24]   28 bytes
      CYCLES(0x6b09, 48);
      /* $6B09 STA */ poke(0x1d90 + s_y, s_a);
      /* $6B0C LDA */ s_a = update_nz(0x00);
      /* $6B0E STA */ poke(0x1db0 + s_y, s_a);
      /* $6B11 STA */ poke(0x1dd0 + s_y, s_a);
      /* $6B14 STA */ poke(0x1df0 + s_y, s_a);
      /* $6B17 STA */ poke(0x1e10 + s_y, s_a);
      /* $6B1A STA */ poke(0x1e30 + s_y, s_a);
      /* $6B1D INC */ tmp16 = 0x1428, poke(tmp16, update_nz(peek(tmp16) + 1));
      /* $6B20 DEC */ tmp16 = 0x1d40 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $6B23 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6b37 : 0x6b25;
      branchTarget = true;
      break;
    case 0x6b25: // [$6B25..$6B31]   13 bytes
      CYCLES(0x6b25, 23);
      /* $6B25 LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $6B27 LDX */ s_x = update_nz(peek(0x1cf0 + s_y));
      /* $6B2A LDA */ s_a = update_nz(peek(0x1ce0 + s_y));
      /* $6B2D LDY */ s_y = update_nz(0x0d);
      /* $6B2F JSR */ push16(0x6b31), s_pc = 0x4d21;
      branchTarget = true;
      break;
    case 0x6b32: // [$6B32..$6B36]    5 bytes
      CYCLES(0x6b32, 9);
      /* $6B32 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $6B34 JSR */ push16(0x6b36), s_pc = 0x6df0;
      branchTarget = true;
      break;
    case 0x6b37: // [$6B37..$6B39]    3 bytes
      CYCLES(0x6b37, 6);
      /* $6B37 JMP */ s_pc = 0x6a12;
      branchTarget = true;
      break;
    case 0x6b3a: // [$6B3A..$6B41]    8 bytes
      CYCLES(0x6b3a, 14);
      /* $6B3A STA */ poke_zpg(0x06, s_a);
      /* $6B3C STY */ poke_zpg(0x07, s_y);
      /* $6B3E SBC */ tmp = peek_zpg(0x07), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6B40 BCS */ s_pc = s_status & STATUS_C ? 0x6b44 : 0x6b42;
      branchTarget = true;
      break;
    case 0x6b42: // [$6B42..$6B43]    2 bytes
      CYCLES(0x6b42, 4);
      /* $6B42 EOR */ s_a = update_nz(s_a ^ 0xff);
    case 0x6b44: // [$6B44..$6B4A]    7 bytes
      CYCLES(0x6b44, 12);
      /* $6B44 CMP */ update_nz_inv_c(s_a - peek(0x0c50));
      /* $6B47 PHP */ push8(s_status | STATUS_B);
      /* $6B48 JSR */ push16(0x6b4a), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x6b4b: // [$6B4B..$6B52]    8 bytes
      CYCLES(0x6b4b, 14);
      /* $6B4B LDA */ s_a = update_nz(0x00);
      /* $6B4D LDY */ s_y = update_nz(peek_zpg(0x4e));
      /* $6B4F CPY */ update_nz_inv_c(s_y - 0x60);
      /* $6B51 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6b5e : 0x6b53;
      branchTarget = true;
      break;
    case 0x6b53: // [$6B53..$6B59]    7 bytes
      CYCLES(0x6b53, 12);
      /* $6B53 LDA */ s_a = update_nz(peek(0x0c43));
      /* $6B56 CPY */ update_nz_inv_c(s_y - 0xb0);
      /* $6B58 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6b5e : 0x6b5a;
      branchTarget = true;
      break;
    case 0x6b5a: // [$6B5A..$6B5D]    4 bytes
      CYCLES(0x6b5a, 7);
      /* $6B5A EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $6B5C ADC */ tmp = 0x00, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0x6b5e: // [$6B5E..$6B60]    3 bytes
      CYCLES(0x6b5e, 6);
      /* $6B5E PLP */ s_status = pop8() & ~STATUS_B;
      /* $6B5F BCS */ s_pc = s_status & STATUS_C ? 0x6b7a : 0x6b61;
      branchTarget = true;
      break;
    case 0x6b61: // [$6B61..$6B67]    7 bytes
      CYCLES(0x6b61, 12);
      /* $6B61 LDY */ s_y = update_nz(peek_zpg(0x4f));
      /* $6B63 CPY */ update_nz_inv_c(s_y - peek(0x0c56));
      /* $6B66 BCS */ s_pc = s_status & STATUS_C ? 0x6b7a : 0x6b68;
      branchTarget = true;
      break;
    case 0x6b68: // [$6B68..$6B6E]    7 bytes
      CYCLES(0x6b68, 12);
      /* $6B68 LDY */ s_y = update_nz(peek_zpg(0x06));
      /* $6B6A CPY */ update_nz_inv_c(s_y - peek_zpg(0x07));
      /* $6B6C TAY */ s_y = update_nz(s_a);
      /* $6B6D BCS */ s_pc = s_status & STATUS_C ? 0x6b73 : 0x6b6f;
      branchTarget = true;
      break;
    case 0x6b6f: // [$6B6F..$6B70]    2 bytes
      CYCLES(0x6b6f, 4);
      /* $6B6F BPL */ s_pc = !(s_status & STATUS_N) ? 0x6b7a : 0x6b71;
      branchTarget = true;
      break;
    case 0x6b71: // [$6B71..$6B72]    2 bytes
      CYCLES(0x6b71, 4);
      /* $6B71 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6b76 : 0x6b73;
      branchTarget = true;
      break;
    case 0x6b73: // [$6B73..$6B74]    2 bytes
      CYCLES(0x6b73, 4);
      /* $6B73 BMI */ s_pc = s_status & STATUS_N ? 0x6b7a : 0x6b75;
      branchTarget = true;
      break;
    case 0x6b75: // [$6B75..$6B75]    1 bytes
      CYCLES(0x6b75, 2);
      /* $6B75 CLC */ s_status &= ~STATUS_C;
    case 0x6b76: // [$6B76..$6B79]    4 bytes
      CYCLES(0x6b76, 7);
      /* $6B76 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $6B78 ADC */ tmp = 0x01, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0x6b7a: // [$6B7A..$6B7A]    1 bytes
      CYCLES(0x6b7a, 2);
      /* $6B7A RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6b7b: // [$6B7B..$6B7D]    3 bytes
      CYCLES(0x6b7b, 6);
      /* $6B7B LDX */ s_x = update_nz(peek(0x1428));
    case 0x6b7e: // [$6B7E..$6B80]    3 bytes
      CYCLES(0x6b7e, 6);
      /* $6B7E DEX */ s_x = update_nz(s_x - 1);
      /* $6B7F BPL */ s_pc = !(s_status & STATUS_N) ? 0x6b82 : 0x6b81;
      branchTarget = true;
      break;
    case 0x6b81: // [$6B81..$6B81]    1 bytes
      CYCLES(0x6b81, 2);
      /* $6B81 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6b82: // [$6B82..$6B86]    5 bytes
      CYCLES(0x6b82, 9);
      /* $6B82 DEC */ tmp16 = 0x1df0 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $6B85 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6b7e : 0x6b87;
      branchTarget = true;
      break;
    case 0x6b87: // [$6B87..$6B9B]   21 bytes
      CYCLES(0x6b87, 36);
      /* $6B87 STX */ poke_zpg(0x00, s_x);
      /* $6B89 LDA */ s_a = update_nz(peek(0x0c4e));
      /* $6B8C STA */ poke(0x1df0 + s_x, s_a);
      /* $6B8F LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $6B91 LDX */ s_x = update_nz(peek(0x1d90 + s_y));
      /* $6B94 LDA */ s_a = update_nz(peek(0x1d70 + s_y));
      /* $6B97 LDY */ s_y = update_nz(0x0f);
      /* $6B99 JSR */ push16(0x6b9b), s_pc = 0x4d21;
      branchTarget = true;
      break;
    case 0x6b9c: // [$6B9C..$6B9D]    2 bytes
      CYCLES(0x6b9c, 4);
      /* $6B9C LDX */ s_x = update_nz(peek_zpg(0x00));
    case 0x6b9e: // [$6B9E..$6BA8]   11 bytes
      CYCLES(0x6b9e, 19);
      /* $6B9E LDA */ s_a = update_nz(peek(0x1d70 + s_x));
      /* $6BA1 CLC */ s_status &= ~STATUS_C;
      /* $6BA2 ADC */ tmp = peek(0x1db0 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6BA5 CMP */ update_nz_inv_c(s_a - 0x08);
      /* $6BA7 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6bad : 0x6ba9;
      branchTarget = true;
      break;
    case 0x6ba9: // [$6BA9..$6BAC]    4 bytes
      CYCLES(0x6ba9, 7);
      /* $6BA9 CMP */ update_nz_inv_c(s_a - 0xe8);
      /* $6BAB BCC */ s_pc = !(s_status & STATUS_C) ? 0x6bbd : 0x6bad;
      branchTarget = true;
      break;
    case 0x6bad: // [$6BAD..$6BBA]   14 bytes
      CYCLES(0x6bad, 24);
      /* $6BAD LDA */ s_a = update_nz(peek(0x1db0 + s_x));
      /* $6BB0 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $6BB2 CLC */ s_status &= ~STATUS_C;
      /* $6BB3 ADC */ tmp = 0x01, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6BB5 STA */ poke(0x1db0 + s_x, s_a);
      /* $6BB8 JSR */ push16(0x6bba), s_pc = 0x6c2f;
      branchTarget = true;
      break;
    case 0x6bbb: // [$6BBB..$6BBC]    2 bytes
      CYCLES(0x6bbb, 4);
      /* $6BBB BPL */ s_pc = !(s_status & STATUS_N) ? 0x6b9e : 0x6bbd;
      branchTarget = true;
      break;
    case 0x6bbd: // [$6BBD..$6BBF]    3 bytes
      CYCLES(0x6bbd, 6);
      /* $6BBD STA */ poke(0x1d70 + s_x, s_a);
    case 0x6bc0: // [$6BC0..$6BCA]   11 bytes
      CYCLES(0x6bc0, 19);
      /* $6BC0 LDA */ s_a = update_nz(peek(0x1d90 + s_x));
      /* $6BC3 CLC */ s_status &= ~STATUS_C;
      /* $6BC4 ADC */ tmp = peek(0x1dd0 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6BC7 CMP */ update_nz_inv_c(s_a - 0x08);
      /* $6BC9 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6bcf : 0x6bcb;
      branchTarget = true;
      break;
    case 0x6bcb: // [$6BCB..$6BCE]    4 bytes
      CYCLES(0x6bcb, 7);
      /* $6BCB CMP */ update_nz_inv_c(s_a - 0xa8);
      /* $6BCD BCC */ s_pc = !(s_status & STATUS_C) ? 0x6bdf : 0x6bcf;
      branchTarget = true;
      break;
    case 0x6bcf: // [$6BCF..$6BDC]   14 bytes
      CYCLES(0x6bcf, 24);
      /* $6BCF LDA */ s_a = update_nz(peek(0x1dd0 + s_x));
      /* $6BD2 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $6BD4 CLC */ s_status &= ~STATUS_C;
      /* $6BD5 ADC */ tmp = 0x01, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6BD7 STA */ poke(0x1dd0 + s_x, s_a);
      /* $6BDA JSR */ push16(0x6bdc), s_pc = 0x6c2f;
      branchTarget = true;
      break;
    case 0x6bdd: // [$6BDD..$6BDE]    2 bytes
      CYCLES(0x6bdd, 4);
      /* $6BDD BPL */ s_pc = !(s_status & STATUS_N) ? 0x6bc0 : 0x6bdf;
      branchTarget = true;
      break;
    case 0x6bdf: // [$6BDF..$6BFE]   32 bytes
      CYCLES(0x6bdf, 55);
      /* $6BDF STA */ poke(0x1d90 + s_x, s_a);
      /* $6BE2 LDA */ s_a = update_nz(peek(0x1e10 + s_x));
      /* $6BE5 AND */ s_a = update_nz(s_a & 0x03);
      /* $6BE7 EOR */ s_a = update_nz(s_a ^ peek(0x1e30 + s_x));
      /* $6BEA TAY */ s_y = update_nz(s_a);
      /* $6BEB LDA */ s_a = update_nz(peek(0x0920 + s_y));
      /* $6BEE STA */ poke_zpg(0x06, s_a);
      /* $6BF0 LDA */ s_a = update_nz(peek(0x0928 + s_y));
      /* $6BF3 STA */ poke_zpg(0x07, s_a);
      /* $6BF5 LDY */ s_y = update_nz(peek(0x1d90 + s_x));
      /* $6BF8 LDA */ s_a = update_nz(peek(0x1d70 + s_x));
      /* $6BFB TAX */ s_x = update_nz(s_a);
      /* $6BFC JSR */ push16(0x6bfe), s_pc = 0x4cf6;
      branchTarget = true;
      break;
    case 0x6bff: // [$6BFF..$6C05]    7 bytes
      CYCLES(0x6bff, 12);
      /* $6BFF LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $6C01 DEC */ tmp16 = 0x1e10 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $6C04 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6c2c : 0x6c06;
      branchTarget = true;
      break;
    case 0x6c06: // [$6C06..$6C0B]    6 bytes
      CYCLES(0x6c06, 11);
      /* $6C06 LDA */ s_a = update_nz(peek(0x0c4b));
      /* $6C09 JSR */ push16(0x6c0b), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x6c0c: // [$6C0C..$6C19]   14 bytes
      CYCLES(0x6c0c, 24);
      /* $6C0C LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $6C0E STA */ poke(0x1e10 + s_x, s_a);
      /* $6C11 LDA */ s_a = update_nz(peek(0x1d70 + s_x));
      /* $6C14 LDY */ s_y = update_nz(peek(0x1500));
      /* $6C17 JSR */ push16(0x6c19), s_pc = 0x6c38;
      branchTarget = true;
      break;
    case 0x6c1a: // [$6C1A..$6C25]   12 bytes
      CYCLES(0x6c1a, 21);
      /* $6C1A STA */ poke(0x1db0 + s_x, s_a);
      /* $6C1D LDA */ s_a = update_nz(peek(0x1d90 + s_x));
      /* $6C20 LDY */ s_y = update_nz(peek(0x1501));
      /* $6C23 JSR */ push16(0x6c25), s_pc = 0x6c38;
      branchTarget = true;
      break;
    case 0x6c26: // [$6C26..$6C2B]    6 bytes
      CYCLES(0x6c26, 11);
      /* $6C26 STA */ poke(0x1dd0 + s_x, s_a);
      /* $6C29 JSR */ push16(0x6c2b), s_pc = 0x6c2f;
      branchTarget = true;
      break;
    case 0x6c2c: // [$6C2C..$6C2E]    3 bytes
      CYCLES(0x6c2c, 6);
      /* $6C2C JMP */ s_pc = 0x6b7e;
      branchTarget = true;
      break;
    case 0x6c2f: // [$6C2F..$6C37]    9 bytes
      CYCLES(0x6c2f, 16);
      /* $6C2F LDA */ s_a = update_nz(peek(0x1e30 + s_x));
      /* $6C32 EOR */ s_a = update_nz(s_a ^ 0x03);
      /* $6C34 STA */ poke(0x1e30 + s_x, s_a);
      /* $6C37 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6c38: // [$6C38..$6C43]   12 bytes
      CYCLES(0x6c38, 21);
      /* $6C38 STY */ poke_zpg(0x06, s_y);
      /* $6C3A CMP */ update_nz_inv_c(s_a - peek_zpg(0x06));
      /* $6C3C PHP */ push8(s_status | STATUS_B);
      /* $6C3D LDA */ s_a = update_nz(peek(0x0c44));
      /* $6C40 ASL */ s_a = update_nzc(s_a << 1);
      /* $6C41 JSR */ push16(0x6c43), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x6c44: // [$6C44..$6C54]   17 bytes
      CYCLES(0x6c44, 29);
      /* $6C44 SEC */ s_status |= STATUS_C;
      /* $6C45 SBC */ tmp = peek(0x0c44), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6C48 ADC */ tmp = 0x00, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6C4A AND */ s_a = update_nz(s_a & 0xfe);
      /* $6C4C LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $6C4E LDY */ s_y = update_nz(peek_zpg(0x4e));
      /* $6C50 CPY */ update_nz_inv_c(s_y - peek(0x0c54));
      /* $6C53 BCS */ s_pc = s_status & STATUS_C ? 0x6c65 : 0x6c55;
      branchTarget = true;
      break;
    case 0x6c55: // [$6C55..$6C58]    4 bytes
      CYCLES(0x6c55, 7);
      /* $6C55 PLP */ s_status = pop8() & ~STATUS_B;
      /* $6C56 TAY */ s_y = update_nz(s_a);
      /* $6C57 BCS */ s_pc = s_status & STATUS_C ? 0x6c5d : 0x6c59;
      branchTarget = true;
      break;
    case 0x6c59: // [$6C59..$6C5A]    2 bytes
      CYCLES(0x6c59, 4);
      /* $6C59 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6c64 : 0x6c5b;
      branchTarget = true;
      break;
    case 0x6c5b: // [$6C5B..$6C5C]    2 bytes
      CYCLES(0x6c5b, 4);
      /* $6C5B BCC */ s_pc = !(s_status & STATUS_C) ? 0x6c60 : 0x6c5d;
      branchTarget = true;
      break;
    case 0x6c5d: // [$6C5D..$6C5E]    2 bytes
      CYCLES(0x6c5d, 4);
      /* $6C5D BMI */ s_pc = s_status & STATUS_N ? 0x6c64 : 0x6c5f;
      branchTarget = true;
      break;
    case 0x6c5f: // [$6C5F..$6C5F]    1 bytes
      CYCLES(0x6c5f, 2);
      /* $6C5F CLC */ s_status &= ~STATUS_C;
    case 0x6c60: // [$6C60..$6C63]    4 bytes
      CYCLES(0x6c60, 7);
      /* $6C60 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $6C62 ADC */ tmp = 0x01, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0x6c64: // [$6C64..$6C64]    1 bytes
      CYCLES(0x6c64, 2);
      /* $6C64 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6c65: // [$6C65..$6C66]    2 bytes
      CYCLES(0x6c65, 4);
      /* $6C65 PLP */ s_status = pop8() & ~STATUS_B;
      /* $6C66 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6c67: // [$6C67..$6C69]    3 bytes
      CYCLES(0x6c67, 6);
      /* $6C67 LDX */ s_x = update_nz(peek(0x1429));
    case 0x6c6a: // [$6C6A..$6C6C]    3 bytes
      CYCLES(0x6c6a, 6);
      /* $6C6A DEX */ s_x = update_nz(s_x - 1);
      /* $6C6B BPL */ s_pc = !(s_status & STATUS_N) ? 0x6c70 : 0x6c6d;
      branchTarget = true;
      break;
    case 0x6c6d: // [$6C6D..$6C6F]    3 bytes
      CYCLES(0x6c6d, 6);
      /* $6C6D JMP */ s_pc = 0x6d24;
      branchTarget = true;
      break;
    case 0x6c70: // [$6C70..$6C74]    5 bytes
      CYCLES(0x6c70, 9);
      /* $6C70 DEC */ tmp16 = 0x1e90 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $6C73 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6c6a : 0x6c75;
      branchTarget = true;
      break;
    case 0x6c75: // [$6C75..$6C89]   21 bytes
      CYCLES(0x6c75, 36);
      /* $6C75 STX */ poke_zpg(0x00, s_x);
      /* $6C77 LDA */ s_a = update_nz(peek(0x0c4f));
      /* $6C7A STA */ poke(0x1e90 + s_x, s_a);
      /* $6C7D LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $6C7F LDX */ s_x = update_nz(peek(0x1e60 + s_y));
      /* $6C82 LDA */ s_a = update_nz(peek(0x1e50 + s_y));
      /* $6C85 LDY */ s_y = update_nz(0x07);
      /* $6C87 JSR */ push16(0x6c89), s_pc = 0x4cd0;
      branchTarget = true;
      break;
    case 0x6c8a: // [$6C8A..$6C8B]    2 bytes
      CYCLES(0x6c8a, 4);
      /* $6C8A LDX */ s_x = update_nz(peek_zpg(0x00));
    case 0x6c8c: // [$6C8C..$6C97]   12 bytes
      CYCLES(0x6c8c, 21);
      /* $6C8C LDA */ s_a = update_nz(peek(0x1e50 + s_x));
      /* $6C8F CLC */ s_status &= ~STATUS_C;
      /* $6C90 ADC */ tmp = peek(0x1e70 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6C93 LDY */ s_y = update_nz(peek(0x1e70 + s_x));
      /* $6C96 BMI */ s_pc = s_status & STATUS_N ? 0x6c9d : 0x6c98;
      branchTarget = true;
      break;
    case 0x6c98: // [$6C98..$6C99]    2 bytes
      CYCLES(0x6c98, 4);
      /* $6C98 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6caa : 0x6c9a;
      branchTarget = true;
      break;
    case 0x6c9a: // [$6C9A..$6C9C]    3 bytes
      CYCLES(0x6c9a, 6);
      /* $6C9A CLC */ s_status &= ~STATUS_C;
      /* $6C9B BCC */ s_pc = !(s_status & STATUS_C) ? 0x6c9f : 0x6c9d;
      branchTarget = true;
      break;
    case 0x6c9d: // [$6C9D..$6C9E]    2 bytes
      CYCLES(0x6c9d, 4);
      /* $6C9D BCS */ s_pc = s_status & STATUS_C ? 0x6caa : 0x6c9f;
      branchTarget = true;
      break;
    case 0x6c9f: // [$6C9F..$6CA9]   11 bytes
      CYCLES(0x6c9f, 19);
      /* $6C9F TYA */ s_a = update_nz(s_y);
      /* $6CA0 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $6CA2 ADC */ tmp = 0x01, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6CA4 STA */ poke(0x1e70 + s_x, s_a);
      /* $6CA7 JMP */ s_pc = 0x6c8c;
      branchTarget = true;
      break;
    case 0x6caa: // [$6CAA..$6CAC]    3 bytes
      CYCLES(0x6caa, 6);
      /* $6CAA STA */ poke(0x1e50 + s_x, s_a);
    case 0x6cad: // [$6CAD..$6CB7]   11 bytes
      CYCLES(0x6cad, 19);
      /* $6CAD LDA */ s_a = update_nz(peek(0x1e60 + s_x));
      /* $6CB0 CLC */ s_status &= ~STATUS_C;
      /* $6CB1 ADC */ tmp = peek(0x1e80 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6CB4 CMP */ update_nz_inv_c(s_a - 0xb8);
      /* $6CB6 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6cc5 : 0x6cb8;
      branchTarget = true;
      break;
    case 0x6cb8: // [$6CB8..$6CC4]   13 bytes
      CYCLES(0x6cb8, 23);
      /* $6CB8 LDA */ s_a = update_nz(peek(0x1e80 + s_x));
      /* $6CBB EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $6CBD ADC */ tmp = 0x00, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6CBF STA */ poke(0x1e80 + s_x, s_a);
      /* $6CC2 JMP */ s_pc = 0x6cad;
      branchTarget = true;
      break;
    case 0x6cc5: // [$6CC5..$6CCC]    8 bytes
      CYCLES(0x6cc5, 14);
      /* $6CC5 STA */ poke(0x1e60 + s_x, s_a);
      /* $6CC8 DEC */ tmp16 = 0x1ea0 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $6CCB BPL */ s_pc = !(s_status & STATUS_N) ? 0x6cd3 : 0x6ccd;
      branchTarget = true;
      break;
    case 0x6ccd: // [$6CCD..$6CCF]    3 bytes
      CYCLES(0x6ccd, 6);
      /* $6CCD JSR */ push16(0x6ccf), s_pc = 0x6cf2;
      branchTarget = true;
      break;
    case 0x6cd0: // [$6CD0..$6CD2]    3 bytes
      CYCLES(0x6cd0, 6);
      /* $6CD0 JMP */ s_pc = 0x6c6a;
      branchTarget = true;
      break;
    case 0x6cd3: // [$6CD3..$6CEC]   26 bytes
      CYCLES(0x6cd3, 45);
      /* $6CD3 LDA */ s_a = update_nz(peek(0x1ea0 + s_x));
      /* $6CD6 AND */ s_a = update_nz(s_a & 0x01);
      /* $6CD8 TAY */ s_y = update_nz(s_a);
      /* $6CD9 LDA */ s_a = update_nz(peek(0x0930 + s_y));
      /* $6CDC STA */ poke_zpg(0x06, s_a);
      /* $6CDE LDA */ s_a = update_nz(peek(0x0934 + s_y));
      /* $6CE1 STA */ poke_zpg(0x07, s_a);
      /* $6CE3 LDY */ s_y = update_nz(peek(0x1e60 + s_x));
      /* $6CE6 LDA */ s_a = update_nz(peek(0x1e50 + s_x));
      /* $6CE9 TAX */ s_x = update_nz(s_a);
      /* $6CEA JSR */ push16(0x6cec), s_pc = 0x4cac;
      branchTarget = true;
      break;
    case 0x6ced: // [$6CED..$6CF1]    5 bytes
      CYCLES(0x6ced, 9);
      /* $6CED LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $6CEF JMP */ s_pc = 0x6c6a;
      branchTarget = true;
      break;
    case 0x6cf2: // [$6CF2..$6CF6]    5 bytes
      CYCLES(0x6cf2, 9);
      /* $6CF2 TXA */ s_a = update_nz(s_x);
      /* $6CF3 PHA */ push8(s_a);
      /* $6CF4 DEC */ tmp16 = 0x1429, poke(tmp16, update_nz(peek(tmp16) - 1));
    case 0x6cf7: // [$6CF7..$6D20]   42 bytes
      CYCLES(0x6cf7, 72);
      /* $6CF7 LDA */ s_a = update_nz(peek(0x1e51 + s_x));
      /* $6CFA STA */ poke(0x1e50 + s_x, s_a);
      /* $6CFD LDA */ s_a = update_nz(peek(0x1e61 + s_x));
      /* $6D00 STA */ poke(0x1e60 + s_x, s_a);
      /* $6D03 LDA */ s_a = update_nz(peek(0x1e71 + s_x));
      /* $6D06 STA */ poke(0x1e70 + s_x, s_a);
      /* $6D09 LDA */ s_a = update_nz(peek(0x1e81 + s_x));
      /* $6D0C STA */ poke(0x1e80 + s_x, s_a);
      /* $6D0F LDA */ s_a = update_nz(peek(0x1e91 + s_x));
      /* $6D12 STA */ poke(0x1e90 + s_x, s_a);
      /* $6D15 LDA */ s_a = update_nz(peek(0x1ea1 + s_x));
      /* $6D18 STA */ poke(0x1ea0 + s_x, s_a);
      /* $6D1B INX */ s_x = update_nz(s_x + 1);
      /* $6D1C CPX */ update_nz_inv_c(s_x - peek(0x1429));
      /* $6D1F BCC */ s_pc = !(s_status & STATUS_C) ? 0x6cf7 : 0x6d21;
      branchTarget = true;
      break;
    case 0x6d21: // [$6D21..$6D23]    3 bytes
      CYCLES(0x6d21, 6);
      /* $6D21 PLA */ s_a = update_nz(pop8());
      /* $6D22 TAX */ s_x = update_nz(s_a);
      /* $6D23 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6d24: // [$6D24..$6D28]    5 bytes
      CYCLES(0x6d24, 9);
      /* $6D24 DEC */ tmp16 = 0x142a, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $6D27 BMI */ s_pc = s_status & STATUS_N ? 0x6d2a : 0x6d29;
      branchTarget = true;
      break;
    case 0x6d29: // [$6D29..$6D29]    1 bytes
      CYCLES(0x6d29, 2);
      /* $6D29 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6d2a: // [$6D2A..$6D2F]    6 bytes
      CYCLES(0x6d2a, 11);
      /* $6D2A LDA */ s_a = update_nz(peek(0x142b));
      /* $6D2D JSR */ push16(0x6d2f), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x6d30: // [$6D30..$6D37]    8 bytes
      CYCLES(0x6d30, 14);
      /* $6D30 STA */ poke(0x142a, s_a);
      /* $6D33 LDA */ s_a = update_nz(peek(0x1428));
      /* $6D36 BEQ */ s_pc = s_status & STATUS_Z ? 0x6d29 : 0x6d38;
      branchTarget = true;
      break;
    case 0x6d38: // [$6D38..$6D3A]    3 bytes
      CYCLES(0x6d38, 6);
      /* $6D38 JSR */ push16(0x6d3a), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x6d3b: // [$6D3B..$6D42]    8 bytes
      CYCLES(0x6d3b, 14);
      /* $6D3B TAX */ s_x = update_nz(s_a);
      /* $6D3C LDY */ s_y = update_nz(peek(0x1429));
      /* $6D3F CPY */ update_nz_inv_c(s_y - 0x10);
      /* $6D41 BCS */ s_pc = s_status & STATUS_C ? 0x6d29 : 0x6d43;
      branchTarget = true;
      break;
    case 0x6d43: // [$6D43..$6D45]    3 bytes
      CYCLES(0x6d43, 6);
      /* $6D43 TYA */ s_a = update_nz(s_y);
      /* $6D44 BEQ */ s_pc = s_status & STATUS_Z ? 0x6d4e : 0x6d46;
      branchTarget = true;
      break;
    case 0x6d46: // [$6D46..$6D48]    3 bytes
      CYCLES(0x6d46, 6);
      /* $6D46 JSR */ push16(0x6d48), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x6d49: // [$6D49..$6D4D]    5 bytes
      CYCLES(0x6d49, 9);
      /* $6D49 CMP */ update_nz_inv_c(s_a - peek(0x0c67));
      /* $6D4C BCS */ s_pc = s_status & STATUS_C ? 0x6d5a : 0x6d4e;
      branchTarget = true;
      break;
    case 0x6d4e: // [$6D4E..$6D59]   12 bytes
      CYCLES(0x6d4e, 21);
      /* $6D4E LDA */ s_a = update_nz(peek(0x0c63));
      /* $6D51 STA */ poke(0x142e, s_a);
      /* $6D54 LDA */ s_a = update_nz(peek(0x0c64));
      /* $6D57 STA */ poke(0x142f, s_a);
    case 0x6d5a: // [$6D5A..$6D7F]   38 bytes
      CYCLES(0x6d5a, 65);
      /* $6D5A LDA */ s_a = update_nz(peek(0x1d70 + s_x));
      /* $6D5D CLC */ s_status &= ~STATUS_C;
      /* $6D5E ADC */ tmp = 0x03, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6D60 STA */ poke(0x1e50 + s_y, s_a);
      /* $6D63 LDA */ s_a = update_nz(peek(0x1d90 + s_x));
      /* $6D66 CLC */ s_status &= ~STATUS_C;
      /* $6D67 ADC */ tmp = 0x04, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6D69 STA */ poke(0x1e60 + s_y, s_a);
      /* $6D6C LDA */ s_a = update_nz(peek(0x1500));
      /* $6D6F LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $6D70 ADC */ tmp = 0x40, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6D72 STA */ poke_zpg(0x02, s_a);
      /* $6D74 LDA */ s_a = update_nz(peek(0x1501));
      /* $6D77 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $6D78 ADC */ tmp = 0x50, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6D7A STA */ poke_zpg(0x03, s_a);
      /* $6D7C LDA */ s_a = update_nz(peek_zpg(0x4e));
      /* $6D7E BPL */ s_pc = !(s_status & STATUS_N) ? 0x6d92 : 0x6d80;
      branchTarget = true;
      break;
    case 0x6d80: // [$6D80..$6D83]    4 bytes
      CYCLES(0x6d80, 7);
      /* $6D80 LDA */ s_a = update_nz(peek_zpg(0x02));
      /* $6D82 BMI */ s_pc = s_status & STATUS_N ? 0x6d8b : 0x6d84;
      branchTarget = true;
      break;
    case 0x6d84: // [$6D84..$6D8A]    7 bytes
      CYCLES(0x6d84, 12);
      /* $6D84 LDA */ s_a = update_nz(0x80);
      /* $6D86 SEC */ s_status |= STATUS_C;
      /* $6D87 SBC */ tmp = peek_zpg(0x02), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6D89 BCS */ s_pc = s_status & STATUS_C ? 0x6d90 : 0x6d8b;
      branchTarget = true;
      break;
    case 0x6d8b: // [$6D8B..$6D8F]    5 bytes
      CYCLES(0x6d8b, 9);
      /* $6D8B EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $6D8D CLC */ s_status &= ~STATUS_C;
      /* $6D8E ADC */ tmp = 0x80, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0x6d90: // [$6D90..$6D91]    2 bytes
      CYCLES(0x6d90, 4);
      /* $6D90 STA */ poke_zpg(0x02, s_a);
    case 0x6d92: // [$6D92..$6D95]    4 bytes
      CYCLES(0x6d92, 7);
      /* $6D92 LDA */ s_a = update_nz(peek_zpg(0x4f));
      /* $6D94 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6da8 : 0x6d96;
      branchTarget = true;
      break;
    case 0x6d96: // [$6D96..$6D99]    4 bytes
      CYCLES(0x6d96, 7);
      /* $6D96 LDA */ s_a = update_nz(peek_zpg(0x03));
      /* $6D98 BMI */ s_pc = s_status & STATUS_N ? 0x6da1 : 0x6d9a;
      branchTarget = true;
      break;
    case 0x6d9a: // [$6D9A..$6DA0]    7 bytes
      CYCLES(0x6d9a, 12);
      /* $6D9A LDA */ s_a = update_nz(0xa0);
      /* $6D9C SEC */ s_status |= STATUS_C;
      /* $6D9D SBC */ tmp = peek_zpg(0x03), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6D9F BCS */ s_pc = s_status & STATUS_C ? 0x6da6 : 0x6da1;
      branchTarget = true;
      break;
    case 0x6da1: // [$6DA1..$6DA5]    5 bytes
      CYCLES(0x6da1, 9);
      /* $6DA1 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $6DA3 CLC */ s_status &= ~STATUS_C;
      /* $6DA4 ADC */ tmp = 0x60, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0x6da6: // [$6DA6..$6DA7]    2 bytes
      CYCLES(0x6da6, 4);
      /* $6DA6 STA */ poke_zpg(0x03, s_a);
    case 0x6da8: // [$6DA8..$6DB1]   10 bytes
      CYCLES(0x6da8, 18);
      /* $6DA8 LDA */ s_a = update_nz(peek(0x1e50 + s_y));
      /* $6DAB STA */ poke_zpg(0x00, s_a);
      /* $6DAD LDA */ s_a = update_nz(peek(0x1e60 + s_y));
      /* $6DB0 STA */ poke_zpg(0x01, s_a);
    case 0x6db2: // [$6DB2..$6DC0]   15 bytes
      CYCLES(0x6db2, 26);
      /* $6DB2 LSR */ tmp16 = 0x00, tmp = peek_zpg(tmp16), set_c_to_bit0(tmp), poke_zpg(tmp16, update_nz(tmp >> 1));
      /* $6DB4 LSR */ tmp16 = 0x01, tmp = peek_zpg(tmp16), set_c_to_bit0(tmp), poke_zpg(tmp16, update_nz(tmp >> 1));
      /* $6DB6 LSR */ tmp16 = 0x02, tmp = peek_zpg(tmp16), set_c_to_bit0(tmp), poke_zpg(tmp16, update_nz(tmp >> 1));
      /* $6DB8 LSR */ tmp16 = 0x03, tmp = peek_zpg(tmp16), set_c_to_bit0(tmp), poke_zpg(tmp16, update_nz(tmp >> 1));
      /* $6DBA LDA */ s_a = update_nz(peek_zpg(0x02));
      /* $6DBC SBC */ tmp = peek_zpg(0x00), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6DBE TAX */ s_x = update_nz(s_a);
      /* $6DBF BCS */ s_pc = s_status & STATUS_C ? 0x6dc3 : 0x6dc1;
      branchTarget = true;
      break;
    case 0x6dc1: // [$6DC1..$6DC2]    2 bytes
      CYCLES(0x6dc1, 4);
      /* $6DC1 EOR */ s_a = update_nz(s_a ^ 0xff);
    case 0x6dc3: // [$6DC3..$6DC7]    5 bytes
      CYCLES(0x6dc3, 9);
      /* $6DC3 CMP */ update_nz_inv_c(s_a - peek(0x0c45));
      /* $6DC6 BCS */ s_pc = s_status & STATUS_C ? 0x6db2 : 0x6dc8;
      branchTarget = true;
      break;
    case 0x6dc8: // [$6DC8..$6DD0]    9 bytes
      CYCLES(0x6dc8, 16);
      /* $6DC8 STX */ poke_zpg(0x06, s_x);
      /* $6DCA LDA */ s_a = update_nz(peek_zpg(0x03));
      /* $6DCC SBC */ tmp = peek_zpg(0x01), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6DCE TAX */ s_x = update_nz(s_a);
      /* $6DCF BCS */ s_pc = s_status & STATUS_C ? 0x6dd3 : 0x6dd1;
      branchTarget = true;
      break;
    case 0x6dd1: // [$6DD1..$6DD2]    2 bytes
      CYCLES(0x6dd1, 4);
      /* $6DD1 EOR */ s_a = update_nz(s_a ^ 0xff);
    case 0x6dd3: // [$6DD3..$6DD7]    5 bytes
      CYCLES(0x6dd3, 9);
      /* $6DD3 CMP */ update_nz_inv_c(s_a - peek(0x0c45));
      /* $6DD6 BCS */ s_pc = s_status & STATUS_C ? 0x6db2 : 0x6dd8;
      branchTarget = true;
      break;
    case 0x6dd8: // [$6DD8..$6DEF]   24 bytes
      CYCLES(0x6dd8, 41);
      /* $6DD8 LDA */ s_a = update_nz(peek_zpg(0x06));
      /* $6DDA STA */ poke(0x1e70 + s_y, s_a);
      /* $6DDD TXA */ s_a = update_nz(s_x);
      /* $6DDE STA */ poke(0x1e80 + s_y, s_a);
      /* $6DE1 LDA */ s_a = update_nz(0x00);
      /* $6DE3 STA */ poke(0x1e90 + s_y, s_a);
      /* $6DE6 LDA */ s_a = update_nz(peek(0x0c4c));
      /* $6DE9 STA */ poke(0x1ea0 + s_y, s_a);
      /* $6DEC INC */ tmp16 = 0x1429, poke(tmp16, update_nz(peek(tmp16) + 1));
      /* $6DEF RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6df0: // [$6DF0..$6DF4]    5 bytes
      CYCLES(0x6df0, 9);
      /* $6DF0 TXA */ s_a = update_nz(s_x);
      /* $6DF1 PHA */ push8(s_a);
      /* $6DF2 DEC */ tmp16 = 0x1427, poke(tmp16, update_nz(peek(tmp16) - 1));
    case 0x6df5: // [$6DF5..$6E30]   60 bytes
      CYCLES(0x6df5, 103);
      /* $6DF5 LDA */ s_a = update_nz(peek(0x1ce1 + s_x));
      /* $6DF8 STA */ poke(0x1ce0 + s_x, s_a);
      /* $6DFB LDA */ s_a = update_nz(peek(0x1cf1 + s_x));
      /* $6DFE STA */ poke(0x1cf0 + s_x, s_a);
      /* $6E01 LDA */ s_a = update_nz(peek(0x1d01 + s_x));
      /* $6E04 STA */ poke(0x1d00 + s_x, s_a);
      /* $6E07 LDA */ s_a = update_nz(peek(0x1d11 + s_x));
      /* $6E0A STA */ poke(0x1d10 + s_x, s_a);
      /* $6E0D LDA */ s_a = update_nz(peek(0x1d21 + s_x));
      /* $6E10 STA */ poke(0x1d20 + s_x, s_a);
      /* $6E13 LDA */ s_a = update_nz(peek(0x1d31 + s_x));
      /* $6E16 STA */ poke(0x1d30 + s_x, s_a);
      /* $6E19 LDA */ s_a = update_nz(peek(0x1d41 + s_x));
      /* $6E1C STA */ poke(0x1d40 + s_x, s_a);
      /* $6E1F LDA */ s_a = update_nz(peek(0x1d51 + s_x));
      /* $6E22 STA */ poke(0x1d50 + s_x, s_a);
      /* $6E25 LDA */ s_a = update_nz(peek(0x1d61 + s_x));
      /* $6E28 STA */ poke(0x1d60 + s_x, s_a);
      /* $6E2B INX */ s_x = update_nz(s_x + 1);
      /* $6E2C CPX */ update_nz_inv_c(s_x - peek(0x1427));
      /* $6E2F BCC */ s_pc = !(s_status & STATUS_C) ? 0x6df5 : 0x6e31;
      branchTarget = true;
      break;
    case 0x6e31: // [$6E31..$6E33]    3 bytes
      CYCLES(0x6e31, 6);
      /* $6E31 PLA */ s_a = update_nz(pop8());
      /* $6E32 TAX */ s_x = update_nz(s_a);
      /* $6E33 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6e34: // [$6E34..$6E3D]   10 bytes
      CYCLES(0x6e34, 18);
      /* $6E34 LDX */ s_x = update_nz(peek(0x1427));
      /* $6E37 LDA */ s_a = update_nz(peek_zpg(0x1c));
      /* $6E39 SEC */ s_status |= STATUS_C;
      /* $6E3A SBC */ tmp = 0x0d, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6E3C BCS */ s_pc = s_status & STATUS_C ? 0x6e40 : 0x6e3e;
      branchTarget = true;
      break;
    case 0x6e3e: // [$6E3E..$6E3F]    2 bytes
      CYCLES(0x6e3e, 4);
      /* $6E3E LDA */ s_a = update_nz(0x00);
    case 0x6e40: // [$6E40..$6E40]    1 bytes
      CYCLES(0x6e40, 2);
      /* $6E40 TAY */ s_y = update_nz(s_a);
    case 0x6e41: // [$6E41..$6E42]    2 bytes
      CYCLES(0x6e41, 4);
      /* $6E41 LDA */ s_a = update_nz(peek_zpg(0x1c));
    case 0x6e43: // [$6E43..$6E45]    3 bytes
      CYCLES(0x6e43, 6);
      /* $6E43 DEX */ s_x = update_nz(s_x - 1);
      /* $6E44 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6e48 : 0x6e46;
      branchTarget = true;
      break;
    case 0x6e46: // [$6E46..$6E47]    2 bytes
      CYCLES(0x6e46, 4);
      /* $6E46 CLC */ s_status &= ~STATUS_C;
      /* $6E47 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6e48: // [$6E48..$6E4C]    5 bytes
      CYCLES(0x6e48, 9);
      /* $6E48 CMP */ update_nz_inv_c(s_a - peek(0x1ce0 + s_x));
      /* $6E4B BCC */ s_pc = !(s_status & STATUS_C) ? 0x6e43 : 0x6e4d;
      branchTarget = true;
      break;
    case 0x6e4d: // [$6E4D..$6E52]    6 bytes
      CYCLES(0x6e4d, 11);
      /* $6E4D TYA */ s_a = update_nz(s_y);
      /* $6E4E CMP */ update_nz_inv_c(s_a - peek(0x1ce0 + s_x));
      /* $6E51 BCS */ s_pc = s_status & STATUS_C ? 0x6e41 : 0x6e53;
      branchTarget = true;
      break;
    case 0x6e53: // [$6E53..$6E59]    7 bytes
      CYCLES(0x6e53, 12);
      /* $6E53 LDA */ s_a = update_nz(peek_zpg(0x1d));
      /* $6E55 CMP */ update_nz_inv_c(s_a - peek(0x1cf0 + s_x));
      /* $6E58 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6e41 : 0x6e5a;
      branchTarget = true;
      break;
    case 0x6e5a: // [$6E5A..$6E5D]    4 bytes
      CYCLES(0x6e5a, 7);
      /* $6E5A SBC */ tmp = 0x0d, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6E5C BCC */ s_pc = !(s_status & STATUS_C) ? 0x6e63 : 0x6e5e;
      branchTarget = true;
      break;
    case 0x6e5e: // [$6E5E..$6E62]    5 bytes
      CYCLES(0x6e5e, 9);
      /* $6E5E CMP */ update_nz_inv_c(s_a - peek(0x1cf0 + s_x));
      /* $6E61 BCS */ s_pc = s_status & STATUS_C ? 0x6e41 : 0x6e63;
      branchTarget = true;
      break;
    case 0x6e63: // [$6E63..$6E66]    4 bytes
      CYCLES(0x6e63, 7);
      /* $6E63 LDA */ s_a = update_nz(peek_zpg(0xe8));
      /* $6E65 BEQ */ s_pc = s_status & STATUS_Z ? 0x6e69 : 0x6e67;
      branchTarget = true;
      break;
    case 0x6e67: // [$6E67..$6E68]    2 bytes
      CYCLES(0x6e67, 4);
      /* $6E67 SEC */ s_status |= STATUS_C;
      /* $6E68 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6e69: // [$6E69..$6E76]   14 bytes
      CYCLES(0x6e69, 24);
      /* $6E69 TXA */ s_a = update_nz(s_x);
      /* $6E6A PHA */ push8(s_a);
      /* $6E6B TAY */ s_y = update_nz(s_a);
      /* $6E6C LDX */ s_x = update_nz(peek(0x1cf0 + s_y));
      /* $6E6F LDA */ s_a = update_nz(peek(0x1ce0 + s_y));
      /* $6E72 LDY */ s_y = update_nz(0x0d);
      /* $6E74 JSR */ push16(0x6e76), s_pc = 0x4d21;
      branchTarget = true;
      break;
    case 0x6e77: // [$6E77..$6E89]   19 bytes
      CYCLES(0x6e77, 33);
      /* $6E77 PLA */ s_a = update_nz(pop8());
      /* $6E78 PHA */ push8(s_a);
      /* $6E79 TAY */ s_y = update_nz(s_a);
      /* $6E7A LDA */ s_a = update_nz(peek(0x1ce0 + s_y));
      /* $6E7D CLC */ s_status &= ~STATUS_C;
      /* $6E7E ADC */ tmp = 0x03, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6E80 TAX */ s_x = update_nz(s_a);
      /* $6E81 LDA */ s_a = update_nz(peek(0x1cf0 + s_y));
      /* $6E84 TAY */ s_y = update_nz(s_a);
      /* $6E85 LDA */ s_a = update_nz(0x06);
      /* $6E87 JSR */ push16(0x6e89), s_pc = 0x5a27;
      branchTarget = true;
      break;
    case 0x6e8a: // [$6E8A..$6E8E]    5 bytes
      CYCLES(0x6e8a, 9);
      /* $6E8A PLA */ s_a = update_nz(pop8());
      /* $6E8B TAX */ s_x = update_nz(s_a);
      /* $6E8C JSR */ push16(0x6e8e), s_pc = 0x6df0;
      branchTarget = true;
      break;
    case 0x6e8f: // [$6E8F..$6E95]    7 bytes
      CYCLES(0x6e8f, 12);
      /* $6E8F LDA */ s_a = update_nz(0x03);
      /* $6E91 LDX */ s_x = update_nz(0xe8);
      /* $6E93 JSR */ push16(0x6e95), s_pc = 0x4dd0;
      branchTarget = true;
      break;
    case 0x6e96: // [$6E96..$6E97]    2 bytes
      CYCLES(0x6e96, 4);
      /* $6E96 SEC */ s_status |= STATUS_C;
      /* $6E97 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6e98: // [$6E98..$6EA1]   10 bytes
      CYCLES(0x6e98, 18);
      /* $6E98 LDX */ s_x = update_nz(peek(0x1428));
      /* $6E9B LDA */ s_a = update_nz(peek_zpg(0x1c));
      /* $6E9D SEC */ s_status |= STATUS_C;
      /* $6E9E SBC */ tmp = 0x0d, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6EA0 BCS */ s_pc = s_status & STATUS_C ? 0x6ea4 : 0x6ea2;
      branchTarget = true;
      break;
    case 0x6ea2: // [$6EA2..$6EA3]    2 bytes
      CYCLES(0x6ea2, 4);
      /* $6EA2 LDA */ s_a = update_nz(0x00);
    case 0x6ea4: // [$6EA4..$6EA4]    1 bytes
      CYCLES(0x6ea4, 2);
      /* $6EA4 TAY */ s_y = update_nz(s_a);
    case 0x6ea5: // [$6EA5..$6EA6]    2 bytes
      CYCLES(0x6ea5, 4);
      /* $6EA5 LDA */ s_a = update_nz(peek_zpg(0x1c));
    case 0x6ea7: // [$6EA7..$6EA9]    3 bytes
      CYCLES(0x6ea7, 6);
      /* $6EA7 DEX */ s_x = update_nz(s_x - 1);
      /* $6EA8 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6eac : 0x6eaa;
      branchTarget = true;
      break;
    case 0x6eaa: // [$6EAA..$6EAB]    2 bytes
      CYCLES(0x6eaa, 4);
      /* $6EAA CLC */ s_status &= ~STATUS_C;
      /* $6EAB RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6eac: // [$6EAC..$6EB0]    5 bytes
      CYCLES(0x6eac, 9);
      /* $6EAC CMP */ update_nz_inv_c(s_a - peek(0x1d70 + s_x));
      /* $6EAF BCC */ s_pc = !(s_status & STATUS_C) ? 0x6ea7 : 0x6eb1;
      branchTarget = true;
      break;
    case 0x6eb1: // [$6EB1..$6EB6]    6 bytes
      CYCLES(0x6eb1, 11);
      /* $6EB1 TYA */ s_a = update_nz(s_y);
      /* $6EB2 CMP */ update_nz_inv_c(s_a - peek(0x1d70 + s_x));
      /* $6EB5 BEQ */ s_pc = s_status & STATUS_Z ? 0x6eb9 : 0x6eb7;
      branchTarget = true;
      break;
    case 0x6eb7: // [$6EB7..$6EB8]    2 bytes
      CYCLES(0x6eb7, 4);
      /* $6EB7 BCS */ s_pc = s_status & STATUS_C ? 0x6ea5 : 0x6eb9;
      branchTarget = true;
      break;
    case 0x6eb9: // [$6EB9..$6EBF]    7 bytes
      CYCLES(0x6eb9, 12);
      /* $6EB9 LDA */ s_a = update_nz(peek_zpg(0x1d));
      /* $6EBB CMP */ update_nz_inv_c(s_a - peek(0x1d90 + s_x));
      /* $6EBE BCC */ s_pc = !(s_status & STATUS_C) ? 0x6ea5 : 0x6ec0;
      branchTarget = true;
      break;
    case 0x6ec0: // [$6EC0..$6EC3]    4 bytes
      CYCLES(0x6ec0, 7);
      /* $6EC0 SBC */ tmp = 0x0f, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6EC2 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6ec9 : 0x6ec4;
      branchTarget = true;
      break;
    case 0x6ec4: // [$6EC4..$6EC8]    5 bytes
      CYCLES(0x6ec4, 9);
      /* $6EC4 CMP */ update_nz_inv_c(s_a - peek(0x1d90 + s_x));
      /* $6EC7 BCS */ s_pc = s_status & STATUS_C ? 0x6ea5 : 0x6ec9;
      branchTarget = true;
      break;
    case 0x6ec9: // [$6EC9..$6ECC]    4 bytes
      CYCLES(0x6ec9, 7);
      /* $6EC9 LDA */ s_a = update_nz(peek_zpg(0xe8));
      /* $6ECB BEQ */ s_pc = s_status & STATUS_Z ? 0x6ecf : 0x6ecd;
      branchTarget = true;
      break;
    case 0x6ecd: // [$6ECD..$6ECE]    2 bytes
      CYCLES(0x6ecd, 4);
      /* $6ECD SEC */ s_status |= STATUS_C;
      /* $6ECE RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6ecf: // [$6ECF..$6EDC]   14 bytes
      CYCLES(0x6ecf, 24);
      /* $6ECF TXA */ s_a = update_nz(s_x);
      /* $6ED0 PHA */ push8(s_a);
      /* $6ED1 TAY */ s_y = update_nz(s_a);
      /* $6ED2 LDX */ s_x = update_nz(peek(0x1d90 + s_y));
      /* $6ED5 LDA */ s_a = update_nz(peek(0x1d70 + s_y));
      /* $6ED8 LDY */ s_y = update_nz(0x0f);
      /* $6EDA JSR */ push16(0x6edc), s_pc = 0x4d21;
      branchTarget = true;
      break;
    case 0x6edd: // [$6EDD..$6EF2]   22 bytes
      CYCLES(0x6edd, 38);
      /* $6EDD PLA */ s_a = update_nz(pop8());
      /* $6EDE TAX */ s_x = update_nz(s_a);
      /* $6EDF PHA */ push8(s_a);
      /* $6EE0 LDA */ s_a = update_nz(peek(0x1d90 + s_x));
      /* $6EE3 CLC */ s_status &= ~STATUS_C;
      /* $6EE4 ADC */ tmp = 0x07, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6EE6 TAY */ s_y = update_nz(s_a);
      /* $6EE7 LDA */ s_a = update_nz(peek(0x1d70 + s_x));
      /* $6EEA CLC */ s_status &= ~STATUS_C;
      /* $6EEB ADC */ tmp = 0x03, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $6EED TAX */ s_x = update_nz(s_a);
      /* $6EEE LDA */ s_a = update_nz(0xaa);
      /* $6EF0 JSR */ push16(0x6ef2), s_pc = 0x4f74;
      branchTarget = true;
      break;
    case 0x6ef3: // [$6EF3..$6EF7]    5 bytes
      CYCLES(0x6ef3, 9);
      /* $6EF3 PLA */ s_a = update_nz(pop8());
      /* $6EF4 TAX */ s_x = update_nz(s_a);
      /* $6EF5 JSR */ push16(0x6ef7), s_pc = 0x6f6f;
      branchTarget = true;
      break;
    case 0x6ef8: // [$6EF8..$6EFE]    7 bytes
      CYCLES(0x6ef8, 12);
      /* $6EF8 LDA */ s_a = update_nz(0x00);
      /* $6EFA LDX */ s_x = update_nz(0xc8);
      /* $6EFC JSR */ push16(0x6efe), s_pc = 0x4dd0;
      branchTarget = true;
      break;
    case 0x6eff: // [$6EFF..$6F12]   20 bytes
      CYCLES(0x6eff, 35);
      /* $6EFF LDA */ s_a = update_nz(peek(0x0c46));
      /* $6F02 STA */ poke(0x1406, s_a);
      /* $6F05 LDA */ s_a = update_nz(peek(0x0c47));
      /* $6F08 STA */ poke(0x1422, s_a);
      /* $6F0B LDA */ s_a = update_nz(peek(0x0c48));
      /* $6F0E STA */ poke(0x1423, s_a);
      /* $6F11 SEC */ s_status |= STATUS_C;
      /* $6F12 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6f13: // [$6F13..$6F1C]   10 bytes
      CYCLES(0x6f13, 18);
      /* $6F13 LDX */ s_x = update_nz(peek(0x1429));
      /* $6F16 LDA */ s_a = update_nz(peek_zpg(0x1c));
      /* $6F18 SEC */ s_status |= STATUS_C;
      /* $6F19 SBC */ tmp = 0x07, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6F1B BCS */ s_pc = s_status & STATUS_C ? 0x6f1f : 0x6f1d;
      branchTarget = true;
      break;
    case 0x6f1d: // [$6F1D..$6F1E]    2 bytes
      CYCLES(0x6f1d, 4);
      /* $6F1D LDA */ s_a = update_nz(0x00);
    case 0x6f1f: // [$6F1F..$6F1F]    1 bytes
      CYCLES(0x6f1f, 2);
      /* $6F1F TAY */ s_y = update_nz(s_a);
    case 0x6f20: // [$6F20..$6F21]    2 bytes
      CYCLES(0x6f20, 4);
      /* $6F20 LDA */ s_a = update_nz(peek_zpg(0x1c));
    case 0x6f22: // [$6F22..$6F24]    3 bytes
      CYCLES(0x6f22, 6);
      /* $6F22 DEX */ s_x = update_nz(s_x - 1);
      /* $6F23 BPL */ s_pc = !(s_status & STATUS_N) ? 0x6f27 : 0x6f25;
      branchTarget = true;
      break;
    case 0x6f25: // [$6F25..$6F26]    2 bytes
      CYCLES(0x6f25, 4);
      /* $6F25 CLC */ s_status &= ~STATUS_C;
      /* $6F26 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6f27: // [$6F27..$6F2B]    5 bytes
      CYCLES(0x6f27, 9);
      /* $6F27 CMP */ update_nz_inv_c(s_a - peek(0x1e50 + s_x));
      /* $6F2A BCC */ s_pc = !(s_status & STATUS_C) ? 0x6f22 : 0x6f2c;
      branchTarget = true;
      break;
    case 0x6f2c: // [$6F2C..$6F31]    6 bytes
      CYCLES(0x6f2c, 11);
      /* $6F2C TYA */ s_a = update_nz(s_y);
      /* $6F2D CMP */ update_nz_inv_c(s_a - peek(0x1e50 + s_x));
      /* $6F30 BCS */ s_pc = s_status & STATUS_C ? 0x6f20 : 0x6f32;
      branchTarget = true;
      break;
    case 0x6f32: // [$6F32..$6F38]    7 bytes
      CYCLES(0x6f32, 12);
      /* $6F32 LDA */ s_a = update_nz(peek_zpg(0x1d));
      /* $6F34 CMP */ update_nz_inv_c(s_a - peek(0x1e60 + s_x));
      /* $6F37 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6f20 : 0x6f39;
      branchTarget = true;
      break;
    case 0x6f39: // [$6F39..$6F3C]    4 bytes
      CYCLES(0x6f39, 7);
      /* $6F39 SBC */ tmp = 0x07, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $6F3B BCC */ s_pc = !(s_status & STATUS_C) ? 0x6f42 : 0x6f3d;
      branchTarget = true;
      break;
    case 0x6f3d: // [$6F3D..$6F41]    5 bytes
      CYCLES(0x6f3d, 9);
      /* $6F3D CMP */ update_nz_inv_c(s_a - peek(0x1e60 + s_x));
      /* $6F40 BCS */ s_pc = s_status & STATUS_C ? 0x6f20 : 0x6f42;
      branchTarget = true;
      break;
    case 0x6f42: // [$6F42..$6F45]    4 bytes
      CYCLES(0x6f42, 7);
      /* $6F42 LDA */ s_a = update_nz(peek_zpg(0xe8));
      /* $6F44 BEQ */ s_pc = s_status & STATUS_Z ? 0x6f48 : 0x6f46;
      branchTarget = true;
      break;
    case 0x6f46: // [$6F46..$6F47]    2 bytes
      CYCLES(0x6f46, 4);
      /* $6F46 SEC */ s_status |= STATUS_C;
      /* $6F47 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6f48: // [$6F48..$6F55]   14 bytes
      CYCLES(0x6f48, 24);
      /* $6F48 TXA */ s_a = update_nz(s_x);
      /* $6F49 PHA */ push8(s_a);
      /* $6F4A TAY */ s_y = update_nz(s_a);
      /* $6F4B LDX */ s_x = update_nz(peek(0x1e60 + s_y));
      /* $6F4E LDA */ s_a = update_nz(peek(0x1e50 + s_y));
      /* $6F51 LDY */ s_y = update_nz(0x07);
      /* $6F53 JSR */ push16(0x6f55), s_pc = 0x4cd0;
      branchTarget = true;
      break;
    case 0x6f56: // [$6F56..$6F5A]    5 bytes
      CYCLES(0x6f56, 9);
      /* $6F56 PLA */ s_a = update_nz(pop8());
      /* $6F57 TAX */ s_x = update_nz(s_a);
      /* $6F58 JSR */ push16(0x6f5a), s_pc = 0x6cf2;
      branchTarget = true;
      break;
    case 0x6f5b: // [$6F5B..$6F6C]   18 bytes
      CYCLES(0x6f5b, 31);
      /* $6F5B LDA */ s_a = update_nz(peek(0x0c49));
      /* $6F5E STA */ poke(0x1413, s_a);
      /* $6F61 LDA */ s_a = update_nz(0x01);
      /* $6F63 STA */ poke(0x1412, s_a);
      /* $6F66 LDA */ s_a = update_nz(0x00);
      /* $6F68 LDX */ s_x = update_nz(0x19);
      /* $6F6A JSR */ push16(0x6f6c), s_pc = 0x4dd0;
      branchTarget = true;
      break;
    case 0x6f6d: // [$6F6D..$6F6E]    2 bytes
      CYCLES(0x6f6d, 4);
      /* $6F6D SEC */ s_status |= STATUS_C;
      /* $6F6E RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x6f6f: // [$6F6F..$6F73]    5 bytes
      CYCLES(0x6f6f, 9);
      /* $6F6F TXA */ s_a = update_nz(s_x);
      /* $6F70 PHA */ push8(s_a);
      /* $6F71 DEC */ tmp16 = 0x1428, poke(tmp16, update_nz(peek(tmp16) - 1));
    case 0x6f74: // [$6F74..$6FA3]   48 bytes
      CYCLES(0x6f74, 82);
      /* $6F74 LDA */ s_a = update_nz(peek(0x1d71 + s_x));
      /* $6F77 STA */ poke(0x1d70 + s_x, s_a);
      /* $6F7A LDA */ s_a = update_nz(peek(0x1d91 + s_x));
      /* $6F7D STA */ poke(0x1d90 + s_x, s_a);
      /* $6F80 LDA */ s_a = update_nz(peek(0x1db1 + s_x));
      /* $6F83 STA */ poke(0x1db0 + s_x, s_a);
      /* $6F86 LDA */ s_a = update_nz(peek(0x1dd1 + s_x));
      /* $6F89 STA */ poke(0x1dd0 + s_x, s_a);
      /* $6F8C LDA */ s_a = update_nz(peek(0x1df1 + s_x));
      /* $6F8F STA */ poke(0x1df0 + s_x, s_a);
      /* $6F92 LDA */ s_a = update_nz(peek(0x1e11 + s_x));
      /* $6F95 STA */ poke(0x1e10 + s_x, s_a);
      /* $6F98 LDA */ s_a = update_nz(peek(0x1e31 + s_x));
      /* $6F9B STA */ poke(0x1e30 + s_x, s_a);
      /* $6F9E INX */ s_x = update_nz(s_x + 1);
      /* $6F9F CPX */ update_nz_inv_c(s_x - peek(0x1428));
      /* $6FA2 BCC */ s_pc = !(s_status & STATUS_C) ? 0x6f74 : 0x6fa4;
      branchTarget = true;
      break;
    case 0x6fa4: // [$6FA4..$6FA6]    3 bytes
      CYCLES(0x6fa4, 6);
      /* $6FA4 PLA */ s_a = update_nz(pop8());
      /* $6FA5 TAX */ s_x = update_nz(s_a);
      /* $6FA6 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x7000: // [$7000..$7002]    3 bytes
      CYCLES(0x7000, 6);
      /* $7000 JMP */ s_pc = 0x7015;
      branchTarget = true;
      break;
    case 0x7003: // [$7003..$7005]    3 bytes
      CYCLES(0x7003, 6);
      /* $7003 JMP */ s_pc = 0x70ca;
      branchTarget = true;
      break;
    case 0x7006: // [$7006..$7008]    3 bytes
      CYCLES(0x7006, 6);
      /* $7006 JMP */ s_pc = 0x76f1;
      branchTarget = true;
      break;
    case 0x7009: // [$7009..$700B]    3 bytes
      CYCLES(0x7009, 6);
      /* $7009 JMP */ s_pc = 0x74bd;
      branchTarget = true;
      break;
    case 0x700c: // [$700C..$700E]    3 bytes
      CYCLES(0x700c, 6);
      /* $700C JMP */ s_pc = 0x73a3;
      branchTarget = true;
      break;
    case 0x700f: // [$700F..$7011]    3 bytes
      CYCLES(0x700f, 6);
      /* $700F JMP */ s_pc = 0x788a;
      branchTarget = true;
      break;
    case 0x7012: // [$7012..$7014]    3 bytes
      CYCLES(0x7012, 6);
      /* $7012 JMP */ s_pc = 0x7585;
      branchTarget = true;
      break;
    case 0x7015: // [$7015..$701C]    8 bytes
      CYCLES(0x7015, 14);
      /* $7015 LDA */ s_a = update_nz(peek(0x0c29));
      /* $7018 LDY */ s_y = update_nz(peek(0x1418));
      /* $701B BNE */ s_pc = !(s_status & STATUS_Z) ? 0x701f : 0x701d;
      branchTarget = true;
      break;
    case 0x701d: // [$701D..$701E]    2 bytes
      CYCLES(0x701d, 4);
      /* $701D LDA */ s_a = update_nz(0x00);
    case 0x701f: // [$701F..$7026]    8 bytes
      CYCLES(0x701f, 14);
      /* $701F STA */ poke(0x141c, s_a);
      /* $7022 CMP */ update_nz_inv_c(s_a - peek(0x1421));
      /* $7025 BCC */ s_pc = !(s_status & STATUS_C) ? 0x702a : 0x7027;
      branchTarget = true;
      break;
    case 0x7027: // [$7027..$7029]    3 bytes
      CYCLES(0x7027, 6);
      /* $7027 STA */ poke(0x1421, s_a);
    case 0x702a: // [$702A..$702C]    3 bytes
      CYCLES(0x702a, 6);
      /* $702A DEY */ s_y = update_nz(s_y - 1);
      /* $702B BPL */ s_pc = !(s_status & STATUS_N) ? 0x703f : 0x702d;
      branchTarget = true;
      break;
    case 0x702d: // [$702D..$703E]   18 bytes
      CYCLES(0x702d, 31);
      /* $702D LDA */ s_a = update_nz(0x00);
      /* $702F STA */ poke(0x141b, s_a);
      /* $7032 STA */ poke(0x141d, s_a);
      /* $7035 STA */ poke(0x141e, s_a);
      /* $7038 STA */ poke(0x1419, s_a);
      /* $703B STA */ poke(0x1426, s_a);
      /* $703E RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x703f: // [$703F..$7041]    3 bytes
      CYCLES(0x703f, 6);
      /* $703F JSR */ push16(0x7041), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x7042: // [$7042..$704A]    9 bytes
      CYCLES(0x7042, 16);
      /* $7042 PHP */ push8(s_status | STATUS_B);
      /* $7043 AND */ s_a = update_nz(s_a & 0x01);
      /* $7045 STA */ poke(0x1b40 + s_y, s_a);
      /* $7048 PLP */ s_status = pop8() & ~STATUS_B;
      /* $7049 BMI */ s_pc = s_status & STATUS_N ? 0x7070 : 0x704b;
      branchTarget = true;
      break;
    case 0x704b: // [$704B..$704F]    5 bytes
      CYCLES(0x704b, 9);
      /* $704B LDA */ s_a = update_nz(0xdb);
      /* $704D JSR */ push16(0x704f), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x7050: // [$7050..$705A]   11 bytes
      CYCLES(0x7050, 19);
      /* $7050 CLC */ s_status &= ~STATUS_C;
      /* $7051 ADC */ tmp = 0x0c, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $7053 AND */ s_a = update_nz(s_a & 0xfe);
      /* $7055 STA */ poke(0x1b20 + s_y, s_a);
      /* $7058 JSR */ push16(0x705a), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x705b: // [$705B..$7061]    7 bytes
      CYCLES(0x705b, 12);
      /* $705B STA */ poke_zpg(0x00, s_a);
      /* $705D LDA */ s_a = update_nz(0x20);
      /* $705F JSR */ push16(0x7061), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x7062: // [$7062..$7068]    7 bytes
      CYCLES(0x7062, 12);
      /* $7062 CLC */ s_status &= ~STATUS_C;
      /* $7063 ADC */ tmp = 0x0c, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $7065 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $7067 BPL */ s_pc = !(s_status & STATUS_N) ? 0x706b : 0x7069;
      branchTarget = true;
      break;
    case 0x7069: // [$7069..$706A]    2 bytes
      CYCLES(0x7069, 4);
      /* $7069 ADC */ tmp = 0x7c, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0x706b: // [$706B..$706F]    5 bytes
      CYCLES(0x706b, 9);
      /* $706B STA */ poke(0x1b30 + s_y, s_a);
      /* $706E BNE */ s_pc = !(s_status & STATUS_Z) ? 0x7093 : 0x7070;
      branchTarget = true;
      break;
    case 0x7070: // [$7070..$7074]    5 bytes
      CYCLES(0x7070, 9);
      /* $7070 LDA */ s_a = update_nz(0x9c);
      /* $7072 JSR */ push16(0x7074), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x7075: // [$7075..$707D]    9 bytes
      CYCLES(0x7075, 16);
      /* $7075 CLC */ s_status &= ~STATUS_C;
      /* $7076 ADC */ tmp = 0x0c, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $7078 STA */ poke(0x1b30 + s_y, s_a);
      /* $707B JSR */ push16(0x707d), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x707e: // [$707E..$7084]    7 bytes
      CYCLES(0x707e, 12);
      /* $707E STA */ poke_zpg(0x00, s_a);
      /* $7080 LDA */ s_a = update_nz(0x37);
      /* $7082 JSR */ push16(0x7084), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x7085: // [$7085..$708B]    7 bytes
      CYCLES(0x7085, 12);
      /* $7085 CLC */ s_status &= ~STATUS_C;
      /* $7086 ADC */ tmp = 0x0c, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $7088 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $708A BPL */ s_pc = !(s_status & STATUS_N) ? 0x708e : 0x708c;
      branchTarget = true;
      break;
    case 0x708c: // [$708C..$708D]    2 bytes
      CYCLES(0x708c, 4);
      /* $708C ADC */ tmp = 0xa4, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0x708e: // [$708E..$7092]    5 bytes
      CYCLES(0x708e, 9);
      /* $708E AND */ s_a = update_nz(s_a & 0xfe);
      /* $7090 STA */ poke(0x1b20 + s_y, s_a);
    case 0x7093: // [$7093..$7097]    5 bytes
      CYCLES(0x7093, 9);
      /* $7093 LDA */ s_a = update_nz(peek(0x140a));
      /* $7096 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x709c : 0x7098;
      branchTarget = true;
      break;
    case 0x7098: // [$7098..$709B]    4 bytes
      CYCLES(0x7098, 7);
      /* $7098 LDA */ s_a = update_nz(0xff);
      /* $709A BMI */ s_pc = s_status & STATUS_N ? 0x709f : 0x709c;
      branchTarget = true;
      break;
    case 0x709c: // [$709C..$709E]    3 bytes
      CYCLES(0x709c, 6);
      /* $709C JSR */ push16(0x709e), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x709f: // [$709F..$70AD]   15 bytes
      CYCLES(0x709f, 26);
      /* $709F STA */ poke(0x1b50 + s_y, s_a);
      /* $70A2 TYA */ s_a = update_nz(s_y);
      /* $70A3 AND */ s_a = update_nz(s_a & 0x03);
      /* $70A5 STA */ poke(0x1b60 + s_y, s_a);
      /* $70A8 LDA */ s_a = update_nz(peek(0x0c25));
      /* $70AB JSR */ push16(0x70ad), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x70ae: // [$70AE..$70B6]    9 bytes
      CYCLES(0x70ae, 16);
      /* $70AE STA */ poke(0x1b70 + s_y, s_a);
      /* $70B1 LDA */ s_a = update_nz(peek(0x0c26));
      /* $70B4 JSR */ push16(0x70b6), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x70b7: // [$70B7..$70C1]   11 bytes
      CYCLES(0x70b7, 19);
      /* $70B7 STA */ poke(0x1b80 + s_y, s_a);
      /* $70BA LDA */ s_a = update_nz(0xff);
      /* $70BC STA */ poke(0x1b90 + s_y, s_a);
      /* $70BF JSR */ push16(0x70c1), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x70c2: // [$70C2..$70C9]    8 bytes
      CYCLES(0x70c2, 14);
      /* $70C2 AND */ s_a = update_nz(s_a & 0x01);
      /* $70C4 STA */ poke(0x1ba0 + s_y, s_a);
      /* $70C7 JMP */ s_pc = 0x702a;
      branchTarget = true;
      break;
    case 0x70ca: // [$70CA..$70D1]    8 bytes
      CYCLES(0x70ca, 14);
      /* $70CA LDX */ s_x = update_nz(peek(0x1418));
      /* $70CD LDA */ s_a = update_nz(peek(0x141c));
      /* $70D0 BEQ */ s_pc = s_status & STATUS_Z ? 0x70d5 : 0x70d2;
      branchTarget = true;
      break;
    case 0x70d2: // [$70D2..$70D4]    3 bytes
      CYCLES(0x70d2, 6);
      /* $70D2 JMP */ s_pc = 0x7211;
      branchTarget = true;
      break;
    case 0x70d5: // [$70D5..$70D7]    3 bytes
      CYCLES(0x70d5, 6);
      /* $70D5 DEX */ s_x = update_nz(s_x - 1);
      /* $70D6 BPL */ s_pc = !(s_status & STATUS_N) ? 0x70d9 : 0x70d8;
      branchTarget = true;
      break;
    case 0x70d8: // [$70D8..$70D8]    1 bytes
      CYCLES(0x70d8, 2);
      /* $70D8 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x70d9: // [$70D9..$70DD]    5 bytes
      CYCLES(0x70d9, 9);
      /* $70D9 LDA */ s_a = update_nz(peek(0x1b90 + s_x));
      /* $70DC BMI */ s_pc = s_status & STATUS_N ? 0x70e3 : 0x70de;
      branchTarget = true;
      break;
    case 0x70de: // [$70DE..$70E2]    5 bytes
      CYCLES(0x70de, 9);
      /* $70DE STX */ poke_zpg(0x00, s_x);
      /* $70E0 JMP */ s_pc = 0x7278;
      branchTarget = true;
      break;
    case 0x70e3: // [$70E3..$70E7]    5 bytes
      CYCLES(0x70e3, 9);
      /* $70E3 DEC */ tmp16 = 0x1b60 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $70E6 BPL */ s_pc = !(s_status & STATUS_N) ? 0x70d5 : 0x70e8;
      branchTarget = true;
      break;
    case 0x70e8: // [$70E8..$70F4]   13 bytes
      CYCLES(0x70e8, 23);
      /* $70E8 LDA */ s_a = update_nz(peek(0x0c24));
      /* $70EB STA */ poke(0x1b60 + s_x, s_a);
      /* $70EE STX */ poke_zpg(0x00, s_x);
      /* $70F0 DEC */ tmp16 = 0x1b70 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $70F3 BPL */ s_pc = !(s_status & STATUS_N) ? 0x7102 : 0x70f5;
      branchTarget = true;
      break;
    case 0x70f5: // [$70F5..$70FD]    9 bytes
      CYCLES(0x70f5, 16);
      /* $70F5 LDA */ s_a = update_nz(peek(0x0c25));
      /* $70F8 STA */ poke(0x1b70 + s_x, s_a);
      /* $70FB JSR */ push16(0x70fd), s_pc = 0x72d9;
      branchTarget = true;
      break;
    case 0x70fe: // [$70FE..$7101]    4 bytes
      CYCLES(0x70fe, 7);
      /* $70FE TYA */ s_a = update_nz(s_y);
      /* $70FF STA */ poke(0x1b50 + s_x, s_a);
    case 0x7102: // [$7102..$710E]   13 bytes
      CYCLES(0x7102, 23);
      /* $7102 LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $7104 LDX */ s_x = update_nz(peek(0x1b30 + s_y));
      /* $7107 LDA */ s_a = update_nz(peek(0x1b20 + s_y));
      /* $710A LDY */ s_y = update_nz(0x0d);
      /* $710C JSR */ push16(0x710e), s_pc = 0x4d21;
      branchTarget = true;
      break;
    case 0x710f: // [$710F..$711D]   15 bytes
      CYCLES(0x710f, 26);
      /* $710F LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $7111 LDA */ s_a = update_nz(peek(0x1b40 + s_y));
      /* $7114 EOR */ s_a = update_nz(s_a ^ 0x01);
      /* $7116 STA */ poke(0x1b40 + s_y, s_a);
      /* $7119 LDX */ s_x = update_nz(peek(0x1b50 + s_y));
      /* $711C BMI */ s_pc = s_status & STATUS_N ? 0x7131 : 0x711e;
      branchTarget = true;
      break;
    case 0x711e: // [$711E..$7122]    5 bytes
      CYCLES(0x711e, 9);
      /* $711E CPX */ update_nz_inv_c(s_x - peek(0x140a));
      /* $7121 BCC */ s_pc = !(s_status & STATUS_C) ? 0x713d : 0x7123;
      branchTarget = true;
      break;
    case 0x7123: // [$7123..$7127]    5 bytes
      CYCLES(0x7123, 9);
      /* $7123 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $7125 JSR */ push16(0x7127), s_pc = 0x72d9;
      branchTarget = true;
      break;
    case 0x7128: // [$7128..$7130]    9 bytes
      CYCLES(0x7128, 16);
      /* $7128 TYA */ s_a = update_nz(s_y);
      /* $7129 STA */ poke(0x1b50 + s_x, s_a);
      /* $712C LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $712E TAX */ s_x = update_nz(s_a);
      /* $712F BPL */ s_pc = !(s_status & STATUS_N) ? 0x713d : 0x7131;
      branchTarget = true;
      break;
    case 0x7131: // [$7131..$713C]   12 bytes
      CYCLES(0x7131, 21);
      /* $7131 LDA */ s_a = update_nz(peek(0x1500));
      /* $7134 STA */ poke_zpg(0x06, s_a);
      /* $7136 LDA */ s_a = update_nz(peek(0x1501));
      /* $7139 STA */ poke_zpg(0x07, s_a);
      /* $713B BNE */ s_pc = !(s_status & STATUS_Z) ? 0x7147 : 0x713d;
      branchTarget = true;
      break;
    case 0x713d: // [$713D..$7146]   10 bytes
      CYCLES(0x713d, 18);
      /* $713D LDA */ s_a = update_nz(peek(0x1950 + s_x));
      /* $7140 STA */ poke_zpg(0x06, s_a);
      /* $7142 LDA */ s_a = update_nz(peek(0x1960 + s_x));
      /* $7145 STA */ poke_zpg(0x07, s_a);
    case 0x7147: // [$7147..$714E]    8 bytes
      CYCLES(0x7147, 14);
      /* $7147 LDA */ s_a = update_nz(peek(0x1b30 + s_y));
      /* $714A SEC */ s_status |= STATUS_C;
      /* $714B SBC */ tmp = peek_zpg(0x07), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $714D BCS */ s_pc = s_status & STATUS_C ? 0x7153 : 0x714f;
      branchTarget = true;
      break;
    case 0x714f: // [$714F..$7152]    4 bytes
      CYCLES(0x714f, 7);
      /* $714F EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $7151 ADC */ tmp = 0x01, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0x7153: // [$7153..$7156]    4 bytes
      CYCLES(0x7153, 7);
      /* $7153 CMP */ update_nz_inv_c(s_a - 0x02);
      /* $7155 BCS */ s_pc = s_status & STATUS_C ? 0x715b : 0x7157;
      branchTarget = true;
      break;
    case 0x7157: // [$7157..$715A]    4 bytes
      CYCLES(0x7157, 7);
      /* $7157 LDA */ s_a = update_nz(peek_zpg(0x07));
      /* $7159 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x7168 : 0x715b;
      branchTarget = true;
      break;
    case 0x715b: // [$715B..$7161]    7 bytes
      CYCLES(0x715b, 12);
      /* $715B LDA */ s_a = update_nz(peek(0x1b30 + s_y));
      /* $715E CMP */ update_nz_inv_c(s_a - peek_zpg(0x07));
      /* $7160 BCS */ s_pc = s_status & STATUS_C ? 0x7166 : 0x7162;
      branchTarget = true;
      break;
    case 0x7162: // [$7162..$7165]    4 bytes
      CYCLES(0x7162, 7);
      /* $7162 ADC */ tmp = 0x02, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $7164 BCC */ s_pc = !(s_status & STATUS_C) ? 0x7168 : 0x7166;
      branchTarget = true;
      break;
    case 0x7166: // [$7166..$7167]    2 bytes
      CYCLES(0x7166, 4);
      /* $7166 SBC */ tmp = 0x02, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
    case 0x7168: // [$7168..$7175]   14 bytes
      CYCLES(0x7168, 24);
      /* $7168 STA */ poke(0x1b30 + s_y, s_a);
      /* $716B LDA */ s_a = update_nz(0x00);
      /* $716D STA */ poke_zpg(0x01, s_a);
      /* $716F LDA */ s_a = update_nz(peek(0x1b20 + s_y));
      /* $7172 CMP */ update_nz_inv_c(s_a - peek_zpg(0x06));
      /* $7174 BCS */ s_pc = s_status & STATUS_C ? 0x718e : 0x7176;
      branchTarget = true;
      break;
    case 0x7176: // [$7176..$7180]   11 bytes
      CYCLES(0x7176, 19);
      /* $7176 ADC */ tmp = 0x02, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $7178 STA */ poke(0x1b20 + s_y, s_a);
      /* $717B ADC */ tmp = 0x0f, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $717D CMP */ update_nz_inv_c(s_a - peek_zpg(0x06));
      /* $717F BCC */ s_pc = !(s_status & STATUS_C) ? 0x719b : 0x7181;
      branchTarget = true;
      break;
    case 0x7181: // [$7181..$7187]    7 bytes
      CYCLES(0x7181, 12);
      /* $7181 LDA */ s_a = update_nz(peek(0x1b30 + s_y));
      /* $7184 CMP */ update_nz_inv_c(s_a - peek_zpg(0x07));
      /* $7186 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x719b : 0x7188;
      branchTarget = true;
      break;
    case 0x7188: // [$7188..$718A]    3 bytes
      CYCLES(0x7188, 6);
      /* $7188 TXA */ s_a = update_nz(s_x);
      /* $7189 BMI */ s_pc = s_status & STATUS_N ? 0x719b : 0x718b;
      branchTarget = true;
      break;
    case 0x718b: // [$718B..$718D]    3 bytes
      CYCLES(0x718b, 6);
      /* $718B JMP */ s_pc = 0x7314;
      branchTarget = true;
      break;
    case 0x718e: // [$718E..$719A]   13 bytes
      CYCLES(0x718e, 23);
      /* $718E INC */ tmp16 = 0x01, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $7190 SBC */ tmp = 0x02, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $7192 STA */ poke(0x1b20 + s_y, s_a);
      /* $7195 SBC */ tmp = 0x09, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $7197 CMP */ update_nz_inv_c(s_a - peek_zpg(0x06));
      /* $7199 BCC */ s_pc = !(s_status & STATUS_C) ? 0x7181 : 0x719b;
      branchTarget = true;
      break;
    case 0x719b: // [$719B..$71B8]   30 bytes
      CYCLES(0x719b, 52);
      /* $719B LDA */ s_a = update_nz(peek_zpg(0x01));
      /* $719D STA */ poke(0x1ba0 + s_y, s_a);
      /* $71A0 ASL */ s_a = update_nzc(s_a << 1);
      /* $71A1 ORA */ s_a = update_nz(s_a | peek(0x1b40 + s_y));
      /* $71A4 TAX */ s_x = update_nz(s_a);
      /* $71A5 LDA */ s_a = update_nz(peek(0x08e0 + s_x));
      /* $71A8 STA */ poke_zpg(0x06, s_a);
      /* $71AA LDA */ s_a = update_nz(peek(0x08e8 + s_x));
      /* $71AD STA */ poke_zpg(0x07, s_a);
      /* $71AF LDX */ s_x = update_nz(peek(0x1b20 + s_y));
      /* $71B2 LDA */ s_a = update_nz(peek(0x1b30 + s_y));
      /* $71B5 TAY */ s_y = update_nz(s_a);
      /* $71B6 JSR */ push16(0x71b8), s_pc = 0x4cf6;
      branchTarget = true;
      break;
    case 0x71b9: // [$71B9..$71BF]    7 bytes
      CYCLES(0x71b9, 12);
      /* $71B9 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $71BB DEC */ tmp16 = 0x1b80 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $71BE BPL */ s_pc = !(s_status & STATUS_N) ? 0x720e : 0x71c0;
      branchTarget = true;
      break;
    case 0x71c0: // [$71C0..$71C8]    9 bytes
      CYCLES(0x71c0, 16);
      /* $71C0 LDA */ s_a = update_nz(peek(0x141d));
      /* $71C3 ASL */ s_a = update_nzc(s_a << 1);
      /* $71C4 TAY */ s_y = update_nz(s_a);
      /* $71C5 CPY */ update_nz_inv_c(s_y - 0x10);
      /* $71C7 BCS */ s_pc = s_status & STATUS_C ? 0x720e : 0x71c9;
      branchTarget = true;
      break;
    case 0x71c9: // [$71C9..$71E3]   27 bytes
      CYCLES(0x71c9, 46);
      /* $71C9 INC */ tmp16 = 0x141d, poke(tmp16, update_nz(peek(tmp16) + 1));
      /* $71CC LDA */ s_a = update_nz(peek(0x1b20 + s_x));
      /* $71CF CLC */ s_status &= ~STATUS_C;
      /* $71D0 ADC */ tmp = 0x05, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $71D2 STA */ poke(0x1c70 + s_y, s_a);
      /* $71D5 STA */ poke(0x1c71 + s_y, s_a);
      /* $71D8 LDA */ s_a = update_nz(peek(0x1b30 + s_x));
      /* $71DB STA */ poke(0x1c80 + s_y, s_a);
      /* $71DE STA */ poke(0x1c81 + s_y, s_a);
      /* $71E1 JSR */ push16(0x71e3), s_pc = 0x768f;
      branchTarget = true;
      break;
    case 0x71e4: // [$71E4..$7208]   37 bytes
      CYCLES(0x71e4, 63);
      /* $71E4 LDA */ s_a = update_nz(peek_zpg(0x02));
      /* $71E6 STA */ poke(0x1c90 + s_y, s_a);
      /* $71E9 STA */ poke(0x1c91 + s_y, s_a);
      /* $71EC LDA */ s_a = update_nz(peek_zpg(0x03));
      /* $71EE STA */ poke(0x1ca0 + s_y, s_a);
      /* $71F1 STA */ poke(0x1ca1 + s_y, s_a);
      /* $71F4 LDA */ s_a = update_nz(peek(0x0c2e));
      /* $71F7 STA */ poke(0x1cb0 + s_y, s_a);
      /* $71FA STA */ poke(0x1cb1 + s_y, s_a);
      /* $71FD LDA */ s_a = update_nz(peek(0x0c2e));
      /* $7200 STA */ poke(0x1cc1 + s_y, s_a);
      /* $7203 LDA */ s_a = update_nz(peek(0x0c26));
      /* $7206 JSR */ push16(0x7208), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x7209: // [$7209..$720D]    5 bytes
      CYCLES(0x7209, 9);
      /* $7209 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $720B STA */ poke(0x1b80 + s_x, s_a);
    case 0x720e: // [$720E..$7210]    3 bytes
      CYCLES(0x720e, 6);
      /* $720E JMP */ s_pc = 0x70d5;
      branchTarget = true;
      break;
    case 0x7211: // [$7211..$7219]    9 bytes
      CYCLES(0x7211, 16);
      /* $7211 ASL */ s_a = update_nzc(s_a << 1);
      /* $7212 ASL */ s_a = update_nzc(s_a << 1);
      /* $7213 STA */ poke_zpg(0x0a, s_a);
      /* $7215 LDY */ s_y = update_nz(0x0d);
      /* $7217 JSR */ push16(0x7219), s_pc = 0x50a1;
      branchTarget = true;
      break;
    case 0x721a: // [$721A..$721C]    3 bytes
      CYCLES(0x721a, 6);
      /* $721A LDX */ s_x = update_nz(peek(0x1418));
    case 0x721d: // [$721D..$721F]    3 bytes
      CYCLES(0x721d, 6);
      /* $721D DEX */ s_x = update_nz(s_x - 1);
      /* $721E BPL */ s_pc = !(s_status & STATUS_N) ? 0x7224 : 0x7220;
      branchTarget = true;
      break;
    case 0x7220: // [$7220..$7223]    4 bytes
      CYCLES(0x7220, 7);
      /* $7220 DEC */ tmp16 = 0x141c, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $7223 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x7224: // [$7224..$723E]   27 bytes
      CYCLES(0x7224, 46);
      /* $7224 STX */ poke_zpg(0x00, s_x);
      /* $7226 LDA */ s_a = update_nz(peek(0x1ba0 + s_x));
      /* $7229 ASL */ s_a = update_nzc(s_a << 1);
      /* $722A TAY */ s_y = update_nz(s_a);
      /* $722B LDA */ s_a = update_nz(peek(0x08e0 + s_y));
      /* $722E STA */ poke_zpg(0x06, s_a);
      /* $7230 LDA */ s_a = update_nz(peek(0x08e8 + s_y));
      /* $7233 STA */ poke_zpg(0x07, s_a);
      /* $7235 LDY */ s_y = update_nz(peek(0x1b30 + s_x));
      /* $7238 LDA */ s_a = update_nz(peek(0x1b20 + s_x));
      /* $723B TAX */ s_x = update_nz(s_a);
      /* $723C JSR */ push16(0x723e), s_pc = 0x4c7c;
      branchTarget = true;
      break;
    case 0x723f: // [$723F..$7242]    4 bytes
      CYCLES(0x723f, 7);
      /* $723F LDA */ s_a = update_nz(0x00);
      /* $7241 STA */ poke_zpg(0x20, s_a);
    case 0x7243: // [$7243..$726F]   45 bytes
      CYCLES(0x7243, 77);
      /* $7243 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $7245 STA */ poke_zpg(0x06, s_a);
      /* $7247 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $7249 STA */ poke_zpg(0x07, s_a);
      /* $724B STY */ poke_zpg(0x05, s_y);
      /* $724D LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $724F LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $7251 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x20 + s_x)));
      /* $7253 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $7255 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $7257 INY */ s_y = update_nz(s_y + 1);
      /* $7258 LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $725A AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x20 + s_x)));
      /* $725C STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $725E INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $7260 INY */ s_y = update_nz(s_y + 1);
      /* $7261 LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $7263 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x20 + s_x)));
      /* $7265 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $7267 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $7269 INC */ tmp16 = 0x20, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $726B LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $726D DEY */ s_y = update_nz(s_y - 1);
      /* $726E BPL */ s_pc = !(s_status & STATUS_N) ? 0x7243 : 0x7270;
      branchTarget = true;
      break;
    case 0x7270: // [$7270..$7272]    3 bytes
      CYCLES(0x7270, 6);
      /* $7270 JSR */ push16(0x7272), s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x7273: // [$7273..$7277]    5 bytes
      CYCLES(0x7273, 9);
      /* $7273 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $7275 JMP */ s_pc = 0x721d;
      branchTarget = true;
      break;
    case 0x7278: // [$7278..$7284]   13 bytes
      CYCLES(0x7278, 23);
      /* $7278 LDY */ s_y = update_nz(peek(0x0c3d));
      /* $727B LDA */ s_a = update_nz(peek(0x1b40 + s_x));
      /* $727E EOR */ s_a = update_nz(s_a ^ 0x01);
      /* $7280 STA */ poke(0x1b40 + s_x, s_a);
      /* $7283 BEQ */ s_pc = s_status & STATUS_Z ? 0x7288 : 0x7285;
      branchTarget = true;
      break;
    case 0x7285: // [$7285..$7287]    3 bytes
      CYCLES(0x7285, 6);
      /* $7285 LDY */ s_y = update_nz(peek(0x0c3e));
    case 0x7288: // [$7288..$72A2]   27 bytes
      CYCLES(0x7288, 46);
      /* $7288 STY */ poke_zpg(0x02, s_y);
      /* $728A LDA */ s_a = update_nz(peek(0x1ba0 + s_x));
      /* $728D ASL */ s_a = update_nzc(s_a << 1);
      /* $728E TAY */ s_y = update_nz(s_a);
      /* $728F LDA */ s_a = update_nz(peek(0x08e0 + s_y));
      /* $7292 STA */ poke_zpg(0x06, s_a);
      /* $7294 LDA */ s_a = update_nz(peek(0x08e8 + s_y));
      /* $7297 STA */ poke_zpg(0x07, s_a);
      /* $7299 LDY */ s_y = update_nz(peek(0x1b30 + s_x));
      /* $729C LDA */ s_a = update_nz(peek(0x1b20 + s_x));
      /* $729F TAX */ s_x = update_nz(s_a);
      /* $72A0 JSR */ push16(0x72a2), s_pc = 0x4c7c;
      branchTarget = true;
      break;
    case 0x72a3: // [$72A3..$72CD]   43 bytes
      CYCLES(0x72a3, 74);
      /* $72A3 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $72A5 STA */ poke_zpg(0x06, s_a);
      /* $72A7 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $72A9 STA */ poke_zpg(0x07, s_a);
      /* $72AB STY */ poke_zpg(0x05, s_y);
      /* $72AD LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $72AF LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $72B1 EOR */ s_a = update_nz(s_a ^ peek_zpg(0x02));
      /* $72B3 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $72B5 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $72B7 INY */ s_y = update_nz(s_y + 1);
      /* $72B8 LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $72BA EOR */ s_a = update_nz(s_a ^ peek_zpg(0x02));
      /* $72BC STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $72BE INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $72C0 INY */ s_y = update_nz(s_y + 1);
      /* $72C1 LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $72C3 EOR */ s_a = update_nz(s_a ^ peek_zpg(0x02));
      /* $72C5 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $72C7 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $72C9 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $72CB DEY */ s_y = update_nz(s_y - 1);
      /* $72CC BPL */ s_pc = !(s_status & STATUS_N) ? 0x72a3 : 0x72ce;
      branchTarget = true;
      break;
    case 0x72ce: // [$72CE..$72D5]    8 bytes
      CYCLES(0x72ce, 14);
      /* $72CE LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $72D0 DEC */ tmp16 = 0x1b90 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $72D3 JSR */ push16(0x72d5), s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x72d6: // [$72D6..$72D8]    3 bytes
      CYCLES(0x72d6, 6);
      /* $72D6 JMP */ s_pc = 0x70d5;
      branchTarget = true;
      break;
    case 0x72d9: // [$72D9..$72DD]    5 bytes
      CYCLES(0x72d9, 9);
      /* $72D9 LDY */ s_y = update_nz(peek(0x140a));
      /* $72DC BNE */ s_pc = !(s_status & STATUS_Z) ? 0x72e0 : 0x72de;
      branchTarget = true;
      break;
    case 0x72de: // [$72DE..$72DF]    2 bytes
      CYCLES(0x72de, 4);
      /* $72DE DEY */ s_y = update_nz(s_y - 1);
      /* $72DF RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x72e0: // [$72E0..$72E7]    8 bytes
      CYCLES(0x72e0, 14);
      /* $72E0 LDA */ s_a = update_nz(0xff);
      /* $72E2 STA */ poke_zpg(0x06, s_a);
      /* $72E4 LDA */ s_a = update_nz(0x00);
      /* $72E6 STA */ poke_zpg(0x07, s_a);
    case 0x72e8: // [$72E8..$72EA]    3 bytes
      CYCLES(0x72e8, 6);
      /* $72E8 DEY */ s_y = update_nz(s_y - 1);
      /* $72E9 BMI */ s_pc = s_status & STATUS_N ? 0x7311 : 0x72eb;
      branchTarget = true;
      break;
    case 0x72eb: // [$72EB..$72F2]    8 bytes
      CYCLES(0x72eb, 14);
      /* $72EB LDA */ s_a = update_nz(peek(0x1950 + s_y));
      /* $72EE SBC */ tmp = peek(0x1b20 + s_x), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $72F1 BCS */ s_pc = s_status & STATUS_C ? 0x72f5 : 0x72f3;
      branchTarget = true;
      break;
    case 0x72f3: // [$72F3..$72F4]    2 bytes
      CYCLES(0x72f3, 4);
      /* $72F3 EOR */ s_a = update_nz(s_a ^ 0xff);
    case 0x72f5: // [$72F5..$72FE]   10 bytes
      CYCLES(0x72f5, 18);
      /* $72F5 STA */ poke_zpg(0x08, s_a);
      /* $72F7 LDA */ s_a = update_nz(peek(0x1960 + s_y));
      /* $72FA SBC */ tmp = peek(0x1b30 + s_x), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $72FD BCS */ s_pc = s_status & STATUS_C ? 0x7301 : 0x72ff;
      branchTarget = true;
      break;
    case 0x72ff: // [$72FF..$7300]    2 bytes
      CYCLES(0x72ff, 4);
      /* $72FF EOR */ s_a = update_nz(s_a ^ 0xff);
    case 0x7301: // [$7301..$7304]    4 bytes
      CYCLES(0x7301, 7);
      /* $7301 ADC */ tmp = peek_zpg(0x08), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $7303 BCC */ s_pc = !(s_status & STATUS_C) ? 0x7307 : 0x7305;
      branchTarget = true;
      break;
    case 0x7305: // [$7305..$7306]    2 bytes
      CYCLES(0x7305, 4);
      /* $7305 LDA */ s_a = update_nz(0xff);
    case 0x7307: // [$7307..$730A]    4 bytes
      CYCLES(0x7307, 7);
      /* $7307 CMP */ update_nz_inv_c(s_a - peek_zpg(0x06));
      /* $7309 BCS */ s_pc = s_status & STATUS_C ? 0x72e8 : 0x730b;
      branchTarget = true;
      break;
    case 0x730b: // [$730B..$7310]    6 bytes
      CYCLES(0x730b, 11);
      /* $730B STA */ poke_zpg(0x06, s_a);
      /* $730D STY */ poke_zpg(0x07, s_y);
      /* $730F BCC */ s_pc = !(s_status & STATUS_C) ? 0x72e8 : 0x7311;
      branchTarget = true;
      break;
    case 0x7311: // [$7311..$7313]    3 bytes
      CYCLES(0x7311, 6);
      /* $7311 LDY */ s_y = update_nz(peek_zpg(0x07));
      /* $7313 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x7314: // [$7314..$7324]   17 bytes
      CYCLES(0x7314, 29);
      /* $7314 LDA */ s_a = update_nz(peek(0x1990 + s_x));
      /* $7317 PHA */ push8(s_a);
      /* $7318 TXA */ s_a = update_nz(s_x);
      /* $7319 TAY */ s_y = update_nz(s_a);
      /* $731A LDX */ s_x = update_nz(peek(0x1960 + s_y));
      /* $731D LDA */ s_a = update_nz(peek(0x1950 + s_y));
      /* $7320 LDY */ s_y = update_nz(0x0a);
      /* $7322 JSR */ push16(0x7324), s_pc = 0x4cd0;
      branchTarget = true;
      break;
    case 0x7325: // [$7325..$732C]    8 bytes
      CYCLES(0x7325, 14);
      /* $7325 LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $7327 LDX */ s_x = update_nz(peek(0x1b50 + s_y));
      /* $732A JSR */ push16(0x732c), s_pc = 0x5a21;
      branchTarget = true;
      break;
    case 0x732d: // [$732D..$7334]    8 bytes
      CYCLES(0x732d, 14);
      /* $732D LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $732F LDA */ s_a = update_nz(peek(0x141b));
      /* $7332 ASL */ s_a = update_nzc(s_a << 1);
      /* $7333 STA */ poke_zpg(0x01, s_a);
    case 0x7335: // [$7335..$733C]    8 bytes
      CYCLES(0x7335, 14);
      /* $7335 LDA */ s_a = update_nz(peek(0x1b20 + s_y));
      /* $7338 LDX */ s_x = update_nz(peek(0x1ba0 + s_y));
      /* $733B BNE */ s_pc = !(s_status & STATUS_Z) ? 0x734e : 0x733d;
      branchTarget = true;
      break;
    case 0x733d: // [$733D..$7341]    5 bytes
      CYCLES(0x733d, 9);
      /* $733D CLC */ s_status &= ~STATUS_C;
      /* $733E ADC */ tmp = 0x12, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $7340 BCS */ s_pc = s_status & STATUS_C ? 0x7346 : 0x7342;
      branchTarget = true;
      break;
    case 0x7342: // [$7342..$7345]    4 bytes
      CYCLES(0x7342, 7);
      /* $7342 CMP */ update_nz_inv_c(s_a - 0xf3);
      /* $7344 BCC */ s_pc = !(s_status & STATUS_C) ? 0x7357 : 0x7346;
      branchTarget = true;
      break;
    case 0x7346: // [$7346..$734D]    8 bytes
      CYCLES(0x7346, 14);
      /* $7346 TXA */ s_a = update_nz(s_x);
      /* $7347 EOR */ s_a = update_nz(s_a ^ 0x01);
      /* $7349 STA */ poke(0x1ba0 + s_y, s_a);
      /* $734C BPL */ s_pc = !(s_status & STATUS_N) ? 0x7335 : 0x734e;
      branchTarget = true;
      break;
    case 0x734e: // [$734E..$7352]    5 bytes
      CYCLES(0x734e, 9);
      /* $734E SEC */ s_status |= STATUS_C;
      /* $734F SBC */ tmp = 0x0c, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $7351 BCC */ s_pc = !(s_status & STATUS_C) ? 0x7346 : 0x7353;
      branchTarget = true;
      break;
    case 0x7353: // [$7353..$7356]    4 bytes
      CYCLES(0x7353, 7);
      /* $7353 CMP */ update_nz_inv_c(s_a - 0x06);
      /* $7355 BCC */ s_pc = !(s_status & STATUS_C) ? 0x7346 : 0x7357;
      branchTarget = true;
      break;
    case 0x7357: // [$7357..$73A2]   76 bytes
      CYCLES(0x7357, 130);
      /* $7357 LDX */ s_x = update_nz(peek_zpg(0x01));
      /* $7359 STA */ poke(0x1bb0 + s_x, s_a);
      /* $735C STA */ poke(0x1bb1 + s_x, s_a);
      /* $735F LDA */ s_a = update_nz(peek(0x1b30 + s_y));
      /* $7362 STA */ poke(0x1bd0 + s_x, s_a);
      /* $7365 STA */ poke(0x1bd1 + s_x, s_a);
      /* $7368 LDA */ s_a = update_nz(peek(0x1ba0 + s_y));
      /* $736B STA */ poke(0x1bf0 + s_x, s_a);
      /* $736E STA */ poke(0x1bf1 + s_x, s_a);
      /* $7371 STA */ poke(0x1c30 + s_x, s_a);
      /* $7374 LDA */ s_a = update_nz(0x70);
      /* $7376 STA */ poke(0x1c10 + s_x, s_a);
      /* $7379 STA */ poke(0x1c11 + s_x, s_a);
      /* $737C PLA */ s_a = update_nz(pop8());
      /* $737D STA */ poke(0x1c50 + s_x, s_a);
      /* $7380 LDA */ s_a = update_nz(peek(0x0c2a));
      /* $7383 SEC */ s_status |= STATUS_C;
      /* $7384 SBC */ tmp = 0x01, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $7386 STA */ poke(0x1c31 + s_x, s_a);
      /* $7389 LDA */ s_a = update_nz(peek(0x0c28));
      /* $738C STA */ poke(0x1419, s_a);
      /* $738F STA */ poke(0x1b90 + s_y, s_a);
      /* $7392 STA */ poke(0x1c51 + s_x, s_a);
      /* $7395 LDA */ s_a = update_nz(peek(0x0c23));
      /* $7398 STA */ poke(0x141a, s_a);
      /* $739B INC */ tmp16 = 0x141b, poke(tmp16, update_nz(peek(tmp16) + 1));
      /* $739E LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $73A0 JMP */ s_pc = 0x70d5;
      branchTarget = true;
      break;
    case 0x73a3: // [$73A3..$73AC]   10 bytes
      CYCLES(0x73a3, 18);
      /* $73A3 LDX */ s_x = update_nz(peek(0x1418));
      /* $73A6 LDA */ s_a = update_nz(peek_zpg(0x1c));
      /* $73A8 SEC */ s_status |= STATUS_C;
      /* $73A9 SBC */ tmp = 0x0d, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $73AB BCS */ s_pc = s_status & STATUS_C ? 0x73af : 0x73ad;
      branchTarget = true;
      break;
    case 0x73ad: // [$73AD..$73AE]    2 bytes
      CYCLES(0x73ad, 4);
      /* $73AD LDA */ s_a = update_nz(0x00);
    case 0x73af: // [$73AF..$73AF]    1 bytes
      CYCLES(0x73af, 2);
      /* $73AF TAY */ s_y = update_nz(s_a);
    case 0x73b0: // [$73B0..$73B1]    2 bytes
      CYCLES(0x73b0, 4);
      /* $73B0 LDA */ s_a = update_nz(peek_zpg(0x1c));
    case 0x73b2: // [$73B2..$73B4]    3 bytes
      CYCLES(0x73b2, 6);
      /* $73B2 DEX */ s_x = update_nz(s_x - 1);
      /* $73B3 BPL */ s_pc = !(s_status & STATUS_N) ? 0x73b7 : 0x73b5;
      branchTarget = true;
      break;
    case 0x73b5: // [$73B5..$73B6]    2 bytes
      CYCLES(0x73b5, 4);
      /* $73B5 CLC */ s_status &= ~STATUS_C;
      /* $73B6 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x73b7: // [$73B7..$73BB]    5 bytes
      CYCLES(0x73b7, 9);
      /* $73B7 CMP */ update_nz_inv_c(s_a - peek(0x1b20 + s_x));
      /* $73BA BCC */ s_pc = !(s_status & STATUS_C) ? 0x73b2 : 0x73bc;
      branchTarget = true;
      break;
    case 0x73bc: // [$73BC..$73C1]    6 bytes
      CYCLES(0x73bc, 11);
      /* $73BC TYA */ s_a = update_nz(s_y);
      /* $73BD CMP */ update_nz_inv_c(s_a - peek(0x1b20 + s_x));
      /* $73C0 BCS */ s_pc = s_status & STATUS_C ? 0x73b0 : 0x73c2;
      branchTarget = true;
      break;
    case 0x73c2: // [$73C2..$73C8]    7 bytes
      CYCLES(0x73c2, 12);
      /* $73C2 LDA */ s_a = update_nz(peek_zpg(0x1d));
      /* $73C4 CMP */ update_nz_inv_c(s_a - peek(0x1b30 + s_x));
      /* $73C7 BCC */ s_pc = !(s_status & STATUS_C) ? 0x73b0 : 0x73c9;
      branchTarget = true;
      break;
    case 0x73c9: // [$73C9..$73CC]    4 bytes
      CYCLES(0x73c9, 7);
      /* $73C9 SBC */ tmp = 0x0d, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $73CB BCC */ s_pc = !(s_status & STATUS_C) ? 0x73d2 : 0x73cd;
      branchTarget = true;
      break;
    case 0x73cd: // [$73CD..$73D1]    5 bytes
      CYCLES(0x73cd, 9);
      /* $73CD CMP */ update_nz_inv_c(s_a - peek(0x1b30 + s_x));
      /* $73D0 BCS */ s_pc = s_status & STATUS_C ? 0x73b0 : 0x73d2;
      branchTarget = true;
      break;
    case 0x73d2: // [$73D2..$73D5]    4 bytes
      CYCLES(0x73d2, 7);
      /* $73D2 LDA */ s_a = update_nz(peek_zpg(0xe8));
      /* $73D4 BEQ */ s_pc = s_status & STATUS_Z ? 0x73d8 : 0x73d6;
      branchTarget = true;
      break;
    case 0x73d6: // [$73D6..$73D7]    2 bytes
      CYCLES(0x73d6, 4);
      /* $73D6 SEC */ s_status |= STATUS_C;
      /* $73D7 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x73d8: // [$73D8..$73E6]   15 bytes
      CYCLES(0x73d8, 26);
      /* $73D8 STX */ poke_zpg(0x00, s_x);
      /* $73DA LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $73DC LDX */ s_x = update_nz(peek(0x1b30 + s_y));
      /* $73DF LDA */ s_a = update_nz(peek(0x1b20 + s_y));
      /* $73E2 LDY */ s_y = update_nz(0x0d);
      /* $73E4 JSR */ push16(0x73e6), s_pc = 0x4d21;
      branchTarget = true;
      break;
    case 0x73e7: // [$73E7..$73FA]   20 bytes
      CYCLES(0x73e7, 35);
      /* $73E7 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $73E9 LDA */ s_a = update_nz(peek(0x1b30 + s_x));
      /* $73EC CLC */ s_status &= ~STATUS_C;
      /* $73ED ADC */ tmp = 0x06, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $73EF TAY */ s_y = update_nz(s_a);
      /* $73F0 LDA */ s_a = update_nz(peek(0x1b20 + s_x));
      /* $73F3 ADC */ tmp = 0x03, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $73F5 TAX */ s_x = update_nz(s_a);
      /* $73F6 LDA */ s_a = update_nz(0xd5);
      /* $73F8 JSR */ push16(0x73fa), s_pc = 0x4f74;
      branchTarget = true;
      break;
    case 0x73fb: // [$73FB..$7401]    7 bytes
      CYCLES(0x73fb, 12);
      /* $73FB LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $73FD LDA */ s_a = update_nz(peek(0x1b90 + s_x));
      /* $7400 BMI */ s_pc = s_status & STATUS_N ? 0x7441 : 0x7402;
      branchTarget = true;
      break;
    case 0x7402: // [$7402..$7406]    5 bytes
      CYCLES(0x7402, 9);
      /* $7402 LDA */ s_a = update_nz(peek(0x141b));
      /* $7405 ASL */ s_a = update_nzc(s_a << 1);
      /* $7406 TAY */ s_y = update_nz(s_a);
    case 0x7407: // [$7407..$740A]    4 bytes
      CYCLES(0x7407, 7);
      /* $7407 DEY */ s_y = update_nz(s_y - 1);
      /* $7408 DEY */ s_y = update_nz(s_y - 1);
      /* $7409 BMI */ s_pc = s_status & STATUS_N ? 0x743f : 0x740b;
      branchTarget = true;
      break;
    case 0x740b: // [$740B..$740F]    5 bytes
      CYCLES(0x740b, 9);
      /* $740B LDA */ s_a = update_nz(peek(0x1c51 + s_y));
      /* $740E BMI */ s_pc = s_status & STATUS_N ? 0x7407 : 0x7410;
      branchTarget = true;
      break;
    case 0x7410: // [$7410..$7412]    3 bytes
      CYCLES(0x7410, 6);
      /* $7410 JSR */ push16(0x7412), s_pc = 0x7979;
      branchTarget = true;
      break;
    case 0x7413: // [$7413..$7414]    2 bytes
      CYCLES(0x7413, 4);
      /* $7413 BCC */ s_pc = !(s_status & STATUS_C) ? 0x7407 : 0x7415;
      branchTarget = true;
      break;
    case 0x7415: // [$7415..$7423]   15 bytes
      CYCLES(0x7415, 26);
      /* $7415 STY */ poke_zpg(0x01, s_y);
      /* $7417 LDX */ s_x = update_nz(peek(0x1bd0 + s_y));
      /* $741A DEX */ s_x = update_nz(s_x - 1);
      /* $741B DEX */ s_x = update_nz(s_x - 1);
      /* $741C LDA */ s_a = update_nz(peek(0x1bb0 + s_y));
      /* $741F LDY */ s_y = update_nz(0x0e);
      /* $7421 JSR */ push16(0x7423), s_pc = 0x4cd0;
      branchTarget = true;
      break;
    case 0x7424: // [$7424..$742F]   12 bytes
      CYCLES(0x7424, 21);
      /* $7424 LDX */ s_x = update_nz(peek_zpg(0x01));
      /* $7426 LDY */ s_y = update_nz(peek(0x1bd0 + s_x));
      /* $7429 LDA */ s_a = update_nz(peek(0x1bb0 + s_x));
      /* $742C TAX */ s_x = update_nz(s_a);
      /* $742D JSR */ push16(0x742f), s_pc = 0x5a24;
      branchTarget = true;
      break;
    case 0x7430: // [$7430..$7434]    5 bytes
      CYCLES(0x7430, 9);
      /* $7430 LDX */ s_x = update_nz(peek_zpg(0x01));
      /* $7432 JSR */ push16(0x7434), s_pc = 0x7942;
      branchTarget = true;
      break;
    case 0x7435: // [$7435..$743E]   10 bytes
      CYCLES(0x7435, 18);
      /* $7435 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $7437 LDA */ s_a = update_nz(0xff);
      /* $7439 STA */ poke(0x1b90 + s_x, s_a);
      /* $743C JSR */ push16(0x743e), s_pc = 0x7459;
      branchTarget = true;
      break;
    case 0x743f: // [$743F..$7440]    2 bytes
      CYCLES(0x743f, 4);
      /* $743F LDX */ s_x = update_nz(peek_zpg(0x00));
    case 0x7441: // [$7441..$7443]    3 bytes
      CYCLES(0x7441, 6);
      /* $7441 JSR */ push16(0x7443), s_pc = 0x7479;
      branchTarget = true;
      break;
    case 0x7444: // [$7444..$744A]    7 bytes
      CYCLES(0x7444, 12);
      /* $7444 LDA */ s_a = update_nz(0x01);
      /* $7446 LDX */ s_x = update_nz(0xf4);
      /* $7448 JSR */ push16(0x744a), s_pc = 0x4dd0;
      branchTarget = true;
      break;
    case 0x744b: // [$744B..$7458]   14 bytes
      CYCLES(0x744b, 24);
      /* $744B LDA */ s_a = update_nz(peek(0x0c32));
      /* $744E STA */ poke(0x141e, s_a);
      /* $7451 LDA */ s_a = update_nz(peek(0x0c31));
      /* $7454 STA */ poke(0x141f, s_a);
      /* $7457 SEC */ s_status |= STATUS_C;
      /* $7458 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x7459: // [$7459..$745D]    5 bytes
      CYCLES(0x7459, 9);
      /* $7459 LDA */ s_a = update_nz(peek(0x1419));
      /* $745C BEQ */ s_pc = s_status & STATUS_Z ? 0x7478 : 0x745e;
      branchTarget = true;
      break;
    case 0x745e: // [$745E..$7462]    5 bytes
      CYCLES(0x745e, 9);
      /* $745E LDX */ s_x = update_nz(peek(0x1418));
      /* $7461 LDA */ s_a = update_nz(0x00);
    case 0x7463: // [$7463..$7465]    3 bytes
      CYCLES(0x7463, 6);
      /* $7463 DEX */ s_x = update_nz(s_x - 1);
      /* $7464 BMI */ s_pc = s_status & STATUS_N ? 0x7475 : 0x7466;
      branchTarget = true;
      break;
    case 0x7466: // [$7466..$746A]    5 bytes
      CYCLES(0x7466, 9);
      /* $7466 LDY */ s_y = update_nz(peek(0x1b90 + s_x));
      /* $7469 BMI */ s_pc = s_status & STATUS_N ? 0x7463 : 0x746b;
      branchTarget = true;
      break;
    case 0x746b: // [$746B..$746F]    5 bytes
      CYCLES(0x746b, 9);
      /* $746B CMP */ update_nz_inv_c(s_a - peek(0x1b90 + s_x));
      /* $746E BCS */ s_pc = s_status & STATUS_C ? 0x7463 : 0x7470;
      branchTarget = true;
      break;
    case 0x7470: // [$7470..$7474]    5 bytes
      CYCLES(0x7470, 9);
      /* $7470 LDA */ s_a = update_nz(peek(0x1b90 + s_x));
      /* $7473 BPL */ s_pc = !(s_status & STATUS_N) ? 0x7463 : 0x7475;
      branchTarget = true;
      break;
    case 0x7475: // [$7475..$7477]    3 bytes
      CYCLES(0x7475, 6);
      /* $7475 STA */ poke(0x1419, s_a);
    case 0x7478: // [$7478..$7478]    1 bytes
      CYCLES(0x7478, 2);
      /* $7478 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x7479: // [$7479..$747D]    5 bytes
      CYCLES(0x7479, 9);
      /* $7479 TXA */ s_a = update_nz(s_x);
      /* $747A PHA */ push8(s_a);
      /* $747B DEC */ tmp16 = 0x1418, poke(tmp16, update_nz(peek(tmp16) - 1));
    case 0x747e: // [$747E..$74B9]   60 bytes
      CYCLES(0x747e, 103);
      /* $747E LDA */ s_a = update_nz(peek(0x1b21 + s_x));
      /* $7481 STA */ poke(0x1b20 + s_x, s_a);
      /* $7484 LDA */ s_a = update_nz(peek(0x1b31 + s_x));
      /* $7487 STA */ poke(0x1b30 + s_x, s_a);
      /* $748A LDA */ s_a = update_nz(peek(0x1b41 + s_x));
      /* $748D STA */ poke(0x1b40 + s_x, s_a);
      /* $7490 LDA */ s_a = update_nz(peek(0x1b51 + s_x));
      /* $7493 STA */ poke(0x1b50 + s_x, s_a);
      /* $7496 LDA */ s_a = update_nz(peek(0x1b61 + s_x));
      /* $7499 STA */ poke(0x1b60 + s_x, s_a);
      /* $749C LDA */ s_a = update_nz(peek(0x1b71 + s_x));
      /* $749F STA */ poke(0x1b70 + s_x, s_a);
      /* $74A2 LDA */ s_a = update_nz(peek(0x1b81 + s_x));
      /* $74A5 STA */ poke(0x1b80 + s_x, s_a);
      /* $74A8 LDA */ s_a = update_nz(peek(0x1b91 + s_x));
      /* $74AB STA */ poke(0x1b90 + s_x, s_a);
      /* $74AE LDA */ s_a = update_nz(peek(0x1ba1 + s_x));
      /* $74B1 STA */ poke(0x1ba0 + s_x, s_a);
      /* $74B4 INX */ s_x = update_nz(s_x + 1);
      /* $74B5 CPX */ update_nz_inv_c(s_x - peek(0x1418));
      /* $74B8 BCC */ s_pc = !(s_status & STATUS_C) ? 0x747e : 0x74ba;
      branchTarget = true;
      break;
    case 0x74ba: // [$74BA..$74BC]    3 bytes
      CYCLES(0x74ba, 6);
      /* $74BA PLA */ s_a = update_nz(pop8());
      /* $74BB TAX */ s_x = update_nz(s_a);
      /* $74BC RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x74bd: // [$74BD..$74C7]   11 bytes
      CYCLES(0x74bd, 19);
      /* $74BD LDA */ s_a = update_nz(0x00);
      /* $74BF STA */ poke_zpg(0xfc, s_a);
      /* $74C1 STA */ poke_zpg(0xfe, s_a);
      /* $74C3 LDA */ s_a = update_nz(peek(0x141d));
      /* $74C6 ASL */ s_a = update_nzc(s_a << 1);
      /* $74C7 TAX */ s_x = update_nz(s_a);
    case 0x74c8: // [$74C8..$74CB]    4 bytes
      CYCLES(0x74c8, 7);
      /* $74C8 DEX */ s_x = update_nz(s_x - 1);
      /* $74C9 DEX */ s_x = update_nz(s_x - 1);
      /* $74CA BPL */ s_pc = !(s_status & STATUS_N) ? 0x74cd : 0x74cc;
      branchTarget = true;
      break;
    case 0x74cc: // [$74CC..$74CC]    1 bytes
      CYCLES(0x74cc, 2);
      /* $74CC RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x74cd: // [$74CD..$74D1]    5 bytes
      CYCLES(0x74cd, 9);
      /* $74CD STX */ poke_zpg(0x00, s_x);
      /* $74CF JSR */ push16(0x74d1), s_pc = 0x7628;
      branchTarget = true;
      break;
    case 0x74d2: // [$74D2..$74D6]    5 bytes
      CYCLES(0x74d2, 9);
      /* $74D2 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $74D4 JSR */ push16(0x74d6), s_pc = 0x7546;
      branchTarget = true;
      break;
    case 0x74d7: // [$74D7..$74DF]    9 bytes
      CYCLES(0x74d7, 16);
      /* $74D7 LDY */ s_y = update_nz(peek(0x1c80 + s_x));
      /* $74DA LDA */ s_a = update_nz(peek(0x1c70 + s_x));
      /* $74DD JSR */ push16(0x74df), s_pc = 0x4c1c;
      branchTarget = true;
      break;
    case 0x74e0: // [$74E0..$74E4]    5 bytes
      CYCLES(0x74e0, 9);
      /* $74E0 STA */ poke_zpg(0x04, s_a);
      /* $74E2 LDA */ s_a = update_nz(0x00);
      /* $74E4 SEC */ s_status |= STATUS_C;
    case 0x74e5: // [$74E5..$74E8]    4 bytes
      CYCLES(0x74e5, 7);
      /* $74E5 ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $74E6 DEX */ s_x = update_nz(s_x - 1);
      /* $74E7 BPL */ s_pc = !(s_status & STATUS_N) ? 0x74e5 : 0x74e9;
      branchTarget = true;
      break;
    case 0x74e9: // [$74E9..$74EC]    4 bytes
      CYCLES(0x74e9, 7);
      /* $74E9 STA */ poke_zpg(0x0a, s_a);
      /* $74EB LDX */ s_x = update_nz(0x02);
    case 0x74ed: // [$74ED..$7503]   23 bytes
      CYCLES(0x74ed, 40);
      /* $74ED LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $74EF STA */ poke_zpg(0x06, s_a);
      /* $74F1 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $74F3 STA */ poke_zpg(0x07, s_a);
      /* $74F5 STY */ poke_zpg(0x05, s_y);
      /* $74F7 LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $74F9 LDA */ s_a = update_nz(peek_zpg(0x0a));
      /* $74FB ORA */ s_a = update_nz(s_a | peek(peek16_zpg(0x06) + s_y));
      /* $74FD STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $74FF LDA */ s_a = update_nz(peek_zpg(0x0a));
      /* $7501 ASL */ s_a = update_nzc(s_a << 1);
      /* $7502 BPL */ s_pc = !(s_status & STATUS_N) ? 0x7507 : 0x7504;
      branchTarget = true;
      break;
    case 0x7504: // [$7504..$7506]    3 bytes
      CYCLES(0x7504, 6);
      /* $7504 INY */ s_y = update_nz(s_y + 1);
      /* $7505 LDA */ s_a = update_nz(0x01);
    case 0x7507: // [$7507..$7510]   10 bytes
      CYCLES(0x7507, 18);
      /* $7507 ORA */ s_a = update_nz(s_a | peek(peek16_zpg(0x06) + s_y));
      /* $7509 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $750B LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $750D INY */ s_y = update_nz(s_y + 1);
      /* $750E DEX */ s_x = update_nz(s_x - 1);
      /* $750F BNE */ s_pc = !(s_status & STATUS_Z) ? 0x74ed : 0x7511;
      branchTarget = true;
      break;
    case 0x7511: // [$7511..$7517]    7 bytes
      CYCLES(0x7511, 12);
      /* $7511 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $7513 DEC */ tmp16 = 0x1cb0 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $7516 BPL */ s_pc = !(s_status & STATUS_N) ? 0x74c8 : 0x7518;
      branchTarget = true;
      break;
    case 0x7518: // [$7518..$751C]    5 bytes
      CYCLES(0x7518, 9);
      /* $7518 LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $751A JSR */ push16(0x751c), s_pc = 0x768f;
      branchTarget = true;
      break;
    case 0x751d: // [$751D..$7532]   22 bytes
      CYCLES(0x751d, 38);
      /* $751D LDA */ s_a = update_nz(peek_zpg(0x02));
      /* $751F STA */ poke(0x1c90 + s_y, s_a);
      /* $7522 STA */ poke(0x1cc0 + s_y, s_a);
      /* $7525 LDA */ s_a = update_nz(peek_zpg(0x03));
      /* $7527 STA */ poke(0x1ca0 + s_y, s_a);
      /* $752A STA */ poke(0x1cd0 + s_y, s_a);
      /* $752D LDA */ s_a = update_nz(peek(0x0c2d));
      /* $7530 JSR */ push16(0x7532), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x7533: // [$7533..$7545]   19 bytes
      CYCLES(0x7533, 33);
      /* $7533 CLC */ s_status &= ~STATUS_C;
      /* $7534 ADC */ tmp = peek(0x0c2e), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $7537 STA */ poke(0x1cb0 + s_y, s_a);
      /* $753A INY */ s_y = update_nz(s_y + 1);
      /* $753B LDA */ s_a = update_nz(peek(0x0c2e));
      /* $753E STA */ poke(0x1cb0 + s_y, s_a);
      /* $7541 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $7543 JMP */ s_pc = 0x74c8;
      branchTarget = true;
      break;
    case 0x7546: // [$7546..$7550]   11 bytes
      CYCLES(0x7546, 19);
      /* $7546 LDA */ s_a = update_nz(peek(0x1c70 + s_x));
      /* $7549 CLC */ s_status &= ~STATUS_C;
      /* $754A ADC */ tmp = peek(0x1c90 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $754D CMP */ update_nz_inv_c(s_a - 0x02);
      /* $754F BCC */ s_pc = !(s_status & STATUS_C) ? 0x7555 : 0x7551;
      branchTarget = true;
      break;
    case 0x7551: // [$7551..$7554]    4 bytes
      CYCLES(0x7551, 7);
      /* $7551 CMP */ update_nz_inv_c(s_a - 0xfe);
      /* $7553 BCC */ s_pc = !(s_status & STATUS_C) ? 0x7562 : 0x7555;
      branchTarget = true;
      break;
    case 0x7555: // [$7555..$7561]   13 bytes
      CYCLES(0x7555, 23);
      /* $7555 LDA */ s_a = update_nz(peek(0x1c90 + s_x));
      /* $7558 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $755A CLC */ s_status &= ~STATUS_C;
      /* $755B ADC */ tmp = 0x01, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $755D STA */ poke(0x1c90 + s_x, s_a);
      /* $7560 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x7546 : 0x7562;
      branchTarget = true;
      break;
    case 0x7562: // [$7562..$7564]    3 bytes
      CYCLES(0x7562, 6);
      /* $7562 STA */ poke(0x1c70 + s_x, s_a);
    case 0x7565: // [$7565..$756F]   11 bytes
      CYCLES(0x7565, 19);
      /* $7565 LDA */ s_a = update_nz(peek(0x1c80 + s_x));
      /* $7568 CLC */ s_status &= ~STATUS_C;
      /* $7569 ADC */ tmp = peek(0x1ca0 + s_x), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $756C CMP */ update_nz_inv_c(s_a - 0x02);
      /* $756E BCC */ s_pc = !(s_status & STATUS_C) ? 0x7574 : 0x7570;
      branchTarget = true;
      break;
    case 0x7570: // [$7570..$7573]    4 bytes
      CYCLES(0x7570, 7);
      /* $7570 CMP */ update_nz_inv_c(s_a - 0xbe);
      /* $7572 BCC */ s_pc = !(s_status & STATUS_C) ? 0x7581 : 0x7574;
      branchTarget = true;
      break;
    case 0x7574: // [$7574..$7580]   13 bytes
      CYCLES(0x7574, 23);
      /* $7574 LDA */ s_a = update_nz(peek(0x1ca0 + s_x));
      /* $7577 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $7579 CLC */ s_status &= ~STATUS_C;
      /* $757A ADC */ tmp = 0x01, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $757C STA */ poke(0x1ca0 + s_x, s_a);
      /* $757F BNE */ s_pc = !(s_status & STATUS_Z) ? 0x7565 : 0x7581;
      branchTarget = true;
      break;
    case 0x7581: // [$7581..$7584]    4 bytes
      CYCLES(0x7581, 7);
      /* $7581 STA */ poke(0x1c80 + s_x, s_a);
      /* $7584 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x7585: // [$7585..$7594]   16 bytes
      CYCLES(0x7585, 28);
      /* $7585 LDA */ s_a = update_nz(peek(0x141d));
      /* $7588 ASL */ s_a = update_nzc(s_a << 1);
      /* $7589 TAX */ s_x = update_nz(s_a);
      /* $758A INC */ tmp16 = 0x1c, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $758C INC */ tmp16 = 0x1d, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $758E LDA */ s_a = update_nz(peek_zpg(0x1c));
      /* $7590 SEC */ s_status |= STATUS_C;
      /* $7591 SBC */ tmp = 0x04, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $7593 BCS */ s_pc = s_status & STATUS_C ? 0x7597 : 0x7595;
      branchTarget = true;
      break;
    case 0x7595: // [$7595..$7596]    2 bytes
      CYCLES(0x7595, 4);
      /* $7595 LDA */ s_a = update_nz(0x00);
    case 0x7597: // [$7597..$7597]    1 bytes
      CYCLES(0x7597, 2);
      /* $7597 TAY */ s_y = update_nz(s_a);
    case 0x7598: // [$7598..$7599]    2 bytes
      CYCLES(0x7598, 4);
      /* $7598 LDA */ s_a = update_nz(peek_zpg(0x1c));
    case 0x759a: // [$759A..$759D]    4 bytes
      CYCLES(0x759a, 7);
      /* $759A DEX */ s_x = update_nz(s_x - 1);
      /* $759B DEX */ s_x = update_nz(s_x - 1);
      /* $759C BPL */ s_pc = !(s_status & STATUS_N) ? 0x75a4 : 0x759e;
      branchTarget = true;
      break;
    case 0x759e: // [$759E..$75A3]    6 bytes
      CYCLES(0x759e, 11);
      /* $759E DEC */ tmp16 = 0x1c, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $75A0 DEC */ tmp16 = 0x1d, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $75A2 CLC */ s_status &= ~STATUS_C;
      /* $75A3 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x75a4: // [$75A4..$75A8]    5 bytes
      CYCLES(0x75a4, 9);
      /* $75A4 CMP */ update_nz_inv_c(s_a - peek(0x1c70 + s_x));
      /* $75A7 BCC */ s_pc = !(s_status & STATUS_C) ? 0x759a : 0x75a9;
      branchTarget = true;
      break;
    case 0x75a9: // [$75A9..$75AE]    6 bytes
      CYCLES(0x75a9, 11);
      /* $75A9 TYA */ s_a = update_nz(s_y);
      /* $75AA CMP */ update_nz_inv_c(s_a - peek(0x1c70 + s_x));
      /* $75AD BCS */ s_pc = s_status & STATUS_C ? 0x7598 : 0x75af;
      branchTarget = true;
      break;
    case 0x75af: // [$75AF..$75B5]    7 bytes
      CYCLES(0x75af, 12);
      /* $75AF LDA */ s_a = update_nz(peek_zpg(0x1d));
      /* $75B1 CMP */ update_nz_inv_c(s_a - peek(0x1c80 + s_x));
      /* $75B4 BCC */ s_pc = !(s_status & STATUS_C) ? 0x7598 : 0x75b6;
      branchTarget = true;
      break;
    case 0x75b6: // [$75B6..$75B9]    4 bytes
      CYCLES(0x75b6, 7);
      /* $75B6 SBC */ tmp = 0x04, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $75B8 BCC */ s_pc = !(s_status & STATUS_C) ? 0x75bf : 0x75ba;
      branchTarget = true;
      break;
    case 0x75ba: // [$75BA..$75BE]    5 bytes
      CYCLES(0x75ba, 9);
      /* $75BA CMP */ update_nz_inv_c(s_a - peek(0x1c80 + s_x));
      /* $75BD BCS */ s_pc = s_status & STATUS_C ? 0x7598 : 0x75bf;
      branchTarget = true;
      break;
    case 0x75bf: // [$75BF..$75C2]    4 bytes
      CYCLES(0x75bf, 7);
      /* $75BF LDA */ s_a = update_nz(peek_zpg(0xe8));
      /* $75C1 BEQ */ s_pc = s_status & STATUS_Z ? 0x75c5 : 0x75c3;
      branchTarget = true;
      break;
    case 0x75c3: // [$75C3..$75C4]    2 bytes
      CYCLES(0x75c3, 4);
      /* $75C3 SEC */ s_status |= STATUS_C;
      /* $75C4 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x75c5: // [$75C5..$75D1]   13 bytes
      CYCLES(0x75c5, 23);
      /* $75C5 STX */ poke_zpg(0x00, s_x);
      /* $75C7 LDA */ s_a = update_nz(0x00);
      /* $75C9 STA */ poke_zpg(0xfc, s_a);
      /* $75CB STA */ poke_zpg(0xfe, s_a);
      /* $75CD LDA */ s_a = update_nz(peek(0x0c2e));
      /* $75D0 STA */ poke_zpg(0x02, s_a);
    case 0x75d2: // [$75D2..$75D4]    3 bytes
      CYCLES(0x75d2, 6);
      /* $75D2 JSR */ push16(0x75d4), s_pc = 0x7628;
      branchTarget = true;
      break;
    case 0x75d5: // [$75D5..$75D8]    4 bytes
      CYCLES(0x75d5, 7);
      /* $75D5 DEC */ tmp16 = 0x02, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $75D7 BPL */ s_pc = !(s_status & STATUS_N) ? 0x75d2 : 0x75d9;
      branchTarget = true;
      break;
    case 0x75d9: // [$75D9..$75DD]    5 bytes
      CYCLES(0x75d9, 9);
      /* $75D9 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $75DB JSR */ push16(0x75dd), s_pc = 0x75eb;
      branchTarget = true;
      break;
    case 0x75de: // [$75DE..$75E4]    7 bytes
      CYCLES(0x75de, 12);
      /* $75DE LDA */ s_a = update_nz(0x00);
      /* $75E0 LDX */ s_x = update_nz(0x64);
      /* $75E2 JSR */ push16(0x75e4), s_pc = 0x4dd0;
      branchTarget = true;
      break;
    case 0x75e5: // [$75E5..$75EA]    6 bytes
      CYCLES(0x75e5, 11);
      /* $75E5 DEC */ tmp16 = 0x1c, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $75E7 DEC */ tmp16 = 0x1d, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $75E9 SEC */ s_status |= STATUS_C;
      /* $75EA RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x75eb: // [$75EB..$75F5]   11 bytes
      CYCLES(0x75eb, 19);
      /* $75EB TXA */ s_a = update_nz(s_x);
      /* $75EC PHA */ push8(s_a);
      /* $75ED DEC */ tmp16 = 0x141d, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $75F0 LDA */ s_a = update_nz(peek(0x141d));
      /* $75F3 ASL */ s_a = update_nzc(s_a << 1);
      /* $75F4 STA */ poke_zpg(0x06, s_a);
    case 0x75f6: // [$75F6..$7624]   47 bytes
      CYCLES(0x75f6, 80);
      /* $75F6 LDA */ s_a = update_nz(peek(0x1c72 + s_x));
      /* $75F9 STA */ poke(0x1c70 + s_x, s_a);
      /* $75FC LDA */ s_a = update_nz(peek(0x1c82 + s_x));
      /* $75FF STA */ poke(0x1c80 + s_x, s_a);
      /* $7602 LDA */ s_a = update_nz(peek(0x1c92 + s_x));
      /* $7605 STA */ poke(0x1c90 + s_x, s_a);
      /* $7608 LDA */ s_a = update_nz(peek(0x1ca2 + s_x));
      /* $760B STA */ poke(0x1ca0 + s_x, s_a);
      /* $760E LDA */ s_a = update_nz(peek(0x1cb2 + s_x));
      /* $7611 STA */ poke(0x1cb0 + s_x, s_a);
      /* $7614 LDA */ s_a = update_nz(peek(0x1cc2 + s_x));
      /* $7617 STA */ poke(0x1cc0 + s_x, s_a);
      /* $761A LDA */ s_a = update_nz(peek(0x1cd2 + s_x));
      /* $761D STA */ poke(0x1cd0 + s_x, s_a);
      /* $7620 INX */ s_x = update_nz(s_x + 1);
      /* $7621 CPX */ update_nz_inv_c(s_x - peek_zpg(0x06));
      /* $7623 BCC */ s_pc = !(s_status & STATUS_C) ? 0x75f6 : 0x7625;
      branchTarget = true;
      break;
    case 0x7625: // [$7625..$7627]    3 bytes
      CYCLES(0x7625, 6);
      /* $7625 PLA */ s_a = update_nz(pop8());
      /* $7626 TAX */ s_x = update_nz(s_a);
      /* $7627 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x7628: // [$7628..$762F]    8 bytes
      CYCLES(0x7628, 14);
      /* $7628 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $762A INX */ s_x = update_nz(s_x + 1);
      /* $762B LDA */ s_a = update_nz(peek(0x1cc0 + s_x));
      /* $762E BEQ */ s_pc = s_status & STATUS_Z ? 0x7635 : 0x7630;
      branchTarget = true;
      break;
    case 0x7630: // [$7630..$7634]    5 bytes
      CYCLES(0x7630, 9);
      /* $7630 DEC */ tmp16 = 0x1cc0 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $7633 BPL */ s_pc = !(s_status & STATUS_N) ? 0x768e : 0x7635;
      branchTarget = true;
      break;
    case 0x7635: // [$7635..$763D]    9 bytes
      CYCLES(0x7635, 16);
      /* $7635 LDY */ s_y = update_nz(peek(0x1c80 + s_x));
      /* $7638 LDA */ s_a = update_nz(peek(0x1c70 + s_x));
      /* $763B JSR */ push16(0x763d), s_pc = 0x4c1c;
      branchTarget = true;
      break;
    case 0x763e: // [$763E..$7642]    5 bytes
      CYCLES(0x763e, 9);
      /* $763E STA */ poke_zpg(0x04, s_a);
      /* $7640 LDA */ s_a = update_nz(0xff);
      /* $7642 CLC */ s_status &= ~STATUS_C;
    case 0x7643: // [$7643..$7646]    4 bytes
      CYCLES(0x7643, 7);
      /* $7643 ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $7644 DEX */ s_x = update_nz(s_x - 1);
      /* $7645 BPL */ s_pc = !(s_status & STATUS_N) ? 0x7643 : 0x7647;
      branchTarget = true;
      break;
    case 0x7647: // [$7647..$764A]    4 bytes
      CYCLES(0x7647, 7);
      /* $7647 STA */ poke_zpg(0x0a, s_a);
      /* $7649 LDX */ s_x = update_nz(0x02);
    case 0x764b: // [$764B..$7662]   24 bytes
      CYCLES(0x764b, 41);
      /* $764B LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $764D STA */ poke_zpg(0x06, s_a);
      /* $764F LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $7651 STA */ poke_zpg(0x07, s_a);
      /* $7653 STY */ poke_zpg(0x05, s_y);
      /* $7655 LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $7657 LDA */ s_a = update_nz(peek_zpg(0x0a));
      /* $7659 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $765B STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $765D LDA */ s_a = update_nz(peek_zpg(0x0a));
      /* $765F SEC */ s_status |= STATUS_C;
      /* $7660 ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $7661 BMI */ s_pc = s_status & STATUS_N ? 0x7666 : 0x7663;
      branchTarget = true;
      break;
    case 0x7663: // [$7663..$7665]    3 bytes
      CYCLES(0x7663, 6);
      /* $7663 INY */ s_y = update_nz(s_y + 1);
      /* $7664 LDA */ s_a = update_nz(0xfe);
    case 0x7666: // [$7666..$766F]   10 bytes
      CYCLES(0x7666, 18);
      /* $7666 AND */ s_a = update_nz(s_a & peek(peek16_zpg(0x06) + s_y));
      /* $7668 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $766A LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $766C INY */ s_y = update_nz(s_y + 1);
      /* $766D DEX */ s_x = update_nz(s_x - 1);
      /* $766E BNE */ s_pc = !(s_status & STATUS_Z) ? 0x764b : 0x7670;
      branchTarget = true;
      break;
    case 0x7670: // [$7670..$7675]    6 bytes
      CYCLES(0x7670, 11);
      /* $7670 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $7672 INX */ s_x = update_nz(s_x + 1);
      /* $7673 JSR */ push16(0x7675), s_pc = 0x7546;
      branchTarget = true;
      break;
    case 0x7676: // [$7676..$767A]    5 bytes
      CYCLES(0x7676, 9);
      /* $7676 DEC */ tmp16 = 0x1cb0 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $7679 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x768e : 0x767b;
      branchTarget = true;
      break;
    case 0x767b: // [$767B..$768D]   19 bytes
      CYCLES(0x767b, 33);
      /* $767B LDA */ s_a = update_nz(0x40);
      /* $767D STA */ poke(0x1cb0 + s_x, s_a);
      /* $7680 LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $7682 LDA */ s_a = update_nz(peek(0x1cc0 + s_y));
      /* $7685 STA */ poke(0x1c90 + s_x, s_a);
      /* $7688 LDA */ s_a = update_nz(peek(0x1cd0 + s_y));
      /* $768B STA */ poke(0x1ca0 + s_x, s_a);
    case 0x768e: // [$768E..$768E]    1 bytes
      CYCLES(0x768e, 2);
      /* $768E RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x768f: // [$768F..$7695]    7 bytes
      CYCLES(0x768f, 12);
      /* $768F LDX */ s_x = update_nz(0x00);
      /* $7691 LDA */ s_a = update_nz(peek_zpg(0x4e));
      /* $7693 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $7694 BCC */ s_pc = !(s_status & STATUS_C) ? 0x769c : 0x7696;
      branchTarget = true;
      break;
    case 0x7696: // [$7696..$7699]    4 bytes
      CYCLES(0x7696, 7);
      /* $7696 INX */ s_x = update_nz(s_x + 1);
      /* $7697 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $7698 BCC */ s_pc = !(s_status & STATUS_C) ? 0x769c : 0x769a;
      branchTarget = true;
      break;
    case 0x769a: // [$769A..$769B]    2 bytes
      CYCLES(0x769a, 4);
      /* $769A DEX */ s_x = update_nz(s_x - 1);
      /* $769B DEX */ s_x = update_nz(s_x - 1);
    case 0x769c: // [$769C..$76A4]    9 bytes
      CYCLES(0x769c, 16);
      /* $769C LDA */ s_a = update_nz(peek_zpg(0x4f));
      /* $769E AND */ s_a = update_nz(s_a & 0x03);
      /* $76A0 SEC */ s_status |= STATUS_C;
      /* $76A1 SBC */ tmp = 0x02, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $76A3 BCC */ s_pc = !(s_status & STATUS_C) ? 0x76a7 : 0x76a5;
      branchTarget = true;
      break;
    case 0x76a5: // [$76A5..$76A6]    2 bytes
      CYCLES(0x76a5, 4);
      /* $76A5 ADC */ tmp = 0x00, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0x76a7: // [$76A7..$76AA]    4 bytes
      CYCLES(0x76a7, 7);
      /* $76A7 BIT */ tmp = peek_zpg(0x4f), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $76A9 BPL */ s_pc = !(s_status & STATUS_N) ? 0x76b0 : 0x76ab;
      branchTarget = true;
      break;
    case 0x76ab: // [$76AB..$76AF]    5 bytes
      CYCLES(0x76ab, 9);
      /* $76AB STX */ poke_zpg(0x02, s_x);
      /* $76AD TAX */ s_x = update_nz(s_a);
      /* $76AE LDA */ s_a = update_nz(peek_zpg(0x02));
    case 0x76b0: // [$76B0..$76B6]    7 bytes
      CYCLES(0x76b0, 12);
      /* $76B0 STX */ poke_zpg(0x02, s_x);
      /* $76B2 STA */ poke_zpg(0x03, s_a);
      /* $76B4 JSR */ push16(0x76b6), s_pc = 0x4c36;
      branchTarget = true;
      break;
    case 0x76b7: // [$76B7..$76BB]    5 bytes
      CYCLES(0x76b7, 9);
      /* $76B7 CMP */ update_nz_inv_c(s_a - peek(0x0c2f));
      /* $76BA BCS */ s_pc = s_status & STATUS_C ? 0x76f0 : 0x76bc;
      branchTarget = true;
      break;
    case 0x76bc: // [$76BC..$76C5]   10 bytes
      CYCLES(0x76bc, 18);
      /* $76BC LDA */ s_a = update_nz(peek(0x1c70 + s_y));
      /* $76BF CMP */ update_nz_inv_c(s_a - peek(0x1500));
      /* $76C2 LDA */ s_a = update_nz(peek_zpg(0x02));
      /* $76C4 BCS */ s_pc = s_status & STATUS_C ? 0x76ce : 0x76c6;
      branchTarget = true;
      break;
    case 0x76c6: // [$76C6..$76C7]    2 bytes
      CYCLES(0x76c6, 4);
      /* $76C6 BPL */ s_pc = !(s_status & STATUS_N) ? 0x76d4 : 0x76c8;
      branchTarget = true;
      break;
    case 0x76c8: // [$76C8..$76CD]    6 bytes
      CYCLES(0x76c8, 11);
      /* $76C8 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $76CA ADC */ tmp = 0x01, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $76CC BPL */ s_pc = !(s_status & STATUS_N) ? 0x76d4 : 0x76ce;
      branchTarget = true;
      break;
    case 0x76ce: // [$76CE..$76CF]    2 bytes
      CYCLES(0x76ce, 4);
      /* $76CE BMI */ s_pc = s_status & STATUS_N ? 0x76d4 : 0x76d0;
      branchTarget = true;
      break;
    case 0x76d0: // [$76D0..$76D3]    4 bytes
      CYCLES(0x76d0, 7);
      /* $76D0 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $76D2 ADC */ tmp = 0x00, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0x76d4: // [$76D4..$76DF]   12 bytes
      CYCLES(0x76d4, 21);
      /* $76D4 STA */ poke_zpg(0x02, s_a);
      /* $76D6 LDA */ s_a = update_nz(peek(0x1c80 + s_y));
      /* $76D9 CMP */ update_nz_inv_c(s_a - peek(0x1501));
      /* $76DC LDA */ s_a = update_nz(peek_zpg(0x03));
      /* $76DE BCS */ s_pc = s_status & STATUS_C ? 0x76e8 : 0x76e0;
      branchTarget = true;
      break;
    case 0x76e0: // [$76E0..$76E1]    2 bytes
      CYCLES(0x76e0, 4);
      /* $76E0 BPL */ s_pc = !(s_status & STATUS_N) ? 0x76ee : 0x76e2;
      branchTarget = true;
      break;
    case 0x76e2: // [$76E2..$76E7]    6 bytes
      CYCLES(0x76e2, 11);
      /* $76E2 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $76E4 ADC */ tmp = 0x01, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $76E6 BPL */ s_pc = !(s_status & STATUS_N) ? 0x76ee : 0x76e8;
      branchTarget = true;
      break;
    case 0x76e8: // [$76E8..$76E9]    2 bytes
      CYCLES(0x76e8, 4);
      /* $76E8 BMI */ s_pc = s_status & STATUS_N ? 0x76ee : 0x76ea;
      branchTarget = true;
      break;
    case 0x76ea: // [$76EA..$76ED]    4 bytes
      CYCLES(0x76ea, 7);
      /* $76EA EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $76EC ADC */ tmp = 0x00, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0x76ee: // [$76EE..$76EF]    2 bytes
      CYCLES(0x76ee, 4);
      /* $76EE STA */ poke_zpg(0x03, s_a);
    case 0x76f0: // [$76F0..$76F0]    1 bytes
      CYCLES(0x76f0, 2);
      /* $76F0 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x76f1: // [$76F1..$76F5]    5 bytes
      CYCLES(0x76f1, 9);
      /* $76F1 LDA */ s_a = update_nz(peek(0x141b));
      /* $76F4 ASL */ s_a = update_nzc(s_a << 1);
      /* $76F5 TAX */ s_x = update_nz(s_a);
    case 0x76f6: // [$76F6..$76F9]    4 bytes
      CYCLES(0x76f6, 7);
      /* $76F6 DEX */ s_x = update_nz(s_x - 1);
      /* $76F7 DEX */ s_x = update_nz(s_x - 1);
      /* $76F8 BPL */ s_pc = !(s_status & STATUS_N) ? 0x76fd : 0x76fa;
      branchTarget = true;
      break;
    case 0x76fa: // [$76FA..$76FC]    3 bytes
      CYCLES(0x76fa, 6);
      /* $76FA JMP */ s_pc = 0x77dd;
      branchTarget = true;
      break;
    case 0x76fd: // [$76FD..$7701]    5 bytes
      CYCLES(0x76fd, 9);
      /* $76FD LDA */ s_a = update_nz(peek(0x1c51 + s_x));
      /* $7700 BMI */ s_pc = s_status & STATUS_N ? 0x770b : 0x7702;
      branchTarget = true;
      break;
    case 0x7702: // [$7702..$7706]    5 bytes
      CYCLES(0x7702, 9);
      /* $7702 STX */ poke_zpg(0x00, s_x);
      /* $7704 JSR */ push16(0x7706), s_pc = 0x7821;
      branchTarget = true;
      break;
    case 0x7707: // [$7707..$770A]    4 bytes
      CYCLES(0x7707, 7);
      /* $7707 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $7709 BPL */ s_pc = !(s_status & STATUS_N) ? 0x76f6 : 0x770b;
      branchTarget = true;
      break;
    case 0x770b: // [$770B..$770F]    5 bytes
      CYCLES(0x770b, 9);
      /* $770B LDA */ s_a = update_nz(peek(0x1508));
      /* $770E BNE */ s_pc = !(s_status & STATUS_Z) ? 0x76f6 : 0x7710;
      branchTarget = true;
      break;
    case 0x7710: // [$7710..$7716]    7 bytes
      CYCLES(0x7710, 12);
      /* $7710 STX */ poke_zpg(0x00, s_x);
      /* $7712 LDA */ s_a = update_nz(peek(0x1c31 + s_x));
      /* $7715 BMI */ s_pc = s_status & STATUS_N ? 0x771d : 0x7717;
      branchTarget = true;
      break;
    case 0x7717: // [$7717..$771C]    6 bytes
      CYCLES(0x7717, 11);
      /* $7717 DEC */ tmp16 = 0x1c31 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $771A JMP */ s_pc = 0x7720;
      branchTarget = true;
      break;
    case 0x771d: // [$771D..$771F]    3 bytes
      CYCLES(0x771d, 6);
      /* $771D JSR */ push16(0x771f), s_pc = 0x7778;
      branchTarget = true;
      break;
    case 0x7720: // [$7720..$7724]    5 bytes
      CYCLES(0x7720, 9);
      /* $7720 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $7722 JSR */ push16(0x7724), s_pc = 0x779e;
      branchTarget = true;
      break;
    case 0x7725: // [$7725..$7741]   29 bytes
      CYCLES(0x7725, 50);
      /* $7725 LDA */ s_a = update_nz(peek(0x1c50 + s_x));
      /* $7728 ASL */ s_a = update_nzc(s_a << 1);
      /* $7729 ASL */ s_a = update_nzc(s_a << 1);
      /* $772A ORA */ s_a = update_nz(s_a | peek(0x1bf0 + s_x));
      /* $772D TAY */ s_y = update_nz(s_a);
      /* $772E LDA */ s_a = update_nz(peek(0x08f0 + s_y));
      /* $7731 STA */ poke_zpg(0x06, s_a);
      /* $7733 LDA */ s_a = update_nz(peek(0x0900 + s_y));
      /* $7736 STA */ poke_zpg(0x07, s_a);
      /* $7738 LDY */ s_y = update_nz(peek(0x1bd0 + s_x));
      /* $773B LDA */ s_a = update_nz(peek(0x1bb0 + s_x));
      /* $773E TAX */ s_x = update_nz(s_a);
      /* $773F JSR */ push16(0x7741), s_pc = 0x5059;
      branchTarget = true;
      break;
    case 0x7742: // [$7742..$7748]    7 bytes
      CYCLES(0x7742, 12);
      /* $7742 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $7744 DEC */ tmp16 = 0x1c10 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $7747 BPL */ s_pc = !(s_status & STATUS_N) ? 0x76f6 : 0x7749;
      branchTarget = true;
      break;
    case 0x7749: // [$7749..$7756]   14 bytes
      CYCLES(0x7749, 24);
      /* $7749 LDA */ s_a = update_nz(0x70);
      /* $774B STA */ poke(0x1c10 + s_x, s_a);
      /* $774E LDA */ s_a = update_nz(peek(0x1bb0 + s_x));
      /* $7751 LDY */ s_y = update_nz(peek(0x1500));
      /* $7754 JSR */ push16(0x7756), s_pc = 0x7812;
      branchTarget = true;
      break;
    case 0x7757: // [$7757..$7763]   13 bytes
      CYCLES(0x7757, 23);
      /* $7757 STA */ poke_zpg(0x02, s_a);
      /* $7759 STY */ poke_zpg(0x03, s_y);
      /* $775B LDA */ s_a = update_nz(peek(0x1bd0 + s_x));
      /* $775E LDY */ s_y = update_nz(peek(0x1501));
      /* $7761 JSR */ push16(0x7763), s_pc = 0x7812;
      branchTarget = true;
      break;
    case 0x7764: // [$7764..$7767]    4 bytes
      CYCLES(0x7764, 7);
      /* $7764 CMP */ update_nz_inv_c(s_a - peek_zpg(0x02));
      /* $7766 BCS */ s_pc = s_status & STATUS_C ? 0x776c : 0x7768;
      branchTarget = true;
      break;
    case 0x7768: // [$7768..$776B]    4 bytes
      CYCLES(0x7768, 7);
      /* $7768 LDA */ s_a = update_nz(peek_zpg(0x03));
      /* $776A BPL */ s_pc = !(s_status & STATUS_N) ? 0x776f : 0x776c;
      branchTarget = true;
      break;
    case 0x776c: // [$776C..$776E]    3 bytes
      CYCLES(0x776c, 6);
      /* $776C TYA */ s_a = update_nz(s_y);
      /* $776D ORA */ s_a = update_nz(s_a | 0x02);
    case 0x776f: // [$776F..$7777]    9 bytes
      CYCLES(0x776f, 16);
      /* $776F STA */ poke(0x1bf0 + s_x, s_a);
      /* $7772 STA */ poke(0x1c30 + s_x, s_a);
      /* $7775 JMP */ s_pc = 0x76f6;
      branchTarget = true;
      break;
    case 0x7778: // [$7778..$777D]    6 bytes
      CYCLES(0x7778, 11);
      /* $7778 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $777A INX */ s_x = update_nz(s_x + 1);
      /* $777B JSR */ push16(0x777d), s_pc = 0x779e;
      branchTarget = true;
      break;
    case 0x777e: // [$777E..$778A]   13 bytes
      CYCLES(0x777e, 23);
      /* $777E TXA */ s_a = update_nz(s_x);
      /* $777F TAY */ s_y = update_nz(s_a);
      /* $7780 LDX */ s_x = update_nz(peek(0x1bd0 + s_y));
      /* $7783 LDA */ s_a = update_nz(peek(0x1bb0 + s_y));
      /* $7786 LDY */ s_y = update_nz(0x0a);
      /* $7788 JSR */ push16(0x778a), s_pc = 0x4cd0;
      branchTarget = true;
      break;
    case 0x778b: // [$778B..$7791]    7 bytes
      CYCLES(0x778b, 12);
      /* $778B LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $778D DEC */ tmp16 = 0x1c11 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $7790 BPL */ s_pc = !(s_status & STATUS_N) ? 0x779d : 0x7792;
      branchTarget = true;
      break;
    case 0x7792: // [$7792..$779C]   11 bytes
      CYCLES(0x7792, 19);
      /* $7792 LDA */ s_a = update_nz(0x70);
      /* $7794 STA */ poke(0x1c11 + s_x, s_a);
      /* $7797 LDA */ s_a = update_nz(peek(0x1c30 + s_x));
      /* $779A STA */ poke(0x1bf1 + s_x, s_a);
    case 0x779d: // [$779D..$779D]    1 bytes
      CYCLES(0x779d, 2);
      /* $779D RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x779e: // [$779E..$77AB]   14 bytes
      CYCLES(0x779e, 24);
      /* $779E LDY */ s_y = update_nz(peek(0x1bf0 + s_x));
      /* $77A1 LDA */ s_a = update_nz(peek(0x1bb0 + s_x));
      /* $77A4 CLC */ s_status &= ~STATUS_C;
      /* $77A5 ADC */ tmp = peek(0x0d3e + s_y), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $77A8 CMP */ update_nz_inv_c(s_a - 0x06);
      /* $77AA BCS */ s_pc = s_status & STATUS_C ? 0x77b0 : 0x77ac;
      branchTarget = true;
      break;
    case 0x77ac: // [$77AC..$77AF]    4 bytes
      CYCLES(0x77ac, 7);
      /* $77AC LDA */ s_a = update_nz(0x00);
      /* $77AE BPL */ s_pc = !(s_status & STATUS_N) ? 0x77b6 : 0x77b0;
      branchTarget = true;
      break;
    case 0x77b0: // [$77B0..$77B3]    4 bytes
      CYCLES(0x77b0, 7);
      /* $77B0 CMP */ update_nz_inv_c(s_a - 0xf3);
      /* $77B2 BCC */ s_pc = !(s_status & STATUS_C) ? 0x77bb : 0x77b4;
      branchTarget = true;
      break;
    case 0x77b4: // [$77B4..$77B5]    2 bytes
      CYCLES(0x77b4, 4);
      /* $77B4 LDA */ s_a = update_nz(0x01);
    case 0x77b6: // [$77B6..$77BA]    5 bytes
      CYCLES(0x77b6, 9);
      /* $77B6 STA */ poke(0x1bf0 + s_x, s_a);
      /* $77B9 BPL */ s_pc = !(s_status & STATUS_N) ? 0x779e : 0x77bb;
      branchTarget = true;
      break;
    case 0x77bb: // [$77BB..$77BD]    3 bytes
      CYCLES(0x77bb, 6);
      /* $77BB STA */ poke(0x1bb0 + s_x, s_a);
    case 0x77be: // [$77BE..$77C8]   11 bytes
      CYCLES(0x77be, 19);
      /* $77BE LDA */ s_a = update_nz(peek(0x1bd0 + s_x));
      /* $77C1 CLC */ s_status &= ~STATUS_C;
      /* $77C2 ADC */ tmp = peek(0x0d42 + s_y), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $77C5 CMP */ update_nz_inv_c(s_a - 0x06);
      /* $77C7 BCS */ s_pc = s_status & STATUS_C ? 0x77cd : 0x77c9;
      branchTarget = true;
      break;
    case 0x77c9: // [$77C9..$77CC]    4 bytes
      CYCLES(0x77c9, 7);
      /* $77C9 LDA */ s_a = update_nz(0x02);
      /* $77CB BPL */ s_pc = !(s_status & STATUS_N) ? 0x77d3 : 0x77cd;
      branchTarget = true;
      break;
    case 0x77cd: // [$77CD..$77D0]    4 bytes
      CYCLES(0x77cd, 7);
      /* $77CD CMP */ update_nz_inv_c(s_a - 0xb0);
      /* $77CF BCC */ s_pc = !(s_status & STATUS_C) ? 0x77d9 : 0x77d1;
      branchTarget = true;
      break;
    case 0x77d1: // [$77D1..$77D2]    2 bytes
      CYCLES(0x77d1, 4);
      /* $77D1 LDA */ s_a = update_nz(0x03);
    case 0x77d3: // [$77D3..$77D8]    6 bytes
      CYCLES(0x77d3, 11);
      /* $77D3 STA */ poke(0x1bf0 + s_x, s_a);
      /* $77D6 TAY */ s_y = update_nz(s_a);
      /* $77D7 BPL */ s_pc = !(s_status & STATUS_N) ? 0x77be : 0x77d9;
      branchTarget = true;
      break;
    case 0x77d9: // [$77D9..$77DC]    4 bytes
      CYCLES(0x77d9, 7);
      /* $77D9 STA */ poke(0x1bd0 + s_x, s_a);
      /* $77DC RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x77dd: // [$77DD..$77E1]    5 bytes
      CYCLES(0x77dd, 9);
      /* $77DD DEC */ tmp16 = 0x1426, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $77E0 BPL */ s_pc = !(s_status & STATUS_N) ? 0x7811 : 0x77e2;
      branchTarget = true;
      break;
    case 0x77e2: // [$77E2..$77E7]    6 bytes
      CYCLES(0x77e2, 11);
      /* $77E2 LDA */ s_a = update_nz(peek(0x0c2b));
      /* $77E5 JSR */ push16(0x77e7), s_pc = 0x4c4b;
      branchTarget = true;
      break;
    case 0x77e8: // [$77E8..$77FA]   19 bytes
      CYCLES(0x77e8, 33);
      /* $77E8 STA */ poke_zpg(0x06, s_a);
      /* $77EA LDA */ s_a = update_nz(peek(0x0c2a));
      /* $77ED CLC */ s_status &= ~STATUS_C;
      /* $77EE ADC */ tmp = 0x01, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $77F0 ASL */ s_a = update_nzc(s_a << 1);
      /* $77F1 ADC */ tmp = peek_zpg(0x06), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $77F3 STA */ poke(0x1426, s_a);
      /* $77F6 LDA */ s_a = update_nz(peek(0x141b));
      /* $77F9 ASL */ s_a = update_nzc(s_a << 1);
      /* $77FA TAX */ s_x = update_nz(s_a);
    case 0x77fb: // [$77FB..$77FE]    4 bytes
      CYCLES(0x77fb, 7);
      /* $77FB DEX */ s_x = update_nz(s_x - 1);
      /* $77FC DEX */ s_x = update_nz(s_x - 1);
      /* $77FD BMI */ s_pc = s_status & STATUS_N ? 0x7811 : 0x77ff;
      branchTarget = true;
      break;
    case 0x77ff: // [$77FF..$7803]    5 bytes
      CYCLES(0x77ff, 9);
      /* $77FF LDA */ s_a = update_nz(peek(0x1c31 + s_x));
      /* $7802 BPL */ s_pc = !(s_status & STATUS_N) ? 0x77fb : 0x7804;
      branchTarget = true;
      break;
    case 0x7804: // [$7804..$7810]   13 bytes
      CYCLES(0x7804, 23);
      /* $7804 LDA */ s_a = update_nz(0x00);
      /* $7806 STA */ poke(0x1c10 + s_x, s_a);
      /* $7809 LDA */ s_a = update_nz(peek(0x0c2a));
      /* $780C STA */ poke(0x1c11 + s_x, s_a);
      /* $780F BPL */ s_pc = !(s_status & STATUS_N) ? 0x77fb : 0x7811;
      branchTarget = true;
      break;
    case 0x7811: // [$7811..$7811]    1 bytes
      CYCLES(0x7811, 2);
      /* $7811 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x7812: // [$7812..$781A]    9 bytes
      CYCLES(0x7812, 16);
      /* $7812 STY */ poke_zpg(0x01, s_y);
      /* $7814 LDY */ s_y = update_nz(0x01);
      /* $7816 SEC */ s_status |= STATUS_C;
      /* $7817 SBC */ tmp = peek_zpg(0x01), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $7819 BCS */ s_pc = s_status & STATUS_C ? 0x7820 : 0x781b;
      branchTarget = true;
      break;
    case 0x781b: // [$781B..$781F]    5 bytes
      CYCLES(0x781b, 9);
      /* $781B EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $781D ADC */ tmp = 0x01, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $781F DEY */ s_y = update_nz(s_y - 1);
    case 0x7820: // [$7820..$7820]    1 bytes
      CYCLES(0x7820, 2);
      /* $7820 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x7821: // [$7821..$782D]   13 bytes
      CYCLES(0x7821, 23);
      /* $7821 LDY */ s_y = update_nz(peek(0x1508));
      /* $7824 LDA */ s_a = update_nz(peek(0x0c41 + s_y));
      /* $7827 STA */ poke_zpg(0x02, s_a);
      /* $7829 DEC */ tmp16 = 0x1c51 + s_x, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $782C BPL */ s_pc = !(s_status & STATUS_N) ? 0x783e : 0x782e;
      branchTarget = true;
      break;
    case 0x782e: // [$782E..$783D]   16 bytes
      CYCLES(0x782e, 28);
      /* $782E LDA */ s_a = update_nz(peek(0x1bd0 + s_x));
      /* $7831 SEC */ s_status |= STATUS_C;
      /* $7832 SBC */ tmp = 0x02, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $7834 LDY */ s_y = update_nz(peek(0x1bb0 + s_x));
      /* $7837 TAX */ s_x = update_nz(s_a);
      /* $7838 TYA */ s_a = update_nz(s_y);
      /* $7839 LDY */ s_y = update_nz(0x0e);
      /* $783B JMP */ s_pc = 0x4cd0;
      branchTarget = true;
      break;
    case 0x783e: // [$783E..$785A]   29 bytes
      CYCLES(0x783e, 50);
      /* $783E LDA */ s_a = update_nz(peek(0x1c50 + s_x));
      /* $7841 ASL */ s_a = update_nzc(s_a << 1);
      /* $7842 ORA */ s_a = update_nz(s_a | peek(0x1bf0 + s_x));
      /* $7845 ASL */ s_a = update_nzc(s_a << 1);
      /* $7846 TAY */ s_y = update_nz(s_a);
      /* $7847 LDA */ s_a = update_nz(peek(0x08a0 + s_y));
      /* $784A STA */ poke_zpg(0x06, s_a);
      /* $784C LDA */ s_a = update_nz(peek(0x08b0 + s_y));
      /* $784F STA */ poke_zpg(0x07, s_a);
      /* $7851 LDA */ s_a = update_nz(peek(0x1bd0 + s_x));
      /* $7854 SEC */ s_status |= STATUS_C;
      /* $7855 SBC */ tmp = 0x02, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $7857 LDY */ s_y = update_nz(peek_zpg(0x4e));
      /* $7859 BPL */ s_pc = !(s_status & STATUS_N) ? 0x785d : 0x785b;
      branchTarget = true;
      break;
    case 0x785b: // [$785B..$785C]    2 bytes
      CYCLES(0x785b, 4);
      /* $785B SBC */ tmp = 0xfc, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
    case 0x785d: // [$785D..$7864]    8 bytes
      CYCLES(0x785d, 14);
      /* $785D TAY */ s_y = update_nz(s_a);
      /* $785E LDA */ s_a = update_nz(peek(0x1bb0 + s_x));
      /* $7861 TAX */ s_x = update_nz(s_a);
      /* $7862 JSR */ push16(0x7864), s_pc = 0x4c7c;
      branchTarget = true;
      break;
    case 0x7865: // [$7865..$7886]   34 bytes
      CYCLES(0x7865, 58);
      /* $7865 LDA */ s_a = update_nz(peek(peek16_zpg(0xfc) + s_y));
      /* $7867 STA */ poke_zpg(0x06, s_a);
      /* $7869 LDA */ s_a = update_nz(peek(peek16_zpg(0xfe) + s_y));
      /* $786B STA */ poke_zpg(0x07, s_a);
      /* $786D STY */ poke_zpg(0x05, s_y);
      /* $786F LDY */ s_y = update_nz(peek_zpg(0x04));
      /* $7871 LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $7873 EOR */ s_a = update_nz(s_a ^ peek_zpg(0x02));
      /* $7875 STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $7877 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $7879 INY */ s_y = update_nz(s_y + 1);
      /* $787A LDA */ s_a = update_nz(peek(peek16_zpg(0x08 + s_x)));
      /* $787C EOR */ s_a = update_nz(s_a ^ peek_zpg(0x02));
      /* $787E STA */ poke(peek16_zpg(0x06) + s_y, s_a);
      /* $7880 INC */ tmp16 = 0x08, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $7882 LDY */ s_y = update_nz(peek_zpg(0x05));
      /* $7884 DEY */ s_y = update_nz(s_y - 1);
      /* $7885 BPL */ s_pc = !(s_status & STATUS_N) ? 0x7865 : 0x7887;
      branchTarget = true;
      break;
    case 0x7887: // [$7887..$7889]    3 bytes
      CYCLES(0x7887, 6);
      /* $7887 JMP */ s_pc = 0x4c6e;
      branchTarget = true;
      break;
    case 0x788a: // [$788A..$7895]   12 bytes
      CYCLES(0x788a, 21);
      /* $788A LDA */ s_a = update_nz(peek(0x141b));
      /* $788D ASL */ s_a = update_nzc(s_a << 1);
      /* $788E TAX */ s_x = update_nz(s_a);
      /* $788F LDA */ s_a = update_nz(peek_zpg(0x1c));
      /* $7891 SEC */ s_status |= STATUS_C;
      /* $7892 SBC */ tmp = 0x07, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $7894 BCS */ s_pc = s_status & STATUS_C ? 0x7898 : 0x7896;
      branchTarget = true;
      break;
    case 0x7896: // [$7896..$7897]    2 bytes
      CYCLES(0x7896, 4);
      /* $7896 LDA */ s_a = update_nz(0x00);
    case 0x7898: // [$7898..$7898]    1 bytes
      CYCLES(0x7898, 2);
      /* $7898 TAY */ s_y = update_nz(s_a);
    case 0x7899: // [$7899..$789A]    2 bytes
      CYCLES(0x7899, 4);
      /* $7899 LDA */ s_a = update_nz(peek_zpg(0x1c));
    case 0x789b: // [$789B..$789E]    4 bytes
      CYCLES(0x789b, 7);
      /* $789B DEX */ s_x = update_nz(s_x - 1);
      /* $789C DEX */ s_x = update_nz(s_x - 1);
      /* $789D BPL */ s_pc = !(s_status & STATUS_N) ? 0x78a1 : 0x789f;
      branchTarget = true;
      break;
    case 0x789f: // [$789F..$78A0]    2 bytes
      CYCLES(0x789f, 4);
      /* $789F CLC */ s_status &= ~STATUS_C;
      /* $78A0 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x78a1: // [$78A1..$78A5]    5 bytes
      CYCLES(0x78a1, 9);
      /* $78A1 CMP */ update_nz_inv_c(s_a - peek(0x1bb0 + s_x));
      /* $78A4 BCC */ s_pc = !(s_status & STATUS_C) ? 0x789b : 0x78a6;
      branchTarget = true;
      break;
    case 0x78a6: // [$78A6..$78AB]    6 bytes
      CYCLES(0x78a6, 11);
      /* $78A6 TYA */ s_a = update_nz(s_y);
      /* $78A7 CMP */ update_nz_inv_c(s_a - peek(0x1bb0 + s_x));
      /* $78AA BCS */ s_pc = s_status & STATUS_C ? 0x7899 : 0x78ac;
      branchTarget = true;
      break;
    case 0x78ac: // [$78AC..$78B2]    7 bytes
      CYCLES(0x78ac, 12);
      /* $78AC LDA */ s_a = update_nz(peek_zpg(0x1d));
      /* $78AE CMP */ update_nz_inv_c(s_a - peek(0x1bd0 + s_x));
      /* $78B1 BCC */ s_pc = !(s_status & STATUS_C) ? 0x7899 : 0x78b3;
      branchTarget = true;
      break;
    case 0x78b3: // [$78B3..$78B6]    4 bytes
      CYCLES(0x78b3, 7);
      /* $78B3 SBC */ tmp = 0x0a, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $78B5 BCC */ s_pc = !(s_status & STATUS_C) ? 0x78bc : 0x78b7;
      branchTarget = true;
      break;
    case 0x78b7: // [$78B7..$78BB]    5 bytes
      CYCLES(0x78b7, 9);
      /* $78B7 CMP */ update_nz_inv_c(s_a - peek(0x1bd0 + s_x));
      /* $78BA BCS */ s_pc = s_status & STATUS_C ? 0x7899 : 0x78bc;
      branchTarget = true;
      break;
    case 0x78bc: // [$78BC..$78BF]    4 bytes
      CYCLES(0x78bc, 7);
      /* $78BC LDA */ s_a = update_nz(peek_zpg(0xe8));
      /* $78BE BEQ */ s_pc = s_status & STATUS_Z ? 0x78c2 : 0x78c0;
      branchTarget = true;
      break;
    case 0x78c0: // [$78C0..$78C1]    2 bytes
      CYCLES(0x78c0, 4);
      /* $78C0 SEC */ s_status |= STATUS_C;
      /* $78C1 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x78c2: // [$78C2..$78C8]    7 bytes
      CYCLES(0x78c2, 12);
      /* $78C2 STX */ poke_zpg(0x00, s_x);
      /* $78C4 LDA */ s_a = update_nz(peek(0x1c51 + s_x));
      /* $78C7 BPL */ s_pc = !(s_status & STATUS_N) ? 0x78fa : 0x78c9;
      branchTarget = true;
      break;
    case 0x78c9: // [$78C9..$78CD]    5 bytes
      CYCLES(0x78c9, 9);
      /* $78C9 LDA */ s_a = update_nz(peek(0x0c2a));
      /* $78CC STA */ poke_zpg(0x01, s_a);
    case 0x78ce: // [$78CE..$78D0]    3 bytes
      CYCLES(0x78ce, 6);
      /* $78CE JSR */ push16(0x78d0), s_pc = 0x7778;
      branchTarget = true;
      break;
    case 0x78d1: // [$78D1..$78D4]    4 bytes
      CYCLES(0x78d1, 7);
      /* $78D1 DEC */ tmp16 = 0x01, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $78D3 BPL */ s_pc = !(s_status & STATUS_N) ? 0x78ce : 0x78d5;
      branchTarget = true;
      break;
    case 0x78d5: // [$78D5..$78E4]   16 bytes
      CYCLES(0x78d5, 28);
      /* $78D5 LDA */ s_a = update_nz(peek(0x1bd0 + s_x));
      /* $78D8 CLC */ s_status &= ~STATUS_C;
      /* $78D9 ADC */ tmp = 0x05, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $78DB TAY */ s_y = update_nz(s_a);
      /* $78DC LDA */ s_a = update_nz(peek(0x1bb0 + s_x));
      /* $78DF TAX */ s_x = update_nz(s_a);
      /* $78E0 LDA */ s_a = update_nz(0x7f);
      /* $78E2 JSR */ push16(0x78e4), s_pc = 0x4f74;
      branchTarget = true;
      break;
    case 0x78e5: // [$78E5..$78F9]   21 bytes
      CYCLES(0x78e5, 36);
      /* $78E5 LDA */ s_a = update_nz(peek(0x0c37));
      /* $78E8 STA */ poke(0x1406, s_a);
      /* $78EB LDA */ s_a = update_nz(peek(0x0c38));
      /* $78EE STA */ poke(0x1422, s_a);
      /* $78F1 LDA */ s_a = update_nz(peek(0x0c39));
      /* $78F4 STA */ poke(0x1423, s_a);
      /* $78F7 SEC */ s_status |= STATUS_C;
      /* $78F8 BCS */ s_pc = s_status & STATUS_C ? 0x7930 : 0x78fa;
      branchTarget = true;
      break;
    case 0x78fa: // [$78FA..$7908]   15 bytes
      CYCLES(0x78fa, 26);
      /* $78FA LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $78FC LDX */ s_x = update_nz(peek(0x1bd0 + s_y));
      /* $78FF DEX */ s_x = update_nz(s_x - 1);
      /* $7900 DEX */ s_x = update_nz(s_x - 1);
      /* $7901 LDA */ s_a = update_nz(peek(0x1bb0 + s_y));
      /* $7904 LDY */ s_y = update_nz(0x0e);
      /* $7906 JSR */ push16(0x7908), s_pc = 0x4cd0;
      branchTarget = true;
      break;
    case 0x7909: // [$7909..$7914]   12 bytes
      CYCLES(0x7909, 21);
      /* $7909 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $790B LDY */ s_y = update_nz(peek(0x1bd0 + s_x));
      /* $790E LDA */ s_a = update_nz(peek(0x1bb0 + s_x));
      /* $7911 TAX */ s_x = update_nz(s_a);
      /* $7912 JSR */ push16(0x7914), s_pc = 0x5a24;
      branchTarget = true;
      break;
    case 0x7915: // [$7915..$7919]    5 bytes
      CYCLES(0x7915, 9);
      /* $7915 LDY */ s_y = update_nz(peek_zpg(0x00));
      /* $7917 LDX */ s_x = update_nz(peek(0x1418));
    case 0x791a: // [$791A..$791C]    3 bytes
      CYCLES(0x791a, 6);
      /* $791A DEX */ s_x = update_nz(s_x - 1);
      /* $791B BMI */ s_pc = s_status & STATUS_N ? 0x792f : 0x791d;
      branchTarget = true;
      break;
    case 0x791d: // [$791D..$7921]    5 bytes
      CYCLES(0x791d, 9);
      /* $791D LDA */ s_a = update_nz(peek(0x1b90 + s_x));
      /* $7920 BMI */ s_pc = s_status & STATUS_N ? 0x791a : 0x7922;
      branchTarget = true;
      break;
    case 0x7922: // [$7922..$7924]    3 bytes
      CYCLES(0x7922, 6);
      /* $7922 JSR */ push16(0x7924), s_pc = 0x7979;
      branchTarget = true;
      break;
    case 0x7925: // [$7925..$7926]    2 bytes
      CYCLES(0x7925, 4);
      /* $7925 BCC */ s_pc = !(s_status & STATUS_C) ? 0x791a : 0x7927;
      branchTarget = true;
      break;
    case 0x7927: // [$7927..$792E]    8 bytes
      CYCLES(0x7927, 14);
      /* $7927 LDA */ s_a = update_nz(0xff);
      /* $7929 STA */ poke(0x1b90 + s_x, s_a);
      /* $792C JSR */ push16(0x792e), s_pc = 0x7459;
      branchTarget = true;
      break;
    case 0x792f: // [$792F..$792F]    1 bytes
      CYCLES(0x792f, 2);
      /* $792F CLC */ s_status &= ~STATUS_C;
    case 0x7930: // [$7930..$7935]    6 bytes
      CYCLES(0x7930, 11);
      /* $7930 PHP */ push8(s_status | STATUS_B);
      /* $7931 LDX */ s_x = update_nz(peek_zpg(0x00));
      /* $7933 JSR */ push16(0x7935), s_pc = 0x7942;
      branchTarget = true;
      break;
    case 0x7936: // [$7936..$7938]    3 bytes
      CYCLES(0x7936, 6);
      /* $7936 PLP */ s_status = pop8() & ~STATUS_B;
      /* $7937 BCC */ s_pc = !(s_status & STATUS_C) ? 0x7940 : 0x7939;
      branchTarget = true;
      break;
    case 0x7939: // [$7939..$793F]    7 bytes
      CYCLES(0x7939, 12);
      /* $7939 LDA */ s_a = update_nz(0x00);
      /* $793B LDX */ s_x = update_nz(0x64);
      /* $793D JSR */ push16(0x793f), s_pc = 0x4dd0;
      branchTarget = true;
      break;
    case 0x7940: // [$7940..$7941]    2 bytes
      CYCLES(0x7940, 4);
      /* $7940 SEC */ s_status |= STATUS_C;
      /* $7941 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x7942: // [$7942..$794C]   11 bytes
      CYCLES(0x7942, 19);
      /* $7942 TXA */ s_a = update_nz(s_x);
      /* $7943 PHA */ push8(s_a);
      /* $7944 DEC */ tmp16 = 0x141b, poke(tmp16, update_nz(peek(tmp16) - 1));
      /* $7947 LDA */ s_a = update_nz(peek(0x141b));
      /* $794A ASL */ s_a = update_nzc(s_a << 1);
      /* $794B STA */ poke_zpg(0x06, s_a);
    case 0x794d: // [$794D..$7975]   41 bytes
      CYCLES(0x794d, 70);
      /* $794D LDA */ s_a = update_nz(peek(0x1bb2 + s_x));
      /* $7950 STA */ poke(0x1bb0 + s_x, s_a);
      /* $7953 LDA */ s_a = update_nz(peek(0x1bd2 + s_x));
      /* $7956 STA */ poke(0x1bd0 + s_x, s_a);
      /* $7959 LDA */ s_a = update_nz(peek(0x1bf2 + s_x));
      /* $795C STA */ poke(0x1bf0 + s_x, s_a);
      /* $795F LDA */ s_a = update_nz(peek(0x1c12 + s_x));
      /* $7962 STA */ poke(0x1c10 + s_x, s_a);
      /* $7965 LDA */ s_a = update_nz(peek(0x1c32 + s_x));
      /* $7968 STA */ poke(0x1c30 + s_x, s_a);
      /* $796B LDA */ s_a = update_nz(peek(0x1c52 + s_x));
      /* $796E STA */ poke(0x1c50 + s_x, s_a);
      /* $7971 INX */ s_x = update_nz(s_x + 1);
      /* $7972 CPX */ update_nz_inv_c(s_x - peek_zpg(0x06));
      /* $7974 BCC */ s_pc = !(s_status & STATUS_C) ? 0x794d : 0x7976;
      branchTarget = true;
      break;
    case 0x7976: // [$7976..$7978]    3 bytes
      CYCLES(0x7976, 6);
      /* $7976 PLA */ s_a = update_nz(pop8());
      /* $7977 TAX */ s_x = update_nz(s_a);
      /* $7978 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x7979: // [$7979..$7980]    8 bytes
      CYCLES(0x7979, 14);
      /* $7979 LDA */ s_a = update_nz(peek(0x1b30 + s_x));
      /* $797C CMP */ update_nz_inv_c(s_a - peek(0x1bd0 + s_y));
      /* $797F BNE */ s_pc = !(s_status & STATUS_Z) ? 0x7996 : 0x7981;
      branchTarget = true;
      break;
    case 0x7981: // [$7981..$798B]   11 bytes
      CYCLES(0x7981, 19);
      /* $7981 LDA */ s_a = update_nz(peek(0x1b20 + s_x));
      /* $7984 CLC */ s_status &= ~STATUS_C;
      /* $7985 ADC */ tmp = 0x12, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $7987 CMP */ update_nz_inv_c(s_a - peek(0x1bb0 + s_y));
      /* $798A BEQ */ s_pc = s_status & STATUS_Z ? 0x7994 : 0x798c;
      branchTarget = true;
      break;
    case 0x798c: // [$798C..$7993]    8 bytes
      CYCLES(0x798c, 14);
      /* $798C SEC */ s_status |= STATUS_C;
      /* $798D SBC */ tmp = 0x1e, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $798F CMP */ update_nz_inv_c(s_a - peek(0x1bb0 + s_y));
      /* $7992 BNE */ s_pc = !(s_status & STATUS_Z) ? 0x7996 : 0x7994;
      branchTarget = true;
      break;
    case 0x7994: // [$7994..$7995]    2 bytes
      CYCLES(0x7994, 4);
      /* $7994 SEC */ s_status |= STATUS_C;
      /* $7995 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0x7996: // [$7996..$7997]    2 bytes
      CYCLES(0x7996, 4);
      /* $7996 CLC */ s_status &= ~STATUS_C;
      /* $7997 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xfb1e: // [$FB1E..$FB1E]    1 bytes
      CYCLES(0xfb1e, 2);
      /* $FB1E ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0xfb1e);
                      error_handler(s_pc);
      break;
    case 0xfca8: // [$FCA8..$FCA8]    1 bytes
      CYCLES(0xfca8, 2);
      /* $FCA8 ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0xfca8);
                      error_handler(s_pc);
      break;
    default:
      fprintf(stderr, "Unknown code address: $%04X\n", s_pc);
      error_handler(s_pc);
    }
  }
}
static const uint8_t s_mem_2dfd[0x6200] = {
  0x4C, 0x00, 0x40, 0x00, 0x00, 0x10, 0x20, 0xFF, 0xFF, 0x30, 0x40, 0x50, 0x60, 0x10, 0x20, 0xFF,
  0xFF, 0x30, 0x40, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x50, 0x60, 0x10, 0x20, 0xFF,
  0xFF, 0x30, 0x40, 0x7A, 0x7A, 0x7A, 0x7A, 0xFF, 0xFF, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0xFF,
  0xFF, 0x7A, 0x7A, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7A, 0x7A, 0x7A, 0x7A, 0xFF,
  0xFF, 0x7A, 0x7A, 0xD0, 0xD0, 0xE0, 0xF0, 0xFF, 0xFF, 0x00, 0x10, 0x20, 0x30, 0xE0, 0xF0, 0xFF,
  0xFF, 0x00, 0x10, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x20, 0x30, 0xE0, 0xF0, 0xFF,
  0xFF, 0x00, 0x10, 0x7B, 0x7B, 0x7B, 0x7B, 0xFF, 0xFF, 0x7C, 0x7C, 0x7C, 0x7C, 0x7B, 0x7B, 0xFF,
  0xFF, 0x7C, 0x7C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7C, 0x7C, 0x7B, 0x7B, 0xFF,
  0xFF, 0x7C, 0x7C, 0x70, 0x80, 0xFF, 0xFF, 0x7A, 0x7A, 0xFF, 0xFF, 0x40, 0x50, 0xFF, 0xFF, 0x7C,
  0x7C, 0xFF, 0xFF, 0xE0, 0xD0, 0x00, 0xF0, 0x20, 0x10, 0x20, 0x10, 0x7A, 0x7A, 0x7B, 0x7A, 0x7B,
  0x7B, 0x7B, 0x7B, 0x10, 0x20, 0x30, 0x40, 0x90, 0xA0, 0xB0, 0xC0, 0x70, 0x80, 0x90, 0xA0, 0xFF,
  0xFF, 0xFF, 0xFF, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7D, 0x7D, 0x7D, 0x7D, 0xFF,
  0xFF, 0xFF, 0xFF, 0x60, 0x50, 0x40, 0x30, 0xFF, 0xFF, 0xFF, 0xFF, 0x7C, 0x7B, 0x7B, 0x7B, 0xFF,
  0xFF, 0xFF, 0xFF, 0x60, 0xFF, 0xFF, 0xFF, 0x7B, 0xFF, 0xFF, 0xFF, 0x70, 0x80, 0xFF, 0xFF, 0x7B,
  0x7B, 0xFF, 0xFF, 0x90, 0xA0, 0xB0, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0x7B, 0x7B, 0x7B, 0x7B, 0xFF,
  0xFF, 0xFF, 0xFF, 0x70, 0x80, 0x90, 0x90, 0xA0, 0xB0, 0xC0, 0xC0, 0xB0, 0xC0, 0xD0, 0xD0, 0xFF,
  0xFF, 0xFF, 0xFF, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x7C, 0x7D, 0x7D, 0x7D, 0x7D, 0xFF,
  0xFF, 0xFF, 0xFF, 0x40, 0x30, 0x20, 0x10, 0xFF, 0xFF, 0xFF, 0xFF, 0x7D, 0x7D, 0x7D, 0x7D, 0xFF,
  0xFF, 0xFF, 0xFF, 0xD0, 0xE0, 0xF0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x7C, 0x7C, 0x7C, 0x7D, 0xFF,
  0xFF, 0xFF, 0xFF, 0x50, 0x60, 0xFF, 0xFF, 0x7D, 0x7D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x00, 0x10, 0x20, 0x30, 0x40, 0x90, 0xA0, 0xFF, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x7F, 0x7F, 0xFF, 0x80, 0x70, 0x60, 0x50, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x7F, 0x7F, 0xFF,
  0xFF, 0xFF, 0xFF, 0xF0, 0xE0, 0xFF, 0xFF, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0xFF, 0x05, 0x01, 0x0D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x0C, 0x04, 0xFF, 0x07, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07,
  0xFF, 0x04, 0x05, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0xFF, 0x03, 0x00,
  0xFF, 0xFF, 0x0C, 0x0D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05,
  0x00, 0x05, 0x05, 0x0D, 0x0F, 0x0F, 0x0F, 0x0F, 0x0C, 0x0C, 0x0C, 0x0D, 0x0D, 0x0D, 0x05, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x03, 0x07, 0x07, 0x05, 0x01, 0x05, 0x05, 0x05,
  0x05, 0x05, 0x01, 0x01, 0x03, 0x03, 0x07, 0x03, 0x01, 0x07, 0x03, 0x07, 0x01, 0x07, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x08, 0x78, 0x10, 0x20, 0x30, 0x04, 0x04, 0x08, 0x05, 0x04, 0x18, 0x08, 0x18,
  0x06, 0x20, 0x0D, 0xA8, 0x61, 0x00, 0x03, 0x28, 0x0C, 0x1C, 0x0A, 0x01, 0x01, 0x01, 0x10, 0x07,
  0xFF, 0x08, 0x10, 0xF0, 0x60, 0x08, 0x20, 0x04, 0x04, 0x2C, 0xA0, 0x30, 0x30, 0x06, 0x18, 0x20,
  0x04, 0x07, 0xA0, 0x18, 0xFF, 0x02, 0xFF, 0x60, 0x06, 0x06, 0x60, 0x02, 0x20, 0xD0, 0xF0, 0xFF,
  0xFF, 0x00, 0x60, 0x01, 0x00, 0xFF, 0x08, 0x04, 0x0D, 0x40, 0x03, 0x10, 0x40, 0x10, 0x18, 0x28,
  0x02, 0x02, 0x01, 0x30, 0x06, 0x08, 0x01, 0x20, 0x80, 0xE0, 0x03, 0x10, 0x1C, 0x04, 0x30, 0xF8,
  0x08, 0x06, 0x02, 0x03, 0x0B, 0x00, 0x03, 0x90, 0x0A, 0x60, 0x80, 0xC0, 0x17, 0x05, 0x0C, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x12, 0x01, 0x08, 0x0A, 0x06, 0x14, 0x04, 0x1E, 0x02, 0x28, 0x00, 0xFF, 0x13,
  0x01, 0x08, 0x0A, 0x06, 0x14, 0x04, 0x1E, 0x02, 0x28, 0x00, 0xFF, 0x14, 0x01, 0x40, 0x10, 0x30,
  0x20, 0x20, 0x30, 0x18, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x00, 0x02, 0x12, 0xFE, 0x03, 0xFD, 0x07, 0xF9, 0x0C, 0xF4, 0x10, 0xF0, 0xFF,
  0xFF, 0xBE, 0xBE, 0x9C, 0x9C, 0x8C, 0x8C, 0x04, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x04, 0xFC, 0x02,
  0xFE, 0x0C, 0x0D, 0x01, 0x05, 0x04, 0x07, 0x03, 0x0F, 0x7F, 0x3F, 0x3E, 0x1E, 0x1C, 0x0C, 0x7F,
  0x3F, 0x3E, 0x1E, 0x1C, 0x0C, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0xFE, 0xFC, 0xFA, 0xF8, 0xF6,
  0xF4, 0x06, 0xFA, 0x00, 0x00, 0x00, 0x00, 0x06, 0xFA, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xD8, 0xD8, 0xD8, 0x00, 0xFF, 0xFF, 0xFF, 0xD8, 0xD8, 0xB4, 0xB4, 0x80, 0x80, 0x80, 0x00,
  0x00, 0xEA, 0x02, 0xEA, 0x06, 0x04, 0x03, 0xEA, 0x05, 0xEA, 0xEA, 0xEA, 0xEA, 0x00, 0x01, 0xEA,
  0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x0B, 0x12, 0x15, 0x19, 0x0C, 0x15, 0x00, 0x15, 0x1E, 0x0F, 0x19, 0x00, 0x19,
  0x19, 0x11, 0x19, 0x00, 0x1A, 0x24, 0x11, 0x19, 0x00, 0x19, 0x1D, 0x12, 0x19, 0x00, 0x19, 0x28,
  0x13, 0x1D, 0x00, 0x20, 0x1D, 0x14, 0x1D, 0x00, 0x20, 0x2B, 0x14, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01, 0x28, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x80, 0x00, 0x09, 0x00, 0x00, 0x00,
  0x00, 0x28, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x14, 0x00, 0x10, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x04, 0x05, 0x05, 0x00, 0x06, 0x08, 0x06, 0x02, 0x00, 0x07, 0x09, 0x07,
  0x10, 0x00, 0x07, 0x0A, 0x08, 0x02, 0x00, 0x07, 0x0A, 0x07, 0x07, 0x03, 0x09, 0x0A, 0x02, 0x09,
  0x04, 0x07, 0x0B, 0x07, 0x10, 0x04, 0x09, 0x03, 0x09, 0x09, 0x04, 0x01, 0x01, 0x01, 0x10, 0x01,
  0x01, 0x01, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x10, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x10, 0x00, 0x00, 0x10, 0x02, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x00, 0x03, 0x05, 0x04, 0x02, 0x0E, 0x05, 0x05, 0x05,
  0x07, 0x00, 0x06, 0x07, 0x03, 0x06, 0x05, 0x06, 0x07, 0x06, 0x06, 0x00, 0x06, 0x03, 0x06, 0x07,
  0x0F, 0x07, 0x05, 0x07, 0x06, 0x00, 0x03, 0x05, 0x07, 0x07, 0x06, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x05, 0x05, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00,
  0x00, 0x01, 0x01, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x10, 0x00, 0x00, 0x09, 0x00, 0x01, 0x00, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x03, 0x0C, 0x05, 0x05, 0x04, 0x02, 0x00, 0x04, 0x05, 0x04,
  0x06, 0x00, 0x05, 0x06, 0x02, 0x05, 0x06, 0x06, 0x07, 0x06, 0x06, 0x0F, 0x06, 0x03, 0x06, 0x07,
  0x00, 0x06, 0x05, 0x06, 0x08, 0x01, 0x03, 0x05, 0x07, 0x07, 0x06, 0x01, 0x01, 0x01, 0x01, 0x0E,
  0x01, 0x01, 0x05, 0x05, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01,
  0x00, 0x01, 0x00, 0x10, 0x05, 0x05, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01,
  0x06, 0x00, 0x00, 0x00, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x01, 0x02, 0x02, 0x02, 0x00, 0x00, 0x02, 0x02, 0x02,
  0x02, 0x10, 0x02, 0x02, 0x01, 0x02, 0x05, 0x02, 0x02, 0x02, 0x02, 0x01, 0x02, 0x01, 0x02, 0x02,
  0x00, 0x02, 0x02, 0x02, 0x02, 0x0F, 0x01, 0x02, 0x02, 0x02, 0x04, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x10, 0x05, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x00,
  0x0E, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0x06, 0x09, 0x03, 0x09, 0x00, 0x09, 0x09, 0x06, 0x09, 0x00, 0x09,
  0x06, 0x06, 0x09, 0x00, 0x09, 0x0A, 0x07, 0x09, 0x00, 0x0C, 0x0C, 0x07, 0x09, 0x07, 0x0C, 0x0B,
  0x0C, 0x0C, 0x00, 0x0C, 0x0C, 0x0C, 0x0C, 0x0A, 0x0F, 0x0B, 0x0F, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x05, 0x05, 0x00, 0x00,
  0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00,
  0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x00,
  0x0E, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x01, 0x01, 0x01, 0x01, 0x10,
  0x01, 0x01, 0x00, 0x00, 0x10, 0x00, 0x01, 0x01, 0x01, 0x00, 0x10, 0x00, 0x00, 0x00, 0x01, 0x10,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
  0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x00,
  0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00,
  0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x05, 0x01, 0x01,
  0x00, 0x00, 0x09, 0x01, 0x00, 0x00, 0x01, 0x07, 0x02, 0x02, 0x01, 0x01, 0x06, 0x01, 0x01, 0x01,
  0x01, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x01, 0x10, 0x01, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01,
  0x01, 0x00, 0x04, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x10,
  0x01, 0x10, 0x00, 0x10, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8,
  0xA8, 0xA8, 0xA8, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8,
  0xA8, 0xA8, 0xA8, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8,
  0xA8, 0xA8, 0xA8, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0x28, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8,
  0xA8, 0xA8, 0xA8, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0,
  0xD0, 0xD0, 0xD0, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0,
  0xD0, 0xD0, 0xD0, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0,
  0xD0, 0xD0, 0xD0, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0,
  0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0,
  0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0,
  0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0,
  0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0, 0xD0,
  0xD0, 0xD0, 0xD0, 0x20, 0x24, 0x28, 0x2C, 0x30, 0x34, 0x38, 0x3C, 0x20, 0x24, 0x28, 0x2C, 0x30,
  0x34, 0x38, 0x3C, 0x21, 0x25, 0x29, 0x2D, 0x31, 0x35, 0x39, 0x3D, 0x21, 0x25, 0x29, 0x2D, 0x31,
  0x35, 0x39, 0x3D, 0x22, 0x26, 0x2A, 0x2E, 0x32, 0x36, 0x3A, 0x3E, 0x22, 0x26, 0x2A, 0x2E, 0x32,
  0x36, 0x3A, 0x3E, 0x23, 0x27, 0x2B, 0x2F, 0x33, 0x37, 0x3B, 0x3F, 0x23, 0x27, 0x2B, 0x2F, 0x33,
  0x37, 0x3B, 0x3F, 0x20, 0x24, 0x28, 0x2C, 0x30, 0x34, 0x38, 0x3C, 0x20, 0x24, 0x28, 0x2C, 0x30,
  0x34, 0x38, 0x3C, 0x21, 0x25, 0x29, 0x2D, 0x31, 0x35, 0x39, 0x3D, 0x21, 0x25, 0x29, 0x2D, 0x31,
  0x35, 0x39, 0x3D, 0x22, 0x26, 0x2A, 0x2E, 0x32, 0x36, 0x3A, 0x3E, 0x22, 0x26, 0x2A, 0x2E, 0x32,
  0x36, 0x3A, 0x3E, 0x23, 0x27, 0x2B, 0x2F, 0x33, 0x37, 0x3B, 0x3F, 0x23, 0x27, 0x2B, 0x2F, 0x33,
  0x37, 0x3B, 0x3F, 0x20, 0x24, 0x28, 0x2C, 0x30, 0x34, 0x38, 0x3C, 0x20, 0x24, 0x28, 0x2C, 0x30,
  0x34, 0x38, 0x3C, 0x21, 0x25, 0x29, 0x2D, 0x31, 0x35, 0x39, 0x3D, 0x21, 0x25, 0x29, 0x2D, 0x31,
  0x35, 0x39, 0x3D, 0x22, 0x26, 0x2A, 0x2E, 0x32, 0x36, 0x3A, 0x3E, 0x22, 0x26, 0x2A, 0x2E, 0x32,
  0x36, 0x3A, 0x3E, 0x23, 0x27, 0x2B, 0x2F, 0x33, 0x37, 0x3B, 0x3F, 0x23, 0x27, 0x2B, 0x2F, 0x33,
  0x37, 0x3B, 0x3F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
  0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
  0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
  0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F,
  0x1F, 0x1F, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x4C, 0x58, 0x01, 0x4C, 0x4F, 0x03, 0x4C, 0x8E, 0x02, 0x4C, 0xB4, 0x03, 0x4C,
  0xFE, 0x03, 0x4C, 0x74, 0x04, 0x4C, 0xE9, 0x04, 0x4C, 0x4A, 0x05, 0x20, 0xD3, 0x04, 0x0D, 0x03,
  0x18, 0x49, 0x4E, 0x53, 0x50, 0x49, 0x52, 0x45, 0x44, 0x20, 0x42, 0x59, 0x20, 0x48, 0x49, 0x53,
  0x20, 0x4E, 0x45, 0x56, 0x45, 0x52, 0x2D, 0x45, 0x4E, 0x44, 0x49, 0x4E, 0x47, 0x20, 0x51, 0x55,
  0x45, 0x53, 0x54, 0x0D, 0x03, 0x28, 0x46, 0x4F, 0x52, 0x20, 0x50, 0x52, 0x4F, 0x47, 0x52, 0x45,
  0x53, 0x53, 0x2C, 0x20, 0x49, 0x4E, 0x20, 0x32, 0x30, 0x38, 0x34, 0x20, 0x4D, 0x41, 0x4E, 0x20,
  0x50, 0x45, 0x52, 0x46, 0x45, 0x43, 0x54, 0x53, 0x0D, 0x03, 0x38, 0x54, 0x48, 0x45, 0x20, 0x52,
  0x4F, 0x42, 0x4F, 0x54, 0x52, 0x4F, 0x4E, 0x53, 0x3A, 0x02, 0x08, 0x0D, 0x03, 0x50, 0x41, 0x20,
  0x52, 0x4F, 0x42, 0x4F, 0x54, 0x20, 0x53, 0x50, 0x45, 0x43, 0x49, 0x45, 0x53, 0x20, 0x53, 0x4F,
  0x20, 0x41, 0x44, 0x56, 0x41, 0x4E, 0x43, 0x45, 0x44, 0x20, 0x54, 0x48, 0x41, 0x54, 0x20, 0x4D,
  0x41, 0x4E, 0x0D, 0x03, 0x60, 0x49, 0x53, 0x20, 0x49, 0x4E, 0x46, 0x45, 0x52, 0x49, 0x4F, 0x52,
  0x20, 0x54, 0x4F, 0x20, 0x48, 0x49, 0x53, 0x20, 0x4F, 0x57, 0x4E, 0x20, 0x43, 0x52, 0x45, 0x41,
  0x54, 0x49, 0x4F, 0x4E, 0x2E, 0x02, 0x08, 0x0D, 0x03, 0x78, 0x47, 0x55, 0x49, 0x44, 0x45, 0x44,
  0x20, 0x42, 0x59, 0x20, 0x54, 0x48, 0x45, 0x49, 0x52, 0x20, 0x49, 0x4E, 0x46, 0x41, 0x4C, 0x4C,
  0x49, 0x42, 0x4C, 0x45, 0x20, 0x4C, 0x4F, 0x47, 0x49, 0x43, 0x2C, 0x0D, 0x03, 0x88, 0x54, 0x48,
  0x45, 0x20, 0x52, 0x4F, 0x42, 0x4F, 0x54, 0x52, 0x4F, 0x4E, 0x53, 0x20, 0x43, 0x4F, 0x4E, 0x43,
  0x4C, 0x55, 0x44, 0x45, 0x3A, 0x02, 0x08, 0x0D, 0x03, 0xA0, 0x54, 0x48, 0x45, 0x20, 0x48, 0x55,
  0x4D, 0x41, 0x4E, 0x20, 0x52, 0x41, 0x43, 0x45, 0x20, 0x49, 0x53, 0x20, 0x49, 0x4E, 0x45, 0x46,
  0x46, 0x49, 0x43, 0x49, 0x45, 0x4E, 0x54, 0x2C, 0x20, 0x41, 0x4E, 0x44, 0x0D, 0x03, 0xB0, 0x54,
  0x48, 0x45, 0x52, 0x45, 0x46, 0x4F, 0x52, 0x45, 0x20, 0x4D, 0x55, 0x53, 0x54, 0x20, 0x42, 0x45,
  0x20, 0x44, 0x45, 0x53, 0x54, 0x52, 0x4F, 0x59, 0x45, 0x44, 0x2E, 0x02, 0x60, 0x02, 0x01, 0x00,
  0x60, 0x20, 0xD3, 0x04, 0x0D, 0x03, 0x18, 0x59, 0x4F, 0x55, 0x20, 0x41, 0x52, 0x45, 0x20, 0x54,
  0x48, 0x45, 0x20, 0x4C, 0x41, 0x53, 0x54, 0x20, 0x48, 0x4F, 0x50, 0x45, 0x20, 0x4F, 0x46, 0x20,
  0x4D, 0x41, 0x4E, 0x4B, 0x49, 0x4E, 0x44, 0x2E, 0x02, 0x08, 0x0D, 0x03, 0x30, 0x44, 0x55, 0x45,
  0x20, 0x54, 0x4F, 0x20, 0x41, 0x20, 0x47, 0x45, 0x4E, 0x45, 0x54, 0x49, 0x43, 0x20, 0x45, 0x4E,
  0x47, 0x49, 0x4E, 0x45, 0x45, 0x52, 0x49, 0x4E, 0x47, 0x20, 0x45, 0x52, 0x52, 0x4F, 0x52, 0x2C,
  0x0D, 0x03, 0x40, 0x59, 0x4F, 0x55, 0x20, 0x50, 0x4F, 0x53, 0x53, 0x45, 0x53, 0x53, 0x20, 0x53,
  0x55, 0x50, 0x45, 0x52, 0x48, 0x55, 0x4D, 0x41, 0x4E, 0x20, 0x50, 0x4F, 0x57, 0x45, 0x52, 0x53,
  0x2E, 0x02, 0x08, 0x0D, 0x03, 0x58, 0x59, 0x4F, 0x55, 0x52, 0x20, 0x4D, 0x49, 0x53, 0x53, 0x49,
  0x4F, 0x4E, 0x20, 0x49, 0x53, 0x20, 0x54, 0x4F, 0x20, 0x53, 0x54, 0x4F, 0x50, 0x20, 0x54, 0x48,
  0x45, 0x0D, 0x03, 0x68, 0x52, 0x4F, 0x42, 0x4F, 0x54, 0x52, 0x4F, 0x4E, 0x53, 0x2C, 0x20, 0x41,
  0x4E, 0x44, 0x20, 0x53, 0x41, 0x56, 0x45, 0x20, 0x54, 0x48, 0x45, 0x20, 0x4C, 0x41, 0x53, 0x54,
  0x0D, 0x03, 0x78, 0x48, 0x55, 0x4D, 0x41, 0x4E, 0x20, 0x46, 0x41, 0x4D, 0x49, 0x4C, 0x59, 0x2E,
  0x00, 0x60, 0x20, 0xD3, 0x04, 0x0D, 0x03, 0x28, 0x54, 0x48, 0x45, 0x20, 0x46, 0x4F, 0x52, 0x43,
  0x45, 0x20, 0x4F, 0x46, 0x20, 0x47, 0x52, 0x4F, 0x55, 0x4E, 0x44, 0x20, 0x52, 0x4F, 0x56, 0x49,
  0x4E, 0x47, 0x20, 0x55, 0x4E, 0x49, 0x54, 0x0D, 0x03, 0x38, 0x4E, 0x45, 0x54, 0x57, 0x4F, 0x52,
  0x4B, 0x20, 0x54, 0x45, 0x52, 0x4D, 0x49, 0x4E, 0x41, 0x54, 0x4F, 0x52, 0x20, 0x28, 0x47, 0x52,
  0x55, 0x4E, 0x54, 0x29, 0x0D, 0x03, 0x48, 0x52, 0x4F, 0x42, 0x4F, 0x54, 0x52, 0x4F, 0x4E, 0x53,
  0x20, 0x53, 0x45, 0x45, 0x4B, 0x20, 0x54, 0x4F, 0x20, 0x44, 0x45, 0x53, 0x54, 0x52, 0x4F, 0x59,
  0x20, 0x59, 0x4F, 0x55, 0x2E, 0x00, 0x60, 0x20, 0xD3, 0x04, 0x0D, 0x03, 0x38, 0x54, 0x48, 0x45,
  0x20, 0x48, 0x55, 0x4C, 0x4B, 0x20, 0x52, 0x4F, 0x42, 0x4F, 0x54, 0x52, 0x4F, 0x4E, 0x53, 0x20,
  0x53, 0x45, 0x45, 0x4B, 0x20, 0x4F, 0x55, 0x54, 0x20, 0x41, 0x4E, 0x44, 0x0D, 0x03, 0x48, 0x45,
  0x4C, 0x49, 0x4D, 0x49, 0x4E, 0x41, 0x54, 0x45, 0x20, 0x54, 0x48, 0x45, 0x20, 0x4C, 0x41, 0x53,
  0x54, 0x20, 0x48, 0x55, 0x4D, 0x41, 0x4E, 0x20, 0x46, 0x41, 0x4D, 0x49, 0x4C, 0x59, 0x2E, 0x00,
  0x60, 0x20, 0xD3, 0x04, 0x0D, 0x03, 0x28, 0x42, 0x45, 0x57, 0x41, 0x52, 0x45, 0x20, 0x4F, 0x46,
  0x20, 0x54, 0x48, 0x45, 0x20, 0x49, 0x4E, 0x47, 0x45, 0x4E, 0x49, 0x4F, 0x55, 0x53, 0x0D, 0x03,
  0x38, 0x42, 0x52, 0x41, 0x49, 0x4E, 0x20, 0x52, 0x4F, 0x42, 0x4F, 0x54, 0x52, 0x4F, 0x4E, 0x53,
  0x2C, 0x20, 0x54, 0x48, 0x41, 0x54, 0x20, 0x50, 0x4F, 0x53, 0x53, 0x45, 0x53, 0x53, 0x0D, 0x03,
  0x48, 0x54, 0x48, 0x45, 0x20, 0x50, 0x4F, 0x57, 0x45, 0x52, 0x20, 0x54, 0x4F, 0x20, 0x52, 0x45,
  0x50, 0x52, 0x4F, 0x47, 0x52, 0x41, 0x4D, 0x20, 0x48, 0x55, 0x4D, 0x41, 0x4E, 0x53, 0x0D, 0x03,
  0x58, 0x49, 0x4E, 0x54, 0x4F, 0x20, 0x53, 0x49, 0x4E, 0x49, 0x53, 0x54, 0x45, 0x52, 0x20, 0x50,
  0x52, 0x4F, 0x47, 0x53, 0x2E, 0x00, 0x60, 0x20, 0xD3, 0x04, 0x0D, 0x03, 0x38, 0x54, 0x48, 0x45,
  0x20, 0x53, 0x50, 0x48, 0x45, 0x52, 0x4F, 0x49, 0x44, 0x53, 0x20, 0x41, 0x4E, 0x44, 0x20, 0x51,
  0x55, 0x41, 0x52, 0x4B, 0x53, 0x0D, 0x03, 0x48, 0x41, 0x52, 0x45, 0x20, 0x50, 0x52, 0x4F, 0x47,
  0x52, 0x41, 0x4D, 0x4D, 0x45, 0x44, 0x20, 0x54, 0x4F, 0x20, 0x4D, 0x41, 0x4E, 0x55, 0x46, 0x41,
  0x43, 0x54, 0x55, 0x52, 0x45, 0x0D, 0x03, 0x58, 0x45, 0x4E, 0x46, 0x4F, 0x52, 0x43, 0x45, 0x52,
  0x20, 0x41, 0x4E, 0x44, 0x20, 0x54, 0x41, 0x4E, 0x4B, 0x20, 0x52, 0x4F, 0x42, 0x4F, 0x54, 0x52,
  0x4F, 0x4E, 0x53, 0x2E, 0x00, 0x60, 0x68, 0x85, 0x28, 0x68, 0x85, 0x29, 0xA9, 0x00, 0x85, 0x2A,
  0xA9, 0xFF, 0x85, 0x2B, 0x85, 0x2C, 0xA2, 0x01, 0x20, 0x40, 0x05, 0x60, 0xA0, 0x00, 0xB1, 0x28,
  0xD0, 0x01, 0x60, 0xA6, 0x2A, 0xF0, 0x03, 0xC6, 0x2A, 0x60, 0xC9, 0x01, 0xD0, 0x0A, 0xC8, 0xB1,
  0x28, 0x85, 0x2B, 0xA2, 0x02, 0x4C, 0x40, 0x05, 0xC9, 0x02, 0xD0, 0x0A, 0xC8, 0xB1, 0x28, 0x85,
  0x2A, 0xA2, 0x02, 0x4C, 0x40, 0x05, 0xC9, 0x0D, 0xD0, 0x0F, 0xC8, 0xB1, 0x28, 0x85, 0x2D, 0xC8,
  0xB1, 0x28, 0x85, 0x2E, 0xA2, 0x03, 0x4C, 0x40, 0x05, 0xA6, 0x2D, 0xA4, 0x2E, 0x48, 0x20, 0x08,
  0x51, 0x68, 0xC9, 0x20, 0xF0, 0x09, 0xA5, 0x2F, 0xC9, 0x60, 0xF0, 0x03, 0xAD, 0x30, 0xC0, 0xE6,
  0x2D, 0xA2, 0x01, 0xE6, 0x28, 0xD0, 0x02, 0xE6, 0x29, 0xCA, 0xD0, 0xF7, 0x60, 0x20, 0xB8, 0x51,
  0x0D, 0x03, 0xA0, 0x43, 0x52, 0x41, 0x4B, 0x45, 0x44, 0x20, 0x71, 0x79, 0x78, 0x74, 0x20, 0x42,
  0x59, 0x20, 0x54, 0x48, 0x45, 0x20, 0x4D, 0x41, 0x53, 0x4B, 0x20, 0x4F, 0x46, 0x20, 0x44, 0x45,
  0x41, 0x54, 0x48, 0x61, 0x0D, 0x0A, 0xAB, 0x24, 0x20, 0x31, 0x39, 0x38, 0x33, 0x20, 0x41, 0x54,
  0x41, 0x52, 0x49, 0x2C, 0x20, 0x49, 0x4E, 0x43, 0x2E, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0xCC, 0x81, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xCE, 0x83,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xCB, 0x86, 0x80, 0x80, 0x80, 0x80, 0x80, 0xC0, 0xC9, 0x8C,
  0x80, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xC8, 0x98, 0x80, 0x80, 0x80, 0x80, 0x80, 0xB0, 0xC8, 0xB0,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x98, 0xC8, 0xE0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x8C, 0xC8, 0xC4,
  0x81, 0x80, 0x80, 0x80, 0x80, 0x86, 0xC9, 0x8C, 0x83, 0x80, 0x80, 0x80, 0x80, 0xC3, 0xC9, 0x9C,
  0x86, 0x80, 0x80, 0x80, 0x80, 0xE1, 0xC9, 0xB4, 0x84, 0x80, 0x80, 0xFE, 0x9F, 0xB1, 0xC9, 0xE4,
  0x84, 0xC0, 0x87, 0x82, 0x90, 0x99, 0xC9, 0xC4, 0x84, 0xC0, 0x84, 0x82, 0x90, 0x89, 0xC9, 0xC4,
  0x84, 0xC0, 0x84, 0xE2, 0x9F, 0x89, 0xC9, 0xC4, 0x84, 0xC0, 0x84, 0xC6, 0x81, 0x89, 0xC9, 0xC4,
  0x84, 0xC0, 0x84, 0x8C, 0x83, 0x89, 0xC9, 0xC4, 0x84, 0xC0, 0x84, 0x98, 0x86, 0x89, 0xC9, 0xC4,
  0xE4, 0xFF, 0x84, 0xB0, 0x8C, 0x89, 0xC9, 0xFC, 0xA4, 0x80, 0x84, 0xE0, 0x98, 0x89, 0xC9, 0x80,
  0xA4, 0x80, 0x84, 0xC0, 0x91, 0x89, 0xC9, 0x80, 0xA4, 0xFE, 0x84, 0x80, 0x93, 0x89, 0xC9, 0xFC,
  0xA4, 0xC2, 0x84, 0x9E, 0x92, 0x89, 0xC9, 0xC4, 0xA4, 0xC2, 0x84, 0xA2, 0x91, 0x89, 0xC9, 0xC4,
  0xA4, 0xC2, 0x84, 0xE2, 0x91, 0x89, 0xC9, 0xC4, 0xA4, 0xC2, 0x84, 0x86, 0x98, 0x89, 0xC9, 0xC4,
  0xA4, 0xC2, 0x84, 0x8C, 0x8C, 0x89, 0xC9, 0xC4, 0xA4, 0xC2, 0x84, 0x98, 0x86, 0x89, 0xC9, 0xC4,
  0xA4, 0xC2, 0x84, 0xB0, 0x83, 0x99, 0xC9, 0xE4, 0xA4, 0xC2, 0x84, 0xE0, 0x81, 0xB1, 0xC9, 0xB4,
  0xE4, 0xC3, 0x87, 0x80, 0x80, 0xE3, 0xC9, 0x9C, 0x86, 0x80, 0x80, 0x80, 0x80, 0xC6, 0xC9, 0x8C,
  0x83, 0x80, 0x80, 0x80, 0x80, 0x8C, 0xC9, 0xC4, 0x81, 0x80, 0x80, 0x80, 0x80, 0x98, 0xC8, 0xE0,
  0x80, 0x80, 0x80, 0x80, 0x80, 0xB0, 0xC8, 0xB0, 0x80, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xC8, 0x98,
  0x80, 0x80, 0x80, 0x80, 0x80, 0xC0, 0xC9, 0x8C, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xCB, 0x86,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xCE, 0x83, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xCC, 0x81,
  0x80, 0x80, 0x80, 0xA2, 0x3F, 0x9A, 0xA9, 0x2E, 0xA2, 0x0C, 0xA0, 0x08, 0x20, 0x9D, 0x44, 0xA9,
  0x3A, 0xA2, 0x06, 0xA0, 0x01, 0x20, 0x9D, 0x44, 0xA2, 0x3F, 0x9A, 0x20, 0x2F, 0x51, 0xA9, 0xEA,
  0xA2, 0x14, 0x9D, 0x00, 0x40, 0xCA, 0x10, 0xFA, 0xA9, 0x12, 0x85, 0xFD, 0xA9, 0x13, 0x85, 0xFF,
  0xAD, 0x50, 0xC0, 0xAD, 0x52, 0xC0, 0xAD, 0x54, 0xC0, 0xAD, 0x57, 0xC0, 0xA9, 0x00, 0x8D, 0x07,
  0x14, 0x8D, 0x0C, 0x15, 0x8D, 0x32, 0x14, 0x8D, 0x33, 0x14, 0x8D, 0x34, 0x14, 0xAD, 0x10, 0x0C,
  0x8D, 0x0A, 0x15, 0xAD, 0x11, 0x0C, 0x8D, 0x0B, 0x15, 0xA9, 0x01, 0x8D, 0x17, 0x14, 0xA9, 0x06,
  0x8D, 0x09, 0x15, 0xA2, 0x3F, 0x9A, 0x20, 0x04, 0x45, 0x20, 0x67, 0x4B, 0xAD, 0x6A, 0x0C, 0x8D,
  0x09, 0x15, 0xA9, 0x00, 0x8D, 0x07, 0x14, 0x8D, 0x0A, 0x15, 0x8D, 0x0B, 0x15, 0x8D, 0x0C, 0x15,
  0xAE, 0x10, 0x0C, 0xCA, 0x8E, 0x0D, 0x15, 0xAE, 0x11, 0x0C, 0x8E, 0x0E, 0x15, 0x20, 0x2A, 0x44,
  0x20, 0xC0, 0x43, 0xA2, 0x3F, 0x9A, 0x20, 0x6B, 0x42, 0x90, 0x14, 0x20, 0x4A, 0x4D, 0xEE, 0x07,
  0x14, 0xAD, 0x07, 0x14, 0xC9, 0x64, 0x90, 0xE5, 0xA9, 0x00, 0x8D, 0x07, 0x14, 0xF0, 0xDE, 0x20,
  0x0E, 0x43, 0x20, 0x10, 0x41, 0x20, 0x5B, 0x42, 0x20, 0x10, 0x41, 0x20, 0x71, 0x41, 0x20, 0x09,
  0x52, 0x20, 0x1B, 0x5A, 0x20, 0x06, 0x52, 0x20, 0x03, 0x52, 0x90, 0x0E, 0x20, 0xF1, 0x4E, 0xCE,
  0x09, 0x15, 0xD0, 0xBC, 0x20, 0x15, 0x62, 0x4C, 0x60, 0x40, 0x20, 0x1E, 0x5A, 0x20, 0x10, 0x41,
  0x20, 0x09, 0x5A, 0x20, 0x0C, 0x5A, 0x20, 0x0F, 0x5A, 0x20, 0x10, 0x41, 0x20, 0x03, 0x62, 0x20,
  0x06, 0x62, 0x20, 0x09, 0x62, 0x20, 0x10, 0x41, 0x20, 0x03, 0x70, 0x20, 0x06, 0x70, 0x20, 0x09,
  0x70, 0x20, 0x09, 0x70, 0x20, 0x10, 0x41, 0x20, 0x03, 0x69, 0x20, 0x06, 0x69, 0x20, 0x09, 0x69,
  0x4C, 0x90, 0x40, 0xAD, 0x17, 0x14, 0xC9, 0x01, 0xD0, 0x14, 0x20, 0xE3, 0x41, 0xB9, 0x00, 0x0A,
  0x30, 0x03, 0x8D, 0x06, 0x15, 0xB9, 0x80, 0x0A, 0x30, 0x03, 0x8D, 0x16, 0x14, 0x60, 0xAA, 0xD0,
  0x40, 0xAD, 0x62, 0xC0, 0x10, 0x1D, 0xAD, 0x35, 0x14, 0x18, 0x6D, 0x6B, 0x0C, 0x8D, 0x35, 0x14,
  0x90, 0x11, 0xAE, 0x36, 0x14, 0xBD, 0x5E, 0x0D, 0x69, 0x00, 0x29, 0x07, 0xAA, 0xBD, 0x1E, 0x0D,
  0x8D, 0x36, 0x14, 0xA9, 0x00, 0xAC, 0x61, 0xC0, 0x10, 0x11, 0xAD, 0x36, 0x14, 0xAE, 0x31, 0x14,
  0x30, 0x09, 0xAE, 0x06, 0x15, 0xF0, 0x04, 0x8E, 0x36, 0x14, 0x8A, 0x8D, 0x16, 0x14, 0x8C, 0x31,
  0x14, 0x4C, 0x6E, 0x4C, 0xAD, 0x17, 0x14, 0xC9, 0x03, 0xD0, 0x3C, 0x20, 0x85, 0x41, 0x20, 0xE3,
  0x41, 0xB9, 0x00, 0x0B, 0x8D, 0x16, 0x14, 0x60, 0xA9, 0x03, 0xAE, 0x08, 0x15, 0xD0, 0x02, 0x49,
  0x0F, 0x2D, 0x06, 0x15, 0x8D, 0x06, 0x15, 0x20, 0x1E, 0xFB, 0xA2, 0x00, 0xC0, 0xC0, 0x90, 0x01,
  0xE8, 0xC0, 0x40, 0xB0, 0x01, 0xCA, 0x8A, 0x29, 0x03, 0xAE, 0x08, 0x15, 0xF0, 0x02, 0x0A, 0x0A,
  0x0D, 0x06, 0x15, 0x8D, 0x06, 0x15, 0x60, 0xC9, 0x02, 0xD0, 0x21, 0xAE, 0x08, 0x15, 0x20, 0x1E,
  0xFB, 0x98, 0x4A, 0x4A, 0x4A, 0x4A, 0x4A, 0xA8, 0xB9, 0x1E, 0x0D, 0xAE, 0x08, 0x15, 0xD0, 0x05,
  0x8D, 0x06, 0x15, 0xF0, 0x03, 0x8D, 0x16, 0x14, 0x20, 0xE3, 0x41, 0x60, 0xAA, 0xD0, 0x06, 0x20,
  0x85, 0x41, 0x20, 0xE3, 0x41, 0x60, 0xAD, 0x00, 0xC0, 0x30, 0x03, 0x68, 0x68, 0x60, 0x20, 0x42,
  0x42, 0xC0, 0x1B, 0xD0, 0x07, 0xAD, 0x00, 0xC0, 0x10, 0xFB, 0x30, 0xEF, 0xC0, 0x13, 0xD0, 0x0A,
  0xAD, 0x6E, 0x4C, 0x49, 0xC5, 0x8D, 0x6E, 0x4C, 0xD0, 0xE1, 0xC0, 0x12, 0xD0, 0x2B, 0x20, 0x4E,
  0x42, 0xB0, 0xD8, 0x85, 0x07, 0xA2, 0x0A, 0x20, 0x4F, 0x4C, 0x86, 0x06, 0x20, 0x4E, 0x42, 0xD0,
  0x04, 0xA5, 0x07, 0x10, 0x05, 0xB0, 0xC4, 0x18, 0x65, 0x06, 0x38, 0xE9, 0x01, 0x90, 0xBC, 0x8D,
  0x07, 0x14, 0x68, 0x68, 0x68, 0x68, 0x4C, 0x8A, 0x40, 0xC9, 0x11, 0xD0, 0x07, 0x68, 0x68, 0x68,
  0x68, 0x4C, 0x60, 0x40, 0x60, 0xAD, 0x00, 0xC0, 0x10, 0xFB, 0x8D, 0x10, 0xC0, 0x29, 0x7F, 0xA8,
  0x60, 0x20, 0x42, 0x42, 0xC9, 0x0D, 0xF0, 0x05, 0x38, 0xE9, 0x30, 0xC9, 0x0A, 0x60, 0xAE, 0x11,
  0x14, 0xE8, 0xA0, 0x40, 0x88, 0xD0, 0xFD, 0x20, 0x6E, 0x4C, 0xCA, 0xD0, 0xF5, 0x60, 0xCE, 0x04,
  0x14, 0x30, 0x02, 0x18, 0x60, 0xA9, 0x0A, 0x8D, 0x04, 0x14, 0xAD, 0x02, 0x14, 0xAE, 0x03, 0x14,
  0x20, 0x00, 0x4C, 0x0A, 0x85, 0x00, 0xAD, 0x0E, 0x14, 0x6D, 0x0F, 0x14, 0x6D, 0x1B, 0x14, 0x6D,
  0x1D, 0x14, 0x0A, 0x65, 0x00, 0x85, 0x00, 0xAD, 0x09, 0x14, 0x6D, 0x10, 0x14, 0x6D, 0x18, 0x14,
  0x6D, 0x27, 0x14, 0x6D, 0x28, 0x14, 0x6D, 0x29, 0x14, 0x65, 0x00, 0x85, 0x00, 0xAD, 0x0A, 0x14,
  0x4A, 0x18, 0x65, 0x00, 0xAE, 0x1C, 0x14, 0xF0, 0x03, 0x6D, 0x18, 0x14, 0x85, 0x00, 0xA5, 0x00,
  0xAE, 0x57, 0x0C, 0x20, 0x4F, 0x4C, 0x86, 0x00, 0xAD, 0x01, 0x0C, 0x38, 0xE5, 0x00, 0xB0, 0x02,
  0xA9, 0x00, 0x8D, 0x11, 0x14, 0xAD, 0x03, 0x14, 0xCD, 0x0D, 0x0C, 0x90, 0x03, 0xCE, 0x03, 0x14,
  0xAD, 0x14, 0x0C, 0xAE, 0x0F, 0x14, 0xE8, 0x20, 0x00, 0x4C, 0x8D, 0x25, 0x14, 0xAD, 0x55, 0x0C,
  0xAE, 0x28, 0x14, 0xE8, 0x20, 0x00, 0x4C, 0x8D, 0x2B, 0x14, 0x18, 0xAD, 0x02, 0x14, 0x0D, 0x0F,
  0x14, 0x0D, 0x0E, 0x14, 0x0D, 0x18, 0x14, 0x0D, 0x27, 0x14, 0x0D, 0x28, 0x14, 0xD0, 0x01, 0x38,
  0x60, 0xA2, 0x00, 0xAD, 0x06, 0x14, 0xCD, 0x23, 0x14, 0x90, 0x07, 0xED, 0x22, 0x14, 0x8D, 0x06,
  0x14, 0xAA, 0xAD, 0x12, 0x14, 0x30, 0x06, 0xCE, 0x12, 0x14, 0xAE, 0x13, 0x14, 0xAD, 0x14, 0x14,
  0x30, 0x0D, 0xCE, 0x14, 0x14, 0xAD, 0x15, 0x14, 0x6D, 0x21, 0x0C, 0x8D, 0x15, 0x14, 0xAA, 0xAD,
  0x2E, 0x14, 0x30, 0x1A, 0xAD, 0x2F, 0x14, 0xAA, 0xED, 0x65, 0x0C, 0x8D, 0x2F, 0x14, 0xCD, 0x66,
  0x0C, 0xB0, 0x0B, 0xAD, 0x64, 0x0C, 0x8D, 0x2F, 0x14, 0xCE, 0x2E, 0x14, 0x10, 0xE6, 0xAD, 0x1E,
  0x14, 0xF0, 0x1E, 0xAD, 0x1F, 0x14, 0xED, 0x30, 0x0C, 0x8D, 0x1F, 0x14, 0xCD, 0x3F, 0x0C, 0xB0,
  0x0F, 0xCE, 0x1E, 0x14, 0xF0, 0x0B, 0xAD, 0x31, 0x0C, 0x38, 0xED, 0x2C, 0x0C, 0x8D, 0x1F, 0x14,
  0xAA, 0xAD, 0x19, 0x14, 0xF0, 0x0D, 0xCE, 0x19, 0x14, 0xAD, 0x1A, 0x14, 0x4D, 0x27, 0x0C, 0x8D,
  0x1A, 0x14, 0xAA, 0xAD, 0x0B, 0x14, 0x10, 0x06, 0x69, 0x06, 0x8D, 0x0B, 0x14, 0xAA, 0xAD, 0x0C,
  0x14, 0xC9, 0xC0, 0x90, 0x06, 0xE9, 0x02, 0x8D, 0x0C, 0x14, 0xAA, 0xAD, 0x1C, 0x14, 0xF0, 0x03,
  0x49, 0xFF, 0xAA, 0xAC, 0x30, 0x14, 0x30, 0x06, 0xBE, 0x46, 0x0D, 0xCE, 0x30, 0x14, 0x86, 0xCA,
  0x4C, 0x6E, 0x4C, 0x20, 0x42, 0x4F, 0xA2, 0x00, 0xBC, 0x80, 0x0C, 0x30, 0x1B, 0xE8, 0xBD, 0x80,
  0x0C, 0xE8, 0xC9, 0x80, 0xB0, 0xF2, 0xE9, 0x00, 0xCD, 0x07, 0x14, 0xF0, 0x02, 0xB0, 0x06, 0xBD,
  0x80, 0x0C, 0x99, 0x00, 0x0C, 0xE8, 0x10, 0xE6, 0xA9, 0x00, 0x8D, 0x00, 0x14, 0x85, 0xCA, 0x8D,
  0x06, 0x14, 0x8D, 0x01, 0x14, 0x8D, 0x16, 0x14, 0x8D, 0x04, 0x14, 0x8D, 0x35, 0x14, 0xA9, 0x0C,
  0x8D, 0x36, 0x14, 0xA9, 0xFF, 0x8D, 0x12, 0x14, 0x8D, 0x30, 0x14, 0x20, 0x00, 0x52, 0x20, 0x00,
  0x5A, 0x20, 0x00, 0x70, 0xAD, 0x02, 0x0C, 0xCD, 0x21, 0x14, 0x90, 0x05, 0x69, 0x00, 0x8D, 0x21,
  0x14, 0x20, 0x03, 0x5A, 0x20, 0x06, 0x5A, 0x20, 0x00, 0x62, 0x4C, 0x00, 0x69, 0xAC, 0x07, 0x14,
  0xB9, 0x00, 0x0E, 0x8D, 0x02, 0x14, 0xB9, 0x80, 0x0E, 0x8D, 0x09, 0x14, 0xB9, 0x80, 0x0F, 0x85,
  0x01, 0xB9, 0x00, 0x10, 0x85, 0x02, 0xBE, 0x00, 0x0F, 0xA0, 0x00, 0x98, 0xCA, 0x30, 0x06, 0x99,
  0x90, 0x19, 0xC8, 0x10, 0xF7, 0xA6, 0x01, 0xA9, 0x01, 0xCA, 0x30, 0x06, 0x99, 0x90, 0x19, 0xC8,
  0x10, 0xF7, 0xA6, 0x02, 0xA9, 0x02, 0xCA, 0x30, 0x06, 0x99, 0x90, 0x19, 0xC8, 0x10, 0xF7, 0x8C,
  0x0A, 0x14, 0xAC, 0x07, 0x14, 0xB9, 0x80, 0x10, 0x8D, 0x0F, 0x14, 0xA9, 0x00, 0x8D, 0x0E, 0x14,
  0xB9, 0x00, 0x11, 0x8D, 0x18, 0x14, 0xB9, 0x80, 0x11, 0x8D, 0x27, 0x14, 0xA2, 0x00, 0xAD, 0x51,
  0x0C, 0x9D, 0x40, 0x1D, 0xE8, 0xEC, 0x27, 0x14, 0x90, 0xF7, 0xA9, 0x00, 0x8D, 0x28, 0x14, 0x60,
  0x85, 0x07, 0x84, 0x09, 0xA0, 0x00, 0x84, 0x06, 0x84, 0x08, 0x68, 0x85, 0x01, 0x68, 0x85, 0x00,
  0xB1, 0x06, 0x91, 0x08, 0xC8, 0xD0, 0xF9, 0xE6, 0x07, 0xE6, 0x09, 0xCA, 0xD0, 0xF2, 0xA5, 0x00,
  0x48, 0xA5, 0x01, 0x48, 0x60, 0x20, 0x6B, 0x42, 0x20, 0x5B, 0x42, 0x20, 0x09, 0x52, 0x20, 0x1B,
  0x5A, 0x20, 0x06, 0x52, 0x20, 0x03, 0x52, 0x20, 0x1E, 0x5A, 0x20, 0x09, 0x5A, 0x20, 0x0C, 0x5A,
  0x20, 0x0F, 0x5A, 0x20, 0x03, 0x62, 0x20, 0x06, 0x62, 0x20, 0x09, 0x62, 0x20, 0x03, 0x70, 0x20,
  0x06, 0x70, 0x20, 0x09, 0x70, 0x20, 0x09, 0x70, 0x20, 0x03, 0x69, 0x20, 0x06, 0x69, 0x20, 0x09,
  0x69, 0x20, 0x52, 0x01, 0x4C, 0x3D, 0x45, 0x8D, 0x10, 0xC0, 0xAD, 0x6E, 0x4C, 0x85, 0x2F, 0xA9,
  0x60, 0x8D, 0x6E, 0x4C, 0x8D, 0xD0, 0x4D, 0x20, 0x22, 0x45, 0xA5, 0x2F, 0x8D, 0x6E, 0x4C, 0xA9,
  0x86, 0x8D, 0xD0, 0x4D, 0x60, 0x20, 0x5E, 0x45, 0x20, 0x2A, 0x46, 0x20, 0x28, 0x47, 0x20, 0x7F,
  0x46, 0x20, 0xBC, 0x47, 0x20, 0x88, 0x48, 0x20, 0x52, 0x4A, 0x20, 0x3E, 0x48, 0x4C, 0x22, 0x45,
  0xAC, 0x00, 0xC0, 0x10, 0x1B, 0x8C, 0x10, 0xC0, 0x68, 0x68, 0xC0, 0x8D, 0xD0, 0x01, 0x60, 0x68,
  0x68, 0xC0, 0xA0, 0xD0, 0x01, 0x60, 0xC0, 0x9B, 0xD0, 0x03, 0x4C, 0x37, 0x45, 0x4C, 0x22, 0x45,
  0x60, 0x20, 0x42, 0x4F, 0xAD, 0x32, 0x14, 0x0D, 0x33, 0x14, 0xF0, 0x03, 0x20, 0x18, 0x62, 0x20,
  0xB8, 0x51, 0x0D, 0x0C, 0x12, 0x41, 0x54, 0x41, 0x52, 0x49, 0x20, 0x50, 0x52, 0x45, 0x53, 0x45,
  0x4E, 0x54, 0x53, 0x3A, 0x00, 0xA9, 0x03, 0x8D, 0x5A, 0x0C, 0xA9, 0x28, 0x8D, 0x5B, 0x0C, 0xA2,
  0x00, 0x20, 0x81, 0x50, 0xA9, 0xFF, 0x85, 0x0A, 0x20, 0xC5, 0x50, 0x20, 0x3D, 0x45, 0xA5, 0x0A,
  0xF0, 0x0D, 0x38, 0xE9, 0x06, 0x85, 0x0A, 0xB0, 0xEF, 0xA9, 0x00, 0x85, 0x0A, 0xF0, 0xE9, 0xA9,
  0x0F, 0x8D, 0x5A, 0x0C, 0xA9, 0x60, 0x8D, 0x5B, 0x0C, 0xA2, 0x01, 0x20, 0x81, 0x50, 0xA9, 0xFF,
  0xA0, 0x00, 0x99, 0x00, 0x07, 0xC8, 0xD0, 0xFA, 0x20, 0xCA, 0x50, 0xA9, 0x03, 0x8D, 0x5A, 0x0C,
  0xA9, 0x28, 0x8D, 0x5B, 0x0C, 0xA2, 0x00, 0x20, 0x81, 0x50, 0x20, 0x55, 0x01, 0xA2, 0x05, 0xA9,
  0x00, 0x20, 0xA8, 0xFC, 0xCA, 0xD0, 0xF8, 0xA9, 0x80, 0x85, 0xFA, 0xA9, 0x00, 0x85, 0x0A, 0xA6,
  0x0A, 0xBD, 0x1E, 0x46, 0x85, 0x00, 0xBC, 0x1F, 0x46, 0xA2, 0x00, 0xA5, 0x00, 0x9D, 0x00, 0x07,
  0x98, 0x9D, 0x01, 0x07, 0xE8, 0xE8, 0xD0, 0xF3, 0x20, 0xCA, 0x50, 0xE6, 0x0A, 0xE6, 0x0A, 0xA5,
  0x0A, 0xC9, 0x0C, 0xD0, 0x04, 0xA9, 0x00, 0x85, 0x0A, 0x20, 0x3D, 0x45, 0xC6, 0xFA, 0xD0, 0xCF,
  0x60, 0x55, 0x2A, 0x2A, 0x55, 0x7F, 0x7F, 0xD5, 0xAA, 0xAA, 0xD5, 0xFF, 0xFF, 0x20, 0x66, 0x46,
  0x20, 0x40, 0x01, 0x20, 0x6B, 0x42, 0x20, 0x5B, 0x42, 0x20, 0x3D, 0x45, 0x20, 0x52, 0x01, 0xA0,
  0x00, 0xB1, 0x28, 0xD0, 0xEE, 0x60, 0xA9, 0x00, 0x8D, 0x02, 0x14, 0x8D, 0x09, 0x14, 0x8D, 0x0A,
  0x14, 0x8D, 0x0E, 0x14, 0x8D, 0x0F, 0x14, 0x8D, 0x18, 0x14, 0x8D, 0x27, 0x14, 0x8D, 0x28, 0x14,
  0x20, 0xC0, 0x43, 0xA9, 0x00, 0x8D, 0x21, 0x14, 0x60, 0x20, 0x43, 0x46, 0x20, 0xB8, 0x51, 0x0D,
  0x0D, 0x00, 0x52, 0x4F, 0x42, 0x4F, 0x54, 0x52, 0x4F, 0x4E, 0x3A, 0x20, 0x32, 0x30, 0x38, 0x34,
  0x00, 0x60, 0x20, 0x66, 0x46, 0x20, 0x43, 0x01, 0xA0, 0x90, 0xA2, 0x06, 0x20, 0x31, 0x48, 0xA0,
  0x14, 0x8C, 0x02, 0x14, 0x8C, 0x08, 0x14, 0xAD, 0x0C, 0x0C, 0x8D, 0x03, 0x14, 0x88, 0x30, 0x28,
  0xA9, 0x56, 0x20, 0x4B, 0x4C, 0x69, 0x60, 0x99, 0xF0, 0x15, 0xA9, 0x10, 0x20, 0x4B, 0x4C, 0x18,
  0x69, 0xE8, 0x29, 0xFE, 0x99, 0x70, 0x15, 0x98, 0xA2, 0x08, 0x20, 0x4F, 0x4C, 0x8A, 0x99, 0xF0,
  0x16, 0xA9, 0x00, 0x99, 0x70, 0x16, 0xF0, 0xD5, 0xA2, 0xDC, 0xA9, 0x46, 0x20, 0x24, 0x48, 0x20,
  0x06, 0x47, 0x20, 0xC2, 0x44, 0xA9, 0x68, 0xA2, 0xC0, 0x20, 0xE9, 0x46, 0x4C, 0xCC, 0x46, 0x38,
  0x01, 0x30, 0x00, 0x34, 0x81, 0x20, 0x01, 0x0C, 0x80, 0x10, 0x04, 0x00, 0x85, 0x06, 0x86, 0x07,
  0xAC, 0x05, 0x14, 0x88, 0x30, 0x12, 0xB9, 0x70, 0x17, 0xC5, 0x06, 0x90, 0x04, 0xC5, 0x07, 0x90,
  0xF2, 0xA9, 0x01, 0x99, 0xB0, 0x18, 0x10, 0xEB, 0x60, 0xC6, 0xFA, 0xD0, 0x1D, 0xA4, 0xFB, 0xB1,
  0x22, 0xD0, 0x03, 0x68, 0x68, 0x60, 0x85, 0xFA, 0xC8, 0xB1, 0x22, 0x10, 0x07, 0x29, 0x0F, 0x8D,
  0x16, 0x14, 0x10, 0x03, 0x8D, 0x06, 0x15, 0xC8, 0x84, 0xFB, 0x60, 0x20, 0x66, 0x46, 0x20, 0x46,
  0x01, 0xA0, 0xA0, 0xA2, 0x78, 0x20, 0x31, 0x48, 0xA9, 0x6D, 0x85, 0x20, 0xA9, 0x47, 0x85, 0x21,
  0x20, 0x82, 0x47, 0xA2, 0x50, 0xA9, 0x47, 0x20, 0x24, 0x48, 0x20, 0x06, 0x47, 0x20, 0xC2, 0x44,
  0x4C, 0x47, 0x47, 0x20, 0x00, 0x20, 0x03, 0x06, 0x07, 0x0C, 0x0D, 0x2F, 0x01, 0x0C, 0x05, 0x17,
  0x03, 0x06, 0x0F, 0x0C, 0x00, 0x10, 0x81, 0x10, 0x83, 0x10, 0x81, 0x10, 0x83, 0x20, 0x80, 0x00,
  0x10, 0xB0, 0x00, 0x01, 0x60, 0x90, 0x01, 0x00, 0x7C, 0x90, 0x00, 0x02, 0xE0, 0xA0, 0x01, 0x01,
  0xF0, 0xB0, 0x01, 0x02, 0x00, 0xA2, 0x00, 0xA0, 0x00, 0xA9, 0x0A, 0x8D, 0x0D, 0x14, 0xB1, 0x20,
  0xD0, 0x04, 0x8E, 0x0A, 0x14, 0x60, 0x9D, 0x50, 0x19, 0xC8, 0xB1, 0x20, 0x9D, 0x60, 0x19, 0xC8,
  0xB1, 0x20, 0x9D, 0x70, 0x19, 0xC8, 0xB1, 0x20, 0x9D, 0x90, 0x19, 0xC8, 0xA9, 0x00, 0x9D, 0x80,
  0x19, 0x8A, 0x29, 0x03, 0x9D, 0xA0, 0x19, 0xA9, 0x7F, 0x9D, 0xB0, 0x19, 0xE8, 0x10, 0xCF, 0x20,
  0x66, 0x46, 0x20, 0x49, 0x01, 0xA2, 0xF0, 0xA0, 0xB0, 0x20, 0x31, 0x48, 0xA9, 0x17, 0x85, 0x20,
  0xA9, 0x48, 0x85, 0x21, 0x20, 0x82, 0x47, 0xA2, 0xFE, 0xA9, 0x47, 0x20, 0x24, 0x48, 0xA9, 0x09,
  0x8D, 0x00, 0x19, 0xA9, 0x90, 0x8D, 0x10, 0x19, 0xA9, 0x00, 0x8D, 0x20, 0x19, 0x8D, 0x30, 0x19,
  0xA9, 0x7F, 0x8D, 0x40, 0x19, 0xEE, 0x09, 0x14, 0x20, 0x06, 0x47, 0x20, 0xC2, 0x44, 0x4C, 0xF5,
  0x47, 0x58, 0x00, 0x10, 0x0F, 0x01, 0x00, 0x70, 0x83, 0x08, 0x03, 0x01, 0x04, 0x09, 0x80, 0x10,
  0x03, 0x0A, 0x0C, 0x01, 0x00, 0x10, 0x81, 0x20, 0x80, 0x00, 0x30, 0x90, 0x00, 0x01, 0x50, 0x90,
  0x00, 0x00, 0x70, 0x90, 0x00, 0x02, 0x00, 0x86, 0x22, 0x85, 0x23, 0xA9, 0x01, 0x85, 0xFA, 0xA9,
  0x00, 0x85, 0xFB, 0x60, 0x8E, 0x00, 0x15, 0x8E, 0x02, 0x15, 0x8C, 0x01, 0x15, 0x8C, 0x03, 0x15,
  0x60, 0x20, 0x43, 0x46, 0x20, 0x30, 0x49, 0xA2, 0x08, 0xA0, 0x08, 0x20, 0x31, 0x48, 0xA2, 0x5F,
  0xA9, 0x48, 0x20, 0x24, 0x48, 0x20, 0x06, 0x47, 0xA9, 0x7F, 0x85, 0x2A, 0x20, 0xC2, 0x44, 0x4C,
  0x52, 0x48, 0x19, 0x01, 0x19, 0x04, 0x1A, 0x01, 0x12, 0x0C, 0x09, 0x01, 0x08, 0x00, 0x10, 0x81,
  0x10, 0x83, 0x10, 0x81, 0x10, 0x83, 0x02, 0x80, 0x0A, 0x01, 0x12, 0x04, 0x1C, 0x01, 0x18, 0x0C,
  0x10, 0x01, 0x18, 0x04, 0x72, 0x03, 0x19, 0x0C, 0x20, 0x00, 0x00, 0x20, 0x66, 0x46, 0x20, 0x4C,
  0x01, 0xA2, 0xF0, 0xA0, 0xA0, 0x20, 0x31, 0x48, 0xA9, 0x2B, 0x85, 0x20, 0xA9, 0x49, 0x85, 0x21,
  0x20, 0x82, 0x47, 0xA2, 0x10, 0xA9, 0x49, 0x20, 0x24, 0x48, 0xA9, 0x08, 0x8D, 0x20, 0x1B, 0xA9,
  0xA0, 0x8D, 0x30, 0x1B, 0xA9, 0x00, 0x8D, 0x40, 0x1B, 0x8D, 0x60, 0x1B, 0x8D, 0x70, 0x1B, 0xA9,
  0x7F, 0x8D, 0x80, 0x1B, 0xA9, 0xFF, 0x8D, 0x90, 0x1B, 0xEE, 0x18, 0x14, 0x20, 0x06, 0x47, 0x20,
  0xC2, 0x44, 0xA9, 0x80, 0xA2, 0xC0, 0x20, 0xE9, 0x46, 0xAD, 0x1D, 0x14, 0xD0, 0x32, 0xA5, 0xFB,
  0xC9, 0x0A, 0xD0, 0x2C, 0xA2, 0x01, 0xAD, 0x20, 0x1B, 0x18, 0x69, 0x06, 0x9D, 0x70, 0x1C, 0xAD,
  0x30, 0x1B, 0x9D, 0x80, 0x1C, 0xA9, 0x02, 0x9D, 0x90, 0x1C, 0xA9, 0x00, 0x9D, 0xA0, 0x1C, 0xA9,
  0x7F, 0x9D, 0xB0, 0x1C, 0xCA, 0xF0, 0xDF, 0xAD, 0x2E, 0x0C, 0x8D, 0xC1, 0x1C, 0xEE, 0x1D, 0x14,
  0x4C, 0xC9, 0x48, 0x80, 0x00, 0x60, 0x00, 0x01, 0x83, 0x20, 0x80, 0x10, 0x00, 0x02, 0x0F, 0x05,
  0x00, 0x01, 0x83, 0x10, 0x80, 0x2A, 0x03, 0x01, 0x83, 0x0B, 0x80, 0x20, 0x00, 0x00, 0x22, 0xA0,
  0x00, 0x01, 0x00, 0x20, 0xB8, 0x51, 0x0D, 0x0F, 0x00, 0x4B, 0x45, 0x59, 0x42, 0x4F, 0x41, 0x52,
  0x44, 0x0D, 0x0F, 0x08, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x0D, 0x0A, 0x10, 0x4D,
  0x4F, 0x56, 0x45, 0x3A, 0x0D, 0x17, 0x10, 0x53, 0x48, 0x4F, 0x4F, 0x54, 0x3A, 0x0D, 0x0A, 0x1F,
  0x51, 0x20, 0x57, 0x20, 0x45, 0x0D, 0x17, 0x1F, 0x49, 0x20, 0x4F, 0x20, 0x50, 0x0D, 0x0A, 0x28,
  0x41, 0x20, 0x20, 0x20, 0x44, 0x0D, 0x17, 0x28, 0x4B, 0x20, 0x20, 0x20, 0x3B, 0x0D, 0x0A, 0x31,
  0x5A, 0x20, 0x58, 0x20, 0x43, 0x0D, 0x17, 0x31, 0x2C, 0x20, 0x2E, 0x20, 0x2F, 0x0D, 0x0F, 0x48,
  0x4A, 0x4F, 0x59, 0x53, 0x54, 0x49, 0x43, 0x4B, 0x0D, 0x0F, 0x50, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D,
  0x3D, 0x3D, 0x3D, 0x0D, 0x01, 0x58, 0x4D, 0x4F, 0x56, 0x45, 0x3A, 0x4A, 0x4F, 0x59, 0x53, 0x54,
  0x49, 0x43, 0x4B, 0x20, 0x20, 0x20, 0x53, 0x48, 0x4F, 0x4F, 0x54, 0x3A, 0x42, 0x55, 0x54, 0x54,
  0x4F, 0x4E, 0x53, 0x20, 0x30, 0x20, 0x41, 0x4E, 0x44, 0x20, 0x31, 0x0D, 0x0F, 0x70, 0x50, 0x41,
  0x44, 0x44, 0x4C, 0x45, 0x53, 0x0D, 0x0F, 0x78, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x0D,
  0x04, 0x80, 0x4D, 0x4F, 0x56, 0x45, 0x3A, 0x50, 0x41, 0x44, 0x44, 0x4C, 0x45, 0x20, 0x30, 0x20,
  0x20, 0x20, 0x53, 0x48, 0x4F, 0x4F, 0x54, 0x3A, 0x50, 0x41, 0x44, 0x44, 0x4C, 0x45, 0x20, 0x31,
  0x0D, 0x09, 0x98, 0x4A, 0x4F, 0x59, 0x53, 0x54, 0x49, 0x43, 0x4B, 0x20, 0x2B, 0x20, 0x4B, 0x45,
  0x59, 0x42, 0x4F, 0x41, 0x52, 0x44, 0x0D, 0x09, 0xA0, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D,
  0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x3D, 0x0D, 0x01, 0xA8, 0x4D,
  0x4F, 0x56, 0x45, 0x3A, 0x4A, 0x4F, 0x59, 0x53, 0x54, 0x49, 0x43, 0x4B, 0x20, 0x20, 0x20, 0x53,
  0x48, 0x4F, 0x4F, 0x54, 0x3A, 0x57, 0x48, 0x4F, 0x4C, 0x45, 0x20, 0x4B, 0x45, 0x59, 0x42, 0x4F,
  0x41, 0x52, 0x44, 0x00, 0x60, 0x20, 0x66, 0x46, 0x20, 0x4F, 0x01, 0xA2, 0x08, 0xA0, 0x08, 0x20,
  0x31, 0x48, 0xA2, 0x42, 0xA9, 0x4B, 0x20, 0x24, 0x48, 0xA9, 0xF1, 0x8D, 0xC0, 0x19, 0xA9, 0xF0,
  0x8D, 0xE0, 0x1C, 0xA9, 0x90, 0x8D, 0xD0, 0x19, 0xA9, 0xA7, 0x8D, 0xF0, 0x1C, 0xA9, 0x00, 0x8D,
  0xE0, 0x19, 0x8D, 0xF0, 0x19, 0x8D, 0x00, 0x1D, 0x8D, 0x10, 0x1D, 0x8D, 0x00, 0x1A, 0x8D, 0x20,
  0x1D, 0x8D, 0x40, 0x1A, 0x8D, 0x60, 0x1D, 0x8D, 0xC1, 0x19, 0x8D, 0xE1, 0x1C, 0xA9, 0x7F, 0x8D,
  0x20, 0x1A, 0x8D, 0x50, 0x1D, 0x8D, 0x30, 0x1D, 0xEE, 0x0E, 0x14, 0xEE, 0x27, 0x14, 0x20, 0x06,
  0x47, 0xAD, 0x30, 0x1D, 0xC9, 0x10, 0xB0, 0x05, 0xA9, 0x7F, 0x8D, 0x30, 0x1D, 0xA9, 0x7F, 0x8D,
  0x10, 0x1A, 0x8D, 0x24, 0x14, 0x8D, 0x2A, 0x14, 0xA5, 0xFB, 0xC9, 0x06, 0xD0, 0x05, 0xA9, 0xFC,
  0x8D, 0xE0, 0x19, 0xAD, 0x0F, 0x14, 0xD0, 0x23, 0xAD, 0xC0, 0x19, 0xC9, 0xC1, 0xD0, 0x1C, 0x18,
  0x69, 0x08, 0x8D, 0x50, 0x1A, 0xAD, 0xD0, 0x19, 0x8D, 0x60, 0x1A, 0xA9, 0x00, 0x8D, 0x70, 0x1A,
  0x8D, 0x90, 0x1A, 0xA9, 0xFE, 0x8D, 0x80, 0x1A, 0xEE, 0x0F, 0x14, 0xA5, 0xFB, 0xC9, 0x18, 0xD0,
  0x05, 0xA9, 0xFC, 0x8D, 0x00, 0x1D, 0xAD, 0x28, 0x14, 0xD0, 0x2D, 0xAD, 0xE0, 0x1C, 0xC9, 0xC0,
  0xD0, 0x26, 0x18, 0x69, 0x08, 0x09, 0x01, 0x8D, 0x70, 0x1D, 0xAD, 0xF0, 0x1C, 0x8D, 0x90, 0x1D,
  0xA9, 0x00, 0x8D, 0xF0, 0x1D, 0x8D, 0xD0, 0x1D, 0x8D, 0x30, 0x1E, 0xA9, 0xFE, 0x8D, 0xB0, 0x1D,
  0xA9, 0x7F, 0x8D, 0x10, 0x1E, 0xEE, 0x28, 0x14, 0x20, 0xC2, 0x44, 0xA9, 0x80, 0xA2, 0xB0, 0x20,
  0xE9, 0x46, 0x4C, 0xAB, 0x4A, 0x44, 0x04, 0x01, 0x00, 0x50, 0x00, 0x04, 0x01, 0x01, 0x81, 0x0F,
  0x80, 0x01, 0x81, 0x0B, 0x80, 0x20, 0x03, 0x0C, 0x04, 0x01, 0x00, 0x50, 0x00, 0x04, 0x01, 0x01,
  0x81, 0x1F, 0x80, 0x01, 0x81, 0x14, 0x80, 0x20, 0x00, 0x00, 0x20, 0x25, 0x4F, 0x20, 0xB8, 0x51,
  0x0D, 0x09, 0x28, 0x43, 0x48, 0x4F, 0x4F, 0x53, 0x45, 0x20, 0x43, 0x4F, 0x4E, 0x54, 0x52, 0x4F,
  0x4C, 0x53, 0x3A, 0x0D, 0x09, 0x40, 0x31, 0x29, 0x20, 0x4A, 0x4F, 0x59, 0x53, 0x54, 0x49, 0x43,
  0x4B, 0x0D, 0x09, 0x50, 0x32, 0x29, 0x20, 0x4B, 0x45, 0x59, 0x42, 0x4F, 0x41, 0x52, 0x44, 0x0D,
  0x09, 0x60, 0x33, 0x29, 0x20, 0x50, 0x41, 0x44, 0x44, 0x4C, 0x45, 0x53, 0x0D, 0x09, 0x70, 0x34,
  0x29, 0x20, 0x4A, 0x4F, 0x59, 0x53, 0x54, 0x49, 0x43, 0x4B, 0x20, 0x41, 0x4E, 0x44, 0x20, 0x4B,
  0x45, 0x59, 0x42, 0x4F, 0x41, 0x52, 0x44, 0x0D, 0x09, 0x90, 0x57, 0x48, 0x49, 0x43, 0x48, 0x3F,
  0x00, 0x20, 0x42, 0x42, 0xC9, 0x20, 0xF0, 0x19, 0xA2, 0x00, 0xDD, 0xEF, 0x4B, 0xF0, 0x0F, 0xDD,
  0xF3, 0x4B, 0xF0, 0x0A, 0xE8, 0xE0, 0x04, 0x90, 0xF1, 0x68, 0x68, 0x4C, 0x60, 0x40, 0x8E, 0x17,
  0x14, 0x60, 0x31, 0x32, 0x33, 0x34, 0x4A, 0x4B, 0x50, 0x26, 0x02, 0xBB, 0x5A, 0x30, 0x5F, 0xEE,
  0x3D, 0xA8, 0xFF, 0x85, 0xE0, 0x86, 0xE1, 0xA9, 0x00, 0x85, 0xE2, 0xA2, 0x08, 0x06, 0xE0, 0x2A,
  0xC5, 0xE1, 0x90, 0x02, 0xE5, 0xE1, 0x26, 0xE2, 0xCA, 0xD0, 0xF2, 0xAA, 0xA5, 0xE2, 0x60, 0x85,
  0xE0, 0xA9, 0x00, 0x85, 0xE2, 0xA2, 0x08, 0x06, 0xE0, 0x2A, 0xC9, 0x07, 0x90, 0x02, 0xE9, 0x07,
  0x26, 0xE2, 0xCA, 0xD0, 0xF2, 0xAA, 0xA5, 0xE2, 0x60, 0xA5, 0x4F, 0x0A, 0x65, 0xFC, 0x45, 0x4E,
  0x48, 0xE6, 0x4E, 0xA5, 0x4E, 0x4D, 0x0A, 0x15, 0x85, 0x4F, 0x68, 0x85, 0x4E, 0x60, 0xAA, 0x20,
  0x36, 0x4C, 0x85, 0xE0, 0x86, 0xE1, 0xA9, 0x00, 0x85, 0xE2, 0xA2, 0x08, 0x0A, 0x26, 0xE2, 0x06,
  0xE0, 0x90, 0x07, 0x18, 0x65, 0xE1, 0x90, 0x02, 0xE6, 0xE2, 0xCA, 0xD0, 0xEF, 0xAA, 0xA5, 0xE2,
  0x60, 0xA5, 0xCA, 0xF0, 0x09, 0x65, 0xCB, 0x85, 0xCB, 0x90, 0x03, 0xAD, 0x30, 0xC0, 0x60, 0x84,
  0xFC, 0x84, 0xFE, 0x86, 0x05, 0xA9, 0x00, 0x85, 0x04, 0xA2, 0x08, 0x06, 0x05, 0x2A, 0xC9, 0x07,
  0x90, 0x02, 0xE9, 0x07, 0x26, 0x04, 0xCA, 0xD0, 0xF2, 0x69, 0x01, 0x0A, 0xA8, 0xB1, 0x06, 0x85,
  0x08, 0xC8, 0xB1, 0x06, 0x85, 0x09, 0xA0, 0x01, 0xB1, 0x06, 0xA8, 0x88, 0xA2, 0x00, 0x60, 0x20,
  0x7C, 0x4C, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05, 0xA4, 0x04, 0xA1, 0x08,
  0x91, 0x06, 0xE6, 0x08, 0xC8, 0xA1, 0x08, 0x91, 0x06, 0xE6, 0x08, 0xA4, 0x05, 0x88, 0x10, 0xE2,
  0x4C, 0x6E, 0x4C, 0x86, 0xFC, 0x86, 0xFE, 0x20, 0x1C, 0x4C, 0x85, 0x04, 0x88, 0xA2, 0x00, 0xB1,
  0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05, 0xA4, 0x04, 0x8A, 0x91, 0x06, 0xC8, 0x91,
  0x06, 0xA4, 0x05, 0x88, 0x10, 0xE9, 0x4C, 0x6E, 0x4C, 0x20, 0x7C, 0x4C, 0xB1, 0xFC, 0x85, 0x06,
  0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05, 0xA4, 0x04, 0xA1, 0x08, 0x91, 0x06, 0xE6, 0x08, 0xC8, 0xA1,
  0x08, 0x91, 0x06, 0xE6, 0x08, 0xC8, 0xA1, 0x08, 0x91, 0x06, 0xE6, 0x08, 0xA4, 0x05, 0x88, 0x10,
  0xDB, 0x4C, 0x6E, 0x4C, 0x86, 0xFC, 0x86, 0xFE, 0x20, 0x1C, 0x4C, 0x85, 0x04, 0x88, 0xA2, 0x00,
  0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05, 0xA4, 0x04, 0x8A, 0x91, 0x06, 0xC8,
  0x91, 0x06, 0xC8, 0x91, 0x06, 0xA4, 0x05, 0x88, 0x10, 0xE6, 0x4C, 0x6E, 0x4C, 0xA2, 0x0B, 0x20,
  0x36, 0x4C, 0x9D, 0x00, 0x07, 0x20, 0x36, 0x4C, 0x9D, 0x80, 0x07, 0xCA, 0xD0, 0xF1, 0x86, 0x00,
  0x86, 0xFC, 0x86, 0xFE, 0x8E, 0x00, 0x07, 0x8E, 0x80, 0x07, 0xA5, 0x00, 0x85, 0x01, 0x18, 0x69,
  0x60, 0xA2, 0xFF, 0x20, 0x4F, 0x4C, 0x85, 0xCA, 0xA5, 0x01, 0x38, 0xE5, 0x00, 0x4A, 0x4A, 0x4A,
  0xAA, 0xA9, 0x60, 0x18, 0x65, 0x01, 0x20, 0xA2, 0x4D, 0xA9, 0x5F, 0x38, 0xE5, 0x01, 0x20, 0xA2,
  0x4D, 0xA5, 0x01, 0x18, 0x69, 0x08, 0x85, 0x01, 0xC9, 0x60, 0x90, 0xDC, 0xE6, 0x00, 0xA5, 0x00,
  0xC9, 0x60, 0x90, 0xC6, 0x60, 0xA8, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0xA0, 0x26,
  0xBD, 0x00, 0x07, 0x91, 0x06, 0x88, 0x88, 0x10, 0xFA, 0x20, 0x6E, 0x4C, 0xA0, 0x27, 0xBD, 0x80,
  0x07, 0x91, 0x06, 0x88, 0x88, 0x10, 0xFA, 0x20, 0x6E, 0x4C, 0xA0, 0xFF, 0x88, 0xD0, 0xFD, 0x20,
  0x6E, 0x4C, 0x60, 0x86, 0x06, 0x85, 0x07, 0xAD, 0x0A, 0x15, 0x18, 0x65, 0x06, 0x8D, 0x0A, 0x15,
  0xAD, 0x0B, 0x15, 0x65, 0x07, 0x8D, 0x0B, 0x15, 0xAD, 0x0C, 0x15, 0x69, 0x00, 0x8D, 0x0C, 0x15,
  0xAD, 0x0D, 0x15, 0x38, 0xE5, 0x06, 0x8D, 0x0D, 0x15, 0xAD, 0x0E, 0x15, 0xE5, 0x07, 0x8D, 0x0E,
  0x15, 0x10, 0x1F, 0xEE, 0x09, 0x15, 0x20, 0x4F, 0x4E, 0xAD, 0x0D, 0x15, 0x18, 0x6D, 0x10, 0x0C,
  0x8D, 0x0D, 0x15, 0xAD, 0x0E, 0x15, 0x6D, 0x11, 0x0C, 0x8D, 0x0E, 0x15, 0xAD, 0x69, 0x0C, 0x8D,
  0x30, 0x14, 0xA9, 0x38, 0x85, 0xFC, 0xAD, 0x0A, 0x15, 0x85, 0x00, 0xAD, 0x0B, 0x15, 0x85, 0x01,
  0xAD, 0x0C, 0x15, 0x85, 0x02, 0x20, 0xC4, 0x4E, 0x18, 0x69, 0xB0, 0xA2, 0x27, 0xA4, 0xFC, 0x20,
  0x08, 0x51, 0xA5, 0xFC, 0x38, 0xE9, 0x08, 0x85, 0xFC, 0xA5, 0x00, 0x05, 0x01, 0x05, 0x02, 0xD0,
  0xE4, 0x60, 0xAD, 0x00, 0x08, 0x85, 0x06, 0xAD, 0x20, 0x08, 0x85, 0x07, 0xA0, 0x02, 0xB1, 0x06,
  0x85, 0x0A, 0xC8, 0xB1, 0x06, 0x85, 0x0B, 0xAE, 0x09, 0x15, 0xCA, 0x86, 0x04, 0xA9, 0x60, 0x85,
  0xFC, 0x85, 0xFE, 0xC6, 0x04, 0x30, 0x33, 0xA5, 0x0A, 0x85, 0x08, 0xA5, 0x0B, 0x85, 0x09, 0xA0,
  0x09, 0xA2, 0x00, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05, 0xA0, 0x27, 0xA1,
  0x08, 0x91, 0x06, 0xE6, 0x08, 0xE6, 0x08, 0xA4, 0x05, 0x88, 0x10, 0xE7, 0xA5, 0xFC, 0x18, 0x69,
  0x0B, 0x85, 0xFC, 0x85, 0xFE, 0xC9, 0xA5, 0x90, 0xCA, 0x60, 0xA5, 0xFC, 0xC9, 0xA5, 0xB0, 0xF9,
  0xA0, 0x09, 0xA2, 0x00, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0xA0, 0x27, 0x8A, 0xB1,
  0x06, 0xA4, 0x05, 0x88, 0x10, 0xEE, 0x60, 0xA9, 0x00, 0x85, 0x03, 0x85, 0x04, 0x85, 0x05, 0xA2,
  0x18, 0x06, 0x00, 0x26, 0x01, 0x26, 0x02, 0x2A, 0xC9, 0x0A, 0x90, 0x02, 0xE9, 0x0A, 0x26, 0x03,
  0x26, 0x04, 0x26, 0x05, 0xCA, 0xD0, 0xEA, 0xA6, 0x03, 0x86, 0x00, 0xA6, 0x04, 0x86, 0x01, 0xA6,
  0x05, 0x86, 0x02, 0x60, 0xA9, 0x06, 0x85, 0x00, 0x20, 0x36, 0x4C, 0x85, 0xCA, 0xA0, 0x00, 0x84,
  0xFC, 0x84, 0xFE, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0xA2, 0x20, 0x20, 0x36, 0x4C,
  0x85, 0xCB, 0xB1, 0x06, 0x49, 0xFF, 0x91, 0x06, 0x20, 0x6E, 0x4C, 0xC8, 0xD0, 0xF4, 0xE6, 0x07,
  0xCA, 0xD0, 0xEA, 0xC6, 0x00, 0xD0, 0xDC, 0x60, 0xA0, 0x00, 0x84, 0xFC, 0x84, 0xFE, 0xB1, 0xFC,
  0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0xA2, 0x20, 0xA9, 0x00, 0x91, 0x06, 0xC8, 0xD0, 0xFB, 0xE6,
  0x07, 0xCA, 0xD0, 0xF6, 0x60, 0x20, 0x25, 0x4F, 0x20, 0x1F, 0x4E, 0x20, 0x4F, 0x4E, 0xAE, 0x07,
  0x14, 0xE8, 0x86, 0x00, 0xA9, 0x00, 0x85, 0x01, 0x85, 0x02, 0xA9, 0xB8, 0x85, 0xFC, 0x20, 0xC4,
  0x4E, 0x18, 0x69, 0xB0, 0xA4, 0xFC, 0xA2, 0x27, 0x20, 0x08, 0x51, 0xA5, 0xFC, 0x38, 0xE9, 0x08,
  0x85, 0xFC, 0xA5, 0x00, 0xD0, 0xE8, 0x60, 0x48, 0xAD, 0x03, 0x0C, 0x38, 0xED, 0x05, 0x14, 0xCD,
  0x0B, 0x0C, 0xB0, 0x02, 0x68, 0x60, 0x8A, 0x20, 0x1C, 0x4C, 0x85, 0x04, 0x4A, 0x68, 0x90, 0x0F,
  0x48, 0x29, 0x80, 0x85, 0x06, 0x68, 0x29, 0x7F, 0x4A, 0xB0, 0x02, 0x09, 0x40, 0x05, 0x06, 0x85,
  0x06, 0x84, 0x05, 0xAC, 0x0B, 0x0C, 0x88, 0xAE, 0x05, 0x14, 0xA5, 0x05, 0x9D, 0x70, 0x17, 0xA5,
  0x04, 0x9D, 0xC0, 0x17, 0xA5, 0x06, 0x39, 0x0C, 0x0D, 0x9D, 0x10, 0x18, 0xB9, 0x04, 0x0D, 0x9D,
  0x60, 0x18, 0xAD, 0x07, 0x0C, 0x9D, 0xB0, 0x18, 0xE8, 0x88, 0x10, 0xDE, 0x8E, 0x05, 0x14, 0x60,
  0x20, 0x7C, 0x4C, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05, 0xA4, 0x04, 0xA1,
  0x08, 0x31, 0x06, 0x91, 0x06, 0xE6, 0x08, 0xC8, 0xA1, 0x08, 0x31, 0x06, 0x91, 0x06, 0xE6, 0x08,
  0xC8, 0xA1, 0x08, 0x31, 0x06, 0x91, 0x06, 0xE6, 0x08, 0xA4, 0x05, 0x88, 0x10, 0xD5, 0x4C, 0x6E,
  0x4C, 0x20, 0x7C, 0x4C, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05, 0xA4, 0x04,
  0xA1, 0x08, 0x91, 0x06, 0xE6, 0x08, 0xC8, 0xA1, 0x08, 0x11, 0x06, 0x91, 0x06, 0xE6, 0x08, 0xC8,
  0xA1, 0x08, 0x11, 0x06, 0x91, 0x06, 0xE6, 0x08, 0xA4, 0x05, 0x88, 0x10, 0xD7, 0x4C, 0x6E, 0x4C,
  0x20, 0x7C, 0x4C, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05, 0xA4, 0x04, 0xA1,
  0x08, 0x49, 0xFF, 0x31, 0x06, 0x91, 0x06, 0xE6, 0x08, 0xC8, 0xA1, 0x08, 0x49, 0xFF, 0x31, 0x06,
  0x91, 0x06, 0xE6, 0x08, 0xA4, 0x05, 0x88, 0x10, 0xDA, 0x4C, 0x6E, 0x4C, 0x20, 0x7C, 0x4C, 0xB1,
  0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05, 0xA4, 0x04, 0xA1, 0x08, 0x11, 0x06, 0x91,
  0x06, 0xE6, 0x08, 0xC8, 0xA1, 0x08, 0x11, 0x06, 0x91, 0x06, 0xE6, 0x08, 0xA4, 0x05, 0x88, 0x10,
  0xDE, 0x4C, 0x6E, 0x4C, 0xBD, 0x60, 0x09, 0x85, 0x06, 0xBD, 0x64, 0x09, 0x85, 0x07, 0xA0, 0x00,
  0xB1, 0x06, 0x85, 0xE6, 0xC8, 0xB1, 0x06, 0x85, 0xE7, 0xC8, 0xB1, 0x06, 0x85, 0xE8, 0xC8, 0xB1,
  0x06, 0x85, 0xE9, 0x60, 0x84, 0x07, 0xA0, 0x00, 0x84, 0x20, 0xA9, 0x07, 0x85, 0x21, 0xA5, 0x4E,
  0x85, 0x06, 0xA2, 0x07, 0x20, 0x36, 0x4C, 0xC5, 0x0A, 0x26, 0x06, 0xCA, 0xD0, 0xF6, 0xA5, 0x06,
  0x91, 0x20, 0xC8, 0xC4, 0x07, 0xD0, 0xE7, 0x60, 0xA0, 0x00, 0x20, 0xA1, 0x50, 0xAD, 0x5B, 0x0C,
  0x85, 0xFC, 0x85, 0xFE, 0xA5, 0xE8, 0x85, 0x08, 0xA5, 0xE9, 0x85, 0x09, 0xA4, 0xE7, 0x88, 0xA2,
  0x00, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x01, 0xA5, 0xE6, 0x85, 0x00, 0xAC,
  0x5A, 0x0C, 0xA1, 0x08, 0x21, 0x20, 0x91, 0x06, 0xE6, 0x20, 0xE6, 0x08, 0xD0, 0x02, 0xE6, 0x09,
  0xC8, 0xC6, 0x00, 0xD0, 0xED, 0xA4, 0x01, 0x88, 0x10, 0xD7, 0x60, 0x84, 0xFC, 0x84, 0xFE, 0xA0,
  0x40, 0x84, 0x09, 0x38, 0xE9, 0x20, 0x0A, 0x0A, 0x0A, 0x26, 0x09, 0x85, 0x08, 0xA0, 0x07, 0xB1,
  0xFC, 0x8D, 0x29, 0x51, 0xB1, 0xFE, 0x8D, 0x2A, 0x51, 0xB1, 0x08, 0x9D, 0xFF, 0xFF, 0x88, 0x10,
  0xEE, 0x60, 0xA9, 0x00, 0x85, 0x00, 0x85, 0x08, 0xA9, 0x7A, 0x85, 0x01, 0xA9, 0x90, 0x85, 0x09,
  0xA0, 0x00, 0xB1, 0x00, 0x10, 0x01, 0x60, 0xAA, 0xC8, 0xB1, 0x00, 0x20, 0x4F, 0x4C, 0x86, 0x04,
  0xA9, 0x02, 0x85, 0x02, 0xA4, 0x02, 0xB1, 0x00, 0x85, 0x06, 0xC8, 0xB1, 0x00, 0x85, 0x07, 0xC8,
  0x84, 0x02, 0xC0, 0x10, 0x90, 0x10, 0xA5, 0x00, 0x18, 0x69, 0x10, 0x85, 0x00, 0xA5, 0x01, 0x69,
  0x00, 0x85, 0x01, 0x38, 0xB0, 0xCA, 0xA5, 0x08, 0x18, 0x65, 0x04, 0x90, 0x06, 0xE6, 0x09, 0xA9,
  0x00, 0x85, 0x08, 0xA5, 0x08, 0x91, 0x00, 0xC8, 0xA5, 0x09, 0x91, 0x00, 0xA0, 0x00, 0x18, 0x08,
  0xA5, 0x04, 0x85, 0x05, 0xB1, 0x06, 0x29, 0x80, 0x85, 0x0A, 0xB1, 0x06, 0x28, 0x2A, 0x0A, 0x08,
  0x4A, 0x05, 0x0A, 0x91, 0x08, 0xC8, 0xC6, 0x05, 0xD0, 0xEA, 0x28, 0xA5, 0x08, 0x18, 0x65, 0x04,
  0x85, 0x08, 0xA5, 0x09, 0x69, 0x00, 0x85, 0x09, 0x38, 0xB0, 0x99, 0x68, 0x85, 0x22, 0x68, 0x85,
  0x23, 0x20, 0xE8, 0x51, 0xAA, 0xD0, 0x07, 0xA5, 0x23, 0x48, 0xA5, 0x22, 0x48, 0x60, 0xC9, 0x0D,
  0xD0, 0x0D, 0x20, 0xE8, 0x51, 0x85, 0xE0, 0x20, 0xE8, 0x51, 0x85, 0xE1, 0x4C, 0xBE, 0x51, 0xA6,
  0xE0, 0xA4, 0xE1, 0x20, 0x08, 0x51, 0xE6, 0xE0, 0x4C, 0xBE, 0x51, 0xE6, 0x22, 0xD0, 0x02, 0xE6,
  0x23, 0xA0, 0x00, 0xB1, 0x22, 0x60, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
  0x10, 0x10, 0x70, 0x4C, 0x0C, 0x52, 0x4C, 0x2E, 0x52, 0x4C, 0x1D, 0x55, 0x4C, 0xEC, 0x53, 0xA9,
  0x78, 0x8D, 0x00, 0x15, 0x8D, 0x02, 0x15, 0xA9, 0x5A, 0x8D, 0x01, 0x15, 0x8D, 0x03, 0x15, 0xA9,
  0x00, 0x8D, 0x04, 0x15, 0x8D, 0x05, 0x15, 0x8D, 0x06, 0x15, 0x8D, 0x07, 0x15, 0x8D, 0x08, 0x15,
  0x60, 0xAD, 0x21, 0x14, 0xF0, 0x4A, 0xCE, 0x21, 0x14, 0xCD, 0x02, 0x0C, 0xD0, 0x40, 0xAC, 0x05,
  0x14, 0xA9, 0x0B, 0x85, 0x00, 0xAD, 0x00, 0x15, 0x20, 0x1C, 0x4C, 0x85, 0x02, 0xA5, 0x02, 0x99,
  0xC0, 0x17, 0xA6, 0x00, 0xBD, 0x26, 0x0D, 0x99, 0x10, 0x18, 0xBD, 0x32, 0x0D, 0x99, 0x60, 0x18,
  0xAE, 0x02, 0x0C, 0x20, 0x4F, 0x4C, 0x86, 0x06, 0xA9, 0x60, 0x38, 0xE5, 0x06, 0x99, 0x70, 0x17,
  0xAD, 0x02, 0x0C, 0x99, 0xB0, 0x18, 0xC8, 0xC6, 0x00, 0x10, 0xD2, 0x8C, 0x05, 0x14, 0x18, 0x60,
  0xAD, 0x06, 0x15, 0x29, 0x03, 0x8D, 0x04, 0x15, 0xAD, 0x06, 0x15, 0x4A, 0x4A, 0x8D, 0x05, 0x15,
  0xAD, 0x00, 0x15, 0xAE, 0x04, 0x15, 0x18, 0x7D, 0x00, 0x0D, 0xC9, 0x05, 0x90, 0x04, 0xC9, 0xF4,
  0x90, 0x03, 0xAD, 0x00, 0x15, 0x8D, 0x02, 0x15, 0xAD, 0x01, 0x15, 0xAE, 0x05, 0x15, 0x18, 0x7D,
  0x00, 0x0D, 0xC9, 0x05, 0x90, 0x04, 0xC9, 0xB1, 0x90, 0x03, 0xAD, 0x01, 0x15, 0x8D, 0x03, 0x15,
  0xAD, 0x07, 0x15, 0x0A, 0x0D, 0x08, 0x15, 0x48, 0xAA, 0xBD, 0x00, 0x08, 0x85, 0x06, 0xBD, 0x20,
  0x08, 0x85, 0x07, 0xAC, 0x01, 0x15, 0xAE, 0x00, 0x15, 0x20, 0x2D, 0x50, 0x68, 0xAA, 0xBD, 0x40,
  0x08, 0x85, 0x06, 0xBD, 0x60, 0x08, 0x85, 0x07, 0xAC, 0x01, 0x15, 0x88, 0xAE, 0x00, 0x15, 0xCA,
  0xCA, 0x20, 0x7C, 0x4C, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05, 0xA4, 0x04,
  0xA1, 0x08, 0x31, 0x06, 0xF0, 0x03, 0x20, 0x8D, 0x53, 0xE6, 0x08, 0xC8, 0xA1, 0x08, 0x31, 0x06,
  0xF0, 0x03, 0x20, 0x8D, 0x53, 0xE6, 0x08, 0xC8, 0xA1, 0x08, 0x31, 0x06, 0xF0, 0x03, 0x20, 0x8D,
  0x53, 0xE6, 0x08, 0xA4, 0x05, 0x88, 0x10, 0xCC, 0x20, 0x6E, 0x4C, 0xAD, 0x08, 0x15, 0x49, 0x01,
  0x8D, 0x08, 0x15, 0xAD, 0x06, 0x15, 0x8D, 0x07, 0x15, 0x0A, 0x0D, 0x08, 0x15, 0xAA, 0xBD, 0x00,
  0x08, 0x85, 0x06, 0xBD, 0x20, 0x08, 0x85, 0x07, 0xAC, 0x03, 0x15, 0x8C, 0x01, 0x15, 0xAE, 0x02,
  0x15, 0x8E, 0x00, 0x15, 0x20, 0x7C, 0x4C, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84,
  0x05, 0xA4, 0x04, 0xB1, 0x06, 0x21, 0x08, 0xF0, 0x03, 0x20, 0x8D, 0x53, 0xA1, 0x08, 0x11, 0x06,
  0x91, 0x06, 0xE6, 0x08, 0xC8, 0xB1, 0x06, 0x21, 0x08, 0xF0, 0x03, 0x20, 0x8D, 0x53, 0xA1, 0x08,
  0x11, 0x06, 0x91, 0x06, 0xE6, 0x08, 0xA4, 0x05, 0x88, 0x10, 0xCC, 0x20, 0x6E, 0x4C, 0x18, 0x60,
  0x48, 0x8E, 0x00, 0x07, 0x8C, 0x01, 0x07, 0xA5, 0xFC, 0x8D, 0x02, 0x07, 0xA2, 0x05, 0xB5, 0x04,
  0x9D, 0x03, 0x07, 0xCA, 0x10, 0xF8, 0x68, 0x20, 0x8D, 0x59, 0xA9, 0x01, 0x85, 0xE8, 0x20, 0x18,
  0x5A, 0xB0, 0x24, 0x20, 0xB5, 0x59, 0x90, 0x1F, 0xAD, 0x07, 0x15, 0x0A, 0x0D, 0x08, 0x15, 0xAA,
  0xBD, 0x00, 0x08, 0x85, 0x06, 0xBD, 0x20, 0x08, 0x85, 0x07, 0xAE, 0x02, 0x15, 0xAC, 0x03, 0x15,
  0x20, 0x59, 0x50, 0x68, 0x68, 0x38, 0x60, 0xA2, 0x05, 0xBD, 0x03, 0x07, 0x95, 0x04, 0xCA, 0x10,
  0xF8, 0xAD, 0x02, 0x07, 0x85, 0xFC, 0x85, 0xFE, 0xAC, 0x01, 0x07, 0xAE, 0x00, 0x07, 0x60, 0xAD,
  0x21, 0x14, 0xF0, 0x01, 0x60, 0xCE, 0x01, 0x14, 0x10, 0xFA, 0xEE, 0x01, 0x14, 0xAD, 0x16, 0x14,
  0xF0, 0xF2, 0xAC, 0x00, 0x14, 0xCC, 0x00, 0x0C, 0xB0, 0xEA, 0x99, 0x30, 0x15, 0xAA, 0xA9, 0x01,
  0x99, 0x40, 0x15, 0xA9, 0x02, 0x8D, 0x01, 0x14, 0xEE, 0x00, 0x14, 0xE0, 0x01, 0xD0, 0x20, 0xAD,
  0x00, 0x15, 0x18, 0x69, 0x0B, 0x20, 0x1C, 0x4C, 0x99, 0x20, 0x15, 0xA9, 0xFF, 0x0A, 0xCA, 0x10,
  0xFC, 0x4A, 0x99, 0x50, 0x15, 0xAD, 0x01, 0x15, 0x18, 0x69, 0x05, 0x99, 0x10, 0x15, 0x60, 0xE0,
  0x03, 0xD0, 0x2A, 0xAD, 0x00, 0x15, 0x38, 0xE9, 0x04, 0x20, 0x1C, 0x4C, 0x99, 0x20, 0x15, 0x8A,
  0xF0, 0x0C, 0xA9, 0x00, 0x38, 0x2A, 0xCA, 0x10, 0xFB, 0x99, 0x50, 0x15, 0xD0, 0xD7, 0xA9, 0x7F,
  0x99, 0x50, 0x15, 0xBE, 0x20, 0x15, 0xCA, 0x8A, 0x99, 0x20, 0x15, 0x10, 0xC8, 0xE0, 0x04, 0xD0,
  0x18, 0xAD, 0x01, 0x15, 0x18, 0x69, 0x0E, 0x99, 0x10, 0x15, 0x20, 0x1C, 0x4C, 0x86, 0x00, 0xA9,
  0x06, 0x38, 0xE5, 0x00, 0x99, 0x60, 0x15, 0x10, 0x1C, 0xE0, 0x0C, 0xD0, 0x2F, 0xAD, 0x01, 0x15,
  0x38, 0xE9, 0x04, 0x20, 0x1C, 0x4C, 0x85, 0x00, 0x0A, 0x65, 0x00, 0x0A, 0x65, 0x00, 0x99, 0x10,
  0x15, 0x8A, 0x99, 0x60, 0x15, 0xAD, 0x00, 0x15, 0x18, 0x69, 0x03, 0x20, 0x1C, 0x4C, 0x99, 0x20,
  0x15, 0xA9, 0x00, 0x38, 0x2A, 0xCA, 0x10, 0xFC, 0x99, 0x50, 0x15, 0x60, 0xE0, 0x05, 0xD0, 0x06,
  0x20, 0xDD, 0x54, 0x4C, 0xEF, 0x54, 0xE0, 0x07, 0xD0, 0x06, 0x20, 0xDD, 0x54, 0x4C, 0x06, 0x55,
  0xE0, 0x0D, 0xD0, 0x06, 0x20, 0xE5, 0x54, 0x4C, 0xEF, 0x54, 0x20, 0xE5, 0x54, 0x4C, 0x06, 0x55,
  0xAD, 0x01, 0x15, 0x18, 0x69, 0x0C, 0x90, 0x06, 0xAD, 0x01, 0x15, 0x38, 0xE9, 0x02, 0x99, 0x10,
  0x15, 0x60, 0xAD, 0x00, 0x15, 0x18, 0x69, 0x09, 0x20, 0x1C, 0x4C, 0x99, 0x20, 0x15, 0xA9, 0x00,
  0x38, 0x2A, 0xCA, 0x10, 0xFC, 0x99, 0x50, 0x15, 0x60, 0xAD, 0x00, 0x15, 0x38, 0xE9, 0x02, 0x20,
  0xF5, 0x54, 0x4A, 0x90, 0x0A, 0x98, 0xAA, 0xDE, 0x20, 0x15, 0xA9, 0x40, 0x99, 0x50, 0x15, 0x60,
  0xAE, 0x00, 0x14, 0xAD, 0x21, 0x14, 0xD0, 0x03, 0xCA, 0x10, 0x01, 0x60, 0x86, 0x00, 0xBD, 0x30,
  0x15, 0xC9, 0x04, 0xB0, 0x5A, 0x49, 0x03, 0x85, 0x04, 0xC6, 0x04, 0xBD, 0x40, 0x15, 0xF0, 0x05,
  0xDE, 0x40, 0x15, 0x10, 0x18, 0x20, 0x37, 0x58, 0xA9, 0x7F, 0x9D, 0x50, 0x15, 0xBD, 0x20, 0x15,
  0x18, 0x65, 0x04, 0x9D, 0x20, 0x15, 0xC9, 0x25, 0x90, 0x03, 0x4C, 0x59, 0x59, 0xBD, 0x10, 0x15,
  0x85, 0xFC, 0x85, 0xFE, 0xA0, 0x00, 0x84, 0x05, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07,
  0xBC, 0x20, 0x15, 0xBD, 0x50, 0x15, 0x31, 0x06, 0xF0, 0x0B, 0x20, 0xAA, 0x59, 0x90, 0x06, 0x20,
  0x37, 0x58, 0x4C, 0x59, 0x59, 0xBD, 0x50, 0x15, 0x11, 0x06, 0x91, 0x06, 0x4C, 0x25, 0x55, 0xC9,
  0x04, 0xD0, 0x6B, 0xBD, 0x40, 0x15, 0xF0, 0x05, 0xDE, 0x40, 0x15, 0x10, 0x1B, 0x20, 0x55, 0x58,
  0xA6, 0x00, 0xBD, 0x10, 0x15, 0x38, 0x7D, 0x60, 0x15, 0x9D, 0x10, 0x15, 0xC9, 0xBD, 0x90, 0x03,
  0x4C, 0x59, 0x59, 0xA9, 0x06, 0x9D, 0x60, 0x15, 0xBD, 0x10, 0x15, 0x85, 0xFC, 0x85, 0xFE, 0xBC,
  0x60, 0x15, 0xBD, 0x20, 0x15, 0x85, 0x04, 0xBD, 0x50, 0x15, 0xAA, 0xB1, 0xFC, 0x85, 0x06, 0xB1,
  0xFE, 0x85, 0x07, 0x84, 0x05, 0xA4, 0x04, 0x8A, 0x31, 0x06, 0xF0, 0x13, 0x86, 0x01, 0xA6, 0x00,
  0x20, 0xAA, 0x59, 0x90, 0x08, 0x20, 0x55, 0x58, 0xA6, 0x1F, 0x4C, 0x59, 0x59, 0xA6, 0x01, 0x8A,
  0x11, 0x06, 0x91, 0x06, 0xA4, 0x05, 0x88, 0x10, 0xD2, 0xA6, 0x00, 0x4C, 0x25, 0x55, 0xC9, 0x0C,
  0xD0, 0x68, 0xBD, 0x40, 0x15, 0xF0, 0x05, 0xDE, 0x40, 0x15, 0x10, 0x18, 0x20, 0x55, 0x58, 0xA6,
  0x00, 0xBD, 0x10, 0x15, 0x38, 0xE9, 0x07, 0x9D, 0x10, 0x15, 0xB0, 0x03, 0x4C, 0x59, 0x59, 0xA9,
  0x06, 0x9D, 0x60, 0x15, 0xBD, 0x10, 0x15, 0x85, 0xFC, 0x85, 0xFE, 0xBC, 0x60, 0x15, 0xBD, 0x20,
  0x15, 0x85, 0x04, 0xBD, 0x50, 0x15, 0xAA, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84,
  0x05, 0xA4, 0x04, 0x8A, 0x31, 0x06, 0xF0, 0x13, 0x86, 0x01, 0xA6, 0x00, 0x20, 0xAA, 0x59, 0x90,
  0x08, 0x20, 0x55, 0x58, 0xA6, 0x1F, 0x4C, 0x59, 0x59, 0xA6, 0x01, 0x8A, 0x11, 0x06, 0x91, 0x06,
  0xA4, 0x05, 0x88, 0x10, 0xD2, 0xA6, 0x00, 0x4C, 0x25, 0x55, 0xC9, 0x05, 0xD0, 0x77, 0xBD, 0x40,
  0x15, 0xF0, 0x05, 0xDE, 0x40, 0x15, 0x10, 0x23, 0x20, 0x83, 0x58, 0xA6, 0x00, 0xA9, 0x01, 0x9D,
  0x50, 0x15, 0xFE, 0x20, 0x15, 0xBD, 0x20, 0x15, 0xC9, 0x25, 0xB0, 0x0C, 0x98, 0x18, 0x7D, 0x10,
  0x15, 0x9D, 0x10, 0x15, 0xC9, 0xBD, 0x90, 0x03, 0x4C, 0x59, 0x59, 0xBD, 0x20, 0x15, 0x85, 0x04,
  0xBD, 0x10, 0x15, 0x85, 0xFC, 0x85, 0xFE, 0xBD, 0x50, 0x15, 0xAA, 0xA0, 0x00, 0xB1, 0xFC, 0x85,
  0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05, 0xA4, 0x04, 0x8A, 0x31, 0x06, 0xF0, 0x13, 0x86, 0x01,
  0xA6, 0x00, 0x20, 0xAA, 0x59, 0x90, 0x08, 0x20, 0x83, 0x58, 0xA6, 0x1F, 0x4C, 0x59, 0x59, 0xA6,
  0x01, 0x8A, 0x11, 0x06, 0x91, 0x06, 0x8A, 0x0A, 0x30, 0x06, 0xAA, 0xA4, 0x05, 0xC8, 0xD0, 0xCD,
  0xA6, 0x00, 0x4C, 0x25, 0x55, 0xC9, 0x07, 0xD0, 0x71, 0xBD, 0x40, 0x15, 0xF0, 0x05, 0xDE, 0x40,
  0x15, 0x10, 0x1E, 0x20, 0xB9, 0x58, 0xA6, 0x00, 0xA9, 0x80, 0x9D, 0x50, 0x15, 0xDE, 0x20, 0x15,
  0x30, 0x0C, 0x98, 0x18, 0x7D, 0x10, 0x15, 0x9D, 0x10, 0x15, 0xC9, 0xBD, 0x90, 0x03, 0x4C, 0x59,
  0x59, 0xBD, 0x20, 0x15, 0x85, 0x04, 0xBD, 0x10, 0x15, 0x85, 0xFC, 0x85, 0xFE, 0xBD, 0x50, 0x15,
  0xAA, 0xA0, 0x00, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05, 0xA4, 0x04, 0x8A,
  0x4A, 0xB0, 0x22, 0xAA, 0x31, 0x06, 0xF0, 0x13, 0x86, 0x01, 0xA6, 0x00, 0x20, 0xAA, 0x59, 0x90,
  0x08, 0x20, 0xB9, 0x58, 0xA6, 0x1F, 0x4C, 0x59, 0x59, 0xA6, 0x01, 0x8A, 0x11, 0x06, 0x91, 0x06,
  0xA4, 0x05, 0xC8, 0xD0, 0xCE, 0xA6, 0x00, 0x4C, 0x25, 0x55, 0xC9, 0x0D, 0xD0, 0x71, 0xBD, 0x40,
  0x15, 0xF0, 0x05, 0xDE, 0x40, 0x15, 0x10, 0x1D, 0x20, 0xEE, 0x58, 0xA6, 0x00, 0xA9, 0x01, 0x9D,
  0x50, 0x15, 0xFE, 0x20, 0x15, 0xBD, 0x20, 0x15, 0xC9, 0x25, 0xB0, 0x06, 0x98, 0x9D, 0x10, 0x15,
  0xD0, 0x03, 0x4C, 0x59, 0x59, 0xBD, 0x20, 0x15, 0x85, 0x04, 0xA9, 0x00, 0x85, 0xFC, 0x85, 0xFE,
  0xBC, 0x10, 0x15, 0xBD, 0x50, 0x15, 0xAA, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84,
  0x05, 0xA4, 0x04, 0x8A, 0x31, 0x06, 0xF0, 0x13, 0x86, 0x01, 0xA6, 0x00, 0x20, 0xAA, 0x59, 0x90,
  0x08, 0x20, 0xEE, 0x58, 0xA6, 0x1F, 0x4C, 0x59, 0x59, 0xA6, 0x01, 0x8A, 0x11, 0x06, 0x91, 0x06,
  0x8A, 0x0A, 0x30, 0x06, 0xAA, 0xA4, 0x05, 0x88, 0xD0, 0xCD, 0xA6, 0x00, 0x4C, 0x25, 0x55, 0xBD,
  0x40, 0x15, 0xF0, 0x05, 0xDE, 0x40, 0x15, 0x10, 0x18, 0x20, 0x24, 0x59, 0xA6, 0x00, 0xA9, 0x80,
  0x9D, 0x50, 0x15, 0xDE, 0x20, 0x15, 0x30, 0x06, 0x98, 0x9D, 0x10, 0x15, 0xD0, 0x03, 0x4C, 0x59,
  0x59, 0xBD, 0x20, 0x15, 0x85, 0x04, 0xA9, 0x00, 0x85, 0xFC, 0x85, 0xFE, 0xBC, 0x10, 0x15, 0xBD,
  0x50, 0x15, 0xAA, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05, 0xA4, 0x04, 0x8A,
  0x4A, 0xB0, 0x22, 0xAA, 0x31, 0x06, 0xF0, 0x13, 0x86, 0x01, 0xA6, 0x00, 0x20, 0xAA, 0x59, 0x90,
  0x08, 0x20, 0x24, 0x59, 0xA6, 0x1F, 0x4C, 0x59, 0x59, 0xA6, 0x01, 0x8A, 0x11, 0x06, 0x91, 0x06,
  0xA4, 0x05, 0x88, 0xD0, 0xCE, 0xA6, 0x00, 0x4C, 0x25, 0x55, 0xBD, 0x10, 0x15, 0x85, 0xFC, 0x85,
  0xFE, 0xA0, 0x00, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0xBC, 0x20, 0x15, 0xBD, 0x50,
  0x15, 0x49, 0xFF, 0x31, 0x06, 0x91, 0x06, 0x60, 0xBD, 0x10, 0x15, 0x85, 0xFC, 0x85, 0xFE, 0xBC,
  0x60, 0x15, 0xBD, 0x20, 0x15, 0x85, 0x04, 0xBD, 0x50, 0x15, 0x49, 0xFF, 0xAA, 0xB1, 0xFC, 0x85,
  0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05, 0xA4, 0x04, 0x8A, 0x31, 0x06, 0x91, 0x06, 0xA4, 0x05,
  0x88, 0x10, 0xEA, 0x4C, 0x6E, 0x4C, 0xBD, 0x20, 0x15, 0x85, 0x04, 0xBD, 0x10, 0x15, 0x85, 0xFC,
  0x85, 0xFE, 0xBD, 0x50, 0x15, 0x49, 0xFF, 0xAA, 0xA0, 0x00, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE,
  0x85, 0x07, 0x84, 0x05, 0xA4, 0x04, 0x8A, 0x31, 0x06, 0x91, 0x06, 0x8A, 0x38, 0x2A, 0x10, 0x06,
  0xAA, 0xA4, 0x05, 0xC8, 0xD0, 0xE4, 0xA4, 0x05, 0xC8, 0x4C, 0x6E, 0x4C, 0xBD, 0x20, 0x15, 0x85,
  0x04, 0xBD, 0x10, 0x15, 0x85, 0xFC, 0x85, 0xFE, 0xBD, 0x50, 0x15, 0x49, 0xFF, 0xAA, 0xA0, 0x00,
  0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05, 0xA4, 0x04, 0x8A, 0x38, 0x6A, 0x90,
  0x0A, 0xAA, 0x31, 0x06, 0x91, 0x06, 0xA4, 0x05, 0xC8, 0xD0, 0xE5, 0xA4, 0x05, 0xC8, 0x4C, 0x6E,
  0x4C, 0xBD, 0x20, 0x15, 0x85, 0x04, 0xA9, 0x00, 0x85, 0xFC, 0x85, 0xFE, 0xBC, 0x10, 0x15, 0xBD,
  0x50, 0x15, 0x49, 0xFF, 0xAA, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05, 0xA4,
  0x04, 0x8A, 0x31, 0x06, 0x91, 0x06, 0x8A, 0x38, 0x2A, 0x10, 0x06, 0xAA, 0xA4, 0x05, 0x88, 0xD0,
  0xE4, 0xA4, 0x05, 0x88, 0x4C, 0x6E, 0x4C, 0xBD, 0x20, 0x15, 0x85, 0x04, 0xA9, 0x00, 0x85, 0xFC,
  0x85, 0xFE, 0xBC, 0x10, 0x15, 0xBD, 0x50, 0x15, 0x49, 0xFF, 0xAA, 0xB1, 0xFC, 0x85, 0x06, 0xB1,
  0xFE, 0x85, 0x07, 0x84, 0x05, 0xA4, 0x04, 0x8A, 0x38, 0x6A, 0x90, 0x0A, 0xAA, 0x31, 0x06, 0x91,
  0x06, 0xA4, 0x05, 0x88, 0xD0, 0xE5, 0xA4, 0x05, 0x88, 0x4C, 0x6E, 0x4C, 0xCE, 0x00, 0x14, 0x8A,
  0x48, 0xBD, 0x11, 0x15, 0x9D, 0x10, 0x15, 0xBD, 0x21, 0x15, 0x9D, 0x20, 0x15, 0xBD, 0x31, 0x15,
  0x9D, 0x30, 0x15, 0xBD, 0x41, 0x15, 0x9D, 0x40, 0x15, 0xBD, 0x51, 0x15, 0x9D, 0x50, 0x15, 0xBD,
  0x61, 0x15, 0x9D, 0x60, 0x15, 0xE8, 0xEC, 0x00, 0x14, 0x90, 0xD6, 0x68, 0xAA, 0x4C, 0x25, 0x55,
  0xA2, 0xFF, 0xE8, 0x4A, 0x90, 0xFC, 0x86, 0x19, 0x98, 0x85, 0x1A, 0x0A, 0x65, 0x1A, 0x0A, 0x65,
  0x1A, 0x65, 0x19, 0x85, 0x1C, 0xA5, 0xFC, 0x18, 0x65, 0x05, 0x85, 0x1D, 0x60, 0x86, 0x1F, 0x84,
  0x1E, 0x20, 0x8D, 0x59, 0xA9, 0x00, 0x85, 0xE8, 0x20, 0x12, 0x5A, 0xB0, 0x32, 0x20, 0x15, 0x5A,
  0xB0, 0x2D, 0x20, 0x0C, 0x62, 0xB0, 0x28, 0x20, 0x0F, 0x62, 0xB0, 0x23, 0x20, 0x12, 0x62, 0xB0,
  0x1E, 0x20, 0x0C, 0x70, 0xB0, 0x19, 0x20, 0x0F, 0x70, 0xB0, 0x14, 0x20, 0x12, 0x70, 0xB0, 0x0F,
  0x20, 0x0C, 0x69, 0xB0, 0x0A, 0x20, 0x0F, 0x69, 0xB0, 0x05, 0x20, 0x12, 0x69, 0xB0, 0x00, 0xA6,
  0x1F, 0xA4, 0x1E, 0x60, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
  0x10, 0x10, 0x70, 0x4C, 0x2A, 0x5A, 0x4C, 0xB1, 0x5A, 0x4C, 0x26, 0x5B, 0x4C, 0x7E, 0x5B, 0x4C,
  0x9F, 0x5C, 0x4C, 0x60, 0x5E, 0x4C, 0xF2, 0x5B, 0x4C, 0xBE, 0x5D, 0x4C, 0x73, 0x60, 0x4C, 0x8A,
  0x5F, 0x4C, 0xE3, 0x5F, 0x4C, 0x52, 0x5F, 0x4C, 0x27, 0x61, 0x4C, 0xB9, 0x61, 0xA9, 0x00, 0x8D,
  0x08, 0x14, 0xAC, 0x02, 0x14, 0x98, 0x4A, 0x4A, 0x8D, 0x21, 0x14, 0x88, 0x10, 0x16, 0xAD, 0x07,
  0x14, 0xA2, 0x0A, 0x20, 0x00, 0x4C, 0xAD, 0x0C, 0x0C, 0xE0, 0x08, 0xD0, 0x03, 0xAD, 0x0F, 0x0C,
  0x8D, 0x03, 0x14, 0x60, 0x20, 0x36, 0x4C, 0xC9, 0xA0, 0xB0, 0x24, 0xA9, 0xF5, 0x20, 0x4B, 0x4C,
  0x29, 0xFE, 0x99, 0x70, 0x15, 0xA9, 0x22, 0x20, 0x4B, 0x4C, 0x85, 0x00, 0xA2, 0x00, 0x20, 0x36,
  0x4C, 0x10, 0x02, 0xA2, 0x90, 0x8A, 0x18, 0x65, 0x00, 0x99, 0xF0, 0x15, 0x38, 0xB0, 0x21, 0xA9,
  0xB2, 0x20, 0x4B, 0x4C, 0x99, 0xF0, 0x15, 0xA9, 0x45, 0x20, 0x4B, 0x4C, 0x85, 0x00, 0xA2, 0x00,
  0x20, 0x36, 0x4C, 0x10, 0x02, 0xA2, 0xB0, 0x8A, 0x18, 0x65, 0x00, 0x29, 0xFE, 0x99, 0x70, 0x15,
  0x20, 0x36, 0x4C, 0x29, 0x01, 0x99, 0x70, 0x16, 0x98, 0x4A, 0x4A, 0x18, 0x6D, 0x07, 0x0C, 0x99,
  0xF0, 0x16, 0xD0, 0x87, 0xAC, 0x09, 0x14, 0x88, 0x10, 0x01, 0x60, 0x20, 0x36, 0x4C, 0xC9, 0xA0,
  0xB0, 0x25, 0xA9, 0xE6, 0x20, 0x4B, 0x4C, 0x18, 0x69, 0x06, 0x09, 0x01, 0x99, 0x00, 0x19, 0xA2,
  0x06, 0x20, 0x36, 0x4C, 0x10, 0x02, 0xA2, 0x90, 0x86, 0x00, 0xA9, 0x1C, 0x20, 0x4B, 0x4C, 0x18,
  0x65, 0x00, 0x99, 0x10, 0x19, 0xD0, 0x23, 0xA9, 0xA6, 0x20, 0x4B, 0x4C, 0x18, 0x69, 0x06, 0x99,
  0x10, 0x19, 0xA2, 0x06, 0x20, 0x36, 0x4C, 0x10, 0x02, 0xA2, 0xC0, 0x86, 0x00, 0xA9, 0x2C, 0x20,
  0x4B, 0x4C, 0x18, 0x65, 0x00, 0x09, 0x01, 0x99, 0x00, 0x19, 0x20, 0x36, 0x4C, 0x29, 0x01, 0x99,
  0x20, 0x19, 0x98, 0x29, 0x03, 0x99, 0x30, 0x19, 0xAD, 0x0E, 0x0C, 0x20, 0x4B, 0x4C, 0x18, 0x69,
  0x03, 0x09, 0x01, 0x99, 0x40, 0x19, 0x4C, 0xB4, 0x5A, 0xAC, 0x0A, 0x14, 0x88, 0x10, 0x19, 0xA9,
  0x00, 0x8D, 0x0B, 0x14, 0x8D, 0x0C, 0x14, 0xA9, 0x0A, 0x8D, 0x0D, 0x14, 0xA9, 0x00, 0x8D, 0x05,
  0x14, 0x8D, 0x2C, 0x14, 0x8D, 0x2D, 0x14, 0x60, 0xA9, 0xF1, 0x20, 0x4B, 0x4C, 0x18, 0x69, 0x04,
  0x99, 0x50, 0x19, 0xA9, 0xAE, 0x20, 0x4B, 0x4C, 0x18, 0x69, 0x04, 0x99, 0x60, 0x19, 0x20, 0x36,
  0x4C, 0x29, 0x01, 0x99, 0x70, 0x19, 0xA9, 0x05, 0x20, 0x4B, 0x4C, 0x38, 0xE9, 0x02, 0x99, 0x80,
  0x19, 0x98, 0x29, 0x03, 0x99, 0xA0, 0x19, 0xA9, 0x20, 0x20, 0x4B, 0x4C, 0x99, 0xB0, 0x19, 0x10,
  0xAB, 0xAE, 0x02, 0x14, 0xCA, 0x10, 0x03, 0x4C, 0x08, 0x60, 0xDE, 0xF0, 0x16, 0x10, 0xF5, 0x86,
  0x00, 0xBC, 0x70, 0x16, 0xB9, 0x88, 0x08, 0x85, 0x06, 0xB9, 0x8C, 0x08, 0x85, 0x07, 0xBC, 0xF0,
  0x15, 0xBD, 0x70, 0x15, 0xAA, 0x20, 0xCD, 0x4F, 0xA6, 0x00, 0xBD, 0x70, 0x16, 0x49, 0x01, 0x9D,
  0x70, 0x16, 0xA8, 0xB9, 0x80, 0x08, 0x85, 0x06, 0xB9, 0x84, 0x08, 0x85, 0x07, 0xBD, 0xF0, 0x15,
  0xCD, 0x01, 0x15, 0xB0, 0x05, 0x6D, 0x09, 0x0C, 0x90, 0x03, 0xED, 0x09, 0x0C, 0x9D, 0xF0, 0x15,
  0xA8, 0xBD, 0x70, 0x15, 0xCD, 0x00, 0x15, 0xB0, 0x05, 0x6D, 0x09, 0x0C, 0x90, 0x03, 0xED, 0x09,
  0x0C, 0x9D, 0x70, 0x15, 0xAA, 0x20, 0xFE, 0x4F, 0xAD, 0x03, 0x14, 0x20, 0x4B, 0x4C, 0xA6, 0x00,
  0x9D, 0xF0, 0x16, 0x10, 0x8F, 0xAE, 0x02, 0x14, 0xA5, 0x1C, 0x38, 0xE9, 0x0B, 0xB0, 0x02, 0xA9,
  0x00, 0xA8, 0xA5, 0x1C, 0xCA, 0x10, 0x02, 0x18, 0x60, 0xDD, 0x70, 0x15, 0x90, 0xF6, 0x98, 0xDD,
  0x70, 0x15, 0xB0, 0xEE, 0xA5, 0x1D, 0xDD, 0xF0, 0x15, 0x90, 0xE7, 0x38, 0xE9, 0x0E, 0x90, 0x05,
  0xDD, 0xF0, 0x15, 0xB0, 0xDD, 0xA5, 0xE8, 0xF0, 0x02, 0x38, 0x60, 0x20, 0x3F, 0x5C, 0xAD, 0x04,
  0x0C, 0x8D, 0x06, 0x14, 0xAD, 0x05, 0x0C, 0x8D, 0x22, 0x14, 0xAD, 0x06, 0x0C, 0x8D, 0x23, 0x14,
  0x38, 0x60, 0x8A, 0x48, 0xAA, 0xBC, 0x70, 0x16, 0xB9, 0x88, 0x08, 0x85, 0x06, 0xB9, 0x8C, 0x08,
  0x85, 0x07, 0xBC, 0xF0, 0x15, 0xBD, 0x70, 0x15, 0xAA, 0x20, 0xCD, 0x4F, 0x68, 0x48, 0xAA, 0xBD,
  0xF0, 0x15, 0x18, 0x69, 0x07, 0xA8, 0xBD, 0x70, 0x15, 0x18, 0x69, 0x03, 0xAA, 0xA9, 0xAA, 0x20,
  0x74, 0x4F, 0x68, 0x48, 0xAA, 0xCE, 0x02, 0x14, 0xBD, 0x71, 0x15, 0x9D, 0x70, 0x15, 0xBD, 0xF1,
  0x15, 0x9D, 0xF0, 0x15, 0xBD, 0x71, 0x16, 0x9D, 0x70, 0x16, 0xBD, 0xF1, 0x16, 0x9D, 0xF0, 0x16,
  0xE8, 0xEC, 0x02, 0x14, 0x90, 0xE2, 0xA9, 0x00, 0xA2, 0x64, 0x20, 0xD0, 0x4D, 0x68, 0xAA, 0x4C,
  0x6E, 0x4C, 0xAE, 0x09, 0x14, 0xCA, 0x10, 0x01, 0x60, 0xDE, 0x30, 0x19, 0x10, 0xF7, 0x86, 0x00,
  0xA9, 0x03, 0x9D, 0x30, 0x19, 0xDE, 0x40, 0x19, 0x10, 0x28, 0xAD, 0x21, 0x14, 0xD0, 0x13, 0xBD,
  0x20, 0x19, 0x29, 0x02, 0x49, 0x02, 0x85, 0x06, 0x20, 0x36, 0x4C, 0x29, 0x01, 0x05, 0x06, 0x9D,
  0x20, 0x19, 0xAD, 0x0E, 0x0C, 0x20, 0x4B, 0x4C, 0x18, 0x69, 0x03, 0x09, 0x01, 0xA4, 0x00, 0x99,
  0x40, 0x19, 0xA4, 0x00, 0xB9, 0x00, 0x19, 0xBE, 0x10, 0x19, 0xA0, 0x0E, 0x20, 0x21, 0x4D, 0xA4,
  0x00, 0xAD, 0x21, 0x14, 0xF0, 0x07, 0xB9, 0x20, 0x19, 0x0A, 0xAA, 0x10, 0x63, 0xBE, 0x20, 0x19,
  0xB9, 0x00, 0x19, 0x18, 0x7D, 0x14, 0x0D, 0xC9, 0x06, 0x90, 0x04, 0xC9, 0xED, 0x90, 0x0C, 0x20,
  0x36, 0x4C, 0x29, 0x01, 0x09, 0x02, 0x99, 0x20, 0x19, 0xD0, 0xE2, 0x99, 0x00, 0x19, 0x18, 0x69,
  0x10, 0x85, 0x1A, 0xE9, 0x19, 0xB0, 0x02, 0xA9, 0x00, 0x85, 0x1B, 0xBE, 0x20, 0x19, 0xB9, 0x10,
  0x19, 0x18, 0x7D, 0x18, 0x0D, 0xC9, 0x06, 0x90, 0x04, 0xC9, 0xAC, 0x90, 0x0A, 0x20, 0x36, 0x4C,
  0x29, 0x01, 0x99, 0x20, 0x19, 0x10, 0xE4, 0x99, 0x10, 0x19, 0x18, 0x69, 0x11, 0x85, 0x1C, 0xE9,
  0x1D, 0xB0, 0x02, 0xA9, 0x00, 0x85, 0x1D, 0xB9, 0x40, 0x19, 0x4A, 0xB9, 0x20, 0x19, 0x2A, 0xAA,
  0xBD, 0x90, 0x08, 0x85, 0x06, 0xBD, 0x98, 0x08, 0x85, 0x07, 0xBE, 0x00, 0x19, 0xB9, 0x10, 0x19,
  0xA8, 0x20, 0xF6, 0x4C, 0xAD, 0x21, 0x14, 0xD0, 0x43, 0xAE, 0x0A, 0x14, 0xA4, 0x1B, 0xA5, 0x1A,
  0xCA, 0x30, 0x39, 0xDD, 0x50, 0x19, 0x90, 0xF8, 0x98, 0xDD, 0x50, 0x19, 0xB0, 0xF0, 0xA5, 0x1C,
  0xDD, 0x60, 0x19, 0x90, 0xE9, 0xA5, 0x1D, 0xDD, 0x60, 0x19, 0xB0, 0xE2, 0x8A, 0x48, 0xA8, 0xBE,
  0x60, 0x19, 0xB9, 0x50, 0x19, 0xA0, 0x0A, 0x20, 0xD0, 0x4C, 0x68, 0xAA, 0x48, 0xBC, 0x60, 0x19,
  0xBD, 0x50, 0x19, 0xAA, 0x20, 0x27, 0x61, 0x68, 0xAA, 0x20, 0x52, 0x5F, 0xA6, 0x00, 0x4C, 0xA2,
  0x5C, 0xAE, 0x09, 0x14, 0xA5, 0x1C, 0x38, 0xE9, 0x0D, 0xB0, 0x02, 0xA9, 0x00, 0xA8, 0xA5, 0x1C,
  0xCA, 0x10, 0x02, 0x18, 0x60, 0xDD, 0x00, 0x19, 0x90, 0xF6, 0x98, 0xDD, 0x00, 0x19, 0xB0, 0xEE,
  0xA5, 0x1D, 0xDD, 0x10, 0x19, 0x90, 0xE7, 0x38, 0xE9, 0x0E, 0x90, 0x05, 0xDD, 0x10, 0x19, 0xB0,
  0xDD, 0xA5, 0xE8, 0xF0, 0x02, 0x38, 0x60, 0x8A, 0x48, 0xA8, 0xBE, 0x10, 0x19, 0xB9, 0x00, 0x19,
  0xA0, 0x0E, 0x20, 0x21, 0x4D, 0x68, 0xAA, 0x20, 0x26, 0x5E, 0xBD, 0x40, 0x19, 0x4A, 0xBD, 0x20,
  0x19, 0x2A, 0xA8, 0xB9, 0x90, 0x08, 0x85, 0x06, 0xB9, 0x98, 0x08, 0x85, 0x07, 0xBC, 0x10, 0x19,
  0xBD, 0x00, 0x19, 0xAA, 0x20, 0xF6, 0x4C, 0x38, 0x60, 0xA4, 0x1F, 0xB9, 0x30, 0x15, 0x48, 0x29,
  0x03, 0xA8, 0xBD, 0x00, 0x19, 0x18, 0x79, 0x00, 0x0D, 0xC9, 0x06, 0xB0, 0x02, 0x69, 0x04, 0xC9,
  0xED, 0x90, 0x02, 0xE9, 0x04, 0x9D, 0x00, 0x19, 0x68, 0x4A, 0x4A, 0xA8, 0xBD, 0x10, 0x19, 0x18,
  0x79, 0x00, 0x0D, 0xC9, 0x06, 0xB0, 0x02, 0x69, 0x04, 0xC9, 0xAC, 0x90, 0x02, 0xE9, 0x04, 0x9D,
  0x10, 0x19, 0x60, 0xAE, 0x0A, 0x14, 0xCA, 0x10, 0x03, 0x4C, 0xB0, 0x60, 0xDE, 0xA0, 0x19, 0x10,
  0xF5, 0x86, 0x00, 0xAD, 0x08, 0x0C, 0x9D, 0xA0, 0x19, 0xDE, 0xB0, 0x19, 0x10, 0x1E, 0x20, 0x36,
  0x4C, 0x29, 0x01, 0x9D, 0x70, 0x19, 0xA9, 0x05, 0x20, 0x4B, 0x4C, 0x38, 0xE9, 0x02, 0xA4, 0x00,
  0x99, 0x80, 0x19, 0xAD, 0x0A, 0x0C, 0x20, 0x4B, 0x4C, 0x99, 0xB0, 0x19, 0xA4, 0x00, 0xBE, 0x60,
  0x19, 0xB9, 0x50, 0x19, 0xA0, 0x0A, 0x20, 0xD0, 0x4C, 0xA4, 0x00, 0xB9, 0x50, 0x19, 0xBE, 0x70,
  0x19, 0x18, 0x7D, 0x1C, 0x0D, 0xC9, 0x04, 0x90, 0x04, 0xC9, 0xF5, 0x90, 0x08, 0x8A, 0x49, 0x01,
  0x99, 0x70, 0x19, 0x10, 0xE6, 0x99, 0x50, 0x19, 0xB9, 0x60, 0x19, 0x18, 0x79, 0x80, 0x19, 0xC9,
  0x04, 0x90, 0x04, 0xC9, 0xB2, 0x90, 0x08, 0xA9, 0x00, 0x99, 0x80, 0x19, 0xB9, 0x60, 0x19, 0x99,
  0x60, 0x19, 0xB9, 0x90, 0x19, 0x0A, 0x19, 0x70, 0x19, 0x0A, 0x85, 0x06, 0xB9, 0xB0, 0x19, 0x29,
  0x01, 0x05, 0x06, 0xAA, 0xBD, 0xA0, 0x08, 0x85, 0x06, 0xBD, 0xB0, 0x08, 0x85, 0x07, 0xBE, 0x50,
  0x19, 0xB9, 0x60, 0x19, 0xA8, 0x20, 0xAC, 0x4C, 0xA6, 0x00, 0x4C, 0x63, 0x5E, 0xBD, 0x60, 0x19,
  0x18, 0x69, 0x02, 0x48, 0xBD, 0x50, 0x19, 0x38, 0xE9, 0x04, 0x48, 0x20, 0x52, 0x5F, 0xAD, 0x0D,
  0x14, 0xA2, 0x0A, 0x20, 0x00, 0x4C, 0x38, 0xE9, 0x01, 0x85, 0x06, 0x68, 0xAA, 0x68, 0xA8, 0xA5,
  0x06, 0x20, 0xB9, 0x61, 0xAD, 0x0D, 0x14, 0xA2, 0x64, 0x20, 0x4F, 0x4C, 0x20, 0xD0, 0x4D, 0xAD,
  0x0D, 0x14, 0x18, 0x69, 0x0A, 0xC9, 0x33, 0xB0, 0x03, 0x8D, 0x0D, 0x14, 0xAD, 0x68, 0x0C, 0x8D,
  0x0B, 0x14, 0x4C, 0x6E, 0x4C, 0x8A, 0x48, 0xCE, 0x0A, 0x14, 0xBD, 0x51, 0x19, 0x9D, 0x50, 0x19,
  0xBD, 0x61, 0x19, 0x9D, 0x60, 0x19, 0xBD, 0x71, 0x19, 0x9D, 0x70, 0x19, 0xBD, 0x81, 0x19, 0x9D,
  0x80, 0x19, 0xBD, 0x91, 0x19, 0x9D, 0x90, 0x19, 0xBD, 0xA1, 0x19, 0x9D, 0xA0, 0x19, 0xBD, 0xB1,
  0x19, 0x9D, 0xB0, 0x19, 0xE8, 0xEC, 0x0A, 0x14, 0x90, 0xD0, 0x68, 0xAA, 0x60, 0xAE, 0x05, 0x14,
  0xA9, 0x00, 0x85, 0xFC, 0x85, 0xFE, 0xCA, 0x10, 0x01, 0x60, 0xBC, 0x70, 0x17, 0xB1, 0xFC, 0x85,
  0x06, 0xB1, 0xFE, 0x85, 0x07, 0xBC, 0xC0, 0x17, 0xA9, 0x00, 0x91, 0x06, 0xBD, 0x70, 0x17, 0x18,
  0x7D, 0x60, 0x18, 0x9D, 0x70, 0x17, 0xDE, 0xB0, 0x18, 0xD0, 0xDB, 0x8A, 0xA8, 0xCE, 0x05, 0x14,
  0xB9, 0x71, 0x17, 0x99, 0x70, 0x17, 0xB9, 0xC1, 0x17, 0x99, 0xC0, 0x17, 0xB9, 0x11, 0x18, 0x99,
  0x10, 0x18, 0xB9, 0x61, 0x18, 0x99, 0x60, 0x18, 0xB9, 0xB1, 0x18, 0x99, 0xB0, 0x18, 0xC8, 0xCC,
  0x05, 0x14, 0x90, 0xDC, 0xB0, 0xB0, 0xAE, 0x05, 0x14, 0xA9, 0x00, 0x85, 0xFC, 0x85, 0xFE, 0xCA,
  0x10, 0x01, 0x60, 0xBC, 0x70, 0x17, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0xBC, 0xC0,
  0x17, 0xBD, 0x10, 0x18, 0x91, 0x06, 0xD0, 0xE7, 0x4C, 0xEC, 0x5F, 0xAC, 0x08, 0x14, 0xCC, 0x02,
  0x14, 0x90, 0x01, 0x60, 0x98, 0x18, 0x69, 0x04, 0x8D, 0x08, 0x14, 0xB9, 0xF0, 0x15, 0x18, 0x69,
  0x07, 0x85, 0x05, 0xB9, 0x70, 0x15, 0x18, 0x69, 0x03, 0x20, 0x1C, 0x4C, 0x85, 0x04, 0xAC, 0x0B,
  0x0C, 0x88, 0xAD, 0x05, 0x14, 0xAA, 0x18, 0x6D, 0x0B, 0x0C, 0x8D, 0x05, 0x14, 0xAD, 0x07, 0x0C,
  0x9D, 0xB0, 0x18, 0xA5, 0x04, 0x9D, 0xC0, 0x17, 0x4A, 0xA9, 0xAA, 0x90, 0x02, 0xA9, 0xD5, 0x39,
  0x0C, 0x0D, 0x9D, 0x10, 0x18, 0xB9, 0x04, 0x0D, 0x9D, 0x60, 0x18, 0x86, 0x00, 0xAE, 0x07, 0x0C,
  0x20, 0x4F, 0x4C, 0x86, 0x02, 0xA5, 0x05, 0x38, 0xE5, 0x02, 0xA6, 0x00, 0x9D, 0x70, 0x17, 0xE8,
  0x88, 0x10, 0xCA, 0x4C, 0x6E, 0x4C, 0xAE, 0x0A, 0x14, 0xA5, 0x1C, 0xCA, 0x10, 0x02, 0x18, 0x60,
  0xDD, 0x50, 0x19, 0x90, 0xF6, 0xE9, 0x08, 0x90, 0x05, 0xDD, 0x50, 0x19, 0xB0, 0xEB, 0xA5, 0x1D,
  0xDD, 0x60, 0x19, 0x90, 0xE4, 0xE9, 0x0B, 0x90, 0x05, 0xDD, 0x60, 0x19, 0xB0, 0xDB, 0x8A, 0x48,
  0xA8, 0xBE, 0x60, 0x19, 0xB9, 0x50, 0x19, 0xA0, 0x0A, 0x20, 0xD0, 0x4C, 0x68, 0xAA, 0x20, 0x0A,
  0x5F, 0x38, 0x60, 0xAE, 0x2C, 0x14, 0xCA, 0x10, 0x03, 0x4C, 0x50, 0x61, 0xDE, 0xD0, 0x1E, 0x10,
  0xF5, 0xAD, 0x5E, 0x0C, 0x9D, 0xD0, 0x1E, 0x86, 0x00, 0xBC, 0xF0, 0x1E, 0xB9, 0x40, 0x09, 0x85,
  0x06, 0xB9, 0x48, 0x09, 0x85, 0x07, 0xBC, 0xC0, 0x1E, 0xBD, 0xB0, 0x1E, 0xAA, 0x20, 0xF6, 0x4C,
  0xA6, 0x00, 0xDE, 0xE0, 0x1E, 0x10, 0xCF, 0xA4, 0x00, 0xBE, 0xC0, 0x1E, 0xB9, 0xF0, 0x1E, 0xC9,
  0x05, 0xB9, 0xB0, 0x1E, 0xA0, 0x05, 0x90, 0x02, 0xA0, 0x0D, 0x20, 0x21, 0x4D, 0xA6, 0x00, 0xCE,
  0x2C, 0x14, 0xBD, 0xB1, 0x1E, 0x9D, 0xB0, 0x1E, 0xBD, 0xC1, 0x1E, 0x9D, 0xC0, 0x1E, 0xBD, 0xD1,
  0x1E, 0x9D, 0xD0, 0x1E, 0xBD, 0xE1, 0x1E, 0x9D, 0xE0, 0x1E, 0xBD, 0xF1, 0x1E, 0x9D, 0xF0, 0x1E,
  0xE8, 0xEC, 0x2C, 0x14, 0x90, 0xDC, 0xA6, 0x00, 0x10, 0x8C, 0x88, 0x98, 0xAC, 0x2D, 0x14, 0x99,
  0x10, 0x1F, 0x8A, 0x20, 0x1C, 0x4C, 0xA2, 0x07, 0x20, 0x4F, 0x4C, 0x8A, 0x99, 0x00, 0x1F, 0xA9,
  0x00, 0x99, 0x20, 0x1F, 0xAD, 0x61, 0x0C, 0x99, 0x30, 0x1F, 0xEE, 0x2D, 0x14, 0xA9, 0xFF, 0x8D,
  0x0C, 0x14, 0x60, 0xAE, 0x2D, 0x14, 0xCA, 0x10, 0x01, 0x60, 0xDE, 0x20, 0x1F, 0x10, 0xF7, 0x86,
  0x00, 0xAD, 0x60, 0x0C, 0x9D, 0x20, 0x1F, 0xBD, 0x30, 0x1F, 0x29, 0x03, 0xA8, 0xB9, 0x50, 0x09,
  0x85, 0x06, 0xB9, 0x58, 0x09, 0x85, 0x07, 0xBC, 0x10, 0x1F, 0xBD, 0x00, 0x1F, 0xAA, 0x20, 0xAC,
  0x4C, 0xA6, 0x00, 0xDE, 0x30, 0x1F, 0x10, 0xCE, 0xA4, 0x00, 0xBE, 0x10, 0x1F, 0xB9, 0x00, 0x1F,
  0xA0, 0x0C, 0x20, 0xD0, 0x4C, 0xA6, 0x00, 0xCE, 0x2D, 0x14, 0xBD, 0x01, 0x1F, 0x9D, 0x00, 0x1F,
  0xBD, 0x11, 0x1F, 0x9D, 0x10, 0x1F, 0xBD, 0x21, 0x1F, 0x9D, 0x20, 0x1F, 0xBD, 0x31, 0x1F, 0x9D,
  0x30, 0x1F, 0xE8, 0xEC, 0x2D, 0x14, 0x90, 0xE2, 0xA6, 0x00, 0x10, 0x9A, 0x48, 0x8A, 0x20, 0x1C,
  0x4C, 0xA2, 0x07, 0x20, 0x4F, 0x4C, 0x8A, 0xAE, 0x2C, 0x14, 0x9D, 0xB0, 0x1E, 0x98, 0x9D, 0xC0,
  0x1E, 0x68, 0x9D, 0xF0, 0x1E, 0xC9, 0x05, 0xA9, 0x00, 0x9D, 0xD0, 0x1E, 0xAD, 0x5F, 0x0C, 0x90,
  0x02, 0xA9, 0x01, 0x9D, 0xE0, 0x1E, 0xEE, 0x2C, 0x14, 0x60, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x4C, 0x1B, 0x62, 0x4C, 0xBE, 0x62, 0x4C, 0x36, 0x64, 0x4C, 0x23, 0x65, 0x4C,
  0x9F, 0x66, 0x4C, 0x0E, 0x67, 0x4C, 0x43, 0x66, 0x4C, 0x4E, 0x68, 0x4C, 0x6A, 0x68, 0xAE, 0x0E,
  0x14, 0xA9, 0x00, 0x18, 0xCA, 0x30, 0x05, 0x7D, 0x30, 0x1A, 0x90, 0xF8, 0x6D, 0x0F, 0x14, 0x85,
  0x00, 0xA2, 0x00, 0xA0, 0x05, 0xA5, 0x00, 0xF0, 0x12, 0x38, 0xE9, 0x05, 0xB0, 0x04, 0xA4, 0x00,
  0xA9, 0x00, 0x85, 0x00, 0x98, 0x9D, 0x30, 0x1A, 0xE8, 0xD0, 0xE8, 0x8E, 0x0E, 0x14, 0xAC, 0x0E,
  0x14, 0x88, 0x10, 0x0E, 0xA9, 0x00, 0x8D, 0x0F, 0x14, 0x8D, 0x10, 0x14, 0xA9, 0xFF, 0x8D, 0x14,
  0x14, 0x60, 0x20, 0x36, 0x4C, 0x30, 0x1A, 0xA9, 0xF3, 0x20, 0x4B, 0x4C, 0x09, 0x01, 0x99, 0xC0,
  0x19, 0xA2, 0x00, 0x20, 0x36, 0x4C, 0x10, 0x02, 0xA2, 0xB3, 0x8A, 0x99, 0xD0, 0x19, 0x38, 0xB0,
  0x15, 0xA9, 0xB3, 0x20, 0x4B, 0x4C, 0x99, 0xD0, 0x19, 0xA2, 0x01, 0x20, 0x36, 0x4C, 0x10, 0x02,
  0xA2, 0xF3, 0x8A, 0x99, 0xC0, 0x19, 0xA9, 0x00, 0x99, 0xE0, 0x19, 0x99, 0xF0, 0x19, 0x99, 0x40,
  0x1A, 0x99, 0x00, 0x1A, 0xAD, 0x21, 0x14, 0xAE, 0x18, 0x0C, 0xE8, 0x20, 0x00, 0x4C, 0x99, 0x10,
  0x1A, 0xAD, 0x13, 0x0C, 0x20, 0x4B, 0x4C, 0x18, 0x6D, 0x12, 0x0C, 0x99, 0x20, 0x1A, 0x38, 0xB0,
  0x90, 0xAE, 0x0E, 0x14, 0xCA, 0x10, 0x01, 0x60, 0xDE, 0x00, 0x1A, 0x10, 0xF7, 0xAD, 0x18, 0x0C,
  0x9D, 0x00, 0x1A, 0x86, 0x00, 0xA4, 0x00, 0xBE, 0xD0, 0x19, 0xB9, 0xC0, 0x19, 0xA0, 0x0D, 0x20,
  0x21, 0x4D, 0xA6, 0x00, 0xDE, 0x10, 0x1A, 0x10, 0x3C, 0xAD, 0x1B, 0x0C, 0x20, 0x4B, 0x4C, 0xA4,
  0x00, 0x99, 0x10, 0x1A, 0xAD, 0x17, 0x0C, 0x0A, 0x48, 0x20, 0x4B, 0x4C, 0x38, 0xED, 0x17, 0x0C,
  0x29, 0xFE, 0x99, 0xE0, 0x19, 0x68, 0x20, 0x4B, 0x4C, 0x38, 0xED, 0x17, 0x0C, 0x99, 0xF0, 0x19,
  0xA6, 0x00, 0xDE, 0x20, 0x1A, 0x10, 0x0E, 0xAD, 0x13, 0x0C, 0x20, 0x4B, 0x4C, 0x99, 0x20, 0x1A,
  0xA6, 0x00, 0x20, 0xB3, 0x63, 0xBD, 0x20, 0x1A, 0xD0, 0x2C, 0xBD, 0x10, 0x1A, 0xCD, 0x1C, 0x0C,
  0xB0, 0x24, 0xBD, 0xE0, 0x19, 0x30, 0x07, 0xF0, 0x08, 0x38, 0xE9, 0x02, 0xB0, 0x03, 0x18, 0x69,
  0x02, 0x9D, 0xE0, 0x19, 0xBD, 0xF0, 0x19, 0x30, 0x07, 0xF0, 0x08, 0x38, 0xE9, 0x02, 0xB0, 0x03,
  0x18, 0x69, 0x02, 0x9D, 0xF0, 0x19, 0xDE, 0x40, 0x1A, 0x10, 0x05, 0xA9, 0x03, 0x9D, 0x40, 0x1A,
  0xBC, 0x40, 0x1A, 0xB9, 0xC0, 0x08, 0x85, 0x06, 0xB9, 0xC8, 0x08, 0x85, 0x07, 0xBD, 0xC0, 0x19,
  0x18, 0x7D, 0xE0, 0x19, 0xC9, 0xF3, 0x90, 0x10, 0xA9, 0x00, 0xBC, 0xE0, 0x19, 0x9D, 0xE0, 0x19,
  0x30, 0x04, 0xA9, 0xF1, 0x30, 0x02, 0xA9, 0x01, 0x9D, 0xC0, 0x19, 0xBD, 0xD0, 0x19, 0x18, 0x7D,
  0xF0, 0x19, 0xC9, 0xB3, 0x90, 0x10, 0xA9, 0x00, 0xBC, 0xF0, 0x19, 0x9D, 0xF0, 0x19, 0x30, 0x04,
  0xA9, 0xB2, 0x30, 0x02, 0xA9, 0x00, 0x9D, 0xD0, 0x19, 0xA8, 0xBD, 0xC0, 0x19, 0xAA, 0x20, 0xF6,
  0x4C, 0xA6, 0x00, 0x4C, 0xC1, 0x62, 0xAC, 0x0F, 0x14, 0xC0, 0x10, 0x90, 0x01, 0x60, 0xBD, 0xC0,
  0x19, 0x99, 0x50, 0x1A, 0xBD, 0xD0, 0x19, 0x99, 0x60, 0x1A, 0xA9, 0x01, 0x99, 0x70, 0x1A, 0x86,
  0x00, 0x98, 0xAA, 0x20, 0xB8, 0x67, 0xA6, 0x00, 0xAD, 0x1E, 0x0C, 0x8D, 0x12, 0x14, 0xAD, 0x1D,
  0x0C, 0x8D, 0x13, 0x14, 0xEE, 0x0F, 0x14, 0xDE, 0x30, 0x1A, 0xF0, 0x01, 0x60, 0x20, 0xF2, 0x63,
  0x68, 0x68, 0x4C, 0xC1, 0x62, 0x8A, 0x48, 0xCE, 0x0E, 0x14, 0xBD, 0xC1, 0x19, 0x9D, 0xC0, 0x19,
  0xBD, 0xD1, 0x19, 0x9D, 0xD0, 0x19, 0xBD, 0xE1, 0x19, 0x9D, 0xE0, 0x19, 0xBD, 0xF1, 0x19, 0x9D,
  0xF0, 0x19, 0xBD, 0x01, 0x1A, 0x9D, 0x00, 0x1A, 0xBD, 0x11, 0x1A, 0x9D, 0x10, 0x1A, 0xBD, 0x21,
  0x1A, 0x9D, 0x20, 0x1A, 0xBD, 0x31, 0x1A, 0x9D, 0x30, 0x1A, 0xBD, 0x41, 0x1A, 0x9D, 0x40, 0x1A,
  0xE8, 0xEC, 0x0E, 0x14, 0x90, 0xC4, 0x68, 0xAA, 0x60, 0xAE, 0x0F, 0x14, 0xD0, 0x01, 0x60, 0xCA,
  0x10, 0x03, 0x4C, 0x89, 0x67, 0xDE, 0x70, 0x1A, 0x10, 0xF5, 0xAD, 0x19, 0x0C, 0x9D, 0x70, 0x1A,
  0x86, 0x00, 0xA4, 0x00, 0xBE, 0x60, 0x1A, 0xB9, 0x50, 0x1A, 0xA0, 0x0D, 0x20, 0x21, 0x4D, 0xA6,
  0x00, 0xBD, 0x50, 0x1A, 0x18, 0x7D, 0x80, 0x1A, 0xBC, 0x80, 0x1A, 0x30, 0x0A, 0xB0, 0x04, 0xC9,
  0xF4, 0x90, 0x08, 0xA9, 0xF3, 0x30, 0x04, 0xB0, 0x02, 0xA9, 0x00, 0x9D, 0x50, 0x1A, 0xBD, 0x60,
  0x1A, 0x18, 0x7D, 0x90, 0x1A, 0xC9, 0xB3, 0x90, 0x09, 0xA9, 0xB2, 0xBC, 0x90, 0x1A, 0x10, 0x02,
  0xA9, 0x00, 0x9D, 0x60, 0x1A, 0xA8, 0xBD, 0x50, 0x1A, 0xAA, 0xAD, 0xD0, 0x08, 0x85, 0x06, 0xAD,
  0xD4, 0x08, 0x85, 0x07, 0x20, 0xF6, 0x4C, 0xA6, 0x00, 0x10, 0x94, 0xAC, 0x10, 0x14, 0xC0, 0x10,
  0xB0, 0x50, 0xBD, 0x60, 0x1A, 0x69, 0x03, 0x99, 0xB0, 0x1A, 0xBD, 0x50, 0x1A, 0x69, 0x04, 0x99,
  0xA0, 0x1A, 0xAE, 0x00, 0x15, 0x20, 0x00, 0x65, 0x99, 0xC0, 0x1A, 0xB9, 0xB0, 0x1A, 0xAE, 0x01,
  0x15, 0x20, 0x00, 0x65, 0x99, 0xD0, 0x1A, 0xAD, 0x16, 0x0C, 0x99, 0xF0, 0x1A, 0xA5, 0x4E, 0x29,
  0x01, 0x99, 0xE0, 0x1A, 0xA9, 0x03, 0x20, 0x4B, 0x4C, 0x38, 0xE9, 0x01, 0x99, 0x00, 0x1B, 0xA9,
  0x03, 0x20, 0x4B, 0x4C, 0x38, 0xE9, 0x01, 0x99, 0x10, 0x1B, 0x19, 0x00, 0x1B, 0xF0, 0xF0, 0xEE,
  0x10, 0x14, 0x60, 0x4A, 0x4A, 0x4A, 0x4A, 0x4A, 0x85, 0x06, 0x8A, 0x4A, 0x4A, 0x4A, 0x4A, 0x4A,
  0x38, 0xE5, 0x06, 0x85, 0x06, 0xA9, 0x05, 0x20, 0x4B, 0x4C, 0x38, 0xE9, 0x02, 0x18, 0x65, 0x06,
  0xD0, 0x03, 0x18, 0x69, 0x01, 0x60, 0xAE, 0x10, 0x14, 0xCA, 0x10, 0x01, 0x60, 0xDE, 0xE0, 0x1A,
  0x10, 0xF7, 0xAD, 0x1A, 0x0C, 0x9D, 0xE0, 0x1A, 0x86, 0x00, 0xA4, 0x00, 0xBE, 0xB0, 0x1A, 0xB9,
  0xA0, 0x1A, 0xA0, 0x07, 0x20, 0xD0, 0x4C, 0xA6, 0x00, 0xDE, 0xF0, 0x1A, 0xD0, 0x06, 0x20, 0xD9,
  0x65, 0x4C, 0x26, 0x65, 0xBD, 0xF0, 0x1A, 0x29, 0x01, 0x85, 0x08, 0xA8, 0xB9, 0xD8, 0x08, 0x85,
  0x06, 0xB9, 0xDC, 0x08, 0x85, 0x07, 0xBD, 0xB0, 0x1A, 0x18, 0x7D, 0xD0, 0x1A, 0xC9, 0xB9, 0x90,
  0x0F, 0xA9, 0xB8, 0xBC, 0xD0, 0x1A, 0x10, 0x02, 0xA9, 0x00, 0x9D, 0xB0, 0x1A, 0x48, 0x10, 0x19,
  0x9D, 0xB0, 0x1A, 0x48, 0xA5, 0x08, 0xF0, 0x11, 0xBD, 0xD0, 0x1A, 0x7D, 0x10, 0x1B, 0xC9, 0xF6,
  0xB0, 0x04, 0xC9, 0x0A, 0xB0, 0x03, 0x9D, 0xD0, 0x1A, 0xBD, 0xA0, 0x1A, 0x18, 0x7D, 0xC0, 0x1A,
  0xBC, 0xC0, 0x1A, 0x30, 0x06, 0x90, 0x0F, 0xA9, 0xF9, 0x30, 0x04, 0xB0, 0x09, 0xA9, 0x00, 0x9D,
  0xA0, 0x1A, 0x48, 0x38, 0xB0, 0x1A, 0x9D, 0xA0, 0x1A, 0x48, 0xA5, 0x08, 0xF0, 0x12, 0xBD, 0xC0,
  0x1A, 0x18, 0x7D, 0x00, 0x1B, 0xC9, 0xF6, 0xB0, 0x04, 0xC9, 0x0A, 0xB0, 0x03, 0x9D, 0xC0, 0x1A,
  0x68, 0xAA, 0x68, 0xA8, 0x20, 0xAC, 0x4C, 0xA6, 0x00, 0x4C, 0x26, 0x65, 0x8A, 0x48, 0xCE, 0x10,
  0x14, 0xBD, 0xA1, 0x1A, 0x9D, 0xA0, 0x1A, 0xBD, 0xB1, 0x1A, 0x9D, 0xB0, 0x1A, 0xBD, 0xC1, 0x1A,
  0x9D, 0xC0, 0x1A, 0xBD, 0xD1, 0x1A, 0x9D, 0xD0, 0x1A, 0xBD, 0xE1, 0x1A, 0x9D, 0xE0, 0x1A, 0xBD,
  0xF1, 0x1A, 0x9D, 0xF0, 0x1A, 0xBD, 0x01, 0x1B, 0x9D, 0x00, 0x1B, 0xBD, 0x11, 0x1B, 0x9D, 0x10,
  0x1B, 0xE8, 0xEC, 0x10, 0x14, 0x90, 0xCA, 0x68, 0xAA, 0x60, 0x8A, 0x48, 0xCE, 0x0F, 0x14, 0xBD,
  0x51, 0x1A, 0x9D, 0x50, 0x1A, 0xBD, 0x61, 0x1A, 0x9D, 0x60, 0x1A, 0xBD, 0x71, 0x1A, 0x9D, 0x70,
  0x1A, 0xBD, 0x81, 0x1A, 0x9D, 0x80, 0x1A, 0xBD, 0x91, 0x1A, 0x9D, 0x90, 0x1A, 0xE8, 0xEC, 0x0F,
  0x14, 0x90, 0xDC, 0x68, 0xAA, 0x60, 0xAE, 0x10, 0x14, 0xA5, 0x1C, 0x38, 0xE9, 0x07, 0xB0, 0x02,
  0xA9, 0x00, 0xA8, 0xA5, 0x1C, 0xCA, 0x10, 0x02, 0x18, 0x60, 0xDD, 0xA0, 0x1A, 0x90, 0xF6, 0x98,
  0xDD, 0xA0, 0x1A, 0xB0, 0xEE, 0xA5, 0x1D, 0xDD, 0xB0, 0x1A, 0x90, 0xE7, 0xE9, 0x07, 0x90, 0x05,
  0xDD, 0xB0, 0x1A, 0xB0, 0xDE, 0xA5, 0xE8, 0xF0, 0x02, 0x38, 0x60, 0x8A, 0x48, 0xA8, 0xBE, 0xB0,
  0x1A, 0xB9, 0xA0, 0x1A, 0xA0, 0x07, 0x20, 0xD0, 0x4C, 0x68, 0xAA, 0x20, 0xD9, 0x65, 0xAD, 0x20,
  0x0C, 0x8D, 0x13, 0x14, 0xA9, 0x01, 0x8D, 0x12, 0x14, 0xA9, 0x00, 0xA2, 0x19, 0x20, 0xD0, 0x4D,
  0x38, 0x60, 0xAE, 0x0E, 0x14, 0xA5, 0x1C, 0x38, 0xE9, 0x0D, 0xB0, 0x02, 0xA9, 0x00, 0xA8, 0xA5,
  0x1C, 0xCA, 0x10, 0x02, 0x18, 0x60, 0xDD, 0xC0, 0x19, 0x90, 0xF6, 0x98, 0xDD, 0xC0, 0x19, 0xB0,
  0xEE, 0xA5, 0x1D, 0xDD, 0xD0, 0x19, 0x90, 0xE7, 0xE9, 0x0D, 0x90, 0x05, 0xDD, 0xD0, 0x19, 0xB0,
  0xDE, 0xA5, 0xE8, 0xF0, 0x02, 0x38, 0x60, 0x8A, 0x48, 0xA8, 0xBE, 0xD0, 0x19, 0xB9, 0xC0, 0x19,
  0xA0, 0x0D, 0x20, 0x21, 0x4D, 0x68, 0x48, 0xA8, 0xB9, 0xC0, 0x19, 0x18, 0x69, 0x03, 0xAA, 0xB9,
  0xD0, 0x19, 0xA8, 0xA9, 0x05, 0x20, 0x27, 0x5A, 0x68, 0xAA, 0x20, 0xF2, 0x63, 0xA9, 0x03, 0xA2,
  0xE8, 0x20, 0xD0, 0x4D, 0xAD, 0x22, 0x0C, 0x8D, 0x14, 0x14, 0xA9, 0x00, 0x8D, 0x15, 0x14, 0x38,
  0x60, 0xAE, 0x0F, 0x14, 0xA5, 0x1C, 0x38, 0xE9, 0x0A, 0xB0, 0x02, 0xA9, 0x00, 0xA8, 0xA5, 0x1C,
  0xCA, 0x10, 0x02, 0x18, 0x60, 0xDD, 0x50, 0x1A, 0x90, 0xF6, 0x98, 0xDD, 0x50, 0x1A, 0xF0, 0x02,
  0xB0, 0xEC, 0xA5, 0x1D, 0xDD, 0x60, 0x1A, 0x90, 0xE5, 0xE9, 0x0D, 0x90, 0x05, 0xDD, 0x60, 0x1A,
  0xB0, 0xDC, 0xA5, 0xE8, 0xF0, 0x02, 0x38, 0x60, 0x8A, 0x48, 0xA8, 0xBE, 0x60, 0x1A, 0xB9, 0x50,
  0x1A, 0xA0, 0x0D, 0x20, 0x21, 0x4D, 0x68, 0xAA, 0x48, 0xBD, 0x60, 0x1A, 0x18, 0x69, 0x06, 0xA8,
  0xBD, 0x50, 0x1A, 0x18, 0x69, 0x03, 0xAA, 0xA9, 0x55, 0x20, 0x74, 0x4F, 0x68, 0xAA, 0x20, 0x17,
  0x66, 0xA9, 0x00, 0xA2, 0xC8, 0x20, 0xD0, 0x4D, 0xAD, 0x34, 0x0C, 0x8D, 0x06, 0x14, 0xAD, 0x35,
  0x0C, 0x8D, 0x22, 0x14, 0xAD, 0x36, 0x0C, 0x8D, 0x23, 0x14, 0x38, 0x60, 0xCE, 0x24, 0x14, 0x30,
  0x01, 0x60, 0xAD, 0x25, 0x14, 0x20, 0x4B, 0x4C, 0x8D, 0x24, 0x14, 0xAD, 0x0F, 0x14, 0x20, 0x4B,
  0x4C, 0xAA, 0x20, 0xA8, 0x64, 0xAD, 0x0F, 0x14, 0x20, 0x4B, 0x4C, 0xAA, 0x20, 0x36, 0x4C, 0xCD,
  0x3A, 0x0C, 0x90, 0x07, 0xCD, 0x3B, 0x0C, 0x90, 0x38, 0xB0, 0x19, 0xBD, 0x50, 0x1A, 0xAC, 0x00,
  0x15, 0x20, 0x32, 0x68, 0x9D, 0x80, 0x1A, 0xBD, 0x60, 0x1A, 0xAC, 0x01, 0x15, 0x20, 0x32, 0x68,
  0x9D, 0x90, 0x1A, 0x60, 0x8A, 0xA8, 0xAD, 0x15, 0x0C, 0x0A, 0x48, 0x20, 0x4B, 0x4C, 0x38, 0xED,
  0x15, 0x0C, 0x99, 0x80, 0x1A, 0x68, 0x20, 0x4B, 0x4C, 0x38, 0xED, 0x15, 0x0C, 0x99, 0x90, 0x1A,
  0x60, 0xBD, 0x50, 0x1A, 0x85, 0x02, 0xAD, 0x60, 0x1A, 0x85, 0x03, 0xAD, 0x00, 0x15, 0x85, 0x04,
  0xAD, 0x01, 0x15, 0x85, 0x05, 0x46, 0x02, 0x46, 0x03, 0x46, 0x04, 0x46, 0x05, 0xA5, 0x02, 0xE5,
  0x04, 0xA8, 0xB0, 0x02, 0x49, 0xFF, 0xCD, 0x15, 0x0C, 0xB0, 0xEA, 0x84, 0x06, 0xA5, 0x03, 0xE5,
  0x05, 0xA8, 0xB0, 0x02, 0x49, 0xFF, 0xCD, 0x15, 0x0C, 0xB0, 0xDA, 0xA5, 0x06, 0x9D, 0x80, 0x1A,
  0x98, 0x9D, 0x90, 0x1A, 0x60, 0x85, 0x03, 0x4A, 0x4A, 0x4A, 0x4A, 0x4A, 0x85, 0x02, 0x98, 0x4A,
  0x4A, 0x4A, 0x4A, 0x4A, 0xE5, 0x02, 0xD0, 0x08, 0xA9, 0x01, 0xC4, 0x03, 0xB0, 0x02, 0xA9, 0xFF,
  0x60, 0xA2, 0x02, 0xBD, 0x0A, 0x15, 0xDD, 0x32, 0x14, 0x90, 0x11, 0xD0, 0x06, 0xCA, 0x10, 0xF3,
  0x4C, 0x69, 0x68, 0xBD, 0x0A, 0x15, 0x9D, 0x32, 0x14, 0xCA, 0x10, 0xF7, 0x60, 0x20, 0xB8, 0x51,
  0x0D, 0x0C, 0x00, 0x48, 0x49, 0x47, 0x48, 0x20, 0x53, 0x43, 0x4F, 0x52, 0x45, 0x3A, 0x00, 0xA2,
  0x02, 0xBD, 0x32, 0x14, 0x95, 0x00, 0xCA, 0x10, 0xF8, 0xA0, 0x00, 0x20, 0xC4, 0x4E, 0xC8, 0xA5,
  0x00, 0x05, 0x01, 0x05, 0x02, 0xD0, 0xF4, 0x98, 0x18, 0x65, 0xE0, 0x85, 0xE0, 0xA2, 0x02, 0xBD,
  0x32, 0x14, 0x95, 0x00, 0xCA, 0x10, 0xF8, 0xC6, 0xE0, 0x20, 0xC4, 0x4E, 0x18, 0x69, 0xB0, 0xA6,
  0xE0, 0xA4, 0xE1, 0x20, 0x08, 0x51, 0xA5, 0x00, 0x05, 0x01, 0x05, 0x02, 0xD0, 0xE9, 0x60, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x4C, 0x15, 0x69, 0x4C, 0x0F, 0x6A, 0x4C, 0x7B, 0x6B, 0x4C, 0x67, 0x6C, 0x4C,
  0x34, 0x6E, 0x4C, 0x98, 0x6E, 0x4C, 0x13, 0x6F, 0xAE, 0x4D, 0x0C, 0xE8, 0xAD, 0x21, 0x14, 0x20,
  0x00, 0x4C, 0x85, 0x08, 0xAC, 0x27, 0x14, 0x88, 0x10, 0x03, 0x4C, 0x7D, 0x69, 0xA9, 0x02, 0x06,
  0x4E, 0xA6, 0x4F, 0xB0, 0x17, 0x10, 0x02, 0xA9, 0xB2, 0x99, 0xF0, 0x1C, 0xA9, 0xF1, 0x20, 0x4B,
  0x4C, 0x18, 0x69, 0x02, 0x29, 0xFE, 0x99, 0xE0, 0x1C, 0x4C, 0x58, 0x69, 0x10, 0x02, 0xA9, 0xF2,
  0x99, 0xE0, 0x1C, 0xA9, 0xB3, 0x20, 0x4B, 0x4C, 0x99, 0xF0, 0x1C, 0xA9, 0x00, 0x99, 0x00, 0x1D,
  0x99, 0x10, 0x1D, 0x99, 0x60, 0x1D, 0x98, 0x29, 0x01, 0x99, 0x20, 0x1D, 0xA5, 0x08, 0x99, 0x30,
  0x1D, 0xAD, 0x52, 0x0C, 0x20, 0x4B, 0x4C, 0x18, 0x6D, 0x53, 0x0C, 0x99, 0x50, 0x1D, 0x10, 0xA7,
  0xAD, 0x21, 0x14, 0xAE, 0x4E, 0x0C, 0xE8, 0x20, 0x00, 0x4C, 0x85, 0x08, 0xAC, 0x28, 0x14, 0x88,
  0x10, 0x15, 0xA9, 0x00, 0x8D, 0x29, 0x14, 0xA9, 0xFF, 0x8D, 0x2E, 0x14, 0xAD, 0x21, 0x14, 0x18,
  0x6D, 0x59, 0x0C, 0x8D, 0x2A, 0x14, 0x60, 0x06, 0x4E, 0xA5, 0x4F, 0x30, 0x22, 0xA9, 0x10, 0x90,
  0x02, 0xA9, 0xA0, 0x85, 0x06, 0xA9, 0x3E, 0x20, 0x4B, 0x4C, 0x18, 0x65, 0x06, 0x09, 0x01, 0x99,
  0x70, 0x1D, 0xA9, 0x90, 0x20, 0x4B, 0x4C, 0x18, 0x69, 0x10, 0x99, 0x90, 0x1D, 0xD0, 0x20, 0xA9,
  0x10, 0x90, 0x02, 0xA9, 0x80, 0x85, 0x06, 0xA9, 0x20, 0x20, 0x4B, 0x4C, 0x18, 0x65, 0x06, 0x99,
  0x90, 0x1D, 0xA9, 0xCE, 0x20, 0x4B, 0x4C, 0x18, 0x69, 0x10, 0x09, 0x01, 0x99, 0x70, 0x1D, 0xA9,
  0x00, 0x99, 0xB0, 0x1D, 0x99, 0xD0, 0x1D, 0xA5, 0x08, 0x99, 0x10, 0x1E, 0xA2, 0x00, 0x20, 0x36,
  0x4C, 0x10, 0x02, 0xA2, 0x03, 0x8A, 0x99, 0x30, 0x1E, 0x98, 0x29, 0x03, 0x99, 0xF0, 0x1D, 0x4C,
  0x8C, 0x69, 0xAE, 0x27, 0x14, 0xCA, 0x10, 0x01, 0x60, 0xDE, 0x20, 0x1D, 0x10, 0xF7, 0xAD, 0x4D,
  0x0C, 0x9D, 0x20, 0x1D, 0x86, 0x00, 0xA4, 0x00, 0xBE, 0xF0, 0x1C, 0xB9, 0xE0, 0x1C, 0xA0, 0x0D,
  0x20, 0x21, 0x4D, 0xA4, 0x00, 0xB9, 0xE0, 0x1C, 0x18, 0x79, 0x00, 0x1D, 0xD0, 0x02, 0xA9, 0x02,
  0xC9, 0xF3, 0x90, 0x10, 0xA9, 0xF2, 0xBE, 0x00, 0x1D, 0x10, 0x02, 0xA9, 0x02, 0x48, 0xA9, 0x00,
  0x99, 0x00, 0x1D, 0x68, 0x99, 0xE0, 0x1C, 0xB9, 0xF0, 0x1C, 0x18, 0x79, 0x10, 0x1D, 0xC9, 0xB3,
  0x90, 0x10, 0xA9, 0xB2, 0xBE, 0x10, 0x1D, 0x10, 0x02, 0xA9, 0x00, 0x48, 0xA9, 0x00, 0x99, 0x10,
  0x1D, 0x68, 0x99, 0xF0, 0x1C, 0xB9, 0x30, 0x1D, 0x29, 0x03, 0xAA, 0xBD, 0x10, 0x09, 0x85, 0x06,
  0xBD, 0x18, 0x09, 0x85, 0x07, 0xBE, 0xE0, 0x1C, 0xB9, 0xF0, 0x1C, 0xA8, 0x20, 0xF6, 0x4C, 0xA6,
  0x00, 0xDE, 0x30, 0x1D, 0x10, 0x3F, 0xAD, 0x4A, 0x0C, 0x20, 0x4B, 0x4C, 0x09, 0x03, 0xA6, 0x00,
  0x9D, 0x30, 0x1D, 0xBD, 0xE0, 0x1C, 0xAC, 0x00, 0x15, 0x20, 0x3A, 0x6B, 0x9D, 0x00, 0x1D, 0xBD,
  0xF0, 0x1C, 0xAC, 0x01, 0x15, 0x20, 0x3A, 0x6B, 0x9D, 0x10, 0x1D, 0xA9, 0x00, 0x9D, 0x60, 0x1D,
  0xDE, 0x50, 0x1D, 0x10, 0x10, 0xAD, 0x52, 0x0C, 0x20, 0x4B, 0x4C, 0xA6, 0x00, 0x9D, 0x50, 0x1D,
  0xA9, 0x01, 0x9D, 0x60, 0x1D, 0xBD, 0x60, 0x1D, 0xF0, 0x60, 0xBD, 0x30, 0x1D, 0x29, 0x03, 0xD0,
  0x59, 0xAC, 0x28, 0x14, 0xCC, 0x58, 0x0C, 0xB0, 0x51, 0xBD, 0xE0, 0x1C, 0x09, 0x01, 0xC9, 0x11,
  0xB0, 0x02, 0xA9, 0x11, 0xC9, 0xE0, 0x90, 0x02, 0xA9, 0xDF, 0x99, 0x70, 0x1D, 0xBD, 0xF0, 0x1C,
  0xC9, 0x11, 0xB0, 0x02, 0xA9, 0x11, 0xC9, 0xA0, 0x90, 0x02, 0xA9, 0x9F, 0x99, 0x90, 0x1D, 0xA9,
  0x00, 0x99, 0xB0, 0x1D, 0x99, 0xD0, 0x1D, 0x99, 0xF0, 0x1D, 0x99, 0x10, 0x1E, 0x99, 0x30, 0x1E,
  0xEE, 0x28, 0x14, 0xDE, 0x40, 0x1D, 0x10, 0x12, 0xA4, 0x00, 0xBE, 0xF0, 0x1C, 0xB9, 0xE0, 0x1C,
  0xA0, 0x0D, 0x20, 0x21, 0x4D, 0xA6, 0x00, 0x20, 0xF0, 0x6D, 0x4C, 0x12, 0x6A, 0x85, 0x06, 0x84,
  0x07, 0xE5, 0x07, 0xB0, 0x02, 0x49, 0xFF, 0xCD, 0x50, 0x0C, 0x08, 0x20, 0x36, 0x4C, 0xA9, 0x00,
  0xA4, 0x4E, 0xC0, 0x60, 0x90, 0x0B, 0xAD, 0x43, 0x0C, 0xC0, 0xB0, 0x90, 0x04, 0x49, 0xFF, 0x69,
  0x00, 0x28, 0xB0, 0x19, 0xA4, 0x4F, 0xCC, 0x56, 0x0C, 0xB0, 0x12, 0xA4, 0x06, 0xC4, 0x07, 0xA8,
  0xB0, 0x04, 0x10, 0x09, 0x90, 0x03, 0x30, 0x05, 0x18, 0x49, 0xFF, 0x69, 0x01, 0x60, 0xAE, 0x28,
  0x14, 0xCA, 0x10, 0x01, 0x60, 0xDE, 0xF0, 0x1D, 0x10, 0xF7, 0x86, 0x00, 0xAD, 0x4E, 0x0C, 0x9D,
  0xF0, 0x1D, 0xA4, 0x00, 0xBE, 0x90, 0x1D, 0xB9, 0x70, 0x1D, 0xA0, 0x0F, 0x20, 0x21, 0x4D, 0xA6,
  0x00, 0xBD, 0x70, 0x1D, 0x18, 0x7D, 0xB0, 0x1D, 0xC9, 0x08, 0x90, 0x04, 0xC9, 0xE8, 0x90, 0x10,
  0xBD, 0xB0, 0x1D, 0x49, 0xFF, 0x18, 0x69, 0x01, 0x9D, 0xB0, 0x1D, 0x20, 0x2F, 0x6C, 0x10, 0xE1,
  0x9D, 0x70, 0x1D, 0xBD, 0x90, 0x1D, 0x18, 0x7D, 0xD0, 0x1D, 0xC9, 0x08, 0x90, 0x04, 0xC9, 0xA8,
  0x90, 0x10, 0xBD, 0xD0, 0x1D, 0x49, 0xFF, 0x18, 0x69, 0x01, 0x9D, 0xD0, 0x1D, 0x20, 0x2F, 0x6C,
  0x10, 0xE1, 0x9D, 0x90, 0x1D, 0xBD, 0x10, 0x1E, 0x29, 0x03, 0x5D, 0x30, 0x1E, 0xA8, 0xB9, 0x20,
  0x09, 0x85, 0x06, 0xB9, 0x28, 0x09, 0x85, 0x07, 0xBC, 0x90, 0x1D, 0xBD, 0x70, 0x1D, 0xAA, 0x20,
  0xF6, 0x4C, 0xA6, 0x00, 0xDE, 0x10, 0x1E, 0x10, 0x26, 0xAD, 0x4B, 0x0C, 0x20, 0x4B, 0x4C, 0xA6,
  0x00, 0x9D, 0x10, 0x1E, 0xBD, 0x70, 0x1D, 0xAC, 0x00, 0x15, 0x20, 0x38, 0x6C, 0x9D, 0xB0, 0x1D,
  0xBD, 0x90, 0x1D, 0xAC, 0x01, 0x15, 0x20, 0x38, 0x6C, 0x9D, 0xD0, 0x1D, 0x20, 0x2F, 0x6C, 0x4C,
  0x7E, 0x6B, 0xBD, 0x30, 0x1E, 0x49, 0x03, 0x9D, 0x30, 0x1E, 0x60, 0x84, 0x06, 0xC5, 0x06, 0x08,
  0xAD, 0x44, 0x0C, 0x0A, 0x20, 0x4B, 0x4C, 0x38, 0xED, 0x44, 0x0C, 0x69, 0x00, 0x29, 0xFE, 0xA6,
  0x00, 0xA4, 0x4E, 0xCC, 0x54, 0x0C, 0xB0, 0x10, 0x28, 0xA8, 0xB0, 0x04, 0x10, 0x09, 0x90, 0x03,
  0x30, 0x05, 0x18, 0x49, 0xFF, 0x69, 0x01, 0x60, 0x28, 0x60, 0xAE, 0x29, 0x14, 0xCA, 0x10, 0x03,
  0x4C, 0x24, 0x6D, 0xDE, 0x90, 0x1E, 0x10, 0xF5, 0x86, 0x00, 0xAD, 0x4F, 0x0C, 0x9D, 0x90, 0x1E,
  0xA4, 0x00, 0xBE, 0x60, 0x1E, 0xB9, 0x50, 0x1E, 0xA0, 0x07, 0x20, 0xD0, 0x4C, 0xA6, 0x00, 0xBD,
  0x50, 0x1E, 0x18, 0x7D, 0x70, 0x1E, 0xBC, 0x70, 0x1E, 0x30, 0x05, 0x90, 0x10, 0x18, 0x90, 0x02,
  0xB0, 0x0B, 0x98, 0x49, 0xFF, 0x69, 0x01, 0x9D, 0x70, 0x1E, 0x4C, 0x8C, 0x6C, 0x9D, 0x50, 0x1E,
  0xBD, 0x60, 0x1E, 0x18, 0x7D, 0x80, 0x1E, 0xC9, 0xB8, 0x90, 0x0D, 0xBD, 0x80, 0x1E, 0x49, 0xFF,
  0x69, 0x00, 0x9D, 0x80, 0x1E, 0x4C, 0xAD, 0x6C, 0x9D, 0x60, 0x1E, 0xDE, 0xA0, 0x1E, 0x10, 0x06,
  0x20, 0xF2, 0x6C, 0x4C, 0x6A, 0x6C, 0xBD, 0xA0, 0x1E, 0x29, 0x01, 0xA8, 0xB9, 0x30, 0x09, 0x85,
  0x06, 0xB9, 0x34, 0x09, 0x85, 0x07, 0xBC, 0x60, 0x1E, 0xBD, 0x50, 0x1E, 0xAA, 0x20, 0xAC, 0x4C,
  0xA6, 0x00, 0x4C, 0x6A, 0x6C, 0x8A, 0x48, 0xCE, 0x29, 0x14, 0xBD, 0x51, 0x1E, 0x9D, 0x50, 0x1E,
  0xBD, 0x61, 0x1E, 0x9D, 0x60, 0x1E, 0xBD, 0x71, 0x1E, 0x9D, 0x70, 0x1E, 0xBD, 0x81, 0x1E, 0x9D,
  0x80, 0x1E, 0xBD, 0x91, 0x1E, 0x9D, 0x90, 0x1E, 0xBD, 0xA1, 0x1E, 0x9D, 0xA0, 0x1E, 0xE8, 0xEC,
  0x29, 0x14, 0x90, 0xD6, 0x68, 0xAA, 0x60, 0xCE, 0x2A, 0x14, 0x30, 0x01, 0x60, 0xAD, 0x2B, 0x14,
  0x20, 0x4B, 0x4C, 0x8D, 0x2A, 0x14, 0xAD, 0x28, 0x14, 0xF0, 0xF1, 0x20, 0x4B, 0x4C, 0xAA, 0xAC,
  0x29, 0x14, 0xC0, 0x10, 0xB0, 0xE6, 0x98, 0xF0, 0x08, 0x20, 0x36, 0x4C, 0xCD, 0x67, 0x0C, 0xB0,
  0x0C, 0xAD, 0x63, 0x0C, 0x8D, 0x2E, 0x14, 0xAD, 0x64, 0x0C, 0x8D, 0x2F, 0x14, 0xBD, 0x70, 0x1D,
  0x18, 0x69, 0x03, 0x99, 0x50, 0x1E, 0xBD, 0x90, 0x1D, 0x18, 0x69, 0x04, 0x99, 0x60, 0x1E, 0xAD,
  0x00, 0x15, 0x4A, 0x69, 0x40, 0x85, 0x02, 0xAD, 0x01, 0x15, 0x4A, 0x69, 0x50, 0x85, 0x03, 0xA5,
  0x4E, 0x10, 0x12, 0xA5, 0x02, 0x30, 0x07, 0xA9, 0x80, 0x38, 0xE5, 0x02, 0xB0, 0x05, 0x49, 0xFF,
  0x18, 0x69, 0x80, 0x85, 0x02, 0xA5, 0x4F, 0x10, 0x12, 0xA5, 0x03, 0x30, 0x07, 0xA9, 0xA0, 0x38,
  0xE5, 0x03, 0xB0, 0x05, 0x49, 0xFF, 0x18, 0x69, 0x60, 0x85, 0x03, 0xB9, 0x50, 0x1E, 0x85, 0x00,
  0xB9, 0x60, 0x1E, 0x85, 0x01, 0x46, 0x00, 0x46, 0x01, 0x46, 0x02, 0x46, 0x03, 0xA5, 0x02, 0xE5,
  0x00, 0xAA, 0xB0, 0x02, 0x49, 0xFF, 0xCD, 0x45, 0x0C, 0xB0, 0xEA, 0x86, 0x06, 0xA5, 0x03, 0xE5,
  0x01, 0xAA, 0xB0, 0x02, 0x49, 0xFF, 0xCD, 0x45, 0x0C, 0xB0, 0xDA, 0xA5, 0x06, 0x99, 0x70, 0x1E,
  0x8A, 0x99, 0x80, 0x1E, 0xA9, 0x00, 0x99, 0x90, 0x1E, 0xAD, 0x4C, 0x0C, 0x99, 0xA0, 0x1E, 0xEE,
  0x29, 0x14, 0x60, 0x8A, 0x48, 0xCE, 0x27, 0x14, 0xBD, 0xE1, 0x1C, 0x9D, 0xE0, 0x1C, 0xBD, 0xF1,
  0x1C, 0x9D, 0xF0, 0x1C, 0xBD, 0x01, 0x1D, 0x9D, 0x00, 0x1D, 0xBD, 0x11, 0x1D, 0x9D, 0x10, 0x1D,
  0xBD, 0x21, 0x1D, 0x9D, 0x20, 0x1D, 0xBD, 0x31, 0x1D, 0x9D, 0x30, 0x1D, 0xBD, 0x41, 0x1D, 0x9D,
  0x40, 0x1D, 0xBD, 0x51, 0x1D, 0x9D, 0x50, 0x1D, 0xBD, 0x61, 0x1D, 0x9D, 0x60, 0x1D, 0xE8, 0xEC,
  0x27, 0x14, 0x90, 0xC4, 0x68, 0xAA, 0x60, 0xAE, 0x27, 0x14, 0xA5, 0x1C, 0x38, 0xE9, 0x0D, 0xB0,
  0x02, 0xA9, 0x00, 0xA8, 0xA5, 0x1C, 0xCA, 0x10, 0x02, 0x18, 0x60, 0xDD, 0xE0, 0x1C, 0x90, 0xF6,
  0x98, 0xDD, 0xE0, 0x1C, 0xB0, 0xEE, 0xA5, 0x1D, 0xDD, 0xF0, 0x1C, 0x90, 0xE7, 0xE9, 0x0D, 0x90,
  0x05, 0xDD, 0xF0, 0x1C, 0xB0, 0xDE, 0xA5, 0xE8, 0xF0, 0x02, 0x38, 0x60, 0x8A, 0x48, 0xA8, 0xBE,
  0xF0, 0x1C, 0xB9, 0xE0, 0x1C, 0xA0, 0x0D, 0x20, 0x21, 0x4D, 0x68, 0x48, 0xA8, 0xB9, 0xE0, 0x1C,
  0x18, 0x69, 0x03, 0xAA, 0xB9, 0xF0, 0x1C, 0xA8, 0xA9, 0x06, 0x20, 0x27, 0x5A, 0x68, 0xAA, 0x20,
  0xF0, 0x6D, 0xA9, 0x03, 0xA2, 0xE8, 0x20, 0xD0, 0x4D, 0x38, 0x60, 0xAE, 0x28, 0x14, 0xA5, 0x1C,
  0x38, 0xE9, 0x0D, 0xB0, 0x02, 0xA9, 0x00, 0xA8, 0xA5, 0x1C, 0xCA, 0x10, 0x02, 0x18, 0x60, 0xDD,
  0x70, 0x1D, 0x90, 0xF6, 0x98, 0xDD, 0x70, 0x1D, 0xF0, 0x02, 0xB0, 0xEC, 0xA5, 0x1D, 0xDD, 0x90,
  0x1D, 0x90, 0xE5, 0xE9, 0x0F, 0x90, 0x05, 0xDD, 0x90, 0x1D, 0xB0, 0xDC, 0xA5, 0xE8, 0xF0, 0x02,
  0x38, 0x60, 0x8A, 0x48, 0xA8, 0xBE, 0x90, 0x1D, 0xB9, 0x70, 0x1D, 0xA0, 0x0F, 0x20, 0x21, 0x4D,
  0x68, 0xAA, 0x48, 0xBD, 0x90, 0x1D, 0x18, 0x69, 0x07, 0xA8, 0xBD, 0x70, 0x1D, 0x18, 0x69, 0x03,
  0xAA, 0xA9, 0xAA, 0x20, 0x74, 0x4F, 0x68, 0xAA, 0x20, 0x6F, 0x6F, 0xA9, 0x00, 0xA2, 0xC8, 0x20,
  0xD0, 0x4D, 0xAD, 0x46, 0x0C, 0x8D, 0x06, 0x14, 0xAD, 0x47, 0x0C, 0x8D, 0x22, 0x14, 0xAD, 0x48,
  0x0C, 0x8D, 0x23, 0x14, 0x38, 0x60, 0xAE, 0x29, 0x14, 0xA5, 0x1C, 0x38, 0xE9, 0x07, 0xB0, 0x02,
  0xA9, 0x00, 0xA8, 0xA5, 0x1C, 0xCA, 0x10, 0x02, 0x18, 0x60, 0xDD, 0x50, 0x1E, 0x90, 0xF6, 0x98,
  0xDD, 0x50, 0x1E, 0xB0, 0xEE, 0xA5, 0x1D, 0xDD, 0x60, 0x1E, 0x90, 0xE7, 0xE9, 0x07, 0x90, 0x05,
  0xDD, 0x60, 0x1E, 0xB0, 0xDE, 0xA5, 0xE8, 0xF0, 0x02, 0x38, 0x60, 0x8A, 0x48, 0xA8, 0xBE, 0x60,
  0x1E, 0xB9, 0x50, 0x1E, 0xA0, 0x07, 0x20, 0xD0, 0x4C, 0x68, 0xAA, 0x20, 0xF2, 0x6C, 0xAD, 0x49,
  0x0C, 0x8D, 0x13, 0x14, 0xA9, 0x01, 0x8D, 0x12, 0x14, 0xA9, 0x00, 0xA2, 0x19, 0x20, 0xD0, 0x4D,
  0x38, 0x60, 0x8A, 0x48, 0xCE, 0x28, 0x14, 0xBD, 0x71, 0x1D, 0x9D, 0x70, 0x1D, 0xBD, 0x91, 0x1D,
  0x9D, 0x90, 0x1D, 0xBD, 0xB1, 0x1D, 0x9D, 0xB0, 0x1D, 0xBD, 0xD1, 0x1D, 0x9D, 0xD0, 0x1D, 0xBD,
  0xF1, 0x1D, 0x9D, 0xF0, 0x1D, 0xBD, 0x11, 0x1E, 0x9D, 0x10, 0x1E, 0xBD, 0x31, 0x1E, 0x9D, 0x30,
  0x1E, 0xE8, 0xEC, 0x28, 0x14, 0x90, 0xD0, 0x68, 0xAA, 0x60, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x4C, 0x15, 0x70, 0x4C, 0xCA, 0x70, 0x4C, 0xF1, 0x76, 0x4C, 0xBD, 0x74, 0x4C,
  0xA3, 0x73, 0x4C, 0x8A, 0x78, 0x4C, 0x85, 0x75, 0xAD, 0x29, 0x0C, 0xAC, 0x18, 0x14, 0xD0, 0x02,
  0xA9, 0x00, 0x8D, 0x1C, 0x14, 0xCD, 0x21, 0x14, 0x90, 0x03, 0x8D, 0x21, 0x14, 0x88, 0x10, 0x12,
  0xA9, 0x00, 0x8D, 0x1B, 0x14, 0x8D, 0x1D, 0x14, 0x8D, 0x1E, 0x14, 0x8D, 0x19, 0x14, 0x8D, 0x26,
  0x14, 0x60, 0x20, 0x36, 0x4C, 0x08, 0x29, 0x01, 0x99, 0x40, 0x1B, 0x28, 0x30, 0x25, 0xA9, 0xDB,
  0x20, 0x4B, 0x4C, 0x18, 0x69, 0x0C, 0x29, 0xFE, 0x99, 0x20, 0x1B, 0x20, 0x36, 0x4C, 0x85, 0x00,
  0xA9, 0x20, 0x20, 0x4B, 0x4C, 0x18, 0x69, 0x0C, 0xA6, 0x00, 0x10, 0x02, 0x69, 0x7C, 0x99, 0x30,
  0x1B, 0xD0, 0x23, 0xA9, 0x9C, 0x20, 0x4B, 0x4C, 0x18, 0x69, 0x0C, 0x99, 0x30, 0x1B, 0x20, 0x36,
  0x4C, 0x85, 0x00, 0xA9, 0x37, 0x20, 0x4B, 0x4C, 0x18, 0x69, 0x0C, 0xA6, 0x00, 0x10, 0x02, 0x69,
  0xA4, 0x29, 0xFE, 0x99, 0x20, 0x1B, 0xAD, 0x0A, 0x14, 0xD0, 0x04, 0xA9, 0xFF, 0x30, 0x03, 0x20,
  0x4B, 0x4C, 0x99, 0x50, 0x1B, 0x98, 0x29, 0x03, 0x99, 0x60, 0x1B, 0xAD, 0x25, 0x0C, 0x20, 0x4B,
  0x4C, 0x99, 0x70, 0x1B, 0xAD, 0x26, 0x0C, 0x20, 0x4B, 0x4C, 0x99, 0x80, 0x1B, 0xA9, 0xFF, 0x99,
  0x90, 0x1B, 0x20, 0x36, 0x4C, 0x29, 0x01, 0x99, 0xA0, 0x1B, 0x4C, 0x2A, 0x70, 0xAE, 0x18, 0x14,
  0xAD, 0x1C, 0x14, 0xF0, 0x03, 0x4C, 0x11, 0x72, 0xCA, 0x10, 0x01, 0x60, 0xBD, 0x90, 0x1B, 0x30,
  0x05, 0x86, 0x00, 0x4C, 0x78, 0x72, 0xDE, 0x60, 0x1B, 0x10, 0xED, 0xAD, 0x24, 0x0C, 0x9D, 0x60,
  0x1B, 0x86, 0x00, 0xDE, 0x70, 0x1B, 0x10, 0x0D, 0xAD, 0x25, 0x0C, 0x9D, 0x70, 0x1B, 0x20, 0xD9,
  0x72, 0x98, 0x9D, 0x50, 0x1B, 0xA4, 0x00, 0xBE, 0x30, 0x1B, 0xB9, 0x20, 0x1B, 0xA0, 0x0D, 0x20,
  0x21, 0x4D, 0xA4, 0x00, 0xB9, 0x40, 0x1B, 0x49, 0x01, 0x99, 0x40, 0x1B, 0xBE, 0x50, 0x1B, 0x30,
  0x13, 0xEC, 0x0A, 0x14, 0x90, 0x1A, 0xA6, 0x00, 0x20, 0xD9, 0x72, 0x98, 0x9D, 0x50, 0x1B, 0xA4,
  0x00, 0xAA, 0x10, 0x0C, 0xAD, 0x00, 0x15, 0x85, 0x06, 0xAD, 0x01, 0x15, 0x85, 0x07, 0xD0, 0x0A,
  0xBD, 0x50, 0x19, 0x85, 0x06, 0xBD, 0x60, 0x19, 0x85, 0x07, 0xB9, 0x30, 0x1B, 0x38, 0xE5, 0x07,
  0xB0, 0x04, 0x49, 0xFF, 0x69, 0x01, 0xC9, 0x02, 0xB0, 0x04, 0xA5, 0x07, 0xD0, 0x0D, 0xB9, 0x30,
  0x1B, 0xC5, 0x07, 0xB0, 0x04, 0x69, 0x02, 0x90, 0x02, 0xE9, 0x02, 0x99, 0x30, 0x1B, 0xA9, 0x00,
  0x85, 0x01, 0xB9, 0x20, 0x1B, 0xC5, 0x06, 0xB0, 0x18, 0x69, 0x02, 0x99, 0x20, 0x1B, 0x69, 0x0F,
  0xC5, 0x06, 0x90, 0x1A, 0xB9, 0x30, 0x1B, 0xC5, 0x07, 0xD0, 0x13, 0x8A, 0x30, 0x10, 0x4C, 0x14,
  0x73, 0xE6, 0x01, 0xE9, 0x02, 0x99, 0x20, 0x1B, 0xE9, 0x09, 0xC5, 0x06, 0x90, 0xE6, 0xA5, 0x01,
  0x99, 0xA0, 0x1B, 0x0A, 0x19, 0x40, 0x1B, 0xAA, 0xBD, 0xE0, 0x08, 0x85, 0x06, 0xBD, 0xE8, 0x08,
  0x85, 0x07, 0xBE, 0x20, 0x1B, 0xB9, 0x30, 0x1B, 0xA8, 0x20, 0xF6, 0x4C, 0xA6, 0x00, 0xDE, 0x80,
  0x1B, 0x10, 0x4E, 0xAD, 0x1D, 0x14, 0x0A, 0xA8, 0xC0, 0x10, 0xB0, 0x45, 0xEE, 0x1D, 0x14, 0xBD,
  0x20, 0x1B, 0x18, 0x69, 0x05, 0x99, 0x70, 0x1C, 0x99, 0x71, 0x1C, 0xBD, 0x30, 0x1B, 0x99, 0x80,
  0x1C, 0x99, 0x81, 0x1C, 0x20, 0x8F, 0x76, 0xA5, 0x02, 0x99, 0x90, 0x1C, 0x99, 0x91, 0x1C, 0xA5,
  0x03, 0x99, 0xA0, 0x1C, 0x99, 0xA1, 0x1C, 0xAD, 0x2E, 0x0C, 0x99, 0xB0, 0x1C, 0x99, 0xB1, 0x1C,
  0xAD, 0x2E, 0x0C, 0x99, 0xC1, 0x1C, 0xAD, 0x26, 0x0C, 0x20, 0x4B, 0x4C, 0xA6, 0x00, 0x9D, 0x80,
  0x1B, 0x4C, 0xD5, 0x70, 0x0A, 0x0A, 0x85, 0x0A, 0xA0, 0x0D, 0x20, 0xA1, 0x50, 0xAE, 0x18, 0x14,
  0xCA, 0x10, 0x04, 0xCE, 0x1C, 0x14, 0x60, 0x86, 0x00, 0xBD, 0xA0, 0x1B, 0x0A, 0xA8, 0xB9, 0xE0,
  0x08, 0x85, 0x06, 0xB9, 0xE8, 0x08, 0x85, 0x07, 0xBC, 0x30, 0x1B, 0xBD, 0x20, 0x1B, 0xAA, 0x20,
  0x7C, 0x4C, 0xA9, 0x00, 0x85, 0x20, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05,
  0xA4, 0x04, 0xA1, 0x08, 0x21, 0x20, 0x91, 0x06, 0xE6, 0x08, 0xC8, 0xA1, 0x08, 0x21, 0x20, 0x91,
  0x06, 0xE6, 0x08, 0xC8, 0xA1, 0x08, 0x21, 0x20, 0x91, 0x06, 0xE6, 0x08, 0xE6, 0x20, 0xA4, 0x05,
  0x88, 0x10, 0xD3, 0x20, 0x6E, 0x4C, 0xA6, 0x00, 0x4C, 0x1D, 0x72, 0xAC, 0x3D, 0x0C, 0xBD, 0x40,
  0x1B, 0x49, 0x01, 0x9D, 0x40, 0x1B, 0xF0, 0x03, 0xAC, 0x3E, 0x0C, 0x84, 0x02, 0xBD, 0xA0, 0x1B,
  0x0A, 0xA8, 0xB9, 0xE0, 0x08, 0x85, 0x06, 0xB9, 0xE8, 0x08, 0x85, 0x07, 0xBC, 0x30, 0x1B, 0xBD,
  0x20, 0x1B, 0xAA, 0x20, 0x7C, 0x4C, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05,
  0xA4, 0x04, 0xA1, 0x08, 0x45, 0x02, 0x91, 0x06, 0xE6, 0x08, 0xC8, 0xA1, 0x08, 0x45, 0x02, 0x91,
  0x06, 0xE6, 0x08, 0xC8, 0xA1, 0x08, 0x45, 0x02, 0x91, 0x06, 0xE6, 0x08, 0xA4, 0x05, 0x88, 0x10,
  0xD5, 0xA6, 0x00, 0xDE, 0x90, 0x1B, 0x20, 0x6E, 0x4C, 0x4C, 0xD5, 0x70, 0xAC, 0x0A, 0x14, 0xD0,
  0x02, 0x88, 0x60, 0xA9, 0xFF, 0x85, 0x06, 0xA9, 0x00, 0x85, 0x07, 0x88, 0x30, 0x26, 0xB9, 0x50,
  0x19, 0xFD, 0x20, 0x1B, 0xB0, 0x02, 0x49, 0xFF, 0x85, 0x08, 0xB9, 0x60, 0x19, 0xFD, 0x30, 0x1B,
  0xB0, 0x02, 0x49, 0xFF, 0x65, 0x08, 0x90, 0x02, 0xA9, 0xFF, 0xC5, 0x06, 0xB0, 0xDD, 0x85, 0x06,
  0x84, 0x07, 0x90, 0xD7, 0xA4, 0x07, 0x60, 0xBD, 0x90, 0x19, 0x48, 0x8A, 0xA8, 0xBE, 0x60, 0x19,
  0xB9, 0x50, 0x19, 0xA0, 0x0A, 0x20, 0xD0, 0x4C, 0xA4, 0x00, 0xBE, 0x50, 0x1B, 0x20, 0x21, 0x5A,
  0xA4, 0x00, 0xAD, 0x1B, 0x14, 0x0A, 0x85, 0x01, 0xB9, 0x20, 0x1B, 0xBE, 0xA0, 0x1B, 0xD0, 0x11,
  0x18, 0x69, 0x12, 0xB0, 0x04, 0xC9, 0xF3, 0x90, 0x11, 0x8A, 0x49, 0x01, 0x99, 0xA0, 0x1B, 0x10,
  0xE7, 0x38, 0xE9, 0x0C, 0x90, 0xF3, 0xC9, 0x06, 0x90, 0xEF, 0xA6, 0x01, 0x9D, 0xB0, 0x1B, 0x9D,
  0xB1, 0x1B, 0xB9, 0x30, 0x1B, 0x9D, 0xD0, 0x1B, 0x9D, 0xD1, 0x1B, 0xB9, 0xA0, 0x1B, 0x9D, 0xF0,
  0x1B, 0x9D, 0xF1, 0x1B, 0x9D, 0x30, 0x1C, 0xA9, 0x70, 0x9D, 0x10, 0x1C, 0x9D, 0x11, 0x1C, 0x68,
  0x9D, 0x50, 0x1C, 0xAD, 0x2A, 0x0C, 0x38, 0xE9, 0x01, 0x9D, 0x31, 0x1C, 0xAD, 0x28, 0x0C, 0x8D,
  0x19, 0x14, 0x99, 0x90, 0x1B, 0x9D, 0x51, 0x1C, 0xAD, 0x23, 0x0C, 0x8D, 0x1A, 0x14, 0xEE, 0x1B,
  0x14, 0xA6, 0x00, 0x4C, 0xD5, 0x70, 0xAE, 0x18, 0x14, 0xA5, 0x1C, 0x38, 0xE9, 0x0D, 0xB0, 0x02,
  0xA9, 0x00, 0xA8, 0xA5, 0x1C, 0xCA, 0x10, 0x02, 0x18, 0x60, 0xDD, 0x20, 0x1B, 0x90, 0xF6, 0x98,
  0xDD, 0x20, 0x1B, 0xB0, 0xEE, 0xA5, 0x1D, 0xDD, 0x30, 0x1B, 0x90, 0xE7, 0xE9, 0x0D, 0x90, 0x05,
  0xDD, 0x30, 0x1B, 0xB0, 0xDE, 0xA5, 0xE8, 0xF0, 0x02, 0x38, 0x60, 0x86, 0x00, 0xA4, 0x00, 0xBE,
  0x30, 0x1B, 0xB9, 0x20, 0x1B, 0xA0, 0x0D, 0x20, 0x21, 0x4D, 0xA6, 0x00, 0xBD, 0x30, 0x1B, 0x18,
  0x69, 0x06, 0xA8, 0xBD, 0x20, 0x1B, 0x69, 0x03, 0xAA, 0xA9, 0xD5, 0x20, 0x74, 0x4F, 0xA6, 0x00,
  0xBD, 0x90, 0x1B, 0x30, 0x3F, 0xAD, 0x1B, 0x14, 0x0A, 0xA8, 0x88, 0x88, 0x30, 0x34, 0xB9, 0x51,
  0x1C, 0x30, 0xF7, 0x20, 0x79, 0x79, 0x90, 0xF2, 0x84, 0x01, 0xBE, 0xD0, 0x1B, 0xCA, 0xCA, 0xB9,
  0xB0, 0x1B, 0xA0, 0x0E, 0x20, 0xD0, 0x4C, 0xA6, 0x01, 0xBC, 0xD0, 0x1B, 0xBD, 0xB0, 0x1B, 0xAA,
  0x20, 0x24, 0x5A, 0xA6, 0x01, 0x20, 0x42, 0x79, 0xA6, 0x00, 0xA9, 0xFF, 0x9D, 0x90, 0x1B, 0x20,
  0x59, 0x74, 0xA6, 0x00, 0x20, 0x79, 0x74, 0xA9, 0x01, 0xA2, 0xF4, 0x20, 0xD0, 0x4D, 0xAD, 0x32,
  0x0C, 0x8D, 0x1E, 0x14, 0xAD, 0x31, 0x0C, 0x8D, 0x1F, 0x14, 0x38, 0x60, 0xAD, 0x19, 0x14, 0xF0,
  0x1A, 0xAE, 0x18, 0x14, 0xA9, 0x00, 0xCA, 0x30, 0x0F, 0xBC, 0x90, 0x1B, 0x30, 0xF8, 0xDD, 0x90,
  0x1B, 0xB0, 0xF3, 0xBD, 0x90, 0x1B, 0x10, 0xEE, 0x8D, 0x19, 0x14, 0x60, 0x8A, 0x48, 0xCE, 0x18,
  0x14, 0xBD, 0x21, 0x1B, 0x9D, 0x20, 0x1B, 0xBD, 0x31, 0x1B, 0x9D, 0x30, 0x1B, 0xBD, 0x41, 0x1B,
  0x9D, 0x40, 0x1B, 0xBD, 0x51, 0x1B, 0x9D, 0x50, 0x1B, 0xBD, 0x61, 0x1B, 0x9D, 0x60, 0x1B, 0xBD,
  0x71, 0x1B, 0x9D, 0x70, 0x1B, 0xBD, 0x81, 0x1B, 0x9D, 0x80, 0x1B, 0xBD, 0x91, 0x1B, 0x9D, 0x90,
  0x1B, 0xBD, 0xA1, 0x1B, 0x9D, 0xA0, 0x1B, 0xE8, 0xEC, 0x18, 0x14, 0x90, 0xC4, 0x68, 0xAA, 0x60,
  0xA9, 0x00, 0x85, 0xFC, 0x85, 0xFE, 0xAD, 0x1D, 0x14, 0x0A, 0xAA, 0xCA, 0xCA, 0x10, 0x01, 0x60,
  0x86, 0x00, 0x20, 0x28, 0x76, 0xA6, 0x00, 0x20, 0x46, 0x75, 0xBC, 0x80, 0x1C, 0xBD, 0x70, 0x1C,
  0x20, 0x1C, 0x4C, 0x85, 0x04, 0xA9, 0x00, 0x38, 0x2A, 0xCA, 0x10, 0xFC, 0x85, 0x0A, 0xA2, 0x02,
  0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05, 0xA4, 0x04, 0xA5, 0x0A, 0x11, 0x06,
  0x91, 0x06, 0xA5, 0x0A, 0x0A, 0x10, 0x03, 0xC8, 0xA9, 0x01, 0x11, 0x06, 0x91, 0x06, 0xA4, 0x05,
  0xC8, 0xCA, 0xD0, 0xDC, 0xA6, 0x00, 0xDE, 0xB0, 0x1C, 0x10, 0xB0, 0xA4, 0x00, 0x20, 0x8F, 0x76,
  0xA5, 0x02, 0x99, 0x90, 0x1C, 0x99, 0xC0, 0x1C, 0xA5, 0x03, 0x99, 0xA0, 0x1C, 0x99, 0xD0, 0x1C,
  0xAD, 0x2D, 0x0C, 0x20, 0x4B, 0x4C, 0x18, 0x6D, 0x2E, 0x0C, 0x99, 0xB0, 0x1C, 0xC8, 0xAD, 0x2E,
  0x0C, 0x99, 0xB0, 0x1C, 0xA6, 0x00, 0x4C, 0xC8, 0x74, 0xBD, 0x70, 0x1C, 0x18, 0x7D, 0x90, 0x1C,
  0xC9, 0x02, 0x90, 0x04, 0xC9, 0xFE, 0x90, 0x0D, 0xBD, 0x90, 0x1C, 0x49, 0xFF, 0x18, 0x69, 0x01,
  0x9D, 0x90, 0x1C, 0xD0, 0xE4, 0x9D, 0x70, 0x1C, 0xBD, 0x80, 0x1C, 0x18, 0x7D, 0xA0, 0x1C, 0xC9,
  0x02, 0x90, 0x04, 0xC9, 0xBE, 0x90, 0x0D, 0xBD, 0xA0, 0x1C, 0x49, 0xFF, 0x18, 0x69, 0x01, 0x9D,
  0xA0, 0x1C, 0xD0, 0xE4, 0x9D, 0x80, 0x1C, 0x60, 0xAD, 0x1D, 0x14, 0x0A, 0xAA, 0xE6, 0x1C, 0xE6,
  0x1D, 0xA5, 0x1C, 0x38, 0xE9, 0x04, 0xB0, 0x02, 0xA9, 0x00, 0xA8, 0xA5, 0x1C, 0xCA, 0xCA, 0x10,
  0x06, 0xC6, 0x1C, 0xC6, 0x1D, 0x18, 0x60, 0xDD, 0x70, 0x1C, 0x90, 0xF1, 0x98, 0xDD, 0x70, 0x1C,
  0xB0, 0xE9, 0xA5, 0x1D, 0xDD, 0x80, 0x1C, 0x90, 0xE2, 0xE9, 0x04, 0x90, 0x05, 0xDD, 0x80, 0x1C,
  0xB0, 0xD9, 0xA5, 0xE8, 0xF0, 0x02, 0x38, 0x60, 0x86, 0x00, 0xA9, 0x00, 0x85, 0xFC, 0x85, 0xFE,
  0xAD, 0x2E, 0x0C, 0x85, 0x02, 0x20, 0x28, 0x76, 0xC6, 0x02, 0x10, 0xF9, 0xA6, 0x00, 0x20, 0xEB,
  0x75, 0xA9, 0x00, 0xA2, 0x64, 0x20, 0xD0, 0x4D, 0xC6, 0x1C, 0xC6, 0x1D, 0x38, 0x60, 0x8A, 0x48,
  0xCE, 0x1D, 0x14, 0xAD, 0x1D, 0x14, 0x0A, 0x85, 0x06, 0xBD, 0x72, 0x1C, 0x9D, 0x70, 0x1C, 0xBD,
  0x82, 0x1C, 0x9D, 0x80, 0x1C, 0xBD, 0x92, 0x1C, 0x9D, 0x90, 0x1C, 0xBD, 0xA2, 0x1C, 0x9D, 0xA0,
  0x1C, 0xBD, 0xB2, 0x1C, 0x9D, 0xB0, 0x1C, 0xBD, 0xC2, 0x1C, 0x9D, 0xC0, 0x1C, 0xBD, 0xD2, 0x1C,
  0x9D, 0xD0, 0x1C, 0xE8, 0xE4, 0x06, 0x90, 0xD1, 0x68, 0xAA, 0x60, 0xA6, 0x00, 0xE8, 0xBD, 0xC0,
  0x1C, 0xF0, 0x05, 0xDE, 0xC0, 0x1C, 0x10, 0x59, 0xBC, 0x80, 0x1C, 0xBD, 0x70, 0x1C, 0x20, 0x1C,
  0x4C, 0x85, 0x04, 0xA9, 0xFF, 0x18, 0x2A, 0xCA, 0x10, 0xFC, 0x85, 0x0A, 0xA2, 0x02, 0xB1, 0xFC,
  0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07, 0x84, 0x05, 0xA4, 0x04, 0xA5, 0x0A, 0x31, 0x06, 0x91, 0x06,
  0xA5, 0x0A, 0x38, 0x2A, 0x30, 0x03, 0xC8, 0xA9, 0xFE, 0x31, 0x06, 0x91, 0x06, 0xA4, 0x05, 0xC8,
  0xCA, 0xD0, 0xDB, 0xA6, 0x00, 0xE8, 0x20, 0x46, 0x75, 0xDE, 0xB0, 0x1C, 0xD0, 0x13, 0xA9, 0x40,
  0x9D, 0xB0, 0x1C, 0xA4, 0x00, 0xB9, 0xC0, 0x1C, 0x9D, 0x90, 0x1C, 0xB9, 0xD0, 0x1C, 0x9D, 0xA0,
  0x1C, 0x60, 0xA2, 0x00, 0xA5, 0x4E, 0x4A, 0x90, 0x06, 0xE8, 0x4A, 0x90, 0x02, 0xCA, 0xCA, 0xA5,
  0x4F, 0x29, 0x03, 0x38, 0xE9, 0x02, 0x90, 0x02, 0x69, 0x00, 0x24, 0x4F, 0x10, 0x05, 0x86, 0x02,
  0xAA, 0xA5, 0x02, 0x86, 0x02, 0x85, 0x03, 0x20, 0x36, 0x4C, 0xCD, 0x2F, 0x0C, 0xB0, 0x34, 0xB9,
  0x70, 0x1C, 0xCD, 0x00, 0x15, 0xA5, 0x02, 0xB0, 0x08, 0x10, 0x0C, 0x49, 0xFF, 0x69, 0x01, 0x10,
  0x06, 0x30, 0x04, 0x49, 0xFF, 0x69, 0x00, 0x85, 0x02, 0xB9, 0x80, 0x1C, 0xCD, 0x01, 0x15, 0xA5,
  0x03, 0xB0, 0x08, 0x10, 0x0C, 0x49, 0xFF, 0x69, 0x01, 0x10, 0x06, 0x30, 0x04, 0x49, 0xFF, 0x69,
  0x00, 0x85, 0x03, 0x60, 0xAD, 0x1B, 0x14, 0x0A, 0xAA, 0xCA, 0xCA, 0x10, 0x03, 0x4C, 0xDD, 0x77,
  0xBD, 0x51, 0x1C, 0x30, 0x09, 0x86, 0x00, 0x20, 0x21, 0x78, 0xA6, 0x00, 0x10, 0xEB, 0xAD, 0x08,
  0x15, 0xD0, 0xE6, 0x86, 0x00, 0xBD, 0x31, 0x1C, 0x30, 0x06, 0xDE, 0x31, 0x1C, 0x4C, 0x20, 0x77,
  0x20, 0x78, 0x77, 0xA6, 0x00, 0x20, 0x9E, 0x77, 0xBD, 0x50, 0x1C, 0x0A, 0x0A, 0x1D, 0xF0, 0x1B,
  0xA8, 0xB9, 0xF0, 0x08, 0x85, 0x06, 0xB9, 0x00, 0x09, 0x85, 0x07, 0xBC, 0xD0, 0x1B, 0xBD, 0xB0,
  0x1B, 0xAA, 0x20, 0x59, 0x50, 0xA6, 0x00, 0xDE, 0x10, 0x1C, 0x10, 0xAD, 0xA9, 0x70, 0x9D, 0x10,
  0x1C, 0xBD, 0xB0, 0x1B, 0xAC, 0x00, 0x15, 0x20, 0x12, 0x78, 0x85, 0x02, 0x84, 0x03, 0xBD, 0xD0,
  0x1B, 0xAC, 0x01, 0x15, 0x20, 0x12, 0x78, 0xC5, 0x02, 0xB0, 0x04, 0xA5, 0x03, 0x10, 0x03, 0x98,
  0x09, 0x02, 0x9D, 0xF0, 0x1B, 0x9D, 0x30, 0x1C, 0x4C, 0xF6, 0x76, 0xA6, 0x00, 0xE8, 0x20, 0x9E,
  0x77, 0x8A, 0xA8, 0xBE, 0xD0, 0x1B, 0xB9, 0xB0, 0x1B, 0xA0, 0x0A, 0x20, 0xD0, 0x4C, 0xA6, 0x00,
  0xDE, 0x11, 0x1C, 0x10, 0x0B, 0xA9, 0x70, 0x9D, 0x11, 0x1C, 0xBD, 0x30, 0x1C, 0x9D, 0xF1, 0x1B,
  0x60, 0xBC, 0xF0, 0x1B, 0xBD, 0xB0, 0x1B, 0x18, 0x79, 0x3E, 0x0D, 0xC9, 0x06, 0xB0, 0x04, 0xA9,
  0x00, 0x10, 0x06, 0xC9, 0xF3, 0x90, 0x07, 0xA9, 0x01, 0x9D, 0xF0, 0x1B, 0x10, 0xE3, 0x9D, 0xB0,
  0x1B, 0xBD, 0xD0, 0x1B, 0x18, 0x79, 0x42, 0x0D, 0xC9, 0x06, 0xB0, 0x04, 0xA9, 0x02, 0x10, 0x06,
  0xC9, 0xB0, 0x90, 0x08, 0xA9, 0x03, 0x9D, 0xF0, 0x1B, 0xA8, 0x10, 0xE5, 0x9D, 0xD0, 0x1B, 0x60,
  0xCE, 0x26, 0x14, 0x10, 0x2F, 0xAD, 0x2B, 0x0C, 0x20, 0x4B, 0x4C, 0x85, 0x06, 0xAD, 0x2A, 0x0C,
  0x18, 0x69, 0x01, 0x0A, 0x65, 0x06, 0x8D, 0x26, 0x14, 0xAD, 0x1B, 0x14, 0x0A, 0xAA, 0xCA, 0xCA,
  0x30, 0x12, 0xBD, 0x31, 0x1C, 0x10, 0xF7, 0xA9, 0x00, 0x9D, 0x10, 0x1C, 0xAD, 0x2A, 0x0C, 0x9D,
  0x11, 0x1C, 0x10, 0xEA, 0x60, 0x84, 0x01, 0xA0, 0x01, 0x38, 0xE5, 0x01, 0xB0, 0x05, 0x49, 0xFF,
  0x69, 0x01, 0x88, 0x60, 0xAC, 0x08, 0x15, 0xB9, 0x41, 0x0C, 0x85, 0x02, 0xDE, 0x51, 0x1C, 0x10,
  0x10, 0xBD, 0xD0, 0x1B, 0x38, 0xE9, 0x02, 0xBC, 0xB0, 0x1B, 0xAA, 0x98, 0xA0, 0x0E, 0x4C, 0xD0,
  0x4C, 0xBD, 0x50, 0x1C, 0x0A, 0x1D, 0xF0, 0x1B, 0x0A, 0xA8, 0xB9, 0xA0, 0x08, 0x85, 0x06, 0xB9,
  0xB0, 0x08, 0x85, 0x07, 0xBD, 0xD0, 0x1B, 0x38, 0xE9, 0x02, 0xA4, 0x4E, 0x10, 0x02, 0xE9, 0xFC,
  0xA8, 0xBD, 0xB0, 0x1B, 0xAA, 0x20, 0x7C, 0x4C, 0xB1, 0xFC, 0x85, 0x06, 0xB1, 0xFE, 0x85, 0x07,
  0x84, 0x05, 0xA4, 0x04, 0xA1, 0x08, 0x45, 0x02, 0x91, 0x06, 0xE6, 0x08, 0xC8, 0xA1, 0x08, 0x45,
  0x02, 0x91, 0x06, 0xE6, 0x08, 0xA4, 0x05, 0x88, 0x10, 0xDE, 0x4C, 0x6E, 0x4C, 0xAD, 0x1B, 0x14,
  0x0A, 0xAA, 0xA5, 0x1C, 0x38, 0xE9, 0x07, 0xB0, 0x02, 0xA9, 0x00, 0xA8, 0xA5, 0x1C, 0xCA, 0xCA,
  0x10, 0x02, 0x18, 0x60, 0xDD, 0xB0, 0x1B, 0x90, 0xF5, 0x98, 0xDD, 0xB0, 0x1B, 0xB0, 0xED, 0xA5,
  0x1D, 0xDD, 0xD0, 0x1B, 0x90, 0xE6, 0xE9, 0x0A, 0x90, 0x05, 0xDD, 0xD0, 0x1B, 0xB0, 0xDD, 0xA5,
  0xE8, 0xF0, 0x02, 0x38, 0x60, 0x86, 0x00, 0xBD, 0x51, 0x1C, 0x10, 0x31, 0xAD, 0x2A, 0x0C, 0x85,
  0x01, 0x20, 0x78, 0x77, 0xC6, 0x01, 0x10, 0xF9, 0xBD, 0xD0, 0x1B, 0x18, 0x69, 0x05, 0xA8, 0xBD,
  0xB0, 0x1B, 0xAA, 0xA9, 0x7F, 0x20, 0x74, 0x4F, 0xAD, 0x37, 0x0C, 0x8D, 0x06, 0x14, 0xAD, 0x38,
  0x0C, 0x8D, 0x22, 0x14, 0xAD, 0x39, 0x0C, 0x8D, 0x23, 0x14, 0x38, 0xB0, 0x36, 0xA4, 0x00, 0xBE,
  0xD0, 0x1B, 0xCA, 0xCA, 0xB9, 0xB0, 0x1B, 0xA0, 0x0E, 0x20, 0xD0, 0x4C, 0xA6, 0x00, 0xBC, 0xD0,
  0x1B, 0xBD, 0xB0, 0x1B, 0xAA, 0x20, 0x24, 0x5A, 0xA4, 0x00, 0xAE, 0x18, 0x14, 0xCA, 0x30, 0x12,
  0xBD, 0x90, 0x1B, 0x30, 0xF8, 0x20, 0x79, 0x79, 0x90, 0xF3, 0xA9, 0xFF, 0x9D, 0x90, 0x1B, 0x20,
  0x59, 0x74, 0x18, 0x08, 0xA6, 0x00, 0x20, 0x42, 0x79, 0x28, 0x90, 0x07, 0xA9, 0x00, 0xA2, 0x64,
  0x20, 0xD0, 0x4D, 0x38, 0x60, 0x8A, 0x48, 0xCE, 0x1B, 0x14, 0xAD, 0x1B, 0x14, 0x0A, 0x85, 0x06,
  0xBD, 0xB2, 0x1B, 0x9D, 0xB0, 0x1B, 0xBD, 0xD2, 0x1B, 0x9D, 0xD0, 0x1B, 0xBD, 0xF2, 0x1B, 0x9D,
  0xF0, 0x1B, 0xBD, 0x12, 0x1C, 0x9D, 0x10, 0x1C, 0xBD, 0x32, 0x1C, 0x9D, 0x30, 0x1C, 0xBD, 0x52,
  0x1C, 0x9D, 0x50, 0x1C, 0xE8, 0xE4, 0x06, 0x90, 0xD7, 0x68, 0xAA, 0x60, 0xBD, 0x30, 0x1B, 0xD9,
  0xD0, 0x1B, 0xD0, 0x15, 0xBD, 0x20, 0x1B, 0x18, 0x69, 0x12, 0xD9, 0xB0, 0x1B, 0xF0, 0x08, 0x38,
  0xE9, 0x1E, 0xD9, 0xB0, 0x1B, 0xD0, 0x02, 0x38, 0x60, 0x18, 0x60, 0x10, 0x10, 0x10, 0x10, 0x10,
  0x10, 0x10, 0x9C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
  0x10, 0x10, 0x9C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
  0x10, 0x10, 0x5C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
  0x10, 0x10, 0x90, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
  0x10, 0x10, 0x5C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
  0x10, 0x10, 0x14, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
  0x10, 0x10, 0x50, 0x02, 0x0A, 0x00, 0x82, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x14, 0x82, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x28, 0x82, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x3C, 0x82, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x50, 0x82, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x64, 0x82, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x78, 0x82, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0D, 0x8C, 0x82, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0D, 0xB3, 0x82, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0xDA, 0x82, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x00, 0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x14, 0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x28, 0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0E, 0x3C, 0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0E, 0x66, 0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0E, 0x90, 0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0E, 0xBA, 0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0E, 0x00, 0x84, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0E, 0x2A, 0x84, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0D, 0x54, 0x84, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0D, 0x7B, 0x84, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0D, 0xA2, 0x84, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0D, 0xC9, 0x84, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x07, 0xF0, 0x84, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x07, 0x00, 0x85, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0D, 0x0E, 0x85, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0D, 0x35, 0x85, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0D, 0x5C, 0x85, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0D, 0x83, 0x85, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0C, 0xAA, 0x85, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0C, 0xCE, 0x85, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0C, 0x00, 0x86, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0C, 0x24, 0x86, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0C, 0x48, 0x86, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0C, 0x6C, 0x86, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0C, 0x90, 0x86, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0D, 0xB4, 0x86, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0D, 0x00, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0D, 0x27, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x4E, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x62, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x76, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x8A, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x9E, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0xB2, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0F, 0xC6, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0F, 0x00, 0x88, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0F, 0x2D, 0x88, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0F, 0x5A, 0x88, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0D, 0x87, 0x88, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0D, 0xAE, 0x88, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0D, 0xD5, 0x88, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0D, 0x00, 0x89, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x07, 0x27, 0x89, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x07, 0x35, 0x89, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x00, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x14, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x28, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x3C, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x50, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x64, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0A, 0x78, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x18, 0x00, 0x08, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0x08,
  0x00, 0x1C, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x14, 0x00, 0x1C, 0x00, 0x3E,
  0x00, 0x1C, 0x00, 0x08, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x08,
  0x00, 0x1C, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0x08, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x1E, 0x00, 0x14, 0x00, 0x1C, 0x00, 0x3E, 0x00, 0x1C, 0x00, 0x08, 0x00, 0x1C, 0x00, 0x1C,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0x00, 0x77, 0x00, 0x63, 0x00, 0x63, 0x00, 0x63, 0x00, 0x77,
  0x00, 0x63, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x73, 0x00, 0x77, 0x00, 0x63, 0x00, 0x63,
  0x00, 0x63, 0x00, 0x77, 0x00, 0x63, 0x00, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x49, 0x00, 0x6B,
  0x00, 0x22, 0x00, 0x22, 0x00, 0x41, 0x00, 0x77, 0x00, 0x63, 0x00, 0x63, 0x00, 0x00, 0x00, 0xD0,
  0x82, 0x80, 0xD4, 0x8A, 0x80, 0x84, 0x88, 0x80, 0xC5, 0xA8, 0x80, 0x80, 0x80, 0x80, 0xDD, 0xBB,
  0x80, 0xD5, 0xAA, 0x80, 0xDD, 0xBB, 0x80, 0x80, 0x80, 0x80, 0xC5, 0xA8, 0x80, 0x84, 0x88, 0x80,
  0xD4, 0x8A, 0x80, 0xD0, 0x82, 0x80, 0x55, 0x2A, 0x00, 0x33, 0x33, 0x00, 0x65, 0x29, 0x00, 0x49,
  0x24, 0x00, 0x00, 0x00, 0x00, 0x5D, 0x3B, 0x00, 0x55, 0x2A, 0x00, 0x5D, 0x3B, 0x00, 0x00, 0x00,
  0x00, 0x49, 0x24, 0x00, 0x65, 0x29, 0x00, 0x33, 0x33, 0x00, 0x55, 0x2A, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x03, 0x05, 0x43, 0x89, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x05, 0x52, 0x89, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x05, 0x61, 0x89, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x05, 0x70, 0x89, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x05, 0x7F, 0x89, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0C, 0x8E, 0x89, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0C, 0xA6, 0x89, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0C, 0xBE, 0x89, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x02, 0x0C, 0xD6, 0x89, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0D, 0x8C, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x03, 0x0D, 0xB3, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDA, 0x7E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x08, 0x28, 0xC0, 0x05, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x22, 0x2D, 0x00, 0x8A, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C,
  0x00, 0x0C, 0x0C, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x14, 0x3E, 0x14, 0x3E,
  0x14, 0x14, 0x00, 0x3E, 0x41, 0x59, 0x45, 0x45, 0x59, 0x41, 0x3E, 0x26, 0x26, 0x10, 0x08, 0x04,
  0x32, 0x32, 0x00, 0x0C, 0x14, 0x0C, 0x06, 0x15, 0x25, 0x1E, 0x04, 0x08, 0x08, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x10, 0x08, 0x0C, 0x0C, 0x0C, 0x0C, 0x08, 0x10, 0x04, 0x08, 0x18, 0x18, 0x18,
  0x18, 0x08, 0x04, 0x08, 0x2A, 0x1C, 0x7F, 0x1C, 0x2A, 0x08, 0x00, 0x00, 0x08, 0x08, 0x3E, 0x08,
  0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x0C, 0x00, 0x00, 0x00, 0x3E, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04,
  0x02, 0x01, 0x00, 0x3E, 0x22, 0x22, 0x32, 0x32, 0x32, 0x3E, 0x00, 0x08, 0x08, 0x08, 0x0C, 0x0C,
  0x0C, 0x0C, 0x00, 0x3C, 0x34, 0x30, 0x3E, 0x06, 0x06, 0x3E, 0x00, 0x3E, 0x30, 0x30, 0x3E, 0x20,
  0x20, 0x3E, 0x00, 0x06, 0x06, 0x26, 0x26, 0x3E, 0x20, 0x20, 0x00, 0x1E, 0x16, 0x06, 0x3E, 0x30,
  0x30, 0x3E, 0x00, 0x02, 0x02, 0x02, 0x3E, 0x32, 0x32, 0x3E, 0x00, 0x3E, 0x30, 0x30, 0x10, 0x08,
  0x08, 0x08, 0x00, 0x3E, 0x26, 0x26, 0x3E, 0x32, 0x32, 0x3E, 0x00, 0x3E, 0x26, 0x26, 0x3E, 0x30,
  0x30, 0x30, 0x00, 0x00, 0x0C, 0x0C, 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
  0x18, 0x18, 0x0C, 0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0x00, 0x55, 0x2A, 0x55, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00, 0x3E, 0x32, 0x30, 0x3C, 0x0C,
  0x00, 0x0C, 0x0C, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x24, 0x24, 0x3E, 0x26,
  0x26, 0x26, 0x00, 0x1E, 0x16, 0x16, 0x3E, 0x26, 0x26, 0x3E, 0x00, 0x3E, 0x26, 0x06, 0x06, 0x06,
  0x26, 0x3E, 0x00, 0x1E, 0x26, 0x26, 0x26, 0x26, 0x26, 0x1E, 0x00, 0x3E, 0x06, 0x06, 0x1E, 0x06,
  0x06, 0x3E, 0x00, 0x3E, 0x06, 0x06, 0x1E, 0x06, 0x06, 0x06, 0x00, 0x3E, 0x06, 0x06, 0x36, 0x26,
  0x26, 0x3E, 0x00, 0x26, 0x26, 0x26, 0x3E, 0x26, 0x26, 0x26, 0x00, 0x1E, 0x0C, 0x0C, 0x0C, 0x0C,
  0x0C, 0x1E, 0x00, 0x30, 0x30, 0x30, 0x30, 0x30, 0x36, 0x3E, 0x00, 0x36, 0x36, 0x16, 0x0E, 0x16,
  0x36, 0x36, 0x00, 0x04, 0x04, 0x04, 0x06, 0x06, 0x06, 0x3E, 0x00, 0x22, 0x36, 0x2A, 0x22, 0x22,
  0x22, 0x22, 0x00, 0x26, 0x2E, 0x2E, 0x36, 0x26, 0x26, 0x26, 0x00, 0x3E, 0x26, 0x26, 0x26, 0x22,
  0x22, 0x3E, 0x00, 0x3E, 0x26, 0x26, 0x3E, 0x06, 0x06, 0x06, 0x00, 0x3E, 0x22, 0x22, 0x22, 0x32,
  0x32, 0x3E, 0x70, 0x3E, 0x22, 0x22, 0x3E, 0x12, 0x22, 0x22, 0x00, 0x3E, 0x26, 0x06, 0x3E, 0x30,
  0x32, 0x3E, 0x00, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x26, 0x26, 0x26, 0x26, 0x26,
  0x26, 0x3E, 0x00, 0x36, 0x36, 0x36, 0x36, 0x14, 0x1C, 0x08, 0x00, 0x22, 0x22, 0x22, 0x22, 0x2A,
  0x36, 0x22, 0x00, 0x36, 0x36, 0x14, 0x08, 0x14, 0x36, 0x36, 0x00, 0x36, 0x36, 0x36, 0x14, 0x08,
  0x08, 0x08, 0x00, 0x3E, 0x26, 0x10, 0x08, 0x04, 0x32, 0x3E, 0x00, 0x1E, 0x06, 0x06, 0x06, 0x06,
  0x06, 0x06, 0x1E, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x1E, 0x18, 0x18, 0x18, 0x18,
  0x18, 0x1E, 0x00, 0x08, 0x1C, 0x2A, 0x08, 0x08, 0x08, 0x08, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x36, 0x00, 0x14, 0x00, 0x14, 0x00, 0x14, 0x00, 0x5D, 0x00, 0x5D, 0x00, 0x3E,
  0x00, 0x08, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0x18, 0x00, 0x08, 0x00, 0x08, 0x00, 0x1C, 0x00, 0x1C,
  0x00, 0x1C, 0x00, 0x1C, 0x00, 0x08, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0x3C, 0x00, 0x14, 0x00, 0x14,
  0x00, 0x1C, 0x00, 0x3E, 0x00, 0x3E, 0x00, 0x1C, 0x00, 0x08, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0x0C,
  0x00, 0x08, 0x00, 0x08, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0x08, 0x00, 0x1C,
  0x00, 0x1C, 0x00, 0x1E, 0x00, 0x14, 0x00, 0x14, 0x00, 0x1C, 0x00, 0x3E, 0x00, 0x3E, 0x00, 0x1C,
  0x00, 0x08, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0x06, 0x00, 0x34, 0x00, 0x14, 0x00, 0x14, 0x00, 0x5D,
  0x00, 0x5D, 0x00, 0x3E, 0x00, 0x08, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0x30, 0x00, 0x16, 0x00, 0x14,
  0x00, 0x14, 0x00, 0x5D, 0x00, 0x5D, 0x00, 0x3E, 0x00, 0x08, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0x8F,
  0x80, 0x80, 0x8A, 0x9F, 0x80, 0x8A, 0x85, 0x80, 0xAA, 0x85, 0x80, 0xAA, 0x85, 0x80, 0xA9, 0x89,
  0x80, 0xF9, 0x89, 0x80, 0xAD, 0x8B, 0x80, 0xF6, 0x86, 0x80, 0xA0, 0x80, 0x80, 0xA8, 0x81, 0x80,
  0xFC, 0x83, 0x80, 0xA8, 0x81, 0x80, 0x80, 0x8F, 0x80, 0x8F, 0x85, 0x80, 0x8A, 0x85, 0x80, 0xAA,
  0x85, 0x80, 0xAA, 0x85, 0x80, 0xA9, 0x89, 0x80, 0xF9, 0x89, 0x80, 0xAD, 0x8B, 0x80, 0xF6, 0x86,
  0x80, 0xA0, 0x80, 0x80, 0xA8, 0x81, 0x80, 0xFC, 0x83, 0x80, 0xA8, 0x81, 0x80, 0x18, 0x00, 0x08,
  0x00, 0x08, 0x00, 0x1C, 0x00, 0x1C, 0x00, 0x1F, 0x00, 0x1F, 0x00, 0x0E, 0x00, 0x1E, 0x00, 0x1C,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x3C, 0x00, 0x14, 0x00, 0x14, 0x00, 0x1C, 0x00, 0x3E, 0x00, 0x3F, 0x00, 0x1F,
  0x00, 0x0E, 0x00, 0x1E, 0x00, 0x1C, 0x00, 0x0C, 0x00, 0x08, 0x00, 0x08, 0x00, 0x1C, 0x00, 0x1C,
  0x00, 0x7C, 0x00, 0x7C, 0x00, 0x38, 0x00, 0x3C, 0x00, 0x1C, 0x00, 0x1E, 0x00, 0x14, 0x00, 0x14,
  0x00, 0x1C, 0x00, 0x3E, 0x00, 0x7E, 0x00, 0x7C, 0x00, 0x38, 0x00, 0x3C, 0x00, 0x1C, 0x00, 0x14,
  0x10, 0x00, 0x02, 0x28, 0x00, 0x04, 0x04, 0x00, 0x08, 0x02, 0x00, 0x54, 0x0A, 0x00, 0x55, 0x2A,
  0x00, 0x56, 0x12, 0x00, 0x54, 0x08, 0x00, 0x14, 0x08, 0x00, 0x14, 0x0A, 0x00, 0x54, 0x0A, 0x00,
  0x60, 0x01, 0x00, 0x60, 0x01, 0x00, 0xD0, 0x82, 0x80, 0x40, 0x02, 0x00, 0x40, 0x00, 0x00, 0x40,
  0x00, 0x00, 0x40, 0x00, 0x00, 0x54, 0x0A, 0x00, 0x55, 0x2A, 0x00, 0x52, 0x1A, 0x00, 0x44, 0x0A,
  0x00, 0x04, 0x0A, 0x00, 0x14, 0x0A, 0x00, 0x54, 0x0A, 0x00, 0x60, 0x01, 0x00, 0x60, 0x01, 0x00,
  0xD0, 0x82, 0x80, 0x02, 0x0A, 0x00, 0x05, 0x10, 0x00, 0x08, 0x08, 0x00, 0x10, 0x04, 0x00, 0x54,
  0x0A, 0x00, 0x55, 0x2A, 0x00, 0x52, 0x1A, 0x00, 0x44, 0x0A, 0x00, 0x04, 0x0A, 0x00, 0x14, 0x0A,
  0x00, 0x54, 0x0A, 0x00, 0x60, 0x01, 0x00, 0x60, 0x01, 0x00, 0xD0, 0x82, 0x80, 0x50, 0x00, 0x00,
  0x40, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x00, 0x00, 0x54, 0x0A, 0x00, 0x55, 0x2A, 0x00, 0x56,
  0x12, 0x00, 0x54, 0x08, 0x00, 0x14, 0x08, 0x00, 0x14, 0x0A, 0x00, 0x54, 0x0A, 0x00, 0x60, 0x01,
  0x00, 0x60, 0x01, 0x00, 0xD0, 0x82, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x01, 0x00, 0x28, 0x01, 0x00, 0x20, 0x01, 0x00, 0x57,
  0x3A, 0x00, 0x57, 0x3A, 0x00, 0x57, 0x3A, 0x00, 0x53, 0x32, 0x00, 0x53, 0x32, 0x00, 0x53, 0x32,
  0x00, 0x5F, 0x3E, 0x00, 0x60, 0x01, 0x00, 0x60, 0x01, 0x00, 0xD0, 0x82, 0x80, 0x28, 0x00, 0x00,
  0x20, 0x00, 0x00, 0x20, 0x05, 0x00, 0x20, 0x01, 0x00, 0x57, 0x3A, 0x00, 0x57, 0x3A, 0x00, 0x57,
  0x3A, 0x00, 0x53, 0x32, 0x00, 0x53, 0x32, 0x00, 0x53, 0x32, 0x00, 0x5F, 0x3E, 0x00, 0x60, 0x01,
  0x00, 0x60, 0x01, 0x00, 0xD0, 0x82, 0x80, 0xD0, 0x82, 0x80, 0x94, 0x8A, 0x80, 0x84, 0x88, 0x80,
  0x85, 0xA8, 0x80, 0x81, 0xA0, 0x80, 0xC1, 0xA0, 0x80, 0xD1, 0xA2, 0x80, 0xC1, 0xA0, 0x80, 0x81,
  0xA0, 0x80, 0x85, 0xA8, 0x80, 0x84, 0x88, 0x80, 0x94, 0x8A, 0x80, 0xD0, 0x82, 0x80, 0x80, 0x80,
  0x80, 0xD0, 0x82, 0x80, 0x94, 0x8A, 0x80, 0x84, 0x88, 0x80, 0x85, 0xA8, 0x80, 0x81, 0xA0, 0x80,
  0xC1, 0xA0, 0x80, 0x81, 0xA0, 0x80, 0x85, 0xA8, 0x80, 0x84, 0x88, 0x80, 0x94, 0x8A, 0x80, 0xD0,
  0x82, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xC0, 0x80, 0x80, 0xD0, 0x82,
  0x80, 0x94, 0x8A, 0x80, 0x84, 0x88, 0x80, 0x84, 0x88, 0x80, 0x84, 0x88, 0x80, 0x94, 0x8A, 0x80,
  0xD0, 0x82, 0x80, 0xC0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x5E, 0x07, 0x00, 0x08,
  0x01, 0x00, 0x28, 0x01, 0x00, 0x28, 0x01, 0x00, 0x23, 0x0C, 0x00, 0x7E, 0x07, 0x00, 0x23, 0x0C,
  0x00, 0x50, 0x00, 0x00, 0x56, 0x06, 0x00, 0x2C, 0x03, 0x00, 0x58, 0x01, 0x00, 0x70, 0x00, 0x00,
  0x20, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x7F, 0x00, 0x08, 0x00, 0x08, 0x00, 0x08,
  0x00, 0x00, 0x00, 0x41, 0x00, 0x22, 0x00, 0x14, 0x00, 0x08, 0x00, 0x14, 0x00, 0x22, 0x00, 0x41,
  0x00, 0x00, 0x14, 0x00, 0x00, 0x04, 0x00, 0x00, 0x04, 0x00, 0x00, 0x34, 0x00, 0x00, 0x04, 0x00,
  0xF0, 0x8A, 0x80, 0xDC, 0xAA, 0x80, 0xF7, 0xBE, 0x80, 0xD5, 0xAB, 0x80, 0xF7, 0xAA, 0x80, 0xDD,
  0xBB, 0x80, 0xD4, 0xAE, 0x80, 0xD0, 0x8B, 0x80, 0x40, 0x32, 0x00, 0x00, 0x09, 0x00, 0x00, 0x04,
  0x00, 0x00, 0x34, 0x00, 0x00, 0x04, 0x00, 0xA0, 0x8A, 0x80, 0xC8, 0xAA, 0x80, 0xA2, 0xBE, 0x80,
  0x95, 0xA9, 0x80, 0xA2, 0xAA, 0x80, 0x89, 0x91, 0x80, 0xD4, 0xA4, 0x80, 0x90, 0x89, 0x80, 0x0A,
  0x00, 0x00, 0x08, 0x00, 0x00, 0x08, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x08, 0x00, 0x00, 0xD4, 0x83,
  0x80, 0xD5, 0x8E, 0x80, 0xDF, 0xBB, 0x80, 0xF5, 0xAA, 0x80, 0xD5, 0xBB, 0x80, 0xF7, 0xAE, 0x80,
  0xDD, 0x8A, 0x80, 0xF4, 0x82, 0x80, 0x53, 0x00, 0x00, 0x24, 0x00, 0x00, 0x08, 0x00, 0x00, 0x0B,
  0x00, 0x00, 0x08, 0x00, 0x00, 0x94, 0x81, 0x80, 0xD5, 0x84, 0x80, 0x9F, 0x91, 0x80, 0xA5, 0xAA,
  0x80, 0x95, 0x91, 0x80, 0xA2, 0xA4, 0x80, 0xC9, 0x8A, 0x80, 0xA4, 0x82, 0x80, 0x7E, 0x07, 0x00,
  0x23, 0x0C, 0x00, 0x2E, 0x07, 0x00, 0x2E, 0x07, 0x00, 0x0B, 0x0D, 0x00, 0x0B, 0x0D, 0x00, 0x0B,
  0x0D, 0x00, 0x06, 0x06, 0x00, 0x5C, 0x03, 0x00, 0x0C, 0x03, 0x00, 0x0C, 0x03, 0x00, 0x78, 0x01,
  0x00, 0x70, 0x01, 0x00, 0x18, 0x03, 0x00, 0x58, 0x01, 0x00, 0x58, 0x01, 0x00, 0x0C, 0x03, 0x00,
  0x0C, 0x03, 0x00, 0x0C, 0x03, 0x00, 0x0C, 0x03, 0x00, 0x58, 0x01, 0x00, 0x0C, 0x03, 0x00, 0x0C,
  0x03, 0x00, 0x78, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x78, 0x03, 0x00, 0x0C, 0x06, 0x00, 0x2C, 0x03, 0x00, 0x2C, 0x03, 0x00, 0x0C,
  0x03, 0x00, 0x06, 0x06, 0x00, 0x06, 0x06, 0x00, 0x0C, 0x03, 0x00, 0x58, 0x01, 0x00, 0x0C, 0x03,
  0x00, 0x0C, 0x03, 0x00, 0x78, 0x01, 0x00, 0x78, 0x00, 0x00, 0x4C, 0x01, 0x00, 0x58, 0x01, 0x00,
  0x58, 0x01, 0x00, 0x0C, 0x03, 0x00, 0x0C, 0x03, 0x00, 0x0C, 0x03, 0x00, 0x0C, 0x03, 0x00, 0x58,
  0x01, 0x00, 0x0C, 0x03, 0x00, 0x0C, 0x03, 0x00, 0x78, 0x01, 0x00, 0x7C, 0x01, 0x00, 0x06, 0x03,
  0x00, 0x2C, 0x03, 0x00, 0x2C, 0x03, 0x00, 0x0C, 0x03, 0x00, 0x06, 0x06, 0x00, 0x06, 0x06, 0x00,
  0x0C, 0x03, 0x00, 0x58, 0x01, 0x00, 0x0C, 0x03, 0x00, 0x0C, 0x03, 0x00, 0x78, 0x01, 0x00, 0x3E,
  0x00, 0x00, 0x63, 0x07, 0x00, 0x2E, 0x0C, 0x00, 0x2E, 0x07, 0x00, 0x0B, 0x0D, 0x00, 0x0B, 0x0D,
  0x00, 0x0B, 0x0D, 0x00, 0x06, 0x06, 0x00, 0x5C, 0x03, 0x00, 0x0C, 0x03, 0x00, 0x0C, 0x03, 0x00,
  0x78, 0x01, 0x00, 0x60, 0x07, 0x00, 0x3E, 0x0C, 0x00, 0x23, 0x07, 0x00, 0x2E, 0x07, 0x00, 0x0B,
  0x0D, 0x00, 0x0B, 0x0D, 0x00, 0x0B, 0x0D, 0x00, 0x06, 0x06, 0x00, 0x5C, 0x03, 0x00, 0x0C, 0x03,
  0x00, 0x0C, 0x03, 0x00, 0x78, 0x01, 0x00, 0xF0, 0xFF, 0xFF, 0xF5, 0xE0, 0xFF, 0xF5, 0xFA, 0xFF,
  0xD5, 0xFA, 0xFF, 0xD5, 0xFA, 0xFF, 0xD6, 0xF6, 0xFF, 0x86, 0xF6, 0xFF, 0xD2, 0xF4, 0xFF, 0x89,
  0xF9, 0xFF, 0xDF, 0xFF, 0xFF, 0xD7, 0xFE, 0xFF, 0x83, 0xFC, 0xFF, 0xD7, 0xFE, 0xFF, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xFF, 0xF0, 0xFF, 0xF0, 0xFA, 0xFF, 0xF5, 0xFA, 0xFF, 0xD5, 0xFA, 0xFF, 0xD5,
  0xFA, 0xFF, 0xD6, 0xF6, 0xFF, 0x86, 0xF6, 0xFF, 0xD2, 0xF4, 0xFF, 0x89, 0xF9, 0xFF, 0xDF, 0xFF,
  0xFF, 0xD7, 0xFE, 0xFF, 0x83, 0xFC, 0xFF, 0xD7, 0xFE, 0xFF, 0x84, 0x88, 0x80, 0x85, 0xA8, 0x80,
  0x81, 0xA0, 0x80, 0x80, 0x80, 0x80, 0xC0, 0x80, 0x80, 0x90, 0x82, 0x80, 0x90, 0x82, 0x80, 0x90,
  0x82, 0x80, 0xC0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x81, 0xA0, 0x80, 0x85, 0xA8, 0x80, 0x84, 0x88,
  0x80, 0x67, 0x00, 0x77, 0x00, 0x77, 0x00, 0x63, 0x00, 0x63, 0x00, 0x63, 0x00, 0x63, 0x00, 0x77,
  0x00, 0x63, 0x00, 0x63, 0x00, 0x73, 0x00, 0x77, 0x00, 0x77, 0x00, 0x63, 0x00, 0x63, 0x00, 0x63,
  0x00, 0x63, 0x00, 0x77, 0x00, 0x63, 0x00, 0x63, 0x00, 0x49, 0x00, 0x6B, 0x00, 0x6B, 0x00, 0x22,
  0x00, 0x22, 0x00, 0x22, 0x00, 0x41, 0x00, 0x77, 0x00, 0x63, 0x00, 0x63, 0x00, 0x67, 0x00, 0x77,
  0x00, 0x77, 0x00, 0x63, 0x00, 0x63, 0x00, 0x60, 0x00, 0x60, 0x00, 0x71, 0x00, 0x61, 0x00, 0x63,
  0x00, 0x73, 0x00, 0x77, 0x00, 0x77, 0x00, 0x63, 0x00, 0x63, 0x00, 0x03, 0x00, 0x03, 0x00, 0x47,
  0x00, 0x43, 0x00, 0x63, 0x00, 0x49, 0x00, 0x6B, 0x00, 0x6B, 0x00, 0x22, 0x00, 0x22, 0x00, 0x22,
  0x00, 0x00, 0x00, 0x41, 0x00, 0x41, 0x00, 0x63, 0x00, 0xE4, 0x8F, 0x80, 0x82, 0x80, 0x80, 0x81,
  0x80, 0x80, 0xCE, 0x9F, 0x80, 0xC0, 0x80, 0x80, 0xD4, 0x8A, 0x80, 0x84, 0x88, 0x80, 0xE4, 0x89,
  0x80, 0xF4, 0x8B, 0x80, 0x84, 0x88, 0x80, 0xD4, 0x8A, 0x80, 0xC0, 0x80, 0x80, 0xD0, 0x82, 0x80,
  0x90, 0x82, 0x80, 0xD0, 0x82, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xF8, 0x83, 0x80, 0x80, 0x90, 0x80, 0x81, 0xA0, 0x80, 0xBE, 0x9E, 0x80, 0xC0,
  0x80, 0x80, 0xD4, 0x8A, 0x80, 0x84, 0x88, 0x80, 0xE4, 0x89, 0x80, 0xF4, 0x8B, 0x80, 0x84, 0x88,
  0x80, 0xD4, 0x8A, 0x80, 0xC0, 0x80, 0x80, 0xD0, 0x82, 0x80, 0x90, 0x82, 0x80, 0xD0, 0x82, 0x80,
  0xFC, 0x8C, 0x80, 0x82, 0x90, 0x80, 0x80, 0xA0, 0x80, 0xFC, 0x99, 0x80, 0xC0, 0x80, 0x80, 0xD4,
  0x8A, 0x80, 0x84, 0x88, 0x80, 0xE4, 0x89, 0x80, 0xF4, 0x8B, 0x80, 0x84, 0x88, 0x80, 0xD4, 0x8A,
  0x80, 0xC0, 0x80, 0x80, 0xD0, 0x82, 0x80, 0xB0, 0x83, 0x80, 0xD0, 0x82, 0x80, 0x9C, 0x8F, 0x80,
  0x82, 0x90, 0x80, 0x81, 0xA0, 0x80, 0xF2, 0x87, 0x80, 0xC0, 0x80, 0x80, 0xD4, 0x8A, 0x80, 0x84,
  0x88, 0x80, 0xE4, 0x89, 0x80, 0xF4, 0x8B, 0x80, 0x84, 0x88, 0x80, 0xD4, 0x8A, 0x80, 0xC0, 0x80,
  0x80, 0xD0, 0x82, 0x80, 0xB0, 0x83, 0x80, 0xD0, 0x82, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x01, 0x00, 0x20, 0x01, 0x00, 0x60,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x02, 0x00,
  0x30, 0x03, 0x00, 0x50, 0x02, 0x00, 0x30, 0x03, 0x00, 0x50, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x0A,
  0x00, 0x0C, 0x0C, 0x00, 0x14, 0x0B, 0x00, 0x34, 0x09, 0x00, 0x44, 0x08, 0x00, 0x24, 0x0B, 0x00,
  0x34, 0x0A, 0x00, 0x0C, 0x0C, 0x00, 0x54, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x55, 0x2A, 0x00, 0x33, 0x33, 0x00, 0x65, 0x29, 0x00, 0x49, 0x24, 0x00, 0x53,
  0x32, 0x00, 0x27, 0x39, 0x00, 0x55, 0x2A, 0x00, 0x27, 0x39, 0x00, 0x53, 0x32, 0x00, 0x49, 0x24,
  0x00, 0x65, 0x29, 0x00, 0x33, 0x33, 0x00, 0x55, 0x2A, 0x00, 0x08, 0x00, 0x2A, 0x00, 0x3E, 0x00,
  0x5D, 0x00, 0x3E, 0x00, 0x2A, 0x00, 0x08, 0x00, 0x14, 0x00, 0x08, 0x00, 0x5D, 0x00, 0x36, 0x00,
  0x5D, 0x00, 0x08, 0x00, 0x14, 0x00, 0x77, 0x6E, 0x01, 0x52, 0x2A, 0x01, 0x52, 0x2A, 0x01, 0x53,
  0x2A, 0x01, 0x72, 0x6E, 0x01, 0x77, 0x6E, 0x01, 0x51, 0x2A, 0x01, 0x57, 0x2A, 0x01, 0x54, 0x2A,
  0x01, 0x77, 0x6E, 0x01, 0x77, 0x6E, 0x01, 0x54, 0x2A, 0x01, 0x56, 0x2A, 0x01, 0x54, 0x2A, 0x01,
  0x77, 0x6E, 0x01, 0x74, 0x6E, 0x01, 0x57, 0x2A, 0x01, 0x55, 0x2A, 0x01, 0x51, 0x2A, 0x01, 0x71,
  0x6E, 0x01, 0x77, 0x6E, 0x01, 0x54, 0x2A, 0x01, 0x57, 0x2A, 0x01, 0x51, 0x2A, 0x01, 0x77, 0x6E,
  0x01, 0x03, 0x30, 0x63, 0x31, 0x74, 0x0B, 0x18, 0x06, 0x70, 0x03, 0x78, 0x07, 0x4C, 0x0C, 0x4C,
  0x0C, 0x78, 0x07, 0x74, 0x0B, 0x03, 0x30, 0x03, 0x30, 0x03, 0x10, 0x23, 0x31, 0x54, 0x0B, 0x08,
  0x04, 0x70, 0x02, 0x38, 0x07, 0x4C, 0x08, 0x4C, 0x0C, 0x70, 0x06, 0x30, 0x03, 0x01, 0x30, 0x02,
  0x10, 0x01, 0x10, 0x22, 0x21, 0x54, 0x0A, 0x18, 0x04, 0x10, 0x03, 0x08, 0x04, 0x74, 0x0B, 0x4C,
  0x0C, 0x60, 0x06, 0x24, 0x0A, 0x01, 0x10, 0x02, 0x20, 0x02, 0x10, 0x21, 0x21, 0x20, 0x02, 0x18,
  0x04, 0x10, 0x03, 0x28, 0x02, 0x04, 0x08, 0x48, 0x04, 0x50, 0x04, 0x04, 0x0A, 0x00, 0x20, 0x02,
  0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xFC, 0xFF, 0x8F, 0x80, 0x80, 0xFF, 0x81, 0xFF, 0xFF, 0xBF, 0x80, 0xFF, 0xFF,
  0xBF, 0x80, 0xFF, 0xFF, 0xBF, 0x80, 0xFF, 0x9F, 0xFC, 0x87, 0x80, 0x80, 0xFF, 0xFF, 0xBF, 0xE0,
  0xBF, 0x80, 0xFF, 0x81, 0x80, 0xFC, 0xFF, 0x8F, 0x80, 0x80, 0xFF, 0xC1, 0xFF, 0xC0, 0xFF, 0xC0,
  0xFF, 0xC0, 0xFF, 0xC0, 0xFF, 0xC0, 0xFF, 0xC0, 0xFF, 0x9F, 0xFC, 0x87, 0x80, 0xC0, 0xFF, 0xC0,
  0xFF, 0xE0, 0xBF, 0x80, 0xFF, 0x81, 0x80, 0xFC, 0xFF, 0x8F, 0x80, 0x80, 0xFF, 0xE1, 0xBF, 0x80,
  0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0xE1, 0xFF, 0x9F, 0xFC, 0x87, 0x80, 0xE0,
  0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0x81, 0x80, 0xFC, 0xFF, 0x8F, 0x80, 0x80, 0xFF, 0xE1,
  0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0xE1, 0xFF, 0x9F, 0xFC, 0x87,
  0x80, 0xE0, 0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0x81, 0x80, 0xFC, 0xFF, 0x8F, 0x80, 0x80,
  0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0xE1, 0xFF, 0x9F,
  0xFC, 0x87, 0x80, 0xE0, 0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0x81, 0x80, 0xFC, 0xFF, 0x8F,
  0x80, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0xE1,
  0xFF, 0x80, 0xFC, 0x87, 0x80, 0xE0, 0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0x81, 0x80, 0xFC,
  0xFF, 0x8F, 0x80, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80,
  0xFF, 0xE1, 0xBF, 0x80, 0xFC, 0x87, 0x80, 0xE0, 0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0x81,
  0x80, 0xFC, 0xFF, 0x8F, 0x80, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0xE1,
  0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFC, 0x87, 0x80, 0xE0, 0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80,
  0xFF, 0x81, 0x80, 0xFC, 0xFF, 0x8F, 0x80, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80,
  0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFC, 0x87, 0x80, 0xE0, 0xBF, 0x80, 0xFF, 0xE1,
  0xBF, 0x80, 0xFF, 0x81, 0x80, 0xFC, 0xFF, 0x8F, 0x80, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0xE1,
  0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0xE1, 0xBF, 0x80, 0xFC, 0x87, 0x80, 0xE0, 0xBF, 0x80,
  0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0x81, 0x80, 0xFC, 0xFF, 0x8F, 0x80, 0x80, 0xFF, 0xE1, 0xFF, 0x83,
  0xFF, 0xE1, 0xBF, 0xF0, 0xFF, 0xE1, 0xFF, 0x83, 0xFF, 0xE1, 0xBF, 0x80, 0xFC, 0x87, 0x80, 0xE0,
  0xFF, 0x83, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0x81, 0x80, 0xFC, 0xFF, 0x8F, 0x80, 0x80, 0xFF, 0xE1,
  0xFF, 0x87, 0xFF, 0xE1, 0xBF, 0xF8, 0xFF, 0xE1, 0xFF, 0x87, 0xFF, 0xE1, 0xBF, 0x80, 0xFC, 0x87,
  0x80, 0xE0, 0xFF, 0x87, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0x81, 0x80, 0xFC, 0xFF, 0x8F, 0x80, 0x80,
  0xFF, 0xE1, 0xFF, 0x87, 0xFF, 0xE1, 0xBF, 0xF8, 0xFF, 0xE1, 0xFF, 0x87, 0xFF, 0xE1, 0xBF, 0x80,
  0xFC, 0x87, 0x80, 0xE0, 0xFF, 0x87, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0x81, 0x80, 0xFC, 0xFF, 0x8F,
  0x80, 0x80, 0xFF, 0xE1, 0xFF, 0x87, 0xFF, 0xE1, 0xBF, 0xF8, 0xFF, 0xE1, 0xFF, 0x87, 0xFF, 0xE1,
  0xBF, 0x80, 0xFC, 0x87, 0x80, 0xE0, 0xFF, 0x87, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0x81, 0x80, 0xFC,
  0xFF, 0x8F, 0x80, 0x80, 0xFF, 0xE1, 0xFF, 0x87, 0xFF, 0xE1, 0xBF, 0xF8, 0xFF, 0xE1, 0xFF, 0x87,
  0xFF, 0xE1, 0xBF, 0x80, 0xFC, 0x87, 0x80, 0xE0, 0xFF, 0x87, 0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0x81,
  0x80, 0xFC, 0xFF, 0x8F, 0x80, 0x80, 0xFF, 0xE1, 0xFF, 0x87, 0xFF, 0xE1, 0xBF, 0xF8, 0xFF, 0xE1,
  0xFF, 0x87, 0xFF, 0xE1, 0xBF, 0x80, 0xFC, 0x87, 0xFF, 0xE1, 0xFF, 0x87, 0xFF, 0xE1, 0xBF, 0x80,
  0xFF, 0x81, 0x80, 0xFC, 0xFF, 0x8F, 0x80, 0x80, 0xFF, 0xE1, 0xFF, 0x87, 0xFF, 0xE1, 0xBF, 0xF8,
  0xFF, 0xE1, 0xFF, 0x87, 0xFF, 0xE1, 0xBF, 0x80, 0xFC, 0x87, 0xFF, 0xE1, 0xFF, 0x87, 0xFF, 0xE1,
  0xBF, 0x80, 0xFF, 0x81, 0x80, 0xFC, 0xFF, 0x8F, 0x80, 0x80, 0xFF, 0xE1, 0xFF, 0x87, 0xFF, 0xE1,
  0xBF, 0xF8, 0xFF, 0xE1, 0xFF, 0x87, 0xFF, 0xE1, 0xBF, 0x80, 0xFC, 0xCF, 0xFF, 0xE1, 0xFF, 0x87,
  0xFF, 0xE1, 0xBF, 0x80, 0xFF, 0x81, 0x80, 0xFC, 0xFF, 0x8F, 0x80, 0xC0, 0xFF, 0xE1, 0xFF, 0x87,
  0xFF, 0xE1, 0xBF, 0xF8, 0xFF, 0xE1, 0xFF, 0x87, 0xFF, 0xE1, 0xBF, 0x80, 0xFC, 0xFF, 0xFF, 0xE1,
  0xFF, 0x87, 0xFF, 0xE1, 0xFF, 0xC0, 0xFF, 0x81, 0x80, 0xFC, 0xFF, 0x9F, 0x80, 0xE0, 0xFF, 0xE1,
  0xFF, 0x87, 0xFF, 0xE1, 0xBF, 0xF8, 0xFF, 0xE1, 0xFF, 0x87, 0xFF, 0xE1, 0xBF, 0x80, 0xFC, 0xFF,
  0xFF, 0xE1, 0xFF, 0x87, 0xFF, 0xE1, 0xFF, 0xFF, 0xFF, 0x81, 0x80, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xC0, 0xFF, 0xCF, 0xFF, 0xE0, 0xFF, 0xFC, 0xFF, 0xC0, 0xFF, 0xCF, 0xFF, 0xE0, 0xBF, 0x80,
  0xF8, 0xFF, 0xFF, 0xC0, 0xFF, 0xCF, 0xFF, 0xC0, 0xFF, 0xFF, 0xFF, 0x81, 0x80, 0xFC, 0xFF, 0xFF,
  0xFF, 0xFF, 0xBF, 0x80, 0xFF, 0xFF, 0xBF, 0xE0, 0xFF, 0xFF, 0xBF, 0x80, 0xFF, 0xFF, 0xBF, 0xE0,
  0xBF, 0x80, 0xF0, 0xFF, 0xBF, 0x80, 0xFF, 0xFF, 0xBF, 0x80, 0xFF, 0xFF, 0xFF, 0x80, 0x80, 0xFC,
  0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0xFC, 0x8F, 0x80, 0xFC, 0xFF, 0x81, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0xFC, 0x87, 0x80, 0xF8, 0xFF, 0x83, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0xFC, 0x87, 0x80, 0xF8, 0xFF, 0x83, 0x80, 0x80, 0x80, 0x80, 0xE0,
  0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFC, 0x87, 0x80, 0xF8, 0xFF, 0x83, 0x80, 0x80, 0x80,
  0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0xC0, 0xFF, 0xFF, 0xFF, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFC, 0x87, 0x80, 0xF8, 0xFF, 0x83, 0x80,
  0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0xC0, 0xFF, 0xFF, 0xFF, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFC, 0x87, 0x80, 0xF8, 0xFF,
  0x83, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0xC0, 0xFF, 0xFF, 0xFF,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFC, 0x87, 0x80,
  0xF8, 0xFF, 0x83, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0xC0, 0xFF,
  0xFF, 0xFF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFC,
  0x87, 0x80, 0xF8, 0xFF, 0x83, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80,
  0xC0, 0xFF, 0xFF, 0xFF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0xFC, 0x87, 0x80, 0xF8, 0xFF, 0x83, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80,
  0x80, 0x80, 0xC0, 0xFF, 0xFF, 0xFF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0xFC, 0x87, 0x80, 0xF8, 0xFF, 0x83, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0xFC, 0x87, 0x80, 0xF8, 0xFF, 0x83, 0x80, 0x80, 0x80, 0x80, 0xE0,
  0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFC, 0x87, 0x80, 0xF8, 0xFF, 0x83, 0x80, 0x80, 0x80,
  0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFC, 0x87, 0x80, 0xF8, 0xFF, 0x83, 0x80,
  0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFC, 0x87, 0x80, 0xF8, 0xFF,
  0x83, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFC, 0x87, 0x80,
  0xF8, 0xFF, 0x83, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xE0,
  0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFC,
  0x87, 0x80, 0xF8, 0xFF, 0x83, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0xFC, 0x87, 0x80, 0xF8, 0xFF, 0x83, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0xFC, 0x87, 0x80, 0xF8, 0xFF, 0x83, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0xFC, 0x87, 0x80, 0xF8, 0xFF, 0x83, 0x80, 0x80, 0x80, 0x80, 0xE0,
  0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xE0, 0xBF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFC, 0x8F, 0x80, 0xFC, 0xFF, 0x83, 0x80, 0x80, 0x80,
  0x80, 0xC0, 0x9F, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xC0, 0x9F, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0x81, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFF, 0xFF, 0xFF
};
