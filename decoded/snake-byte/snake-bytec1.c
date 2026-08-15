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
void func_7267(uint16_t ret_addr);
void func_728d(uint16_t ret_addr);
void func_7633(uint16_t ret_addr);
void func_7642(uint16_t ret_addr);
void FUNC_MVBLKUP2(uint16_t ret_addr);
void FUNC_FNDLIN(uint16_t ret_addr);
void FUNC_FNDLIN2(uint16_t ret_addr);
void FUNC_CLRTXTPTR(uint16_t ret_addr);
void FUNC_RESTORE(uint16_t ret_addr);
void FUNC_NEGATE(uint16_t ret_addr);
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
void FUNC_MON_PLOT(uint16_t ret_addr);
void FUNC_PLOT1(uint16_t ret_addr);
void FUNC_HLINE(uint16_t ret_addr);
void FUNC_VLINE(uint16_t ret_addr);
void FUNC_CLRTOP(uint16_t ret_addr);
void FUNC_GBASCALC(uint16_t ret_addr);
void FUNC_SETCOL(uint16_t ret_addr);
void FUNC_SCRN(uint16_t ret_addr);
void FUNC_INIT(uint16_t ret_addr);
void FUNC_APPLEII(uint16_t ret_addr);
void FUNC_BASCALC(uint16_t ret_addr);
void FUNC_VTABZ(uint16_t ret_addr);
void FUNC_HOME(uint16_t ret_addr);
void FUNC_CLREOL(uint16_t ret_addr);
void FUNC_CLREOLZ(uint16_t ret_addr);
void FUNC_MON_WAIT(uint16_t ret_addr);
void FUNC_SETNORM(uint16_t ret_addr);
void FUNC_SETKBD(uint16_t ret_addr);
void FUNC_SETVID(uint16_t ret_addr);
void FUNC_SAV1(uint16_t ret_addr);

static void emulated_entry_point(void) {
  func_t001(false);
}

void FUNC_BCC(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$0090*/ CYCLES(0x0090, 0);
            fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x0090, 0x0090, 0x03); error_handler(0x0090);
}


void func_60e4(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$60E4*/ CYCLES(0x60e4, 6);
            func_6127(0x60e6);
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
  /*$60E7*/ CYCLES(0x60e7, 22);
            s_x = 0x00;
  /*$60E9*/ tmp1_U8 = ram_peek(0x0003);
  /*$60EB*/ ram_poke(0x0007, 0x00);
  /*$60F0*/ ram_poke(0x0004, ram_peek((0x6000 + tmp1_U8)));
  /*$60F5*/ ram_poke(0x0005, ram_peek((0x6030 + tmp1_U8)));
bb_1:
  /*$60F7*/ CYCLES(0x60f7, 16);
  /*$60F9*/ ram_poke(0x0006, ram_peek(0x0001));
  /*$60FE*/ ram_poke(0x0006, (uint8_t)((ram_peek(0x0006) << 0x01) | (ram_peek(0x0007) & 0x01)));
  /*$6100*/ CYCLES(0x6100, 62);
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
            if (!tmp1_U8)
              goto bb_6;
bb_5:
  /*$6124*/ CYCLES_EDGE(0x6124, 1);
            branchTarget = true;
            goto bb_1;
bb_6:
  /*$6126*/ CYCLES(0x6126, 6);
            if (ret_addr) pop16(); return;
}


void func_6127(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6127*/ CYCLES(0x6127, 53);
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

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6148*/ CYCLES(0x6148, 6);
            func_6127(0x614a);
            branchTarget = true;
bb_1:
  /*$614B*/ CYCLES(0x614b, 6);
            func_60e7(0x614d);
  /*$614E*/ CYCLES(0x614e, 8);
            tmp1_U8 = ram_peek(0x0002);
  /*$6150*/ tmp2_U8 = ram_peek(0x0008);
            s_status_c = (tmp1_U8 >= tmp2_U8);
            branchTarget = true;
            if ((tmp1_U8 != tmp2_U8))
              goto bb_3;
bb_2:
  /*$6152*/ CYCLES_EDGE(0x6152, 1);
  /*$6159*/ CYCLES(0x6159, 6);
            if (ret_addr) pop16(); return;
bb_3:
  /*$6154*/ CYCLES(0x6154, 8);
            ram_poke(0x0002, (uint8_t)(ram_peek(0x0002) + 0x01));
            branchTarget = true;
            goto bb_1;
}


void func_615a(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$615A*/ CYCLES(0x615a, 6);
            func_6127(0x615c);
            branchTarget = true;
bb_1:
  /*$615D*/ CYCLES(0x615d, 6);
            func_60e7(0x615f);
  /*$6160*/ CYCLES(0x6160, 8);
            branchTarget = true;
            if ((ram_peek(0x0003) != ram_peek(0x0008)))
              goto bb_3;
bb_2:
  /*$6164*/ CYCLES_EDGE(0x6164, 1);
  /*$6159*/ CYCLES(0x6159, 6);
            if (ret_addr) pop16(); return;
bb_3:
  /*$6166*/ CYCLES(0x6166, 8);
            ram_poke(0x0003, (uint8_t)(ram_peek(0x0003) + 0x01));
            branchTarget = true;
            goto bb_1;
}


void func_6217(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6217*/ CYCLES(0x6217, 10);
            s_x = ram_peek(0x624d);
  /*$621A*/ tmp1_U8 = io_peek(0xc000);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 & 0x80))
              goto bb_2;
            else
              goto bb_3;
bb_1:
  /*$6216*/ CYCLES(0x6216, 6);
            if (ret_addr) pop16(); return;
bb_2:
  /*$621D*/ CYCLES_EDGE(0x621d, 1);
            branchTarget = true;
            goto bb_1;
bb_3:
  /*$621F*/ CYCLES(0x621f, 21);
            tmp1_U8 = s_a;
            io_poke(0xc010, tmp1_U8);
  /*$6222*/ tmp2_U8 = s_x;
            ram_poke((0x623c + tmp2_U8), tmp1_U8);
  /*$6225*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
            s_x = tmp2_U8;
  /*$6227*/ tmp2_U8 = tmp2_U8 & 0x0f;
            s_a = tmp2_U8;
            branchTarget = true;
            if ((tmp2_U8 != ram_peek(0x624c)))
              goto bb_5;
bb_4:
  /*$622C*/ CYCLES_EDGE(0x622c, 1);
            branchTarget = true;
            goto bb_1;
bb_5:
  /*$622E*/ CYCLES(0x622e, 10);
            ram_poke(0x624d, s_a);
  /*$6231*/ if (ret_addr) pop16(); return;
}


void func_64a9(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$64A9*/ CYCLES(0x64a9, 2);
            s_x = 0x00;
bb_1:
  /*$64AB*/ CYCLES(0x64ab, 4);
  /*$64AC*/ s_y = s_x;
bb_2:
  /*$64AD*/ CYCLES(0x64ad, 4);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_4;
bb_3:
  /*$64AE*/ CYCLES_EDGE(0x64ae, 1);
            branchTarget = true;
            goto bb_2;
bb_4:
  /*$64B0*/ CYCLES(0x64b0, 12);
  /*$64B3*/ tmp1_U8 = peek((0xc000 + ram_peek(0x6c49)));
  /*$64B6*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_6;
bb_5:
  /*$64B7*/ CYCLES_EDGE(0x64b7, 1);
            branchTarget = true;
            goto bb_1;
bb_6:
  /*$64B9*/ CYCLES(0x64b9, 4);
  /*$64BA*/ s_y = s_x;
bb_7:
  /*$64BB*/ CYCLES(0x64bb, 4);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_9;
bb_8:
  /*$64BC*/ CYCLES_EDGE(0x64bc, 1);
            branchTarget = true;
            goto bb_7;
bb_9:
  /*$64BE*/ CYCLES(0x64be, 12);
            tmp1_U8 = ram_peek(0x6c49);
            s_y = tmp1_U8;
  /*$64C1*/ tmp1_U8 = peek((0xc000 + tmp1_U8));
            s_a = tmp1_U8;
  /*$64C4*/ tmp1_U8 = (uint8_t)(s_x + 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_x = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_11;
bb_10:
  /*$64C5*/ CYCLES_EDGE(0x64c5, 1);
            branchTarget = true;
            goto bb_6;
bb_11:
  /*$64C7*/ CYCLES(0x64c7, 6);
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
  /*$64C8*/ CYCLES(0x64c8, 12);
  /*$64CA*/ ram_poke(0x6c4a, 0x00);
  /*$64CD*/ tmp1_U8 = ram_peek(0x6634);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_2;
bb_1:
  /*$64D0*/ CYCLES_EDGE(0x64d0, 1);
  /*$64D3*/ CYCLES(0x64d3, 36);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_4;
            else
              goto bb_3;
bb_2:
  /*$64D2*/ CYCLES(0x64d2, 6);
            if (ret_addr) pop16(); return;
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
  /*$64E8*/ FUNC_SCRN(0x64ea);
  /*$64EB*/ CYCLES(0x64eb, 4);
            branchTarget = true;
            if ((s_a != 0x00))
              goto bb_10;
bb_9:
  /*$64ED*/ CYCLES_EDGE(0x64ed, 1);
            branchTarget = true;
            goto bb_19;
bb_10:
  /*$64EF*/ CYCLES(0x64ef, 14);
            s_y = ram_peek(0x6637);
  /*$64F2*/ s_a = ram_peek(0x6634);
  /*$64F5*/ FUNC_SCRN(0x64f7);
  /*$64F8*/ CYCLES(0x64f8, 4);
            branchTarget = true;
            if ((s_a != 0x00))
              goto bb_12;
bb_11:
  /*$64FA*/ CYCLES_EDGE(0x64fa, 1);
            branchTarget = true;
            goto bb_13;
bb_12:
  /*$64FC*/ CYCLES(0x64fc, 24);
  /*$64FF*/ ram_poke(0x6637, ram_peek(0x6633));
  /*$6507*/ ram_poke(0x6635, (ram_peek(0x6635) ^ 0xfe));
  /*$650A*/ ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
bb_13:
  /*$650D*/ CYCLES(0x650d, 14);
            s_a = ram_peek(0x6638);
  /*$6510*/ s_y = ram_peek(0x6633);
  /*$6513*/ FUNC_SCRN(0x6515);
  /*$6516*/ CYCLES(0x6516, 4);
            branchTarget = true;
            if ((s_a != 0x00))
              goto bb_15;
bb_14:
  /*$6518*/ CYCLES_EDGE(0x6518, 1);
            branchTarget = true;
            goto bb_16;
bb_15:
  /*$651A*/ CYCLES(0x651a, 24);
  /*$651D*/ ram_poke(0x6638, ram_peek(0x6634));
  /*$6525*/ ram_poke(0x6636, (ram_peek(0x6636) ^ 0xfe));
  /*$6528*/ ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
bb_16:
  /*$652B*/ CYCLES(0x652b, 6);
            branchTarget = true;
            if (!ram_peek(0x6c4a))
              goto bb_18;
bb_17:
  /*$652E*/ CYCLES_EDGE(0x652e, 1);
            branchTarget = true;
            goto bb_19;
bb_18:
  /*$6530*/ CYCLES(0x6530, 36);
  /*$6533*/ ram_poke(0x6637, ram_peek(0x6633));
  /*$6539*/ ram_poke(0x6638, ram_peek(0x6634));
  /*$6541*/ ram_poke(0x6635, (ram_peek(0x6635) ^ 0xfe));
  /*$6549*/ ram_poke(0x6636, (ram_peek(0x6636) ^ 0xfe));
bb_19:
  /*$654C*/ CYCLES(0x654c, 11);
            s_a = 0x00;
  /*$654E*/ ram_poke(0x0001, 0x00);
  /*$6550*/ FUNC_SETCOL(0x6552);
  /*$6553*/ CYCLES(0x6553, 20);
  /*$6556*/ ram_poke(0x0002, ram_peek(0x6633));
  /*$655B*/ ram_poke(0x0003, ram_peek(0x6634));
  /*$655D*/ func_60e4(0x655f);
  /*$6560*/ CYCLES(0x6560, 14);
            s_a = ram_peek(0x6634);
  /*$6563*/ s_y = ram_peek(0x6633);
  /*$6566*/ FUNC_MON_PLOT(0x6568);
  /*$6569*/ CYCLES(0x6569, 11);
  /*$656B*/ ram_poke(0x0000, 0x1a);
  /*$656D*/ tmp1_U8 = ram_peek(0x6638);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_21;
bb_20:
  /*$6570*/ CYCLES_EDGE(0x6570, 1);
  /*$6573*/ CYCLES(0x6573, 29);
            tmp1_U8 = s_a;
            ram_poke(0x0003, tmp1_U8);
  /*$6575*/ ram_poke(0x6634, tmp1_U8);
  /*$6578*/ tmp1_U8 = ram_peek(0x6637);
  /*$657B*/ ram_poke(0x0002, tmp1_U8);
  /*$657D*/ ram_poke(0x6633, tmp1_U8);
  /*$6580*/ s_a = 0x03;
  /*$6582*/ ram_poke(0x0001, 0x03);
  /*$6584*/ FUNC_SETCOL(0x6586);
  /*$6587*/ CYCLES(0x6587, 6);
            func_60e4(0x6589);
  /*$658A*/ CYCLES(0x658a, 14);
            s_a = ram_peek(0x6634);
  /*$658D*/ s_y = ram_peek(0x6633);
  /*$6590*/ FUNC_MON_PLOT(0x6592);
  /*$6593*/ CYCLES(0x6593, 6);
            if (ret_addr) pop16(); return;
bb_21:
  /*$6572*/ CYCLES(0x6572, 6);
            if (ret_addr) pop16(); return;
}


void func_6594(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6594*/ CYCLES(0x6594, 6);
            branchTarget = true;
            if (ram_peek(0x0301))
              goto bb_5;
            else
              goto bb_6;
bb_1:
  /*$6200*/ CYCLES(0x6200, 10);
            tmp1_U8 = ram_peek(0x624c);
            s_x = tmp1_U8;
  /*$6203*/ tmp1_U8 = ram_peek((0x623c + tmp1_U8));
            s_a = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_3;
bb_2:
  /*$6206*/ CYCLES_EDGE(0x6206, 1);
            branchTarget = true;
            goto bb_4;
bb_3:
  /*$6208*/ CYCLES(0x6208, 24);
            push8(s_a);
  /*$620B*/ tmp1_U8 = s_x;
            ram_poke((0x623c + tmp1_U8), 0x00);
  /*$620E*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
            s_x = tmp1_U8;
  /*$6212*/ ram_poke(0x624c, (tmp1_U8 & 0x0f));
  /*$6215*/ tmp1_U8 = pop8();
            s_a = tmp1_U8;
bb_4:
  /*$6216*/ CYCLES(0x6216, 6);
            if (ret_addr) pop16(); return;
bb_5:
  /*$6597*/ CYCLES_EDGE(0x6597, 1);
  /*$659C*/ CYCLES(0x659c, 38);
  /*$659F*/ ram_poke(0x6633, ram_peek(0x6639));
  /*$65A5*/ ram_poke(0x6634, ram_peek(0x663a));
  /*$65AB*/ ram_poke(0x6635, ram_peek(0x663d));
  /*$65B1*/ ram_poke(0x6636, ram_peek(0x663e));
  /*$65B4*/ func_64c8(0x65b6);
  /*$65B7*/ CYCLES(0x65b7, 40);
  /*$65BA*/ ram_poke(0x6639, ram_peek(0x6633));
  /*$65C0*/ ram_poke(0x663a, ram_peek(0x6634));
  /*$65C6*/ ram_poke(0x663d, ram_peek(0x6635));
  /*$65CC*/ ram_poke(0x663e, ram_peek(0x6636));
  /*$65CF*/ tmp1_U8 = ram_peek(0x0301);
  /*$65D2*/ s_status_c = (tmp1_U8 >= 0x01);
            branchTarget = true;
            if ((tmp1_U8 != 0x01))
              goto bb_7;
            else
              goto bb_8;
bb_6:
  /*$6599*/ CYCLES(0x6599, 3);
            branchTarget = true;
            goto bb_1;
bb_7:
  /*$65D4*/ CYCLES_EDGE(0x65d4, 1);
  /*$65D9*/ CYCLES(0x65d9, 38);
  /*$65DC*/ ram_poke(0x6633, ram_peek(0x663b));
  /*$65E2*/ ram_poke(0x6634, ram_peek(0x663c));
  /*$65E8*/ ram_poke(0x6635, ram_peek(0x663f));
  /*$65EE*/ ram_poke(0x6636, ram_peek(0x6640));
  /*$65F1*/ func_64c8(0x65f3);
  /*$65F4*/ CYCLES(0x65f4, 35);
  /*$65F7*/ ram_poke(0x663b, ram_peek(0x6633));
  /*$65FD*/ ram_poke(0x663c, ram_peek(0x6634));
  /*$6603*/ ram_poke(0x663f, ram_peek(0x6635));
  /*$6609*/ ram_poke(0x6640, ram_peek(0x6636));
            branchTarget = true;
            goto bb_1;
bb_8:
  /*$65D6*/ CYCLES(0x65d6, 3);
            branchTarget = true;
            goto bb_1;
}


void func_660f(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$660F*/ CYCLES(0x660f, 50);
            ram_poke(0x624f, s_a);
  /*$6614*/ ram_poke(0x6639, 0x01);
  /*$6617*/ ram_poke(0x663a, 0x01);
  /*$661A*/ ram_poke(0x663d, 0x01);
  /*$661D*/ ram_poke(0x663e, 0x01);
  /*$6620*/ ram_poke(0x6640, 0x01);
  /*$6623*/ ram_poke(0x663c, 0x01);
  /*$6628*/ ram_poke(0x663b, 0x26);
  /*$662D*/ ram_poke(0x663f, 0xff);
  /*$6630*/ s_a = 0x14;
  /*$6632*/ if (ret_addr) pop16(); return;
}


void func_6641(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6641*/ CYCLES(0x6641, 16);
  /*$6643*/ ram_poke(0x0036, 0x4a);
  /*$6647*/ ram_poke(0x0037, 0x66);
  /*$6649*/ if (ret_addr) pop16(); return;
}


void func_69a9(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$69A9*/ CYCLES(0x69a9, 4);
            branchTarget = true;
            if (!(s_a != 0x9b))
              goto bb_2;
bb_1:
  /*$69AB*/ CYCLES_EDGE(0x69ab, 1);
            branchTarget = true;
            goto bb_5;
bb_2:
  /*$69AD*/ CYCLES(0x69ad, 6);
            tmp1_U8 = io_peek(0xc000);
            s_a = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_4;
bb_3:
  /*$69B0*/ CYCLES_EDGE(0x69b0, 1);
            branchTarget = true;
            goto bb_2;
bb_4:
  /*$69B2*/ CYCLES(0x69b2, 4);
            io_poke(0xc010, s_a);
bb_5:
  /*$69B5*/ CYCLES(0x69b5, 4);
            tmp1_U8 = s_a;
            s_status_c = (tmp1_U8 >= 0x93);
            branchTarget = true;
            if (!(tmp1_U8 != 0x93))
              goto bb_7;
bb_6:
  /*$69B7*/ CYCLES_EDGE(0x69b7, 1);
            branchTarget = true;
            goto bb_8;
bb_7:
  /*$69B9*/ CYCLES(0x69b9, 10);
  /*$69BE*/ ram_poke(0x69c2, (ram_peek(0x69c2) ^ 0x01));
bb_8:
  /*$69C1*/ CYCLES(0x69c1, 6);
            if (ret_addr) pop16(); return;
}


void func_69c3(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$69C3*/ CYCLES(0x69c3, 14);
  /*$69C6*/ ram_poke(0x6b39, ram_peek(0x624f));
  /*$69CB*/ ram_poke(0x6b3a, 0x01);
bb_1:
  /*$69CE*/ CYCLES(0x69ce, 14);
            s_a = ram_peek(0x6b3a);
  /*$69D1*/ s_y = ram_peek(0x6b39);
  /*$69D4*/ FUNC_SCRN(0x69d6);
  /*$69D7*/ CYCLES(0x69d7, 4);
            branchTarget = true;
            if ((s_a != 0x0f))
              goto bb_3;
bb_2:
  /*$69D9*/ CYCLES_EDGE(0x69d9, 1);
            branchTarget = true;
            goto bb_15;
bb_3:
  /*$69DB*/ CYCLES(0x69db, 14);
            ram_poke(0x6b3a, (uint8_t)(ram_peek(0x6b3a) + 0x01));
            branchTarget = true;
            if (!(ram_peek(0x6b3a) != 0x27))
              goto bb_5;
bb_4:
  /*$69E3*/ CYCLES_EDGE(0x69e3, 1);
            branchTarget = true;
            goto bb_1;
bb_5:
  /*$69E5*/ CYCLES(0x69e5, 14);
  /*$69E7*/ ram_poke(0x6b3a, 0x01);
  /*$69EA*/ tmp1_U8 = (uint8_t)(ram_peek(0x6b39) - 0x01);
            ram_poke(0x6b39, tmp1_U8);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_7;
bb_6:
  /*$69ED*/ CYCLES_EDGE(0x69ed, 1);
            branchTarget = true;
            goto bb_1;
bb_7:
  /*$69EF*/ CYCLES(0x69ef, 8);
  /*$69F2*/ ram_poke(0x6b39, ram_peek(0x624f));
bb_8:
  /*$69F5*/ CYCLES(0x69f5, 14);
            s_a = ram_peek(0x6b3a);
  /*$69F8*/ s_y = ram_peek(0x6b39);
  /*$69FB*/ FUNC_SCRN(0x69fd);
  /*$69FE*/ CYCLES(0x69fe, 2);
  /*$6A00*/ CYCLES(0x6a00, 2);
            branchTarget = true;
            if ((s_a != 0x0f))
              goto bb_10;
bb_9:
  /*$6A00*/ CYCLES_EDGE(0x6a00, 1);
            branchTarget = true;
            goto bb_15;
bb_10:
  /*$6A02*/ CYCLES(0x6a02, 14);
            ram_poke(0x6b3a, (uint8_t)(ram_peek(0x6b3a) + 0x01));
            branchTarget = true;
            if (!(ram_peek(0x6b3a) != 0x27))
              goto bb_12;
bb_11:
  /*$6A0A*/ CYCLES_EDGE(0x6a0a, 1);
            branchTarget = true;
            goto bb_8;
bb_12:
  /*$6A0C*/ CYCLES(0x6a0c, 20);
  /*$6A0E*/ ram_poke(0x6b3a, 0x01);
  /*$6A11*/ ram_poke(0x6b39, (uint8_t)(ram_peek(0x6b39) + 0x01));
            branchTarget = true;
            if (!(ram_peek(0x6b39) != 0x27))
              goto bb_14;
bb_13:
  /*$6A19*/ CYCLES_EDGE(0x6a19, 1);
            branchTarget = true;
            goto bb_8;
bb_14:
  /*$6A1B*/ CYCLES(0x6a1b, 12);
  /*$6A1D*/ ram_poke(0x6b3a, 0x00);
  /*$6A22*/ ram_poke(0x6b39, 0x14);
bb_15:
  /*$6A25*/ CYCLES(0x6a25, 22);
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

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6AB8*/ CYCLES(0x6ab8, 42);
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
  /*$6AD2*/ FUNC_SCRN(0x6ad4);
  /*$6AD5*/ CYCLES(0x6ad5, 4);
            tmp1_U8 = s_a != 0x00;
            s_status_not_z = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_8;
bb_7:
  /*$6AD7*/ CYCLES_EDGE(0x6ad7, 1);
            branchTarget = true;
            goto bb_9;
bb_8:
  /*$6AD9*/ CYCLES(0x6ad9, 2);
            s_status_not_z = (s_a != 0x0f);
bb_9:
  /*$6ADB*/ CYCLES(0x6adb, 2);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_11;
bb_10:
  /*$6ADB*/ CYCLES_EDGE(0x6adb, 1);
  /*$6ADE*/ CYCLES(0x6ade, 12);
  /*$6AE0*/ ram_poke(0x6c4a, 0x00);
  /*$6AE3*/ tmp1_U8 = ram_peek(0x6638);
            s_status_not_z = tmp1_U8;
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_12;
            else
              goto bb_13;
bb_11:
  /*$6ADD*/ CYCLES(0x6add, 6);
            if (ret_addr) pop16(); return;
bb_12:
  /*$6AE6*/ CYCLES_EDGE(0x6ae6, 1);
            branchTarget = true;
            goto bb_11;
bb_13:
  /*$6AE8*/ CYCLES(0x6ae8, 12);
  /*$6AEB*/ s_y = (uint8_t)(ram_peek(0x6637) + 0x01);
  /*$6AEC*/ FUNC_SCRN(0x6aee);
  /*$6AEF*/ CYCLES(0x6aef, 4);
            branchTarget = true;
            if (!(s_a != 0x00))
              goto bb_15;
bb_14:
  /*$6AF1*/ CYCLES_EDGE(0x6af1, 1);
            branchTarget = true;
            goto bb_16;
bb_15:
  /*$6AF3*/ CYCLES(0x6af3, 6);
            ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
bb_16:
  /*$6AF6*/ CYCLES(0x6af6, 16);
            s_a = ram_peek(0x6638);
  /*$6AFC*/ s_y = (uint8_t)(ram_peek(0x6637) - 0x01);
  /*$6AFD*/ FUNC_SCRN(0x6aff);
  /*$6B00*/ CYCLES(0x6b00, 4);
            branchTarget = true;
            if (!(s_a != 0x00))
              goto bb_18;
bb_17:
  /*$6B02*/ CYCLES_EDGE(0x6b02, 1);
            branchTarget = true;
            goto bb_19;
bb_18:
  /*$6B04*/ CYCLES(0x6b04, 6);
            ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
bb_19:
  /*$6B07*/ CYCLES(0x6b07, 18);
            s_a = ram_peek(0x6638);
  /*$6B0A*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_21;
bb_20:
  /*$6B0B*/ tmp4_U16 = s_a;
            tmp3_U16 = (tmp4_U16 + 0x0001) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0x0001);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_22;
bb_21:
  /*$6B0B*/ tmp4_U16 = adc_dec16(s_a, 0x01, s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
bb_22:
  /*$6B0D*/ s_y = ram_peek(0x6637);
  /*$6B10*/ FUNC_SCRN(0x6b12);
  /*$6B13*/ CYCLES(0x6b13, 4);
            branchTarget = true;
            if (!(s_a != 0x00))
              goto bb_24;
bb_23:
  /*$6B15*/ CYCLES_EDGE(0x6b15, 1);
            branchTarget = true;
            goto bb_25;
bb_24:
  /*$6B17*/ CYCLES(0x6b17, 6);
            ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
bb_25:
  /*$6B1A*/ CYCLES(0x6b1a, 18);
            s_a = ram_peek(0x6638);
  /*$6B1D*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_27;
bb_26:
  /*$6B1E*/ tmp3_U16 = s_a;
            tmp4_U16 = (tmp3_U16 - 0x0001) - (uint8_t)(0x01 - s_status_c);
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0xfffe);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_28;
bb_27:
  /*$6B1E*/ tmp4_U16 = sbc_dec16(s_a, 0x01, s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
bb_28:
  /*$6B20*/ s_y = ram_peek(0x6637);
  /*$6B23*/ FUNC_SCRN(0x6b25);
  /*$6B26*/ CYCLES(0x6b26, 4);
            branchTarget = true;
            if (!(s_a != 0x00))
              goto bb_30;
bb_29:
  /*$6B28*/ CYCLES_EDGE(0x6b28, 1);
            branchTarget = true;
            goto bb_31;
bb_30:
  /*$6B2A*/ CYCLES(0x6b2a, 6);
            ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
bb_31:
  /*$6B2D*/ CYCLES(0x6b2d, 6);
            branchTarget = true;
            if (ram_peek(0x6c4a))
              goto bb_33;
bb_32:
  /*$6B30*/ CYCLES_EDGE(0x6b30, 1);
  /*$6B35*/ CYCLES(0x6b35, 8);
            s_status_not_z = 0x01;
  /*$6B37*/ if (ret_addr) pop16(); return;
bb_33:
  /*$6B32*/ CYCLES(0x6b32, 8);
            s_status_not_z = 0x00;
  /*$6B34*/ if (ret_addr) pop16(); return;
}


void func_6b3d(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6B3D*/ CYCLES(0x6b3d, 6);
            func_6c4b(0x6b3f);
  /*$6B40*/ CYCLES(0x6b40, 26);
  /*$6B42*/ ram_poke(0x0000, 0x15);
  /*$6B46*/ ram_poke(0x0001, 0x02);
  /*$6B4A*/ ram_poke(0x0002, 0x00);
  /*$6B4E*/ ram_poke(0x0003, 0x01);
  /*$6B50*/ tmp1_U8 = ram_peek(0x6255);
            s_a = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_2;
bb_1:
  /*$6B53*/ CYCLES_EDGE(0x6b53, 1);
            branchTarget = true;
            goto bb_3;
bb_2:
  /*$6B55*/ CYCLES(0x6b55, 8);
  /*$6B57*/ ram_poke(0x6255, 0xff);
  /*$6B5A*/ s_a = 0x70;
bb_3:
  /*$6B5C*/ CYCLES(0x6b5c, 18);
  /*$6B5E*/ ram_poke(0x0008, (s_a >> 0x02));
  /*$6B60*/ ram_poke(0x0008, (uint8_t)(ram_peek(0x0008) + 0x01));
  /*$6B62*/ func_615a(0x6b64);
  /*$6B65*/ CYCLES(0x6b65, 16);
  /*$6B67*/ ram_poke(0x0002, 0x27);
  /*$6B6B*/ ram_poke(0x0003, 0x01);
  /*$6B6D*/ func_615a(0x6b6f);
  /*$6B70*/ CYCLES(0x6b70, 30);
            ram_poke(0x0008, (uint8_t)(ram_peek(0x0008) + 0x01));
  /*$6B72*/ tmp1_U8 = ram_peek(0x0008);
  /*$6B74*/ push8(tmp1_U8);
  /*$6B75*/ ram_poke(0x0003, tmp1_U8);
  /*$6B79*/ ram_poke(0x0001, 0x0d);
  /*$6B7D*/ ram_poke(0x0008, 0x27);
  /*$6B7F*/ func_615a(0x6b81);
  /*$6B82*/ CYCLES(0x6b82, 18);
            tmp1_U8 = pop8();
  /*$6B83*/ ram_poke(0x0003, tmp1_U8);
  /*$6B87*/ ram_poke(0x0002, 0x00);
  /*$6B89*/ func_615a(0x6b8b);
  /*$6B8C*/ CYCLES(0x6b8c, 7);
            s_a = 0x27;
  /*$6B8E*/ s_y = 0x14;
  /*$6B90*/ FUNC_SCRN(0x0000);
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
            func_6127(0x6b95);
  /*$6B96*/ CYCLES(0x6b96, 22);
            s_x = 0x00;
  /*$6B98*/ tmp1_U8 = ram_peek(0x0003);
  /*$6B9A*/ ram_poke(0x0007, 0x00);
  /*$6B9F*/ ram_poke(0x0004, ram_peek((0x6000 + tmp1_U8)));
  /*$6BA4*/ ram_poke(0x0005, ram_peek((0x6030 + tmp1_U8)));
bb_1:
  /*$6BA6*/ CYCLES(0x6ba6, 85);
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
            if (!(tmp2_U8 != 0x04))
              goto bb_6;
bb_5:
  /*$6BD7*/ CYCLES_EDGE(0x6bd7, 1);
            branchTarget = true;
            goto bb_1;
bb_6:
  /*$6BD9*/ CYCLES(0x6bd9, 6);
            if (ret_addr) pop16(); return;
}


void func_6bda(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6BDA*/ CYCLES(0x6bda, 6);
            func_60e4(0x6bdc);
  /*$6BDD*/ CYCLES(0x6bdd, 6);
            branchTarget = true;
            if (ram_peek(0x0305))
              goto bb_2;
bb_1:
  /*$6BE0*/ CYCLES_EDGE(0x6be0, 1);
            branchTarget = true;
            goto bb_3;
bb_2:
  /*$6BE2*/ CYCLES(0x6be2, 11);
  /*$6BE4*/ ram_poke(0x0000, 0x01);
  /*$6BE6*/ func_6b93(0x6be8);
            branchTarget = true;
bb_3:
  /*$6BE9*/ CYCLES(0x6be9, 12);
  /*$6BEB*/ ram_poke(0x0305, 0x00);
  /*$6BEE*/ if (ret_addr) pop16(); return;
}


void func_6bef(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6BEF*/ CYCLES(0x6bef, 6);
            FUNC_MON_PLOT(0x6bf1);
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
  /*$6BFB*/ CYCLES(0x6bfb, 6);
  /*$6BFD*/ ram_poke(0x6c48, 0x14);
bb_1:
  /*$6C00*/ CYCLES(0x6c00, 6);
            tmp1_U8 = ram_peek(0x6c46);
            s_a = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_3;
bb_2:
  /*$6C03*/ CYCLES_EDGE(0x6c03, 1);
            branchTarget = true;
            goto bb_8;
bb_3:
  /*$6C05*/ CYCLES(0x6c05, 4);
            branchTarget = true;
            if (!(s_a >= 0x80))
              goto bb_5;
bb_4:
  /*$6C07*/ CYCLES_EDGE(0x6c07, 1);
            branchTarget = true;
            goto bb_8;
bb_5:
  /*$6C09*/ CYCLES(0x6c09, 8);
            tmp1_U8 = (uint8_t)(ram_peek(0x6c47) - 0x01);
            ram_poke(0x6c47, tmp1_U8);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_7;
bb_6:
  /*$6C0C*/ CYCLES_EDGE(0x6c0c, 1);
            branchTarget = true;
            goto bb_8;
bb_7:
  /*$6C0E*/ CYCLES(0x6c0e, 28);
            tmp1_U8 = ram_peek(0x6c49);
            s_y = tmp1_U8;
  /*$6C11*/ tmp1_U8 = peek((0xc000 + tmp1_U8));
  /*$6C14*/ ram_poke(0x6c46, (uint8_t)(ram_peek(0x6c46) + 0x01));
  /*$6C17*/ ram_poke(0x6c46, (uint8_t)(ram_peek(0x6c46) + 0x01));
  /*$6C1D*/ ram_poke(0x6c47, ram_peek(0x6c46));
bb_8:
  /*$6C20*/ CYCLES(0x6c20, 8);
            branchTarget = true;
            if ((ram_peek(0x6c46) >= 0x80))
              goto bb_10;
bb_9:
  /*$6C25*/ CYCLES_EDGE(0x6c25, 1);
            branchTarget = true;
            goto bb_11;
bb_10:
  /*$6C27*/ CYCLES(0x6c27, 6);
  /*$6C29*/ ram_poke(0x6c46, 0x00);
bb_11:
  /*$6C2C*/ CYCLES(0x6c2c, 12);
  /*$6C2E*/ ram_poke(0x6c49, 0x20);
            branchTarget = true;
            if (!ram_peek(0x0302))
              goto bb_13;
bb_12:
  /*$6C34*/ CYCLES_EDGE(0x6c34, 1);
            branchTarget = true;
            goto bb_16;
bb_13:
  /*$6C36*/ CYCLES(0x6c36, 6);
            branchTarget = true;
            if (!ram_peek(0x69c2))
              goto bb_15;
bb_14:
  /*$6C39*/ CYCLES_EDGE(0x6c39, 1);
            branchTarget = true;
            goto bb_16;
bb_15:
  /*$6C3B*/ CYCLES(0x6c3b, 6);
  /*$6C3D*/ ram_poke(0x6c49, 0x30);
bb_16:
  /*$6C40*/ CYCLES(0x6c40, 8);
            tmp1_U8 = (uint8_t)(ram_peek(0x6c48) - 0x01);
            ram_poke(0x6c48, tmp1_U8);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_18;
bb_17:
  /*$6C43*/ CYCLES_EDGE(0x6c43, 1);
            branchTarget = true;
            goto bb_1;
bb_18:
  /*$6C45*/ CYCLES(0x6c45, 6);
            if (ret_addr) pop16(); return;
}


void func_6c4b(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6C4B*/ CYCLES(0x6c4b, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x000e) + 0x01);
            ram_poke(0x000e, tmp1_U8);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_2;
bb_1:
  /*$6C4D*/ CYCLES_EDGE(0x6c4d, 1);
            branchTarget = true;
            goto bb_3;
bb_2:
  /*$6C4F*/ CYCLES(0x6c4f, 5);
            ram_poke(0x000f, (uint8_t)(ram_peek(0x000f) + 0x01));
bb_3:
  /*$6C51*/ CYCLES(0x6c51, 9);
  /*$6C53*/ tmp1_U8 = peek(ram_peek16al(0x000e));
            s_a = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_5;
bb_4:
  /*$6C55*/ CYCLES_EDGE(0x6c55, 1);
  /*$6C62*/ CYCLES(0x6c62, 6);
            if (ret_addr) pop16(); return;
bb_5:
  /*$6C57*/ CYCLES(0x6c57, 13);
  /*$6C59*/ ram_poke(0x000e, 0x00);
  /*$6C5D*/ ram_poke(0x000f, 0x18);
            branchTarget = true;
            goto bb_3;
}


void func_6c72(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$6C72*/ CYCLES(0x6c72, 6);
            func_6594(0x6c74);
  /*$6C75*/ CYCLES(0x6c75, 9);
            push8(s_a);
            branchTarget = true;
            if (ram_peek(0x0302))
              goto bb_2;
bb_1:
  /*$6C79*/ CYCLES_EDGE(0x6c79, 1);
  /*$6C90*/ CYCLES(0x6c90, 6);
            tmp2_U8 = pop8();
            s_a = tmp2_U8;
  /*$6C91*/ s_x = 0x05;
            goto bb_10;
bb_2:
  /*$6C7B*/ CYCLES(0x6c7b, 6);
            branchTarget = true;
            if (ram_peek(0x6c71))
              goto bb_4;
bb_3:
  /*$6C7E*/ CYCLES_EDGE(0x6c7e, 1);
            branchTarget = true;
            goto bb_7;
bb_4:
  /*$6C80*/ CYCLES(0x6c80, 6);
            tmp1_U8 = io_peek(0xc061);
            branchTarget = true;
            if (!(tmp1_U8 & 0x80))
              goto bb_6;
bb_5:
  /*$6C83*/ CYCLES_EDGE(0x6c83, 1);
            branchTarget = true;
            goto bb_7;
bb_6:
  /*$6C85*/ CYCLES(0x6c85, 12);
            tmp1_U8 = pop8();
  /*$6C86*/ s_status_n = 0x80;
            s_a = 0x92;
  /*$6C88*/ if (ret_addr) pop16(); return;
bb_7:
  /*$6C89*/ CYCLES(0x6c89, 6);
            tmp1_U8 = pop8();
            tmp2_U8 = tmp1_U8 & 0x80;
            s_status_n = tmp2_U8;
            s_a = tmp1_U8;
            branchTarget = true;
            if (tmp2_U8)
              goto bb_9;
bb_8:
  /*$6C8A*/ CYCLES_EDGE(0x6c8a, 1);
  /*$6C8F*/ CYCLES(0x6c8f, 6);
            if (ret_addr) pop16(); return;
bb_9:
  /*$6C8C*/ CYCLES(0x6c8c, 8);
            s_status_n = 0x80;
            s_a = 0x92;
  /*$6C8E*/ if (ret_addr) pop16(); return;
bb_10:
  /*$6C93*/ CYCLES(0x6c93, 6);
            branchTarget = true;
            if ((s_a != ram_peek((0x6c63 + s_x))))
              goto bb_12;
bb_11:
  /*$6C96*/ CYCLES_EDGE(0x6c96, 1);
  /*$6C9E*/ CYCLES(0x6c9e, 4);
            s_a = ram_peek((0x6c6a + s_x));
            goto bb_15;
bb_12:
  /*$6C98*/ CYCLES(0x6c98, 4);
            tmp2_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp2_U8;
            branchTarget = true;
            if ((tmp2_U8 & 0x80))
              goto bb_14;
bb_13:
  /*$6C99*/ CYCLES_EDGE(0x6c99, 1);
            branchTarget = true;
            goto bb_10;
bb_14:
  /*$6C9B*/ CYCLES(0x6c9b, 3);
            branchTarget = true;
bb_15:
  /*$6CA1*/ CYCLES(0x6ca1, 4);
            branchTarget = true;
            if (!(s_a != 0x80))
              goto bb_17;
bb_16:
  /*$6CA3*/ CYCLES_EDGE(0x6ca3, 1);
  /*$6CAB*/ CYCLES(0x6cab, 4);
            branchTarget = true;
            if ((s_a != 0x8b))
              goto bb_18;
            else
              goto bb_19;
bb_17:
  /*$6CA5*/ CYCLES(0x6ca5, 12);
            s_status_n = 0x00;
            s_a = 0x01;
  /*$6CA7*/ ram_poke(0x6c71, 0x01);
  /*$6CAA*/ if (ret_addr) pop16(); return;
bb_18:
  /*$6CAD*/ CYCLES_EDGE(0x6cad, 1);
  /*$6CB5*/ CYCLES(0x6cb5, 4);
            tmp2_U8 = s_a & 0x80;
            s_status_n = tmp2_U8;
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_20;
            else
              goto bb_21;
bb_19:
  /*$6CAF*/ CYCLES(0x6caf, 12);
            s_status_n = 0x00;
            s_a = 0x00;
  /*$6CB1*/ ram_poke(0x6c71, 0x00);
  /*$6CB4*/ if (ret_addr) pop16(); return;
bb_20:
  /*$6CB7*/ CYCLES_EDGE(0x6cb7, 1);
  /*$6CBA*/ CYCLES(0x6cba, 6);
            tmp2_U8 = ram_peek(0x6c71);
            s_x = tmp2_U8;
            branchTarget = true;
            if (tmp2_U8)
              goto bb_22;
            else
              goto bb_23;
bb_21:
  /*$6CB9*/ CYCLES(0x6cb9, 6);
            if (ret_addr) pop16(); return;
bb_22:
  /*$6CBD*/ CYCLES_EDGE(0x6cbd, 1);
  /*$6CC2*/ CYCLES(0x6cc2, 12);
            s_y = 0x00;
  /*$6CC4*/ tmp2_U8 = io_peek(0xc05b);
  /*$6CC7*/ tmp2_U8 = io_peek(0xc062);
            branchTarget = true;
            if ((tmp2_U8 & 0x80))
              goto bb_24;
            else
              goto bb_25;
bb_23:
  /*$6CBF*/ CYCLES(0x6cbf, 8);
            s_status_n = (s_a & 0x80);
  /*$6CC1*/ if (ret_addr) pop16(); return;
bb_24:
  /*$6CCA*/ CYCLES_EDGE(0x6cca, 1);
            branchTarget = true;
            goto bb_26;
bb_25:
  /*$6CCC*/ CYCLES(0x6ccc, 4);
            s_x = 0x00;
  /*$6CCE*/ s_y = (uint8_t)(s_y + 0x01);
bb_26:
  /*$6CCF*/ CYCLES(0x6ccf, 6);
            tmp2_U8 = io_peek(0xc063);
            branchTarget = true;
            if (!(tmp2_U8 & 0x80))
              goto bb_28;
bb_27:
  /*$6CD2*/ CYCLES_EDGE(0x6cd2, 1);
            branchTarget = true;
            goto bb_29;
bb_28:
  /*$6CD4*/ CYCLES(0x6cd4, 4);
            s_x = 0x03;
  /*$6CD6*/ s_y = (uint8_t)(s_y + 0x01);
bb_29:
  /*$6CD7*/ CYCLES(0x6cd7, 10);
            tmp2_U8 = io_peek(0xc05a);
  /*$6CDA*/ tmp2_U8 = io_peek(0xc062);
            branchTarget = true;
            if (!(tmp2_U8 & 0x80))
              goto bb_31;
bb_30:
  /*$6CDD*/ CYCLES_EDGE(0x6cdd, 1);
            branchTarget = true;
            goto bb_32;
bb_31:
  /*$6CDF*/ CYCLES(0x6cdf, 4);
            s_x = 0x01;
  /*$6CE1*/ s_y = (uint8_t)(s_y + 0x01);
bb_32:
  /*$6CE2*/ CYCLES(0x6ce2, 6);
            tmp2_U8 = io_peek(0xc063);
            branchTarget = true;
            if (!(tmp2_U8 & 0x80))
              goto bb_34;
bb_33:
  /*$6CE5*/ CYCLES_EDGE(0x6ce5, 1);
            branchTarget = true;
            goto bb_35;
bb_34:
  /*$6CE7*/ CYCLES(0x6ce7, 4);
            s_x = 0x02;
  /*$6CE9*/ s_y = (uint8_t)(s_y + 0x01);
bb_35:
  /*$6CEA*/ CYCLES(0x6cea, 4);
            branchTarget = true;
            if ((s_y != 0x01))
              goto bb_37;
bb_36:
  /*$6CEC*/ CYCLES_EDGE(0x6cec, 1);
  /*$6CF1*/ CYCLES(0x6cf1, 10);
            tmp2_U8 = ram_peek((0x6c6a + s_x));
            s_status_n = (tmp2_U8 & 0x80);
            s_a = tmp2_U8;
  /*$6CF4*/ if (ret_addr) pop16(); return;
bb_37:
  /*$6CEE*/ CYCLES(0x6cee, 8);
            s_status_n = 0x00;
            s_a = 0x00;
  /*$6CF0*/ if (ret_addr) pop16(); return;
}


void func_7000(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$7000*/ CYCLES(0x7000, 6);
  /*$7002*/ push8(ram_peek(0x0003));
bb_1:
  /*$7003*/ CYCLES(0x7003, 12);
            s_a = ram_peek(0x0003);
  /*$7005*/ s_y = ram_peek(0x0002);
  /*$7007*/ FUNC_MON_PLOT(0x7009);
  /*$700A*/ CYCLES(0x700a, 8);
            branchTarget = true;
            if ((ram_peek(0x0003) != ram_peek(0x0008)))
              goto bb_3;
bb_2:
  /*$700E*/ CYCLES_EDGE(0x700e, 1);
  /*$7015*/ CYCLES(0x7015, 13);
            tmp1_U8 = pop8();
  /*$7016*/ ram_poke(0x0003, tmp1_U8);
  /*$7018*/ if (ret_addr) pop16(); return;
bb_3:
  /*$7010*/ CYCLES(0x7010, 8);
            ram_poke(0x0003, (uint8_t)(ram_peek(0x0003) + 0x01));
            branchTarget = true;
            goto bb_1;
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
            ram_poke(0x000a, tmp1_U8);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_2;
bb_1:
  /*$701F*/ CYCLES_EDGE(0x701f, 1);
            branchTarget = true;
            goto bb_3;
bb_2:
  /*$7021*/ CYCLES(0x7021, 5);
            ram_poke(0x000b, (uint8_t)(ram_peek(0x000b) + 0x01));
bb_3:
  /*$7023*/ CYCLES(0x7023, 6);
            if (ret_addr) pop16(); return;
}


void func_7024(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$7024*/ CYCLES(0x7024, 2);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_2;
bb_1:
  /*$7024*/ CYCLES_EDGE(0x7024, 1);
            branchTarget = true;
            goto bb_3;
bb_2:
  /*$7026*/ CYCLES(0x7026, 2);
            s_a = 0x05;
bb_3:
  /*$7028*/ CYCLES(0x7028, 3);
            FUNC_SETCOL(0x0000);
            if (ret_addr) pop16(); return;
}


void func_702b(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$702B*/ CYCLES(0x702b, 12);
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
            if (!tmp1_U8)
              goto bb_3;
bb_2:
  /*$703A*/ CYCLES_EDGE(0x703a, 1);
            branchTarget = true;
            goto bb_1;
bb_3:
  /*$703C*/ CYCLES(0x703c, 12);
            ram_poke(0x0005, (uint8_t)(ram_peek(0x0005) + 0x01));
  /*$703E*/ tmp1_U8 = ram_peek(0x0005);
  /*$7040*/ s_status_c = (tmp1_U8 >= 0x40);
            branchTarget = true;
            if (!(tmp1_U8 != 0x40))
              goto bb_5;
bb_4:
  /*$7042*/ CYCLES_EDGE(0x7042, 1);
            branchTarget = true;
            goto bb_1;
bb_5:
  /*$7044*/ CYCLES(0x7044, 6);
            if (ret_addr) pop16(); return;
}


void func_7045(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$7045*/ CYCLES(0x7045, 6);
            func_702b(0x7047);
  /*$7048*/ CYCLES(0x7048, 32);
  /*$704A*/ ram_poke(0x6c46, 0x00);
  /*$704D*/ tmp1_U8 = io_peek(0xc050);
  /*$7050*/ tmp1_U8 = io_peek(0xc057);
  /*$7053*/ tmp1_U8 = io_peek(0xc055);
  /*$7056*/ tmp1_U8 = io_peek(0xc052);
  /*$705B*/ ram_poke(0x0002, 0x04);
  /*$705F*/ ram_poke(0x0003, 0x00);
bb_1:
  /*$7061*/ CYCLES(0x7061, 4);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_3;
bb_2:
  /*$7062*/ CYCLES_EDGE(0x7062, 1);
            branchTarget = true;
            goto bb_1;
bb_3:
  /*$7064*/ CYCLES(0x7064, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x0003) - 0x01);
            ram_poke(0x0003, tmp1_U8);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_5;
bb_4:
  /*$7066*/ CYCLES_EDGE(0x7066, 1);
            branchTarget = true;
            goto bb_1;
bb_5:
  /*$7068*/ CYCLES(0x7068, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x0002) - 0x01);
            ram_poke(0x0002, tmp1_U8);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_7;
bb_6:
  /*$706A*/ CYCLES_EDGE(0x706a, 1);
            branchTarget = true;
            goto bb_1;
bb_7:
  /*$706C*/ CYCLES(0x706c, 13);
  /*$706E*/ ram_poke(0x0003, 0x27);
  /*$7070*/ s_status_not_z = 0x00;
            s_a = 0x00;
  /*$7072*/ func_7024(0x7074);
            branchTarget = true;
bb_8:
  /*$7075*/ CYCLES(0x7075, 16);
  /*$7077*/ ram_poke(0x002c, 0x27);
  /*$7079*/ s_y = 0x00;
  /*$707B*/ s_a = ram_peek(0x0003);
  /*$707D*/ FUNC_HLINE(0x707f);
  /*$7080*/ CYCLES(0x7080, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x0003) - 0x01);
            ram_poke(0x0003, tmp1_U8);
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_10;
bb_9:
  /*$7082*/ CYCLES_EDGE(0x7082, 1);
            branchTarget = true;
            goto bb_8;
bb_10:
  /*$7084*/ CYCLES(0x7084, 21);
  /*$7086*/ ram_poke(0x0022, 0x14);
  /*$708A*/ ram_poke(0x0000, 0x15);
  /*$708C*/ s_status_not_z = 0x0d;
            s_a = 0x0d;
  /*$708E*/ ram_poke(0x0001, 0x0d);
  /*$7090*/ func_7024(0x7092);
  /*$7093*/ CYCLES(0x7093, 6);
            branchTarget = true;
            if (ram_peek(0x0301))
              goto bb_12;
bb_11:
  /*$7096*/ CYCLES_EDGE(0x7096, 1);
            branchTarget = true;
            goto bb_15;
bb_12:
  /*$7098*/ CYCLES(0x7098, 10);
            s_a = 0x01;
  /*$709A*/ s_y = 0x01;
  /*$709B*/ FUNC_MON_PLOT(0x709d);
  /*$709E*/ CYCLES(0x709e, 8);
            tmp1_U8 = ram_peek(0x0301);
  /*$70A1*/ s_status_c = (tmp1_U8 >= 0x01);
            branchTarget = true;
            if ((tmp1_U8 != 0x01))
              goto bb_14;
bb_13:
  /*$70A3*/ CYCLES_EDGE(0x70a3, 1);
            branchTarget = true;
            goto bb_15;
bb_14:
  /*$70A5*/ CYCLES(0x70a5, 10);
            s_a = 0x01;
  /*$70A7*/ s_y = 0x26;
  /*$70A9*/ FUNC_MON_PLOT(0x70ab);
            branchTarget = true;
bb_15:
  /*$70AC*/ CYCLES(0x70ac, 10);
            s_y = 0x00;
  /*$70AE*/ s_a = 0x00;
  /*$70B0*/ FUNC_HLINE(0x70b2);
  /*$70B3*/ CYCLES(0x70b3, 10);
            s_y = 0x00;
  /*$70B5*/ s_a = 0x27;
  /*$70B7*/ FUNC_HLINE(0x70b9);
  /*$70BA*/ CYCLES(0x70ba, 21);
  /*$70BC*/ ram_poke(0x0002, 0x00);
  /*$70C0*/ ram_poke(0x0003, 0x00);
  /*$70C4*/ ram_poke(0x0008, 0x27);
  /*$70C6*/ func_7000(0x70c8);
  /*$70C9*/ CYCLES(0x70c9, 16);
  /*$70CB*/ ram_poke(0x0003, 0x00);
  /*$70CF*/ ram_poke(0x0002, 0x27);
  /*$70D1*/ func_7000(0x70d3);
  /*$70D4*/ CYCLES(0x70d4, 19);
  /*$70D6*/ ram_poke(0x0002, 0x00);
  /*$70D8*/ ram_poke(0x0003, 0x00);
  /*$70DC*/ ram_poke(0x0008, 0x27);
  /*$70DE*/ func_6148(0x70e0);
  /*$70E1*/ CYCLES(0x70e1, 16);
  /*$70E3*/ ram_poke(0x0002, 0x00);
  /*$70E7*/ ram_poke(0x0003, 0x27);
  /*$70E9*/ func_6148(0x70eb);
  /*$70EC*/ CYCLES(0x70ec, 14);
  /*$70EE*/ ram_poke(0x0002, 0x00);
  /*$70F0*/ ram_poke(0x0003, 0x00);
  /*$70F2*/ func_615a(0x70f4);
  /*$70F5*/ CYCLES(0x70f5, 16);
  /*$70F7*/ ram_poke(0x0003, 0x00);
  /*$70FB*/ ram_poke(0x0002, 0x27);
  /*$70FD*/ func_615a(0x70ff);
  /*$7100*/ CYCLES(0x7100, 26);
  /*$7102*/ ram_poke(0x0001, 0x03);
  /*$7106*/ ram_poke(0x0003, 0x27);
  /*$710A*/ ram_poke(0x0002, 0x12);
  /*$710E*/ ram_poke(0x0008, 0x16);
  /*$7110*/ func_6148(0x7112);
            branchTarget = true;
bb_16:
  /*$7113*/ CYCLES(0x7113, 14);
            s_x = ram_peek(0x0303);
  /*$7118*/ ram_poke(0x000a, 0x00);
  /*$711C*/ ram_poke(0x000b, 0x80);
bb_17:
  /*$711E*/ CYCLES(0x711e, 4);
            tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_19;
bb_18:
  /*$711F*/ CYCLES_EDGE(0x711f, 1);
            branchTarget = true;
            goto bb_22;
bb_19:
  /*$7121*/ CYCLES(0x7121, 6);
            func_7019(0x7123);
  /*$7124*/ CYCLES(0x7124, 4);
            branchTarget = true;
            if ((s_a != 0x2a))
              goto bb_21;
bb_20:
  /*$7126*/ CYCLES_EDGE(0x7126, 1);
            branchTarget = true;
            goto bb_17;
bb_21:
  /*$7128*/ CYCLES(0x7128, 3);
            branchTarget = true;
            goto bb_19;
bb_22:
  /*$712B*/ CYCLES(0x712b, 6);
            func_7019(0x712d);
  /*$712E*/ CYCLES(0x712e, 4);
            branchTarget = true;
            if (!(s_a != 0x45))
              goto bb_24;
bb_23:
  /*$7130*/ CYCLES_EDGE(0x7130, 1);
  /*$713A*/ CYCLES(0x713a, 4);
            branchTarget = true;
            if ((s_a != 0x48))
              goto bb_25;
            else
              goto bb_26;
bb_24:
  /*$7132*/ CYCLES(0x7132, 9);
  /*$7134*/ ram_poke(0x0303, 0x01);
            branchTarget = true;
            goto bb_16;
bb_25:
  /*$713C*/ CYCLES_EDGE(0x713c, 1);
  /*$7168*/ CYCLES(0x7168, 4);
            branchTarget = true;
            if ((s_a != 0x56))
              goto bb_27;
            else
              goto bb_28;
bb_26:
  /*$713E*/ CYCLES(0x713e, 6);
            func_7019(0x7140);
  /*$7141*/ CYCLES(0x7141, 9);
            ram_poke(0x0001, s_a);
  /*$7143*/ func_7019(0x7145);
  /*$7146*/ CYCLES(0x7146, 9);
            ram_poke(0x0002, s_a);
  /*$7148*/ func_7019(0x714a);
  /*$714B*/ CYCLES(0x714b, 9);
            ram_poke(0x0008, s_a);
  /*$714D*/ func_7019(0x714f);
  /*$7150*/ CYCLES(0x7150, 12);
            ram_poke(0x0003, s_a);
  /*$7152*/ tmp1_U8 = ram_peek(0x0001);
            s_status_not_z = tmp1_U8;
            s_a = tmp1_U8;
  /*$7154*/ func_7024(0x7156);
  /*$7157*/ CYCLES(0x7157, 18);
  /*$7159*/ ram_poke(0x002c, ram_peek(0x0008));
  /*$715B*/ s_a = ram_peek(0x0003);
  /*$715D*/ s_y = ram_peek(0x0002);
  /*$715F*/ FUNC_HLINE(0x7161);
  /*$7162*/ CYCLES(0x7162, 6);
            func_6148(0x7164);
  /*$7165*/ CYCLES(0x7165, 3);
            branchTarget = true;
            goto bb_22;
bb_27:
  /*$716A*/ CYCLES_EDGE(0x716a, 1);
  /*$718E*/ CYCLES(0x718e, 4);
            branchTarget = true;
            if ((s_a != 0x50))
              goto bb_29;
            else
              goto bb_30;
bb_28:
  /*$716C*/ CYCLES(0x716c, 6);
            func_7019(0x716e);
  /*$716F*/ CYCLES(0x716f, 9);
            ram_poke(0x0001, s_a);
  /*$7171*/ func_7019(0x7173);
  /*$7174*/ CYCLES(0x7174, 9);
            ram_poke(0x0003, s_a);
  /*$7176*/ func_7019(0x7178);
  /*$7179*/ CYCLES(0x7179, 9);
            ram_poke(0x0008, s_a);
  /*$717B*/ func_7019(0x717d);
  /*$717E*/ CYCLES(0x717e, 12);
            ram_poke(0x0002, s_a);
  /*$7180*/ tmp1_U8 = ram_peek(0x0001);
            s_status_not_z = tmp1_U8;
            s_a = tmp1_U8;
  /*$7182*/ func_7024(0x7184);
  /*$7185*/ CYCLES(0x7185, 6);
            func_7000(0x7187);
  /*$7188*/ CYCLES(0x7188, 6);
            func_615a(0x718a);
  /*$718B*/ CYCLES(0x718b, 3);
            branchTarget = true;
            goto bb_22;
bb_29:
  /*$7190*/ CYCLES_EDGE(0x7190, 1);
  /*$71B3*/ CYCLES(0x71b3, 4);
            branchTarget = true;
            if ((s_a != 0x54))
              goto bb_31;
            else
              goto bb_32;
bb_30:
  /*$7192*/ CYCLES(0x7192, 6);
            func_7019(0x7194);
  /*$7195*/ CYCLES(0x7195, 9);
            ram_poke(0x0001, s_a);
  /*$7197*/ func_7019(0x7199);
  /*$719A*/ CYCLES(0x719a, 9);
            ram_poke(0x0002, s_a);
  /*$719C*/ func_7019(0x719e);
  /*$719F*/ CYCLES(0x719f, 12);
            ram_poke(0x0003, s_a);
  /*$71A1*/ tmp1_U8 = ram_peek(0x0001);
            s_status_not_z = tmp1_U8;
            s_a = tmp1_U8;
  /*$71A3*/ func_7024(0x71a5);
  /*$71A6*/ CYCLES(0x71a6, 12);
            s_a = ram_peek(0x0003);
  /*$71A8*/ s_y = ram_peek(0x0002);
  /*$71AA*/ FUNC_MON_PLOT(0x71ac);
  /*$71AD*/ CYCLES(0x71ad, 6);
            func_60e4(0x71af);
  /*$71B0*/ CYCLES(0x71b0, 3);
            branchTarget = true;
            goto bb_22;
bb_31:
  /*$71B5*/ CYCLES_EDGE(0x71b5, 1);
  /*$71C0*/ CYCLES(0x71c0, 4);
            branchTarget = true;
            if (!(s_a != 0x2a))
              goto bb_33;
            else
              goto bb_34;
bb_32:
  /*$71B7*/ CYCLES(0x71b7, 6);
            func_7019(0x71b9);
  /*$71BA*/ CYCLES(0x71ba, 7);
            ram_poke(0x0304, s_a);
            branchTarget = true;
            goto bb_22;
bb_33:
  /*$71C2*/ CYCLES_EDGE(0x71c2, 1);
  /*$71C7*/ CYCLES(0x71c7, 6);
            if (ret_addr) pop16(); return;
bb_34:
  /*$71C4*/ CYCLES(0x71c4, 3);
            branchTarget = true;
            goto bb_22;
}


void func_71cd(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint16_t tmp2_U16;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$71CD*/ CYCLES(0x71cd, 20);
  /*$71CF*/ ram_poke(0x71cb, 0x00);
  /*$71D2*/ ram_poke(0x71cc, 0x00);
  /*$71D5*/ s_x = ram_peek(0x0301);
  /*$71D8*/ s_y = ram_peek(0x0303);
  /*$71DB*/ s_status_d = 0x01;
bb_1:
  /*$71DC*/ CYCLES(0x71dc, 28);
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
            s_status_v = ovf8((uint8_t)tmp2_U16, (uint8_t)tmp1_U16, (uint8_t)0x0000);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_7;
bb_6:
  /*$71E9*/ tmp2_U16 = adc_dec16(s_a, 0x00, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            s_status_v = (((uint8_t)(tmp2_U16 >> 8) & 0x40) != 0);
bb_7:
  /*$71EB*/ ram_poke(0x71cc, s_a);
  /*$71EE*/ tmp3_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp3_U8;
            branchTarget = true;
            if (!tmp3_U8)
              goto bb_9;
bb_8:
  /*$71EF*/ CYCLES_EDGE(0x71ef, 1);
            branchTarget = true;
            goto bb_1;
bb_9:
  /*$71F1*/ CYCLES(0x71f1, 8);
            s_status_d = 0x00;
  /*$71F2*/ if (ret_addr) pop16(); return;
}


void func_7267(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint16_t tmp2_U16;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$7267*/ CYCLES(0x7267, 56);
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
            s_a = ((uint8_t)tmp2_U16);
            goto bb_9;
bb_8:
  /*$7286*/ tmp2_U16 = adc_dec16(s_a, 0x00, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            tmp3_U8 = (uint8_t)(tmp2_U16 >> 8);
            s_status_c = (tmp3_U8 & 0x01);
            s_status_v = ((tmp3_U8 & 0x40) != 0);
bb_9:
  /*$7288*/ ram_poke(0x7255, s_a);
  /*$728B*/ s_status_d = 0x00;
  /*$728C*/ if (ret_addr) pop16(); return;
}


void func_728d(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$728D*/ CYCLES(0x728d, 10);
            tmp1_U8 = ram_peek(0x7255);
  /*$7290*/ tmp2_U8 = ram_peek(0x7259);
            s_status_not_z = (tmp1_U8 != tmp2_U8);
            tmp2_U8 = tmp1_U8 >= tmp2_U8;
            s_status_c = tmp2_U8;
            branchTarget = true;
            if (tmp2_U8)
              goto bb_2;
bb_1:
  /*$7293*/ CYCLES_EDGE(0x7293, 1);
            branchTarget = true;
            goto bb_17;
bb_2:
  /*$7295*/ CYCLES(0x7295, 2);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_4;
bb_3:
  /*$7295*/ CYCLES_EDGE(0x7295, 1);
            branchTarget = true;
            goto bb_16;
bb_4:
  /*$7297*/ CYCLES(0x7297, 10);
            tmp2_U8 = ram_peek(0x7254);
  /*$729A*/ tmp1_U8 = ram_peek(0x7258);
            s_status_not_z = (tmp2_U8 != tmp1_U8);
            tmp1_U8 = tmp2_U8 >= tmp1_U8;
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_6;
bb_5:
  /*$729D*/ CYCLES_EDGE(0x729d, 1);
            branchTarget = true;
            goto bb_17;
bb_6:
  /*$729F*/ CYCLES(0x729f, 2);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_8;
bb_7:
  /*$729F*/ CYCLES_EDGE(0x729f, 1);
            branchTarget = true;
            goto bb_16;
bb_8:
  /*$72A1*/ CYCLES(0x72a1, 10);
            tmp1_U8 = ram_peek(0x7253);
  /*$72A4*/ tmp2_U8 = ram_peek(0x7257);
            s_status_not_z = (tmp1_U8 != tmp2_U8);
            tmp2_U8 = tmp1_U8 >= tmp2_U8;
            s_status_c = tmp2_U8;
            branchTarget = true;
            if (tmp2_U8)
              goto bb_10;
bb_9:
  /*$72A7*/ CYCLES_EDGE(0x72a7, 1);
            branchTarget = true;
            goto bb_17;
bb_10:
  /*$72A9*/ CYCLES(0x72a9, 2);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_12;
bb_11:
  /*$72A9*/ CYCLES_EDGE(0x72a9, 1);
            branchTarget = true;
            goto bb_16;
bb_12:
  /*$72AB*/ CYCLES(0x72ab, 10);
            tmp2_U8 = ram_peek(0x7252);
  /*$72AE*/ tmp1_U8 = ram_peek(0x7256);
            s_status_not_z = (tmp2_U8 != tmp1_U8);
            tmp1_U8 = tmp2_U8 >= tmp1_U8;
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_14;
bb_13:
  /*$72B1*/ CYCLES_EDGE(0x72b1, 1);
            branchTarget = true;
            goto bb_17;
bb_14:
  /*$72B3*/ CYCLES(0x72b3, 2);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_16;
bb_15:
  /*$72B3*/ CYCLES_EDGE(0x72b3, 1);
            branchTarget = true;
bb_16:
  /*$72B5*/ CYCLES(0x72b5, 32);
  /*$72B8*/ ram_poke(0x7256, ram_peek(0x7252));
  /*$72BE*/ ram_poke(0x7257, ram_peek(0x7253));
  /*$72C4*/ ram_poke(0x7258, ram_peek(0x7254));
  /*$72CA*/ ram_poke(0x7259, ram_peek(0x7255));
bb_17:
  /*$72CD*/ CYCLES(0x72cd, 6);
            if (ret_addr) pop16(); return;
}


void func_7633(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$7633*/ CYCLES(0x7633, 22);
  /*$7638*/ tmp1_U16 = adc_dec16(ram_peek(0x725e), 0x01, 0x00);
            tmp2_U8 = (uint8_t)(tmp1_U16 >> 8);
            s_status_c = (tmp2_U8 & 0x01);
            s_status_v = ((tmp2_U8 & 0x40) != 0);
  /*$763A*/ ram_poke(0x725e, ((uint8_t)tmp1_U16));
  /*$763D*/ s_status_d = 0x00;
  /*$763E*/ func_64a9(0x7640);
  /*$7641*/ CYCLES(0x7641, 6);
            if (ret_addr) pop16(); return;
}


void func_7642(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint16_t tmp2_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$7642*/ CYCLES(0x7642, 6);
            func_6c4b(0x7644);
  /*$7645*/ CYCLES(0x7645, 9);
            ram_poke(0x0002, s_a);
  /*$7647*/ func_6c4b(0x7649);
  /*$764A*/ CYCLES(0x764a, 15);
            ram_poke(0x0003, s_a);
  /*$764C*/ s_a = ram_peek(0x0003);
  /*$764E*/ s_y = ram_peek(0x0002);
  /*$7650*/ FUNC_SCRN(0x7652);
  /*$7653*/ CYCLES(0x7653, 2);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_2;
bb_1:
  /*$7653*/ CYCLES_EDGE(0x7653, 1);
            branchTarget = true;
            goto bb_0;
bb_2:
  /*$7655*/ CYCLES(0x7655, 8);
            s_a = 0x0f;
  /*$7657*/ FUNC_SETCOL(0x7659);
  /*$765A*/ CYCLES(0x765a, 12);
            s_a = ram_peek(0x0003);
  /*$765C*/ s_y = ram_peek(0x0002);
  /*$765E*/ FUNC_MON_PLOT(0x7660);
  /*$7661*/ CYCLES(0x7661, 16);
  /*$7663*/ ram_poke(0x0000, 0x01);
  /*$7667*/ ram_poke(0x0001, 0x09);
  /*$7669*/ func_60e4(0x766b);
  /*$766C*/ CYCLES(0x766c, 32);
  /*$7671*/ tmp1_U16 = adc_dec16(ram_peek(0x725f), 0x01, 0x00);
            s_status_c = ((uint8_t)(tmp1_U16 >> 8) & 0x01);
  /*$7673*/ ram_poke(0x725f, ((uint8_t)tmp1_U16));
  /*$7676*/ s_a = ram_peek(0x7260);
            if (0x01)
              goto bb_4;
bb_3:
  /*$7679*/ tmp1_U16 = s_a;
            tmp2_U16 = tmp1_U16 + s_status_c;
            s_status_v = ovf8((uint8_t)tmp2_U16, (uint8_t)tmp1_U16, (uint8_t)0x0000);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_5;
bb_4:
  /*$7679*/ tmp2_U16 = adc_dec16(s_a, 0x00, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            s_status_v = (((uint8_t)(tmp2_U16 >> 8) & 0x40) != 0);
bb_5:
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
  /*$D39A*/ CYCLES(0xd39a, 27);
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
            if (tmp2_U8)
              goto bb_8;
bb_7:
  /*$D3A9*/ CYCLES_EDGE(0xd3a9, 1);
            branchTarget = true;
            goto bb_25;
bb_8:
  /*$D3AB*/ CYCLES(0xd3ab, 13);
            s_a = ram_peek(0x0096);
  /*$D3AD*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_10;
bb_9:
  /*$D3AE*/ tmp4_U16 = (s_a - ram_peek(0x005e)) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
            s_a = ((uint8_t)tmp4_U16);
            goto bb_11;
bb_10:
  /*$D3AE*/ tmp1_U16 = sbc_dec16(s_a, ram_peek(0x005e), s_status_c);
            s_a = ((uint8_t)tmp1_U16);
            s_status_c = ((uint8_t)(tmp1_U16 >> 8) & 0x01);
bb_11:
  /*$D3B0*/ ram_poke(0x0096, s_a);
            branchTarget = true;
            if (!s_status_c)
              goto bb_13;
bb_12:
  /*$D3B2*/ CYCLES_EDGE(0xd3b2, 1);
            branchTarget = true;
            goto bb_14;
bb_13:
  /*$D3B4*/ CYCLES(0xd3b4, 7);
            ram_poke(0x0097, (uint8_t)(ram_peek(0x0097) - 0x01));
  /*$D3B6*/ s_status_c = 0x01;
bb_14:
  /*$D3B7*/ CYCLES(0xd3b7, 11);
            s_a = ram_peek(0x0094);
            if (s_status_d)
              goto bb_16;
bb_15:
  /*$D3B9*/ tmp4_U16 = s_a;
            tmp3_U16 = ram_peek(0x005e);
            tmp1_U16 = (tmp4_U16 - tmp3_U16) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp1_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp1_U16, (uint8_t)tmp4_U16, (uint8_t)(~tmp3_U16));
            s_a = ((uint8_t)tmp1_U16);
            goto bb_17;
bb_16:
  /*$D3B9*/ tmp1_U16 = sbc_dec16(s_a, ram_peek(0x005e), s_status_c);
            s_a = ((uint8_t)tmp1_U16);
            tmp2_U8 = (uint8_t)(tmp1_U16 >> 8);
            s_status_c = (tmp2_U8 & 0x01);
            s_status_v = ((tmp2_U8 & 0x40) != 0);
bb_17:
  /*$D3BB*/ ram_poke(0x0094, s_a);
            branchTarget = true;
            if (!s_status_c)
              goto bb_19;
bb_18:
  /*$D3BD*/ CYCLES_EDGE(0xd3bd, 1);
            branchTarget = true;
            goto bb_22;
bb_19:
  /*$D3BF*/ CYCLES(0xd3bf, 7);
            ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) - 0x01));
            branchTarget = true;
            if (s_status_c)
              goto bb_21;
bb_20:
  /*$D3C1*/ CYCLES_EDGE(0xd3c1, 1);
            branchTarget = true;
            goto bb_22;
bb_21:
  /*$D3C3*/ CYCLES(0xd3c3, 11);
            tmp2_U8 = s_y;
            tmp5_U8 = peek((ram_peek16al(0x0096) + tmp2_U8));
  /*$D3C5*/ poke((ram_peek16al(0x0094) + tmp2_U8), tmp5_U8);
bb_22:
  /*$D3C7*/ CYCLES(0xd3c7, 4);
            tmp2_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp2_U8;
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_24;
bb_23:
  /*$D3C8*/ CYCLES_EDGE(0xd3c8, 1);
            branchTarget = true;
            goto bb_21;
bb_24:
  /*$D3CA*/ CYCLES(0xd3ca, 11);
            tmp2_U8 = s_y;
            tmp5_U8 = peek((ram_peek16al(0x0096) + tmp2_U8));
            s_a = tmp5_U8;
  /*$D3CC*/ poke((ram_peek16al(0x0094) + tmp2_U8), tmp5_U8);
bb_25:
  /*$D3CE*/ CYCLES(0xd3ce, 14);
            ram_poke(0x0097, (uint8_t)(ram_peek(0x0097) - 0x01));
  /*$D3D0*/ ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) - 0x01));
  /*$D3D2*/ tmp2_U8 = (uint8_t)(s_x - 0x01);
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_x = tmp2_U8;
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_27;
bb_26:
  /*$D3D3*/ CYCLES_EDGE(0xd3d3, 1);
            branchTarget = true;
            goto bb_22;
bb_27:
  /*$D3D5*/ CYCLES(0xd3d5, 6);
            if (ret_addr) pop16(); return;
}


void FUNC_FNDLIN(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$D61A*/ CYCLES(0xd61a, 6);
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
  /*$D61E*/ CYCLES(0xd61e, 15);
            s_y = 0x01;
  /*$D620*/ ram_poke(0x009b, s_a);
  /*$D622*/ ram_poke(0x009c, s_x);
  /*$D624*/ tmp1_U8 = peek((ram_peek16(0x009b) + 0x0001));
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_2;
bb_1:
  /*$D626*/ CYCLES_EDGE(0xd626, 1);
            branchTarget = true;
            goto bb_14;
bb_2:
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
            if (tmp3_U8)
              goto bb_4;
bb_3:
  /*$D62E*/ CYCLES_EDGE(0xd62e, 1);
            branchTarget = true;
            goto bb_15;
bb_4:
  /*$D630*/ CYCLES(0xd630, 2);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_6;
bb_5:
  /*$D630*/ CYCLES_EDGE(0xd630, 1);
            branchTarget = true;
            goto bb_8;
bb_6:
  /*$D632*/ CYCLES(0xd632, 4);
            tmp3_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp3_U8;
            branchTarget = true;
            if (!tmp3_U8)
              goto bb_8;
bb_7:
  /*$D633*/ CYCLES_EDGE(0xd633, 1);
            branchTarget = true;
            goto bb_12;
bb_8:
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
            if (tmp2_U8)
              goto bb_10;
bb_9:
  /*$D63A*/ CYCLES_EDGE(0xd63a, 1);
            branchTarget = true;
            goto bb_15;
bb_10:
  /*$D63C*/ CYCLES(0xd63c, 2);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_12;
bb_11:
  /*$D63C*/ CYCLES_EDGE(0xd63c, 1);
            branchTarget = true;
            goto bb_15;
bb_12:
  /*$D63E*/ CYCLES(0xd63e, 18);
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
            if (!s_status_c)
              goto bb_14;
bb_13:
  /*$D645*/ CYCLES_EDGE(0xd645, 1);
            branchTarget = true;
            goto bb_0;
bb_14:
  /*$D647*/ CYCLES(0xd647, 2);
            s_status_c = 0x00;
bb_15:
  /*$D648*/ CYCLES(0xd648, 6);
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
  /*$D849*/ CYCLES(0xd849, 12);
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
            if (!s_status_c)
              goto bb_5;
bb_4:
  /*$D850*/ CYCLES_EDGE(0xd850, 1);
            branchTarget = true;
            goto bb_6;
bb_5:
  /*$D852*/ CYCLES(0xd852, 2);
            s_y = (uint8_t)(s_y - 0x01);
bb_6:
  /*$D853*/ CYCLES(0xd853, 6);
            ram_poke(0x007d, s_a);
  /*$D855*/ ram_poke(0x007e, s_y);
  /*$D857*/ CYCLES(0xd857, 6);
            if (ret_addr) pop16(); return;
}


void FUNC_NEGATE(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$DB00*/ CYCLES(0xdb00, 2);
            tmp1_U8 = s_a ^ 0xff;
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
  /*$DB02*/ CYCLES(0xdb02, 6);
            if (ret_addr) pop16(); return;
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
  /*$E07D*/ CYCLES(0xe07d, 4);
            tmp1_U8 = s_a;
            s_status_not_z = (tmp1_U8 != 0x41);
            tmp2_U8 = tmp1_U8 >= 0x41;
            s_status_c = tmp2_U8;
            s_status_n = ((uint8_t)(tmp1_U8 - 0x41) & 0x80);
            branchTarget = true;
            if (tmp2_U8)
              goto bb_2;
bb_1:
  /*$E07F*/ CYCLES_EDGE(0xe07f, 1);
            branchTarget = true;
            goto bb_9;
bb_2:
  /*$E081*/ CYCLES(0xe081, 6);
            if (s_status_d)
              goto bb_4;
bb_3:
  /*$E081*/ s_a = (uint8_t)((s_a - 0x005b) - (uint8_t)(0x01 - s_status_c));
            goto bb_5;
bb_4:
  /*$E081*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x5b, s_status_c));
bb_5:
  /*$E083*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_7;
bb_6:
  /*$E084*/ tmp3_U16 = s_a;
            tmp4_U16 = (tmp3_U16 - 0x00a5) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0xff5a);
            tmp2_U8 = (uint8_t)tmp4_U16;
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_a = tmp2_U8;
            goto bb_8;
bb_7:
  /*$E084*/ tmp4_U16 = sbc_dec16(s_a, 0xa5, s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
            s_status_c = (tmp2_U8 & 0x01);
            s_status_not_z = (~tmp2_U8 & 2);
            s_status_v = ((tmp2_U8 & 0x40) != 0);
            s_status_n = (tmp2_U8 & 0x80);
bb_8:
bb_9:
  /*$E086*/ CYCLES(0xe086, 6);
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
  /*$E0ED*/ CYCLES(0xe0ed, 15);
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
            if (s_status_c)
              goto bb_8;
bb_7:
  /*$E0F6*/ CYCLES_EDGE(0xe0f6, 1);
            branchTarget = true;
            goto bb_9;
bb_8:
  /*$E0F8*/ CYCLES(0xe0f8, 2);
            s_y = (uint8_t)(s_y + 0x01);
bb_9:
  /*$E0F9*/ CYCLES(0xe0f9, 12);
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
  /*$E484*/ CYCLES(0xe484, 6);
            s_x = ram_peek(0x0073);
  /*$E486*/ s_a = ram_peek(0x0074);
bb_1:
  /*$E488*/ CYCLES(0xe488, 33);
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
  /*$E4A0*/ CYCLES(0xe4a0, 5);
            branchTarget = true;
            if ((s_a != ram_peek(0x0052)))
              goto bb_4;
bb_3:
  /*$E4A2*/ CYCLES_EDGE(0xe4a2, 1);
            branchTarget = true;
            goto bb_6;
bb_4:
  /*$E4A4*/ CYCLES(0xe4a4, 6);
            FUNC_CHKVAR(0xe4a6);
  /*$E4A7*/ CYCLES(0xe4a7, 2);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_6;
bb_5:
  /*$E4A7*/ CYCLES_EDGE(0xe4a7, 1);
            branchTarget = true;
            goto bb_2;
bb_6:
  /*$E4A9*/ CYCLES(0xe4a9, 17);
  /*$E4AB*/ ram_poke(0x008f, 0x07);
  /*$E4AD*/ tmp1_U8 = ram_peek(0x0069);
            s_a = tmp1_U8;
  /*$E4AF*/ tmp2_U8 = ram_peek(0x006a);
            s_x = tmp2_U8;
  /*$E4B1*/ ram_poke(0x005e, tmp1_U8);
  /*$E4B3*/ ram_poke(0x005f, tmp2_U8);
bb_7:
  /*$E4B5*/ CYCLES(0xe4b5, 5);
            branchTarget = true;
            if (!(s_x != ram_peek(0x006c)))
              goto bb_9;
bb_8:
  /*$E4B7*/ CYCLES_EDGE(0xe4b7, 1);
            branchTarget = true;
            goto bb_11;
bb_9:
  /*$E4B9*/ CYCLES(0xe4b9, 5);
            branchTarget = true;
            if ((s_a != ram_peek(0x006b)))
              goto bb_11;
bb_10:
  /*$E4BB*/ CYCLES_EDGE(0xe4bb, 1);
            branchTarget = true;
            goto bb_13;
bb_11:
  /*$E4BD*/ CYCLES(0xe4bd, 6);
            FUNC_CHKSMPLVAR(0xe4bf);
  /*$E4C0*/ CYCLES(0xe4c0, 2);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_13;
bb_12:
  /*$E4C0*/ CYCLES_EDGE(0xe4c0, 1);
            branchTarget = true;
            goto bb_7;
bb_13:
  /*$E4C2*/ CYCLES(0xe4c2, 11);
            ram_poke(0x0094, s_a);
  /*$E4C4*/ ram_poke(0x0095, s_x);
  /*$E4C8*/ ram_poke(0x008f, 0x03);
bb_14:
  /*$E4CA*/ CYCLES(0xe4ca, 6);
            s_a = ram_peek(0x0094);
  /*$E4CC*/ s_x = ram_peek(0x0095);
bb_15:
  /*$E4CE*/ CYCLES(0xe4ce, 5);
            tmp2_U8 = s_x;
            tmp1_U8 = ram_peek(0x006e);
            s_status_c = (tmp2_U8 >= tmp1_U8);
            branchTarget = true;
            if (!(tmp2_U8 != tmp1_U8))
              goto bb_17;
bb_16:
  /*$E4D0*/ CYCLES_EDGE(0xe4d0, 1);
            branchTarget = true;
            goto bb_20;
bb_17:
  /*$E4D2*/ CYCLES(0xe4d2, 5);
            tmp1_U8 = s_a;
            tmp2_U8 = ram_peek(0x006d);
            s_status_c = (tmp1_U8 >= tmp2_U8);
            branchTarget = true;
            if (!(tmp1_U8 != tmp2_U8))
              goto bb_19;
bb_18:
  /*$E4D4*/ CYCLES_EDGE(0xe4d4, 1);
            branchTarget = true;
            goto bb_20;
bb_19:
  /*$E4D6*/ CYCLES(0xe4d6, 3);
  /*$E562*/ CYCLES(0xe562, 5);
            branchTarget = true;
            if (!ram_peek(0x008b))
              goto bb_46;
            else
              goto bb_47;
bb_20:
  /*$E4D9*/ CYCLES(0xe4d9, 57);
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
              goto bb_22;
bb_21:
  /*$E4E9*/ tmp4_U16 = (s_a + ram_peek(0x0094)) + s_status_c;
            s_status_c = (uint8_t)(tmp4_U16 >> 8);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_23;
bb_22:
  /*$E4E9*/ tmp5_U16 = adc_dec16(s_a, ram_peek(0x0094), s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            s_status_c = ((uint8_t)(tmp5_U16 >> 8) & 0x01);
bb_23:
  /*$E4EB*/ ram_poke(0x0094, s_a);
  /*$E4ED*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$E4EE*/ tmp1_U8 = peek((ram_peek16al(0x005e) + tmp1_U8));
            s_a = tmp1_U8;
            if (s_status_d)
              goto bb_25;
bb_24:
  /*$E4F0*/ s_a = (uint8_t)((s_a + ram_peek(0x0095)) + s_status_c);
            goto bb_26;
bb_25:
  /*$E4F0*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x0095), s_status_c));
bb_26:
  /*$E4F2*/ ram_poke(0x0095, s_a);
  /*$E4F4*/ tmp1_U8 = pop8();
            s_status_i = ((tmp1_U8 & 0x04) != 0);
            s_status_d = ((tmp1_U8 & 0x08) != 0);
            s_status_b = 0x00;
            s_status_v = ((tmp1_U8 & 0x40) != 0);
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_28;
bb_27:
  /*$E4F5*/ CYCLES_EDGE(0xe4f5, 1);
            branchTarget = true;
            goto bb_14;
bb_28:
  /*$E4F7*/ CYCLES(0xe4f7, 4);
            branchTarget = true;
            if (!(s_x & 0x80))
              goto bb_30;
bb_29:
  /*$E4F8*/ CYCLES_EDGE(0xe4f8, 1);
            branchTarget = true;
            goto bb_14;
bb_30:
  /*$E4FA*/ CYCLES(0xe4fa, 21);
  /*$E4FB*/ tmp1_U8 = peek((ram_peek16al(0x005e) + (uint8_t)(s_y + 0x01)));
  /*$E4FD*/ s_y = 0x00;
  /*$E4FF*/ tmp4_U16 = tmp1_U8 << 0x01;
            s_status_c = (uint8_t)(tmp4_U16 >> 8);
            s_a = ((uint8_t)tmp4_U16);
            if (s_status_d)
              goto bb_32;
bb_31:
  /*$E500*/ tmp4_U16 = (s_a + 0x0005) + s_status_c;
            s_status_c = (uint8_t)(tmp4_U16 >> 8);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_33;
bb_32:
  /*$E500*/ tmp5_U16 = adc_dec16(s_a, 0x05, s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            s_status_c = ((uint8_t)(tmp5_U16 >> 8) & 0x01);
bb_33:
            if (s_status_d)
              goto bb_35;
bb_34:
  /*$E502*/ tmp4_U16 = s_a;
            tmp3_U16 = ram_peek(0x005e);
            tmp5_U16 = (tmp4_U16 + tmp3_U16) + s_status_c;
            s_status_c = (uint8_t)(tmp5_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)tmp3_U16);
            s_a = ((uint8_t)tmp5_U16);
            goto bb_36;
bb_35:
  /*$E502*/ tmp5_U16 = adc_dec16(s_a, ram_peek(0x005e), s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
            s_status_c = (tmp1_U8 & 0x01);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
bb_36:
  /*$E504*/ ram_poke(0x005e, s_a);
            branchTarget = true;
            if (s_status_c)
              goto bb_38;
bb_37:
  /*$E506*/ CYCLES_EDGE(0xe506, 1);
            branchTarget = true;
            goto bb_39;
bb_38:
  /*$E508*/ CYCLES(0xe508, 5);
            ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) + 0x01));
bb_39:
  /*$E50A*/ CYCLES(0xe50a, 3);
            s_x = ram_peek(0x005f);
bb_40:
  /*$E50C*/ CYCLES(0xe50c, 5);
            branchTarget = true;
            if (!(s_x != ram_peek(0x0095)))
              goto bb_42;
bb_41:
  /*$E50E*/ CYCLES_EDGE(0xe50e, 1);
            branchTarget = true;
            goto bb_44;
bb_42:
  /*$E510*/ CYCLES(0xe510, 5);
            branchTarget = true;
            if ((s_a != ram_peek(0x0094)))
              goto bb_44;
bb_43:
  /*$E512*/ CYCLES_EDGE(0xe512, 1);
            branchTarget = true;
            goto bb_15;
bb_44:
  /*$E514*/ CYCLES(0xe514, 6);
            FUNC_CHKVAR(0xe516);
  /*$E517*/ CYCLES(0xe517, 2);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_54;
bb_45:
  /*$E517*/ CYCLES_EDGE(0xe517, 1);
            branchTarget = true;
            goto bb_40;
bb_46:
  /*$E564*/ CYCLES_EDGE(0xe564, 1);
  /*$E55D*/ CYCLES(0xe55d, 11);
            s_x = ram_peek(0x005f);
  /*$E55F*/ s_status_not_z = 0x00;
            s_status_n = 0x00;
            s_y = 0x00;
  /*$E561*/ if (ret_addr) pop16(); return;
bb_47:
  /*$E566*/ CYCLES(0xe566, 49);
  /*$E56A*/ s_status_c = 0x00;
            tmp2_U8 = (ram_peek(0x0091) & 0x04) >> 0x01;
  /*$E56C*/ ram_poke(0x0091, tmp2_U8);
  /*$E56E*/ tmp2_U8 = peek((ram_peek16al(0x008a) + tmp2_U8));
            s_a = tmp2_U8;
            if (s_status_d)
              goto bb_49;
bb_48:
  /*$E570*/ tmp3_U16 = (s_a + ram_peek(0x009b)) + s_status_c;
            s_status_c = (uint8_t)(tmp3_U16 >> 8);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_50;
bb_49:
  /*$E570*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x009b), s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
bb_50:
  /*$E572*/ ram_poke(0x0096, s_a);
  /*$E574*/ s_a = ram_peek(0x009c);
            if (s_status_d)
              goto bb_52;
bb_51:
  /*$E576*/ tmp3_U16 = s_a;
            tmp4_U16 = tmp3_U16 + s_status_c;
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x0000);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_53;
bb_52:
  /*$E576*/ tmp4_U16 = adc_dec16(s_a, 0x00, s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
bb_53:
  /*$E578*/ ram_poke(0x0097, s_a);
  /*$E57E*/ ram_poke(0x0094, ram_peek(0x006f));
  /*$E580*/ ram_poke(0x0095, ram_peek(0x0070));
  /*$E582*/ FUNC_MVBLKUP2(0xe584);
  /*$E585*/ CYCLES(0xe585, 35);
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
bb_54:
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
            if (!(tmp1_U8 & 0x80))
              goto bb_2;
bb_1:
  /*$E51B*/ CYCLES_EDGE(0xe51b, 1);
            branchTarget = true;
            goto bb_5;
bb_2:
  /*$E51D*/ CYCLES(0xe51d, 9);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$E51E*/ tmp1_U8 = peek((ram_peek16al(0x005e) + tmp1_U8));
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_4;
bb_3:
  /*$E520*/ CYCLES_EDGE(0xe520, 1);
            branchTarget = true;
            goto bb_5;
bb_4:
  /*$E522*/ CYCLES(0xe522, 2);
            s_y = (uint8_t)(s_y + 0x01);
            FUNC_CHKVAR(0x0000);
            if (ret_addr) pop16(); return;
bb_5:
  /*$E552*/ CYCLES(0xe552, 13);
            s_a = ram_peek(0x008f);
  /*$E554*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_7;
bb_6:
  /*$E555*/ tmp2_U16 = s_a;
            tmp3_U16 = ram_peek(0x005e);
            tmp4_U16 = (tmp2_U16 + tmp3_U16) + s_status_c;
            s_status_c = (uint8_t)(tmp4_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp2_U16, (uint8_t)tmp3_U16);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_8;
bb_7:
  /*$E555*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x005e), s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
            s_status_c = (tmp1_U8 & 0x01);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
bb_8:
  /*$E557*/ ram_poke(0x005e, s_a);
            branchTarget = true;
            if (s_status_c)
              goto bb_10;
bb_9:
  /*$E559*/ CYCLES_EDGE(0xe559, 1);
            branchTarget = true;
            goto bb_11;
bb_10:
  /*$E55B*/ CYCLES(0xe55b, 5);
            ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) + 0x01));
bb_11:
  /*$E55D*/ CYCLES(0xe55d, 11);
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
            if (tmp1_U8)
              goto bb_2;
bb_1:
  /*$E525*/ CYCLES_EDGE(0xe525, 1);
            branchTarget = true;
            goto bb_15;
bb_2:
  /*$E527*/ CYCLES(0xe527, 21);
            tmp1_U8 = s_y;
  /*$E528*/ tmp2_U8 = peek((ram_peek16al(0x005e) + (uint8_t)(tmp1_U8 + 0x01)));
  /*$E52A*/ s_x = tmp2_U8;
  /*$E52C*/ tmp1_U8 = peek((ram_peek16al(0x005e) + (uint8_t)(tmp1_U8 + 0x02)));
            s_a = tmp1_U8;
  /*$E52E*/ tmp2_U8 = ram_peek(0x0070);
            s_status_not_z = (tmp1_U8 != tmp2_U8);
            branchTarget = true;
            if ((tmp1_U8 >= tmp2_U8))
              goto bb_4;
bb_3:
  /*$E530*/ CYCLES_EDGE(0xe530, 1);
            branchTarget = true;
            goto bb_8;
bb_4:
  /*$E532*/ CYCLES(0xe532, 2);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_6;
bb_5:
  /*$E532*/ CYCLES_EDGE(0xe532, 1);
            branchTarget = true;
            goto bb_15;
bb_6:
  /*$E534*/ CYCLES(0xe534, 5);
            branchTarget = true;
            if (!(s_x >= ram_peek(0x006f)))
              goto bb_8;
bb_7:
  /*$E536*/ CYCLES_EDGE(0xe536, 1);
            branchTarget = true;
            goto bb_15;
bb_8:
  /*$E538*/ CYCLES(0xe538, 5);
            tmp2_U8 = s_a;
            tmp1_U8 = ram_peek(0x009c);
            s_status_not_z = (tmp2_U8 != tmp1_U8);
            branchTarget = true;
            if ((tmp2_U8 >= tmp1_U8))
              goto bb_10;
bb_9:
  /*$E53A*/ CYCLES_EDGE(0xe53a, 1);
            branchTarget = true;
            goto bb_15;
bb_10:
  /*$E53C*/ CYCLES(0xe53c, 2);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_12;
bb_11:
  /*$E53C*/ CYCLES_EDGE(0xe53c, 1);
            branchTarget = true;
            goto bb_14;
bb_12:
  /*$E53E*/ CYCLES(0xe53e, 5);
            branchTarget = true;
            if ((s_x >= ram_peek(0x009b)))
              goto bb_14;
bb_13:
  /*$E540*/ CYCLES_EDGE(0xe540, 1);
            branchTarget = true;
            goto bb_15;
bb_14:
  /*$E542*/ CYCLES(0xe542, 24);
            ram_poke(0x009b, s_x);
  /*$E544*/ ram_poke(0x009c, s_a);
  /*$E54A*/ ram_poke(0x008a, ram_peek(0x005e));
  /*$E54C*/ ram_poke(0x008b, ram_peek(0x005f));
  /*$E550*/ ram_poke(0x0091, ram_peek(0x008f));
bb_15:
  /*$E552*/ CYCLES(0xe552, 13);
            s_a = ram_peek(0x008f);
  /*$E554*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_17;
bb_16:
  /*$E555*/ tmp3_U16 = s_a;
            tmp4_U16 = ram_peek(0x005e);
            tmp5_U16 = (tmp3_U16 + tmp4_U16) + s_status_c;
            s_status_c = (uint8_t)(tmp5_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
            s_a = ((uint8_t)tmp5_U16);
            goto bb_18;
bb_17:
  /*$E555*/ tmp5_U16 = adc_dec16(s_a, ram_peek(0x005e), s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
            s_status_c = (tmp1_U8 & 0x01);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
bb_18:
  /*$E557*/ ram_poke(0x005e, s_a);
            branchTarget = true;
            if (s_status_c)
              goto bb_20;
bb_19:
  /*$E559*/ CYCLES_EDGE(0xe559, 1);
            branchTarget = true;
            goto bb_21;
bb_20:
  /*$E55B*/ CYCLES(0xe55b, 5);
            ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) + 0x01));
bb_21:
  /*$E55D*/ CYCLES(0xe55d, 11);
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
  /*$E5D4*/ CYCLES(0xe5d4, 32);
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
  /*$E5E2*/ CYCLES(0xe5e2, 6);
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
  /*$E5E6*/ CYCLES(0xe5e6, 4);
            tmp1_U8 = s_a;
            s_y = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_2;
bb_1:
  /*$E5E7*/ CYCLES_EDGE(0xe5e7, 1);
            branchTarget = true;
            goto bb_6;
bb_2:
  /*$E5E9*/ CYCLES(0xe5e9, 3);
            push8(s_a);
bb_3:
  /*$E5EA*/ CYCLES(0xe5ea, 17);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
  /*$E5EB*/ tmp2_U8 = peek((ram_peek16al(0x005e) + tmp1_U8));
  /*$E5ED*/ poke((ram_peek16(0x0071) + tmp1_U8), tmp2_U8);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_5;
bb_4:
  /*$E5F0*/ CYCLES_EDGE(0xe5f0, 1);
            branchTarget = true;
            goto bb_3;
bb_5:
  /*$E5F2*/ CYCLES(0xe5f2, 4);
            tmp1_U8 = pop8();
            s_a = tmp1_U8;
bb_6:
  /*$E5F3*/ CYCLES(0xe5f3, 10);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_8;
bb_7:
  /*$E5F4*/ tmp3_U16 = s_a;
            tmp4_U16 = ram_peek(0x0071);
            tmp5_U16 = (tmp3_U16 + tmp4_U16) + s_status_c;
            s_status_c = (uint8_t)(tmp5_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
            tmp1_U8 = (uint8_t)tmp5_U16;
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            goto bb_9;
bb_8:
  /*$E5F4*/ tmp5_U16 = adc_dec16(s_a, ram_peek(0x0071), s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
            s_status_c = (tmp1_U8 & 0x01);
            s_status_not_z = (~tmp1_U8 & 2);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
            s_status_n = (tmp1_U8 & 0x80);
bb_9:
  /*$E5F6*/ ram_poke(0x0071, s_a);
            branchTarget = true;
            if (s_status_c)
              goto bb_11;
bb_10:
  /*$E5F8*/ CYCLES_EDGE(0xe5f8, 1);
            branchTarget = true;
            goto bb_12;
bb_11:
  /*$E5FA*/ CYCLES(0xe5fa, 5);
            tmp1_U8 = (uint8_t)(ram_peek(0x0072) + 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            ram_poke(0x0072, tmp1_U8);
bb_12:
  /*$E5FC*/ CYCLES(0xe5fc, 6);
            if (ret_addr) pop16(); return;
}


void FUNC_FREFAC(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E600*/ CYCLES(0xe600, 6);
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
  /*$E608*/ FUNC_FRETMS(0xe60a);
  /*$E60B*/ CYCLES(0xe60b, 41);
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
            if (!tmp2_U8)
              goto bb_2;
bb_1:
  /*$E61B*/ CYCLES_EDGE(0xe61b, 1);
            branchTarget = true;
            goto bb_13;
bb_2:
  /*$E61D*/ CYCLES(0xe61d, 5);
            tmp2_U8 = s_y;
            tmp1_U8 = ram_peek(0x0070);
            tmp3_U8 = tmp2_U8 != tmp1_U8;
            s_status_not_z = tmp3_U8;
            s_status_c = (tmp2_U8 >= tmp1_U8);
            s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
            branchTarget = true;
            if (!tmp3_U8)
              goto bb_4;
bb_3:
  /*$E61F*/ CYCLES_EDGE(0xe61f, 1);
            branchTarget = true;
            goto bb_13;
bb_4:
  /*$E621*/ CYCLES(0xe621, 5);
            tmp3_U8 = s_x;
            tmp1_U8 = ram_peek(0x006f);
            tmp2_U8 = tmp3_U8 != tmp1_U8;
            s_status_not_z = tmp2_U8;
            s_status_c = (tmp3_U8 >= tmp1_U8);
            s_status_n = ((uint8_t)(tmp3_U8 - tmp1_U8) & 0x80);
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_6;
bb_5:
  /*$E623*/ CYCLES_EDGE(0xe623, 1);
            branchTarget = true;
            goto bb_13;
bb_6:
  /*$E625*/ CYCLES(0xe625, 13);
            push8(s_a);
  /*$E626*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_8;
bb_7:
  /*$E627*/ tmp4_U16 = s_a;
            tmp5_U16 = ram_peek(0x006f);
            tmp6_U16 = (tmp4_U16 + tmp5_U16) + s_status_c;
            s_status_c = (uint8_t)(tmp6_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)tmp5_U16);
            s_a = ((uint8_t)tmp6_U16);
            goto bb_9;
bb_8:
  /*$E627*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x006f), s_status_c);
            s_a = ((uint8_t)tmp6_U16);
            tmp2_U8 = (uint8_t)(tmp6_U16 >> 8);
            s_status_c = (tmp2_U8 & 0x01);
            s_status_v = ((tmp2_U8 & 0x40) != 0);
bb_9:
  /*$E629*/ ram_poke(0x006f, s_a);
            branchTarget = true;
            if (s_status_c)
              goto bb_11;
bb_10:
  /*$E62B*/ CYCLES_EDGE(0xe62b, 1);
            branchTarget = true;
            goto bb_12;
bb_11:
  /*$E62D*/ CYCLES(0xe62d, 5);
            ram_poke(0x0070, (uint8_t)(ram_peek(0x0070) + 0x01));
bb_12:
  /*$E62F*/ CYCLES(0xe62f, 4);
            tmp2_U8 = pop8();
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_a = tmp2_U8;
bb_13:
  /*$E630*/ CYCLES(0xe630, 12);
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
  /*$E635*/ CYCLES(0xe635, 5);
            tmp1_U8 = s_y;
            tmp2_U8 = ram_peek(0x0054);
            tmp3_U8 = tmp1_U8 != tmp2_U8;
            s_status_not_z = tmp3_U8;
            s_status_c = (tmp1_U8 >= tmp2_U8);
            s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
            branchTarget = true;
            if (!tmp3_U8)
              goto bb_2;
bb_1:
  /*$E637*/ CYCLES_EDGE(0xe637, 1);
            branchTarget = true;
            goto bb_8;
bb_2:
  /*$E639*/ CYCLES(0xe639, 5);
            tmp3_U8 = s_a;
            tmp2_U8 = ram_peek(0x0053);
            tmp1_U8 = tmp3_U8 != tmp2_U8;
            s_status_not_z = tmp1_U8;
            s_status_c = (tmp3_U8 >= tmp2_U8);
            s_status_n = ((uint8_t)(tmp3_U8 - tmp2_U8) & 0x80);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_4;
bb_3:
  /*$E63B*/ CYCLES_EDGE(0xe63b, 1);
            branchTarget = true;
            goto bb_8;
bb_4:
  /*$E63D*/ CYCLES(0xe63d, 10);
            ram_poke(0x0052, s_a);
            if (s_status_d)
              goto bb_6;
bb_5:
  /*$E63F*/ tmp4_U16 = s_a;
            tmp5_U16 = (tmp4_U16 - 0x0003) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0xfffc);
            s_a = ((uint8_t)tmp5_U16);
            goto bb_7;
bb_6:
  /*$E63F*/ tmp5_U16 = sbc_dec16(s_a, 0x03, s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
            s_status_c = (tmp1_U8 & 0x01);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
bb_7:
  /*$E641*/ ram_poke(0x0053, s_a);
  /*$E643*/ s_status_not_z = 0x00;
            s_status_n = 0x00;
bb_8:
  /*$E645*/ CYCLES(0xe645, 6);
            if (ret_addr) pop16(); return;
}


void FUNC_TWSCMPFAC(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E89E*/ CYCLES(0xe89e, 8);
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
  /*$E8A4*/ CYCLES(0xe8a4, 47);
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
  /*$E8C4*/ CYCLES_EDGE(0xe8c4, 1);
  /*$E8D4*/ CYCLES(0xe8d4, 6);
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
            if (!tmp1_U8)
              goto bb_2;
bb_1:
  /*$E8C8*/ CYCLES_EDGE(0xe8c8, 1);
            branchTarget = true;
            goto bb_7;
bb_2:
  /*$E8CA*/ CYCLES(0xe8ca, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x00a0) + 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            ram_poke(0x00a0, tmp1_U8);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_4;
bb_3:
  /*$E8CC*/ CYCLES_EDGE(0xe8cc, 1);
            branchTarget = true;
            goto bb_7;
bb_4:
  /*$E8CE*/ CYCLES(0xe8ce, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x009f) + 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            ram_poke(0x009f, tmp1_U8);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_6;
bb_5:
  /*$E8D0*/ CYCLES_EDGE(0xe8d0, 1);
            branchTarget = true;
            goto bb_7;
bb_6:
  /*$E8D2*/ CYCLES(0xe8d2, 5);
            tmp1_U8 = (uint8_t)(ram_peek(0x009e) + 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            ram_poke(0x009e, tmp1_U8);
bb_7:
  /*$E8D4*/ CYCLES(0xe8d4, 6);
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
  /*$E8F0*/ CYCLES(0xe8f0, 4);
            if (s_status_d)
              goto bb_3;
            else
              goto bb_2;
bb_1:
  /*$E8DC*/ CYCLES(0xe8dc, 38);
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
            if (!s_status_n)
              goto bb_6;
bb_5:
  /*$E8F2*/ CYCLES_EDGE(0xe8f2, 1);
            branchTarget = true;
            goto bb_1;
bb_6:
  /*$E8F4*/ CYCLES(0xe8f4, 2);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_8;
bb_7:
  /*$E8F4*/ CYCLES_EDGE(0xe8f4, 1);
            branchTarget = true;
            goto bb_1;
bb_8:
  /*$E8F6*/ CYCLES(0xe8f6, 9);
            if (s_status_d)
              goto bb_10;
bb_9:
  /*$E8F6*/ tmp1_U16 = s_a;
            tmp3_U16 = (tmp1_U16 - 0x0008) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp1_U16, (uint8_t)0xfff7);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_11;
bb_10:
  /*$E8F6*/ tmp1_U16 = sbc_dec16(s_a, 0x08, s_status_c);
            s_a = ((uint8_t)tmp1_U16);
            tmp2_U8 = (uint8_t)(tmp1_U16 >> 8);
            s_status_c = (tmp2_U8 & 0x01);
            s_status_v = ((tmp2_U8 & 0x40) != 0);
bb_11:
  /*$E8F8*/ s_y = s_a;
  /*$E8F9*/ tmp2_U8 = ram_peek(0x00ac);
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_a = tmp2_U8;
            branchTarget = true;
            if (!s_status_c)
              goto bb_13;
bb_12:
  /*$E8FB*/ CYCLES_EDGE(0xe8fb, 1);
            branchTarget = true;
            goto bb_18;
bb_13:
  /*$E8FD*/ CYCLES(0xe8fd, 8);
            tmp3_U16 = (uint8_t)(0x01 + s_x);
            tmp1_U16 = ram_peek(tmp3_U16) << 0x01;
            tmp2_U8 = (uint8_t)(tmp1_U16 >> 8);
            s_status_c = tmp2_U8;
            ram_poke(tmp3_U16, ((uint8_t)tmp1_U16));
            branchTarget = true;
            if (tmp2_U8)
              goto bb_15;
bb_14:
  /*$E8FF*/ CYCLES_EDGE(0xe8ff, 1);
            branchTarget = true;
            goto bb_16;
bb_15:
  /*$E901*/ CYCLES(0xe901, 6);
            tmp1_U16 = (uint8_t)(0x01 + s_x);
            ram_poke(tmp1_U16, (uint8_t)(ram_peek(tmp1_U16) + 0x01));
bb_16:
  /*$E903*/ CYCLES(0xe903, 12);
            tmp2_U8 = s_x;
            tmp1_U16 = (uint8_t)(0x01 + tmp2_U8);
            tmp4_U8 = ram_peek(tmp1_U16);
            ram_poke(tmp1_U16, (uint8_t)((tmp4_U8 | (0x00 + (s_status_c << 8))) >> 0x01));
  /*$E905*/ tmp1_U16 = (uint8_t)(0x01 + tmp2_U8);
            tmp5_U8 = ram_peek(tmp1_U16);
            ram_poke(tmp1_U16, (uint8_t)((tmp5_U8 | (0x00 + ((tmp4_U8 & 0x01) << 8))) >> 0x01));
  /*$E907*/ CYCLES(0xe907, 24);
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
            if (!tmp2_U8)
              goto bb_18;
bb_17:
  /*$E90F*/ CYCLES_EDGE(0xe90f, 1);
            branchTarget = true;
            goto bb_13;
bb_18:
  /*$E911*/ CYCLES(0xe911, 8);
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
  /*$E907*/ CYCLES(0xe907, 24);
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
            if (tmp1_U8)
              goto bb_4;
            else
              goto bb_5;
bb_1:
  /*$E8FF*/ CYCLES_EDGE(0xe8ff, 1);
            branchTarget = true;
            goto bb_3;
bb_2:
  /*$E901*/ CYCLES(0xe901, 6);
            tmp5_U16 = (uint8_t)(0x01 + s_x);
            ram_poke(tmp5_U16, (uint8_t)(ram_peek(tmp5_U16) + 0x01));
bb_3:
  /*$E903*/ CYCLES(0xe903, 12);
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
  /*$E90F*/ CYCLES_EDGE(0xe90f, 1);
  /*$E8FD*/ CYCLES(0xe8fd, 8);
            tmp2_U16 = (uint8_t)(0x01 + s_x);
            tmp5_U16 = ram_peek(tmp2_U16) << 0x01;
            tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
            s_status_c = tmp1_U8;
            ram_poke(tmp2_U16, ((uint8_t)tmp5_U16));
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_1;
            else
              goto bb_2;
bb_5:
  /*$E911*/ CYCLES(0xe911, 8);
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
  /*$E9B0*/ CYCLES(0xe9b0, 2);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_19;
            else
              goto bb_20;
bb_1:
  /*$E8DC*/ CYCLES(0xe8dc, 38);
            tmp1_U8 = s_x;
  /*$E8DE*/ ram_poke(0x00ac, ram_peek((uint8_t)(0x04 + tmp1_U8)));
  /*$E8E2*/ ram_poke((uint8_t)(0x04 + tmp1_U8), ram_peek((uint8_t)(0x03 + tmp1_U8)));
  /*$E8E6*/ ram_poke((uint8_t)(0x03 + tmp1_U8), ram_peek((uint8_t)(0x02 + tmp1_U8)));
  /*$E8EA*/ ram_poke((uint8_t)(0x02 + tmp1_U8), ram_peek((uint8_t)(0x01 + tmp1_U8)));
  /*$E8EE*/ ram_poke((uint8_t)(0x01 + tmp1_U8), ram_peek(0x00a4));
  /*$E8F0*/ CYCLES(0xe8f0, 4);
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
            if (!s_status_n)
              goto bb_6;
bb_5:
  /*$E8F2*/ CYCLES_EDGE(0xe8f2, 1);
            branchTarget = true;
            goto bb_1;
bb_6:
  /*$E8F4*/ CYCLES(0xe8f4, 2);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_8;
bb_7:
  /*$E8F4*/ CYCLES_EDGE(0xe8f4, 1);
            branchTarget = true;
            goto bb_1;
bb_8:
  /*$E8F6*/ CYCLES(0xe8f6, 9);
            if (s_status_d)
              goto bb_10;
bb_9:
  /*$E8F6*/ tmp2_U16 = (s_a - 0x0008) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp2_U16 >> 8) & 0x01));
            s_a = ((uint8_t)tmp2_U16);
            goto bb_11;
bb_10:
  /*$E8F6*/ tmp3_U16 = sbc_dec16(s_a, 0x08, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
bb_11:
  /*$E8F8*/ s_y = s_a;
  /*$E8F9*/ s_a = ram_peek(0x00ac);
            branchTarget = true;
            if (!s_status_c)
              goto bb_13;
bb_12:
  /*$E8FB*/ CYCLES_EDGE(0xe8fb, 1);
            branchTarget = true;
            goto bb_18;
bb_13:
  /*$E8FD*/ CYCLES(0xe8fd, 8);
            tmp2_U16 = (uint8_t)(0x01 + s_x);
            tmp3_U16 = ram_peek(tmp2_U16) << 0x01;
            tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
            s_status_c = tmp1_U8;
            ram_poke(tmp2_U16, ((uint8_t)tmp3_U16));
            branchTarget = true;
            if (tmp1_U8)
              goto bb_15;
bb_14:
  /*$E8FF*/ CYCLES_EDGE(0xe8ff, 1);
            branchTarget = true;
            goto bb_16;
bb_15:
  /*$E901*/ CYCLES(0xe901, 6);
            tmp3_U16 = (uint8_t)(0x01 + s_x);
            ram_poke(tmp3_U16, (uint8_t)(ram_peek(tmp3_U16) + 0x01));
bb_16:
  /*$E903*/ CYCLES(0xe903, 12);
            tmp1_U8 = s_x;
            tmp3_U16 = (uint8_t)(0x01 + tmp1_U8);
            tmp4_U8 = ram_peek(tmp3_U16);
            ram_poke(tmp3_U16, (uint8_t)((tmp4_U8 | (0x00 + (s_status_c << 8))) >> 0x01));
  /*$E905*/ tmp3_U16 = (uint8_t)(0x01 + tmp1_U8);
            tmp5_U8 = ram_peek(tmp3_U16);
            ram_poke(tmp3_U16, (uint8_t)((tmp5_U8 | (0x00 + ((tmp4_U8 & 0x01) << 8))) >> 0x01));
  /*$E907*/ CYCLES(0xe907, 24);
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
            if (!tmp1_U8)
              goto bb_18;
bb_17:
  /*$E90F*/ CYCLES_EDGE(0xe90f, 1);
            branchTarget = true;
            goto bb_13;
bb_18:
  /*$E911*/ CYCLES(0xe911, 8);
            s_status_c = 0x00;
  /*$E912*/ if (ret_addr) pop16(); return;
bb_19:
  /*$E9B0*/ CYCLES_EDGE(0xe9b0, 1);
            FUNC_MULT2(0x0000);
            if (ret_addr) pop16(); return;
bb_20:
  /*$E9B2*/ CYCLES(0xe9b2, 3);
  /*$E8DA*/ CYCLES(0xe8da, 2);
            s_x = 0x61;
            goto bb_1;
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
  /*$E9B5*/ CYCLES(0xe9b5, 4);
            tmp1_U8 = s_a;
            s_status_c = (tmp1_U8 & 0x01);
  /*$E9B6*/ s_a = ((tmp1_U8 >> 0x01) | 0x80);
bb_1:
  /*$E9B8*/ CYCLES(0xe9b8, 4);
            s_y = s_a;
            branchTarget = true;
            if (s_status_c)
              goto bb_3;
bb_2:
  /*$E9B9*/ CYCLES_EDGE(0xe9b9, 1);
            branchTarget = true;
            goto bb_16;
bb_3:
  /*$E9BB*/ CYCLES(0xe9bb, 38);
            s_status_c = 0x00;
  /*$E9BC*/ s_a = ram_peek(0x0065);
            if (s_status_d)
              goto bb_5;
bb_4:
  /*$E9BE*/ tmp2_U16 = (s_a + ram_peek(0x00a9)) + s_status_c;
            s_status_c = (uint8_t)(tmp2_U16 >> 8);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_6;
bb_5:
  /*$E9BE*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x00a9), s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
bb_6:
  /*$E9C0*/ ram_poke(0x0065, s_a);
  /*$E9C2*/ s_a = ram_peek(0x0064);
            if (s_status_d)
              goto bb_8;
bb_7:
  /*$E9C4*/ tmp2_U16 = (s_a + ram_peek(0x00a8)) + s_status_c;
            s_status_c = (uint8_t)(tmp2_U16 >> 8);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_9;
bb_8:
  /*$E9C4*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x00a8), s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
bb_9:
  /*$E9C6*/ ram_poke(0x0064, s_a);
  /*$E9C8*/ s_a = ram_peek(0x0063);
            if (s_status_d)
              goto bb_11;
bb_10:
  /*$E9CA*/ tmp2_U16 = (s_a + ram_peek(0x00a7)) + s_status_c;
            s_status_c = (uint8_t)(tmp2_U16 >> 8);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_12;
bb_11:
  /*$E9CA*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x00a7), s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
bb_12:
  /*$E9CC*/ ram_poke(0x0063, s_a);
  /*$E9CE*/ s_a = ram_peek(0x0062);
            if (s_status_d)
              goto bb_14;
bb_13:
  /*$E9D0*/ tmp2_U16 = s_a;
            tmp3_U16 = ram_peek(0x00a6);
            tmp4_U16 = (tmp2_U16 + tmp3_U16) + s_status_c;
            s_status_c = (uint8_t)(tmp4_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp2_U16, (uint8_t)tmp3_U16);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_15;
bb_14:
  /*$E9D0*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x00a6), s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
            s_status_c = (tmp5_U8 & 0x01);
            s_status_v = ((tmp5_U8 & 0x40) != 0);
bb_15:
  /*$E9D2*/ ram_poke(0x0062, s_a);
bb_16:
  /*$E9D4*/ CYCLES(0xe9d4, 31);
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
            if (!tmp5_U8)
              goto bb_18;
bb_17:
  /*$E9E0*/ CYCLES_EDGE(0xe9e0, 1);
            branchTarget = true;
            goto bb_1;
bb_18:
  /*$E9E2*/ CYCLES(0xe9e2, 6);
            if (ret_addr) pop16(); return;
}


void FUNC_UPAY2ARG(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E9E3*/ CYCLES(0xe9e3, 79);
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
  /*$EAF9*/ CYCLES(0xeaf9, 70);
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
  /*$EBB2*/ CYCLES(0xebb2, 19);
            ram_poke(0x0060, s_a);
  /*$EBB4*/ ram_poke(0x0061, s_y);
  /*$EBB8*/ tmp1_U8 = peek(ram_peek16al(0x0060));
  /*$EBBA*/ s_y = 0x01;
  /*$EBBB*/ s_x = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_7;
            else
              goto bb_8;
bb_1:
  /*$EB84*/ CYCLES_EDGE(0xeb84, 1);
            branchTarget = true;
            goto bb_6;
bb_2:
  /*$EB86*/ CYCLES(0xeb86, 3);
            s_a = ram_peek(0x00a2);
bb_3:
  /*$EB88*/ CYCLES(0xeb88, 6);
            tmp1_U8 = (uint8_t)(((s_a << 0x01) | s_status_c) >> 8);
            s_status_c = tmp1_U8;
  /*$EB89*/ s_status_not_z = 0xff;
            s_status_n = 0x80;
            s_a = 0xff;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_5;
bb_4:
  /*$EB8B*/ CYCLES_EDGE(0xeb8b, 1);
            branchTarget = true;
            goto bb_6;
bb_5:
  /*$EB8D*/ CYCLES(0xeb8d, 2);
            s_status_not_z = 0x01;
            s_status_n = 0x00;
            s_a = 0x01;
bb_6:
  /*$EB8F*/ CYCLES(0xeb8f, 6);
            if (ret_addr) pop16(); return;
bb_7:
  /*$EBBC*/ CYCLES_EDGE(0xebbc, 1);
  /*$EB82*/ CYCLES(0xeb82, 5);
            tmp1_U8 = ram_peek(0x009d);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_1;
            else
              goto bb_2;
bb_8:
  /*$EBBE*/ CYCLES(0xebbe, 10);
            tmp1_U8 = peek((ram_peek16al(0x0060) + s_y));
            branchTarget = true;
            if (!((tmp1_U8 ^ ram_peek(0x00a2)) & 0x80))
              goto bb_10;
bb_9:
  /*$EBC2*/ CYCLES_EDGE(0xebc2, 1);
            branchTarget = true;
            goto bb_2;
bb_10:
  /*$EBC4*/ CYCLES(0xebc4, 5);
            tmp1_U8 = s_x;
            tmp2_U8 = ram_peek(0x009d);
            s_status_c = (tmp1_U8 >= tmp2_U8);
            branchTarget = true;
            if (!(tmp1_U8 != tmp2_U8))
              goto bb_12;
bb_11:
  /*$EBC6*/ CYCLES_EDGE(0xebc6, 1);
            branchTarget = true;
            goto bb_23;
bb_12:
  /*$EBC8*/ CYCLES(0xebc8, 12);
            tmp2_U8 = peek((ram_peek16al(0x0060) + s_y));
  /*$EBCA*/ tmp2_U8 = tmp2_U8 | 0x80;
  /*$EBCC*/ tmp1_U8 = ram_peek(0x009e);
            s_status_c = (tmp2_U8 >= tmp1_U8);
            branchTarget = true;
            if (!(tmp2_U8 != tmp1_U8))
              goto bb_14;
bb_13:
  /*$EBCE*/ CYCLES_EDGE(0xebce, 1);
            branchTarget = true;
            goto bb_23;
bb_14:
  /*$EBD0*/ CYCLES(0xebd0, 12);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$EBD1*/ tmp1_U8 = peek((ram_peek16al(0x0060) + tmp1_U8));
  /*$EBD3*/ tmp2_U8 = ram_peek(0x009f);
            s_status_c = (tmp1_U8 >= tmp2_U8);
            branchTarget = true;
            if (!(tmp1_U8 != tmp2_U8))
              goto bb_16;
bb_15:
  /*$EBD5*/ CYCLES_EDGE(0xebd5, 1);
            branchTarget = true;
            goto bb_23;
bb_16:
  /*$EBD7*/ CYCLES(0xebd7, 12);
            tmp2_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp2_U8;
  /*$EBD8*/ tmp2_U8 = peek((ram_peek16al(0x0060) + tmp2_U8));
  /*$EBDA*/ tmp1_U8 = ram_peek(0x00a0);
            s_status_c = (tmp2_U8 >= tmp1_U8);
            branchTarget = true;
            if (!(tmp2_U8 != tmp1_U8))
              goto bb_18;
bb_17:
  /*$EBDC*/ CYCLES_EDGE(0xebdc, 1);
            branchTarget = true;
            goto bb_23;
bb_18:
  /*$EBDE*/ CYCLES(0xebde, 17);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$EBE1*/ s_status_c = (0x7f >= ram_peek(0x00ac));
  /*$EBE3*/ tmp1_U8 = peek((ram_peek16al(0x0060) + tmp1_U8));
            s_a = tmp1_U8;
            if (s_status_d)
              goto bb_20;
bb_19:
  /*$EBE5*/ tmp3_U16 = s_a;
            tmp4_U16 = ram_peek(0x00a1);
            tmp5_U16 = (tmp3_U16 - tmp4_U16) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp4_U16));
            tmp1_U8 = (uint8_t)tmp5_U16;
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            goto bb_21;
bb_20:
  /*$EBE5*/ tmp5_U16 = sbc_dec16(s_a, ram_peek(0x00a1), s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
            s_status_c = (tmp1_U8 & 0x01);
            s_status_not_z = (~tmp1_U8 & 2);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
            s_status_n = (tmp1_U8 & 0x80);
bb_21:
            branchTarget = true;
            if (s_status_not_z)
              goto bb_23;
bb_22:
  /*$EBE7*/ CYCLES_EDGE(0xebe7, 1);
  /*$EC11*/ CYCLES(0xec11, 6);
            if (ret_addr) pop16(); return;
bb_23:
  /*$EBE9*/ CYCLES(0xebe9, 5);
            s_a = ram_peek(0x00a2);
            branchTarget = true;
            if (s_status_c)
              goto bb_25;
bb_24:
  /*$EBEB*/ CYCLES_EDGE(0xebeb, 1);
            branchTarget = true;
            goto bb_26;
bb_25:
  /*$EBED*/ CYCLES(0xebed, 2);
            s_a = (s_a ^ 0xff);
bb_26:
  /*$EBEF*/ CYCLES(0xebef, 3);
            branchTarget = true;
            goto bb_3;
}


void FUNC_QINT(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$EBF2*/ CYCLES(0xebf2, 5);
            tmp1_U8 = ram_peek(0x009d);
            s_a = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_2;
bb_1:
  /*$EBF4*/ CYCLES_EDGE(0xebf4, 1);
  /*$EC40*/ CYCLES(0xec40, 20);
            tmp1_U8 = s_a;
            ram_poke(0x009e, tmp1_U8);
  /*$EC42*/ ram_poke(0x009f, tmp1_U8);
  /*$EC44*/ ram_poke(0x00a0, tmp1_U8);
  /*$EC46*/ ram_poke(0x00a1, tmp1_U8);
  /*$EC48*/ s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_y = tmp1_U8;
  /*$EC49*/ if (ret_addr) pop16(); return;
bb_2:
  /*$EBF6*/ CYCLES(0xebf6, 9);
            s_status_c = 0x01;
            if (s_status_d)
              goto bb_4;
bb_3:
  /*$EBF7*/ s_a = (uint8_t)((s_a - 0x00a0) - (uint8_t)(0x01 - s_status_c));
            goto bb_5;
bb_4:
  /*$EBF7*/ s_a = ((uint8_t)sbc_dec16(s_a, 0xa0, s_status_c));
bb_5:
  /*$EBF9*/ tmp1_U8 = ram_peek(0x00a2);
            s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_7;
bb_6:
  /*$EBFB*/ CYCLES_EDGE(0xebfb, 1);
            branchTarget = true;
            goto bb_8;
bb_7:
  /*$EBFD*/ CYCLES(0xebfd, 13);
            tmp1_U8 = s_a;
  /*$EC00*/ ram_poke(0x00a4, 0xff);
  /*$EC02*/ FUNC_TCFACMANT(0xec04);
  /*$EC05*/ CYCLES(0xec05, 2);
            s_a = tmp1_U8;
bb_8:
  /*$EC06*/ CYCLES(0xec06, 6);
            s_x = 0x9d;
  /*$EC08*/ tmp1_U8 = s_a;
            s_status_c = (tmp1_U8 >= 0xf9);
            branchTarget = true;
            if (((uint8_t)(tmp1_U8 - 0xf9) & 0x80))
              goto bb_10;
bb_9:
  /*$EC0A*/ CYCLES_EDGE(0xec0a, 1);
  /*$EC12*/ CYCLES(0xec12, 24);
            s_y = s_a;
  /*$EC17*/ tmp1_U8 = ram_peek(0x009e);
            s_status_c = (tmp1_U8 & 0x01);
            ram_poke(0x009e, (tmp1_U8 >> 0x01));
  /*$EC19*/ tmp1_U8 = (ram_peek(0x00a2) & 0x80) | ram_peek(0x009e);
            s_a = tmp1_U8;
  /*$EC1B*/ ram_poke(0x009e, tmp1_U8);
  /*$EC1D*/ FUNC_SHFTRGHT4(0xec1f);
  /*$EC20*/ CYCLES(0xec20, 9);
            ram_poke(0x00a4, s_y);
  /*$EC22*/ if (ret_addr) pop16(); return;
bb_10:
  /*$EC0C*/ CYCLES(0xec0c, 6);
            FUNC_SHFTRGHT(0xec0e);
  /*$EC0F*/ CYCLES(0xec0f, 3);
            ram_poke(0x00a4, s_y);
  /*$EC11*/ CYCLES(0xec11, 6);
            if (ret_addr) pop16(); return;
}


void FUNC_NORMAL(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$F273*/ CYCLES(0xf273, 4);
  /*$F275*/ CYCLES_EDGE(0xf275, 1);
  /*$F279*/ CYCLES(0xf279, 14);
  /*$F27B*/ ram_poke(0x0032, 0xff);
  /*$F27D*/ ram_poke(0x00f3, 0x00);
  /*$F27F*/ if (ret_addr) pop16(); return;
}


void FUNC_MON_PLOT(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$F800*/ CYCLES(0xf800, 11);
            tmp1_U8 = s_a;
            tmp2_U8 = tmp1_U8 >> 0x01;
            s_a = tmp2_U8;
  /*$F801*/ push8(((tmp1_U8 & 0x01) | ((tmp2_U8 == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | (tmp2_U8 & 0x80)));
  /*$F802*/ FUNC_GBASCALC(0xf804);
  /*$F805*/ CYCLES(0xf805, 8);
            tmp1_U8 = pop8();
            tmp2_U8 = tmp1_U8 & 0x01;
            s_status_c = tmp2_U8;
            s_status_i = ((tmp1_U8 & 0x04) != 0);
            s_status_d = ((tmp1_U8 & 0x08) != 0);
            s_status_b = 0x00;
            s_status_v = ((tmp1_U8 & 0x40) != 0);
  /*$F806*/ s_a = 0x0f;
            branchTarget = true;
            if (tmp2_U8)
              goto bb_2;
bb_1:
  /*$F808*/ CYCLES_EDGE(0xf808, 1);
            branchTarget = true;
            goto bb_6;
bb_2:
  /*$F80A*/ CYCLES(0xf80a, 2);
            if (s_status_d)
              goto bb_4;
bb_3:
  /*$F80A*/ tmp3_U16 = s_a;
            tmp4_U16 = (tmp3_U16 + 0x00e0) + s_status_c;
            s_status_c = (uint8_t)(tmp4_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x00e0);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_5;
bb_4:
  /*$F80A*/ tmp4_U16 = adc_dec16(s_a, 0xe0, s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
            s_status_c = (tmp2_U8 & 0x01);
            s_status_v = ((tmp2_U8 & 0x40) != 0);
bb_5:
bb_6:
  /*$F80C*/ CYCLES(0xf80c, 3);
            ram_poke(0x002e, s_a);
            FUNC_PLOT1(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_PLOT1(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$F80E*/ CYCLES(0xf80e, 28);
            tmp1_U8 = s_y;
            tmp2_U8 = peek((ram_peek16al(0x0026) + tmp1_U8));
  /*$F814*/ tmp3_U8 = peek((ram_peek16al(0x0026) + tmp1_U8));
            tmp2_U8 = ((tmp2_U8 ^ ram_peek(0x0030)) & ram_peek(0x002e)) ^ tmp3_U8;
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_a = tmp2_U8;
  /*$F816*/ poke((ram_peek16al(0x0026) + tmp1_U8), tmp2_U8);
  /*$F818*/ if (ret_addr) pop16(); return;
}


void FUNC_HLINE(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$F819*/ CYCLES(0xf819, 6);
            FUNC_MON_PLOT(0xf81b);
            branchTarget = true;
bb_1:
  /*$F81C*/ CYCLES(0xf81c, 5);
            tmp1_U8 = s_y >= ram_peek(0x002c);
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_3;
bb_2:
  /*$F81E*/ CYCLES_EDGE(0xf81e, 1);
            branchTarget = true;
            goto bb_10;
bb_3:
  /*$F820*/ CYCLES(0xf820, 8);
            s_y = (uint8_t)(s_y + 0x01);
  /*$F821*/ FUNC_PLOT1(0xf823);
  /*$F824*/ CYCLES(0xf824, 2);
            branchTarget = true;
            if (s_status_c)
              goto bb_5;
bb_4:
  /*$F824*/ CYCLES_EDGE(0xf824, 1);
            branchTarget = true;
            goto bb_1;
bb_5:
  /*$F826*/ CYCLES(0xf826, 2);
            if (s_status_d)
              goto bb_7;
bb_6:
  /*$F826*/ tmp2_U16 = s_a;
            tmp3_U16 = (tmp2_U16 + 0x0001) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0x0001);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_8;
bb_7:
  /*$F826*/ tmp3_U16 = adc_dec16(s_a, 0x01, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_8:
  /*$F828*/ CYCLES(0xf828, 9);
            push8(s_a);
  /*$F829*/ FUNC_MON_PLOT(0xf82b);
  /*$F82C*/ CYCLES(0xf82c, 9);
            tmp1_U8 = pop8();
            s_a = tmp1_U8;
  /*$F82D*/ tmp1_U8 = tmp1_U8 >= ram_peek(0x002d);
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_10;
bb_9:
  /*$F82F*/ CYCLES_EDGE(0xf82f, 1);
            branchTarget = true;
            goto bb_5;
bb_10:
  /*$F831*/ CYCLES(0xf831, 6);
            if (ret_addr) pop16(); return;
}


void FUNC_VLINE(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$F828*/ CYCLES(0xf828, 9);
            push8(s_a);
  /*$F829*/ FUNC_MON_PLOT(0xf82b);
  /*$F82C*/ CYCLES(0xf82c, 9);
            tmp1_U8 = pop8();
            s_a = tmp1_U8;
  /*$F82D*/ tmp1_U8 = tmp1_U8 >= ram_peek(0x002d);
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_4;
            else
              goto bb_5;
bb_1:
  /*$F826*/ tmp2_U16 = s_a;
            tmp3_U16 = (tmp2_U16 + 0x0001) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0x0001);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_3;
bb_2:
  /*$F826*/ tmp3_U16 = adc_dec16(s_a, 0x01, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_3:
            goto bb_0;
bb_4:
  /*$F82F*/ CYCLES_EDGE(0xf82f, 1);
  /*$F826*/ CYCLES(0xf826, 2);
            if (s_status_d)
              goto bb_2;
            else
              goto bb_1;
bb_5:
  /*$F831*/ CYCLES(0xf831, 6);
            if (ret_addr) pop16(); return;
}


void FUNC_CLRTOP(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$F836*/ CYCLES(0xf836, 7);
  /*$F838*/ ram_poke(0x002d, 0x27);
  /*$F83A*/ s_y = 0x27;
bb_1:
  /*$F83C*/ CYCLES(0xf83c, 11);
            s_a = 0x00;
  /*$F83E*/ ram_poke(0x0030, 0x00);
  /*$F840*/ FUNC_VLINE(0xf842);
  /*$F843*/ CYCLES(0xf843, 4);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_3;
bb_2:
  /*$F844*/ CYCLES_EDGE(0xf844, 1);
            branchTarget = true;
            goto bb_1;
bb_3:
  /*$F846*/ CYCLES(0xf846, 6);
            if (ret_addr) pop16(); return;
}


void FUNC_GBASCALC(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$F847*/ CYCLES(0xf847, 20);
            tmp1_U8 = s_a;
            push8(tmp1_U8);
  /*$F848*/ tmp2_U8 = tmp1_U8 & 0x01;
            s_status_c = tmp2_U8;
  /*$F84D*/ ram_poke(0x0027, (((tmp1_U8 >> 0x01) & 0x03) | 0x04));
  /*$F84F*/ tmp1_U8 = pop8();
  /*$F850*/ s_a = (tmp1_U8 & 0x18);
            branchTarget = true;
            if (tmp2_U8)
              goto bb_2;
bb_1:
  /*$F852*/ CYCLES_EDGE(0xf852, 1);
            branchTarget = true;
            goto bb_6;
bb_2:
  /*$F854*/ CYCLES(0xf854, 2);
            if (s_status_d)
              goto bb_4;
bb_3:
  /*$F854*/ s_a = (uint8_t)((s_a + 0x007f) + s_status_c);
            goto bb_5;
bb_4:
  /*$F854*/ s_a = ((uint8_t)adc_dec16(s_a, 0x7f, s_status_c));
bb_5:
bb_6:
  /*$F856*/ CYCLES(0xf856, 19);
            tmp2_U8 = s_a;
            ram_poke(0x0026, tmp2_U8);
  /*$F85C*/ ram_poke(0x0026, ((uint8_t)(tmp2_U8 << 0x02) | ram_peek(0x0026)));
  /*$F85E*/ if (ret_addr) pop16(); return;
}


void FUNC_SETCOL(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$F864*/ CYCLES(0xf864, 25);
            tmp1_U8 = s_a & 0x0f;
  /*$F866*/ ram_poke(0x0030, tmp1_U8);
  /*$F86B*/ tmp2_U16 = tmp1_U8 << 0x04;
            s_status_c = (uint8_t)((tmp2_U16 & 0x01ff) >> 8);
  /*$F86C*/ tmp1_U8 = ((uint8_t)tmp2_U16) | ram_peek(0x0030);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
  /*$F86E*/ ram_poke(0x0030, tmp1_U8);
  /*$F870*/ if (ret_addr) pop16(); return;
}


void FUNC_SCRN(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$F871*/ CYCLES(0xf871, 11);
            tmp1_U8 = s_a;
            tmp2_U8 = tmp1_U8 >> 0x01;
            s_a = tmp2_U8;
  /*$F872*/ push8(((tmp1_U8 & 0x01) | ((tmp2_U8 == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | (tmp2_U8 & 0x80)));
  /*$F873*/ FUNC_GBASCALC(0xf875);
  /*$F876*/ CYCLES(0xf876, 11);
            tmp1_U8 = peek((ram_peek16al(0x0026) + s_y));
            s_a = tmp1_U8;
  /*$F878*/ tmp1_U8 = pop8();
            tmp2_U8 = tmp1_U8 & 0x01;
            s_status_c = tmp2_U8;
            s_status_i = ((tmp1_U8 & 0x04) != 0);
            s_status_d = ((tmp1_U8 & 0x08) != 0);
            s_status_b = 0x00;
            s_status_v = ((tmp1_U8 & 0x40) != 0);
            branchTarget = true;
            if (tmp2_U8)
              goto bb_2;
bb_1:
  /*$F879*/ CYCLES_EDGE(0xf879, 1);
            branchTarget = true;
            goto bb_3;
bb_2:
  /*$F87B*/ CYCLES(0xf87b, 8);
            tmp2_U8 = s_a;
  /*$F87E*/ s_status_c = ((tmp2_U8 >> 0x03) & 0x01);
            s_a = (tmp2_U8 >> 0x04);
bb_3:
  /*$F87F*/ CYCLES(0xf87f, 8);
            tmp2_U8 = s_a & 0x0f;
            s_status_not_z = tmp2_U8;
            s_status_n = 0x00;
            s_a = tmp2_U8;
  /*$F881*/ if (ret_addr) pop16(); return;
}


void FUNC_INIT(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FB2F*/ CYCLES(0xfb2f, 21);
  /*$FB31*/ ram_poke(0x0048, 0x00);
  /*$FB33*/ tmp1_U8 = io_peek(0xc056);
  /*$FB36*/ tmp1_U8 = io_peek(0xc054);
  /*$FB39*/ tmp1_U8 = io_peek(0xc051);
  /*$FB3E*/ CYCLES_EDGE(0xfb3e, 1);
  /*$FB4B*/ CYCLES(0xfb4b, 26);
            ram_poke(0x0022, 0x00);
  /*$FB4F*/ ram_poke(0x0020, 0x00);
  /*$FB53*/ ram_poke(0x0021, 0x28);
  /*$FB57*/ ram_poke(0x0023, 0x18);
  /*$FB5B*/ ram_poke(0x0025, 0x17);
  /*$FC22*/ CYCLES(0xfc22, 3);
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
            FUNC_HOME(0xfb62);
  /*$FB63*/ CYCLES(0xfb63, 2);
            s_y = 0x08;
bb_1:
  /*$FB65*/ CYCLES(0xfb65, 13);
            tmp1_U8 = s_y;
  /*$FB68*/ ram_poke((0x040e + tmp1_U8), ram_peek((0xfb08 + tmp1_U8)));
  /*$FB6B*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_3;
bb_2:
  /*$FB6C*/ CYCLES_EDGE(0xfb6c, 1);
            branchTarget = true;
            goto bb_1;
bb_3:
  /*$FB6E*/ CYCLES(0xfb6e, 6);
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
  /*$FBC1*/ CYCLES(0xfbc1, 20);
            tmp1_U8 = s_a;
            push8(tmp1_U8);
  /*$FBC2*/ tmp2_U8 = tmp1_U8 & 0x01;
            s_status_c = tmp2_U8;
  /*$FBC7*/ ram_poke(0x0029, (((tmp1_U8 >> 0x01) & 0x03) | 0x04));
  /*$FBC9*/ tmp1_U8 = pop8();
  /*$FBCA*/ s_a = (tmp1_U8 & 0x18);
            branchTarget = true;
            if (tmp2_U8)
              goto bb_2;
bb_1:
  /*$FBCC*/ CYCLES_EDGE(0xfbcc, 1);
            branchTarget = true;
            goto bb_6;
bb_2:
  /*$FBCE*/ CYCLES(0xfbce, 2);
            if (s_status_d)
              goto bb_4;
bb_3:
  /*$FBCE*/ tmp3_U16 = s_a;
            tmp4_U16 = (tmp3_U16 + 0x007f) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x007f);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_5;
bb_4:
  /*$FBCE*/ tmp4_U16 = adc_dec16(s_a, 0x7f, s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
bb_5:
bb_6:
  /*$FBD0*/ CYCLES(0xfbd0, 19);
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
            FUNC_BASCALC(0xfc26);
  /*$FC27*/ CYCLES(0xfc27, 6);
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
  /*$FC2B*/ CYCLES(0xfc2b, 6);
            if (ret_addr) pop16(); return;
}


void FUNC_HOME(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FC58*/ CYCLES(0xfc58, 13);
            tmp1_U8 = ram_peek(0x0022);
            s_a = tmp1_U8;
  /*$FC5A*/ ram_poke(0x0025, tmp1_U8);
  /*$FC5C*/ s_y = 0x00;
  /*$FC5E*/ ram_poke(0x0024, 0x00);
  /*$FC60*/ CYCLES_EDGE(0xfc60, 1);
            branchTarget = true;
bb_1:
  /*$FC46*/ CYCLES(0xfc46, 9);
            push8(s_a);
  /*$FC47*/ FUNC_VTABZ(0xfc49);
  /*$FC4A*/ CYCLES(0xfc4a, 6);
            FUNC_CLREOLZ(0xfc4c);
  /*$FC4D*/ CYCLES(0xfc4d, 13);
            s_y = 0x00;
  /*$FC4F*/ tmp1_U8 = pop8();
            s_a = tmp1_U8;
            if (s_status_d)
              goto bb_3;
bb_2:
  /*$FC50*/ s_a = (uint8_t)(s_a + s_status_c);
            goto bb_4;
bb_3:
  /*$FC50*/ s_a = ((uint8_t)adc_dec16(s_a, 0x00, s_status_c));
bb_4:
  /*$FC52*/ tmp1_U8 = s_a >= ram_peek(0x0023);
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_6;
bb_5:
  /*$FC54*/ CYCLES_EDGE(0xfc54, 1);
            branchTarget = true;
            goto bb_1;
bb_6:
  /*$FC56*/ CYCLES(0xfc56, 2);
            branchTarget = true;
            if (!s_status_c)
              goto bb_0;
bb_7:
  /*$FC56*/ CYCLES_EDGE(0xfc56, 1);
  /*$FC22*/ CYCLES(0xfc22, 3);
            s_a = ram_peek(0x0025);
            FUNC_VTABZ(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_CLREOL(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FC9C*/ CYCLES(0xfc9c, 3);
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
  /*$FC9E*/ CYCLES(0xfc9e, 2);
            s_a = 0xa0;
bb_1:
  /*$FCA0*/ CYCLES(0xfca0, 13);
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
            if (tmp3_U8)
              goto bb_3;
bb_2:
  /*$FCA5*/ CYCLES_EDGE(0xfca5, 1);
            branchTarget = true;
            goto bb_1;
bb_3:
  /*$FCA7*/ CYCLES(0xfca7, 6);
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
  /*$FCA9*/ CYCLES(0xfca9, 3);
            push8(s_a);
bb_2:
  /*$FCAA*/ CYCLES(0xfcaa, 4);
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
            if (!s_status_not_z)
              goto bb_7;
bb_6:
  /*$FCAC*/ CYCLES_EDGE(0xfcac, 1);
            branchTarget = true;
            goto bb_2;
bb_7:
  /*$FCAE*/ CYCLES(0xfcae, 8);
            tmp2_U8 = pop8();
            s_a = tmp2_U8;
            if (s_status_d)
              goto bb_9;
bb_8:
  /*$FCAF*/ tmp1_U16 = s_a;
            tmp3_U16 = (tmp1_U16 - 0x0001) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp1_U16, (uint8_t)0xfffe);
            tmp2_U8 = (uint8_t)tmp3_U16;
            s_status_not_z = tmp2_U8;
            s_a = tmp2_U8;
            goto bb_10;
bb_9:
  /*$FCAF*/ tmp3_U16 = sbc_dec16(s_a, 0x01, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
            s_status_c = (tmp2_U8 & 0x01);
            s_status_not_z = (~tmp2_U8 & 2);
            s_status_v = ((tmp2_U8 & 0x40) != 0);
bb_10:
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_12;
bb_11:
  /*$FCB1*/ CYCLES_EDGE(0xfcb1, 1);
            branchTarget = true;
            goto bb_1;
bb_12:
  /*$FCB3*/ CYCLES(0xfcb3, 6);
            if (ret_addr) pop16(); return;
}


void FUNC_SETNORM(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FE84*/ CYCLES(0xfe84, 11);
  /*$FE86*/ ram_poke(0x0032, 0xff);
  /*$FE88*/ if (ret_addr) pop16(); return;
}


void FUNC_SETKBD(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FE89*/ CYCLES(0xfe89, 11);
  /*$FE8B*/ ram_poke(0x003e, 0x00);
  /*$FE8D*/ s_x = 0x38;
  /*$FE8F*/ s_y = 0x1b;
  /*$FE91*/ CYCLES_EDGE(0xfe91, 1);
  /*$FE9B*/ CYCLES(0xfe9b, 7);
  /*$FE9D*/ tmp1_U8 = ram_peek(0x003e) & 0x0f;
            s_a = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_2;
bb_1:
  /*$FE9F*/ CYCLES_EDGE(0xfe9f, 1);
  /*$FEA7*/ CYCLES(0xfea7, 2);
            s_a = 0xfd;
            goto bb_3;
bb_2:
  /*$FEA1*/ CYCLES(0xfea1, 6);
            s_a = (s_a | 0xc0);
  /*$FEA3*/ s_y = 0x00;
  /*$FEA5*/ CYCLES_EDGE(0xfea5, 1);
            branchTarget = true;
bb_3:
  /*$FEA9*/ CYCLES(0xfea9, 14);
            tmp1_U8 = s_x;
            ram_poke(tmp1_U8, s_y);
  /*$FEAB*/ ram_poke((uint8_t)(0x01 + tmp1_U8), s_a);
  /*$FEAD*/ if (ret_addr) pop16(); return;
}


void FUNC_SETVID(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FE93*/ CYCLES(0xfe93, 9);
  /*$FE95*/ ram_poke(0x003e, 0x00);
  /*$FE97*/ s_x = 0x36;
  /*$FE99*/ s_y = 0xf0;
  /*$FE9B*/ CYCLES(0xfe9b, 7);
  /*$FE9D*/ tmp1_U8 = ram_peek(0x003e) & 0x0f;
            s_a = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_2;
bb_1:
  /*$FE9F*/ CYCLES_EDGE(0xfe9f, 1);
  /*$FEA7*/ CYCLES(0xfea7, 2);
            s_a = 0xfd;
            goto bb_3;
bb_2:
  /*$FEA1*/ CYCLES(0xfea1, 6);
            s_a = (s_a | 0xc0);
  /*$FEA3*/ s_y = 0x00;
  /*$FEA5*/ CYCLES_EDGE(0xfea5, 1);
            branchTarget = true;
bb_3:
  /*$FEA9*/ CYCLES(0xfea9, 14);
            tmp1_U8 = s_x;
            ram_poke(tmp1_U8, s_y);
  /*$FEAB*/ ram_poke((uint8_t)(0x01 + tmp1_U8), s_a);
  /*$FEAD*/ if (ret_addr) pop16(); return;
}


void FUNC_SAV1(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FF4C*/ CYCLES(0xff4c, 29);
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
                s_sp = 0xff;
                s_status_b = 0x00;
                s_status_i = 0x00;
      /*$FA62*/ CYCLES(0xfa62, 8);
                s_status_d = 0x00;
      /*$FA63*/ FUNC_SETNORM(0xfa65);
                branchTarget = true; block_id = 2410;
      break;
    case 1:  // $0090
      /*$0090*/ CYCLES(0x0090, 0);
                fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x0090, 0x0090, 0x03); error_handler(0x0090);
      break;
    case 2:  // $00B1
      /*$00B1*/ CYCLES(0x00b1, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x00b8) + 0x01);
                ram_poke(0x00b8, tmp1_U8);
      /*$00B3*/ branchTarget = true; block_id = tmp1_U8 ? 3 : 4;
      break;
    case 3:  // $00B3
      /*$00B3*/ CYCLES_EDGE(0x00b3, 1);
                branchTarget = true; block_id = 5;
      break;
    case 4:  // $00B5
      /*$00B5*/ CYCLES(0x00b5, 5);
                ram_poke(0x00b9, (uint8_t)(ram_peek(0x00b9) + 0x01));
                block_id = 5;
      break;
    case 5:  // $00B7
      /*$00B7*/ CYCLES(0x00b7, 8);
                tmp1_U8 = peek(ram_peek16al(0x00b8));
                s_a = tmp1_U8;
      /*$00BA*/ s_status_not_z = (tmp1_U8 != 0x3a);
                tmp2_U8 = tmp1_U8 >= 0x3a;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - 0x3a) & 0x80);
      /*$00BC*/ branchTarget = true; block_id = tmp2_U8 ? 6 : 7;
      break;
    case 6:  // $00BC
      /*$00BC*/ CYCLES_EDGE(0x00bc, 1);
                branchTarget = true; block_id = 15;
      break;
    case 7:  // $00BE
      /*$00BE*/ CYCLES(0x00be, 4);
                tmp2_U8 = s_a;
                tmp1_U8 = tmp2_U8 != 0x20;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp2_U8 >= 0x20);
                s_status_n = ((uint8_t)(tmp2_U8 - 0x20) & 0x80);
      /*$00C0*/ branchTarget = true; block_id = tmp1_U8 ? 9 : 8;
      break;
    case 8:  // $00C0
      /*$00C0*/ CYCLES_EDGE(0x00c0, 1);
                branchTarget = true; block_id = find_block_id_func_t001(0x00c0, (0x00c2 + ((int8_t)ram_peek(0x00c1))));
      break;
    case 9:  // $00C2
      /*$00C2*/ CYCLES(0x00c2, 8);
                s_status_c = 0x01;
      /*$00C3*/ block_id = s_status_d ? 11 : 10;
      break;
    case 10:  // $00C3
      /*$00C3*/ s_a = (uint8_t)((s_a - 0x0030) - (uint8_t)(0x01 - s_status_c));
                block_id = 12;
      break;
    case 11:  // $00C3
      /*$00C3*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x30, s_status_c));
                block_id = 12;
      break;
    case 12:  // $00C5
      /*$00C5*/ s_status_c = 0x01;
      /*$00C6*/ block_id = s_status_d ? 14 : 13;
      break;
    case 13:  // $00C6
      /*$00C6*/ tmp3_U16 = s_a;
                tmp4_U16 = (tmp3_U16 - 0x00d0) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0xff2f);
                tmp1_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 16;
      break;
    case 14:  // $00C6
      /*$00C6*/ tmp4_U16 = sbc_dec16(s_a, 0xd0, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
                block_id = 16;
      break;
    case 15:  // $00C8
      /*$00C8*/ CYCLES(0x00c8, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x00c8, pop16() + 1);;
      break;
    case 16:  // $00C8
      /*$00C6*/ block_id = 15;
      break;
    case 17:  // $3750
      /*$3750*/ CYCLES(0x3750, 2);
                s_x = 0x00;
                block_id = 18;
      break;
    case 18:  // $3752
      /*$3752*/ CYCLES(0x3752, 13);
                tmp1_U8 = s_x;
                tmp2_U8 = peek((ram_peek16(0x3753) + tmp1_U8));
      /*$3755*/ poke((ram_peek16al(0x3756) + tmp1_U8), tmp2_U8);
      /*$3758*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
                s_x = tmp1_U8;
      /*$3759*/ branchTarget = true; block_id = tmp1_U8 ? 19 : 20;
      break;
    case 19:  // $3759
      /*$3759*/ CYCLES_EDGE(0x3759, 1);
                branchTarget = true; block_id = 18;
      break;
    case 20:  // $375B
      /*$375B*/ CYCLES(0x375b, 20);
                ram_poke(0x3754, (uint8_t)(ram_peek(0x3754) + 0x01));
      /*$375E*/ ram_poke(0x3757, (uint8_t)(ram_peek(0x3757) + 0x01));
      /*$3761*/ tmp1_U8 = ram_peek(0x3757);
      /*$3764*/ s_status_c = (tmp1_U8 >= 0x20);
      /*$3766*/ branchTarget = true; block_id = (tmp1_U8 != 0x20) ? 21 : 22;
      break;
    case 21:  // $3766
      /*$3766*/ CYCLES_EDGE(0x3766, 1);
                branchTarget = true; block_id = 17;
      break;
    case 22:  // $3768
      /*$3768*/ CYCLES(0x3768, 6);
                FUNC_SETVID(0x376a);
                branchTarget = true; block_id = 23;
      break;
    case 23:  // $376B
      /*$376B*/ CYCLES(0x376b, 6);
                FUNC_SETKBD(0x376d);
                branchTarget = true; block_id = 24;
      break;
    case 24:  // $376E
      /*$376E*/ CYCLES(0x376e, 29);
      /*$3770*/ ram_poke(0x0300, 0x52);
      /*$3775*/ ram_poke(0x0301, 0x01);
      /*$3778*/ ram_poke(0x0302, 0x01);
      /*$377B*/ ram_poke(0x0303, 0x01);
      /*$3780*/ ram_poke(0x0304, 0x64);
      /*$3783*/ branchTarget = true; block_id = 703;
      break;
    case 25:  // $60E4
      /*$60E4*/ CYCLES(0x60e4, 6);
                func_6127(0x60e6);
                branchTarget = true; block_id = 27;
      break;
    case 26:  // $60E7
      /*$60E7*/ CYCLES(0x60e7, 22);
                s_x = 0x00;
      /*$60E9*/ tmp1_U8 = ram_peek(0x0003);
      /*$60EB*/ ram_poke(0x0007, 0x00);
      /*$60F0*/ ram_poke(0x0004, ram_peek((0x6000 + tmp1_U8)));
      /*$60F5*/ ram_poke(0x0005, ram_peek((0x6030 + tmp1_U8)));
                block_id = 28;
      break;
    case 27:  // $60E7
      /*$60E4*/ func_60e7(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x60e4, pop16() + 1);;
      break;
    case 28:  // $60F7
      /*$60F7*/ CYCLES(0x60f7, 16);
      /*$60F9*/ ram_poke(0x0006, ram_peek(0x0001));
      /*$60FE*/ ram_poke(0x0006, (uint8_t)((ram_peek(0x0006) << 0x01) | (ram_peek(0x0007) & 0x01)));
      /*$6100*/ CYCLES(0x6100, 62);
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
      /*$611E*/ block_id = s_status_d ? 30 : 29;
      break;
    case 29:  // $611E
      /*$611E*/ tmp4_U16 = s_a;
                tmp3_U16 = (tmp4_U16 + 0x0004) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0x0004);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 31;
      break;
    case 30:  // $611E
      /*$611E*/ tmp3_U16 = adc_dec16(s_a, 0x04, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
                block_id = 31;
      break;
    case 31:  // $6120
      /*$6120*/ ram_poke(0x0005, s_a);
      /*$6122*/ tmp2_U8 = s_x;
                tmp1_U8 = tmp2_U8 != 0x04;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp2_U8 >= 0x04);
                s_status_n = ((uint8_t)(tmp2_U8 - 0x04) & 0x80);
      /*$6124*/ branchTarget = true; block_id = tmp1_U8 ? 32 : 33;
      break;
    case 32:  // $6124
      /*$6124*/ CYCLES_EDGE(0x6124, 1);
                branchTarget = true; block_id = 28;
      break;
    case 33:  // $6126
      /*$6126*/ CYCLES(0x6126, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x6126, pop16() + 1);;
      break;
    case 34:  // $6127
      /*$6127*/ CYCLES(0x6127, 53);
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
    case 35:  // $6148
      /*$6148*/ CYCLES(0x6148, 6);
                func_6127(0x614a);
                branchTarget = true; block_id = 36;
      break;
    case 36:  // $614B
      /*$614B*/ CYCLES(0x614b, 6);
                func_60e7(0x614d);
                branchTarget = true; block_id = 37;
      break;
    case 37:  // $614E
      /*$614E*/ CYCLES(0x614e, 8);
                tmp1_U8 = ram_peek(0x0002);
                s_a = tmp1_U8;
      /*$6150*/ tmp2_U8 = ram_peek(0x0008);
                tmp5_U8 = tmp1_U8 != tmp2_U8;
                s_status_not_z = tmp5_U8;
                s_status_c = (tmp1_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
      /*$6152*/ branchTarget = true; block_id = !tmp5_U8 ? 38 : 39;
      break;
    case 38:  // $6152
      /*$6152*/ CYCLES_EDGE(0x6152, 1);
                branchTarget = true; block_id = 40;
      break;
    case 39:  // $6154
      /*$6154*/ CYCLES(0x6154, 8);
                ram_poke(0x0002, (uint8_t)(ram_peek(0x0002) + 0x01));
      /*$6156*/ branchTarget = true; block_id = 36;
      break;
    case 40:  // $6159
      /*$6159*/ CYCLES(0x6159, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x6159, pop16() + 1);;
      break;
    case 41:  // $615A
      /*$615A*/ CYCLES(0x615a, 6);
                func_6127(0x615c);
                branchTarget = true; block_id = 42;
      break;
    case 42:  // $615D
      /*$615D*/ CYCLES(0x615d, 6);
                func_60e7(0x615f);
                branchTarget = true; block_id = 43;
      break;
    case 43:  // $6160
      /*$6160*/ CYCLES(0x6160, 8);
                tmp5_U8 = ram_peek(0x0003);
                s_a = tmp5_U8;
      /*$6162*/ tmp2_U8 = ram_peek(0x0008);
                tmp1_U8 = tmp5_U8 != tmp2_U8;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp5_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp5_U8 - tmp2_U8) & 0x80);
      /*$6164*/ branchTarget = true; block_id = !tmp1_U8 ? 44 : 45;
      break;
    case 44:  // $6164
      /*$6164*/ CYCLES_EDGE(0x6164, 1);
                branchTarget = true; block_id = 40;
      break;
    case 45:  // $6166
      /*$6166*/ CYCLES(0x6166, 8);
                ram_poke(0x0003, (uint8_t)(ram_peek(0x0003) + 0x01));
      /*$6168*/ branchTarget = true; block_id = 42;
      break;
    case 46:  // $6200
      /*$6200*/ CYCLES(0x6200, 10);
                tmp1_U8 = ram_peek(0x624c);
                s_x = tmp1_U8;
      /*$6203*/ tmp1_U8 = ram_peek((0x623c + tmp1_U8));
                s_status_not_z = tmp1_U8;
                tmp2_U8 = tmp1_U8 & 0x80;
                s_status_n = tmp2_U8;
                s_a = tmp1_U8;
      /*$6206*/ branchTarget = true; block_id = !tmp2_U8 ? 47 : 48;
      break;
    case 47:  // $6206
      /*$6206*/ CYCLES_EDGE(0x6206, 1);
                branchTarget = true; block_id = 49;
      break;
    case 48:  // $6208
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
                block_id = 49;
      break;
    case 49:  // $6216
      /*$6216*/ CYCLES(0x6216, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x6216, pop16() + 1);;
      break;
    case 50:  // $6217
      /*$6217*/ CYCLES(0x6217, 10);
                s_x = ram_peek(0x624d);
      /*$621A*/ tmp2_U8 = io_peek(0xc000);
                s_status_not_z = tmp2_U8;
                tmp1_U8 = tmp2_U8 & 0x80;
                s_status_n = tmp1_U8;
                s_a = tmp2_U8;
      /*$621D*/ branchTarget = true; block_id = !tmp1_U8 ? 51 : 52;
      break;
    case 51:  // $621D
      /*$621D*/ CYCLES_EDGE(0x621d, 1);
                branchTarget = true; block_id = 49;
      break;
    case 52:  // $621F
      /*$621F*/ CYCLES(0x621f, 21);
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
      /*$622C*/ branchTarget = true; block_id = !tmp5_U8 ? 53 : 54;
      break;
    case 53:  // $622C
      /*$622C*/ CYCLES_EDGE(0x622c, 1);
                branchTarget = true; block_id = 49;
      break;
    case 54:  // $622E
      /*$622E*/ CYCLES(0x622e, 10);
                ram_poke(0x624d, s_a);
      /*$6231*/ branchTarget = true; block_id = find_block_id_func_t001(0x6231, pop16() + 1);;
      break;
    case 55:  // $6256
      /*$6256*/ CYCLES(0x6256, 8);
                s_a = 0x14;
      /*$6258*/ func_660f(0x625a);
                branchTarget = true; block_id = 56;
      break;
    case 56:  // $625B
      /*$625B*/ CYCLES(0x625b, 36);
                ram_poke(0x6251, s_a);
      /*$6260*/ ram_poke(0x6250, 0x27);
      /*$6263*/ ram_poke(0x6252, 0x27);
      /*$6268*/ ram_poke(0x624e, 0x02);
      /*$626D*/ ram_poke(0x6254, 0x0a);
      /*$6272*/ ram_poke(0x6255, 0x64);
      /*$6275*/ s_x = 0x0f;
      /*$6277*/ s_a = 0x00;
                block_id = 57;
      break;
    case 57:  // $6279
      /*$6279*/ CYCLES(0x6279, 9);
                tmp5_U8 = s_x;
                ram_poke((0x623c + tmp5_U8), s_a);
      /*$627C*/ tmp5_U8 = (uint8_t)(tmp5_U8 - 0x01);
                s_x = tmp5_U8;
      /*$627D*/ branchTarget = true; block_id = !(tmp5_U8 & 0x80) ? 58 : 59;
      break;
    case 58:  // $627D
      /*$627D*/ CYCLES_EDGE(0x627d, 1);
                branchTarget = true; block_id = 57;
      break;
    case 59:  // $627F
      /*$627F*/ CYCLES(0x627f, 11);
                tmp5_U8 = s_a;
                ram_poke(0x624c, tmp5_U8);
      /*$6282*/ ram_poke(0x624d, tmp5_U8);
      /*$6285*/ branchTarget = true; block_id = 60;
      break;
    case 60:  // $6288
      /*$6288*/ CYCLES(0x6288, 6);
                func_69c3(0x628a);
                branchTarget = true; block_id = 61;
      break;
    case 61:  // $628B
      /*$628B*/ CYCLES(0x628b, 6);
                func_6217(0x628d);
                branchTarget = true; block_id = 62;
      break;
    case 62:  // $628E
      /*$628E*/ CYCLES(0x628e, 6);
                func_6c72(0x6290);
                branchTarget = true; block_id = 63;
      break;
    case 63:  // $6291
      /*$6291*/ CYCLES(0x6291, 2);
                branchTarget = true; block_id = !s_status_n ? 64 : 65;
      break;
    case 64:  // $6291
      /*$6291*/ CYCLES_EDGE(0x6291, 1);
                branchTarget = true; block_id = 84;
      break;
    case 65:  // $6293
      /*$6293*/ CYCLES(0x6293, 10);
                s_x = 0x10;
      /*$6295*/ ram_poke(0x6473, 0x10);
      /*$6298*/ tmp5_U8 = s_a != 0x95;
                s_status_not_z = tmp5_U8;
      /*$629A*/ branchTarget = true; block_id = tmp5_U8 ? 66 : 67;
      break;
    case 66:  // $629A
      /*$629A*/ CYCLES_EDGE(0x629a, 1);
      /*$6306*/ CYCLES(0x6306, 2);
                branchTarget = true; block_id = s_status_not_z ? 83 : 84;
      break;
    case 67:  // $629C
      /*$629C*/ CYCLES(0x629c, 14);
                s_a = ram_peek(0x624e);
      /*$629F*/ ram_poke(0x624e, (uint8_t)(ram_peek(0x624e) - 0x01));
      /*$62A2*/ s_status_c = 0x00;
      /*$62A3*/ block_id = s_status_d ? 69 : 68;
      break;
    case 68:  // $62A3
      /*$62A3*/ s_a = (uint8_t)((s_a + 0x0010) + s_status_c);
                block_id = 70;
      break;
    case 69:  // $62A3
      /*$62A3*/ s_a = ((uint8_t)adc_dec16(s_a, 0x10, s_status_c));
                block_id = 70;
      break;
    case 70:  // $62A5
      /*$62A3*/ block_id = 71;
      break;
    case 71:  // $62A5
      /*$62A5*/ CYCLES(0x62a5, 28);
                ram_poke(0x0000, s_a);
      /*$62A9*/ ram_poke(0x0001, 0x0c);
      /*$62AE*/ ram_poke(0x0002, ram_peek(0x624f));
      /*$62B3*/ ram_poke(0x0003, ram_peek(0x6250));
      /*$62B5*/ func_6bda(0x62b7);
                branchTarget = true; block_id = 72;
      break;
    case 72:  // $62B8
      /*$62B8*/ CYCLES(0x62b8, 26);
      /*$62C0*/ tmp5_U8 = (uint8_t)(((uint8_t)(ram_peek(0x624e) - 0x01) & 0x03) + 0x01);
      /*$62C1*/ ram_poke(0x624e, tmp5_U8);
      /*$62C4*/ s_a = tmp5_U8;
      /*$62C5*/ FUNC_SETCOL(0x62c7);
                branchTarget = true; block_id = 73;
      break;
    case 73:  // $62C8
      /*$62C8*/ CYCLES(0x62c8, 14);
                s_y = ram_peek(0x624f);
      /*$62CB*/ s_a = ram_peek(0x6250);
      /*$62CE*/ FUNC_MON_PLOT(0x62d0);
                branchTarget = true; block_id = 74;
      break;
    case 74:  // $62D1
      /*$62D1*/ CYCLES(0x62d1, 42);
                s_x = ram_peek(0x624e);
      /*$62D4*/ s_a = ram_peek(0x624f);
      /*$62D7*/ s_status_c = 0x00;
      /*$62D8*/ block_id = s_status_d ? 76 : 75;
      break;
    case 75:  // $62D8
      /*$62D8*/ s_a = (uint8_t)((s_a + ram_peek((0x6232 + s_x))) + s_status_c);
                block_id = 77;
      break;
    case 76:  // $62D8
      /*$62D8*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek((0x6232 + s_x)), s_status_c));
                block_id = 77;
      break;
    case 77:  // $62DB
      /*$62DB*/ ram_poke(0x624f, s_a);
      /*$62DE*/ s_a = ram_peek(0x6250);
      /*$62E1*/ s_status_c = 0x00;
      /*$62E2*/ block_id = s_status_d ? 79 : 78;
      break;
    case 78:  // $62E2
      /*$62E2*/ tmp3_U16 = s_a;
                tmp4_U16 = ram_peek((0x6237 + s_x));
                tmp6_U16 = (tmp3_U16 + tmp4_U16) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 80;
      break;
    case 79:  // $62E2
      /*$62E2*/ tmp6_U16 = adc_dec16(s_a, ram_peek((0x6237 + s_x)), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 80;
      break;
    case 80:  // $62E5
      /*$62E5*/ ram_poke(0x6250, s_a);
      /*$62E8*/ s_y = ram_peek(0x624f);
      /*$62EB*/ FUNC_SCRN(0x62ed);
                branchTarget = true; block_id = 81;
      break;
    case 81:  // $62EE
      /*$62EE*/ CYCLES(0x62ee, 25);
                ram_poke(0x6253, s_a);
      /*$62F4*/ ram_poke(0x0002, ram_peek(0x624f));
      /*$62F9*/ ram_poke(0x0003, ram_peek(0x6250));
      /*$62FE*/ ram_poke(0x0000, ram_peek(0x624e));
      /*$6300*/ CYCLES(0x6300, 6);
                func_60e4(0x6302);
                branchTarget = true; block_id = 82;
      break;
    case 82:  // $6303
      /*$6303*/ CYCLES(0x6303, 3);
      /*$6474*/ CYCLES(0x6474, 6);
                tmp1_U8 = ram_peek(0x6253);
                s_a = tmp1_U8;
      /*$6477*/ branchTarget = true; block_id = tmp1_U8 ? 150 : 151;
      break;
    case 83:  // $6306
      /*$6306*/ CYCLES_EDGE(0x6306, 1);
      /*$631E*/ CYCLES(0x631e, 4);
      /*$6320*/ branchTarget = true; block_id = (s_a != 0x88) ? 94 : 95;
      break;
    case 84:  // $6308
      /*$6308*/ CYCLES(0x6308, 6);
                tmp5_U8 = ram_peek(0x0302);
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$630B*/ branchTarget = true; block_id = !tmp5_U8 ? 85 : 86;
      break;
    case 85:  // $630B
      /*$630B*/ CYCLES_EDGE(0x630b, 1);
                branchTarget = true; block_id = 90;
      break;
    case 86:  // $630D
      /*$630D*/ CYCLES(0x630d, 6);
                branchTarget = true; push16(0x630f); block_id = 267;
      break;
    case 87:  // $6310
      /*$6310*/ CYCLES(0x6310, 2);
                branchTarget = true; block_id = !s_status_n ? 88 : 89;
      break;
    case 88:  // $6310
      /*$6310*/ CYCLES_EDGE(0x6310, 1);
                branchTarget = true; block_id = 90;
      break;
    case 89:  // $6312
      /*$6312*/ CYCLES(0x6312, 3);
                branchTarget = true; block_id = 65;
      break;
    case 90:  // $6315
      /*$6315*/ CYCLES(0x6315, 11);
                s_a = ram_peek(0x624e);
      /*$6318*/ s_status_c = 0x00;
      /*$6319*/ block_id = s_status_d ? 92 : 91;
      break;
    case 91:  // $6319
      /*$6319*/ s_a = (uint8_t)((s_a + 0x0008) + s_status_c);
                block_id = 93;
      break;
    case 92:  // $6319
      /*$6319*/ s_a = ((uint8_t)adc_dec16(s_a, 0x08, s_status_c));
                block_id = 93;
      break;
    case 93:  // $631B
      /*$631B*/ branchTarget = true; block_id = 71;
      break;
    case 94:  // $6320
      /*$6320*/ CYCLES_EDGE(0x6320, 1);
      /*$6349*/ CYCLES(0x6349, 4);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x92);
      /*$634B*/ branchTarget = true; block_id = (tmp1_U8 != 0x92) ? 105 : 106;
      break;
    case 95:  // $6322
      /*$6322*/ CYCLES(0x6322, 17);
                s_a = ram_peek(0x624e);
      /*$6325*/ ram_poke(0x624e, (uint8_t)(ram_peek(0x624e) + 0x01));
      /*$6328*/ s_status_c = 0x00;
      /*$6329*/ block_id = s_status_d ? 97 : 96;
      break;
    case 96:  // $6329
      /*$6329*/ s_a = (uint8_t)((s_a + 0x0004) + s_status_c);
                block_id = 98;
      break;
    case 97:  // $6329
      /*$6329*/ s_a = ((uint8_t)adc_dec16(s_a, 0x04, s_status_c));
                block_id = 98;
      break;
    case 98:  // $632B
      /*$632B*/ branchTarget = true; block_id = 71;
      break;
    case 99:  // $6333
      /*$6333*/ CYCLES(0x6333, 14);
                s_a = ram_peek(0x6250);
      /*$6336*/ s_y = ram_peek(0x624f);
      /*$6339*/ FUNC_MON_PLOT(0x633b);
                branchTarget = true; block_id = 100;
      break;
    case 100:  // $633C
      /*$633C*/ CYCLES(0x633c, 8);
                tmp1_U8 = ram_peek(0x624f);
      /*$633F*/ s_status_c = (tmp1_U8 >= 0x14);
      /*$6341*/ branchTarget = true; block_id = (tmp1_U8 != 0x14) ? 101 : 102;
      break;
    case 101:  // $6341
      /*$6341*/ CYCLES_EDGE(0x6341, 1);
                branchTarget = true; block_id = 115;
      break;
    case 102:  // $6343
      /*$6343*/ CYCLES(0x6343, 6);
                tmp1_U8 = ram_peek(0x6250);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$6346*/ branchTarget = true; block_id = tmp1_U8 ? 103 : 104;
      break;
    case 103:  // $6346
      /*$6346*/ CYCLES_EDGE(0x6346, 1);
                branchTarget = true; block_id = 115;
      break;
    case 104:  // $6348
      /*$6348*/ CYCLES(0x6348, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x6348, pop16() + 1);;
      break;
    case 105:  // $634B
      /*$634B*/ CYCLES_EDGE(0x634b, 1);
      /*$6353*/ CYCLES(0x6353, 4);
      /*$6355*/ branchTarget = true; block_id = (s_a != 0xc9) ? 107 : 108;
      break;
    case 106:  // $634D
      /*$634D*/ CYCLES(0x634d, 12);
                s_status_not_z = 0xff;
                s_status_n = 0x80;
                s_a = 0xff;
      /*$634F*/ ram_poke(0x6253, 0xff);
      /*$6352*/ branchTarget = true; block_id = find_block_id_func_t001(0x6352, pop16() + 1);;
      break;
    case 107:  // $6355
      /*$6355*/ CYCLES_EDGE(0x6355, 1);
      /*$6360*/ CYCLES(0x6360, 4);
      /*$6362*/ branchTarget = true; block_id = (s_a != 0xca) ? 109 : 110;
      break;
    case 108:  // $6357
      /*$6357*/ CYCLES(0x6357, 11);
      /*$635A*/ tmp1_U8 = ram_peek((0x6387 + ram_peek(0x624e)));
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$635D*/ branchTarget = true; block_id = 63;
      break;
    case 109:  // $6362
      /*$6362*/ CYCLES_EDGE(0x6362, 1);
      /*$636D*/ CYCLES(0x636d, 4);
      /*$636F*/ branchTarget = true; block_id = (s_a != 0xcb) ? 111 : 112;
      break;
    case 110:  // $6364
      /*$6364*/ CYCLES(0x6364, 11);
      /*$6367*/ tmp1_U8 = ram_peek((0x638c + ram_peek(0x624e)));
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$636A*/ branchTarget = true; block_id = 63;
      break;
    case 111:  // $636F
      /*$636F*/ CYCLES_EDGE(0x636f, 1);
      /*$637A*/ CYCLES(0x637a, 4);
      /*$637C*/ branchTarget = true; block_id = (s_a != 0xcd) ? 113 : 114;
      break;
    case 112:  // $6371
      /*$6371*/ CYCLES(0x6371, 11);
      /*$6374*/ tmp1_U8 = ram_peek((0x6391 + ram_peek(0x624e)));
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$6377*/ branchTarget = true; block_id = 63;
      break;
    case 113:  // $637C
      /*$637C*/ CYCLES_EDGE(0x637c, 1);
      /*$639B*/ CYCLES(0x639b, 6);
                func_69a9(0x639d);
      /*$639E*/ CYCLES(0x639e, 3);
                branchTarget = true; block_id = 133;
      break;
    case 114:  // $637E
      /*$637E*/ CYCLES(0x637e, 11);
      /*$6381*/ tmp1_U8 = ram_peek((0x6396 + ram_peek(0x624e)));
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$6384*/ branchTarget = true; block_id = 63;
      break;
    case 115:  // $63A1
      /*$63A1*/ CYCLES(0x63a1, 6);
      /*$63A4*/ branchTarget = true; block_id = !ram_peek(0x6254) ? 116 : 117;
      break;
    case 116:  // $63A4
      /*$63A4*/ CYCLES_EDGE(0x63a4, 1);
      /*$63B1*/ CYCLES(0x63b1, 14);
                s_a = ram_peek(0x6252);
      /*$63B4*/ s_y = ram_peek(0x6251);
      /*$63B7*/ FUNC_SCRN(0x63b9);
                branchTarget = true; block_id = 118;
      break;
    case 117:  // $63A6
      /*$63A6*/ CYCLES(0x63a6, 15);
                ram_poke(0x6254, (uint8_t)(ram_peek(0x6254) - 0x01));
      /*$63A9*/ s_x = 0x07;
      /*$63AB*/ ram_poke(0x6473, 0x07);
      /*$63AE*/ branchTarget = true; block_id = 133;
      break;
    case 118:  // $63BA
      /*$63BA*/ CYCLES(0x63ba, 11);
                push8(s_a);
      /*$63BB*/ s_a = 0x00;
      /*$63BD*/ FUNC_SETCOL(0x63bf);
                branchTarget = true; block_id = 119;
      break;
    case 119:  // $63C0
      /*$63C0*/ CYCLES(0x63c0, 14);
                s_a = ram_peek(0x6252);
      /*$63C3*/ s_y = ram_peek(0x6251);
      /*$63C6*/ FUNC_MON_PLOT(0x63c8);
                branchTarget = true; block_id = 120;
      break;
    case 120:  // $63C9
      /*$63C9*/ CYCLES(0x63c9, 25);
      /*$63CB*/ ram_poke(0x0001, 0x00);
      /*$63D0*/ ram_poke(0x0003, ram_peek(0x6252));
      /*$63D5*/ ram_poke(0x0002, ram_peek(0x6251));
      /*$63D7*/ func_60e4(0x63d9);
                branchTarget = true; block_id = 121;
      break;
    case 121:  // $63DA
      /*$63DA*/ CYCLES(0x63da, 44);
                tmp1_U8 = pop8();
      /*$63DB*/ s_x = tmp1_U8;
      /*$63DC*/ s_a = ram_peek(0x6251);
      /*$63DF*/ s_status_c = 0x00;
      /*$63E0*/ block_id = s_status_d ? 123 : 122;
      break;
    case 122:  // $63E0
      /*$63E0*/ s_a = (uint8_t)((s_a + ram_peek((0x6232 + s_x))) + s_status_c);
                block_id = 124;
      break;
    case 123:  // $63E0
      /*$63E0*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek((0x6232 + s_x)), s_status_c));
                block_id = 124;
      break;
    case 124:  // $63E3
      /*$63E3*/ ram_poke(0x6251, s_a);
      /*$63E6*/ s_a = ram_peek(0x6252);
      /*$63E9*/ s_status_c = 0x00;
      /*$63EA*/ block_id = s_status_d ? 126 : 125;
      break;
    case 125:  // $63EA
      /*$63EA*/ tmp6_U16 = s_a;
                tmp4_U16 = ram_peek((0x6237 + s_x));
                tmp3_U16 = (tmp6_U16 + tmp4_U16) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 127;
      break;
    case 126:  // $63EA
      /*$63EA*/ tmp3_U16 = adc_dec16(s_a, ram_peek((0x6237 + s_x)), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
                block_id = 127;
      break;
    case 127:  // $63ED
      /*$63ED*/ ram_poke(0x6252, s_a);
      /*$63F0*/ s_y = ram_peek(0x6251);
      /*$63F3*/ FUNC_SCRN(0x63f5);
                branchTarget = true; block_id = 128;
      break;
    case 128:  // $63F6
      /*$63F6*/ CYCLES(0x63f6, 32);
                s_status_c = 0x00;
      /*$63F7*/ block_id = s_status_d ? 130 : 129;
      break;
    case 129:  // $63F7
      /*$63F7*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
                block_id = 131;
      break;
    case 130:  // $63F7
      /*$63F7*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
                block_id = 131;
      break;
    case 131:  // $63F9
      /*$63F9*/ ram_poke(0x0000, s_a);
      /*$63FD*/ ram_poke(0x0001, 0x0c);
      /*$6402*/ ram_poke(0x0002, ram_peek(0x6251));
      /*$6407*/ ram_poke(0x0003, ram_peek(0x6252));
      /*$6409*/ func_60e4(0x640b);
                branchTarget = true; block_id = 132;
      break;
    case 132:  // $640C
      /*$640C*/ CYCLES(0x640c, 3);
                branchTarget = true; block_id = 133;
      break;
    case 133:  // $640F
      /*$640F*/ CYCLES(0x640f, 20);
                tmp1_U8 = ram_peek(0x6c49);
                s_y = tmp1_U8;
      /*$6412*/ tmp1_U8 = peek((0xc000 + tmp1_U8));
      /*$6417*/ tmp1_U8 = (uint8_t)(ram_peek(0x6255) - 0x01);
                ram_poke(0x6255, tmp1_U8);
      /*$641A*/ branchTarget = true; block_id = tmp1_U8 ? 134 : 135;
      break;
    case 134:  // $641A
      /*$641A*/ CYCLES_EDGE(0x641a, 1);
      /*$6422*/ CYCLES(0x6422, 10);
      /*$6426*/ func_6b3d(0x6428);
                branchTarget = true; block_id = 136;
      break;
    case 135:  // $641C
      /*$641C*/ CYCLES(0x641c, 12);
                s_status_not_z = 0xfe;
                s_status_n = 0x80;
                s_a = 0xfe;
      /*$641E*/ ram_poke(0x6253, 0xfe);
      /*$6421*/ branchTarget = true; block_id = find_block_id_func_t001(0x6421, pop16() + 1);;
      break;
    case 136:  // $6429
      /*$6429*/ CYCLES(0x6429, 4);
      /*$642B*/ branchTarget = true; block_id = (s_a != 0x00) ? 137 : 138;
      break;
    case 137:  // $642B
      /*$642B*/ CYCLES_EDGE(0x642b, 1);
                branchTarget = true; block_id = 141;
      break;
    case 138:  // $642D
      /*$642D*/ CYCLES(0x642d, 31);
      /*$642F*/ ram_poke(0x0000, 0x15);
      /*$6433*/ ram_poke(0x0001, 0x0d);
      /*$6437*/ ram_poke(0x0003, 0x27);
      /*$643B*/ ram_poke(0x0002, 0x12);
      /*$643F*/ ram_poke(0x0008, 0x16);
      /*$6441*/ func_6148(0x6443);
                branchTarget = true; block_id = 139;
      break;
    case 139:  // $6444
      /*$6444*/ CYCLES(0x6444, 8);
                s_a = 0x0d;
      /*$6446*/ FUNC_SETCOL(0x6448);
                branchTarget = true; block_id = 140;
      break;
    case 140:  // $6449
      /*$6449*/ CYCLES(0x6449, 10);
                s_a = 0x27;
      /*$644B*/ s_y = 0x14;
      /*$644D*/ FUNC_MON_PLOT(0x644f);
                branchTarget = true; block_id = 141;
      break;
    case 141:  // $6450
      /*$6450*/ CYCLES(0x6450, 4);
                s_x = ram_peek(0x0300);
                block_id = 142;
      break;
    case 142:  // $6453
      /*$6453*/ CYCLES(0x6453, 6);
                func_6bfb(0x6455);
                branchTarget = true; block_id = 143;
      break;
    case 143:  // $6456
      /*$6456*/ CYCLES(0x6456, 11);
      /*$6457*/ push8(s_x);
      /*$6458*/ func_6217(0x645a);
                branchTarget = true; block_id = 144;
      break;
    case 144:  // $645B
      /*$645B*/ CYCLES(0x645b, 6);
      /*$645E*/ branchTarget = true; block_id = !ram_peek(0x6473) ? 145 : 146;
      break;
    case 145:  // $645E
      /*$645E*/ CYCLES_EDGE(0x645e, 1);
                branchTarget = true; block_id = 147;
      break;
    case 146:  // $6460
      /*$6460*/ CYCLES(0x6460, 18);
                tmp1_U8 = ram_peek(0x6c49);
                s_y = tmp1_U8;
      /*$6463*/ tmp1_U8 = peek((0xc000 + tmp1_U8));
      /*$6468*/ ram_poke(0x6473, (uint8_t)(ram_peek(0x6473) - 0x01));
                block_id = 147;
      break;
    case 147:  // $646B
      /*$646B*/ CYCLES(0x646b, 10);
                tmp1_U8 = pop8();
      /*$646D*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_x = tmp1_U8;
      /*$646E*/ branchTarget = true; block_id = tmp1_U8 ? 148 : 149;
      break;
    case 148:  // $646E
      /*$646E*/ CYCLES_EDGE(0x646e, 1);
                branchTarget = true; block_id = 142;
      break;
    case 149:  // $6470
      /*$6470*/ CYCLES(0x6470, 3);
                branchTarget = true; block_id = 61;
      break;
    case 150:  // $6477
      /*$6477*/ CYCLES_EDGE(0x6477, 1);
      /*$647C*/ CYCLES(0x647c, 4);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x0f);
      /*$647E*/ branchTarget = true; block_id = (tmp1_U8 != 0x0f) ? 152 : 153;
      break;
    case 151:  // $6479
      /*$6479*/ CYCLES(0x6479, 3);
      /*$632E*/ CYCLES(0x632e, 8);
                s_a = 0x07;
      /*$6330*/ FUNC_SETCOL(0x6332);
                branchTarget = true; block_id = 99;
      break;
    case 152:  // $647E
      /*$647E*/ CYCLES_EDGE(0x647e, 1);
      /*$6494*/ CYCLES(0x6494, 6);
      /*$6496*/ s_x = 0xff;
                block_id = 156;
      break;
    case 153:  // $6480
      /*$6480*/ CYCLES(0x6480, 14);
                s_x = 0x20;
      /*$6482*/ ram_poke(0x6473, 0x20);
      /*$6485*/ s_a = 0x07;
      /*$6487*/ FUNC_SETCOL(0x6489);
                branchTarget = true; block_id = 154;
      break;
    case 154:  // $648A
      /*$648A*/ CYCLES(0x648a, 14);
                s_a = ram_peek(0x6250);
      /*$648D*/ s_y = ram_peek(0x624f);
      /*$6490*/ func_6bef(0x6492);
                branchTarget = true; block_id = 155;
      break;
    case 155:  // $6493
      /*$6493*/ CYCLES(0x6493, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x6493, pop16() + 1);;
      break;
    case 156:  // $6498
      /*$6498*/ CYCLES(0x6498, 6);
      /*$649B*/ s_y = ram_peek((0xe000 + s_x));
                block_id = 157;
      break;
    case 157:  // $649C
      /*$649C*/ CYCLES(0x649c, 4);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp1_U8;
      /*$649D*/ branchTarget = true; block_id = tmp1_U8 ? 158 : 159;
      break;
    case 158:  // $649D
      /*$649D*/ CYCLES_EDGE(0x649d, 1);
                branchTarget = true; block_id = 157;
      break;
    case 159:  // $649F
      /*$649F*/ CYCLES(0x649f, 12);
                tmp1_U8 = ram_peek(0x6c49);
                s_y = tmp1_U8;
      /*$64A2*/ tmp1_U8 = peek((0xc000 + tmp1_U8));
                s_a = tmp1_U8;
      /*$64A5*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$64A6*/ branchTarget = true; block_id = tmp1_U8 ? 160 : 161;
      break;
    case 160:  // $64A6
      /*$64A6*/ CYCLES_EDGE(0x64a6, 1);
                branchTarget = true; block_id = 156;
      break;
    case 161:  // $64A8
      /*$64A8*/ CYCLES(0x64a8, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x64a8, pop16() + 1);;
      break;
    case 162:  // $64A9
      /*$64A9*/ CYCLES(0x64a9, 2);
                s_x = 0x00;
                block_id = 163;
      break;
    case 163:  // $64AB
      /*$64AB*/ CYCLES(0x64ab, 4);
      /*$64AC*/ s_y = s_x;
                block_id = 164;
      break;
    case 164:  // $64AD
      /*$64AD*/ CYCLES(0x64ad, 4);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp1_U8;
      /*$64AE*/ branchTarget = true; block_id = tmp1_U8 ? 165 : 166;
      break;
    case 165:  // $64AE
      /*$64AE*/ CYCLES_EDGE(0x64ae, 1);
                branchTarget = true; block_id = 164;
      break;
    case 166:  // $64B0
      /*$64B0*/ CYCLES(0x64b0, 12);
      /*$64B3*/ tmp1_U8 = peek((0xc000 + ram_peek(0x6c49)));
      /*$64B6*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp1_U8;
      /*$64B7*/ branchTarget = true; block_id = tmp1_U8 ? 167 : 168;
      break;
    case 167:  // $64B7
      /*$64B7*/ CYCLES_EDGE(0x64b7, 1);
                branchTarget = true; block_id = 163;
      break;
    case 168:  // $64B9
      /*$64B9*/ CYCLES(0x64b9, 4);
      /*$64BA*/ s_y = s_x;
                block_id = 169;
      break;
    case 169:  // $64BB
      /*$64BB*/ CYCLES(0x64bb, 4);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp1_U8;
      /*$64BC*/ branchTarget = true; block_id = tmp1_U8 ? 170 : 171;
      break;
    case 170:  // $64BC
      /*$64BC*/ CYCLES_EDGE(0x64bc, 1);
                branchTarget = true; block_id = 169;
      break;
    case 171:  // $64BE
      /*$64BE*/ CYCLES(0x64be, 12);
                tmp1_U8 = ram_peek(0x6c49);
                s_y = tmp1_U8;
      /*$64C1*/ tmp1_U8 = peek((0xc000 + tmp1_U8));
                s_a = tmp1_U8;
      /*$64C4*/ tmp1_U8 = (uint8_t)(s_x + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$64C5*/ branchTarget = true; block_id = tmp1_U8 ? 172 : 173;
      break;
    case 172:  // $64C5
      /*$64C5*/ CYCLES_EDGE(0x64c5, 1);
                branchTarget = true; block_id = 168;
      break;
    case 173:  // $64C7
      /*$64C7*/ CYCLES(0x64c7, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x64c7, pop16() + 1);;
      break;
    case 174:  // $64C8
      /*$64C8*/ CYCLES(0x64c8, 12);
      /*$64CA*/ ram_poke(0x6c4a, 0x00);
      /*$64CD*/ tmp1_U8 = ram_peek(0x6634);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$64D0*/ branchTarget = true; block_id = tmp1_U8 ? 175 : 176;
      break;
    case 175:  // $64D0
      /*$64D0*/ CYCLES_EDGE(0x64d0, 1);
      /*$64D3*/ CYCLES(0x64d3, 36);
                s_status_c = 0x00;
      /*$64D4*/ block_id = s_status_d ? 178 : 177;
      break;
    case 176:  // $64D2
      /*$64D2*/ CYCLES(0x64d2, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x64d2, pop16() + 1);;
      break;
    case 177:  // $64D4
      /*$64D4*/ s_a = (uint8_t)((s_a + ram_peek(0x6636)) + s_status_c);
                block_id = 179;
      break;
    case 178:  // $64D4
      /*$64D4*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x6636), s_status_c));
                block_id = 179;
      break;
    case 179:  // $64D7
      /*$64D7*/ ram_poke(0x6638, s_a);
      /*$64DA*/ s_a = ram_peek(0x6633);
      /*$64DD*/ s_status_c = 0x00;
      /*$64DE*/ block_id = s_status_d ? 181 : 180;
      break;
    case 180:  // $64DE
      /*$64DE*/ tmp3_U16 = s_a;
                tmp4_U16 = ram_peek(0x6635);
                tmp6_U16 = (tmp3_U16 + tmp4_U16) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 182;
      break;
    case 181:  // $64DE
      /*$64DE*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x6635), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 182;
      break;
    case 182:  // $64E1
      /*$64E1*/ tmp1_U8 = s_a;
                ram_poke(0x6637, tmp1_U8);
      /*$64E4*/ s_y = tmp1_U8;
      /*$64E5*/ s_a = ram_peek(0x6638);
      /*$64E8*/ FUNC_SCRN(0x64ea);
                branchTarget = true; block_id = 183;
      break;
    case 183:  // $64EB
      /*$64EB*/ CYCLES(0x64eb, 4);
      /*$64ED*/ branchTarget = true; block_id = !(s_a != 0x00) ? 184 : 185;
      break;
    case 184:  // $64ED
      /*$64ED*/ CYCLES_EDGE(0x64ed, 1);
                branchTarget = true; block_id = 196;
      break;
    case 185:  // $64EF
      /*$64EF*/ CYCLES(0x64ef, 14);
                s_y = ram_peek(0x6637);
      /*$64F2*/ s_a = ram_peek(0x6634);
      /*$64F5*/ FUNC_SCRN(0x64f7);
                branchTarget = true; block_id = 186;
      break;
    case 186:  // $64F8
      /*$64F8*/ CYCLES(0x64f8, 4);
      /*$64FA*/ branchTarget = true; block_id = !(s_a != 0x00) ? 187 : 188;
      break;
    case 187:  // $64FA
      /*$64FA*/ CYCLES_EDGE(0x64fa, 1);
                branchTarget = true; block_id = 189;
      break;
    case 188:  // $64FC
      /*$64FC*/ CYCLES(0x64fc, 24);
      /*$64FF*/ ram_poke(0x6637, ram_peek(0x6633));
      /*$6507*/ ram_poke(0x6635, (ram_peek(0x6635) ^ 0xfe));
      /*$650A*/ ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
                block_id = 189;
      break;
    case 189:  // $650D
      /*$650D*/ CYCLES(0x650d, 14);
                s_a = ram_peek(0x6638);
      /*$6510*/ s_y = ram_peek(0x6633);
      /*$6513*/ FUNC_SCRN(0x6515);
                branchTarget = true; block_id = 190;
      break;
    case 190:  // $6516
      /*$6516*/ CYCLES(0x6516, 4);
      /*$6518*/ branchTarget = true; block_id = !(s_a != 0x00) ? 191 : 192;
      break;
    case 191:  // $6518
      /*$6518*/ CYCLES_EDGE(0x6518, 1);
                branchTarget = true; block_id = 193;
      break;
    case 192:  // $651A
      /*$651A*/ CYCLES(0x651a, 24);
      /*$651D*/ ram_poke(0x6638, ram_peek(0x6634));
      /*$6525*/ ram_poke(0x6636, (ram_peek(0x6636) ^ 0xfe));
      /*$6528*/ ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
                block_id = 193;
      break;
    case 193:  // $652B
      /*$652B*/ CYCLES(0x652b, 6);
      /*$652E*/ branchTarget = true; block_id = ram_peek(0x6c4a) ? 194 : 195;
      break;
    case 194:  // $652E
      /*$652E*/ CYCLES_EDGE(0x652e, 1);
                branchTarget = true; block_id = 196;
      break;
    case 195:  // $6530
      /*$6530*/ CYCLES(0x6530, 36);
      /*$6533*/ ram_poke(0x6637, ram_peek(0x6633));
      /*$6539*/ ram_poke(0x6638, ram_peek(0x6634));
      /*$6541*/ ram_poke(0x6635, (ram_peek(0x6635) ^ 0xfe));
      /*$6549*/ ram_poke(0x6636, (ram_peek(0x6636) ^ 0xfe));
                block_id = 196;
      break;
    case 196:  // $654C
      /*$654C*/ CYCLES(0x654c, 11);
                s_a = 0x00;
      /*$654E*/ ram_poke(0x0001, 0x00);
      /*$6550*/ FUNC_SETCOL(0x6552);
                branchTarget = true; block_id = 197;
      break;
    case 197:  // $6553
      /*$6553*/ CYCLES(0x6553, 20);
      /*$6556*/ ram_poke(0x0002, ram_peek(0x6633));
      /*$655B*/ ram_poke(0x0003, ram_peek(0x6634));
      /*$655D*/ func_60e4(0x655f);
                branchTarget = true; block_id = 198;
      break;
    case 198:  // $6560
      /*$6560*/ CYCLES(0x6560, 14);
                s_a = ram_peek(0x6634);
      /*$6563*/ s_y = ram_peek(0x6633);
      /*$6566*/ FUNC_MON_PLOT(0x6568);
                branchTarget = true; block_id = 199;
      break;
    case 199:  // $6569
      /*$6569*/ CYCLES(0x6569, 11);
      /*$656B*/ ram_poke(0x0000, 0x1a);
      /*$656D*/ tmp1_U8 = ram_peek(0x6638);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$6570*/ branchTarget = true; block_id = tmp1_U8 ? 200 : 201;
      break;
    case 200:  // $6570
      /*$6570*/ CYCLES_EDGE(0x6570, 1);
      /*$6573*/ CYCLES(0x6573, 29);
                tmp1_U8 = s_a;
                ram_poke(0x0003, tmp1_U8);
      /*$6575*/ ram_poke(0x6634, tmp1_U8);
      /*$6578*/ tmp1_U8 = ram_peek(0x6637);
      /*$657B*/ ram_poke(0x0002, tmp1_U8);
      /*$657D*/ ram_poke(0x6633, tmp1_U8);
      /*$6580*/ s_a = 0x03;
      /*$6582*/ ram_poke(0x0001, 0x03);
      /*$6584*/ FUNC_SETCOL(0x6586);
                branchTarget = true; block_id = 202;
      break;
    case 201:  // $6572
      /*$6572*/ CYCLES(0x6572, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x6572, pop16() + 1);;
      break;
    case 202:  // $6587
      /*$6587*/ CYCLES(0x6587, 6);
                func_60e4(0x6589);
                branchTarget = true; block_id = 203;
      break;
    case 203:  // $658A
      /*$658A*/ CYCLES(0x658a, 14);
                s_a = ram_peek(0x6634);
      /*$658D*/ s_y = ram_peek(0x6633);
      /*$6590*/ FUNC_MON_PLOT(0x6592);
                branchTarget = true; block_id = 204;
      break;
    case 204:  // $6593
      /*$6593*/ CYCLES(0x6593, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x6593, pop16() + 1);;
      break;
    case 205:  // $6594
      /*$6594*/ CYCLES(0x6594, 6);
      /*$6597*/ branchTarget = true; block_id = ram_peek(0x0301) ? 206 : 207;
      break;
    case 206:  // $6597
      /*$6597*/ CYCLES_EDGE(0x6597, 1);
      /*$659C*/ CYCLES(0x659c, 38);
      /*$659F*/ ram_poke(0x6633, ram_peek(0x6639));
      /*$65A5*/ ram_poke(0x6634, ram_peek(0x663a));
      /*$65AB*/ ram_poke(0x6635, ram_peek(0x663d));
      /*$65B1*/ ram_poke(0x6636, ram_peek(0x663e));
      /*$65B4*/ func_64c8(0x65b6);
                branchTarget = true; block_id = 208;
      break;
    case 207:  // $6599
      /*$6599*/ CYCLES(0x6599, 3);
                branchTarget = true; block_id = 46;
      break;
    case 208:  // $65B7
      /*$65B7*/ CYCLES(0x65b7, 40);
      /*$65BA*/ ram_poke(0x6639, ram_peek(0x6633));
      /*$65C0*/ ram_poke(0x663a, ram_peek(0x6634));
      /*$65C6*/ ram_poke(0x663d, ram_peek(0x6635));
      /*$65CC*/ ram_poke(0x663e, ram_peek(0x6636));
      /*$65CF*/ tmp1_U8 = ram_peek(0x0301);
      /*$65D2*/ s_status_c = (tmp1_U8 >= 0x01);
      /*$65D4*/ branchTarget = true; block_id = (tmp1_U8 != 0x01) ? 209 : 210;
      break;
    case 209:  // $65D4
      /*$65D4*/ CYCLES_EDGE(0x65d4, 1);
      /*$65D9*/ CYCLES(0x65d9, 38);
      /*$65DC*/ ram_poke(0x6633, ram_peek(0x663b));
      /*$65E2*/ ram_poke(0x6634, ram_peek(0x663c));
      /*$65E8*/ ram_poke(0x6635, ram_peek(0x663f));
      /*$65EE*/ ram_poke(0x6636, ram_peek(0x6640));
      /*$65F1*/ func_64c8(0x65f3);
      /*$65F4*/ CYCLES(0x65f4, 35);
      /*$65F7*/ ram_poke(0x663b, ram_peek(0x6633));
      /*$65FD*/ ram_poke(0x663c, ram_peek(0x6634));
      /*$6603*/ ram_poke(0x663f, ram_peek(0x6635));
      /*$6609*/ ram_poke(0x6640, ram_peek(0x6636));
      /*$660C*/ branchTarget = true; block_id = 46;
      break;
    case 210:  // $65D6
      /*$65D6*/ CYCLES(0x65d6, 3);
                branchTarget = true; block_id = 46;
      break;
    case 211:  // $660F
      /*$660F*/ CYCLES(0x660f, 50);
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
    case 212:  // $6641
      /*$6641*/ CYCLES(0x6641, 16);
      /*$6643*/ ram_poke(0x0036, 0x4a);
      /*$6645*/ s_status_not_z = 0x66;
                s_status_n = 0x00;
                s_a = 0x66;
      /*$6647*/ ram_poke(0x0037, 0x66);
      /*$6649*/ branchTarget = true; block_id = find_block_id_func_t001(0x6649, pop16() + 1);;
      break;
    case 213:  // $664A
      /*$664A*/ CYCLES(0x664a, 9);
                tmp1_U8 = s_a;
                push8(tmp1_U8);
      /*$664B*/ tmp1_U8 = tmp1_U8 & 0x7f;
                s_a = tmp1_U8;
      /*$664F*/ branchTarget = true; block_id = (tmp1_U8 >= 0x20) ? 214 : 215;
      break;
    case 214:  // $664F
      /*$664F*/ CYCLES_EDGE(0x664f, 1);
      /*$6655*/ CYCLES(0x6655, 82);
                ram_poke(0x0008, s_a);
      /*$6657*/ ram_poke(0x0002, s_x);
      /*$6659*/ ram_poke(0x0003, s_y);
      /*$665D*/ ram_poke(0x0000, 0x00);
      /*$665F*/ ram_poke(0x0001, 0x00);
      /*$6661*/ s_a = ram_peek(0x0029);
      /*$6663*/ s_status_c = 0x01;
      /*$6664*/ block_id = s_status_d ? 217 : 216;
      break;
    case 215:  // $6651
      /*$6651*/ CYCLES(0x6651, 7);
                tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$6652*/ branchTarget = true; block_id = 2625;
      break;
    case 216:  // $6664
      /*$6664*/ s_a = (uint8_t)((s_a - 0x0004) - (uint8_t)(0x01 - s_status_c));
                block_id = 218;
      break;
    case 217:  // $6664
      /*$6664*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x04, s_status_c));
                block_id = 218;
      break;
    case 218:  // $6666
      /*$6666*/ s_status_c = 0x00;
      /*$6667*/ block_id = s_status_d ? 220 : 219;
      break;
    case 219:  // $6667
      /*$6667*/ s_a = (uint8_t)((s_a + 0x0020) + s_status_c);
                block_id = 221;
      break;
    case 220:  // $6667
      /*$6667*/ s_a = ((uint8_t)adc_dec16(s_a, 0x20, s_status_c));
                block_id = 221;
      break;
    case 221:  // $6669
      /*$6669*/ ram_poke(0x0005, s_a);
      /*$666B*/ s_a = ram_peek(0x0028);
      /*$666D*/ s_status_c = 0x00;
      /*$666E*/ block_id = s_status_d ? 223 : 222;
      break;
    case 222:  // $666E
      /*$666E*/ s_a = (uint8_t)((s_a + ram_peek(0x0024)) + s_status_c);
                block_id = 224;
      break;
    case 223:  // $666E
      /*$666E*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x0024), s_status_c));
                block_id = 224;
      break;
    case 224:  // $6670
      /*$6670*/ ram_poke(0x0004, s_a);
      /*$6672*/ s_a = ram_peek(0x0008);
      /*$6674*/ s_status_c = 0x01;
      /*$6675*/ block_id = s_status_d ? 226 : 225;
      break;
    case 225:  // $6675
      /*$6675*/ s_a = (uint8_t)((s_a - 0x0020) - (uint8_t)(0x01 - s_status_c));
                block_id = 227;
      break;
    case 226:  // $6675
      /*$6675*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x20, s_status_c));
                block_id = 227;
      break;
    case 227:  // $6677
      /*$6677*/ tmp6_U16 = s_a;
      /*$6679*/ ram_poke(0x0001, (uint8_t)((ram_peek(0x0001) << 0x01) | (uint8_t)(((tmp6_U16 << 0x02) & 0x01ff) >> 8)));
      /*$667B*/ tmp6_U16 = tmp6_U16 << 0x03;
                s_a = ((uint8_t)tmp6_U16);
      /*$667C*/ ram_poke(0x0001, (uint8_t)((ram_peek(0x0001) << 0x01) | (uint8_t)((tmp6_U16 & 0x01ff) >> 8)));
      /*$667E*/ s_status_c = 0x00;
      /*$667F*/ block_id = s_status_d ? 229 : 228;
      break;
    case 228:  // $667F
      /*$667F*/ tmp6_U16 = (s_a + 0x00a9) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 230;
      break;
    case 229:  // $667F
      /*$667F*/ tmp6_U16 = adc_dec16(s_a, 0xa9, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 230;
      break;
    case 230:  // $6681
      /*$6681*/ ram_poke(0x0000, s_a);
      /*$6683*/ s_a = ram_peek(0x0001);
      /*$6685*/ block_id = s_status_d ? 232 : 231;
      break;
    case 231:  // $6685
      /*$6685*/ s_a = (uint8_t)((s_a + 0x0066) + s_status_c);
                block_id = 233;
      break;
    case 232:  // $6685
      /*$6685*/ s_a = ((uint8_t)adc_dec16(s_a, 0x66, s_status_c));
                block_id = 233;
      break;
    case 233:  // $6687
      /*$6687*/ ram_poke(0x0001, s_a);
      /*$6689*/ s_x = 0x00;
                block_id = 234;
      break;
    case 234:  // $668B
      /*$668B*/ CYCLES(0x668b, 33);
      /*$668D*/ tmp1_U8 = peek((ram_peek16al(0x0000) + s_x));
      /*$6691*/ poke(ram_peek16al(0x0004), tmp1_U8);
      /*$6693*/ s_a = ram_peek(0x0005);
      /*$6695*/ s_status_c = 0x00;
      /*$6696*/ block_id = s_status_d ? 236 : 235;
      break;
    case 235:  // $6696
      /*$6696*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 + 0x0004) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x0004);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 237;
      break;
    case 236:  // $6696
      /*$6696*/ tmp4_U16 = adc_dec16(s_a, 0x04, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 237;
      break;
    case 237:  // $6698
      /*$6698*/ ram_poke(0x0005, s_a);
      /*$669A*/ tmp1_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp1_U8;
      /*$669D*/ branchTarget = true; block_id = (tmp1_U8 != 0x08) ? 238 : 239;
      break;
    case 238:  // $669D
      /*$669D*/ CYCLES_EDGE(0x669d, 1);
                branchTarget = true; block_id = 234;
      break;
    case 239:  // $669F
      /*$669F*/ CYCLES(0x669f, 9);
                s_x = ram_peek(0x0002);
      /*$66A1*/ s_y = ram_peek(0x0003);
      /*$66A3*/ branchTarget = true; block_id = 215;
      break;
    case 240:  // $69A9
      /*$69A9*/ CYCLES(0x69a9, 4);
      /*$69AB*/ branchTarget = true; block_id = (s_a != 0x9b) ? 241 : 242;
      break;
    case 241:  // $69AB
      /*$69AB*/ CYCLES_EDGE(0x69ab, 1);
                branchTarget = true; block_id = 245;
      break;
    case 242:  // $69AD
      /*$69AD*/ CYCLES(0x69ad, 6);
                tmp5_U8 = io_peek(0xc000);
                s_a = tmp5_U8;
      /*$69B0*/ branchTarget = true; block_id = !(tmp5_U8 & 0x80) ? 243 : 244;
      break;
    case 243:  // $69B0
      /*$69B0*/ CYCLES_EDGE(0x69b0, 1);
                branchTarget = true; block_id = 242;
      break;
    case 244:  // $69B2
      /*$69B2*/ CYCLES(0x69b2, 4);
                io_poke(0xc010, s_a);
                block_id = 245;
      break;
    case 245:  // $69B5
      /*$69B5*/ CYCLES(0x69b5, 4);
                tmp1_U8 = s_a;
                tmp5_U8 = tmp1_U8 != 0x93;
                s_status_not_z = tmp5_U8;
                s_status_c = (tmp1_U8 >= 0x93);
                s_status_n = ((uint8_t)(tmp1_U8 - 0x93) & 0x80);
      /*$69B7*/ branchTarget = true; block_id = tmp5_U8 ? 246 : 247;
      break;
    case 246:  // $69B7
      /*$69B7*/ CYCLES_EDGE(0x69b7, 1);
                branchTarget = true; block_id = 248;
      break;
    case 247:  // $69B9
      /*$69B9*/ CYCLES(0x69b9, 10);
      /*$69BC*/ tmp5_U8 = ram_peek(0x69c2) ^ 0x01;
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$69BE*/ ram_poke(0x69c2, tmp5_U8);
                block_id = 248;
      break;
    case 248:  // $69C1
      /*$69C1*/ CYCLES(0x69c1, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x69c1, pop16() + 1);;
      break;
    case 249:  // $69C3
      /*$69C3*/ CYCLES(0x69c3, 14);
      /*$69C6*/ ram_poke(0x6b39, ram_peek(0x624f));
      /*$69CB*/ ram_poke(0x6b3a, 0x01);
                block_id = 250;
      break;
    case 250:  // $69CE
      /*$69CE*/ CYCLES(0x69ce, 14);
                s_a = ram_peek(0x6b3a);
      /*$69D1*/ s_y = ram_peek(0x6b39);
      /*$69D4*/ FUNC_SCRN(0x69d6);
                branchTarget = true; block_id = 251;
      break;
    case 251:  // $69D7
      /*$69D7*/ CYCLES(0x69d7, 4);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0x0f);
      /*$69D9*/ branchTarget = true; block_id = !(tmp5_U8 != 0x0f) ? 252 : 253;
      break;
    case 252:  // $69D9
      /*$69D9*/ CYCLES_EDGE(0x69d9, 1);
                branchTarget = true; block_id = 266;
      break;
    case 253:  // $69DB
      /*$69DB*/ CYCLES(0x69db, 14);
                ram_poke(0x6b3a, (uint8_t)(ram_peek(0x6b3a) + 0x01));
      /*$69E3*/ branchTarget = true; block_id = (ram_peek(0x6b3a) != 0x27) ? 254 : 255;
      break;
    case 254:  // $69E3
      /*$69E3*/ CYCLES_EDGE(0x69e3, 1);
                branchTarget = true; block_id = 250;
      break;
    case 255:  // $69E5
      /*$69E5*/ CYCLES(0x69e5, 14);
      /*$69E7*/ ram_poke(0x6b3a, 0x01);
      /*$69EA*/ tmp5_U8 = (uint8_t)(ram_peek(0x6b39) - 0x01);
                ram_poke(0x6b39, tmp5_U8);
      /*$69ED*/ branchTarget = true; block_id = tmp5_U8 ? 256 : 257;
      break;
    case 256:  // $69ED
      /*$69ED*/ CYCLES_EDGE(0x69ed, 1);
                branchTarget = true; block_id = 250;
      break;
    case 257:  // $69EF
      /*$69EF*/ CYCLES(0x69ef, 8);
      /*$69F2*/ ram_poke(0x6b39, ram_peek(0x624f));
                block_id = 258;
      break;
    case 258:  // $69F5
      /*$69F5*/ CYCLES(0x69f5, 14);
                s_a = ram_peek(0x6b3a);
      /*$69F8*/ s_y = ram_peek(0x6b39);
      /*$69FB*/ FUNC_SCRN(0x69fd);
                branchTarget = true; block_id = 259;
      break;
    case 259:  // $69FE
      /*$69FE*/ CYCLES(0x69fe, 2);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0x0f);
      /*$6A00*/ CYCLES(0x6a00, 2);
                branchTarget = true; block_id = !(tmp5_U8 != 0x0f) ? 260 : 261;
      break;
    case 260:  // $6A00
      /*$6A00*/ CYCLES_EDGE(0x6a00, 1);
                branchTarget = true; block_id = 266;
      break;
    case 261:  // $6A02
      /*$6A02*/ CYCLES(0x6a02, 14);
                ram_poke(0x6b3a, (uint8_t)(ram_peek(0x6b3a) + 0x01));
      /*$6A0A*/ branchTarget = true; block_id = (ram_peek(0x6b3a) != 0x27) ? 262 : 263;
      break;
    case 262:  // $6A0A
      /*$6A0A*/ CYCLES_EDGE(0x6a0a, 1);
                branchTarget = true; block_id = 258;
      break;
    case 263:  // $6A0C
      /*$6A0C*/ CYCLES(0x6a0c, 20);
      /*$6A0E*/ ram_poke(0x6b3a, 0x01);
      /*$6A11*/ ram_poke(0x6b39, (uint8_t)(ram_peek(0x6b39) + 0x01));
      /*$6A14*/ tmp5_U8 = ram_peek(0x6b39);
      /*$6A17*/ s_status_c = (tmp5_U8 >= 0x27);
      /*$6A19*/ branchTarget = true; block_id = (tmp5_U8 != 0x27) ? 264 : 265;
      break;
    case 264:  // $6A19
      /*$6A19*/ CYCLES_EDGE(0x6a19, 1);
                branchTarget = true; block_id = 258;
      break;
    case 265:  // $6A1B
      /*$6A1B*/ CYCLES(0x6a1b, 12);
      /*$6A1D*/ ram_poke(0x6b3a, 0x00);
      /*$6A22*/ ram_poke(0x6b39, 0x14);
                block_id = 266;
      break;
    case 266:  // $6A25
      /*$6A25*/ CYCLES(0x6a25, 22);
      /*$6A28*/ ram_poke(0x6b3b, ram_peek(0x6b39));
      /*$6A2B*/ tmp5_U8 = ram_peek(0x6b3a);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$6A2E*/ ram_poke(0x6b3c, tmp5_U8);
      /*$6A31*/ branchTarget = true; block_id = find_block_id_func_t001(0x6a31, pop16() + 1);;
      break;
    case 267:  // $6A32
      /*$6A32*/ CYCLES(0x6a32, 10);
                tmp5_U8 = ram_peek(0x6b3c);
      /*$6A35*/ tmp1_U8 = ram_peek(0x6250);
                s_status_c = (tmp5_U8 >= tmp1_U8);
      /*$6A38*/ branchTarget = true; block_id = !(tmp5_U8 != tmp1_U8) ? 268 : 269;
      break;
    case 268:  // $6A38
      /*$6A38*/ CYCLES_EDGE(0x6a38, 1);
                branchTarget = true; block_id = 278;
      break;
    case 269:  // $6A3A
      /*$6A3A*/ CYCLES(0x6a3a, 4);
                s_a = 0x04;
      /*$6A3C*/ branchTarget = true; block_id = s_status_c ? 270 : 271;
      break;
    case 270:  // $6A3C
      /*$6A3C*/ CYCLES_EDGE(0x6a3c, 1);
                branchTarget = true; block_id = 272;
      break;
    case 271:  // $6A3E
      /*$6A3E*/ CYCLES(0x6a3e, 2);
                s_a = 0x02;
                block_id = 272;
      break;
    case 272:  // $6A40
      /*$6A40*/ CYCLES(0x6a40, 10);
                ram_poke(0x6b38, s_a);
      /*$6A43*/ func_6ab8(0x6a45);
      /*$6A46*/ CYCLES(0x6a46, 2);
                branchTarget = true; block_id = s_status_not_z ? 273 : 274;
      break;
    case 273:  // $6A46
      /*$6A46*/ CYCLES_EDGE(0x6a46, 1);
                branchTarget = true; block_id = 278;
      break;
    case 274:  // $6A48
      /*$6A48*/ CYCLES(0x6a48, 10);
                tmp5_U8 = ram_peek(0x6b38);
                s_a = tmp5_U8;
      /*$6A4B*/ tmp1_U8 = ram_peek(0x624e);
                tmp2_U8 = tmp5_U8 != tmp1_U8;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp5_U8 >= tmp1_U8);
                s_status_n = ((uint8_t)(tmp5_U8 - tmp1_U8) & 0x80);
      /*$6A4E*/ branchTarget = true; block_id = !tmp2_U8 ? 275 : 276;
      break;
    case 275:  // $6A4E
      /*$6A4E*/ CYCLES_EDGE(0x6a4e, 1);
                branchTarget = true; block_id = 277;
      break;
    case 276:  // $6A50
      /*$6A50*/ CYCLES(0x6a50, 6);
                tmp2_U8 = s_a;
                s_x = tmp2_U8;
      /*$6A51*/ tmp2_U8 = ram_peek((0x6a55 + tmp2_U8));
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 277;
      break;
    case 277:  // $6A54
      /*$6A54*/ CYCLES(0x6a54, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x6a54, pop16() + 1);;
      break;
    case 278:  // $6A5A
      /*$6A5A*/ CYCLES(0x6a5a, 10);
      /*$6A60*/ branchTarget = true; block_id = !(ram_peek(0x6b3b) >= ram_peek(0x624f)) ? 279 : 280;
      break;
    case 279:  // $6A60
      /*$6A60*/ CYCLES_EDGE(0x6a60, 1);
      /*$6A79*/ CYCLES(0x6a79, 12);
      /*$6A7B*/ ram_poke(0x6b38, 0x03);
      /*$6A7E*/ func_6ab8(0x6a80);
      /*$6A81*/ CYCLES(0x6a81, 2);
                branchTarget = true; block_id = !s_status_not_z ? 285 : 286;
      break;
    case 280:  // $6A62
      /*$6A62*/ CYCLES(0x6a62, 12);
      /*$6A64*/ ram_poke(0x6b38, 0x01);
      /*$6A67*/ func_6ab8(0x6a69);
      /*$6A6A*/ CYCLES(0x6a6a, 2);
                branchTarget = true; block_id = !s_status_not_z ? 281 : 282;
      break;
    case 281:  // $6A6A
      /*$6A6A*/ CYCLES_EDGE(0x6a6a, 1);
                branchTarget = true; block_id = 274;
      break;
    case 282:  // $6A6C
      /*$6A6C*/ CYCLES(0x6a6c, 12);
      /*$6A6E*/ ram_poke(0x6b38, 0x03);
      /*$6A71*/ func_6ab8(0x6a73);
      /*$6A74*/ CYCLES(0x6a74, 2);
                branchTarget = true; block_id = !s_status_not_z ? 283 : 284;
      break;
    case 283:  // $6A74
      /*$6A74*/ CYCLES_EDGE(0x6a74, 1);
                branchTarget = true; block_id = 274;
      break;
    case 284:  // $6A76
      /*$6A76*/ CYCLES(0x6a76, 3);
                branchTarget = true; block_id = 288;
      break;
    case 285:  // $6A81
      /*$6A81*/ CYCLES_EDGE(0x6a81, 1);
                branchTarget = true; block_id = 274;
      break;
    case 286:  // $6A83
      /*$6A83*/ CYCLES(0x6a83, 12);
      /*$6A85*/ ram_poke(0x6b38, 0x01);
      /*$6A88*/ func_6ab8(0x6a8a);
      /*$6A8B*/ CYCLES(0x6a8b, 2);
                branchTarget = true; block_id = !s_status_not_z ? 287 : 288;
      break;
    case 287:  // $6A8B
      /*$6A8B*/ CYCLES_EDGE(0x6a8b, 1);
                branchTarget = true; block_id = 274;
      break;
    case 288:  // $6A8D
      /*$6A8D*/ CYCLES(0x6a8d, 10);
      /*$6A93*/ branchTarget = true; block_id = !(ram_peek(0x6b3c) >= ram_peek(0x6250)) ? 289 : 290;
      break;
    case 289:  // $6A93
      /*$6A93*/ CYCLES_EDGE(0x6a93, 1);
                branchTarget = true; block_id = 292;
      break;
    case 290:  // $6A95
      /*$6A95*/ CYCLES(0x6a95, 12);
      /*$6A97*/ ram_poke(0x6b38, 0x04);
      /*$6A9A*/ func_6ab8(0x6a9c);
      /*$6A9D*/ CYCLES(0x6a9d, 2);
                branchTarget = true; block_id = !s_status_not_z ? 291 : 292;
      break;
    case 291:  // $6A9D
      /*$6A9D*/ CYCLES_EDGE(0x6a9d, 1);
                branchTarget = true; block_id = 274;
      break;
    case 292:  // $6A9F
      /*$6A9F*/ CYCLES(0x6a9f, 12);
      /*$6AA1*/ ram_poke(0x6b38, 0x02);
      /*$6AA4*/ func_6ab8(0x6aa6);
      /*$6AA7*/ CYCLES(0x6aa7, 2);
                branchTarget = true; block_id = !s_status_not_z ? 293 : 294;
      break;
    case 293:  // $6AA7
      /*$6AA7*/ CYCLES_EDGE(0x6aa7, 1);
                branchTarget = true; block_id = 274;
      break;
    case 294:  // $6AA9
      /*$6AA9*/ CYCLES(0x6aa9, 12);
      /*$6AAB*/ ram_poke(0x6b38, 0x04);
      /*$6AAE*/ func_6ab8(0x6ab0);
      /*$6AB1*/ CYCLES(0x6ab1, 2);
                branchTarget = true; block_id = !s_status_not_z ? 295 : 296;
      break;
    case 295:  // $6AB1
      /*$6AB1*/ CYCLES_EDGE(0x6ab1, 1);
                branchTarget = true; block_id = 274;
      break;
    case 296:  // $6AB3
      /*$6AB3*/ CYCLES(0x6ab3, 11);
                tmp2_U8 = pop8();
      /*$6AB4*/ tmp2_U8 = pop8();
      /*$6AB5*/ branchTarget = true; block_id = 90;
      break;
    case 297:  // $6AB8
      /*$6AB8*/ CYCLES(0x6ab8, 42);
                tmp5_U8 = ram_peek(0x6b38);
                s_x = tmp5_U8;
      /*$6ABB*/ s_a = ram_peek((0x6232 + tmp5_U8));
      /*$6ABE*/ s_status_c = 0x00;
      /*$6ABF*/ block_id = s_status_d ? 299 : 298;
      break;
    case 298:  // $6ABF
      /*$6ABF*/ s_a = (uint8_t)((s_a + ram_peek(0x624f)) + s_status_c);
                block_id = 300;
      break;
    case 299:  // $6ABF
      /*$6ABF*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x624f), s_status_c));
                block_id = 300;
      break;
    case 300:  // $6AC2
      /*$6AC2*/ ram_poke(0x6637, s_a);
      /*$6AC5*/ s_a = ram_peek((0x6237 + s_x));
      /*$6AC8*/ s_status_c = 0x00;
      /*$6AC9*/ block_id = s_status_d ? 302 : 301;
      break;
    case 301:  // $6AC9
      /*$6AC9*/ tmp4_U16 = s_a;
                tmp6_U16 = ram_peek(0x6250);
                tmp3_U16 = (tmp4_U16 + tmp6_U16) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp6_U16);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 303;
      break;
    case 302:  // $6AC9
      /*$6AC9*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x6250), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
                block_id = 303;
      break;
    case 303:  // $6ACC
      /*$6ACC*/ ram_poke(0x6638, s_a);
      /*$6ACF*/ s_y = ram_peek(0x6637);
      /*$6AD2*/ FUNC_SCRN(0x6ad4);
      /*$6AD5*/ CYCLES(0x6ad5, 4);
                tmp2_U8 = s_a;
                tmp1_U8 = tmp2_U8 != 0x00;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp2_U8 >= 0x00);
                s_status_n = (tmp2_U8 & 0x80);
      /*$6AD7*/ branchTarget = true; block_id = !tmp1_U8 ? 304 : 305;
      break;
    case 304:  // $6AD7
      /*$6AD7*/ CYCLES_EDGE(0x6ad7, 1);
                branchTarget = true; block_id = 306;
      break;
    case 305:  // $6AD9
      /*$6AD9*/ CYCLES(0x6ad9, 2);
                tmp1_U8 = s_a;
                s_status_not_z = (tmp1_U8 != 0x0f);
                s_status_c = (tmp1_U8 >= 0x0f);
                s_status_n = ((uint8_t)(tmp1_U8 - 0x0f) & 0x80);
                block_id = 306;
      break;
    case 306:  // $6ADB
      /*$6ADB*/ CYCLES(0x6adb, 2);
                branchTarget = true; block_id = !s_status_not_z ? 307 : 308;
      break;
    case 307:  // $6ADB
      /*$6ADB*/ CYCLES_EDGE(0x6adb, 1);
      /*$6ADE*/ CYCLES(0x6ade, 12);
      /*$6AE0*/ ram_poke(0x6c4a, 0x00);
      /*$6AE3*/ tmp1_U8 = ram_peek(0x6638);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$6AE6*/ branchTarget = true; block_id = !tmp1_U8 ? 309 : 310;
      break;
    case 308:  // $6ADD
      /*$6ADD*/ CYCLES(0x6add, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x6add, pop16() + 1);;
      break;
    case 309:  // $6AE6
      /*$6AE6*/ CYCLES_EDGE(0x6ae6, 1);
                branchTarget = true; block_id = 308;
      break;
    case 310:  // $6AE8
      /*$6AE8*/ CYCLES(0x6ae8, 12);
      /*$6AEB*/ s_y = (uint8_t)(ram_peek(0x6637) + 0x01);
      /*$6AEC*/ FUNC_SCRN(0x6aee);
      /*$6AEF*/ CYCLES(0x6aef, 4);
      /*$6AF1*/ branchTarget = true; block_id = (s_a != 0x00) ? 311 : 312;
      break;
    case 311:  // $6AF1
      /*$6AF1*/ CYCLES_EDGE(0x6af1, 1);
                branchTarget = true; block_id = 313;
      break;
    case 312:  // $6AF3
      /*$6AF3*/ CYCLES(0x6af3, 6);
                ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
                block_id = 313;
      break;
    case 313:  // $6AF6
      /*$6AF6*/ CYCLES(0x6af6, 16);
                s_a = ram_peek(0x6638);
      /*$6AFC*/ s_y = (uint8_t)(ram_peek(0x6637) - 0x01);
      /*$6AFD*/ FUNC_SCRN(0x6aff);
      /*$6B00*/ CYCLES(0x6b00, 4);
      /*$6B02*/ branchTarget = true; block_id = (s_a != 0x00) ? 314 : 315;
      break;
    case 314:  // $6B02
      /*$6B02*/ CYCLES_EDGE(0x6b02, 1);
                branchTarget = true; block_id = 316;
      break;
    case 315:  // $6B04
      /*$6B04*/ CYCLES(0x6b04, 6);
                ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
                block_id = 316;
      break;
    case 316:  // $6B07
      /*$6B07*/ CYCLES(0x6b07, 18);
                s_a = ram_peek(0x6638);
      /*$6B0A*/ s_status_c = 0x00;
      /*$6B0B*/ block_id = s_status_d ? 318 : 317;
      break;
    case 317:  // $6B0B
      /*$6B0B*/ tmp3_U16 = s_a;
                tmp6_U16 = (tmp3_U16 + 0x0001) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)0x0001);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 319;
      break;
    case 318:  // $6B0B
      /*$6B0B*/ tmp6_U16 = adc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 319;
      break;
    case 319:  // $6B0D
      /*$6B0D*/ s_y = ram_peek(0x6637);
      /*$6B10*/ FUNC_SCRN(0x6b12);
      /*$6B13*/ CYCLES(0x6b13, 4);
      /*$6B15*/ branchTarget = true; block_id = (s_a != 0x00) ? 320 : 321;
      break;
    case 320:  // $6B15
      /*$6B15*/ CYCLES_EDGE(0x6b15, 1);
                branchTarget = true; block_id = 322;
      break;
    case 321:  // $6B17
      /*$6B17*/ CYCLES(0x6b17, 6);
                ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
                block_id = 322;
      break;
    case 322:  // $6B1A
      /*$6B1A*/ CYCLES(0x6b1a, 18);
                s_a = ram_peek(0x6638);
      /*$6B1D*/ s_status_c = 0x01;
      /*$6B1E*/ block_id = s_status_d ? 324 : 323;
      break;
    case 323:  // $6B1E
      /*$6B1E*/ tmp6_U16 = s_a;
                tmp3_U16 = (tmp6_U16 - 0x0001) - (uint8_t)(0x01 - s_status_c);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)0xfffe);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 325;
      break;
    case 324:  // $6B1E
      /*$6B1E*/ tmp3_U16 = sbc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
                block_id = 325;
      break;
    case 325:  // $6B20
      /*$6B20*/ s_y = ram_peek(0x6637);
      /*$6B23*/ FUNC_SCRN(0x6b25);
      /*$6B26*/ CYCLES(0x6b26, 4);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x00);
      /*$6B28*/ branchTarget = true; block_id = (tmp1_U8 != 0x00) ? 326 : 327;
      break;
    case 326:  // $6B28
      /*$6B28*/ CYCLES_EDGE(0x6b28, 1);
                branchTarget = true; block_id = 328;
      break;
    case 327:  // $6B2A
      /*$6B2A*/ CYCLES(0x6b2a, 6);
                ram_poke(0x6c4a, (uint8_t)(ram_peek(0x6c4a) + 0x01));
                block_id = 328;
      break;
    case 328:  // $6B2D
      /*$6B2D*/ CYCLES(0x6b2d, 6);
      /*$6B30*/ branchTarget = true; block_id = !ram_peek(0x6c4a) ? 329 : 330;
      break;
    case 329:  // $6B30
      /*$6B30*/ CYCLES_EDGE(0x6b30, 1);
      /*$6B35*/ CYCLES(0x6b35, 8);
                s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_a = 0x01;
      /*$6B37*/ branchTarget = true; block_id = find_block_id_func_t001(0x6b37, pop16() + 1);;
      break;
    case 330:  // $6B32
      /*$6B32*/ CYCLES(0x6b32, 8);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$6B34*/ branchTarget = true; block_id = find_block_id_func_t001(0x6b34, pop16() + 1);;
      break;
    case 331:  // $6B3D
      /*$6B3D*/ CYCLES(0x6b3d, 6);
                func_6c4b(0x6b3f);
                branchTarget = true; block_id = 332;
      break;
    case 332:  // $6B40
      /*$6B40*/ CYCLES(0x6b40, 26);
      /*$6B42*/ ram_poke(0x0000, 0x15);
      /*$6B46*/ ram_poke(0x0001, 0x02);
      /*$6B4A*/ ram_poke(0x0002, 0x00);
      /*$6B4E*/ ram_poke(0x0003, 0x01);
      /*$6B50*/ tmp1_U8 = ram_peek(0x6255);
                s_a = tmp1_U8;
      /*$6B53*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 333 : 334;
      break;
    case 333:  // $6B53
      /*$6B53*/ CYCLES_EDGE(0x6b53, 1);
                branchTarget = true; block_id = 335;
      break;
    case 334:  // $6B55
      /*$6B55*/ CYCLES(0x6b55, 8);
      /*$6B57*/ ram_poke(0x6255, 0xff);
      /*$6B5A*/ s_a = 0x70;
                block_id = 335;
      break;
    case 335:  // $6B5C
      /*$6B5C*/ CYCLES(0x6b5c, 18);
      /*$6B5E*/ ram_poke(0x0008, (s_a >> 0x02));
      /*$6B60*/ ram_poke(0x0008, (uint8_t)(ram_peek(0x0008) + 0x01));
      /*$6B62*/ func_615a(0x6b64);
                branchTarget = true; block_id = 336;
      break;
    case 336:  // $6B65
      /*$6B65*/ CYCLES(0x6b65, 16);
      /*$6B67*/ ram_poke(0x0002, 0x27);
      /*$6B6B*/ ram_poke(0x0003, 0x01);
      /*$6B6D*/ func_615a(0x6b6f);
                branchTarget = true; block_id = 337;
      break;
    case 337:  // $6B70
      /*$6B70*/ CYCLES(0x6b70, 30);
                ram_poke(0x0008, (uint8_t)(ram_peek(0x0008) + 0x01));
      /*$6B72*/ tmp1_U8 = ram_peek(0x0008);
      /*$6B74*/ push8(tmp1_U8);
      /*$6B75*/ ram_poke(0x0003, tmp1_U8);
      /*$6B79*/ ram_poke(0x0001, 0x0d);
      /*$6B7D*/ ram_poke(0x0008, 0x27);
      /*$6B7F*/ func_615a(0x6b81);
                branchTarget = true; block_id = 338;
      break;
    case 338:  // $6B82
      /*$6B82*/ CYCLES(0x6b82, 18);
                tmp1_U8 = pop8();
      /*$6B83*/ ram_poke(0x0003, tmp1_U8);
      /*$6B87*/ ram_poke(0x0002, 0x00);
      /*$6B89*/ func_615a(0x6b8b);
                branchTarget = true; block_id = 339;
      break;
    case 339:  // $6B8C
      /*$6B8C*/ CYCLES(0x6b8c, 7);
                s_a = 0x27;
      /*$6B8E*/ s_y = 0x14;
      /*$6B90*/ FUNC_SCRN(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x6b90, pop16() + 1);;
      break;
    case 340:  // $6B93
      /*$6B93*/ CYCLES(0x6b93, 6);
                func_6127(0x6b95);
                branchTarget = true; block_id = 341;
      break;
    case 341:  // $6B96
      /*$6B96*/ CYCLES(0x6b96, 22);
                s_x = 0x00;
      /*$6B98*/ tmp1_U8 = ram_peek(0x0003);
      /*$6B9A*/ ram_poke(0x0007, 0x00);
      /*$6B9F*/ ram_poke(0x0004, ram_peek((0x6000 + tmp1_U8)));
      /*$6BA4*/ ram_poke(0x0005, ram_peek((0x6030 + tmp1_U8)));
                block_id = 342;
      break;
    case 342:  // $6BA6
      /*$6BA6*/ CYCLES(0x6ba6, 85);
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
      /*$6BD1*/ block_id = s_status_d ? 344 : 343;
      break;
    case 343:  // $6BD1
      /*$6BD1*/ tmp3_U16 = s_a;
                tmp6_U16 = (tmp3_U16 + 0x0004) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)0x0004);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 345;
      break;
    case 344:  // $6BD1
      /*$6BD1*/ tmp6_U16 = adc_dec16(s_a, 0x04, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 345;
      break;
    case 345:  // $6BD3
      /*$6BD3*/ ram_poke(0x0005, s_a);
      /*$6BD5*/ tmp2_U8 = s_x;
                tmp1_U8 = tmp2_U8 != 0x04;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp2_U8 >= 0x04);
                s_status_n = ((uint8_t)(tmp2_U8 - 0x04) & 0x80);
      /*$6BD7*/ branchTarget = true; block_id = tmp1_U8 ? 346 : 347;
      break;
    case 346:  // $6BD7
      /*$6BD7*/ CYCLES_EDGE(0x6bd7, 1);
                branchTarget = true; block_id = 342;
      break;
    case 347:  // $6BD9
      /*$6BD9*/ CYCLES(0x6bd9, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x6bd9, pop16() + 1);;
      break;
    case 348:  // $6BDA
      /*$6BDA*/ CYCLES(0x6bda, 6);
                func_60e4(0x6bdc);
                branchTarget = true; block_id = 349;
      break;
    case 349:  // $6BDD
      /*$6BDD*/ CYCLES(0x6bdd, 6);
      /*$6BE0*/ branchTarget = true; block_id = !ram_peek(0x0305) ? 350 : 351;
      break;
    case 350:  // $6BE0
      /*$6BE0*/ CYCLES_EDGE(0x6be0, 1);
                branchTarget = true; block_id = 352;
      break;
    case 351:  // $6BE2
      /*$6BE2*/ CYCLES(0x6be2, 11);
      /*$6BE4*/ ram_poke(0x0000, 0x01);
      /*$6BE6*/ func_6b93(0x6be8);
                branchTarget = true; block_id = 352;
      break;
    case 352:  // $6BE9
      /*$6BE9*/ CYCLES(0x6be9, 12);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$6BEB*/ ram_poke(0x0305, 0x00);
      /*$6BEE*/ branchTarget = true; block_id = find_block_id_func_t001(0x6bee, pop16() + 1);;
      break;
    case 353:  // $6BEF
      /*$6BEF*/ CYCLES(0x6bef, 6);
                FUNC_MON_PLOT(0x6bf1);
                branchTarget = true; block_id = 354;
      break;
    case 354:  // $6BF2
      /*$6BF2*/ CYCLES(0x6bf2, 16);
                s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_a = 0x01;
      /*$6BF4*/ ram_poke(0x0305, 0x01);
      /*$6BF7*/ ram_poke(0x6c46, 0x01);
      /*$6BFA*/ branchTarget = true; block_id = find_block_id_func_t001(0x6bfa, pop16() + 1);;
      break;
    case 355:  // $6BFB
      /*$6BFB*/ CYCLES(0x6bfb, 6);
      /*$6BFD*/ ram_poke(0x6c48, 0x14);
                block_id = 356;
      break;
    case 356:  // $6C00
      /*$6C00*/ CYCLES(0x6c00, 6);
                tmp1_U8 = ram_peek(0x6c46);
                s_a = tmp1_U8;
      /*$6C03*/ branchTarget = true; block_id = !tmp1_U8 ? 357 : 358;
      break;
    case 357:  // $6C03
      /*$6C03*/ CYCLES_EDGE(0x6c03, 1);
                branchTarget = true; block_id = 363;
      break;
    case 358:  // $6C05
      /*$6C05*/ CYCLES(0x6c05, 4);
      /*$6C07*/ branchTarget = true; block_id = (s_a >= 0x80) ? 359 : 360;
      break;
    case 359:  // $6C07
      /*$6C07*/ CYCLES_EDGE(0x6c07, 1);
                branchTarget = true; block_id = 363;
      break;
    case 360:  // $6C09
      /*$6C09*/ CYCLES(0x6c09, 8);
                tmp1_U8 = (uint8_t)(ram_peek(0x6c47) - 0x01);
                ram_poke(0x6c47, tmp1_U8);
      /*$6C0C*/ branchTarget = true; block_id = tmp1_U8 ? 361 : 362;
      break;
    case 361:  // $6C0C
      /*$6C0C*/ CYCLES_EDGE(0x6c0c, 1);
                branchTarget = true; block_id = 363;
      break;
    case 362:  // $6C0E
      /*$6C0E*/ CYCLES(0x6c0e, 28);
                tmp1_U8 = ram_peek(0x6c49);
                s_y = tmp1_U8;
      /*$6C11*/ tmp1_U8 = peek((0xc000 + tmp1_U8));
      /*$6C14*/ ram_poke(0x6c46, (uint8_t)(ram_peek(0x6c46) + 0x01));
      /*$6C17*/ ram_poke(0x6c46, (uint8_t)(ram_peek(0x6c46) + 0x01));
      /*$6C1D*/ ram_poke(0x6c47, ram_peek(0x6c46));
                block_id = 363;
      break;
    case 363:  // $6C20
      /*$6C20*/ CYCLES(0x6c20, 8);
      /*$6C23*/ tmp1_U8 = ram_peek(0x6c46) >= 0x80;
                s_status_c = tmp1_U8;
      /*$6C25*/ branchTarget = true; block_id = !tmp1_U8 ? 364 : 365;
      break;
    case 364:  // $6C25
      /*$6C25*/ CYCLES_EDGE(0x6c25, 1);
                branchTarget = true; block_id = 366;
      break;
    case 365:  // $6C27
      /*$6C27*/ CYCLES(0x6c27, 6);
      /*$6C29*/ ram_poke(0x6c46, 0x00);
                block_id = 366;
      break;
    case 366:  // $6C2C
      /*$6C2C*/ CYCLES(0x6c2c, 12);
      /*$6C2E*/ ram_poke(0x6c49, 0x20);
      /*$6C31*/ tmp1_U8 = ram_peek(0x0302);
                s_a = tmp1_U8;
      /*$6C34*/ branchTarget = true; block_id = tmp1_U8 ? 367 : 368;
      break;
    case 367:  // $6C34
      /*$6C34*/ CYCLES_EDGE(0x6c34, 1);
                branchTarget = true; block_id = 371;
      break;
    case 368:  // $6C36
      /*$6C36*/ CYCLES(0x6c36, 6);
                tmp1_U8 = ram_peek(0x69c2);
                s_a = tmp1_U8;
      /*$6C39*/ branchTarget = true; block_id = tmp1_U8 ? 369 : 370;
      break;
    case 369:  // $6C39
      /*$6C39*/ CYCLES_EDGE(0x6c39, 1);
                branchTarget = true; block_id = 371;
      break;
    case 370:  // $6C3B
      /*$6C3B*/ CYCLES(0x6c3b, 6);
                s_a = 0x30;
      /*$6C3D*/ ram_poke(0x6c49, 0x30);
                block_id = 371;
      break;
    case 371:  // $6C40
      /*$6C40*/ CYCLES(0x6c40, 8);
                tmp1_U8 = (uint8_t)(ram_peek(0x6c48) - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x6c48, tmp1_U8);
      /*$6C43*/ branchTarget = true; block_id = tmp1_U8 ? 372 : 373;
      break;
    case 372:  // $6C43
      /*$6C43*/ CYCLES_EDGE(0x6c43, 1);
                branchTarget = true; block_id = 356;
      break;
    case 373:  // $6C45
      /*$6C45*/ CYCLES(0x6c45, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x6c45, pop16() + 1);;
      break;
    case 374:  // $6C4B
      /*$6C4B*/ CYCLES(0x6c4b, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x000e) + 0x01);
                ram_poke(0x000e, tmp1_U8);
      /*$6C4D*/ branchTarget = true; block_id = tmp1_U8 ? 375 : 376;
      break;
    case 375:  // $6C4D
      /*$6C4D*/ CYCLES_EDGE(0x6c4d, 1);
                branchTarget = true; block_id = 377;
      break;
    case 376:  // $6C4F
      /*$6C4F*/ CYCLES(0x6c4f, 5);
                ram_poke(0x000f, (uint8_t)(ram_peek(0x000f) + 0x01));
                block_id = 377;
      break;
    case 377:  // $6C51
      /*$6C51*/ CYCLES(0x6c51, 9);
                s_y = 0x00;
      /*$6C53*/ tmp1_U8 = peek(ram_peek16al(0x000e));
                s_status_not_z = tmp1_U8;
                tmp2_U8 = tmp1_U8 & 0x80;
                s_status_n = tmp2_U8;
                s_a = tmp1_U8;
      /*$6C55*/ branchTarget = true; block_id = !tmp2_U8 ? 378 : 379;
      break;
    case 378:  // $6C55
      /*$6C55*/ CYCLES_EDGE(0x6c55, 1);
      /*$6C62*/ CYCLES(0x6c62, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x6c62, pop16() + 1);;
      break;
    case 379:  // $6C57
      /*$6C57*/ CYCLES(0x6c57, 13);
      /*$6C59*/ ram_poke(0x000e, 0x00);
      /*$6C5D*/ ram_poke(0x000f, 0x18);
      /*$6C5F*/ branchTarget = true; block_id = 377;
      break;
    case 380:  // $6C72
      /*$6C72*/ CYCLES(0x6c72, 6);
                func_6594(0x6c74);
                branchTarget = true; block_id = 381;
      break;
    case 381:  // $6C75
      /*$6C75*/ CYCLES(0x6c75, 9);
                push8(s_a);
      /*$6C79*/ branchTarget = true; block_id = !ram_peek(0x0302) ? 382 : 383;
      break;
    case 382:  // $6C79
      /*$6C79*/ CYCLES_EDGE(0x6c79, 1);
      /*$6C90*/ CYCLES(0x6c90, 6);
                tmp2_U8 = pop8();
                s_a = tmp2_U8;
      /*$6C91*/ s_x = 0x05;
                block_id = 391;
      break;
    case 383:  // $6C7B
      /*$6C7B*/ CYCLES(0x6c7b, 6);
      /*$6C7E*/ branchTarget = true; block_id = !ram_peek(0x6c71) ? 384 : 385;
      break;
    case 384:  // $6C7E
      /*$6C7E*/ CYCLES_EDGE(0x6c7e, 1);
                branchTarget = true; block_id = 388;
      break;
    case 385:  // $6C80
      /*$6C80*/ CYCLES(0x6c80, 6);
                tmp1_U8 = io_peek(0xc061);
      /*$6C83*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 386 : 387;
      break;
    case 386:  // $6C83
      /*$6C83*/ CYCLES_EDGE(0x6c83, 1);
                branchTarget = true; block_id = 388;
      break;
    case 387:  // $6C85
      /*$6C85*/ CYCLES(0x6c85, 12);
                tmp1_U8 = pop8();
      /*$6C86*/ s_status_not_z = 0x92;
                s_status_n = 0x80;
                s_a = 0x92;
      /*$6C88*/ branchTarget = true; block_id = find_block_id_func_t001(0x6c88, pop16() + 1);;
      break;
    case 388:  // $6C89
      /*$6C89*/ CYCLES(0x6c89, 6);
                tmp2_U8 = pop8();
                s_status_not_z = tmp2_U8;
                tmp1_U8 = tmp2_U8 & 0x80;
                s_status_n = tmp1_U8;
                s_a = tmp2_U8;
      /*$6C8A*/ branchTarget = true; block_id = !tmp1_U8 ? 389 : 390;
      break;
    case 389:  // $6C8A
      /*$6C8A*/ CYCLES_EDGE(0x6c8a, 1);
      /*$6C8F*/ CYCLES(0x6c8f, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x6c8f, pop16() + 1);;
      break;
    case 390:  // $6C8C
      /*$6C8C*/ CYCLES(0x6c8c, 8);
                s_status_not_z = 0x92;
                s_status_n = 0x80;
                s_a = 0x92;
      /*$6C8E*/ branchTarget = true; block_id = find_block_id_func_t001(0x6c8e, pop16() + 1);;
      break;
    case 391:  // $6C93
      /*$6C93*/ CYCLES(0x6c93, 6);
      /*$6C96*/ branchTarget = true; block_id = !(s_a != ram_peek((0x6c63 + s_x))) ? 392 : 393;
      break;
    case 392:  // $6C96
      /*$6C96*/ CYCLES_EDGE(0x6c96, 1);
      /*$6C9E*/ CYCLES(0x6c9e, 4);
                s_a = ram_peek((0x6c6a + s_x));
                block_id = 396;
      break;
    case 393:  // $6C98
      /*$6C98*/ CYCLES(0x6c98, 4);
                tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp1_U8;
      /*$6C99*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 394 : 395;
      break;
    case 394:  // $6C99
      /*$6C99*/ CYCLES_EDGE(0x6c99, 1);
                branchTarget = true; block_id = 391;
      break;
    case 395:  // $6C9B
      /*$6C9B*/ CYCLES(0x6c9b, 3);
                branchTarget = true; block_id = 396;
      break;
    case 396:  // $6CA1
      /*$6CA1*/ CYCLES(0x6ca1, 4);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x80);
      /*$6CA3*/ branchTarget = true; block_id = (tmp1_U8 != 0x80) ? 397 : 398;
      break;
    case 397:  // $6CA3
      /*$6CA3*/ CYCLES_EDGE(0x6ca3, 1);
      /*$6CAB*/ CYCLES(0x6cab, 4);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x8b);
      /*$6CAD*/ branchTarget = true; block_id = (tmp1_U8 != 0x8b) ? 399 : 400;
      break;
    case 398:  // $6CA5
      /*$6CA5*/ CYCLES(0x6ca5, 12);
                s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_a = 0x01;
      /*$6CA7*/ ram_poke(0x6c71, 0x01);
      /*$6CAA*/ branchTarget = true; block_id = find_block_id_func_t001(0x6caa, pop16() + 1);;
      break;
    case 399:  // $6CAD
      /*$6CAD*/ CYCLES_EDGE(0x6cad, 1);
      /*$6CB5*/ CYCLES(0x6cb5, 4);
                tmp1_U8 = s_a;
                s_status_not_z = (tmp1_U8 != 0x00);
                s_status_c = (tmp1_U8 >= 0x00);
                tmp1_U8 = tmp1_U8 & 0x80;
                s_status_n = tmp1_U8;
      /*$6CB7*/ branchTarget = true; block_id = !tmp1_U8 ? 401 : 402;
      break;
    case 400:  // $6CAF
      /*$6CAF*/ CYCLES(0x6caf, 12);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$6CB1*/ ram_poke(0x6c71, 0x00);
      /*$6CB4*/ branchTarget = true; block_id = find_block_id_func_t001(0x6cb4, pop16() + 1);;
      break;
    case 401:  // $6CB7
      /*$6CB7*/ CYCLES_EDGE(0x6cb7, 1);
      /*$6CBA*/ CYCLES(0x6cba, 6);
                tmp1_U8 = ram_peek(0x6c71);
                s_x = tmp1_U8;
      /*$6CBD*/ branchTarget = true; block_id = tmp1_U8 ? 403 : 404;
      break;
    case 402:  // $6CB9
      /*$6CB9*/ CYCLES(0x6cb9, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x6cb9, pop16() + 1);;
      break;
    case 403:  // $6CBD
      /*$6CBD*/ CYCLES_EDGE(0x6cbd, 1);
      /*$6CC2*/ CYCLES(0x6cc2, 12);
                s_y = 0x00;
      /*$6CC4*/ tmp1_U8 = io_peek(0xc05b);
      /*$6CC7*/ tmp1_U8 = io_peek(0xc062);
      /*$6CCA*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 405 : 406;
      break;
    case 404:  // $6CBF
      /*$6CBF*/ CYCLES(0x6cbf, 8);
                tmp1_U8 = s_a;
                s_status_not_z = (tmp1_U8 != 0x00);
                s_status_c = (tmp1_U8 >= 0x00);
                s_status_n = (tmp1_U8 & 0x80);
      /*$6CC1*/ branchTarget = true; block_id = find_block_id_func_t001(0x6cc1, pop16() + 1);;
      break;
    case 405:  // $6CCA
      /*$6CCA*/ CYCLES_EDGE(0x6cca, 1);
                branchTarget = true; block_id = 407;
      break;
    case 406:  // $6CCC
      /*$6CCC*/ CYCLES(0x6ccc, 4);
                s_x = 0x00;
      /*$6CCE*/ s_y = (uint8_t)(s_y + 0x01);
                block_id = 407;
      break;
    case 407:  // $6CCF
      /*$6CCF*/ CYCLES(0x6ccf, 6);
                tmp1_U8 = io_peek(0xc063);
      /*$6CD2*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 408 : 409;
      break;
    case 408:  // $6CD2
      /*$6CD2*/ CYCLES_EDGE(0x6cd2, 1);
                branchTarget = true; block_id = 410;
      break;
    case 409:  // $6CD4
      /*$6CD4*/ CYCLES(0x6cd4, 4);
                s_x = 0x03;
      /*$6CD6*/ s_y = (uint8_t)(s_y + 0x01);
                block_id = 410;
      break;
    case 410:  // $6CD7
      /*$6CD7*/ CYCLES(0x6cd7, 10);
                tmp1_U8 = io_peek(0xc05a);
      /*$6CDA*/ tmp1_U8 = io_peek(0xc062);
      /*$6CDD*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 411 : 412;
      break;
    case 411:  // $6CDD
      /*$6CDD*/ CYCLES_EDGE(0x6cdd, 1);
                branchTarget = true; block_id = 413;
      break;
    case 412:  // $6CDF
      /*$6CDF*/ CYCLES(0x6cdf, 4);
                s_x = 0x01;
      /*$6CE1*/ s_y = (uint8_t)(s_y + 0x01);
                block_id = 413;
      break;
    case 413:  // $6CE2
      /*$6CE2*/ CYCLES(0x6ce2, 6);
                tmp1_U8 = io_peek(0xc063);
      /*$6CE5*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 414 : 415;
      break;
    case 414:  // $6CE5
      /*$6CE5*/ CYCLES_EDGE(0x6ce5, 1);
                branchTarget = true; block_id = 416;
      break;
    case 415:  // $6CE7
      /*$6CE7*/ CYCLES(0x6ce7, 4);
                s_x = 0x02;
      /*$6CE9*/ s_y = (uint8_t)(s_y + 0x01);
                block_id = 416;
      break;
    case 416:  // $6CEA
      /*$6CEA*/ CYCLES(0x6cea, 4);
                tmp1_U8 = s_y;
                s_status_c = (tmp1_U8 >= 0x01);
      /*$6CEC*/ branchTarget = true; block_id = !(tmp1_U8 != 0x01) ? 417 : 418;
      break;
    case 417:  // $6CEC
      /*$6CEC*/ CYCLES_EDGE(0x6cec, 1);
      /*$6CF1*/ CYCLES(0x6cf1, 10);
                tmp1_U8 = ram_peek((0x6c6a + s_x));
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$6CF4*/ branchTarget = true; block_id = find_block_id_func_t001(0x6cf4, pop16() + 1);;
      break;
    case 418:  // $6CEE
      /*$6CEE*/ CYCLES(0x6cee, 8);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$6CF0*/ branchTarget = true; block_id = find_block_id_func_t001(0x6cf0, pop16() + 1);;
      break;
    case 419:  // $7000
      /*$7000*/ CYCLES(0x7000, 6);
      /*$7002*/ push8(ram_peek(0x0003));
                block_id = 420;
      break;
    case 420:  // $7003
      /*$7003*/ CYCLES(0x7003, 12);
                s_a = ram_peek(0x0003);
      /*$7005*/ s_y = ram_peek(0x0002);
      /*$7007*/ FUNC_MON_PLOT(0x7009);
                branchTarget = true; block_id = 421;
      break;
    case 421:  // $700A
      /*$700A*/ CYCLES(0x700a, 8);
                tmp1_U8 = ram_peek(0x0003);
      /*$700C*/ tmp2_U8 = ram_peek(0x0008);
                s_status_c = (tmp1_U8 >= tmp2_U8);
      /*$700E*/ branchTarget = true; block_id = !(tmp1_U8 != tmp2_U8) ? 422 : 423;
      break;
    case 422:  // $700E
      /*$700E*/ CYCLES_EDGE(0x700e, 1);
      /*$7015*/ CYCLES(0x7015, 13);
                tmp2_U8 = pop8();
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$7016*/ ram_poke(0x0003, tmp2_U8);
      /*$7018*/ branchTarget = true; block_id = find_block_id_func_t001(0x7018, pop16() + 1);;
      break;
    case 423:  // $7010
      /*$7010*/ CYCLES(0x7010, 8);
                ram_poke(0x0003, (uint8_t)(ram_peek(0x0003) + 0x01));
      /*$7012*/ branchTarget = true; block_id = 420;
      break;
    case 424:  // $7019
      /*$7019*/ CYCLES(0x7019, 14);
                s_y = 0x00;
      /*$701B*/ tmp2_U8 = peek(ram_peek16al(0x000a));
                s_a = tmp2_U8;
      /*$701D*/ tmp2_U8 = (uint8_t)(ram_peek(0x000a) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x000a, tmp2_U8);
      /*$701F*/ branchTarget = true; block_id = tmp2_U8 ? 425 : 426;
      break;
    case 425:  // $701F
      /*$701F*/ CYCLES_EDGE(0x701f, 1);
                branchTarget = true; block_id = 427;
      break;
    case 426:  // $7021
      /*$7021*/ CYCLES(0x7021, 5);
                tmp2_U8 = (uint8_t)(ram_peek(0x000b) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x000b, tmp2_U8);
                block_id = 427;
      break;
    case 427:  // $7023
      /*$7023*/ CYCLES(0x7023, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x7023, pop16() + 1);;
      break;
    case 428:  // $7024
      /*$7024*/ CYCLES(0x7024, 2);
                branchTarget = true; block_id = !s_status_not_z ? 429 : 430;
      break;
    case 429:  // $7024
      /*$7024*/ CYCLES_EDGE(0x7024, 1);
                branchTarget = true; block_id = 431;
      break;
    case 430:  // $7026
      /*$7026*/ CYCLES(0x7026, 2);
                s_a = 0x05;
                block_id = 431;
      break;
    case 431:  // $7028
      /*$7028*/ CYCLES(0x7028, 3);
                FUNC_SETCOL(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x7028, pop16() + 1);;
      break;
    case 432:  // $702B
      /*$702B*/ CYCLES(0x702b, 12);
      /*$702D*/ ram_poke(0x0004, 0x00);
      /*$7031*/ ram_poke(0x0005, 0x20);
      /*$7033*/ s_y = 0x00;
                block_id = 433;
      break;
    case 433:  // $7035
      /*$7035*/ CYCLES(0x7035, 12);
      /*$7037*/ tmp2_U8 = s_y;
                poke((ram_peek16al(0x0004) + tmp2_U8), 0x00);
      /*$7039*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_y = tmp2_U8;
      /*$703A*/ branchTarget = true; block_id = tmp2_U8 ? 434 : 435;
      break;
    case 434:  // $703A
      /*$703A*/ CYCLES_EDGE(0x703a, 1);
                branchTarget = true; block_id = 433;
      break;
    case 435:  // $703C
      /*$703C*/ CYCLES(0x703c, 12);
                ram_poke(0x0005, (uint8_t)(ram_peek(0x0005) + 0x01));
      /*$703E*/ tmp2_U8 = ram_peek(0x0005);
                s_a = tmp2_U8;
      /*$7040*/ tmp1_U8 = tmp2_U8 != 0x40;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp2_U8 >= 0x40);
                s_status_n = ((uint8_t)(tmp2_U8 - 0x40) & 0x80);
      /*$7042*/ branchTarget = true; block_id = tmp1_U8 ? 436 : 437;
      break;
    case 436:  // $7042
      /*$7042*/ CYCLES_EDGE(0x7042, 1);
                branchTarget = true; block_id = 433;
      break;
    case 437:  // $7044
      /*$7044*/ CYCLES(0x7044, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x7044, pop16() + 1);;
      break;
    case 438:  // $7045
      /*$7045*/ CYCLES(0x7045, 6);
                func_702b(0x7047);
                branchTarget = true; block_id = 439;
      break;
    case 439:  // $7048
      /*$7048*/ CYCLES(0x7048, 32);
      /*$704A*/ ram_poke(0x6c46, 0x00);
      /*$704D*/ tmp1_U8 = io_peek(0xc050);
      /*$7050*/ tmp1_U8 = io_peek(0xc057);
      /*$7053*/ tmp1_U8 = io_peek(0xc055);
      /*$7056*/ tmp1_U8 = io_peek(0xc052);
      /*$705B*/ ram_poke(0x0002, 0x04);
      /*$705F*/ ram_poke(0x0003, 0x00);
                block_id = 440;
      break;
    case 440:  // $7061
      /*$7061*/ CYCLES(0x7061, 4);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp1_U8;
      /*$7062*/ branchTarget = true; block_id = tmp1_U8 ? 441 : 442;
      break;
    case 441:  // $7062
      /*$7062*/ CYCLES_EDGE(0x7062, 1);
                branchTarget = true; block_id = 440;
      break;
    case 442:  // $7064
      /*$7064*/ CYCLES(0x7064, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x0003) - 0x01);
                ram_poke(0x0003, tmp1_U8);
      /*$7066*/ branchTarget = true; block_id = tmp1_U8 ? 443 : 444;
      break;
    case 443:  // $7066
      /*$7066*/ CYCLES_EDGE(0x7066, 1);
                branchTarget = true; block_id = 440;
      break;
    case 444:  // $7068
      /*$7068*/ CYCLES(0x7068, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x0002) - 0x01);
                ram_poke(0x0002, tmp1_U8);
      /*$706A*/ branchTarget = true; block_id = tmp1_U8 ? 445 : 446;
      break;
    case 445:  // $706A
      /*$706A*/ CYCLES_EDGE(0x706a, 1);
                branchTarget = true; block_id = 440;
      break;
    case 446:  // $706C
      /*$706C*/ CYCLES(0x706c, 13);
      /*$706E*/ ram_poke(0x0003, 0x27);
      /*$7070*/ s_status_not_z = 0x00;
                s_a = 0x00;
      /*$7072*/ func_7024(0x7074);
                branchTarget = true; block_id = 447;
      break;
    case 447:  // $7075
      /*$7075*/ CYCLES(0x7075, 16);
      /*$7077*/ ram_poke(0x002c, 0x27);
      /*$7079*/ s_y = 0x00;
      /*$707B*/ s_a = ram_peek(0x0003);
      /*$707D*/ FUNC_HLINE(0x707f);
                branchTarget = true; block_id = 448;
      break;
    case 448:  // $7080
      /*$7080*/ CYCLES(0x7080, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x0003) - 0x01);
                ram_poke(0x0003, tmp1_U8);
      /*$7082*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 449 : 450;
      break;
    case 449:  // $7082
      /*$7082*/ CYCLES_EDGE(0x7082, 1);
                branchTarget = true; block_id = 447;
      break;
    case 450:  // $7084
      /*$7084*/ CYCLES(0x7084, 21);
      /*$7086*/ ram_poke(0x0022, 0x14);
      /*$708A*/ ram_poke(0x0000, 0x15);
      /*$708C*/ s_status_not_z = 0x0d;
                s_a = 0x0d;
      /*$708E*/ ram_poke(0x0001, 0x0d);
      /*$7090*/ func_7024(0x7092);
                branchTarget = true; block_id = 451;
      break;
    case 451:  // $7093
      /*$7093*/ CYCLES(0x7093, 6);
      /*$7096*/ branchTarget = true; block_id = !ram_peek(0x0301) ? 452 : 453;
      break;
    case 452:  // $7096
      /*$7096*/ CYCLES_EDGE(0x7096, 1);
                branchTarget = true; block_id = 457;
      break;
    case 453:  // $7098
      /*$7098*/ CYCLES(0x7098, 10);
                s_a = 0x01;
      /*$709A*/ s_y = 0x01;
      /*$709B*/ FUNC_MON_PLOT(0x709d);
                branchTarget = true; block_id = 454;
      break;
    case 454:  // $709E
      /*$709E*/ CYCLES(0x709e, 8);
                tmp1_U8 = ram_peek(0x0301);
      /*$70A1*/ s_status_c = (tmp1_U8 >= 0x01);
      /*$70A3*/ branchTarget = true; block_id = !(tmp1_U8 != 0x01) ? 455 : 456;
      break;
    case 455:  // $70A3
      /*$70A3*/ CYCLES_EDGE(0x70a3, 1);
                branchTarget = true; block_id = 457;
      break;
    case 456:  // $70A5
      /*$70A5*/ CYCLES(0x70a5, 10);
                s_a = 0x01;
      /*$70A7*/ s_y = 0x26;
      /*$70A9*/ FUNC_MON_PLOT(0x70ab);
                branchTarget = true; block_id = 457;
      break;
    case 457:  // $70AC
      /*$70AC*/ CYCLES(0x70ac, 10);
                s_y = 0x00;
      /*$70AE*/ s_a = 0x00;
      /*$70B0*/ FUNC_HLINE(0x70b2);
                branchTarget = true; block_id = 458;
      break;
    case 458:  // $70B3
      /*$70B3*/ CYCLES(0x70b3, 10);
                s_y = 0x00;
      /*$70B5*/ s_a = 0x27;
      /*$70B7*/ FUNC_HLINE(0x70b9);
                branchTarget = true; block_id = 459;
      break;
    case 459:  // $70BA
      /*$70BA*/ CYCLES(0x70ba, 21);
      /*$70BC*/ ram_poke(0x0002, 0x00);
      /*$70C0*/ ram_poke(0x0003, 0x00);
      /*$70C4*/ ram_poke(0x0008, 0x27);
      /*$70C6*/ func_7000(0x70c8);
                branchTarget = true; block_id = 460;
      break;
    case 460:  // $70C9
      /*$70C9*/ CYCLES(0x70c9, 16);
      /*$70CB*/ ram_poke(0x0003, 0x00);
      /*$70CF*/ ram_poke(0x0002, 0x27);
      /*$70D1*/ func_7000(0x70d3);
                branchTarget = true; block_id = 461;
      break;
    case 461:  // $70D4
      /*$70D4*/ CYCLES(0x70d4, 19);
      /*$70D6*/ ram_poke(0x0002, 0x00);
      /*$70D8*/ ram_poke(0x0003, 0x00);
      /*$70DC*/ ram_poke(0x0008, 0x27);
      /*$70DE*/ func_6148(0x70e0);
                branchTarget = true; block_id = 462;
      break;
    case 462:  // $70E1
      /*$70E1*/ CYCLES(0x70e1, 16);
      /*$70E3*/ ram_poke(0x0002, 0x00);
      /*$70E7*/ ram_poke(0x0003, 0x27);
      /*$70E9*/ func_6148(0x70eb);
                branchTarget = true; block_id = 463;
      break;
    case 463:  // $70EC
      /*$70EC*/ CYCLES(0x70ec, 14);
      /*$70EE*/ ram_poke(0x0002, 0x00);
      /*$70F0*/ ram_poke(0x0003, 0x00);
      /*$70F2*/ func_615a(0x70f4);
                branchTarget = true; block_id = 464;
      break;
    case 464:  // $70F5
      /*$70F5*/ CYCLES(0x70f5, 16);
      /*$70F7*/ ram_poke(0x0003, 0x00);
      /*$70FB*/ ram_poke(0x0002, 0x27);
      /*$70FD*/ func_615a(0x70ff);
                branchTarget = true; block_id = 465;
      break;
    case 465:  // $7100
      /*$7100*/ CYCLES(0x7100, 26);
      /*$7102*/ ram_poke(0x0001, 0x03);
      /*$7106*/ ram_poke(0x0003, 0x27);
      /*$710A*/ ram_poke(0x0002, 0x12);
      /*$710E*/ ram_poke(0x0008, 0x16);
      /*$7110*/ func_6148(0x7112);
                branchTarget = true; block_id = 466;
      break;
    case 466:  // $7113
      /*$7113*/ CYCLES(0x7113, 14);
                s_x = ram_peek(0x0303);
      /*$7118*/ ram_poke(0x000a, 0x00);
      /*$711C*/ ram_poke(0x000b, 0x80);
                block_id = 467;
      break;
    case 467:  // $711E
      /*$711E*/ CYCLES(0x711e, 4);
                tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp1_U8;
      /*$711F*/ branchTarget = true; block_id = !tmp1_U8 ? 468 : 469;
      break;
    case 468:  // $711F
      /*$711F*/ CYCLES_EDGE(0x711f, 1);
                branchTarget = true; block_id = 472;
      break;
    case 469:  // $7121
      /*$7121*/ CYCLES(0x7121, 6);
                func_7019(0x7123);
      /*$7124*/ CYCLES(0x7124, 4);
      /*$7126*/ branchTarget = true; block_id = !(s_a != 0x2a) ? 470 : 471;
      break;
    case 470:  // $7126
      /*$7126*/ CYCLES_EDGE(0x7126, 1);
                branchTarget = true; block_id = 467;
      break;
    case 471:  // $7128
      /*$7128*/ CYCLES(0x7128, 3);
                branchTarget = true; block_id = 469;
      break;
    case 472:  // $712B
      /*$712B*/ CYCLES(0x712b, 6);
                func_7019(0x712d);
                branchTarget = true; block_id = 473;
      break;
    case 473:  // $712E
      /*$712E*/ CYCLES(0x712e, 4);
      /*$7130*/ branchTarget = true; block_id = (s_a != 0x45) ? 474 : 475;
      break;
    case 474:  // $7130
      /*$7130*/ CYCLES_EDGE(0x7130, 1);
      /*$713A*/ CYCLES(0x713a, 4);
      /*$713C*/ branchTarget = true; block_id = (s_a != 0x48) ? 476 : 477;
      break;
    case 475:  // $7132
      /*$7132*/ CYCLES(0x7132, 9);
      /*$7134*/ ram_poke(0x0303, 0x01);
      /*$7137*/ branchTarget = true; block_id = 466;
      break;
    case 476:  // $713C
      /*$713C*/ CYCLES_EDGE(0x713c, 1);
      /*$7168*/ CYCLES(0x7168, 4);
      /*$716A*/ branchTarget = true; block_id = (s_a != 0x56) ? 478 : 479;
      break;
    case 477:  // $713E
      /*$713E*/ CYCLES(0x713e, 6);
                func_7019(0x7140);
      /*$7141*/ CYCLES(0x7141, 9);
                ram_poke(0x0001, s_a);
      /*$7143*/ func_7019(0x7145);
      /*$7146*/ CYCLES(0x7146, 9);
                ram_poke(0x0002, s_a);
      /*$7148*/ func_7019(0x714a);
      /*$714B*/ CYCLES(0x714b, 9);
                ram_poke(0x0008, s_a);
      /*$714D*/ func_7019(0x714f);
      /*$7150*/ CYCLES(0x7150, 12);
                ram_poke(0x0003, s_a);
      /*$7152*/ tmp1_U8 = ram_peek(0x0001);
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
      /*$7154*/ func_7024(0x7156);
      /*$7157*/ CYCLES(0x7157, 18);
      /*$7159*/ ram_poke(0x002c, ram_peek(0x0008));
      /*$715B*/ s_a = ram_peek(0x0003);
      /*$715D*/ s_y = ram_peek(0x0002);
      /*$715F*/ FUNC_HLINE(0x7161);
      /*$7162*/ CYCLES(0x7162, 6);
                func_6148(0x7164);
      /*$7165*/ CYCLES(0x7165, 3);
                branchTarget = true; block_id = 472;
      break;
    case 478:  // $716A
      /*$716A*/ CYCLES_EDGE(0x716a, 1);
      /*$718E*/ CYCLES(0x718e, 4);
      /*$7190*/ branchTarget = true; block_id = (s_a != 0x50) ? 480 : 481;
      break;
    case 479:  // $716C
      /*$716C*/ CYCLES(0x716c, 6);
                func_7019(0x716e);
      /*$716F*/ CYCLES(0x716f, 9);
                ram_poke(0x0001, s_a);
      /*$7171*/ func_7019(0x7173);
      /*$7174*/ CYCLES(0x7174, 9);
                ram_poke(0x0003, s_a);
      /*$7176*/ func_7019(0x7178);
      /*$7179*/ CYCLES(0x7179, 9);
                ram_poke(0x0008, s_a);
      /*$717B*/ func_7019(0x717d);
      /*$717E*/ CYCLES(0x717e, 12);
                ram_poke(0x0002, s_a);
      /*$7180*/ tmp1_U8 = ram_peek(0x0001);
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
      /*$7182*/ func_7024(0x7184);
      /*$7185*/ CYCLES(0x7185, 6);
                func_7000(0x7187);
      /*$7188*/ CYCLES(0x7188, 6);
                func_615a(0x718a);
      /*$718B*/ CYCLES(0x718b, 3);
                branchTarget = true; block_id = 472;
      break;
    case 480:  // $7190
      /*$7190*/ CYCLES_EDGE(0x7190, 1);
      /*$71B3*/ CYCLES(0x71b3, 4);
      /*$71B5*/ branchTarget = true; block_id = (s_a != 0x54) ? 482 : 483;
      break;
    case 481:  // $7192
      /*$7192*/ CYCLES(0x7192, 6);
                func_7019(0x7194);
      /*$7195*/ CYCLES(0x7195, 9);
                ram_poke(0x0001, s_a);
      /*$7197*/ func_7019(0x7199);
      /*$719A*/ CYCLES(0x719a, 9);
                ram_poke(0x0002, s_a);
      /*$719C*/ func_7019(0x719e);
      /*$719F*/ CYCLES(0x719f, 12);
                ram_poke(0x0003, s_a);
      /*$71A1*/ tmp1_U8 = ram_peek(0x0001);
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
      /*$71A3*/ func_7024(0x71a5);
      /*$71A6*/ CYCLES(0x71a6, 12);
                s_a = ram_peek(0x0003);
      /*$71A8*/ s_y = ram_peek(0x0002);
      /*$71AA*/ FUNC_MON_PLOT(0x71ac);
      /*$71AD*/ CYCLES(0x71ad, 6);
                func_60e4(0x71af);
      /*$71B0*/ CYCLES(0x71b0, 3);
                branchTarget = true; block_id = 472;
      break;
    case 482:  // $71B5
      /*$71B5*/ CYCLES_EDGE(0x71b5, 1);
      /*$71C0*/ CYCLES(0x71c0, 4);
                tmp1_U8 = s_a;
                tmp2_U8 = tmp1_U8 != 0x2a;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp1_U8 >= 0x2a);
                s_status_n = ((uint8_t)(tmp1_U8 - 0x2a) & 0x80);
      /*$71C2*/ branchTarget = true; block_id = !tmp2_U8 ? 485 : 486;
      break;
    case 483:  // $71B7
      /*$71B7*/ CYCLES(0x71b7, 6);
                func_7019(0x71b9);
                branchTarget = true; block_id = 484;
      break;
    case 484:  // $71BA
      /*$71BA*/ CYCLES(0x71ba, 7);
                ram_poke(0x0304, s_a);
      /*$71BD*/ branchTarget = true; block_id = 472;
      break;
    case 485:  // $71C2
      /*$71C2*/ CYCLES_EDGE(0x71c2, 1);
      /*$71C7*/ CYCLES(0x71c7, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x71c7, pop16() + 1);;
      break;
    case 486:  // $71C4
      /*$71C4*/ CYCLES(0x71c4, 3);
                branchTarget = true; block_id = 472;
      break;
    case 487:  // $71CD
      /*$71CD*/ CYCLES(0x71cd, 20);
      /*$71CF*/ ram_poke(0x71cb, 0x00);
      /*$71D2*/ ram_poke(0x71cc, 0x00);
      /*$71D5*/ s_x = ram_peek(0x0301);
      /*$71D8*/ s_y = ram_peek(0x0303);
      /*$71DB*/ s_status_d = 0x01;
                block_id = 488;
      break;
    case 488:  // $71DC
      /*$71DC*/ CYCLES(0x71dc, 28);
                s_a = ram_peek((0x71c8 + s_x));
      /*$71DF*/ s_status_c = 0x00;
      /*$71E0*/ block_id = s_status_d ? 490 : 489;
      break;
    case 489:  // $71E0
      /*$71E0*/ tmp6_U16 = (s_a + ram_peek(0x71cb)) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 491;
      break;
    case 490:  // $71E0
      /*$71E0*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x71cb), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 491;
      break;
    case 491:  // $71E3
      /*$71E3*/ ram_poke(0x71cb, s_a);
      /*$71E6*/ s_a = ram_peek(0x71cc);
      /*$71E9*/ block_id = s_status_d ? 493 : 492;
      break;
    case 492:  // $71E9
      /*$71E9*/ tmp6_U16 = s_a;
                tmp3_U16 = tmp6_U16 + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 494;
      break;
    case 493:  // $71E9
      /*$71E9*/ tmp3_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 494;
      break;
    case 494:  // $71EB
      /*$71EB*/ ram_poke(0x71cc, s_a);
      /*$71EE*/ tmp2_U8 = (uint8_t)(s_y - 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_y = tmp2_U8;
      /*$71EF*/ branchTarget = true; block_id = tmp2_U8 ? 495 : 496;
      break;
    case 495:  // $71EF
      /*$71EF*/ CYCLES_EDGE(0x71ef, 1);
                branchTarget = true; block_id = 488;
      break;
    case 496:  // $71F1
      /*$71F1*/ CYCLES(0x71f1, 8);
                s_status_d = 0x00;
      /*$71F2*/ branchTarget = true; block_id = find_block_id_func_t001(0x71f2, pop16() + 1);;
      break;
    case 497:  // $71F3
      /*$71F3*/ CYCLES(0x71f3, 15);
                tmp2_U8 = s_a;
                push8(tmp2_U8);
      /*$71F7*/ tmp2_U8 = tmp2_U8 >> 0x04;
                s_a = tmp2_U8;
      /*$71FA*/ branchTarget = true; block_id = !(tmp2_U8 != 0x00) ? 498 : 499;
      break;
    case 498:  // $71FA
      /*$71FA*/ CYCLES_EDGE(0x71fa, 1);
                branchTarget = true; block_id = 500;
      break;
    case 499:  // $71FC
      /*$71FC*/ CYCLES(0x71fc, 3);
                ram_poke(0x002c, s_a);
                block_id = 500;
      break;
    case 500:  // $71FE
      /*$71FE*/ CYCLES(0x71fe, 8);
                push8(s_a);
      /*$7201*/ branchTarget = true; block_id = ram_peek(0x002c) ? 501 : 502;
      break;
    case 501:  // $7201
      /*$7201*/ CYCLES_EDGE(0x7201, 1);
      /*$7207*/ CYCLES(0x7207, 14);
                tmp2_U8 = pop8();
                s_a = tmp2_U8;
      /*$7208*/ s_status_c = 0x00;
      /*$7209*/ block_id = s_status_d ? 504 : 503;
      break;
    case 502:  // $7203
      /*$7203*/ CYCLES(0x7203, 7);
                tmp2_U8 = pop8();
      /*$7204*/ branchTarget = true; block_id = 506;
      break;
    case 503:  // $7209
      /*$7209*/ tmp3_U16 = s_a;
                tmp6_U16 = (tmp3_U16 + 0x00b0) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)0x00b0);
                tmp2_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 505;
      break;
    case 504:  // $7209
      /*$7209*/ tmp6_U16 = adc_dec16(s_a, 0xb0, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp2_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 505;
      break;
    case 505:  // $720B
      /*$720B*/ branchTarget = true; push16(0x720d); block_id = 2623;
      break;
    case 506:  // $720E
      /*$720E*/ CYCLES(0x720e, 10);
                tmp2_U8 = pop8();
      /*$720F*/ tmp2_U8 = tmp2_U8 & 0x0f;
                s_a = tmp2_U8;
      /*$7211*/ s_status_c = (tmp2_U8 >= 0x00);
      /*$7213*/ branchTarget = true; block_id = !(tmp2_U8 != 0x00) ? 507 : 508;
      break;
    case 507:  // $7213
      /*$7213*/ CYCLES_EDGE(0x7213, 1);
                branchTarget = true; block_id = 509;
      break;
    case 508:  // $7215
      /*$7215*/ CYCLES(0x7215, 3);
                ram_poke(0x002c, s_a);
                block_id = 509;
      break;
    case 509:  // $7217
      /*$7217*/ CYCLES(0x7217, 8);
                push8(s_a);
      /*$721A*/ branchTarget = true; block_id = ram_peek(0x002c) ? 510 : 511;
      break;
    case 510:  // $721A
      /*$721A*/ CYCLES_EDGE(0x721a, 1);
      /*$721E*/ CYCLES(0x721e, 14);
                tmp2_U8 = pop8();
                s_a = tmp2_U8;
      /*$721F*/ s_status_c = 0x00;
      /*$7220*/ block_id = s_status_d ? 513 : 512;
      break;
    case 511:  // $721C
      /*$721C*/ CYCLES(0x721c, 10);
                tmp2_U8 = pop8();
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$721D*/ branchTarget = true; block_id = find_block_id_func_t001(0x721d, pop16() + 1);;
      break;
    case 512:  // $7220
      /*$7220*/ tmp6_U16 = s_a;
                tmp3_U16 = (tmp6_U16 + 0x00b0) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)0x00b0);
                tmp2_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 514;
      break;
    case 513:  // $7220
      /*$7220*/ tmp3_U16 = adc_dec16(s_a, 0xb0, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 514;
      break;
    case 514:  // $7222
      /*$7222*/ branchTarget = true; push16(0x7224); block_id = 2623;
      break;
    case 515:  // $7225
      /*$7225*/ CYCLES(0x7225, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x7225, pop16() + 1);;
      break;
    case 516:  // $7226
      /*$7226*/ CYCLES(0x7226, 5);
                tmp2_U8 = ram_peek(0x002c);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$7228*/ branchTarget = true; block_id = tmp2_U8 ? 517 : 518;
      break;
    case 517:  // $7228
      /*$7228*/ CYCLES_EDGE(0x7228, 1);
      /*$722F*/ CYCLES(0x722f, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x722f, pop16() + 1);;
      break;
    case 518:  // $722A
      /*$722A*/ CYCLES(0x722a, 5);
                s_status_not_z = 0xb0;
                s_status_n = 0x80;
                s_a = 0xb0;
      /*$722C*/ branchTarget = true; block_id = 2623;
      break;
    case 519:  // $7230
      /*$7230*/ CYCLES(0x7230, 20);
                tmp2_U8 = pop8();
      /*$7231*/ ram_poke(0x000c, tmp2_U8);
      /*$7233*/ tmp2_U8 = pop8();
      /*$7234*/ ram_poke(0x000d, tmp2_U8);
      /*$7236*/ branchTarget = true; push16(0x7238); block_id = 2541;
      break;
    case 520:  // $7239
      /*$7239*/ CYCLES(0x7239, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x000c) + 0x01);
                ram_poke(0x000c, tmp2_U8);
      /*$723B*/ branchTarget = true; block_id = tmp2_U8 ? 521 : 522;
      break;
    case 521:  // $723B
      /*$723B*/ CYCLES_EDGE(0x723b, 1);
                branchTarget = true; block_id = 523;
      break;
    case 522:  // $723D
      /*$723D*/ CYCLES(0x723d, 5);
                ram_poke(0x000d, (uint8_t)(ram_peek(0x000d) + 0x01));
                block_id = 523;
      break;
    case 523:  // $723F
      /*$723F*/ CYCLES(0x723f, 9);
                s_y = 0x00;
      /*$7241*/ tmp2_U8 = peek(ram_peek16al(0x000c));
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$7243*/ branchTarget = true; block_id = !tmp2_U8 ? 524 : 525;
      break;
    case 524:  // $7243
      /*$7243*/ CYCLES_EDGE(0x7243, 1);
      /*$724B*/ CYCLES(0x724b, 18);
      /*$724D*/ push8(ram_peek(0x000d));
      /*$724E*/ tmp2_U8 = ram_peek(0x000c);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$7250*/ push8(tmp2_U8);
      /*$7251*/ branchTarget = true; block_id = find_block_id_func_t001(0x7251, pop16() + 1);;
      break;
    case 525:  // $7245
      /*$7245*/ CYCLES(0x7245, 6);
                branchTarget = true; push16(0x7247); block_id = 2623;
      break;
    case 526:  // $7248
      /*$7248*/ CYCLES(0x7248, 3);
                branchTarget = true; block_id = 520;
      break;
    case 527:  // $7267
      /*$7267*/ CYCLES(0x7267, 56);
                s_status_d = 0x01;
      /*$726C*/ tmp3_U16 = adc_dec16(ram_peek(0x71cb), ram_peek(0x7252), 0x00);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 529;
      break;
    case 528:  // $726C
      /*$726C*/ tmp3_U16 = (s_a + ram_peek(0x7252)) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 529;
      break;
    case 529:  // $726F
      /*$726F*/ ram_poke(0x7252, s_a);
      /*$7272*/ s_a = ram_peek(0x71cc);
      /*$7275*/ block_id = s_status_d ? 531 : 530;
      break;
    case 530:  // $7275
      /*$7275*/ tmp3_U16 = (s_a + ram_peek(0x7253)) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 532;
      break;
    case 531:  // $7275
      /*$7275*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x7253), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 532;
      break;
    case 532:  // $7278
      /*$7278*/ ram_poke(0x7253, s_a);
      /*$727B*/ s_a = ram_peek(0x7254);
      /*$727E*/ block_id = s_status_d ? 534 : 533;
      break;
    case 533:  // $727E
      /*$727E*/ tmp3_U16 = s_a + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 535;
      break;
    case 534:  // $727E
      /*$727E*/ tmp3_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 535;
      break;
    case 535:  // $7280
      /*$7280*/ ram_poke(0x7254, s_a);
      /*$7283*/ s_a = ram_peek(0x7255);
      /*$7286*/ block_id = s_status_d ? 537 : 536;
      break;
    case 536:  // $7286
      /*$7286*/ tmp3_U16 = s_a;
                tmp6_U16 = tmp3_U16 + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)0x0000);
                tmp2_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 538;
      break;
    case 537:  // $7286
      /*$7286*/ tmp6_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp2_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 538;
      break;
    case 538:  // $7288
      /*$7288*/ ram_poke(0x7255, s_a);
      /*$728B*/ s_status_d = 0x00;
      /*$728C*/ branchTarget = true; block_id = find_block_id_func_t001(0x728c, pop16() + 1);;
      break;
    case 539:  // $728D
      /*$728D*/ CYCLES(0x728d, 10);
                tmp2_U8 = ram_peek(0x7255);
                s_a = tmp2_U8;
      /*$7290*/ tmp1_U8 = ram_peek(0x7259);
                s_status_not_z = (tmp2_U8 != tmp1_U8);
                tmp5_U8 = tmp2_U8 >= tmp1_U8;
                s_status_c = tmp5_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
      /*$7293*/ branchTarget = true; block_id = !tmp5_U8 ? 540 : 541;
      break;
    case 540:  // $7293
      /*$7293*/ CYCLES_EDGE(0x7293, 1);
                branchTarget = true; block_id = 556;
      break;
    case 541:  // $7295
      /*$7295*/ CYCLES(0x7295, 2);
                branchTarget = true; block_id = s_status_not_z ? 542 : 543;
      break;
    case 542:  // $7295
      /*$7295*/ CYCLES_EDGE(0x7295, 1);
                branchTarget = true; block_id = 555;
      break;
    case 543:  // $7297
      /*$7297*/ CYCLES(0x7297, 10);
                tmp5_U8 = ram_peek(0x7254);
                s_a = tmp5_U8;
      /*$729A*/ tmp1_U8 = ram_peek(0x7258);
                s_status_not_z = (tmp5_U8 != tmp1_U8);
                tmp2_U8 = tmp5_U8 >= tmp1_U8;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp5_U8 - tmp1_U8) & 0x80);
      /*$729D*/ branchTarget = true; block_id = !tmp2_U8 ? 544 : 545;
      break;
    case 544:  // $729D
      /*$729D*/ CYCLES_EDGE(0x729d, 1);
                branchTarget = true; block_id = 556;
      break;
    case 545:  // $729F
      /*$729F*/ CYCLES(0x729f, 2);
                branchTarget = true; block_id = s_status_not_z ? 546 : 547;
      break;
    case 546:  // $729F
      /*$729F*/ CYCLES_EDGE(0x729f, 1);
                branchTarget = true; block_id = 555;
      break;
    case 547:  // $72A1
      /*$72A1*/ CYCLES(0x72a1, 10);
                tmp2_U8 = ram_peek(0x7253);
                s_a = tmp2_U8;
      /*$72A4*/ tmp1_U8 = ram_peek(0x7257);
                s_status_not_z = (tmp2_U8 != tmp1_U8);
                tmp5_U8 = tmp2_U8 >= tmp1_U8;
                s_status_c = tmp5_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
      /*$72A7*/ branchTarget = true; block_id = !tmp5_U8 ? 548 : 549;
      break;
    case 548:  // $72A7
      /*$72A7*/ CYCLES_EDGE(0x72a7, 1);
                branchTarget = true; block_id = 556;
      break;
    case 549:  // $72A9
      /*$72A9*/ CYCLES(0x72a9, 2);
                branchTarget = true; block_id = s_status_not_z ? 550 : 551;
      break;
    case 550:  // $72A9
      /*$72A9*/ CYCLES_EDGE(0x72a9, 1);
                branchTarget = true; block_id = 555;
      break;
    case 551:  // $72AB
      /*$72AB*/ CYCLES(0x72ab, 10);
                tmp5_U8 = ram_peek(0x7252);
                s_a = tmp5_U8;
      /*$72AE*/ tmp1_U8 = ram_peek(0x7256);
                s_status_not_z = (tmp5_U8 != tmp1_U8);
                tmp2_U8 = tmp5_U8 >= tmp1_U8;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp5_U8 - tmp1_U8) & 0x80);
      /*$72B1*/ branchTarget = true; block_id = !tmp2_U8 ? 552 : 553;
      break;
    case 552:  // $72B1
      /*$72B1*/ CYCLES_EDGE(0x72b1, 1);
                branchTarget = true; block_id = 556;
      break;
    case 553:  // $72B3
      /*$72B3*/ CYCLES(0x72b3, 2);
                branchTarget = true; block_id = s_status_not_z ? 554 : 555;
      break;
    case 554:  // $72B3
      /*$72B3*/ CYCLES_EDGE(0x72b3, 1);
                branchTarget = true; block_id = 555;
      break;
    case 555:  // $72B5
      /*$72B5*/ CYCLES(0x72b5, 32);
      /*$72B8*/ ram_poke(0x7256, ram_peek(0x7252));
      /*$72BE*/ ram_poke(0x7257, ram_peek(0x7253));
      /*$72C4*/ ram_poke(0x7258, ram_peek(0x7254));
      /*$72C7*/ tmp5_U8 = ram_peek(0x7255);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$72CA*/ ram_poke(0x7259, tmp5_U8);
                block_id = 556;
      break;
    case 556:  // $72CD
      /*$72CD*/ CYCLES(0x72cd, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x72cd, pop16() + 1);;
      break;
    case 557:  // $72CE
      /*$72CE*/ CYCLES(0x72ce, 16);
      /*$72D0*/ ram_poke(0x0025, 0x14);
      /*$72D4*/ ram_poke(0x0024, 0x00);
      /*$72D6*/ branchTarget = true; push16(0x72d8); block_id = 519;
      break;
    case 558:  // $72D9
      /*$72D9*/ CYCLES(0x72d9, 0);
                fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x72d9, 0x72d9, 0x03); error_handler(0x72d9);
      break;
    case 559:  // $72E2
      /*$72E2*/ CYCLES(0x72e2, 15);
      /*$72E4*/ ram_poke(0x002c, 0x00);
      /*$72E6*/ s_a = ram_peek(0x7255);
      /*$72E9*/ branchTarget = true; push16(0x72eb); block_id = 497;
      break;
    case 560:  // $72EC
      /*$72EC*/ CYCLES(0x72ec, 10);
                s_a = ram_peek(0x7254);
      /*$72EF*/ branchTarget = true; push16(0x72f1); block_id = 497;
      break;
    case 561:  // $72F2
      /*$72F2*/ CYCLES(0x72f2, 10);
                s_a = ram_peek(0x7253);
      /*$72F5*/ branchTarget = true; push16(0x72f7); block_id = 497;
      break;
    case 562:  // $72F8
      /*$72F8*/ CYCLES(0x72f8, 10);
                s_a = ram_peek(0x7252);
      /*$72FB*/ branchTarget = true; push16(0x72fd); block_id = 497;
      break;
    case 563:  // $72FE
      /*$72FE*/ CYCLES(0x72fe, 6);
                branchTarget = true; push16(0x7300); block_id = 516;
      break;
    case 564:  // $7301
      /*$7301*/ CYCLES(0x7301, 11);
      /*$7303*/ ram_poke(0x0024, 0x14);
      /*$7305*/ branchTarget = true; push16(0x7307); block_id = 519;
      break;
    case 565:  // $7308
      /*$7308*/ CYCLES(0x7308, 4);
      /*$730B*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x730b, 0x730b, 0x03); error_handler(0x730b);
      break;
    case 566:  // $7314
      /*$7314*/ CYCLES(0x7314, 15);
      /*$7316*/ ram_poke(0x002c, 0x00);
      /*$7318*/ s_a = ram_peek(0x7259);
      /*$731B*/ branchTarget = true; push16(0x731d); block_id = 497;
      break;
    case 567:  // $731E
      /*$731E*/ CYCLES(0x731e, 10);
                s_a = ram_peek(0x7258);
      /*$7321*/ branchTarget = true; push16(0x7323); block_id = 497;
      break;
    case 568:  // $7324
      /*$7324*/ CYCLES(0x7324, 10);
                s_a = ram_peek(0x7257);
      /*$7327*/ branchTarget = true; push16(0x7329); block_id = 497;
      break;
    case 569:  // $732A
      /*$732A*/ CYCLES(0x732a, 10);
                s_a = ram_peek(0x7256);
      /*$732D*/ branchTarget = true; push16(0x732f); block_id = 497;
      break;
    case 570:  // $7330
      /*$7330*/ CYCLES(0x7330, 6);
                branchTarget = true; push16(0x7332); block_id = 516;
      break;
    case 571:  // $7333
      /*$7333*/ CYCLES(0x7333, 16);
      /*$7335*/ ram_poke(0x0024, 0x00);
      /*$7337*/ s_a = 0x15;
      /*$7339*/ ram_poke(0x0025, 0x15);
      /*$733B*/ branchTarget = true; push16(0x733d); block_id = 519;
      break;
    case 572:  // $733E
      /*$733E*/ CYCLES(0x733e, 8);
                tmp2_U8 = s_x;
                tmp2_U8 = peek((ram_peek((uint8_t)(0xd0 + tmp2_U8)) + (ram_peek((uint8_t)(0xd1 + tmp2_U8)) << 8)));
      /*$7340*/ branchTarget = true; block_id = (s_a != tmp2_U8) ? 573 : 574;
      break;
    case 573:  // $7340
      /*$7340*/ CYCLES_EDGE(0x7340, 1);
      /*$730E*/ CYCLES(0x730e, 0);
                fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x730e, 0x730e, 0x03); error_handler(0x730e);
      break;
    case 574:  // $7342
      /*$7342*/ CYCLES(0x7342, 8);
      /*$7348*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x7348, 0x7348, 0x03); error_handler(0x7348);
      break;
    case 575:  // $734D
      /*$734D*/ CYCLES(0x734d, 15);
      /*$734F*/ ram_poke(0x002c, 0x00);
      /*$7351*/ s_a = ram_peek(0x725b);
      /*$7354*/ branchTarget = true; push16(0x7356); block_id = 497;
      break;
    case 576:  // $7357
      /*$7357*/ CYCLES(0x7357, 10);
                s_a = ram_peek(0x725a);
      /*$735A*/ branchTarget = true; push16(0x735c); block_id = 497;
      break;
    case 577:  // $735D
      /*$735D*/ CYCLES(0x735d, 6);
                branchTarget = true; push16(0x735f); block_id = 516;
      break;
    case 578:  // $7360
      /*$7360*/ CYCLES(0x7360, 8);
                s_status_not_z = 0xa0;
                s_status_n = 0x80;
                s_a = 0xa0;
      /*$7362*/ branchTarget = true; push16(0x7364); block_id = 2623;
      break;
    case 579:  // $7365
      /*$7365*/ CYCLES(0x7365, 11);
                s_a = 0x14;
      /*$7367*/ ram_poke(0x0024, 0x14);
      /*$7369*/ branchTarget = true; push16(0x736b); block_id = 519;
      break;
    case 580:  // $736C
      /*$736C*/ CYCLES(0x736c, 21);
                tmp6_U16 = (uint8_t)(0xc1 + s_x);
                ram_poke(tmp6_U16, (uint8_t)(ram_peek(tmp6_U16) - 0x01));
      /*$736E*/ tmp2_U8 = io_peek(0xc5d5);
                s_status_c = (s_y >= tmp2_U8);
      /*$7371*/ s_x = s_sp;
      /*$7372*/ s_status_not_z = 0xa0;
                s_status_n = 0x80;
                s_y = 0xa0;
      /*$7374*/ branchTarget = true; push16(0x7375); block_id = 2644;
      break;
    case 581:  // $7375
      /*$7375*/ CYCLES(0x7375, 15);
      /*$7377*/ ram_poke(0x002c, 0x00);
      /*$7379*/ s_a = ram_peek(0x71cc);
      /*$737C*/ branchTarget = true; push16(0x737e); block_id = 497;
      break;
    case 582:  // $7376
      /*$7376*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x7374, 0x7376, 0x02); error_handler(0x7374);
      break;
    case 583:  // $7376
      /*$7376*/ CYCLES(0x7376, 7);
                branchTarget = true; push16(0x7377); block_id = 2644;
      break;
    case 584:  // $7378
      /*$7378*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x7376, 0x7378, 0x02); error_handler(0x7376);
      break;
    case 585:  // $7378
      /*$7378*/ CYCLES(0x7378, 9);
                tmp1_U8 = io_peek(0xccad);
      /*$737B*/ block_id = s_status_d ? 587 : 586;
      break;
    case 586:  // $737B
      /*$737B*/ tmp1_U8 = peek((ram_peek16al(0x0020) + s_y));
                block_id = 588;
      break;
    case 587:  // $737B
      /*$737B*/ tmp1_U8 = peek((ram_peek16al(0x0020) + s_y));
                block_id = 588;
      break;
    case 588:  // $737D
      /*$737D*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x737d, 0x737d, 0x03); error_handler(0x737d);
      break;
    case 589:  // $737F
      /*$737F*/ CYCLES(0x737f, 10);
                s_a = ram_peek(0x71cb);
      /*$7382*/ branchTarget = true; push16(0x7384); block_id = 497;
      break;
    case 590:  // $7385
      /*$7385*/ CYCLES(0x7385, 6);
                branchTarget = true; push16(0x7387); block_id = 516;
      break;
    case 591:  // $7388
      /*$7388*/ CYCLES(0x7388, 16);
      /*$738A*/ ram_poke(0x0025, 0x16);
      /*$738E*/ ram_poke(0x0024, 0x00);
      /*$7390*/ branchTarget = true; push16(0x7392); block_id = 519;
      break;
    case 592:  // $7393
      /*$7393*/ CYCLES(0x7393, 0);
                fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x7393, 0x7393, 0x03); error_handler(0x7393);
      break;
    case 593:  // $73A2
      /*$73A2*/ CYCLES(0x73a2, 11);
                s_a = 0x00;
      /*$73A4*/ ram_poke(0x002c, 0x00);
      /*$73A6*/ branchTarget = true; push16(0x73a8); block_id = 497;
      break;
    case 594:  // $73A9
      /*$73A9*/ CYCLES(0x73a9, 10);
                s_a = ram_peek(0x725e);
      /*$73AC*/ branchTarget = true; push16(0x73ae); block_id = 497;
      break;
    case 595:  // $73AF
      /*$73AF*/ CYCLES(0x73af, 6);
                branchTarget = true; push16(0x73b1); block_id = 516;
      break;
    case 596:  // $73B2
      /*$73B2*/ CYCLES(0x73b2, 11);
                s_a = 0x14;
      /*$73B4*/ ram_poke(0x0024, 0x14);
      /*$73B6*/ branchTarget = true; push16(0x73b8); block_id = 519;
      break;
    case 597:  // $73B9
      /*$73B9*/ CYCLES(0x73b9, 18);
      /*$73BC*/ s_status_c = (s_a >= ram_peek(0x00cc));
      /*$73BE*/ s_x = s_sp;
      /*$73BF*/ s_status_not_z = 0xa0;
                s_status_n = 0x80;
                s_y = 0xa0;
      /*$73C1*/ branchTarget = true; push16(0x73c2); block_id = 2644;
      break;
    case 598:  // $73C2
      /*$73C2*/ CYCLES(0x73c2, 15);
      /*$73C4*/ ram_poke(0x002c, 0x00);
      /*$73C6*/ s_a = ram_peek(0x7265);
      /*$73C9*/ push16(0x73cb); block_id = 497;
      break;
    case 599:  // $73C3
      /*$73C3*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x73c1, 0x73c3, 0x02); error_handler(0x73c1);
      break;
    case 600:  // $73C3
      /*$73C3*/ CYCLES(0x73c3, 7);
                branchTarget = true; push16(0x73c4); block_id = 2644;
      break;
    case 601:  // $73C5
      /*$73C5*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x73c3, 0x73c5, 0x02); error_handler(0x73c3);
      break;
    case 602:  // $73C5
      /*$73C5*/ CYCLES(0x73c5, 4);
      /*$73C8*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x73c8, 0x73c8, 0x03); error_handler(0x73c8);
      break;
    case 603:  // $73CC
      /*$73CC*/ CYCLES(0x73cc, 6);
                branchTarget = true; push16(0x73ce); block_id = 516;
      break;
    case 604:  // $73CF
      /*$73CF*/ CYCLES(0x73cf, 11);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$73D1*/ ram_poke(0x0025, 0x00);
      /*$73D3*/ branchTarget = true; push16(0x73d5); block_id = 2541;
      break;
    case 605:  // $73D6
      /*$73D6*/ CYCLES(0x73d6, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x73d6, pop16() + 1);;
      break;
    case 606:  // $73DB
      /*$73DB*/ CYCLES_EDGE(0x73db, 1);
      /*$73E9*/ CYCLES(0x73e9, 16);
      /*$73EB*/ ram_poke(0x0025, 0x17);
      /*$73EF*/ ram_poke(0x0024, 0x00);
      /*$73F1*/ branchTarget = true; push16(0x73f3); block_id = 519;
      break;
    case 607:  // $73DD
      /*$73DD*/ CYCLES(0x73dd, 20);
                s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_a = 0x01;
      /*$73DF*/ ram_poke(0x0302, 0x01);
      /*$73E2*/ ram_poke(0x0301, 0x01);
      /*$73E5*/ ram_poke(0x73d7, 0x01);
      /*$73E8*/ branchTarget = true; block_id = find_block_id_func_t001(0x73e8, pop16() + 1);;
      break;
    case 608:  // $73F4
      /*$73F4*/ CYCLES(0x73f4, 12);
      /*$73FF*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x73ff, 0x73ff, 0x03); error_handler(0x73ff);
      break;
    case 609:  // $7414
      /*$7414*/ CYCLES(0x7414, 10);
      /*$7416*/ ram_poke(0x0002, 0xe8);
      /*$741A*/ ram_poke(0x0003, 0x00);
                block_id = 610;
      break;
    case 610:  // $741C
      /*$741C*/ CYCLES(0x741c, 4);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$741D*/ branchTarget = true; block_id = tmp1_U8 ? 611 : 612;
      break;
    case 611:  // $741D
      /*$741D*/ CYCLES_EDGE(0x741d, 1);
                branchTarget = true; block_id = 610;
      break;
    case 612:  // $741F
      /*$741F*/ CYCLES(0x741f, 6);
                tmp1_U8 = io_peek(0xc000);
                s_a = tmp1_U8;
      /*$7422*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 613 : 614;
      break;
    case 613:  // $7422
      /*$7422*/ CYCLES_EDGE(0x7422, 1);
                branchTarget = true; block_id = 628;
      break;
    case 614:  // $7424
      /*$7424*/ CYCLES(0x7424, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x0003) + 0x01);
                ram_poke(0x0003, tmp1_U8);
      /*$7426*/ branchTarget = true; block_id = tmp1_U8 ? 615 : 616;
      break;
    case 615:  // $7426
      /*$7426*/ CYCLES_EDGE(0x7426, 1);
                branchTarget = true; block_id = 610;
      break;
    case 616:  // $7428
      /*$7428*/ CYCLES(0x7428, 6);
      /*$742B*/ branchTarget = true; block_id = !ram_peek(0x6c71) ? 617 : 618;
      break;
    case 617:  // $742B
      /*$742B*/ CYCLES_EDGE(0x742b, 1);
                branchTarget = true; block_id = 625;
      break;
    case 618:  // $742D
      /*$742D*/ CYCLES(0x742d, 10);
                tmp1_U8 = io_peek(0xc05b);
      /*$7430*/ tmp1_U8 = io_peek(0xc062);
      /*$7433*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 619 : 620;
      break;
    case 619:  // $7433
      /*$7433*/ CYCLES_EDGE(0x7433, 1);
      /*$743A*/ CYCLES(0x743a, 10);
                tmp1_U8 = io_peek(0xc05a);
      /*$743D*/ tmp1_U8 = io_peek(0xc062);
      /*$7440*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 621 : 622;
      break;
    case 620:  // $7435
      /*$7435*/ CYCLES(0x7435, 5);
                s_a = 0xb1;
      /*$7437*/ branchTarget = true; block_id = 628;
      break;
    case 621:  // $7440
      /*$7440*/ CYCLES_EDGE(0x7440, 1);
      /*$7447*/ CYCLES(0x7447, 6);
                tmp1_U8 = io_peek(0xc063);
      /*$744A*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 623 : 624;
      break;
    case 622:  // $7442
      /*$7442*/ CYCLES(0x7442, 5);
                s_a = 0xb0;
      /*$7444*/ branchTarget = true; block_id = 628;
      break;
    case 623:  // $744A
      /*$744A*/ CYCLES_EDGE(0x744a, 1);
                branchTarget = true; block_id = 625;
      break;
    case 624:  // $744C
      /*$744C*/ CYCLES(0x744c, 5);
                s_a = 0xb2;
      /*$744E*/ branchTarget = true; block_id = 628;
      break;
    case 625:  // $7451
      /*$7451*/ CYCLES(0x7451, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x0002) + 0x01);
                ram_poke(0x0002, tmp1_U8);
      /*$7453*/ branchTarget = true; block_id = tmp1_U8 ? 626 : 627;
      break;
    case 626:  // $7453
      /*$7453*/ CYCLES_EDGE(0x7453, 1);
                branchTarget = true; block_id = 610;
      break;
    case 627:  // $7455
      /*$7455*/ CYCLES(0x7455, 20);
                s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_a = 0x01;
      /*$7457*/ ram_poke(0x0302, 0x01);
      /*$745A*/ ram_poke(0x0301, 0x01);
      /*$745D*/ io_poke(0xc010, 0x01);
      /*$7460*/ branchTarget = true; block_id = find_block_id_func_t001(0x7460, pop16() + 1);;
      break;
    case 628:  // $7461
      /*$7461*/ CYCLES(0x7461, 8);
                tmp1_U8 = s_a;
                io_poke(0xc010, tmp1_U8);
      /*$7466*/ branchTarget = true; block_id = !(tmp1_U8 != 0xc3) ? 629 : 630;
      break;
    case 629:  // $7466
      /*$7466*/ CYCLES_EDGE(0x7466, 1);
      /*$747F*/ CYCLES(0x747f, 6);
                func_702b(0x7481);
      /*$7482*/ CYCLES(0x7482, 10);
                tmp1_U8 = io_peek(0xc052);
      /*$7485*/ func_6641(0x7487);
      /*$7488*/ CYCLES(0x7488, 11);
                s_a = 0x01;
      /*$748A*/ ram_poke(0x0025, 0x01);
      /*$748C*/ branchTarget = true; push16(0x748e); block_id = 519;
      break;
    case 630:  // $7468
      /*$7468*/ CYCLES(0x7468, 4);
                tmp1_U8 = s_a >= 0xb0;
                s_status_c = tmp1_U8;
      /*$746A*/ branchTarget = true; block_id = !tmp1_U8 ? 631 : 632;
      break;
    case 631:  // $746A
      /*$746A*/ CYCLES_EDGE(0x746a, 1);
                branchTarget = true; block_id = 610;
      break;
    case 632:  // $746C
      /*$746C*/ CYCLES(0x746c, 4);
                tmp1_U8 = s_a >= 0xb3;
                s_status_c = tmp1_U8;
      /*$746E*/ branchTarget = true; block_id = tmp1_U8 ? 633 : 634;
      break;
    case 633:  // $746E
      /*$746E*/ CYCLES_EDGE(0x746e, 1);
                branchTarget = true; block_id = 610;
      break;
    case 634:  // $7470
      /*$7470*/ CYCLES(0x7470, 24);
                s_status_c = 0x01;
      /*$7471*/ block_id = s_status_d ? 636 : 635;
      break;
    case 635:  // $7471
      /*$7471*/ tmp6_U16 = s_a;
                tmp3_U16 = (tmp6_U16 - 0x00b0) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)0xff4f);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 637;
      break;
    case 636:  // $7471
      /*$7471*/ tmp3_U16 = sbc_dec16(s_a, 0xb0, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 637;
      break;
    case 637:  // $7473
      /*$7473*/ ram_poke(0x0301, s_a);
      /*$7476*/ s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$7478*/ ram_poke(0x0302, 0x00);
      /*$747B*/ io_poke(0xc010, 0x00);
      /*$747E*/ branchTarget = true; block_id = find_block_id_func_t001(0x747e, pop16() + 1);;
      break;
    case 638:  // $748F
      /*$748F*/ CYCLES(0x748f, 14);
                ram_poke(0xa0a0, s_a);
      /*$7496*/ tmp1_U8 = s_x;
                tmp1_U8 = peek((ram_peek((uint8_t)(0xc2 + tmp1_U8)) + (ram_peek((uint8_t)(0xc3 + tmp1_U8)) << 8)));
      /*$7498*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x7498, 0x7498, 0x03); error_handler(0x7498);
      break;
    case 639:  // $7541
      /*$7541*/ CYCLES(0x7541, 2);
                s_x = 0x00;
                block_id = 640;
      break;
    case 640:  // $7543
      /*$7543*/ CYCLES(0x7543, 10);
                s_a = ram_peek((0x6c63 + s_x));
      /*$7546*/ branchTarget = true; push16(0x7548); block_id = 649;
      break;
    case 641:  // $7549
      /*$7549*/ CYCLES(0x7549, 6);
                tmp1_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp1_U8;
      /*$754C*/ branchTarget = true; block_id = (tmp1_U8 != 0x06) ? 642 : 643;
      break;
    case 642:  // $754C
      /*$754C*/ CYCLES_EDGE(0x754c, 1);
                branchTarget = true; block_id = 640;
      break;
    case 643:  // $754E
      /*$754E*/ CYCLES(0x754e, 26);
      /*$7550*/ ram_poke(0x0001, 0x0c);
      /*$7554*/ ram_poke(0x0000, 0x02);
      /*$7558*/ ram_poke(0x0003, 0x12);
      /*$755C*/ ram_poke(0x0002, 0x1e);
      /*$755E*/ func_60e4(0x7560);
      /*$7561*/ CYCLES(0x7561, 21);
      /*$7563*/ ram_poke(0x0003, 0x13);
      /*$7567*/ ram_poke(0x0008, 0x1d);
      /*$756B*/ ram_poke(0x0000, 0x0a);
      /*$756D*/ func_615a(0x756f);
      /*$7570*/ CYCLES(0x7570, 11);
      /*$7572*/ ram_poke(0x0000, 0x0e);
      /*$7574*/ func_60e4(0x7576);
      /*$7577*/ CYCLES(0x7577, 2);
                s_x = 0x00;
                block_id = 644;
      break;
    case 644:  // $7579
      /*$7579*/ CYCLES(0x7579, 6);
                branchTarget = true; push16(0x757b); block_id = 658;
      break;
    case 645:  // $757C
      /*$757C*/ CYCLES(0x757c, 11);
                ram_poke((0x6c63 + s_x), s_a);
      /*$757F*/ branchTarget = true; push16(0x7581); block_id = 649;
      break;
    case 646:  // $7582
      /*$7582*/ CYCLES(0x7582, 6);
                tmp1_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp1_U8;
      /*$7583*/ s_status_c = (tmp1_U8 >= 0x06);
      /*$7585*/ branchTarget = true; block_id = (tmp1_U8 != 0x06) ? 647 : 648;
      break;
    case 647:  // $7585
      /*$7585*/ CYCLES_EDGE(0x7585, 1);
                branchTarget = true; block_id = 644;
      break;
    case 648:  // $7587
      /*$7587*/ CYCLES(0x7587, 16);
      /*$7589*/ ram_poke(0x0036, 0xf0);
      /*$758B*/ s_status_not_z = 0xfd;
                s_status_n = 0x80;
                s_a = 0xfd;
      /*$758D*/ ram_poke(0x0037, 0xfd);
      /*$758F*/ branchTarget = true; block_id = find_block_id_func_t001(0x758f, pop16() + 1);;
      break;
    case 649:  // $7590
      /*$7590*/ CYCLES(0x7590, 7);
                ram_poke(0x0002, s_x);
      /*$7594*/ branchTarget = true; block_id = (s_a != 0x88) ? 650 : 651;
      break;
    case 650:  // $7594
      /*$7594*/ CYCLES_EDGE(0x7594, 1);
                branchTarget = true; block_id = 652;
      break;
    case 651:  // $7596
      /*$7596*/ CYCLES(0x7596, 2);
                s_a = 0xe6;
                block_id = 652;
      break;
    case 652:  // $7598
      /*$7598*/ CYCLES(0x7598, 4);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x95);
      /*$759A*/ branchTarget = true; block_id = (tmp1_U8 != 0x95) ? 653 : 654;
      break;
    case 653:  // $759A
      /*$759A*/ CYCLES_EDGE(0x759a, 1);
                branchTarget = true; block_id = 655;
      break;
    case 654:  // $759C
      /*$759C*/ CYCLES(0x759c, 2);
                s_a = 0xe7;
                block_id = 655;
      break;
    case 655:  // $759E
      /*$759E*/ CYCLES(0x759e, 23);
                push8(s_a);
      /*$759F*/ tmp1_U8 = s_x;
      /*$75A2*/ ram_poke(0x0024, ram_peek((0x75b3 + tmp1_U8)));
      /*$75A7*/ ram_poke(0x0025, ram_peek((0x75b9 + tmp1_U8)));
      /*$75A9*/ branchTarget = true; push16(0x75ab); block_id = 2541;
      break;
    case 656:  // $75AC
      /*$75AC*/ CYCLES(0x75ac, 10);
                tmp1_U8 = pop8();
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$75AD*/ branchTarget = true; push16(0x75af); block_id = 2623;
      break;
    case 657:  // $75B0
      /*$75B0*/ CYCLES(0x75b0, 9);
                tmp1_U8 = ram_peek(0x0002);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$75B2*/ branchTarget = true; block_id = find_block_id_func_t001(0x75b2, pop16() + 1);;
      break;
    case 658:  // $75D1
      /*$75D1*/ CYCLES(0x75d1, 23);
                tmp1_U8 = s_x;
                ram_poke(0x0002, tmp1_U8);
      /*$75D6*/ ram_poke(0x0024, ram_peek((0x75bf + tmp1_U8)));
      /*$75DB*/ ram_poke(0x0025, ram_peek((0x75c5 + tmp1_U8)));
      /*$75DD*/ branchTarget = true; push16(0x75df); block_id = 2541;
      break;
    case 659:  // $75E0
      /*$75E0*/ CYCLES(0x75e0, 11);
                s_x = ram_peek(0x0002);
      /*$75E2*/ s_status_not_z = 0xa0;
                s_status_n = 0x80;
                s_a = 0xa0;
      /*$75E4*/ branchTarget = true; push16(0x75e6); block_id = 2623;
      break;
    case 660:  // $75E7
      /*$75E7*/ CYCLES(0x75e7, 2);
                s_y = 0x00;
                block_id = 661;
      break;
    case 661:  // $75E9
      /*$75E9*/ CYCLES(0x75e9, 4);
                tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp1_U8;
      /*$75EA*/ branchTarget = true; block_id = tmp1_U8 ? 662 : 663;
      break;
    case 662:  // $75EA
      /*$75EA*/ CYCLES_EDGE(0x75ea, 1);
                branchTarget = true; block_id = 661;
      break;
    case 663:  // $75EC
      /*$75EC*/ CYCLES(0x75ec, 4);
      /*$75F0*/ CYCLES(0x75f0, 4);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp1_U8;
      /*$75F1*/ branchTarget = true; block_id = tmp1_U8 ? 665 : 666;
      break;
    case 664:  // $75EE
      /*$75EE*/ CYCLES_EDGE(0x75ee, 1);
                branchTarget = true; block_id = 669;
      break;
    case 665:  // $75F1
      /*$75F1*/ CYCLES_EDGE(0x75f1, 1);
                branchTarget = true; block_id = 661;
      break;
    case 666:  // $75F3
      /*$75F3*/ CYCLES(0x75f3, 23);
                tmp1_U8 = ram_peek(0x0002);
                s_x = tmp1_U8;
      /*$75F8*/ ram_poke(0x0024, ram_peek((0x75bf + tmp1_U8)));
      /*$75FD*/ ram_poke(0x0025, ram_peek((0x75c5 + tmp1_U8)));
      /*$75FF*/ branchTarget = true; push16(0x7601); block_id = 2541;
      break;
    case 667:  // $7602
      /*$7602*/ CYCLES(0x7602, 13);
                tmp1_U8 = ram_peek(0x0002);
                s_x = tmp1_U8;
      /*$7604*/ tmp1_U8 = ram_peek((0x75cb + tmp1_U8));
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$7607*/ branchTarget = true; push16(0x7609); block_id = 2623;
      break;
    case 668:  // $760A
      /*$760A*/ CYCLES(0x760a, 2);
                s_y = 0x00;
                block_id = 669;
      break;
    case 669:  // $760C
      /*$760C*/ CYCLES(0x760c, 4);
                tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp1_U8;
      /*$760D*/ branchTarget = true; block_id = tmp1_U8 ? 670 : 671;
      break;
    case 670:  // $760D
      /*$760D*/ CYCLES_EDGE(0x760d, 1);
                branchTarget = true; block_id = 669;
      break;
    case 671:  // $760F
      /*$760F*/ CYCLES(0x760f, 6);
                tmp1_U8 = io_peek(0xc000);
                s_a = tmp1_U8;
      /*$7612*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 672 : 673;
      break;
    case 672:  // $7612
      /*$7612*/ CYCLES_EDGE(0x7612, 1);
      /*$761C*/ CYCLES(0x761c, 8);
                tmp1_U8 = s_a;
                io_poke(0xc010, tmp1_U8);
      /*$761F*/ tmp1_U8 = tmp1_U8 >= 0xa1;
                s_status_c = tmp1_U8;
      /*$7621*/ branchTarget = true; block_id = tmp1_U8 ? 676 : 677;
      break;
    case 673:  // $7614
      /*$7614*/ CYCLES(0x7614, 4);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp1_U8;
      /*$7615*/ branchTarget = true; block_id = tmp1_U8 ? 674 : 675;
      break;
    case 674:  // $7615
      /*$7615*/ CYCLES_EDGE(0x7615, 1);
                branchTarget = true; block_id = 669;
      break;
    case 675:  // $7617
      /*$7617*/ CYCLES(0x7617, 6);
                s_x = ram_peek(0x0002);
      /*$7619*/ branchTarget = true; block_id = 658;
      break;
    case 676:  // $7621
      /*$7621*/ CYCLES_EDGE(0x7621, 1);
                branchTarget = true; block_id = 682;
      break;
    case 677:  // $7623
      /*$7623*/ CYCLES(0x7623, 4);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x88);
      /*$7625*/ branchTarget = true; block_id = !(tmp1_U8 != 0x88) ? 678 : 679;
      break;
    case 678:  // $7625
      /*$7625*/ CYCLES_EDGE(0x7625, 1);
                branchTarget = true; block_id = 682;
      break;
    case 679:  // $7627
      /*$7627*/ CYCLES(0x7627, 4);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x95);
      /*$7629*/ branchTarget = true; block_id = !(tmp1_U8 != 0x95) ? 680 : 681;
      break;
    case 680:  // $7629
      /*$7629*/ CYCLES_EDGE(0x7629, 1);
                branchTarget = true; block_id = 682;
      break;
    case 681:  // $762B
      /*$762B*/ CYCLES(0x762b, 6);
                s_x = ram_peek(0x0002);
      /*$762D*/ branchTarget = true; block_id = 658;
      break;
    case 682:  // $7630
      /*$7630*/ CYCLES(0x7630, 9);
                tmp1_U8 = ram_peek(0x0002);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$7632*/ branchTarget = true; block_id = find_block_id_func_t001(0x7632, pop16() + 1);;
      break;
    case 683:  // $7633
      /*$7633*/ CYCLES(0x7633, 22);
      /*$7638*/ tmp3_U16 = adc_dec16(ram_peek(0x725e), 0x01, 0x00);
                s_a = ((uint8_t)tmp3_U16);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 685;
      break;
    case 684:  // $7638
      /*$7638*/ tmp3_U16 = s_a;
                tmp6_U16 = (tmp3_U16 + 0x0001) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)0x0001);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 685;
      break;
    case 685:  // $763A
      /*$763A*/ ram_poke(0x725e, s_a);
      /*$763D*/ s_status_d = 0x00;
      /*$763E*/ func_64a9(0x7640);
      /*$7641*/ CYCLES(0x7641, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x7641, pop16() + 1);;
      break;
    case 686:  // $7645
      /*$7645*/ CYCLES(0x7645, 9);
                ram_poke(0x0002, s_a);
      /*$7647*/ func_6c4b(0x7649);
                branchTarget = true; block_id = 687;
      break;
    case 687:  // $764A
      /*$764A*/ CYCLES(0x764a, 15);
                ram_poke(0x0003, s_a);
      /*$764C*/ s_a = ram_peek(0x0003);
      /*$764E*/ s_y = ram_peek(0x0002);
      /*$7650*/ FUNC_SCRN(0x7652);
                branchTarget = true; block_id = 688;
      break;
    case 688:  // $7653
      /*$7653*/ CYCLES(0x7653, 2);
                branchTarget = true; block_id = s_status_not_z ? 689 : 690;
      break;
    case 689:  // $7653
      /*$7653*/ CYCLES_EDGE(0x7653, 1);
      /*$7642*/ CYCLES(0x7642, 6);
                func_6c4b(0x7644);
                branchTarget = true; block_id = 686;
      break;
    case 690:  // $7655
      /*$7655*/ CYCLES(0x7655, 8);
                s_a = 0x0f;
      /*$7657*/ FUNC_SETCOL(0x7659);
                branchTarget = true; block_id = 691;
      break;
    case 691:  // $765A
      /*$765A*/ CYCLES(0x765a, 12);
                s_a = ram_peek(0x0003);
      /*$765C*/ s_y = ram_peek(0x0002);
      /*$765E*/ FUNC_MON_PLOT(0x7660);
                branchTarget = true; block_id = 692;
      break;
    case 692:  // $7661
      /*$7661*/ CYCLES(0x7661, 16);
      /*$7663*/ ram_poke(0x0000, 0x01);
      /*$7667*/ ram_poke(0x0001, 0x09);
      /*$7669*/ func_60e4(0x766b);
                branchTarget = true; block_id = 693;
      break;
    case 693:  // $766C
      /*$766C*/ CYCLES(0x766c, 32);
                s_status_d = 0x01;
      /*$7671*/ tmp3_U16 = adc_dec16(ram_peek(0x725f), 0x01, 0x00);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 695;
      break;
    case 694:  // $7671
      /*$7671*/ tmp6_U16 = (s_a + 0x0001) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 695;
      break;
    case 695:  // $7673
      /*$7673*/ ram_poke(0x725f, s_a);
      /*$7676*/ s_a = ram_peek(0x7260);
      /*$7679*/ block_id = s_status_d ? 697 : 696;
      break;
    case 696:  // $7679
      /*$7679*/ tmp6_U16 = s_a;
                tmp3_U16 = tmp6_U16 + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)0x0000);
                tmp1_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 698;
      break;
    case 697:  // $7679
      /*$7679*/ tmp3_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
                block_id = 698;
      break;
    case 698:  // $767B
      /*$767B*/ ram_poke(0x7260, s_a);
      /*$767E*/ s_status_d = 0x00;
      /*$767F*/ branchTarget = true; block_id = find_block_id_func_t001(0x767f, pop16() + 1);;
      break;
    case 699:  // $7691
      /*$7691*/ CYCLES(0x7691, 6);
                branchTarget = true; push16(0x7693); block_id = 823;
      break;
    case 700:  // $7694
      /*$7694*/ CYCLES(0x7694, 6);
                func_728d(0x7696);
                branchTarget = true; block_id = 701;
      break;
    case 701:  // $7697
      /*$7697*/ CYCLES(0x7697, 40);
      /*$7699*/ ram_poke(0x0303, 0x01);
      /*$769C*/ ram_poke(0x7265, 0x01);
      /*$76A1*/ ram_poke(0x7252, 0x00);
      /*$76A4*/ ram_poke(0x7253, 0x00);
      /*$76A7*/ ram_poke(0x7254, 0x00);
      /*$76AA*/ ram_poke(0x7255, 0x00);
      /*$76AF*/ ram_poke(0x725e, 0x02);
      /*$76B4*/ ram_poke(0x725f, 0x00);
                block_id = 702;
      break;
    case 702:  // $76B7
      /*$76B7*/ CYCLES(0x76b7, 14);
      /*$76BA*/ ram_poke(0x78b2, ram_peek(0x725e));
      /*$76BF*/ ram_poke(0x7263, 0x10);
                block_id = 703;
      break;
    case 703:  // $76C2
      /*$76C2*/ CYCLES(0x76c2, 6);
      /*$76C4*/ ram_poke(0x7264, 0x00);
                block_id = 704;
      break;
    case 704:  // $76C7
      /*$76C7*/ CYCLES(0x76c7, 40);
      /*$76C9*/ ram_poke(0x725f, 0x00);
      /*$76CC*/ ram_poke(0x7260, 0x00);
      /*$76CF*/ ram_poke(0x7261, 0x00);
      /*$76D2*/ ram_poke(0x7262, 0x00);
      /*$76D8*/ ram_poke(0x725a, ram_peek(0x7263));
      /*$76DE*/ ram_poke(0x725b, ram_peek(0x7264));
      /*$76E1*/ func_7045(0x76e3);
                branchTarget = true; block_id = 705;
      break;
    case 705:  // $76E4
      /*$76E4*/ CYCLES(0x76e4, 14);
      /*$76E7*/ ram_poke(0x7266, ram_peek(0x0304));
      /*$76EA*/ func_71cd(0x76ec);
                branchTarget = true; block_id = 706;
      break;
    case 706:  // $76ED
      /*$76ED*/ CYCLES(0x76ed, 14);
                tmp2_U8 = io_peek(0xc054);
      /*$76F0*/ tmp2_U8 = io_peek(0xc053);
      /*$76F3*/ func_7642(0x76f5);
                branchTarget = true; block_id = 707;
      break;
    case 707:  // $76F6
      /*$76F6*/ CYCLES(0x76f6, 6);
                func_60e4(0x76f8);
                branchTarget = true; block_id = 708;
      break;
    case 708:  // $76F9
      /*$76F9*/ CYCLES(0x76f9, 8);
      /*$76FB*/ ram_poke(0x0300, 0x52);
      /*$76FE*/ s_a = 0x00;
      /*$7700*/ CYCLES(0x7700, 23);
                ram_poke(0x0305, 0x00);
      /*$7706*/ ram_poke(0x6255, ram_peek(0x7266));
      /*$770B*/ ram_poke(0x0022, 0x14);
      /*$770D*/ FUNC_HOME(0x770f);
                branchTarget = true; block_id = 709;
      break;
    case 709:  // $7710
      /*$7710*/ CYCLES(0x7710, 6);
                branchTarget = true; push16(0x7712); block_id = 557;
      break;
    case 710:  // $7713
      /*$7713*/ CYCLES(0x7713, 6);
                branchTarget = true; push16(0x7715); block_id = 55;
      break;
    case 711:  // $7716
      /*$7716*/ CYCLES(0x7716, 3);
                branchTarget = true; block_id = 721;
      break;
    case 712:  // $7719
      /*$7719*/ CYCLES(0x7719, 19);
      /*$771C*/ ram_poke(0x6255, ram_peek(0x7266));
      /*$7721*/ ram_poke(0x0022, 0x14);
      /*$7723*/ FUNC_HOME(0x7725);
                branchTarget = true; block_id = 713;
      break;
    case 713:  // $7726
      /*$7726*/ CYCLES(0x7726, 6);
                branchTarget = true; push16(0x7728); block_id = 557;
      break;
    case 714:  // $7729
      /*$7729*/ CYCLES(0x7729, 8);
                tmp1_U8 = ram_peek(0x0300);
                s_a = tmp1_U8;
      /*$772E*/ branchTarget = true; block_id = !(tmp1_U8 >= 0x03) ? 715 : 716;
      break;
    case 715:  // $772E
      /*$772E*/ CYCLES_EDGE(0x772e, 1);
                branchTarget = true; block_id = 720;
      break;
    case 716:  // $7730
      /*$7730*/ CYCLES(0x7730, 8);
                s_status_c = 0x01;
      /*$7731*/ block_id = s_status_d ? 718 : 717;
      break;
    case 717:  // $7731
      /*$7731*/ tmp3_U16 = s_a;
                tmp6_U16 = (tmp3_U16 - 0x0002) - (uint8_t)(0x01 - s_status_c);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)0xfffd);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 719;
      break;
    case 718:  // $7731
      /*$7731*/ tmp6_U16 = sbc_dec16(s_a, 0x02, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 719;
      break;
    case 719:  // $7733
      /*$7733*/ ram_poke(0x0300, s_a);
                block_id = 720;
      break;
    case 720:  // $7736
      /*$7736*/ CYCLES(0x7736, 6);
                branchTarget = true; push16(0x7738); block_id = 60;
      break;
    case 721:  // $7739
      /*$7739*/ CYCLES(0x7739, 8);
                tmp1_U8 = ram_peek(0x6253);
                s_a = tmp1_U8;
      /*$773E*/ branchTarget = true; block_id = !(tmp1_U8 != 0x0f) ? 722 : 723;
      break;
    case 722:  // $773E
      /*$773E*/ CYCLES_EDGE(0x773e, 1);
      /*$7743*/ CYCLES(0x7743, 76);
                s_status_d = 0x01;
      /*$7748*/ tmp6_U16 = sbc_dec16(ram_peek(0x725f), 0x01, 0x01);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 725;
      break;
    case 723:  // $7740
      /*$7740*/ CYCLES(0x7740, 3);
      /*$77E6*/ CYCLES(0x77e6, 4);
      /*$77E8*/ branchTarget = true; block_id = (s_a != 0x00) ? 759 : 760;
      break;
    case 724:  // $7748
      /*$7748*/ tmp6_U16 = (s_a - 0x0001) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp6_U16);
                block_id = 725;
      break;
    case 725:  // $774A
      /*$774A*/ ram_poke(0x725f, s_a);
      /*$774D*/ s_a = ram_peek(0x7260);
      /*$7750*/ block_id = s_status_d ? 727 : 726;
      break;
    case 726:  // $7750
      /*$7750*/ s_a = (uint8_t)(s_a - (uint8_t)(0x01 - s_status_c));
                block_id = 728;
      break;
    case 727:  // $7750
      /*$7750*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x00, s_status_c));
                block_id = 728;
      break;
    case 728:  // $7752
      /*$7752*/ ram_poke(0x7260, s_a);
      /*$7755*/ s_a = ram_peek(0x725a);
      /*$7758*/ s_status_c = 0x01;
      /*$7759*/ block_id = s_status_d ? 730 : 729;
      break;
    case 729:  // $7759
      /*$7759*/ tmp6_U16 = (s_a - 0x0001) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp6_U16);
                block_id = 731;
      break;
    case 730:  // $7759
      /*$7759*/ tmp6_U16 = sbc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 731;
      break;
    case 731:  // $775B
      /*$775B*/ ram_poke(0x725a, s_a);
      /*$775E*/ s_a = ram_peek(0x725b);
      /*$7761*/ block_id = s_status_d ? 733 : 732;
      break;
    case 732:  // $7761
      /*$7761*/ s_a = (uint8_t)(s_a - (uint8_t)(0x01 - s_status_c));
                block_id = 734;
      break;
    case 733:  // $7761
      /*$7761*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x00, s_status_c));
                block_id = 734;
      break;
    case 734:  // $7763
      /*$7763*/ ram_poke(0x725b, s_a);
      /*$7766*/ s_a = ram_peek(0x7261);
      /*$7769*/ s_status_c = 0x00;
      /*$776A*/ block_id = s_status_d ? 736 : 735;
      break;
    case 735:  // $776A
      /*$776A*/ tmp6_U16 = (s_a + 0x0001) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 737;
      break;
    case 736:  // $776A
      /*$776A*/ tmp6_U16 = adc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 737;
      break;
    case 737:  // $776C
      /*$776C*/ ram_poke(0x7261, s_a);
      /*$776F*/ s_a = ram_peek(0x7262);
      /*$7772*/ block_id = s_status_d ? 739 : 738;
      break;
    case 738:  // $7772
      /*$7772*/ s_a = (uint8_t)(s_a + s_status_c);
                block_id = 740;
      break;
    case 739:  // $7772
      /*$7772*/ s_a = ((uint8_t)adc_dec16(s_a, 0x00, s_status_c));
                block_id = 740;
      break;
    case 740:  // $7774
      /*$7774*/ ram_poke(0x7262, s_a);
      /*$7777*/ s_status_d = 0x00;
      /*$777B*/ branchTarget = true; block_id = ram_peek(0x7262) ? 741 : 742;
      break;
    case 741:  // $777B
      /*$777B*/ CYCLES_EDGE(0x777b, 1);
                branchTarget = true; block_id = 745;
      break;
    case 742:  // $777D
      /*$777D*/ CYCLES(0x777d, 8);
      /*$7782*/ branchTarget = true; block_id = (ram_peek(0x7261) >= 0x11) ? 743 : 744;
      break;
    case 743:  // $7782
      /*$7782*/ CYCLES_EDGE(0x7782, 1);
                branchTarget = true; block_id = 745;
      break;
    case 744:  // $7784
      /*$7784*/ CYCLES(0x7784, 6);
                func_7267(0x7786);
                branchTarget = true; block_id = 745;
      break;
    case 745:  // $7787
      /*$7787*/ CYCLES(0x7787, 18);
                s_a = ram_peek(0x6254);
      /*$778A*/ s_status_c = 0x00;
      /*$778B*/ block_id = s_status_d ? 747 : 746;
      break;
    case 746:  // $778B
      /*$778B*/ tmp6_U16 = s_a;
                tmp3_U16 = (tmp6_U16 + 0x000a) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)0x000a);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 748;
      break;
    case 747:  // $778B
      /*$778B*/ tmp3_U16 = adc_dec16(s_a, 0x0a, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
                block_id = 748;
      break;
    case 748:  // $778D
      /*$778D*/ ram_poke(0x6254, s_a);
      /*$7793*/ branchTarget = true; block_id = ram_peek(0x725a) ? 749 : 750;
      break;
    case 749:  // $7793
      /*$7793*/ CYCLES_EDGE(0x7793, 1);
                branchTarget = true; block_id = 753;
      break;
    case 750:  // $7795
      /*$7795*/ CYCLES(0x7795, 6);
      /*$7798*/ branchTarget = true; block_id = ram_peek(0x725b) ? 751 : 752;
      break;
    case 751:  // $7798
      /*$7798*/ CYCLES_EDGE(0x7798, 1);
                branchTarget = true; block_id = 753;
      break;
    case 752:  // $779A
      /*$779A*/ CYCLES(0x779a, 31);
      /*$779C*/ ram_poke(0x0001, 0x06);
      /*$77A0*/ ram_poke(0x0003, 0x00);
      /*$77A4*/ ram_poke(0x0002, 0x12);
      /*$77A8*/ ram_poke(0x0008, 0x16);
      /*$77AC*/ ram_poke(0x0000, 0x15);
      /*$77AE*/ func_6148(0x77b0);
      /*$77B1*/ CYCLES(0x77b1, 16);
      /*$77B3*/ ram_poke(0x0001, 0x00);
      /*$77B7*/ ram_poke(0x0002, 0x14);
      /*$77B9*/ func_60e4(0x77bb);
      /*$77BC*/ CYCLES(0x77bc, 14);
      /*$77BE*/ ram_poke(0x7266, 0xff);
      /*$77C1*/ s_a = 0x00;
      /*$77C3*/ FUNC_SETCOL(0x77c5);
      /*$77C6*/ CYCLES(0x77c6, 10);
                s_a = 0x00;
      /*$77C8*/ s_y = 0x14;
      /*$77CA*/ FUNC_MON_PLOT(0x77cc);
      /*$77CD*/ CYCLES(0x77cd, 3);
                branchTarget = true; block_id = 712;
      break;
    case 753:  // $77D0
      /*$77D0*/ CYCLES(0x77d0, 6);
      /*$77D3*/ branchTarget = true; block_id = !ram_peek(0x725f) ? 754 : 755;
      break;
    case 754:  // $77D3
      /*$77D3*/ CYCLES_EDGE(0x77d3, 1);
      /*$77D8*/ CYCLES(0x77d8, 6);
                tmp1_U8 = ram_peek(0x7260);
                s_a = tmp1_U8;
      /*$77DB*/ branchTarget = true; block_id = !tmp1_U8 ? 756 : 757;
      break;
    case 755:  // $77D5
      /*$77D5*/ CYCLES(0x77d5, 3);
                branchTarget = true; block_id = 712;
      break;
    case 756:  // $77DB
      /*$77DB*/ CYCLES_EDGE(0x77db, 1);
      /*$77E0*/ CYCLES(0x77e0, 6);
                func_7642(0x77e2);
                branchTarget = true; block_id = 758;
      break;
    case 757:  // $77DD
      /*$77DD*/ CYCLES(0x77dd, 3);
                branchTarget = true; block_id = 712;
      break;
    case 758:  // $77E3
      /*$77E3*/ CYCLES(0x77e3, 3);
                branchTarget = true; block_id = 712;
      break;
    case 759:  // $77E8
      /*$77E8*/ CYCLES_EDGE(0x77e8, 1);
      /*$7809*/ CYCLES(0x7809, 4);
      /*$780B*/ branchTarget = true; block_id = (s_a != 0xfe) ? 766 : 767;
      break;
    case 760:  // $77EA
      /*$77EA*/ CYCLES(0x77ea, 32);
      /*$77EF*/ s_a = ((uint8_t)adc_dec16(ram_peek(0x7265), 0x01, 0x00));
                block_id = 762;
      break;
    case 761:  // $77EF
      /*$77EF*/ s_a = (uint8_t)((s_a + 0x0001) + s_status_c);
                block_id = 762;
      break;
    case 762:  // $77F1
      /*$77F1*/ ram_poke(0x7265, s_a);
      /*$77F5*/ ram_poke(0x0303, (uint8_t)(ram_peek(0x0303) + 0x01));
      /*$77FE*/ branchTarget = true; block_id = (ram_peek(0x725e) != ram_peek(0x78b2)) ? 763 : 764;
      break;
    case 763:  // $77FE
      /*$77FE*/ CYCLES_EDGE(0x77fe, 1);
                branchTarget = true; block_id = 765;
      break;
    case 764:  // $7800
      /*$7800*/ CYCLES(0x7800, 6);
                branchTarget = true; push16(0x7802); block_id = 803;
      break;
    case 765:  // $7803
      /*$7803*/ CYCLES(0x7803, 6);
                func_7633(0x7805);
      /*$7806*/ CYCLES(0x7806, 3);
                branchTarget = true; block_id = 702;
      break;
    case 766:  // $780B
      /*$780B*/ CYCLES_EDGE(0x780b, 1);
                branchTarget = true; block_id = 783;
      break;
    case 767:  // $780D
      /*$780D*/ CYCLES(0x780d, 6);
      /*$7810*/ branchTarget = true; block_id = ram_peek(0x725b) ? 768 : 769;
      break;
    case 768:  // $7810
      /*$7810*/ CYCLES_EDGE(0x7810, 1);
                branchTarget = true; block_id = 771;
      break;
    case 769:  // $7812
      /*$7812*/ CYCLES(0x7812, 6);
      /*$7815*/ branchTarget = true; block_id = !ram_peek(0x725a) ? 770 : 771;
      break;
    case 770:  // $7815
      /*$7815*/ CYCLES_EDGE(0x7815, 1);
                branchTarget = true; block_id = 783;
      break;
    case 771:  // $7817
      /*$7817*/ CYCLES(0x7817, 54);
                s_status_d = 0x01;
      /*$781C*/ tmp3_U16 = adc_dec16(ram_peek(0x7263), 0x03, 0x00);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 773;
      break;
    case 772:  // $781C
      /*$781C*/ tmp3_U16 = (s_a + 0x0003) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 773;
      break;
    case 773:  // $781E
      /*$781E*/ ram_poke(0x7263, s_a);
      /*$7821*/ s_a = ram_peek(0x7264);
      /*$7824*/ block_id = s_status_d ? 775 : 774;
      break;
    case 774:  // $7824
      /*$7824*/ s_a = (uint8_t)(s_a + s_status_c);
                block_id = 776;
      break;
    case 775:  // $7824
      /*$7824*/ s_a = ((uint8_t)adc_dec16(s_a, 0x00, s_status_c));
                block_id = 776;
      break;
    case 776:  // $7826
      /*$7826*/ ram_poke(0x7264, s_a);
      /*$7829*/ s_a = ram_peek(0x725a);
      /*$782C*/ s_status_c = 0x00;
      /*$782D*/ block_id = s_status_d ? 778 : 777;
      break;
    case 777:  // $782D
      /*$782D*/ tmp3_U16 = (s_a + 0x0003) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 779;
      break;
    case 778:  // $782D
      /*$782D*/ tmp3_U16 = adc_dec16(s_a, 0x03, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 779;
      break;
    case 779:  // $782F
      /*$782F*/ ram_poke(0x725a, s_a);
      /*$7832*/ s_a = ram_peek(0x725b);
      /*$7835*/ block_id = s_status_d ? 781 : 780;
      break;
    case 780:  // $7835
      /*$7835*/ tmp3_U16 = s_a;
                tmp6_U16 = tmp3_U16 + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 782;
      break;
    case 781:  // $7835
      /*$7835*/ tmp6_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 782;
      break;
    case 782:  // $7837
      /*$7837*/ ram_poke(0x725b, s_a);
      /*$783A*/ s_status_d = 0x00;
      /*$783B*/ func_7642(0x783d);
      /*$783E*/ CYCLES(0x783e, 6);
                func_7642(0x7840);
      /*$7841*/ CYCLES(0x7841, 6);
                func_7642(0x7843);
      /*$7844*/ CYCLES(0x7844, 3);
                branchTarget = true; block_id = 712;
      break;
    case 783:  // $7847
      /*$7847*/ CYCLES(0x7847, 8);
                tmp1_U8 = ram_peek(0x6253);
                s_a = tmp1_U8;
      /*$784C*/ branchTarget = true; block_id = (tmp1_U8 != 0xff) ? 784 : 785;
      break;
    case 784:  // $784C
      /*$784C*/ CYCLES_EDGE(0x784c, 1);
      /*$7851*/ CYCLES(0x7851, 4);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0xfe);
      /*$7853*/ branchTarget = true; block_id = (tmp1_U8 != 0xfe) ? 786 : 787;
      break;
    case 785:  // $784E
      /*$784E*/ CYCLES(0x784e, 3);
                branchTarget = true; block_id = 699;
      break;
    case 786:  // $7853
      /*$7853*/ CYCLES_EDGE(0x7853, 1);
      /*$7858*/ CYCLES(0x7858, 6);
      /*$785B*/ branchTarget = true; block_id = ram_peek(0x0302) ? 788 : 789;
      break;
    case 787:  // $7855
      /*$7855*/ CYCLES(0x7855, 3);
                branchTarget = true; block_id = 712;
      break;
    case 788:  // $785B
      /*$785B*/ CYCLES_EDGE(0x785b, 1);
                branchTarget = true; block_id = 798;
      break;
    case 789:  // $785D
      /*$785D*/ CYCLES(0x785d, 16);
      /*$785F*/ ram_poke(0x0025, 0x17);
      /*$7863*/ ram_poke(0x0024, 0x00);
      /*$7865*/ branchTarget = true; push16(0x7867); block_id = 519;
      break;
    case 790:  // $7868
      /*$7868*/ CYCLES(0x7868, 0);
                fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x7868, 0x7868, 0x03); error_handler(0x7868);
      break;
    case 791:  // $7886
      /*$7886*/ CYCLES(0x7886, 6);
      /*$7889*/ branchTarget = true; block_id = !ram_peek(0x6c71) ? 792 : 793;
      break;
    case 792:  // $7889
      /*$7889*/ CYCLES_EDGE(0x7889, 1);
                branchTarget = true; block_id = 795;
      break;
    case 793:  // $788B
      /*$788B*/ CYCLES(0x788b, 6);
                tmp1_U8 = io_peek(0xc061);
      /*$788E*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 794 : 795;
      break;
    case 794:  // $788E
      /*$788E*/ CYCLES_EDGE(0x788e, 1);
                branchTarget = true; block_id = 798;
      break;
    case 795:  // $7890
      /*$7890*/ CYCLES(0x7890, 8);
                tmp1_U8 = io_peek(0xc000);
                s_a = tmp1_U8;
      /*$7895*/ branchTarget = true; block_id = (tmp1_U8 != 0xa0) ? 796 : 797;
      break;
    case 796:  // $7895
      /*$7895*/ CYCLES_EDGE(0x7895, 1);
                branchTarget = true; block_id = 791;
      break;
    case 797:  // $7897
      /*$7897*/ CYCLES(0x7897, 4);
                io_poke(0xc010, s_a);
                block_id = 798;
      break;
    case 798:  // $789A
      /*$789A*/ CYCLES(0x789a, 6);
      /*$789D*/ branchTarget = true; block_id = ram_peek(0x725e) ? 799 : 800;
      break;
    case 799:  // $789D
      /*$789D*/ CYCLES_EDGE(0x789d, 1);
      /*$78A2*/ CYCLES(0x78a2, 19);
      /*$78A7*/ tmp6_U16 = sbc_dec16(ram_peek(0x725e), 0x01, 0x01);
                s_a = ((uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 802;
      break;
    case 800:  // $789F
      /*$789F*/ CYCLES(0x789f, 3);
                branchTarget = true; block_id = 699;
      break;
    case 801:  // $78A7
      /*$78A7*/ tmp6_U16 = s_a;
                tmp3_U16 = (tmp6_U16 - 0x0001) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)0xfffe);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 802;
      break;
    case 802:  // $78A9
      /*$78A9*/ ram_poke(0x725e, s_a);
      /*$78AC*/ s_status_d = 0x00;
      /*$78AD*/ branchTarget = true; block_id = 704;
      break;
    case 803:  // $78B3
      /*$78B3*/ CYCLES(0x78b3, 36);
                s_status_d = 0x01;
      /*$78B8*/ tmp3_U16 = adc_dec16(ram_peek(0x71cb), ram_peek(0x71cb), 0x00);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 805;
      break;
    case 804:  // $78B8
      /*$78B8*/ tmp3_U16 = (s_a + ram_peek(0x71cb)) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 805;
      break;
    case 805:  // $78BB
      /*$78BB*/ ram_poke(0x78b0, s_a);
      /*$78BE*/ s_a = ram_peek(0x71cc);
      /*$78C1*/ block_id = s_status_d ? 807 : 806;
      break;
    case 806:  // $78C1
      /*$78C1*/ s_a = (uint8_t)((s_a + ram_peek(0x71cc)) + s_status_c);
                block_id = 808;
      break;
    case 807:  // $78C1
      /*$78C1*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x71cc), s_status_c));
                block_id = 808;
      break;
    case 808:  // $78C4
      /*$78C4*/ ram_poke(0x78b1, s_a);
      /*$78C8*/ func_7267(0x78ca);
      /*$78CB*/ CYCLES(0x78cb, 6);
                func_7267(0x78cd);
      /*$78CE*/ CYCLES(0x78ce, 6);
                branchTarget = true; push16(0x78d0); block_id = 557;
      break;
    case 809:  // $78D1
      /*$78D1*/ CYCLES(0x78d1, 31);
      /*$78D3*/ ram_poke(0x0000, 0x01);
      /*$78D7*/ ram_poke(0x0001, 0x09);
      /*$78DB*/ ram_poke(0x0002, 0x0d);
      /*$78DF*/ ram_poke(0x0003, 0x10);
      /*$78E3*/ ram_poke(0x0008, 0x1a);
      /*$78E5*/ func_6148(0x78e7);
      /*$78E8*/ CYCLES(0x78e8, 16);
      /*$78EA*/ ram_poke(0x0002, 0x0d);
      /*$78EE*/ ram_poke(0x0003, 0x15);
      /*$78F0*/ func_6148(0x78f2);
      /*$78F3*/ CYCLES(0x78f3, 16);
      /*$78F5*/ ram_poke(0x0003, 0x10);
      /*$78F9*/ ram_poke(0x0008, 0x15);
      /*$78FB*/ func_615a(0x78fd);
      /*$78FE*/ CYCLES(0x78fe, 16);
      /*$7900*/ ram_poke(0x0002, 0x0d);
      /*$7904*/ ram_poke(0x0003, 0x10);
      /*$7906*/ func_615a(0x7908);
      /*$7909*/ CYCLES(0x7909, 26);
      /*$790B*/ ram_poke(0x0001, 0x00);
      /*$790F*/ ram_poke(0x0008, 0x19);
      /*$7913*/ ram_poke(0x0003, 0x11);
      /*$7917*/ ram_poke(0x0002, 0x0e);
      /*$7919*/ func_6148(0x791b);
      /*$791C*/ CYCLES(0x791c, 16);
      /*$791E*/ ram_poke(0x0002, 0x0e);
      /*$7920*/ ram_poke(0x0003, (uint8_t)(ram_peek(0x0003) + 0x01));
      /*$7922*/ func_6148(0x7924);
      /*$7925*/ CYCLES(0x7925, 16);
      /*$7927*/ ram_poke(0x0002, 0x0e);
      /*$7929*/ ram_poke(0x0003, (uint8_t)(ram_peek(0x0003) + 0x01));
      /*$792B*/ func_6148(0x792d);
      /*$792E*/ CYCLES(0x792e, 16);
      /*$7930*/ ram_poke(0x0002, 0x0e);
      /*$7932*/ ram_poke(0x0003, (uint8_t)(ram_peek(0x0003) + 0x01));
      /*$7934*/ func_6148(0x7936);
      /*$7937*/ CYCLES(0x7937, 16);
      /*$7939*/ ram_poke(0x0024, 0x0f);
      /*$793D*/ ram_poke(0x0025, 0x09);
      /*$793F*/ func_6641(0x7941);
      /*$7942*/ CYCLES(0x7942, 6);
                branchTarget = true; push16(0x7944); block_id = 519;
      break;
    case 810:  // $7945
      /*$7945*/ CYCLES(0x7945, 0);
                fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x7945, 0x7945, 0x03); error_handler(0x7945);
      break;
    case 811:  // $794D
      /*$794D*/ CYCLES(0x794d, 15);
      /*$794F*/ ram_poke(0x002c, 0x00);
      /*$7951*/ s_a = ram_peek(0x78b1);
      /*$7954*/ branchTarget = true; push16(0x7956); block_id = 497;
      break;
    case 812:  // $7957
      /*$7957*/ CYCLES(0x7957, 10);
                s_a = ram_peek(0x78b0);
      /*$795A*/ branchTarget = true; push16(0x795c); block_id = 497;
      break;
    case 813:  // $795D
      /*$795D*/ CYCLES(0x795d, 15);
      /*$795F*/ ram_poke(0x0036, 0xf0);
      /*$7963*/ ram_poke(0x0037, 0xfd);
      /*$7967*/ ram_poke(0x0002, 0x20);
                block_id = 814;
      break;
    case 814:  // $7969
      /*$7969*/ CYCLES(0x7969, 2);
                s_x = 0x80;
                block_id = 815;
      break;
    case 815:  // $796B
      /*$796B*/ CYCLES(0x796b, 4);
      /*$796C*/ s_y = s_x;
                block_id = 816;
      break;
    case 816:  // $796D
      /*$796D*/ CYCLES(0x796d, 4);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp1_U8;
      /*$796E*/ branchTarget = true; block_id = tmp1_U8 ? 817 : 818;
      break;
    case 817:  // $796E
      /*$796E*/ CYCLES_EDGE(0x796e, 1);
                branchTarget = true; block_id = 816;
      break;
    case 818:  // $7970
      /*$7970*/ CYCLES(0x7970, 12);
                tmp1_U8 = ram_peek(0x6c49);
                s_y = tmp1_U8;
      /*$7973*/ tmp1_U8 = peek((0xc000 + tmp1_U8));
                s_a = tmp1_U8;
      /*$7976*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp1_U8;
      /*$7977*/ branchTarget = true; block_id = tmp1_U8 ? 819 : 820;
      break;
    case 819:  // $7977
      /*$7977*/ CYCLES_EDGE(0x7977, 1);
                branchTarget = true; block_id = 815;
      break;
    case 820:  // $7979
      /*$7979*/ CYCLES(0x7979, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x0002) - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x0002, tmp1_U8);
      /*$797B*/ branchTarget = true; block_id = tmp1_U8 ? 821 : 822;
      break;
    case 821:  // $797B
      /*$797B*/ CYCLES_EDGE(0x797b, 1);
                branchTarget = true; block_id = 814;
      break;
    case 822:  // $797D
      /*$797D*/ CYCLES(0x797d, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x797d, pop16() + 1);;
      break;
    case 823:  // $7980
      /*$7980*/ CYCLES(0x7980, 7);
                tmp1_U8 = ram_peek(0x000f);
                s_a = tmp1_U8;
      /*$7982*/ tmp1_U8 = tmp1_U8 >= 0x1f;
                s_status_c = tmp1_U8;
      /*$7984*/ branchTarget = true; block_id = tmp1_U8 ? 824 : 825;
      break;
    case 824:  // $7984
      /*$7984*/ CYCLES_EDGE(0x7984, 1);
                branchTarget = true; block_id = 827;
      break;
    case 825:  // $7986
      /*$7986*/ CYCLES(0x7986, 4);
                tmp1_U8 = s_a >= 0x18;
                s_status_c = tmp1_U8;
      /*$7988*/ branchTarget = true; block_id = tmp1_U8 ? 826 : 827;
      break;
    case 826:  // $7988
      /*$7988*/ CYCLES_EDGE(0x7988, 1);
                branchTarget = true; block_id = 828;
      break;
    case 827:  // $798A
      /*$798A*/ CYCLES(0x798a, 8);
      /*$798E*/ ram_poke(0x000e, (ram_peek(0x000e) & 0xde));
                block_id = 828;
      break;
    case 828:  // $7990
      /*$7990*/ CYCLES(0x7990, 13);
      /*$7996*/ ram_poke(0x000f, ((ram_peek(0x000f) & 0x1f) | 0x18));
      /*$73D8*/ CYCLES(0x73d8, 6);
      /*$73DB*/ branchTarget = true; block_id = ram_peek(0x73d7) ? 606 : 607;
      break;
    case 829:  // $D393
      /*$D393*/ CYCLES(0xd393, 6);
                branchTarget = true; push16(0xd395); block_id = 867;
      break;
    case 830:  // $D396
      /*$D396*/ CYCLES(0xd396, 6);
                ram_poke(0x006d, s_a);
      /*$D398*/ ram_poke(0x006e, s_y);
                FUNC_MVBLKUP2(0x0000);
                block_id = find_block_id_func_t001(0xd398, pop16() + 1);;
      break;
    case 831:  // $D39A
      /*$D39A*/ CYCLES(0xd39a, 27);
                s_status_c = 0x01;
      /*$D39B*/ s_a = ram_peek(0x0096);
      /*$D39D*/ block_id = s_status_d ? 833 : 832;
      break;
    case 832:  // $D39D
      /*$D39D*/ tmp3_U16 = (s_a - ram_peek(0x009b)) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp3_U16);
                block_id = 834;
      break;
    case 833:  // $D39D
      /*$D39D*/ tmp3_U16 = sbc_dec16(s_a, ram_peek(0x009b), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 834;
      break;
    case 834:  // $D39F
      /*$D39F*/ tmp5_U8 = s_a;
                ram_poke(0x005e, tmp5_U8);
      /*$D3A1*/ s_y = tmp5_U8;
      /*$D3A2*/ s_a = ram_peek(0x0097);
      /*$D3A4*/ block_id = s_status_d ? 836 : 835;
      break;
    case 835:  // $D3A4
      /*$D3A4*/ tmp3_U16 = s_a;
                tmp6_U16 = ram_peek(0x009c);
                tmp4_U16 = (tmp3_U16 - tmp6_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp6_U16));
                s_a = ((uint8_t)tmp4_U16);
                block_id = 837;
      break;
    case 836:  // $D3A4
      /*$D3A4*/ tmp4_U16 = sbc_dec16(s_a, ram_peek(0x009c), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 837;
      break;
    case 837:  // $D3A6
      /*$D3A7*/ s_x = (uint8_t)(s_a + 0x01);
      /*$D3A8*/ tmp5_U8 = s_y;
                s_a = tmp5_U8;
      /*$D3A9*/ branchTarget = true; block_id = !tmp5_U8 ? 838 : 839;
      break;
    case 838:  // $D3A9
      /*$D3A9*/ CYCLES_EDGE(0xd3a9, 1);
                branchTarget = true; block_id = 856;
      break;
    case 839:  // $D3AB
      /*$D3AB*/ CYCLES(0xd3ab, 13);
                s_a = ram_peek(0x0096);
      /*$D3AD*/ s_status_c = 0x01;
      /*$D3AE*/ block_id = s_status_d ? 841 : 840;
      break;
    case 840:  // $D3AE
      /*$D3AE*/ tmp4_U16 = (s_a - ram_peek(0x005e)) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp4_U16);
                block_id = 842;
      break;
    case 841:  // $D3AE
      /*$D3AE*/ tmp4_U16 = sbc_dec16(s_a, ram_peek(0x005e), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 842;
      break;
    case 842:  // $D3B0
      /*$D3B0*/ ram_poke(0x0096, s_a);
      /*$D3B2*/ branchTarget = true; block_id = s_status_c ? 843 : 844;
      break;
    case 843:  // $D3B2
      /*$D3B2*/ CYCLES_EDGE(0xd3b2, 1);
                branchTarget = true; block_id = 845;
      break;
    case 844:  // $D3B4
      /*$D3B4*/ CYCLES(0xd3b4, 7);
                ram_poke(0x0097, (uint8_t)(ram_peek(0x0097) - 0x01));
      /*$D3B6*/ s_status_c = 0x01;
                block_id = 845;
      break;
    case 845:  // $D3B7
      /*$D3B7*/ CYCLES(0xd3b7, 11);
                s_a = ram_peek(0x0094);
      /*$D3B9*/ block_id = s_status_d ? 847 : 846;
      break;
    case 846:  // $D3B9
      /*$D3B9*/ tmp4_U16 = s_a;
                tmp6_U16 = ram_peek(0x005e);
                tmp3_U16 = (tmp4_U16 - tmp6_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)(~tmp6_U16));
                s_a = ((uint8_t)tmp3_U16);
                block_id = 848;
      break;
    case 847:  // $D3B9
      /*$D3B9*/ tmp3_U16 = sbc_dec16(s_a, ram_peek(0x005e), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 848;
      break;
    case 848:  // $D3BB
      /*$D3BB*/ ram_poke(0x0094, s_a);
      /*$D3BD*/ branchTarget = true; block_id = s_status_c ? 849 : 850;
      break;
    case 849:  // $D3BD
      /*$D3BD*/ CYCLES_EDGE(0xd3bd, 1);
                branchTarget = true; block_id = 853;
      break;
    case 850:  // $D3BF
      /*$D3BF*/ CYCLES(0xd3bf, 7);
                ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) - 0x01));
      /*$D3C1*/ branchTarget = true; block_id = !s_status_c ? 851 : 852;
      break;
    case 851:  // $D3C1
      /*$D3C1*/ CYCLES_EDGE(0xd3c1, 1);
                branchTarget = true; block_id = 853;
      break;
    case 852:  // $D3C3
      /*$D3C3*/ CYCLES(0xd3c3, 11);
                tmp5_U8 = s_y;
                tmp2_U8 = peek((ram_peek16al(0x0096) + tmp5_U8));
      /*$D3C5*/ poke((ram_peek16al(0x0094) + tmp5_U8), tmp2_U8);
                block_id = 853;
      break;
    case 853:  // $D3C7
      /*$D3C7*/ CYCLES(0xd3c7, 4);
                tmp5_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp5_U8;
      /*$D3C8*/ branchTarget = true; block_id = tmp5_U8 ? 854 : 855;
      break;
    case 854:  // $D3C8
      /*$D3C8*/ CYCLES_EDGE(0xd3c8, 1);
                branchTarget = true; block_id = 852;
      break;
    case 855:  // $D3CA
      /*$D3CA*/ CYCLES(0xd3ca, 11);
                tmp5_U8 = s_y;
                tmp2_U8 = peek((ram_peek16al(0x0096) + tmp5_U8));
                s_a = tmp2_U8;
      /*$D3CC*/ poke((ram_peek16al(0x0094) + tmp5_U8), tmp2_U8);
                block_id = 856;
      break;
    case 856:  // $D3CE
      /*$D3CE*/ CYCLES(0xd3ce, 14);
                ram_poke(0x0097, (uint8_t)(ram_peek(0x0097) - 0x01));
      /*$D3D0*/ ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) - 0x01));
      /*$D3D2*/ tmp5_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_x = tmp5_U8;
      /*$D3D3*/ branchTarget = true; block_id = tmp5_U8 ? 857 : 858;
      break;
    case 857:  // $D3D3
      /*$D3D3*/ CYCLES_EDGE(0xd3d3, 1);
                branchTarget = true; block_id = 853;
      break;
    case 858:  // $D3D5
      /*$D3D5*/ CYCLES(0xd3d5, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xd3d5, pop16() + 1);;
      break;
    case 859:  // $D3D6
      /*$D3D6*/ CYCLES(0xd3d6, 6);
                tmp3_U16 = s_a << 0x01;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
      /*$D3D7*/ block_id = s_status_d ? 861 : 860;
      break;
    case 860:  // $D3D7
      /*$D3D7*/ tmp3_U16 = s_a;
                tmp6_U16 = (tmp3_U16 + 0x0036) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)0x0036);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 862;
      break;
    case 861:  // $D3D7
      /*$D3D7*/ tmp6_U16 = adc_dec16(s_a, 0x36, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 862;
      break;
    case 862:  // $D3D9
      /*$D3D9*/ branchTarget = true; block_id = s_status_c ? 863 : 864;
      break;
    case 863:  // $D3D9
      /*$D3D9*/ CYCLES_EDGE(0xd3d9, 1);
                branchTarget = true; block_id = 886;
      break;
    case 864:  // $D3DB
      /*$D3DB*/ CYCLES(0xd3db, 10);
                ram_poke(0x005e, s_a);
      /*$D3DD*/ tmp5_U8 = s_sp;
                s_x = tmp5_U8;
      /*$D3DE*/ tmp2_U8 = ram_peek(0x005e);
                s_status_not_z = (tmp5_U8 != tmp2_U8);
                tmp1_U8 = tmp5_U8 >= tmp2_U8;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp5_U8 - tmp2_U8) & 0x80);
      /*$D3E0*/ branchTarget = true; block_id = !tmp1_U8 ? 865 : 866;
      break;
    case 865:  // $D3E0
      /*$D3E0*/ CYCLES_EDGE(0xd3e0, 1);
                branchTarget = true; block_id = 886;
      break;
    case 866:  // $D3E2
      /*$D3E2*/ CYCLES(0xd3e2, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xd3e2, pop16() + 1);;
      break;
    case 867:  // $D3E3
      /*$D3E3*/ CYCLES(0xd3e3, 5);
                tmp1_U8 = s_y;
                tmp2_U8 = ram_peek(0x0070);
                s_status_not_z = (tmp1_U8 != tmp2_U8);
                tmp5_U8 = tmp1_U8 >= tmp2_U8;
                s_status_c = tmp5_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
      /*$D3E5*/ branchTarget = true; block_id = !tmp5_U8 ? 868 : 869;
      break;
    case 868:  // $D3E5
      /*$D3E5*/ CYCLES_EDGE(0xd3e5, 1);
                branchTarget = true; block_id = 885;
      break;
    case 869:  // $D3E7
      /*$D3E7*/ CYCLES(0xd3e7, 2);
                branchTarget = true; block_id = s_status_not_z ? 870 : 871;
      break;
    case 870:  // $D3E7
      /*$D3E7*/ CYCLES_EDGE(0xd3e7, 1);
                branchTarget = true; block_id = 873;
      break;
    case 871:  // $D3E9
      /*$D3E9*/ CYCLES(0xd3e9, 5);
                tmp1_U8 = s_a;
                tmp2_U8 = ram_peek(0x006f);
                s_status_not_z = (tmp1_U8 != tmp2_U8);
                tmp5_U8 = tmp1_U8 >= tmp2_U8;
                s_status_c = tmp5_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
      /*$D3EB*/ branchTarget = true; block_id = !tmp5_U8 ? 872 : 873;
      break;
    case 872:  // $D3EB
      /*$D3EB*/ CYCLES_EDGE(0xd3eb, 1);
                branchTarget = true; block_id = 885;
      break;
    case 873:  // $D3ED
      /*$D3ED*/ CYCLES(0xd3ed, 7);
                push8(s_a);
      /*$D3EE*/ s_x = 0x09;
      /*$D3F0*/ s_a = s_y;
                block_id = 874;
      break;
    case 874:  // $D3F1
      /*$D3F1*/ CYCLES(0xd3f1, 11);
                push8(s_a);
      /*$D3F2*/ tmp5_U8 = s_x;
                s_a = ram_peek((uint8_t)(0x93 + tmp5_U8));
      /*$D3F4*/ tmp5_U8 = (uint8_t)(tmp5_U8 - 0x01);
                s_x = tmp5_U8;
      /*$D3F5*/ branchTarget = true; block_id = !(tmp5_U8 & 0x80) ? 875 : 876;
      break;
    case 875:  // $D3F5
      /*$D3F5*/ CYCLES_EDGE(0xd3f5, 1);
                branchTarget = true; block_id = 874;
      break;
    case 876:  // $D3F7
      /*$D3F7*/ CYCLES(0xd3f7, 6);
                FUNC_GARBAGE(0xd3f9);
      /*$D3FA*/ CYCLES(0xd3fa, 2);
                s_x = 0xf7;
                block_id = 877;
      break;
    case 877:  // $D3FC
      /*$D3FC*/ CYCLES(0xd3fc, 12);
                tmp5_U8 = pop8();
      /*$D3FD*/ tmp2_U8 = s_x;
                ram_poke((uint8_t)(0x9d + tmp2_U8), tmp5_U8);
      /*$D3FF*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_x = tmp2_U8;
      /*$D400*/ branchTarget = true; block_id = (tmp2_U8 & 0x80) ? 878 : 879;
      break;
    case 878:  // $D400
      /*$D400*/ CYCLES_EDGE(0xd400, 1);
                branchTarget = true; block_id = 877;
      break;
    case 879:  // $D402
      /*$D402*/ CYCLES(0xd402, 15);
                tmp2_U8 = pop8();
      /*$D403*/ s_y = tmp2_U8;
      /*$D404*/ tmp5_U8 = pop8();
                s_a = tmp5_U8;
      /*$D405*/ tmp5_U8 = ram_peek(0x0070);
                s_status_not_z = (tmp2_U8 != tmp5_U8);
                tmp1_U8 = tmp2_U8 >= tmp5_U8;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp5_U8) & 0x80);
      /*$D407*/ branchTarget = true; block_id = !tmp1_U8 ? 880 : 881;
      break;
    case 880:  // $D407
      /*$D407*/ CYCLES_EDGE(0xd407, 1);
                branchTarget = true; block_id = 885;
      break;
    case 881:  // $D409
      /*$D409*/ CYCLES(0xd409, 2);
                branchTarget = true; block_id = s_status_not_z ? 882 : 883;
      break;
    case 882:  // $D409
      /*$D409*/ CYCLES_EDGE(0xd409, 1);
                branchTarget = true; block_id = 886;
      break;
    case 883:  // $D40B
      /*$D40B*/ CYCLES(0xd40b, 5);
                tmp1_U8 = s_a;
                tmp5_U8 = ram_peek(0x006f);
                s_status_not_z = (tmp1_U8 != tmp5_U8);
                tmp2_U8 = tmp1_U8 >= tmp5_U8;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - tmp5_U8) & 0x80);
      /*$D40D*/ branchTarget = true; block_id = tmp2_U8 ? 884 : 885;
      break;
    case 884:  // $D40D
      /*$D40D*/ CYCLES_EDGE(0xd40d, 1);
                branchTarget = true; block_id = 886;
      break;
    case 885:  // $D40F
      /*$D40F*/ CYCLES(0xd40f, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xd40f, pop16() + 1);;
      break;
    case 886:  // $D410
      /*$D410*/ CYCLES(0xd410, 2);
                s_x = 0x4d;
                block_id = 887;
      break;
    case 887:  // $D412
      /*$D412*/ CYCLES(0xd412, 5);
                tmp2_U8 = ram_peek(0x00d8);
                s_status_v = ((tmp2_U8 >> 0x06) & 0x01);
      /*$D414*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 888 : 889;
      break;
    case 888:  // $D414
      /*$D414*/ CYCLES_EDGE(0xd414, 1);
      /*$D419*/ CYCLES(0xd419, 6);
                branchTarget = true; push16(0xd41b); block_id = 1239;
      break;
    case 889:  // $D416
      /*$D416*/ CYCLES(0xd416, 3);
                branchTarget = true; block_id = 2365;
      break;
    case 890:  // $D41C
      /*$D41C*/ CYCLES(0xd41c, 6);
                branchTarget = true; push16(0xd41e); block_id = 1252;
      break;
    case 891:  // $D41F
      /*$D41F*/ CYCLES(0xd41f, 13);
                tmp2_U8 = ram_peek((0xd260 + s_x));
                s_a = tmp2_U8;
      /*$D422*/ push8(tmp2_U8);
      /*$D423*/ branchTarget = true; push16(0xd425); block_id = 1253;
      break;
    case 892:  // $D426
      /*$D426*/ CYCLES(0xd426, 8);
                s_x = (uint8_t)(s_x + 0x01);
      /*$D427*/ tmp2_U8 = pop8();
      /*$D428*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 893 : 894;
      break;
    case 893:  // $D428
      /*$D428*/ CYCLES_EDGE(0xd428, 1);
                branchTarget = true; block_id = 891;
      break;
    case 894:  // $D42A
      /*$D42A*/ CYCLES(0xd42a, 6);
                branchTarget = true; push16(0xd42c); block_id = 1079;
      break;
    case 895:  // $D42D
      /*$D42D*/ CYCLES(0xd42d, 4);
                s_a = 0x50;
      /*$D42F*/ s_y = 0xd3;
                block_id = 896;
      break;
    case 896:  // $D431
      /*$D431*/ CYCLES(0xd431, 6);
                branchTarget = true; push16(0xd433); block_id = 1243;
      break;
    case 897:  // $D434
      /*$D434*/ CYCLES(0xd434, 7);
      /*$D436*/ tmp5_U8 = (uint8_t)(ram_peek(0x0076) + 0x01);
                s_y = tmp5_U8;
      /*$D437*/ branchTarget = true; block_id = !tmp5_U8 ? 898 : 899;
      break;
    case 898:  // $D437
      /*$D437*/ CYCLES_EDGE(0xd437, 1);
                branchTarget = true; block_id = 900;
      break;
    case 899:  // $D439
      /*$D439*/ CYCLES(0xd439, 6);
                branchTarget = true; push16(0xd43b); block_id = 2216;
      break;
    case 900:  // $D43C
      /*$D43C*/ CYCLES(0xd43c, 6);
                branchTarget = true; push16(0xd43e); block_id = 1239;
      break;
    case 901:  // $D43F
      /*$D43F*/ CYCLES(0xd43f, 8);
                s_x = 0xdd;
      /*$D441*/ branchTarget = true; push16(0xd443); block_id = 968;
      break;
    case 902:  // $D444
      /*$D444*/ CYCLES(0xd444, 17);
                ram_poke(0x00b8, s_x);
      /*$D446*/ ram_poke(0x00b9, s_y);
      /*$D448*/ tmp5_U8 = ram_peek(0x00d8);
                s_status_c = (tmp5_U8 & 0x01);
                ram_poke(0x00d8, (tmp5_U8 >> 0x01));
      /*$D44A*/ branchTarget = true; push16(0xd44c); block_id = 2;
      break;
    case 903:  // $D44D
      /*$D44D*/ CYCLES(0xd44d, 4);
                tmp1_U8 = s_a;
                s_x = tmp1_U8;
      /*$D44E*/ branchTarget = true; block_id = !tmp1_U8 ? 904 : 905;
      break;
    case 904:  // $D44E
      /*$D44E*/ CYCLES_EDGE(0xd44e, 1);
                branchTarget = true; block_id = 900;
      break;
    case 905:  // $D450
      /*$D450*/ CYCLES(0xd450, 7);
                s_x = 0xff;
      /*$D452*/ ram_poke(0x0076, 0xff);
      /*$D454*/ branchTarget = true; block_id = !s_status_c ? 906 : 907;
      break;
    case 906:  // $D454
      /*$D454*/ CYCLES_EDGE(0xd454, 1);
      /*$D45C*/ CYCLES(0xd45c, 18);
      /*$D45E*/ ram_poke(0x0069, ram_peek(0x00af));
      /*$D462*/ ram_poke(0x006a, ram_peek(0x00b0));
      /*$D464*/ branchTarget = true; push16(0xd466); block_id = 1192;
      break;
    case 907:  // $D456
      /*$D456*/ CYCLES(0xd456, 6);
                branchTarget = true; push16(0xd458); block_id = 979;
      break;
    case 908:  // $D459
      /*$D459*/ CYCLES(0xd459, 3);
                branchTarget = true; block_id = 1102;
      break;
    case 909:  // $D467
      /*$D467*/ CYCLES(0xd467, 6);
                branchTarget = true; push16(0xd469); block_id = 979;
      break;
    case 910:  // $D46A
      /*$D46A*/ CYCLES(0xd46a, 9);
                ram_poke(0x000f, s_y);
      /*$D46C*/ FUNC_FNDLIN(0xd46e);
      /*$D46F*/ CYCLES(0xd46f, 2);
                branchTarget = true; block_id = !s_status_c ? 911 : 912;
      break;
    case 911:  // $D46F
      /*$D46F*/ CYCLES_EDGE(0xd46f, 1);
                branchTarget = true; block_id = 940;
      break;
    case 912:  // $D471
      /*$D471*/ CYCLES(0xd471, 68);
      /*$D473*/ tmp2_U8 = peek((ram_peek16(0x009b) + 0x0001));
      /*$D475*/ ram_poke(0x005f, tmp2_U8);
      /*$D479*/ ram_poke(0x005e, ram_peek(0x0069));
      /*$D47D*/ ram_poke(0x0061, ram_peek(0x009c));
      /*$D47F*/ s_a = ram_peek(0x009b);
      /*$D481*/ s_y = 0x00;
      /*$D482*/ block_id = s_status_d ? 914 : 913;
      break;
    case 913:  // $D482
      /*$D482*/ tmp2_U8 = peek((ram_peek16(0x009b) + s_y));
                s_a = (uint8_t)((s_a - tmp2_U8) - (uint8_t)(0x01 - s_status_c));
                block_id = 915;
      break;
    case 914:  // $D482
      /*$D482*/ tmp2_U8 = peek((ram_peek16(0x009b) + s_y));
                s_a = ((uint8_t)sbc_dec16(s_a, tmp2_U8, s_status_c));
                block_id = 915;
      break;
    case 915:  // $D484
      /*$D484*/ s_status_c = 0x00;
      /*$D485*/ block_id = s_status_d ? 917 : 916;
      break;
    case 916:  // $D485
      /*$D485*/ tmp6_U16 = (s_a + ram_peek(0x0069)) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 918;
      break;
    case 917:  // $D485
      /*$D485*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x0069), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 918;
      break;
    case 918:  // $D487
      /*$D487*/ tmp2_U8 = s_a;
                ram_poke(0x0069, tmp2_U8);
      /*$D489*/ ram_poke(0x0060, tmp2_U8);
      /*$D48B*/ s_a = ram_peek(0x006a);
      /*$D48D*/ block_id = s_status_d ? 920 : 919;
      break;
    case 919:  // $D48D
      /*$D48D*/ tmp6_U16 = (s_a + 0x00ff) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 921;
      break;
    case 920:  // $D48D
      /*$D48D*/ tmp6_U16 = adc_dec16(s_a, 0xff, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 921;
      break;
    case 921:  // $D48F
      /*$D48F*/ ram_poke(0x006a, s_a);
      /*$D491*/ block_id = s_status_d ? 923 : 922;
      break;
    case 922:  // $D491
      /*$D491*/ s_a = (uint8_t)((s_a - ram_peek(0x009c)) - (uint8_t)(0x01 - s_status_c));
                block_id = 924;
      break;
    case 923:  // $D491
      /*$D491*/ s_a = ((uint8_t)sbc_dec16(s_a, ram_peek(0x009c), s_status_c));
                block_id = 924;
      break;
    case 924:  // $D493
      /*$D493*/ s_x = s_a;
      /*$D494*/ s_status_c = 0x01;
      /*$D495*/ s_a = ram_peek(0x009b);
      /*$D497*/ block_id = s_status_d ? 926 : 925;
      break;
    case 925:  // $D497
      /*$D497*/ tmp6_U16 = (s_a - ram_peek(0x0069)) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp6_U16);
                block_id = 927;
      break;
    case 926:  // $D497
      /*$D497*/ tmp6_U16 = sbc_dec16(s_a, ram_peek(0x0069), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 927;
      break;
    case 927:  // $D499
      /*$D499*/ s_y = s_a;
      /*$D49A*/ branchTarget = true; block_id = s_status_c ? 928 : 929;
      break;
    case 928:  // $D49A
      /*$D49A*/ CYCLES_EDGE(0xd49a, 1);
                branchTarget = true; block_id = 930;
      break;
    case 929:  // $D49C
      /*$D49C*/ CYCLES(0xd49c, 7);
                s_x = (uint8_t)(s_x + 0x01);
      /*$D49D*/ ram_poke(0x0061, (uint8_t)(ram_peek(0x0061) - 0x01));
                block_id = 930;
      break;
    case 930:  // $D49F
      /*$D49F*/ CYCLES(0xd49f, 7);
                s_status_c = 0x00;
      /*$D4A0*/ block_id = s_status_d ? 932 : 931;
      break;
    case 931:  // $D4A0
      /*$D4A0*/ tmp6_U16 = s_a;
                tmp3_U16 = ram_peek(0x005e);
                tmp4_U16 = (tmp6_U16 + tmp3_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)tmp3_U16);
                block_id = 933;
      break;
    case 932:  // $D4A0
      /*$D4A0*/ tmp2_U8 = (uint8_t)(adc_dec16(s_a, ram_peek(0x005e), s_status_c) >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 933;
      break;
    case 933:  // $D4A2
      /*$D4A2*/ branchTarget = true; block_id = !s_status_c ? 934 : 935;
      break;
    case 934:  // $D4A2
      /*$D4A2*/ CYCLES_EDGE(0xd4a2, 1);
                branchTarget = true; block_id = 936;
      break;
    case 935:  // $D4A4
      /*$D4A4*/ CYCLES(0xd4a4, 7);
                ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) - 0x01));
      /*$D4A6*/ s_status_c = 0x00;
                block_id = 936;
      break;
    case 936:  // $D4A7
      /*$D4A7*/ CYCLES(0xd4a7, 15);
                tmp2_U8 = s_y;
                tmp5_U8 = peek((ram_peek16al(0x005e) + tmp2_U8));
      /*$D4A9*/ poke((ram_peek16al(0x0060) + tmp2_U8), tmp5_U8);
      /*$D4AB*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_y = tmp2_U8;
      /*$D4AC*/ branchTarget = true; block_id = tmp2_U8 ? 937 : 938;
      break;
    case 937:  // $D4AC
      /*$D4AC*/ CYCLES_EDGE(0xd4ac, 1);
                branchTarget = true; block_id = 936;
      break;
    case 938:  // $D4AE
      /*$D4AE*/ CYCLES(0xd4ae, 14);
                ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) + 0x01));
      /*$D4B0*/ ram_poke(0x0061, (uint8_t)(ram_peek(0x0061) + 0x01));
      /*$D4B2*/ tmp2_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp2_U8;
      /*$D4B3*/ branchTarget = true; block_id = tmp2_U8 ? 939 : 940;
      break;
    case 939:  // $D4B3
      /*$D4B3*/ CYCLES_EDGE(0xd4b3, 1);
                branchTarget = true; block_id = 936;
      break;
    case 940:  // $D4B5
      /*$D4B5*/ CYCLES(0xd4b5, 6);
      /*$D4B8*/ branchTarget = true; block_id = !ram_peek(0x0200) ? 941 : 942;
      break;
    case 941:  // $D4B8
      /*$D4B8*/ CYCLES_EDGE(0xd4b8, 1);
                branchTarget = true; block_id = 952;
      break;
    case 942:  // $D4BA
      /*$D4BA*/ CYCLES(0xd4ba, 32);
      /*$D4BE*/ ram_poke(0x006f, ram_peek(0x0073));
      /*$D4C0*/ ram_poke(0x0070, ram_peek(0x0074));
      /*$D4C2*/ tmp2_U8 = ram_peek(0x0069);
                s_a = tmp2_U8;
      /*$D4C4*/ ram_poke(0x0096, tmp2_U8);
      /*$D4C6*/ block_id = s_status_d ? 944 : 943;
      break;
    case 943:  // $D4C6
      /*$D4C6*/ tmp3_U16 = s_a;
                tmp6_U16 = ram_peek(0x000f);
                tmp4_U16 = (tmp3_U16 + tmp6_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)tmp6_U16);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 945;
      break;
    case 944:  // $D4C6
      /*$D4C6*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x000f), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 945;
      break;
    case 945:  // $D4C8
      /*$D4C8*/ ram_poke(0x0094, s_a);
      /*$D4CA*/ tmp2_U8 = ram_peek(0x006a);
                s_y = tmp2_U8;
      /*$D4CC*/ ram_poke(0x0097, tmp2_U8);
      /*$D4CE*/ branchTarget = true; block_id = !s_status_c ? 946 : 947;
      break;
    case 946:  // $D4CE
      /*$D4CE*/ CYCLES_EDGE(0xd4ce, 1);
                branchTarget = true; block_id = 948;
      break;
    case 947:  // $D4D0
      /*$D4D0*/ CYCLES(0xd4d0, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 948;
      break;
    case 948:  // $D4D1
      /*$D4D1*/ CYCLES(0xd4d1, 9);
                ram_poke(0x0095, s_y);
      /*$D4D3*/ branchTarget = true; push16(0xd4d5); block_id = 829;
      break;
    case 949:  // $D4D6
      /*$D4D6*/ CYCLES(0xd4d6, 29);
      /*$D4DA*/ ram_poke(0x01fe, ram_peek(0x0050));
      /*$D4DD*/ ram_poke(0x01ff, ram_peek(0x0051));
      /*$D4E4*/ ram_poke(0x0069, ram_peek(0x006d));
      /*$D4E6*/ ram_poke(0x006a, ram_peek(0x006e));
      /*$D4E8*/ s_y = ram_peek(0x000f);
                block_id = 950;
      break;
    case 950:  // $D4EA
      /*$D4EA*/ CYCLES(0xd4ea, 14);
                tmp2_U8 = s_y;
      /*$D4ED*/ tmp5_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_y = tmp5_U8;
      /*$D4EE*/ poke((ram_peek16(0x009b) + tmp5_U8), ram_peek((0x01fb + tmp2_U8)));
      /*$D4F0*/ branchTarget = true; block_id = tmp5_U8 ? 951 : 952;
      break;
    case 951:  // $D4F0
      /*$D4F0*/ CYCLES_EDGE(0xd4f0, 1);
                branchTarget = true; block_id = 950;
      break;
    case 952:  // $D4F2
      /*$D4F2*/ CYCLES(0xd4f2, 6);
                branchTarget = true; push16(0xd4f4); block_id = 1076;
      break;
    case 953:  // $D4F5
      /*$D4F5*/ CYCLES(0xd4f5, 14);
      /*$D4F9*/ ram_poke(0x005e, ram_peek(0x0067));
      /*$D4FB*/ ram_poke(0x005f, ram_peek(0x0068));
      /*$D4FD*/ s_status_c = 0x00;
                block_id = 954;
      break;
    case 954:  // $D4FE
      /*$D4FE*/ CYCLES(0xd4fe, 9);
                s_y = 0x01;
      /*$D500*/ tmp5_U8 = peek((ram_peek16al(0x005e) + 0x0001));
      /*$D502*/ branchTarget = true; block_id = tmp5_U8 ? 955 : 956;
      break;
    case 955:  // $D502
      /*$D502*/ CYCLES_EDGE(0xd502, 1);
      /*$D50F*/ CYCLES(0xd50f, 2);
                s_y = 0x04;
                block_id = 957;
      break;
    case 956:  // $D504
      /*$D504*/ CYCLES(0xd504, 15);
      /*$D506*/ ram_poke(0x00af, ram_peek(0x0069));
      /*$D508*/ tmp5_U8 = ram_peek(0x006a);
                s_a = tmp5_U8;
      /*$D50A*/ ram_poke(0x00b0, tmp5_U8);
      /*$D50C*/ branchTarget = true; block_id = 900;
      break;
    case 957:  // $D511
      /*$D511*/ CYCLES(0xd511, 9);
                tmp5_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp5_U8;
      /*$D512*/ tmp5_U8 = peek((ram_peek16al(0x005e) + tmp5_U8));
      /*$D514*/ branchTarget = true; block_id = tmp5_U8 ? 958 : 959;
      break;
    case 958:  // $D514
      /*$D514*/ CYCLES_EDGE(0xd514, 1);
                branchTarget = true; block_id = 957;
      break;
    case 959:  // $D516
      /*$D516*/ CYCLES(0xd516, 38);
      /*$D517*/ s_a = (uint8_t)(s_y + 0x01);
      /*$D518*/ block_id = s_status_d ? 961 : 960;
      break;
    case 960:  // $D518
      /*$D518*/ tmp4_U16 = (s_a + ram_peek(0x005e)) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 962;
      break;
    case 961:  // $D518
      /*$D518*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x005e), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 962;
      break;
    case 962:  // $D51A
      /*$D51A*/ tmp5_U8 = s_a;
                s_x = tmp5_U8;
      /*$D51B*/ s_y = 0x00;
      /*$D51D*/ poke(ram_peek16al(0x005e), tmp5_U8);
      /*$D51F*/ s_a = ram_peek(0x005f);
      /*$D521*/ block_id = s_status_d ? 964 : 963;
      break;
    case 963:  // $D521
      /*$D521*/ tmp4_U16 = s_a;
                tmp6_U16 = tmp4_U16 + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 965;
      break;
    case 964:  // $D521
      /*$D521*/ tmp6_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 965;
      break;
    case 965:  // $D523
      /*$D523*/ tmp5_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp5_U8;
      /*$D524*/ tmp2_U8 = s_a;
                poke((ram_peek16al(0x005e) + tmp5_U8), tmp2_U8);
      /*$D526*/ ram_poke(0x005e, s_x);
      /*$D528*/ ram_poke(0x005f, tmp2_U8);
      /*$D52A*/ branchTarget = true; block_id = !s_status_c ? 966 : 967;
      break;
    case 966:  // $D52A
      /*$D52A*/ CYCLES_EDGE(0xd52a, 1);
                branchTarget = true; block_id = 954;
      break;
    case 967:  // $D52C
      /*$D52C*/ CYCLES(0xd52c, 2);
                s_x = 0x80;
                block_id = 968;
      break;
    case 968:  // $D52E
      /*$D52E*/ CYCLES(0xd52e, 9);
                ram_poke(0x0033, s_x);
      /*$D530*/ branchTarget = true; push16(0xd532); block_id = 2597;
      break;
    case 969:  // $D533
      /*$D533*/ CYCLES(0xd533, 4);
                tmp2_U8 = s_x >= 0xef;
                s_status_c = tmp2_U8;
      /*$D535*/ branchTarget = true; block_id = !tmp2_U8 ? 970 : 971;
      break;
    case 970:  // $D535
      /*$D535*/ CYCLES_EDGE(0xd535, 1);
                branchTarget = true; block_id = 972;
      break;
    case 971:  // $D537
      /*$D537*/ CYCLES(0xd537, 2);
                s_x = 0xef;
                block_id = 972;
      break;
    case 972:  // $D539
      /*$D539*/ CYCLES(0xd539, 11);
      /*$D53B*/ tmp2_U8 = s_x;
                ram_poke((0x0200 + tmp2_U8), 0x00);
      /*$D53F*/ branchTarget = true; block_id = !tmp2_U8 ? 973 : 974;
      break;
    case 973:  // $D53F
      /*$D53F*/ CYCLES_EDGE(0xd53f, 1);
                branchTarget = true; block_id = 976;
      break;
    case 974:  // $D541
      /*$D541*/ CYCLES(0xd541, 15);
                tmp2_U8 = s_x;
      /*$D546*/ ram_poke((0x01ff + tmp2_U8), (ram_peek((0x01ff + tmp2_U8)) & 0x7f));
      /*$D549*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_x = tmp2_U8;
      /*$D54A*/ branchTarget = true; block_id = tmp2_U8 ? 975 : 976;
      break;
    case 975:  // $D54A
      /*$D54A*/ CYCLES_EDGE(0xd54a, 1);
                branchTarget = true; block_id = 974;
      break;
    case 976:  // $D54C
      /*$D54C*/ CYCLES(0xd54c, 12);
                s_a = 0x00;
      /*$D54E*/ s_x = 0xff;
      /*$D550*/ s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_y = 0x01;
      /*$D552*/ branchTarget = true; block_id = find_block_id_func_t001(0xd552, pop16() + 1);;
      break;
    case 977:  // $D553
      /*$D553*/ CYCLES(0xd553, 6);
                branchTarget = true; push16(0xd555); block_id = 2574;
      break;
    case 978:  // $D556
      /*$D556*/ CYCLES(0xd556, 8);
                tmp2_U8 = s_a & 0x7f;
                s_status_not_z = tmp2_U8;
                s_status_n = 0x00;
                s_a = tmp2_U8;
      /*$D558*/ branchTarget = true; block_id = find_block_id_func_t001(0xd558, pop16() + 1);;
      break;
    case 979:  // $D559
      /*$D559*/ CYCLES(0xd559, 15);
      /*$D55B*/ s_x = (uint8_t)(ram_peek(0x00b8) - 0x01);
      /*$D55C*/ s_y = 0x04;
      /*$D55E*/ ram_poke(0x0013, 0x04);
      /*$D560*/ tmp5_U8 = ram_peek(0x00d6);
                s_status_v = ((tmp5_U8 >> 0x06) & 0x01);
      /*$D562*/ branchTarget = true; block_id = !(tmp5_U8 & 0x80) ? 980 : 981;
      break;
    case 980:  // $D562
      /*$D562*/ CYCLES_EDGE(0xd562, 1);
                branchTarget = true; block_id = 983;
      break;
    case 981:  // $D564
      /*$D564*/ CYCLES(0xd564, 14);
                tmp2_U8 = pop8();
      /*$D565*/ tmp2_U8 = pop8();
      /*$D566*/ branchTarget = true; push16(0xd568); block_id = 1076;
      break;
    case 982:  // $D569
      /*$D569*/ CYCLES(0xd569, 3);
                branchTarget = true; block_id = 1088;
      break;
    case 983:  // $D56C
      /*$D56C*/ CYCLES(0xd56c, 2);
                s_x = (uint8_t)(s_x + 0x01);
                block_id = 984;
      break;
    case 984:  // $D56D
      /*$D56D*/ CYCLES(0xd56d, 9);
                s_a = ram_peek((0x0200 + s_x));
      /*$D570*/ tmp2_U8 = (ram_peek(0x0013) >> 0x06) & 0x01;
                s_status_v = tmp2_U8;
      /*$D572*/ branchTarget = true; block_id = tmp2_U8 ? 985 : 986;
      break;
    case 985:  // $D572
      /*$D572*/ CYCLES_EDGE(0xd572, 1);
                branchTarget = true; block_id = 988;
      break;
    case 986:  // $D574
      /*$D574*/ CYCLES(0xd574, 4);
      /*$D576*/ branchTarget = true; block_id = !(s_a != 0x20) ? 987 : 988;
      break;
    case 987:  // $D576
      /*$D576*/ CYCLES_EDGE(0xd576, 1);
                branchTarget = true; block_id = 983;
      break;
    case 988:  // $D578
      /*$D578*/ CYCLES(0xd578, 7);
                tmp2_U8 = s_a;
                ram_poke(0x000e, tmp2_U8);
      /*$D57A*/ s_status_c = (tmp2_U8 >= 0x22);
      /*$D57C*/ branchTarget = true; block_id = !(tmp2_U8 != 0x22) ? 989 : 990;
      break;
    case 989:  // $D57C
      /*$D57C*/ CYCLES_EDGE(0xd57c, 1);
                branchTarget = true; block_id = 1040;
      break;
    case 990:  // $D57E
      /*$D57E*/ CYCLES(0xd57e, 2);
                branchTarget = true; block_id = s_status_v ? 991 : 992;
      break;
    case 991:  // $D57E
      /*$D57E*/ CYCLES_EDGE(0xd57e, 1);
                branchTarget = true; block_id = 1020;
      break;
    case 992:  // $D580
      /*$D580*/ CYCLES(0xd580, 4);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x3f);
      /*$D582*/ branchTarget = true; block_id = (tmp2_U8 != 0x3f) ? 993 : 994;
      break;
    case 993:  // $D582
      /*$D582*/ CYCLES_EDGE(0xd582, 1);
      /*$D588*/ CYCLES(0xd588, 4);
      /*$D58A*/ branchTarget = true; block_id = !(s_a >= 0x30) ? 995 : 996;
      break;
    case 994:  // $D584
      /*$D584*/ CYCLES(0xd584, 4);
                s_a = 0xba;
      /*$D586*/ CYCLES_EDGE(0xd586, 1);
                branchTarget = true; block_id = 1020;
      break;
    case 995:  // $D58A
      /*$D58A*/ CYCLES_EDGE(0xd58a, 1);
                branchTarget = true; block_id = 998;
      break;
    case 996:  // $D58C
      /*$D58C*/ CYCLES(0xd58c, 4);
                tmp2_U8 = s_a >= 0x3c;
                s_status_c = tmp2_U8;
      /*$D58E*/ branchTarget = true; block_id = !tmp2_U8 ? 997 : 998;
      break;
    case 997:  // $D58E
      /*$D58E*/ CYCLES_EDGE(0xd58e, 1);
                branchTarget = true; block_id = 1020;
      break;
    case 998:  // $D590
      /*$D590*/ CYCLES(0xd590, 25);
                ram_poke(0x00ad, s_y);
      /*$D594*/ ram_poke(0x009d, 0xd0);
      /*$D598*/ ram_poke(0x009e, 0xcf);
      /*$D59C*/ ram_poke(0x000f, 0x00);
      /*$D59E*/ s_y = 0xff;
      /*$D59F*/ tmp2_U8 = s_x;
                ram_poke(0x00b8, tmp2_U8);
      /*$D5A1*/ s_x = (uint8_t)(tmp2_U8 - 0x01);
                block_id = 999;
      break;
    case 999:  // $D5A2
      /*$D5A2*/ CYCLES(0xd5a2, 4);
                tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$D5A3*/ branchTarget = true; block_id = tmp2_U8 ? 1000 : 1001;
      break;
    case 1000:  // $D5A3
      /*$D5A3*/ CYCLES_EDGE(0xd5a3, 1);
                branchTarget = true; block_id = 1002;
      break;
    case 1001:  // $D5A5
      /*$D5A5*/ CYCLES(0xd5a5, 5);
                ram_poke(0x009e, (uint8_t)(ram_peek(0x009e) + 0x01));
                block_id = 1002;
      break;
    case 1002:  // $D5A7
      /*$D5A7*/ CYCLES(0xd5a7, 2);
                s_x = (uint8_t)(s_x + 0x01);
                block_id = 1003;
      break;
    case 1003:  // $D5A8
      /*$D5A8*/ CYCLES(0xd5a8, 8);
                tmp2_U8 = ram_peek((0x0200 + s_x));
                s_a = tmp2_U8;
      /*$D5AD*/ branchTarget = true; block_id = !(tmp2_U8 != 0x20) ? 1004 : 1005;
      break;
    case 1004:  // $D5AD
      /*$D5AD*/ CYCLES_EDGE(0xd5ad, 1);
                branchTarget = true; block_id = 1002;
      break;
    case 1005:  // $D5AF
      /*$D5AF*/ CYCLES(0xd5af, 9);
                s_status_c = 0x01;
      /*$D5B0*/ block_id = s_status_d ? 1007 : 1006;
      break;
    case 1006:  // $D5B0
      /*$D5B0*/ tmp6_U16 = s_a;
                tmp2_U8 = peek((ram_peek16(0x009d) + s_y));
                tmp4_U16 = tmp2_U8;
                tmp3_U16 = (tmp6_U16 - tmp4_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)(~tmp4_U16));
                tmp2_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp2_U8;
                s_a = tmp2_U8;
                block_id = 1008;
      break;
    case 1007:  // $D5B0
      /*$D5B0*/ tmp2_U8 = peek((ram_peek16(0x009d) + s_y));
                tmp3_U16 = sbc_dec16(s_a, tmp2_U8, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 1008;
      break;
    case 1008:  // $D5B2
      /*$D5B2*/ branchTarget = true; block_id = !s_status_not_z ? 1009 : 1010;
      break;
    case 1009:  // $D5B2
      /*$D5B2*/ CYCLES_EDGE(0xd5b2, 1);
                branchTarget = true; block_id = 999;
      break;
    case 1010:  // $D5B4
      /*$D5B4*/ CYCLES(0xd5b4, 4);
      /*$D5B6*/ branchTarget = true; block_id = (s_a != 0x80) ? 1011 : 1012;
      break;
    case 1011:  // $D5B6
      /*$D5B6*/ CYCLES_EDGE(0xd5b6, 1);
                branchTarget = true; block_id = 1042;
      break;
    case 1012:  // $D5B8
      /*$D5B8*/ CYCLES(0xd5b8, 7);
                tmp2_U8 = s_a | ram_peek(0x000f);
                s_a = tmp2_U8;
      /*$D5BA*/ s_status_c = (tmp2_U8 >= 0xc5);
      /*$D5BC*/ branchTarget = true; block_id = (tmp2_U8 != 0xc5) ? 1013 : 1014;
      break;
    case 1013:  // $D5BC
      /*$D5BC*/ CYCLES_EDGE(0xd5bc, 1);
                branchTarget = true; block_id = 1019;
      break;
    case 1014:  // $D5BE
      /*$D5BE*/ CYCLES(0xd5be, 8);
                tmp2_U8 = ram_peek((0x0201 + s_x));
                s_a = tmp2_U8;
      /*$D5C3*/ branchTarget = true; block_id = !(tmp2_U8 != 0x4e) ? 1015 : 1016;
      break;
    case 1015:  // $D5C3
      /*$D5C3*/ CYCLES_EDGE(0xd5c3, 1);
                branchTarget = true; block_id = 1042;
      break;
    case 1016:  // $D5C5
      /*$D5C5*/ CYCLES(0xd5c5, 4);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x4f);
      /*$D5C7*/ branchTarget = true; block_id = !(tmp2_U8 != 0x4f) ? 1017 : 1018;
      break;
    case 1017:  // $D5C7
      /*$D5C7*/ CYCLES_EDGE(0xd5c7, 1);
                branchTarget = true; block_id = 1042;
      break;
    case 1018:  // $D5C9
      /*$D5C9*/ CYCLES(0xd5c9, 2);
                s_a = 0xc5;
                block_id = 1019;
      break;
    case 1019:  // $D5CB
      /*$D5CB*/ CYCLES(0xd5cb, 3);
                s_y = ram_peek(0x00ad);
                block_id = 1020;
      break;
    case 1020:  // $D5CD
      /*$D5CD*/ CYCLES(0xd5cd, 15);
                s_x = (uint8_t)(s_x + 0x01);
      /*$D5CE*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$D5CF*/ ram_poke((0x01fb + tmp2_U8), s_a);
      /*$D5D2*/ tmp2_U8 = ram_peek((0x01fb + tmp2_U8));
                s_a = tmp2_U8;
      /*$D5D5*/ branchTarget = true; block_id = !tmp2_U8 ? 1021 : 1022;
      break;
    case 1021:  // $D5D5
      /*$D5D5*/ CYCLES_EDGE(0xd5d5, 1);
                branchTarget = true; block_id = 1052;
      break;
    case 1022:  // $D5D7
      /*$D5D7*/ CYCLES(0xd5d7, 6);
                s_status_c = 0x01;
      /*$D5D8*/ block_id = s_status_d ? 1024 : 1023;
      break;
    case 1023:  // $D5D8
      /*$D5D8*/ tmp2_U8 = (uint8_t)((s_a - 0x003a) - (uint8_t)(0x01 - s_status_c));
                s_status_not_z = tmp2_U8;
                s_a = tmp2_U8;
                block_id = 1025;
      break;
    case 1024:  // $D5D8
      /*$D5D8*/ tmp3_U16 = sbc_dec16(s_a, 0x3a, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_not_z = (~(uint8_t)(tmp3_U16 >> 8) & 2);
                block_id = 1025;
      break;
    case 1025:  // $D5DA
      /*$D5DA*/ branchTarget = true; block_id = !s_status_not_z ? 1026 : 1027;
      break;
    case 1026:  // $D5DA
      /*$D5DA*/ CYCLES_EDGE(0xd5da, 1);
                branchTarget = true; block_id = 1029;
      break;
    case 1027:  // $D5DC
      /*$D5DC*/ CYCLES(0xd5dc, 4);
      /*$D5DE*/ branchTarget = true; block_id = (s_a != 0x49) ? 1028 : 1029;
      break;
    case 1028:  // $D5DE
      /*$D5DE*/ CYCLES_EDGE(0xd5de, 1);
                branchTarget = true; block_id = 1030;
      break;
    case 1029:  // $D5E0
      /*$D5E0*/ CYCLES(0xd5e0, 3);
                ram_poke(0x0013, s_a);
                block_id = 1030;
      break;
    case 1030:  // $D5E2
      /*$D5E2*/ CYCLES(0xd5e2, 6);
                s_status_c = 0x01;
      /*$D5E3*/ block_id = s_status_d ? 1032 : 1031;
      break;
    case 1031:  // $D5E3
      /*$D5E3*/ tmp3_U16 = s_a;
                tmp4_U16 = (tmp3_U16 - 0x0078) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0xff87);
                tmp2_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp2_U8;
                s_a = tmp2_U8;
                block_id = 1033;
      break;
    case 1032:  // $D5E3
      /*$D5E3*/ tmp4_U16 = sbc_dec16(s_a, 0x78, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 1033;
      break;
    case 1033:  // $D5E5
      /*$D5E5*/ branchTarget = true; block_id = s_status_not_z ? 1034 : 1035;
      break;
    case 1034:  // $D5E5
      /*$D5E5*/ CYCLES_EDGE(0xd5e5, 1);
                branchTarget = true; block_id = 984;
      break;
    case 1035:  // $D5E7
      /*$D5E7*/ CYCLES(0xd5e7, 3);
                ram_poke(0x000e, s_a);
                block_id = 1036;
      break;
    case 1036:  // $D5E9
      /*$D5E9*/ CYCLES(0xd5e9, 6);
                tmp2_U8 = ram_peek((0x0200 + s_x));
                s_a = tmp2_U8;
      /*$D5EC*/ branchTarget = true; block_id = !tmp2_U8 ? 1037 : 1038;
      break;
    case 1037:  // $D5EC
      /*$D5EC*/ CYCLES_EDGE(0xd5ec, 1);
                branchTarget = true; block_id = 1020;
      break;
    case 1038:  // $D5EE
      /*$D5EE*/ CYCLES(0xd5ee, 5);
                tmp2_U8 = s_a;
                tmp5_U8 = ram_peek(0x000e);
                s_status_c = (tmp2_U8 >= tmp5_U8);
      /*$D5F0*/ branchTarget = true; block_id = !(tmp2_U8 != tmp5_U8) ? 1039 : 1040;
      break;
    case 1039:  // $D5F0
      /*$D5F0*/ CYCLES_EDGE(0xd5f0, 1);
                branchTarget = true; block_id = 1020;
      break;
    case 1040:  // $D5F2
      /*$D5F2*/ CYCLES(0xd5f2, 11);
                tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$D5F3*/ ram_poke((0x01fb + tmp2_U8), s_a);
      /*$D5F6*/ tmp2_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp2_U8;
      /*$D5F7*/ branchTarget = true; block_id = tmp2_U8 ? 1041 : 1042;
      break;
    case 1041:  // $D5F7
      /*$D5F7*/ CYCLES_EDGE(0xd5f7, 1);
                branchTarget = true; block_id = 1036;
      break;
    case 1042:  // $D5F9
      /*$D5F9*/ CYCLES(0xd5f9, 8);
                s_x = ram_peek(0x00b8);
      /*$D5FB*/ ram_poke(0x000f, (uint8_t)(ram_peek(0x000f) + 0x01));
                block_id = 1043;
      break;
    case 1043:  // $D5FD
      /*$D5FD*/ CYCLES(0xd5fd, 9);
                tmp5_U8 = s_y;
                tmp2_U8 = peek((ram_peek16(0x009d) + tmp5_U8));
                s_a = tmp2_U8;
      /*$D5FF*/ tmp5_U8 = (uint8_t)(tmp5_U8 + 0x01);
                s_y = tmp5_U8;
      /*$D600*/ branchTarget = true; block_id = tmp5_U8 ? 1044 : 1045;
      break;
    case 1044:  // $D600
      /*$D600*/ CYCLES_EDGE(0xd600, 1);
                branchTarget = true; block_id = 1046;
      break;
    case 1045:  // $D602
      /*$D602*/ CYCLES(0xd602, 5);
                ram_poke(0x009e, (uint8_t)(ram_peek(0x009e) + 0x01));
                block_id = 1046;
      break;
    case 1046:  // $D604
      /*$D604*/ CYCLES(0xd604, 4);
                tmp5_U8 = (uint8_t)((s_a << 0x01) >> 8);
                s_status_c = tmp5_U8;
      /*$D605*/ branchTarget = true; block_id = !tmp5_U8 ? 1047 : 1048;
      break;
    case 1047:  // $D605
      /*$D605*/ CYCLES_EDGE(0xd605, 1);
                branchTarget = true; block_id = 1043;
      break;
    case 1048:  // $D607
      /*$D607*/ CYCLES(0xd607, 7);
                tmp5_U8 = peek((ram_peek16(0x009d) + s_y));
      /*$D609*/ branchTarget = true; block_id = tmp5_U8 ? 1049 : 1050;
      break;
    case 1049:  // $D609
      /*$D609*/ CYCLES_EDGE(0xd609, 1);
                branchTarget = true; block_id = 1003;
      break;
    case 1050:  // $D60B
      /*$D60B*/ CYCLES(0xd60b, 6);
                tmp5_U8 = ram_peek((0x0200 + s_x));
                s_a = tmp5_U8;
      /*$D60E*/ branchTarget = true; block_id = !(tmp5_U8 & 0x80) ? 1051 : 1052;
      break;
    case 1051:  // $D60E
      /*$D60E*/ CYCLES_EDGE(0xd60e, 1);
                branchTarget = true; block_id = 1019;
      break;
    case 1052:  // $D610
      /*$D610*/ CYCLES(0xd610, 21);
                ram_poke((0x01fd + s_y), s_a);
      /*$D613*/ ram_poke(0x00b9, (uint8_t)(ram_peek(0x00b9) - 0x01));
      /*$D615*/ s_status_not_z = 0xff;
                s_status_n = 0x80;
                s_a = 0xff;
      /*$D617*/ ram_poke(0x00b8, 0xff);
      /*$D619*/ branchTarget = true; block_id = find_block_id_func_t001(0xd619, pop16() + 1);;
      break;
    case 1053:  // $D61A
      /*$D61A*/ CYCLES(0xd61a, 6);
                s_a = ram_peek(0x0067);
      /*$D61C*/ s_x = ram_peek(0x0068);
                FUNC_FNDLIN2(0x0000);
                block_id = find_block_id_func_t001(0xd61c, pop16() + 1);;
      break;
    case 1054:  // $D626
      /*$D626*/ CYCLES_EDGE(0xd626, 1);
                branchTarget = true; block_id = 1067;
      break;
    case 1055:  // $D628
      /*$D628*/ CYCLES(0xd628, 14);
      /*$D629*/ tmp5_U8 = (uint8_t)(s_y + 0x02);
                s_y = tmp5_U8;
      /*$D62A*/ tmp2_U8 = ram_peek(0x0051);
                s_a = tmp2_U8;
      /*$D62C*/ tmp5_U8 = peek((ram_peek16(0x009b) + tmp5_U8));
                s_status_not_z = (tmp2_U8 != tmp5_U8);
                tmp1_U8 = tmp2_U8 >= tmp5_U8;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp5_U8) & 0x80);
      /*$D62E*/ branchTarget = true; block_id = !tmp1_U8 ? 1056 : 1057;
      break;
    case 1056:  // $D62E
      /*$D62E*/ CYCLES_EDGE(0xd62e, 1);
                branchTarget = true; block_id = 1068;
      break;
    case 1057:  // $D630
      /*$D630*/ CYCLES(0xd630, 2);
                branchTarget = true; block_id = !s_status_not_z ? 1058 : 1059;
      break;
    case 1058:  // $D630
      /*$D630*/ CYCLES_EDGE(0xd630, 1);
                branchTarget = true; block_id = 1061;
      break;
    case 1059:  // $D632
      /*$D632*/ CYCLES(0xd632, 4);
                tmp2_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp2_U8;
      /*$D633*/ branchTarget = true; block_id = tmp2_U8 ? 1060 : 1061;
      break;
    case 1060:  // $D633
      /*$D633*/ CYCLES_EDGE(0xd633, 1);
                branchTarget = true; block_id = 1065;
      break;
    case 1061:  // $D635
      /*$D635*/ CYCLES(0xd635, 12);
                tmp1_U8 = ram_peek(0x0050);
                s_a = tmp1_U8;
      /*$D637*/ tmp5_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp5_U8;
      /*$D638*/ tmp5_U8 = peek((ram_peek16(0x009b) + tmp5_U8));
                s_status_not_z = (tmp1_U8 != tmp5_U8);
                tmp2_U8 = tmp1_U8 >= tmp5_U8;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - tmp5_U8) & 0x80);
      /*$D63A*/ branchTarget = true; block_id = !tmp2_U8 ? 1062 : 1063;
      break;
    case 1062:  // $D63A
      /*$D63A*/ CYCLES_EDGE(0xd63a, 1);
                branchTarget = true; block_id = 1068;
      break;
    case 1063:  // $D63C
      /*$D63C*/ CYCLES(0xd63c, 2);
                branchTarget = true; block_id = !s_status_not_z ? 1064 : 1065;
      break;
    case 1064:  // $D63C
      /*$D63C*/ CYCLES_EDGE(0xd63c, 1);
                branchTarget = true; block_id = 1068;
      break;
    case 1065:  // $D63E
      /*$D63E*/ CYCLES(0xd63e, 18);
                tmp2_U8 = s_y;
      /*$D63F*/ tmp5_U8 = peek((ram_peek16(0x009b) + (uint8_t)(tmp2_U8 - 0x01)));
      /*$D641*/ s_x = tmp5_U8;
      /*$D642*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x02);
                s_y = tmp2_U8;
      /*$D643*/ tmp2_U8 = peek((ram_peek16(0x009b) + tmp2_U8));
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$D645*/ branchTarget = true; block_id = s_status_c ? 1066 : 1067;
      break;
    case 1066:  // $D645
      /*$D645*/ CYCLES_EDGE(0xd645, 1);
      /*$D61E*/ CYCLES(0xd61e, 15);
                s_y = 0x01;
      /*$D620*/ ram_poke(0x009b, s_a);
      /*$D622*/ ram_poke(0x009c, s_x);
      /*$D624*/ tmp2_U8 = peek((ram_peek16(0x009b) + 0x0001));
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$D626*/ branchTarget = true; block_id = !tmp2_U8 ? 1054 : 1055;
      break;
    case 1067:  // $D647
      /*$D647*/ CYCLES(0xd647, 2);
                s_status_c = 0x00;
                block_id = 1068;
      break;
    case 1068:  // $D648
      /*$D648*/ CYCLES(0xd648, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xd648, pop16() + 1);;
      break;
    case 1069:  // $D64B
      /*$D64B*/ CYCLES(0xd64b, 43);
      /*$D64D*/ ram_poke(0x00d6, 0x00);
      /*$D650*/ poke(ram_peek16(0x0067), 0x00);
      /*$D653*/ poke((ram_peek16(0x0067) + 0x0001), 0x00);
      /*$D655*/ s_a = ram_peek(0x0067);
      /*$D657*/ block_id = s_status_d ? 1071 : 1070;
      break;
    case 1070:  // $D657
      /*$D657*/ tmp4_U16 = (s_a + 0x0002) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1072;
      break;
    case 1071:  // $D657
      /*$D657*/ tmp4_U16 = adc_dec16(s_a, 0x02, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1072;
      break;
    case 1072:  // $D659
      /*$D659*/ tmp2_U8 = s_a;
                ram_poke(0x0069, tmp2_U8);
      /*$D65B*/ ram_poke(0x00af, tmp2_U8);
      /*$D65D*/ s_a = ram_peek(0x0068);
      /*$D65F*/ block_id = s_status_d ? 1074 : 1073;
      break;
    case 1073:  // $D65F
      /*$D65F*/ s_a = (uint8_t)(s_a + s_status_c);
                block_id = 1075;
      break;
    case 1074:  // $D65F
      /*$D65F*/ s_a = ((uint8_t)adc_dec16(s_a, 0x00, s_status_c));
                block_id = 1075;
      break;
    case 1075:  // $D661
      /*$D661*/ tmp2_U8 = s_a;
                ram_poke(0x006a, tmp2_U8);
      /*$D663*/ ram_poke(0x00b0, tmp2_U8);
                block_id = 1076;
      break;
    case 1076:  // $D665
      /*$D665*/ CYCLES(0xd665, 6);
                FUNC_CLRTXTPTR(0xd667);
                branchTarget = true; block_id = 1077;
      break;
    case 1077:  // $D668
      /*$D668*/ CYCLES(0xd668, 4);
      /*$D66C*/ CYCLES(0xd66c, 36);
      /*$D670*/ ram_poke(0x006f, ram_peek(0x0073));
      /*$D672*/ ram_poke(0x0070, ram_peek(0x0074));
      /*$D674*/ tmp2_U8 = ram_peek(0x0069);
      /*$D676*/ tmp5_U8 = ram_peek(0x006a);
      /*$D678*/ ram_poke(0x006b, tmp2_U8);
      /*$D67A*/ ram_poke(0x006c, tmp5_U8);
      /*$D67C*/ ram_poke(0x006d, tmp2_U8);
      /*$D67E*/ ram_poke(0x006e, tmp5_U8);
      /*$D680*/ FUNC_RESTORE(0xd682);
                branchTarget = true; block_id = 1079;
      break;
    case 1078:  // $D66A
      /*$D66A*/ CYCLES_EDGE(0xd66a, 1);
                branchTarget = true; block_id = 1080;
      break;
    case 1079:  // $D683
      /*$D683*/ CYCLES(0xd683, 35);
      /*$D685*/ ram_poke(0x0052, 0x55);
      /*$D687*/ tmp5_U8 = pop8();
      /*$D688*/ s_y = tmp5_U8;
      /*$D689*/ tmp2_U8 = pop8();
      /*$D68A*/ s_x = 0xf8;
      /*$D68C*/ s_sp = 0xf8;
      /*$D68D*/ push8(tmp2_U8);
      /*$D68F*/ push8(tmp5_U8);
      /*$D690*/ s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$D692*/ ram_poke(0x007a, 0x00);
      /*$D694*/ ram_poke(0x0014, 0x00);
                block_id = 1080;
      break;
    case 1080:  // $D696
      /*$D696*/ CYCLES(0xd696, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xd696, pop16() + 1);;
      break;
    case 1081:  // $D697
      /*$D697*/ CYCLES(0xd697, 24);
                s_status_c = 0x00;
      /*$D698*/ s_a = ram_peek(0x0067);
      /*$D69A*/ block_id = s_status_d ? 1083 : 1082;
      break;
    case 1082:  // $D69A
      /*$D69A*/ tmp4_U16 = (s_a + 0x00ff) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1084;
      break;
    case 1083:  // $D69A
      /*$D69A*/ tmp4_U16 = adc_dec16(s_a, 0xff, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1084;
      break;
    case 1084:  // $D69C
      /*$D69C*/ ram_poke(0x00b8, s_a);
      /*$D69E*/ s_a = ram_peek(0x0068);
      /*$D6A0*/ block_id = s_status_d ? 1086 : 1085;
      break;
    case 1085:  // $D6A0
      /*$D6A0*/ tmp4_U16 = s_a;
                tmp3_U16 = (tmp4_U16 + 0x00ff) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0x00ff);
                tmp5_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
                block_id = 1087;
      break;
    case 1086:  // $D6A0
      /*$D6A0*/ tmp3_U16 = adc_dec16(s_a, 0xff, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_not_z = (~tmp5_U8 & 2);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                s_status_n = (tmp5_U8 & 0x80);
                block_id = 1087;
      break;
    case 1087:  // $D6A2
      /*$D6A2*/ ram_poke(0x00b9, s_a);
      /*$D6A4*/ branchTarget = true; block_id = find_block_id_func_t001(0xd6a4, pop16() + 1);;
      break;
    case 1088:  // $D7D2
      /*$D7D2*/ CYCLES(0xd7d2, 11);
                tmp2_U8 = s_sp;
                s_x = tmp2_U8;
      /*$D7D3*/ ram_poke(0x00f8, tmp2_U8);
      /*$D7D5*/ branchTarget = true; push16(0xd7d7); block_id = 1133;
      break;
    case 1089:  // $D7D8
      /*$D7D8*/ CYCLES(0xd7d8, 13);
                s_a = ram_peek(0x00b8);
      /*$D7DA*/ s_y = ram_peek(0x00b9);
      /*$D7DE*/ tmp5_U8 = (uint8_t)(ram_peek(0x0076) + 0x01);
                s_x = tmp5_U8;
      /*$D7DF*/ branchTarget = true; block_id = !tmp5_U8 ? 1090 : 1091;
      break;
    case 1090:  // $D7DF
      /*$D7DF*/ CYCLES_EDGE(0xd7df, 1);
                branchTarget = true; block_id = 1092;
      break;
    case 1091:  // $D7E1
      /*$D7E1*/ CYCLES(0xd7e1, 6);
                ram_poke(0x0079, s_a);
      /*$D7E3*/ ram_poke(0x007a, s_y);
                block_id = 1092;
      break;
    case 1092:  // $D7E5
      /*$D7E5*/ CYCLES(0xd7e5, 9);
                s_y = 0x00;
      /*$D7E7*/ tmp2_U8 = peek(ram_peek16al(0x00b8));
                s_a = tmp2_U8;
      /*$D7E9*/ branchTarget = true; block_id = tmp2_U8 ? 1093 : 1094;
      break;
    case 1093:  // $D7E9
      /*$D7E9*/ CYCLES_EDGE(0xd7e9, 1);
      /*$D842*/ CYCLES(0xd842, 4);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x3a);
      /*$D844*/ branchTarget = true; block_id = !(tmp2_U8 != 0x3a) ? 1123 : 1124;
      break;
    case 1094:  // $D7EB
      /*$D7EB*/ CYCLES(0xd7eb, 11);
                s_y = 0x02;
      /*$D7ED*/ tmp2_U8 = peek((ram_peek16al(0x00b8) + 0x0002));
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$D7EF*/ s_status_c = 0x00;
      /*$D7F0*/ branchTarget = true; block_id = !tmp2_U8 ? 1095 : 1096;
      break;
    case 1095:  // $D7F0
      /*$D7F0*/ CYCLES_EDGE(0xd7f0, 1);
      /*$D826*/ CYCLES(0xd826, 2);
                branchTarget = true; block_id = !s_status_not_z ? 1112 : 1113;
      break;
    case 1096:  // $D7F2
      /*$D7F2*/ CYCLES(0xd7f2, 30);
                tmp2_U8 = s_y;
      /*$D7F3*/ tmp5_U8 = peek((ram_peek16al(0x00b8) + (uint8_t)(tmp2_U8 + 0x01)));
      /*$D7F5*/ ram_poke(0x0075, tmp5_U8);
      /*$D7F7*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x02);
                s_y = tmp2_U8;
      /*$D7F8*/ tmp5_U8 = peek((ram_peek16al(0x00b8) + tmp2_U8));
      /*$D7FA*/ ram_poke(0x0076, tmp5_U8);
      /*$D7FC*/ s_a = tmp2_U8;
      /*$D7FD*/ block_id = s_status_d ? 1098 : 1097;
      break;
    case 1097:  // $D7FD
      /*$D7FD*/ tmp3_U16 = (s_a + ram_peek(0x00b8)) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1099;
      break;
    case 1098:  // $D7FD
      /*$D7FD*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x00b8), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 1099;
      break;
    case 1099:  // $D7FF
      /*$D7FF*/ ram_poke(0x00b8, s_a);
      /*$D801*/ branchTarget = true; block_id = !s_status_c ? 1100 : 1101;
      break;
    case 1100:  // $D801
      /*$D801*/ CYCLES_EDGE(0xd801, 1);
                branchTarget = true; block_id = 1102;
      break;
    case 1101:  // $D803
      /*$D803*/ CYCLES(0xd803, 5);
                ram_poke(0x00b9, (uint8_t)(ram_peek(0x00b9) + 0x01));
                block_id = 1102;
      break;
    case 1102:  // $D805
      /*$D805*/ CYCLES(0xd805, 5);
                tmp5_U8 = ram_peek(0x00f2);
                tmp2_U8 = tmp5_U8 & 0x80;
                s_status_n = tmp2_U8;
                s_status_v = ((tmp5_U8 >> 0x06) & 0x01);
                s_status_not_z = (s_a & tmp5_U8);
      /*$D807*/ branchTarget = true; block_id = !tmp2_U8 ? 1103 : 1104;
      break;
    case 1103:  // $D807
      /*$D807*/ CYCLES_EDGE(0xd807, 1);
                branchTarget = true; block_id = 1109;
      break;
    case 1104:  // $D809
      /*$D809*/ CYCLES(0xd809, 7);
      /*$D80B*/ tmp2_U8 = (uint8_t)(ram_peek(0x0076) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$D80C*/ branchTarget = true; block_id = !tmp2_U8 ? 1105 : 1106;
      break;
    case 1105:  // $D80C
      /*$D80C*/ CYCLES_EDGE(0xd80c, 1);
                branchTarget = true; block_id = 1109;
      break;
    case 1106:  // $D80E
      /*$D80E*/ CYCLES(0xd80e, 8);
                s_a = 0x23;
      /*$D810*/ branchTarget = true; push16(0xd812); block_id = 1253;
      break;
    case 1107:  // $D813
      /*$D813*/ CYCLES(0xd813, 12);
                s_x = ram_peek(0x0075);
      /*$D815*/ tmp2_U8 = ram_peek(0x0076);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$D817*/ branchTarget = true; push16(0xd819); block_id = 2218;
      break;
    case 1108:  // $D81A
      /*$D81A*/ CYCLES(0xd81a, 6);
                branchTarget = true; push16(0xd81c); block_id = 1251;
      break;
    case 1109:  // $D81D
      /*$D81D*/ CYCLES(0xd81d, 6);
                branchTarget = true; push16(0xd81f); block_id = 2;
      break;
    case 1110:  // $D820
      /*$D820*/ CYCLES(0xd820, 6);
                branchTarget = true; push16(0xd822); block_id = 1113;
      break;
    case 1111:  // $D823
      /*$D823*/ CYCLES(0xd823, 3);
                branchTarget = true; block_id = 1088;
      break;
    case 1112:  // $D826
      /*$D826*/ CYCLES_EDGE(0xd826, 1);
                branchTarget = true; block_id = 1147;
      break;
    case 1113:  // $D828
      /*$D828*/ CYCLES(0xd828, 2);
                branchTarget = true; block_id = !s_status_not_z ? 1114 : 1115;
      break;
    case 1114:  // $D828
      /*$D828*/ CYCLES_EDGE(0xd828, 1);
                branchTarget = true; block_id = 1132;
      break;
    case 1115:  // $D82A
      /*$D82A*/ CYCLES(0xd82a, 4);
                block_id = s_status_d ? 1117 : 1116;
      break;
    case 1116:  // $D82A
      /*$D82A*/ tmp3_U16 = s_a;
                tmp4_U16 = (tmp3_U16 - 0x0080) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0xff7f);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1118;
      break;
    case 1117:  // $D82A
      /*$D82A*/ tmp4_U16 = sbc_dec16(s_a, 0x80, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 1118;
      break;
    case 1118:  // $D82C
      /*$D82C*/ branchTarget = true; block_id = !s_status_c ? 1119 : 1120;
      break;
    case 1119:  // $D82C
      /*$D82C*/ CYCLES_EDGE(0xd82c, 1);
      /*$D83F*/ CYCLES(0xd83f, 3);
      /*$DA46*/ CYCLES(0xda46, 6);
                branchTarget = true; push16(0xda48); block_id = 1400;
      break;
    case 1120:  // $D82E
      /*$D82E*/ CYCLES(0xd82e, 4);
                tmp2_U8 = s_a >= 0x40;
                s_status_c = tmp2_U8;
      /*$D830*/ branchTarget = true; block_id = tmp2_U8 ? 1121 : 1122;
      break;
    case 1121:  // $D830
      /*$D830*/ CYCLES_EDGE(0xd830, 1);
                branchTarget = true; block_id = 1124;
      break;
    case 1122:  // $D832
      /*$D832*/ CYCLES(0xd832, 21);
                tmp4_U16 = s_a << 0x01;
      /*$D833*/ s_y = ((uint8_t)tmp4_U16);
      /*$D837*/ push8(ram_peek((0xd001 + (tmp4_U16 & 0x00ff))));
      /*$D83B*/ push8(ram_peek((0xd000 + (tmp4_U16 & 0x00ff))));
      /*$D83C*/ branchTarget = true; block_id = 2;
      break;
    case 1123:  // $D844
      /*$D844*/ CYCLES_EDGE(0xd844, 1);
                branchTarget = true; block_id = 1102;
      break;
    case 1124:  // $D846
      /*$D846*/ CYCLES(0xd846, 3);
                branchTarget = true; block_id = 1378;
      break;
    case 1125:  // $D849
      /*$D849*/ CYCLES(0xd849, 12);
                s_status_c = 0x01;
      /*$D84A*/ s_a = ram_peek(0x0067);
      /*$D84C*/ block_id = s_status_d ? 1127 : 1126;
      break;
    case 1126:  // $D84C
      /*$D84C*/ tmp4_U16 = s_a;
                tmp3_U16 = (tmp4_U16 - 0x0001) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0xfffe);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1128;
      break;
    case 1127:  // $D84C
      /*$D84C*/ tmp3_U16 = sbc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 1128;
      break;
    case 1128:  // $D84E
      /*$D84E*/ tmp2_U8 = ram_peek(0x0068);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_y = tmp2_U8;
      /*$D850*/ branchTarget = true; block_id = s_status_c ? 1129 : 1130;
      break;
    case 1129:  // $D850
      /*$D850*/ CYCLES_EDGE(0xd850, 1);
                branchTarget = true; block_id = 1131;
      break;
    case 1130:  // $D852
      /*$D852*/ CYCLES(0xd852, 2);
                tmp2_U8 = (uint8_t)(s_y - 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_y = tmp2_U8;
                block_id = 1131;
      break;
    case 1131:  // $D853
      /*$D853*/ CYCLES(0xd853, 6);
                ram_poke(0x007d, s_a);
      /*$D855*/ ram_poke(0x007e, s_y);
                block_id = 1132;
      break;
    case 1132:  // $D857
      /*$D857*/ CYCLES(0xd857, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xd857, pop16() + 1);;
      break;
    case 1133:  // $D858
      /*$D858*/ CYCLES(0xd858, 8);
                tmp5_U8 = io_peek(0xc000);
                s_a = tmp5_U8;
      /*$D85B*/ tmp2_U8 = tmp5_U8 != 0x83;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp5_U8 >= 0x83);
                s_status_n = ((uint8_t)(tmp5_U8 - 0x83) & 0x80);
      /*$D85D*/ branchTarget = true; block_id = !tmp2_U8 ? 1134 : 1135;
      break;
    case 1134:  // $D85D
      /*$D85D*/ CYCLES_EDGE(0xd85d, 1);
      /*$D860*/ CYCLES(0xd860, 6);
                branchTarget = true; push16(0xd862); block_id = 977;
      break;
    case 1135:  // $D85F
      /*$D85F*/ CYCLES(0xd85f, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xd85f, pop16() + 1);;
      break;
    case 1136:  // $D863
      /*$D863*/ CYCLES(0xd863, 7);
                s_x = 0xff;
      /*$D865*/ tmp2_U8 = ram_peek(0x00d8);
                s_status_v = ((tmp2_U8 >> 0x06) & 0x01);
      /*$D867*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1137 : 1138;
      break;
    case 1137:  // $D867
      /*$D867*/ CYCLES_EDGE(0xd867, 1);
      /*$D86C*/ CYCLES(0xd86c, 4);
                tmp2_U8 = s_a;
                s_status_not_z = (tmp2_U8 != 0x03);
                tmp5_U8 = tmp2_U8 >= 0x03;
                s_status_c = tmp5_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - 0x03) & 0x80);
      /*$D86E*/ branchTarget = true; block_id = tmp5_U8 ? 1139 : 1140;
      break;
    case 1138:  // $D869
      /*$D869*/ CYCLES(0xd869, 3);
                branchTarget = true; block_id = 2365;
      break;
    case 1139:  // $D86E
      /*$D86E*/ CYCLES_EDGE(0xd86e, 1);
                branchTarget = true; block_id = 1141;
      break;
    case 1140:  // $D870
      /*$D870*/ CYCLES(0xd870, 2);
                s_status_c = 0x00;
                block_id = 1141;
      break;
    case 1141:  // $D871
      /*$D871*/ CYCLES(0xd871, 2);
                branchTarget = true; block_id = s_status_not_z ? 1142 : 1143;
      break;
    case 1142:  // $D871
      /*$D871*/ CYCLES_EDGE(0xd871, 1);
      /*$D8AF*/ CYCLES(0xd8af, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xd8af, pop16() + 1);;
      break;
    case 1143:  // $D873
      /*$D873*/ CYCLES(0xd873, 13);
                s_a = ram_peek(0x00b8);
      /*$D875*/ s_y = ram_peek(0x00b9);
      /*$D879*/ tmp5_U8 = (uint8_t)(ram_peek(0x0076) + 0x01);
                s_x = tmp5_U8;
      /*$D87A*/ branchTarget = true; block_id = !tmp5_U8 ? 1144 : 1145;
      break;
    case 1144:  // $D87A
      /*$D87A*/ CYCLES_EDGE(0xd87a, 1);
                branchTarget = true; block_id = 1146;
      break;
    case 1145:  // $D87C
      /*$D87C*/ CYCLES(0xd87c, 18);
                ram_poke(0x0079, s_a);
      /*$D87E*/ ram_poke(0x007a, s_y);
      /*$D884*/ ram_poke(0x0077, ram_peek(0x0075));
      /*$D886*/ ram_poke(0x0078, ram_peek(0x0076));
                block_id = 1146;
      break;
    case 1146:  // $D888
      /*$D888*/ CYCLES(0xd888, 8);
                tmp5_U8 = pop8();
      /*$D889*/ tmp5_U8 = pop8();
                block_id = 1147;
      break;
    case 1147:  // $D88A
      /*$D88A*/ CYCLES(0xd88a, 6);
                s_a = 0x5d;
      /*$D88C*/ s_y = 0xd3;
      /*$D88E*/ branchTarget = true; block_id = !s_status_c ? 1148 : 1149;
      break;
    case 1148:  // $D88E
      /*$D88E*/ CYCLES_EDGE(0xd88e, 1);
      /*$D893*/ CYCLES(0xd893, 3);
                branchTarget = true; block_id = 900;
      break;
    case 1149:  // $D890
      /*$D890*/ CYCLES(0xd890, 3);
                branchTarget = true; block_id = 896;
      break;
    case 1150:  // $D93E
      /*$D93E*/ CYCLES(0xd93e, 6);
                branchTarget = true; push16(0xd940); block_id = 1192;
      break;
    case 1151:  // $D941
      /*$D941*/ CYCLES(0xd941, 6);
                branchTarget = true; push16(0xd943); block_id = 1172;
      break;
    case 1152:  // $D944
      /*$D944*/ CYCLES(0xd944, 8);
      /*$D948*/ branchTarget = true; block_id = (ram_peek(0x0076) >= ram_peek(0x0051)) ? 1153 : 1154;
      break;
    case 1153:  // $D948
      /*$D948*/ CYCLES_EDGE(0xd948, 1);
                branchTarget = true; block_id = 1161;
      break;
    case 1154:  // $D94A
      /*$D94A*/ CYCLES(0xd94a, 12);
                s_a = s_y;
      /*$D94B*/ s_status_c = 0x01;
      /*$D94C*/ block_id = s_status_d ? 1156 : 1155;
      break;
    case 1155:  // $D94C
      /*$D94C*/ tmp3_U16 = (s_a + ram_peek(0x00b8)) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1157;
      break;
    case 1156:  // $D94C
      /*$D94C*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x00b8), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 1157;
      break;
    case 1157:  // $D94E
      /*$D94E*/ s_x = ram_peek(0x00b9);
      /*$D950*/ branchTarget = true; block_id = !s_status_c ? 1158 : 1159;
      break;
    case 1158:  // $D950
      /*$D950*/ CYCLES_EDGE(0xd950, 1);
                branchTarget = true; block_id = 1162;
      break;
    case 1159:  // $D952
      /*$D952*/ CYCLES(0xd952, 4);
                s_x = (uint8_t)(s_x + 0x01);
      /*$D953*/ branchTarget = true; block_id = s_status_c ? 1160 : 1161;
      break;
    case 1160:  // $D953
      /*$D953*/ CYCLES_EDGE(0xd953, 1);
                branchTarget = true; block_id = 1162;
      break;
    case 1161:  // $D955
      /*$D955*/ CYCLES(0xd955, 6);
                s_a = ram_peek(0x0067);
      /*$D957*/ s_x = ram_peek(0x0068);
                block_id = 1162;
      break;
    case 1162:  // $D959
      /*$D959*/ CYCLES(0xd959, 6);
                FUNC_FNDLIN2(0xd95b);
      /*$D95C*/ CYCLES(0xd95c, 2);
                branchTarget = true; block_id = !s_status_c ? 1163 : 1164;
      break;
    case 1163:  // $D95C
      /*$D95C*/ CYCLES_EDGE(0xd95c, 1);
      /*$D97C*/ CYCLES(0xd97c, 5);
                s_x = 0x5a;
      /*$D97E*/ branchTarget = true; block_id = 887;
      break;
    case 1164:  // $D95E
      /*$D95E*/ CYCLES(0xd95e, 22);
                s_a = ram_peek(0x009b);
      /*$D960*/ block_id = s_status_d ? 1166 : 1165;
      break;
    case 1165:  // $D960
      /*$D960*/ tmp3_U16 = (s_a - 0x0001) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1167;
      break;
    case 1166:  // $D960
      /*$D960*/ tmp3_U16 = sbc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 1167;
      break;
    case 1167:  // $D962
      /*$D962*/ ram_poke(0x00b8, s_a);
      /*$D964*/ s_a = ram_peek(0x009c);
      /*$D966*/ block_id = s_status_d ? 1169 : 1168;
      break;
    case 1168:  // $D966
      /*$D966*/ tmp3_U16 = s_a;
                tmp4_U16 = tmp3_U16 - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0xffff);
                tmp5_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
                block_id = 1170;
      break;
    case 1169:  // $D966
      /*$D966*/ tmp4_U16 = sbc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_not_z = (~tmp5_U8 & 2);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                s_status_n = (tmp5_U8 & 0x80);
                block_id = 1170;
      break;
    case 1170:  // $D968
      /*$D968*/ ram_poke(0x00b9, s_a);
      /*$D96A*/ branchTarget = true; block_id = find_block_id_func_t001(0xd96a, pop16() + 1);;
      break;
    case 1171:  // $D9A2
      /*$D9A2*/ CYCLES(0xd9a2, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xd9a2, pop16() + 1);;
      break;
    case 1172:  // $D9A6
      /*$D9A6*/ CYCLES(0xd9a6, 10);
      /*$D9A8*/ ram_poke(0x000d, 0x00);
      /*$D9AA*/ s_y = 0x00;
      /*$D9AC*/ ram_poke(0x000e, 0x00);
                block_id = 1173;
      break;
    case 1173:  // $D9AE
      /*$D9AE*/ CYCLES(0xd9ae, 12);
      /*$D9B0*/ tmp5_U8 = ram_peek(0x000d);
                s_x = tmp5_U8;
      /*$D9B2*/ ram_poke(0x000d, ram_peek(0x000e));
      /*$D9B4*/ ram_poke(0x000e, tmp5_U8);
                block_id = 1174;
      break;
    case 1174:  // $D9B6
      /*$D9B6*/ CYCLES(0xd9b6, 7);
                tmp5_U8 = peek((ram_peek16al(0x00b8) + s_y));
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$D9B8*/ branchTarget = true; block_id = !tmp5_U8 ? 1175 : 1176;
      break;
    case 1175:  // $D9B8
      /*$D9B8*/ CYCLES_EDGE(0xd9b8, 1);
                branchTarget = true; block_id = 1171;
      break;
    case 1176:  // $D9BA
      /*$D9BA*/ CYCLES(0xd9ba, 5);
                tmp5_U8 = s_a;
                tmp2_U8 = ram_peek(0x000e);
                tmp1_U8 = tmp5_U8 != tmp2_U8;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp5_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp5_U8 - tmp2_U8) & 0x80);
      /*$D9BC*/ branchTarget = true; block_id = !tmp1_U8 ? 1177 : 1178;
      break;
    case 1177:  // $D9BC
      /*$D9BC*/ CYCLES_EDGE(0xd9bc, 1);
                branchTarget = true; block_id = 1171;
      break;
    case 1178:  // $D9BE
      /*$D9BE*/ CYCLES(0xd9be, 6);
                s_y = (uint8_t)(s_y + 0x01);
      /*$D9BF*/ tmp1_U8 = s_a;
                tmp2_U8 = tmp1_U8 != 0x22;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp1_U8 >= 0x22);
      /*$D9C1*/ branchTarget = true; block_id = tmp2_U8 ? 1179 : 1180;
      break;
    case 1179:  // $D9C1
      /*$D9C1*/ CYCLES_EDGE(0xd9c1, 1);
                branchTarget = true; block_id = 1174;
      break;
    case 1180:  // $D9C3
      /*$D9C3*/ CYCLES(0xd9c3, 2);
                branchTarget = true; block_id = !s_status_not_z ? 1181 : 1182;
      break;
    case 1181:  // $D9C3
      /*$D9C3*/ CYCLES_EDGE(0xd9c3, 1);
                branchTarget = true; block_id = 1173;
      break;
    case 1182:  // $D9C5
      /*$D9C5*/ CYCLES(0xd9c5, 18);
                tmp2_U8 = pop8();
      /*$D9C6*/ tmp2_U8 = pop8();
      /*$D9C7*/ tmp2_U8 = pop8();
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$D9C8*/ branchTarget = true; block_id = find_block_id_func_t001(0xd9c8, pop16() + 1);;
      break;
    case 1183:  // $D9F6
      /*$D9F6*/ CYCLES_EDGE(0xd9f6, 1);
      /*$D981*/ CYCLES(0xd981, 3);
                branchTarget = true; block_id = 1378;
      break;
    case 1184:  // $D9F8
      /*$D9F8*/ CYCLES(0xd9f8, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x00a1) - 0x01);
                ram_poke(0x00a1, tmp2_U8);
      /*$D9FA*/ branchTarget = true; block_id = tmp2_U8 ? 1185 : 1186;
      break;
    case 1185:  // $D9FA
      /*$D9FA*/ CYCLES_EDGE(0xd9fa, 1);
      /*$DA00*/ CYCLES(0xda00, 6);
                branchTarget = true; push16(0xda02); block_id = 2;
      break;
    case 1186:  // $D9FC
      /*$D9FC*/ CYCLES(0xd9fc, 7);
                tmp2_U8 = pop8();
                s_a = tmp2_U8;
      /*$D9FD*/ branchTarget = true; block_id = 1115;
      break;
    case 1187:  // $DA03
      /*$DA03*/ CYCLES(0xda03, 6);
                branchTarget = true; push16(0xda05); block_id = 1192;
      break;
    case 1188:  // $DA06
      /*$DA06*/ CYCLES(0xda06, 4);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x2c);
      /*$DA08*/ branchTarget = true; block_id = !(tmp2_U8 != 0x2c) ? 1189 : 1190;
      break;
    case 1189:  // $DA08
      /*$DA08*/ CYCLES_EDGE(0xda08, 1);
                branchTarget = true; block_id = 1184;
      break;
    case 1190:  // $DA0A
      /*$DA0A*/ CYCLES(0xda0a, 4);
                tmp2_U8 = pop8();
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 1191;
      break;
    case 1191:  // $DA0B
      /*$DA0B*/ CYCLES(0xda0b, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xda0b, pop16() + 1);;
      break;
    case 1192:  // $DA0C
      /*$DA0C*/ CYCLES(0xda0c, 8);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_x = 0x00;
      /*$DA0E*/ ram_poke(0x0050, 0x00);
      /*$DA10*/ ram_poke(0x0051, 0x00);
                block_id = 1193;
      break;
    case 1193:  // $DA12
      /*$DA12*/ CYCLES(0xda12, 2);
                branchTarget = true; block_id = s_status_c ? 1194 : 1195;
      break;
    case 1194:  // $DA12
      /*$DA12*/ CYCLES_EDGE(0xda12, 1);
                branchTarget = true; block_id = 1191;
      break;
    case 1195:  // $DA14
      /*$DA14*/ CYCLES(0xda14, 15);
                block_id = s_status_d ? 1197 : 1196;
      break;
    case 1196:  // $DA14
      /*$DA14*/ tmp4_U16 = s_a;
                tmp3_U16 = (tmp4_U16 - 0x002f) - (uint8_t)(0x01 - s_status_c);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0xffd0);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1198;
      break;
    case 1197:  // $DA14
      /*$DA14*/ tmp3_U16 = sbc_dec16(s_a, 0x2f, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
                block_id = 1198;
      break;
    case 1198:  // $DA16
      /*$DA16*/ ram_poke(0x000d, s_a);
      /*$DA18*/ tmp2_U8 = ram_peek(0x0051);
                s_a = tmp2_U8;
      /*$DA1A*/ ram_poke(0x005e, tmp2_U8);
      /*$DA1E*/ branchTarget = true; block_id = (tmp2_U8 >= 0x19) ? 1199 : 1200;
      break;
    case 1199:  // $DA1E
      /*$DA1E*/ CYCLES_EDGE(0xda1e, 1);
      /*$D9F4*/ CYCLES(0xd9f4, 4);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0xab);
      /*$D9F6*/ branchTarget = true; block_id = (tmp2_U8 != 0xab) ? 1183 : 1184;
      break;
    case 1200:  // $DA20
      /*$DA20*/ CYCLES(0xda20, 53);
      /*$DA22*/ tmp3_U16 = ram_peek(0x0050);
      /*$DA23*/ ram_poke(0x005e, (uint8_t)((ram_peek(0x005e) << 0x01) | (uint8_t)((tmp3_U16 << 0x01) >> 8)));
      /*$DA25*/ tmp3_U16 = tmp3_U16 << 0x02;
                s_a = ((uint8_t)tmp3_U16);
      /*$DA26*/ tmp3_U16 = (ram_peek(0x005e) << 0x01) | (uint8_t)((tmp3_U16 & 0x01ff) >> 8);
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                ram_poke(0x005e, ((uint8_t)tmp3_U16));
      /*$DA28*/ block_id = s_status_d ? 1202 : 1201;
      break;
    case 1201:  // $DA28
      /*$DA28*/ tmp3_U16 = (s_a + ram_peek(0x0050)) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1203;
      break;
    case 1202:  // $DA28
      /*$DA28*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x0050), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 1203;
      break;
    case 1203:  // $DA2A
      /*$DA2A*/ ram_poke(0x0050, s_a);
      /*$DA2C*/ s_a = ram_peek(0x005e);
      /*$DA2E*/ block_id = s_status_d ? 1205 : 1204;
      break;
    case 1204:  // $DA2E
      /*$DA2E*/ s_a = (uint8_t)((s_a + ram_peek(0x0051)) + s_status_c);
                block_id = 1206;
      break;
    case 1205:  // $DA2E
      /*$DA2E*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x0051), s_status_c));
                block_id = 1206;
      break;
    case 1206:  // $DA30
      /*$DA30*/ ram_poke(0x0051, s_a);
      /*$DA32*/ tmp3_U16 = ram_peek(0x0050) << 0x01;
                ram_poke(0x0050, ((uint8_t)tmp3_U16));
      /*$DA34*/ tmp3_U16 = (ram_peek(0x0051) << 0x01) | (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                ram_poke(0x0051, ((uint8_t)tmp3_U16));
      /*$DA36*/ s_a = ram_peek(0x0050);
      /*$DA38*/ block_id = s_status_d ? 1208 : 1207;
      break;
    case 1207:  // $DA38
      /*$DA38*/ tmp3_U16 = s_a;
                tmp4_U16 = ram_peek(0x000d);
                tmp6_U16 = (tmp3_U16 + tmp4_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
                tmp2_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 1209;
      break;
    case 1208:  // $DA38
      /*$DA38*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x000d), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp2_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 1209;
      break;
    case 1209:  // $DA3A
      /*$DA3A*/ ram_poke(0x0050, s_a);
      /*$DA3C*/ branchTarget = true; block_id = !s_status_c ? 1210 : 1211;
      break;
    case 1210:  // $DA3C
      /*$DA3C*/ CYCLES_EDGE(0xda3c, 1);
                branchTarget = true; block_id = 1212;
      break;
    case 1211:  // $DA3E
      /*$DA3E*/ CYCLES(0xda3e, 5);
                tmp2_U8 = (uint8_t)(ram_peek(0x0051) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x0051, tmp2_U8);
                block_id = 1212;
      break;
    case 1212:  // $DA40
      /*$DA40*/ CYCLES(0xda40, 6);
                branchTarget = true; push16(0xda42); block_id = 2;
      break;
    case 1213:  // $DA43
      /*$DA43*/ CYCLES(0xda43, 3);
                branchTarget = true; block_id = 1193;
      break;
    case 1214:  // $DA49
      /*$DA49*/ CYCLES(0xda49, 14);
                ram_poke(0x0085, s_a);
      /*$DA4B*/ ram_poke(0x0086, s_y);
      /*$DA4D*/ s_a = 0xd0;
      /*$DA4F*/ branchTarget = true; push16(0xda51); block_id = 1375;
      break;
    case 1215:  // $DA52
      /*$DA52*/ CYCLES(0xda52, 18);
      /*$DA54*/ push8(ram_peek(0x0012));
      /*$DA57*/ push8(ram_peek(0x0011));
      /*$DA58*/ branchTarget = true; push16(0xda5a); block_id = 1269;
      break;
    case 1216:  // $DA5B
      /*$DA5B*/ CYCLES(0xda5b, 12);
                tmp1_U8 = pop8();
      /*$DA5C*/ tmp6_U16 = (tmp1_U8 << 0x01) | s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                tmp1_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
      /*$DA5D*/ branchTarget = true; push16(0xda5f); block_id = 1262;
      break;
    case 1217:  // $DA60
      /*$DA60*/ CYCLES(0xda60, 2);
                branchTarget = true; block_id = s_status_not_z ? 1218 : 1219;
      break;
    case 1218:  // $DA60
      /*$DA60*/ CYCLES_EDGE(0xda60, 1);
      /*$DA7A*/ CYCLES(0xda7a, 16);
                tmp2_U8 = pop8();
      /*$DA7B*/ s_y = 0x02;
      /*$DA7D*/ tmp2_U8 = peek((ram_peek16al(0x00a0) + 0x0002));
      /*$DA7F*/ tmp1_U8 = ram_peek(0x0070);
                s_status_not_z = (tmp2_U8 != tmp1_U8);
      /*$DA81*/ branchTarget = true; block_id = !(tmp2_U8 >= tmp1_U8) ? 1224 : 1225;
      break;
    case 1219:  // $DA62
      /*$DA62*/ CYCLES(0xda62, 6);
                tmp2_U8 = pop8();
      /*$DA63*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1220 : 1221;
      break;
    case 1220:  // $DA63
      /*$DA63*/ CYCLES_EDGE(0xda63, 1);
      /*$DA77*/ CYCLES(0xda77, 3);
      /*$EB27*/ CYCLES(0xeb27, 6);
                s_x = ram_peek(0x0085);
      /*$EB29*/ s_y = ram_peek(0x0086);
                block_id = 2070;
      break;
    case 1221:  // $DA65
      /*$DA65*/ CYCLES(0xda65, 6);
                branchTarget = true; push16(0xda67); block_id = 2081;
      break;
    case 1222:  // $DA68
      /*$DA68*/ CYCLES(0xda68, 6);
                branchTarget = true; push16(0xda6a); block_id = 1494;
      break;
    case 1223:  // $DA6B
      /*$DA6B*/ CYCLES(0xda6b, 28);
      /*$DA6F*/ poke(ram_peek16(0x0085), ram_peek(0x00a0));
      /*$DA71*/ s_y = 0x01;
      /*$DA72*/ tmp1_U8 = ram_peek(0x00a1);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$DA74*/ poke((ram_peek16(0x0085) + 0x0001), tmp1_U8);
      /*$DA76*/ branchTarget = true; block_id = find_block_id_func_t001(0xda76, pop16() + 1);;
      break;
    case 1224:  // $DA81
      /*$DA81*/ CYCLES_EDGE(0xda81, 1);
                branchTarget = true; block_id = 1235;
      break;
    case 1225:  // $DA83
      /*$DA83*/ CYCLES(0xda83, 2);
                branchTarget = true; block_id = s_status_not_z ? 1226 : 1227;
      break;
    case 1226:  // $DA83
      /*$DA83*/ CYCLES_EDGE(0xda83, 1);
                branchTarget = true; block_id = 1229;
      break;
    case 1227:  // $DA85
      /*$DA85*/ CYCLES(0xda85, 12);
      /*$DA86*/ tmp2_U8 = peek((ram_peek16al(0x00a0) + (uint8_t)(s_y - 0x01)));
      /*$DA8A*/ branchTarget = true; block_id = !(tmp2_U8 >= ram_peek(0x006f)) ? 1228 : 1229;
      break;
    case 1228:  // $DA8A
      /*$DA8A*/ CYCLES_EDGE(0xda8a, 1);
                branchTarget = true; block_id = 1235;
      break;
    case 1229:  // $DA8C
      /*$DA8C*/ CYCLES(0xda8c, 8);
                tmp1_U8 = ram_peek(0x00a1);
      /*$DA8E*/ tmp2_U8 = ram_peek(0x006a);
                s_status_not_z = (tmp1_U8 != tmp2_U8);
                tmp2_U8 = tmp1_U8 >= tmp2_U8;
                s_status_c = tmp2_U8;
      /*$DA90*/ branchTarget = true; block_id = !tmp2_U8 ? 1230 : 1231;
      break;
    case 1230:  // $DA90
      /*$DA90*/ CYCLES_EDGE(0xda90, 1);
                branchTarget = true; block_id = 1235;
      break;
    case 1231:  // $DA92
      /*$DA92*/ CYCLES(0xda92, 2);
                branchTarget = true; block_id = s_status_not_z ? 1232 : 1233;
      break;
    case 1232:  // $DA92
      /*$DA92*/ CYCLES_EDGE(0xda92, 1);
                branchTarget = true; block_id = 1236;
      break;
    case 1233:  // $DA94
      /*$DA94*/ CYCLES(0xda94, 8);
      /*$DA96*/ tmp2_U8 = ram_peek(0x00a0) >= ram_peek(0x0069);
                s_status_c = tmp2_U8;
      /*$DA98*/ branchTarget = true; block_id = tmp2_U8 ? 1234 : 1235;
      break;
    case 1234:  // $DA98
      /*$DA98*/ CYCLES_EDGE(0xda98, 1);
                branchTarget = true; block_id = 1236;
      break;
    case 1235:  // $DA9A
      /*$DA9A*/ CYCLES(0xda9a, 9);
                s_a = ram_peek(0x00a0);
      /*$DA9C*/ s_y = ram_peek(0x00a1);
      /*$DA9E*/ branchTarget = true; block_id = 1238;
      break;
    case 1236:  // $DAA1
      /*$DAA1*/ CYCLES(0xdaa1, 13);
      /*$DAA3*/ tmp2_U8 = peek(ram_peek16al(0x00a0));
                s_a = tmp2_U8;
      /*$DAA5*/ branchTarget = true; push16(0xdaa7); block_id = 1657;
      break;
    case 1237:  // $DAA8
      /*$DAA8*/ CYCLES(0xdaa8, 18);
      /*$DAAC*/ ram_poke(0x00ab, ram_peek(0x008c));
      /*$DAAE*/ ram_poke(0x00ac, ram_peek(0x008d));
      /*$DAB0*/ FUNC_MOVINS(0xdab2);
      /*$DAB3*/ CYCLES(0xdab3, 4);
                s_a = 0x9d;
      /*$DAB5*/ s_y = 0x00;
                block_id = 1238;
      break;
    case 1238:  // $DAB7
      /*$DAB7*/ CYCLES(0xdab7, 12);
                ram_poke(0x008c, s_a);
      /*$DAB9*/ ram_poke(0x008d, s_y);
      /*$DABB*/ FUNC_FRETMS(0xdabd);
      /*$DABE*/ CYCLES(0xdabe, 45);
      /*$DAC0*/ tmp2_U8 = peek(ram_peek16al(0x008c));
      /*$DAC2*/ poke(ram_peek16(0x0085), tmp2_U8);
      /*$DAC5*/ tmp2_U8 = peek((ram_peek16al(0x008c) + 0x0001));
      /*$DAC7*/ poke((ram_peek16(0x0085) + 0x0001), tmp2_U8);
      /*$DAC9*/ s_y = 0x02;
      /*$DACA*/ tmp2_U8 = peek((ram_peek16al(0x008c) + 0x0002));
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$DACC*/ poke((ram_peek16(0x0085) + 0x0002), tmp2_U8);
      /*$DACE*/ branchTarget = true; block_id = find_block_id_func_t001(0xdace, pop16() + 1);;
      break;
    case 1239:  // $DAFB
      /*$DAFB*/ CYCLES(0xdafb, 8);
                s_a = 0x0d;
      /*$DAFD*/ branchTarget = true; push16(0xdaff); block_id = 1253;
      break;
    case 1240:  // $DB00
      /*$DB00*/ CYCLES(0xdb00, 2);
                tmp5_U8 = s_a ^ 0xff;
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
                block_id = 1242;
      break;
    case 1241:  // $DB00
      /*$DAFD*/ FUNC_NEGATE(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xdafd, pop16() + 1);;
      break;
    case 1242:  // $DB02
      /*$DB02*/ CYCLES(0xdb02, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xdb02, pop16() + 1);;
      break;
    case 1243:  // $DB3A
      /*$DB3A*/ CYCLES(0xdb3a, 6);
                branchTarget = true; push16(0xdb3c); block_id = 1659;
      break;
    case 1244:  // $DB3D
      /*$DB3D*/ CYCLES(0xdb3d, 6);
                FUNC_FREFAC(0xdb3f);
      /*$DB40*/ CYCLES(0xdb40, 6);
      /*$DB41*/ s_y = 0x00;
      /*$DB43*/ s_x = (uint8_t)(s_a + 0x01);
                block_id = 1245;
      break;
    case 1245:  // $DB44
      /*$DB44*/ CYCLES(0xdb44, 4);
                tmp2_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$DB45*/ branchTarget = true; block_id = !tmp2_U8 ? 1246 : 1247;
      break;
    case 1246:  // $DB45
      /*$DB45*/ CYCLES_EDGE(0xdb45, 1);
                branchTarget = true; block_id = 1242;
      break;
    case 1247:  // $DB47
      /*$DB47*/ CYCLES(0xdb47, 11);
                tmp2_U8 = peek((ram_peek16al(0x005e) + s_y));
                s_a = tmp2_U8;
      /*$DB49*/ branchTarget = true; push16(0xdb4b); block_id = 1253;
      break;
    case 1248:  // $DB4C
      /*$DB4C*/ CYCLES(0xdb4c, 6);
                s_y = (uint8_t)(s_y + 0x01);
      /*$DB4D*/ tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x0d);
      /*$DB4F*/ branchTarget = true; block_id = (tmp2_U8 != 0x0d) ? 1249 : 1250;
      break;
    case 1249:  // $DB4F
      /*$DB4F*/ CYCLES_EDGE(0xdb4f, 1);
                branchTarget = true; block_id = 1245;
      break;
    case 1250:  // $DB51
      /*$DB51*/ CYCLES(0xdb51, 6);
                FUNC_NEGATE(0xdb53);
      /*$DB54*/ CYCLES(0xdb54, 3);
                branchTarget = true; block_id = 1245;
      break;
    case 1251:  // $DB57
      /*$DB57*/ CYCLES(0xdb57, 6);
                s_a = 0x20;
      /*$DB59*/ s_status_v = ((ram_peek(0x3fa9) >> 0x06) & 0x01);
                block_id = 1253;
      break;
    case 1252:  // $DB5A
      /*$DB5A*/ CYCLES(0xdb5a, 2);
                s_a = 0x3f;
                block_id = 1253;
      break;
    case 1253:  // $DB5C
      /*$DB5C*/ CYCLES(0xdb5c, 6);
                tmp2_U8 = s_a | 0x80;
                s_a = tmp2_U8;
      /*$DB5E*/ s_status_not_z = (tmp2_U8 != 0xa0);
                tmp5_U8 = tmp2_U8 >= 0xa0;
                s_status_c = tmp5_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - 0xa0) & 0x80);
      /*$DB60*/ branchTarget = true; block_id = !tmp5_U8 ? 1254 : 1255;
      break;
    case 1254:  // $DB60
      /*$DB60*/ CYCLES_EDGE(0xdb60, 1);
                branchTarget = true; block_id = 1256;
      break;
    case 1255:  // $DB62
      /*$DB62*/ CYCLES(0xdb62, 3);
                tmp2_U8 = s_a | ram_peek(0x00f3);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 1256;
      break;
    case 1256:  // $DB64
      /*$DB64*/ CYCLES(0xdb64, 6);
                branchTarget = true; push16(0xdb66); block_id = 2623;
      break;
    case 1257:  // $DB67
      /*$DB67*/ CYCLES(0xdb67, 14);
      /*$DB69*/ push8((s_a & 0x7f));
      /*$DB6A*/ s_a = ram_peek(0x00f1);
      /*$DB6C*/ FUNC_MON_WAIT(0xdb6e);
                branchTarget = true; block_id = 1258;
      break;
    case 1258:  // $DB6F
      /*$DB6F*/ CYCLES(0xdb6f, 10);
                tmp2_U8 = pop8();
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$DB70*/ branchTarget = true; block_id = find_block_id_func_t001(0xdb70, pop16() + 1);;
      break;
    case 1259:  // $DD67
      /*$DD67*/ CYCLES(0xdd67, 6);
                branchTarget = true; push16(0xdd69); block_id = 1269;
      break;
    case 1260:  // $DD6A
      /*$DD6A*/ CYCLES(0xdd6a, 5);
                s_status_c = 0x00;
      /*$DD6B*/ block_id = 1262;
      break;
    case 1261:  // $DD6C
      /*$DD6C*/ CYCLES(0xdd6c, 2);
                s_status_c = 0x01;
                block_id = 1262;
      break;
    case 1262:  // $DD6D
      /*$DD6D*/ CYCLES(0xdd6d, 5);
                tmp1_U8 = ram_peek(0x0011);
                tmp2_U8 = tmp1_U8 & 0x80;
                s_status_n = tmp2_U8;
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
                s_status_not_z = (s_a & tmp1_U8);
      /*$DD6F*/ branchTarget = true; block_id = tmp2_U8 ? 1263 : 1264;
      break;
    case 1263:  // $DD6F
      /*$DD6F*/ CYCLES_EDGE(0xdd6f, 1);
      /*$DD74*/ CYCLES(0xdd74, 2);
                branchTarget = true; block_id = s_status_c ? 1267 : 1268;
      break;
    case 1264:  // $DD71
      /*$DD71*/ CYCLES(0xdd71, 2);
                branchTarget = true; block_id = s_status_c ? 1265 : 1266;
      break;
    case 1265:  // $DD71
      /*$DD71*/ CYCLES_EDGE(0xdd71, 1);
                branchTarget = true; block_id = 1268;
      break;
    case 1266:  // $DD73
      /*$DD73*/ CYCLES(0xdd73, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xdd73, pop16() + 1);;
      break;
    case 1267:  // $DD74
      /*$DD74*/ CYCLES_EDGE(0xdd74, 1);
                branchTarget = true; block_id = 1266;
      break;
    case 1268:  // $DD76
      /*$DD76*/ CYCLES(0xdd76, 5);
                s_x = 0xa3;
      /*$DD78*/ branchTarget = true; block_id = 887;
      break;
    case 1269:  // $DD7B
      /*$DD7B*/ CYCLES(0xdd7b, 5);
      /*$DD7D*/ branchTarget = true; block_id = ram_peek(0x00b8) ? 1270 : 1271;
      break;
    case 1270:  // $DD7D
      /*$DD7D*/ CYCLES_EDGE(0xdd7d, 1);
                branchTarget = true; block_id = 1272;
      break;
    case 1271:  // $DD7F
      /*$DD7F*/ CYCLES(0xdd7f, 5);
                ram_poke(0x00b9, (uint8_t)(ram_peek(0x00b9) - 0x01));
                block_id = 1272;
      break;
    case 1272:  // $DD81
      /*$DD81*/ CYCLES(0xdd81, 23);
                ram_poke(0x00b8, (uint8_t)(ram_peek(0x00b8) - 0x01));
      /*$DD83*/ s_x = 0x00;
      /*$DD85*/ s_status_v = ((ram_peek(0x0048) >> 0x06) & 0x01);
      /*$DD88*/ push8(0x00);
      /*$DD89*/ s_a = 0x01;
      /*$DD8B*/ branchTarget = true; push16(0xdd8d); block_id = 859;
      break;
    case 1273:  // $DD8E
      /*$DD8E*/ CYCLES(0xdd8e, 6);
                branchTarget = true; push16(0xdd90); block_id = 1339;
      break;
    case 1274:  // $DD91
      /*$DD91*/ CYCLES(0xdd91, 5);
                s_a = 0x00;
      /*$DD93*/ ram_poke(0x0089, 0x00);
                block_id = 1275;
      break;
    case 1275:  // $DD95
      /*$DD95*/ CYCLES(0xdd95, 6);
                branchTarget = true; push16(0xdd97); block_id = 5;
      break;
    case 1276:  // $DD98
      /*$DD98*/ CYCLES(0xdd98, 6);
                s_status_c = 0x01;
      /*$DD99*/ block_id = s_status_d ? 1278 : 1277;
      break;
    case 1277:  // $DD99
      /*$DD99*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 - 0x00cf) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0xff30);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1279;
      break;
    case 1278:  // $DD99
      /*$DD99*/ tmp4_U16 = sbc_dec16(s_a, 0xcf, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 1279;
      break;
    case 1279:  // $DD9B
      /*$DD9B*/ branchTarget = true; block_id = !s_status_c ? 1280 : 1281;
      break;
    case 1280:  // $DD9B
      /*$DD9B*/ CYCLES_EDGE(0xdd9b, 1);
                branchTarget = true; block_id = 1287;
      break;
    case 1281:  // $DD9D
      /*$DD9D*/ CYCLES(0xdd9d, 4);
                tmp2_U8 = s_a >= 0x03;
                s_status_c = tmp2_U8;
      /*$DD9F*/ branchTarget = true; block_id = tmp2_U8 ? 1282 : 1283;
      break;
    case 1282:  // $DD9F
      /*$DD9F*/ CYCLES_EDGE(0xdd9f, 1);
                branchTarget = true; block_id = 1287;
      break;
    case 1283:  // $DDA1
      /*$DDA1*/ CYCLES(0xdda1, 14);
                tmp2_U8 = s_a;
      /*$DDA6*/ tmp2_U8 = ((uint8_t)((tmp2_U8 << 0x01) | (tmp2_U8 >= 0x01)) ^ 0x01) ^ ram_peek(0x0089);
                s_a = tmp2_U8;
      /*$DDA8*/ tmp2_U8 = tmp2_U8 >= ram_peek(0x0089);
                s_status_c = tmp2_U8;
      /*$DDAA*/ branchTarget = true; block_id = !tmp2_U8 ? 1284 : 1285;
      break;
    case 1284:  // $DDAA
      /*$DDAA*/ CYCLES_EDGE(0xddaa, 1);
      /*$DE0D*/ CYCLES(0xde0d, 3);
                branchTarget = true; block_id = 1378;
      break;
    case 1285:  // $DDAC
      /*$DDAC*/ CYCLES(0xddac, 9);
                ram_poke(0x0089, s_a);
      /*$DDAE*/ branchTarget = true; push16(0xddb0); block_id = 2;
      break;
    case 1286:  // $DDB1
      /*$DDB1*/ CYCLES(0xddb1, 3);
                branchTarget = true; block_id = 1276;
      break;
    case 1287:  // $DDB4
      /*$DDB4*/ CYCLES(0xddb4, 5);
                tmp2_U8 = ram_peek(0x0089);
                s_x = tmp2_U8;
      /*$DDB6*/ branchTarget = true; block_id = tmp2_U8 ? 1288 : 1289;
      break;
    case 1288:  // $DDB6
      /*$DDB6*/ CYCLES_EDGE(0xddb6, 1);
                branchTarget = true; block_id = 1319;
      break;
    case 1289:  // $DDB8
      /*$DDB8*/ CYCLES(0xddb8, 2);
                branchTarget = true; block_id = s_status_c ? 1290 : 1291;
      break;
    case 1290:  // $DDB8
      /*$DDB8*/ CYCLES_EDGE(0xddb8, 1);
                branchTarget = true; block_id = 1330;
      break;
    case 1291:  // $DDBA
      /*$DDBA*/ CYCLES(0xddba, 4);
                block_id = s_status_d ? 1293 : 1292;
      break;
    case 1292:  // $DDBA
      /*$DDBA*/ tmp4_U16 = s_a;
                tmp6_U16 = (tmp4_U16 + 0x0007) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0x0007);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1294;
      break;
    case 1293:  // $DDBA
      /*$DDBA*/ tmp6_U16 = adc_dec16(s_a, 0x07, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp2_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 1294;
      break;
    case 1294:  // $DDBC
      /*$DDBC*/ branchTarget = true; block_id = !s_status_c ? 1295 : 1296;
      break;
    case 1295:  // $DDBC
      /*$DDBC*/ CYCLES_EDGE(0xddbc, 1);
                branchTarget = true; block_id = 1330;
      break;
    case 1296:  // $DDBE
      /*$DDBE*/ CYCLES(0xddbe, 5);
                block_id = s_status_d ? 1298 : 1297;
      break;
    case 1297:  // $DDBE
      /*$DDBE*/ tmp6_U16 = s_a;
                tmp4_U16 = ram_peek(0x0011);
                tmp3_U16 = (tmp6_U16 + tmp4_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)tmp4_U16);
                tmp2_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp2_U8;
                s_a = tmp2_U8;
                block_id = 1299;
      break;
    case 1298:  // $DDBE
      /*$DDBE*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x0011), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 1299;
      break;
    case 1299:  // $DDC0
      /*$DDC0*/ branchTarget = true; block_id = s_status_not_z ? 1300 : 1301;
      break;
    case 1300:  // $DDC0
      /*$DDC0*/ CYCLES_EDGE(0xddc0, 1);
      /*$DDC5*/ CYCLES(0xddc5, 12);
                block_id = s_status_d ? 1303 : 1302;
      break;
    case 1301:  // $DDC2
      /*$DDC2*/ CYCLES(0xddc2, 3);
      /*$E597*/ CYCLES(0xe597, 18);
      /*$E599*/ push8(ram_peek(0x00a1));
      /*$E59A*/ tmp2_U8 = ram_peek(0x00a0);
                s_a = tmp2_U8;
      /*$E59C*/ push8(tmp2_U8);
      /*$E59D*/ branchTarget = true; push16(0xe59f); block_id = 1339;
      break;
    case 1302:  // $DDC5
      /*$DDC5*/ s_a = (uint8_t)((s_a + 0x00ff) + s_status_c);
                block_id = 1304;
      break;
    case 1303:  // $DDC5
      /*$DDC5*/ s_a = ((uint8_t)adc_dec16(s_a, 0xff, s_status_c));
                block_id = 1304;
      break;
    case 1304:  // $DDC7
      /*$DDC7*/ tmp2_U8 = s_a;
                ram_poke(0x005e, tmp2_U8);
      /*$DDC9*/ tmp3_U16 = tmp2_U8 << 0x01;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
      /*$DDCA*/ block_id = s_status_d ? 1306 : 1305;
      break;
    case 1305:  // $DDCA
      /*$DDCA*/ tmp3_U16 = s_a;
                tmp4_U16 = ram_peek(0x005e);
                tmp6_U16 = (tmp3_U16 + tmp4_U16) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1307;
      break;
    case 1306:  // $DDCA
      /*$DDCA*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x005e), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 1307;
      break;
    case 1307:  // $DDCC
      /*$DDCC*/ s_y = s_a;
                block_id = 1308;
      break;
    case 1308:  // $DDCD
      /*$DDCD*/ CYCLES(0xddcd, 10);
                tmp2_U8 = pop8();
                s_a = tmp2_U8;
      /*$DDCE*/ tmp2_U8 = tmp2_U8 >= ram_peek((0xd0b2 + s_y));
                s_status_c = tmp2_U8;
      /*$DDD1*/ branchTarget = true; block_id = tmp2_U8 ? 1309 : 1310;
      break;
    case 1309:  // $DDD1
      /*$DDD1*/ CYCLES_EDGE(0xddd1, 1);
                branchTarget = true; block_id = 1333;
      break;
    case 1310:  // $DDD3
      /*$DDD3*/ CYCLES(0xddd3, 6);
                branchTarget = true; push16(0xddd5); block_id = 1260;
      break;
    case 1311:  // $DDD6
      /*$DDD6*/ CYCLES(0xddd6, 3);
                push8(s_a);
                block_id = 1312;
      break;
    case 1312:  // $DDD7
      /*$DDD7*/ CYCLES(0xddd7, 6);
                branchTarget = true; push16(0xddd9); block_id = 1326;
      break;
    case 1313:  // $DDDA
      /*$DDDA*/ CYCLES(0xddda, 9);
                tmp2_U8 = pop8();
                s_a = tmp2_U8;
      /*$DDDB*/ tmp2_U8 = ram_peek(0x0087);
                s_y = tmp2_U8;
      /*$DDDD*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1314 : 1315;
      break;
    case 1314:  // $DDDD
      /*$DDDD*/ CYCLES_EDGE(0xdddd, 1);
      /*$DDF6*/ CYCLES(0xddf6, 6);
                tmp2_U8 = s_a >= ram_peek((0xd0b2 + s_y));
                s_status_c = tmp2_U8;
      /*$DDF9*/ branchTarget = true; block_id = tmp2_U8 ? 1323 : 1324;
      break;
    case 1315:  // $DDDF
      /*$DDDF*/ CYCLES(0xdddf, 4);
                tmp2_U8 = s_a;
                s_status_not_z = tmp2_U8;
                s_x = tmp2_U8;
      /*$DDE0*/ branchTarget = true; block_id = !tmp2_U8 ? 1316 : 1317;
      break;
    case 1316:  // $DDE0
      /*$DDE0*/ CYCLES_EDGE(0xdde0, 1);
                branchTarget = true; block_id = 1331;
      break;
    case 1317:  // $DDE2
      /*$DDE2*/ CYCLES(0xdde2, 2);
                branchTarget = true; block_id = s_status_not_z ? 1318 : 1319;
      break;
    case 1318:  // $DDE2
      /*$DDE2*/ CYCLES_EDGE(0xdde2, 1);
                branchTarget = true; block_id = 1337;
      break;
    case 1319:  // $DDE4
      /*$DDE4*/ CYCLES(0xdde4, 14);
                tmp2_U8 = ram_peek(0x0011);
                ram_poke(0x0011, (tmp2_U8 >> 0x01));
      /*$DDE7*/ s_a = (uint8_t)((s_x << 0x01) | (tmp2_U8 & 0x01));
      /*$DDE8*/ tmp2_U8 = ram_peek(0x00b8);
                s_x = tmp2_U8;
      /*$DDEA*/ branchTarget = true; block_id = tmp2_U8 ? 1320 : 1321;
      break;
    case 1320:  // $DDEA
      /*$DDEA*/ CYCLES_EDGE(0xddea, 1);
                branchTarget = true; block_id = 1322;
      break;
    case 1321:  // $DDEC
      /*$DDEC*/ CYCLES(0xddec, 5);
                ram_poke(0x00b9, (uint8_t)(ram_peek(0x00b9) - 0x01));
                block_id = 1322;
      break;
    case 1322:  // $DDEE
      /*$DDEE*/ CYCLES(0xddee, 12);
                ram_poke(0x00b8, (uint8_t)(ram_peek(0x00b8) - 0x01));
      /*$DDF0*/ s_y = 0x1b;
      /*$DDF2*/ ram_poke(0x0089, s_a);
      /*$DDF4*/ CYCLES_EDGE(0xddf4, 1);
                branchTarget = true; block_id = 1308;
      break;
    case 1323:  // $DDF9
      /*$DDF9*/ CYCLES_EDGE(0xddf9, 1);
                branchTarget = true; block_id = 1337;
      break;
    case 1324:  // $DDFB
      /*$DDFB*/ CYCLES(0xddfb, 2);
                branchTarget = true; block_id = !s_status_c ? 1325 : 1326;
      break;
    case 1325:  // $DDFB
      /*$DDFB*/ CYCLES_EDGE(0xddfb, 1);
                branchTarget = true; block_id = 1311;
      break;
    case 1326:  // $DDFD
      /*$DDFD*/ CYCLES(0xddfd, 20);
                tmp2_U8 = s_y;
      /*$DE00*/ push8(ram_peek((0xd0b4 + tmp2_U8)));
      /*$DE04*/ push8(ram_peek((0xd0b3 + tmp2_U8)));
      /*$DE05*/ branchTarget = true; push16(0xde07); block_id = 1328;
      break;
    case 1327:  // $DE08
      /*$DE08*/ CYCLES(0xde08, 6);
      /*$DD86*/ CYCLES(0xdd86, 16);
                push8(ram_peek(0x0089));
      /*$DD88*/ push8(s_x);
      /*$DD89*/ s_a = 0x01;
      /*$DD8B*/ branchTarget = true; push16(0xdd8d); block_id = 859;
      break;
    case 1328:  // $DE10
      /*$DE10*/ CYCLES(0xde10, 39);
                tmp2_U8 = ram_peek(0x00a2);
      /*$DE12*/ s_x = ram_peek((0xd0b2 + s_y));
      /*$DE15*/ s_y = tmp2_U8;
      /*$DE16*/ tmp5_U8 = pop8();
      /*$DE17*/ ram_poke(0x005e, tmp5_U8);
      /*$DE19*/ ram_poke(0x005e, (uint8_t)(ram_peek(0x005e) + 0x01));
      /*$DE1B*/ tmp5_U8 = pop8();
      /*$DE1C*/ ram_poke(0x005f, tmp5_U8);
      /*$DE1F*/ push8(tmp2_U8);
      /*$DE20*/ branchTarget = true; push16(0xde22); block_id = 2081;
      break;
    case 1329:  // $DE23
      /*$DE23*/ CYCLES(0xde23, 35);
      /*$DE25*/ push8(ram_peek(0x00a1));
      /*$DE28*/ push8(ram_peek(0x00a0));
      /*$DE2B*/ push8(ram_peek(0x009f));
      /*$DE2E*/ push8(ram_peek(0x009e));
      /*$DE2F*/ tmp2_U8 = ram_peek(0x009d);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$DE31*/ push8(tmp2_U8);
      /*$DE32*/ branchTarget = true; block_id = find_block_id_func_t001(0xde32, ram_peek16al(0x005e));
      break;
    case 1330:  // $DE35
      /*$DE35*/ CYCLES(0xde35, 6);
                s_y = 0xff;
      /*$DE37*/ tmp2_U8 = pop8();
                s_status_not_z = tmp2_U8;
                s_a = tmp2_U8;
                block_id = 1331;
      break;
    case 1331:  // $DE38
      /*$DE38*/ CYCLES(0xde38, 2);
                branchTarget = true; block_id = !s_status_not_z ? 1332 : 1333;
      break;
    case 1332:  // $DE38
      /*$DE38*/ CYCLES_EDGE(0xde38, 1);
                branchTarget = true; block_id = 1338;
      break;
    case 1333:  // $DE3A
      /*$DE3A*/ CYCLES(0xde3a, 4);
      /*$DE3C*/ branchTarget = true; block_id = !(s_a != 0x64) ? 1334 : 1335;
      break;
    case 1334:  // $DE3C
      /*$DE3C*/ CYCLES_EDGE(0xde3c, 1);
                branchTarget = true; block_id = 1336;
      break;
    case 1335:  // $DE3E
      /*$DE3E*/ CYCLES(0xde3e, 6);
                branchTarget = true; push16(0xde40); block_id = 1260;
      break;
    case 1336:  // $DE41
      /*$DE41*/ CYCLES(0xde41, 3);
                ram_poke(0x0087, s_y);
                block_id = 1337;
      break;
    case 1337:  // $DE43
      /*$DE43*/ CYCLES(0xde43, 57);
                tmp2_U8 = pop8();
      /*$DE44*/ s_status_c = (tmp2_U8 & 0x01);
      /*$DE45*/ ram_poke(0x0016, (tmp2_U8 >> 0x01));
      /*$DE47*/ tmp2_U8 = pop8();
      /*$DE48*/ ram_poke(0x00a5, tmp2_U8);
      /*$DE4A*/ tmp2_U8 = pop8();
      /*$DE4B*/ ram_poke(0x00a6, tmp2_U8);
      /*$DE4D*/ tmp2_U8 = pop8();
      /*$DE4E*/ ram_poke(0x00a7, tmp2_U8);
      /*$DE50*/ tmp2_U8 = pop8();
      /*$DE51*/ ram_poke(0x00a8, tmp2_U8);
      /*$DE53*/ tmp2_U8 = pop8();
      /*$DE54*/ ram_poke(0x00a9, tmp2_U8);
      /*$DE56*/ tmp2_U8 = pop8();
      /*$DE57*/ ram_poke(0x00aa, tmp2_U8);
      /*$DE5B*/ ram_poke(0x00ab, (tmp2_U8 ^ ram_peek(0x00a2)));
                block_id = 1338;
      break;
    case 1338:  // $DE5D
      /*$DE5D*/ CYCLES(0xde5d, 9);
                tmp2_U8 = ram_peek(0x009d);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$DE5F*/ branchTarget = true; block_id = find_block_id_func_t001(0xde5f, pop16() + 1);;
      break;
    case 1339:  // $DE60
      /*$DE60*/ CYCLES(0xde60, 5);
                s_a = 0x00;
      /*$DE62*/ ram_poke(0x0011, 0x00);
                block_id = 1340;
      break;
    case 1340:  // $DE64
      /*$DE64*/ CYCLES(0xde64, 6);
                branchTarget = true; push16(0xde66); block_id = 2;
      break;
    case 1341:  // $DE67
      /*$DE67*/ CYCLES(0xde67, 2);
                branchTarget = true; block_id = s_status_c ? 1342 : 1343;
      break;
    case 1342:  // $DE67
      /*$DE67*/ CYCLES_EDGE(0xde67, 1);
      /*$DE6C*/ CYCLES(0xde6c, 6);
                FUNC_ISLETC(0xde6e);
      /*$DE6F*/ CYCLES(0xde6f, 2);
                branchTarget = true; block_id = s_status_c ? 1344 : 1345;
      break;
    case 1343:  // $DE69
      /*$DE69*/ CYCLES(0xde69, 3);
      /*$EC4A*/ CYCLES(0xec4a, 4);
                s_y = 0x00;
      /*$EC4C*/ s_x = 0x0a;
                block_id = 2131;
      break;
    case 1344:  // $DE6F
      /*$DE6F*/ CYCLES_EDGE(0xde6f, 1);
      /*$DED5*/ CYCLES(0xded5, 6);
                branchTarget = true; push16(0xded7); block_id = 1400;
      break;
    case 1345:  // $DE71
      /*$DE71*/ CYCLES(0xde71, 4);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0x2e);
      /*$DE73*/ branchTarget = true; block_id = !(tmp5_U8 != 0x2e) ? 1346 : 1347;
      break;
    case 1346:  // $DE73
      /*$DE73*/ CYCLES_EDGE(0xde73, 1);
                branchTarget = true; block_id = 1343;
      break;
    case 1347:  // $DE75
      /*$DE75*/ CYCLES(0xde75, 4);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0xc9);
      /*$DE77*/ branchTarget = true; block_id = !(tmp5_U8 != 0xc9) ? 1348 : 1349;
      break;
    case 1348:  // $DE77
      /*$DE77*/ CYCLES_EDGE(0xde77, 1);
      /*$DECE*/ CYCLES(0xdece, 2);
                s_y = 0x15;
                block_id = 1379;
      break;
    case 1349:  // $DE79
      /*$DE79*/ CYCLES(0xde79, 4);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0xc8);
      /*$DE7B*/ branchTarget = true; block_id = !(tmp5_U8 != 0xc8) ? 1350 : 1351;
      break;
    case 1350:  // $DE7B
      /*$DE7B*/ CYCLES_EDGE(0xde7b, 1);
                branchTarget = true; block_id = 1340;
      break;
    case 1351:  // $DE7D
      /*$DE7D*/ CYCLES(0xde7d, 4);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0x22);
      /*$DE7F*/ branchTarget = true; block_id = (tmp5_U8 != 0x22) ? 1352 : 1353;
      break;
    case 1352:  // $DE7F
      /*$DE7F*/ CYCLES_EDGE(0xde7f, 1);
      /*$DE90*/ CYCLES(0xde90, 4);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0xc6);
      /*$DE92*/ branchTarget = true; block_id = (tmp5_U8 != 0xc6) ? 1361 : 1362;
      break;
    case 1353:  // $DE81
      /*$DE81*/ CYCLES(0xde81, 10);
                s_a = ram_peek(0x00b8);
      /*$DE83*/ s_y = ram_peek(0x00b9);
      /*$DE85*/ block_id = s_status_d ? 1355 : 1354;
      break;
    case 1354:  // $DE85
      /*$DE85*/ tmp6_U16 = s_a;
                tmp4_U16 = tmp6_U16 + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1356;
      break;
    case 1355:  // $DE85
      /*$DE85*/ tmp4_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1356;
      break;
    case 1356:  // $DE87
      /*$DE87*/ branchTarget = true; block_id = !s_status_c ? 1357 : 1358;
      break;
    case 1357:  // $DE87
      /*$DE87*/ CYCLES_EDGE(0xde87, 1);
                branchTarget = true; block_id = 1359;
      break;
    case 1358:  // $DE89
      /*$DE89*/ CYCLES(0xde89, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 1359;
      break;
    case 1359:  // $DE8A
      /*$DE8A*/ CYCLES(0xde8a, 6);
                branchTarget = true; push16(0xde8c); block_id = 1659;
      break;
    case 1360:  // $DE8D
      /*$DE8D*/ CYCLES(0xde8d, 3);
      /*$E73D*/ CYCLES(0xe73d, 18);
                tmp5_U8 = ram_peek(0x00ad);
                s_x = tmp5_U8;
      /*$E73F*/ tmp2_U8 = ram_peek(0x00ae);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_y = tmp2_U8;
      /*$E741*/ ram_poke(0x00b8, tmp5_U8);
      /*$E743*/ ram_poke(0x00b9, tmp2_U8);
      /*$E745*/ branchTarget = true; block_id = find_block_id_func_t001(0xe745, pop16() + 1);;
      break;
    case 1361:  // $DE92
      /*$DE92*/ CYCLES_EDGE(0xde92, 1);
      /*$DEA4*/ CYCLES(0xdea4, 4);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0xc2);
      /*$DEA6*/ branchTarget = true; block_id = (tmp2_U8 != 0xc2) ? 1366 : 1367;
      break;
    case 1362:  // $DE94
      /*$DE94*/ CYCLES(0xde94, 4);
                s_y = 0x18;
      /*$DE96*/ CYCLES_EDGE(0xde96, 1);
                branchTarget = true; block_id = 1379;
      break;
    case 1363:  // $DE98
      /*$DE98*/ CYCLES(0xde98, 5);
      /*$DE9A*/ branchTarget = true; block_id = ram_peek(0x009d) ? 1364 : 1365;
      break;
    case 1364:  // $DE9A
      /*$DE9A*/ CYCLES_EDGE(0xde9a, 1);
      /*$DE9F*/ CYCLES(0xde9f, 5);
                s_y = 0x00;
      /*$DEA1*/ branchTarget = true; block_id = 1637;
      break;
    case 1365:  // $DE9C
      /*$DE9C*/ CYCLES(0xde9c, 9);
                s_y = 0x01;
      /*$DEA1*/ branchTarget = true; block_id = 1637;
      break;
    case 1366:  // $DEA6
      /*$DEA6*/ CYCLES_EDGE(0xdea6, 1);
      /*$DEAB*/ CYCLES(0xdeab, 4);
                tmp2_U8 = s_a >= 0xd2;
                s_status_c = tmp2_U8;
      /*$DEAD*/ branchTarget = true; block_id = !tmp2_U8 ? 1368 : 1369;
      break;
    case 1367:  // $DEA8
      /*$DEA8*/ CYCLES(0xdea8, 3);
      /*$E354*/ CYCLES(0xe354, 6);
                branchTarget = true; push16(0xe356); block_id = 1641;
      break;
    case 1368:  // $DEAD
      /*$DEAD*/ CYCLES_EDGE(0xdead, 1);
                branchTarget = true; block_id = 1370;
      break;
    case 1369:  // $DEAF
      /*$DEAF*/ CYCLES(0xdeaf, 3);
      /*$DF0C*/ CYCLES(0xdf0c, 4);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0xd7);
      /*$DF0E*/ branchTarget = true; block_id = !(tmp2_U8 != 0xd7) ? 1388 : 1389;
      break;
    case 1370:  // $DEB2
      /*$DEB2*/ CYCLES(0xdeb2, 6);
                branchTarget = true; push16(0xdeb4); block_id = 1373;
      break;
    case 1371:  // $DEB5
      /*$DEB5*/ CYCLES(0xdeb5, 6);
                branchTarget = true; push16(0xdeb7); block_id = 1269;
      break;
    case 1372:  // $DEB8
      /*$DEB8*/ CYCLES(0xdeb8, 10);
                s_a = 0x29;
      /*$DEBD*/ s_status_v = ((ram_peek(0x2ca9) >> 0x06) & 0x01);
                block_id = 1375;
      break;
    case 1373:  // $DEBB
      /*$DEBB*/ CYCLES(0xdebb, 6);
                s_a = 0x28;
      /*$DEBD*/ s_status_v = ((ram_peek(0x2ca9) >> 0x06) & 0x01);
                block_id = 1375;
      break;
    case 1374:  // $DEBE
      /*$DEBE*/ CYCLES(0xdebe, 2);
                s_a = 0x2c;
                block_id = 1375;
      break;
    case 1375:  // $DEC0
      /*$DEC0*/ CYCLES(0xdec0, 9);
                s_y = 0x00;
      /*$DEC2*/ tmp2_U8 = s_a;
                tmp1_U8 = peek(ram_peek16al(0x00b8));
                s_status_c = (tmp2_U8 >= tmp1_U8);
      /*$DEC4*/ branchTarget = true; block_id = (tmp2_U8 != tmp1_U8) ? 1376 : 1377;
      break;
    case 1376:  // $DEC4
      /*$DEC4*/ CYCLES_EDGE(0xdec4, 1);
                branchTarget = true; block_id = 1378;
      break;
    case 1377:  // $DEC6
      /*$DEC6*/ CYCLES(0xdec6, 3);
                branchTarget = true; block_id = 2;
      break;
    case 1378:  // $DEC9
      /*$DEC9*/ CYCLES(0xdec9, 5);
                s_x = 0x10;
      /*$DECB*/ branchTarget = true; block_id = 887;
      break;
    case 1379:  // $DED0
      /*$DED0*/ CYCLES(0xded0, 11);
                tmp2_U8 = pop8();
      /*$DED1*/ tmp2_U8 = pop8();
      /*$DED2*/ branchTarget = true; block_id = 1312;
      break;
    case 1380:  // $DED8
      /*$DED8*/ CYCLES(0xded8, 11);
                ram_poke(0x00a0, s_a);
      /*$DEDA*/ ram_poke(0x00a1, s_y);
      /*$DEDE*/ branchTarget = true; block_id = !ram_peek(0x0011) ? 1381 : 1382;
      break;
    case 1381:  // $DEDE
      /*$DEDE*/ CYCLES_EDGE(0xdede, 1);
      /*$DEE5*/ CYCLES(0xdee5, 5);
                tmp2_U8 = ram_peek(0x0012);
                s_x = tmp2_U8;
      /*$DEE7*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1383 : 1384;
      break;
    case 1382:  // $DEE0
      /*$DEE0*/ CYCLES(0xdee0, 11);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_x = 0x00;
      /*$DEE2*/ ram_poke(0x00ac, 0x00);
      /*$DEE4*/ branchTarget = true; block_id = find_block_id_func_t001(0xdee4, pop16() + 1);;
      break;
    case 1383:  // $DEE7
      /*$DEE7*/ CYCLES_EDGE(0xdee7, 1);
      /*$DEF6*/ CYCLES(0xdef6, 3);
                FUNC_UPAY2FAC(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xdef6, pop16() + 1);;
      break;
    case 1384:  // $DEE9
      /*$DEE9*/ CYCLES(0xdee9, 23);
      /*$DEEB*/ tmp2_U8 = peek(ram_peek16al(0x00a0));
      /*$DEEF*/ tmp5_U8 = peek((ram_peek16al(0x00a0) + 0x0001));
      /*$DEF1*/ s_y = tmp5_U8;
      /*$DEF2*/ s_a = tmp2_U8;
      /*$DEF3*/ branchTarget = true; block_id = 1636;
      break;
    case 1385:  // $DEFC
      /*$DEFC*/ CYCLES(0xdefc, 6);
                branchTarget = true; push16(0xdefe); block_id = 2353;
      break;
    case 1386:  // $DEFF
      /*$DEFF*/ CYCLES(0xdeff, 11);
                s_a = s_x;
      /*$DF00*/ s_y = ram_peek(0x00f0);
      /*$DF02*/ FUNC_SCRN(0xdf04);
      /*$DF05*/ CYCLES(0xdf05, 8);
                s_y = s_a;
      /*$DF06*/ branchTarget = true; push16(0xdf08); block_id = 1637;
      break;
    case 1387:  // $DF09
      /*$DF09*/ CYCLES(0xdf09, 3);
                branchTarget = true; block_id = 1372;
      break;
    case 1388:  // $DF0E
      /*$DF0E*/ CYCLES_EDGE(0xdf0e, 1);
      /*$DEF9*/ CYCLES(0xdef9, 6);
                branchTarget = true; push16(0xdefb); block_id = 2;
      break;
    case 1389:  // $DF10
      /*$DF10*/ CYCLES(0xdf10, 13);
                tmp2_U8 = (uint8_t)(s_a << 0x01);
                s_a = tmp2_U8;
      /*$DF11*/ push8(tmp2_U8);
      /*$DF12*/ s_x = tmp2_U8;
      /*$DF13*/ branchTarget = true; push16(0xdf15); block_id = 2;
      break;
    case 1390:  // $DF16
      /*$DF16*/ CYCLES(0xdf16, 4);
                tmp2_U8 = s_x >= 0xcf;
                s_status_c = tmp2_U8;
      /*$DF18*/ branchTarget = true; block_id = !tmp2_U8 ? 1391 : 1392;
      break;
    case 1391:  // $DF18
      /*$DF18*/ CYCLES_EDGE(0xdf18, 1);
      /*$DF3A*/ CYCLES(0xdf3a, 6);
                branchTarget = true; push16(0xdf3c); block_id = 1370;
      break;
    case 1392:  // $DF1A
      /*$DF1A*/ CYCLES(0xdf1a, 6);
                branchTarget = true; push16(0xdf1c); block_id = 1373;
      break;
    case 1393:  // $DF1D
      /*$DF1D*/ CYCLES(0xdf1d, 6);
                branchTarget = true; push16(0xdf1f); block_id = 1269;
      break;
    case 1394:  // $DF20
      /*$DF20*/ CYCLES(0xdf20, 6);
                branchTarget = true; push16(0xdf22); block_id = 1374;
      break;
    case 1395:  // $DF23
      /*$DF23*/ CYCLES(0xdf23, 6);
                branchTarget = true; push16(0xdf25); block_id = 1261;
      break;
    case 1396:  // $DF26
      /*$DF26*/ CYCLES(0xdf26, 29);
                tmp2_U8 = pop8();
      /*$DF27*/ s_x = tmp2_U8;
      /*$DF2A*/ push8(ram_peek(0x00a1));
      /*$DF2D*/ push8(ram_peek(0x00a0));
      /*$DF2E*/ s_a = tmp2_U8;
      /*$DF2F*/ push8(tmp2_U8);
      /*$DF30*/ branchTarget = true; push16(0xdf32); block_id = 1833;
      break;
    case 1397:  // $DF33
      /*$DF33*/ CYCLES(0xdf33, 14);
                tmp2_U8 = pop8();
      /*$DF34*/ s_y = tmp2_U8;
      /*$DF36*/ push8(s_x);
      /*$DF37*/ branchTarget = true; block_id = 1399;
      break;
    case 1398:  // $DF3D
      /*$DF3D*/ CYCLES(0xdf3d, 6);
                tmp2_U8 = pop8();
      /*$DF3E*/ s_y = tmp2_U8;
                block_id = 1399;
      break;
    case 1399:  // $DF3F
      /*$DF3F*/ CYCLES(0xdf3f, 20);
                tmp2_U8 = s_y;
                tmp5_U8 = peek((0xcfdc + tmp2_U8));
      /*$DF42*/ ram_poke(0x0091, tmp5_U8);
      /*$DF44*/ tmp2_U8 = peek((0xcfdd + tmp2_U8));
                s_a = tmp2_U8;
      /*$DF47*/ ram_poke(0x0092, tmp2_U8);
      /*$DF49*/ FUNC_BCC(0xdf4b);
      /*$DF4C*/ CYCLES(0xdf4c, 3);
                branchTarget = true; block_id = 1260;
      break;
    case 1400:  // $DFE3
      /*$DFE3*/ CYCLES(0xdfe3, 8);
                s_x = 0x00;
      /*$DFE5*/ branchTarget = true; push16(0xdfe7); block_id = 5;
      break;
    case 1401:  // $DFE8
      /*$DFE8*/ CYCLES(0xdfe8, 3);
                ram_poke(0x0010, s_x);
                block_id = 1402;
      break;
    case 1402:  // $DFEA
      /*$DFEA*/ CYCLES(0xdfea, 9);
                ram_poke(0x0081, s_a);
      /*$DFEC*/ branchTarget = true; push16(0xdfee); block_id = 5;
      break;
    case 1403:  // $DFEF
      /*$DFEF*/ CYCLES(0xdfef, 6);
                FUNC_ISLETC(0xdff1);
      /*$DFF2*/ CYCLES(0xdff2, 2);
                branchTarget = true; block_id = s_status_c ? 1404 : 1405;
      break;
    case 1404:  // $DFF2
      /*$DFF2*/ CYCLES_EDGE(0xdff2, 1);
      /*$DFF7*/ CYCLES(0xdff7, 11);
                s_x = 0x00;
      /*$DFF9*/ ram_poke(0x0011, 0x00);
      /*$DFFB*/ ram_poke(0x0012, 0x00);
      /*$E007*/ CYCLES(0xe007, 6);
                branchTarget = true; push16(0xe009); block_id = 2;
      break;
    case 1405:  // $DFF4
      /*$DFF4*/ CYCLES(0xdff4, 3);
                branchTarget = true; block_id = 1378;
      break;
    case 1406:  // $E00A
      /*$E00A*/ CYCLES(0xe00a, 2);
                branchTarget = true; block_id = !s_status_c ? 1407 : 1408;
      break;
    case 1407:  // $E00A
      /*$E00A*/ CYCLES_EDGE(0xe00a, 1);
                branchTarget = true; block_id = 1410;
      break;
    case 1408:  // $E00C
      /*$E00C*/ CYCLES(0xe00c, 6);
                FUNC_ISLETC(0xe00e);
      /*$E00F*/ CYCLES(0xe00f, 2);
                branchTarget = true; block_id = !s_status_c ? 1409 : 1410;
      break;
    case 1409:  // $E00F
      /*$E00F*/ CYCLES_EDGE(0xe00f, 1);
                branchTarget = true; block_id = 1416;
      break;
    case 1410:  // $E011
      /*$E011*/ CYCLES(0xe011, 2);
                s_x = s_a;
                block_id = 1411;
      break;
    case 1411:  // $E012
      /*$E012*/ CYCLES(0xe012, 6);
                branchTarget = true; push16(0xe014); block_id = 2;
      break;
    case 1412:  // $E015
      /*$E015*/ CYCLES(0xe015, 2);
                branchTarget = true; block_id = !s_status_c ? 1413 : 1414;
      break;
    case 1413:  // $E015
      /*$E015*/ CYCLES_EDGE(0xe015, 1);
                branchTarget = true; block_id = 1411;
      break;
    case 1414:  // $E017
      /*$E017*/ CYCLES(0xe017, 6);
                FUNC_ISLETC(0xe019);
      /*$E01A*/ CYCLES(0xe01a, 2);
                branchTarget = true; block_id = s_status_c ? 1415 : 1416;
      break;
    case 1415:  // $E01A
      /*$E01A*/ CYCLES_EDGE(0xe01a, 1);
                branchTarget = true; block_id = 1411;
      break;
    case 1416:  // $E01C
      /*$E01C*/ CYCLES(0xe01c, 4);
      /*$E01E*/ branchTarget = true; block_id = (s_a != 0x24) ? 1417 : 1418;
      break;
    case 1417:  // $E01E
      /*$E01E*/ CYCLES_EDGE(0xe01e, 1);
      /*$E026*/ CYCLES(0xe026, 4);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x25);
      /*$E028*/ branchTarget = true; block_id = (tmp2_U8 != 0x25) ? 1419 : 1420;
      break;
    case 1418:  // $E020
      /*$E020*/ CYCLES(0xe020, 7);
      /*$E022*/ ram_poke(0x0011, 0xff);
      /*$E024*/ CYCLES_EDGE(0xe024, 1);
                branchTarget = true; block_id = 1423;
      break;
    case 1419:  // $E028
      /*$E028*/ CYCLES_EDGE(0xe028, 1);
                branchTarget = true; block_id = 1424;
      break;
    case 1420:  // $E02A
      /*$E02A*/ CYCLES(0xe02a, 5);
      /*$E02C*/ branchTarget = true; block_id = (ram_peek(0x0014) & 0x80) ? 1421 : 1422;
      break;
    case 1421:  // $E02C
      /*$E02C*/ CYCLES_EDGE(0xe02c, 1);
                branchTarget = true; block_id = 1405;
      break;
    case 1422:  // $E02E
      /*$E02E*/ CYCLES(0xe02e, 11);
      /*$E030*/ ram_poke(0x0012, 0x80);
      /*$E034*/ ram_poke(0x0081, (0x80 | ram_peek(0x0081)));
                block_id = 1423;
      break;
    case 1423:  // $E036
      /*$E036*/ CYCLES(0xe036, 12);
      /*$E037*/ tmp2_U8 = s_x | 0x80;
                s_a = tmp2_U8;
      /*$E039*/ s_x = tmp2_U8;
      /*$E03A*/ branchTarget = true; push16(0xe03c); block_id = 2;
      break;
    case 1424:  // $E03D
      /*$E03D*/ CYCLES(0xe03d, 12);
                ram_poke(0x0082, s_x);
      /*$E03F*/ s_status_c = 0x01;
      /*$E040*/ s_a = (s_a | ram_peek(0x0014));
      /*$E042*/ block_id = s_status_d ? 1426 : 1425;
      break;
    case 1425:  // $E042
      /*$E042*/ tmp4_U16 = s_a;
                tmp6_U16 = (tmp4_U16 - 0x0028) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0xffd7);
                s_status_not_z = ((uint8_t)tmp6_U16);
                block_id = 1427;
      break;
    case 1426:  // $E042
      /*$E042*/ tmp2_U8 = (uint8_t)(sbc_dec16(s_a, 0x28, s_status_c) >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 1427;
      break;
    case 1427:  // $E044
      /*$E044*/ branchTarget = true; block_id = s_status_not_z ? 1428 : 1429;
      break;
    case 1428:  // $E044
      /*$E044*/ CYCLES_EDGE(0xe044, 1);
      /*$E049*/ CYCLES(0xe049, 5);
                tmp2_U8 = ram_peek(0x0014);
                s_status_v = ((tmp2_U8 >> 0x06) & 0x01);
      /*$E04B*/ branchTarget = true; block_id = (tmp2_U8 & 0x80) ? 1430 : 1431;
      break;
    case 1429:  // $E046
      /*$E046*/ CYCLES(0xe046, 3);
      /*$E11E*/ CYCLES(0xe11e, 5);
      /*$E120*/ branchTarget = true; block_id = ram_peek(0x0014) ? 1500 : 1501;
      break;
    case 1430:  // $E04B
      /*$E04B*/ CYCLES_EDGE(0xe04b, 1);
                branchTarget = true; block_id = 1433;
      break;
    case 1431:  // $E04D
      /*$E04D*/ CYCLES(0xe04d, 2);
                branchTarget = true; block_id = s_status_v ? 1432 : 1433;
      break;
    case 1432:  // $E04D
      /*$E04D*/ CYCLES_EDGE(0xe04d, 1);
                branchTarget = true; block_id = 1429;
      break;
    case 1433:  // $E04F
      /*$E04F*/ CYCLES(0xe04f, 13);
      /*$E051*/ ram_poke(0x0014, 0x00);
      /*$E053*/ s_a = ram_peek(0x0069);
      /*$E055*/ s_x = ram_peek(0x006a);
      /*$E057*/ s_y = 0x00;
                block_id = 1434;
      break;
    case 1434:  // $E059
      /*$E059*/ CYCLES(0xe059, 3);
                ram_poke(0x009c, s_x);
                block_id = 1435;
      break;
    case 1435:  // $E05B
      /*$E05B*/ CYCLES(0xe05b, 8);
                ram_poke(0x009b, s_a);
      /*$E05F*/ branchTarget = true; block_id = (s_x != ram_peek(0x006c)) ? 1436 : 1437;
      break;
    case 1436:  // $E05F
      /*$E05F*/ CYCLES_EDGE(0xe05f, 1);
                branchTarget = true; block_id = 1439;
      break;
    case 1437:  // $E061
      /*$E061*/ CYCLES(0xe061, 5);
      /*$E063*/ branchTarget = true; block_id = !(s_a != ram_peek(0x006b)) ? 1438 : 1439;
      break;
    case 1438:  // $E063
      /*$E063*/ CYCLES_EDGE(0xe063, 1);
      /*$E087*/ CYCLES(0xe087, 11);
                tmp2_U8 = pop8();
      /*$E088*/ push8(tmp2_U8);
      /*$E08B*/ branchTarget = true; block_id = (tmp2_U8 != 0xd7) ? 1461 : 1462;
      break;
    case 1439:  // $E065
      /*$E065*/ CYCLES(0xe065, 10);
      /*$E067*/ tmp2_U8 = peek((ram_peek16(0x009b) + s_y));
      /*$E069*/ branchTarget = true; block_id = (ram_peek(0x0081) != tmp2_U8) ? 1440 : 1441;
      break;
    case 1440:  // $E069
      /*$E069*/ CYCLES_EDGE(0xe069, 1);
                branchTarget = true; block_id = 1444;
      break;
    case 1441:  // $E06B
      /*$E06B*/ CYCLES(0xe06b, 12);
      /*$E06D*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$E06E*/ tmp2_U8 = peek((ram_peek16(0x009b) + tmp2_U8));
      /*$E070*/ branchTarget = true; block_id = !(ram_peek(0x0082) != tmp2_U8) ? 1442 : 1443;
      break;
    case 1442:  // $E070
      /*$E070*/ CYCLES_EDGE(0xe070, 1);
                branchTarget = true; block_id = 1473;
      break;
    case 1443:  // $E072
      /*$E072*/ CYCLES(0xe072, 2);
                s_y = (uint8_t)(s_y - 0x01);
                block_id = 1444;
      break;
    case 1444:  // $E073
      /*$E073*/ CYCLES(0xe073, 9);
                s_status_c = 0x00;
      /*$E074*/ s_a = ram_peek(0x009b);
      /*$E076*/ block_id = s_status_d ? 1446 : 1445;
      break;
    case 1445:  // $E076
      /*$E076*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 + 0x0007) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x0007);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1447;
      break;
    case 1446:  // $E076
      /*$E076*/ tmp4_U16 = adc_dec16(s_a, 0x07, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 1447;
      break;
    case 1447:  // $E078
      /*$E078*/ branchTarget = true; block_id = !s_status_c ? 1448 : 1449;
      break;
    case 1448:  // $E078
      /*$E078*/ CYCLES_EDGE(0xe078, 1);
                branchTarget = true; block_id = 1435;
      break;
    case 1449:  // $E07A
      /*$E07A*/ CYCLES(0xe07a, 4);
                tmp2_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp2_U8;
      /*$E07B*/ branchTarget = true; block_id = tmp2_U8 ? 1450 : 2639;
      break;
    case 1450:  // $E07B
      /*$E07B*/ CYCLES_EDGE(0xe07b, 1);
                branchTarget = true; block_id = 1434;
      break;
    case 1451:  // $E07D
      /*$E07D*/ CYCLES(0xe07d, 4);
                tmp2_U8 = s_a;
                s_status_not_z = (tmp2_U8 != 0x41);
                tmp5_U8 = tmp2_U8 >= 0x41;
                s_status_c = tmp5_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - 0x41) & 0x80);
      /*$E07F*/ branchTarget = true; block_id = !tmp5_U8 ? 1452 : 1453;
      break;
    case 1452:  // $E07F
      /*$E07F*/ CYCLES_EDGE(0xe07f, 1);
                branchTarget = true; block_id = 1459;
      break;
    case 1453:  // $E081
      /*$E081*/ CYCLES(0xe081, 6);
                block_id = s_status_d ? 1455 : 1454;
      break;
    case 1454:  // $E081
      /*$E081*/ s_a = (uint8_t)((s_a - 0x005b) - (uint8_t)(0x01 - s_status_c));
                block_id = 1456;
      break;
    case 1455:  // $E081
      /*$E081*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x5b, s_status_c));
                block_id = 1456;
      break;
    case 1456:  // $E083
      /*$E083*/ s_status_c = 0x01;
      /*$E084*/ block_id = s_status_d ? 1458 : 1457;
      break;
    case 1457:  // $E084
      /*$E084*/ tmp4_U16 = s_a;
                tmp6_U16 = (tmp4_U16 - 0x00a5) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0xff5a);
                tmp2_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 1460;
      break;
    case 1458:  // $E084
      /*$E084*/ tmp6_U16 = sbc_dec16(s_a, 0xa5, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp2_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 1460;
      break;
    case 1459:  // $E086
      /*$E086*/ CYCLES(0xe086, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xe086, pop16() + 1);;
      break;
    case 1460:  // $E086
      /*$E084*/ block_id = 1459;
      break;
    case 1461:  // $E08B
      /*$E08B*/ CYCLES_EDGE(0xe08b, 1);
                branchTarget = true; block_id = 1465;
      break;
    case 1462:  // $E08D
      /*$E08D*/ CYCLES(0xe08d, 10);
                tmp5_U8 = s_sp;
                s_x = tmp5_U8;
      /*$E08E*/ tmp5_U8 = ram_peek((0x0102 + tmp5_U8));
      /*$E091*/ s_status_c = (tmp5_U8 >= 0xde);
      /*$E093*/ branchTarget = true; block_id = (tmp5_U8 != 0xde) ? 1463 : 1464;
      break;
    case 1463:  // $E093
      /*$E093*/ CYCLES_EDGE(0xe093, 1);
                branchTarget = true; block_id = 1465;
      break;
    case 1464:  // $E095
      /*$E095*/ CYCLES(0xe095, 10);
                s_a = 0x9a;
      /*$E097*/ s_status_not_z = 0xe0;
                s_status_n = 0x80;
                s_y = 0xe0;
      /*$E099*/ branchTarget = true; block_id = find_block_id_func_t001(0xe099, pop16() + 1);;
      break;
    case 1465:  // $E09C
      /*$E09C*/ CYCLES(0xe09c, 30);
      /*$E0A0*/ ram_poke(0x009b, ram_peek(0x006b));
      /*$E0A2*/ ram_poke(0x009c, ram_peek(0x006c));
      /*$E0A4*/ tmp2_U8 = ram_peek(0x006d);
                s_a = tmp2_U8;
      /*$E0A6*/ tmp5_U8 = ram_peek(0x006e);
                s_y = tmp5_U8;
      /*$E0A8*/ ram_poke(0x0096, tmp2_U8);
      /*$E0AA*/ ram_poke(0x0097, tmp5_U8);
      /*$E0AC*/ s_status_c = 0x00;
      /*$E0AD*/ block_id = s_status_d ? 1467 : 1466;
      break;
    case 1466:  // $E0AD
      /*$E0AD*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 + 0x0007) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x0007);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1468;
      break;
    case 1467:  // $E0AD
      /*$E0AD*/ tmp4_U16 = adc_dec16(s_a, 0x07, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1468;
      break;
    case 1468:  // $E0AF
      /*$E0AF*/ branchTarget = true; block_id = !s_status_c ? 1469 : 1470;
      break;
    case 1469:  // $E0AF
      /*$E0AF*/ CYCLES_EDGE(0xe0af, 1);
                branchTarget = true; block_id = 1471;
      break;
    case 1470:  // $E0B1
      /*$E0B1*/ CYCLES(0xe0b1, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 1471;
      break;
    case 1471:  // $E0B2
      /*$E0B2*/ CYCLES(0xe0b2, 12);
                ram_poke(0x0094, s_a);
      /*$E0B4*/ ram_poke(0x0095, s_y);
      /*$E0B6*/ branchTarget = true; push16(0xe0b8); block_id = 829;
      break;
    case 1472:  // $E0B9
      /*$E0B9*/ CYCLES(0xe0b9, 78);
      /*$E0BE*/ ram_poke(0x006b, ram_peek(0x0094));
      /*$E0C0*/ ram_poke(0x006c, (uint8_t)(ram_peek(0x0095) + 0x01));
      /*$E0C6*/ poke(ram_peek16(0x009b), ram_peek(0x0081));
      /*$E0CB*/ poke((ram_peek16(0x009b) + 0x0001), ram_peek(0x0082));
      /*$E0D0*/ poke((ram_peek16(0x009b) + 0x0002), 0x00);
      /*$E0D3*/ poke((ram_peek16(0x009b) + 0x0003), 0x00);
      /*$E0D6*/ poke((ram_peek16(0x009b) + 0x0004), 0x00);
      /*$E0D9*/ poke((ram_peek16(0x009b) + 0x0005), 0x00);
      /*$E0DC*/ poke((ram_peek16(0x009b) + 0x0006), 0x00);
                block_id = 1473;
      break;
    case 1473:  // $E0DE
      /*$E0DE*/ CYCLES(0xe0de, 12);
                s_a = ram_peek(0x009b);
      /*$E0E0*/ s_status_c = 0x00;
      /*$E0E1*/ block_id = s_status_d ? 1475 : 1474;
      break;
    case 1474:  // $E0E1
      /*$E0E1*/ tmp4_U16 = s_a;
                tmp6_U16 = (tmp4_U16 + 0x0002) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0x0002);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1476;
      break;
    case 1475:  // $E0E1
      /*$E0E1*/ tmp6_U16 = adc_dec16(s_a, 0x02, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1476;
      break;
    case 1476:  // $E0E3
      /*$E0E3*/ tmp5_U8 = ram_peek(0x009c);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_y = tmp5_U8;
      /*$E0E5*/ branchTarget = true; block_id = !s_status_c ? 1477 : 1478;
      break;
    case 1477:  // $E0E5
      /*$E0E5*/ CYCLES_EDGE(0xe0e5, 1);
                branchTarget = true; block_id = 1479;
      break;
    case 1478:  // $E0E7
      /*$E0E7*/ CYCLES(0xe0e7, 2);
                tmp5_U8 = (uint8_t)(s_y + 0x01);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_y = tmp5_U8;
                block_id = 1479;
      break;
    case 1479:  // $E0E8
      /*$E0E8*/ CYCLES(0xe0e8, 12);
                ram_poke(0x0083, s_a);
      /*$E0EA*/ ram_poke(0x0084, s_y);
      /*$E0EC*/ branchTarget = true; block_id = find_block_id_func_t001(0xe0ec, pop16() + 1);;
      break;
    case 1480:  // $E0ED
      /*$E0ED*/ CYCLES(0xe0ed, 15);
      /*$E0EF*/ tmp6_U16 = ram_peek(0x000f) << 0x01;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
      /*$E0F0*/ block_id = s_status_d ? 1482 : 1481;
      break;
    case 1481:  // $E0F0
      /*$E0F0*/ tmp6_U16 = (s_a + 0x0005) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1483;
      break;
    case 1482:  // $E0F0
      /*$E0F0*/ tmp6_U16 = adc_dec16(s_a, 0x05, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1483;
      break;
    case 1483:  // $E0F2
      /*$E0F2*/ block_id = s_status_d ? 1485 : 1484;
      break;
    case 1484:  // $E0F2
      /*$E0F2*/ tmp6_U16 = s_a;
                tmp4_U16 = ram_peek(0x009b);
                tmp3_U16 = (tmp6_U16 + tmp4_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1486;
      break;
    case 1485:  // $E0F2
      /*$E0F2*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x009b), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1486;
      break;
    case 1486:  // $E0F4
      /*$E0F4*/ tmp5_U8 = ram_peek(0x009c);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_y = tmp5_U8;
      /*$E0F6*/ branchTarget = true; block_id = !s_status_c ? 1487 : 1488;
      break;
    case 1487:  // $E0F6
      /*$E0F6*/ CYCLES_EDGE(0xe0f6, 1);
                branchTarget = true; block_id = 1489;
      break;
    case 1488:  // $E0F8
      /*$E0F8*/ CYCLES(0xe0f8, 2);
                tmp5_U8 = (uint8_t)(s_y + 0x01);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_y = tmp5_U8;
                block_id = 1489;
      break;
    case 1489:  // $E0F9
      /*$E0F9*/ CYCLES(0xe0f9, 12);
                ram_poke(0x0094, s_a);
      /*$E0FB*/ ram_poke(0x0095, s_y);
      /*$E0FD*/ branchTarget = true; block_id = find_block_id_func_t001(0xe0fd, pop16() + 1);;
      break;
    case 1490:  // $E102
      /*$E102*/ CYCLES(0xe102, 6);
                branchTarget = true; push16(0xe104); block_id = 2;
      break;
    case 1491:  // $E105
      /*$E105*/ CYCLES(0xe105, 6);
                branchTarget = true; push16(0xe107); block_id = 1259;
      break;
    case 1492:  // $E108
      /*$E108*/ CYCLES(0xe108, 5);
                tmp5_U8 = ram_peek(0x00a2);
                s_status_not_z = tmp5_U8;
      /*$E10A*/ branchTarget = true; block_id = (tmp5_U8 & 0x80) ? 1493 : 1494;
      break;
    case 1493:  // $E10A
      /*$E10A*/ CYCLES_EDGE(0xe10a, 1);
                branchTarget = true; block_id = 1497;
      break;
    case 1494:  // $E10C
      /*$E10C*/ CYCLES(0xe10c, 7);
      /*$E10E*/ tmp1_U8 = ram_peek(0x009d) >= 0x90;
                s_status_c = tmp1_U8;
      /*$E110*/ branchTarget = true; block_id = !tmp1_U8 ? 1495 : 1496;
      break;
    case 1495:  // $E110
      /*$E110*/ CYCLES_EDGE(0xe110, 1);
                branchTarget = true; block_id = 1499;
      break;
    case 1496:  // $E112
      /*$E112*/ CYCLES(0xe112, 10);
                s_a = 0xfe;
      /*$E114*/ s_y = 0xe0;
      /*$E116*/ FUNC_FCOMP(0xe118);
                branchTarget = true; block_id = 1497;
      break;
    case 1497:  // $E119
      /*$E119*/ CYCLES(0xe119, 2);
                branchTarget = true; block_id = s_status_not_z ? 1498 : 1499;
      break;
    case 1498:  // $E119
      /*$E119*/ CYCLES_EDGE(0xe119, 1);
                branchTarget = true; block_id = 1526;
      break;
    case 1499:  // $E11B
      /*$E11B*/ CYCLES(0xe11b, 3);
                FUNC_QINT(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xe11b, pop16() + 1);;
      break;
    case 1500:  // $E120
      /*$E120*/ CYCLES_EDGE(0xe120, 1);
                branchTarget = true; block_id = 1508;
      break;
    case 1501:  // $E122
      /*$E122*/ CYCLES(0xe122, 17);
      /*$E126*/ push8((ram_peek(0x0010) | ram_peek(0x0012)));
      /*$E129*/ push8(ram_peek(0x0011));
      /*$E12A*/ s_y = 0x00;
                block_id = 1502;
      break;
    case 1502:  // $E12C
      /*$E12C*/ CYCLES(0xe12c, 23);
      /*$E12D*/ push8(s_y);
      /*$E130*/ push8(ram_peek(0x0082));
      /*$E131*/ tmp5_U8 = ram_peek(0x0081);
                s_a = tmp5_U8;
      /*$E133*/ push8(tmp5_U8);
      /*$E134*/ branchTarget = true; push16(0xe136); block_id = 1490;
      break;
    case 1503:  // $E137
      /*$E137*/ CYCLES(0xe137, 60);
                tmp5_U8 = pop8();
      /*$E138*/ ram_poke(0x0081, tmp5_U8);
      /*$E13A*/ tmp5_U8 = pop8();
      /*$E13B*/ ram_poke(0x0082, tmp5_U8);
      /*$E13D*/ tmp5_U8 = pop8();
      /*$E13F*/ tmp2_U8 = s_sp;
                s_x = tmp2_U8;
      /*$E143*/ push8(ram_peek((0x0102 + tmp2_U8)));
      /*$E147*/ push8(ram_peek((0x0101 + tmp2_U8)));
      /*$E14A*/ ram_poke((0x0102 + tmp2_U8), ram_peek(0x00a0));
      /*$E14D*/ tmp1_U8 = ram_peek(0x00a1);
                s_a = tmp1_U8;
      /*$E14F*/ ram_poke((0x0101 + tmp2_U8), tmp1_U8);
      /*$E152*/ s_y = (uint8_t)(tmp5_U8 + 0x01);
      /*$E153*/ branchTarget = true; push16(0xe155); block_id = 5;
      break;
    case 1504:  // $E156
      /*$E156*/ CYCLES(0xe156, 4);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0x2c);
      /*$E158*/ branchTarget = true; block_id = !(tmp5_U8 != 0x2c) ? 1505 : 1506;
      break;
    case 1505:  // $E158
      /*$E158*/ CYCLES_EDGE(0xe158, 1);
                branchTarget = true; block_id = 1502;
      break;
    case 1506:  // $E15A
      /*$E15A*/ CYCLES(0xe15a, 9);
                ram_poke(0x000f, s_y);
      /*$E15C*/ branchTarget = true; push16(0xe15e); block_id = 1372;
      break;
    case 1507:  // $E15F
      /*$E15F*/ CYCLES(0xe15f, 19);
                tmp5_U8 = pop8();
      /*$E160*/ ram_poke(0x0011, tmp5_U8);
      /*$E162*/ tmp5_U8 = pop8();
      /*$E163*/ ram_poke(0x0012, tmp5_U8);
      /*$E167*/ ram_poke(0x0010, (tmp5_U8 & 0x7f));
                block_id = 1508;
      break;
    case 1508:  // $E169
      /*$E169*/ CYCLES(0xe169, 6);
                s_x = ram_peek(0x006b);
      /*$E16B*/ s_a = ram_peek(0x006c);
                block_id = 1509;
      break;
    case 1509:  // $E16D
      /*$E16D*/ CYCLES(0xe16d, 11);
                ram_poke(0x009b, s_x);
      /*$E16F*/ tmp5_U8 = s_a;
                ram_poke(0x009c, tmp5_U8);
      /*$E173*/ branchTarget = true; block_id = (tmp5_U8 != ram_peek(0x006e)) ? 1510 : 1511;
      break;
    case 1510:  // $E173
      /*$E173*/ CYCLES_EDGE(0xe173, 1);
                branchTarget = true; block_id = 1513;
      break;
    case 1511:  // $E175
      /*$E175*/ CYCLES(0xe175, 5);
                tmp5_U8 = s_x;
                tmp2_U8 = ram_peek(0x006d);
                s_status_c = (tmp5_U8 >= tmp2_U8);
      /*$E177*/ branchTarget = true; block_id = !(tmp5_U8 != tmp2_U8) ? 1512 : 1513;
      break;
    case 1512:  // $E177
      /*$E177*/ CYCLES_EDGE(0xe177, 1);
      /*$E1B8*/ CYCLES(0xe1b8, 5);
      /*$E1BA*/ branchTarget = true; block_id = !ram_peek(0x0014) ? 1534 : 1535;
      break;
    case 1513:  // $E179
      /*$E179*/ CYCLES(0xe179, 14);
      /*$E17B*/ tmp5_U8 = peek(ram_peek16(0x009b));
      /*$E17D*/ s_y = 0x01;
      /*$E180*/ branchTarget = true; block_id = (tmp5_U8 != ram_peek(0x0081)) ? 1514 : 1515;
      break;
    case 1514:  // $E180
      /*$E180*/ CYCLES_EDGE(0xe180, 1);
                branchTarget = true; block_id = 1517;
      break;
    case 1515:  // $E182
      /*$E182*/ CYCLES(0xe182, 10);
                tmp2_U8 = ram_peek(0x0082);
      /*$E184*/ tmp5_U8 = peek((ram_peek16(0x009b) + s_y));
                s_status_c = (tmp2_U8 >= tmp5_U8);
      /*$E186*/ branchTarget = true; block_id = !(tmp2_U8 != tmp5_U8) ? 1516 : 1517;
      break;
    case 1516:  // $E186
      /*$E186*/ CYCLES_EDGE(0xe186, 1);
      /*$E19E*/ CYCLES(0xe19e, 7);
                s_x = 0x78;
      /*$E1A2*/ branchTarget = true; block_id = ram_peek(0x0010) ? 1528 : 1529;
      break;
    case 1517:  // $E188
      /*$E188*/ CYCLES(0xe188, 26);
                tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$E189*/ tmp2_U8 = peek((ram_peek16(0x009b) + tmp2_U8));
                s_a = tmp2_U8;
      /*$E18B*/ s_status_c = 0x00;
      /*$E18C*/ block_id = s_status_d ? 1519 : 1518;
      break;
    case 1518:  // $E18C
      /*$E18C*/ tmp3_U16 = (s_a + ram_peek(0x009b)) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1520;
      break;
    case 1519:  // $E18C
      /*$E18C*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x009b), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 1520;
      break;
    case 1520:  // $E18E
      /*$E18E*/ s_x = s_a;
      /*$E18F*/ tmp5_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp5_U8;
      /*$E190*/ tmp5_U8 = peek((ram_peek16(0x009b) + tmp5_U8));
                s_a = tmp5_U8;
      /*$E192*/ block_id = s_status_d ? 1522 : 1521;
      break;
    case 1521:  // $E192
      /*$E192*/ tmp3_U16 = s_a;
                tmp4_U16 = ram_peek(0x009c);
                tmp6_U16 = (tmp3_U16 + tmp4_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1523;
      break;
    case 1522:  // $E192
      /*$E192*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x009c), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1523;
      break;
    case 1523:  // $E194
      /*$E194*/ branchTarget = true; block_id = !s_status_c ? 1524 : 1525;
      break;
    case 1524:  // $E194
      /*$E194*/ CYCLES_EDGE(0xe194, 1);
                branchTarget = true; block_id = 1509;
      break;
    case 1525:  // $E196
      /*$E196*/ CYCLES(0xe196, 6);
                s_x = 0x6b;
      /*$E198*/ block_id = 1527;
      break;
    case 1526:  // $E199
      /*$E199*/ CYCLES(0xe199, 2);
                s_x = 0x35;
                block_id = 1527;
      break;
    case 1527:  // $E19B
      /*$E19B*/ CYCLES(0xe19b, 3);
                branchTarget = true; block_id = 887;
      break;
    case 1528:  // $E1A2
      /*$E1A2*/ CYCLES_EDGE(0xe1a2, 1);
                branchTarget = true; block_id = 1527;
      break;
    case 1529:  // $E1A4
      /*$E1A4*/ CYCLES(0xe1a4, 5);
                tmp5_U8 = ram_peek(0x0014);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$E1A6*/ branchTarget = true; block_id = !tmp5_U8 ? 1530 : 1531;
      break;
    case 1530:  // $E1A6
      /*$E1A6*/ CYCLES_EDGE(0xe1a6, 1);
      /*$E1AA*/ CYCLES(0xe1aa, 6);
                FUNC_GETARY(0xe1ac);
      /*$E1AD*/ CYCLES(0xe1ad, 12);
                tmp5_U8 = ram_peek(0x000f);
      /*$E1AF*/ s_y = 0x04;
      /*$E1B1*/ tmp2_U8 = peek((ram_peek16(0x009b) + 0x0004));
                s_status_c = (tmp5_U8 >= tmp2_U8);
      /*$E1B3*/ branchTarget = true; block_id = (tmp5_U8 != tmp2_U8) ? 1532 : 1533;
      break;
    case 1531:  // $E1A8
      /*$E1A8*/ CYCLES(0xe1a8, 8);
                s_status_c = 0x01;
      /*$E1A9*/ branchTarget = true; block_id = find_block_id_func_t001(0xe1a9, pop16() + 1);;
      break;
    case 1532:  // $E1B3
      /*$E1B3*/ CYCLES_EDGE(0xe1b3, 1);
                branchTarget = true; block_id = 1525;
      break;
    case 1533:  // $E1B5
      /*$E1B5*/ CYCLES(0xe1b5, 3);
                branchTarget = true; block_id = 1583;
      break;
    case 1534:  // $E1BA
      /*$E1BA*/ CYCLES_EDGE(0xe1ba, 1);
      /*$E1C1*/ CYCLES(0xe1c1, 6);
                FUNC_GETARY(0xe1c3);
      /*$E1C4*/ CYCLES(0xe1c4, 6);
                branchTarget = true; push16(0xe1c6); block_id = 867;
      break;
    case 1535:  // $E1BC
      /*$E1BC*/ CYCLES(0xe1bc, 5);
                s_x = 0x2a;
      /*$E1BE*/ branchTarget = true; block_id = 887;
      break;
    case 1536:  // $E1C7
      /*$E1C7*/ CYCLES(0xe1c7, 20);
      /*$E1C9*/ s_y = 0x00;
      /*$E1CA*/ ram_poke(0x00ae, 0x00);
      /*$E1CC*/ s_x = 0x05;
      /*$E1CE*/ tmp2_U8 = ram_peek(0x0081);
      /*$E1D0*/ poke(ram_peek16(0x009b), tmp2_U8);
      /*$E1D2*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1537 : 1538;
      break;
    case 1537:  // $E1D2
      /*$E1D2*/ CYCLES_EDGE(0xe1d2, 1);
                branchTarget = true; block_id = 1539;
      break;
    case 1538:  // $E1D4
      /*$E1D4*/ CYCLES(0xe1d4, 2);
                s_x = (uint8_t)(s_x - 0x01);
                block_id = 1539;
      break;
    case 1539:  // $E1D5
      /*$E1D5*/ CYCLES(0xe1d5, 13);
                tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$E1D6*/ tmp5_U8 = ram_peek(0x0082);
      /*$E1D8*/ poke((ram_peek16(0x009b) + tmp2_U8), tmp5_U8);
      /*$E1DA*/ branchTarget = true; block_id = !(tmp5_U8 & 0x80) ? 1540 : 1541;
      break;
    case 1540:  // $E1DA
      /*$E1DA*/ CYCLES_EDGE(0xe1da, 1);
                branchTarget = true; block_id = 1542;
      break;
    case 1541:  // $E1DC
      /*$E1DC*/ CYCLES(0xe1dc, 4);
      /*$E1DD*/ s_x = (uint8_t)(s_x - 0x02);
                block_id = 1542;
      break;
    case 1542:  // $E1DE
      /*$E1DE*/ CYCLES(0xe1de, 18);
                ram_poke(0x00ad, s_x);
      /*$E1E4*/ tmp5_U8 = (uint8_t)(s_y + 0x03);
                s_y = tmp5_U8;
      /*$E1E5*/ poke((ram_peek16(0x009b) + tmp5_U8), ram_peek(0x000f));
                block_id = 1543;
      break;
    case 1543:  // $E1E7
      /*$E1E7*/ CYCLES(0xe1e7, 9);
                s_x = 0x0b;
      /*$E1E9*/ s_a = 0x00;
      /*$E1EB*/ tmp5_U8 = (ram_peek(0x0010) >> 0x06) & 0x01;
                s_status_v = tmp5_U8;
      /*$E1ED*/ branchTarget = true; block_id = !tmp5_U8 ? 1544 : 1545;
      break;
    case 1544:  // $E1ED
      /*$E1ED*/ CYCLES_EDGE(0xe1ed, 1);
                branchTarget = true; block_id = 1551;
      break;
    case 1545:  // $E1EF
      /*$E1EF*/ CYCLES(0xe1ef, 16);
                tmp5_U8 = pop8();
                s_a = tmp5_U8;
      /*$E1F0*/ s_status_c = 0x00;
      /*$E1F1*/ block_id = s_status_d ? 1547 : 1546;
      break;
    case 1546:  // $E1F1
      /*$E1F1*/ tmp6_U16 = (s_a + 0x0001) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1548;
      break;
    case 1547:  // $E1F1
      /*$E1F1*/ tmp6_U16 = adc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1548;
      break;
    case 1548:  // $E1F3
      /*$E1F3*/ s_x = s_a;
      /*$E1F4*/ tmp5_U8 = pop8();
                s_a = tmp5_U8;
      /*$E1F5*/ block_id = s_status_d ? 1550 : 1549;
      break;
    case 1549:  // $E1F5
      /*$E1F5*/ tmp6_U16 = s_a;
                tmp4_U16 = tmp6_U16 + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1552;
      break;
    case 1550:  // $E1F5
      /*$E1F5*/ tmp4_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1552;
      break;
    case 1551:  // $E1F7
      /*$E1F7*/ CYCLES(0xe1f7, 24);
                tmp5_U8 = s_y;
      /*$E1F8*/ poke((ram_peek16(0x009b) + (uint8_t)(tmp5_U8 + 0x01)), s_a);
      /*$E1FA*/ tmp5_U8 = (uint8_t)(tmp5_U8 + 0x02);
                s_y = tmp5_U8;
      /*$E1FB*/ tmp2_U8 = s_x;
                s_a = tmp2_U8;
      /*$E1FC*/ poke((ram_peek16(0x009b) + tmp5_U8), tmp2_U8);
      /*$E1FE*/ branchTarget = true; push16(0xe200); block_id = 1619;
      break;
    case 1552:  // $E1F7
      /*$E1F5*/ block_id = 1551;
      break;
    case 1553:  // $E201
      /*$E201*/ CYCLES(0xe201, 16);
                ram_poke(0x00ad, s_x);
      /*$E203*/ ram_poke(0x00ae, s_a);
      /*$E205*/ s_y = ram_peek(0x005e);
      /*$E207*/ tmp5_U8 = (uint8_t)(ram_peek(0x000f) - 0x01);
                ram_poke(0x000f, tmp5_U8);
      /*$E209*/ branchTarget = true; block_id = tmp5_U8 ? 1554 : 1555;
      break;
    case 1554:  // $E209
      /*$E209*/ CYCLES_EDGE(0xe209, 1);
                branchTarget = true; block_id = 1543;
      break;
    case 1555:  // $E20B
      /*$E20B*/ CYCLES(0xe20b, 5);
                block_id = s_status_d ? 1557 : 1556;
      break;
    case 1556:  // $E20B
      /*$E20B*/ tmp4_U16 = (s_a + ram_peek(0x0095)) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1558;
      break;
    case 1557:  // $E20B
      /*$E20B*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x0095), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1558;
      break;
    case 1558:  // $E20D
      /*$E20D*/ branchTarget = true; block_id = s_status_c ? 1559 : 1560;
      break;
    case 1559:  // $E20D
      /*$E20D*/ CYCLES_EDGE(0xe20d, 1);
                branchTarget = true; block_id = 1591;
      break;
    case 1560:  // $E20F
      /*$E20F*/ CYCLES(0xe20f, 12);
                tmp5_U8 = s_a;
                ram_poke(0x0095, tmp5_U8);
      /*$E211*/ s_y = tmp5_U8;
      /*$E212*/ s_a = s_x;
      /*$E213*/ block_id = s_status_d ? 1562 : 1561;
      break;
    case 1561:  // $E213
      /*$E213*/ tmp4_U16 = s_a;
                tmp6_U16 = ram_peek(0x0094);
                tmp3_U16 = (tmp4_U16 + tmp6_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp6_U16);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1563;
      break;
    case 1562:  // $E213
      /*$E213*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x0094), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1563;
      break;
    case 1563:  // $E215
      /*$E215*/ branchTarget = true; block_id = !s_status_c ? 1564 : 1565;
      break;
    case 1564:  // $E215
      /*$E215*/ CYCLES_EDGE(0xe215, 1);
                branchTarget = true; block_id = 1567;
      break;
    case 1565:  // $E217
      /*$E217*/ CYCLES(0xe217, 4);
                tmp5_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp5_U8;
      /*$E218*/ branchTarget = true; block_id = !tmp5_U8 ? 1566 : 1567;
      break;
    case 1566:  // $E218
      /*$E218*/ CYCLES_EDGE(0xe218, 1);
                branchTarget = true; block_id = 1591;
      break;
    case 1567:  // $E21A
      /*$E21A*/ CYCLES(0xe21a, 6);
                branchTarget = true; push16(0xe21c); block_id = 867;
      break;
    case 1568:  // $E21D
      /*$E21D*/ CYCLES(0xe21d, 18);
                ram_poke(0x006d, s_a);
      /*$E21F*/ ram_poke(0x006e, s_y);
      /*$E221*/ s_a = 0x00;
      /*$E223*/ ram_poke(0x00ae, (uint8_t)(ram_peek(0x00ae) + 0x01));
      /*$E225*/ tmp5_U8 = ram_peek(0x00ad);
                s_y = tmp5_U8;
      /*$E227*/ branchTarget = true; block_id = !tmp5_U8 ? 1569 : 1570;
      break;
    case 1569:  // $E227
      /*$E227*/ CYCLES_EDGE(0xe227, 1);
                branchTarget = true; block_id = 1572;
      break;
    case 1570:  // $E229
      /*$E229*/ CYCLES(0xe229, 10);
                tmp5_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp5_U8;
      /*$E22A*/ poke((ram_peek16al(0x0094) + tmp5_U8), s_a);
      /*$E22C*/ branchTarget = true; block_id = tmp5_U8 ? 1571 : 1572;
      break;
    case 1571:  // $E22C
      /*$E22C*/ CYCLES_EDGE(0xe22c, 1);
                branchTarget = true; block_id = 1570;
      break;
    case 1572:  // $E22E
      /*$E22E*/ CYCLES(0xe22e, 12);
                ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) - 0x01));
      /*$E230*/ tmp5_U8 = (uint8_t)(ram_peek(0x00ae) - 0x01);
                ram_poke(0x00ae, tmp5_U8);
      /*$E232*/ branchTarget = true; block_id = tmp5_U8 ? 1573 : 1574;
      break;
    case 1573:  // $E232
      /*$E232*/ CYCLES_EDGE(0xe232, 1);
                branchTarget = true; block_id = 1570;
      break;
    case 1574:  // $E234
      /*$E234*/ CYCLES(0xe234, 40);
                ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) + 0x01));
      /*$E236*/ s_status_c = 0x01;
      /*$E237*/ s_a = ram_peek(0x006d);
      /*$E239*/ block_id = s_status_d ? 1576 : 1575;
      break;
    case 1575:  // $E239
      /*$E239*/ tmp3_U16 = (s_a - ram_peek(0x009b)) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1577;
      break;
    case 1576:  // $E239
      /*$E239*/ tmp3_U16 = sbc_dec16(s_a, ram_peek(0x009b), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 1577;
      break;
    case 1577:  // $E23B
      /*$E23D*/ poke((ram_peek16(0x009b) + 0x0002), s_a);
      /*$E23F*/ s_a = ram_peek(0x006e);
      /*$E241*/ s_y = 0x03;
      /*$E242*/ block_id = s_status_d ? 1579 : 1578;
      break;
    case 1578:  // $E242
      /*$E242*/ tmp3_U16 = s_a;
                tmp6_U16 = ram_peek(0x009c);
                tmp4_U16 = (tmp3_U16 - tmp6_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp6_U16));
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1580;
      break;
    case 1579:  // $E242
      /*$E242*/ tmp4_U16 = sbc_dec16(s_a, ram_peek(0x009c), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1580;
      break;
    case 1580:  // $E244
      /*$E244*/ poke((ram_peek16(0x009b) + s_y), s_a);
      /*$E246*/ tmp5_U8 = ram_peek(0x0010);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$E248*/ branchTarget = true; block_id = tmp5_U8 ? 1581 : 1582;
      break;
    case 1581:  // $E248
      /*$E248*/ CYCLES_EDGE(0xe248, 1);
                branchTarget = true; block_id = 1618;
      break;
    case 1582:  // $E24A
      /*$E24A*/ CYCLES(0xe24a, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 1583;
      break;
    case 1583:  // $E24B
      /*$E24B*/ CYCLES(0xe24b, 13);
                tmp2_U8 = peek((ram_peek16(0x009b) + s_y));
      /*$E24D*/ ram_poke(0x000f, tmp2_U8);
      /*$E24F*/ s_a = 0x00;
      /*$E251*/ ram_poke(0x00ad, 0x00);
                block_id = 1584;
      break;
    case 1584:  // $E253
      /*$E253*/ CYCLES(0xe253, 28);
                ram_poke(0x00ae, s_a);
      /*$E255*/ tmp5_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp5_U8;
      /*$E256*/ tmp2_U8 = pop8();
      /*$E257*/ s_x = tmp2_U8;
      /*$E258*/ ram_poke(0x00a0, tmp2_U8);
      /*$E25A*/ tmp2_U8 = pop8();
      /*$E25B*/ ram_poke(0x00a1, tmp2_U8);
      /*$E25D*/ tmp5_U8 = peek((ram_peek16(0x009b) + tmp5_U8));
                s_status_not_z = (tmp2_U8 != tmp5_U8);
                tmp5_U8 = tmp2_U8 >= tmp5_U8;
                s_status_c = tmp5_U8;
      /*$E25F*/ branchTarget = true; block_id = !tmp5_U8 ? 1585 : 1586;
      break;
    case 1585:  // $E25F
      /*$E25F*/ CYCLES_EDGE(0xe25f, 1);
      /*$E26F*/ CYCLES(0xe26f, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 1592;
      break;
    case 1586:  // $E261
      /*$E261*/ CYCLES(0xe261, 2);
                branchTarget = true; block_id = s_status_not_z ? 1587 : 1588;
      break;
    case 1587:  // $E261
      /*$E261*/ CYCLES_EDGE(0xe261, 1);
                branchTarget = true; block_id = 1590;
      break;
    case 1588:  // $E263
      /*$E263*/ CYCLES(0xe263, 11);
                tmp5_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp5_U8;
      /*$E265*/ tmp5_U8 = peek((ram_peek16(0x009b) + tmp5_U8));
                tmp5_U8 = s_x >= tmp5_U8;
                s_status_c = tmp5_U8;
      /*$E267*/ branchTarget = true; block_id = !tmp5_U8 ? 1589 : 1590;
      break;
    case 1589:  // $E267
      /*$E267*/ CYCLES_EDGE(0xe267, 1);
                branchTarget = true; block_id = 1592;
      break;
    case 1590:  // $E269
      /*$E269*/ CYCLES(0xe269, 3);
                branchTarget = true; block_id = 1525;
      break;
    case 1591:  // $E26C
      /*$E26C*/ CYCLES(0xe26c, 3);
                branchTarget = true; block_id = 886;
      break;
    case 1592:  // $E270
      /*$E270*/ CYCLES(0xe270, 10);
      /*$E272*/ tmp5_U8 = ram_peek(0x00ae) | ram_peek(0x00ad);
                s_a = tmp5_U8;
      /*$E274*/ s_status_c = 0x00;
      /*$E275*/ branchTarget = true; block_id = !tmp5_U8 ? 1593 : 1594;
      break;
    case 1593:  // $E275
      /*$E275*/ CYCLES_EDGE(0xe275, 1);
                branchTarget = true; block_id = 1599;
      break;
    case 1594:  // $E277
      /*$E277*/ CYCLES(0xe277, 6);
                branchTarget = true; push16(0xe279); block_id = 1619;
      break;
    case 1595:  // $E27A
      /*$E27A*/ CYCLES(0xe27a, 12);
                s_a = s_x;
      /*$E27B*/ block_id = s_status_d ? 1597 : 1596;
      break;
    case 1596:  // $E27B
      /*$E27B*/ tmp4_U16 = (s_a + ram_peek(0x00a0)) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1598;
      break;
    case 1597:  // $E27B
      /*$E27B*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x00a0), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1598;
      break;
    case 1598:  // $E27D
      /*$E27D*/ s_x = s_a;
      /*$E27E*/ s_a = s_y;
      /*$E27F*/ s_y = ram_peek(0x005e);
                block_id = 1599;
      break;
    case 1599:  // $E281
      /*$E281*/ CYCLES(0xe281, 13);
                block_id = s_status_d ? 1601 : 1600;
      break;
    case 1600:  // $E281
      /*$E281*/ tmp4_U16 = s_a;
                tmp6_U16 = ram_peek(0x00a1);
                tmp3_U16 = (tmp4_U16 + tmp6_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp6_U16);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1602;
      break;
    case 1601:  // $E281
      /*$E281*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x00a1), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1602;
      break;
    case 1602:  // $E283
      /*$E283*/ ram_poke(0x00ad, s_x);
      /*$E285*/ tmp5_U8 = (uint8_t)(ram_peek(0x000f) - 0x01);
                ram_poke(0x000f, tmp5_U8);
      /*$E287*/ branchTarget = true; block_id = tmp5_U8 ? 1603 : 1604;
      break;
    case 1603:  // $E287
      /*$E287*/ CYCLES_EDGE(0xe287, 1);
                branchTarget = true; block_id = 1584;
      break;
    case 1604:  // $E289
      /*$E289*/ CYCLES(0xe289, 10);
                ram_poke(0x00ae, s_a);
      /*$E28B*/ s_x = 0x05;
      /*$E28F*/ branchTarget = true; block_id = !(ram_peek(0x0081) & 0x80) ? 1605 : 1606;
      break;
    case 1605:  // $E28F
      /*$E28F*/ CYCLES_EDGE(0xe28f, 1);
                branchTarget = true; block_id = 1607;
      break;
    case 1606:  // $E291
      /*$E291*/ CYCLES(0xe291, 2);
                s_x = (uint8_t)(s_x - 0x01);
                block_id = 1607;
      break;
    case 1607:  // $E292
      /*$E292*/ CYCLES(0xe292, 5);
      /*$E294*/ branchTarget = true; block_id = !(ram_peek(0x0082) & 0x80) ? 1608 : 1609;
      break;
    case 1608:  // $E294
      /*$E294*/ CYCLES_EDGE(0xe294, 1);
                branchTarget = true; block_id = 1610;
      break;
    case 1609:  // $E296
      /*$E296*/ CYCLES(0xe296, 4);
      /*$E297*/ s_x = (uint8_t)(s_x - 0x02);
                block_id = 1610;
      break;
    case 1610:  // $E298
      /*$E298*/ CYCLES(0xe298, 11);
                ram_poke(0x0064, s_x);
      /*$E29A*/ s_a = 0x00;
      /*$E29C*/ branchTarget = true; push16(0xe29e); block_id = 1620;
      break;
    case 1611:  // $E29F
      /*$E29F*/ CYCLES(0xe29f, 21);
                s_a = s_x;
      /*$E2A0*/ block_id = s_status_d ? 1613 : 1612;
      break;
    case 1612:  // $E2A0
      /*$E2A0*/ tmp3_U16 = (s_a + ram_peek(0x0094)) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1614;
      break;
    case 1613:  // $E2A0
      /*$E2A0*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x0094), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 1614;
      break;
    case 1614:  // $E2A2
      /*$E2A2*/ ram_poke(0x0083, s_a);
      /*$E2A4*/ s_a = s_y;
      /*$E2A5*/ block_id = s_status_d ? 1616 : 1615;
      break;
    case 1615:  // $E2A5
      /*$E2A5*/ tmp3_U16 = s_a;
                tmp6_U16 = ram_peek(0x0095);
                tmp4_U16 = (tmp3_U16 + tmp6_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)tmp6_U16);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1617;
      break;
    case 1616:  // $E2A5
      /*$E2A5*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x0095), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1617;
      break;
    case 1617:  // $E2A7
      /*$E2A7*/ tmp5_U8 = s_a;
                ram_poke(0x0084, tmp5_U8);
      /*$E2A9*/ s_y = tmp5_U8;
      /*$E2AA*/ tmp5_U8 = ram_peek(0x0083);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
                block_id = 1618;
      break;
    case 1618:  // $E2AC
      /*$E2AC*/ CYCLES(0xe2ac, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xe2ac, pop16() + 1);;
      break;
    case 1619:  // $E2AD
      /*$E2AD*/ CYCLES(0xe2ad, 18);
                tmp5_U8 = s_y;
                ram_poke(0x005e, tmp5_U8);
      /*$E2AF*/ tmp2_U8 = peek((ram_peek16(0x009b) + tmp5_U8));
      /*$E2B1*/ ram_poke(0x0064, tmp2_U8);
      /*$E2B4*/ tmp5_U8 = peek((ram_peek16(0x009b) + (uint8_t)(tmp5_U8 - 0x01)));
                s_a = tmp5_U8;
                block_id = 1620;
      break;
    case 1620:  // $E2B6
      /*$E2B6*/ CYCLES(0xe2b6, 12);
                ram_poke(0x0065, s_a);
      /*$E2BA*/ ram_poke(0x0099, 0x10);
      /*$E2BC*/ s_x = 0x00;
      /*$E2BE*/ s_y = 0x00;
                block_id = 1621;
      break;
    case 1621:  // $E2C0
      /*$E2C0*/ CYCLES(0xe2c0, 14);
      /*$E2C1*/ tmp4_U16 = s_x << 0x01;
      /*$E2C2*/ s_x = ((uint8_t)tmp4_U16);
      /*$E2C4*/ tmp4_U16 = (s_y << 0x01) | (uint8_t)(tmp4_U16 >> 8);
                tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = tmp5_U8;
                tmp2_U8 = (uint8_t)tmp4_U16;
                s_a = tmp2_U8;
      /*$E2C5*/ s_y = tmp2_U8;
      /*$E2C6*/ branchTarget = true; block_id = tmp5_U8 ? 1622 : 1623;
      break;
    case 1622:  // $E2C6
      /*$E2C6*/ CYCLES_EDGE(0xe2c6, 1);
                branchTarget = true; block_id = 1591;
      break;
    case 1623:  // $E2C8
      /*$E2C8*/ CYCLES(0xe2c8, 12);
                tmp4_U16 = ram_peek(0x00ad) << 0x01;
                ram_poke(0x00ad, ((uint8_t)tmp4_U16));
      /*$E2CA*/ tmp4_U16 = (ram_peek(0x00ae) << 0x01) | (uint8_t)(tmp4_U16 >> 8);
                tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = tmp5_U8;
                ram_poke(0x00ae, ((uint8_t)tmp4_U16));
      /*$E2CC*/ branchTarget = true; block_id = !tmp5_U8 ? 1624 : 1625;
      break;
    case 1624:  // $E2CC
      /*$E2CC*/ CYCLES_EDGE(0xe2cc, 1);
                branchTarget = true; block_id = 1633;
      break;
    case 1625:  // $E2CE
      /*$E2CE*/ CYCLES(0xe2ce, 18);
                s_status_c = 0x00;
      /*$E2CF*/ s_a = s_x;
      /*$E2D0*/ block_id = s_status_d ? 1627 : 1626;
      break;
    case 1626:  // $E2D0
      /*$E2D0*/ tmp4_U16 = (s_a + ram_peek(0x0064)) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1628;
      break;
    case 1627:  // $E2D0
      /*$E2D0*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x0064), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1628;
      break;
    case 1628:  // $E2D2
      /*$E2D2*/ s_x = s_a;
      /*$E2D3*/ s_a = s_y;
      /*$E2D4*/ block_id = s_status_d ? 1630 : 1629;
      break;
    case 1629:  // $E2D4
      /*$E2D4*/ tmp4_U16 = s_a;
                tmp6_U16 = ram_peek(0x0065);
                tmp3_U16 = (tmp4_U16 + tmp6_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp6_U16);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1631;
      break;
    case 1630:  // $E2D4
      /*$E2D4*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x0065), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1631;
      break;
    case 1631:  // $E2D6
      /*$E2D6*/ s_y = s_a;
      /*$E2D7*/ branchTarget = true; block_id = s_status_c ? 1632 : 1633;
      break;
    case 1632:  // $E2D7
      /*$E2D7*/ CYCLES_EDGE(0xe2d7, 1);
                branchTarget = true; block_id = 1591;
      break;
    case 1633:  // $E2D9
      /*$E2D9*/ CYCLES(0xe2d9, 7);
                tmp5_U8 = (uint8_t)(ram_peek(0x0099) - 0x01);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                ram_poke(0x0099, tmp5_U8);
      /*$E2DB*/ branchTarget = true; block_id = tmp5_U8 ? 1634 : 1635;
      break;
    case 1634:  // $E2DB
      /*$E2DB*/ CYCLES_EDGE(0xe2db, 1);
                branchTarget = true; block_id = 1621;
      break;
    case 1635:  // $E2DD
      /*$E2DD*/ CYCLES(0xe2dd, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xe2dd, pop16() + 1);;
      break;
    case 1636:  // $E2F2
      /*$E2F2*/ CYCLES(0xe2f2, 16);
      /*$E2F4*/ ram_poke(0x0011, 0x00);
      /*$E2F6*/ ram_poke(0x009e, s_a);
      /*$E2F8*/ ram_poke(0x009f, s_y);
      /*$E2FA*/ s_x = 0x90;
      /*$E2FC*/ branchTarget = true; block_id = 2095;
      break;
    case 1637:  // $E301
      /*$E301*/ CYCLES(0xe301, 6);
                s_a = 0x00;
      /*$E303*/ s_status_c = 0x01;
      /*$E304*/ CYCLES_EDGE(0xe304, 1);
                branchTarget = true; block_id = 1636;
      break;
    case 1638:  // $E306
      /*$E306*/ CYCLES(0xe306, 7);
      /*$E308*/ tmp5_U8 = (uint8_t)(ram_peek(0x0076) + 0x01);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_x = tmp5_U8;
      /*$E309*/ branchTarget = true; block_id = tmp5_U8 ? 1639 : 1640;
      break;
    case 1639:  // $E309
      /*$E309*/ CYCLES_EDGE(0xe309, 1);
                branchTarget = true; block_id = 1618;
      break;
    case 1640:  // $E30B
      /*$E30B*/ CYCLES(0xe30b, 9);
                s_x = 0x95;
      /*$E310*/ branchTarget = true; block_id = 887;
      break;
    case 1641:  // $E341
      /*$E341*/ CYCLES(0xe341, 8);
                s_a = 0xc2;
      /*$E343*/ branchTarget = true; push16(0xe345); block_id = 1375;
      break;
    case 1642:  // $E346
      /*$E346*/ CYCLES(0xe346, 11);
                tmp5_U8 = s_a | 0x80;
                s_a = tmp5_U8;
      /*$E348*/ ram_poke(0x0014, tmp5_U8);
      /*$E34A*/ branchTarget = true; push16(0xe34c); block_id = 1402;
      break;
    case 1643:  // $E34D
      /*$E34D*/ CYCLES(0xe34d, 9);
                ram_poke(0x008a, s_a);
      /*$E34F*/ ram_poke(0x008b, s_y);
      /*$E351*/ branchTarget = true; block_id = 1260;
      break;
    case 1644:  // $E357
      /*$E357*/ CYCLES(0xe357, 18);
      /*$E359*/ push8(ram_peek(0x008b));
      /*$E35A*/ tmp5_U8 = ram_peek(0x008a);
                s_a = tmp5_U8;
      /*$E35C*/ push8(tmp5_U8);
      /*$E35D*/ branchTarget = true; push16(0xe35f); block_id = 1370;
      break;
    case 1645:  // $E360
      /*$E360*/ CYCLES(0xe360, 6);
                branchTarget = true; push16(0xe362); block_id = 1260;
      break;
    case 1646:  // $E363
      /*$E363*/ CYCLES(0xe363, 35);
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
      /*$E373*/ branchTarget = true; block_id = !tmp5_U8 ? 1647 : 1648;
      break;
    case 1647:  // $E373
      /*$E373*/ CYCLES_EDGE(0xe373, 1);
      /*$E30E*/ CYCLES(0xe30e, 5);
                s_x = 0xe0;
      /*$E310*/ branchTarget = true; block_id = 887;
      break;
    case 1648:  // $E375
      /*$E375*/ CYCLES(0xe375, 5);
                ram_poke(0x0084, s_a);
      /*$E377*/ s_y = (uint8_t)(s_y + 0x01);
                block_id = 1649;
      break;
    case 1649:  // $E378
      /*$E378*/ CYCLES(0xe378, 12);
                tmp5_U8 = s_y;
                tmp2_U8 = peek((ram_peek16(0x0083) + tmp5_U8));
      /*$E37A*/ push8(tmp2_U8);
      /*$E37B*/ tmp5_U8 = (uint8_t)(tmp5_U8 - 0x01);
                s_y = tmp5_U8;
      /*$E37C*/ branchTarget = true; block_id = !(tmp5_U8 & 0x80) ? 1650 : 1651;
      break;
    case 1650:  // $E37C
      /*$E37C*/ CYCLES_EDGE(0xe37c, 1);
                branchTarget = true; block_id = 1649;
      break;
    case 1651:  // $E37E
      /*$E37E*/ CYCLES(0xe37e, 9);
                s_y = ram_peek(0x0084);
      /*$E380*/ branchTarget = true; push16(0xe382); block_id = 2070;
      break;
    case 1652:  // $E383
      /*$E383*/ CYCLES(0xe383, 48);
      /*$E385*/ push8(ram_peek(0x00b9));
      /*$E388*/ push8(ram_peek(0x00b8));
      /*$E389*/ tmp5_U8 = s_y;
                tmp2_U8 = peek((ram_peek16al(0x008a) + tmp5_U8));
      /*$E38B*/ ram_poke(0x00b8, tmp2_U8);
      /*$E38D*/ tmp5_U8 = (uint8_t)(tmp5_U8 + 0x01);
                s_y = tmp5_U8;
      /*$E38E*/ tmp5_U8 = peek((ram_peek16al(0x008a) + tmp5_U8));
      /*$E390*/ ram_poke(0x00b9, tmp5_U8);
      /*$E394*/ push8(ram_peek(0x0084));
      /*$E395*/ tmp5_U8 = ram_peek(0x0083);
                s_a = tmp5_U8;
      /*$E397*/ push8(tmp5_U8);
      /*$E398*/ branchTarget = true; push16(0xe39a); block_id = 1259;
      break;
    case 1653:  // $E39B
      /*$E39B*/ CYCLES(0xe39b, 20);
                tmp5_U8 = pop8();
      /*$E39C*/ ram_poke(0x008a, tmp5_U8);
      /*$E39E*/ tmp5_U8 = pop8();
                s_status_not_z = tmp5_U8;
      /*$E39F*/ ram_poke(0x008b, tmp5_U8);
      /*$E3A1*/ branchTarget = true; push16(0xe3a3); block_id = 5;
      break;
    case 1654:  // $E3A4
      /*$E3A4*/ CYCLES(0xe3a4, 2);
                branchTarget = true; block_id = !s_status_not_z ? 1655 : 1656;
      break;
    case 1655:  // $E3A4
      /*$E3A4*/ CYCLES_EDGE(0xe3a4, 1);
      /*$E3A9*/ CYCLES(0xe3a9, 80);
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
    case 1656:  // $E3A6
      /*$E3A6*/ CYCLES(0xe3a6, 3);
                branchTarget = true; block_id = 1378;
      break;
    case 1657:  // $E3D5
      /*$E3D5*/ CYCLES(0xe3d5, 18);
                tmp2_U8 = ram_peek(0x00a0);
                s_x = tmp2_U8;
      /*$E3D7*/ tmp1_U8 = ram_peek(0x00a1);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
      /*$E3D9*/ ram_poke(0x008c, tmp2_U8);
      /*$E3DB*/ ram_poke(0x008d, tmp1_U8);
      /*$E3DD*/ branchTarget = true; push16(0xe3df); block_id = 1685;
      break;
    case 1658:  // $E3E0
      /*$E3E0*/ CYCLES(0xe3e0, 15);
                ram_poke(0x009e, s_x);
      /*$E3E2*/ ram_poke(0x009f, s_y);
      /*$E3E4*/ ram_poke(0x009d, s_a);
      /*$E3E6*/ branchTarget = true; block_id = find_block_id_func_t001(0xe3e6, pop16() + 1);;
      break;
    case 1659:  // $E3E7
      /*$E3E7*/ CYCLES(0xe3e7, 22);
      /*$E3E9*/ ram_poke(0x000d, 0x22);
      /*$E3EB*/ ram_poke(0x000e, 0x22);
      /*$E3ED*/ tmp5_U8 = s_a;
                ram_poke(0x00ab, tmp5_U8);
      /*$E3EF*/ tmp2_U8 = s_y;
                ram_poke(0x00ac, tmp2_U8);
      /*$E3F1*/ ram_poke(0x009e, tmp5_U8);
      /*$E3F3*/ ram_poke(0x009f, tmp2_U8);
      /*$E3F5*/ s_y = 0xff;
                block_id = 1660;
      break;
    case 1660:  // $E3F7
      /*$E3F7*/ CYCLES(0xe3f7, 9);
                tmp5_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp5_U8;
      /*$E3F8*/ tmp5_U8 = peek((ram_peek16(0x00ab) + tmp5_U8));
                s_a = tmp5_U8;
      /*$E3FA*/ branchTarget = true; block_id = !tmp5_U8 ? 1661 : 1662;
      break;
    case 1661:  // $E3FA
      /*$E3FA*/ CYCLES_EDGE(0xe3fa, 1);
                branchTarget = true; block_id = 1668;
      break;
    case 1662:  // $E3FC
      /*$E3FC*/ CYCLES(0xe3fc, 5);
      /*$E3FE*/ branchTarget = true; block_id = !(s_a != ram_peek(0x000d)) ? 1663 : 1664;
      break;
    case 1663:  // $E3FE
      /*$E3FE*/ CYCLES_EDGE(0xe3fe, 1);
                branchTarget = true; block_id = 1666;
      break;
    case 1664:  // $E400
      /*$E400*/ CYCLES(0xe400, 5);
      /*$E402*/ branchTarget = true; block_id = (s_a != ram_peek(0x000e)) ? 1665 : 1666;
      break;
    case 1665:  // $E402
      /*$E402*/ CYCLES_EDGE(0xe402, 1);
                branchTarget = true; block_id = 1660;
      break;
    case 1666:  // $E404
      /*$E404*/ CYCLES(0xe404, 4);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0x22);
      /*$E406*/ branchTarget = true; block_id = !(tmp5_U8 != 0x22) ? 1667 : 1668;
      break;
    case 1667:  // $E406
      /*$E406*/ CYCLES_EDGE(0xe406, 1);
                branchTarget = true; block_id = 1669;
      break;
    case 1668:  // $E408
      /*$E408*/ CYCLES(0xe408, 2);
                s_status_c = 0x00;
                block_id = 1669;
      break;
    case 1669:  // $E409
      /*$E409*/ CYCLES(0xe409, 16);
                tmp5_U8 = s_y;
                ram_poke(0x009d, tmp5_U8);
      /*$E40B*/ s_a = tmp5_U8;
      /*$E40C*/ block_id = s_status_d ? 1671 : 1670;
      break;
    case 1670:  // $E40C
      /*$E40C*/ tmp3_U16 = s_a;
                tmp6_U16 = ram_peek(0x00ab);
                tmp4_U16 = (tmp3_U16 + tmp6_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)tmp6_U16);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1672;
      break;
    case 1671:  // $E40C
      /*$E40C*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x00ab), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1672;
      break;
    case 1672:  // $E40E
      /*$E40E*/ ram_poke(0x00ad, s_a);
      /*$E410*/ s_x = ram_peek(0x00ac);
      /*$E412*/ branchTarget = true; block_id = !s_status_c ? 1673 : 1674;
      break;
    case 1673:  // $E412
      /*$E412*/ CYCLES_EDGE(0xe412, 1);
                branchTarget = true; block_id = 1675;
      break;
    case 1674:  // $E414
      /*$E414*/ CYCLES(0xe414, 2);
                s_x = (uint8_t)(s_x + 0x01);
                block_id = 1675;
      break;
    case 1675:  // $E415
      /*$E415*/ CYCLES(0xe415, 8);
                ram_poke(0x00ae, s_x);
      /*$E417*/ tmp5_U8 = ram_peek(0x00ac);
                s_a = tmp5_U8;
      /*$E419*/ branchTarget = true; block_id = !tmp5_U8 ? 1676 : 1677;
      break;
    case 1676:  // $E419
      /*$E419*/ CYCLES_EDGE(0xe419, 1);
                branchTarget = true; block_id = 1679;
      break;
    case 1677:  // $E41B
      /*$E41B*/ CYCLES(0xe41b, 4);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0x02);
      /*$E41D*/ branchTarget = true; block_id = (tmp5_U8 != 0x02) ? 1678 : 1679;
      break;
    case 1678:  // $E41D
      /*$E41D*/ CYCLES_EDGE(0xe41d, 1);
                branchTarget = true; block_id = 1681;
      break;
    case 1679:  // $E41F
      /*$E41F*/ CYCLES(0xe41f, 8);
                s_a = s_y;
      /*$E420*/ branchTarget = true; push16(0xe422); block_id = 1657;
      break;
    case 1680:  // $E423
      /*$E423*/ CYCLES(0xe423, 12);
                s_x = ram_peek(0x00ab);
      /*$E425*/ s_y = ram_peek(0x00ac);
      /*$E427*/ FUNC_MOVSTR(0xe429);
                branchTarget = true; block_id = 1681;
      break;
    case 1681:  // $E42A
      /*$E42A*/ CYCLES(0xe42a, 7);
                tmp5_U8 = ram_peek(0x0052);
                s_x = tmp5_U8;
      /*$E42C*/ s_status_c = (tmp5_U8 >= 0x5e);
      /*$E42E*/ branchTarget = true; block_id = (tmp5_U8 != 0x5e) ? 1682 : 1683;
      break;
    case 1682:  // $E42E
      /*$E42E*/ CYCLES_EDGE(0xe42e, 1);
      /*$E435*/ CYCLES(0xe435, 52);
      /*$E437*/ tmp5_U8 = s_x;
                ram_poke(tmp5_U8, ram_peek(0x009d));
      /*$E43B*/ ram_poke((uint8_t)(0x01 + tmp5_U8), ram_peek(0x009e));
      /*$E43D*/ tmp2_U8 = ram_peek(0x009f);
                s_a = tmp2_U8;
      /*$E43F*/ ram_poke((uint8_t)(0x02 + tmp5_U8), tmp2_U8);
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
    case 1683:  // $E430
      /*$E430*/ CYCLES(0xe430, 2);
                s_x = 0xbf;
                block_id = 1684;
      break;
    case 1684:  // $E432
      /*$E432*/ CYCLES(0xe432, 3);
                branchTarget = true; block_id = 887;
      break;
    case 1685:  // $E452
      /*$E452*/ CYCLES(0xe452, 5);
                ram_poke(0x0013, (ram_peek(0x0013) >> 0x01));
                block_id = 1686;
      break;
    case 1686:  // $E454
      /*$E454*/ CYCLES(0xe454, 15);
                tmp5_U8 = s_a;
                push8(tmp5_U8);
      /*$E455*/ s_a = (tmp5_U8 ^ 0xff);
      /*$E457*/ s_status_c = 0x01;
      /*$E458*/ block_id = s_status_d ? 1688 : 1687;
      break;
    case 1687:  // $E458
      /*$E458*/ tmp4_U16 = s_a;
                tmp6_U16 = ram_peek(0x006f);
                tmp3_U16 = (tmp4_U16 + tmp6_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp6_U16);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1689;
      break;
    case 1688:  // $E458
      /*$E458*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x006f), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1689;
      break;
    case 1689:  // $E45A
      /*$E45A*/ s_y = ram_peek(0x0070);
      /*$E45C*/ branchTarget = true; block_id = s_status_c ? 1690 : 1691;
      break;
    case 1690:  // $E45C
      /*$E45C*/ CYCLES_EDGE(0xe45c, 1);
                branchTarget = true; block_id = 1692;
      break;
    case 1691:  // $E45E
      /*$E45E*/ CYCLES(0xe45e, 2);
                s_y = (uint8_t)(s_y - 0x01);
                block_id = 1692;
      break;
    case 1692:  // $E45F
      /*$E45F*/ CYCLES(0xe45f, 5);
                tmp5_U8 = s_y;
                tmp2_U8 = ram_peek(0x006e);
                s_status_not_z = (tmp5_U8 != tmp2_U8);
                tmp2_U8 = tmp5_U8 >= tmp2_U8;
                s_status_c = tmp2_U8;
      /*$E461*/ branchTarget = true; block_id = !tmp2_U8 ? 1693 : 1694;
      break;
    case 1693:  // $E461
      /*$E461*/ CYCLES_EDGE(0xe461, 1);
                branchTarget = true; block_id = 1699;
      break;
    case 1694:  // $E463
      /*$E463*/ CYCLES(0xe463, 2);
                branchTarget = true; block_id = s_status_not_z ? 1695 : 1696;
      break;
    case 1695:  // $E463
      /*$E463*/ CYCLES_EDGE(0xe463, 1);
                branchTarget = true; block_id = 1698;
      break;
    case 1696:  // $E465
      /*$E465*/ CYCLES(0xe465, 5);
                tmp2_U8 = s_a >= ram_peek(0x006d);
                s_status_c = tmp2_U8;
      /*$E467*/ branchTarget = true; block_id = !tmp2_U8 ? 1697 : 1698;
      break;
    case 1697:  // $E467
      /*$E467*/ CYCLES_EDGE(0xe467, 1);
                branchTarget = true; block_id = 1699;
      break;
    case 1698:  // $E469
      /*$E469*/ CYCLES(0xe469, 24);
                tmp2_U8 = s_a;
                ram_poke(0x006f, tmp2_U8);
      /*$E46B*/ tmp5_U8 = s_y;
                ram_poke(0x0070, tmp5_U8);
      /*$E46D*/ ram_poke(0x0071, tmp2_U8);
      /*$E46F*/ ram_poke(0x0072, tmp5_U8);
      /*$E471*/ s_x = tmp2_U8;
      /*$E472*/ tmp2_U8 = pop8();
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$E473*/ branchTarget = true; block_id = find_block_id_func_t001(0xe473, pop16() + 1);;
      break;
    case 1699:  // $E474
      /*$E474*/ CYCLES(0xe474, 7);
                s_x = 0x4d;
      /*$E476*/ tmp2_U8 = ram_peek(0x0013);
                s_a = tmp2_U8;
      /*$E478*/ branchTarget = true; block_id = (tmp2_U8 & 0x80) ? 1700 : 1701;
      break;
    case 1700:  // $E478
      /*$E478*/ CYCLES_EDGE(0xe478, 1);
                branchTarget = true; block_id = 1684;
      break;
    case 1701:  // $E47A
      /*$E47A*/ CYCLES(0xe47a, 6);
                FUNC_GARBAGE(0xe47c);
      /*$E47D*/ CYCLES(0xe47d, 11);
      /*$E47F*/ ram_poke(0x0013, 0x80);
      /*$E481*/ tmp2_U8 = pop8();
                s_a = tmp2_U8;
      /*$E482*/ branchTarget = true; block_id = tmp2_U8 ? 1702 : 2640;
      break;
    case 1702:  // $E482
      /*$E482*/ CYCLES_EDGE(0xe482, 1);
                branchTarget = true; block_id = 1686;
      break;
    case 1703:  // $E484
      /*$E484*/ CYCLES(0xe484, 6);
                s_x = ram_peek(0x0073);
      /*$E486*/ s_a = ram_peek(0x0074);
                block_id = 1704;
      break;
    case 1704:  // $E488
      /*$E488*/ CYCLES(0xe488, 33);
                ram_poke(0x006f, s_x);
      /*$E48A*/ ram_poke(0x0070, s_a);
      /*$E48C*/ s_y = 0x00;
      /*$E48E*/ ram_poke(0x008b, 0x00);
      /*$E494*/ ram_poke(0x009b, ram_peek(0x006d));
      /*$E496*/ ram_poke(0x009c, ram_peek(0x006e));
      /*$E498*/ s_a = 0x55;
      /*$E49C*/ ram_poke(0x005e, 0x55);
      /*$E49E*/ ram_poke(0x005f, 0x00);
                block_id = 1705;
      break;
    case 1705:  // $E4A0
      /*$E4A0*/ CYCLES(0xe4a0, 5);
      /*$E4A2*/ branchTarget = true; block_id = !(s_a != ram_peek(0x0052)) ? 1706 : 1707;
      break;
    case 1706:  // $E4A2
      /*$E4A2*/ CYCLES_EDGE(0xe4a2, 1);
                branchTarget = true; block_id = 1709;
      break;
    case 1707:  // $E4A4
      /*$E4A4*/ CYCLES(0xe4a4, 6);
                FUNC_CHKVAR(0xe4a6);
      /*$E4A7*/ CYCLES(0xe4a7, 2);
                branchTarget = true; block_id = !s_status_not_z ? 1708 : 1709;
      break;
    case 1708:  // $E4A7
      /*$E4A7*/ CYCLES_EDGE(0xe4a7, 1);
                branchTarget = true; block_id = 1705;
      break;
    case 1709:  // $E4A9
      /*$E4A9*/ CYCLES(0xe4a9, 17);
      /*$E4AB*/ ram_poke(0x008f, 0x07);
      /*$E4AD*/ tmp2_U8 = ram_peek(0x0069);
                s_a = tmp2_U8;
      /*$E4AF*/ tmp5_U8 = ram_peek(0x006a);
                s_x = tmp5_U8;
      /*$E4B1*/ ram_poke(0x005e, tmp2_U8);
      /*$E4B3*/ ram_poke(0x005f, tmp5_U8);
                block_id = 1710;
      break;
    case 1710:  // $E4B5
      /*$E4B5*/ CYCLES(0xe4b5, 5);
      /*$E4B7*/ branchTarget = true; block_id = (s_x != ram_peek(0x006c)) ? 1711 : 1712;
      break;
    case 1711:  // $E4B7
      /*$E4B7*/ CYCLES_EDGE(0xe4b7, 1);
                branchTarget = true; block_id = 1714;
      break;
    case 1712:  // $E4B9
      /*$E4B9*/ CYCLES(0xe4b9, 5);
      /*$E4BB*/ branchTarget = true; block_id = !(s_a != ram_peek(0x006b)) ? 1713 : 1714;
      break;
    case 1713:  // $E4BB
      /*$E4BB*/ CYCLES_EDGE(0xe4bb, 1);
                branchTarget = true; block_id = 1716;
      break;
    case 1714:  // $E4BD
      /*$E4BD*/ CYCLES(0xe4bd, 6);
                FUNC_CHKSMPLVAR(0xe4bf);
      /*$E4C0*/ CYCLES(0xe4c0, 2);
                branchTarget = true; block_id = !s_status_not_z ? 1715 : 1716;
      break;
    case 1715:  // $E4C0
      /*$E4C0*/ CYCLES_EDGE(0xe4c0, 1);
                branchTarget = true; block_id = 1710;
      break;
    case 1716:  // $E4C2
      /*$E4C2*/ CYCLES(0xe4c2, 11);
                ram_poke(0x0094, s_a);
      /*$E4C4*/ ram_poke(0x0095, s_x);
      /*$E4C8*/ ram_poke(0x008f, 0x03);
                block_id = 1717;
      break;
    case 1717:  // $E4CA
      /*$E4CA*/ CYCLES(0xe4ca, 6);
                s_a = ram_peek(0x0094);
      /*$E4CC*/ s_x = ram_peek(0x0095);
                block_id = 1718;
      break;
    case 1718:  // $E4CE
      /*$E4CE*/ CYCLES(0xe4ce, 5);
                tmp5_U8 = s_x;
                tmp2_U8 = ram_peek(0x006e);
                s_status_c = (tmp5_U8 >= tmp2_U8);
      /*$E4D0*/ branchTarget = true; block_id = (tmp5_U8 != tmp2_U8) ? 1719 : 1720;
      break;
    case 1719:  // $E4D0
      /*$E4D0*/ CYCLES_EDGE(0xe4d0, 1);
                branchTarget = true; block_id = 1723;
      break;
    case 1720:  // $E4D2
      /*$E4D2*/ CYCLES(0xe4d2, 5);
                tmp5_U8 = s_a;
                tmp2_U8 = ram_peek(0x006d);
                s_status_c = (tmp5_U8 >= tmp2_U8);
      /*$E4D4*/ branchTarget = true; block_id = (tmp5_U8 != tmp2_U8) ? 1721 : 1722;
      break;
    case 1721:  // $E4D4
      /*$E4D4*/ CYCLES_EDGE(0xe4d4, 1);
                branchTarget = true; block_id = 1723;
      break;
    case 1722:  // $E4D6
      /*$E4D6*/ CYCLES(0xe4d6, 3);
      /*$E562*/ CYCLES(0xe562, 5);
      /*$E564*/ branchTarget = true; block_id = !ram_peek(0x008b) ? 1776 : 1777;
      break;
    case 1723:  // $E4D9
      /*$E4D9*/ CYCLES(0xe4d9, 57);
                ram_poke(0x005e, s_a);
      /*$E4DB*/ ram_poke(0x005f, s_x);
      /*$E4DF*/ tmp2_U8 = peek(ram_peek16al(0x005e));
      /*$E4E1*/ s_x = tmp2_U8;
      /*$E4E3*/ tmp2_U8 = peek((ram_peek16al(0x005e) + 0x0001));
      /*$E4E5*/ tmp5_U8 = s_status_d;
                push8((s_status_c | ((tmp2_U8 == 0) << 1) | (s_status_i << 2) | (tmp5_U8 << 3) | STATUS_B | (s_status_v << 6) | (tmp2_U8 & 0x80)));
      /*$E4E6*/ s_y = 0x02;
      /*$E4E7*/ tmp2_U8 = peek((ram_peek16al(0x005e) + 0x0002));
                s_a = tmp2_U8;
      /*$E4E9*/ block_id = tmp5_U8 ? 1725 : 1724;
      break;
    case 1724:  // $E4E9
      /*$E4E9*/ tmp3_U16 = (s_a + ram_peek(0x0094)) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1726;
      break;
    case 1725:  // $E4E9
      /*$E4E9*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x0094), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 1726;
      break;
    case 1726:  // $E4EB
      /*$E4EB*/ ram_poke(0x0094, s_a);
      /*$E4ED*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$E4EE*/ tmp2_U8 = peek((ram_peek16al(0x005e) + tmp2_U8));
                s_a = tmp2_U8;
      /*$E4F0*/ block_id = s_status_d ? 1728 : 1727;
      break;
    case 1727:  // $E4F0
      /*$E4F0*/ s_a = (uint8_t)((s_a + ram_peek(0x0095)) + s_status_c);
                block_id = 1729;
      break;
    case 1728:  // $E4F0
      /*$E4F0*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x0095), s_status_c));
                block_id = 1729;
      break;
    case 1729:  // $E4F2
      /*$E4F2*/ ram_poke(0x0095, s_a);
      /*$E4F4*/ tmp2_U8 = pop8();
                s_status_i = ((tmp2_U8 & 0x04) != 0);
                s_status_d = ((tmp2_U8 & 0x08) != 0);
                s_status_b = 0x00;
                s_status_v = ((tmp2_U8 & 0x40) != 0);
      /*$E4F5*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1730 : 1731;
      break;
    case 1730:  // $E4F5
      /*$E4F5*/ CYCLES_EDGE(0xe4f5, 1);
                branchTarget = true; block_id = 1717;
      break;
    case 1731:  // $E4F7
      /*$E4F7*/ CYCLES(0xe4f7, 4);
      /*$E4F8*/ branchTarget = true; block_id = (s_x & 0x80) ? 1732 : 1733;
      break;
    case 1732:  // $E4F8
      /*$E4F8*/ CYCLES_EDGE(0xe4f8, 1);
                branchTarget = true; block_id = 1717;
      break;
    case 1733:  // $E4FA
      /*$E4FA*/ CYCLES(0xe4fa, 21);
      /*$E4FB*/ tmp2_U8 = peek((ram_peek16al(0x005e) + (uint8_t)(s_y + 0x01)));
      /*$E4FD*/ s_y = 0x00;
      /*$E4FF*/ tmp3_U16 = tmp2_U8 << 0x01;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
      /*$E500*/ block_id = s_status_d ? 1735 : 1734;
      break;
    case 1734:  // $E500
      /*$E500*/ tmp3_U16 = (s_a + 0x0005) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1736;
      break;
    case 1735:  // $E500
      /*$E500*/ tmp3_U16 = adc_dec16(s_a, 0x05, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 1736;
      break;
    case 1736:  // $E502
      /*$E502*/ block_id = s_status_d ? 1738 : 1737;
      break;
    case 1737:  // $E502
      /*$E502*/ tmp3_U16 = s_a;
                tmp6_U16 = ram_peek(0x005e);
                tmp4_U16 = (tmp3_U16 + tmp6_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)tmp6_U16);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1739;
      break;
    case 1738:  // $E502
      /*$E502*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x005e), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 1739;
      break;
    case 1739:  // $E504
      /*$E504*/ ram_poke(0x005e, s_a);
      /*$E506*/ branchTarget = true; block_id = !s_status_c ? 1740 : 1741;
      break;
    case 1740:  // $E506
      /*$E506*/ CYCLES_EDGE(0xe506, 1);
                branchTarget = true; block_id = 1742;
      break;
    case 1741:  // $E508
      /*$E508*/ CYCLES(0xe508, 5);
                ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) + 0x01));
                block_id = 1742;
      break;
    case 1742:  // $E50A
      /*$E50A*/ CYCLES(0xe50a, 3);
                s_x = ram_peek(0x005f);
                block_id = 1743;
      break;
    case 1743:  // $E50C
      /*$E50C*/ CYCLES(0xe50c, 5);
      /*$E50E*/ branchTarget = true; block_id = (s_x != ram_peek(0x0095)) ? 1744 : 1745;
      break;
    case 1744:  // $E50E
      /*$E50E*/ CYCLES_EDGE(0xe50e, 1);
                branchTarget = true; block_id = 1747;
      break;
    case 1745:  // $E510
      /*$E510*/ CYCLES(0xe510, 5);
      /*$E512*/ branchTarget = true; block_id = !(s_a != ram_peek(0x0094)) ? 1746 : 1747;
      break;
    case 1746:  // $E512
      /*$E512*/ CYCLES_EDGE(0xe512, 1);
                branchTarget = true; block_id = 1718;
      break;
    case 1747:  // $E514
      /*$E514*/ CYCLES(0xe514, 6);
                FUNC_CHKVAR(0xe516);
      /*$E517*/ CYCLES(0xe517, 2);
                branchTarget = true; block_id = !s_status_not_z ? 1748 : 2641;
      break;
    case 1748:  // $E517
      /*$E517*/ CYCLES_EDGE(0xe517, 1);
                branchTarget = true; block_id = 1743;
      break;
    case 1749:  // $E519
      /*$E519*/ CYCLES(0xe519, 7);
                tmp5_U8 = peek((ram_peek16al(0x005e) + s_y));
      /*$E51B*/ branchTarget = true; block_id = (tmp5_U8 & 0x80) ? 1750 : 1751;
      break;
    case 1750:  // $E51B
      /*$E51B*/ CYCLES_EDGE(0xe51b, 1);
                branchTarget = true; block_id = 1769;
      break;
    case 1751:  // $E51D
      /*$E51D*/ CYCLES(0xe51d, 9);
                tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$E51E*/ tmp2_U8 = peek((ram_peek16al(0x005e) + tmp2_U8));
      /*$E520*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1752 : 1753;
      break;
    case 1752:  // $E520
      /*$E520*/ CYCLES_EDGE(0xe520, 1);
                branchTarget = true; block_id = 1769;
      break;
    case 1753:  // $E522
      /*$E522*/ CYCLES(0xe522, 2);
                s_y = (uint8_t)(s_y + 0x01);
                FUNC_CHKVAR(0x0000);
                block_id = find_block_id_func_t001(0xe522, pop16() + 1);;
      break;
    case 1754:  // $E523
      /*$E523*/ CYCLES(0xe523, 7);
                tmp5_U8 = peek((ram_peek16al(0x005e) + s_y));
      /*$E525*/ branchTarget = true; block_id = !tmp5_U8 ? 1755 : 1756;
      break;
    case 1755:  // $E525
      /*$E525*/ CYCLES_EDGE(0xe525, 1);
                branchTarget = true; block_id = 1769;
      break;
    case 1756:  // $E527
      /*$E527*/ CYCLES(0xe527, 21);
                tmp2_U8 = s_y;
      /*$E528*/ tmp5_U8 = peek((ram_peek16al(0x005e) + (uint8_t)(tmp2_U8 + 0x01)));
      /*$E52A*/ s_x = tmp5_U8;
      /*$E52C*/ tmp2_U8 = peek((ram_peek16al(0x005e) + (uint8_t)(tmp2_U8 + 0x02)));
                s_a = tmp2_U8;
      /*$E52E*/ tmp5_U8 = ram_peek(0x0070);
                s_status_not_z = (tmp2_U8 != tmp5_U8);
      /*$E530*/ branchTarget = true; block_id = !(tmp2_U8 >= tmp5_U8) ? 1757 : 1758;
      break;
    case 1757:  // $E530
      /*$E530*/ CYCLES_EDGE(0xe530, 1);
                branchTarget = true; block_id = 1762;
      break;
    case 1758:  // $E532
      /*$E532*/ CYCLES(0xe532, 2);
                branchTarget = true; block_id = s_status_not_z ? 1759 : 1760;
      break;
    case 1759:  // $E532
      /*$E532*/ CYCLES_EDGE(0xe532, 1);
                branchTarget = true; block_id = 1769;
      break;
    case 1760:  // $E534
      /*$E534*/ CYCLES(0xe534, 5);
      /*$E536*/ branchTarget = true; block_id = (s_x >= ram_peek(0x006f)) ? 1761 : 1762;
      break;
    case 1761:  // $E536
      /*$E536*/ CYCLES_EDGE(0xe536, 1);
                branchTarget = true; block_id = 1769;
      break;
    case 1762:  // $E538
      /*$E538*/ CYCLES(0xe538, 5);
                tmp5_U8 = s_a;
                tmp2_U8 = ram_peek(0x009c);
                s_status_not_z = (tmp5_U8 != tmp2_U8);
      /*$E53A*/ branchTarget = true; block_id = !(tmp5_U8 >= tmp2_U8) ? 1763 : 1764;
      break;
    case 1763:  // $E53A
      /*$E53A*/ CYCLES_EDGE(0xe53a, 1);
                branchTarget = true; block_id = 1769;
      break;
    case 1764:  // $E53C
      /*$E53C*/ CYCLES(0xe53c, 2);
                branchTarget = true; block_id = s_status_not_z ? 1765 : 1766;
      break;
    case 1765:  // $E53C
      /*$E53C*/ CYCLES_EDGE(0xe53c, 1);
                branchTarget = true; block_id = 1768;
      break;
    case 1766:  // $E53E
      /*$E53E*/ CYCLES(0xe53e, 5);
      /*$E540*/ branchTarget = true; block_id = !(s_x >= ram_peek(0x009b)) ? 1767 : 1768;
      break;
    case 1767:  // $E540
      /*$E540*/ CYCLES_EDGE(0xe540, 1);
                branchTarget = true; block_id = 1769;
      break;
    case 1768:  // $E542
      /*$E542*/ CYCLES(0xe542, 24);
                ram_poke(0x009b, s_x);
      /*$E544*/ ram_poke(0x009c, s_a);
      /*$E54A*/ ram_poke(0x008a, ram_peek(0x005e));
      /*$E54C*/ ram_poke(0x008b, ram_peek(0x005f));
      /*$E550*/ ram_poke(0x0091, ram_peek(0x008f));
                block_id = 1769;
      break;
    case 1769:  // $E552
      /*$E552*/ CYCLES(0xe552, 13);
                s_a = ram_peek(0x008f);
      /*$E554*/ s_status_c = 0x00;
      /*$E555*/ block_id = s_status_d ? 1771 : 1770;
      break;
    case 1770:  // $E555
      /*$E555*/ tmp4_U16 = s_a;
                tmp6_U16 = ram_peek(0x005e);
                tmp3_U16 = (tmp4_U16 + tmp6_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp6_U16);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1772;
      break;
    case 1771:  // $E555
      /*$E555*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x005e), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 1772;
      break;
    case 1772:  // $E557
      /*$E557*/ ram_poke(0x005e, s_a);
      /*$E559*/ branchTarget = true; block_id = !s_status_c ? 1773 : 1774;
      break;
    case 1773:  // $E559
      /*$E559*/ CYCLES_EDGE(0xe559, 1);
                branchTarget = true; block_id = 1775;
      break;
    case 1774:  // $E55B
      /*$E55B*/ CYCLES(0xe55b, 5);
                ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) + 0x01));
                block_id = 1775;
      break;
    case 1775:  // $E55D
      /*$E55D*/ CYCLES(0xe55d, 11);
                s_x = ram_peek(0x005f);
      /*$E55F*/ s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_y = 0x00;
      /*$E561*/ branchTarget = true; block_id = find_block_id_func_t001(0xe561, pop16() + 1);;
      break;
    case 1776:  // $E564
      /*$E564*/ CYCLES_EDGE(0xe564, 1);
                branchTarget = true; block_id = 1775;
      break;
    case 1777:  // $E566
      /*$E566*/ CYCLES(0xe566, 49);
      /*$E56A*/ s_status_c = 0x00;
                tmp2_U8 = (ram_peek(0x0091) & 0x04) >> 0x01;
      /*$E56C*/ ram_poke(0x0091, tmp2_U8);
      /*$E56E*/ tmp2_U8 = peek((ram_peek16al(0x008a) + tmp2_U8));
                s_a = tmp2_U8;
      /*$E570*/ block_id = s_status_d ? 1779 : 1778;
      break;
    case 1778:  // $E570
      /*$E570*/ tmp3_U16 = (s_a + ram_peek(0x009b)) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1780;
      break;
    case 1779:  // $E570
      /*$E570*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x009b), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 1780;
      break;
    case 1780:  // $E572
      /*$E572*/ ram_poke(0x0096, s_a);
      /*$E574*/ s_a = ram_peek(0x009c);
      /*$E576*/ block_id = s_status_d ? 1782 : 1781;
      break;
    case 1781:  // $E576
      /*$E576*/ tmp3_U16 = s_a;
                tmp6_U16 = tmp3_U16 + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1783;
      break;
    case 1782:  // $E576
      /*$E576*/ tmp6_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 1783;
      break;
    case 1783:  // $E578
      /*$E578*/ ram_poke(0x0097, s_a);
      /*$E57E*/ ram_poke(0x0094, ram_peek(0x006f));
      /*$E580*/ ram_poke(0x0095, ram_peek(0x0070));
      /*$E582*/ FUNC_MVBLKUP2(0xe584);
      /*$E585*/ CYCLES(0xe585, 35);
                tmp2_U8 = ram_peek(0x0091);
      /*$E588*/ tmp5_U8 = ram_peek(0x0094);
      /*$E58A*/ poke((ram_peek16al(0x008a) + (uint8_t)(tmp2_U8 + 0x01)), tmp5_U8);
      /*$E58C*/ s_x = tmp5_U8;
      /*$E58D*/ ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) + 0x01));
      /*$E58F*/ tmp5_U8 = ram_peek(0x0095);
                s_a = tmp5_U8;
      /*$E592*/ poke((ram_peek16al(0x008a) + (uint8_t)(tmp2_U8 + 0x02)), tmp5_U8);
      /*$E594*/ branchTarget = true; block_id = 1704;
      break;
    case 1784:  // $E5A0
      /*$E5A0*/ CYCLES(0xe5a0, 6);
                branchTarget = true; push16(0xe5a2); block_id = 1261;
      break;
    case 1785:  // $E5A3
      /*$E5A3*/ CYCLES(0xe5a3, 30);
                tmp2_U8 = pop8();
      /*$E5A4*/ ram_poke(0x00ab, tmp2_U8);
      /*$E5A6*/ tmp2_U8 = pop8();
      /*$E5A7*/ ram_poke(0x00ac, tmp2_U8);
      /*$E5A9*/ s_y = 0x00;
      /*$E5AB*/ tmp2_U8 = peek(ram_peek16(0x00ab));
                s_a = tmp2_U8;
      /*$E5AD*/ s_status_c = 0x00;
      /*$E5AE*/ block_id = s_status_d ? 1787 : 1786;
      break;
    case 1786:  // $E5AE
      /*$E5AE*/ tmp6_U16 = s_a;
                tmp2_U8 = peek((ram_peek16al(0x00a0) + s_y));
                tmp3_U16 = tmp2_U8;
                tmp4_U16 = (tmp6_U16 + tmp3_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)tmp3_U16);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1788;
      break;
    case 1787:  // $E5AE
      /*$E5AE*/ tmp2_U8 = peek((ram_peek16al(0x00a0) + s_y));
                tmp4_U16 = adc_dec16(s_a, tmp2_U8, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 1788;
      break;
    case 1788:  // $E5B0
      /*$E5B0*/ branchTarget = true; block_id = !s_status_c ? 1789 : 1790;
      break;
    case 1789:  // $E5B0
      /*$E5B0*/ CYCLES_EDGE(0xe5b0, 1);
      /*$E5B7*/ CYCLES(0xe5b7, 6);
                branchTarget = true; push16(0xe5b9); block_id = 1657;
      break;
    case 1790:  // $E5B2
      /*$E5B2*/ CYCLES(0xe5b2, 5);
                s_x = 0xb0;
      /*$E5B4*/ branchTarget = true; block_id = 887;
      break;
    case 1791:  // $E5BA
      /*$E5BA*/ CYCLES(0xe5ba, 6);
                FUNC_MOVINS(0xe5bc);
      /*$E5BD*/ CYCLES(0xe5bd, 12);
                s_a = ram_peek(0x008c);
      /*$E5BF*/ s_y = ram_peek(0x008d);
      /*$E5C1*/ FUNC_FRETMP(0xe5c3);
      /*$E5C4*/ CYCLES(0xe5c4, 6);
                FUNC_MOVSTR1(0xe5c6);
      /*$E5C7*/ CYCLES(0xe5c7, 12);
                s_a = ram_peek(0x00ab);
      /*$E5C9*/ s_y = ram_peek(0x00ac);
      /*$E5CB*/ FUNC_FRETMP(0xe5cd);
      /*$E5CE*/ CYCLES(0xe5ce, 6);
                branchTarget = true; push16(0xe5d0); block_id = 1681;
      break;
    case 1792:  // $E5D1
      /*$E5D1*/ CYCLES(0xe5d1, 3);
                branchTarget = true; block_id = 1275;
      break;
    case 1793:  // $E5D4
      /*$E5D4*/ CYCLES(0xe5d4, 32);
      /*$E5D6*/ tmp1_U8 = peek(ram_peek16(0x00ab));
      /*$E5D8*/ push8(tmp1_U8);
      /*$E5DA*/ tmp1_U8 = peek((ram_peek16(0x00ab) + 0x0001));
      /*$E5DC*/ s_x = tmp1_U8;
      /*$E5DE*/ tmp1_U8 = peek((ram_peek16(0x00ab) + 0x0002));
      /*$E5E0*/ s_y = tmp1_U8;
      /*$E5E1*/ tmp1_U8 = pop8();
                s_a = tmp1_U8;
                FUNC_MOVSTR(0x0000);
                block_id = find_block_id_func_t001(0xe5e1, pop16() + 1);;
      break;
    case 1794:  // $E5E2
      /*$E5E2*/ CYCLES(0xe5e2, 6);
                ram_poke(0x005e, s_x);
      /*$E5E4*/ ram_poke(0x005f, s_y);
                FUNC_MOVSTR1(0x0000);
                block_id = find_block_id_func_t001(0xe5e4, pop16() + 1);;
      break;
    case 1795:  // $E5E6
      /*$E5E6*/ CYCLES(0xe5e6, 4);
                tmp5_U8 = s_a;
                s_y = tmp5_U8;
      /*$E5E7*/ branchTarget = true; block_id = !tmp5_U8 ? 1796 : 1797;
      break;
    case 1796:  // $E5E7
      /*$E5E7*/ CYCLES_EDGE(0xe5e7, 1);
                branchTarget = true; block_id = 1801;
      break;
    case 1797:  // $E5E9
      /*$E5E9*/ CYCLES(0xe5e9, 3);
                push8(s_a);
                block_id = 1798;
      break;
    case 1798:  // $E5EA
      /*$E5EA*/ CYCLES(0xe5ea, 17);
                tmp5_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp5_U8;
      /*$E5EB*/ tmp2_U8 = peek((ram_peek16al(0x005e) + tmp5_U8));
      /*$E5ED*/ poke((ram_peek16(0x0071) + tmp5_U8), tmp2_U8);
      /*$E5F0*/ branchTarget = true; block_id = tmp5_U8 ? 1799 : 1800;
      break;
    case 1799:  // $E5F0
      /*$E5F0*/ CYCLES_EDGE(0xe5f0, 1);
                branchTarget = true; block_id = 1798;
      break;
    case 1800:  // $E5F2
      /*$E5F2*/ CYCLES(0xe5f2, 4);
                tmp5_U8 = pop8();
                s_a = tmp5_U8;
                block_id = 1801;
      break;
    case 1801:  // $E5F3
      /*$E5F3*/ CYCLES(0xe5f3, 10);
                s_status_c = 0x00;
      /*$E5F4*/ block_id = s_status_d ? 1803 : 1802;
      break;
    case 1802:  // $E5F4
      /*$E5F4*/ tmp4_U16 = s_a;
                tmp3_U16 = ram_peek(0x0071);
                tmp6_U16 = (tmp4_U16 + tmp3_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
                block_id = 1804;
      break;
    case 1803:  // $E5F4
      /*$E5F4*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x0071), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_not_z = (~tmp5_U8 & 2);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                s_status_n = (tmp5_U8 & 0x80);
                block_id = 1804;
      break;
    case 1804:  // $E5F6
      /*$E5F6*/ ram_poke(0x0071, s_a);
      /*$E5F8*/ branchTarget = true; block_id = !s_status_c ? 1805 : 1806;
      break;
    case 1805:  // $E5F8
      /*$E5F8*/ CYCLES_EDGE(0xe5f8, 1);
                branchTarget = true; block_id = 1807;
      break;
    case 1806:  // $E5FA
      /*$E5FA*/ CYCLES(0xe5fa, 5);
                tmp5_U8 = (uint8_t)(ram_peek(0x0072) + 0x01);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                ram_poke(0x0072, tmp5_U8);
                block_id = 1807;
      break;
    case 1807:  // $E5FC
      /*$E5FC*/ CYCLES(0xe5fc, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xe5fc, pop16() + 1);;
      break;
    case 1808:  // $E600
      /*$E600*/ CYCLES(0xe600, 6);
                s_a = ram_peek(0x00a0);
      /*$E602*/ s_y = ram_peek(0x00a1);
                FUNC_FRETMP(0x0000);
                block_id = find_block_id_func_t001(0xe602, pop16() + 1);;
      break;
    case 1809:  // $E604
      /*$E604*/ CYCLES(0xe604, 12);
                ram_poke(0x005e, s_a);
      /*$E606*/ ram_poke(0x005f, s_y);
      /*$E608*/ FUNC_FRETMS(0xe60a);
      /*$E60B*/ CYCLES(0xe60b, 41);
                push8((s_status_c | ((s_status_not_z == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | s_status_n));
      /*$E60E*/ tmp2_U8 = peek(ram_peek16al(0x005e));
      /*$E610*/ push8(tmp2_U8);
      /*$E612*/ tmp2_U8 = peek((ram_peek16al(0x005e) + 0x0001));
      /*$E614*/ s_x = tmp2_U8;
      /*$E616*/ tmp2_U8 = peek((ram_peek16al(0x005e) + 0x0002));
      /*$E618*/ s_y = tmp2_U8;
      /*$E619*/ tmp2_U8 = pop8();
                s_a = tmp2_U8;
      /*$E61A*/ tmp2_U8 = pop8();
                s_status_c = (tmp2_U8 & 0x01);
                tmp5_U8 = (~tmp2_U8 & 2);
                s_status_not_z = tmp5_U8;
                s_status_i = ((tmp2_U8 & 0x04) != 0);
                s_status_d = ((tmp2_U8 & 0x08) != 0);
                s_status_b = 0x00;
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
      /*$E61B*/ branchTarget = true; block_id = tmp5_U8 ? 1810 : 1811;
      break;
    case 1810:  // $E61B
      /*$E61B*/ CYCLES_EDGE(0xe61b, 1);
                branchTarget = true; block_id = 1822;
      break;
    case 1811:  // $E61D
      /*$E61D*/ CYCLES(0xe61d, 5);
                tmp5_U8 = s_y;
                tmp2_U8 = ram_peek(0x0070);
                tmp1_U8 = tmp5_U8 != tmp2_U8;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp5_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp5_U8 - tmp2_U8) & 0x80);
      /*$E61F*/ branchTarget = true; block_id = tmp1_U8 ? 1812 : 1813;
      break;
    case 1812:  // $E61F
      /*$E61F*/ CYCLES_EDGE(0xe61f, 1);
                branchTarget = true; block_id = 1822;
      break;
    case 1813:  // $E621
      /*$E621*/ CYCLES(0xe621, 5);
                tmp1_U8 = s_x;
                tmp2_U8 = ram_peek(0x006f);
                tmp5_U8 = tmp1_U8 != tmp2_U8;
                s_status_not_z = tmp5_U8;
                s_status_c = (tmp1_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
      /*$E623*/ branchTarget = true; block_id = tmp5_U8 ? 1814 : 1815;
      break;
    case 1814:  // $E623
      /*$E623*/ CYCLES_EDGE(0xe623, 1);
                branchTarget = true; block_id = 1822;
      break;
    case 1815:  // $E625
      /*$E625*/ CYCLES(0xe625, 13);
                push8(s_a);
      /*$E626*/ s_status_c = 0x00;
      /*$E627*/ block_id = s_status_d ? 1817 : 1816;
      break;
    case 1816:  // $E627
      /*$E627*/ tmp6_U16 = s_a;
                tmp3_U16 = ram_peek(0x006f);
                tmp4_U16 = (tmp6_U16 + tmp3_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)tmp3_U16);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1818;
      break;
    case 1817:  // $E627
      /*$E627*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x006f), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1818;
      break;
    case 1818:  // $E629
      /*$E629*/ ram_poke(0x006f, s_a);
      /*$E62B*/ branchTarget = true; block_id = !s_status_c ? 1819 : 1820;
      break;
    case 1819:  // $E62B
      /*$E62B*/ CYCLES_EDGE(0xe62b, 1);
                branchTarget = true; block_id = 1821;
      break;
    case 1820:  // $E62D
      /*$E62D*/ CYCLES(0xe62d, 5);
                ram_poke(0x0070, (uint8_t)(ram_peek(0x0070) + 0x01));
                block_id = 1821;
      break;
    case 1821:  // $E62F
      /*$E62F*/ CYCLES(0xe62f, 4);
                tmp5_U8 = pop8();
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
                block_id = 1822;
      break;
    case 1822:  // $E630
      /*$E630*/ CYCLES(0xe630, 12);
                ram_poke(0x005e, s_x);
      /*$E632*/ ram_poke(0x005f, s_y);
      /*$E634*/ branchTarget = true; block_id = find_block_id_func_t001(0xe634, pop16() + 1);;
      break;
    case 1823:  // $E635
      /*$E635*/ CYCLES(0xe635, 5);
                tmp1_U8 = s_y;
                tmp2_U8 = ram_peek(0x0054);
                tmp5_U8 = tmp1_U8 != tmp2_U8;
                s_status_not_z = tmp5_U8;
                s_status_c = (tmp1_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
      /*$E637*/ branchTarget = true; block_id = tmp5_U8 ? 1824 : 1825;
      break;
    case 1824:  // $E637
      /*$E637*/ CYCLES_EDGE(0xe637, 1);
                branchTarget = true; block_id = 1831;
      break;
    case 1825:  // $E639
      /*$E639*/ CYCLES(0xe639, 5);
                tmp5_U8 = s_a;
                tmp2_U8 = ram_peek(0x0053);
                tmp1_U8 = tmp5_U8 != tmp2_U8;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp5_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp5_U8 - tmp2_U8) & 0x80);
      /*$E63B*/ branchTarget = true; block_id = tmp1_U8 ? 1826 : 1827;
      break;
    case 1826:  // $E63B
      /*$E63B*/ CYCLES_EDGE(0xe63b, 1);
                branchTarget = true; block_id = 1831;
      break;
    case 1827:  // $E63D
      /*$E63D*/ CYCLES(0xe63d, 10);
                ram_poke(0x0052, s_a);
      /*$E63F*/ block_id = s_status_d ? 1829 : 1828;
      break;
    case 1828:  // $E63F
      /*$E63F*/ tmp4_U16 = s_a;
                tmp3_U16 = (tmp4_U16 - 0x0003) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0xfffc);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1830;
      break;
    case 1829:  // $E63F
      /*$E63F*/ tmp3_U16 = sbc_dec16(s_a, 0x03, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 1830;
      break;
    case 1830:  // $E641
      /*$E641*/ ram_poke(0x0053, s_a);
      /*$E643*/ s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_y = 0x00;
                block_id = 1831;
      break;
    case 1831:  // $E645
      /*$E645*/ CYCLES(0xe645, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xe645, pop16() + 1);;
      break;
    case 1832:  // $E6F2
      /*$E6F2*/ CYCLES(0xe6f2, 3);
                branchTarget = true; block_id = 1526;
      break;
    case 1833:  // $E6F8
      /*$E6F8*/ CYCLES(0xe6f8, 6);
                branchTarget = true; push16(0xe6fa); block_id = 1259;
      break;
    case 1834:  // $E6FB
      /*$E6FB*/ CYCLES(0xe6fb, 6);
                branchTarget = true; push16(0xe6fd); block_id = 1492;
      break;
    case 1835:  // $E6FE
      /*$E6FE*/ CYCLES(0xe6fe, 5);
      /*$E700*/ branchTarget = true; block_id = ram_peek(0x00a0) ? 1836 : 1837;
      break;
    case 1836:  // $E700
      /*$E700*/ CYCLES_EDGE(0xe700, 1);
                branchTarget = true; block_id = 1832;
      break;
    case 1837:  // $E702
      /*$E702*/ CYCLES(0xe702, 6);
                s_x = ram_peek(0x00a1);
      /*$E704*/ branchTarget = true; block_id = 5;
      break;
    case 1838:  // $E752
      /*$E752*/ CYCLES(0xe752, 7);
      /*$E754*/ tmp1_U8 = ram_peek(0x009d) >= 0x91;
                s_status_c = tmp1_U8;
      /*$E756*/ branchTarget = true; block_id = tmp1_U8 ? 1839 : 1840;
      break;
    case 1839:  // $E756
      /*$E756*/ CYCLES_EDGE(0xe756, 1);
                branchTarget = true; block_id = 1832;
      break;
    case 1840:  // $E758
      /*$E758*/ CYCLES(0xe758, 6);
                FUNC_QINT(0xe75a);
                branchTarget = true; block_id = 1841;
      break;
    case 1841:  // $E75B
      /*$E75B*/ CYCLES(0xe75b, 18);
                tmp1_U8 = ram_peek(0x00a0);
                s_a = tmp1_U8;
      /*$E75D*/ tmp2_U8 = ram_peek(0x00a1);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_y = tmp2_U8;
      /*$E75F*/ ram_poke(0x0050, tmp2_U8);
      /*$E761*/ ram_poke(0x0051, tmp1_U8);
      /*$E763*/ branchTarget = true; block_id = find_block_id_func_t001(0xe763, pop16() + 1);;
      break;
    case 1842:  // $E7A0
      /*$E7A0*/ CYCLES(0xe7a0, 7);
                s_a = 0x64;
      /*$E7A2*/ s_y = 0xee;
      /*$E7A4*/ branchTarget = true; block_id = 1845;
      break;
    case 1843:  // $E7BC
      /*$E7BC*/ CYCLES(0xe7bc, 2);
                branchTarget = true; block_id = !s_status_c ? 1844 : 1845;
      break;
    case 1844:  // $E7BC
      /*$E7BC*/ CYCLES_EDGE(0xe7bc, 1);
                branchTarget = true; block_id = 1865;
      break;
    case 1845:  // $E7BE
      /*$E7BE*/ CYCLES(0xe7be, 6);
                FUNC_UPAY2ARG(0xe7c0);
                branchTarget = true; block_id = 1846;
      break;
    case 1846:  // $E7C1
      /*$E7C1*/ CYCLES(0xe7c1, 2);
                branchTarget = true; block_id = s_status_not_z ? 1847 : 1848;
      break;
    case 1847:  // $E7C1
      /*$E7C1*/ CYCLES_EDGE(0xe7c1, 1);
      /*$E7C6*/ CYCLES(0xe7c6, 11);
      /*$E7C8*/ ram_poke(0x0092, ram_peek(0x00ac));
      /*$E7CA*/ s_x = 0xa5;
      /*$E7CC*/ s_a = ram_peek(0x00a5);
                block_id = 1849;
      break;
    case 1848:  // $E7C3
      /*$E7C3*/ CYCLES(0xe7c3, 3);
      /*$EB53*/ CYCLES(0xeb53, 8);
      /*$EB55*/ ram_poke(0x00a2, ram_peek(0x00aa));
      /*$EB57*/ s_x = 0x05;
                block_id = 2072;
      break;
    case 1849:  // $E7CE
      /*$E7CE*/ CYCLES(0xe7ce, 4);
                tmp1_U8 = s_a;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
      /*$E7CF*/ branchTarget = true; block_id = !tmp1_U8 ? 1850 : 1851;
      break;
    case 1850:  // $E7CF
      /*$E7CF*/ CYCLES_EDGE(0xe7cf, 1);
      /*$E79F*/ CYCLES(0xe79f, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xe79f, pop16() + 1);;
      break;
    case 1851:  // $E7D1
      /*$E7D1*/ CYCLES(0xe7d1, 7);
                s_status_c = 0x01;
      /*$E7D2*/ block_id = s_status_d ? 1853 : 1852;
      break;
    case 1852:  // $E7D2
      /*$E7D2*/ tmp3_U16 = s_a;
                tmp4_U16 = ram_peek(0x009d);
                tmp6_U16 = (tmp3_U16 - tmp4_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp4_U16));
                tmp1_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
                block_id = 1854;
      break;
    case 1853:  // $E7D2
      /*$E7D2*/ tmp6_U16 = sbc_dec16(s_a, ram_peek(0x009d), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 1854;
      break;
    case 1854:  // $E7D4
      /*$E7D4*/ branchTarget = true; block_id = !s_status_not_z ? 1855 : 1856;
      break;
    case 1855:  // $E7D4
      /*$E7D4*/ CYCLES_EDGE(0xe7d4, 1);
                branchTarget = true; block_id = 1865;
      break;
    case 1856:  // $E7D6
      /*$E7D6*/ CYCLES(0xe7d6, 2);
                branchTarget = true; block_id = !s_status_c ? 1857 : 1858;
      break;
    case 1857:  // $E7D6
      /*$E7D6*/ CYCLES_EDGE(0xe7d6, 1);
      /*$E7EA*/ CYCLES(0xe7ea, 5);
                s_y = 0x00;
      /*$E7EC*/ ram_poke(0x00ac, 0x00);
                block_id = 1862;
      break;
    case 1858:  // $E7D8
      /*$E7D8*/ CYCLES(0xe7d8, 22);
                ram_poke(0x009d, s_y);
      /*$E7DC*/ ram_poke(0x00a2, ram_peek(0x00aa));
      /*$E7DE*/ s_a = (s_a ^ 0xff);
      /*$E7E0*/ block_id = s_status_d ? 1860 : 1859;
      break;
    case 1859:  // $E7E0
      /*$E7E0*/ tmp6_U16 = s_a;
                tmp4_U16 = tmp6_U16 + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1861;
      break;
    case 1860:  // $E7E0
      /*$E7E0*/ tmp4_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 1861;
      break;
    case 1861:  // $E7E2
      /*$E7E2*/ s_y = 0x00;
      /*$E7E4*/ ram_poke(0x0092, 0x00);
      /*$E7E6*/ s_x = 0x9d;
      /*$E7E8*/ CYCLES_EDGE(0xe7e8, 1);
                branchTarget = true; block_id = 1862;
      break;
    case 1862:  // $E7EE
      /*$E7EE*/ CYCLES(0xe7ee, 4);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0xf9);
      /*$E7F0*/ branchTarget = true; block_id = ((uint8_t)(tmp1_U8 - 0xf9) & 0x80) ? 1863 : 1864;
      break;
    case 1863:  // $E7F0
      /*$E7F0*/ CYCLES_EDGE(0xe7f0, 1);
      /*$E7B9*/ CYCLES(0xe7b9, 6);
                FUNC_SHFTRGHT(0xe7bb);
                branchTarget = true; block_id = 1843;
      break;
    case 1864:  // $E7F2
      /*$E7F2*/ CYCLES(0xe7f2, 17);
                s_y = s_a;
      /*$E7F3*/ s_a = ram_peek(0x00ac);
      /*$E7F5*/ tmp4_U16 = (uint8_t)(0x01 + s_x);
                tmp1_U8 = ram_peek(tmp4_U16);
                s_status_c = (tmp1_U8 & 0x01);
                ram_poke(tmp4_U16, (tmp1_U8 >> 0x01));
      /*$E7F7*/ FUNC_SHFTRGHT4(0xe7f9);
                branchTarget = true; block_id = 1865;
      break;
    case 1865:  // $E7FA
      /*$E7FA*/ CYCLES(0xe7fa, 5);
      /*$E7FC*/ branchTarget = true; block_id = !(ram_peek(0x00ab) & 0x80) ? 1866 : 1867;
      break;
    case 1866:  // $E7FC
      /*$E7FC*/ CYCLES_EDGE(0xe7fc, 1);
      /*$E855*/ CYCLES(0xe855, 45);
                block_id = s_status_d ? 1900 : 1899;
      break;
    case 1867:  // $E7FE
      /*$E7FE*/ CYCLES(0xe7fe, 6);
                s_y = 0x9d;
      /*$E802*/ branchTarget = true; block_id = !(s_x != 0xa5) ? 1868 : 1869;
      break;
    case 1868:  // $E802
      /*$E802*/ CYCLES_EDGE(0xe802, 1);
                branchTarget = true; block_id = 1870;
      break;
    case 1869:  // $E804
      /*$E804*/ CYCLES(0xe804, 2);
                s_y = 0xa5;
                block_id = 1870;
      break;
    case 1870:  // $E806
      /*$E806*/ CYCLES(0xe806, 54);
                s_status_c = 0x01;
      /*$E807*/ s_a = (s_a ^ 0xff);
      /*$E809*/ block_id = s_status_d ? 1872 : 1871;
      break;
    case 1871:  // $E809
      /*$E809*/ tmp4_U16 = (s_a + ram_peek(0x0092)) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1873;
      break;
    case 1872:  // $E809
      /*$E809*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x0092), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1873;
      break;
    case 1873:  // $E80B
      /*$E80B*/ ram_poke(0x00ac, s_a);
      /*$E80D*/ s_a = ram_peek((0x0004 + s_y));
      /*$E810*/ block_id = s_status_d ? 1875 : 1874;
      break;
    case 1874:  // $E810
      /*$E810*/ tmp4_U16 = (s_a - ram_peek((uint8_t)(0x04 + s_x))) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1876;
      break;
    case 1875:  // $E810
      /*$E810*/ tmp4_U16 = sbc_dec16(s_a, ram_peek((uint8_t)(0x04 + s_x)), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1876;
      break;
    case 1876:  // $E812
      /*$E812*/ ram_poke(0x00a1, s_a);
      /*$E814*/ s_a = ram_peek((0x0003 + s_y));
      /*$E817*/ block_id = s_status_d ? 1878 : 1877;
      break;
    case 1877:  // $E817
      /*$E817*/ tmp4_U16 = (s_a - ram_peek((uint8_t)(0x03 + s_x))) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1879;
      break;
    case 1878:  // $E817
      /*$E817*/ tmp4_U16 = sbc_dec16(s_a, ram_peek((uint8_t)(0x03 + s_x)), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1879;
      break;
    case 1879:  // $E819
      /*$E819*/ ram_poke(0x00a0, s_a);
      /*$E81B*/ s_a = ram_peek((0x0002 + s_y));
      /*$E81E*/ block_id = s_status_d ? 1881 : 1880;
      break;
    case 1880:  // $E81E
      /*$E81E*/ tmp4_U16 = (s_a - ram_peek((uint8_t)(0x02 + s_x))) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1882;
      break;
    case 1881:  // $E81E
      /*$E81E*/ tmp4_U16 = sbc_dec16(s_a, ram_peek((uint8_t)(0x02 + s_x)), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1882;
      break;
    case 1882:  // $E820
      /*$E820*/ ram_poke(0x009f, s_a);
      /*$E822*/ s_a = ram_peek((0x0001 + s_y));
      /*$E825*/ block_id = s_status_d ? 1884 : 1883;
      break;
    case 1883:  // $E825
      /*$E825*/ tmp4_U16 = (s_a - ram_peek((uint8_t)(0x01 + s_x))) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1885;
      break;
    case 1884:  // $E825
      /*$E825*/ tmp4_U16 = sbc_dec16(s_a, ram_peek((uint8_t)(0x01 + s_x)), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1885;
      break;
    case 1885:  // $E827
      /*$E827*/ ram_poke(0x009e, s_a);
                block_id = 1886;
      break;
    case 1886:  // $E829
      /*$E829*/ CYCLES(0xe829, 2);
                branchTarget = true; block_id = s_status_c ? 1887 : 1888;
      break;
    case 1887:  // $E829
      /*$E829*/ CYCLES_EDGE(0xe829, 1);
                branchTarget = true; block_id = 1889;
      break;
    case 1888:  // $E82B
      /*$E82B*/ CYCLES(0xe82b, 6);
                FUNC_TWSCMPFAC(0xe82d);
                branchTarget = true; block_id = 1889;
      break;
    case 1889:  // $E82E
      /*$E82E*/ CYCLES(0xe82e, 6);
                s_y = 0x00;
      /*$E830*/ s_a = 0x00;
      /*$E831*/ s_status_c = 0x00;
                block_id = 1890;
      break;
    case 1890:  // $E832
      /*$E832*/ CYCLES(0xe832, 5);
                tmp1_U8 = ram_peek(0x009e);
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$E834*/ branchTarget = true; block_id = tmp1_U8 ? 1891 : 1892;
      break;
    case 1891:  // $E834
      /*$E834*/ CYCLES_EDGE(0xe834, 1);
                branchTarget = true; block_id = 1917;
      break;
    case 1892:  // $E836
      /*$E836*/ CYCLES(0xe836, 33);
      /*$E838*/ ram_poke(0x009e, ram_peek(0x009f));
      /*$E83C*/ ram_poke(0x009f, ram_peek(0x00a0));
      /*$E840*/ ram_poke(0x00a0, ram_peek(0x00a1));
      /*$E842*/ tmp1_U8 = ram_peek(0x00ac);
                s_x = tmp1_U8;
      /*$E844*/ ram_poke(0x00a1, tmp1_U8);
      /*$E846*/ ram_poke(0x00ac, s_y);
      /*$E848*/ block_id = s_status_d ? 1894 : 1893;
      break;
    case 1893:  // $E848
      /*$E848*/ tmp4_U16 = s_a;
                tmp6_U16 = (tmp4_U16 + 0x0008) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0x0008);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1895;
      break;
    case 1894:  // $E848
      /*$E848*/ tmp6_U16 = adc_dec16(s_a, 0x08, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 1895;
      break;
    case 1895:  // $E84A
      /*$E84A*/ tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x20);
      /*$E84C*/ branchTarget = true; block_id = (tmp1_U8 != 0x20) ? 1896 : 1897;
      break;
    case 1896:  // $E84C
      /*$E84C*/ CYCLES_EDGE(0xe84c, 1);
                branchTarget = true; block_id = 1890;
      break;
    case 1897:  // $E84E
      /*$E84E*/ CYCLES(0xe84e, 5);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$E850*/ ram_poke(0x009d, 0x00);
                block_id = 1898;
      break;
    case 1898:  // $E852
      /*$E852*/ CYCLES(0xe852, 9);
                ram_poke(0x00a2, s_a);
      /*$E854*/ branchTarget = true; block_id = find_block_id_func_t001(0xe854, pop16() + 1);;
      break;
    case 1899:  // $E855
      /*$E855*/ tmp6_U16 = (s_a + ram_peek(0x0092)) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1901;
      break;
    case 1900:  // $E855
      /*$E855*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x0092), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1901;
      break;
    case 1901:  // $E857
      /*$E857*/ ram_poke(0x00ac, s_a);
      /*$E859*/ s_a = ram_peek(0x00a1);
      /*$E85B*/ block_id = s_status_d ? 1903 : 1902;
      break;
    case 1902:  // $E85B
      /*$E85B*/ tmp6_U16 = (s_a + ram_peek(0x00a9)) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1904;
      break;
    case 1903:  // $E85B
      /*$E85B*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x00a9), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1904;
      break;
    case 1904:  // $E85D
      /*$E85D*/ ram_poke(0x00a1, s_a);
      /*$E85F*/ s_a = ram_peek(0x00a0);
      /*$E861*/ block_id = s_status_d ? 1906 : 1905;
      break;
    case 1905:  // $E861
      /*$E861*/ tmp6_U16 = (s_a + ram_peek(0x00a8)) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1907;
      break;
    case 1906:  // $E861
      /*$E861*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x00a8), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1907;
      break;
    case 1907:  // $E863
      /*$E863*/ ram_poke(0x00a0, s_a);
      /*$E865*/ s_a = ram_peek(0x009f);
      /*$E867*/ block_id = s_status_d ? 1909 : 1908;
      break;
    case 1908:  // $E867
      /*$E867*/ tmp6_U16 = (s_a + ram_peek(0x00a7)) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1910;
      break;
    case 1909:  // $E867
      /*$E867*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x00a7), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1910;
      break;
    case 1910:  // $E869
      /*$E869*/ ram_poke(0x009f, s_a);
      /*$E86B*/ s_a = ram_peek(0x009e);
      /*$E86D*/ block_id = s_status_d ? 1912 : 1911;
      break;
    case 1911:  // $E86D
      /*$E86D*/ tmp6_U16 = s_a;
                tmp4_U16 = ram_peek(0x00a6);
                tmp3_U16 = (tmp6_U16 + tmp4_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)tmp4_U16);
                tmp1_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 1913;
      break;
    case 1912:  // $E86D
      /*$E86D*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x00a6), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
                block_id = 1913;
      break;
    case 1913:  // $E86F
      /*$E86F*/ ram_poke(0x009e, s_a);
      /*$E871*/ branchTarget = true; block_id = 1928;
      break;
    case 1914:  // $E874
      /*$E874*/ s_a = (uint8_t)((s_a + 0x0001) + s_status_c);
                block_id = 1916;
      break;
    case 1915:  // $E874
      /*$E874*/ s_a = ((uint8_t)adc_dec16(s_a, 0x01, s_status_c));
                block_id = 1916;
      break;
    case 1916:  // $E876
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
                tmp1_U8 = (uint8_t)tmp3_U16;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x009e, tmp1_U8);
                block_id = 1917;
      break;
    case 1917:  // $E880
      /*$E880*/ CYCLES(0xe880, 2);
                branchTarget = true; block_id = !s_status_n ? 1918 : 1919;
      break;
    case 1918:  // $E880
      /*$E880*/ CYCLES_EDGE(0xe880, 1);
      /*$E874*/ CYCLES(0xe874, 27);
                block_id = s_status_d ? 1915 : 1914;
      break;
    case 1919:  // $E882
      /*$E882*/ CYCLES(0xe882, 7);
                s_status_c = 0x01;
      /*$E883*/ block_id = s_status_d ? 1921 : 1920;
      break;
    case 1920:  // $E883
      /*$E883*/ tmp3_U16 = s_a;
                tmp4_U16 = ram_peek(0x009d);
                tmp6_U16 = (tmp3_U16 - tmp4_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp4_U16));
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1922;
      break;
    case 1921:  // $E883
      /*$E883*/ tmp6_U16 = sbc_dec16(s_a, ram_peek(0x009d), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 1922;
      break;
    case 1922:  // $E885
      /*$E885*/ branchTarget = true; block_id = s_status_c ? 1923 : 1924;
      break;
    case 1923:  // $E885
      /*$E885*/ CYCLES_EDGE(0xe885, 1);
                branchTarget = true; block_id = 1897;
      break;
    case 1924:  // $E887
      /*$E887*/ CYCLES(0xe887, 7);
                s_a = (s_a ^ 0xff);
      /*$E889*/ block_id = s_status_d ? 1926 : 1925;
      break;
    case 1925:  // $E889
      /*$E889*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 + 0x0001) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x0001);
                tmp1_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 1927;
      break;
    case 1926:  // $E889
      /*$E889*/ tmp4_U16 = adc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
                block_id = 1927;
      break;
    case 1927:  // $E88B
      /*$E88B*/ ram_poke(0x009d, s_a);
                block_id = 1928;
      break;
    case 1928:  // $E88D
      /*$E88D*/ CYCLES(0xe88d, 2);
                branchTarget = true; block_id = !s_status_c ? 1929 : 1930;
      break;
    case 1929:  // $E88D
      /*$E88D*/ CYCLES_EDGE(0xe88d, 1);
                branchTarget = true; block_id = 1933;
      break;
    case 1930:  // $E88F
      /*$E88F*/ CYCLES(0xe88f, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x009d) + 0x01);
                ram_poke(0x009d, tmp1_U8);
      /*$E891*/ branchTarget = true; block_id = !tmp1_U8 ? 1931 : 1932;
      break;
    case 1931:  // $E891
      /*$E891*/ CYCLES_EDGE(0xe891, 1);
                branchTarget = true; block_id = 1945;
      break;
    case 1932:  // $E893
      /*$E893*/ CYCLES(0xe893, 25);
                tmp1_U8 = ram_peek(0x009e);
                ram_poke(0x009e, (uint8_t)((tmp1_U8 | (0x00 + (s_status_c << 8))) >> 0x01));
      /*$E895*/ tmp2_U8 = ram_peek(0x009f);
                ram_poke(0x009f, (uint8_t)((tmp2_U8 | (0x00 + ((tmp1_U8 & 0x01) << 8))) >> 0x01));
      /*$E897*/ tmp1_U8 = ram_peek(0x00a0);
                ram_poke(0x00a0, (uint8_t)((tmp1_U8 | (0x00 + ((tmp2_U8 & 0x01) << 8))) >> 0x01));
      /*$E899*/ tmp2_U8 = ram_peek(0x00a1);
                ram_poke(0x00a1, (uint8_t)((tmp2_U8 | (0x00 + ((tmp1_U8 & 0x01) << 8))) >> 0x01));
      /*$E89B*/ tmp1_U8 = ram_peek(0x00ac);
                tmp2_U8 = (uint8_t)((tmp1_U8 | (0x00 + ((tmp2_U8 & 0x01) << 8))) >> 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x00ac, tmp2_U8);
                s_status_c = (tmp1_U8 & 0x01);
                block_id = 1933;
      break;
    case 1933:  // $E89D
      /*$E89D*/ CYCLES(0xe89d, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xe89d, pop16() + 1);;
      break;
    case 1934:  // $E89E
      /*$E89E*/ CYCLES(0xe89e, 8);
      /*$E8A2*/ ram_poke(0x00a2, (ram_peek(0x00a2) ^ 0xff));
                FUNC_TCFACMANT(0x0000);
                block_id = find_block_id_func_t001(0xe8a2, pop16() + 1);;
      break;
    case 1935:  // $E8A4
      /*$E8A4*/ CYCLES(0xe8a4, 47);
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
      /*$E8C4*/ branchTarget = true; block_id = tmp1_U8 ? 1936 : 2642;
      break;
    case 1936:  // $E8C4
      /*$E8C4*/ CYCLES_EDGE(0xe8c4, 1);
                branchTarget = true; block_id = 1944;
      break;
    case 1937:  // $E8C6
      /*$E8C6*/ CYCLES(0xe8c6, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x00a1) + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x00a1, tmp1_U8);
      /*$E8C8*/ branchTarget = true; block_id = tmp1_U8 ? 1938 : 1939;
      break;
    case 1938:  // $E8C8
      /*$E8C8*/ CYCLES_EDGE(0xe8c8, 1);
                branchTarget = true; block_id = 1944;
      break;
    case 1939:  // $E8CA
      /*$E8CA*/ CYCLES(0xe8ca, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x00a0) + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x00a0, tmp1_U8);
      /*$E8CC*/ branchTarget = true; block_id = tmp1_U8 ? 1940 : 1941;
      break;
    case 1940:  // $E8CC
      /*$E8CC*/ CYCLES_EDGE(0xe8cc, 1);
                branchTarget = true; block_id = 1944;
      break;
    case 1941:  // $E8CE
      /*$E8CE*/ CYCLES(0xe8ce, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x009f) + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x009f, tmp1_U8);
      /*$E8D0*/ branchTarget = true; block_id = tmp1_U8 ? 1942 : 1943;
      break;
    case 1942:  // $E8D0
      /*$E8D0*/ CYCLES_EDGE(0xe8d0, 1);
                branchTarget = true; block_id = 1944;
      break;
    case 1943:  // $E8D2
      /*$E8D2*/ CYCLES(0xe8d2, 5);
                tmp1_U8 = (uint8_t)(ram_peek(0x009e) + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x009e, tmp1_U8);
                block_id = 1944;
      break;
    case 1944:  // $E8D4
      /*$E8D4*/ CYCLES(0xe8d4, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xe8d4, pop16() + 1);;
      break;
    case 1945:  // $E8D5
      /*$E8D5*/ CYCLES(0xe8d5, 5);
                s_x = 0x45;
      /*$E8D7*/ branchTarget = true; block_id = 887;
      break;
    case 1946:  // $E8DC
      /*$E8DC*/ CYCLES(0xe8dc, 38);
                tmp1_U8 = s_x;
      /*$E8DE*/ ram_poke(0x00ac, ram_peek((uint8_t)(0x04 + tmp1_U8)));
      /*$E8E2*/ ram_poke((uint8_t)(0x04 + tmp1_U8), ram_peek((uint8_t)(0x03 + tmp1_U8)));
      /*$E8E6*/ ram_poke((uint8_t)(0x03 + tmp1_U8), ram_peek((uint8_t)(0x02 + tmp1_U8)));
      /*$E8EA*/ ram_poke((uint8_t)(0x02 + tmp1_U8), ram_peek((uint8_t)(0x01 + tmp1_U8)));
      /*$E8EE*/ ram_poke((uint8_t)(0x01 + tmp1_U8), ram_peek(0x00a4));
      /*$E8F0*/ CYCLES(0xe8f0, 4);
                block_id = s_status_d ? 1948 : 1947;
      break;
    case 1947:  // $E8F0
      /*$E8F0*/ tmp4_U16 = (s_a + 0x0008) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                tmp1_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 1949;
      break;
    case 1948:  // $E8F0
      /*$E8F0*/ tmp4_U16 = adc_dec16(s_a, 0x08, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_n = (tmp1_U8 & 0x80);
                block_id = 1949;
      break;
    case 1949:  // $E8F2
      /*$E8F2*/ branchTarget = true; block_id = s_status_n ? 1950 : 1951;
      break;
    case 1950:  // $E8F2
      /*$E8F2*/ CYCLES_EDGE(0xe8f2, 1);
                branchTarget = true; block_id = 1946;
      break;
    case 1951:  // $E8F4
      /*$E8F4*/ CYCLES(0xe8f4, 2);
                branchTarget = true; block_id = !s_status_not_z ? 1952 : 1953;
      break;
    case 1952:  // $E8F4
      /*$E8F4*/ CYCLES_EDGE(0xe8f4, 1);
                branchTarget = true; block_id = 1946;
      break;
    case 1953:  // $E8F6
      /*$E8F6*/ CYCLES(0xe8f6, 9);
                block_id = s_status_d ? 1955 : 1954;
      break;
    case 1954:  // $E8F6
      /*$E8F6*/ tmp4_U16 = s_a;
                tmp6_U16 = (tmp4_U16 - 0x0008) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0xfff7);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1956;
      break;
    case 1955:  // $E8F6
      /*$E8F6*/ tmp6_U16 = sbc_dec16(s_a, 0x08, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 1956;
      break;
    case 1956:  // $E8F8
      /*$E8F8*/ s_y = s_a;
      /*$E8F9*/ tmp1_U8 = ram_peek(0x00ac);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$E8FB*/ branchTarget = true; block_id = s_status_c ? 1957 : 1958;
      break;
    case 1957:  // $E8FB
      /*$E8FB*/ CYCLES_EDGE(0xe8fb, 1);
                branchTarget = true; block_id = 1963;
      break;
    case 1958:  // $E8FD
      /*$E8FD*/ CYCLES(0xe8fd, 8);
                tmp6_U16 = (uint8_t)(0x01 + s_x);
                tmp4_U16 = ram_peek(tmp6_U16) << 0x01;
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = tmp1_U8;
                ram_poke(tmp6_U16, ((uint8_t)tmp4_U16));
      /*$E8FF*/ branchTarget = true; block_id = !tmp1_U8 ? 1959 : 1960;
      break;
    case 1959:  // $E8FF
      /*$E8FF*/ CYCLES_EDGE(0xe8ff, 1);
                branchTarget = true; block_id = 1961;
      break;
    case 1960:  // $E901
      /*$E901*/ CYCLES(0xe901, 6);
                tmp4_U16 = (uint8_t)(0x01 + s_x);
                ram_poke(tmp4_U16, (uint8_t)(ram_peek(tmp4_U16) + 0x01));
                block_id = 1961;
      break;
    case 1961:  // $E903
      /*$E903*/ CYCLES(0xe903, 12);
                tmp1_U8 = s_x;
                tmp4_U16 = (uint8_t)(0x01 + tmp1_U8);
                tmp2_U8 = ram_peek(tmp4_U16);
                ram_poke(tmp4_U16, (uint8_t)((tmp2_U8 | (0x00 + (s_status_c << 8))) >> 0x01));
      /*$E905*/ tmp4_U16 = (uint8_t)(0x01 + tmp1_U8);
                tmp5_U8 = ram_peek(tmp4_U16);
                ram_poke(tmp4_U16, (uint8_t)((tmp5_U8 | (0x00 + ((tmp2_U8 & 0x01) << 8))) >> 0x01));
      /*$E907*/ CYCLES(0xe907, 24);
                tmp4_U16 = (uint8_t)(0x02 + tmp1_U8);
                tmp2_U8 = ram_peek(tmp4_U16);
                ram_poke(tmp4_U16, (uint8_t)((tmp2_U8 | (0x00 + ((tmp5_U8 & 0x01) << 8))) >> 0x01));
      /*$E909*/ tmp4_U16 = (uint8_t)(0x03 + tmp1_U8);
                tmp5_U8 = ram_peek(tmp4_U16);
                ram_poke(tmp4_U16, (uint8_t)((tmp5_U8 | (0x00 + ((tmp2_U8 & 0x01) << 8))) >> 0x01));
      /*$E90B*/ tmp4_U16 = (uint8_t)(0x04 + tmp1_U8);
                tmp1_U8 = ram_peek(tmp4_U16);
                ram_poke(tmp4_U16, (uint8_t)((tmp1_U8 | (0x00 + ((tmp5_U8 & 0x01) << 8))) >> 0x01));
      /*$E90D*/ s_a = (uint8_t)((s_a | (0x00 + ((tmp1_U8 & 0x01) << 8))) >> 0x01);
      /*$E90E*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
      /*$E90F*/ branchTarget = true; block_id = tmp1_U8 ? 1962 : 1963;
      break;
    case 1962:  // $E90F
      /*$E90F*/ CYCLES_EDGE(0xe90f, 1);
                branchTarget = true; block_id = 1958;
      break;
    case 1963:  // $E911
      /*$E911*/ CYCLES(0xe911, 8);
                s_status_c = 0x00;
      /*$E912*/ branchTarget = true; block_id = find_block_id_func_t001(0xe912, pop16() + 1);;
      break;
    case 1964:  // $E97F
      /*$E97F*/ CYCLES(0xe97f, 6);
                FUNC_UPAY2ARG(0xe981);
      /*$E982*/ CYCLES(0xe982, 2);
                branchTarget = true; block_id = s_status_not_z ? 1965 : 1966;
      break;
    case 1965:  // $E982
      /*$E982*/ CYCLES_EDGE(0xe982, 1);
      /*$E987*/ CYCLES(0xe987, 6);
                branchTarget = true; push16(0xe989); block_id = 1991;
      break;
    case 1966:  // $E984
      /*$E984*/ CYCLES(0xe984, 3);
                branchTarget = true; block_id = 1989;
      break;
    case 1967:  // $E98A
      /*$E98A*/ CYCLES(0xe98a, 23);
      /*$E98C*/ ram_poke(0x0062, 0x00);
      /*$E98E*/ ram_poke(0x0063, 0x00);
      /*$E990*/ ram_poke(0x0064, 0x00);
      /*$E992*/ ram_poke(0x0065, 0x00);
      /*$E994*/ tmp1_U8 = ram_peek(0x00ac);
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
      /*$E996*/ FUNC_MULT1(0xe998);
      /*$E999*/ CYCLES(0xe999, 9);
                tmp1_U8 = ram_peek(0x00a1);
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
      /*$E99B*/ FUNC_MULT1(0xe99d);
      /*$E99E*/ CYCLES(0xe99e, 9);
                tmp1_U8 = ram_peek(0x00a0);
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
      /*$E9A0*/ FUNC_MULT1(0xe9a2);
      /*$E9A3*/ CYCLES(0xe9a3, 9);
                tmp1_U8 = ram_peek(0x009f);
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
      /*$E9A5*/ FUNC_MULT1(0xe9a7);
      /*$E9A8*/ CYCLES(0xe9a8, 9);
                s_a = ram_peek(0x009e);
      /*$E9AA*/ FUNC_MULT2(0xe9ac);
      /*$E9AD*/ CYCLES(0xe9ad, 3);
                branchTarget = true; block_id = 2068;
      break;
    case 1968:  // $E9B0
      /*$E9B0*/ CYCLES(0xe9b0, 2);
                branchTarget = true; block_id = s_status_not_z ? 1969 : 1970;
      break;
    case 1969:  // $E9B0
      /*$E9B0*/ CYCLES_EDGE(0xe9b0, 1);
                FUNC_MULT2(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xe9b0, pop16() + 1);;
      break;
    case 1970:  // $E9B2
      /*$E9B2*/ CYCLES(0xe9b2, 3);
      /*$E8DA*/ CYCLES(0xe8da, 2);
                s_x = 0x61;
                block_id = 1946;
      break;
    case 1971:  // $E9B5
      /*$E9B5*/ CYCLES(0xe9b5, 4);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 & 0x01);
      /*$E9B6*/ s_a = ((tmp1_U8 >> 0x01) | 0x80);
                block_id = 1972;
      break;
    case 1972:  // $E9B8
      /*$E9B8*/ CYCLES(0xe9b8, 4);
                s_y = s_a;
      /*$E9B9*/ branchTarget = true; block_id = !s_status_c ? 1973 : 1974;
      break;
    case 1973:  // $E9B9
      /*$E9B9*/ CYCLES_EDGE(0xe9b9, 1);
                branchTarget = true; block_id = 1987;
      break;
    case 1974:  // $E9BB
      /*$E9BB*/ CYCLES(0xe9bb, 38);
                s_status_c = 0x00;
      /*$E9BC*/ s_a = ram_peek(0x0065);
      /*$E9BE*/ block_id = s_status_d ? 1976 : 1975;
      break;
    case 1975:  // $E9BE
      /*$E9BE*/ tmp4_U16 = (s_a + ram_peek(0x00a9)) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1977;
      break;
    case 1976:  // $E9BE
      /*$E9BE*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x00a9), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1977;
      break;
    case 1977:  // $E9C0
      /*$E9C0*/ ram_poke(0x0065, s_a);
      /*$E9C2*/ s_a = ram_peek(0x0064);
      /*$E9C4*/ block_id = s_status_d ? 1979 : 1978;
      break;
    case 1978:  // $E9C4
      /*$E9C4*/ tmp4_U16 = (s_a + ram_peek(0x00a8)) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1980;
      break;
    case 1979:  // $E9C4
      /*$E9C4*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x00a8), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1980;
      break;
    case 1980:  // $E9C6
      /*$E9C6*/ ram_poke(0x0064, s_a);
      /*$E9C8*/ s_a = ram_peek(0x0063);
      /*$E9CA*/ block_id = s_status_d ? 1982 : 1981;
      break;
    case 1981:  // $E9CA
      /*$E9CA*/ tmp4_U16 = (s_a + ram_peek(0x00a7)) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1983;
      break;
    case 1982:  // $E9CA
      /*$E9CA*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x00a7), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1983;
      break;
    case 1983:  // $E9CC
      /*$E9CC*/ ram_poke(0x0063, s_a);
      /*$E9CE*/ s_a = ram_peek(0x0062);
      /*$E9D0*/ block_id = s_status_d ? 1985 : 1984;
      break;
    case 1984:  // $E9D0
      /*$E9D0*/ tmp4_U16 = s_a;
                tmp6_U16 = ram_peek(0x00a6);
                tmp3_U16 = (tmp4_U16 + tmp6_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp6_U16);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1986;
      break;
    case 1985:  // $E9D0
      /*$E9D0*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x00a6), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 1986;
      break;
    case 1986:  // $E9D2
      /*$E9D2*/ ram_poke(0x0062, s_a);
                block_id = 1987;
      break;
    case 1987:  // $E9D4
      /*$E9D4*/ CYCLES(0xe9d4, 31);
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
      /*$E9E0*/ branchTarget = true; block_id = tmp5_U8 ? 1988 : 1989;
      break;
    case 1988:  // $E9E0
      /*$E9E0*/ CYCLES_EDGE(0xe9e0, 1);
                branchTarget = true; block_id = 1972;
      break;
    case 1989:  // $E9E2
      /*$E9E2*/ CYCLES(0xe9e2, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xe9e2, pop16() + 1);;
      break;
    case 1990:  // $E9E3
      /*$E9E3*/ CYCLES(0xe9e3, 79);
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
      /*$EA0D*/ branchTarget = true; block_id = find_block_id_func_t001(0xea0d, pop16() + 1);;
      break;
    case 1991:  // $EA0E
      /*$EA0E*/ CYCLES(0xea0e, 5);
                tmp1_U8 = ram_peek(0x00a5);
                s_a = tmp1_U8;
      /*$EA10*/ branchTarget = true; block_id = !tmp1_U8 ? 1992 : 1993;
      break;
    case 1992:  // $EA10
      /*$EA10*/ CYCLES_EDGE(0xea10, 1);
                branchTarget = true; block_id = 2008;
      break;
    case 1993:  // $EA12
      /*$EA12*/ CYCLES(0xea12, 7);
                s_status_c = 0x00;
      /*$EA13*/ block_id = s_status_d ? 1995 : 1994;
      break;
    case 1994:  // $EA13
      /*$EA13*/ tmp3_U16 = s_a;
                tmp6_U16 = ram_peek(0x009d);
                tmp4_U16 = (tmp3_U16 + tmp6_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)tmp6_U16);
                tmp5_U8 = (uint8_t)tmp4_U16;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
                block_id = 1996;
      break;
    case 1995:  // $EA13
      /*$EA13*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x009d), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                s_status_n = (tmp5_U8 & 0x80);
                block_id = 1996;
      break;
    case 1996:  // $EA15
      /*$EA15*/ branchTarget = true; block_id = !s_status_c ? 1997 : 1998;
      break;
    case 1997:  // $EA15
      /*$EA15*/ CYCLES_EDGE(0xea15, 1);
      /*$EA1B*/ CYCLES(0xea1b, 2);
                branchTarget = true; block_id = !s_status_n ? 2001 : 2002;
      break;
    case 1998:  // $EA17
      /*$EA17*/ CYCLES(0xea17, 2);
                branchTarget = true; block_id = s_status_n ? 1999 : 2000;
      break;
    case 1999:  // $EA17
      /*$EA17*/ CYCLES_EDGE(0xea17, 1);
                branchTarget = true; block_id = 2009;
      break;
    case 2000:  // $EA19
      /*$EA19*/ CYCLES(0xea19, 6);
                s_status_c = 0x00;
      /*$EA1A*/ block_id = 2002;
      break;
    case 2001:  // $EA1B
      /*$EA1B*/ CYCLES_EDGE(0xea1b, 1);
                branchTarget = true; block_id = 2008;
      break;
    case 2002:  // $EA1D
      /*$EA1D*/ CYCLES(0xea1d, 7);
                block_id = s_status_d ? 2004 : 2003;
      break;
    case 2003:  // $EA1D
      /*$EA1D*/ tmp4_U16 = s_a;
                tmp6_U16 = (tmp4_U16 + 0x0080) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0x0080);
                tmp5_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
                block_id = 2005;
      break;
    case 2004:  // $EA1D
      /*$EA1D*/ tmp6_U16 = adc_dec16(s_a, 0x80, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_not_z = (~tmp5_U8 & 2);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                s_status_n = (tmp5_U8 & 0x80);
                block_id = 2005;
      break;
    case 2005:  // $EA1F
      /*$EA1F*/ ram_poke(0x009d, s_a);
      /*$EA21*/ branchTarget = true; block_id = s_status_not_z ? 2006 : 2007;
      break;
    case 2006:  // $EA21
      /*$EA21*/ CYCLES_EDGE(0xea21, 1);
      /*$EA26*/ CYCLES(0xea26, 12);
                tmp5_U8 = ram_peek(0x00ab);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$EA28*/ ram_poke(0x00a2, tmp5_U8);
      /*$EA2A*/ branchTarget = true; block_id = find_block_id_func_t001(0xea2a, pop16() + 1);;
      break;
    case 2007:  // $EA23
      /*$EA23*/ CYCLES(0xea23, 3);
                branchTarget = true; block_id = 1898;
      break;
    case 2008:  // $EA31
      /*$EA31*/ CYCLES(0xea31, 11);
                tmp5_U8 = pop8();
      /*$EA32*/ tmp5_U8 = pop8();
      /*$EA33*/ branchTarget = true; block_id = 1897;
      break;
    case 2009:  // $EA36
      /*$EA36*/ CYCLES(0xea36, 3);
                branchTarget = true; block_id = 1945;
      break;
    case 2010:  // $EA39
      /*$EA39*/ CYCLES(0xea39, 6);
                branchTarget = true; push16(0xea3b); block_id = 2075;
      break;
    case 2011:  // $EA3C
      /*$EA3C*/ CYCLES(0xea3c, 4);
                tmp5_U8 = s_a;
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_x = tmp5_U8;
      /*$EA3D*/ branchTarget = true; block_id = !tmp5_U8 ? 2012 : 2013;
      break;
    case 2012:  // $EA3D
      /*$EA3D*/ CYCLES_EDGE(0xea3d, 1);
                branchTarget = true; block_id = 2021;
      break;
    case 2013:  // $EA3F
      /*$EA3F*/ CYCLES(0xea3f, 6);
                s_status_c = 0x00;
      /*$EA40*/ block_id = s_status_d ? 2015 : 2014;
      break;
    case 2014:  // $EA40
      /*$EA40*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 + 0x0002) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x0002);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 2016;
      break;
    case 2015:  // $EA40
      /*$EA40*/ tmp4_U16 = adc_dec16(s_a, 0x02, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp5_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 2016;
      break;
    case 2016:  // $EA42
      /*$EA42*/ branchTarget = true; block_id = s_status_c ? 2017 : 2018;
      break;
    case 2017:  // $EA42
      /*$EA42*/ CYCLES_EDGE(0xea42, 1);
                branchTarget = true; block_id = 2009;
      break;
    case 2018:  // $EA44
      /*$EA44*/ CYCLES(0xea44, 11);
                s_x = 0x00;
      /*$EA46*/ ram_poke(0x00ab, 0x00);
      /*$EA48*/ branchTarget = true; push16(0xea4a); block_id = 1849;
      break;
    case 2019:  // $EA4B
      /*$EA4B*/ CYCLES(0xea4b, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x009d) + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x009d, tmp1_U8);
      /*$EA4D*/ branchTarget = true; block_id = !tmp1_U8 ? 2020 : 2021;
      break;
    case 2020:  // $EA4D
      /*$EA4D*/ CYCLES_EDGE(0xea4d, 1);
                branchTarget = true; block_id = 2009;
      break;
    case 2021:  // $EA4F
      /*$EA4F*/ CYCLES(0xea4f, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xea4f, pop16() + 1);;
      break;
    case 2022:  // $EA55
      /*$EA55*/ CYCLES(0xea55, 6);
                branchTarget = true; push16(0xea57); block_id = 2075;
      break;
    case 2023:  // $EA58
      /*$EA58*/ CYCLES(0xea58, 15);
                s_a = 0x50;
      /*$EA5A*/ s_y = 0xea;
      /*$EA5C*/ s_x = 0x00;
      /*$EA5E*/ ram_poke(0x00ab, 0x00);
      /*$EA60*/ FUNC_UPAY2FAC(0xea62);
      /*$EA63*/ CYCLES(0xea63, 3);
      /*$EA69*/ CYCLES(0xea69, 2);
                branchTarget = true; block_id = !s_status_not_z ? 2024 : 2025;
      break;
    case 2024:  // $EA69
      /*$EA69*/ CYCLES_EDGE(0xea69, 1);
      /*$EAE1*/ CYCLES(0xeae1, 5);
                s_x = 0x85;
      /*$EAE3*/ branchTarget = true; block_id = 887;
      break;
    case 2025:  // $EA6B
      /*$EA6B*/ CYCLES(0xea6b, 6);
                branchTarget = true; push16(0xea6d); block_id = 2081;
      break;
    case 2026:  // $EA6E
      /*$EA6E*/ CYCLES(0xea6e, 16);
                s_a = 0x00;
      /*$EA70*/ s_status_c = 0x01;
      /*$EA71*/ block_id = s_status_d ? 2028 : 2027;
      break;
    case 2027:  // $EA71
      /*$EA71*/ tmp4_U16 = s_a;
                tmp6_U16 = ram_peek(0x009d);
                tmp3_U16 = (tmp4_U16 - tmp6_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)(~tmp6_U16));
                s_a = ((uint8_t)tmp3_U16);
                block_id = 2029;
      break;
    case 2028:  // $EA71
      /*$EA71*/ tmp3_U16 = sbc_dec16(s_a, ram_peek(0x009d), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                block_id = 2029;
      break;
    case 2029:  // $EA73
      /*$EA73*/ ram_poke(0x009d, s_a);
      /*$EA75*/ branchTarget = true; push16(0xea77); block_id = 1991;
      break;
    case 2030:  // $EA78
      /*$EA78*/ CYCLES(0xea78, 7);
                tmp5_U8 = (uint8_t)(ram_peek(0x009d) + 0x01);
                ram_poke(0x009d, tmp5_U8);
      /*$EA7A*/ branchTarget = true; block_id = !tmp5_U8 ? 2031 : 2032;
      break;
    case 2031:  // $EA7A
      /*$EA7A*/ CYCLES_EDGE(0xea7a, 1);
                branchTarget = true; block_id = 2009;
      break;
    case 2032:  // $EA7C
      /*$EA7C*/ CYCLES(0xea7c, 4);
                s_x = 0xfc;
      /*$EA7E*/ s_a = 0x01;
                block_id = 2033;
      break;
    case 2033:  // $EA80
      /*$EA80*/ CYCLES(0xea80, 8);
                tmp5_U8 = ram_peek(0x00a6);
      /*$EA82*/ tmp1_U8 = ram_peek(0x009e);
                tmp2_U8 = tmp5_U8 != tmp1_U8;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp5_U8 >= tmp1_U8);
                s_status_n = ((uint8_t)(tmp5_U8 - tmp1_U8) & 0x80);
      /*$EA84*/ branchTarget = true; block_id = tmp2_U8 ? 2034 : 2035;
      break;
    case 2034:  // $EA84
      /*$EA84*/ CYCLES_EDGE(0xea84, 1);
                branchTarget = true; block_id = 2040;
      break;
    case 2035:  // $EA86
      /*$EA86*/ CYCLES(0xea86, 8);
                tmp2_U8 = ram_peek(0x00a7);
      /*$EA88*/ tmp1_U8 = ram_peek(0x009f);
                tmp5_U8 = tmp2_U8 != tmp1_U8;
                s_status_not_z = tmp5_U8;
                s_status_c = (tmp2_U8 >= tmp1_U8);
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
      /*$EA8A*/ branchTarget = true; block_id = tmp5_U8 ? 2036 : 2037;
      break;
    case 2036:  // $EA8A
      /*$EA8A*/ CYCLES_EDGE(0xea8a, 1);
                branchTarget = true; block_id = 2040;
      break;
    case 2037:  // $EA8C
      /*$EA8C*/ CYCLES(0xea8c, 8);
                tmp5_U8 = ram_peek(0x00a8);
      /*$EA8E*/ tmp1_U8 = ram_peek(0x00a0);
                tmp2_U8 = tmp5_U8 != tmp1_U8;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp5_U8 >= tmp1_U8);
                s_status_n = ((uint8_t)(tmp5_U8 - tmp1_U8) & 0x80);
      /*$EA90*/ branchTarget = true; block_id = tmp2_U8 ? 2038 : 2039;
      break;
    case 2038:  // $EA90
      /*$EA90*/ CYCLES_EDGE(0xea90, 1);
                branchTarget = true; block_id = 2040;
      break;
    case 2039:  // $EA92
      /*$EA92*/ CYCLES(0xea92, 6);
                tmp2_U8 = ram_peek(0x00a9);
      /*$EA94*/ tmp1_U8 = ram_peek(0x00a1);
                s_status_not_z = (tmp2_U8 != tmp1_U8);
                s_status_c = (tmp2_U8 >= tmp1_U8);
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
                block_id = 2040;
      break;
    case 2040:  // $EA96
      /*$EA96*/ CYCLES(0xea96, 7);
                tmp2_U8 = s_status_c;
                push8((tmp2_U8 | ((s_status_not_z == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | s_status_n));
      /*$EA97*/ tmp3_U16 = (s_a << 0x01) | tmp2_U8;
                s_a = ((uint8_t)tmp3_U16);
      /*$EA98*/ branchTarget = true; block_id = !(uint8_t)(tmp3_U16 >> 8) ? 2041 : 2042;
      break;
    case 2041:  // $EA98
      /*$EA98*/ CYCLES_EDGE(0xea98, 1);
                branchTarget = true; block_id = 2047;
      break;
    case 2042:  // $EA9A
      /*$EA9A*/ CYCLES(0xea9a, 8);
                tmp2_U8 = s_x;
                tmp1_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$EA9B*/ ram_poke((uint8_t)(tmp2_U8 + 0x66), s_a);
      /*$EA9D*/ branchTarget = true; block_id = !tmp1_U8 ? 2043 : 2044;
      break;
    case 2043:  // $EA9D
      /*$EA9D*/ CYCLES_EDGE(0xea9d, 1);
      /*$EAD1*/ CYCLES(0xead1, 4);
                s_a = 0x40;
      /*$EAD3*/ CYCLES_EDGE(0xead3, 1);
                branchTarget = true; block_id = 2047;
      break;
    case 2044:  // $EA9F
      /*$EA9F*/ CYCLES(0xea9f, 2);
                branchTarget = true; block_id = !s_status_n ? 2045 : 2046;
      break;
    case 2045:  // $EA9F
      /*$EA9F*/ CYCLES_EDGE(0xea9f, 1);
      /*$EAD5*/ CYCLES(0xead5, 22);
      /*$EADB*/ ram_poke(0x00ac, (uint8_t)(s_a << 0x06));
      /*$EADD*/ tmp1_U8 = pop8();
                s_status_i = ((tmp1_U8 & 0x04) != 0);
                s_status_d = ((tmp1_U8 & 0x08) != 0);
                s_status_b = 0x00;
      /*$EADE*/ branchTarget = true; block_id = 2068;
      break;
    case 2046:  // $EAA1
      /*$EAA1*/ CYCLES(0xeaa1, 2);
                s_a = 0x01;
                block_id = 2047;
      break;
    case 2047:  // $EAA3
      /*$EAA3*/ CYCLES(0xeaa3, 6);
                tmp1_U8 = pop8();
                tmp2_U8 = tmp1_U8 & 0x01;
                s_status_c = tmp2_U8;
                s_status_i = ((tmp1_U8 & 0x04) != 0);
                s_status_d = ((tmp1_U8 & 0x08) != 0);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
      /*$EAA4*/ branchTarget = true; block_id = tmp2_U8 ? 2048 : 2049;
      break;
    case 2048:  // $EAA4
      /*$EAA4*/ CYCLES_EDGE(0xeaa4, 1);
                branchTarget = true; block_id = 2055;
      break;
    case 2049:  // $EAA6
      /*$EAA6*/ CYCLES(0xeaa6, 22);
                tmp3_U16 = ram_peek(0x00a9) << 0x01;
                ram_poke(0x00a9, ((uint8_t)tmp3_U16));
      /*$EAA8*/ tmp3_U16 = (ram_peek(0x00a8) << 0x01) | (uint8_t)(tmp3_U16 >> 8);
                ram_poke(0x00a8, ((uint8_t)tmp3_U16));
      /*$EAAA*/ tmp3_U16 = (ram_peek(0x00a7) << 0x01) | (uint8_t)(tmp3_U16 >> 8);
                ram_poke(0x00a7, ((uint8_t)tmp3_U16));
      /*$EAAC*/ tmp3_U16 = (ram_peek(0x00a6) << 0x01) | (uint8_t)(tmp3_U16 >> 8);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = tmp1_U8;
                tmp2_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x00a6, tmp2_U8);
      /*$EAAE*/ branchTarget = true; block_id = tmp1_U8 ? 2050 : 2051;
      break;
    case 2050:  // $EAAE
      /*$EAAE*/ CYCLES_EDGE(0xeaae, 1);
                branchTarget = true; block_id = 2040;
      break;
    case 2051:  // $EAB0
      /*$EAB0*/ CYCLES(0xeab0, 2);
                branchTarget = true; block_id = s_status_n ? 2052 : 2053;
      break;
    case 2052:  // $EAB0
      /*$EAB0*/ CYCLES_EDGE(0xeab0, 1);
                branchTarget = true; block_id = 2033;
      break;
    case 2053:  // $EAB2
      /*$EAB2*/ CYCLES(0xeab2, 2);
                branchTarget = true; block_id = !s_status_n ? 2054 : 2055;
      break;
    case 2054:  // $EAB2
      /*$EAB2*/ CYCLES_EDGE(0xeab2, 1);
                branchTarget = true; block_id = 2040;
      break;
    case 2055:  // $EAB4
      /*$EAB4*/ CYCLES(0xeab4, 43);
                s_y = s_a;
      /*$EAB5*/ s_a = ram_peek(0x00a9);
      /*$EAB7*/ block_id = s_status_d ? 2057 : 2056;
      break;
    case 2056:  // $EAB7
      /*$EAB7*/ tmp3_U16 = (s_a - ram_peek(0x00a1)) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp3_U16);
                block_id = 2058;
      break;
    case 2057:  // $EAB7
      /*$EAB7*/ tmp3_U16 = sbc_dec16(s_a, ram_peek(0x00a1), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 2058;
      break;
    case 2058:  // $EAB9
      /*$EAB9*/ ram_poke(0x00a9, s_a);
      /*$EABB*/ s_a = ram_peek(0x00a8);
      /*$EABD*/ block_id = s_status_d ? 2060 : 2059;
      break;
    case 2059:  // $EABD
      /*$EABD*/ tmp3_U16 = (s_a - ram_peek(0x00a0)) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp3_U16);
                block_id = 2061;
      break;
    case 2060:  // $EABD
      /*$EABD*/ tmp3_U16 = sbc_dec16(s_a, ram_peek(0x00a0), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 2061;
      break;
    case 2061:  // $EABF
      /*$EABF*/ ram_poke(0x00a8, s_a);
      /*$EAC1*/ s_a = ram_peek(0x00a7);
      /*$EAC3*/ block_id = s_status_d ? 2063 : 2062;
      break;
    case 2062:  // $EAC3
      /*$EAC3*/ tmp3_U16 = (s_a - ram_peek(0x009f)) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp3_U16);
                block_id = 2064;
      break;
    case 2063:  // $EAC3
      /*$EAC3*/ tmp3_U16 = sbc_dec16(s_a, ram_peek(0x009f), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                block_id = 2064;
      break;
    case 2064:  // $EAC5
      /*$EAC5*/ ram_poke(0x00a7, s_a);
      /*$EAC7*/ s_a = ram_peek(0x00a6);
      /*$EAC9*/ block_id = s_status_d ? 2066 : 2065;
      break;
    case 2065:  // $EAC9
      /*$EAC9*/ tmp3_U16 = s_a;
                tmp6_U16 = ram_peek(0x009e);
                tmp4_U16 = (tmp3_U16 - tmp6_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp6_U16));
                s_a = ((uint8_t)tmp4_U16);
                block_id = 2067;
      break;
    case 2066:  // $EAC9
      /*$EAC9*/ tmp4_U16 = sbc_dec16(s_a, ram_peek(0x009e), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 2067;
      break;
    case 2067:  // $EACB
      /*$EACB*/ ram_poke(0x00a6, s_a);
      /*$EACD*/ s_a = s_y;
      /*$EACE*/ branchTarget = true; block_id = 2049;
      break;
    case 2068:  // $EAE6
      /*$EAE6*/ CYCLES(0xeae6, 27);
      /*$EAE8*/ ram_poke(0x009e, ram_peek(0x0062));
      /*$EAEC*/ ram_poke(0x009f, ram_peek(0x0063));
      /*$EAF0*/ ram_poke(0x00a0, ram_peek(0x0064));
      /*$EAF4*/ ram_poke(0x00a1, ram_peek(0x0065));
      /*$EAF6*/ branchTarget = true; block_id = 1889;
      break;
    case 2069:  // $EAF9
      /*$EAF9*/ CYCLES(0xeaf9, 70);
                ram_poke(0x005e, s_a);
      /*$EAFB*/ ram_poke(0x005f, s_y);
      /*$EAFF*/ tmp2_U8 = peek((ram_peek16al(0x005e) + 0x0004));
      /*$EB01*/ ram_poke(0x00a1, tmp2_U8);
      /*$EB04*/ tmp2_U8 = peek((ram_peek16al(0x005e) + 0x0003));
      /*$EB06*/ ram_poke(0x00a0, tmp2_U8);
      /*$EB09*/ tmp2_U8 = peek((ram_peek16al(0x005e) + 0x0002));
      /*$EB0B*/ ram_poke(0x009f, tmp2_U8);
      /*$EB0E*/ tmp2_U8 = peek((ram_peek16al(0x005e) + 0x0001));
      /*$EB10*/ ram_poke(0x00a2, tmp2_U8);
      /*$EB14*/ ram_poke(0x009e, (tmp2_U8 | 0x80));
      /*$EB16*/ s_y = 0x00;
      /*$EB17*/ tmp2_U8 = peek(ram_peek16al(0x005e));
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$EB19*/ ram_poke(0x009d, tmp2_U8);
      /*$EB1B*/ ram_poke(0x00ac, 0x00);
      /*$EB1D*/ branchTarget = true; block_id = find_block_id_func_t001(0xeb1d, pop16() + 1);;
      break;
    case 2070:  // $EB2B
      /*$EB2B*/ CYCLES(0xeb2b, 6);
                branchTarget = true; push16(0xeb2d); block_id = 2081;
      break;
    case 2071:  // $EB2E
      /*$EB2E*/ CYCLES(0xeb2e, 75);
                ram_poke(0x005e, s_x);
      /*$EB30*/ ram_poke(0x005f, s_y);
      /*$EB36*/ poke((ram_peek16al(0x005e) + 0x0004), ram_peek(0x00a1));
      /*$EB3B*/ poke((ram_peek16al(0x005e) + 0x0003), ram_peek(0x00a0));
      /*$EB40*/ poke((ram_peek16al(0x005e) + 0x0002), ram_peek(0x009f));
      /*$EB49*/ poke((ram_peek16al(0x005e) + 0x0001), ((ram_peek(0x00a2) | 0x7f) & ram_peek(0x009e)));
      /*$EB4B*/ s_y = 0x00;
      /*$EB4C*/ tmp1_U8 = ram_peek(0x009d);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$EB4E*/ poke(ram_peek16al(0x005e), tmp1_U8);
      /*$EB50*/ ram_poke(0x00ac, 0x00);
      /*$EB52*/ branchTarget = true; block_id = find_block_id_func_t001(0xeb52, pop16() + 1);;
      break;
    case 2072:  // $EB59
      /*$EB59*/ CYCLES(0xeb59, 12);
                tmp1_U8 = s_x;
                tmp2_U8 = ram_peek((uint8_t)(0xa4 + tmp1_U8));
                s_a = tmp2_U8;
      /*$EB5B*/ ram_poke((uint8_t)(0x9c + tmp1_U8), tmp2_U8);
      /*$EB5D*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$EB5E*/ branchTarget = true; block_id = tmp1_U8 ? 2073 : 2074;
      break;
    case 2073:  // $EB5E
      /*$EB5E*/ CYCLES_EDGE(0xeb5e, 1);
                branchTarget = true; block_id = 2072;
      break;
    case 2074:  // $EB60
      /*$EB60*/ CYCLES(0xeb60, 9);
                ram_poke(0x00ac, s_x);
      /*$EB62*/ branchTarget = true; block_id = find_block_id_func_t001(0xeb62, pop16() + 1);;
      break;
    case 2075:  // $EB63
      /*$EB63*/ CYCLES(0xeb63, 6);
                branchTarget = true; push16(0xeb65); block_id = 2081;
      break;
    case 2076:  // $EB66
      /*$EB66*/ CYCLES(0xeb66, 2);
                s_x = 0x06;
                block_id = 2077;
      break;
    case 2077:  // $EB68
      /*$EB68*/ CYCLES(0xeb68, 12);
                tmp1_U8 = s_x;
                tmp2_U8 = ram_peek((uint8_t)(0x9c + tmp1_U8));
                s_a = tmp2_U8;
      /*$EB6A*/ ram_poke((uint8_t)(0xa4 + tmp1_U8), tmp2_U8);
      /*$EB6C*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$EB6D*/ branchTarget = true; block_id = tmp1_U8 ? 2078 : 2079;
      break;
    case 2078:  // $EB6D
      /*$EB6D*/ CYCLES_EDGE(0xeb6d, 1);
                branchTarget = true; block_id = 2077;
      break;
    case 2079:  // $EB6F
      /*$EB6F*/ CYCLES(0xeb6f, 3);
                ram_poke(0x00ac, s_x);
                block_id = 2080;
      break;
    case 2080:  // $EB71
      /*$EB71*/ CYCLES(0xeb71, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xeb71, pop16() + 1);;
      break;
    case 2081:  // $EB72
      /*$EB72*/ CYCLES(0xeb72, 5);
                tmp1_U8 = ram_peek(0x009d);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$EB74*/ branchTarget = true; block_id = !tmp1_U8 ? 2082 : 2083;
      break;
    case 2082:  // $EB74
      /*$EB74*/ CYCLES_EDGE(0xeb74, 1);
                branchTarget = true; block_id = 2080;
      break;
    case 2083:  // $EB76
      /*$EB76*/ CYCLES(0xeb76, 7);
                tmp4_U16 = ram_peek(0x00ac) << 0x01;
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = tmp1_U8;
                tmp2_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x00ac, tmp2_U8);
      /*$EB78*/ branchTarget = true; block_id = !tmp1_U8 ? 2084 : 2085;
      break;
    case 2084:  // $EB78
      /*$EB78*/ CYCLES_EDGE(0xeb78, 1);
                branchTarget = true; block_id = 2080;
      break;
    case 2085:  // $EB7A
      /*$EB7A*/ CYCLES(0xeb7a, 6);
                FUNC_INCFACMAN(0xeb7c);
      /*$EB7D*/ CYCLES(0xeb7d, 2);
                branchTarget = true; block_id = s_status_not_z ? 2086 : 2087;
      break;
    case 2086:  // $EB7D
      /*$EB7D*/ CYCLES_EDGE(0xeb7d, 1);
                branchTarget = true; block_id = 2080;
      break;
    case 2087:  // $EB7F
      /*$EB7F*/ CYCLES(0xeb7f, 3);
                branchTarget = true; block_id = 1930;
      break;
    case 2088:  // $EB84
      /*$EB84*/ CYCLES_EDGE(0xeb84, 1);
                branchTarget = true; block_id = 2093;
      break;
    case 2089:  // $EB86
      /*$EB86*/ CYCLES(0xeb86, 3);
                s_a = ram_peek(0x00a2);
                block_id = 2090;
      break;
    case 2090:  // $EB88
      /*$EB88*/ CYCLES(0xeb88, 6);
                tmp1_U8 = (uint8_t)(((s_a << 0x01) | s_status_c) >> 8);
                s_status_c = tmp1_U8;
      /*$EB89*/ s_status_not_z = 0xff;
                s_status_n = 0x80;
                s_a = 0xff;
      /*$EB8B*/ branchTarget = true; block_id = tmp1_U8 ? 2091 : 2092;
      break;
    case 2091:  // $EB8B
      /*$EB8B*/ CYCLES_EDGE(0xeb8b, 1);
                branchTarget = true; block_id = 2093;
      break;
    case 2092:  // $EB8D
      /*$EB8D*/ CYCLES(0xeb8d, 2);
                s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_a = 0x01;
                block_id = 2093;
      break;
    case 2093:  // $EB8F
      /*$EB8F*/ CYCLES(0xeb8f, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xeb8f, pop16() + 1);;
      break;
    case 2094:  // $EB93
      /*$EB93*/ CYCLES(0xeb93, 10);
                ram_poke(0x009e, s_a);
      /*$EB97*/ ram_poke(0x009f, 0x00);
      /*$EB99*/ s_x = 0x88;
                block_id = 2095;
      break;
    case 2095:  // $EB9B
      /*$EB9B*/ CYCLES(0xeb9b, 7);
      /*$EB9F*/ s_status_c = (uint8_t)((((ram_peek(0x009e) ^ 0xff) << 0x01) | s_status_c) >> 8);
                block_id = 2096;
      break;
    case 2096:  // $EBA0
      /*$EBA0*/ CYCLES(0xeba0, 20);
      /*$EBA2*/ ram_poke(0x00a1, 0x00);
      /*$EBA4*/ ram_poke(0x00a0, 0x00);
      /*$EBA6*/ ram_poke(0x009d, s_x);
      /*$EBA8*/ ram_poke(0x00ac, 0x00);
      /*$EBAA*/ ram_poke(0x00a2, 0x00);
      /*$EBAC*/ branchTarget = true; block_id = 1886;
      break;
    case 2097:  // $EBB2
      /*$EBB2*/ CYCLES(0xebb2, 19);
                ram_poke(0x0060, s_a);
      /*$EBB4*/ ram_poke(0x0061, s_y);
      /*$EBB8*/ tmp5_U8 = peek(ram_peek16al(0x0060));
      /*$EBBA*/ s_y = 0x01;
      /*$EBBB*/ s_x = tmp5_U8;
      /*$EBBC*/ branchTarget = true; block_id = !tmp5_U8 ? 2098 : 2099;
      break;
    case 2098:  // $EBBC
      /*$EBBC*/ CYCLES_EDGE(0xebbc, 1);
      /*$EB82*/ CYCLES(0xeb82, 5);
                tmp1_U8 = ram_peek(0x009d);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$EB84*/ branchTarget = true; block_id = !tmp1_U8 ? 2088 : 2089;
      break;
    case 2099:  // $EBBE
      /*$EBBE*/ CYCLES(0xebbe, 10);
                tmp1_U8 = peek((ram_peek16al(0x0060) + s_y));
      /*$EBC2*/ branchTarget = true; block_id = ((tmp1_U8 ^ ram_peek(0x00a2)) & 0x80) ? 2100 : 2101;
      break;
    case 2100:  // $EBC2
      /*$EBC2*/ CYCLES_EDGE(0xebc2, 1);
                branchTarget = true; block_id = 2089;
      break;
    case 2101:  // $EBC4
      /*$EBC4*/ CYCLES(0xebc4, 5);
                tmp1_U8 = s_x;
                tmp2_U8 = ram_peek(0x009d);
                s_status_c = (tmp1_U8 >= tmp2_U8);
      /*$EBC6*/ branchTarget = true; block_id = (tmp1_U8 != tmp2_U8) ? 2102 : 2103;
      break;
    case 2102:  // $EBC6
      /*$EBC6*/ CYCLES_EDGE(0xebc6, 1);
                branchTarget = true; block_id = 2114;
      break;
    case 2103:  // $EBC8
      /*$EBC8*/ CYCLES(0xebc8, 12);
                tmp2_U8 = peek((ram_peek16al(0x0060) + s_y));
      /*$EBCA*/ tmp2_U8 = tmp2_U8 | 0x80;
      /*$EBCC*/ tmp1_U8 = ram_peek(0x009e);
                s_status_c = (tmp2_U8 >= tmp1_U8);
      /*$EBCE*/ branchTarget = true; block_id = (tmp2_U8 != tmp1_U8) ? 2104 : 2105;
      break;
    case 2104:  // $EBCE
      /*$EBCE*/ CYCLES_EDGE(0xebce, 1);
                branchTarget = true; block_id = 2114;
      break;
    case 2105:  // $EBD0
      /*$EBD0*/ CYCLES(0xebd0, 12);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$EBD1*/ tmp1_U8 = peek((ram_peek16al(0x0060) + tmp1_U8));
      /*$EBD3*/ tmp2_U8 = ram_peek(0x009f);
                s_status_c = (tmp1_U8 >= tmp2_U8);
      /*$EBD5*/ branchTarget = true; block_id = (tmp1_U8 != tmp2_U8) ? 2106 : 2107;
      break;
    case 2106:  // $EBD5
      /*$EBD5*/ CYCLES_EDGE(0xebd5, 1);
                branchTarget = true; block_id = 2114;
      break;
    case 2107:  // $EBD7
      /*$EBD7*/ CYCLES(0xebd7, 12);
                tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$EBD8*/ tmp2_U8 = peek((ram_peek16al(0x0060) + tmp2_U8));
      /*$EBDA*/ tmp1_U8 = ram_peek(0x00a0);
                s_status_c = (tmp2_U8 >= tmp1_U8);
      /*$EBDC*/ branchTarget = true; block_id = (tmp2_U8 != tmp1_U8) ? 2108 : 2109;
      break;
    case 2108:  // $EBDC
      /*$EBDC*/ CYCLES_EDGE(0xebdc, 1);
                branchTarget = true; block_id = 2114;
      break;
    case 2109:  // $EBDE
      /*$EBDE*/ CYCLES(0xebde, 17);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$EBE1*/ s_status_c = (0x7f >= ram_peek(0x00ac));
      /*$EBE3*/ tmp1_U8 = peek((ram_peek16al(0x0060) + tmp1_U8));
                s_a = tmp1_U8;
      /*$EBE5*/ block_id = s_status_d ? 2111 : 2110;
      break;
    case 2110:  // $EBE5
      /*$EBE5*/ tmp4_U16 = s_a;
                tmp6_U16 = ram_peek(0x00a1);
                tmp3_U16 = (tmp4_U16 - tmp6_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)(~tmp6_U16));
                tmp1_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 2112;
      break;
    case 2111:  // $EBE5
      /*$EBE5*/ tmp3_U16 = sbc_dec16(s_a, ram_peek(0x00a1), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
                block_id = 2112;
      break;
    case 2112:  // $EBE7
      /*$EBE7*/ branchTarget = true; block_id = !s_status_not_z ? 2113 : 2114;
      break;
    case 2113:  // $EBE7
      /*$EBE7*/ CYCLES_EDGE(0xebe7, 1);
                branchTarget = true; block_id = 2130;
      break;
    case 2114:  // $EBE9
      /*$EBE9*/ CYCLES(0xebe9, 5);
                s_a = ram_peek(0x00a2);
      /*$EBEB*/ branchTarget = true; block_id = !s_status_c ? 2115 : 2116;
      break;
    case 2115:  // $EBEB
      /*$EBEB*/ CYCLES_EDGE(0xebeb, 1);
                branchTarget = true; block_id = 2117;
      break;
    case 2116:  // $EBED
      /*$EBED*/ CYCLES(0xebed, 2);
                s_a = (s_a ^ 0xff);
                block_id = 2117;
      break;
    case 2117:  // $EBEF
      /*$EBEF*/ CYCLES(0xebef, 3);
                branchTarget = true; block_id = 2090;
      break;
    case 2118:  // $EBF2
      /*$EBF2*/ CYCLES(0xebf2, 5);
                tmp5_U8 = ram_peek(0x009d);
                s_a = tmp5_U8;
      /*$EBF4*/ branchTarget = true; block_id = !tmp5_U8 ? 2119 : 2120;
      break;
    case 2119:  // $EBF4
      /*$EBF4*/ CYCLES_EDGE(0xebf4, 1);
      /*$EC40*/ CYCLES(0xec40, 20);
                tmp1_U8 = s_a;
                ram_poke(0x009e, tmp1_U8);
      /*$EC42*/ ram_poke(0x009f, tmp1_U8);
      /*$EC44*/ ram_poke(0x00a0, tmp1_U8);
      /*$EC46*/ ram_poke(0x00a1, tmp1_U8);
      /*$EC48*/ s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
      /*$EC49*/ branchTarget = true; block_id = find_block_id_func_t001(0xec49, pop16() + 1);;
      break;
    case 2120:  // $EBF6
      /*$EBF6*/ CYCLES(0xebf6, 9);
                s_status_c = 0x01;
      /*$EBF7*/ block_id = s_status_d ? 2122 : 2121;
      break;
    case 2121:  // $EBF7
      /*$EBF7*/ s_a = (uint8_t)((s_a - 0x00a0) - (uint8_t)(0x01 - s_status_c));
                block_id = 2123;
      break;
    case 2122:  // $EBF7
      /*$EBF7*/ s_a = ((uint8_t)sbc_dec16(s_a, 0xa0, s_status_c));
                block_id = 2123;
      break;
    case 2123:  // $EBF9
      /*$EBF9*/ tmp1_U8 = ram_peek(0x00a2);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
      /*$EBFB*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 2124 : 2125;
      break;
    case 2124:  // $EBFB
      /*$EBFB*/ CYCLES_EDGE(0xebfb, 1);
                branchTarget = true; block_id = 2126;
      break;
    case 2125:  // $EBFD
      /*$EBFD*/ CYCLES(0xebfd, 13);
                tmp1_U8 = s_a;
      /*$EC00*/ ram_poke(0x00a4, 0xff);
      /*$EC02*/ FUNC_TCFACMANT(0xec04);
      /*$EC05*/ CYCLES(0xec05, 2);
                s_a = tmp1_U8;
                block_id = 2126;
      break;
    case 2126:  // $EC06
      /*$EC06*/ CYCLES(0xec06, 6);
                s_x = 0x9d;
      /*$EC08*/ tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0xf9);
      /*$EC0A*/ branchTarget = true; block_id = !((uint8_t)(tmp1_U8 - 0xf9) & 0x80) ? 2127 : 2128;
      break;
    case 2127:  // $EC0A
      /*$EC0A*/ CYCLES_EDGE(0xec0a, 1);
      /*$EC12*/ CYCLES(0xec12, 24);
                s_y = s_a;
      /*$EC17*/ tmp1_U8 = ram_peek(0x009e);
                s_status_c = (tmp1_U8 & 0x01);
                ram_poke(0x009e, (tmp1_U8 >> 0x01));
      /*$EC19*/ tmp1_U8 = (ram_peek(0x00a2) & 0x80) | ram_peek(0x009e);
                s_a = tmp1_U8;
      /*$EC1B*/ ram_poke(0x009e, tmp1_U8);
      /*$EC1D*/ FUNC_SHFTRGHT4(0xec1f);
      /*$EC20*/ CYCLES(0xec20, 9);
                ram_poke(0x00a4, s_y);
      /*$EC22*/ branchTarget = true; block_id = find_block_id_func_t001(0xec22, pop16() + 1);;
      break;
    case 2128:  // $EC0C
      /*$EC0C*/ CYCLES(0xec0c, 6);
                FUNC_SHFTRGHT(0xec0e);
                branchTarget = true; block_id = 2129;
      break;
    case 2129:  // $EC0F
      /*$EC0F*/ CYCLES(0xec0f, 3);
                ram_poke(0x00a4, s_y);
                block_id = 2130;
      break;
    case 2130:  // $EC11
      /*$EC11*/ CYCLES(0xec11, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xec11, pop16() + 1);;
      break;
    case 2131:  // $EC4E
      /*$EC4E*/ CYCLES(0xec4e, 8);
                tmp1_U8 = s_x;
                ram_poke((uint8_t)(0x99 + tmp1_U8), s_y);
      /*$EC50*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_x = tmp1_U8;
      /*$EC51*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 2132 : 2133;
      break;
    case 2132:  // $EC51
      /*$EC51*/ CYCLES_EDGE(0xec51, 1);
                branchTarget = true; block_id = 2131;
      break;
    case 2133:  // $EC53
      /*$EC53*/ CYCLES(0xec53, 2);
                branchTarget = true; block_id = !s_status_c ? 2134 : 2135;
      break;
    case 2134:  // $EC53
      /*$EC53*/ CYCLES_EDGE(0xec53, 1);
                branchTarget = true; block_id = 2142;
      break;
    case 2135:  // $EC55
      /*$EC55*/ CYCLES(0xec55, 4);
                tmp1_U8 = s_a;
                tmp2_U8 = tmp1_U8 != 0x2d;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp1_U8 >= 0x2d);
      /*$EC57*/ branchTarget = true; block_id = tmp2_U8 ? 2136 : 2137;
      break;
    case 2136:  // $EC57
      /*$EC57*/ CYCLES_EDGE(0xec57, 1);
                branchTarget = true; block_id = 2139;
      break;
    case 2137:  // $EC59
      /*$EC59*/ CYCLES(0xec59, 5);
                ram_poke(0x00a3, s_x);
      /*$EC5B*/ branchTarget = true; block_id = !s_status_not_z ? 2138 : 2139;
      break;
    case 2138:  // $EC5B
      /*$EC5B*/ CYCLES_EDGE(0xec5b, 1);
                branchTarget = true; block_id = 2141;
      break;
    case 2139:  // $EC5D
      /*$EC5D*/ CYCLES(0xec5d, 4);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x2b);
      /*$EC5F*/ branchTarget = true; block_id = (tmp2_U8 != 0x2b) ? 2140 : 2141;
      break;
    case 2140:  // $EC5F
      /*$EC5F*/ CYCLES_EDGE(0xec5f, 1);
                branchTarget = true; block_id = 2144;
      break;
    case 2141:  // $EC61
      /*$EC61*/ CYCLES(0xec61, 6);
                branchTarget = true; push16(0xec63); block_id = 2;
      break;
    case 2142:  // $EC64
      /*$EC64*/ CYCLES(0xec64, 2);
                branchTarget = true; block_id = !s_status_c ? 2143 : 2144;
      break;
    case 2143:  // $EC64
      /*$EC64*/ CYCLES_EDGE(0xec64, 1);
      /*$ECC1*/ CYCLES(0xecc1, 8);
                push8(s_a);
      /*$ECC2*/ tmp2_U8 = ram_peek(0x009b);
                s_status_v = ((tmp2_U8 >> 0x06) & 0x01);
      /*$ECC4*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 2191 : 2192;
      break;
    case 2144:  // $EC66
      /*$EC66*/ CYCLES(0xec66, 4);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x2e);
      /*$EC68*/ branchTarget = true; block_id = !(tmp2_U8 != 0x2e) ? 2145 : 2146;
      break;
    case 2145:  // $EC68
      /*$EC68*/ CYCLES_EDGE(0xec68, 1);
      /*$EC98*/ CYCLES(0xec98, 10);
                tmp2_U8 = ram_peek(0x009b);
                ram_poke(0x009b, (uint8_t)((tmp2_U8 | (0x00 + (s_status_c << 8))) >> 0x01));
                s_status_c = (tmp2_U8 & 0x01);
      /*$EC9A*/ tmp2_U8 = (ram_peek(0x009b) >> 0x06) & 0x01;
                s_status_v = tmp2_U8;
      /*$EC9C*/ branchTarget = true; block_id = !tmp2_U8 ? 2171 : 2172;
      break;
    case 2146:  // $EC6A
      /*$EC6A*/ CYCLES(0xec6a, 4);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x45);
      /*$EC6C*/ branchTarget = true; block_id = (tmp2_U8 != 0x45) ? 2147 : 2148;
      break;
    case 2147:  // $EC6C
      /*$EC6C*/ CYCLES_EDGE(0xec6c, 1);
                branchTarget = true; block_id = 2172;
      break;
    case 2148:  // $EC6E
      /*$EC6E*/ CYCLES(0xec6e, 6);
                branchTarget = true; push16(0xec70); block_id = 2;
      break;
    case 2149:  // $EC71
      /*$EC71*/ CYCLES(0xec71, 2);
                branchTarget = true; block_id = !s_status_c ? 2150 : 2151;
      break;
    case 2150:  // $EC71
      /*$EC71*/ CYCLES_EDGE(0xec71, 1);
                branchTarget = true; block_id = 2163;
      break;
    case 2151:  // $EC73
      /*$EC73*/ CYCLES(0xec73, 4);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0xc9);
      /*$EC75*/ branchTarget = true; block_id = !(tmp2_U8 != 0xc9) ? 2152 : 2153;
      break;
    case 2152:  // $EC75
      /*$EC75*/ CYCLES_EDGE(0xec75, 1);
                branchTarget = true; block_id = 2161;
      break;
    case 2153:  // $EC77
      /*$EC77*/ CYCLES(0xec77, 4);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x2d);
      /*$EC79*/ branchTarget = true; block_id = !(tmp2_U8 != 0x2d) ? 2154 : 2155;
      break;
    case 2154:  // $EC79
      /*$EC79*/ CYCLES_EDGE(0xec79, 1);
                branchTarget = true; block_id = 2161;
      break;
    case 2155:  // $EC7B
      /*$EC7B*/ CYCLES(0xec7b, 4);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0xc8);
      /*$EC7D*/ branchTarget = true; block_id = !(tmp2_U8 != 0xc8) ? 2156 : 2157;
      break;
    case 2156:  // $EC7D
      /*$EC7D*/ CYCLES_EDGE(0xec7d, 1);
                branchTarget = true; block_id = 2162;
      break;
    case 2157:  // $EC7F
      /*$EC7F*/ CYCLES(0xec7f, 4);
                tmp2_U8 = s_a;
                tmp1_U8 = tmp2_U8 != 0x2b;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp2_U8 >= 0x2b);
      /*$EC81*/ branchTarget = true; block_id = !tmp1_U8 ? 2158 : 2159;
      break;
    case 2158:  // $EC81
      /*$EC81*/ CYCLES_EDGE(0xec81, 1);
                branchTarget = true; block_id = 2162;
      break;
    case 2159:  // $EC83
      /*$EC83*/ CYCLES(0xec83, 2);
                branchTarget = true; block_id = s_status_not_z ? 2160 : 2161;
      break;
    case 2160:  // $EC83
      /*$EC83*/ CYCLES_EDGE(0xec83, 1);
                branchTarget = true; block_id = 2165;
      break;
    case 2161:  // $EC85
      /*$EC85*/ CYCLES(0xec85, 5);
                tmp2_U8 = ram_peek(0x009c);
                ram_poke(0x009c, (uint8_t)((tmp2_U8 | (0x00 + (s_status_c << 8))) >> 0x01));
                s_status_c = (tmp2_U8 & 0x01);
                block_id = 2162;
      break;
    case 2162:  // $EC87
      /*$EC87*/ CYCLES(0xec87, 6);
                branchTarget = true; push16(0xec89); block_id = 2;
      break;
    case 2163:  // $EC8A
      /*$EC8A*/ CYCLES(0xec8a, 2);
                branchTarget = true; block_id = !s_status_c ? 2164 : 2165;
      break;
    case 2164:  // $EC8A
      /*$EC8A*/ CYCLES_EDGE(0xec8a, 1);
      /*$ECE8*/ CYCLES(0xece8, 7);
                tmp1_U8 = ram_peek(0x009a);
                s_a = tmp1_U8;
      /*$ECEA*/ tmp1_U8 = tmp1_U8 >= 0x0a;
                s_status_c = tmp1_U8;
      /*$ECEC*/ branchTarget = true; block_id = !tmp1_U8 ? 2202 : 2203;
      break;
    case 2165:  // $EC8C
      /*$EC8C*/ CYCLES(0xec8c, 5);
      /*$EC8E*/ branchTarget = true; block_id = !(ram_peek(0x009c) & 0x80) ? 2166 : 2167;
      break;
    case 2166:  // $EC8E
      /*$EC8E*/ CYCLES_EDGE(0xec8e, 1);
                branchTarget = true; block_id = 2172;
      break;
    case 2167:  // $EC90
      /*$EC90*/ CYCLES(0xec90, 10);
                s_a = 0x00;
      /*$EC92*/ s_status_c = 0x01;
      /*$EC93*/ block_id = s_status_d ? 2169 : 2168;
      break;
    case 2168:  // $EC93
      /*$EC93*/ s_a = (uint8_t)((s_a - ram_peek(0x009a)) - (uint8_t)(0x01 - s_status_c));
                block_id = 2170;
      break;
    case 2169:  // $EC93
      /*$EC93*/ s_a = ((uint8_t)sbc_dec16(s_a, ram_peek(0x009a), s_status_c));
                block_id = 2170;
      break;
    case 2170:  // $EC95
      /*$EC95*/ branchTarget = true; block_id = 2173;
      break;
    case 2171:  // $EC9C
      /*$EC9C*/ CYCLES_EDGE(0xec9c, 1);
                branchTarget = true; block_id = 2141;
      break;
    case 2172:  // $EC9E
      /*$EC9E*/ CYCLES(0xec9e, 3);
                s_a = ram_peek(0x009a);
                block_id = 2173;
      break;
    case 2173:  // $ECA0
      /*$ECA0*/ CYCLES(0xeca0, 10);
                s_status_c = 0x01;
      /*$ECA1*/ block_id = s_status_d ? 2175 : 2174;
      break;
    case 2174:  // $ECA1
      /*$ECA1*/ tmp3_U16 = s_a;
                tmp6_U16 = ram_peek(0x0099);
                tmp4_U16 = (tmp3_U16 - tmp6_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp6_U16));
                tmp1_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 2176;
      break;
    case 2175:  // $ECA1
      /*$ECA1*/ tmp4_U16 = sbc_dec16(s_a, ram_peek(0x0099), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
                block_id = 2176;
      break;
    case 2176:  // $ECA3
      /*$ECA3*/ ram_poke(0x009a, s_a);
      /*$ECA5*/ branchTarget = true; block_id = !s_status_not_z ? 2177 : 2178;
      break;
    case 2177:  // $ECA5
      /*$ECA5*/ CYCLES_EDGE(0xeca5, 1);
                branchTarget = true; block_id = 2188;
      break;
    case 2178:  // $ECA7
      /*$ECA7*/ CYCLES(0xeca7, 2);
                branchTarget = true; block_id = !s_status_n ? 2179 : 2180;
      break;
    case 2179:  // $ECA7
      /*$ECA7*/ CYCLES_EDGE(0xeca7, 1);
                branchTarget = true; block_id = 2185;
      break;
    case 2180:  // $ECA9
      /*$ECA9*/ CYCLES(0xeca9, 6);
                branchTarget = true; push16(0xecab); block_id = 2022;
      break;
    case 2181:  // $ECAC
      /*$ECAC*/ CYCLES(0xecac, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x009a) + 0x01);
                s_status_not_z = tmp2_U8;
                ram_poke(0x009a, tmp2_U8);
      /*$ECAE*/ branchTarget = true; block_id = tmp2_U8 ? 2182 : 2183;
      break;
    case 2182:  // $ECAE
      /*$ECAE*/ CYCLES_EDGE(0xecae, 1);
                branchTarget = true; block_id = 2180;
      break;
    case 2183:  // $ECB0
      /*$ECB0*/ CYCLES(0xecb0, 2);
                branchTarget = true; block_id = !s_status_not_z ? 2184 : 2185;
      break;
    case 2184:  // $ECB0
      /*$ECB0*/ CYCLES_EDGE(0xecb0, 1);
                branchTarget = true; block_id = 2188;
      break;
    case 2185:  // $ECB2
      /*$ECB2*/ CYCLES(0xecb2, 6);
                branchTarget = true; push16(0xecb4); block_id = 2010;
      break;
    case 2186:  // $ECB5
      /*$ECB5*/ CYCLES(0xecb5, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x009a) - 0x01);
                ram_poke(0x009a, tmp2_U8);
      /*$ECB7*/ branchTarget = true; block_id = tmp2_U8 ? 2187 : 2188;
      break;
    case 2187:  // $ECB7
      /*$ECB7*/ CYCLES_EDGE(0xecb7, 1);
                branchTarget = true; block_id = 2185;
      break;
    case 2188:  // $ECB9
      /*$ECB9*/ CYCLES(0xecb9, 5);
                tmp1_U8 = ram_peek(0x00a3);
                s_status_not_z = tmp1_U8;
                tmp2_U8 = tmp1_U8 & 0x80;
                s_status_n = tmp2_U8;
                s_a = tmp1_U8;
      /*$ECBB*/ branchTarget = true; block_id = tmp2_U8 ? 2189 : 2190;
      break;
    case 2189:  // $ECBB
      /*$ECBB*/ CYCLES_EDGE(0xecbb, 1);
      /*$ECBE*/ CYCLES(0xecbe, 3);
      /*$EED0*/ CYCLES(0xeed0, 5);
                tmp2_U8 = ram_peek(0x009d);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$EED2*/ branchTarget = true; block_id = !tmp2_U8 ? 2332 : 2333;
      break;
    case 2190:  // $ECBD
      /*$ECBD*/ CYCLES(0xecbd, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xecbd, pop16() + 1);;
      break;
    case 2191:  // $ECC4
      /*$ECC4*/ CYCLES_EDGE(0xecc4, 1);
                branchTarget = true; block_id = 2193;
      break;
    case 2192:  // $ECC6
      /*$ECC6*/ CYCLES(0xecc6, 5);
                ram_poke(0x0099, (uint8_t)(ram_peek(0x0099) + 0x01));
                block_id = 2193;
      break;
    case 2193:  // $ECC8
      /*$ECC8*/ CYCLES(0xecc8, 6);
                branchTarget = true; push16(0xecca); block_id = 2010;
      break;
    case 2194:  // $ECCB
      /*$ECCB*/ CYCLES(0xeccb, 14);
                tmp5_U8 = pop8();
                s_a = tmp5_U8;
      /*$ECCC*/ s_status_c = 0x01;
      /*$ECCD*/ block_id = s_status_d ? 2196 : 2195;
      break;
    case 2195:  // $ECCD
      /*$ECCD*/ tmp4_U16 = s_a;
                tmp6_U16 = (tmp4_U16 - 0x0030) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0xffcf);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 2197;
      break;
    case 2196:  // $ECCD
      /*$ECCD*/ tmp6_U16 = sbc_dec16(s_a, 0x30, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp2_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 2197;
      break;
    case 2197:  // $ECCF
      /*$ECCF*/ branchTarget = true; push16(0xecd1); block_id = 2199;
      break;
    case 2198:  // $ECD2
      /*$ECD2*/ CYCLES(0xecd2, 3);
                branchTarget = true; block_id = 2141;
      break;
    case 2199:  // $ECD5
      /*$ECD5*/ CYCLES(0xecd5, 9);
                push8(s_a);
      /*$ECD6*/ branchTarget = true; push16(0xecd8); block_id = 2075;
      break;
    case 2200:  // $ECD9
      /*$ECD9*/ CYCLES(0xecd9, 10);
                tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$ECDA*/ branchTarget = true; push16(0xecdc); block_id = 2094;
      break;
    case 2201:  // $ECDD
      /*$ECDD*/ CYCLES(0xecdd, 15);
      /*$ECE1*/ ram_poke(0x00ab, (ram_peek(0x00aa) ^ ram_peek(0x00a2)));
      /*$ECE3*/ s_status_not_z = ram_peek(0x009d);
      /*$ECE5*/ branchTarget = true; block_id = 1846;
      break;
    case 2202:  // $ECEC
      /*$ECEC*/ CYCLES_EDGE(0xecec, 1);
      /*$ECF7*/ CYCLES(0xecf7, 24);
      /*$ECF8*/ s_a = (uint8_t)(s_a << 0x02);
      /*$ECF9*/ s_status_c = 0x00;
      /*$ECFA*/ block_id = s_status_d ? 2207 : 2206;
      break;
    case 2203:  // $ECEE
      /*$ECEE*/ CYCLES(0xecee, 7);
                s_a = 0x64;
      /*$ECF0*/ tmp2_U8 = ram_peek(0x009c);
                s_status_v = ((tmp2_U8 >> 0x06) & 0x01);
      /*$ECF2*/ branchTarget = true; block_id = (tmp2_U8 & 0x80) ? 2204 : 2205;
      break;
    case 2204:  // $ECF2
      /*$ECF2*/ CYCLES_EDGE(0xecf2, 1);
                branchTarget = true; block_id = 2214;
      break;
    case 2205:  // $ECF4
      /*$ECF4*/ CYCLES(0xecf4, 3);
                branchTarget = true; block_id = 1945;
      break;
    case 2206:  // $ECFA
      /*$ECFA*/ s_a = (uint8_t)((s_a + ram_peek(0x009a)) + s_status_c);
                block_id = 2208;
      break;
    case 2207:  // $ECFA
      /*$ECFA*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x009a), s_status_c));
                block_id = 2208;
      break;
    case 2208:  // $ECFC
      /*$ECFC*/ s_a = (uint8_t)(s_a << 0x01);
      /*$ECFD*/ s_status_c = 0x00;
      /*$ECFE*/ s_y = 0x00;
      /*$ED00*/ block_id = s_status_d ? 2210 : 2209;
      break;
    case 2209:  // $ED00
      /*$ED00*/ tmp2_U8 = peek((ram_peek16al(0x00b8) + s_y));
                s_a = (uint8_t)((s_a + tmp2_U8) + s_status_c);
                block_id = 2211;
      break;
    case 2210:  // $ED00
      /*$ED00*/ tmp2_U8 = peek((ram_peek16al(0x00b8) + s_y));
                s_a = ((uint8_t)adc_dec16(s_a, tmp2_U8, s_status_c));
                block_id = 2211;
      break;
    case 2211:  // $ED02
      /*$ED02*/ s_status_c = 0x01;
      /*$ED03*/ block_id = s_status_d ? 2213 : 2212;
      break;
    case 2212:  // $ED03
      /*$ED03*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 - 0x0030) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0xffcf);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 2215;
      break;
    case 2213:  // $ED03
      /*$ED03*/ tmp4_U16 = sbc_dec16(s_a, 0x30, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 2215;
      break;
    case 2214:  // $ED05
      /*$ED05*/ CYCLES(0xed05, 6);
                ram_poke(0x009a, s_a);
      /*$ED07*/ branchTarget = true; block_id = 2162;
      break;
    case 2215:  // $ED05
      /*$ED03*/ block_id = 2214;
      break;
    case 2216:  // $ED19
      /*$ED19*/ CYCLES(0xed19, 10);
                s_a = 0x58;
      /*$ED1B*/ s_y = 0xd3;
      /*$ED1D*/ branchTarget = true; push16(0xed1f); block_id = 2220;
      break;
    case 2217:  // $ED20
      /*$ED20*/ CYCLES(0xed20, 6);
                s_a = ram_peek(0x0076);
      /*$ED22*/ s_x = ram_peek(0x0075);
                block_id = 2218;
      break;
    case 2218:  // $ED24
      /*$ED24*/ CYCLES(0xed24, 16);
                ram_poke(0x009e, s_a);
      /*$ED26*/ ram_poke(0x009f, s_x);
      /*$ED28*/ s_x = 0x90;
      /*$ED2A*/ s_status_c = 0x01;
      /*$ED2B*/ branchTarget = true; push16(0xed2d); block_id = 2096;
      break;
    case 2219:  // $ED2E
      /*$ED2E*/ CYCLES(0xed2e, 6);
                branchTarget = true; push16(0xed30); block_id = 2221;
      break;
    case 2220:  // $ED31
      /*$ED31*/ CYCLES(0xed31, 3);
                branchTarget = true; block_id = 1243;
      break;
    case 2221:  // $ED34
      /*$ED34*/ CYCLES(0xed34, 11);
      /*$ED36*/ s_a = 0x2d;
      /*$ED38*/ s_y = 0x00;
      /*$ED39*/ tmp2_U8 = ram_peek(0x00a2);
                s_status_v = ((tmp2_U8 >> 0x06) & 0x01);
      /*$ED3B*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 2222 : 2223;
      break;
    case 2222:  // $ED3B
      /*$ED3B*/ CYCLES_EDGE(0xed3b, 1);
                branchTarget = true; block_id = 2224;
      break;
    case 2223:  // $ED3D
      /*$ED3D*/ CYCLES(0xed3d, 7);
                tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$ED3E*/ ram_poke((0x00ff + tmp2_U8), s_a);
                block_id = 2224;
      break;
    case 2224:  // $ED41
      /*$ED41*/ CYCLES(0xed41, 15);
                ram_poke(0x00a2, s_a);
      /*$ED43*/ tmp2_U8 = s_y;
                ram_poke(0x00ad, tmp2_U8);
      /*$ED45*/ s_y = (uint8_t)(tmp2_U8 + 0x01);
      /*$ED46*/ s_a = 0x30;
      /*$ED48*/ tmp2_U8 = ram_peek(0x009d);
                s_x = tmp2_U8;
      /*$ED4A*/ branchTarget = true; block_id = tmp2_U8 ? 2225 : 2226;
      break;
    case 2225:  // $ED4A
      /*$ED4A*/ CYCLES_EDGE(0xed4a, 1);
      /*$ED4F*/ CYCLES(0xed4f, 6);
                s_a = 0x00;
      /*$ED51*/ tmp2_U8 = s_x;
                s_status_c = (tmp2_U8 >= 0x80);
      /*$ED53*/ branchTarget = true; block_id = !(tmp2_U8 != 0x80) ? 2227 : 2228;
      break;
    case 2226:  // $ED4C
      /*$ED4C*/ CYCLES(0xed4c, 3);
      /*$EE57*/ CYCLES(0xee57, 5);
                ram_poke((0x00ff + s_y), s_a);
                block_id = 2330;
      break;
    case 2227:  // $ED53
      /*$ED53*/ CYCLES_EDGE(0xed53, 1);
                branchTarget = true; block_id = 2230;
      break;
    case 2228:  // $ED55
      /*$ED55*/ CYCLES(0xed55, 2);
                branchTarget = true; block_id = s_status_c ? 2229 : 2230;
      break;
    case 2229:  // $ED55
      /*$ED55*/ CYCLES_EDGE(0xed55, 1);
                branchTarget = true; block_id = 2232;
      break;
    case 2230:  // $ED57
      /*$ED57*/ CYCLES(0xed57, 10);
                s_a = 0x14;
      /*$ED59*/ s_y = 0xed;
      /*$ED5B*/ branchTarget = true; push16(0xed5d); block_id = 1964;
      break;
    case 2231:  // $ED5E
      /*$ED5E*/ CYCLES(0xed5e, 2);
                s_a = 0xf7;
                block_id = 2232;
      break;
    case 2232:  // $ED60
      /*$ED60*/ CYCLES(0xed60, 3);
                ram_poke(0x0099, s_a);
                block_id = 2233;
      break;
    case 2233:  // $ED62
      /*$ED62*/ CYCLES(0xed62, 10);
                s_a = 0x0f;
      /*$ED64*/ s_y = 0xed;
      /*$ED66*/ FUNC_FCOMP(0xed68);
      /*$ED69*/ CYCLES(0xed69, 2);
                branchTarget = true; block_id = !s_status_not_z ? 2234 : 2235;
      break;
    case 2234:  // $ED69
      /*$ED69*/ CYCLES_EDGE(0xed69, 1);
                branchTarget = true; block_id = 2248;
      break;
    case 2235:  // $ED6B
      /*$ED6B*/ CYCLES(0xed6b, 2);
                branchTarget = true; block_id = !s_status_n ? 2236 : 2237;
      break;
    case 2236:  // $ED6B
      /*$ED6B*/ CYCLES_EDGE(0xed6b, 1);
                branchTarget = true; block_id = 2244;
      break;
    case 2237:  // $ED6D
      /*$ED6D*/ CYCLES(0xed6d, 10);
                s_a = 0x0a;
      /*$ED6F*/ s_y = 0xed;
      /*$ED71*/ FUNC_FCOMP(0xed73);
      /*$ED74*/ CYCLES(0xed74, 2);
                branchTarget = true; block_id = !s_status_not_z ? 2238 : 2239;
      break;
    case 2238:  // $ED74
      /*$ED74*/ CYCLES_EDGE(0xed74, 1);
                branchTarget = true; block_id = 2241;
      break;
    case 2239:  // $ED76
      /*$ED76*/ CYCLES(0xed76, 2);
                branchTarget = true; block_id = !s_status_n ? 2240 : 2241;
      break;
    case 2240:  // $ED76
      /*$ED76*/ CYCLES_EDGE(0xed76, 1);
                branchTarget = true; block_id = 2247;
      break;
    case 2241:  // $ED78
      /*$ED78*/ CYCLES(0xed78, 6);
                branchTarget = true; push16(0xed7a); block_id = 2010;
      break;
    case 2242:  // $ED7B
      /*$ED7B*/ CYCLES(0xed7b, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x0099) - 0x01);
                ram_poke(0x0099, tmp2_U8);
      /*$ED7D*/ branchTarget = true; block_id = tmp2_U8 ? 2243 : 2244;
      break;
    case 2243:  // $ED7D
      /*$ED7D*/ CYCLES_EDGE(0xed7d, 1);
                branchTarget = true; block_id = 2237;
      break;
    case 2244:  // $ED7F
      /*$ED7F*/ CYCLES(0xed7f, 6);
                branchTarget = true; push16(0xed81); block_id = 2022;
      break;
    case 2245:  // $ED82
      /*$ED82*/ CYCLES(0xed82, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x0099) + 0x01);
                ram_poke(0x0099, tmp2_U8);
      /*$ED84*/ branchTarget = true; block_id = tmp2_U8 ? 2246 : 2247;
      break;
    case 2246:  // $ED84
      /*$ED84*/ CYCLES_EDGE(0xed84, 1);
                branchTarget = true; block_id = 2233;
      break;
    case 2247:  // $ED86
      /*$ED86*/ CYCLES(0xed86, 6);
                branchTarget = true; push16(0xed88); block_id = 1842;
      break;
    case 2248:  // $ED89
      /*$ED89*/ CYCLES(0xed89, 6);
                FUNC_QINT(0xed8b);
      /*$ED8C*/ CYCLES(0xed8c, 11);
                s_x = 0x01;
      /*$ED8E*/ s_a = ram_peek(0x0099);
      /*$ED90*/ s_status_c = 0x00;
      /*$ED91*/ block_id = s_status_d ? 2250 : 2249;
      break;
    case 2249:  // $ED91
      /*$ED91*/ tmp2_U8 = (uint8_t)((s_a + 0x000a) + s_status_c);
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 2251;
      break;
    case 2250:  // $ED91
      /*$ED91*/ tmp4_U16 = adc_dec16(s_a, 0x0a, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_n = ((uint8_t)(tmp4_U16 >> 8) & 0x80);
                block_id = 2251;
      break;
    case 2251:  // $ED93
      /*$ED93*/ branchTarget = true; block_id = s_status_n ? 2252 : 2253;
      break;
    case 2252:  // $ED93
      /*$ED93*/ CYCLES_EDGE(0xed93, 1);
                branchTarget = true; block_id = 2259;
      break;
    case 2253:  // $ED95
      /*$ED95*/ CYCLES(0xed95, 4);
                tmp2_U8 = s_a >= 0x0b;
                s_status_c = tmp2_U8;
      /*$ED97*/ branchTarget = true; block_id = tmp2_U8 ? 2254 : 2255;
      break;
    case 2254:  // $ED97
      /*$ED97*/ CYCLES_EDGE(0xed97, 1);
                branchTarget = true; block_id = 2260;
      break;
    case 2255:  // $ED99
      /*$ED99*/ CYCLES(0xed99, 6);
                block_id = s_status_d ? 2257 : 2256;
      break;
    case 2256:  // $ED99
      /*$ED99*/ s_a = (uint8_t)((s_a + 0x00ff) + s_status_c);
                block_id = 2258;
      break;
    case 2257:  // $ED99
      /*$ED99*/ s_a = ((uint8_t)adc_dec16(s_a, 0xff, s_status_c));
                block_id = 2258;
      break;
    case 2258:  // $ED9B
      /*$ED9B*/ s_x = s_a;
      /*$ED9C*/ s_a = 0x02;
                block_id = 2259;
      break;
    case 2259:  // $ED9E
      /*$ED9E*/ CYCLES(0xed9e, 2);
                s_status_c = 0x01;
                block_id = 2260;
      break;
    case 2260:  // $ED9F
      /*$ED9F*/ CYCLES(0xed9f, 12);
                block_id = s_status_d ? 2262 : 2261;
      break;
    case 2261:  // $ED9F
      /*$ED9F*/ s_a = (uint8_t)((s_a - 0x0002) - (uint8_t)(0x01 - s_status_c));
                block_id = 2263;
      break;
    case 2262:  // $ED9F
      /*$ED9F*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x02, s_status_c));
                block_id = 2263;
      break;
    case 2263:  // $EDA1
      /*$EDA1*/ ram_poke(0x009a, s_a);
      /*$EDA3*/ tmp2_U8 = s_x;
                ram_poke(0x0099, tmp2_U8);
      /*$EDA5*/ s_status_n = (tmp2_U8 & 0x80);
      /*$EDA6*/ branchTarget = true; block_id = !tmp2_U8 ? 2264 : 2265;
      break;
    case 2264:  // $EDA6
      /*$EDA6*/ CYCLES_EDGE(0xeda6, 1);
                branchTarget = true; block_id = 2267;
      break;
    case 2265:  // $EDA8
      /*$EDA8*/ CYCLES(0xeda8, 2);
                branchTarget = true; block_id = !s_status_n ? 2266 : 2267;
      break;
    case 2266:  // $EDA8
      /*$EDA8*/ CYCLES_EDGE(0xeda8, 1);
                branchTarget = true; block_id = 2271;
      break;
    case 2267:  // $EDAA
      /*$EDAA*/ CYCLES(0xedaa, 16);
      /*$EDAE*/ tmp2_U8 = (uint8_t)(ram_peek(0x00ad) + 0x01);
                s_y = tmp2_U8;
      /*$EDAF*/ ram_poke((0x00ff + tmp2_U8), 0x2e);
      /*$EDB3*/ branchTarget = true; block_id = !s_x ? 2268 : 2269;
      break;
    case 2268:  // $EDB3
      /*$EDB3*/ CYCLES_EDGE(0xedb3, 1);
                branchTarget = true; block_id = 2270;
      break;
    case 2269:  // $EDB5
      /*$EDB5*/ CYCLES(0xedb5, 9);
      /*$EDB7*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$EDB8*/ ram_poke((0x00ff + tmp2_U8), 0x30);
                block_id = 2270;
      break;
    case 2270:  // $EDBB
      /*$EDBB*/ CYCLES(0xedbb, 3);
                ram_poke(0x00ad, s_y);
                block_id = 2271;
      break;
    case 2271:  // $EDBD
      /*$EDBD*/ CYCLES(0xedbd, 4);
                s_y = 0x00;
      /*$EDBF*/ s_x = 0x80;
                block_id = 2272;
      break;
    case 2272:  // $EDC1
      /*$EDC1*/ CYCLES(0xedc1, 46);
                s_a = ram_peek(0x00a1);
      /*$EDC3*/ s_status_c = 0x00;
      /*$EDC4*/ block_id = s_status_d ? 2274 : 2273;
      break;
    case 2273:  // $EDC4
      /*$EDC4*/ tmp4_U16 = (s_a + ram_peek((0xee6c + s_y))) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 2275;
      break;
    case 2274:  // $EDC4
      /*$EDC4*/ tmp4_U16 = adc_dec16(s_a, ram_peek((0xee6c + s_y)), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 2275;
      break;
    case 2275:  // $EDC7
      /*$EDC7*/ ram_poke(0x00a1, s_a);
      /*$EDC9*/ s_a = ram_peek(0x00a0);
      /*$EDCB*/ block_id = s_status_d ? 2277 : 2276;
      break;
    case 2276:  // $EDCB
      /*$EDCB*/ tmp4_U16 = (s_a + ram_peek((0xee6b + s_y))) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 2278;
      break;
    case 2277:  // $EDCB
      /*$EDCB*/ tmp4_U16 = adc_dec16(s_a, ram_peek((0xee6b + s_y)), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 2278;
      break;
    case 2278:  // $EDCE
      /*$EDCE*/ ram_poke(0x00a0, s_a);
      /*$EDD0*/ s_a = ram_peek(0x009f);
      /*$EDD2*/ block_id = s_status_d ? 2280 : 2279;
      break;
    case 2279:  // $EDD2
      /*$EDD2*/ tmp4_U16 = (s_a + ram_peek((0xee6a + s_y))) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 2281;
      break;
    case 2280:  // $EDD2
      /*$EDD2*/ tmp4_U16 = adc_dec16(s_a, ram_peek((0xee6a + s_y)), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 2281;
      break;
    case 2281:  // $EDD5
      /*$EDD5*/ ram_poke(0x009f, s_a);
      /*$EDD7*/ s_a = ram_peek(0x009e);
      /*$EDD9*/ block_id = s_status_d ? 2283 : 2282;
      break;
    case 2282:  // $EDD9
      /*$EDD9*/ tmp4_U16 = (s_a + ram_peek((0xee69 + s_y))) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 2284;
      break;
    case 2283:  // $EDD9
      /*$EDD9*/ tmp4_U16 = adc_dec16(s_a, ram_peek((0xee69 + s_y)), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 2284;
      break;
    case 2284:  // $EDDC
      /*$EDDC*/ ram_poke(0x009e, s_a);
      /*$EDDE*/ tmp2_U8 = (uint8_t)(s_x + 0x01);
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$EDDF*/ branchTarget = true; block_id = s_status_c ? 2285 : 2286;
      break;
    case 2285:  // $EDDF
      /*$EDDF*/ CYCLES_EDGE(0xeddf, 1);
                branchTarget = true; block_id = 2290;
      break;
    case 2286:  // $EDE1
      /*$EDE1*/ CYCLES(0xede1, 2);
                branchTarget = true; block_id = !s_status_n ? 2287 : 2288;
      break;
    case 2287:  // $EDE1
      /*$EDE1*/ CYCLES_EDGE(0xede1, 1);
                branchTarget = true; block_id = 2272;
      break;
    case 2288:  // $EDE3
      /*$EDE3*/ CYCLES(0xede3, 2);
                branchTarget = true; block_id = s_status_n ? 2289 : 2290;
      break;
    case 2289:  // $EDE3
      /*$EDE3*/ CYCLES_EDGE(0xede3, 1);
                branchTarget = true; block_id = 2292;
      break;
    case 2290:  // $EDE5
      /*$EDE5*/ CYCLES(0xede5, 2);
                branchTarget = true; block_id = s_status_n ? 2291 : 2292;
      break;
    case 2291:  // $EDE5
      /*$EDE5*/ CYCLES_EDGE(0xede5, 1);
                branchTarget = true; block_id = 2272;
      break;
    case 2292:  // $EDE7
      /*$EDE7*/ CYCLES(0xede7, 4);
                s_a = s_x;
      /*$EDE8*/ branchTarget = true; block_id = !s_status_c ? 2293 : 2294;
      break;
    case 2293:  // $EDE8
      /*$EDE8*/ CYCLES_EDGE(0xede8, 1);
                branchTarget = true; block_id = 2297;
      break;
    case 2294:  // $EDEA
      /*$EDEA*/ CYCLES(0xedea, 4);
                s_a = (s_a ^ 0xff);
      /*$EDEC*/ block_id = s_status_d ? 2296 : 2295;
      break;
    case 2295:  // $EDEC
      /*$EDEC*/ tmp4_U16 = (s_a + 0x000a) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 2298;
      break;
    case 2296:  // $EDEC
      /*$EDEC*/ tmp4_U16 = adc_dec16(s_a, 0x0a, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 2298;
      break;
    case 2297:  // $EDEE
      /*$EDEE*/ CYCLES(0xedee, 34);
                block_id = s_status_d ? 2300 : 2299;
      break;
    case 2298:  // $EDEE
      /*$EDEC*/ block_id = 2297;
      break;
    case 2299:  // $EDEE
      /*$EDEE*/ tmp4_U16 = s_a;
                tmp6_U16 = (tmp4_U16 + 0x002f) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0x002f);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 2301;
      break;
    case 2300:  // $EDEE
      /*$EDEE*/ tmp6_U16 = adc_dec16(s_a, 0x2f, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 2301;
      break;
    case 2301:  // $EDF0
      /*$EDF4*/ ram_poke(0x0083, (uint8_t)(s_y + 0x04));
      /*$EDF8*/ tmp2_U8 = (uint8_t)(ram_peek(0x00ad) + 0x01);
                s_y = tmp2_U8;
      /*$EDF9*/ tmp1_U8 = s_a;
                s_x = tmp1_U8;
      /*$EDFC*/ ram_poke((0x00ff + tmp2_U8), (tmp1_U8 & 0x7f));
      /*$EDFF*/ tmp2_U8 = (uint8_t)(ram_peek(0x0099) - 0x01);
                ram_poke(0x0099, tmp2_U8);
      /*$EE01*/ branchTarget = true; block_id = tmp2_U8 ? 2302 : 2303;
      break;
    case 2302:  // $EE01
      /*$EE01*/ CYCLES_EDGE(0xee01, 1);
                branchTarget = true; block_id = 2304;
      break;
    case 2303:  // $EE03
      /*$EE03*/ CYCLES(0xee03, 9);
      /*$EE05*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$EE06*/ ram_poke((0x00ff + tmp2_U8), 0x2e);
                block_id = 2304;
      break;
    case 2304:  // $EE09
      /*$EE09*/ CYCLES(0xee09, 18);
                ram_poke(0x00ad, s_y);
      /*$EE0B*/ tmp2_U8 = ram_peek(0x0083);
                s_y = tmp2_U8;
      /*$EE12*/ s_x = ((s_x ^ 0xff) & 0x80);
      /*$EE15*/ branchTarget = true; block_id = (tmp2_U8 != 0x24) ? 2305 : 2306;
      break;
    case 2305:  // $EE15
      /*$EE15*/ CYCLES_EDGE(0xee15, 1);
                branchTarget = true; block_id = 2272;
      break;
    case 2306:  // $EE17
      /*$EE17*/ CYCLES(0xee17, 3);
                s_y = ram_peek(0x00ad);
                block_id = 2307;
      break;
    case 2307:  // $EE19
      /*$EE19*/ CYCLES(0xee19, 10);
                tmp2_U8 = s_y;
                tmp1_U8 = ram_peek((0x00ff + tmp2_U8));
                s_a = tmp1_U8;
      /*$EE1C*/ s_y = (uint8_t)(tmp2_U8 - 0x01);
      /*$EE1F*/ branchTarget = true; block_id = !(tmp1_U8 != 0x30) ? 2308 : 2309;
      break;
    case 2308:  // $EE1F
      /*$EE1F*/ CYCLES_EDGE(0xee1f, 1);
                branchTarget = true; block_id = 2307;
      break;
    case 2309:  // $EE21
      /*$EE21*/ CYCLES(0xee21, 4);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x2e);
      /*$EE23*/ branchTarget = true; block_id = !(tmp1_U8 != 0x2e) ? 2310 : 2311;
      break;
    case 2310:  // $EE23
      /*$EE23*/ CYCLES_EDGE(0xee23, 1);
                branchTarget = true; block_id = 2312;
      break;
    case 2311:  // $EE25
      /*$EE25*/ CYCLES(0xee25, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 2312;
      break;
    case 2312:  // $EE26
      /*$EE26*/ CYCLES(0xee26, 7);
                s_a = 0x2b;
      /*$EE28*/ tmp1_U8 = ram_peek(0x009a);
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$EE2A*/ branchTarget = true; block_id = !tmp1_U8 ? 2313 : 2314;
      break;
    case 2313:  // $EE2A
      /*$EE2A*/ CYCLES_EDGE(0xee2a, 1);
                branchTarget = true; block_id = 2330;
      break;
    case 2314:  // $EE2C
      /*$EE2C*/ CYCLES(0xee2c, 2);
                branchTarget = true; block_id = !s_status_n ? 2315 : 2316;
      break;
    case 2315:  // $EE2C
      /*$EE2C*/ CYCLES_EDGE(0xee2c, 1);
                branchTarget = true; block_id = 2320;
      break;
    case 2316:  // $EE2E
      /*$EE2E*/ CYCLES(0xee2e, 11);
                s_a = 0x00;
      /*$EE30*/ s_status_c = 0x01;
      /*$EE31*/ block_id = s_status_d ? 2318 : 2317;
      break;
    case 2317:  // $EE31
      /*$EE31*/ s_a = (uint8_t)((s_a - ram_peek(0x009a)) - (uint8_t)(0x01 - s_status_c));
                block_id = 2319;
      break;
    case 2318:  // $EE31
      /*$EE31*/ s_a = ((uint8_t)sbc_dec16(s_a, ram_peek(0x009a), s_status_c));
                block_id = 2319;
      break;
    case 2319:  // $EE33
      /*$EE33*/ s_x = s_a;
      /*$EE34*/ s_a = 0x2d;
                block_id = 2320;
      break;
    case 2320:  // $EE36
      /*$EE36*/ CYCLES(0xee36, 18);
                tmp1_U8 = s_y;
                ram_poke((0x0101 + tmp1_U8), s_a);
      /*$EE3B*/ ram_poke((0x0100 + tmp1_U8), 0x45);
      /*$EE3E*/ s_a = s_x;
      /*$EE3F*/ s_x = 0x2f;
      /*$EE41*/ s_status_c = 0x01;
                block_id = 2321;
      break;
    case 2321:  // $EE42
      /*$EE42*/ CYCLES(0xee42, 6);
                s_x = (uint8_t)(s_x + 0x01);
      /*$EE43*/ block_id = s_status_d ? 2323 : 2322;
      break;
    case 2322:  // $EE43
      /*$EE43*/ tmp6_U16 = (s_a - 0x000a) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp6_U16);
                block_id = 2324;
      break;
    case 2323:  // $EE43
      /*$EE43*/ tmp6_U16 = sbc_dec16(s_a, 0x0a, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 2324;
      break;
    case 2324:  // $EE45
      /*$EE45*/ branchTarget = true; block_id = s_status_c ? 2325 : 2326;
      break;
    case 2325:  // $EE45
      /*$EE45*/ CYCLES_EDGE(0xee45, 1);
                branchTarget = true; block_id = 2321;
      break;
    case 2326:  // $EE47
      /*$EE47*/ CYCLES(0xee47, 23);
                block_id = s_status_d ? 2328 : 2327;
      break;
    case 2327:  // $EE47
      /*$EE47*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 + 0x003a) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x003a);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 2329;
      break;
    case 2328:  // $EE47
      /*$EE47*/ tmp4_U16 = adc_dec16(s_a, 0x3a, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 2329;
      break;
    case 2329:  // $EE49
      /*$EE49*/ tmp1_U8 = s_y;
                ram_poke((0x0103 + tmp1_U8), s_a);
      /*$EE4D*/ ram_poke((0x0102 + tmp1_U8), s_x);
      /*$EE52*/ ram_poke((0x0104 + tmp1_U8), 0x00);
      /*$EE55*/ CYCLES_EDGE(0xee55, 1);
                branchTarget = true; block_id = 2331;
      break;
    case 2330:  // $EE5A
      /*$EE5A*/ CYCLES(0xee5a, 7);
      /*$EE5C*/ ram_poke((0x0100 + s_y), 0x00);
                block_id = 2331;
      break;
    case 2331:  // $EE5F
      /*$EE5F*/ CYCLES(0xee5f, 10);
                s_a = 0x00;
      /*$EE61*/ s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_y = 0x01;
      /*$EE63*/ branchTarget = true; block_id = find_block_id_func_t001(0xee63, pop16() + 1);;
      break;
    case 2332:  // $EED2
      /*$EED2*/ CYCLES_EDGE(0xeed2, 1);
                branchTarget = true; block_id = 2334;
      break;
    case 2333:  // $EED4
      /*$EED4*/ CYCLES(0xeed4, 8);
      /*$EED6*/ tmp1_U8 = ram_peek(0x00a2) ^ 0xff;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$EED8*/ ram_poke(0x00a2, tmp1_U8);
                block_id = 2334;
      break;
    case 2334:  // $EEDA
      /*$EEDA*/ CYCLES(0xeeda, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xeeda, pop16() + 1);;
      break;
    case 2335:  // $F13E
      /*$F13E*/ CYCLES(0xf13e, 26);
      /*$F140*/ ram_poke(0x0000, 0x4c);
      /*$F142*/ ram_poke(0x0003, 0x4c);
      /*$F144*/ ram_poke(0x0090, 0x4c);
      /*$F146*/ ram_poke(0x000a, 0x4c);
      /*$F14A*/ s_y = 0xe1;
      /*$F14C*/ ram_poke(0x000b, 0x99);
      /*$F14E*/ ram_poke(0x000c, 0xe1);
      /*$F150*/ s_x = 0x1c;
                block_id = 2336;
      break;
    case 2336:  // $F152
      /*$F152*/ CYCLES(0xf152, 15);
                tmp1_U8 = s_x;
      /*$F155*/ ram_poke((uint8_t)(0xb0 + tmp1_U8), ram_peek((0xf10a + tmp1_U8)));
      /*$F157*/ ram_poke(0x00f1, tmp1_U8);
      /*$F159*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_x = tmp1_U8;
      /*$F15A*/ branchTarget = true; block_id = tmp1_U8 ? 2337 : 2338;
      break;
    case 2337:  // $F15A
      /*$F15A*/ CYCLES_EDGE(0xf15a, 1);
                branchTarget = true; block_id = 2336;
      break;
    case 2338:  // $F15C
      /*$F15C*/ CYCLES(0xf15c, 25);
                tmp1_U8 = s_x;
                ram_poke(0x00f2, tmp1_U8);
      /*$F15F*/ ram_poke(0x00a4, tmp1_U8);
      /*$F161*/ ram_poke(0x0054, tmp1_U8);
      /*$F163*/ push8(tmp1_U8);
      /*$F166*/ ram_poke(0x008f, 0x03);
      /*$F168*/ branchTarget = true; push16(0xf16a); block_id = 1239;
      break;
    case 2339:  // $F16B
      /*$F16B*/ CYCLES(0xf16b, 27);
      /*$F16D*/ ram_poke(0x01fd, 0x01);
      /*$F170*/ ram_poke(0x01fc, 0x01);
      /*$F175*/ ram_poke(0x0052, 0x55);
      /*$F17B*/ ram_poke(0x0050, 0x00);
      /*$F17D*/ ram_poke(0x0051, 0x08);
      /*$F17F*/ s_y = 0x00;
                block_id = 2340;
      break;
    case 2340:  // $F181
      /*$F181*/ CYCLES(0xf181, 25);
                ram_poke(0x0051, (uint8_t)(ram_peek(0x0051) + 0x01));
      /*$F183*/ tmp1_U8 = s_y;
                tmp2_U8 = peek((ram_peek16al(0x0050) + tmp1_U8));
      /*$F185*/ tmp2_U8 = tmp2_U8 ^ 0xff;
                s_a = tmp2_U8;
      /*$F187*/ poke((ram_peek16al(0x0050) + tmp1_U8), tmp2_U8);
      /*$F189*/ tmp1_U8 = peek((ram_peek16al(0x0050) + tmp1_U8));
                s_status_c = (tmp2_U8 >= tmp1_U8);
      /*$F18B*/ branchTarget = true; block_id = (tmp2_U8 != tmp1_U8) ? 2341 : 2342;
      break;
    case 2341:  // $F18B
      /*$F18B*/ CYCLES_EDGE(0xf18b, 1);
                branchTarget = true; block_id = 2344;
      break;
    case 2342:  // $F18D
      /*$F18D*/ CYCLES(0xf18d, 15);
                tmp2_U8 = s_a ^ 0xff;
      /*$F18F*/ tmp1_U8 = s_y;
                poke((ram_peek16al(0x0050) + tmp1_U8), tmp2_U8);
      /*$F191*/ tmp1_U8 = peek((ram_peek16al(0x0050) + tmp1_U8));
                s_status_c = (tmp2_U8 >= tmp1_U8);
      /*$F193*/ branchTarget = true; block_id = !(tmp2_U8 != tmp1_U8) ? 2343 : 2344;
      break;
    case 2343:  // $F193
      /*$F193*/ CYCLES_EDGE(0xf193, 1);
                branchTarget = true; block_id = 2340;
      break;
    case 2344:  // $F195
      /*$F195*/ CYCLES(0xf195, 50);
                tmp1_U8 = ram_peek(0x0050);
      /*$F199*/ tmp2_U8 = ram_peek(0x0051) & 0xf0;
      /*$F19B*/ ram_poke(0x0073, tmp1_U8);
      /*$F19D*/ ram_poke(0x0074, tmp2_U8);
      /*$F19F*/ ram_poke(0x006f, tmp1_U8);
      /*$F1A1*/ ram_poke(0x0070, tmp2_U8);
      /*$F1A3*/ s_x = 0x00;
      /*$F1A7*/ ram_poke(0x0067, 0x00);
      /*$F1A9*/ ram_poke(0x0068, 0x08);
      /*$F1AD*/ ram_poke(0x00d6, 0x00);
      /*$F1B0*/ poke(ram_peek16(0x0067), 0x00);
      /*$F1B2*/ tmp2_U8 = (uint8_t)(ram_peek(0x0067) + 0x01);
                ram_poke(0x0067, tmp2_U8);
      /*$F1B4*/ branchTarget = true; block_id = tmp2_U8 ? 2345 : 2346;
      break;
    case 2345:  // $F1B4
      /*$F1B4*/ CYCLES_EDGE(0xf1b4, 1);
                branchTarget = true; block_id = 2347;
      break;
    case 2346:  // $F1B6
      /*$F1B6*/ CYCLES(0xf1b6, 5);
                ram_poke(0x0068, (uint8_t)(ram_peek(0x0068) + 0x01));
                block_id = 2347;
      break;
    case 2347:  // $F1B8
      /*$F1B8*/ CYCLES(0xf1b8, 12);
                s_a = ram_peek(0x0067);
      /*$F1BA*/ s_y = ram_peek(0x0068);
      /*$F1BC*/ branchTarget = true; push16(0xf1be); block_id = 867;
      break;
    case 2348:  // $F1BF
      /*$F1BF*/ CYCLES(0xf1bf, 6);
                branchTarget = true; push16(0xf1c1); block_id = 1069;
      break;
    case 2349:  // $F1C2
      /*$F1C2*/ CYCLES(0xf1c2, 25);
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
    case 2350:  // $F1D5
      /*$F1D5*/ CYCLES(0xf1d5, 6);
                branchTarget = true; push16(0xf1d7); block_id = 1259;
      break;
    case 2351:  // $F1D8
      /*$F1D8*/ CYCLES(0xf1d8, 6);
                branchTarget = true; push16(0xf1da); block_id = 1838;
      break;
    case 2352:  // $F1DB
      /*$F1DB*/ CYCLES(0xf1db, 5);
                branchTarget = true; block_id = find_block_id_func_t001(0xf1db, ram_peek16al(0x0050));
      break;
    case 2353:  // $F1EC
      /*$F1EC*/ CYCLES(0xf1ec, 6);
                branchTarget = true; push16(0xf1ee); block_id = 1833;
      break;
    case 2354:  // $F1EF
      /*$F1EF*/ CYCLES(0xf1ef, 4);
                tmp1_U8 = s_x >= 0x30;
                s_status_c = tmp1_U8;
      /*$F1F1*/ branchTarget = true; block_id = tmp1_U8 ? 2355 : 2356;
      break;
    case 2355:  // $F1F1
      /*$F1F1*/ CYCLES_EDGE(0xf1f1, 1);
                branchTarget = true; block_id = 2361;
      break;
    case 2356:  // $F1F3
      /*$F1F3*/ CYCLES(0xf1f3, 11);
                ram_poke(0x00f0, s_x);
      /*$F1F5*/ s_a = 0x2c;
      /*$F1F7*/ branchTarget = true; push16(0xf1f9); block_id = 1375;
      break;
    case 2357:  // $F1FA
      /*$F1FA*/ CYCLES(0xf1fa, 6);
                branchTarget = true; push16(0xf1fc); block_id = 1833;
      break;
    case 2358:  // $F1FD
      /*$F1FD*/ CYCLES(0xf1fd, 4);
                tmp1_U8 = s_x;
                s_status_not_z = (tmp1_U8 != 0x30);
                tmp2_U8 = tmp1_U8 >= 0x30;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - 0x30) & 0x80);
      /*$F1FF*/ branchTarget = true; block_id = tmp2_U8 ? 2359 : 2360;
      break;
    case 2359:  // $F1FF
      /*$F1FF*/ CYCLES_EDGE(0xf1ff, 1);
                branchTarget = true; block_id = 2361;
      break;
    case 2360:  // $F201
      /*$F201*/ CYCLES(0xf201, 12);
                tmp2_U8 = s_x;
                ram_poke(0x002c, tmp2_U8);
      /*$F203*/ ram_poke(0x002d, tmp2_U8);
      /*$F205*/ branchTarget = true; block_id = find_block_id_func_t001(0xf205, pop16() + 1);;
      break;
    case 2361:  // $F206
      /*$F206*/ CYCLES(0xf206, 3);
                branchTarget = true; block_id = 1526;
      break;
    case 2362:  // $F273
      /*$F273*/ CYCLES(0xf273, 4);
                s_a = 0xff;
      /*$F275*/ CYCLES_EDGE(0xf275, 1);
                branchTarget = true; block_id = 2364;
      break;
    case 2363:  // $F277
      /*$F277*/ CYCLES(0xf277, 2);
                s_a = 0x3f;
                block_id = 2364;
      break;
    case 2364:  // $F279
      /*$F279*/ CYCLES(0xf279, 14);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_x = 0x00;
      /*$F27B*/ ram_poke(0x0032, s_a);
      /*$F27D*/ ram_poke(0x00f3, 0x00);
      /*$F27F*/ branchTarget = true; block_id = find_block_id_func_t001(0xf27f, pop16() + 1);;
      break;
    case 2365:  // $F2E9
      /*$F2E9*/ CYCLES(0xf2e9, 63);
                ram_poke(0x00de, s_x);
      /*$F2EB*/ tmp2_U8 = ram_peek(0x00f8);
                s_x = tmp2_U8;
      /*$F2ED*/ ram_poke(0x00df, tmp2_U8);
      /*$F2F1*/ ram_poke(0x00da, ram_peek(0x0075));
      /*$F2F5*/ ram_poke(0x00db, ram_peek(0x0076));
      /*$F2F9*/ ram_poke(0x00dc, ram_peek(0x0079));
      /*$F2FD*/ ram_poke(0x00dd, ram_peek(0x007a));
      /*$F301*/ ram_poke(0x00b8, ram_peek(0x00f4));
      /*$F305*/ ram_poke(0x00b9, ram_peek(0x00f5));
      /*$F309*/ ram_poke(0x0075, ram_peek(0x00f6));
      /*$F30B*/ tmp2_U8 = ram_peek(0x00f7);
                s_a = tmp2_U8;
      /*$F30D*/ ram_poke(0x0076, tmp2_U8);
      /*$F30F*/ branchTarget = true; push16(0xf311); block_id = 5;
      break;
    case 2366:  // $F312
      /*$F312*/ CYCLES(0xf312, 6);
                branchTarget = true; push16(0xf314); block_id = 1150;
      break;
    case 2367:  // $F315
      /*$F315*/ CYCLES(0xf315, 3);
                branchTarget = true; block_id = 1088;
      break;
    case 2368:  // $F800
      /*$F800*/ CYCLES(0xf800, 11);
                tmp5_U8 = s_a;
                tmp1_U8 = tmp5_U8 >> 0x01;
                s_a = tmp1_U8;
      /*$F801*/ push8(((tmp5_U8 & 0x01) | ((tmp1_U8 == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | (tmp1_U8 & 0x80)));
      /*$F802*/ FUNC_GBASCALC(0xf804);
                branchTarget = true; block_id = 2369;
      break;
    case 2369:  // $F805
      /*$F805*/ CYCLES(0xf805, 8);
                tmp2_U8 = pop8();
                tmp1_U8 = tmp2_U8 & 0x01;
                s_status_c = tmp1_U8;
                s_status_i = ((tmp2_U8 & 0x04) != 0);
                s_status_d = ((tmp2_U8 & 0x08) != 0);
                s_status_b = 0x00;
                s_status_v = ((tmp2_U8 & 0x40) != 0);
      /*$F806*/ s_a = 0x0f;
      /*$F808*/ branchTarget = true; block_id = !tmp1_U8 ? 2370 : 2371;
      break;
    case 2370:  // $F808
      /*$F808*/ CYCLES_EDGE(0xf808, 1);
                branchTarget = true; block_id = 2374;
      break;
    case 2371:  // $F80A
      /*$F80A*/ CYCLES(0xf80a, 2);
                block_id = s_status_d ? 2373 : 2372;
      break;
    case 2372:  // $F80A
      /*$F80A*/ tmp4_U16 = s_a;
                tmp6_U16 = (tmp4_U16 + 0x00e0) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0x00e0);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 2375;
      break;
    case 2373:  // $F80A
      /*$F80A*/ tmp6_U16 = adc_dec16(s_a, 0xe0, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp2_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 2375;
      break;
    case 2374:  // $F80C
      /*$F80C*/ CYCLES(0xf80c, 3);
                ram_poke(0x002e, s_a);
                FUNC_PLOT1(0x0000);
                block_id = find_block_id_func_t001(0xf80c, pop16() + 1);;
      break;
    case 2375:  // $F80C
      /*$F80A*/ block_id = 2374;
      break;
    case 2376:  // $F80E
      /*$F80E*/ CYCLES(0xf80e, 28);
                tmp2_U8 = s_y;
                tmp1_U8 = peek((ram_peek16al(0x0026) + tmp2_U8));
      /*$F814*/ tmp5_U8 = peek((ram_peek16al(0x0026) + tmp2_U8));
                tmp1_U8 = ((tmp1_U8 ^ ram_peek(0x0030)) & ram_peek(0x002e)) ^ tmp5_U8;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$F816*/ poke((ram_peek16al(0x0026) + tmp2_U8), tmp1_U8);
      /*$F818*/ branchTarget = true; block_id = find_block_id_func_t001(0xf818, pop16() + 1);;
      break;
    case 2377:  // $F819
      /*$F819*/ CYCLES(0xf819, 6);
                FUNC_MON_PLOT(0xf81b);
                branchTarget = true; block_id = 2378;
      break;
    case 2378:  // $F81C
      /*$F81C*/ CYCLES(0xf81c, 5);
                tmp2_U8 = s_y;
                tmp1_U8 = ram_peek(0x002c);
                s_status_not_z = (tmp2_U8 != tmp1_U8);
                tmp5_U8 = tmp2_U8 >= tmp1_U8;
                s_status_c = tmp5_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
      /*$F81E*/ branchTarget = true; block_id = tmp5_U8 ? 2379 : 2380;
      break;
    case 2379:  // $F81E
      /*$F81E*/ CYCLES_EDGE(0xf81e, 1);
                branchTarget = true; block_id = 2388;
      break;
    case 2380:  // $F820
      /*$F820*/ CYCLES(0xf820, 8);
                s_y = (uint8_t)(s_y + 0x01);
      /*$F821*/ FUNC_PLOT1(0xf823);
                branchTarget = true; block_id = 2381;
      break;
    case 2381:  // $F824
      /*$F824*/ CYCLES(0xf824, 2);
                branchTarget = true; block_id = !s_status_c ? 2382 : 2383;
      break;
    case 2382:  // $F824
      /*$F824*/ CYCLES_EDGE(0xf824, 1);
                branchTarget = true; block_id = 2378;
      break;
    case 2383:  // $F826
      /*$F826*/ CYCLES(0xf826, 2);
                block_id = s_status_d ? 2385 : 2384;
      break;
    case 2384:  // $F826
      /*$F826*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 + 0x0001) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x0001);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 2386;
      break;
    case 2385:  // $F826
      /*$F826*/ tmp4_U16 = adc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 2386;
      break;
    case 2386:  // $F828
      /*$F828*/ CYCLES(0xf828, 9);
                push8(s_a);
      /*$F829*/ FUNC_MON_PLOT(0xf82b);
      /*$F82C*/ CYCLES(0xf82c, 9);
                tmp5_U8 = pop8();
                s_a = tmp5_U8;
      /*$F82D*/ tmp1_U8 = ram_peek(0x002d);
                s_status_not_z = (tmp5_U8 != tmp1_U8);
                tmp2_U8 = tmp5_U8 >= tmp1_U8;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp5_U8 - tmp1_U8) & 0x80);
      /*$F82F*/ branchTarget = true; block_id = !tmp2_U8 ? 2387 : 2388;
      break;
    case 2387:  // $F82F
      /*$F82F*/ CYCLES_EDGE(0xf82f, 1);
                branchTarget = true; block_id = 2383;
      break;
    case 2388:  // $F831
      /*$F831*/ CYCLES(0xf831, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xf831, pop16() + 1);;
      break;
    case 2389:  // $F836
      /*$F836*/ CYCLES(0xf836, 7);
      /*$F838*/ ram_poke(0x002d, 0x27);
      /*$F83A*/ s_y = 0x27;
                block_id = 2390;
      break;
    case 2390:  // $F83C
      /*$F83C*/ CYCLES(0xf83c, 11);
                s_a = 0x00;
      /*$F83E*/ ram_poke(0x0030, 0x00);
      /*$F840*/ FUNC_VLINE(0xf842);
      /*$F843*/ CYCLES(0xf843, 4);
                tmp2_U8 = (uint8_t)(s_y - 0x01);
                s_status_not_z = tmp2_U8;
                tmp1_U8 = tmp2_U8 & 0x80;
                s_status_n = tmp1_U8;
                s_y = tmp2_U8;
      /*$F844*/ branchTarget = true; block_id = !tmp1_U8 ? 2391 : 2392;
      break;
    case 2391:  // $F844
      /*$F844*/ CYCLES_EDGE(0xf844, 1);
                branchTarget = true; block_id = 2390;
      break;
    case 2392:  // $F846
      /*$F846*/ CYCLES(0xf846, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xf846, pop16() + 1);;
      break;
    case 2393:  // $F847
      /*$F847*/ CYCLES(0xf847, 20);
                tmp1_U8 = s_a;
                push8(tmp1_U8);
      /*$F848*/ tmp2_U8 = tmp1_U8 & 0x01;
                s_status_c = tmp2_U8;
      /*$F84D*/ ram_poke(0x0027, (((tmp1_U8 >> 0x01) & 0x03) | 0x04));
      /*$F84F*/ tmp1_U8 = pop8();
      /*$F850*/ s_a = (tmp1_U8 & 0x18);
      /*$F852*/ branchTarget = true; block_id = !tmp2_U8 ? 2394 : 2395;
      break;
    case 2394:  // $F852
      /*$F852*/ CYCLES_EDGE(0xf852, 1);
                branchTarget = true; block_id = 2398;
      break;
    case 2395:  // $F854
      /*$F854*/ CYCLES(0xf854, 2);
                block_id = s_status_d ? 2397 : 2396;
      break;
    case 2396:  // $F854
      /*$F854*/ tmp4_U16 = s_a;
                tmp6_U16 = (tmp4_U16 + 0x007f) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0x007f);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 2399;
      break;
    case 2397:  // $F854
      /*$F854*/ tmp6_U16 = adc_dec16(s_a, 0x7f, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 2399;
      break;
    case 2398:  // $F856
      /*$F856*/ CYCLES(0xf856, 19);
                tmp1_U8 = s_a;
                ram_poke(0x0026, tmp1_U8);
      /*$F859*/ tmp4_U16 = tmp1_U8 << 0x02;
                s_status_c = (uint8_t)((tmp4_U16 & 0x01ff) >> 8);
      /*$F85A*/ tmp1_U8 = ((uint8_t)tmp4_U16) | ram_peek(0x0026);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$F85C*/ ram_poke(0x0026, tmp1_U8);
      /*$F85E*/ branchTarget = true; block_id = find_block_id_func_t001(0xf85e, pop16() + 1);;
      break;
    case 2399:  // $F856
      /*$F854*/ block_id = 2398;
      break;
    case 2400:  // $F864
      /*$F864*/ CYCLES(0xf864, 25);
                tmp5_U8 = s_a & 0x0f;
      /*$F866*/ ram_poke(0x0030, tmp5_U8);
      /*$F86B*/ tmp3_U16 = tmp5_U8 << 0x04;
                s_status_c = (uint8_t)((tmp3_U16 & 0x01ff) >> 8);
      /*$F86C*/ tmp5_U8 = ((uint8_t)tmp3_U16) | ram_peek(0x0030);
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$F86E*/ ram_poke(0x0030, tmp5_U8);
      /*$F870*/ branchTarget = true; block_id = find_block_id_func_t001(0xf870, pop16() + 1);;
      break;
    case 2401:  // $F871
      /*$F871*/ CYCLES(0xf871, 11);
                tmp5_U8 = s_a;
                tmp1_U8 = tmp5_U8 >> 0x01;
                s_a = tmp1_U8;
      /*$F872*/ push8(((tmp5_U8 & 0x01) | ((tmp1_U8 == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | (tmp1_U8 & 0x80)));
      /*$F873*/ FUNC_GBASCALC(0xf875);
                branchTarget = true; block_id = 2402;
      break;
    case 2402:  // $F876
      /*$F876*/ CYCLES(0xf876, 11);
                tmp1_U8 = peek((ram_peek16al(0x0026) + s_y));
                s_a = tmp1_U8;
      /*$F878*/ tmp1_U8 = pop8();
                tmp2_U8 = tmp1_U8 & 0x01;
                s_status_c = tmp2_U8;
                s_status_i = ((tmp1_U8 & 0x04) != 0);
                s_status_d = ((tmp1_U8 & 0x08) != 0);
                s_status_b = 0x00;
                s_status_v = ((tmp1_U8 & 0x40) != 0);
      /*$F879*/ branchTarget = true; block_id = !tmp2_U8 ? 2403 : 2404;
      break;
    case 2403:  // $F879
      /*$F879*/ CYCLES_EDGE(0xf879, 1);
                branchTarget = true; block_id = 2405;
      break;
    case 2404:  // $F87B
      /*$F87B*/ CYCLES(0xf87b, 8);
                tmp2_U8 = s_a;
      /*$F87E*/ s_status_c = ((tmp2_U8 >> 0x03) & 0x01);
                s_a = (tmp2_U8 >> 0x04);
                block_id = 2405;
      break;
    case 2405:  // $F87F
      /*$F87F*/ CYCLES(0xf87f, 8);
                tmp2_U8 = s_a & 0x0f;
                s_status_not_z = tmp2_U8;
                s_status_n = 0x00;
                s_a = tmp2_U8;
      /*$F881*/ branchTarget = true; block_id = find_block_id_func_t001(0xf881, pop16() + 1);;
      break;
    case 2406:  // $F940
      /*$F940*/ CYCLES(0xf940, 8);
                s_a = s_y;
      /*$F941*/ branchTarget = true; push16(0xf943); block_id = 2616;
      break;
    case 2407:  // $F944
      /*$F944*/ CYCLES(0xf944, 5);
                s_a = s_x;
      /*$F945*/ branchTarget = true; block_id = 2616;
      break;
    case 2408:  // $FA47
      /*$FA47*/ CYCLES_EDGE(0xfa47, 1);
      /*$FA4C*/ CYCLES(0xfa4c, 10);
                tmp2_U8 = pop8();
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_i = ((tmp2_U8 & 0x04) != 0);
                s_status_d = ((tmp2_U8 & 0x08) != 0);
                s_status_b = 0x00;
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
      /*$FA4D*/ FUNC_SAV1(0xfa4f);
      /*$FA50*/ CYCLES(0xfa50, 19);
                tmp2_U8 = pop8();
      /*$FA51*/ ram_poke(0x003a, tmp2_U8);
      /*$FA53*/ tmp2_U8 = pop8();
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$FA54*/ ram_poke(0x003b, tmp2_U8);
      /*$FA56*/ branchTarget = true; block_id = find_block_id_func_t001(0xfa56, ram_peek16al(0x03f0));
      break;
    case 2409:  // $FA49
      /*$FA49*/ CYCLES(0xfa49, 5);
                branchTarget = true; block_id = find_block_id_func_t001(0xfa49, ram_peek16al(0x03fe));
      break;
    case 2410:  // $FA66
      /*$FA66*/ CYCLES(0xfa66, 6);
                FUNC_INIT(0xfa68);
                branchTarget = true; block_id = 2411;
      break;
    case 2411:  // $FA69
      /*$FA69*/ CYCLES(0xfa69, 6);
                FUNC_SETVID(0xfa6b);
                branchTarget = true; block_id = 2412;
      break;
    case 2412:  // $FA6C
      /*$FA6C*/ CYCLES(0xfa6c, 6);
                FUNC_SETKBD(0xfa6e);
                branchTarget = true; block_id = 2413;
      break;
    case 2413:  // $FA6F
      /*$FA6F*/ CYCLES(0xfa6f, 32);
                tmp2_U8 = io_peek(0xc058);
      /*$FA72*/ tmp2_U8 = io_peek(0xc05a);
      /*$FA75*/ tmp2_U8 = io_peek(0xc05d);
      /*$FA78*/ tmp2_U8 = io_peek(0xc05f);
      /*$FA7B*/ tmp2_U8 = io_peek(0xcfff);
      /*$FA7E*/ tmp2_U8 = io_peek(0xc010);
                s_status_v = ((tmp2_U8 >> 0x06) & 0x01);
      /*$FA81*/ s_status_d = 0x00;
      /*$FA82*/ branchTarget = true; push16(0xfa84); block_id = 2637;
      break;
    case 2414:  // $FA85
      /*$FA85*/ CYCLES(0xfa85, 12);
      /*$FA88*/ tmp2_U8 = ram_peek(0x03f3) ^ 0xa5;
      /*$FA8A*/ tmp1_U8 = ram_peek(0x03f4);
                s_status_c = (tmp2_U8 >= tmp1_U8);
      /*$FA8D*/ branchTarget = true; block_id = (tmp2_U8 != tmp1_U8) ? 2415 : 2416;
      break;
    case 2415:  // $FA8D
      /*$FA8D*/ CYCLES_EDGE(0xfa8d, 1);
      /*$FAA6*/ CYCLES(0xfaa6, 6);
                FUNC_APPLEII(0xfaa8);
                branchTarget = true; block_id = 2422;
      break;
    case 2416:  // $FA8F
      /*$FA8F*/ CYCLES(0xfa8f, 6);
                tmp1_U8 = ram_peek(0x03f2);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$FA92*/ branchTarget = true; block_id = tmp1_U8 ? 2417 : 2418;
      break;
    case 2417:  // $FA92
      /*$FA92*/ CYCLES_EDGE(0xfa92, 1);
                branchTarget = true; block_id = 2421;
      break;
    case 2418:  // $FA94
      /*$FA94*/ CYCLES(0xfa94, 8);
                s_a = 0xe0;
      /*$FA96*/ tmp1_U8 = ram_peek(0x03f3);
                tmp2_U8 = 0xe0 != tmp1_U8;
                s_status_not_z = tmp2_U8;
                s_status_c = (0xe0 >= tmp1_U8);
                s_status_n = ((uint8_t)(0xe0 - tmp1_U8) & 0x80);
      /*$FA99*/ branchTarget = true; block_id = tmp2_U8 ? 2419 : 2420;
      break;
    case 2419:  // $FA99
      /*$FA99*/ CYCLES_EDGE(0xfa99, 1);
                branchTarget = true; block_id = 2421;
      break;
    case 2420:  // $FA9B
      /*$FA9B*/ CYCLES(0xfa9b, 9);
      /*$FA9D*/ ram_poke(0x03f2, 0x03);
      /*$E000*/ CYCLES(0xe000, 3);
      /*$F128*/ CYCLES(0xf128, 31);
      /*$F12A*/ ram_poke(0x0076, 0xff);
      /*$F12E*/ s_sp = 0xfb;
      /*$F133*/ ram_poke(0x0001, 0x28);
      /*$F135*/ ram_poke(0x0002, 0xf1);
      /*$F137*/ ram_poke(0x0004, 0x28);
      /*$F139*/ ram_poke(0x0005, 0xf1);
      /*$F13B*/ FUNC_NORMAL(0xf13d);
                branchTarget = true; block_id = 2335;
      break;
    case 2421:  // $FAA3
      /*$FAA3*/ CYCLES(0xfaa3, 5);
                branchTarget = true; block_id = find_block_id_func_t001(0xfaa3, ram_peek16al(0x03f2));
      break;
    case 2422:  // $FAA9
      /*$FAA9*/ CYCLES(0xfaa9, 2);
                s_x = 0x05;
                block_id = 2423;
      break;
    case 2423:  // $FAAB
      /*$FAAB*/ CYCLES(0xfaab, 13);
                tmp2_U8 = s_x;
      /*$FAAE*/ ram_poke((0x03ef + tmp2_U8), ram_peek((0xfafc + tmp2_U8)));
      /*$FAB1*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_x = tmp2_U8;
      /*$FAB2*/ branchTarget = true; block_id = tmp2_U8 ? 2424 : 2425;
      break;
    case 2424:  // $FAB2
      /*$FAB2*/ CYCLES_EDGE(0xfab2, 1);
                branchTarget = true; block_id = 2423;
      break;
    case 2425:  // $FAB4
      /*$FAB4*/ CYCLES(0xfab4, 8);
      /*$FAB6*/ ram_poke(0x0000, s_x);
      /*$FAB8*/ ram_poke(0x0001, 0xc8);
                block_id = 2426;
      break;
    case 2426:  // $FABA
      /*$FABA*/ CYCLES(0xfaba, 14);
                s_y = 0x07;
      /*$FABC*/ ram_poke(0x0001, (uint8_t)(ram_peek(0x0001) - 0x01));
      /*$FABE*/ tmp2_U8 = ram_peek(0x0001);
                s_a = tmp2_U8;
      /*$FAC0*/ s_status_c = (tmp2_U8 >= 0xc0);
      /*$FAC2*/ branchTarget = true; block_id = !(tmp2_U8 != 0xc0) ? 2427 : 2428;
      break;
    case 2427:  // $FAC2
      /*$FAC2*/ CYCLES_EDGE(0xfac2, 1);
                branchTarget = true; block_id = 2420;
      break;
    case 2428:  // $FAC4
      /*$FAC4*/ CYCLES(0xfac4, 4);
                ram_poke(0x07f8, s_a);
                block_id = 2429;
      break;
    case 2429:  // $FAC7
      /*$FAC7*/ CYCLES(0xfac7, 11);
                tmp2_U8 = s_y;
                tmp1_U8 = peek((ram_peek16al(0x0000) + tmp2_U8));
                s_a = tmp1_U8;
      /*$FAC9*/ tmp2_U8 = ram_peek((0xfb01 + tmp2_U8));
                s_status_c = (tmp1_U8 >= tmp2_U8);
      /*$FACC*/ branchTarget = true; block_id = (tmp1_U8 != tmp2_U8) ? 2430 : 2431;
      break;
    case 2430:  // $FACC
      /*$FACC*/ CYCLES_EDGE(0xfacc, 1);
                branchTarget = true; block_id = 2426;
      break;
    case 2431:  // $FACE
      /*$FACE*/ CYCLES(0xface, 6);
      /*$FACF*/ tmp2_U8 = (uint8_t)(s_y - 0x02);
                s_status_not_z = tmp2_U8;
                tmp1_U8 = tmp2_U8 & 0x80;
                s_status_n = tmp1_U8;
                s_y = tmp2_U8;
      /*$FAD0*/ branchTarget = true; block_id = !tmp1_U8 ? 2432 : 2433;
      break;
    case 2432:  // $FAD0
      /*$FAD0*/ CYCLES_EDGE(0xfad0, 1);
                branchTarget = true; block_id = 2429;
      break;
    case 2433:  // $FAD2
      /*$FAD2*/ CYCLES(0xfad2, 5);
                branchTarget = true; block_id = find_block_id_func_t001(0xfad2, ram_peek16al(0x0000));
      break;
    case 2434:  // $FB2F
      /*$FB2F*/ CYCLES(0xfb2f, 21);
      /*$FB31*/ ram_poke(0x0048, 0x00);
      /*$FB33*/ tmp2_U8 = io_peek(0xc056);
      /*$FB36*/ tmp2_U8 = io_peek(0xc054);
      /*$FB39*/ tmp2_U8 = io_peek(0xc051);
      /*$FB3C*/ s_a = 0x00;
      /*$FB3E*/ CYCLES_EDGE(0xfb3e, 1);
                branchTarget = true; block_id = 2436;
      break;
    case 2435:  // $FB40
      /*$FB40*/ CYCLES(0xfb40, 14);
                tmp1_U8 = io_peek(0xc050);
      /*$FB43*/ tmp1_U8 = io_peek(0xc053);
      /*$FB46*/ FUNC_CLRTOP(0xfb48);
      /*$FB49*/ CYCLES(0xfb49, 2);
                s_a = 0x14;
                block_id = 2436;
      break;
    case 2436:  // $FB4B
      /*$FB4B*/ CYCLES(0xfb4b, 26);
                ram_poke(0x0022, s_a);
      /*$FB4F*/ ram_poke(0x0020, 0x00);
      /*$FB53*/ ram_poke(0x0021, 0x28);
      /*$FB57*/ ram_poke(0x0023, 0x18);
      /*$FB5B*/ ram_poke(0x0025, 0x17);
      /*$FB5D*/ branchTarget = true; block_id = 2499;
      break;
    case 2437:  // $FB60
      /*$FB60*/ CYCLES(0xfb60, 6);
                FUNC_HOME(0xfb62);
                branchTarget = true; block_id = 2438;
      break;
    case 2438:  // $FB63
      /*$FB63*/ CYCLES(0xfb63, 2);
                s_y = 0x08;
                block_id = 2439;
      break;
    case 2439:  // $FB65
      /*$FB65*/ CYCLES(0xfb65, 13);
                tmp1_U8 = s_y;
                tmp2_U8 = ram_peek((0xfb08 + tmp1_U8));
                s_a = tmp2_U8;
      /*$FB68*/ ram_poke((0x040e + tmp1_U8), tmp2_U8);
      /*$FB6B*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
      /*$FB6C*/ branchTarget = true; block_id = tmp1_U8 ? 2440 : 2441;
      break;
    case 2440:  // $FB6C
      /*$FB6C*/ CYCLES_EDGE(0xfb6c, 1);
                branchTarget = true; block_id = 2439;
      break;
    case 2441:  // $FB6E
      /*$FB6E*/ CYCLES(0xfb6e, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xfb6e, pop16() + 1);;
      break;
    case 2442:  // $FB78
      /*$FB78*/ CYCLES(0xfb78, 4);
      /*$FB7A*/ branchTarget = true; block_id = (s_a != 0x8d) ? 2443 : 2444;
      break;
    case 2443:  // $FB7A
      /*$FB7A*/ CYCLES_EDGE(0xfb7a, 1);
                branchTarget = true; block_id = 2454;
      break;
    case 2444:  // $FB7C
      /*$FB7C*/ CYCLES(0xfb7c, 6);
                tmp1_U8 = io_peek(0xc000);
                s_y = tmp1_U8;
      /*$FB7F*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 2445 : 2446;
      break;
    case 2445:  // $FB7F
      /*$FB7F*/ CYCLES_EDGE(0xfb7f, 1);
                branchTarget = true; block_id = 2454;
      break;
    case 2446:  // $FB81
      /*$FB81*/ CYCLES(0xfb81, 4);
      /*$FB83*/ branchTarget = true; block_id = (s_y != 0x93) ? 2447 : 2448;
      break;
    case 2447:  // $FB83
      /*$FB83*/ CYCLES_EDGE(0xfb83, 1);
                branchTarget = true; block_id = 2454;
      break;
    case 2448:  // $FB85
      /*$FB85*/ CYCLES(0xfb85, 4);
                tmp1_U8 = io_peek(0xc010);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
                block_id = 2449;
      break;
    case 2449:  // $FB88
      /*$FB88*/ CYCLES(0xfb88, 6);
                tmp1_U8 = io_peek(0xc000);
                s_y = tmp1_U8;
      /*$FB8B*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 2450 : 2451;
      break;
    case 2450:  // $FB8B
      /*$FB8B*/ CYCLES_EDGE(0xfb8b, 1);
                branchTarget = true; block_id = 2449;
      break;
    case 2451:  // $FB8D
      /*$FB8D*/ CYCLES(0xfb8d, 4);
      /*$FB8F*/ branchTarget = true; block_id = !(s_y != 0x83) ? 2452 : 2453;
      break;
    case 2452:  // $FB8F
      /*$FB8F*/ CYCLES_EDGE(0xfb8f, 1);
                branchTarget = true; block_id = 2454;
      break;
    case 2453:  // $FB91
      /*$FB91*/ CYCLES(0xfb91, 4);
                tmp1_U8 = io_peek(0xc010);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
                block_id = 2454;
      break;
    case 2454:  // $FB94
      /*$FB94*/ CYCLES(0xfb94, 3);
      /*$FBFD*/ CYCLES(0xfbfd, 4);
      /*$FBFF*/ branchTarget = true; block_id = (s_a >= 0xa0) ? 2484 : 2485;
      break;
    case 2455:  // $FB97
      /*$FB97*/ CYCLES(0xfb97, 5);
                s_status_c = 0x01;
      /*$FC2C*/ CYCLES(0xfc2c, 4);
                tmp1_U8 = s_a ^ 0xc0;
                s_a = tmp1_U8;
      /*$FC2E*/ branchTarget = true; block_id = !tmp1_U8 ? 2506 : 2507;
      break;
    case 2456:  // $FBA2
      /*$FBA2*/ CYCLES(0xfba2, 6);
                branchTarget = true; push16(0xfba4); block_id = 2574;
      break;
    case 2457:  // $FBA5
      /*$FBA5*/ CYCLES(0xfba5, 4);
      /*$FBA7*/ branchTarget = true; block_id = (s_a >= 0xce) ? 2458 : 2459;
      break;
    case 2458:  // $FBA7
      /*$FBA7*/ CYCLES_EDGE(0xfba7, 1);
                branchTarget = true; block_id = 2455;
      break;
    case 2459:  // $FBA9
      /*$FBA9*/ CYCLES(0xfba9, 4);
      /*$FBAB*/ branchTarget = true; block_id = !(s_a >= 0xc9) ? 2460 : 2461;
      break;
    case 2460:  // $FBAB
      /*$FBAB*/ CYCLES_EDGE(0xfbab, 1);
                branchTarget = true; block_id = 2455;
      break;
    case 2461:  // $FBAD
      /*$FBAD*/ CYCLES(0xfbad, 4);
                tmp1_U8 = s_a;
                tmp2_U8 = tmp1_U8 != 0xcc;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp1_U8 >= 0xcc);
      /*$FBAF*/ branchTarget = true; block_id = !tmp2_U8 ? 2462 : 2463;
      break;
    case 2462:  // $FBAF
      /*$FBAF*/ CYCLES_EDGE(0xfbaf, 1);
                branchTarget = true; block_id = 2455;
      break;
    case 2463:  // $FBB1
      /*$FBB1*/ CYCLES(0xfbb1, 2);
                branchTarget = true; block_id = s_status_not_z ? 2464 : 2465;
      break;
    case 2464:  // $FBB1
      /*$FBB1*/ CYCLES_EDGE(0xfbb1, 1);
      /*$FB9B*/ CYCLES(0xfb9b, 12);
                tmp2_U8 = s_a;
                s_y = tmp2_U8;
      /*$FB9C*/ s_a = ram_peek((0xfa48 + tmp2_U8));
      /*$FB9F*/ branchTarget = true; push16(0xfba1); block_id = 2455;
      break;
    case 2465:  // $FBB3
      /*$FBB3*/ CYCLES(0xfbb3, 28);
      /*$FBC0*/ FUNC_BASCALC(0x0000);
                block_id = find_block_id_func_t001(0xfbc0, pop16() + 1);;
      break;
    case 2466:  // $FBC1
      /*$FBC1*/ CYCLES(0xfbc1, 20);
                tmp2_U8 = s_a;
                push8(tmp2_U8);
      /*$FBC2*/ tmp1_U8 = tmp2_U8 & 0x01;
                s_status_c = tmp1_U8;
      /*$FBC7*/ ram_poke(0x0029, (((tmp2_U8 >> 0x01) & 0x03) | 0x04));
      /*$FBC9*/ tmp2_U8 = pop8();
      /*$FBCA*/ s_a = (tmp2_U8 & 0x18);
      /*$FBCC*/ branchTarget = true; block_id = !tmp1_U8 ? 2467 : 2468;
      break;
    case 2467:  // $FBCC
      /*$FBCC*/ CYCLES_EDGE(0xfbcc, 1);
                branchTarget = true; block_id = 2471;
      break;
    case 2468:  // $FBCE
      /*$FBCE*/ CYCLES(0xfbce, 2);
                block_id = s_status_d ? 2470 : 2469;
      break;
    case 2469:  // $FBCE
      /*$FBCE*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 + 0x007f) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x007f);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 2472;
      break;
    case 2470:  // $FBCE
      /*$FBCE*/ tmp4_U16 = adc_dec16(s_a, 0x7f, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 2472;
      break;
    case 2471:  // $FBD0
      /*$FBD0*/ CYCLES(0xfbd0, 19);
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
    case 2472:  // $FBD0
      /*$FBCE*/ block_id = 2471;
      break;
    case 2473:  // $FBDB
      /*$FBDB*/ CYCLES_EDGE(0xfbdb, 1);
                branchTarget = true; block_id = 2479;
      break;
    case 2474:  // $FBDD
      /*$FBDD*/ CYCLES(0xfbdd, 8);
                s_a = 0x40;
      /*$FBDF*/ FUNC_MON_WAIT(0xfbe1);
                branchTarget = true; block_id = 2475;
      break;
    case 2475:  // $FBE2
      /*$FBE2*/ CYCLES(0xfbe2, 2);
                s_y = 0xc0;
                block_id = 2476;
      break;
    case 2476:  // $FBE4
      /*$FBE4*/ CYCLES(0xfbe4, 8);
                s_a = 0x0c;
      /*$FBE6*/ FUNC_MON_WAIT(0xfbe8);
                branchTarget = true; block_id = 2477;
      break;
    case 2477:  // $FBE9
      /*$FBE9*/ CYCLES(0xfbe9, 8);
                tmp1_U8 = io_peek(0xc030);
                s_a = tmp1_U8;
      /*$FBEC*/ tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
      /*$FBED*/ branchTarget = true; block_id = tmp1_U8 ? 2478 : 2479;
      break;
    case 2478:  // $FBED
      /*$FBED*/ CYCLES_EDGE(0xfbed, 1);
                branchTarget = true; block_id = 2476;
      break;
    case 2479:  // $FBEF
      /*$FBEF*/ CYCLES(0xfbef, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xfbef, pop16() + 1);;
      break;
    case 2480:  // $FBF0
      /*$FBF0*/ CYCLES(0xfbf0, 9);
                tmp1_U8 = ram_peek(0x0024);
                s_y = tmp1_U8;
      /*$FBF2*/ poke((ram_peek16al(0x0028) + tmp1_U8), s_a);
                block_id = 2481;
      break;
    case 2481:  // $FBF4
      /*$FBF4*/ CYCLES(0xfbf4, 13);
                ram_poke(0x0024, (uint8_t)(ram_peek(0x0024) + 0x01));
      /*$FBF6*/ tmp1_U8 = ram_peek(0x0024);
                s_a = tmp1_U8;
      /*$FBF8*/ tmp2_U8 = ram_peek(0x0021);
                s_status_not_z = (tmp1_U8 != tmp2_U8);
                tmp5_U8 = tmp1_U8 >= tmp2_U8;
                s_status_c = tmp5_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
      /*$FBFA*/ branchTarget = true; block_id = tmp5_U8 ? 2482 : 2483;
      break;
    case 2482:  // $FBFA
      /*$FBFA*/ CYCLES_EDGE(0xfbfa, 1);
                branchTarget = true; block_id = 2539;
      break;
    case 2483:  // $FBFC
      /*$FBFC*/ CYCLES(0xfbfc, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xfbfc, pop16() + 1);;
      break;
    case 2484:  // $FBFF
      /*$FBFF*/ CYCLES_EDGE(0xfbff, 1);
                branchTarget = true; block_id = 2480;
      break;
    case 2485:  // $FC01
      /*$FC01*/ CYCLES(0xfc01, 4);
                tmp5_U8 = s_a;
                s_y = tmp5_U8;
      /*$FC02*/ branchTarget = true; block_id = !(tmp5_U8 & 0x80) ? 2486 : 2487;
      break;
    case 2486:  // $FC02
      /*$FC02*/ CYCLES_EDGE(0xfc02, 1);
                branchTarget = true; block_id = 2480;
      break;
    case 2487:  // $FC04
      /*$FC04*/ CYCLES(0xfc04, 4);
      /*$FC06*/ branchTarget = true; block_id = !(s_a != 0x8d) ? 2488 : 2489;
      break;
    case 2488:  // $FC06
      /*$FC06*/ CYCLES_EDGE(0xfc06, 1);
                branchTarget = true; block_id = 2539;
      break;
    case 2489:  // $FC08
      /*$FC08*/ CYCLES(0xfc08, 4);
      /*$FC0A*/ branchTarget = true; block_id = !(s_a != 0x8a) ? 2490 : 2491;
      break;
    case 2490:  // $FC0A
      /*$FC0A*/ CYCLES_EDGE(0xfc0a, 1);
                branchTarget = true; block_id = 2540;
      break;
    case 2491:  // $FC0C
      /*$FC0C*/ CYCLES(0xfc0c, 4);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0x88);
      /*$FC0E*/ branchTarget = true; block_id = (tmp5_U8 != 0x88) ? 2492 : 2493;
      break;
    case 2492:  // $FC0E
      /*$FC0E*/ CYCLES_EDGE(0xfc0e, 1);
      /*$FBD9*/ CYCLES(0xfbd9, 4);
                tmp2_U8 = s_a;
                tmp5_U8 = tmp2_U8 != 0x87;
                s_status_not_z = tmp5_U8;
                s_status_c = (tmp2_U8 >= 0x87);
                s_status_n = ((uint8_t)(tmp2_U8 - 0x87) & 0x80);
      /*$FBDB*/ branchTarget = true; block_id = tmp5_U8 ? 2473 : 2474;
      break;
    case 2493:  // $FC10
      /*$FC10*/ CYCLES(0xfc10, 7);
                tmp5_U8 = (uint8_t)(ram_peek(0x0024) - 0x01);
                s_status_not_z = tmp5_U8;
                tmp2_U8 = tmp5_U8 & 0x80;
                s_status_n = tmp2_U8;
                ram_poke(0x0024, tmp5_U8);
      /*$FC12*/ branchTarget = true; block_id = !tmp2_U8 ? 2494 : 2495;
      break;
    case 2494:  // $FC12
      /*$FC12*/ CYCLES_EDGE(0xfc12, 1);
                branchTarget = true; block_id = 2483;
      break;
    case 2495:  // $FC14
      /*$FC14*/ CYCLES(0xfc14, 11);
      /*$FC16*/ ram_poke(0x0024, ram_peek(0x0021));
      /*$FC18*/ ram_poke(0x0024, (uint8_t)(ram_peek(0x0024) - 0x01));
                block_id = 2496;
      break;
    case 2496:  // $FC1A
      /*$FC1A*/ CYCLES(0xfc1a, 8);
                tmp5_U8 = ram_peek(0x0022);
                s_a = tmp5_U8;
      /*$FC1C*/ tmp2_U8 = ram_peek(0x0025);
                s_status_not_z = (tmp5_U8 != tmp2_U8);
                tmp1_U8 = tmp5_U8 >= tmp2_U8;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp5_U8 - tmp2_U8) & 0x80);
      /*$FC1E*/ branchTarget = true; block_id = tmp1_U8 ? 2497 : 2498;
      break;
    case 2497:  // $FC1E
      /*$FC1E*/ CYCLES_EDGE(0xfc1e, 1);
                branchTarget = true; block_id = 2505;
      break;
    case 2498:  // $FC20
      /*$FC20*/ CYCLES(0xfc20, 5);
                ram_poke(0x0025, (uint8_t)(ram_peek(0x0025) - 0x01));
                block_id = 2499;
      break;
    case 2499:  // $FC22
      /*$FC22*/ CYCLES(0xfc22, 3);
                s_a = ram_peek(0x0025);
                FUNC_VTABZ(0x0000);
                block_id = find_block_id_func_t001(0xfc22, pop16() + 1);;
      break;
    case 2500:  // $FC24
      /*$FC24*/ CYCLES(0xfc24, 6);
                FUNC_BASCALC(0xfc26);
                branchTarget = true; block_id = 2501;
      break;
    case 2501:  // $FC27
      /*$FC27*/ CYCLES(0xfc27, 6);
                block_id = s_status_d ? 2503 : 2502;
      break;
    case 2502:  // $FC27
      /*$FC27*/ tmp4_U16 = s_a;
                tmp6_U16 = ram_peek(0x0020);
                tmp3_U16 = (tmp4_U16 + tmp6_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 2504;
      break;
    case 2503:  // $FC27
      /*$FC27*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x0020), s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
                block_id = 2504;
      break;
    case 2504:  // $FC29
      /*$FC29*/ ram_poke(0x0028, s_a);
                block_id = 2505;
      break;
    case 2505:  // $FC2B
      /*$FC2B*/ CYCLES(0xfc2b, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xfc2b, pop16() + 1);;
      break;
    case 2506:  // $FC2E
      /*$FC2E*/ CYCLES_EDGE(0xfc2e, 1);
                FUNC_HOME(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xfc2e, pop16() + 1);;
      break;
    case 2507:  // $FC30
      /*$FC30*/ CYCLES(0xfc30, 4);
                block_id = s_status_d ? 2509 : 2508;
      break;
    case 2508:  // $FC30
      /*$FC30*/ tmp3_U16 = s_a;
                tmp6_U16 = (tmp3_U16 + 0x00fd) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)0x00fd);
                tmp1_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
                block_id = 2510;
      break;
    case 2509:  // $FC30
      /*$FC30*/ tmp6_U16 = adc_dec16(s_a, 0xfd, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 2510;
      break;
    case 2510:  // $FC32
      /*$FC32*/ branchTarget = true; block_id = !s_status_c ? 2511 : 2512;
      break;
    case 2511:  // $FC32
      /*$FC32*/ CYCLES_EDGE(0xfc32, 1);
                branchTarget = true; block_id = 2481;
      break;
    case 2512:  // $FC34
      /*$FC34*/ CYCLES(0xfc34, 2);
                branchTarget = true; block_id = !s_status_not_z ? 2513 : 2514;
      break;
    case 2513:  // $FC34
      /*$FC34*/ CYCLES_EDGE(0xfc34, 1);
                branchTarget = true; block_id = 2493;
      break;
    case 2514:  // $FC36
      /*$FC36*/ CYCLES(0xfc36, 4);
                block_id = s_status_d ? 2516 : 2515;
      break;
    case 2515:  // $FC36
      /*$FC36*/ tmp6_U16 = s_a;
                tmp3_U16 = (tmp6_U16 + 0x00fd) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)0x00fd);
                tmp1_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
                block_id = 2517;
      break;
    case 2516:  // $FC36
      /*$FC36*/ tmp3_U16 = adc_dec16(s_a, 0xfd, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 2517;
      break;
    case 2517:  // $FC38
      /*$FC38*/ branchTarget = true; block_id = !s_status_c ? 2518 : 2519;
      break;
    case 2518:  // $FC38
      /*$FC38*/ CYCLES_EDGE(0xfc38, 1);
                branchTarget = true; block_id = 2540;
      break;
    case 2519:  // $FC3A
      /*$FC3A*/ CYCLES(0xfc3a, 2);
                branchTarget = true; block_id = !s_status_not_z ? 2520 : 2521;
      break;
    case 2520:  // $FC3A
      /*$FC3A*/ CYCLES_EDGE(0xfc3a, 1);
                branchTarget = true; block_id = 2496;
      break;
    case 2521:  // $FC3C
      /*$FC3C*/ CYCLES(0xfc3c, 4);
                block_id = s_status_d ? 2523 : 2522;
      break;
    case 2522:  // $FC3C
      /*$FC3C*/ tmp3_U16 = s_a;
                tmp6_U16 = (tmp3_U16 + 0x00fd) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)0x00fd);
                tmp1_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 2524;
      break;
    case 2523:  // $FC3C
      /*$FC3C*/ tmp6_U16 = adc_dec16(s_a, 0xfd, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp1_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
                block_id = 2524;
      break;
    case 2524:  // $FC3E
      /*$FC3E*/ branchTarget = true; block_id = !s_status_c ? 2525 : 2526;
      break;
    case 2525:  // $FC3E
      /*$FC3E*/ CYCLES_EDGE(0xfc3e, 1);
                FUNC_CLREOL(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xfc3e, pop16() + 1);;
      break;
    case 2526:  // $FC40
      /*$FC40*/ CYCLES(0xfc40, 2);
                branchTarget = true; block_id = s_status_not_z ? 2527 : 2528;
      break;
    case 2527:  // $FC40
      /*$FC40*/ CYCLES_EDGE(0xfc40, 1);
                branchTarget = true; block_id = 2505;
      break;
    case 2528:  // $FC42
      /*$FC42*/ CYCLES(0xfc42, 6);
                s_y = ram_peek(0x0024);
      /*$FC44*/ s_a = ram_peek(0x0025);
                block_id = 2529;
      break;
    case 2529:  // $FC46
      /*$FC46*/ CYCLES(0xfc46, 9);
                push8(s_a);
      /*$FC47*/ FUNC_VTABZ(0xfc49);
                branchTarget = true; block_id = 2530;
      break;
    case 2530:  // $FC4A
      /*$FC4A*/ CYCLES(0xfc4a, 6);
                FUNC_CLREOLZ(0xfc4c);
                branchTarget = true; block_id = 2531;
      break;
    case 2531:  // $FC4D
      /*$FC4D*/ CYCLES(0xfc4d, 13);
                s_y = 0x00;
      /*$FC4F*/ tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$FC50*/ block_id = s_status_d ? 2533 : 2532;
      break;
    case 2532:  // $FC50
      /*$FC50*/ s_a = (uint8_t)(s_a + s_status_c);
                block_id = 2534;
      break;
    case 2533:  // $FC50
      /*$FC50*/ s_a = ((uint8_t)adc_dec16(s_a, 0x00, s_status_c));
                block_id = 2534;
      break;
    case 2534:  // $FC52
      /*$FC52*/ tmp1_U8 = s_a >= ram_peek(0x0023);
                s_status_c = tmp1_U8;
      /*$FC54*/ branchTarget = true; block_id = !tmp1_U8 ? 2535 : 2536;
      break;
    case 2535:  // $FC54
      /*$FC54*/ CYCLES_EDGE(0xfc54, 1);
                branchTarget = true; block_id = 2529;
      break;
    case 2536:  // $FC56
      /*$FC56*/ CYCLES(0xfc56, 2);
                branchTarget = true; block_id = s_status_c ? 2537 : 2538;
      break;
    case 2537:  // $FC56
      /*$FC56*/ CYCLES_EDGE(0xfc56, 1);
                branchTarget = true; block_id = 2499;
      break;
    case 2538:  // $FC58
      /*$FC58*/ CYCLES(0xfc58, 13);
                tmp1_U8 = ram_peek(0x0022);
                s_a = tmp1_U8;
      /*$FC5A*/ ram_poke(0x0025, tmp1_U8);
      /*$FC5C*/ s_y = 0x00;
      /*$FC5E*/ ram_poke(0x0024, 0x00);
      /*$FC60*/ CYCLES_EDGE(0xfc60, 1);
                branchTarget = true; block_id = 2529;
      break;
    case 2539:  // $FC62
      /*$FC62*/ CYCLES(0xfc62, 5);
      /*$FC64*/ ram_poke(0x0024, 0x00);
                block_id = 2540;
      break;
    case 2540:  // $FC66
      /*$FC66*/ CYCLES(0xfc66, 5);
                ram_poke(0x0025, (uint8_t)(ram_peek(0x0025) + 0x01));
                block_id = 2541;
      break;
    case 2541:  // $FC68
      /*$FC68*/ CYCLES(0xfc68, 8);
                tmp2_U8 = ram_peek(0x0025);
                s_a = tmp2_U8;
      /*$FC6C*/ branchTarget = true; block_id = !(tmp2_U8 >= ram_peek(0x0023)) ? 2542 : 2543;
      break;
    case 2542:  // $FC6C
      /*$FC6C*/ CYCLES_EDGE(0xfc6c, 1);
                FUNC_VTABZ(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xfc6c, pop16() + 1);;
      break;
    case 2543:  // $FC6E
      /*$FC6E*/ CYCLES(0xfc6e, 17);
                ram_poke(0x0025, (uint8_t)(ram_peek(0x0025) - 0x01));
      /*$FC70*/ tmp1_U8 = ram_peek(0x0022);
                s_a = tmp1_U8;
      /*$FC72*/ push8(tmp1_U8);
      /*$FC73*/ FUNC_VTABZ(0xfc75);
                branchTarget = true; block_id = 2544;
      break;
    case 2544:  // $FC76
      /*$FC76*/ CYCLES(0xfc76, 28);
      /*$FC78*/ ram_poke(0x002a, ram_peek(0x0028));
      /*$FC7C*/ ram_poke(0x002b, ram_peek(0x0029));
      /*$FC80*/ s_y = (uint8_t)(ram_peek(0x0021) - 0x01);
      /*$FC81*/ tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$FC82*/ block_id = s_status_d ? 2546 : 2545;
      break;
    case 2545:  // $FC82
      /*$FC82*/ tmp6_U16 = s_a;
                tmp3_U16 = (tmp6_U16 + 0x0001) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)0x0001);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 2547;
      break;
    case 2546:  // $FC82
      /*$FC82*/ tmp3_U16 = adc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
                block_id = 2547;
      break;
    case 2547:  // $FC84
      /*$FC86*/ branchTarget = true; block_id = (s_a >= ram_peek(0x0023)) ? 2548 : 2549;
      break;
    case 2548:  // $FC86
      /*$FC86*/ CYCLES_EDGE(0xfc86, 1);
                branchTarget = true; block_id = 2554;
      break;
    case 2549:  // $FC88
      /*$FC88*/ CYCLES(0xfc88, 9);
                push8(s_a);
      /*$FC89*/ FUNC_VTABZ(0xfc8b);
                branchTarget = true; block_id = 2550;
      break;
    case 2550:  // $FC8C
      /*$FC8C*/ CYCLES(0xfc8c, 15);
                tmp1_U8 = s_y;
                tmp2_U8 = peek((ram_peek16al(0x0028) + tmp1_U8));
      /*$FC8E*/ poke((ram_peek16al(0x002a) + tmp1_U8), tmp2_U8);
      /*$FC90*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                tmp2_U8 = tmp1_U8 & 0x80;
                s_status_n = tmp2_U8;
                s_y = tmp1_U8;
      /*$FC91*/ branchTarget = true; block_id = !tmp2_U8 ? 2551 : 2552;
      break;
    case 2551:  // $FC91
      /*$FC91*/ CYCLES_EDGE(0xfc91, 1);
                branchTarget = true; block_id = 2550;
      break;
    case 2552:  // $FC93
      /*$FC93*/ CYCLES(0xfc93, 2);
                branchTarget = true; block_id = s_status_n ? 2553 : 2554;
      break;
    case 2553:  // $FC93
      /*$FC93*/ CYCLES_EDGE(0xfc93, 1);
                branchTarget = true; block_id = 2544;
      break;
    case 2554:  // $FC95
      /*$FC95*/ CYCLES(0xfc95, 8);
                s_y = 0x00;
      /*$FC97*/ FUNC_CLREOLZ(0xfc99);
      /*$FC9A*/ CYCLES(0xfc9a, 2);
                branchTarget = true; block_id = s_status_c ? 2555 : 2643;
      break;
    case 2555:  // $FC9A
      /*$FC9A*/ CYCLES_EDGE(0xfc9a, 1);
                branchTarget = true; block_id = 2499;
      break;
    case 2556:  // $FC9C
      /*$FC9C*/ CYCLES(0xfc9c, 3);
                s_y = ram_peek(0x0024);
                FUNC_CLREOLZ(0x0000);
                block_id = find_block_id_func_t001(0xfc9c, pop16() + 1);;
      break;
    case 2557:  // $FC9E
      /*$FC9E*/ CYCLES(0xfc9e, 2);
                s_a = 0xa0;
                block_id = 2558;
      break;
    case 2558:  // $FCA0
      /*$FCA0*/ CYCLES(0xfca0, 13);
                tmp2_U8 = s_y;
                poke((ram_peek16al(0x0028) + tmp2_U8), s_a);
      /*$FCA2*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_y = tmp2_U8;
      /*$FCA3*/ tmp1_U8 = ram_peek(0x0021);
                s_status_not_z = (tmp2_U8 != tmp1_U8);
                tmp5_U8 = tmp2_U8 >= tmp1_U8;
                s_status_c = tmp5_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
      /*$FCA5*/ branchTarget = true; block_id = !tmp5_U8 ? 2559 : 2560;
      break;
    case 2559:  // $FCA5
      /*$FCA5*/ CYCLES_EDGE(0xfca5, 1);
                branchTarget = true; block_id = 2558;
      break;
    case 2560:  // $FCA7
      /*$FCA7*/ CYCLES(0xfca7, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xfca7, pop16() + 1);;
      break;
    case 2561:  // $FCA8
      /*$FCA8*/ CYCLES(0xfca8, 2);
                s_status_c = 0x01;
                block_id = 2562;
      break;
    case 2562:  // $FCA9
      /*$FCA9*/ CYCLES(0xfca9, 3);
                push8(s_a);
                block_id = 2563;
      break;
    case 2563:  // $FCAA
      /*$FCAA*/ CYCLES(0xfcaa, 4);
                block_id = s_status_d ? 2565 : 2564;
      break;
    case 2564:  // $FCAA
      /*$FCAA*/ tmp3_U16 = (s_a - 0x0001) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                tmp5_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp5_U8;
                s_a = tmp5_U8;
                block_id = 2566;
      break;
    case 2565:  // $FCAA
      /*$FCAA*/ tmp3_U16 = sbc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp5_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_not_z = (~tmp5_U8 & 2);
                block_id = 2566;
      break;
    case 2566:  // $FCAC
      /*$FCAC*/ branchTarget = true; block_id = s_status_not_z ? 2567 : 2568;
      break;
    case 2567:  // $FCAC
      /*$FCAC*/ CYCLES_EDGE(0xfcac, 1);
                branchTarget = true; block_id = 2563;
      break;
    case 2568:  // $FCAE
      /*$FCAE*/ CYCLES(0xfcae, 8);
                tmp5_U8 = pop8();
                s_a = tmp5_U8;
      /*$FCAF*/ block_id = s_status_d ? 2570 : 2569;
      break;
    case 2569:  // $FCAF
      /*$FCAF*/ tmp3_U16 = s_a;
                tmp6_U16 = (tmp3_U16 - 0x0001) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp3_U16, (uint8_t)0xfffe);
                tmp5_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
                block_id = 2571;
      break;
    case 2570:  // $FCAF
      /*$FCAF*/ tmp6_U16 = sbc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp5_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp5_U8 & 0x01);
                s_status_not_z = (~tmp5_U8 & 2);
                s_status_v = ((tmp5_U8 & 0x40) != 0);
                s_status_n = (tmp5_U8 & 0x80);
                block_id = 2571;
      break;
    case 2571:  // $FCB1
      /*$FCB1*/ branchTarget = true; block_id = s_status_not_z ? 2572 : 2573;
      break;
    case 2572:  // $FCB1
      /*$FCB1*/ CYCLES_EDGE(0xfcb1, 1);
                branchTarget = true; block_id = 2562;
      break;
    case 2573:  // $FCB3
      /*$FCB3*/ CYCLES(0xfcb3, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xfcb3, pop16() + 1);;
      break;
    case 2574:  // $FD0C
      /*$FD0C*/ CYCLES(0xfd0c, 30);
                tmp2_U8 = ram_peek(0x0024);
                s_y = tmp2_U8;
      /*$FD0E*/ tmp5_U8 = peek((ram_peek16al(0x0028) + tmp2_U8));
      /*$FD10*/ push8(tmp5_U8);
      /*$FD15*/ poke((ram_peek16al(0x0028) + tmp2_U8), ((tmp5_U8 & 0x3f) | 0x40));
      /*$FD17*/ tmp2_U8 = pop8();
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$FD18*/ branchTarget = true; block_id = find_block_id_func_t001(0xfd18, ram_peek16al(0x0038));
      break;
    case 2575:  // $FD1B
      /*$FD1B*/ CYCLES(0xfd1b, 7);
                tmp5_U8 = (uint8_t)(ram_peek(0x004e) + 0x01);
                ram_poke(0x004e, tmp5_U8);
      /*$FD1D*/ branchTarget = true; block_id = tmp5_U8 ? 2576 : 2577;
      break;
    case 2576:  // $FD1D
      /*$FD1D*/ CYCLES_EDGE(0xfd1d, 1);
                branchTarget = true; block_id = 2578;
      break;
    case 2577:  // $FD1F
      /*$FD1F*/ CYCLES(0xfd1f, 5);
                ram_poke(0x004f, (uint8_t)(ram_peek(0x004f) + 0x01));
                block_id = 2578;
      break;
    case 2578:  // $FD21
      /*$FD21*/ CYCLES(0xfd21, 6);
                tmp5_U8 = io_peek(0xc000);
      /*$FD24*/ branchTarget = true; block_id = !(tmp5_U8 & 0x80) ? 2579 : 2580;
      break;
    case 2579:  // $FD24
      /*$FD24*/ CYCLES_EDGE(0xfd24, 1);
                branchTarget = true; block_id = 2575;
      break;
    case 2580:  // $FD26
      /*$FD26*/ CYCLES(0xfd26, 20);
                poke((ram_peek16al(0x0028) + s_y), s_a);
      /*$FD28*/ tmp5_U8 = io_peek(0xc000);
                s_a = tmp5_U8;
      /*$FD2B*/ tmp1_U8 = io_peek(0xc010);
                s_status_n = (tmp1_U8 & 0x80);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
                s_status_not_z = (tmp5_U8 & tmp1_U8);
      /*$FD2E*/ branchTarget = true; block_id = find_block_id_func_t001(0xfd2e, pop16() + 1);;
      break;
    case 2581:  // $FD32
      /*$FD32*/ CYCLES(0xfd32, 6);
                branchTarget = true; push16(0xfd34); block_id = 2457;
      break;
    case 2582:  // $FD35
      /*$FD35*/ CYCLES(0xfd35, 6);
                branchTarget = true; push16(0xfd37); block_id = 2574;
      break;
    case 2583:  // $FD38
      /*$FD38*/ CYCLES(0xfd38, 4);
                tmp1_U8 = s_a;
                tmp5_U8 = tmp1_U8 != 0x9b;
                s_status_not_z = tmp5_U8;
                s_status_c = (tmp1_U8 >= 0x9b);
                s_status_n = ((uint8_t)(tmp1_U8 - 0x9b) & 0x80);
      /*$FD3A*/ branchTarget = true; block_id = !tmp5_U8 ? 2584 : 2585;
      break;
    case 2584:  // $FD3A
      /*$FD3A*/ CYCLES_EDGE(0xfd3a, 1);
      /*$FD2F*/ CYCLES(0xfd2f, 6);
                branchTarget = true; push16(0xfd31); block_id = 2574;
      break;
    case 2585:  // $FD3C
      /*$FD3C*/ CYCLES(0xfd3c, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xfd3c, pop16() + 1);;
      break;
    case 2586:  // $FD4A
      /*$FD4A*/ CYCLES(0xfd4a, 15);
                tmp5_U8 = pop8();
      /*$FD4B*/ ram_poke(0x0032, tmp5_U8);
      /*$FD4D*/ tmp5_U8 = ram_peek((0x0200 + s_x));
                s_a = tmp5_U8;
      /*$FD50*/ s_status_c = (tmp5_U8 >= 0x88);
      /*$FD52*/ branchTarget = true; block_id = !(tmp5_U8 != 0x88) ? 2587 : 2588;
      break;
    case 2587:  // $FD52
      /*$FD52*/ CYCLES_EDGE(0xfd52, 1);
                branchTarget = true; block_id = 2599;
      break;
    case 2588:  // $FD54
      /*$FD54*/ CYCLES(0xfd54, 4);
                tmp5_U8 = s_a;
                s_status_c = (tmp5_U8 >= 0x98);
      /*$FD56*/ branchTarget = true; block_id = !(tmp5_U8 != 0x98) ? 2589 : 2590;
      break;
    case 2589:  // $FD56
      /*$FD56*/ CYCLES_EDGE(0xfd56, 1);
                branchTarget = true; block_id = 2595;
      break;
    case 2590:  // $FD58
      /*$FD58*/ CYCLES(0xfd58, 4);
                tmp5_U8 = s_x >= 0xf8;
                s_status_c = tmp5_U8;
      /*$FD5A*/ branchTarget = true; block_id = !tmp5_U8 ? 2591 : 2592;
      break;
    case 2591:  // $FD5A
      /*$FD5A*/ CYCLES_EDGE(0xfd5a, 1);
                branchTarget = true; block_id = 2593;
      break;
    case 2592:  // $FD5C
      /*$FD5C*/ CYCLES(0xfd5c, 6);
                branchTarget = true; push16(0xfd5e); block_id = 2637;
      break;
    case 2593:  // $FD5F
      /*$FD5F*/ CYCLES(0xfd5f, 4);
                tmp5_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp5_U8;
      /*$FD60*/ branchTarget = true; block_id = tmp5_U8 ? 2594 : 2595;
      break;
    case 2594:  // $FD60
      /*$FD60*/ CYCLES_EDGE(0xfd60, 1);
                branchTarget = true; block_id = 2602;
      break;
    case 2595:  // $FD62
      /*$FD62*/ CYCLES(0xfd62, 8);
                s_status_not_z = 0xdc;
                s_status_n = 0x80;
                s_a = 0xdc;
      /*$FD64*/ branchTarget = true; push16(0xfd66); block_id = 2623;
      break;
    case 2596:  // $FD67
      /*$FD67*/ CYCLES(0xfd67, 6);
                branchTarget = true; push16(0xfd69); block_id = 2612;
      break;
    case 2597:  // $FD6A
      /*$FD6A*/ CYCLES(0xfd6a, 9);
                tmp2_U8 = ram_peek(0x0033);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$FD6C*/ branchTarget = true; push16(0xfd6e); block_id = 2623;
      break;
    case 2598:  // $FD6F
      /*$FD6F*/ CYCLES(0xfd6f, 2);
                s_x = 0x01;
                block_id = 2599;
      break;
    case 2599:  // $FD71
      /*$FD71*/ CYCLES(0xfd71, 4);
                tmp5_U8 = s_x;
                s_a = tmp5_U8;
      /*$FD72*/ branchTarget = true; block_id = !tmp5_U8 ? 2600 : 2601;
      break;
    case 2600:  // $FD72
      /*$FD72*/ CYCLES_EDGE(0xfd72, 1);
                branchTarget = true; block_id = 2596;
      break;
    case 2601:  // $FD74
      /*$FD74*/ CYCLES(0xfd74, 2);
                s_x = (uint8_t)(s_x - 0x01);
                block_id = 2602;
      break;
    case 2602:  // $FD75
      /*$FD75*/ CYCLES(0xfd75, 6);
                branchTarget = true; push16(0xfd77); block_id = 2582;
      break;
    case 2603:  // $FD78
      /*$FD78*/ CYCLES(0xfd78, 4);
      /*$FD7A*/ branchTarget = true; block_id = (s_a != 0x95) ? 2604 : 2605;
      break;
    case 2604:  // $FD7A
      /*$FD7A*/ CYCLES_EDGE(0xfd7a, 1);
                branchTarget = true; block_id = 2606;
      break;
    case 2605:  // $FD7C
      /*$FD7C*/ CYCLES(0xfd7c, 5);
                tmp5_U8 = peek((ram_peek16al(0x0028) + s_y));
                s_a = tmp5_U8;
                block_id = 2606;
      break;
    case 2606:  // $FD7E
      /*$FD7E*/ CYCLES(0xfd7e, 4);
      /*$FD80*/ branchTarget = true; block_id = !(s_a >= 0xe0) ? 2607 : 2608;
      break;
    case 2607:  // $FD80
      /*$FD80*/ CYCLES_EDGE(0xfd80, 1);
                branchTarget = true; block_id = 2609;
      break;
    case 2608:  // $FD82
      /*$FD82*/ CYCLES(0xfd82, 2);
                s_a = (s_a & 0xdf);
                block_id = 2609;
      break;
    case 2609:  // $FD84
      /*$FD84*/ CYCLES(0xfd84, 9);
                tmp5_U8 = s_a;
                ram_poke((0x0200 + s_x), tmp5_U8);
      /*$FD87*/ s_status_c = (tmp5_U8 >= 0x8d);
      /*$FD89*/ branchTarget = true; block_id = (tmp5_U8 != 0x8d) ? 2610 : 2611;
      break;
    case 2610:  // $FD89
      /*$FD89*/ CYCLES_EDGE(0xfd89, 1);
      /*$FD3D*/ CYCLES(0xfd3d, 21);
      /*$FD3F*/ push8(ram_peek(0x0032));
      /*$FD42*/ ram_poke(0x0032, 0xff);
      /*$FD44*/ tmp5_U8 = ram_peek((0x0200 + s_x));
                s_status_not_z = tmp5_U8;
                s_status_n = (tmp5_U8 & 0x80);
                s_a = tmp5_U8;
      /*$FD47*/ branchTarget = true; push16(0xfd49); block_id = 2623;
      break;
    case 2611:  // $FD8B
      /*$FD8B*/ CYCLES(0xfd8b, 6);
                FUNC_CLREOL(0xfd8d);
                branchTarget = true; block_id = 2612;
      break;
    case 2612:  // $FD8E
      /*$FD8E*/ CYCLES(0xfd8e, 4);
                s_status_not_z = 0x8d;
                s_status_n = 0x80;
                s_a = 0x8d;
      /*$FD90*/ CYCLES_EDGE(0xfd90, 1);
                branchTarget = true; block_id = 2623;
      break;
    case 2613:  // $FD92
      /*$FD92*/ CYCLES(0xfd92, 12);
                s_y = ram_peek(0x003d);
      /*$FD94*/ s_x = ram_peek(0x003c);
      /*$FD96*/ branchTarget = true; push16(0xfd98); block_id = 2612;
      break;
    case 2614:  // $FD99
      /*$FD99*/ CYCLES(0xfd99, 6);
                branchTarget = true; push16(0xfd9b); block_id = 2406;
      break;
    case 2615:  // $FD9C
      /*$FD9C*/ CYCLES(0xfd9c, 7);
                s_y = 0x00;
      /*$FD9E*/ s_status_not_z = 0xad;
                s_status_n = 0x80;
                s_a = 0xad;
      /*$FDA0*/ branchTarget = true; block_id = 2623;
      break;
    case 2616:  // $FDDA
      /*$FDDA*/ CYCLES(0xfdda, 17);
                tmp2_U8 = s_a;
                push8(tmp2_U8);
      /*$FDDE*/ s_a = (tmp2_U8 >> 0x04);
      /*$FDDF*/ branchTarget = true; push16(0xfde1); block_id = 2618;
      break;
    case 2617:  // $FDE2
      /*$FDE2*/ CYCLES(0xfde2, 6);
                tmp5_U8 = pop8();
      /*$FDE3*/ s_a = (tmp5_U8 & 0x0f);
                block_id = 2618;
      break;
    case 2618:  // $FDE5
      /*$FDE5*/ CYCLES(0xfde5, 6);
                tmp5_U8 = s_a | 0xb0;
                s_a = tmp5_U8;
      /*$FDE7*/ s_status_not_z = (tmp5_U8 != 0xba);
                tmp1_U8 = tmp5_U8 >= 0xba;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp5_U8 - 0xba) & 0x80);
      /*$FDE9*/ branchTarget = true; block_id = !tmp1_U8 ? 2619 : 2620;
      break;
    case 2619:  // $FDE9
      /*$FDE9*/ CYCLES_EDGE(0xfde9, 1);
                branchTarget = true; block_id = 2623;
      break;
    case 2620:  // $FDEB
      /*$FDEB*/ CYCLES(0xfdeb, 2);
                block_id = s_status_d ? 2622 : 2621;
      break;
    case 2621:  // $FDEB
      /*$FDEB*/ tmp6_U16 = s_a;
                tmp3_U16 = (tmp6_U16 + 0x0006) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp6_U16, (uint8_t)0x0006);
                tmp1_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 2624;
      break;
    case 2622:  // $FDEB
      /*$FDEB*/ tmp3_U16 = adc_dec16(s_a, 0x06, s_status_c);
                s_a = ((uint8_t)tmp3_U16);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
                block_id = 2624;
      break;
    case 2623:  // $FDED
      /*$FDED*/ CYCLES(0xfded, 5);
                branchTarget = true; block_id = find_block_id_func_t001(0xfded, ram_peek16al(0x0036));
      break;
    case 2624:  // $FDED
      /*$FDEB*/ block_id = 2623;
      break;
    case 2625:  // $FDF0
      /*$FDF0*/ CYCLES(0xfdf0, 4);
                tmp1_U8 = s_a >= 0xa0;
                s_status_c = tmp1_U8;
      /*$FDF2*/ branchTarget = true; block_id = !tmp1_U8 ? 2626 : 2627;
      break;
    case 2626:  // $FDF2
      /*$FDF2*/ CYCLES_EDGE(0xfdf2, 1);
                branchTarget = true; block_id = 2628;
      break;
    case 2627:  // $FDF4
      /*$FDF4*/ CYCLES(0xfdf4, 3);
                s_a = (s_a & ram_peek(0x0032));
                block_id = 2628;
      break;
    case 2628:  // $FDF6
      /*$FDF6*/ CYCLES(0xfdf6, 12);
                ram_poke(0x0035, s_y);
      /*$FDF8*/ push8(s_a);
      /*$FDF9*/ branchTarget = true; push16(0xfdfb); block_id = 2442;
      break;
    case 2629:  // $FDFC
      /*$FDFC*/ CYCLES(0xfdfc, 13);
                tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$FDFD*/ tmp1_U8 = ram_peek(0x0035);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
      /*$FDFF*/ branchTarget = true; block_id = find_block_id_func_t001(0xfdff, pop16() + 1);;
      break;
    case 2630:  // $FE84
      /*$FE84*/ CYCLES(0xfe84, 11);
                s_status_not_z = 0xff;
                s_status_n = 0x80;
                s_y = 0xff;
      /*$FE86*/ ram_poke(0x0032, 0xff);
      /*$FE88*/ branchTarget = true; block_id = find_block_id_func_t001(0xfe88, pop16() + 1);;
      break;
    case 2631:  // $FE89
      /*$FE89*/ CYCLES(0xfe89, 11);
      /*$FE8B*/ ram_poke(0x003e, 0x00);
      /*$FE8D*/ s_x = 0x38;
      /*$FE8F*/ s_y = 0x1b;
      /*$FE91*/ CYCLES_EDGE(0xfe91, 1);
                branchTarget = true; block_id = 2633;
      break;
    case 2632:  // $FE93
      /*$FE93*/ CYCLES(0xfe93, 9);
      /*$FE95*/ ram_poke(0x003e, 0x00);
      /*$FE97*/ s_x = 0x36;
      /*$FE99*/ s_y = 0xf0;
                block_id = 2633;
      break;
    case 2633:  // $FE9B
      /*$FE9B*/ CYCLES(0xfe9b, 7);
      /*$FE9D*/ tmp1_U8 = ram_peek(0x003e) & 0x0f;
                s_a = tmp1_U8;
      /*$FE9F*/ branchTarget = true; block_id = !tmp1_U8 ? 2634 : 2635;
      break;
    case 2634:  // $FE9F
      /*$FE9F*/ CYCLES_EDGE(0xfe9f, 1);
      /*$FEA7*/ CYCLES(0xfea7, 2);
                s_status_not_z = 0xfd;
                s_status_n = 0x80;
                s_a = 0xfd;
                block_id = 2636;
      break;
    case 2635:  // $FEA1
      /*$FEA1*/ CYCLES(0xfea1, 6);
                s_a = (s_a | 0xc0);
      /*$FEA3*/ s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_y = 0x00;
      /*$FEA5*/ CYCLES_EDGE(0xfea5, 1);
                branchTarget = true; block_id = 2636;
      break;
    case 2636:  // $FEA9
      /*$FEA9*/ CYCLES(0xfea9, 14);
                tmp1_U8 = s_x;
                ram_poke(tmp1_U8, s_y);
      /*$FEAB*/ ram_poke((uint8_t)(0x01 + tmp1_U8), s_a);
      /*$FEAD*/ branchTarget = true; block_id = find_block_id_func_t001(0xfead, pop16() + 1);;
      break;
    case 2637:  // $FF3A
      /*$FF3A*/ CYCLES(0xff3a, 5);
                s_status_not_z = 0x87;
                s_status_n = 0x80;
                s_a = 0x87;
      /*$FF3C*/ branchTarget = true; block_id = 2623;
      break;
    case 2638:  // $FF4C
      /*$FF4C*/ CYCLES(0xff4c, 29);
                ram_poke(0x0046, s_x);
      /*$FF4E*/ ram_poke(0x0047, s_y);
      /*$FF50*/ push8((s_status_c | ((s_status_not_z == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | s_status_n));
      /*$FF51*/ tmp2_U8 = pop8();
                s_a = tmp2_U8;
      /*$FF52*/ ram_poke(0x0048, tmp2_U8);
      /*$FF54*/ tmp2_U8 = s_sp;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$FF55*/ ram_poke(0x0049, tmp2_U8);
      /*$FF57*/ s_status_d = 0x00;
      /*$FF58*/ branchTarget = true; block_id = find_block_id_func_t001(0xff58, pop16() + 1);;
      break;
    case 2639:
      /*$E07B*/ FUNC_ISLETC(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xe07b, pop16() + 1);;
      break;
    case 2640:
      /*$E482*/ FUNC_GARBAGE(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xe482, pop16() + 1);;
      break;
    case 2641:
      /*$E517*/ FUNC_CHKSMPLVAR(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xe517, pop16() + 1);;
      break;
    case 2642:
      /*$E8C4*/ FUNC_INCFACMAN(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xe8c4, pop16() + 1);;
      break;
    case 2643:
      /*$FC9A*/ FUNC_CLREOL(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xfc9a, pop16() + 1);;
      break;
    case 2644:  // $10100
      /*$7374*/ push8((s_status_c | ((s_status_not_z == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | s_status_n));
      /*$FA40*/ CYCLES(0xfa40, 18);
                ram_poke(0x0045, s_a);
      /*$FA42*/ tmp2_U8 = pop8();
      /*$FA43*/ push8(tmp2_U8);
      /*$FA46*/ tmp6_U16 = tmp2_U8 << 0x03;
                tmp2_U8 = (uint8_t)tmp6_U16;
                s_status_c = (uint8_t)((tmp6_U16 & 0x01ff) >> 8);
                s_status_not_z = tmp2_U8;
                tmp1_U8 = tmp2_U8 & 0x80;
                s_status_n = tmp1_U8;
                s_a = tmp2_U8;
      /*$FA47*/ branchTarget = true; block_id = tmp1_U8 ? 2408 : 2409;
      break;
    default:
      fprintf(stderr, "panic: unknown block_id: %u\n", block_id);
      abort();
    }
  }
}

static const unsigned s_block_map_func_t001[] = {
    0x00b1,    2, 0x3750,   17, 0x376b,   23, 0x376e,   24, 0x60e7,   27,
    0x614b,   36, 0x614e,   37, 0x615d,   42, 0x6160,   43, 0x625b,   56,
    0x628b,   61, 0x628e,   62, 0x6291,   63, 0x62b8,   72, 0x62c8,   73,
    0x62d1,   74, 0x62ee,   81, 0x6303,   82, 0x6310,   87, 0x6333,   99,
    0x633c,  100, 0x63ba,  118, 0x63c0,  119, 0x63c9,  120, 0x63da,  121,
    0x63f6,  128, 0x640c,  132, 0x6429,  136, 0x6444,  139, 0x6449,  140,
    0x6450,  141, 0x6456,  143, 0x645b,  144, 0x648a,  154, 0x6493,  155,
    0x64eb,  183, 0x64f8,  186, 0x6516,  190, 0x6553,  197, 0x6560,  198,
    0x6569,  199, 0x6587,  202, 0x658a,  203, 0x6593,  204, 0x65b7,  208,
    0x664a,  213, 0x69d7,  251, 0x69fe,  259, 0x6b40,  332, 0x6b65,  336,
    0x6b70,  337, 0x6b82,  338, 0x6b8c,  339, 0x6b96,  341, 0x6bdd,  349,
    0x6be9,  352, 0x6bf2,  354, 0x6c75,  381, 0x700a,  421, 0x7048,  439,
    0x7075,  447, 0x7080,  448, 0x7093,  451, 0x709e,  454, 0x70b3,  458,
    0x70ba,  459, 0x70c9,  460, 0x70d4,  461, 0x70e1,  462, 0x70ec,  463,
    0x70f5,  464, 0x7100,  465, 0x7113,  466, 0x712e,  473, 0x71ba,  484,
    0x720e,  506, 0x7225,  515, 0x7239,  520, 0x7248,  526, 0x72d9,  558,
    0x72e2,  559, 0x72ec,  560, 0x72f2,  561, 0x72f8,  562, 0x72fe,  563,
    0x7301,  564, 0x7308,  565, 0x7314,  566, 0x731e,  567, 0x7324,  568,
    0x732a,  569, 0x7330,  570, 0x7333,  571, 0x733e,  572, 0x734d,  575,
    0x7357,  576, 0x735d,  577, 0x7360,  578, 0x7365,  579, 0x736c,  580,
    0x7375,  581, 0x737f,  589, 0x7385,  590, 0x7388,  591, 0x7393,  592,
    0x73a2,  593, 0x73a9,  594, 0x73af,  595, 0x73b2,  596, 0x73b9,  597,
    0x73c2,  598, 0x73cc,  603, 0x73cf,  604, 0x73d6,  605, 0x73f4,  608,
    0x7414,  609, 0x748f,  638, 0x7541,  639, 0x7549,  641, 0x757c,  645,
    0x7582,  646, 0x75ac,  656, 0x75b0,  657, 0x75e0,  659, 0x75e7,  660,
    0x7602,  667, 0x760a,  668, 0x7645,  686, 0x764a,  687, 0x7653,  688,
    0x765a,  691, 0x7661,  692, 0x766c,  693, 0x7694,  700, 0x7697,  701,
    0x76e4,  705, 0x76ed,  706, 0x76f6,  707, 0x76f9,  708, 0x7710,  709,
    0x7713,  710, 0x7716,  711, 0x7726,  713, 0x7729,  714, 0x7739,  721,
    0x7787,  745, 0x77e3,  758, 0x7803,  765, 0x7868,  790, 0x7886,  791,
    0x78d1,  809, 0x7945,  810, 0x794d,  811, 0x7957,  812, 0x795d,  813,
    0xd396,  830, 0xd41c,  890, 0xd41f,  891, 0xd426,  892, 0xd42d,  895,
    0xd434,  897, 0xd43c,  900, 0xd43f,  901, 0xd444,  902, 0xd44d,  903,
    0xd459,  908, 0xd467,  909, 0xd46a,  910, 0xd4d6,  949, 0xd4f5,  953,
    0xd533,  969, 0xd556,  978, 0xd569,  982, 0xd668, 1077, 0xd683, 1079,
    0xd7d8, 1089, 0xd813, 1107, 0xd81a, 1108, 0xd81d, 1109, 0xd820, 1110,
    0xd823, 1111, 0xd863, 1136, 0xd941, 1151, 0xd944, 1152, 0xda03, 1187,
    0xda06, 1188, 0xda43, 1213, 0xda49, 1214, 0xda52, 1215, 0xda5b, 1216,
    0xda60, 1217, 0xda68, 1222, 0xda6b, 1223, 0xdaa8, 1237, 0xdb00, 1241,
    0xdb3d, 1244, 0xdb4c, 1248, 0xdb67, 1257, 0xdb6f, 1258, 0xdd6a, 1260,
    0xdd8e, 1273, 0xdd91, 1274, 0xdd98, 1276, 0xddb1, 1286, 0xddd6, 1311,
    0xddda, 1313, 0xde08, 1327, 0xde23, 1329, 0xde41, 1336, 0xde67, 1341,
    0xde8d, 1360, 0xdeb5, 1371, 0xdeb8, 1372, 0xded8, 1380, 0xdefc, 1385,
    0xdeff, 1386, 0xdf09, 1387, 0xdf16, 1390, 0xdf1d, 1393, 0xdf20, 1394,
    0xdf23, 1395, 0xdf26, 1396, 0xdf33, 1397, 0xdf3d, 1398, 0xdfe8, 1401,
    0xdfef, 1403, 0xe00a, 1406, 0xe015, 1412, 0xe03d, 1424, 0xe0b9, 1472,
    0xe105, 1491, 0xe108, 1492, 0xe137, 1503, 0xe156, 1504, 0xe15f, 1507,
    0xe1c7, 1536, 0xe201, 1553, 0xe21d, 1568, 0xe27a, 1595, 0xe29f, 1611,
    0xe346, 1642, 0xe34d, 1643, 0xe357, 1644, 0xe360, 1645, 0xe363, 1646,
    0xe383, 1652, 0xe39b, 1653, 0xe3a4, 1654, 0xe3e0, 1658, 0xe423, 1680,
    0xe5a0, 1784, 0xe5a3, 1785, 0xe5ba, 1791, 0xe5d1, 1792, 0xe6fb, 1834,
    0xe6fe, 1835, 0xe75b, 1841, 0xe7bc, 1843, 0xe7fa, 1865, 0xe98a, 1967,
    0xea3c, 2011, 0xea4b, 2019, 0xea58, 2023, 0xea6e, 2026, 0xea78, 2030,
    0xeb2e, 2071, 0xeb66, 2076, 0xec0f, 2129, 0xec64, 2142, 0xec71, 2149,
    0xec8a, 2163, 0xecac, 2181, 0xecb5, 2186, 0xeccb, 2194, 0xecd2, 2198,
    0xecd9, 2200, 0xecdd, 2201, 0xed20, 2217, 0xed2e, 2219, 0xed31, 2220,
    0xed5e, 2231, 0xed7b, 2242, 0xed82, 2245, 0xed89, 2248, 0xf13e, 2335,
    0xf16b, 2339, 0xf1bf, 2348, 0xf1c2, 2349, 0xf1d5, 2350, 0xf1d8, 2351,
    0xf1db, 2352, 0xf1ef, 2354, 0xf1fa, 2357, 0xf1fd, 2358, 0xf312, 2366,
    0xf315, 2367, 0xf805, 2369, 0xf81c, 2378, 0xf824, 2381, 0xf876, 2402,
    0xf944, 2407, 0xfa66, 2410, 0xfa69, 2411, 0xfa6c, 2412, 0xfa6f, 2413,
    0xfa85, 2414, 0xfaa9, 2422, 0xfb63, 2438, 0xfba2, 2456, 0xfba5, 2457,
    0xfbe2, 2475, 0xfbe9, 2477, 0xfc27, 2501, 0xfc4a, 2530, 0xfc4d, 2531,
    0xfd1b, 2575, 0xfd32, 2581, 0xfd35, 2582, 0xfd38, 2583, 0xfd4a, 2586,
    0xfd5f, 2593, 0xfd67, 2596, 0xfd6a, 2597, 0xfd6f, 2598, 0xfd78, 2603,
    0xfd8e, 2612, 0xfd99, 2614, 0xfd9c, 2615, 0xfde2, 2617, 0xfdf0, 2625,
    0xfdfc, 2629
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
