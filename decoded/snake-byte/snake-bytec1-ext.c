// Loaded binary at [$3750..$854E]
// --code-at: 3 asserted edges applied
// Loaded segment [$00B1..$00C8]
// 293 new runtime blocks added
// code labels: 1694
// data labels: 300

#include "apple2tc/system2-inc.h"

static const uint8_t s_mem_00b1[0x0018];
static const uint8_t s_mem_3750[0x4dff];
static const uint8_t s_mem_d000[0x3000];

void init_emulated(void) {
  memcpy(s_ram + 0x00b1, s_mem_00b1, 0x0018);
  memcpy(s_ram + 0x3750, s_mem_3750, 0x4dff);
  memcpy(s_ram + 0xd000, s_mem_d000, 0x3000);
}

static inline uint8_t ovf8(uint8_t res, uint8_t a, uint8_t b) {
  return (~(a ^ b) & (a ^ res)) >> 7;
}
static uint16_t adc_dec16(uint8_t a, uint8_t b, uint8_t cf) {
  struct ResAndStatus res = adc_decimal(a, b, cf);
  return res.result | (res.status << 8);
}
static uint16_t sbc_dec16(uint8_t a, uint8_t b, uint8_t cf) {
  struct ResAndStatus res = sbc_decimal(a, b, cf);
  return res.result | (res.status << 8);
}

static int cmp_map_addr(const void *a, const void *b) {
  return *((const int *)a) - *((const int *)b);
}

static unsigned
addr_to_block_id(uint16_t from_pc, uint16_t addr, const unsigned *block_map, size_t length) {
  unsigned uaddr = addr;
  const unsigned *p =
      (const unsigned *)bsearch(&uaddr, block_map, length, sizeof(unsigned) * 2, cmp_map_addr);
  if (p)
    return p[1];
  fprintf(stderr, "Unknown address $%04X\n", addr);
  error_handler(from_pc);
  abort();
};

void func_t001(uint16_t ret_addr);
void rom_plot(uint16_t ret_addr);
void rom_hline(uint16_t ret_addr);
void rom_setcol(uint16_t ret_addr);
void rom_scrn(uint16_t ret_addr);
void rom_home(uint16_t ret_addr);
void rom_fc68(uint16_t ret_addr);
void rom_cout(uint16_t ret_addr);
void rom_setkbd(uint16_t ret_addr);
void rom_setvid(uint16_t ret_addr);
void FUNC_BCC(uint16_t ret_addr);
void func_60e4(uint16_t ret_addr);
void func_60e7(uint16_t ret_addr);
void func_6127(uint16_t ret_addr);
void func_6148(uint16_t ret_addr);
void func_615a(uint16_t ret_addr);
void func_6217(uint16_t ret_addr);
void func_64a9(uint16_t ret_addr);
void func_64c8(uint16_t ret_addr);
void func_6594(uint16_t ret_addr);
void func_660f(uint16_t ret_addr);
void func_6641(uint16_t ret_addr);
void func_69a9(uint16_t ret_addr);
void func_69c3(uint16_t ret_addr);
void func_6ab8(uint16_t ret_addr);
void func_6b3d(uint16_t ret_addr);
void func_6b93(uint16_t ret_addr);
void func_6bda(uint16_t ret_addr);
void func_6bef(uint16_t ret_addr);
void func_6bfb(uint16_t ret_addr);
void func_6c4b(uint16_t ret_addr);
void func_6c72(uint16_t ret_addr);
void func_7000(uint16_t ret_addr);
void func_7019(uint16_t ret_addr);
void func_7024(uint16_t ret_addr);
void func_702b(uint16_t ret_addr);
void func_7045(uint16_t ret_addr);
void func_71cd(uint16_t ret_addr);
void func_71f3(uint16_t ret_addr);
void func_7226(uint16_t ret_addr);
void func_7267(uint16_t ret_addr);
void func_728d(uint16_t ret_addr);
void func_7590(uint16_t ret_addr);
void func_75d1(uint16_t ret_addr);
void func_7633(uint16_t ret_addr);
void func_7642(uint16_t ret_addr);
void FUNC_MVBLKUP2(uint16_t ret_addr);
void FUNC_FNDLIN(uint16_t ret_addr);
void FUNC_FNDLIN2(uint16_t ret_addr);
void FUNC_CLRTXTPTR(uint16_t ret_addr);
void FUNC_RESTORE(uint16_t ret_addr);
void FUNC_CRDO(uint16_t ret_addr);
void FUNC_NEGATE(uint16_t ret_addr);
void FUNC_OUTSP(uint16_t ret_addr);
void FUNC_OUTQUES(uint16_t ret_addr);
void FUNC_OUTDO(uint16_t ret_addr);
void FUNC_ISLETC(uint16_t ret_addr);
void FUNC_GETARY(uint16_t ret_addr);
void FUNC_GARBAGE(uint16_t ret_addr);
void FUNC_CHKSMPLVAR(uint16_t ret_addr);
void FUNC_CHKVAR(uint16_t ret_addr);
void FUNC_MOVINS(uint16_t ret_addr);
void FUNC_MOVSTR(uint16_t ret_addr);
void FUNC_MOVSTR1(uint16_t ret_addr);
void FUNC_FREFAC(uint16_t ret_addr);
void FUNC_FRETMP(uint16_t ret_addr);
void FUNC_FRETMS(uint16_t ret_addr);
void FUNC_TWSCMPFAC(uint16_t ret_addr);
void FUNC_TCFACMANT(uint16_t ret_addr);
void FUNC_INCFACMAN(uint16_t ret_addr);
void FUNC_SHFTRGHT(uint16_t ret_addr);
void FUNC_SHFTRGHT4(uint16_t ret_addr);
void FUNC_MULT1(uint16_t ret_addr);
void FUNC_MULT2(uint16_t ret_addr);
void FUNC_UPAY2ARG(uint16_t ret_addr);
void FUNC_UPAY2FAC(uint16_t ret_addr);
void FUNC_FCOMP(uint16_t ret_addr);
void FUNC_QINT(uint16_t ret_addr);
void FUNC_NORMAL(uint16_t ret_addr);
void FUNC_INIT(uint16_t ret_addr);
void FUNC_APPLEII(uint16_t ret_addr);
void FUNC_ESCOLD(uint16_t ret_addr);
void FUNC_BASCALC(uint16_t ret_addr);
void FUNC_VTABZ(uint16_t ret_addr);
void FUNC_CLREOL(uint16_t ret_addr);
void FUNC_CLREOLZ(uint16_t ret_addr);
void FUNC_MON_WAIT(uint16_t ret_addr);
void FUNC_CROUT(uint16_t ret_addr);
void FUNC_SETNORM(uint16_t ret_addr);
void FUNC_BELL(uint16_t ret_addr);
void FUNC_SAV1(uint16_t ret_addr);

static void emulated_entry_point(void) {
  func_t001(false);
}

void FUNC_BCC(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$0090*/ CYCLES(0x0090, 2);
            fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x0090, 0x0090, 0x03); error_handler(0x0090);
}


void func_60e4(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$60E4*/ CYCLES(0x60e4, 6);
            func_6127(0xfffe);
            func_60e7(0x0000);
            if (ret_addr) pop16(); return;
}


void func_60e7(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$60E7*/ CYCLES(0x60e7, 28);
            s_x = 0x00;
  /*$60E9*/ tmp1_U8 = ram_peek(0x0003);
  /*$60EB*/ ram_poke(0x0007, 0x00);
  /*$60F0*/ ram_poke(0x0004, ram_peek((0x6000 + tmp1_U8)));
  /*$60F5*/ ram_poke(0x0005, ram_peek((0x6030 + tmp1_U8)));
bb_1:
  /*$60F7*/ CYCLES(0x60f7, 16);
  /*$60F9*/ ram_poke(0x0006, ram_peek(0x0001));
  /*$60FE*/ ram_poke(0x0006, (uint8_t)((ram_peek(0x0006) << 0x01) | (ram_peek(0x0007) & 0x01)));
  /*$6100*/ CYCLES(0x6100, 65);
            ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) << 0x01));
  /*$6102*/ ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) << 0x01));
  /*$610A*/ ram_poke(0x0006, ((ram_peek(0x0002) & 0x03) | ram_peek(0x0006)));
  /*$6111*/ tmp1_U8 = ram_peek(0x0002);
            s_y = tmp1_U8;
  /*$6113*/ tmp2_U8 = s_x;
  /*$6116*/ poke((ram_peek16al(0x0004) + tmp1_U8), (ram_peek((0x6064 + ram_peek(0x0006))) & ram_peek((0x6060 + tmp2_U8))));
  /*$6118*/ s_x = (uint8_t)(tmp2_U8 + 0x01);
  /*$6119*/ ram_poke(0x0007, (uint8_t)(ram_peek(0x0007) + 0x01));
  /*$611B*/ s_a = ram_peek(0x0005);
  /*$611D*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_3;
bb_2:
  /*$611E*/ tmp3_U16 = s_a;
            tmp4_U16 = (tmp3_U16 + 0x0004) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x0004);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_4;
bb_3:
  /*$611E*/ tmp4_U16 = adc_dec16(s_a, 0x04, s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
bb_4:
  /*$6120*/ ram_poke(0x0005, s_a);
  /*$6122*/ tmp2_U8 = s_x;
            tmp1_U8 = tmp2_U8 != 0x04;
            s_status_not_z = tmp1_U8;
            s_status_c = (tmp2_U8 >= 0x04);
            s_status_n = ((uint8_t)(tmp2_U8 - 0x04) & 0x80);
            branchTarget = true;
            if (tmp1_U8)
              goto bb_1;
bb_5:
  /*$6126*/ CYCLES(0x6126, 2);
            if (ret_addr) pop16(); return;
}


void func_6127(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6127*/ CYCLES(0x6127, 57);
  /*$612A*/ tmp1_U16 = ram_peek(0x0000) << 0x02;
            tmp2_U8 = (uint8_t)tmp1_U16;
  /*$612F*/ ram_poke(0x6060, ram_peek((0x6174 + (tmp1_U16 & 0x00ff))));
  /*$6136*/ ram_poke(0x6061, ram_peek((0x6174 + (uint8_t)(tmp2_U8 + 0x01))));
  /*$613D*/ ram_poke(0x6062, ram_peek((0x6174 + (uint8_t)(tmp2_U8 + 0x02))));
  /*$6140*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x03);
            s_x = tmp2_U8;
  /*$6144*/ ram_poke(0x6063, ram_peek((0x6174 + tmp2_U8)));
  /*$6147*/ if (ret_addr) pop16(); return;
}


void func_6148(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6148*/ CYCLES(0x6148, 6);
            func_6127(0xfffe);
            branchTarget = true;
bb_1:
  /*$614B*/ CYCLES(0x614b, 6);
            func_60e7(0xfffe);
  /*$614E*/ CYCLES(0x614e, 11);
            tmp1_U8 = ram_peek(0x0002);
  /*$6150*/ tmp2_U8 = ram_peek(0x0008);
            tmp3_U8 = tmp1_U8 != tmp2_U8;
            s_status_not_z = tmp3_U8;
            s_status_c = (tmp1_U8 >= tmp2_U8);
            s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
            branchTarget = true;
            if (!tmp3_U8)
              goto bb_3;
bb_2:
  /*$6154*/ CYCLES(0x6154, 9);
            ram_poke(0x0002, (uint8_t)(ram_peek(0x0002) + 0x01));
            branchTarget = true;
            goto bb_1;
bb_3:
  /*$6159*/ CYCLES(0x6159, 2);
            if (ret_addr) pop16(); return;
}


void func_615a(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$615A*/ CYCLES(0x615a, 6);
            func_6127(0xfffe);
            branchTarget = true;
            goto bb_2;
bb_1:
  /*$6159*/ CYCLES(0x6159, 2);
            if (ret_addr) pop16(); return;
bb_2:
  /*$615D*/ CYCLES(0x615d, 6);
            func_60e7(0xfffe);
  /*$6160*/ CYCLES(0x6160, 11);
            tmp1_U8 = ram_peek(0x0003);
  /*$6162*/ tmp2_U8 = ram_peek(0x0008);
            tmp3_U8 = tmp1_U8 != tmp2_U8;
            s_status_not_z = tmp3_U8;
            s_status_c = (tmp1_U8 >= tmp2_U8);
            s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
            branchTarget = true;
            if (!tmp3_U8)
              goto bb_1;
bb_3:
  /*$6166*/ CYCLES(0x6166, 9);
            ram_poke(0x0003, (uint8_t)(ram_peek(0x0003) + 0x01));
            branchTarget = true;
            goto bb_2;
}


void func_6217(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6217*/ CYCLES(0x6217, 14);
            s_x = ram_peek(0x624d);
  /*$621A*/ tmp1_U8 = io_peek(0xc000);
            s_a = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_2;
bb_1:
  /*$6216*/ CYCLES(0x6216, 2);
            if (ret_addr) pop16(); return;
bb_2:
  /*$621F*/ CYCLES(0x621f, 26);
            tmp1_U8 = s_a;
            io_poke(0xc010, tmp1_U8);
  /*$6222*/ tmp2_U8 = s_x;
            ram_poke((0x623c + tmp2_U8), tmp1_U8);
  /*$6225*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
            s_x = tmp2_U8;
  /*$6227*/ tmp2_U8 = tmp2_U8 & 0x0f;
            s_a = tmp2_U8;
            branchTarget = true;
            if (!(tmp2_U8 != ram_peek(0x624c)))
              goto bb_1;
bb_3:
  /*$622E*/ CYCLES(0x622e, 7);
            ram_poke(0x624d, s_a);
  /*$6231*/ if (ret_addr) pop16(); return;
}


void func_64a9(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$64A9*/ CYCLES(0x64a9, 4);
            s_x = 0x00;
bb_1:
  /*$64AB*/ CYCLES(0x64ab, 4);
  /*$64AC*/ s_y = s_x;
bb_2:
  /*$64AD*/ CYCLES(0x64ad, 6);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_2;
bb_3:
  /*$64B0*/ CYCLES(0x64b0, 16);
  /*$64B3*/ tmp1_U8 = peek((0xc000 + ram_peek(0x6c49)));
  /*$64B6*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_1;
bb_4:
  /*$64B9*/ CYCLES(0x64b9, 4);
  /*$64BA*/ s_y = s_x;
bb_5:
  /*$64BB*/ CYCLES(0x64bb, 6);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_5;
bb_6:
  /*$64BE*/ CYCLES(0x64be, 16);
            tmp1_U8 = ram_peek(0x6c49);
            s_y = tmp1_U8;
  /*$64C1*/ tmp1_U8 = peek((0xc000 + tmp1_U8));
            s_a = tmp1_U8;
  /*$64C4*/ tmp1_U8 = (uint8_t)(s_x + 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_x = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_4;
bb_7:
  /*$64C7*/ CYCLES(0x64c7, 2);
            if (ret_addr) pop16(); return;
}


void func_64c8(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$64C8*/ CYCLES(0x64c8, 18);
  /*$64CA*/ ram_poke(0x6c4a, 0x00);
  /*$64CD*/ tmp1_U8 = ram_peek(0x6634);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_2;
bb_1:
  /*$64D2*/ CYCLES(0x64d2, 2);
            if (ret_addr) pop16(); return;
bb_2:
  /*$64D3*/ CYCLES(0x64d3, 41);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_4;
bb_3:
  /*$64D4*/ s_a = (uint8_t)((s_a + ram_peek(0x6636)) + s_status_c);
            goto bb_5;
bb_4:
  /*$64D4*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x6636), s_status_c));
bb_5:
  /*$64D7*/ ram_poke(0x6638, s_a);
  /*$64DA*/ s_a = ram_peek(0x6633);
  /*$64DD*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_7;
bb_6:
  /*$64DE*/ tmp2_U16 = s_a;
            tmp3_U16 = ram_peek(0x6635);
            tmp4_U16 = (tmp2_U16 + tmp3_U16) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp2_U16, (uint8_t)tmp3_U16);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_8;
bb_7:
  /*$64DE*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x6635), s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
bb_8:
  /*$64E1*/ tmp1_U8 = s_a;
            ram_poke(0x6637, tmp1_U8);
  /*$64E4*/ s_y = tmp1_U8;
  /*$64E5*/ s_a = ram_peek(0x6638);
  /*$64E8*/ rom_scrn(0xfffe);
  /*$64EB*/ CYCLES(0x64eb, 7);
            tmp1_U8 = s_a;
            s_status_c = (tmp1_U8 >= 0x00);
            branchTarget = true;
            if (!(tmp1_U8 != 0x00))
              goto bb_15;
bb_9:
  /*$64EF*/ CYCLES(0x64ef, 16);
            s_y = ram_peek(0x6637);
  /*$64F2*/ s_a = ram_peek(0x6634);
  /*$64F5*/ rom_scrn(0xfffe);
  /*$64F8*/ CYCLES(0x64f8, 7);
            branchTarget = true;
            if (!(s_a != 0x00))
              goto bb_11;
bb_10:
  /*$64FC*/ CYCLES(0x64fc, 29);
  /*$64FF*/ ram_poke(0x6637, ram_peek(0x6633));
  /*$6507*/ ram_poke(0x6635, (ram_peek(0x6635) ^ 0xfe));
  /*$650A*/ ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
bb_11:
  /*$650D*/ CYCLES(0x650d, 16);
            s_a = ram_peek(0x6638);
  /*$6510*/ s_y = ram_peek(0x6633);
  /*$6513*/ rom_scrn(0xfffe);
  /*$6516*/ CYCLES(0x6516, 7);
            tmp1_U8 = s_a;
            s_status_c = (tmp1_U8 >= 0x00);
            branchTarget = true;
            if (!(tmp1_U8 != 0x00))
              goto bb_13;
bb_12:
  /*$651A*/ CYCLES(0x651a, 29);
  /*$651D*/ ram_poke(0x6638, ram_peek(0x6634));
  /*$6525*/ ram_poke(0x6636, (ram_peek(0x6636) ^ 0xfe));
  /*$6528*/ ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
bb_13:
  /*$652B*/ CYCLES(0x652b, 9);
            branchTarget = true;
            if (ram_peek(0x6c4a))
              goto bb_15;
bb_14:
  /*$6530*/ CYCLES(0x6530, 48);
  /*$6533*/ ram_poke(0x6637, ram_peek(0x6633));
  /*$6539*/ ram_poke(0x6638, ram_peek(0x6634));
  /*$6541*/ ram_poke(0x6635, (ram_peek(0x6635) ^ 0xfe));
  /*$6549*/ ram_poke(0x6636, (ram_peek(0x6636) ^ 0xfe));
bb_15:
  /*$654C*/ CYCLES(0x654c, 12);
            s_a = 0x00;
  /*$654E*/ ram_poke(0x0001, 0x00);
  /*$6550*/ rom_setcol(0xfffe);
  /*$6553*/ CYCLES(0x6553, 23);
  /*$6556*/ ram_poke(0x0002, ram_peek(0x6633));
  /*$655B*/ ram_poke(0x0003, ram_peek(0x6634));
  /*$655D*/ func_60e4(0xfffe);
  /*$6560*/ CYCLES(0x6560, 16);
            s_a = ram_peek(0x6634);
  /*$6563*/ s_y = ram_peek(0x6633);
  /*$6566*/ rom_plot(0xfffe);
  /*$6569*/ CYCLES(0x6569, 16);
  /*$656B*/ ram_poke(0x0000, 0x1a);
  /*$656D*/ tmp1_U8 = ram_peek(0x6638);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_17;
bb_16:
  /*$6572*/ CYCLES(0x6572, 2);
            if (ret_addr) pop16(); return;
bb_17:
  /*$6573*/ CYCLES(0x6573, 35);
            tmp1_U8 = s_a;
            ram_poke(0x0003, tmp1_U8);
  /*$6575*/ ram_poke(0x6634, tmp1_U8);
  /*$6578*/ tmp1_U8 = ram_peek(0x6637);
  /*$657B*/ ram_poke(0x0002, tmp1_U8);
  /*$657D*/ ram_poke(0x6633, tmp1_U8);
  /*$6580*/ s_a = 0x03;
  /*$6582*/ ram_poke(0x0001, 0x03);
  /*$6584*/ rom_setcol(0xfffe);
  /*$6587*/ CYCLES(0x6587, 6);
            func_60e4(0xfffe);
  /*$658A*/ CYCLES(0x658a, 16);
            s_a = ram_peek(0x6634);
  /*$658D*/ tmp1_U8 = ram_peek(0x6633);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_y = tmp1_U8;
  /*$6590*/ rom_plot(0xfffe);
  /*$6593*/ CYCLES(0x6593, 2);
            if (ret_addr) pop16(); return;
}


void func_6594(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6594*/ CYCLES(0x6594, 9);
            branchTarget = true;
            if (ram_peek(0x0301))
              goto bb_5;
            else
              goto bb_4;
bb_1:
  /*$6200*/ CYCLES(0x6200, 14);
            tmp1_U8 = ram_peek(0x624c);
            s_x = tmp1_U8;
  /*$6203*/ tmp1_U8 = ram_peek((0x623c + tmp1_U8));
            s_a = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 & 0x80))
              goto bb_3;
bb_2:
  /*$6208*/ CYCLES(0x6208, 24);
            push8(s_a);
  /*$620B*/ tmp1_U8 = s_x;
            ram_poke((0x623c + tmp1_U8), 0x00);
  /*$620E*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
            s_x = tmp1_U8;
  /*$6212*/ ram_poke(0x624c, (tmp1_U8 & 0x0f));
  /*$6215*/ tmp1_U8 = pop8();
            s_a = tmp1_U8;
bb_3:
  /*$6216*/ CYCLES(0x6216, 2);
            if (ret_addr) pop16(); return;
bb_4:
  /*$6599*/ CYCLES(0x6599, 6);
            branchTarget = true;
            goto bb_1;
bb_5:
  /*$659C*/ CYCLES(0x659c, 46);
  /*$659F*/ ram_poke(0x6633, ram_peek(0x6639));
  /*$65A5*/ ram_poke(0x6634, ram_peek(0x663a));
  /*$65AB*/ ram_poke(0x6635, ram_peek(0x663d));
  /*$65B1*/ ram_poke(0x6636, ram_peek(0x663e));
  /*$65B4*/ func_64c8(0xfffe);
  /*$65B7*/ CYCLES(0x65b7, 53);
  /*$65BA*/ ram_poke(0x6639, ram_peek(0x6633));
  /*$65C0*/ ram_poke(0x663a, ram_peek(0x6634));
  /*$65C6*/ ram_poke(0x663d, ram_peek(0x6635));
  /*$65CC*/ ram_poke(0x663e, ram_peek(0x6636));
  /*$65CF*/ tmp1_U8 = ram_peek(0x0301);
  /*$65D2*/ s_status_c = (tmp1_U8 >= 0x01);
            branchTarget = true;
            if ((tmp1_U8 != 0x01))
              goto bb_7;
bb_6:
  /*$65D6*/ CYCLES(0x65d6, 6);
            branchTarget = true;
            goto bb_1;
bb_7:
  /*$65D9*/ CYCLES(0x65d9, 46);
  /*$65DC*/ ram_poke(0x6633, ram_peek(0x663b));
  /*$65E2*/ ram_poke(0x6634, ram_peek(0x663c));
  /*$65E8*/ ram_poke(0x6635, ram_peek(0x663f));
  /*$65EE*/ ram_poke(0x6636, ram_peek(0x6640));
  /*$65F1*/ func_64c8(0xfffe);
  /*$65F4*/ CYCLES(0x65f4, 46);
  /*$65F7*/ ram_poke(0x663b, ram_peek(0x6633));
  /*$65FD*/ ram_poke(0x663c, ram_peek(0x6634));
  /*$6603*/ ram_poke(0x663f, ram_peek(0x6635));
  /*$6609*/ ram_poke(0x6640, ram_peek(0x6636));
            branchTarget = true;
            goto bb_1;
}


void func_660f(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$660F*/ CYCLES(0x660f, 62);
            ram_poke(0x624f, s_a);
  /*$6614*/ ram_poke(0x6639, 0x01);
  /*$6617*/ ram_poke(0x663a, 0x01);
  /*$661A*/ ram_poke(0x663d, 0x01);
  /*$661D*/ ram_poke(0x663e, 0x01);
  /*$6620*/ ram_poke(0x6640, 0x01);
  /*$6623*/ ram_poke(0x663c, 0x01);
  /*$6628*/ ram_poke(0x663b, 0x26);
  /*$662D*/ ram_poke(0x663f, 0xff);
  /*$6630*/ s_status_not_z = 0x14;
            s_a = 0x14;
  /*$6632*/ if (ret_addr) pop16(); return;
}


void func_6641(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6641*/ CYCLES(0x6641, 16);
  /*$6643*/ ram_poke(0x0036, 0x4a);
  /*$6645*/ s_status_not_z = 0x66;
            s_status_n = 0x00;
            s_a = 0x66;
  /*$6647*/ ram_poke(0x0037, 0x66);
  /*$6649*/ if (ret_addr) pop16(); return;
}


void func_69a9(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$69A9*/ CYCLES(0x69a9, 7);
            branchTarget = true;
            if ((s_a != 0x9b))
              goto bb_3;
bb_1:
  /*$69AD*/ CYCLES(0x69ad, 9);
            tmp1_U8 = io_peek(0xc000);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 & 0x80))
              goto bb_1;
bb_2:
  /*$69B2*/ CYCLES(0x69b2, 6);
            io_poke(0xc010, s_a);
bb_3:
  /*$69B5*/ CYCLES(0x69b5, 7);
            tmp1_U8 = s_a;
            s_status_c = (tmp1_U8 >= 0x93);
            branchTarget = true;
            if ((tmp1_U8 != 0x93))
              goto bb_5;
bb_4:
  /*$69B9*/ CYCLES(0x69b9, 14);
  /*$69BE*/ ram_poke(0x69c2, (ram_peek(0x69c2) ^ 0x01));
bb_5:
  /*$69C1*/ CYCLES(0x69c1, 2);
            if (ret_addr) pop16(); return;
}


void func_69c3(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$69C3*/ CYCLES(0x69c3, 19);
  /*$69C6*/ ram_poke(0x6b39, ram_peek(0x624f));
  /*$69CB*/ ram_poke(0x6b3a, 0x01);
bb_1:
  /*$69CE*/ CYCLES(0x69ce, 16);
            s_a = ram_peek(0x6b3a);
  /*$69D1*/ s_y = ram_peek(0x6b39);
  /*$69D4*/ rom_scrn(0xfffe);
  /*$69D7*/ CYCLES(0x69d7, 7);
            tmp1_U8 = s_a;
            s_status_c = (tmp1_U8 >= 0x0f);
            branchTarget = true;
            if (!(tmp1_U8 != 0x0f))
              goto bb_9;
bb_2:
  /*$69DB*/ CYCLES(0x69db, 18);
            ram_poke(0x6b3a, (uint8_t)(ram_peek(0x6b3a) + 0x01));
            branchTarget = true;
            if ((ram_peek(0x6b3a) != 0x27))
              goto bb_1;
bb_3:
  /*$69E5*/ CYCLES(0x69e5, 18);
  /*$69E7*/ ram_poke(0x6b3a, 0x01);
  /*$69EA*/ tmp1_U8 = (uint8_t)(ram_peek(0x6b39) - 0x01);
            ram_poke(0x6b39, tmp1_U8);
            branchTarget = true;
            if (tmp1_U8)
              goto bb_1;
bb_4:
  /*$69EF*/ CYCLES(0x69ef, 11);
  /*$69F2*/ ram_poke(0x6b39, ram_peek(0x624f));
bb_5:
  /*$69F5*/ CYCLES(0x69f5, 16);
            s_a = ram_peek(0x6b3a);
  /*$69F8*/ s_y = ram_peek(0x6b39);
  /*$69FB*/ rom_scrn(0xfffe);
  /*$69FE*/ CYCLES(0x69fe, 4);
            tmp1_U8 = s_a;
            tmp2_U8 = tmp1_U8 != 0x0f;
            s_status_not_z = tmp2_U8;
            s_status_c = (tmp1_U8 >= 0x0f);
  /*$6A00*/ CYCLES(0x6a00, 4);
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_9;
bb_6:
  /*$6A02*/ CYCLES(0x6a02, 18);
            ram_poke(0x6b3a, (uint8_t)(ram_peek(0x6b3a) + 0x01));
            branchTarget = true;
            if ((ram_peek(0x6b3a) != 0x27))
              goto bb_5;
bb_7:
  /*$6A0C*/ CYCLES(0x6a0c, 26);
  /*$6A0E*/ ram_poke(0x6b3a, 0x01);
  /*$6A11*/ ram_poke(0x6b39, (uint8_t)(ram_peek(0x6b39) + 0x01));
  /*$6A14*/ tmp2_U8 = ram_peek(0x6b39);
  /*$6A17*/ s_status_c = (tmp2_U8 >= 0x27);
            branchTarget = true;
            if ((tmp2_U8 != 0x27))
              goto bb_5;
bb_8:
  /*$6A1B*/ CYCLES(0x6a1b, 18);
  /*$6A1D*/ ram_poke(0x6b3a, 0x00);
  /*$6A22*/ ram_poke(0x6b39, 0x14);
bb_9:
  /*$6A25*/ CYCLES(0x6a25, 23);
  /*$6A28*/ ram_poke(0x6b3b, ram_peek(0x6b39));
  /*$6A2E*/ ram_poke(0x6b3c, ram_peek(0x6b3a));
  /*$6A31*/ if (ret_addr) pop16(); return;
}


void func_6ab8(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;
  uint8_t tmp5_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6AB8*/ CYCLES(0x6ab8, 50);
            tmp1_U8 = ram_peek(0x6b38);
            s_x = tmp1_U8;
  /*$6ABB*/ s_a = ram_peek((0x6232 + tmp1_U8));
  /*$6ABE*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_2;
bb_1:
  /*$6ABF*/ s_a = (uint8_t)((s_a + ram_peek(0x624f)) + s_status_c);
            goto bb_3;
bb_2:
  /*$6ABF*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x624f), s_status_c));
bb_3:
  /*$6AC2*/ ram_poke(0x6637, s_a);
  /*$6AC5*/ s_a = ram_peek((0x6237 + s_x));
  /*$6AC8*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_5;
bb_4:
  /*$6AC9*/ tmp2_U16 = s_a;
            tmp3_U16 = ram_peek(0x6250);
            tmp4_U16 = (tmp2_U16 + tmp3_U16) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp2_U16, (uint8_t)tmp3_U16);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_6;
bb_5:
  /*$6AC9*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x6250), s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
bb_6:
  /*$6ACC*/ ram_poke(0x6638, s_a);
  /*$6ACF*/ s_y = ram_peek(0x6637);
  /*$6AD2*/ rom_scrn(0xfffe);
  /*$6AD5*/ CYCLES(0x6ad5, 7);
            tmp1_U8 = s_a;
            tmp5_U8 = tmp1_U8 != 0x00;
            s_status_not_z = tmp5_U8;
            s_status_c = (tmp1_U8 >= 0x00);
            s_status_n = (tmp1_U8 & 0x80);
            branchTarget = true;
            if (!tmp5_U8)
              goto bb_8;
bb_7:
  /*$6AD9*/ CYCLES(0x6ad9, 4);
            tmp5_U8 = s_a;
            s_status_not_z = (tmp5_U8 != 0x0f);
            s_status_c = (tmp5_U8 >= 0x0f);
            s_status_n = ((uint8_t)(tmp5_U8 - 0x0f) & 0x80);
bb_8:
  /*$6ADB*/ CYCLES(0x6adb, 4);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_10;
bb_9:
  /*$6ADD*/ CYCLES(0x6add, 2);
            if (ret_addr) pop16(); return;
bb_10:
  /*$6ADE*/ CYCLES(0x6ade, 18);
  /*$6AE0*/ ram_poke(0x6c4a, 0x00);
  /*$6AE3*/ tmp5_U8 = ram_peek(0x6638);
            s_status_not_z = tmp5_U8;
            s_status_n = (tmp5_U8 & 0x80);
            s_a = tmp5_U8;
            branchTarget = true;
            if (!tmp5_U8)
              goto bb_9;
bb_11:
  /*$6AE8*/ CYCLES(0x6ae8, 12);
  /*$6AEB*/ s_y = (uint8_t)(ram_peek(0x6637) + 0x01);
  /*$6AEC*/ rom_scrn(0xfffe);
  /*$6AEF*/ CYCLES(0x6aef, 7);
            branchTarget = true;
            if ((s_a != 0x00))
              goto bb_13;
bb_12:
  /*$6AF3*/ CYCLES(0x6af3, 6);
            ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
bb_13:
  /*$6AF6*/ CYCLES(0x6af6, 18);
            s_a = ram_peek(0x6638);
  /*$6AFC*/ s_y = (uint8_t)(ram_peek(0x6637) - 0x01);
  /*$6AFD*/ rom_scrn(0xfffe);
  /*$6B00*/ CYCLES(0x6b00, 7);
            branchTarget = true;
            if ((s_a != 0x00))
              goto bb_15;
bb_14:
  /*$6B04*/ CYCLES(0x6b04, 6);
            ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
bb_15:
  /*$6B07*/ CYCLES(0x6b07, 21);
            s_a = ram_peek(0x6638);
  /*$6B0A*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_17;
bb_16:
  /*$6B0B*/ tmp4_U16 = s_a;
            tmp3_U16 = (tmp4_U16 + 0x0001) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0x0001);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_18;
bb_17:
  /*$6B0B*/ tmp4_U16 = adc_dec16(s_a, 0x01, s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
bb_18:
  /*$6B0D*/ s_y = ram_peek(0x6637);
  /*$6B10*/ rom_scrn(0xfffe);
  /*$6B13*/ CYCLES(0x6b13, 7);
            branchTarget = true;
            if ((s_a != 0x00))
              goto bb_20;
bb_19:
  /*$6B17*/ CYCLES(0x6b17, 6);
            ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
bb_20:
  /*$6B1A*/ CYCLES(0x6b1a, 21);
            s_a = ram_peek(0x6638);
  /*$6B1D*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_22;
bb_21:
  /*$6B1E*/ tmp3_U16 = s_a;
            tmp4_U16 = (tmp3_U16 - 0x0001) - (uint8_t)(0x01 - s_status_c);
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0xfffe);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_23;
bb_22:
  /*$6B1E*/ tmp4_U16 = sbc_dec16(s_a, 0x01, s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
bb_23:
  /*$6B20*/ s_y = ram_peek(0x6637);
  /*$6B23*/ rom_scrn(0xfffe);
  /*$6B26*/ CYCLES(0x6b26, 7);
            branchTarget = true;
            if ((s_a != 0x00))
              goto bb_25;
bb_24:
  /*$6B2A*/ CYCLES(0x6b2a, 6);
            ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
bb_25:
  /*$6B2D*/ CYCLES(0x6b2d, 9);
            branchTarget = true;
            if (!ram_peek(0x6c4a))
              goto bb_27;
bb_26:
  /*$6B32*/ CYCLES(0x6b32, 6);
            s_status_not_z = 0x00;
            s_status_n = 0x00;
  /*$6B34*/ if (ret_addr) pop16(); return;
bb_27:
  /*$6B35*/ CYCLES(0x6b35, 6);
            s_status_not_z = 0x01;
            s_status_n = 0x00;
  /*$6B37*/ if (ret_addr) pop16(); return;
}


void func_6b3d(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6B3D*/ CYCLES(0x6b3d, 6);
            func_6c4b(0xfffe);
  /*$6B40*/ CYCLES(0x6b40, 36);
  /*$6B42*/ ram_poke(0x0000, 0x15);
  /*$6B46*/ ram_poke(0x0001, 0x02);
  /*$6B4A*/ ram_poke(0x0002, 0x00);
  /*$6B4E*/ ram_poke(0x0003, 0x01);
  /*$6B50*/ tmp1_U8 = ram_peek(0x6255);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 & 0x80))
              goto bb_2;
bb_1:
  /*$6B55*/ CYCLES(0x6b55, 12);
  /*$6B57*/ ram_poke(0x6255, 0xff);
  /*$6B5A*/ s_a = 0x70;
bb_2:
  /*$6B5C*/ CYCLES(0x6b5c, 16);
  /*$6B5E*/ ram_poke(0x0008, (s_a >> 0x02));
  /*$6B60*/ ram_poke(0x0008, (uint8_t)(ram_peek(0x0008) + 0x01));
  /*$6B62*/ func_615a(0xfffe);
  /*$6B65*/ CYCLES(0x6b65, 19);
  /*$6B67*/ ram_poke(0x0002, 0x27);
  /*$6B6B*/ ram_poke(0x0003, 0x01);
  /*$6B6D*/ func_615a(0xfffe);
  /*$6B70*/ CYCLES(0x6b70, 31);
            ram_poke(0x0008, (uint8_t)(ram_peek(0x0008) + 0x01));
  /*$6B72*/ tmp1_U8 = ram_peek(0x0008);
  /*$6B74*/ push8(tmp1_U8);
  /*$6B75*/ ram_poke(0x0003, tmp1_U8);
  /*$6B79*/ ram_poke(0x0001, 0x0d);
  /*$6B7D*/ ram_poke(0x0008, 0x27);
  /*$6B7F*/ func_615a(0xfffe);
  /*$6B82*/ CYCLES(0x6b82, 18);
            tmp1_U8 = pop8();
  /*$6B83*/ ram_poke(0x0003, tmp1_U8);
  /*$6B87*/ ram_poke(0x0002, 0x00);
  /*$6B89*/ func_615a(0xfffe);
  /*$6B8C*/ CYCLES(0x6b8c, 12);
            s_a = 0x27;
  /*$6B8E*/ s_y = 0x14;
  /*$6B90*/ rom_scrn(0x0000);
            if (ret_addr) pop16(); return;
}


void func_6b93(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;
  uint16_t tmp4_U16;
  uint16_t tmp5_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6B93*/ CYCLES(0x6b93, 6);
            func_6127(0xfffe);
  /*$6B96*/ CYCLES(0x6b96, 28);
            s_x = 0x00;
  /*$6B98*/ tmp1_U8 = ram_peek(0x0003);
  /*$6B9A*/ ram_poke(0x0007, 0x00);
  /*$6B9F*/ ram_poke(0x0004, ram_peek((0x6000 + tmp1_U8)));
  /*$6BA4*/ ram_poke(0x0005, ram_peek((0x6030 + tmp1_U8)));
bb_1:
  /*$6BA6*/ CYCLES(0x6ba6, 87);
  /*$6BA8*/ ram_poke(0x0006, ram_peek(0x0001));
  /*$6BAD*/ ram_poke(0x0006, (uint8_t)((ram_peek(0x0006) | (0x00 + ((ram_peek(0x0007) & 0x01) << 8))) >> 0x01));
  /*$6BAF*/ ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) << 0x01));
  /*$6BB1*/ ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) << 0x01));
  /*$6BB9*/ ram_poke(0x0006, ((ram_peek(0x0002) & 0x03) | ram_peek(0x0006)));
  /*$6BC2*/ tmp1_U8 = ram_peek(0x0002);
            s_y = tmp1_U8;
  /*$6BC4*/ tmp2_U8 = s_x;
  /*$6BC7*/ tmp3_U8 = peek((ram_peek16al(0x0004) + tmp1_U8));
  /*$6BC9*/ poke((ram_peek16al(0x0004) + tmp1_U8), (((ram_peek((0x6064 + ram_peek(0x0006))) ^ 0x7f) & ram_peek((0x6060 + tmp2_U8))) | tmp3_U8));
  /*$6BCB*/ s_x = (uint8_t)(tmp2_U8 + 0x01);
  /*$6BCC*/ ram_poke(0x0007, (uint8_t)(ram_peek(0x0007) + 0x01));
  /*$6BCE*/ s_a = ram_peek(0x0005);
  /*$6BD0*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_3;
bb_2:
  /*$6BD1*/ tmp4_U16 = s_a;
            tmp5_U16 = (tmp4_U16 + 0x0004) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x0004);
            s_a = ((uint8_t)tmp5_U16);
            goto bb_4;
bb_3:
  /*$6BD1*/ tmp5_U16 = adc_dec16(s_a, 0x04, s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
bb_4:
  /*$6BD3*/ ram_poke(0x0005, s_a);
  /*$6BD5*/ tmp2_U8 = s_x;
            s_status_c = (tmp2_U8 >= 0x04);
            branchTarget = true;
            if ((tmp2_U8 != 0x04))
              goto bb_1;
bb_5:
  /*$6BD9*/ CYCLES(0x6bd9, 2);
            if (ret_addr) pop16(); return;
}


void func_6bda(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6BDA*/ CYCLES(0x6bda, 6);
            func_60e4(0xfffe);
  /*$6BDD*/ CYCLES(0x6bdd, 9);
            branchTarget = true;
            if (!ram_peek(0x0305))
              goto bb_2;
bb_1:
  /*$6BE2*/ CYCLES(0x6be2, 12);
  /*$6BE4*/ ram_poke(0x0000, 0x01);
  /*$6BE6*/ func_6b93(0xfffe);
            branchTarget = true;
bb_2:
  /*$6BE9*/ CYCLES(0x6be9, 11);
            s_status_not_z = 0x00;
            s_status_n = 0x00;
  /*$6BEB*/ ram_poke(0x0305, 0x00);
  /*$6BEE*/ if (ret_addr) pop16(); return;
}


void func_6bef(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6BEF*/ CYCLES(0x6bef, 6);
            rom_plot(0xfffe);
  /*$6BF2*/ CYCLES(0x6bf2, 16);
            s_status_not_z = 0x01;
            s_status_n = 0x00;
            s_a = 0x01;
  /*$6BF4*/ ram_poke(0x0305, 0x01);
  /*$6BF7*/ ram_poke(0x6c46, 0x01);
  /*$6BFA*/ if (ret_addr) pop16(); return;
}


void func_6bfb(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6BFB*/ CYCLES(0x6bfb, 9);
  /*$6BFD*/ ram_poke(0x6c48, 0x14);
bb_1:
  /*$6C00*/ CYCLES(0x6c00, 9);
            tmp1_U8 = ram_peek(0x6c46);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_5;
bb_2:
  /*$6C05*/ CYCLES(0x6c05, 7);
            branchTarget = true;
            if ((s_a >= 0x80))
              goto bb_5;
bb_3:
  /*$6C09*/ CYCLES(0x6c09, 9);
            tmp1_U8 = (uint8_t)(ram_peek(0x6c47) - 0x01);
            ram_poke(0x6c47, tmp1_U8);
            branchTarget = true;
            if (tmp1_U8)
              goto bb_5;
bb_4:
  /*$6C0E*/ CYCLES(0x6c0e, 31);
            tmp1_U8 = ram_peek(0x6c49);
            s_y = tmp1_U8;
  /*$6C11*/ tmp1_U8 = peek((0xc000 + tmp1_U8));
  /*$6C14*/ ram_poke(0x6c46, (uint8_t)(ram_peek(0x6c46) + 0x01));
  /*$6C17*/ ram_poke(0x6c46, (uint8_t)(ram_peek(0x6c46) + 0x01));
  /*$6C1D*/ ram_poke(0x6c47, ram_peek(0x6c46));
bb_5:
  /*$6C20*/ CYCLES(0x6c20, 12);
            branchTarget = true;
            if (!(ram_peek(0x6c46) >= 0x80))
              goto bb_7;
bb_6:
  /*$6C27*/ CYCLES(0x6c27, 9);
  /*$6C29*/ ram_poke(0x6c46, 0x00);
bb_7:
  /*$6C2C*/ CYCLES(0x6c2c, 18);
  /*$6C2E*/ ram_poke(0x6c49, 0x20);
            branchTarget = true;
            if (ram_peek(0x0302))
              goto bb_10;
bb_8:
  /*$6C36*/ CYCLES(0x6c36, 9);
            branchTarget = true;
            if (ram_peek(0x69c2))
              goto bb_10;
bb_9:
  /*$6C3B*/ CYCLES(0x6c3b, 9);
  /*$6C3D*/ ram_poke(0x6c49, 0x30);
bb_10:
  /*$6C40*/ CYCLES(0x6c40, 9);
            tmp1_U8 = (uint8_t)(ram_peek(0x6c48) - 0x01);
            ram_poke(0x6c48, tmp1_U8);
            branchTarget = true;
            if (tmp1_U8)
              goto bb_1;
bb_11:
  /*$6C45*/ CYCLES(0x6c45, 2);
            if (ret_addr) pop16(); return;
}


void func_6c4b(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6C4B*/ CYCLES(0x6c4b, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x000e) + 0x01);
            ram_poke(0x000e, tmp1_U8);
            branchTarget = true;
            if (tmp1_U8)
              goto bb_2;
bb_1:
  /*$6C4F*/ CYCLES(0x6c4f, 4);
            ram_poke(0x000f, (uint8_t)(ram_peek(0x000f) + 0x01));
bb_2:
  /*$6C51*/ CYCLES(0x6c51, 11);
  /*$6C53*/ tmp1_U8 = peek(ram_peek16al(0x000e));
            tmp2_U8 = tmp1_U8 & 0x80;
            s_status_n = tmp2_U8;
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_4;
bb_3:
  /*$6C57*/ CYCLES(0x6c57, 19);
  /*$6C59*/ ram_poke(0x000e, 0x00);
  /*$6C5D*/ ram_poke(0x000f, 0x18);
            branchTarget = true;
            goto bb_2;
bb_4:
  /*$6C62*/ CYCLES(0x6c62, 2);
            if (ret_addr) pop16(); return;
}


void func_6c72(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6C72*/ CYCLES(0x6c72, 6);
            func_6594(0xfffe);
  /*$6C75*/ CYCLES(0x6c75, 11);
            push8(s_a);
            branchTarget = true;
            if (!ram_peek(0x0302))
              goto bb_7;
bb_1:
  /*$6C7B*/ CYCLES(0x6c7b, 9);
            branchTarget = true;
            if (!ram_peek(0x6c71))
              goto bb_4;
bb_2:
  /*$6C80*/ CYCLES(0x6c80, 9);
            tmp1_U8 = io_peek(0xc061);
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_4;
bb_3:
  /*$6C85*/ CYCLES(0x6c85, 7);
            tmp1_U8 = pop8();
  /*$6C86*/ s_status_n = 0x80;
            s_a = 0x92;
  /*$6C88*/ if (ret_addr) pop16(); return;
bb_4:
  /*$6C89*/ CYCLES(0x6c89, 6);
            tmp1_U8 = pop8();
            tmp2_U8 = tmp1_U8 & 0x80;
            s_status_n = tmp2_U8;
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_6;
bb_5:
  /*$6C8C*/ CYCLES(0x6c8c, 6);
            s_status_n = 0x80;
            s_a = 0x92;
  /*$6C8E*/ if (ret_addr) pop16(); return;
bb_6:
  /*$6C8F*/ CYCLES(0x6c8f, 2);
            if (ret_addr) pop16(); return;
bb_7:
  /*$6C90*/ CYCLES(0x6c90, 6);
            tmp2_U8 = pop8();
            s_a = tmp2_U8;
  /*$6C91*/ s_x = 0x05;
bb_8:
  /*$6C93*/ CYCLES(0x6c93, 9);
            branchTarget = true;
            if (!(s_a != ram_peek((0x6c63 + s_x))))
              goto bb_11;
bb_9:
  /*$6C98*/ CYCLES(0x6c98, 6);
            tmp2_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp2_U8;
            branchTarget = true;
            if (!(tmp2_U8 & 0x80))
              goto bb_8;
bb_10:
  /*$6C9B*/ CYCLES(0x6c9b, 6);
            branchTarget = true;
            goto bb_12;
bb_11:
  /*$6C9E*/ CYCLES(0x6c9e, 6);
            s_a = ram_peek((0x6c6a + s_x));
bb_12:
  /*$6CA1*/ CYCLES(0x6ca1, 7);
            branchTarget = true;
            if ((s_a != 0x80))
              goto bb_14;
bb_13:
  /*$6CA5*/ CYCLES(0x6ca5, 11);
            s_status_n = 0x00;
            s_a = 0x01;
  /*$6CA7*/ ram_poke(0x6c71, 0x01);
  /*$6CAA*/ if (ret_addr) pop16(); return;
bb_14:
  /*$6CAB*/ CYCLES(0x6cab, 7);
            branchTarget = true;
            if ((s_a != 0x8b))
              goto bb_16;
bb_15:
  /*$6CAF*/ CYCLES(0x6caf, 11);
            s_status_n = 0x00;
            s_a = 0x00;
  /*$6CB1*/ ram_poke(0x6c71, 0x00);
  /*$6CB4*/ if (ret_addr) pop16(); return;
bb_16:
  /*$6CB5*/ CYCLES(0x6cb5, 7);
            tmp2_U8 = s_a & 0x80;
            s_status_n = tmp2_U8;
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_18;
bb_17:
  /*$6CB9*/ CYCLES(0x6cb9, 2);
            if (ret_addr) pop16(); return;
bb_18:
  /*$6CBA*/ CYCLES(0x6cba, 9);
            tmp2_U8 = ram_peek(0x6c71);
            s_x = tmp2_U8;
            branchTarget = true;
            if (tmp2_U8)
              goto bb_20;
bb_19:
  /*$6CBF*/ CYCLES(0x6cbf, 6);
            s_status_n = (s_a & 0x80);
  /*$6CC1*/ if (ret_addr) pop16(); return;
bb_20:
  /*$6CC2*/ CYCLES(0x6cc2, 18);
            s_y = 0x00;
  /*$6CC4*/ tmp2_U8 = io_peek(0xc05b);
  /*$6CC7*/ tmp2_U8 = io_peek(0xc062);
            branchTarget = true;
            if ((tmp2_U8 & 0x80))
              goto bb_22;
bb_21:
  /*$6CCC*/ CYCLES(0x6ccc, 6);
            s_x = 0x00;
  /*$6CCE*/ s_y = (uint8_t)(s_y + 0x01);
bb_22:
  /*$6CCF*/ CYCLES(0x6ccf, 9);
            tmp2_U8 = io_peek(0xc063);
            branchTarget = true;
            if ((tmp2_U8 & 0x80))
              goto bb_24;
bb_23:
  /*$6CD4*/ CYCLES(0x6cd4, 6);
            s_x = 0x03;
  /*$6CD6*/ s_y = (uint8_t)(s_y + 0x01);
bb_24:
  /*$6CD7*/ CYCLES(0x6cd7, 14);
            tmp2_U8 = io_peek(0xc05a);
  /*$6CDA*/ tmp2_U8 = io_peek(0xc062);
            branchTarget = true;
            if ((tmp2_U8 & 0x80))
              goto bb_26;
bb_25:
  /*$6CDF*/ CYCLES(0x6cdf, 6);
            s_x = 0x01;
  /*$6CE1*/ s_y = (uint8_t)(s_y + 0x01);
bb_26:
  /*$6CE2*/ CYCLES(0x6ce2, 9);
            tmp2_U8 = io_peek(0xc063);
            branchTarget = true;
            if ((tmp2_U8 & 0x80))
              goto bb_28;
bb_27:
  /*$6CE7*/ CYCLES(0x6ce7, 6);
            s_x = 0x02;
  /*$6CE9*/ s_y = (uint8_t)(s_y + 0x01);
bb_28:
  /*$6CEA*/ CYCLES(0x6cea, 7);
            branchTarget = true;
            if (!(s_y != 0x01))
              goto bb_30;
bb_29:
  /*$6CEE*/ CYCLES(0x6cee, 6);
            s_status_n = 0x00;
            s_a = 0x00;
  /*$6CF0*/ if (ret_addr) pop16(); return;
bb_30:
  /*$6CF1*/ CYCLES(0x6cf1, 7);
            tmp2_U8 = ram_peek((0x6c6a + s_x));
            s_status_n = (tmp2_U8 & 0x80);
            s_a = tmp2_U8;
  /*$6CF4*/ if (ret_addr) pop16(); return;
}


void func_7000(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$7000*/ CYCLES(0x7000, 6);
  /*$7002*/ push8(ram_peek(0x0003));
bb_1:
  /*$7003*/ CYCLES(0x7003, 12);
            s_a = ram_peek(0x0003);
  /*$7005*/ s_y = ram_peek(0x0002);
  /*$7007*/ rom_plot(0xfffe);
  /*$700A*/ CYCLES(0x700a, 11);
            tmp1_U8 = ram_peek(0x0003);
  /*$700C*/ tmp2_U8 = ram_peek(0x0008);
            s_status_c = (tmp1_U8 >= tmp2_U8);
            branchTarget = true;
            if (!(tmp1_U8 != tmp2_U8))
              goto bb_3;
bb_2:
  /*$7010*/ CYCLES(0x7010, 9);
            ram_poke(0x0003, (uint8_t)(ram_peek(0x0003) + 0x01));
            branchTarget = true;
            goto bb_1;
bb_3:
  /*$7015*/ CYCLES(0x7015, 7);
            tmp2_U8 = pop8();
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
  /*$7016*/ ram_poke(0x0003, tmp2_U8);
  /*$7018*/ if (ret_addr) pop16(); return;
}


void func_7019(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$7019*/ CYCLES(0x7019, 14);
            s_y = 0x00;
  /*$701B*/ tmp1_U8 = peek(ram_peek16al(0x000a));
            s_a = tmp1_U8;
  /*$701D*/ tmp1_U8 = (uint8_t)(ram_peek(0x000a) + 0x01);
            s_status_n = (tmp1_U8 & 0x80);
            ram_poke(0x000a, tmp1_U8);
            branchTarget = true;
            if (tmp1_U8)
              goto bb_2;
bb_1:
  /*$7021*/ CYCLES(0x7021, 4);
            tmp1_U8 = (uint8_t)(ram_peek(0x000b) + 0x01);
            s_status_n = (tmp1_U8 & 0x80);
            ram_poke(0x000b, tmp1_U8);
bb_2:
  /*$7023*/ CYCLES(0x7023, 2);
            if (ret_addr) pop16(); return;
}


void func_7024(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$7024*/ CYCLES(0x7024, 4);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_2;
bb_1:
  /*$7026*/ CYCLES(0x7026, 4);
            s_a = 0x05;
bb_2:
  /*$7028*/ CYCLES(0x7028, 6);
            rom_setcol(0x0000);
            if (ret_addr) pop16(); return;
}


void func_702b(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$702B*/ CYCLES(0x702b, 18);
  /*$702D*/ ram_poke(0x0004, 0x00);
  /*$7031*/ ram_poke(0x0005, 0x20);
  /*$7033*/ s_y = 0x00;
bb_1:
  /*$7035*/ CYCLES(0x7035, 12);
  /*$7037*/ tmp1_U8 = s_y;
            poke((ram_peek16al(0x0004) + tmp1_U8), 0x00);
  /*$7039*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_1;
bb_2:
  /*$703C*/ CYCLES(0x703c, 14);
            ram_poke(0x0005, (uint8_t)(ram_peek(0x0005) + 0x01));
  /*$703E*/ tmp1_U8 = ram_peek(0x0005);
  /*$7040*/ s_status_c = (tmp1_U8 >= 0x40);
            s_status_n = ((uint8_t)(tmp1_U8 - 0x40) & 0x80);
            branchTarget = true;
            if ((tmp1_U8 != 0x40))
              goto bb_1;
bb_3:
  /*$7044*/ CYCLES(0x7044, 2);
            if (ret_addr) pop16(); return;
}


void func_7045(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$7045*/ CYCLES(0x7045, 6);
            func_702b(0xfffe);
  /*$7048*/ CYCLES(0x7048, 43);
  /*$704A*/ ram_poke(0x6c46, 0x00);
  /*$704D*/ tmp1_U8 = io_peek(0xc050);
  /*$7050*/ tmp1_U8 = io_peek(0xc057);
  /*$7053*/ tmp1_U8 = io_peek(0xc055);
  /*$7056*/ tmp1_U8 = io_peek(0xc052);
  /*$705B*/ ram_poke(0x0002, 0x04);
  /*$705F*/ ram_poke(0x0003, 0x00);
bb_1:
  /*$7061*/ CYCLES(0x7061, 6);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_1;
bb_2:
  /*$7064*/ CYCLES(0x7064, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x0003) - 0x01);
            ram_poke(0x0003, tmp1_U8);
            branchTarget = true;
            if (tmp1_U8)
              goto bb_1;
bb_3:
  /*$7068*/ CYCLES(0x7068, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x0002) - 0x01);
            ram_poke(0x0002, tmp1_U8);
            branchTarget = true;
            if (tmp1_U8)
              goto bb_1;
bb_4:
  /*$706C*/ CYCLES(0x706c, 16);
  /*$706E*/ ram_poke(0x0003, 0x27);
  /*$7070*/ s_status_not_z = 0x00;
            s_a = 0x00;
  /*$7072*/ func_7024(0xfffe);
            branchTarget = true;
bb_5:
  /*$7075*/ CYCLES(0x7075, 19);
  /*$7077*/ ram_poke(0x002c, 0x27);
  /*$7079*/ s_y = 0x00;
  /*$707B*/ s_a = ram_peek(0x0003);
  /*$707D*/ rom_hline(0xfffe);
  /*$7080*/ CYCLES(0x7080, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x0003) - 0x01);
            ram_poke(0x0003, tmp1_U8);
            branchTarget = true;
            if (!(tmp1_U8 & 0x80))
              goto bb_5;
bb_6:
  /*$7084*/ CYCLES(0x7084, 26);
  /*$7086*/ ram_poke(0x0022, 0x14);
  /*$708A*/ ram_poke(0x0000, 0x15);
  /*$708C*/ s_status_not_z = 0x0d;
            s_a = 0x0d;
  /*$708E*/ ram_poke(0x0001, 0x0d);
  /*$7090*/ func_7024(0xfffe);
  /*$7093*/ CYCLES(0x7093, 9);
            branchTarget = true;
            if (!ram_peek(0x0301))
              goto bb_9;
bb_7:
  /*$7098*/ CYCLES(0x7098, 11);
            s_a = 0x01;
  /*$709A*/ s_y = 0x01;
  /*$709B*/ rom_plot(0xfffe);
  /*$709E*/ CYCLES(0x709e, 12);
            tmp1_U8 = ram_peek(0x0301);
  /*$70A1*/ s_status_c = (tmp1_U8 >= 0x01);
            branchTarget = true;
            if (!(tmp1_U8 != 0x01))
              goto bb_9;
bb_8:
  /*$70A5*/ CYCLES(0x70a5, 12);
            s_a = 0x01;
  /*$70A7*/ s_y = 0x26;
  /*$70A9*/ rom_plot(0xfffe);
            branchTarget = true;
bb_9:
  /*$70AC*/ CYCLES(0x70ac, 12);
            s_y = 0x00;
  /*$70AE*/ s_a = 0x00;
  /*$70B0*/ rom_hline(0xfffe);
  /*$70B3*/ CYCLES(0x70b3, 12);
            s_y = 0x00;
  /*$70B5*/ s_a = 0x27;
  /*$70B7*/ rom_hline(0xfffe);
  /*$70BA*/ CYCLES(0x70ba, 26);
  /*$70BC*/ ram_poke(0x0002, 0x00);
  /*$70C0*/ ram_poke(0x0003, 0x00);
  /*$70C4*/ ram_poke(0x0008, 0x27);
  /*$70C6*/ func_7000(0xfffe);
  /*$70C9*/ CYCLES(0x70c9, 19);
  /*$70CB*/ ram_poke(0x0003, 0x00);
  /*$70CF*/ ram_poke(0x0002, 0x27);
  /*$70D1*/ func_7000(0xfffe);
  /*$70D4*/ CYCLES(0x70d4, 23);
  /*$70D6*/ ram_poke(0x0002, 0x00);
  /*$70D8*/ ram_poke(0x0003, 0x00);
  /*$70DC*/ ram_poke(0x0008, 0x27);
  /*$70DE*/ func_6148(0xfffe);
  /*$70E1*/ CYCLES(0x70e1, 19);
  /*$70E3*/ ram_poke(0x0002, 0x00);
  /*$70E7*/ ram_poke(0x0003, 0x27);
  /*$70E9*/ func_6148(0xfffe);
  /*$70EC*/ CYCLES(0x70ec, 16);
  /*$70EE*/ ram_poke(0x0002, 0x00);
  /*$70F0*/ ram_poke(0x0003, 0x00);
  /*$70F2*/ func_615a(0xfffe);
  /*$70F5*/ CYCLES(0x70f5, 19);
  /*$70F7*/ ram_poke(0x0003, 0x00);
  /*$70FB*/ ram_poke(0x0002, 0x27);
  /*$70FD*/ func_615a(0xfffe);
  /*$7100*/ CYCLES(0x7100, 33);
  /*$7102*/ ram_poke(0x0001, 0x03);
  /*$7106*/ ram_poke(0x0003, 0x27);
  /*$710A*/ ram_poke(0x0002, 0x12);
  /*$710E*/ ram_poke(0x0008, 0x16);
  /*$7110*/ func_6148(0xfffe);
            branchTarget = true;
bb_10:
  /*$7113*/ CYCLES(0x7113, 19);
            s_x = ram_peek(0x0303);
  /*$7118*/ ram_poke(0x000a, 0x00);
  /*$711C*/ ram_poke(0x000b, 0x80);
bb_11:
  /*$711E*/ CYCLES(0x711e, 6);
            tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_14;
bb_12:
  /*$7121*/ CYCLES(0x7121, 6);
            func_7019(0xfffe);
  /*$7124*/ CYCLES(0x7124, 7);
            branchTarget = true;
            if (!(s_a != 0x2a))
              goto bb_11;
bb_13:
  /*$7128*/ CYCLES(0x7128, 6);
            branchTarget = true;
            goto bb_12;
bb_14:
  /*$712B*/ CYCLES(0x712b, 6);
            func_7019(0xfffe);
  /*$712E*/ CYCLES(0x712e, 7);
            branchTarget = true;
            if ((s_a != 0x45))
              goto bb_16;
bb_15:
  /*$7132*/ CYCLES(0x7132, 14);
  /*$7134*/ ram_poke(0x0303, 0x01);
            branchTarget = true;
            goto bb_10;
bb_16:
  /*$713A*/ CYCLES(0x713a, 7);
            tmp1_U8 = s_a;
            s_status_c = (tmp1_U8 >= 0x48);
            branchTarget = true;
            if ((tmp1_U8 != 0x48))
              goto bb_18;
bb_17:
  /*$713E*/ CYCLES(0x713e, 6);
            func_7019(0xfffe);
  /*$7141*/ CYCLES(0x7141, 9);
            ram_poke(0x0001, s_a);
  /*$7143*/ func_7019(0xfffe);
  /*$7146*/ CYCLES(0x7146, 9);
            ram_poke(0x0002, s_a);
  /*$7148*/ func_7019(0xfffe);
  /*$714B*/ CYCLES(0x714b, 9);
            ram_poke(0x0008, s_a);
  /*$714D*/ func_7019(0xfffe);
  /*$7150*/ CYCLES(0x7150, 12);
            ram_poke(0x0003, s_a);
  /*$7152*/ tmp1_U8 = ram_peek(0x0001);
            s_status_not_z = tmp1_U8;
            s_a = tmp1_U8;
  /*$7154*/ func_7024(0xfffe);
  /*$7157*/ CYCLES(0x7157, 19);
  /*$7159*/ ram_poke(0x002c, ram_peek(0x0008));
  /*$715B*/ s_a = ram_peek(0x0003);
  /*$715D*/ s_y = ram_peek(0x0002);
  /*$715F*/ rom_hline(0xfffe);
  /*$7162*/ CYCLES(0x7162, 6);
            func_6148(0xfffe);
  /*$7165*/ CYCLES(0x7165, 6);
            branchTarget = true;
            goto bb_14;
bb_18:
  /*$7168*/ CYCLES(0x7168, 7);
            tmp1_U8 = s_a;
            s_status_c = (tmp1_U8 >= 0x56);
            branchTarget = true;
            if ((tmp1_U8 != 0x56))
              goto bb_20;
bb_19:
  /*$716C*/ CYCLES(0x716c, 6);
            func_7019(0xfffe);
  /*$716F*/ CYCLES(0x716f, 9);
            ram_poke(0x0001, s_a);
  /*$7171*/ func_7019(0xfffe);
  /*$7174*/ CYCLES(0x7174, 9);
            ram_poke(0x0003, s_a);
  /*$7176*/ func_7019(0xfffe);
  /*$7179*/ CYCLES(0x7179, 9);
            ram_poke(0x0008, s_a);
  /*$717B*/ func_7019(0xfffe);
  /*$717E*/ CYCLES(0x717e, 12);
            ram_poke(0x0002, s_a);
  /*$7180*/ tmp1_U8 = ram_peek(0x0001);
            s_status_not_z = tmp1_U8;
            s_a = tmp1_U8;
  /*$7182*/ func_7024(0xfffe);
  /*$7185*/ CYCLES(0x7185, 6);
            func_7000(0xfffe);
  /*$7188*/ CYCLES(0x7188, 6);
            func_615a(0xfffe);
  /*$718B*/ CYCLES(0x718b, 6);
            branchTarget = true;
            goto bb_14;
bb_20:
  /*$718E*/ CYCLES(0x718e, 7);
            tmp1_U8 = s_a;
            s_status_c = (tmp1_U8 >= 0x50);
            branchTarget = true;
            if ((tmp1_U8 != 0x50))
              goto bb_22;
bb_21:
  /*$7192*/ CYCLES(0x7192, 6);
            func_7019(0xfffe);
  /*$7195*/ CYCLES(0x7195, 9);
            ram_poke(0x0001, s_a);
  /*$7197*/ func_7019(0xfffe);
  /*$719A*/ CYCLES(0x719a, 9);
            ram_poke(0x0002, s_a);
  /*$719C*/ func_7019(0xfffe);
  /*$719F*/ CYCLES(0x719f, 12);
            ram_poke(0x0003, s_a);
  /*$71A1*/ tmp1_U8 = ram_peek(0x0001);
            s_status_not_z = tmp1_U8;
            s_a = tmp1_U8;
  /*$71A3*/ func_7024(0xfffe);
  /*$71A6*/ CYCLES(0x71a6, 12);
            s_a = ram_peek(0x0003);
  /*$71A8*/ s_y = ram_peek(0x0002);
  /*$71AA*/ rom_plot(0xfffe);
  /*$71AD*/ CYCLES(0x71ad, 6);
            func_60e4(0xfffe);
  /*$71B0*/ CYCLES(0x71b0, 6);
            branchTarget = true;
            goto bb_14;
bb_22:
  /*$71B3*/ CYCLES(0x71b3, 7);
            branchTarget = true;
            if ((s_a != 0x54))
              goto bb_24;
bb_23:
  /*$71B7*/ CYCLES(0x71b7, 6);
            func_7019(0xfffe);
  /*$71BA*/ CYCLES(0x71ba, 11);
            ram_poke(0x0304, s_a);
            branchTarget = true;
            goto bb_14;
bb_24:
  /*$71C0*/ CYCLES(0x71c0, 7);
            branchTarget = true;
            if (!(s_a != 0x2a))
              goto bb_26;
bb_25:
  /*$71C4*/ CYCLES(0x71c4, 6);
            branchTarget = true;
            goto bb_14;
bb_26:
  /*$71C7*/ CYCLES(0x71c7, 2);
            if (ret_addr) pop16(); return;
}


void func_71cd(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint16_t tmp2_U16;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$71CD*/ CYCLES(0x71cd, 26);
  /*$71CF*/ ram_poke(0x71cb, 0x00);
  /*$71D2*/ ram_poke(0x71cc, 0x00);
  /*$71D5*/ s_x = ram_peek(0x0301);
  /*$71D8*/ s_y = ram_peek(0x0303);
  /*$71DB*/ s_status_d = 0x01;
bb_1:
  /*$71DC*/ CYCLES(0x71dc, 36);
            s_a = ram_peek((0x71c8 + s_x));
  /*$71DF*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_3;
bb_2:
  /*$71E0*/ tmp1_U16 = (s_a + ram_peek(0x71cb)) + s_status_c;
            s_status_c = (uint8_t)(tmp1_U16 >> 8);
            s_a = ((uint8_t)tmp1_U16);
            goto bb_4;
bb_3:
  /*$71E0*/ tmp2_U16 = adc_dec16(s_a, ram_peek(0x71cb), s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            s_status_c = ((uint8_t)(tmp2_U16 >> 8) & 0x01);
bb_4:
  /*$71E3*/ ram_poke(0x71cb, s_a);
  /*$71E6*/ s_a = ram_peek(0x71cc);
            if (s_status_d)
              goto bb_6;
bb_5:
  /*$71E9*/ tmp1_U16 = s_a;
            tmp2_U16 = tmp1_U16 + s_status_c;
            s_status_c = (uint8_t)(tmp2_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp2_U16, (uint8_t)tmp1_U16, (uint8_t)0x0000);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_7;
bb_6:
  /*$71E9*/ tmp2_U16 = adc_dec16(s_a, 0x00, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            tmp3_U8 = (uint8_t)(tmp2_U16 >> 8);
            s_status_c = (tmp3_U8 & 0x01);
            s_status_v = ((tmp3_U8 & 0x40) != 0);
bb_7:
  /*$71EB*/ ram_poke(0x71cc, s_a);
  /*$71EE*/ tmp3_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp3_U8;
            branchTarget = true;
            if (tmp3_U8)
              goto bb_1;
bb_8:
  /*$71F1*/ CYCLES(0x71f1, 4);
            s_status_d = 0x00;
  /*$71F2*/ if (ret_addr) pop16(); return;
}


void func_71f3(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$71F3*/ CYCLES(0x71f3, 16);
            tmp1_U8 = s_a;
            push8(tmp1_U8);
  /*$71F7*/ tmp1_U8 = tmp1_U8 >> 0x04;
            s_a = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 != 0x00))
              goto bb_2;
bb_1:
  /*$71FC*/ CYCLES(0x71fc, 4);
            ram_poke(0x002c, s_a);
bb_2:
  /*$71FE*/ CYCLES(0x71fe, 9);
            push8(s_a);
            branchTarget = true;
            if (ram_peek(0x002c))
              goto bb_4;
bb_3:
  /*$7203*/ CYCLES(0x7203, 7);
            tmp1_U8 = pop8();
            branchTarget = true;
            goto bb_8;
bb_4:
  /*$7207*/ CYCLES(0x7207, 12);
            tmp1_U8 = pop8();
            s_a = tmp1_U8;
  /*$7208*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_6;
bb_5:
  /*$7209*/ tmp3_U16 = s_a;
            tmp2_U16 = (tmp3_U16 + 0x00b0) + s_status_c;
            s_status_c = (uint8_t)(tmp2_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp2_U16, (uint8_t)tmp3_U16, (uint8_t)0x00b0);
            tmp1_U8 = (uint8_t)tmp2_U16;
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            goto bb_7;
bb_6:
  /*$7209*/ tmp2_U16 = adc_dec16(s_a, 0xb0, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            tmp1_U8 = (uint8_t)(tmp2_U16 >> 8);
            s_status_c = (tmp1_U8 & 0x01);
            s_status_not_z = (~tmp1_U8 & 2);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
            s_status_n = (tmp1_U8 & 0x80);
bb_7:
  /*$720B*/ rom_cout(0xfffe);
            branchTarget = true;
bb_8:
  /*$720E*/ CYCLES(0x720e, 12);
            tmp1_U8 = pop8();
  /*$720F*/ tmp1_U8 = tmp1_U8 & 0x0f;
            s_a = tmp1_U8;
  /*$7211*/ s_status_c = (tmp1_U8 >= 0x00);
            branchTarget = true;
            if (!(tmp1_U8 != 0x00))
              goto bb_10;
bb_9:
  /*$7215*/ CYCLES(0x7215, 4);
            ram_poke(0x002c, s_a);
bb_10:
  /*$7217*/ CYCLES(0x7217, 9);
            push8(s_a);
            branchTarget = true;
            if (ram_peek(0x002c))
              goto bb_12;
bb_11:
  /*$721C*/ CYCLES(0x721c, 4);
            tmp1_U8 = pop8();
  /*$721D*/ if (ret_addr) pop16(); return;
bb_12:
  /*$721E*/ CYCLES(0x721e, 12);
            tmp1_U8 = pop8();
            s_a = tmp1_U8;
  /*$721F*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_14;
bb_13:
  /*$7220*/ tmp2_U16 = s_a;
            tmp3_U16 = (tmp2_U16 + 0x00b0) + s_status_c;
            s_status_c = (uint8_t)(tmp3_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0x00b0);
            tmp1_U8 = (uint8_t)tmp3_U16;
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            goto bb_15;
bb_14:
  /*$7220*/ tmp3_U16 = adc_dec16(s_a, 0xb0, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
            s_status_c = (tmp1_U8 & 0x01);
            s_status_not_z = (~tmp1_U8 & 2);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
            s_status_n = (tmp1_U8 & 0x80);
bb_15:
  /*$7222*/ rom_cout(0xfffe);
  /*$7225*/ CYCLES(0x7225, 2);
            if (ret_addr) pop16(); return;
}


void func_7226(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$7226*/ CYCLES(0x7226, 7);
            tmp1_U8 = ram_peek(0x002c);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_2;
bb_1:
  /*$722A*/ CYCLES(0x722a, 9);
            s_status_not_z = 0xb0;
            s_status_n = 0x80;
            s_a = 0xb0;
  /*$722C*/ rom_cout(0x0000);
            if (ret_addr) pop16(); return;
bb_2:
  /*$722F*/ CYCLES(0x722f, 2);
            if (ret_addr) pop16(); return;
}


void func_7267(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint16_t tmp2_U16;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$7267*/ CYCLES(0x7267, 65);
            s_status_d = 0x01;
  /*$726C*/ tmp1_U16 = adc_dec16(ram_peek(0x71cb), ram_peek(0x7252), 0x00);
            s_status_c = ((uint8_t)(tmp1_U16 >> 8) & 0x01);
  /*$726F*/ ram_poke(0x7252, ((uint8_t)tmp1_U16));
  /*$7272*/ s_a = ram_peek(0x71cc);
            if (0x01)
              goto bb_2;
bb_1:
  /*$7275*/ tmp1_U16 = (s_a + ram_peek(0x7253)) + s_status_c;
            s_status_c = (uint8_t)(tmp1_U16 >> 8);
            s_a = ((uint8_t)tmp1_U16);
            goto bb_3;
bb_2:
  /*$7275*/ tmp2_U16 = adc_dec16(s_a, ram_peek(0x7253), s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            s_status_c = ((uint8_t)(tmp2_U16 >> 8) & 0x01);
bb_3:
  /*$7278*/ ram_poke(0x7253, s_a);
  /*$727B*/ s_a = ram_peek(0x7254);
            if (s_status_d)
              goto bb_5;
bb_4:
  /*$727E*/ tmp1_U16 = s_a + s_status_c;
            s_status_c = (uint8_t)(tmp1_U16 >> 8);
            s_a = ((uint8_t)tmp1_U16);
            goto bb_6;
bb_5:
  /*$727E*/ tmp2_U16 = adc_dec16(s_a, 0x00, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            s_status_c = ((uint8_t)(tmp2_U16 >> 8) & 0x01);
bb_6:
  /*$7280*/ ram_poke(0x7254, s_a);
  /*$7283*/ s_a = ram_peek(0x7255);
            if (s_status_d)
              goto bb_8;
bb_7:
  /*$7286*/ tmp1_U16 = s_a;
            tmp2_U16 = tmp1_U16 + s_status_c;
            s_status_c = (uint8_t)(tmp2_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp2_U16, (uint8_t)tmp1_U16, (uint8_t)0x0000);
            tmp3_U8 = (uint8_t)tmp2_U16;
            s_status_not_z = tmp3_U8;
            s_status_n = (tmp3_U8 & 0x80);
            s_a = tmp3_U8;
            goto bb_9;
bb_8:
  /*$7286*/ tmp2_U16 = adc_dec16(s_a, 0x00, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            tmp3_U8 = (uint8_t)(tmp2_U16 >> 8);
            s_status_c = (tmp3_U8 & 0x01);
            s_status_not_z = (~tmp3_U8 & 2);
            s_status_v = ((tmp3_U8 & 0x40) != 0);
            s_status_n = (tmp3_U8 & 0x80);
bb_9:
  /*$7288*/ ram_poke(0x7255, s_a);
  /*$728B*/ s_status_d = 0x00;
  /*$728C*/ if (ret_addr) pop16(); return;
}


void func_728d(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$728D*/ CYCLES(0x728d, 14);
            tmp1_U8 = ram_peek(0x7255);
  /*$7290*/ tmp2_U8 = ram_peek(0x7259);
            s_status_not_z = (tmp1_U8 != tmp2_U8);
            tmp3_U8 = tmp1_U8 >= tmp2_U8;
            s_status_c = tmp3_U8;
            s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
            branchTarget = true;
            if (!tmp3_U8)
              goto bb_9;
bb_1:
  /*$7295*/ CYCLES(0x7295, 4);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_8;
bb_2:
  /*$7297*/ CYCLES(0x7297, 14);
            tmp3_U8 = ram_peek(0x7254);
  /*$729A*/ tmp2_U8 = ram_peek(0x7258);
            s_status_not_z = (tmp3_U8 != tmp2_U8);
            tmp1_U8 = tmp3_U8 >= tmp2_U8;
            s_status_c = tmp1_U8;
            s_status_n = ((uint8_t)(tmp3_U8 - tmp2_U8) & 0x80);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_9;
bb_3:
  /*$729F*/ CYCLES(0x729f, 4);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_8;
bb_4:
  /*$72A1*/ CYCLES(0x72a1, 14);
            tmp1_U8 = ram_peek(0x7253);
  /*$72A4*/ tmp2_U8 = ram_peek(0x7257);
            s_status_not_z = (tmp1_U8 != tmp2_U8);
            tmp3_U8 = tmp1_U8 >= tmp2_U8;
            s_status_c = tmp3_U8;
            s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
            branchTarget = true;
            if (!tmp3_U8)
              goto bb_9;
bb_5:
  /*$72A9*/ CYCLES(0x72a9, 4);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_8;
bb_6:
  /*$72AB*/ CYCLES(0x72ab, 14);
            tmp3_U8 = ram_peek(0x7252);
  /*$72AE*/ tmp2_U8 = ram_peek(0x7256);
            s_status_not_z = (tmp3_U8 != tmp2_U8);
            tmp1_U8 = tmp3_U8 >= tmp2_U8;
            s_status_c = tmp1_U8;
            s_status_n = ((uint8_t)(tmp3_U8 - tmp2_U8) & 0x80);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_9;
bb_7:
  /*$72B3*/ CYCLES(0x72b3, 4);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_8;
bb_8:
  /*$72B5*/ CYCLES(0x72b5, 41);
  /*$72B8*/ ram_poke(0x7256, ram_peek(0x7252));
  /*$72BE*/ ram_poke(0x7257, ram_peek(0x7253));
  /*$72C4*/ ram_poke(0x7258, ram_peek(0x7254));
  /*$72C7*/ tmp1_U8 = ram_peek(0x7255);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
  /*$72CA*/ ram_poke(0x7259, tmp1_U8);
bb_9:
  /*$72CD*/ CYCLES(0x72cd, 2);
            if (ret_addr) pop16(); return;
}


void func_7590(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$7590*/ CYCLES(0x7590, 11);
            ram_poke(0x0002, s_x);
            branchTarget = true;
            if ((s_a != 0x88))
              goto bb_2;
bb_1:
  /*$7596*/ CYCLES(0x7596, 4);
            s_a = 0xe6;
bb_2:
  /*$7598*/ CYCLES(0x7598, 7);
            tmp1_U8 = s_a;
            s_status_c = (tmp1_U8 >= 0x95);
            branchTarget = true;
            if ((tmp1_U8 != 0x95))
              goto bb_4;
bb_3:
  /*$759C*/ CYCLES(0x759c, 4);
            s_a = 0xe7;
bb_4:
  /*$759E*/ CYCLES(0x759e, 24);
            push8(s_a);
  /*$759F*/ tmp1_U8 = s_x;
  /*$75A2*/ ram_poke(0x0024, ram_peek((0x75b3 + tmp1_U8)));
  /*$75A7*/ ram_poke(0x0025, ram_peek((0x75b9 + tmp1_U8)));
  /*$75A9*/ rom_fc68(0xfffe);
  /*$75AC*/ CYCLES(0x75ac, 7);
            tmp1_U8 = pop8();
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
  /*$75AD*/ rom_cout(0xfffe);
  /*$75B0*/ CYCLES(0x75b0, 6);
            tmp1_U8 = ram_peek(0x0002);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_x = tmp1_U8;
  /*$75B2*/ if (ret_addr) pop16(); return;
}


void func_75d1(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$75D1*/ CYCLES(0x75d1, 26);
            tmp1_U8 = s_x;
            ram_poke(0x0002, tmp1_U8);
  /*$75D6*/ ram_poke(0x0024, ram_peek((0x75bf + tmp1_U8)));
  /*$75DB*/ ram_poke(0x0025, ram_peek((0x75c5 + tmp1_U8)));
  /*$75DD*/ rom_fc68(0xfffe);
  /*$75E0*/ CYCLES(0x75e0, 12);
            s_x = ram_peek(0x0002);
  /*$75E2*/ s_status_not_z = 0xa0;
            s_status_n = 0x80;
            s_a = 0xa0;
  /*$75E4*/ rom_cout(0xfffe);
  /*$75E7*/ CYCLES(0x75e7, 4);
            s_y = 0x00;
bb_1:
  /*$75E9*/ CYCLES(0x75e9, 6);
            tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_1;
bb_2:
  /*$75EC*/ CYCLES(0x75ec, 7);
  /*$75F0*/ CYCLES(0x75f0, 6);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_1;
bb_3:
  /*$75F3*/ CYCLES(0x75f3, 26);
            tmp1_U8 = ram_peek(0x0002);
            s_x = tmp1_U8;
  /*$75F8*/ ram_poke(0x0024, ram_peek((0x75bf + tmp1_U8)));
  /*$75FD*/ ram_poke(0x0025, ram_peek((0x75c5 + tmp1_U8)));
  /*$75FF*/ rom_fc68(0xfffe);
  /*$7602*/ CYCLES(0x7602, 14);
            tmp1_U8 = ram_peek(0x0002);
            s_x = tmp1_U8;
  /*$7604*/ tmp1_U8 = ram_peek((0x75cb + tmp1_U8));
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
  /*$7607*/ rom_cout(0xfffe);
  /*$760A*/ CYCLES(0x760a, 4);
            s_y = 0x00;
bb_4:
  /*$760C*/ CYCLES(0x760c, 6);
            tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_4;
bb_5:
  /*$760F*/ CYCLES(0x760f, 9);
            tmp1_U8 = io_peek(0xc000);
            s_a = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_8;
bb_6:
  /*$7614*/ CYCLES(0x7614, 6);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_4;
bb_7:
  /*$7617*/ CYCLES(0x7617, 9);
            s_x = ram_peek(0x0002);
            branchTarget = true;
            goto bb_0;
bb_8:
  /*$761C*/ CYCLES(0x761c, 12);
            tmp1_U8 = s_a;
            io_poke(0xc010, tmp1_U8);
            branchTarget = true;
            if ((tmp1_U8 >= 0xa1))
              goto bb_12;
bb_9:
  /*$7623*/ CYCLES(0x7623, 7);
            branchTarget = true;
            if (!(s_a != 0x88))
              goto bb_12;
bb_10:
  /*$7627*/ CYCLES(0x7627, 7);
            tmp1_U8 = s_a;
            s_status_c = (tmp1_U8 >= 0x95);
            branchTarget = true;
            if (!(tmp1_U8 != 0x95))
              goto bb_12;
bb_11:
  /*$762B*/ CYCLES(0x762b, 9);
            s_x = ram_peek(0x0002);
            branchTarget = true;
            goto bb_0;
bb_12:
  /*$7630*/ CYCLES(0x7630, 6);
            tmp1_U8 = ram_peek(0x0002);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_x = tmp1_U8;
  /*$7632*/ if (ret_addr) pop16(); return;
}


void func_7633(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$7633*/ CYCLES(0x7633, 24);
  /*$7638*/ tmp1_U16 = adc_dec16(ram_peek(0x725e), 0x01, 0x00);
            tmp2_U8 = (uint8_t)(tmp1_U16 >> 8);
            s_status_c = (tmp2_U8 & 0x01);
            s_status_v = ((tmp2_U8 & 0x40) != 0);
  /*$763A*/ ram_poke(0x725e, ((uint8_t)tmp1_U16));
  /*$763D*/ s_status_d = 0x00;
  /*$763E*/ func_64a9(0xfffe);
  /*$7641*/ CYCLES(0x7641, 2);
            if (ret_addr) pop16(); return;
}


void func_7642(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$7642*/ CYCLES(0x7642, 6);
            func_6c4b(0xfffe);
  /*$7645*/ CYCLES(0x7645, 9);
            ram_poke(0x0002, s_a);
  /*$7647*/ func_6c4b(0xfffe);
  /*$764A*/ CYCLES(0x764a, 16);
            ram_poke(0x0003, s_a);
  /*$764C*/ s_a = ram_peek(0x0003);
  /*$764E*/ tmp1_U8 = ram_peek(0x0002);
            s_status_not_z = tmp1_U8;
            s_y = tmp1_U8;
  /*$7650*/ rom_scrn(0xfffe);
  /*$7653*/ CYCLES(0x7653, 4);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_0;
bb_1:
  /*$7655*/ CYCLES(0x7655, 9);
            s_a = 0x0f;
  /*$7657*/ rom_setcol(0xfffe);
  /*$765A*/ CYCLES(0x765a, 12);
            s_a = ram_peek(0x0003);
  /*$765C*/ s_y = ram_peek(0x0002);
  /*$765E*/ rom_plot(0xfffe);
  /*$7661*/ CYCLES(0x7661, 19);
  /*$7663*/ ram_poke(0x0000, 0x01);
  /*$7667*/ ram_poke(0x0001, 0x09);
  /*$7669*/ func_60e4(0xfffe);
  /*$766C*/ CYCLES(0x766c, 35);
  /*$7671*/ tmp2_U16 = adc_dec16(ram_peek(0x725f), 0x01, 0x00);
            s_status_c = ((uint8_t)(tmp2_U16 >> 8) & 0x01);
  /*$7673*/ ram_poke(0x725f, ((uint8_t)tmp2_U16));
  /*$7676*/ s_a = ram_peek(0x7260);
            if (0x01)
              goto bb_3;
bb_2:
  /*$7679*/ tmp2_U16 = s_a;
            tmp3_U16 = tmp2_U16 + s_status_c;
            s_status_c = (uint8_t)(tmp3_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0x0000);
            tmp1_U8 = (uint8_t)tmp3_U16;
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            goto bb_4;
bb_3:
  /*$7679*/ tmp3_U16 = adc_dec16(s_a, 0x00, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
            s_status_c = (tmp1_U8 & 0x01);
            s_status_not_z = (~tmp1_U8 & 2);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
            s_status_n = (tmp1_U8 & 0x80);
bb_4:
  /*$767B*/ ram_poke(0x7260, s_a);
  /*$767E*/ s_status_d = 0x00;
  /*$767F*/ if (ret_addr) pop16(); return;
}


void FUNC_MVBLKUP2(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;
  uint8_t tmp5_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$D39A*/ CYCLES(0xd39a, 29);
            s_status_c = 0x01;
  /*$D39B*/ s_a = ram_peek(0x0096);
            if (s_status_d)
              goto bb_2;
bb_1:
  /*$D39D*/ tmp1_U16 = (s_a - ram_peek(0x009b)) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp1_U16 >> 8) & 0x01));
            s_a = ((uint8_t)tmp1_U16);
            goto bb_3;
bb_2:
  /*$D39D*/ tmp1_U16 = sbc_dec16(s_a, ram_peek(0x009b), s_status_c);
            s_a = ((uint8_t)tmp1_U16);
            s_status_c = ((uint8_t)(tmp1_U16 >> 8) & 0x01);
bb_3:
  /*$D39F*/ tmp2_U8 = s_a;
            ram_poke(0x005e, tmp2_U8);
  /*$D3A1*/ s_y = tmp2_U8;
  /*$D3A2*/ s_a = ram_peek(0x0097);
            if (s_status_d)
              goto bb_5;
bb_4:
  /*$D3A4*/ tmp1_U16 = s_a;
            tmp3_U16 = ram_peek(0x009c);
            tmp4_U16 = (tmp1_U16 - tmp3_U16) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp1_U16, (uint8_t)(~tmp3_U16));
            s_a = ((uint8_t)tmp4_U16);
            goto bb_6;
bb_5:
  /*$D3A4*/ tmp1_U16 = sbc_dec16(s_a, ram_peek(0x009c), s_status_c);
            s_a = ((uint8_t)tmp1_U16);
            tmp2_U8 = (uint8_t)(tmp1_U16 >> 8);
            s_status_c = (tmp2_U8 & 0x01);
            s_status_v = ((tmp2_U8 & 0x40) != 0);
bb_6:
  /*$D3A7*/ s_x = (uint8_t)(s_a + 0x01);
  /*$D3A8*/ tmp2_U8 = s_y;
            s_a = tmp2_U8;
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_20;
bb_7:
  /*$D3AB*/ CYCLES(0xd3ab, 16);
            s_a = ram_peek(0x0096);
  /*$D3AD*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_9;
bb_8:
  /*$D3AE*/ tmp4_U16 = (s_a - ram_peek(0x005e)) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
            s_a = ((uint8_t)tmp4_U16);
            goto bb_10;
bb_9:
  /*$D3AE*/ tmp1_U16 = sbc_dec16(s_a, ram_peek(0x005e), s_status_c);
            s_a = ((uint8_t)tmp1_U16);
            s_status_c = ((uint8_t)(tmp1_U16 >> 8) & 0x01);
bb_10:
  /*$D3B0*/ ram_poke(0x0096, s_a);
            branchTarget = true;
            if (s_status_c)
              goto bb_12;
bb_11:
  /*$D3B4*/ CYCLES(0xd3b4, 6);
            ram_poke(0x0097, (uint8_t)(ram_peek(0x0097) - 0x01));
  /*$D3B6*/ s_status_c = 0x01;
bb_12:
  /*$D3B7*/ CYCLES(0xd3b7, 14);
            s_a = ram_peek(0x0094);
            if (s_status_d)
              goto bb_14;
bb_13:
  /*$D3B9*/ tmp4_U16 = s_a;
            tmp3_U16 = ram_peek(0x005e);
            tmp1_U16 = (tmp4_U16 - tmp3_U16) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp1_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp1_U16, (uint8_t)tmp4_U16, (uint8_t)(~tmp3_U16));
            s_a = ((uint8_t)tmp1_U16);
            goto bb_15;
bb_14:
  /*$D3B9*/ tmp1_U16 = sbc_dec16(s_a, ram_peek(0x005e), s_status_c);
            s_a = ((uint8_t)tmp1_U16);
            tmp2_U8 = (uint8_t)(tmp1_U16 >> 8);
            s_status_c = (tmp2_U8 & 0x01);
            s_status_v = ((tmp2_U8 & 0x40) != 0);
bb_15:
  /*$D3BB*/ ram_poke(0x0094, s_a);
            branchTarget = true;
            if (s_status_c)
              goto bb_18;
bb_16:
  /*$D3BF*/ CYCLES(0xd3bf, 7);
            ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) - 0x01));
            branchTarget = true;
            if (!s_status_c)
              goto bb_18;
bb_17:
  /*$D3C3*/ CYCLES(0xd3c3, 7);
            tmp2_U8 = s_y;
            tmp5_U8 = peek((ram_peek16al(0x0096) + tmp2_U8));
  /*$D3C5*/ poke((ram_peek16al(0x0094) + tmp2_U8), tmp5_U8);
bb_18:
  /*$D3C7*/ CYCLES(0xd3c7, 6);
            tmp2_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp2_U8;
            branchTarget = true;
            if (tmp2_U8)
              goto bb_17;
bb_19:
  /*$D3CA*/ CYCLES(0xd3ca, 7);
            tmp2_U8 = s_y;
            tmp5_U8 = peek((ram_peek16al(0x0096) + tmp2_U8));
            s_a = tmp5_U8;
  /*$D3CC*/ poke((ram_peek16al(0x0094) + tmp2_U8), tmp5_U8);
bb_20:
  /*$D3CE*/ CYCLES(0xd3ce, 12);
            ram_poke(0x0097, (uint8_t)(ram_peek(0x0097) - 0x01));
  /*$D3D0*/ ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) - 0x01));
  /*$D3D2*/ tmp2_U8 = (uint8_t)(s_x - 0x01);
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_x = tmp2_U8;
            branchTarget = true;
            if (tmp2_U8)
              goto bb_18;
bb_21:
  /*$D3D5*/ CYCLES(0xd3d5, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_FNDLIN(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$D61A*/ CYCLES(0xd61a, 7);
            s_a = ram_peek(0x0067);
  /*$D61C*/ s_x = ram_peek(0x0068);
            FUNC_FNDLIN2(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_FNDLIN2(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$D61E*/ CYCLES(0xd61e, 18);
            s_y = 0x01;
  /*$D620*/ ram_poke(0x009b, s_a);
  /*$D622*/ ram_poke(0x009c, s_x);
  /*$D624*/ tmp1_U8 = peek((ram_peek16(0x009b) + 0x0001));
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_7;
bb_1:
  /*$D628*/ CYCLES(0xd628, 14);
  /*$D629*/ tmp1_U8 = (uint8_t)(s_y + 0x02);
            s_y = tmp1_U8;
  /*$D62A*/ tmp2_U8 = ram_peek(0x0051);
            s_a = tmp2_U8;
  /*$D62C*/ tmp1_U8 = peek((ram_peek16(0x009b) + tmp1_U8));
            s_status_not_z = (tmp2_U8 != tmp1_U8);
            tmp3_U8 = tmp2_U8 >= tmp1_U8;
            s_status_c = tmp3_U8;
            s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
            branchTarget = true;
            if (!tmp3_U8)
              goto bb_8;
bb_2:
  /*$D630*/ CYCLES(0xd630, 4);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_4;
bb_3:
  /*$D632*/ CYCLES(0xd632, 6);
            tmp3_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp3_U8;
            branchTarget = true;
            if (tmp3_U8)
              goto bb_6;
bb_4:
  /*$D635*/ CYCLES(0xd635, 12);
            tmp3_U8 = ram_peek(0x0050);
            s_a = tmp3_U8;
  /*$D637*/ tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
  /*$D638*/ tmp1_U8 = peek((ram_peek16(0x009b) + tmp1_U8));
            s_status_not_z = (tmp3_U8 != tmp1_U8);
            tmp2_U8 = tmp3_U8 >= tmp1_U8;
            s_status_c = tmp2_U8;
            s_status_n = ((uint8_t)(tmp3_U8 - tmp1_U8) & 0x80);
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_8;
bb_5:
  /*$D63C*/ CYCLES(0xd63c, 4);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_8;
bb_6:
  /*$D63E*/ CYCLES(0xd63e, 16);
            tmp2_U8 = s_y;
  /*$D63F*/ tmp1_U8 = peek((ram_peek16(0x009b) + (uint8_t)(tmp2_U8 - 0x01)));
  /*$D641*/ s_x = tmp1_U8;
  /*$D642*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x02);
            s_y = tmp2_U8;
  /*$D643*/ tmp2_U8 = peek((ram_peek16(0x009b) + tmp2_U8));
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_a = tmp2_U8;
            branchTarget = true;
            if (s_status_c)
              goto bb_0;
bb_7:
  /*$D647*/ CYCLES(0xd647, 2);
            s_status_c = 0x00;
bb_8:
  /*$D648*/ CYCLES(0xd648, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_CLRTXTPTR(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$D697*/ CYCLES(0xd697, 24);
            s_status_c = 0x00;
  /*$D698*/ s_a = ram_peek(0x0067);
            if (s_status_d)
              goto bb_2;
bb_1:
  /*$D69A*/ tmp1_U16 = (s_a + 0x00ff) + s_status_c;
            s_status_c = (uint8_t)(tmp1_U16 >> 8);
            s_a = ((uint8_t)tmp1_U16);
            goto bb_3;
bb_2:
  /*$D69A*/ tmp1_U16 = adc_dec16(s_a, 0xff, s_status_c);
            s_a = ((uint8_t)tmp1_U16);
            s_status_c = ((uint8_t)(tmp1_U16 >> 8) & 0x01);
bb_3:
  /*$D69C*/ ram_poke(0x00b8, s_a);
  /*$D69E*/ s_a = ram_peek(0x0068);
            if (s_status_d)
              goto bb_5;
bb_4:
  /*$D6A0*/ s_a = (uint8_t)((s_a + 0x00ff) + s_status_c);
            goto bb_6;
bb_5:
  /*$D6A0*/ s_a = ((uint8_t)adc_dec16(s_a, 0xff, s_status_c));
bb_6:
  /*$D6A2*/ ram_poke(0x00b9, s_a);
  /*$D6A4*/ if (ret_addr) pop16(); return;
}


void FUNC_RESTORE(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint16_t tmp2_U16;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$D849*/ CYCLES(0xd849, 16);
            s_status_c = 0x01;
  /*$D84A*/ s_a = ram_peek(0x0067);
            if (s_status_d)
              goto bb_2;
bb_1:
  /*$D84C*/ tmp1_U16 = s_a;
            tmp2_U16 = (tmp1_U16 - 0x0001) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp2_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp2_U16, (uint8_t)tmp1_U16, (uint8_t)0xfffe);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_3;
bb_2:
  /*$D84C*/ tmp2_U16 = sbc_dec16(s_a, 0x01, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            tmp3_U8 = (uint8_t)(tmp2_U16 >> 8);
            s_status_c = (tmp3_U8 & 0x01);
            s_status_v = ((tmp3_U8 & 0x40) != 0);
bb_3:
  /*$D84E*/ s_y = ram_peek(0x0068);
            branchTarget = true;
            if (s_status_c)
              goto bb_5;
bb_4:
  /*$D852*/ CYCLES(0xd852, 2);
            s_y = (uint8_t)(s_y - 0x01);
bb_5:
  /*$D853*/ CYCLES(0xd853, 7);
            ram_poke(0x007d, s_a);
  /*$D855*/ ram_poke(0x007e, s_y);
  /*$D857*/ CYCLES(0xd857, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_CRDO(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$DAFB*/ CYCLES(0xdafb, 9);
            s_a = 0x0d;
  /*$DAFD*/ FUNC_OUTDO(0xfffe);
            FUNC_NEGATE(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_NEGATE(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$DB00*/ CYCLES(0xdb00, 4);
            tmp1_U8 = s_a ^ 0xff;
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
  /*$DB02*/ CYCLES(0xdb02, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_OUTSP(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$DB57*/ CYCLES(0xdb57, 9);
            s_a = 0x20;
  /*$DB59*/ s_status_v = ((ram_peek(0x3fa9) >> 0x06) & 0x01);
            FUNC_OUTDO(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_OUTQUES(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$DB5A*/ CYCLES(0xdb5a, 4);
            s_a = 0x3f;
            FUNC_OUTDO(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_OUTDO(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$DB5C*/ CYCLES(0xdb5c, 11);
            tmp1_U8 = s_a | 0x80;
            s_a = tmp1_U8;
  /*$DB5E*/ s_status_not_z = (tmp1_U8 != 0xa0);
            tmp2_U8 = tmp1_U8 >= 0xa0;
            s_status_c = tmp2_U8;
            s_status_n = ((uint8_t)(tmp1_U8 - 0xa0) & 0x80);
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_2;
bb_1:
  /*$DB62*/ CYCLES(0xdb62, 4);
            tmp2_U8 = s_a | ram_peek(0x00f3);
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_a = tmp2_U8;
bb_2:
  /*$DB64*/ CYCLES(0xdb64, 6);
            rom_cout(0xfffe);
  /*$DB67*/ CYCLES(0xdb67, 14);
  /*$DB69*/ push8((s_a & 0x7f));
  /*$DB6A*/ s_a = ram_peek(0x00f1);
  /*$DB6C*/ FUNC_MON_WAIT(0xfffe);
  /*$DB6F*/ CYCLES(0xdb6f, 4);
            tmp2_U8 = pop8();
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_a = tmp2_U8;
  /*$DB70*/ if (ret_addr) pop16(); return;
}


void FUNC_ISLETC(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E07D*/ CYCLES(0xe07d, 7);
            tmp1_U8 = s_a;
            s_status_not_z = (tmp1_U8 != 0x41);
            tmp2_U8 = tmp1_U8 >= 0x41;
            s_status_c = tmp2_U8;
            s_status_n = ((uint8_t)(tmp1_U8 - 0x41) & 0x80);
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_8;
bb_1:
  /*$E081*/ CYCLES(0xe081, 9);
            if (s_status_d)
              goto bb_3;
bb_2:
  /*$E081*/ s_a = (uint8_t)((s_a - 0x005b) - (uint8_t)(0x01 - s_status_c));
            goto bb_4;
bb_3:
  /*$E081*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x5b, s_status_c));
bb_4:
  /*$E083*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_6;
bb_5:
  /*$E084*/ tmp3_U16 = s_a;
            tmp4_U16 = (tmp3_U16 - 0x00a5) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0xff5a);
            tmp2_U8 = (uint8_t)tmp4_U16;
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_a = tmp2_U8;
            goto bb_7;
bb_6:
  /*$E084*/ tmp4_U16 = sbc_dec16(s_a, 0xa5, s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
            s_status_c = (tmp2_U8 & 0x01);
            s_status_not_z = (~tmp2_U8 & 2);
            s_status_v = ((tmp2_U8 & 0x40) != 0);
            s_status_n = (tmp2_U8 & 0x80);
bb_7:
bb_8:
  /*$E086*/ CYCLES(0xe086, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_GETARY(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;
  uint8_t tmp4_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E0ED*/ CYCLES(0xe0ed, 19);
  /*$E0EF*/ tmp1_U16 = ram_peek(0x000f) << 0x01;
            s_status_c = (uint8_t)(tmp1_U16 >> 8);
            s_a = ((uint8_t)tmp1_U16);
            if (s_status_d)
              goto bb_2;
bb_1:
  /*$E0F0*/ tmp1_U16 = (s_a + 0x0005) + s_status_c;
            s_status_c = (uint8_t)(tmp1_U16 >> 8);
            s_a = ((uint8_t)tmp1_U16);
            goto bb_3;
bb_2:
  /*$E0F0*/ tmp3_U16 = adc_dec16(s_a, 0x05, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
bb_3:
            if (s_status_d)
              goto bb_5;
bb_4:
  /*$E0F2*/ tmp1_U16 = s_a;
            tmp2_U16 = ram_peek(0x009b);
            tmp3_U16 = (tmp1_U16 + tmp2_U16) + s_status_c;
            s_status_c = (uint8_t)(tmp3_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp1_U16, (uint8_t)tmp2_U16);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_6;
bb_5:
  /*$E0F2*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x009b), s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            tmp4_U8 = (uint8_t)(tmp3_U16 >> 8);
            s_status_c = (tmp4_U8 & 0x01);
            s_status_v = ((tmp4_U8 & 0x40) != 0);
bb_6:
  /*$E0F4*/ s_y = ram_peek(0x009c);
            branchTarget = true;
            if (!s_status_c)
              goto bb_8;
bb_7:
  /*$E0F8*/ CYCLES(0xe0f8, 2);
            s_y = (uint8_t)(s_y + 0x01);
bb_8:
  /*$E0F9*/ CYCLES(0xe0f9, 9);
            ram_poke(0x0094, s_a);
  /*$E0FB*/ ram_poke(0x0095, s_y);
  /*$E0FD*/ if (ret_addr) pop16(); return;
}


void FUNC_GARBAGE(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;
  uint16_t tmp5_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E484*/ CYCLES(0xe484, 7);
            s_x = ram_peek(0x0073);
  /*$E486*/ s_a = ram_peek(0x0074);
bb_1:
  /*$E488*/ CYCLES(0xe488, 41);
            ram_poke(0x006f, s_x);
  /*$E48A*/ ram_poke(0x0070, s_a);
  /*$E48C*/ s_y = 0x00;
  /*$E48E*/ ram_poke(0x008b, 0x00);
  /*$E494*/ ram_poke(0x009b, ram_peek(0x006d));
  /*$E496*/ ram_poke(0x009c, ram_peek(0x006e));
  /*$E498*/ s_a = 0x55;
  /*$E49C*/ ram_poke(0x005e, 0x55);
  /*$E49E*/ ram_poke(0x005f, 0x00);
bb_2:
  /*$E4A0*/ CYCLES(0xe4a0, 7);
            branchTarget = true;
            if (!(s_a != ram_peek(0x0052)))
              goto bb_4;
bb_3:
  /*$E4A4*/ CYCLES(0xe4a4, 6);
            FUNC_CHKVAR(0xfffe);
  /*$E4A7*/ CYCLES(0xe4a7, 4);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_2;
bb_4:
  /*$E4A9*/ CYCLES(0xe4a9, 21);
  /*$E4AB*/ ram_poke(0x008f, 0x07);
  /*$E4AD*/ tmp1_U8 = ram_peek(0x0069);
            s_a = tmp1_U8;
  /*$E4AF*/ tmp2_U8 = ram_peek(0x006a);
            s_x = tmp2_U8;
  /*$E4B1*/ ram_poke(0x005e, tmp1_U8);
  /*$E4B3*/ ram_poke(0x005f, tmp2_U8);
bb_5:
  /*$E4B5*/ CYCLES(0xe4b5, 7);
            branchTarget = true;
            if ((s_x != ram_peek(0x006c)))
              goto bb_7;
bb_6:
  /*$E4B9*/ CYCLES(0xe4b9, 7);
            branchTarget = true;
            if (!(s_a != ram_peek(0x006b)))
              goto bb_8;
bb_7:
  /*$E4BD*/ CYCLES(0xe4bd, 6);
            FUNC_CHKSMPLVAR(0xfffe);
  /*$E4C0*/ CYCLES(0xe4c0, 4);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_5;
bb_8:
  /*$E4C2*/ CYCLES(0xe4c2, 14);
            ram_poke(0x0094, s_a);
  /*$E4C4*/ ram_poke(0x0095, s_x);
  /*$E4C8*/ ram_poke(0x008f, 0x03);
bb_9:
  /*$E4CA*/ CYCLES(0xe4ca, 7);
            s_a = ram_peek(0x0094);
  /*$E4CC*/ s_x = ram_peek(0x0095);
bb_10:
  /*$E4CE*/ CYCLES(0xe4ce, 7);
            tmp2_U8 = s_x;
            tmp1_U8 = ram_peek(0x006e);
            s_status_c = (tmp2_U8 >= tmp1_U8);
            branchTarget = true;
            if ((tmp2_U8 != tmp1_U8))
              goto bb_13;
bb_11:
  /*$E4D2*/ CYCLES(0xe4d2, 7);
            tmp1_U8 = s_a;
            tmp2_U8 = ram_peek(0x006d);
            s_status_c = (tmp1_U8 >= tmp2_U8);
            branchTarget = true;
            if ((tmp1_U8 != tmp2_U8))
              goto bb_13;
bb_12:
  /*$E4D6*/ CYCLES(0xe4d6, 6);
  /*$E562*/ CYCLES(0xe562, 7);
            branchTarget = true;
            if (!ram_peek(0x008b))
              goto bb_33;
            else
              goto bb_34;
bb_13:
  /*$E4D9*/ CYCLES(0xe4d9, 52);
            ram_poke(0x005e, s_a);
  /*$E4DB*/ ram_poke(0x005f, s_x);
  /*$E4DF*/ tmp2_U8 = peek(ram_peek16al(0x005e));
  /*$E4E1*/ s_x = tmp2_U8;
  /*$E4E3*/ tmp2_U8 = peek((ram_peek16al(0x005e) + 0x0001));
  /*$E4E5*/ tmp1_U8 = s_status_d;
            push8((s_status_c | ((tmp2_U8 == 0) << 1) | (s_status_i << 2) | (tmp1_U8 << 3) | STATUS_B | (s_status_v << 6) | (tmp2_U8 & 0x80)));
  /*$E4E6*/ s_y = 0x02;
  /*$E4E7*/ tmp2_U8 = peek((ram_peek16al(0x005e) + 0x0002));
            s_a = tmp2_U8;
            if (tmp1_U8)
              goto bb_15;
bb_14:
  /*$E4E9*/ tmp4_U16 = (s_a + ram_peek(0x0094)) + s_status_c;
            s_status_c = (uint8_t)(tmp4_U16 >> 8);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_16;
bb_15:
  /*$E4E9*/ tmp5_U16 = adc_dec16(s_a, ram_peek(0x0094), s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            s_status_c = ((uint8_t)(tmp5_U16 >> 8) & 0x01);
bb_16:
  /*$E4EB*/ ram_poke(0x0094, s_a);
  /*$E4ED*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$E4EE*/ tmp1_U8 = peek((ram_peek16al(0x005e) + tmp1_U8));
            s_a = tmp1_U8;
            if (s_status_d)
              goto bb_18;
bb_17:
  /*$E4F0*/ s_a = (uint8_t)((s_a + ram_peek(0x0095)) + s_status_c);
            goto bb_19;
bb_18:
  /*$E4F0*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x0095), s_status_c));
bb_19:
  /*$E4F2*/ ram_poke(0x0095, s_a);
  /*$E4F4*/ tmp1_U8 = pop8();
            s_status_i = ((tmp1_U8 & 0x04) != 0);
            s_status_d = ((tmp1_U8 & 0x08) != 0);
            s_status_b = 0x00;
            s_status_v = ((tmp1_U8 & 0x40) != 0);
            branchTarget = true;
            if (!(tmp1_U8 & 0x80))
              goto bb_9;
bb_20:
  /*$E4F7*/ CYCLES(0xe4f7, 6);
            branchTarget = true;
            if ((s_x & 0x80))
              goto bb_9;
bb_21:
  /*$E4FA*/ CYCLES(0xe4fa, 24);
  /*$E4FB*/ tmp1_U8 = peek((ram_peek16al(0x005e) + (uint8_t)(s_y + 0x01)));
  /*$E4FD*/ s_y = 0x00;
  /*$E4FF*/ tmp4_U16 = tmp1_U8 << 0x01;
            s_status_c = (uint8_t)(tmp4_U16 >> 8);
            s_a = ((uint8_t)tmp4_U16);
            if (s_status_d)
              goto bb_23;
bb_22:
  /*$E500*/ tmp4_U16 = (s_a + 0x0005) + s_status_c;
            s_status_c = (uint8_t)(tmp4_U16 >> 8);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_24;
bb_23:
  /*$E500*/ tmp5_U16 = adc_dec16(s_a, 0x05, s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            s_status_c = ((uint8_t)(tmp5_U16 >> 8) & 0x01);
bb_24:
            if (s_status_d)
              goto bb_26;
bb_25:
  /*$E502*/ tmp4_U16 = s_a;
            tmp3_U16 = ram_peek(0x005e);
            tmp5_U16 = (tmp4_U16 + tmp3_U16) + s_status_c;
            s_status_c = (uint8_t)(tmp5_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)tmp3_U16);
            s_a = ((uint8_t)tmp5_U16);
            goto bb_27;
bb_26:
  /*$E502*/ tmp5_U16 = adc_dec16(s_a, ram_peek(0x005e), s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
            s_status_c = (tmp1_U8 & 0x01);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
bb_27:
  /*$E504*/ ram_poke(0x005e, s_a);
            branchTarget = true;
            if (!s_status_c)
              goto bb_29;
bb_28:
  /*$E508*/ CYCLES(0xe508, 4);
            ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) + 0x01));
bb_29:
  /*$E50A*/ CYCLES(0xe50a, 4);
            s_x = ram_peek(0x005f);
bb_30:
  /*$E50C*/ CYCLES(0xe50c, 7);
            branchTarget = true;
            if ((s_x != ram_peek(0x0095)))
              goto bb_32;
bb_31:
  /*$E510*/ CYCLES(0xe510, 7);
            branchTarget = true;
            if (!(s_a != ram_peek(0x0094)))
              goto bb_10;
bb_32:
  /*$E514*/ CYCLES(0xe514, 6);
            FUNC_CHKVAR(0xfffe);
  /*$E517*/ CYCLES(0xe517, 4);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_30;
            else
              goto bb_41;
bb_33:
  /*$E55D*/ CYCLES(0xe55d, 9);
            s_x = ram_peek(0x005f);
  /*$E55F*/ s_status_not_z = 0x00;
            s_status_n = 0x00;
            s_y = 0x00;
  /*$E561*/ if (ret_addr) pop16(); return;
bb_34:
  /*$E566*/ CYCLES(0xe566, 53);
  /*$E56A*/ s_status_c = 0x00;
            tmp2_U8 = (ram_peek(0x0091) & 0x04) >> 0x01;
  /*$E56C*/ ram_poke(0x0091, tmp2_U8);
  /*$E56E*/ tmp2_U8 = peek((ram_peek16al(0x008a) + tmp2_U8));
            s_a = tmp2_U8;
            if (s_status_d)
              goto bb_36;
bb_35:
  /*$E570*/ tmp3_U16 = (s_a + ram_peek(0x009b)) + s_status_c;
            s_status_c = (uint8_t)(tmp3_U16 >> 8);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_37;
bb_36:
  /*$E570*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x009b), s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
bb_37:
  /*$E572*/ ram_poke(0x0096, s_a);
  /*$E574*/ s_a = ram_peek(0x009c);
            if (s_status_d)
              goto bb_39;
bb_38:
  /*$E576*/ tmp3_U16 = s_a;
            tmp4_U16 = tmp3_U16 + s_status_c;
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x0000);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_40;
bb_39:
  /*$E576*/ tmp4_U16 = adc_dec16(s_a, 0x00, s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
bb_40:
  /*$E578*/ ram_poke(0x0097, s_a);
  /*$E57E*/ ram_poke(0x0094, ram_peek(0x006f));
  /*$E580*/ ram_poke(0x0095, ram_peek(0x0070));
  /*$E582*/ FUNC_MVBLKUP2(0xfffe);
  /*$E585*/ CYCLES(0xe585, 31);
            tmp2_U8 = ram_peek(0x0091);
  /*$E588*/ tmp1_U8 = ram_peek(0x0094);
  /*$E58A*/ poke((ram_peek16al(0x008a) + (uint8_t)(tmp2_U8 + 0x01)), tmp1_U8);
  /*$E58C*/ s_x = tmp1_U8;
  /*$E58D*/ ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) + 0x01));
  /*$E58F*/ tmp1_U8 = ram_peek(0x0095);
            s_a = tmp1_U8;
  /*$E592*/ poke((ram_peek16al(0x008a) + (uint8_t)(tmp2_U8 + 0x02)), tmp1_U8);
            branchTarget = true;
            goto bb_1;
bb_41:
  /*$E517*/ FUNC_CHKSMPLVAR(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_CHKSMPLVAR(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E519*/ CYCLES(0xe519, 7);
            tmp1_U8 = peek((ram_peek16al(0x005e) + s_y));
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_3;
bb_1:
  /*$E51D*/ CYCLES(0xe51d, 9);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$E51E*/ tmp1_U8 = peek((ram_peek16al(0x005e) + tmp1_U8));
            branchTarget = true;
            if (!(tmp1_U8 & 0x80))
              goto bb_3;
bb_2:
  /*$E522*/ CYCLES(0xe522, 2);
            s_y = (uint8_t)(s_y + 0x01);
            FUNC_CHKVAR(0x0000);
            if (ret_addr) pop16(); return;
bb_3:
  /*$E552*/ CYCLES(0xe552, 16);
            s_a = ram_peek(0x008f);
  /*$E554*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_5;
bb_4:
  /*$E555*/ tmp2_U16 = s_a;
            tmp3_U16 = ram_peek(0x005e);
            tmp4_U16 = (tmp2_U16 + tmp3_U16) + s_status_c;
            s_status_c = (uint8_t)(tmp4_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp2_U16, (uint8_t)tmp3_U16);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_6;
bb_5:
  /*$E555*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x005e), s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
            s_status_c = (tmp1_U8 & 0x01);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
bb_6:
  /*$E557*/ ram_poke(0x005e, s_a);
            branchTarget = true;
            if (!s_status_c)
              goto bb_8;
bb_7:
  /*$E55B*/ CYCLES(0xe55b, 4);
            ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) + 0x01));
bb_8:
  /*$E55D*/ CYCLES(0xe55d, 9);
            s_x = ram_peek(0x005f);
  /*$E55F*/ s_status_not_z = 0x00;
            s_status_n = 0x00;
            s_y = 0x00;
  /*$E561*/ if (ret_addr) pop16(); return;
}


void FUNC_CHKVAR(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;
  uint16_t tmp5_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E523*/ CYCLES(0xe523, 7);
            tmp1_U8 = peek((ram_peek16al(0x005e) + s_y));
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_8;
bb_1:
  /*$E527*/ CYCLES(0xe527, 19);
            tmp1_U8 = s_y;
  /*$E528*/ tmp2_U8 = peek((ram_peek16al(0x005e) + (uint8_t)(tmp1_U8 + 0x01)));
  /*$E52A*/ s_x = tmp2_U8;
  /*$E52C*/ tmp1_U8 = peek((ram_peek16al(0x005e) + (uint8_t)(tmp1_U8 + 0x02)));
            s_a = tmp1_U8;
  /*$E52E*/ tmp2_U8 = ram_peek(0x0070);
            s_status_not_z = (tmp1_U8 != tmp2_U8);
            branchTarget = true;
            if (!(tmp1_U8 >= tmp2_U8))
              goto bb_4;
bb_2:
  /*$E532*/ CYCLES(0xe532, 4);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_8;
bb_3:
  /*$E534*/ CYCLES(0xe534, 7);
            branchTarget = true;
            if ((s_x >= ram_peek(0x006f)))
              goto bb_8;
bb_4:
  /*$E538*/ CYCLES(0xe538, 7);
            tmp2_U8 = s_a;
            tmp1_U8 = ram_peek(0x009c);
            s_status_not_z = (tmp2_U8 != tmp1_U8);
            branchTarget = true;
            if (!(tmp2_U8 >= tmp1_U8))
              goto bb_8;
bb_5:
  /*$E53C*/ CYCLES(0xe53c, 4);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_7;
bb_6:
  /*$E53E*/ CYCLES(0xe53e, 7);
            branchTarget = true;
            if (!(s_x >= ram_peek(0x009b)))
              goto bb_8;
bb_7:
  /*$E542*/ CYCLES(0xe542, 28);
            ram_poke(0x009b, s_x);
  /*$E544*/ ram_poke(0x009c, s_a);
  /*$E54A*/ ram_poke(0x008a, ram_peek(0x005e));
  /*$E54C*/ ram_poke(0x008b, ram_peek(0x005f));
  /*$E550*/ ram_poke(0x0091, ram_peek(0x008f));
bb_8:
  /*$E552*/ CYCLES(0xe552, 16);
            s_a = ram_peek(0x008f);
  /*$E554*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_10;
bb_9:
  /*$E555*/ tmp3_U16 = s_a;
            tmp4_U16 = ram_peek(0x005e);
            tmp5_U16 = (tmp3_U16 + tmp4_U16) + s_status_c;
            s_status_c = (uint8_t)(tmp5_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
            s_a = ((uint8_t)tmp5_U16);
            goto bb_11;
bb_10:
  /*$E555*/ tmp5_U16 = adc_dec16(s_a, ram_peek(0x005e), s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
            s_status_c = (tmp1_U8 & 0x01);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
bb_11:
  /*$E557*/ ram_poke(0x005e, s_a);
            branchTarget = true;
            if (!s_status_c)
              goto bb_13;
bb_12:
  /*$E55B*/ CYCLES(0xe55b, 4);
            ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) + 0x01));
bb_13:
  /*$E55D*/ CYCLES(0xe55d, 9);
            s_x = ram_peek(0x005f);
  /*$E55F*/ s_status_not_z = 0x00;
            s_status_n = 0x00;
            s_y = 0x00;
  /*$E561*/ if (ret_addr) pop16(); return;
}


void FUNC_MOVINS(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E5D4*/ CYCLES(0xe5d4, 24);
  /*$E5D6*/ tmp1_U8 = peek(ram_peek16(0x00ab));
  /*$E5D8*/ push8(tmp1_U8);
  /*$E5DA*/ tmp1_U8 = peek((ram_peek16(0x00ab) + 0x0001));
  /*$E5DC*/ s_x = tmp1_U8;
  /*$E5DE*/ tmp1_U8 = peek((ram_peek16(0x00ab) + 0x0002));
  /*$E5E0*/ s_y = tmp1_U8;
  /*$E5E1*/ tmp1_U8 = pop8();
            s_a = tmp1_U8;
            FUNC_MOVSTR(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_MOVSTR(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E5E2*/ CYCLES(0xe5e2, 7);
            ram_poke(0x005e, s_x);
  /*$E5E4*/ ram_poke(0x005f, s_y);
            FUNC_MOVSTR1(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_MOVSTR1(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;
  uint16_t tmp5_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E5E6*/ CYCLES(0xe5e6, 6);
            tmp1_U8 = s_a;
            s_y = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_4;
bb_1:
  /*$E5E9*/ CYCLES(0xe5e9, 2);
            push8(s_a);
bb_2:
  /*$E5EA*/ CYCLES(0xe5ea, 14);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
  /*$E5EB*/ tmp2_U8 = peek((ram_peek16al(0x005e) + tmp1_U8));
  /*$E5ED*/ poke((ram_peek16(0x0071) + tmp1_U8), tmp2_U8);
            branchTarget = true;
            if (tmp1_U8)
              goto bb_2;
bb_3:
  /*$E5F2*/ CYCLES(0xe5f2, 2);
            tmp1_U8 = pop8();
            s_a = tmp1_U8;
bb_4:
  /*$E5F3*/ CYCLES(0xe5f3, 12);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_6;
bb_5:
  /*$E5F4*/ tmp3_U16 = s_a;
            tmp4_U16 = ram_peek(0x0071);
            tmp5_U16 = (tmp3_U16 + tmp4_U16) + s_status_c;
            s_status_c = (uint8_t)(tmp5_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
            tmp1_U8 = (uint8_t)tmp5_U16;
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            goto bb_7;
bb_6:
  /*$E5F4*/ tmp5_U16 = adc_dec16(s_a, ram_peek(0x0071), s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
            s_status_c = (tmp1_U8 & 0x01);
            s_status_not_z = (~tmp1_U8 & 2);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
            s_status_n = (tmp1_U8 & 0x80);
bb_7:
  /*$E5F6*/ ram_poke(0x0071, s_a);
            branchTarget = true;
            if (!s_status_c)
              goto bb_9;
bb_8:
  /*$E5FA*/ CYCLES(0xe5fa, 4);
            tmp1_U8 = (uint8_t)(ram_peek(0x0072) + 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            ram_poke(0x0072, tmp1_U8);
bb_9:
  /*$E5FC*/ CYCLES(0xe5fc, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_FREFAC(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E600*/ CYCLES(0xe600, 7);
            s_a = ram_peek(0x00a0);
  /*$E602*/ s_y = ram_peek(0x00a1);
            FUNC_FRETMP(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_FRETMP(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;
  uint16_t tmp4_U16;
  uint16_t tmp5_U16;
  uint16_t tmp6_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E604*/ CYCLES(0xe604, 12);
            ram_poke(0x005e, s_a);
  /*$E606*/ ram_poke(0x005f, s_y);
  /*$E608*/ FUNC_FRETMS(0xfffe);
  /*$E60B*/ CYCLES(0xe60b, 31);
            push8((s_status_c | ((s_status_not_z == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | s_status_n));
  /*$E60E*/ tmp1_U8 = peek(ram_peek16al(0x005e));
  /*$E610*/ push8(tmp1_U8);
  /*$E612*/ tmp1_U8 = peek((ram_peek16al(0x005e) + 0x0001));
  /*$E614*/ s_x = tmp1_U8;
  /*$E616*/ tmp1_U8 = peek((ram_peek16al(0x005e) + 0x0002));
  /*$E618*/ s_y = tmp1_U8;
  /*$E619*/ tmp1_U8 = pop8();
            s_a = tmp1_U8;
  /*$E61A*/ tmp1_U8 = pop8();
            s_status_c = (tmp1_U8 & 0x01);
            tmp2_U8 = (~tmp1_U8 & 2);
            s_status_not_z = tmp2_U8;
            s_status_i = ((tmp1_U8 & 0x04) != 0);
            s_status_d = ((tmp1_U8 & 0x08) != 0);
            s_status_b = 0x00;
            s_status_v = ((tmp1_U8 & 0x40) != 0);
            s_status_n = (tmp1_U8 & 0x80);
            branchTarget = true;
            if (tmp2_U8)
              goto bb_9;
bb_1:
  /*$E61D*/ CYCLES(0xe61d, 7);
            tmp2_U8 = s_y;
            tmp1_U8 = ram_peek(0x0070);
            tmp3_U8 = tmp2_U8 != tmp1_U8;
            s_status_not_z = tmp3_U8;
            s_status_c = (tmp2_U8 >= tmp1_U8);
            s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
            branchTarget = true;
            if (tmp3_U8)
              goto bb_9;
bb_2:
  /*$E621*/ CYCLES(0xe621, 7);
            tmp3_U8 = s_x;
            tmp1_U8 = ram_peek(0x006f);
            tmp2_U8 = tmp3_U8 != tmp1_U8;
            s_status_not_z = tmp2_U8;
            s_status_c = (tmp3_U8 >= tmp1_U8);
            s_status_n = ((uint8_t)(tmp3_U8 - tmp1_U8) & 0x80);
            branchTarget = true;
            if (tmp2_U8)
              goto bb_9;
bb_3:
  /*$E625*/ CYCLES(0xe625, 14);
            push8(s_a);
  /*$E626*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_5;
bb_4:
  /*$E627*/ tmp4_U16 = s_a;
            tmp5_U16 = ram_peek(0x006f);
            tmp6_U16 = (tmp4_U16 + tmp5_U16) + s_status_c;
            s_status_c = (uint8_t)(tmp6_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)tmp5_U16);
            s_a = ((uint8_t)tmp6_U16);
            goto bb_6;
bb_5:
  /*$E627*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x006f), s_status_c);
            s_a = ((uint8_t)tmp6_U16);
            tmp2_U8 = (uint8_t)(tmp6_U16 >> 8);
            s_status_c = (tmp2_U8 & 0x01);
            s_status_v = ((tmp2_U8 & 0x40) != 0);
bb_6:
  /*$E629*/ ram_poke(0x006f, s_a);
            branchTarget = true;
            if (!s_status_c)
              goto bb_8;
bb_7:
  /*$E62D*/ CYCLES(0xe62d, 4);
            ram_poke(0x0070, (uint8_t)(ram_peek(0x0070) + 0x01));
bb_8:
  /*$E62F*/ CYCLES(0xe62f, 2);
            tmp2_U8 = pop8();
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_a = tmp2_U8;
bb_9:
  /*$E630*/ CYCLES(0xe630, 9);
            ram_poke(0x005e, s_x);
  /*$E632*/ ram_poke(0x005f, s_y);
  /*$E634*/ if (ret_addr) pop16(); return;
}


void FUNC_FRETMS(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;
  uint16_t tmp4_U16;
  uint16_t tmp5_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E635*/ CYCLES(0xe635, 7);
            tmp1_U8 = s_y;
            tmp2_U8 = ram_peek(0x0054);
            tmp3_U8 = tmp1_U8 != tmp2_U8;
            s_status_not_z = tmp3_U8;
            s_status_c = (tmp1_U8 >= tmp2_U8);
            s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
            branchTarget = true;
            if (tmp3_U8)
              goto bb_6;
bb_1:
  /*$E639*/ CYCLES(0xe639, 7);
            tmp3_U8 = s_a;
            tmp2_U8 = ram_peek(0x0053);
            tmp1_U8 = tmp3_U8 != tmp2_U8;
            s_status_not_z = tmp1_U8;
            s_status_c = (tmp3_U8 >= tmp2_U8);
            s_status_n = ((uint8_t)(tmp3_U8 - tmp2_U8) & 0x80);
            branchTarget = true;
            if (tmp1_U8)
              goto bb_6;
bb_2:
  /*$E63D*/ CYCLES(0xe63d, 14);
            ram_poke(0x0052, s_a);
            if (s_status_d)
              goto bb_4;
bb_3:
  /*$E63F*/ tmp4_U16 = s_a;
            tmp5_U16 = (tmp4_U16 - 0x0003) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0xfffc);
            s_a = ((uint8_t)tmp5_U16);
            goto bb_5;
bb_4:
  /*$E63F*/ tmp5_U16 = sbc_dec16(s_a, 0x03, s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
            s_status_c = (tmp1_U8 & 0x01);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
bb_5:
  /*$E641*/ ram_poke(0x0053, s_a);
  /*$E643*/ s_status_not_z = 0x00;
            s_status_n = 0x00;
bb_6:
  /*$E645*/ CYCLES(0xe645, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_TWSCMPFAC(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E89E*/ CYCLES(0xe89e, 11);
  /*$E8A2*/ ram_poke(0x00a2, (ram_peek(0x00a2) ^ 0xff));
            FUNC_TCFACMANT(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_TCFACMANT(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E8A4*/ CYCLES(0xe8a4, 58);
  /*$E8A8*/ ram_poke(0x009e, (ram_peek(0x009e) ^ 0xff));
  /*$E8AE*/ ram_poke(0x009f, (ram_peek(0x009f) ^ 0xff));
  /*$E8B4*/ ram_poke(0x00a0, (ram_peek(0x00a0) ^ 0xff));
  /*$E8BA*/ ram_poke(0x00a1, (ram_peek(0x00a1) ^ 0xff));
  /*$E8BE*/ tmp1_U8 = ram_peek(0x00ac) ^ 0xff;
            s_a = tmp1_U8;
  /*$E8C0*/ ram_poke(0x00ac, tmp1_U8);
  /*$E8C2*/ tmp1_U8 = (uint8_t)(ram_peek(0x00ac) + 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            ram_poke(0x00ac, tmp1_U8);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_2;
bb_1:
  /*$E8D4*/ CYCLES(0xe8d4, 2);
            if (ret_addr) pop16(); return;
bb_2:
  /*$E8C4*/ FUNC_INCFACMAN(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_INCFACMAN(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E8C6*/ CYCLES(0xe8c6, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x00a1) + 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            ram_poke(0x00a1, tmp1_U8);
            branchTarget = true;
            if (tmp1_U8)
              goto bb_4;
bb_1:
  /*$E8CA*/ CYCLES(0xe8ca, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x00a0) + 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            ram_poke(0x00a0, tmp1_U8);
            branchTarget = true;
            if (tmp1_U8)
              goto bb_4;
bb_2:
  /*$E8CE*/ CYCLES(0xe8ce, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x009f) + 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            ram_poke(0x009f, tmp1_U8);
            branchTarget = true;
            if (tmp1_U8)
              goto bb_4;
bb_3:
  /*$E8D2*/ CYCLES(0xe8d2, 4);
            tmp1_U8 = (uint8_t)(ram_peek(0x009e) + 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            ram_poke(0x009e, tmp1_U8);
bb_4:
  /*$E8D4*/ CYCLES(0xe8d4, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_SHFTRGHT(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;
  uint8_t tmp4_U8;
  uint8_t tmp5_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E8F0*/ CYCLES(0xe8f0, 7);
            if (s_status_d)
              goto bb_3;
            else
              goto bb_2;
bb_1:
  /*$E8DC*/ CYCLES(0xe8dc, 35);
            tmp2_U8 = s_x;
  /*$E8DE*/ ram_poke(0x00ac, ram_peek((uint8_t)(0x04 + tmp2_U8)));
  /*$E8E2*/ ram_poke((uint8_t)(0x04 + tmp2_U8), ram_peek((uint8_t)(0x03 + tmp2_U8)));
  /*$E8E6*/ ram_poke((uint8_t)(0x03 + tmp2_U8), ram_peek((uint8_t)(0x02 + tmp2_U8)));
  /*$E8EA*/ ram_poke((uint8_t)(0x02 + tmp2_U8), ram_peek((uint8_t)(0x01 + tmp2_U8)));
  /*$E8EE*/ ram_poke((uint8_t)(0x01 + tmp2_U8), ram_peek(0x00a4));
            goto bb_0;
bb_2:
  /*$E8F0*/ tmp1_U16 = (s_a + 0x0008) + s_status_c;
            s_status_c = (uint8_t)(tmp1_U16 >> 8);
            tmp2_U8 = (uint8_t)tmp1_U16;
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_a = tmp2_U8;
            goto bb_4;
bb_3:
  /*$E8F0*/ tmp1_U16 = adc_dec16(s_a, 0x08, s_status_c);
            s_a = ((uint8_t)tmp1_U16);
            tmp2_U8 = (uint8_t)(tmp1_U16 >> 8);
            s_status_c = (tmp2_U8 & 0x01);
            s_status_not_z = (~tmp2_U8 & 2);
            s_status_n = (tmp2_U8 & 0x80);
bb_4:
            branchTarget = true;
            if (s_status_n)
              goto bb_1;
bb_5:
  /*$E8F4*/ CYCLES(0xe8f4, 4);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_1;
bb_6:
  /*$E8F6*/ CYCLES(0xe8f6, 12);
            if (s_status_d)
              goto bb_8;
bb_7:
  /*$E8F6*/ tmp1_U16 = s_a;
            tmp3_U16 = (tmp1_U16 - 0x0008) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp1_U16, (uint8_t)0xfff7);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_9;
bb_8:
  /*$E8F6*/ tmp1_U16 = sbc_dec16(s_a, 0x08, s_status_c);
            s_a = ((uint8_t)tmp1_U16);
            tmp2_U8 = (uint8_t)(tmp1_U16 >> 8);
            s_status_c = (tmp2_U8 & 0x01);
            s_status_v = ((tmp2_U8 & 0x40) != 0);
bb_9:
  /*$E8F8*/ s_y = s_a;
  /*$E8F9*/ tmp2_U8 = ram_peek(0x00ac);
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_a = tmp2_U8;
            branchTarget = true;
            if (s_status_c)
              goto bb_13;
bb_10:
  /*$E8FD*/ CYCLES(0xe8fd, 7);
            tmp3_U16 = (uint8_t)(0x01 + s_x);
            tmp1_U16 = ram_peek(tmp3_U16) << 0x01;
            tmp2_U8 = (uint8_t)(tmp1_U16 >> 8);
            s_status_c = tmp2_U8;
            ram_poke(tmp3_U16, ((uint8_t)tmp1_U16));
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_12;
bb_11:
  /*$E901*/ CYCLES(0xe901, 4);
            tmp1_U16 = (uint8_t)(0x01 + s_x);
            ram_poke(tmp1_U16, (uint8_t)(ram_peek(tmp1_U16) + 0x01));
bb_12:
  /*$E903*/ CYCLES(0xe903, 7);
            tmp2_U8 = s_x;
            tmp1_U16 = (uint8_t)(0x01 + tmp2_U8);
            tmp4_U8 = ram_peek(tmp1_U16);
            ram_poke(tmp1_U16, (uint8_t)((tmp4_U8 | (0x00 + (s_status_c << 8))) >> 0x01));
  /*$E905*/ tmp1_U16 = (uint8_t)(0x01 + tmp2_U8);
            tmp5_U8 = ram_peek(tmp1_U16);
            ram_poke(tmp1_U16, (uint8_t)((tmp5_U8 | (0x00 + ((tmp4_U8 & 0x01) << 8))) >> 0x01));
  /*$E907*/ CYCLES(0xe907, 18);
            tmp1_U16 = (uint8_t)(0x02 + tmp2_U8);
            tmp4_U8 = ram_peek(tmp1_U16);
            ram_poke(tmp1_U16, (uint8_t)((tmp4_U8 | (0x00 + ((tmp5_U8 & 0x01) << 8))) >> 0x01));
  /*$E909*/ tmp1_U16 = (uint8_t)(0x03 + tmp2_U8);
            tmp5_U8 = ram_peek(tmp1_U16);
            ram_poke(tmp1_U16, (uint8_t)((tmp5_U8 | (0x00 + ((tmp4_U8 & 0x01) << 8))) >> 0x01));
  /*$E90B*/ tmp1_U16 = (uint8_t)(0x04 + tmp2_U8);
            tmp2_U8 = ram_peek(tmp1_U16);
            ram_poke(tmp1_U16, (uint8_t)((tmp2_U8 | (0x00 + ((tmp5_U8 & 0x01) << 8))) >> 0x01));
  /*$E90D*/ s_a = (uint8_t)((s_a | (0x00 + ((tmp2_U8 & 0x01) << 8))) >> 0x01);
  /*$E90E*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_y = tmp2_U8;
            branchTarget = true;
            if (tmp2_U8)
              goto bb_10;
bb_13:
  /*$E911*/ CYCLES(0xe911, 4);
            s_status_c = 0x00;
  /*$E912*/ if (ret_addr) pop16(); return;
}


void FUNC_SHFTRGHT4(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint8_t tmp3_U8;
  uint8_t tmp4_U8;
  uint16_t tmp5_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E907*/ CYCLES(0xe907, 18);
            tmp1_U8 = s_x;
            tmp2_U16 = (uint8_t)(0x02 + tmp1_U8);
            tmp3_U8 = ram_peek(tmp2_U16);
            ram_poke(tmp2_U16, (uint8_t)((tmp3_U8 | (0x00 + (s_status_c << 8))) >> 0x01));
  /*$E909*/ tmp2_U16 = (uint8_t)(0x03 + tmp1_U8);
            tmp4_U8 = ram_peek(tmp2_U16);
            ram_poke(tmp2_U16, (uint8_t)((tmp4_U8 | (0x00 + ((tmp3_U8 & 0x01) << 8))) >> 0x01));
  /*$E90B*/ tmp2_U16 = (uint8_t)(0x04 + tmp1_U8);
            tmp1_U8 = ram_peek(tmp2_U16);
            ram_poke(tmp2_U16, (uint8_t)((tmp1_U8 | (0x00 + ((tmp4_U8 & 0x01) << 8))) >> 0x01));
  /*$E90D*/ s_a = (uint8_t)((s_a | (0x00 + ((tmp1_U8 & 0x01) << 8))) >> 0x01);
  /*$E90E*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_4;
bb_1:
  /*$E8FD*/ CYCLES(0xe8fd, 7);
            tmp2_U16 = (uint8_t)(0x01 + s_x);
            tmp5_U16 = ram_peek(tmp2_U16) << 0x01;
            tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
            s_status_c = tmp1_U8;
            ram_poke(tmp2_U16, ((uint8_t)tmp5_U16));
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_3;
bb_2:
  /*$E901*/ CYCLES(0xe901, 4);
            tmp5_U16 = (uint8_t)(0x01 + s_x);
            ram_poke(tmp5_U16, (uint8_t)(ram_peek(tmp5_U16) + 0x01));
bb_3:
  /*$E903*/ CYCLES(0xe903, 7);
            tmp1_U8 = s_x;
            tmp5_U16 = (uint8_t)(0x01 + tmp1_U8);
            tmp4_U8 = ram_peek(tmp5_U16);
            ram_poke(tmp5_U16, (uint8_t)((tmp4_U8 | (0x00 + (s_status_c << 8))) >> 0x01));
  /*$E905*/ tmp5_U16 = (uint8_t)(0x01 + tmp1_U8);
            tmp1_U8 = ram_peek(tmp5_U16);
            ram_poke(tmp5_U16, (uint8_t)((tmp1_U8 | (0x00 + ((tmp4_U8 & 0x01) << 8))) >> 0x01));
            s_status_c = (tmp1_U8 & 0x01);
            goto bb_0;
bb_4:
  /*$E911*/ CYCLES(0xe911, 4);
            s_status_c = 0x00;
  /*$E912*/ if (ret_addr) pop16(); return;
}


void FUNC_MULT1(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;
  uint8_t tmp4_U8;
  uint8_t tmp5_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E9B0*/ CYCLES(0xe9b0, 4);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_15;
            else
              goto bb_14;
bb_1:
  /*$E8DC*/ CYCLES(0xe8dc, 35);
            tmp1_U8 = s_x;
  /*$E8DE*/ ram_poke(0x00ac, ram_peek((uint8_t)(0x04 + tmp1_U8)));
  /*$E8E2*/ ram_poke((uint8_t)(0x04 + tmp1_U8), ram_peek((uint8_t)(0x03 + tmp1_U8)));
  /*$E8E6*/ ram_poke((uint8_t)(0x03 + tmp1_U8), ram_peek((uint8_t)(0x02 + tmp1_U8)));
  /*$E8EA*/ ram_poke((uint8_t)(0x02 + tmp1_U8), ram_peek((uint8_t)(0x01 + tmp1_U8)));
  /*$E8EE*/ ram_poke((uint8_t)(0x01 + tmp1_U8), ram_peek(0x00a4));
  /*$E8F0*/ CYCLES(0xe8f0, 7);
            if (s_status_d)
              goto bb_3;
bb_2:
  /*$E8F0*/ tmp2_U16 = (s_a + 0x0008) + s_status_c;
            s_status_c = (uint8_t)(tmp2_U16 >> 8);
            tmp1_U8 = (uint8_t)tmp2_U16;
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            goto bb_4;
bb_3:
  /*$E8F0*/ tmp3_U16 = adc_dec16(s_a, 0x08, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
            s_status_c = (tmp1_U8 & 0x01);
            s_status_not_z = (~tmp1_U8 & 2);
            s_status_n = (tmp1_U8 & 0x80);
bb_4:
            branchTarget = true;
            if (s_status_n)
              goto bb_1;
bb_5:
  /*$E8F4*/ CYCLES(0xe8f4, 4);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_1;
bb_6:
  /*$E8F6*/ CYCLES(0xe8f6, 12);
            if (s_status_d)
              goto bb_8;
bb_7:
  /*$E8F6*/ tmp2_U16 = (s_a - 0x0008) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp2_U16 >> 8) & 0x01));
            s_a = ((uint8_t)tmp2_U16);
            goto bb_9;
bb_8:
  /*$E8F6*/ tmp3_U16 = sbc_dec16(s_a, 0x08, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
bb_9:
  /*$E8F8*/ s_y = s_a;
  /*$E8F9*/ s_a = ram_peek(0x00ac);
            branchTarget = true;
            if (s_status_c)
              goto bb_13;
bb_10:
  /*$E8FD*/ CYCLES(0xe8fd, 7);
            tmp2_U16 = (uint8_t)(0x01 + s_x);
            tmp3_U16 = ram_peek(tmp2_U16) << 0x01;
            tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
            s_status_c = tmp1_U8;
            ram_poke(tmp2_U16, ((uint8_t)tmp3_U16));
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_12;
bb_11:
  /*$E901*/ CYCLES(0xe901, 4);
            tmp3_U16 = (uint8_t)(0x01 + s_x);
            ram_poke(tmp3_U16, (uint8_t)(ram_peek(tmp3_U16) + 0x01));
bb_12:
  /*$E903*/ CYCLES(0xe903, 7);
            tmp1_U8 = s_x;
            tmp3_U16 = (uint8_t)(0x01 + tmp1_U8);
            tmp4_U8 = ram_peek(tmp3_U16);
            ram_poke(tmp3_U16, (uint8_t)((tmp4_U8 | (0x00 + (s_status_c << 8))) >> 0x01));
  /*$E905*/ tmp3_U16 = (uint8_t)(0x01 + tmp1_U8);
            tmp5_U8 = ram_peek(tmp3_U16);
            ram_poke(tmp3_U16, (uint8_t)((tmp5_U8 | (0x00 + ((tmp4_U8 & 0x01) << 8))) >> 0x01));
  /*$E907*/ CYCLES(0xe907, 18);
            tmp3_U16 = (uint8_t)(0x02 + tmp1_U8);
            tmp4_U8 = ram_peek(tmp3_U16);
            ram_poke(tmp3_U16, (uint8_t)((tmp4_U8 | (0x00 + ((tmp5_U8 & 0x01) << 8))) >> 0x01));
  /*$E909*/ tmp3_U16 = (uint8_t)(0x03 + tmp1_U8);
            tmp5_U8 = ram_peek(tmp3_U16);
            ram_poke(tmp3_U16, (uint8_t)((tmp5_U8 | (0x00 + ((tmp4_U8 & 0x01) << 8))) >> 0x01));
  /*$E90B*/ tmp3_U16 = (uint8_t)(0x04 + tmp1_U8);
            tmp1_U8 = ram_peek(tmp3_U16);
            ram_poke(tmp3_U16, (uint8_t)((tmp1_U8 | (0x00 + ((tmp5_U8 & 0x01) << 8))) >> 0x01));
  /*$E90D*/ s_a = (uint8_t)((s_a | (0x00 + ((tmp1_U8 & 0x01) << 8))) >> 0x01);
  /*$E90E*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_10;
bb_13:
  /*$E911*/ CYCLES(0xe911, 4);
            s_status_c = 0x00;
  /*$E912*/ if (ret_addr) pop16(); return;
bb_14:
  /*$E9B2*/ CYCLES(0xe9b2, 6);
  /*$E8DA*/ CYCLES(0xe8da, 4);
            s_x = 0x61;
            goto bb_1;
bb_15:
  /*$E9B0*/ FUNC_MULT2(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_MULT2(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;
  uint8_t tmp5_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E9B5*/ CYCLES(0xe9b5, 6);
            tmp1_U8 = s_a;
            s_status_c = (tmp1_U8 & 0x01);
  /*$E9B6*/ s_a = ((tmp1_U8 >> 0x01) | 0x80);
bb_1:
  /*$E9B8*/ CYCLES(0xe9b8, 6);
            s_y = s_a;
            branchTarget = true;
            if (!s_status_c)
              goto bb_15;
bb_2:
  /*$E9BB*/ CYCLES(0xe9bb, 43);
            s_status_c = 0x00;
  /*$E9BC*/ s_a = ram_peek(0x0065);
            if (s_status_d)
              goto bb_4;
bb_3:
  /*$E9BE*/ tmp2_U16 = (s_a + ram_peek(0x00a9)) + s_status_c;
            s_status_c = (uint8_t)(tmp2_U16 >> 8);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_5;
bb_4:
  /*$E9BE*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x00a9), s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
bb_5:
  /*$E9C0*/ ram_poke(0x0065, s_a);
  /*$E9C2*/ s_a = ram_peek(0x0064);
            if (s_status_d)
              goto bb_7;
bb_6:
  /*$E9C4*/ tmp2_U16 = (s_a + ram_peek(0x00a8)) + s_status_c;
            s_status_c = (uint8_t)(tmp2_U16 >> 8);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_8;
bb_7:
  /*$E9C4*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x00a8), s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
bb_8:
  /*$E9C6*/ ram_poke(0x0064, s_a);
  /*$E9C8*/ s_a = ram_peek(0x0063);
            if (s_status_d)
              goto bb_10;
bb_9:
  /*$E9CA*/ tmp2_U16 = (s_a + ram_peek(0x00a7)) + s_status_c;
            s_status_c = (uint8_t)(tmp2_U16 >> 8);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_11;
bb_10:
  /*$E9CA*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x00a7), s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
bb_11:
  /*$E9CC*/ ram_poke(0x0063, s_a);
  /*$E9CE*/ s_a = ram_peek(0x0062);
            if (s_status_d)
              goto bb_13;
bb_12:
  /*$E9D0*/ tmp2_U16 = s_a;
            tmp3_U16 = ram_peek(0x00a6);
            tmp4_U16 = (tmp2_U16 + tmp3_U16) + s_status_c;
            s_status_c = (uint8_t)(tmp4_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp2_U16, (uint8_t)tmp3_U16);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_14;
bb_13:
  /*$E9D0*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x00a6), s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
            s_status_c = (tmp5_U8 & 0x01);
            s_status_v = ((tmp5_U8 & 0x40) != 0);
bb_14:
  /*$E9D2*/ ram_poke(0x0062, s_a);
bb_15:
  /*$E9D4*/ CYCLES(0xe9d4, 24);
            tmp1_U8 = ram_peek(0x0062);
            ram_poke(0x0062, (uint8_t)((tmp1_U8 | (0x00 + (s_status_c << 8))) >> 0x01));
  /*$E9D6*/ tmp5_U8 = ram_peek(0x0063);
            ram_poke(0x0063, (uint8_t)((tmp5_U8 | (0x00 + ((tmp1_U8 & 0x01) << 8))) >> 0x01));
  /*$E9D8*/ tmp1_U8 = ram_peek(0x0064);
            ram_poke(0x0064, (uint8_t)((tmp1_U8 | (0x00 + ((tmp5_U8 & 0x01) << 8))) >> 0x01));
  /*$E9DA*/ tmp5_U8 = ram_peek(0x0065);
            ram_poke(0x0065, (uint8_t)((tmp5_U8 | (0x00 + ((tmp1_U8 & 0x01) << 8))) >> 0x01));
  /*$E9DC*/ ram_poke(0x00ac, (uint8_t)((ram_peek(0x00ac) | (0x00 + ((tmp5_U8 & 0x01) << 8))) >> 0x01));
  /*$E9DE*/ tmp5_U8 = s_y;
  /*$E9DF*/ s_status_c = (tmp5_U8 & 0x01);
            tmp5_U8 = tmp5_U8 >> 0x01;
            s_status_not_z = tmp5_U8;
            s_status_n = (tmp5_U8 & 0x80);
            s_a = tmp5_U8;
            branchTarget = true;
            if (tmp5_U8)
              goto bb_1;
bb_16:
  /*$E9E2*/ CYCLES(0xe9e2, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_UPAY2ARG(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E9E3*/ CYCLES(0xe9e3, 74);
            ram_poke(0x005e, s_a);
  /*$E9E5*/ ram_poke(0x005f, s_y);
  /*$E9E9*/ tmp1_U8 = peek((ram_peek16al(0x005e) + 0x0004));
  /*$E9EB*/ ram_poke(0x00a9, tmp1_U8);
  /*$E9EE*/ tmp1_U8 = peek((ram_peek16al(0x005e) + 0x0003));
  /*$E9F0*/ ram_poke(0x00a8, tmp1_U8);
  /*$E9F3*/ tmp1_U8 = peek((ram_peek16al(0x005e) + 0x0002));
  /*$E9F5*/ ram_poke(0x00a7, tmp1_U8);
  /*$E9F8*/ tmp1_U8 = peek((ram_peek16al(0x005e) + 0x0001));
  /*$E9FA*/ ram_poke(0x00aa, tmp1_U8);
  /*$E9FE*/ ram_poke(0x00ab, (tmp1_U8 ^ ram_peek(0x00a2)));
  /*$EA04*/ ram_poke(0x00a6, (ram_peek(0x00aa) | 0x80));
  /*$EA06*/ s_y = 0x00;
  /*$EA07*/ tmp1_U8 = peek(ram_peek16al(0x005e));
  /*$EA09*/ ram_poke(0x00a5, tmp1_U8);
  /*$EA0B*/ tmp1_U8 = ram_peek(0x009d);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
  /*$EA0D*/ if (ret_addr) pop16(); return;
}


void FUNC_UPAY2FAC(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$EAF9*/ CYCLES(0xeaf9, 63);
            ram_poke(0x005e, s_a);
  /*$EAFB*/ ram_poke(0x005f, s_y);
  /*$EAFF*/ tmp1_U8 = peek((ram_peek16al(0x005e) + 0x0004));
  /*$EB01*/ ram_poke(0x00a1, tmp1_U8);
  /*$EB04*/ tmp1_U8 = peek((ram_peek16al(0x005e) + 0x0003));
  /*$EB06*/ ram_poke(0x00a0, tmp1_U8);
  /*$EB09*/ tmp1_U8 = peek((ram_peek16al(0x005e) + 0x0002));
  /*$EB0B*/ ram_poke(0x009f, tmp1_U8);
  /*$EB0E*/ tmp1_U8 = peek((ram_peek16al(0x005e) + 0x0001));
  /*$EB10*/ ram_poke(0x00a2, tmp1_U8);
  /*$EB14*/ ram_poke(0x009e, (tmp1_U8 | 0x80));
  /*$EB16*/ s_y = 0x00;
  /*$EB17*/ tmp1_U8 = peek(ram_peek16al(0x005e));
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
  /*$EB19*/ ram_poke(0x009d, tmp1_U8);
  /*$EB1B*/ ram_poke(0x00ac, 0x00);
  /*$EB1D*/ if (ret_addr) pop16(); return;
}


void FUNC_FCOMP(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;
  uint16_t tmp5_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$EBB2*/ CYCLES(0xebb2, 21);
            ram_poke(0x0060, s_a);
  /*$EBB4*/ ram_poke(0x0061, s_y);
  /*$EBB8*/ tmp1_U8 = peek(ram_peek16al(0x0060));
  /*$EBBA*/ s_y = 0x01;
  /*$EBBB*/ s_x = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_6;
bb_1:
  /*$EB82*/ CYCLES(0xeb82, 7);
            tmp1_U8 = ram_peek(0x009d);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_5;
bb_2:
  /*$EB86*/ CYCLES(0xeb86, 4);
            s_a = ram_peek(0x00a2);
bb_3:
  /*$EB88*/ CYCLES(0xeb88, 9);
            tmp1_U8 = (uint8_t)(((s_a << 0x01) | s_status_c) >> 8);
            s_status_c = tmp1_U8;
  /*$EB89*/ s_status_not_z = 0xff;
            s_status_n = 0x80;
            s_a = 0xff;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_5;
bb_4:
  /*$EB8D*/ CYCLES(0xeb8d, 4);
            s_status_not_z = 0x01;
            s_status_n = 0x00;
            s_a = 0x01;
bb_5:
  /*$EB8F*/ CYCLES(0xeb8f, 2);
            if (ret_addr) pop16(); return;
bb_6:
  /*$EBBE*/ CYCLES(0xebbe, 11);
            tmp1_U8 = peek((ram_peek16al(0x0060) + s_y));
            branchTarget = true;
            if (((tmp1_U8 ^ ram_peek(0x00a2)) & 0x80))
              goto bb_2;
bb_7:
  /*$EBC4*/ CYCLES(0xebc4, 7);
            tmp1_U8 = s_x;
            tmp2_U8 = ram_peek(0x009d);
            s_status_c = (tmp1_U8 >= tmp2_U8);
            branchTarget = true;
            if ((tmp1_U8 != tmp2_U8))
              goto bb_15;
bb_8:
  /*$EBC8*/ CYCLES(0xebc8, 14);
            tmp2_U8 = peek((ram_peek16al(0x0060) + s_y));
  /*$EBCA*/ tmp2_U8 = tmp2_U8 | 0x80;
  /*$EBCC*/ tmp1_U8 = ram_peek(0x009e);
            s_status_c = (tmp2_U8 >= tmp1_U8);
            branchTarget = true;
            if ((tmp2_U8 != tmp1_U8))
              goto bb_15;
bb_9:
  /*$EBD0*/ CYCLES(0xebd0, 12);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$EBD1*/ tmp1_U8 = peek((ram_peek16al(0x0060) + tmp1_U8));
  /*$EBD3*/ tmp2_U8 = ram_peek(0x009f);
            s_status_c = (tmp1_U8 >= tmp2_U8);
            branchTarget = true;
            if ((tmp1_U8 != tmp2_U8))
              goto bb_15;
bb_10:
  /*$EBD7*/ CYCLES(0xebd7, 12);
            tmp2_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp2_U8;
  /*$EBD8*/ tmp2_U8 = peek((ram_peek16al(0x0060) + tmp2_U8));
  /*$EBDA*/ tmp1_U8 = ram_peek(0x00a0);
            s_status_c = (tmp2_U8 >= tmp1_U8);
            branchTarget = true;
            if ((tmp2_U8 != tmp1_U8))
              goto bb_15;
bb_11:
  /*$EBDE*/ CYCLES(0xebde, 19);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$EBE1*/ s_status_c = (0x7f >= ram_peek(0x00ac));
  /*$EBE3*/ tmp1_U8 = peek((ram_peek16al(0x0060) + tmp1_U8));
            s_a = tmp1_U8;
            if (s_status_d)
              goto bb_13;
bb_12:
  /*$EBE5*/ tmp3_U16 = s_a;
            tmp4_U16 = ram_peek(0x00a1);
            tmp5_U16 = (tmp3_U16 - tmp4_U16) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp4_U16));
            tmp1_U8 = (uint8_t)tmp5_U16;
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            goto bb_14;
bb_13:
  /*$EBE5*/ tmp5_U16 = sbc_dec16(s_a, ram_peek(0x00a1), s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
            s_status_c = (tmp1_U8 & 0x01);
            s_status_not_z = (~tmp1_U8 & 2);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
            s_status_n = (tmp1_U8 & 0x80);
bb_14:
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_18;
bb_15:
  /*$EBE9*/ CYCLES(0xebe9, 7);
            s_a = ram_peek(0x00a2);
            branchTarget = true;
            if (!s_status_c)
              goto bb_17;
bb_16:
  /*$EBED*/ CYCLES(0xebed, 4);
            s_a = (s_a ^ 0xff);
bb_17:
  /*$EBEF*/ CYCLES(0xebef, 6);
            branchTarget = true;
            goto bb_3;
bb_18:
  /*$EC11*/ CYCLES(0xec11, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_QINT(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$EBF2*/ CYCLES(0xebf2, 7);
            tmp1_U8 = ram_peek(0x009d);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_9;
bb_1:
  /*$EBF6*/ CYCLES(0xebf6, 12);
            s_status_c = 0x01;
            if (s_status_d)
              goto bb_3;
bb_2:
  /*$EBF7*/ s_a = (uint8_t)((s_a - 0x00a0) - (uint8_t)(0x01 - s_status_c));
            goto bb_4;
bb_3:
  /*$EBF7*/ s_a = ((uint8_t)sbc_dec16(s_a, 0xa0, s_status_c));
bb_4:
  /*$EBF9*/ tmp1_U8 = ram_peek(0x00a2);
            s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
            branchTarget = true;
            if (!(tmp1_U8 & 0x80))
              goto bb_6;
bb_5:
  /*$EBFD*/ CYCLES(0xebfd, 14);
            tmp1_U8 = s_a;
  /*$EC00*/ ram_poke(0x00a4, 0xff);
  /*$EC02*/ FUNC_TCFACMANT(0xfffe);
  /*$EC05*/ CYCLES(0xec05, 2);
            s_a = tmp1_U8;
bb_6:
  /*$EC06*/ CYCLES(0xec06, 11);
            s_x = 0x9d;
  /*$EC08*/ tmp1_U8 = s_a;
            s_status_c = (tmp1_U8 >= 0xf9);
            branchTarget = true;
            if (!((uint8_t)(tmp1_U8 - 0xf9) & 0x80))
              goto bb_8;
bb_7:
  /*$EC0C*/ CYCLES(0xec0c, 6);
            FUNC_SHFTRGHT(0xfffe);
  /*$EC0F*/ CYCLES(0xec0f, 4);
            ram_poke(0x00a4, s_y);
  /*$EC11*/ CYCLES(0xec11, 2);
            if (ret_addr) pop16(); return;
bb_8:
  /*$EC12*/ CYCLES(0xec12, 24);
            s_y = s_a;
  /*$EC17*/ tmp1_U8 = ram_peek(0x009e);
            s_status_c = (tmp1_U8 & 0x01);
            ram_poke(0x009e, (tmp1_U8 >> 0x01));
  /*$EC19*/ tmp1_U8 = (ram_peek(0x00a2) & 0x80) | ram_peek(0x009e);
            s_a = tmp1_U8;
  /*$EC1B*/ ram_poke(0x009e, tmp1_U8);
  /*$EC1D*/ FUNC_SHFTRGHT4(0xfffe);
  /*$EC20*/ CYCLES(0xec20, 6);
            ram_poke(0x00a4, s_y);
  /*$EC22*/ if (ret_addr) pop16(); return;
bb_9:
  /*$EC40*/ CYCLES(0xec40, 18);
            tmp1_U8 = s_a;
            ram_poke(0x009e, tmp1_U8);
  /*$EC42*/ ram_poke(0x009f, tmp1_U8);
  /*$EC44*/ ram_poke(0x00a0, tmp1_U8);
  /*$EC46*/ ram_poke(0x00a1, tmp1_U8);
  /*$EC48*/ s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_y = tmp1_U8;
  /*$EC49*/ if (ret_addr) pop16(); return;
}


void FUNC_NORMAL(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$F273*/ CYCLES(0xf273, 7);
  /*$F279*/ CYCLES(0xf279, 12);
  /*$F27B*/ ram_poke(0x0032, 0xff);
  /*$F27D*/ ram_poke(0x00f3, 0x00);
  /*$F27F*/ if (ret_addr) pop16(); return;
}


void FUNC_INIT(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FB2F*/ CYCLES(0xfb2f, 29);
  /*$FB31*/ ram_poke(0x0048, 0x00);
  /*$FB33*/ tmp1_U8 = io_peek(0xc056);
  /*$FB36*/ tmp1_U8 = io_peek(0xc054);
  /*$FB39*/ tmp1_U8 = io_peek(0xc051);
  /*$FB4B*/ CYCLES(0xfb4b, 36);
            ram_poke(0x0022, 0x00);
  /*$FB4F*/ ram_poke(0x0020, 0x00);
  /*$FB53*/ ram_poke(0x0021, 0x28);
  /*$FB57*/ ram_poke(0x0023, 0x18);
  /*$FB5B*/ ram_poke(0x0025, 0x17);
  /*$FC22*/ CYCLES(0xfc22, 4);
            s_a = ram_peek(0x0025);
            FUNC_VTABZ(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_APPLEII(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FB60*/ CYCLES(0xfb60, 6);
            rom_home(0xfffe);
  /*$FB63*/ CYCLES(0xfb63, 4);
            s_y = 0x08;
bb_1:
  /*$FB65*/ CYCLES(0xfb65, 16);
            tmp1_U8 = s_y;
  /*$FB68*/ ram_poke((0x040e + tmp1_U8), ram_peek((0xfb08 + tmp1_U8)));
  /*$FB6B*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_1;
bb_2:
  /*$FB6E*/ CYCLES(0xfb6e, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_ESCOLD(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;
  uint8_t tmp4_U8;
  uint8_t tmp5_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FB97*/ CYCLES(0xfb97, 7);
            s_status_c = 0x01;
  /*$FC2C*/ CYCLES(0xfc2c, 7);
            tmp1_U8 = s_a ^ 0xc0;
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_34;
            else
              goto bb_9;
bb_1:
  /*$FBF4*/ CYCLES(0xfbf4, 14);
            ram_poke(0x0024, (uint8_t)(ram_peek(0x0024) + 0x01));
  /*$FBF6*/ tmp4_U8 = ram_peek(0x0024);
            s_a = tmp4_U8;
  /*$FBF8*/ tmp5_U8 = ram_peek(0x0021);
            s_status_not_z = (tmp4_U8 != tmp5_U8);
            tmp1_U8 = tmp4_U8 >= tmp5_U8;
            s_status_c = tmp1_U8;
            s_status_n = ((uint8_t)(tmp4_U8 - tmp5_U8) & 0x80);
            branchTarget = true;
            if (tmp1_U8)
              goto bb_30;
bb_2:
  /*$FBFC*/ CYCLES(0xfbfc, 2);
            if (ret_addr) pop16(); return;
bb_3:
  /*$FC10*/ CYCLES(0xfc10, 7);
            tmp5_U8 = (uint8_t)(ram_peek(0x0024) - 0x01);
            s_status_not_z = tmp5_U8;
            tmp4_U8 = tmp5_U8 & 0x80;
            s_status_n = tmp4_U8;
            ram_poke(0x0024, tmp5_U8);
            branchTarget = true;
            if (!tmp4_U8)
              goto bb_2;
bb_4:
  /*$FC14*/ CYCLES(0xfc14, 11);
  /*$FC16*/ ram_poke(0x0024, ram_peek(0x0021));
  /*$FC18*/ ram_poke(0x0024, (uint8_t)(ram_peek(0x0024) - 0x01));
bb_5:
  /*$FC1A*/ CYCLES(0xfc1a, 11);
            tmp1_U8 = ram_peek(0x0022);
            s_a = tmp1_U8;
  /*$FC1C*/ tmp4_U8 = ram_peek(0x0025);
            s_status_not_z = (tmp1_U8 != tmp4_U8);
            tmp5_U8 = tmp1_U8 >= tmp4_U8;
            s_status_c = tmp5_U8;
            s_status_n = ((uint8_t)(tmp1_U8 - tmp4_U8) & 0x80);
            branchTarget = true;
            if (tmp5_U8)
              goto bb_8;
bb_6:
  /*$FC20*/ CYCLES(0xfc20, 4);
            ram_poke(0x0025, (uint8_t)(ram_peek(0x0025) - 0x01));
bb_7:
  /*$FC22*/ CYCLES(0xfc22, 4);
            s_a = ram_peek(0x0025);
            FUNC_VTABZ(0x0000);
            if (ret_addr) pop16(); return;
bb_8:
  /*$FC2B*/ CYCLES(0xfc2b, 2);
            if (ret_addr) pop16(); return;
bb_9:
  /*$FC30*/ CYCLES(0xfc30, 7);
            if (s_status_d)
              goto bb_11;
bb_10:
  /*$FC30*/ tmp2_U16 = s_a;
            tmp3_U16 = (tmp2_U16 + 0x00fd) + s_status_c;
            s_status_c = (uint8_t)(tmp3_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0x00fd);
            tmp1_U8 = (uint8_t)tmp3_U16;
            s_status_not_z = tmp1_U8;
            s_a = tmp1_U8;
            goto bb_12;
bb_11:
  /*$FC30*/ tmp2_U16 = adc_dec16(s_a, 0xfd, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            tmp1_U8 = (uint8_t)(tmp2_U16 >> 8);
            s_status_c = (tmp1_U8 & 0x01);
            s_status_not_z = (~tmp1_U8 & 2);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
bb_12:
            branchTarget = true;
            if (!s_status_c)
              goto bb_1;
bb_13:
  /*$FC34*/ CYCLES(0xfc34, 4);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_3;
bb_14:
  /*$FC36*/ CYCLES(0xfc36, 7);
            if (s_status_d)
              goto bb_16;
bb_15:
  /*$FC36*/ tmp3_U16 = s_a;
            tmp2_U16 = (tmp3_U16 + 0x00fd) + s_status_c;
            s_status_c = (uint8_t)(tmp2_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp2_U16, (uint8_t)tmp3_U16, (uint8_t)0x00fd);
            tmp1_U8 = (uint8_t)tmp2_U16;
            s_status_not_z = tmp1_U8;
            s_a = tmp1_U8;
            goto bb_17;
bb_16:
  /*$FC36*/ tmp2_U16 = adc_dec16(s_a, 0xfd, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            tmp5_U8 = (uint8_t)(tmp2_U16 >> 8);
            s_status_c = (tmp5_U8 & 0x01);
            s_status_not_z = (~tmp5_U8 & 2);
            s_status_v = ((tmp5_U8 & 0x40) != 0);
bb_17:
            branchTarget = true;
            if (!s_status_c)
              goto bb_31;
bb_18:
  /*$FC3A*/ CYCLES(0xfc3a, 4);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_5;
bb_19:
  /*$FC3C*/ CYCLES(0xfc3c, 7);
            if (s_status_d)
              goto bb_21;
bb_20:
  /*$FC3C*/ tmp2_U16 = s_a;
            tmp3_U16 = (tmp2_U16 + 0x00fd) + s_status_c;
            s_status_c = (uint8_t)(tmp3_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0x00fd);
            tmp1_U8 = (uint8_t)tmp3_U16;
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            goto bb_22;
bb_21:
  /*$FC3C*/ tmp2_U16 = adc_dec16(s_a, 0xfd, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            tmp1_U8 = (uint8_t)(tmp2_U16 >> 8);
            s_status_c = (tmp1_U8 & 0x01);
            s_status_not_z = (~tmp1_U8 & 2);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
            s_status_n = (tmp1_U8 & 0x80);
bb_22:
            branchTarget = true;
            if (!s_status_c)
              goto bb_33;
bb_23:
  /*$FC40*/ CYCLES(0xfc40, 4);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_8;
bb_24:
  /*$FC42*/ CYCLES(0xfc42, 7);
            s_y = ram_peek(0x0024);
  /*$FC44*/ s_a = ram_peek(0x0025);
bb_25:
  /*$FC46*/ CYCLES(0xfc46, 7);
            push8(s_a);
  /*$FC47*/ FUNC_VTABZ(0xfffe);
  /*$FC4A*/ CYCLES(0xfc4a, 6);
            FUNC_CLREOLZ(0xfffe);
  /*$FC4D*/ CYCLES(0xfc4d, 16);
            s_y = 0x00;
  /*$FC4F*/ tmp1_U8 = pop8();
            s_a = tmp1_U8;
            if (s_status_d)
              goto bb_27;
bb_26:
  /*$FC50*/ tmp3_U16 = s_a;
            tmp2_U16 = tmp3_U16 + s_status_c;
            s_status_v = ovf8((uint8_t)tmp2_U16, (uint8_t)tmp3_U16, (uint8_t)0x0000);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_28;
bb_27:
  /*$FC50*/ tmp2_U16 = adc_dec16(s_a, 0x00, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            s_status_v = (((uint8_t)(tmp2_U16 >> 8) & 0x40) != 0);
bb_28:
  /*$FC52*/ tmp1_U8 = s_a >= ram_peek(0x0023);
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_25;
bb_29:
  /*$FC56*/ CYCLES(0xfc56, 4);
            branchTarget = true;
            if (s_status_c)
              goto bb_7;
            else
              goto bb_32;
bb_30:
  /*$FC62*/ CYCLES(0xfc62, 7);
  /*$FC64*/ ram_poke(0x0024, 0x00);
bb_31:
  /*$FC66*/ CYCLES(0xfc66, 4);
            ram_poke(0x0025, (uint8_t)(ram_peek(0x0025) + 0x01));
            rom_fc68(0x0000);
            if (ret_addr) pop16(); return;
bb_32:
  /*$FC56*/ rom_home(0x0000);
            if (ret_addr) pop16(); return;
bb_33:
  /*$FC3E*/ FUNC_CLREOL(0x0000);
            if (ret_addr) pop16(); return;
bb_34:
  /*$FC2E*/ rom_home(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_BASCALC(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FBC1*/ CYCLES(0xfbc1, 23);
            tmp1_U8 = s_a;
            push8(tmp1_U8);
  /*$FBC2*/ tmp2_U8 = tmp1_U8 & 0x01;
            s_status_c = tmp2_U8;
  /*$FBC7*/ ram_poke(0x0029, (((tmp1_U8 >> 0x01) & 0x03) | 0x04));
  /*$FBC9*/ tmp1_U8 = pop8();
  /*$FBCA*/ s_a = (tmp1_U8 & 0x18);
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_5;
bb_1:
  /*$FBCE*/ CYCLES(0xfbce, 4);
            if (s_status_d)
              goto bb_3;
bb_2:
  /*$FBCE*/ tmp3_U16 = s_a;
            tmp4_U16 = (tmp3_U16 + 0x007f) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x007f);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_4;
bb_3:
  /*$FBCE*/ tmp4_U16 = adc_dec16(s_a, 0x7f, s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
bb_4:
bb_5:
  /*$FBD0*/ CYCLES(0xfbd0, 16);
            tmp2_U8 = s_a;
            ram_poke(0x0028, tmp2_U8);
  /*$FBD3*/ tmp4_U16 = tmp2_U8 << 0x02;
            s_status_c = (uint8_t)((tmp4_U16 & 0x01ff) >> 8);
  /*$FBD4*/ tmp2_U8 = ((uint8_t)tmp4_U16) | ram_peek(0x0028);
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_a = tmp2_U8;
  /*$FBD6*/ ram_poke(0x0028, tmp2_U8);
  /*$FBD8*/ if (ret_addr) pop16(); return;
}


void FUNC_VTABZ(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;
  uint8_t tmp4_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FC24*/ CYCLES(0xfc24, 6);
            FUNC_BASCALC(0xfffe);
  /*$FC27*/ CYCLES(0xfc27, 7);
            if (s_status_d)
              goto bb_2;
bb_1:
  /*$FC27*/ tmp1_U16 = s_a;
            tmp2_U16 = ram_peek(0x0020);
            tmp3_U16 = (tmp1_U16 + tmp2_U16) + s_status_c;
            s_status_c = (uint8_t)(tmp3_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp1_U16, (uint8_t)tmp2_U16);
            tmp4_U8 = (uint8_t)tmp3_U16;
            s_status_not_z = tmp4_U8;
            s_status_n = (tmp4_U8 & 0x80);
            s_a = tmp4_U8;
            goto bb_3;
bb_2:
  /*$FC27*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x0020), s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            tmp4_U8 = (uint8_t)(tmp3_U16 >> 8);
            s_status_c = (tmp4_U8 & 0x01);
            s_status_not_z = (~tmp4_U8 & 2);
            s_status_v = ((tmp4_U8 & 0x40) != 0);
            s_status_n = (tmp4_U8 & 0x80);
bb_3:
  /*$FC29*/ ram_poke(0x0028, s_a);
  /*$FC2B*/ CYCLES(0xfc2b, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_CLREOL(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FC9C*/ CYCLES(0xfc9c, 4);
            s_y = ram_peek(0x0024);
            FUNC_CLREOLZ(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_CLREOLZ(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FC9E*/ CYCLES(0xfc9e, 4);
            s_a = 0xa0;
bb_1:
  /*$FCA0*/ CYCLES(0xfca0, 12);
            tmp1_U8 = s_y;
            poke((ram_peek16al(0x0028) + tmp1_U8), s_a);
  /*$FCA2*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
            s_y = tmp1_U8;
  /*$FCA3*/ tmp2_U8 = ram_peek(0x0021);
            s_status_not_z = (tmp1_U8 != tmp2_U8);
            tmp3_U8 = tmp1_U8 >= tmp2_U8;
            s_status_c = tmp3_U8;
            s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
            branchTarget = true;
            if (!tmp3_U8)
              goto bb_1;
bb_2:
  /*$FCA7*/ CYCLES(0xfca7, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_MON_WAIT(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FCA8*/ CYCLES(0xfca8, 2);
            s_status_c = 0x01;
bb_1:
  /*$FCA9*/ CYCLES(0xfca9, 2);
            push8(s_a);
bb_2:
  /*$FCAA*/ CYCLES(0xfcaa, 7);
            if (s_status_d)
              goto bb_4;
bb_3:
  /*$FCAA*/ tmp1_U16 = (s_a - 0x0001) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp1_U16 >> 8) & 0x01));
            tmp2_U8 = (uint8_t)tmp1_U16;
            s_status_not_z = tmp2_U8;
            s_a = tmp2_U8;
            goto bb_5;
bb_4:
  /*$FCAA*/ tmp3_U16 = sbc_dec16(s_a, 0x01, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
            s_status_c = (tmp2_U8 & 0x01);
            s_status_not_z = (~tmp2_U8 & 2);
bb_5:
            branchTarget = true;
            if (s_status_not_z)
              goto bb_2;
bb_6:
  /*$FCAE*/ CYCLES(0xfcae, 9);
            tmp2_U8 = pop8();
            s_a = tmp2_U8;
            if (s_status_d)
              goto bb_8;
bb_7:
  /*$FCAF*/ tmp1_U16 = s_a;
            tmp3_U16 = (tmp1_U16 - 0x0001) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp1_U16, (uint8_t)0xfffe);
            tmp2_U8 = (uint8_t)tmp3_U16;
            s_status_not_z = tmp2_U8;
            s_a = tmp2_U8;
            goto bb_9;
bb_8:
  /*$FCAF*/ tmp3_U16 = sbc_dec16(s_a, 0x01, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
            s_status_c = (tmp2_U8 & 0x01);
            s_status_not_z = (~tmp2_U8 & 2);
            s_status_v = ((tmp2_U8 & 0x40) != 0);
bb_9:
            branchTarget = true;
            if (s_status_not_z)
              goto bb_1;
bb_10:
  /*$FCB3*/ CYCLES(0xfcb3, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_CROUT(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FD8E*/ CYCLES(0xfd8e, 7);
            s_status_not_z = 0x8d;
            s_status_n = 0x80;
            s_a = 0x8d;
  /*$FD90*/ rom_cout(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_SETNORM(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FE84*/ CYCLES(0xfe84, 9);
            s_y = 0xff;
  /*$FE86*/ ram_poke(0x0032, 0xff);
  /*$FE88*/ if (ret_addr) pop16(); return;
}


void FUNC_BELL(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FF3A*/ CYCLES(0xff3a, 9);
            s_status_not_z = 0x87;
            s_status_n = 0x80;
            s_a = 0x87;
  /*$FF3C*/ rom_cout(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_SAV1(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FF4C*/ CYCLES(0xff4c, 23);
            ram_poke(0x0046, s_x);
  /*$FF4E*/ ram_poke(0x0047, s_y);
  /*$FF50*/ push8((s_status_c | ((s_status_not_z == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | s_status_n));
  /*$FF51*/ tmp1_U8 = pop8();
  /*$FF52*/ ram_poke(0x0048, tmp1_U8);
  /*$FF54*/ tmp1_U8 = s_sp;
            s_x = tmp1_U8;
  /*$FF55*/ ram_poke(0x0049, tmp1_U8);
  /*$FF57*/ s_status_d = 0x00;
  /*$FF58*/ if (ret_addr) pop16(); return;
}


static unsigned find_block_id_func_t001(uint16_t from_pc, uint16_t addr);

void func_t001(uint16_t ret_addr) {
  unsigned block_id = 0;
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;
  uint8_t tmp5_U8;
  uint16_t tmp6_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  for(;;) {
    switch (block_id) {
    case 0:
                s_x = 0x00;
                s_sp = 0xff;
                s_status_b = 0x00;
                s_status_i = 0x00;
      /*$FA62*/ CYCLES(0xfa62, 7);
                s_status_d = 0x00;
      /*$FA63*/ FUNC_SETNORM(0xfffe);
                branchTarget = true; block_id = 1808;
      break;
    case 1:  // $0090
      /*$0090*/ CYCLES(0x0090, 2);
                fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x0090, 0x0090, 0x03); error_handler(0x0090);
      break;
    case 2:  // $00B1
      /*$00B1*/ CYCLES(0x00b1, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x00b8) + 0x01);
                ram_poke(0x00b8, tmp1_U8);
      /*$00B3*/ branchTarget = true; block_id = tmp1_U8 ? 4 : 3;
      break;
    case 3:  // $00B5
      /*$00B5*/ CYCLES(0x00b5, 4);
                ram_poke(0x00b9, (uint8_t)(ram_peek(0x00b9) + 0x01));
                block_id = 4;
      break;
    case 4:  // $00B7
      /*$00B7*/ CYCLES(0x00b7, 12);
                tmp1_U8 = peek(ram_peek16al(0x00b8));
                s_a = tmp1_U8;
      /*$00BA*/ s_status_not_z = (tmp1_U8 != 0x3a);
                tmp2_U8 = tmp1_U8 >= 0x3a;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - 0x3a) & 0x80);
      /*$00BC*/ branchTarget = true; block_id = tmp2_U8 ? 13 : 5;
      break;
    case 5:  // $00BE
      /*$00BE*/ CYCLES(0x00be, 7);
                tmp2_U8 = s_a;
                tmp1_U8 = tmp2_U8 != 0x20;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp2_U8 >= 0x20);
                s_status_n = ((uint8_t)(tmp2_U8 - 0x20) & 0x80);
      /*$00C0*/ branchTarget = true; block_id = tmp1_U8 ? 7 : 6;
      break;
    case 6:  // $00C0
      /*$00C0*/ branchTarget = true; block_id = find_block_id_func_t001(0x00c0, (0x00c2 + ((int8_t)ram_peek(0x00c1))));
      break;
    case 7:  // $00C2
      /*$00C2*/ CYCLES(0x00c2, 11);
                s_status_c = 0x01;
      /*$00C3*/ block_id = s_status_d ? 9 : 8;
      break;
    case 8:  // $00C3
      /*$00C3*/ s_a = (uint8_t)((s_a - 0x0030) - (uint8_t)(0x01 - s_status_c));
                block_id = 10;
      break;
    case 9:  // $00C3
      /*$00C3*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x30, s_status_c));
                block_id = 10;
      break;
    case 10:  // $00C5
      /*$00C5*/ s_status_c = 0x01;
      /*$00C6*/ block_id = s_status_d ? 12 : 11;
      break;
    case 11:  // $00C6
      /*$00C6*/ tmp3_U16 = s_a;
                tmp4_U16 = (tmp3_U16 - 0x00d0) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0xff2f);
                tmp1_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 14;
      break;
    case 12:  // $00C6
      /*$00C6*/ tmp4_U16 = sbc_dec16(s_a, 0xd0, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
                block_id = 14;
      break;
    case 13:  // $00C8
      /*$00C8*/ CYCLES(0x00c8, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x00c8, pop16() + 1);;
      break;
    case 14:  // $00C8
      /*$00C6*/ block_id = 13;
      break;
    case 15:  // $3750
      /*$3750*/ CYCLES(0x3750, 4);
                s_x = 0x00;
                block_id = 16;
      break;
    case 16:  // $3752
      /*$3752*/ CYCLES(0x3752, 16);
                tmp1_U8 = s_x;
                tmp2_U8 = peek((ram_peek16(0x3753) + tmp1_U8));
      /*$3755*/ poke((ram_peek16al(0x3756) + tmp1_U8), tmp2_U8);
      /*$3758*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
                s_x = tmp1_U8;
      /*$3759*/ branchTarget = true; block_id = tmp1_U8 ? 16 : 17;
      break;
    case 17:  // $375B
      /*$375B*/ CYCLES(0x375b, 23);
                ram_poke(0x3754, (uint8_t)(ram_peek(0x3754) + 0x01));
      /*$375E*/ ram_poke(0x3757, (uint8_t)(ram_peek(0x3757) + 0x01));
      /*$3761*/ tmp1_U8 = ram_peek(0x3757);
      /*$3764*/ s_status_c = (tmp1_U8 >= 0x20);
      /*$3766*/ branchTarget = true; block_id = (tmp1_U8 != 0x20) ? 15 : 18;
      break;
    case 18:  // $3768
      /*$3768*/ CYCLES(0x3768, 6);
                rom_setvid(0xfffe);
      /*$376B*/ CYCLES(0x376b, 6);
                rom_setkbd(0xfffe);
      /*$376E*/ CYCLES(0x376e, 41);
      /*$3770*/ ram_poke(0x0300, 0x52);
      /*$3775*/ ram_poke(0x0301, 0x01);
      /*$3778*/ ram_poke(0x0302, 0x01);
      /*$377B*/ ram_poke(0x0303, 0x01);
      /*$3780*/ ram_poke(0x0304, 0x64);
      /*$3783*/ branchTarget = true; block_id = 503;
      break;
    case 19:  // $60E4
      /*$60E4*/ CYCLES(0x60e4, 6);
                func_6127(0xfffe);
                branchTarget = true; block_id = 21;
      break;
    case 20:  // $60E7
      /*$60E7*/ CYCLES(0x60e7, 28);
                s_x = 0x00;
      /*$60E9*/ tmp1_U8 = ram_peek(0x0003);
      /*$60EB*/ ram_poke(0x0007, 0x00);
      /*$60F0*/ ram_poke(0x0004, ram_peek((0x6000 + tmp1_U8)));
      /*$60F5*/ ram_poke(0x0005, ram_peek((0x6030 + tmp1_U8)));
                block_id = 22;
      break;
    case 21:  // $60E7
      /*$60E4*/ func_60e7(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x60e4, pop16() + 1);;
      break;
    case 22:  // $60F7
      /*$60F7*/ CYCLES(0x60f7, 16);
      /*$60F9*/ ram_poke(0x0006, ram_peek(0x0001));
      /*$60FE*/ ram_poke(0x0006, (uint8_t)((ram_peek(0x0006) << 0x01) | (ram_peek(0x0007) & 0x01)));
      /*$6100*/ CYCLES(0x6100, 65);
                ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) << 0x01));
      /*$6102*/ ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) << 0x01));
      /*$610A*/ ram_poke(0x0006, ((ram_peek(0x0002) & 0x03) | ram_peek(0x0006)));
      /*$6111*/ tmp1_U8 = ram_peek(0x0002);
                s_y = tmp1_U8;
      /*$6113*/ tmp2_U8 = s_x;
      /*$6116*/ poke((ram_peek16al(0x0004) + tmp1_U8), (ram_peek((0x6064 + ram_peek(0x0006))) & ram_peek((0x6060 + tmp2_U8))));
      /*$6118*/ s_x = (uint8_t)(tmp2_U8 + 0x01);
      /*$6119*/ ram_poke(0x0007, (uint8_t)(ram_peek(0x0007) + 0x01));
      /*$611B*/ s_a = ram_peek(0x0005);
      /*$611D*/ s_status_c = 0x00;
      /*$611E*/ block_id = s_status_d ? 24 : 23;
      break;
    case 23:  // $611E
      /*$611E*/ tmp4_U16 = s_a;
                tmp3_U16 = (tmp4_U16 + 0x0004) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0x0004);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 25;
      break;
    case 24:  // $611E
      /*$611E*/ tmp3_U16 = adc_dec16(s_a, 0x04, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
                block_id = 25;
      break;
    case 25:  // $6120
      /*$6120*/ ram_poke(0x0005, s_a);
      /*$6122*/ tmp2_U8 = s_x;
                tmp1_U8 = tmp2_U8 != 0x04;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp2_U8 >= 0x04);
                s_status_n = ((uint8_t)(tmp2_U8 - 0x04) & 0x80);
      /*$6124*/ branchTarget = true; block_id = tmp1_U8 ? 22 : 26;
      break;
    case 26:  // $6126
      /*$6126*/ CYCLES(0x6126, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x6126, pop16() + 1);;
      break;
    case 27:  // $6127
      /*$6127*/ CYCLES(0x6127, 57);
      /*$612A*/ tmp4_U16 = ram_peek(0x0000) << 0x02;
                tmp1_U8 = (uint8_t)tmp4_U16;
                s_status_c = (uint8_t)((tmp4_U16 & 0x01ff) >> 8);
      /*$612F*/ ram_poke(0x6060, ram_peek((0x6174 + (tmp4_U16 & 0x00ff))));
      /*$6136*/ ram_poke(0x6061, ram_peek((0x6174 + (uint8_t)(tmp1_U8 + 0x01))));
      /*$613D*/ ram_poke(0x6062, ram_peek((0x6174 + (uint8_t)(tmp1_U8 + 0x02))));
      /*$6140*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x03);
                s_x = tmp1_U8;
      /*$6141*/ tmp1_U8 = ram_peek((0x6174 + tmp1_U8));
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$6144*/ ram_poke(0x6063, tmp1_U8);
      /*$6147*/ branchTarget = true; block_id = find_block_id_func_t001(0x6147, pop16() + 1);;
      break;
    case 28:  // $6148
      /*$6148*/ CYCLES(0x6148, 6);
                func_6127(0xfffe);
                branchTarget = true; block_id = 29;
      break;
    case 29:  // $614B
      /*$614B*/ CYCLES(0x614b, 6);
                func_60e7(0xfffe);
                branchTarget = true; block_id = 30;
      break;
    case 30:  // $614E
      /*$614E*/ CYCLES(0x614e, 11);
                tmp1_U8 = ram_peek(0x0002);
                s_a = tmp1_U8;
      /*$6150*/ tmp2_U8 = ram_peek(0x0008);
                tmp5_U8 = tmp1_U8 != tmp2_U8;
                s_status_not_z = tmp5_U8;
                s_status_c = (tmp1_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
      /*$6152*/ branchTarget = true; block_id = !tmp5_U8 ? 32 : 31;
      break;
    case 31:  // $6154
      /*$6154*/ CYCLES(0x6154, 9);
                ram_poke(0x0002, (uint8_t)(ram_peek(0x0002) + 0x01));
      /*$6156*/ branchTarget = true; block_id = 29;
      break;
    case 32:  // $6159
      /*$6159*/ CYCLES(0x6159, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x6159, pop16() + 1);;
      break;
    case 33:  // $615A
      /*$615A*/ CYCLES(0x615a, 6);
                func_6127(0xfffe);
                branchTarget = true; block_id = 34;
      break;
    case 34:  // $615D
      /*$615D*/ CYCLES(0x615d, 6);
                func_60e7(0xfffe);
                branchTarget = true; block_id = 35;
      break;
    case 35:  // $6160
      /*$6160*/ CYCLES(0x6160, 11);
                tmp5_U8 = ram_peek(0x0003);
                s_a = tmp5_U8;
      /*$6162*/ tmp2_U8 = ram_peek(0x0008);
                tmp1_U8 = tmp5_U8 != tmp2_U8;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp5_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp5_U8 - tmp2_U8) & 0x80);
      /*$6164*/ branchTarget = true; block_id = !tmp1_U8 ? 32 : 36;
      break;
    case 36:  // $6166
      /*$6166*/ CYCLES(0x6166, 9);
                ram_poke(0x0003, (uint8_t)(ram_peek(0x0003) + 0x01));
      /*$6168*/ branchTarget = true; block_id = 34;
      break;
    case 37:  // $6200
      /*$6200*/ CYCLES(0x6200, 14);
                tmp1_U8 = ram_peek(0x624c);
                s_x = tmp1_U8;
      /*$6203*/ tmp1_U8 = ram_peek((0x623c + tmp1_U8));
                s_status_not_z = tmp1_U8;
                tmp2_U8 = tmp1_U8 & 0x80;
                s_status_n = tmp2_U8;
                s_a = tmp1_U8;
      /*$6206*/ branchTarget = true; block_id = !tmp2_U8 ? 39 : 38;
      break;
    case 38:  // $6208
      /*$6208*/ CYCLES(0x6208, 24);
                push8(s_a);
      /*$620B*/ tmp2_U8 = s_x;
                ram_poke((0x623c + tmp2_U8), 0x00);
      /*$620E*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_x = tmp2_U8;
      /*$6212*/ ram_poke(0x624c, (tmp2_U8 & 0x0f));
      /*$6215*/ tmp2_U8 = pop8();
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 39;
      break;
    case 39:  // $6216
      /*$6216*/ CYCLES(0x6216, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x6216, pop16() + 1);;
      break;
    case 40:  // $6217
      /*$6217*/ CYCLES(0x6217, 14);
                s_x = ram_peek(0x624d);
      /*$621A*/ tmp2_U8 = io_peek(0xc000);
                s_status_not_z = tmp2_U8;
                tmp1_U8 = tmp2_U8 & 0x80;
                s_status_n = tmp1_U8;
                s_a = tmp2_U8;
      /*$621D*/ branchTarget = true; block_id = !tmp1_U8 ? 39 : 41;
      break;
    case 41:  // $621F
      /*$621F*/ CYCLES(0x621f, 26);
                tmp1_U8 = s_a;
                io_poke(0xc010, tmp1_U8);
      /*$6222*/ tmp2_U8 = s_x;
                ram_poke((0x623c + tmp2_U8), tmp1_U8);
      /*$6225*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_x = tmp2_U8;
      /*$6227*/ tmp2_U8 = tmp2_U8 & 0x0f;
                s_a = tmp2_U8;
      /*$6229*/ tmp1_U8 = ram_peek(0x624c);
                tmp5_U8 = tmp2_U8 != tmp1_U8;
                s_status_not_z = tmp5_U8;
                s_status_c = (tmp2_U8 >= tmp1_U8);
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
      /*$622C*/ branchTarget = true; block_id = !tmp5_U8 ? 39 : 42;
      break;
    case 42:  // $622E
      /*$622E*/ CYCLES(0x622e, 7);
                ram_poke(0x624d, s_a);
      /*$6231*/ branchTarget = true; block_id = find_block_id_func_t001(0x6231, pop16() + 1);;
      break;
    case 43:  // $6256
      /*$6256*/ CYCLES(0x6256, 9);
                s_a = 0x14;
      /*$6258*/ func_660f(0xfffe);
                branchTarget = true; block_id = 44;
      break;
    case 44:  // $625B
      /*$625B*/ CYCLES(0x625b, 52);
                ram_poke(0x6251, s_a);
      /*$6260*/ ram_poke(0x6250, 0x27);
      /*$6263*/ ram_poke(0x6252, 0x27);
      /*$6268*/ ram_poke(0x624e, 0x02);
      /*$626D*/ ram_poke(0x6254, 0x0a);
      /*$6272*/ ram_poke(0x6255, 0x64);
      /*$6275*/ s_x = 0x0f;
      /*$6277*/ s_a = 0x00;
                block_id = 45;
      break;
    case 45:  // $6279
      /*$6279*/ CYCLES(0x6279, 11);
                tmp5_U8 = s_x;
                ram_poke((0x623c + tmp5_U8), s_a);
      /*$627C*/ tmp5_U8 = (uint8_t)(tmp5_U8 - 0x01);
                tmp1_U8 = tmp5_U8 & 0x80;
                s_status_n = tmp1_U8;
                s_x = tmp5_U8;
      /*$627D*/ branchTarget = true; block_id = !tmp1_U8 ? 45 : 46;
      break;
    case 46:  // $627F
      /*$627F*/ CYCLES(0x627f, 16);
                tmp1_U8 = s_a;
                ram_poke(0x624c, tmp1_U8);
      /*$6282*/ ram_poke(0x624d, tmp1_U8);
      /*$6285*/ branchTarget = true; block_id = 47;
      break;
    case 47:  // $6288
      /*$6288*/ CYCLES(0x6288, 6);
                func_69c3(0xfffe);
                branchTarget = true; block_id = 48;
      break;
    case 48:  // $628B
      /*$628B*/ CYCLES(0x628b, 6);
                func_6217(0xfffe);
                branchTarget = true; block_id = 49;
      break;
    case 49:  // $628E
      /*$628E*/ CYCLES(0x628e, 6);
                func_6c72(0xfffe);
                branchTarget = true; block_id = 50;
      break;
    case 50:  // $6291
      /*$6291*/ CYCLES(0x6291, 4);
                branchTarget = true; block_id = !s_status_n ? 66 : 51;
      break;
    case 51:  // $6293
      /*$6293*/ CYCLES(0x6293, 16);
                s_x = 0x10;
      /*$6295*/ ram_poke(0x6473, 0x10);
      /*$6298*/ tmp1_U8 = s_a != 0x95;
                s_status_not_z = tmp1_U8;
      /*$629A*/ branchTarget = true; block_id = tmp1_U8 ? 65 : 52;
      break;
    case 52:  // $629C
      /*$629C*/ CYCLES(0x629c, 16);
                s_a = ram_peek(0x624e);
      /*$629F*/ ram_poke(0x624e, (uint8_t)(ram_peek(0x624e) - 0x01));
      /*$62A2*/ s_status_c = 0x00;
      /*$62A3*/ block_id = s_status_d ? 54 : 53;
      break;
    case 53:  // $62A3
      /*$62A3*/ s_a = (uint8_t)((s_a + 0x0010) + s_status_c);
                block_id = 55;
      break;
    case 54:  // $62A3
      /*$62A3*/ s_a = ((uint8_t)adc_dec16(s_a, 0x10, s_status_c));
                block_id = 55;
      break;
    case 55:  // $62A5
      /*$62A3*/ block_id = 56;
      break;
    case 56:  // $62A5
      /*$62A5*/ CYCLES(0x62a5, 33);
                ram_poke(0x0000, s_a);
      /*$62A9*/ ram_poke(0x0001, 0x0c);
      /*$62AE*/ ram_poke(0x0002, ram_peek(0x624f));
      /*$62B3*/ ram_poke(0x0003, ram_peek(0x6250));
      /*$62B5*/ func_6bda(0xfffe);
                branchTarget = true; block_id = 57;
      break;
    case 57:  // $62B8
      /*$62B8*/ CYCLES(0x62b8, 28);
      /*$62C0*/ tmp1_U8 = (uint8_t)(((uint8_t)(ram_peek(0x624e) - 0x01) & 0x03) + 0x01);
                s_x = tmp1_U8;
      /*$62C1*/ ram_poke(0x624e, tmp1_U8);
      /*$62C4*/ s_a = tmp1_U8;
      /*$62C5*/ rom_setcol(0xfffe);
      /*$62C8*/ CYCLES(0x62c8, 16);
                s_y = ram_peek(0x624f);
      /*$62CB*/ s_a = ram_peek(0x6250);
      /*$62CE*/ rom_plot(0xfffe);
      /*$62D1*/ CYCLES(0x62d1, 50);
                s_x = ram_peek(0x624e);
      /*$62D4*/ s_a = ram_peek(0x624f);
      /*$62D7*/ s_status_c = 0x00;
      /*$62D8*/ block_id = s_status_d ? 59 : 58;
      break;
    case 58:  // $62D8
      /*$62D8*/ s_a = (uint8_t)((s_a + ram_peek((0x6232 + s_x))) + s_status_c);
                block_id = 60;
      break;
    case 59:  // $62D8
      /*$62D8*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek((0x6232 + s_x)), s_status_c));
                block_id = 60;
      break;
    case 60:  // $62DB
      /*$62DB*/ ram_poke(0x624f, s_a);
      /*$62DE*/ s_a = ram_peek(0x6250);
      /*$62E1*/ s_status_c = 0x00;
      /*$62E2*/ block_id = s_status_d ? 62 : 61;
      break;
    case 61:  // $62E2
      /*$62E2*/ tmp3_U16 = s_a;
                tmp4_U16 = ram_peek((0x6237 + s_x));
                tmp6_U16 = (tmp3_U16 + tmp4_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 63;
      break;
    case 62:  // $62E2
      /*$62E2*/ tmp6_U16 = adc_dec16(s_a, ram_peek((0x6237 + s_x)), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 63;
      break;
    case 63:  // $62E5
      /*$62E5*/ ram_poke(0x6250, s_a);
      /*$62E8*/ s_y = ram_peek(0x624f);
      /*$62EB*/ rom_scrn(0xfffe);
      /*$62EE*/ CYCLES(0x62ee, 31);
                ram_poke(0x6253, s_a);
      /*$62F4*/ ram_poke(0x0002, ram_peek(0x624f));
      /*$62F9*/ ram_poke(0x0003, ram_peek(0x6250));
      /*$62FE*/ ram_poke(0x0000, ram_peek(0x624e));
      /*$6300*/ CYCLES(0x6300, 6);
                func_60e4(0xfffe);
                branchTarget = true; block_id = 64;
      break;
    case 64:  // $6303
      /*$6303*/ CYCLES(0x6303, 6);
      /*$6474*/ CYCLES(0x6474, 9);
                tmp1_U8 = ram_peek(0x6253);
                s_a = tmp1_U8;
      /*$6477*/ branchTarget = true; block_id = tmp1_U8 ? 120 : 119;
      break;
    case 65:  // $6306
      /*$6306*/ CYCLES(0x6306, 4);
                branchTarget = true; block_id = s_status_not_z ? 74 : 66;
      break;
    case 66:  // $6308
      /*$6308*/ CYCLES(0x6308, 9);
                tmp1_U8 = ram_peek(0x0302);
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$630B*/ branchTarget = true; block_id = !tmp1_U8 ? 70 : 67;
      break;
    case 67:  // $630D
      /*$630D*/ CYCLES(0x630d, 6);
                branchTarget = true; push16(0x630f); block_id = 180;
      break;
    case 68:  // $6310
      /*$6310*/ CYCLES(0x6310, 4);
                branchTarget = true; block_id = !s_status_n ? 70 : 69;
      break;
    case 69:  // $6312
      /*$6312*/ CYCLES(0x6312, 6);
                branchTarget = true; block_id = 51;
      break;
    case 70:  // $6315
      /*$6315*/ CYCLES(0x6315, 16);
                s_a = ram_peek(0x624e);
      /*$6318*/ s_status_c = 0x00;
      /*$6319*/ block_id = s_status_d ? 72 : 71;
      break;
    case 71:  // $6319
      /*$6319*/ s_a = (uint8_t)((s_a + 0x0008) + s_status_c);
                block_id = 73;
      break;
    case 72:  // $6319
      /*$6319*/ s_a = ((uint8_t)adc_dec16(s_a, 0x08, s_status_c));
                block_id = 73;
      break;
    case 73:  // $631B
      /*$631B*/ branchTarget = true; block_id = 56;
      break;
    case 74:  // $631E
      /*$631E*/ CYCLES(0x631e, 7);
      /*$6320*/ branchTarget = true; block_id = (s_a != 0x88) ? 81 : 75;
      break;
    case 75:  // $6322
      /*$6322*/ CYCLES(0x6322, 21);
                s_a = ram_peek(0x624e);
      /*$6325*/ ram_poke(0x624e, (uint8_t)(ram_peek(0x624e) + 0x01));
      /*$6328*/ s_status_c = 0x00;
      /*$6329*/ block_id = s_status_d ? 77 : 76;
      break;
    case 76:  // $6329
      /*$6329*/ s_a = (uint8_t)((s_a + 0x0004) + s_status_c);
                block_id = 78;
      break;
    case 77:  // $6329
      /*$6329*/ s_a = ((uint8_t)adc_dec16(s_a, 0x04, s_status_c));
                block_id = 78;
      break;
    case 78:  // $632B
      /*$632B*/ branchTarget = true; block_id = 56;
      break;
    case 79:  // $6343
      /*$6343*/ CYCLES(0x6343, 9);
                tmp5_U8 = ram_peek(0x6250);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$6346*/ branchTarget = true; block_id = tmp5_U8 ? 92 : 80;
      break;
    case 80:  // $6348
      /*$6348*/ CYCLES(0x6348, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x6348, pop16() + 1);;
      break;
    case 81:  // $6349
      /*$6349*/ CYCLES(0x6349, 7);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0x92);
      /*$634B*/ branchTarget = true; block_id = (tmp5_U8 != 0x92) ? 83 : 82;
      break;
    case 82:  // $634D
      /*$634D*/ CYCLES(0x634d, 11);
                s_status_not_z = 0xff;
                s_status_n = 0x80;
                s_a = 0xff;
      /*$634F*/ ram_poke(0x6253, 0xff);
      /*$6352*/ branchTarget = true; block_id = find_block_id_func_t001(0x6352, pop16() + 1);;
      break;
    case 83:  // $6353
      /*$6353*/ CYCLES(0x6353, 7);
      /*$6355*/ branchTarget = true; block_id = (s_a != 0xc9) ? 85 : 84;
      break;
    case 84:  // $6357
      /*$6357*/ CYCLES(0x6357, 16);
      /*$635A*/ tmp5_U8 = ram_peek((0x6387 + ram_peek(0x624e)));
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$635D*/ branchTarget = true; block_id = 50;
      break;
    case 85:  // $6360
      /*$6360*/ CYCLES(0x6360, 7);
      /*$6362*/ branchTarget = true; block_id = (s_a != 0xca) ? 87 : 86;
      break;
    case 86:  // $6364
      /*$6364*/ CYCLES(0x6364, 16);
      /*$6367*/ tmp5_U8 = ram_peek((0x638c + ram_peek(0x624e)));
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$636A*/ branchTarget = true; block_id = 50;
      break;
    case 87:  // $636D
      /*$636D*/ CYCLES(0x636d, 7);
      /*$636F*/ branchTarget = true; block_id = (s_a != 0xcb) ? 89 : 88;
      break;
    case 88:  // $6371
      /*$6371*/ CYCLES(0x6371, 16);
      /*$6374*/ tmp5_U8 = ram_peek((0x6391 + ram_peek(0x624e)));
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$6377*/ branchTarget = true; block_id = 50;
      break;
    case 89:  // $637A
      /*$637A*/ CYCLES(0x637a, 7);
      /*$637C*/ branchTarget = true; block_id = (s_a != 0xcd) ? 91 : 90;
      break;
    case 90:  // $637E
      /*$637E*/ CYCLES(0x637e, 16);
      /*$6381*/ tmp5_U8 = ram_peek((0x6396 + ram_peek(0x624e)));
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$6384*/ branchTarget = true; block_id = 50;
      break;
    case 91:  // $639B
      /*$639B*/ CYCLES(0x639b, 6);
                func_69a9(0xfffe);
      /*$639E*/ CYCLES(0x639e, 6);
                branchTarget = true; block_id = 106;
      break;
    case 92:  // $63A1
      /*$63A1*/ CYCLES(0x63a1, 9);
      /*$63A4*/ branchTarget = true; block_id = !ram_peek(0x6254) ? 94 : 93;
      break;
    case 93:  // $63A6
      /*$63A6*/ CYCLES(0x63a6, 19);
                ram_poke(0x6254, (uint8_t)(ram_peek(0x6254) - 0x01));
      /*$63A9*/ s_x = 0x07;
      /*$63AB*/ ram_poke(0x6473, 0x07);
      /*$63AE*/ branchTarget = true; block_id = 106;
      break;
    case 94:  // $63B1
      /*$63B1*/ CYCLES(0x63b1, 16);
                s_a = ram_peek(0x6252);
      /*$63B4*/ s_y = ram_peek(0x6251);
      /*$63B7*/ rom_scrn(0xfffe);
      /*$63BA*/ CYCLES(0x63ba, 11);
                push8(s_a);
      /*$63BB*/ s_a = 0x00;
      /*$63BD*/ rom_setcol(0xfffe);
      /*$63C0*/ CYCLES(0x63c0, 16);
                s_a = ram_peek(0x6252);
      /*$63C3*/ s_y = ram_peek(0x6251);
      /*$63C6*/ rom_plot(0xfffe);
      /*$63C9*/ CYCLES(0x63c9, 29);
      /*$63CB*/ ram_poke(0x0001, 0x00);
      /*$63D0*/ ram_poke(0x0003, ram_peek(0x6252));
      /*$63D5*/ ram_poke(0x0002, ram_peek(0x6251));
      /*$63D7*/ func_60e4(0xfffe);
                branchTarget = true; block_id = 95;
      break;
    case 95:  // $63DA
      /*$63DA*/ CYCLES(0x63da, 48);
                tmp1_U8 = pop8();
      /*$63DB*/ s_x = tmp1_U8;
      /*$63DC*/ s_a = ram_peek(0x6251);
      /*$63DF*/ s_status_c = 0x00;
      /*$63E0*/ block_id = s_status_d ? 97 : 96;
      break;
    case 96:  // $63E0
      /*$63E0*/ s_a = (uint8_t)((s_a + ram_peek((0x6232 + s_x))) + s_status_c);
                block_id = 98;
      break;
    case 97:  // $63E0
      /*$63E0*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek((0x6232 + s_x)), s_status_c));
                block_id = 98;
      break;
    case 98:  // $63E3
      /*$63E3*/ ram_poke(0x6251, s_a);
      /*$63E6*/ s_a = ram_peek(0x6252);
      /*$63E9*/ s_status_c = 0x00;
      /*$63EA*/ block_id = s_status_d ? 100 : 99;
      break;
    case 99:  // $63EA
      /*$63EA*/ tmp6_U16 = s_a;
                tmp4_U16 = ram_peek((0x6237 + s_x));
                tmp3_U16 = (tmp6_U16 + tmp4_U16) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 101;
      break;
    case 100:  // $63EA
      /*$63EA*/ tmp3_U16 = adc_dec16(s_a, ram_peek((0x6237 + s_x)), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
                block_id = 101;
      break;
    case 101:  // $63ED
      /*$63ED*/ ram_poke(0x6252, s_a);
      /*$63F0*/ s_y = ram_peek(0x6251);
      /*$63F3*/ rom_scrn(0xfffe);
      /*$63F6*/ CYCLES(0x63f6, 38);
                s_status_c = 0x00;
      /*$63F7*/ block_id = s_status_d ? 103 : 102;
      break;
    case 102:  // $63F7
      /*$63F7*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
                block_id = 104;
      break;
    case 103:  // $63F7
      /*$63F7*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
                block_id = 104;
      break;
    case 104:  // $63F9
      /*$63F9*/ ram_poke(0x0000, s_a);
      /*$63FD*/ ram_poke(0x0001, 0x0c);
      /*$6402*/ ram_poke(0x0002, ram_peek(0x6251));
      /*$6407*/ ram_poke(0x0003, ram_peek(0x6252));
      /*$6409*/ func_60e4(0xfffe);
                branchTarget = true; block_id = 105;
      break;
    case 105:  // $640C
      /*$640C*/ CYCLES(0x640c, 6);
                branchTarget = true; block_id = 106;
      break;
    case 106:  // $640F
      /*$640F*/ CYCLES(0x640f, 23);
                tmp5_U8 = ram_peek(0x6c49);
                s_y = tmp5_U8;
      /*$6412*/ tmp5_U8 = peek((0xc000 + tmp5_U8));
      /*$6417*/ tmp5_U8 = (uint8_t)(ram_peek(0x6255) - 0x01);
                ram_poke(0x6255, tmp5_U8);
      /*$641A*/ branchTarget = true; block_id = tmp5_U8 ? 108 : 107;
      break;
    case 107:  // $641C
      /*$641C*/ CYCLES(0x641c, 11);
                s_status_not_z = 0xfe;
                s_status_n = 0x80;
                s_a = 0xfe;
      /*$641E*/ ram_poke(0x6253, 0xfe);
      /*$6421*/ branchTarget = true; block_id = find_block_id_func_t001(0x6421, pop16() + 1);;
      break;
    case 108:  // $6422
      /*$6422*/ CYCLES(0x6422, 12);
      /*$6426*/ func_6b3d(0xfffe);
                branchTarget = true; block_id = 109;
      break;
    case 109:  // $6429
      /*$6429*/ CYCLES(0x6429, 7);
      /*$642B*/ branchTarget = true; block_id = (s_a != 0x00) ? 112 : 110;
      break;
    case 110:  // $642D
      /*$642D*/ CYCLES(0x642d, 40);
      /*$642F*/ ram_poke(0x0000, 0x15);
      /*$6433*/ ram_poke(0x0001, 0x0d);
      /*$6437*/ ram_poke(0x0003, 0x27);
      /*$643B*/ ram_poke(0x0002, 0x12);
      /*$643F*/ ram_poke(0x0008, 0x16);
      /*$6441*/ func_6148(0xfffe);
                branchTarget = true; block_id = 111;
      break;
    case 111:  // $6444
      /*$6444*/ CYCLES(0x6444, 9);
                s_a = 0x0d;
      /*$6446*/ rom_setcol(0xfffe);
      /*$6449*/ CYCLES(0x6449, 12);
                s_a = 0x27;
      /*$644B*/ s_y = 0x14;
      /*$644D*/ rom_plot(0xfffe);
                branchTarget = true; block_id = 112;
      break;
    case 112:  // $6450
      /*$6450*/ CYCLES(0x6450, 6);
                s_x = ram_peek(0x0300);
                block_id = 113;
      break;
    case 113:  // $6453
      /*$6453*/ CYCLES(0x6453, 6);
                func_6bfb(0xfffe);
                branchTarget = true; block_id = 114;
      break;
    case 114:  // $6456
      /*$6456*/ CYCLES(0x6456, 9);
      /*$6457*/ push8(s_x);
      /*$6458*/ func_6217(0xfffe);
                branchTarget = true; block_id = 115;
      break;
    case 115:  // $645B
      /*$645B*/ CYCLES(0x645b, 9);
      /*$645E*/ branchTarget = true; block_id = !ram_peek(0x6473) ? 117 : 116;
      break;
    case 116:  // $6460
      /*$6460*/ CYCLES(0x6460, 19);
                tmp5_U8 = ram_peek(0x6c49);
                s_y = tmp5_U8;
      /*$6463*/ tmp5_U8 = peek((0xc000 + tmp5_U8));
      /*$6468*/ ram_poke(0x6473, (uint8_t)(ram_peek(0x6473) - 0x01));
                block_id = 117;
      break;
    case 117:  // $646B
      /*$646B*/ CYCLES(0x646b, 9);
                tmp5_U8 = pop8();
      /*$646D*/ tmp5_U8 = (uint8_t)(tmp5_U8 - 0x01);
                s_x = tmp5_U8;
      /*$646E*/ branchTarget = true; block_id = tmp5_U8 ? 113 : 118;
      break;
    case 118:  // $6470
      /*$6470*/ CYCLES(0x6470, 6);
                branchTarget = true; block_id = 48;
      break;
    case 119:  // $6479
      /*$6479*/ CYCLES(0x6479, 6);
      /*$632E*/ CYCLES(0x632e, 9);
                s_a = 0x07;
      /*$6330*/ rom_setcol(0xfffe);
      /*$6333*/ CYCLES(0x6333, 16);
                s_a = ram_peek(0x6250);
      /*$6336*/ s_y = ram_peek(0x624f);
      /*$6339*/ rom_plot(0xfffe);
      /*$633C*/ CYCLES(0x633c, 12);
                tmp5_U8 = ram_peek(0x624f);
      /*$633F*/ s_status_c = (tmp5_U8 >= 0x14);
      /*$6341*/ branchTarget = true; block_id = (tmp5_U8 != 0x14) ? 92 : 79;
      break;
    case 120:  // $647C
      /*$647C*/ CYCLES(0x647c, 7);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0x0f);
      /*$647E*/ branchTarget = true; block_id = (tmp5_U8 != 0x0f) ? 123 : 121;
      break;
    case 121:  // $6480
      /*$6480*/ CYCLES(0x6480, 18);
                s_x = 0x20;
      /*$6482*/ ram_poke(0x6473, 0x20);
      /*$6485*/ s_a = 0x07;
      /*$6487*/ rom_setcol(0xfffe);
      /*$648A*/ CYCLES(0x648a, 16);
                s_a = ram_peek(0x6250);
      /*$648D*/ tmp5_U8 = ram_peek(0x624f);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_y = tmp5_U8;
      /*$6490*/ func_6bef(0xfffe);
                branchTarget = true; block_id = 122;
      break;
    case 122:  // $6493
      /*$6493*/ CYCLES(0x6493, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x6493, pop16() + 1);;
      break;
    case 123:  // $6494
      /*$6494*/ CYCLES(0x6494, 7);
      /*$6496*/ s_x = 0xff;
                block_id = 124;
      break;
    case 124:  // $6498
      /*$6498*/ CYCLES(0x6498, 7);
      /*$649B*/ s_y = ram_peek((0xe000 + s_x));
                block_id = 125;
      break;
    case 125:  // $649C
      /*$649C*/ CYCLES(0x649c, 6);
                tmp5_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp5_U8;
      /*$649D*/ branchTarget = true; block_id = tmp5_U8 ? 125 : 126;
      break;
    case 126:  // $649F
      /*$649F*/ CYCLES(0x649f, 16);
                tmp5_U8 = ram_peek(0x6c49);
                s_y = tmp5_U8;
      /*$64A2*/ tmp5_U8 = peek((0xc000 + tmp5_U8));
                s_a = tmp5_U8;
      /*$64A5*/ tmp5_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_x = tmp5_U8;
      /*$64A6*/ branchTarget = true; block_id = tmp5_U8 ? 124 : 127;
      break;
    case 127:  // $64A8
      /*$64A8*/ CYCLES(0x64a8, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x64a8, pop16() + 1);;
      break;
    case 128:  // $64A9
      /*$64A9*/ CYCLES(0x64a9, 4);
                s_x = 0x00;
                block_id = 129;
      break;
    case 129:  // $64AB
      /*$64AB*/ CYCLES(0x64ab, 4);
      /*$64AC*/ s_y = s_x;
                block_id = 130;
      break;
    case 130:  // $64AD
      /*$64AD*/ CYCLES(0x64ad, 6);
                tmp5_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp5_U8;
      /*$64AE*/ branchTarget = true; block_id = tmp5_U8 ? 130 : 131;
      break;
    case 131:  // $64B0
      /*$64B0*/ CYCLES(0x64b0, 16);
      /*$64B3*/ tmp5_U8 = peek((0xc000 + ram_peek(0x6c49)));
      /*$64B6*/ tmp5_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp5_U8;
      /*$64B7*/ branchTarget = true; block_id = tmp5_U8 ? 129 : 132;
      break;
    case 132:  // $64B9
      /*$64B9*/ CYCLES(0x64b9, 4);
      /*$64BA*/ s_y = s_x;
                block_id = 133;
      break;
    case 133:  // $64BB
      /*$64BB*/ CYCLES(0x64bb, 6);
                tmp5_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp5_U8;
      /*$64BC*/ branchTarget = true; block_id = tmp5_U8 ? 133 : 134;
      break;
    case 134:  // $64BE
      /*$64BE*/ CYCLES(0x64be, 16);
                tmp5_U8 = ram_peek(0x6c49);
                s_y = tmp5_U8;
      /*$64C1*/ tmp5_U8 = peek((0xc000 + tmp5_U8));
                s_a = tmp5_U8;
      /*$64C4*/ tmp5_U8 = (uint8_t)(s_x + 0x01);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_x = tmp5_U8;
      /*$64C5*/ branchTarget = true; block_id = tmp5_U8 ? 132 : 135;
      break;
    case 135:  // $64C7
      /*$64C7*/ CYCLES(0x64c7, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x64c7, pop16() + 1);;
      break;
    case 136:  // $64C8
      /*$64C8*/ CYCLES(0x64c8, 18);
      /*$64CA*/ ram_poke(0x6c4a, 0x00);
      /*$64CD*/ tmp5_U8 = ram_peek(0x6634);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$64D0*/ branchTarget = true; block_id = tmp5_U8 ? 138 : 137;
      break;
    case 137:  // $64D2
      /*$64D2*/ CYCLES(0x64d2, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x64d2, pop16() + 1);;
      break;
    case 138:  // $64D3
      /*$64D3*/ CYCLES(0x64d3, 41);
                s_status_c = 0x00;
      /*$64D4*/ block_id = s_status_d ? 140 : 139;
      break;
    case 139:  // $64D4
      /*$64D4*/ s_a = (uint8_t)((s_a + ram_peek(0x6636)) + s_status_c);
                block_id = 141;
      break;
    case 140:  // $64D4
      /*$64D4*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x6636), s_status_c));
                block_id = 141;
      break;
    case 141:  // $64D7
      /*$64D7*/ ram_poke(0x6638, s_a);
      /*$64DA*/ s_a = ram_peek(0x6633);
      /*$64DD*/ s_status_c = 0x00;
      /*$64DE*/ block_id = s_status_d ? 143 : 142;
      break;
    case 142:  // $64DE
      /*$64DE*/ tmp3_U16 = s_a;
                tmp4_U16 = ram_peek(0x6635);
                tmp6_U16 = (tmp3_U16 + tmp4_U16) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 144;
      break;
    case 143:  // $64DE
      /*$64DE*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x6635), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 144;
      break;
    case 144:  // $64E1
      /*$64E1*/ tmp5_U8 = s_a;
                ram_poke(0x6637, tmp5_U8);
      /*$64E4*/ s_y = tmp5_U8;
      /*$64E5*/ s_a = ram_peek(0x6638);
      /*$64E8*/ rom_scrn(0xfffe);
      /*$64EB*/ CYCLES(0x64eb, 7);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0x00);
      /*$64ED*/ branchTarget = true; block_id = !(tmp5_U8 != 0x00) ? 151 : 145;
      break;
    case 145:  // $64EF
      /*$64EF*/ CYCLES(0x64ef, 16);
                s_y = ram_peek(0x6637);
      /*$64F2*/ s_a = ram_peek(0x6634);
      /*$64F5*/ rom_scrn(0xfffe);
      /*$64F8*/ CYCLES(0x64f8, 7);
      /*$64FA*/ branchTarget = true; block_id = !(s_a != 0x00) ? 147 : 146;
      break;
    case 146:  // $64FC
      /*$64FC*/ CYCLES(0x64fc, 29);
      /*$64FF*/ ram_poke(0x6637, ram_peek(0x6633));
      /*$6507*/ ram_poke(0x6635, (ram_peek(0x6635) ^ 0xfe));
      /*$650A*/ ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
                block_id = 147;
      break;
    case 147:  // $650D
      /*$650D*/ CYCLES(0x650d, 16);
                s_a = ram_peek(0x6638);
      /*$6510*/ s_y = ram_peek(0x6633);
      /*$6513*/ rom_scrn(0xfffe);
      /*$6516*/ CYCLES(0x6516, 7);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0x00);
      /*$6518*/ branchTarget = true; block_id = !(tmp5_U8 != 0x00) ? 149 : 148;
      break;
    case 148:  // $651A
      /*$651A*/ CYCLES(0x651a, 29);
      /*$651D*/ ram_poke(0x6638, ram_peek(0x6634));
      /*$6525*/ ram_poke(0x6636, (ram_peek(0x6636) ^ 0xfe));
      /*$6528*/ ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
                block_id = 149;
      break;
    case 149:  // $652B
      /*$652B*/ CYCLES(0x652b, 9);
      /*$652E*/ branchTarget = true; block_id = ram_peek(0x6c4a) ? 151 : 150;
      break;
    case 150:  // $6530
      /*$6530*/ CYCLES(0x6530, 48);
      /*$6533*/ ram_poke(0x6637, ram_peek(0x6633));
      /*$6539*/ ram_poke(0x6638, ram_peek(0x6634));
      /*$6541*/ ram_poke(0x6635, (ram_peek(0x6635) ^ 0xfe));
      /*$6549*/ ram_poke(0x6636, (ram_peek(0x6636) ^ 0xfe));
                block_id = 151;
      break;
    case 151:  // $654C
      /*$654C*/ CYCLES(0x654c, 12);
                s_a = 0x00;
      /*$654E*/ ram_poke(0x0001, 0x00);
      /*$6550*/ rom_setcol(0xfffe);
      /*$6553*/ CYCLES(0x6553, 23);
      /*$6556*/ ram_poke(0x0002, ram_peek(0x6633));
      /*$655B*/ ram_poke(0x0003, ram_peek(0x6634));
      /*$655D*/ func_60e4(0xfffe);
                branchTarget = true; block_id = 152;
      break;
    case 152:  // $6560
      /*$6560*/ CYCLES(0x6560, 16);
                s_a = ram_peek(0x6634);
      /*$6563*/ s_y = ram_peek(0x6633);
      /*$6566*/ rom_plot(0xfffe);
      /*$6569*/ CYCLES(0x6569, 16);
      /*$656B*/ ram_poke(0x0000, 0x1a);
      /*$656D*/ tmp1_U8 = ram_peek(0x6638);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$6570*/ branchTarget = true; block_id = tmp1_U8 ? 154 : 153;
      break;
    case 153:  // $6572
      /*$6572*/ CYCLES(0x6572, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x6572, pop16() + 1);;
      break;
    case 154:  // $6573
      /*$6573*/ CYCLES(0x6573, 35);
                tmp5_U8 = s_a;
                ram_poke(0x0003, tmp5_U8);
      /*$6575*/ ram_poke(0x6634, tmp5_U8);
      /*$6578*/ tmp5_U8 = ram_peek(0x6637);
      /*$657B*/ ram_poke(0x0002, tmp5_U8);
      /*$657D*/ ram_poke(0x6633, tmp5_U8);
      /*$6580*/ s_a = 0x03;
      /*$6582*/ ram_poke(0x0001, 0x03);
      /*$6584*/ rom_setcol(0xfffe);
      /*$6587*/ CYCLES(0x6587, 6);
                func_60e4(0xfffe);
                branchTarget = true; block_id = 155;
      break;
    case 155:  // $658A
      /*$658A*/ CYCLES(0x658a, 16);
                s_a = ram_peek(0x6634);
      /*$658D*/ tmp1_U8 = ram_peek(0x6633);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
      /*$6590*/ rom_plot(0xfffe);
      /*$6593*/ CYCLES(0x6593, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x6593, pop16() + 1);;
      break;
    case 156:  // $6594
      /*$6594*/ CYCLES(0x6594, 9);
      /*$6597*/ branchTarget = true; block_id = ram_peek(0x0301) ? 158 : 157;
      break;
    case 157:  // $6599
      /*$6599*/ CYCLES(0x6599, 6);
                branchTarget = true; block_id = 37;
      break;
    case 158:  // $659C
      /*$659C*/ CYCLES(0x659c, 46);
      /*$659F*/ ram_poke(0x6633, ram_peek(0x6639));
      /*$65A5*/ ram_poke(0x6634, ram_peek(0x663a));
      /*$65AB*/ ram_poke(0x6635, ram_peek(0x663d));
      /*$65B1*/ ram_poke(0x6636, ram_peek(0x663e));
      /*$65B4*/ func_64c8(0xfffe);
                branchTarget = true; block_id = 159;
      break;
    case 159:  // $65B7
      /*$65B7*/ CYCLES(0x65b7, 53);
      /*$65BA*/ ram_poke(0x6639, ram_peek(0x6633));
      /*$65C0*/ ram_poke(0x663a, ram_peek(0x6634));
      /*$65C6*/ ram_poke(0x663d, ram_peek(0x6635));
      /*$65CC*/ ram_poke(0x663e, ram_peek(0x6636));
      /*$65CF*/ tmp5_U8 = ram_peek(0x0301);
      /*$65D2*/ s_status_c = (tmp5_U8 >= 0x01);
      /*$65D4*/ branchTarget = true; block_id = (tmp5_U8 != 0x01) ? 161 : 160;
      break;
    case 160:  // $65D6
      /*$65D6*/ CYCLES(0x65d6, 6);
                branchTarget = true; block_id = 37;
      break;
    case 161:  // $65D9
      /*$65D9*/ CYCLES(0x65d9, 46);
      /*$65DC*/ ram_poke(0x6633, ram_peek(0x663b));
      /*$65E2*/ ram_poke(0x6634, ram_peek(0x663c));
      /*$65E8*/ ram_poke(0x6635, ram_peek(0x663f));
      /*$65EE*/ ram_poke(0x6636, ram_peek(0x6640));
      /*$65F1*/ func_64c8(0xfffe);
      /*$65F4*/ CYCLES(0x65f4, 46);
      /*$65F7*/ ram_poke(0x663b, ram_peek(0x6633));
      /*$65FD*/ ram_poke(0x663c, ram_peek(0x6634));
      /*$6603*/ ram_poke(0x663f, ram_peek(0x6635));
      /*$6609*/ ram_poke(0x6640, ram_peek(0x6636));
      /*$660C*/ branchTarget = true; block_id = 37;
      break;
    case 162:  // $660F
      /*$660F*/ CYCLES(0x660f, 62);
                ram_poke(0x624f, s_a);
      /*$6614*/ ram_poke(0x6639, 0x01);
      /*$6617*/ ram_poke(0x663a, 0x01);
      /*$661A*/ ram_poke(0x663d, 0x01);
      /*$661D*/ ram_poke(0x663e, 0x01);
      /*$6620*/ ram_poke(0x6640, 0x01);
      /*$6623*/ ram_poke(0x663c, 0x01);
      /*$6628*/ ram_poke(0x663b, 0x26);
      /*$662D*/ ram_poke(0x663f, 0xff);
      /*$6630*/ s_status_not_z = 0x14;
                s_status_n = 0x00;
                s_a = 0x14;
      /*$6632*/ branchTarget = true; block_id = find_block_id_func_t001(0x6632, pop16() + 1);;
      break;
    case 163:  // $6641
      /*$6641*/ CYCLES(0x6641, 16);
      /*$6643*/ ram_poke(0x0036, 0x4a);
      /*$6645*/ s_status_not_z = 0x66;
                s_status_n = 0x00;
                s_a = 0x66;
      /*$6647*/ ram_poke(0x0037, 0x66);
      /*$6649*/ branchTarget = true; block_id = find_block_id_func_t001(0x6649, pop16() + 1);;
      break;
    case 164:  // $69A9
      /*$69A9*/ CYCLES(0x69a9, 7);
      /*$69AB*/ branchTarget = true; block_id = (s_a != 0x9b) ? 167 : 165;
      break;
    case 165:  // $69AD
      /*$69AD*/ CYCLES(0x69ad, 9);
                tmp1_U8 = io_peek(0xc000);
                s_a = tmp1_U8;
      /*$69B0*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 165 : 166;
      break;
    case 166:  // $69B2
      /*$69B2*/ CYCLES(0x69b2, 6);
                io_poke(0xc010, s_a);
                block_id = 167;
      break;
    case 167:  // $69B5
      /*$69B5*/ CYCLES(0x69b5, 7);
                tmp5_U8 = s_a;
                tmp1_U8 = tmp5_U8 != 0x93;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp5_U8 >= 0x93);
                s_status_n = ((uint8_t)(tmp5_U8 - 0x93) & 0x80);
      /*$69B7*/ branchTarget = true; block_id = tmp1_U8 ? 169 : 168;
      break;
    case 168:  // $69B9
      /*$69B9*/ CYCLES(0x69b9, 14);
      /*$69BC*/ tmp1_U8 = ram_peek(0x69c2) ^ 0x01;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$69BE*/ ram_poke(0x69c2, tmp1_U8);
                block_id = 169;
      break;
    case 169:  // $69C1
      /*$69C1*/ CYCLES(0x69c1, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x69c1, pop16() + 1);;
      break;
    case 170:  // $69C3
      /*$69C3*/ CYCLES(0x69c3, 19);
      /*$69C6*/ ram_poke(0x6b39, ram_peek(0x624f));
      /*$69CB*/ ram_poke(0x6b3a, 0x01);
                block_id = 171;
      break;
    case 171:  // $69CE
      /*$69CE*/ CYCLES(0x69ce, 16);
                s_a = ram_peek(0x6b3a);
      /*$69D1*/ s_y = ram_peek(0x6b39);
      /*$69D4*/ rom_scrn(0xfffe);
      /*$69D7*/ CYCLES(0x69d7, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x0f);
      /*$69D9*/ branchTarget = true; block_id = !(tmp1_U8 != 0x0f) ? 179 : 172;
      break;
    case 172:  // $69DB
      /*$69DB*/ CYCLES(0x69db, 18);
                ram_poke(0x6b3a, (uint8_t)(ram_peek(0x6b3a) + 0x01));
      /*$69E3*/ branchTarget = true; block_id = (ram_peek(0x6b3a) != 0x27) ? 171 : 173;
      break;
    case 173:  // $69E5
      /*$69E5*/ CYCLES(0x69e5, 18);
      /*$69E7*/ ram_poke(0x6b3a, 0x01);
      /*$69EA*/ tmp1_U8 = (uint8_t)(ram_peek(0x6b39) - 0x01);
                ram_poke(0x6b39, tmp1_U8);
      /*$69ED*/ branchTarget = true; block_id = tmp1_U8 ? 171 : 174;
      break;
    case 174:  // $69EF
      /*$69EF*/ CYCLES(0x69ef, 11);
      /*$69F2*/ ram_poke(0x6b39, ram_peek(0x624f));
                block_id = 175;
      break;
    case 175:  // $69F5
      /*$69F5*/ CYCLES(0x69f5, 16);
                s_a = ram_peek(0x6b3a);
      /*$69F8*/ s_y = ram_peek(0x6b39);
      /*$69FB*/ rom_scrn(0xfffe);
      /*$69FE*/ CYCLES(0x69fe, 4);
                tmp1_U8 = s_a;
                tmp5_U8 = tmp1_U8 != 0x0f;
                s_status_not_z = tmp5_U8;
                s_status_c = (tmp1_U8 >= 0x0f);
      /*$6A00*/ CYCLES(0x6a00, 4);
                branchTarget = true; block_id = !tmp5_U8 ? 179 : 176;
      break;
    case 176:  // $6A02
      /*$6A02*/ CYCLES(0x6a02, 18);
                ram_poke(0x6b3a, (uint8_t)(ram_peek(0x6b3a) + 0x01));
      /*$6A0A*/ branchTarget = true; block_id = (ram_peek(0x6b3a) != 0x27) ? 175 : 177;
      break;
    case 177:  // $6A0C
      /*$6A0C*/ CYCLES(0x6a0c, 26);
      /*$6A0E*/ ram_poke(0x6b3a, 0x01);
      /*$6A11*/ ram_poke(0x6b39, (uint8_t)(ram_peek(0x6b39) + 0x01));
      /*$6A14*/ tmp5_U8 = ram_peek(0x6b39);
      /*$6A17*/ s_status_c = (tmp5_U8 >= 0x27);
      /*$6A19*/ branchTarget = true; block_id = (tmp5_U8 != 0x27) ? 175 : 178;
      break;
    case 178:  // $6A1B
      /*$6A1B*/ CYCLES(0x6a1b, 18);
      /*$6A1D*/ ram_poke(0x6b3a, 0x00);
      /*$6A22*/ ram_poke(0x6b39, 0x14);
                block_id = 179;
      break;
    case 179:  // $6A25
      /*$6A25*/ CYCLES(0x6a25, 23);
      /*$6A28*/ ram_poke(0x6b3b, ram_peek(0x6b39));
      /*$6A2B*/ tmp1_U8 = ram_peek(0x6b3a);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$6A2E*/ ram_poke(0x6b3c, tmp1_U8);
      /*$6A31*/ branchTarget = true; block_id = find_block_id_func_t001(0x6a31, pop16() + 1);;
      break;
    case 180:  // $6A32
      /*$6A32*/ CYCLES(0x6a32, 14);
                tmp1_U8 = ram_peek(0x6b3c);
      /*$6A35*/ tmp5_U8 = ram_peek(0x6250);
                s_status_c = (tmp1_U8 >= tmp5_U8);
      /*$6A38*/ branchTarget = true; block_id = !(tmp1_U8 != tmp5_U8) ? 187 : 181;
      break;
    case 181:  // $6A3A
      /*$6A3A*/ CYCLES(0x6a3a, 7);
                s_status_not_z = 0x04;
                s_a = 0x04;
      /*$6A3C*/ branchTarget = true; block_id = s_status_c ? 183 : 182;
      break;
    case 182:  // $6A3E
      /*$6A3E*/ CYCLES(0x6a3e, 4);
                s_status_not_z = 0x02;
                s_a = 0x02;
                block_id = 183;
      break;
    case 183:  // $6A40
      /*$6A40*/ CYCLES(0x6a40, 11);
                ram_poke(0x6b38, s_a);
      /*$6A43*/ func_6ab8(0xfffe);
      /*$6A46*/ CYCLES(0x6a46, 4);
                branchTarget = true; block_id = s_status_not_z ? 187 : 184;
      break;
    case 184:  // $6A48
      /*$6A48*/ CYCLES(0x6a48, 14);
                tmp5_U8 = ram_peek(0x6b38);
                s_a = tmp5_U8;
      /*$6A4B*/ tmp1_U8 = ram_peek(0x624e);
                tmp2_U8 = tmp5_U8 != tmp1_U8;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp5_U8 >= tmp1_U8);
                s_status_n = ((uint8_t)(tmp5_U8 - tmp1_U8) & 0x80);
      /*$6A4E*/ branchTarget = true; block_id = !tmp2_U8 ? 186 : 185;
      break;
    case 185:  // $6A50
      /*$6A50*/ CYCLES(0x6a50, 7);
                tmp2_U8 = s_a;
                s_x = tmp2_U8;
      /*$6A51*/ tmp2_U8 = ram_peek((0x6a55 + tmp2_U8));
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 186;
      break;
    case 186:  // $6A54
      /*$6A54*/ CYCLES(0x6a54, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x6a54, pop16() + 1);;
      break;
    case 187:  // $6A5A
      /*$6A5A*/ CYCLES(0x6a5a, 14);
      /*$6A60*/ branchTarget = true; block_id = !(ram_peek(0x6b3b) >= ram_peek(0x624f)) ? 191 : 188;
      break;
    case 188:  // $6A62
      /*$6A62*/ CYCLES(0x6a62, 14);
                s_status_not_z = 0x01;
      /*$6A64*/ ram_poke(0x6b38, 0x01);
      /*$6A67*/ func_6ab8(0xfffe);
      /*$6A6A*/ CYCLES(0x6a6a, 4);
                branchTarget = true; block_id = !s_status_not_z ? 184 : 189;
      break;
    case 189:  // $6A6C
      /*$6A6C*/ CYCLES(0x6a6c, 14);
                s_status_not_z = 0x03;
      /*$6A6E*/ ram_poke(0x6b38, 0x03);
      /*$6A71*/ func_6ab8(0xfffe);
      /*$6A74*/ CYCLES(0x6a74, 4);
                branchTarget = true; block_id = !s_status_not_z ? 184 : 190;
      break;
    case 190:  // $6A76
      /*$6A76*/ CYCLES(0x6a76, 6);
                branchTarget = true; block_id = 193;
      break;
    case 191:  // $6A79
      /*$6A79*/ CYCLES(0x6a79, 14);
                s_status_not_z = 0x03;
      /*$6A7B*/ ram_poke(0x6b38, 0x03);
      /*$6A7E*/ func_6ab8(0xfffe);
      /*$6A81*/ CYCLES(0x6a81, 4);
                branchTarget = true; block_id = !s_status_not_z ? 184 : 192;
      break;
    case 192:  // $6A83
      /*$6A83*/ CYCLES(0x6a83, 14);
                s_status_not_z = 0x01;
      /*$6A85*/ ram_poke(0x6b38, 0x01);
      /*$6A88*/ func_6ab8(0xfffe);
      /*$6A8B*/ CYCLES(0x6a8b, 4);
                branchTarget = true; block_id = !s_status_not_z ? 184 : 193;
      break;
    case 193:  // $6A8D
      /*$6A8D*/ CYCLES(0x6a8d, 14);
      /*$6A93*/ branchTarget = true; block_id = !(ram_peek(0x6b3c) >= ram_peek(0x6250)) ? 195 : 194;
      break;
    case 194:  // $6A95
      /*$6A95*/ CYCLES(0x6a95, 14);
                s_status_not_z = 0x04;
      /*$6A97*/ ram_poke(0x6b38, 0x04);
      /*$6A9A*/ func_6ab8(0xfffe);
      /*$6A9D*/ CYCLES(0x6a9d, 4);
                branchTarget = true; block_id = !s_status_not_z ? 184 : 195;
      break;
    case 195:  // $6A9F
      /*$6A9F*/ CYCLES(0x6a9f, 14);
                s_status_not_z = 0x02;
      /*$6AA1*/ ram_poke(0x6b38, 0x02);
      /*$6AA4*/ func_6ab8(0xfffe);
      /*$6AA7*/ CYCLES(0x6aa7, 4);
                branchTarget = true; block_id = !s_status_not_z ? 184 : 196;
      break;
    case 196:  // $6AA9
      /*$6AA9*/ CYCLES(0x6aa9, 14);
                s_status_not_z = 0x04;
      /*$6AAB*/ ram_poke(0x6b38, 0x04);
      /*$6AAE*/ func_6ab8(0xfffe);
      /*$6AB1*/ CYCLES(0x6ab1, 4);
                branchTarget = true; block_id = !s_status_not_z ? 184 : 197;
      break;
    case 197:  // $6AB3
      /*$6AB3*/ CYCLES(0x6ab3, 9);
                tmp2_U8 = pop8();
      /*$6AB4*/ tmp2_U8 = pop8();
      /*$6AB5*/ branchTarget = true; block_id = 70;
      break;
    case 198:  // $6AB8
      /*$6AB8*/ CYCLES(0x6ab8, 50);
                tmp5_U8 = ram_peek(0x6b38);
                s_x = tmp5_U8;
      /*$6ABB*/ s_a = ram_peek((0x6232 + tmp5_U8));
      /*$6ABE*/ s_status_c = 0x00;
      /*$6ABF*/ block_id = s_status_d ? 200 : 199;
      break;
    case 199:  // $6ABF
      /*$6ABF*/ s_a = (uint8_t)((s_a + ram_peek(0x624f)) + s_status_c);
                block_id = 201;
      break;
    case 200:  // $6ABF
      /*$6ABF*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x624f), s_status_c));
                block_id = 201;
      break;
    case 201:  // $6AC2
      /*$6AC2*/ ram_poke(0x6637, s_a);
      /*$6AC5*/ s_a = ram_peek((0x6237 + s_x));
      /*$6AC8*/ s_status_c = 0x00;
      /*$6AC9*/ block_id = s_status_d ? 203 : 202;
      break;
    case 202:  // $6AC9
      /*$6AC9*/ tmp6_U16 = s_a;
                tmp4_U16 = ram_peek(0x6250);
                tmp3_U16 = (tmp6_U16 + tmp4_U16) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 204;
      break;
    case 203:  // $6AC9
      /*$6AC9*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x6250), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
                block_id = 204;
      break;
    case 204:  // $6ACC
      /*$6ACC*/ ram_poke(0x6638, s_a);
      /*$6ACF*/ s_y = ram_peek(0x6637);
      /*$6AD2*/ rom_scrn(0xfffe);
      /*$6AD5*/ CYCLES(0x6ad5, 7);
                tmp2_U8 = s_a;
                tmp1_U8 = tmp2_U8 != 0x00;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp2_U8 >= 0x00);
                s_status_n = (tmp2_U8 & 0x80);
      /*$6AD7*/ branchTarget = true; block_id = !tmp1_U8 ? 206 : 205;
      break;
    case 205:  // $6AD9
      /*$6AD9*/ CYCLES(0x6ad9, 4);
                tmp1_U8 = s_a;
                s_status_not_z = (tmp1_U8 != 0x0f);
                s_status_c = (tmp1_U8 >= 0x0f);
                s_status_n = ((uint8_t)(tmp1_U8 - 0x0f) & 0x80);
                block_id = 206;
      break;
    case 206:  // $6ADB
      /*$6ADB*/ CYCLES(0x6adb, 4);
                branchTarget = true; block_id = !s_status_not_z ? 208 : 207;
      break;
    case 207:  // $6ADD
      /*$6ADD*/ CYCLES(0x6add, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x6add, pop16() + 1);;
      break;
    case 208:  // $6ADE
      /*$6ADE*/ CYCLES(0x6ade, 18);
      /*$6AE0*/ ram_poke(0x6c4a, 0x00);
      /*$6AE3*/ tmp1_U8 = ram_peek(0x6638);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$6AE6*/ branchTarget = true; block_id = !tmp1_U8 ? 207 : 209;
      break;
    case 209:  // $6AE8
      /*$6AE8*/ CYCLES(0x6ae8, 12);
      /*$6AEB*/ s_y = (uint8_t)(ram_peek(0x6637) + 0x01);
      /*$6AEC*/ rom_scrn(0xfffe);
      /*$6AEF*/ CYCLES(0x6aef, 7);
      /*$6AF1*/ branchTarget = true; block_id = (s_a != 0x00) ? 211 : 210;
      break;
    case 210:  // $6AF3
      /*$6AF3*/ CYCLES(0x6af3, 6);
                ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
                block_id = 211;
      break;
    case 211:  // $6AF6
      /*$6AF6*/ CYCLES(0x6af6, 18);
                s_a = ram_peek(0x6638);
      /*$6AFC*/ s_y = (uint8_t)(ram_peek(0x6637) - 0x01);
      /*$6AFD*/ rom_scrn(0xfffe);
      /*$6B00*/ CYCLES(0x6b00, 7);
      /*$6B02*/ branchTarget = true; block_id = (s_a != 0x00) ? 213 : 212;
      break;
    case 212:  // $6B04
      /*$6B04*/ CYCLES(0x6b04, 6);
                ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
                block_id = 213;
      break;
    case 213:  // $6B07
      /*$6B07*/ CYCLES(0x6b07, 21);
                s_a = ram_peek(0x6638);
      /*$6B0A*/ s_status_c = 0x00;
      /*$6B0B*/ block_id = s_status_d ? 215 : 214;
      break;
    case 214:  // $6B0B
      /*$6B0B*/ tmp3_U16 = s_a;
                tmp4_U16 = (tmp3_U16 + 0x0001) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x0001);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 216;
      break;
    case 215:  // $6B0B
      /*$6B0B*/ tmp4_U16 = adc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 216;
      break;
    case 216:  // $6B0D
      /*$6B0D*/ s_y = ram_peek(0x6637);
      /*$6B10*/ rom_scrn(0xfffe);
      /*$6B13*/ CYCLES(0x6b13, 7);
      /*$6B15*/ branchTarget = true; block_id = (s_a != 0x00) ? 218 : 217;
      break;
    case 217:  // $6B17
      /*$6B17*/ CYCLES(0x6b17, 6);
                ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
                block_id = 218;
      break;
    case 218:  // $6B1A
      /*$6B1A*/ CYCLES(0x6b1a, 21);
                s_a = ram_peek(0x6638);
      /*$6B1D*/ s_status_c = 0x01;
      /*$6B1E*/ block_id = s_status_d ? 220 : 219;
      break;
    case 219:  // $6B1E
      /*$6B1E*/ tmp4_U16 = s_a;
                tmp3_U16 = (tmp4_U16 - 0x0001) - (uint8_t)(0x01 - s_status_c);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0xfffe);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 221;
      break;
    case 220:  // $6B1E
      /*$6B1E*/ tmp3_U16 = sbc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
                block_id = 221;
      break;
    case 221:  // $6B20
      /*$6B20*/ s_y = ram_peek(0x6637);
      /*$6B23*/ rom_scrn(0xfffe);
      /*$6B26*/ CYCLES(0x6b26, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x00);
      /*$6B28*/ branchTarget = true; block_id = (tmp1_U8 != 0x00) ? 223 : 222;
      break;
    case 222:  // $6B2A
      /*$6B2A*/ CYCLES(0x6b2a, 6);
                ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
                block_id = 223;
      break;
    case 223:  // $6B2D
      /*$6B2D*/ CYCLES(0x6b2d, 9);
      /*$6B30*/ branchTarget = true; block_id = !ram_peek(0x6c4a) ? 225 : 224;
      break;
    case 224:  // $6B32
      /*$6B32*/ CYCLES(0x6b32, 6);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$6B34*/ branchTarget = true; block_id = find_block_id_func_t001(0x6b34, pop16() + 1);;
      break;
    case 225:  // $6B35
      /*$6B35*/ CYCLES(0x6b35, 6);
                s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_a = 0x01;
      /*$6B37*/ branchTarget = true; block_id = find_block_id_func_t001(0x6b37, pop16() + 1);;
      break;
    case 226:  // $6B3D
      /*$6B3D*/ CYCLES(0x6b3d, 6);
                func_6c4b(0xfffe);
                branchTarget = true; block_id = 227;
      break;
    case 227:  // $6B40
      /*$6B40*/ CYCLES(0x6b40, 36);
      /*$6B42*/ ram_poke(0x0000, 0x15);
      /*$6B46*/ ram_poke(0x0001, 0x02);
      /*$6B4A*/ ram_poke(0x0002, 0x00);
      /*$6B4E*/ ram_poke(0x0003, 0x01);
      /*$6B50*/ tmp1_U8 = ram_peek(0x6255);
                s_a = tmp1_U8;
      /*$6B53*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 229 : 228;
      break;
    case 228:  // $6B55
      /*$6B55*/ CYCLES(0x6b55, 12);
      /*$6B57*/ ram_poke(0x6255, 0xff);
      /*$6B5A*/ s_a = 0x70;
                block_id = 229;
      break;
    case 229:  // $6B5C
      /*$6B5C*/ CYCLES(0x6b5c, 16);
      /*$6B5E*/ ram_poke(0x0008, (s_a >> 0x02));
      /*$6B60*/ ram_poke(0x0008, (uint8_t)(ram_peek(0x0008) + 0x01));
      /*$6B62*/ func_615a(0xfffe);
                branchTarget = true; block_id = 230;
      break;
    case 230:  // $6B65
      /*$6B65*/ CYCLES(0x6b65, 19);
      /*$6B67*/ ram_poke(0x0002, 0x27);
      /*$6B6B*/ ram_poke(0x0003, 0x01);
      /*$6B6D*/ func_615a(0xfffe);
                branchTarget = true; block_id = 231;
      break;
    case 231:  // $6B70
      /*$6B70*/ CYCLES(0x6b70, 31);
                ram_poke(0x0008, (uint8_t)(ram_peek(0x0008) + 0x01));
      /*$6B72*/ tmp1_U8 = ram_peek(0x0008);
      /*$6B74*/ push8(tmp1_U8);
      /*$6B75*/ ram_poke(0x0003, tmp1_U8);
      /*$6B79*/ ram_poke(0x0001, 0x0d);
      /*$6B7D*/ ram_poke(0x0008, 0x27);
      /*$6B7F*/ func_615a(0xfffe);
                branchTarget = true; block_id = 232;
      break;
    case 232:  // $6B82
      /*$6B82*/ CYCLES(0x6b82, 18);
                tmp1_U8 = pop8();
      /*$6B83*/ ram_poke(0x0003, tmp1_U8);
      /*$6B87*/ ram_poke(0x0002, 0x00);
      /*$6B89*/ func_615a(0xfffe);
                branchTarget = true; block_id = 233;
      break;
    case 233:  // $6B8C
      /*$6B8C*/ CYCLES(0x6b8c, 12);
                s_a = 0x27;
      /*$6B8E*/ s_y = 0x14;
      /*$6B90*/ rom_scrn(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x6b90, pop16() + 1);;
      break;
    case 234:  // $6B93
      /*$6B93*/ CYCLES(0x6b93, 6);
                func_6127(0xfffe);
                branchTarget = true; block_id = 235;
      break;
    case 235:  // $6B96
      /*$6B96*/ CYCLES(0x6b96, 28);
                s_x = 0x00;
      /*$6B98*/ tmp1_U8 = ram_peek(0x0003);
      /*$6B9A*/ ram_poke(0x0007, 0x00);
      /*$6B9F*/ ram_poke(0x0004, ram_peek((0x6000 + tmp1_U8)));
      /*$6BA4*/ ram_poke(0x0005, ram_peek((0x6030 + tmp1_U8)));
                block_id = 236;
      break;
    case 236:  // $6BA6
      /*$6BA6*/ CYCLES(0x6ba6, 87);
      /*$6BA8*/ ram_poke(0x0006, ram_peek(0x0001));
      /*$6BAD*/ ram_poke(0x0006, (uint8_t)((ram_peek(0x0006) | (0x00 + ((ram_peek(0x0007) & 0x01) << 8))) >> 0x01));
      /*$6BAF*/ ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) << 0x01));
      /*$6BB1*/ ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) << 0x01));
      /*$6BB9*/ ram_poke(0x0006, ((ram_peek(0x0002) & 0x03) | ram_peek(0x0006)));
      /*$6BC2*/ tmp1_U8 = ram_peek(0x0002);
                s_y = tmp1_U8;
      /*$6BC4*/ tmp2_U8 = s_x;
      /*$6BC7*/ tmp5_U8 = peek((ram_peek16al(0x0004) + tmp1_U8));
      /*$6BC9*/ poke((ram_peek16al(0x0004) + tmp1_U8), (((ram_peek((0x6064 + ram_peek(0x0006))) ^ 0x7f) & ram_peek((0x6060 + tmp2_U8))) | tmp5_U8));
      /*$6BCB*/ s_x = (uint8_t)(tmp2_U8 + 0x01);
      /*$6BCC*/ ram_poke(0x0007, (uint8_t)(ram_peek(0x0007) + 0x01));
      /*$6BCE*/ s_a = ram_peek(0x0005);
      /*$6BD0*/ s_status_c = 0x00;
      /*$6BD1*/ block_id = s_status_d ? 238 : 237;
      break;
    case 237:  // $6BD1
      /*$6BD1*/ tmp3_U16 = s_a;
                tmp4_U16 = (tmp3_U16 + 0x0004) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x0004);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 239;
      break;
    case 238:  // $6BD1
      /*$6BD1*/ tmp4_U16 = adc_dec16(s_a, 0x04, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 239;
      break;
    case 239:  // $6BD3
      /*$6BD3*/ ram_poke(0x0005, s_a);
      /*$6BD5*/ tmp2_U8 = s_x;
                tmp1_U8 = tmp2_U8 != 0x04;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp2_U8 >= 0x04);
                s_status_n = ((uint8_t)(tmp2_U8 - 0x04) & 0x80);
      /*$6BD7*/ branchTarget = true; block_id = tmp1_U8 ? 236 : 240;
      break;
    case 240:  // $6BD9
      /*$6BD9*/ CYCLES(0x6bd9, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x6bd9, pop16() + 1);;
      break;
    case 241:  // $6BDA
      /*$6BDA*/ CYCLES(0x6bda, 6);
                func_60e4(0xfffe);
                branchTarget = true; block_id = 242;
      break;
    case 242:  // $6BDD
      /*$6BDD*/ CYCLES(0x6bdd, 9);
      /*$6BE0*/ branchTarget = true; block_id = !ram_peek(0x0305) ? 244 : 243;
      break;
    case 243:  // $6BE2
      /*$6BE2*/ CYCLES(0x6be2, 12);
      /*$6BE4*/ ram_poke(0x0000, 0x01);
      /*$6BE6*/ func_6b93(0xfffe);
                branchTarget = true; block_id = 244;
      break;
    case 244:  // $6BE9
      /*$6BE9*/ CYCLES(0x6be9, 11);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$6BEB*/ ram_poke(0x0305, 0x00);
      /*$6BEE*/ branchTarget = true; block_id = find_block_id_func_t001(0x6bee, pop16() + 1);;
      break;
    case 245:  // $6BEF
      /*$6BEF*/ CYCLES(0x6bef, 6);
                rom_plot(0xfffe);
      /*$6BF2*/ CYCLES(0x6bf2, 16);
                s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_a = 0x01;
      /*$6BF4*/ ram_poke(0x0305, 0x01);
      /*$6BF7*/ ram_poke(0x6c46, 0x01);
      /*$6BFA*/ branchTarget = true; block_id = find_block_id_func_t001(0x6bfa, pop16() + 1);;
      break;
    case 246:  // $6BFB
      /*$6BFB*/ CYCLES(0x6bfb, 9);
      /*$6BFD*/ ram_poke(0x6c48, 0x14);
                block_id = 247;
      break;
    case 247:  // $6C00
      /*$6C00*/ CYCLES(0x6c00, 9);
                tmp1_U8 = ram_peek(0x6c46);
                s_a = tmp1_U8;
      /*$6C03*/ branchTarget = true; block_id = !tmp1_U8 ? 251 : 248;
      break;
    case 248:  // $6C05
      /*$6C05*/ CYCLES(0x6c05, 7);
      /*$6C07*/ branchTarget = true; block_id = (s_a >= 0x80) ? 251 : 249;
      break;
    case 249:  // $6C09
      /*$6C09*/ CYCLES(0x6c09, 9);
                tmp1_U8 = (uint8_t)(ram_peek(0x6c47) - 0x01);
                ram_poke(0x6c47, tmp1_U8);
      /*$6C0C*/ branchTarget = true; block_id = tmp1_U8 ? 251 : 250;
      break;
    case 250:  // $6C0E
      /*$6C0E*/ CYCLES(0x6c0e, 31);
                tmp1_U8 = ram_peek(0x6c49);
                s_y = tmp1_U8;
      /*$6C11*/ tmp1_U8 = peek((0xc000 + tmp1_U8));
      /*$6C14*/ ram_poke(0x6c46, (uint8_t)(ram_peek(0x6c46) + 0x01));
      /*$6C17*/ ram_poke(0x6c46, (uint8_t)(ram_peek(0x6c46) + 0x01));
      /*$6C1D*/ ram_poke(0x6c47, ram_peek(0x6c46));
                block_id = 251;
      break;
    case 251:  // $6C20
      /*$6C20*/ CYCLES(0x6c20, 12);
      /*$6C23*/ tmp1_U8 = ram_peek(0x6c46) >= 0x80;
                s_status_c = tmp1_U8;
      /*$6C25*/ branchTarget = true; block_id = !tmp1_U8 ? 253 : 252;
      break;
    case 252:  // $6C27
      /*$6C27*/ CYCLES(0x6c27, 9);
      /*$6C29*/ ram_poke(0x6c46, 0x00);
                block_id = 253;
      break;
    case 253:  // $6C2C
      /*$6C2C*/ CYCLES(0x6c2c, 18);
      /*$6C2E*/ ram_poke(0x6c49, 0x20);
      /*$6C31*/ tmp1_U8 = ram_peek(0x0302);
                s_a = tmp1_U8;
      /*$6C34*/ branchTarget = true; block_id = tmp1_U8 ? 256 : 254;
      break;
    case 254:  // $6C36
      /*$6C36*/ CYCLES(0x6c36, 9);
                tmp1_U8 = ram_peek(0x69c2);
                s_a = tmp1_U8;
      /*$6C39*/ branchTarget = true; block_id = tmp1_U8 ? 256 : 255;
      break;
    case 255:  // $6C3B
      /*$6C3B*/ CYCLES(0x6c3b, 9);
                s_a = 0x30;
      /*$6C3D*/ ram_poke(0x6c49, 0x30);
                block_id = 256;
      break;
    case 256:  // $6C40
      /*$6C40*/ CYCLES(0x6c40, 9);
                tmp1_U8 = (uint8_t)(ram_peek(0x6c48) - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x6c48, tmp1_U8);
      /*$6C43*/ branchTarget = true; block_id = tmp1_U8 ? 247 : 257;
      break;
    case 257:  // $6C45
      /*$6C45*/ CYCLES(0x6c45, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x6c45, pop16() + 1);;
      break;
    case 258:  // $6C4B
      /*$6C4B*/ CYCLES(0x6c4b, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x000e) + 0x01);
                ram_poke(0x000e, tmp1_U8);
      /*$6C4D*/ branchTarget = true; block_id = tmp1_U8 ? 260 : 259;
      break;
    case 259:  // $6C4F
      /*$6C4F*/ CYCLES(0x6c4f, 4);
                ram_poke(0x000f, (uint8_t)(ram_peek(0x000f) + 0x01));
                block_id = 260;
      break;
    case 260:  // $6C51
      /*$6C51*/ CYCLES(0x6c51, 11);
                s_y = 0x00;
      /*$6C53*/ tmp1_U8 = peek(ram_peek16al(0x000e));
                s_status_not_z = tmp1_U8;
                tmp2_U8 = tmp1_U8 & 0x80;
                s_status_n = tmp2_U8;
                s_a = tmp1_U8;
      /*$6C55*/ branchTarget = true; block_id = !tmp2_U8 ? 262 : 261;
      break;
    case 261:  // $6C57
      /*$6C57*/ CYCLES(0x6c57, 19);
      /*$6C59*/ ram_poke(0x000e, 0x00);
      /*$6C5D*/ ram_poke(0x000f, 0x18);
      /*$6C5F*/ branchTarget = true; block_id = 260;
      break;
    case 262:  // $6C62
      /*$6C62*/ CYCLES(0x6c62, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x6c62, pop16() + 1);;
      break;
    case 263:  // $6C72
      /*$6C72*/ CYCLES(0x6c72, 6);
                func_6594(0xfffe);
                branchTarget = true; block_id = 264;
      break;
    case 264:  // $6C75
      /*$6C75*/ CYCLES(0x6c75, 11);
                push8(s_a);
      /*$6C79*/ branchTarget = true; block_id = !ram_peek(0x0302) ? 271 : 265;
      break;
    case 265:  // $6C7B
      /*$6C7B*/ CYCLES(0x6c7b, 9);
      /*$6C7E*/ branchTarget = true; block_id = !ram_peek(0x6c71) ? 268 : 266;
      break;
    case 266:  // $6C80
      /*$6C80*/ CYCLES(0x6c80, 9);
                tmp1_U8 = io_peek(0xc061);
      /*$6C83*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 268 : 267;
      break;
    case 267:  // $6C85
      /*$6C85*/ CYCLES(0x6c85, 7);
                tmp1_U8 = pop8();
      /*$6C86*/ s_status_not_z = 0x92;
                s_status_n = 0x80;
                s_a = 0x92;
      /*$6C88*/ branchTarget = true; block_id = find_block_id_func_t001(0x6c88, pop16() + 1);;
      break;
    case 268:  // $6C89
      /*$6C89*/ CYCLES(0x6c89, 6);
                tmp2_U8 = pop8();
                s_status_not_z = tmp2_U8;
                tmp1_U8 = tmp2_U8 & 0x80;
                s_status_n = tmp1_U8;
                s_a = tmp2_U8;
      /*$6C8A*/ branchTarget = true; block_id = !tmp1_U8 ? 270 : 269;
      break;
    case 269:  // $6C8C
      /*$6C8C*/ CYCLES(0x6c8c, 6);
                s_status_not_z = 0x92;
                s_status_n = 0x80;
                s_a = 0x92;
      /*$6C8E*/ branchTarget = true; block_id = find_block_id_func_t001(0x6c8e, pop16() + 1);;
      break;
    case 270:  // $6C8F
      /*$6C8F*/ CYCLES(0x6c8f, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x6c8f, pop16() + 1);;
      break;
    case 271:  // $6C90
      /*$6C90*/ CYCLES(0x6c90, 6);
                tmp2_U8 = pop8();
                s_a = tmp2_U8;
      /*$6C91*/ s_x = 0x05;
                block_id = 272;
      break;
    case 272:  // $6C93
      /*$6C93*/ CYCLES(0x6c93, 9);
      /*$6C96*/ branchTarget = true; block_id = !(s_a != ram_peek((0x6c63 + s_x))) ? 275 : 273;
      break;
    case 273:  // $6C98
      /*$6C98*/ CYCLES(0x6c98, 6);
                tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp1_U8;
      /*$6C99*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 272 : 274;
      break;
    case 274:  // $6C9B
      /*$6C9B*/ CYCLES(0x6c9b, 6);
                branchTarget = true; block_id = 276;
      break;
    case 275:  // $6C9E
      /*$6C9E*/ CYCLES(0x6c9e, 6);
                s_a = ram_peek((0x6c6a + s_x));
                block_id = 276;
      break;
    case 276:  // $6CA1
      /*$6CA1*/ CYCLES(0x6ca1, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x80);
      /*$6CA3*/ branchTarget = true; block_id = (tmp1_U8 != 0x80) ? 278 : 277;
      break;
    case 277:  // $6CA5
      /*$6CA5*/ CYCLES(0x6ca5, 11);
                s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_a = 0x01;
      /*$6CA7*/ ram_poke(0x6c71, 0x01);
      /*$6CAA*/ branchTarget = true; block_id = find_block_id_func_t001(0x6caa, pop16() + 1);;
      break;
    case 278:  // $6CAB
      /*$6CAB*/ CYCLES(0x6cab, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x8b);
      /*$6CAD*/ branchTarget = true; block_id = (tmp1_U8 != 0x8b) ? 280 : 279;
      break;
    case 279:  // $6CAF
      /*$6CAF*/ CYCLES(0x6caf, 11);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$6CB1*/ ram_poke(0x6c71, 0x00);
      /*$6CB4*/ branchTarget = true; block_id = find_block_id_func_t001(0x6cb4, pop16() + 1);;
      break;
    case 280:  // $6CB5
      /*$6CB5*/ CYCLES(0x6cb5, 7);
                tmp1_U8 = s_a;
                s_status_not_z = (tmp1_U8 != 0x00);
                s_status_c = (tmp1_U8 >= 0x00);
                tmp1_U8 = tmp1_U8 & 0x80;
                s_status_n = tmp1_U8;
      /*$6CB7*/ branchTarget = true; block_id = !tmp1_U8 ? 282 : 281;
      break;
    case 281:  // $6CB9
      /*$6CB9*/ CYCLES(0x6cb9, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x6cb9, pop16() + 1);;
      break;
    case 282:  // $6CBA
      /*$6CBA*/ CYCLES(0x6cba, 9);
                tmp1_U8 = ram_peek(0x6c71);
                s_x = tmp1_U8;
      /*$6CBD*/ branchTarget = true; block_id = tmp1_U8 ? 284 : 283;
      break;
    case 283:  // $6CBF
      /*$6CBF*/ CYCLES(0x6cbf, 6);
                tmp1_U8 = s_a;
                s_status_not_z = (tmp1_U8 != 0x00);
                s_status_c = (tmp1_U8 >= 0x00);
                s_status_n = (tmp1_U8 & 0x80);
      /*$6CC1*/ branchTarget = true; block_id = find_block_id_func_t001(0x6cc1, pop16() + 1);;
      break;
    case 284:  // $6CC2
      /*$6CC2*/ CYCLES(0x6cc2, 18);
                s_y = 0x00;
      /*$6CC4*/ tmp1_U8 = io_peek(0xc05b);
      /*$6CC7*/ tmp1_U8 = io_peek(0xc062);
      /*$6CCA*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 286 : 285;
      break;
    case 285:  // $6CCC
      /*$6CCC*/ CYCLES(0x6ccc, 6);
                s_x = 0x00;
      /*$6CCE*/ s_y = (uint8_t)(s_y + 0x01);
                block_id = 286;
      break;
    case 286:  // $6CCF
      /*$6CCF*/ CYCLES(0x6ccf, 9);
                tmp1_U8 = io_peek(0xc063);
      /*$6CD2*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 288 : 287;
      break;
    case 287:  // $6CD4
      /*$6CD4*/ CYCLES(0x6cd4, 6);
                s_x = 0x03;
      /*$6CD6*/ s_y = (uint8_t)(s_y + 0x01);
                block_id = 288;
      break;
    case 288:  // $6CD7
      /*$6CD7*/ CYCLES(0x6cd7, 14);
                tmp1_U8 = io_peek(0xc05a);
      /*$6CDA*/ tmp1_U8 = io_peek(0xc062);
      /*$6CDD*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 290 : 289;
      break;
    case 289:  // $6CDF
      /*$6CDF*/ CYCLES(0x6cdf, 6);
                s_x = 0x01;
      /*$6CE1*/ s_y = (uint8_t)(s_y + 0x01);
                block_id = 290;
      break;
    case 290:  // $6CE2
      /*$6CE2*/ CYCLES(0x6ce2, 9);
                tmp1_U8 = io_peek(0xc063);
      /*$6CE5*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 292 : 291;
      break;
    case 291:  // $6CE7
      /*$6CE7*/ CYCLES(0x6ce7, 6);
                s_x = 0x02;
      /*$6CE9*/ s_y = (uint8_t)(s_y + 0x01);
                block_id = 292;
      break;
    case 292:  // $6CEA
      /*$6CEA*/ CYCLES(0x6cea, 7);
                tmp1_U8 = s_y;
                s_status_c = (tmp1_U8 >= 0x01);
      /*$6CEC*/ branchTarget = true; block_id = !(tmp1_U8 != 0x01) ? 294 : 293;
      break;
    case 293:  // $6CEE
      /*$6CEE*/ CYCLES(0x6cee, 6);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$6CF0*/ branchTarget = true; block_id = find_block_id_func_t001(0x6cf0, pop16() + 1);;
      break;
    case 294:  // $6CF1
      /*$6CF1*/ CYCLES(0x6cf1, 7);
                tmp1_U8 = ram_peek((0x6c6a + s_x));
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$6CF4*/ branchTarget = true; block_id = find_block_id_func_t001(0x6cf4, pop16() + 1);;
      break;
    case 295:  // $7000
      /*$7000*/ CYCLES(0x7000, 6);
      /*$7002*/ push8(ram_peek(0x0003));
                block_id = 296;
      break;
    case 296:  // $7003
      /*$7003*/ CYCLES(0x7003, 12);
                s_a = ram_peek(0x0003);
      /*$7005*/ s_y = ram_peek(0x0002);
      /*$7007*/ rom_plot(0xfffe);
      /*$700A*/ CYCLES(0x700a, 11);
                tmp1_U8 = ram_peek(0x0003);
      /*$700C*/ tmp2_U8 = ram_peek(0x0008);
                s_status_c = (tmp1_U8 >= tmp2_U8);
      /*$700E*/ branchTarget = true; block_id = !(tmp1_U8 != tmp2_U8) ? 298 : 297;
      break;
    case 297:  // $7010
      /*$7010*/ CYCLES(0x7010, 9);
                ram_poke(0x0003, (uint8_t)(ram_peek(0x0003) + 0x01));
      /*$7012*/ branchTarget = true; block_id = 296;
      break;
    case 298:  // $7015
      /*$7015*/ CYCLES(0x7015, 7);
                tmp2_U8 = pop8();
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$7016*/ ram_poke(0x0003, tmp2_U8);
      /*$7018*/ branchTarget = true; block_id = find_block_id_func_t001(0x7018, pop16() + 1);;
      break;
    case 299:  // $7019
      /*$7019*/ CYCLES(0x7019, 14);
                s_y = 0x00;
      /*$701B*/ tmp2_U8 = peek(ram_peek16al(0x000a));
                s_a = tmp2_U8;
      /*$701D*/ tmp2_U8 = (uint8_t)(ram_peek(0x000a) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x000a, tmp2_U8);
      /*$701F*/ branchTarget = true; block_id = tmp2_U8 ? 301 : 300;
      break;
    case 300:  // $7021
      /*$7021*/ CYCLES(0x7021, 4);
                tmp2_U8 = (uint8_t)(ram_peek(0x000b) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x000b, tmp2_U8);
                block_id = 301;
      break;
    case 301:  // $7023
      /*$7023*/ CYCLES(0x7023, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x7023, pop16() + 1);;
      break;
    case 302:  // $7024
      /*$7024*/ CYCLES(0x7024, 4);
                branchTarget = true; block_id = !s_status_not_z ? 304 : 303;
      break;
    case 303:  // $7026
      /*$7026*/ CYCLES(0x7026, 4);
                s_a = 0x05;
                block_id = 304;
      break;
    case 304:  // $7028
      /*$7028*/ CYCLES(0x7028, 6);
                rom_setcol(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x7028, pop16() + 1);;
      break;
    case 305:  // $702B
      /*$702B*/ CYCLES(0x702b, 18);
      /*$702D*/ ram_poke(0x0004, 0x00);
      /*$7031*/ ram_poke(0x0005, 0x20);
      /*$7033*/ s_y = 0x00;
                block_id = 306;
      break;
    case 306:  // $7035
      /*$7035*/ CYCLES(0x7035, 12);
      /*$7037*/ tmp2_U8 = s_y;
                poke((ram_peek16al(0x0004) + tmp2_U8), 0x00);
      /*$7039*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_y = tmp2_U8;
      /*$703A*/ branchTarget = true; block_id = tmp2_U8 ? 306 : 307;
      break;
    case 307:  // $703C
      /*$703C*/ CYCLES(0x703c, 14);
                ram_poke(0x0005, (uint8_t)(ram_peek(0x0005) + 0x01));
      /*$703E*/ tmp2_U8 = ram_peek(0x0005);
                s_a = tmp2_U8;
      /*$7040*/ tmp1_U8 = tmp2_U8 != 0x40;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp2_U8 >= 0x40);
                s_status_n = ((uint8_t)(tmp2_U8 - 0x40) & 0x80);
      /*$7042*/ branchTarget = true; block_id = tmp1_U8 ? 306 : 308;
      break;
    case 308:  // $7044
      /*$7044*/ CYCLES(0x7044, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x7044, pop16() + 1);;
      break;
    case 309:  // $7045
      /*$7045*/ CYCLES(0x7045, 6);
                func_702b(0xfffe);
                branchTarget = true; block_id = 310;
      break;
    case 310:  // $7048
      /*$7048*/ CYCLES(0x7048, 43);
      /*$704A*/ ram_poke(0x6c46, 0x00);
      /*$704D*/ tmp1_U8 = io_peek(0xc050);
      /*$7050*/ tmp1_U8 = io_peek(0xc057);
      /*$7053*/ tmp1_U8 = io_peek(0xc055);
      /*$7056*/ tmp1_U8 = io_peek(0xc052);
      /*$705B*/ ram_poke(0x0002, 0x04);
      /*$705F*/ ram_poke(0x0003, 0x00);
                block_id = 311;
      break;
    case 311:  // $7061
      /*$7061*/ CYCLES(0x7061, 6);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp1_U8;
      /*$7062*/ branchTarget = true; block_id = tmp1_U8 ? 311 : 312;
      break;
    case 312:  // $7064
      /*$7064*/ CYCLES(0x7064, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x0003) - 0x01);
                ram_poke(0x0003, tmp1_U8);
      /*$7066*/ branchTarget = true; block_id = tmp1_U8 ? 311 : 313;
      break;
    case 313:  // $7068
      /*$7068*/ CYCLES(0x7068, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x0002) - 0x01);
                ram_poke(0x0002, tmp1_U8);
      /*$706A*/ branchTarget = true; block_id = tmp1_U8 ? 311 : 314;
      break;
    case 314:  // $706C
      /*$706C*/ CYCLES(0x706c, 16);
      /*$706E*/ ram_poke(0x0003, 0x27);
      /*$7070*/ s_status_not_z = 0x00;
                s_a = 0x00;
      /*$7072*/ func_7024(0xfffe);
                branchTarget = true; block_id = 315;
      break;
    case 315:  // $7075
      /*$7075*/ CYCLES(0x7075, 19);
      /*$7077*/ ram_poke(0x002c, 0x27);
      /*$7079*/ s_y = 0x00;
      /*$707B*/ s_a = ram_peek(0x0003);
      /*$707D*/ rom_hline(0xfffe);
      /*$7080*/ CYCLES(0x7080, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x0003) - 0x01);
                ram_poke(0x0003, tmp1_U8);
      /*$7082*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 315 : 316;
      break;
    case 316:  // $7084
      /*$7084*/ CYCLES(0x7084, 26);
      /*$7086*/ ram_poke(0x0022, 0x14);
      /*$708A*/ ram_poke(0x0000, 0x15);
      /*$708C*/ s_status_not_z = 0x0d;
                s_a = 0x0d;
      /*$708E*/ ram_poke(0x0001, 0x0d);
      /*$7090*/ func_7024(0xfffe);
                branchTarget = true; block_id = 317;
      break;
    case 317:  // $7093
      /*$7093*/ CYCLES(0x7093, 9);
      /*$7096*/ branchTarget = true; block_id = !ram_peek(0x0301) ? 320 : 318;
      break;
    case 318:  // $7098
      /*$7098*/ CYCLES(0x7098, 11);
                s_a = 0x01;
      /*$709A*/ s_y = 0x01;
      /*$709B*/ rom_plot(0xfffe);
      /*$709E*/ CYCLES(0x709e, 12);
                tmp1_U8 = ram_peek(0x0301);
      /*$70A1*/ s_status_c = (tmp1_U8 >= 0x01);
      /*$70A3*/ branchTarget = true; block_id = !(tmp1_U8 != 0x01) ? 320 : 319;
      break;
    case 319:  // $70A5
      /*$70A5*/ CYCLES(0x70a5, 12);
                s_a = 0x01;
      /*$70A7*/ s_y = 0x26;
      /*$70A9*/ rom_plot(0xfffe);
                branchTarget = true; block_id = 320;
      break;
    case 320:  // $70AC
      /*$70AC*/ CYCLES(0x70ac, 12);
                s_y = 0x00;
      /*$70AE*/ s_a = 0x00;
      /*$70B0*/ rom_hline(0xfffe);
      /*$70B3*/ CYCLES(0x70b3, 12);
                s_y = 0x00;
      /*$70B5*/ s_a = 0x27;
      /*$70B7*/ rom_hline(0xfffe);
      /*$70BA*/ CYCLES(0x70ba, 26);
      /*$70BC*/ ram_poke(0x0002, 0x00);
      /*$70C0*/ ram_poke(0x0003, 0x00);
      /*$70C4*/ ram_poke(0x0008, 0x27);
      /*$70C6*/ func_7000(0xfffe);
                branchTarget = true; block_id = 321;
      break;
    case 321:  // $70C9
      /*$70C9*/ CYCLES(0x70c9, 19);
      /*$70CB*/ ram_poke(0x0003, 0x00);
      /*$70CF*/ ram_poke(0x0002, 0x27);
      /*$70D1*/ func_7000(0xfffe);
                branchTarget = true; block_id = 322;
      break;
    case 322:  // $70D4
      /*$70D4*/ CYCLES(0x70d4, 23);
      /*$70D6*/ ram_poke(0x0002, 0x00);
      /*$70D8*/ ram_poke(0x0003, 0x00);
      /*$70DC*/ ram_poke(0x0008, 0x27);
      /*$70DE*/ func_6148(0xfffe);
                branchTarget = true; block_id = 323;
      break;
    case 323:  // $70E1
      /*$70E1*/ CYCLES(0x70e1, 19);
      /*$70E3*/ ram_poke(0x0002, 0x00);
      /*$70E7*/ ram_poke(0x0003, 0x27);
      /*$70E9*/ func_6148(0xfffe);
                branchTarget = true; block_id = 324;
      break;
    case 324:  // $70EC
      /*$70EC*/ CYCLES(0x70ec, 16);
      /*$70EE*/ ram_poke(0x0002, 0x00);
      /*$70F0*/ ram_poke(0x0003, 0x00);
      /*$70F2*/ func_615a(0xfffe);
                branchTarget = true; block_id = 325;
      break;
    case 325:  // $70F5
      /*$70F5*/ CYCLES(0x70f5, 19);
      /*$70F7*/ ram_poke(0x0003, 0x00);
      /*$70FB*/ ram_poke(0x0002, 0x27);
      /*$70FD*/ func_615a(0xfffe);
                branchTarget = true; block_id = 326;
      break;
    case 326:  // $7100
      /*$7100*/ CYCLES(0x7100, 33);
      /*$7102*/ ram_poke(0x0001, 0x03);
      /*$7106*/ ram_poke(0x0003, 0x27);
      /*$710A*/ ram_poke(0x0002, 0x12);
      /*$710E*/ ram_poke(0x0008, 0x16);
      /*$7110*/ func_6148(0xfffe);
                branchTarget = true; block_id = 327;
      break;
    case 327:  // $7113
      /*$7113*/ CYCLES(0x7113, 19);
                s_x = ram_peek(0x0303);
      /*$7118*/ ram_poke(0x000a, 0x00);
      /*$711C*/ ram_poke(0x000b, 0x80);
                block_id = 328;
      break;
    case 328:  // $711E
      /*$711E*/ CYCLES(0x711e, 6);
                tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp1_U8;
      /*$711F*/ branchTarget = true; block_id = !tmp1_U8 ? 331 : 329;
      break;
    case 329:  // $7121
      /*$7121*/ CYCLES(0x7121, 6);
                func_7019(0xfffe);
      /*$7124*/ CYCLES(0x7124, 7);
      /*$7126*/ branchTarget = true; block_id = !(s_a != 0x2a) ? 328 : 330;
      break;
    case 330:  // $7128
      /*$7128*/ CYCLES(0x7128, 6);
                branchTarget = true; block_id = 329;
      break;
    case 331:  // $712B
      /*$712B*/ CYCLES(0x712b, 6);
                func_7019(0xfffe);
                branchTarget = true; block_id = 332;
      break;
    case 332:  // $712E
      /*$712E*/ CYCLES(0x712e, 7);
      /*$7130*/ branchTarget = true; block_id = (s_a != 0x45) ? 334 : 333;
      break;
    case 333:  // $7132
      /*$7132*/ CYCLES(0x7132, 14);
      /*$7134*/ ram_poke(0x0303, 0x01);
      /*$7137*/ branchTarget = true; block_id = 327;
      break;
    case 334:  // $713A
      /*$713A*/ CYCLES(0x713a, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x48);
      /*$713C*/ branchTarget = true; block_id = (tmp1_U8 != 0x48) ? 336 : 335;
      break;
    case 335:  // $713E
      /*$713E*/ CYCLES(0x713e, 6);
                func_7019(0xfffe);
      /*$7141*/ CYCLES(0x7141, 9);
                ram_poke(0x0001, s_a);
      /*$7143*/ func_7019(0xfffe);
      /*$7146*/ CYCLES(0x7146, 9);
                ram_poke(0x0002, s_a);
      /*$7148*/ func_7019(0xfffe);
      /*$714B*/ CYCLES(0x714b, 9);
                ram_poke(0x0008, s_a);
      /*$714D*/ func_7019(0xfffe);
      /*$7150*/ CYCLES(0x7150, 12);
                ram_poke(0x0003, s_a);
      /*$7152*/ tmp1_U8 = ram_peek(0x0001);
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
      /*$7154*/ func_7024(0xfffe);
      /*$7157*/ CYCLES(0x7157, 19);
      /*$7159*/ ram_poke(0x002c, ram_peek(0x0008));
      /*$715B*/ s_a = ram_peek(0x0003);
      /*$715D*/ s_y = ram_peek(0x0002);
      /*$715F*/ rom_hline(0xfffe);
      /*$7162*/ CYCLES(0x7162, 6);
                func_6148(0xfffe);
      /*$7165*/ CYCLES(0x7165, 6);
                branchTarget = true; block_id = 331;
      break;
    case 336:  // $7168
      /*$7168*/ CYCLES(0x7168, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x56);
      /*$716A*/ branchTarget = true; block_id = (tmp1_U8 != 0x56) ? 338 : 337;
      break;
    case 337:  // $716C
      /*$716C*/ CYCLES(0x716c, 6);
                func_7019(0xfffe);
      /*$716F*/ CYCLES(0x716f, 9);
                ram_poke(0x0001, s_a);
      /*$7171*/ func_7019(0xfffe);
      /*$7174*/ CYCLES(0x7174, 9);
                ram_poke(0x0003, s_a);
      /*$7176*/ func_7019(0xfffe);
      /*$7179*/ CYCLES(0x7179, 9);
                ram_poke(0x0008, s_a);
      /*$717B*/ func_7019(0xfffe);
      /*$717E*/ CYCLES(0x717e, 12);
                ram_poke(0x0002, s_a);
      /*$7180*/ tmp1_U8 = ram_peek(0x0001);
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
      /*$7182*/ func_7024(0xfffe);
      /*$7185*/ CYCLES(0x7185, 6);
                func_7000(0xfffe);
      /*$7188*/ CYCLES(0x7188, 6);
                func_615a(0xfffe);
      /*$718B*/ CYCLES(0x718b, 6);
                branchTarget = true; block_id = 331;
      break;
    case 338:  // $718E
      /*$718E*/ CYCLES(0x718e, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x50);
      /*$7190*/ branchTarget = true; block_id = (tmp1_U8 != 0x50) ? 340 : 339;
      break;
    case 339:  // $7192
      /*$7192*/ CYCLES(0x7192, 6);
                func_7019(0xfffe);
      /*$7195*/ CYCLES(0x7195, 9);
                ram_poke(0x0001, s_a);
      /*$7197*/ func_7019(0xfffe);
      /*$719A*/ CYCLES(0x719a, 9);
                ram_poke(0x0002, s_a);
      /*$719C*/ func_7019(0xfffe);
      /*$719F*/ CYCLES(0x719f, 12);
                ram_poke(0x0003, s_a);
      /*$71A1*/ tmp1_U8 = ram_peek(0x0001);
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
      /*$71A3*/ func_7024(0xfffe);
      /*$71A6*/ CYCLES(0x71a6, 12);
                s_a = ram_peek(0x0003);
      /*$71A8*/ s_y = ram_peek(0x0002);
      /*$71AA*/ rom_plot(0xfffe);
      /*$71AD*/ CYCLES(0x71ad, 6);
                func_60e4(0xfffe);
      /*$71B0*/ CYCLES(0x71b0, 6);
                branchTarget = true; block_id = 331;
      break;
    case 340:  // $71B3
      /*$71B3*/ CYCLES(0x71b3, 7);
      /*$71B5*/ branchTarget = true; block_id = (s_a != 0x54) ? 343 : 341;
      break;
    case 341:  // $71B7
      /*$71B7*/ CYCLES(0x71b7, 6);
                func_7019(0xfffe);
                branchTarget = true; block_id = 342;
      break;
    case 342:  // $71BA
      /*$71BA*/ CYCLES(0x71ba, 11);
                ram_poke(0x0304, s_a);
      /*$71BD*/ branchTarget = true; block_id = 331;
      break;
    case 343:  // $71C0
      /*$71C0*/ CYCLES(0x71c0, 7);
                tmp1_U8 = s_a;
                tmp2_U8 = tmp1_U8 != 0x2a;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp1_U8 >= 0x2a);
                s_status_n = ((uint8_t)(tmp1_U8 - 0x2a) & 0x80);
      /*$71C2*/ branchTarget = true; block_id = !tmp2_U8 ? 345 : 344;
      break;
    case 344:  // $71C4
      /*$71C4*/ CYCLES(0x71c4, 6);
                branchTarget = true; block_id = 331;
      break;
    case 345:  // $71C7
      /*$71C7*/ CYCLES(0x71c7, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x71c7, pop16() + 1);;
      break;
    case 346:  // $71CD
      /*$71CD*/ CYCLES(0x71cd, 26);
      /*$71CF*/ ram_poke(0x71cb, 0x00);
      /*$71D2*/ ram_poke(0x71cc, 0x00);
      /*$71D5*/ s_x = ram_peek(0x0301);
      /*$71D8*/ s_y = ram_peek(0x0303);
      /*$71DB*/ s_status_d = 0x01;
                block_id = 347;
      break;
    case 347:  // $71DC
      /*$71DC*/ CYCLES(0x71dc, 36);
                s_a = ram_peek((0x71c8 + s_x));
      /*$71DF*/ s_status_c = 0x00;
      /*$71E0*/ block_id = s_status_d ? 349 : 348;
      break;
    case 348:  // $71E0
      /*$71E0*/ tmp4_U16 = (s_a + ram_peek(0x71cb)) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 350;
      break;
    case 349:  // $71E0
      /*$71E0*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x71cb), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 350;
      break;
    case 350:  // $71E3
      /*$71E3*/ ram_poke(0x71cb, s_a);
      /*$71E6*/ s_a = ram_peek(0x71cc);
      /*$71E9*/ block_id = s_status_d ? 352 : 351;
      break;
    case 351:  // $71E9
      /*$71E9*/ tmp4_U16 = s_a;
                tmp3_U16 = tmp4_U16 + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 353;
      break;
    case 352:  // $71E9
      /*$71E9*/ tmp3_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 353;
      break;
    case 353:  // $71EB
      /*$71EB*/ ram_poke(0x71cc, s_a);
      /*$71EE*/ tmp2_U8 = (uint8_t)(s_y - 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_y = tmp2_U8;
      /*$71EF*/ branchTarget = true; block_id = tmp2_U8 ? 347 : 354;
      break;
    case 354:  // $71F1
      /*$71F1*/ CYCLES(0x71f1, 4);
                s_status_d = 0x00;
      /*$71F2*/ branchTarget = true; block_id = find_block_id_func_t001(0x71f2, pop16() + 1);;
      break;
    case 355:  // $71F3
      /*$71F3*/ CYCLES(0x71f3, 16);
                tmp2_U8 = s_a;
                push8(tmp2_U8);
      /*$71F7*/ tmp2_U8 = tmp2_U8 >> 0x04;
                s_a = tmp2_U8;
      /*$71FA*/ branchTarget = true; block_id = !(tmp2_U8 != 0x00) ? 357 : 356;
      break;
    case 356:  // $71FC
      /*$71FC*/ CYCLES(0x71fc, 4);
                ram_poke(0x002c, s_a);
                block_id = 357;
      break;
    case 357:  // $71FE
      /*$71FE*/ CYCLES(0x71fe, 9);
                push8(s_a);
      /*$7201*/ branchTarget = true; block_id = ram_peek(0x002c) ? 359 : 358;
      break;
    case 358:  // $7203
      /*$7203*/ CYCLES(0x7203, 7);
                tmp2_U8 = pop8();
      /*$7204*/ branchTarget = true; block_id = 363;
      break;
    case 359:  // $7207
      /*$7207*/ CYCLES(0x7207, 12);
                tmp2_U8 = pop8();
                s_a = tmp2_U8;
      /*$7208*/ s_status_c = 0x00;
      /*$7209*/ block_id = s_status_d ? 361 : 360;
      break;
    case 360:  // $7209
      /*$7209*/ tmp3_U16 = s_a;
                tmp4_U16 = (tmp3_U16 + 0x00b0) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x00b0);
                tmp2_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 362;
      break;
    case 361:  // $7209
      /*$7209*/ tmp4_U16 = adc_dec16(s_a, 0xb0, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 362;
      break;
    case 362:  // $720B
      /*$720B*/ rom_cout(0xfffe);
                branchTarget = true; block_id = 363;
      break;
    case 363:  // $720E
      /*$720E*/ CYCLES(0x720e, 12);
                tmp2_U8 = pop8();
      /*$720F*/ tmp2_U8 = tmp2_U8 & 0x0f;
                s_a = tmp2_U8;
      /*$7211*/ s_status_c = (tmp2_U8 >= 0x00);
      /*$7213*/ branchTarget = true; block_id = !(tmp2_U8 != 0x00) ? 365 : 364;
      break;
    case 364:  // $7215
      /*$7215*/ CYCLES(0x7215, 4);
                ram_poke(0x002c, s_a);
                block_id = 365;
      break;
    case 365:  // $7217
      /*$7217*/ CYCLES(0x7217, 9);
                push8(s_a);
      /*$721A*/ branchTarget = true; block_id = ram_peek(0x002c) ? 367 : 366;
      break;
    case 366:  // $721C
      /*$721C*/ CYCLES(0x721c, 4);
                tmp2_U8 = pop8();
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$721D*/ branchTarget = true; block_id = find_block_id_func_t001(0x721d, pop16() + 1);;
      break;
    case 367:  // $721E
      /*$721E*/ CYCLES(0x721e, 12);
                tmp2_U8 = pop8();
                s_a = tmp2_U8;
      /*$721F*/ s_status_c = 0x00;
      /*$7220*/ block_id = s_status_d ? 369 : 368;
      break;
    case 368:  // $7220
      /*$7220*/ tmp4_U16 = s_a;
                tmp3_U16 = (tmp4_U16 + 0x00b0) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0x00b0);
                tmp2_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 370;
      break;
    case 369:  // $7220
      /*$7220*/ tmp3_U16 = adc_dec16(s_a, 0xb0, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 370;
      break;
    case 370:  // $7222
      /*$7222*/ rom_cout(0xfffe);
                branchTarget = true; block_id = 371;
      break;
    case 371:  // $7225
      /*$7225*/ CYCLES(0x7225, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x7225, pop16() + 1);;
      break;
    case 372:  // $7226
      /*$7226*/ CYCLES(0x7226, 7);
                tmp2_U8 = ram_peek(0x002c);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$7228*/ branchTarget = true; block_id = tmp2_U8 ? 374 : 373;
      break;
    case 373:  // $722A
      /*$722A*/ CYCLES(0x722a, 9);
                s_status_not_z = 0xb0;
                s_status_n = 0x80;
                s_a = 0xb0;
      /*$722C*/ rom_cout(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x722c, pop16() + 1);;
      break;
    case 374:  // $722F
      /*$722F*/ CYCLES(0x722f, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x722f, pop16() + 1);;
      break;
    case 375:  // $7230
      /*$7230*/ CYCLES(0x7230, 16);
                tmp2_U8 = pop8();
      /*$7231*/ ram_poke(0x000c, tmp2_U8);
      /*$7233*/ tmp2_U8 = pop8();
      /*$7234*/ ram_poke(0x000d, tmp2_U8);
      /*$7236*/ rom_fc68(0xfffe);
                branchTarget = true; block_id = 376;
      break;
    case 376:  // $7239
      /*$7239*/ CYCLES(0x7239, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x000c) + 0x01);
                ram_poke(0x000c, tmp2_U8);
      /*$723B*/ branchTarget = true; block_id = tmp2_U8 ? 378 : 377;
      break;
    case 377:  // $723D
      /*$723D*/ CYCLES(0x723d, 4);
                ram_poke(0x000d, (uint8_t)(ram_peek(0x000d) + 0x01));
                block_id = 378;
      break;
    case 378:  // $723F
      /*$723F*/ CYCLES(0x723f, 11);
                s_y = 0x00;
      /*$7241*/ tmp2_U8 = peek(ram_peek16al(0x000c));
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$7243*/ branchTarget = true; block_id = !tmp2_U8 ? 381 : 379;
      break;
    case 379:  // $7245
      /*$7245*/ CYCLES(0x7245, 6);
                rom_cout(0xfffe);
                branchTarget = true; block_id = 380;
      break;
    case 380:  // $7248
      /*$7248*/ CYCLES(0x7248, 6);
                branchTarget = true; block_id = 376;
      break;
    case 381:  // $724B
      /*$724B*/ CYCLES(0x724b, 12);
      /*$724D*/ push8(ram_peek(0x000d));
      /*$724E*/ tmp2_U8 = ram_peek(0x000c);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$7250*/ push8(tmp2_U8);
      /*$7251*/ branchTarget = true; block_id = find_block_id_func_t001(0x7251, pop16() + 1);;
      break;
    case 382:  // $7267
      /*$7267*/ CYCLES(0x7267, 65);
                s_status_d = 0x01;
      /*$726C*/ tmp3_U16 = adc_dec16(ram_peek(0x71cb), ram_peek(0x7252), 0x00);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
      /*$726F*/ ram_poke(0x7252, ((uint8_t)tmp3_U16));
      /*$7272*/ s_a = ram_peek(0x71cc);
      /*$7275*/ block_id = 0x01 ? 384 : 383;
      break;
    case 383:  // $7275
      /*$7275*/ tmp3_U16 = (s_a + ram_peek(0x7253)) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 385;
      break;
    case 384:  // $7275
      /*$7275*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x7253), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 385;
      break;
    case 385:  // $7278
      /*$7278*/ ram_poke(0x7253, s_a);
      /*$727B*/ s_a = ram_peek(0x7254);
      /*$727E*/ block_id = s_status_d ? 387 : 386;
      break;
    case 386:  // $727E
      /*$727E*/ tmp3_U16 = s_a + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 388;
      break;
    case 387:  // $727E
      /*$727E*/ tmp3_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 388;
      break;
    case 388:  // $7280
      /*$7280*/ ram_poke(0x7254, s_a);
      /*$7283*/ s_a = ram_peek(0x7255);
      /*$7286*/ block_id = s_status_d ? 390 : 389;
      break;
    case 389:  // $7286
      /*$7286*/ tmp3_U16 = s_a;
                tmp4_U16 = tmp3_U16 + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x0000);
                tmp2_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 391;
      break;
    case 390:  // $7286
      /*$7286*/ tmp4_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 391;
      break;
    case 391:  // $7288
      /*$7288*/ ram_poke(0x7255, s_a);
      /*$728B*/ s_status_d = 0x00;
      /*$728C*/ branchTarget = true; block_id = find_block_id_func_t001(0x728c, pop16() + 1);;
      break;
    case 392:  // $728D
      /*$728D*/ CYCLES(0x728d, 14);
                tmp2_U8 = ram_peek(0x7255);
                s_a = tmp2_U8;
      /*$7290*/ tmp1_U8 = ram_peek(0x7259);
                s_status_not_z = (tmp2_U8 != tmp1_U8);
                tmp5_U8 = tmp2_U8 >= tmp1_U8;
                s_status_c = tmp5_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
      /*$7293*/ branchTarget = true; block_id = !tmp5_U8 ? 401 : 393;
      break;
    case 393:  // $7295
      /*$7295*/ CYCLES(0x7295, 4);
                branchTarget = true; block_id = s_status_not_z ? 400 : 394;
      break;
    case 394:  // $7297
      /*$7297*/ CYCLES(0x7297, 14);
                tmp5_U8 = ram_peek(0x7254);
                s_a = tmp5_U8;
      /*$729A*/ tmp1_U8 = ram_peek(0x7258);
                s_status_not_z = (tmp5_U8 != tmp1_U8);
                tmp2_U8 = tmp5_U8 >= tmp1_U8;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp5_U8 - tmp1_U8) & 0x80);
      /*$729D*/ branchTarget = true; block_id = !tmp2_U8 ? 401 : 395;
      break;
    case 395:  // $729F
      /*$729F*/ CYCLES(0x729f, 4);
                branchTarget = true; block_id = s_status_not_z ? 400 : 396;
      break;
    case 396:  // $72A1
      /*$72A1*/ CYCLES(0x72a1, 14);
                tmp2_U8 = ram_peek(0x7253);
                s_a = tmp2_U8;
      /*$72A4*/ tmp1_U8 = ram_peek(0x7257);
                s_status_not_z = (tmp2_U8 != tmp1_U8);
                tmp5_U8 = tmp2_U8 >= tmp1_U8;
                s_status_c = tmp5_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
      /*$72A7*/ branchTarget = true; block_id = !tmp5_U8 ? 401 : 397;
      break;
    case 397:  // $72A9
      /*$72A9*/ CYCLES(0x72a9, 4);
                branchTarget = true; block_id = s_status_not_z ? 400 : 398;
      break;
    case 398:  // $72AB
      /*$72AB*/ CYCLES(0x72ab, 14);
                tmp5_U8 = ram_peek(0x7252);
                s_a = tmp5_U8;
      /*$72AE*/ tmp1_U8 = ram_peek(0x7256);
                s_status_not_z = (tmp5_U8 != tmp1_U8);
                tmp2_U8 = tmp5_U8 >= tmp1_U8;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp5_U8 - tmp1_U8) & 0x80);
      /*$72B1*/ branchTarget = true; block_id = !tmp2_U8 ? 401 : 399;
      break;
    case 399:  // $72B3
      /*$72B3*/ CYCLES(0x72b3, 4);
                branchTarget = true; block_id = s_status_not_z ? 400 : 400;
      break;
    case 400:  // $72B5
      /*$72B5*/ CYCLES(0x72b5, 41);
      /*$72B8*/ ram_poke(0x7256, ram_peek(0x7252));
      /*$72BE*/ ram_poke(0x7257, ram_peek(0x7253));
      /*$72C4*/ ram_poke(0x7258, ram_peek(0x7254));
      /*$72C7*/ tmp5_U8 = ram_peek(0x7255);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$72CA*/ ram_poke(0x7259, tmp5_U8);
                block_id = 401;
      break;
    case 401:  // $72CD
      /*$72CD*/ CYCLES(0x72cd, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x72cd, pop16() + 1);;
      break;
    case 402:  // $72CE
      /*$72CE*/ CYCLES(0x72ce, 19);
      /*$72D0*/ ram_poke(0x0025, 0x14);
      /*$72D4*/ ram_poke(0x0024, 0x00);
      /*$72D6*/ branchTarget = true; push16(0x72d8); block_id = 375;
      break;
    case 403:  // $72D9
      /*$72D9*/ CYCLES(0x72d9, 2);
                fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x72d9, 0x72d9, 0x03); error_handler(0x72d9);
      break;
    case 404:  // $72E2
      /*$72E2*/ CYCLES(0x72e2, 18);
      /*$72E4*/ ram_poke(0x002c, 0x00);
      /*$72E6*/ s_a = ram_peek(0x7255);
      /*$72E9*/ func_71f3(0xfffe);
                branchTarget = true; block_id = 405;
      break;
    case 405:  // $72EC
      /*$72EC*/ CYCLES(0x72ec, 11);
                s_a = ram_peek(0x7254);
      /*$72EF*/ func_71f3(0xfffe);
                branchTarget = true; block_id = 406;
      break;
    case 406:  // $72F2
      /*$72F2*/ CYCLES(0x72f2, 11);
                s_a = ram_peek(0x7253);
      /*$72F5*/ func_71f3(0xfffe);
                branchTarget = true; block_id = 407;
      break;
    case 407:  // $72F8
      /*$72F8*/ CYCLES(0x72f8, 11);
                s_a = ram_peek(0x7252);
      /*$72FB*/ func_71f3(0xfffe);
                branchTarget = true; block_id = 408;
      break;
    case 408:  // $72FE
      /*$72FE*/ CYCLES(0x72fe, 6);
                func_7226(0xfffe);
                branchTarget = true; block_id = 409;
      break;
    case 409:  // $7301
      /*$7301*/ CYCLES(0x7301, 12);
                s_a = 0x14;
      /*$7303*/ ram_poke(0x0024, 0x14);
      /*$7305*/ branchTarget = true; push16(0x7307); block_id = 375;
      break;
    case 410:  // $7308
      /*$7308*/ CYCLES(0x7308, 7);
      /*$730B*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x730b, 0x730b, 0x03); error_handler(0x730b);
      break;
    case 411:  // $730E
      /*$730E*/ CYCLES(0x730e, 2);
                fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x730e, 0x730e, 0x03); error_handler(0x730e);
      break;
    case 412:  // $7314
      /*$7314*/ CYCLES(0x7314, 18);
      /*$7316*/ ram_poke(0x002c, 0x00);
      /*$7318*/ s_a = ram_peek(0x7259);
      /*$731B*/ func_71f3(0xfffe);
                branchTarget = true; block_id = 413;
      break;
    case 413:  // $731E
      /*$731E*/ CYCLES(0x731e, 11);
                s_a = ram_peek(0x7258);
      /*$7321*/ func_71f3(0xfffe);
                branchTarget = true; block_id = 414;
      break;
    case 414:  // $7324
      /*$7324*/ CYCLES(0x7324, 11);
                s_a = ram_peek(0x7257);
      /*$7327*/ func_71f3(0xfffe);
                branchTarget = true; block_id = 415;
      break;
    case 415:  // $732A
      /*$732A*/ CYCLES(0x732a, 11);
                s_a = ram_peek(0x7256);
      /*$732D*/ func_71f3(0xfffe);
                branchTarget = true; block_id = 416;
      break;
    case 416:  // $7330
      /*$7330*/ CYCLES(0x7330, 6);
                func_7226(0xfffe);
                branchTarget = true; block_id = 417;
      break;
    case 417:  // $7333
      /*$7333*/ CYCLES(0x7333, 19);
      /*$7335*/ ram_poke(0x0024, 0x00);
      /*$7337*/ s_a = 0x15;
      /*$7339*/ ram_poke(0x0025, 0x15);
      /*$733B*/ branchTarget = true; push16(0x733d); block_id = 375;
      break;
    case 418:  // $733E
      /*$733E*/ CYCLES(0x733e, 7);
                tmp2_U8 = s_x;
                tmp2_U8 = peek((ram_peek((uint8_t)(0xd0 + tmp2_U8)) + (ram_peek((uint8_t)(0xd1 + tmp2_U8)) << 8)));
      /*$7340*/ branchTarget = true; block_id = (s_a != tmp2_U8) ? 411 : 419;
      break;
    case 419:  // $7342
      /*$7342*/ CYCLES(0x7342, 12);
      /*$7348*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x7348, 0x7348, 0x03); error_handler(0x7348);
      break;
    case 420:  // $734D
      /*$734D*/ CYCLES(0x734d, 18);
      /*$734F*/ ram_poke(0x002c, 0x00);
      /*$7351*/ s_a = ram_peek(0x725b);
      /*$7354*/ func_71f3(0xfffe);
                branchTarget = true; block_id = 421;
      break;
    case 421:  // $7357
      /*$7357*/ CYCLES(0x7357, 11);
                s_a = ram_peek(0x725a);
      /*$735A*/ func_71f3(0xfffe);
                branchTarget = true; block_id = 422;
      break;
    case 422:  // $735D
      /*$735D*/ CYCLES(0x735d, 6);
                func_7226(0xfffe);
                branchTarget = true; block_id = 423;
      break;
    case 423:  // $7360
      /*$7360*/ CYCLES(0x7360, 9);
                s_status_not_z = 0xa0;
                s_status_n = 0x80;
                s_a = 0xa0;
      /*$7362*/ rom_cout(0xfffe);
                branchTarget = true; block_id = 424;
      break;
    case 424:  // $7365
      /*$7365*/ CYCLES(0x7365, 12);
                s_a = 0x14;
      /*$7367*/ ram_poke(0x0024, 0x14);
      /*$7369*/ branchTarget = true; push16(0x736b); block_id = 375;
      break;
    case 425:  // $736C
      /*$736C*/ CYCLES(0x736c, 18);
                tmp4_U16 = (uint8_t)(0xc1 + s_x);
                ram_poke(tmp4_U16, (uint8_t)(ram_peek(tmp4_U16) - 0x01));
      /*$736E*/ tmp2_U8 = io_peek(0xc5d5);
                s_status_c = (s_y >= tmp2_U8);
      /*$7371*/ s_x = s_sp;
      /*$7372*/ s_status_not_z = 0xa0;
                s_status_n = 0x80;
                s_y = 0xa0;
      /*$7374*/ branchTarget = true; push16(0x7375); block_id = 1945;
      break;
    case 426:  // $7375
      /*$7375*/ CYCLES(0x7375, 18);
      /*$7377*/ ram_poke(0x002c, 0x00);
      /*$7379*/ s_a = ram_peek(0x71cc);
      /*$737C*/ func_71f3(0xfffe);
                branchTarget = true; block_id = 428;
      break;
    case 427:  // $7376
      /*$7376*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x7374, 0x7376, 0x02); error_handler(0x7374);
      break;
    case 428:  // $737F
      /*$737F*/ CYCLES(0x737f, 11);
                s_a = ram_peek(0x71cb);
      /*$7382*/ func_71f3(0xfffe);
                branchTarget = true; block_id = 429;
      break;
    case 429:  // $7385
      /*$7385*/ CYCLES(0x7385, 6);
                func_7226(0xfffe);
                branchTarget = true; block_id = 430;
      break;
    case 430:  // $7388
      /*$7388*/ CYCLES(0x7388, 19);
      /*$738A*/ ram_poke(0x0025, 0x16);
      /*$738E*/ ram_poke(0x0024, 0x00);
      /*$7390*/ branchTarget = true; push16(0x7392); block_id = 375;
      break;
    case 431:  // $7393
      /*$7393*/ CYCLES(0x7393, 2);
                fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x7393, 0x7393, 0x03); error_handler(0x7393);
      break;
    case 432:  // $73A2
      /*$73A2*/ CYCLES(0x73a2, 12);
                s_a = 0x00;
      /*$73A4*/ ram_poke(0x002c, 0x00);
      /*$73A6*/ func_71f3(0xfffe);
                branchTarget = true; block_id = 433;
      break;
    case 433:  // $73A9
      /*$73A9*/ CYCLES(0x73a9, 11);
                s_a = ram_peek(0x725e);
      /*$73AC*/ func_71f3(0xfffe);
                branchTarget = true; block_id = 434;
      break;
    case 434:  // $73AF
      /*$73AF*/ CYCLES(0x73af, 6);
                func_7226(0xfffe);
                branchTarget = true; block_id = 435;
      break;
    case 435:  // $73B2
      /*$73B2*/ CYCLES(0x73b2, 12);
                s_a = 0x14;
      /*$73B4*/ ram_poke(0x0024, 0x14);
      /*$73B6*/ branchTarget = true; push16(0x73b8); block_id = 375;
      break;
    case 436:  // $73B9
      /*$73B9*/ CYCLES(0x73b9, 18);
      /*$73BC*/ s_status_c = (s_a >= ram_peek(0x00cc));
      /*$73BE*/ s_x = s_sp;
      /*$73BF*/ s_status_not_z = 0xa0;
                s_status_n = 0x80;
                s_y = 0xa0;
      /*$73C1*/ branchTarget = true; push16(0x73c2); block_id = 1945;
      break;
    case 437:  // $73C2
      /*$73C2*/ CYCLES(0x73c2, 18);
      /*$73C4*/ ram_poke(0x002c, 0x00);
      /*$73C6*/ s_a = ram_peek(0x7265);
      /*$73C9*/ func_71f3(0xfffe);
                block_id = 439;
      break;
    case 438:  // $73C3
      /*$73C3*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x73c1, 0x73c3, 0x02); error_handler(0x73c1);
      break;
    case 439:  // $73CC
      /*$73CC*/ CYCLES(0x73cc, 6);
                func_7226(0xfffe);
                branchTarget = true; block_id = 440;
      break;
    case 440:  // $73CF
      /*$73CF*/ CYCLES(0x73cf, 12);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$73D1*/ ram_poke(0x0025, 0x00);
      /*$73D3*/ rom_fc68(0xfffe);
                branchTarget = true; block_id = 441;
      break;
    case 441:  // $73D6
      /*$73D6*/ CYCLES(0x73d6, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x73d6, pop16() + 1);;
      break;
    case 442:  // $73DD
      /*$73DD*/ CYCLES(0x73dd, 21);
                s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_a = 0x01;
      /*$73DF*/ ram_poke(0x0302, 0x01);
      /*$73E2*/ ram_poke(0x0301, 0x01);
      /*$73E5*/ ram_poke(0x73d7, 0x01);
      /*$73E8*/ branchTarget = true; block_id = find_block_id_func_t001(0x73e8, pop16() + 1);;
      break;
    case 443:  // $73E9
      /*$73E9*/ CYCLES(0x73e9, 19);
      /*$73EB*/ ram_poke(0x0025, 0x17);
      /*$73EF*/ ram_poke(0x0024, 0x00);
      /*$73F1*/ branchTarget = true; push16(0x73f3); block_id = 375;
      break;
    case 444:  // $73F4
      /*$73F4*/ CYCLES(0x73f4, 21);
      /*$73FF*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x73ff, 0x73ff, 0x03); error_handler(0x73ff);
      break;
    case 445:  // $7414
      /*$7414*/ CYCLES(0x7414, 14);
      /*$7416*/ ram_poke(0x0002, 0xe8);
      /*$741A*/ ram_poke(0x0003, 0x00);
                block_id = 446;
      break;
    case 446:  // $741C
      /*$741C*/ CYCLES(0x741c, 6);
                tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$741D*/ branchTarget = true; block_id = tmp2_U8 ? 446 : 447;
      break;
    case 447:  // $741F
      /*$741F*/ CYCLES(0x741f, 9);
                tmp2_U8 = io_peek(0xc000);
                s_a = tmp2_U8;
      /*$7422*/ branchTarget = true; block_id = (tmp2_U8 & 0x80) ? 458 : 448;
      break;
    case 448:  // $7424
      /*$7424*/ CYCLES(0x7424, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x0003) + 0x01);
                ram_poke(0x0003, tmp2_U8);
      /*$7426*/ branchTarget = true; block_id = tmp2_U8 ? 446 : 449;
      break;
    case 449:  // $7428
      /*$7428*/ CYCLES(0x7428, 9);
      /*$742B*/ branchTarget = true; block_id = !ram_peek(0x6c71) ? 456 : 450;
      break;
    case 450:  // $742D
      /*$742D*/ CYCLES(0x742d, 14);
                tmp2_U8 = io_peek(0xc05b);
      /*$7430*/ tmp2_U8 = io_peek(0xc062);
      /*$7433*/ branchTarget = true; block_id = (tmp2_U8 & 0x80) ? 452 : 451;
      break;
    case 451:  // $7435
      /*$7435*/ CYCLES(0x7435, 9);
                s_a = 0xb1;
      /*$7437*/ branchTarget = true; block_id = 458;
      break;
    case 452:  // $743A
      /*$743A*/ CYCLES(0x743a, 14);
                tmp2_U8 = io_peek(0xc05a);
      /*$743D*/ tmp2_U8 = io_peek(0xc062);
      /*$7440*/ branchTarget = true; block_id = (tmp2_U8 & 0x80) ? 454 : 453;
      break;
    case 453:  // $7442
      /*$7442*/ CYCLES(0x7442, 9);
                s_a = 0xb0;
      /*$7444*/ branchTarget = true; block_id = 458;
      break;
    case 454:  // $7447
      /*$7447*/ CYCLES(0x7447, 9);
                tmp2_U8 = io_peek(0xc063);
      /*$744A*/ branchTarget = true; block_id = (tmp2_U8 & 0x80) ? 456 : 455;
      break;
    case 455:  // $744C
      /*$744C*/ CYCLES(0x744c, 9);
                s_a = 0xb2;
      /*$744E*/ branchTarget = true; block_id = 458;
      break;
    case 456:  // $7451
      /*$7451*/ CYCLES(0x7451, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x0002) + 0x01);
                ram_poke(0x0002, tmp2_U8);
      /*$7453*/ branchTarget = true; block_id = tmp2_U8 ? 446 : 457;
      break;
    case 457:  // $7455
      /*$7455*/ CYCLES(0x7455, 21);
                s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_a = 0x01;
      /*$7457*/ ram_poke(0x0302, 0x01);
      /*$745A*/ ram_poke(0x0301, 0x01);
      /*$745D*/ io_poke(0xc010, 0x01);
      /*$7460*/ branchTarget = true; block_id = find_block_id_func_t001(0x7460, pop16() + 1);;
      break;
    case 458:  // $7461
      /*$7461*/ CYCLES(0x7461, 12);
                tmp2_U8 = s_a;
                io_poke(0xc010, tmp2_U8);
      /*$7466*/ branchTarget = true; block_id = !(tmp2_U8 != 0xc3) ? 465 : 459;
      break;
    case 459:  // $7468
      /*$7468*/ CYCLES(0x7468, 7);
                tmp2_U8 = s_a >= 0xb0;
                s_status_c = tmp2_U8;
      /*$746A*/ branchTarget = true; block_id = !tmp2_U8 ? 446 : 460;
      break;
    case 460:  // $746C
      /*$746C*/ CYCLES(0x746c, 7);
                tmp2_U8 = s_a >= 0xb3;
                s_status_c = tmp2_U8;
      /*$746E*/ branchTarget = true; block_id = tmp2_U8 ? 446 : 461;
      break;
    case 461:  // $7470
      /*$7470*/ CYCLES(0x7470, 26);
                s_status_c = 0x01;
      /*$7471*/ block_id = s_status_d ? 463 : 462;
      break;
    case 462:  // $7471
      /*$7471*/ tmp4_U16 = s_a;
                tmp3_U16 = (tmp4_U16 - 0x00b0) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0xff4f);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 464;
      break;
    case 463:  // $7471
      /*$7471*/ tmp3_U16 = sbc_dec16(s_a, 0xb0, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 464;
      break;
    case 464:  // $7473
      /*$7473*/ ram_poke(0x0301, s_a);
      /*$7476*/ s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$7478*/ ram_poke(0x0302, 0x00);
      /*$747B*/ io_poke(0xc010, 0x00);
      /*$747E*/ branchTarget = true; block_id = find_block_id_func_t001(0x747e, pop16() + 1);;
      break;
    case 465:  // $747F
      /*$747F*/ CYCLES(0x747f, 6);
                func_702b(0xfffe);
      /*$7482*/ CYCLES(0x7482, 11);
                tmp2_U8 = io_peek(0xc052);
      /*$7485*/ func_6641(0xfffe);
      /*$7488*/ CYCLES(0x7488, 12);
                s_a = 0x01;
      /*$748A*/ ram_poke(0x0025, 0x01);
      /*$748C*/ branchTarget = true; push16(0x748e); block_id = 375;
      break;
    case 466:  // $748F
      /*$748F*/ CYCLES(0x748f, 18);
                ram_poke(0xa0a0, s_a);
      /*$7496*/ tmp2_U8 = s_x;
                tmp2_U8 = peek((ram_peek((uint8_t)(0xc2 + tmp2_U8)) + (ram_peek((uint8_t)(0xc3 + tmp2_U8)) << 8)));
      /*$7498*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x7498, 0x7498, 0x03); error_handler(0x7498);
      break;
    case 467:  // $7541
      /*$7541*/ CYCLES(0x7541, 4);
                s_x = 0x00;
                block_id = 468;
      break;
    case 468:  // $7543
      /*$7543*/ CYCLES(0x7543, 11);
                s_a = ram_peek((0x6c63 + s_x));
      /*$7546*/ func_7590(0xfffe);
      /*$7549*/ CYCLES(0x7549, 9);
                tmp2_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp2_U8;
      /*$754C*/ branchTarget = true; block_id = (tmp2_U8 != 0x06) ? 468 : 469;
      break;
    case 469:  // $754E
      /*$754E*/ CYCLES(0x754e, 33);
      /*$7550*/ ram_poke(0x0001, 0x0c);
      /*$7554*/ ram_poke(0x0000, 0x02);
      /*$7558*/ ram_poke(0x0003, 0x12);
      /*$755C*/ ram_poke(0x0002, 0x1e);
      /*$755E*/ func_60e4(0xfffe);
      /*$7561*/ CYCLES(0x7561, 26);
      /*$7563*/ ram_poke(0x0003, 0x13);
      /*$7567*/ ram_poke(0x0008, 0x1d);
      /*$756B*/ ram_poke(0x0000, 0x0a);
      /*$756D*/ func_615a(0xfffe);
      /*$7570*/ CYCLES(0x7570, 12);
      /*$7572*/ ram_poke(0x0000, 0x0e);
      /*$7574*/ func_60e4(0xfffe);
      /*$7577*/ CYCLES(0x7577, 4);
                s_x = 0x00;
                block_id = 470;
      break;
    case 470:  // $7579
      /*$7579*/ CYCLES(0x7579, 6);
                func_75d1(0xfffe);
      /*$757C*/ CYCLES(0x757c, 11);
                ram_poke((0x6c63 + s_x), s_a);
      /*$757F*/ func_7590(0xfffe);
      /*$7582*/ CYCLES(0x7582, 9);
                tmp2_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp2_U8;
      /*$7583*/ s_status_c = (tmp2_U8 >= 0x06);
      /*$7585*/ branchTarget = true; block_id = (tmp2_U8 != 0x06) ? 470 : 471;
      break;
    case 471:  // $7587
      /*$7587*/ CYCLES(0x7587, 16);
      /*$7589*/ ram_poke(0x0036, 0xf0);
      /*$758B*/ s_status_not_z = 0xfd;
                s_status_n = 0x80;
                s_a = 0xfd;
      /*$758D*/ ram_poke(0x0037, 0xfd);
      /*$758F*/ branchTarget = true; block_id = find_block_id_func_t001(0x758f, pop16() + 1);;
      break;
    case 472:  // $7590
      /*$7590*/ CYCLES(0x7590, 11);
                ram_poke(0x0002, s_x);
      /*$7594*/ branchTarget = true; block_id = (s_a != 0x88) ? 474 : 473;
      break;
    case 473:  // $7596
      /*$7596*/ CYCLES(0x7596, 4);
                s_a = 0xe6;
                block_id = 474;
      break;
    case 474:  // $7598
      /*$7598*/ CYCLES(0x7598, 7);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x95);
      /*$759A*/ branchTarget = true; block_id = (tmp2_U8 != 0x95) ? 476 : 475;
      break;
    case 475:  // $759C
      /*$759C*/ CYCLES(0x759c, 4);
                s_a = 0xe7;
                block_id = 476;
      break;
    case 476:  // $759E
      /*$759E*/ CYCLES(0x759e, 24);
                push8(s_a);
      /*$759F*/ tmp2_U8 = s_x;
      /*$75A2*/ ram_poke(0x0024, ram_peek((0x75b3 + tmp2_U8)));
      /*$75A7*/ ram_poke(0x0025, ram_peek((0x75b9 + tmp2_U8)));
      /*$75A9*/ rom_fc68(0xfffe);
      /*$75AC*/ CYCLES(0x75ac, 7);
                tmp2_U8 = pop8();
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$75AD*/ rom_cout(0xfffe);
      /*$75B0*/ CYCLES(0x75b0, 6);
                tmp2_U8 = ram_peek(0x0002);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$75B2*/ branchTarget = true; block_id = find_block_id_func_t001(0x75b2, pop16() + 1);;
      break;
    case 477:  // $75D1
      /*$75D1*/ CYCLES(0x75d1, 26);
                tmp2_U8 = s_x;
                ram_poke(0x0002, tmp2_U8);
      /*$75D6*/ ram_poke(0x0024, ram_peek((0x75bf + tmp2_U8)));
      /*$75DB*/ ram_poke(0x0025, ram_peek((0x75c5 + tmp2_U8)));
      /*$75DD*/ rom_fc68(0xfffe);
      /*$75E0*/ CYCLES(0x75e0, 12);
                s_x = ram_peek(0x0002);
      /*$75E2*/ s_status_not_z = 0xa0;
                s_status_n = 0x80;
                s_a = 0xa0;
      /*$75E4*/ rom_cout(0xfffe);
      /*$75E7*/ CYCLES(0x75e7, 4);
                s_y = 0x00;
                block_id = 478;
      break;
    case 478:  // $75E9
      /*$75E9*/ CYCLES(0x75e9, 6);
                tmp2_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp2_U8;
      /*$75EA*/ branchTarget = true; block_id = tmp2_U8 ? 478 : 479;
      break;
    case 479:  // $75EC
      /*$75EC*/ CYCLES(0x75ec, 7);
      /*$75F0*/ CYCLES(0x75f0, 6);
                tmp2_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp2_U8;
      /*$75F1*/ branchTarget = true; block_id = tmp2_U8 ? 478 : 480;
      break;
    case 480:  // $75F3
      /*$75F3*/ CYCLES(0x75f3, 26);
                tmp2_U8 = ram_peek(0x0002);
                s_x = tmp2_U8;
      /*$75F8*/ ram_poke(0x0024, ram_peek((0x75bf + tmp2_U8)));
      /*$75FD*/ ram_poke(0x0025, ram_peek((0x75c5 + tmp2_U8)));
      /*$75FF*/ rom_fc68(0xfffe);
      /*$7602*/ CYCLES(0x7602, 14);
                tmp2_U8 = ram_peek(0x0002);
                s_x = tmp2_U8;
      /*$7604*/ tmp2_U8 = ram_peek((0x75cb + tmp2_U8));
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$7607*/ rom_cout(0xfffe);
      /*$760A*/ CYCLES(0x760a, 4);
                s_y = 0x00;
                block_id = 481;
      break;
    case 481:  // $760C
      /*$760C*/ CYCLES(0x760c, 6);
                tmp2_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp2_U8;
      /*$760D*/ branchTarget = true; block_id = tmp2_U8 ? 481 : 482;
      break;
    case 482:  // $760F
      /*$760F*/ CYCLES(0x760f, 9);
                tmp2_U8 = io_peek(0xc000);
                s_a = tmp2_U8;
      /*$7612*/ branchTarget = true; block_id = (tmp2_U8 & 0x80) ? 485 : 483;
      break;
    case 483:  // $7614
      /*$7614*/ CYCLES(0x7614, 6);
                tmp2_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp2_U8;
      /*$7615*/ branchTarget = true; block_id = tmp2_U8 ? 481 : 484;
      break;
    case 484:  // $7617
      /*$7617*/ CYCLES(0x7617, 9);
                s_x = ram_peek(0x0002);
      /*$7619*/ branchTarget = true; block_id = 477;
      break;
    case 485:  // $761C
      /*$761C*/ CYCLES(0x761c, 12);
                tmp2_U8 = s_a;
                io_poke(0xc010, tmp2_U8);
      /*$761F*/ tmp2_U8 = tmp2_U8 >= 0xa1;
                s_status_c = tmp2_U8;
      /*$7621*/ branchTarget = true; block_id = tmp2_U8 ? 489 : 486;
      break;
    case 486:  // $7623
      /*$7623*/ CYCLES(0x7623, 7);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x88);
      /*$7625*/ branchTarget = true; block_id = !(tmp2_U8 != 0x88) ? 489 : 487;
      break;
    case 487:  // $7627
      /*$7627*/ CYCLES(0x7627, 7);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x95);
      /*$7629*/ branchTarget = true; block_id = !(tmp2_U8 != 0x95) ? 489 : 488;
      break;
    case 488:  // $762B
      /*$762B*/ CYCLES(0x762b, 9);
                s_x = ram_peek(0x0002);
      /*$762D*/ branchTarget = true; block_id = 477;
      break;
    case 489:  // $7630
      /*$7630*/ CYCLES(0x7630, 6);
                tmp2_U8 = ram_peek(0x0002);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$7632*/ branchTarget = true; block_id = find_block_id_func_t001(0x7632, pop16() + 1);;
      break;
    case 490:  // $7633
      /*$7633*/ CYCLES(0x7633, 24);
      /*$7638*/ tmp3_U16 = adc_dec16(ram_peek(0x725e), 0x01, 0x00);
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
      /*$763A*/ ram_poke(0x725e, ((uint8_t)tmp3_U16));
      /*$763D*/ s_status_d = 0x00;
      /*$763E*/ func_64a9(0xfffe);
      /*$7641*/ CYCLES(0x7641, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x7641, pop16() + 1);;
      break;
    case 491:  // $7642
      /*$7642*/ CYCLES(0x7642, 6);
                func_6c4b(0xfffe);
                branchTarget = true; block_id = 492;
      break;
    case 492:  // $7645
      /*$7645*/ CYCLES(0x7645, 9);
                ram_poke(0x0002, s_a);
      /*$7647*/ func_6c4b(0xfffe);
                branchTarget = true; block_id = 493;
      break;
    case 493:  // $764A
      /*$764A*/ CYCLES(0x764a, 16);
                ram_poke(0x0003, s_a);
      /*$764C*/ s_a = ram_peek(0x0003);
      /*$764E*/ tmp2_U8 = ram_peek(0x0002);
                s_status_not_z = tmp2_U8;
                s_y = tmp2_U8;
      /*$7650*/ rom_scrn(0xfffe);
      /*$7653*/ CYCLES(0x7653, 4);
                branchTarget = true; block_id = s_status_not_z ? 491 : 494;
      break;
    case 494:  // $7655
      /*$7655*/ CYCLES(0x7655, 9);
                s_a = 0x0f;
      /*$7657*/ rom_setcol(0xfffe);
      /*$765A*/ CYCLES(0x765a, 12);
                s_a = ram_peek(0x0003);
      /*$765C*/ s_y = ram_peek(0x0002);
      /*$765E*/ rom_plot(0xfffe);
      /*$7661*/ CYCLES(0x7661, 19);
      /*$7663*/ ram_poke(0x0000, 0x01);
      /*$7667*/ ram_poke(0x0001, 0x09);
      /*$7669*/ func_60e4(0xfffe);
                branchTarget = true; block_id = 495;
      break;
    case 495:  // $766C
      /*$766C*/ CYCLES(0x766c, 35);
      /*$7671*/ tmp3_U16 = adc_dec16(ram_peek(0x725f), 0x01, 0x00);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
      /*$7673*/ ram_poke(0x725f, ((uint8_t)tmp3_U16));
      /*$7676*/ s_a = ram_peek(0x7260);
      /*$7679*/ block_id = 0x01 ? 497 : 496;
      break;
    case 496:  // $7679
      /*$7679*/ tmp3_U16 = s_a;
                tmp4_U16 = tmp3_U16 + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x0000);
                tmp2_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 498;
      break;
    case 497:  // $7679
      /*$7679*/ tmp4_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 498;
      break;
    case 498:  // $767B
      /*$767B*/ ram_poke(0x7260, s_a);
      /*$767E*/ s_status_d = 0x00;
      /*$767F*/ branchTarget = true; block_id = find_block_id_func_t001(0x767f, pop16() + 1);;
      break;
    case 499:  // $7691
      /*$7691*/ CYCLES(0x7691, 6);
                branchTarget = true; push16(0x7693); block_id = 595;
      break;
    case 500:  // $7694
      /*$7694*/ CYCLES(0x7694, 6);
                func_728d(0xfffe);
                branchTarget = true; block_id = 501;
      break;
    case 501:  // $7697
      /*$7697*/ CYCLES(0x7697, 55);
      /*$7699*/ ram_poke(0x0303, 0x01);
      /*$769C*/ ram_poke(0x7265, 0x01);
      /*$76A1*/ ram_poke(0x7252, 0x00);
      /*$76A4*/ ram_poke(0x7253, 0x00);
      /*$76A7*/ ram_poke(0x7254, 0x00);
      /*$76AA*/ ram_poke(0x7255, 0x00);
      /*$76AF*/ ram_poke(0x725e, 0x02);
      /*$76B4*/ ram_poke(0x725f, 0x00);
                block_id = 502;
      break;
    case 502:  // $76B7
      /*$76B7*/ CYCLES(0x76b7, 19);
      /*$76BA*/ ram_poke(0x78b2, ram_peek(0x725e));
      /*$76BF*/ ram_poke(0x7263, 0x10);
                block_id = 503;
      break;
    case 503:  // $76C2
      /*$76C2*/ CYCLES(0x76c2, 9);
      /*$76C4*/ ram_poke(0x7264, 0x00);
                block_id = 504;
      break;
    case 504:  // $76C7
      /*$76C7*/ CYCLES(0x76c7, 50);
      /*$76C9*/ ram_poke(0x725f, 0x00);
      /*$76CC*/ ram_poke(0x7260, 0x00);
      /*$76CF*/ ram_poke(0x7261, 0x00);
      /*$76D2*/ ram_poke(0x7262, 0x00);
      /*$76D8*/ ram_poke(0x725a, ram_peek(0x7263));
      /*$76DE*/ ram_poke(0x725b, ram_peek(0x7264));
      /*$76E1*/ func_7045(0xfffe);
                branchTarget = true; block_id = 505;
      break;
    case 505:  // $76E4
      /*$76E4*/ CYCLES(0x76e4, 16);
      /*$76E7*/ ram_poke(0x7266, ram_peek(0x0304));
      /*$76EA*/ func_71cd(0xfffe);
                branchTarget = true; block_id = 506;
      break;
    case 506:  // $76ED
      /*$76ED*/ CYCLES(0x76ed, 16);
                tmp2_U8 = io_peek(0xc054);
      /*$76F0*/ tmp2_U8 = io_peek(0xc053);
      /*$76F3*/ func_7642(0xfffe);
                branchTarget = true; block_id = 507;
      break;
    case 507:  // $76F6
      /*$76F6*/ CYCLES(0x76f6, 6);
                func_60e4(0xfffe);
                branchTarget = true; block_id = 508;
      break;
    case 508:  // $76F9
      /*$76F9*/ CYCLES(0x76f9, 12);
      /*$76FB*/ ram_poke(0x0300, 0x52);
      /*$76FE*/ s_a = 0x00;
      /*$7700*/ CYCLES(0x7700, 28);
                ram_poke(0x0305, 0x00);
      /*$7706*/ ram_poke(0x6255, ram_peek(0x7266));
      /*$770B*/ ram_poke(0x0022, 0x14);
      /*$770D*/ rom_home(0xfffe);
                branchTarget = true; block_id = 509;
      break;
    case 509:  // $7710
      /*$7710*/ CYCLES(0x7710, 6);
                branchTarget = true; push16(0x7712); block_id = 402;
      break;
    case 510:  // $7713
      /*$7713*/ CYCLES(0x7713, 6);
                branchTarget = true; push16(0x7715); block_id = 43;
      break;
    case 511:  // $7716
      /*$7716*/ CYCLES(0x7716, 6);
                branchTarget = true; block_id = 520;
      break;
    case 512:  // $7719
      /*$7719*/ CYCLES(0x7719, 23);
      /*$771C*/ ram_poke(0x6255, ram_peek(0x7266));
      /*$7721*/ ram_poke(0x0022, 0x14);
      /*$7723*/ rom_home(0xfffe);
                branchTarget = true; block_id = 513;
      break;
    case 513:  // $7726
      /*$7726*/ CYCLES(0x7726, 6);
                branchTarget = true; push16(0x7728); block_id = 402;
      break;
    case 514:  // $7729
      /*$7729*/ CYCLES(0x7729, 12);
                tmp1_U8 = ram_peek(0x0300);
                s_a = tmp1_U8;
      /*$772C*/ tmp2_U8 = tmp1_U8 >= 0x03;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - 0x03) & 0x80);
      /*$772E*/ branchTarget = true; block_id = !tmp2_U8 ? 519 : 515;
      break;
    case 515:  // $7730
      /*$7730*/ CYCLES(0x7730, 11);
                s_status_c = 0x01;
      /*$7731*/ block_id = s_status_d ? 517 : 516;
      break;
    case 516:  // $7731
      /*$7731*/ tmp4_U16 = s_a;
                tmp3_U16 = (tmp4_U16 - 0x0002) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0xfffd);
                tmp2_U8 = (uint8_t)tmp3_U16;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 518;
      break;
    case 517:  // $7731
      /*$7731*/ tmp3_U16 = sbc_dec16(s_a, 0x02, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 518;
      break;
    case 518:  // $7733
      /*$7733*/ ram_poke(0x0300, s_a);
                block_id = 519;
      break;
    case 519:  // $7736
      /*$7736*/ CYCLES(0x7736, 6);
                branchTarget = true; push16(0x7738); block_id = 47;
      break;
    case 520:  // $7739
      /*$7739*/ CYCLES(0x7739, 12);
                tmp5_U8 = ram_peek(0x6253);
                s_a = tmp5_U8;
      /*$773E*/ branchTarget = true; block_id = !(tmp5_U8 != 0x0f) ? 522 : 521;
      break;
    case 521:  // $7740
      /*$7740*/ CYCLES(0x7740, 6);
      /*$77E6*/ CYCLES(0x77e6, 7);
      /*$77E8*/ branchTarget = true; block_id = (s_a != 0x00) ? 555 : 552;
      break;
    case 522:  // $7743
      /*$7743*/ CYCLES(0x7743, 99);
                s_status_d = 0x01;
      /*$7748*/ tmp3_U16 = sbc_dec16(ram_peek(0x725f), 0x01, 0x01);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
      /*$774A*/ ram_poke(0x725f, ((uint8_t)tmp3_U16));
      /*$774D*/ s_a = ram_peek(0x7260);
      /*$7750*/ block_id = 0x01 ? 524 : 523;
      break;
    case 523:  // $7750
      /*$7750*/ s_a = (uint8_t)(s_a - (uint8_t)(0x01 - s_status_c));
                block_id = 525;
      break;
    case 524:  // $7750
      /*$7750*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x00, s_status_c));
                block_id = 525;
      break;
    case 525:  // $7752
      /*$7752*/ ram_poke(0x7260, s_a);
      /*$7755*/ s_a = ram_peek(0x725a);
      /*$7758*/ s_status_c = 0x01;
      /*$7759*/ block_id = s_status_d ? 527 : 526;
      break;
    case 526:  // $7759
      /*$7759*/ tmp3_U16 = (s_a - 0x0001) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp3_U16);
                block_id = 528;
      break;
    case 527:  // $7759
      /*$7759*/ tmp3_U16 = sbc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 528;
      break;
    case 528:  // $775B
      /*$775B*/ ram_poke(0x725a, s_a);
      /*$775E*/ s_a = ram_peek(0x725b);
      /*$7761*/ block_id = s_status_d ? 530 : 529;
      break;
    case 529:  // $7761
      /*$7761*/ s_a = (uint8_t)(s_a - (uint8_t)(0x01 - s_status_c));
                block_id = 531;
      break;
    case 530:  // $7761
      /*$7761*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x00, s_status_c));
                block_id = 531;
      break;
    case 531:  // $7763
      /*$7763*/ ram_poke(0x725b, s_a);
      /*$7766*/ s_a = ram_peek(0x7261);
      /*$7769*/ s_status_c = 0x00;
      /*$776A*/ block_id = s_status_d ? 533 : 532;
      break;
    case 532:  // $776A
      /*$776A*/ tmp3_U16 = (s_a + 0x0001) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 534;
      break;
    case 533:  // $776A
      /*$776A*/ tmp3_U16 = adc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 534;
      break;
    case 534:  // $776C
      /*$776C*/ ram_poke(0x7261, s_a);
      /*$776F*/ s_a = ram_peek(0x7262);
      /*$7772*/ block_id = s_status_d ? 536 : 535;
      break;
    case 535:  // $7772
      /*$7772*/ s_a = (uint8_t)(s_a + s_status_c);
                block_id = 537;
      break;
    case 536:  // $7772
      /*$7772*/ s_a = ((uint8_t)adc_dec16(s_a, 0x00, s_status_c));
                block_id = 537;
      break;
    case 537:  // $7774
      /*$7774*/ ram_poke(0x7262, s_a);
      /*$7777*/ s_status_d = 0x00;
      /*$777B*/ branchTarget = true; block_id = ram_peek(0x7262) ? 540 : 538;
      break;
    case 538:  // $777D
      /*$777D*/ CYCLES(0x777d, 12);
      /*$7782*/ branchTarget = true; block_id = (ram_peek(0x7261) >= 0x11) ? 540 : 539;
      break;
    case 539:  // $7784
      /*$7784*/ CYCLES(0x7784, 6);
                func_7267(0xfffe);
                branchTarget = true; block_id = 540;
      break;
    case 540:  // $7787
      /*$7787*/ CYCLES(0x7787, 24);
                s_a = ram_peek(0x6254);
      /*$778A*/ s_status_c = 0x00;
      /*$778B*/ block_id = s_status_d ? 542 : 541;
      break;
    case 541:  // $778B
      /*$778B*/ tmp3_U16 = s_a;
                tmp4_U16 = (tmp3_U16 + 0x000a) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x000a);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 543;
      break;
    case 542:  // $778B
      /*$778B*/ tmp4_U16 = adc_dec16(s_a, 0x0a, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 543;
      break;
    case 543:  // $778D
      /*$778D*/ ram_poke(0x6254, s_a);
      /*$7793*/ branchTarget = true; block_id = ram_peek(0x725a) ? 546 : 544;
      break;
    case 544:  // $7795
      /*$7795*/ CYCLES(0x7795, 9);
      /*$7798*/ branchTarget = true; block_id = ram_peek(0x725b) ? 546 : 545;
      break;
    case 545:  // $779A
      /*$779A*/ CYCLES(0x779a, 40);
      /*$779C*/ ram_poke(0x0001, 0x06);
      /*$77A0*/ ram_poke(0x0003, 0x00);
      /*$77A4*/ ram_poke(0x0002, 0x12);
      /*$77A8*/ ram_poke(0x0008, 0x16);
      /*$77AC*/ ram_poke(0x0000, 0x15);
      /*$77AE*/ func_6148(0xfffe);
      /*$77B1*/ CYCLES(0x77b1, 19);
      /*$77B3*/ ram_poke(0x0001, 0x00);
      /*$77B7*/ ram_poke(0x0002, 0x14);
      /*$77B9*/ func_60e4(0xfffe);
      /*$77BC*/ CYCLES(0x77bc, 18);
      /*$77BE*/ ram_poke(0x7266, 0xff);
      /*$77C1*/ s_a = 0x00;
      /*$77C3*/ rom_setcol(0xfffe);
      /*$77C6*/ CYCLES(0x77c6, 12);
                s_a = 0x00;
      /*$77C8*/ s_y = 0x14;
      /*$77CA*/ rom_plot(0xfffe);
      /*$77CD*/ CYCLES(0x77cd, 6);
                branchTarget = true; block_id = 512;
      break;
    case 546:  // $77D0
      /*$77D0*/ CYCLES(0x77d0, 9);
      /*$77D3*/ branchTarget = true; block_id = !ram_peek(0x725f) ? 548 : 547;
      break;
    case 547:  // $77D5
      /*$77D5*/ CYCLES(0x77d5, 6);
                branchTarget = true; block_id = 512;
      break;
    case 548:  // $77D8
      /*$77D8*/ CYCLES(0x77d8, 9);
                tmp2_U8 = ram_peek(0x7260);
                s_a = tmp2_U8;
      /*$77DB*/ branchTarget = true; block_id = !tmp2_U8 ? 550 : 549;
      break;
    case 549:  // $77DD
      /*$77DD*/ CYCLES(0x77dd, 6);
                branchTarget = true; block_id = 512;
      break;
    case 550:  // $77E0
      /*$77E0*/ CYCLES(0x77e0, 6);
                func_7642(0xfffe);
                branchTarget = true; block_id = 551;
      break;
    case 551:  // $77E3
      /*$77E3*/ CYCLES(0x77e3, 6);
                branchTarget = true; block_id = 512;
      break;
    case 552:  // $77EA
      /*$77EA*/ CYCLES(0x77ea, 38);
      /*$77F1*/ ram_poke(0x7265, ((uint8_t)adc_dec16(ram_peek(0x7265), 0x01, 0x00)));
      /*$77F5*/ ram_poke(0x0303, (uint8_t)(ram_peek(0x0303) + 0x01));
      /*$77FE*/ branchTarget = true; block_id = (ram_peek(0x725e) != ram_peek(0x78b2)) ? 554 : 553;
      break;
    case 553:  // $7800
      /*$7800*/ CYCLES(0x7800, 6);
                branchTarget = true; push16(0x7802); block_id = 582;
      break;
    case 554:  // $7803
      /*$7803*/ CYCLES(0x7803, 6);
                func_7633(0xfffe);
      /*$7806*/ CYCLES(0x7806, 6);
                branchTarget = true; block_id = 502;
      break;
    case 555:  // $7809
      /*$7809*/ CYCLES(0x7809, 7);
      /*$780B*/ branchTarget = true; block_id = (s_a != 0xfe) ? 568 : 556;
      break;
    case 556:  // $780D
      /*$780D*/ CYCLES(0x780d, 9);
      /*$7810*/ branchTarget = true; block_id = ram_peek(0x725b) ? 558 : 557;
      break;
    case 557:  // $7812
      /*$7812*/ CYCLES(0x7812, 9);
      /*$7815*/ branchTarget = true; block_id = !ram_peek(0x725a) ? 568 : 558;
      break;
    case 558:  // $7817
      /*$7817*/ CYCLES(0x7817, 67);
                s_status_d = 0x01;
      /*$781C*/ tmp4_U16 = adc_dec16(ram_peek(0x7263), 0x03, 0x00);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
      /*$781E*/ ram_poke(0x7263, ((uint8_t)tmp4_U16));
      /*$7821*/ s_a = ram_peek(0x7264);
      /*$7824*/ block_id = 0x01 ? 560 : 559;
      break;
    case 559:  // $7824
      /*$7824*/ s_a = (uint8_t)(s_a + s_status_c);
                block_id = 561;
      break;
    case 560:  // $7824
      /*$7824*/ s_a = ((uint8_t)adc_dec16(s_a, 0x00, s_status_c));
                block_id = 561;
      break;
    case 561:  // $7826
      /*$7826*/ ram_poke(0x7264, s_a);
      /*$7829*/ s_a = ram_peek(0x725a);
      /*$782C*/ s_status_c = 0x00;
      /*$782D*/ block_id = s_status_d ? 563 : 562;
      break;
    case 562:  // $782D
      /*$782D*/ tmp4_U16 = (s_a + 0x0003) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 564;
      break;
    case 563:  // $782D
      /*$782D*/ tmp4_U16 = adc_dec16(s_a, 0x03, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 564;
      break;
    case 564:  // $782F
      /*$782F*/ ram_poke(0x725a, s_a);
      /*$7832*/ s_a = ram_peek(0x725b);
      /*$7835*/ block_id = s_status_d ? 566 : 565;
      break;
    case 565:  // $7835
      /*$7835*/ tmp4_U16 = s_a;
                tmp3_U16 = tmp4_U16 + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 567;
      break;
    case 566:  // $7835
      /*$7835*/ tmp3_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 567;
      break;
    case 567:  // $7837
      /*$7837*/ ram_poke(0x725b, s_a);
      /*$783A*/ s_status_d = 0x00;
      /*$783B*/ func_7642(0xfffe);
      /*$783E*/ CYCLES(0x783e, 6);
                func_7642(0xfffe);
      /*$7841*/ CYCLES(0x7841, 6);
                func_7642(0xfffe);
      /*$7844*/ CYCLES(0x7844, 6);
                branchTarget = true; block_id = 512;
      break;
    case 568:  // $7847
      /*$7847*/ CYCLES(0x7847, 12);
                tmp2_U8 = ram_peek(0x6253);
                s_a = tmp2_U8;
      /*$784C*/ branchTarget = true; block_id = (tmp2_U8 != 0xff) ? 570 : 569;
      break;
    case 569:  // $784E
      /*$784E*/ CYCLES(0x784e, 6);
                branchTarget = true; block_id = 499;
      break;
    case 570:  // $7851
      /*$7851*/ CYCLES(0x7851, 7);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0xfe);
      /*$7853*/ branchTarget = true; block_id = (tmp2_U8 != 0xfe) ? 572 : 571;
      break;
    case 571:  // $7855
      /*$7855*/ CYCLES(0x7855, 6);
                branchTarget = true; block_id = 512;
      break;
    case 572:  // $7858
      /*$7858*/ CYCLES(0x7858, 9);
      /*$785B*/ branchTarget = true; block_id = ram_peek(0x0302) ? 579 : 573;
      break;
    case 573:  // $785D
      /*$785D*/ CYCLES(0x785d, 19);
      /*$785F*/ ram_poke(0x0025, 0x17);
      /*$7863*/ ram_poke(0x0024, 0x00);
      /*$7865*/ branchTarget = true; push16(0x7867); block_id = 375;
      break;
    case 574:  // $7868
      /*$7868*/ CYCLES(0x7868, 2);
                fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x7868, 0x7868, 0x03); error_handler(0x7868);
      break;
    case 575:  // $7886
      /*$7886*/ CYCLES(0x7886, 9);
      /*$7889*/ branchTarget = true; block_id = !ram_peek(0x6c71) ? 577 : 576;
      break;
    case 576:  // $788B
      /*$788B*/ CYCLES(0x788b, 9);
                tmp2_U8 = io_peek(0xc061);
      /*$788E*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 579 : 577;
      break;
    case 577:  // $7890
      /*$7890*/ CYCLES(0x7890, 12);
                tmp2_U8 = io_peek(0xc000);
                s_a = tmp2_U8;
      /*$7895*/ branchTarget = true; block_id = (tmp2_U8 != 0xa0) ? 575 : 578;
      break;
    case 578:  // $7897
      /*$7897*/ CYCLES(0x7897, 6);
                io_poke(0xc010, s_a);
                block_id = 579;
      break;
    case 579:  // $789A
      /*$789A*/ CYCLES(0x789a, 9);
      /*$789D*/ branchTarget = true; block_id = ram_peek(0x725e) ? 581 : 580;
      break;
    case 580:  // $789F
      /*$789F*/ CYCLES(0x789f, 6);
                branchTarget = true; block_id = 499;
      break;
    case 581:  // $78A2
      /*$78A2*/ CYCLES(0x78a2, 24);
      /*$78A7*/ tmp3_U16 = sbc_dec16(ram_peek(0x725e), 0x01, 0x01);
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
      /*$78A9*/ ram_poke(0x725e, ((uint8_t)tmp3_U16));
      /*$78AC*/ s_status_d = 0x00;
      /*$78AD*/ branchTarget = true; block_id = 504;
      break;
    case 582:  // $78B3
      /*$78B3*/ CYCLES(0x78b3, 41);
      /*$78B8*/ tmp4_U16 = adc_dec16(ram_peek(0x71cb), ram_peek(0x71cb), 0x00);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
      /*$78BB*/ ram_poke(0x78b0, ((uint8_t)tmp4_U16));
      /*$78BE*/ s_a = ram_peek(0x71cc);
      /*$78C1*/ block_id = 0x01 ? 584 : 583;
      break;
    case 583:  // $78C1
      /*$78C1*/ s_a = (uint8_t)((s_a + ram_peek(0x71cc)) + s_status_c);
                block_id = 585;
      break;
    case 584:  // $78C1
      /*$78C1*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x71cc), s_status_c));
                block_id = 585;
      break;
    case 585:  // $78C4
      /*$78C4*/ ram_poke(0x78b1, s_a);
      /*$78C8*/ func_7267(0xfffe);
      /*$78CB*/ CYCLES(0x78cb, 6);
                func_7267(0xfffe);
      /*$78CE*/ CYCLES(0x78ce, 6);
                branchTarget = true; push16(0x78d0); block_id = 402;
      break;
    case 586:  // $78D1
      /*$78D1*/ CYCLES(0x78d1, 40);
      /*$78D3*/ ram_poke(0x0000, 0x01);
      /*$78D7*/ ram_poke(0x0001, 0x09);
      /*$78DB*/ ram_poke(0x0002, 0x0d);
      /*$78DF*/ ram_poke(0x0003, 0x10);
      /*$78E3*/ ram_poke(0x0008, 0x1a);
      /*$78E5*/ func_6148(0xfffe);
      /*$78E8*/ CYCLES(0x78e8, 19);
      /*$78EA*/ ram_poke(0x0002, 0x0d);
      /*$78EE*/ ram_poke(0x0003, 0x15);
      /*$78F0*/ func_6148(0xfffe);
      /*$78F3*/ CYCLES(0x78f3, 19);
      /*$78F5*/ ram_poke(0x0003, 0x10);
      /*$78F9*/ ram_poke(0x0008, 0x15);
      /*$78FB*/ func_615a(0xfffe);
      /*$78FE*/ CYCLES(0x78fe, 19);
      /*$7900*/ ram_poke(0x0002, 0x0d);
      /*$7904*/ ram_poke(0x0003, 0x10);
      /*$7906*/ func_615a(0xfffe);
      /*$7909*/ CYCLES(0x7909, 33);
      /*$790B*/ ram_poke(0x0001, 0x00);
      /*$790F*/ ram_poke(0x0008, 0x19);
      /*$7913*/ ram_poke(0x0003, 0x11);
      /*$7917*/ ram_poke(0x0002, 0x0e);
      /*$7919*/ func_6148(0xfffe);
      /*$791C*/ CYCLES(0x791c, 16);
      /*$791E*/ ram_poke(0x0002, 0x0e);
      /*$7920*/ ram_poke(0x0003, (uint8_t)(ram_peek(0x0003) + 0x01));
      /*$7922*/ func_6148(0xfffe);
      /*$7925*/ CYCLES(0x7925, 16);
      /*$7927*/ ram_poke(0x0002, 0x0e);
      /*$7929*/ ram_poke(0x0003, (uint8_t)(ram_peek(0x0003) + 0x01));
      /*$792B*/ func_6148(0xfffe);
      /*$792E*/ CYCLES(0x792e, 16);
      /*$7930*/ ram_poke(0x0002, 0x0e);
      /*$7932*/ ram_poke(0x0003, (uint8_t)(ram_peek(0x0003) + 0x01));
      /*$7934*/ func_6148(0xfffe);
      /*$7937*/ CYCLES(0x7937, 19);
      /*$7939*/ ram_poke(0x0024, 0x0f);
      /*$793D*/ ram_poke(0x0025, 0x09);
      /*$793F*/ func_6641(0xfffe);
      /*$7942*/ CYCLES(0x7942, 6);
                branchTarget = true; push16(0x7944); block_id = 375;
      break;
    case 587:  // $7945
      /*$7945*/ CYCLES(0x7945, 2);
                fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x7945, 0x7945, 0x03); error_handler(0x7945);
      break;
    case 588:  // $794D
      /*$794D*/ CYCLES(0x794d, 18);
      /*$794F*/ ram_poke(0x002c, 0x00);
      /*$7951*/ s_a = ram_peek(0x78b1);
      /*$7954*/ func_71f3(0xfffe);
      /*$7957*/ CYCLES(0x7957, 11);
                s_a = ram_peek(0x78b0);
      /*$795A*/ func_71f3(0xfffe);
      /*$795D*/ CYCLES(0x795d, 21);
      /*$795F*/ ram_poke(0x0036, 0xf0);
      /*$7963*/ ram_poke(0x0037, 0xfd);
      /*$7967*/ ram_poke(0x0002, 0x20);
                block_id = 589;
      break;
    case 589:  // $7969
      /*$7969*/ CYCLES(0x7969, 4);
                s_x = 0x80;
                block_id = 590;
      break;
    case 590:  // $796B
      /*$796B*/ CYCLES(0x796b, 4);
      /*$796C*/ s_y = s_x;
                block_id = 591;
      break;
    case 591:  // $796D
      /*$796D*/ CYCLES(0x796d, 6);
                tmp2_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp2_U8;
      /*$796E*/ branchTarget = true; block_id = tmp2_U8 ? 591 : 592;
      break;
    case 592:  // $7970
      /*$7970*/ CYCLES(0x7970, 16);
                tmp2_U8 = ram_peek(0x6c49);
                s_y = tmp2_U8;
      /*$7973*/ tmp2_U8 = peek((0xc000 + tmp2_U8));
                s_a = tmp2_U8;
      /*$7976*/ tmp2_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp2_U8;
      /*$7977*/ branchTarget = true; block_id = tmp2_U8 ? 590 : 593;
      break;
    case 593:  // $7979
      /*$7979*/ CYCLES(0x7979, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x0002) - 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x0002, tmp2_U8);
      /*$797B*/ branchTarget = true; block_id = tmp2_U8 ? 589 : 594;
      break;
    case 594:  // $797D
      /*$797D*/ CYCLES(0x797d, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x797d, pop16() + 1);;
      break;
    case 595:  // $7980
      /*$7980*/ CYCLES(0x7980, 11);
                tmp2_U8 = ram_peek(0x000f);
                s_a = tmp2_U8;
      /*$7982*/ tmp2_U8 = tmp2_U8 >= 0x1f;
                s_status_c = tmp2_U8;
      /*$7984*/ branchTarget = true; block_id = tmp2_U8 ? 597 : 596;
      break;
    case 596:  // $7986
      /*$7986*/ CYCLES(0x7986, 7);
                tmp2_U8 = s_a >= 0x18;
                s_status_c = tmp2_U8;
      /*$7988*/ branchTarget = true; block_id = tmp2_U8 ? 598 : 597;
      break;
    case 597:  // $798A
      /*$798A*/ CYCLES(0x798a, 11);
      /*$798E*/ ram_poke(0x000e, (ram_peek(0x000e) & 0xde));
                block_id = 598;
      break;
    case 598:  // $7990
      /*$7990*/ CYCLES(0x7990, 19);
      /*$7996*/ ram_poke(0x000f, ((ram_peek(0x000f) & 0x1f) | 0x18));
      /*$73D8*/ CYCLES(0x73d8, 9);
      /*$73DB*/ branchTarget = true; block_id = ram_peek(0x73d7) ? 443 : 442;
      break;
    case 599:  // $D393
      /*$D393*/ CYCLES(0xd393, 6);
                branchTarget = true; push16(0xd395); block_id = 629;
      break;
    case 600:  // $D396
      /*$D396*/ CYCLES(0xd396, 7);
                ram_poke(0x006d, s_a);
      /*$D398*/ ram_poke(0x006e, s_y);
                FUNC_MVBLKUP2(0x0000);
                block_id = find_block_id_func_t001(0xd398, pop16() + 1);;
      break;
    case 601:  // $D39A
      /*$D39A*/ CYCLES(0xd39a, 29);
                s_status_c = 0x01;
      /*$D39B*/ s_a = ram_peek(0x0096);
      /*$D39D*/ block_id = s_status_d ? 603 : 602;
      break;
    case 602:  // $D39D
      /*$D39D*/ tmp3_U16 = (s_a - ram_peek(0x009b)) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp3_U16);
                block_id = 604;
      break;
    case 603:  // $D39D
      /*$D39D*/ tmp3_U16 = sbc_dec16(s_a, ram_peek(0x009b), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 604;
      break;
    case 604:  // $D39F
      /*$D39F*/ tmp5_U8 = s_a;
                ram_poke(0x005e, tmp5_U8);
      /*$D3A1*/ s_y = tmp5_U8;
      /*$D3A2*/ s_a = ram_peek(0x0097);
      /*$D3A4*/ block_id = s_status_d ? 606 : 605;
      break;
    case 605:  // $D3A4
      /*$D3A4*/ tmp3_U16 = s_a;
                tmp4_U16 = ram_peek(0x009c);
                tmp6_U16 = (tmp3_U16 - tmp4_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp4_U16));
                s_a = ((uint8_t)tmp6_U16);
                block_id = 607;
      break;
    case 606:  // $D3A4
      /*$D3A4*/ tmp6_U16 = sbc_dec16(s_a, ram_peek(0x009c), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 607;
      break;
    case 607:  // $D3A6
      /*$D3A7*/ s_x = (uint8_t)(s_a + 0x01);
      /*$D3A8*/ tmp5_U8 = s_y;
                s_a = tmp5_U8;
      /*$D3A9*/ branchTarget = true; block_id = !tmp5_U8 ? 621 : 608;
      break;
    case 608:  // $D3AB
      /*$D3AB*/ CYCLES(0xd3ab, 16);
                s_a = ram_peek(0x0096);
      /*$D3AD*/ s_status_c = 0x01;
      /*$D3AE*/ block_id = s_status_d ? 610 : 609;
      break;
    case 609:  // $D3AE
      /*$D3AE*/ tmp6_U16 = (s_a - ram_peek(0x005e)) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp6_U16);
                block_id = 611;
      break;
    case 610:  // $D3AE
      /*$D3AE*/ tmp6_U16 = sbc_dec16(s_a, ram_peek(0x005e), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 611;
      break;
    case 611:  // $D3B0
      /*$D3B0*/ ram_poke(0x0096, s_a);
      /*$D3B2*/ branchTarget = true; block_id = s_status_c ? 613 : 612;
      break;
    case 612:  // $D3B4
      /*$D3B4*/ CYCLES(0xd3b4, 6);
                ram_poke(0x0097, (uint8_t)(ram_peek(0x0097) - 0x01));
      /*$D3B6*/ s_status_c = 0x01;
                block_id = 613;
      break;
    case 613:  // $D3B7
      /*$D3B7*/ CYCLES(0xd3b7, 14);
                s_a = ram_peek(0x0094);
      /*$D3B9*/ block_id = s_status_d ? 615 : 614;
      break;
    case 614:  // $D3B9
      /*$D3B9*/ tmp6_U16 = s_a;
                tmp4_U16 = ram_peek(0x005e);
                tmp3_U16 = (tmp6_U16 - tmp4_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)(~tmp4_U16));
                s_a = ((uint8_t)tmp3_U16);
                block_id = 616;
      break;
    case 615:  // $D3B9
      /*$D3B9*/ tmp3_U16 = sbc_dec16(s_a, ram_peek(0x005e), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 616;
      break;
    case 616:  // $D3BB
      /*$D3BB*/ ram_poke(0x0094, s_a);
      /*$D3BD*/ branchTarget = true; block_id = s_status_c ? 619 : 617;
      break;
    case 617:  // $D3BF
      /*$D3BF*/ CYCLES(0xd3bf, 7);
                ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) - 0x01));
      /*$D3C1*/ branchTarget = true; block_id = !s_status_c ? 619 : 618;
      break;
    case 618:  // $D3C3
      /*$D3C3*/ CYCLES(0xd3c3, 7);
                tmp5_U8 = s_y;
                tmp1_U8 = peek((ram_peek16al(0x0096) + tmp5_U8));
      /*$D3C5*/ poke((ram_peek16al(0x0094) + tmp5_U8), tmp1_U8);
                block_id = 619;
      break;
    case 619:  // $D3C7
      /*$D3C7*/ CYCLES(0xd3c7, 6);
                tmp5_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp5_U8;
      /*$D3C8*/ branchTarget = true; block_id = tmp5_U8 ? 618 : 620;
      break;
    case 620:  // $D3CA
      /*$D3CA*/ CYCLES(0xd3ca, 7);
                tmp5_U8 = s_y;
                tmp1_U8 = peek((ram_peek16al(0x0096) + tmp5_U8));
                s_a = tmp1_U8;
      /*$D3CC*/ poke((ram_peek16al(0x0094) + tmp5_U8), tmp1_U8);
                block_id = 621;
      break;
    case 621:  // $D3CE
      /*$D3CE*/ CYCLES(0xd3ce, 12);
                ram_poke(0x0097, (uint8_t)(ram_peek(0x0097) - 0x01));
      /*$D3D0*/ ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) - 0x01));
      /*$D3D2*/ tmp5_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_x = tmp5_U8;
      /*$D3D3*/ branchTarget = true; block_id = tmp5_U8 ? 619 : 622;
      break;
    case 622:  // $D3D5
      /*$D3D5*/ CYCLES(0xd3d5, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd3d5, pop16() + 1);;
      break;
    case 623:  // $D3D6
      /*$D3D6*/ CYCLES(0xd3d6, 9);
                tmp3_U16 = s_a << 0x01;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
      /*$D3D7*/ block_id = s_status_d ? 625 : 624;
      break;
    case 624:  // $D3D7
      /*$D3D7*/ tmp3_U16 = s_a;
                tmp4_U16 = (tmp3_U16 + 0x0036) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x0036);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 626;
      break;
    case 625:  // $D3D7
      /*$D3D7*/ tmp4_U16 = adc_dec16(s_a, 0x36, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 626;
      break;
    case 626:  // $D3D9
      /*$D3D9*/ branchTarget = true; block_id = s_status_c ? 640 : 627;
      break;
    case 627:  // $D3DB
      /*$D3DB*/ CYCLES(0xd3db, 12);
                ram_poke(0x005e, s_a);
      /*$D3DD*/ tmp5_U8 = s_sp;
                s_x = tmp5_U8;
      /*$D3DE*/ tmp1_U8 = ram_peek(0x005e);
                s_status_not_z = (tmp5_U8 != tmp1_U8);
                tmp2_U8 = tmp5_U8 >= tmp1_U8;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp5_U8 - tmp1_U8) & 0x80);
      /*$D3E0*/ branchTarget = true; block_id = !tmp2_U8 ? 640 : 628;
      break;
    case 628:  // $D3E2
      /*$D3E2*/ CYCLES(0xd3e2, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd3e2, pop16() + 1);;
      break;
    case 629:  // $D3E3
      /*$D3E3*/ CYCLES(0xd3e3, 7);
                tmp2_U8 = s_y;
                tmp1_U8 = ram_peek(0x0070);
                s_status_not_z = (tmp2_U8 != tmp1_U8);
                tmp5_U8 = tmp2_U8 >= tmp1_U8;
                s_status_c = tmp5_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
      /*$D3E5*/ branchTarget = true; block_id = !tmp5_U8 ? 639 : 630;
      break;
    case 630:  // $D3E7
      /*$D3E7*/ CYCLES(0xd3e7, 4);
                branchTarget = true; block_id = s_status_not_z ? 632 : 631;
      break;
    case 631:  // $D3E9
      /*$D3E9*/ CYCLES(0xd3e9, 7);
                tmp2_U8 = s_a;
                tmp1_U8 = ram_peek(0x006f);
                s_status_not_z = (tmp2_U8 != tmp1_U8);
                tmp5_U8 = tmp2_U8 >= tmp1_U8;
                s_status_c = tmp5_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
      /*$D3EB*/ branchTarget = true; block_id = !tmp5_U8 ? 639 : 632;
      break;
    case 632:  // $D3ED
      /*$D3ED*/ CYCLES(0xd3ed, 7);
                push8(s_a);
      /*$D3EE*/ s_x = 0x09;
      /*$D3F0*/ s_a = s_y;
                block_id = 633;
      break;
    case 633:  // $D3F1
      /*$D3F1*/ CYCLES(0xd3f1, 11);
                push8(s_a);
      /*$D3F2*/ tmp5_U8 = s_x;
                s_a = ram_peek((uint8_t)(0x93 + tmp5_U8));
      /*$D3F4*/ tmp5_U8 = (uint8_t)(tmp5_U8 - 0x01);
                s_x = tmp5_U8;
      /*$D3F5*/ branchTarget = true; block_id = !(tmp5_U8 & 0x80) ? 633 : 634;
      break;
    case 634:  // $D3F7
      /*$D3F7*/ CYCLES(0xd3f7, 6);
                FUNC_GARBAGE(0xfffe);
      /*$D3FA*/ CYCLES(0xd3fa, 4);
                s_x = 0xf7;
                block_id = 635;
      break;
    case 635:  // $D3FC
      /*$D3FC*/ CYCLES(0xd3fc, 11);
                tmp5_U8 = pop8();
      /*$D3FD*/ tmp1_U8 = s_x;
                ram_poke((uint8_t)(0x9d + tmp1_U8), tmp5_U8);
      /*$D3FF*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
                s_x = tmp1_U8;
      /*$D400*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 635 : 636;
      break;
    case 636:  // $D402
      /*$D402*/ CYCLES(0xd402, 12);
                tmp1_U8 = pop8();
      /*$D403*/ s_y = tmp1_U8;
      /*$D404*/ tmp5_U8 = pop8();
                s_a = tmp5_U8;
      /*$D405*/ tmp5_U8 = ram_peek(0x0070);
                s_status_not_z = (tmp1_U8 != tmp5_U8);
                tmp2_U8 = tmp1_U8 >= tmp5_U8;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - tmp5_U8) & 0x80);
      /*$D407*/ branchTarget = true; block_id = !tmp2_U8 ? 639 : 637;
      break;
    case 637:  // $D409
      /*$D409*/ CYCLES(0xd409, 4);
                branchTarget = true; block_id = s_status_not_z ? 640 : 638;
      break;
    case 638:  // $D40B
      /*$D40B*/ CYCLES(0xd40b, 7);
                tmp2_U8 = s_a;
                tmp5_U8 = ram_peek(0x006f);
                s_status_not_z = (tmp2_U8 != tmp5_U8);
                tmp1_U8 = tmp2_U8 >= tmp5_U8;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp5_U8) & 0x80);
      /*$D40D*/ branchTarget = true; block_id = tmp1_U8 ? 640 : 639;
      break;
    case 639:  // $D40F
      /*$D40F*/ CYCLES(0xd40f, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd40f, pop16() + 1);;
      break;
    case 640:  // $D410
      /*$D410*/ CYCLES(0xd410, 4);
                s_x = 0x4d;
                block_id = 641;
      break;
    case 641:  // $D412
      /*$D412*/ CYCLES(0xd412, 7);
                tmp1_U8 = ram_peek(0x00d8);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
      /*$D414*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 643 : 642;
      break;
    case 642:  // $D416
      /*$D416*/ CYCLES(0xd416, 6);
                branchTarget = true; block_id = 1803;
      break;
    case 643:  // $D419
      /*$D419*/ CYCLES(0xd419, 6);
                FUNC_CRDO(0xfffe);
      /*$D41C*/ CYCLES(0xd41c, 6);
                FUNC_OUTQUES(0xfffe);
                branchTarget = true; block_id = 644;
      break;
    case 644:  // $D41F
      /*$D41F*/ CYCLES(0xd41f, 12);
                tmp1_U8 = s_x;
                tmp5_U8 = ram_peek((0xd260 + tmp1_U8));
                s_a = tmp5_U8;
      /*$D422*/ push8(tmp5_U8);
      /*$D423*/ FUNC_OUTDO(0xfffe);
      /*$D426*/ CYCLES(0xd426, 7);
                s_x = (uint8_t)(tmp1_U8 + 0x01);
      /*$D427*/ tmp1_U8 = pop8();
      /*$D428*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 644 : 645;
      break;
    case 645:  // $D42A
      /*$D42A*/ CYCLES(0xd42a, 6);
                branchTarget = true; push16(0xd42c); block_id = 783;
      break;
    case 646:  // $D42D
      /*$D42D*/ CYCLES(0xd42d, 7);
                s_a = 0x50;
      /*$D42F*/ s_y = 0xd3;
                block_id = 647;
      break;
    case 647:  // $D431
      /*$D431*/ CYCLES(0xd431, 6);
                branchTarget = true; push16(0xd433); block_id = 918;
      break;
    case 648:  // $D434
      /*$D434*/ CYCLES(0xd434, 9);
      /*$D436*/ tmp5_U8 = (uint8_t)(ram_peek(0x0076) + 0x01);
                s_y = tmp5_U8;
      /*$D437*/ branchTarget = true; block_id = !tmp5_U8 ? 650 : 649;
      break;
    case 649:  // $D439
      /*$D439*/ CYCLES(0xd439, 6);
                branchTarget = true; push16(0xd43b); block_id = 1689;
      break;
    case 650:  // $D43C
      /*$D43C*/ CYCLES(0xd43c, 6);
                FUNC_CRDO(0xfffe);
                branchTarget = true; block_id = 651;
      break;
    case 651:  // $D43F
      /*$D43F*/ CYCLES(0xd43f, 9);
                s_x = 0xdd;
      /*$D441*/ branchTarget = true; push16(0xd443); block_id = 707;
      break;
    case 652:  // $D444
      /*$D444*/ CYCLES(0xd444, 16);
                ram_poke(0x00b8, s_x);
      /*$D446*/ ram_poke(0x00b9, s_y);
      /*$D448*/ tmp5_U8 = ram_peek(0x00d8);
                s_status_c = (tmp5_U8 & 0x01);
                ram_poke(0x00d8, (tmp5_U8 >> 0x01));
      /*$D44A*/ branchTarget = true; push16(0xd44c); block_id = 2;
      break;
    case 653:  // $D44D
      /*$D44D*/ CYCLES(0xd44d, 6);
                tmp1_U8 = s_a;
                s_x = tmp1_U8;
      /*$D44E*/ branchTarget = true; block_id = !tmp1_U8 ? 650 : 654;
      break;
    case 654:  // $D450
      /*$D450*/ CYCLES(0xd450, 11);
                s_x = 0xff;
      /*$D452*/ ram_poke(0x0076, 0xff);
      /*$D454*/ branchTarget = true; block_id = !s_status_c ? 657 : 655;
      break;
    case 655:  // $D456
      /*$D456*/ CYCLES(0xd456, 6);
                branchTarget = true; push16(0xd458); block_id = 715;
      break;
    case 656:  // $D459
      /*$D459*/ CYCLES(0xd459, 6);
                branchTarget = true; block_id = 801;
      break;
    case 657:  // $D45C
      /*$D45C*/ CYCLES(0xd45c, 19);
      /*$D45E*/ ram_poke(0x0069, ram_peek(0x00af));
      /*$D462*/ ram_poke(0x006a, ram_peek(0x00b0));
      /*$D464*/ branchTarget = true; push16(0xd466); block_id = 876;
      break;
    case 658:  // $D467
      /*$D467*/ CYCLES(0xd467, 6);
                branchTarget = true; push16(0xd469); block_id = 715;
      break;
    case 659:  // $D46A
      /*$D46A*/ CYCLES(0xd46a, 9);
                ram_poke(0x000f, s_y);
      /*$D46C*/ FUNC_FNDLIN(0xfffe);
      /*$D46F*/ CYCLES(0xd46f, 4);
                branchTarget = true; block_id = !s_status_c ? 684 : 660;
      break;
    case 660:  // $D471
      /*$D471*/ CYCLES(0xd471, 74);
      /*$D473*/ tmp1_U8 = peek((ram_peek16(0x009b) + 0x0001));
      /*$D475*/ ram_poke(0x005f, tmp1_U8);
      /*$D479*/ ram_poke(0x005e, ram_peek(0x0069));
      /*$D47D*/ ram_poke(0x0061, ram_peek(0x009c));
      /*$D47F*/ s_a = ram_peek(0x009b);
      /*$D481*/ s_y = 0x00;
      /*$D482*/ block_id = s_status_d ? 662 : 661;
      break;
    case 661:  // $D482
      /*$D482*/ tmp1_U8 = peek((ram_peek16(0x009b) + s_y));
                s_a = (uint8_t)((s_a - tmp1_U8) - (uint8_t)(0x01 - s_status_c));
                block_id = 663;
      break;
    case 662:  // $D482
      /*$D482*/ tmp1_U8 = peek((ram_peek16(0x009b) + s_y));
                s_a = ((uint8_t)sbc_dec16(s_a, tmp1_U8, s_status_c));
                block_id = 663;
      break;
    case 663:  // $D484
      /*$D484*/ s_status_c = 0x00;
      /*$D485*/ block_id = s_status_d ? 665 : 664;
      break;
    case 664:  // $D485
      /*$D485*/ tmp4_U16 = (s_a + ram_peek(0x0069)) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 666;
      break;
    case 665:  // $D485
      /*$D485*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x0069), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 666;
      break;
    case 666:  // $D487
      /*$D487*/ tmp1_U8 = s_a;
                ram_poke(0x0069, tmp1_U8);
      /*$D489*/ ram_poke(0x0060, tmp1_U8);
      /*$D48B*/ s_a = ram_peek(0x006a);
      /*$D48D*/ block_id = s_status_d ? 668 : 667;
      break;
    case 667:  // $D48D
      /*$D48D*/ tmp4_U16 = (s_a + 0x00ff) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 669;
      break;
    case 668:  // $D48D
      /*$D48D*/ tmp4_U16 = adc_dec16(s_a, 0xff, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 669;
      break;
    case 669:  // $D48F
      /*$D48F*/ ram_poke(0x006a, s_a);
      /*$D491*/ block_id = s_status_d ? 671 : 670;
      break;
    case 670:  // $D491
      /*$D491*/ s_a = (uint8_t)((s_a - ram_peek(0x009c)) - (uint8_t)(0x01 - s_status_c));
                block_id = 672;
      break;
    case 671:  // $D491
      /*$D491*/ s_a = ((uint8_t)sbc_dec16(s_a, ram_peek(0x009c), s_status_c));
                block_id = 672;
      break;
    case 672:  // $D493
      /*$D493*/ s_x = s_a;
      /*$D494*/ s_status_c = 0x01;
      /*$D495*/ s_a = ram_peek(0x009b);
      /*$D497*/ block_id = s_status_d ? 674 : 673;
      break;
    case 673:  // $D497
      /*$D497*/ tmp4_U16 = (s_a - ram_peek(0x0069)) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp4_U16);
                block_id = 675;
      break;
    case 674:  // $D497
      /*$D497*/ tmp4_U16 = sbc_dec16(s_a, ram_peek(0x0069), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 675;
      break;
    case 675:  // $D499
      /*$D499*/ s_y = s_a;
      /*$D49A*/ branchTarget = true; block_id = s_status_c ? 677 : 676;
      break;
    case 676:  // $D49C
      /*$D49C*/ CYCLES(0xd49c, 6);
                s_x = (uint8_t)(s_x + 0x01);
      /*$D49D*/ ram_poke(0x0061, (uint8_t)(ram_peek(0x0061) - 0x01));
                block_id = 677;
      break;
    case 677:  // $D49F
      /*$D49F*/ CYCLES(0xd49f, 9);
                s_status_c = 0x00;
      /*$D4A0*/ block_id = s_status_d ? 679 : 678;
      break;
    case 678:  // $D4A0
      /*$D4A0*/ tmp4_U16 = s_a;
                tmp3_U16 = ram_peek(0x005e);
                tmp6_U16 = (tmp4_U16 + tmp3_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)tmp3_U16);
                block_id = 680;
      break;
    case 679:  // $D4A0
      /*$D4A0*/ tmp1_U8 = (uint8_t)(adc_dec16(s_a, ram_peek(0x005e), s_status_c) >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 680;
      break;
    case 680:  // $D4A2
      /*$D4A2*/ branchTarget = true; block_id = !s_status_c ? 682 : 681;
      break;
    case 681:  // $D4A4
      /*$D4A4*/ CYCLES(0xd4a4, 6);
                ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) - 0x01));
      /*$D4A6*/ s_status_c = 0x00;
                block_id = 682;
      break;
    case 682:  // $D4A7
      /*$D4A7*/ CYCLES(0xd4a7, 12);
                tmp1_U8 = s_y;
                tmp5_U8 = peek((ram_peek16al(0x005e) + tmp1_U8));
      /*$D4A9*/ poke((ram_peek16al(0x0060) + tmp1_U8), tmp5_U8);
      /*$D4AB*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
                s_y = tmp1_U8;
      /*$D4AC*/ branchTarget = true; block_id = tmp1_U8 ? 682 : 683;
      break;
    case 683:  // $D4AE
      /*$D4AE*/ CYCLES(0xd4ae, 12);
                ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) + 0x01));
      /*$D4B0*/ ram_poke(0x0061, (uint8_t)(ram_peek(0x0061) + 0x01));
      /*$D4B2*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp1_U8;
      /*$D4B3*/ branchTarget = true; block_id = tmp1_U8 ? 682 : 684;
      break;
    case 684:  // $D4B5
      /*$D4B5*/ CYCLES(0xd4b5, 9);
      /*$D4B8*/ branchTarget = true; block_id = !ram_peek(0x0200) ? 693 : 685;
      break;
    case 685:  // $D4BA
      /*$D4BA*/ CYCLES(0xd4ba, 38);
      /*$D4BE*/ ram_poke(0x006f, ram_peek(0x0073));
      /*$D4C0*/ ram_poke(0x0070, ram_peek(0x0074));
      /*$D4C2*/ tmp1_U8 = ram_peek(0x0069);
                s_a = tmp1_U8;
      /*$D4C4*/ ram_poke(0x0096, tmp1_U8);
      /*$D4C6*/ block_id = s_status_d ? 687 : 686;
      break;
    case 686:  // $D4C6
      /*$D4C6*/ tmp3_U16 = s_a;
                tmp4_U16 = ram_peek(0x000f);
                tmp6_U16 = (tmp3_U16 + tmp4_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 688;
      break;
    case 687:  // $D4C6
      /*$D4C6*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x000f), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 688;
      break;
    case 688:  // $D4C8
      /*$D4C8*/ ram_poke(0x0094, s_a);
      /*$D4CA*/ tmp1_U8 = ram_peek(0x006a);
                s_y = tmp1_U8;
      /*$D4CC*/ ram_poke(0x0097, tmp1_U8);
      /*$D4CE*/ branchTarget = true; block_id = !s_status_c ? 690 : 689;
      break;
    case 689:  // $D4D0
      /*$D4D0*/ CYCLES(0xd4d0, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 690;
      break;
    case 690:  // $D4D1
      /*$D4D1*/ CYCLES(0xd4d1, 9);
                ram_poke(0x0095, s_y);
      /*$D4D3*/ branchTarget = true; push16(0xd4d5); block_id = 599;
      break;
    case 691:  // $D4D6
      /*$D4D6*/ CYCLES(0xd4d6, 35);
      /*$D4DA*/ ram_poke(0x01fe, ram_peek(0x0050));
      /*$D4DD*/ ram_poke(0x01ff, ram_peek(0x0051));
      /*$D4E4*/ ram_poke(0x0069, ram_peek(0x006d));
      /*$D4E6*/ ram_poke(0x006a, ram_peek(0x006e));
      /*$D4E8*/ s_y = ram_peek(0x000f);
                block_id = 692;
      break;
    case 692:  // $D4EA
      /*$D4EA*/ CYCLES(0xd4ea, 14);
                tmp1_U8 = s_y;
      /*$D4ED*/ tmp5_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_y = tmp5_U8;
      /*$D4EE*/ poke((ram_peek16(0x009b) + tmp5_U8), ram_peek((0x01fb + tmp1_U8)));
      /*$D4F0*/ branchTarget = true; block_id = tmp5_U8 ? 692 : 693;
      break;
    case 693:  // $D4F2
      /*$D4F2*/ CYCLES(0xd4f2, 6);
                branchTarget = true; push16(0xd4f4); block_id = 781;
      break;
    case 694:  // $D4F5
      /*$D4F5*/ CYCLES(0xd4f5, 16);
      /*$D4F9*/ ram_poke(0x005e, ram_peek(0x0067));
      /*$D4FB*/ ram_poke(0x005f, ram_peek(0x0068));
      /*$D4FD*/ s_status_c = 0x00;
                block_id = 695;
      break;
    case 695:  // $D4FE
      /*$D4FE*/ CYCLES(0xd4fe, 11);
                s_y = 0x01;
      /*$D500*/ tmp5_U8 = peek((ram_peek16al(0x005e) + 0x0001));
      /*$D502*/ branchTarget = true; block_id = tmp5_U8 ? 697 : 696;
      break;
    case 696:  // $D504
      /*$D504*/ CYCLES(0xd504, 19);
      /*$D506*/ ram_poke(0x00af, ram_peek(0x0069));
      /*$D50A*/ ram_poke(0x00b0, ram_peek(0x006a));
      /*$D50C*/ branchTarget = true; block_id = 650;
      break;
    case 697:  // $D50F
      /*$D50F*/ CYCLES(0xd50f, 4);
                s_y = 0x04;
                block_id = 698;
      break;
    case 698:  // $D511
      /*$D511*/ CYCLES(0xd511, 9);
                tmp5_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp5_U8;
      /*$D512*/ tmp5_U8 = peek((ram_peek16al(0x005e) + tmp5_U8));
      /*$D514*/ branchTarget = true; block_id = tmp5_U8 ? 698 : 699;
      break;
    case 699:  // $D516
      /*$D516*/ CYCLES(0xd516, 38);
      /*$D517*/ s_a = (uint8_t)(s_y + 0x01);
      /*$D518*/ block_id = s_status_d ? 701 : 700;
      break;
    case 700:  // $D518
      /*$D518*/ tmp6_U16 = (s_a + ram_peek(0x005e)) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 702;
      break;
    case 701:  // $D518
      /*$D518*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x005e), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 702;
      break;
    case 702:  // $D51A
      /*$D51A*/ tmp5_U8 = s_a;
                s_x = tmp5_U8;
      /*$D51B*/ s_y = 0x00;
      /*$D51D*/ poke(ram_peek16al(0x005e), tmp5_U8);
      /*$D51F*/ s_a = ram_peek(0x005f);
      /*$D521*/ block_id = s_status_d ? 704 : 703;
      break;
    case 703:  // $D521
      /*$D521*/ tmp6_U16 = s_a;
                tmp4_U16 = tmp6_U16 + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 705;
      break;
    case 704:  // $D521
      /*$D521*/ tmp4_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 705;
      break;
    case 705:  // $D523
      /*$D523*/ tmp5_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp5_U8;
      /*$D524*/ tmp1_U8 = s_a;
                poke((ram_peek16al(0x005e) + tmp5_U8), tmp1_U8);
      /*$D526*/ ram_poke(0x005e, s_x);
      /*$D528*/ ram_poke(0x005f, tmp1_U8);
      /*$D52A*/ branchTarget = true; block_id = !s_status_c ? 695 : 706;
      break;
    case 706:  // $D52C
      /*$D52C*/ CYCLES(0xd52c, 4);
                s_x = 0x80;
                block_id = 707;
      break;
    case 707:  // $D52E
      /*$D52E*/ CYCLES(0xd52e, 9);
                ram_poke(0x0033, s_x);
      /*$D530*/ branchTarget = true; push16(0xd532); block_id = 1917;
      break;
    case 708:  // $D533
      /*$D533*/ CYCLES(0xd533, 7);
                tmp1_U8 = s_x >= 0xef;
                s_status_c = tmp1_U8;
      /*$D535*/ branchTarget = true; block_id = !tmp1_U8 ? 710 : 709;
      break;
    case 709:  // $D537
      /*$D537*/ CYCLES(0xd537, 4);
                s_x = 0xef;
                block_id = 710;
      break;
    case 710:  // $D539
      /*$D539*/ CYCLES(0xd539, 14);
      /*$D53B*/ tmp1_U8 = s_x;
                ram_poke((0x0200 + tmp1_U8), 0x00);
      /*$D53F*/ branchTarget = true; block_id = !tmp1_U8 ? 712 : 711;
      break;
    case 711:  // $D541
      /*$D541*/ CYCLES(0xd541, 19);
                tmp1_U8 = s_x;
      /*$D546*/ ram_poke((0x01ff + tmp1_U8), (ram_peek((0x01ff + tmp1_U8)) & 0x7f));
      /*$D549*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_x = tmp1_U8;
      /*$D54A*/ branchTarget = true; block_id = tmp1_U8 ? 711 : 712;
      break;
    case 712:  // $D54C
      /*$D54C*/ CYCLES(0xd54c, 12);
                s_a = 0x00;
      /*$D54E*/ s_x = 0xff;
      /*$D550*/ s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_y = 0x01;
      /*$D552*/ branchTarget = true; block_id = find_block_id_func_t001(0xd552, pop16() + 1);;
      break;
    case 713:  // $D553
      /*$D553*/ CYCLES(0xd553, 6);
                branchTarget = true; push16(0xd555); block_id = 1899;
      break;
    case 714:  // $D556
      /*$D556*/ CYCLES(0xd556, 6);
                tmp1_U8 = s_a & 0x7f;
                s_status_not_z = tmp1_U8;
                s_status_n = 0x00;
                s_a = tmp1_U8;
      /*$D558*/ branchTarget = true; block_id = find_block_id_func_t001(0xd558, pop16() + 1);;
      break;
    case 715:  // $D559
      /*$D559*/ CYCLES(0xd559, 19);
      /*$D55B*/ s_x = (uint8_t)(ram_peek(0x00b8) - 0x01);
      /*$D55C*/ s_y = 0x04;
      /*$D55E*/ ram_poke(0x0013, 0x04);
      /*$D560*/ tmp5_U8 = ram_peek(0x00d6);
                s_status_v = ((tmp5_U8 >> 0x06) & 0x01);
      /*$D562*/ branchTarget = true; block_id = !(tmp5_U8 & 0x80) ? 718 : 716;
      break;
    case 716:  // $D564
      /*$D564*/ CYCLES(0xd564, 9);
                tmp1_U8 = pop8();
      /*$D565*/ tmp1_U8 = pop8();
      /*$D566*/ branchTarget = true; push16(0xd568); block_id = 781;
      break;
    case 717:  // $D569
      /*$D569*/ CYCLES(0xd569, 6);
                branchTarget = true; block_id = 791;
      break;
    case 718:  // $D56C
      /*$D56C*/ CYCLES(0xd56c, 2);
                s_x = (uint8_t)(s_x + 0x01);
                block_id = 719;
      break;
    case 719:  // $D56D
      /*$D56D*/ CYCLES(0xd56d, 12);
                s_a = ram_peek((0x0200 + s_x));
      /*$D570*/ tmp1_U8 = (ram_peek(0x0013) >> 0x06) & 0x01;
                s_status_v = tmp1_U8;
      /*$D572*/ branchTarget = true; block_id = tmp1_U8 ? 721 : 720;
      break;
    case 720:  // $D574
      /*$D574*/ CYCLES(0xd574, 7);
      /*$D576*/ branchTarget = true; block_id = !(s_a != 0x20) ? 718 : 721;
      break;
    case 721:  // $D578
      /*$D578*/ CYCLES(0xd578, 11);
                tmp1_U8 = s_a;
                ram_poke(0x000e, tmp1_U8);
      /*$D57A*/ s_status_c = (tmp1_U8 >= 0x22);
      /*$D57C*/ branchTarget = true; block_id = !(tmp1_U8 != 0x22) ? 756 : 722;
      break;
    case 722:  // $D57E
      /*$D57E*/ CYCLES(0xd57e, 4);
                branchTarget = true; block_id = s_status_v ? 742 : 723;
      break;
    case 723:  // $D580
      /*$D580*/ CYCLES(0xd580, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x3f);
      /*$D582*/ branchTarget = true; block_id = (tmp1_U8 != 0x3f) ? 725 : 724;
      break;
    case 724:  // $D584
      /*$D584*/ CYCLES(0xd584, 7);
                s_a = 0xba;
      /*$D586*/ branchTarget = true; block_id = 742;
      break;
    case 725:  // $D588
      /*$D588*/ CYCLES(0xd588, 7);
      /*$D58A*/ branchTarget = true; block_id = !(s_a >= 0x30) ? 727 : 726;
      break;
    case 726:  // $D58C
      /*$D58C*/ CYCLES(0xd58c, 7);
                tmp1_U8 = s_a >= 0x3c;
                s_status_c = tmp1_U8;
      /*$D58E*/ branchTarget = true; block_id = !tmp1_U8 ? 742 : 727;
      break;
    case 727:  // $D590
      /*$D590*/ CYCLES(0xd590, 31);
                ram_poke(0x00ad, s_y);
      /*$D594*/ ram_poke(0x009d, 0xd0);
      /*$D598*/ ram_poke(0x009e, 0xcf);
      /*$D59C*/ ram_poke(0x000f, 0x00);
      /*$D59E*/ s_y = 0xff;
      /*$D59F*/ tmp1_U8 = s_x;
                ram_poke(0x00b8, tmp1_U8);
      /*$D5A1*/ s_x = (uint8_t)(tmp1_U8 - 0x01);
                block_id = 728;
      break;
    case 728:  // $D5A2
      /*$D5A2*/ CYCLES(0xd5a2, 6);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$D5A3*/ branchTarget = true; block_id = tmp1_U8 ? 730 : 729;
      break;
    case 729:  // $D5A5
      /*$D5A5*/ CYCLES(0xd5a5, 4);
                ram_poke(0x009e, (uint8_t)(ram_peek(0x009e) + 0x01));
                block_id = 730;
      break;
    case 730:  // $D5A7
      /*$D5A7*/ CYCLES(0xd5a7, 2);
                s_x = (uint8_t)(s_x + 0x01);
                block_id = 731;
      break;
    case 731:  // $D5A8
      /*$D5A8*/ CYCLES(0xd5a8, 12);
                tmp1_U8 = ram_peek((0x0200 + s_x));
                s_a = tmp1_U8;
      /*$D5AD*/ branchTarget = true; block_id = !(tmp1_U8 != 0x20) ? 730 : 732;
      break;
    case 732:  // $D5AF
      /*$D5AF*/ CYCLES(0xd5af, 9);
                s_status_c = 0x01;
      /*$D5B0*/ block_id = s_status_d ? 734 : 733;
      break;
    case 733:  // $D5B0
      /*$D5B0*/ tmp4_U16 = s_a;
                tmp1_U8 = peek((ram_peek16(0x009d) + s_y));
                tmp6_U16 = tmp1_U8;
                tmp3_U16 = (tmp4_U16 - tmp6_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)(~tmp6_U16));
                tmp1_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
                block_id = 735;
      break;
    case 734:  // $D5B0
      /*$D5B0*/ tmp1_U8 = peek((ram_peek16(0x009d) + s_y));
                tmp3_U16 = sbc_dec16(s_a, tmp1_U8, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 735;
      break;
    case 735:  // $D5B2
      /*$D5B2*/ branchTarget = true; block_id = !s_status_not_z ? 728 : 736;
      break;
    case 736:  // $D5B4
      /*$D5B4*/ CYCLES(0xd5b4, 7);
      /*$D5B6*/ branchTarget = true; block_id = (s_a != 0x80) ? 757 : 737;
      break;
    case 737:  // $D5B8
      /*$D5B8*/ CYCLES(0xd5b8, 11);
                tmp1_U8 = s_a | ram_peek(0x000f);
                s_a = tmp1_U8;
      /*$D5BA*/ s_status_c = (tmp1_U8 >= 0xc5);
      /*$D5BC*/ branchTarget = true; block_id = (tmp1_U8 != 0xc5) ? 741 : 738;
      break;
    case 738:  // $D5BE
      /*$D5BE*/ CYCLES(0xd5be, 12);
                tmp1_U8 = ram_peek((0x0201 + s_x));
                s_a = tmp1_U8;
      /*$D5C3*/ branchTarget = true; block_id = !(tmp1_U8 != 0x4e) ? 757 : 739;
      break;
    case 739:  // $D5C5
      /*$D5C5*/ CYCLES(0xd5c5, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x4f);
      /*$D5C7*/ branchTarget = true; block_id = !(tmp1_U8 != 0x4f) ? 757 : 740;
      break;
    case 740:  // $D5C9
      /*$D5C9*/ CYCLES(0xd5c9, 4);
                s_a = 0xc5;
                block_id = 741;
      break;
    case 741:  // $D5CB
      /*$D5CB*/ CYCLES(0xd5cb, 4);
                s_y = ram_peek(0x00ad);
                block_id = 742;
      break;
    case 742:  // $D5CD
      /*$D5CD*/ CYCLES(0xd5cd, 18);
                s_x = (uint8_t)(s_x + 0x01);
      /*$D5CE*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$D5CF*/ ram_poke((0x01fb + tmp1_U8), s_a);
      /*$D5D2*/ tmp1_U8 = ram_peek((0x01fb + tmp1_U8));
                s_a = tmp1_U8;
      /*$D5D5*/ branchTarget = true; block_id = !tmp1_U8 ? 763 : 743;
      break;
    case 743:  // $D5D7
      /*$D5D7*/ CYCLES(0xd5d7, 9);
                s_status_c = 0x01;
      /*$D5D8*/ block_id = s_status_d ? 745 : 744;
      break;
    case 744:  // $D5D8
      /*$D5D8*/ tmp1_U8 = (uint8_t)((s_a - 0x003a) - (uint8_t)(0x01 - s_status_c));
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
                block_id = 746;
      break;
    case 745:  // $D5D8
      /*$D5D8*/ tmp3_U16 = sbc_dec16(s_a, 0x3a, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_not_z = (~(uint8_t)(tmp3_U16 >> 8) & 2);
                block_id = 746;
      break;
    case 746:  // $D5DA
      /*$D5DA*/ branchTarget = true; block_id = !s_status_not_z ? 748 : 747;
      break;
    case 747:  // $D5DC
      /*$D5DC*/ CYCLES(0xd5dc, 7);
      /*$D5DE*/ branchTarget = true; block_id = (s_a != 0x49) ? 749 : 748;
      break;
    case 748:  // $D5E0
      /*$D5E0*/ CYCLES(0xd5e0, 4);
                ram_poke(0x0013, s_a);
                block_id = 749;
      break;
    case 749:  // $D5E2
      /*$D5E2*/ CYCLES(0xd5e2, 9);
                s_status_c = 0x01;
      /*$D5E3*/ block_id = s_status_d ? 751 : 750;
      break;
    case 750:  // $D5E3
      /*$D5E3*/ tmp3_U16 = s_a;
                tmp6_U16 = (tmp3_U16 - 0x0078) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)0xff87);
                tmp1_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
                block_id = 752;
      break;
    case 751:  // $D5E3
      /*$D5E3*/ tmp6_U16 = sbc_dec16(s_a, 0x78, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 752;
      break;
    case 752:  // $D5E5
      /*$D5E5*/ branchTarget = true; block_id = s_status_not_z ? 719 : 753;
      break;
    case 753:  // $D5E7
      /*$D5E7*/ CYCLES(0xd5e7, 4);
                ram_poke(0x000e, s_a);
                block_id = 754;
      break;
    case 754:  // $D5E9
      /*$D5E9*/ CYCLES(0xd5e9, 9);
                tmp1_U8 = ram_peek((0x0200 + s_x));
                s_a = tmp1_U8;
      /*$D5EC*/ branchTarget = true; block_id = !tmp1_U8 ? 742 : 755;
      break;
    case 755:  // $D5EE
      /*$D5EE*/ CYCLES(0xd5ee, 7);
                tmp1_U8 = s_a;
                tmp5_U8 = ram_peek(0x000e);
                s_status_c = (tmp1_U8 >= tmp5_U8);
      /*$D5F0*/ branchTarget = true; block_id = !(tmp1_U8 != tmp5_U8) ? 742 : 756;
      break;
    case 756:  // $D5F2
      /*$D5F2*/ CYCLES(0xd5f2, 12);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$D5F3*/ ram_poke((0x01fb + tmp1_U8), s_a);
      /*$D5F6*/ tmp1_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp1_U8;
      /*$D5F7*/ branchTarget = true; block_id = tmp1_U8 ? 754 : 757;
      break;
    case 757:  // $D5F9
      /*$D5F9*/ CYCLES(0xd5f9, 7);
                s_x = ram_peek(0x00b8);
      /*$D5FB*/ ram_poke(0x000f, (uint8_t)(ram_peek(0x000f) + 0x01));
                block_id = 758;
      break;
    case 758:  // $D5FD
      /*$D5FD*/ CYCLES(0xd5fd, 9);
                tmp5_U8 = s_y;
                tmp1_U8 = peek((ram_peek16(0x009d) + tmp5_U8));
                s_a = tmp1_U8;
      /*$D5FF*/ tmp5_U8 = (uint8_t)(tmp5_U8 + 0x01);
                s_y = tmp5_U8;
      /*$D600*/ branchTarget = true; block_id = tmp5_U8 ? 760 : 759;
      break;
    case 759:  // $D602
      /*$D602*/ CYCLES(0xd602, 4);
                ram_poke(0x009e, (uint8_t)(ram_peek(0x009e) + 0x01));
                block_id = 760;
      break;
    case 760:  // $D604
      /*$D604*/ CYCLES(0xd604, 6);
                tmp5_U8 = (uint8_t)((s_a << 0x01) >> 8);
                s_status_c = tmp5_U8;
      /*$D605*/ branchTarget = true; block_id = !tmp5_U8 ? 758 : 761;
      break;
    case 761:  // $D607
      /*$D607*/ CYCLES(0xd607, 7);
                tmp5_U8 = peek((ram_peek16(0x009d) + s_y));
      /*$D609*/ branchTarget = true; block_id = tmp5_U8 ? 731 : 762;
      break;
    case 762:  // $D60B
      /*$D60B*/ CYCLES(0xd60b, 9);
                tmp5_U8 = ram_peek((0x0200 + s_x));
                s_a = tmp5_U8;
      /*$D60E*/ branchTarget = true; block_id = !(tmp5_U8 & 0x80) ? 741 : 763;
      break;
    case 763:  // $D610
      /*$D610*/ CYCLES(0xd610, 18);
                ram_poke((0x01fd + s_y), s_a);
      /*$D613*/ ram_poke(0x00b9, (uint8_t)(ram_peek(0x00b9) - 0x01));
      /*$D615*/ s_status_not_z = 0xff;
                s_status_n = 0x80;
                s_a = 0xff;
      /*$D617*/ ram_poke(0x00b8, 0xff);
      /*$D619*/ branchTarget = true; block_id = find_block_id_func_t001(0xd619, pop16() + 1);;
      break;
    case 764:  // $D61A
      /*$D61A*/ CYCLES(0xd61a, 7);
                s_a = ram_peek(0x0067);
      /*$D61C*/ s_x = ram_peek(0x0068);
                FUNC_FNDLIN2(0x0000);
                block_id = find_block_id_func_t001(0xd61c, pop16() + 1);;
      break;
    case 765:  // $D61E
      /*$D61E*/ CYCLES(0xd61e, 18);
                s_y = 0x01;
      /*$D620*/ ram_poke(0x009b, s_a);
      /*$D622*/ ram_poke(0x009c, s_x);
      /*$D624*/ tmp5_U8 = peek((ram_peek16(0x009b) + 0x0001));
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$D626*/ branchTarget = true; block_id = !tmp5_U8 ? 772 : 766;
      break;
    case 766:  // $D628
      /*$D628*/ CYCLES(0xd628, 14);
      /*$D629*/ tmp5_U8 = (uint8_t)(s_y + 0x02);
                s_y = tmp5_U8;
      /*$D62A*/ tmp1_U8 = ram_peek(0x0051);
                s_a = tmp1_U8;
      /*$D62C*/ tmp5_U8 = peek((ram_peek16(0x009b) + tmp5_U8));
                s_status_not_z = (tmp1_U8 != tmp5_U8);
                tmp2_U8 = tmp1_U8 >= tmp5_U8;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - tmp5_U8) & 0x80);
      /*$D62E*/ branchTarget = true; block_id = !tmp2_U8 ? 773 : 767;
      break;
    case 767:  // $D630
      /*$D630*/ CYCLES(0xd630, 4);
                branchTarget = true; block_id = !s_status_not_z ? 769 : 768;
      break;
    case 768:  // $D632
      /*$D632*/ CYCLES(0xd632, 6);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp1_U8;
      /*$D633*/ branchTarget = true; block_id = tmp1_U8 ? 771 : 769;
      break;
    case 769:  // $D635
      /*$D635*/ CYCLES(0xd635, 12);
                tmp2_U8 = ram_peek(0x0050);
                s_a = tmp2_U8;
      /*$D637*/ tmp5_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp5_U8;
      /*$D638*/ tmp5_U8 = peek((ram_peek16(0x009b) + tmp5_U8));
                s_status_not_z = (tmp2_U8 != tmp5_U8);
                tmp1_U8 = tmp2_U8 >= tmp5_U8;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp5_U8) & 0x80);
      /*$D63A*/ branchTarget = true; block_id = !tmp1_U8 ? 773 : 770;
      break;
    case 770:  // $D63C
      /*$D63C*/ CYCLES(0xd63c, 4);
                branchTarget = true; block_id = !s_status_not_z ? 773 : 771;
      break;
    case 771:  // $D63E
      /*$D63E*/ CYCLES(0xd63e, 16);
                tmp1_U8 = s_y;
      /*$D63F*/ tmp5_U8 = peek((ram_peek16(0x009b) + (uint8_t)(tmp1_U8 - 0x01)));
      /*$D641*/ s_x = tmp5_U8;
      /*$D642*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x02);
                s_y = tmp1_U8;
      /*$D643*/ tmp1_U8 = peek((ram_peek16(0x009b) + tmp1_U8));
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$D645*/ branchTarget = true; block_id = s_status_c ? 765 : 772;
      break;
    case 772:  // $D647
      /*$D647*/ CYCLES(0xd647, 2);
                s_status_c = 0x00;
                block_id = 773;
      break;
    case 773:  // $D648
      /*$D648*/ CYCLES(0xd648, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd648, pop16() + 1);;
      break;
    case 774:  // $D64B
      /*$D64B*/ CYCLES(0xd64b, 45);
      /*$D64D*/ ram_poke(0x00d6, 0x00);
      /*$D650*/ poke(ram_peek16(0x0067), 0x00);
      /*$D653*/ poke((ram_peek16(0x0067) + 0x0001), 0x00);
      /*$D655*/ s_a = ram_peek(0x0067);
      /*$D657*/ block_id = s_status_d ? 776 : 775;
      break;
    case 775:  // $D657
      /*$D657*/ tmp6_U16 = (s_a + 0x0002) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 777;
      break;
    case 776:  // $D657
      /*$D657*/ tmp6_U16 = adc_dec16(s_a, 0x02, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 777;
      break;
    case 777:  // $D659
      /*$D659*/ tmp1_U8 = s_a;
                ram_poke(0x0069, tmp1_U8);
      /*$D65B*/ ram_poke(0x00af, tmp1_U8);
      /*$D65D*/ s_a = ram_peek(0x0068);
      /*$D65F*/ block_id = s_status_d ? 779 : 778;
      break;
    case 778:  // $D65F
      /*$D65F*/ s_a = (uint8_t)(s_a + s_status_c);
                block_id = 780;
      break;
    case 779:  // $D65F
      /*$D65F*/ s_a = ((uint8_t)adc_dec16(s_a, 0x00, s_status_c));
                block_id = 780;
      break;
    case 780:  // $D661
      /*$D661*/ tmp1_U8 = s_a;
                ram_poke(0x006a, tmp1_U8);
      /*$D663*/ ram_poke(0x00b0, tmp1_U8);
                block_id = 781;
      break;
    case 781:  // $D665
      /*$D665*/ CYCLES(0xd665, 6);
                FUNC_CLRTXTPTR(0xfffe);
                branchTarget = true; block_id = 782;
      break;
    case 782:  // $D668
      /*$D668*/ CYCLES(0xd668, 7);
      /*$D66C*/ CYCLES(0xd66c, 40);
      /*$D670*/ ram_poke(0x006f, ram_peek(0x0073));
      /*$D672*/ ram_poke(0x0070, ram_peek(0x0074));
      /*$D674*/ tmp1_U8 = ram_peek(0x0069);
      /*$D676*/ tmp5_U8 = ram_peek(0x006a);
      /*$D678*/ ram_poke(0x006b, tmp1_U8);
      /*$D67A*/ ram_poke(0x006c, tmp5_U8);
      /*$D67C*/ ram_poke(0x006d, tmp1_U8);
      /*$D67E*/ ram_poke(0x006e, tmp5_U8);
      /*$D680*/ FUNC_RESTORE(0xfffe);
                branchTarget = true; block_id = 783;
      break;
    case 783:  // $D683
      /*$D683*/ CYCLES(0xd683, 33);
      /*$D685*/ ram_poke(0x0052, 0x55);
      /*$D687*/ tmp5_U8 = pop8();
      /*$D688*/ s_y = tmp5_U8;
      /*$D689*/ tmp1_U8 = pop8();
      /*$D68A*/ s_x = 0xf8;
      /*$D68C*/ s_sp = 0xf8;
      /*$D68D*/ push8(tmp1_U8);
      /*$D68F*/ push8(tmp5_U8);
      /*$D690*/ s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$D692*/ ram_poke(0x007a, 0x00);
      /*$D694*/ ram_poke(0x0014, 0x00);
      /*$D696*/ CYCLES(0xd696, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd696, pop16() + 1);;
      break;
    case 784:  // $D697
      /*$D697*/ CYCLES(0xd697, 24);
                s_status_c = 0x00;
      /*$D698*/ s_a = ram_peek(0x0067);
      /*$D69A*/ block_id = s_status_d ? 786 : 785;
      break;
    case 785:  // $D69A
      /*$D69A*/ tmp6_U16 = (s_a + 0x00ff) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 787;
      break;
    case 786:  // $D69A
      /*$D69A*/ tmp6_U16 = adc_dec16(s_a, 0xff, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 787;
      break;
    case 787:  // $D69C
      /*$D69C*/ ram_poke(0x00b8, s_a);
      /*$D69E*/ s_a = ram_peek(0x0068);
      /*$D6A0*/ block_id = s_status_d ? 789 : 788;
      break;
    case 788:  // $D6A0
      /*$D6A0*/ tmp6_U16 = s_a;
                tmp3_U16 = (tmp6_U16 + 0x00ff) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)0x00ff);
                tmp5_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
                block_id = 790;
      break;
    case 789:  // $D6A0
      /*$D6A0*/ tmp3_U16 = adc_dec16(s_a, 0xff, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_not_z = (~tmp5_U8 & 2);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                s_status_n = (tmp5_U8 & 0x80);
                block_id = 790;
      break;
    case 790:  // $D6A2
      /*$D6A2*/ ram_poke(0x00b9, s_a);
      /*$D6A4*/ branchTarget = true; block_id = find_block_id_func_t001(0xd6a4, pop16() + 1);;
      break;
    case 791:  // $D7D2
      /*$D7D2*/ CYCLES(0xd7d2, 11);
                tmp1_U8 = s_sp;
                s_x = tmp1_U8;
      /*$D7D3*/ ram_poke(0x00f8, tmp1_U8);
      /*$D7D5*/ branchTarget = true; push16(0xd7d7); block_id = 826;
      break;
    case 792:  // $D7D8
      /*$D7D8*/ CYCLES(0xd7d8, 16);
                s_a = ram_peek(0x00b8);
      /*$D7DA*/ s_y = ram_peek(0x00b9);
      /*$D7DE*/ tmp5_U8 = (uint8_t)(ram_peek(0x0076) + 0x01);
                s_x = tmp5_U8;
      /*$D7DF*/ branchTarget = true; block_id = !tmp5_U8 ? 794 : 793;
      break;
    case 793:  // $D7E1
      /*$D7E1*/ CYCLES(0xd7e1, 7);
                ram_poke(0x0079, s_a);
      /*$D7E3*/ ram_poke(0x007a, s_y);
                block_id = 794;
      break;
    case 794:  // $D7E5
      /*$D7E5*/ CYCLES(0xd7e5, 11);
                s_y = 0x00;
      /*$D7E7*/ tmp1_U8 = peek(ram_peek16al(0x00b8));
                s_a = tmp1_U8;
      /*$D7E9*/ branchTarget = true; block_id = tmp1_U8 ? 817 : 795;
      break;
    case 795:  // $D7EB
      /*$D7EB*/ CYCLES(0xd7eb, 12);
                s_y = 0x02;
      /*$D7ED*/ tmp1_U8 = peek((ram_peek16al(0x00b8) + 0x0002));
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$D7EF*/ s_status_c = 0x00;
      /*$D7F0*/ branchTarget = true; block_id = !tmp1_U8 ? 808 : 796;
      break;
    case 796:  // $D7F2
      /*$D7F2*/ CYCLES(0xd7f2, 29);
                tmp1_U8 = s_y;
      /*$D7F3*/ tmp5_U8 = peek((ram_peek16al(0x00b8) + (uint8_t)(tmp1_U8 + 0x01)));
      /*$D7F5*/ ram_poke(0x0075, tmp5_U8);
      /*$D7F7*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x02);
                s_y = tmp1_U8;
      /*$D7F8*/ tmp5_U8 = peek((ram_peek16al(0x00b8) + tmp1_U8));
      /*$D7FA*/ ram_poke(0x0076, tmp5_U8);
      /*$D7FC*/ s_a = tmp1_U8;
      /*$D7FD*/ block_id = s_status_d ? 798 : 797;
      break;
    case 797:  // $D7FD
      /*$D7FD*/ tmp3_U16 = (s_a + ram_peek(0x00b8)) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 799;
      break;
    case 798:  // $D7FD
      /*$D7FD*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x00b8), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 799;
      break;
    case 799:  // $D7FF
      /*$D7FF*/ ram_poke(0x00b8, s_a);
      /*$D801*/ branchTarget = true; block_id = !s_status_c ? 801 : 800;
      break;
    case 800:  // $D803
      /*$D803*/ CYCLES(0xd803, 4);
                ram_poke(0x00b9, (uint8_t)(ram_peek(0x00b9) + 0x01));
                block_id = 801;
      break;
    case 801:  // $D805
      /*$D805*/ CYCLES(0xd805, 7);
                tmp5_U8 = ram_peek(0x00f2);
                tmp1_U8 = tmp5_U8 & 0x80;
                s_status_n = tmp1_U8;
                s_status_v = ((tmp5_U8 >> 0x06) & 0x01);
                s_status_not_z = (s_a & tmp5_U8);
      /*$D807*/ branchTarget = true; block_id = !tmp1_U8 ? 805 : 802;
      break;
    case 802:  // $D809
      /*$D809*/ CYCLES(0xd809, 9);
      /*$D80B*/ tmp1_U8 = (uint8_t)(ram_peek(0x0076) + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$D80C*/ branchTarget = true; block_id = !tmp1_U8 ? 805 : 803;
      break;
    case 803:  // $D80E
      /*$D80E*/ CYCLES(0xd80e, 9);
                s_a = 0x23;
      /*$D810*/ FUNC_OUTDO(0xfffe);
      /*$D813*/ CYCLES(0xd813, 12);
                s_x = ram_peek(0x0075);
      /*$D815*/ s_a = ram_peek(0x0076);
      /*$D817*/ branchTarget = true; push16(0xd819); block_id = 1691;
      break;
    case 804:  // $D81A
      /*$D81A*/ CYCLES(0xd81a, 6);
                FUNC_OUTSP(0xfffe);
                branchTarget = true; block_id = 805;
      break;
    case 805:  // $D81D
      /*$D81D*/ CYCLES(0xd81d, 6);
                branchTarget = true; push16(0xd81f); block_id = 2;
      break;
    case 806:  // $D820
      /*$D820*/ CYCLES(0xd820, 6);
                branchTarget = true; push16(0xd822); block_id = 809;
      break;
    case 807:  // $D823
      /*$D823*/ CYCLES(0xd823, 6);
                branchTarget = true; block_id = 791;
      break;
    case 808:  // $D826
      /*$D826*/ CYCLES(0xd826, 4);
                branchTarget = true; block_id = !s_status_not_z ? 837 : 809;
      break;
    case 809:  // $D828
      /*$D828*/ CYCLES(0xd828, 4);
                branchTarget = true; block_id = !s_status_not_z ? 825 : 810;
      break;
    case 810:  // $D82A
      /*$D82A*/ CYCLES(0xd82a, 7);
                block_id = s_status_d ? 812 : 811;
      break;
    case 811:  // $D82A
      /*$D82A*/ tmp3_U16 = s_a;
                tmp6_U16 = (tmp3_U16 - 0x0080) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)0xff7f);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 813;
      break;
    case 812:  // $D82A
      /*$D82A*/ tmp6_U16 = sbc_dec16(s_a, 0x80, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 813;
      break;
    case 813:  // $D82C
      /*$D82C*/ branchTarget = true; block_id = !s_status_c ? 816 : 814;
      break;
    case 814:  // $D82E
      /*$D82E*/ CYCLES(0xd82e, 7);
                tmp1_U8 = s_a >= 0x40;
                s_status_c = tmp1_U8;
      /*$D830*/ branchTarget = true; block_id = tmp1_U8 ? 818 : 815;
      break;
    case 815:  // $D832
      /*$D832*/ CYCLES(0xd832, 23);
                tmp6_U16 = s_a << 0x01;
      /*$D833*/ s_y = ((uint8_t)tmp6_U16);
      /*$D837*/ push8(ram_peek((0xd001 + (tmp6_U16 & 0x00ff))));
      /*$D83B*/ push8(ram_peek((0xd000 + (tmp6_U16 & 0x00ff))));
      /*$D83C*/ branchTarget = true; block_id = 2;
      break;
    case 816:  // $D83F
      /*$D83F*/ CYCLES(0xd83f, 6);
      /*$DA46*/ CYCLES(0xda46, 6);
                branchTarget = true; push16(0xda48); block_id = 1047;
      break;
    case 817:  // $D842
      /*$D842*/ CYCLES(0xd842, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x3a);
      /*$D844*/ branchTarget = true; block_id = !(tmp1_U8 != 0x3a) ? 801 : 818;
      break;
    case 818:  // $D846
      /*$D846*/ CYCLES(0xd846, 6);
                branchTarget = true; block_id = 1023;
      break;
    case 819:  // $D849
      /*$D849*/ CYCLES(0xd849, 16);
                s_status_c = 0x01;
      /*$D84A*/ s_a = ram_peek(0x0067);
      /*$D84C*/ block_id = s_status_d ? 821 : 820;
      break;
    case 820:  // $D84C
      /*$D84C*/ tmp6_U16 = s_a;
                tmp3_U16 = (tmp6_U16 - 0x0001) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)0xfffe);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 822;
      break;
    case 821:  // $D84C
      /*$D84C*/ tmp3_U16 = sbc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 822;
      break;
    case 822:  // $D84E
      /*$D84E*/ tmp1_U8 = ram_peek(0x0068);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
      /*$D850*/ branchTarget = true; block_id = s_status_c ? 824 : 823;
      break;
    case 823:  // $D852
      /*$D852*/ CYCLES(0xd852, 2);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
                block_id = 824;
      break;
    case 824:  // $D853
      /*$D853*/ CYCLES(0xd853, 7);
                ram_poke(0x007d, s_a);
      /*$D855*/ ram_poke(0x007e, s_y);
                block_id = 825;
      break;
    case 825:  // $D857
      /*$D857*/ CYCLES(0xd857, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd857, pop16() + 1);;
      break;
    case 826:  // $D858
      /*$D858*/ CYCLES(0xd858, 12);
                tmp5_U8 = io_peek(0xc000);
                s_a = tmp5_U8;
      /*$D85B*/ tmp1_U8 = tmp5_U8 != 0x83;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp5_U8 >= 0x83);
                s_status_n = ((uint8_t)(tmp5_U8 - 0x83) & 0x80);
      /*$D85D*/ branchTarget = true; block_id = !tmp1_U8 ? 828 : 827;
      break;
    case 827:  // $D85F
      /*$D85F*/ CYCLES(0xd85f, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd85f, pop16() + 1);;
      break;
    case 828:  // $D860
      /*$D860*/ CYCLES(0xd860, 6);
                branchTarget = true; push16(0xd862); block_id = 713;
      break;
    case 829:  // $D863
      /*$D863*/ CYCLES(0xd863, 11);
                s_x = 0xff;
      /*$D865*/ tmp1_U8 = ram_peek(0x00d8);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
      /*$D867*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 831 : 830;
      break;
    case 830:  // $D869
      /*$D869*/ CYCLES(0xd869, 6);
                branchTarget = true; block_id = 1803;
      break;
    case 831:  // $D86C
      /*$D86C*/ CYCLES(0xd86c, 7);
                tmp1_U8 = s_a;
                s_status_not_z = (tmp1_U8 != 0x03);
                tmp5_U8 = tmp1_U8 >= 0x03;
                s_status_c = tmp5_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - 0x03) & 0x80);
      /*$D86E*/ branchTarget = true; block_id = tmp5_U8 ? 833 : 832;
      break;
    case 832:  // $D870
      /*$D870*/ CYCLES(0xd870, 2);
                s_status_c = 0x00;
                block_id = 833;
      break;
    case 833:  // $D871
      /*$D871*/ CYCLES(0xd871, 4);
                branchTarget = true; block_id = s_status_not_z ? 840 : 834;
      break;
    case 834:  // $D873
      /*$D873*/ CYCLES(0xd873, 16);
                s_a = ram_peek(0x00b8);
      /*$D875*/ s_y = ram_peek(0x00b9);
      /*$D879*/ tmp5_U8 = (uint8_t)(ram_peek(0x0076) + 0x01);
                s_x = tmp5_U8;
      /*$D87A*/ branchTarget = true; block_id = !tmp5_U8 ? 836 : 835;
      break;
    case 835:  // $D87C
      /*$D87C*/ CYCLES(0xd87c, 21);
                ram_poke(0x0079, s_a);
      /*$D87E*/ ram_poke(0x007a, s_y);
      /*$D884*/ ram_poke(0x0077, ram_peek(0x0075));
      /*$D886*/ ram_poke(0x0078, ram_peek(0x0076));
                block_id = 836;
      break;
    case 836:  // $D888
      /*$D888*/ CYCLES(0xd888, 4);
                tmp5_U8 = pop8();
      /*$D889*/ tmp5_U8 = pop8();
                block_id = 837;
      break;
    case 837:  // $D88A
      /*$D88A*/ CYCLES(0xd88a, 11);
                s_a = 0x5d;
      /*$D88C*/ s_y = 0xd3;
      /*$D88E*/ branchTarget = true; block_id = !s_status_c ? 839 : 838;
      break;
    case 838:  // $D890
      /*$D890*/ CYCLES(0xd890, 6);
                branchTarget = true; block_id = 647;
      break;
    case 839:  // $D893
      /*$D893*/ CYCLES(0xd893, 6);
                branchTarget = true; block_id = 650;
      break;
    case 840:  // $D8AF
      /*$D8AF*/ CYCLES(0xd8af, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd8af, pop16() + 1);;
      break;
    case 841:  // $D93E
      /*$D93E*/ CYCLES(0xd93e, 6);
                branchTarget = true; push16(0xd940); block_id = 876;
      break;
    case 842:  // $D941
      /*$D941*/ CYCLES(0xd941, 6);
                branchTarget = true; push16(0xd943); block_id = 861;
      break;
    case 843:  // $D944
      /*$D944*/ CYCLES(0xd944, 11);
      /*$D948*/ branchTarget = true; block_id = (ram_peek(0x0076) >= ram_peek(0x0051)) ? 849 : 844;
      break;
    case 844:  // $D94A
      /*$D94A*/ CYCLES(0xd94a, 14);
                s_a = s_y;
      /*$D94B*/ s_status_c = 0x01;
      /*$D94C*/ block_id = s_status_d ? 846 : 845;
      break;
    case 845:  // $D94C
      /*$D94C*/ tmp3_U16 = (s_a + ram_peek(0x00b8)) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 847;
      break;
    case 846:  // $D94C
      /*$D94C*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x00b8), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 847;
      break;
    case 847:  // $D94E
      /*$D94E*/ s_x = ram_peek(0x00b9);
      /*$D950*/ branchTarget = true; block_id = !s_status_c ? 850 : 848;
      break;
    case 848:  // $D952
      /*$D952*/ CYCLES(0xd952, 6);
                s_x = (uint8_t)(s_x + 0x01);
      /*$D953*/ branchTarget = true; block_id = s_status_c ? 850 : 849;
      break;
    case 849:  // $D955
      /*$D955*/ CYCLES(0xd955, 7);
                s_a = ram_peek(0x0067);
      /*$D957*/ s_x = ram_peek(0x0068);
                block_id = 850;
      break;
    case 850:  // $D959
      /*$D959*/ CYCLES(0xd959, 6);
                FUNC_FNDLIN2(0xfffe);
      /*$D95C*/ CYCLES(0xd95c, 4);
                branchTarget = true; block_id = !s_status_c ? 858 : 851;
      break;
    case 851:  // $D95E
      /*$D95E*/ CYCLES(0xd95e, 23);
                s_a = ram_peek(0x009b);
      /*$D960*/ block_id = s_status_d ? 853 : 852;
      break;
    case 852:  // $D960
      /*$D960*/ tmp3_U16 = (s_a - 0x0001) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp3_U16);
                block_id = 854;
      break;
    case 853:  // $D960
      /*$D960*/ tmp3_U16 = sbc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 854;
      break;
    case 854:  // $D962
      /*$D962*/ ram_poke(0x00b8, s_a);
      /*$D964*/ s_a = ram_peek(0x009c);
      /*$D966*/ block_id = s_status_d ? 856 : 855;
      break;
    case 855:  // $D966
      /*$D966*/ tmp3_U16 = s_a;
                tmp6_U16 = tmp3_U16 - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)0xffff);
                tmp5_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
                block_id = 857;
      break;
    case 856:  // $D966
      /*$D966*/ tmp6_U16 = sbc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_not_z = (~tmp5_U8 & 2);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                s_status_n = (tmp5_U8 & 0x80);
                block_id = 857;
      break;
    case 857:  // $D968
      /*$D968*/ ram_poke(0x00b9, s_a);
      /*$D96A*/ branchTarget = true; block_id = find_block_id_func_t001(0xd96a, pop16() + 1);;
      break;
    case 858:  // $D97C
      /*$D97C*/ CYCLES(0xd97c, 9);
                s_x = 0x5a;
      /*$D97E*/ branchTarget = true; block_id = 641;
      break;
    case 859:  // $D981
      /*$D981*/ CYCLES(0xd981, 6);
                branchTarget = true; block_id = 1023;
      break;
    case 860:  // $D9A2
      /*$D9A2*/ CYCLES(0xd9a2, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd9a2, pop16() + 1);;
      break;
    case 861:  // $D9A6
      /*$D9A6*/ CYCLES(0xd9a6, 14);
      /*$D9A8*/ ram_poke(0x000d, 0x00);
      /*$D9AA*/ s_y = 0x00;
      /*$D9AC*/ ram_poke(0x000e, 0x00);
                block_id = 862;
      break;
    case 862:  // $D9AE
      /*$D9AE*/ CYCLES(0xd9ae, 14);
      /*$D9B0*/ tmp5_U8 = ram_peek(0x000d);
                s_x = tmp5_U8;
      /*$D9B2*/ ram_poke(0x000d, ram_peek(0x000e));
      /*$D9B4*/ ram_poke(0x000e, tmp5_U8);
                block_id = 863;
      break;
    case 863:  // $D9B6
      /*$D9B6*/ CYCLES(0xd9b6, 7);
                tmp5_U8 = peek((ram_peek16al(0x00b8) + s_y));
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$D9B8*/ branchTarget = true; block_id = !tmp5_U8 ? 860 : 864;
      break;
    case 864:  // $D9BA
      /*$D9BA*/ CYCLES(0xd9ba, 7);
                tmp5_U8 = s_a;
                tmp1_U8 = ram_peek(0x000e);
                tmp2_U8 = tmp5_U8 != tmp1_U8;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp5_U8 >= tmp1_U8);
                s_status_n = ((uint8_t)(tmp5_U8 - tmp1_U8) & 0x80);
      /*$D9BC*/ branchTarget = true; block_id = !tmp2_U8 ? 860 : 865;
      break;
    case 865:  // $D9BE
      /*$D9BE*/ CYCLES(0xd9be, 9);
                s_y = (uint8_t)(s_y + 0x01);
      /*$D9BF*/ tmp2_U8 = s_a;
                tmp1_U8 = tmp2_U8 != 0x22;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp2_U8 >= 0x22);
      /*$D9C1*/ branchTarget = true; block_id = tmp1_U8 ? 863 : 866;
      break;
    case 866:  // $D9C3
      /*$D9C3*/ CYCLES(0xd9c3, 4);
                branchTarget = true; block_id = !s_status_not_z ? 862 : 867;
      break;
    case 867:  // $D9C5
      /*$D9C5*/ CYCLES(0xd9c5, 7);
                tmp1_U8 = pop8();
      /*$D9C6*/ tmp1_U8 = pop8();
      /*$D9C7*/ tmp1_U8 = pop8();
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$D9C8*/ branchTarget = true; block_id = find_block_id_func_t001(0xd9c8, pop16() + 1);;
      break;
    case 868:  // $D9F4
      /*$D9F4*/ CYCLES(0xd9f4, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0xab);
      /*$D9F6*/ branchTarget = true; block_id = (tmp1_U8 != 0xab) ? 859 : 869;
      break;
    case 869:  // $D9F8
      /*$D9F8*/ CYCLES(0xd9f8, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x00a1) - 0x01);
                ram_poke(0x00a1, tmp1_U8);
      /*$D9FA*/ branchTarget = true; block_id = tmp1_U8 ? 871 : 870;
      break;
    case 870:  // $D9FC
      /*$D9FC*/ CYCLES(0xd9fc, 7);
                tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$D9FD*/ branchTarget = true; block_id = 810;
      break;
    case 871:  // $DA00
      /*$DA00*/ CYCLES(0xda00, 6);
                branchTarget = true; push16(0xda02); block_id = 2;
      break;
    case 872:  // $DA03
      /*$DA03*/ CYCLES(0xda03, 6);
                branchTarget = true; push16(0xda05); block_id = 876;
      break;
    case 873:  // $DA06
      /*$DA06*/ CYCLES(0xda06, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x2c);
      /*$DA08*/ branchTarget = true; block_id = !(tmp1_U8 != 0x2c) ? 869 : 874;
      break;
    case 874:  // $DA0A
      /*$DA0A*/ CYCLES(0xda0a, 2);
                tmp1_U8 = pop8();
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 875;
      break;
    case 875:  // $DA0B
      /*$DA0B*/ CYCLES(0xda0b, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xda0b, pop16() + 1);;
      break;
    case 876:  // $DA0C
      /*$DA0C*/ CYCLES(0xda0c, 11);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_x = 0x00;
      /*$DA0E*/ ram_poke(0x0050, 0x00);
      /*$DA10*/ ram_poke(0x0051, 0x00);
                block_id = 877;
      break;
    case 877:  // $DA12
      /*$DA12*/ CYCLES(0xda12, 4);
                branchTarget = true; block_id = s_status_c ? 875 : 878;
      break;
    case 878:  // $DA14
      /*$DA14*/ CYCLES(0xda14, 21);
                block_id = s_status_d ? 880 : 879;
      break;
    case 879:  // $DA14
      /*$DA14*/ tmp6_U16 = s_a;
                tmp3_U16 = (tmp6_U16 - 0x002f) - (uint8_t)(0x01 - s_status_c);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)0xffd0);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 881;
      break;
    case 880:  // $DA14
      /*$DA14*/ tmp3_U16 = sbc_dec16(s_a, 0x2f, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
                block_id = 881;
      break;
    case 881:  // $DA16
      /*$DA16*/ ram_poke(0x000d, s_a);
      /*$DA18*/ tmp1_U8 = ram_peek(0x0051);
                s_a = tmp1_U8;
      /*$DA1A*/ ram_poke(0x005e, tmp1_U8);
      /*$DA1E*/ branchTarget = true; block_id = (tmp1_U8 >= 0x19) ? 868 : 882;
      break;
    case 882:  // $DA20
      /*$DA20*/ CYCLES(0xda20, 52);
      /*$DA22*/ tmp3_U16 = ram_peek(0x0050);
      /*$DA23*/ ram_poke(0x005e, (uint8_t)((ram_peek(0x005e) << 0x01) | (uint8_t)((tmp3_U16 << 0x01) >> 8)));
      /*$DA25*/ tmp3_U16 = tmp3_U16 << 0x02;
                s_a = ((uint8_t)tmp3_U16);
      /*$DA26*/ tmp3_U16 = (ram_peek(0x005e) << 0x01) | (uint8_t)((tmp3_U16 & 0x01ff) >> 8);
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                ram_poke(0x005e, ((uint8_t)tmp3_U16));
      /*$DA28*/ block_id = s_status_d ? 884 : 883;
      break;
    case 883:  // $DA28
      /*$DA28*/ tmp3_U16 = (s_a + ram_peek(0x0050)) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 885;
      break;
    case 884:  // $DA28
      /*$DA28*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x0050), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 885;
      break;
    case 885:  // $DA2A
      /*$DA2A*/ ram_poke(0x0050, s_a);
      /*$DA2C*/ s_a = ram_peek(0x005e);
      /*$DA2E*/ block_id = s_status_d ? 887 : 886;
      break;
    case 886:  // $DA2E
      /*$DA2E*/ s_a = (uint8_t)((s_a + ram_peek(0x0051)) + s_status_c);
                block_id = 888;
      break;
    case 887:  // $DA2E
      /*$DA2E*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x0051), s_status_c));
                block_id = 888;
      break;
    case 888:  // $DA30
      /*$DA30*/ ram_poke(0x0051, s_a);
      /*$DA32*/ tmp3_U16 = ram_peek(0x0050) << 0x01;
                ram_poke(0x0050, ((uint8_t)tmp3_U16));
      /*$DA34*/ tmp3_U16 = (ram_peek(0x0051) << 0x01) | (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                ram_poke(0x0051, ((uint8_t)tmp3_U16));
      /*$DA36*/ s_a = ram_peek(0x0050);
      /*$DA38*/ block_id = s_status_d ? 890 : 889;
      break;
    case 889:  // $DA38
      /*$DA38*/ tmp3_U16 = s_a;
                tmp6_U16 = ram_peek(0x000d);
                tmp4_U16 = (tmp3_U16 + tmp6_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 891;
      break;
    case 890:  // $DA38
      /*$DA38*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x000d), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
                block_id = 891;
      break;
    case 891:  // $DA3A
      /*$DA3A*/ ram_poke(0x0050, s_a);
      /*$DA3C*/ branchTarget = true; block_id = !s_status_c ? 893 : 892;
      break;
    case 892:  // $DA3E
      /*$DA3E*/ CYCLES(0xda3e, 4);
                tmp1_U8 = (uint8_t)(ram_peek(0x0051) + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x0051, tmp1_U8);
                block_id = 893;
      break;
    case 893:  // $DA40
      /*$DA40*/ CYCLES(0xda40, 6);
                branchTarget = true; push16(0xda42); block_id = 2;
      break;
    case 894:  // $DA43
      /*$DA43*/ CYCLES(0xda43, 6);
                branchTarget = true; block_id = 877;
      break;
    case 895:  // $DA49
      /*$DA49*/ CYCLES(0xda49, 16);
                ram_poke(0x0085, s_a);
      /*$DA4B*/ ram_poke(0x0086, s_y);
      /*$DA4D*/ s_a = 0xd0;
      /*$DA4F*/ branchTarget = true; push16(0xda51); block_id = 1021;
      break;
    case 896:  // $DA52
      /*$DA52*/ CYCLES(0xda52, 16);
      /*$DA54*/ push8(ram_peek(0x0012));
      /*$DA57*/ push8(ram_peek(0x0011));
      /*$DA58*/ branchTarget = true; push16(0xda5a); block_id = 938;
      break;
    case 897:  // $DA5B
      /*$DA5B*/ CYCLES(0xda5b, 9);
                tmp2_U8 = pop8();
      /*$DA5C*/ tmp4_U16 = (tmp2_U8 << 0x01) | s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                tmp2_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp2_U8;
                s_a = tmp2_U8;
      /*$DA5D*/ branchTarget = true; push16(0xda5f); block_id = 933;
      break;
    case 898:  // $DA60
      /*$DA60*/ CYCLES(0xda60, 4);
                branchTarget = true; block_id = s_status_not_z ? 904 : 899;
      break;
    case 899:  // $DA62
      /*$DA62*/ CYCLES(0xda62, 6);
                tmp2_U8 = pop8();
      /*$DA63*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 903 : 900;
      break;
    case 900:  // $DA65
      /*$DA65*/ CYCLES(0xda65, 6);
                branchTarget = true; push16(0xda67); block_id = 1588;
      break;
    case 901:  // $DA68
      /*$DA68*/ CYCLES(0xda68, 6);
                branchTarget = true; push16(0xda6a); block_id = 1121;
      break;
    case 902:  // $DA6B
      /*$DA6B*/ CYCLES(0xda6b, 21);
      /*$DA6F*/ poke(ram_peek16(0x0085), ram_peek(0x00a0));
      /*$DA71*/ s_y = 0x01;
      /*$DA72*/ tmp2_U8 = ram_peek(0x00a1);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$DA74*/ poke((ram_peek16(0x0085) + 0x0001), tmp2_U8);
      /*$DA76*/ branchTarget = true; block_id = find_block_id_func_t001(0xda76, pop16() + 1);;
      break;
    case 903:  // $DA77
      /*$DA77*/ CYCLES(0xda77, 6);
      /*$EB27*/ CYCLES(0xeb27, 7);
                s_x = ram_peek(0x0085);
      /*$EB29*/ s_y = ram_peek(0x0086);
                block_id = 1579;
      break;
    case 904:  // $DA7A
      /*$DA7A*/ CYCLES(0xda7a, 16);
                tmp1_U8 = pop8();
      /*$DA7B*/ s_y = 0x02;
      /*$DA7D*/ tmp1_U8 = peek((ram_peek16al(0x00a0) + 0x0002));
      /*$DA7F*/ tmp2_U8 = ram_peek(0x0070);
                s_status_not_z = (tmp1_U8 != tmp2_U8);
      /*$DA81*/ branchTarget = true; block_id = !(tmp1_U8 >= tmp2_U8) ? 910 : 905;
      break;
    case 905:  // $DA83
      /*$DA83*/ CYCLES(0xda83, 4);
                branchTarget = true; block_id = s_status_not_z ? 907 : 906;
      break;
    case 906:  // $DA85
      /*$DA85*/ CYCLES(0xda85, 12);
      /*$DA86*/ tmp1_U8 = peek((ram_peek16al(0x00a0) + (uint8_t)(s_y - 0x01)));
      /*$DA8A*/ branchTarget = true; block_id = !(tmp1_U8 >= ram_peek(0x006f)) ? 910 : 907;
      break;
    case 907:  // $DA8C
      /*$DA8C*/ CYCLES(0xda8c, 11);
                tmp2_U8 = ram_peek(0x00a1);
      /*$DA8E*/ tmp1_U8 = ram_peek(0x006a);
                s_status_not_z = (tmp2_U8 != tmp1_U8);
                tmp1_U8 = tmp2_U8 >= tmp1_U8;
                s_status_c = tmp1_U8;
      /*$DA90*/ branchTarget = true; block_id = !tmp1_U8 ? 910 : 908;
      break;
    case 908:  // $DA92
      /*$DA92*/ CYCLES(0xda92, 4);
                branchTarget = true; block_id = s_status_not_z ? 911 : 909;
      break;
    case 909:  // $DA94
      /*$DA94*/ CYCLES(0xda94, 11);
      /*$DA96*/ tmp1_U8 = ram_peek(0x00a0) >= ram_peek(0x0069);
                s_status_c = tmp1_U8;
      /*$DA98*/ branchTarget = true; block_id = tmp1_U8 ? 911 : 910;
      break;
    case 910:  // $DA9A
      /*$DA9A*/ CYCLES(0xda9a, 12);
                s_a = ram_peek(0x00a0);
      /*$DA9C*/ s_y = ram_peek(0x00a1);
      /*$DA9E*/ branchTarget = true; block_id = 913;
      break;
    case 911:  // $DAA1
      /*$DAA1*/ CYCLES(0xdaa1, 12);
      /*$DAA3*/ tmp1_U8 = peek(ram_peek16al(0x00a0));
                s_a = tmp1_U8;
      /*$DAA5*/ branchTarget = true; push16(0xdaa7); block_id = 1250;
      break;
    case 912:  // $DAA8
      /*$DAA8*/ CYCLES(0xdaa8, 19);
      /*$DAAC*/ ram_poke(0x00ab, ram_peek(0x008c));
      /*$DAAE*/ ram_poke(0x00ac, ram_peek(0x008d));
      /*$DAB0*/ FUNC_MOVINS(0xfffe);
      /*$DAB3*/ CYCLES(0xdab3, 7);
                s_a = 0x9d;
      /*$DAB5*/ s_y = 0x00;
                block_id = 913;
      break;
    case 913:  // $DAB7
      /*$DAB7*/ CYCLES(0xdab7, 12);
                ram_poke(0x008c, s_a);
      /*$DAB9*/ ram_poke(0x008d, s_y);
      /*$DABB*/ FUNC_FRETMS(0xfffe);
      /*$DABE*/ CYCLES(0xdabe, 29);
      /*$DAC0*/ tmp1_U8 = peek(ram_peek16al(0x008c));
      /*$DAC2*/ poke(ram_peek16(0x0085), tmp1_U8);
      /*$DAC5*/ tmp1_U8 = peek((ram_peek16al(0x008c) + 0x0001));
      /*$DAC7*/ poke((ram_peek16(0x0085) + 0x0001), tmp1_U8);
      /*$DAC9*/ s_y = 0x02;
      /*$DACA*/ tmp1_U8 = peek((ram_peek16al(0x008c) + 0x0002));
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$DACC*/ poke((ram_peek16(0x0085) + 0x0002), tmp1_U8);
      /*$DACE*/ branchTarget = true; block_id = find_block_id_func_t001(0xdace, pop16() + 1);;
      break;
    case 914:  // $DAFB
      /*$DAFB*/ CYCLES(0xdafb, 9);
                s_a = 0x0d;
      /*$DAFD*/ FUNC_OUTDO(0xfffe);
                branchTarget = true; block_id = 916;
      break;
    case 915:  // $DB00
      /*$DB00*/ CYCLES(0xdb00, 4);
                tmp5_U8 = s_a ^ 0xff;
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
                block_id = 917;
      break;
    case 916:  // $DB00
      /*$DAFD*/ FUNC_NEGATE(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xdafd, pop16() + 1);;
      break;
    case 917:  // $DB02
      /*$DB02*/ CYCLES(0xdb02, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xdb02, pop16() + 1);;
      break;
    case 918:  // $DB3A
      /*$DB3A*/ CYCLES(0xdb3a, 6);
                branchTarget = true; push16(0xdb3c); block_id = 1252;
      break;
    case 919:  // $DB3D
      /*$DB3D*/ CYCLES(0xdb3d, 6);
                FUNC_FREFAC(0xfffe);
      /*$DB40*/ CYCLES(0xdb40, 7);
      /*$DB41*/ s_y = 0x00;
      /*$DB43*/ s_x = (uint8_t)(s_a + 0x01);
                block_id = 920;
      break;
    case 920:  // $DB44
      /*$DB44*/ CYCLES(0xdb44, 6);
                tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$DB45*/ branchTarget = true; block_id = !tmp1_U8 ? 917 : 921;
      break;
    case 921:  // $DB47
      /*$DB47*/ CYCLES(0xdb47, 9);
                tmp1_U8 = s_y;
                tmp5_U8 = peek((ram_peek16al(0x005e) + tmp1_U8));
                s_a = tmp5_U8;
      /*$DB49*/ FUNC_OUTDO(0xfffe);
      /*$DB4C*/ CYCLES(0xdb4c, 9);
                s_y = (uint8_t)(tmp1_U8 + 0x01);
      /*$DB4D*/ tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x0d);
      /*$DB4F*/ branchTarget = true; block_id = (tmp1_U8 != 0x0d) ? 920 : 922;
      break;
    case 922:  // $DB51
      /*$DB51*/ CYCLES(0xdb51, 6);
                FUNC_NEGATE(0xfffe);
      /*$DB54*/ CYCLES(0xdb54, 6);
                branchTarget = true; block_id = 920;
      break;
    case 923:  // $DB57
      /*$DB57*/ CYCLES(0xdb57, 9);
                s_a = 0x20;
      /*$DB59*/ s_status_v = ((ram_peek(0x3fa9) >> 0x06) & 0x01);
                FUNC_OUTDO(0x0000);
                block_id = find_block_id_func_t001(0xdb59, pop16() + 1);;
      break;
    case 924:  // $DB5A
      /*$DB5A*/ CYCLES(0xdb5a, 4);
                s_a = 0x3f;
                FUNC_OUTDO(0x0000);
                block_id = find_block_id_func_t001(0xdb5a, pop16() + 1);;
      break;
    case 925:  // $DB5C
      /*$DB5C*/ CYCLES(0xdb5c, 11);
                tmp1_U8 = s_a | 0x80;
                s_a = tmp1_U8;
      /*$DB5E*/ s_status_not_z = (tmp1_U8 != 0xa0);
                tmp5_U8 = tmp1_U8 >= 0xa0;
                s_status_c = tmp5_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - 0xa0) & 0x80);
      /*$DB60*/ branchTarget = true; block_id = !tmp5_U8 ? 927 : 926;
      break;
    case 926:  // $DB62
      /*$DB62*/ CYCLES(0xdb62, 4);
                tmp1_U8 = s_a | ram_peek(0x00f3);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 927;
      break;
    case 927:  // $DB64
      /*$DB64*/ CYCLES(0xdb64, 6);
                rom_cout(0xfffe);
                branchTarget = true; block_id = 928;
      break;
    case 928:  // $DB67
      /*$DB67*/ CYCLES(0xdb67, 14);
      /*$DB69*/ push8((s_a & 0x7f));
      /*$DB6A*/ s_a = ram_peek(0x00f1);
      /*$DB6C*/ FUNC_MON_WAIT(0xfffe);
                branchTarget = true; block_id = 929;
      break;
    case 929:  // $DB6F
      /*$DB6F*/ CYCLES(0xdb6f, 4);
                tmp1_U8 = pop8();
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$DB70*/ branchTarget = true; block_id = find_block_id_func_t001(0xdb70, pop16() + 1);;
      break;
    case 930:  // $DD67
      /*$DD67*/ CYCLES(0xdd67, 6);
                branchTarget = true; push16(0xdd69); block_id = 938;
      break;
    case 931:  // $DD6A
      /*$DD6A*/ CYCLES(0xdd6a, 6);
                s_status_c = 0x00;
      /*$DD6B*/ block_id = 933;
      break;
    case 932:  // $DD6C
      /*$DD6C*/ CYCLES(0xdd6c, 2);
                s_status_c = 0x01;
                block_id = 933;
      break;
    case 933:  // $DD6D
      /*$DD6D*/ CYCLES(0xdd6d, 7);
                tmp2_U8 = ram_peek(0x0011);
                tmp1_U8 = tmp2_U8 & 0x80;
                s_status_n = tmp1_U8;
                s_status_v = ((tmp2_U8 >> 0x06) & 0x01);
                s_status_not_z = (s_a & tmp2_U8);
      /*$DD6F*/ branchTarget = true; block_id = tmp1_U8 ? 936 : 934;
      break;
    case 934:  // $DD71
      /*$DD71*/ CYCLES(0xdd71, 4);
                branchTarget = true; block_id = s_status_c ? 937 : 935;
      break;
    case 935:  // $DD73
      /*$DD73*/ CYCLES(0xdd73, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xdd73, pop16() + 1);;
      break;
    case 936:  // $DD74
      /*$DD74*/ CYCLES(0xdd74, 4);
                branchTarget = true; block_id = s_status_c ? 935 : 937;
      break;
    case 937:  // $DD76
      /*$DD76*/ CYCLES(0xdd76, 9);
                s_x = 0xa3;
      /*$DD78*/ branchTarget = true; block_id = 641;
      break;
    case 938:  // $DD7B
      /*$DD7B*/ CYCLES(0xdd7b, 7);
      /*$DD7D*/ branchTarget = true; block_id = ram_peek(0x00b8) ? 940 : 939;
      break;
    case 939:  // $DD7F
      /*$DD7F*/ CYCLES(0xdd7f, 4);
                ram_poke(0x00b9, (uint8_t)(ram_peek(0x00b9) - 0x01));
                block_id = 940;
      break;
    case 940:  // $DD81
      /*$DD81*/ CYCLES(0xdd81, 23);
                ram_poke(0x00b8, (uint8_t)(ram_peek(0x00b8) - 0x01));
      /*$DD83*/ s_x = 0x00;
      /*$DD85*/ s_status_v = ((ram_peek(0x0048) >> 0x06) & 0x01);
      /*$DD88*/ push8(0x00);
      /*$DD89*/ s_a = 0x01;
      /*$DD8B*/ branchTarget = true; push16(0xdd8d); block_id = 623;
      break;
    case 941:  // $DD8E
      /*$DD8E*/ CYCLES(0xdd8e, 6);
                branchTarget = true; push16(0xdd90); block_id = 994;
      break;
    case 942:  // $DD91
      /*$DD91*/ CYCLES(0xdd91, 7);
                s_a = 0x00;
      /*$DD93*/ ram_poke(0x0089, 0x00);
                block_id = 943;
      break;
    case 943:  // $DD95
      /*$DD95*/ CYCLES(0xdd95, 6);
                branchTarget = true; push16(0xdd97); block_id = 4;
      break;
    case 944:  // $DD98
      /*$DD98*/ CYCLES(0xdd98, 9);
                s_status_c = 0x01;
      /*$DD99*/ block_id = s_status_d ? 946 : 945;
      break;
    case 945:  // $DD99
      /*$DD99*/ tmp4_U16 = s_a;
                tmp6_U16 = (tmp4_U16 - 0x00cf) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0xff30);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 947;
      break;
    case 946:  // $DD99
      /*$DD99*/ tmp6_U16 = sbc_dec16(s_a, 0xcf, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 947;
      break;
    case 947:  // $DD9B
      /*$DD9B*/ branchTarget = true; block_id = !s_status_c ? 952 : 948;
      break;
    case 948:  // $DD9D
      /*$DD9D*/ CYCLES(0xdd9d, 7);
                tmp1_U8 = s_a >= 0x03;
                s_status_c = tmp1_U8;
      /*$DD9F*/ branchTarget = true; block_id = tmp1_U8 ? 952 : 949;
      break;
    case 949:  // $DDA1
      /*$DDA1*/ CYCLES(0xdda1, 19);
                tmp1_U8 = s_a;
      /*$DDA6*/ tmp1_U8 = ((uint8_t)((tmp1_U8 << 0x01) | (tmp1_U8 >= 0x01)) ^ 0x01) ^ ram_peek(0x0089);
                s_a = tmp1_U8;
      /*$DDA8*/ tmp1_U8 = tmp1_U8 >= ram_peek(0x0089);
                s_status_c = tmp1_U8;
      /*$DDAA*/ branchTarget = true; block_id = !tmp1_U8 ? 984 : 950;
      break;
    case 950:  // $DDAC
      /*$DDAC*/ CYCLES(0xddac, 9);
                ram_poke(0x0089, s_a);
      /*$DDAE*/ branchTarget = true; push16(0xddb0); block_id = 2;
      break;
    case 951:  // $DDB1
      /*$DDB1*/ CYCLES(0xddb1, 6);
                branchTarget = true; block_id = 944;
      break;
    case 952:  // $DDB4
      /*$DDB4*/ CYCLES(0xddb4, 7);
                tmp1_U8 = ram_peek(0x0089);
                s_x = tmp1_U8;
      /*$DDB6*/ branchTarget = true; block_id = tmp1_U8 ? 977 : 953;
      break;
    case 953:  // $DDB8
      /*$DDB8*/ CYCLES(0xddb8, 4);
                branchTarget = true; block_id = s_status_c ? 987 : 954;
      break;
    case 954:  // $DDBA
      /*$DDBA*/ CYCLES(0xddba, 7);
                block_id = s_status_d ? 956 : 955;
      break;
    case 955:  // $DDBA
      /*$DDBA*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 + 0x0007) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x0007);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 957;
      break;
    case 956:  // $DDBA
      /*$DDBA*/ tmp4_U16 = adc_dec16(s_a, 0x07, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 957;
      break;
    case 957:  // $DDBC
      /*$DDBC*/ branchTarget = true; block_id = !s_status_c ? 987 : 958;
      break;
    case 958:  // $DDBE
      /*$DDBE*/ CYCLES(0xddbe, 7);
                block_id = s_status_d ? 960 : 959;
      break;
    case 959:  // $DDBE
      /*$DDBE*/ tmp4_U16 = s_a;
                tmp6_U16 = ram_peek(0x0011);
                tmp3_U16 = (tmp4_U16 + tmp6_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
                block_id = 961;
      break;
    case 960:  // $DDBE
      /*$DDBE*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x0011), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 961;
      break;
    case 961:  // $DDC0
      /*$DDC0*/ branchTarget = true; block_id = s_status_not_z ? 963 : 962;
      break;
    case 962:  // $DDC2
      /*$DDC2*/ CYCLES(0xddc2, 6);
      /*$E597*/ CYCLES(0xe597, 16);
      /*$E599*/ push8(ram_peek(0x00a1));
      /*$E59A*/ tmp1_U8 = ram_peek(0x00a0);
                s_a = tmp1_U8;
      /*$E59C*/ push8(tmp1_U8);
      /*$E59D*/ branchTarget = true; push16(0xe59f); block_id = 994;
      break;
    case 963:  // $DDC5
      /*$DDC5*/ CYCLES(0xddc5, 14);
                block_id = s_status_d ? 965 : 964;
      break;
    case 964:  // $DDC5
      /*$DDC5*/ s_a = (uint8_t)((s_a + 0x00ff) + s_status_c);
                block_id = 966;
      break;
    case 965:  // $DDC5
      /*$DDC5*/ s_a = ((uint8_t)adc_dec16(s_a, 0xff, s_status_c));
                block_id = 966;
      break;
    case 966:  // $DDC7
      /*$DDC7*/ tmp1_U8 = s_a;
                ram_poke(0x005e, tmp1_U8);
      /*$DDC9*/ tmp3_U16 = tmp1_U8 << 0x01;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
      /*$DDCA*/ block_id = s_status_d ? 968 : 967;
      break;
    case 967:  // $DDCA
      /*$DDCA*/ tmp3_U16 = s_a;
                tmp6_U16 = ram_peek(0x005e);
                tmp4_U16 = (tmp3_U16 + tmp6_U16) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)tmp6_U16);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 969;
      break;
    case 968:  // $DDCA
      /*$DDCA*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x005e), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 969;
      break;
    case 969:  // $DDCC
      /*$DDCC*/ s_y = s_a;
                block_id = 970;
      break;
    case 970:  // $DDCD
      /*$DDCD*/ CYCLES(0xddcd, 11);
                tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$DDCE*/ tmp1_U8 = tmp1_U8 >= ram_peek((0xd0b2 + s_y));
                s_status_c = tmp1_U8;
      /*$DDD1*/ branchTarget = true; block_id = tmp1_U8 ? 989 : 971;
      break;
    case 971:  // $DDD3
      /*$DDD3*/ CYCLES(0xddd3, 6);
                branchTarget = true; push16(0xddd5); block_id = 931;
      break;
    case 972:  // $DDD6
      /*$DDD6*/ CYCLES(0xddd6, 2);
                push8(s_a);
                block_id = 973;
      break;
    case 973:  // $DDD7
      /*$DDD7*/ CYCLES(0xddd7, 6);
                branchTarget = true; push16(0xddd9); block_id = 982;
      break;
    case 974:  // $DDDA
      /*$DDDA*/ CYCLES(0xddda, 9);
                tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$DDDB*/ tmp1_U8 = ram_peek(0x0087);
                s_y = tmp1_U8;
      /*$DDDD*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 980 : 975;
      break;
    case 975:  // $DDDF
      /*$DDDF*/ CYCLES(0xdddf, 6);
                tmp1_U8 = s_a;
                s_status_not_z = tmp1_U8;
                s_x = tmp1_U8;
      /*$DDE0*/ branchTarget = true; block_id = !tmp1_U8 ? 988 : 976;
      break;
    case 976:  // $DDE2
      /*$DDE2*/ CYCLES(0xdde2, 4);
                branchTarget = true; block_id = s_status_not_z ? 992 : 977;
      break;
    case 977:  // $DDE4
      /*$DDE4*/ CYCLES(0xdde4, 14);
                tmp1_U8 = ram_peek(0x0011);
                ram_poke(0x0011, (tmp1_U8 >> 0x01));
      /*$DDE7*/ s_a = (uint8_t)((s_x << 0x01) | (tmp1_U8 & 0x01));
      /*$DDE8*/ tmp1_U8 = ram_peek(0x00b8);
                s_x = tmp1_U8;
      /*$DDEA*/ branchTarget = true; block_id = tmp1_U8 ? 979 : 978;
      break;
    case 978:  // $DDEC
      /*$DDEC*/ CYCLES(0xddec, 4);
                ram_poke(0x00b9, (uint8_t)(ram_peek(0x00b9) - 0x01));
                block_id = 979;
      break;
    case 979:  // $DDEE
      /*$DDEE*/ CYCLES(0xddee, 14);
                ram_poke(0x00b8, (uint8_t)(ram_peek(0x00b8) - 0x01));
      /*$DDF0*/ s_y = 0x1b;
      /*$DDF2*/ ram_poke(0x0089, s_a);
      /*$DDF4*/ branchTarget = true; block_id = 970;
      break;
    case 980:  // $DDF6
      /*$DDF6*/ CYCLES(0xddf6, 9);
                tmp1_U8 = s_a >= ram_peek((0xd0b2 + s_y));
                s_status_c = tmp1_U8;
      /*$DDF9*/ branchTarget = true; block_id = tmp1_U8 ? 992 : 981;
      break;
    case 981:  // $DDFB
      /*$DDFB*/ CYCLES(0xddfb, 4);
                branchTarget = true; block_id = !s_status_c ? 972 : 982;
      break;
    case 982:  // $DDFD
      /*$DDFD*/ CYCLES(0xddfd, 19);
                tmp1_U8 = s_y;
      /*$DE00*/ push8(ram_peek((0xd0b4 + tmp1_U8)));
      /*$DE04*/ push8(ram_peek((0xd0b3 + tmp1_U8)));
      /*$DE05*/ branchTarget = true; push16(0xde07); block_id = 985;
      break;
    case 983:  // $DE08
      /*$DE08*/ CYCLES(0xde08, 9);
      /*$DD86*/ CYCLES(0xdd86, 14);
                push8(ram_peek(0x0089));
      /*$DD88*/ push8(s_x);
      /*$DD89*/ s_a = 0x01;
      /*$DD8B*/ branchTarget = true; push16(0xdd8d); block_id = 623;
      break;
    case 984:  // $DE0D
      /*$DE0D*/ CYCLES(0xde0d, 6);
                branchTarget = true; block_id = 1023;
      break;
    case 985:  // $DE10
      /*$DE10*/ CYCLES(0xde10, 33);
                tmp1_U8 = ram_peek(0x00a2);
      /*$DE12*/ s_x = ram_peek((0xd0b2 + s_y));
      /*$DE15*/ s_y = tmp1_U8;
      /*$DE16*/ tmp5_U8 = pop8();
      /*$DE17*/ ram_poke(0x005e, tmp5_U8);
      /*$DE19*/ ram_poke(0x005e, (uint8_t)(ram_peek(0x005e) + 0x01));
      /*$DE1B*/ tmp5_U8 = pop8();
      /*$DE1C*/ ram_poke(0x005f, tmp5_U8);
      /*$DE1F*/ push8(tmp1_U8);
      /*$DE20*/ branchTarget = true; push16(0xde22); block_id = 1588;
      break;
    case 986:  // $DE23
      /*$DE23*/ CYCLES(0xde23, 31);
      /*$DE25*/ push8(ram_peek(0x00a1));
      /*$DE28*/ push8(ram_peek(0x00a0));
      /*$DE2B*/ push8(ram_peek(0x009f));
      /*$DE2E*/ push8(ram_peek(0x009e));
      /*$DE2F*/ tmp1_U8 = ram_peek(0x009d);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$DE31*/ push8(tmp1_U8);
      /*$DE32*/ branchTarget = true; block_id = find_block_id_func_t001(0xde32, ram_peek16al(0x005e));
      break;
    case 987:  // $DE35
      /*$DE35*/ CYCLES(0xde35, 6);
                s_y = 0xff;
      /*$DE37*/ tmp1_U8 = pop8();
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
                block_id = 988;
      break;
    case 988:  // $DE38
      /*$DE38*/ CYCLES(0xde38, 4);
                branchTarget = true; block_id = !s_status_not_z ? 993 : 989;
      break;
    case 989:  // $DE3A
      /*$DE3A*/ CYCLES(0xde3a, 7);
      /*$DE3C*/ branchTarget = true; block_id = !(s_a != 0x64) ? 991 : 990;
      break;
    case 990:  // $DE3E
      /*$DE3E*/ CYCLES(0xde3e, 6);
                branchTarget = true; push16(0xde40); block_id = 931;
      break;
    case 991:  // $DE41
      /*$DE41*/ CYCLES(0xde41, 4);
                ram_poke(0x0087, s_y);
                block_id = 992;
      break;
    case 992:  // $DE43
      /*$DE43*/ CYCLES(0xde43, 45);
                tmp1_U8 = pop8();
      /*$DE44*/ s_status_c = (tmp1_U8 & 0x01);
      /*$DE45*/ ram_poke(0x0016, (tmp1_U8 >> 0x01));
      /*$DE47*/ tmp1_U8 = pop8();
      /*$DE48*/ ram_poke(0x00a5, tmp1_U8);
      /*$DE4A*/ tmp1_U8 = pop8();
      /*$DE4B*/ ram_poke(0x00a6, tmp1_U8);
      /*$DE4D*/ tmp1_U8 = pop8();
      /*$DE4E*/ ram_poke(0x00a7, tmp1_U8);
      /*$DE50*/ tmp1_U8 = pop8();
      /*$DE51*/ ram_poke(0x00a8, tmp1_U8);
      /*$DE53*/ tmp1_U8 = pop8();
      /*$DE54*/ ram_poke(0x00a9, tmp1_U8);
      /*$DE56*/ tmp1_U8 = pop8();
      /*$DE57*/ ram_poke(0x00aa, tmp1_U8);
      /*$DE5B*/ ram_poke(0x00ab, (tmp1_U8 ^ ram_peek(0x00a2)));
                block_id = 993;
      break;
    case 993:  // $DE5D
      /*$DE5D*/ CYCLES(0xde5d, 6);
                tmp1_U8 = ram_peek(0x009d);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$DE5F*/ branchTarget = true; block_id = find_block_id_func_t001(0xde5f, pop16() + 1);;
      break;
    case 994:  // $DE60
      /*$DE60*/ CYCLES(0xde60, 7);
                s_a = 0x00;
      /*$DE62*/ ram_poke(0x0011, 0x00);
                block_id = 995;
      break;
    case 995:  // $DE64
      /*$DE64*/ CYCLES(0xde64, 6);
                branchTarget = true; push16(0xde66); block_id = 2;
      break;
    case 996:  // $DE67
      /*$DE67*/ CYCLES(0xde67, 4);
                branchTarget = true; block_id = s_status_c ? 998 : 997;
      break;
    case 997:  // $DE69
      /*$DE69*/ CYCLES(0xde69, 6);
      /*$EC4A*/ CYCLES(0xec4a, 7);
                s_y = 0x00;
      /*$EC4C*/ s_x = 0x0a;
                block_id = 1625;
      break;
    case 998:  // $DE6C
      /*$DE6C*/ CYCLES(0xde6c, 6);
                FUNC_ISLETC(0xfffe);
      /*$DE6F*/ CYCLES(0xde6f, 4);
                branchTarget = true; block_id = s_status_c ? 1026 : 999;
      break;
    case 999:  // $DE71
      /*$DE71*/ CYCLES(0xde71, 7);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0x2e);
      /*$DE73*/ branchTarget = true; block_id = !(tmp5_U8 != 0x2e) ? 997 : 1000;
      break;
    case 1000:  // $DE75
      /*$DE75*/ CYCLES(0xde75, 7);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0xc9);
      /*$DE77*/ branchTarget = true; block_id = !(tmp5_U8 != 0xc9) ? 1024 : 1001;
      break;
    case 1001:  // $DE79
      /*$DE79*/ CYCLES(0xde79, 7);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0xc8);
      /*$DE7B*/ branchTarget = true; block_id = !(tmp5_U8 != 0xc8) ? 995 : 1002;
      break;
    case 1002:  // $DE7D
      /*$DE7D*/ CYCLES(0xde7d, 7);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0x22);
      /*$DE7F*/ branchTarget = true; block_id = (tmp5_U8 != 0x22) ? 1010 : 1003;
      break;
    case 1003:  // $DE81
      /*$DE81*/ CYCLES(0xde81, 14);
                s_a = ram_peek(0x00b8);
      /*$DE83*/ s_y = ram_peek(0x00b9);
      /*$DE85*/ block_id = s_status_d ? 1005 : 1004;
      break;
    case 1004:  // $DE85
      /*$DE85*/ tmp4_U16 = s_a;
                tmp6_U16 = tmp4_U16 + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1006;
      break;
    case 1005:  // $DE85
      /*$DE85*/ tmp6_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1006;
      break;
    case 1006:  // $DE87
      /*$DE87*/ branchTarget = true; block_id = !s_status_c ? 1008 : 1007;
      break;
    case 1007:  // $DE89
      /*$DE89*/ CYCLES(0xde89, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 1008;
      break;
    case 1008:  // $DE8A
      /*$DE8A*/ CYCLES(0xde8a, 6);
                branchTarget = true; push16(0xde8c); block_id = 1252;
      break;
    case 1009:  // $DE8D
      /*$DE8D*/ CYCLES(0xde8d, 6);
      /*$E73D*/ CYCLES(0xe73d, 16);
                tmp5_U8 = ram_peek(0x00ad);
                s_x = tmp5_U8;
      /*$E73F*/ tmp1_U8 = ram_peek(0x00ae);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
      /*$E741*/ ram_poke(0x00b8, tmp5_U8);
      /*$E743*/ ram_poke(0x00b9, tmp1_U8);
      /*$E745*/ branchTarget = true; block_id = find_block_id_func_t001(0xe745, pop16() + 1);;
      break;
    case 1010:  // $DE90
      /*$DE90*/ CYCLES(0xde90, 7);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0xc6);
      /*$DE92*/ branchTarget = true; block_id = (tmp5_U8 != 0xc6) ? 1012 : 1011;
      break;
    case 1011:  // $DE94
      /*$DE94*/ CYCLES(0xde94, 7);
                s_y = 0x18;
      /*$DE96*/ branchTarget = true; block_id = 1025;
      break;
    case 1012:  // $DEA4
      /*$DEA4*/ CYCLES(0xdea4, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0xc2);
      /*$DEA6*/ branchTarget = true; block_id = (tmp1_U8 != 0xc2) ? 1014 : 1013;
      break;
    case 1013:  // $DEA8
      /*$DEA8*/ CYCLES(0xdea8, 6);
      /*$E354*/ CYCLES(0xe354, 6);
                branchTarget = true; push16(0xe356); block_id = 1236;
      break;
    case 1014:  // $DEAB
      /*$DEAB*/ CYCLES(0xdeab, 7);
                tmp1_U8 = s_a >= 0xd2;
                s_status_c = tmp1_U8;
      /*$DEAD*/ branchTarget = true; block_id = !tmp1_U8 ? 1016 : 1015;
      break;
    case 1015:  // $DEAF
      /*$DEAF*/ CYCLES(0xdeaf, 6);
      /*$DF0C*/ CYCLES(0xdf0c, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0xd7);
      /*$DF0E*/ branchTarget = true; block_id = !(tmp1_U8 != 0xd7) ? 1032 : 1036;
      break;
    case 1016:  // $DEB2
      /*$DEB2*/ CYCLES(0xdeb2, 6);
                branchTarget = true; push16(0xdeb4); block_id = 1019;
      break;
    case 1017:  // $DEB5
      /*$DEB5*/ CYCLES(0xdeb5, 6);
                branchTarget = true; push16(0xdeb7); block_id = 938;
      break;
    case 1018:  // $DEB8
      /*$DEB8*/ CYCLES(0xdeb8, 14);
                s_a = 0x29;
      /*$DEBD*/ s_status_v = ((ram_peek(0x2ca9) >> 0x06) & 0x01);
                block_id = 1021;
      break;
    case 1019:  // $DEBB
      /*$DEBB*/ CYCLES(0xdebb, 9);
                s_a = 0x28;
      /*$DEBD*/ s_status_v = ((ram_peek(0x2ca9) >> 0x06) & 0x01);
                block_id = 1021;
      break;
    case 1020:  // $DEBE
      /*$DEBE*/ CYCLES(0xdebe, 4);
                s_a = 0x2c;
                block_id = 1021;
      break;
    case 1021:  // $DEC0
      /*$DEC0*/ CYCLES(0xdec0, 11);
                s_y = 0x00;
      /*$DEC2*/ tmp1_U8 = s_a;
                tmp2_U8 = peek(ram_peek16al(0x00b8));
                s_status_c = (tmp1_U8 >= tmp2_U8);
      /*$DEC4*/ branchTarget = true; block_id = (tmp1_U8 != tmp2_U8) ? 1023 : 1022;
      break;
    case 1022:  // $DEC6
      /*$DEC6*/ CYCLES(0xdec6, 6);
                branchTarget = true; block_id = 2;
      break;
    case 1023:  // $DEC9
      /*$DEC9*/ CYCLES(0xdec9, 9);
                s_x = 0x10;
      /*$DECB*/ branchTarget = true; block_id = 641;
      break;
    case 1024:  // $DECE
      /*$DECE*/ CYCLES(0xdece, 4);
                s_y = 0x15;
                block_id = 1025;
      break;
    case 1025:  // $DED0
      /*$DED0*/ CYCLES(0xded0, 9);
                tmp1_U8 = pop8();
      /*$DED1*/ tmp1_U8 = pop8();
      /*$DED2*/ branchTarget = true; block_id = 973;
      break;
    case 1026:  // $DED5
      /*$DED5*/ CYCLES(0xded5, 6);
                branchTarget = true; push16(0xded7); block_id = 1047;
      break;
    case 1027:  // $DED8
      /*$DED8*/ CYCLES(0xded8, 14);
                ram_poke(0x00a0, s_a);
      /*$DEDA*/ ram_poke(0x00a1, s_y);
      /*$DEDE*/ branchTarget = true; block_id = !ram_peek(0x0011) ? 1029 : 1028;
      break;
    case 1028:  // $DEE0
      /*$DEE0*/ CYCLES(0xdee0, 9);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_x = 0x00;
      /*$DEE2*/ ram_poke(0x00ac, 0x00);
      /*$DEE4*/ branchTarget = true; block_id = find_block_id_func_t001(0xdee4, pop16() + 1);;
      break;
    case 1029:  // $DEE5
      /*$DEE5*/ CYCLES(0xdee5, 7);
                tmp1_U8 = ram_peek(0x0012);
                s_x = tmp1_U8;
      /*$DEE7*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 1031 : 1030;
      break;
    case 1030:  // $DEE9
      /*$DEE9*/ CYCLES(0xdee9, 23);
      /*$DEEB*/ tmp1_U8 = peek(ram_peek16al(0x00a0));
      /*$DEEF*/ tmp5_U8 = peek((ram_peek16al(0x00a0) + 0x0001));
      /*$DEF1*/ s_y = tmp5_U8;
      /*$DEF2*/ s_a = tmp1_U8;
      /*$DEF3*/ branchTarget = true; block_id = 1233;
      break;
    case 1031:  // $DEF6
      /*$DEF6*/ CYCLES(0xdef6, 6);
                FUNC_UPAY2FAC(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xdef6, pop16() + 1);;
      break;
    case 1032:  // $DEF9
      /*$DEF9*/ CYCLES(0xdef9, 6);
                branchTarget = true; push16(0xdefb); block_id = 2;
      break;
    case 1033:  // $DEFC
      /*$DEFC*/ CYCLES(0xdefc, 6);
                branchTarget = true; push16(0xdefe); block_id = 1795;
      break;
    case 1034:  // $DEFF
      /*$DEFF*/ CYCLES(0xdeff, 11);
                s_a = s_x;
      /*$DF00*/ s_y = ram_peek(0x00f0);
      /*$DF02*/ rom_scrn(0xfffe);
      /*$DF05*/ CYCLES(0xdf05, 7);
                s_y = s_a;
      /*$DF06*/ branchTarget = true; push16(0xdf08); block_id = 1234;
      break;
    case 1035:  // $DF09
      /*$DF09*/ CYCLES(0xdf09, 6);
                branchTarget = true; block_id = 1018;
      break;
    case 1036:  // $DF10
      /*$DF10*/ CYCLES(0xdf10, 11);
                tmp1_U8 = (uint8_t)(s_a << 0x01);
                s_a = tmp1_U8;
      /*$DF11*/ push8(tmp1_U8);
      /*$DF12*/ s_x = tmp1_U8;
      /*$DF13*/ branchTarget = true; push16(0xdf15); block_id = 2;
      break;
    case 1037:  // $DF16
      /*$DF16*/ CYCLES(0xdf16, 7);
                tmp1_U8 = s_x >= 0xcf;
                s_status_c = tmp1_U8;
      /*$DF18*/ branchTarget = true; block_id = !tmp1_U8 ? 1044 : 1038;
      break;
    case 1038:  // $DF1A
      /*$DF1A*/ CYCLES(0xdf1a, 6);
                branchTarget = true; push16(0xdf1c); block_id = 1019;
      break;
    case 1039:  // $DF1D
      /*$DF1D*/ CYCLES(0xdf1d, 6);
                branchTarget = true; push16(0xdf1f); block_id = 938;
      break;
    case 1040:  // $DF20
      /*$DF20*/ CYCLES(0xdf20, 6);
                branchTarget = true; push16(0xdf22); block_id = 1020;
      break;
    case 1041:  // $DF23
      /*$DF23*/ CYCLES(0xdf23, 6);
                branchTarget = true; push16(0xdf25); block_id = 932;
      break;
    case 1042:  // $DF26
      /*$DF26*/ CYCLES(0xdf26, 23);
                tmp1_U8 = pop8();
      /*$DF27*/ s_x = tmp1_U8;
      /*$DF2A*/ push8(ram_peek(0x00a1));
      /*$DF2D*/ push8(ram_peek(0x00a0));
      /*$DF2E*/ s_a = tmp1_U8;
      /*$DF2F*/ push8(tmp1_U8);
      /*$DF30*/ branchTarget = true; push16(0xdf32); block_id = 1380;
      break;
    case 1043:  // $DF33
      /*$DF33*/ CYCLES(0xdf33, 12);
                tmp1_U8 = pop8();
      /*$DF34*/ s_y = tmp1_U8;
      /*$DF36*/ push8(s_x);
      /*$DF37*/ branchTarget = true; block_id = 1046;
      break;
    case 1044:  // $DF3A
      /*$DF3A*/ CYCLES(0xdf3a, 6);
                branchTarget = true; push16(0xdf3c); block_id = 1016;
      break;
    case 1045:  // $DF3D
      /*$DF3D*/ CYCLES(0xdf3d, 4);
                tmp1_U8 = pop8();
      /*$DF3E*/ s_y = tmp1_U8;
                block_id = 1046;
      break;
    case 1046:  // $DF3F
      /*$DF3F*/ CYCLES(0xdf3f, 23);
                tmp1_U8 = s_y;
                tmp5_U8 = peek((0xcfdc + tmp1_U8));
      /*$DF42*/ ram_poke(0x0091, tmp5_U8);
      /*$DF44*/ tmp1_U8 = peek((0xcfdd + tmp1_U8));
                s_a = tmp1_U8;
      /*$DF47*/ ram_poke(0x0092, tmp1_U8);
      /*$DF49*/ FUNC_BCC(0xfffe);
      /*$DF4C*/ CYCLES(0xdf4c, 6);
                branchTarget = true; block_id = 931;
      break;
    case 1047:  // $DFE3
      /*$DFE3*/ CYCLES(0xdfe3, 9);
                s_x = 0x00;
      /*$DFE5*/ branchTarget = true; push16(0xdfe7); block_id = 4;
      break;
    case 1048:  // $DFE8
      /*$DFE8*/ CYCLES(0xdfe8, 4);
                ram_poke(0x0010, s_x);
                block_id = 1049;
      break;
    case 1049:  // $DFEA
      /*$DFEA*/ CYCLES(0xdfea, 9);
                ram_poke(0x0081, s_a);
      /*$DFEC*/ branchTarget = true; push16(0xdfee); block_id = 4;
      break;
    case 1050:  // $DFEF
      /*$DFEF*/ CYCLES(0xdfef, 6);
                FUNC_ISLETC(0xfffe);
      /*$DFF2*/ CYCLES(0xdff2, 4);
                branchTarget = true; block_id = s_status_c ? 1052 : 1051;
      break;
    case 1051:  // $DFF4
      /*$DFF4*/ CYCLES(0xdff4, 6);
                branchTarget = true; block_id = 1023;
      break;
    case 1052:  // $DFF7
      /*$DFF7*/ CYCLES(0xdff7, 16);
                s_x = 0x00;
      /*$DFF9*/ ram_poke(0x0011, 0x00);
      /*$DFFB*/ ram_poke(0x0012, 0x00);
      /*$E007*/ CYCLES(0xe007, 6);
                branchTarget = true; push16(0xe009); block_id = 2;
      break;
    case 1053:  // $E00A
      /*$E00A*/ CYCLES(0xe00a, 4);
                branchTarget = true; block_id = !s_status_c ? 1055 : 1054;
      break;
    case 1054:  // $E00C
      /*$E00C*/ CYCLES(0xe00c, 6);
                FUNC_ISLETC(0xfffe);
      /*$E00F*/ CYCLES(0xe00f, 4);
                branchTarget = true; block_id = !s_status_c ? 1059 : 1055;
      break;
    case 1055:  // $E011
      /*$E011*/ CYCLES(0xe011, 2);
                s_x = s_a;
                block_id = 1056;
      break;
    case 1056:  // $E012
      /*$E012*/ CYCLES(0xe012, 6);
                branchTarget = true; push16(0xe014); block_id = 2;
      break;
    case 1057:  // $E015
      /*$E015*/ CYCLES(0xe015, 4);
                branchTarget = true; block_id = !s_status_c ? 1056 : 1058;
      break;
    case 1058:  // $E017
      /*$E017*/ CYCLES(0xe017, 6);
                FUNC_ISLETC(0xfffe);
      /*$E01A*/ CYCLES(0xe01a, 4);
                branchTarget = true; block_id = s_status_c ? 1056 : 1059;
      break;
    case 1059:  // $E01C
      /*$E01C*/ CYCLES(0xe01c, 7);
      /*$E01E*/ branchTarget = true; block_id = (s_a != 0x24) ? 1061 : 1060;
      break;
    case 1060:  // $E020
      /*$E020*/ CYCLES(0xe020, 11);
      /*$E022*/ ram_poke(0x0011, 0xff);
      /*$E024*/ branchTarget = true; block_id = 1064;
      break;
    case 1061:  // $E026
      /*$E026*/ CYCLES(0xe026, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x25);
      /*$E028*/ branchTarget = true; block_id = (tmp1_U8 != 0x25) ? 1065 : 1062;
      break;
    case 1062:  // $E02A
      /*$E02A*/ CYCLES(0xe02a, 7);
      /*$E02C*/ branchTarget = true; block_id = (ram_peek(0x0014) & 0x80) ? 1051 : 1063;
      break;
    case 1063:  // $E02E
      /*$E02E*/ CYCLES(0xe02e, 14);
      /*$E030*/ ram_poke(0x0012, 0x80);
      /*$E034*/ ram_poke(0x0081, (0x80 | ram_peek(0x0081)));
                block_id = 1064;
      break;
    case 1064:  // $E036
      /*$E036*/ CYCLES(0xe036, 12);
      /*$E037*/ tmp1_U8 = s_x | 0x80;
                s_a = tmp1_U8;
      /*$E039*/ s_x = tmp1_U8;
      /*$E03A*/ branchTarget = true; push16(0xe03c); block_id = 2;
      break;
    case 1065:  // $E03D
      /*$E03D*/ CYCLES(0xe03d, 16);
                ram_poke(0x0082, s_x);
      /*$E03F*/ s_status_c = 0x01;
      /*$E040*/ s_a = (s_a | ram_peek(0x0014));
      /*$E042*/ block_id = s_status_d ? 1067 : 1066;
      break;
    case 1066:  // $E042
      /*$E042*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 - 0x0028) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0xffd7);
                s_status_not_z = ((uint8_t)tmp4_U16);
                block_id = 1068;
      break;
    case 1067:  // $E042
      /*$E042*/ tmp1_U8 = (uint8_t)(sbc_dec16(s_a, 0x28, s_status_c) >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 1068;
      break;
    case 1068:  // $E044
      /*$E044*/ branchTarget = true; block_id = s_status_not_z ? 1070 : 1069;
      break;
    case 1069:  // $E046
      /*$E046*/ CYCLES(0xe046, 6);
      /*$E11E*/ CYCLES(0xe11e, 7);
      /*$E120*/ branchTarget = true; block_id = ram_peek(0x0014) ? 1131 : 1125;
      break;
    case 1070:  // $E049
      /*$E049*/ CYCLES(0xe049, 7);
                tmp1_U8 = ram_peek(0x0014);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
      /*$E04B*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 1072 : 1071;
      break;
    case 1071:  // $E04D
      /*$E04D*/ CYCLES(0xe04d, 4);
                branchTarget = true; block_id = s_status_v ? 1069 : 1072;
      break;
    case 1072:  // $E04F
      /*$E04F*/ CYCLES(0xe04f, 18);
      /*$E051*/ ram_poke(0x0014, 0x00);
      /*$E053*/ s_a = ram_peek(0x0069);
      /*$E055*/ s_x = ram_peek(0x006a);
      /*$E057*/ s_y = 0x00;
                block_id = 1073;
      break;
    case 1073:  // $E059
      /*$E059*/ CYCLES(0xe059, 4);
                ram_poke(0x009c, s_x);
                block_id = 1074;
      break;
    case 1074:  // $E05B
      /*$E05B*/ CYCLES(0xe05b, 11);
                ram_poke(0x009b, s_a);
      /*$E05F*/ branchTarget = true; block_id = (s_x != ram_peek(0x006c)) ? 1076 : 1075;
      break;
    case 1075:  // $E061
      /*$E061*/ CYCLES(0xe061, 7);
      /*$E063*/ branchTarget = true; block_id = !(s_a != ram_peek(0x006b)) ? 1093 : 1076;
      break;
    case 1076:  // $E065
      /*$E065*/ CYCLES(0xe065, 11);
      /*$E067*/ tmp1_U8 = peek((ram_peek16(0x009b) + s_y));
      /*$E069*/ branchTarget = true; block_id = (ram_peek(0x0081) != tmp1_U8) ? 1079 : 1077;
      break;
    case 1077:  // $E06B
      /*$E06B*/ CYCLES(0xe06b, 12);
      /*$E06D*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$E06E*/ tmp1_U8 = peek((ram_peek16(0x009b) + tmp1_U8));
      /*$E070*/ branchTarget = true; block_id = !(ram_peek(0x0082) != tmp1_U8) ? 1103 : 1078;
      break;
    case 1078:  // $E072
      /*$E072*/ CYCLES(0xe072, 2);
                s_y = (uint8_t)(s_y - 0x01);
                block_id = 1079;
      break;
    case 1079:  // $E073
      /*$E073*/ CYCLES(0xe073, 12);
                s_status_c = 0x00;
      /*$E074*/ s_a = ram_peek(0x009b);
      /*$E076*/ block_id = s_status_d ? 1081 : 1080;
      break;
    case 1080:  // $E076
      /*$E076*/ tmp4_U16 = s_a;
                tmp6_U16 = (tmp4_U16 + 0x0007) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0x0007);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1082;
      break;
    case 1081:  // $E076
      /*$E076*/ tmp6_U16 = adc_dec16(s_a, 0x07, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 1082;
      break;
    case 1082:  // $E078
      /*$E078*/ branchTarget = true; block_id = !s_status_c ? 1074 : 1083;
      break;
    case 1083:  // $E07A
      /*$E07A*/ CYCLES(0xe07a, 6);
                tmp1_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp1_U8;
      /*$E07B*/ branchTarget = true; block_id = tmp1_U8 ? 1073 : 1936;
      break;
    case 1084:  // $E07D
      /*$E07D*/ CYCLES(0xe07d, 7);
                tmp1_U8 = s_a;
                s_status_not_z = (tmp1_U8 != 0x41);
                tmp5_U8 = tmp1_U8 >= 0x41;
                s_status_c = tmp5_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - 0x41) & 0x80);
      /*$E07F*/ branchTarget = true; block_id = !tmp5_U8 ? 1091 : 1085;
      break;
    case 1085:  // $E081
      /*$E081*/ CYCLES(0xe081, 9);
                block_id = s_status_d ? 1087 : 1086;
      break;
    case 1086:  // $E081
      /*$E081*/ s_a = (uint8_t)((s_a - 0x005b) - (uint8_t)(0x01 - s_status_c));
                block_id = 1088;
      break;
    case 1087:  // $E081
      /*$E081*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x5b, s_status_c));
                block_id = 1088;
      break;
    case 1088:  // $E083
      /*$E083*/ s_status_c = 0x01;
      /*$E084*/ block_id = s_status_d ? 1090 : 1089;
      break;
    case 1089:  // $E084
      /*$E084*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 - 0x00a5) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0xff5a);
                tmp1_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 1092;
      break;
    case 1090:  // $E084
      /*$E084*/ tmp4_U16 = sbc_dec16(s_a, 0xa5, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
                block_id = 1092;
      break;
    case 1091:  // $E086
      /*$E086*/ CYCLES(0xe086, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xe086, pop16() + 1);;
      break;
    case 1092:  // $E086
      /*$E084*/ block_id = 1091;
      break;
    case 1093:  // $E087
      /*$E087*/ CYCLES(0xe087, 11);
                tmp1_U8 = pop8();
      /*$E088*/ push8(tmp1_U8);
      /*$E08B*/ branchTarget = true; block_id = (tmp1_U8 != 0xd7) ? 1096 : 1094;
      break;
    case 1094:  // $E08D
      /*$E08D*/ CYCLES(0xe08d, 14);
                tmp5_U8 = s_sp;
                s_x = tmp5_U8;
      /*$E08E*/ tmp5_U8 = ram_peek((0x0102 + tmp5_U8));
      /*$E091*/ s_status_c = (tmp5_U8 >= 0xde);
      /*$E093*/ branchTarget = true; block_id = (tmp5_U8 != 0xde) ? 1096 : 1095;
      break;
    case 1095:  // $E095
      /*$E095*/ CYCLES(0xe095, 9);
                s_a = 0x9a;
      /*$E097*/ s_status_not_z = 0xe0;
                s_status_n = 0x80;
                s_y = 0xe0;
      /*$E099*/ branchTarget = true; block_id = find_block_id_func_t001(0xe099, pop16() + 1);;
      break;
    case 1096:  // $E09C
      /*$E09C*/ CYCLES(0xe09c, 36);
      /*$E0A0*/ ram_poke(0x009b, ram_peek(0x006b));
      /*$E0A2*/ ram_poke(0x009c, ram_peek(0x006c));
      /*$E0A4*/ tmp1_U8 = ram_peek(0x006d);
                s_a = tmp1_U8;
      /*$E0A6*/ tmp5_U8 = ram_peek(0x006e);
                s_y = tmp5_U8;
      /*$E0A8*/ ram_poke(0x0096, tmp1_U8);
      /*$E0AA*/ ram_poke(0x0097, tmp5_U8);
      /*$E0AC*/ s_status_c = 0x00;
      /*$E0AD*/ block_id = s_status_d ? 1098 : 1097;
      break;
    case 1097:  // $E0AD
      /*$E0AD*/ tmp4_U16 = s_a;
                tmp6_U16 = (tmp4_U16 + 0x0007) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0x0007);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1099;
      break;
    case 1098:  // $E0AD
      /*$E0AD*/ tmp6_U16 = adc_dec16(s_a, 0x07, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1099;
      break;
    case 1099:  // $E0AF
      /*$E0AF*/ branchTarget = true; block_id = !s_status_c ? 1101 : 1100;
      break;
    case 1100:  // $E0B1
      /*$E0B1*/ CYCLES(0xe0b1, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 1101;
      break;
    case 1101:  // $E0B2
      /*$E0B2*/ CYCLES(0xe0b2, 12);
                ram_poke(0x0094, s_a);
      /*$E0B4*/ ram_poke(0x0095, s_y);
      /*$E0B6*/ branchTarget = true; push16(0xe0b8); block_id = 599;
      break;
    case 1102:  // $E0B9
      /*$E0B9*/ CYCLES(0xe0b9, 63);
      /*$E0BE*/ ram_poke(0x006b, ram_peek(0x0094));
      /*$E0C0*/ ram_poke(0x006c, (uint8_t)(ram_peek(0x0095) + 0x01));
      /*$E0C6*/ poke(ram_peek16(0x009b), ram_peek(0x0081));
      /*$E0CB*/ poke((ram_peek16(0x009b) + 0x0001), ram_peek(0x0082));
      /*$E0D0*/ poke((ram_peek16(0x009b) + 0x0002), 0x00);
      /*$E0D3*/ poke((ram_peek16(0x009b) + 0x0003), 0x00);
      /*$E0D6*/ poke((ram_peek16(0x009b) + 0x0004), 0x00);
      /*$E0D9*/ poke((ram_peek16(0x009b) + 0x0005), 0x00);
      /*$E0DC*/ poke((ram_peek16(0x009b) + 0x0006), 0x00);
                block_id = 1103;
      break;
    case 1103:  // $E0DE
      /*$E0DE*/ CYCLES(0xe0de, 16);
                s_a = ram_peek(0x009b);
      /*$E0E0*/ s_status_c = 0x00;
      /*$E0E1*/ block_id = s_status_d ? 1105 : 1104;
      break;
    case 1104:  // $E0E1
      /*$E0E1*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 + 0x0002) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x0002);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1106;
      break;
    case 1105:  // $E0E1
      /*$E0E1*/ tmp4_U16 = adc_dec16(s_a, 0x02, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1106;
      break;
    case 1106:  // $E0E3
      /*$E0E3*/ tmp5_U8 = ram_peek(0x009c);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_y = tmp5_U8;
      /*$E0E5*/ branchTarget = true; block_id = !s_status_c ? 1108 : 1107;
      break;
    case 1107:  // $E0E7
      /*$E0E7*/ CYCLES(0xe0e7, 2);
                tmp5_U8 = (uint8_t)(s_y + 0x01);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_y = tmp5_U8;
                block_id = 1108;
      break;
    case 1108:  // $E0E8
      /*$E0E8*/ CYCLES(0xe0e8, 9);
                ram_poke(0x0083, s_a);
      /*$E0EA*/ ram_poke(0x0084, s_y);
      /*$E0EC*/ branchTarget = true; block_id = find_block_id_func_t001(0xe0ec, pop16() + 1);;
      break;
    case 1109:  // $E0ED
      /*$E0ED*/ CYCLES(0xe0ed, 19);
      /*$E0EF*/ tmp4_U16 = ram_peek(0x000f) << 0x01;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
      /*$E0F0*/ block_id = s_status_d ? 1111 : 1110;
      break;
    case 1110:  // $E0F0
      /*$E0F0*/ tmp4_U16 = (s_a + 0x0005) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1112;
      break;
    case 1111:  // $E0F0
      /*$E0F0*/ tmp4_U16 = adc_dec16(s_a, 0x05, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1112;
      break;
    case 1112:  // $E0F2
      /*$E0F2*/ block_id = s_status_d ? 1114 : 1113;
      break;
    case 1113:  // $E0F2
      /*$E0F2*/ tmp4_U16 = s_a;
                tmp6_U16 = ram_peek(0x009b);
                tmp3_U16 = (tmp4_U16 + tmp6_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp6_U16);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1115;
      break;
    case 1114:  // $E0F2
      /*$E0F2*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x009b), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1115;
      break;
    case 1115:  // $E0F4
      /*$E0F4*/ tmp5_U8 = ram_peek(0x009c);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_y = tmp5_U8;
      /*$E0F6*/ branchTarget = true; block_id = !s_status_c ? 1117 : 1116;
      break;
    case 1116:  // $E0F8
      /*$E0F8*/ CYCLES(0xe0f8, 2);
                tmp5_U8 = (uint8_t)(s_y + 0x01);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_y = tmp5_U8;
                block_id = 1117;
      break;
    case 1117:  // $E0F9
      /*$E0F9*/ CYCLES(0xe0f9, 9);
                ram_poke(0x0094, s_a);
      /*$E0FB*/ ram_poke(0x0095, s_y);
      /*$E0FD*/ branchTarget = true; block_id = find_block_id_func_t001(0xe0fd, pop16() + 1);;
      break;
    case 1118:  // $E102
      /*$E102*/ CYCLES(0xe102, 6);
                branchTarget = true; push16(0xe104); block_id = 2;
      break;
    case 1119:  // $E105
      /*$E105*/ CYCLES(0xe105, 6);
                branchTarget = true; push16(0xe107); block_id = 930;
      break;
    case 1120:  // $E108
      /*$E108*/ CYCLES(0xe108, 7);
                tmp5_U8 = ram_peek(0x00a2);
                s_status_not_z = tmp5_U8;
      /*$E10A*/ branchTarget = true; block_id = (tmp5_U8 & 0x80) ? 1123 : 1121;
      break;
    case 1121:  // $E10C
      /*$E10C*/ CYCLES(0xe10c, 11);
      /*$E10E*/ tmp2_U8 = ram_peek(0x009d) >= 0x90;
                s_status_c = tmp2_U8;
      /*$E110*/ branchTarget = true; block_id = !tmp2_U8 ? 1124 : 1122;
      break;
    case 1122:  // $E112
      /*$E112*/ CYCLES(0xe112, 12);
                s_a = 0xfe;
      /*$E114*/ s_y = 0xe0;
      /*$E116*/ FUNC_FCOMP(0xfffe);
                branchTarget = true; block_id = 1123;
      break;
    case 1123:  // $E119
      /*$E119*/ CYCLES(0xe119, 4);
                branchTarget = true; block_id = s_status_not_z ? 1144 : 1124;
      break;
    case 1124:  // $E11B
      /*$E11B*/ CYCLES(0xe11b, 6);
                FUNC_QINT(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xe11b, pop16() + 1);;
      break;
    case 1125:  // $E122
      /*$E122*/ CYCLES(0xe122, 18);
      /*$E126*/ push8((ram_peek(0x0010) | ram_peek(0x0012)));
      /*$E129*/ push8(ram_peek(0x0011));
      /*$E12A*/ s_y = 0x00;
                block_id = 1126;
      break;
    case 1126:  // $E12C
      /*$E12C*/ CYCLES(0xe12c, 19);
      /*$E12D*/ push8(s_y);
      /*$E130*/ push8(ram_peek(0x0082));
      /*$E131*/ tmp5_U8 = ram_peek(0x0081);
                s_a = tmp5_U8;
      /*$E133*/ push8(tmp5_U8);
      /*$E134*/ branchTarget = true; push16(0xe136); block_id = 1118;
      break;
    case 1127:  // $E137
      /*$E137*/ CYCLES(0xe137, 53);
                tmp5_U8 = pop8();
      /*$E138*/ ram_poke(0x0081, tmp5_U8);
      /*$E13A*/ tmp5_U8 = pop8();
      /*$E13B*/ ram_poke(0x0082, tmp5_U8);
      /*$E13D*/ tmp5_U8 = pop8();
      /*$E13F*/ tmp1_U8 = s_sp;
                s_x = tmp1_U8;
      /*$E143*/ push8(ram_peek((0x0102 + tmp1_U8)));
      /*$E147*/ push8(ram_peek((0x0101 + tmp1_U8)));
      /*$E14A*/ ram_poke((0x0102 + tmp1_U8), ram_peek(0x00a0));
      /*$E14D*/ tmp2_U8 = ram_peek(0x00a1);
                s_a = tmp2_U8;
      /*$E14F*/ ram_poke((0x0101 + tmp1_U8), tmp2_U8);
      /*$E152*/ s_y = (uint8_t)(tmp5_U8 + 0x01);
      /*$E153*/ branchTarget = true; push16(0xe155); block_id = 4;
      break;
    case 1128:  // $E156
      /*$E156*/ CYCLES(0xe156, 7);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0x2c);
      /*$E158*/ branchTarget = true; block_id = !(tmp5_U8 != 0x2c) ? 1126 : 1129;
      break;
    case 1129:  // $E15A
      /*$E15A*/ CYCLES(0xe15a, 9);
                ram_poke(0x000f, s_y);
      /*$E15C*/ branchTarget = true; push16(0xe15e); block_id = 1018;
      break;
    case 1130:  // $E15F
      /*$E15F*/ CYCLES(0xe15f, 18);
                tmp5_U8 = pop8();
      /*$E160*/ ram_poke(0x0011, tmp5_U8);
      /*$E162*/ tmp5_U8 = pop8();
      /*$E163*/ ram_poke(0x0012, tmp5_U8);
      /*$E167*/ ram_poke(0x0010, (tmp5_U8 & 0x7f));
                block_id = 1131;
      break;
    case 1131:  // $E169
      /*$E169*/ CYCLES(0xe169, 7);
                s_x = ram_peek(0x006b);
      /*$E16B*/ s_a = ram_peek(0x006c);
                block_id = 1132;
      break;
    case 1132:  // $E16D
      /*$E16D*/ CYCLES(0xe16d, 14);
                ram_poke(0x009b, s_x);
      /*$E16F*/ tmp5_U8 = s_a;
                ram_poke(0x009c, tmp5_U8);
      /*$E173*/ branchTarget = true; block_id = (tmp5_U8 != ram_peek(0x006e)) ? 1134 : 1133;
      break;
    case 1133:  // $E175
      /*$E175*/ CYCLES(0xe175, 7);
                tmp5_U8 = s_x;
                tmp1_U8 = ram_peek(0x006d);
                s_status_c = (tmp5_U8 >= tmp1_U8);
      /*$E177*/ branchTarget = true; block_id = !(tmp5_U8 != tmp1_U8) ? 1151 : 1134;
      break;
    case 1134:  // $E179
      /*$E179*/ CYCLES(0xe179, 16);
      /*$E17B*/ tmp5_U8 = peek(ram_peek16(0x009b));
      /*$E17D*/ s_y = 0x01;
      /*$E180*/ branchTarget = true; block_id = (tmp5_U8 != ram_peek(0x0081)) ? 1136 : 1135;
      break;
    case 1135:  // $E182
      /*$E182*/ CYCLES(0xe182, 11);
                tmp1_U8 = ram_peek(0x0082);
      /*$E184*/ tmp5_U8 = peek((ram_peek16(0x009b) + s_y));
                s_status_c = (tmp1_U8 >= tmp5_U8);
      /*$E186*/ branchTarget = true; block_id = !(tmp1_U8 != tmp5_U8) ? 1146 : 1136;
      break;
    case 1136:  // $E188
      /*$E188*/ CYCLES(0xe188, 24);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$E189*/ tmp1_U8 = peek((ram_peek16(0x009b) + tmp1_U8));
                s_a = tmp1_U8;
      /*$E18B*/ s_status_c = 0x00;
      /*$E18C*/ block_id = s_status_d ? 1138 : 1137;
      break;
    case 1137:  // $E18C
      /*$E18C*/ tmp3_U16 = (s_a + ram_peek(0x009b)) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1139;
      break;
    case 1138:  // $E18C
      /*$E18C*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x009b), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 1139;
      break;
    case 1139:  // $E18E
      /*$E18E*/ s_x = s_a;
      /*$E18F*/ tmp5_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp5_U8;
      /*$E190*/ tmp5_U8 = peek((ram_peek16(0x009b) + tmp5_U8));
                s_a = tmp5_U8;
      /*$E192*/ block_id = s_status_d ? 1141 : 1140;
      break;
    case 1140:  // $E192
      /*$E192*/ tmp3_U16 = s_a;
                tmp6_U16 = ram_peek(0x009c);
                tmp4_U16 = (tmp3_U16 + tmp6_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)tmp6_U16);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1142;
      break;
    case 1141:  // $E192
      /*$E192*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x009c), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1142;
      break;
    case 1142:  // $E194
      /*$E194*/ branchTarget = true; block_id = !s_status_c ? 1132 : 1143;
      break;
    case 1143:  // $E196
      /*$E196*/ CYCLES(0xe196, 9);
                s_x = 0x6b;
      /*$E198*/ block_id = 1145;
      break;
    case 1144:  // $E199
      /*$E199*/ CYCLES(0xe199, 4);
                s_x = 0x35;
                block_id = 1145;
      break;
    case 1145:  // $E19B
      /*$E19B*/ CYCLES(0xe19b, 6);
                branchTarget = true; block_id = 641;
      break;
    case 1146:  // $E19E
      /*$E19E*/ CYCLES(0xe19e, 11);
                s_x = 0x78;
      /*$E1A2*/ branchTarget = true; block_id = ram_peek(0x0010) ? 1145 : 1147;
      break;
    case 1147:  // $E1A4
      /*$E1A4*/ CYCLES(0xe1a4, 7);
                tmp5_U8 = ram_peek(0x0014);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$E1A6*/ branchTarget = true; block_id = !tmp5_U8 ? 1149 : 1148;
      break;
    case 1148:  // $E1A8
      /*$E1A8*/ CYCLES(0xe1a8, 4);
                s_status_c = 0x01;
      /*$E1A9*/ branchTarget = true; block_id = find_block_id_func_t001(0xe1a9, pop16() + 1);;
      break;
    case 1149:  // $E1AA
      /*$E1AA*/ CYCLES(0xe1aa, 6);
                FUNC_GETARY(0xfffe);
      /*$E1AD*/ CYCLES(0xe1ad, 14);
                tmp5_U8 = ram_peek(0x000f);
      /*$E1AF*/ s_y = 0x04;
      /*$E1B1*/ tmp1_U8 = peek((ram_peek16(0x009b) + 0x0004));
                s_status_c = (tmp5_U8 >= tmp1_U8);
      /*$E1B3*/ branchTarget = true; block_id = (tmp5_U8 != tmp1_U8) ? 1143 : 1150;
      break;
    case 1150:  // $E1B5
      /*$E1B5*/ CYCLES(0xe1b5, 6);
                branchTarget = true; block_id = 1190;
      break;
    case 1151:  // $E1B8
      /*$E1B8*/ CYCLES(0xe1b8, 7);
      /*$E1BA*/ branchTarget = true; block_id = !ram_peek(0x0014) ? 1153 : 1152;
      break;
    case 1152:  // $E1BC
      /*$E1BC*/ CYCLES(0xe1bc, 9);
                s_x = 0x2a;
      /*$E1BE*/ branchTarget = true; block_id = 641;
      break;
    case 1153:  // $E1C1
      /*$E1C1*/ CYCLES(0xe1c1, 6);
                FUNC_GETARY(0xfffe);
      /*$E1C4*/ CYCLES(0xe1c4, 6);
                branchTarget = true; push16(0xe1c6); block_id = 629;
      break;
    case 1154:  // $E1C7
      /*$E1C7*/ CYCLES(0xe1c7, 23);
      /*$E1C9*/ s_y = 0x00;
      /*$E1CA*/ ram_poke(0x00ae, 0x00);
      /*$E1CC*/ s_x = 0x05;
      /*$E1CE*/ tmp1_U8 = ram_peek(0x0081);
      /*$E1D0*/ poke(ram_peek16(0x009b), tmp1_U8);
      /*$E1D2*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 1156 : 1155;
      break;
    case 1155:  // $E1D4
      /*$E1D4*/ CYCLES(0xe1d4, 2);
                s_x = (uint8_t)(s_x - 0x01);
                block_id = 1156;
      break;
    case 1156:  // $E1D5
      /*$E1D5*/ CYCLES(0xe1d5, 12);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$E1D6*/ tmp5_U8 = ram_peek(0x0082);
      /*$E1D8*/ poke((ram_peek16(0x009b) + tmp1_U8), tmp5_U8);
      /*$E1DA*/ branchTarget = true; block_id = !(tmp5_U8 & 0x80) ? 1158 : 1157;
      break;
    case 1157:  // $E1DC
      /*$E1DC*/ CYCLES(0xe1dc, 4);
      /*$E1DD*/ s_x = (uint8_t)(s_x - 0x02);
                block_id = 1158;
      break;
    case 1158:  // $E1DE
      /*$E1DE*/ CYCLES(0xe1de, 16);
                ram_poke(0x00ad, s_x);
      /*$E1E4*/ tmp5_U8 = (uint8_t)(s_y + 0x03);
                s_y = tmp5_U8;
      /*$E1E5*/ poke((ram_peek16(0x009b) + tmp5_U8), ram_peek(0x000f));
                block_id = 1159;
      break;
    case 1159:  // $E1E7
      /*$E1E7*/ CYCLES(0xe1e7, 14);
                s_x = 0x0b;
      /*$E1E9*/ s_a = 0x00;
      /*$E1EB*/ tmp5_U8 = (ram_peek(0x0010) >> 0x06) & 0x01;
                s_status_v = tmp5_U8;
      /*$E1ED*/ branchTarget = true; block_id = !tmp5_U8 ? 1166 : 1160;
      break;
    case 1160:  // $E1EF
      /*$E1EF*/ CYCLES(0xe1ef, 14);
                tmp5_U8 = pop8();
                s_a = tmp5_U8;
      /*$E1F0*/ s_status_c = 0x00;
      /*$E1F1*/ block_id = s_status_d ? 1162 : 1161;
      break;
    case 1161:  // $E1F1
      /*$E1F1*/ tmp4_U16 = (s_a + 0x0001) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1163;
      break;
    case 1162:  // $E1F1
      /*$E1F1*/ tmp4_U16 = adc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1163;
      break;
    case 1163:  // $E1F3
      /*$E1F3*/ s_x = s_a;
      /*$E1F4*/ tmp5_U8 = pop8();
                s_a = tmp5_U8;
      /*$E1F5*/ block_id = s_status_d ? 1165 : 1164;
      break;
    case 1164:  // $E1F5
      /*$E1F5*/ tmp4_U16 = s_a;
                tmp6_U16 = tmp4_U16 + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1167;
      break;
    case 1165:  // $E1F5
      /*$E1F5*/ tmp6_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1167;
      break;
    case 1166:  // $E1F7
      /*$E1F7*/ CYCLES(0xe1f7, 18);
                tmp5_U8 = s_y;
      /*$E1F8*/ poke((ram_peek16(0x009b) + (uint8_t)(tmp5_U8 + 0x01)), s_a);
      /*$E1FA*/ tmp5_U8 = (uint8_t)(tmp5_U8 + 0x02);
                s_y = tmp5_U8;
      /*$E1FB*/ tmp1_U8 = s_x;
                s_a = tmp1_U8;
      /*$E1FC*/ poke((ram_peek16(0x009b) + tmp5_U8), tmp1_U8);
      /*$E1FE*/ branchTarget = true; push16(0xe200); block_id = 1220;
      break;
    case 1167:  // $E1F7
      /*$E1F5*/ block_id = 1166;
      break;
    case 1168:  // $E201
      /*$E201*/ CYCLES(0xe201, 18);
                ram_poke(0x00ad, s_x);
      /*$E203*/ ram_poke(0x00ae, s_a);
      /*$E205*/ s_y = ram_peek(0x005e);
      /*$E207*/ tmp5_U8 = (uint8_t)(ram_peek(0x000f) - 0x01);
                ram_poke(0x000f, tmp5_U8);
      /*$E209*/ branchTarget = true; block_id = tmp5_U8 ? 1159 : 1169;
      break;
    case 1169:  // $E20B
      /*$E20B*/ CYCLES(0xe20b, 7);
                block_id = s_status_d ? 1171 : 1170;
      break;
    case 1170:  // $E20B
      /*$E20B*/ tmp6_U16 = (s_a + ram_peek(0x0095)) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1172;
      break;
    case 1171:  // $E20B
      /*$E20B*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x0095), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1172;
      break;
    case 1172:  // $E20D
      /*$E20D*/ branchTarget = true; block_id = s_status_c ? 1195 : 1173;
      break;
    case 1173:  // $E20F
      /*$E20F*/ CYCLES(0xe20f, 14);
                tmp5_U8 = s_a;
                ram_poke(0x0095, tmp5_U8);
      /*$E211*/ s_y = tmp5_U8;
      /*$E212*/ s_a = s_x;
      /*$E213*/ block_id = s_status_d ? 1175 : 1174;
      break;
    case 1174:  // $E213
      /*$E213*/ tmp6_U16 = s_a;
                tmp4_U16 = ram_peek(0x0094);
                tmp3_U16 = (tmp6_U16 + tmp4_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1176;
      break;
    case 1175:  // $E213
      /*$E213*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x0094), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1176;
      break;
    case 1176:  // $E215
      /*$E215*/ branchTarget = true; block_id = !s_status_c ? 1178 : 1177;
      break;
    case 1177:  // $E217
      /*$E217*/ CYCLES(0xe217, 6);
                tmp5_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp5_U8;
      /*$E218*/ branchTarget = true; block_id = !tmp5_U8 ? 1195 : 1178;
      break;
    case 1178:  // $E21A
      /*$E21A*/ CYCLES(0xe21a, 6);
                branchTarget = true; push16(0xe21c); block_id = 629;
      break;
    case 1179:  // $E21D
      /*$E21D*/ CYCLES(0xe21d, 21);
                ram_poke(0x006d, s_a);
      /*$E21F*/ ram_poke(0x006e, s_y);
      /*$E221*/ s_a = 0x00;
      /*$E223*/ ram_poke(0x00ae, (uint8_t)(ram_peek(0x00ae) + 0x01));
      /*$E225*/ tmp5_U8 = ram_peek(0x00ad);
                s_y = tmp5_U8;
      /*$E227*/ branchTarget = true; block_id = !tmp5_U8 ? 1181 : 1180;
      break;
    case 1180:  // $E229
      /*$E229*/ CYCLES(0xe229, 9);
                tmp5_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp5_U8;
      /*$E22A*/ poke((ram_peek16al(0x0094) + tmp5_U8), s_a);
      /*$E22C*/ branchTarget = true; block_id = tmp5_U8 ? 1180 : 1181;
      break;
    case 1181:  // $E22E
      /*$E22E*/ CYCLES(0xe22e, 11);
                ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) - 0x01));
      /*$E230*/ tmp5_U8 = (uint8_t)(ram_peek(0x00ae) - 0x01);
                ram_poke(0x00ae, tmp5_U8);
      /*$E232*/ branchTarget = true; block_id = tmp5_U8 ? 1180 : 1182;
      break;
    case 1182:  // $E234
      /*$E234*/ CYCLES(0xe234, 38);
                ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) + 0x01));
      /*$E236*/ s_status_c = 0x01;
      /*$E237*/ s_a = ram_peek(0x006d);
      /*$E239*/ block_id = s_status_d ? 1184 : 1183;
      break;
    case 1183:  // $E239
      /*$E239*/ tmp3_U16 = (s_a - ram_peek(0x009b)) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1185;
      break;
    case 1184:  // $E239
      /*$E239*/ tmp3_U16 = sbc_dec16(s_a, ram_peek(0x009b), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 1185;
      break;
    case 1185:  // $E23B
      /*$E23D*/ poke((ram_peek16(0x009b) + 0x0002), s_a);
      /*$E23F*/ s_a = ram_peek(0x006e);
      /*$E241*/ s_y = 0x03;
      /*$E242*/ block_id = s_status_d ? 1187 : 1186;
      break;
    case 1186:  // $E242
      /*$E242*/ tmp3_U16 = s_a;
                tmp4_U16 = ram_peek(0x009c);
                tmp6_U16 = (tmp3_U16 - tmp4_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp4_U16));
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1188;
      break;
    case 1187:  // $E242
      /*$E242*/ tmp6_U16 = sbc_dec16(s_a, ram_peek(0x009c), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1188;
      break;
    case 1188:  // $E244
      /*$E244*/ poke((ram_peek16(0x009b) + s_y), s_a);
      /*$E246*/ tmp5_U8 = ram_peek(0x0010);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$E248*/ branchTarget = true; block_id = tmp5_U8 ? 1219 : 1189;
      break;
    case 1189:  // $E24A
      /*$E24A*/ CYCLES(0xe24a, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 1190;
      break;
    case 1190:  // $E24B
      /*$E24B*/ CYCLES(0xe24b, 14);
                tmp1_U8 = peek((ram_peek16(0x009b) + s_y));
      /*$E24D*/ ram_poke(0x000f, tmp1_U8);
      /*$E24F*/ s_a = 0x00;
      /*$E251*/ ram_poke(0x00ad, 0x00);
                block_id = 1191;
      break;
    case 1191:  // $E253
      /*$E253*/ CYCLES(0xe253, 24);
                ram_poke(0x00ae, s_a);
      /*$E255*/ tmp5_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp5_U8;
      /*$E256*/ tmp1_U8 = pop8();
      /*$E257*/ s_x = tmp1_U8;
      /*$E258*/ ram_poke(0x00a0, tmp1_U8);
      /*$E25A*/ tmp1_U8 = pop8();
      /*$E25B*/ ram_poke(0x00a1, tmp1_U8);
      /*$E25D*/ tmp5_U8 = peek((ram_peek16(0x009b) + tmp5_U8));
                s_status_not_z = (tmp1_U8 != tmp5_U8);
                tmp5_U8 = tmp1_U8 >= tmp5_U8;
                s_status_c = tmp5_U8;
      /*$E25F*/ branchTarget = true; block_id = !tmp5_U8 ? 1196 : 1192;
      break;
    case 1192:  // $E261
      /*$E261*/ CYCLES(0xe261, 4);
                branchTarget = true; block_id = s_status_not_z ? 1194 : 1193;
      break;
    case 1193:  // $E263
      /*$E263*/ CYCLES(0xe263, 11);
                tmp5_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp5_U8;
      /*$E265*/ tmp5_U8 = peek((ram_peek16(0x009b) + tmp5_U8));
                tmp5_U8 = s_x >= tmp5_U8;
                s_status_c = tmp5_U8;
      /*$E267*/ branchTarget = true; block_id = !tmp5_U8 ? 1197 : 1194;
      break;
    case 1194:  // $E269
      /*$E269*/ CYCLES(0xe269, 6);
                branchTarget = true; block_id = 1143;
      break;
    case 1195:  // $E26C
      /*$E26C*/ CYCLES(0xe26c, 6);
                branchTarget = true; block_id = 640;
      break;
    case 1196:  // $E26F
      /*$E26F*/ CYCLES(0xe26f, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 1197;
      break;
    case 1197:  // $E270
      /*$E270*/ CYCLES(0xe270, 12);
      /*$E272*/ tmp5_U8 = ram_peek(0x00ae) | ram_peek(0x00ad);
                s_a = tmp5_U8;
      /*$E274*/ s_status_c = 0x00;
      /*$E275*/ branchTarget = true; block_id = !tmp5_U8 ? 1203 : 1198;
      break;
    case 1198:  // $E277
      /*$E277*/ CYCLES(0xe277, 6);
                branchTarget = true; push16(0xe279); block_id = 1220;
      break;
    case 1199:  // $E27A
      /*$E27A*/ CYCLES(0xe27a, 12);
                s_a = s_x;
      /*$E27B*/ block_id = s_status_d ? 1201 : 1200;
      break;
    case 1200:  // $E27B
      /*$E27B*/ tmp6_U16 = (s_a + ram_peek(0x00a0)) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1202;
      break;
    case 1201:  // $E27B
      /*$E27B*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x00a0), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1202;
      break;
    case 1202:  // $E27D
      /*$E27D*/ s_x = s_a;
      /*$E27E*/ s_a = s_y;
      /*$E27F*/ s_y = ram_peek(0x005e);
                block_id = 1203;
      break;
    case 1203:  // $E281
      /*$E281*/ CYCLES(0xe281, 14);
                block_id = s_status_d ? 1205 : 1204;
      break;
    case 1204:  // $E281
      /*$E281*/ tmp6_U16 = s_a;
                tmp4_U16 = ram_peek(0x00a1);
                tmp3_U16 = (tmp6_U16 + tmp4_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1206;
      break;
    case 1205:  // $E281
      /*$E281*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x00a1), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1206;
      break;
    case 1206:  // $E283
      /*$E283*/ ram_poke(0x00ad, s_x);
      /*$E285*/ tmp5_U8 = (uint8_t)(ram_peek(0x000f) - 0x01);
                ram_poke(0x000f, tmp5_U8);
      /*$E287*/ branchTarget = true; block_id = tmp5_U8 ? 1191 : 1207;
      break;
    case 1207:  // $E289
      /*$E289*/ CYCLES(0xe289, 14);
                ram_poke(0x00ae, s_a);
      /*$E28B*/ s_x = 0x05;
      /*$E28F*/ branchTarget = true; block_id = !(ram_peek(0x0081) & 0x80) ? 1209 : 1208;
      break;
    case 1208:  // $E291
      /*$E291*/ CYCLES(0xe291, 2);
                s_x = (uint8_t)(s_x - 0x01);
                block_id = 1209;
      break;
    case 1209:  // $E292
      /*$E292*/ CYCLES(0xe292, 7);
      /*$E294*/ branchTarget = true; block_id = !(ram_peek(0x0082) & 0x80) ? 1211 : 1210;
      break;
    case 1210:  // $E296
      /*$E296*/ CYCLES(0xe296, 4);
      /*$E297*/ s_x = (uint8_t)(s_x - 0x02);
                block_id = 1211;
      break;
    case 1211:  // $E298
      /*$E298*/ CYCLES(0xe298, 12);
                ram_poke(0x0064, s_x);
      /*$E29A*/ s_a = 0x00;
      /*$E29C*/ branchTarget = true; push16(0xe29e); block_id = 1221;
      break;
    case 1212:  // $E29F
      /*$E29F*/ CYCLES(0xe29f, 23);
                s_a = s_x;
      /*$E2A0*/ block_id = s_status_d ? 1214 : 1213;
      break;
    case 1213:  // $E2A0
      /*$E2A0*/ tmp3_U16 = (s_a + ram_peek(0x0094)) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1215;
      break;
    case 1214:  // $E2A0
      /*$E2A0*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x0094), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 1215;
      break;
    case 1215:  // $E2A2
      /*$E2A2*/ ram_poke(0x0083, s_a);
      /*$E2A4*/ s_a = s_y;
      /*$E2A5*/ block_id = s_status_d ? 1217 : 1216;
      break;
    case 1216:  // $E2A5
      /*$E2A5*/ tmp3_U16 = s_a;
                tmp4_U16 = ram_peek(0x0095);
                tmp6_U16 = (tmp3_U16 + tmp4_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1218;
      break;
    case 1217:  // $E2A5
      /*$E2A5*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x0095), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1218;
      break;
    case 1218:  // $E2A7
      /*$E2A7*/ tmp5_U8 = s_a;
                ram_poke(0x0084, tmp5_U8);
      /*$E2A9*/ s_y = tmp5_U8;
      /*$E2AA*/ tmp5_U8 = ram_peek(0x0083);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
                block_id = 1219;
      break;
    case 1219:  // $E2AC
      /*$E2AC*/ CYCLES(0xe2ac, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xe2ac, pop16() + 1);;
      break;
    case 1220:  // $E2AD
      /*$E2AD*/ CYCLES(0xe2ad, 16);
                tmp5_U8 = s_y;
                ram_poke(0x005e, tmp5_U8);
      /*$E2AF*/ tmp1_U8 = peek((ram_peek16(0x009b) + tmp5_U8));
      /*$E2B1*/ ram_poke(0x0064, tmp1_U8);
      /*$E2B4*/ tmp5_U8 = peek((ram_peek16(0x009b) + (uint8_t)(tmp5_U8 - 0x01)));
                s_a = tmp5_U8;
                block_id = 1221;
      break;
    case 1221:  // $E2B6
      /*$E2B6*/ CYCLES(0xe2b6, 18);
                ram_poke(0x0065, s_a);
      /*$E2BA*/ ram_poke(0x0099, 0x10);
      /*$E2BC*/ s_x = 0x00;
      /*$E2BE*/ s_y = 0x00;
                block_id = 1222;
      break;
    case 1222:  // $E2C0
      /*$E2C0*/ CYCLES(0xe2c0, 14);
      /*$E2C1*/ tmp6_U16 = s_x << 0x01;
      /*$E2C2*/ s_x = ((uint8_t)tmp6_U16);
      /*$E2C4*/ tmp6_U16 = (s_y << 0x01) | (uint8_t)(tmp6_U16 >> 8);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = tmp5_U8;
                tmp1_U8 = (uint8_t)tmp6_U16;
                s_a = tmp1_U8;
      /*$E2C5*/ s_y = tmp1_U8;
      /*$E2C6*/ branchTarget = true; block_id = tmp5_U8 ? 1195 : 1223;
      break;
    case 1223:  // $E2C8
      /*$E2C8*/ CYCLES(0xe2c8, 11);
                tmp6_U16 = ram_peek(0x00ad) << 0x01;
                ram_poke(0x00ad, ((uint8_t)tmp6_U16));
      /*$E2CA*/ tmp6_U16 = (ram_peek(0x00ae) << 0x01) | (uint8_t)(tmp6_U16 >> 8);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = tmp5_U8;
                ram_poke(0x00ae, ((uint8_t)tmp6_U16));
      /*$E2CC*/ branchTarget = true; block_id = !tmp5_U8 ? 1231 : 1224;
      break;
    case 1224:  // $E2CE
      /*$E2CE*/ CYCLES(0xe2ce, 19);
                s_status_c = 0x00;
      /*$E2CF*/ s_a = s_x;
      /*$E2D0*/ block_id = s_status_d ? 1226 : 1225;
      break;
    case 1225:  // $E2D0
      /*$E2D0*/ tmp6_U16 = (s_a + ram_peek(0x0064)) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1227;
      break;
    case 1226:  // $E2D0
      /*$E2D0*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x0064), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1227;
      break;
    case 1227:  // $E2D2
      /*$E2D2*/ s_x = s_a;
      /*$E2D3*/ s_a = s_y;
      /*$E2D4*/ block_id = s_status_d ? 1229 : 1228;
      break;
    case 1228:  // $E2D4
      /*$E2D4*/ tmp6_U16 = s_a;
                tmp4_U16 = ram_peek(0x0065);
                tmp3_U16 = (tmp6_U16 + tmp4_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1230;
      break;
    case 1229:  // $E2D4
      /*$E2D4*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x0065), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1230;
      break;
    case 1230:  // $E2D6
      /*$E2D6*/ s_y = s_a;
      /*$E2D7*/ branchTarget = true; block_id = s_status_c ? 1195 : 1231;
      break;
    case 1231:  // $E2D9
      /*$E2D9*/ CYCLES(0xe2d9, 7);
                tmp5_U8 = (uint8_t)(ram_peek(0x0099) - 0x01);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                ram_poke(0x0099, tmp5_U8);
      /*$E2DB*/ branchTarget = true; block_id = tmp5_U8 ? 1222 : 1232;
      break;
    case 1232:  // $E2DD
      /*$E2DD*/ CYCLES(0xe2dd, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xe2dd, pop16() + 1);;
      break;
    case 1233:  // $E2F2
      /*$E2F2*/ CYCLES(0xe2f2, 23);
      /*$E2F4*/ ram_poke(0x0011, 0x00);
      /*$E2F6*/ ram_poke(0x009e, s_a);
      /*$E2F8*/ ram_poke(0x009f, s_y);
      /*$E2FA*/ s_x = 0x90;
      /*$E2FC*/ branchTarget = true; block_id = 1598;
      break;
    case 1234:  // $E301
      /*$E301*/ CYCLES(0xe301, 9);
                s_a = 0x00;
      /*$E303*/ s_status_c = 0x01;
      /*$E304*/ branchTarget = true; block_id = 1233;
      break;
    case 1235:  // $E30E
      /*$E30E*/ CYCLES(0xe30e, 9);
                s_x = 0xe0;
      /*$E310*/ branchTarget = true; block_id = 641;
      break;
    case 1236:  // $E341
      /*$E341*/ CYCLES(0xe341, 9);
                s_a = 0xc2;
      /*$E343*/ branchTarget = true; push16(0xe345); block_id = 1021;
      break;
    case 1237:  // $E346
      /*$E346*/ CYCLES(0xe346, 12);
                tmp5_U8 = s_a | 0x80;
                s_a = tmp5_U8;
      /*$E348*/ ram_poke(0x0014, tmp5_U8);
      /*$E34A*/ branchTarget = true; push16(0xe34c); block_id = 1049;
      break;
    case 1238:  // $E34D
      /*$E34D*/ CYCLES(0xe34d, 12);
                ram_poke(0x008a, s_a);
      /*$E34F*/ ram_poke(0x008b, s_y);
      /*$E351*/ branchTarget = true; block_id = 931;
      break;
    case 1239:  // $E357
      /*$E357*/ CYCLES(0xe357, 16);
      /*$E359*/ push8(ram_peek(0x008b));
      /*$E35A*/ tmp5_U8 = ram_peek(0x008a);
                s_a = tmp5_U8;
      /*$E35C*/ push8(tmp5_U8);
      /*$E35D*/ branchTarget = true; push16(0xe35f); block_id = 1016;
      break;
    case 1240:  // $E360
      /*$E360*/ CYCLES(0xe360, 6);
                branchTarget = true; push16(0xe362); block_id = 931;
      break;
    case 1241:  // $E363
      /*$E363*/ CYCLES(0xe363, 31);
                tmp5_U8 = pop8();
      /*$E364*/ ram_poke(0x008a, tmp5_U8);
      /*$E366*/ tmp5_U8 = pop8();
      /*$E367*/ ram_poke(0x008b, tmp5_U8);
      /*$E36B*/ tmp5_U8 = peek((ram_peek16al(0x008a) + 0x0002));
      /*$E36D*/ ram_poke(0x0083, tmp5_U8);
      /*$E36F*/ s_x = tmp5_U8;
      /*$E370*/ s_y = 0x03;
      /*$E371*/ tmp5_U8 = peek((ram_peek16al(0x008a) + 0x0003));
                s_a = tmp5_U8;
      /*$E373*/ branchTarget = true; block_id = !tmp5_U8 ? 1235 : 1242;
      break;
    case 1242:  // $E375
      /*$E375*/ CYCLES(0xe375, 6);
                ram_poke(0x0084, s_a);
      /*$E377*/ s_y = (uint8_t)(s_y + 0x01);
                block_id = 1243;
      break;
    case 1243:  // $E378
      /*$E378*/ CYCLES(0xe378, 11);
                tmp5_U8 = s_y;
                tmp1_U8 = peek((ram_peek16(0x0083) + tmp5_U8));
      /*$E37A*/ push8(tmp1_U8);
      /*$E37B*/ tmp5_U8 = (uint8_t)(tmp5_U8 - 0x01);
                s_y = tmp5_U8;
      /*$E37C*/ branchTarget = true; block_id = !(tmp5_U8 & 0x80) ? 1243 : 1244;
      break;
    case 1244:  // $E37E
      /*$E37E*/ CYCLES(0xe37e, 9);
                s_y = ram_peek(0x0084);
      /*$E380*/ branchTarget = true; push16(0xe382); block_id = 1579;
      break;
    case 1245:  // $E383
      /*$E383*/ CYCLES(0xe383, 41);
      /*$E385*/ push8(ram_peek(0x00b9));
      /*$E388*/ push8(ram_peek(0x00b8));
      /*$E389*/ tmp5_U8 = s_y;
                tmp1_U8 = peek((ram_peek16al(0x008a) + tmp5_U8));
      /*$E38B*/ ram_poke(0x00b8, tmp1_U8);
      /*$E38D*/ tmp5_U8 = (uint8_t)(tmp5_U8 + 0x01);
                s_y = tmp5_U8;
      /*$E38E*/ tmp5_U8 = peek((ram_peek16al(0x008a) + tmp5_U8));
      /*$E390*/ ram_poke(0x00b9, tmp5_U8);
      /*$E394*/ push8(ram_peek(0x0084));
      /*$E395*/ tmp5_U8 = ram_peek(0x0083);
                s_a = tmp5_U8;
      /*$E397*/ push8(tmp5_U8);
      /*$E398*/ branchTarget = true; push16(0xe39a); block_id = 930;
      break;
    case 1246:  // $E39B
      /*$E39B*/ CYCLES(0xe39b, 16);
                tmp5_U8 = pop8();
      /*$E39C*/ ram_poke(0x008a, tmp5_U8);
      /*$E39E*/ tmp5_U8 = pop8();
                s_status_not_z = tmp5_U8;
      /*$E39F*/ ram_poke(0x008b, tmp5_U8);
      /*$E3A1*/ branchTarget = true; push16(0xe3a3); block_id = 4;
      break;
    case 1247:  // $E3A4
      /*$E3A4*/ CYCLES(0xe3a4, 4);
                branchTarget = true; block_id = !s_status_not_z ? 1249 : 1248;
      break;
    case 1248:  // $E3A6
      /*$E3A6*/ CYCLES(0xe3a6, 6);
                branchTarget = true; block_id = 1023;
      break;
    case 1249:  // $E3A9
      /*$E3A9*/ CYCLES(0xe3a9, 48);
                tmp5_U8 = pop8();
      /*$E3AA*/ ram_poke(0x00b8, tmp5_U8);
      /*$E3AC*/ tmp5_U8 = pop8();
      /*$E3AD*/ ram_poke(0x00b9, tmp5_U8);
      /*$E3B1*/ tmp5_U8 = pop8();
      /*$E3B2*/ poke(ram_peek16al(0x008a), tmp5_U8);
      /*$E3B4*/ tmp5_U8 = pop8();
      /*$E3B6*/ poke((ram_peek16al(0x008a) + 0x0001), tmp5_U8);
      /*$E3B8*/ tmp5_U8 = pop8();
      /*$E3BA*/ poke((ram_peek16al(0x008a) + 0x0002), tmp5_U8);
      /*$E3BC*/ tmp5_U8 = pop8();
      /*$E3BE*/ poke((ram_peek16al(0x008a) + 0x0003), tmp5_U8);
      /*$E3C0*/ tmp5_U8 = pop8();
                s_a = tmp5_U8;
      /*$E3C1*/ s_status_not_z = 0x04;
                s_status_n = 0x00;
                s_y = 0x04;
      /*$E3C2*/ poke((ram_peek16al(0x008a) + 0x0004), tmp5_U8);
      /*$E3C4*/ branchTarget = true; block_id = find_block_id_func_t001(0xe3c4, pop16() + 1);;
      break;
    case 1250:  // $E3D5
      /*$E3D5*/ CYCLES(0xe3d5, 19);
                tmp1_U8 = ram_peek(0x00a0);
                s_x = tmp1_U8;
      /*$E3D7*/ tmp2_U8 = ram_peek(0x00a1);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_y = tmp2_U8;
      /*$E3D9*/ ram_poke(0x008c, tmp1_U8);
      /*$E3DB*/ ram_poke(0x008d, tmp2_U8);
      /*$E3DD*/ branchTarget = true; push16(0xe3df); block_id = 1271;
      break;
    case 1251:  // $E3E0
      /*$E3E0*/ CYCLES(0xe3e0, 12);
                ram_poke(0x009e, s_x);
      /*$E3E2*/ ram_poke(0x009f, s_y);
      /*$E3E4*/ ram_poke(0x009d, s_a);
      /*$E3E6*/ branchTarget = true; block_id = find_block_id_func_t001(0xe3e6, pop16() + 1);;
      break;
    case 1252:  // $E3E7
      /*$E3E7*/ CYCLES(0xe3e7, 28);
      /*$E3E9*/ ram_poke(0x000d, 0x22);
      /*$E3EB*/ ram_poke(0x000e, 0x22);
      /*$E3ED*/ tmp5_U8 = s_a;
                ram_poke(0x00ab, tmp5_U8);
      /*$E3EF*/ tmp1_U8 = s_y;
                ram_poke(0x00ac, tmp1_U8);
      /*$E3F1*/ ram_poke(0x009e, tmp5_U8);
      /*$E3F3*/ ram_poke(0x009f, tmp1_U8);
      /*$E3F5*/ s_y = 0xff;
                block_id = 1253;
      break;
    case 1253:  // $E3F7
      /*$E3F7*/ CYCLES(0xe3f7, 9);
                tmp5_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp5_U8;
      /*$E3F8*/ tmp5_U8 = peek((ram_peek16(0x00ab) + tmp5_U8));
                s_a = tmp5_U8;
      /*$E3FA*/ branchTarget = true; block_id = !tmp5_U8 ? 1257 : 1254;
      break;
    case 1254:  // $E3FC
      /*$E3FC*/ CYCLES(0xe3fc, 7);
      /*$E3FE*/ branchTarget = true; block_id = !(s_a != ram_peek(0x000d)) ? 1256 : 1255;
      break;
    case 1255:  // $E400
      /*$E400*/ CYCLES(0xe400, 7);
      /*$E402*/ branchTarget = true; block_id = (s_a != ram_peek(0x000e)) ? 1253 : 1256;
      break;
    case 1256:  // $E404
      /*$E404*/ CYCLES(0xe404, 7);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0x22);
      /*$E406*/ branchTarget = true; block_id = !(tmp5_U8 != 0x22) ? 1258 : 1257;
      break;
    case 1257:  // $E408
      /*$E408*/ CYCLES(0xe408, 2);
                s_status_c = 0x00;
                block_id = 1258;
      break;
    case 1258:  // $E409
      /*$E409*/ CYCLES(0xe409, 19);
                tmp5_U8 = s_y;
                ram_poke(0x009d, tmp5_U8);
      /*$E40B*/ s_a = tmp5_U8;
      /*$E40C*/ block_id = s_status_d ? 1260 : 1259;
      break;
    case 1259:  // $E40C
      /*$E40C*/ tmp3_U16 = s_a;
                tmp4_U16 = ram_peek(0x00ab);
                tmp6_U16 = (tmp3_U16 + tmp4_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1261;
      break;
    case 1260:  // $E40C
      /*$E40C*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x00ab), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1261;
      break;
    case 1261:  // $E40E
      /*$E40E*/ ram_poke(0x00ad, s_a);
      /*$E410*/ s_x = ram_peek(0x00ac);
      /*$E412*/ branchTarget = true; block_id = !s_status_c ? 1263 : 1262;
      break;
    case 1262:  // $E414
      /*$E414*/ CYCLES(0xe414, 2);
                s_x = (uint8_t)(s_x + 0x01);
                block_id = 1263;
      break;
    case 1263:  // $E415
      /*$E415*/ CYCLES(0xe415, 11);
                ram_poke(0x00ae, s_x);
      /*$E417*/ tmp5_U8 = ram_peek(0x00ac);
                s_a = tmp5_U8;
      /*$E419*/ branchTarget = true; block_id = !tmp5_U8 ? 1265 : 1264;
      break;
    case 1264:  // $E41B
      /*$E41B*/ CYCLES(0xe41b, 7);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0x02);
      /*$E41D*/ branchTarget = true; block_id = (tmp5_U8 != 0x02) ? 1267 : 1265;
      break;
    case 1265:  // $E41F
      /*$E41F*/ CYCLES(0xe41f, 7);
                s_a = s_y;
      /*$E420*/ branchTarget = true; push16(0xe422); block_id = 1250;
      break;
    case 1266:  // $E423
      /*$E423*/ CYCLES(0xe423, 12);
                s_x = ram_peek(0x00ab);
      /*$E425*/ s_y = ram_peek(0x00ac);
      /*$E427*/ FUNC_MOVSTR(0xfffe);
                branchTarget = true; block_id = 1267;
      break;
    case 1267:  // $E42A
      /*$E42A*/ CYCLES(0xe42a, 11);
                tmp5_U8 = ram_peek(0x0052);
                s_x = tmp5_U8;
      /*$E42C*/ s_status_c = (tmp5_U8 >= 0x5e);
      /*$E42E*/ branchTarget = true; block_id = (tmp5_U8 != 0x5e) ? 1270 : 1268;
      break;
    case 1268:  // $E430
      /*$E430*/ CYCLES(0xe430, 4);
                s_x = 0xbf;
                block_id = 1269;
      break;
    case 1269:  // $E432
      /*$E432*/ CYCLES(0xe432, 6);
                branchTarget = true; block_id = 641;
      break;
    case 1270:  // $E435
      /*$E435*/ CYCLES(0xe435, 50);
      /*$E437*/ tmp5_U8 = s_x;
                ram_poke(tmp5_U8, ram_peek(0x009d));
      /*$E43B*/ ram_poke((uint8_t)(0x01 + tmp5_U8), ram_peek(0x009e));
      /*$E43D*/ tmp1_U8 = ram_peek(0x009f);
                s_a = tmp1_U8;
      /*$E43F*/ ram_poke((uint8_t)(0x02 + tmp5_U8), tmp1_U8);
      /*$E443*/ ram_poke(0x00a0, tmp5_U8);
      /*$E445*/ ram_poke(0x00a1, 0x00);
      /*$E447*/ s_y = 0xff;
      /*$E448*/ ram_poke(0x0011, 0xff);
      /*$E44A*/ ram_poke(0x0053, tmp5_U8);
      /*$E44E*/ tmp5_U8 = (uint8_t)(tmp5_U8 + 0x03);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_x = tmp5_U8;
      /*$E44F*/ ram_poke(0x0052, tmp5_U8);
      /*$E451*/ branchTarget = true; block_id = find_block_id_func_t001(0xe451, pop16() + 1);;
      break;
    case 1271:  // $E452
      /*$E452*/ CYCLES(0xe452, 4);
                ram_poke(0x0013, (ram_peek(0x0013) >> 0x01));
                block_id = 1272;
      break;
    case 1272:  // $E454
      /*$E454*/ CYCLES(0xe454, 18);
                tmp5_U8 = s_a;
                push8(tmp5_U8);
      /*$E455*/ s_a = (tmp5_U8 ^ 0xff);
      /*$E457*/ s_status_c = 0x01;
      /*$E458*/ block_id = s_status_d ? 1274 : 1273;
      break;
    case 1273:  // $E458
      /*$E458*/ tmp6_U16 = s_a;
                tmp4_U16 = ram_peek(0x006f);
                tmp3_U16 = (tmp6_U16 + tmp4_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1275;
      break;
    case 1274:  // $E458
      /*$E458*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x006f), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1275;
      break;
    case 1275:  // $E45A
      /*$E45A*/ s_y = ram_peek(0x0070);
      /*$E45C*/ branchTarget = true; block_id = s_status_c ? 1277 : 1276;
      break;
    case 1276:  // $E45E
      /*$E45E*/ CYCLES(0xe45e, 2);
                s_y = (uint8_t)(s_y - 0x01);
                block_id = 1277;
      break;
    case 1277:  // $E45F
      /*$E45F*/ CYCLES(0xe45f, 7);
                tmp5_U8 = s_y;
                tmp1_U8 = ram_peek(0x006e);
                s_status_not_z = (tmp5_U8 != tmp1_U8);
                tmp1_U8 = tmp5_U8 >= tmp1_U8;
                s_status_c = tmp1_U8;
      /*$E461*/ branchTarget = true; block_id = !tmp1_U8 ? 1281 : 1278;
      break;
    case 1278:  // $E463
      /*$E463*/ CYCLES(0xe463, 4);
                branchTarget = true; block_id = s_status_not_z ? 1280 : 1279;
      break;
    case 1279:  // $E465
      /*$E465*/ CYCLES(0xe465, 7);
                tmp1_U8 = s_a >= ram_peek(0x006d);
                s_status_c = tmp1_U8;
      /*$E467*/ branchTarget = true; block_id = !tmp1_U8 ? 1281 : 1280;
      break;
    case 1280:  // $E469
      /*$E469*/ CYCLES(0xe469, 19);
                tmp1_U8 = s_a;
                ram_poke(0x006f, tmp1_U8);
      /*$E46B*/ tmp5_U8 = s_y;
                ram_poke(0x0070, tmp5_U8);
      /*$E46D*/ ram_poke(0x0071, tmp1_U8);
      /*$E46F*/ ram_poke(0x0072, tmp5_U8);
      /*$E471*/ s_x = tmp1_U8;
      /*$E472*/ tmp1_U8 = pop8();
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$E473*/ branchTarget = true; block_id = find_block_id_func_t001(0xe473, pop16() + 1);;
      break;
    case 1281:  // $E474
      /*$E474*/ CYCLES(0xe474, 11);
                s_x = 0x4d;
      /*$E476*/ tmp1_U8 = ram_peek(0x0013);
                s_a = tmp1_U8;
      /*$E478*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 1269 : 1282;
      break;
    case 1282:  // $E47A
      /*$E47A*/ CYCLES(0xe47a, 6);
                FUNC_GARBAGE(0xfffe);
      /*$E47D*/ CYCLES(0xe47d, 12);
      /*$E47F*/ ram_poke(0x0013, 0x80);
      /*$E481*/ tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$E482*/ branchTarget = true; block_id = tmp1_U8 ? 1272 : 1937;
      break;
    case 1283:  // $E484
      /*$E484*/ CYCLES(0xe484, 7);
                s_x = ram_peek(0x0073);
      /*$E486*/ s_a = ram_peek(0x0074);
                block_id = 1284;
      break;
    case 1284:  // $E488
      /*$E488*/ CYCLES(0xe488, 41);
                ram_poke(0x006f, s_x);
      /*$E48A*/ ram_poke(0x0070, s_a);
      /*$E48C*/ s_y = 0x00;
      /*$E48E*/ ram_poke(0x008b, 0x00);
      /*$E494*/ ram_poke(0x009b, ram_peek(0x006d));
      /*$E496*/ ram_poke(0x009c, ram_peek(0x006e));
      /*$E498*/ s_a = 0x55;
      /*$E49C*/ ram_poke(0x005e, 0x55);
      /*$E49E*/ ram_poke(0x005f, 0x00);
                block_id = 1285;
      break;
    case 1285:  // $E4A0
      /*$E4A0*/ CYCLES(0xe4a0, 7);
      /*$E4A2*/ branchTarget = true; block_id = !(s_a != ram_peek(0x0052)) ? 1287 : 1286;
      break;
    case 1286:  // $E4A4
      /*$E4A4*/ CYCLES(0xe4a4, 6);
                FUNC_CHKVAR(0xfffe);
      /*$E4A7*/ CYCLES(0xe4a7, 4);
                branchTarget = true; block_id = !s_status_not_z ? 1285 : 1287;
      break;
    case 1287:  // $E4A9
      /*$E4A9*/ CYCLES(0xe4a9, 21);
      /*$E4AB*/ ram_poke(0x008f, 0x07);
      /*$E4AD*/ tmp1_U8 = ram_peek(0x0069);
                s_a = tmp1_U8;
      /*$E4AF*/ tmp5_U8 = ram_peek(0x006a);
                s_x = tmp5_U8;
      /*$E4B1*/ ram_poke(0x005e, tmp1_U8);
      /*$E4B3*/ ram_poke(0x005f, tmp5_U8);
                block_id = 1288;
      break;
    case 1288:  // $E4B5
      /*$E4B5*/ CYCLES(0xe4b5, 7);
      /*$E4B7*/ branchTarget = true; block_id = (s_x != ram_peek(0x006c)) ? 1290 : 1289;
      break;
    case 1289:  // $E4B9
      /*$E4B9*/ CYCLES(0xe4b9, 7);
      /*$E4BB*/ branchTarget = true; block_id = !(s_a != ram_peek(0x006b)) ? 1291 : 1290;
      break;
    case 1290:  // $E4BD
      /*$E4BD*/ CYCLES(0xe4bd, 6);
                FUNC_CHKSMPLVAR(0xfffe);
      /*$E4C0*/ CYCLES(0xe4c0, 4);
                branchTarget = true; block_id = !s_status_not_z ? 1288 : 1291;
      break;
    case 1291:  // $E4C2
      /*$E4C2*/ CYCLES(0xe4c2, 14);
                ram_poke(0x0094, s_a);
      /*$E4C4*/ ram_poke(0x0095, s_x);
      /*$E4C8*/ ram_poke(0x008f, 0x03);
                block_id = 1292;
      break;
    case 1292:  // $E4CA
      /*$E4CA*/ CYCLES(0xe4ca, 7);
                s_a = ram_peek(0x0094);
      /*$E4CC*/ s_x = ram_peek(0x0095);
                block_id = 1293;
      break;
    case 1293:  // $E4CE
      /*$E4CE*/ CYCLES(0xe4ce, 7);
                tmp5_U8 = s_x;
                tmp1_U8 = ram_peek(0x006e);
                s_status_c = (tmp5_U8 >= tmp1_U8);
      /*$E4D0*/ branchTarget = true; block_id = (tmp5_U8 != tmp1_U8) ? 1296 : 1294;
      break;
    case 1294:  // $E4D2
      /*$E4D2*/ CYCLES(0xe4d2, 7);
                tmp5_U8 = s_a;
                tmp1_U8 = ram_peek(0x006d);
                s_status_c = (tmp5_U8 >= tmp1_U8);
      /*$E4D4*/ branchTarget = true; block_id = (tmp5_U8 != tmp1_U8) ? 1296 : 1295;
      break;
    case 1295:  // $E4D6
      /*$E4D6*/ CYCLES(0xe4d6, 6);
      /*$E562*/ CYCLES(0xe562, 7);
      /*$E564*/ branchTarget = true; block_id = !ram_peek(0x008b) ? 1332 : 1333;
      break;
    case 1296:  // $E4D9
      /*$E4D9*/ CYCLES(0xe4d9, 52);
                ram_poke(0x005e, s_a);
      /*$E4DB*/ ram_poke(0x005f, s_x);
      /*$E4DF*/ tmp1_U8 = peek(ram_peek16al(0x005e));
      /*$E4E1*/ s_x = tmp1_U8;
      /*$E4E3*/ tmp1_U8 = peek((ram_peek16al(0x005e) + 0x0001));
      /*$E4E5*/ tmp5_U8 = s_status_d;
                push8((s_status_c | ((tmp1_U8 == 0) << 1) | (s_status_i << 2) | (tmp5_U8 << 3) | STATUS_B | (s_status_v << 6) | (tmp1_U8 & 0x80)));
      /*$E4E6*/ s_y = 0x02;
      /*$E4E7*/ tmp1_U8 = peek((ram_peek16al(0x005e) + 0x0002));
                s_a = tmp1_U8;
      /*$E4E9*/ block_id = tmp5_U8 ? 1298 : 1297;
      break;
    case 1297:  // $E4E9
      /*$E4E9*/ tmp3_U16 = (s_a + ram_peek(0x0094)) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1299;
      break;
    case 1298:  // $E4E9
      /*$E4E9*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x0094), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 1299;
      break;
    case 1299:  // $E4EB
      /*$E4EB*/ ram_poke(0x0094, s_a);
      /*$E4ED*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$E4EE*/ tmp1_U8 = peek((ram_peek16al(0x005e) + tmp1_U8));
                s_a = tmp1_U8;
      /*$E4F0*/ block_id = s_status_d ? 1301 : 1300;
      break;
    case 1300:  // $E4F0
      /*$E4F0*/ s_a = (uint8_t)((s_a + ram_peek(0x0095)) + s_status_c);
                block_id = 1302;
      break;
    case 1301:  // $E4F0
      /*$E4F0*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x0095), s_status_c));
                block_id = 1302;
      break;
    case 1302:  // $E4F2
      /*$E4F2*/ ram_poke(0x0095, s_a);
      /*$E4F4*/ tmp1_U8 = pop8();
                s_status_i = ((tmp1_U8 & 0x04) != 0);
                s_status_d = ((tmp1_U8 & 0x08) != 0);
                s_status_b = 0x00;
                s_status_v = ((tmp1_U8 & 0x40) != 0);
      /*$E4F5*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 1292 : 1303;
      break;
    case 1303:  // $E4F7
      /*$E4F7*/ CYCLES(0xe4f7, 6);
      /*$E4F8*/ branchTarget = true; block_id = (s_x & 0x80) ? 1292 : 1304;
      break;
    case 1304:  // $E4FA
      /*$E4FA*/ CYCLES(0xe4fa, 24);
      /*$E4FB*/ tmp1_U8 = peek((ram_peek16al(0x005e) + (uint8_t)(s_y + 0x01)));
      /*$E4FD*/ s_y = 0x00;
      /*$E4FF*/ tmp3_U16 = tmp1_U8 << 0x01;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
      /*$E500*/ block_id = s_status_d ? 1306 : 1305;
      break;
    case 1305:  // $E500
      /*$E500*/ tmp3_U16 = (s_a + 0x0005) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1307;
      break;
    case 1306:  // $E500
      /*$E500*/ tmp3_U16 = adc_dec16(s_a, 0x05, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 1307;
      break;
    case 1307:  // $E502
      /*$E502*/ block_id = s_status_d ? 1309 : 1308;
      break;
    case 1308:  // $E502
      /*$E502*/ tmp3_U16 = s_a;
                tmp4_U16 = ram_peek(0x005e);
                tmp6_U16 = (tmp3_U16 + tmp4_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1310;
      break;
    case 1309:  // $E502
      /*$E502*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x005e), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 1310;
      break;
    case 1310:  // $E504
      /*$E504*/ ram_poke(0x005e, s_a);
      /*$E506*/ branchTarget = true; block_id = !s_status_c ? 1312 : 1311;
      break;
    case 1311:  // $E508
      /*$E508*/ CYCLES(0xe508, 4);
                ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) + 0x01));
                block_id = 1312;
      break;
    case 1312:  // $E50A
      /*$E50A*/ CYCLES(0xe50a, 4);
                s_x = ram_peek(0x005f);
                block_id = 1313;
      break;
    case 1313:  // $E50C
      /*$E50C*/ CYCLES(0xe50c, 7);
      /*$E50E*/ branchTarget = true; block_id = (s_x != ram_peek(0x0095)) ? 1315 : 1314;
      break;
    case 1314:  // $E510
      /*$E510*/ CYCLES(0xe510, 7);
      /*$E512*/ branchTarget = true; block_id = !(s_a != ram_peek(0x0094)) ? 1293 : 1315;
      break;
    case 1315:  // $E514
      /*$E514*/ CYCLES(0xe514, 6);
                FUNC_CHKVAR(0xfffe);
      /*$E517*/ CYCLES(0xe517, 4);
                branchTarget = true; block_id = !s_status_not_z ? 1313 : 1938;
      break;
    case 1316:  // $E519
      /*$E519*/ CYCLES(0xe519, 7);
                tmp5_U8 = peek((ram_peek16al(0x005e) + s_y));
      /*$E51B*/ branchTarget = true; block_id = (tmp5_U8 & 0x80) ? 1327 : 1317;
      break;
    case 1317:  // $E51D
      /*$E51D*/ CYCLES(0xe51d, 9);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$E51E*/ tmp1_U8 = peek((ram_peek16al(0x005e) + tmp1_U8));
      /*$E520*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 1327 : 1318;
      break;
    case 1318:  // $E522
      /*$E522*/ CYCLES(0xe522, 2);
                s_y = (uint8_t)(s_y + 0x01);
                FUNC_CHKVAR(0x0000);
                block_id = find_block_id_func_t001(0xe522, pop16() + 1);;
      break;
    case 1319:  // $E523
      /*$E523*/ CYCLES(0xe523, 7);
                tmp5_U8 = peek((ram_peek16al(0x005e) + s_y));
      /*$E525*/ branchTarget = true; block_id = !tmp5_U8 ? 1327 : 1320;
      break;
    case 1320:  // $E527
      /*$E527*/ CYCLES(0xe527, 19);
                tmp1_U8 = s_y;
      /*$E528*/ tmp5_U8 = peek((ram_peek16al(0x005e) + (uint8_t)(tmp1_U8 + 0x01)));
      /*$E52A*/ s_x = tmp5_U8;
      /*$E52C*/ tmp1_U8 = peek((ram_peek16al(0x005e) + (uint8_t)(tmp1_U8 + 0x02)));
                s_a = tmp1_U8;
      /*$E52E*/ tmp5_U8 = ram_peek(0x0070);
                s_status_not_z = (tmp1_U8 != tmp5_U8);
      /*$E530*/ branchTarget = true; block_id = !(tmp1_U8 >= tmp5_U8) ? 1323 : 1321;
      break;
    case 1321:  // $E532
      /*$E532*/ CYCLES(0xe532, 4);
                branchTarget = true; block_id = s_status_not_z ? 1327 : 1322;
      break;
    case 1322:  // $E534
      /*$E534*/ CYCLES(0xe534, 7);
      /*$E536*/ branchTarget = true; block_id = (s_x >= ram_peek(0x006f)) ? 1327 : 1323;
      break;
    case 1323:  // $E538
      /*$E538*/ CYCLES(0xe538, 7);
                tmp5_U8 = s_a;
                tmp1_U8 = ram_peek(0x009c);
                s_status_not_z = (tmp5_U8 != tmp1_U8);
      /*$E53A*/ branchTarget = true; block_id = !(tmp5_U8 >= tmp1_U8) ? 1327 : 1324;
      break;
    case 1324:  // $E53C
      /*$E53C*/ CYCLES(0xe53c, 4);
                branchTarget = true; block_id = s_status_not_z ? 1326 : 1325;
      break;
    case 1325:  // $E53E
      /*$E53E*/ CYCLES(0xe53e, 7);
      /*$E540*/ branchTarget = true; block_id = !(s_x >= ram_peek(0x009b)) ? 1327 : 1326;
      break;
    case 1326:  // $E542
      /*$E542*/ CYCLES(0xe542, 28);
                ram_poke(0x009b, s_x);
      /*$E544*/ ram_poke(0x009c, s_a);
      /*$E54A*/ ram_poke(0x008a, ram_peek(0x005e));
      /*$E54C*/ ram_poke(0x008b, ram_peek(0x005f));
      /*$E550*/ ram_poke(0x0091, ram_peek(0x008f));
                block_id = 1327;
      break;
    case 1327:  // $E552
      /*$E552*/ CYCLES(0xe552, 16);
                s_a = ram_peek(0x008f);
      /*$E554*/ s_status_c = 0x00;
      /*$E555*/ block_id = s_status_d ? 1329 : 1328;
      break;
    case 1328:  // $E555
      /*$E555*/ tmp6_U16 = s_a;
                tmp4_U16 = ram_peek(0x005e);
                tmp3_U16 = (tmp6_U16 + tmp4_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1330;
      break;
    case 1329:  // $E555
      /*$E555*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x005e), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 1330;
      break;
    case 1330:  // $E557
      /*$E557*/ ram_poke(0x005e, s_a);
      /*$E559*/ branchTarget = true; block_id = !s_status_c ? 1332 : 1331;
      break;
    case 1331:  // $E55B
      /*$E55B*/ CYCLES(0xe55b, 4);
                ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) + 0x01));
                block_id = 1332;
      break;
    case 1332:  // $E55D
      /*$E55D*/ CYCLES(0xe55d, 9);
                s_x = ram_peek(0x005f);
      /*$E55F*/ s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_y = 0x00;
      /*$E561*/ branchTarget = true; block_id = find_block_id_func_t001(0xe561, pop16() + 1);;
      break;
    case 1333:  // $E566
      /*$E566*/ CYCLES(0xe566, 53);
      /*$E56A*/ s_status_c = 0x00;
                tmp1_U8 = (ram_peek(0x0091) & 0x04) >> 0x01;
      /*$E56C*/ ram_poke(0x0091, tmp1_U8);
      /*$E56E*/ tmp1_U8 = peek((ram_peek16al(0x008a) + tmp1_U8));
                s_a = tmp1_U8;
      /*$E570*/ block_id = s_status_d ? 1335 : 1334;
      break;
    case 1334:  // $E570
      /*$E570*/ tmp3_U16 = (s_a + ram_peek(0x009b)) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1336;
      break;
    case 1335:  // $E570
      /*$E570*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x009b), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 1336;
      break;
    case 1336:  // $E572
      /*$E572*/ ram_poke(0x0096, s_a);
      /*$E574*/ s_a = ram_peek(0x009c);
      /*$E576*/ block_id = s_status_d ? 1338 : 1337;
      break;
    case 1337:  // $E576
      /*$E576*/ tmp3_U16 = s_a;
                tmp4_U16 = tmp3_U16 + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1339;
      break;
    case 1338:  // $E576
      /*$E576*/ tmp4_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 1339;
      break;
    case 1339:  // $E578
      /*$E578*/ ram_poke(0x0097, s_a);
      /*$E57E*/ ram_poke(0x0094, ram_peek(0x006f));
      /*$E580*/ ram_poke(0x0095, ram_peek(0x0070));
      /*$E582*/ FUNC_MVBLKUP2(0xfffe);
      /*$E585*/ CYCLES(0xe585, 31);
                tmp1_U8 = ram_peek(0x0091);
      /*$E588*/ tmp5_U8 = ram_peek(0x0094);
      /*$E58A*/ poke((ram_peek16al(0x008a) + (uint8_t)(tmp1_U8 + 0x01)), tmp5_U8);
      /*$E58C*/ s_x = tmp5_U8;
      /*$E58D*/ ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) + 0x01));
      /*$E58F*/ tmp5_U8 = ram_peek(0x0095);
                s_a = tmp5_U8;
      /*$E592*/ poke((ram_peek16al(0x008a) + (uint8_t)(tmp1_U8 + 0x02)), tmp5_U8);
      /*$E594*/ branchTarget = true; block_id = 1284;
      break;
    case 1340:  // $E5A0
      /*$E5A0*/ CYCLES(0xe5a0, 6);
                branchTarget = true; push16(0xe5a2); block_id = 932;
      break;
    case 1341:  // $E5A3
      /*$E5A3*/ CYCLES(0xe5a3, 26);
                tmp1_U8 = pop8();
      /*$E5A4*/ ram_poke(0x00ab, tmp1_U8);
      /*$E5A6*/ tmp1_U8 = pop8();
      /*$E5A7*/ ram_poke(0x00ac, tmp1_U8);
      /*$E5A9*/ s_y = 0x00;
      /*$E5AB*/ tmp1_U8 = peek(ram_peek16(0x00ab));
                s_a = tmp1_U8;
      /*$E5AD*/ s_status_c = 0x00;
      /*$E5AE*/ block_id = s_status_d ? 1343 : 1342;
      break;
    case 1342:  // $E5AE
      /*$E5AE*/ tmp4_U16 = s_a;
                tmp1_U8 = peek((ram_peek16al(0x00a0) + s_y));
                tmp3_U16 = tmp1_U8;
                tmp6_U16 = (tmp4_U16 + tmp3_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)tmp3_U16);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1344;
      break;
    case 1343:  // $E5AE
      /*$E5AE*/ tmp1_U8 = peek((ram_peek16al(0x00a0) + s_y));
                tmp6_U16 = adc_dec16(s_a, tmp1_U8, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 1344;
      break;
    case 1344:  // $E5B0
      /*$E5B0*/ branchTarget = true; block_id = !s_status_c ? 1346 : 1345;
      break;
    case 1345:  // $E5B2
      /*$E5B2*/ CYCLES(0xe5b2, 9);
                s_x = 0xb0;
      /*$E5B4*/ branchTarget = true; block_id = 641;
      break;
    case 1346:  // $E5B7
      /*$E5B7*/ CYCLES(0xe5b7, 6);
                branchTarget = true; push16(0xe5b9); block_id = 1250;
      break;
    case 1347:  // $E5BA
      /*$E5BA*/ CYCLES(0xe5ba, 6);
                FUNC_MOVINS(0xfffe);
      /*$E5BD*/ CYCLES(0xe5bd, 12);
                s_a = ram_peek(0x008c);
      /*$E5BF*/ s_y = ram_peek(0x008d);
      /*$E5C1*/ FUNC_FRETMP(0xfffe);
      /*$E5C4*/ CYCLES(0xe5c4, 6);
                FUNC_MOVSTR1(0xfffe);
      /*$E5C7*/ CYCLES(0xe5c7, 12);
                s_a = ram_peek(0x00ab);
      /*$E5C9*/ s_y = ram_peek(0x00ac);
      /*$E5CB*/ FUNC_FRETMP(0xfffe);
      /*$E5CE*/ CYCLES(0xe5ce, 6);
                branchTarget = true; push16(0xe5d0); block_id = 1267;
      break;
    case 1348:  // $E5D1
      /*$E5D1*/ CYCLES(0xe5d1, 6);
                branchTarget = true; block_id = 943;
      break;
    case 1349:  // $E5D4
      /*$E5D4*/ CYCLES(0xe5d4, 24);
      /*$E5D6*/ tmp2_U8 = peek(ram_peek16(0x00ab));
      /*$E5D8*/ push8(tmp2_U8);
      /*$E5DA*/ tmp2_U8 = peek((ram_peek16(0x00ab) + 0x0001));
      /*$E5DC*/ s_x = tmp2_U8;
      /*$E5DE*/ tmp2_U8 = peek((ram_peek16(0x00ab) + 0x0002));
      /*$E5E0*/ s_y = tmp2_U8;
      /*$E5E1*/ tmp2_U8 = pop8();
                s_a = tmp2_U8;
                FUNC_MOVSTR(0x0000);
                block_id = find_block_id_func_t001(0xe5e1, pop16() + 1);;
      break;
    case 1350:  // $E5E2
      /*$E5E2*/ CYCLES(0xe5e2, 7);
                ram_poke(0x005e, s_x);
      /*$E5E4*/ ram_poke(0x005f, s_y);
                FUNC_MOVSTR1(0x0000);
                block_id = find_block_id_func_t001(0xe5e4, pop16() + 1);;
      break;
    case 1351:  // $E5E6
      /*$E5E6*/ CYCLES(0xe5e6, 6);
                tmp5_U8 = s_a;
                s_y = tmp5_U8;
      /*$E5E7*/ branchTarget = true; block_id = !tmp5_U8 ? 1355 : 1352;
      break;
    case 1352:  // $E5E9
      /*$E5E9*/ CYCLES(0xe5e9, 2);
                push8(s_a);
                block_id = 1353;
      break;
    case 1353:  // $E5EA
      /*$E5EA*/ CYCLES(0xe5ea, 14);
                tmp5_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp5_U8;
      /*$E5EB*/ tmp1_U8 = peek((ram_peek16al(0x005e) + tmp5_U8));
      /*$E5ED*/ poke((ram_peek16(0x0071) + tmp5_U8), tmp1_U8);
      /*$E5F0*/ branchTarget = true; block_id = tmp5_U8 ? 1353 : 1354;
      break;
    case 1354:  // $E5F2
      /*$E5F2*/ CYCLES(0xe5f2, 2);
                tmp5_U8 = pop8();
                s_a = tmp5_U8;
                block_id = 1355;
      break;
    case 1355:  // $E5F3
      /*$E5F3*/ CYCLES(0xe5f3, 12);
                s_status_c = 0x00;
      /*$E5F4*/ block_id = s_status_d ? 1357 : 1356;
      break;
    case 1356:  // $E5F4
      /*$E5F4*/ tmp6_U16 = s_a;
                tmp3_U16 = ram_peek(0x0071);
                tmp4_U16 = (tmp6_U16 + tmp3_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
                block_id = 1358;
      break;
    case 1357:  // $E5F4
      /*$E5F4*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x0071), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_not_z = (~tmp5_U8 & 2);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                s_status_n = (tmp5_U8 & 0x80);
                block_id = 1358;
      break;
    case 1358:  // $E5F6
      /*$E5F6*/ ram_poke(0x0071, s_a);
      /*$E5F8*/ branchTarget = true; block_id = !s_status_c ? 1360 : 1359;
      break;
    case 1359:  // $E5FA
      /*$E5FA*/ CYCLES(0xe5fa, 4);
                tmp5_U8 = (uint8_t)(ram_peek(0x0072) + 0x01);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                ram_poke(0x0072, tmp5_U8);
                block_id = 1360;
      break;
    case 1360:  // $E5FC
      /*$E5FC*/ CYCLES(0xe5fc, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xe5fc, pop16() + 1);;
      break;
    case 1361:  // $E600
      /*$E600*/ CYCLES(0xe600, 7);
                s_a = ram_peek(0x00a0);
      /*$E602*/ s_y = ram_peek(0x00a1);
                FUNC_FRETMP(0x0000);
                block_id = find_block_id_func_t001(0xe602, pop16() + 1);;
      break;
    case 1362:  // $E604
      /*$E604*/ CYCLES(0xe604, 12);
                ram_poke(0x005e, s_a);
      /*$E606*/ ram_poke(0x005f, s_y);
      /*$E608*/ FUNC_FRETMS(0xfffe);
      /*$E60B*/ CYCLES(0xe60b, 31);
                push8((s_status_c | ((s_status_not_z == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | s_status_n));
      /*$E60E*/ tmp1_U8 = peek(ram_peek16al(0x005e));
      /*$E610*/ push8(tmp1_U8);
      /*$E612*/ tmp1_U8 = peek((ram_peek16al(0x005e) + 0x0001));
      /*$E614*/ s_x = tmp1_U8;
      /*$E616*/ tmp1_U8 = peek((ram_peek16al(0x005e) + 0x0002));
      /*$E618*/ s_y = tmp1_U8;
      /*$E619*/ tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$E61A*/ tmp1_U8 = pop8();
                s_status_c = (tmp1_U8 & 0x01);
                tmp5_U8 = (~tmp1_U8 & 2);
                s_status_not_z = tmp5_U8;
                s_status_i = ((tmp1_U8 & 0x04) != 0);
                s_status_d = ((tmp1_U8 & 0x08) != 0);
                s_status_b = 0x00;
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
      /*$E61B*/ branchTarget = true; block_id = tmp5_U8 ? 1371 : 1363;
      break;
    case 1363:  // $E61D
      /*$E61D*/ CYCLES(0xe61d, 7);
                tmp5_U8 = s_y;
                tmp1_U8 = ram_peek(0x0070);
                tmp2_U8 = tmp5_U8 != tmp1_U8;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp5_U8 >= tmp1_U8);
                s_status_n = ((uint8_t)(tmp5_U8 - tmp1_U8) & 0x80);
      /*$E61F*/ branchTarget = true; block_id = tmp2_U8 ? 1371 : 1364;
      break;
    case 1364:  // $E621
      /*$E621*/ CYCLES(0xe621, 7);
                tmp2_U8 = s_x;
                tmp1_U8 = ram_peek(0x006f);
                tmp5_U8 = tmp2_U8 != tmp1_U8;
                s_status_not_z = tmp5_U8;
                s_status_c = (tmp2_U8 >= tmp1_U8);
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
      /*$E623*/ branchTarget = true; block_id = tmp5_U8 ? 1371 : 1365;
      break;
    case 1365:  // $E625
      /*$E625*/ CYCLES(0xe625, 14);
                push8(s_a);
      /*$E626*/ s_status_c = 0x00;
      /*$E627*/ block_id = s_status_d ? 1367 : 1366;
      break;
    case 1366:  // $E627
      /*$E627*/ tmp4_U16 = s_a;
                tmp3_U16 = ram_peek(0x006f);
                tmp6_U16 = (tmp4_U16 + tmp3_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)tmp3_U16);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1368;
      break;
    case 1367:  // $E627
      /*$E627*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x006f), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1368;
      break;
    case 1368:  // $E629
      /*$E629*/ ram_poke(0x006f, s_a);
      /*$E62B*/ branchTarget = true; block_id = !s_status_c ? 1370 : 1369;
      break;
    case 1369:  // $E62D
      /*$E62D*/ CYCLES(0xe62d, 4);
                ram_poke(0x0070, (uint8_t)(ram_peek(0x0070) + 0x01));
                block_id = 1370;
      break;
    case 1370:  // $E62F
      /*$E62F*/ CYCLES(0xe62f, 2);
                tmp5_U8 = pop8();
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
                block_id = 1371;
      break;
    case 1371:  // $E630
      /*$E630*/ CYCLES(0xe630, 9);
                ram_poke(0x005e, s_x);
      /*$E632*/ ram_poke(0x005f, s_y);
      /*$E634*/ branchTarget = true; block_id = find_block_id_func_t001(0xe634, pop16() + 1);;
      break;
    case 1372:  // $E635
      /*$E635*/ CYCLES(0xe635, 7);
                tmp2_U8 = s_y;
                tmp1_U8 = ram_peek(0x0054);
                tmp5_U8 = tmp2_U8 != tmp1_U8;
                s_status_not_z = tmp5_U8;
                s_status_c = (tmp2_U8 >= tmp1_U8);
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
      /*$E637*/ branchTarget = true; block_id = tmp5_U8 ? 1378 : 1373;
      break;
    case 1373:  // $E639
      /*$E639*/ CYCLES(0xe639, 7);
                tmp5_U8 = s_a;
                tmp1_U8 = ram_peek(0x0053);
                tmp2_U8 = tmp5_U8 != tmp1_U8;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp5_U8 >= tmp1_U8);
                s_status_n = ((uint8_t)(tmp5_U8 - tmp1_U8) & 0x80);
      /*$E63B*/ branchTarget = true; block_id = tmp2_U8 ? 1378 : 1374;
      break;
    case 1374:  // $E63D
      /*$E63D*/ CYCLES(0xe63d, 14);
                ram_poke(0x0052, s_a);
      /*$E63F*/ block_id = s_status_d ? 1376 : 1375;
      break;
    case 1375:  // $E63F
      /*$E63F*/ tmp6_U16 = s_a;
                tmp3_U16 = (tmp6_U16 - 0x0003) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)0xfffc);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1377;
      break;
    case 1376:  // $E63F
      /*$E63F*/ tmp3_U16 = sbc_dec16(s_a, 0x03, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 1377;
      break;
    case 1377:  // $E641
      /*$E641*/ ram_poke(0x0053, s_a);
      /*$E643*/ s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_y = 0x00;
                block_id = 1378;
      break;
    case 1378:  // $E645
      /*$E645*/ CYCLES(0xe645, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xe645, pop16() + 1);;
      break;
    case 1379:  // $E6F2
      /*$E6F2*/ CYCLES(0xe6f2, 6);
                branchTarget = true; block_id = 1144;
      break;
    case 1380:  // $E6F8
      /*$E6F8*/ CYCLES(0xe6f8, 6);
                branchTarget = true; push16(0xe6fa); block_id = 930;
      break;
    case 1381:  // $E6FB
      /*$E6FB*/ CYCLES(0xe6fb, 6);
                branchTarget = true; push16(0xe6fd); block_id = 1120;
      break;
    case 1382:  // $E6FE
      /*$E6FE*/ CYCLES(0xe6fe, 7);
      /*$E700*/ branchTarget = true; block_id = ram_peek(0x00a0) ? 1379 : 1383;
      break;
    case 1383:  // $E702
      /*$E702*/ CYCLES(0xe702, 9);
                s_x = ram_peek(0x00a1);
      /*$E704*/ branchTarget = true; block_id = 4;
      break;
    case 1384:  // $E752
      /*$E752*/ CYCLES(0xe752, 11);
      /*$E754*/ tmp2_U8 = ram_peek(0x009d) >= 0x91;
                s_status_c = tmp2_U8;
      /*$E756*/ branchTarget = true; block_id = tmp2_U8 ? 1379 : 1385;
      break;
    case 1385:  // $E758
      /*$E758*/ CYCLES(0xe758, 6);
                FUNC_QINT(0xfffe);
                branchTarget = true; block_id = 1386;
      break;
    case 1386:  // $E75B
      /*$E75B*/ CYCLES(0xe75b, 16);
                tmp2_U8 = ram_peek(0x00a0);
                s_a = tmp2_U8;
      /*$E75D*/ tmp1_U8 = ram_peek(0x00a1);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
      /*$E75F*/ ram_poke(0x0050, tmp1_U8);
      /*$E761*/ ram_poke(0x0051, tmp2_U8);
      /*$E763*/ branchTarget = true; block_id = find_block_id_func_t001(0xe763, pop16() + 1);;
      break;
    case 1387:  // $E79F
      /*$E79F*/ CYCLES(0xe79f, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xe79f, pop16() + 1);;
      break;
    case 1388:  // $E7A0
      /*$E7A0*/ CYCLES(0xe7a0, 12);
                s_a = 0x64;
      /*$E7A2*/ s_y = 0xee;
      /*$E7A4*/ branchTarget = true; block_id = 1391;
      break;
    case 1389:  // $E7B9
      /*$E7B9*/ CYCLES(0xe7b9, 6);
                FUNC_SHFTRGHT(0xfffe);
                branchTarget = true; block_id = 1390;
      break;
    case 1390:  // $E7BC
      /*$E7BC*/ CYCLES(0xe7bc, 4);
                branchTarget = true; block_id = !s_status_c ? 1408 : 1391;
      break;
    case 1391:  // $E7BE
      /*$E7BE*/ CYCLES(0xe7be, 6);
                FUNC_UPAY2ARG(0xfffe);
                branchTarget = true; block_id = 1392;
      break;
    case 1392:  // $E7C1
      /*$E7C1*/ CYCLES(0xe7c1, 4);
                branchTarget = true; block_id = s_status_not_z ? 1394 : 1393;
      break;
    case 1393:  // $E7C3
      /*$E7C3*/ CYCLES(0xe7c3, 6);
      /*$EB53*/ CYCLES(0xeb53, 11);
      /*$EB55*/ ram_poke(0x00a2, ram_peek(0x00aa));
      /*$EB57*/ s_x = 0x05;
                block_id = 1581;
      break;
    case 1394:  // $E7C6
      /*$E7C6*/ CYCLES(0xe7c6, 14);
      /*$E7C8*/ ram_poke(0x0092, ram_peek(0x00ac));
      /*$E7CA*/ s_x = 0xa5;
      /*$E7CC*/ s_a = ram_peek(0x00a5);
                block_id = 1395;
      break;
    case 1395:  // $E7CE
      /*$E7CE*/ CYCLES(0xe7ce, 6);
                tmp2_U8 = s_a;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_y = tmp2_U8;
      /*$E7CF*/ branchTarget = true; block_id = !tmp2_U8 ? 1387 : 1396;
      break;
    case 1396:  // $E7D1
      /*$E7D1*/ CYCLES(0xe7d1, 9);
                s_status_c = 0x01;
      /*$E7D2*/ block_id = s_status_d ? 1398 : 1397;
      break;
    case 1397:  // $E7D2
      /*$E7D2*/ tmp3_U16 = s_a;
                tmp6_U16 = ram_peek(0x009d);
                tmp4_U16 = (tmp3_U16 - tmp6_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp6_U16));
                tmp2_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp2_U8;
                s_a = tmp2_U8;
                block_id = 1399;
      break;
    case 1398:  // $E7D2
      /*$E7D2*/ tmp4_U16 = sbc_dec16(s_a, ram_peek(0x009d), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 1399;
      break;
    case 1399:  // $E7D4
      /*$E7D4*/ branchTarget = true; block_id = !s_status_not_z ? 1408 : 1400;
      break;
    case 1400:  // $E7D6
      /*$E7D6*/ CYCLES(0xe7d6, 4);
                branchTarget = true; block_id = !s_status_c ? 1405 : 1401;
      break;
    case 1401:  // $E7D8
      /*$E7D8*/ CYCLES(0xe7d8, 31);
                ram_poke(0x009d, s_y);
      /*$E7DC*/ ram_poke(0x00a2, ram_peek(0x00aa));
      /*$E7DE*/ s_a = (s_a ^ 0xff);
      /*$E7E0*/ block_id = s_status_d ? 1403 : 1402;
      break;
    case 1402:  // $E7E0
      /*$E7E0*/ tmp4_U16 = s_a;
                tmp6_U16 = tmp4_U16 + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1404;
      break;
    case 1403:  // $E7E0
      /*$E7E0*/ tmp6_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 1404;
      break;
    case 1404:  // $E7E2
      /*$E7E2*/ s_y = 0x00;
      /*$E7E4*/ ram_poke(0x0092, 0x00);
      /*$E7E6*/ s_x = 0x9d;
      /*$E7E8*/ branchTarget = true; block_id = 1406;
      break;
    case 1405:  // $E7EA
      /*$E7EA*/ CYCLES(0xe7ea, 7);
                s_y = 0x00;
      /*$E7EC*/ ram_poke(0x00ac, 0x00);
                block_id = 1406;
      break;
    case 1406:  // $E7EE
      /*$E7EE*/ CYCLES(0xe7ee, 7);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0xf9);
      /*$E7F0*/ branchTarget = true; block_id = ((uint8_t)(tmp2_U8 - 0xf9) & 0x80) ? 1389 : 1407;
      break;
    case 1407:  // $E7F2
      /*$E7F2*/ CYCLES(0xe7f2, 14);
                s_y = s_a;
      /*$E7F3*/ s_a = ram_peek(0x00ac);
      /*$E7F5*/ tmp6_U16 = (uint8_t)(0x01 + s_x);
                tmp2_U8 = ram_peek(tmp6_U16);
                s_status_c = (tmp2_U8 & 0x01);
                ram_poke(tmp6_U16, (tmp2_U8 >> 0x01));
      /*$E7F7*/ FUNC_SHFTRGHT4(0xfffe);
                branchTarget = true; block_id = 1408;
      break;
    case 1408:  // $E7FA
      /*$E7FA*/ CYCLES(0xe7fa, 7);
      /*$E7FC*/ branchTarget = true; block_id = !(ram_peek(0x00ab) & 0x80) ? 1437 : 1409;
      break;
    case 1409:  // $E7FE
      /*$E7FE*/ CYCLES(0xe7fe, 11);
                s_y = 0x9d;
      /*$E802*/ branchTarget = true; block_id = !(s_x != 0xa5) ? 1411 : 1410;
      break;
    case 1410:  // $E804
      /*$E804*/ CYCLES(0xe804, 4);
                s_y = 0xa5;
                block_id = 1411;
      break;
    case 1411:  // $E806
      /*$E806*/ CYCLES(0xe806, 60);
                s_status_c = 0x01;
      /*$E807*/ s_a = (s_a ^ 0xff);
      /*$E809*/ block_id = s_status_d ? 1413 : 1412;
      break;
    case 1412:  // $E809
      /*$E809*/ tmp6_U16 = (s_a + ram_peek(0x0092)) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1414;
      break;
    case 1413:  // $E809
      /*$E809*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x0092), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1414;
      break;
    case 1414:  // $E80B
      /*$E80B*/ ram_poke(0x00ac, s_a);
      /*$E80D*/ s_a = ram_peek((0x0004 + s_y));
      /*$E810*/ block_id = s_status_d ? 1416 : 1415;
      break;
    case 1415:  // $E810
      /*$E810*/ tmp6_U16 = (s_a - ram_peek((uint8_t)(0x04 + s_x))) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1417;
      break;
    case 1416:  // $E810
      /*$E810*/ tmp6_U16 = sbc_dec16(s_a, ram_peek((uint8_t)(0x04 + s_x)), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1417;
      break;
    case 1417:  // $E812
      /*$E812*/ ram_poke(0x00a1, s_a);
      /*$E814*/ s_a = ram_peek((0x0003 + s_y));
      /*$E817*/ block_id = s_status_d ? 1419 : 1418;
      break;
    case 1418:  // $E817
      /*$E817*/ tmp6_U16 = (s_a - ram_peek((uint8_t)(0x03 + s_x))) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1420;
      break;
    case 1419:  // $E817
      /*$E817*/ tmp6_U16 = sbc_dec16(s_a, ram_peek((uint8_t)(0x03 + s_x)), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1420;
      break;
    case 1420:  // $E819
      /*$E819*/ ram_poke(0x00a0, s_a);
      /*$E81B*/ s_a = ram_peek((0x0002 + s_y));
      /*$E81E*/ block_id = s_status_d ? 1422 : 1421;
      break;
    case 1421:  // $E81E
      /*$E81E*/ tmp6_U16 = (s_a - ram_peek((uint8_t)(0x02 + s_x))) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1423;
      break;
    case 1422:  // $E81E
      /*$E81E*/ tmp6_U16 = sbc_dec16(s_a, ram_peek((uint8_t)(0x02 + s_x)), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1423;
      break;
    case 1423:  // $E820
      /*$E820*/ ram_poke(0x009f, s_a);
      /*$E822*/ s_a = ram_peek((0x0001 + s_y));
      /*$E825*/ block_id = s_status_d ? 1425 : 1424;
      break;
    case 1424:  // $E825
      /*$E825*/ tmp6_U16 = (s_a - ram_peek((uint8_t)(0x01 + s_x))) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1426;
      break;
    case 1425:  // $E825
      /*$E825*/ tmp6_U16 = sbc_dec16(s_a, ram_peek((uint8_t)(0x01 + s_x)), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1426;
      break;
    case 1426:  // $E827
      /*$E827*/ ram_poke(0x009e, s_a);
                block_id = 1427;
      break;
    case 1427:  // $E829
      /*$E829*/ CYCLES(0xe829, 4);
                branchTarget = true; block_id = s_status_c ? 1429 : 1428;
      break;
    case 1428:  // $E82B
      /*$E82B*/ CYCLES(0xe82b, 6);
                FUNC_TWSCMPFAC(0xfffe);
                branchTarget = true; block_id = 1429;
      break;
    case 1429:  // $E82E
      /*$E82E*/ CYCLES(0xe82e, 7);
                s_y = 0x00;
      /*$E830*/ s_a = 0x00;
      /*$E831*/ s_status_c = 0x00;
                block_id = 1430;
      break;
    case 1430:  // $E832
      /*$E832*/ CYCLES(0xe832, 7);
                tmp2_U8 = ram_peek(0x009e);
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$E834*/ branchTarget = true; block_id = tmp2_U8 ? 1457 : 1431;
      break;
    case 1431:  // $E836
      /*$E836*/ CYCLES(0xe836, 41);
      /*$E838*/ ram_poke(0x009e, ram_peek(0x009f));
      /*$E83C*/ ram_poke(0x009f, ram_peek(0x00a0));
      /*$E840*/ ram_poke(0x00a0, ram_peek(0x00a1));
      /*$E842*/ tmp2_U8 = ram_peek(0x00ac);
                s_x = tmp2_U8;
      /*$E844*/ ram_poke(0x00a1, tmp2_U8);
      /*$E846*/ ram_poke(0x00ac, s_y);
      /*$E848*/ block_id = s_status_d ? 1433 : 1432;
      break;
    case 1432:  // $E848
      /*$E848*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 + 0x0008) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x0008);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1434;
      break;
    case 1433:  // $E848
      /*$E848*/ tmp4_U16 = adc_dec16(s_a, 0x08, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 1434;
      break;
    case 1434:  // $E84A
      /*$E84A*/ tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x20);
      /*$E84C*/ branchTarget = true; block_id = (tmp2_U8 != 0x20) ? 1430 : 1435;
      break;
    case 1435:  // $E84E
      /*$E84E*/ CYCLES(0xe84e, 7);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$E850*/ ram_poke(0x009d, 0x00);
                block_id = 1436;
      break;
    case 1436:  // $E852
      /*$E852*/ CYCLES(0xe852, 6);
                ram_poke(0x00a2, s_a);
      /*$E854*/ branchTarget = true; block_id = find_block_id_func_t001(0xe854, pop16() + 1);;
      break;
    case 1437:  // $E855
      /*$E855*/ CYCLES(0xe855, 53);
                block_id = s_status_d ? 1439 : 1438;
      break;
    case 1438:  // $E855
      /*$E855*/ tmp4_U16 = (s_a + ram_peek(0x0092)) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1440;
      break;
    case 1439:  // $E855
      /*$E855*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x0092), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1440;
      break;
    case 1440:  // $E857
      /*$E857*/ ram_poke(0x00ac, s_a);
      /*$E859*/ s_a = ram_peek(0x00a1);
      /*$E85B*/ block_id = s_status_d ? 1442 : 1441;
      break;
    case 1441:  // $E85B
      /*$E85B*/ tmp4_U16 = (s_a + ram_peek(0x00a9)) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1443;
      break;
    case 1442:  // $E85B
      /*$E85B*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x00a9), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1443;
      break;
    case 1443:  // $E85D
      /*$E85D*/ ram_poke(0x00a1, s_a);
      /*$E85F*/ s_a = ram_peek(0x00a0);
      /*$E861*/ block_id = s_status_d ? 1445 : 1444;
      break;
    case 1444:  // $E861
      /*$E861*/ tmp4_U16 = (s_a + ram_peek(0x00a8)) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1446;
      break;
    case 1445:  // $E861
      /*$E861*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x00a8), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1446;
      break;
    case 1446:  // $E863
      /*$E863*/ ram_poke(0x00a0, s_a);
      /*$E865*/ s_a = ram_peek(0x009f);
      /*$E867*/ block_id = s_status_d ? 1448 : 1447;
      break;
    case 1447:  // $E867
      /*$E867*/ tmp4_U16 = (s_a + ram_peek(0x00a7)) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1449;
      break;
    case 1448:  // $E867
      /*$E867*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x00a7), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1449;
      break;
    case 1449:  // $E869
      /*$E869*/ ram_poke(0x009f, s_a);
      /*$E86B*/ s_a = ram_peek(0x009e);
      /*$E86D*/ block_id = s_status_d ? 1451 : 1450;
      break;
    case 1450:  // $E86D
      /*$E86D*/ tmp4_U16 = s_a;
                tmp6_U16 = ram_peek(0x00a6);
                tmp3_U16 = (tmp4_U16 + tmp6_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp6_U16);
                tmp2_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 1452;
      break;
    case 1451:  // $E86D
      /*$E86D*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x00a6), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 1452;
      break;
    case 1452:  // $E86F
      /*$E86F*/ ram_poke(0x009e, s_a);
      /*$E871*/ branchTarget = true; block_id = 1466;
      break;
    case 1453:  // $E874
      /*$E874*/ CYCLES(0xe874, 21);
                block_id = s_status_d ? 1455 : 1454;
      break;
    case 1454:  // $E874
      /*$E874*/ s_a = (uint8_t)((s_a + 0x0001) + s_status_c);
                block_id = 1456;
      break;
    case 1455:  // $E874
      /*$E874*/ s_a = ((uint8_t)adc_dec16(s_a, 0x01, s_status_c));
                block_id = 1456;
      break;
    case 1456:  // $E876
      /*$E876*/ tmp3_U16 = ram_peek(0x00ac) << 0x01;
                ram_poke(0x00ac, ((uint8_t)tmp3_U16));
      /*$E878*/ tmp3_U16 = (ram_peek(0x00a1) << 0x01) | (uint8_t)(tmp3_U16 >> 8);
                ram_poke(0x00a1, ((uint8_t)tmp3_U16));
      /*$E87A*/ tmp3_U16 = (ram_peek(0x00a0) << 0x01) | (uint8_t)(tmp3_U16 >> 8);
                ram_poke(0x00a0, ((uint8_t)tmp3_U16));
      /*$E87C*/ tmp3_U16 = (ram_peek(0x009f) << 0x01) | (uint8_t)(tmp3_U16 >> 8);
                ram_poke(0x009f, ((uint8_t)tmp3_U16));
      /*$E87E*/ tmp3_U16 = (ram_peek(0x009e) << 0x01) | (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                tmp2_U8 = (uint8_t)tmp3_U16;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x009e, tmp2_U8);
                block_id = 1457;
      break;
    case 1457:  // $E880
      /*$E880*/ CYCLES(0xe880, 4);
                branchTarget = true; block_id = !s_status_n ? 1453 : 1458;
      break;
    case 1458:  // $E882
      /*$E882*/ CYCLES(0xe882, 9);
                s_status_c = 0x01;
      /*$E883*/ block_id = s_status_d ? 1460 : 1459;
      break;
    case 1459:  // $E883
      /*$E883*/ tmp3_U16 = s_a;
                tmp6_U16 = ram_peek(0x009d);
                tmp4_U16 = (tmp3_U16 - tmp6_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp6_U16));
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1461;
      break;
    case 1460:  // $E883
      /*$E883*/ tmp4_U16 = sbc_dec16(s_a, ram_peek(0x009d), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 1461;
      break;
    case 1461:  // $E885
      /*$E885*/ branchTarget = true; block_id = s_status_c ? 1435 : 1462;
      break;
    case 1462:  // $E887
      /*$E887*/ CYCLES(0xe887, 11);
                s_a = (s_a ^ 0xff);
      /*$E889*/ block_id = s_status_d ? 1464 : 1463;
      break;
    case 1463:  // $E889
      /*$E889*/ tmp4_U16 = s_a;
                tmp6_U16 = (tmp4_U16 + 0x0001) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0x0001);
                tmp2_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 1465;
      break;
    case 1464:  // $E889
      /*$E889*/ tmp6_U16 = adc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp2_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 1465;
      break;
    case 1465:  // $E88B
      /*$E88B*/ ram_poke(0x009d, s_a);
                block_id = 1466;
      break;
    case 1466:  // $E88D
      /*$E88D*/ CYCLES(0xe88d, 4);
                branchTarget = true; block_id = !s_status_c ? 1469 : 1467;
      break;
    case 1467:  // $E88F
      /*$E88F*/ CYCLES(0xe88f, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x009d) + 0x01);
                ram_poke(0x009d, tmp2_U8);
      /*$E891*/ branchTarget = true; block_id = !tmp2_U8 ? 1477 : 1468;
      break;
    case 1468:  // $E893
      /*$E893*/ CYCLES(0xe893, 18);
                tmp2_U8 = ram_peek(0x009e);
                ram_poke(0x009e, (uint8_t)((tmp2_U8 | (0x00 + (s_status_c << 8))) >> 0x01));
      /*$E895*/ tmp1_U8 = ram_peek(0x009f);
                ram_poke(0x009f, (uint8_t)((tmp1_U8 | (0x00 + ((tmp2_U8 & 0x01) << 8))) >> 0x01));
      /*$E897*/ tmp2_U8 = ram_peek(0x00a0);
                ram_poke(0x00a0, (uint8_t)((tmp2_U8 | (0x00 + ((tmp1_U8 & 0x01) << 8))) >> 0x01));
      /*$E899*/ tmp1_U8 = ram_peek(0x00a1);
                ram_poke(0x00a1, (uint8_t)((tmp1_U8 | (0x00 + ((tmp2_U8 & 0x01) << 8))) >> 0x01));
      /*$E89B*/ tmp2_U8 = ram_peek(0x00ac);
                tmp1_U8 = (uint8_t)((tmp2_U8 | (0x00 + ((tmp1_U8 & 0x01) << 8))) >> 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x00ac, tmp1_U8);
                s_status_c = (tmp2_U8 & 0x01);
                block_id = 1469;
      break;
    case 1469:  // $E89D
      /*$E89D*/ CYCLES(0xe89d, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xe89d, pop16() + 1);;
      break;
    case 1470:  // $E89E
      /*$E89E*/ CYCLES(0xe89e, 11);
      /*$E8A2*/ ram_poke(0x00a2, (ram_peek(0x00a2) ^ 0xff));
                FUNC_TCFACMANT(0x0000);
                block_id = find_block_id_func_t001(0xe8a2, pop16() + 1);;
      break;
    case 1471:  // $E8A4
      /*$E8A4*/ CYCLES(0xe8a4, 58);
      /*$E8A8*/ ram_poke(0x009e, (ram_peek(0x009e) ^ 0xff));
      /*$E8AE*/ ram_poke(0x009f, (ram_peek(0x009f) ^ 0xff));
      /*$E8B4*/ ram_poke(0x00a0, (ram_peek(0x00a0) ^ 0xff));
      /*$E8BA*/ ram_poke(0x00a1, (ram_peek(0x00a1) ^ 0xff));
      /*$E8BE*/ tmp2_U8 = ram_peek(0x00ac) ^ 0xff;
                s_a = tmp2_U8;
      /*$E8C0*/ ram_poke(0x00ac, tmp2_U8);
      /*$E8C2*/ tmp2_U8 = (uint8_t)(ram_peek(0x00ac) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x00ac, tmp2_U8);
      /*$E8C4*/ branchTarget = true; block_id = tmp2_U8 ? 1476 : 1939;
      break;
    case 1472:  // $E8C6
      /*$E8C6*/ CYCLES(0xe8c6, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x00a1) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x00a1, tmp2_U8);
      /*$E8C8*/ branchTarget = true; block_id = tmp2_U8 ? 1476 : 1473;
      break;
    case 1473:  // $E8CA
      /*$E8CA*/ CYCLES(0xe8ca, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x00a0) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x00a0, tmp2_U8);
      /*$E8CC*/ branchTarget = true; block_id = tmp2_U8 ? 1476 : 1474;
      break;
    case 1474:  // $E8CE
      /*$E8CE*/ CYCLES(0xe8ce, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x009f) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x009f, tmp2_U8);
      /*$E8D0*/ branchTarget = true; block_id = tmp2_U8 ? 1476 : 1475;
      break;
    case 1475:  // $E8D2
      /*$E8D2*/ CYCLES(0xe8d2, 4);
                tmp2_U8 = (uint8_t)(ram_peek(0x009e) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x009e, tmp2_U8);
                block_id = 1476;
      break;
    case 1476:  // $E8D4
      /*$E8D4*/ CYCLES(0xe8d4, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xe8d4, pop16() + 1);;
      break;
    case 1477:  // $E8D5
      /*$E8D5*/ CYCLES(0xe8d5, 9);
                s_x = 0x45;
      /*$E8D7*/ branchTarget = true; block_id = 641;
      break;
    case 1478:  // $E8DC
      /*$E8DC*/ CYCLES(0xe8dc, 35);
                tmp2_U8 = s_x;
      /*$E8DE*/ ram_poke(0x00ac, ram_peek((uint8_t)(0x04 + tmp2_U8)));
      /*$E8E2*/ ram_poke((uint8_t)(0x04 + tmp2_U8), ram_peek((uint8_t)(0x03 + tmp2_U8)));
      /*$E8E6*/ ram_poke((uint8_t)(0x03 + tmp2_U8), ram_peek((uint8_t)(0x02 + tmp2_U8)));
      /*$E8EA*/ ram_poke((uint8_t)(0x02 + tmp2_U8), ram_peek((uint8_t)(0x01 + tmp2_U8)));
      /*$E8EE*/ ram_poke((uint8_t)(0x01 + tmp2_U8), ram_peek(0x00a4));
      /*$E8F0*/ CYCLES(0xe8f0, 7);
                block_id = s_status_d ? 1480 : 1479;
      break;
    case 1479:  // $E8F0
      /*$E8F0*/ tmp6_U16 = (s_a + 0x0008) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                tmp2_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 1481;
      break;
    case 1480:  // $E8F0
      /*$E8F0*/ tmp6_U16 = adc_dec16(s_a, 0x08, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp2_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 1481;
      break;
    case 1481:  // $E8F2
      /*$E8F2*/ branchTarget = true; block_id = s_status_n ? 1478 : 1482;
      break;
    case 1482:  // $E8F4
      /*$E8F4*/ CYCLES(0xe8f4, 4);
                branchTarget = true; block_id = !s_status_not_z ? 1478 : 1483;
      break;
    case 1483:  // $E8F6
      /*$E8F6*/ CYCLES(0xe8f6, 12);
                block_id = s_status_d ? 1485 : 1484;
      break;
    case 1484:  // $E8F6
      /*$E8F6*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 - 0x0008) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0xfff7);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1486;
      break;
    case 1485:  // $E8F6
      /*$E8F6*/ tmp4_U16 = sbc_dec16(s_a, 0x08, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 1486;
      break;
    case 1486:  // $E8F8
      /*$E8F8*/ s_y = s_a;
      /*$E8F9*/ tmp2_U8 = ram_peek(0x00ac);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$E8FB*/ branchTarget = true; block_id = s_status_c ? 1490 : 1487;
      break;
    case 1487:  // $E8FD
      /*$E8FD*/ CYCLES(0xe8fd, 7);
                tmp4_U16 = (uint8_t)(0x01 + s_x);
                tmp6_U16 = ram_peek(tmp4_U16) << 0x01;
                tmp2_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = tmp2_U8;
                ram_poke(tmp4_U16, ((uint8_t)tmp6_U16));
      /*$E8FF*/ branchTarget = true; block_id = !tmp2_U8 ? 1489 : 1488;
      break;
    case 1488:  // $E901
      /*$E901*/ CYCLES(0xe901, 4);
                tmp6_U16 = (uint8_t)(0x01 + s_x);
                ram_poke(tmp6_U16, (uint8_t)(ram_peek(tmp6_U16) + 0x01));
                block_id = 1489;
      break;
    case 1489:  // $E903
      /*$E903*/ CYCLES(0xe903, 7);
                tmp2_U8 = s_x;
                tmp6_U16 = (uint8_t)(0x01 + tmp2_U8);
                tmp1_U8 = ram_peek(tmp6_U16);
                ram_poke(tmp6_U16, (uint8_t)((tmp1_U8 | (0x00 + (s_status_c << 8))) >> 0x01));
      /*$E905*/ tmp6_U16 = (uint8_t)(0x01 + tmp2_U8);
                tmp5_U8 = ram_peek(tmp6_U16);
                ram_poke(tmp6_U16, (uint8_t)((tmp5_U8 | (0x00 + ((tmp1_U8 & 0x01) << 8))) >> 0x01));
      /*$E907*/ CYCLES(0xe907, 18);
                tmp6_U16 = (uint8_t)(0x02 + tmp2_U8);
                tmp1_U8 = ram_peek(tmp6_U16);
                ram_poke(tmp6_U16, (uint8_t)((tmp1_U8 | (0x00 + ((tmp5_U8 & 0x01) << 8))) >> 0x01));
      /*$E909*/ tmp6_U16 = (uint8_t)(0x03 + tmp2_U8);
                tmp5_U8 = ram_peek(tmp6_U16);
                ram_poke(tmp6_U16, (uint8_t)((tmp5_U8 | (0x00 + ((tmp1_U8 & 0x01) << 8))) >> 0x01));
      /*$E90B*/ tmp6_U16 = (uint8_t)(0x04 + tmp2_U8);
                tmp2_U8 = ram_peek(tmp6_U16);
                ram_poke(tmp6_U16, (uint8_t)((tmp2_U8 | (0x00 + ((tmp5_U8 & 0x01) << 8))) >> 0x01));
      /*$E90D*/ s_a = (uint8_t)((s_a | (0x00 + ((tmp2_U8 & 0x01) << 8))) >> 0x01);
      /*$E90E*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_y = tmp2_U8;
      /*$E90F*/ branchTarget = true; block_id = tmp2_U8 ? 1487 : 1490;
      break;
    case 1490:  // $E911
      /*$E911*/ CYCLES(0xe911, 4);
                s_status_c = 0x00;
      /*$E912*/ branchTarget = true; block_id = find_block_id_func_t001(0xe912, pop16() + 1);;
      break;
    case 1491:  // $E97F
      /*$E97F*/ CYCLES(0xe97f, 6);
                FUNC_UPAY2ARG(0xfffe);
      /*$E982*/ CYCLES(0xe982, 4);
                branchTarget = true; block_id = s_status_not_z ? 1493 : 1492;
      break;
    case 1492:  // $E984
      /*$E984*/ CYCLES(0xe984, 6);
                branchTarget = true; block_id = 1513;
      break;
    case 1493:  // $E987
      /*$E987*/ CYCLES(0xe987, 6);
                branchTarget = true; push16(0xe989); block_id = 1515;
      break;
    case 1494:  // $E98A
      /*$E98A*/ CYCLES(0xe98a, 26);
      /*$E98C*/ ram_poke(0x0062, 0x00);
      /*$E98E*/ ram_poke(0x0063, 0x00);
      /*$E990*/ ram_poke(0x0064, 0x00);
      /*$E992*/ ram_poke(0x0065, 0x00);
      /*$E994*/ tmp2_U8 = ram_peek(0x00ac);
                s_status_not_z = tmp2_U8;
                s_a = tmp2_U8;
      /*$E996*/ FUNC_MULT1(0xfffe);
      /*$E999*/ CYCLES(0xe999, 9);
                tmp2_U8 = ram_peek(0x00a1);
                s_status_not_z = tmp2_U8;
                s_a = tmp2_U8;
      /*$E99B*/ FUNC_MULT1(0xfffe);
      /*$E99E*/ CYCLES(0xe99e, 9);
                tmp2_U8 = ram_peek(0x00a0);
                s_status_not_z = tmp2_U8;
                s_a = tmp2_U8;
      /*$E9A0*/ FUNC_MULT1(0xfffe);
      /*$E9A3*/ CYCLES(0xe9a3, 9);
                tmp2_U8 = ram_peek(0x009f);
                s_status_not_z = tmp2_U8;
                s_a = tmp2_U8;
      /*$E9A5*/ FUNC_MULT1(0xfffe);
      /*$E9A8*/ CYCLES(0xe9a8, 9);
                s_a = ram_peek(0x009e);
      /*$E9AA*/ FUNC_MULT2(0xfffe);
      /*$E9AD*/ CYCLES(0xe9ad, 6);
                branchTarget = true; block_id = 1577;
      break;
    case 1495:  // $E9B0
      /*$E9B0*/ CYCLES(0xe9b0, 4);
                branchTarget = true; block_id = s_status_not_z ? 1940 : 1496;
      break;
    case 1496:  // $E9B2
      /*$E9B2*/ CYCLES(0xe9b2, 6);
      /*$E8DA*/ CYCLES(0xe8da, 4);
                s_x = 0x61;
                block_id = 1478;
      break;
    case 1497:  // $E9B5
      /*$E9B5*/ CYCLES(0xe9b5, 6);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 & 0x01);
      /*$E9B6*/ s_a = ((tmp2_U8 >> 0x01) | 0x80);
                block_id = 1498;
      break;
    case 1498:  // $E9B8
      /*$E9B8*/ CYCLES(0xe9b8, 6);
                s_y = s_a;
      /*$E9B9*/ branchTarget = true; block_id = !s_status_c ? 1512 : 1499;
      break;
    case 1499:  // $E9BB
      /*$E9BB*/ CYCLES(0xe9bb, 43);
                s_status_c = 0x00;
      /*$E9BC*/ s_a = ram_peek(0x0065);
      /*$E9BE*/ block_id = s_status_d ? 1501 : 1500;
      break;
    case 1500:  // $E9BE
      /*$E9BE*/ tmp6_U16 = (s_a + ram_peek(0x00a9)) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1502;
      break;
    case 1501:  // $E9BE
      /*$E9BE*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x00a9), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1502;
      break;
    case 1502:  // $E9C0
      /*$E9C0*/ ram_poke(0x0065, s_a);
      /*$E9C2*/ s_a = ram_peek(0x0064);
      /*$E9C4*/ block_id = s_status_d ? 1504 : 1503;
      break;
    case 1503:  // $E9C4
      /*$E9C4*/ tmp6_U16 = (s_a + ram_peek(0x00a8)) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1505;
      break;
    case 1504:  // $E9C4
      /*$E9C4*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x00a8), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1505;
      break;
    case 1505:  // $E9C6
      /*$E9C6*/ ram_poke(0x0064, s_a);
      /*$E9C8*/ s_a = ram_peek(0x0063);
      /*$E9CA*/ block_id = s_status_d ? 1507 : 1506;
      break;
    case 1506:  // $E9CA
      /*$E9CA*/ tmp6_U16 = (s_a + ram_peek(0x00a7)) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1508;
      break;
    case 1507:  // $E9CA
      /*$E9CA*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x00a7), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1508;
      break;
    case 1508:  // $E9CC
      /*$E9CC*/ ram_poke(0x0063, s_a);
      /*$E9CE*/ s_a = ram_peek(0x0062);
      /*$E9D0*/ block_id = s_status_d ? 1510 : 1509;
      break;
    case 1509:  // $E9D0
      /*$E9D0*/ tmp6_U16 = s_a;
                tmp4_U16 = ram_peek(0x00a6);
                tmp3_U16 = (tmp6_U16 + tmp4_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1511;
      break;
    case 1510:  // $E9D0
      /*$E9D0*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x00a6), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1511;
      break;
    case 1511:  // $E9D2
      /*$E9D2*/ ram_poke(0x0062, s_a);
                block_id = 1512;
      break;
    case 1512:  // $E9D4
      /*$E9D4*/ CYCLES(0xe9d4, 24);
                tmp2_U8 = ram_peek(0x0062);
                ram_poke(0x0062, (uint8_t)((tmp2_U8 | (0x00 + (s_status_c << 8))) >> 0x01));
      /*$E9D6*/ tmp5_U8 = ram_peek(0x0063);
                ram_poke(0x0063, (uint8_t)((tmp5_U8 | (0x00 + ((tmp2_U8 & 0x01) << 8))) >> 0x01));
      /*$E9D8*/ tmp2_U8 = ram_peek(0x0064);
                ram_poke(0x0064, (uint8_t)((tmp2_U8 | (0x00 + ((tmp5_U8 & 0x01) << 8))) >> 0x01));
      /*$E9DA*/ tmp5_U8 = ram_peek(0x0065);
                ram_poke(0x0065, (uint8_t)((tmp5_U8 | (0x00 + ((tmp2_U8 & 0x01) << 8))) >> 0x01));
      /*$E9DC*/ ram_poke(0x00ac, (uint8_t)((ram_peek(0x00ac) | (0x00 + ((tmp5_U8 & 0x01) << 8))) >> 0x01));
      /*$E9DE*/ tmp5_U8 = s_y;
      /*$E9DF*/ s_status_c = (tmp5_U8 & 0x01);
                tmp5_U8 = tmp5_U8 >> 0x01;
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$E9E0*/ branchTarget = true; block_id = tmp5_U8 ? 1498 : 1513;
      break;
    case 1513:  // $E9E2
      /*$E9E2*/ CYCLES(0xe9e2, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xe9e2, pop16() + 1);;
      break;
    case 1514:  // $E9E3
      /*$E9E3*/ CYCLES(0xe9e3, 74);
                ram_poke(0x005e, s_a);
      /*$E9E5*/ ram_poke(0x005f, s_y);
      /*$E9E9*/ tmp2_U8 = peek((ram_peek16al(0x005e) + 0x0004));
      /*$E9EB*/ ram_poke(0x00a9, tmp2_U8);
      /*$E9EE*/ tmp2_U8 = peek((ram_peek16al(0x005e) + 0x0003));
      /*$E9F0*/ ram_poke(0x00a8, tmp2_U8);
      /*$E9F3*/ tmp2_U8 = peek((ram_peek16al(0x005e) + 0x0002));
      /*$E9F5*/ ram_poke(0x00a7, tmp2_U8);
      /*$E9F8*/ tmp2_U8 = peek((ram_peek16al(0x005e) + 0x0001));
      /*$E9FA*/ ram_poke(0x00aa, tmp2_U8);
      /*$E9FE*/ ram_poke(0x00ab, (tmp2_U8 ^ ram_peek(0x00a2)));
      /*$EA04*/ ram_poke(0x00a6, (ram_peek(0x00aa) | 0x80));
      /*$EA06*/ s_y = 0x00;
      /*$EA07*/ tmp2_U8 = peek(ram_peek16al(0x005e));
      /*$EA09*/ ram_poke(0x00a5, tmp2_U8);
      /*$EA0B*/ tmp2_U8 = ram_peek(0x009d);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$EA0D*/ branchTarget = true; block_id = find_block_id_func_t001(0xea0d, pop16() + 1);;
      break;
    case 1515:  // $EA0E
      /*$EA0E*/ CYCLES(0xea0e, 7);
                tmp2_U8 = ram_peek(0x00a5);
                s_a = tmp2_U8;
      /*$EA10*/ branchTarget = true; block_id = !tmp2_U8 ? 1529 : 1516;
      break;
    case 1516:  // $EA12
      /*$EA12*/ CYCLES(0xea12, 9);
                s_status_c = 0x00;
      /*$EA13*/ block_id = s_status_d ? 1518 : 1517;
      break;
    case 1517:  // $EA13
      /*$EA13*/ tmp3_U16 = s_a;
                tmp4_U16 = ram_peek(0x009d);
                tmp6_U16 = (tmp3_U16 + tmp4_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
                tmp5_U8 = (uint8_t)tmp6_U16;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
                block_id = 1519;
      break;
    case 1518:  // $EA13
      /*$EA13*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x009d), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                s_status_n = (tmp5_U8 & 0x80);
                block_id = 1519;
      break;
    case 1519:  // $EA15
      /*$EA15*/ branchTarget = true; block_id = !s_status_c ? 1522 : 1520;
      break;
    case 1520:  // $EA17
      /*$EA17*/ CYCLES(0xea17, 4);
                branchTarget = true; block_id = s_status_n ? 1530 : 1521;
      break;
    case 1521:  // $EA19
      /*$EA19*/ CYCLES(0xea19, 7);
                s_status_c = 0x00;
      /*$EA1A*/ block_id = 1523;
      break;
    case 1522:  // $EA1B
      /*$EA1B*/ CYCLES(0xea1b, 4);
                branchTarget = true; block_id = !s_status_n ? 1529 : 1523;
      break;
    case 1523:  // $EA1D
      /*$EA1D*/ CYCLES(0xea1d, 11);
                block_id = s_status_d ? 1525 : 1524;
      break;
    case 1524:  // $EA1D
      /*$EA1D*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 + 0x0080) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x0080);
                tmp5_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
                block_id = 1526;
      break;
    case 1525:  // $EA1D
      /*$EA1D*/ tmp4_U16 = adc_dec16(s_a, 0x80, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_not_z = (~tmp5_U8 & 2);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                s_status_n = (tmp5_U8 & 0x80);
                block_id = 1526;
      break;
    case 1526:  // $EA1F
      /*$EA1F*/ ram_poke(0x009d, s_a);
      /*$EA21*/ branchTarget = true; block_id = s_status_not_z ? 1528 : 1527;
      break;
    case 1527:  // $EA23
      /*$EA23*/ CYCLES(0xea23, 6);
                branchTarget = true; block_id = 1436;
      break;
    case 1528:  // $EA26
      /*$EA26*/ CYCLES(0xea26, 9);
                tmp5_U8 = ram_peek(0x00ab);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$EA28*/ ram_poke(0x00a2, tmp5_U8);
      /*$EA2A*/ branchTarget = true; block_id = find_block_id_func_t001(0xea2a, pop16() + 1);;
      break;
    case 1529:  // $EA31
      /*$EA31*/ CYCLES(0xea31, 9);
                tmp5_U8 = pop8();
      /*$EA32*/ tmp5_U8 = pop8();
      /*$EA33*/ branchTarget = true; block_id = 1435;
      break;
    case 1530:  // $EA36
      /*$EA36*/ CYCLES(0xea36, 6);
                branchTarget = true; block_id = 1477;
      break;
    case 1531:  // $EA39
      /*$EA39*/ CYCLES(0xea39, 6);
                branchTarget = true; push16(0xea3b); block_id = 1583;
      break;
    case 1532:  // $EA3C
      /*$EA3C*/ CYCLES(0xea3c, 6);
                tmp5_U8 = s_a;
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_x = tmp5_U8;
      /*$EA3D*/ branchTarget = true; block_id = !tmp5_U8 ? 1539 : 1533;
      break;
    case 1533:  // $EA3F
      /*$EA3F*/ CYCLES(0xea3f, 9);
                s_status_c = 0x00;
      /*$EA40*/ block_id = s_status_d ? 1535 : 1534;
      break;
    case 1534:  // $EA40
      /*$EA40*/ tmp4_U16 = s_a;
                tmp6_U16 = (tmp4_U16 + 0x0002) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0x0002);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1536;
      break;
    case 1535:  // $EA40
      /*$EA40*/ tmp6_U16 = adc_dec16(s_a, 0x02, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1536;
      break;
    case 1536:  // $EA42
      /*$EA42*/ branchTarget = true; block_id = s_status_c ? 1530 : 1537;
      break;
    case 1537:  // $EA44
      /*$EA44*/ CYCLES(0xea44, 12);
                s_x = 0x00;
      /*$EA46*/ ram_poke(0x00ab, 0x00);
      /*$EA48*/ branchTarget = true; push16(0xea4a); block_id = 1395;
      break;
    case 1538:  // $EA4B
      /*$EA4B*/ CYCLES(0xea4b, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x009d) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x009d, tmp2_U8);
      /*$EA4D*/ branchTarget = true; block_id = !tmp2_U8 ? 1530 : 1539;
      break;
    case 1539:  // $EA4F
      /*$EA4F*/ CYCLES(0xea4f, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xea4f, pop16() + 1);;
      break;
    case 1540:  // $EA55
      /*$EA55*/ CYCLES(0xea55, 6);
                branchTarget = true; push16(0xea57); block_id = 1583;
      break;
    case 1541:  // $EA58
      /*$EA58*/ CYCLES(0xea58, 19);
                s_a = 0x50;
      /*$EA5A*/ s_y = 0xea;
      /*$EA5C*/ s_x = 0x00;
      /*$EA5E*/ ram_poke(0x00ab, 0x00);
      /*$EA60*/ FUNC_UPAY2FAC(0xfffe);
      /*$EA63*/ CYCLES(0xea63, 6);
      /*$EA69*/ CYCLES(0xea69, 4);
                branchTarget = true; block_id = !s_status_not_z ? 1576 : 1542;
      break;
    case 1542:  // $EA6B
      /*$EA6B*/ CYCLES(0xea6b, 6);
                branchTarget = true; push16(0xea6d); block_id = 1588;
      break;
    case 1543:  // $EA6E
      /*$EA6E*/ CYCLES(0xea6e, 18);
                s_a = 0x00;
      /*$EA70*/ s_status_c = 0x01;
      /*$EA71*/ block_id = s_status_d ? 1545 : 1544;
      break;
    case 1544:  // $EA71
      /*$EA71*/ tmp6_U16 = s_a;
                tmp4_U16 = ram_peek(0x009d);
                tmp3_U16 = (tmp6_U16 - tmp4_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)(~tmp4_U16));
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1546;
      break;
    case 1545:  // $EA71
      /*$EA71*/ tmp3_U16 = sbc_dec16(s_a, ram_peek(0x009d), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1546;
      break;
    case 1546:  // $EA73
      /*$EA73*/ ram_poke(0x009d, s_a);
      /*$EA75*/ branchTarget = true; push16(0xea77); block_id = 1515;
      break;
    case 1547:  // $EA78
      /*$EA78*/ CYCLES(0xea78, 7);
                tmp5_U8 = (uint8_t)(ram_peek(0x009d) + 0x01);
                ram_poke(0x009d, tmp5_U8);
      /*$EA7A*/ branchTarget = true; block_id = !tmp5_U8 ? 1530 : 1548;
      break;
    case 1548:  // $EA7C
      /*$EA7C*/ CYCLES(0xea7c, 7);
                s_x = 0xfc;
      /*$EA7E*/ s_a = 0x01;
                block_id = 1549;
      break;
    case 1549:  // $EA80
      /*$EA80*/ CYCLES(0xea80, 11);
                tmp5_U8 = ram_peek(0x00a6);
      /*$EA82*/ tmp2_U8 = ram_peek(0x009e);
                tmp1_U8 = tmp5_U8 != tmp2_U8;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp5_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp5_U8 - tmp2_U8) & 0x80);
      /*$EA84*/ branchTarget = true; block_id = tmp1_U8 ? 1553 : 1550;
      break;
    case 1550:  // $EA86
      /*$EA86*/ CYCLES(0xea86, 11);
                tmp1_U8 = ram_peek(0x00a7);
      /*$EA88*/ tmp2_U8 = ram_peek(0x009f);
                tmp5_U8 = tmp1_U8 != tmp2_U8;
                s_status_not_z = tmp5_U8;
                s_status_c = (tmp1_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
      /*$EA8A*/ branchTarget = true; block_id = tmp5_U8 ? 1553 : 1551;
      break;
    case 1551:  // $EA8C
      /*$EA8C*/ CYCLES(0xea8c, 11);
                tmp5_U8 = ram_peek(0x00a8);
      /*$EA8E*/ tmp2_U8 = ram_peek(0x00a0);
                tmp1_U8 = tmp5_U8 != tmp2_U8;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp5_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp5_U8 - tmp2_U8) & 0x80);
      /*$EA90*/ branchTarget = true; block_id = tmp1_U8 ? 1553 : 1552;
      break;
    case 1552:  // $EA92
      /*$EA92*/ CYCLES(0xea92, 7);
                tmp1_U8 = ram_peek(0x00a9);
      /*$EA94*/ tmp2_U8 = ram_peek(0x00a1);
                s_status_not_z = (tmp1_U8 != tmp2_U8);
                s_status_c = (tmp1_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
                block_id = 1553;
      break;
    case 1553:  // $EA96
      /*$EA96*/ CYCLES(0xea96, 7);
                tmp1_U8 = s_status_c;
                push8((tmp1_U8 | ((s_status_not_z == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | s_status_n));
      /*$EA97*/ tmp3_U16 = (s_a << 0x01) | tmp1_U8;
                s_a = ((uint8_t)tmp3_U16);
      /*$EA98*/ branchTarget = true; block_id = !(uint8_t)(tmp3_U16 >> 8) ? 1557 : 1554;
      break;
    case 1554:  // $EA9A
      /*$EA9A*/ CYCLES(0xea9a, 9);
                tmp1_U8 = s_x;
                tmp2_U8 = (uint8_t)(tmp1_U8 + 0x01);
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$EA9B*/ ram_poke((uint8_t)(tmp1_U8 + 0x66), s_a);
      /*$EA9D*/ branchTarget = true; block_id = !tmp2_U8 ? 1574 : 1555;
      break;
    case 1555:  // $EA9F
      /*$EA9F*/ CYCLES(0xea9f, 4);
                branchTarget = true; block_id = !s_status_n ? 1575 : 1556;
      break;
    case 1556:  // $EAA1
      /*$EAA1*/ CYCLES(0xeaa1, 4);
                s_a = 0x01;
                block_id = 1557;
      break;
    case 1557:  // $EAA3
      /*$EAA3*/ CYCLES(0xeaa3, 6);
                tmp2_U8 = pop8();
                tmp1_U8 = tmp2_U8 & 0x01;
                s_status_c = tmp1_U8;
                s_status_i = ((tmp2_U8 & 0x04) != 0);
                s_status_d = ((tmp2_U8 & 0x08) != 0);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
      /*$EAA4*/ branchTarget = true; block_id = tmp1_U8 ? 1561 : 1558;
      break;
    case 1558:  // $EAA6
      /*$EAA6*/ CYCLES(0xeaa6, 18);
                tmp3_U16 = ram_peek(0x00a9) << 0x01;
                ram_poke(0x00a9, ((uint8_t)tmp3_U16));
      /*$EAA8*/ tmp3_U16 = (ram_peek(0x00a8) << 0x01) | (uint8_t)(tmp3_U16 >> 8);
                ram_poke(0x00a8, ((uint8_t)tmp3_U16));
      /*$EAAA*/ tmp3_U16 = (ram_peek(0x00a7) << 0x01) | (uint8_t)(tmp3_U16 >> 8);
                ram_poke(0x00a7, ((uint8_t)tmp3_U16));
      /*$EAAC*/ tmp3_U16 = (ram_peek(0x00a6) << 0x01) | (uint8_t)(tmp3_U16 >> 8);
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = tmp2_U8;
                tmp1_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x00a6, tmp1_U8);
      /*$EAAE*/ branchTarget = true; block_id = tmp2_U8 ? 1553 : 1559;
      break;
    case 1559:  // $EAB0
      /*$EAB0*/ CYCLES(0xeab0, 4);
                branchTarget = true; block_id = s_status_n ? 1549 : 1560;
      break;
    case 1560:  // $EAB2
      /*$EAB2*/ CYCLES(0xeab2, 4);
                branchTarget = true; block_id = !s_status_n ? 1553 : 1561;
      break;
    case 1561:  // $EAB4
      /*$EAB4*/ CYCLES(0xeab4, 50);
                s_y = s_a;
      /*$EAB5*/ s_a = ram_peek(0x00a9);
      /*$EAB7*/ block_id = s_status_d ? 1563 : 1562;
      break;
    case 1562:  // $EAB7
      /*$EAB7*/ tmp3_U16 = (s_a - ram_peek(0x00a1)) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1564;
      break;
    case 1563:  // $EAB7
      /*$EAB7*/ tmp3_U16 = sbc_dec16(s_a, ram_peek(0x00a1), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 1564;
      break;
    case 1564:  // $EAB9
      /*$EAB9*/ ram_poke(0x00a9, s_a);
      /*$EABB*/ s_a = ram_peek(0x00a8);
      /*$EABD*/ block_id = s_status_d ? 1566 : 1565;
      break;
    case 1565:  // $EABD
      /*$EABD*/ tmp3_U16 = (s_a - ram_peek(0x00a0)) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1567;
      break;
    case 1566:  // $EABD
      /*$EABD*/ tmp3_U16 = sbc_dec16(s_a, ram_peek(0x00a0), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 1567;
      break;
    case 1567:  // $EABF
      /*$EABF*/ ram_poke(0x00a8, s_a);
      /*$EAC1*/ s_a = ram_peek(0x00a7);
      /*$EAC3*/ block_id = s_status_d ? 1569 : 1568;
      break;
    case 1568:  // $EAC3
      /*$EAC3*/ tmp3_U16 = (s_a - ram_peek(0x009f)) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1570;
      break;
    case 1569:  // $EAC3
      /*$EAC3*/ tmp3_U16 = sbc_dec16(s_a, ram_peek(0x009f), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 1570;
      break;
    case 1570:  // $EAC5
      /*$EAC5*/ ram_poke(0x00a7, s_a);
      /*$EAC7*/ s_a = ram_peek(0x00a6);
      /*$EAC9*/ block_id = s_status_d ? 1572 : 1571;
      break;
    case 1571:  // $EAC9
      /*$EAC9*/ tmp3_U16 = s_a;
                tmp4_U16 = ram_peek(0x009e);
                tmp6_U16 = (tmp3_U16 - tmp4_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp4_U16));
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1573;
      break;
    case 1572:  // $EAC9
      /*$EAC9*/ tmp6_U16 = sbc_dec16(s_a, ram_peek(0x009e), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 1573;
      break;
    case 1573:  // $EACB
      /*$EACB*/ ram_poke(0x00a6, s_a);
      /*$EACD*/ s_a = s_y;
      /*$EACE*/ branchTarget = true; block_id = 1558;
      break;
    case 1574:  // $EAD1
      /*$EAD1*/ CYCLES(0xead1, 7);
                s_a = 0x40;
      /*$EAD3*/ branchTarget = true; block_id = 1557;
      break;
    case 1575:  // $EAD5
      /*$EAD5*/ CYCLES(0xead5, 21);
      /*$EADB*/ ram_poke(0x00ac, (uint8_t)(s_a << 0x06));
      /*$EADD*/ tmp2_U8 = pop8();
                s_status_i = ((tmp2_U8 & 0x04) != 0);
                s_status_d = ((tmp2_U8 & 0x08) != 0);
                s_status_b = 0x00;
      /*$EADE*/ branchTarget = true; block_id = 1577;
      break;
    case 1576:  // $EAE1
      /*$EAE1*/ CYCLES(0xeae1, 9);
                s_x = 0x85;
      /*$EAE3*/ branchTarget = true; block_id = 641;
      break;
    case 1577:  // $EAE6
      /*$EAE6*/ CYCLES(0xeae6, 33);
      /*$EAE8*/ ram_poke(0x009e, ram_peek(0x0062));
      /*$EAEC*/ ram_poke(0x009f, ram_peek(0x0063));
      /*$EAF0*/ ram_poke(0x00a0, ram_peek(0x0064));
      /*$EAF4*/ ram_poke(0x00a1, ram_peek(0x0065));
      /*$EAF6*/ branchTarget = true; block_id = 1429;
      break;
    case 1578:  // $EAF9
      /*$EAF9*/ CYCLES(0xeaf9, 63);
                ram_poke(0x005e, s_a);
      /*$EAFB*/ ram_poke(0x005f, s_y);
      /*$EAFF*/ tmp1_U8 = peek((ram_peek16al(0x005e) + 0x0004));
      /*$EB01*/ ram_poke(0x00a1, tmp1_U8);
      /*$EB04*/ tmp1_U8 = peek((ram_peek16al(0x005e) + 0x0003));
      /*$EB06*/ ram_poke(0x00a0, tmp1_U8);
      /*$EB09*/ tmp1_U8 = peek((ram_peek16al(0x005e) + 0x0002));
      /*$EB0B*/ ram_poke(0x009f, tmp1_U8);
      /*$EB0E*/ tmp1_U8 = peek((ram_peek16al(0x005e) + 0x0001));
      /*$EB10*/ ram_poke(0x00a2, tmp1_U8);
      /*$EB14*/ ram_poke(0x009e, (tmp1_U8 | 0x80));
      /*$EB16*/ s_y = 0x00;
      /*$EB17*/ tmp1_U8 = peek(ram_peek16al(0x005e));
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$EB19*/ ram_poke(0x009d, tmp1_U8);
      /*$EB1B*/ ram_poke(0x00ac, 0x00);
      /*$EB1D*/ branchTarget = true; block_id = find_block_id_func_t001(0xeb1d, pop16() + 1);;
      break;
    case 1579:  // $EB2B
      /*$EB2B*/ CYCLES(0xeb2b, 6);
                branchTarget = true; push16(0xeb2d); block_id = 1588;
      break;
    case 1580:  // $EB2E
      /*$EB2E*/ CYCLES(0xeb2e, 63);
                ram_poke(0x005e, s_x);
      /*$EB30*/ ram_poke(0x005f, s_y);
      /*$EB36*/ poke((ram_peek16al(0x005e) + 0x0004), ram_peek(0x00a1));
      /*$EB3B*/ poke((ram_peek16al(0x005e) + 0x0003), ram_peek(0x00a0));
      /*$EB40*/ poke((ram_peek16al(0x005e) + 0x0002), ram_peek(0x009f));
      /*$EB49*/ poke((ram_peek16al(0x005e) + 0x0001), ((ram_peek(0x00a2) | 0x7f) & ram_peek(0x009e)));
      /*$EB4B*/ s_y = 0x00;
      /*$EB4C*/ tmp2_U8 = ram_peek(0x009d);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$EB4E*/ poke(ram_peek16al(0x005e), tmp2_U8);
      /*$EB50*/ ram_poke(0x00ac, 0x00);
      /*$EB52*/ branchTarget = true; block_id = find_block_id_func_t001(0xeb52, pop16() + 1);;
      break;
    case 1581:  // $EB59
      /*$EB59*/ CYCLES(0xeb59, 12);
                tmp2_U8 = s_x;
                tmp1_U8 = ram_peek((uint8_t)(0xa4 + tmp2_U8));
                s_a = tmp1_U8;
      /*$EB5B*/ ram_poke((uint8_t)(0x9c + tmp2_U8), tmp1_U8);
      /*$EB5D*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$EB5E*/ branchTarget = true; block_id = tmp2_U8 ? 1581 : 1582;
      break;
    case 1582:  // $EB60
      /*$EB60*/ CYCLES(0xeb60, 6);
                ram_poke(0x00ac, s_x);
      /*$EB62*/ branchTarget = true; block_id = find_block_id_func_t001(0xeb62, pop16() + 1);;
      break;
    case 1583:  // $EB63
      /*$EB63*/ CYCLES(0xeb63, 6);
                branchTarget = true; push16(0xeb65); block_id = 1588;
      break;
    case 1584:  // $EB66
      /*$EB66*/ CYCLES(0xeb66, 4);
                s_x = 0x06;
                block_id = 1585;
      break;
    case 1585:  // $EB68
      /*$EB68*/ CYCLES(0xeb68, 12);
                tmp2_U8 = s_x;
                tmp1_U8 = ram_peek((uint8_t)(0x9c + tmp2_U8));
                s_a = tmp1_U8;
      /*$EB6A*/ ram_poke((uint8_t)(0xa4 + tmp2_U8), tmp1_U8);
      /*$EB6C*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$EB6D*/ branchTarget = true; block_id = tmp2_U8 ? 1585 : 1586;
      break;
    case 1586:  // $EB6F
      /*$EB6F*/ CYCLES(0xeb6f, 4);
                ram_poke(0x00ac, s_x);
                block_id = 1587;
      break;
    case 1587:  // $EB71
      /*$EB71*/ CYCLES(0xeb71, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xeb71, pop16() + 1);;
      break;
    case 1588:  // $EB72
      /*$EB72*/ CYCLES(0xeb72, 7);
                tmp2_U8 = ram_peek(0x009d);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$EB74*/ branchTarget = true; block_id = !tmp2_U8 ? 1587 : 1589;
      break;
    case 1589:  // $EB76
      /*$EB76*/ CYCLES(0xeb76, 7);
                tmp6_U16 = ram_peek(0x00ac) << 0x01;
                tmp2_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = tmp2_U8;
                tmp1_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x00ac, tmp1_U8);
      /*$EB78*/ branchTarget = true; block_id = !tmp2_U8 ? 1587 : 1590;
      break;
    case 1590:  // $EB7A
      /*$EB7A*/ CYCLES(0xeb7a, 6);
                FUNC_INCFACMAN(0xfffe);
      /*$EB7D*/ CYCLES(0xeb7d, 4);
                branchTarget = true; block_id = s_status_not_z ? 1587 : 1591;
      break;
    case 1591:  // $EB7F
      /*$EB7F*/ CYCLES(0xeb7f, 6);
                branchTarget = true; block_id = 1467;
      break;
    case 1592:  // $EB82
      /*$EB82*/ CYCLES(0xeb82, 7);
                tmp2_U8 = ram_peek(0x009d);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$EB84*/ branchTarget = true; block_id = !tmp2_U8 ? 1596 : 1593;
      break;
    case 1593:  // $EB86
      /*$EB86*/ CYCLES(0xeb86, 4);
                s_a = ram_peek(0x00a2);
                block_id = 1594;
      break;
    case 1594:  // $EB88
      /*$EB88*/ CYCLES(0xeb88, 9);
                tmp2_U8 = (uint8_t)(((s_a << 0x01) | s_status_c) >> 8);
                s_status_c = tmp2_U8;
      /*$EB89*/ s_status_not_z = 0xff;
                s_status_n = 0x80;
                s_a = 0xff;
      /*$EB8B*/ branchTarget = true; block_id = tmp2_U8 ? 1596 : 1595;
      break;
    case 1595:  // $EB8D
      /*$EB8D*/ CYCLES(0xeb8d, 4);
                s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_a = 0x01;
                block_id = 1596;
      break;
    case 1596:  // $EB8F
      /*$EB8F*/ CYCLES(0xeb8f, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xeb8f, pop16() + 1);;
      break;
    case 1597:  // $EB93
      /*$EB93*/ CYCLES(0xeb93, 14);
                ram_poke(0x009e, s_a);
      /*$EB97*/ ram_poke(0x009f, 0x00);
      /*$EB99*/ s_x = 0x88;
                block_id = 1598;
      break;
    case 1598:  // $EB9B
      /*$EB9B*/ CYCLES(0xeb9b, 9);
      /*$EB9F*/ s_status_c = (uint8_t)((((ram_peek(0x009e) ^ 0xff) << 0x01) | s_status_c) >> 8);
                block_id = 1599;
      break;
    case 1599:  // $EBA0
      /*$EBA0*/ CYCLES(0xeba0, 26);
      /*$EBA2*/ ram_poke(0x00a1, 0x00);
      /*$EBA4*/ ram_poke(0x00a0, 0x00);
      /*$EBA6*/ ram_poke(0x009d, s_x);
      /*$EBA8*/ ram_poke(0x00ac, 0x00);
      /*$EBAA*/ ram_poke(0x00a2, 0x00);
      /*$EBAC*/ branchTarget = true; block_id = 1427;
      break;
    case 1600:  // $EBB2
      /*$EBB2*/ CYCLES(0xebb2, 21);
                ram_poke(0x0060, s_a);
      /*$EBB4*/ ram_poke(0x0061, s_y);
      /*$EBB8*/ tmp5_U8 = peek(ram_peek16al(0x0060));
      /*$EBBA*/ s_y = 0x01;
      /*$EBBB*/ s_x = tmp5_U8;
      /*$EBBC*/ branchTarget = true; block_id = !tmp5_U8 ? 1592 : 1601;
      break;
    case 1601:  // $EBBE
      /*$EBBE*/ CYCLES(0xebbe, 11);
                tmp2_U8 = peek((ram_peek16al(0x0060) + s_y));
      /*$EBC2*/ branchTarget = true; block_id = ((tmp2_U8 ^ ram_peek(0x00a2)) & 0x80) ? 1593 : 1602;
      break;
    case 1602:  // $EBC4
      /*$EBC4*/ CYCLES(0xebc4, 7);
                tmp2_U8 = s_x;
                tmp1_U8 = ram_peek(0x009d);
                s_status_c = (tmp2_U8 >= tmp1_U8);
      /*$EBC6*/ branchTarget = true; block_id = (tmp2_U8 != tmp1_U8) ? 1610 : 1603;
      break;
    case 1603:  // $EBC8
      /*$EBC8*/ CYCLES(0xebc8, 14);
                tmp1_U8 = peek((ram_peek16al(0x0060) + s_y));
      /*$EBCA*/ tmp1_U8 = tmp1_U8 | 0x80;
      /*$EBCC*/ tmp2_U8 = ram_peek(0x009e);
                s_status_c = (tmp1_U8 >= tmp2_U8);
      /*$EBCE*/ branchTarget = true; block_id = (tmp1_U8 != tmp2_U8) ? 1610 : 1604;
      break;
    case 1604:  // $EBD0
      /*$EBD0*/ CYCLES(0xebd0, 12);
                tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$EBD1*/ tmp2_U8 = peek((ram_peek16al(0x0060) + tmp2_U8));
      /*$EBD3*/ tmp1_U8 = ram_peek(0x009f);
                s_status_c = (tmp2_U8 >= tmp1_U8);
      /*$EBD5*/ branchTarget = true; block_id = (tmp2_U8 != tmp1_U8) ? 1610 : 1605;
      break;
    case 1605:  // $EBD7
      /*$EBD7*/ CYCLES(0xebd7, 12);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$EBD8*/ tmp1_U8 = peek((ram_peek16al(0x0060) + tmp1_U8));
      /*$EBDA*/ tmp2_U8 = ram_peek(0x00a0);
                s_status_c = (tmp1_U8 >= tmp2_U8);
      /*$EBDC*/ branchTarget = true; block_id = (tmp1_U8 != tmp2_U8) ? 1610 : 1606;
      break;
    case 1606:  // $EBDE
      /*$EBDE*/ CYCLES(0xebde, 19);
                tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$EBE1*/ s_status_c = (0x7f >= ram_peek(0x00ac));
      /*$EBE3*/ tmp2_U8 = peek((ram_peek16al(0x0060) + tmp2_U8));
                s_a = tmp2_U8;
      /*$EBE5*/ block_id = s_status_d ? 1608 : 1607;
      break;
    case 1607:  // $EBE5
      /*$EBE5*/ tmp6_U16 = s_a;
                tmp4_U16 = ram_peek(0x00a1);
                tmp3_U16 = (tmp6_U16 - tmp4_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)(~tmp4_U16));
                tmp2_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 1609;
      break;
    case 1608:  // $EBE5
      /*$EBE5*/ tmp3_U16 = sbc_dec16(s_a, ram_peek(0x00a1), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 1609;
      break;
    case 1609:  // $EBE7
      /*$EBE7*/ branchTarget = true; block_id = !s_status_not_z ? 1622 : 1610;
      break;
    case 1610:  // $EBE9
      /*$EBE9*/ CYCLES(0xebe9, 7);
                s_a = ram_peek(0x00a2);
      /*$EBEB*/ branchTarget = true; block_id = !s_status_c ? 1612 : 1611;
      break;
    case 1611:  // $EBED
      /*$EBED*/ CYCLES(0xebed, 4);
                s_a = (s_a ^ 0xff);
                block_id = 1612;
      break;
    case 1612:  // $EBEF
      /*$EBEF*/ CYCLES(0xebef, 6);
                branchTarget = true; block_id = 1594;
      break;
    case 1613:  // $EBF2
      /*$EBF2*/ CYCLES(0xebf2, 7);
                tmp5_U8 = ram_peek(0x009d);
                s_a = tmp5_U8;
      /*$EBF4*/ branchTarget = true; block_id = !tmp5_U8 ? 1624 : 1614;
      break;
    case 1614:  // $EBF6
      /*$EBF6*/ CYCLES(0xebf6, 12);
                s_status_c = 0x01;
      /*$EBF7*/ block_id = s_status_d ? 1616 : 1615;
      break;
    case 1615:  // $EBF7
      /*$EBF7*/ s_a = (uint8_t)((s_a - 0x00a0) - (uint8_t)(0x01 - s_status_c));
                block_id = 1617;
      break;
    case 1616:  // $EBF7
      /*$EBF7*/ s_a = ((uint8_t)sbc_dec16(s_a, 0xa0, s_status_c));
                block_id = 1617;
      break;
    case 1617:  // $EBF9
      /*$EBF9*/ tmp2_U8 = ram_peek(0x00a2);
                s_status_v = ((tmp2_U8 >> 0x06) & 0x01);
      /*$EBFB*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1619 : 1618;
      break;
    case 1618:  // $EBFD
      /*$EBFD*/ CYCLES(0xebfd, 14);
                tmp2_U8 = s_a;
      /*$EC00*/ ram_poke(0x00a4, 0xff);
      /*$EC02*/ FUNC_TCFACMANT(0xfffe);
      /*$EC05*/ CYCLES(0xec05, 2);
                s_a = tmp2_U8;
                block_id = 1619;
      break;
    case 1619:  // $EC06
      /*$EC06*/ CYCLES(0xec06, 11);
                s_x = 0x9d;
      /*$EC08*/ tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0xf9);
      /*$EC0A*/ branchTarget = true; block_id = !((uint8_t)(tmp2_U8 - 0xf9) & 0x80) ? 1623 : 1620;
      break;
    case 1620:  // $EC0C
      /*$EC0C*/ CYCLES(0xec0c, 6);
                FUNC_SHFTRGHT(0xfffe);
                branchTarget = true; block_id = 1621;
      break;
    case 1621:  // $EC0F
      /*$EC0F*/ CYCLES(0xec0f, 4);
                ram_poke(0x00a4, s_y);
                block_id = 1622;
      break;
    case 1622:  // $EC11
      /*$EC11*/ CYCLES(0xec11, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xec11, pop16() + 1);;
      break;
    case 1623:  // $EC12
      /*$EC12*/ CYCLES(0xec12, 24);
                s_y = s_a;
      /*$EC17*/ tmp2_U8 = ram_peek(0x009e);
                s_status_c = (tmp2_U8 & 0x01);
                ram_poke(0x009e, (tmp2_U8 >> 0x01));
      /*$EC19*/ tmp2_U8 = (ram_peek(0x00a2) & 0x80) | ram_peek(0x009e);
                s_a = tmp2_U8;
      /*$EC1B*/ ram_poke(0x009e, tmp2_U8);
      /*$EC1D*/ FUNC_SHFTRGHT4(0xfffe);
      /*$EC20*/ CYCLES(0xec20, 6);
                ram_poke(0x00a4, s_y);
      /*$EC22*/ branchTarget = true; block_id = find_block_id_func_t001(0xec22, pop16() + 1);;
      break;
    case 1624:  // $EC40
      /*$EC40*/ CYCLES(0xec40, 18);
                tmp2_U8 = s_a;
                ram_poke(0x009e, tmp2_U8);
      /*$EC42*/ ram_poke(0x009f, tmp2_U8);
      /*$EC44*/ ram_poke(0x00a0, tmp2_U8);
      /*$EC46*/ ram_poke(0x00a1, tmp2_U8);
      /*$EC48*/ s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_y = tmp2_U8;
      /*$EC49*/ branchTarget = true; block_id = find_block_id_func_t001(0xec49, pop16() + 1);;
      break;
    case 1625:  // $EC4E
      /*$EC4E*/ CYCLES(0xec4e, 9);
                tmp2_U8 = s_x;
                ram_poke((uint8_t)(0x99 + tmp2_U8), s_y);
      /*$EC50*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_x = tmp2_U8;
      /*$EC51*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1625 : 1626;
      break;
    case 1626:  // $EC53
      /*$EC53*/ CYCLES(0xec53, 4);
                branchTarget = true; block_id = !s_status_c ? 1631 : 1627;
      break;
    case 1627:  // $EC55
      /*$EC55*/ CYCLES(0xec55, 7);
                tmp2_U8 = s_a;
                tmp1_U8 = tmp2_U8 != 0x2d;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp2_U8 >= 0x2d);
      /*$EC57*/ branchTarget = true; block_id = tmp1_U8 ? 1629 : 1628;
      break;
    case 1628:  // $EC59
      /*$EC59*/ CYCLES(0xec59, 7);
                ram_poke(0x00a3, s_x);
      /*$EC5B*/ branchTarget = true; block_id = !s_status_not_z ? 1630 : 1629;
      break;
    case 1629:  // $EC5D
      /*$EC5D*/ CYCLES(0xec5d, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x2b);
      /*$EC5F*/ branchTarget = true; block_id = (tmp1_U8 != 0x2b) ? 1632 : 1630;
      break;
    case 1630:  // $EC61
      /*$EC61*/ CYCLES(0xec61, 6);
                branchTarget = true; push16(0xec63); block_id = 2;
      break;
    case 1631:  // $EC64
      /*$EC64*/ CYCLES(0xec64, 4);
                branchTarget = true; block_id = !s_status_c ? 1664 : 1632;
      break;
    case 1632:  // $EC66
      /*$EC66*/ CYCLES(0xec66, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x2e);
      /*$EC68*/ branchTarget = true; block_id = !(tmp1_U8 != 0x2e) ? 1649 : 1633;
      break;
    case 1633:  // $EC6A
      /*$EC6A*/ CYCLES(0xec6a, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x45);
      /*$EC6C*/ branchTarget = true; block_id = (tmp1_U8 != 0x45) ? 1650 : 1634;
      break;
    case 1634:  // $EC6E
      /*$EC6E*/ CYCLES(0xec6e, 6);
                branchTarget = true; push16(0xec70); block_id = 2;
      break;
    case 1635:  // $EC71
      /*$EC71*/ CYCLES(0xec71, 4);
                branchTarget = true; block_id = !s_status_c ? 1643 : 1636;
      break;
    case 1636:  // $EC73
      /*$EC73*/ CYCLES(0xec73, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0xc9);
      /*$EC75*/ branchTarget = true; block_id = !(tmp1_U8 != 0xc9) ? 1641 : 1637;
      break;
    case 1637:  // $EC77
      /*$EC77*/ CYCLES(0xec77, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x2d);
      /*$EC79*/ branchTarget = true; block_id = !(tmp1_U8 != 0x2d) ? 1641 : 1638;
      break;
    case 1638:  // $EC7B
      /*$EC7B*/ CYCLES(0xec7b, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0xc8);
      /*$EC7D*/ branchTarget = true; block_id = !(tmp1_U8 != 0xc8) ? 1642 : 1639;
      break;
    case 1639:  // $EC7F
      /*$EC7F*/ CYCLES(0xec7f, 7);
                tmp1_U8 = s_a;
                tmp2_U8 = tmp1_U8 != 0x2b;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp1_U8 >= 0x2b);
      /*$EC81*/ branchTarget = true; block_id = !tmp2_U8 ? 1642 : 1640;
      break;
    case 1640:  // $EC83
      /*$EC83*/ CYCLES(0xec83, 4);
                branchTarget = true; block_id = s_status_not_z ? 1644 : 1641;
      break;
    case 1641:  // $EC85
      /*$EC85*/ CYCLES(0xec85, 4);
                tmp1_U8 = ram_peek(0x009c);
                ram_poke(0x009c, (uint8_t)((tmp1_U8 | (0x00 + (s_status_c << 8))) >> 0x01));
                s_status_c = (tmp1_U8 & 0x01);
                block_id = 1642;
      break;
    case 1642:  // $EC87
      /*$EC87*/ CYCLES(0xec87, 6);
                branchTarget = true; push16(0xec89); block_id = 2;
      break;
    case 1643:  // $EC8A
      /*$EC8A*/ CYCLES(0xec8a, 4);
                branchTarget = true; block_id = !s_status_c ? 1675 : 1644;
      break;
    case 1644:  // $EC8C
      /*$EC8C*/ CYCLES(0xec8c, 7);
      /*$EC8E*/ branchTarget = true; block_id = !(ram_peek(0x009c) & 0x80) ? 1650 : 1645;
      break;
    case 1645:  // $EC90
      /*$EC90*/ CYCLES(0xec90, 14);
                s_a = 0x00;
      /*$EC92*/ s_status_c = 0x01;
      /*$EC93*/ block_id = s_status_d ? 1647 : 1646;
      break;
    case 1646:  // $EC93
      /*$EC93*/ s_a = (uint8_t)((s_a - ram_peek(0x009a)) - (uint8_t)(0x01 - s_status_c));
                block_id = 1648;
      break;
    case 1647:  // $EC93
      /*$EC93*/ s_a = ((uint8_t)sbc_dec16(s_a, ram_peek(0x009a), s_status_c));
                block_id = 1648;
      break;
    case 1648:  // $EC95
      /*$EC95*/ branchTarget = true; block_id = 1651;
      break;
    case 1649:  // $EC98
      /*$EC98*/ CYCLES(0xec98, 11);
                tmp1_U8 = ram_peek(0x009b);
                ram_poke(0x009b, (uint8_t)((tmp1_U8 | (0x00 + (s_status_c << 8))) >> 0x01));
                s_status_c = (tmp1_U8 & 0x01);
      /*$EC9A*/ tmp1_U8 = (ram_peek(0x009b) >> 0x06) & 0x01;
                s_status_v = tmp1_U8;
      /*$EC9C*/ branchTarget = true; block_id = !tmp1_U8 ? 1630 : 1650;
      break;
    case 1650:  // $EC9E
      /*$EC9E*/ CYCLES(0xec9e, 4);
                s_a = ram_peek(0x009a);
                block_id = 1651;
      break;
    case 1651:  // $ECA0
      /*$ECA0*/ CYCLES(0xeca0, 12);
                s_status_c = 0x01;
      /*$ECA1*/ block_id = s_status_d ? 1653 : 1652;
      break;
    case 1652:  // $ECA1
      /*$ECA1*/ tmp3_U16 = s_a;
                tmp4_U16 = ram_peek(0x0099);
                tmp6_U16 = (tmp3_U16 - tmp4_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp4_U16));
                tmp2_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 1654;
      break;
    case 1653:  // $ECA1
      /*$ECA1*/ tmp6_U16 = sbc_dec16(s_a, ram_peek(0x0099), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp2_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 1654;
      break;
    case 1654:  // $ECA3
      /*$ECA3*/ ram_poke(0x009a, s_a);
      /*$ECA5*/ branchTarget = true; block_id = !s_status_not_z ? 1661 : 1655;
      break;
    case 1655:  // $ECA7
      /*$ECA7*/ CYCLES(0xeca7, 4);
                branchTarget = true; block_id = !s_status_n ? 1659 : 1656;
      break;
    case 1656:  // $ECA9
      /*$ECA9*/ CYCLES(0xeca9, 6);
                branchTarget = true; push16(0xecab); block_id = 1540;
      break;
    case 1657:  // $ECAC
      /*$ECAC*/ CYCLES(0xecac, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x009a) + 0x01);
                s_status_not_z = tmp1_U8;
                ram_poke(0x009a, tmp1_U8);
      /*$ECAE*/ branchTarget = true; block_id = tmp1_U8 ? 1656 : 1658;
      break;
    case 1658:  // $ECB0
      /*$ECB0*/ CYCLES(0xecb0, 4);
                branchTarget = true; block_id = !s_status_not_z ? 1661 : 1659;
      break;
    case 1659:  // $ECB2
      /*$ECB2*/ CYCLES(0xecb2, 6);
                branchTarget = true; push16(0xecb4); block_id = 1531;
      break;
    case 1660:  // $ECB5
      /*$ECB5*/ CYCLES(0xecb5, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x009a) - 0x01);
                ram_poke(0x009a, tmp1_U8);
      /*$ECB7*/ branchTarget = true; block_id = tmp1_U8 ? 1659 : 1661;
      break;
    case 1661:  // $ECB9
      /*$ECB9*/ CYCLES(0xecb9, 7);
                tmp2_U8 = ram_peek(0x00a3);
                s_status_not_z = tmp2_U8;
                tmp1_U8 = tmp2_U8 & 0x80;
                s_status_n = tmp1_U8;
                s_a = tmp2_U8;
      /*$ECBB*/ branchTarget = true; block_id = tmp1_U8 ? 1663 : 1662;
      break;
    case 1662:  // $ECBD
      /*$ECBD*/ CYCLES(0xecbd, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xecbd, pop16() + 1);;
      break;
    case 1663:  // $ECBE
      /*$ECBE*/ CYCLES(0xecbe, 6);
      /*$EED0*/ CYCLES(0xeed0, 7);
                tmp1_U8 = ram_peek(0x009d);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$EED2*/ branchTarget = true; block_id = !tmp1_U8 ? 1780 : 1779;
      break;
    case 1664:  // $ECC1
      /*$ECC1*/ CYCLES(0xecc1, 9);
                push8(s_a);
      /*$ECC2*/ tmp1_U8 = ram_peek(0x009b);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
      /*$ECC4*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 1666 : 1665;
      break;
    case 1665:  // $ECC6
      /*$ECC6*/ CYCLES(0xecc6, 4);
                ram_poke(0x0099, (uint8_t)(ram_peek(0x0099) + 0x01));
                block_id = 1666;
      break;
    case 1666:  // $ECC8
      /*$ECC8*/ CYCLES(0xecc8, 6);
                branchTarget = true; push16(0xecca); block_id = 1531;
      break;
    case 1667:  // $ECCB
      /*$ECCB*/ CYCLES(0xeccb, 12);
                tmp5_U8 = pop8();
                s_a = tmp5_U8;
      /*$ECCC*/ s_status_c = 0x01;
      /*$ECCD*/ block_id = s_status_d ? 1669 : 1668;
      break;
    case 1668:  // $ECCD
      /*$ECCD*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 - 0x0030) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0xffcf);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1670;
      break;
    case 1669:  // $ECCD
      /*$ECCD*/ tmp4_U16 = sbc_dec16(s_a, 0x30, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 1670;
      break;
    case 1670:  // $ECCF
      /*$ECCF*/ branchTarget = true; push16(0xecd1); block_id = 1672;
      break;
    case 1671:  // $ECD2
      /*$ECD2*/ CYCLES(0xecd2, 6);
                branchTarget = true; block_id = 1630;
      break;
    case 1672:  // $ECD5
      /*$ECD5*/ CYCLES(0xecd5, 7);
                push8(s_a);
      /*$ECD6*/ branchTarget = true; push16(0xecd8); block_id = 1583;
      break;
    case 1673:  // $ECD9
      /*$ECD9*/ CYCLES(0xecd9, 7);
                tmp2_U8 = pop8();
                s_a = tmp2_U8;
      /*$ECDA*/ branchTarget = true; push16(0xecdc); block_id = 1597;
      break;
    case 1674:  // $ECDD
      /*$ECDD*/ CYCLES(0xecdd, 19);
      /*$ECE1*/ ram_poke(0x00ab, (ram_peek(0x00aa) ^ ram_peek(0x00a2)));
      /*$ECE3*/ s_status_not_z = ram_peek(0x009d);
      /*$ECE5*/ branchTarget = true; block_id = 1392;
      break;
    case 1675:  // $ECE8
      /*$ECE8*/ CYCLES(0xece8, 11);
                tmp2_U8 = ram_peek(0x009a);
                s_a = tmp2_U8;
      /*$ECEA*/ tmp2_U8 = tmp2_U8 >= 0x0a;
                s_status_c = tmp2_U8;
      /*$ECEC*/ branchTarget = true; block_id = !tmp2_U8 ? 1678 : 1676;
      break;
    case 1676:  // $ECEE
      /*$ECEE*/ CYCLES(0xecee, 11);
                s_a = 0x64;
      /*$ECF0*/ tmp1_U8 = ram_peek(0x009c);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
      /*$ECF2*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 1687 : 1677;
      break;
    case 1677:  // $ECF4
      /*$ECF4*/ CYCLES(0xecf4, 6);
                branchTarget = true; block_id = 1477;
      break;
    case 1678:  // $ECF7
      /*$ECF7*/ CYCLES(0xecf7, 24);
      /*$ECF8*/ s_a = (uint8_t)(s_a << 0x02);
      /*$ECF9*/ s_status_c = 0x00;
      /*$ECFA*/ block_id = s_status_d ? 1680 : 1679;
      break;
    case 1679:  // $ECFA
      /*$ECFA*/ s_a = (uint8_t)((s_a + ram_peek(0x009a)) + s_status_c);
                block_id = 1681;
      break;
    case 1680:  // $ECFA
      /*$ECFA*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x009a), s_status_c));
                block_id = 1681;
      break;
    case 1681:  // $ECFC
      /*$ECFC*/ s_a = (uint8_t)(s_a << 0x01);
      /*$ECFD*/ s_status_c = 0x00;
      /*$ECFE*/ s_y = 0x00;
      /*$ED00*/ block_id = s_status_d ? 1683 : 1682;
      break;
    case 1682:  // $ED00
      /*$ED00*/ tmp1_U8 = peek((ram_peek16al(0x00b8) + s_y));
                s_a = (uint8_t)((s_a + tmp1_U8) + s_status_c);
                block_id = 1684;
      break;
    case 1683:  // $ED00
      /*$ED00*/ tmp1_U8 = peek((ram_peek16al(0x00b8) + s_y));
                s_a = ((uint8_t)adc_dec16(s_a, tmp1_U8, s_status_c));
                block_id = 1684;
      break;
    case 1684:  // $ED02
      /*$ED02*/ s_status_c = 0x01;
      /*$ED03*/ block_id = s_status_d ? 1686 : 1685;
      break;
    case 1685:  // $ED03
      /*$ED03*/ tmp4_U16 = s_a;
                tmp6_U16 = (tmp4_U16 - 0x0030) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0xffcf);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1688;
      break;
    case 1686:  // $ED03
      /*$ED03*/ tmp6_U16 = sbc_dec16(s_a, 0x30, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 1688;
      break;
    case 1687:  // $ED05
      /*$ED05*/ CYCLES(0xed05, 9);
                ram_poke(0x009a, s_a);
      /*$ED07*/ branchTarget = true; block_id = 1642;
      break;
    case 1688:  // $ED05
      /*$ED03*/ block_id = 1687;
      break;
    case 1689:  // $ED19
      /*$ED19*/ CYCLES(0xed19, 12);
                s_a = 0x58;
      /*$ED1B*/ s_y = 0xd3;
      /*$ED1D*/ branchTarget = true; push16(0xed1f); block_id = 1693;
      break;
    case 1690:  // $ED20
      /*$ED20*/ CYCLES(0xed20, 7);
                s_a = ram_peek(0x0076);
      /*$ED22*/ s_x = ram_peek(0x0075);
                block_id = 1691;
      break;
    case 1691:  // $ED24
      /*$ED24*/ CYCLES(0xed24, 18);
                ram_poke(0x009e, s_a);
      /*$ED26*/ ram_poke(0x009f, s_x);
      /*$ED28*/ s_x = 0x90;
      /*$ED2A*/ s_status_c = 0x01;
      /*$ED2B*/ branchTarget = true; push16(0xed2d); block_id = 1599;
      break;
    case 1692:  // $ED2E
      /*$ED2E*/ CYCLES(0xed2e, 6);
                branchTarget = true; push16(0xed30); block_id = 1694;
      break;
    case 1693:  // $ED31
      /*$ED31*/ CYCLES(0xed31, 6);
                branchTarget = true; block_id = 918;
      break;
    case 1694:  // $ED34
      /*$ED34*/ CYCLES(0xed34, 16);
      /*$ED36*/ s_a = 0x2d;
      /*$ED38*/ s_y = 0x00;
      /*$ED39*/ tmp1_U8 = ram_peek(0x00a2);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
      /*$ED3B*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 1696 : 1695;
      break;
    case 1695:  // $ED3D
      /*$ED3D*/ CYCLES(0xed3d, 7);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$ED3E*/ ram_poke((0x00ff + tmp1_U8), s_a);
                block_id = 1696;
      break;
    case 1696:  // $ED41
      /*$ED41*/ CYCLES(0xed41, 19);
                ram_poke(0x00a2, s_a);
      /*$ED43*/ tmp1_U8 = s_y;
                ram_poke(0x00ad, tmp1_U8);
      /*$ED45*/ s_y = (uint8_t)(tmp1_U8 + 0x01);
      /*$ED46*/ s_a = 0x30;
      /*$ED48*/ tmp1_U8 = ram_peek(0x009d);
                s_x = tmp1_U8;
      /*$ED4A*/ branchTarget = true; block_id = tmp1_U8 ? 1698 : 1697;
      break;
    case 1697:  // $ED4C
      /*$ED4C*/ CYCLES(0xed4c, 6);
      /*$EE57*/ CYCLES(0xee57, 6);
                ram_poke((0x00ff + s_y), s_a);
                block_id = 1777;
      break;
    case 1698:  // $ED4F
      /*$ED4F*/ CYCLES(0xed4f, 11);
                s_a = 0x00;
      /*$ED51*/ tmp1_U8 = s_x;
                s_status_c = (tmp1_U8 >= 0x80);
      /*$ED53*/ branchTarget = true; block_id = !(tmp1_U8 != 0x80) ? 1700 : 1699;
      break;
    case 1699:  // $ED55
      /*$ED55*/ CYCLES(0xed55, 4);
                branchTarget = true; block_id = s_status_c ? 1702 : 1700;
      break;
    case 1700:  // $ED57
      /*$ED57*/ CYCLES(0xed57, 12);
                s_a = 0x14;
      /*$ED59*/ s_y = 0xed;
      /*$ED5B*/ branchTarget = true; push16(0xed5d); block_id = 1491;
      break;
    case 1701:  // $ED5E
      /*$ED5E*/ CYCLES(0xed5e, 4);
                s_a = 0xf7;
                block_id = 1702;
      break;
    case 1702:  // $ED60
      /*$ED60*/ CYCLES(0xed60, 4);
                ram_poke(0x0099, s_a);
                block_id = 1703;
      break;
    case 1703:  // $ED62
      /*$ED62*/ CYCLES(0xed62, 12);
                s_a = 0x0f;
      /*$ED64*/ s_y = 0xed;
      /*$ED66*/ FUNC_FCOMP(0xfffe);
      /*$ED69*/ CYCLES(0xed69, 4);
                branchTarget = true; block_id = !s_status_not_z ? 1712 : 1704;
      break;
    case 1704:  // $ED6B
      /*$ED6B*/ CYCLES(0xed6b, 4);
                branchTarget = true; block_id = !s_status_n ? 1709 : 1705;
      break;
    case 1705:  // $ED6D
      /*$ED6D*/ CYCLES(0xed6d, 12);
                s_a = 0x0a;
      /*$ED6F*/ s_y = 0xed;
      /*$ED71*/ FUNC_FCOMP(0xfffe);
      /*$ED74*/ CYCLES(0xed74, 4);
                branchTarget = true; block_id = !s_status_not_z ? 1707 : 1706;
      break;
    case 1706:  // $ED76
      /*$ED76*/ CYCLES(0xed76, 4);
                branchTarget = true; block_id = !s_status_n ? 1711 : 1707;
      break;
    case 1707:  // $ED78
      /*$ED78*/ CYCLES(0xed78, 6);
                branchTarget = true; push16(0xed7a); block_id = 1531;
      break;
    case 1708:  // $ED7B
      /*$ED7B*/ CYCLES(0xed7b, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x0099) - 0x01);
                ram_poke(0x0099, tmp1_U8);
      /*$ED7D*/ branchTarget = true; block_id = tmp1_U8 ? 1705 : 1709;
      break;
    case 1709:  // $ED7F
      /*$ED7F*/ CYCLES(0xed7f, 6);
                branchTarget = true; push16(0xed81); block_id = 1540;
      break;
    case 1710:  // $ED82
      /*$ED82*/ CYCLES(0xed82, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x0099) + 0x01);
                ram_poke(0x0099, tmp1_U8);
      /*$ED84*/ branchTarget = true; block_id = tmp1_U8 ? 1703 : 1711;
      break;
    case 1711:  // $ED86
      /*$ED86*/ CYCLES(0xed86, 6);
                branchTarget = true; push16(0xed88); block_id = 1388;
      break;
    case 1712:  // $ED89
      /*$ED89*/ CYCLES(0xed89, 6);
                FUNC_QINT(0xfffe);
      /*$ED8C*/ CYCLES(0xed8c, 16);
                s_x = 0x01;
      /*$ED8E*/ s_a = ram_peek(0x0099);
      /*$ED90*/ s_status_c = 0x00;
      /*$ED91*/ block_id = s_status_d ? 1714 : 1713;
      break;
    case 1713:  // $ED91
      /*$ED91*/ tmp1_U8 = (uint8_t)((s_a + 0x000a) + s_status_c);
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 1715;
      break;
    case 1714:  // $ED91
      /*$ED91*/ tmp6_U16 = adc_dec16(s_a, 0x0a, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_n = ((uint8_t)(tmp6_U16 >> 8) & 0x80);
                block_id = 1715;
      break;
    case 1715:  // $ED93
      /*$ED93*/ branchTarget = true; block_id = s_status_n ? 1721 : 1716;
      break;
    case 1716:  // $ED95
      /*$ED95*/ CYCLES(0xed95, 7);
                tmp1_U8 = s_a >= 0x0b;
                s_status_c = tmp1_U8;
      /*$ED97*/ branchTarget = true; block_id = tmp1_U8 ? 1722 : 1717;
      break;
    case 1717:  // $ED99
      /*$ED99*/ CYCLES(0xed99, 9);
                block_id = s_status_d ? 1719 : 1718;
      break;
    case 1718:  // $ED99
      /*$ED99*/ s_a = (uint8_t)((s_a + 0x00ff) + s_status_c);
                block_id = 1720;
      break;
    case 1719:  // $ED99
      /*$ED99*/ s_a = ((uint8_t)adc_dec16(s_a, 0xff, s_status_c));
                block_id = 1720;
      break;
    case 1720:  // $ED9B
      /*$ED9B*/ s_x = s_a;
      /*$ED9C*/ s_a = 0x02;
                block_id = 1721;
      break;
    case 1721:  // $ED9E
      /*$ED9E*/ CYCLES(0xed9e, 2);
                s_status_c = 0x01;
                block_id = 1722;
      break;
    case 1722:  // $ED9F
      /*$ED9F*/ CYCLES(0xed9f, 16);
                block_id = s_status_d ? 1724 : 1723;
      break;
    case 1723:  // $ED9F
      /*$ED9F*/ s_a = (uint8_t)((s_a - 0x0002) - (uint8_t)(0x01 - s_status_c));
                block_id = 1725;
      break;
    case 1724:  // $ED9F
      /*$ED9F*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x02, s_status_c));
                block_id = 1725;
      break;
    case 1725:  // $EDA1
      /*$EDA1*/ ram_poke(0x009a, s_a);
      /*$EDA3*/ tmp1_U8 = s_x;
                ram_poke(0x0099, tmp1_U8);
      /*$EDA5*/ s_status_n = (tmp1_U8 & 0x80);
      /*$EDA6*/ branchTarget = true; block_id = !tmp1_U8 ? 1727 : 1726;
      break;
    case 1726:  // $EDA8
      /*$EDA8*/ CYCLES(0xeda8, 4);
                branchTarget = true; block_id = !s_status_n ? 1730 : 1727;
      break;
    case 1727:  // $EDAA
      /*$EDAA*/ CYCLES(0xedaa, 19);
      /*$EDAE*/ tmp1_U8 = (uint8_t)(ram_peek(0x00ad) + 0x01);
                s_y = tmp1_U8;
      /*$EDAF*/ ram_poke((0x00ff + tmp1_U8), 0x2e);
      /*$EDB3*/ branchTarget = true; block_id = !s_x ? 1729 : 1728;
      break;
    case 1728:  // $EDB5
      /*$EDB5*/ CYCLES(0xedb5, 11);
      /*$EDB7*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$EDB8*/ ram_poke((0x00ff + tmp1_U8), 0x30);
                block_id = 1729;
      break;
    case 1729:  // $EDBB
      /*$EDBB*/ CYCLES(0xedbb, 4);
                ram_poke(0x00ad, s_y);
                block_id = 1730;
      break;
    case 1730:  // $EDBD
      /*$EDBD*/ CYCLES(0xedbd, 7);
                s_y = 0x00;
      /*$EDBF*/ s_x = 0x80;
                block_id = 1731;
      break;
    case 1731:  // $EDC1
      /*$EDC1*/ CYCLES(0xedc1, 55);
                s_a = ram_peek(0x00a1);
      /*$EDC3*/ s_status_c = 0x00;
      /*$EDC4*/ block_id = s_status_d ? 1733 : 1732;
      break;
    case 1732:  // $EDC4
      /*$EDC4*/ tmp6_U16 = (s_a + ram_peek((0xee6c + s_y))) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1734;
      break;
    case 1733:  // $EDC4
      /*$EDC4*/ tmp6_U16 = adc_dec16(s_a, ram_peek((0xee6c + s_y)), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1734;
      break;
    case 1734:  // $EDC7
      /*$EDC7*/ ram_poke(0x00a1, s_a);
      /*$EDC9*/ s_a = ram_peek(0x00a0);
      /*$EDCB*/ block_id = s_status_d ? 1736 : 1735;
      break;
    case 1735:  // $EDCB
      /*$EDCB*/ tmp6_U16 = (s_a + ram_peek((0xee6b + s_y))) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1737;
      break;
    case 1736:  // $EDCB
      /*$EDCB*/ tmp6_U16 = adc_dec16(s_a, ram_peek((0xee6b + s_y)), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1737;
      break;
    case 1737:  // $EDCE
      /*$EDCE*/ ram_poke(0x00a0, s_a);
      /*$EDD0*/ s_a = ram_peek(0x009f);
      /*$EDD2*/ block_id = s_status_d ? 1739 : 1738;
      break;
    case 1738:  // $EDD2
      /*$EDD2*/ tmp6_U16 = (s_a + ram_peek((0xee6a + s_y))) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1740;
      break;
    case 1739:  // $EDD2
      /*$EDD2*/ tmp6_U16 = adc_dec16(s_a, ram_peek((0xee6a + s_y)), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1740;
      break;
    case 1740:  // $EDD5
      /*$EDD5*/ ram_poke(0x009f, s_a);
      /*$EDD7*/ s_a = ram_peek(0x009e);
      /*$EDD9*/ block_id = s_status_d ? 1742 : 1741;
      break;
    case 1741:  // $EDD9
      /*$EDD9*/ tmp6_U16 = (s_a + ram_peek((0xee69 + s_y))) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1743;
      break;
    case 1742:  // $EDD9
      /*$EDD9*/ tmp6_U16 = adc_dec16(s_a, ram_peek((0xee69 + s_y)), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1743;
      break;
    case 1743:  // $EDDC
      /*$EDDC*/ ram_poke(0x009e, s_a);
      /*$EDDE*/ tmp1_U8 = (uint8_t)(s_x + 0x01);
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$EDDF*/ branchTarget = true; block_id = s_status_c ? 1746 : 1744;
      break;
    case 1744:  // $EDE1
      /*$EDE1*/ CYCLES(0xede1, 4);
                branchTarget = true; block_id = !s_status_n ? 1731 : 1745;
      break;
    case 1745:  // $EDE3
      /*$EDE3*/ CYCLES(0xede3, 4);
                branchTarget = true; block_id = s_status_n ? 1747 : 1746;
      break;
    case 1746:  // $EDE5
      /*$EDE5*/ CYCLES(0xede5, 4);
                branchTarget = true; block_id = s_status_n ? 1731 : 1747;
      break;
    case 1747:  // $EDE7
      /*$EDE7*/ CYCLES(0xede7, 6);
                s_a = s_x;
      /*$EDE8*/ branchTarget = true; block_id = !s_status_c ? 1751 : 1748;
      break;
    case 1748:  // $EDEA
      /*$EDEA*/ CYCLES(0xedea, 7);
                s_a = (s_a ^ 0xff);
      /*$EDEC*/ block_id = s_status_d ? 1750 : 1749;
      break;
    case 1749:  // $EDEC
      /*$EDEC*/ tmp6_U16 = (s_a + 0x000a) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1752;
      break;
    case 1750:  // $EDEC
      /*$EDEC*/ tmp6_U16 = adc_dec16(s_a, 0x0a, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1752;
      break;
    case 1751:  // $EDEE
      /*$EDEE*/ CYCLES(0xedee, 36);
                block_id = s_status_d ? 1754 : 1753;
      break;
    case 1752:  // $EDEE
      /*$EDEC*/ block_id = 1751;
      break;
    case 1753:  // $EDEE
      /*$EDEE*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 + 0x002f) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x002f);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1755;
      break;
    case 1754:  // $EDEE
      /*$EDEE*/ tmp4_U16 = adc_dec16(s_a, 0x2f, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 1755;
      break;
    case 1755:  // $EDF0
      /*$EDF4*/ ram_poke(0x0083, (uint8_t)(s_y + 0x04));
      /*$EDF8*/ tmp1_U8 = (uint8_t)(ram_peek(0x00ad) + 0x01);
                s_y = tmp1_U8;
      /*$EDF9*/ tmp2_U8 = s_a;
                s_x = tmp2_U8;
      /*$EDFC*/ ram_poke((0x00ff + tmp1_U8), (tmp2_U8 & 0x7f));
      /*$EDFF*/ tmp1_U8 = (uint8_t)(ram_peek(0x0099) - 0x01);
                ram_poke(0x0099, tmp1_U8);
      /*$EE01*/ branchTarget = true; block_id = tmp1_U8 ? 1757 : 1756;
      break;
    case 1756:  // $EE03
      /*$EE03*/ CYCLES(0xee03, 11);
      /*$EE05*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$EE06*/ ram_poke((0x00ff + tmp1_U8), 0x2e);
                block_id = 1757;
      break;
    case 1757:  // $EE09
      /*$EE09*/ CYCLES(0xee09, 24);
                ram_poke(0x00ad, s_y);
      /*$EE0B*/ tmp1_U8 = ram_peek(0x0083);
                s_y = tmp1_U8;
      /*$EE12*/ s_x = ((s_x ^ 0xff) & 0x80);
      /*$EE15*/ branchTarget = true; block_id = (tmp1_U8 != 0x24) ? 1731 : 1758;
      break;
    case 1758:  // $EE17
      /*$EE17*/ CYCLES(0xee17, 4);
                s_y = ram_peek(0x00ad);
                block_id = 1759;
      break;
    case 1759:  // $EE19
      /*$EE19*/ CYCLES(0xee19, 14);
                tmp1_U8 = s_y;
                tmp2_U8 = ram_peek((0x00ff + tmp1_U8));
                s_a = tmp2_U8;
      /*$EE1C*/ s_y = (uint8_t)(tmp1_U8 - 0x01);
      /*$EE1F*/ branchTarget = true; block_id = !(tmp2_U8 != 0x30) ? 1759 : 1760;
      break;
    case 1760:  // $EE21
      /*$EE21*/ CYCLES(0xee21, 7);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x2e);
      /*$EE23*/ branchTarget = true; block_id = !(tmp2_U8 != 0x2e) ? 1762 : 1761;
      break;
    case 1761:  // $EE25
      /*$EE25*/ CYCLES(0xee25, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 1762;
      break;
    case 1762:  // $EE26
      /*$EE26*/ CYCLES(0xee26, 11);
                s_a = 0x2b;
      /*$EE28*/ tmp2_U8 = ram_peek(0x009a);
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$EE2A*/ branchTarget = true; block_id = !tmp2_U8 ? 1777 : 1763;
      break;
    case 1763:  // $EE2C
      /*$EE2C*/ CYCLES(0xee2c, 4);
                branchTarget = true; block_id = !s_status_n ? 1768 : 1764;
      break;
    case 1764:  // $EE2E
      /*$EE2E*/ CYCLES(0xee2e, 14);
                s_a = 0x00;
      /*$EE30*/ s_status_c = 0x01;
      /*$EE31*/ block_id = s_status_d ? 1766 : 1765;
      break;
    case 1765:  // $EE31
      /*$EE31*/ s_a = (uint8_t)((s_a - ram_peek(0x009a)) - (uint8_t)(0x01 - s_status_c));
                block_id = 1767;
      break;
    case 1766:  // $EE31
      /*$EE31*/ s_a = ((uint8_t)sbc_dec16(s_a, ram_peek(0x009a), s_status_c));
                block_id = 1767;
      break;
    case 1767:  // $EE33
      /*$EE33*/ s_x = s_a;
      /*$EE34*/ s_a = 0x2d;
                block_id = 1768;
      break;
    case 1768:  // $EE36
      /*$EE36*/ CYCLES(0xee36, 21);
                tmp2_U8 = s_y;
                ram_poke((0x0101 + tmp2_U8), s_a);
      /*$EE3B*/ ram_poke((0x0100 + tmp2_U8), 0x45);
      /*$EE3E*/ s_a = s_x;
      /*$EE3F*/ s_x = 0x2f;
      /*$EE41*/ s_status_c = 0x01;
                block_id = 1769;
      break;
    case 1769:  // $EE42
      /*$EE42*/ CYCLES(0xee42, 9);
                s_x = (uint8_t)(s_x + 0x01);
      /*$EE43*/ block_id = s_status_d ? 1771 : 1770;
      break;
    case 1770:  // $EE43
      /*$EE43*/ tmp4_U16 = (s_a - 0x000a) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1772;
      break;
    case 1771:  // $EE43
      /*$EE43*/ tmp4_U16 = sbc_dec16(s_a, 0x0a, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1772;
      break;
    case 1772:  // $EE45
      /*$EE45*/ branchTarget = true; block_id = s_status_c ? 1769 : 1773;
      break;
    case 1773:  // $EE47
      /*$EE47*/ CYCLES(0xee47, 28);
                block_id = s_status_d ? 1775 : 1774;
      break;
    case 1774:  // $EE47
      /*$EE47*/ tmp4_U16 = s_a;
                tmp6_U16 = (tmp4_U16 + 0x003a) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0x003a);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1776;
      break;
    case 1775:  // $EE47
      /*$EE47*/ tmp6_U16 = adc_dec16(s_a, 0x3a, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp2_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 1776;
      break;
    case 1776:  // $EE49
      /*$EE49*/ tmp2_U8 = s_y;
                ram_poke((0x0103 + tmp2_U8), s_a);
      /*$EE4D*/ ram_poke((0x0102 + tmp2_U8), s_x);
      /*$EE52*/ ram_poke((0x0104 + tmp2_U8), 0x00);
      /*$EE55*/ branchTarget = true; block_id = 1778;
      break;
    case 1777:  // $EE5A
      /*$EE5A*/ CYCLES(0xee5a, 9);
      /*$EE5C*/ ram_poke((0x0100 + s_y), 0x00);
                block_id = 1778;
      break;
    case 1778:  // $EE5F
      /*$EE5F*/ CYCLES(0xee5f, 9);
                s_a = 0x00;
      /*$EE61*/ s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_y = 0x01;
      /*$EE63*/ branchTarget = true; block_id = find_block_id_func_t001(0xee63, pop16() + 1);;
      break;
    case 1779:  // $EED4
      /*$EED4*/ CYCLES(0xeed4, 11);
      /*$EED6*/ tmp2_U8 = ram_peek(0x00a2) ^ 0xff;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$EED8*/ ram_poke(0x00a2, tmp2_U8);
                block_id = 1780;
      break;
    case 1780:  // $EEDA
      /*$EEDA*/ CYCLES(0xeeda, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xeeda, pop16() + 1);;
      break;
    case 1781:  // $F13E
      /*$F13E*/ CYCLES(0xf13e, 35);
      /*$F140*/ ram_poke(0x0000, 0x4c);
      /*$F142*/ ram_poke(0x0003, 0x4c);
      /*$F144*/ ram_poke(0x0090, 0x4c);
      /*$F146*/ ram_poke(0x000a, 0x4c);
      /*$F14A*/ s_y = 0xe1;
      /*$F14C*/ ram_poke(0x000b, 0x99);
      /*$F14E*/ ram_poke(0x000c, 0xe1);
      /*$F150*/ s_x = 0x1c;
                block_id = 1782;
      break;
    case 1782:  // $F152
      /*$F152*/ CYCLES(0xf152, 18);
                tmp2_U8 = s_x;
      /*$F155*/ ram_poke((uint8_t)(0xb0 + tmp2_U8), ram_peek((0xf10a + tmp2_U8)));
      /*$F157*/ ram_poke(0x00f1, tmp2_U8);
      /*$F159*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_x = tmp2_U8;
      /*$F15A*/ branchTarget = true; block_id = tmp2_U8 ? 1782 : 1783;
      break;
    case 1783:  // $F15C
      /*$F15C*/ CYCLES(0xf15c, 26);
                tmp2_U8 = s_x;
                ram_poke(0x00f2, tmp2_U8);
      /*$F15F*/ ram_poke(0x00a4, tmp2_U8);
      /*$F161*/ ram_poke(0x0054, tmp2_U8);
      /*$F163*/ push8(tmp2_U8);
      /*$F166*/ ram_poke(0x008f, 0x03);
      /*$F168*/ FUNC_CRDO(0xfffe);
                branchTarget = true; block_id = 1784;
      break;
    case 1784:  // $F16B
      /*$F16B*/ CYCLES(0xf16b, 38);
      /*$F16D*/ ram_poke(0x01fd, 0x01);
      /*$F170*/ ram_poke(0x01fc, 0x01);
      /*$F175*/ ram_poke(0x0052, 0x55);
      /*$F17B*/ ram_poke(0x0050, 0x00);
      /*$F17D*/ ram_poke(0x0051, 0x08);
      /*$F17F*/ s_y = 0x00;
                block_id = 1785;
      break;
    case 1785:  // $F181
      /*$F181*/ CYCLES(0xf181, 21);
                ram_poke(0x0051, (uint8_t)(ram_peek(0x0051) + 0x01));
      /*$F183*/ tmp2_U8 = s_y;
                tmp1_U8 = peek((ram_peek16al(0x0050) + tmp2_U8));
      /*$F185*/ tmp1_U8 = tmp1_U8 ^ 0xff;
                s_a = tmp1_U8;
      /*$F187*/ poke((ram_peek16al(0x0050) + tmp2_U8), tmp1_U8);
      /*$F189*/ tmp2_U8 = peek((ram_peek16al(0x0050) + tmp2_U8));
                s_status_c = (tmp1_U8 >= tmp2_U8);
      /*$F18B*/ branchTarget = true; block_id = (tmp1_U8 != tmp2_U8) ? 1787 : 1786;
      break;
    case 1786:  // $F18D
      /*$F18D*/ CYCLES(0xf18d, 14);
                tmp1_U8 = s_a ^ 0xff;
      /*$F18F*/ tmp2_U8 = s_y;
                poke((ram_peek16al(0x0050) + tmp2_U8), tmp1_U8);
      /*$F191*/ tmp2_U8 = peek((ram_peek16al(0x0050) + tmp2_U8));
                s_status_c = (tmp1_U8 >= tmp2_U8);
      /*$F193*/ branchTarget = true; block_id = !(tmp1_U8 != tmp2_U8) ? 1785 : 1787;
      break;
    case 1787:  // $F195
      /*$F195*/ CYCLES(0xf195, 57);
                tmp2_U8 = ram_peek(0x0050);
      /*$F199*/ tmp1_U8 = ram_peek(0x0051) & 0xf0;
      /*$F19B*/ ram_poke(0x0073, tmp2_U8);
      /*$F19D*/ ram_poke(0x0074, tmp1_U8);
      /*$F19F*/ ram_poke(0x006f, tmp2_U8);
      /*$F1A1*/ ram_poke(0x0070, tmp1_U8);
      /*$F1A3*/ s_x = 0x00;
      /*$F1A7*/ ram_poke(0x0067, 0x00);
      /*$F1A9*/ ram_poke(0x0068, 0x08);
      /*$F1AD*/ ram_poke(0x00d6, 0x00);
      /*$F1B0*/ poke(ram_peek16(0x0067), 0x00);
      /*$F1B2*/ tmp1_U8 = (uint8_t)(ram_peek(0x0067) + 0x01);
                ram_poke(0x0067, tmp1_U8);
      /*$F1B4*/ branchTarget = true; block_id = tmp1_U8 ? 1789 : 1788;
      break;
    case 1788:  // $F1B6
      /*$F1B6*/ CYCLES(0xf1b6, 4);
                ram_poke(0x0068, (uint8_t)(ram_peek(0x0068) + 0x01));
                block_id = 1789;
      break;
    case 1789:  // $F1B8
      /*$F1B8*/ CYCLES(0xf1b8, 12);
                s_a = ram_peek(0x0067);
      /*$F1BA*/ s_y = ram_peek(0x0068);
      /*$F1BC*/ branchTarget = true; push16(0xf1be); block_id = 629;
      break;
    case 1790:  // $F1BF
      /*$F1BF*/ CYCLES(0xf1bf, 6);
                branchTarget = true; push16(0xf1c1); block_id = 774;
      break;
    case 1791:  // $F1C2
      /*$F1C2*/ CYCLES(0xf1c2, 33);
      /*$F1C6*/ ram_poke(0x0004, 0x3a);
      /*$F1C8*/ ram_poke(0x0005, 0xdb);
      /*$F1CA*/ s_a = 0x3c;
      /*$F1CC*/ s_status_not_z = 0xd4;
                s_status_n = 0x80;
                s_y = 0xd4;
      /*$F1CE*/ ram_poke(0x0001, 0x3c);
      /*$F1D0*/ ram_poke(0x0002, 0xd4);
      /*$F1D2*/ branchTarget = true; block_id = find_block_id_func_t001(0xf1d2, ram_peek16(0x0001));
      break;
    case 1792:  // $F1D5
      /*$F1D5*/ CYCLES(0xf1d5, 6);
                branchTarget = true; push16(0xf1d7); block_id = 930;
      break;
    case 1793:  // $F1D8
      /*$F1D8*/ CYCLES(0xf1d8, 6);
                branchTarget = true; push16(0xf1da); block_id = 1384;
      break;
    case 1794:  // $F1DB
      /*$F1DB*/ CYCLES(0xf1db, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xf1db, ram_peek16al(0x0050));
      break;
    case 1795:  // $F1EC
      /*$F1EC*/ CYCLES(0xf1ec, 6);
                branchTarget = true; push16(0xf1ee); block_id = 1380;
      break;
    case 1796:  // $F1EF
      /*$F1EF*/ CYCLES(0xf1ef, 7);
                tmp2_U8 = s_x >= 0x30;
                s_status_c = tmp2_U8;
      /*$F1F1*/ branchTarget = true; block_id = tmp2_U8 ? 1801 : 1797;
      break;
    case 1797:  // $F1F3
      /*$F1F3*/ CYCLES(0xf1f3, 12);
                ram_poke(0x00f0, s_x);
      /*$F1F5*/ s_a = 0x2c;
      /*$F1F7*/ branchTarget = true; push16(0xf1f9); block_id = 1021;
      break;
    case 1798:  // $F1FA
      /*$F1FA*/ CYCLES(0xf1fa, 6);
                branchTarget = true; push16(0xf1fc); block_id = 1380;
      break;
    case 1799:  // $F1FD
      /*$F1FD*/ CYCLES(0xf1fd, 7);
                tmp2_U8 = s_x;
                s_status_not_z = (tmp2_U8 != 0x30);
                tmp1_U8 = tmp2_U8 >= 0x30;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - 0x30) & 0x80);
      /*$F1FF*/ branchTarget = true; block_id = tmp1_U8 ? 1801 : 1800;
      break;
    case 1800:  // $F201
      /*$F201*/ CYCLES(0xf201, 9);
                tmp1_U8 = s_x;
                ram_poke(0x002c, tmp1_U8);
      /*$F203*/ ram_poke(0x002d, tmp1_U8);
      /*$F205*/ branchTarget = true; block_id = find_block_id_func_t001(0xf205, pop16() + 1);;
      break;
    case 1801:  // $F206
      /*$F206*/ CYCLES(0xf206, 6);
                branchTarget = true; block_id = 1144;
      break;
    case 1802:  // $F273
      /*$F273*/ CYCLES(0xf273, 7);
                s_a = 0xff;
      /*$F279*/ CYCLES(0xf279, 12);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_x = 0x00;
      /*$F27B*/ ram_poke(0x0032, 0xff);
      /*$F27D*/ ram_poke(0x00f3, 0x00);
      /*$F27F*/ branchTarget = true; block_id = find_block_id_func_t001(0xf27f, pop16() + 1);;
      break;
    case 1803:  // $F2E9
      /*$F2E9*/ CYCLES(0xf2e9, 70);
                ram_poke(0x00de, s_x);
      /*$F2EB*/ tmp1_U8 = ram_peek(0x00f8);
                s_x = tmp1_U8;
      /*$F2ED*/ ram_poke(0x00df, tmp1_U8);
      /*$F2F1*/ ram_poke(0x00da, ram_peek(0x0075));
      /*$F2F5*/ ram_poke(0x00db, ram_peek(0x0076));
      /*$F2F9*/ ram_poke(0x00dc, ram_peek(0x0079));
      /*$F2FD*/ ram_poke(0x00dd, ram_peek(0x007a));
      /*$F301*/ ram_poke(0x00b8, ram_peek(0x00f4));
      /*$F305*/ ram_poke(0x00b9, ram_peek(0x00f5));
      /*$F309*/ ram_poke(0x0075, ram_peek(0x00f6));
      /*$F30B*/ tmp1_U8 = ram_peek(0x00f7);
                s_a = tmp1_U8;
      /*$F30D*/ ram_poke(0x0076, tmp1_U8);
      /*$F30F*/ branchTarget = true; push16(0xf311); block_id = 4;
      break;
    case 1804:  // $F312
      /*$F312*/ CYCLES(0xf312, 6);
                branchTarget = true; push16(0xf314); block_id = 841;
      break;
    case 1805:  // $F315
      /*$F315*/ CYCLES(0xf315, 6);
                branchTarget = true; block_id = 791;
      break;
    case 1806:  // $FA49
      /*$FA49*/ CYCLES(0xfa49, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xfa49, ram_peek16al(0x03fe));
      break;
    case 1807:  // $FA4C
      /*$FA4C*/ CYCLES(0xfa4c, 7);
                tmp1_U8 = pop8();
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_i = ((tmp1_U8 & 0x04) != 0);
                s_status_d = ((tmp1_U8 & 0x08) != 0);
                s_status_b = 0x00;
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
      /*$FA4D*/ FUNC_SAV1(0xfffe);
      /*$FA50*/ CYCLES(0xfa50, 16);
                tmp1_U8 = pop8();
      /*$FA51*/ ram_poke(0x003a, tmp1_U8);
      /*$FA53*/ tmp1_U8 = pop8();
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$FA54*/ ram_poke(0x003b, tmp1_U8);
      /*$FA56*/ branchTarget = true; block_id = find_block_id_func_t001(0xfa56, ram_peek16al(0x03f0));
      break;
    case 1808:  // $FA66
      /*$FA66*/ CYCLES(0xfa66, 6);
                FUNC_INIT(0xfffe);
                branchTarget = true; block_id = 1809;
      break;
    case 1809:  // $FA69
      /*$FA69*/ CYCLES(0xfa69, 6);
                rom_setvid(0xfffe);
      /*$FA6C*/ CYCLES(0xfa6c, 6);
                rom_setkbd(0xfffe);
      /*$FA6F*/ CYCLES(0xfa6f, 38);
                tmp1_U8 = io_peek(0xc058);
      /*$FA72*/ tmp1_U8 = io_peek(0xc05a);
      /*$FA75*/ tmp1_U8 = io_peek(0xc05d);
      /*$FA78*/ tmp1_U8 = io_peek(0xc05f);
      /*$FA7B*/ tmp1_U8 = io_peek(0xcfff);
      /*$FA7E*/ tmp1_U8 = io_peek(0xc010);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
      /*$FA81*/ s_status_d = 0x00;
      /*$FA82*/ FUNC_BELL(0xfffe);
                branchTarget = true; block_id = 1810;
      break;
    case 1810:  // $FA85
      /*$FA85*/ CYCLES(0xfa85, 18);
      /*$FA88*/ tmp1_U8 = ram_peek(0x03f3) ^ 0xa5;
      /*$FA8A*/ tmp2_U8 = ram_peek(0x03f4);
                s_status_c = (tmp1_U8 >= tmp2_U8);
      /*$FA8D*/ branchTarget = true; block_id = (tmp1_U8 != tmp2_U8) ? 1815 : 1811;
      break;
    case 1811:  // $FA8F
      /*$FA8F*/ CYCLES(0xfa8f, 9);
                tmp2_U8 = ram_peek(0x03f2);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$FA92*/ branchTarget = true; block_id = tmp2_U8 ? 1814 : 1812;
      break;
    case 1812:  // $FA94
      /*$FA94*/ CYCLES(0xfa94, 12);
                s_a = 0xe0;
      /*$FA96*/ tmp2_U8 = ram_peek(0x03f3);
                tmp1_U8 = 0xe0 != tmp2_U8;
                s_status_not_z = tmp1_U8;
                s_status_c = (0xe0 >= tmp2_U8);
                s_status_n = ((uint8_t)(0xe0 - tmp2_U8) & 0x80);
      /*$FA99*/ branchTarget = true; block_id = tmp1_U8 ? 1814 : 1813;
      break;
    case 1813:  // $FA9B
      /*$FA9B*/ CYCLES(0xfa9b, 14);
      /*$FA9D*/ ram_poke(0x03f2, 0x03);
      /*$E000*/ CYCLES(0xe000, 6);
      /*$F128*/ CYCLES(0xf128, 38);
      /*$F12A*/ ram_poke(0x0076, 0xff);
      /*$F12E*/ s_sp = 0xfb;
      /*$F133*/ ram_poke(0x0001, 0x28);
      /*$F135*/ ram_poke(0x0002, 0xf1);
      /*$F137*/ ram_poke(0x0004, 0x28);
      /*$F139*/ ram_poke(0x0005, 0xf1);
      /*$F13B*/ FUNC_NORMAL(0xfffe);
                branchTarget = true; block_id = 1781;
      break;
    case 1814:  // $FAA3
      /*$FAA3*/ CYCLES(0xfaa3, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xfaa3, ram_peek16al(0x03f2));
      break;
    case 1815:  // $FAA6
      /*$FAA6*/ CYCLES(0xfaa6, 6);
                FUNC_APPLEII(0xfffe);
                branchTarget = true; block_id = 1816;
      break;
    case 1816:  // $FAA9
      /*$FAA9*/ CYCLES(0xfaa9, 4);
                s_x = 0x05;
                block_id = 1817;
      break;
    case 1817:  // $FAAB
      /*$FAAB*/ CYCLES(0xfaab, 16);
                tmp1_U8 = s_x;
      /*$FAAE*/ ram_poke((0x03ef + tmp1_U8), ram_peek((0xfafc + tmp1_U8)));
      /*$FAB1*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_x = tmp1_U8;
      /*$FAB2*/ branchTarget = true; block_id = tmp1_U8 ? 1817 : 1818;
      break;
    case 1818:  // $FAB4
      /*$FAB4*/ CYCLES(0xfab4, 11);
      /*$FAB6*/ ram_poke(0x0000, s_x);
      /*$FAB8*/ ram_poke(0x0001, 0xc8);
                block_id = 1819;
      break;
    case 1819:  // $FABA
      /*$FABA*/ CYCLES(0xfaba, 18);
                s_y = 0x07;
      /*$FABC*/ ram_poke(0x0001, (uint8_t)(ram_peek(0x0001) - 0x01));
      /*$FABE*/ tmp1_U8 = ram_peek(0x0001);
                s_a = tmp1_U8;
      /*$FAC2*/ branchTarget = true; block_id = !(tmp1_U8 != 0xc0) ? 1813 : 1820;
      break;
    case 1820:  // $FAC4
      /*$FAC4*/ CYCLES(0xfac4, 6);
                ram_poke(0x07f8, s_a);
                block_id = 1821;
      break;
    case 1821:  // $FAC7
      /*$FAC7*/ CYCLES(0xfac7, 12);
                tmp1_U8 = s_y;
                tmp2_U8 = peek((ram_peek16al(0x0000) + tmp1_U8));
                s_a = tmp2_U8;
      /*$FAC9*/ tmp1_U8 = ram_peek((0xfb01 + tmp1_U8));
                s_status_c = (tmp2_U8 >= tmp1_U8);
      /*$FACC*/ branchTarget = true; block_id = (tmp2_U8 != tmp1_U8) ? 1819 : 1822;
      break;
    case 1822:  // $FACE
      /*$FACE*/ CYCLES(0xface, 7);
      /*$FACF*/ tmp1_U8 = (uint8_t)(s_y - 0x02);
                s_status_not_z = tmp1_U8;
                tmp2_U8 = tmp1_U8 & 0x80;
                s_status_n = tmp2_U8;
                s_y = tmp1_U8;
      /*$FAD0*/ branchTarget = true; block_id = !tmp2_U8 ? 1821 : 1823;
      break;
    case 1823:  // $FAD2
      /*$FAD2*/ CYCLES(0xfad2, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xfad2, ram_peek16al(0x0000));
      break;
    case 1824:  // $FB2F
      /*$FB2F*/ CYCLES(0xfb2f, 29);
      /*$FB31*/ ram_poke(0x0048, 0x00);
      /*$FB33*/ tmp1_U8 = io_peek(0xc056);
      /*$FB36*/ tmp1_U8 = io_peek(0xc054);
      /*$FB39*/ tmp1_U8 = io_peek(0xc051);
      /*$FB4B*/ CYCLES(0xfb4b, 36);
                ram_poke(0x0022, 0x00);
      /*$FB4F*/ ram_poke(0x0020, 0x00);
      /*$FB53*/ ram_poke(0x0021, 0x28);
      /*$FB57*/ ram_poke(0x0023, 0x18);
      /*$FB5B*/ ram_poke(0x0025, 0x17);
      /*$FB5D*/ branchTarget = true; block_id = 1852;
      break;
    case 1825:  // $FB60
      /*$FB60*/ CYCLES(0xfb60, 6);
                rom_home(0xfffe);
                branchTarget = true; block_id = 1826;
      break;
    case 1826:  // $FB63
      /*$FB63*/ CYCLES(0xfb63, 4);
                s_y = 0x08;
                block_id = 1827;
      break;
    case 1827:  // $FB65
      /*$FB65*/ CYCLES(0xfb65, 16);
                tmp2_U8 = s_y;
                tmp1_U8 = ram_peek((0xfb08 + tmp2_U8));
                s_a = tmp1_U8;
      /*$FB68*/ ram_poke((0x040e + tmp2_U8), tmp1_U8);
      /*$FB6B*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_y = tmp2_U8;
      /*$FB6C*/ branchTarget = true; block_id = tmp2_U8 ? 1827 : 1828;
      break;
    case 1828:  // $FB6E
      /*$FB6E*/ CYCLES(0xfb6e, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfb6e, pop16() + 1);;
      break;
    case 1829:  // $FB97
      /*$FB97*/ CYCLES(0xfb97, 7);
                s_status_c = 0x01;
      /*$FC2C*/ CYCLES(0xfc2c, 7);
                tmp2_U8 = s_a ^ 0xc0;
                s_a = tmp2_U8;
      /*$FC2E*/ branchTarget = true; block_id = !tmp2_U8 ? 1934 : 1859;
      break;
    case 1830:  // $FB9B
      /*$FB9B*/ CYCLES(0xfb9b, 12);
                tmp2_U8 = s_a;
                s_y = tmp2_U8;
      /*$FB9C*/ s_a = ram_peek((0xfa48 + tmp2_U8));
      /*$FB9F*/ FUNC_ESCOLD(0xfffe);
      /*$FBA2*/ CYCLES(0xfba2, 6);
                branchTarget = true; push16(0xfba4); block_id = 1899;
      break;
    case 1831:  // $FBA5
      /*$FBA5*/ CYCLES(0xfba5, 7);
      /*$FBA7*/ branchTarget = true; block_id = (s_a >= 0xce) ? 1941 : 1832;
      break;
    case 1832:  // $FBA9
      /*$FBA9*/ CYCLES(0xfba9, 7);
      /*$FBAB*/ branchTarget = true; block_id = !(s_a >= 0xc9) ? 1942 : 1833;
      break;
    case 1833:  // $FBAD
      /*$FBAD*/ CYCLES(0xfbad, 7);
                tmp2_U8 = s_a != 0xcc;
                s_status_not_z = tmp2_U8;
      /*$FBAF*/ branchTarget = true; block_id = !tmp2_U8 ? 1943 : 1834;
      break;
    case 1834:  // $FBB1
      /*$FBB1*/ CYCLES(0xfbb1, 4);
                branchTarget = true; block_id = s_status_not_z ? 1830 : 1835;
      break;
    case 1835:  // $FBB3
      /*$FBB3*/ CYCLES(0xfbb3, 24);
      /*$FBC0*/ FUNC_BASCALC(0x0000);
                block_id = find_block_id_func_t001(0xfbc0, pop16() + 1);;
      break;
    case 1836:  // $FBC1
      /*$FBC1*/ CYCLES(0xfbc1, 23);
                tmp2_U8 = s_a;
                push8(tmp2_U8);
      /*$FBC2*/ tmp1_U8 = tmp2_U8 & 0x01;
                s_status_c = tmp1_U8;
      /*$FBC7*/ ram_poke(0x0029, (((tmp2_U8 >> 0x01) & 0x03) | 0x04));
      /*$FBC9*/ tmp2_U8 = pop8();
      /*$FBCA*/ s_a = (tmp2_U8 & 0x18);
      /*$FBCC*/ branchTarget = true; block_id = !tmp1_U8 ? 1840 : 1837;
      break;
    case 1837:  // $FBCE
      /*$FBCE*/ CYCLES(0xfbce, 4);
                block_id = s_status_d ? 1839 : 1838;
      break;
    case 1838:  // $FBCE
      /*$FBCE*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 + 0x007f) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x007f);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1841;
      break;
    case 1839:  // $FBCE
      /*$FBCE*/ tmp4_U16 = adc_dec16(s_a, 0x7f, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 1841;
      break;
    case 1840:  // $FBD0
      /*$FBD0*/ CYCLES(0xfbd0, 16);
                tmp1_U8 = s_a;
                ram_poke(0x0028, tmp1_U8);
      /*$FBD3*/ tmp6_U16 = tmp1_U8 << 0x02;
                s_status_c = (uint8_t)((tmp6_U16 & 0x01ff) >> 8);
      /*$FBD4*/ tmp1_U8 = ((uint8_t)tmp6_U16) | ram_peek(0x0028);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$FBD6*/ ram_poke(0x0028, tmp1_U8);
      /*$FBD8*/ branchTarget = true; block_id = find_block_id_func_t001(0xfbd8, pop16() + 1);;
      break;
    case 1841:  // $FBD0
      /*$FBCE*/ block_id = 1840;
      break;
    case 1842:  // $FBE2
      /*$FBE2*/ CYCLES(0xfbe2, 4);
                s_y = 0xc0;
                block_id = 1843;
      break;
    case 1843:  // $FBE4
      /*$FBE4*/ CYCLES(0xfbe4, 9);
                s_a = 0x0c;
      /*$FBE6*/ FUNC_MON_WAIT(0xfffe);
                branchTarget = true; block_id = 1844;
      break;
    case 1844:  // $FBE9
      /*$FBE9*/ CYCLES(0xfbe9, 11);
                tmp1_U8 = io_peek(0xc030);
                s_a = tmp1_U8;
      /*$FBEC*/ tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
      /*$FBED*/ branchTarget = true; block_id = tmp1_U8 ? 1843 : 1845;
      break;
    case 1845:  // $FBEF
      /*$FBEF*/ CYCLES(0xfbef, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfbef, pop16() + 1);;
      break;
    case 1846:  // $FBF4
      /*$FBF4*/ CYCLES(0xfbf4, 14);
                ram_poke(0x0024, (uint8_t)(ram_peek(0x0024) + 0x01));
      /*$FBF6*/ tmp1_U8 = ram_peek(0x0024);
                s_a = tmp1_U8;
      /*$FBF8*/ tmp2_U8 = ram_peek(0x0021);
                s_status_not_z = (tmp1_U8 != tmp2_U8);
                tmp5_U8 = tmp1_U8 >= tmp2_U8;
                s_status_c = tmp5_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
      /*$FBFA*/ branchTarget = true; block_id = tmp5_U8 ? 1882 : 1847;
      break;
    case 1847:  // $FBFC
      /*$FBFC*/ CYCLES(0xfbfc, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfbfc, pop16() + 1);;
      break;
    case 1848:  // $FC10
      /*$FC10*/ CYCLES(0xfc10, 7);
                tmp5_U8 = (uint8_t)(ram_peek(0x0024) - 0x01);
                s_status_not_z = tmp5_U8;
                tmp2_U8 = tmp5_U8 & 0x80;
                s_status_n = tmp2_U8;
                ram_poke(0x0024, tmp5_U8);
      /*$FC12*/ branchTarget = true; block_id = !tmp2_U8 ? 1847 : 1849;
      break;
    case 1849:  // $FC14
      /*$FC14*/ CYCLES(0xfc14, 11);
      /*$FC16*/ ram_poke(0x0024, ram_peek(0x0021));
      /*$FC18*/ ram_poke(0x0024, (uint8_t)(ram_peek(0x0024) - 0x01));
                block_id = 1850;
      break;
    case 1850:  // $FC1A
      /*$FC1A*/ CYCLES(0xfc1a, 11);
                tmp2_U8 = ram_peek(0x0022);
                s_a = tmp2_U8;
      /*$FC1C*/ tmp5_U8 = ram_peek(0x0025);
                s_status_not_z = (tmp2_U8 != tmp5_U8);
                tmp1_U8 = tmp2_U8 >= tmp5_U8;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp5_U8) & 0x80);
      /*$FC1E*/ branchTarget = true; block_id = tmp1_U8 ? 1858 : 1851;
      break;
    case 1851:  // $FC20
      /*$FC20*/ CYCLES(0xfc20, 4);
                ram_poke(0x0025, (uint8_t)(ram_peek(0x0025) - 0x01));
                block_id = 1852;
      break;
    case 1852:  // $FC22
      /*$FC22*/ CYCLES(0xfc22, 4);
                s_a = ram_peek(0x0025);
                FUNC_VTABZ(0x0000);
                block_id = find_block_id_func_t001(0xfc22, pop16() + 1);;
      break;
    case 1853:  // $FC24
      /*$FC24*/ CYCLES(0xfc24, 6);
                FUNC_BASCALC(0xfffe);
                branchTarget = true; block_id = 1854;
      break;
    case 1854:  // $FC27
      /*$FC27*/ CYCLES(0xfc27, 7);
                block_id = s_status_d ? 1856 : 1855;
      break;
    case 1855:  // $FC27
      /*$FC27*/ tmp4_U16 = s_a;
                tmp6_U16 = ram_peek(0x0020);
                tmp3_U16 = (tmp4_U16 + tmp6_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 1857;
      break;
    case 1856:  // $FC27
      /*$FC27*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x0020), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
                block_id = 1857;
      break;
    case 1857:  // $FC29
      /*$FC29*/ ram_poke(0x0028, s_a);
                block_id = 1858;
      break;
    case 1858:  // $FC2B
      /*$FC2B*/ CYCLES(0xfc2b, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfc2b, pop16() + 1);;
      break;
    case 1859:  // $FC30
      /*$FC30*/ CYCLES(0xfc30, 7);
                block_id = s_status_d ? 1861 : 1860;
      break;
    case 1860:  // $FC30
      /*$FC30*/ tmp3_U16 = s_a;
                tmp6_U16 = (tmp3_U16 + 0x00fd) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)0x00fd);
                tmp1_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
                block_id = 1862;
      break;
    case 1861:  // $FC30
      /*$FC30*/ tmp6_U16 = adc_dec16(s_a, 0xfd, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 1862;
      break;
    case 1862:  // $FC32
      /*$FC32*/ branchTarget = true; block_id = !s_status_c ? 1846 : 1863;
      break;
    case 1863:  // $FC34
      /*$FC34*/ CYCLES(0xfc34, 4);
                branchTarget = true; block_id = !s_status_not_z ? 1848 : 1864;
      break;
    case 1864:  // $FC36
      /*$FC36*/ CYCLES(0xfc36, 7);
                block_id = s_status_d ? 1866 : 1865;
      break;
    case 1865:  // $FC36
      /*$FC36*/ tmp6_U16 = s_a;
                tmp3_U16 = (tmp6_U16 + 0x00fd) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)0x00fd);
                tmp1_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
                block_id = 1867;
      break;
    case 1866:  // $FC36
      /*$FC36*/ tmp3_U16 = adc_dec16(s_a, 0xfd, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 1867;
      break;
    case 1867:  // $FC38
      /*$FC38*/ branchTarget = true; block_id = !s_status_c ? 1883 : 1868;
      break;
    case 1868:  // $FC3A
      /*$FC3A*/ CYCLES(0xfc3a, 4);
                branchTarget = true; block_id = !s_status_not_z ? 1850 : 1869;
      break;
    case 1869:  // $FC3C
      /*$FC3C*/ CYCLES(0xfc3c, 7);
                block_id = s_status_d ? 1871 : 1870;
      break;
    case 1870:  // $FC3C
      /*$FC3C*/ tmp3_U16 = s_a;
                tmp6_U16 = (tmp3_U16 + 0x00fd) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)0x00fd);
                tmp1_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 1872;
      break;
    case 1871:  // $FC3C
      /*$FC3C*/ tmp6_U16 = adc_dec16(s_a, 0xfd, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
                block_id = 1872;
      break;
    case 1872:  // $FC3E
      /*$FC3E*/ branchTarget = true; block_id = !s_status_c ? 1944 : 1873;
      break;
    case 1873:  // $FC40
      /*$FC40*/ CYCLES(0xfc40, 4);
                branchTarget = true; block_id = s_status_not_z ? 1858 : 1874;
      break;
    case 1874:  // $FC42
      /*$FC42*/ CYCLES(0xfc42, 7);
                s_y = ram_peek(0x0024);
      /*$FC44*/ s_a = ram_peek(0x0025);
                block_id = 1875;
      break;
    case 1875:  // $FC46
      /*$FC46*/ CYCLES(0xfc46, 7);
                push8(s_a);
      /*$FC47*/ FUNC_VTABZ(0xfffe);
                branchTarget = true; block_id = 1876;
      break;
    case 1876:  // $FC4A
      /*$FC4A*/ CYCLES(0xfc4a, 6);
                FUNC_CLREOLZ(0xfffe);
                branchTarget = true; block_id = 1877;
      break;
    case 1877:  // $FC4D
      /*$FC4D*/ CYCLES(0xfc4d, 16);
                s_y = 0x00;
      /*$FC4F*/ tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$FC50*/ block_id = s_status_d ? 1879 : 1878;
      break;
    case 1878:  // $FC50
      /*$FC50*/ tmp6_U16 = s_a;
                tmp3_U16 = tmp6_U16 + s_status_c;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1880;
      break;
    case 1879:  // $FC50
      /*$FC50*/ tmp3_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
                block_id = 1880;
      break;
    case 1880:  // $FC52
      /*$FC52*/ tmp1_U8 = s_a >= ram_peek(0x0023);
                s_status_c = tmp1_U8;
      /*$FC54*/ branchTarget = true; block_id = !tmp1_U8 ? 1875 : 1881;
      break;
    case 1881:  // $FC56
      /*$FC56*/ CYCLES(0xfc56, 4);
                branchTarget = true; block_id = s_status_c ? 1852 : 1935;
      break;
    case 1882:  // $FC62
      /*$FC62*/ CYCLES(0xfc62, 7);
      /*$FC64*/ ram_poke(0x0024, 0x00);
                block_id = 1883;
      break;
    case 1883:  // $FC66
      /*$FC66*/ CYCLES(0xfc66, 4);
                ram_poke(0x0025, (uint8_t)(ram_peek(0x0025) + 0x01));
                rom_fc68(0x0000);
                block_id = find_block_id_func_t001(0xfc66, pop16() + 1);;
      break;
    case 1884:  // $FC9C
      /*$FC9C*/ CYCLES(0xfc9c, 4);
                s_y = ram_peek(0x0024);
                FUNC_CLREOLZ(0x0000);
                block_id = find_block_id_func_t001(0xfc9c, pop16() + 1);;
      break;
    case 1885:  // $FC9E
      /*$FC9E*/ CYCLES(0xfc9e, 4);
                s_a = 0xa0;
                block_id = 1886;
      break;
    case 1886:  // $FCA0
      /*$FCA0*/ CYCLES(0xfca0, 12);
                tmp1_U8 = s_y;
                poke((ram_peek16al(0x0028) + tmp1_U8), s_a);
      /*$FCA2*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
                s_y = tmp1_U8;
      /*$FCA3*/ tmp5_U8 = ram_peek(0x0021);
                s_status_not_z = (tmp1_U8 != tmp5_U8);
                tmp2_U8 = tmp1_U8 >= tmp5_U8;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - tmp5_U8) & 0x80);
      /*$FCA5*/ branchTarget = true; block_id = !tmp2_U8 ? 1886 : 1887;
      break;
    case 1887:  // $FCA7
      /*$FCA7*/ CYCLES(0xfca7, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfca7, pop16() + 1);;
      break;
    case 1888:  // $FCA8
      /*$FCA8*/ CYCLES(0xfca8, 2);
                s_status_c = 0x01;
                block_id = 1889;
      break;
    case 1889:  // $FCA9
      /*$FCA9*/ CYCLES(0xfca9, 2);
                push8(s_a);
                block_id = 1890;
      break;
    case 1890:  // $FCAA
      /*$FCAA*/ CYCLES(0xfcaa, 7);
                block_id = s_status_d ? 1892 : 1891;
      break;
    case 1891:  // $FCAA
      /*$FCAA*/ tmp3_U16 = (s_a - 0x0001) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                tmp2_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp2_U8;
                s_a = tmp2_U8;
                block_id = 1893;
      break;
    case 1892:  // $FCAA
      /*$FCAA*/ tmp3_U16 = sbc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                block_id = 1893;
      break;
    case 1893:  // $FCAC
      /*$FCAC*/ branchTarget = true; block_id = s_status_not_z ? 1890 : 1894;
      break;
    case 1894:  // $FCAE
      /*$FCAE*/ CYCLES(0xfcae, 9);
                tmp2_U8 = pop8();
                s_a = tmp2_U8;
      /*$FCAF*/ block_id = s_status_d ? 1896 : 1895;
      break;
    case 1895:  // $FCAF
      /*$FCAF*/ tmp3_U16 = s_a;
                tmp6_U16 = (tmp3_U16 - 0x0001) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)0xfffe);
                tmp2_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 1897;
      break;
    case 1896:  // $FCAF
      /*$FCAF*/ tmp6_U16 = sbc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp2_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 1897;
      break;
    case 1897:  // $FCB1
      /*$FCB1*/ branchTarget = true; block_id = s_status_not_z ? 1889 : 1898;
      break;
    case 1898:  // $FCB3
      /*$FCB3*/ CYCLES(0xfcb3, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfcb3, pop16() + 1);;
      break;
    case 1899:  // $FD0C
      /*$FD0C*/ CYCLES(0xfd0c, 26);
                tmp1_U8 = ram_peek(0x0024);
                s_y = tmp1_U8;
      /*$FD0E*/ tmp5_U8 = peek((ram_peek16al(0x0028) + tmp1_U8));
      /*$FD10*/ push8(tmp5_U8);
      /*$FD15*/ poke((ram_peek16al(0x0028) + tmp1_U8), ((tmp5_U8 & 0x3f) | 0x40));
      /*$FD17*/ tmp1_U8 = pop8();
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$FD18*/ branchTarget = true; block_id = find_block_id_func_t001(0xfd18, ram_peek16al(0x0038));
      break;
    case 1900:  // $FD1B
      /*$FD1B*/ CYCLES(0xfd1b, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x004e) + 0x01);
                ram_poke(0x004e, tmp2_U8);
      /*$FD1D*/ branchTarget = true; block_id = tmp2_U8 ? 1902 : 1901;
      break;
    case 1901:  // $FD1F
      /*$FD1F*/ CYCLES(0xfd1f, 4);
                ram_poke(0x004f, (uint8_t)(ram_peek(0x004f) + 0x01));
                block_id = 1902;
      break;
    case 1902:  // $FD21
      /*$FD21*/ CYCLES(0xfd21, 9);
                tmp2_U8 = io_peek(0xc000);
      /*$FD24*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1900 : 1903;
      break;
    case 1903:  // $FD26
      /*$FD26*/ CYCLES(0xfd26, 16);
                poke((ram_peek16al(0x0028) + s_y), s_a);
      /*$FD28*/ tmp2_U8 = io_peek(0xc000);
                s_a = tmp2_U8;
      /*$FD2B*/ tmp5_U8 = io_peek(0xc010);
                s_status_n = (tmp5_U8 & 0x80);
                s_status_v = ((tmp5_U8 >> 0x06) & 0x01);
                s_status_not_z = (tmp2_U8 & tmp5_U8);
      /*$FD2E*/ branchTarget = true; block_id = find_block_id_func_t001(0xfd2e, pop16() + 1);;
      break;
    case 1904:  // $FD2F
      /*$FD2F*/ CYCLES(0xfd2f, 6);
                branchTarget = true; push16(0xfd31); block_id = 1899;
      break;
    case 1905:  // $FD32
      /*$FD32*/ CYCLES(0xfd32, 6);
                branchTarget = true; push16(0xfd34); block_id = 1831;
      break;
    case 1906:  // $FD35
      /*$FD35*/ CYCLES(0xfd35, 6);
                branchTarget = true; push16(0xfd37); block_id = 1899;
      break;
    case 1907:  // $FD38
      /*$FD38*/ CYCLES(0xfd38, 7);
                tmp5_U8 = s_a;
                tmp2_U8 = tmp5_U8 != 0x9b;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp5_U8 >= 0x9b);
                s_status_n = ((uint8_t)(tmp5_U8 - 0x9b) & 0x80);
      /*$FD3A*/ branchTarget = true; block_id = !tmp2_U8 ? 1904 : 1908;
      break;
    case 1908:  // $FD3C
      /*$FD3C*/ CYCLES(0xfd3c, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfd3c, pop16() + 1);;
      break;
    case 1909:  // $FD3D
      /*$FD3D*/ CYCLES(0xfd3d, 23);
      /*$FD3F*/ push8(ram_peek(0x0032));
      /*$FD42*/ ram_poke(0x0032, 0xff);
      /*$FD44*/ tmp2_U8 = ram_peek((0x0200 + s_x));
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$FD47*/ rom_cout(0xfffe);
                branchTarget = true; block_id = 1910;
      break;
    case 1910:  // $FD4A
      /*$FD4A*/ CYCLES(0xfd4a, 18);
                tmp2_U8 = pop8();
      /*$FD4B*/ ram_poke(0x0032, tmp2_U8);
      /*$FD4D*/ tmp2_U8 = ram_peek((0x0200 + s_x));
                s_a = tmp2_U8;
      /*$FD50*/ s_status_c = (tmp2_U8 >= 0x88);
      /*$FD52*/ branchTarget = true; block_id = !(tmp2_U8 != 0x88) ? 1919 : 1911;
      break;
    case 1911:  // $FD54
      /*$FD54*/ CYCLES(0xfd54, 7);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x98);
      /*$FD56*/ branchTarget = true; block_id = !(tmp2_U8 != 0x98) ? 1915 : 1912;
      break;
    case 1912:  // $FD58
      /*$FD58*/ CYCLES(0xfd58, 7);
                tmp2_U8 = s_x >= 0xf8;
                s_status_c = tmp2_U8;
      /*$FD5A*/ branchTarget = true; block_id = !tmp2_U8 ? 1914 : 1913;
      break;
    case 1913:  // $FD5C
      /*$FD5C*/ CYCLES(0xfd5c, 6);
                FUNC_BELL(0xfffe);
                branchTarget = true; block_id = 1914;
      break;
    case 1914:  // $FD5F
      /*$FD5F*/ CYCLES(0xfd5f, 6);
                tmp2_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp2_U8;
      /*$FD60*/ branchTarget = true; block_id = tmp2_U8 ? 1921 : 1915;
      break;
    case 1915:  // $FD62
      /*$FD62*/ CYCLES(0xfd62, 9);
                s_status_not_z = 0xdc;
                s_status_n = 0x80;
                s_a = 0xdc;
      /*$FD64*/ rom_cout(0xfffe);
                branchTarget = true; block_id = 1916;
      break;
    case 1916:  // $FD67
      /*$FD67*/ CYCLES(0xfd67, 6);
                FUNC_CROUT(0xfffe);
                branchTarget = true; block_id = 1917;
      break;
    case 1917:  // $FD6A
      /*$FD6A*/ CYCLES(0xfd6a, 9);
                tmp1_U8 = ram_peek(0x0033);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$FD6C*/ rom_cout(0xfffe);
                branchTarget = true; block_id = 1918;
      break;
    case 1918:  // $FD6F
      /*$FD6F*/ CYCLES(0xfd6f, 4);
                s_x = 0x01;
                block_id = 1919;
      break;
    case 1919:  // $FD71
      /*$FD71*/ CYCLES(0xfd71, 6);
                tmp2_U8 = s_x;
                s_a = tmp2_U8;
      /*$FD72*/ branchTarget = true; block_id = !tmp2_U8 ? 1916 : 1920;
      break;
    case 1920:  // $FD74
      /*$FD74*/ CYCLES(0xfd74, 2);
                s_x = (uint8_t)(s_x - 0x01);
                block_id = 1921;
      break;
    case 1921:  // $FD75
      /*$FD75*/ CYCLES(0xfd75, 6);
                branchTarget = true; push16(0xfd77); block_id = 1906;
      break;
    case 1922:  // $FD78
      /*$FD78*/ CYCLES(0xfd78, 7);
      /*$FD7A*/ branchTarget = true; block_id = (s_a != 0x95) ? 1924 : 1923;
      break;
    case 1923:  // $FD7C
      /*$FD7C*/ CYCLES(0xfd7c, 4);
                tmp2_U8 = peek((ram_peek16al(0x0028) + s_y));
                s_a = tmp2_U8;
                block_id = 1924;
      break;
    case 1924:  // $FD7E
      /*$FD7E*/ CYCLES(0xfd7e, 7);
      /*$FD80*/ branchTarget = true; block_id = !(s_a >= 0xe0) ? 1926 : 1925;
      break;
    case 1925:  // $FD82
      /*$FD82*/ CYCLES(0xfd82, 4);
                s_a = (s_a & 0xdf);
                block_id = 1926;
      break;
    case 1926:  // $FD84
      /*$FD84*/ CYCLES(0xfd84, 12);
                tmp2_U8 = s_a;
                ram_poke((0x0200 + s_x), tmp2_U8);
      /*$FD87*/ s_status_c = (tmp2_U8 >= 0x8d);
      /*$FD89*/ branchTarget = true; block_id = (tmp2_U8 != 0x8d) ? 1909 : 1927;
      break;
    case 1927:  // $FD8B
      /*$FD8B*/ CYCLES(0xfd8b, 6);
                FUNC_CLREOL(0xfffe);
                FUNC_CROUT(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xfd8b, pop16() + 1);;
      break;
    case 1928:  // $FD8E
      /*$FD8E*/ CYCLES(0xfd8e, 7);
                s_status_not_z = 0x8d;
                s_status_n = 0x80;
                s_a = 0x8d;
      /*$FD90*/ rom_cout(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xfd90, pop16() + 1);;
      break;
    case 1929:  // $FD8E
      /*$FCA7*/ FUNC_CROUT(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xfca7, pop16() + 1);;
      break;
    case 1930:  // $FDFC
      /*$FDFC*/ CYCLES(0xfdfc, 7);
                tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$FDFD*/ tmp1_U8 = ram_peek(0x0035);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
      /*$FDFF*/ branchTarget = true; block_id = find_block_id_func_t001(0xfdff, pop16() + 1);;
      break;
    case 1931:  // $FE84
      /*$FE84*/ CYCLES(0xfe84, 9);
                s_status_not_z = 0xff;
                s_status_n = 0x80;
                s_y = 0xff;
      /*$FE86*/ ram_poke(0x0032, 0xff);
      /*$FE88*/ branchTarget = true; block_id = find_block_id_func_t001(0xfe88, pop16() + 1);;
      break;
    case 1932:  // $FF3A
      /*$FF3A*/ CYCLES(0xff3a, 9);
                s_status_not_z = 0x87;
                s_status_n = 0x80;
                s_a = 0x87;
      /*$FF3C*/ rom_cout(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xff3c, pop16() + 1);;
      break;
    case 1933:  // $FF4C
      /*$FF4C*/ CYCLES(0xff4c, 23);
                ram_poke(0x0046, s_x);
      /*$FF4E*/ ram_poke(0x0047, s_y);
      /*$FF50*/ push8((s_status_c | ((s_status_not_z == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | s_status_n));
      /*$FF51*/ tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$FF52*/ ram_poke(0x0048, tmp1_U8);
      /*$FF54*/ tmp1_U8 = s_sp;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$FF55*/ ram_poke(0x0049, tmp1_U8);
      /*$FF57*/ s_status_d = 0x00;
      /*$FF58*/ branchTarget = true; block_id = find_block_id_func_t001(0xff58, pop16() + 1);;
      break;
    case 1934:
      /*$FC2E*/ rom_home(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xfc2e, pop16() + 1);;
      break;
    case 1935:
      /*$FC56*/ rom_home(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xfc56, pop16() + 1);;
      break;
    case 1936:
      /*$E07B*/ FUNC_ISLETC(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xe07b, pop16() + 1);;
      break;
    case 1937:
      /*$E482*/ FUNC_GARBAGE(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xe482, pop16() + 1);;
      break;
    case 1938:
      /*$E517*/ FUNC_CHKSMPLVAR(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xe517, pop16() + 1);;
      break;
    case 1939:
      /*$E8C4*/ FUNC_INCFACMAN(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xe8c4, pop16() + 1);;
      break;
    case 1940:
      /*$E9B0*/ FUNC_MULT2(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xe9b0, pop16() + 1);;
      break;
    case 1941:
      /*$FBA7*/ FUNC_ESCOLD(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xfba7, pop16() + 1);;
      break;
    case 1942:
      /*$FBAB*/ FUNC_ESCOLD(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xfbab, pop16() + 1);;
      break;
    case 1943:
      /*$FBAF*/ FUNC_ESCOLD(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xfbaf, pop16() + 1);;
      break;
    case 1944:
      /*$FC3E*/ FUNC_CLREOL(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xfc3e, pop16() + 1);;
      break;
    case 1945:  // $10100
      /*$7374*/ push8((s_status_c | ((s_status_not_z == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | s_status_n));
      /*$FA40*/ CYCLES(0xfa40, 16);
                ram_poke(0x0045, s_a);
      /*$FA42*/ tmp2_U8 = pop8();
      /*$FA43*/ push8(tmp2_U8);
      /*$FA46*/ tmp4_U16 = tmp2_U8 << 0x03;
                tmp2_U8 = (uint8_t)tmp4_U16;
                s_status_c = (uint8_t)((tmp4_U16 & 0x01ff) >> 8);
                s_status_not_z = tmp2_U8;
                tmp1_U8 = tmp2_U8 & 0x80;
                s_status_n = tmp1_U8;
                s_a = tmp2_U8;
      /*$FA47*/ branchTarget = true; block_id = tmp1_U8 ? 1807 : 1806;
      break;
    default:
      fprintf(stderr, "panic: unknown block_id: %u\n", block_id);
      abort();
    }
  }
}

static const unsigned s_block_map_func_t001[] = {
    0x00b1,    2, 0x3750,   15, 0x60e7,   21, 0x614b,   29, 0x614e,   30,
    0x615d,   34, 0x6160,   35, 0x625b,   44, 0x628b,   48, 0x628e,   49,
    0x6291,   50, 0x62b8,   57, 0x6303,   64, 0x6310,   68, 0x63da,   95,
    0x640c,  105, 0x6429,  109, 0x6444,  111, 0x6456,  114, 0x645b,  115,
    0x6493,  122, 0x6560,  152, 0x658a,  155, 0x65b7,  159, 0x6b40,  227,
    0x6b65,  230, 0x6b70,  231, 0x6b82,  232, 0x6b8c,  233, 0x6b96,  235,
    0x6bdd,  242, 0x6be9,  244, 0x6c75,  264, 0x7048,  310, 0x7075,  315,
    0x7093,  317, 0x70c9,  321, 0x70d4,  322, 0x70e1,  323, 0x70ec,  324,
    0x70f5,  325, 0x7100,  326, 0x7113,  327, 0x712e,  332, 0x71ba,  342,
    0x720e,  363, 0x7225,  371, 0x7239,  376, 0x7248,  380, 0x72d9,  403,
    0x72e2,  404, 0x72ec,  405, 0x72f2,  406, 0x72f8,  407, 0x72fe,  408,
    0x7301,  409, 0x7308,  410, 0x7314,  412, 0x731e,  413, 0x7324,  414,
    0x732a,  415, 0x7330,  416, 0x7333,  417, 0x733e,  418, 0x734d,  420,
    0x7357,  421, 0x735d,  422, 0x7360,  423, 0x7365,  424, 0x736c,  425,
    0x7375,  426, 0x737f,  428, 0x7385,  429, 0x7388,  430, 0x7393,  431,
    0x73a2,  432, 0x73a9,  433, 0x73af,  434, 0x73b2,  435, 0x73b9,  436,
    0x73c2,  437, 0x73cc,  439, 0x73cf,  440, 0x73d6,  441, 0x73f4,  444,
    0x7414,  445, 0x748f,  466, 0x7541,  467, 0x7645,  492, 0x764a,  493,
    0x766c,  495, 0x7694,  500, 0x7697,  501, 0x76e4,  505, 0x76ed,  506,
    0x76f6,  507, 0x76f9,  508, 0x7710,  509, 0x7713,  510, 0x7716,  511,
    0x7726,  513, 0x7729,  514, 0x7739,  520, 0x7787,  540, 0x77e3,  551,
    0x7803,  554, 0x7868,  574, 0x7886,  575, 0x78d1,  586, 0x7945,  587,
    0x794d,  588, 0xd396,  600, 0xd42d,  646, 0xd434,  648, 0xd43c,  650,
    0xd43f,  651, 0xd444,  652, 0xd44d,  653, 0xd459,  656, 0xd467,  658,
    0xd46a,  659, 0xd4d6,  691, 0xd4f5,  694, 0xd533,  708, 0xd556,  714,
    0xd569,  717, 0xd668,  782, 0xd683,  783, 0xd7d8,  792, 0xd81a,  804,
    0xd820,  806, 0xd823,  807, 0xd863,  829, 0xd941,  842, 0xd944,  843,
    0xda03,  872, 0xda06,  873, 0xda43,  894, 0xda49,  895, 0xda52,  896,
    0xda5b,  897, 0xda60,  898, 0xda68,  901, 0xda6b,  902, 0xdaa8,  912,
    0xdb00,  916, 0xdb3d,  919, 0xdb67,  928, 0xdb6f,  929, 0xdd6a,  931,
    0xdd8e,  941, 0xdd91,  942, 0xdd98,  944, 0xddb1,  951, 0xddd6,  972,
    0xddda,  974, 0xde08,  983, 0xde23,  986, 0xde41,  991, 0xde67,  996,
    0xde8d, 1009, 0xdeb5, 1017, 0xdeb8, 1018, 0xded8, 1027, 0xdefc, 1033,
    0xdeff, 1034, 0xdf09, 1035, 0xdf16, 1037, 0xdf1d, 1039, 0xdf20, 1040,
    0xdf23, 1041, 0xdf26, 1042, 0xdf33, 1043, 0xdf3d, 1045, 0xdfe8, 1048,
    0xdfef, 1050, 0xe00a, 1053, 0xe015, 1057, 0xe03d, 1065, 0xe0b9, 1102,
    0xe105, 1119, 0xe108, 1120, 0xe137, 1127, 0xe156, 1128, 0xe15f, 1130,
    0xe1c7, 1154, 0xe201, 1168, 0xe21d, 1179, 0xe27a, 1199, 0xe29f, 1212,
    0xe346, 1237, 0xe34d, 1238, 0xe357, 1239, 0xe360, 1240, 0xe363, 1241,
    0xe383, 1245, 0xe39b, 1246, 0xe3a4, 1247, 0xe3e0, 1251, 0xe423, 1266,
    0xe5a0, 1340, 0xe5a3, 1341, 0xe5ba, 1347, 0xe5d1, 1348, 0xe6fb, 1381,
    0xe6fe, 1382, 0xe75b, 1386, 0xe7bc, 1390, 0xe7fa, 1408, 0xe98a, 1494,
    0xea3c, 1532, 0xea4b, 1538, 0xea58, 1541, 0xea6e, 1543, 0xea78, 1547,
    0xeb2e, 1580, 0xeb66, 1584, 0xec0f, 1621, 0xec64, 1631, 0xec71, 1635,
    0xec8a, 1643, 0xecac, 1657, 0xecb5, 1660, 0xeccb, 1667, 0xecd2, 1671,
    0xecd9, 1673, 0xecdd, 1674, 0xed20, 1690, 0xed2e, 1692, 0xed31, 1693,
    0xed5e, 1701, 0xed7b, 1708, 0xed82, 1710, 0xed89, 1712, 0xf13e, 1781,
    0xf16b, 1784, 0xf1bf, 1790, 0xf1c2, 1791, 0xf1d5, 1792, 0xf1d8, 1793,
    0xf1db, 1794, 0xf1ef, 1796, 0xf1fa, 1798, 0xf1fd, 1799, 0xf312, 1804,
    0xf315, 1805, 0xfa66, 1808, 0xfa69, 1809, 0xfa85, 1810, 0xfaa9, 1816,
    0xfb63, 1826, 0xfba5, 1831, 0xfbe2, 1842, 0xfbe9, 1844, 0xfc27, 1854,
    0xfc4a, 1876, 0xfc4d, 1877, 0xfd1b, 1900, 0xfd32, 1905, 0xfd35, 1906,
    0xfd38, 1907, 0xfd4a, 1910, 0xfd6f, 1918, 0xfd78, 1922, 0xfd8e, 1929,
    0xfdfc, 1930
};

static unsigned find_block_id_func_t001(uint16_t from_pc, uint16_t addr) {
  return addr_to_block_id(from_pc, addr, s_block_map_func_t001, sizeof(s_block_map_func_t001) / (sizeof(unsigned) * 2));
};
static const uint8_t s_mem_00b1[0x0018] = {
  0xE6, 0xB8, 0xD0, 0x02, 0xE6, 0xB9, 0xAD, 0x06, 0x02, 0xC9, 0x3A, 0xB0, 0x0A, 0xC9, 0x20, 0xF0,
  0xEF, 0x38, 0xE9, 0x30, 0x38, 0xE9, 0xD0, 0x60
};
static const uint8_t s_mem_3750[0x4dff] = {
  0xA2, 0x00, 0xBD, 0x00, 0x38, 0x9D, 0x00, 0x18, 0xE8, 0xD0, 0xF7, 0xEE, 0x54, 0x37, 0xEE, 0x57,
  0x37, 0xAD, 0x57, 0x37, 0xC9, 0x20, 0xD0, 0xE8, 0x20, 0x93, 0xFE, 0x20, 0x89, 0xFE, 0xA9, 0x52,
  0x8D, 0x00, 0x03, 0xA9, 0x01, 0x8D, 0x01, 0x03, 0x8D, 0x02, 0x03, 0x8D, 0x03, 0x03, 0xA9, 0x64,
  0x8D, 0x04, 0x03, 0x4C, 0xC2, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0C, 0x1B, 0x11, 0x1A, 0x0C, 0x0A, 0x1A, 0x02, 0x13, 0x0B, 0x1D, 0x1D, 0x18, 0x04, 0x1B, 0x1B,
  0x0A, 0x06, 0x10, 0x11, 0x03, 0x07, 0x03, 0x0E, 0x26, 0x1F, 0x13, 0x03, 0x26, 0x1C, 0x10, 0x16,
  0x09, 0x18, 0x1A, 0x11, 0x01, 0x13, 0x17, 0x1F, 0x25, 0x07, 0x1B, 0x0B, 0x1B, 0x1B, 0x04, 0x21,
  0x26, 0x20, 0x0F, 0x0B, 0x10, 0x15, 0x10, 0x1F, 0x15, 0x02, 0x0B, 0x12, 0x1C, 0x06, 0x03, 0x0F,
  0x10, 0x21, 0x17, 0x23, 0x1B, 0x0A, 0x09, 0x1C, 0x14, 0x1D, 0x22, 0x0C, 0x0D, 0x16, 0x01, 0x0E,
  0x21, 0x0C, 0x0E, 0x1A, 0x0F, 0x14, 0x1E, 0x01, 0x15, 0x15, 0x16, 0x03, 0x05, 0x10, 0x11, 0x0C,
  0x24, 0x17, 0x1C, 0x26, 0x10, 0x06, 0x10, 0x0F, 0x1D, 0x0C, 0x17, 0x02, 0x0C, 0x04, 0x16, 0x20,
  0x26, 0x02, 0x09, 0x0F, 0x1E, 0x08, 0x13, 0x0B, 0x1B, 0x0D, 0x24, 0x0E, 0x15, 0x1A, 0x17, 0x17,
  0x09, 0x14, 0x23, 0x19, 0x13, 0x16, 0x1D, 0x15, 0x08, 0x25, 0x0A, 0x16, 0x24, 0x0A, 0x06, 0x09,
  0x0C, 0x06, 0x21, 0x19, 0x07, 0x24, 0x1D, 0x10, 0x1C, 0x16, 0x19, 0x21, 0x25, 0x14, 0x12, 0x03,
  0x0B, 0x0B, 0x26, 0x1F, 0x15, 0x26, 0x14, 0x19, 0x12, 0x23, 0x11, 0x04, 0x1C, 0x12, 0x11, 0x0C,
  0x0F, 0x06, 0x25, 0x1A, 0x03, 0x1D, 0x12, 0x07, 0x13, 0x04, 0x02, 0x23, 0x20, 0x0C, 0x23, 0x03,
  0x07, 0x22, 0x21, 0x02, 0x24, 0x19, 0x1D, 0x1C, 0x01, 0x01, 0x18, 0x0D, 0x0A, 0x20, 0x23, 0x18,
  0x02, 0x01, 0x0C, 0x22, 0x08, 0x12, 0x05, 0x26, 0x0C, 0x1F, 0x1B, 0x03, 0x14, 0x21, 0x24, 0x05,
  0x04, 0x11, 0x07, 0x04, 0x06, 0x0A, 0x0E, 0x04, 0x14, 0x05, 0x05, 0x23, 0x1F, 0x05, 0x11, 0x12,
  0x26, 0x23, 0x10, 0x0D, 0x03, 0x21, 0x14, 0x1E, 0x1D, 0x07, 0x0F, 0x22, 0x21, 0x02, 0x0C, 0x06,
  0x1B, 0x1B, 0x03, 0x05, 0x0C, 0x15, 0x0B, 0x26, 0x04, 0x05, 0x02, 0x0D, 0x19, 0x10, 0x15, 0x0B,
  0x24, 0x04, 0x15, 0x1E, 0x12, 0x1D, 0x1D, 0x1E, 0x02, 0x11, 0x1B, 0x1F, 0x03, 0x09, 0x21, 0x18,
  0x18, 0x1C, 0x18, 0x17, 0x1E, 0x07, 0x18, 0x20, 0x0F, 0x10, 0x03, 0x23, 0x13, 0x12, 0x1E, 0x1D,
  0x1D, 0x1C, 0x0E, 0x0A, 0x06, 0x1A, 0x1C, 0x05, 0x0D, 0x21, 0x07, 0x03, 0x12, 0x09, 0x10, 0x12,
  0x21, 0x0D, 0x0D, 0x04, 0x21, 0x02, 0x13, 0x14, 0x0C, 0x18, 0x0B, 0x26, 0x16, 0x1E, 0x01, 0x19,
  0x04, 0x01, 0x26, 0x06, 0x23, 0x0D, 0x0C, 0x1C, 0x01, 0x12, 0x15, 0x17, 0x10, 0x24, 0x01, 0x0B,
  0x09, 0x16, 0x06, 0x20, 0x26, 0x18, 0x03, 0x10, 0x1E, 0x20, 0x26, 0x02, 0x0E, 0x02, 0x13, 0x02,
  0x08, 0x1D, 0x1E, 0x23, 0x07, 0x01, 0x22, 0x01, 0x10, 0x26, 0x10, 0x13, 0x05, 0x02, 0x19, 0x14,
  0x23, 0x04, 0x24, 0x05, 0x1F, 0x1F, 0x07, 0x13, 0x1F, 0x22, 0x11, 0x09, 0x13, 0x1F, 0x06, 0x15,
  0x1A, 0x18, 0x0F, 0x06, 0x19, 0x26, 0x21, 0x1E, 0x0F, 0x26, 0x09, 0x12, 0x0C, 0x06, 0x11, 0x13,
  0x15, 0x08, 0x0A, 0x15, 0x1B, 0x0F, 0x26, 0x04, 0x05, 0x12, 0x04, 0x0D, 0x22, 0x0D, 0x06, 0x14,
  0x0A, 0x11, 0x08, 0x01, 0x23, 0x21, 0x02, 0x11, 0x11, 0x1C, 0x14, 0x19, 0x0E, 0x02, 0x04, 0x1A,
  0x05, 0x15, 0x13, 0x13, 0x0B, 0x06, 0x19, 0x16, 0x03, 0x1E, 0x01, 0x21, 0x0F, 0x03, 0x0B, 0x23,
  0x14, 0x10, 0x0D, 0x11, 0x1D, 0x1D, 0x11, 0x13, 0x0F, 0x19, 0x0B, 0x0D, 0x09, 0x10, 0x1C, 0x0B,
  0x09, 0x12, 0x20, 0x02, 0x0E, 0x1D, 0x15, 0x22, 0x17, 0x18, 0x08, 0x0E, 0x03, 0x0D, 0x26, 0x08,
  0x19, 0x09, 0x0C, 0x14, 0x0B, 0x1B, 0x18, 0x19, 0x24, 0x01, 0x22, 0x02, 0x11, 0x0D, 0x19, 0x18,
  0x21, 0x24, 0x16, 0x02, 0x25, 0x1A, 0x23, 0x09, 0x11, 0x02, 0x12, 0x0F, 0x08, 0x21, 0x07, 0x09,
  0x07, 0x15, 0x25, 0x20, 0x0A, 0x16, 0x0B, 0x1F, 0x1F, 0x06, 0x18, 0x1A, 0x04, 0x16, 0x1E, 0x1A,
  0x11, 0x1F, 0x21, 0x01, 0x1E, 0x23, 0x1D, 0x05, 0x05, 0x13, 0x04, 0x13, 0x05, 0x0C, 0x26, 0x10,
  0x04, 0x05, 0x24, 0x0D, 0x16, 0x26, 0x01, 0x07, 0x1D, 0x19, 0x19, 0x15, 0x07, 0x02, 0x1F, 0x26,
  0x0B, 0x0B, 0x15, 0x24, 0x12, 0x01, 0x21, 0x07, 0x0A, 0x23, 0x07, 0x26, 0x07, 0x26, 0x23, 0x07,
  0x17, 0x0A, 0x0D, 0x04, 0x01, 0x06, 0x0D, 0x01, 0x0F, 0x14, 0x0C, 0x11, 0x0A, 0x14, 0x02, 0x0D,
  0x23, 0x26, 0x0D, 0x22, 0x26, 0x10, 0x23, 0x07, 0x08, 0x1C, 0x22, 0x0E, 0x0F, 0x03, 0x1D, 0x16,
  0x0B, 0x20, 0x1F, 0x13, 0x21, 0x1D, 0x1E, 0x26, 0x17, 0x24, 0x11, 0x16, 0x01, 0x19, 0x25, 0x1C,
  0x1C, 0x12, 0x0C, 0x1A, 0x20, 0x20, 0x26, 0x20, 0x21, 0x0A, 0x03, 0x1F, 0x14, 0x03, 0x24, 0x1F,
  0x20, 0x0A, 0x21, 0x22, 0x17, 0x11, 0x1B, 0x05, 0x1D, 0x04, 0x08, 0x10, 0x18, 0x0B, 0x1D, 0x26,
  0x1A, 0x1F, 0x24, 0x25, 0x07, 0x05, 0x25, 0x1B, 0x19, 0x19, 0x1C, 0x01, 0x0D, 0x11, 0x22, 0x0D,
  0x21, 0x01, 0x1A, 0x13, 0x1F, 0x17, 0x15, 0x03, 0x01, 0x15, 0x16, 0x21, 0x1E, 0x20, 0x12, 0x0D,
  0x12, 0x20, 0x26, 0x26, 0x0E, 0x05, 0x03, 0x11, 0x03, 0x04, 0x16, 0x13, 0x13, 0x08, 0x08, 0x25,
  0x04, 0x15, 0x05, 0x12, 0x21, 0x0B, 0x1F, 0x0E, 0x23, 0x1A, 0x1D, 0x12, 0x12, 0x05, 0x16, 0x1C,
  0x1D, 0x11, 0x03, 0x25, 0x13, 0x13, 0x17, 0x20, 0x08, 0x0B, 0x1B, 0x0F, 0x12, 0x0C, 0x01, 0x16,
  0x1A, 0x01, 0x0D, 0x1F, 0x10, 0x1A, 0x13, 0x26, 0x01, 0x1B, 0x0E, 0x26, 0x22, 0x08, 0x01, 0x20,
  0x17, 0x10, 0x12, 0x07, 0x04, 0x0E, 0x23, 0x11, 0x09, 0x18, 0x15, 0x0D, 0x19, 0x0B, 0x0A, 0x1D,
  0x26, 0x06, 0x13, 0x1E, 0x0B, 0x0F, 0x1C, 0x03, 0x08, 0x1B, 0x21, 0x0E, 0x0E, 0x17, 0x15, 0x02,
  0x17, 0x16, 0x05, 0x23, 0x02, 0x21, 0x09, 0x17, 0x02, 0x16, 0x1B, 0x07, 0x13, 0x15, 0x21, 0x13,
  0x03, 0x12, 0x11, 0x0C, 0x20, 0x1B, 0x25, 0x0E, 0x0A, 0x0C, 0x05, 0x02, 0x22, 0x02, 0x25, 0x10,
  0x0D, 0x15, 0x06, 0x0D, 0x18, 0x1D, 0x04, 0x0F, 0x16, 0x0C, 0x09, 0x14, 0x12, 0x1D, 0x0E, 0x08,
  0x22, 0x05, 0x13, 0x03, 0x06, 0x1F, 0x17, 0x0D, 0x22, 0x09, 0x25, 0x1E, 0x1F, 0x1D, 0x24, 0x07,
  0x02, 0x24, 0x12, 0x16, 0x07, 0x25, 0x20, 0x1F, 0x0A, 0x0B, 0x15, 0x22, 0x18, 0x0C, 0x04, 0x1D,
  0x13, 0x23, 0x12, 0x0E, 0x17, 0x1E, 0x20, 0x1E, 0x15, 0x19, 0x0E, 0x14, 0x03, 0x23, 0x1B, 0x0F,
  0x05, 0x1E, 0x01, 0x09, 0x24, 0x18, 0x19, 0x01, 0x05, 0x07, 0x0C, 0x0C, 0x23, 0x08, 0x02, 0x1A,
  0x0A, 0x20, 0x20, 0x0D, 0x19, 0x02, 0x14, 0x05, 0x11, 0x0D, 0x06, 0x19, 0x0D, 0x1C, 0x09, 0x21,
  0x19, 0x01, 0x14, 0x10, 0x1F, 0x22, 0x09, 0x25, 0x1C, 0x0E, 0x02, 0x06, 0x11, 0x1B, 0x22, 0x1D,
  0x18, 0x0B, 0x11, 0x0E, 0x14, 0x07, 0x24, 0x11, 0x23, 0x0A, 0x1C, 0x0B, 0x1A, 0x19, 0x04, 0x03,
  0x0D, 0x12, 0x1C, 0x10, 0x0E, 0x23, 0x09, 0x0B, 0x12, 0x05, 0x16, 0x0D, 0x05, 0x0F, 0x05, 0x22,
  0x0F, 0x23, 0x1D, 0x16, 0x26, 0x16, 0x15, 0x04, 0x11, 0x10, 0x1A, 0x25, 0x22, 0x1E, 0x02, 0x26,
  0x11, 0x18, 0x26, 0x0D, 0x23, 0x08, 0x03, 0x07, 0x21, 0x05, 0x0E, 0x0B, 0x23, 0x19, 0x06, 0x09,
  0x18, 0x24, 0x1B, 0x0B, 0x10, 0x03, 0x06, 0x22, 0x16, 0x15, 0x23, 0x10, 0x13, 0x02, 0x1C, 0x05,
  0x19, 0x0D, 0x0A, 0x18, 0x04, 0x1C, 0x0F, 0x01, 0x08, 0x06, 0x09, 0x12, 0x11, 0x06, 0x08, 0x18,
  0x24, 0x20, 0x03, 0x14, 0x18, 0x1A, 0x07, 0x0A, 0x10, 0x21, 0x22, 0x0F, 0x15, 0x1C, 0x1D, 0x05,
  0x20, 0x21, 0x11, 0x18, 0x0B, 0x0E, 0x1D, 0x11, 0x04, 0x20, 0x16, 0x07, 0x07, 0x1C, 0x1D, 0x10,
  0x1F, 0x06, 0x14, 0x09, 0x22, 0x19, 0x09, 0x02, 0x09, 0x03, 0x26, 0x24, 0x11, 0x10, 0x0E, 0x05,
  0x23, 0x11, 0x20, 0x04, 0x05, 0x22, 0x0A, 0x0C, 0x17, 0x0D, 0x0A, 0x13, 0x10, 0x10, 0x25, 0x02,
  0x10, 0x0D, 0x0F, 0x17, 0x02, 0x23, 0x03, 0x22, 0x14, 0x07, 0x22, 0x13, 0x15, 0x1F, 0x0D, 0x0D,
  0x10, 0x1C, 0x16, 0x0E, 0x1B, 0x13, 0x12, 0x1B, 0x0B, 0x0C, 0x11, 0x14, 0x1B, 0x13, 0x24, 0x25,
  0x11, 0x09, 0x0B, 0x11, 0x13, 0x1A, 0x25, 0x26, 0x0D, 0x12, 0x14, 0x24, 0x1D, 0x26, 0x05, 0x0B,
  0x14, 0x0E, 0x19, 0x0F, 0x1B, 0x12, 0x1F, 0x09, 0x14, 0x14, 0x07, 0x1C, 0x26, 0x0E, 0x10, 0x15,
  0x03, 0x20, 0x06, 0x19, 0x0F, 0x03, 0x16, 0x0C, 0x15, 0x1E, 0x23, 0x11, 0x18, 0x1B, 0x1D, 0x15,
  0x0A, 0x20, 0x06, 0x0E, 0x10, 0x07, 0x17, 0x24, 0x0D, 0x13, 0x01, 0x12, 0x07, 0x06, 0x04, 0x1D,
  0x18, 0x05, 0x0F, 0x15, 0x0E, 0x13, 0x22, 0x0D, 0x0C, 0x10, 0x11, 0x1F, 0x1D, 0x23, 0x11, 0x0B,
  0x14, 0x20, 0x14, 0x16, 0x26, 0x24, 0x1D, 0x07, 0x03, 0x0C, 0x07, 0x1A, 0x03, 0x18, 0x06, 0x25,
  0x21, 0x0C, 0x18, 0x0C, 0x19, 0x1D, 0x14, 0x07, 0x09, 0x20, 0x16, 0x1B, 0x1E, 0x1D, 0x0B, 0x1F,
  0x21, 0x25, 0x15, 0x25, 0x0E, 0x10, 0x07, 0x11, 0x07, 0x1A, 0x15, 0x18, 0x07, 0x22, 0x18, 0x16,
  0x18, 0x18, 0x23, 0x23, 0x12, 0x1E, 0x20, 0x22, 0x04, 0x12, 0x07, 0x01, 0x18, 0x08, 0x0C, 0x12,
  0x10, 0x26, 0x0D, 0x19, 0x21, 0x11, 0x18, 0x1C, 0x13, 0x0C, 0x04, 0x13, 0x1D, 0x09, 0x0A, 0x11,
  0x1F, 0x08, 0x25, 0x0D, 0x1F, 0x0E, 0x0A, 0x0A, 0x15, 0x1A, 0x04, 0x09, 0x0F, 0x0B, 0x23, 0x12,
  0x02, 0x0D, 0x16, 0x24, 0x16, 0x08, 0x19, 0x26, 0x0E, 0x21, 0x1B, 0x02, 0x20, 0x24, 0x0C, 0x16,
  0x12, 0x15, 0x25, 0x18, 0x0B, 0x08, 0x08, 0x03, 0x12, 0x13, 0x07, 0x1A, 0x0E, 0x15, 0x1B, 0x21,
  0x1C, 0x24, 0x11, 0x1F, 0x16, 0x03, 0x1B, 0x11, 0x19, 0x0E, 0x17, 0x25, 0x18, 0x1F, 0x18, 0x17,
  0x16, 0x15, 0x0C, 0x08, 0x11, 0x15, 0x0B, 0x1F, 0x13, 0x18, 0x1C, 0x0C, 0x1D, 0x17, 0x18, 0x20,
  0x0E, 0x20, 0x23, 0x26, 0x17, 0x1E, 0x07, 0x22, 0x1F, 0x25, 0x06, 0x02, 0x18, 0x25, 0x26, 0x09,
  0x1C, 0x1C, 0x22, 0x24, 0x1A, 0x25, 0x04, 0x0A, 0x17, 0x1E, 0x05, 0x05, 0x24, 0x14, 0x1B, 0x25,
  0x17, 0x19, 0x0B, 0x12, 0x24, 0x0F, 0x04, 0x17, 0x10, 0x1E, 0x0A, 0x1C, 0x17, 0x07, 0x19, 0x0E,
  0x16, 0x16, 0x0C, 0x1C, 0x18, 0x06, 0x17, 0x1E, 0x19, 0x18, 0x15, 0x01, 0x15, 0x25, 0x01, 0x10,
  0x19, 0x1D, 0x0E, 0x1C, 0x11, 0x23, 0x16, 0x03, 0x07, 0x1A, 0x16, 0x19, 0x09, 0x24, 0x22, 0x1E,
  0x24, 0x15, 0x23, 0x1B, 0x14, 0x23, 0x14, 0x1D, 0x0E, 0x0A, 0x1F, 0x24, 0x1E, 0x19, 0x19, 0x02,
  0x03, 0x20, 0x02, 0x0B, 0x10, 0x02, 0x25, 0x17, 0x11, 0x1E, 0x11, 0x1E, 0x02, 0x20, 0x12, 0x1D,
  0x02, 0x23, 0x0A, 0x0C, 0x16, 0x03, 0x26, 0x20, 0x0F, 0x0B, 0x16, 0x10, 0x21, 0x18, 0x02, 0x05,
  0x21, 0x26, 0x14, 0x10, 0x03, 0x09, 0x10, 0x04, 0x04, 0x20, 0x16, 0x21, 0x1E, 0x17, 0x22, 0x24,
  0x14, 0x23, 0x20, 0x07, 0x01, 0x21, 0x17, 0x18, 0x0D, 0x05, 0x26, 0x05, 0x26, 0x03, 0x0C, 0x1C,
  0x17, 0x08, 0x05, 0x19, 0x18, 0x25, 0x01, 0x0B, 0x1D, 0x26, 0x0A, 0x0A, 0x08, 0x08, 0x03, 0x04,
  0x21, 0x02, 0x26, 0x20, 0x04, 0x0B, 0x1B, 0x1C, 0x01, 0x0B, 0x1D, 0x0F, 0x19, 0x24, 0x09, 0x18,
  0x24, 0x1A, 0x05, 0x13, 0x1C, 0x1B, 0x05, 0x0A, 0x04, 0x16, 0x1E, 0x0F, 0x13, 0x23, 0x17, 0x23,
  0x24, 0x0F, 0x08, 0x06, 0x25, 0x10, 0x23, 0x02, 0x19, 0x05, 0x25, 0x06, 0x21, 0x02, 0x02, 0x24,
  0x22, 0x06, 0x26, 0x08, 0x15, 0x15, 0x22, 0x21, 0x04, 0x0F, 0x17, 0x04, 0x09, 0x25, 0x1D, 0x24,
  0x1E, 0x05, 0x05, 0x13, 0x05, 0x0E, 0x01, 0x19, 0x0D, 0x0B, 0x19, 0x23, 0x06, 0x1F, 0x06, 0x23,
  0x18, 0x1D, 0x25, 0x16, 0x0C, 0x19, 0x0B, 0x1F, 0x1A, 0x14, 0x14, 0x14, 0x18, 0x20, 0x0A, 0x1A,
  0x13, 0x19, 0x08, 0x23, 0x07, 0x16, 0x20, 0x09, 0x13, 0x1D, 0x14, 0x07, 0x17, 0x22, 0x09, 0x1E,
  0x02, 0x0B, 0x02, 0x17, 0x0C, 0x12, 0x10, 0x07, 0x13, 0x1C, 0x04, 0x16, 0x15, 0x25, 0x06, 0x10,
  0x1F, 0x23, 0x0E, 0x14, 0x07, 0x02, 0x01, 0x13, 0x17, 0x07, 0x06, 0x0E, 0x23, 0x18, 0x1C, 0x15,
  0x1E, 0x19, 0x05, 0x22, 0x06, 0x07, 0x24, 0x12, 0x17, 0x0C, 0x24, 0x1A, 0x21, 0x0F, 0x24, 0x12,
  0x13, 0x09, 0x14, 0x0C, 0x1A, 0x09, 0x22, 0x08, 0x01, 0x0B, 0x14, 0x10, 0x1B, 0x1F, 0x0B, 0x0B,
  0x14, 0x0E, 0x10, 0x25, 0x04, 0x08, 0x14, 0x01, 0x20, 0x1F, 0x1E, 0x15, 0x20, 0x08, 0x0E, 0x21,
  0x20, 0x1B, 0x08, 0x19, 0x17, 0x20, 0x14, 0x1D, 0x06, 0x12, 0x1C, 0x21, 0x19, 0x09, 0x09, 0x06,
  0x24, 0x09, 0x16, 0x20, 0x12, 0x18, 0x04, 0x0E, 0x1A, 0x15, 0x0A, 0x22, 0x25, 0x0E, 0x0A, 0x16,
  0x01, 0x0C, 0x1E, 0x20, 0x0D, 0x03, 0x03, 0x24, 0x24, 0x25, 0x13, 0x0C, 0x21, 0x22, 0x13, 0x1C,
  0x22, 0x0C, 0x24, 0x1E, 0x08, 0x1F, 0x0F, 0x14, 0x25, 0x1C, 0x1C, 0x20, 0x01, 0x0F, 0x13, 0x17,
  0x03, 0x1F, 0x04, 0x1C, 0x17, 0x1F, 0x05, 0x03, 0x04, 0x1C, 0x17, 0x16, 0x09, 0x05, 0x1E, 0x0A,
  0x1F, 0x14, 0x14, 0x0E, 0x1C, 0x03, 0x14, 0x1A, 0x09, 0x06, 0x24, 0x1F, 0x0C, 0x21, 0x25, 0x25,
  0x25, 0x0F, 0x25, 0x0F, 0x21, 0x25, 0x14, 0x20, 0x17, 0x09, 0x1A, 0x25, 0x25, 0x0F, 0x15, 0x0E,
  0x0D, 0x24, 0x0C, 0x25, 0x0D, 0x1C, 0x20, 0x25, 0x0F, 0x1E, 0x24, 0x21, 0x08, 0x0E, 0x08, 0x04,
  0x22, 0x04, 0x0A, 0x01, 0x0F, 0x21, 0x0E, 0x1A, 0x0C, 0x0A, 0x22, 0x01, 0x0E, 0x04, 0x06, 0x09,
  0x01, 0x17, 0x25, 0x1F, 0x1C, 0x09, 0x0B, 0x04, 0x20, 0x1A, 0x22, 0x1F, 0x25, 0x03, 0x01, 0x1B,
  0x1E, 0x22, 0x06, 0x1B, 0x1B, 0x0F, 0x17, 0x24, 0x0F, 0x22, 0x01, 0x0A, 0x01, 0x17, 0x09, 0x17,
  0x1B, 0x1B, 0x08, 0x12, 0x08, 0x1B, 0x1E, 0x0D, 0x0F, 0x17, 0x06, 0x06, 0x08, 0x17, 0x1E, 0x09,
  0x1A, 0x06, 0x0A, 0x1E, 0x0A, 0x1C, 0x01, 0x06, 0x1C, 0x08, 0x01, 0x1E, 0x1A, 0x06, 0x0A, 0x06,
  0x1E, 0x1F, 0x0F, 0x0D, 0x09, 0x12, 0x0A, 0x22, 0x22, 0x0A, 0x0A, 0x1B, 0x08, 0x08, 0x1B, 0x22,
  0x1E, 0x1F, 0x1F, 0x0A, 0x1E, 0x0A, 0x22, 0x0F, 0x0A, 0x1B, 0x08, 0x22, 0x08, 0x1A, 0x1A, 0x0D,
  0x1A, 0x0F, 0x1E, 0x1B, 0x1B, 0x12, 0x1B, 0x1E, 0x08, 0x08, 0x0F, 0x08, 0x0F, 0x08, 0x08, 0x1A,
  0x1A, 0x0F, 0x1B, 0x12, 0x0F, 0x0A, 0x1A, 0x1B, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0x1A, 0xFF, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE7,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x0E, 0x0A, 0x55,
  0x02, 0x00, 0x2A, 0x40, 0x02, 0x00, 0x20, 0x51, 0x02, 0x00, 0x0A, 0x54, 0x3C, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x78, 0x70, 0x07, 0x2A, 0x65, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x15, 0x50, 0x2A, 0x21, 0x55, 0x6A, 0x57, 0x3A, 0x7D, 0x3A, 0x77, 0x3A, 0x77, 0x7A,
  0x55, 0x3A, 0x57, 0x6A, 0x7D, 0x2A, 0x57, 0x3A, 0x5D, 0x3A, 0x5D, 0x7A, 0x55, 0x0A, 0x2A, 0x00,
  0x28, 0x55, 0x02, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x04, 0x80, 0x10, 0xD8, 0x08, 0xD8, 0x0D, 0xA3,
  0x55, 0x2A, 0x75, 0x2B, 0x55, 0x2B, 0x5D, 0x2B, 0x55, 0x2B, 0x5D, 0x3E, 0x75, 0x2B, 0x55, 0x2A,
  0x55, 0x2A, 0x7D, 0x2F, 0x5D, 0x2B, 0x7D, 0x2A, 0x75, 0x2B, 0x7D, 0x2A, 0x5D, 0x3B, 0x75, 0x2B,
  0x75, 0x2B, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x1C, 0x2A, 0x55,
  0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x2A, 0x01, 0x30, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x0A, 0x73, 0x19, 0x2A, 0x05, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0xFF, 0x42, 0x00, 0x44, 0xA0, 0xC4, 0x88, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x0A, 0x2A, 0x55,
  0x0A, 0x01, 0x0A, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x00, 0x13, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x1C, 0x0A, 0x40,
  0x2A, 0x00, 0x00, 0x55, 0x02, 0x00, 0x20, 0x15, 0x00, 0x00, 0x0A, 0x40, 0x4A, 0x29, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x31, 0x78, 0x28, 0x15, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x15, 0x84, 0x20, 0x90, 0xA2, 0xC4, 0x88, 0x41, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x40, 0x2A, 0x15, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x50, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x01, 0x00, 0x52, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x03, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x28, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x18, 0x23, 0x15, 0x4C, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x15, 0x00, 0x88, 0x91, 0xA2, 0x84, 0x2A, 0x15, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x02,
  0x28, 0x55, 0x02, 0x55, 0x0A, 0x54, 0x42, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x46, 0x02, 0x50, 0x00, 0x00, 0x2A, 0x41, 0x0A, 0x00, 0x28, 0x54, 0x00, 0x00,
  0x0A, 0x50, 0x02, 0x00, 0x0A, 0x50, 0x32, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x60, 0x7F, 0x1F,
  0x2A, 0x45, 0x2A, 0x21, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x0A, 0x3F, 0x54, 0x02, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x15, 0xC0, 0x88, 0x41, 0x2A, 0x15, 0x0A, 0x50, 0x2A, 0x55, 0x2A, 0x01, 0x44, 0x01, 0x44, 0x2A,
  0x00, 0x50, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x01, 0x44, 0x2A, 0x00, 0x50, 0x2A, 0x55, 0x2A, 0x55,
  0x2A, 0x10, 0x22, 0x40, 0x28, 0x55, 0x2A, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x06, 0x28, 0x15, 0x00, 0x00, 0x0A, 0x05, 0x2A, 0x00, 0x28, 0x55, 0x2A, 0x00,
  0x0A, 0x15, 0x00, 0x00, 0x2A, 0x01, 0x30, 0x28, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x79, 0x3F, 0x40,
  0x2A, 0x15, 0x0F, 0x28, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x3A,
  0x5D, 0x2A, 0x5F, 0x2A, 0x5D, 0x2A, 0x75, 0x6A, 0x75, 0x7A, 0x5D, 0x2A, 0x75, 0x2F, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x41, 0x51, 0x2A, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x29, 0x55, 0x2A, 0x15, 0x02, 0xC4, 0x88, 0x01, 0x50, 0x2A, 0x0A, 0x55, 0x2A, 0x55, 0x2A, 0x55,
  0x2A, 0x55, 0x50, 0x2A, 0x00, 0x01, 0x0A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x0A, 0x11, 0x50, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x20, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x46, 0x0A, 0x50, 0x0A, 0x00, 0x0A, 0x54, 0x2A, 0x00, 0x28, 0x40, 0x2A, 0x00,
  0x0A, 0x54, 0x0A, 0x00, 0x0A, 0x40, 0x4A, 0x29, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x07, 0x7F, 0x47,
  0x2A, 0x65, 0x3F, 0x02, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x7E,
  0x7F, 0x2A, 0x5F, 0x2A, 0x75, 0x3F, 0x55, 0x2A, 0x7D, 0x3F, 0x75, 0x3F, 0x7D, 0x3E, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x40, 0x0A, 0x28, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x15, 0x84, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x66, 0x03, 0x46,
  0x0A, 0x60, 0x4F, 0x2A, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x42, 0x0A, 0x29, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x75, 0x2A, 0x7D, 0x2A, 0x7F, 0x2B, 0x7D, 0x2A, 0x5F, 0x2B, 0x75, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x7F, 0x2B, 0x7F, 0x2B, 0x7F, 0x2B, 0x75, 0x2A, 0x7F, 0x2B, 0x5F, 0x2B, 0x7D, 0x2B,
  0x75, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x1C, 0x0A, 0x55,
  0x02, 0x00, 0x2A, 0x40, 0x02, 0x00, 0x20, 0x51, 0x02, 0x00, 0x0A, 0x54, 0x30, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x60, 0x7C, 0x19, 0x0A, 0x64, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x15, 0x40, 0x2A, 0x00, 0x55, 0x2A, 0x5F, 0x3A, 0x7D, 0x3A, 0x77, 0x3A, 0x77, 0x3A,
  0x5D, 0x3A, 0x5F, 0x6A, 0x5F, 0x2A, 0x5F, 0x3A, 0x5D, 0x3A, 0x5D, 0x3A, 0x5D, 0x4A, 0x2A, 0x55,
  0x2A, 0x55, 0x28, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x2F, 0x55, 0x2F, 0x5D, 0x2F, 0x55, 0x2F, 0x5D, 0x3E, 0x55, 0x2F, 0x55, 0x2A,
  0x55, 0x2A, 0x5D, 0x2A, 0x5D, 0x2F, 0x5D, 0x2E, 0x55, 0x2F, 0x5D, 0x2E, 0x5D, 0x3B, 0x75, 0x2B,
  0x55, 0x2F, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x1C, 0x2A, 0x55,
  0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x20, 0x01, 0x00, 0x00, 0x2A, 0x01, 0x30, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x0A, 0x3F, 0x18, 0x2A, 0x65, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0xFF, 0x12, 0x08, 0x00, 0x02, 0x91, 0xA2, 0x28, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x0A, 0x22, 0x55,
  0x0A, 0x41, 0x2A, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x05, 0x1C, 0x0A, 0x40,
  0x2A, 0x00, 0x00, 0x55, 0x02, 0x00, 0x20, 0x15, 0x00, 0x00, 0x0A, 0x40, 0x4A, 0x29, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x01, 0x78, 0x28, 0x15, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x15, 0x00, 0x20, 0xC4, 0x88, 0x91, 0xA2, 0x44, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x02, 0x20, 0x15, 0x54, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x78, 0x28, 0x01, 0x00, 0x50, 0x02, 0x14, 0x00, 0x40, 0x2A, 0x01, 0x00, 0x50,
  0x02, 0x05, 0x00, 0x50, 0x2A, 0x65, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x0A,
  0x20, 0x05, 0x42, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x05, 0x7E, 0x7F, 0x7F,
  0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x3F, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x00, 0x23, 0x55, 0x4C, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x15, 0x00, 0xA2, 0xC4, 0x88, 0x40, 0x2A, 0x15, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x40,
  0x2A, 0x55, 0x02, 0x55, 0x2A, 0x50, 0x4A, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x46, 0x02, 0x40, 0x02, 0x00, 0x2A, 0x01, 0x2A, 0x00, 0x28, 0x50, 0x02, 0x00,
  0x0A, 0x54, 0x02, 0x00, 0x0A, 0x40, 0x32, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x78, 0x03, 0x00,
  0x2A, 0x05, 0x28, 0x25, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x0A, 0x3C, 0x54, 0x0A, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x15, 0x90, 0xA2, 0x50, 0x2A, 0x05, 0x00, 0x50, 0x2A, 0x55, 0x2A, 0x55, 0x00, 0x55, 0x00, 0x00,
  0x28, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x00, 0x00, 0x28, 0x55, 0x2A, 0x55, 0x2A, 0x55,
  0x22, 0x11, 0x02, 0x00, 0x20, 0x55, 0x2A, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x06, 0x00, 0x54, 0x00, 0x00, 0x0A, 0x14, 0x2A, 0x00, 0x28, 0x55, 0x2A, 0x00,
  0x0A, 0x54, 0x00, 0x00, 0x0A, 0x40, 0x72, 0x29, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x79, 0x03, 0x46,
  0x2A, 0x04, 0x30, 0x28, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x7A,
  0x5F, 0x2A, 0x5F, 0x2A, 0x5D, 0x2A, 0x75, 0x6A, 0x75, 0x7A, 0x5D, 0x2A, 0x75, 0x2B, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x51, 0x2A, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x29, 0x55, 0x2A, 0x01, 0x00, 0x91, 0x82, 0x01, 0x55, 0x2A, 0x0A, 0x45, 0x2A, 0x45, 0x2A, 0x55,
  0x2A, 0x15, 0x55, 0x2A, 0x05, 0x00, 0x0A, 0x45, 0x2A, 0x55, 0x2A, 0x15, 0x0A, 0x00, 0x54, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x06, 0x2A, 0x55, 0x0A, 0x40, 0x0A, 0x54, 0x2A, 0x00, 0x2A, 0x40, 0x2A, 0x40,
  0x0A, 0x54, 0x0A, 0x40, 0x2A, 0x55, 0x4A, 0x29, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x61, 0x7F, 0x47,
  0x2A, 0x05, 0x30, 0x02, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x42, 0x2A, 0x29, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x15, 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x66, 0x70, 0x47,
  0x2A, 0x78, 0x4F, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x6A,
  0x55, 0x6A, 0x55, 0x3E, 0x57, 0x3E, 0x57, 0x7E, 0x57, 0x7E, 0x57, 0x3E, 0x57, 0x7A, 0x55, 0x3E,
  0x55, 0x3E, 0x55, 0x7E, 0x57, 0x2A, 0x55, 0x2A, 0x55, 0x40, 0x0A, 0x28, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x7D, 0x2B, 0x75, 0x2A, 0x5D, 0x2F, 0x75, 0x2A, 0x5D, 0x2B, 0x7D, 0x2B, 0x55, 0x2A,
  0x55, 0x2A, 0x7D, 0x2B, 0x5D, 0x2F, 0x7D, 0x2B, 0x7D, 0x2B, 0x7D, 0x2B, 0x5D, 0x2E, 0x7F, 0x2B,
  0x7D, 0x2B, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x1C, 0x0A, 0x54,
  0x02, 0x00, 0x28, 0x01, 0x0A, 0x00, 0x20, 0x01, 0x0A, 0x00, 0x0A, 0x50, 0x32, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x62, 0x3C, 0x18, 0x2A, 0x60, 0x50, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x05, 0x44, 0x2A, 0x10, 0x55, 0x3A, 0x5D, 0x3A, 0x7D, 0x3A, 0x75, 0x3A, 0x75, 0x3A,
  0x7D, 0x3A, 0x5F, 0x6A, 0x5F, 0x2A, 0x5F, 0x3A, 0x7D, 0x3A, 0x7D, 0x3A, 0x7D, 0x4A, 0x2A, 0x55,
  0x2A, 0x55, 0x08, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x5D, 0x2E, 0x55, 0x2F, 0x5D, 0x2F, 0x55, 0x2F, 0x5D, 0x3E, 0x5D, 0x2E, 0x55, 0x2A,
  0x55, 0x2A, 0x5D, 0x2A, 0x5D, 0x2F, 0x5D, 0x3E, 0x5D, 0x2E, 0x5D, 0x3E, 0x5D, 0x3B, 0x75, 0x2B,
  0x5D, 0x2E, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x1C, 0x2A, 0x55,
  0x02, 0x00, 0x00, 0x15, 0x00, 0x00, 0x20, 0x05, 0x00, 0x00, 0x2A, 0x01, 0x30, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x0A, 0x3C, 0x1E, 0x2A, 0x65, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0xFF, 0x42, 0x08, 0x11, 0xA0, 0xC4, 0x88, 0x21, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x0A, 0x20, 0x44,
  0x00, 0x50, 0x2A, 0x28, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x05, 0x0C, 0x0A, 0x40,
  0x2A, 0x00, 0x00, 0x55, 0x02, 0x00, 0x20, 0x15, 0x00, 0x00, 0x0A, 0x40, 0x4A, 0x29, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x1F, 0x28, 0x15, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x15, 0x84, 0x00, 0x90, 0xA2, 0xC4, 0x88, 0x41, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x0A, 0x28, 0x55, 0x54, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x18, 0x28, 0x01, 0x00, 0x50, 0x02, 0x14, 0x00, 0x40, 0x2A, 0x01, 0x00, 0x50,
  0x02, 0x05, 0x00, 0x50, 0x2A, 0x05, 0x43, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x02,
  0x28, 0x55, 0x4A, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x78, 0x03, 0x55, 0x40, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x15, 0xC4, 0x88, 0x91, 0x82, 0x54, 0x2A, 0x45, 0x44, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x05, 0x00, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x01, 0x54,
  0x2A, 0x55, 0x0A, 0x41, 0x2A, 0x41, 0x4A, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x46, 0x02, 0x40, 0x02, 0x00, 0x2A, 0x01, 0x2A, 0x00, 0x28, 0x50, 0x02, 0x00,
  0x0A, 0x54, 0x02, 0x00, 0x0A, 0x40, 0x32, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x18, 0x70, 0x1F,
  0x2A, 0x05, 0x20, 0x25, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x28, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x3C, 0x54, 0x08, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x05, 0xC0, 0x88, 0x54, 0x2A, 0x45, 0x00, 0x51, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55,
  0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15,
  0x02, 0x00, 0x50, 0x02, 0x22, 0x55, 0x0A, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x06, 0x00, 0x54, 0x00, 0x00, 0x0A, 0x14, 0x2A, 0x00, 0x28, 0x55, 0x2A, 0x00,
  0x0A, 0x54, 0x00, 0x00, 0x0A, 0x40, 0x42, 0x29, 0x55, 0x2A, 0x55, 0x2A, 0x05, 0x78, 0x70, 0x47,
  0x2A, 0x60, 0x43, 0x29, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x7A,
  0x5F, 0x2A, 0x57, 0x2A, 0x5D, 0x3A, 0x75, 0x7F, 0x75, 0x7A, 0x5D, 0x3A, 0x75, 0x2E, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x51, 0x2A, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x29, 0x55, 0x2A, 0x45, 0xA0, 0xC4, 0x88, 0x28, 0x55, 0x2A, 0x00, 0x44, 0x22, 0x44, 0x22, 0x54,
  0x2A, 0x10, 0x55, 0x2A, 0x55, 0x02, 0x00, 0x44, 0x22, 0x54, 0x2A, 0x10, 0x28, 0x20, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x1E, 0x2A, 0x55, 0x0A, 0x40, 0x0A, 0x54, 0x2A, 0x00, 0x2A, 0x40, 0x2A, 0x40,
  0x0A, 0x54, 0x0A, 0x40, 0x2A, 0x55, 0x4A, 0x29, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x79, 0x7F, 0x47,
  0x2A, 0x05, 0x03, 0x02, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x42, 0x2A, 0x29, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x2A, 0x15, 0x7E, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x7F, 0x7F, 0x7F, 0x7F, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x7E, 0x7C, 0x47,
  0x2A, 0x19, 0x40, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x20, 0x55, 0x7A,
  0x57, 0x7A, 0x57, 0x3A, 0x5D, 0x3A, 0x5D, 0x7A, 0x57, 0x3A, 0x5F, 0x3A, 0x5D, 0x6A, 0x55, 0x3A,
  0x55, 0x3A, 0x55, 0x7A, 0x57, 0x2A, 0x55, 0x00, 0x54, 0x40, 0x2A, 0x29, 0x55, 0x2A, 0x55, 0x2A,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x7D, 0x2B, 0x55, 0x2B, 0x5D, 0x2E, 0x55, 0x2B, 0x5D, 0x2E, 0x7D, 0x2B, 0x55, 0x2A,
  0x55, 0x2A, 0x5D, 0x2F, 0x5D, 0x2E, 0x5D, 0x2F, 0x7D, 0x2B, 0x5D, 0x2F, 0x7D, 0x2E, 0x75, 0x2E,
  0x7D, 0x2B, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x1C, 0x0A, 0x54,
  0x02, 0x00, 0x28, 0x01, 0x0A, 0x00, 0x20, 0x01, 0x0A, 0x00, 0x0A, 0x50, 0x32, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x62, 0x0C, 0x1E, 0x2A, 0x05, 0x50, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x05, 0x44, 0x0A, 0x10, 0x00, 0x3A, 0x7D, 0x3A, 0x7D, 0x3A, 0x75, 0x3A, 0x75, 0x3A,
  0x7D, 0x3A, 0x7F, 0x2A, 0x5F, 0x2A, 0x5F, 0x3A, 0x7D, 0x3A, 0x7D, 0x3A, 0x7D, 0x4A, 0x2A, 0x55,
  0x2A, 0x15, 0x40, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x5D, 0x3E, 0x55, 0x2F, 0x5D, 0x3F, 0x55, 0x2F, 0x5D, 0x3E, 0x5D, 0x3E, 0x55, 0x2A,
  0x55, 0x2A, 0x5D, 0x2A, 0x5D, 0x3F, 0x5D, 0x3E, 0x5D, 0x3E, 0x5D, 0x3E, 0x5D, 0x3B, 0x75, 0x2B,
  0x5D, 0x3E, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x1C, 0x2A, 0x55,
  0x02, 0x00, 0x00, 0x15, 0x00, 0x00, 0x20, 0x05, 0x00, 0x00, 0x2A, 0x01, 0x30, 0x28, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x0C, 0x1E, 0x28, 0x61, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0xFF, 0x12, 0x00, 0x00, 0x00, 0x91, 0xA2, 0x24, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x20, 0x04,
  0x02, 0x54, 0x2A, 0x29, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x15, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x0F, 0x0A, 0x40,
  0x2A, 0x00, 0x00, 0x55, 0x02, 0x00, 0x20, 0x15, 0x00, 0x00, 0x0A, 0x40, 0x4A, 0x29, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x07, 0x28, 0x15, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x15, 0x00, 0x08, 0xC4, 0x88, 0x91, 0xA2, 0x50, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x0A, 0x2A, 0x55, 0x00, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x1E, 0x2A, 0x05, 0x00, 0x40, 0x0A, 0x50, 0x00, 0x00, 0x2A, 0x05, 0x00, 0x40,
  0x02, 0x14, 0x00, 0x40, 0x2A, 0x15, 0x4F, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x60,
  0x28, 0x55, 0x42, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x60, 0x0F, 0x55, 0x42, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x15, 0x90, 0xA2, 0xC4, 0x00, 0x55, 0x2A, 0x01, 0x00, 0x25, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x01, 0x55, 0x40, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x0A, 0x20, 0x55,
  0x2A, 0x55, 0x0A, 0x50, 0x2A, 0x05, 0x0A, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x46, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x05, 0x2A, 0x00, 0x28, 0x50, 0x0A, 0x00,
  0x2A, 0x15, 0x00, 0x00, 0x0A, 0x00, 0x30, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x06, 0x7F, 0x1F,
  0x2A, 0x01, 0x00, 0x25, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x7E,
  0x57, 0x3E, 0x57, 0x2A, 0x75, 0x2F, 0x7D, 0x2E, 0x7D, 0x2E, 0x75, 0x2F, 0x7D, 0x2E, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x3C, 0x54, 0x00, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x01, 0x01, 0x82, 0x55, 0x2A, 0x04, 0xA0, 0x50, 0x2A, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55,
  0x2A, 0x55, 0x2A, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x10,
  0x02, 0x2A, 0x55, 0x0A, 0x20, 0x54, 0x4A, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x46, 0x02, 0x50, 0x02, 0x00, 0x0A, 0x14, 0x2A, 0x00, 0x28, 0x40, 0x2A, 0x00,
  0x0A, 0x54, 0x02, 0x00, 0x0A, 0x40, 0x4A, 0x29, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x07, 0x7C, 0x47,
  0x2A, 0x65, 0x7F, 0x21, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x7A,
  0x7F, 0x2A, 0x5F, 0x2A, 0x5D, 0x7A, 0x75, 0x7A, 0x75, 0x7A, 0x5D, 0x7A, 0x75, 0x3E, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x05, 0x50, 0x00, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x21, 0x55, 0x2A, 0x04, 0x88, 0x91, 0x42, 0x2A, 0x55, 0x2A, 0x01, 0x00, 0x02, 0x00, 0x02, 0x14,
  0x22, 0x51, 0x55, 0x2A, 0x55, 0x2A, 0x01, 0x00, 0x02, 0x14, 0x22, 0x51, 0x00, 0x28, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x18, 0x28, 0x55, 0x02, 0x50, 0x0A, 0x54, 0x0A, 0x40, 0x2A, 0x40, 0x0A, 0x50,
  0x0A, 0x54, 0x02, 0x50, 0x2A, 0x55, 0x42, 0x29, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x7F, 0x03, 0x40,
  0x0A, 0x65, 0x43, 0x02, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x42, 0x2A, 0x29, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x2A, 0x45, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x60, 0x50, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x1E, 0x7C, 0x47,
  0x2A, 0x01, 0x43, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x20, 0x55, 0x7A,
  0x57, 0x3A, 0x57, 0x7A, 0x5D, 0x7A, 0x5D, 0x3A, 0x5F, 0x3A, 0x5D, 0x3A, 0x5D, 0x2A, 0x57, 0x3A,
  0x55, 0x3A, 0x55, 0x3A, 0x5F, 0x2A, 0x01, 0x50, 0x40, 0x50, 0x2A, 0x29, 0x55, 0x2A, 0x55, 0x2A,
  0x00, 0x18, 0x18, 0x1C, 0x44, 0x1C, 0x38, 0x38, 0x24, 0x3E, 0x00, 0x08, 0x18, 0x18, 0x08, 0x00,
  0x1C, 0x44, 0x00, 0x30, 0x38, 0x1C, 0x38, 0x24, 0x30, 0x00, 0x30, 0x18, 0x3C, 0x3E, 0x44, 0x10,
  0x1C, 0x38, 0x00, 0x38, 0x44, 0x18, 0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x5D, 0x2E, 0x55, 0x2B, 0x5D, 0x3E, 0x55, 0x2B, 0x5D, 0x2E, 0x5D, 0x2E, 0x55, 0x2A,
  0x55, 0x2A, 0x5D, 0x2E, 0x5D, 0x3E, 0x5D, 0x2E, 0x5D, 0x2E, 0x5D, 0x2E, 0x7D, 0x3E, 0x75, 0x3A,
  0x5D, 0x2E, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x1C, 0x0A, 0x54,
  0x02, 0x00, 0x20, 0x45, 0x0A, 0x00, 0x20, 0x01, 0x00, 0x00, 0x0A, 0x40, 0x32, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x62, 0x4C, 0x1F, 0x2A, 0x65, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x05, 0x00, 0x0A, 0x00, 0xA8, 0x3A, 0x7D, 0x3A, 0x7D, 0x3A, 0x75, 0x3A, 0x75, 0x3A,
  0x7D, 0x3A, 0x7F, 0x2A, 0x5F, 0x2A, 0x5F, 0x3A, 0x7D, 0x3A, 0x7D, 0x3A, 0x7D, 0x4A, 0x2A, 0x55,
  0x2A, 0x15, 0x42, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x5D, 0x3E, 0x55, 0x2F, 0x5D, 0x3F, 0x55, 0x2F, 0x5D, 0x3E, 0x5D, 0x3E, 0x55, 0x2A,
  0x55, 0x2A, 0x5D, 0x2A, 0x5D, 0x3F, 0x5D, 0x3E, 0x5D, 0x3E, 0x5D, 0x3E, 0x5D, 0x3F, 0x75, 0x2B,
  0x5D, 0x3E, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x1C, 0x2A, 0x54,
  0x0A, 0x00, 0x00, 0x55, 0x00, 0x00, 0x20, 0x05, 0x00, 0x00, 0x0A, 0x40, 0x72, 0x29, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x4C, 0x67, 0x28, 0x05, 0x50, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x00, 0x20, 0x04, 0xA0, 0xC4, 0x88, 0x81, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x01, 0x10,
  0x00, 0x44, 0x2A, 0x21, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x4B, 0x2A, 0x55,
  0x2A, 0x00, 0x00, 0x55, 0x02, 0x00, 0x28, 0x15, 0x00, 0x40, 0x2A, 0x55, 0x4A, 0x29, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x05, 0x66, 0x28, 0x15, 0x50, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x15, 0x00, 0x00, 0x91, 0xA2, 0xC4, 0x88, 0x50, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x0A, 0x00, 0x55, 0x00, 0x00, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x06, 0x2A, 0x05, 0x00, 0x40, 0x0A, 0x50, 0x00, 0x00, 0x2A, 0x05, 0x00, 0x40,
  0x02, 0x14, 0x00, 0x40, 0x2A, 0x15, 0x0C, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x01, 0x7E,
  0x28, 0x05, 0x00, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x62, 0x0C, 0x15, 0x40, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x15, 0xC4, 0x88, 0x91, 0x20, 0x55, 0x2A, 0x11, 0x28, 0x05, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x29, 0x55, 0x0A, 0x28, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x02, 0x2A, 0x55,
  0x2A, 0x55, 0x0A, 0x54, 0x2A, 0x55, 0x2A, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x46, 0x0A, 0x00, 0x00, 0x00, 0x0A, 0x05, 0x2A, 0x00, 0x28, 0x50, 0x0A, 0x00,
  0x2A, 0x15, 0x00, 0x00, 0x0A, 0x00, 0x30, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x60, 0x0F, 0x18,
  0x2A, 0x15, 0x00, 0x25, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x7A,
  0x5F, 0x3A, 0x5D, 0x2A, 0x75, 0x2F, 0x75, 0x2E, 0x75, 0x2E, 0x75, 0x2F, 0x75, 0x2E, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x30, 0x54, 0x0A, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x21, 0x05, 0x20, 0x55, 0x0A, 0x04, 0x88, 0x54, 0x22, 0x44, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55,
  0x2A, 0x55, 0x22, 0x44, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x10,
  0x50, 0x2A, 0x55, 0x2A, 0x20, 0x44, 0x42, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x46, 0x02, 0x50, 0x02, 0x00, 0x0A, 0x14, 0x2A, 0x00, 0x28, 0x40, 0x2A, 0x00,
  0x0A, 0x54, 0x02, 0x00, 0x0A, 0x40, 0x4A, 0x29, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x61, 0x7F, 0x47,
  0x2A, 0x65, 0x7F, 0x21, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x3A,
  0x7D, 0x2A, 0x5F, 0x2A, 0x5D, 0x7A, 0x75, 0x7A, 0x75, 0x7A, 0x5D, 0x7A, 0x75, 0x3E, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x40, 0x2A, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x21, 0x55, 0x22, 0x00, 0xA2, 0xC4, 0x40, 0x2A, 0x55, 0x2A, 0x15, 0x00, 0x08, 0x00, 0x08, 0x10,
  0x00, 0x29, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x00, 0x08, 0x10, 0x00, 0x01, 0x50, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x78, 0x28, 0x55, 0x02, 0x50, 0x0A, 0x54, 0x0A, 0x40, 0x2A, 0x40, 0x0A, 0x50,
  0x0A, 0x54, 0x02, 0x50, 0x2A, 0x55, 0x72, 0x29, 0x55, 0x2A, 0x55, 0x2A, 0x05, 0x7E, 0x70, 0x47,
  0x0A, 0x60, 0x4F, 0x0A, 0x50, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x42, 0x0A, 0x28, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x2A, 0x05, 0x53, 0x2A, 0x15, 0x00, 0x50, 0x02, 0x14, 0x00, 0x40, 0x2A, 0x05,
  0x00, 0x50, 0x2A, 0x65, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x18, 0x7F, 0x47,
  0x2A, 0x65, 0x43, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x40, 0x20, 0x55, 0x3A,
  0x5D, 0x3A, 0x5F, 0x7A, 0x7D, 0x7A, 0x7D, 0x3A, 0x5D, 0x3A, 0x7D, 0x3A, 0x75, 0x2A, 0x57, 0x3A,
  0x55, 0x3A, 0x55, 0x3A, 0x5D, 0x2A, 0x20, 0x55, 0x0A, 0x50, 0x0A, 0x28, 0x55, 0x2A, 0x55, 0x2A,
  0x00, 0x24, 0x24, 0x24, 0x28, 0x24, 0x10, 0x04, 0x24, 0x08, 0x00, 0x08, 0x24, 0x24, 0x08, 0x00,
  0x24, 0x28, 0x00, 0x08, 0x10, 0x24, 0x10, 0x24, 0x08, 0x00, 0x08, 0x24, 0x04, 0x08, 0x54, 0x28,
  0x24, 0x08, 0x00, 0x10, 0x4C, 0x24, 0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x5D, 0x2E, 0x55, 0x2B, 0x5D, 0x2F, 0x55, 0x2B, 0x5D, 0x3E, 0x5D, 0x2E, 0x55, 0x2A,
  0x55, 0x2A, 0x5D, 0x3E, 0x5D, 0x2F, 0x5D, 0x2E, 0x5D, 0x2E, 0x5D, 0x2E, 0x7D, 0x3A, 0x75, 0x2A,
  0x5D, 0x2E, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x1C, 0x0A, 0x54,
  0x02, 0x00, 0x20, 0x45, 0x0A, 0x00, 0x20, 0x01, 0x00, 0x00, 0x0A, 0x40, 0x32, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x02, 0x43, 0x1F, 0x2A, 0x65, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x0A, 0x00, 0x11, 0x02, 0x00, 0xD2, 0x7A, 0x7F, 0x7A, 0x5F, 0x3A, 0x75, 0x3A, 0x75, 0x7A,
  0x7F, 0x3A, 0x7F, 0x2A, 0x57, 0x6A, 0x5F, 0x7A, 0x7F, 0x7A, 0x7F, 0x7A, 0x7F, 0x4A, 0x2A, 0x55,
  0x2A, 0x15, 0x52, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x7D, 0x3F, 0x75, 0x2F, 0x5D, 0x3F, 0x75, 0x2F, 0x7D, 0x3F, 0x7D, 0x3F, 0x55, 0x2A,
  0x55, 0x2A, 0x7D, 0x2A, 0x5D, 0x3F, 0x7D, 0x3F, 0x7D, 0x3F, 0x7D, 0x3F, 0x5D, 0x3F, 0x75, 0x2B,
  0x7D, 0x3F, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x1C, 0x2A, 0x54,
  0x0A, 0x00, 0x00, 0x55, 0x00, 0x00, 0x20, 0x05, 0x00, 0x00, 0x0A, 0x40, 0x42, 0x29, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x70, 0x67, 0x28, 0x15, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x10, 0x20, 0x04, 0x88, 0x91, 0xA2, 0x84, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x01, 0x00,
  0x04, 0x40, 0x2A, 0x25, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x05, 0x90, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x43, 0x2A, 0x55,
  0x2A, 0x00, 0x00, 0x55, 0x02, 0x00, 0x28, 0x15, 0x00, 0x40, 0x2A, 0x55, 0x4A, 0x29, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x66, 0x20, 0x55, 0x40, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x15, 0x00, 0x00, 0xC4, 0x88, 0x91, 0x82, 0x54, 0x50, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x02, 0x20, 0x55, 0x22, 0x15, 0x54, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x46, 0x2A, 0x15, 0x00, 0x00, 0x2A, 0x40, 0x02, 0x00, 0x28, 0x15, 0x00, 0x00,
  0x0A, 0x50, 0x00, 0x00, 0x0A, 0x54, 0x3C, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x70, 0x7F,
  0x28, 0x15, 0x28, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x62, 0x0C, 0x55, 0x42, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x15, 0x90, 0xA2, 0xC4, 0x28, 0x55, 0x0A, 0x00, 0x2A, 0x55, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x28, 0x55, 0x2A, 0x01, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x40, 0x2A, 0x55,
  0x2A, 0x55, 0x2A, 0x04, 0x2A, 0x55, 0x2A, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x06, 0x2A, 0x05, 0x00, 0x00, 0x0A, 0x05, 0x2A, 0x00, 0x28, 0x50, 0x0A, 0x00,
  0x2A, 0x01, 0x00, 0x00, 0x2A, 0x01, 0x30, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x18, 0x40, 0x07,
  0x2A, 0x15, 0x00, 0x20, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x3A,
  0x5F, 0x7A, 0x5D, 0x2A, 0x7D, 0x3E, 0x75, 0x3A, 0x75, 0x3A, 0x7D, 0x3E, 0x75, 0x3A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x31, 0x54, 0x0A, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x21, 0x55, 0x2A, 0x55, 0x0A, 0x00, 0xA2, 0x04, 0x22, 0x44, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55,
  0x2A, 0x55, 0x22, 0x44, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x22, 0x01,
  0x55, 0x2A, 0x55, 0x2A, 0x01, 0x44, 0x40, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x46, 0x02, 0x50, 0x0A, 0x00, 0x0A, 0x14, 0x2A, 0x00, 0x28, 0x40, 0x2A, 0x00,
  0x0A, 0x54, 0x0A, 0x00, 0x0A, 0x40, 0x4A, 0x29, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x79, 0x7F, 0x47,
  0x2A, 0x65, 0x3F, 0x20, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x3A,
  0x7D, 0x2A, 0x5F, 0x2A, 0x5D, 0x7A, 0x75, 0x7A, 0x75, 0x7A, 0x5D, 0x7A, 0x75, 0x7E, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x46, 0x2A, 0x28, 0x55, 0x2A, 0x55, 0x2A,
  0x21, 0x55, 0x00, 0xC0, 0x88, 0x91, 0x52, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x00, 0x2A, 0x00, 0x00,
  0x00, 0x28, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x00, 0x00, 0x00, 0x00, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x28, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x1E, 0x7C, 0x47,
  0x2A, 0x65, 0x4F, 0x0A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x42, 0x2A, 0x29, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x53, 0x2A, 0x15, 0x00, 0x50, 0x02, 0x14, 0x00, 0x40, 0x2A, 0x05,
  0x00, 0x50, 0x2A, 0x05, 0x43, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x18, 0x3F, 0x46,
  0x0A, 0x64, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x02, 0x08, 0x20, 0x55, 0x3A,
  0x5D, 0x3A, 0x5D, 0x7A, 0x7D, 0x7A, 0x7D, 0x3A, 0x5D, 0x3A, 0x5F, 0x7A, 0x75, 0x2A, 0x57, 0x3A,
  0x55, 0x3A, 0x55, 0x3A, 0x5D, 0x2A, 0x28, 0x55, 0x0A, 0x54, 0x2A, 0x28, 0x55, 0x2A, 0x55, 0x2A,
  0x00, 0x04, 0x24, 0x1C, 0x10, 0x1C, 0x10, 0x34, 0x3C, 0x08, 0x00, 0x08, 0x38, 0x18, 0x08, 0x00,
  0x1C, 0x10, 0x00, 0x10, 0x10, 0x1C, 0x10, 0x24, 0x10, 0x00, 0x10, 0x24, 0x3C, 0x08, 0x54, 0x38,
  0x1C, 0x38, 0x00, 0x10, 0x54, 0x04, 0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x5D, 0x2A, 0x55, 0x2B, 0x7D, 0x2B, 0x55, 0x2B, 0x5D, 0x3E, 0x5D, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x5D, 0x3E, 0x7D, 0x2B, 0x5D, 0x2A, 0x5D, 0x2A, 0x5D, 0x2A, 0x5D, 0x3B, 0x75, 0x2A,
  0x5D, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x1C, 0x0A, 0x50,
  0x02, 0x00, 0x00, 0x55, 0x02, 0x00, 0x20, 0x01, 0x00, 0x00, 0x0A, 0x00, 0x30, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x0A, 0x73, 0x1F, 0x2A, 0x61, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x02, 0x00, 0x11, 0x00, 0xC0, 0xA8, 0x6A, 0x5F, 0x6A, 0x5F, 0x3E, 0x75, 0x3E, 0x75, 0x7E,
  0x5F, 0x3E, 0x5F, 0x2A, 0x57, 0x6A, 0x5F, 0x7E, 0x5F, 0x7E, 0x5F, 0x7E, 0x5F, 0x4A, 0x2A, 0x55,
  0x2A, 0x11, 0x50, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x75, 0x2F, 0x75, 0x2F, 0x5F, 0x2F, 0x75, 0x2F, 0x7F, 0x2F, 0x75, 0x2F, 0x55, 0x2A,
  0x55, 0x2A, 0x7F, 0x2A, 0x5F, 0x2F, 0x7F, 0x2F, 0x75, 0x2F, 0x7F, 0x2F, 0x5F, 0x3F, 0x7D, 0x2B,
  0x75, 0x2F, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x1C, 0x0A, 0x50,
  0x0A, 0x00, 0x00, 0x55, 0x02, 0x00, 0x20, 0x15, 0x00, 0x00, 0x0A, 0x40, 0x4A, 0x29, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x71, 0x61, 0x28, 0x15, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x06, 0x00, 0x00, 0xA2, 0xC4, 0x88, 0x81, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x05, 0x10, 0x28, 0x25, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x05, 0x95, 0x50, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x53, 0x2A, 0x55,
  0x0A, 0x00, 0x00, 0x55, 0x00, 0x00, 0x2A, 0x05, 0x00, 0x50, 0x2A, 0x55, 0x42, 0x29, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x7E, 0x23, 0x55, 0x4C, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x15, 0x00, 0x00, 0x91, 0xA2, 0xC4, 0x00, 0x55, 0x50, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x05, 0x40, 0x20, 0x55, 0x02, 0x55, 0x50, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x46, 0x2A, 0x15, 0x00, 0x00, 0x2A, 0x40, 0x02, 0x00, 0x28, 0x15, 0x00, 0x00,
  0x0A, 0x50, 0x00, 0x00, 0x0A, 0x54, 0x30, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x0A, 0x7C, 0x1F,
  0x28, 0x15, 0x28, 0x28, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x02, 0x0F, 0x55, 0x42, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x15, 0xC4, 0x88, 0x91, 0x28, 0x55, 0x22, 0x04, 0x2A, 0x55, 0x40, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x28, 0x55, 0x2A, 0x55, 0x40, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x00, 0x54, 0x2A, 0x55,
  0x2A, 0x55, 0x22, 0x40, 0x2A, 0x55, 0x2A, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x06, 0x2A, 0x05, 0x00, 0x00, 0x0A, 0x05, 0x2A, 0x00, 0x28, 0x50, 0x0A, 0x00,
  0x2A, 0x01, 0x00, 0x00, 0x2A, 0x01, 0x30, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x05, 0x06, 0x7F, 0x47,
  0x2A, 0x15, 0x00, 0x28, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x3A,
  0x5F, 0x6A, 0x75, 0x2A, 0x7D, 0x3E, 0x75, 0x3A, 0x75, 0x3A, 0x7D, 0x3E, 0x75, 0x3A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x31, 0x50, 0x0A, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x29, 0x55, 0x2A, 0x55, 0x08, 0xC0, 0x88, 0x10, 0x22, 0x04, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55,
  0x2A, 0x55, 0x22, 0x04, 0x02, 0x51, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x02, 0x28,
  0x55, 0x2A, 0x55, 0x2A, 0x01, 0x44, 0x50, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x46, 0x02, 0x50, 0x0A, 0x00, 0x0A, 0x14, 0x2A, 0x00, 0x28, 0x40, 0x2A, 0x00,
  0x0A, 0x54, 0x0A, 0x00, 0x0A, 0x40, 0x4A, 0x29, 0x55, 0x2A, 0x55, 0x2A, 0x05, 0x78, 0x03, 0x40,
  0x0A, 0x00, 0x30, 0x22, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x3A,
  0x7D, 0x2A, 0x5F, 0x2A, 0x75, 0x7A, 0x75, 0x7A, 0x75, 0x7A, 0x75, 0x7A, 0x75, 0x7E, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x40, 0x2A, 0x29, 0x55, 0x2A, 0x55, 0x2A,
  0x05, 0x44, 0x08, 0x90, 0xA2, 0xC4, 0x50, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x00, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x62, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x3F, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x1E, 0x7F, 0x47,
  0x2A, 0x05, 0x40, 0x0A, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x42, 0x2A, 0x29, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x2A, 0x15, 0x47, 0x2A, 0x55, 0x00, 0x40, 0x0A, 0x50, 0x00, 0x50, 0x2A, 0x15,
  0x00, 0x40, 0x2A, 0x15, 0x4F, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x18, 0x03, 0x46,
  0x2A, 0x00, 0x50, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x40, 0x2A, 0x20, 0x55, 0x3A,
  0x55, 0x3A, 0x7D, 0x3A, 0x77, 0x3A, 0x77, 0x3A, 0x55, 0x7A, 0x57, 0x7A, 0x75, 0x2A, 0x57, 0x3A,
  0x55, 0x3A, 0x55, 0x3A, 0x55, 0x2A, 0x2A, 0x55, 0x0A, 0x54, 0x2A, 0x28, 0x55, 0x2A, 0x55, 0x2A,
  0x00, 0x24, 0x24, 0x04, 0x10, 0x14, 0x10, 0x24, 0x24, 0x08, 0x00, 0x08, 0x10, 0x24, 0x08, 0x00,
  0x24, 0x10, 0x00, 0x20, 0x10, 0x14, 0x10, 0x24, 0x20, 0x00, 0x20, 0x24, 0x04, 0x08, 0x6C, 0x44,
  0x14, 0x08, 0x00, 0x10, 0x64, 0x24, 0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x7D, 0x2B, 0x55, 0x2B, 0x7D, 0x2A, 0x55, 0x2B, 0x5D, 0x3E, 0x7D, 0x2B, 0x55, 0x2A,
  0x55, 0x2A, 0x7D, 0x2F, 0x7D, 0x2A, 0x7D, 0x2A, 0x7D, 0x2B, 0x7D, 0x2A, 0x5D, 0x3B, 0x75, 0x2A,
  0x7D, 0x2B, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x1C, 0x0A, 0x50,
  0x02, 0x00, 0x00, 0x55, 0x02, 0x00, 0x20, 0x01, 0x00, 0x00, 0x0A, 0x00, 0x30, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x0A, 0x73, 0x19, 0x2A, 0x00, 0x50, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x02, 0x82, 0x40, 0x00, 0x90, 0xE2, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x4A, 0x2A, 0x55,
  0x2A, 0x11, 0x40, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x1C, 0x0A, 0x50,
  0x0A, 0x00, 0x00, 0x55, 0x02, 0x00, 0x20, 0x15, 0x00, 0x00, 0x0A, 0x40, 0x4A, 0x29, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x71, 0x79, 0x28, 0x01, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x15, 0x90, 0x08, 0xC0, 0x88, 0x91, 0xA2, 0x84, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x00, 0x2A, 0x05, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x62, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x01, 0x94, 0x52, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x53, 0x2A, 0x55,
  0x0A, 0x00, 0x00, 0x55, 0x00, 0x00, 0x2A, 0x05, 0x00, 0x50, 0x2A, 0x55, 0x72, 0x29, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x1E, 0x20, 0x55, 0x4C, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x15, 0x00, 0xA0, 0xC4, 0x88, 0x91, 0x20, 0x55, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x00, 0x55, 0x02, 0x50, 0x0A, 0x55, 0x52, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x46, 0x02, 0x50, 0x00, 0x00, 0x2A, 0x41, 0x0A, 0x00, 0x28, 0x54, 0x00, 0x00,
  0x0A, 0x50, 0x02, 0x00, 0x0A, 0x50, 0x32, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x02, 0x7F, 0x1F,
  0x2A, 0x55, 0x2A, 0x29, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x0A, 0x0F, 0x14, 0x40, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x15, 0x90, 0xA2, 0x84, 0x2A, 0x55, 0x02, 0x40, 0x2A, 0x55, 0x0A, 0x28, 0x55, 0x28, 0x55, 0x2A,
  0x55, 0x00, 0x2A, 0x55, 0x2A, 0x55, 0x0A, 0x28, 0x55, 0x2A, 0x55, 0x00, 0x28, 0x55, 0x2A, 0x55,
  0x2A, 0x04, 0x22, 0x40, 0x2A, 0x55, 0x2A, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x06, 0x28, 0x15, 0x00, 0x00, 0x0A, 0x05, 0x2A, 0x00, 0x28, 0x55, 0x2A, 0x00,
  0x0A, 0x15, 0x00, 0x00, 0x2A, 0x01, 0x30, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x45, 0x61, 0x7F, 0x47,
  0x2A, 0x15, 0x03, 0x28, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x3A,
  0x5F, 0x6A, 0x7F, 0x2A, 0x5D, 0x3A, 0x75, 0x6A, 0x75, 0x7A, 0x5D, 0x3A, 0x75, 0x3E, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x71, 0x51, 0x00, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x29, 0x55, 0x2A, 0x15, 0x00, 0x90, 0x82, 0x10, 0x02, 0x00, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55,
  0x2A, 0x55, 0x02, 0x00, 0x08, 0x11, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x01, 0x02, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x15, 0x00, 0x54, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x15, 0x46, 0x0A, 0x50, 0x0A, 0x00, 0x0A, 0x54, 0x2A, 0x00, 0x28, 0x40, 0x2A, 0x00,
  0x0A, 0x54, 0x0A, 0x00, 0x0A, 0x40, 0x4A, 0x29, 0x55, 0x2A, 0x55, 0x2A, 0x05, 0x1E, 0x70, 0x47,
  0x2A, 0x65, 0x03, 0x02, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x7A,
  0x7F, 0x2A, 0x5F, 0x2A, 0x75, 0x7F, 0x7D, 0x3A, 0x75, 0x7F, 0x75, 0x7F, 0x75, 0x7E, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x40, 0x2A, 0x29, 0x55, 0x2A, 0x55, 0x2A,
  0x05, 0xC4, 0x00, 0x90, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x06, 0x3F, 0x40,
  0x2A, 0x65, 0x43, 0x0A, 0x54, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x42, 0x2A, 0x29, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
  0x55, 0x2A, 0x55, 0x2A, 0x15, 0x46, 0x2A, 0x55, 0x00, 0x40, 0x0A, 0x50, 0x00, 0x50, 0x2A, 0x15,
  0x00, 0x40, 0x2A, 0x15, 0x0C, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x18, 0x73, 0x47,
  0x2A, 0x65, 0x53, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x15, 0x50, 0x2A, 0x21, 0x55, 0x7A,
  0x57, 0x3A, 0x7D, 0x3A, 0x77, 0x3A, 0x77, 0x7A, 0x55, 0x7A, 0x55, 0x6A, 0x7D, 0x2A, 0x57, 0x3A,
  0x55, 0x3A, 0x55, 0x7A, 0x55, 0x0A, 0x2A, 0x55, 0x00, 0x55, 0x22, 0x2A, 0x55, 0x2A, 0x55, 0x2A,
  0x00, 0x18, 0x18, 0x04, 0x10, 0x24, 0x38, 0x18, 0x24, 0x08, 0x00, 0x08, 0x08, 0x18, 0x08, 0x00,
  0x1C, 0x10, 0x00, 0x18, 0x38, 0x24, 0x38, 0x18, 0x18, 0x00, 0x18, 0x18, 0x04, 0x08, 0x44, 0x44,
  0x24, 0x38, 0x00, 0x38, 0x44, 0x18, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80,
  0x28, 0x28, 0xA8, 0xA8, 0x28, 0x28, 0xA8, 0xA8, 0x28, 0x28, 0xA8, 0xA8, 0x28, 0x28, 0xA8, 0xA8,
  0x50, 0x50, 0xD0, 0xD0, 0x50, 0x50, 0xD0, 0xD0, 0x50, 0x50, 0xD0, 0xD0, 0x50, 0x50, 0xD0, 0xD0,
  0x20, 0x30, 0x20, 0x30, 0x21, 0x31, 0x21, 0x31, 0x22, 0x32, 0x22, 0x32, 0x23, 0x33, 0x23, 0x33,
  0x20, 0x30, 0x20, 0x30, 0x21, 0x31, 0x21, 0x31, 0x22, 0x32, 0x22, 0x32, 0x23, 0x33, 0x23, 0x33,
  0x20, 0x30, 0x20, 0x30, 0x21, 0x31, 0x21, 0x31, 0x22, 0x32, 0x22, 0x32, 0x23, 0x33, 0x23, 0x33,
  0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x91, 0xA2, 0xC4,
  0xA2, 0xC4, 0x88, 0x91, 0xC4, 0x88, 0x91, 0xA2, 0x91, 0xA2, 0xC4, 0x88, 0x55, 0x2A, 0x55, 0x2A,
  0x55, 0x2A, 0x55, 0x2A, 0x08, 0x11, 0x22, 0x44, 0x22, 0x44, 0x08, 0x11, 0x2A, 0x55, 0x2A, 0x55,
  0x55, 0x2A, 0x55, 0x2A, 0xD5, 0xAA, 0xD5, 0xAA, 0xD5, 0xAA, 0xD5, 0xAA, 0xF7, 0xEE, 0xDD, 0xBB,
  0xDD, 0xBB, 0xF7, 0xEE, 0x11, 0x22, 0x44, 0x08, 0x44, 0x08, 0x11, 0x22, 0xAA, 0xD5, 0xAA, 0xD5,
  0xAA, 0xD5, 0xAA, 0xD5, 0x33, 0x66, 0x4C, 0x19, 0x4C, 0x19, 0x33, 0x66, 0xAA, 0xD5, 0xAA, 0xD5,
  0x55, 0x2A, 0x55, 0x2A, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, 0xAA, 0xD5, 0xAA, 0xD5,
  0x2A, 0x55, 0x2A, 0x55, 0x3B, 0x77, 0x6E, 0x5D, 0x6E, 0x5D, 0x3B, 0x77, 0x7F, 0x7F, 0x7F, 0x7F,
  0x7F, 0x7F, 0x7F, 0x7F, 0x20, 0x27, 0x61, 0xA2, 0x00, 0xA4, 0x03, 0x86, 0x07, 0xB9, 0x00, 0x60,
  0x85, 0x04, 0xB9, 0x30, 0x60, 0x85, 0x05, 0xA5, 0x01, 0x85, 0x06, 0xA5, 0x07, 0x6A, 0x26, 0x06,
  0x06, 0x06, 0x06, 0x06, 0xA5, 0x02, 0x29, 0x03, 0x05, 0x06, 0x85, 0x06, 0xA4, 0x06, 0xB9, 0x64,
  0x60, 0xA4, 0x02, 0x3D, 0x60, 0x60, 0x91, 0x04, 0xE8, 0xE6, 0x07, 0xA5, 0x05, 0x18, 0x69, 0x04,
  0x85, 0x05, 0xE0, 0x04, 0xD0, 0xD1, 0x60, 0xA5, 0x00, 0x0A, 0x0A, 0xAA, 0xBD, 0x74, 0x61, 0x8D,
  0x60, 0x60, 0xE8, 0xBD, 0x74, 0x61, 0x8D, 0x61, 0x60, 0xE8, 0xBD, 0x74, 0x61, 0x8D, 0x62, 0x60,
  0xE8, 0xBD, 0x74, 0x61, 0x8D, 0x63, 0x60, 0x60, 0x20, 0x27, 0x61, 0x20, 0xE7, 0x60, 0xA5, 0x02,
  0xC5, 0x08, 0xF0, 0x05, 0xE6, 0x02, 0x4C, 0x4B, 0x61, 0x60, 0x20, 0x27, 0x61, 0x20, 0xE7, 0x60,
  0xA5, 0x03, 0xC5, 0x08, 0xF0, 0xF3, 0xE6, 0x03, 0x4C, 0x5D, 0x61, 0xAD, 0x00, 0xC0, 0x8D, 0x10,
  0xC0, 0x85, 0x09, 0x60, 0x00, 0x00, 0x00, 0x00, 0x8C, 0xBF, 0x8C, 0x00, 0x00, 0x8C, 0xBF, 0x8C,
  0x98, 0xFE, 0x98, 0x00, 0x8C, 0xBF, 0x8C, 0x00, 0xB3, 0xB0, 0x8F, 0x00, 0x8F, 0xB0, 0xB3, 0xB3,
  0xFC, 0x83, 0xF3, 0xB3, 0xF3, 0x83, 0xFC, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xB3, 0xB3, 0xB3, 0xB3,
  0xFF, 0x00, 0xFF, 0x00, 0xB3, 0xB3, 0xB3, 0xB3, 0xE0, 0xFF, 0xE0, 0x00, 0xBF, 0x0C, 0x0C, 0x0C,
  0x83, 0xFF, 0x83, 0x00, 0x0C, 0x0C, 0x0C, 0xBF, 0x8F, 0xB0, 0xB3, 0xB3, 0xFC, 0x83, 0xF3, 0xB3,
  0xF3, 0x83, 0xFC, 0x00, 0xB3, 0xB0, 0x8F, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xF8, 0xFE, 0xFF,
  0xFF, 0xBF, 0x0F, 0x03, 0x03, 0x0F, 0xBF, 0xFF, 0xFF, 0xFE, 0xF8, 0xE0, 0x9C, 0xE3, 0xE3, 0x9C,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xAE, 0x4C, 0x62, 0xBD, 0x3C, 0x62, 0x10, 0x0E, 0x48, 0xA9, 0x00, 0x9D, 0x3C, 0x62, 0xE8, 0x8A,
  0x29, 0x0F, 0x8D, 0x4C, 0x62, 0x68, 0x60, 0xAE, 0x4D, 0x62, 0xAD, 0x00, 0xC0, 0x10, 0xF7, 0x8D,
  0x10, 0xC0, 0x9D, 0x3C, 0x62, 0xE8, 0x8A, 0x29, 0x0F, 0xCD, 0x4C, 0x62, 0xF0, 0xE8, 0x8D, 0x4D,
  0x62, 0x60, 0x00, 0x01, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x14,
  0x24, 0x14, 0x27, 0x00, 0x07, 0x61, 0xA9, 0x14, 0x20, 0x0F, 0x66, 0x8D, 0x51, 0x62, 0xA9, 0x27,
  0x8D, 0x50, 0x62, 0x8D, 0x52, 0x62, 0xA9, 0x02, 0x8D, 0x4E, 0x62, 0xA9, 0x0A, 0x8D, 0x54, 0x62,
  0xA9, 0x64, 0x8D, 0x55, 0x62, 0xA2, 0x0F, 0xA9, 0x00, 0x9D, 0x3C, 0x62, 0xCA, 0x10, 0xFA, 0x8D,
  0x4C, 0x62, 0x8D, 0x4D, 0x62, 0x4C, 0x88, 0x62, 0x20, 0xC3, 0x69, 0x20, 0x17, 0x62, 0x20, 0x72,
  0x6C, 0x10, 0x75, 0xA2, 0x10, 0x8E, 0x73, 0x64, 0xC9, 0x95, 0xD0, 0x6A, 0xAD, 0x4E, 0x62, 0xCE,
  0x4E, 0x62, 0x18, 0x69, 0x10, 0x85, 0x00, 0xA9, 0x0C, 0x85, 0x01, 0xAD, 0x4F, 0x62, 0x85, 0x02,
  0xAD, 0x50, 0x62, 0x85, 0x03, 0x20, 0xDA, 0x6B, 0xAE, 0x4E, 0x62, 0xCA, 0x8A, 0x29, 0x03, 0xAA,
  0xE8, 0x8E, 0x4E, 0x62, 0x8A, 0x20, 0x64, 0xF8, 0xAC, 0x4F, 0x62, 0xAD, 0x50, 0x62, 0x20, 0x00,
  0xF8, 0xAE, 0x4E, 0x62, 0xAD, 0x4F, 0x62, 0x18, 0x7D, 0x32, 0x62, 0x8D, 0x4F, 0x62, 0xAD, 0x50,
  0x62, 0x18, 0x7D, 0x37, 0x62, 0x8D, 0x50, 0x62, 0xAC, 0x4F, 0x62, 0x20, 0x71, 0xF8, 0x8D, 0x53,
  0x62, 0xAD, 0x4F, 0x62, 0x85, 0x02, 0xAD, 0x50, 0x62, 0x85, 0x03, 0xAD, 0x4E, 0x62, 0x85, 0x00,
  0x20, 0xE4, 0x60, 0x4C, 0x74, 0x64, 0xD0, 0x16, 0xAD, 0x02, 0x03, 0xF0, 0x08, 0x20, 0x32, 0x6A,
  0x10, 0x03, 0x4C, 0x93, 0x62, 0xAD, 0x4E, 0x62, 0x18, 0x69, 0x08, 0x4C, 0xA5, 0x62, 0xC9, 0x88,
  0xD0, 0x27, 0xAD, 0x4E, 0x62, 0xEE, 0x4E, 0x62, 0x18, 0x69, 0x04, 0x4C, 0xA5, 0x62, 0xA9, 0x07,
  0x20, 0x64, 0xF8, 0xAD, 0x50, 0x62, 0xAC, 0x4F, 0x62, 0x20, 0x00, 0xF8, 0xAD, 0x4F, 0x62, 0xC9,
  0x14, 0xD0, 0x5E, 0xAD, 0x50, 0x62, 0xD0, 0x59, 0x60, 0xC9, 0x92, 0xD0, 0x06, 0xA9, 0xFF, 0x8D,
  0x53, 0x62, 0x60, 0xC9, 0xC9, 0xD0, 0x09, 0xAE, 0x4E, 0x62, 0xBD, 0x87, 0x63, 0x4C, 0x91, 0x62,
  0xC9, 0xCA, 0xD0, 0x09, 0xAE, 0x4E, 0x62, 0xBD, 0x8C, 0x63, 0x4C, 0x91, 0x62, 0xC9, 0xCB, 0xD0,
  0x09, 0xAE, 0x4E, 0x62, 0xBD, 0x91, 0x63, 0x4C, 0x91, 0x62, 0xC9, 0xCD, 0xD0, 0x1D, 0xAE, 0x4E,
  0x62, 0xBD, 0x96, 0x63, 0x4C, 0x91, 0x62, 0x00, 0x88, 0x00, 0x95, 0x00, 0x00, 0x00, 0x88, 0x00,
  0x95, 0x00, 0x00, 0x95, 0x00, 0x88, 0x00, 0x95, 0x00, 0x88, 0x00, 0x20, 0xA9, 0x69, 0x4C, 0x0F,
  0x64, 0xAD, 0x54, 0x62, 0xF0, 0x0B, 0xCE, 0x54, 0x62, 0xA2, 0x07, 0x8E, 0x73, 0x64, 0x4C, 0x0F,
  0x64, 0xAD, 0x52, 0x62, 0xAC, 0x51, 0x62, 0x20, 0x71, 0xF8, 0x48, 0xA9, 0x00, 0x20, 0x64, 0xF8,
  0xAD, 0x52, 0x62, 0xAC, 0x51, 0x62, 0x20, 0x00, 0xF8, 0xA9, 0x00, 0x85, 0x01, 0xAD, 0x52, 0x62,
  0x85, 0x03, 0xAD, 0x51, 0x62, 0x85, 0x02, 0x20, 0xE4, 0x60, 0x68, 0xAA, 0xAD, 0x51, 0x62, 0x18,
  0x7D, 0x32, 0x62, 0x8D, 0x51, 0x62, 0xAD, 0x52, 0x62, 0x18, 0x7D, 0x37, 0x62, 0x8D, 0x52, 0x62,
  0xAC, 0x51, 0x62, 0x20, 0x71, 0xF8, 0x18, 0x69, 0x0C, 0x85, 0x00, 0xA9, 0x0C, 0x85, 0x01, 0xAD,
  0x51, 0x62, 0x85, 0x02, 0xAD, 0x52, 0x62, 0x85, 0x03, 0x20, 0xE4, 0x60, 0x4C, 0x0F, 0x64, 0xAC,
  0x49, 0x6C, 0xB9, 0x00, 0xC0, 0xEA, 0xEA, 0xCE, 0x55, 0x62, 0xD0, 0x06, 0xA9, 0xFE, 0x8D, 0x53,
  0x62, 0x60, 0xA9, 0x27, 0xA0, 0x14, 0x20, 0x3D, 0x6B, 0xC9, 0x00, 0xD0, 0x23, 0xA9, 0x15, 0x85,
  0x00, 0xA9, 0x0D, 0x85, 0x01, 0xA9, 0x27, 0x85, 0x03, 0xA9, 0x12, 0x85, 0x02, 0xA9, 0x16, 0x85,
  0x08, 0x20, 0x48, 0x61, 0xA9, 0x0D, 0x20, 0x64, 0xF8, 0xA9, 0x27, 0xA0, 0x14, 0x20, 0x00, 0xF8,
  0xAE, 0x00, 0x03, 0x20, 0xFB, 0x6B, 0x8A, 0x48, 0x20, 0x17, 0x62, 0xAD, 0x73, 0x64, 0xF0, 0x0B,
  0xAC, 0x49, 0x6C, 0xB9, 0x00, 0xC0, 0xEA, 0xEA, 0xCE, 0x73, 0x64, 0x68, 0xAA, 0xCA, 0xD0, 0xE3,
  0x4C, 0x8B, 0x62, 0x00, 0xAD, 0x53, 0x62, 0xD0, 0x03, 0x4C, 0x2E, 0x63, 0xC9, 0x0F, 0xD0, 0x14,
  0xA2, 0x20, 0x8E, 0x73, 0x64, 0xA9, 0x07, 0x20, 0x64, 0xF8, 0xAD, 0x50, 0x62, 0xAC, 0x4F, 0x62,
  0x20, 0xEF, 0x6B, 0x60, 0xEA, 0xEA, 0xA2, 0xFF, 0xBD, 0x00, 0xE0, 0xA8, 0x88, 0xD0, 0xFD, 0xAC,
  0x49, 0x6C, 0xB9, 0x00, 0xC0, 0xCA, 0xD0, 0xF0, 0x60, 0xA2, 0x00, 0x8A, 0xA8, 0x88, 0xD0, 0xFD,
  0xAC, 0x49, 0x6C, 0xB9, 0x00, 0xC0, 0xCA, 0xD0, 0xF2, 0x8A, 0xA8, 0x88, 0xD0, 0xFD, 0xAC, 0x49,
  0x6C, 0xB9, 0x00, 0xC0, 0xE8, 0xD0, 0xF2, 0x60, 0xA9, 0x00, 0x8D, 0x4A, 0x6C, 0xAD, 0x34, 0x66,
  0xD0, 0x01, 0x60, 0x18, 0x6D, 0x36, 0x66, 0x8D, 0x38, 0x66, 0xAD, 0x33, 0x66, 0x18, 0x6D, 0x35,
  0x66, 0x8D, 0x37, 0x66, 0xA8, 0xAD, 0x38, 0x66, 0x20, 0x71, 0xF8, 0xC9, 0x00, 0xF0, 0x5D, 0xAC,
  0x37, 0x66, 0xAD, 0x34, 0x66, 0x20, 0x71, 0xF8, 0xC9, 0x00, 0xF0, 0x11, 0xAD, 0x33, 0x66, 0x8D,
  0x37, 0x66, 0xAD, 0x35, 0x66, 0x49, 0xFE, 0x8D, 0x35, 0x66, 0xEE, 0x4A, 0x6C, 0xAD, 0x38, 0x66,
  0xAC, 0x33, 0x66, 0x20, 0x71, 0xF8, 0xC9, 0x00, 0xF0, 0x11, 0xAD, 0x34, 0x66, 0x8D, 0x38, 0x66,
  0xAD, 0x36, 0x66, 0x49, 0xFE, 0x8D, 0x36, 0x66, 0xEE, 0x4A, 0x6C, 0xAD, 0x4A, 0x6C, 0xD0, 0x1C,
  0xAD, 0x33, 0x66, 0x8D, 0x37, 0x66, 0xAD, 0x34, 0x66, 0x8D, 0x38, 0x66, 0xAD, 0x35, 0x66, 0x49,
  0xFE, 0x8D, 0x35, 0x66, 0xAD, 0x36, 0x66, 0x49, 0xFE, 0x8D, 0x36, 0x66, 0xA9, 0x00, 0x85, 0x01,
  0x20, 0x64, 0xF8, 0xAD, 0x33, 0x66, 0x85, 0x02, 0xAD, 0x34, 0x66, 0x85, 0x03, 0x20, 0xE4, 0x60,
  0xAD, 0x34, 0x66, 0xAC, 0x33, 0x66, 0x20, 0x00, 0xF8, 0xA9, 0x1A, 0x85, 0x00, 0xAD, 0x38, 0x66,
  0xD0, 0x01, 0x60, 0x85, 0x03, 0x8D, 0x34, 0x66, 0xAD, 0x37, 0x66, 0x85, 0x02, 0x8D, 0x33, 0x66,
  0xA9, 0x03, 0x85, 0x01, 0x20, 0x64, 0xF8, 0x20, 0xE4, 0x60, 0xAD, 0x34, 0x66, 0xAC, 0x33, 0x66,
  0x20, 0x00, 0xF8, 0x60, 0xAD, 0x01, 0x03, 0xD0, 0x03, 0x4C, 0x00, 0x62, 0xAD, 0x39, 0x66, 0x8D,
  0x33, 0x66, 0xAD, 0x3A, 0x66, 0x8D, 0x34, 0x66, 0xAD, 0x3D, 0x66, 0x8D, 0x35, 0x66, 0xAD, 0x3E,
  0x66, 0x8D, 0x36, 0x66, 0x20, 0xC8, 0x64, 0xAD, 0x33, 0x66, 0x8D, 0x39, 0x66, 0xAD, 0x34, 0x66,
  0x8D, 0x3A, 0x66, 0xAD, 0x35, 0x66, 0x8D, 0x3D, 0x66, 0xAD, 0x36, 0x66, 0x8D, 0x3E, 0x66, 0xAD,
  0x01, 0x03, 0xC9, 0x01, 0xD0, 0x03, 0x4C, 0x00, 0x62, 0xAD, 0x3B, 0x66, 0x8D, 0x33, 0x66, 0xAD,
  0x3C, 0x66, 0x8D, 0x34, 0x66, 0xAD, 0x3F, 0x66, 0x8D, 0x35, 0x66, 0xAD, 0x40, 0x66, 0x8D, 0x36,
  0x66, 0x20, 0xC8, 0x64, 0xAD, 0x33, 0x66, 0x8D, 0x3B, 0x66, 0xAD, 0x34, 0x66, 0x8D, 0x3C, 0x66,
  0xAD, 0x35, 0x66, 0x8D, 0x3F, 0x66, 0xAD, 0x36, 0x66, 0x8D, 0x40, 0x66, 0x4C, 0x00, 0x62, 0x8D,
  0x4F, 0x62, 0xA9, 0x01, 0x8D, 0x39, 0x66, 0x8D, 0x3A, 0x66, 0x8D, 0x3D, 0x66, 0x8D, 0x3E, 0x66,
  0x8D, 0x40, 0x66, 0x8D, 0x3C, 0x66, 0xA9, 0x26, 0x8D, 0x3B, 0x66, 0xA9, 0xFF, 0x8D, 0x3F, 0x66,
  0xA9, 0x14, 0x60, 0x04, 0x04, 0x01, 0x01, 0x14, 0x24, 0x04, 0x04, 0x26, 0x01, 0x01, 0x01, 0xFF,
  0x01, 0xA9, 0x4A, 0x85, 0x36, 0xA9, 0x66, 0x85, 0x37, 0x60, 0x48, 0x29, 0x7F, 0xC9, 0x20, 0xB0,
  0x04, 0x68, 0x4C, 0xF0, 0xFD, 0x85, 0x08, 0x86, 0x02, 0x84, 0x03, 0xA9, 0x00, 0x85, 0x00, 0x85,
  0x01, 0xA5, 0x29, 0x38, 0xE9, 0x04, 0x18, 0x69, 0x20, 0x85, 0x05, 0xA5, 0x28, 0x18, 0x65, 0x24,
  0x85, 0x04, 0xA5, 0x08, 0x38, 0xE9, 0x20, 0x0A, 0x0A, 0x26, 0x01, 0x0A, 0x26, 0x01, 0x18, 0x69,
  0xA9, 0x85, 0x00, 0xA5, 0x01, 0x69, 0x66, 0x85, 0x01, 0xA2, 0x00, 0x8A, 0xA8, 0xB1, 0x00, 0xA0,
  0x00, 0x91, 0x04, 0xA5, 0x05, 0x18, 0x69, 0x04, 0x85, 0x05, 0xE8, 0xE0, 0x08, 0xD0, 0xEC, 0xA6,
  0x02, 0xA4, 0x03, 0x4C, 0x51, 0x66, 0x4C, 0x55, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x08, 0x00, 0x14, 0x14, 0x14, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x28, 0x94, 0x7E, 0x14, 0xBF, 0x8A, 0x0A, 0x00, 0x08, 0x3C, 0x0A, 0x1C, 0x28, 0x1E, 0x08,
  0x00, 0x06, 0x26, 0x10, 0x08, 0x04, 0x32, 0x30, 0x00, 0x04, 0x0A, 0x0A, 0x04, 0x2A, 0x12, 0x2C,
  0x00, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x04, 0x02, 0x02, 0x02, 0x04, 0x08,
  0x00, 0x08, 0x10, 0x20, 0x20, 0x20, 0x10, 0x08, 0x00, 0x08, 0x2A, 0x1C, 0x08, 0x1C, 0x2A, 0x08,
  0x00, 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8C, 0x88,
  0x84, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8C,
  0x00, 0x00, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00, 0x8C, 0x92, 0x22, 0x22, 0x22, 0x92, 0x8C,
  0x00, 0x08, 0x0C, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x1C, 0x22, 0x20, 0x10, 0x84, 0x82, 0x3E,
  0x00, 0x3E, 0x90, 0x10, 0x18, 0x20, 0x22, 0x1C, 0x00, 0x10, 0x18, 0x14, 0x12, 0x3E, 0x10, 0x10,
  0x00, 0x3E, 0x02, 0x1E, 0x20, 0x20, 0x22, 0x1C, 0x00, 0x18, 0x04, 0x82, 0x1E, 0x22, 0x22, 0x1C,
  0x00, 0x3E, 0x90, 0x10, 0x88, 0x08, 0x84, 0x04, 0x00, 0x1C, 0x22, 0x22, 0x1C, 0x22, 0x22, 0x1C,
  0x00, 0x1C, 0x22, 0x22, 0x3C, 0x90, 0x10, 0x0C, 0x00, 0x00, 0x00, 0x8C, 0x00, 0x00, 0x8C, 0x00,
  0x00, 0x00, 0x00, 0x8C, 0x00, 0x00, 0x8C, 0x88, 0x84, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10,
  0x00, 0x00, 0x00, 0x3E, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x04, 0x08, 0x10, 0x20, 0x10, 0x08, 0x04,
  0x00, 0x1C, 0x22, 0x90, 0x88, 0x08, 0x00, 0x08, 0x00, 0x1C, 0x22, 0x2A, 0x3A, 0x1A, 0x02, 0x3C,
  0x00, 0x08, 0x8C, 0x14, 0x92, 0x3E, 0x22, 0x22, 0x00, 0x1E, 0x22, 0x22, 0x1E, 0x22, 0x22, 0x1E,
  0x00, 0x1C, 0x22, 0x02, 0x02, 0x02, 0x22, 0x1C, 0x00, 0x1E, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1E,
  0x00, 0x3E, 0x02, 0x02, 0x1E, 0x02, 0x02, 0x3E, 0x00, 0x3E, 0x02, 0x02, 0x1E, 0x02, 0x02, 0x02,
  0x00, 0x3C, 0x02, 0x02, 0x02, 0x32, 0x22, 0x3C, 0x00, 0x22, 0x22, 0x22, 0x3E, 0x22, 0x22, 0x22,
  0x00, 0x1C, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x1C,
  0x00, 0x22, 0x12, 0x0A, 0x06, 0x0A, 0x12, 0x22, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x3E,
  0x00, 0x22, 0x36, 0x2A, 0x2A, 0x22, 0x22, 0x22, 0x00, 0x22, 0x22, 0x26, 0x2A, 0x32, 0x22, 0x22,
  0x00, 0x1C, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1C, 0x00, 0x1E, 0x22, 0x22, 0x1E, 0x02, 0x02, 0x02,
  0x00, 0x1C, 0x22, 0x22, 0x22, 0x2A, 0x12, 0x2C, 0x00, 0x1E, 0x22, 0x22, 0x1E, 0x0A, 0x12, 0x22,
  0x00, 0x1C, 0x22, 0x02, 0x1C, 0x20, 0x22, 0x1C, 0x00, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
  0x00, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x1C, 0x00, 0x22, 0x22, 0x22, 0x92, 0x14, 0x8C, 0x08,
  0x00, 0x22, 0x22, 0x22, 0x2A, 0x2A, 0x36, 0x22, 0x00, 0x22, 0x92, 0x14, 0x08, 0x14, 0x92, 0x22,
  0x00, 0x22, 0x92, 0x14, 0x08, 0x08, 0x08, 0x08, 0x00, 0x3E, 0x90, 0x10, 0x08, 0x04, 0x82, 0x3E,
  0x00, 0x3E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x3E, 0x00, 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00,
  0x00, 0x3E, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3E, 0x00, 0x00, 0x00, 0x08, 0x14, 0x22, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x04, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x1C, 0x20, 0x3C, 0x22, 0x3C, 0x00, 0x00, 0x08, 0x1C, 0x3E, 0x08, 0x08, 0x08,
  0x08, 0x00, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x1C, 0x08, 0x00, 0x00, 0x04, 0x06, 0x7F, 0x06, 0x04,
  0x00, 0x00, 0x00, 0x10, 0x30, 0x7F, 0x30, 0x10, 0x00, 0x00, 0x00, 0x04, 0x7E, 0x01, 0x7E, 0x04,
  0x00, 0x00, 0x00, 0x10, 0x3F, 0x40, 0x3F, 0x10, 0x00, 0x02, 0x02, 0x1E, 0x22, 0x22, 0x22, 0x22,
  0x00, 0x08, 0x00, 0x0C, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x10, 0x00, 0x18, 0x10, 0x10, 0x10, 0x12,
  0x0C, 0x02, 0x02, 0x22, 0x12, 0x0E, 0x12, 0x22, 0x00, 0x0C, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C,
  0x00, 0x00, 0x00, 0x36, 0x2A, 0x2A, 0x2A, 0x22, 0x00, 0x00, 0x00, 0x1E, 0x22, 0x22, 0x22, 0x22,
  0x00, 0x00, 0x00, 0x1C, 0x22, 0x22, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x1E, 0x22, 0x22, 0x1E, 0x02,
  0x02, 0x00, 0x00, 0x3C, 0x22, 0x22, 0x3C, 0x20, 0x20, 0x00, 0x00, 0x3A, 0x06, 0x02, 0x02, 0x02,
  0x00, 0x00, 0x00, 0x3C, 0x02, 0x1C, 0x20, 0x1E, 0x00, 0x04, 0x04, 0x1E, 0x04, 0x04, 0x24, 0x18,
  0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x32, 0x2C, 0x00, 0x00, 0x00, 0x22, 0x92, 0x14, 0x8C, 0x08,
  0x00, 0x00, 0x00, 0x22, 0x22, 0x2A, 0x2A, 0x36, 0x00, 0x00, 0x00, 0x22, 0x14, 0x08, 0x14, 0x22,
  0x00, 0x00, 0x00, 0x22, 0x92, 0x14, 0x8C, 0x08, 0x86, 0x00, 0x00, 0x3E, 0x10, 0x08, 0x04, 0x3E,
  0x00, 0x38, 0x0C, 0x0C, 0x06, 0x0C, 0x0C, 0x38, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
  0x08, 0x0E, 0x18, 0x18, 0x30, 0x18, 0x18, 0x0E, 0x00, 0x2C, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0xC9, 0x9B, 0xD0, 0x08, 0xAD, 0x00, 0xC0,
  0x10, 0xFB, 0x8D, 0x10, 0xC0, 0xC9, 0x93, 0xD0, 0x08, 0xAD, 0xC2, 0x69, 0x49, 0x01, 0x8D, 0xC2,
  0x69, 0x60, 0x00, 0xAD, 0x4F, 0x62, 0x8D, 0x39, 0x6B, 0xA9, 0x01, 0x8D, 0x3A, 0x6B, 0xAD, 0x3A,
  0x6B, 0xAC, 0x39, 0x6B, 0x20, 0x71, 0xF8, 0xC9, 0x0F, 0xF0, 0x4A, 0xEE, 0x3A, 0x6B, 0xAD, 0x3A,
  0x6B, 0xC9, 0x27, 0xD0, 0xE9, 0xA9, 0x01, 0x8D, 0x3A, 0x6B, 0xCE, 0x39, 0x6B, 0xD0, 0xDF, 0xAD,
  0x4F, 0x62, 0x8D, 0x39, 0x6B, 0xAD, 0x3A, 0x6B, 0xAC, 0x39, 0x6B, 0x20, 0x71, 0xF8, 0xC9, 0x0F,
  0xF0, 0x23, 0xEE, 0x3A, 0x6B, 0xAD, 0x3A, 0x6B, 0xC9, 0x27, 0xD0, 0xE9, 0xA9, 0x01, 0x8D, 0x3A,
  0x6B, 0xEE, 0x39, 0x6B, 0xAD, 0x39, 0x6B, 0xC9, 0x27, 0xD0, 0xDA, 0xA9, 0x00, 0x8D, 0x3A, 0x6B,
  0xA9, 0x14, 0x8D, 0x39, 0x6B, 0xAD, 0x39, 0x6B, 0x8D, 0x3B, 0x6B, 0xAD, 0x3A, 0x6B, 0x8D, 0x3C,
  0x6B, 0x60, 0xAD, 0x3C, 0x6B, 0xCD, 0x50, 0x62, 0xF0, 0x20, 0xA9, 0x04, 0xB0, 0x02, 0xA9, 0x02,
  0x8D, 0x38, 0x6B, 0x20, 0xB8, 0x6A, 0xD0, 0x12, 0xAD, 0x38, 0x6B, 0xCD, 0x4E, 0x62, 0xF0, 0x04,
  0xAA, 0xBD, 0x55, 0x6A, 0x60, 0x00, 0xCB, 0xC9, 0xCA, 0xCD, 0xAD, 0x3B, 0x6B, 0xCD, 0x4F, 0x62,
  0x90, 0x17, 0xA9, 0x01, 0x8D, 0x38, 0x6B, 0x20, 0xB8, 0x6A, 0xF0, 0xDC, 0xA9, 0x03, 0x8D, 0x38,
  0x6B, 0x20, 0xB8, 0x6A, 0xF0, 0xD2, 0x4C, 0x8D, 0x6A, 0xA9, 0x03, 0x8D, 0x38, 0x6B, 0x20, 0xB8,
  0x6A, 0xF0, 0xC5, 0xA9, 0x01, 0x8D, 0x38, 0x6B, 0x20, 0xB8, 0x6A, 0xF0, 0xBB, 0xAD, 0x3C, 0x6B,
  0xCD, 0x50, 0x62, 0x90, 0x0A, 0xA9, 0x04, 0x8D, 0x38, 0x6B, 0x20, 0xB8, 0x6A, 0xF0, 0xA9, 0xA9,
  0x02, 0x8D, 0x38, 0x6B, 0x20, 0xB8, 0x6A, 0xF0, 0x9F, 0xA9, 0x04, 0x8D, 0x38, 0x6B, 0x20, 0xB8,
  0x6A, 0xF0, 0x95, 0x68, 0x68, 0x4C, 0x15, 0x63, 0xAE, 0x38, 0x6B, 0xBD, 0x32, 0x62, 0x18, 0x6D,
  0x4F, 0x62, 0x8D, 0x37, 0x66, 0xBD, 0x37, 0x62, 0x18, 0x6D, 0x50, 0x62, 0x8D, 0x38, 0x66, 0xAC,
  0x37, 0x66, 0x20, 0x71, 0xF8, 0xC9, 0x00, 0xF0, 0x02, 0xC9, 0x0F, 0xF0, 0x01, 0x60, 0xA9, 0x00,
  0x8D, 0x4A, 0x6C, 0xAD, 0x38, 0x66, 0xF0, 0xF5, 0xAC, 0x37, 0x66, 0xC8, 0x20, 0x71, 0xF8, 0xC9,
  0x00, 0xD0, 0x03, 0xEE, 0x4A, 0x6C, 0xAD, 0x38, 0x66, 0xAC, 0x37, 0x66, 0x88, 0x20, 0x71, 0xF8,
  0xC9, 0x00, 0xD0, 0x03, 0xEE, 0x4A, 0x6C, 0xAD, 0x38, 0x66, 0x18, 0x69, 0x01, 0xAC, 0x37, 0x66,
  0x20, 0x71, 0xF8, 0xC9, 0x00, 0xD0, 0x03, 0xEE, 0x4A, 0x6C, 0xAD, 0x38, 0x66, 0x38, 0xE9, 0x01,
  0xAC, 0x37, 0x66, 0x20, 0x71, 0xF8, 0xC9, 0x00, 0xD0, 0x03, 0xEE, 0x4A, 0x6C, 0xAD, 0x4A, 0x6C,
  0xF0, 0x03, 0xA9, 0x00, 0x60, 0xA9, 0x01, 0x60, 0x02, 0x13, 0x1D, 0x13, 0x1D, 0x20, 0x4B, 0x6C,
  0xA9, 0x15, 0x85, 0x00, 0xA9, 0x02, 0x85, 0x01, 0xA9, 0x00, 0x85, 0x02, 0xA9, 0x01, 0x85, 0x03,
  0xAD, 0x55, 0x62, 0x10, 0x07, 0xA9, 0xFF, 0x8D, 0x55, 0x62, 0xA9, 0x70, 0x4A, 0x4A, 0x85, 0x08,
  0xE6, 0x08, 0x20, 0x5A, 0x61, 0xA9, 0x27, 0x85, 0x02, 0xA9, 0x01, 0x85, 0x03, 0x20, 0x5A, 0x61,
  0xE6, 0x08, 0xA5, 0x08, 0x48, 0x85, 0x03, 0xA9, 0x0D, 0x85, 0x01, 0xA9, 0x27, 0x85, 0x08, 0x20,
  0x5A, 0x61, 0x68, 0x85, 0x03, 0xA9, 0x00, 0x85, 0x02, 0x20, 0x5A, 0x61, 0xA9, 0x27, 0xA0, 0x14,
  0x4C, 0x71, 0xF8, 0x20, 0x27, 0x61, 0xA2, 0x00, 0xA4, 0x03, 0x86, 0x07, 0xB9, 0x00, 0x60, 0x85,
  0x04, 0xB9, 0x30, 0x60, 0x85, 0x05, 0xA5, 0x01, 0x85, 0x06, 0xA5, 0x07, 0x6A, 0x66, 0x06, 0x06,
  0x06, 0x06, 0x06, 0xA5, 0x02, 0x29, 0x03, 0x05, 0x06, 0x85, 0x06, 0xA4, 0x06, 0xB9, 0x64, 0x60,
  0x49, 0x7F, 0xA4, 0x02, 0x3D, 0x60, 0x60, 0x11, 0x04, 0x91, 0x04, 0xE8, 0xE6, 0x07, 0xA5, 0x05,
  0x18, 0x69, 0x04, 0x85, 0x05, 0xE0, 0x04, 0xD0, 0xCD, 0x60, 0x20, 0xE4, 0x60, 0xAD, 0x05, 0x03,
  0xF0, 0x07, 0xA9, 0x01, 0x85, 0x00, 0x20, 0x93, 0x6B, 0xA9, 0x00, 0x8D, 0x05, 0x03, 0x60, 0x20,
  0x00, 0xF8, 0xA9, 0x01, 0x8D, 0x05, 0x03, 0x8D, 0x46, 0x6C, 0x60, 0xA9, 0x14, 0x8D, 0x48, 0x6C,
  0xAD, 0x46, 0x6C, 0xF0, 0x1B, 0xC9, 0x80, 0xB0, 0x17, 0xCE, 0x47, 0x6C, 0xD0, 0x12, 0xAC, 0x49,
  0x6C, 0xB9, 0x00, 0xC0, 0xEE, 0x46, 0x6C, 0xEE, 0x46, 0x6C, 0xAD, 0x46, 0x6C, 0x8D, 0x47, 0x6C,
  0xAD, 0x46, 0x6C, 0xC9, 0x80, 0x90, 0x05, 0xA9, 0x00, 0x8D, 0x46, 0x6C, 0xA9, 0x20, 0x8D, 0x49,
  0x6C, 0xAD, 0x02, 0x03, 0xD0, 0x0A, 0xAD, 0xC2, 0x69, 0xD0, 0x05, 0xA9, 0x30, 0x8D, 0x49, 0x6C,
  0xCE, 0x48, 0x6C, 0xD0, 0xBB, 0x60, 0x00, 0x00, 0x0F, 0x20, 0x03, 0xE6, 0x0E, 0xD0, 0x02, 0xE6,
  0x0F, 0xA0, 0x00, 0xB1, 0x0E, 0x10, 0x0B, 0xA9, 0x00, 0x85, 0x0E, 0xA9, 0x18, 0x85, 0x0F, 0x4C,
  0x51, 0x6C, 0x60, 0xC9, 0xCA, 0xCB, 0xCD, 0x88, 0x95, 0x00, 0xC9, 0xCA, 0xCB, 0xCD, 0x88, 0x95,
  0x00, 0x00, 0x20, 0x94, 0x65, 0x48, 0xAD, 0x02, 0x03, 0xF0, 0x15, 0xAD, 0x71, 0x6C, 0xF0, 0x09,
  0xAD, 0x61, 0xC0, 0x30, 0x04, 0x68, 0xA9, 0x92, 0x60, 0x68, 0x10, 0x03, 0xA9, 0x92, 0x60, 0x60,
  0x68, 0xA2, 0x05, 0xDD, 0x63, 0x6C, 0xF0, 0x06, 0xCA, 0x10, 0xF8, 0x4C, 0xA1, 0x6C, 0xBD, 0x6A,
  0x6C, 0xC9, 0x80, 0xD0, 0x06, 0xA9, 0x01, 0x8D, 0x71, 0x6C, 0x60, 0xC9, 0x8B, 0xD0, 0x06, 0xA9,
  0x00, 0x8D, 0x71, 0x6C, 0x60, 0xC9, 0x00, 0x10, 0x01, 0x60, 0xAE, 0x71, 0x6C, 0xD0, 0x03, 0xC9,
  0x00, 0x60, 0xA0, 0x00, 0xAD, 0x5B, 0xC0, 0xAD, 0x62, 0xC0, 0x30, 0x03, 0xA2, 0x00, 0xC8, 0xAD,
  0x63, 0xC0, 0x30, 0x03, 0xA2, 0x03, 0xC8, 0xAD, 0x5A, 0xC0, 0xAD, 0x62, 0xC0, 0x30, 0x03, 0xA2,
  0x01, 0xC8, 0xAD, 0x63, 0xC0, 0x30, 0x03, 0xA2, 0x02, 0xC8, 0xC0, 0x01, 0xF0, 0x03, 0xA9, 0x00,
  0x60, 0xBD, 0x6A, 0x6C, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xA5, 0x03, 0x48, 0xA5, 0x03, 0xA4, 0x02, 0x20, 0x00, 0xF8, 0xA5, 0x03, 0xC5, 0x08, 0xF0, 0x05,
  0xE6, 0x03, 0x4C, 0x03, 0x70, 0x68, 0x85, 0x03, 0x60, 0xA0, 0x00, 0xB1, 0x0A, 0xE6, 0x0A, 0xD0,
  0x02, 0xE6, 0x0B, 0x60, 0xF0, 0x02, 0xA9, 0x05, 0x4C, 0x64, 0xF8, 0xA9, 0x00, 0x85, 0x04, 0xA9,
  0x20, 0x85, 0x05, 0xA0, 0x00, 0xA9, 0x00, 0x91, 0x04, 0xC8, 0xD0, 0xF9, 0xE6, 0x05, 0xA5, 0x05,
  0xC9, 0x40, 0xD0, 0xF1, 0x60, 0x20, 0x2B, 0x70, 0xA9, 0x00, 0x8D, 0x46, 0x6C, 0xAD, 0x50, 0xC0,
  0xAD, 0x57, 0xC0, 0xAD, 0x55, 0xC0, 0xAD, 0x52, 0xC0, 0xA9, 0x04, 0x85, 0x02, 0xA9, 0x00, 0x85,
  0x03, 0x88, 0xD0, 0xFD, 0xC6, 0x03, 0xD0, 0xF9, 0xC6, 0x02, 0xD0, 0xF5, 0xA9, 0x27, 0x85, 0x03,
  0xA9, 0x00, 0x20, 0x24, 0x70, 0xA9, 0x27, 0x85, 0x2C, 0xA0, 0x00, 0xA5, 0x03, 0x20, 0x19, 0xF8,
  0xC6, 0x03, 0x10, 0xF1, 0xA9, 0x14, 0x85, 0x22, 0xA9, 0x15, 0x85, 0x00, 0xA9, 0x0D, 0x85, 0x01,
  0x20, 0x24, 0x70, 0xAD, 0x01, 0x03, 0xF0, 0x14, 0xA9, 0x01, 0xA8, 0x20, 0x00, 0xF8, 0xAD, 0x01,
  0x03, 0xC9, 0x01, 0xF0, 0x07, 0xA9, 0x01, 0xA0, 0x26, 0x20, 0x00, 0xF8, 0xA0, 0x00, 0xA9, 0x00,
  0x20, 0x19, 0xF8, 0xA0, 0x00, 0xA9, 0x27, 0x20, 0x19, 0xF8, 0xA9, 0x00, 0x85, 0x02, 0xA9, 0x00,
  0x85, 0x03, 0xA9, 0x27, 0x85, 0x08, 0x20, 0x00, 0x70, 0xA9, 0x00, 0x85, 0x03, 0xA9, 0x27, 0x85,
  0x02, 0x20, 0x00, 0x70, 0xA9, 0x00, 0x85, 0x02, 0x85, 0x03, 0xA9, 0x27, 0x85, 0x08, 0x20, 0x48,
  0x61, 0xA9, 0x00, 0x85, 0x02, 0xA9, 0x27, 0x85, 0x03, 0x20, 0x48, 0x61, 0xA9, 0x00, 0x85, 0x02,
  0x85, 0x03, 0x20, 0x5A, 0x61, 0xA9, 0x00, 0x85, 0x03, 0xA9, 0x27, 0x85, 0x02, 0x20, 0x5A, 0x61,
  0xA9, 0x03, 0x85, 0x01, 0xA9, 0x27, 0x85, 0x03, 0xA9, 0x12, 0x85, 0x02, 0xA9, 0x16, 0x85, 0x08,
  0x20, 0x48, 0x61, 0xAE, 0x03, 0x03, 0xA9, 0x00, 0x85, 0x0A, 0xA9, 0x80, 0x85, 0x0B, 0xCA, 0xF0,
  0x0A, 0x20, 0x19, 0x70, 0xC9, 0x2A, 0xF0, 0xF6, 0x4C, 0x21, 0x71, 0x20, 0x19, 0x70, 0xC9, 0x45,
  0xD0, 0x08, 0xA2, 0x01, 0x8E, 0x03, 0x03, 0x4C, 0x13, 0x71, 0xC9, 0x48, 0xD0, 0x2A, 0x20, 0x19,
  0x70, 0x85, 0x01, 0x20, 0x19, 0x70, 0x85, 0x02, 0x20, 0x19, 0x70, 0x85, 0x08, 0x20, 0x19, 0x70,
  0x85, 0x03, 0xA5, 0x01, 0x20, 0x24, 0x70, 0xA5, 0x08, 0x85, 0x2C, 0xA5, 0x03, 0xA4, 0x02, 0x20,
  0x19, 0xF8, 0x20, 0x48, 0x61, 0x4C, 0x2B, 0x71, 0xC9, 0x56, 0xD0, 0x22, 0x20, 0x19, 0x70, 0x85,
  0x01, 0x20, 0x19, 0x70, 0x85, 0x03, 0x20, 0x19, 0x70, 0x85, 0x08, 0x20, 0x19, 0x70, 0x85, 0x02,
  0xA5, 0x01, 0x20, 0x24, 0x70, 0x20, 0x00, 0x70, 0x20, 0x5A, 0x61, 0x4C, 0x2B, 0x71, 0xC9, 0x50,
  0xD0, 0x21, 0x20, 0x19, 0x70, 0x85, 0x01, 0x20, 0x19, 0x70, 0x85, 0x02, 0x20, 0x19, 0x70, 0x85,
  0x03, 0xA5, 0x01, 0x20, 0x24, 0x70, 0xA5, 0x03, 0xA4, 0x02, 0x20, 0x00, 0xF8, 0x20, 0xE4, 0x60,
  0x4C, 0x2B, 0x71, 0xC9, 0x54, 0xD0, 0x09, 0x20, 0x19, 0x70, 0x8D, 0x04, 0x03, 0x4C, 0x2B, 0x71,
  0xC9, 0x2A, 0xF0, 0x03, 0x4C, 0x2B, 0x71, 0x60, 0x10, 0x15, 0x20, 0x15, 0x00, 0xA9, 0x00, 0x8D,
  0xCB, 0x71, 0x8D, 0xCC, 0x71, 0xAE, 0x01, 0x03, 0xAC, 0x03, 0x03, 0xF8, 0xBD, 0xC8, 0x71, 0x18,
  0x6D, 0xCB, 0x71, 0x8D, 0xCB, 0x71, 0xAD, 0xCC, 0x71, 0x69, 0x00, 0x8D, 0xCC, 0x71, 0x88, 0xD0,
  0xEB, 0xD8, 0x60, 0x48, 0x4A, 0x4A, 0x4A, 0x4A, 0xC9, 0x00, 0xF0, 0x02, 0x85, 0x2C, 0x48, 0xA5,
  0x2C, 0xD0, 0x04, 0x68, 0x4C, 0x0E, 0x72, 0x68, 0x18, 0x69, 0xB0, 0x20, 0xED, 0xFD, 0x68, 0x29,
  0x0F, 0xC9, 0x00, 0xF0, 0x02, 0x85, 0x2C, 0x48, 0xA5, 0x2C, 0xD0, 0x02, 0x68, 0x60, 0x68, 0x18,
  0x69, 0xB0, 0x20, 0xED, 0xFD, 0x60, 0xA5, 0x2C, 0xD0, 0x05, 0xA9, 0xB0, 0x4C, 0xED, 0xFD, 0x60,
  0x68, 0x85, 0x0C, 0x68, 0x85, 0x0D, 0x20, 0x68, 0xFC, 0xE6, 0x0C, 0xD0, 0x02, 0xE6, 0x0D, 0xA0,
  0x00, 0xB1, 0x0C, 0xF0, 0x06, 0x20, 0xED, 0xFD, 0x4C, 0x39, 0x72, 0xA5, 0x0D, 0x48, 0xA5, 0x0C,
  0x48, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x02, 0x01,
  0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x64, 0xF8, 0xAD, 0xCB, 0x71, 0x18, 0x6D, 0x52, 0x72, 0x8D,
  0x52, 0x72, 0xAD, 0xCC, 0x71, 0x6D, 0x53, 0x72, 0x8D, 0x53, 0x72, 0xAD, 0x54, 0x72, 0x69, 0x00,
  0x8D, 0x54, 0x72, 0xAD, 0x55, 0x72, 0x69, 0x00, 0x8D, 0x55, 0x72, 0xD8, 0x60, 0xAD, 0x55, 0x72,
  0xCD, 0x59, 0x72, 0x90, 0x38, 0xD0, 0x1E, 0xAD, 0x54, 0x72, 0xCD, 0x58, 0x72, 0x90, 0x2E, 0xD0,
  0x14, 0xAD, 0x53, 0x72, 0xCD, 0x57, 0x72, 0x90, 0x24, 0xD0, 0x0A, 0xAD, 0x52, 0x72, 0xCD, 0x56,
  0x72, 0x90, 0x1A, 0xD0, 0x00, 0xAD, 0x52, 0x72, 0x8D, 0x56, 0x72, 0xAD, 0x53, 0x72, 0x8D, 0x57,
  0x72, 0xAD, 0x54, 0x72, 0x8D, 0x58, 0x72, 0xAD, 0x55, 0x72, 0x8D, 0x59, 0x72, 0x60, 0xA9, 0x14,
  0x85, 0x25, 0xA9, 0x00, 0x85, 0x24, 0x20, 0x30, 0x72, 0xD3, 0xC3, 0xCF, 0xD2, 0xC5, 0xBA, 0xA0,
  0xA0, 0x00, 0xA9, 0x00, 0x85, 0x2C, 0xAD, 0x55, 0x72, 0x20, 0xF3, 0x71, 0xAD, 0x54, 0x72, 0x20,
  0xF3, 0x71, 0xAD, 0x53, 0x72, 0x20, 0xF3, 0x71, 0xAD, 0x52, 0x72, 0x20, 0xF3, 0x71, 0x20, 0x26,
  0x72, 0xA9, 0x14, 0x85, 0x24, 0x20, 0x30, 0x72, 0xC8, 0xC9, 0xA0, 0xD3, 0xC3, 0xCF, 0xD2, 0xC5,
  0xBA, 0xA0, 0xA0, 0x00, 0xA9, 0x00, 0x85, 0x2C, 0xAD, 0x59, 0x72, 0x20, 0xF3, 0x71, 0xAD, 0x58,
  0x72, 0x20, 0xF3, 0x71, 0xAD, 0x57, 0x72, 0x20, 0xF3, 0x71, 0xAD, 0x56, 0x72, 0x20, 0xF3, 0x71,
  0x20, 0x26, 0x72, 0xA9, 0x00, 0x85, 0x24, 0xA9, 0x15, 0x85, 0x25, 0x20, 0x30, 0x72, 0xC1, 0xD0,
  0xD0, 0xCC, 0xC5, 0xD3, 0xA0, 0xCC, 0xC5, 0xC6, 0xD4, 0xBA, 0xA0, 0xA0, 0x00, 0xA9, 0x00, 0x85,
  0x2C, 0xAD, 0x5B, 0x72, 0x20, 0xF3, 0x71, 0xAD, 0x5A, 0x72, 0x20, 0xF3, 0x71, 0x20, 0x26, 0x72,
  0xA9, 0xA0, 0x20, 0xED, 0xFD, 0xA9, 0x14, 0x85, 0x24, 0x20, 0x30, 0x72, 0xD6, 0xC1, 0xCC, 0xD5,
  0xC5, 0xBA, 0xA0, 0xA0, 0x00, 0xA9, 0x00, 0x85, 0x2C, 0xAD, 0xCC, 0x71, 0x20, 0xF3, 0x71, 0xAD,
  0xCB, 0x71, 0x20, 0xF3, 0x71, 0x20, 0x26, 0x72, 0xA9, 0x16, 0x85, 0x25, 0xA9, 0x00, 0x85, 0x24,
  0x20, 0x30, 0x72, 0xD3, 0xCE, 0xC1, 0xCB, 0xC5, 0xD3, 0xA0, 0xCC, 0xC5, 0xC6, 0xD4, 0xBA, 0xA0,
  0xA0, 0x00, 0xA9, 0x00, 0x85, 0x2C, 0x20, 0xF3, 0x71, 0xAD, 0x5E, 0x72, 0x20, 0xF3, 0x71, 0x20,
  0x26, 0x72, 0xA9, 0x14, 0x85, 0x24, 0x20, 0x30, 0x72, 0xCC, 0xC5, 0xD6, 0xC5, 0xCC, 0xBA, 0xA0,
  0xA0, 0x00, 0xA9, 0x00, 0x85, 0x2C, 0xAD, 0x65, 0x72, 0x20, 0xF3, 0x71, 0x20, 0x26, 0x72, 0xA9,
  0x00, 0x85, 0x25, 0x20, 0x68, 0xFC, 0x60, 0x01, 0xAD, 0xD7, 0x73, 0xD0, 0x0C, 0xA9, 0x01, 0x8D,
  0x02, 0x03, 0x8D, 0x01, 0x03, 0x8D, 0xD7, 0x73, 0x60, 0xA9, 0x17, 0x85, 0x25, 0xA9, 0x00, 0x85,
  0x24, 0x20, 0x30, 0x72, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xC8, 0xCF,
  0xD7, 0xA0, 0xCD, 0xC1, 0xCE, 0xD9, 0xA0, 0xD0, 0xCC, 0xD5, 0xCD, 0xD3, 0xA0, 0xA8, 0xB0, 0xAD,
  0xB2, 0xA9, 0xBF, 0x00, 0xA9, 0xE8, 0x85, 0x02, 0xA9, 0x00, 0x85, 0x03, 0xC8, 0xD0, 0xFD, 0xAD,
  0x00, 0xC0, 0x30, 0x3D, 0xE6, 0x03, 0xD0, 0xF4, 0xAD, 0x71, 0x6C, 0xF0, 0x24, 0xAD, 0x5B, 0xC0,
  0xAD, 0x62, 0xC0, 0x30, 0x05, 0xA9, 0xB1, 0x4C, 0x61, 0x74, 0xAD, 0x5A, 0xC0, 0xAD, 0x62, 0xC0,
  0x30, 0x05, 0xA9, 0xB0, 0x4C, 0x61, 0x74, 0xAD, 0x63, 0xC0, 0x30, 0x05, 0xA9, 0xB2, 0x4C, 0x61,
  0x74, 0xE6, 0x02, 0xD0, 0xC7, 0xA9, 0x01, 0x8D, 0x02, 0x03, 0x8D, 0x01, 0x03, 0x8D, 0x10, 0xC0,
  0x60, 0x8D, 0x10, 0xC0, 0xC9, 0xC3, 0xF0, 0x17, 0xC9, 0xB0, 0x90, 0xB0, 0xC9, 0xB3, 0xB0, 0xAC,
  0x38, 0xE9, 0xB0, 0x8D, 0x01, 0x03, 0xA9, 0x00, 0x8D, 0x02, 0x03, 0x8D, 0x10, 0xC0, 0x60, 0x20,
  0x2B, 0x70, 0xAD, 0x52, 0xC0, 0x20, 0x41, 0x66, 0xA9, 0x01, 0x85, 0x25, 0x20, 0x30, 0x72, 0x8D,
  0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xC1, 0xC2, 0xD3, 0xCF, 0xCC, 0xD5, 0xD4, 0xC5, 0xA0, 0xA0,
  0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xD2, 0xC5, 0xCC, 0xC1, 0xD4, 0xC9,
  0xD6, 0xC5, 0x8D, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xC4, 0xC9, 0xD2, 0xC5, 0xC3, 0xD4, 0xC9,
  0xCF, 0xCE, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xC4, 0xC9, 0xD2,
  0xC5, 0xC3, 0xD4, 0xC9, 0xCF, 0xCE, 0x8D, 0x8D, 0x8D, 0x8D, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0,
  0xA0, 0xA0, 0xA0, 0xA0, 0xE2, 0x8D, 0x8D, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xE4,
  0xA0, 0xAB, 0xA0, 0xE5, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0,
  0xA0, 0xA0, 0xA0, 0xE4, 0xA0, 0xA0, 0xA0, 0xE5, 0x8D, 0x8D, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0,
  0xA0, 0xA0, 0xA0, 0xA0, 0xE3, 0x8D, 0x8D, 0x8D, 0x8D, 0x8D, 0x8D, 0x8D, 0xA0, 0xA0, 0xA0, 0xA0,
  0xA0, 0xD0, 0xD2, 0xC5, 0xD3, 0xD3, 0xA0, 0xD4, 0xC8, 0xC5, 0xA0, 0xCB, 0xC5, 0xD9, 0xA0, 0xC6,
  0xCF, 0xD2, 0xA0, 0xD4, 0xC8, 0xC9, 0xD3, 0xA0, 0xC6, 0xD5, 0xCE, 0xC3, 0xD4, 0xC9, 0xCF, 0xCE,
  0x00, 0xA2, 0x00, 0xBD, 0x63, 0x6C, 0x20, 0x90, 0x75, 0xE8, 0xE0, 0x06, 0xD0, 0xF5, 0xA9, 0x0C,
  0x85, 0x01, 0xA9, 0x02, 0x85, 0x00, 0xA9, 0x12, 0x85, 0x03, 0xA9, 0x1E, 0x85, 0x02, 0x20, 0xE4,
  0x60, 0xA9, 0x13, 0x85, 0x03, 0xA9, 0x1D, 0x85, 0x08, 0xA9, 0x0A, 0x85, 0x00, 0x20, 0x5A, 0x61,
  0xA9, 0x0E, 0x85, 0x00, 0x20, 0xE4, 0x60, 0xA2, 0x00, 0x20, 0xD1, 0x75, 0x9D, 0x63, 0x6C, 0x20,
  0x90, 0x75, 0xE8, 0xE0, 0x06, 0xD0, 0xF2, 0xA9, 0xF0, 0x85, 0x36, 0xA9, 0xFD, 0x85, 0x37, 0x60,
  0x86, 0x02, 0xC9, 0x88, 0xD0, 0x02, 0xA9, 0xE6, 0xC9, 0x95, 0xD0, 0x02, 0xA9, 0xE7, 0x48, 0xBD,
  0xB3, 0x75, 0x85, 0x24, 0xBD, 0xB9, 0x75, 0x85, 0x25, 0x20, 0x68, 0xFC, 0x68, 0x20, 0xED, 0xFD,
  0xA6, 0x02, 0x60, 0x0A, 0x06, 0x0E, 0x0A, 0x1A, 0x22, 0x05, 0x09, 0x09, 0x0D, 0x09, 0x09, 0x0A,
  0x08, 0x0C, 0x0A, 0x1C, 0x20, 0x07, 0x09, 0x09, 0x0B, 0x09, 0x09, 0xE2, 0xE4, 0xE5, 0xE3, 0xE4,
  0xE5, 0x86, 0x02, 0xBD, 0xBF, 0x75, 0x85, 0x24, 0xBD, 0xC5, 0x75, 0x85, 0x25, 0x20, 0x68, 0xFC,
  0xA6, 0x02, 0xA9, 0xA0, 0x20, 0xED, 0xFD, 0xA0, 0x00, 0xCA, 0xD0, 0xFD, 0xA9, 0x41, 0xF0, 0x1C,
  0x88, 0xD0, 0xF6, 0xA6, 0x02, 0xBD, 0xBF, 0x75, 0x85, 0x24, 0xBD, 0xC5, 0x75, 0x85, 0x25, 0x20,
  0x68, 0xFC, 0xA6, 0x02, 0xBD, 0xCB, 0x75, 0x20, 0xED, 0xFD, 0xA0, 0x00, 0xCA, 0xD0, 0xFD, 0xAD,
  0x00, 0xC0, 0x30, 0x08, 0x88, 0xD0, 0xF5, 0xA6, 0x02, 0x4C, 0xD1, 0x75, 0x8D, 0x10, 0xC0, 0xC9,
  0xA1, 0xB0, 0x0D, 0xC9, 0x88, 0xF0, 0x09, 0xC9, 0x95, 0xF0, 0x05, 0xA6, 0x02, 0x4C, 0xD1, 0x75,
  0xA6, 0x02, 0x60, 0xF8, 0xAD, 0x5E, 0x72, 0x18, 0x69, 0x01, 0x8D, 0x5E, 0x72, 0xD8, 0x20, 0xA9,
  0x64, 0x60, 0x20, 0x4B, 0x6C, 0x85, 0x02, 0x20, 0x4B, 0x6C, 0x85, 0x03, 0xA5, 0x03, 0xA4, 0x02,
  0x20, 0x71, 0xF8, 0xD0, 0xED, 0xA9, 0x0F, 0x20, 0x64, 0xF8, 0xA5, 0x03, 0xA4, 0x02, 0x20, 0x00,
  0xF8, 0xA9, 0x01, 0x85, 0x00, 0xA9, 0x09, 0x85, 0x01, 0x20, 0xE4, 0x60, 0xF8, 0xAD, 0x5F, 0x72,
  0x18, 0x69, 0x01, 0x8D, 0x5F, 0x72, 0xAD, 0x60, 0x72, 0x69, 0x00, 0x8D, 0x60, 0x72, 0xD8, 0x60,
  0xA9, 0xF0, 0x85, 0x36, 0xA9, 0xFD, 0x85, 0x37, 0x1A, 0x5A, 0x3A, 0x5A, 0x1A, 0x5A, 0x20, 0x80,
  0x7A, 0x20, 0x80, 0x79, 0x20, 0x8D, 0x72, 0xA9, 0x01, 0x8D, 0x03, 0x03, 0x8D, 0x65, 0x72, 0xA9,
  0x00, 0x8D, 0x52, 0x72, 0x8D, 0x53, 0x72, 0x8D, 0x54, 0x72, 0x8D, 0x55, 0x72, 0xA9, 0x02, 0x8D,
  0x5E, 0x72, 0xA9, 0x00, 0x8D, 0x5F, 0x72, 0xAD, 0x5E, 0x72, 0x8D, 0xB2, 0x78, 0xA9, 0x10, 0x8D,
  0x63, 0x72, 0xA9, 0x00, 0x8D, 0x64, 0x72, 0xA9, 0x00, 0x8D, 0x5F, 0x72, 0x8D, 0x60, 0x72, 0x8D,
  0x61, 0x72, 0x8D, 0x62, 0x72, 0xAD, 0x63, 0x72, 0x8D, 0x5A, 0x72, 0xAD, 0x64, 0x72, 0x8D, 0x5B,
  0x72, 0x20, 0x45, 0x70, 0xAD, 0x04, 0x03, 0x8D, 0x66, 0x72, 0x20, 0xCD, 0x71, 0xAD, 0x54, 0xC0,
  0xAD, 0x53, 0xC0, 0x20, 0x42, 0x76, 0x20, 0xE4, 0x60, 0xA9, 0x52, 0x8D, 0x00, 0x03, 0xA9, 0x00,
  0x8D, 0x05, 0x03, 0xAD, 0x66, 0x72, 0x8D, 0x55, 0x62, 0xA9, 0x14, 0x85, 0x22, 0x20, 0x58, 0xFC,
  0x20, 0xCE, 0x72, 0x20, 0x56, 0x62, 0x4C, 0x39, 0x77, 0xAD, 0x66, 0x72, 0x8D, 0x55, 0x62, 0xA9,
  0x14, 0x85, 0x22, 0x20, 0x58, 0xFC, 0x20, 0xCE, 0x72, 0xAD, 0x00, 0x03, 0xC9, 0x03, 0x90, 0x06,
  0x38, 0xE9, 0x02, 0x8D, 0x00, 0x03, 0x20, 0x88, 0x62, 0xAD, 0x53, 0x62, 0xC9, 0x0F, 0xF0, 0x03,
  0x4C, 0xE6, 0x77, 0xF8, 0xAD, 0x5F, 0x72, 0x38, 0xE9, 0x01, 0x8D, 0x5F, 0x72, 0xAD, 0x60, 0x72,
  0xE9, 0x00, 0x8D, 0x60, 0x72, 0xAD, 0x5A, 0x72, 0x38, 0xE9, 0x01, 0x8D, 0x5A, 0x72, 0xAD, 0x5B,
  0x72, 0xE9, 0x00, 0x8D, 0x5B, 0x72, 0xAD, 0x61, 0x72, 0x18, 0x69, 0x01, 0x8D, 0x61, 0x72, 0xAD,
  0x62, 0x72, 0x69, 0x00, 0x8D, 0x62, 0x72, 0xD8, 0xAD, 0x62, 0x72, 0xD0, 0x0A, 0xAD, 0x61, 0x72,
  0xC9, 0x11, 0xB0, 0x03, 0x20, 0x67, 0x72, 0xAD, 0x54, 0x62, 0x18, 0x69, 0x0A, 0x8D, 0x54, 0x62,
  0xAD, 0x5A, 0x72, 0xD0, 0x3B, 0xAD, 0x5B, 0x72, 0xD0, 0x36, 0xA9, 0x06, 0x85, 0x01, 0xA9, 0x00,
  0x85, 0x03, 0xA9, 0x12, 0x85, 0x02, 0xA9, 0x16, 0x85, 0x08, 0xA9, 0x15, 0x85, 0x00, 0x20, 0x48,
  0x61, 0xA9, 0x00, 0x85, 0x01, 0xA9, 0x14, 0x85, 0x02, 0x20, 0xE4, 0x60, 0xA9, 0xFF, 0x8D, 0x66,
  0x72, 0xA9, 0x00, 0x20, 0x64, 0xF8, 0xA9, 0x00, 0xA0, 0x14, 0x20, 0x00, 0xF8, 0x4C, 0x19, 0x77,
  0xAD, 0x5F, 0x72, 0xF0, 0x03, 0x4C, 0x19, 0x77, 0xAD, 0x60, 0x72, 0xF0, 0x03, 0x4C, 0x19, 0x77,
  0x20, 0x42, 0x76, 0x4C, 0x19, 0x77, 0xC9, 0x00, 0xD0, 0x1F, 0xF8, 0x18, 0xAD, 0x65, 0x72, 0x69,
  0x01, 0x8D, 0x65, 0x72, 0xD8, 0xEE, 0x03, 0x03, 0xAD, 0x5E, 0x72, 0xCD, 0xB2, 0x78, 0xD0, 0x03,
  0x20, 0xB3, 0x78, 0x20, 0x33, 0x76, 0x4C, 0xB7, 0x76, 0xC9, 0xFE, 0xD0, 0x3A, 0xAD, 0x5B, 0x72,
  0xD0, 0x05, 0xAD, 0x5A, 0x72, 0xF0, 0x30, 0xF8, 0xAD, 0x63, 0x72, 0x18, 0x69, 0x03, 0x8D, 0x63,
  0x72, 0xAD, 0x64, 0x72, 0x69, 0x00, 0x8D, 0x64, 0x72, 0xAD, 0x5A, 0x72, 0x18, 0x69, 0x03, 0x8D,
  0x5A, 0x72, 0xAD, 0x5B, 0x72, 0x69, 0x00, 0x8D, 0x5B, 0x72, 0xD8, 0x20, 0x42, 0x76, 0x20, 0x42,
  0x76, 0x20, 0x42, 0x76, 0x4C, 0x19, 0x77, 0xAD, 0x53, 0x62, 0xC9, 0xFF, 0xD0, 0x03, 0x4C, 0x91,
  0x76, 0xC9, 0xFE, 0xD0, 0x03, 0x4C, 0x19, 0x77, 0xAD, 0x02, 0x03, 0xD0, 0x3D, 0xA9, 0x17, 0x85,
  0x25, 0xA9, 0x00, 0x85, 0x24, 0x20, 0x30, 0x72, 0x03, 0x12, 0x01, 0x13, 0x08, 0xA0, 0xD0, 0xD2,
  0xC5, 0xD3, 0xD3, 0xA0, 0xD3, 0xD0, 0xC1, 0xC3, 0xC5, 0xA0, 0xD4, 0xCF, 0xA0, 0xC3, 0xCF, 0xCE,
  0xD4, 0xC9, 0xCE, 0xD5, 0xC5, 0x00, 0xAD, 0x71, 0x6C, 0xF0, 0x05, 0xAD, 0x61, 0xC0, 0x10, 0x0A,
  0xAD, 0x00, 0xC0, 0xC9, 0xA0, 0xD0, 0xEF, 0x8D, 0x10, 0xC0, 0xAD, 0x5E, 0x72, 0xD0, 0x03, 0x4C,
  0x91, 0x76, 0xF8, 0xAD, 0x5E, 0x72, 0x38, 0xE9, 0x01, 0x8D, 0x5E, 0x72, 0xD8, 0x4C, 0xC7, 0x76,
  0x00, 0x00, 0x02, 0xF8, 0x18, 0xAD, 0xCB, 0x71, 0x6D, 0xCB, 0x71, 0x8D, 0xB0, 0x78, 0xAD, 0xCC,
  0x71, 0x6D, 0xCC, 0x71, 0x8D, 0xB1, 0x78, 0xD8, 0x20, 0x67, 0x72, 0x20, 0x67, 0x72, 0x20, 0xCE,
  0x72, 0xA9, 0x01, 0x85, 0x00, 0xA9, 0x09, 0x85, 0x01, 0xA9, 0x0D, 0x85, 0x02, 0xA9, 0x10, 0x85,
  0x03, 0xA9, 0x1A, 0x85, 0x08, 0x20, 0x48, 0x61, 0xA9, 0x0D, 0x85, 0x02, 0xA9, 0x15, 0x85, 0x03,
  0x20, 0x48, 0x61, 0xA9, 0x10, 0x85, 0x03, 0xA9, 0x15, 0x85, 0x08, 0x20, 0x5A, 0x61, 0xA9, 0x0D,
  0x85, 0x02, 0xA9, 0x10, 0x85, 0x03, 0x20, 0x5A, 0x61, 0xA9, 0x00, 0x85, 0x01, 0xA9, 0x19, 0x85,
  0x08, 0xA9, 0x11, 0x85, 0x03, 0xA9, 0x0E, 0x85, 0x02, 0x20, 0x48, 0x61, 0xA9, 0x0E, 0x85, 0x02,
  0xE6, 0x03, 0x20, 0x48, 0x61, 0xA9, 0x0E, 0x85, 0x02, 0xE6, 0x03, 0x20, 0x48, 0x61, 0xA9, 0x0E,
  0x85, 0x02, 0xE6, 0x03, 0x20, 0x48, 0x61, 0xA9, 0x0F, 0x85, 0x24, 0xA9, 0x09, 0x85, 0x25, 0x20,
  0x41, 0x66, 0x20, 0x30, 0x72, 0xC2, 0xCF, 0xCE, 0xD5, 0xD3, 0xBA, 0xA0, 0x00, 0xA9, 0x00, 0x85,
  0x2C, 0xAD, 0xB1, 0x78, 0x20, 0xF3, 0x71, 0xAD, 0xB0, 0x78, 0x20, 0xF3, 0x71, 0xA9, 0xF0, 0x85,
  0x36, 0xA9, 0xFD, 0x85, 0x37, 0xA9, 0x20, 0x85, 0x02, 0xA2, 0x80, 0x8A, 0xA8, 0x88, 0xD0, 0xFD,
  0xAC, 0x49, 0x6C, 0xB9, 0x00, 0xC0, 0xCA, 0xD0, 0xF2, 0xC6, 0x02, 0xD0, 0xEC, 0x60, 0x00, 0x00,
  0xA5, 0x0F, 0xC9, 0x1F, 0xB0, 0x04, 0xC9, 0x18, 0xB0, 0x06, 0xA5, 0x0E, 0x29, 0xDE, 0x85, 0x0E,
  0xA5, 0x0F, 0x29, 0x1F, 0x09, 0x18, 0x85, 0x0F, 0x4C, 0xD8, 0x73, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xA9, 0x2B, 0x8D, 0x46, 0x70, 0xA9, 0x70, 0x8D, 0x47, 0x70, 0xAD, 0x81, 0xC0, 0xAD, 0x81, 0xC0,
  0xA9, 0x40, 0x8D, 0xFC, 0xFF, 0x8D, 0xF2, 0x03, 0xA9, 0x7A, 0x8D, 0xFD, 0xFF, 0x8D, 0xF3, 0x03,
  0x49, 0xA5, 0x8D, 0xF4, 0x03, 0xAD, 0x80, 0xC0, 0xAD, 0x55, 0xC0, 0x20, 0x2B, 0x70, 0xA9, 0x20,
  0x8D, 0x8E, 0x76, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xA0, 0x00, 0x84, 0x00, 0xA9, 0x18, 0x85, 0x01, 0x98, 0xA2, 0x61, 0x91, 0x00, 0xC8, 0xD0, 0xFB,
  0xE6, 0x01, 0xCA, 0xD0, 0xF6, 0x20, 0xDD, 0xFB, 0xA5, 0x2B, 0x4A, 0x4A, 0x4A, 0x4A, 0x09, 0xC0,
  0x38, 0xE9, 0x01, 0x48, 0xA9, 0xFF, 0x48, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xA6, 0x2B, 0xBD, 0x89, 0xC0, 0xA9, 0x00, 0x20, 0xA8, 0xFC, 0xA9, 0x25, 0x20, 0xA0, 0x7A, 0x20,
  0x40, 0x7B, 0xBD, 0x88, 0xC0, 0x20, 0x2B, 0x70, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x86, 0x2B, 0x85, 0x2A, 0xCD, 0x2F, 0x7B, 0xF0, 0x53, 0xA9, 0x00, 0x85, 0x26, 0xAD, 0x2F, 0x7B,
  0x85, 0x27, 0x38, 0xE5, 0x2A, 0xF0, 0x33, 0xB0, 0x07, 0x49, 0xFF, 0xEE, 0x2F, 0x7B, 0x90, 0x05,
  0x69, 0xFE, 0xCE, 0x2F, 0x7B, 0xC5, 0x26, 0x90, 0x02, 0xA5, 0x26, 0xC9, 0x0C, 0xB0, 0x01, 0xA8,
  0x38, 0x20, 0xEE, 0x7A, 0xB9, 0x11, 0x7B, 0x20, 0x00, 0x7B, 0xA5, 0x27, 0x18, 0x20, 0xF1, 0x7A,
  0xB9, 0x1D, 0x7B, 0x20, 0x00, 0x7B, 0xE6, 0x26, 0xD0, 0xC3, 0x20, 0x00, 0x7B, 0x18, 0xAD, 0x2F,
  0x7B, 0x29, 0x03, 0x2A, 0x05, 0x2B, 0xAA, 0xBD, 0x80, 0xC0, 0xA6, 0x2B, 0x60, 0xAA, 0xA0, 0xA0,
  0xA2, 0x11, 0xCA, 0xD0, 0xFD, 0xE6, 0x46, 0xD0, 0x02, 0xE6, 0x47, 0x38, 0xE9, 0x01, 0xD0, 0xF0,
  0x60, 0x01, 0x30, 0x28, 0x24, 0x20, 0x1E, 0x1D, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x70, 0x2C, 0x26,
  0x22, 0x1F, 0x1E, 0x1D, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xBD, 0x8C, 0xC0, 0x10, 0xFB, 0xC9, 0xA5, 0xD0, 0xF7, 0xBD, 0x8C, 0xC0, 0x10, 0xFB, 0xC9, 0xB5,
  0xD0, 0xF3, 0xBD, 0x8C, 0xC0, 0x10, 0xFB, 0xC9, 0xD5, 0xD0, 0xEA, 0x60, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x54, 0x64, 0x2A, 0x48, 0x0D, 0x0A, 0x1E, 0x14, 0x2A, 0x48, 0x0B, 0x0A, 0x1E, 0x14, 0x56, 0x0B,
  0x0A, 0x1E, 0x14, 0x2A, 0x48, 0x0F, 0x0A, 0x1E, 0x0A, 0x48, 0x0F, 0x0A, 0x1E, 0x14, 0x48, 0x0F,
  0x0A, 0x1E, 0x1E, 0x56, 0x0F, 0x0A, 0x1E, 0x0A, 0x2A, 0x56, 0x0E, 0x0A, 0x1E, 0x0A, 0x56, 0x0E,
  0x0A, 0x1E, 0x14, 0x56, 0x0E, 0x0A, 0x1E, 0x1E, 0x48, 0x0E, 0x0A, 0x1E, 0x14, 0x2A, 0x56, 0x08,
  0x0A, 0x1E, 0x0A, 0x56, 0x08, 0x0A, 0x1E, 0x1E, 0x48, 0x08, 0x0A, 0x1E, 0x1E, 0x48, 0x08, 0x0A,
  0x13, 0x0A, 0x48, 0x08, 0x16, 0x1E, 0x0A, 0x2A, 0x56, 0x01, 0x0A, 0x1E, 0x0A, 0x56, 0x01, 0x0A,
  0x1E, 0x1E, 0x48, 0x01, 0x0A, 0x19, 0x14, 0x48, 0x01, 0x0F, 0x1E, 0x0A, 0x48, 0x01, 0x0F, 0x1E,
  0x1E, 0x2A, 0x48, 0x05, 0x01, 0x12, 0x13, 0x48, 0x05, 0x15, 0x26, 0x14, 0x2A, 0x56, 0x0E, 0x0A,
  0x1E, 0x05, 0x56, 0x0E, 0x0A, 0x1E, 0x0A, 0x56, 0x0E, 0x0A, 0x1E, 0x0F, 0x56, 0x0E, 0x0A, 0x1E,
  0x14, 0x56, 0x0E, 0x0A, 0x1E, 0x19, 0x56, 0x0E, 0x0A, 0x1E, 0x1E, 0x56, 0x0E, 0x0A, 0x1E, 0x23,
  0x2A, 0x56, 0x02, 0x0A, 0x1E, 0x0A, 0x48, 0x02, 0x0A, 0x19, 0x0A, 0x48, 0x02, 0x0A, 0x19, 0x14,
  0x48, 0x02, 0x0A, 0x19, 0x1E, 0x56, 0x04, 0x0A, 0x1E, 0x1E, 0x48, 0x04, 0x0F, 0x1E, 0x0F, 0x48,
  0x04, 0x0F, 0x1E, 0x19, 0x2A, 0x48, 0x0D, 0x0A, 0x1E, 0x0A, 0x48, 0x0D, 0x0A, 0x1E, 0x1E, 0x56,
  0x0D, 0x0A, 0x1E, 0x0A, 0x56, 0x0D, 0x0A, 0x1E, 0x1E, 0x56, 0x00, 0x13, 0x14, 0x0A, 0x56, 0x00,
  0x14, 0x15, 0x1E, 0x56, 0x0D, 0x0A, 0x1E, 0x14, 0x2A, 0x48, 0x0B, 0x0A, 0x1E, 0x0A, 0x48, 0x0B,
  0x0A, 0x1E, 0x1E, 0x56, 0x0B, 0x0A, 0x1E, 0x0A, 0x56, 0x0B, 0x0A, 0x1E, 0x1E, 0x48, 0x00, 0x0A,
  0x1E, 0x14, 0x56, 0x09, 0x12, 0x16, 0x14, 0x2A, 0x48, 0x07, 0x0D, 0x13, 0x07, 0x48, 0x07, 0x19,
  0x1F, 0x07, 0x48, 0x07, 0x07, 0x0D, 0x0D, 0x48, 0x07, 0x13, 0x19, 0x0D, 0x48, 0x07, 0x07, 0x0D,
  0x13, 0x48, 0x07, 0x19, 0x1F, 0x13, 0x48, 0x07, 0x13, 0x1F, 0x19, 0x48, 0x07, 0x0D, 0x19, 0x1F,
  0x56, 0x07, 0x08, 0x0D, 0x07, 0x56, 0x07, 0x07, 0x0D, 0x13, 0x56, 0x07, 0x07, 0x13, 0x1F, 0x56,
  0x07, 0x13, 0x19, 0x0D, 0x56, 0x07, 0x13, 0x19, 0x13, 0x56, 0x07, 0x19, 0x1F, 0x07, 0x56, 0x07,
  0x19, 0x1F, 0x1F, 0x2A, 0x56, 0x04, 0x05, 0x13, 0x13, 0x56, 0x04, 0x15, 0x22, 0x13, 0x56, 0x04,
  0x05, 0x13, 0x22, 0x56, 0x04, 0x15, 0x22, 0x22, 0x56, 0x04, 0x05, 0x13, 0x05, 0x56, 0x04, 0x15,
  0x22, 0x05, 0x48, 0x04, 0x05, 0x13, 0x13, 0x48, 0x04, 0x15, 0x22, 0x13, 0x48, 0x04, 0x05, 0x13,
  0x22, 0x48, 0x04, 0x15, 0x22, 0x22, 0x48, 0x04, 0x05, 0x13, 0x05, 0x48, 0x04, 0x15, 0x22, 0x05,
  0x48, 0x00, 0x11, 0x17, 0x0A, 0x48, 0x00, 0x11, 0x17, 0x1E, 0x56, 0x00, 0x11, 0x17, 0x0A, 0x56,
  0x00, 0x11, 0x17, 0x1E, 0x2A, 0x48, 0x07, 0x01, 0x12, 0x12, 0x56, 0x07, 0x01, 0x08, 0x12, 0x56,
  0x07, 0x0B, 0x12, 0x12, 0x48, 0x07, 0x15, 0x26, 0x15, 0x56, 0x07, 0x15, 0x1C, 0x15, 0x56, 0x07,
  0x1F, 0x26, 0x15, 0x2A, 0x48, 0x01, 0x01, 0x26, 0x13, 0x56, 0x01, 0x01, 0x26, 0x13, 0x48, 0x00,
  0x11, 0x15, 0x13, 0x56, 0x00, 0x11, 0x15, 0x13, 0x2A, 0x48, 0x0E, 0x01, 0x12, 0x12, 0x56, 0x0E,
  0x01, 0x12, 0x15, 0x48, 0x0E, 0x15, 0x26, 0x15, 0x56, 0x0E, 0x15, 0x26, 0x12, 0x2A, 0x48, 0x08,
  0x01, 0x08, 0x13, 0x48, 0x08, 0x0A, 0x1C, 0x13, 0x48, 0x08, 0x1E, 0x26, 0x13, 0x56, 0x08, 0x01,
  0x08, 0x13, 0x56, 0x08, 0x0A, 0x1C, 0x13, 0x56, 0x08, 0x1E, 0x26, 0x13, 0x2A, 0x48, 0x0B, 0x05,
  0x22, 0x05, 0x48, 0x0B, 0x05, 0x22, 0x22, 0x48, 0x02, 0x0A, 0x1D, 0x0A, 0x48, 0x02, 0x0A, 0x1D,
  0x1D, 0x56, 0x0B, 0x05, 0x22, 0x05, 0x56, 0x0B, 0x05, 0x22, 0x22, 0x56, 0x02, 0x0A, 0x1D, 0x0A,
  0x56, 0x02, 0x0A, 0x1D, 0x1D, 0x48, 0x00, 0x01, 0x26, 0x14, 0x56, 0x00, 0x01, 0x26, 0x14, 0x2A,
  0x48, 0x06, 0x06, 0x0A, 0x05, 0x48, 0x06, 0x0F, 0x19, 0x05, 0x48, 0x06, 0x1E, 0x23, 0x05, 0x48,
  0x06, 0x0F, 0x14, 0x0A, 0x48, 0x06, 0x0A, 0x0F, 0x0F, 0x48, 0x06, 0x1E, 0x23, 0x0F, 0x48, 0x06,
  0x0F, 0x1E, 0x14, 0x48, 0x06, 0x0A, 0x0F, 0x19, 0x48, 0x06, 0x14, 0x1E, 0x19, 0x48, 0x06, 0x14,
  0x1E, 0x1E, 0x48, 0x06, 0x0F, 0x19, 0x23, 0x48, 0x06, 0x1E, 0x23, 0x23, 0x56, 0x06, 0x0A, 0x14,
  0x05, 0x56, 0x06, 0x19, 0x23, 0x05, 0x56, 0x06, 0x05, 0x0A, 0x0A, 0x56, 0x06, 0x0F, 0x19, 0x0A,
  0x56, 0x06, 0x1E, 0x23, 0x0A, 0x56, 0x06, 0x1E, 0x23, 0x0F, 0x56, 0x06, 0x0A, 0x0F, 0x14, 0x56,
  0x06, 0x05, 0x0A, 0x19, 0x56, 0x06, 0x0F, 0x14, 0x19, 0x56, 0x06, 0x05, 0x0A, 0x1E, 0x56, 0x06,
  0x1E, 0x23, 0x1E, 0x56, 0x06, 0x0A, 0x14, 0x23, 0x56, 0x06, 0x19, 0x1E, 0x23, 0x2A, 0x48, 0x03,
  0x05, 0x12, 0x05, 0x48, 0x03, 0x15, 0x22, 0x05, 0x56, 0x03, 0x05, 0x22, 0x05, 0x56, 0x03, 0x05,
  0x22, 0x22, 0x48, 0x03, 0x05, 0x12, 0x22, 0x48, 0x03, 0x15, 0x22, 0x22, 0x48, 0x03, 0x0A, 0x1D,
  0x1D, 0x48, 0x03, 0x0A, 0x1D, 0x0A, 0x56, 0x03, 0x0A, 0x1D, 0x0A, 0x56, 0x03, 0x0A, 0x1D, 0x1D,
  0x56, 0x00, 0x13, 0x14, 0x1D, 0x2A, 0x48, 0x09, 0x0C, 0x1C, 0x0A, 0x48, 0x09, 0x0A, 0x1E, 0x14,
  0x48, 0x09, 0x0C, 0x1C, 0x1E, 0x56, 0x09, 0x0C, 0x1C, 0x0A, 0x56, 0x09, 0x0A, 0x1E, 0x14, 0x56,
  0x09, 0x0C, 0x1C, 0x1E, 0x48, 0x00, 0x13, 0x15, 0x14, 0x56, 0x00, 0x13, 0x15, 0x14, 0x2A, 0x56,
  0x0F, 0x01, 0x08, 0x12, 0x56, 0x0F, 0x0B, 0x26, 0x12, 0x56, 0x0F, 0x01, 0x1C, 0x15, 0x56, 0x0F,
  0x1F, 0x26, 0x15, 0x2A, 0x48, 0x01, 0x05, 0x22, 0x02, 0x48, 0x02, 0x05, 0x22, 0x04, 0x48, 0x03,
  0x05, 0x22, 0x06, 0x48, 0x04, 0x05, 0x22, 0x08, 0x48, 0x05, 0x05, 0x22, 0x0A, 0x48, 0x06, 0x05,
  0x22, 0x0C, 0x48, 0x07, 0x05, 0x22, 0x0E, 0x48, 0x08, 0x05, 0x22, 0x10, 0x48, 0x09, 0x05, 0x22,
  0x25, 0x48, 0x0A, 0x05, 0x22, 0x23, 0x48, 0x0B, 0x05, 0x22, 0x21, 0x48, 0x0C, 0x05, 0x22, 0x1F,
  0x48, 0x0D, 0x05, 0x22, 0x1D, 0x48, 0x0E, 0x05, 0x22, 0x1B, 0x48, 0x0F, 0x05, 0x22, 0x19, 0x48,
  0x08, 0x05, 0x22, 0x17, 0x56, 0x08, 0x10, 0x17, 0x13, 0x56, 0x08, 0x10, 0x17, 0x15, 0x56, 0x00,
  0x01, 0x26, 0x14, 0x2A, 0x48, 0x04, 0x03, 0x24, 0x06, 0x48, 0x02, 0x09, 0x24, 0x09, 0x48, 0x04,
  0x0C, 0x24, 0x0C, 0x48, 0x02, 0x0F, 0x24, 0x0F, 0x48, 0x04, 0x12, 0x24, 0x12, 0x48, 0x02, 0x15,
  0x24, 0x15, 0x48, 0x04, 0x18, 0x24, 0x18, 0x48, 0x02, 0x1B, 0x24, 0x1B, 0x48, 0x04, 0x1E, 0x24,
  0x1E, 0x48, 0x02, 0x21, 0x24, 0x21, 0x56, 0x02, 0x09, 0x21, 0x03, 0x56, 0x04, 0x06, 0x21, 0x06,
  0x56, 0x02, 0x09, 0x21, 0x09, 0x56, 0x04, 0x0C, 0x21, 0x0C, 0x56, 0x02, 0x0F, 0x21, 0x0F, 0x56,
  0x04, 0x12, 0x21, 0x12, 0x56, 0x02, 0x15, 0x21, 0x15, 0x56, 0x04, 0x18, 0x21, 0x18, 0x56, 0x02,
  0x1B, 0x21, 0x1B, 0x56, 0x04, 0x1E, 0x21, 0x1E, 0x2A, 0x48, 0x09, 0x02, 0x13, 0x13, 0x48, 0x09,
  0x02, 0x08, 0x0A, 0x48, 0x09, 0x0B, 0x13, 0x0A, 0x56, 0x09, 0x0A, 0x0D, 0x08, 0x56, 0x09, 0x0A,
  0x0D, 0x0B, 0x48, 0x02, 0x15, 0x25, 0x13, 0x48, 0x02, 0x15, 0x1C, 0x0A, 0x48, 0x02, 0x1F, 0x25,
  0x0A, 0x56, 0x02, 0x07, 0x0A, 0x1C, 0x56, 0x02, 0x07, 0x0A, 0x1F, 0x56, 0x09, 0x0A, 0x13, 0x02,
  0x56, 0x09, 0x0A, 0x13, 0x13, 0x56, 0x02, 0x0A, 0x13, 0x15, 0x56, 0x02, 0x0A, 0x13, 0x25, 0x48,
  0x05, 0x02, 0x13, 0x15, 0x48, 0x05, 0x02, 0x08, 0x1E, 0x48, 0x05, 0x0B, 0x13, 0x1E, 0x56, 0x05,
  0x1E, 0x21, 0x08, 0x56, 0x05, 0x1E, 0x21, 0x0B, 0x56, 0x05, 0x15, 0x1E, 0x02, 0x56, 0x05, 0x15,
  0x1E, 0x13, 0x48, 0x06, 0x15, 0x25, 0x15, 0x56, 0x06, 0x15, 0x1E, 0x15, 0x56, 0x06, 0x15, 0x1E,
  0x25, 0x48, 0x06, 0x15, 0x1C, 0x1E, 0x48, 0x06, 0x1F, 0x25, 0x1E, 0x56, 0x06, 0x1B, 0x1E, 0x1C,
  0x56, 0x06, 0x1B, 0x1E, 0x1F, 0x2A, 0x48, 0x06, 0x01, 0x26, 0x13, 0x48, 0x06, 0x01, 0x26, 0x09,
  0x48, 0x06, 0x01, 0x26, 0x1D, 0x56, 0x06, 0x01, 0x26, 0x13, 0x56, 0x06, 0x01, 0x26, 0x09, 0x56,
  0x06, 0x01, 0x26, 0x1D, 0x56, 0x00, 0x01, 0x26, 0x04, 0x56, 0x00, 0x01, 0x26, 0x0E, 0x56, 0x00,
  0x01, 0x26, 0x18, 0x56, 0x00, 0x01, 0x26, 0x22, 0x48, 0x00, 0x01, 0x26, 0x04, 0x48, 0x00, 0x01,
  0x26, 0x0E, 0x48, 0x00, 0x01, 0x26, 0x18, 0x48, 0x00, 0x01, 0x26, 0x22, 0x2A, 0x56, 0x06, 0x01,
  0x26, 0x13, 0x56, 0x06, 0x01, 0x26, 0x0F, 0x56, 0x06, 0x01, 0x26, 0x17, 0x48, 0x00, 0x0F, 0x17,
  0x12, 0x48, 0x00, 0x0F, 0x17, 0x16, 0x56, 0x06, 0x02, 0x25, 0x11, 0x56, 0x06, 0x02, 0x25, 0x15,
  0x48, 0x06, 0x0F, 0x17, 0x13, 0x48, 0x06, 0x0F, 0x17, 0x15, 0x48, 0x00, 0x0F, 0x17, 0x14, 0x2A,
  0x48, 0x02, 0x01, 0x26, 0x13, 0x48, 0x02, 0x01, 0x26, 0x09, 0x48, 0x02, 0x01, 0x26, 0x1D, 0x56,
  0x02, 0x01, 0x26, 0x13, 0x56, 0x02, 0x01, 0x26, 0x09, 0x56, 0x02, 0x01, 0x26, 0x1D, 0x56, 0x00,
  0x01, 0x26, 0x04, 0x56, 0x00, 0x01, 0x26, 0x22, 0x48, 0x00, 0x01, 0x26, 0x04, 0x48, 0x00, 0x01,
  0x26, 0x0E, 0x48, 0x00, 0x01, 0x26, 0x18, 0x48, 0x00, 0x01, 0x26, 0x22, 0x2A, 0x45, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
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
