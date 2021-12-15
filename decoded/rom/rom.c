// 45 new runtime labels added
// ranges: 61
// code labels: 1159
// data labels: 187

#include "apple2tc/system-inc.h"

static const uint8_t s_mem_d000[0x3000];

void init_emulated(void) {
  memcpy(s_ram + 0xd000, s_mem_d000, 0x3000);
  s_pc = 0xfa62;
}

void run_emulated(unsigned run_cycles) {
  bool branchTarget = true;
  for(unsigned start_cycles = s_cycles; s_cycles - start_cycles < run_cycles;) {
    uint16_t tmp16;
    uint8_t tmp;
    switch (s_pc) {
    case 0x0090: // [$0090..$0090]    1 bytes
      CYCLES(0x0090, 2);
      // ERROR: opcode self modification.
      /* $0090 ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x0090);
                      abort();

    case 0x00b1: // [$00B1..$00B1]    1 bytes
      CYCLES(0x00b1, 2);
      /* $00B1 ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x00b1);
                      abort();

    case 0x00b5: // [$00B5..$00B5]    1 bytes
      CYCLES(0x00b5, 2);
      /* $00B5 ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x00b5);
                      abort();

    case 0x00b7: // [$00B7..$00B7]    1 bytes
      CYCLES(0x00b7, 2);
      /* $00B7 ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x00b7);
                      abort();

    case 0x00be: // [$00BE..$00BE]    1 bytes
      CYCLES(0x00be, 2);
      /* $00BE ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x00be);
                      abort();

    case 0x00c2: // [$00C2..$00C2]    1 bytes
      CYCLES(0x00c2, 2);
      /* $00C2 ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x00c2);
                      abort();

    case 0x00c8: // [$00C8..$00C8]    1 bytes
      CYCLES(0x00c8, 2);
      /* $00C8 ??? */ fprintf(stderr, "Warning: INVALID at $%04X\n", 0x00c8);
                      abort();

    case 0xd365: // [$D365..$D369]    5 bytes
      CYCLES(0xd365, 9);
      /* $D365 TSX */ s_x = update_nz(s_sp);
      /* $D366 INX */ s_x = update_nz(s_x + 1);
      /* $D367 INX */ s_x = update_nz(s_x + 1);
      /* $D368 INX */ s_x = update_nz(s_x + 1);
      /* $D369 INX */ s_x = update_nz(s_x + 1);
    case 0xd36a: // [$D36A..$D370]    7 bytes
      CYCLES(0xd36a, 12);
      /* $D36A LDA */ s_a = update_nz(peek(0x0101 + s_x));
      /* $D36D CMP */ update_nz_inv_c(s_a - 0x81);
      /* $D36F BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd392 : 0xd371;
      branchTarget = true;
      break;
    case 0xd371: // [$D371..$D374]    4 bytes
      CYCLES(0xd371, 7);
      /* $D371 LDA */ s_a = update_nz(peek_zpg(0x86));
      /* $D373 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd37f : 0xd375;
      branchTarget = true;
      break;
    case 0xd375: // [$D375..$D37E]   10 bytes
      CYCLES(0xd375, 18);
      /* $D375 LDA */ s_a = update_nz(peek(0x0102 + s_x));
      /* $D378 STA */ poke_zpg(0x85, s_a);
      /* $D37A LDA */ s_a = update_nz(peek(0x0103 + s_x));
      /* $D37D STA */ poke_zpg(0x86, s_a);
    case 0xd37f: // [$D37F..$D383]    5 bytes
      CYCLES(0xd37f, 9);
      /* $D37F CMP */ update_nz_inv_c(s_a - peek(0x0103 + s_x));
      /* $D382 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd38b : 0xd384;
      branchTarget = true;
      break;
    case 0xd384: // [$D384..$D38A]    7 bytes
      CYCLES(0xd384, 12);
      /* $D384 LDA */ s_a = update_nz(peek_zpg(0x85));
      /* $D386 CMP */ update_nz_inv_c(s_a - peek(0x0102 + s_x));
      /* $D389 BEQ */ s_pc = s_status & STATUS_Z ? 0xd392 : 0xd38b;
      branchTarget = true;
      break;
    case 0xd38b: // [$D38B..$D391]    7 bytes
      CYCLES(0xd38b, 12);
      /* $D38B TXA */ s_a = update_nz(s_x);
      /* $D38C CLC */ s_status &= ~STATUS_C;
      /* $D38D ADC */ tmp = 0x12, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $D38F TAX */ s_x = update_nz(s_a);
      /* $D390 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd36a : 0xd392;
      branchTarget = true;
      break;
    case 0xd392: // [$D392..$D392]    1 bytes
      CYCLES(0xd392, 2);
      /* $D392 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xd393: // [$D393..$D395]    3 bytes
      CYCLES(0xd393, 6);
      /* $D393 JSR */ push16(0xd395), s_pc = 0xd3e3;
      branchTarget = true;
      break;
    case 0xd396: // [$D396..$D399]    4 bytes
      CYCLES(0xd396, 7);
      /* $D396 STA */ poke_zpg(0x6d, s_a);
      /* $D398 STY */ poke_zpg(0x6e, s_y);
    case 0xd39a: // [$D39A..$D3AA]   17 bytes
      CYCLES(0xd39a, 29);
      /* $D39A SEC */ s_status |= STATUS_C;
      /* $D39B LDA */ s_a = update_nz(peek_zpg(0x96));
      /* $D39D SBC */ tmp = peek_zpg(0x9b), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $D39F STA */ poke_zpg(0x5e, s_a);
      /* $D3A1 TAY */ s_y = update_nz(s_a);
      /* $D3A2 LDA */ s_a = update_nz(peek_zpg(0x97));
      /* $D3A4 SBC */ tmp = peek_zpg(0x9c), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $D3A6 TAX */ s_x = update_nz(s_a);
      /* $D3A7 INX */ s_x = update_nz(s_x + 1);
      /* $D3A8 TYA */ s_a = update_nz(s_y);
      /* $D3A9 BEQ */ s_pc = s_status & STATUS_Z ? 0xd3ce : 0xd3ab;
      branchTarget = true;
      break;
    case 0xd3ab: // [$D3AB..$D3B3]    9 bytes
      CYCLES(0xd3ab, 16);
      /* $D3AB LDA */ s_a = update_nz(peek_zpg(0x96));
      /* $D3AD SEC */ s_status |= STATUS_C;
      /* $D3AE SBC */ tmp = peek_zpg(0x5e), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $D3B0 STA */ poke_zpg(0x96, s_a);
      /* $D3B2 BCS */ s_pc = s_status & STATUS_C ? 0xd3b7 : 0xd3b4;
      branchTarget = true;
      break;
    case 0xd3b4: // [$D3B4..$D3B6]    3 bytes
      CYCLES(0xd3b4, 6);
      /* $D3B4 DEC */ tmp16 = 0x97, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $D3B6 SEC */ s_status |= STATUS_C;
    case 0xd3b7: // [$D3B7..$D3BE]    8 bytes
      CYCLES(0xd3b7, 14);
      /* $D3B7 LDA */ s_a = update_nz(peek_zpg(0x94));
      /* $D3B9 SBC */ tmp = peek_zpg(0x5e), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $D3BB STA */ poke_zpg(0x94, s_a);
      /* $D3BD BCS */ s_pc = s_status & STATUS_C ? 0xd3c7 : 0xd3bf;
      branchTarget = true;
      break;
    case 0xd3bf: // [$D3BF..$D3C2]    4 bytes
      CYCLES(0xd3bf, 7);
      /* $D3BF DEC */ tmp16 = 0x95, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $D3C1 BCC */ s_pc = !(s_status & STATUS_C) ? 0xd3c7 : 0xd3c3;
      branchTarget = true;
      break;
    case 0xd3c3: // [$D3C3..$D3C6]    4 bytes
      CYCLES(0xd3c3, 7);
      /* $D3C3 LDA */ s_a = update_nz(peek(peek16_zpg(0x96) + s_y));
      /* $D3C5 STA */ poke(peek16_zpg(0x94) + s_y, s_a);
    case 0xd3c7: // [$D3C7..$D3C9]    3 bytes
      CYCLES(0xd3c7, 6);
      /* $D3C7 DEY */ s_y = update_nz(s_y - 1);
      /* $D3C8 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd3c3 : 0xd3ca;
      branchTarget = true;
      break;
    case 0xd3ca: // [$D3CA..$D3CD]    4 bytes
      CYCLES(0xd3ca, 7);
      /* $D3CA LDA */ s_a = update_nz(peek(peek16_zpg(0x96) + s_y));
      /* $D3CC STA */ poke(peek16_zpg(0x94) + s_y, s_a);
    case 0xd3ce: // [$D3CE..$D3D4]    7 bytes
      CYCLES(0xd3ce, 12);
      /* $D3CE DEC */ tmp16 = 0x97, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $D3D0 DEC */ tmp16 = 0x95, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $D3D2 DEX */ s_x = update_nz(s_x - 1);
      /* $D3D3 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd3c7 : 0xd3d5;
      branchTarget = true;
      break;
    case 0xd3d5: // [$D3D5..$D3D5]    1 bytes
      CYCLES(0xd3d5, 2);
      /* $D3D5 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xd3d6: // [$D3D6..$D3DA]    5 bytes
      CYCLES(0xd3d6, 9);
      /* $D3D6 ASL */ s_a = update_nzc(s_a << 1);
      /* $D3D7 ADC */ tmp = 0x36, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $D3D9 BCS */ s_pc = s_status & STATUS_C ? 0xd410 : 0xd3db;
      branchTarget = true;
      break;
    case 0xd3db: // [$D3DB..$D3E1]    7 bytes
      CYCLES(0xd3db, 12);
      /* $D3DB STA */ poke_zpg(0x5e, s_a);
      /* $D3DD TSX */ s_x = update_nz(s_sp);
      /* $D3DE CPX */ update_nz_inv_c(s_x - peek_zpg(0x5e));
      /* $D3E0 BCC */ s_pc = !(s_status & STATUS_C) ? 0xd410 : 0xd3e2;
      branchTarget = true;
      break;
    case 0xd3e2: // [$D3E2..$D3E2]    1 bytes
      CYCLES(0xd3e2, 2);
      /* $D3E2 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xd3e3: // [$D3E3..$D3E6]    4 bytes
      CYCLES(0xd3e3, 7);
      /* $D3E3 CPY */ update_nz_inv_c(s_y - peek_zpg(0x70));
      /* $D3E5 BCC */ s_pc = !(s_status & STATUS_C) ? 0xd40f : 0xd3e7;
      branchTarget = true;
      break;
    case 0xd3e7: // [$D3E7..$D3E8]    2 bytes
      CYCLES(0xd3e7, 4);
      /* $D3E7 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd3ed : 0xd3e9;
      branchTarget = true;
      break;
    case 0xd3e9: // [$D3E9..$D3EC]    4 bytes
      CYCLES(0xd3e9, 7);
      /* $D3E9 CMP */ update_nz_inv_c(s_a - peek_zpg(0x6f));
      /* $D3EB BCC */ s_pc = !(s_status & STATUS_C) ? 0xd40f : 0xd3ed;
      branchTarget = true;
      break;
    case 0xd3ed: // [$D3ED..$D3F0]    4 bytes
      CYCLES(0xd3ed, 7);
      /* $D3ED PHA */ push8(s_a);
      /* $D3EE LDX */ s_x = update_nz(0x09);
      /* $D3F0 TYA */ s_a = update_nz(s_y);
    case 0xd3f1: // [$D3F1..$D3F6]    6 bytes
      CYCLES(0xd3f1, 11);
      /* $D3F1 PHA */ push8(s_a);
      /* $D3F2 LDA */ s_a = update_nz(peek_zpg((uint8_t)(0x93 + s_x)));
      /* $D3F4 DEX */ s_x = update_nz(s_x - 1);
      /* $D3F5 BPL */ s_pc = !(s_status & STATUS_N) ? 0xd3f1 : 0xd3f7;
      branchTarget = true;
      break;
    case 0xd3f7: // [$D3F7..$D3F9]    3 bytes
      CYCLES(0xd3f7, 6);
      /* $D3F7 JSR */ push16(0xd3f9), s_pc = 0xe484;
      branchTarget = true;
      break;
    case 0xd3fa: // [$D3FA..$D3FB]    2 bytes
      CYCLES(0xd3fa, 4);
      /* $D3FA LDX */ s_x = update_nz(0xf7);
    case 0xd3fc: // [$D3FC..$D401]    6 bytes
      CYCLES(0xd3fc, 11);
      /* $D3FC PLA */ s_a = update_nz(pop8());
      /* $D3FD STA */ poke_zpg((uint8_t)(0x9d + s_x), s_a);
      /* $D3FF INX */ s_x = update_nz(s_x + 1);
      /* $D400 BMI */ s_pc = s_status & STATUS_N ? 0xd3fc : 0xd402;
      branchTarget = true;
      break;
    case 0xd402: // [$D402..$D408]    7 bytes
      CYCLES(0xd402, 12);
      /* $D402 PLA */ s_a = update_nz(pop8());
      /* $D403 TAY */ s_y = update_nz(s_a);
      /* $D404 PLA */ s_a = update_nz(pop8());
      /* $D405 CPY */ update_nz_inv_c(s_y - peek_zpg(0x70));
      /* $D407 BCC */ s_pc = !(s_status & STATUS_C) ? 0xd40f : 0xd409;
      branchTarget = true;
      break;
    case 0xd409: // [$D409..$D40A]    2 bytes
      CYCLES(0xd409, 4);
      /* $D409 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd410 : 0xd40b;
      branchTarget = true;
      break;
    case 0xd40b: // [$D40B..$D40E]    4 bytes
      CYCLES(0xd40b, 7);
      /* $D40B CMP */ update_nz_inv_c(s_a - peek_zpg(0x6f));
      /* $D40D BCS */ s_pc = s_status & STATUS_C ? 0xd410 : 0xd40f;
      branchTarget = true;
      break;
    case 0xd40f: // [$D40F..$D40F]    1 bytes
      CYCLES(0xd40f, 2);
      /* $D40F RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xd410: // [$D410..$D411]    2 bytes
      CYCLES(0xd410, 4);
      /* $D410 LDX */ s_x = update_nz(0x4d);
    case 0xd412: // [$D412..$D415]    4 bytes
      CYCLES(0xd412, 7);
      /* $D412 BIT */ tmp = peek_zpg(0xd8), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $D414 BPL */ s_pc = !(s_status & STATUS_N) ? 0xd419 : 0xd416;
      branchTarget = true;
      break;
    case 0xd416: // [$D416..$D418]    3 bytes
      CYCLES(0xd416, 6);
      /* $D416 JMP */ s_pc = 0xf2e9;
      branchTarget = true;
      break;
    case 0xd419: // [$D419..$D41B]    3 bytes
      CYCLES(0xd419, 6);
      /* $D419 JSR */ push16(0xd41b), s_pc = 0xdafb;
      branchTarget = true;
      break;
    case 0xd41c: // [$D41C..$D41E]    3 bytes
      CYCLES(0xd41c, 6);
      /* $D41C JSR */ push16(0xd41e), s_pc = 0xdb5a;
      branchTarget = true;
      break;
    case 0xd41f: // [$D41F..$D425]    7 bytes
      CYCLES(0xd41f, 12);
      /* $D41F LDA */ s_a = update_nz(peek(0xd260 + s_x));
      /* $D422 PHA */ push8(s_a);
      /* $D423 JSR */ push16(0xd425), s_pc = 0xdb5c;
      branchTarget = true;
      break;
    case 0xd426: // [$D426..$D429]    4 bytes
      CYCLES(0xd426, 7);
      /* $D426 INX */ s_x = update_nz(s_x + 1);
      /* $D427 PLA */ s_a = update_nz(pop8());
      /* $D428 BPL */ s_pc = !(s_status & STATUS_N) ? 0xd41f : 0xd42a;
      branchTarget = true;
      break;
    case 0xd42a: // [$D42A..$D42C]    3 bytes
      CYCLES(0xd42a, 6);
      /* $D42A JSR */ push16(0xd42c), s_pc = 0xd683;
      branchTarget = true;
      break;
    case 0xd42d: // [$D42D..$D430]    4 bytes
      CYCLES(0xd42d, 7);
      /* $D42D LDA */ s_a = update_nz(0x50);
      /* $D42F LDY */ s_y = update_nz(0xd3);
    case 0xd431: // [$D431..$D433]    3 bytes
      CYCLES(0xd431, 6);
      /* $D431 JSR */ push16(0xd433), s_pc = 0xdb3a;
      branchTarget = true;
      break;
    case 0xd434: // [$D434..$D438]    5 bytes
      CYCLES(0xd434, 9);
      /* $D434 LDY */ s_y = update_nz(peek_zpg(0x76));
      /* $D436 INY */ s_y = update_nz(s_y + 1);
      /* $D437 BEQ */ s_pc = s_status & STATUS_Z ? 0xd43c : 0xd439;
      branchTarget = true;
      break;
    case 0xd439: // [$D439..$D43B]    3 bytes
      CYCLES(0xd439, 6);
      /* $D439 JSR */ push16(0xd43b), s_pc = 0xed19;
      branchTarget = true;
      break;
    case 0xd43c: // [$D43C..$D43E]    3 bytes
      CYCLES(0xd43c, 6);
      /* $D43C JSR */ push16(0xd43e), s_pc = 0xdafb;
      branchTarget = true;
      break;
    case 0xd43f: // [$D43F..$D443]    5 bytes
      CYCLES(0xd43f, 9);
      /* $D43F LDX */ s_x = update_nz(0xdd);
      /* $D441 JSR */ push16(0xd443), s_pc = 0xd52e;
      branchTarget = true;
      break;
    case 0xd444: // [$D444..$D44C]    9 bytes
      CYCLES(0xd444, 16);
      /* $D444 STX */ poke_zpg(0xb8, s_x);
      /* $D446 STY */ poke_zpg(0xb9, s_y);
      /* $D448 LSR */ tmp16 = 0xd8, tmp = peek_zpg(tmp16), set_c_to_bit0(tmp), poke_zpg(tmp16, update_nz(tmp >> 1));
      /* $D44A JSR */ push16(0xd44c), s_pc = 0x00b1;
      branchTarget = true;
      break;
    case 0xd44d: // [$D44D..$D44F]    3 bytes
      CYCLES(0xd44d, 6);
      /* $D44D TAX */ s_x = update_nz(s_a);
      /* $D44E BEQ */ s_pc = s_status & STATUS_Z ? 0xd43c : 0xd450;
      branchTarget = true;
      break;
    case 0xd450: // [$D450..$D455]    6 bytes
      CYCLES(0xd450, 11);
      /* $D450 LDX */ s_x = update_nz(0xff);
      /* $D452 STX */ poke_zpg(0x76, s_x);
      /* $D454 BCC */ s_pc = !(s_status & STATUS_C) ? 0xd45c : 0xd456;
      branchTarget = true;
      break;
    case 0xd456: // [$D456..$D458]    3 bytes
      CYCLES(0xd456, 6);
      /* $D456 JSR */ push16(0xd458), s_pc = 0xd559;
      branchTarget = true;
      break;
    case 0xd459: // [$D459..$D45B]    3 bytes
      CYCLES(0xd459, 6);
      /* $D459 JMP */ s_pc = 0xd805;
      branchTarget = true;
      break;
    case 0xd45c: // [$D45C..$D466]   11 bytes
      CYCLES(0xd45c, 19);
      /* $D45C LDX */ s_x = update_nz(peek_zpg(0xaf));
      /* $D45E STX */ poke_zpg(0x69, s_x);
      /* $D460 LDX */ s_x = update_nz(peek_zpg(0xb0));
      /* $D462 STX */ poke_zpg(0x6a, s_x);
      /* $D464 JSR */ push16(0xd466), s_pc = 0xda0c;
      branchTarget = true;
      break;
    case 0xd467: // [$D467..$D469]    3 bytes
      CYCLES(0xd467, 6);
      /* $D467 JSR */ push16(0xd469), s_pc = 0xd559;
      branchTarget = true;
      break;
    case 0xd46a: // [$D46A..$D46E]    5 bytes
      CYCLES(0xd46a, 9);
      /* $D46A STY */ poke_zpg(0x0f, s_y);
      /* $D46C JSR */ push16(0xd46e), s_pc = 0xd61a;
      branchTarget = true;
      break;
    case 0xd46f: // [$D46F..$D470]    2 bytes
      CYCLES(0xd46f, 4);
      /* $D46F BCC */ s_pc = !(s_status & STATUS_C) ? 0xd4b5 : 0xd471;
      branchTarget = true;
      break;
    case 0xd471: // [$D471..$D49B]   43 bytes
      CYCLES(0xd471, 74);
      /* $D471 LDY */ s_y = update_nz(0x01);
      /* $D473 LDA */ s_a = update_nz(peek(peek16_zpg(0x9b) + s_y));
      /* $D475 STA */ poke_zpg(0x5f, s_a);
      /* $D477 LDA */ s_a = update_nz(peek_zpg(0x69));
      /* $D479 STA */ poke_zpg(0x5e, s_a);
      /* $D47B LDA */ s_a = update_nz(peek_zpg(0x9c));
      /* $D47D STA */ poke_zpg(0x61, s_a);
      /* $D47F LDA */ s_a = update_nz(peek_zpg(0x9b));
      /* $D481 DEY */ s_y = update_nz(s_y - 1);
      /* $D482 SBC */ tmp = peek(peek16_zpg(0x9b) + s_y), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $D484 CLC */ s_status &= ~STATUS_C;
      /* $D485 ADC */ tmp = peek_zpg(0x69), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $D487 STA */ poke_zpg(0x69, s_a);
      /* $D489 STA */ poke_zpg(0x60, s_a);
      /* $D48B LDA */ s_a = update_nz(peek_zpg(0x6a));
      /* $D48D ADC */ tmp = 0xff, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $D48F STA */ poke_zpg(0x6a, s_a);
      /* $D491 SBC */ tmp = peek_zpg(0x9c), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $D493 TAX */ s_x = update_nz(s_a);
      /* $D494 SEC */ s_status |= STATUS_C;
      /* $D495 LDA */ s_a = update_nz(peek_zpg(0x9b));
      /* $D497 SBC */ tmp = peek_zpg(0x69), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $D499 TAY */ s_y = update_nz(s_a);
      /* $D49A BCS */ s_pc = s_status & STATUS_C ? 0xd49f : 0xd49c;
      branchTarget = true;
      break;
    case 0xd49c: // [$D49C..$D49E]    3 bytes
      CYCLES(0xd49c, 6);
      /* $D49C INX */ s_x = update_nz(s_x + 1);
      /* $D49D DEC */ tmp16 = 0x61, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
    case 0xd49f: // [$D49F..$D4A3]    5 bytes
      CYCLES(0xd49f, 9);
      /* $D49F CLC */ s_status &= ~STATUS_C;
      /* $D4A0 ADC */ tmp = peek_zpg(0x5e), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $D4A2 BCC */ s_pc = !(s_status & STATUS_C) ? 0xd4a7 : 0xd4a4;
      branchTarget = true;
      break;
    case 0xd4a4: // [$D4A4..$D4A6]    3 bytes
      CYCLES(0xd4a4, 6);
      /* $D4A4 DEC */ tmp16 = 0x5f, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $D4A6 CLC */ s_status &= ~STATUS_C;
    case 0xd4a7: // [$D4A7..$D4AD]    7 bytes
      CYCLES(0xd4a7, 12);
      /* $D4A7 LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $D4A9 STA */ poke(peek16_zpg(0x60) + s_y, s_a);
      /* $D4AB INY */ s_y = update_nz(s_y + 1);
      /* $D4AC BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd4a7 : 0xd4ae;
      branchTarget = true;
      break;
    case 0xd4ae: // [$D4AE..$D4B4]    7 bytes
      CYCLES(0xd4ae, 12);
      /* $D4AE INC */ tmp16 = 0x5f, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $D4B0 INC */ tmp16 = 0x61, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $D4B2 DEX */ s_x = update_nz(s_x - 1);
      /* $D4B3 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd4a7 : 0xd4b5;
      branchTarget = true;
      break;
    case 0xd4b5: // [$D4B5..$D4B9]    5 bytes
      CYCLES(0xd4b5, 9);
      /* $D4B5 LDA */ s_a = update_nz(peek(0x0200));
      /* $D4B8 BEQ */ s_pc = s_status & STATUS_Z ? 0xd4f2 : 0xd4ba;
      branchTarget = true;
      break;
    case 0xd4ba: // [$D4BA..$D4CF]   22 bytes
      CYCLES(0xd4ba, 38);
      /* $D4BA LDA */ s_a = update_nz(peek_zpg(0x73));
      /* $D4BC LDY */ s_y = update_nz(peek_zpg(0x74));
      /* $D4BE STA */ poke_zpg(0x6f, s_a);
      /* $D4C0 STY */ poke_zpg(0x70, s_y);
      /* $D4C2 LDA */ s_a = update_nz(peek_zpg(0x69));
      /* $D4C4 STA */ poke_zpg(0x96, s_a);
      /* $D4C6 ADC */ tmp = peek_zpg(0x0f), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $D4C8 STA */ poke_zpg(0x94, s_a);
      /* $D4CA LDY */ s_y = update_nz(peek_zpg(0x6a));
      /* $D4CC STY */ poke_zpg(0x97, s_y);
      /* $D4CE BCC */ s_pc = !(s_status & STATUS_C) ? 0xd4d1 : 0xd4d0;
      branchTarget = true;
      break;
    case 0xd4d0: // [$D4D0..$D4D0]    1 bytes
      CYCLES(0xd4d0, 2);
      /* $D4D0 INY */ s_y = update_nz(s_y + 1);
    case 0xd4d1: // [$D4D1..$D4D5]    5 bytes
      CYCLES(0xd4d1, 9);
      /* $D4D1 STY */ poke_zpg(0x95, s_y);
      /* $D4D3 JSR */ push16(0xd4d5), s_pc = 0xd393;
      branchTarget = true;
      break;
    case 0xd4d6: // [$D4D6..$D4E9]   20 bytes
      CYCLES(0xd4d6, 35);
      /* $D4D6 LDA */ s_a = update_nz(peek_zpg(0x50));
      /* $D4D8 LDY */ s_y = update_nz(peek_zpg(0x51));
      /* $D4DA STA */ poke(0x01fe, s_a);
      /* $D4DD STY */ poke(0x01ff, s_y);
      /* $D4E0 LDA */ s_a = update_nz(peek_zpg(0x6d));
      /* $D4E2 LDY */ s_y = update_nz(peek_zpg(0x6e));
      /* $D4E4 STA */ poke_zpg(0x69, s_a);
      /* $D4E6 STY */ poke_zpg(0x6a, s_y);
      /* $D4E8 LDY */ s_y = update_nz(peek_zpg(0x0f));
    case 0xd4ea: // [$D4EA..$D4F1]    8 bytes
      CYCLES(0xd4ea, 14);
      /* $D4EA LDA */ s_a = update_nz(peek(0x01fb + s_y));
      /* $D4ED DEY */ s_y = update_nz(s_y - 1);
      /* $D4EE STA */ poke(peek16_zpg(0x9b) + s_y, s_a);
      /* $D4F0 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd4ea : 0xd4f2;
      branchTarget = true;
      break;
    case 0xd4f2: // [$D4F2..$D4F4]    3 bytes
      CYCLES(0xd4f2, 6);
      /* $D4F2 JSR */ push16(0xd4f4), s_pc = 0xd665;
      branchTarget = true;
      break;
    case 0xd4f5: // [$D4F5..$D4FD]    9 bytes
      CYCLES(0xd4f5, 16);
      /* $D4F5 LDA */ s_a = update_nz(peek_zpg(0x67));
      /* $D4F7 LDY */ s_y = update_nz(peek_zpg(0x68));
      /* $D4F9 STA */ poke_zpg(0x5e, s_a);
      /* $D4FB STY */ poke_zpg(0x5f, s_y);
      /* $D4FD CLC */ s_status &= ~STATUS_C;
    case 0xd4fe: // [$D4FE..$D503]    6 bytes
      CYCLES(0xd4fe, 11);
      /* $D4FE LDY */ s_y = update_nz(0x01);
      /* $D500 LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $D502 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd50f : 0xd504;
      branchTarget = true;
      break;
    case 0xd504: // [$D504..$D50E]   11 bytes
      CYCLES(0xd504, 19);
      /* $D504 LDA */ s_a = update_nz(peek_zpg(0x69));
      /* $D506 STA */ poke_zpg(0xaf, s_a);
      /* $D508 LDA */ s_a = update_nz(peek_zpg(0x6a));
      /* $D50A STA */ poke_zpg(0xb0, s_a);
      /* $D50C JMP */ s_pc = 0xd43c;
      branchTarget = true;
      break;
    case 0xd50f: // [$D50F..$D510]    2 bytes
      CYCLES(0xd50f, 4);
      /* $D50F LDY */ s_y = update_nz(0x04);
    case 0xd511: // [$D511..$D515]    5 bytes
      CYCLES(0xd511, 9);
      /* $D511 INY */ s_y = update_nz(s_y + 1);
      /* $D512 LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $D514 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd511 : 0xd516;
      branchTarget = true;
      break;
    case 0xd516: // [$D516..$D52B]   22 bytes
      CYCLES(0xd516, 38);
      /* $D516 INY */ s_y = update_nz(s_y + 1);
      /* $D517 TYA */ s_a = update_nz(s_y);
      /* $D518 ADC */ tmp = peek_zpg(0x5e), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $D51A TAX */ s_x = update_nz(s_a);
      /* $D51B LDY */ s_y = update_nz(0x00);
      /* $D51D STA */ poke(peek16_zpg(0x5e) + s_y, s_a);
      /* $D51F LDA */ s_a = update_nz(peek_zpg(0x5f));
      /* $D521 ADC */ tmp = 0x00, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $D523 INY */ s_y = update_nz(s_y + 1);
      /* $D524 STA */ poke(peek16_zpg(0x5e) + s_y, s_a);
      /* $D526 STX */ poke_zpg(0x5e, s_x);
      /* $D528 STA */ poke_zpg(0x5f, s_a);
      /* $D52A BCC */ s_pc = !(s_status & STATUS_C) ? 0xd4fe : 0xd52c;
      branchTarget = true;
      break;
    case 0xd52c: // [$D52C..$D52D]    2 bytes
      CYCLES(0xd52c, 4);
      /* $D52C LDX */ s_x = update_nz(0x80);
    case 0xd52e: // [$D52E..$D532]    5 bytes
      CYCLES(0xd52e, 9);
      /* $D52E STX */ poke_zpg(0x33, s_x);
      /* $D530 JSR */ push16(0xd532), s_pc = 0xfd6a;
      branchTarget = true;
      break;
    case 0xd533: // [$D533..$D536]    4 bytes
      CYCLES(0xd533, 7);
      /* $D533 CPX */ update_nz_inv_c(s_x - 0xef);
      /* $D535 BCC */ s_pc = !(s_status & STATUS_C) ? 0xd539 : 0xd537;
      branchTarget = true;
      break;
    case 0xd537: // [$D537..$D538]    2 bytes
      CYCLES(0xd537, 4);
      /* $D537 LDX */ s_x = update_nz(0xef);
    case 0xd539: // [$D539..$D540]    8 bytes
      CYCLES(0xd539, 14);
      /* $D539 LDA */ s_a = update_nz(0x00);
      /* $D53B STA */ poke(0x0200 + s_x, s_a);
      /* $D53E TXA */ s_a = update_nz(s_x);
      /* $D53F BEQ */ s_pc = s_status & STATUS_Z ? 0xd54c : 0xd541;
      branchTarget = true;
      break;
    case 0xd541: // [$D541..$D54B]   11 bytes
      CYCLES(0xd541, 19);
      /* $D541 LDA */ s_a = update_nz(peek(0x01ff + s_x));
      /* $D544 AND */ s_a = update_nz(s_a & 0x7f);
      /* $D546 STA */ poke(0x01ff + s_x, s_a);
      /* $D549 DEX */ s_x = update_nz(s_x - 1);
      /* $D54A BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd541 : 0xd54c;
      branchTarget = true;
      break;
    case 0xd54c: // [$D54C..$D552]    7 bytes
      CYCLES(0xd54c, 12);
      /* $D54C LDA */ s_a = update_nz(0x00);
      /* $D54E LDX */ s_x = update_nz(0xff);
      /* $D550 LDY */ s_y = update_nz(0x01);
      /* $D552 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xd553: // [$D553..$D555]    3 bytes
      CYCLES(0xd553, 6);
      /* $D553 JSR */ push16(0xd555), s_pc = 0xfd0c;
      branchTarget = true;
      break;
    case 0xd556: // [$D556..$D558]    3 bytes
      CYCLES(0xd556, 6);
      /* $D556 AND */ s_a = update_nz(s_a & 0x7f);
      /* $D558 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xd559: // [$D559..$D563]   11 bytes
      CYCLES(0xd559, 19);
      /* $D559 LDX */ s_x = update_nz(peek_zpg(0xb8));
      /* $D55B DEX */ s_x = update_nz(s_x - 1);
      /* $D55C LDY */ s_y = update_nz(0x04);
      /* $D55E STY */ poke_zpg(0x13, s_y);
      /* $D560 BIT */ tmp = peek_zpg(0xd6), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $D562 BPL */ s_pc = !(s_status & STATUS_N) ? 0xd56c : 0xd564;
      branchTarget = true;
      break;
    case 0xd564: // [$D564..$D568]    5 bytes
      CYCLES(0xd564, 9);
      /* $D564 PLA */ s_a = update_nz(pop8());
      /* $D565 PLA */ s_a = update_nz(pop8());
      /* $D566 JSR */ push16(0xd568), s_pc = 0xd665;
      branchTarget = true;
      break;
    case 0xd569: // [$D569..$D56B]    3 bytes
      CYCLES(0xd569, 6);
      /* $D569 JMP */ s_pc = 0xd7d2;
      branchTarget = true;
      break;
    case 0xd56c: // [$D56C..$D56C]    1 bytes
      CYCLES(0xd56c, 2);
      /* $D56C INX */ s_x = update_nz(s_x + 1);
    case 0xd56d: // [$D56D..$D573]    7 bytes
      CYCLES(0xd56d, 12);
      /* $D56D LDA */ s_a = update_nz(peek(0x0200 + s_x));
      /* $D570 BIT */ tmp = peek_zpg(0x13), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $D572 BVS */ s_pc = s_status & STATUS_V ? 0xd578 : 0xd574;
      branchTarget = true;
      break;
    case 0xd574: // [$D574..$D577]    4 bytes
      CYCLES(0xd574, 7);
      /* $D574 CMP */ update_nz_inv_c(s_a - 0x20);
      /* $D576 BEQ */ s_pc = s_status & STATUS_Z ? 0xd56c : 0xd578;
      branchTarget = true;
      break;
    case 0xd578: // [$D578..$D57D]    6 bytes
      CYCLES(0xd578, 11);
      /* $D578 STA */ poke_zpg(0x0e, s_a);
      /* $D57A CMP */ update_nz_inv_c(s_a - 0x22);
      /* $D57C BEQ */ s_pc = s_status & STATUS_Z ? 0xd5f2 : 0xd57e;
      branchTarget = true;
      break;
    case 0xd57e: // [$D57E..$D57F]    2 bytes
      CYCLES(0xd57e, 4);
      /* $D57E BVS */ s_pc = s_status & STATUS_V ? 0xd5cd : 0xd580;
      branchTarget = true;
      break;
    case 0xd580: // [$D580..$D583]    4 bytes
      CYCLES(0xd580, 7);
      /* $D580 CMP */ update_nz_inv_c(s_a - 0x3f);
      /* $D582 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd588 : 0xd584;
      branchTarget = true;
      break;
    case 0xd584: // [$D584..$D587]    4 bytes
      CYCLES(0xd584, 7);
      /* $D584 LDA */ s_a = update_nz(0xba);
      /* $D586 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd5cd : 0xd588;
      branchTarget = true;
      break;
    case 0xd588: // [$D588..$D58B]    4 bytes
      CYCLES(0xd588, 7);
      /* $D588 CMP */ update_nz_inv_c(s_a - 0x30);
      /* $D58A BCC */ s_pc = !(s_status & STATUS_C) ? 0xd590 : 0xd58c;
      branchTarget = true;
      break;
    case 0xd58c: // [$D58C..$D58F]    4 bytes
      CYCLES(0xd58c, 7);
      /* $D58C CMP */ update_nz_inv_c(s_a - 0x3c);
      /* $D58E BCC */ s_pc = !(s_status & STATUS_C) ? 0xd5cd : 0xd590;
      branchTarget = true;
      break;
    case 0xd590: // [$D590..$D5A1]   18 bytes
      CYCLES(0xd590, 31);
      /* $D590 STY */ poke_zpg(0xad, s_y);
      /* $D592 LDA */ s_a = update_nz(0xd0);
      /* $D594 STA */ poke_zpg(0x9d, s_a);
      /* $D596 LDA */ s_a = update_nz(0xcf);
      /* $D598 STA */ poke_zpg(0x9e, s_a);
      /* $D59A LDY */ s_y = update_nz(0x00);
      /* $D59C STY */ poke_zpg(0x0f, s_y);
      /* $D59E DEY */ s_y = update_nz(s_y - 1);
      /* $D59F STX */ poke_zpg(0xb8, s_x);
      /* $D5A1 DEX */ s_x = update_nz(s_x - 1);
    case 0xd5a2: // [$D5A2..$D5A4]    3 bytes
      CYCLES(0xd5a2, 6);
      /* $D5A2 INY */ s_y = update_nz(s_y + 1);
      /* $D5A3 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd5a7 : 0xd5a5;
      branchTarget = true;
      break;
    case 0xd5a5: // [$D5A5..$D5A6]    2 bytes
      CYCLES(0xd5a5, 4);
      /* $D5A5 INC */ tmp16 = 0x9e, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
    case 0xd5a7: // [$D5A7..$D5A7]    1 bytes
      CYCLES(0xd5a7, 2);
      /* $D5A7 INX */ s_x = update_nz(s_x + 1);
    case 0xd5a8: // [$D5A8..$D5AE]    7 bytes
      CYCLES(0xd5a8, 12);
      /* $D5A8 LDA */ s_a = update_nz(peek(0x0200 + s_x));
      /* $D5AB CMP */ update_nz_inv_c(s_a - 0x20);
      /* $D5AD BEQ */ s_pc = s_status & STATUS_Z ? 0xd5a7 : 0xd5af;
      branchTarget = true;
      break;
    case 0xd5af: // [$D5AF..$D5B3]    5 bytes
      CYCLES(0xd5af, 9);
      /* $D5AF SEC */ s_status |= STATUS_C;
      /* $D5B0 SBC */ tmp = peek(peek16_zpg(0x9d) + s_y), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $D5B2 BEQ */ s_pc = s_status & STATUS_Z ? 0xd5a2 : 0xd5b4;
      branchTarget = true;
      break;
    case 0xd5b4: // [$D5B4..$D5B7]    4 bytes
      CYCLES(0xd5b4, 7);
      /* $D5B4 CMP */ update_nz_inv_c(s_a - 0x80);
      /* $D5B6 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd5f9 : 0xd5b8;
      branchTarget = true;
      break;
    case 0xd5b8: // [$D5B8..$D5BD]    6 bytes
      CYCLES(0xd5b8, 11);
      /* $D5B8 ORA */ s_a = update_nz(s_a | peek_zpg(0x0f));
      /* $D5BA CMP */ update_nz_inv_c(s_a - 0xc5);
      /* $D5BC BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd5cb : 0xd5be;
      branchTarget = true;
      break;
    case 0xd5be: // [$D5BE..$D5C4]    7 bytes
      CYCLES(0xd5be, 12);
      /* $D5BE LDA */ s_a = update_nz(peek(0x0201 + s_x));
      /* $D5C1 CMP */ update_nz_inv_c(s_a - 0x4e);
      /* $D5C3 BEQ */ s_pc = s_status & STATUS_Z ? 0xd5f9 : 0xd5c5;
      branchTarget = true;
      break;
    case 0xd5c5: // [$D5C5..$D5C8]    4 bytes
      CYCLES(0xd5c5, 7);
      /* $D5C5 CMP */ update_nz_inv_c(s_a - 0x4f);
      /* $D5C7 BEQ */ s_pc = s_status & STATUS_Z ? 0xd5f9 : 0xd5c9;
      branchTarget = true;
      break;
    case 0xd5c9: // [$D5C9..$D5CA]    2 bytes
      CYCLES(0xd5c9, 4);
      /* $D5C9 LDA */ s_a = update_nz(0xc5);
    case 0xd5cb: // [$D5CB..$D5CC]    2 bytes
      CYCLES(0xd5cb, 4);
      /* $D5CB LDY */ s_y = update_nz(peek_zpg(0xad));
    case 0xd5cd: // [$D5CD..$D5D6]   10 bytes
      CYCLES(0xd5cd, 18);
      /* $D5CD INX */ s_x = update_nz(s_x + 1);
      /* $D5CE INY */ s_y = update_nz(s_y + 1);
      /* $D5CF STA */ poke(0x01fb + s_y, s_a);
      /* $D5D2 LDA */ s_a = update_nz(peek(0x01fb + s_y));
      /* $D5D5 BEQ */ s_pc = s_status & STATUS_Z ? 0xd610 : 0xd5d7;
      branchTarget = true;
      break;
    case 0xd5d7: // [$D5D7..$D5DB]    5 bytes
      CYCLES(0xd5d7, 9);
      /* $D5D7 SEC */ s_status |= STATUS_C;
      /* $D5D8 SBC */ tmp = 0x3a, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $D5DA BEQ */ s_pc = s_status & STATUS_Z ? 0xd5e0 : 0xd5dc;
      branchTarget = true;
      break;
    case 0xd5dc: // [$D5DC..$D5DF]    4 bytes
      CYCLES(0xd5dc, 7);
      /* $D5DC CMP */ update_nz_inv_c(s_a - 0x49);
      /* $D5DE BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd5e2 : 0xd5e0;
      branchTarget = true;
      break;
    case 0xd5e0: // [$D5E0..$D5E1]    2 bytes
      CYCLES(0xd5e0, 4);
      /* $D5E0 STA */ poke_zpg(0x13, s_a);
    case 0xd5e2: // [$D5E2..$D5E6]    5 bytes
      CYCLES(0xd5e2, 9);
      /* $D5E2 SEC */ s_status |= STATUS_C;
      /* $D5E3 SBC */ tmp = 0x78, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $D5E5 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd56d : 0xd5e7;
      branchTarget = true;
      break;
    case 0xd5e7: // [$D5E7..$D5E8]    2 bytes
      CYCLES(0xd5e7, 4);
      /* $D5E7 STA */ poke_zpg(0x0e, s_a);
    case 0xd5e9: // [$D5E9..$D5ED]    5 bytes
      CYCLES(0xd5e9, 9);
      /* $D5E9 LDA */ s_a = update_nz(peek(0x0200 + s_x));
      /* $D5EC BEQ */ s_pc = s_status & STATUS_Z ? 0xd5cd : 0xd5ee;
      branchTarget = true;
      break;
    case 0xd5ee: // [$D5EE..$D5F1]    4 bytes
      CYCLES(0xd5ee, 7);
      /* $D5EE CMP */ update_nz_inv_c(s_a - peek_zpg(0x0e));
      /* $D5F0 BEQ */ s_pc = s_status & STATUS_Z ? 0xd5cd : 0xd5f2;
      branchTarget = true;
      break;
    case 0xd5f2: // [$D5F2..$D5F8]    7 bytes
      CYCLES(0xd5f2, 12);
      /* $D5F2 INY */ s_y = update_nz(s_y + 1);
      /* $D5F3 STA */ poke(0x01fb + s_y, s_a);
      /* $D5F6 INX */ s_x = update_nz(s_x + 1);
      /* $D5F7 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd5e9 : 0xd5f9;
      branchTarget = true;
      break;
    case 0xd5f9: // [$D5F9..$D5FC]    4 bytes
      CYCLES(0xd5f9, 7);
      /* $D5F9 LDX */ s_x = update_nz(peek_zpg(0xb8));
      /* $D5FB INC */ tmp16 = 0x0f, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
    case 0xd5fd: // [$D5FD..$D601]    5 bytes
      CYCLES(0xd5fd, 9);
      /* $D5FD LDA */ s_a = update_nz(peek(peek16_zpg(0x9d) + s_y));
      /* $D5FF INY */ s_y = update_nz(s_y + 1);
      /* $D600 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd604 : 0xd602;
      branchTarget = true;
      break;
    case 0xd602: // [$D602..$D603]    2 bytes
      CYCLES(0xd602, 4);
      /* $D602 INC */ tmp16 = 0x9e, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
    case 0xd604: // [$D604..$D606]    3 bytes
      CYCLES(0xd604, 6);
      /* $D604 ASL */ s_a = update_nzc(s_a << 1);
      /* $D605 BCC */ s_pc = !(s_status & STATUS_C) ? 0xd5fd : 0xd607;
      branchTarget = true;
      break;
    case 0xd607: // [$D607..$D60A]    4 bytes
      CYCLES(0xd607, 7);
      /* $D607 LDA */ s_a = update_nz(peek(peek16_zpg(0x9d) + s_y));
      /* $D609 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd5a8 : 0xd60b;
      branchTarget = true;
      break;
    case 0xd60b: // [$D60B..$D60F]    5 bytes
      CYCLES(0xd60b, 9);
      /* $D60B LDA */ s_a = update_nz(peek(0x0200 + s_x));
      /* $D60E BPL */ s_pc = !(s_status & STATUS_N) ? 0xd5cb : 0xd610;
      branchTarget = true;
      break;
    case 0xd610: // [$D610..$D619]   10 bytes
      CYCLES(0xd610, 18);
      /* $D610 STA */ poke(0x01fd + s_y, s_a);
      /* $D613 DEC */ tmp16 = 0xb9, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $D615 LDA */ s_a = update_nz(0xff);
      /* $D617 STA */ poke_zpg(0xb8, s_a);
      /* $D619 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xd61a: // [$D61A..$D61D]    4 bytes
      CYCLES(0xd61a, 7);
      /* $D61A LDA */ s_a = update_nz(peek_zpg(0x67));
      /* $D61C LDX */ s_x = update_nz(peek_zpg(0x68));
    case 0xd61e: // [$D61E..$D627]   10 bytes
      CYCLES(0xd61e, 18);
      /* $D61E LDY */ s_y = update_nz(0x01);
      /* $D620 STA */ poke_zpg(0x9b, s_a);
      /* $D622 STX */ poke_zpg(0x9c, s_x);
      /* $D624 LDA */ s_a = update_nz(peek(peek16_zpg(0x9b) + s_y));
      /* $D626 BEQ */ s_pc = s_status & STATUS_Z ? 0xd647 : 0xd628;
      branchTarget = true;
      break;
    case 0xd628: // [$D628..$D62F]    8 bytes
      CYCLES(0xd628, 14);
      /* $D628 INY */ s_y = update_nz(s_y + 1);
      /* $D629 INY */ s_y = update_nz(s_y + 1);
      /* $D62A LDA */ s_a = update_nz(peek_zpg(0x51));
      /* $D62C CMP */ update_nz_inv_c(s_a - peek(peek16_zpg(0x9b) + s_y));
      /* $D62E BCC */ s_pc = !(s_status & STATUS_C) ? 0xd648 : 0xd630;
      branchTarget = true;
      break;
    case 0xd630: // [$D630..$D631]    2 bytes
      CYCLES(0xd630, 4);
      /* $D630 BEQ */ s_pc = s_status & STATUS_Z ? 0xd635 : 0xd632;
      branchTarget = true;
      break;
    case 0xd632: // [$D632..$D634]    3 bytes
      CYCLES(0xd632, 6);
      /* $D632 DEY */ s_y = update_nz(s_y - 1);
      /* $D633 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd63e : 0xd635;
      branchTarget = true;
      break;
    case 0xd635: // [$D635..$D63B]    7 bytes
      CYCLES(0xd635, 12);
      /* $D635 LDA */ s_a = update_nz(peek_zpg(0x50));
      /* $D637 DEY */ s_y = update_nz(s_y - 1);
      /* $D638 CMP */ update_nz_inv_c(s_a - peek(peek16_zpg(0x9b) + s_y));
      /* $D63A BCC */ s_pc = !(s_status & STATUS_C) ? 0xd648 : 0xd63c;
      branchTarget = true;
      break;
    case 0xd63c: // [$D63C..$D63D]    2 bytes
      CYCLES(0xd63c, 4);
      /* $D63C BEQ */ s_pc = s_status & STATUS_Z ? 0xd648 : 0xd63e;
      branchTarget = true;
      break;
    case 0xd63e: // [$D63E..$D646]    9 bytes
      CYCLES(0xd63e, 16);
      /* $D63E DEY */ s_y = update_nz(s_y - 1);
      /* $D63F LDA */ s_a = update_nz(peek(peek16_zpg(0x9b) + s_y));
      /* $D641 TAX */ s_x = update_nz(s_a);
      /* $D642 DEY */ s_y = update_nz(s_y - 1);
      /* $D643 LDA */ s_a = update_nz(peek(peek16_zpg(0x9b) + s_y));
      /* $D645 BCS */ s_pc = s_status & STATUS_C ? 0xd61e : 0xd647;
      branchTarget = true;
      break;
    case 0xd647: // [$D647..$D647]    1 bytes
      CYCLES(0xd647, 2);
      /* $D647 CLC */ s_status &= ~STATUS_C;
    case 0xd648: // [$D648..$D648]    1 bytes
      CYCLES(0xd648, 2);
      /* $D648 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xd64b: // [$D64B..$D664]   26 bytes
      CYCLES(0xd64b, 45);
      /* $D64B LDA */ s_a = update_nz(0x00);
      /* $D64D STA */ poke_zpg(0xd6, s_a);
      /* $D64F TAY */ s_y = update_nz(s_a);
      /* $D650 STA */ poke(peek16_zpg(0x67) + s_y, s_a);
      /* $D652 INY */ s_y = update_nz(s_y + 1);
      /* $D653 STA */ poke(peek16_zpg(0x67) + s_y, s_a);
      /* $D655 LDA */ s_a = update_nz(peek_zpg(0x67));
      /* $D657 ADC */ tmp = 0x02, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $D659 STA */ poke_zpg(0x69, s_a);
      /* $D65B STA */ poke_zpg(0xaf, s_a);
      /* $D65D LDA */ s_a = update_nz(peek_zpg(0x68));
      /* $D65F ADC */ tmp = 0x00, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $D661 STA */ poke_zpg(0x6a, s_a);
      /* $D663 STA */ poke_zpg(0xb0, s_a);
    case 0xd665: // [$D665..$D667]    3 bytes
      CYCLES(0xd665, 6);
      /* $D665 JSR */ push16(0xd667), s_pc = 0xd697;
      branchTarget = true;
      break;
    case 0xd668: // [$D668..$D66B]    4 bytes
      CYCLES(0xd668, 7);
      /* $D668 LDA */ s_a = update_nz(0x00);
      /* $D66A BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd696 : 0xd66c;
      branchTarget = true;
      break;
    case 0xd66c: // [$D66C..$D682]   23 bytes
      CYCLES(0xd66c, 40);
      /* $D66C LDA */ s_a = update_nz(peek_zpg(0x73));
      /* $D66E LDY */ s_y = update_nz(peek_zpg(0x74));
      /* $D670 STA */ poke_zpg(0x6f, s_a);
      /* $D672 STY */ poke_zpg(0x70, s_y);
      /* $D674 LDA */ s_a = update_nz(peek_zpg(0x69));
      /* $D676 LDY */ s_y = update_nz(peek_zpg(0x6a));
      /* $D678 STA */ poke_zpg(0x6b, s_a);
      /* $D67A STY */ poke_zpg(0x6c, s_y);
      /* $D67C STA */ poke_zpg(0x6d, s_a);
      /* $D67E STY */ poke_zpg(0x6e, s_y);
      /* $D680 JSR */ push16(0xd682), s_pc = 0xd849;
      branchTarget = true;
      break;
    case 0xd683: // [$D683..$D695]   19 bytes
      CYCLES(0xd683, 33);
      /* $D683 LDX */ s_x = update_nz(0x55);
      /* $D685 STX */ poke_zpg(0x52, s_x);
      /* $D687 PLA */ s_a = update_nz(pop8());
      /* $D688 TAY */ s_y = update_nz(s_a);
      /* $D689 PLA */ s_a = update_nz(pop8());
      /* $D68A LDX */ s_x = update_nz(0xf8);
      /* $D68C TXS */ s_sp = s_x;
      /* $D68D PHA */ push8(s_a);
      /* $D68E TYA */ s_a = update_nz(s_y);
      /* $D68F PHA */ push8(s_a);
      /* $D690 LDA */ s_a = update_nz(0x00);
      /* $D692 STA */ poke_zpg(0x7a, s_a);
      /* $D694 STA */ poke_zpg(0x14, s_a);
    case 0xd696: // [$D696..$D696]    1 bytes
      CYCLES(0xd696, 2);
      /* $D696 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xd697: // [$D697..$D6A4]   14 bytes
      CYCLES(0xd697, 24);
      /* $D697 CLC */ s_status &= ~STATUS_C;
      /* $D698 LDA */ s_a = update_nz(peek_zpg(0x67));
      /* $D69A ADC */ tmp = 0xff, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $D69C STA */ poke_zpg(0xb8, s_a);
      /* $D69E LDA */ s_a = update_nz(peek_zpg(0x68));
      /* $D6A0 ADC */ tmp = 0xff, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $D6A2 STA */ poke_zpg(0xb9, s_a);
      /* $D6A4 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xd6a5: // [$D6A5..$D6A6]    2 bytes
      CYCLES(0xd6a5, 4);
      /* $D6A5 BCC */ s_pc = !(s_status & STATUS_C) ? 0xd6b1 : 0xd6a7;
      branchTarget = true;
      break;
    case 0xd6a7: // [$D6A7..$D6A8]    2 bytes
      CYCLES(0xd6a7, 4);
      /* $D6A7 BEQ */ s_pc = s_status & STATUS_Z ? 0xd6b1 : 0xd6a9;
      branchTarget = true;
      break;
    case 0xd6a9: // [$D6A9..$D6AC]    4 bytes
      CYCLES(0xd6a9, 7);
      /* $D6A9 CMP */ update_nz_inv_c(s_a - 0xc9);
      /* $D6AB BEQ */ s_pc = s_status & STATUS_Z ? 0xd6b1 : 0xd6ad;
      branchTarget = true;
      break;
    case 0xd6ad: // [$D6AD..$D6B0]    4 bytes
      CYCLES(0xd6ad, 7);
      /* $D6AD CMP */ update_nz_inv_c(s_a - 0x2c);
      /* $D6AF BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd696 : 0xd6b1;
      branchTarget = true;
      break;
    case 0xd6b1: // [$D6B1..$D6B3]    3 bytes
      CYCLES(0xd6b1, 6);
      /* $D6B1 JSR */ push16(0xd6b3), s_pc = 0xda0c;
      branchTarget = true;
      break;
    case 0xd6b4: // [$D6B4..$D6B6]    3 bytes
      CYCLES(0xd6b4, 6);
      /* $D6B4 JSR */ push16(0xd6b6), s_pc = 0xd61a;
      branchTarget = true;
      break;
    case 0xd6b7: // [$D6B7..$D6B9]    3 bytes
      CYCLES(0xd6b7, 6);
      /* $D6B7 JSR */ push16(0xd6b9), s_pc = 0x00b7;
      branchTarget = true;
      break;
    case 0xd6ba: // [$D6BA..$D6BB]    2 bytes
      CYCLES(0xd6ba, 4);
      /* $D6BA BEQ */ s_pc = s_status & STATUS_Z ? 0xd6cc : 0xd6bc;
      branchTarget = true;
      break;
    case 0xd6bc: // [$D6BC..$D6BF]    4 bytes
      CYCLES(0xd6bc, 7);
      /* $D6BC CMP */ update_nz_inv_c(s_a - 0xc9);
      /* $D6BE BEQ */ s_pc = s_status & STATUS_Z ? 0xd6c4 : 0xd6c0;
      branchTarget = true;
      break;
    case 0xd6c0: // [$D6C0..$D6C3]    4 bytes
      CYCLES(0xd6c0, 7);
      /* $D6C0 CMP */ update_nz_inv_c(s_a - 0x2c);
      /* $D6C2 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd648 : 0xd6c4;
      branchTarget = true;
      break;
    case 0xd6c4: // [$D6C4..$D6C6]    3 bytes
      CYCLES(0xd6c4, 6);
      /* $D6C4 JSR */ push16(0xd6c6), s_pc = 0x00b1;
      branchTarget = true;
      break;
    case 0xd6c7: // [$D6C7..$D6C9]    3 bytes
      CYCLES(0xd6c7, 6);
      /* $D6C7 JSR */ push16(0xd6c9), s_pc = 0xda0c;
      branchTarget = true;
      break;
    case 0xd6ca: // [$D6CA..$D6CB]    2 bytes
      CYCLES(0xd6ca, 4);
      /* $D6CA BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd696 : 0xd6cc;
      branchTarget = true;
      break;
    case 0xd6cc: // [$D6CC..$D6D3]    8 bytes
      CYCLES(0xd6cc, 14);
      /* $D6CC PLA */ s_a = update_nz(pop8());
      /* $D6CD PLA */ s_a = update_nz(pop8());
      /* $D6CE LDA */ s_a = update_nz(peek_zpg(0x50));
      /* $D6D0 ORA */ s_a = update_nz(s_a | peek_zpg(0x51));
      /* $D6D2 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd6da : 0xd6d4;
      branchTarget = true;
      break;
    case 0xd6d4: // [$D6D4..$D6D9]    6 bytes
      CYCLES(0xd6d4, 11);
      /* $D6D4 LDA */ s_a = update_nz(0xff);
      /* $D6D6 STA */ poke_zpg(0x50, s_a);
      /* $D6D8 STA */ poke_zpg(0x51, s_a);
    case 0xd6da: // [$D6DA..$D6DF]    6 bytes
      CYCLES(0xd6da, 11);
      /* $D6DA LDY */ s_y = update_nz(0x01);
      /* $D6DC LDA */ s_a = update_nz(peek(peek16_zpg(0x9b) + s_y));
      /* $D6DE BEQ */ s_pc = s_status & STATUS_Z ? 0xd724 : 0xd6e0;
      branchTarget = true;
      break;
    case 0xd6e0: // [$D6E0..$D6E2]    3 bytes
      CYCLES(0xd6e0, 6);
      /* $D6E0 JSR */ push16(0xd6e2), s_pc = 0xd858;
      branchTarget = true;
      break;
    case 0xd6e3: // [$D6E3..$D6E5]    3 bytes
      CYCLES(0xd6e3, 6);
      /* $D6E3 JSR */ push16(0xd6e5), s_pc = 0xdafb;
      branchTarget = true;
      break;
    case 0xd6e6: // [$D6E6..$D6F0]   11 bytes
      CYCLES(0xd6e6, 19);
      /* $D6E6 INY */ s_y = update_nz(s_y + 1);
      /* $D6E7 LDA */ s_a = update_nz(peek(peek16_zpg(0x9b) + s_y));
      /* $D6E9 TAX */ s_x = update_nz(s_a);
      /* $D6EA INY */ s_y = update_nz(s_y + 1);
      /* $D6EB LDA */ s_a = update_nz(peek(peek16_zpg(0x9b) + s_y));
      /* $D6ED CMP */ update_nz_inv_c(s_a - peek_zpg(0x51));
      /* $D6EF BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd6f5 : 0xd6f1;
      branchTarget = true;
      break;
    case 0xd6f1: // [$D6F1..$D6F4]    4 bytes
      CYCLES(0xd6f1, 7);
      /* $D6F1 CPX */ update_nz_inv_c(s_x - peek_zpg(0x50));
      /* $D6F3 BEQ */ s_pc = s_status & STATUS_Z ? 0xd6f7 : 0xd6f5;
      branchTarget = true;
      break;
    case 0xd6f5: // [$D6F5..$D6F6]    2 bytes
      CYCLES(0xd6f5, 4);
      /* $D6F5 BCS */ s_pc = s_status & STATUS_C ? 0xd724 : 0xd6f7;
      branchTarget = true;
      break;
    case 0xd6f7: // [$D6F7..$D6FB]    5 bytes
      CYCLES(0xd6f7, 9);
      /* $D6F7 STY */ poke_zpg(0x85, s_y);
      /* $D6F9 JSR */ push16(0xd6fb), s_pc = 0xed24;
      branchTarget = true;
      break;
    case 0xd6fc: // [$D6FC..$D6FD]    2 bytes
      CYCLES(0xd6fc, 4);
      /* $D6FC LDA */ s_a = update_nz(0x20);
    case 0xd6fe: // [$D6FE..$D701]    4 bytes
      CYCLES(0xd6fe, 7);
      /* $D6FE LDY */ s_y = update_nz(peek_zpg(0x85));
      /* $D700 AND */ s_a = update_nz(s_a & 0x7f);
    case 0xd702: // [$D702..$D704]    3 bytes
      CYCLES(0xd702, 6);
      /* $D702 JSR */ push16(0xd704), s_pc = 0xdb5c;
      branchTarget = true;
      break;
    case 0xd705: // [$D705..$D70A]    6 bytes
      CYCLES(0xd705, 11);
      /* $D705 LDA */ s_a = update_nz(peek_zpg(0x24));
      /* $D707 CMP */ update_nz_inv_c(s_a - 0x21);
      /* $D709 BCC */ s_pc = !(s_status & STATUS_C) ? 0xd712 : 0xd70b;
      branchTarget = true;
      break;
    case 0xd70b: // [$D70B..$D70D]    3 bytes
      CYCLES(0xd70b, 6);
      /* $D70B JSR */ push16(0xd70d), s_pc = 0xdafb;
      branchTarget = true;
      break;
    case 0xd70e: // [$D70E..$D711]    4 bytes
      CYCLES(0xd70e, 7);
      /* $D70E LDA */ s_a = update_nz(0x05);
      /* $D710 STA */ poke_zpg(0x24, s_a);
    case 0xd712: // [$D712..$D716]    5 bytes
      CYCLES(0xd712, 9);
      /* $D712 INY */ s_y = update_nz(s_y + 1);
      /* $D713 LDA */ s_a = update_nz(peek(peek16_zpg(0x9b) + s_y));
      /* $D715 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd734 : 0xd717;
      branchTarget = true;
      break;
    case 0xd717: // [$D717..$D723]   13 bytes
      CYCLES(0xd717, 23);
      /* $D717 TAY */ s_y = update_nz(s_a);
      /* $D718 LDA */ s_a = update_nz(peek(peek16_zpg(0x9b) + s_y));
      /* $D71A TAX */ s_x = update_nz(s_a);
      /* $D71B INY */ s_y = update_nz(s_y + 1);
      /* $D71C LDA */ s_a = update_nz(peek(peek16_zpg(0x9b) + s_y));
      /* $D71E STX */ poke_zpg(0x9b, s_x);
      /* $D720 STA */ poke_zpg(0x9c, s_a);
      /* $D722 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd6da : 0xd724;
      branchTarget = true;
      break;
    case 0xd724: // [$D724..$D728]    5 bytes
      CYCLES(0xd724, 9);
      /* $D724 LDA */ s_a = update_nz(0x0d);
      /* $D726 JSR */ push16(0xd728), s_pc = 0xdb5c;
      branchTarget = true;
      break;
    case 0xd729: // [$D729..$D72B]    3 bytes
      CYCLES(0xd729, 6);
      /* $D729 JMP */ s_pc = 0xd7d2;
      branchTarget = true;
      break;
    case 0xd72c: // [$D72C..$D72E]    3 bytes
      CYCLES(0xd72c, 6);
      /* $D72C INY */ s_y = update_nz(s_y + 1);
      /* $D72D BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd731 : 0xd72f;
      branchTarget = true;
      break;
    case 0xd72f: // [$D72F..$D730]    2 bytes
      CYCLES(0xd72f, 4);
      /* $D72F INC */ tmp16 = 0x9e, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
    case 0xd731: // [$D731..$D733]    3 bytes
      CYCLES(0xd731, 6);
      /* $D731 LDA */ s_a = update_nz(peek(peek16_zpg(0x9d) + s_y));
      /* $D733 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xd734: // [$D734..$D735]    2 bytes
      CYCLES(0xd734, 4);
      /* $D734 BPL */ s_pc = !(s_status & STATUS_N) ? 0xd702 : 0xd736;
      branchTarget = true;
      break;
    case 0xd736: // [$D736..$D745]   16 bytes
      CYCLES(0xd736, 28);
      /* $D736 SEC */ s_status |= STATUS_C;
      /* $D737 SBC */ tmp = 0x7f, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $D739 TAX */ s_x = update_nz(s_a);
      /* $D73A STY */ poke_zpg(0x85, s_y);
      /* $D73C LDY */ s_y = update_nz(0xd0);
      /* $D73E STY */ poke_zpg(0x9d, s_y);
      /* $D740 LDY */ s_y = update_nz(0xcf);
      /* $D742 STY */ poke_zpg(0x9e, s_y);
      /* $D744 LDY */ s_y = update_nz(0xff);
    case 0xd746: // [$D746..$D748]    3 bytes
      CYCLES(0xd746, 6);
      /* $D746 DEX */ s_x = update_nz(s_x - 1);
      /* $D747 BEQ */ s_pc = s_status & STATUS_Z ? 0xd750 : 0xd749;
      branchTarget = true;
      break;
    case 0xd749: // [$D749..$D74B]    3 bytes
      CYCLES(0xd749, 6);
      /* $D749 JSR */ push16(0xd74b), s_pc = 0xd72c;
      branchTarget = true;
      break;
    case 0xd74c: // [$D74C..$D74D]    2 bytes
      CYCLES(0xd74c, 4);
      /* $D74C BPL */ s_pc = !(s_status & STATUS_N) ? 0xd749 : 0xd74e;
      branchTarget = true;
      break;
    case 0xd74e: // [$D74E..$D74F]    2 bytes
      CYCLES(0xd74e, 4);
      /* $D74E BMI */ s_pc = s_status & STATUS_N ? 0xd746 : 0xd750;
      branchTarget = true;
      break;
    case 0xd750: // [$D750..$D754]    5 bytes
      CYCLES(0xd750, 9);
      /* $D750 LDA */ s_a = update_nz(0x20);
      /* $D752 JSR */ push16(0xd754), s_pc = 0xdb5c;
      branchTarget = true;
      break;
    case 0xd755: // [$D755..$D757]    3 bytes
      CYCLES(0xd755, 6);
      /* $D755 JSR */ push16(0xd757), s_pc = 0xd72c;
      branchTarget = true;
      break;
    case 0xd758: // [$D758..$D759]    2 bytes
      CYCLES(0xd758, 4);
      /* $D758 BMI */ s_pc = s_status & STATUS_N ? 0xd75f : 0xd75a;
      branchTarget = true;
      break;
    case 0xd75a: // [$D75A..$D75C]    3 bytes
      CYCLES(0xd75a, 6);
      /* $D75A JSR */ push16(0xd75c), s_pc = 0xdb5c;
      branchTarget = true;
      break;
    case 0xd75d: // [$D75D..$D75E]    2 bytes
      CYCLES(0xd75d, 4);
      /* $D75D BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd755 : 0xd75f;
      branchTarget = true;
      break;
    case 0xd75f: // [$D75F..$D761]    3 bytes
      CYCLES(0xd75f, 6);
      /* $D75F JSR */ push16(0xd761), s_pc = 0xdb5c;
      branchTarget = true;
      break;
    case 0xd762: // [$D762..$D765]    4 bytes
      CYCLES(0xd762, 7);
      /* $D762 LDA */ s_a = update_nz(0x20);
      /* $D764 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd6fe : 0xd766;
      branchTarget = true;
      break;
    case 0xd766: // [$D766..$D76C]    7 bytes
      CYCLES(0xd766, 12);
      /* $D766 LDA */ s_a = update_nz(0x80);
      /* $D768 STA */ poke_zpg(0x14, s_a);
      /* $D76A JSR */ push16(0xd76c), s_pc = 0xda46;
      branchTarget = true;
      break;
    case 0xd76d: // [$D76D..$D76F]    3 bytes
      CYCLES(0xd76d, 6);
      /* $D76D JSR */ push16(0xd76f), s_pc = 0xd365;
      branchTarget = true;
      break;
    case 0xd770: // [$D770..$D771]    2 bytes
      CYCLES(0xd770, 4);
      /* $D770 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd777 : 0xd772;
      branchTarget = true;
      break;
    case 0xd772: // [$D772..$D776]    5 bytes
      CYCLES(0xd772, 9);
      /* $D772 TXA */ s_a = update_nz(s_x);
      /* $D773 ADC */ tmp = 0x0f, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $D775 TAX */ s_x = update_nz(s_a);
      /* $D776 TXS */ s_sp = s_x;
    case 0xd777: // [$D777..$D77D]    7 bytes
      CYCLES(0xd777, 12);
      /* $D777 PLA */ s_a = update_nz(pop8());
      /* $D778 PLA */ s_a = update_nz(pop8());
      /* $D779 LDA */ s_a = update_nz(0x09);
      /* $D77B JSR */ push16(0xd77d), s_pc = 0xd3d6;
      branchTarget = true;
      break;
    case 0xd77e: // [$D77E..$D780]    3 bytes
      CYCLES(0xd77e, 6);
      /* $D77E JSR */ push16(0xd780), s_pc = 0xd9a3;
      branchTarget = true;
      break;
    case 0xd781: // [$D781..$D795]   21 bytes
      CYCLES(0xd781, 36);
      /* $D781 CLC */ s_status &= ~STATUS_C;
      /* $D782 TYA */ s_a = update_nz(s_y);
      /* $D783 ADC */ tmp = peek_zpg(0xb8), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $D785 PHA */ push8(s_a);
      /* $D786 LDA */ s_a = update_nz(peek_zpg(0xb9));
      /* $D788 ADC */ tmp = 0x00, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $D78A PHA */ push8(s_a);
      /* $D78B LDA */ s_a = update_nz(peek_zpg(0x76));
      /* $D78D PHA */ push8(s_a);
      /* $D78E LDA */ s_a = update_nz(peek_zpg(0x75));
      /* $D790 PHA */ push8(s_a);
      /* $D791 LDA */ s_a = update_nz(0xc1);
      /* $D793 JSR */ push16(0xd795), s_pc = 0xdec0;
      branchTarget = true;
      break;
    case 0xd796: // [$D796..$D798]    3 bytes
      CYCLES(0xd796, 6);
      /* $D796 JSR */ push16(0xd798), s_pc = 0xdd6a;
      branchTarget = true;
      break;
    case 0xd799: // [$D799..$D79B]    3 bytes
      CYCLES(0xd799, 6);
      /* $D799 JSR */ push16(0xd79b), s_pc = 0xdd67;
      branchTarget = true;
      break;
    case 0xd79c: // [$D79C..$D7AE]   19 bytes
      CYCLES(0xd79c, 33);
      /* $D79C LDA */ s_a = update_nz(peek_zpg(0xa2));
      /* $D79E ORA */ s_a = update_nz(s_a | 0x7f);
      /* $D7A0 AND */ s_a = update_nz(s_a & peek_zpg(0x9e));
      /* $D7A2 STA */ poke_zpg(0x9e, s_a);
      /* $D7A4 LDA */ s_a = update_nz(0xaf);
      /* $D7A6 LDY */ s_y = update_nz(0xd7);
      /* $D7A8 STA */ poke_zpg(0x5e, s_a);
      /* $D7AA STY */ poke_zpg(0x5f, s_y);
      /* $D7AC JMP */ s_pc = 0xde20;
      branchTarget = true;
      break;
    case 0xd7d2: // [$D7D2..$D7D7]    6 bytes
      CYCLES(0xd7d2, 11);
      /* $D7D2 TSX */ s_x = update_nz(s_sp);
      /* $D7D3 STX */ poke_zpg(0xf8, s_x);
      /* $D7D5 JSR */ push16(0xd7d7), s_pc = 0xd858;
      branchTarget = true;
      break;
    case 0xd7d8: // [$D7D8..$D7E0]    9 bytes
      CYCLES(0xd7d8, 16);
      /* $D7D8 LDA */ s_a = update_nz(peek_zpg(0xb8));
      /* $D7DA LDY */ s_y = update_nz(peek_zpg(0xb9));
      /* $D7DC LDX */ s_x = update_nz(peek_zpg(0x76));
      /* $D7DE INX */ s_x = update_nz(s_x + 1);
      /* $D7DF BEQ */ s_pc = s_status & STATUS_Z ? 0xd7e5 : 0xd7e1;
      branchTarget = true;
      break;
    case 0xd7e1: // [$D7E1..$D7E4]    4 bytes
      CYCLES(0xd7e1, 7);
      /* $D7E1 STA */ poke_zpg(0x79, s_a);
      /* $D7E3 STY */ poke_zpg(0x7a, s_y);
    case 0xd7e5: // [$D7E5..$D7EA]    6 bytes
      CYCLES(0xd7e5, 11);
      /* $D7E5 LDY */ s_y = update_nz(0x00);
      /* $D7E7 LDA */ s_a = update_nz(peek(peek16_zpg(0xb8) + s_y));
      /* $D7E9 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd842 : 0xd7eb;
      branchTarget = true;
      break;
    case 0xd7eb: // [$D7EB..$D7F1]    7 bytes
      CYCLES(0xd7eb, 12);
      /* $D7EB LDY */ s_y = update_nz(0x02);
      /* $D7ED LDA */ s_a = update_nz(peek(peek16_zpg(0xb8) + s_y));
      /* $D7EF CLC */ s_status &= ~STATUS_C;
      /* $D7F0 BEQ */ s_pc = s_status & STATUS_Z ? 0xd826 : 0xd7f2;
      branchTarget = true;
      break;
    case 0xd7f2: // [$D7F2..$D802]   17 bytes
      CYCLES(0xd7f2, 29);
      /* $D7F2 INY */ s_y = update_nz(s_y + 1);
      /* $D7F3 LDA */ s_a = update_nz(peek(peek16_zpg(0xb8) + s_y));
      /* $D7F5 STA */ poke_zpg(0x75, s_a);
      /* $D7F7 INY */ s_y = update_nz(s_y + 1);
      /* $D7F8 LDA */ s_a = update_nz(peek(peek16_zpg(0xb8) + s_y));
      /* $D7FA STA */ poke_zpg(0x76, s_a);
      /* $D7FC TYA */ s_a = update_nz(s_y);
      /* $D7FD ADC */ tmp = peek_zpg(0xb8), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $D7FF STA */ poke_zpg(0xb8, s_a);
      /* $D801 BCC */ s_pc = !(s_status & STATUS_C) ? 0xd805 : 0xd803;
      branchTarget = true;
      break;
    case 0xd803: // [$D803..$D804]    2 bytes
      CYCLES(0xd803, 4);
      /* $D803 INC */ tmp16 = 0xb9, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
    case 0xd805: // [$D805..$D808]    4 bytes
      CYCLES(0xd805, 7);
      /* $D805 BIT */ tmp = peek_zpg(0xf2), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $D807 BPL */ s_pc = !(s_status & STATUS_N) ? 0xd81d : 0xd809;
      branchTarget = true;
      break;
    case 0xd809: // [$D809..$D80D]    5 bytes
      CYCLES(0xd809, 9);
      /* $D809 LDX */ s_x = update_nz(peek_zpg(0x76));
      /* $D80B INX */ s_x = update_nz(s_x + 1);
      /* $D80C BEQ */ s_pc = s_status & STATUS_Z ? 0xd81d : 0xd80e;
      branchTarget = true;
      break;
    case 0xd80e: // [$D80E..$D812]    5 bytes
      CYCLES(0xd80e, 9);
      /* $D80E LDA */ s_a = update_nz(0x23);
      /* $D810 JSR */ push16(0xd812), s_pc = 0xdb5c;
      branchTarget = true;
      break;
    case 0xd813: // [$D813..$D819]    7 bytes
      CYCLES(0xd813, 12);
      /* $D813 LDX */ s_x = update_nz(peek_zpg(0x75));
      /* $D815 LDA */ s_a = update_nz(peek_zpg(0x76));
      /* $D817 JSR */ push16(0xd819), s_pc = 0xed24;
      branchTarget = true;
      break;
    case 0xd81a: // [$D81A..$D81C]    3 bytes
      CYCLES(0xd81a, 6);
      /* $D81A JSR */ push16(0xd81c), s_pc = 0xdb57;
      branchTarget = true;
      break;
    case 0xd81d: // [$D81D..$D81F]    3 bytes
      CYCLES(0xd81d, 6);
      /* $D81D JSR */ push16(0xd81f), s_pc = 0x00b1;
      branchTarget = true;
      break;
    case 0xd820: // [$D820..$D822]    3 bytes
      CYCLES(0xd820, 6);
      /* $D820 JSR */ push16(0xd822), s_pc = 0xd828;
      branchTarget = true;
      break;
    case 0xd823: // [$D823..$D825]    3 bytes
      CYCLES(0xd823, 6);
      /* $D823 JMP */ s_pc = 0xd7d2;
      branchTarget = true;
      break;
    case 0xd826: // [$D826..$D827]    2 bytes
      CYCLES(0xd826, 4);
      /* $D826 BEQ */ s_pc = s_status & STATUS_Z ? 0xd88a : 0xd828;
      branchTarget = true;
      break;
    case 0xd828: // [$D828..$D829]    2 bytes
      CYCLES(0xd828, 4);
      /* $D828 BEQ */ s_pc = s_status & STATUS_Z ? 0xd857 : 0xd82a;
      branchTarget = true;
      break;
    case 0xd82a: // [$D82A..$D82D]    4 bytes
      CYCLES(0xd82a, 7);
      /* $D82A SBC */ tmp = 0x80, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $D82C BCC */ s_pc = !(s_status & STATUS_C) ? 0xd83f : 0xd82e;
      branchTarget = true;
      break;
    case 0xd82e: // [$D82E..$D831]    4 bytes
      CYCLES(0xd82e, 7);
      /* $D82E CMP */ update_nz_inv_c(s_a - 0x40);
      /* $D830 BCS */ s_pc = s_status & STATUS_C ? 0xd846 : 0xd832;
      branchTarget = true;
      break;
    case 0xd832: // [$D832..$D83E]   13 bytes
      CYCLES(0xd832, 23);
      /* $D832 ASL */ s_a = update_nzc(s_a << 1);
      /* $D833 TAY */ s_y = update_nz(s_a);
      /* $D834 LDA */ s_a = update_nz(peek(0xd001 + s_y));
      /* $D837 PHA */ push8(s_a);
      /* $D838 LDA */ s_a = update_nz(peek(0xd000 + s_y));
      /* $D83B PHA */ push8(s_a);
      /* $D83C JMP */ s_pc = 0x00b1;
      branchTarget = true;
      break;
    case 0xd83f: // [$D83F..$D841]    3 bytes
      CYCLES(0xd83f, 6);
      /* $D83F JMP */ s_pc = 0xda46;
      branchTarget = true;
      break;
    case 0xd842: // [$D842..$D845]    4 bytes
      CYCLES(0xd842, 7);
      /* $D842 CMP */ update_nz_inv_c(s_a - 0x3a);
      /* $D844 BEQ */ s_pc = s_status & STATUS_Z ? 0xd805 : 0xd846;
      branchTarget = true;
      break;
    case 0xd846: // [$D846..$D848]    3 bytes
      CYCLES(0xd846, 6);
      /* $D846 JMP */ s_pc = 0xdec9;
      branchTarget = true;
      break;
    case 0xd849: // [$D849..$D851]    9 bytes
      CYCLES(0xd849, 16);
      /* $D849 SEC */ s_status |= STATUS_C;
      /* $D84A LDA */ s_a = update_nz(peek_zpg(0x67));
      /* $D84C SBC */ tmp = 0x01, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $D84E LDY */ s_y = update_nz(peek_zpg(0x68));
      /* $D850 BCS */ s_pc = s_status & STATUS_C ? 0xd853 : 0xd852;
      branchTarget = true;
      break;
    case 0xd852: // [$D852..$D852]    1 bytes
      CYCLES(0xd852, 2);
      /* $D852 DEY */ s_y = update_nz(s_y - 1);
    case 0xd853: // [$D853..$D856]    4 bytes
      CYCLES(0xd853, 7);
      /* $D853 STA */ poke_zpg(0x7d, s_a);
      /* $D855 STY */ poke_zpg(0x7e, s_y);
    case 0xd857: // [$D857..$D857]    1 bytes
      CYCLES(0xd857, 2);
      /* $D857 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xd858: // [$D858..$D85E]    7 bytes
      CYCLES(0xd858, 12);
      /* $D858 LDA */ s_a = update_nz(peek(0xc000));
      /* $D85B CMP */ update_nz_inv_c(s_a - 0x83);
      /* $D85D BEQ */ s_pc = s_status & STATUS_Z ? 0xd860 : 0xd85f;
      branchTarget = true;
      break;
    case 0xd85f: // [$D85F..$D85F]    1 bytes
      CYCLES(0xd85f, 2);
      /* $D85F RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xd860: // [$D860..$D862]    3 bytes
      CYCLES(0xd860, 6);
      /* $D860 JSR */ push16(0xd862), s_pc = 0xd553;
      branchTarget = true;
      break;
    case 0xd863: // [$D863..$D868]    6 bytes
      CYCLES(0xd863, 11);
      /* $D863 LDX */ s_x = update_nz(0xff);
      /* $D865 BIT */ tmp = peek_zpg(0xd8), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $D867 BPL */ s_pc = !(s_status & STATUS_N) ? 0xd86c : 0xd869;
      branchTarget = true;
      break;
    case 0xd869: // [$D869..$D86B]    3 bytes
      CYCLES(0xd869, 6);
      /* $D869 JMP */ s_pc = 0xf2e9;
      branchTarget = true;
      break;
    case 0xd86c: // [$D86C..$D86F]    4 bytes
      CYCLES(0xd86c, 7);
      /* $D86C CMP */ update_nz_inv_c(s_a - 0x03);
      /* $D86E BCS */ s_pc = s_status & STATUS_C ? 0xd871 : 0xd870;
      branchTarget = true;
      break;
    case 0xd870: // [$D870..$D870]    1 bytes
      CYCLES(0xd870, 2);
      /* $D870 CLC */ s_status &= ~STATUS_C;
    case 0xd871: // [$D871..$D872]    2 bytes
      CYCLES(0xd871, 4);
      /* $D871 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd8af : 0xd873;
      branchTarget = true;
      break;
    case 0xd873: // [$D873..$D87B]    9 bytes
      CYCLES(0xd873, 16);
      /* $D873 LDA */ s_a = update_nz(peek_zpg(0xb8));
      /* $D875 LDY */ s_y = update_nz(peek_zpg(0xb9));
      /* $D877 LDX */ s_x = update_nz(peek_zpg(0x76));
      /* $D879 INX */ s_x = update_nz(s_x + 1);
      /* $D87A BEQ */ s_pc = s_status & STATUS_Z ? 0xd888 : 0xd87c;
      branchTarget = true;
      break;
    case 0xd87c: // [$D87C..$D887]   12 bytes
      CYCLES(0xd87c, 21);
      /* $D87C STA */ poke_zpg(0x79, s_a);
      /* $D87E STY */ poke_zpg(0x7a, s_y);
      /* $D880 LDA */ s_a = update_nz(peek_zpg(0x75));
      /* $D882 LDY */ s_y = update_nz(peek_zpg(0x76));
      /* $D884 STA */ poke_zpg(0x77, s_a);
      /* $D886 STY */ poke_zpg(0x78, s_y);
    case 0xd888: // [$D888..$D889]    2 bytes
      CYCLES(0xd888, 4);
      /* $D888 PLA */ s_a = update_nz(pop8());
      /* $D889 PLA */ s_a = update_nz(pop8());
    case 0xd88a: // [$D88A..$D88F]    6 bytes
      CYCLES(0xd88a, 11);
      /* $D88A LDA */ s_a = update_nz(0x5d);
      /* $D88C LDY */ s_y = update_nz(0xd3);
      /* $D88E BCC */ s_pc = !(s_status & STATUS_C) ? 0xd893 : 0xd890;
      branchTarget = true;
      break;
    case 0xd890: // [$D890..$D892]    3 bytes
      CYCLES(0xd890, 6);
      /* $D890 JMP */ s_pc = 0xd431;
      branchTarget = true;
      break;
    case 0xd893: // [$D893..$D895]    3 bytes
      CYCLES(0xd893, 6);
      /* $D893 JMP */ s_pc = 0xd43c;
      branchTarget = true;
      break;
    case 0xd8af: // [$D8AF..$D8AF]    1 bytes
      CYCLES(0xd8af, 2);
      /* $D8AF RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xd93e: // [$D93E..$D940]    3 bytes
      CYCLES(0xd93e, 6);
      /* $D93E JSR */ push16(0xd940), s_pc = 0xda0c;
      branchTarget = true;
      break;
    case 0xd941: // [$D941..$D943]    3 bytes
      CYCLES(0xd941, 6);
      /* $D941 JSR */ push16(0xd943), s_pc = 0xd9a6;
      branchTarget = true;
      break;
    case 0xd944: // [$D944..$D949]    6 bytes
      CYCLES(0xd944, 11);
      /* $D944 LDA */ s_a = update_nz(peek_zpg(0x76));
      /* $D946 CMP */ update_nz_inv_c(s_a - peek_zpg(0x51));
      /* $D948 BCS */ s_pc = s_status & STATUS_C ? 0xd955 : 0xd94a;
      branchTarget = true;
      break;
    case 0xd94a: // [$D94A..$D951]    8 bytes
      CYCLES(0xd94a, 14);
      /* $D94A TYA */ s_a = update_nz(s_y);
      /* $D94B SEC */ s_status |= STATUS_C;
      /* $D94C ADC */ tmp = peek_zpg(0xb8), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $D94E LDX */ s_x = update_nz(peek_zpg(0xb9));
      /* $D950 BCC */ s_pc = !(s_status & STATUS_C) ? 0xd959 : 0xd952;
      branchTarget = true;
      break;
    case 0xd952: // [$D952..$D954]    3 bytes
      CYCLES(0xd952, 6);
      /* $D952 INX */ s_x = update_nz(s_x + 1);
      /* $D953 BCS */ s_pc = s_status & STATUS_C ? 0xd959 : 0xd955;
      branchTarget = true;
      break;
    case 0xd955: // [$D955..$D958]    4 bytes
      CYCLES(0xd955, 7);
      /* $D955 LDA */ s_a = update_nz(peek_zpg(0x67));
      /* $D957 LDX */ s_x = update_nz(peek_zpg(0x68));
    case 0xd959: // [$D959..$D95B]    3 bytes
      CYCLES(0xd959, 6);
      /* $D959 JSR */ push16(0xd95b), s_pc = 0xd61e;
      branchTarget = true;
      break;
    case 0xd95c: // [$D95C..$D95D]    2 bytes
      CYCLES(0xd95c, 4);
      /* $D95C BCC */ s_pc = !(s_status & STATUS_C) ? 0xd97c : 0xd95e;
      branchTarget = true;
      break;
    case 0xd95e: // [$D95E..$D96A]   13 bytes
      CYCLES(0xd95e, 23);
      /* $D95E LDA */ s_a = update_nz(peek_zpg(0x9b));
      /* $D960 SBC */ tmp = 0x01, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $D962 STA */ poke_zpg(0xb8, s_a);
      /* $D964 LDA */ s_a = update_nz(peek_zpg(0x9c));
      /* $D966 SBC */ tmp = 0x00, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $D968 STA */ poke_zpg(0xb9, s_a);
      /* $D96A RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xd97c: // [$D97C..$D980]    5 bytes
      CYCLES(0xd97c, 9);
      /* $D97C LDX */ s_x = update_nz(0x5a);
      /* $D97E JMP */ s_pc = 0xd412;
      branchTarget = true;
      break;
    case 0xd981: // [$D981..$D983]    3 bytes
      CYCLES(0xd981, 6);
      /* $D981 JMP */ s_pc = 0xdec9;
      branchTarget = true;
      break;
    case 0xd9a2: // [$D9A2..$D9A2]    1 bytes
      CYCLES(0xd9a2, 2);
      /* $D9A2 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xd9a3: // [$D9A3..$D9A5]    3 bytes
      CYCLES(0xd9a3, 6);
      /* $D9A3 LDX */ s_x = update_nz(0x3a);
      /* $D9A5 BIT */ tmp = peek(0x00a2), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $D9A8 STX */ poke_zpg(0x0d, s_x);
      /* $D9AA LDY */ s_y = update_nz(0x00);
      /* $D9AC STY */ poke_zpg(0x0e, s_y);
    case 0xd9ae: // [$D9AE..$D9B5]    8 bytes
      CYCLES(0xd9ae, 14);
      /* $D9AE LDA */ s_a = update_nz(peek_zpg(0x0e));
      /* $D9B0 LDX */ s_x = update_nz(peek_zpg(0x0d));
      /* $D9B2 STA */ poke_zpg(0x0d, s_a);
      /* $D9B4 STX */ poke_zpg(0x0e, s_x);
    case 0xd9b6: // [$D9B6..$D9B9]    4 bytes
      CYCLES(0xd9b6, 7);
      /* $D9B6 LDA */ s_a = update_nz(peek(peek16_zpg(0xb8) + s_y));
      /* $D9B8 BEQ */ s_pc = s_status & STATUS_Z ? 0xd9a2 : 0xd9ba;
      branchTarget = true;
      break;
    case 0xd9ba: // [$D9BA..$D9BD]    4 bytes
      CYCLES(0xd9ba, 7);
      /* $D9BA CMP */ update_nz_inv_c(s_a - peek_zpg(0x0e));
      /* $D9BC BEQ */ s_pc = s_status & STATUS_Z ? 0xd9a2 : 0xd9be;
      branchTarget = true;
      break;
    case 0xd9be: // [$D9BE..$D9C2]    5 bytes
      CYCLES(0xd9be, 9);
      /* $D9BE INY */ s_y = update_nz(s_y + 1);
      /* $D9BF CMP */ update_nz_inv_c(s_a - 0x22);
      /* $D9C1 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd9b6 : 0xd9c3;
      branchTarget = true;
      break;
    case 0xd9c3: // [$D9C3..$D9C4]    2 bytes
      CYCLES(0xd9c3, 4);
      /* $D9C3 BEQ */ s_pc = s_status & STATUS_Z ? 0xd9ae : 0xd9c5;
      branchTarget = true;
      break;
    case 0xd9c5: // [$D9C5..$D9C8]    4 bytes
      CYCLES(0xd9c5, 7);
      /* $D9C5 PLA */ s_a = update_nz(pop8());
      /* $D9C6 PLA */ s_a = update_nz(pop8());
      /* $D9C7 PLA */ s_a = update_nz(pop8());
      /* $D9C8 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xd9f4: // [$D9F4..$D9F7]    4 bytes
      CYCLES(0xd9f4, 7);
      /* $D9F4 CMP */ update_nz_inv_c(s_a - 0xab);
      /* $D9F6 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xd981 : 0xd9f8;
      branchTarget = true;
      break;
    case 0xd9f8: // [$D9F8..$D9FB]    4 bytes
      CYCLES(0xd9f8, 7);
      /* $D9F8 DEC */ tmp16 = 0xa1, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $D9FA BNE */ s_pc = !(s_status & STATUS_Z) ? 0xda00 : 0xd9fc;
      branchTarget = true;
      break;
    case 0xd9fc: // [$D9FC..$D9FF]    4 bytes
      CYCLES(0xd9fc, 7);
      /* $D9FC PLA */ s_a = update_nz(pop8());
      /* $D9FD JMP */ s_pc = 0xd82a;
      branchTarget = true;
      break;
    case 0xda00: // [$DA00..$DA02]    3 bytes
      CYCLES(0xda00, 6);
      /* $DA00 JSR */ push16(0xda02), s_pc = 0x00b1;
      branchTarget = true;
      break;
    case 0xda03: // [$DA03..$DA05]    3 bytes
      CYCLES(0xda03, 6);
      /* $DA03 JSR */ push16(0xda05), s_pc = 0xda0c;
      branchTarget = true;
      break;
    case 0xda06: // [$DA06..$DA09]    4 bytes
      CYCLES(0xda06, 7);
      /* $DA06 CMP */ update_nz_inv_c(s_a - 0x2c);
      /* $DA08 BEQ */ s_pc = s_status & STATUS_Z ? 0xd9f8 : 0xda0a;
      branchTarget = true;
      break;
    case 0xda0a: // [$DA0A..$DA0A]    1 bytes
      CYCLES(0xda0a, 2);
      /* $DA0A PLA */ s_a = update_nz(pop8());
    case 0xda0b: // [$DA0B..$DA0B]    1 bytes
      CYCLES(0xda0b, 2);
      /* $DA0B RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xda0c: // [$DA0C..$DA11]    6 bytes
      CYCLES(0xda0c, 11);
      /* $DA0C LDX */ s_x = update_nz(0x00);
      /* $DA0E STX */ poke_zpg(0x50, s_x);
      /* $DA10 STX */ poke_zpg(0x51, s_x);
    case 0xda12: // [$DA12..$DA13]    2 bytes
      CYCLES(0xda12, 4);
      /* $DA12 BCS */ s_pc = s_status & STATUS_C ? 0xda0b : 0xda14;
      branchTarget = true;
      break;
    case 0xda14: // [$DA14..$DA1F]   12 bytes
      CYCLES(0xda14, 21);
      /* $DA14 SBC */ tmp = 0x2f, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $DA16 STA */ poke_zpg(0x0d, s_a);
      /* $DA18 LDA */ s_a = update_nz(peek_zpg(0x51));
      /* $DA1A STA */ poke_zpg(0x5e, s_a);
      /* $DA1C CMP */ update_nz_inv_c(s_a - 0x19);
      /* $DA1E BCS */ s_pc = s_status & STATUS_C ? 0xd9f4 : 0xda20;
      branchTarget = true;
      break;
    case 0xda20: // [$DA20..$DA3D]   30 bytes
      CYCLES(0xda20, 52);
      /* $DA20 LDA */ s_a = update_nz(peek_zpg(0x50));
      /* $DA22 ASL */ s_a = update_nzc(s_a << 1);
      /* $DA23 ROL */ tmp16 = 0x5e, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $DA25 ASL */ s_a = update_nzc(s_a << 1);
      /* $DA26 ROL */ tmp16 = 0x5e, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $DA28 ADC */ tmp = peek_zpg(0x50), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $DA2A STA */ poke_zpg(0x50, s_a);
      /* $DA2C LDA */ s_a = update_nz(peek_zpg(0x5e));
      /* $DA2E ADC */ tmp = peek_zpg(0x51), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $DA30 STA */ poke_zpg(0x51, s_a);
      /* $DA32 ASL */ s_a = update_nzc(peek_zpg(0x50) << 1);
      /* $DA34 ROL */ tmp16 = 0x51, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $DA36 LDA */ s_a = update_nz(peek_zpg(0x50));
      /* $DA38 ADC */ tmp = peek_zpg(0x0d), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $DA3A STA */ poke_zpg(0x50, s_a);
      /* $DA3C BCC */ s_pc = !(s_status & STATUS_C) ? 0xda40 : 0xda3e;
      branchTarget = true;
      break;
    case 0xda3e: // [$DA3E..$DA3F]    2 bytes
      CYCLES(0xda3e, 4);
      /* $DA3E INC */ tmp16 = 0x51, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
    case 0xda40: // [$DA40..$DA42]    3 bytes
      CYCLES(0xda40, 6);
      /* $DA40 JSR */ push16(0xda42), s_pc = 0x00b1;
      branchTarget = true;
      break;
    case 0xda43: // [$DA43..$DA45]    3 bytes
      CYCLES(0xda43, 6);
      /* $DA43 JMP */ s_pc = 0xda12;
      branchTarget = true;
      break;
    case 0xda46: // [$DA46..$DA48]    3 bytes
      CYCLES(0xda46, 6);
      /* $DA46 JSR */ push16(0xda48), s_pc = 0xdfe3;
      branchTarget = true;
      break;
    case 0xda49: // [$DA49..$DA51]    9 bytes
      CYCLES(0xda49, 16);
      /* $DA49 STA */ poke_zpg(0x85, s_a);
      /* $DA4B STY */ poke_zpg(0x86, s_y);
      /* $DA4D LDA */ s_a = update_nz(0xd0);
      /* $DA4F JSR */ push16(0xda51), s_pc = 0xdec0;
      branchTarget = true;
      break;
    case 0xda52: // [$DA52..$DA5A]    9 bytes
      CYCLES(0xda52, 16);
      /* $DA52 LDA */ s_a = update_nz(peek_zpg(0x12));
      /* $DA54 PHA */ push8(s_a);
      /* $DA55 LDA */ s_a = update_nz(peek_zpg(0x11));
      /* $DA57 PHA */ push8(s_a);
      /* $DA58 JSR */ push16(0xda5a), s_pc = 0xdd7b;
      branchTarget = true;
      break;
    case 0xda5b: // [$DA5B..$DA5F]    5 bytes
      CYCLES(0xda5b, 9);
      /* $DA5B PLA */ s_a = update_nz(pop8());
      /* $DA5C ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $DA5D JSR */ push16(0xda5f), s_pc = 0xdd6d;
      branchTarget = true;
      break;
    case 0xda60: // [$DA60..$DA61]    2 bytes
      CYCLES(0xda60, 4);
      /* $DA60 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xda7a : 0xda62;
      branchTarget = true;
      break;
    case 0xda62: // [$DA62..$DA64]    3 bytes
      CYCLES(0xda62, 6);
      /* $DA62 PLA */ s_a = update_nz(pop8());
      /* $DA63 BPL */ s_pc = !(s_status & STATUS_N) ? 0xda77 : 0xda65;
      branchTarget = true;
      break;
    case 0xda65: // [$DA65..$DA67]    3 bytes
      CYCLES(0xda65, 6);
      /* $DA65 JSR */ push16(0xda67), s_pc = 0xeb72;
      branchTarget = true;
      break;
    case 0xda68: // [$DA68..$DA6A]    3 bytes
      CYCLES(0xda68, 6);
      /* $DA68 JSR */ push16(0xda6a), s_pc = 0xe10c;
      branchTarget = true;
      break;
    case 0xda6b: // [$DA6B..$DA76]   12 bytes
      CYCLES(0xda6b, 21);
      /* $DA6B LDY */ s_y = update_nz(0x00);
      /* $DA6D LDA */ s_a = update_nz(peek_zpg(0xa0));
      /* $DA6F STA */ poke(peek16_zpg(0x85) + s_y, s_a);
      /* $DA71 INY */ s_y = update_nz(s_y + 1);
      /* $DA72 LDA */ s_a = update_nz(peek_zpg(0xa1));
      /* $DA74 STA */ poke(peek16_zpg(0x85) + s_y, s_a);
      /* $DA76 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xda77: // [$DA77..$DA79]    3 bytes
      CYCLES(0xda77, 6);
      /* $DA77 JMP */ s_pc = 0xeb27;
      branchTarget = true;
      break;
    case 0xda7a: // [$DA7A..$DA82]    9 bytes
      CYCLES(0xda7a, 16);
      /* $DA7A PLA */ s_a = update_nz(pop8());
      /* $DA7B LDY */ s_y = update_nz(0x02);
      /* $DA7D LDA */ s_a = update_nz(peek(peek16_zpg(0xa0) + s_y));
      /* $DA7F CMP */ update_nz_inv_c(s_a - peek_zpg(0x70));
      /* $DA81 BCC */ s_pc = !(s_status & STATUS_C) ? 0xda9a : 0xda83;
      branchTarget = true;
      break;
    case 0xda83: // [$DA83..$DA84]    2 bytes
      CYCLES(0xda83, 4);
      /* $DA83 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xda8c : 0xda85;
      branchTarget = true;
      break;
    case 0xda85: // [$DA85..$DA8B]    7 bytes
      CYCLES(0xda85, 12);
      /* $DA85 DEY */ s_y = update_nz(s_y - 1);
      /* $DA86 LDA */ s_a = update_nz(peek(peek16_zpg(0xa0) + s_y));
      /* $DA88 CMP */ update_nz_inv_c(s_a - peek_zpg(0x6f));
      /* $DA8A BCC */ s_pc = !(s_status & STATUS_C) ? 0xda9a : 0xda8c;
      branchTarget = true;
      break;
    case 0xda8c: // [$DA8C..$DA91]    6 bytes
      CYCLES(0xda8c, 11);
      /* $DA8C LDY */ s_y = update_nz(peek_zpg(0xa1));
      /* $DA8E CPY */ update_nz_inv_c(s_y - peek_zpg(0x6a));
      /* $DA90 BCC */ s_pc = !(s_status & STATUS_C) ? 0xda9a : 0xda92;
      branchTarget = true;
      break;
    case 0xda92: // [$DA92..$DA93]    2 bytes
      CYCLES(0xda92, 4);
      /* $DA92 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xdaa1 : 0xda94;
      branchTarget = true;
      break;
    case 0xda94: // [$DA94..$DA99]    6 bytes
      CYCLES(0xda94, 11);
      /* $DA94 LDA */ s_a = update_nz(peek_zpg(0xa0));
      /* $DA96 CMP */ update_nz_inv_c(s_a - peek_zpg(0x69));
      /* $DA98 BCS */ s_pc = s_status & STATUS_C ? 0xdaa1 : 0xda9a;
      branchTarget = true;
      break;
    case 0xda9a: // [$DA9A..$DAA0]    7 bytes
      CYCLES(0xda9a, 12);
      /* $DA9A LDA */ s_a = update_nz(peek_zpg(0xa0));
      /* $DA9C LDY */ s_y = update_nz(peek_zpg(0xa1));
      /* $DA9E JMP */ s_pc = 0xdab7;
      branchTarget = true;
      break;
    case 0xdaa1: // [$DAA1..$DAA7]    7 bytes
      CYCLES(0xdaa1, 12);
      /* $DAA1 LDY */ s_y = update_nz(0x00);
      /* $DAA3 LDA */ s_a = update_nz(peek(peek16_zpg(0xa0) + s_y));
      /* $DAA5 JSR */ push16(0xdaa7), s_pc = 0xe3d5;
      branchTarget = true;
      break;
    case 0xdaa8: // [$DAA8..$DAB2]   11 bytes
      CYCLES(0xdaa8, 19);
      /* $DAA8 LDA */ s_a = update_nz(peek_zpg(0x8c));
      /* $DAAA LDY */ s_y = update_nz(peek_zpg(0x8d));
      /* $DAAC STA */ poke_zpg(0xab, s_a);
      /* $DAAE STY */ poke_zpg(0xac, s_y);
      /* $DAB0 JSR */ push16(0xdab2), s_pc = 0xe5d4;
      branchTarget = true;
      break;
    case 0xdab3: // [$DAB3..$DAB6]    4 bytes
      CYCLES(0xdab3, 7);
      /* $DAB3 LDA */ s_a = update_nz(0x9d);
      /* $DAB5 LDY */ s_y = update_nz(0x00);
    case 0xdab7: // [$DAB7..$DABD]    7 bytes
      CYCLES(0xdab7, 12);
      /* $DAB7 STA */ poke_zpg(0x8c, s_a);
      /* $DAB9 STY */ poke_zpg(0x8d, s_y);
      /* $DABB JSR */ push16(0xdabd), s_pc = 0xe635;
      branchTarget = true;
      break;
    case 0xdabe: // [$DABE..$DACE]   17 bytes
      CYCLES(0xdabe, 29);
      /* $DABE LDY */ s_y = update_nz(0x00);
      /* $DAC0 LDA */ s_a = update_nz(peek(peek16_zpg(0x8c) + s_y));
      /* $DAC2 STA */ poke(peek16_zpg(0x85) + s_y, s_a);
      /* $DAC4 INY */ s_y = update_nz(s_y + 1);
      /* $DAC5 LDA */ s_a = update_nz(peek(peek16_zpg(0x8c) + s_y));
      /* $DAC7 STA */ poke(peek16_zpg(0x85) + s_y, s_a);
      /* $DAC9 INY */ s_y = update_nz(s_y + 1);
      /* $DACA LDA */ s_a = update_nz(peek(peek16_zpg(0x8c) + s_y));
      /* $DACC STA */ poke(peek16_zpg(0x85) + s_y, s_a);
      /* $DACE RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xdafb: // [$DAFB..$DAFF]    5 bytes
      CYCLES(0xdafb, 9);
      /* $DAFB LDA */ s_a = update_nz(0x0d);
      /* $DAFD JSR */ push16(0xdaff), s_pc = 0xdb5c;
      branchTarget = true;
      break;
    case 0xdb00: // [$DB00..$DB01]    2 bytes
      CYCLES(0xdb00, 4);
      /* $DB00 EOR */ s_a = update_nz(s_a ^ 0xff);
    case 0xdb02: // [$DB02..$DB02]    1 bytes
      CYCLES(0xdb02, 2);
      /* $DB02 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xdb3a: // [$DB3A..$DB3C]    3 bytes
      CYCLES(0xdb3a, 6);
      /* $DB3A JSR */ push16(0xdb3c), s_pc = 0xe3e7;
      branchTarget = true;
      break;
    case 0xdb3d: // [$DB3D..$DB3F]    3 bytes
      CYCLES(0xdb3d, 6);
      /* $DB3D JSR */ push16(0xdb3f), s_pc = 0xe600;
      branchTarget = true;
      break;
    case 0xdb40: // [$DB40..$DB43]    4 bytes
      CYCLES(0xdb40, 7);
      /* $DB40 TAX */ s_x = update_nz(s_a);
      /* $DB41 LDY */ s_y = update_nz(0x00);
      /* $DB43 INX */ s_x = update_nz(s_x + 1);
    case 0xdb44: // [$DB44..$DB46]    3 bytes
      CYCLES(0xdb44, 6);
      /* $DB44 DEX */ s_x = update_nz(s_x - 1);
      /* $DB45 BEQ */ s_pc = s_status & STATUS_Z ? 0xdb02 : 0xdb47;
      branchTarget = true;
      break;
    case 0xdb47: // [$DB47..$DB4B]    5 bytes
      CYCLES(0xdb47, 9);
      /* $DB47 LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $DB49 JSR */ push16(0xdb4b), s_pc = 0xdb5c;
      branchTarget = true;
      break;
    case 0xdb4c: // [$DB4C..$DB50]    5 bytes
      CYCLES(0xdb4c, 9);
      /* $DB4C INY */ s_y = update_nz(s_y + 1);
      /* $DB4D CMP */ update_nz_inv_c(s_a - 0x0d);
      /* $DB4F BNE */ s_pc = !(s_status & STATUS_Z) ? 0xdb44 : 0xdb51;
      branchTarget = true;
      break;
    case 0xdb51: // [$DB51..$DB53]    3 bytes
      CYCLES(0xdb51, 6);
      /* $DB51 JSR */ push16(0xdb53), s_pc = 0xdb00;
      branchTarget = true;
      break;
    case 0xdb54: // [$DB54..$DB56]    3 bytes
      CYCLES(0xdb54, 6);
      /* $DB54 JMP */ s_pc = 0xdb44;
      branchTarget = true;
      break;
    case 0xdb57: // [$DB57..$DB59]    3 bytes
      CYCLES(0xdb57, 6);
      /* $DB57 LDA */ s_a = update_nz(0x20);
      /* $DB59 BIT */ tmp = peek(0x3fa9), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
    case 0xdb5c: // [$DB5C..$DB61]    6 bytes
      CYCLES(0xdb5c, 11);
      /* $DB5C ORA */ s_a = update_nz(s_a | 0x80);
      /* $DB5E CMP */ update_nz_inv_c(s_a - 0xa0);
      /* $DB60 BCC */ s_pc = !(s_status & STATUS_C) ? 0xdb64 : 0xdb62;
      branchTarget = true;
      break;
    case 0xdb62: // [$DB62..$DB63]    2 bytes
      CYCLES(0xdb62, 4);
      /* $DB62 ORA */ s_a = update_nz(s_a | peek_zpg(0xf3));
    case 0xdb64: // [$DB64..$DB66]    3 bytes
      CYCLES(0xdb64, 6);
      /* $DB64 JSR */ push16(0xdb66), s_pc = 0xfded;
      branchTarget = true;
      break;
    case 0xdb67: // [$DB67..$DB6E]    8 bytes
      CYCLES(0xdb67, 14);
      /* $DB67 AND */ s_a = update_nz(s_a & 0x7f);
      /* $DB69 PHA */ push8(s_a);
      /* $DB6A LDA */ s_a = update_nz(peek_zpg(0xf1));
      /* $DB6C JSR */ push16(0xdb6e), s_pc = 0xfca8;
      branchTarget = true;
      break;
    case 0xdb6f: // [$DB6F..$DB70]    2 bytes
      CYCLES(0xdb6f, 4);
      /* $DB6F PLA */ s_a = update_nz(pop8());
      /* $DB70 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xdd67: // [$DD67..$DD69]    3 bytes
      CYCLES(0xdd67, 6);
      /* $DD67 JSR */ push16(0xdd69), s_pc = 0xdd7b;
      branchTarget = true;
      break;
    case 0xdd6a: // [$DD6A..$DD6B]    2 bytes
      CYCLES(0xdd6a, 4);
      /* $DD6A CLC */ s_status &= ~STATUS_C;
      /* $DD6B BIT */ tmp = peek_zpg(0x38), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
    case 0xdd6d: // [$DD6D..$DD70]    4 bytes
      CYCLES(0xdd6d, 7);
      /* $DD6D BIT */ tmp = peek_zpg(0x11), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $DD6F BMI */ s_pc = s_status & STATUS_N ? 0xdd74 : 0xdd71;
      branchTarget = true;
      break;
    case 0xdd71: // [$DD71..$DD72]    2 bytes
      CYCLES(0xdd71, 4);
      /* $DD71 BCS */ s_pc = s_status & STATUS_C ? 0xdd76 : 0xdd73;
      branchTarget = true;
      break;
    case 0xdd73: // [$DD73..$DD73]    1 bytes
      CYCLES(0xdd73, 2);
      /* $DD73 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xdd74: // [$DD74..$DD75]    2 bytes
      CYCLES(0xdd74, 4);
      /* $DD74 BCS */ s_pc = s_status & STATUS_C ? 0xdd73 : 0xdd76;
      branchTarget = true;
      break;
    case 0xdd76: // [$DD76..$DD7A]    5 bytes
      CYCLES(0xdd76, 9);
      /* $DD76 LDX */ s_x = update_nz(0xa3);
      /* $DD78 JMP */ s_pc = 0xd412;
      branchTarget = true;
      break;
    case 0xdd7b: // [$DD7B..$DD7E]    4 bytes
      CYCLES(0xdd7b, 7);
      /* $DD7B LDX */ s_x = update_nz(peek_zpg(0xb8));
      /* $DD7D BNE */ s_pc = !(s_status & STATUS_Z) ? 0xdd81 : 0xdd7f;
      branchTarget = true;
      break;
    case 0xdd7f: // [$DD7F..$DD80]    2 bytes
      CYCLES(0xdd7f, 4);
      /* $DD7F DEC */ tmp16 = 0xb9, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
    case 0xdd81: // [$DD81..$DD85]    5 bytes
      CYCLES(0xdd81, 9);
      /* $DD81 DEC */ tmp16 = 0xb8, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $DD83 LDX */ s_x = update_nz(0x00);
      /* $DD85 BIT */ tmp = peek_zpg(0x48), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $DD87 TXA */ s_a = update_nz(s_x);
      /* $DD88 PHA */ push8(s_a);
      /* $DD89 LDA */ s_a = update_nz(0x01);
      /* $DD8B JSR */ push16(0xdd8d), s_pc = 0xd3d6;
      branchTarget = true;
      break;
    case 0xdd8e: // [$DD8E..$DD90]    3 bytes
      CYCLES(0xdd8e, 6);
      /* $DD8E JSR */ push16(0xdd90), s_pc = 0xde60;
      branchTarget = true;
      break;
    case 0xdd91: // [$DD91..$DD94]    4 bytes
      CYCLES(0xdd91, 7);
      /* $DD91 LDA */ s_a = update_nz(0x00);
      /* $DD93 STA */ poke_zpg(0x89, s_a);
    case 0xdd95: // [$DD95..$DD97]    3 bytes
      CYCLES(0xdd95, 6);
      /* $DD95 JSR */ push16(0xdd97), s_pc = 0x00b7;
      branchTarget = true;
      break;
    case 0xdd98: // [$DD98..$DD9C]    5 bytes
      CYCLES(0xdd98, 9);
      /* $DD98 SEC */ s_status |= STATUS_C;
      /* $DD99 SBC */ tmp = 0xcf, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $DD9B BCC */ s_pc = !(s_status & STATUS_C) ? 0xddb4 : 0xdd9d;
      branchTarget = true;
      break;
    case 0xdd9d: // [$DD9D..$DDA0]    4 bytes
      CYCLES(0xdd9d, 7);
      /* $DD9D CMP */ update_nz_inv_c(s_a - 0x03);
      /* $DD9F BCS */ s_pc = s_status & STATUS_C ? 0xddb4 : 0xdda1;
      branchTarget = true;
      break;
    case 0xdda1: // [$DDA1..$DDAB]   11 bytes
      CYCLES(0xdda1, 19);
      /* $DDA1 CMP */ update_nz_inv_c(s_a - 0x01);
      /* $DDA3 ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $DDA4 EOR */ s_a = update_nz(s_a ^ 0x01);
      /* $DDA6 EOR */ s_a = update_nz(s_a ^ peek_zpg(0x89));
      /* $DDA8 CMP */ update_nz_inv_c(s_a - peek_zpg(0x89));
      /* $DDAA BCC */ s_pc = !(s_status & STATUS_C) ? 0xde0d : 0xddac;
      branchTarget = true;
      break;
    case 0xddac: // [$DDAC..$DDB0]    5 bytes
      CYCLES(0xddac, 9);
      /* $DDAC STA */ poke_zpg(0x89, s_a);
      /* $DDAE JSR */ push16(0xddb0), s_pc = 0x00b1;
      branchTarget = true;
      break;
    case 0xddb1: // [$DDB1..$DDB3]    3 bytes
      CYCLES(0xddb1, 6);
      /* $DDB1 JMP */ s_pc = 0xdd98;
      branchTarget = true;
      break;
    case 0xddb4: // [$DDB4..$DDB7]    4 bytes
      CYCLES(0xddb4, 7);
      /* $DDB4 LDX */ s_x = update_nz(peek_zpg(0x89));
      /* $DDB6 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xdde4 : 0xddb8;
      branchTarget = true;
      break;
    case 0xddb8: // [$DDB8..$DDB9]    2 bytes
      CYCLES(0xddb8, 4);
      /* $DDB8 BCS */ s_pc = s_status & STATUS_C ? 0xde35 : 0xddba;
      branchTarget = true;
      break;
    case 0xddba: // [$DDBA..$DDBD]    4 bytes
      CYCLES(0xddba, 7);
      /* $DDBA ADC */ tmp = 0x07, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $DDBC BCC */ s_pc = !(s_status & STATUS_C) ? 0xde35 : 0xddbe;
      branchTarget = true;
      break;
    case 0xddbe: // [$DDBE..$DDC1]    4 bytes
      CYCLES(0xddbe, 7);
      /* $DDBE ADC */ tmp = peek_zpg(0x11), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $DDC0 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xddc5 : 0xddc2;
      branchTarget = true;
      break;
    case 0xddc2: // [$DDC2..$DDC4]    3 bytes
      CYCLES(0xddc2, 6);
      /* $DDC2 JMP */ s_pc = 0xe597;
      branchTarget = true;
      break;
    case 0xddc5: // [$DDC5..$DDCC]    8 bytes
      CYCLES(0xddc5, 14);
      /* $DDC5 ADC */ tmp = 0xff, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $DDC7 STA */ poke_zpg(0x5e, s_a);
      /* $DDC9 ASL */ s_a = update_nzc(s_a << 1);
      /* $DDCA ADC */ tmp = peek_zpg(0x5e), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $DDCC TAY */ s_y = update_nz(s_a);
    case 0xddcd: // [$DDCD..$DDD2]    6 bytes
      CYCLES(0xddcd, 11);
      /* $DDCD PLA */ s_a = update_nz(pop8());
      /* $DDCE CMP */ update_nz_inv_c(s_a - peek(0xd0b2 + s_y));
      /* $DDD1 BCS */ s_pc = s_status & STATUS_C ? 0xde3a : 0xddd3;
      branchTarget = true;
      break;
    case 0xddd3: // [$DDD3..$DDD5]    3 bytes
      CYCLES(0xddd3, 6);
      /* $DDD3 JSR */ push16(0xddd5), s_pc = 0xdd6a;
      branchTarget = true;
      break;
    case 0xddd6: // [$DDD6..$DDD6]    1 bytes
      CYCLES(0xddd6, 2);
      /* $DDD6 PHA */ push8(s_a);
    case 0xddd7: // [$DDD7..$DDD9]    3 bytes
      CYCLES(0xddd7, 6);
      /* $DDD7 JSR */ push16(0xddd9), s_pc = 0xddfd;
      branchTarget = true;
      break;
    case 0xddda: // [$DDDA..$DDDE]    5 bytes
      CYCLES(0xddda, 9);
      /* $DDDA PLA */ s_a = update_nz(pop8());
      /* $DDDB LDY */ s_y = update_nz(peek_zpg(0x87));
      /* $DDDD BPL */ s_pc = !(s_status & STATUS_N) ? 0xddf6 : 0xdddf;
      branchTarget = true;
      break;
    case 0xdddf: // [$DDDF..$DDE1]    3 bytes
      CYCLES(0xdddf, 6);
      /* $DDDF TAX */ s_x = update_nz(s_a);
      /* $DDE0 BEQ */ s_pc = s_status & STATUS_Z ? 0xde38 : 0xdde2;
      branchTarget = true;
      break;
    case 0xdde2: // [$DDE2..$DDE3]    2 bytes
      CYCLES(0xdde2, 4);
      /* $DDE2 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xde43 : 0xdde4;
      branchTarget = true;
      break;
    case 0xdde4: // [$DDE4..$DDEB]    8 bytes
      CYCLES(0xdde4, 14);
      /* $DDE4 LSR */ tmp16 = 0x11, tmp = peek_zpg(tmp16), set_c_to_bit0(tmp), poke_zpg(tmp16, update_nz(tmp >> 1));
      /* $DDE6 TXA */ s_a = update_nz(s_x);
      /* $DDE7 ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $DDE8 LDX */ s_x = update_nz(peek_zpg(0xb8));
      /* $DDEA BNE */ s_pc = !(s_status & STATUS_Z) ? 0xddee : 0xddec;
      branchTarget = true;
      break;
    case 0xddec: // [$DDEC..$DDED]    2 bytes
      CYCLES(0xddec, 4);
      /* $DDEC DEC */ tmp16 = 0xb9, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
    case 0xddee: // [$DDEE..$DDF5]    8 bytes
      CYCLES(0xddee, 14);
      /* $DDEE DEC */ tmp16 = 0xb8, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $DDF0 LDY */ s_y = update_nz(0x1b);
      /* $DDF2 STA */ poke_zpg(0x89, s_a);
      /* $DDF4 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xddcd : 0xddf6;
      branchTarget = true;
      break;
    case 0xddf6: // [$DDF6..$DDFA]    5 bytes
      CYCLES(0xddf6, 9);
      /* $DDF6 CMP */ update_nz_inv_c(s_a - peek(0xd0b2 + s_y));
      /* $DDF9 BCS */ s_pc = s_status & STATUS_C ? 0xde43 : 0xddfb;
      branchTarget = true;
      break;
    case 0xddfb: // [$DDFB..$DDFC]    2 bytes
      CYCLES(0xddfb, 4);
      /* $DDFB BCC */ s_pc = !(s_status & STATUS_C) ? 0xddd6 : 0xddfd;
      branchTarget = true;
      break;
    case 0xddfd: // [$DDFD..$DE07]   11 bytes
      CYCLES(0xddfd, 19);
      /* $DDFD LDA */ s_a = update_nz(peek(0xd0b4 + s_y));
      /* $DE00 PHA */ push8(s_a);
      /* $DE01 LDA */ s_a = update_nz(peek(0xd0b3 + s_y));
      /* $DE04 PHA */ push8(s_a);
      /* $DE05 JSR */ push16(0xde07), s_pc = 0xde10;
      branchTarget = true;
      break;
    case 0xde08: // [$DE08..$DE0C]    5 bytes
      CYCLES(0xde08, 9);
      /* $DE08 LDA */ s_a = update_nz(peek_zpg(0x89));
      /* $DE0A JMP */ s_pc = 0xdd86;
      branchTarget = true;
      break;
    case 0xde0d: // [$DE0D..$DE0F]    3 bytes
      CYCLES(0xde0d, 6);
      /* $DE0D JMP */ s_pc = 0xdec9;
      branchTarget = true;
      break;
    case 0xde10: // [$DE10..$DE1F]   16 bytes
      CYCLES(0xde10, 28);
      /* $DE10 LDA */ s_a = update_nz(peek_zpg(0xa2));
      /* $DE12 LDX */ s_x = update_nz(peek(0xd0b2 + s_y));
      /* $DE15 TAY */ s_y = update_nz(s_a);
      /* $DE16 PLA */ s_a = update_nz(pop8());
      /* $DE17 STA */ poke_zpg(0x5e, s_a);
      /* $DE19 INC */ tmp16 = 0x5e, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $DE1B PLA */ s_a = update_nz(pop8());
      /* $DE1C STA */ poke_zpg(0x5f, s_a);
      /* $DE1E TYA */ s_a = update_nz(s_y);
      /* $DE1F PHA */ push8(s_a);
    case 0xde20: // [$DE20..$DE22]    3 bytes
      CYCLES(0xde20, 6);
      /* $DE20 JSR */ push16(0xde22), s_pc = 0xeb72;
      branchTarget = true;
      break;
    case 0xde23: // [$DE23..$DE34]   18 bytes
      CYCLES(0xde23, 31);
      /* $DE23 LDA */ s_a = update_nz(peek_zpg(0xa1));
      /* $DE25 PHA */ push8(s_a);
      /* $DE26 LDA */ s_a = update_nz(peek_zpg(0xa0));
      /* $DE28 PHA */ push8(s_a);
      /* $DE29 LDA */ s_a = update_nz(peek_zpg(0x9f));
      /* $DE2B PHA */ push8(s_a);
      /* $DE2C LDA */ s_a = update_nz(peek_zpg(0x9e));
      /* $DE2E PHA */ push8(s_a);
      /* $DE2F LDA */ s_a = update_nz(peek_zpg(0x9d));
      /* $DE31 PHA */ push8(s_a);
      /* $DE32 JMP */ s_pc = peek16(0x005e);
      branchTarget = true;
      break;
    case 0xde35: // [$DE35..$DE37]    3 bytes
      CYCLES(0xde35, 6);
      /* $DE35 LDY */ s_y = update_nz(0xff);
      /* $DE37 PLA */ s_a = update_nz(pop8());
    case 0xde38: // [$DE38..$DE39]    2 bytes
      CYCLES(0xde38, 4);
      /* $DE38 BEQ */ s_pc = s_status & STATUS_Z ? 0xde5d : 0xde3a;
      branchTarget = true;
      break;
    case 0xde3a: // [$DE3A..$DE3D]    4 bytes
      CYCLES(0xde3a, 7);
      /* $DE3A CMP */ update_nz_inv_c(s_a - 0x64);
      /* $DE3C BEQ */ s_pc = s_status & STATUS_Z ? 0xde41 : 0xde3e;
      branchTarget = true;
      break;
    case 0xde3e: // [$DE3E..$DE40]    3 bytes
      CYCLES(0xde3e, 6);
      /* $DE3E JSR */ push16(0xde40), s_pc = 0xdd6a;
      branchTarget = true;
      break;
    case 0xde41: // [$DE41..$DE42]    2 bytes
      CYCLES(0xde41, 4);
      /* $DE41 STY */ poke_zpg(0x87, s_y);
    case 0xde43: // [$DE43..$DE5C]   26 bytes
      CYCLES(0xde43, 45);
      /* $DE43 PLA */ s_a = update_nz(pop8());
      /* $DE44 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $DE45 STA */ poke_zpg(0x16, s_a);
      /* $DE47 PLA */ s_a = update_nz(pop8());
      /* $DE48 STA */ poke_zpg(0xa5, s_a);
      /* $DE4A PLA */ s_a = update_nz(pop8());
      /* $DE4B STA */ poke_zpg(0xa6, s_a);
      /* $DE4D PLA */ s_a = update_nz(pop8());
      /* $DE4E STA */ poke_zpg(0xa7, s_a);
      /* $DE50 PLA */ s_a = update_nz(pop8());
      /* $DE51 STA */ poke_zpg(0xa8, s_a);
      /* $DE53 PLA */ s_a = update_nz(pop8());
      /* $DE54 STA */ poke_zpg(0xa9, s_a);
      /* $DE56 PLA */ s_a = update_nz(pop8());
      /* $DE57 STA */ poke_zpg(0xaa, s_a);
      /* $DE59 EOR */ s_a = update_nz(s_a ^ peek_zpg(0xa2));
      /* $DE5B STA */ poke_zpg(0xab, s_a);
    case 0xde5d: // [$DE5D..$DE5F]    3 bytes
      CYCLES(0xde5d, 6);
      /* $DE5D LDA */ s_a = update_nz(peek_zpg(0x9d));
      /* $DE5F RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xde60: // [$DE60..$DE63]    4 bytes
      CYCLES(0xde60, 7);
      /* $DE60 LDA */ s_a = update_nz(0x00);
      /* $DE62 STA */ poke_zpg(0x11, s_a);
    case 0xde64: // [$DE64..$DE66]    3 bytes
      CYCLES(0xde64, 6);
      /* $DE64 JSR */ push16(0xde66), s_pc = 0x00b1;
      branchTarget = true;
      break;
    case 0xde67: // [$DE67..$DE68]    2 bytes
      CYCLES(0xde67, 4);
      /* $DE67 BCS */ s_pc = s_status & STATUS_C ? 0xde6c : 0xde69;
      branchTarget = true;
      break;
    case 0xde69: // [$DE69..$DE6B]    3 bytes
      CYCLES(0xde69, 6);
      /* $DE69 JMP */ s_pc = 0xec4a;
      branchTarget = true;
      break;
    case 0xde6c: // [$DE6C..$DE6E]    3 bytes
      CYCLES(0xde6c, 6);
      /* $DE6C JSR */ push16(0xde6e), s_pc = 0xe07d;
      branchTarget = true;
      break;
    case 0xde6f: // [$DE6F..$DE70]    2 bytes
      CYCLES(0xde6f, 4);
      /* $DE6F BCS */ s_pc = s_status & STATUS_C ? 0xded5 : 0xde71;
      branchTarget = true;
      break;
    case 0xde71: // [$DE71..$DE74]    4 bytes
      CYCLES(0xde71, 7);
      /* $DE71 CMP */ update_nz_inv_c(s_a - 0x2e);
      /* $DE73 BEQ */ s_pc = s_status & STATUS_Z ? 0xde69 : 0xde75;
      branchTarget = true;
      break;
    case 0xde75: // [$DE75..$DE78]    4 bytes
      CYCLES(0xde75, 7);
      /* $DE75 CMP */ update_nz_inv_c(s_a - 0xc9);
      /* $DE77 BEQ */ s_pc = s_status & STATUS_Z ? 0xdece : 0xde79;
      branchTarget = true;
      break;
    case 0xde79: // [$DE79..$DE7C]    4 bytes
      CYCLES(0xde79, 7);
      /* $DE79 CMP */ update_nz_inv_c(s_a - 0xc8);
      /* $DE7B BEQ */ s_pc = s_status & STATUS_Z ? 0xde64 : 0xde7d;
      branchTarget = true;
      break;
    case 0xde7d: // [$DE7D..$DE80]    4 bytes
      CYCLES(0xde7d, 7);
      /* $DE7D CMP */ update_nz_inv_c(s_a - 0x22);
      /* $DE7F BNE */ s_pc = !(s_status & STATUS_Z) ? 0xde90 : 0xde81;
      branchTarget = true;
      break;
    case 0xde81: // [$DE81..$DE88]    8 bytes
      CYCLES(0xde81, 14);
      /* $DE81 LDA */ s_a = update_nz(peek_zpg(0xb8));
      /* $DE83 LDY */ s_y = update_nz(peek_zpg(0xb9));
      /* $DE85 ADC */ tmp = 0x00, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $DE87 BCC */ s_pc = !(s_status & STATUS_C) ? 0xde8a : 0xde89;
      branchTarget = true;
      break;
    case 0xde89: // [$DE89..$DE89]    1 bytes
      CYCLES(0xde89, 2);
      /* $DE89 INY */ s_y = update_nz(s_y + 1);
    case 0xde8a: // [$DE8A..$DE8C]    3 bytes
      CYCLES(0xde8a, 6);
      /* $DE8A JSR */ push16(0xde8c), s_pc = 0xe3e7;
      branchTarget = true;
      break;
    case 0xde8d: // [$DE8D..$DE8F]    3 bytes
      CYCLES(0xde8d, 6);
      /* $DE8D JMP */ s_pc = 0xe73d;
      branchTarget = true;
      break;
    case 0xde90: // [$DE90..$DE93]    4 bytes
      CYCLES(0xde90, 7);
      /* $DE90 CMP */ update_nz_inv_c(s_a - 0xc6);
      /* $DE92 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xdea4 : 0xde94;
      branchTarget = true;
      break;
    case 0xde94: // [$DE94..$DE97]    4 bytes
      CYCLES(0xde94, 7);
      /* $DE94 LDY */ s_y = update_nz(0x18);
      /* $DE96 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xded0 : 0xde98;
      branchTarget = true;
      break;
    case 0xde98: // [$DE98..$DE9B]    4 bytes
      CYCLES(0xde98, 7);
      /* $DE98 LDA */ s_a = update_nz(peek_zpg(0x9d));
      /* $DE9A BNE */ s_pc = !(s_status & STATUS_Z) ? 0xde9f : 0xde9c;
      branchTarget = true;
      break;
    case 0xde9c: // [$DE9C..$DE9E]    3 bytes
      CYCLES(0xde9c, 6);
      /* $DE9C LDY */ s_y = update_nz(0x01);
      /* $DE9E BIT */ tmp = peek(0x00a0), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $DEA1 JMP */ s_pc = 0xe301;
      branchTarget = true;
      break;
    case 0xdea4: // [$DEA4..$DEA7]    4 bytes
      CYCLES(0xdea4, 7);
      /* $DEA4 CMP */ update_nz_inv_c(s_a - 0xc2);
      /* $DEA6 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xdeab : 0xdea8;
      branchTarget = true;
      break;
    case 0xdea8: // [$DEA8..$DEAA]    3 bytes
      CYCLES(0xdea8, 6);
      /* $DEA8 JMP */ s_pc = 0xe354;
      branchTarget = true;
      break;
    case 0xdeab: // [$DEAB..$DEAE]    4 bytes
      CYCLES(0xdeab, 7);
      /* $DEAB CMP */ update_nz_inv_c(s_a - 0xd2);
      /* $DEAD BCC */ s_pc = !(s_status & STATUS_C) ? 0xdeb2 : 0xdeaf;
      branchTarget = true;
      break;
    case 0xdeaf: // [$DEAF..$DEB1]    3 bytes
      CYCLES(0xdeaf, 6);
      /* $DEAF JMP */ s_pc = 0xdf0c;
      branchTarget = true;
      break;
    case 0xdeb2: // [$DEB2..$DEB4]    3 bytes
      CYCLES(0xdeb2, 6);
      /* $DEB2 JSR */ push16(0xdeb4), s_pc = 0xdebb;
      branchTarget = true;
      break;
    case 0xdeb5: // [$DEB5..$DEB7]    3 bytes
      CYCLES(0xdeb5, 6);
      /* $DEB5 JSR */ push16(0xdeb7), s_pc = 0xdd7b;
      branchTarget = true;
      break;
    case 0xdeb8: // [$DEB8..$DEBA]    3 bytes
      CYCLES(0xdeb8, 6);
      /* $DEB8 LDA */ s_a = update_nz(0x29);
      /* $DEBA BIT */ tmp = peek(0x28a9), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $DEBD BIT */ tmp = peek(0x2ca9), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
    case 0xdec0: // [$DEC0..$DEC5]    6 bytes
      CYCLES(0xdec0, 11);
      /* $DEC0 LDY */ s_y = update_nz(0x00);
      /* $DEC2 CMP */ update_nz_inv_c(s_a - peek(peek16_zpg(0xb8) + s_y));
      /* $DEC4 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xdec9 : 0xdec6;
      branchTarget = true;
      break;
    case 0xdec6: // [$DEC6..$DEC8]    3 bytes
      CYCLES(0xdec6, 6);
      /* $DEC6 JMP */ s_pc = 0x00b1;
      branchTarget = true;
      break;
    case 0xdec9: // [$DEC9..$DECD]    5 bytes
      CYCLES(0xdec9, 9);
      /* $DEC9 LDX */ s_x = update_nz(0x10);
      /* $DECB JMP */ s_pc = 0xd412;
      branchTarget = true;
      break;
    case 0xdece: // [$DECE..$DECF]    2 bytes
      CYCLES(0xdece, 4);
      /* $DECE LDY */ s_y = update_nz(0x15);
    case 0xded0: // [$DED0..$DED4]    5 bytes
      CYCLES(0xded0, 9);
      /* $DED0 PLA */ s_a = update_nz(pop8());
      /* $DED1 PLA */ s_a = update_nz(pop8());
      /* $DED2 JMP */ s_pc = 0xddd7;
      branchTarget = true;
      break;
    case 0xded5: // [$DED5..$DED7]    3 bytes
      CYCLES(0xded5, 6);
      /* $DED5 JSR */ push16(0xded7), s_pc = 0xdfe3;
      branchTarget = true;
      break;
    case 0xded8: // [$DED8..$DEDF]    8 bytes
      CYCLES(0xded8, 14);
      /* $DED8 STA */ poke_zpg(0xa0, s_a);
      /* $DEDA STY */ poke_zpg(0xa1, s_y);
      /* $DEDC LDX */ s_x = update_nz(peek_zpg(0x11));
      /* $DEDE BEQ */ s_pc = s_status & STATUS_Z ? 0xdee5 : 0xdee0;
      branchTarget = true;
      break;
    case 0xdee0: // [$DEE0..$DEE4]    5 bytes
      CYCLES(0xdee0, 9);
      /* $DEE0 LDX */ s_x = update_nz(0x00);
      /* $DEE2 STX */ poke_zpg(0xac, s_x);
      /* $DEE4 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xdee5: // [$DEE5..$DEE8]    4 bytes
      CYCLES(0xdee5, 7);
      /* $DEE5 LDX */ s_x = update_nz(peek_zpg(0x12));
      /* $DEE7 BPL */ s_pc = !(s_status & STATUS_N) ? 0xdef6 : 0xdee9;
      branchTarget = true;
      break;
    case 0xdee9: // [$DEE9..$DEF5]   13 bytes
      CYCLES(0xdee9, 23);
      /* $DEE9 LDY */ s_y = update_nz(0x00);
      /* $DEEB LDA */ s_a = update_nz(peek(peek16_zpg(0xa0) + s_y));
      /* $DEED TAX */ s_x = update_nz(s_a);
      /* $DEEE INY */ s_y = update_nz(s_y + 1);
      /* $DEEF LDA */ s_a = update_nz(peek(peek16_zpg(0xa0) + s_y));
      /* $DEF1 TAY */ s_y = update_nz(s_a);
      /* $DEF2 TXA */ s_a = update_nz(s_x);
      /* $DEF3 JMP */ s_pc = 0xe2f2;
      branchTarget = true;
      break;
    case 0xdef6: // [$DEF6..$DEF8]    3 bytes
      CYCLES(0xdef6, 6);
      /* $DEF6 JMP */ s_pc = 0xeaf9;
      branchTarget = true;
      break;
    case 0xdef9: // [$DEF9..$DEFB]    3 bytes
      CYCLES(0xdef9, 6);
      /* $DEF9 JSR */ push16(0xdefb), s_pc = 0x00b1;
      branchTarget = true;
      break;
    case 0xdefc: // [$DEFC..$DEFE]    3 bytes
      CYCLES(0xdefc, 6);
      /* $DEFC JSR */ push16(0xdefe), s_pc = 0xf1ec;
      branchTarget = true;
      break;
    case 0xdeff: // [$DEFF..$DF04]    6 bytes
      CYCLES(0xdeff, 11);
      /* $DEFF TXA */ s_a = update_nz(s_x);
      /* $DF00 LDY */ s_y = update_nz(peek_zpg(0xf0));
      /* $DF02 JSR */ push16(0xdf04), s_pc = 0xf871;
      branchTarget = true;
      break;
    case 0xdf05: // [$DF05..$DF08]    4 bytes
      CYCLES(0xdf05, 7);
      /* $DF05 TAY */ s_y = update_nz(s_a);
      /* $DF06 JSR */ push16(0xdf08), s_pc = 0xe301;
      branchTarget = true;
      break;
    case 0xdf09: // [$DF09..$DF0B]    3 bytes
      CYCLES(0xdf09, 6);
      /* $DF09 JMP */ s_pc = 0xdeb8;
      branchTarget = true;
      break;
    case 0xdf0c: // [$DF0C..$DF0F]    4 bytes
      CYCLES(0xdf0c, 7);
      /* $DF0C CMP */ update_nz_inv_c(s_a - 0xd7);
      /* $DF0E BEQ */ s_pc = s_status & STATUS_Z ? 0xdef9 : 0xdf10;
      branchTarget = true;
      break;
    case 0xdf10: // [$DF10..$DF15]    6 bytes
      CYCLES(0xdf10, 11);
      /* $DF10 ASL */ s_a = update_nzc(s_a << 1);
      /* $DF11 PHA */ push8(s_a);
      /* $DF12 TAX */ s_x = update_nz(s_a);
      /* $DF13 JSR */ push16(0xdf15), s_pc = 0x00b1;
      branchTarget = true;
      break;
    case 0xdf16: // [$DF16..$DF19]    4 bytes
      CYCLES(0xdf16, 7);
      /* $DF16 CPX */ update_nz_inv_c(s_x - 0xcf);
      /* $DF18 BCC */ s_pc = !(s_status & STATUS_C) ? 0xdf3a : 0xdf1a;
      branchTarget = true;
      break;
    case 0xdf1a: // [$DF1A..$DF1C]    3 bytes
      CYCLES(0xdf1a, 6);
      /* $DF1A JSR */ push16(0xdf1c), s_pc = 0xdebb;
      branchTarget = true;
      break;
    case 0xdf1d: // [$DF1D..$DF1F]    3 bytes
      CYCLES(0xdf1d, 6);
      /* $DF1D JSR */ push16(0xdf1f), s_pc = 0xdd7b;
      branchTarget = true;
      break;
    case 0xdf20: // [$DF20..$DF22]    3 bytes
      CYCLES(0xdf20, 6);
      /* $DF20 JSR */ push16(0xdf22), s_pc = 0xdebe;
      branchTarget = true;
      break;
    case 0xdf23: // [$DF23..$DF25]    3 bytes
      CYCLES(0xdf23, 6);
      /* $DF23 JSR */ push16(0xdf25), s_pc = 0xdd6c;
      branchTarget = true;
      break;
    case 0xdf26: // [$DF26..$DF32]   13 bytes
      CYCLES(0xdf26, 23);
      /* $DF26 PLA */ s_a = update_nz(pop8());
      /* $DF27 TAX */ s_x = update_nz(s_a);
      /* $DF28 LDA */ s_a = update_nz(peek_zpg(0xa1));
      /* $DF2A PHA */ push8(s_a);
      /* $DF2B LDA */ s_a = update_nz(peek_zpg(0xa0));
      /* $DF2D PHA */ push8(s_a);
      /* $DF2E TXA */ s_a = update_nz(s_x);
      /* $DF2F PHA */ push8(s_a);
      /* $DF30 JSR */ push16(0xdf32), s_pc = 0xe6f8;
      branchTarget = true;
      break;
    case 0xdf33: // [$DF33..$DF39]    7 bytes
      CYCLES(0xdf33, 12);
      /* $DF33 PLA */ s_a = update_nz(pop8());
      /* $DF34 TAY */ s_y = update_nz(s_a);
      /* $DF35 TXA */ s_a = update_nz(s_x);
      /* $DF36 PHA */ push8(s_a);
      /* $DF37 JMP */ s_pc = 0xdf3f;
      branchTarget = true;
      break;
    case 0xdf3a: // [$DF3A..$DF3C]    3 bytes
      CYCLES(0xdf3a, 6);
      /* $DF3A JSR */ push16(0xdf3c), s_pc = 0xdeb2;
      branchTarget = true;
      break;
    case 0xdf3d: // [$DF3D..$DF3E]    2 bytes
      CYCLES(0xdf3d, 4);
      /* $DF3D PLA */ s_a = update_nz(pop8());
      /* $DF3E TAY */ s_y = update_nz(s_a);
    case 0xdf3f: // [$DF3F..$DF4B]   13 bytes
      CYCLES(0xdf3f, 23);
      /* $DF3F LDA */ s_a = update_nz(peek(0xcfdc + s_y));
      /* $DF42 STA */ poke_zpg(0x91, s_a);
      /* $DF44 LDA */ s_a = update_nz(peek(0xcfdd + s_y));
      /* $DF47 STA */ poke_zpg(0x92, s_a);
      /* $DF49 JSR */ push16(0xdf4b), s_pc = 0x0090;
      branchTarget = true;
      break;
    case 0xdf4c: // [$DF4C..$DF4E]    3 bytes
      CYCLES(0xdf4c, 6);
      /* $DF4C JMP */ s_pc = 0xdd6a;
      branchTarget = true;
      break;
    case 0xdfe3: // [$DFE3..$DFE7]    5 bytes
      CYCLES(0xdfe3, 9);
      /* $DFE3 LDX */ s_x = update_nz(0x00);
      /* $DFE5 JSR */ push16(0xdfe7), s_pc = 0x00b7;
      branchTarget = true;
      break;
    case 0xdfe8: // [$DFE8..$DFE9]    2 bytes
      CYCLES(0xdfe8, 4);
      /* $DFE8 STX */ poke_zpg(0x10, s_x);
    case 0xdfea: // [$DFEA..$DFEE]    5 bytes
      CYCLES(0xdfea, 9);
      /* $DFEA STA */ poke_zpg(0x81, s_a);
      /* $DFEC JSR */ push16(0xdfee), s_pc = 0x00b7;
      branchTarget = true;
      break;
    case 0xdfef: // [$DFEF..$DFF1]    3 bytes
      CYCLES(0xdfef, 6);
      /* $DFEF JSR */ push16(0xdff1), s_pc = 0xe07d;
      branchTarget = true;
      break;
    case 0xdff2: // [$DFF2..$DFF3]    2 bytes
      CYCLES(0xdff2, 4);
      /* $DFF2 BCS */ s_pc = s_status & STATUS_C ? 0xdff7 : 0xdff4;
      branchTarget = true;
      break;
    case 0xdff4: // [$DFF4..$DFF6]    3 bytes
      CYCLES(0xdff4, 6);
      /* $DFF4 JMP */ s_pc = 0xdec9;
      branchTarget = true;
      break;
    case 0xdff7: // [$DFF7..$DFFF]    9 bytes
      CYCLES(0xdff7, 16);
      /* $DFF7 LDX */ s_x = update_nz(0x00);
      /* $DFF9 STX */ poke_zpg(0x11, s_x);
      /* $DFFB STX */ poke_zpg(0x12, s_x);
      /* $DFFD JMP */ s_pc = 0xe007;
      branchTarget = true;
      break;
    case 0xe000: // [$E000..$E002]    3 bytes
      CYCLES(0xe000, 6);
      /* $E000 JMP */ s_pc = 0xf128;
      branchTarget = true;
      break;
    case 0xe007: // [$E007..$E009]    3 bytes
      CYCLES(0xe007, 6);
      /* $E007 JSR */ push16(0xe009), s_pc = 0x00b1;
      branchTarget = true;
      break;
    case 0xe00a: // [$E00A..$E00B]    2 bytes
      CYCLES(0xe00a, 4);
      /* $E00A BCC */ s_pc = !(s_status & STATUS_C) ? 0xe011 : 0xe00c;
      branchTarget = true;
      break;
    case 0xe00c: // [$E00C..$E00E]    3 bytes
      CYCLES(0xe00c, 6);
      /* $E00C JSR */ push16(0xe00e), s_pc = 0xe07d;
      branchTarget = true;
      break;
    case 0xe00f: // [$E00F..$E010]    2 bytes
      CYCLES(0xe00f, 4);
      /* $E00F BCC */ s_pc = !(s_status & STATUS_C) ? 0xe01c : 0xe011;
      branchTarget = true;
      break;
    case 0xe011: // [$E011..$E011]    1 bytes
      CYCLES(0xe011, 2);
      /* $E011 TAX */ s_x = update_nz(s_a);
    case 0xe012: // [$E012..$E014]    3 bytes
      CYCLES(0xe012, 6);
      /* $E012 JSR */ push16(0xe014), s_pc = 0x00b1;
      branchTarget = true;
      break;
    case 0xe015: // [$E015..$E016]    2 bytes
      CYCLES(0xe015, 4);
      /* $E015 BCC */ s_pc = !(s_status & STATUS_C) ? 0xe012 : 0xe017;
      branchTarget = true;
      break;
    case 0xe017: // [$E017..$E019]    3 bytes
      CYCLES(0xe017, 6);
      /* $E017 JSR */ push16(0xe019), s_pc = 0xe07d;
      branchTarget = true;
      break;
    case 0xe01a: // [$E01A..$E01B]    2 bytes
      CYCLES(0xe01a, 4);
      /* $E01A BCS */ s_pc = s_status & STATUS_C ? 0xe012 : 0xe01c;
      branchTarget = true;
      break;
    case 0xe01c: // [$E01C..$E01F]    4 bytes
      CYCLES(0xe01c, 7);
      /* $E01C CMP */ update_nz_inv_c(s_a - 0x24);
      /* $E01E BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe026 : 0xe020;
      branchTarget = true;
      break;
    case 0xe020: // [$E020..$E025]    6 bytes
      CYCLES(0xe020, 11);
      /* $E020 LDA */ s_a = update_nz(0xff);
      /* $E022 STA */ poke_zpg(0x11, s_a);
      /* $E024 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe036 : 0xe026;
      branchTarget = true;
      break;
    case 0xe026: // [$E026..$E029]    4 bytes
      CYCLES(0xe026, 7);
      /* $E026 CMP */ update_nz_inv_c(s_a - 0x25);
      /* $E028 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe03d : 0xe02a;
      branchTarget = true;
      break;
    case 0xe02a: // [$E02A..$E02D]    4 bytes
      CYCLES(0xe02a, 7);
      /* $E02A LDA */ s_a = update_nz(peek_zpg(0x14));
      /* $E02C BMI */ s_pc = s_status & STATUS_N ? 0xdff4 : 0xe02e;
      branchTarget = true;
      break;
    case 0xe02e: // [$E02E..$E035]    8 bytes
      CYCLES(0xe02e, 14);
      /* $E02E LDA */ s_a = update_nz(0x80);
      /* $E030 STA */ poke_zpg(0x12, s_a);
      /* $E032 ORA */ s_a = update_nz(s_a | peek_zpg(0x81));
      /* $E034 STA */ poke_zpg(0x81, s_a);
    case 0xe036: // [$E036..$E03C]    7 bytes
      CYCLES(0xe036, 12);
      /* $E036 TXA */ s_a = update_nz(s_x);
      /* $E037 ORA */ s_a = update_nz(s_a | 0x80);
      /* $E039 TAX */ s_x = update_nz(s_a);
      /* $E03A JSR */ push16(0xe03c), s_pc = 0x00b1;
      branchTarget = true;
      break;
    case 0xe03d: // [$E03D..$E045]    9 bytes
      CYCLES(0xe03d, 16);
      /* $E03D STX */ poke_zpg(0x82, s_x);
      /* $E03F SEC */ s_status |= STATUS_C;
      /* $E040 ORA */ s_a = update_nz(s_a | peek_zpg(0x14));
      /* $E042 SBC */ tmp = 0x28, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $E044 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe049 : 0xe046;
      branchTarget = true;
      break;
    case 0xe046: // [$E046..$E048]    3 bytes
      CYCLES(0xe046, 6);
      /* $E046 JMP */ s_pc = 0xe11e;
      branchTarget = true;
      break;
    case 0xe049: // [$E049..$E04C]    4 bytes
      CYCLES(0xe049, 7);
      /* $E049 BIT */ tmp = peek_zpg(0x14), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $E04B BMI */ s_pc = s_status & STATUS_N ? 0xe04f : 0xe04d;
      branchTarget = true;
      break;
    case 0xe04d: // [$E04D..$E04E]    2 bytes
      CYCLES(0xe04d, 4);
      /* $E04D BVS */ s_pc = s_status & STATUS_V ? 0xe046 : 0xe04f;
      branchTarget = true;
      break;
    case 0xe04f: // [$E04F..$E058]   10 bytes
      CYCLES(0xe04f, 18);
      /* $E04F LDA */ s_a = update_nz(0x00);
      /* $E051 STA */ poke_zpg(0x14, s_a);
      /* $E053 LDA */ s_a = update_nz(peek_zpg(0x69));
      /* $E055 LDX */ s_x = update_nz(peek_zpg(0x6a));
      /* $E057 LDY */ s_y = update_nz(0x00);
    case 0xe059: // [$E059..$E05A]    2 bytes
      CYCLES(0xe059, 4);
      /* $E059 STX */ poke_zpg(0x9c, s_x);
    case 0xe05b: // [$E05B..$E060]    6 bytes
      CYCLES(0xe05b, 11);
      /* $E05B STA */ poke_zpg(0x9b, s_a);
      /* $E05D CPX */ update_nz_inv_c(s_x - peek_zpg(0x6c));
      /* $E05F BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe065 : 0xe061;
      branchTarget = true;
      break;
    case 0xe061: // [$E061..$E064]    4 bytes
      CYCLES(0xe061, 7);
      /* $E061 CMP */ update_nz_inv_c(s_a - peek_zpg(0x6b));
      /* $E063 BEQ */ s_pc = s_status & STATUS_Z ? 0xe087 : 0xe065;
      branchTarget = true;
      break;
    case 0xe065: // [$E065..$E06A]    6 bytes
      CYCLES(0xe065, 11);
      /* $E065 LDA */ s_a = update_nz(peek_zpg(0x81));
      /* $E067 CMP */ update_nz_inv_c(s_a - peek(peek16_zpg(0x9b) + s_y));
      /* $E069 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe073 : 0xe06b;
      branchTarget = true;
      break;
    case 0xe06b: // [$E06B..$E071]    7 bytes
      CYCLES(0xe06b, 12);
      /* $E06B LDA */ s_a = update_nz(peek_zpg(0x82));
      /* $E06D INY */ s_y = update_nz(s_y + 1);
      /* $E06E CMP */ update_nz_inv_c(s_a - peek(peek16_zpg(0x9b) + s_y));
      /* $E070 BEQ */ s_pc = s_status & STATUS_Z ? 0xe0de : 0xe072;
      branchTarget = true;
      break;
    case 0xe072: // [$E072..$E072]    1 bytes
      CYCLES(0xe072, 2);
      /* $E072 DEY */ s_y = update_nz(s_y - 1);
    case 0xe073: // [$E073..$E079]    7 bytes
      CYCLES(0xe073, 12);
      /* $E073 CLC */ s_status &= ~STATUS_C;
      /* $E074 LDA */ s_a = update_nz(peek_zpg(0x9b));
      /* $E076 ADC */ tmp = 0x07, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E078 BCC */ s_pc = !(s_status & STATUS_C) ? 0xe05b : 0xe07a;
      branchTarget = true;
      break;
    case 0xe07a: // [$E07A..$E07C]    3 bytes
      CYCLES(0xe07a, 6);
      /* $E07A INX */ s_x = update_nz(s_x + 1);
      /* $E07B BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe059 : 0xe07d;
      branchTarget = true;
      break;
    case 0xe07d: // [$E07D..$E080]    4 bytes
      CYCLES(0xe07d, 7);
      /* $E07D CMP */ update_nz_inv_c(s_a - 0x41);
      /* $E07F BCC */ s_pc = !(s_status & STATUS_C) ? 0xe086 : 0xe081;
      branchTarget = true;
      break;
    case 0xe081: // [$E081..$E085]    5 bytes
      CYCLES(0xe081, 9);
      /* $E081 SBC */ tmp = 0x5b, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $E083 SEC */ s_status |= STATUS_C;
      /* $E084 SBC */ tmp = 0xa5, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
    case 0xe086: // [$E086..$E086]    1 bytes
      CYCLES(0xe086, 2);
      /* $E086 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe087: // [$E087..$E08C]    6 bytes
      CYCLES(0xe087, 11);
      /* $E087 PLA */ s_a = update_nz(pop8());
      /* $E088 PHA */ push8(s_a);
      /* $E089 CMP */ update_nz_inv_c(s_a - 0xd7);
      /* $E08B BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe09c : 0xe08d;
      branchTarget = true;
      break;
    case 0xe08d: // [$E08D..$E094]    8 bytes
      CYCLES(0xe08d, 14);
      /* $E08D TSX */ s_x = update_nz(s_sp);
      /* $E08E LDA */ s_a = update_nz(peek(0x0102 + s_x));
      /* $E091 CMP */ update_nz_inv_c(s_a - 0xde);
      /* $E093 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe09c : 0xe095;
      branchTarget = true;
      break;
    case 0xe095: // [$E095..$E099]    5 bytes
      CYCLES(0xe095, 9);
      /* $E095 LDA */ s_a = update_nz(0x9a);
      /* $E097 LDY */ s_y = update_nz(0xe0);
      /* $E099 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe09c: // [$E09C..$E0B0]   21 bytes
      CYCLES(0xe09c, 36);
      /* $E09C LDA */ s_a = update_nz(peek_zpg(0x6b));
      /* $E09E LDY */ s_y = update_nz(peek_zpg(0x6c));
      /* $E0A0 STA */ poke_zpg(0x9b, s_a);
      /* $E0A2 STY */ poke_zpg(0x9c, s_y);
      /* $E0A4 LDA */ s_a = update_nz(peek_zpg(0x6d));
      /* $E0A6 LDY */ s_y = update_nz(peek_zpg(0x6e));
      /* $E0A8 STA */ poke_zpg(0x96, s_a);
      /* $E0AA STY */ poke_zpg(0x97, s_y);
      /* $E0AC CLC */ s_status &= ~STATUS_C;
      /* $E0AD ADC */ tmp = 0x07, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E0AF BCC */ s_pc = !(s_status & STATUS_C) ? 0xe0b2 : 0xe0b1;
      branchTarget = true;
      break;
    case 0xe0b1: // [$E0B1..$E0B1]    1 bytes
      CYCLES(0xe0b1, 2);
      /* $E0B1 INY */ s_y = update_nz(s_y + 1);
    case 0xe0b2: // [$E0B2..$E0B8]    7 bytes
      CYCLES(0xe0b2, 12);
      /* $E0B2 STA */ poke_zpg(0x94, s_a);
      /* $E0B4 STY */ poke_zpg(0x95, s_y);
      /* $E0B6 JSR */ push16(0xe0b8), s_pc = 0xd393;
      branchTarget = true;
      break;
    case 0xe0b9: // [$E0B9..$E0DD]   37 bytes
      CYCLES(0xe0b9, 63);
      /* $E0B9 LDA */ s_a = update_nz(peek_zpg(0x94));
      /* $E0BB LDY */ s_y = update_nz(peek_zpg(0x95));
      /* $E0BD INY */ s_y = update_nz(s_y + 1);
      /* $E0BE STA */ poke_zpg(0x6b, s_a);
      /* $E0C0 STY */ poke_zpg(0x6c, s_y);
      /* $E0C2 LDY */ s_y = update_nz(0x00);
      /* $E0C4 LDA */ s_a = update_nz(peek_zpg(0x81));
      /* $E0C6 STA */ poke(peek16_zpg(0x9b) + s_y, s_a);
      /* $E0C8 INY */ s_y = update_nz(s_y + 1);
      /* $E0C9 LDA */ s_a = update_nz(peek_zpg(0x82));
      /* $E0CB STA */ poke(peek16_zpg(0x9b) + s_y, s_a);
      /* $E0CD LDA */ s_a = update_nz(0x00);
      /* $E0CF INY */ s_y = update_nz(s_y + 1);
      /* $E0D0 STA */ poke(peek16_zpg(0x9b) + s_y, s_a);
      /* $E0D2 INY */ s_y = update_nz(s_y + 1);
      /* $E0D3 STA */ poke(peek16_zpg(0x9b) + s_y, s_a);
      /* $E0D5 INY */ s_y = update_nz(s_y + 1);
      /* $E0D6 STA */ poke(peek16_zpg(0x9b) + s_y, s_a);
      /* $E0D8 INY */ s_y = update_nz(s_y + 1);
      /* $E0D9 STA */ poke(peek16_zpg(0x9b) + s_y, s_a);
      /* $E0DB INY */ s_y = update_nz(s_y + 1);
      /* $E0DC STA */ poke(peek16_zpg(0x9b) + s_y, s_a);
    case 0xe0de: // [$E0DE..$E0E6]    9 bytes
      CYCLES(0xe0de, 16);
      /* $E0DE LDA */ s_a = update_nz(peek_zpg(0x9b));
      /* $E0E0 CLC */ s_status &= ~STATUS_C;
      /* $E0E1 ADC */ tmp = 0x02, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E0E3 LDY */ s_y = update_nz(peek_zpg(0x9c));
      /* $E0E5 BCC */ s_pc = !(s_status & STATUS_C) ? 0xe0e8 : 0xe0e7;
      branchTarget = true;
      break;
    case 0xe0e7: // [$E0E7..$E0E7]    1 bytes
      CYCLES(0xe0e7, 2);
      /* $E0E7 INY */ s_y = update_nz(s_y + 1);
    case 0xe0e8: // [$E0E8..$E0EC]    5 bytes
      CYCLES(0xe0e8, 9);
      /* $E0E8 STA */ poke_zpg(0x83, s_a);
      /* $E0EA STY */ poke_zpg(0x84, s_y);
      /* $E0EC RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe0ed: // [$E0ED..$E0F7]   11 bytes
      CYCLES(0xe0ed, 19);
      /* $E0ED LDA */ s_a = update_nz(peek_zpg(0x0f));
      /* $E0EF ASL */ s_a = update_nzc(s_a << 1);
      /* $E0F0 ADC */ tmp = 0x05, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E0F2 ADC */ tmp = peek_zpg(0x9b), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E0F4 LDY */ s_y = update_nz(peek_zpg(0x9c));
      /* $E0F6 BCC */ s_pc = !(s_status & STATUS_C) ? 0xe0f9 : 0xe0f8;
      branchTarget = true;
      break;
    case 0xe0f8: // [$E0F8..$E0F8]    1 bytes
      CYCLES(0xe0f8, 2);
      /* $E0F8 INY */ s_y = update_nz(s_y + 1);
    case 0xe0f9: // [$E0F9..$E0FD]    5 bytes
      CYCLES(0xe0f9, 9);
      /* $E0F9 STA */ poke_zpg(0x94, s_a);
      /* $E0FB STY */ poke_zpg(0x95, s_y);
      /* $E0FD RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe102: // [$E102..$E104]    3 bytes
      CYCLES(0xe102, 6);
      /* $E102 JSR */ push16(0xe104), s_pc = 0x00b1;
      branchTarget = true;
      break;
    case 0xe105: // [$E105..$E107]    3 bytes
      CYCLES(0xe105, 6);
      /* $E105 JSR */ push16(0xe107), s_pc = 0xdd67;
      branchTarget = true;
      break;
    case 0xe108: // [$E108..$E10B]    4 bytes
      CYCLES(0xe108, 7);
      /* $E108 LDA */ s_a = update_nz(peek_zpg(0xa2));
      /* $E10A BMI */ s_pc = s_status & STATUS_N ? 0xe119 : 0xe10c;
      branchTarget = true;
      break;
    case 0xe10c: // [$E10C..$E111]    6 bytes
      CYCLES(0xe10c, 11);
      /* $E10C LDA */ s_a = update_nz(peek_zpg(0x9d));
      /* $E10E CMP */ update_nz_inv_c(s_a - 0x90);
      /* $E110 BCC */ s_pc = !(s_status & STATUS_C) ? 0xe11b : 0xe112;
      branchTarget = true;
      break;
    case 0xe112: // [$E112..$E118]    7 bytes
      CYCLES(0xe112, 12);
      /* $E112 LDA */ s_a = update_nz(0xfe);
      /* $E114 LDY */ s_y = update_nz(0xe0);
      /* $E116 JSR */ push16(0xe118), s_pc = 0xebb2;
      branchTarget = true;
      break;
    case 0xe119: // [$E119..$E11A]    2 bytes
      CYCLES(0xe119, 4);
      /* $E119 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe199 : 0xe11b;
      branchTarget = true;
      break;
    case 0xe11b: // [$E11B..$E11D]    3 bytes
      CYCLES(0xe11b, 6);
      /* $E11B JMP */ s_pc = 0xebf2;
      branchTarget = true;
      break;
    case 0xe11e: // [$E11E..$E121]    4 bytes
      CYCLES(0xe11e, 7);
      /* $E11E LDA */ s_a = update_nz(peek_zpg(0x14));
      /* $E120 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe169 : 0xe122;
      branchTarget = true;
      break;
    case 0xe122: // [$E122..$E12B]   10 bytes
      CYCLES(0xe122, 18);
      /* $E122 LDA */ s_a = update_nz(peek_zpg(0x10));
      /* $E124 ORA */ s_a = update_nz(s_a | peek_zpg(0x12));
      /* $E126 PHA */ push8(s_a);
      /* $E127 LDA */ s_a = update_nz(peek_zpg(0x11));
      /* $E129 PHA */ push8(s_a);
      /* $E12A LDY */ s_y = update_nz(0x00);
    case 0xe12c: // [$E12C..$E136]   11 bytes
      CYCLES(0xe12c, 19);
      /* $E12C TYA */ s_a = update_nz(s_y);
      /* $E12D PHA */ push8(s_a);
      /* $E12E LDA */ s_a = update_nz(peek_zpg(0x82));
      /* $E130 PHA */ push8(s_a);
      /* $E131 LDA */ s_a = update_nz(peek_zpg(0x81));
      /* $E133 PHA */ push8(s_a);
      /* $E134 JSR */ push16(0xe136), s_pc = 0xe102;
      branchTarget = true;
      break;
    case 0xe137: // [$E137..$E155]   31 bytes
      CYCLES(0xe137, 53);
      /* $E137 PLA */ s_a = update_nz(pop8());
      /* $E138 STA */ poke_zpg(0x81, s_a);
      /* $E13A PLA */ s_a = update_nz(pop8());
      /* $E13B STA */ poke_zpg(0x82, s_a);
      /* $E13D PLA */ s_a = update_nz(pop8());
      /* $E13E TAY */ s_y = update_nz(s_a);
      /* $E13F TSX */ s_x = update_nz(s_sp);
      /* $E140 LDA */ s_a = update_nz(peek(0x0102 + s_x));
      /* $E143 PHA */ push8(s_a);
      /* $E144 LDA */ s_a = update_nz(peek(0x0101 + s_x));
      /* $E147 PHA */ push8(s_a);
      /* $E148 LDA */ s_a = update_nz(peek_zpg(0xa0));
      /* $E14A STA */ poke(0x0102 + s_x, s_a);
      /* $E14D LDA */ s_a = update_nz(peek_zpg(0xa1));
      /* $E14F STA */ poke(0x0101 + s_x, s_a);
      /* $E152 INY */ s_y = update_nz(s_y + 1);
      /* $E153 JSR */ push16(0xe155), s_pc = 0x00b7;
      branchTarget = true;
      break;
    case 0xe156: // [$E156..$E159]    4 bytes
      CYCLES(0xe156, 7);
      /* $E156 CMP */ update_nz_inv_c(s_a - 0x2c);
      /* $E158 BEQ */ s_pc = s_status & STATUS_Z ? 0xe12c : 0xe15a;
      branchTarget = true;
      break;
    case 0xe15a: // [$E15A..$E15E]    5 bytes
      CYCLES(0xe15a, 9);
      /* $E15A STY */ poke_zpg(0x0f, s_y);
      /* $E15C JSR */ push16(0xe15e), s_pc = 0xdeb8;
      branchTarget = true;
      break;
    case 0xe15f: // [$E15F..$E168]   10 bytes
      CYCLES(0xe15f, 18);
      /* $E15F PLA */ s_a = update_nz(pop8());
      /* $E160 STA */ poke_zpg(0x11, s_a);
      /* $E162 PLA */ s_a = update_nz(pop8());
      /* $E163 STA */ poke_zpg(0x12, s_a);
      /* $E165 AND */ s_a = update_nz(s_a & 0x7f);
      /* $E167 STA */ poke_zpg(0x10, s_a);
    case 0xe169: // [$E169..$E16C]    4 bytes
      CYCLES(0xe169, 7);
      /* $E169 LDX */ s_x = update_nz(peek_zpg(0x6b));
      /* $E16B LDA */ s_a = update_nz(peek_zpg(0x6c));
    case 0xe16d: // [$E16D..$E174]    8 bytes
      CYCLES(0xe16d, 14);
      /* $E16D STX */ poke_zpg(0x9b, s_x);
      /* $E16F STA */ poke_zpg(0x9c, s_a);
      /* $E171 CMP */ update_nz_inv_c(s_a - peek_zpg(0x6e));
      /* $E173 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe179 : 0xe175;
      branchTarget = true;
      break;
    case 0xe175: // [$E175..$E178]    4 bytes
      CYCLES(0xe175, 7);
      /* $E175 CPX */ update_nz_inv_c(s_x - peek_zpg(0x6d));
      /* $E177 BEQ */ s_pc = s_status & STATUS_Z ? 0xe1b8 : 0xe179;
      branchTarget = true;
      break;
    case 0xe179: // [$E179..$E181]    9 bytes
      CYCLES(0xe179, 16);
      /* $E179 LDY */ s_y = update_nz(0x00);
      /* $E17B LDA */ s_a = update_nz(peek(peek16_zpg(0x9b) + s_y));
      /* $E17D INY */ s_y = update_nz(s_y + 1);
      /* $E17E CMP */ update_nz_inv_c(s_a - peek_zpg(0x81));
      /* $E180 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe188 : 0xe182;
      branchTarget = true;
      break;
    case 0xe182: // [$E182..$E187]    6 bytes
      CYCLES(0xe182, 11);
      /* $E182 LDA */ s_a = update_nz(peek_zpg(0x82));
      /* $E184 CMP */ update_nz_inv_c(s_a - peek(peek16_zpg(0x9b) + s_y));
      /* $E186 BEQ */ s_pc = s_status & STATUS_Z ? 0xe19e : 0xe188;
      branchTarget = true;
      break;
    case 0xe188: // [$E188..$E195]   14 bytes
      CYCLES(0xe188, 24);
      /* $E188 INY */ s_y = update_nz(s_y + 1);
      /* $E189 LDA */ s_a = update_nz(peek(peek16_zpg(0x9b) + s_y));
      /* $E18B CLC */ s_status &= ~STATUS_C;
      /* $E18C ADC */ tmp = peek_zpg(0x9b), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E18E TAX */ s_x = update_nz(s_a);
      /* $E18F INY */ s_y = update_nz(s_y + 1);
      /* $E190 LDA */ s_a = update_nz(peek(peek16_zpg(0x9b) + s_y));
      /* $E192 ADC */ tmp = peek_zpg(0x9c), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E194 BCC */ s_pc = !(s_status & STATUS_C) ? 0xe16d : 0xe196;
      branchTarget = true;
      break;
    case 0xe196: // [$E196..$E198]    3 bytes
      CYCLES(0xe196, 6);
      /* $E196 LDX */ s_x = update_nz(0x6b);
      /* $E198 BIT */ tmp = peek(0x35a2), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
    case 0xe19b: // [$E19B..$E19D]    3 bytes
      CYCLES(0xe19b, 6);
      /* $E19B JMP */ s_pc = 0xd412;
      branchTarget = true;
      break;
    case 0xe19e: // [$E19E..$E1A3]    6 bytes
      CYCLES(0xe19e, 11);
      /* $E19E LDX */ s_x = update_nz(0x78);
      /* $E1A0 LDA */ s_a = update_nz(peek_zpg(0x10));
      /* $E1A2 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe19b : 0xe1a4;
      branchTarget = true;
      break;
    case 0xe1a4: // [$E1A4..$E1A7]    4 bytes
      CYCLES(0xe1a4, 7);
      /* $E1A4 LDA */ s_a = update_nz(peek_zpg(0x14));
      /* $E1A6 BEQ */ s_pc = s_status & STATUS_Z ? 0xe1aa : 0xe1a8;
      branchTarget = true;
      break;
    case 0xe1a8: // [$E1A8..$E1A9]    2 bytes
      CYCLES(0xe1a8, 4);
      /* $E1A8 SEC */ s_status |= STATUS_C;
      /* $E1A9 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe1aa: // [$E1AA..$E1AC]    3 bytes
      CYCLES(0xe1aa, 6);
      /* $E1AA JSR */ push16(0xe1ac), s_pc = 0xe0ed;
      branchTarget = true;
      break;
    case 0xe1ad: // [$E1AD..$E1B4]    8 bytes
      CYCLES(0xe1ad, 14);
      /* $E1AD LDA */ s_a = update_nz(peek_zpg(0x0f));
      /* $E1AF LDY */ s_y = update_nz(0x04);
      /* $E1B1 CMP */ update_nz_inv_c(s_a - peek(peek16_zpg(0x9b) + s_y));
      /* $E1B3 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe196 : 0xe1b5;
      branchTarget = true;
      break;
    case 0xe1b5: // [$E1B5..$E1B7]    3 bytes
      CYCLES(0xe1b5, 6);
      /* $E1B5 JMP */ s_pc = 0xe24b;
      branchTarget = true;
      break;
    case 0xe1b8: // [$E1B8..$E1BB]    4 bytes
      CYCLES(0xe1b8, 7);
      /* $E1B8 LDA */ s_a = update_nz(peek_zpg(0x14));
      /* $E1BA BEQ */ s_pc = s_status & STATUS_Z ? 0xe1c1 : 0xe1bc;
      branchTarget = true;
      break;
    case 0xe1bc: // [$E1BC..$E1C0]    5 bytes
      CYCLES(0xe1bc, 9);
      /* $E1BC LDX */ s_x = update_nz(0x2a);
      /* $E1BE JMP */ s_pc = 0xd412;
      branchTarget = true;
      break;
    case 0xe1c1: // [$E1C1..$E1C3]    3 bytes
      CYCLES(0xe1c1, 6);
      /* $E1C1 JSR */ push16(0xe1c3), s_pc = 0xe0ed;
      branchTarget = true;
      break;
    case 0xe1c4: // [$E1C4..$E1C6]    3 bytes
      CYCLES(0xe1c4, 6);
      /* $E1C4 JSR */ push16(0xe1c6), s_pc = 0xd3e3;
      branchTarget = true;
      break;
    case 0xe1c7: // [$E1C7..$E1D3]   13 bytes
      CYCLES(0xe1c7, 23);
      /* $E1C7 LDA */ s_a = update_nz(0x00);
      /* $E1C9 TAY */ s_y = update_nz(s_a);
      /* $E1CA STA */ poke_zpg(0xae, s_a);
      /* $E1CC LDX */ s_x = update_nz(0x05);
      /* $E1CE LDA */ s_a = update_nz(peek_zpg(0x81));
      /* $E1D0 STA */ poke(peek16_zpg(0x9b) + s_y, s_a);
      /* $E1D2 BPL */ s_pc = !(s_status & STATUS_N) ? 0xe1d5 : 0xe1d4;
      branchTarget = true;
      break;
    case 0xe1d4: // [$E1D4..$E1D4]    1 bytes
      CYCLES(0xe1d4, 2);
      /* $E1D4 DEX */ s_x = update_nz(s_x - 1);
    case 0xe1d5: // [$E1D5..$E1DB]    7 bytes
      CYCLES(0xe1d5, 12);
      /* $E1D5 INY */ s_y = update_nz(s_y + 1);
      /* $E1D6 LDA */ s_a = update_nz(peek_zpg(0x82));
      /* $E1D8 STA */ poke(peek16_zpg(0x9b) + s_y, s_a);
      /* $E1DA BPL */ s_pc = !(s_status & STATUS_N) ? 0xe1de : 0xe1dc;
      branchTarget = true;
      break;
    case 0xe1dc: // [$E1DC..$E1DD]    2 bytes
      CYCLES(0xe1dc, 4);
      /* $E1DC DEX */ s_x = update_nz(s_x - 1);
      /* $E1DD DEX */ s_x = update_nz(s_x - 1);
    case 0xe1de: // [$E1DE..$E1E6]    9 bytes
      CYCLES(0xe1de, 16);
      /* $E1DE STX */ poke_zpg(0xad, s_x);
      /* $E1E0 LDA */ s_a = update_nz(peek_zpg(0x0f));
      /* $E1E2 INY */ s_y = update_nz(s_y + 1);
      /* $E1E3 INY */ s_y = update_nz(s_y + 1);
      /* $E1E4 INY */ s_y = update_nz(s_y + 1);
      /* $E1E5 STA */ poke(peek16_zpg(0x9b) + s_y, s_a);
    case 0xe1e7: // [$E1E7..$E1EE]    8 bytes
      CYCLES(0xe1e7, 14);
      /* $E1E7 LDX */ s_x = update_nz(0x0b);
      /* $E1E9 LDA */ s_a = update_nz(0x00);
      /* $E1EB BIT */ tmp = peek_zpg(0x10), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $E1ED BVC */ s_pc = !(s_status & STATUS_V) ? 0xe1f7 : 0xe1ef;
      branchTarget = true;
      break;
    case 0xe1ef: // [$E1EF..$E1F6]    8 bytes
      CYCLES(0xe1ef, 14);
      /* $E1EF PLA */ s_a = update_nz(pop8());
      /* $E1F0 CLC */ s_status &= ~STATUS_C;
      /* $E1F1 ADC */ tmp = 0x01, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E1F3 TAX */ s_x = update_nz(s_a);
      /* $E1F4 PLA */ s_a = update_nz(pop8());
      /* $E1F5 ADC */ tmp = 0x00, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0xe1f7: // [$E1F7..$E200]   10 bytes
      CYCLES(0xe1f7, 18);
      /* $E1F7 INY */ s_y = update_nz(s_y + 1);
      /* $E1F8 STA */ poke(peek16_zpg(0x9b) + s_y, s_a);
      /* $E1FA INY */ s_y = update_nz(s_y + 1);
      /* $E1FB TXA */ s_a = update_nz(s_x);
      /* $E1FC STA */ poke(peek16_zpg(0x9b) + s_y, s_a);
      /* $E1FE JSR */ push16(0xe200), s_pc = 0xe2ad;
      branchTarget = true;
      break;
    case 0xe201: // [$E201..$E20A]   10 bytes
      CYCLES(0xe201, 18);
      /* $E201 STX */ poke_zpg(0xad, s_x);
      /* $E203 STA */ poke_zpg(0xae, s_a);
      /* $E205 LDY */ s_y = update_nz(peek_zpg(0x5e));
      /* $E207 DEC */ tmp16 = 0x0f, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $E209 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe1e7 : 0xe20b;
      branchTarget = true;
      break;
    case 0xe20b: // [$E20B..$E20E]    4 bytes
      CYCLES(0xe20b, 7);
      /* $E20B ADC */ tmp = peek_zpg(0x95), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E20D BCS */ s_pc = s_status & STATUS_C ? 0xe26c : 0xe20f;
      branchTarget = true;
      break;
    case 0xe20f: // [$E20F..$E216]    8 bytes
      CYCLES(0xe20f, 14);
      /* $E20F STA */ poke_zpg(0x95, s_a);
      /* $E211 TAY */ s_y = update_nz(s_a);
      /* $E212 TXA */ s_a = update_nz(s_x);
      /* $E213 ADC */ tmp = peek_zpg(0x94), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E215 BCC */ s_pc = !(s_status & STATUS_C) ? 0xe21a : 0xe217;
      branchTarget = true;
      break;
    case 0xe217: // [$E217..$E219]    3 bytes
      CYCLES(0xe217, 6);
      /* $E217 INY */ s_y = update_nz(s_y + 1);
      /* $E218 BEQ */ s_pc = s_status & STATUS_Z ? 0xe26c : 0xe21a;
      branchTarget = true;
      break;
    case 0xe21a: // [$E21A..$E21C]    3 bytes
      CYCLES(0xe21a, 6);
      /* $E21A JSR */ push16(0xe21c), s_pc = 0xd3e3;
      branchTarget = true;
      break;
    case 0xe21d: // [$E21D..$E228]   12 bytes
      CYCLES(0xe21d, 21);
      /* $E21D STA */ poke_zpg(0x6d, s_a);
      /* $E21F STY */ poke_zpg(0x6e, s_y);
      /* $E221 LDA */ s_a = update_nz(0x00);
      /* $E223 INC */ tmp16 = 0xae, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $E225 LDY */ s_y = update_nz(peek_zpg(0xad));
      /* $E227 BEQ */ s_pc = s_status & STATUS_Z ? 0xe22e : 0xe229;
      branchTarget = true;
      break;
    case 0xe229: // [$E229..$E22D]    5 bytes
      CYCLES(0xe229, 9);
      /* $E229 DEY */ s_y = update_nz(s_y - 1);
      /* $E22A STA */ poke(peek16_zpg(0x94) + s_y, s_a);
      /* $E22C BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe229 : 0xe22e;
      branchTarget = true;
      break;
    case 0xe22e: // [$E22E..$E233]    6 bytes
      CYCLES(0xe22e, 11);
      /* $E22E DEC */ tmp16 = 0x95, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $E230 DEC */ tmp16 = 0xae, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $E232 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe229 : 0xe234;
      branchTarget = true;
      break;
    case 0xe234: // [$E234..$E249]   22 bytes
      CYCLES(0xe234, 38);
      /* $E234 INC */ tmp16 = 0x95, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $E236 SEC */ s_status |= STATUS_C;
      /* $E237 LDA */ s_a = update_nz(peek_zpg(0x6d));
      /* $E239 SBC */ tmp = peek_zpg(0x9b), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $E23B LDY */ s_y = update_nz(0x02);
      /* $E23D STA */ poke(peek16_zpg(0x9b) + s_y, s_a);
      /* $E23F LDA */ s_a = update_nz(peek_zpg(0x6e));
      /* $E241 INY */ s_y = update_nz(s_y + 1);
      /* $E242 SBC */ tmp = peek_zpg(0x9c), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $E244 STA */ poke(peek16_zpg(0x9b) + s_y, s_a);
      /* $E246 LDA */ s_a = update_nz(peek_zpg(0x10));
      /* $E248 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe2ac : 0xe24a;
      branchTarget = true;
      break;
    case 0xe24a: // [$E24A..$E24A]    1 bytes
      CYCLES(0xe24a, 2);
      /* $E24A INY */ s_y = update_nz(s_y + 1);
    case 0xe24b: // [$E24B..$E252]    8 bytes
      CYCLES(0xe24b, 14);
      /* $E24B LDA */ s_a = update_nz(peek(peek16_zpg(0x9b) + s_y));
      /* $E24D STA */ poke_zpg(0x0f, s_a);
      /* $E24F LDA */ s_a = update_nz(0x00);
      /* $E251 STA */ poke_zpg(0xad, s_a);
    case 0xe253: // [$E253..$E260]   14 bytes
      CYCLES(0xe253, 24);
      /* $E253 STA */ poke_zpg(0xae, s_a);
      /* $E255 INY */ s_y = update_nz(s_y + 1);
      /* $E256 PLA */ s_a = update_nz(pop8());
      /* $E257 TAX */ s_x = update_nz(s_a);
      /* $E258 STA */ poke_zpg(0xa0, s_a);
      /* $E25A PLA */ s_a = update_nz(pop8());
      /* $E25B STA */ poke_zpg(0xa1, s_a);
      /* $E25D CMP */ update_nz_inv_c(s_a - peek(peek16_zpg(0x9b) + s_y));
      /* $E25F BCC */ s_pc = !(s_status & STATUS_C) ? 0xe26f : 0xe261;
      branchTarget = true;
      break;
    case 0xe261: // [$E261..$E262]    2 bytes
      CYCLES(0xe261, 4);
      /* $E261 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe269 : 0xe263;
      branchTarget = true;
      break;
    case 0xe263: // [$E263..$E268]    6 bytes
      CYCLES(0xe263, 11);
      /* $E263 INY */ s_y = update_nz(s_y + 1);
      /* $E264 TXA */ s_a = update_nz(s_x);
      /* $E265 CMP */ update_nz_inv_c(s_a - peek(peek16_zpg(0x9b) + s_y));
      /* $E267 BCC */ s_pc = !(s_status & STATUS_C) ? 0xe270 : 0xe269;
      branchTarget = true;
      break;
    case 0xe269: // [$E269..$E26B]    3 bytes
      CYCLES(0xe269, 6);
      /* $E269 JMP */ s_pc = 0xe196;
      branchTarget = true;
      break;
    case 0xe26c: // [$E26C..$E26E]    3 bytes
      CYCLES(0xe26c, 6);
      /* $E26C JMP */ s_pc = 0xd410;
      branchTarget = true;
      break;
    case 0xe26f: // [$E26F..$E26F]    1 bytes
      CYCLES(0xe26f, 2);
      /* $E26F INY */ s_y = update_nz(s_y + 1);
    case 0xe270: // [$E270..$E276]    7 bytes
      CYCLES(0xe270, 12);
      /* $E270 LDA */ s_a = update_nz(peek_zpg(0xae));
      /* $E272 ORA */ s_a = update_nz(s_a | peek_zpg(0xad));
      /* $E274 CLC */ s_status &= ~STATUS_C;
      /* $E275 BEQ */ s_pc = s_status & STATUS_Z ? 0xe281 : 0xe277;
      branchTarget = true;
      break;
    case 0xe277: // [$E277..$E279]    3 bytes
      CYCLES(0xe277, 6);
      /* $E277 JSR */ push16(0xe279), s_pc = 0xe2ad;
      branchTarget = true;
      break;
    case 0xe27a: // [$E27A..$E280]    7 bytes
      CYCLES(0xe27a, 12);
      /* $E27A TXA */ s_a = update_nz(s_x);
      /* $E27B ADC */ tmp = peek_zpg(0xa0), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E27D TAX */ s_x = update_nz(s_a);
      /* $E27E TYA */ s_a = update_nz(s_y);
      /* $E27F LDY */ s_y = update_nz(peek_zpg(0x5e));
    case 0xe281: // [$E281..$E288]    8 bytes
      CYCLES(0xe281, 14);
      /* $E281 ADC */ tmp = peek_zpg(0xa1), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E283 STX */ poke_zpg(0xad, s_x);
      /* $E285 DEC */ tmp16 = 0x0f, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $E287 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe253 : 0xe289;
      branchTarget = true;
      break;
    case 0xe289: // [$E289..$E290]    8 bytes
      CYCLES(0xe289, 14);
      /* $E289 STA */ poke_zpg(0xae, s_a);
      /* $E28B LDX */ s_x = update_nz(0x05);
      /* $E28D LDA */ s_a = update_nz(peek_zpg(0x81));
      /* $E28F BPL */ s_pc = !(s_status & STATUS_N) ? 0xe292 : 0xe291;
      branchTarget = true;
      break;
    case 0xe291: // [$E291..$E291]    1 bytes
      CYCLES(0xe291, 2);
      /* $E291 DEX */ s_x = update_nz(s_x - 1);
    case 0xe292: // [$E292..$E295]    4 bytes
      CYCLES(0xe292, 7);
      /* $E292 LDA */ s_a = update_nz(peek_zpg(0x82));
      /* $E294 BPL */ s_pc = !(s_status & STATUS_N) ? 0xe298 : 0xe296;
      branchTarget = true;
      break;
    case 0xe296: // [$E296..$E297]    2 bytes
      CYCLES(0xe296, 4);
      /* $E296 DEX */ s_x = update_nz(s_x - 1);
      /* $E297 DEX */ s_x = update_nz(s_x - 1);
    case 0xe298: // [$E298..$E29E]    7 bytes
      CYCLES(0xe298, 12);
      /* $E298 STX */ poke_zpg(0x64, s_x);
      /* $E29A LDA */ s_a = update_nz(0x00);
      /* $E29C JSR */ push16(0xe29e), s_pc = 0xe2b6;
      branchTarget = true;
      break;
    case 0xe29f: // [$E29F..$E2AB]   13 bytes
      CYCLES(0xe29f, 23);
      /* $E29F TXA */ s_a = update_nz(s_x);
      /* $E2A0 ADC */ tmp = peek_zpg(0x94), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E2A2 STA */ poke_zpg(0x83, s_a);
      /* $E2A4 TYA */ s_a = update_nz(s_y);
      /* $E2A5 ADC */ tmp = peek_zpg(0x95), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E2A7 STA */ poke_zpg(0x84, s_a);
      /* $E2A9 TAY */ s_y = update_nz(s_a);
      /* $E2AA LDA */ s_a = update_nz(peek_zpg(0x83));
    case 0xe2ac: // [$E2AC..$E2AC]    1 bytes
      CYCLES(0xe2ac, 2);
      /* $E2AC RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe2ad: // [$E2AD..$E2B5]    9 bytes
      CYCLES(0xe2ad, 16);
      /* $E2AD STY */ poke_zpg(0x5e, s_y);
      /* $E2AF LDA */ s_a = update_nz(peek(peek16_zpg(0x9b) + s_y));
      /* $E2B1 STA */ poke_zpg(0x64, s_a);
      /* $E2B3 DEY */ s_y = update_nz(s_y - 1);
      /* $E2B4 LDA */ s_a = update_nz(peek(peek16_zpg(0x9b) + s_y));
    case 0xe2b6: // [$E2B6..$E2BF]   10 bytes
      CYCLES(0xe2b6, 18);
      /* $E2B6 STA */ poke_zpg(0x65, s_a);
      /* $E2B8 LDA */ s_a = update_nz(0x10);
      /* $E2BA STA */ poke_zpg(0x99, s_a);
      /* $E2BC LDX */ s_x = update_nz(0x00);
      /* $E2BE LDY */ s_y = update_nz(0x00);
    case 0xe2c0: // [$E2C0..$E2C7]    8 bytes
      CYCLES(0xe2c0, 14);
      /* $E2C0 TXA */ s_a = update_nz(s_x);
      /* $E2C1 ASL */ s_a = update_nzc(s_a << 1);
      /* $E2C2 TAX */ s_x = update_nz(s_a);
      /* $E2C3 TYA */ s_a = update_nz(s_y);
      /* $E2C4 ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $E2C5 TAY */ s_y = update_nz(s_a);
      /* $E2C6 BCS */ s_pc = s_status & STATUS_C ? 0xe26c : 0xe2c8;
      branchTarget = true;
      break;
    case 0xe2c8: // [$E2C8..$E2CD]    6 bytes
      CYCLES(0xe2c8, 11);
      /* $E2C8 ASL */ s_a = update_nzc(peek_zpg(0xad) << 1);
      /* $E2CA ROL */ tmp16 = 0xae, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $E2CC BCC */ s_pc = !(s_status & STATUS_C) ? 0xe2d9 : 0xe2ce;
      branchTarget = true;
      break;
    case 0xe2ce: // [$E2CE..$E2D8]   11 bytes
      CYCLES(0xe2ce, 19);
      /* $E2CE CLC */ s_status &= ~STATUS_C;
      /* $E2CF TXA */ s_a = update_nz(s_x);
      /* $E2D0 ADC */ tmp = peek_zpg(0x64), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E2D2 TAX */ s_x = update_nz(s_a);
      /* $E2D3 TYA */ s_a = update_nz(s_y);
      /* $E2D4 ADC */ tmp = peek_zpg(0x65), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E2D6 TAY */ s_y = update_nz(s_a);
      /* $E2D7 BCS */ s_pc = s_status & STATUS_C ? 0xe26c : 0xe2d9;
      branchTarget = true;
      break;
    case 0xe2d9: // [$E2D9..$E2DC]    4 bytes
      CYCLES(0xe2d9, 7);
      /* $E2D9 DEC */ tmp16 = 0x99, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $E2DB BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe2c0 : 0xe2dd;
      branchTarget = true;
      break;
    case 0xe2dd: // [$E2DD..$E2DD]    1 bytes
      CYCLES(0xe2dd, 2);
      /* $E2DD RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe2f2: // [$E2F2..$E2FE]   13 bytes
      CYCLES(0xe2f2, 23);
      /* $E2F2 LDX */ s_x = update_nz(0x00);
      /* $E2F4 STX */ poke_zpg(0x11, s_x);
      /* $E2F6 STA */ poke_zpg(0x9e, s_a);
      /* $E2F8 STY */ poke_zpg(0x9f, s_y);
      /* $E2FA LDX */ s_x = update_nz(0x90);
      /* $E2FC JMP */ s_pc = 0xeb9b;
      branchTarget = true;
      break;
    case 0xe301: // [$E301..$E305]    5 bytes
      CYCLES(0xe301, 9);
      /* $E301 LDA */ s_a = update_nz(0x00);
      /* $E303 SEC */ s_status |= STATUS_C;
      /* $E304 BEQ */ s_pc = s_status & STATUS_Z ? 0xe2f2 : 0xe306;
      branchTarget = true;
      break;
    case 0xe306: // [$E306..$E30A]    5 bytes
      CYCLES(0xe306, 9);
      /* $E306 LDX */ s_x = update_nz(peek_zpg(0x76));
      /* $E308 INX */ s_x = update_nz(s_x + 1);
      /* $E309 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe2ac : 0xe30b;
      branchTarget = true;
      break;
    case 0xe30b: // [$E30B..$E30D]    3 bytes
      CYCLES(0xe30b, 6);
      /* $E30B LDX */ s_x = update_nz(0x95);
      /* $E30D BIT */ tmp = peek(0xe0a2), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $E310 JMP */ s_pc = 0xd412;
      branchTarget = true;
      break;
    case 0xe341: // [$E341..$E345]    5 bytes
      CYCLES(0xe341, 9);
      /* $E341 LDA */ s_a = update_nz(0xc2);
      /* $E343 JSR */ push16(0xe345), s_pc = 0xdec0;
      branchTarget = true;
      break;
    case 0xe346: // [$E346..$E34C]    7 bytes
      CYCLES(0xe346, 12);
      /* $E346 ORA */ s_a = update_nz(s_a | 0x80);
      /* $E348 STA */ poke_zpg(0x14, s_a);
      /* $E34A JSR */ push16(0xe34c), s_pc = 0xdfea;
      branchTarget = true;
      break;
    case 0xe34d: // [$E34D..$E353]    7 bytes
      CYCLES(0xe34d, 12);
      /* $E34D STA */ poke_zpg(0x8a, s_a);
      /* $E34F STY */ poke_zpg(0x8b, s_y);
      /* $E351 JMP */ s_pc = 0xdd6a;
      branchTarget = true;
      break;
    case 0xe354: // [$E354..$E356]    3 bytes
      CYCLES(0xe354, 6);
      /* $E354 JSR */ push16(0xe356), s_pc = 0xe341;
      branchTarget = true;
      break;
    case 0xe357: // [$E357..$E35F]    9 bytes
      CYCLES(0xe357, 16);
      /* $E357 LDA */ s_a = update_nz(peek_zpg(0x8b));
      /* $E359 PHA */ push8(s_a);
      /* $E35A LDA */ s_a = update_nz(peek_zpg(0x8a));
      /* $E35C PHA */ push8(s_a);
      /* $E35D JSR */ push16(0xe35f), s_pc = 0xdeb2;
      branchTarget = true;
      break;
    case 0xe360: // [$E360..$E362]    3 bytes
      CYCLES(0xe360, 6);
      /* $E360 JSR */ push16(0xe362), s_pc = 0xdd6a;
      branchTarget = true;
      break;
    case 0xe363: // [$E363..$E374]   18 bytes
      CYCLES(0xe363, 31);
      /* $E363 PLA */ s_a = update_nz(pop8());
      /* $E364 STA */ poke_zpg(0x8a, s_a);
      /* $E366 PLA */ s_a = update_nz(pop8());
      /* $E367 STA */ poke_zpg(0x8b, s_a);
      /* $E369 LDY */ s_y = update_nz(0x02);
      /* $E36B LDA */ s_a = update_nz(peek(peek16_zpg(0x8a) + s_y));
      /* $E36D STA */ poke_zpg(0x83, s_a);
      /* $E36F TAX */ s_x = update_nz(s_a);
      /* $E370 INY */ s_y = update_nz(s_y + 1);
      /* $E371 LDA */ s_a = update_nz(peek(peek16_zpg(0x8a) + s_y));
      /* $E373 BEQ */ s_pc = s_status & STATUS_Z ? 0xe30e : 0xe375;
      branchTarget = true;
      break;
    case 0xe375: // [$E375..$E377]    3 bytes
      CYCLES(0xe375, 6);
      /* $E375 STA */ poke_zpg(0x84, s_a);
      /* $E377 INY */ s_y = update_nz(s_y + 1);
    case 0xe378: // [$E378..$E37D]    6 bytes
      CYCLES(0xe378, 11);
      /* $E378 LDA */ s_a = update_nz(peek(peek16_zpg(0x83) + s_y));
      /* $E37A PHA */ push8(s_a);
      /* $E37B DEY */ s_y = update_nz(s_y - 1);
      /* $E37C BPL */ s_pc = !(s_status & STATUS_N) ? 0xe378 : 0xe37e;
      branchTarget = true;
      break;
    case 0xe37e: // [$E37E..$E382]    5 bytes
      CYCLES(0xe37e, 9);
      /* $E37E LDY */ s_y = update_nz(peek_zpg(0x84));
      /* $E380 JSR */ push16(0xe382), s_pc = 0xeb2b;
      branchTarget = true;
      break;
    case 0xe383: // [$E383..$E39A]   24 bytes
      CYCLES(0xe383, 41);
      /* $E383 LDA */ s_a = update_nz(peek_zpg(0xb9));
      /* $E385 PHA */ push8(s_a);
      /* $E386 LDA */ s_a = update_nz(peek_zpg(0xb8));
      /* $E388 PHA */ push8(s_a);
      /* $E389 LDA */ s_a = update_nz(peek(peek16_zpg(0x8a) + s_y));
      /* $E38B STA */ poke_zpg(0xb8, s_a);
      /* $E38D INY */ s_y = update_nz(s_y + 1);
      /* $E38E LDA */ s_a = update_nz(peek(peek16_zpg(0x8a) + s_y));
      /* $E390 STA */ poke_zpg(0xb9, s_a);
      /* $E392 LDA */ s_a = update_nz(peek_zpg(0x84));
      /* $E394 PHA */ push8(s_a);
      /* $E395 LDA */ s_a = update_nz(peek_zpg(0x83));
      /* $E397 PHA */ push8(s_a);
      /* $E398 JSR */ push16(0xe39a), s_pc = 0xdd67;
      branchTarget = true;
      break;
    case 0xe39b: // [$E39B..$E3A3]    9 bytes
      CYCLES(0xe39b, 16);
      /* $E39B PLA */ s_a = update_nz(pop8());
      /* $E39C STA */ poke_zpg(0x8a, s_a);
      /* $E39E PLA */ s_a = update_nz(pop8());
      /* $E39F STA */ poke_zpg(0x8b, s_a);
      /* $E3A1 JSR */ push16(0xe3a3), s_pc = 0x00b7;
      branchTarget = true;
      break;
    case 0xe3a4: // [$E3A4..$E3A5]    2 bytes
      CYCLES(0xe3a4, 4);
      /* $E3A4 BEQ */ s_pc = s_status & STATUS_Z ? 0xe3a9 : 0xe3a6;
      branchTarget = true;
      break;
    case 0xe3a6: // [$E3A6..$E3A8]    3 bytes
      CYCLES(0xe3a6, 6);
      /* $E3A6 JMP */ s_pc = 0xdec9;
      branchTarget = true;
      break;
    case 0xe3a9: // [$E3A9..$E3C4]   28 bytes
      CYCLES(0xe3a9, 48);
      /* $E3A9 PLA */ s_a = update_nz(pop8());
      /* $E3AA STA */ poke_zpg(0xb8, s_a);
      /* $E3AC PLA */ s_a = update_nz(pop8());
      /* $E3AD STA */ poke_zpg(0xb9, s_a);
      /* $E3AF LDY */ s_y = update_nz(0x00);
      /* $E3B1 PLA */ s_a = update_nz(pop8());
      /* $E3B2 STA */ poke(peek16_zpg(0x8a) + s_y, s_a);
      /* $E3B4 PLA */ s_a = update_nz(pop8());
      /* $E3B5 INY */ s_y = update_nz(s_y + 1);
      /* $E3B6 STA */ poke(peek16_zpg(0x8a) + s_y, s_a);
      /* $E3B8 PLA */ s_a = update_nz(pop8());
      /* $E3B9 INY */ s_y = update_nz(s_y + 1);
      /* $E3BA STA */ poke(peek16_zpg(0x8a) + s_y, s_a);
      /* $E3BC PLA */ s_a = update_nz(pop8());
      /* $E3BD INY */ s_y = update_nz(s_y + 1);
      /* $E3BE STA */ poke(peek16_zpg(0x8a) + s_y, s_a);
      /* $E3C0 PLA */ s_a = update_nz(pop8());
      /* $E3C1 INY */ s_y = update_nz(s_y + 1);
      /* $E3C2 STA */ poke(peek16_zpg(0x8a) + s_y, s_a);
      /* $E3C4 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe3d5: // [$E3D5..$E3DF]   11 bytes
      CYCLES(0xe3d5, 19);
      /* $E3D5 LDX */ s_x = update_nz(peek_zpg(0xa0));
      /* $E3D7 LDY */ s_y = update_nz(peek_zpg(0xa1));
      /* $E3D9 STX */ poke_zpg(0x8c, s_x);
      /* $E3DB STY */ poke_zpg(0x8d, s_y);
      /* $E3DD JSR */ push16(0xe3df), s_pc = 0xe452;
      branchTarget = true;
      break;
    case 0xe3e0: // [$E3E0..$E3E6]    7 bytes
      CYCLES(0xe3e0, 12);
      /* $E3E0 STX */ poke_zpg(0x9e, s_x);
      /* $E3E2 STY */ poke_zpg(0x9f, s_y);
      /* $E3E4 STA */ poke_zpg(0x9d, s_a);
      /* $E3E6 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe3e7: // [$E3E7..$E3F6]   16 bytes
      CYCLES(0xe3e7, 28);
      /* $E3E7 LDX */ s_x = update_nz(0x22);
      /* $E3E9 STX */ poke_zpg(0x0d, s_x);
      /* $E3EB STX */ poke_zpg(0x0e, s_x);
      /* $E3ED STA */ poke_zpg(0xab, s_a);
      /* $E3EF STY */ poke_zpg(0xac, s_y);
      /* $E3F1 STA */ poke_zpg(0x9e, s_a);
      /* $E3F3 STY */ poke_zpg(0x9f, s_y);
      /* $E3F5 LDY */ s_y = update_nz(0xff);
    case 0xe3f7: // [$E3F7..$E3FB]    5 bytes
      CYCLES(0xe3f7, 9);
      /* $E3F7 INY */ s_y = update_nz(s_y + 1);
      /* $E3F8 LDA */ s_a = update_nz(peek(peek16_zpg(0xab) + s_y));
      /* $E3FA BEQ */ s_pc = s_status & STATUS_Z ? 0xe408 : 0xe3fc;
      branchTarget = true;
      break;
    case 0xe3fc: // [$E3FC..$E3FF]    4 bytes
      CYCLES(0xe3fc, 7);
      /* $E3FC CMP */ update_nz_inv_c(s_a - peek_zpg(0x0d));
      /* $E3FE BEQ */ s_pc = s_status & STATUS_Z ? 0xe404 : 0xe400;
      branchTarget = true;
      break;
    case 0xe400: // [$E400..$E403]    4 bytes
      CYCLES(0xe400, 7);
      /* $E400 CMP */ update_nz_inv_c(s_a - peek_zpg(0x0e));
      /* $E402 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe3f7 : 0xe404;
      branchTarget = true;
      break;
    case 0xe404: // [$E404..$E407]    4 bytes
      CYCLES(0xe404, 7);
      /* $E404 CMP */ update_nz_inv_c(s_a - 0x22);
      /* $E406 BEQ */ s_pc = s_status & STATUS_Z ? 0xe409 : 0xe408;
      branchTarget = true;
      break;
    case 0xe408: // [$E408..$E408]    1 bytes
      CYCLES(0xe408, 2);
      /* $E408 CLC */ s_status &= ~STATUS_C;
    case 0xe409: // [$E409..$E413]   11 bytes
      CYCLES(0xe409, 19);
      /* $E409 STY */ poke_zpg(0x9d, s_y);
      /* $E40B TYA */ s_a = update_nz(s_y);
      /* $E40C ADC */ tmp = peek_zpg(0xab), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E40E STA */ poke_zpg(0xad, s_a);
      /* $E410 LDX */ s_x = update_nz(peek_zpg(0xac));
      /* $E412 BCC */ s_pc = !(s_status & STATUS_C) ? 0xe415 : 0xe414;
      branchTarget = true;
      break;
    case 0xe414: // [$E414..$E414]    1 bytes
      CYCLES(0xe414, 2);
      /* $E414 INX */ s_x = update_nz(s_x + 1);
    case 0xe415: // [$E415..$E41A]    6 bytes
      CYCLES(0xe415, 11);
      /* $E415 STX */ poke_zpg(0xae, s_x);
      /* $E417 LDA */ s_a = update_nz(peek_zpg(0xac));
      /* $E419 BEQ */ s_pc = s_status & STATUS_Z ? 0xe41f : 0xe41b;
      branchTarget = true;
      break;
    case 0xe41b: // [$E41B..$E41E]    4 bytes
      CYCLES(0xe41b, 7);
      /* $E41B CMP */ update_nz_inv_c(s_a - 0x02);
      /* $E41D BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe42a : 0xe41f;
      branchTarget = true;
      break;
    case 0xe41f: // [$E41F..$E422]    4 bytes
      CYCLES(0xe41f, 7);
      /* $E41F TYA */ s_a = update_nz(s_y);
      /* $E420 JSR */ push16(0xe422), s_pc = 0xe3d5;
      branchTarget = true;
      break;
    case 0xe423: // [$E423..$E429]    7 bytes
      CYCLES(0xe423, 12);
      /* $E423 LDX */ s_x = update_nz(peek_zpg(0xab));
      /* $E425 LDY */ s_y = update_nz(peek_zpg(0xac));
      /* $E427 JSR */ push16(0xe429), s_pc = 0xe5e2;
      branchTarget = true;
      break;
    case 0xe42a: // [$E42A..$E42F]    6 bytes
      CYCLES(0xe42a, 11);
      /* $E42A LDX */ s_x = update_nz(peek_zpg(0x52));
      /* $E42C CPX */ update_nz_inv_c(s_x - 0x5e);
      /* $E42E BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe435 : 0xe430;
      branchTarget = true;
      break;
    case 0xe430: // [$E430..$E431]    2 bytes
      CYCLES(0xe430, 4);
      /* $E430 LDX */ s_x = update_nz(0xbf);
    case 0xe432: // [$E432..$E434]    3 bytes
      CYCLES(0xe432, 6);
      /* $E432 JMP */ s_pc = 0xd412;
      branchTarget = true;
      break;
    case 0xe435: // [$E435..$E451]   29 bytes
      CYCLES(0xe435, 50);
      /* $E435 LDA */ s_a = update_nz(peek_zpg(0x9d));
      /* $E437 STA */ poke_zpg((uint8_t)(0x00 + s_x), s_a);
      /* $E439 LDA */ s_a = update_nz(peek_zpg(0x9e));
      /* $E43B STA */ poke_zpg((uint8_t)(0x01 + s_x), s_a);
      /* $E43D LDA */ s_a = update_nz(peek_zpg(0x9f));
      /* $E43F STA */ poke_zpg((uint8_t)(0x02 + s_x), s_a);
      /* $E441 LDY */ s_y = update_nz(0x00);
      /* $E443 STX */ poke_zpg(0xa0, s_x);
      /* $E445 STY */ poke_zpg(0xa1, s_y);
      /* $E447 DEY */ s_y = update_nz(s_y - 1);
      /* $E448 STY */ poke_zpg(0x11, s_y);
      /* $E44A STX */ poke_zpg(0x53, s_x);
      /* $E44C INX */ s_x = update_nz(s_x + 1);
      /* $E44D INX */ s_x = update_nz(s_x + 1);
      /* $E44E INX */ s_x = update_nz(s_x + 1);
      /* $E44F STX */ poke_zpg(0x52, s_x);
      /* $E451 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe452: // [$E452..$E453]    2 bytes
      CYCLES(0xe452, 4);
      /* $E452 LSR */ tmp16 = 0x13, tmp = peek_zpg(tmp16), set_c_to_bit0(tmp), poke_zpg(tmp16, update_nz(tmp >> 1));
    case 0xe454: // [$E454..$E45D]   10 bytes
      CYCLES(0xe454, 18);
      /* $E454 PHA */ push8(s_a);
      /* $E455 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $E457 SEC */ s_status |= STATUS_C;
      /* $E458 ADC */ tmp = peek_zpg(0x6f), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E45A LDY */ s_y = update_nz(peek_zpg(0x70));
      /* $E45C BCS */ s_pc = s_status & STATUS_C ? 0xe45f : 0xe45e;
      branchTarget = true;
      break;
    case 0xe45e: // [$E45E..$E45E]    1 bytes
      CYCLES(0xe45e, 2);
      /* $E45E DEY */ s_y = update_nz(s_y - 1);
    case 0xe45f: // [$E45F..$E462]    4 bytes
      CYCLES(0xe45f, 7);
      /* $E45F CPY */ update_nz_inv_c(s_y - peek_zpg(0x6e));
      /* $E461 BCC */ s_pc = !(s_status & STATUS_C) ? 0xe474 : 0xe463;
      branchTarget = true;
      break;
    case 0xe463: // [$E463..$E464]    2 bytes
      CYCLES(0xe463, 4);
      /* $E463 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe469 : 0xe465;
      branchTarget = true;
      break;
    case 0xe465: // [$E465..$E468]    4 bytes
      CYCLES(0xe465, 7);
      /* $E465 CMP */ update_nz_inv_c(s_a - peek_zpg(0x6d));
      /* $E467 BCC */ s_pc = !(s_status & STATUS_C) ? 0xe474 : 0xe469;
      branchTarget = true;
      break;
    case 0xe469: // [$E469..$E473]   11 bytes
      CYCLES(0xe469, 19);
      /* $E469 STA */ poke_zpg(0x6f, s_a);
      /* $E46B STY */ poke_zpg(0x70, s_y);
      /* $E46D STA */ poke_zpg(0x71, s_a);
      /* $E46F STY */ poke_zpg(0x72, s_y);
      /* $E471 TAX */ s_x = update_nz(s_a);
      /* $E472 PLA */ s_a = update_nz(pop8());
      /* $E473 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe474: // [$E474..$E479]    6 bytes
      CYCLES(0xe474, 11);
      /* $E474 LDX */ s_x = update_nz(0x4d);
      /* $E476 LDA */ s_a = update_nz(peek_zpg(0x13));
      /* $E478 BMI */ s_pc = s_status & STATUS_N ? 0xe432 : 0xe47a;
      branchTarget = true;
      break;
    case 0xe47a: // [$E47A..$E47C]    3 bytes
      CYCLES(0xe47a, 6);
      /* $E47A JSR */ push16(0xe47c), s_pc = 0xe484;
      branchTarget = true;
      break;
    case 0xe47d: // [$E47D..$E483]    7 bytes
      CYCLES(0xe47d, 12);
      /* $E47D LDA */ s_a = update_nz(0x80);
      /* $E47F STA */ poke_zpg(0x13, s_a);
      /* $E481 PLA */ s_a = update_nz(pop8());
      /* $E482 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe454 : 0xe484;
      branchTarget = true;
      break;
    case 0xe484: // [$E484..$E487]    4 bytes
      CYCLES(0xe484, 7);
      /* $E484 LDX */ s_x = update_nz(peek_zpg(0x73));
      /* $E486 LDA */ s_a = update_nz(peek_zpg(0x74));
    case 0xe488: // [$E488..$E49F]   24 bytes
      CYCLES(0xe488, 41);
      /* $E488 STX */ poke_zpg(0x6f, s_x);
      /* $E48A STA */ poke_zpg(0x70, s_a);
      /* $E48C LDY */ s_y = update_nz(0x00);
      /* $E48E STY */ poke_zpg(0x8b, s_y);
      /* $E490 LDA */ s_a = update_nz(peek_zpg(0x6d));
      /* $E492 LDX */ s_x = update_nz(peek_zpg(0x6e));
      /* $E494 STA */ poke_zpg(0x9b, s_a);
      /* $E496 STX */ poke_zpg(0x9c, s_x);
      /* $E498 LDA */ s_a = update_nz(0x55);
      /* $E49A LDX */ s_x = update_nz(0x00);
      /* $E49C STA */ poke_zpg(0x5e, s_a);
      /* $E49E STX */ poke_zpg(0x5f, s_x);
    case 0xe4a0: // [$E4A0..$E4A3]    4 bytes
      CYCLES(0xe4a0, 7);
      /* $E4A0 CMP */ update_nz_inv_c(s_a - peek_zpg(0x52));
      /* $E4A2 BEQ */ s_pc = s_status & STATUS_Z ? 0xe4a9 : 0xe4a4;
      branchTarget = true;
      break;
    case 0xe4a4: // [$E4A4..$E4A6]    3 bytes
      CYCLES(0xe4a4, 6);
      /* $E4A4 JSR */ push16(0xe4a6), s_pc = 0xe523;
      branchTarget = true;
      break;
    case 0xe4a7: // [$E4A7..$E4A8]    2 bytes
      CYCLES(0xe4a7, 4);
      /* $E4A7 BEQ */ s_pc = s_status & STATUS_Z ? 0xe4a0 : 0xe4a9;
      branchTarget = true;
      break;
    case 0xe4a9: // [$E4A9..$E4B4]   12 bytes
      CYCLES(0xe4a9, 21);
      /* $E4A9 LDA */ s_a = update_nz(0x07);
      /* $E4AB STA */ poke_zpg(0x8f, s_a);
      /* $E4AD LDA */ s_a = update_nz(peek_zpg(0x69));
      /* $E4AF LDX */ s_x = update_nz(peek_zpg(0x6a));
      /* $E4B1 STA */ poke_zpg(0x5e, s_a);
      /* $E4B3 STX */ poke_zpg(0x5f, s_x);
    case 0xe4b5: // [$E4B5..$E4B8]    4 bytes
      CYCLES(0xe4b5, 7);
      /* $E4B5 CPX */ update_nz_inv_c(s_x - peek_zpg(0x6c));
      /* $E4B7 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe4bd : 0xe4b9;
      branchTarget = true;
      break;
    case 0xe4b9: // [$E4B9..$E4BC]    4 bytes
      CYCLES(0xe4b9, 7);
      /* $E4B9 CMP */ update_nz_inv_c(s_a - peek_zpg(0x6b));
      /* $E4BB BEQ */ s_pc = s_status & STATUS_Z ? 0xe4c2 : 0xe4bd;
      branchTarget = true;
      break;
    case 0xe4bd: // [$E4BD..$E4BF]    3 bytes
      CYCLES(0xe4bd, 6);
      /* $E4BD JSR */ push16(0xe4bf), s_pc = 0xe519;
      branchTarget = true;
      break;
    case 0xe4c0: // [$E4C0..$E4C1]    2 bytes
      CYCLES(0xe4c0, 4);
      /* $E4C0 BEQ */ s_pc = s_status & STATUS_Z ? 0xe4b5 : 0xe4c2;
      branchTarget = true;
      break;
    case 0xe4c2: // [$E4C2..$E4C9]    8 bytes
      CYCLES(0xe4c2, 14);
      /* $E4C2 STA */ poke_zpg(0x94, s_a);
      /* $E4C4 STX */ poke_zpg(0x95, s_x);
      /* $E4C6 LDA */ s_a = update_nz(0x03);
      /* $E4C8 STA */ poke_zpg(0x8f, s_a);
    case 0xe4ca: // [$E4CA..$E4CD]    4 bytes
      CYCLES(0xe4ca, 7);
      /* $E4CA LDA */ s_a = update_nz(peek_zpg(0x94));
      /* $E4CC LDX */ s_x = update_nz(peek_zpg(0x95));
    case 0xe4ce: // [$E4CE..$E4D1]    4 bytes
      CYCLES(0xe4ce, 7);
      /* $E4CE CPX */ update_nz_inv_c(s_x - peek_zpg(0x6e));
      /* $E4D0 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe4d9 : 0xe4d2;
      branchTarget = true;
      break;
    case 0xe4d2: // [$E4D2..$E4D5]    4 bytes
      CYCLES(0xe4d2, 7);
      /* $E4D2 CMP */ update_nz_inv_c(s_a - peek_zpg(0x6d));
      /* $E4D4 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe4d9 : 0xe4d6;
      branchTarget = true;
      break;
    case 0xe4d6: // [$E4D6..$E4D8]    3 bytes
      CYCLES(0xe4d6, 6);
      /* $E4D6 JMP */ s_pc = 0xe562;
      branchTarget = true;
      break;
    case 0xe4d9: // [$E4D9..$E4F6]   30 bytes
      CYCLES(0xe4d9, 52);
      /* $E4D9 STA */ poke_zpg(0x5e, s_a);
      /* $E4DB STX */ poke_zpg(0x5f, s_x);
      /* $E4DD LDY */ s_y = update_nz(0x00);
      /* $E4DF LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $E4E1 TAX */ s_x = update_nz(s_a);
      /* $E4E2 INY */ s_y = update_nz(s_y + 1);
      /* $E4E3 LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $E4E5 PHP */ push8(s_status | STATUS_B);
      /* $E4E6 INY */ s_y = update_nz(s_y + 1);
      /* $E4E7 LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $E4E9 ADC */ tmp = peek_zpg(0x94), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E4EB STA */ poke_zpg(0x94, s_a);
      /* $E4ED INY */ s_y = update_nz(s_y + 1);
      /* $E4EE LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $E4F0 ADC */ tmp = peek_zpg(0x95), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E4F2 STA */ poke_zpg(0x95, s_a);
      /* $E4F4 PLP */ s_status = pop8() & ~STATUS_B;
      /* $E4F5 BPL */ s_pc = !(s_status & STATUS_N) ? 0xe4ca : 0xe4f7;
      branchTarget = true;
      break;
    case 0xe4f7: // [$E4F7..$E4F9]    3 bytes
      CYCLES(0xe4f7, 6);
      /* $E4F7 TXA */ s_a = update_nz(s_x);
      /* $E4F8 BMI */ s_pc = s_status & STATUS_N ? 0xe4ca : 0xe4fa;
      branchTarget = true;
      break;
    case 0xe4fa: // [$E4FA..$E507]   14 bytes
      CYCLES(0xe4fa, 24);
      /* $E4FA INY */ s_y = update_nz(s_y + 1);
      /* $E4FB LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $E4FD LDY */ s_y = update_nz(0x00);
      /* $E4FF ASL */ s_a = update_nzc(s_a << 1);
      /* $E500 ADC */ tmp = 0x05, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E502 ADC */ tmp = peek_zpg(0x5e), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E504 STA */ poke_zpg(0x5e, s_a);
      /* $E506 BCC */ s_pc = !(s_status & STATUS_C) ? 0xe50a : 0xe508;
      branchTarget = true;
      break;
    case 0xe508: // [$E508..$E509]    2 bytes
      CYCLES(0xe508, 4);
      /* $E508 INC */ tmp16 = 0x5f, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
    case 0xe50a: // [$E50A..$E50B]    2 bytes
      CYCLES(0xe50a, 4);
      /* $E50A LDX */ s_x = update_nz(peek_zpg(0x5f));
    case 0xe50c: // [$E50C..$E50F]    4 bytes
      CYCLES(0xe50c, 7);
      /* $E50C CPX */ update_nz_inv_c(s_x - peek_zpg(0x95));
      /* $E50E BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe514 : 0xe510;
      branchTarget = true;
      break;
    case 0xe510: // [$E510..$E513]    4 bytes
      CYCLES(0xe510, 7);
      /* $E510 CMP */ update_nz_inv_c(s_a - peek_zpg(0x94));
      /* $E512 BEQ */ s_pc = s_status & STATUS_Z ? 0xe4ce : 0xe514;
      branchTarget = true;
      break;
    case 0xe514: // [$E514..$E516]    3 bytes
      CYCLES(0xe514, 6);
      /* $E514 JSR */ push16(0xe516), s_pc = 0xe523;
      branchTarget = true;
      break;
    case 0xe517: // [$E517..$E518]    2 bytes
      CYCLES(0xe517, 4);
      /* $E517 BEQ */ s_pc = s_status & STATUS_Z ? 0xe50c : 0xe519;
      branchTarget = true;
      break;
    case 0xe519: // [$E519..$E51C]    4 bytes
      CYCLES(0xe519, 7);
      /* $E519 LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $E51B BMI */ s_pc = s_status & STATUS_N ? 0xe552 : 0xe51d;
      branchTarget = true;
      break;
    case 0xe51d: // [$E51D..$E521]    5 bytes
      CYCLES(0xe51d, 9);
      /* $E51D INY */ s_y = update_nz(s_y + 1);
      /* $E51E LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $E520 BPL */ s_pc = !(s_status & STATUS_N) ? 0xe552 : 0xe522;
      branchTarget = true;
      break;
    case 0xe522: // [$E522..$E522]    1 bytes
      CYCLES(0xe522, 2);
      /* $E522 INY */ s_y = update_nz(s_y + 1);
    case 0xe523: // [$E523..$E526]    4 bytes
      CYCLES(0xe523, 7);
      /* $E523 LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $E525 BEQ */ s_pc = s_status & STATUS_Z ? 0xe552 : 0xe527;
      branchTarget = true;
      break;
    case 0xe527: // [$E527..$E531]   11 bytes
      CYCLES(0xe527, 19);
      /* $E527 INY */ s_y = update_nz(s_y + 1);
      /* $E528 LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $E52A TAX */ s_x = update_nz(s_a);
      /* $E52B INY */ s_y = update_nz(s_y + 1);
      /* $E52C LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $E52E CMP */ update_nz_inv_c(s_a - peek_zpg(0x70));
      /* $E530 BCC */ s_pc = !(s_status & STATUS_C) ? 0xe538 : 0xe532;
      branchTarget = true;
      break;
    case 0xe532: // [$E532..$E533]    2 bytes
      CYCLES(0xe532, 4);
      /* $E532 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe552 : 0xe534;
      branchTarget = true;
      break;
    case 0xe534: // [$E534..$E537]    4 bytes
      CYCLES(0xe534, 7);
      /* $E534 CPX */ update_nz_inv_c(s_x - peek_zpg(0x6f));
      /* $E536 BCS */ s_pc = s_status & STATUS_C ? 0xe552 : 0xe538;
      branchTarget = true;
      break;
    case 0xe538: // [$E538..$E53B]    4 bytes
      CYCLES(0xe538, 7);
      /* $E538 CMP */ update_nz_inv_c(s_a - peek_zpg(0x9c));
      /* $E53A BCC */ s_pc = !(s_status & STATUS_C) ? 0xe552 : 0xe53c;
      branchTarget = true;
      break;
    case 0xe53c: // [$E53C..$E53D]    2 bytes
      CYCLES(0xe53c, 4);
      /* $E53C BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe542 : 0xe53e;
      branchTarget = true;
      break;
    case 0xe53e: // [$E53E..$E541]    4 bytes
      CYCLES(0xe53e, 7);
      /* $E53E CPX */ update_nz_inv_c(s_x - peek_zpg(0x9b));
      /* $E540 BCC */ s_pc = !(s_status & STATUS_C) ? 0xe552 : 0xe542;
      branchTarget = true;
      break;
    case 0xe542: // [$E542..$E551]   16 bytes
      CYCLES(0xe542, 28);
      /* $E542 STX */ poke_zpg(0x9b, s_x);
      /* $E544 STA */ poke_zpg(0x9c, s_a);
      /* $E546 LDA */ s_a = update_nz(peek_zpg(0x5e));
      /* $E548 LDX */ s_x = update_nz(peek_zpg(0x5f));
      /* $E54A STA */ poke_zpg(0x8a, s_a);
      /* $E54C STX */ poke_zpg(0x8b, s_x);
      /* $E54E LDA */ s_a = update_nz(peek_zpg(0x8f));
      /* $E550 STA */ poke_zpg(0x91, s_a);
    case 0xe552: // [$E552..$E55A]    9 bytes
      CYCLES(0xe552, 16);
      /* $E552 LDA */ s_a = update_nz(peek_zpg(0x8f));
      /* $E554 CLC */ s_status &= ~STATUS_C;
      /* $E555 ADC */ tmp = peek_zpg(0x5e), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E557 STA */ poke_zpg(0x5e, s_a);
      /* $E559 BCC */ s_pc = !(s_status & STATUS_C) ? 0xe55d : 0xe55b;
      branchTarget = true;
      break;
    case 0xe55b: // [$E55B..$E55C]    2 bytes
      CYCLES(0xe55b, 4);
      /* $E55B INC */ tmp16 = 0x5f, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
    case 0xe55d: // [$E55D..$E561]    5 bytes
      CYCLES(0xe55d, 9);
      /* $E55D LDX */ s_x = update_nz(peek_zpg(0x5f));
      /* $E55F LDY */ s_y = update_nz(0x00);
      /* $E561 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe562: // [$E562..$E565]    4 bytes
      CYCLES(0xe562, 7);
      /* $E562 LDX */ s_x = update_nz(peek_zpg(0x8b));
      /* $E564 BEQ */ s_pc = s_status & STATUS_Z ? 0xe55d : 0xe566;
      branchTarget = true;
      break;
    case 0xe566: // [$E566..$E584]   31 bytes
      CYCLES(0xe566, 53);
      /* $E566 LDA */ s_a = update_nz(peek_zpg(0x91));
      /* $E568 AND */ s_a = update_nz(s_a & 0x04);
      /* $E56A LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $E56B TAY */ s_y = update_nz(s_a);
      /* $E56C STA */ poke_zpg(0x91, s_a);
      /* $E56E LDA */ s_a = update_nz(peek(peek16_zpg(0x8a) + s_y));
      /* $E570 ADC */ tmp = peek_zpg(0x9b), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E572 STA */ poke_zpg(0x96, s_a);
      /* $E574 LDA */ s_a = update_nz(peek_zpg(0x9c));
      /* $E576 ADC */ tmp = 0x00, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E578 STA */ poke_zpg(0x97, s_a);
      /* $E57A LDA */ s_a = update_nz(peek_zpg(0x6f));
      /* $E57C LDX */ s_x = update_nz(peek_zpg(0x70));
      /* $E57E STA */ poke_zpg(0x94, s_a);
      /* $E580 STX */ poke_zpg(0x95, s_x);
      /* $E582 JSR */ push16(0xe584), s_pc = 0xd39a;
      branchTarget = true;
      break;
    case 0xe585: // [$E585..$E596]   18 bytes
      CYCLES(0xe585, 31);
      /* $E585 LDY */ s_y = update_nz(peek_zpg(0x91));
      /* $E587 INY */ s_y = update_nz(s_y + 1);
      /* $E588 LDA */ s_a = update_nz(peek_zpg(0x94));
      /* $E58A STA */ poke(peek16_zpg(0x8a) + s_y, s_a);
      /* $E58C TAX */ s_x = update_nz(s_a);
      /* $E58D INC */ tmp16 = 0x95, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $E58F LDA */ s_a = update_nz(peek_zpg(0x95));
      /* $E591 INY */ s_y = update_nz(s_y + 1);
      /* $E592 STA */ poke(peek16_zpg(0x8a) + s_y, s_a);
      /* $E594 JMP */ s_pc = 0xe488;
      branchTarget = true;
      break;
    case 0xe597: // [$E597..$E59F]    9 bytes
      CYCLES(0xe597, 16);
      /* $E597 LDA */ s_a = update_nz(peek_zpg(0xa1));
      /* $E599 PHA */ push8(s_a);
      /* $E59A LDA */ s_a = update_nz(peek_zpg(0xa0));
      /* $E59C PHA */ push8(s_a);
      /* $E59D JSR */ push16(0xe59f), s_pc = 0xde60;
      branchTarget = true;
      break;
    case 0xe5a0: // [$E5A0..$E5A2]    3 bytes
      CYCLES(0xe5a0, 6);
      /* $E5A0 JSR */ push16(0xe5a2), s_pc = 0xdd6c;
      branchTarget = true;
      break;
    case 0xe5a3: // [$E5A3..$E5B1]   15 bytes
      CYCLES(0xe5a3, 26);
      /* $E5A3 PLA */ s_a = update_nz(pop8());
      /* $E5A4 STA */ poke_zpg(0xab, s_a);
      /* $E5A6 PLA */ s_a = update_nz(pop8());
      /* $E5A7 STA */ poke_zpg(0xac, s_a);
      /* $E5A9 LDY */ s_y = update_nz(0x00);
      /* $E5AB LDA */ s_a = update_nz(peek(peek16_zpg(0xab) + s_y));
      /* $E5AD CLC */ s_status &= ~STATUS_C;
      /* $E5AE ADC */ tmp = peek(peek16_zpg(0xa0) + s_y), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E5B0 BCC */ s_pc = !(s_status & STATUS_C) ? 0xe5b7 : 0xe5b2;
      branchTarget = true;
      break;
    case 0xe5b2: // [$E5B2..$E5B6]    5 bytes
      CYCLES(0xe5b2, 9);
      /* $E5B2 LDX */ s_x = update_nz(0xb0);
      /* $E5B4 JMP */ s_pc = 0xd412;
      branchTarget = true;
      break;
    case 0xe5b7: // [$E5B7..$E5B9]    3 bytes
      CYCLES(0xe5b7, 6);
      /* $E5B7 JSR */ push16(0xe5b9), s_pc = 0xe3d5;
      branchTarget = true;
      break;
    case 0xe5ba: // [$E5BA..$E5BC]    3 bytes
      CYCLES(0xe5ba, 6);
      /* $E5BA JSR */ push16(0xe5bc), s_pc = 0xe5d4;
      branchTarget = true;
      break;
    case 0xe5bd: // [$E5BD..$E5C3]    7 bytes
      CYCLES(0xe5bd, 12);
      /* $E5BD LDA */ s_a = update_nz(peek_zpg(0x8c));
      /* $E5BF LDY */ s_y = update_nz(peek_zpg(0x8d));
      /* $E5C1 JSR */ push16(0xe5c3), s_pc = 0xe604;
      branchTarget = true;
      break;
    case 0xe5c4: // [$E5C4..$E5C6]    3 bytes
      CYCLES(0xe5c4, 6);
      /* $E5C4 JSR */ push16(0xe5c6), s_pc = 0xe5e6;
      branchTarget = true;
      break;
    case 0xe5c7: // [$E5C7..$E5CD]    7 bytes
      CYCLES(0xe5c7, 12);
      /* $E5C7 LDA */ s_a = update_nz(peek_zpg(0xab));
      /* $E5C9 LDY */ s_y = update_nz(peek_zpg(0xac));
      /* $E5CB JSR */ push16(0xe5cd), s_pc = 0xe604;
      branchTarget = true;
      break;
    case 0xe5ce: // [$E5CE..$E5D0]    3 bytes
      CYCLES(0xe5ce, 6);
      /* $E5CE JSR */ push16(0xe5d0), s_pc = 0xe42a;
      branchTarget = true;
      break;
    case 0xe5d1: // [$E5D1..$E5D3]    3 bytes
      CYCLES(0xe5d1, 6);
      /* $E5D1 JMP */ s_pc = 0xdd95;
      branchTarget = true;
      break;
    case 0xe5d4: // [$E5D4..$E5E1]   14 bytes
      CYCLES(0xe5d4, 24);
      /* $E5D4 LDY */ s_y = update_nz(0x00);
      /* $E5D6 LDA */ s_a = update_nz(peek(peek16_zpg(0xab) + s_y));
      /* $E5D8 PHA */ push8(s_a);
      /* $E5D9 INY */ s_y = update_nz(s_y + 1);
      /* $E5DA LDA */ s_a = update_nz(peek(peek16_zpg(0xab) + s_y));
      /* $E5DC TAX */ s_x = update_nz(s_a);
      /* $E5DD INY */ s_y = update_nz(s_y + 1);
      /* $E5DE LDA */ s_a = update_nz(peek(peek16_zpg(0xab) + s_y));
      /* $E5E0 TAY */ s_y = update_nz(s_a);
      /* $E5E1 PLA */ s_a = update_nz(pop8());
    case 0xe5e2: // [$E5E2..$E5E5]    4 bytes
      CYCLES(0xe5e2, 7);
      /* $E5E2 STX */ poke_zpg(0x5e, s_x);
      /* $E5E4 STY */ poke_zpg(0x5f, s_y);
    case 0xe5e6: // [$E5E6..$E5E8]    3 bytes
      CYCLES(0xe5e6, 6);
      /* $E5E6 TAY */ s_y = update_nz(s_a);
      /* $E5E7 BEQ */ s_pc = s_status & STATUS_Z ? 0xe5f3 : 0xe5e9;
      branchTarget = true;
      break;
    case 0xe5e9: // [$E5E9..$E5E9]    1 bytes
      CYCLES(0xe5e9, 2);
      /* $E5E9 PHA */ push8(s_a);
    case 0xe5ea: // [$E5EA..$E5F1]    8 bytes
      CYCLES(0xe5ea, 14);
      /* $E5EA DEY */ s_y = update_nz(s_y - 1);
      /* $E5EB LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $E5ED STA */ poke(peek16_zpg(0x71) + s_y, s_a);
      /* $E5EF TYA */ s_a = update_nz(s_y);
      /* $E5F0 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe5ea : 0xe5f2;
      branchTarget = true;
      break;
    case 0xe5f2: // [$E5F2..$E5F2]    1 bytes
      CYCLES(0xe5f2, 2);
      /* $E5F2 PLA */ s_a = update_nz(pop8());
    case 0xe5f3: // [$E5F3..$E5F9]    7 bytes
      CYCLES(0xe5f3, 12);
      /* $E5F3 CLC */ s_status &= ~STATUS_C;
      /* $E5F4 ADC */ tmp = peek_zpg(0x71), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E5F6 STA */ poke_zpg(0x71, s_a);
      /* $E5F8 BCC */ s_pc = !(s_status & STATUS_C) ? 0xe5fc : 0xe5fa;
      branchTarget = true;
      break;
    case 0xe5fa: // [$E5FA..$E5FB]    2 bytes
      CYCLES(0xe5fa, 4);
      /* $E5FA INC */ tmp16 = 0x72, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
    case 0xe5fc: // [$E5FC..$E5FC]    1 bytes
      CYCLES(0xe5fc, 2);
      /* $E5FC RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe600: // [$E600..$E603]    4 bytes
      CYCLES(0xe600, 7);
      /* $E600 LDA */ s_a = update_nz(peek_zpg(0xa0));
      /* $E602 LDY */ s_y = update_nz(peek_zpg(0xa1));
    case 0xe604: // [$E604..$E60A]    7 bytes
      CYCLES(0xe604, 12);
      /* $E604 STA */ poke_zpg(0x5e, s_a);
      /* $E606 STY */ poke_zpg(0x5f, s_y);
      /* $E608 JSR */ push16(0xe60a), s_pc = 0xe635;
      branchTarget = true;
      break;
    case 0xe60b: // [$E60B..$E61C]   18 bytes
      CYCLES(0xe60b, 31);
      /* $E60B PHP */ push8(s_status | STATUS_B);
      /* $E60C LDY */ s_y = update_nz(0x00);
      /* $E60E LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $E610 PHA */ push8(s_a);
      /* $E611 INY */ s_y = update_nz(s_y + 1);
      /* $E612 LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $E614 TAX */ s_x = update_nz(s_a);
      /* $E615 INY */ s_y = update_nz(s_y + 1);
      /* $E616 LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $E618 TAY */ s_y = update_nz(s_a);
      /* $E619 PLA */ s_a = update_nz(pop8());
      /* $E61A PLP */ s_status = pop8() & ~STATUS_B;
      /* $E61B BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe630 : 0xe61d;
      branchTarget = true;
      break;
    case 0xe61d: // [$E61D..$E620]    4 bytes
      CYCLES(0xe61d, 7);
      /* $E61D CPY */ update_nz_inv_c(s_y - peek_zpg(0x70));
      /* $E61F BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe630 : 0xe621;
      branchTarget = true;
      break;
    case 0xe621: // [$E621..$E624]    4 bytes
      CYCLES(0xe621, 7);
      /* $E621 CPX */ update_nz_inv_c(s_x - peek_zpg(0x6f));
      /* $E623 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe630 : 0xe625;
      branchTarget = true;
      break;
    case 0xe625: // [$E625..$E62C]    8 bytes
      CYCLES(0xe625, 14);
      /* $E625 PHA */ push8(s_a);
      /* $E626 CLC */ s_status &= ~STATUS_C;
      /* $E627 ADC */ tmp = peek_zpg(0x6f), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E629 STA */ poke_zpg(0x6f, s_a);
      /* $E62B BCC */ s_pc = !(s_status & STATUS_C) ? 0xe62f : 0xe62d;
      branchTarget = true;
      break;
    case 0xe62d: // [$E62D..$E62E]    2 bytes
      CYCLES(0xe62d, 4);
      /* $E62D INC */ tmp16 = 0x70, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
    case 0xe62f: // [$E62F..$E62F]    1 bytes
      CYCLES(0xe62f, 2);
      /* $E62F PLA */ s_a = update_nz(pop8());
    case 0xe630: // [$E630..$E634]    5 bytes
      CYCLES(0xe630, 9);
      /* $E630 STX */ poke_zpg(0x5e, s_x);
      /* $E632 STY */ poke_zpg(0x5f, s_y);
      /* $E634 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe635: // [$E635..$E638]    4 bytes
      CYCLES(0xe635, 7);
      /* $E635 CPY */ update_nz_inv_c(s_y - peek_zpg(0x54));
      /* $E637 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe645 : 0xe639;
      branchTarget = true;
      break;
    case 0xe639: // [$E639..$E63C]    4 bytes
      CYCLES(0xe639, 7);
      /* $E639 CMP */ update_nz_inv_c(s_a - peek_zpg(0x53));
      /* $E63B BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe645 : 0xe63d;
      branchTarget = true;
      break;
    case 0xe63d: // [$E63D..$E644]    8 bytes
      CYCLES(0xe63d, 14);
      /* $E63D STA */ poke_zpg(0x52, s_a);
      /* $E63F SBC */ tmp = 0x03, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $E641 STA */ poke_zpg(0x53, s_a);
      /* $E643 LDY */ s_y = update_nz(0x00);
    case 0xe645: // [$E645..$E645]    1 bytes
      CYCLES(0xe645, 2);
      /* $E645 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe6f2: // [$E6F2..$E6F4]    3 bytes
      CYCLES(0xe6f2, 6);
      /* $E6F2 JMP */ s_pc = 0xe199;
      branchTarget = true;
      break;
    case 0xe6f8: // [$E6F8..$E6FA]    3 bytes
      CYCLES(0xe6f8, 6);
      /* $E6F8 JSR */ push16(0xe6fa), s_pc = 0xdd67;
      branchTarget = true;
      break;
    case 0xe6fb: // [$E6FB..$E6FD]    3 bytes
      CYCLES(0xe6fb, 6);
      /* $E6FB JSR */ push16(0xe6fd), s_pc = 0xe108;
      branchTarget = true;
      break;
    case 0xe6fe: // [$E6FE..$E701]    4 bytes
      CYCLES(0xe6fe, 7);
      /* $E6FE LDX */ s_x = update_nz(peek_zpg(0xa0));
      /* $E700 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe6f2 : 0xe702;
      branchTarget = true;
      break;
    case 0xe702: // [$E702..$E706]    5 bytes
      CYCLES(0xe702, 9);
      /* $E702 LDX */ s_x = update_nz(peek_zpg(0xa1));
      /* $E704 JMP */ s_pc = 0x00b7;
      branchTarget = true;
      break;
    case 0xe73d: // [$E73D..$E745]    9 bytes
      CYCLES(0xe73d, 16);
      /* $E73D LDX */ s_x = update_nz(peek_zpg(0xad));
      /* $E73F LDY */ s_y = update_nz(peek_zpg(0xae));
      /* $E741 STX */ poke_zpg(0xb8, s_x);
      /* $E743 STY */ poke_zpg(0xb9, s_y);
      /* $E745 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe79f: // [$E79F..$E79F]    1 bytes
      CYCLES(0xe79f, 2);
      /* $E79F RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe7a0: // [$E7A0..$E7A6]    7 bytes
      CYCLES(0xe7a0, 12);
      /* $E7A0 LDA */ s_a = update_nz(0x64);
      /* $E7A2 LDY */ s_y = update_nz(0xee);
      /* $E7A4 JMP */ s_pc = 0xe7be;
      branchTarget = true;
      break;
    case 0xe7b9: // [$E7B9..$E7BB]    3 bytes
      CYCLES(0xe7b9, 6);
      /* $E7B9 JSR */ push16(0xe7bb), s_pc = 0xe8f0;
      branchTarget = true;
      break;
    case 0xe7bc: // [$E7BC..$E7BD]    2 bytes
      CYCLES(0xe7bc, 4);
      /* $E7BC BCC */ s_pc = !(s_status & STATUS_C) ? 0xe7fa : 0xe7be;
      branchTarget = true;
      break;
    case 0xe7be: // [$E7BE..$E7C0]    3 bytes
      CYCLES(0xe7be, 6);
      /* $E7BE JSR */ push16(0xe7c0), s_pc = 0xe9e3;
      branchTarget = true;
      break;
    case 0xe7c1: // [$E7C1..$E7C2]    2 bytes
      CYCLES(0xe7c1, 4);
      /* $E7C1 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe7c6 : 0xe7c3;
      branchTarget = true;
      break;
    case 0xe7c3: // [$E7C3..$E7C5]    3 bytes
      CYCLES(0xe7c3, 6);
      /* $E7C3 JMP */ s_pc = 0xeb53;
      branchTarget = true;
      break;
    case 0xe7c6: // [$E7C6..$E7CD]    8 bytes
      CYCLES(0xe7c6, 14);
      /* $E7C6 LDX */ s_x = update_nz(peek_zpg(0xac));
      /* $E7C8 STX */ poke_zpg(0x92, s_x);
      /* $E7CA LDX */ s_x = update_nz(0xa5);
      /* $E7CC LDA */ s_a = update_nz(peek_zpg(0xa5));
    case 0xe7ce: // [$E7CE..$E7D0]    3 bytes
      CYCLES(0xe7ce, 6);
      /* $E7CE TAY */ s_y = update_nz(s_a);
      /* $E7CF BEQ */ s_pc = s_status & STATUS_Z ? 0xe79f : 0xe7d1;
      branchTarget = true;
      break;
    case 0xe7d1: // [$E7D1..$E7D5]    5 bytes
      CYCLES(0xe7d1, 9);
      /* $E7D1 SEC */ s_status |= STATUS_C;
      /* $E7D2 SBC */ tmp = peek_zpg(0x9d), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $E7D4 BEQ */ s_pc = s_status & STATUS_Z ? 0xe7fa : 0xe7d6;
      branchTarget = true;
      break;
    case 0xe7d6: // [$E7D6..$E7D7]    2 bytes
      CYCLES(0xe7d6, 4);
      /* $E7D6 BCC */ s_pc = !(s_status & STATUS_C) ? 0xe7ea : 0xe7d8;
      branchTarget = true;
      break;
    case 0xe7d8: // [$E7D8..$E7E9]   18 bytes
      CYCLES(0xe7d8, 31);
      /* $E7D8 STY */ poke_zpg(0x9d, s_y);
      /* $E7DA LDY */ s_y = update_nz(peek_zpg(0xaa));
      /* $E7DC STY */ poke_zpg(0xa2, s_y);
      /* $E7DE EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $E7E0 ADC */ tmp = 0x00, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E7E2 LDY */ s_y = update_nz(0x00);
      /* $E7E4 STY */ poke_zpg(0x92, s_y);
      /* $E7E6 LDX */ s_x = update_nz(0x9d);
      /* $E7E8 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe7ee : 0xe7ea;
      branchTarget = true;
      break;
    case 0xe7ea: // [$E7EA..$E7ED]    4 bytes
      CYCLES(0xe7ea, 7);
      /* $E7EA LDY */ s_y = update_nz(0x00);
      /* $E7EC STY */ poke_zpg(0xac, s_y);
    case 0xe7ee: // [$E7EE..$E7F1]    4 bytes
      CYCLES(0xe7ee, 7);
      /* $E7EE CMP */ update_nz_inv_c(s_a - 0xf9);
      /* $E7F0 BMI */ s_pc = s_status & STATUS_N ? 0xe7b9 : 0xe7f2;
      branchTarget = true;
      break;
    case 0xe7f2: // [$E7F2..$E7F9]    8 bytes
      CYCLES(0xe7f2, 14);
      /* $E7F2 TAY */ s_y = update_nz(s_a);
      /* $E7F3 LDA */ s_a = update_nz(peek_zpg(0xac));
      /* $E7F5 LSR */ tmp16 = (uint8_t)(0x01 + s_x), tmp = peek_zpg(tmp16), set_c_to_bit0(tmp), poke_zpg(tmp16, update_nz(tmp >> 1));
      /* $E7F7 JSR */ push16(0xe7f9), s_pc = 0xe907;
      branchTarget = true;
      break;
    case 0xe7fa: // [$E7FA..$E7FD]    4 bytes
      CYCLES(0xe7fa, 7);
      /* $E7FA BIT */ tmp = peek_zpg(0xab), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $E7FC BPL */ s_pc = !(s_status & STATUS_N) ? 0xe855 : 0xe7fe;
      branchTarget = true;
      break;
    case 0xe7fe: // [$E7FE..$E803]    6 bytes
      CYCLES(0xe7fe, 11);
      /* $E7FE LDY */ s_y = update_nz(0x9d);
      /* $E800 CPX */ update_nz_inv_c(s_x - 0xa5);
      /* $E802 BEQ */ s_pc = s_status & STATUS_Z ? 0xe806 : 0xe804;
      branchTarget = true;
      break;
    case 0xe804: // [$E804..$E805]    2 bytes
      CYCLES(0xe804, 4);
      /* $E804 LDY */ s_y = update_nz(0xa5);
    case 0xe806: // [$E806..$E828]   35 bytes
      CYCLES(0xe806, 60);
      /* $E806 SEC */ s_status |= STATUS_C;
      /* $E807 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $E809 ADC */ tmp = peek_zpg(0x92), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E80B STA */ poke_zpg(0xac, s_a);
      /* $E80D LDA */ s_a = update_nz(peek(0x0004 + s_y));
      /* $E810 SBC */ tmp = peek_zpg((uint8_t)(0x04 + s_x)), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $E812 STA */ poke_zpg(0xa1, s_a);
      /* $E814 LDA */ s_a = update_nz(peek(0x0003 + s_y));
      /* $E817 SBC */ tmp = peek_zpg((uint8_t)(0x03 + s_x)), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $E819 STA */ poke_zpg(0xa0, s_a);
      /* $E81B LDA */ s_a = update_nz(peek(0x0002 + s_y));
      /* $E81E SBC */ tmp = peek_zpg((uint8_t)(0x02 + s_x)), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $E820 STA */ poke_zpg(0x9f, s_a);
      /* $E822 LDA */ s_a = update_nz(peek(0x0001 + s_y));
      /* $E825 SBC */ tmp = peek_zpg((uint8_t)(0x01 + s_x)), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $E827 STA */ poke_zpg(0x9e, s_a);
    case 0xe829: // [$E829..$E82A]    2 bytes
      CYCLES(0xe829, 4);
      /* $E829 BCS */ s_pc = s_status & STATUS_C ? 0xe82e : 0xe82b;
      branchTarget = true;
      break;
    case 0xe82b: // [$E82B..$E82D]    3 bytes
      CYCLES(0xe82b, 6);
      /* $E82B JSR */ push16(0xe82d), s_pc = 0xe89e;
      branchTarget = true;
      break;
    case 0xe82e: // [$E82E..$E831]    4 bytes
      CYCLES(0xe82e, 7);
      /* $E82E LDY */ s_y = update_nz(0x00);
      /* $E830 TYA */ s_a = update_nz(s_y);
      /* $E831 CLC */ s_status &= ~STATUS_C;
    case 0xe832: // [$E832..$E835]    4 bytes
      CYCLES(0xe832, 7);
      /* $E832 LDX */ s_x = update_nz(peek_zpg(0x9e));
      /* $E834 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe880 : 0xe836;
      branchTarget = true;
      break;
    case 0xe836: // [$E836..$E84D]   24 bytes
      CYCLES(0xe836, 41);
      /* $E836 LDX */ s_x = update_nz(peek_zpg(0x9f));
      /* $E838 STX */ poke_zpg(0x9e, s_x);
      /* $E83A LDX */ s_x = update_nz(peek_zpg(0xa0));
      /* $E83C STX */ poke_zpg(0x9f, s_x);
      /* $E83E LDX */ s_x = update_nz(peek_zpg(0xa1));
      /* $E840 STX */ poke_zpg(0xa0, s_x);
      /* $E842 LDX */ s_x = update_nz(peek_zpg(0xac));
      /* $E844 STX */ poke_zpg(0xa1, s_x);
      /* $E846 STY */ poke_zpg(0xac, s_y);
      /* $E848 ADC */ tmp = 0x08, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E84A CMP */ update_nz_inv_c(s_a - 0x20);
      /* $E84C BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe832 : 0xe84e;
      branchTarget = true;
      break;
    case 0xe84e: // [$E84E..$E851]    4 bytes
      CYCLES(0xe84e, 7);
      /* $E84E LDA */ s_a = update_nz(0x00);
      /* $E850 STA */ poke_zpg(0x9d, s_a);
    case 0xe852: // [$E852..$E854]    3 bytes
      CYCLES(0xe852, 6);
      /* $E852 STA */ poke_zpg(0xa2, s_a);
      /* $E854 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe855: // [$E855..$E873]   31 bytes
      CYCLES(0xe855, 53);
      /* $E855 ADC */ tmp = peek_zpg(0x92), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E857 STA */ poke_zpg(0xac, s_a);
      /* $E859 LDA */ s_a = update_nz(peek_zpg(0xa1));
      /* $E85B ADC */ tmp = peek_zpg(0xa9), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E85D STA */ poke_zpg(0xa1, s_a);
      /* $E85F LDA */ s_a = update_nz(peek_zpg(0xa0));
      /* $E861 ADC */ tmp = peek_zpg(0xa8), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E863 STA */ poke_zpg(0xa0, s_a);
      /* $E865 LDA */ s_a = update_nz(peek_zpg(0x9f));
      /* $E867 ADC */ tmp = peek_zpg(0xa7), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E869 STA */ poke_zpg(0x9f, s_a);
      /* $E86B LDA */ s_a = update_nz(peek_zpg(0x9e));
      /* $E86D ADC */ tmp = peek_zpg(0xa6), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E86F STA */ poke_zpg(0x9e, s_a);
      /* $E871 JMP */ s_pc = 0xe88d;
      branchTarget = true;
      break;
    case 0xe874: // [$E874..$E87F]   12 bytes
      CYCLES(0xe874, 21);
      /* $E874 ADC */ tmp = 0x01, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E876 ASL */ s_a = update_nzc(peek_zpg(0xac) << 1);
      /* $E878 ROL */ tmp16 = 0xa1, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $E87A ROL */ tmp16 = 0xa0, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $E87C ROL */ tmp16 = 0x9f, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $E87E ROL */ tmp16 = 0x9e, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
    case 0xe880: // [$E880..$E881]    2 bytes
      CYCLES(0xe880, 4);
      /* $E880 BPL */ s_pc = !(s_status & STATUS_N) ? 0xe874 : 0xe882;
      branchTarget = true;
      break;
    case 0xe882: // [$E882..$E886]    5 bytes
      CYCLES(0xe882, 9);
      /* $E882 SEC */ s_status |= STATUS_C;
      /* $E883 SBC */ tmp = peek_zpg(0x9d), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $E885 BCS */ s_pc = s_status & STATUS_C ? 0xe84e : 0xe887;
      branchTarget = true;
      break;
    case 0xe887: // [$E887..$E88C]    6 bytes
      CYCLES(0xe887, 11);
      /* $E887 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $E889 ADC */ tmp = 0x01, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E88B STA */ poke_zpg(0x9d, s_a);
    case 0xe88d: // [$E88D..$E88E]    2 bytes
      CYCLES(0xe88d, 4);
      /* $E88D BCC */ s_pc = !(s_status & STATUS_C) ? 0xe89d : 0xe88f;
      branchTarget = true;
      break;
    case 0xe88f: // [$E88F..$E892]    4 bytes
      CYCLES(0xe88f, 7);
      /* $E88F INC */ tmp16 = 0x9d, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $E891 BEQ */ s_pc = s_status & STATUS_Z ? 0xe8d5 : 0xe893;
      branchTarget = true;
      break;
    case 0xe893: // [$E893..$E89C]   10 bytes
      CYCLES(0xe893, 18);
      /* $E893 ROR */ tmp16 = 0x9e, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7))), set_c_to_bit0(tmp);
      /* $E895 ROR */ tmp16 = 0x9f, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7))), set_c_to_bit0(tmp);
      /* $E897 ROR */ tmp16 = 0xa0, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7))), set_c_to_bit0(tmp);
      /* $E899 ROR */ tmp16 = 0xa1, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7))), set_c_to_bit0(tmp);
      /* $E89B ROR */ tmp16 = 0xac, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7))), set_c_to_bit0(tmp);
    case 0xe89d: // [$E89D..$E89D]    1 bytes
      CYCLES(0xe89d, 2);
      /* $E89D RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe89e: // [$E89E..$E8A3]    6 bytes
      CYCLES(0xe89e, 11);
      /* $E89E LDA */ s_a = update_nz(peek_zpg(0xa2));
      /* $E8A0 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $E8A2 STA */ poke_zpg(0xa2, s_a);
    case 0xe8a4: // [$E8A4..$E8C5]   34 bytes
      CYCLES(0xe8a4, 58);
      /* $E8A4 LDA */ s_a = update_nz(peek_zpg(0x9e));
      /* $E8A6 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $E8A8 STA */ poke_zpg(0x9e, s_a);
      /* $E8AA LDA */ s_a = update_nz(peek_zpg(0x9f));
      /* $E8AC EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $E8AE STA */ poke_zpg(0x9f, s_a);
      /* $E8B0 LDA */ s_a = update_nz(peek_zpg(0xa0));
      /* $E8B2 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $E8B4 STA */ poke_zpg(0xa0, s_a);
      /* $E8B6 LDA */ s_a = update_nz(peek_zpg(0xa1));
      /* $E8B8 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $E8BA STA */ poke_zpg(0xa1, s_a);
      /* $E8BC LDA */ s_a = update_nz(peek_zpg(0xac));
      /* $E8BE EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $E8C0 STA */ poke_zpg(0xac, s_a);
      /* $E8C2 INC */ tmp16 = 0xac, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $E8C4 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe8d4 : 0xe8c6;
      branchTarget = true;
      break;
    case 0xe8c6: // [$E8C6..$E8C9]    4 bytes
      CYCLES(0xe8c6, 7);
      /* $E8C6 INC */ tmp16 = 0xa1, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $E8C8 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe8d4 : 0xe8ca;
      branchTarget = true;
      break;
    case 0xe8ca: // [$E8CA..$E8CD]    4 bytes
      CYCLES(0xe8ca, 7);
      /* $E8CA INC */ tmp16 = 0xa0, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $E8CC BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe8d4 : 0xe8ce;
      branchTarget = true;
      break;
    case 0xe8ce: // [$E8CE..$E8D1]    4 bytes
      CYCLES(0xe8ce, 7);
      /* $E8CE INC */ tmp16 = 0x9f, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $E8D0 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe8d4 : 0xe8d2;
      branchTarget = true;
      break;
    case 0xe8d2: // [$E8D2..$E8D3]    2 bytes
      CYCLES(0xe8d2, 4);
      /* $E8D2 INC */ tmp16 = 0x9e, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
    case 0xe8d4: // [$E8D4..$E8D4]    1 bytes
      CYCLES(0xe8d4, 2);
      /* $E8D4 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe8d5: // [$E8D5..$E8D9]    5 bytes
      CYCLES(0xe8d5, 9);
      /* $E8D5 LDX */ s_x = update_nz(0x45);
      /* $E8D7 JMP */ s_pc = 0xd412;
      branchTarget = true;
      break;
    case 0xe8da: // [$E8DA..$E8DB]    2 bytes
      CYCLES(0xe8da, 4);
      /* $E8DA LDX */ s_x = update_nz(0x61);
    case 0xe8dc: // [$E8DC..$E8EF]   20 bytes
      CYCLES(0xe8dc, 35);
      /* $E8DC LDY */ s_y = update_nz(peek_zpg((uint8_t)(0x04 + s_x)));
      /* $E8DE STY */ poke_zpg(0xac, s_y);
      /* $E8E0 LDY */ s_y = update_nz(peek_zpg((uint8_t)(0x03 + s_x)));
      /* $E8E2 STY */ poke_zpg((uint8_t)(0x04 + s_x), s_y);
      /* $E8E4 LDY */ s_y = update_nz(peek_zpg((uint8_t)(0x02 + s_x)));
      /* $E8E6 STY */ poke_zpg((uint8_t)(0x03 + s_x), s_y);
      /* $E8E8 LDY */ s_y = update_nz(peek_zpg((uint8_t)(0x01 + s_x)));
      /* $E8EA STY */ poke_zpg((uint8_t)(0x02 + s_x), s_y);
      /* $E8EC LDY */ s_y = update_nz(peek_zpg(0xa4));
      /* $E8EE STY */ poke_zpg((uint8_t)(0x01 + s_x), s_y);
    case 0xe8f0: // [$E8F0..$E8F3]    4 bytes
      CYCLES(0xe8f0, 7);
      /* $E8F0 ADC */ tmp = 0x08, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E8F2 BMI */ s_pc = s_status & STATUS_N ? 0xe8dc : 0xe8f4;
      branchTarget = true;
      break;
    case 0xe8f4: // [$E8F4..$E8F5]    2 bytes
      CYCLES(0xe8f4, 4);
      /* $E8F4 BEQ */ s_pc = s_status & STATUS_Z ? 0xe8dc : 0xe8f6;
      branchTarget = true;
      break;
    case 0xe8f6: // [$E8F6..$E8FC]    7 bytes
      CYCLES(0xe8f6, 12);
      /* $E8F6 SBC */ tmp = 0x08, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $E8F8 TAY */ s_y = update_nz(s_a);
      /* $E8F9 LDA */ s_a = update_nz(peek_zpg(0xac));
      /* $E8FB BCS */ s_pc = s_status & STATUS_C ? 0xe911 : 0xe8fd;
      branchTarget = true;
      break;
    case 0xe8fd: // [$E8FD..$E900]    4 bytes
      CYCLES(0xe8fd, 7);
      /* $E8FD ASL */ s_a = update_nzc(peek_zpg((uint8_t)(0x01 + s_x)) << 1);
      /* $E8FF BCC */ s_pc = !(s_status & STATUS_C) ? 0xe903 : 0xe901;
      branchTarget = true;
      break;
    case 0xe901: // [$E901..$E902]    2 bytes
      CYCLES(0xe901, 4);
      /* $E901 INC */ tmp16 = (uint8_t)(0x01 + s_x), poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
    case 0xe903: // [$E903..$E906]    4 bytes
      CYCLES(0xe903, 7);
      /* $E903 ROR */ tmp16 = (uint8_t)(0x01 + s_x), tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7))), set_c_to_bit0(tmp);
      /* $E905 ROR */ tmp16 = (uint8_t)(0x01 + s_x), tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7))), set_c_to_bit0(tmp);
    case 0xe907: // [$E907..$E910]   10 bytes
      CYCLES(0xe907, 18);
      /* $E907 ROR */ tmp16 = (uint8_t)(0x02 + s_x), tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7))), set_c_to_bit0(tmp);
      /* $E909 ROR */ tmp16 = (uint8_t)(0x03 + s_x), tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7))), set_c_to_bit0(tmp);
      /* $E90B ROR */ tmp16 = (uint8_t)(0x04 + s_x), tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7))), set_c_to_bit0(tmp);
      /* $E90D ROR */ tmp = s_a, s_a = update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7)), set_c_to_bit0(tmp);
      /* $E90E INY */ s_y = update_nz(s_y + 1);
      /* $E90F BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe8fd : 0xe911;
      branchTarget = true;
      break;
    case 0xe911: // [$E911..$E912]    2 bytes
      CYCLES(0xe911, 4);
      /* $E911 CLC */ s_status &= ~STATUS_C;
      /* $E912 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe97f: // [$E97F..$E981]    3 bytes
      CYCLES(0xe97f, 6);
      /* $E97F JSR */ push16(0xe981), s_pc = 0xe9e3;
      branchTarget = true;
      break;
    case 0xe982: // [$E982..$E983]    2 bytes
      CYCLES(0xe982, 4);
      /* $E982 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe987 : 0xe984;
      branchTarget = true;
      break;
    case 0xe984: // [$E984..$E986]    3 bytes
      CYCLES(0xe984, 6);
      /* $E984 JMP */ s_pc = 0xe9e2;
      branchTarget = true;
      break;
    case 0xe987: // [$E987..$E989]    3 bytes
      CYCLES(0xe987, 6);
      /* $E987 JSR */ push16(0xe989), s_pc = 0xea0e;
      branchTarget = true;
      break;
    case 0xe98a: // [$E98A..$E998]   15 bytes
      CYCLES(0xe98a, 26);
      /* $E98A LDA */ s_a = update_nz(0x00);
      /* $E98C STA */ poke_zpg(0x62, s_a);
      /* $E98E STA */ poke_zpg(0x63, s_a);
      /* $E990 STA */ poke_zpg(0x64, s_a);
      /* $E992 STA */ poke_zpg(0x65, s_a);
      /* $E994 LDA */ s_a = update_nz(peek_zpg(0xac));
      /* $E996 JSR */ push16(0xe998), s_pc = 0xe9b0;
      branchTarget = true;
      break;
    case 0xe999: // [$E999..$E99D]    5 bytes
      CYCLES(0xe999, 9);
      /* $E999 LDA */ s_a = update_nz(peek_zpg(0xa1));
      /* $E99B JSR */ push16(0xe99d), s_pc = 0xe9b0;
      branchTarget = true;
      break;
    case 0xe99e: // [$E99E..$E9A2]    5 bytes
      CYCLES(0xe99e, 9);
      /* $E99E LDA */ s_a = update_nz(peek_zpg(0xa0));
      /* $E9A0 JSR */ push16(0xe9a2), s_pc = 0xe9b0;
      branchTarget = true;
      break;
    case 0xe9a3: // [$E9A3..$E9A7]    5 bytes
      CYCLES(0xe9a3, 9);
      /* $E9A3 LDA */ s_a = update_nz(peek_zpg(0x9f));
      /* $E9A5 JSR */ push16(0xe9a7), s_pc = 0xe9b0;
      branchTarget = true;
      break;
    case 0xe9a8: // [$E9A8..$E9AC]    5 bytes
      CYCLES(0xe9a8, 9);
      /* $E9A8 LDA */ s_a = update_nz(peek_zpg(0x9e));
      /* $E9AA JSR */ push16(0xe9ac), s_pc = 0xe9b5;
      branchTarget = true;
      break;
    case 0xe9ad: // [$E9AD..$E9AF]    3 bytes
      CYCLES(0xe9ad, 6);
      /* $E9AD JMP */ s_pc = 0xeae6;
      branchTarget = true;
      break;
    case 0xe9b0: // [$E9B0..$E9B1]    2 bytes
      CYCLES(0xe9b0, 4);
      /* $E9B0 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe9b5 : 0xe9b2;
      branchTarget = true;
      break;
    case 0xe9b2: // [$E9B2..$E9B4]    3 bytes
      CYCLES(0xe9b2, 6);
      /* $E9B2 JMP */ s_pc = 0xe8da;
      branchTarget = true;
      break;
    case 0xe9b5: // [$E9B5..$E9B7]    3 bytes
      CYCLES(0xe9b5, 6);
      /* $E9B5 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $E9B6 ORA */ s_a = update_nz(s_a | 0x80);
    case 0xe9b8: // [$E9B8..$E9BA]    3 bytes
      CYCLES(0xe9b8, 6);
      /* $E9B8 TAY */ s_y = update_nz(s_a);
      /* $E9B9 BCC */ s_pc = !(s_status & STATUS_C) ? 0xe9d4 : 0xe9bb;
      branchTarget = true;
      break;
    case 0xe9bb: // [$E9BB..$E9D3]   25 bytes
      CYCLES(0xe9bb, 43);
      /* $E9BB CLC */ s_status &= ~STATUS_C;
      /* $E9BC LDA */ s_a = update_nz(peek_zpg(0x65));
      /* $E9BE ADC */ tmp = peek_zpg(0xa9), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E9C0 STA */ poke_zpg(0x65, s_a);
      /* $E9C2 LDA */ s_a = update_nz(peek_zpg(0x64));
      /* $E9C4 ADC */ tmp = peek_zpg(0xa8), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E9C6 STA */ poke_zpg(0x64, s_a);
      /* $E9C8 LDA */ s_a = update_nz(peek_zpg(0x63));
      /* $E9CA ADC */ tmp = peek_zpg(0xa7), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E9CC STA */ poke_zpg(0x63, s_a);
      /* $E9CE LDA */ s_a = update_nz(peek_zpg(0x62));
      /* $E9D0 ADC */ tmp = peek_zpg(0xa6), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $E9D2 STA */ poke_zpg(0x62, s_a);
    case 0xe9d4: // [$E9D4..$E9E1]   14 bytes
      CYCLES(0xe9d4, 24);
      /* $E9D4 ROR */ tmp16 = 0x62, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7))), set_c_to_bit0(tmp);
      /* $E9D6 ROR */ tmp16 = 0x63, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7))), set_c_to_bit0(tmp);
      /* $E9D8 ROR */ tmp16 = 0x64, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7))), set_c_to_bit0(tmp);
      /* $E9DA ROR */ tmp16 = 0x65, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7))), set_c_to_bit0(tmp);
      /* $E9DC ROR */ tmp16 = 0xac, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7))), set_c_to_bit0(tmp);
      /* $E9DE TYA */ s_a = update_nz(s_y);
      /* $E9DF LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $E9E0 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xe9b8 : 0xe9e2;
      branchTarget = true;
      break;
    case 0xe9e2: // [$E9E2..$E9E2]    1 bytes
      CYCLES(0xe9e2, 2);
      /* $E9E2 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xe9e3: // [$E9E3..$EA0D]   43 bytes
      CYCLES(0xe9e3, 74);
      /* $E9E3 STA */ poke_zpg(0x5e, s_a);
      /* $E9E5 STY */ poke_zpg(0x5f, s_y);
      /* $E9E7 LDY */ s_y = update_nz(0x04);
      /* $E9E9 LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $E9EB STA */ poke_zpg(0xa9, s_a);
      /* $E9ED DEY */ s_y = update_nz(s_y - 1);
      /* $E9EE LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $E9F0 STA */ poke_zpg(0xa8, s_a);
      /* $E9F2 DEY */ s_y = update_nz(s_y - 1);
      /* $E9F3 LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $E9F5 STA */ poke_zpg(0xa7, s_a);
      /* $E9F7 DEY */ s_y = update_nz(s_y - 1);
      /* $E9F8 LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $E9FA STA */ poke_zpg(0xaa, s_a);
      /* $E9FC EOR */ s_a = update_nz(s_a ^ peek_zpg(0xa2));
      /* $E9FE STA */ poke_zpg(0xab, s_a);
      /* $EA00 LDA */ s_a = update_nz(peek_zpg(0xaa));
      /* $EA02 ORA */ s_a = update_nz(s_a | 0x80);
      /* $EA04 STA */ poke_zpg(0xa6, s_a);
      /* $EA06 DEY */ s_y = update_nz(s_y - 1);
      /* $EA07 LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $EA09 STA */ poke_zpg(0xa5, s_a);
      /* $EA0B LDA */ s_a = update_nz(peek_zpg(0x9d));
      /* $EA0D RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xea0e: // [$EA0E..$EA11]    4 bytes
      CYCLES(0xea0e, 7);
      /* $EA0E LDA */ s_a = update_nz(peek_zpg(0xa5));
      /* $EA10 BEQ */ s_pc = s_status & STATUS_Z ? 0xea31 : 0xea12;
      branchTarget = true;
      break;
    case 0xea12: // [$EA12..$EA16]    5 bytes
      CYCLES(0xea12, 9);
      /* $EA12 CLC */ s_status &= ~STATUS_C;
      /* $EA13 ADC */ tmp = peek_zpg(0x9d), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $EA15 BCC */ s_pc = !(s_status & STATUS_C) ? 0xea1b : 0xea17;
      branchTarget = true;
      break;
    case 0xea17: // [$EA17..$EA18]    2 bytes
      CYCLES(0xea17, 4);
      /* $EA17 BMI */ s_pc = s_status & STATUS_N ? 0xea36 : 0xea19;
      branchTarget = true;
      break;
    case 0xea19: // [$EA19..$EA1A]    2 bytes
      CYCLES(0xea19, 4);
      /* $EA19 CLC */ s_status &= ~STATUS_C;
      /* $EA1A BIT */ tmp = peek(0x1410), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $EA1D ADC */ tmp = 0x80, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $EA1F STA */ poke_zpg(0x9d, s_a);
      /* $EA21 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xea26 : 0xea23;
      branchTarget = true;
      break;
    case 0xea23: // [$EA23..$EA25]    3 bytes
      CYCLES(0xea23, 6);
      /* $EA23 JMP */ s_pc = 0xe852;
      branchTarget = true;
      break;
    case 0xea26: // [$EA26..$EA2A]    5 bytes
      CYCLES(0xea26, 9);
      /* $EA26 LDA */ s_a = update_nz(peek_zpg(0xab));
      /* $EA28 STA */ poke_zpg(0xa2, s_a);
      /* $EA2A RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xea31: // [$EA31..$EA35]    5 bytes
      CYCLES(0xea31, 9);
      /* $EA31 PLA */ s_a = update_nz(pop8());
      /* $EA32 PLA */ s_a = update_nz(pop8());
      /* $EA33 JMP */ s_pc = 0xe84e;
      branchTarget = true;
      break;
    case 0xea36: // [$EA36..$EA38]    3 bytes
      CYCLES(0xea36, 6);
      /* $EA36 JMP */ s_pc = 0xe8d5;
      branchTarget = true;
      break;
    case 0xea39: // [$EA39..$EA3B]    3 bytes
      CYCLES(0xea39, 6);
      /* $EA39 JSR */ push16(0xea3b), s_pc = 0xeb63;
      branchTarget = true;
      break;
    case 0xea3c: // [$EA3C..$EA3E]    3 bytes
      CYCLES(0xea3c, 6);
      /* $EA3C TAX */ s_x = update_nz(s_a);
      /* $EA3D BEQ */ s_pc = s_status & STATUS_Z ? 0xea4f : 0xea3f;
      branchTarget = true;
      break;
    case 0xea3f: // [$EA3F..$EA43]    5 bytes
      CYCLES(0xea3f, 9);
      /* $EA3F CLC */ s_status &= ~STATUS_C;
      /* $EA40 ADC */ tmp = 0x02, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $EA42 BCS */ s_pc = s_status & STATUS_C ? 0xea36 : 0xea44;
      branchTarget = true;
      break;
    case 0xea44: // [$EA44..$EA4A]    7 bytes
      CYCLES(0xea44, 12);
      /* $EA44 LDX */ s_x = update_nz(0x00);
      /* $EA46 STX */ poke_zpg(0xab, s_x);
      /* $EA48 JSR */ push16(0xea4a), s_pc = 0xe7ce;
      branchTarget = true;
      break;
    case 0xea4b: // [$EA4B..$EA4E]    4 bytes
      CYCLES(0xea4b, 7);
      /* $EA4B INC */ tmp16 = 0x9d, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $EA4D BEQ */ s_pc = s_status & STATUS_Z ? 0xea36 : 0xea4f;
      branchTarget = true;
      break;
    case 0xea4f: // [$EA4F..$EA4F]    1 bytes
      CYCLES(0xea4f, 2);
      /* $EA4F RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xea55: // [$EA55..$EA57]    3 bytes
      CYCLES(0xea55, 6);
      /* $EA55 JSR */ push16(0xea57), s_pc = 0xeb63;
      branchTarget = true;
      break;
    case 0xea58: // [$EA58..$EA62]   11 bytes
      CYCLES(0xea58, 19);
      /* $EA58 LDA */ s_a = update_nz(0x50);
      /* $EA5A LDY */ s_y = update_nz(0xea);
      /* $EA5C LDX */ s_x = update_nz(0x00);
      /* $EA5E STX */ poke_zpg(0xab, s_x);
      /* $EA60 JSR */ push16(0xea62), s_pc = 0xeaf9;
      branchTarget = true;
      break;
    case 0xea63: // [$EA63..$EA65]    3 bytes
      CYCLES(0xea63, 6);
      /* $EA63 JMP */ s_pc = 0xea69;
      branchTarget = true;
      break;
    case 0xea69: // [$EA69..$EA6A]    2 bytes
      CYCLES(0xea69, 4);
      /* $EA69 BEQ */ s_pc = s_status & STATUS_Z ? 0xeae1 : 0xea6b;
      branchTarget = true;
      break;
    case 0xea6b: // [$EA6B..$EA6D]    3 bytes
      CYCLES(0xea6b, 6);
      /* $EA6B JSR */ push16(0xea6d), s_pc = 0xeb72;
      branchTarget = true;
      break;
    case 0xea6e: // [$EA6E..$EA77]   10 bytes
      CYCLES(0xea6e, 18);
      /* $EA6E LDA */ s_a = update_nz(0x00);
      /* $EA70 SEC */ s_status |= STATUS_C;
      /* $EA71 SBC */ tmp = peek_zpg(0x9d), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $EA73 STA */ poke_zpg(0x9d, s_a);
      /* $EA75 JSR */ push16(0xea77), s_pc = 0xea0e;
      branchTarget = true;
      break;
    case 0xea78: // [$EA78..$EA7B]    4 bytes
      CYCLES(0xea78, 7);
      /* $EA78 INC */ tmp16 = 0x9d, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $EA7A BEQ */ s_pc = s_status & STATUS_Z ? 0xea36 : 0xea7c;
      branchTarget = true;
      break;
    case 0xea7c: // [$EA7C..$EA7F]    4 bytes
      CYCLES(0xea7c, 7);
      /* $EA7C LDX */ s_x = update_nz(0xfc);
      /* $EA7E LDA */ s_a = update_nz(0x01);
    case 0xea80: // [$EA80..$EA85]    6 bytes
      CYCLES(0xea80, 11);
      /* $EA80 LDY */ s_y = update_nz(peek_zpg(0xa6));
      /* $EA82 CPY */ update_nz_inv_c(s_y - peek_zpg(0x9e));
      /* $EA84 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xea96 : 0xea86;
      branchTarget = true;
      break;
    case 0xea86: // [$EA86..$EA8B]    6 bytes
      CYCLES(0xea86, 11);
      /* $EA86 LDY */ s_y = update_nz(peek_zpg(0xa7));
      /* $EA88 CPY */ update_nz_inv_c(s_y - peek_zpg(0x9f));
      /* $EA8A BNE */ s_pc = !(s_status & STATUS_Z) ? 0xea96 : 0xea8c;
      branchTarget = true;
      break;
    case 0xea8c: // [$EA8C..$EA91]    6 bytes
      CYCLES(0xea8c, 11);
      /* $EA8C LDY */ s_y = update_nz(peek_zpg(0xa8));
      /* $EA8E CPY */ update_nz_inv_c(s_y - peek_zpg(0xa0));
      /* $EA90 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xea96 : 0xea92;
      branchTarget = true;
      break;
    case 0xea92: // [$EA92..$EA95]    4 bytes
      CYCLES(0xea92, 7);
      /* $EA92 LDY */ s_y = update_nz(peek_zpg(0xa9));
      /* $EA94 CPY */ update_nz_inv_c(s_y - peek_zpg(0xa1));
    case 0xea96: // [$EA96..$EA99]    4 bytes
      CYCLES(0xea96, 7);
      /* $EA96 PHP */ push8(s_status | STATUS_B);
      /* $EA97 ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $EA98 BCC */ s_pc = !(s_status & STATUS_C) ? 0xeaa3 : 0xea9a;
      branchTarget = true;
      break;
    case 0xea9a: // [$EA9A..$EA9E]    5 bytes
      CYCLES(0xea9a, 9);
      /* $EA9A INX */ s_x = update_nz(s_x + 1);
      /* $EA9B STA */ poke_zpg((uint8_t)(0x65 + s_x), s_a);
      /* $EA9D BEQ */ s_pc = s_status & STATUS_Z ? 0xead1 : 0xea9f;
      branchTarget = true;
      break;
    case 0xea9f: // [$EA9F..$EAA0]    2 bytes
      CYCLES(0xea9f, 4);
      /* $EA9F BPL */ s_pc = !(s_status & STATUS_N) ? 0xead5 : 0xeaa1;
      branchTarget = true;
      break;
    case 0xeaa1: // [$EAA1..$EAA2]    2 bytes
      CYCLES(0xeaa1, 4);
      /* $EAA1 LDA */ s_a = update_nz(0x01);
    case 0xeaa3: // [$EAA3..$EAA5]    3 bytes
      CYCLES(0xeaa3, 6);
      /* $EAA3 PLP */ s_status = pop8() & ~STATUS_B;
      /* $EAA4 BCS */ s_pc = s_status & STATUS_C ? 0xeab4 : 0xeaa6;
      branchTarget = true;
      break;
    case 0xeaa6: // [$EAA6..$EAAF]   10 bytes
      CYCLES(0xeaa6, 18);
      /* $EAA6 ASL */ s_a = update_nzc(peek_zpg(0xa9) << 1);
      /* $EAA8 ROL */ tmp16 = 0xa8, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $EAAA ROL */ tmp16 = 0xa7, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $EAAC ROL */ tmp16 = 0xa6, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp << 1) | (s_status & STATUS_C))), set_c_to_bit0(tmp >> 7);
      /* $EAAE BCS */ s_pc = s_status & STATUS_C ? 0xea96 : 0xeab0;
      branchTarget = true;
      break;
    case 0xeab0: // [$EAB0..$EAB1]    2 bytes
      CYCLES(0xeab0, 4);
      /* $EAB0 BMI */ s_pc = s_status & STATUS_N ? 0xea80 : 0xeab2;
      branchTarget = true;
      break;
    case 0xeab2: // [$EAB2..$EAB3]    2 bytes
      CYCLES(0xeab2, 4);
      /* $EAB2 BPL */ s_pc = !(s_status & STATUS_N) ? 0xea96 : 0xeab4;
      branchTarget = true;
      break;
    case 0xeab4: // [$EAB4..$EAD0]   29 bytes
      CYCLES(0xeab4, 50);
      /* $EAB4 TAY */ s_y = update_nz(s_a);
      /* $EAB5 LDA */ s_a = update_nz(peek_zpg(0xa9));
      /* $EAB7 SBC */ tmp = peek_zpg(0xa1), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $EAB9 STA */ poke_zpg(0xa9, s_a);
      /* $EABB LDA */ s_a = update_nz(peek_zpg(0xa8));
      /* $EABD SBC */ tmp = peek_zpg(0xa0), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $EABF STA */ poke_zpg(0xa8, s_a);
      /* $EAC1 LDA */ s_a = update_nz(peek_zpg(0xa7));
      /* $EAC3 SBC */ tmp = peek_zpg(0x9f), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $EAC5 STA */ poke_zpg(0xa7, s_a);
      /* $EAC7 LDA */ s_a = update_nz(peek_zpg(0xa6));
      /* $EAC9 SBC */ tmp = peek_zpg(0x9e), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $EACB STA */ poke_zpg(0xa6, s_a);
      /* $EACD TYA */ s_a = update_nz(s_y);
      /* $EACE JMP */ s_pc = 0xeaa6;
      branchTarget = true;
      break;
    case 0xead1: // [$EAD1..$EAD4]    4 bytes
      CYCLES(0xead1, 7);
      /* $EAD1 LDA */ s_a = update_nz(0x40);
      /* $EAD3 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xeaa3 : 0xead5;
      branchTarget = true;
      break;
    case 0xead5: // [$EAD5..$EAE0]   12 bytes
      CYCLES(0xead5, 21);
      /* $EAD5 ASL */ s_a = update_nzc(s_a << 1);
      /* $EAD6 ASL */ s_a = update_nzc(s_a << 1);
      /* $EAD7 ASL */ s_a = update_nzc(s_a << 1);
      /* $EAD8 ASL */ s_a = update_nzc(s_a << 1);
      /* $EAD9 ASL */ s_a = update_nzc(s_a << 1);
      /* $EADA ASL */ s_a = update_nzc(s_a << 1);
      /* $EADB STA */ poke_zpg(0xac, s_a);
      /* $EADD PLP */ s_status = pop8() & ~STATUS_B;
      /* $EADE JMP */ s_pc = 0xeae6;
      branchTarget = true;
      break;
    case 0xeae1: // [$EAE1..$EAE5]    5 bytes
      CYCLES(0xeae1, 9);
      /* $EAE1 LDX */ s_x = update_nz(0x85);
      /* $EAE3 JMP */ s_pc = 0xd412;
      branchTarget = true;
      break;
    case 0xeae6: // [$EAE6..$EAF8]   19 bytes
      CYCLES(0xeae6, 33);
      /* $EAE6 LDA */ s_a = update_nz(peek_zpg(0x62));
      /* $EAE8 STA */ poke_zpg(0x9e, s_a);
      /* $EAEA LDA */ s_a = update_nz(peek_zpg(0x63));
      /* $EAEC STA */ poke_zpg(0x9f, s_a);
      /* $EAEE LDA */ s_a = update_nz(peek_zpg(0x64));
      /* $EAF0 STA */ poke_zpg(0xa0, s_a);
      /* $EAF2 LDA */ s_a = update_nz(peek_zpg(0x65));
      /* $EAF4 STA */ poke_zpg(0xa1, s_a);
      /* $EAF6 JMP */ s_pc = 0xe82e;
      branchTarget = true;
      break;
    case 0xeaf9: // [$EAF9..$EB1D]   37 bytes
      CYCLES(0xeaf9, 63);
      /* $EAF9 STA */ poke_zpg(0x5e, s_a);
      /* $EAFB STY */ poke_zpg(0x5f, s_y);
      /* $EAFD LDY */ s_y = update_nz(0x04);
      /* $EAFF LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $EB01 STA */ poke_zpg(0xa1, s_a);
      /* $EB03 DEY */ s_y = update_nz(s_y - 1);
      /* $EB04 LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $EB06 STA */ poke_zpg(0xa0, s_a);
      /* $EB08 DEY */ s_y = update_nz(s_y - 1);
      /* $EB09 LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $EB0B STA */ poke_zpg(0x9f, s_a);
      /* $EB0D DEY */ s_y = update_nz(s_y - 1);
      /* $EB0E LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $EB10 STA */ poke_zpg(0xa2, s_a);
      /* $EB12 ORA */ s_a = update_nz(s_a | 0x80);
      /* $EB14 STA */ poke_zpg(0x9e, s_a);
      /* $EB16 DEY */ s_y = update_nz(s_y - 1);
      /* $EB17 LDA */ s_a = update_nz(peek(peek16_zpg(0x5e) + s_y));
      /* $EB19 STA */ poke_zpg(0x9d, s_a);
      /* $EB1B STY */ poke_zpg(0xac, s_y);
      /* $EB1D RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xeb27: // [$EB27..$EB2A]    4 bytes
      CYCLES(0xeb27, 7);
      /* $EB27 LDX */ s_x = update_nz(peek_zpg(0x85));
      /* $EB29 LDY */ s_y = update_nz(peek_zpg(0x86));
    case 0xeb2b: // [$EB2B..$EB2D]    3 bytes
      CYCLES(0xeb2b, 6);
      /* $EB2B JSR */ push16(0xeb2d), s_pc = 0xeb72;
      branchTarget = true;
      break;
    case 0xeb2e: // [$EB2E..$EB52]   37 bytes
      CYCLES(0xeb2e, 63);
      /* $EB2E STX */ poke_zpg(0x5e, s_x);
      /* $EB30 STY */ poke_zpg(0x5f, s_y);
      /* $EB32 LDY */ s_y = update_nz(0x04);
      /* $EB34 LDA */ s_a = update_nz(peek_zpg(0xa1));
      /* $EB36 STA */ poke(peek16_zpg(0x5e) + s_y, s_a);
      /* $EB38 DEY */ s_y = update_nz(s_y - 1);
      /* $EB39 LDA */ s_a = update_nz(peek_zpg(0xa0));
      /* $EB3B STA */ poke(peek16_zpg(0x5e) + s_y, s_a);
      /* $EB3D DEY */ s_y = update_nz(s_y - 1);
      /* $EB3E LDA */ s_a = update_nz(peek_zpg(0x9f));
      /* $EB40 STA */ poke(peek16_zpg(0x5e) + s_y, s_a);
      /* $EB42 DEY */ s_y = update_nz(s_y - 1);
      /* $EB43 LDA */ s_a = update_nz(peek_zpg(0xa2));
      /* $EB45 ORA */ s_a = update_nz(s_a | 0x7f);
      /* $EB47 AND */ s_a = update_nz(s_a & peek_zpg(0x9e));
      /* $EB49 STA */ poke(peek16_zpg(0x5e) + s_y, s_a);
      /* $EB4B DEY */ s_y = update_nz(s_y - 1);
      /* $EB4C LDA */ s_a = update_nz(peek_zpg(0x9d));
      /* $EB4E STA */ poke(peek16_zpg(0x5e) + s_y, s_a);
      /* $EB50 STY */ poke_zpg(0xac, s_y);
      /* $EB52 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xeb53: // [$EB53..$EB58]    6 bytes
      CYCLES(0xeb53, 11);
      /* $EB53 LDA */ s_a = update_nz(peek_zpg(0xaa));
      /* $EB55 STA */ poke_zpg(0xa2, s_a);
      /* $EB57 LDX */ s_x = update_nz(0x05);
    case 0xeb59: // [$EB59..$EB5F]    7 bytes
      CYCLES(0xeb59, 12);
      /* $EB59 LDA */ s_a = update_nz(peek_zpg((uint8_t)(0xa4 + s_x)));
      /* $EB5B STA */ poke_zpg((uint8_t)(0x9c + s_x), s_a);
      /* $EB5D DEX */ s_x = update_nz(s_x - 1);
      /* $EB5E BNE */ s_pc = !(s_status & STATUS_Z) ? 0xeb59 : 0xeb60;
      branchTarget = true;
      break;
    case 0xeb60: // [$EB60..$EB62]    3 bytes
      CYCLES(0xeb60, 6);
      /* $EB60 STX */ poke_zpg(0xac, s_x);
      /* $EB62 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xeb63: // [$EB63..$EB65]    3 bytes
      CYCLES(0xeb63, 6);
      /* $EB63 JSR */ push16(0xeb65), s_pc = 0xeb72;
      branchTarget = true;
      break;
    case 0xeb66: // [$EB66..$EB67]    2 bytes
      CYCLES(0xeb66, 4);
      /* $EB66 LDX */ s_x = update_nz(0x06);
    case 0xeb68: // [$EB68..$EB6E]    7 bytes
      CYCLES(0xeb68, 12);
      /* $EB68 LDA */ s_a = update_nz(peek_zpg((uint8_t)(0x9c + s_x)));
      /* $EB6A STA */ poke_zpg((uint8_t)(0xa4 + s_x), s_a);
      /* $EB6C DEX */ s_x = update_nz(s_x - 1);
      /* $EB6D BNE */ s_pc = !(s_status & STATUS_Z) ? 0xeb68 : 0xeb6f;
      branchTarget = true;
      break;
    case 0xeb6f: // [$EB6F..$EB70]    2 bytes
      CYCLES(0xeb6f, 4);
      /* $EB6F STX */ poke_zpg(0xac, s_x);
    case 0xeb71: // [$EB71..$EB71]    1 bytes
      CYCLES(0xeb71, 2);
      /* $EB71 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xeb72: // [$EB72..$EB75]    4 bytes
      CYCLES(0xeb72, 7);
      /* $EB72 LDA */ s_a = update_nz(peek_zpg(0x9d));
      /* $EB74 BEQ */ s_pc = s_status & STATUS_Z ? 0xeb71 : 0xeb76;
      branchTarget = true;
      break;
    case 0xeb76: // [$EB76..$EB79]    4 bytes
      CYCLES(0xeb76, 7);
      /* $EB76 ASL */ s_a = update_nzc(peek_zpg(0xac) << 1);
      /* $EB78 BCC */ s_pc = !(s_status & STATUS_C) ? 0xeb71 : 0xeb7a;
      branchTarget = true;
      break;
    case 0xeb7a: // [$EB7A..$EB7C]    3 bytes
      CYCLES(0xeb7a, 6);
      /* $EB7A JSR */ push16(0xeb7c), s_pc = 0xe8c6;
      branchTarget = true;
      break;
    case 0xeb7d: // [$EB7D..$EB7E]    2 bytes
      CYCLES(0xeb7d, 4);
      /* $EB7D BNE */ s_pc = !(s_status & STATUS_Z) ? 0xeb71 : 0xeb7f;
      branchTarget = true;
      break;
    case 0xeb7f: // [$EB7F..$EB81]    3 bytes
      CYCLES(0xeb7f, 6);
      /* $EB7F JMP */ s_pc = 0xe88f;
      branchTarget = true;
      break;
    case 0xeb82: // [$EB82..$EB85]    4 bytes
      CYCLES(0xeb82, 7);
      /* $EB82 LDA */ s_a = update_nz(peek_zpg(0x9d));
      /* $EB84 BEQ */ s_pc = s_status & STATUS_Z ? 0xeb8f : 0xeb86;
      branchTarget = true;
      break;
    case 0xeb86: // [$EB86..$EB87]    2 bytes
      CYCLES(0xeb86, 4);
      /* $EB86 LDA */ s_a = update_nz(peek_zpg(0xa2));
    case 0xeb88: // [$EB88..$EB8C]    5 bytes
      CYCLES(0xeb88, 9);
      /* $EB88 ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
      /* $EB89 LDA */ s_a = update_nz(0xff);
      /* $EB8B BCS */ s_pc = s_status & STATUS_C ? 0xeb8f : 0xeb8d;
      branchTarget = true;
      break;
    case 0xeb8d: // [$EB8D..$EB8E]    2 bytes
      CYCLES(0xeb8d, 4);
      /* $EB8D LDA */ s_a = update_nz(0x01);
    case 0xeb8f: // [$EB8F..$EB8F]    1 bytes
      CYCLES(0xeb8f, 2);
      /* $EB8F RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xeb93: // [$EB93..$EB9A]    8 bytes
      CYCLES(0xeb93, 14);
      /* $EB93 STA */ poke_zpg(0x9e, s_a);
      /* $EB95 LDA */ s_a = update_nz(0x00);
      /* $EB97 STA */ poke_zpg(0x9f, s_a);
      /* $EB99 LDX */ s_x = update_nz(0x88);
    case 0xeb9b: // [$EB9B..$EB9F]    5 bytes
      CYCLES(0xeb9b, 9);
      /* $EB9B LDA */ s_a = update_nz(peek_zpg(0x9e));
      /* $EB9D EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $EB9F ROL */ tmp = s_a, s_a = update_nz((tmp << 1) | (s_status & STATUS_C)), set_c_to_bit0(tmp >> 7);
    case 0xeba0: // [$EBA0..$EBAE]   15 bytes
      CYCLES(0xeba0, 26);
      /* $EBA0 LDA */ s_a = update_nz(0x00);
      /* $EBA2 STA */ poke_zpg(0xa1, s_a);
      /* $EBA4 STA */ poke_zpg(0xa0, s_a);
      /* $EBA6 STX */ poke_zpg(0x9d, s_x);
      /* $EBA8 STA */ poke_zpg(0xac, s_a);
      /* $EBAA STA */ poke_zpg(0xa2, s_a);
      /* $EBAC JMP */ s_pc = 0xe829;
      branchTarget = true;
      break;
    case 0xebb2: // [$EBB2..$EBBD]   12 bytes
      CYCLES(0xebb2, 21);
      /* $EBB2 STA */ poke_zpg(0x60, s_a);
      /* $EBB4 STY */ poke_zpg(0x61, s_y);
      /* $EBB6 LDY */ s_y = update_nz(0x00);
      /* $EBB8 LDA */ s_a = update_nz(peek(peek16_zpg(0x60) + s_y));
      /* $EBBA INY */ s_y = update_nz(s_y + 1);
      /* $EBBB TAX */ s_x = update_nz(s_a);
      /* $EBBC BEQ */ s_pc = s_status & STATUS_Z ? 0xeb82 : 0xebbe;
      branchTarget = true;
      break;
    case 0xebbe: // [$EBBE..$EBC3]    6 bytes
      CYCLES(0xebbe, 11);
      /* $EBBE LDA */ s_a = update_nz(peek(peek16_zpg(0x60) + s_y));
      /* $EBC0 EOR */ s_a = update_nz(s_a ^ peek_zpg(0xa2));
      /* $EBC2 BMI */ s_pc = s_status & STATUS_N ? 0xeb86 : 0xebc4;
      branchTarget = true;
      break;
    case 0xebc4: // [$EBC4..$EBC7]    4 bytes
      CYCLES(0xebc4, 7);
      /* $EBC4 CPX */ update_nz_inv_c(s_x - peek_zpg(0x9d));
      /* $EBC6 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xebe9 : 0xebc8;
      branchTarget = true;
      break;
    case 0xebc8: // [$EBC8..$EBCF]    8 bytes
      CYCLES(0xebc8, 14);
      /* $EBC8 LDA */ s_a = update_nz(peek(peek16_zpg(0x60) + s_y));
      /* $EBCA ORA */ s_a = update_nz(s_a | 0x80);
      /* $EBCC CMP */ update_nz_inv_c(s_a - peek_zpg(0x9e));
      /* $EBCE BNE */ s_pc = !(s_status & STATUS_Z) ? 0xebe9 : 0xebd0;
      branchTarget = true;
      break;
    case 0xebd0: // [$EBD0..$EBD6]    7 bytes
      CYCLES(0xebd0, 12);
      /* $EBD0 INY */ s_y = update_nz(s_y + 1);
      /* $EBD1 LDA */ s_a = update_nz(peek(peek16_zpg(0x60) + s_y));
      /* $EBD3 CMP */ update_nz_inv_c(s_a - peek_zpg(0x9f));
      /* $EBD5 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xebe9 : 0xebd7;
      branchTarget = true;
      break;
    case 0xebd7: // [$EBD7..$EBDD]    7 bytes
      CYCLES(0xebd7, 12);
      /* $EBD7 INY */ s_y = update_nz(s_y + 1);
      /* $EBD8 LDA */ s_a = update_nz(peek(peek16_zpg(0x60) + s_y));
      /* $EBDA CMP */ update_nz_inv_c(s_a - peek_zpg(0xa0));
      /* $EBDC BNE */ s_pc = !(s_status & STATUS_Z) ? 0xebe9 : 0xebde;
      branchTarget = true;
      break;
    case 0xebde: // [$EBDE..$EBE8]   11 bytes
      CYCLES(0xebde, 19);
      /* $EBDE INY */ s_y = update_nz(s_y + 1);
      /* $EBDF LDA */ s_a = update_nz(0x7f);
      /* $EBE1 CMP */ update_nz_inv_c(s_a - peek_zpg(0xac));
      /* $EBE3 LDA */ s_a = update_nz(peek(peek16_zpg(0x60) + s_y));
      /* $EBE5 SBC */ tmp = peek_zpg(0xa1), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $EBE7 BEQ */ s_pc = s_status & STATUS_Z ? 0xec11 : 0xebe9;
      branchTarget = true;
      break;
    case 0xebe9: // [$EBE9..$EBEC]    4 bytes
      CYCLES(0xebe9, 7);
      /* $EBE9 LDA */ s_a = update_nz(peek_zpg(0xa2));
      /* $EBEB BCC */ s_pc = !(s_status & STATUS_C) ? 0xebef : 0xebed;
      branchTarget = true;
      break;
    case 0xebed: // [$EBED..$EBEE]    2 bytes
      CYCLES(0xebed, 4);
      /* $EBED EOR */ s_a = update_nz(s_a ^ 0xff);
    case 0xebef: // [$EBEF..$EBF1]    3 bytes
      CYCLES(0xebef, 6);
      /* $EBEF JMP */ s_pc = 0xeb88;
      branchTarget = true;
      break;
    case 0xebf2: // [$EBF2..$EBF5]    4 bytes
      CYCLES(0xebf2, 7);
      /* $EBF2 LDA */ s_a = update_nz(peek_zpg(0x9d));
      /* $EBF4 BEQ */ s_pc = s_status & STATUS_Z ? 0xec40 : 0xebf6;
      branchTarget = true;
      break;
    case 0xebf6: // [$EBF6..$EBFC]    7 bytes
      CYCLES(0xebf6, 12);
      /* $EBF6 SEC */ s_status |= STATUS_C;
      /* $EBF7 SBC */ tmp = 0xa0, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $EBF9 BIT */ tmp = peek_zpg(0xa2), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $EBFB BPL */ s_pc = !(s_status & STATUS_N) ? 0xec06 : 0xebfd;
      branchTarget = true;
      break;
    case 0xebfd: // [$EBFD..$EC04]    8 bytes
      CYCLES(0xebfd, 14);
      /* $EBFD TAX */ s_x = update_nz(s_a);
      /* $EBFE LDA */ s_a = update_nz(0xff);
      /* $EC00 STA */ poke_zpg(0xa4, s_a);
      /* $EC02 JSR */ push16(0xec04), s_pc = 0xe8a4;
      branchTarget = true;
      break;
    case 0xec05: // [$EC05..$EC05]    1 bytes
      CYCLES(0xec05, 2);
      /* $EC05 TXA */ s_a = update_nz(s_x);
    case 0xec06: // [$EC06..$EC0B]    6 bytes
      CYCLES(0xec06, 11);
      /* $EC06 LDX */ s_x = update_nz(0x9d);
      /* $EC08 CMP */ update_nz_inv_c(s_a - 0xf9);
      /* $EC0A BPL */ s_pc = !(s_status & STATUS_N) ? 0xec12 : 0xec0c;
      branchTarget = true;
      break;
    case 0xec0c: // [$EC0C..$EC0E]    3 bytes
      CYCLES(0xec0c, 6);
      /* $EC0C JSR */ push16(0xec0e), s_pc = 0xe8f0;
      branchTarget = true;
      break;
    case 0xec0f: // [$EC0F..$EC10]    2 bytes
      CYCLES(0xec0f, 4);
      /* $EC0F STY */ poke_zpg(0xa4, s_y);
    case 0xec11: // [$EC11..$EC11]    1 bytes
      CYCLES(0xec11, 2);
      /* $EC11 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xec12: // [$EC12..$EC1F]   14 bytes
      CYCLES(0xec12, 24);
      /* $EC12 TAY */ s_y = update_nz(s_a);
      /* $EC13 LDA */ s_a = update_nz(peek_zpg(0xa2));
      /* $EC15 AND */ s_a = update_nz(s_a & 0x80);
      /* $EC17 LSR */ tmp16 = 0x9e, tmp = peek_zpg(tmp16), set_c_to_bit0(tmp), poke_zpg(tmp16, update_nz(tmp >> 1));
      /* $EC19 ORA */ s_a = update_nz(s_a | peek_zpg(0x9e));
      /* $EC1B STA */ poke_zpg(0x9e, s_a);
      /* $EC1D JSR */ push16(0xec1f), s_pc = 0xe907;
      branchTarget = true;
      break;
    case 0xec20: // [$EC20..$EC22]    3 bytes
      CYCLES(0xec20, 6);
      /* $EC20 STY */ poke_zpg(0xa4, s_y);
      /* $EC22 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xec40: // [$EC40..$EC49]   10 bytes
      CYCLES(0xec40, 18);
      /* $EC40 STA */ poke_zpg(0x9e, s_a);
      /* $EC42 STA */ poke_zpg(0x9f, s_a);
      /* $EC44 STA */ poke_zpg(0xa0, s_a);
      /* $EC46 STA */ poke_zpg(0xa1, s_a);
      /* $EC48 TAY */ s_y = update_nz(s_a);
      /* $EC49 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xec4a: // [$EC4A..$EC4D]    4 bytes
      CYCLES(0xec4a, 7);
      /* $EC4A LDY */ s_y = update_nz(0x00);
      /* $EC4C LDX */ s_x = update_nz(0x0a);
    case 0xec4e: // [$EC4E..$EC52]    5 bytes
      CYCLES(0xec4e, 9);
      /* $EC4E STY */ poke_zpg((uint8_t)(0x99 + s_x), s_y);
      /* $EC50 DEX */ s_x = update_nz(s_x - 1);
      /* $EC51 BPL */ s_pc = !(s_status & STATUS_N) ? 0xec4e : 0xec53;
      branchTarget = true;
      break;
    case 0xec53: // [$EC53..$EC54]    2 bytes
      CYCLES(0xec53, 4);
      /* $EC53 BCC */ s_pc = !(s_status & STATUS_C) ? 0xec64 : 0xec55;
      branchTarget = true;
      break;
    case 0xec55: // [$EC55..$EC58]    4 bytes
      CYCLES(0xec55, 7);
      /* $EC55 CMP */ update_nz_inv_c(s_a - 0x2d);
      /* $EC57 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xec5d : 0xec59;
      branchTarget = true;
      break;
    case 0xec59: // [$EC59..$EC5C]    4 bytes
      CYCLES(0xec59, 7);
      /* $EC59 STX */ poke_zpg(0xa3, s_x);
      /* $EC5B BEQ */ s_pc = s_status & STATUS_Z ? 0xec61 : 0xec5d;
      branchTarget = true;
      break;
    case 0xec5d: // [$EC5D..$EC60]    4 bytes
      CYCLES(0xec5d, 7);
      /* $EC5D CMP */ update_nz_inv_c(s_a - 0x2b);
      /* $EC5F BNE */ s_pc = !(s_status & STATUS_Z) ? 0xec66 : 0xec61;
      branchTarget = true;
      break;
    case 0xec61: // [$EC61..$EC63]    3 bytes
      CYCLES(0xec61, 6);
      /* $EC61 JSR */ push16(0xec63), s_pc = 0x00b1;
      branchTarget = true;
      break;
    case 0xec64: // [$EC64..$EC65]    2 bytes
      CYCLES(0xec64, 4);
      /* $EC64 BCC */ s_pc = !(s_status & STATUS_C) ? 0xecc1 : 0xec66;
      branchTarget = true;
      break;
    case 0xec66: // [$EC66..$EC69]    4 bytes
      CYCLES(0xec66, 7);
      /* $EC66 CMP */ update_nz_inv_c(s_a - 0x2e);
      /* $EC68 BEQ */ s_pc = s_status & STATUS_Z ? 0xec98 : 0xec6a;
      branchTarget = true;
      break;
    case 0xec6a: // [$EC6A..$EC6D]    4 bytes
      CYCLES(0xec6a, 7);
      /* $EC6A CMP */ update_nz_inv_c(s_a - 0x45);
      /* $EC6C BNE */ s_pc = !(s_status & STATUS_Z) ? 0xec9e : 0xec6e;
      branchTarget = true;
      break;
    case 0xec6e: // [$EC6E..$EC70]    3 bytes
      CYCLES(0xec6e, 6);
      /* $EC6E JSR */ push16(0xec70), s_pc = 0x00b1;
      branchTarget = true;
      break;
    case 0xec71: // [$EC71..$EC72]    2 bytes
      CYCLES(0xec71, 4);
      /* $EC71 BCC */ s_pc = !(s_status & STATUS_C) ? 0xec8a : 0xec73;
      branchTarget = true;
      break;
    case 0xec73: // [$EC73..$EC76]    4 bytes
      CYCLES(0xec73, 7);
      /* $EC73 CMP */ update_nz_inv_c(s_a - 0xc9);
      /* $EC75 BEQ */ s_pc = s_status & STATUS_Z ? 0xec85 : 0xec77;
      branchTarget = true;
      break;
    case 0xec77: // [$EC77..$EC7A]    4 bytes
      CYCLES(0xec77, 7);
      /* $EC77 CMP */ update_nz_inv_c(s_a - 0x2d);
      /* $EC79 BEQ */ s_pc = s_status & STATUS_Z ? 0xec85 : 0xec7b;
      branchTarget = true;
      break;
    case 0xec7b: // [$EC7B..$EC7E]    4 bytes
      CYCLES(0xec7b, 7);
      /* $EC7B CMP */ update_nz_inv_c(s_a - 0xc8);
      /* $EC7D BEQ */ s_pc = s_status & STATUS_Z ? 0xec87 : 0xec7f;
      branchTarget = true;
      break;
    case 0xec7f: // [$EC7F..$EC82]    4 bytes
      CYCLES(0xec7f, 7);
      /* $EC7F CMP */ update_nz_inv_c(s_a - 0x2b);
      /* $EC81 BEQ */ s_pc = s_status & STATUS_Z ? 0xec87 : 0xec83;
      branchTarget = true;
      break;
    case 0xec83: // [$EC83..$EC84]    2 bytes
      CYCLES(0xec83, 4);
      /* $EC83 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xec8c : 0xec85;
      branchTarget = true;
      break;
    case 0xec85: // [$EC85..$EC86]    2 bytes
      CYCLES(0xec85, 4);
      /* $EC85 ROR */ tmp16 = 0x9c, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7))), set_c_to_bit0(tmp);
    case 0xec87: // [$EC87..$EC89]    3 bytes
      CYCLES(0xec87, 6);
      /* $EC87 JSR */ push16(0xec89), s_pc = 0x00b1;
      branchTarget = true;
      break;
    case 0xec8a: // [$EC8A..$EC8B]    2 bytes
      CYCLES(0xec8a, 4);
      /* $EC8A BCC */ s_pc = !(s_status & STATUS_C) ? 0xece8 : 0xec8c;
      branchTarget = true;
      break;
    case 0xec8c: // [$EC8C..$EC8F]    4 bytes
      CYCLES(0xec8c, 7);
      /* $EC8C BIT */ tmp = peek_zpg(0x9c), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $EC8E BPL */ s_pc = !(s_status & STATUS_N) ? 0xec9e : 0xec90;
      branchTarget = true;
      break;
    case 0xec90: // [$EC90..$EC97]    8 bytes
      CYCLES(0xec90, 14);
      /* $EC90 LDA */ s_a = update_nz(0x00);
      /* $EC92 SEC */ s_status |= STATUS_C;
      /* $EC93 SBC */ tmp = peek_zpg(0x9a), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $EC95 JMP */ s_pc = 0xeca0;
      branchTarget = true;
      break;
    case 0xec98: // [$EC98..$EC9D]    6 bytes
      CYCLES(0xec98, 11);
      /* $EC98 ROR */ tmp16 = 0x9b, tmp = peek_zpg(tmp16), poke_zpg(tmp16, update_nz((tmp >> 1) | ((s_status & STATUS_C) << 7))), set_c_to_bit0(tmp);
      /* $EC9A BIT */ tmp = peek_zpg(0x9b), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $EC9C BVC */ s_pc = !(s_status & STATUS_V) ? 0xec61 : 0xec9e;
      branchTarget = true;
      break;
    case 0xec9e: // [$EC9E..$EC9F]    2 bytes
      CYCLES(0xec9e, 4);
      /* $EC9E LDA */ s_a = update_nz(peek_zpg(0x9a));
    case 0xeca0: // [$ECA0..$ECA6]    7 bytes
      CYCLES(0xeca0, 12);
      /* $ECA0 SEC */ s_status |= STATUS_C;
      /* $ECA1 SBC */ tmp = peek_zpg(0x99), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $ECA3 STA */ poke_zpg(0x9a, s_a);
      /* $ECA5 BEQ */ s_pc = s_status & STATUS_Z ? 0xecb9 : 0xeca7;
      branchTarget = true;
      break;
    case 0xeca7: // [$ECA7..$ECA8]    2 bytes
      CYCLES(0xeca7, 4);
      /* $ECA7 BPL */ s_pc = !(s_status & STATUS_N) ? 0xecb2 : 0xeca9;
      branchTarget = true;
      break;
    case 0xeca9: // [$ECA9..$ECAB]    3 bytes
      CYCLES(0xeca9, 6);
      /* $ECA9 JSR */ push16(0xecab), s_pc = 0xea55;
      branchTarget = true;
      break;
    case 0xecac: // [$ECAC..$ECAF]    4 bytes
      CYCLES(0xecac, 7);
      /* $ECAC INC */ tmp16 = 0x9a, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $ECAE BNE */ s_pc = !(s_status & STATUS_Z) ? 0xeca9 : 0xecb0;
      branchTarget = true;
      break;
    case 0xecb0: // [$ECB0..$ECB1]    2 bytes
      CYCLES(0xecb0, 4);
      /* $ECB0 BEQ */ s_pc = s_status & STATUS_Z ? 0xecb9 : 0xecb2;
      branchTarget = true;
      break;
    case 0xecb2: // [$ECB2..$ECB4]    3 bytes
      CYCLES(0xecb2, 6);
      /* $ECB2 JSR */ push16(0xecb4), s_pc = 0xea39;
      branchTarget = true;
      break;
    case 0xecb5: // [$ECB5..$ECB8]    4 bytes
      CYCLES(0xecb5, 7);
      /* $ECB5 DEC */ tmp16 = 0x9a, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $ECB7 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xecb2 : 0xecb9;
      branchTarget = true;
      break;
    case 0xecb9: // [$ECB9..$ECBC]    4 bytes
      CYCLES(0xecb9, 7);
      /* $ECB9 LDA */ s_a = update_nz(peek_zpg(0xa3));
      /* $ECBB BMI */ s_pc = s_status & STATUS_N ? 0xecbe : 0xecbd;
      branchTarget = true;
      break;
    case 0xecbd: // [$ECBD..$ECBD]    1 bytes
      CYCLES(0xecbd, 2);
      /* $ECBD RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xecbe: // [$ECBE..$ECC0]    3 bytes
      CYCLES(0xecbe, 6);
      /* $ECBE JMP */ s_pc = 0xeed0;
      branchTarget = true;
      break;
    case 0xecc1: // [$ECC1..$ECC5]    5 bytes
      CYCLES(0xecc1, 9);
      /* $ECC1 PHA */ push8(s_a);
      /* $ECC2 BIT */ tmp = peek_zpg(0x9b), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $ECC4 BPL */ s_pc = !(s_status & STATUS_N) ? 0xecc8 : 0xecc6;
      branchTarget = true;
      break;
    case 0xecc6: // [$ECC6..$ECC7]    2 bytes
      CYCLES(0xecc6, 4);
      /* $ECC6 INC */ tmp16 = 0x99, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
    case 0xecc8: // [$ECC8..$ECCA]    3 bytes
      CYCLES(0xecc8, 6);
      /* $ECC8 JSR */ push16(0xecca), s_pc = 0xea39;
      branchTarget = true;
      break;
    case 0xeccb: // [$ECCB..$ECD1]    7 bytes
      CYCLES(0xeccb, 12);
      /* $ECCB PLA */ s_a = update_nz(pop8());
      /* $ECCC SEC */ s_status |= STATUS_C;
      /* $ECCD SBC */ tmp = 0x30, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $ECCF JSR */ push16(0xecd1), s_pc = 0xecd5;
      branchTarget = true;
      break;
    case 0xecd2: // [$ECD2..$ECD4]    3 bytes
      CYCLES(0xecd2, 6);
      /* $ECD2 JMP */ s_pc = 0xec61;
      branchTarget = true;
      break;
    case 0xecd5: // [$ECD5..$ECD8]    4 bytes
      CYCLES(0xecd5, 7);
      /* $ECD5 PHA */ push8(s_a);
      /* $ECD6 JSR */ push16(0xecd8), s_pc = 0xeb63;
      branchTarget = true;
      break;
    case 0xecd9: // [$ECD9..$ECDC]    4 bytes
      CYCLES(0xecd9, 7);
      /* $ECD9 PLA */ s_a = update_nz(pop8());
      /* $ECDA JSR */ push16(0xecdc), s_pc = 0xeb93;
      branchTarget = true;
      break;
    case 0xecdd: // [$ECDD..$ECE7]   11 bytes
      CYCLES(0xecdd, 19);
      /* $ECDD LDA */ s_a = update_nz(peek_zpg(0xaa));
      /* $ECDF EOR */ s_a = update_nz(s_a ^ peek_zpg(0xa2));
      /* $ECE1 STA */ poke_zpg(0xab, s_a);
      /* $ECE3 LDX */ s_x = update_nz(peek_zpg(0x9d));
      /* $ECE5 JMP */ s_pc = 0xe7c1;
      branchTarget = true;
      break;
    case 0xece8: // [$ECE8..$ECED]    6 bytes
      CYCLES(0xece8, 11);
      /* $ECE8 LDA */ s_a = update_nz(peek_zpg(0x9a));
      /* $ECEA CMP */ update_nz_inv_c(s_a - 0x0a);
      /* $ECEC BCC */ s_pc = !(s_status & STATUS_C) ? 0xecf7 : 0xecee;
      branchTarget = true;
      break;
    case 0xecee: // [$ECEE..$ECF3]    6 bytes
      CYCLES(0xecee, 11);
      /* $ECEE LDA */ s_a = update_nz(0x64);
      /* $ECF0 BIT */ tmp = peek_zpg(0x9c), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $ECF2 BMI */ s_pc = s_status & STATUS_N ? 0xed05 : 0xecf4;
      branchTarget = true;
      break;
    case 0xecf4: // [$ECF4..$ECF6]    3 bytes
      CYCLES(0xecf4, 6);
      /* $ECF4 JMP */ s_pc = 0xe8d5;
      branchTarget = true;
      break;
    case 0xecf7: // [$ECF7..$ED04]   14 bytes
      CYCLES(0xecf7, 24);
      /* $ECF7 ASL */ s_a = update_nzc(s_a << 1);
      /* $ECF8 ASL */ s_a = update_nzc(s_a << 1);
      /* $ECF9 CLC */ s_status &= ~STATUS_C;
      /* $ECFA ADC */ tmp = peek_zpg(0x9a), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $ECFC ASL */ s_a = update_nzc(s_a << 1);
      /* $ECFD CLC */ s_status &= ~STATUS_C;
      /* $ECFE LDY */ s_y = update_nz(0x00);
      /* $ED00 ADC */ tmp = peek(peek16_zpg(0xb8) + s_y), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $ED02 SEC */ s_status |= STATUS_C;
      /* $ED03 SBC */ tmp = 0x30, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
    case 0xed05: // [$ED05..$ED09]    5 bytes
      CYCLES(0xed05, 9);
      /* $ED05 STA */ poke_zpg(0x9a, s_a);
      /* $ED07 JMP */ s_pc = 0xec87;
      branchTarget = true;
      break;
    case 0xed19: // [$ED19..$ED1F]    7 bytes
      CYCLES(0xed19, 12);
      /* $ED19 LDA */ s_a = update_nz(0x58);
      /* $ED1B LDY */ s_y = update_nz(0xd3);
      /* $ED1D JSR */ push16(0xed1f), s_pc = 0xed31;
      branchTarget = true;
      break;
    case 0xed20: // [$ED20..$ED23]    4 bytes
      CYCLES(0xed20, 7);
      /* $ED20 LDA */ s_a = update_nz(peek_zpg(0x76));
      /* $ED22 LDX */ s_x = update_nz(peek_zpg(0x75));
    case 0xed24: // [$ED24..$ED2D]   10 bytes
      CYCLES(0xed24, 18);
      /* $ED24 STA */ poke_zpg(0x9e, s_a);
      /* $ED26 STX */ poke_zpg(0x9f, s_x);
      /* $ED28 LDX */ s_x = update_nz(0x90);
      /* $ED2A SEC */ s_status |= STATUS_C;
      /* $ED2B JSR */ push16(0xed2d), s_pc = 0xeba0;
      branchTarget = true;
      break;
    case 0xed2e: // [$ED2E..$ED30]    3 bytes
      CYCLES(0xed2e, 6);
      /* $ED2E JSR */ push16(0xed30), s_pc = 0xed34;
      branchTarget = true;
      break;
    case 0xed31: // [$ED31..$ED33]    3 bytes
      CYCLES(0xed31, 6);
      /* $ED31 JMP */ s_pc = 0xdb3a;
      branchTarget = true;
      break;
    case 0xed34: // [$ED34..$ED3C]    9 bytes
      CYCLES(0xed34, 16);
      /* $ED34 LDY */ s_y = update_nz(0x01);
      /* $ED36 LDA */ s_a = update_nz(0x2d);
      /* $ED38 DEY */ s_y = update_nz(s_y - 1);
      /* $ED39 BIT */ tmp = peek_zpg(0xa2), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $ED3B BPL */ s_pc = !(s_status & STATUS_N) ? 0xed41 : 0xed3d;
      branchTarget = true;
      break;
    case 0xed3d: // [$ED3D..$ED40]    4 bytes
      CYCLES(0xed3d, 7);
      /* $ED3D INY */ s_y = update_nz(s_y + 1);
      /* $ED3E STA */ poke(0x00ff + s_y, s_a);
    case 0xed41: // [$ED41..$ED4B]   11 bytes
      CYCLES(0xed41, 19);
      /* $ED41 STA */ poke_zpg(0xa2, s_a);
      /* $ED43 STY */ poke_zpg(0xad, s_y);
      /* $ED45 INY */ s_y = update_nz(s_y + 1);
      /* $ED46 LDA */ s_a = update_nz(0x30);
      /* $ED48 LDX */ s_x = update_nz(peek_zpg(0x9d));
      /* $ED4A BNE */ s_pc = !(s_status & STATUS_Z) ? 0xed4f : 0xed4c;
      branchTarget = true;
      break;
    case 0xed4c: // [$ED4C..$ED4E]    3 bytes
      CYCLES(0xed4c, 6);
      /* $ED4C JMP */ s_pc = 0xee57;
      branchTarget = true;
      break;
    case 0xed4f: // [$ED4F..$ED54]    6 bytes
      CYCLES(0xed4f, 11);
      /* $ED4F LDA */ s_a = update_nz(0x00);
      /* $ED51 CPX */ update_nz_inv_c(s_x - 0x80);
      /* $ED53 BEQ */ s_pc = s_status & STATUS_Z ? 0xed57 : 0xed55;
      branchTarget = true;
      break;
    case 0xed55: // [$ED55..$ED56]    2 bytes
      CYCLES(0xed55, 4);
      /* $ED55 BCS */ s_pc = s_status & STATUS_C ? 0xed60 : 0xed57;
      branchTarget = true;
      break;
    case 0xed57: // [$ED57..$ED5D]    7 bytes
      CYCLES(0xed57, 12);
      /* $ED57 LDA */ s_a = update_nz(0x14);
      /* $ED59 LDY */ s_y = update_nz(0xed);
      /* $ED5B JSR */ push16(0xed5d), s_pc = 0xe97f;
      branchTarget = true;
      break;
    case 0xed5e: // [$ED5E..$ED5F]    2 bytes
      CYCLES(0xed5e, 4);
      /* $ED5E LDA */ s_a = update_nz(0xf7);
    case 0xed60: // [$ED60..$ED61]    2 bytes
      CYCLES(0xed60, 4);
      /* $ED60 STA */ poke_zpg(0x99, s_a);
    case 0xed62: // [$ED62..$ED68]    7 bytes
      CYCLES(0xed62, 12);
      /* $ED62 LDA */ s_a = update_nz(0x0f);
      /* $ED64 LDY */ s_y = update_nz(0xed);
      /* $ED66 JSR */ push16(0xed68), s_pc = 0xebb2;
      branchTarget = true;
      break;
    case 0xed69: // [$ED69..$ED6A]    2 bytes
      CYCLES(0xed69, 4);
      /* $ED69 BEQ */ s_pc = s_status & STATUS_Z ? 0xed89 : 0xed6b;
      branchTarget = true;
      break;
    case 0xed6b: // [$ED6B..$ED6C]    2 bytes
      CYCLES(0xed6b, 4);
      /* $ED6B BPL */ s_pc = !(s_status & STATUS_N) ? 0xed7f : 0xed6d;
      branchTarget = true;
      break;
    case 0xed6d: // [$ED6D..$ED73]    7 bytes
      CYCLES(0xed6d, 12);
      /* $ED6D LDA */ s_a = update_nz(0x0a);
      /* $ED6F LDY */ s_y = update_nz(0xed);
      /* $ED71 JSR */ push16(0xed73), s_pc = 0xebb2;
      branchTarget = true;
      break;
    case 0xed74: // [$ED74..$ED75]    2 bytes
      CYCLES(0xed74, 4);
      /* $ED74 BEQ */ s_pc = s_status & STATUS_Z ? 0xed78 : 0xed76;
      branchTarget = true;
      break;
    case 0xed76: // [$ED76..$ED77]    2 bytes
      CYCLES(0xed76, 4);
      /* $ED76 BPL */ s_pc = !(s_status & STATUS_N) ? 0xed86 : 0xed78;
      branchTarget = true;
      break;
    case 0xed78: // [$ED78..$ED7A]    3 bytes
      CYCLES(0xed78, 6);
      /* $ED78 JSR */ push16(0xed7a), s_pc = 0xea39;
      branchTarget = true;
      break;
    case 0xed7b: // [$ED7B..$ED7E]    4 bytes
      CYCLES(0xed7b, 7);
      /* $ED7B DEC */ tmp16 = 0x99, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $ED7D BNE */ s_pc = !(s_status & STATUS_Z) ? 0xed6d : 0xed7f;
      branchTarget = true;
      break;
    case 0xed7f: // [$ED7F..$ED81]    3 bytes
      CYCLES(0xed7f, 6);
      /* $ED7F JSR */ push16(0xed81), s_pc = 0xea55;
      branchTarget = true;
      break;
    case 0xed82: // [$ED82..$ED85]    4 bytes
      CYCLES(0xed82, 7);
      /* $ED82 INC */ tmp16 = 0x99, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $ED84 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xed62 : 0xed86;
      branchTarget = true;
      break;
    case 0xed86: // [$ED86..$ED88]    3 bytes
      CYCLES(0xed86, 6);
      /* $ED86 JSR */ push16(0xed88), s_pc = 0xe7a0;
      branchTarget = true;
      break;
    case 0xed89: // [$ED89..$ED8B]    3 bytes
      CYCLES(0xed89, 6);
      /* $ED89 JSR */ push16(0xed8b), s_pc = 0xebf2;
      branchTarget = true;
      break;
    case 0xed8c: // [$ED8C..$ED94]    9 bytes
      CYCLES(0xed8c, 16);
      /* $ED8C LDX */ s_x = update_nz(0x01);
      /* $ED8E LDA */ s_a = update_nz(peek_zpg(0x99));
      /* $ED90 CLC */ s_status &= ~STATUS_C;
      /* $ED91 ADC */ tmp = 0x0a, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $ED93 BMI */ s_pc = s_status & STATUS_N ? 0xed9e : 0xed95;
      branchTarget = true;
      break;
    case 0xed95: // [$ED95..$ED98]    4 bytes
      CYCLES(0xed95, 7);
      /* $ED95 CMP */ update_nz_inv_c(s_a - 0x0b);
      /* $ED97 BCS */ s_pc = s_status & STATUS_C ? 0xed9f : 0xed99;
      branchTarget = true;
      break;
    case 0xed99: // [$ED99..$ED9D]    5 bytes
      CYCLES(0xed99, 9);
      /* $ED99 ADC */ tmp = 0xff, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $ED9B TAX */ s_x = update_nz(s_a);
      /* $ED9C LDA */ s_a = update_nz(0x02);
    case 0xed9e: // [$ED9E..$ED9E]    1 bytes
      CYCLES(0xed9e, 2);
      /* $ED9E SEC */ s_status |= STATUS_C;
    case 0xed9f: // [$ED9F..$EDA7]    9 bytes
      CYCLES(0xed9f, 16);
      /* $ED9F SBC */ tmp = 0x02, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $EDA1 STA */ poke_zpg(0x9a, s_a);
      /* $EDA3 STX */ poke_zpg(0x99, s_x);
      /* $EDA5 TXA */ s_a = update_nz(s_x);
      /* $EDA6 BEQ */ s_pc = s_status & STATUS_Z ? 0xedaa : 0xeda8;
      branchTarget = true;
      break;
    case 0xeda8: // [$EDA8..$EDA9]    2 bytes
      CYCLES(0xeda8, 4);
      /* $EDA8 BPL */ s_pc = !(s_status & STATUS_N) ? 0xedbd : 0xedaa;
      branchTarget = true;
      break;
    case 0xedaa: // [$EDAA..$EDB4]   11 bytes
      CYCLES(0xedaa, 19);
      /* $EDAA LDY */ s_y = update_nz(peek_zpg(0xad));
      /* $EDAC LDA */ s_a = update_nz(0x2e);
      /* $EDAE INY */ s_y = update_nz(s_y + 1);
      /* $EDAF STA */ poke(0x00ff + s_y, s_a);
      /* $EDB2 TXA */ s_a = update_nz(s_x);
      /* $EDB3 BEQ */ s_pc = s_status & STATUS_Z ? 0xedbb : 0xedb5;
      branchTarget = true;
      break;
    case 0xedb5: // [$EDB5..$EDBA]    6 bytes
      CYCLES(0xedb5, 11);
      /* $EDB5 LDA */ s_a = update_nz(0x30);
      /* $EDB7 INY */ s_y = update_nz(s_y + 1);
      /* $EDB8 STA */ poke(0x00ff + s_y, s_a);
    case 0xedbb: // [$EDBB..$EDBC]    2 bytes
      CYCLES(0xedbb, 4);
      /* $EDBB STY */ poke_zpg(0xad, s_y);
    case 0xedbd: // [$EDBD..$EDC0]    4 bytes
      CYCLES(0xedbd, 7);
      /* $EDBD LDY */ s_y = update_nz(0x00);
      /* $EDBF LDX */ s_x = update_nz(0x80);
    case 0xedc1: // [$EDC1..$EDE0]   32 bytes
      CYCLES(0xedc1, 55);
      /* $EDC1 LDA */ s_a = update_nz(peek_zpg(0xa1));
      /* $EDC3 CLC */ s_status &= ~STATUS_C;
      /* $EDC4 ADC */ tmp = peek(0xee6c + s_y), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $EDC7 STA */ poke_zpg(0xa1, s_a);
      /* $EDC9 LDA */ s_a = update_nz(peek_zpg(0xa0));
      /* $EDCB ADC */ tmp = peek(0xee6b + s_y), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $EDCE STA */ poke_zpg(0xa0, s_a);
      /* $EDD0 LDA */ s_a = update_nz(peek_zpg(0x9f));
      /* $EDD2 ADC */ tmp = peek(0xee6a + s_y), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $EDD5 STA */ poke_zpg(0x9f, s_a);
      /* $EDD7 LDA */ s_a = update_nz(peek_zpg(0x9e));
      /* $EDD9 ADC */ tmp = peek(0xee69 + s_y), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $EDDC STA */ poke_zpg(0x9e, s_a);
      /* $EDDE INX */ s_x = update_nz(s_x + 1);
      /* $EDDF BCS */ s_pc = s_status & STATUS_C ? 0xede5 : 0xede1;
      branchTarget = true;
      break;
    case 0xede1: // [$EDE1..$EDE2]    2 bytes
      CYCLES(0xede1, 4);
      /* $EDE1 BPL */ s_pc = !(s_status & STATUS_N) ? 0xedc1 : 0xede3;
      branchTarget = true;
      break;
    case 0xede3: // [$EDE3..$EDE4]    2 bytes
      CYCLES(0xede3, 4);
      /* $EDE3 BMI */ s_pc = s_status & STATUS_N ? 0xede7 : 0xede5;
      branchTarget = true;
      break;
    case 0xede5: // [$EDE5..$EDE6]    2 bytes
      CYCLES(0xede5, 4);
      /* $EDE5 BMI */ s_pc = s_status & STATUS_N ? 0xedc1 : 0xede7;
      branchTarget = true;
      break;
    case 0xede7: // [$EDE7..$EDE9]    3 bytes
      CYCLES(0xede7, 6);
      /* $EDE7 TXA */ s_a = update_nz(s_x);
      /* $EDE8 BCC */ s_pc = !(s_status & STATUS_C) ? 0xedee : 0xedea;
      branchTarget = true;
      break;
    case 0xedea: // [$EDEA..$EDED]    4 bytes
      CYCLES(0xedea, 7);
      /* $EDEA EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $EDEC ADC */ tmp = 0x0a, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0xedee: // [$EDEE..$EE02]   21 bytes
      CYCLES(0xedee, 36);
      /* $EDEE ADC */ tmp = 0x2f, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $EDF0 INY */ s_y = update_nz(s_y + 1);
      /* $EDF1 INY */ s_y = update_nz(s_y + 1);
      /* $EDF2 INY */ s_y = update_nz(s_y + 1);
      /* $EDF3 INY */ s_y = update_nz(s_y + 1);
      /* $EDF4 STY */ poke_zpg(0x83, s_y);
      /* $EDF6 LDY */ s_y = update_nz(peek_zpg(0xad));
      /* $EDF8 INY */ s_y = update_nz(s_y + 1);
      /* $EDF9 TAX */ s_x = update_nz(s_a);
      /* $EDFA AND */ s_a = update_nz(s_a & 0x7f);
      /* $EDFC STA */ poke(0x00ff + s_y, s_a);
      /* $EDFF DEC */ tmp16 = 0x99, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $EE01 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xee09 : 0xee03;
      branchTarget = true;
      break;
    case 0xee03: // [$EE03..$EE08]    6 bytes
      CYCLES(0xee03, 11);
      /* $EE03 LDA */ s_a = update_nz(0x2e);
      /* $EE05 INY */ s_y = update_nz(s_y + 1);
      /* $EE06 STA */ poke(0x00ff + s_y, s_a);
    case 0xee09: // [$EE09..$EE16]   14 bytes
      CYCLES(0xee09, 24);
      /* $EE09 STY */ poke_zpg(0xad, s_y);
      /* $EE0B LDY */ s_y = update_nz(peek_zpg(0x83));
      /* $EE0D TXA */ s_a = update_nz(s_x);
      /* $EE0E EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $EE10 AND */ s_a = update_nz(s_a & 0x80);
      /* $EE12 TAX */ s_x = update_nz(s_a);
      /* $EE13 CPY */ update_nz_inv_c(s_y - 0x24);
      /* $EE15 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xedc1 : 0xee17;
      branchTarget = true;
      break;
    case 0xee17: // [$EE17..$EE18]    2 bytes
      CYCLES(0xee17, 4);
      /* $EE17 LDY */ s_y = update_nz(peek_zpg(0xad));
    case 0xee19: // [$EE19..$EE20]    8 bytes
      CYCLES(0xee19, 14);
      /* $EE19 LDA */ s_a = update_nz(peek(0x00ff + s_y));
      /* $EE1C DEY */ s_y = update_nz(s_y - 1);
      /* $EE1D CMP */ update_nz_inv_c(s_a - 0x30);
      /* $EE1F BEQ */ s_pc = s_status & STATUS_Z ? 0xee19 : 0xee21;
      branchTarget = true;
      break;
    case 0xee21: // [$EE21..$EE24]    4 bytes
      CYCLES(0xee21, 7);
      /* $EE21 CMP */ update_nz_inv_c(s_a - 0x2e);
      /* $EE23 BEQ */ s_pc = s_status & STATUS_Z ? 0xee26 : 0xee25;
      branchTarget = true;
      break;
    case 0xee25: // [$EE25..$EE25]    1 bytes
      CYCLES(0xee25, 2);
      /* $EE25 INY */ s_y = update_nz(s_y + 1);
    case 0xee26: // [$EE26..$EE2B]    6 bytes
      CYCLES(0xee26, 11);
      /* $EE26 LDA */ s_a = update_nz(0x2b);
      /* $EE28 LDX */ s_x = update_nz(peek_zpg(0x9a));
      /* $EE2A BEQ */ s_pc = s_status & STATUS_Z ? 0xee5a : 0xee2c;
      branchTarget = true;
      break;
    case 0xee2c: // [$EE2C..$EE2D]    2 bytes
      CYCLES(0xee2c, 4);
      /* $EE2C BPL */ s_pc = !(s_status & STATUS_N) ? 0xee36 : 0xee2e;
      branchTarget = true;
      break;
    case 0xee2e: // [$EE2E..$EE35]    8 bytes
      CYCLES(0xee2e, 14);
      /* $EE2E LDA */ s_a = update_nz(0x00);
      /* $EE30 SEC */ s_status |= STATUS_C;
      /* $EE31 SBC */ tmp = peek_zpg(0x9a), s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $EE33 TAX */ s_x = update_nz(s_a);
      /* $EE34 LDA */ s_a = update_nz(0x2d);
    case 0xee36: // [$EE36..$EE41]   12 bytes
      CYCLES(0xee36, 21);
      /* $EE36 STA */ poke(0x0101 + s_y, s_a);
      /* $EE39 LDA */ s_a = update_nz(0x45);
      /* $EE3B STA */ poke(0x0100 + s_y, s_a);
      /* $EE3E TXA */ s_a = update_nz(s_x);
      /* $EE3F LDX */ s_x = update_nz(0x2f);
      /* $EE41 SEC */ s_status |= STATUS_C;
    case 0xee42: // [$EE42..$EE46]    5 bytes
      CYCLES(0xee42, 9);
      /* $EE42 INX */ s_x = update_nz(s_x + 1);
      /* $EE43 SBC */ tmp = 0x0a, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $EE45 BCS */ s_pc = s_status & STATUS_C ? 0xee42 : 0xee47;
      branchTarget = true;
      break;
    case 0xee47: // [$EE47..$EE56]   16 bytes
      CYCLES(0xee47, 28);
      /* $EE47 ADC */ tmp = 0x3a, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $EE49 STA */ poke(0x0103 + s_y, s_a);
      /* $EE4C TXA */ s_a = update_nz(s_x);
      /* $EE4D STA */ poke(0x0102 + s_y, s_a);
      /* $EE50 LDA */ s_a = update_nz(0x00);
      /* $EE52 STA */ poke(0x0104 + s_y, s_a);
      /* $EE55 BEQ */ s_pc = s_status & STATUS_Z ? 0xee5f : 0xee57;
      branchTarget = true;
      break;
    case 0xee57: // [$EE57..$EE59]    3 bytes
      CYCLES(0xee57, 6);
      /* $EE57 STA */ poke(0x00ff + s_y, s_a);
    case 0xee5a: // [$EE5A..$EE5E]    5 bytes
      CYCLES(0xee5a, 9);
      /* $EE5A LDA */ s_a = update_nz(0x00);
      /* $EE5C STA */ poke(0x0100 + s_y, s_a);
    case 0xee5f: // [$EE5F..$EE63]    5 bytes
      CYCLES(0xee5f, 9);
      /* $EE5F LDA */ s_a = update_nz(0x00);
      /* $EE61 LDY */ s_y = update_nz(0x01);
      /* $EE63 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xeed0: // [$EED0..$EED3]    4 bytes
      CYCLES(0xeed0, 7);
      /* $EED0 LDA */ s_a = update_nz(peek_zpg(0x9d));
      /* $EED2 BEQ */ s_pc = s_status & STATUS_Z ? 0xeeda : 0xeed4;
      branchTarget = true;
      break;
    case 0xeed4: // [$EED4..$EED9]    6 bytes
      CYCLES(0xeed4, 11);
      /* $EED4 LDA */ s_a = update_nz(peek_zpg(0xa2));
      /* $EED6 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $EED8 STA */ poke_zpg(0xa2, s_a);
    case 0xeeda: // [$EEDA..$EEDA]    1 bytes
      CYCLES(0xeeda, 2);
      /* $EEDA RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xf128: // [$F128..$F13D]   22 bytes
      CYCLES(0xf128, 38);
      /* $F128 LDX */ s_x = update_nz(0xff);
      /* $F12A STX */ poke_zpg(0x76, s_x);
      /* $F12C LDX */ s_x = update_nz(0xfb);
      /* $F12E TXS */ s_sp = s_x;
      /* $F12F LDA */ s_a = update_nz(0x28);
      /* $F131 LDY */ s_y = update_nz(0xf1);
      /* $F133 STA */ poke_zpg(0x01, s_a);
      /* $F135 STY */ poke_zpg(0x02, s_y);
      /* $F137 STA */ poke_zpg(0x04, s_a);
      /* $F139 STY */ poke_zpg(0x05, s_y);
      /* $F13B JSR */ push16(0xf13d), s_pc = 0xf273;
      branchTarget = true;
      break;
    case 0xf13e: // [$F13E..$F151]   20 bytes
      CYCLES(0xf13e, 35);
      /* $F13E LDA */ s_a = update_nz(0x4c);
      /* $F140 STA */ poke_zpg(0x00, s_a);
      /* $F142 STA */ poke_zpg(0x03, s_a);
      // WARNING: performs self modification.
      /* $F144 STA */ poke_zpg(0x90, s_a);
      /* $F146 STA */ poke_zpg(0x0a, s_a);
      /* $F148 LDA */ s_a = update_nz(0x99);
      /* $F14A LDY */ s_y = update_nz(0xe1);
      /* $F14C STA */ poke_zpg(0x0b, s_a);
      /* $F14E STY */ poke_zpg(0x0c, s_y);
      /* $F150 LDX */ s_x = update_nz(0x1c);
    case 0xf152: // [$F152..$F15B]   10 bytes
      CYCLES(0xf152, 18);
      /* $F152 LDA */ s_a = update_nz(peek(0xf10a + s_x));
      /* $F155 STA */ poke_zpg((uint8_t)(0xb0 + s_x), s_a);
      /* $F157 STX */ poke_zpg(0xf1, s_x);
      /* $F159 DEX */ s_x = update_nz(s_x - 1);
      /* $F15A BNE */ s_pc = !(s_status & STATUS_Z) ? 0xf152 : 0xf15c;
      branchTarget = true;
      break;
    case 0xf15c: // [$F15C..$F16A]   15 bytes
      CYCLES(0xf15c, 26);
      /* $F15C STX */ poke_zpg(0xf2, s_x);
      /* $F15E TXA */ s_a = update_nz(s_x);
      /* $F15F STA */ poke_zpg(0xa4, s_a);
      /* $F161 STA */ poke_zpg(0x54, s_a);
      /* $F163 PHA */ push8(s_a);
      /* $F164 LDA */ s_a = update_nz(0x03);
      /* $F166 STA */ poke_zpg(0x8f, s_a);
      /* $F168 JSR */ push16(0xf16a), s_pc = 0xdafb;
      branchTarget = true;
      break;
    case 0xf16b: // [$F16B..$F180]   22 bytes
      CYCLES(0xf16b, 38);
      /* $F16B LDA */ s_a = update_nz(0x01);
      /* $F16D STA */ poke(0x01fd, s_a);
      /* $F170 STA */ poke(0x01fc, s_a);
      /* $F173 LDX */ s_x = update_nz(0x55);
      /* $F175 STX */ poke_zpg(0x52, s_x);
      /* $F177 LDA */ s_a = update_nz(0x00);
      /* $F179 LDY */ s_y = update_nz(0x08);
      /* $F17B STA */ poke_zpg(0x50, s_a);
      /* $F17D STY */ poke_zpg(0x51, s_y);
      /* $F17F LDY */ s_y = update_nz(0x00);
    case 0xf181: // [$F181..$F18C]   12 bytes
      CYCLES(0xf181, 21);
      /* $F181 INC */ tmp16 = 0x51, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $F183 LDA */ s_a = update_nz(peek(peek16_zpg(0x50) + s_y));
      /* $F185 EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $F187 STA */ poke(peek16_zpg(0x50) + s_y, s_a);
      /* $F189 CMP */ update_nz_inv_c(s_a - peek(peek16_zpg(0x50) + s_y));
      /* $F18B BNE */ s_pc = !(s_status & STATUS_Z) ? 0xf195 : 0xf18d;
      branchTarget = true;
      break;
    case 0xf18d: // [$F18D..$F194]    8 bytes
      CYCLES(0xf18d, 14);
      /* $F18D EOR */ s_a = update_nz(s_a ^ 0xff);
      /* $F18F STA */ poke(peek16_zpg(0x50) + s_y, s_a);
      /* $F191 CMP */ update_nz_inv_c(s_a - peek(peek16_zpg(0x50) + s_y));
      /* $F193 BEQ */ s_pc = s_status & STATUS_Z ? 0xf181 : 0xf195;
      branchTarget = true;
      break;
    case 0xf195: // [$F195..$F1B5]   33 bytes
      CYCLES(0xf195, 57);
      /* $F195 LDY */ s_y = update_nz(peek_zpg(0x50));
      /* $F197 LDA */ s_a = update_nz(peek_zpg(0x51));
      /* $F199 AND */ s_a = update_nz(s_a & 0xf0);
      /* $F19B STY */ poke_zpg(0x73, s_y);
      /* $F19D STA */ poke_zpg(0x74, s_a);
      /* $F19F STY */ poke_zpg(0x6f, s_y);
      /* $F1A1 STA */ poke_zpg(0x70, s_a);
      /* $F1A3 LDX */ s_x = update_nz(0x00);
      /* $F1A5 LDY */ s_y = update_nz(0x08);
      /* $F1A7 STX */ poke_zpg(0x67, s_x);
      /* $F1A9 STY */ poke_zpg(0x68, s_y);
      /* $F1AB LDY */ s_y = update_nz(0x00);
      /* $F1AD STY */ poke_zpg(0xd6, s_y);
      /* $F1AF TYA */ s_a = update_nz(s_y);
      /* $F1B0 STA */ poke(peek16_zpg(0x67) + s_y, s_a);
      /* $F1B2 INC */ tmp16 = 0x67, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $F1B4 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xf1b8 : 0xf1b6;
      branchTarget = true;
      break;
    case 0xf1b6: // [$F1B6..$F1B7]    2 bytes
      CYCLES(0xf1b6, 4);
      /* $F1B6 INC */ tmp16 = 0x68, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
    case 0xf1b8: // [$F1B8..$F1BE]    7 bytes
      CYCLES(0xf1b8, 12);
      /* $F1B8 LDA */ s_a = update_nz(peek_zpg(0x67));
      /* $F1BA LDY */ s_y = update_nz(peek_zpg(0x68));
      /* $F1BC JSR */ push16(0xf1be), s_pc = 0xd3e3;
      branchTarget = true;
      break;
    case 0xf1bf: // [$F1BF..$F1C1]    3 bytes
      CYCLES(0xf1bf, 6);
      /* $F1BF JSR */ push16(0xf1c1), s_pc = 0xd64b;
      branchTarget = true;
      break;
    case 0xf1c2: // [$F1C2..$F1D4]   19 bytes
      CYCLES(0xf1c2, 33);
      /* $F1C2 LDA */ s_a = update_nz(0x3a);
      /* $F1C4 LDY */ s_y = update_nz(0xdb);
      /* $F1C6 STA */ poke_zpg(0x04, s_a);
      /* $F1C8 STY */ poke_zpg(0x05, s_y);
      /* $F1CA LDA */ s_a = update_nz(0x3c);
      /* $F1CC LDY */ s_y = update_nz(0xd4);
      /* $F1CE STA */ poke_zpg(0x01, s_a);
      /* $F1D0 STY */ poke_zpg(0x02, s_y);
      /* $F1D2 JMP */ s_pc = peek16(0x0001);
      branchTarget = true;
      break;
    case 0xf1ec: // [$F1EC..$F1EE]    3 bytes
      CYCLES(0xf1ec, 6);
      /* $F1EC JSR */ push16(0xf1ee), s_pc = 0xe6f8;
      branchTarget = true;
      break;
    case 0xf1ef: // [$F1EF..$F1F2]    4 bytes
      CYCLES(0xf1ef, 7);
      /* $F1EF CPX */ update_nz_inv_c(s_x - 0x30);
      /* $F1F1 BCS */ s_pc = s_status & STATUS_C ? 0xf206 : 0xf1f3;
      branchTarget = true;
      break;
    case 0xf1f3: // [$F1F3..$F1F9]    7 bytes
      CYCLES(0xf1f3, 12);
      /* $F1F3 STX */ poke_zpg(0xf0, s_x);
      /* $F1F5 LDA */ s_a = update_nz(0x2c);
      /* $F1F7 JSR */ push16(0xf1f9), s_pc = 0xdec0;
      branchTarget = true;
      break;
    case 0xf1fa: // [$F1FA..$F1FC]    3 bytes
      CYCLES(0xf1fa, 6);
      /* $F1FA JSR */ push16(0xf1fc), s_pc = 0xe6f8;
      branchTarget = true;
      break;
    case 0xf1fd: // [$F1FD..$F200]    4 bytes
      CYCLES(0xf1fd, 7);
      /* $F1FD CPX */ update_nz_inv_c(s_x - 0x30);
      /* $F1FF BCS */ s_pc = s_status & STATUS_C ? 0xf206 : 0xf201;
      branchTarget = true;
      break;
    case 0xf201: // [$F201..$F205]    5 bytes
      CYCLES(0xf201, 9);
      /* $F201 STX */ poke_zpg(0x2c, s_x);
      /* $F203 STX */ poke_zpg(0x2d, s_x);
      /* $F205 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xf206: // [$F206..$F208]    3 bytes
      CYCLES(0xf206, 6);
      /* $F206 JMP */ s_pc = 0xe199;
      branchTarget = true;
      break;
    case 0xf273: // [$F273..$F276]    4 bytes
      CYCLES(0xf273, 7);
      /* $F273 LDA */ s_a = update_nz(0xff);
      /* $F275 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xf279 : 0xf277;
      branchTarget = true;
      break;
    case 0xf277: // [$F277..$F278]    2 bytes
      CYCLES(0xf277, 4);
      /* $F277 LDA */ s_a = update_nz(0x3f);
    case 0xf279: // [$F279..$F27F]    7 bytes
      CYCLES(0xf279, 12);
      /* $F279 LDX */ s_x = update_nz(0x00);
      /* $F27B STA */ poke_zpg(0x32, s_a);
      /* $F27D STX */ poke_zpg(0xf3, s_x);
      /* $F27F RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xf2e9: // [$F2E9..$F311]   41 bytes
      CYCLES(0xf2e9, 70);
      /* $F2E9 STX */ poke_zpg(0xde, s_x);
      /* $F2EB LDX */ s_x = update_nz(peek_zpg(0xf8));
      /* $F2ED STX */ poke_zpg(0xdf, s_x);
      /* $F2EF LDA */ s_a = update_nz(peek_zpg(0x75));
      /* $F2F1 STA */ poke_zpg(0xda, s_a);
      /* $F2F3 LDA */ s_a = update_nz(peek_zpg(0x76));
      /* $F2F5 STA */ poke_zpg(0xdb, s_a);
      /* $F2F7 LDA */ s_a = update_nz(peek_zpg(0x79));
      /* $F2F9 STA */ poke_zpg(0xdc, s_a);
      /* $F2FB LDA */ s_a = update_nz(peek_zpg(0x7a));
      /* $F2FD STA */ poke_zpg(0xdd, s_a);
      /* $F2FF LDA */ s_a = update_nz(peek_zpg(0xf4));
      /* $F301 STA */ poke_zpg(0xb8, s_a);
      /* $F303 LDA */ s_a = update_nz(peek_zpg(0xf5));
      /* $F305 STA */ poke_zpg(0xb9, s_a);
      /* $F307 LDA */ s_a = update_nz(peek_zpg(0xf6));
      /* $F309 STA */ poke_zpg(0x75, s_a);
      /* $F30B LDA */ s_a = update_nz(peek_zpg(0xf7));
      /* $F30D STA */ poke_zpg(0x76, s_a);
      /* $F30F JSR */ push16(0xf311), s_pc = 0x00b7;
      branchTarget = true;
      break;
    case 0xf312: // [$F312..$F314]    3 bytes
      CYCLES(0xf312, 6);
      /* $F312 JSR */ push16(0xf314), s_pc = 0xd93e;
      branchTarget = true;
      break;
    case 0xf315: // [$F315..$F317]    3 bytes
      CYCLES(0xf315, 6);
      /* $F315 JMP */ s_pc = 0xd7d2;
      branchTarget = true;
      break;
    case 0xf399: // [$F399..$F39E]    6 bytes
      CYCLES(0xf399, 11);
      /* $F399 LDA */ s_a = update_nz(peek(0xc054));
      /* $F39C JMP */ s_pc = 0xfb39;
      branchTarget = true;
      break;
    case 0xf3e2: // [$F3E2..$F3FD]   28 bytes
      CYCLES(0xf3e2, 48);
      /* $F3E2 LDA */ s_a = update_nz(0x20);
      /* $F3E4 BIT */ tmp = peek(0xc054), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $F3E7 BIT */ tmp = peek(0xc053), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $F3EA STA */ poke_zpg(0xe6, s_a);
      /* $F3EC LDA */ s_a = update_nz(peek(0xc057));
      /* $F3EF LDA */ s_a = update_nz(peek(0xc050));
      /* $F3F2 LDA */ s_a = update_nz(0x00);
      /* $F3F4 STA */ poke_zpg(0x1c, s_a);
      /* $F3F6 LDA */ s_a = update_nz(peek_zpg(0xe6));
      /* $F3F8 STA */ poke_zpg(0x1b, s_a);
      /* $F3FA LDY */ s_y = update_nz(0x00);
      /* $F3FC STY */ poke_zpg(0x1a, s_y);
    case 0xf3fe: // [$F3FE..$F404]    7 bytes
      CYCLES(0xf3fe, 12);
      /* $F3FE LDA */ s_a = update_nz(peek_zpg(0x1c));
      /* $F400 STA */ poke(peek16_zpg(0x1a) + s_y, s_a);
      /* $F402 JSR */ push16(0xf404), s_pc = 0xf47e;
      branchTarget = true;
      break;
    case 0xf405: // [$F405..$F407]    3 bytes
      CYCLES(0xf405, 6);
      /* $F405 INY */ s_y = update_nz(s_y + 1);
      /* $F406 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xf3fe : 0xf408;
      branchTarget = true;
      break;
    case 0xf408: // [$F408..$F40F]    8 bytes
      CYCLES(0xf408, 14);
      /* $F408 INC */ tmp16 = 0x1b, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $F40A LDA */ s_a = update_nz(peek_zpg(0x1b));
      /* $F40C AND */ s_a = update_nz(s_a & 0x1f);
      /* $F40E BNE */ s_pc = !(s_status & STATUS_Z) ? 0xf3fe : 0xf410;
      branchTarget = true;
      break;
    case 0xf410: // [$F410..$F410]    1 bytes
      CYCLES(0xf410, 2);
      /* $F410 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xf47e: // [$F47E..$F482]    5 bytes
      CYCLES(0xf47e, 9);
      /* $F47E ASL */ s_a = update_nzc(s_a << 1);
      /* $F47F CMP */ update_nz_inv_c(s_a - 0xc0);
      /* $F481 BPL */ s_pc = !(s_status & STATUS_N) ? 0xf489 : 0xf483;
      branchTarget = true;
      break;
    case 0xf483: // [$F483..$F488]    6 bytes
      CYCLES(0xf483, 11);
      /* $F483 LDA */ s_a = update_nz(peek_zpg(0x1c));
      /* $F485 EOR */ s_a = update_nz(s_a ^ 0x7f);
      /* $F487 STA */ poke_zpg(0x1c, s_a);
    case 0xf489: // [$F489..$F489]    1 bytes
      CYCLES(0xf489, 2);
      /* $F489 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xf800: // [$F800..$F804]    5 bytes
      CYCLES(0xf800, 9);
      /* $F800 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $F801 PHP */ push8(s_status | STATUS_B);
      /* $F802 JSR */ push16(0xf804), s_pc = 0xf847;
      branchTarget = true;
      break;
    case 0xf805: // [$F805..$F809]    5 bytes
      CYCLES(0xf805, 9);
      /* $F805 PLP */ s_status = pop8() & ~STATUS_B;
      /* $F806 LDA */ s_a = update_nz(0x0f);
      /* $F808 BCC */ s_pc = !(s_status & STATUS_C) ? 0xf80c : 0xf80a;
      branchTarget = true;
      break;
    case 0xf80a: // [$F80A..$F80B]    2 bytes
      CYCLES(0xf80a, 4);
      /* $F80A ADC */ tmp = 0xe0, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0xf80c: // [$F80C..$F818]   13 bytes
      CYCLES(0xf80c, 23);
      /* $F80C STA */ poke_zpg(0x2e, s_a);
      /* $F80E LDA */ s_a = update_nz(peek(peek16_zpg(0x26) + s_y));
      /* $F810 EOR */ s_a = update_nz(s_a ^ peek_zpg(0x30));
      /* $F812 AND */ s_a = update_nz(s_a & peek_zpg(0x2e));
      /* $F814 EOR */ s_a = update_nz(s_a ^ peek(peek16_zpg(0x26) + s_y));
      /* $F816 STA */ poke(peek16_zpg(0x26) + s_y, s_a);
      /* $F818 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xf826: // [$F826..$F827]    2 bytes
      CYCLES(0xf826, 4);
      /* $F826 ADC */ tmp = 0x01, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0xf828: // [$F828..$F82B]    4 bytes
      CYCLES(0xf828, 7);
      /* $F828 PHA */ push8(s_a);
      /* $F829 JSR */ push16(0xf82b), s_pc = 0xf800;
      branchTarget = true;
      break;
    case 0xf82c: // [$F82C..$F830]    5 bytes
      CYCLES(0xf82c, 9);
      /* $F82C PLA */ s_a = update_nz(pop8());
      /* $F82D CMP */ update_nz_inv_c(s_a - peek_zpg(0x2d));
      /* $F82F BCC */ s_pc = !(s_status & STATUS_C) ? 0xf826 : 0xf831;
      branchTarget = true;
      break;
    case 0xf831: // [$F831..$F831]    1 bytes
      CYCLES(0xf831, 2);
      /* $F831 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xf836: // [$F836..$F83B]    6 bytes
      CYCLES(0xf836, 11);
      /* $F836 LDY */ s_y = update_nz(0x27);
      /* $F838 STY */ poke_zpg(0x2d, s_y);
      /* $F83A LDY */ s_y = update_nz(0x27);
    case 0xf83c: // [$F83C..$F842]    7 bytes
      CYCLES(0xf83c, 12);
      /* $F83C LDA */ s_a = update_nz(0x00);
      /* $F83E STA */ poke_zpg(0x30, s_a);
      /* $F840 JSR */ push16(0xf842), s_pc = 0xf828;
      branchTarget = true;
      break;
    case 0xf843: // [$F843..$F845]    3 bytes
      CYCLES(0xf843, 6);
      /* $F843 DEY */ s_y = update_nz(s_y - 1);
      /* $F844 BPL */ s_pc = !(s_status & STATUS_N) ? 0xf83c : 0xf846;
      branchTarget = true;
      break;
    case 0xf846: // [$F846..$F846]    1 bytes
      CYCLES(0xf846, 2);
      /* $F846 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xf847: // [$F847..$F853]   13 bytes
      CYCLES(0xf847, 23);
      /* $F847 PHA */ push8(s_a);
      /* $F848 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $F849 AND */ s_a = update_nz(s_a & 0x03);
      /* $F84B ORA */ s_a = update_nz(s_a | 0x04);
      /* $F84D STA */ poke_zpg(0x27, s_a);
      /* $F84F PLA */ s_a = update_nz(pop8());
      /* $F850 AND */ s_a = update_nz(s_a & 0x18);
      /* $F852 BCC */ s_pc = !(s_status & STATUS_C) ? 0xf856 : 0xf854;
      branchTarget = true;
      break;
    case 0xf854: // [$F854..$F855]    2 bytes
      CYCLES(0xf854, 4);
      /* $F854 ADC */ tmp = 0x7f, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0xf856: // [$F856..$F85E]    9 bytes
      CYCLES(0xf856, 16);
      /* $F856 STA */ poke_zpg(0x26, s_a);
      /* $F858 ASL */ s_a = update_nzc(s_a << 1);
      /* $F859 ASL */ s_a = update_nzc(s_a << 1);
      /* $F85A ORA */ s_a = update_nz(s_a | peek_zpg(0x26));
      /* $F85C STA */ poke_zpg(0x26, s_a);
      /* $F85E RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xf871: // [$F871..$F875]    5 bytes
      CYCLES(0xf871, 9);
      /* $F871 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $F872 PHP */ push8(s_status | STATUS_B);
      /* $F873 JSR */ push16(0xf875), s_pc = 0xf847;
      branchTarget = true;
      break;
    case 0xf876: // [$F876..$F87A]    5 bytes
      CYCLES(0xf876, 9);
      /* $F876 LDA */ s_a = update_nz(peek(peek16_zpg(0x26) + s_y));
      /* $F878 PLP */ s_status = pop8() & ~STATUS_B;
      /* $F879 BCC */ s_pc = !(s_status & STATUS_C) ? 0xf87f : 0xf87b;
      branchTarget = true;
      break;
    case 0xf87b: // [$F87B..$F87E]    4 bytes
      CYCLES(0xf87b, 7);
      /* $F87B LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $F87C LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $F87D LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $F87E LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
    case 0xf87f: // [$F87F..$F881]    3 bytes
      CYCLES(0xf87f, 6);
      /* $F87F AND */ s_a = update_nz(s_a & 0x0f);
      /* $F881 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xf940: // [$F940..$F943]    4 bytes
      CYCLES(0xf940, 7);
      /* $F940 TYA */ s_a = update_nz(s_y);
      /* $F941 JSR */ push16(0xf943), s_pc = 0xfdda;
      branchTarget = true;
      break;
    case 0xf944: // [$F944..$F947]    4 bytes
      CYCLES(0xf944, 7);
      /* $F944 TXA */ s_a = update_nz(s_x);
      /* $F945 JMP */ s_pc = 0xfdda;
      branchTarget = true;
      break;
    case 0xfa62: // [$FA62..$FA65]    4 bytes
      CYCLES(0xfa62, 7);
      /* $FA62 CLD */ s_status &= ~STATUS_D;
      /* $FA63 JSR */ push16(0xfa65), s_pc = 0xfe84;
      branchTarget = true;
      break;
    case 0xfa66: // [$FA66..$FA68]    3 bytes
      CYCLES(0xfa66, 6);
      /* $FA66 JSR */ push16(0xfa68), s_pc = 0xfb2f;
      branchTarget = true;
      break;
    case 0xfa69: // [$FA69..$FA6B]    3 bytes
      CYCLES(0xfa69, 6);
      /* $FA69 JSR */ push16(0xfa6b), s_pc = 0xfe93;
      branchTarget = true;
      break;
    case 0xfa6c: // [$FA6C..$FA6E]    3 bytes
      CYCLES(0xfa6c, 6);
      /* $FA6C JSR */ push16(0xfa6e), s_pc = 0xfe89;
      branchTarget = true;
      break;
    case 0xfa6f: // [$FA6F..$FA84]   22 bytes
      CYCLES(0xfa6f, 38);
      /* $FA6F LDA */ s_a = update_nz(peek(0xc058));
      /* $FA72 LDA */ s_a = update_nz(peek(0xc05a));
      /* $FA75 LDA */ s_a = update_nz(peek(0xc05d));
      /* $FA78 LDA */ s_a = update_nz(peek(0xc05f));
      /* $FA7B LDA */ s_a = update_nz(peek(0xcfff));
      /* $FA7E BIT */ tmp = peek(0xc010), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $FA81 CLD */ s_status &= ~STATUS_D;
      /* $FA82 JSR */ push16(0xfa84), s_pc = 0xff3a;
      branchTarget = true;
      break;
    case 0xfa85: // [$FA85..$FA8E]   10 bytes
      CYCLES(0xfa85, 18);
      /* $FA85 LDA */ s_a = update_nz(peek(0x03f3));
      /* $FA88 EOR */ s_a = update_nz(s_a ^ 0xa5);
      /* $FA8A CMP */ update_nz_inv_c(s_a - peek(0x03f4));
      /* $FA8D BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfaa6 : 0xfa8f;
      branchTarget = true;
      break;
    case 0xfa8f: // [$FA8F..$FA93]    5 bytes
      CYCLES(0xfa8f, 9);
      /* $FA8F LDA */ s_a = update_nz(peek(0x03f2));
      /* $FA92 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfaa3 : 0xfa94;
      branchTarget = true;
      break;
    case 0xfa94: // [$FA94..$FA9A]    7 bytes
      CYCLES(0xfa94, 12);
      /* $FA94 LDA */ s_a = update_nz(0xe0);
      /* $FA96 CMP */ update_nz_inv_c(s_a - peek(0x03f3));
      /* $FA99 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfaa3 : 0xfa9b;
      branchTarget = true;
      break;
    case 0xfa9b: // [$FA9B..$FAA2]    8 bytes
      CYCLES(0xfa9b, 14);
      /* $FA9B LDY */ s_y = update_nz(0x03);
      /* $FA9D STY */ poke(0x03f2, s_y);
      /* $FAA0 JMP */ s_pc = 0xe000;
      branchTarget = true;
      break;
    case 0xfaa3: // [$FAA3..$FAA5]    3 bytes
      CYCLES(0xfaa3, 6);
      /* $FAA3 JMP */ s_pc = peek16(0x03f2);
      branchTarget = true;
      break;
    case 0xfaa6: // [$FAA6..$FAA8]    3 bytes
      CYCLES(0xfaa6, 6);
      /* $FAA6 JSR */ push16(0xfaa8), s_pc = 0xfb60;
      branchTarget = true;
      break;
    case 0xfaa9: // [$FAA9..$FAAA]    2 bytes
      CYCLES(0xfaa9, 4);
      /* $FAA9 LDX */ s_x = update_nz(0x05);
    case 0xfaab: // [$FAAB..$FAB3]    9 bytes
      CYCLES(0xfaab, 16);
      /* $FAAB LDA */ s_a = update_nz(peek(0xfafc + s_x));
      /* $FAAE STA */ poke(0x03ef + s_x, s_a);
      /* $FAB1 DEX */ s_x = update_nz(s_x - 1);
      /* $FAB2 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfaab : 0xfab4;
      branchTarget = true;
      break;
    case 0xfab4: // [$FAB4..$FAB9]    6 bytes
      CYCLES(0xfab4, 11);
      /* $FAB4 LDA */ s_a = update_nz(0xc8);
      /* $FAB6 STX */ poke_zpg(0x00, s_x);
      /* $FAB8 STA */ poke_zpg(0x01, s_a);
    case 0xfaba: // [$FABA..$FAC3]   10 bytes
      CYCLES(0xfaba, 18);
      /* $FABA LDY */ s_y = update_nz(0x07);
      /* $FABC DEC */ tmp16 = 0x01, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $FABE LDA */ s_a = update_nz(peek_zpg(0x01));
      /* $FAC0 CMP */ update_nz_inv_c(s_a - 0xc0);
      /* $FAC2 BEQ */ s_pc = s_status & STATUS_Z ? 0xfa9b : 0xfac4;
      branchTarget = true;
      break;
    case 0xfac4: // [$FAC4..$FAC6]    3 bytes
      CYCLES(0xfac4, 6);
      /* $FAC4 STA */ poke(0x07f8, s_a);
    case 0xfac7: // [$FAC7..$FACD]    7 bytes
      CYCLES(0xfac7, 12);
      /* $FAC7 LDA */ s_a = update_nz(peek(peek16_zpg(0x00) + s_y));
      /* $FAC9 CMP */ update_nz_inv_c(s_a - peek(0xfb01 + s_y));
      /* $FACC BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfaba : 0xface;
      branchTarget = true;
      break;
    case 0xface: // [$FACE..$FAD1]    4 bytes
      CYCLES(0xface, 7);
      /* $FACE DEY */ s_y = update_nz(s_y - 1);
      /* $FACF DEY */ s_y = update_nz(s_y - 1);
      /* $FAD0 BPL */ s_pc = !(s_status & STATUS_N) ? 0xfac7 : 0xfad2;
      branchTarget = true;
      break;
    case 0xfad2: // [$FAD2..$FAD4]    3 bytes
      CYCLES(0xfad2, 6);
      /* $FAD2 JMP */ s_pc = peek16(0x0000);
      branchTarget = true;
      break;
    case 0xfb2f: // [$FB2F..$FB38]   10 bytes
      CYCLES(0xfb2f, 18);
      /* $FB2F LDA */ s_a = update_nz(0x00);
      /* $FB31 STA */ poke_zpg(0x48, s_a);
      /* $FB33 LDA */ s_a = update_nz(peek(0xc056));
      /* $FB36 LDA */ s_a = update_nz(peek(0xc054));
    case 0xfb39: // [$FB39..$FB3F]    7 bytes
      CYCLES(0xfb39, 12);
      /* $FB39 LDA */ s_a = update_nz(peek(0xc051));
      /* $FB3C LDA */ s_a = update_nz(0x00);
      /* $FB3E BEQ */ s_pc = s_status & STATUS_Z ? 0xfb4b : 0xfb40;
      branchTarget = true;
      break;
    case 0xfb40: // [$FB40..$FB48]    9 bytes
      CYCLES(0xfb40, 16);
      /* $FB40 LDA */ s_a = update_nz(peek(0xc050));
      /* $FB43 LDA */ s_a = update_nz(peek(0xc053));
      /* $FB46 JSR */ push16(0xfb48), s_pc = 0xf836;
      branchTarget = true;
      break;
    case 0xfb49: // [$FB49..$FB4A]    2 bytes
      CYCLES(0xfb49, 4);
      /* $FB49 LDA */ s_a = update_nz(0x14);
    case 0xfb4b: // [$FB4B..$FB5F]   21 bytes
      CYCLES(0xfb4b, 36);
      /* $FB4B STA */ poke_zpg(0x22, s_a);
      /* $FB4D LDA */ s_a = update_nz(0x00);
      /* $FB4F STA */ poke_zpg(0x20, s_a);
      /* $FB51 LDA */ s_a = update_nz(0x28);
      /* $FB53 STA */ poke_zpg(0x21, s_a);
      /* $FB55 LDA */ s_a = update_nz(0x18);
      /* $FB57 STA */ poke_zpg(0x23, s_a);
      /* $FB59 LDA */ s_a = update_nz(0x17);
      /* $FB5B STA */ poke_zpg(0x25, s_a);
      /* $FB5D JMP */ s_pc = 0xfc22;
      branchTarget = true;
      break;
    case 0xfb60: // [$FB60..$FB62]    3 bytes
      CYCLES(0xfb60, 6);
      /* $FB60 JSR */ push16(0xfb62), s_pc = 0xfc58;
      branchTarget = true;
      break;
    case 0xfb63: // [$FB63..$FB64]    2 bytes
      CYCLES(0xfb63, 4);
      /* $FB63 LDY */ s_y = update_nz(0x08);
    case 0xfb65: // [$FB65..$FB6D]    9 bytes
      CYCLES(0xfb65, 16);
      /* $FB65 LDA */ s_a = update_nz(peek(0xfb08 + s_y));
      /* $FB68 STA */ poke(0x040e + s_y, s_a);
      /* $FB6B DEY */ s_y = update_nz(s_y - 1);
      /* $FB6C BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfb65 : 0xfb6e;
      branchTarget = true;
      break;
    case 0xfb6e: // [$FB6E..$FB6E]    1 bytes
      CYCLES(0xfb6e, 2);
      /* $FB6E RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xfb78: // [$FB78..$FB7B]    4 bytes
      CYCLES(0xfb78, 7);
      /* $FB78 CMP */ update_nz_inv_c(s_a - 0x8d);
      /* $FB7A BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfb94 : 0xfb7c;
      branchTarget = true;
      break;
    case 0xfb7c: // [$FB7C..$FB80]    5 bytes
      CYCLES(0xfb7c, 9);
      /* $FB7C LDY */ s_y = update_nz(peek(0xc000));
      /* $FB7F BPL */ s_pc = !(s_status & STATUS_N) ? 0xfb94 : 0xfb81;
      branchTarget = true;
      break;
    case 0xfb81: // [$FB81..$FB84]    4 bytes
      CYCLES(0xfb81, 7);
      /* $FB81 CPY */ update_nz_inv_c(s_y - 0x93);
      /* $FB83 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfb94 : 0xfb85;
      branchTarget = true;
      break;
    case 0xfb85: // [$FB85..$FB87]    3 bytes
      CYCLES(0xfb85, 6);
      /* $FB85 BIT */ tmp = peek(0xc010), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
    case 0xfb88: // [$FB88..$FB8C]    5 bytes
      CYCLES(0xfb88, 9);
      /* $FB88 LDY */ s_y = update_nz(peek(0xc000));
      /* $FB8B BPL */ s_pc = !(s_status & STATUS_N) ? 0xfb88 : 0xfb8d;
      branchTarget = true;
      break;
    case 0xfb8d: // [$FB8D..$FB90]    4 bytes
      CYCLES(0xfb8d, 7);
      /* $FB8D CPY */ update_nz_inv_c(s_y - 0x83);
      /* $FB8F BEQ */ s_pc = s_status & STATUS_Z ? 0xfb94 : 0xfb91;
      branchTarget = true;
      break;
    case 0xfb91: // [$FB91..$FB93]    3 bytes
      CYCLES(0xfb91, 6);
      /* $FB91 BIT */ tmp = peek(0xc010), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
    case 0xfb94: // [$FB94..$FB96]    3 bytes
      CYCLES(0xfb94, 6);
      /* $FB94 JMP */ s_pc = 0xfbfd;
      branchTarget = true;
      break;
    case 0xfb97: // [$FB97..$FB9A]    4 bytes
      CYCLES(0xfb97, 7);
      /* $FB97 SEC */ s_status |= STATUS_C;
      /* $FB98 JMP */ s_pc = 0xfc2c;
      branchTarget = true;
      break;
    case 0xfb9b: // [$FB9B..$FBA1]    7 bytes
      CYCLES(0xfb9b, 12);
      /* $FB9B TAY */ s_y = update_nz(s_a);
      /* $FB9C LDA */ s_a = update_nz(peek(0xfa48 + s_y));
      /* $FB9F JSR */ push16(0xfba1), s_pc = 0xfb97;
      branchTarget = true;
      break;
    case 0xfba2: // [$FBA2..$FBA4]    3 bytes
      CYCLES(0xfba2, 6);
      /* $FBA2 JSR */ push16(0xfba4), s_pc = 0xfd0c;
      branchTarget = true;
      break;
    case 0xfba5: // [$FBA5..$FBA8]    4 bytes
      CYCLES(0xfba5, 7);
      /* $FBA5 CMP */ update_nz_inv_c(s_a - 0xce);
      /* $FBA7 BCS */ s_pc = s_status & STATUS_C ? 0xfb97 : 0xfba9;
      branchTarget = true;
      break;
    case 0xfba9: // [$FBA9..$FBAC]    4 bytes
      CYCLES(0xfba9, 7);
      /* $FBA9 CMP */ update_nz_inv_c(s_a - 0xc9);
      /* $FBAB BCC */ s_pc = !(s_status & STATUS_C) ? 0xfb97 : 0xfbad;
      branchTarget = true;
      break;
    case 0xfbad: // [$FBAD..$FBB0]    4 bytes
      CYCLES(0xfbad, 7);
      /* $FBAD CMP */ update_nz_inv_c(s_a - 0xcc);
      /* $FBAF BEQ */ s_pc = s_status & STATUS_Z ? 0xfb97 : 0xfbb1;
      branchTarget = true;
      break;
    case 0xfbb1: // [$FBB1..$FBB2]    2 bytes
      CYCLES(0xfbb1, 4);
      /* $FBB1 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfb9b : 0xfbb3;
      branchTarget = true;
      break;
    case 0xfbb3: // [$FBB3..$FBC0]   14 bytes
      CYCLES(0xfbb3, 24);
      /* $FBB3 NOP */ (void)0;
      /* $FBB4 NOP */ (void)0;
      /* $FBB5 NOP */ (void)0;
      /* $FBB6 NOP */ (void)0;
      /* $FBB7 NOP */ (void)0;
      /* $FBB8 NOP */ (void)0;
      /* $FBB9 NOP */ (void)0;
      /* $FBBA NOP */ (void)0;
      /* $FBBB NOP */ (void)0;
      /* $FBBC NOP */ (void)0;
      /* $FBBD NOP */ (void)0;
      /* $FBBE NOP */ (void)0;
      /* $FBBF NOP */ (void)0;
      /* $FBC0 NOP */ (void)0;
    case 0xfbc1: // [$FBC1..$FBCD]   13 bytes
      CYCLES(0xfbc1, 23);
      /* $FBC1 PHA */ push8(s_a);
      /* $FBC2 LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $FBC3 AND */ s_a = update_nz(s_a & 0x03);
      /* $FBC5 ORA */ s_a = update_nz(s_a | 0x04);
      /* $FBC7 STA */ poke_zpg(0x29, s_a);
      /* $FBC9 PLA */ s_a = update_nz(pop8());
      /* $FBCA AND */ s_a = update_nz(s_a & 0x18);
      /* $FBCC BCC */ s_pc = !(s_status & STATUS_C) ? 0xfbd0 : 0xfbce;
      branchTarget = true;
      break;
    case 0xfbce: // [$FBCE..$FBCF]    2 bytes
      CYCLES(0xfbce, 4);
      /* $FBCE ADC */ tmp = 0x7f, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0xfbd0: // [$FBD0..$FBD8]    9 bytes
      CYCLES(0xfbd0, 16);
      /* $FBD0 STA */ poke_zpg(0x28, s_a);
      /* $FBD2 ASL */ s_a = update_nzc(s_a << 1);
      /* $FBD3 ASL */ s_a = update_nzc(s_a << 1);
      /* $FBD4 ORA */ s_a = update_nz(s_a | peek_zpg(0x28));
      /* $FBD6 STA */ poke_zpg(0x28, s_a);
      /* $FBD8 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xfbd9: // [$FBD9..$FBDC]    4 bytes
      CYCLES(0xfbd9, 7);
      /* $FBD9 CMP */ update_nz_inv_c(s_a - 0x87);
      /* $FBDB BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfbef : 0xfbdd;
      branchTarget = true;
      break;
    case 0xfbdd: // [$FBDD..$FBE1]    5 bytes
      CYCLES(0xfbdd, 9);
      /* $FBDD LDA */ s_a = update_nz(0x40);
      /* $FBDF JSR */ push16(0xfbe1), s_pc = 0xfca8;
      branchTarget = true;
      break;
    case 0xfbe2: // [$FBE2..$FBE3]    2 bytes
      CYCLES(0xfbe2, 4);
      /* $FBE2 LDY */ s_y = update_nz(0xc0);
    case 0xfbe4: // [$FBE4..$FBE8]    5 bytes
      CYCLES(0xfbe4, 9);
      /* $FBE4 LDA */ s_a = update_nz(0x0c);
      /* $FBE6 JSR */ push16(0xfbe8), s_pc = 0xfca8;
      branchTarget = true;
      break;
    case 0xfbe9: // [$FBE9..$FBEE]    6 bytes
      CYCLES(0xfbe9, 11);
      /* $FBE9 LDA */ s_a = update_nz(peek(0xc030));
      /* $FBEC DEY */ s_y = update_nz(s_y - 1);
      /* $FBED BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfbe4 : 0xfbef;
      branchTarget = true;
      break;
    case 0xfbef: // [$FBEF..$FBEF]    1 bytes
      CYCLES(0xfbef, 2);
      /* $FBEF RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xfbf0: // [$FBF0..$FBF3]    4 bytes
      CYCLES(0xfbf0, 7);
      /* $FBF0 LDY */ s_y = update_nz(peek_zpg(0x24));
      /* $FBF2 STA */ poke(peek16_zpg(0x28) + s_y, s_a);
    case 0xfbf4: // [$FBF4..$FBFB]    8 bytes
      CYCLES(0xfbf4, 14);
      /* $FBF4 INC */ tmp16 = 0x24, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $FBF6 LDA */ s_a = update_nz(peek_zpg(0x24));
      /* $FBF8 CMP */ update_nz_inv_c(s_a - peek_zpg(0x21));
      /* $FBFA BCS */ s_pc = s_status & STATUS_C ? 0xfc62 : 0xfbfc;
      branchTarget = true;
      break;
    case 0xfbfc: // [$FBFC..$FBFC]    1 bytes
      CYCLES(0xfbfc, 2);
      /* $FBFC RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xfbfd: // [$FBFD..$FC00]    4 bytes
      CYCLES(0xfbfd, 7);
      /* $FBFD CMP */ update_nz_inv_c(s_a - 0xa0);
      /* $FBFF BCS */ s_pc = s_status & STATUS_C ? 0xfbf0 : 0xfc01;
      branchTarget = true;
      break;
    case 0xfc01: // [$FC01..$FC03]    3 bytes
      CYCLES(0xfc01, 6);
      /* $FC01 TAY */ s_y = update_nz(s_a);
      /* $FC02 BPL */ s_pc = !(s_status & STATUS_N) ? 0xfbf0 : 0xfc04;
      branchTarget = true;
      break;
    case 0xfc04: // [$FC04..$FC07]    4 bytes
      CYCLES(0xfc04, 7);
      /* $FC04 CMP */ update_nz_inv_c(s_a - 0x8d);
      /* $FC06 BEQ */ s_pc = s_status & STATUS_Z ? 0xfc62 : 0xfc08;
      branchTarget = true;
      break;
    case 0xfc08: // [$FC08..$FC0B]    4 bytes
      CYCLES(0xfc08, 7);
      /* $FC08 CMP */ update_nz_inv_c(s_a - 0x8a);
      /* $FC0A BEQ */ s_pc = s_status & STATUS_Z ? 0xfc66 : 0xfc0c;
      branchTarget = true;
      break;
    case 0xfc0c: // [$FC0C..$FC0F]    4 bytes
      CYCLES(0xfc0c, 7);
      /* $FC0C CMP */ update_nz_inv_c(s_a - 0x88);
      /* $FC0E BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfbd9 : 0xfc10;
      branchTarget = true;
      break;
    case 0xfc10: // [$FC10..$FC13]    4 bytes
      CYCLES(0xfc10, 7);
      /* $FC10 DEC */ tmp16 = 0x24, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $FC12 BPL */ s_pc = !(s_status & STATUS_N) ? 0xfbfc : 0xfc14;
      branchTarget = true;
      break;
    case 0xfc14: // [$FC14..$FC19]    6 bytes
      CYCLES(0xfc14, 11);
      /* $FC14 LDA */ s_a = update_nz(peek_zpg(0x21));
      /* $FC16 STA */ poke_zpg(0x24, s_a);
      /* $FC18 DEC */ tmp16 = 0x24, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
    case 0xfc1a: // [$FC1A..$FC1F]    6 bytes
      CYCLES(0xfc1a, 11);
      /* $FC1A LDA */ s_a = update_nz(peek_zpg(0x22));
      /* $FC1C CMP */ update_nz_inv_c(s_a - peek_zpg(0x25));
      /* $FC1E BCS */ s_pc = s_status & STATUS_C ? 0xfc2b : 0xfc20;
      branchTarget = true;
      break;
    case 0xfc20: // [$FC20..$FC21]    2 bytes
      CYCLES(0xfc20, 4);
      /* $FC20 DEC */ tmp16 = 0x25, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
    case 0xfc22: // [$FC22..$FC23]    2 bytes
      CYCLES(0xfc22, 4);
      /* $FC22 LDA */ s_a = update_nz(peek_zpg(0x25));
    case 0xfc24: // [$FC24..$FC26]    3 bytes
      CYCLES(0xfc24, 6);
      /* $FC24 JSR */ push16(0xfc26), s_pc = 0xfbc1;
      branchTarget = true;
      break;
    case 0xfc27: // [$FC27..$FC2A]    4 bytes
      CYCLES(0xfc27, 7);
      /* $FC27 ADC */ tmp = peek_zpg(0x20), s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $FC29 STA */ poke_zpg(0x28, s_a);
    case 0xfc2b: // [$FC2B..$FC2B]    1 bytes
      CYCLES(0xfc2b, 2);
      /* $FC2B RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xfc2c: // [$FC2C..$FC2F]    4 bytes
      CYCLES(0xfc2c, 7);
      /* $FC2C EOR */ s_a = update_nz(s_a ^ 0xc0);
      /* $FC2E BEQ */ s_pc = s_status & STATUS_Z ? 0xfc58 : 0xfc30;
      branchTarget = true;
      break;
    case 0xfc30: // [$FC30..$FC33]    4 bytes
      CYCLES(0xfc30, 7);
      /* $FC30 ADC */ tmp = 0xfd, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $FC32 BCC */ s_pc = !(s_status & STATUS_C) ? 0xfbf4 : 0xfc34;
      branchTarget = true;
      break;
    case 0xfc34: // [$FC34..$FC35]    2 bytes
      CYCLES(0xfc34, 4);
      /* $FC34 BEQ */ s_pc = s_status & STATUS_Z ? 0xfc10 : 0xfc36;
      branchTarget = true;
      break;
    case 0xfc36: // [$FC36..$FC39]    4 bytes
      CYCLES(0xfc36, 7);
      /* $FC36 ADC */ tmp = 0xfd, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $FC38 BCC */ s_pc = !(s_status & STATUS_C) ? 0xfc66 : 0xfc3a;
      branchTarget = true;
      break;
    case 0xfc3a: // [$FC3A..$FC3B]    2 bytes
      CYCLES(0xfc3a, 4);
      /* $FC3A BEQ */ s_pc = s_status & STATUS_Z ? 0xfc1a : 0xfc3c;
      branchTarget = true;
      break;
    case 0xfc3c: // [$FC3C..$FC3F]    4 bytes
      CYCLES(0xfc3c, 7);
      /* $FC3C ADC */ tmp = 0xfd, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $FC3E BCC */ s_pc = !(s_status & STATUS_C) ? 0xfc9c : 0xfc40;
      branchTarget = true;
      break;
    case 0xfc40: // [$FC40..$FC41]    2 bytes
      CYCLES(0xfc40, 4);
      /* $FC40 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfc2b : 0xfc42;
      branchTarget = true;
      break;
    case 0xfc42: // [$FC42..$FC45]    4 bytes
      CYCLES(0xfc42, 7);
      /* $FC42 LDY */ s_y = update_nz(peek_zpg(0x24));
      /* $FC44 LDA */ s_a = update_nz(peek_zpg(0x25));
    case 0xfc46: // [$FC46..$FC49]    4 bytes
      CYCLES(0xfc46, 7);
      /* $FC46 PHA */ push8(s_a);
      /* $FC47 JSR */ push16(0xfc49), s_pc = 0xfc24;
      branchTarget = true;
      break;
    case 0xfc4a: // [$FC4A..$FC4C]    3 bytes
      CYCLES(0xfc4a, 6);
      /* $FC4A JSR */ push16(0xfc4c), s_pc = 0xfc9e;
      branchTarget = true;
      break;
    case 0xfc4d: // [$FC4D..$FC55]    9 bytes
      CYCLES(0xfc4d, 16);
      /* $FC4D LDY */ s_y = update_nz(0x00);
      /* $FC4F PLA */ s_a = update_nz(pop8());
      /* $FC50 ADC */ tmp = 0x00, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $FC52 CMP */ update_nz_inv_c(s_a - peek_zpg(0x23));
      /* $FC54 BCC */ s_pc = !(s_status & STATUS_C) ? 0xfc46 : 0xfc56;
      branchTarget = true;
      break;
    case 0xfc56: // [$FC56..$FC57]    2 bytes
      CYCLES(0xfc56, 4);
      /* $FC56 BCS */ s_pc = s_status & STATUS_C ? 0xfc22 : 0xfc58;
      branchTarget = true;
      break;
    case 0xfc58: // [$FC58..$FC61]   10 bytes
      CYCLES(0xfc58, 18);
      /* $FC58 LDA */ s_a = update_nz(peek_zpg(0x22));
      /* $FC5A STA */ poke_zpg(0x25, s_a);
      /* $FC5C LDY */ s_y = update_nz(0x00);
      /* $FC5E STY */ poke_zpg(0x24, s_y);
      /* $FC60 BEQ */ s_pc = s_status & STATUS_Z ? 0xfc46 : 0xfc62;
      branchTarget = true;
      break;
    case 0xfc62: // [$FC62..$FC65]    4 bytes
      CYCLES(0xfc62, 7);
      /* $FC62 LDA */ s_a = update_nz(0x00);
      /* $FC64 STA */ poke_zpg(0x24, s_a);
    case 0xfc66: // [$FC66..$FC6D]    8 bytes
      CYCLES(0xfc66, 14);
      /* $FC66 INC */ tmp16 = 0x25, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $FC68 LDA */ s_a = update_nz(peek_zpg(0x25));
      /* $FC6A CMP */ update_nz_inv_c(s_a - peek_zpg(0x23));
      /* $FC6C BCC */ s_pc = !(s_status & STATUS_C) ? 0xfc24 : 0xfc6e;
      branchTarget = true;
      break;
    case 0xfc6e: // [$FC6E..$FC75]    8 bytes
      CYCLES(0xfc6e, 14);
      /* $FC6E DEC */ tmp16 = 0x25, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) - 1));
      /* $FC70 LDA */ s_a = update_nz(peek_zpg(0x22));
      /* $FC72 PHA */ push8(s_a);
      /* $FC73 JSR */ push16(0xfc75), s_pc = 0xfc24;
      branchTarget = true;
      break;
    case 0xfc76: // [$FC76..$FC87]   18 bytes
      CYCLES(0xfc76, 31);
      /* $FC76 LDA */ s_a = update_nz(peek_zpg(0x28));
      /* $FC78 STA */ poke_zpg(0x2a, s_a);
      /* $FC7A LDA */ s_a = update_nz(peek_zpg(0x29));
      /* $FC7C STA */ poke_zpg(0x2b, s_a);
      /* $FC7E LDY */ s_y = update_nz(peek_zpg(0x21));
      /* $FC80 DEY */ s_y = update_nz(s_y - 1);
      /* $FC81 PLA */ s_a = update_nz(pop8());
      /* $FC82 ADC */ tmp = 0x01, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
      /* $FC84 CMP */ update_nz_inv_c(s_a - peek_zpg(0x23));
      /* $FC86 BCS */ s_pc = s_status & STATUS_C ? 0xfc95 : 0xfc88;
      branchTarget = true;
      break;
    case 0xfc88: // [$FC88..$FC8B]    4 bytes
      CYCLES(0xfc88, 7);
      /* $FC88 PHA */ push8(s_a);
      /* $FC89 JSR */ push16(0xfc8b), s_pc = 0xfc24;
      branchTarget = true;
      break;
    case 0xfc8c: // [$FC8C..$FC92]    7 bytes
      CYCLES(0xfc8c, 12);
      /* $FC8C LDA */ s_a = update_nz(peek(peek16_zpg(0x28) + s_y));
      /* $FC8E STA */ poke(peek16_zpg(0x2a) + s_y, s_a);
      /* $FC90 DEY */ s_y = update_nz(s_y - 1);
      /* $FC91 BPL */ s_pc = !(s_status & STATUS_N) ? 0xfc8c : 0xfc93;
      branchTarget = true;
      break;
    case 0xfc93: // [$FC93..$FC94]    2 bytes
      CYCLES(0xfc93, 4);
      /* $FC93 BMI */ s_pc = s_status & STATUS_N ? 0xfc76 : 0xfc95;
      branchTarget = true;
      break;
    case 0xfc95: // [$FC95..$FC99]    5 bytes
      CYCLES(0xfc95, 9);
      /* $FC95 LDY */ s_y = update_nz(0x00);
      /* $FC97 JSR */ push16(0xfc99), s_pc = 0xfc9e;
      branchTarget = true;
      break;
    case 0xfc9a: // [$FC9A..$FC9B]    2 bytes
      CYCLES(0xfc9a, 4);
      /* $FC9A BCS */ s_pc = s_status & STATUS_C ? 0xfc22 : 0xfc9c;
      branchTarget = true;
      break;
    case 0xfc9c: // [$FC9C..$FC9D]    2 bytes
      CYCLES(0xfc9c, 4);
      /* $FC9C LDY */ s_y = update_nz(peek_zpg(0x24));
    case 0xfc9e: // [$FC9E..$FC9F]    2 bytes
      CYCLES(0xfc9e, 4);
      /* $FC9E LDA */ s_a = update_nz(0xa0);
    case 0xfca0: // [$FCA0..$FCA6]    7 bytes
      CYCLES(0xfca0, 12);
      /* $FCA0 STA */ poke(peek16_zpg(0x28) + s_y, s_a);
      /* $FCA2 INY */ s_y = update_nz(s_y + 1);
      /* $FCA3 CPY */ update_nz_inv_c(s_y - peek_zpg(0x21));
      /* $FCA5 BCC */ s_pc = !(s_status & STATUS_C) ? 0xfca0 : 0xfca7;
      branchTarget = true;
      break;
    case 0xfca7: // [$FCA7..$FCA7]    1 bytes
      CYCLES(0xfca7, 2);
      /* $FCA7 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xfca8: // [$FCA8..$FCA8]    1 bytes
      CYCLES(0xfca8, 2);
      /* $FCA8 SEC */ s_status |= STATUS_C;
    case 0xfca9: // [$FCA9..$FCA9]    1 bytes
      CYCLES(0xfca9, 2);
      /* $FCA9 PHA */ push8(s_a);
    case 0xfcaa: // [$FCAA..$FCAD]    4 bytes
      CYCLES(0xfcaa, 7);
      /* $FCAA SBC */ tmp = 0x01, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $FCAC BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfcaa : 0xfcae;
      branchTarget = true;
      break;
    case 0xfcae: // [$FCAE..$FCB2]    5 bytes
      CYCLES(0xfcae, 9);
      /* $FCAE PLA */ s_a = update_nz(pop8());
      /* $FCAF SBC */ tmp = 0x01, s_a = s_status & STATUS_D ? sbc_decimal(tmp) : update_nzv_inv_c(s_a - tmp - (~s_status & STATUS_C), s_a, ~tmp);
      /* $FCB1 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfca9 : 0xfcb3;
      branchTarget = true;
      break;
    case 0xfcb3: // [$FCB3..$FCB3]    1 bytes
      CYCLES(0xfcb3, 2);
      /* $FCB3 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xfd0c: // [$FD0C..$FD1A]   15 bytes
      CYCLES(0xfd0c, 26);
      /* $FD0C LDY */ s_y = update_nz(peek_zpg(0x24));
      /* $FD0E LDA */ s_a = update_nz(peek(peek16_zpg(0x28) + s_y));
      /* $FD10 PHA */ push8(s_a);
      /* $FD11 AND */ s_a = update_nz(s_a & 0x3f);
      /* $FD13 ORA */ s_a = update_nz(s_a | 0x40);
      /* $FD15 STA */ poke(peek16_zpg(0x28) + s_y, s_a);
      /* $FD17 PLA */ s_a = update_nz(pop8());
      /* $FD18 JMP */ s_pc = peek16(0x0038);
      branchTarget = true;
      break;
    case 0xfd1b: // [$FD1B..$FD1E]    4 bytes
      CYCLES(0xfd1b, 7);
      /* $FD1B INC */ tmp16 = 0x4e, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
      /* $FD1D BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfd21 : 0xfd1f;
      branchTarget = true;
      break;
    case 0xfd1f: // [$FD1F..$FD20]    2 bytes
      CYCLES(0xfd1f, 4);
      /* $FD1F INC */ tmp16 = 0x4f, poke_zpg(tmp16, update_nz(peek_zpg(tmp16) + 1));
    case 0xfd21: // [$FD21..$FD25]    5 bytes
      CYCLES(0xfd21, 9);
      /* $FD21 BIT */ tmp = peek(0xc000), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $FD24 BPL */ s_pc = !(s_status & STATUS_N) ? 0xfd1b : 0xfd26;
      branchTarget = true;
      break;
    case 0xfd26: // [$FD26..$FD2E]    9 bytes
      CYCLES(0xfd26, 16);
      /* $FD26 STA */ poke(peek16_zpg(0x28) + s_y, s_a);
      /* $FD28 LDA */ s_a = update_nz(peek(0xc000));
      /* $FD2B BIT */ tmp = peek(0xc010), s_status = (s_status & ~(0xC0 | STATUS_Z)) | (tmp & 0xC0) | (s_a & tmp ? 0 : STATUS_Z);
      /* $FD2E RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xfd2f: // [$FD2F..$FD31]    3 bytes
      CYCLES(0xfd2f, 6);
      /* $FD2F JSR */ push16(0xfd31), s_pc = 0xfd0c;
      branchTarget = true;
      break;
    case 0xfd32: // [$FD32..$FD34]    3 bytes
      CYCLES(0xfd32, 6);
      /* $FD32 JSR */ push16(0xfd34), s_pc = 0xfba5;
      branchTarget = true;
      break;
    case 0xfd35: // [$FD35..$FD37]    3 bytes
      CYCLES(0xfd35, 6);
      /* $FD35 JSR */ push16(0xfd37), s_pc = 0xfd0c;
      branchTarget = true;
      break;
    case 0xfd38: // [$FD38..$FD3B]    4 bytes
      CYCLES(0xfd38, 7);
      /* $FD38 CMP */ update_nz_inv_c(s_a - 0x9b);
      /* $FD3A BEQ */ s_pc = s_status & STATUS_Z ? 0xfd2f : 0xfd3c;
      branchTarget = true;
      break;
    case 0xfd3c: // [$FD3C..$FD3C]    1 bytes
      CYCLES(0xfd3c, 2);
      /* $FD3C RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xfd3d: // [$FD3D..$FD49]   13 bytes
      CYCLES(0xfd3d, 23);
      /* $FD3D LDA */ s_a = update_nz(peek_zpg(0x32));
      /* $FD3F PHA */ push8(s_a);
      /* $FD40 LDA */ s_a = update_nz(0xff);
      /* $FD42 STA */ poke_zpg(0x32, s_a);
      /* $FD44 LDA */ s_a = update_nz(peek(0x0200 + s_x));
      /* $FD47 JSR */ push16(0xfd49), s_pc = 0xfded;
      branchTarget = true;
      break;
    case 0xfd4a: // [$FD4A..$FD53]   10 bytes
      CYCLES(0xfd4a, 18);
      /* $FD4A PLA */ s_a = update_nz(pop8());
      /* $FD4B STA */ poke_zpg(0x32, s_a);
      /* $FD4D LDA */ s_a = update_nz(peek(0x0200 + s_x));
      /* $FD50 CMP */ update_nz_inv_c(s_a - 0x88);
      /* $FD52 BEQ */ s_pc = s_status & STATUS_Z ? 0xfd71 : 0xfd54;
      branchTarget = true;
      break;
    case 0xfd54: // [$FD54..$FD57]    4 bytes
      CYCLES(0xfd54, 7);
      /* $FD54 CMP */ update_nz_inv_c(s_a - 0x98);
      /* $FD56 BEQ */ s_pc = s_status & STATUS_Z ? 0xfd62 : 0xfd58;
      branchTarget = true;
      break;
    case 0xfd58: // [$FD58..$FD5B]    4 bytes
      CYCLES(0xfd58, 7);
      /* $FD58 CPX */ update_nz_inv_c(s_x - 0xf8);
      /* $FD5A BCC */ s_pc = !(s_status & STATUS_C) ? 0xfd5f : 0xfd5c;
      branchTarget = true;
      break;
    case 0xfd5c: // [$FD5C..$FD5E]    3 bytes
      CYCLES(0xfd5c, 6);
      /* $FD5C JSR */ push16(0xfd5e), s_pc = 0xff3a;
      branchTarget = true;
      break;
    case 0xfd5f: // [$FD5F..$FD61]    3 bytes
      CYCLES(0xfd5f, 6);
      /* $FD5F INX */ s_x = update_nz(s_x + 1);
      /* $FD60 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfd75 : 0xfd62;
      branchTarget = true;
      break;
    case 0xfd62: // [$FD62..$FD66]    5 bytes
      CYCLES(0xfd62, 9);
      /* $FD62 LDA */ s_a = update_nz(0xdc);
      /* $FD64 JSR */ push16(0xfd66), s_pc = 0xfded;
      branchTarget = true;
      break;
    case 0xfd67: // [$FD67..$FD69]    3 bytes
      CYCLES(0xfd67, 6);
      /* $FD67 JSR */ push16(0xfd69), s_pc = 0xfd8e;
      branchTarget = true;
      break;
    case 0xfd6a: // [$FD6A..$FD6E]    5 bytes
      CYCLES(0xfd6a, 9);
      /* $FD6A LDA */ s_a = update_nz(peek_zpg(0x33));
      /* $FD6C JSR */ push16(0xfd6e), s_pc = 0xfded;
      branchTarget = true;
      break;
    case 0xfd6f: // [$FD6F..$FD70]    2 bytes
      CYCLES(0xfd6f, 4);
      /* $FD6F LDX */ s_x = update_nz(0x01);
    case 0xfd71: // [$FD71..$FD73]    3 bytes
      CYCLES(0xfd71, 6);
      /* $FD71 TXA */ s_a = update_nz(s_x);
      /* $FD72 BEQ */ s_pc = s_status & STATUS_Z ? 0xfd67 : 0xfd74;
      branchTarget = true;
      break;
    case 0xfd74: // [$FD74..$FD74]    1 bytes
      CYCLES(0xfd74, 2);
      /* $FD74 DEX */ s_x = update_nz(s_x - 1);
    case 0xfd75: // [$FD75..$FD77]    3 bytes
      CYCLES(0xfd75, 6);
      /* $FD75 JSR */ push16(0xfd77), s_pc = 0xfd35;
      branchTarget = true;
      break;
    case 0xfd78: // [$FD78..$FD7B]    4 bytes
      CYCLES(0xfd78, 7);
      /* $FD78 CMP */ update_nz_inv_c(s_a - 0x95);
      /* $FD7A BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfd7e : 0xfd7c;
      branchTarget = true;
      break;
    case 0xfd7c: // [$FD7C..$FD7D]    2 bytes
      CYCLES(0xfd7c, 4);
      /* $FD7C LDA */ s_a = update_nz(peek(peek16_zpg(0x28) + s_y));
    case 0xfd7e: // [$FD7E..$FD81]    4 bytes
      CYCLES(0xfd7e, 7);
      /* $FD7E CMP */ update_nz_inv_c(s_a - 0xe0);
      /* $FD80 BCC */ s_pc = !(s_status & STATUS_C) ? 0xfd84 : 0xfd82;
      branchTarget = true;
      break;
    case 0xfd82: // [$FD82..$FD83]    2 bytes
      CYCLES(0xfd82, 4);
      /* $FD82 AND */ s_a = update_nz(s_a & 0xdf);
    case 0xfd84: // [$FD84..$FD8A]    7 bytes
      CYCLES(0xfd84, 12);
      /* $FD84 STA */ poke(0x0200 + s_x, s_a);
      /* $FD87 CMP */ update_nz_inv_c(s_a - 0x8d);
      /* $FD89 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfd3d : 0xfd8b;
      branchTarget = true;
      break;
    case 0xfd8b: // [$FD8B..$FD8D]    3 bytes
      CYCLES(0xfd8b, 6);
      /* $FD8B JSR */ push16(0xfd8d), s_pc = 0xfc9c;
      branchTarget = true;
      break;
    case 0xfd8e: // [$FD8E..$FD91]    4 bytes
      CYCLES(0xfd8e, 7);
      /* $FD8E LDA */ s_a = update_nz(0x8d);
      /* $FD90 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfded : 0xfd92;
      branchTarget = true;
      break;
    case 0xfd92: // [$FD92..$FD98]    7 bytes
      CYCLES(0xfd92, 12);
      /* $FD92 LDY */ s_y = update_nz(peek_zpg(0x3d));
      /* $FD94 LDX */ s_x = update_nz(peek_zpg(0x3c));
      /* $FD96 JSR */ push16(0xfd98), s_pc = 0xfd8e;
      branchTarget = true;
      break;
    case 0xfd99: // [$FD99..$FD9B]    3 bytes
      CYCLES(0xfd99, 6);
      /* $FD99 JSR */ push16(0xfd9b), s_pc = 0xf940;
      branchTarget = true;
      break;
    case 0xfd9c: // [$FD9C..$FDA2]    7 bytes
      CYCLES(0xfd9c, 12);
      /* $FD9C LDY */ s_y = update_nz(0x00);
      /* $FD9E LDA */ s_a = update_nz(0xad);
      /* $FDA0 JMP */ s_pc = 0xfded;
      branchTarget = true;
      break;
    case 0xfdda: // [$FDDA..$FDE1]    8 bytes
      CYCLES(0xfdda, 14);
      /* $FDDA PHA */ push8(s_a);
      /* $FDDB LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $FDDC LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $FDDD LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $FDDE LSR */ set_c_to_bit0(s_a), s_a = update_nz(s_a >> 1);
      /* $FDDF JSR */ push16(0xfde1), s_pc = 0xfde5;
      branchTarget = true;
      break;
    case 0xfde2: // [$FDE2..$FDE4]    3 bytes
      CYCLES(0xfde2, 6);
      /* $FDE2 PLA */ s_a = update_nz(pop8());
      /* $FDE3 AND */ s_a = update_nz(s_a & 0x0f);
    case 0xfde5: // [$FDE5..$FDEA]    6 bytes
      CYCLES(0xfde5, 11);
      /* $FDE5 ORA */ s_a = update_nz(s_a | 0xb0);
      /* $FDE7 CMP */ update_nz_inv_c(s_a - 0xba);
      /* $FDE9 BCC */ s_pc = !(s_status & STATUS_C) ? 0xfded : 0xfdeb;
      branchTarget = true;
      break;
    case 0xfdeb: // [$FDEB..$FDEC]    2 bytes
      CYCLES(0xfdeb, 4);
      /* $FDEB ADC */ tmp = 0x06, s_a = s_status & STATUS_D ? adc_decimal(tmp) : update_nzvc(s_a + tmp + (s_status & STATUS_C), s_a, tmp);
    case 0xfded: // [$FDED..$FDEF]    3 bytes
      CYCLES(0xfded, 6);
      /* $FDED JMP */ s_pc = peek16(0x0036);
      branchTarget = true;
      break;
    case 0xfdf0: // [$FDF0..$FDF3]    4 bytes
      CYCLES(0xfdf0, 7);
      /* $FDF0 CMP */ update_nz_inv_c(s_a - 0xa0);
      /* $FDF2 BCC */ s_pc = !(s_status & STATUS_C) ? 0xfdf6 : 0xfdf4;
      branchTarget = true;
      break;
    case 0xfdf4: // [$FDF4..$FDF5]    2 bytes
      CYCLES(0xfdf4, 4);
      /* $FDF4 AND */ s_a = update_nz(s_a & peek_zpg(0x32));
    case 0xfdf6: // [$FDF6..$FDFB]    6 bytes
      CYCLES(0xfdf6, 11);
      /* $FDF6 STY */ poke_zpg(0x35, s_y);
      /* $FDF8 PHA */ push8(s_a);
      /* $FDF9 JSR */ push16(0xfdfb), s_pc = 0xfb78;
      branchTarget = true;
      break;
    case 0xfdfc: // [$FDFC..$FDFF]    4 bytes
      CYCLES(0xfdfc, 7);
      /* $FDFC PLA */ s_a = update_nz(pop8());
      /* $FDFD LDY */ s_y = update_nz(peek_zpg(0x35));
      /* $FDFF RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xfe84: // [$FE84..$FE88]    5 bytes
      CYCLES(0xfe84, 9);
      /* $FE84 LDY */ s_y = update_nz(0xff);
      /* $FE86 STY */ poke_zpg(0x32, s_y);
      /* $FE88 RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xfe89: // [$FE89..$FE92]   10 bytes
      CYCLES(0xfe89, 18);
      /* $FE89 LDA */ s_a = update_nz(0x00);
      /* $FE8B STA */ poke_zpg(0x3e, s_a);
      /* $FE8D LDX */ s_x = update_nz(0x38);
      /* $FE8F LDY */ s_y = update_nz(0x1b);
      /* $FE91 BNE */ s_pc = !(s_status & STATUS_Z) ? 0xfe9b : 0xfe93;
      branchTarget = true;
      break;
    case 0xfe93: // [$FE93..$FE9A]    8 bytes
      CYCLES(0xfe93, 14);
      /* $FE93 LDA */ s_a = update_nz(0x00);
      /* $FE95 STA */ poke_zpg(0x3e, s_a);
      /* $FE97 LDX */ s_x = update_nz(0x36);
      /* $FE99 LDY */ s_y = update_nz(0xf0);
    case 0xfe9b: // [$FE9B..$FEA0]    6 bytes
      CYCLES(0xfe9b, 11);
      /* $FE9B LDA */ s_a = update_nz(peek_zpg(0x3e));
      /* $FE9D AND */ s_a = update_nz(s_a & 0x0f);
      /* $FE9F BEQ */ s_pc = s_status & STATUS_Z ? 0xfea7 : 0xfea1;
      branchTarget = true;
      break;
    case 0xfea1: // [$FEA1..$FEA6]    6 bytes
      CYCLES(0xfea1, 11);
      /* $FEA1 ORA */ s_a = update_nz(s_a | 0xc0);
      /* $FEA3 LDY */ s_y = update_nz(0x00);
      /* $FEA5 BEQ */ s_pc = s_status & STATUS_Z ? 0xfea9 : 0xfea7;
      branchTarget = true;
      break;
    case 0xfea7: // [$FEA7..$FEA8]    2 bytes
      CYCLES(0xfea7, 4);
      /* $FEA7 LDA */ s_a = update_nz(0xfd);
    case 0xfea9: // [$FEA9..$FEAD]    5 bytes
      CYCLES(0xfea9, 9);
      /* $FEA9 STY */ poke_zpg((uint8_t)(0x00 + s_x), s_y);
      /* $FEAB STA */ poke_zpg((uint8_t)(0x01 + s_x), s_a);
      /* $FEAD RTS */ s_pc = pop16() + 1;
      branchTarget = true;
      break;
    case 0xff3a: // [$FF3A..$FF3E]    5 bytes
      CYCLES(0xff3a, 9);
      /* $FF3A LDA */ s_a = update_nz(0x87);
      /* $FF3C JMP */ s_pc = 0xfded;
      branchTarget = true;
      break;
    default:
      fprintf(stderr, "Unknown code address: $%04X\n", s_pc);
      error_handler(s_pc);
    }
  }
}
static const uint8_t s_mem_d000[0x3000] = {
  0x6F, 0xD8, 0x65, 0xD7, 0xF8, 0xDC, 0x94, 0xD9, 0xB1, 0xDB, 0x30, 0xF3, 0xD8, 0xDF, 0xE1, 0xDB,
  0x8F, 0xF3, 0x98, 0xF3, 0xE4, 0xF1, 0xDD, 0xF1, 0xD4, 0xF1, 0x24, 0xF2, 0x31, 0xF2, 0x40, 0xF2,
  0xD7, 0xF3, 0xE1, 0xF3, 0xE8, 0xF6, 0xFD, 0xF6, 0x68, 0xF7, 0x6E, 0xF7, 0xE6, 0xF7, 0x57, 0xFC,
  0x20, 0xF7, 0x26, 0xF7, 0x74, 0xF7, 0x6C, 0xF2, 0x6E, 0xF2, 0x72, 0xF2, 0x76, 0xF2, 0x7F, 0xF2,
  0x4E, 0xF2, 0x6A, 0xD9, 0x55, 0xF2, 0x85, 0xF2, 0xA5, 0xF2, 0xCA, 0xF2, 0x17, 0xF3, 0xBB, 0xF3,
  0x9E, 0xF3, 0x61, 0xF2, 0x45, 0xDA, 0x3D, 0xD9, 0x11, 0xD9, 0xC8, 0xD9, 0x48, 0xD8, 0xF4, 0x03,
  0x20, 0xD9, 0x6A, 0xD9, 0xDB, 0xD9, 0x6D, 0xD8, 0xEB, 0xD9, 0x83, 0xE7, 0xC8, 0xD8, 0xAF, 0xD8,
  0x12, 0xE3, 0x7A, 0xE7, 0xD4, 0xDA, 0x95, 0xD8, 0xA4, 0xD6, 0x69, 0xD6, 0x9F, 0xDB, 0x48, 0xD6,
  0x90, 0xEB, 0x23, 0xEC, 0xAF, 0xEB, 0x0A, 0x00, 0xDE, 0xE2, 0x12, 0xD4, 0xCD, 0xDF, 0xFF, 0xE2,
  0x8D, 0xEE, 0xAE, 0xEF, 0x41, 0xE9, 0x09, 0xEF, 0xEA, 0xEF, 0xF1, 0xEF, 0x3A, 0xF0, 0x9E, 0xF0,
  0x64, 0xE7, 0xD6, 0xE6, 0xC5, 0xE3, 0x07, 0xE7, 0xE5, 0xE6, 0x46, 0xE6, 0x5A, 0xE6, 0x86, 0xE6,
  0x91, 0xE6, 0x79, 0xC0, 0xE7, 0x79, 0xA9, 0xE7, 0x7B, 0x81, 0xE9, 0x7B, 0x68, 0xEA, 0x7D, 0x96,
  0xEE, 0x50, 0x54, 0xDF, 0x46, 0x4E, 0xDF, 0x7F, 0xCF, 0xEE, 0x7F, 0x97, 0xDE, 0x64, 0x64, 0xDF,
  0x45, 0x4E, 0xC4, 0x46, 0x4F, 0xD2, 0x4E, 0x45, 0x58, 0xD4, 0x44, 0x41, 0x54, 0xC1, 0x49, 0x4E,
  0x50, 0x55, 0xD4, 0x44, 0x45, 0xCC, 0x44, 0x49, 0xCD, 0x52, 0x45, 0x41, 0xC4, 0x47, 0xD2, 0x54,
  0x45, 0x58, 0xD4, 0x50, 0x52, 0xA3, 0x49, 0x4E, 0xA3, 0x43, 0x41, 0x4C, 0xCC, 0x50, 0x4C, 0x4F,
  0xD4, 0x48, 0x4C, 0x49, 0xCE, 0x56, 0x4C, 0x49, 0xCE, 0x48, 0x47, 0x52, 0xB2, 0x48, 0x47, 0xD2,
  0x48, 0x43, 0x4F, 0x4C, 0x4F, 0x52, 0xBD, 0x48, 0x50, 0x4C, 0x4F, 0xD4, 0x44, 0x52, 0x41, 0xD7,
  0x58, 0x44, 0x52, 0x41, 0xD7, 0x48, 0x54, 0x41, 0xC2, 0x48, 0x4F, 0x4D, 0xC5, 0x52, 0x4F, 0x54,
  0xBD, 0x53, 0x43, 0x41, 0x4C, 0x45, 0xBD, 0x53, 0x48, 0x4C, 0x4F, 0x41, 0xC4, 0x54, 0x52, 0x41,
  0x43, 0xC5, 0x4E, 0x4F, 0x54, 0x52, 0x41, 0x43, 0xC5, 0x4E, 0x4F, 0x52, 0x4D, 0x41, 0xCC, 0x49,
  0x4E, 0x56, 0x45, 0x52, 0x53, 0xC5, 0x46, 0x4C, 0x41, 0x53, 0xC8, 0x43, 0x4F, 0x4C, 0x4F, 0x52,
  0xBD, 0x50, 0x4F, 0xD0, 0x56, 0x54, 0x41, 0xC2, 0x48, 0x49, 0x4D, 0x45, 0x4D, 0xBA, 0x4C, 0x4F,
  0x4D, 0x45, 0x4D, 0xBA, 0x4F, 0x4E, 0x45, 0x52, 0xD2, 0x52, 0x45, 0x53, 0x55, 0x4D, 0xC5, 0x52,
  0x45, 0x43, 0x41, 0x4C, 0xCC, 0x53, 0x54, 0x4F, 0x52, 0xC5, 0x53, 0x50, 0x45, 0x45, 0x44, 0xBD,
  0x4C, 0x45, 0xD4, 0x47, 0x4F, 0x54, 0xCF, 0x52, 0x55, 0xCE, 0x49, 0xC6, 0x52, 0x45, 0x53, 0x54,
  0x4F, 0x52, 0xC5, 0xA6, 0x47, 0x4F, 0x53, 0x55, 0xC2, 0x52, 0x45, 0x54, 0x55, 0x52, 0xCE, 0x52,
  0x45, 0xCD, 0x53, 0x54, 0x4F, 0xD0, 0x4F, 0xCE, 0x57, 0x41, 0x49, 0xD4, 0x4C, 0x4F, 0x41, 0xC4,
  0x53, 0x41, 0x56, 0xC5, 0x44, 0x45, 0xC6, 0x50, 0x4F, 0x4B, 0xC5, 0x50, 0x52, 0x49, 0x4E, 0xD4,
  0x43, 0x4F, 0x4E, 0xD4, 0x4C, 0x49, 0x53, 0xD4, 0x43, 0x4C, 0x45, 0x41, 0xD2, 0x47, 0x45, 0xD4,
  0x4E, 0x45, 0xD7, 0x54, 0x41, 0x42, 0xA8, 0x54, 0xCF, 0x46, 0xCE, 0x53, 0x50, 0x43, 0xA8, 0x54,
  0x48, 0x45, 0xCE, 0x41, 0xD4, 0x4E, 0x4F, 0xD4, 0x53, 0x54, 0x45, 0xD0, 0xAB, 0xAD, 0xAA, 0xAF,
  0xDE, 0x41, 0x4E, 0xC4, 0x4F, 0xD2, 0xBE, 0xBD, 0xBC, 0x53, 0x47, 0xCE, 0x49, 0x4E, 0xD4, 0x41,
  0x42, 0xD3, 0x55, 0x53, 0xD2, 0x46, 0x52, 0xC5, 0x53, 0x43, 0x52, 0x4E, 0xA8, 0x50, 0x44, 0xCC,
  0x50, 0x4F, 0xD3, 0x53, 0x51, 0xD2, 0x52, 0x4E, 0xC4, 0x4C, 0x4F, 0xC7, 0x45, 0x58, 0xD0, 0x43,
  0x4F, 0xD3, 0x53, 0x49, 0xCE, 0x54, 0x41, 0xCE, 0x41, 0x54, 0xCE, 0x50, 0x45, 0x45, 0xCB, 0x4C,
  0x45, 0xCE, 0x53, 0x54, 0x52, 0xA4, 0x56, 0x41, 0xCC, 0x41, 0x53, 0xC3, 0x43, 0x48, 0x52, 0xA4,
  0x4C, 0x45, 0x46, 0x54, 0xA4, 0x52, 0x49, 0x47, 0x48, 0x54, 0xA4, 0x4D, 0x49, 0x44, 0xA4, 0x00,
  0x4E, 0x45, 0x58, 0x54, 0x20, 0x57, 0x49, 0x54, 0x48, 0x4F, 0x55, 0x54, 0x20, 0x46, 0x4F, 0xD2,
  0x53, 0x59, 0x4E, 0x54, 0x41, 0xD8, 0x52, 0x45, 0x54, 0x55, 0x52, 0x4E, 0x20, 0x57, 0x49, 0x54,
  0x48, 0x4F, 0x55, 0x54, 0x20, 0x47, 0x4F, 0x53, 0x55, 0xC2, 0x4F, 0x55, 0x54, 0x20, 0x4F, 0x46,
  0x20, 0x44, 0x41, 0x54, 0xC1, 0x49, 0x4C, 0x4C, 0x45, 0x47, 0x41, 0x4C, 0x20, 0x51, 0x55, 0x41,
  0x4E, 0x54, 0x49, 0x54, 0xD9, 0x4F, 0x56, 0x45, 0x52, 0x46, 0x4C, 0x4F, 0xD7, 0x4F, 0x55, 0x54,
  0x20, 0x4F, 0x46, 0x20, 0x4D, 0x45, 0x4D, 0x4F, 0x52, 0xD9, 0x55, 0x4E, 0x44, 0x45, 0x46, 0x27,
  0x44, 0x20, 0x53, 0x54, 0x41, 0x54, 0x45, 0x4D, 0x45, 0x4E, 0xD4, 0x42, 0x41, 0x44, 0x20, 0x53,
  0x55, 0x42, 0x53, 0x43, 0x52, 0x49, 0x50, 0xD4, 0x52, 0x45, 0x44, 0x49, 0x4D, 0x27, 0x44, 0x20,
  0x41, 0x52, 0x52, 0x41, 0xD9, 0x44, 0x49, 0x56, 0x49, 0x53, 0x49, 0x4F, 0x4E, 0x20, 0x42, 0x59,
  0x20, 0x5A, 0x45, 0x52, 0xCF, 0x49, 0x4C, 0x4C, 0x45, 0x47, 0x41, 0x4C, 0x20, 0x44, 0x49, 0x52,
  0x45, 0x43, 0xD4, 0x54, 0x59, 0x50, 0x45, 0x20, 0x4D, 0x49, 0x53, 0x4D, 0x41, 0x54, 0x43, 0xC8,
  0x53, 0x54, 0x52, 0x49, 0x4E, 0x47, 0x20, 0x54, 0x4F, 0x4F, 0x20, 0x4C, 0x4F, 0x4E, 0xC7, 0x46,
  0x4F, 0x52, 0x4D, 0x55, 0x4C, 0x41, 0x20, 0x54, 0x4F, 0x4F, 0x20, 0x43, 0x4F, 0x4D, 0x50, 0x4C,
  0x45, 0xD8, 0x43, 0x41, 0x4E, 0x27, 0x54, 0x20, 0x43, 0x4F, 0x4E, 0x54, 0x49, 0x4E, 0x55, 0xC5,
  0x55, 0x4E, 0x44, 0x45, 0x46, 0x27, 0x44, 0x20, 0x46, 0x55, 0x4E, 0x43, 0x54, 0x49, 0x4F, 0xCE,
  0x20, 0x45, 0x52, 0x52, 0x4F, 0x52, 0x07, 0x00, 0x20, 0x49, 0x4E, 0x20, 0x00, 0x0D, 0x42, 0x52,
  0x45, 0x41, 0x4B, 0x07, 0x00, 0xBA, 0xE8, 0xE8, 0xE8, 0xE8, 0xBD, 0x01, 0x01, 0xC9, 0x81, 0xD0,
  0x21, 0xA5, 0x86, 0xD0, 0x0A, 0xBD, 0x02, 0x01, 0x85, 0x85, 0xBD, 0x03, 0x01, 0x85, 0x86, 0xDD,
  0x03, 0x01, 0xD0, 0x07, 0xA5, 0x85, 0xDD, 0x02, 0x01, 0xF0, 0x07, 0x8A, 0x18, 0x69, 0x12, 0xAA,
  0xD0, 0xD8, 0x60, 0x20, 0xE3, 0xD3, 0x85, 0x6D, 0x84, 0x6E, 0x38, 0xA5, 0x96, 0xE5, 0x9B, 0x85,
  0x5E, 0xA8, 0xA5, 0x97, 0xE5, 0x9C, 0xAA, 0xE8, 0x98, 0xF0, 0x23, 0xA5, 0x96, 0x38, 0xE5, 0x5E,
  0x85, 0x96, 0xB0, 0x03, 0xC6, 0x97, 0x38, 0xA5, 0x94, 0xE5, 0x5E, 0x85, 0x94, 0xB0, 0x08, 0xC6,
  0x95, 0x90, 0x04, 0xB1, 0x96, 0x91, 0x94, 0x88, 0xD0, 0xF9, 0xB1, 0x96, 0x91, 0x94, 0xC6, 0x97,
  0xC6, 0x95, 0xCA, 0xD0, 0xF2, 0x60, 0x0A, 0x69, 0x36, 0xB0, 0x35, 0x85, 0x5E, 0xBA, 0xE4, 0x5E,
  0x90, 0x2E, 0x60, 0xC4, 0x70, 0x90, 0x28, 0xD0, 0x04, 0xC5, 0x6F, 0x90, 0x22, 0x48, 0xA2, 0x09,
  0x98, 0x48, 0xB5, 0x93, 0xCA, 0x10, 0xFA, 0x20, 0x84, 0xE4, 0xA2, 0xF7, 0x68, 0x95, 0x9D, 0xE8,
  0x30, 0xFA, 0x68, 0xA8, 0x68, 0xC4, 0x70, 0x90, 0x06, 0xD0, 0x05, 0xC5, 0x6F, 0xB0, 0x01, 0x60,
  0xA2, 0x4D, 0x24, 0xD8, 0x10, 0x03, 0x4C, 0xE9, 0xF2, 0x20, 0xFB, 0xDA, 0x20, 0x5A, 0xDB, 0xBD,
  0x60, 0xD2, 0x48, 0x20, 0x5C, 0xDB, 0xE8, 0x68, 0x10, 0xF5, 0x20, 0x83, 0xD6, 0xA9, 0x50, 0xA0,
  0xD3, 0x20, 0x3A, 0xDB, 0xA4, 0x76, 0xC8, 0xF0, 0x03, 0x20, 0x19, 0xED, 0x20, 0xFB, 0xDA, 0xA2,
  0xDD, 0x20, 0x2E, 0xD5, 0x86, 0xB8, 0x84, 0xB9, 0x46, 0xD8, 0x20, 0xB1, 0x00, 0xAA, 0xF0, 0xEC,
  0xA2, 0xFF, 0x86, 0x76, 0x90, 0x06, 0x20, 0x59, 0xD5, 0x4C, 0x05, 0xD8, 0xA6, 0xAF, 0x86, 0x69,
  0xA6, 0xB0, 0x86, 0x6A, 0x20, 0x0C, 0xDA, 0x20, 0x59, 0xD5, 0x84, 0x0F, 0x20, 0x1A, 0xD6, 0x90,
  0x44, 0xA0, 0x01, 0xB1, 0x9B, 0x85, 0x5F, 0xA5, 0x69, 0x85, 0x5E, 0xA5, 0x9C, 0x85, 0x61, 0xA5,
  0x9B, 0x88, 0xF1, 0x9B, 0x18, 0x65, 0x69, 0x85, 0x69, 0x85, 0x60, 0xA5, 0x6A, 0x69, 0xFF, 0x85,
  0x6A, 0xE5, 0x9C, 0xAA, 0x38, 0xA5, 0x9B, 0xE5, 0x69, 0xA8, 0xB0, 0x03, 0xE8, 0xC6, 0x61, 0x18,
  0x65, 0x5E, 0x90, 0x03, 0xC6, 0x5F, 0x18, 0xB1, 0x5E, 0x91, 0x60, 0xC8, 0xD0, 0xF9, 0xE6, 0x5F,
  0xE6, 0x61, 0xCA, 0xD0, 0xF2, 0xAD, 0x00, 0x02, 0xF0, 0x38, 0xA5, 0x73, 0xA4, 0x74, 0x85, 0x6F,
  0x84, 0x70, 0xA5, 0x69, 0x85, 0x96, 0x65, 0x0F, 0x85, 0x94, 0xA4, 0x6A, 0x84, 0x97, 0x90, 0x01,
  0xC8, 0x84, 0x95, 0x20, 0x93, 0xD3, 0xA5, 0x50, 0xA4, 0x51, 0x8D, 0xFE, 0x01, 0x8C, 0xFF, 0x01,
  0xA5, 0x6D, 0xA4, 0x6E, 0x85, 0x69, 0x84, 0x6A, 0xA4, 0x0F, 0xB9, 0xFB, 0x01, 0x88, 0x91, 0x9B,
  0xD0, 0xF8, 0x20, 0x65, 0xD6, 0xA5, 0x67, 0xA4, 0x68, 0x85, 0x5E, 0x84, 0x5F, 0x18, 0xA0, 0x01,
  0xB1, 0x5E, 0xD0, 0x0B, 0xA5, 0x69, 0x85, 0xAF, 0xA5, 0x6A, 0x85, 0xB0, 0x4C, 0x3C, 0xD4, 0xA0,
  0x04, 0xC8, 0xB1, 0x5E, 0xD0, 0xFB, 0xC8, 0x98, 0x65, 0x5E, 0xAA, 0xA0, 0x00, 0x91, 0x5E, 0xA5,
  0x5F, 0x69, 0x00, 0xC8, 0x91, 0x5E, 0x86, 0x5E, 0x85, 0x5F, 0x90, 0xD2, 0xA2, 0x80, 0x86, 0x33,
  0x20, 0x6A, 0xFD, 0xE0, 0xEF, 0x90, 0x02, 0xA2, 0xEF, 0xA9, 0x00, 0x9D, 0x00, 0x02, 0x8A, 0xF0,
  0x0B, 0xBD, 0xFF, 0x01, 0x29, 0x7F, 0x9D, 0xFF, 0x01, 0xCA, 0xD0, 0xF5, 0xA9, 0x00, 0xA2, 0xFF,
  0xA0, 0x01, 0x60, 0x20, 0x0C, 0xFD, 0x29, 0x7F, 0x60, 0xA6, 0xB8, 0xCA, 0xA0, 0x04, 0x84, 0x13,
  0x24, 0xD6, 0x10, 0x08, 0x68, 0x68, 0x20, 0x65, 0xD6, 0x4C, 0xD2, 0xD7, 0xE8, 0xBD, 0x00, 0x02,
  0x24, 0x13, 0x70, 0x04, 0xC9, 0x20, 0xF0, 0xF4, 0x85, 0x0E, 0xC9, 0x22, 0xF0, 0x74, 0x70, 0x4D,
  0xC9, 0x3F, 0xD0, 0x04, 0xA9, 0xBA, 0xD0, 0x45, 0xC9, 0x30, 0x90, 0x04, 0xC9, 0x3C, 0x90, 0x3D,
  0x84, 0xAD, 0xA9, 0xD0, 0x85, 0x9D, 0xA9, 0xCF, 0x85, 0x9E, 0xA0, 0x00, 0x84, 0x0F, 0x88, 0x86,
  0xB8, 0xCA, 0xC8, 0xD0, 0x02, 0xE6, 0x9E, 0xE8, 0xBD, 0x00, 0x02, 0xC9, 0x20, 0xF0, 0xF8, 0x38,
  0xF1, 0x9D, 0xF0, 0xEE, 0xC9, 0x80, 0xD0, 0x41, 0x05, 0x0F, 0xC9, 0xC5, 0xD0, 0x0D, 0xBD, 0x01,
  0x02, 0xC9, 0x4E, 0xF0, 0x34, 0xC9, 0x4F, 0xF0, 0x30, 0xA9, 0xC5, 0xA4, 0xAD, 0xE8, 0xC8, 0x99,
  0xFB, 0x01, 0xB9, 0xFB, 0x01, 0xF0, 0x39, 0x38, 0xE9, 0x3A, 0xF0, 0x04, 0xC9, 0x49, 0xD0, 0x02,
  0x85, 0x13, 0x38, 0xE9, 0x78, 0xD0, 0x86, 0x85, 0x0E, 0xBD, 0x00, 0x02, 0xF0, 0xDF, 0xC5, 0x0E,
  0xF0, 0xDB, 0xC8, 0x99, 0xFB, 0x01, 0xE8, 0xD0, 0xF0, 0xA6, 0xB8, 0xE6, 0x0F, 0xB1, 0x9D, 0xC8,
  0xD0, 0x02, 0xE6, 0x9E, 0x0A, 0x90, 0xF6, 0xB1, 0x9D, 0xD0, 0x9D, 0xBD, 0x00, 0x02, 0x10, 0xBB,
  0x99, 0xFD, 0x01, 0xC6, 0xB9, 0xA9, 0xFF, 0x85, 0xB8, 0x60, 0xA5, 0x67, 0xA6, 0x68, 0xA0, 0x01,
  0x85, 0x9B, 0x86, 0x9C, 0xB1, 0x9B, 0xF0, 0x1F, 0xC8, 0xC8, 0xA5, 0x51, 0xD1, 0x9B, 0x90, 0x18,
  0xF0, 0x03, 0x88, 0xD0, 0x09, 0xA5, 0x50, 0x88, 0xD1, 0x9B, 0x90, 0x0C, 0xF0, 0x0A, 0x88, 0xB1,
  0x9B, 0xAA, 0x88, 0xB1, 0x9B, 0xB0, 0xD7, 0x18, 0x60, 0xD0, 0xFD, 0xA9, 0x00, 0x85, 0xD6, 0xA8,
  0x91, 0x67, 0xC8, 0x91, 0x67, 0xA5, 0x67, 0x69, 0x02, 0x85, 0x69, 0x85, 0xAF, 0xA5, 0x68, 0x69,
  0x00, 0x85, 0x6A, 0x85, 0xB0, 0x20, 0x97, 0xD6, 0xA9, 0x00, 0xD0, 0x2A, 0xA5, 0x73, 0xA4, 0x74,
  0x85, 0x6F, 0x84, 0x70, 0xA5, 0x69, 0xA4, 0x6A, 0x85, 0x6B, 0x84, 0x6C, 0x85, 0x6D, 0x84, 0x6E,
  0x20, 0x49, 0xD8, 0xA2, 0x55, 0x86, 0x52, 0x68, 0xA8, 0x68, 0xA2, 0xF8, 0x9A, 0x48, 0x98, 0x48,
  0xA9, 0x00, 0x85, 0x7A, 0x85, 0x14, 0x60, 0x18, 0xA5, 0x67, 0x69, 0xFF, 0x85, 0xB8, 0xA5, 0x68,
  0x69, 0xFF, 0x85, 0xB9, 0x60, 0x90, 0x0A, 0xF0, 0x08, 0xC9, 0xC9, 0xF0, 0x04, 0xC9, 0x2C, 0xD0,
  0xE5, 0x20, 0x0C, 0xDA, 0x20, 0x1A, 0xD6, 0x20, 0xB7, 0x00, 0xF0, 0x10, 0xC9, 0xC9, 0xF0, 0x04,
  0xC9, 0x2C, 0xD0, 0x84, 0x20, 0xB1, 0x00, 0x20, 0x0C, 0xDA, 0xD0, 0xCA, 0x68, 0x68, 0xA5, 0x50,
  0x05, 0x51, 0xD0, 0x06, 0xA9, 0xFF, 0x85, 0x50, 0x85, 0x51, 0xA0, 0x01, 0xB1, 0x9B, 0xF0, 0x44,
  0x20, 0x58, 0xD8, 0x20, 0xFB, 0xDA, 0xC8, 0xB1, 0x9B, 0xAA, 0xC8, 0xB1, 0x9B, 0xC5, 0x51, 0xD0,
  0x04, 0xE4, 0x50, 0xF0, 0x02, 0xB0, 0x2D, 0x84, 0x85, 0x20, 0x24, 0xED, 0xA9, 0x20, 0xA4, 0x85,
  0x29, 0x7F, 0x20, 0x5C, 0xDB, 0xA5, 0x24, 0xC9, 0x21, 0x90, 0x07, 0x20, 0xFB, 0xDA, 0xA9, 0x05,
  0x85, 0x24, 0xC8, 0xB1, 0x9B, 0xD0, 0x1D, 0xA8, 0xB1, 0x9B, 0xAA, 0xC8, 0xB1, 0x9B, 0x86, 0x9B,
  0x85, 0x9C, 0xD0, 0xB6, 0xA9, 0x0D, 0x20, 0x5C, 0xDB, 0x4C, 0xD2, 0xD7, 0xC8, 0xD0, 0x02, 0xE6,
  0x9E, 0xB1, 0x9D, 0x60, 0x10, 0xCC, 0x38, 0xE9, 0x7F, 0xAA, 0x84, 0x85, 0xA0, 0xD0, 0x84, 0x9D,
  0xA0, 0xCF, 0x84, 0x9E, 0xA0, 0xFF, 0xCA, 0xF0, 0x07, 0x20, 0x2C, 0xD7, 0x10, 0xFB, 0x30, 0xF6,
  0xA9, 0x20, 0x20, 0x5C, 0xDB, 0x20, 0x2C, 0xD7, 0x30, 0x05, 0x20, 0x5C, 0xDB, 0xD0, 0xF6, 0x20,
  0x5C, 0xDB, 0xA9, 0x20, 0xD0, 0x98, 0xA9, 0x80, 0x85, 0x14, 0x20, 0x46, 0xDA, 0x20, 0x65, 0xD3,
  0xD0, 0x05, 0x8A, 0x69, 0x0F, 0xAA, 0x9A, 0x68, 0x68, 0xA9, 0x09, 0x20, 0xD6, 0xD3, 0x20, 0xA3,
  0xD9, 0x18, 0x98, 0x65, 0xB8, 0x48, 0xA5, 0xB9, 0x69, 0x00, 0x48, 0xA5, 0x76, 0x48, 0xA5, 0x75,
  0x48, 0xA9, 0xC1, 0x20, 0xC0, 0xDE, 0x20, 0x6A, 0xDD, 0x20, 0x67, 0xDD, 0xA5, 0xA2, 0x09, 0x7F,
  0x25, 0x9E, 0x85, 0x9E, 0xA9, 0xAF, 0xA0, 0xD7, 0x85, 0x5E, 0x84, 0x5F, 0x4C, 0x20, 0xDE, 0xA9,
  0x13, 0xA0, 0xE9, 0x20, 0xF9, 0xEA, 0x20, 0xB7, 0x00, 0xC9, 0xC7, 0xD0, 0x06, 0x20, 0xB1, 0x00,
  0x20, 0x67, 0xDD, 0x20, 0x82, 0xEB, 0x20, 0x15, 0xDE, 0xA5, 0x86, 0x48, 0xA5, 0x85, 0x48, 0xA9,
  0x81, 0x48, 0xBA, 0x86, 0xF8, 0x20, 0x58, 0xD8, 0xA5, 0xB8, 0xA4, 0xB9, 0xA6, 0x76, 0xE8, 0xF0,
  0x04, 0x85, 0x79, 0x84, 0x7A, 0xA0, 0x00, 0xB1, 0xB8, 0xD0, 0x57, 0xA0, 0x02, 0xB1, 0xB8, 0x18,
  0xF0, 0x34, 0xC8, 0xB1, 0xB8, 0x85, 0x75, 0xC8, 0xB1, 0xB8, 0x85, 0x76, 0x98, 0x65, 0xB8, 0x85,
  0xB8, 0x90, 0x02, 0xE6, 0xB9, 0x24, 0xF2, 0x10, 0x14, 0xA6, 0x76, 0xE8, 0xF0, 0x0F, 0xA9, 0x23,
  0x20, 0x5C, 0xDB, 0xA6, 0x75, 0xA5, 0x76, 0x20, 0x24, 0xED, 0x20, 0x57, 0xDB, 0x20, 0xB1, 0x00,
  0x20, 0x28, 0xD8, 0x4C, 0xD2, 0xD7, 0xF0, 0x62, 0xF0, 0x2D, 0xE9, 0x80, 0x90, 0x11, 0xC9, 0x40,
  0xB0, 0x14, 0x0A, 0xA8, 0xB9, 0x01, 0xD0, 0x48, 0xB9, 0x00, 0xD0, 0x48, 0x4C, 0xB1, 0x00, 0x4C,
  0x46, 0xDA, 0xC9, 0x3A, 0xF0, 0xBF, 0x4C, 0xC9, 0xDE, 0x38, 0xA5, 0x67, 0xE9, 0x01, 0xA4, 0x68,
  0xB0, 0x01, 0x88, 0x85, 0x7D, 0x84, 0x7E, 0x60, 0xAD, 0x00, 0xC0, 0xC9, 0x83, 0xF0, 0x01, 0x60,
  0x20, 0x53, 0xD5, 0xA2, 0xFF, 0x24, 0xD8, 0x10, 0x03, 0x4C, 0xE9, 0xF2, 0xC9, 0x03, 0xB0, 0x01,
  0x18, 0xD0, 0x3C, 0xA5, 0xB8, 0xA4, 0xB9, 0xA6, 0x76, 0xE8, 0xF0, 0x0C, 0x85, 0x79, 0x84, 0x7A,
  0xA5, 0x75, 0xA4, 0x76, 0x85, 0x77, 0x84, 0x78, 0x68, 0x68, 0xA9, 0x5D, 0xA0, 0xD3, 0x90, 0x03,
  0x4C, 0x31, 0xD4, 0x4C, 0x3C, 0xD4, 0xD0, 0x17, 0xA2, 0xD2, 0xA4, 0x7A, 0xD0, 0x03, 0x4C, 0x12,
  0xD4, 0xA5, 0x79, 0x85, 0xB8, 0x84, 0xB9, 0xA5, 0x77, 0xA4, 0x78, 0x85, 0x75, 0x84, 0x76, 0x60,
  0x38, 0xA5, 0xAF, 0xE5, 0x67, 0x85, 0x50, 0xA5, 0xB0, 0xE5, 0x68, 0x85, 0x51, 0x20, 0xF0, 0xD8,
  0x20, 0xCD, 0xFE, 0x20, 0x01, 0xD9, 0x4C, 0xCD, 0xFE, 0x20, 0xF0, 0xD8, 0x20, 0xFD, 0xFE, 0x18,
  0xA5, 0x67, 0x65, 0x50, 0x85, 0x69, 0xA5, 0x68, 0x65, 0x51, 0x85, 0x6A, 0xA5, 0x52, 0x85, 0xD6,
  0x20, 0x01, 0xD9, 0x20, 0xFD, 0xFE, 0x24, 0xD6, 0x10, 0x03, 0x4C, 0x65, 0xD6, 0x4C, 0xF2, 0xD4,
  0xA9, 0x50, 0xA0, 0x00, 0x85, 0x3C, 0x84, 0x3D, 0xA9, 0x52, 0x85, 0x3E, 0x84, 0x3F, 0x84, 0xD6,
  0x60, 0xA5, 0x67, 0xA4, 0x68, 0x85, 0x3C, 0x84, 0x3D, 0xA5, 0x69, 0xA4, 0x6A, 0x85, 0x3E, 0x84,
  0x3F, 0x60, 0x08, 0xC6, 0x76, 0x28, 0xD0, 0x03, 0x4C, 0x65, 0xD6, 0x20, 0x6C, 0xD6, 0x4C, 0x35,
  0xD9, 0xA9, 0x03, 0x20, 0xD6, 0xD3, 0xA5, 0xB9, 0x48, 0xA5, 0xB8, 0x48, 0xA5, 0x76, 0x48, 0xA5,
  0x75, 0x48, 0xA9, 0xB0, 0x48, 0x20, 0xB7, 0x00, 0x20, 0x3E, 0xD9, 0x4C, 0xD2, 0xD7, 0x20, 0x0C,
  0xDA, 0x20, 0xA6, 0xD9, 0xA5, 0x76, 0xC5, 0x51, 0xB0, 0x0B, 0x98, 0x38, 0x65, 0xB8, 0xA6, 0xB9,
  0x90, 0x07, 0xE8, 0xB0, 0x04, 0xA5, 0x67, 0xA6, 0x68, 0x20, 0x1E, 0xD6, 0x90, 0x1E, 0xA5, 0x9B,
  0xE9, 0x01, 0x85, 0xB8, 0xA5, 0x9C, 0xE9, 0x00, 0x85, 0xB9, 0x60, 0xD0, 0xFD, 0xA9, 0xFF, 0x85,
  0x85, 0x20, 0x65, 0xD3, 0x9A, 0xC9, 0xB0, 0xF0, 0x0B, 0xA2, 0x16, 0x2C, 0xA2, 0x5A, 0x4C, 0x12,
  0xD4, 0x4C, 0xC9, 0xDE, 0x68, 0x68, 0xC0, 0x42, 0xF0, 0x3B, 0x85, 0x75, 0x68, 0x85, 0x76, 0x68,
  0x85, 0xB8, 0x68, 0x85, 0xB9, 0x20, 0xA3, 0xD9, 0x98, 0x18, 0x65, 0xB8, 0x85, 0xB8, 0x90, 0x02,
  0xE6, 0xB9, 0x60, 0xA2, 0x3A, 0x2C, 0xA2, 0x00, 0x86, 0x0D, 0xA0, 0x00, 0x84, 0x0E, 0xA5, 0x0E,
  0xA6, 0x0D, 0x85, 0x0D, 0x86, 0x0E, 0xB1, 0xB8, 0xF0, 0xE8, 0xC5, 0x0E, 0xF0, 0xE4, 0xC8, 0xC9,
  0x22, 0xD0, 0xF3, 0xF0, 0xE9, 0x68, 0x68, 0x68, 0x60, 0x20, 0x7B, 0xDD, 0x20, 0xB7, 0x00, 0xC9,
  0xAB, 0xF0, 0x05, 0xA9, 0xC4, 0x20, 0xC0, 0xDE, 0xA5, 0x9D, 0xD0, 0x05, 0x20, 0xA6, 0xD9, 0xF0,
  0xB7, 0x20, 0xB7, 0x00, 0xB0, 0x03, 0x4C, 0x3E, 0xD9, 0x4C, 0x28, 0xD8, 0x20, 0xF8, 0xE6, 0x48,
  0xC9, 0xB0, 0xF0, 0x04, 0xC9, 0xAB, 0xD0, 0x89, 0xC6, 0xA1, 0xD0, 0x04, 0x68, 0x4C, 0x2A, 0xD8,
  0x20, 0xB1, 0x00, 0x20, 0x0C, 0xDA, 0xC9, 0x2C, 0xF0, 0xEE, 0x68, 0x60, 0xA2, 0x00, 0x86, 0x50,
  0x86, 0x51, 0xB0, 0xF7, 0xE9, 0x2F, 0x85, 0x0D, 0xA5, 0x51, 0x85, 0x5E, 0xC9, 0x19, 0xB0, 0xD4,
  0xA5, 0x50, 0x0A, 0x26, 0x5E, 0x0A, 0x26, 0x5E, 0x65, 0x50, 0x85, 0x50, 0xA5, 0x5E, 0x65, 0x51,
  0x85, 0x51, 0x06, 0x50, 0x26, 0x51, 0xA5, 0x50, 0x65, 0x0D, 0x85, 0x50, 0x90, 0x02, 0xE6, 0x51,
  0x20, 0xB1, 0x00, 0x4C, 0x12, 0xDA, 0x20, 0xE3, 0xDF, 0x85, 0x85, 0x84, 0x86, 0xA9, 0xD0, 0x20,
  0xC0, 0xDE, 0xA5, 0x12, 0x48, 0xA5, 0x11, 0x48, 0x20, 0x7B, 0xDD, 0x68, 0x2A, 0x20, 0x6D, 0xDD,
  0xD0, 0x18, 0x68, 0x10, 0x12, 0x20, 0x72, 0xEB, 0x20, 0x0C, 0xE1, 0xA0, 0x00, 0xA5, 0xA0, 0x91,
  0x85, 0xC8, 0xA5, 0xA1, 0x91, 0x85, 0x60, 0x4C, 0x27, 0xEB, 0x68, 0xA0, 0x02, 0xB1, 0xA0, 0xC5,
  0x70, 0x90, 0x17, 0xD0, 0x07, 0x88, 0xB1, 0xA0, 0xC5, 0x6F, 0x90, 0x0E, 0xA4, 0xA1, 0xC4, 0x6A,
  0x90, 0x08, 0xD0, 0x0D, 0xA5, 0xA0, 0xC5, 0x69, 0xB0, 0x07, 0xA5, 0xA0, 0xA4, 0xA1, 0x4C, 0xB7,
  0xDA, 0xA0, 0x00, 0xB1, 0xA0, 0x20, 0xD5, 0xE3, 0xA5, 0x8C, 0xA4, 0x8D, 0x85, 0xAB, 0x84, 0xAC,
  0x20, 0xD4, 0xE5, 0xA9, 0x9D, 0xA0, 0x00, 0x85, 0x8C, 0x84, 0x8D, 0x20, 0x35, 0xE6, 0xA0, 0x00,
  0xB1, 0x8C, 0x91, 0x85, 0xC8, 0xB1, 0x8C, 0x91, 0x85, 0xC8, 0xB1, 0x8C, 0x91, 0x85, 0x60, 0x20,
  0x3D, 0xDB, 0x20, 0xB7, 0x00, 0xF0, 0x24, 0xF0, 0x29, 0xC9, 0xC0, 0xF0, 0x39, 0xC9, 0xC3, 0x18,
  0xF0, 0x34, 0xC9, 0x2C, 0x18, 0xF0, 0x1C, 0xC9, 0x3B, 0xF0, 0x44, 0x20, 0x7B, 0xDD, 0x24, 0x11,
  0x30, 0xDD, 0x20, 0x34, 0xED, 0x20, 0xE7, 0xE3, 0x4C, 0xCF, 0xDA, 0xA9, 0x0D, 0x20, 0x5C, 0xDB,
  0x49, 0xFF, 0x60, 0xA5, 0x24, 0xC9, 0x18, 0x90, 0x05, 0x20, 0xFB, 0xDA, 0xD0, 0x21, 0x69, 0x10,
  0x29, 0xF0, 0x85, 0x24, 0x90, 0x19, 0x08, 0x20, 0xF5, 0xE6, 0xC9, 0x29, 0xF0, 0x03, 0x4C, 0xC9,
  0xDE, 0x28, 0x90, 0x07, 0xCA, 0x8A, 0xE5, 0x24, 0x90, 0x05, 0xAA, 0xE8, 0xCA, 0xD0, 0x06, 0x20,
  0xB1, 0x00, 0x4C, 0xD7, 0xDA, 0x20, 0x57, 0xDB, 0xD0, 0xF2, 0x20, 0xE7, 0xE3, 0x20, 0x00, 0xE6,
  0xAA, 0xA0, 0x00, 0xE8, 0xCA, 0xF0, 0xBB, 0xB1, 0x5E, 0x20, 0x5C, 0xDB, 0xC8, 0xC9, 0x0D, 0xD0,
  0xF3, 0x20, 0x00, 0xDB, 0x4C, 0x44, 0xDB, 0xA9, 0x20, 0x2C, 0xA9, 0x3F, 0x09, 0x80, 0xC9, 0xA0,
  0x90, 0x02, 0x05, 0xF3, 0x20, 0xED, 0xFD, 0x29, 0x7F, 0x48, 0xA5, 0xF1, 0x20, 0xA8, 0xFC, 0x68,
  0x60, 0xA5, 0x15, 0xF0, 0x12, 0x30, 0x04, 0xA0, 0xFF, 0xD0, 0x04, 0xA5, 0x7B, 0xA4, 0x7C, 0x85,
  0x75, 0x84, 0x76, 0x4C, 0xC9, 0xDE, 0x68, 0x24, 0xD8, 0x10, 0x05, 0xA2, 0xFE, 0x4C, 0xE9, 0xF2,
  0xA9, 0xEF, 0xA0, 0xDC, 0x20, 0x3A, 0xDB, 0xA5, 0x79, 0xA4, 0x7A, 0x85, 0xB8, 0x84, 0xB9, 0x60,
  0x20, 0x06, 0xE3, 0xA2, 0x01, 0xA0, 0x02, 0xA9, 0x00, 0x8D, 0x01, 0x02, 0xA9, 0x40, 0x20, 0xEB,
  0xDB, 0x60, 0xC9, 0x22, 0xD0, 0x0E, 0x20, 0x81, 0xDE, 0xA9, 0x3B, 0x20, 0xC0, 0xDE, 0x20, 0x3D,
  0xDB, 0x4C, 0xC7, 0xDB, 0x20, 0x5A, 0xDB, 0x20, 0x06, 0xE3, 0xA9, 0x2C, 0x8D, 0xFF, 0x01, 0x20,
  0x2C, 0xD5, 0xAD, 0x00, 0x02, 0xC9, 0x03, 0xD0, 0x10, 0x4C, 0x63, 0xD8, 0x20, 0x5A, 0xDB, 0x4C,
  0x2C, 0xD5, 0xA6, 0x7D, 0xA4, 0x7E, 0xA9, 0x98, 0x2C, 0xA9, 0x00, 0x85, 0x15, 0x86, 0x7F, 0x84,
  0x80, 0x20, 0xE3, 0xDF, 0x85, 0x85, 0x84, 0x86, 0xA5, 0xB8, 0xA4, 0xB9, 0x85, 0x87, 0x84, 0x88,
  0xA6, 0x7F, 0xA4, 0x80, 0x86, 0xB8, 0x84, 0xB9, 0x20, 0xB7, 0x00, 0xD0, 0x1E, 0x24, 0x15, 0x50,
  0x0E, 0x20, 0x0C, 0xFD, 0x29, 0x7F, 0x8D, 0x00, 0x02, 0xA2, 0xFF, 0xA0, 0x01, 0xD0, 0x08, 0x30,
  0x7F, 0x20, 0x5A, 0xDB, 0x20, 0xDC, 0xDB, 0x86, 0xB8, 0x84, 0xB9, 0x20, 0xB1, 0x00, 0x24, 0x11,
  0x10, 0x31, 0x24, 0x15, 0x50, 0x09, 0xE8, 0x86, 0xB8, 0xA9, 0x00, 0x85, 0x0D, 0xF0, 0x0C, 0x85,
  0x0D, 0xC9, 0x22, 0xF0, 0x07, 0xA9, 0x3A, 0x85, 0x0D, 0xA9, 0x2C, 0x18, 0x85, 0x0E, 0xA5, 0xB8,
  0xA4, 0xB9, 0x69, 0x00, 0x90, 0x01, 0xC8, 0x20, 0xED, 0xE3, 0x20, 0x3D, 0xE7, 0x20, 0x7B, 0xDA,
  0x4C, 0x72, 0xDC, 0x48, 0xAD, 0x00, 0x02, 0xF0, 0x30, 0x68, 0x20, 0x4A, 0xEC, 0xA5, 0x12, 0x20,
  0x63, 0xDA, 0x20, 0xB7, 0x00, 0xF0, 0x07, 0xC9, 0x2C, 0xF0, 0x03, 0x4C, 0x71, 0xDB, 0xA5, 0xB8,
  0xA4, 0xB9, 0x85, 0x7F, 0x84, 0x80, 0xA5, 0x87, 0xA4, 0x88, 0x85, 0xB8, 0x84, 0xB9, 0x20, 0xB7,
  0x00, 0xF0, 0x33, 0x20, 0xBE, 0xDE, 0x4C, 0xF1, 0xDB, 0xA5, 0x15, 0xD0, 0xCC, 0x4C, 0x86, 0xDB,
  0x20, 0xA3, 0xD9, 0xC8, 0xAA, 0xD0, 0x12, 0xA2, 0x2A, 0xC8, 0xB1, 0xB8, 0xF0, 0x5F, 0xC8, 0xB1,
  0xB8, 0x85, 0x7B, 0xC8, 0xB1, 0xB8, 0xC8, 0x85, 0x7C, 0xB1, 0xB8, 0xAA, 0x20, 0x98, 0xD9, 0xE0,
  0x83, 0xD0, 0xDD, 0x4C, 0x2B, 0xDC, 0xA5, 0x7F, 0xA4, 0x80, 0xA6, 0x15, 0x10, 0x03, 0x4C, 0x53,
  0xD8, 0xA0, 0x00, 0xB1, 0x7F, 0xF0, 0x07, 0xA9, 0xDF, 0xA0, 0xDC, 0x4C, 0x3A, 0xDB, 0x60, 0x3F,
  0x45, 0x58, 0x54, 0x52, 0x41, 0x20, 0x49, 0x47, 0x4E, 0x4F, 0x52, 0x45, 0x44, 0x0D, 0x00, 0x3F,
  0x52, 0x45, 0x45, 0x4E, 0x54, 0x45, 0x52, 0x0D, 0x00, 0xD0, 0x04, 0xA0, 0x00, 0xF0, 0x03, 0x20,
  0xE3, 0xDF, 0x85, 0x85, 0x84, 0x86, 0x20, 0x65, 0xD3, 0xF0, 0x04, 0xA2, 0x00, 0xF0, 0x69, 0x9A,
  0xE8, 0xE8, 0xE8, 0xE8, 0x8A, 0xE8, 0xE8, 0xE8, 0xE8, 0xE8, 0xE8, 0x86, 0x60, 0xA0, 0x01, 0x20,
  0xF9, 0xEA, 0xBA, 0xBD, 0x09, 0x01, 0x85, 0xA2, 0xA5, 0x85, 0xA4, 0x86, 0x20, 0xBE, 0xE7, 0x20,
  0x27, 0xEB, 0xA0, 0x01, 0x20, 0xB4, 0xEB, 0xBA, 0x38, 0xFD, 0x09, 0x01, 0xF0, 0x17, 0xBD, 0x0F,
  0x01, 0x85, 0x75, 0xBD, 0x10, 0x01, 0x85, 0x76, 0xBD, 0x12, 0x01, 0x85, 0xB8, 0xBD, 0x11, 0x01,
  0x85, 0xB9, 0x4C, 0xD2, 0xD7, 0x8A, 0x69, 0x11, 0xAA, 0x9A, 0x20, 0xB7, 0x00, 0xC9, 0x2C, 0xD0,
  0xF1, 0x20, 0xB1, 0x00, 0x20, 0xFF, 0xDC, 0x20, 0x7B, 0xDD, 0x18, 0x24, 0x38, 0x24, 0x11, 0x30,
  0x03, 0xB0, 0x03, 0x60, 0xB0, 0xFD, 0xA2, 0xA3, 0x4C, 0x12, 0xD4, 0xA6, 0xB8, 0xD0, 0x02, 0xC6,
  0xB9, 0xC6, 0xB8, 0xA2, 0x00, 0x24, 0x48, 0x8A, 0x48, 0xA9, 0x01, 0x20, 0xD6, 0xD3, 0x20, 0x60,
  0xDE, 0xA9, 0x00, 0x85, 0x89, 0x20, 0xB7, 0x00, 0x38, 0xE9, 0xCF, 0x90, 0x17, 0xC9, 0x03, 0xB0,
  0x13, 0xC9, 0x01, 0x2A, 0x49, 0x01, 0x45, 0x89, 0xC5, 0x89, 0x90, 0x61, 0x85, 0x89, 0x20, 0xB1,
  0x00, 0x4C, 0x98, 0xDD, 0xA6, 0x89, 0xD0, 0x2C, 0xB0, 0x7B, 0x69, 0x07, 0x90, 0x77, 0x65, 0x11,
  0xD0, 0x03, 0x4C, 0x97, 0xE5, 0x69, 0xFF, 0x85, 0x5E, 0x0A, 0x65, 0x5E, 0xA8, 0x68, 0xD9, 0xB2,
  0xD0, 0xB0, 0x67, 0x20, 0x6A, 0xDD, 0x48, 0x20, 0xFD, 0xDD, 0x68, 0xA4, 0x87, 0x10, 0x17, 0xAA,
  0xF0, 0x56, 0xD0, 0x5F, 0x46, 0x11, 0x8A, 0x2A, 0xA6, 0xB8, 0xD0, 0x02, 0xC6, 0xB9, 0xC6, 0xB8,
  0xA0, 0x1B, 0x85, 0x89, 0xD0, 0xD7, 0xD9, 0xB2, 0xD0, 0xB0, 0x48, 0x90, 0xD9, 0xB9, 0xB4, 0xD0,
  0x48, 0xB9, 0xB3, 0xD0, 0x48, 0x20, 0x10, 0xDE, 0xA5, 0x89, 0x4C, 0x86, 0xDD, 0x4C, 0xC9, 0xDE,
  0xA5, 0xA2, 0xBE, 0xB2, 0xD0, 0xA8, 0x68, 0x85, 0x5E, 0xE6, 0x5E, 0x68, 0x85, 0x5F, 0x98, 0x48,
  0x20, 0x72, 0xEB, 0xA5, 0xA1, 0x48, 0xA5, 0xA0, 0x48, 0xA5, 0x9F, 0x48, 0xA5, 0x9E, 0x48, 0xA5,
  0x9D, 0x48, 0x6C, 0x5E, 0x00, 0xA0, 0xFF, 0x68, 0xF0, 0x23, 0xC9, 0x64, 0xF0, 0x03, 0x20, 0x6A,
  0xDD, 0x84, 0x87, 0x68, 0x4A, 0x85, 0x16, 0x68, 0x85, 0xA5, 0x68, 0x85, 0xA6, 0x68, 0x85, 0xA7,
  0x68, 0x85, 0xA8, 0x68, 0x85, 0xA9, 0x68, 0x85, 0xAA, 0x45, 0xA2, 0x85, 0xAB, 0xA5, 0x9D, 0x60,
  0xA9, 0x00, 0x85, 0x11, 0x20, 0xB1, 0x00, 0xB0, 0x03, 0x4C, 0x4A, 0xEC, 0x20, 0x7D, 0xE0, 0xB0,
  0x64, 0xC9, 0x2E, 0xF0, 0xF4, 0xC9, 0xC9, 0xF0, 0x55, 0xC9, 0xC8, 0xF0, 0xE7, 0xC9, 0x22, 0xD0,
  0x0F, 0xA5, 0xB8, 0xA4, 0xB9, 0x69, 0x00, 0x90, 0x01, 0xC8, 0x20, 0xE7, 0xE3, 0x4C, 0x3D, 0xE7,
  0xC9, 0xC6, 0xD0, 0x10, 0xA0, 0x18, 0xD0, 0x38, 0xA5, 0x9D, 0xD0, 0x03, 0xA0, 0x01, 0x2C, 0xA0,
  0x00, 0x4C, 0x01, 0xE3, 0xC9, 0xC2, 0xD0, 0x03, 0x4C, 0x54, 0xE3, 0xC9, 0xD2, 0x90, 0x03, 0x4C,
  0x0C, 0xDF, 0x20, 0xBB, 0xDE, 0x20, 0x7B, 0xDD, 0xA9, 0x29, 0x2C, 0xA9, 0x28, 0x2C, 0xA9, 0x2C,
  0xA0, 0x00, 0xD1, 0xB8, 0xD0, 0x03, 0x4C, 0xB1, 0x00, 0xA2, 0x10, 0x4C, 0x12, 0xD4, 0xA0, 0x15,
  0x68, 0x68, 0x4C, 0xD7, 0xDD, 0x20, 0xE3, 0xDF, 0x85, 0xA0, 0x84, 0xA1, 0xA6, 0x11, 0xF0, 0x05,
  0xA2, 0x00, 0x86, 0xAC, 0x60, 0xA6, 0x12, 0x10, 0x0D, 0xA0, 0x00, 0xB1, 0xA0, 0xAA, 0xC8, 0xB1,
  0xA0, 0xA8, 0x8A, 0x4C, 0xF2, 0xE2, 0x4C, 0xF9, 0xEA, 0x20, 0xB1, 0x00, 0x20, 0xEC, 0xF1, 0x8A,
  0xA4, 0xF0, 0x20, 0x71, 0xF8, 0xA8, 0x20, 0x01, 0xE3, 0x4C, 0xB8, 0xDE, 0xC9, 0xD7, 0xF0, 0xE9,
  0x0A, 0x48, 0xAA, 0x20, 0xB1, 0x00, 0xE0, 0xCF, 0x90, 0x20, 0x20, 0xBB, 0xDE, 0x20, 0x7B, 0xDD,
  0x20, 0xBE, 0xDE, 0x20, 0x6C, 0xDD, 0x68, 0xAA, 0xA5, 0xA1, 0x48, 0xA5, 0xA0, 0x48, 0x8A, 0x48,
  0x20, 0xF8, 0xE6, 0x68, 0xA8, 0x8A, 0x48, 0x4C, 0x3F, 0xDF, 0x20, 0xB2, 0xDE, 0x68, 0xA8, 0xB9,
  0xDC, 0xCF, 0x85, 0x91, 0xB9, 0xDD, 0xCF, 0x85, 0x92, 0x20, 0x90, 0x00, 0x4C, 0x6A, 0xDD, 0xA5,
  0xA5, 0x05, 0x9D, 0xD0, 0x0B, 0xA5, 0xA5, 0xF0, 0x04, 0xA5, 0x9D, 0xD0, 0x03, 0xA0, 0x00, 0x2C,
  0xA0, 0x01, 0x4C, 0x01, 0xE3, 0x20, 0x6D, 0xDD, 0xB0, 0x13, 0xA5, 0xAA, 0x09, 0x7F, 0x25, 0xA6,
  0x85, 0xA6, 0xA9, 0xA5, 0xA0, 0x00, 0x20, 0xB2, 0xEB, 0xAA, 0x4C, 0xB0, 0xDF, 0xA9, 0x00, 0x85,
  0x11, 0xC6, 0x89, 0x20, 0x00, 0xE6, 0x85, 0x9D, 0x86, 0x9E, 0x84, 0x9F, 0xA5, 0xA8, 0xA4, 0xA9,
  0x20, 0x04, 0xE6, 0x86, 0xA8, 0x84, 0xA9, 0xAA, 0x38, 0xE5, 0x9D, 0xF0, 0x08, 0xA9, 0x01, 0x90,
  0x04, 0xA6, 0x9D, 0xA9, 0xFF, 0x85, 0xA2, 0xA0, 0xFF, 0xE8, 0xC8, 0xCA, 0xD0, 0x07, 0xA6, 0xA2,
  0x30, 0x0F, 0x18, 0x90, 0x0C, 0xB1, 0xA8, 0xD1, 0x9E, 0xF0, 0xEF, 0xA2, 0xFF, 0xB0, 0x02, 0xA2,
  0x01, 0xE8, 0x8A, 0x2A, 0x25, 0x16, 0xF0, 0x02, 0xA9, 0x01, 0x4C, 0x93, 0xEB, 0x20, 0xFB, 0xE6,
  0x20, 0x1E, 0xFB, 0x4C, 0x01, 0xE3, 0x20, 0xBE, 0xDE, 0xAA, 0x20, 0xE8, 0xDF, 0x20, 0xB7, 0x00,
  0xD0, 0xF4, 0x60, 0xA2, 0x00, 0x20, 0xB7, 0x00, 0x86, 0x10, 0x85, 0x81, 0x20, 0xB7, 0x00, 0x20,
  0x7D, 0xE0, 0xB0, 0x03, 0x4C, 0xC9, 0xDE, 0xA2, 0x00, 0x86, 0x11, 0x86, 0x12, 0x4C, 0x07, 0xE0,
  0x4C, 0x28, 0xF1, 0x4C, 0x3C, 0xD4, 0x00, 0x20, 0xB1, 0x00, 0x90, 0x05, 0x20, 0x7D, 0xE0, 0x90,
  0x0B, 0xAA, 0x20, 0xB1, 0x00, 0x90, 0xFB, 0x20, 0x7D, 0xE0, 0xB0, 0xF6, 0xC9, 0x24, 0xD0, 0x06,
  0xA9, 0xFF, 0x85, 0x11, 0xD0, 0x10, 0xC9, 0x25, 0xD0, 0x13, 0xA5, 0x14, 0x30, 0xC6, 0xA9, 0x80,
  0x85, 0x12, 0x05, 0x81, 0x85, 0x81, 0x8A, 0x09, 0x80, 0xAA, 0x20, 0xB1, 0x00, 0x86, 0x82, 0x38,
  0x05, 0x14, 0xE9, 0x28, 0xD0, 0x03, 0x4C, 0x1E, 0xE1, 0x24, 0x14, 0x30, 0x02, 0x70, 0xF7, 0xA9,
  0x00, 0x85, 0x14, 0xA5, 0x69, 0xA6, 0x6A, 0xA0, 0x00, 0x86, 0x9C, 0x85, 0x9B, 0xE4, 0x6C, 0xD0,
  0x04, 0xC5, 0x6B, 0xF0, 0x22, 0xA5, 0x81, 0xD1, 0x9B, 0xD0, 0x08, 0xA5, 0x82, 0xC8, 0xD1, 0x9B,
  0xF0, 0x6C, 0x88, 0x18, 0xA5, 0x9B, 0x69, 0x07, 0x90, 0xE1, 0xE8, 0xD0, 0xDC, 0xC9, 0x41, 0x90,
  0x05, 0xE9, 0x5B, 0x38, 0xE9, 0xA5, 0x60, 0x68, 0x48, 0xC9, 0xD7, 0xD0, 0x0F, 0xBA, 0xBD, 0x02,
  0x01, 0xC9, 0xDE, 0xD0, 0x07, 0xA9, 0x9A, 0xA0, 0xE0, 0x60, 0x00, 0x00, 0xA5, 0x6B, 0xA4, 0x6C,
  0x85, 0x9B, 0x84, 0x9C, 0xA5, 0x6D, 0xA4, 0x6E, 0x85, 0x96, 0x84, 0x97, 0x18, 0x69, 0x07, 0x90,
  0x01, 0xC8, 0x85, 0x94, 0x84, 0x95, 0x20, 0x93, 0xD3, 0xA5, 0x94, 0xA4, 0x95, 0xC8, 0x85, 0x6B,
  0x84, 0x6C, 0xA0, 0x00, 0xA5, 0x81, 0x91, 0x9B, 0xC8, 0xA5, 0x82, 0x91, 0x9B, 0xA9, 0x00, 0xC8,
  0x91, 0x9B, 0xC8, 0x91, 0x9B, 0xC8, 0x91, 0x9B, 0xC8, 0x91, 0x9B, 0xC8, 0x91, 0x9B, 0xA5, 0x9B,
  0x18, 0x69, 0x02, 0xA4, 0x9C, 0x90, 0x01, 0xC8, 0x85, 0x83, 0x84, 0x84, 0x60, 0xA5, 0x0F, 0x0A,
  0x69, 0x05, 0x65, 0x9B, 0xA4, 0x9C, 0x90, 0x01, 0xC8, 0x85, 0x94, 0x84, 0x95, 0x60, 0x90, 0x80,
  0x00, 0x00, 0x20, 0xB1, 0x00, 0x20, 0x67, 0xDD, 0xA5, 0xA2, 0x30, 0x0D, 0xA5, 0x9D, 0xC9, 0x90,
  0x90, 0x09, 0xA9, 0xFE, 0xA0, 0xE0, 0x20, 0xB2, 0xEB, 0xD0, 0x7E, 0x4C, 0xF2, 0xEB, 0xA5, 0x14,
  0xD0, 0x47, 0xA5, 0x10, 0x05, 0x12, 0x48, 0xA5, 0x11, 0x48, 0xA0, 0x00, 0x98, 0x48, 0xA5, 0x82,
  0x48, 0xA5, 0x81, 0x48, 0x20, 0x02, 0xE1, 0x68, 0x85, 0x81, 0x68, 0x85, 0x82, 0x68, 0xA8, 0xBA,
  0xBD, 0x02, 0x01, 0x48, 0xBD, 0x01, 0x01, 0x48, 0xA5, 0xA0, 0x9D, 0x02, 0x01, 0xA5, 0xA1, 0x9D,
  0x01, 0x01, 0xC8, 0x20, 0xB7, 0x00, 0xC9, 0x2C, 0xF0, 0xD2, 0x84, 0x0F, 0x20, 0xB8, 0xDE, 0x68,
  0x85, 0x11, 0x68, 0x85, 0x12, 0x29, 0x7F, 0x85, 0x10, 0xA6, 0x6B, 0xA5, 0x6C, 0x86, 0x9B, 0x85,
  0x9C, 0xC5, 0x6E, 0xD0, 0x04, 0xE4, 0x6D, 0xF0, 0x3F, 0xA0, 0x00, 0xB1, 0x9B, 0xC8, 0xC5, 0x81,
  0xD0, 0x06, 0xA5, 0x82, 0xD1, 0x9B, 0xF0, 0x16, 0xC8, 0xB1, 0x9B, 0x18, 0x65, 0x9B, 0xAA, 0xC8,
  0xB1, 0x9B, 0x65, 0x9C, 0x90, 0xD7, 0xA2, 0x6B, 0x2C, 0xA2, 0x35, 0x4C, 0x12, 0xD4, 0xA2, 0x78,
  0xA5, 0x10, 0xD0, 0xF7, 0xA5, 0x14, 0xF0, 0x02, 0x38, 0x60, 0x20, 0xED, 0xE0, 0xA5, 0x0F, 0xA0,
  0x04, 0xD1, 0x9B, 0xD0, 0xE1, 0x4C, 0x4B, 0xE2, 0xA5, 0x14, 0xF0, 0x05, 0xA2, 0x2A, 0x4C, 0x12,
  0xD4, 0x20, 0xED, 0xE0, 0x20, 0xE3, 0xD3, 0xA9, 0x00, 0xA8, 0x85, 0xAE, 0xA2, 0x05, 0xA5, 0x81,
  0x91, 0x9B, 0x10, 0x01, 0xCA, 0xC8, 0xA5, 0x82, 0x91, 0x9B, 0x10, 0x02, 0xCA, 0xCA, 0x86, 0xAD,
  0xA5, 0x0F, 0xC8, 0xC8, 0xC8, 0x91, 0x9B, 0xA2, 0x0B, 0xA9, 0x00, 0x24, 0x10, 0x50, 0x08, 0x68,
  0x18, 0x69, 0x01, 0xAA, 0x68, 0x69, 0x00, 0xC8, 0x91, 0x9B, 0xC8, 0x8A, 0x91, 0x9B, 0x20, 0xAD,
  0xE2, 0x86, 0xAD, 0x85, 0xAE, 0xA4, 0x5E, 0xC6, 0x0F, 0xD0, 0xDC, 0x65, 0x95, 0xB0, 0x5D, 0x85,
  0x95, 0xA8, 0x8A, 0x65, 0x94, 0x90, 0x03, 0xC8, 0xF0, 0x52, 0x20, 0xE3, 0xD3, 0x85, 0x6D, 0x84,
  0x6E, 0xA9, 0x00, 0xE6, 0xAE, 0xA4, 0xAD, 0xF0, 0x05, 0x88, 0x91, 0x94, 0xD0, 0xFB, 0xC6, 0x95,
  0xC6, 0xAE, 0xD0, 0xF5, 0xE6, 0x95, 0x38, 0xA5, 0x6D, 0xE5, 0x9B, 0xA0, 0x02, 0x91, 0x9B, 0xA5,
  0x6E, 0xC8, 0xE5, 0x9C, 0x91, 0x9B, 0xA5, 0x10, 0xD0, 0x62, 0xC8, 0xB1, 0x9B, 0x85, 0x0F, 0xA9,
  0x00, 0x85, 0xAD, 0x85, 0xAE, 0xC8, 0x68, 0xAA, 0x85, 0xA0, 0x68, 0x85, 0xA1, 0xD1, 0x9B, 0x90,
  0x0E, 0xD0, 0x06, 0xC8, 0x8A, 0xD1, 0x9B, 0x90, 0x07, 0x4C, 0x96, 0xE1, 0x4C, 0x10, 0xD4, 0xC8,
  0xA5, 0xAE, 0x05, 0xAD, 0x18, 0xF0, 0x0A, 0x20, 0xAD, 0xE2, 0x8A, 0x65, 0xA0, 0xAA, 0x98, 0xA4,
  0x5E, 0x65, 0xA1, 0x86, 0xAD, 0xC6, 0x0F, 0xD0, 0xCA, 0x85, 0xAE, 0xA2, 0x05, 0xA5, 0x81, 0x10,
  0x01, 0xCA, 0xA5, 0x82, 0x10, 0x02, 0xCA, 0xCA, 0x86, 0x64, 0xA9, 0x00, 0x20, 0xB6, 0xE2, 0x8A,
  0x65, 0x94, 0x85, 0x83, 0x98, 0x65, 0x95, 0x85, 0x84, 0xA8, 0xA5, 0x83, 0x60, 0x84, 0x5E, 0xB1,
  0x9B, 0x85, 0x64, 0x88, 0xB1, 0x9B, 0x85, 0x65, 0xA9, 0x10, 0x85, 0x99, 0xA2, 0x00, 0xA0, 0x00,
  0x8A, 0x0A, 0xAA, 0x98, 0x2A, 0xA8, 0xB0, 0xA4, 0x06, 0xAD, 0x26, 0xAE, 0x90, 0x0B, 0x18, 0x8A,
  0x65, 0x64, 0xAA, 0x98, 0x65, 0x65, 0xA8, 0xB0, 0x93, 0xC6, 0x99, 0xD0, 0xE3, 0x60, 0xA5, 0x11,
  0xF0, 0x03, 0x20, 0x00, 0xE6, 0x20, 0x84, 0xE4, 0x38, 0xA5, 0x6F, 0xE5, 0x6D, 0xA8, 0xA5, 0x70,
  0xE5, 0x6E, 0xA2, 0x00, 0x86, 0x11, 0x85, 0x9E, 0x84, 0x9F, 0xA2, 0x90, 0x4C, 0x9B, 0xEB, 0xA4,
  0x24, 0xA9, 0x00, 0x38, 0xF0, 0xEC, 0xA6, 0x76, 0xE8, 0xD0, 0xA1, 0xA2, 0x95, 0x2C, 0xA2, 0xE0,
  0x4C, 0x12, 0xD4, 0x20, 0x41, 0xE3, 0x20, 0x06, 0xE3, 0x20, 0xBB, 0xDE, 0xA9, 0x80, 0x85, 0x14,
  0x20, 0xE3, 0xDF, 0x20, 0x6A, 0xDD, 0x20, 0xB8, 0xDE, 0xA9, 0xD0, 0x20, 0xC0, 0xDE, 0x48, 0xA5,
  0x84, 0x48, 0xA5, 0x83, 0x48, 0xA5, 0xB9, 0x48, 0xA5, 0xB8, 0x48, 0x20, 0x95, 0xD9, 0x4C, 0xAF,
  0xE3, 0xA9, 0xC2, 0x20, 0xC0, 0xDE, 0x09, 0x80, 0x85, 0x14, 0x20, 0xEA, 0xDF, 0x85, 0x8A, 0x84,
  0x8B, 0x4C, 0x6A, 0xDD, 0x20, 0x41, 0xE3, 0xA5, 0x8B, 0x48, 0xA5, 0x8A, 0x48, 0x20, 0xB2, 0xDE,
  0x20, 0x6A, 0xDD, 0x68, 0x85, 0x8A, 0x68, 0x85, 0x8B, 0xA0, 0x02, 0xB1, 0x8A, 0x85, 0x83, 0xAA,
  0xC8, 0xB1, 0x8A, 0xF0, 0x99, 0x85, 0x84, 0xC8, 0xB1, 0x83, 0x48, 0x88, 0x10, 0xFA, 0xA4, 0x84,
  0x20, 0x2B, 0xEB, 0xA5, 0xB9, 0x48, 0xA5, 0xB8, 0x48, 0xB1, 0x8A, 0x85, 0xB8, 0xC8, 0xB1, 0x8A,
  0x85, 0xB9, 0xA5, 0x84, 0x48, 0xA5, 0x83, 0x48, 0x20, 0x67, 0xDD, 0x68, 0x85, 0x8A, 0x68, 0x85,
  0x8B, 0x20, 0xB7, 0x00, 0xF0, 0x03, 0x4C, 0xC9, 0xDE, 0x68, 0x85, 0xB8, 0x68, 0x85, 0xB9, 0xA0,
  0x00, 0x68, 0x91, 0x8A, 0x68, 0xC8, 0x91, 0x8A, 0x68, 0xC8, 0x91, 0x8A, 0x68, 0xC8, 0x91, 0x8A,
  0x68, 0xC8, 0x91, 0x8A, 0x60, 0x20, 0x6A, 0xDD, 0xA0, 0x00, 0x20, 0x36, 0xED, 0x68, 0x68, 0xA9,
  0xFF, 0xA0, 0x00, 0xF0, 0x12, 0xA6, 0xA0, 0xA4, 0xA1, 0x86, 0x8C, 0x84, 0x8D, 0x20, 0x52, 0xE4,
  0x86, 0x9E, 0x84, 0x9F, 0x85, 0x9D, 0x60, 0xA2, 0x22, 0x86, 0x0D, 0x86, 0x0E, 0x85, 0xAB, 0x84,
  0xAC, 0x85, 0x9E, 0x84, 0x9F, 0xA0, 0xFF, 0xC8, 0xB1, 0xAB, 0xF0, 0x0C, 0xC5, 0x0D, 0xF0, 0x04,
  0xC5, 0x0E, 0xD0, 0xF3, 0xC9, 0x22, 0xF0, 0x01, 0x18, 0x84, 0x9D, 0x98, 0x65, 0xAB, 0x85, 0xAD,
  0xA6, 0xAC, 0x90, 0x01, 0xE8, 0x86, 0xAE, 0xA5, 0xAC, 0xF0, 0x04, 0xC9, 0x02, 0xD0, 0x0B, 0x98,
  0x20, 0xD5, 0xE3, 0xA6, 0xAB, 0xA4, 0xAC, 0x20, 0xE2, 0xE5, 0xA6, 0x52, 0xE0, 0x5E, 0xD0, 0x05,
  0xA2, 0xBF, 0x4C, 0x12, 0xD4, 0xA5, 0x9D, 0x95, 0x00, 0xA5, 0x9E, 0x95, 0x01, 0xA5, 0x9F, 0x95,
  0x02, 0xA0, 0x00, 0x86, 0xA0, 0x84, 0xA1, 0x88, 0x84, 0x11, 0x86, 0x53, 0xE8, 0xE8, 0xE8, 0x86,
  0x52, 0x60, 0x46, 0x13, 0x48, 0x49, 0xFF, 0x38, 0x65, 0x6F, 0xA4, 0x70, 0xB0, 0x01, 0x88, 0xC4,
  0x6E, 0x90, 0x11, 0xD0, 0x04, 0xC5, 0x6D, 0x90, 0x0B, 0x85, 0x6F, 0x84, 0x70, 0x85, 0x71, 0x84,
  0x72, 0xAA, 0x68, 0x60, 0xA2, 0x4D, 0xA5, 0x13, 0x30, 0xB8, 0x20, 0x84, 0xE4, 0xA9, 0x80, 0x85,
  0x13, 0x68, 0xD0, 0xD0, 0xA6, 0x73, 0xA5, 0x74, 0x86, 0x6F, 0x85, 0x70, 0xA0, 0x00, 0x84, 0x8B,
  0xA5, 0x6D, 0xA6, 0x6E, 0x85, 0x9B, 0x86, 0x9C, 0xA9, 0x55, 0xA2, 0x00, 0x85, 0x5E, 0x86, 0x5F,
  0xC5, 0x52, 0xF0, 0x05, 0x20, 0x23, 0xE5, 0xF0, 0xF7, 0xA9, 0x07, 0x85, 0x8F, 0xA5, 0x69, 0xA6,
  0x6A, 0x85, 0x5E, 0x86, 0x5F, 0xE4, 0x6C, 0xD0, 0x04, 0xC5, 0x6B, 0xF0, 0x05, 0x20, 0x19, 0xE5,
  0xF0, 0xF3, 0x85, 0x94, 0x86, 0x95, 0xA9, 0x03, 0x85, 0x8F, 0xA5, 0x94, 0xA6, 0x95, 0xE4, 0x6E,
  0xD0, 0x07, 0xC5, 0x6D, 0xD0, 0x03, 0x4C, 0x62, 0xE5, 0x85, 0x5E, 0x86, 0x5F, 0xA0, 0x00, 0xB1,
  0x5E, 0xAA, 0xC8, 0xB1, 0x5E, 0x08, 0xC8, 0xB1, 0x5E, 0x65, 0x94, 0x85, 0x94, 0xC8, 0xB1, 0x5E,
  0x65, 0x95, 0x85, 0x95, 0x28, 0x10, 0xD3, 0x8A, 0x30, 0xD0, 0xC8, 0xB1, 0x5E, 0xA0, 0x00, 0x0A,
  0x69, 0x05, 0x65, 0x5E, 0x85, 0x5E, 0x90, 0x02, 0xE6, 0x5F, 0xA6, 0x5F, 0xE4, 0x95, 0xD0, 0x04,
  0xC5, 0x94, 0xF0, 0xBA, 0x20, 0x23, 0xE5, 0xF0, 0xF3, 0xB1, 0x5E, 0x30, 0x35, 0xC8, 0xB1, 0x5E,
  0x10, 0x30, 0xC8, 0xB1, 0x5E, 0xF0, 0x2B, 0xC8, 0xB1, 0x5E, 0xAA, 0xC8, 0xB1, 0x5E, 0xC5, 0x70,
  0x90, 0x06, 0xD0, 0x1E, 0xE4, 0x6F, 0xB0, 0x1A, 0xC5, 0x9C, 0x90, 0x16, 0xD0, 0x04, 0xE4, 0x9B,
  0x90, 0x10, 0x86, 0x9B, 0x85, 0x9C, 0xA5, 0x5E, 0xA6, 0x5F, 0x85, 0x8A, 0x86, 0x8B, 0xA5, 0x8F,
  0x85, 0x91, 0xA5, 0x8F, 0x18, 0x65, 0x5E, 0x85, 0x5E, 0x90, 0x02, 0xE6, 0x5F, 0xA6, 0x5F, 0xA0,
  0x00, 0x60, 0xA6, 0x8B, 0xF0, 0xF7, 0xA5, 0x91, 0x29, 0x04, 0x4A, 0xA8, 0x85, 0x91, 0xB1, 0x8A,
  0x65, 0x9B, 0x85, 0x96, 0xA5, 0x9C, 0x69, 0x00, 0x85, 0x97, 0xA5, 0x6F, 0xA6, 0x70, 0x85, 0x94,
  0x86, 0x95, 0x20, 0x9A, 0xD3, 0xA4, 0x91, 0xC8, 0xA5, 0x94, 0x91, 0x8A, 0xAA, 0xE6, 0x95, 0xA5,
  0x95, 0xC8, 0x91, 0x8A, 0x4C, 0x88, 0xE4, 0xA5, 0xA1, 0x48, 0xA5, 0xA0, 0x48, 0x20, 0x60, 0xDE,
  0x20, 0x6C, 0xDD, 0x68, 0x85, 0xAB, 0x68, 0x85, 0xAC, 0xA0, 0x00, 0xB1, 0xAB, 0x18, 0x71, 0xA0,
  0x90, 0x05, 0xA2, 0xB0, 0x4C, 0x12, 0xD4, 0x20, 0xD5, 0xE3, 0x20, 0xD4, 0xE5, 0xA5, 0x8C, 0xA4,
  0x8D, 0x20, 0x04, 0xE6, 0x20, 0xE6, 0xE5, 0xA5, 0xAB, 0xA4, 0xAC, 0x20, 0x04, 0xE6, 0x20, 0x2A,
  0xE4, 0x4C, 0x95, 0xDD, 0xA0, 0x00, 0xB1, 0xAB, 0x48, 0xC8, 0xB1, 0xAB, 0xAA, 0xC8, 0xB1, 0xAB,
  0xA8, 0x68, 0x86, 0x5E, 0x84, 0x5F, 0xA8, 0xF0, 0x0A, 0x48, 0x88, 0xB1, 0x5E, 0x91, 0x71, 0x98,
  0xD0, 0xF8, 0x68, 0x18, 0x65, 0x71, 0x85, 0x71, 0x90, 0x02, 0xE6, 0x72, 0x60, 0x20, 0x6C, 0xDD,
  0xA5, 0xA0, 0xA4, 0xA1, 0x85, 0x5E, 0x84, 0x5F, 0x20, 0x35, 0xE6, 0x08, 0xA0, 0x00, 0xB1, 0x5E,
  0x48, 0xC8, 0xB1, 0x5E, 0xAA, 0xC8, 0xB1, 0x5E, 0xA8, 0x68, 0x28, 0xD0, 0x13, 0xC4, 0x70, 0xD0,
  0x0F, 0xE4, 0x6F, 0xD0, 0x0B, 0x48, 0x18, 0x65, 0x6F, 0x85, 0x6F, 0x90, 0x02, 0xE6, 0x70, 0x68,
  0x86, 0x5E, 0x84, 0x5F, 0x60, 0xC4, 0x54, 0xD0, 0x0C, 0xC5, 0x53, 0xD0, 0x08, 0x85, 0x52, 0xE9,
  0x03, 0x85, 0x53, 0xA0, 0x00, 0x60, 0x20, 0xFB, 0xE6, 0x8A, 0x48, 0xA9, 0x01, 0x20, 0xDD, 0xE3,
  0x68, 0xA0, 0x00, 0x91, 0x9E, 0x68, 0x68, 0x4C, 0x2A, 0xE4, 0x20, 0xB9, 0xE6, 0xD1, 0x8C, 0x98,
  0x90, 0x04, 0xB1, 0x8C, 0xAA, 0x98, 0x48, 0x8A, 0x48, 0x20, 0xDD, 0xE3, 0xA5, 0x8C, 0xA4, 0x8D,
  0x20, 0x04, 0xE6, 0x68, 0xA8, 0x68, 0x18, 0x65, 0x5E, 0x85, 0x5E, 0x90, 0x02, 0xE6, 0x5F, 0x98,
  0x20, 0xE6, 0xE5, 0x4C, 0x2A, 0xE4, 0x20, 0xB9, 0xE6, 0x18, 0xF1, 0x8C, 0x49, 0xFF, 0x4C, 0x60,
  0xE6, 0xA9, 0xFF, 0x85, 0xA1, 0x20, 0xB7, 0x00, 0xC9, 0x29, 0xF0, 0x06, 0x20, 0xBE, 0xDE, 0x20,
  0xF8, 0xE6, 0x20, 0xB9, 0xE6, 0xCA, 0x8A, 0x48, 0x18, 0xA2, 0x00, 0xF1, 0x8C, 0xB0, 0xB8, 0x49,
  0xFF, 0xC5, 0xA1, 0x90, 0xB3, 0xA5, 0xA1, 0xB0, 0xAF, 0x20, 0xB8, 0xDE, 0x68, 0xA8, 0x68, 0x85,
  0x91, 0x68, 0x68, 0x68, 0xAA, 0x68, 0x85, 0x8C, 0x68, 0x85, 0x8D, 0xA5, 0x91, 0x48, 0x98, 0x48,
  0xA0, 0x00, 0x8A, 0xF0, 0x1D, 0x60, 0x20, 0xDC, 0xE6, 0x4C, 0x01, 0xE3, 0x20, 0xFD, 0xE5, 0xA2,
  0x00, 0x86, 0x11, 0xA8, 0x60, 0x20, 0xDC, 0xE6, 0xF0, 0x08, 0xA0, 0x00, 0xB1, 0x5E, 0xA8, 0x4C,
  0x01, 0xE3, 0x4C, 0x99, 0xE1, 0x20, 0xB1, 0x00, 0x20, 0x67, 0xDD, 0x20, 0x08, 0xE1, 0xA6, 0xA0,
  0xD0, 0xF0, 0xA6, 0xA1, 0x4C, 0xB7, 0x00, 0x20, 0xDC, 0xE6, 0xD0, 0x03, 0x4C, 0x4E, 0xE8, 0xA6,
  0xB8, 0xA4, 0xB9, 0x86, 0xAD, 0x84, 0xAE, 0xA6, 0x5E, 0x86, 0xB8, 0x18, 0x65, 0x5E, 0x85, 0x60,
  0xA6, 0x5F, 0x86, 0xB9, 0x90, 0x01, 0xE8, 0x86, 0x61, 0xA0, 0x00, 0xB1, 0x60, 0x48, 0xA9, 0x00,
  0x91, 0x60, 0x20, 0xB7, 0x00, 0x20, 0x4A, 0xEC, 0x68, 0xA0, 0x00, 0x91, 0x60, 0xA6, 0xAD, 0xA4,
  0xAE, 0x86, 0xB8, 0x84, 0xB9, 0x60, 0x20, 0x67, 0xDD, 0x20, 0x52, 0xE7, 0x20, 0xBE, 0xDE, 0x4C,
  0xF8, 0xE6, 0xA5, 0x9D, 0xC9, 0x91, 0xB0, 0x9A, 0x20, 0xF2, 0xEB, 0xA5, 0xA0, 0xA4, 0xA1, 0x84,
  0x50, 0x85, 0x51, 0x60, 0xA5, 0x50, 0x48, 0xA5, 0x51, 0x48, 0x20, 0x52, 0xE7, 0xA0, 0x00, 0xB1,
  0x50, 0xA8, 0x68, 0x85, 0x51, 0x68, 0x85, 0x50, 0x4C, 0x01, 0xE3, 0x20, 0x46, 0xE7, 0x8A, 0xA0,
  0x00, 0x91, 0x50, 0x60, 0x20, 0x46, 0xE7, 0x86, 0x85, 0xA2, 0x00, 0x20, 0xB7, 0x00, 0xF0, 0x03,
  0x20, 0x4C, 0xE7, 0x86, 0x86, 0xA0, 0x00, 0xB1, 0x50, 0x45, 0x86, 0x25, 0x85, 0xF0, 0xF8, 0x60,
  0xA9, 0x64, 0xA0, 0xEE, 0x4C, 0xBE, 0xE7, 0x20, 0xE3, 0xE9, 0xA5, 0xA2, 0x49, 0xFF, 0x85, 0xA2,
  0x45, 0xAA, 0x85, 0xAB, 0xA5, 0x9D, 0x4C, 0xC1, 0xE7, 0x20, 0xF0, 0xE8, 0x90, 0x3C, 0x20, 0xE3,
  0xE9, 0xD0, 0x03, 0x4C, 0x53, 0xEB, 0xA6, 0xAC, 0x86, 0x92, 0xA2, 0xA5, 0xA5, 0xA5, 0xA8, 0xF0,
  0xCE, 0x38, 0xE5, 0x9D, 0xF0, 0x24, 0x90, 0x12, 0x84, 0x9D, 0xA4, 0xAA, 0x84, 0xA2, 0x49, 0xFF,
  0x69, 0x00, 0xA0, 0x00, 0x84, 0x92, 0xA2, 0x9D, 0xD0, 0x04, 0xA0, 0x00, 0x84, 0xAC, 0xC9, 0xF9,
  0x30, 0xC7, 0xA8, 0xA5, 0xAC, 0x56, 0x01, 0x20, 0x07, 0xE9, 0x24, 0xAB, 0x10, 0x57, 0xA0, 0x9D,
  0xE0, 0xA5, 0xF0, 0x02, 0xA0, 0xA5, 0x38, 0x49, 0xFF, 0x65, 0x92, 0x85, 0xAC, 0xB9, 0x04, 0x00,
  0xF5, 0x04, 0x85, 0xA1, 0xB9, 0x03, 0x00, 0xF5, 0x03, 0x85, 0xA0, 0xB9, 0x02, 0x00, 0xF5, 0x02,
  0x85, 0x9F, 0xB9, 0x01, 0x00, 0xF5, 0x01, 0x85, 0x9E, 0xB0, 0x03, 0x20, 0x9E, 0xE8, 0xA0, 0x00,
  0x98, 0x18, 0xA6, 0x9E, 0xD0, 0x4A, 0xA6, 0x9F, 0x86, 0x9E, 0xA6, 0xA0, 0x86, 0x9F, 0xA6, 0xA1,
  0x86, 0xA0, 0xA6, 0xAC, 0x86, 0xA1, 0x84, 0xAC, 0x69, 0x08, 0xC9, 0x20, 0xD0, 0xE4, 0xA9, 0x00,
  0x85, 0x9D, 0x85, 0xA2, 0x60, 0x65, 0x92, 0x85, 0xAC, 0xA5, 0xA1, 0x65, 0xA9, 0x85, 0xA1, 0xA5,
  0xA0, 0x65, 0xA8, 0x85, 0xA0, 0xA5, 0x9F, 0x65, 0xA7, 0x85, 0x9F, 0xA5, 0x9E, 0x65, 0xA6, 0x85,
  0x9E, 0x4C, 0x8D, 0xE8, 0x69, 0x01, 0x06, 0xAC, 0x26, 0xA1, 0x26, 0xA0, 0x26, 0x9F, 0x26, 0x9E,
  0x10, 0xF2, 0x38, 0xE5, 0x9D, 0xB0, 0xC7, 0x49, 0xFF, 0x69, 0x01, 0x85, 0x9D, 0x90, 0x0E, 0xE6,
  0x9D, 0xF0, 0x42, 0x66, 0x9E, 0x66, 0x9F, 0x66, 0xA0, 0x66, 0xA1, 0x66, 0xAC, 0x60, 0xA5, 0xA2,
  0x49, 0xFF, 0x85, 0xA2, 0xA5, 0x9E, 0x49, 0xFF, 0x85, 0x9E, 0xA5, 0x9F, 0x49, 0xFF, 0x85, 0x9F,
  0xA5, 0xA0, 0x49, 0xFF, 0x85, 0xA0, 0xA5, 0xA1, 0x49, 0xFF, 0x85, 0xA1, 0xA5, 0xAC, 0x49, 0xFF,
  0x85, 0xAC, 0xE6, 0xAC, 0xD0, 0x0E, 0xE6, 0xA1, 0xD0, 0x0A, 0xE6, 0xA0, 0xD0, 0x06, 0xE6, 0x9F,
  0xD0, 0x02, 0xE6, 0x9E, 0x60, 0xA2, 0x45, 0x4C, 0x12, 0xD4, 0xA2, 0x61, 0xB4, 0x04, 0x84, 0xAC,
  0xB4, 0x03, 0x94, 0x04, 0xB4, 0x02, 0x94, 0x03, 0xB4, 0x01, 0x94, 0x02, 0xA4, 0xA4, 0x94, 0x01,
  0x69, 0x08, 0x30, 0xE8, 0xF0, 0xE6, 0xE9, 0x08, 0xA8, 0xA5, 0xAC, 0xB0, 0x14, 0x16, 0x01, 0x90,
  0x02, 0xF6, 0x01, 0x76, 0x01, 0x76, 0x01, 0x76, 0x02, 0x76, 0x03, 0x76, 0x04, 0x6A, 0xC8, 0xD0,
  0xEC, 0x18, 0x60, 0x81, 0x00, 0x00, 0x00, 0x00, 0x03, 0x7F, 0x5E, 0x56, 0xCB, 0x79, 0x80, 0x13,
  0x9B, 0x0B, 0x64, 0x80, 0x76, 0x38, 0x93, 0x16, 0x82, 0x38, 0xAA, 0x3B, 0x20, 0x80, 0x35, 0x04,
  0xF3, 0x34, 0x81, 0x35, 0x04, 0xF3, 0x34, 0x80, 0x80, 0x00, 0x00, 0x00, 0x80, 0x31, 0x72, 0x17,
  0xF8, 0x20, 0x82, 0xEB, 0xF0, 0x02, 0x10, 0x03, 0x4C, 0x99, 0xE1, 0xA5, 0x9D, 0xE9, 0x7F, 0x48,
  0xA9, 0x80, 0x85, 0x9D, 0xA9, 0x2D, 0xA0, 0xE9, 0x20, 0xBE, 0xE7, 0xA9, 0x32, 0xA0, 0xE9, 0x20,
  0x66, 0xEA, 0xA9, 0x13, 0xA0, 0xE9, 0x20, 0xA7, 0xE7, 0xA9, 0x18, 0xA0, 0xE9, 0x20, 0x5C, 0xEF,
  0xA9, 0x37, 0xA0, 0xE9, 0x20, 0xBE, 0xE7, 0x68, 0x20, 0xD5, 0xEC, 0xA9, 0x3C, 0xA0, 0xE9, 0x20,
  0xE3, 0xE9, 0xD0, 0x03, 0x4C, 0xE2, 0xE9, 0x20, 0x0E, 0xEA, 0xA9, 0x00, 0x85, 0x62, 0x85, 0x63,
  0x85, 0x64, 0x85, 0x65, 0xA5, 0xAC, 0x20, 0xB0, 0xE9, 0xA5, 0xA1, 0x20, 0xB0, 0xE9, 0xA5, 0xA0,
  0x20, 0xB0, 0xE9, 0xA5, 0x9F, 0x20, 0xB0, 0xE9, 0xA5, 0x9E, 0x20, 0xB5, 0xE9, 0x4C, 0xE6, 0xEA,
  0xD0, 0x03, 0x4C, 0xDA, 0xE8, 0x4A, 0x09, 0x80, 0xA8, 0x90, 0x19, 0x18, 0xA5, 0x65, 0x65, 0xA9,
  0x85, 0x65, 0xA5, 0x64, 0x65, 0xA8, 0x85, 0x64, 0xA5, 0x63, 0x65, 0xA7, 0x85, 0x63, 0xA5, 0x62,
  0x65, 0xA6, 0x85, 0x62, 0x66, 0x62, 0x66, 0x63, 0x66, 0x64, 0x66, 0x65, 0x66, 0xAC, 0x98, 0x4A,
  0xD0, 0xD6, 0x60, 0x85, 0x5E, 0x84, 0x5F, 0xA0, 0x04, 0xB1, 0x5E, 0x85, 0xA9, 0x88, 0xB1, 0x5E,
  0x85, 0xA8, 0x88, 0xB1, 0x5E, 0x85, 0xA7, 0x88, 0xB1, 0x5E, 0x85, 0xAA, 0x45, 0xA2, 0x85, 0xAB,
  0xA5, 0xAA, 0x09, 0x80, 0x85, 0xA6, 0x88, 0xB1, 0x5E, 0x85, 0xA5, 0xA5, 0x9D, 0x60, 0xA5, 0xA5,
  0xF0, 0x1F, 0x18, 0x65, 0x9D, 0x90, 0x04, 0x30, 0x1D, 0x18, 0x2C, 0x10, 0x14, 0x69, 0x80, 0x85,
  0x9D, 0xD0, 0x03, 0x4C, 0x52, 0xE8, 0xA5, 0xAB, 0x85, 0xA2, 0x60, 0xA5, 0xA2, 0x49, 0xFF, 0x30,
  0x05, 0x68, 0x68, 0x4C, 0x4E, 0xE8, 0x4C, 0xD5, 0xE8, 0x20, 0x63, 0xEB, 0xAA, 0xF0, 0x10, 0x18,
  0x69, 0x02, 0xB0, 0xF2, 0xA2, 0x00, 0x86, 0xAB, 0x20, 0xCE, 0xE7, 0xE6, 0x9D, 0xF0, 0xE7, 0x60,
  0x84, 0x20, 0x00, 0x00, 0x00, 0x20, 0x63, 0xEB, 0xA9, 0x50, 0xA0, 0xEA, 0xA2, 0x00, 0x86, 0xAB,
  0x20, 0xF9, 0xEA, 0x4C, 0x69, 0xEA, 0x20, 0xE3, 0xE9, 0xF0, 0x76, 0x20, 0x72, 0xEB, 0xA9, 0x00,
  0x38, 0xE5, 0x9D, 0x85, 0x9D, 0x20, 0x0E, 0xEA, 0xE6, 0x9D, 0xF0, 0xBA, 0xA2, 0xFC, 0xA9, 0x01,
  0xA4, 0xA6, 0xC4, 0x9E, 0xD0, 0x10, 0xA4, 0xA7, 0xC4, 0x9F, 0xD0, 0x0A, 0xA4, 0xA8, 0xC4, 0xA0,
  0xD0, 0x04, 0xA4, 0xA9, 0xC4, 0xA1, 0x08, 0x2A, 0x90, 0x09, 0xE8, 0x95, 0x65, 0xF0, 0x32, 0x10,
  0x34, 0xA9, 0x01, 0x28, 0xB0, 0x0E, 0x06, 0xA9, 0x26, 0xA8, 0x26, 0xA7, 0x26, 0xA6, 0xB0, 0xE6,
  0x30, 0xCE, 0x10, 0xE2, 0xA8, 0xA5, 0xA9, 0xE5, 0xA1, 0x85, 0xA9, 0xA5, 0xA8, 0xE5, 0xA0, 0x85,
  0xA8, 0xA5, 0xA7, 0xE5, 0x9F, 0x85, 0xA7, 0xA5, 0xA6, 0xE5, 0x9E, 0x85, 0xA6, 0x98, 0x4C, 0xA6,
  0xEA, 0xA9, 0x40, 0xD0, 0xCE, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x85, 0xAC, 0x28, 0x4C, 0xE6,
  0xEA, 0xA2, 0x85, 0x4C, 0x12, 0xD4, 0xA5, 0x62, 0x85, 0x9E, 0xA5, 0x63, 0x85, 0x9F, 0xA5, 0x64,
  0x85, 0xA0, 0xA5, 0x65, 0x85, 0xA1, 0x4C, 0x2E, 0xE8, 0x85, 0x5E, 0x84, 0x5F, 0xA0, 0x04, 0xB1,
  0x5E, 0x85, 0xA1, 0x88, 0xB1, 0x5E, 0x85, 0xA0, 0x88, 0xB1, 0x5E, 0x85, 0x9F, 0x88, 0xB1, 0x5E,
  0x85, 0xA2, 0x09, 0x80, 0x85, 0x9E, 0x88, 0xB1, 0x5E, 0x85, 0x9D, 0x84, 0xAC, 0x60, 0xA2, 0x98,
  0x2C, 0xA2, 0x93, 0xA0, 0x00, 0xF0, 0x04, 0xA6, 0x85, 0xA4, 0x86, 0x20, 0x72, 0xEB, 0x86, 0x5E,
  0x84, 0x5F, 0xA0, 0x04, 0xA5, 0xA1, 0x91, 0x5E, 0x88, 0xA5, 0xA0, 0x91, 0x5E, 0x88, 0xA5, 0x9F,
  0x91, 0x5E, 0x88, 0xA5, 0xA2, 0x09, 0x7F, 0x25, 0x9E, 0x91, 0x5E, 0x88, 0xA5, 0x9D, 0x91, 0x5E,
  0x84, 0xAC, 0x60, 0xA5, 0xAA, 0x85, 0xA2, 0xA2, 0x05, 0xB5, 0xA4, 0x95, 0x9C, 0xCA, 0xD0, 0xF9,
  0x86, 0xAC, 0x60, 0x20, 0x72, 0xEB, 0xA2, 0x06, 0xB5, 0x9C, 0x95, 0xA4, 0xCA, 0xD0, 0xF9, 0x86,
  0xAC, 0x60, 0xA5, 0x9D, 0xF0, 0xFB, 0x06, 0xAC, 0x90, 0xF7, 0x20, 0xC6, 0xE8, 0xD0, 0xF2, 0x4C,
  0x8F, 0xE8, 0xA5, 0x9D, 0xF0, 0x09, 0xA5, 0xA2, 0x2A, 0xA9, 0xFF, 0xB0, 0x02, 0xA9, 0x01, 0x60,
  0x20, 0x82, 0xEB, 0x85, 0x9E, 0xA9, 0x00, 0x85, 0x9F, 0xA2, 0x88, 0xA5, 0x9E, 0x49, 0xFF, 0x2A,
  0xA9, 0x00, 0x85, 0xA1, 0x85, 0xA0, 0x86, 0x9D, 0x85, 0xAC, 0x85, 0xA2, 0x4C, 0x29, 0xE8, 0x46,
  0xA2, 0x60, 0x85, 0x60, 0x84, 0x61, 0xA0, 0x00, 0xB1, 0x60, 0xC8, 0xAA, 0xF0, 0xC4, 0xB1, 0x60,
  0x45, 0xA2, 0x30, 0xC2, 0xE4, 0x9D, 0xD0, 0x21, 0xB1, 0x60, 0x09, 0x80, 0xC5, 0x9E, 0xD0, 0x19,
  0xC8, 0xB1, 0x60, 0xC5, 0x9F, 0xD0, 0x12, 0xC8, 0xB1, 0x60, 0xC5, 0xA0, 0xD0, 0x0B, 0xC8, 0xA9,
  0x7F, 0xC5, 0xAC, 0xB1, 0x60, 0xE5, 0xA1, 0xF0, 0x28, 0xA5, 0xA2, 0x90, 0x02, 0x49, 0xFF, 0x4C,
  0x88, 0xEB, 0xA5, 0x9D, 0xF0, 0x4A, 0x38, 0xE9, 0xA0, 0x24, 0xA2, 0x10, 0x09, 0xAA, 0xA9, 0xFF,
  0x85, 0xA4, 0x20, 0xA4, 0xE8, 0x8A, 0xA2, 0x9D, 0xC9, 0xF9, 0x10, 0x06, 0x20, 0xF0, 0xE8, 0x84,
  0xA4, 0x60, 0xA8, 0xA5, 0xA2, 0x29, 0x80, 0x46, 0x9E, 0x05, 0x9E, 0x85, 0x9E, 0x20, 0x07, 0xE9,
  0x84, 0xA4, 0x60, 0xA5, 0x9D, 0xC9, 0xA0, 0xB0, 0x20, 0x20, 0xF2, 0xEB, 0x84, 0xAC, 0xA5, 0xA2,
  0x84, 0xA2, 0x49, 0x80, 0x2A, 0xA9, 0xA0, 0x85, 0x9D, 0xA5, 0xA1, 0x85, 0x0D, 0x4C, 0x29, 0xE8,
  0x85, 0x9E, 0x85, 0x9F, 0x85, 0xA0, 0x85, 0xA1, 0xA8, 0x60, 0xA0, 0x00, 0xA2, 0x0A, 0x94, 0x99,
  0xCA, 0x10, 0xFB, 0x90, 0x0F, 0xC9, 0x2D, 0xD0, 0x04, 0x86, 0xA3, 0xF0, 0x04, 0xC9, 0x2B, 0xD0,
  0x05, 0x20, 0xB1, 0x00, 0x90, 0x5B, 0xC9, 0x2E, 0xF0, 0x2E, 0xC9, 0x45, 0xD0, 0x30, 0x20, 0xB1,
  0x00, 0x90, 0x17, 0xC9, 0xC9, 0xF0, 0x0E, 0xC9, 0x2D, 0xF0, 0x0A, 0xC9, 0xC8, 0xF0, 0x08, 0xC9,
  0x2B, 0xF0, 0x04, 0xD0, 0x07, 0x66, 0x9C, 0x20, 0xB1, 0x00, 0x90, 0x5C, 0x24, 0x9C, 0x10, 0x0E,
  0xA9, 0x00, 0x38, 0xE5, 0x9A, 0x4C, 0xA0, 0xEC, 0x66, 0x9B, 0x24, 0x9B, 0x50, 0xC3, 0xA5, 0x9A,
  0x38, 0xE5, 0x99, 0x85, 0x9A, 0xF0, 0x12, 0x10, 0x09, 0x20, 0x55, 0xEA, 0xE6, 0x9A, 0xD0, 0xF9,
  0xF0, 0x07, 0x20, 0x39, 0xEA, 0xC6, 0x9A, 0xD0, 0xF9, 0xA5, 0xA3, 0x30, 0x01, 0x60, 0x4C, 0xD0,
  0xEE, 0x48, 0x24, 0x9B, 0x10, 0x02, 0xE6, 0x99, 0x20, 0x39, 0xEA, 0x68, 0x38, 0xE9, 0x30, 0x20,
  0xD5, 0xEC, 0x4C, 0x61, 0xEC, 0x48, 0x20, 0x63, 0xEB, 0x68, 0x20, 0x93, 0xEB, 0xA5, 0xAA, 0x45,
  0xA2, 0x85, 0xAB, 0xA6, 0x9D, 0x4C, 0xC1, 0xE7, 0xA5, 0x9A, 0xC9, 0x0A, 0x90, 0x09, 0xA9, 0x64,
  0x24, 0x9C, 0x30, 0x11, 0x4C, 0xD5, 0xE8, 0x0A, 0x0A, 0x18, 0x65, 0x9A, 0x0A, 0x18, 0xA0, 0x00,
  0x71, 0xB8, 0x38, 0xE9, 0x30, 0x85, 0x9A, 0x4C, 0x87, 0xEC, 0x9B, 0x3E, 0xBC, 0x1F, 0xFD, 0x9E,
  0x6E, 0x6B, 0x27, 0xFD, 0x9E, 0x6E, 0x6B, 0x28, 0x00, 0xA9, 0x58, 0xA0, 0xD3, 0x20, 0x31, 0xED,
  0xA5, 0x76, 0xA6, 0x75, 0x85, 0x9E, 0x86, 0x9F, 0xA2, 0x90, 0x38, 0x20, 0xA0, 0xEB, 0x20, 0x34,
  0xED, 0x4C, 0x3A, 0xDB, 0xA0, 0x01, 0xA9, 0x2D, 0x88, 0x24, 0xA2, 0x10, 0x04, 0xC8, 0x99, 0xFF,
  0x00, 0x85, 0xA2, 0x84, 0xAD, 0xC8, 0xA9, 0x30, 0xA6, 0x9D, 0xD0, 0x03, 0x4C, 0x57, 0xEE, 0xA9,
  0x00, 0xE0, 0x80, 0xF0, 0x02, 0xB0, 0x09, 0xA9, 0x14, 0xA0, 0xED, 0x20, 0x7F, 0xE9, 0xA9, 0xF7,
  0x85, 0x99, 0xA9, 0x0F, 0xA0, 0xED, 0x20, 0xB2, 0xEB, 0xF0, 0x1E, 0x10, 0x12, 0xA9, 0x0A, 0xA0,
  0xED, 0x20, 0xB2, 0xEB, 0xF0, 0x02, 0x10, 0x0E, 0x20, 0x39, 0xEA, 0xC6, 0x99, 0xD0, 0xEE, 0x20,
  0x55, 0xEA, 0xE6, 0x99, 0xD0, 0xDC, 0x20, 0xA0, 0xE7, 0x20, 0xF2, 0xEB, 0xA2, 0x01, 0xA5, 0x99,
  0x18, 0x69, 0x0A, 0x30, 0x09, 0xC9, 0x0B, 0xB0, 0x06, 0x69, 0xFF, 0xAA, 0xA9, 0x02, 0x38, 0xE9,
  0x02, 0x85, 0x9A, 0x86, 0x99, 0x8A, 0xF0, 0x02, 0x10, 0x13, 0xA4, 0xAD, 0xA9, 0x2E, 0xC8, 0x99,
  0xFF, 0x00, 0x8A, 0xF0, 0x06, 0xA9, 0x30, 0xC8, 0x99, 0xFF, 0x00, 0x84, 0xAD, 0xA0, 0x00, 0xA2,
  0x80, 0xA5, 0xA1, 0x18, 0x79, 0x6C, 0xEE, 0x85, 0xA1, 0xA5, 0xA0, 0x79, 0x6B, 0xEE, 0x85, 0xA0,
  0xA5, 0x9F, 0x79, 0x6A, 0xEE, 0x85, 0x9F, 0xA5, 0x9E, 0x79, 0x69, 0xEE, 0x85, 0x9E, 0xE8, 0xB0,
  0x04, 0x10, 0xDE, 0x30, 0x02, 0x30, 0xDA, 0x8A, 0x90, 0x04, 0x49, 0xFF, 0x69, 0x0A, 0x69, 0x2F,
  0xC8, 0xC8, 0xC8, 0xC8, 0x84, 0x83, 0xA4, 0xAD, 0xC8, 0xAA, 0x29, 0x7F, 0x99, 0xFF, 0x00, 0xC6,
  0x99, 0xD0, 0x06, 0xA9, 0x2E, 0xC8, 0x99, 0xFF, 0x00, 0x84, 0xAD, 0xA4, 0x83, 0x8A, 0x49, 0xFF,
  0x29, 0x80, 0xAA, 0xC0, 0x24, 0xD0, 0xAA, 0xA4, 0xAD, 0xB9, 0xFF, 0x00, 0x88, 0xC9, 0x30, 0xF0,
  0xF8, 0xC9, 0x2E, 0xF0, 0x01, 0xC8, 0xA9, 0x2B, 0xA6, 0x9A, 0xF0, 0x2E, 0x10, 0x08, 0xA9, 0x00,
  0x38, 0xE5, 0x9A, 0xAA, 0xA9, 0x2D, 0x99, 0x01, 0x01, 0xA9, 0x45, 0x99, 0x00, 0x01, 0x8A, 0xA2,
  0x2F, 0x38, 0xE8, 0xE9, 0x0A, 0xB0, 0xFB, 0x69, 0x3A, 0x99, 0x03, 0x01, 0x8A, 0x99, 0x02, 0x01,
  0xA9, 0x00, 0x99, 0x04, 0x01, 0xF0, 0x08, 0x99, 0xFF, 0x00, 0xA9, 0x00, 0x99, 0x00, 0x01, 0xA9,
  0x00, 0xA0, 0x01, 0x60, 0x80, 0x00, 0x00, 0x00, 0x00, 0xFA, 0x0A, 0x1F, 0x00, 0x00, 0x98, 0x96,
  0x80, 0xFF, 0xF0, 0xBD, 0xC0, 0x00, 0x01, 0x86, 0xA0, 0xFF, 0xFF, 0xD8, 0xF0, 0x00, 0x00, 0x03,
  0xE8, 0xFF, 0xFF, 0xFF, 0x9C, 0x00, 0x00, 0x00, 0x0A, 0xFF, 0xFF, 0xFF, 0xFF, 0x20, 0x63, 0xEB,
  0xA9, 0x64, 0xA0, 0xEE, 0x20, 0xF9, 0xEA, 0xF0, 0x70, 0xA5, 0xA5, 0xD0, 0x03, 0x4C, 0x50, 0xE8,
  0xA2, 0x8A, 0xA0, 0x00, 0x20, 0x2B, 0xEB, 0xA5, 0xAA, 0x10, 0x0F, 0x20, 0x23, 0xEC, 0xA9, 0x8A,
  0xA0, 0x00, 0x20, 0xB2, 0xEB, 0xD0, 0x03, 0x98, 0xA4, 0x0D, 0x20, 0x55, 0xEB, 0x98, 0x48, 0x20,
  0x41, 0xE9, 0xA9, 0x8A, 0xA0, 0x00, 0x20, 0x7F, 0xE9, 0x20, 0x09, 0xEF, 0x68, 0x4A, 0x90, 0x0A,
  0xA5, 0x9D, 0xF0, 0x06, 0xA5, 0xA2, 0x49, 0xFF, 0x85, 0xA2, 0x60, 0x81, 0x38, 0xAA, 0x3B, 0x29,
  0x07, 0x71, 0x34, 0x58, 0x3E, 0x56, 0x74, 0x16, 0x7E, 0xB3, 0x1B, 0x77, 0x2F, 0xEE, 0xE3, 0x85,
  0x7A, 0x1D, 0x84, 0x1C, 0x2A, 0x7C, 0x63, 0x59, 0x58, 0x0A, 0x7E, 0x75, 0xFD, 0xE7, 0xC6, 0x80,
  0x31, 0x72, 0x18, 0x10, 0x81, 0x00, 0x00, 0x00, 0x00, 0xA9, 0xDB, 0xA0, 0xEE, 0x20, 0x7F, 0xE9,
  0xA5, 0xAC, 0x69, 0x50, 0x90, 0x03, 0x20, 0x7A, 0xEB, 0x85, 0x92, 0x20, 0x66, 0xEB, 0xA5, 0x9D,
  0xC9, 0x88, 0x90, 0x03, 0x20, 0x2B, 0xEA, 0x20, 0x23, 0xEC, 0xA5, 0x0D, 0x18, 0x69, 0x81, 0xF0,
  0xF3, 0x38, 0xE9, 0x01, 0x48, 0xA2, 0x05, 0xB5, 0xA5, 0xB4, 0x9D, 0x95, 0x9D, 0x94, 0xA5, 0xCA,
  0x10, 0xF5, 0xA5, 0x92, 0x85, 0xAC, 0x20, 0xAA, 0xE7, 0x20, 0xD0, 0xEE, 0xA9, 0xE0, 0xA0, 0xEE,
  0x20, 0x72, 0xEF, 0xA9, 0x00, 0x85, 0xAB, 0x68, 0x20, 0x10, 0xEA, 0x60, 0x85, 0xAD, 0x84, 0xAE,
  0x20, 0x21, 0xEB, 0xA9, 0x93, 0x20, 0x7F, 0xE9, 0x20, 0x76, 0xEF, 0xA9, 0x93, 0xA0, 0x00, 0x4C,
  0x7F, 0xE9, 0x85, 0xAD, 0x84, 0xAE, 0x20, 0x1E, 0xEB, 0xB1, 0xAD, 0x85, 0xA3, 0xA4, 0xAD, 0xC8,
  0x98, 0xD0, 0x02, 0xE6, 0xAE, 0x85, 0xAD, 0xA4, 0xAE, 0x20, 0x7F, 0xE9, 0xA5, 0xAD, 0xA4, 0xAE,
  0x18, 0x69, 0x05, 0x90, 0x01, 0xC8, 0x85, 0xAD, 0x84, 0xAE, 0x20, 0xBE, 0xE7, 0xA9, 0x98, 0xA0,
  0x00, 0xC6, 0xA3, 0xD0, 0xE4, 0x60, 0x98, 0x35, 0x44, 0x7A, 0x68, 0x28, 0xB1, 0x46, 0x20, 0x82,
  0xEB, 0xAA, 0x30, 0x18, 0xA9, 0xC9, 0xA0, 0x00, 0x20, 0xF9, 0xEA, 0x8A, 0xF0, 0xE7, 0xA9, 0xA6,
  0xA0, 0xEF, 0x20, 0x7F, 0xE9, 0xA9, 0xAA, 0xA0, 0xEF, 0x20, 0xBE, 0xE7, 0xA6, 0xA1, 0xA5, 0x9E,
  0x85, 0xA1, 0x86, 0x9E, 0xA9, 0x00, 0x85, 0xA2, 0xA5, 0x9D, 0x85, 0xAC, 0xA9, 0x80, 0x85, 0x9D,
  0x20, 0x2E, 0xE8, 0xA2, 0xC9, 0xA0, 0x00, 0x4C, 0x2B, 0xEB, 0xA9, 0x66, 0xA0, 0xF0, 0x20, 0xBE,
  0xE7, 0x20, 0x63, 0xEB, 0xA9, 0x6B, 0xA0, 0xF0, 0xA6, 0xAA, 0x20, 0x5E, 0xEA, 0x20, 0x63, 0xEB,
  0x20, 0x23, 0xEC, 0xA9, 0x00, 0x85, 0xAB, 0x20, 0xAA, 0xE7, 0xA9, 0x70, 0xA0, 0xF0, 0x20, 0xA7,
  0xE7, 0xA5, 0xA2, 0x48, 0x10, 0x0D, 0x20, 0xA0, 0xE7, 0xA5, 0xA2, 0x30, 0x09, 0xA5, 0x16, 0x49,
  0xFF, 0x85, 0x16, 0x20, 0xD0, 0xEE, 0xA9, 0x70, 0xA0, 0xF0, 0x20, 0xBE, 0xE7, 0x68, 0x10, 0x03,
  0x20, 0xD0, 0xEE, 0xA9, 0x75, 0xA0, 0xF0, 0x4C, 0x5C, 0xEF, 0x20, 0x21, 0xEB, 0xA9, 0x00, 0x85,
  0x16, 0x20, 0xF1, 0xEF, 0xA2, 0x8A, 0xA0, 0x00, 0x20, 0xE7, 0xEF, 0xA9, 0x93, 0xA0, 0x00, 0x20,
  0xF9, 0xEA, 0xA9, 0x00, 0x85, 0xA2, 0xA5, 0x16, 0x20, 0x62, 0xF0, 0xA9, 0x8A, 0xA0, 0x00, 0x4C,
  0x66, 0xEA, 0x48, 0x4C, 0x23, 0xF0, 0x81, 0x49, 0x0F, 0xDA, 0xA2, 0x83, 0x49, 0x0F, 0xDA, 0xA2,
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x05, 0x84, 0xE6, 0x1A, 0x2D, 0x1B, 0x86, 0x28, 0x07, 0xFB, 0xF8,
  0x87, 0x99, 0x68, 0x89, 0x01, 0x87, 0x23, 0x35, 0xDF, 0xE1, 0x86, 0xA5, 0x5D, 0xE7, 0x28, 0x83,
  0x49, 0x0F, 0xDA, 0xA2, 0xA6, 0xD3, 0xC1, 0xC8, 0xD4, 0xC8, 0xD5, 0xC4, 0xCE, 0xCA, 0xA5, 0xA2,
  0x48, 0x10, 0x03, 0x20, 0xD0, 0xEE, 0xA5, 0x9D, 0x48, 0xC9, 0x81, 0x90, 0x07, 0xA9, 0x13, 0xA0,
  0xE9, 0x20, 0x66, 0xEA, 0xA9, 0xCE, 0xA0, 0xF0, 0x20, 0x5C, 0xEF, 0x68, 0xC9, 0x81, 0x90, 0x07,
  0xA9, 0x66, 0xA0, 0xF0, 0x20, 0xA7, 0xE7, 0x68, 0x10, 0x03, 0x4C, 0xD0, 0xEE, 0x60, 0x0B, 0x76,
  0xB3, 0x83, 0xBD, 0xD3, 0x79, 0x1E, 0xF4, 0xA6, 0xF5, 0x7B, 0x83, 0xFC, 0xB0, 0x10, 0x7C, 0x0C,
  0x1F, 0x67, 0xCA, 0x7C, 0xDE, 0x53, 0xCB, 0xC1, 0x7D, 0x14, 0x64, 0x70, 0x4C, 0x7D, 0xB7, 0xEA,
  0x51, 0x7A, 0x7D, 0x63, 0x30, 0x88, 0x7E, 0x7E, 0x92, 0x44, 0x99, 0x3A, 0x7E, 0x4C, 0xCC, 0x91,
  0xC7, 0x7F, 0xAA, 0xAA, 0xAA, 0x13, 0x81, 0x00, 0x00, 0x00, 0x00, 0xE6, 0xB8, 0xD0, 0x02, 0xE6,
  0xB9, 0xAD, 0x60, 0xEA, 0xC9, 0x3A, 0xB0, 0x0A, 0xC9, 0x20, 0xF0, 0xEF, 0x38, 0xE9, 0x30, 0x38,
  0xE9, 0xD0, 0x60, 0x80, 0x4F, 0xC7, 0x52, 0x58, 0xA2, 0xFF, 0x86, 0x76, 0xA2, 0xFB, 0x9A, 0xA9,
  0x28, 0xA0, 0xF1, 0x85, 0x01, 0x84, 0x02, 0x85, 0x04, 0x84, 0x05, 0x20, 0x73, 0xF2, 0xA9, 0x4C,
  0x85, 0x00, 0x85, 0x03, 0x85, 0x90, 0x85, 0x0A, 0xA9, 0x99, 0xA0, 0xE1, 0x85, 0x0B, 0x84, 0x0C,
  0xA2, 0x1C, 0xBD, 0x0A, 0xF1, 0x95, 0xB0, 0x86, 0xF1, 0xCA, 0xD0, 0xF6, 0x86, 0xF2, 0x8A, 0x85,
  0xA4, 0x85, 0x54, 0x48, 0xA9, 0x03, 0x85, 0x8F, 0x20, 0xFB, 0xDA, 0xA9, 0x01, 0x8D, 0xFD, 0x01,
  0x8D, 0xFC, 0x01, 0xA2, 0x55, 0x86, 0x52, 0xA9, 0x00, 0xA0, 0x08, 0x85, 0x50, 0x84, 0x51, 0xA0,
  0x00, 0xE6, 0x51, 0xB1, 0x50, 0x49, 0xFF, 0x91, 0x50, 0xD1, 0x50, 0xD0, 0x08, 0x49, 0xFF, 0x91,
  0x50, 0xD1, 0x50, 0xF0, 0xEC, 0xA4, 0x50, 0xA5, 0x51, 0x29, 0xF0, 0x84, 0x73, 0x85, 0x74, 0x84,
  0x6F, 0x85, 0x70, 0xA2, 0x00, 0xA0, 0x08, 0x86, 0x67, 0x84, 0x68, 0xA0, 0x00, 0x84, 0xD6, 0x98,
  0x91, 0x67, 0xE6, 0x67, 0xD0, 0x02, 0xE6, 0x68, 0xA5, 0x67, 0xA4, 0x68, 0x20, 0xE3, 0xD3, 0x20,
  0x4B, 0xD6, 0xA9, 0x3A, 0xA0, 0xDB, 0x85, 0x04, 0x84, 0x05, 0xA9, 0x3C, 0xA0, 0xD4, 0x85, 0x01,
  0x84, 0x02, 0x6C, 0x01, 0x00, 0x20, 0x67, 0xDD, 0x20, 0x52, 0xE7, 0x6C, 0x50, 0x00, 0x20, 0xF8,
  0xE6, 0x8A, 0x4C, 0x8B, 0xFE, 0x20, 0xF8, 0xE6, 0x8A, 0x4C, 0x95, 0xFE, 0x20, 0xF8, 0xE6, 0xE0,
  0x30, 0xB0, 0x13, 0x86, 0xF0, 0xA9, 0x2C, 0x20, 0xC0, 0xDE, 0x20, 0xF8, 0xE6, 0xE0, 0x30, 0xB0,
  0x05, 0x86, 0x2C, 0x86, 0x2D, 0x60, 0x4C, 0x99, 0xE1, 0x20, 0xEC, 0xF1, 0xE4, 0xF0, 0xB0, 0x08,
  0xA5, 0xF0, 0x85, 0x2C, 0x85, 0x2D, 0x86, 0xF0, 0xA9, 0xC5, 0x20, 0xC0, 0xDE, 0x20, 0xF8, 0xE6,
  0xE0, 0x30, 0xB0, 0xE2, 0x60, 0x20, 0xEC, 0xF1, 0x8A, 0xA4, 0xF0, 0xC0, 0x28, 0xB0, 0xD7, 0x4C,
  0x00, 0xF8, 0x20, 0x09, 0xF2, 0x8A, 0xA4, 0x2C, 0xC0, 0x28, 0xB0, 0xCA, 0xA4, 0xF0, 0x4C, 0x19,
  0xF8, 0x20, 0x09, 0xF2, 0x8A, 0xA8, 0xC0, 0x28, 0xB0, 0xBC, 0xA5, 0xF0, 0x4C, 0x28, 0xF8, 0x20,
  0xF8, 0xE6, 0x8A, 0x4C, 0x64, 0xF8, 0x20, 0xF8, 0xE6, 0xCA, 0x8A, 0xC9, 0x18, 0xB0, 0xA7, 0x4C,
  0x5B, 0xFB, 0x20, 0xF8, 0xE6, 0x8A, 0x49, 0xFF, 0xAA, 0xE8, 0x86, 0xF1, 0x60, 0x38, 0x90, 0x18,
  0x66, 0xF2, 0x60, 0xA9, 0xFF, 0xD0, 0x02, 0xA9, 0x3F, 0xA2, 0x00, 0x85, 0x32, 0x86, 0xF3, 0x60,
  0xA9, 0x7F, 0xA2, 0x40, 0xD0, 0xF5, 0x20, 0x67, 0xDD, 0x20, 0x52, 0xE7, 0xA5, 0x50, 0xC5, 0x6D,
  0xA5, 0x51, 0xE5, 0x6E, 0xB0, 0x03, 0x4C, 0x10, 0xD4, 0xA5, 0x50, 0x85, 0x73, 0x85, 0x6F, 0xA5,
  0x51, 0x85, 0x74, 0x85, 0x70, 0x60, 0x20, 0x67, 0xDD, 0x20, 0x52, 0xE7, 0xA5, 0x50, 0xC5, 0x73,
  0xA5, 0x51, 0xE5, 0x74, 0xB0, 0xE0, 0xA5, 0x50, 0xC5, 0x69, 0xA5, 0x51, 0xE5, 0x6A, 0x90, 0xD6,
  0xA5, 0x50, 0x85, 0x69, 0xA5, 0x51, 0x85, 0x6A, 0x4C, 0x6C, 0xD6, 0xA9, 0xAB, 0x20, 0xC0, 0xDE,
  0xA5, 0xB8, 0x85, 0xF4, 0xA5, 0xB9, 0x85, 0xF5, 0x38, 0x66, 0xD8, 0xA5, 0x75, 0x85, 0xF6, 0xA5,
  0x76, 0x85, 0xF7, 0x20, 0xA6, 0xD9, 0x4C, 0x98, 0xD9, 0x86, 0xDE, 0xA6, 0xF8, 0x86, 0xDF, 0xA5,
  0x75, 0x85, 0xDA, 0xA5, 0x76, 0x85, 0xDB, 0xA5, 0x79, 0x85, 0xDC, 0xA5, 0x7A, 0x85, 0xDD, 0xA5,
  0xF4, 0x85, 0xB8, 0xA5, 0xF5, 0x85, 0xB9, 0xA5, 0xF6, 0x85, 0x75, 0xA5, 0xF7, 0x85, 0x76, 0x20,
  0xB7, 0x00, 0x20, 0x3E, 0xD9, 0x4C, 0xD2, 0xD7, 0xA5, 0xDA, 0x85, 0x75, 0xA5, 0xDB, 0x85, 0x76,
  0xA5, 0xDC, 0x85, 0xB8, 0xA5, 0xDD, 0x85, 0xB9, 0xA6, 0xDF, 0x9A, 0x4C, 0xD2, 0xD7, 0x4C, 0xC9,
  0xDE, 0xB0, 0xFB, 0xA6, 0xAF, 0x86, 0x69, 0xA6, 0xB0, 0x86, 0x6A, 0x20, 0x0C, 0xDA, 0x20, 0x1A,
  0xD6, 0xA5, 0x9B, 0x85, 0x60, 0xA5, 0x9C, 0x85, 0x61, 0xA9, 0x2C, 0x20, 0xC0, 0xDE, 0x20, 0x0C,
  0xDA, 0xE6, 0x50, 0xD0, 0x02, 0xE6, 0x51, 0x20, 0x1A, 0xD6, 0xA5, 0x9B, 0xC5, 0x60, 0xA5, 0x9C,
  0xE5, 0x61, 0xB0, 0x01, 0x60, 0xA0, 0x00, 0xB1, 0x9B, 0x91, 0x60, 0xE6, 0x9B, 0xD0, 0x02, 0xE6,
  0x9C, 0xE6, 0x60, 0xD0, 0x02, 0xE6, 0x61, 0xA5, 0x69, 0xC5, 0x9B, 0xA5, 0x6A, 0xE5, 0x9C, 0xB0,
  0xE6, 0xA6, 0x61, 0xA4, 0x60, 0xD0, 0x01, 0xCA, 0x88, 0x86, 0x6A, 0x84, 0x69, 0x4C, 0xF2, 0xD4,
  0xAD, 0x56, 0xC0, 0xAD, 0x53, 0xC0, 0x4C, 0x40, 0xFB, 0xAD, 0x54, 0xC0, 0x4C, 0x39, 0xFB, 0x20,
  0xD9, 0xF7, 0xA0, 0x03, 0xB1, 0x9B, 0xAA, 0x88, 0xB1, 0x9B, 0xE9, 0x01, 0xB0, 0x01, 0xCA, 0x85,
  0x50, 0x86, 0x51, 0x20, 0xCD, 0xFE, 0x20, 0xBC, 0xF7, 0x4C, 0xCD, 0xFE, 0x20, 0xD9, 0xF7, 0x20,
  0xFD, 0xFE, 0xA0, 0x02, 0xB1, 0x9B, 0xC5, 0x50, 0xC8, 0xB1, 0x9B, 0xE5, 0x51, 0xB0, 0x03, 0x4C,
  0x10, 0xD4, 0x20, 0xBC, 0xF7, 0x4C, 0xFD, 0xFE, 0x2C, 0x55, 0xC0, 0x2C, 0x52, 0xC0, 0xA9, 0x40,
  0xD0, 0x08, 0xA9, 0x20, 0x2C, 0x54, 0xC0, 0x2C, 0x53, 0xC0, 0x85, 0xE6, 0xAD, 0x57, 0xC0, 0xAD,
  0x50, 0xC0, 0xA9, 0x00, 0x85, 0x1C, 0xA5, 0xE6, 0x85, 0x1B, 0xA0, 0x00, 0x84, 0x1A, 0xA5, 0x1C,
  0x91, 0x1A, 0x20, 0x7E, 0xF4, 0xC8, 0xD0, 0xF6, 0xE6, 0x1B, 0xA5, 0x1B, 0x29, 0x1F, 0xD0, 0xEE,
  0x60, 0x85, 0xE2, 0x86, 0xE0, 0x84, 0xE1, 0x48, 0x29, 0xC0, 0x85, 0x26, 0x4A, 0x4A, 0x05, 0x26,
  0x85, 0x26, 0x68, 0x85, 0x27, 0x0A, 0x0A, 0x0A, 0x26, 0x27, 0x0A, 0x26, 0x27, 0x0A, 0x66, 0x26,
  0xA5, 0x27, 0x29, 0x1F, 0x05, 0xE6, 0x85, 0x27, 0x8A, 0xC0, 0x00, 0xF0, 0x05, 0xA0, 0x23, 0x69,
  0x04, 0xC8, 0xE9, 0x07, 0xB0, 0xFB, 0x84, 0xE5, 0xAA, 0xBD, 0xB9, 0xF4, 0x85, 0x30, 0x98, 0x4A,
  0xA5, 0xE4, 0x85, 0x1C, 0xB0, 0x28, 0x60, 0x20, 0x11, 0xF4, 0xA5, 0x1C, 0x51, 0x26, 0x25, 0x30,
  0x51, 0x26, 0x91, 0x26, 0x60, 0x10, 0x23, 0xA5, 0x30, 0x4A, 0xB0, 0x05, 0x49, 0xC0, 0x85, 0x30,
  0x60, 0x88, 0x10, 0x02, 0xA0, 0x27, 0xA9, 0xC0, 0x85, 0x30, 0x84, 0xE5, 0xA5, 0x1C, 0x0A, 0xC9,
  0xC0, 0x10, 0x06, 0xA5, 0x1C, 0x49, 0x7F, 0x85, 0x1C, 0x60, 0xA5, 0x30, 0x0A, 0x49, 0x80, 0x30,
  0xDD, 0xA9, 0x81, 0xC8, 0xC0, 0x28, 0x90, 0xE0, 0xA0, 0x00, 0xB0, 0xDC, 0x18, 0xA5, 0xD1, 0x29,
  0x04, 0xF0, 0x25, 0xA9, 0x7F, 0x25, 0x30, 0x31, 0x26, 0xD0, 0x19, 0xE6, 0xEA, 0xA9, 0x7F, 0x25,
  0x30, 0x10, 0x11, 0x18, 0xA5, 0xD1, 0x29, 0x04, 0xF0, 0x0E, 0xB1, 0x26, 0x45, 0x1C, 0x25, 0x30,
  0xD0, 0x02, 0xE6, 0xEA, 0x51, 0x26, 0x91, 0x26, 0xA5, 0xD1, 0x65, 0xD3, 0x29, 0x03, 0xC9, 0x02,
  0x6A, 0xB0, 0x92, 0x30, 0x30, 0x18, 0xA5, 0x27, 0x2C, 0xB9, 0xF5, 0xD0, 0x22, 0x06, 0x26, 0xB0,
  0x1A, 0x2C, 0xCD, 0xF4, 0xF0, 0x05, 0x69, 0x1F, 0x38, 0xB0, 0x12, 0x69, 0x23, 0x48, 0xA5, 0x26,
  0x69, 0xB0, 0xB0, 0x02, 0x69, 0xF0, 0x85, 0x26, 0x68, 0xB0, 0x02, 0x69, 0x1F, 0x66, 0x26, 0x69,
  0xFC, 0x85, 0x27, 0x60, 0x18, 0xA5, 0x27, 0x69, 0x04, 0x2C, 0xB9, 0xF5, 0xD0, 0xF3, 0x06, 0x26,
  0x90, 0x18, 0x69, 0xE0, 0x18, 0x2C, 0x08, 0xF5, 0xF0, 0x12, 0xA5, 0x26, 0x69, 0x50, 0x49, 0xF0,
  0xF0, 0x02, 0x49, 0xF0, 0x85, 0x26, 0xA5, 0xE6, 0x90, 0x02, 0x69, 0xE0, 0x66, 0x26, 0x90, 0xD1,
  0x48, 0xA9, 0x00, 0x85, 0xE0, 0x85, 0xE1, 0x85, 0xE2, 0x68, 0x48, 0x38, 0xE5, 0xE0, 0x48, 0x8A,
  0xE5, 0xE1, 0x85, 0xD3, 0xB0, 0x0A, 0x68, 0x49, 0xFF, 0x69, 0x01, 0x48, 0xA9, 0x00, 0xE5, 0xD3,
  0x85, 0xD1, 0x85, 0xD5, 0x68, 0x85, 0xD0, 0x85, 0xD4, 0x68, 0x85, 0xE0, 0x86, 0xE1, 0x98, 0x18,
  0xE5, 0xE2, 0x90, 0x04, 0x49, 0xFF, 0x69, 0xFE, 0x85, 0xD2, 0x84, 0xE2, 0x66, 0xD3, 0x38, 0xE5,
  0xD0, 0xAA, 0xA9, 0xFF, 0xE5, 0xD1, 0x85, 0x1D, 0xA4, 0xE5, 0xB0, 0x05, 0x0A, 0x20, 0x65, 0xF4,
  0x38, 0xA5, 0xD4, 0x65, 0xD2, 0x85, 0xD4, 0xA5, 0xD5, 0xE9, 0x00, 0x85, 0xD5, 0xB1, 0x26, 0x45,
  0x1C, 0x25, 0x30, 0x51, 0x26, 0x91, 0x26, 0xE8, 0xD0, 0x04, 0xE6, 0x1D, 0xF0, 0x62, 0xA5, 0xD3,
  0xB0, 0xDA, 0x20, 0xD3, 0xF4, 0x18, 0xA5, 0xD4, 0x65, 0xD0, 0x85, 0xD4, 0xA5, 0xD5, 0x65, 0xD1,
  0x50, 0xD9, 0x81, 0x82, 0x84, 0x88, 0x90, 0xA0, 0xC0, 0x1C, 0xFF, 0xFE, 0xFA, 0xF4, 0xEC, 0xE1,
  0xD4, 0xC5, 0xB4, 0xA1, 0x8D, 0x78, 0x61, 0x49, 0x31, 0x18, 0xFF, 0xA5, 0x26, 0x0A, 0xA5, 0x27,
  0x29, 0x03, 0x2A, 0x05, 0x26, 0x0A, 0x0A, 0x0A, 0x85, 0xE2, 0xA5, 0x27, 0x4A, 0x4A, 0x29, 0x07,
  0x05, 0xE2, 0x85, 0xE2, 0xA5, 0xE5, 0x0A, 0x65, 0xE5, 0x0A, 0xAA, 0xCA, 0xA5, 0x30, 0x29, 0x7F,
  0xE8, 0x4A, 0xD0, 0xFC, 0x85, 0xE1, 0x8A, 0x18, 0x65, 0xE5, 0x90, 0x02, 0xE6, 0xE1, 0x85, 0xE0,
  0x60, 0x86, 0x1A, 0x84, 0x1B, 0xAA, 0x4A, 0x4A, 0x4A, 0x4A, 0x85, 0xD3, 0x8A, 0x29, 0x0F, 0xAA,
  0xBC, 0xBA, 0xF5, 0x84, 0xD0, 0x49, 0x0F, 0xAA, 0xBC, 0xBB, 0xF5, 0xC8, 0x84, 0xD2, 0xA4, 0xE5,
  0xA2, 0x00, 0x86, 0xEA, 0xA1, 0x1A, 0x85, 0xD1, 0xA2, 0x80, 0x86, 0xD4, 0x86, 0xD5, 0xA6, 0xE7,
  0xA5, 0xD4, 0x38, 0x65, 0xD0, 0x85, 0xD4, 0x90, 0x04, 0x20, 0xB3, 0xF4, 0x18, 0xA5, 0xD5, 0x65,
  0xD2, 0x85, 0xD5, 0x90, 0x03, 0x20, 0xB4, 0xF4, 0xCA, 0xD0, 0xE5, 0xA5, 0xD1, 0x4A, 0x4A, 0x4A,
  0xD0, 0xD4, 0xE6, 0x1A, 0xD0, 0x02, 0xE6, 0x1B, 0xA1, 0x1A, 0xD0, 0xCA, 0x60, 0x86, 0x1A, 0x84,
  0x1B, 0xAA, 0x4A, 0x4A, 0x4A, 0x4A, 0x85, 0xD3, 0x8A, 0x29, 0x0F, 0xAA, 0xBC, 0xBA, 0xF5, 0x84,
  0xD0, 0x49, 0x0F, 0xAA, 0xBC, 0xBB, 0xF5, 0xC8, 0x84, 0xD2, 0xA4, 0xE5, 0xA2, 0x00, 0x86, 0xEA,
  0xA1, 0x1A, 0x85, 0xD1, 0xA2, 0x80, 0x86, 0xD4, 0x86, 0xD5, 0xA6, 0xE7, 0xA5, 0xD4, 0x38, 0x65,
  0xD0, 0x85, 0xD4, 0x90, 0x04, 0x20, 0x9C, 0xF4, 0x18, 0xA5, 0xD5, 0x65, 0xD2, 0x85, 0xD5, 0x90,
  0x03, 0x20, 0x9D, 0xF4, 0xCA, 0xD0, 0xE5, 0xA5, 0xD1, 0x4A, 0x4A, 0x4A, 0xD0, 0xD4, 0xE6, 0x1A,
  0xD0, 0x02, 0xE6, 0x1B, 0xA1, 0x1A, 0xD0, 0xCA, 0x60, 0x20, 0x67, 0xDD, 0x20, 0x52, 0xE7, 0xA4,
  0x51, 0xA6, 0x50, 0xC0, 0x01, 0x90, 0x06, 0xD0, 0x1D, 0xE0, 0x18, 0xB0, 0x19, 0x8A, 0x48, 0x98,
  0x48, 0xA9, 0x2C, 0x20, 0xC0, 0xDE, 0x20, 0xF8, 0xE6, 0xE0, 0xC0, 0xB0, 0x09, 0x86, 0x9D, 0x68,
  0xA8, 0x68, 0xAA, 0xA5, 0x9D, 0x60, 0x4C, 0x06, 0xF2, 0x20, 0xF8, 0xE6, 0xE0, 0x08, 0xB0, 0xF6,
  0xBD, 0xF6, 0xF6, 0x85, 0xE4, 0x60, 0x00, 0x2A, 0x55, 0x7F, 0x80, 0xAA, 0xD5, 0xFF, 0xC9, 0xC1,
  0xF0, 0x0D, 0x20, 0xB9, 0xF6, 0x20, 0x57, 0xF4, 0x20, 0xB7, 0x00, 0xC9, 0xC1, 0xD0, 0xE6, 0x20,
  0xC0, 0xDE, 0x20, 0xB9, 0xF6, 0x84, 0x9D, 0xA8, 0x8A, 0xA6, 0x9D, 0x20, 0x3A, 0xF5, 0x4C, 0x08,
  0xF7, 0x20, 0xF8, 0xE6, 0x86, 0xF9, 0x60, 0x20, 0xF8, 0xE6, 0x86, 0xE7, 0x60, 0x20, 0xF8, 0xE6,
  0xA5, 0xE8, 0x85, 0x1A, 0xA5, 0xE9, 0x85, 0x1B, 0x8A, 0xA2, 0x00, 0xC1, 0x1A, 0xF0, 0x02, 0xB0,
  0xA5, 0x0A, 0x90, 0x03, 0xE6, 0x1B, 0x18, 0xA8, 0xB1, 0x1A, 0x65, 0x1A, 0xAA, 0xC8, 0xB1, 0x1A,
  0x65, 0xE9, 0x85, 0x1B, 0x86, 0x1A, 0x20, 0xB7, 0x00, 0xC9, 0xC5, 0xD0, 0x09, 0x20, 0xC0, 0xDE,
  0x20, 0xB9, 0xF6, 0x20, 0x11, 0xF4, 0xA5, 0xF9, 0x60, 0x20, 0x2D, 0xF7, 0x4C, 0x05, 0xF6, 0x20,
  0x2D, 0xF7, 0x4C, 0x61, 0xF6, 0xA9, 0x00, 0x85, 0x3D, 0x85, 0x3F, 0xA0, 0x50, 0x84, 0x3C, 0xC8,
  0x84, 0x3E, 0x20, 0xFD, 0xFE, 0x18, 0xA5, 0x73, 0xAA, 0xCA, 0x86, 0x3E, 0xE5, 0x50, 0x48, 0xA5,
  0x74, 0xA8, 0xE8, 0xD0, 0x01, 0x88, 0x84, 0x3F, 0xE5, 0x51, 0xC5, 0x6E, 0x90, 0x02, 0xD0, 0x03,
  0x4C, 0x10, 0xD4, 0x85, 0x74, 0x85, 0x70, 0x85, 0x3D, 0x85, 0xE9, 0x68, 0x85, 0xE8, 0x85, 0x73,
  0x85, 0x6F, 0x85, 0x3C, 0x20, 0xFA, 0xFC, 0xA9, 0x03, 0x4C, 0x02, 0xFF, 0x18, 0xA5, 0x9B, 0x65,
  0x50, 0x85, 0x3E, 0xA5, 0x9C, 0x65, 0x51, 0x85, 0x3F, 0xA0, 0x04, 0xB1, 0x9B, 0x20, 0xEF, 0xE0,
  0xA5, 0x94, 0x85, 0x3C, 0xA5, 0x95, 0x85, 0x3D, 0x60, 0xA9, 0x40, 0x85, 0x14, 0x20, 0xE3, 0xDF,
  0xA9, 0x00, 0x85, 0x14, 0x4C, 0xF0, 0xD8, 0x20, 0xF8, 0xE6, 0xCA, 0x8A, 0xC9, 0x28, 0x90, 0x0A,
  0xE9, 0x28, 0x48, 0x20, 0xFB, 0xDA, 0x68, 0x4C, 0xEC, 0xF7, 0x85, 0x24, 0x60, 0xCB, 0xD2, 0xD7,
  0x4A, 0x08, 0x20, 0x47, 0xF8, 0x28, 0xA9, 0x0F, 0x90, 0x02, 0x69, 0xE0, 0x85, 0x2E, 0xB1, 0x26,
  0x45, 0x30, 0x25, 0x2E, 0x51, 0x26, 0x91, 0x26, 0x60, 0x20, 0x00, 0xF8, 0xC4, 0x2C, 0xB0, 0x11,
  0xC8, 0x20, 0x0E, 0xF8, 0x90, 0xF6, 0x69, 0x01, 0x48, 0x20, 0x00, 0xF8, 0x68, 0xC5, 0x2D, 0x90,
  0xF5, 0x60, 0xA0, 0x2F, 0xD0, 0x02, 0xA0, 0x27, 0x84, 0x2D, 0xA0, 0x27, 0xA9, 0x00, 0x85, 0x30,
  0x20, 0x28, 0xF8, 0x88, 0x10, 0xF6, 0x60, 0x48, 0x4A, 0x29, 0x03, 0x09, 0x04, 0x85, 0x27, 0x68,
  0x29, 0x18, 0x90, 0x02, 0x69, 0x7F, 0x85, 0x26, 0x0A, 0x0A, 0x05, 0x26, 0x85, 0x26, 0x60, 0xA5,
  0x30, 0x18, 0x69, 0x03, 0x29, 0x0F, 0x85, 0x30, 0x0A, 0x0A, 0x0A, 0x0A, 0x05, 0x30, 0x85, 0x30,
  0x60, 0x4A, 0x08, 0x20, 0x47, 0xF8, 0xB1, 0x26, 0x28, 0x90, 0x04, 0x4A, 0x4A, 0x4A, 0x4A, 0x29,
  0x0F, 0x60, 0xA6, 0x3A, 0xA4, 0x3B, 0x20, 0x96, 0xFD, 0x20, 0x48, 0xF9, 0xA1, 0x3A, 0xA8, 0x4A,
  0x90, 0x09, 0x6A, 0xB0, 0x10, 0xC9, 0xA2, 0xF0, 0x0C, 0x29, 0x87, 0x4A, 0xAA, 0xBD, 0x62, 0xF9,
  0x20, 0x79, 0xF8, 0xD0, 0x04, 0xA0, 0x80, 0xA9, 0x00, 0xAA, 0xBD, 0xA6, 0xF9, 0x85, 0x2E, 0x29,
  0x03, 0x85, 0x2F, 0x98, 0x29, 0x8F, 0xAA, 0x98, 0xA0, 0x03, 0xE0, 0x8A, 0xF0, 0x0B, 0x4A, 0x90,
  0x08, 0x4A, 0x4A, 0x09, 0x20, 0x88, 0xD0, 0xFA, 0xC8, 0x88, 0xD0, 0xF2, 0x60, 0xFF, 0xFF, 0xFF,
  0x20, 0x82, 0xF8, 0x48, 0xB1, 0x3A, 0x20, 0xDA, 0xFD, 0xA2, 0x01, 0x20, 0x4A, 0xF9, 0xC4, 0x2F,
  0xC8, 0x90, 0xF1, 0xA2, 0x03, 0xC0, 0x04, 0x90, 0xF2, 0x68, 0xA8, 0xB9, 0xC0, 0xF9, 0x85, 0x2C,
  0xB9, 0x00, 0xFA, 0x85, 0x2D, 0xA9, 0x00, 0xA0, 0x05, 0x06, 0x2D, 0x26, 0x2C, 0x2A, 0x88, 0xD0,
  0xF8, 0x69, 0xBF, 0x20, 0xED, 0xFD, 0xCA, 0xD0, 0xEC, 0x20, 0x48, 0xF9, 0xA4, 0x2F, 0xA2, 0x06,
  0xE0, 0x03, 0xF0, 0x1C, 0x06, 0x2E, 0x90, 0x0E, 0xBD, 0xB3, 0xF9, 0x20, 0xED, 0xFD, 0xBD, 0xB9,
  0xF9, 0xF0, 0x03, 0x20, 0xED, 0xFD, 0xCA, 0xD0, 0xE7, 0x60, 0x88, 0x30, 0xE7, 0x20, 0xDA, 0xFD,
  0xA5, 0x2E, 0xC9, 0xE8, 0xB1, 0x3A, 0x90, 0xF2, 0x20, 0x56, 0xF9, 0xAA, 0xE8, 0xD0, 0x01, 0xC8,
  0x98, 0x20, 0xDA, 0xFD, 0x8A, 0x4C, 0xDA, 0xFD, 0xA2, 0x03, 0xA9, 0xA0, 0x20, 0xED, 0xFD, 0xCA,
  0xD0, 0xF8, 0x60, 0x38, 0xA5, 0x2F, 0xA4, 0x3B, 0xAA, 0x10, 0x01, 0x88, 0x65, 0x3A, 0x90, 0x01,
  0xC8, 0x60, 0x04, 0x20, 0x54, 0x30, 0x0D, 0x80, 0x04, 0x90, 0x03, 0x22, 0x54, 0x33, 0x0D, 0x80,
  0x04, 0x90, 0x04, 0x20, 0x54, 0x33, 0x0D, 0x80, 0x04, 0x90, 0x04, 0x20, 0x54, 0x3B, 0x0D, 0x80,
  0x04, 0x90, 0x00, 0x22, 0x44, 0x33, 0x0D, 0xC8, 0x44, 0x00, 0x11, 0x22, 0x44, 0x33, 0x0D, 0xC8,
  0x44, 0xA9, 0x01, 0x22, 0x44, 0x33, 0x0D, 0x80, 0x04, 0x90, 0x01, 0x22, 0x44, 0x33, 0x0D, 0x80,
  0x04, 0x90, 0x26, 0x31, 0x87, 0x9A, 0x00, 0x21, 0x81, 0x82, 0x00, 0x00, 0x59, 0x4D, 0x91, 0x92,
  0x86, 0x4A, 0x85, 0x9D, 0xAC, 0xA9, 0xAC, 0xA3, 0xA8, 0xA4, 0xD9, 0x00, 0xD8, 0xA4, 0xA4, 0x00,
  0x1C, 0x8A, 0x1C, 0x23, 0x5D, 0x8B, 0x1B, 0xA1, 0x9D, 0x8A, 0x1D, 0x23, 0x9D, 0x8B, 0x1D, 0xA1,
  0x00, 0x29, 0x19, 0xAE, 0x69, 0xA8, 0x19, 0x23, 0x24, 0x53, 0x1B, 0x23, 0x24, 0x53, 0x19, 0xA1,
  0x00, 0x1A, 0x5B, 0x5B, 0xA5, 0x69, 0x24, 0x24, 0xAE, 0xAE, 0xA8, 0xAD, 0x29, 0x00, 0x7C, 0x00,
  0x15, 0x9C, 0x6D, 0x9C, 0xA5, 0x69, 0x29, 0x53, 0x84, 0x13, 0x34, 0x11, 0xA5, 0x69, 0x23, 0xA0,
  0xD8, 0x62, 0x5A, 0x48, 0x26, 0x62, 0x94, 0x88, 0x54, 0x44, 0xC8, 0x54, 0x68, 0x44, 0xE8, 0x94,
  0x00, 0xB4, 0x08, 0x84, 0x74, 0xB4, 0x28, 0x6E, 0x74, 0xF4, 0xCC, 0x4A, 0x72, 0xF2, 0xA4, 0x8A,
  0x00, 0xAA, 0xA2, 0xA2, 0x74, 0x74, 0x74, 0x72, 0x44, 0x68, 0xB2, 0x32, 0xB2, 0x00, 0x22, 0x00,
  0x1A, 0x1A, 0x26, 0x26, 0x72, 0x72, 0x88, 0xC8, 0xC4, 0xCA, 0x26, 0x48, 0x44, 0x44, 0xA2, 0xC8,
  0x85, 0x45, 0x68, 0x48, 0x0A, 0x0A, 0x0A, 0x30, 0x03, 0x6C, 0xFE, 0x03, 0x28, 0x20, 0x4C, 0xFF,
  0x68, 0x85, 0x3A, 0x68, 0x85, 0x3B, 0x6C, 0xF0, 0x03, 0x20, 0x82, 0xF8, 0x20, 0xDA, 0xFA, 0x4C,
  0x65, 0xFF, 0xD8, 0x20, 0x84, 0xFE, 0x20, 0x2F, 0xFB, 0x20, 0x93, 0xFE, 0x20, 0x89, 0xFE, 0xAD,
  0x58, 0xC0, 0xAD, 0x5A, 0xC0, 0xAD, 0x5D, 0xC0, 0xAD, 0x5F, 0xC0, 0xAD, 0xFF, 0xCF, 0x2C, 0x10,
  0xC0, 0xD8, 0x20, 0x3A, 0xFF, 0xAD, 0xF3, 0x03, 0x49, 0xA5, 0xCD, 0xF4, 0x03, 0xD0, 0x17, 0xAD,
  0xF2, 0x03, 0xD0, 0x0F, 0xA9, 0xE0, 0xCD, 0xF3, 0x03, 0xD0, 0x08, 0xA0, 0x03, 0x8C, 0xF2, 0x03,
  0x4C, 0x00, 0xE0, 0x6C, 0xF2, 0x03, 0x20, 0x60, 0xFB, 0xA2, 0x05, 0xBD, 0xFC, 0xFA, 0x9D, 0xEF,
  0x03, 0xCA, 0xD0, 0xF7, 0xA9, 0xC8, 0x86, 0x00, 0x85, 0x01, 0xA0, 0x07, 0xC6, 0x01, 0xA5, 0x01,
  0xC9, 0xC0, 0xF0, 0xD7, 0x8D, 0xF8, 0x07, 0xB1, 0x00, 0xD9, 0x01, 0xFB, 0xD0, 0xEC, 0x88, 0x88,
  0x10, 0xF5, 0x6C, 0x00, 0x00, 0xEA, 0xEA, 0x20, 0x8E, 0xFD, 0xA9, 0x45, 0x85, 0x40, 0xA9, 0x00,
  0x85, 0x41, 0xA2, 0xFB, 0xA9, 0xA0, 0x20, 0xED, 0xFD, 0xBD, 0x1E, 0xFA, 0x20, 0xED, 0xFD, 0xA9,
  0xBD, 0x20, 0xED, 0xFD, 0xB5, 0x4A, 0x20, 0xDA, 0xFD, 0xE8, 0x30, 0xE8, 0x60, 0x59, 0xFA, 0x00,
  0xE0, 0x45, 0x20, 0xFF, 0x00, 0xFF, 0x03, 0xFF, 0x3C, 0xC1, 0xD0, 0xD0, 0xCC, 0xC5, 0xA0, 0xDD,
  0xDB, 0xC4, 0xC2, 0xC1, 0xFF, 0xC3, 0xFF, 0xFF, 0xFF, 0xC1, 0xD8, 0xD9, 0xD0, 0xD3, 0xAD, 0x70,
  0xC0, 0xA0, 0x00, 0xEA, 0xEA, 0xBD, 0x64, 0xC0, 0x10, 0x04, 0xC8, 0xD0, 0xF8, 0x88, 0x60, 0xA9,
  0x00, 0x85, 0x48, 0xAD, 0x56, 0xC0, 0xAD, 0x54, 0xC0, 0xAD, 0x51, 0xC0, 0xA9, 0x00, 0xF0, 0x0B,
  0xAD, 0x50, 0xC0, 0xAD, 0x53, 0xC0, 0x20, 0x36, 0xF8, 0xA9, 0x14, 0x85, 0x22, 0xA9, 0x00, 0x85,
  0x20, 0xA9, 0x28, 0x85, 0x21, 0xA9, 0x18, 0x85, 0x23, 0xA9, 0x17, 0x85, 0x25, 0x4C, 0x22, 0xFC,
  0x20, 0x58, 0xFC, 0xA0, 0x08, 0xB9, 0x08, 0xFB, 0x99, 0x0E, 0x04, 0x88, 0xD0, 0xF7, 0x60, 0xAD,
  0xF3, 0x03, 0x49, 0xA5, 0x8D, 0xF4, 0x03, 0x60, 0xC9, 0x8D, 0xD0, 0x18, 0xAC, 0x00, 0xC0, 0x10,
  0x13, 0xC0, 0x93, 0xD0, 0x0F, 0x2C, 0x10, 0xC0, 0xAC, 0x00, 0xC0, 0x10, 0xFB, 0xC0, 0x83, 0xF0,
  0x03, 0x2C, 0x10, 0xC0, 0x4C, 0xFD, 0xFB, 0x38, 0x4C, 0x2C, 0xFC, 0xA8, 0xB9, 0x48, 0xFA, 0x20,
  0x97, 0xFB, 0x20, 0x0C, 0xFD, 0xC9, 0xCE, 0xB0, 0xEE, 0xC9, 0xC9, 0x90, 0xEA, 0xC9, 0xCC, 0xF0,
  0xE6, 0xD0, 0xE8, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA,
  0xEA, 0x48, 0x4A, 0x29, 0x03, 0x09, 0x04, 0x85, 0x29, 0x68, 0x29, 0x18, 0x90, 0x02, 0x69, 0x7F,
  0x85, 0x28, 0x0A, 0x0A, 0x05, 0x28, 0x85, 0x28, 0x60, 0xC9, 0x87, 0xD0, 0x12, 0xA9, 0x40, 0x20,
  0xA8, 0xFC, 0xA0, 0xC0, 0xA9, 0x0C, 0x20, 0xA8, 0xFC, 0xAD, 0x30, 0xC0, 0x88, 0xD0, 0xF5, 0x60,
  0xA4, 0x24, 0x91, 0x28, 0xE6, 0x24, 0xA5, 0x24, 0xC5, 0x21, 0xB0, 0x66, 0x60, 0xC9, 0xA0, 0xB0,
  0xEF, 0xA8, 0x10, 0xEC, 0xC9, 0x8D, 0xF0, 0x5A, 0xC9, 0x8A, 0xF0, 0x5A, 0xC9, 0x88, 0xD0, 0xC9,
  0xC6, 0x24, 0x10, 0xE8, 0xA5, 0x21, 0x85, 0x24, 0xC6, 0x24, 0xA5, 0x22, 0xC5, 0x25, 0xB0, 0x0B,
  0xC6, 0x25, 0xA5, 0x25, 0x20, 0xC1, 0xFB, 0x65, 0x20, 0x85, 0x28, 0x60, 0x49, 0xC0, 0xF0, 0x28,
  0x69, 0xFD, 0x90, 0xC0, 0xF0, 0xDA, 0x69, 0xFD, 0x90, 0x2C, 0xF0, 0xDE, 0x69, 0xFD, 0x90, 0x5C,
  0xD0, 0xE9, 0xA4, 0x24, 0xA5, 0x25, 0x48, 0x20, 0x24, 0xFC, 0x20, 0x9E, 0xFC, 0xA0, 0x00, 0x68,
  0x69, 0x00, 0xC5, 0x23, 0x90, 0xF0, 0xB0, 0xCA, 0xA5, 0x22, 0x85, 0x25, 0xA0, 0x00, 0x84, 0x24,
  0xF0, 0xE4, 0xA9, 0x00, 0x85, 0x24, 0xE6, 0x25, 0xA5, 0x25, 0xC5, 0x23, 0x90, 0xB6, 0xC6, 0x25,
  0xA5, 0x22, 0x48, 0x20, 0x24, 0xFC, 0xA5, 0x28, 0x85, 0x2A, 0xA5, 0x29, 0x85, 0x2B, 0xA4, 0x21,
  0x88, 0x68, 0x69, 0x01, 0xC5, 0x23, 0xB0, 0x0D, 0x48, 0x20, 0x24, 0xFC, 0xB1, 0x28, 0x91, 0x2A,
  0x88, 0x10, 0xF9, 0x30, 0xE1, 0xA0, 0x00, 0x20, 0x9E, 0xFC, 0xB0, 0x86, 0xA4, 0x24, 0xA9, 0xA0,
  0x91, 0x28, 0xC8, 0xC4, 0x21, 0x90, 0xF9, 0x60, 0x38, 0x48, 0xE9, 0x01, 0xD0, 0xFC, 0x68, 0xE9,
  0x01, 0xD0, 0xF6, 0x60, 0xE6, 0x42, 0xD0, 0x02, 0xE6, 0x43, 0xA5, 0x3C, 0xC5, 0x3E, 0xA5, 0x3D,
  0xE5, 0x3F, 0xE6, 0x3C, 0xD0, 0x02, 0xE6, 0x3D, 0x60, 0xA0, 0x4B, 0x20, 0xDB, 0xFC, 0xD0, 0xF9,
  0x69, 0xFE, 0xB0, 0xF5, 0xA0, 0x21, 0x20, 0xDB, 0xFC, 0xC8, 0xC8, 0x88, 0xD0, 0xFD, 0x90, 0x05,
  0xA0, 0x32, 0x88, 0xD0, 0xFD, 0xAC, 0x20, 0xC0, 0xA0, 0x2C, 0xCA, 0x60, 0xA2, 0x08, 0x48, 0x20,
  0xFA, 0xFC, 0x68, 0x2A, 0xA0, 0x3A, 0xCA, 0xD0, 0xF5, 0x60, 0x20, 0xFD, 0xFC, 0x88, 0xAD, 0x60,
  0xC0, 0x45, 0x2F, 0x10, 0xF8, 0x45, 0x2F, 0x85, 0x2F, 0xC0, 0x80, 0x60, 0xA4, 0x24, 0xB1, 0x28,
  0x48, 0x29, 0x3F, 0x09, 0x40, 0x91, 0x28, 0x68, 0x6C, 0x38, 0x00, 0xE6, 0x4E, 0xD0, 0x02, 0xE6,
  0x4F, 0x2C, 0x00, 0xC0, 0x10, 0xF5, 0x91, 0x28, 0xAD, 0x00, 0xC0, 0x2C, 0x10, 0xC0, 0x60, 0x20,
  0x0C, 0xFD, 0x20, 0xA5, 0xFB, 0x20, 0x0C, 0xFD, 0xC9, 0x9B, 0xF0, 0xF3, 0x60, 0xA5, 0x32, 0x48,
  0xA9, 0xFF, 0x85, 0x32, 0xBD, 0x00, 0x02, 0x20, 0xED, 0xFD, 0x68, 0x85, 0x32, 0xBD, 0x00, 0x02,
  0xC9, 0x88, 0xF0, 0x1D, 0xC9, 0x98, 0xF0, 0x0A, 0xE0, 0xF8, 0x90, 0x03, 0x20, 0x3A, 0xFF, 0xE8,
  0xD0, 0x13, 0xA9, 0xDC, 0x20, 0xED, 0xFD, 0x20, 0x8E, 0xFD, 0xA5, 0x33, 0x20, 0xED, 0xFD, 0xA2,
  0x01, 0x8A, 0xF0, 0xF3, 0xCA, 0x20, 0x35, 0xFD, 0xC9, 0x95, 0xD0, 0x02, 0xB1, 0x28, 0xC9, 0xE0,
  0x90, 0x02, 0x29, 0xDF, 0x9D, 0x00, 0x02, 0xC9, 0x8D, 0xD0, 0xB2, 0x20, 0x9C, 0xFC, 0xA9, 0x8D,
  0xD0, 0x5B, 0xA4, 0x3D, 0xA6, 0x3C, 0x20, 0x8E, 0xFD, 0x20, 0x40, 0xF9, 0xA0, 0x00, 0xA9, 0xAD,
  0x4C, 0xED, 0xFD, 0xA5, 0x3C, 0x09, 0x07, 0x85, 0x3E, 0xA5, 0x3D, 0x85, 0x3F, 0xA5, 0x3C, 0x29,
  0x07, 0xD0, 0x03, 0x20, 0x92, 0xFD, 0xA9, 0xA0, 0x20, 0xED, 0xFD, 0xB1, 0x3C, 0x20, 0xDA, 0xFD,
  0x20, 0xBA, 0xFC, 0x90, 0xE8, 0x60, 0x4A, 0x90, 0xEA, 0x4A, 0x4A, 0xA5, 0x3E, 0x90, 0x02, 0x49,
  0xFF, 0x65, 0x3C, 0x48, 0xA9, 0xBD, 0x20, 0xED, 0xFD, 0x68, 0x48, 0x4A, 0x4A, 0x4A, 0x4A, 0x20,
  0xE5, 0xFD, 0x68, 0x29, 0x0F, 0x09, 0xB0, 0xC9, 0xBA, 0x90, 0x02, 0x69, 0x06, 0x6C, 0x36, 0x00,
  0xC9, 0xA0, 0x90, 0x02, 0x25, 0x32, 0x84, 0x35, 0x48, 0x20, 0x78, 0xFB, 0x68, 0xA4, 0x35, 0x60,
  0xC6, 0x34, 0xF0, 0x9F, 0xCA, 0xD0, 0x16, 0xC9, 0xBA, 0xD0, 0xBB, 0x85, 0x31, 0xA5, 0x3E, 0x91,
  0x40, 0xE6, 0x40, 0xD0, 0x02, 0xE6, 0x41, 0x60, 0xA4, 0x34, 0xB9, 0xFF, 0x01, 0x85, 0x31, 0x60,
  0xA2, 0x01, 0xB5, 0x3E, 0x95, 0x42, 0x95, 0x44, 0xCA, 0x10, 0xF7, 0x60, 0xB1, 0x3C, 0x91, 0x42,
  0x20, 0xB4, 0xFC, 0x90, 0xF7, 0x60, 0xB1, 0x3C, 0xD1, 0x42, 0xF0, 0x1C, 0x20, 0x92, 0xFD, 0xB1,
  0x3C, 0x20, 0xDA, 0xFD, 0xA9, 0xA0, 0x20, 0xED, 0xFD, 0xA9, 0xA8, 0x20, 0xED, 0xFD, 0xB1, 0x42,
  0x20, 0xDA, 0xFD, 0xA9, 0xA9, 0x20, 0xED, 0xFD, 0x20, 0xB4, 0xFC, 0x90, 0xD9, 0x60, 0x20, 0x75,
  0xFE, 0xA9, 0x14, 0x48, 0x20, 0xD0, 0xF8, 0x20, 0x53, 0xF9, 0x85, 0x3A, 0x84, 0x3B, 0x68, 0x38,
  0xE9, 0x01, 0xD0, 0xEF, 0x60, 0x8A, 0xF0, 0x07, 0xB5, 0x3C, 0x95, 0x3A, 0xCA, 0x10, 0xF9, 0x60,
  0xA0, 0x3F, 0xD0, 0x02, 0xA0, 0xFF, 0x84, 0x32, 0x60, 0xA9, 0x00, 0x85, 0x3E, 0xA2, 0x38, 0xA0,
  0x1B, 0xD0, 0x08, 0xA9, 0x00, 0x85, 0x3E, 0xA2, 0x36, 0xA0, 0xF0, 0xA5, 0x3E, 0x29, 0x0F, 0xF0,
  0x06, 0x09, 0xC0, 0xA0, 0x00, 0xF0, 0x02, 0xA9, 0xFD, 0x94, 0x00, 0x95, 0x01, 0x60, 0xEA, 0xEA,
  0x4C, 0x00, 0xE0, 0x4C, 0x03, 0xE0, 0x20, 0x75, 0xFE, 0x20, 0x3F, 0xFF, 0x6C, 0x3A, 0x00, 0x4C,
  0xD7, 0xFA, 0x60, 0xEA, 0x60, 0xEA, 0xEA, 0xEA, 0xEA, 0xEA, 0x4C, 0xF8, 0x03, 0xA9, 0x40, 0x20,
  0xC9, 0xFC, 0xA0, 0x27, 0xA2, 0x00, 0x41, 0x3C, 0x48, 0xA1, 0x3C, 0x20, 0xED, 0xFE, 0x20, 0xBA,
  0xFC, 0xA0, 0x1D, 0x68, 0x90, 0xEE, 0xA0, 0x22, 0x20, 0xED, 0xFE, 0xF0, 0x4D, 0xA2, 0x10, 0x0A,
  0x20, 0xD6, 0xFC, 0xD0, 0xFA, 0x60, 0x20, 0x00, 0xFE, 0x68, 0x68, 0xD0, 0x6C, 0x20, 0xFA, 0xFC,
  0xA9, 0x16, 0x20, 0xC9, 0xFC, 0x85, 0x2E, 0x20, 0xFA, 0xFC, 0xA0, 0x24, 0x20, 0xFD, 0xFC, 0xB0,
  0xF9, 0x20, 0xFD, 0xFC, 0xA0, 0x3B, 0x20, 0xEC, 0xFC, 0x81, 0x3C, 0x45, 0x2E, 0x85, 0x2E, 0x20,
  0xBA, 0xFC, 0xA0, 0x35, 0x90, 0xF0, 0x20, 0xEC, 0xFC, 0xC5, 0x2E, 0xF0, 0x0D, 0xA9, 0xC5, 0x20,
  0xED, 0xFD, 0xA9, 0xD2, 0x20, 0xED, 0xFD, 0x20, 0xED, 0xFD, 0xA9, 0x87, 0x4C, 0xED, 0xFD, 0xA5,
  0x48, 0x48, 0xA5, 0x45, 0xA6, 0x46, 0xA4, 0x47, 0x28, 0x60, 0x85, 0x45, 0x86, 0x46, 0x84, 0x47,
  0x08, 0x68, 0x85, 0x48, 0xBA, 0x86, 0x49, 0xD8, 0x60, 0x20, 0x84, 0xFE, 0x20, 0x2F, 0xFB, 0x20,
  0x93, 0xFE, 0x20, 0x89, 0xFE, 0xD8, 0x20, 0x3A, 0xFF, 0xA9, 0xAA, 0x85, 0x33, 0x20, 0x67, 0xFD,
  0x20, 0xC7, 0xFF, 0x20, 0xA7, 0xFF, 0x84, 0x34, 0xA0, 0x17, 0x88, 0x30, 0xE8, 0xD9, 0xCC, 0xFF,
  0xD0, 0xF8, 0x20, 0xBE, 0xFF, 0xA4, 0x34, 0x4C, 0x73, 0xFF, 0xA2, 0x03, 0x0A, 0x0A, 0x0A, 0x0A,
  0x0A, 0x26, 0x3E, 0x26, 0x3F, 0xCA, 0x10, 0xF8, 0xA5, 0x31, 0xD0, 0x06, 0xB5, 0x3F, 0x95, 0x3D,
  0x95, 0x41, 0xE8, 0xF0, 0xF3, 0xD0, 0x06, 0xA2, 0x00, 0x86, 0x3E, 0x86, 0x3F, 0xB9, 0x00, 0x02,
  0xC8, 0x49, 0xB0, 0xC9, 0x0A, 0x90, 0xD3, 0x69, 0x88, 0xC9, 0xFA, 0xB0, 0xCD, 0x60, 0xA9, 0xFE,
  0x48, 0xB9, 0xE3, 0xFF, 0x48, 0xA5, 0x31, 0xA0, 0x00, 0x84, 0x31, 0x60, 0xBC, 0xB2, 0xBE, 0xB2,
  0xEF, 0xC4, 0xB2, 0xA9, 0xBB, 0xA6, 0xA4, 0x06, 0x95, 0x07, 0x02, 0x05, 0xF0, 0x00, 0xEB, 0x93,
  0xA7, 0xC6, 0x99, 0xB2, 0xC9, 0xBE, 0xC1, 0x35, 0x8C, 0xC4, 0x96, 0xAF, 0x17, 0x17, 0x2B, 0x1F,
  0x83, 0x7F, 0x5D, 0xCC, 0xB5, 0xFC, 0x17, 0x17, 0xF5, 0x03, 0xFB, 0x03, 0x62, 0xFA, 0x40, 0xFA
};
