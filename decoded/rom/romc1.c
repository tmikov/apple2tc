// Loaded segment [$0090..$0092]
// Loaded segment [$00B1..$00C8]
// 355 new runtime blocks added
// code labels: 1718
// data labels: 243

#include "apple2tc/system2-inc.h"

static const uint8_t s_mem_0090[0x0003];
static const uint8_t s_mem_00b1[0x0018];
static const uint8_t s_mem_d000[0x3000];

void init_emulated(void) {
  memcpy(s_ram + 0x0090, s_mem_0090, 0x0003);
  memcpy(s_ram + 0x00b1, s_mem_00b1, 0x0018);
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
void FUNC_CHRGET(uint16_t ret_addr);
void FUNC_CHRGOT(uint16_t ret_addr);
void FUNC_GETFORPTR(uint16_t ret_addr);
void FUNC_MVBLKUP2(uint16_t ret_addr);
void FUNC_FNDLIN(uint16_t ret_addr);
void FUNC_FNDLIN2(uint16_t ret_addr);
void FUNC_CLRTXTPTR(uint16_t ret_addr);
void FUNC_GETCHAR(uint16_t ret_addr);
void FUNC_ISLETC(uint16_t ret_addr);
void FUNC_GETARY(uint16_t ret_addr);
void FUNC_GARBAGE(uint16_t ret_addr);
void FUNC_CHKSMPLVAR(uint16_t ret_addr);
void FUNC_CHKVAR(uint16_t ret_addr);
void FUNC_FRETMS(uint16_t ret_addr);
void FUNC_POINT(uint16_t ret_addr);
void FUNC_TWSCMPFAC(uint16_t ret_addr);
void FUNC_TCFACMANT(uint16_t ret_addr);
void FUNC_INCFACMAN(uint16_t ret_addr);
void FUNC_SHFTRGHT(uint16_t ret_addr);
void FUNC_SHFTRGHT4(uint16_t ret_addr);
void FUNC_MULT1(uint16_t ret_addr);
void FUNC_MULT2(uint16_t ret_addr);
void FUNC_UPAY2ARG(uint16_t ret_addr);
void FUNC_UPAY2FAC(uint16_t ret_addr);
void FUNC_SIGN(uint16_t ret_addr);
void FUNC_FCOMP(uint16_t ret_addr);
void FUNC_FCOMP2(uint16_t ret_addr);
void FUNC_QINT(uint16_t ret_addr);
void FUNC_HPOSN(uint16_t ret_addr);
void FUNC_HPLOT0(uint16_t ret_addr);
void FUNC_COLORSHIFT(uint16_t ret_addr);
void FUNC_PLOT1(uint16_t ret_addr);
void FUNC_GBASCALC(uint16_t ret_addr);
void FUNC_SCRN2(uint16_t ret_addr);
void FUNC_PCADJ(uint16_t ret_addr);
void FUNC_PCADJ3(uint16_t ret_addr);
void FUNC_PREAD(uint16_t ret_addr);
void FUNC_INIT(uint16_t ret_addr);
void FUNC_BASCALC(uint16_t ret_addr);
void FUNC_VTABZ(uint16_t ret_addr);
void FUNC_CLREOL(uint16_t ret_addr);
void FUNC_CLREOLZ(uint16_t ret_addr);
void FUNC_NXTA1(uint16_t ret_addr);
void FUNC_HEADER(uint16_t ret_addr);
void FUNC_ZERODLY(uint16_t ret_addr);
void FUNC_RD2BIT(uint16_t ret_addr);
void FUNC_RDBIT(uint16_t ret_addr);
void FUNC_A1PC(uint16_t ret_addr);
void FUNC_SETNORM(uint16_t ret_addr);
void FUNC_SETKBD(uint16_t ret_addr);
void FUNC_SETVID(uint16_t ret_addr);
void FUNC_MON_RESTORE(uint16_t ret_addr);
void FUNC_GETNUM(uint16_t ret_addr);
void FUNC_ZMODE(uint16_t ret_addr);

static void emulated_entry_point(void) {
  func_t001(false);
}

void FUNC_CHRGET(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;
  uint16_t tmp5_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$00B1*/ CYCLES(0x00b1, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x00b8) + 0x01);
            ram_poke(0x00b8, tmp1_U8);
            branchTarget = true;
            if (tmp1_U8)
              goto bb_2;
bb_1:
  /*$00B5*/ CYCLES(0x00b5, 4);
            ram_poke(0x00b9, (uint8_t)(ram_peek(0x00b9) + 0x01));
bb_2:
  /*$00B7*/ CYCLES(0x00b7, 12);
            tmp1_U8 = peek(ram_peek16al(0x00b8));
            s_a = tmp1_U8;
  /*$00BA*/ s_status_not_z = (tmp1_U8 != 0x3a);
            tmp2_U8 = tmp1_U8 >= 0x3a;
            s_status_c = tmp2_U8;
            s_status_n = ((uint8_t)(tmp1_U8 - 0x3a) & 0x80);
            branchTarget = true;
            if (tmp2_U8)
              goto bb_5;
bb_3:
  /*$00BE*/ CYCLES(0x00be, 7);
            branchTarget = true;
            if (!(s_a != 0x20))
              goto bb_0;
bb_4:
  /*$00C2*/ CYCLES(0x00c2, 11);
  /*$00C3*/ tmp3_U16 = s_a;
  /*$00C6*/ tmp4_U16 = (tmp3_U16 - 0x0030) & 0x00ff;
            tmp5_U16 = tmp4_U16 - 0x00d0;
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0xff2f);
            tmp2_U8 = (uint8_t)(tmp3_U16 - 0x0100);
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_a = tmp2_U8;
bb_5:
  /*$00C8*/ CYCLES(0x00c8, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_CHRGOT(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;
  uint16_t tmp5_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$00B7*/ CYCLES(0x00b7, 12);
            tmp1_U8 = peek(ram_peek16al(0x00b8));
            s_a = tmp1_U8;
  /*$00BA*/ s_status_not_z = (tmp1_U8 != 0x3a);
            tmp2_U8 = tmp1_U8 >= 0x3a;
            s_status_c = tmp2_U8;
            s_status_n = ((uint8_t)(tmp1_U8 - 0x3a) & 0x80);
            branchTarget = true;
            if (tmp2_U8)
              goto bb_5;
            else
              goto bb_3;
bb_1:
  /*$00B1*/ CYCLES(0x00b1, 7);
            tmp2_U8 = (uint8_t)(ram_peek(0x00b8) + 0x01);
            ram_poke(0x00b8, tmp2_U8);
            branchTarget = true;
            if (tmp2_U8)
              goto bb_0;
bb_2:
  /*$00B5*/ CYCLES(0x00b5, 4);
            ram_poke(0x00b9, (uint8_t)(ram_peek(0x00b9) + 0x01));
            goto bb_0;
bb_3:
  /*$00BE*/ CYCLES(0x00be, 7);
            branchTarget = true;
            if (!(s_a != 0x20))
              goto bb_1;
bb_4:
  /*$00C2*/ CYCLES(0x00c2, 11);
  /*$00C3*/ tmp3_U16 = s_a;
  /*$00C6*/ tmp4_U16 = (tmp3_U16 - 0x0030) & 0x00ff;
            tmp5_U16 = tmp4_U16 - 0x00d0;
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0xff2f);
            tmp2_U8 = (uint8_t)(tmp3_U16 - 0x0100);
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_a = tmp2_U8;
bb_5:
  /*$00C8*/ CYCLES(0x00c8, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_GETFORPTR(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;
  uint16_t tmp4_U16;
  uint16_t tmp5_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$D365*/ CYCLES(0xd365, 9);
  /*$D369*/ s_x = (uint8_t)(s_sp + 0x04);
bb_1:
  /*$D36A*/ CYCLES(0xd36a, 12);
            tmp1_U8 = ram_peek((0x0101 + s_x));
            s_a = tmp1_U8;
  /*$D36D*/ tmp2_U8 = tmp1_U8 != 0x81;
            s_status_not_z = tmp2_U8;
            s_status_c = (tmp1_U8 >= 0x81);
            branchTarget = true;
            if (tmp2_U8)
              goto bb_7;
bb_2:
  /*$D371*/ CYCLES(0xd371, 7);
            tmp2_U8 = ram_peek(0x0086);
            s_a = tmp2_U8;
            branchTarget = true;
            if (tmp2_U8)
              goto bb_4;
bb_3:
  /*$D375*/ CYCLES(0xd375, 18);
            tmp2_U8 = s_x;
  /*$D378*/ ram_poke(0x0085, ram_peek((0x0102 + tmp2_U8)));
  /*$D37A*/ tmp2_U8 = ram_peek((0x0103 + tmp2_U8));
            s_a = tmp2_U8;
  /*$D37D*/ ram_poke(0x0086, tmp2_U8);
bb_4:
  /*$D37F*/ CYCLES(0xd37f, 9);
            branchTarget = true;
            if ((s_a != ram_peek((0x0103 + s_x))))
              goto bb_6;
bb_5:
  /*$D384*/ CYCLES(0xd384, 12);
            tmp2_U8 = ram_peek(0x0085);
            s_a = tmp2_U8;
  /*$D386*/ tmp1_U8 = ram_peek((0x0102 + s_x));
            tmp3_U8 = tmp2_U8 != tmp1_U8;
            s_status_not_z = tmp3_U8;
            s_status_c = (tmp2_U8 >= tmp1_U8);
            branchTarget = true;
            if (!tmp3_U8)
              goto bb_7;
bb_6:
  /*$D38B*/ CYCLES(0xd38b, 12);
  /*$D38D*/ tmp4_U16 = s_x;
            tmp5_U16 = tmp4_U16 + 0x0012;
            s_status_c = (uint8_t)(tmp5_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x0012);
            tmp3_U8 = (uint8_t)tmp5_U16;
            s_a = tmp3_U8;
  /*$D38F*/ s_status_not_z = tmp3_U8;
            s_x = tmp3_U8;
            branchTarget = true;
            if (tmp3_U8)
              goto bb_1;
bb_7:
  /*$D392*/ CYCLES(0xd392, 2);
            if (ret_addr) pop16(); return;
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
  /*$D39D*/ tmp1_U16 = ram_peek(0x0096) - ram_peek(0x009b);
            tmp2_U8 = (uint8_t)tmp1_U16;
  /*$D39F*/ ram_poke(0x005e, tmp2_U8);
  /*$D3A1*/ s_y = tmp2_U8;
  /*$D3A4*/ tmp3_U16 = ram_peek(0x0097);
            tmp4_U16 = ram_peek(0x009c);
            tmp1_U16 = (tmp3_U16 - tmp4_U16) - ((uint8_t)(tmp1_U16 >> 8) & 0x01);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp1_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp1_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp4_U16));
  /*$D3A7*/ s_x = (uint8_t)(((uint8_t)tmp1_U16) + 0x01);
  /*$D3A8*/ s_a = tmp2_U8;
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_8;
bb_1:
  /*$D3AB*/ CYCLES(0xd3ab, 16);
  /*$D3AE*/ tmp1_U16 = ram_peek(0x0096) - ram_peek(0x005e);
            tmp2_U8 = (uint8_t)(0x01 - ((uint8_t)(tmp1_U16 >> 8) & 0x01));
            s_status_c = tmp2_U8;
  /*$D3B0*/ ram_poke(0x0096, ((uint8_t)tmp1_U16));
            branchTarget = true;
            if (tmp2_U8)
              goto bb_3;
bb_2:
  /*$D3B4*/ CYCLES(0xd3b4, 6);
            ram_poke(0x0097, (uint8_t)(ram_peek(0x0097) - 0x01));
  /*$D3B6*/ s_status_c = 0x01;
bb_3:
  /*$D3B7*/ CYCLES(0xd3b7, 14);
  /*$D3B9*/ tmp1_U16 = ram_peek(0x0094);
            tmp4_U16 = ram_peek(0x005e);
            tmp3_U16 = (tmp1_U16 - tmp4_U16) - (uint8_t)(0x01 - s_status_c);
            tmp2_U8 = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
            s_status_c = tmp2_U8;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp1_U16, (uint8_t)(~tmp4_U16));
  /*$D3BB*/ ram_poke(0x0094, ((uint8_t)tmp3_U16));
            branchTarget = true;
            if (tmp2_U8)
              goto bb_6;
bb_4:
  /*$D3BF*/ CYCLES(0xd3bf, 7);
            ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) - 0x01));
            branchTarget = true;
            if (!s_status_c)
              goto bb_6;
bb_5:
  /*$D3C3*/ CYCLES(0xd3c3, 7);
            tmp2_U8 = s_y;
            tmp5_U8 = peek((ram_peek16al(0x0096) + tmp2_U8));
  /*$D3C5*/ poke((ram_peek16al(0x0094) + tmp2_U8), tmp5_U8);
bb_6:
  /*$D3C7*/ CYCLES(0xd3c7, 6);
            tmp2_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp2_U8;
            branchTarget = true;
            if (tmp2_U8)
              goto bb_5;
bb_7:
  /*$D3CA*/ CYCLES(0xd3ca, 7);
            tmp2_U8 = s_y;
            tmp5_U8 = peek((ram_peek16al(0x0096) + tmp2_U8));
            s_a = tmp5_U8;
  /*$D3CC*/ poke((ram_peek16al(0x0094) + tmp2_U8), tmp5_U8);
bb_8:
  /*$D3CE*/ CYCLES(0xd3ce, 12);
            ram_poke(0x0097, (uint8_t)(ram_peek(0x0097) - 0x01));
  /*$D3D0*/ ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) - 0x01));
  /*$D3D2*/ tmp2_U8 = (uint8_t)(s_x - 0x01);
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_x = tmp2_U8;
            branchTarget = true;
            if (tmp2_U8)
              goto bb_6;
bb_9:
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
  uint16_t tmp2_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$D697*/ CYCLES(0xd697, 24);
  /*$D69A*/ tmp1_U16 = ram_peek(0x0067) + 0x00ff;
  /*$D69C*/ ram_poke(0x00b8, ((uint8_t)tmp1_U16));
  /*$D6A0*/ tmp2_U16 = ram_peek(0x0068);
            tmp1_U16 = (tmp2_U16 + 0x00ff) + (uint8_t)(tmp1_U16 >> 8);
            s_status_c = (uint8_t)(tmp1_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp1_U16, (uint8_t)tmp2_U16, (uint8_t)0x00ff);
  /*$D6A2*/ ram_poke(0x00b9, ((uint8_t)tmp1_U16));
  /*$D6A4*/ if (ret_addr) pop16(); return;
}


void FUNC_GETCHAR(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$D72C*/ CYCLES(0xd72c, 6);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_2;
bb_1:
  /*$D72F*/ CYCLES(0xd72f, 4);
            ram_poke(0x009e, (uint8_t)(ram_peek(0x009e) + 0x01));
bb_2:
  /*$D731*/ CYCLES(0xd731, 6);
            tmp1_U8 = peek((ram_peek16(0x009d) + s_y));
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
  /*$D733*/ if (ret_addr) pop16(); return;
}


void FUNC_ISLETC(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;
  uint16_t tmp5_U16;

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
              goto bb_2;
bb_1:
  /*$E081*/ CYCLES(0xe081, 9);
            tmp3_U16 = (s_a - 0x005b) - (uint8_t)(0x01 - s_status_c);
  /*$E084*/ tmp4_U16 = tmp3_U16 & 0x00ff;
            tmp5_U16 = tmp4_U16 - 0x00a5;
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0xff5a);
            tmp2_U8 = (uint8_t)(tmp3_U16 - 0x00a5);
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_a = tmp2_U8;
bb_2:
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
  /*$E0F0*/ tmp1_U16 = ((tmp1_U16 & 0x00ff) + 0x0005) + (uint8_t)(tmp1_U16 >> 8);
  /*$E0F2*/ tmp2_U16 = tmp1_U16 & 0x00ff;
            tmp3_U16 = ram_peek(0x009b);
            tmp1_U16 = (tmp2_U16 + tmp3_U16) + (uint8_t)(tmp1_U16 >> 8);
            tmp4_U8 = (uint8_t)(tmp1_U16 >> 8);
            s_status_c = tmp4_U8;
            s_status_v = ovf8((uint8_t)tmp1_U16, (uint8_t)tmp2_U16, (uint8_t)tmp3_U16);
            s_a = ((uint8_t)tmp1_U16);
  /*$E0F4*/ s_y = ram_peek(0x009c);
            branchTarget = true;
            if (!tmp4_U8)
              goto bb_2;
bb_1:
  /*$E0F8*/ CYCLES(0xe0f8, 2);
            s_y = (uint8_t)(s_y + 0x01);
bb_2:
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
            FUNC_CHKVAR(0xe4a6);
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
            FUNC_CHKSMPLVAR(0xe4bf);
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
              goto bb_21;
            else
              goto bb_22;
bb_13:
  /*$E4D9*/ CYCLES(0xe4d9, 52);
            ram_poke(0x005e, s_a);
  /*$E4DB*/ ram_poke(0x005f, s_x);
  /*$E4DF*/ tmp2_U8 = peek(ram_peek16al(0x005e));
  /*$E4E1*/ s_x = tmp2_U8;
  /*$E4E3*/ tmp2_U8 = peek((ram_peek16al(0x005e) + 0x0001));
  /*$E4E5*/ tmp1_U8 = s_status_c;
            push8((tmp1_U8 | ((tmp2_U8 == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | (tmp2_U8 & 0x80)));
  /*$E4E7*/ tmp2_U8 = peek((ram_peek16al(0x005e) + 0x0002));
  /*$E4E9*/ tmp3_U16 = (tmp2_U8 + ram_peek(0x0094)) + tmp1_U8;
  /*$E4EB*/ ram_poke(0x0094, ((uint8_t)tmp3_U16));
  /*$E4ED*/ s_y = 0x03;
  /*$E4EE*/ tmp2_U8 = peek((ram_peek16al(0x005e) + 0x0003));
  /*$E4F2*/ ram_poke(0x0095, (uint8_t)((tmp2_U8 + ram_peek(0x0095)) + (uint8_t)(tmp3_U16 >> 8)));
  /*$E4F4*/ tmp2_U8 = pop8();
            s_status_i = ((tmp2_U8 & 0x04) != 0);
            s_status_d = ((tmp2_U8 & 0x08) != 0);
            s_status_b = 0x00;
            s_status_v = ((tmp2_U8 & 0x40) != 0);
            branchTarget = true;
            if (!(tmp2_U8 & 0x80))
              goto bb_9;
bb_14:
  /*$E4F7*/ CYCLES(0xe4f7, 6);
            branchTarget = true;
            if ((s_x & 0x80))
              goto bb_9;
bb_15:
  /*$E4FA*/ CYCLES(0xe4fa, 24);
  /*$E4FB*/ tmp2_U8 = peek((ram_peek16al(0x005e) + (uint8_t)(s_y + 0x01)));
  /*$E4FD*/ s_y = 0x00;
  /*$E4FF*/ tmp3_U16 = tmp2_U8 << 0x01;
  /*$E500*/ tmp3_U16 = ((tmp3_U16 & 0x00ff) + 0x0005) + (uint8_t)(tmp3_U16 >> 8);
  /*$E502*/ tmp4_U16 = tmp3_U16 & 0x00ff;
            tmp5_U16 = ram_peek(0x005e);
            tmp3_U16 = (tmp4_U16 + tmp5_U16) + (uint8_t)(tmp3_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp5_U16);
            tmp2_U8 = (uint8_t)tmp3_U16;
            s_a = tmp2_U8;
  /*$E504*/ ram_poke(0x005e, tmp2_U8);
            branchTarget = true;
            if (!(uint8_t)(tmp3_U16 >> 8))
              goto bb_17;
bb_16:
  /*$E508*/ CYCLES(0xe508, 4);
            ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) + 0x01));
bb_17:
  /*$E50A*/ CYCLES(0xe50a, 4);
            s_x = ram_peek(0x005f);
bb_18:
  /*$E50C*/ CYCLES(0xe50c, 7);
            branchTarget = true;
            if ((s_x != ram_peek(0x0095)))
              goto bb_20;
bb_19:
  /*$E510*/ CYCLES(0xe510, 7);
            branchTarget = true;
            if (!(s_a != ram_peek(0x0094)))
              goto bb_10;
bb_20:
  /*$E514*/ CYCLES(0xe514, 6);
            FUNC_CHKVAR(0xe516);
  /*$E517*/ CYCLES(0xe517, 4);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_18;
            else
              goto bb_23;
bb_21:
  /*$E55D*/ CYCLES(0xe55d, 9);
            s_x = ram_peek(0x005f);
  /*$E55F*/ s_status_not_z = 0x00;
            s_status_n = 0x00;
            s_y = 0x00;
  /*$E561*/ if (ret_addr) pop16(); return;
bb_22:
  /*$E566*/ CYCLES(0xe566, 53);
  /*$E56A*/ tmp2_U8 = (ram_peek(0x0091) & 0x04) >> 0x01;
  /*$E56C*/ ram_poke(0x0091, tmp2_U8);
  /*$E56E*/ tmp2_U8 = peek((ram_peek16al(0x008a) + tmp2_U8));
  /*$E570*/ tmp3_U16 = tmp2_U8 + ram_peek(0x009b);
  /*$E572*/ ram_poke(0x0096, ((uint8_t)tmp3_U16));
  /*$E576*/ tmp4_U16 = ram_peek(0x009c);
            tmp3_U16 = tmp4_U16 + (uint8_t)(tmp3_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0x0000);
  /*$E578*/ ram_poke(0x0097, ((uint8_t)tmp3_U16));
  /*$E57E*/ ram_poke(0x0094, ram_peek(0x006f));
  /*$E580*/ ram_poke(0x0095, ram_peek(0x0070));
  /*$E582*/ FUNC_MVBLKUP2(0xe584);
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
bb_23:
  /*$E517*/ FUNC_CHKSMPLVAR(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_CHKSMPLVAR(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;
  uint8_t tmp5_U8;

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
  /*$E555*/ tmp2_U16 = ram_peek(0x008f);
            tmp3_U16 = ram_peek(0x005e);
            tmp4_U16 = tmp2_U16 + tmp3_U16;
            tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
            s_status_c = tmp1_U8;
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp2_U16, (uint8_t)tmp3_U16);
            tmp5_U8 = (uint8_t)tmp4_U16;
            s_a = tmp5_U8;
  /*$E557*/ ram_poke(0x005e, tmp5_U8);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_5;
bb_4:
  /*$E55B*/ CYCLES(0xe55b, 4);
            ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) + 0x01));
bb_5:
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
  /*$E555*/ tmp3_U16 = ram_peek(0x008f);
            tmp4_U16 = ram_peek(0x005e);
            tmp5_U16 = tmp3_U16 + tmp4_U16;
            tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
            s_status_c = tmp1_U8;
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
            tmp2_U8 = (uint8_t)tmp5_U16;
            s_a = tmp2_U8;
  /*$E557*/ ram_poke(0x005e, tmp2_U8);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_10;
bb_9:
  /*$E55B*/ CYCLES(0xe55b, 4);
            ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) + 0x01));
bb_10:
  /*$E55D*/ CYCLES(0xe55d, 9);
            s_x = ram_peek(0x005f);
  /*$E55F*/ s_status_not_z = 0x00;
            s_status_n = 0x00;
            s_y = 0x00;
  /*$E561*/ if (ret_addr) pop16(); return;
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
              goto bb_3;
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
              goto bb_3;
bb_2:
  /*$E63D*/ CYCLES(0xe63d, 14);
            tmp1_U8 = s_a;
            ram_poke(0x0052, tmp1_U8);
  /*$E63F*/ tmp4_U16 = tmp1_U8;
            tmp5_U16 = (tmp4_U16 - 0x0003) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0xfffc);
  /*$E641*/ ram_poke(0x0053, ((uint8_t)tmp5_U16));
  /*$E643*/ s_status_not_z = 0x00;
            s_status_n = 0x00;
bb_3:
  /*$E645*/ CYCLES(0xe645, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_POINT(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$E73D*/ CYCLES(0xe73d, 16);
            tmp1_U8 = ram_peek(0x00ad);
            s_x = tmp1_U8;
  /*$E73F*/ tmp2_U8 = ram_peek(0x00ae);
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_y = tmp2_U8;
  /*$E741*/ ram_poke(0x00b8, tmp1_U8);
  /*$E743*/ ram_poke(0x00b9, tmp2_U8);
  /*$E745*/ if (ret_addr) pop16(); return;
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
            tmp1_U16 = (s_a + 0x0008) + s_status_c;
            s_status_c = (uint8_t)(tmp1_U16 >> 8);
            tmp2_U8 = (uint8_t)tmp1_U16;
            s_status_not_z = tmp2_U8;
            s_a = tmp2_U8;
            branchTarget = true;
            if (!(tmp2_U8 & 0x80))
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
  /*$E8F4*/ CYCLES(0xe8f4, 4);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_1;
bb_3:
  /*$E8F6*/ CYCLES(0xe8f6, 12);
            tmp1_U16 = s_a;
            tmp3_U16 = (tmp1_U16 - 0x0008) - (uint8_t)(0x01 - s_status_c);
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp1_U16, (uint8_t)0xfff7);
  /*$E8F8*/ s_y = ((uint8_t)tmp3_U16);
  /*$E8F9*/ tmp2_U8 = ram_peek(0x00ac);
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_a = tmp2_U8;
            branchTarget = true;
            if ((uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01)))
              goto bb_7;
bb_4:
  /*$E8FD*/ CYCLES(0xe8fd, 7);
            tmp3_U16 = (uint8_t)(0x01 + s_x);
            tmp1_U16 = ram_peek(tmp3_U16) << 0x01;
            tmp2_U8 = (uint8_t)(tmp1_U16 >> 8);
            s_status_c = tmp2_U8;
            ram_poke(tmp3_U16, ((uint8_t)tmp1_U16));
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_6;
bb_5:
  /*$E901*/ CYCLES(0xe901, 4);
            tmp1_U16 = (uint8_t)(0x01 + s_x);
            ram_poke(tmp1_U16, (uint8_t)(ram_peek(tmp1_U16) + 0x01));
bb_6:
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
              goto bb_4;
bb_7:
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
              goto bb_9;
            else
              goto bb_8;
bb_1:
  /*$E8DC*/ CYCLES(0xe8dc, 35);
            tmp1_U8 = s_x;
  /*$E8DE*/ ram_poke(0x00ac, ram_peek((uint8_t)(0x04 + tmp1_U8)));
  /*$E8E2*/ ram_poke((uint8_t)(0x04 + tmp1_U8), ram_peek((uint8_t)(0x03 + tmp1_U8)));
  /*$E8E6*/ ram_poke((uint8_t)(0x03 + tmp1_U8), ram_peek((uint8_t)(0x02 + tmp1_U8)));
  /*$E8EA*/ ram_poke((uint8_t)(0x02 + tmp1_U8), ram_peek((uint8_t)(0x01 + tmp1_U8)));
  /*$E8EE*/ ram_poke((uint8_t)(0x01 + tmp1_U8), ram_peek(0x00a4));
  /*$E8F0*/ CYCLES(0xe8f0, 7);
            tmp2_U16 = (s_a + 0x0008) + s_status_c;
            s_status_c = (uint8_t)(tmp2_U16 >> 8);
            tmp1_U8 = (uint8_t)tmp2_U16;
            s_status_not_z = tmp1_U8;
            s_a = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_1;
bb_2:
  /*$E8F4*/ CYCLES(0xe8f4, 4);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_1;
bb_3:
  /*$E8F6*/ CYCLES(0xe8f6, 12);
            tmp2_U16 = (s_a - 0x0008) - (uint8_t)(0x01 - s_status_c);
  /*$E8F8*/ s_y = ((uint8_t)tmp2_U16);
  /*$E8F9*/ s_a = ram_peek(0x00ac);
            branchTarget = true;
            if ((uint8_t)(0x01 - ((uint8_t)(tmp2_U16 >> 8) & 0x01)))
              goto bb_7;
bb_4:
  /*$E8FD*/ CYCLES(0xe8fd, 7);
            tmp2_U16 = (uint8_t)(0x01 + s_x);
            tmp3_U16 = ram_peek(tmp2_U16) << 0x01;
            tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
            s_status_c = tmp1_U8;
            ram_poke(tmp2_U16, ((uint8_t)tmp3_U16));
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_6;
bb_5:
  /*$E901*/ CYCLES(0xe901, 4);
            tmp3_U16 = (uint8_t)(0x01 + s_x);
            ram_poke(tmp3_U16, (uint8_t)(ram_peek(tmp3_U16) + 0x01));
bb_6:
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
              goto bb_4;
bb_7:
  /*$E911*/ CYCLES(0xe911, 4);
            s_status_c = 0x00;
  /*$E912*/ if (ret_addr) pop16(); return;
bb_8:
  /*$E9B2*/ CYCLES(0xe9b2, 6);
  /*$E8DA*/ CYCLES(0xe8da, 4);
            s_x = 0x61;
            goto bb_1;
bb_9:
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
              goto bb_3;
bb_2:
  /*$E9BB*/ CYCLES(0xe9bb, 43);
  /*$E9BE*/ tmp2_U16 = ram_peek(0x0065) + ram_peek(0x00a9);
  /*$E9C0*/ ram_poke(0x0065, ((uint8_t)tmp2_U16));
  /*$E9C4*/ tmp2_U16 = (ram_peek(0x0064) + ram_peek(0x00a8)) + (uint8_t)(tmp2_U16 >> 8);
  /*$E9C6*/ ram_poke(0x0064, ((uint8_t)tmp2_U16));
  /*$E9CA*/ tmp2_U16 = (ram_peek(0x0063) + ram_peek(0x00a7)) + (uint8_t)(tmp2_U16 >> 8);
  /*$E9CC*/ ram_poke(0x0063, ((uint8_t)tmp2_U16));
  /*$E9D0*/ tmp3_U16 = ram_peek(0x0062);
            tmp4_U16 = ram_peek(0x00a6);
            tmp2_U16 = (tmp3_U16 + tmp4_U16) + (uint8_t)(tmp2_U16 >> 8);
            s_status_c = (uint8_t)(tmp2_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp2_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
  /*$E9D2*/ ram_poke(0x0062, ((uint8_t)tmp2_U16));
bb_3:
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
bb_4:
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


void FUNC_SIGN(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$EB82*/ CYCLES(0xeb82, 7);
            tmp1_U8 = ram_peek(0x009d);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_3;
bb_1:
  /*$EB86*/ CYCLES(0xeb86, 4);
  /*$EB88*/ CYCLES(0xeb88, 9);
            tmp1_U8 = (uint8_t)(((ram_peek(0x00a2) << 0x01) | s_status_c) >> 8);
            s_status_c = tmp1_U8;
  /*$EB89*/ s_status_not_z = 0xff;
            s_status_n = 0x80;
            s_a = 0xff;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_3;
bb_2:
  /*$EB8D*/ CYCLES(0xeb8d, 4);
            s_status_not_z = 0x01;
            s_status_n = 0x00;
            s_a = 0x01;
bb_3:
  /*$EB8F*/ CYCLES(0xeb8f, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_FCOMP(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$EBB2*/ CYCLES(0xebb2, 4);
            ram_poke(0x0060, s_a);
            FUNC_FCOMP2(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_FCOMP2(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;
  uint16_t tmp5_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$EBB4*/ CYCLES(0xebb4, 18);
            ram_poke(0x0061, s_y);
  /*$EBB8*/ tmp1_U8 = peek(ram_peek16al(0x0060));
  /*$EBBA*/ s_y = 0x01;
  /*$EBBB*/ s_x = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_15;
            else
              goto bb_5;
bb_1:
  /*$EB86*/ CYCLES(0xeb86, 4);
            s_a = ram_peek(0x00a2);
bb_2:
  /*$EB88*/ CYCLES(0xeb88, 9);
            tmp1_U8 = (uint8_t)(((s_a << 0x01) | s_status_c) >> 8);
            s_status_c = tmp1_U8;
  /*$EB89*/ s_status_not_z = 0xff;
            s_status_n = 0x80;
            s_a = 0xff;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_4;
bb_3:
  /*$EB8D*/ CYCLES(0xeb8d, 4);
            s_status_not_z = 0x01;
            s_status_n = 0x00;
            s_a = 0x01;
bb_4:
  /*$EB8F*/ CYCLES(0xeb8f, 2);
            if (ret_addr) pop16(); return;
bb_5:
  /*$EBBE*/ CYCLES(0xebbe, 11);
            tmp1_U8 = peek((ram_peek16al(0x0060) + s_y));
            branchTarget = true;
            if (((tmp1_U8 ^ ram_peek(0x00a2)) & 0x80))
              goto bb_1;
bb_6:
  /*$EBC4*/ CYCLES(0xebc4, 7);
            tmp1_U8 = s_x;
            tmp2_U8 = ram_peek(0x009d);
            s_status_c = (tmp1_U8 >= tmp2_U8);
            branchTarget = true;
            if ((tmp1_U8 != tmp2_U8))
              goto bb_11;
bb_7:
  /*$EBC8*/ CYCLES(0xebc8, 14);
            tmp2_U8 = peek((ram_peek16al(0x0060) + s_y));
  /*$EBCA*/ tmp2_U8 = tmp2_U8 | 0x80;
  /*$EBCC*/ tmp1_U8 = ram_peek(0x009e);
            s_status_c = (tmp2_U8 >= tmp1_U8);
            branchTarget = true;
            if ((tmp2_U8 != tmp1_U8))
              goto bb_11;
bb_8:
  /*$EBD0*/ CYCLES(0xebd0, 12);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$EBD1*/ tmp1_U8 = peek((ram_peek16al(0x0060) + tmp1_U8));
  /*$EBD3*/ tmp2_U8 = ram_peek(0x009f);
            s_status_c = (tmp1_U8 >= tmp2_U8);
            branchTarget = true;
            if ((tmp1_U8 != tmp2_U8))
              goto bb_11;
bb_9:
  /*$EBD7*/ CYCLES(0xebd7, 12);
            tmp2_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp2_U8;
  /*$EBD8*/ tmp2_U8 = peek((ram_peek16al(0x0060) + tmp2_U8));
  /*$EBDA*/ tmp1_U8 = ram_peek(0x00a0);
            s_status_c = (tmp2_U8 >= tmp1_U8);
            branchTarget = true;
            if ((tmp2_U8 != tmp1_U8))
              goto bb_11;
bb_10:
  /*$EBDE*/ CYCLES(0xebde, 19);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$EBE3*/ tmp1_U8 = peek((ram_peek16al(0x0060) + tmp1_U8));
  /*$EBE5*/ tmp3_U16 = tmp1_U8;
            tmp4_U16 = ram_peek(0x00a1);
            tmp5_U16 = (tmp3_U16 - tmp4_U16) - (0x7f < ram_peek(0x00ac));
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp4_U16));
            tmp1_U8 = (uint8_t)tmp5_U16;
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_14;
bb_11:
  /*$EBE9*/ CYCLES(0xebe9, 7);
            s_a = ram_peek(0x00a2);
            branchTarget = true;
            if (!s_status_c)
              goto bb_13;
bb_12:
  /*$EBED*/ CYCLES(0xebed, 4);
            s_a = (s_a ^ 0xff);
bb_13:
  /*$EBEF*/ CYCLES(0xebef, 6);
            branchTarget = true;
            goto bb_2;
bb_14:
  /*$EC11*/ CYCLES(0xec11, 2);
            if (ret_addr) pop16(); return;
bb_15:
  /*$EBBC*/ FUNC_SIGN(0x0000);
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
              goto bb_6;
bb_1:
  /*$EBF6*/ CYCLES(0xebf6, 12);
  /*$EBF7*/ s_a = (uint8_t)(s_a - 0x00a0);
  /*$EBF9*/ tmp1_U8 = ram_peek(0x00a2);
            s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
            branchTarget = true;
            if (!(tmp1_U8 & 0x80))
              goto bb_3;
bb_2:
  /*$EBFD*/ CYCLES(0xebfd, 14);
            tmp1_U8 = s_a;
  /*$EC00*/ ram_poke(0x00a4, 0xff);
  /*$EC02*/ FUNC_TCFACMANT(0xec04);
  /*$EC05*/ CYCLES(0xec05, 2);
            s_a = tmp1_U8;
bb_3:
  /*$EC06*/ CYCLES(0xec06, 11);
            s_x = 0x9d;
  /*$EC08*/ tmp1_U8 = s_a;
            s_status_c = (tmp1_U8 >= 0xf9);
            branchTarget = true;
            if (!((uint8_t)(tmp1_U8 - 0xf9) & 0x80))
              goto bb_5;
bb_4:
  /*$EC0C*/ CYCLES(0xec0c, 6);
            FUNC_SHFTRGHT(0xec0e);
  /*$EC0F*/ CYCLES(0xec0f, 4);
            ram_poke(0x00a4, s_y);
  /*$EC11*/ CYCLES(0xec11, 2);
            if (ret_addr) pop16(); return;
bb_5:
  /*$EC12*/ CYCLES(0xec12, 24);
            s_y = s_a;
  /*$EC17*/ tmp1_U8 = ram_peek(0x009e);
            s_status_c = (tmp1_U8 & 0x01);
            ram_poke(0x009e, (tmp1_U8 >> 0x01));
  /*$EC19*/ tmp1_U8 = (ram_peek(0x00a2) & 0x80) | ram_peek(0x009e);
            s_a = tmp1_U8;
  /*$EC1B*/ ram_poke(0x009e, tmp1_U8);
  /*$EC1D*/ FUNC_SHFTRGHT4(0xec1f);
  /*$EC20*/ CYCLES(0xec20, 6);
            ram_poke(0x00a4, s_y);
  /*$EC22*/ if (ret_addr) pop16(); return;
bb_6:
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


void FUNC_HPOSN(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;
  uint16_t tmp4_U16;
  uint16_t tmp5_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$F411*/ CYCLES(0xf411, 75);
            tmp1_U8 = s_a;
            ram_poke(0x00e2, tmp1_U8);
  /*$F413*/ tmp2_U8 = s_x;
            ram_poke(0x00e0, tmp2_U8);
  /*$F415*/ tmp3_U8 = s_y;
            ram_poke(0x00e1, tmp3_U8);
  /*$F417*/ push8(tmp1_U8);
  /*$F418*/ tmp1_U8 = tmp1_U8 & 0xc0;
  /*$F41A*/ ram_poke(0x0026, tmp1_U8);
  /*$F420*/ ram_poke(0x0026, ((tmp1_U8 >> 0x02) | ram_peek(0x0026)));
  /*$F422*/ tmp1_U8 = pop8();
  /*$F423*/ ram_poke(0x0027, tmp1_U8);
  /*$F425*/ tmp4_U16 = tmp1_U8;
  /*$F428*/ ram_poke(0x0027, (uint8_t)((ram_peek(0x0027) << 0x01) | (uint8_t)(((tmp4_U16 << 0x03) & 0x01ff) >> 8)));
  /*$F42B*/ ram_poke(0x0027, (uint8_t)((ram_peek(0x0027) << 0x01) | (uint8_t)(((tmp4_U16 << 0x04) & 0x01ff) >> 8)));
  /*$F42E*/ ram_poke(0x0026, (uint8_t)((ram_peek(0x0026) | (0x00 + ((uint8_t)(((tmp4_U16 << 0x05) & 0x01ff) >> 8) << 8))) >> 0x01));
  /*$F436*/ ram_poke(0x0027, ((ram_peek(0x0027) & 0x1f) | ram_peek(0x00e6)));
  /*$F438*/ s_a = tmp2_U8;
  /*$F439*/ s_status_c = (tmp3_U8 >= 0x00);
            branchTarget = true;
            if (!(tmp3_U8 != 0x00))
              goto bb_3;
bb_1:
  /*$F43D*/ CYCLES(0xf43d, 7);
            s_y = 0x23;
  /*$F43F*/ tmp4_U16 = (s_a + 0x0004) + s_status_c;
            s_status_c = (uint8_t)(tmp4_U16 >> 8);
            s_a = ((uint8_t)tmp4_U16);
bb_2:
  /*$F441*/ CYCLES(0xf441, 2);
            s_y = (uint8_t)(s_y + 0x01);
bb_3:
  /*$F442*/ CYCLES(0xf442, 7);
            tmp4_U16 = s_a;
            tmp5_U16 = (tmp4_U16 - 0x0007) - (uint8_t)(0x01 - s_status_c);
            tmp3_U8 = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
            s_status_c = tmp3_U8;
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0xfff8);
            s_a = ((uint8_t)tmp5_U16);
            branchTarget = true;
            if (tmp3_U8)
              goto bb_2;
bb_4:
  /*$F446*/ CYCLES(0xf446, 28);
            tmp3_U8 = s_y;
            ram_poke(0x00e5, tmp3_U8);
  /*$F448*/ tmp2_U8 = s_a;
            s_x = tmp2_U8;
  /*$F44C*/ ram_poke(0x0030, ram_peek((0xf4b9 + tmp2_U8)));
  /*$F44F*/ tmp3_U8 = tmp3_U8 & 0x01;
            s_status_c = tmp3_U8;
  /*$F450*/ tmp2_U8 = ram_peek(0x00e4);
            s_a = tmp2_U8;
  /*$F452*/ ram_poke(0x001c, tmp2_U8);
            branchTarget = true;
            if (tmp3_U8)
              goto bb_6;
bb_5:
  /*$F456*/ CYCLES(0xf456, 2);
            if (ret_addr) pop16(); return;
bb_6:
  /*$F454*/ FUNC_COLORSHIFT(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_HPLOT0(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$F457*/ CYCLES(0xf457, 6);
            FUNC_HPOSN(0xf459);
  /*$F45A*/ CYCLES(0xf45a, 19);
  /*$F45C*/ tmp1_U8 = s_y;
            tmp2_U8 = peek((ram_peek16al(0x0026) + tmp1_U8));
  /*$F460*/ tmp3_U8 = peek((ram_peek16al(0x0026) + tmp1_U8));
  /*$F462*/ poke((ram_peek16al(0x0026) + tmp1_U8), (((ram_peek(0x001c) ^ tmp2_U8) & ram_peek(0x0030)) ^ tmp3_U8));
  /*$F464*/ if (ret_addr) pop16(); return;
}


void FUNC_COLORSHIFT(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$F47E*/ CYCLES(0xf47e, 9);
            tmp1_U8 = (uint8_t)(s_a << 0x01);
            s_a = tmp1_U8;
  /*$F47F*/ s_status_not_z = (tmp1_U8 != 0xc0);
            s_status_c = (tmp1_U8 >= 0xc0);
            tmp1_U8 = (uint8_t)(tmp1_U8 - 0xc0) & 0x80;
            s_status_n = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_2;
bb_1:
  /*$F483*/ CYCLES(0xf483, 11);
  /*$F485*/ tmp1_U8 = ram_peek(0x001c) ^ 0x7f;
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
  /*$F487*/ ram_poke(0x001c, tmp1_U8);
bb_2:
  /*$F489*/ CYCLES(0xf489, 2);
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
  /*$F80E*/ CYCLES(0xf80e, 19);
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


void FUNC_GBASCALC(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$F847*/ CYCLES(0xf847, 23);
            tmp1_U8 = s_a;
            push8(tmp1_U8);
  /*$F848*/ tmp2_U8 = tmp1_U8 & 0x01;
            s_status_c = tmp2_U8;
  /*$F84D*/ ram_poke(0x0027, (((tmp1_U8 >> 0x01) & 0x03) | 0x04));
  /*$F84F*/ tmp1_U8 = pop8();
  /*$F850*/ s_a = (tmp1_U8 & 0x18);
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_2;
bb_1:
  /*$F854*/ CYCLES(0xf854, 4);
            s_a = (uint8_t)((s_a + 0x007f) + s_status_c);
bb_2:
  /*$F856*/ CYCLES(0xf856, 16);
            tmp2_U8 = s_a;
            ram_poke(0x0026, tmp2_U8);
  /*$F85C*/ ram_poke(0x0026, ((uint8_t)(tmp2_U8 << 0x02) | ram_peek(0x0026)));
  /*$F85E*/ if (ret_addr) pop16(); return;
}


void FUNC_SCRN2(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$F879*/ CYCLES(0xf879, 4);
            branchTarget = true;
            if (!s_status_c)
              goto bb_2;
bb_1:
  /*$F87B*/ CYCLES(0xf87b, 7);
            tmp1_U8 = s_a;
  /*$F87E*/ s_status_c = ((tmp1_U8 >> 0x03) & 0x01);
            s_a = (tmp1_U8 >> 0x04);
bb_2:
  /*$F87F*/ CYCLES(0xf87f, 6);
            tmp1_U8 = s_a & 0x0f;
            s_status_not_z = tmp1_U8;
            s_status_n = 0x00;
            s_a = tmp1_U8;
  /*$F881*/ if (ret_addr) pop16(); return;
}


void FUNC_PCADJ(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$F953*/ CYCLES(0xf953, 6);
            s_status_c = 0x01;
  /*$F954*/ s_a = ram_peek(0x002f);
            FUNC_PCADJ3(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_PCADJ3(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;
  uint8_t tmp5_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$F956*/ CYCLES(0xf956, 9);
            s_y = ram_peek(0x003b);
  /*$F958*/ tmp1_U8 = s_a;
            s_x = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 & 0x80))
              goto bb_2;
bb_1:
  /*$F95B*/ CYCLES(0xf95b, 2);
            s_y = (uint8_t)(s_y - 0x01);
bb_2:
  /*$F95C*/ CYCLES(0xf95c, 7);
            tmp2_U16 = s_a;
            tmp3_U16 = ram_peek(0x003a);
            tmp4_U16 = (tmp2_U16 + tmp3_U16) + s_status_c;
            tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
            s_status_c = tmp1_U8;
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp2_U16, (uint8_t)tmp3_U16);
            tmp5_U8 = (uint8_t)tmp4_U16;
            s_status_not_z = tmp5_U8;
            s_status_n = (tmp5_U8 & 0x80);
            s_a = tmp5_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_4;
bb_3:
  /*$F960*/ CYCLES(0xf960, 2);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_y = tmp1_U8;
bb_4:
  /*$F961*/ CYCLES(0xf961, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_PREAD(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FB1E*/ CYCLES(0xfb1e, 12);
            tmp1_U8 = io_peek(0xc070);
  /*$FB21*/ s_y = 0x00;
bb_1:
  /*$FB25*/ CYCLES(0xfb25, 9);
            tmp1_U8 = peek((0xc064 + s_x));
            branchTarget = true;
            if (!(tmp1_U8 & 0x80))
              goto bb_4;
bb_2:
  /*$FB2A*/ CYCLES(0xfb2a, 6);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_1;
bb_3:
  /*$FB2D*/ CYCLES(0xfb2d, 2);
            s_y = (uint8_t)(s_y - 0x01);
bb_4:
  /*$FB2E*/ CYCLES(0xfb2e, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_INIT(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FB2F*/ CYCLES(0xfb2f, 18);
  /*$FB31*/ ram_poke(0x0048, 0x00);
  /*$FB33*/ tmp1_U8 = io_peek(0xc056);
  /*$FB36*/ tmp1_U8 = io_peek(0xc054);
  /*$FB39*/ CYCLES(0xfb39, 12);
            tmp1_U8 = io_peek(0xc051);
  /*$FB4B*/ CYCLES(0xfb4b, 28);
            ram_poke(0x0022, 0x00);
  /*$FB4F*/ ram_poke(0x0020, 0x00);
  /*$FB53*/ ram_poke(0x0021, 0x28);
  /*$FB57*/ ram_poke(0x0023, 0x18);
  /*$FB5B*/ CYCLES(0xfb5b, 9);
            ram_poke(0x0025, 0x17);
  /*$FC22*/ CYCLES(0xfc22, 4);
            s_a = ram_peek(0x0025);
            FUNC_VTABZ(0x0000);
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
              goto bb_2;
bb_1:
  /*$FBCE*/ CYCLES(0xfbce, 4);
            tmp3_U16 = s_a;
            tmp4_U16 = (tmp3_U16 + 0x007f) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x007f);
            s_a = ((uint8_t)tmp4_U16);
bb_2:
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
            FUNC_BASCALC(0xfc26);
  /*$FC27*/ CYCLES(0xfc27, 7);
            tmp1_U16 = s_a;
            tmp2_U16 = ram_peek(0x0020);
            tmp3_U16 = (tmp1_U16 + tmp2_U16) + s_status_c;
            s_status_c = (uint8_t)(tmp3_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp1_U16, (uint8_t)tmp2_U16);
            tmp4_U8 = (uint8_t)tmp3_U16;
            s_status_not_z = tmp4_U8;
            s_status_n = (tmp4_U8 & 0x80);
            s_a = tmp4_U8;
  /*$FC29*/ ram_poke(0x0028, tmp4_U8);
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


void FUNC_NXTA1(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;
  uint8_t tmp4_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FCBA*/ CYCLES(0xfcba, 21);
  /*$FCC0*/ tmp1_U16 = ram_peek(0x003d);
            tmp2_U16 = ram_peek(0x003f);
            tmp3_U16 = (tmp1_U16 - tmp2_U16) - (ram_peek(0x003c) < ram_peek(0x003e));
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp1_U16, (uint8_t)(~tmp2_U16));
            s_a = ((uint8_t)tmp3_U16);
  /*$FCC2*/ tmp4_U8 = (uint8_t)(ram_peek(0x003c) + 0x01);
            s_status_not_z = tmp4_U8;
            s_status_n = (tmp4_U8 & 0x80);
            ram_poke(0x003c, tmp4_U8);
            branchTarget = true;
            if (tmp4_U8)
              goto bb_2;
bb_1:
  /*$FCC6*/ CYCLES(0xfcc6, 4);
            tmp4_U8 = (uint8_t)(ram_peek(0x003d) + 0x01);
            s_status_not_z = tmp4_U8;
            s_status_n = (tmp4_U8 & 0x80);
            ram_poke(0x003d, tmp4_U8);
bb_2:
  /*$FCC8*/ CYCLES(0xfcc8, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_HEADER(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint16_t tmp2_U16;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FCC9*/ CYCLES(0xfcc9, 9);
            s_y = 0x4b;
  /*$FCCB*/ FUNC_ZERODLY(0xfccd);
  /*$FCCE*/ CYCLES(0xfcce, 4);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_0;
bb_1:
  /*$FCD0*/ CYCLES(0xfcd0, 7);
            tmp1_U16 = s_a;
            tmp2_U16 = (tmp1_U16 + 0x00fe) + s_status_c;
            tmp3_U8 = (uint8_t)(tmp2_U16 >> 8);
            s_status_c = tmp3_U8;
            s_status_v = ovf8((uint8_t)tmp2_U16, (uint8_t)tmp1_U16, (uint8_t)0x00fe);
            s_a = ((uint8_t)tmp2_U16);
            branchTarget = true;
            if (tmp3_U8)
              goto bb_0;
bb_2:
  /*$FCD4*/ CYCLES(0xfcd4, 9);
            s_y = 0x21;
  /*$FCD6*/ FUNC_ZERODLY(0xfcd8);
  /*$FCD9*/ CYCLES(0xfcd9, 4);
  /*$FCDA*/ s_y = (uint8_t)(s_y + 0x02);
            FUNC_ZERODLY(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_ZERODLY(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FCDB*/ CYCLES(0xfcdb, 6);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_0;
bb_1:
  /*$FCDE*/ CYCLES(0xfcde, 4);
            branchTarget = true;
            if (!s_status_c)
              goto bb_4;
bb_2:
  /*$FCE0*/ CYCLES(0xfce0, 4);
            s_y = 0x32;
bb_3:
  /*$FCE2*/ CYCLES(0xfce2, 6);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_3;
bb_4:
  /*$FCE5*/ CYCLES(0xfce5, 12);
            tmp1_U8 = io_peek(0xc020);
  /*$FCE8*/ s_y = 0x2c;
  /*$FCEA*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_x = tmp1_U8;
  /*$FCEB*/ if (ret_addr) pop16(); return;
}


void FUNC_RD2BIT(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FCFA*/ CYCLES(0xfcfa, 6);
            FUNC_RDBIT(0xfcfc);
            FUNC_RDBIT(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_RDBIT(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FCFD*/ CYCLES(0xfcfd, 14);
            s_y = (uint8_t)(s_y - 0x01);
  /*$FCFE*/ tmp1_U8 = io_peek(0xc060);
  /*$FD01*/ tmp1_U8 = tmp1_U8 ^ ram_peek(0x002f);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 & 0x80))
              goto bb_0;
bb_1:
  /*$FD05*/ CYCLES(0xfd05, 12);
            tmp1_U8 = s_a ^ ram_peek(0x002f);
            s_a = tmp1_U8;
  /*$FD07*/ ram_poke(0x002f, tmp1_U8);
  /*$FD09*/ tmp1_U8 = s_y;
            s_status_not_z = (tmp1_U8 != 0x80);
            s_status_c = (tmp1_U8 >= 0x80);
            s_status_n = ((uint8_t)(tmp1_U8 - 0x80) & 0x80);
  /*$FD0B*/ if (ret_addr) pop16(); return;
}


void FUNC_A1PC(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FE75*/ CYCLES(0xfe75, 6);
            branchTarget = true;
            if (!s_x)
              goto bb_2;
bb_1:
  /*$FE78*/ CYCLES(0xfe78, 12);
            tmp1_U8 = s_x;
  /*$FE7A*/ ram_poke((uint8_t)(0x3a + tmp1_U8), ram_peek((uint8_t)(0x3c + tmp1_U8)));
  /*$FE7C*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 & 0x80))
              goto bb_1;
bb_2:
  /*$FE7F*/ CYCLES(0xfe7f, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_SETNORM(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FE84*/ CYCLES(0xfe84, 9);
  /*$FE86*/ ram_poke(0x0032, 0xff);
  /*$FE88*/ if (ret_addr) pop16(); return;
}


void FUNC_SETKBD(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FE89*/ CYCLES(0xfe89, 18);
  /*$FE8B*/ ram_poke(0x003e, 0x00);
  /*$FE8D*/ s_x = 0x38;
  /*$FE8F*/ s_y = 0x1b;
  /*$FE9B*/ CYCLES(0xfe9b, 11);
  /*$FE9D*/ tmp1_U8 = ram_peek(0x003e) & 0x0f;
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_2;
bb_1:
  /*$FEA1*/ CYCLES(0xfea1, 11);
            s_a = (s_a | 0xc0);
  /*$FEA3*/ s_y = 0x00;
            branchTarget = true;
            goto bb_3;
bb_2:
  /*$FEA7*/ CYCLES(0xfea7, 4);
            s_a = 0xfd;
bb_3:
  /*$FEA9*/ CYCLES(0xfea9, 9);
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
  /*$FE93*/ CYCLES(0xfe93, 14);
  /*$FE95*/ ram_poke(0x003e, 0x00);
  /*$FE97*/ s_x = 0x36;
  /*$FE99*/ s_y = 0xf0;
  /*$FE9B*/ CYCLES(0xfe9b, 11);
  /*$FE9D*/ tmp1_U8 = ram_peek(0x003e) & 0x0f;
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_2;
bb_1:
  /*$FEA1*/ CYCLES(0xfea1, 11);
            s_a = (s_a | 0xc0);
  /*$FEA3*/ s_y = 0x00;
            branchTarget = true;
            goto bb_3;
bb_2:
  /*$FEA7*/ CYCLES(0xfea7, 4);
            s_a = 0xfd;
bb_3:
  /*$FEA9*/ CYCLES(0xfea9, 9);
            tmp1_U8 = s_x;
            ram_poke(tmp1_U8, s_y);
  /*$FEAB*/ ram_poke((uint8_t)(0x01 + tmp1_U8), s_a);
  /*$FEAD*/ if (ret_addr) pop16(); return;
}


void FUNC_MON_RESTORE(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FF3F*/ CYCLES(0xff3f, 19);
  /*$FF41*/ push8(ram_peek(0x0048));
  /*$FF42*/ s_a = ram_peek(0x0045);
  /*$FF44*/ s_x = ram_peek(0x0046);
  /*$FF46*/ s_y = ram_peek(0x0047);
  /*$FF48*/ tmp1_U8 = pop8();
            s_status_c = (tmp1_U8 & 0x01);
            s_status_not_z = (~tmp1_U8 & 2);
            s_status_i = ((tmp1_U8 & 0x04) != 0);
            s_status_d = ((tmp1_U8 & 0x08) != 0);
            s_status_b = 0x00;
            s_status_v = ((tmp1_U8 & 0x40) != 0);
            s_status_n = (tmp1_U8 & 0x80);
  /*$FF49*/ if (ret_addr) pop16(); return;
}


void FUNC_GETNUM(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;
  uint8_t tmp4_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FFA7*/ CYCLES(0xffa7, 11);
            s_x = 0x00;
  /*$FFA9*/ ram_poke(0x003e, 0x00);
  /*$FFAB*/ ram_poke(0x003f, 0x00);
            goto bb_7;
bb_1:
  /*$FF8A*/ CYCLES(0xff8a, 11);
            s_x = 0x03;
  /*$FF8F*/ s_a = (uint8_t)(s_a << 0x04);
bb_2:
  /*$FF90*/ CYCLES(0xff90, 14);
            tmp3_U16 = s_a << 0x01;
            s_a = ((uint8_t)tmp3_U16);
  /*$FF91*/ tmp3_U16 = (ram_peek(0x003e) << 0x01) | (uint8_t)(tmp3_U16 >> 8);
            ram_poke(0x003e, ((uint8_t)tmp3_U16));
  /*$FF93*/ ram_poke(0x003f, (uint8_t)((ram_peek(0x003f) << 0x01) | (uint8_t)(tmp3_U16 >> 8)));
  /*$FF95*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 & 0x80))
              goto bb_2;
bb_3:
  /*$FF98*/ CYCLES(0xff98, 7);
            branchTarget = true;
            if (ram_peek(0x0031))
              goto bb_5;
bb_4:
  /*$FF9C*/ CYCLES(0xff9c, 11);
            tmp1_U8 = s_x;
            tmp4_U8 = ram_peek((uint8_t)(0x3f + tmp1_U8));
  /*$FF9E*/ ram_poke((uint8_t)(0x3d + tmp1_U8), tmp4_U8);
  /*$FFA0*/ ram_poke((uint8_t)(0x41 + tmp1_U8), tmp4_U8);
bb_5:
  /*$FFA2*/ CYCLES(0xffa2, 6);
            tmp1_U8 = (uint8_t)(s_x + 0x01);
            s_status_not_z = tmp1_U8;
            s_x = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_3;
bb_6:
  /*$FFA5*/ CYCLES(0xffa5, 4);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_0;
bb_7:
  /*$FFAD*/ CYCLES(0xffad, 18);
            tmp1_U8 = s_y;
  /*$FFB0*/ s_y = (uint8_t)(tmp1_U8 + 0x01);
  /*$FFB1*/ tmp1_U8 = ram_peek((0x0200 + tmp1_U8)) ^ 0xb0;
            s_a = tmp1_U8;
  /*$FFB3*/ tmp1_U8 = tmp1_U8 >= 0x0a;
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_1;
bb_8:
  /*$FFB7*/ CYCLES(0xffb7, 11);
            tmp2_U16 = s_a;
            tmp3_U16 = (tmp2_U16 + 0x0088) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0x0088);
            tmp1_U8 = (uint8_t)tmp3_U16;
            s_a = tmp1_U8;
  /*$FFB9*/ tmp1_U8 = tmp1_U8 >= 0xfa;
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_1;
bb_9:
  /*$FFBD*/ CYCLES(0xffbd, 2);
            if (ret_addr) pop16(); return;
}


void FUNC_ZMODE(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FFC7*/ CYCLES(0xffc7, 9);
            s_status_not_z = 0x00;
            s_status_n = 0x00;
            s_y = 0x00;
  /*$FFC9*/ ram_poke(0x0031, 0x00);
  /*$FFCB*/ if (ret_addr) pop16(); return;
}


static unsigned find_block_id_func_t001(uint16_t from_pc, uint16_t addr);

void func_t001(uint16_t ret_addr) {
  unsigned block_id = 0;
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;
  uint16_t tmp5_U16;
  uint8_t tmp6_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  for(;;) {
    switch (block_id) {
    case 0:
                s_sp = 0xff;
                s_status_b = 0x00;
                s_status_i = 0x00;
      /*$FA62*/ CYCLES(0xfa62, 7);
      /*$FA63*/ FUNC_SETNORM(0xfa65);
                branchTarget = true; block_id = 1425;
      break;
    case 1:  // $0090
      /*$0090*/ CYCLES(0x0090, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x0090, ram_peek16(0x0091));
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
      /*$00BC*/ branchTarget = true; block_id = tmp2_U8 ? 7 : 5;
      break;
    case 5:  // $00BE
      /*$00BE*/ CYCLES(0x00be, 7);
      /*$00C0*/ branchTarget = true; block_id = !(s_a != 0x20) ? 2 : 6;
      break;
    case 6:  // $00C2
      /*$00C2*/ CYCLES(0x00c2, 11);
      /*$00C3*/ tmp3_U16 = s_a;
      /*$00C6*/ tmp4_U16 = (tmp3_U16 - 0x0030) & 0x00ff;
                tmp5_U16 = tmp4_U16 - 0x00d0;
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0xff2f);
                tmp2_U8 = (uint8_t)(tmp3_U16 - 0x0100);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 7;
      break;
    case 7:  // $00C8
      /*$00C8*/ CYCLES(0x00c8, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0x00c8, pop16() + 1);;
      break;
    case 8:  // $00CD
      /*$00CD*/ CYCLES(0x00cd, 2);
                fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x00cd, 0x00cd, 0x03); error_handler(0x00cd);
      break;
    case 9:  // $D365
      /*$D365*/ CYCLES(0xd365, 9);
      /*$D369*/ s_x = (uint8_t)(s_sp + 0x04);
                block_id = 10;
      break;
    case 10:  // $D36A
      /*$D36A*/ CYCLES(0xd36a, 12);
                tmp2_U8 = ram_peek((0x0101 + s_x));
                s_a = tmp2_U8;
      /*$D36D*/ tmp1_U8 = tmp2_U8 != 0x81;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp2_U8 >= 0x81);
                s_status_n = ((uint8_t)(tmp2_U8 - 0x81) & 0x80);
      /*$D36F*/ branchTarget = true; block_id = tmp1_U8 ? 16 : 11;
      break;
    case 11:  // $D371
      /*$D371*/ CYCLES(0xd371, 7);
                tmp1_U8 = ram_peek(0x0086);
                s_a = tmp1_U8;
      /*$D373*/ branchTarget = true; block_id = tmp1_U8 ? 13 : 12;
      break;
    case 12:  // $D375
      /*$D375*/ CYCLES(0xd375, 18);
                tmp1_U8 = s_x;
      /*$D378*/ ram_poke(0x0085, ram_peek((0x0102 + tmp1_U8)));
      /*$D37A*/ tmp1_U8 = ram_peek((0x0103 + tmp1_U8));
                s_a = tmp1_U8;
      /*$D37D*/ ram_poke(0x0086, tmp1_U8);
                block_id = 13;
      break;
    case 13:  // $D37F
      /*$D37F*/ CYCLES(0xd37f, 9);
      /*$D382*/ branchTarget = true; block_id = (s_a != ram_peek((0x0103 + s_x))) ? 15 : 14;
      break;
    case 14:  // $D384
      /*$D384*/ CYCLES(0xd384, 12);
                tmp1_U8 = ram_peek(0x0085);
                s_a = tmp1_U8;
      /*$D386*/ tmp2_U8 = ram_peek((0x0102 + s_x));
                tmp6_U8 = tmp1_U8 != tmp2_U8;
                s_status_not_z = tmp6_U8;
                s_status_c = (tmp1_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
      /*$D389*/ branchTarget = true; block_id = !tmp6_U8 ? 16 : 15;
      break;
    case 15:  // $D38B
      /*$D38B*/ CYCLES(0xd38b, 12);
      /*$D38D*/ tmp4_U16 = s_x;
                tmp3_U16 = tmp4_U16 + 0x0012;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0x0012);
                tmp1_U8 = (uint8_t)tmp3_U16;
                s_a = tmp1_U8;
      /*$D38F*/ s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$D390*/ branchTarget = true; block_id = tmp1_U8 ? 10 : 16;
      break;
    case 16:  // $D392
      /*$D392*/ CYCLES(0xd392, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd392, pop16() + 1);;
      break;
    case 17:  // $D393
      /*$D393*/ CYCLES(0xd393, 6);
                branchTarget = true; push16(0xd395); block_id = 32;
      break;
    case 18:  // $D396
      /*$D396*/ CYCLES(0xd396, 7);
                ram_poke(0x006d, s_a);
      /*$D398*/ ram_poke(0x006e, s_y);
                FUNC_MVBLKUP2(0x0000);
                block_id = find_block_id_func_t001(0xd398, pop16() + 1);;
      break;
    case 19:  // $D39A
      /*$D39A*/ CYCLES(0xd39a, 29);
      /*$D39D*/ tmp3_U16 = ram_peek(0x0096) - ram_peek(0x009b);
                tmp1_U8 = (uint8_t)tmp3_U16;
      /*$D39F*/ ram_poke(0x005e, tmp1_U8);
      /*$D3A1*/ s_y = tmp1_U8;
      /*$D3A4*/ tmp4_U16 = ram_peek(0x0097);
                tmp5_U16 = ram_peek(0x009c);
                tmp3_U16 = (tmp4_U16 - tmp5_U16) - ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)(~tmp5_U16));
      /*$D3A7*/ s_x = (uint8_t)(((uint8_t)tmp3_U16) + 0x01);
      /*$D3A8*/ s_a = tmp1_U8;
      /*$D3A9*/ branchTarget = true; block_id = !tmp1_U8 ? 27 : 20;
      break;
    case 20:  // $D3AB
      /*$D3AB*/ CYCLES(0xd3ab, 16);
      /*$D3AE*/ tmp3_U16 = ram_peek(0x0096) - ram_peek(0x005e);
                tmp1_U8 = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_c = tmp1_U8;
      /*$D3B0*/ ram_poke(0x0096, ((uint8_t)tmp3_U16));
      /*$D3B2*/ branchTarget = true; block_id = tmp1_U8 ? 22 : 21;
      break;
    case 21:  // $D3B4
      /*$D3B4*/ CYCLES(0xd3b4, 6);
                ram_poke(0x0097, (uint8_t)(ram_peek(0x0097) - 0x01));
      /*$D3B6*/ s_status_c = 0x01;
                block_id = 22;
      break;
    case 22:  // $D3B7
      /*$D3B7*/ CYCLES(0xd3b7, 14);
      /*$D3B9*/ tmp3_U16 = ram_peek(0x0094);
                tmp5_U16 = ram_peek(0x005e);
                tmp4_U16 = (tmp3_U16 - tmp5_U16) - (uint8_t)(0x01 - s_status_c);
                tmp1_U8 = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_c = tmp1_U8;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp5_U16));
      /*$D3BB*/ ram_poke(0x0094, ((uint8_t)tmp4_U16));
      /*$D3BD*/ branchTarget = true; block_id = tmp1_U8 ? 25 : 23;
      break;
    case 23:  // $D3BF
      /*$D3BF*/ CYCLES(0xd3bf, 7);
                ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) - 0x01));
      /*$D3C1*/ branchTarget = true; block_id = !s_status_c ? 25 : 24;
      break;
    case 24:  // $D3C3
      /*$D3C3*/ CYCLES(0xd3c3, 7);
                tmp1_U8 = s_y;
                tmp2_U8 = peek((ram_peek16al(0x0096) + tmp1_U8));
      /*$D3C5*/ poke((ram_peek16al(0x0094) + tmp1_U8), tmp2_U8);
                block_id = 25;
      break;
    case 25:  // $D3C7
      /*$D3C7*/ CYCLES(0xd3c7, 6);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp1_U8;
      /*$D3C8*/ branchTarget = true; block_id = tmp1_U8 ? 24 : 26;
      break;
    case 26:  // $D3CA
      /*$D3CA*/ CYCLES(0xd3ca, 7);
                tmp1_U8 = s_y;
                tmp2_U8 = peek((ram_peek16al(0x0096) + tmp1_U8));
                s_a = tmp2_U8;
      /*$D3CC*/ poke((ram_peek16al(0x0094) + tmp1_U8), tmp2_U8);
                block_id = 27;
      break;
    case 27:  // $D3CE
      /*$D3CE*/ CYCLES(0xd3ce, 12);
                ram_poke(0x0097, (uint8_t)(ram_peek(0x0097) - 0x01));
      /*$D3D0*/ ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) - 0x01));
      /*$D3D2*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$D3D3*/ branchTarget = true; block_id = tmp1_U8 ? 25 : 28;
      break;
    case 28:  // $D3D5
      /*$D3D5*/ CYCLES(0xd3d5, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd3d5, pop16() + 1);;
      break;
    case 29:  // $D3D6
      /*$D3D6*/ CYCLES(0xd3d6, 9);
                tmp4_U16 = s_a << 0x01;
      /*$D3D7*/ tmp5_U16 = tmp4_U16 & 0x00ff;
                tmp4_U16 = (tmp5_U16 + 0x0036) + (uint8_t)(tmp4_U16 >> 8);
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = tmp1_U8;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x0036);
                s_a = ((uint8_t)tmp4_U16);
      /*$D3D9*/ branchTarget = true; block_id = tmp1_U8 ? 43 : 30;
      break;
    case 30:  // $D3DB
      /*$D3DB*/ CYCLES(0xd3db, 12);
                ram_poke(0x005e, s_a);
      /*$D3DD*/ tmp1_U8 = s_sp;
                s_x = tmp1_U8;
      /*$D3DE*/ tmp2_U8 = ram_peek(0x005e);
                s_status_not_z = (tmp1_U8 != tmp2_U8);
                tmp6_U8 = tmp1_U8 >= tmp2_U8;
                s_status_c = tmp6_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
      /*$D3E0*/ branchTarget = true; block_id = !tmp6_U8 ? 43 : 31;
      break;
    case 31:  // $D3E2
      /*$D3E2*/ CYCLES(0xd3e2, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd3e2, pop16() + 1);;
      break;
    case 32:  // $D3E3
      /*$D3E3*/ CYCLES(0xd3e3, 7);
                tmp6_U8 = s_y;
                tmp2_U8 = ram_peek(0x0070);
                s_status_not_z = (tmp6_U8 != tmp2_U8);
                tmp1_U8 = tmp6_U8 >= tmp2_U8;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp6_U8 - tmp2_U8) & 0x80);
      /*$D3E5*/ branchTarget = true; block_id = !tmp1_U8 ? 42 : 33;
      break;
    case 33:  // $D3E7
      /*$D3E7*/ CYCLES(0xd3e7, 4);
                branchTarget = true; block_id = s_status_not_z ? 35 : 34;
      break;
    case 34:  // $D3E9
      /*$D3E9*/ CYCLES(0xd3e9, 7);
                tmp6_U8 = s_a;
                tmp2_U8 = ram_peek(0x006f);
                s_status_not_z = (tmp6_U8 != tmp2_U8);
                tmp1_U8 = tmp6_U8 >= tmp2_U8;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp6_U8 - tmp2_U8) & 0x80);
      /*$D3EB*/ branchTarget = true; block_id = !tmp1_U8 ? 42 : 35;
      break;
    case 35:  // $D3ED
      /*$D3ED*/ CYCLES(0xd3ed, 7);
                push8(s_a);
      /*$D3EE*/ s_x = 0x09;
      /*$D3F0*/ s_a = s_y;
                block_id = 36;
      break;
    case 36:  // $D3F1
      /*$D3F1*/ CYCLES(0xd3f1, 11);
                push8(s_a);
      /*$D3F2*/ tmp1_U8 = s_x;
                s_a = ram_peek((uint8_t)(0x93 + tmp1_U8));
      /*$D3F4*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_x = tmp1_U8;
      /*$D3F5*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 36 : 37;
      break;
    case 37:  // $D3F7
      /*$D3F7*/ CYCLES(0xd3f7, 6);
                FUNC_GARBAGE(0xd3f9);
      /*$D3FA*/ CYCLES(0xd3fa, 4);
                s_x = 0xf7;
                block_id = 38;
      break;
    case 38:  // $D3FC
      /*$D3FC*/ CYCLES(0xd3fc, 11);
                tmp1_U8 = pop8();
      /*$D3FD*/ tmp2_U8 = s_x;
                ram_poke((uint8_t)(0x9d + tmp2_U8), tmp1_U8);
      /*$D3FF*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_x = tmp2_U8;
      /*$D400*/ branchTarget = true; block_id = (tmp2_U8 & 0x80) ? 38 : 39;
      break;
    case 39:  // $D402
      /*$D402*/ CYCLES(0xd402, 12);
                tmp2_U8 = pop8();
      /*$D403*/ s_y = tmp2_U8;
      /*$D404*/ tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$D405*/ tmp1_U8 = ram_peek(0x0070);
                s_status_not_z = (tmp2_U8 != tmp1_U8);
                tmp6_U8 = tmp2_U8 >= tmp1_U8;
                s_status_c = tmp6_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
      /*$D407*/ branchTarget = true; block_id = !tmp6_U8 ? 42 : 40;
      break;
    case 40:  // $D409
      /*$D409*/ CYCLES(0xd409, 4);
                branchTarget = true; block_id = s_status_not_z ? 43 : 41;
      break;
    case 41:  // $D40B
      /*$D40B*/ CYCLES(0xd40b, 7);
                tmp6_U8 = s_a;
                tmp1_U8 = ram_peek(0x006f);
                s_status_not_z = (tmp6_U8 != tmp1_U8);
                tmp2_U8 = tmp6_U8 >= tmp1_U8;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp6_U8 - tmp1_U8) & 0x80);
      /*$D40D*/ branchTarget = true; block_id = tmp2_U8 ? 43 : 42;
      break;
    case 42:  // $D40F
      /*$D40F*/ CYCLES(0xd40f, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd40f, pop16() + 1);;
      break;
    case 43:  // $D410
      /*$D410*/ CYCLES(0xd410, 4);
                s_x = 0x4d;
                block_id = 44;
      break;
    case 44:  // $D412
      /*$D412*/ CYCLES(0xd412, 7);
                tmp2_U8 = ram_peek(0x00d8);
                s_status_v = ((tmp2_U8 >> 0x06) & 0x01);
      /*$D414*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 46 : 45;
      break;
    case 45:  // $D416
      /*$D416*/ CYCLES(0xd416, 6);
                branchTarget = true; block_id = 1192;
      break;
    case 46:  // $D419
      /*$D419*/ CYCLES(0xd419, 6);
                branchTarget = true; push16(0xd41b); block_id = 361;
      break;
    case 47:  // $D41C
      /*$D41C*/ CYCLES(0xd41c, 6);
                branchTarget = true; push16(0xd41e); block_id = 389;
      break;
    case 48:  // $D41F
      /*$D41F*/ CYCLES(0xd41f, 12);
                tmp2_U8 = ram_peek((0xd260 + s_x));
                s_a = tmp2_U8;
      /*$D422*/ push8(tmp2_U8);
      /*$D423*/ branchTarget = true; push16(0xd425); block_id = 390;
      break;
    case 49:  // $D426
      /*$D426*/ CYCLES(0xd426, 7);
                s_x = (uint8_t)(s_x + 0x01);
      /*$D427*/ tmp2_U8 = pop8();
      /*$D428*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 48 : 50;
      break;
    case 50:  // $D42A
      /*$D42A*/ CYCLES(0xd42a, 6);
                branchTarget = true; push16(0xd42c); block_id = 150;
      break;
    case 51:  // $D42D
      /*$D42D*/ CYCLES(0xd42d, 7);
                s_a = 0x50;
      /*$D42F*/ s_y = 0xd3;
                block_id = 52;
      break;
    case 52:  // $D431
      /*$D431*/ CYCLES(0xd431, 6);
                branchTarget = true; push16(0xd433); block_id = 380;
      break;
    case 53:  // $D434
      /*$D434*/ CYCLES(0xd434, 9);
      /*$D436*/ tmp1_U8 = (uint8_t)(ram_peek(0x0076) + 0x01);
                s_y = tmp1_U8;
      /*$D437*/ branchTarget = true; block_id = !tmp1_U8 ? 55 : 54;
      break;
    case 54:  // $D439
      /*$D439*/ CYCLES(0xd439, 6);
                branchTarget = true; push16(0xd43b); block_id = 1068;
      break;
    case 55:  // $D43C
      /*$D43C*/ CYCLES(0xd43c, 6);
                branchTarget = true; push16(0xd43e); block_id = 361;
      break;
    case 56:  // $D43F
      /*$D43F*/ CYCLES(0xd43f, 9);
                s_x = 0xdd;
      /*$D441*/ branchTarget = true; push16(0xd443); block_id = 86;
      break;
    case 57:  // $D444
      /*$D444*/ CYCLES(0xd444, 16);
                ram_poke(0x00b8, s_x);
      /*$D446*/ ram_poke(0x00b9, s_y);
      /*$D448*/ ram_poke(0x00d8, (ram_peek(0x00d8) >> 0x01));
      /*$D44A*/ FUNC_CHRGET(0xd44c);
                branchTarget = true; block_id = 58;
      break;
    case 58:  // $D44D
      /*$D44D*/ CYCLES(0xd44d, 6);
                tmp2_U8 = s_a;
                s_x = tmp2_U8;
      /*$D44E*/ branchTarget = true; block_id = !tmp2_U8 ? 55 : 59;
      break;
    case 59:  // $D450
      /*$D450*/ CYCLES(0xd450, 11);
                s_x = 0xff;
      /*$D452*/ ram_poke(0x0076, 0xff);
      /*$D454*/ branchTarget = true; block_id = !s_status_c ? 62 : 60;
      break;
    case 60:  // $D456
      /*$D456*/ CYCLES(0xd456, 6);
                branchTarget = true; push16(0xd458); block_id = 94;
      break;
    case 61:  // $D459
      /*$D459*/ CYCLES(0xd459, 6);
                branchTarget = true; block_id = 223;
      break;
    case 62:  // $D45C
      /*$D45C*/ CYCLES(0xd45c, 19);
      /*$D45E*/ ram_poke(0x0069, ram_peek(0x00af));
      /*$D462*/ ram_poke(0x006a, ram_peek(0x00b0));
      /*$D464*/ branchTarget = true; push16(0xd466); block_id = 317;
      break;
    case 63:  // $D467
      /*$D467*/ CYCLES(0xd467, 6);
                branchTarget = true; push16(0xd469); block_id = 94;
      break;
    case 64:  // $D46A
      /*$D46A*/ CYCLES(0xd46a, 9);
                ram_poke(0x000f, s_y);
      /*$D46C*/ FUNC_FNDLIN(0xd46e);
                branchTarget = true; block_id = 65;
      break;
    case 65:  // $D46F
      /*$D46F*/ CYCLES(0xd46f, 4);
                branchTarget = true; block_id = !s_status_c ? 72 : 66;
      break;
    case 66:  // $D471
      /*$D471*/ CYCLES(0xd471, 74);
      /*$D473*/ tmp1_U8 = peek((ram_peek16(0x009b) + 0x0001));
      /*$D475*/ ram_poke(0x005f, tmp1_U8);
      /*$D479*/ ram_poke(0x005e, ram_peek(0x0069));
      /*$D47D*/ ram_poke(0x0061, ram_peek(0x009c));
      /*$D482*/ tmp1_U8 = peek(ram_peek16(0x009b));
                tmp4_U16 = (ram_peek(0x009b) - tmp1_U8) - (uint8_t)(0x01 - s_status_c);
      /*$D485*/ tmp5_U16 = ram_peek(0x0069);
                tmp1_U8 = (uint8_t)(tmp4_U16 + tmp5_U16);
      /*$D487*/ ram_poke(0x0069, tmp1_U8);
      /*$D489*/ ram_poke(0x0060, tmp1_U8);
      /*$D48D*/ tmp4_U16 = (ram_peek(0x006a) + 0x00ff) + (uint8_t)(((tmp4_U16 & 0x00ff) + tmp5_U16) >> 8);
      /*$D48F*/ ram_poke(0x006a, ((uint8_t)tmp4_U16));
      /*$D493*/ s_x = (uint8_t)(((tmp4_U16 & 0x00ff) - ram_peek(0x009c)) - (uint8_t)(0x01 - (uint8_t)(tmp4_U16 >> 8)));
      /*$D497*/ tmp4_U16 = ram_peek(0x009b) - ram_peek(0x0069);
                tmp1_U8 = (uint8_t)tmp4_U16;
                s_a = tmp1_U8;
      /*$D499*/ s_y = tmp1_U8;
      /*$D49A*/ branchTarget = true; block_id = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01)) ? 68 : 67;
      break;
    case 67:  // $D49C
      /*$D49C*/ CYCLES(0xd49c, 6);
                s_x = (uint8_t)(s_x + 0x01);
      /*$D49D*/ ram_poke(0x0061, (uint8_t)(ram_peek(0x0061) - 0x01));
                block_id = 68;
      break;
    case 68:  // $D49F
      /*$D49F*/ CYCLES(0xd49f, 9);
      /*$D4A0*/ tmp4_U16 = s_a;
                tmp5_U16 = ram_peek(0x005e);
                tmp3_U16 = tmp4_U16 + tmp5_U16;
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = tmp1_U8;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp5_U16);
      /*$D4A2*/ branchTarget = true; block_id = !tmp1_U8 ? 70 : 69;
      break;
    case 69:  // $D4A4
      /*$D4A4*/ CYCLES(0xd4a4, 6);
                ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) - 0x01));
      /*$D4A6*/ s_status_c = 0x00;
                block_id = 70;
      break;
    case 70:  // $D4A7
      /*$D4A7*/ CYCLES(0xd4a7, 12);
                tmp1_U8 = s_y;
                tmp2_U8 = peek((ram_peek16al(0x005e) + tmp1_U8));
      /*$D4A9*/ poke((ram_peek16al(0x0060) + tmp1_U8), tmp2_U8);
      /*$D4AB*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
                s_y = tmp1_U8;
      /*$D4AC*/ branchTarget = true; block_id = tmp1_U8 ? 70 : 71;
      break;
    case 71:  // $D4AE
      /*$D4AE*/ CYCLES(0xd4ae, 12);
                ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) + 0x01));
      /*$D4B0*/ ram_poke(0x0061, (uint8_t)(ram_peek(0x0061) + 0x01));
      /*$D4B2*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp1_U8;
      /*$D4B3*/ branchTarget = true; block_id = tmp1_U8 ? 70 : 72;
      break;
    case 72:  // $D4B5
      /*$D4B5*/ CYCLES(0xd4b5, 9);
      /*$D4B8*/ branchTarget = true; block_id = !ram_peek(0x0200) ? 78 : 73;
      break;
    case 73:  // $D4BA
      /*$D4BA*/ CYCLES(0xd4ba, 38);
      /*$D4BE*/ ram_poke(0x006f, ram_peek(0x0073));
      /*$D4C0*/ ram_poke(0x0070, ram_peek(0x0074));
      /*$D4C2*/ tmp1_U8 = ram_peek(0x0069);
      /*$D4C4*/ ram_poke(0x0096, tmp1_U8);
      /*$D4C6*/ tmp5_U16 = tmp1_U8;
                tmp4_U16 = ram_peek(0x000f);
                tmp3_U16 = (tmp5_U16 + tmp4_U16) + s_status_c;
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = tmp1_U8;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp5_U16, (uint8_t)tmp4_U16);
                tmp2_U8 = (uint8_t)tmp3_U16;
                s_a = tmp2_U8;
      /*$D4C8*/ ram_poke(0x0094, tmp2_U8);
      /*$D4CA*/ tmp2_U8 = ram_peek(0x006a);
                s_y = tmp2_U8;
      /*$D4CC*/ ram_poke(0x0097, tmp2_U8);
      /*$D4CE*/ branchTarget = true; block_id = !tmp1_U8 ? 75 : 74;
      break;
    case 74:  // $D4D0
      /*$D4D0*/ CYCLES(0xd4d0, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 75;
      break;
    case 75:  // $D4D1
      /*$D4D1*/ CYCLES(0xd4d1, 9);
                ram_poke(0x0095, s_y);
      /*$D4D3*/ branchTarget = true; push16(0xd4d5); block_id = 17;
      break;
    case 76:  // $D4D6
      /*$D4D6*/ CYCLES(0xd4d6, 35);
      /*$D4DA*/ ram_poke(0x01fe, ram_peek(0x0050));
      /*$D4DD*/ ram_poke(0x01ff, ram_peek(0x0051));
      /*$D4E4*/ ram_poke(0x0069, ram_peek(0x006d));
      /*$D4E6*/ ram_poke(0x006a, ram_peek(0x006e));
      /*$D4E8*/ s_y = ram_peek(0x000f);
                block_id = 77;
      break;
    case 77:  // $D4EA
      /*$D4EA*/ CYCLES(0xd4ea, 14);
                tmp1_U8 = s_y;
      /*$D4ED*/ tmp2_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_y = tmp2_U8;
      /*$D4EE*/ poke((ram_peek16(0x009b) + tmp2_U8), ram_peek((0x01fb + tmp1_U8)));
      /*$D4F0*/ branchTarget = true; block_id = tmp2_U8 ? 77 : 78;
      break;
    case 78:  // $D4F2
      /*$D4F2*/ CYCLES(0xd4f2, 6);
                branchTarget = true; push16(0xd4f4); block_id = 146;
      break;
    case 79:  // $D4F5
      /*$D4F5*/ CYCLES(0xd4f5, 16);
      /*$D4F9*/ ram_poke(0x005e, ram_peek(0x0067));
      /*$D4FB*/ ram_poke(0x005f, ram_peek(0x0068));
      /*$D4FD*/ s_status_c = 0x00;
                block_id = 80;
      break;
    case 80:  // $D4FE
      /*$D4FE*/ CYCLES(0xd4fe, 11);
                s_y = 0x01;
      /*$D500*/ tmp2_U8 = peek((ram_peek16al(0x005e) + 0x0001));
      /*$D502*/ branchTarget = true; block_id = tmp2_U8 ? 82 : 81;
      break;
    case 81:  // $D504
      /*$D504*/ CYCLES(0xd504, 19);
      /*$D506*/ ram_poke(0x00af, ram_peek(0x0069));
      /*$D50A*/ ram_poke(0x00b0, ram_peek(0x006a));
      /*$D50C*/ branchTarget = true; block_id = 55;
      break;
    case 82:  // $D50F
      /*$D50F*/ CYCLES(0xd50f, 4);
                s_y = 0x04;
                block_id = 83;
      break;
    case 83:  // $D511
      /*$D511*/ CYCLES(0xd511, 9);
                tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$D512*/ tmp2_U8 = peek((ram_peek16al(0x005e) + tmp2_U8));
      /*$D514*/ branchTarget = true; block_id = tmp2_U8 ? 83 : 84;
      break;
    case 84:  // $D516
      /*$D516*/ CYCLES(0xd516, 38);
      /*$D518*/ tmp3_U16 = ((uint8_t)(s_y + 0x01) + ram_peek(0x005e)) + s_status_c;
                tmp2_U8 = (uint8_t)tmp3_U16;
      /*$D51A*/ s_x = tmp2_U8;
      /*$D51D*/ poke(ram_peek16al(0x005e), tmp2_U8);
      /*$D521*/ tmp4_U16 = ram_peek(0x005f);
                tmp3_U16 = tmp4_U16 + (uint8_t)(tmp3_U16 >> 8);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = tmp1_U8;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0x0000);
                tmp6_U8 = (uint8_t)tmp3_U16;
      /*$D523*/ s_y = 0x01;
      /*$D524*/ poke((ram_peek16al(0x005e) + 0x0001), tmp6_U8);
      /*$D526*/ ram_poke(0x005e, tmp2_U8);
      /*$D528*/ ram_poke(0x005f, tmp6_U8);
      /*$D52A*/ branchTarget = true; block_id = !tmp1_U8 ? 80 : 85;
      break;
    case 85:  // $D52C
      /*$D52C*/ CYCLES(0xd52c, 4);
                s_x = 0x80;
                block_id = 86;
      break;
    case 86:  // $D52E
      /*$D52E*/ CYCLES(0xd52e, 9);
                ram_poke(0x0033, s_x);
      /*$D530*/ branchTarget = true; push16(0xd532); block_id = 1562;
      break;
    case 87:  // $D533
      /*$D533*/ CYCLES(0xd533, 7);
                tmp1_U8 = s_x >= 0xef;
                s_status_c = tmp1_U8;
      /*$D535*/ branchTarget = true; block_id = !tmp1_U8 ? 89 : 88;
      break;
    case 88:  // $D537
      /*$D537*/ CYCLES(0xd537, 4);
                s_x = 0xef;
                block_id = 89;
      break;
    case 89:  // $D539
      /*$D539*/ CYCLES(0xd539, 14);
      /*$D53B*/ tmp1_U8 = s_x;
                ram_poke((0x0200 + tmp1_U8), 0x00);
      /*$D53F*/ branchTarget = true; block_id = !tmp1_U8 ? 91 : 90;
      break;
    case 90:  // $D541
      /*$D541*/ CYCLES(0xd541, 19);
                tmp1_U8 = s_x;
      /*$D546*/ ram_poke((0x01ff + tmp1_U8), (ram_peek((0x01ff + tmp1_U8)) & 0x7f));
      /*$D549*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_x = tmp1_U8;
      /*$D54A*/ branchTarget = true; block_id = tmp1_U8 ? 90 : 91;
      break;
    case 91:  // $D54C
      /*$D54C*/ CYCLES(0xd54c, 12);
                s_a = 0x00;
      /*$D54E*/ s_x = 0xff;
      /*$D550*/ s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_y = 0x01;
      /*$D552*/ branchTarget = true; block_id = find_block_id_func_t001(0xd552, pop16() + 1);;
      break;
    case 92:  // $D553
      /*$D553*/ CYCLES(0xd553, 6);
                branchTarget = true; push16(0xd555); block_id = 1544;
      break;
    case 93:  // $D556
      /*$D556*/ CYCLES(0xd556, 6);
                tmp1_U8 = s_a & 0x7f;
                s_status_not_z = tmp1_U8;
                s_status_n = 0x00;
                s_a = tmp1_U8;
      /*$D558*/ branchTarget = true; block_id = find_block_id_func_t001(0xd558, pop16() + 1);;
      break;
    case 94:  // $D559
      /*$D559*/ CYCLES(0xd559, 19);
      /*$D55B*/ s_x = (uint8_t)(ram_peek(0x00b8) - 0x01);
      /*$D55C*/ s_y = 0x04;
      /*$D55E*/ ram_poke(0x0013, 0x04);
      /*$D560*/ tmp1_U8 = ram_peek(0x00d6);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
      /*$D562*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 97 : 95;
      break;
    case 95:  // $D564
      /*$D564*/ CYCLES(0xd564, 9);
                tmp1_U8 = pop8();
      /*$D565*/ tmp1_U8 = pop8();
      /*$D566*/ branchTarget = true; push16(0xd568); block_id = 146;
      break;
    case 96:  // $D569
      /*$D569*/ CYCLES(0xd569, 6);
                branchTarget = true; block_id = 216;
      break;
    case 97:  // $D56C
      /*$D56C*/ CYCLES(0xd56c, 2);
                s_x = (uint8_t)(s_x + 0x01);
                block_id = 98;
      break;
    case 98:  // $D56D
      /*$D56D*/ CYCLES(0xd56d, 12);
                s_a = ram_peek((0x0200 + s_x));
      /*$D570*/ tmp1_U8 = (ram_peek(0x0013) >> 0x06) & 0x01;
                s_status_v = tmp1_U8;
      /*$D572*/ branchTarget = true; block_id = tmp1_U8 ? 100 : 99;
      break;
    case 99:  // $D574
      /*$D574*/ CYCLES(0xd574, 7);
      /*$D576*/ branchTarget = true; block_id = !(s_a != 0x20) ? 97 : 100;
      break;
    case 100:  // $D578
      /*$D578*/ CYCLES(0xd578, 11);
                tmp1_U8 = s_a;
                ram_poke(0x000e, tmp1_U8);
      /*$D57A*/ s_status_c = (tmp1_U8 >= 0x22);
      /*$D57C*/ branchTarget = true; block_id = !(tmp1_U8 != 0x22) ? 126 : 101;
      break;
    case 101:  // $D57E
      /*$D57E*/ CYCLES(0xd57e, 4);
                branchTarget = true; block_id = s_status_v ? 118 : 102;
      break;
    case 102:  // $D580
      /*$D580*/ CYCLES(0xd580, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x3f);
      /*$D582*/ branchTarget = true; block_id = (tmp1_U8 != 0x3f) ? 104 : 103;
      break;
    case 103:  // $D584
      /*$D584*/ CYCLES(0xd584, 7);
                s_a = 0xba;
      /*$D586*/ branchTarget = true; block_id = 118;
      break;
    case 104:  // $D588
      /*$D588*/ CYCLES(0xd588, 7);
      /*$D58A*/ branchTarget = true; block_id = !(s_a >= 0x30) ? 106 : 105;
      break;
    case 105:  // $D58C
      /*$D58C*/ CYCLES(0xd58c, 7);
                tmp1_U8 = s_a >= 0x3c;
                s_status_c = tmp1_U8;
      /*$D58E*/ branchTarget = true; block_id = !tmp1_U8 ? 118 : 106;
      break;
    case 106:  // $D590
      /*$D590*/ CYCLES(0xd590, 31);
                ram_poke(0x00ad, s_y);
      /*$D594*/ ram_poke(0x009d, 0xd0);
      /*$D598*/ ram_poke(0x009e, 0xcf);
      /*$D59C*/ ram_poke(0x000f, 0x00);
      /*$D59E*/ s_y = 0xff;
      /*$D59F*/ tmp1_U8 = s_x;
                ram_poke(0x00b8, tmp1_U8);
      /*$D5A1*/ s_x = (uint8_t)(tmp1_U8 - 0x01);
                block_id = 107;
      break;
    case 107:  // $D5A2
      /*$D5A2*/ CYCLES(0xd5a2, 6);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$D5A3*/ branchTarget = true; block_id = tmp1_U8 ? 109 : 108;
      break;
    case 108:  // $D5A5
      /*$D5A5*/ CYCLES(0xd5a5, 4);
                ram_poke(0x009e, (uint8_t)(ram_peek(0x009e) + 0x01));
                block_id = 109;
      break;
    case 109:  // $D5A7
      /*$D5A7*/ CYCLES(0xd5a7, 2);
                s_x = (uint8_t)(s_x + 0x01);
                block_id = 110;
      break;
    case 110:  // $D5A8
      /*$D5A8*/ CYCLES(0xd5a8, 12);
                tmp1_U8 = ram_peek((0x0200 + s_x));
                s_a = tmp1_U8;
      /*$D5AD*/ branchTarget = true; block_id = !(tmp1_U8 != 0x20) ? 109 : 111;
      break;
    case 111:  // $D5AF
      /*$D5AF*/ CYCLES(0xd5af, 9);
      /*$D5B0*/ tmp3_U16 = s_a;
                tmp1_U8 = peek((ram_peek16(0x009d) + s_y));
                tmp4_U16 = tmp1_U8;
                tmp5_U16 = tmp3_U16 - tmp4_U16;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp4_U16));
                tmp1_U8 = (uint8_t)tmp5_U16;
                s_a = tmp1_U8;
      /*$D5B2*/ branchTarget = true; block_id = !tmp1_U8 ? 107 : 112;
      break;
    case 112:  // $D5B4
      /*$D5B4*/ CYCLES(0xd5b4, 7);
      /*$D5B6*/ branchTarget = true; block_id = (s_a != 0x80) ? 127 : 113;
      break;
    case 113:  // $D5B8
      /*$D5B8*/ CYCLES(0xd5b8, 11);
                tmp1_U8 = s_a | ram_peek(0x000f);
                s_a = tmp1_U8;
      /*$D5BA*/ s_status_c = (tmp1_U8 >= 0xc5);
      /*$D5BC*/ branchTarget = true; block_id = (tmp1_U8 != 0xc5) ? 117 : 114;
      break;
    case 114:  // $D5BE
      /*$D5BE*/ CYCLES(0xd5be, 12);
                tmp1_U8 = ram_peek((0x0201 + s_x));
                s_a = tmp1_U8;
      /*$D5C3*/ branchTarget = true; block_id = !(tmp1_U8 != 0x4e) ? 127 : 115;
      break;
    case 115:  // $D5C5
      /*$D5C5*/ CYCLES(0xd5c5, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x4f);
      /*$D5C7*/ branchTarget = true; block_id = !(tmp1_U8 != 0x4f) ? 127 : 116;
      break;
    case 116:  // $D5C9
      /*$D5C9*/ CYCLES(0xd5c9, 4);
                s_a = 0xc5;
                block_id = 117;
      break;
    case 117:  // $D5CB
      /*$D5CB*/ CYCLES(0xd5cb, 4);
                s_y = ram_peek(0x00ad);
                block_id = 118;
      break;
    case 118:  // $D5CD
      /*$D5CD*/ CYCLES(0xd5cd, 18);
                s_x = (uint8_t)(s_x + 0x01);
      /*$D5CE*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$D5CF*/ ram_poke((0x01fb + tmp1_U8), s_a);
      /*$D5D2*/ tmp1_U8 = ram_peek((0x01fb + tmp1_U8));
                s_a = tmp1_U8;
      /*$D5D5*/ branchTarget = true; block_id = !tmp1_U8 ? 133 : 119;
      break;
    case 119:  // $D5D7
      /*$D5D7*/ CYCLES(0xd5d7, 9);
      /*$D5D8*/ tmp1_U8 = (uint8_t)(s_a - 0x003a);
                s_a = tmp1_U8;
      /*$D5DA*/ branchTarget = true; block_id = !tmp1_U8 ? 121 : 120;
      break;
    case 120:  // $D5DC
      /*$D5DC*/ CYCLES(0xd5dc, 7);
      /*$D5DE*/ branchTarget = true; block_id = (s_a != 0x49) ? 122 : 121;
      break;
    case 121:  // $D5E0
      /*$D5E0*/ CYCLES(0xd5e0, 4);
                ram_poke(0x0013, s_a);
                block_id = 122;
      break;
    case 122:  // $D5E2
      /*$D5E2*/ CYCLES(0xd5e2, 9);
      /*$D5E3*/ tmp5_U16 = s_a;
                tmp4_U16 = tmp5_U16 - 0x0078;
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0xff87);
                tmp1_U8 = (uint8_t)tmp4_U16;
                s_a = tmp1_U8;
      /*$D5E5*/ branchTarget = true; block_id = tmp1_U8 ? 98 : 123;
      break;
    case 123:  // $D5E7
      /*$D5E7*/ CYCLES(0xd5e7, 4);
                ram_poke(0x000e, s_a);
                block_id = 124;
      break;
    case 124:  // $D5E9
      /*$D5E9*/ CYCLES(0xd5e9, 9);
                tmp1_U8 = ram_peek((0x0200 + s_x));
                s_a = tmp1_U8;
      /*$D5EC*/ branchTarget = true; block_id = !tmp1_U8 ? 118 : 125;
      break;
    case 125:  // $D5EE
      /*$D5EE*/ CYCLES(0xd5ee, 7);
                tmp1_U8 = s_a;
                tmp6_U8 = ram_peek(0x000e);
                s_status_c = (tmp1_U8 >= tmp6_U8);
      /*$D5F0*/ branchTarget = true; block_id = !(tmp1_U8 != tmp6_U8) ? 118 : 126;
      break;
    case 126:  // $D5F2
      /*$D5F2*/ CYCLES(0xd5f2, 12);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$D5F3*/ ram_poke((0x01fb + tmp1_U8), s_a);
      /*$D5F6*/ tmp1_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp1_U8;
      /*$D5F7*/ branchTarget = true; block_id = tmp1_U8 ? 124 : 127;
      break;
    case 127:  // $D5F9
      /*$D5F9*/ CYCLES(0xd5f9, 7);
                s_x = ram_peek(0x00b8);
      /*$D5FB*/ ram_poke(0x000f, (uint8_t)(ram_peek(0x000f) + 0x01));
                block_id = 128;
      break;
    case 128:  // $D5FD
      /*$D5FD*/ CYCLES(0xd5fd, 9);
                tmp6_U8 = s_y;
                tmp1_U8 = peek((ram_peek16(0x009d) + tmp6_U8));
                s_a = tmp1_U8;
      /*$D5FF*/ tmp6_U8 = (uint8_t)(tmp6_U8 + 0x01);
                s_y = tmp6_U8;
      /*$D600*/ branchTarget = true; block_id = tmp6_U8 ? 130 : 129;
      break;
    case 129:  // $D602
      /*$D602*/ CYCLES(0xd602, 4);
                ram_poke(0x009e, (uint8_t)(ram_peek(0x009e) + 0x01));
                block_id = 130;
      break;
    case 130:  // $D604
      /*$D604*/ CYCLES(0xd604, 6);
                tmp6_U8 = (uint8_t)((s_a << 0x01) >> 8);
                s_status_c = tmp6_U8;
      /*$D605*/ branchTarget = true; block_id = !tmp6_U8 ? 128 : 131;
      break;
    case 131:  // $D607
      /*$D607*/ CYCLES(0xd607, 7);
                tmp6_U8 = peek((ram_peek16(0x009d) + s_y));
      /*$D609*/ branchTarget = true; block_id = tmp6_U8 ? 110 : 132;
      break;
    case 132:  // $D60B
      /*$D60B*/ CYCLES(0xd60b, 9);
                tmp6_U8 = ram_peek((0x0200 + s_x));
                s_a = tmp6_U8;
      /*$D60E*/ branchTarget = true; block_id = !(tmp6_U8 & 0x80) ? 117 : 133;
      break;
    case 133:  // $D610
      /*$D610*/ CYCLES(0xd610, 18);
                ram_poke((0x01fd + s_y), s_a);
      /*$D613*/ ram_poke(0x00b9, (uint8_t)(ram_peek(0x00b9) - 0x01));
      /*$D615*/ s_status_not_z = 0xff;
                s_status_n = 0x80;
                s_a = 0xff;
      /*$D617*/ ram_poke(0x00b8, 0xff);
      /*$D619*/ branchTarget = true; block_id = find_block_id_func_t001(0xd619, pop16() + 1);;
      break;
    case 134:  // $D61A
      /*$D61A*/ CYCLES(0xd61a, 7);
                s_a = ram_peek(0x0067);
      /*$D61C*/ s_x = ram_peek(0x0068);
                FUNC_FNDLIN2(0x0000);
                block_id = find_block_id_func_t001(0xd61c, pop16() + 1);;
      break;
    case 135:  // $D61E
      /*$D61E*/ CYCLES(0xd61e, 18);
                s_y = 0x01;
      /*$D620*/ ram_poke(0x009b, s_a);
      /*$D622*/ ram_poke(0x009c, s_x);
      /*$D624*/ tmp6_U8 = peek((ram_peek16(0x009b) + 0x0001));
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$D626*/ branchTarget = true; block_id = !tmp6_U8 ? 142 : 136;
      break;
    case 136:  // $D628
      /*$D628*/ CYCLES(0xd628, 14);
      /*$D629*/ tmp6_U8 = (uint8_t)(s_y + 0x02);
                s_y = tmp6_U8;
      /*$D62A*/ tmp1_U8 = ram_peek(0x0051);
                s_a = tmp1_U8;
      /*$D62C*/ tmp6_U8 = peek((ram_peek16(0x009b) + tmp6_U8));
                s_status_not_z = (tmp1_U8 != tmp6_U8);
                tmp2_U8 = tmp1_U8 >= tmp6_U8;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - tmp6_U8) & 0x80);
      /*$D62E*/ branchTarget = true; block_id = !tmp2_U8 ? 143 : 137;
      break;
    case 137:  // $D630
      /*$D630*/ CYCLES(0xd630, 4);
                branchTarget = true; block_id = !s_status_not_z ? 139 : 138;
      break;
    case 138:  // $D632
      /*$D632*/ CYCLES(0xd632, 6);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp1_U8;
      /*$D633*/ branchTarget = true; block_id = tmp1_U8 ? 141 : 139;
      break;
    case 139:  // $D635
      /*$D635*/ CYCLES(0xd635, 12);
                tmp2_U8 = ram_peek(0x0050);
                s_a = tmp2_U8;
      /*$D637*/ tmp6_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp6_U8;
      /*$D638*/ tmp6_U8 = peek((ram_peek16(0x009b) + tmp6_U8));
                s_status_not_z = (tmp2_U8 != tmp6_U8);
                tmp1_U8 = tmp2_U8 >= tmp6_U8;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp6_U8) & 0x80);
      /*$D63A*/ branchTarget = true; block_id = !tmp1_U8 ? 143 : 140;
      break;
    case 140:  // $D63C
      /*$D63C*/ CYCLES(0xd63c, 4);
                branchTarget = true; block_id = !s_status_not_z ? 143 : 141;
      break;
    case 141:  // $D63E
      /*$D63E*/ CYCLES(0xd63e, 16);
                tmp1_U8 = s_y;
      /*$D63F*/ tmp6_U8 = peek((ram_peek16(0x009b) + (uint8_t)(tmp1_U8 - 0x01)));
      /*$D641*/ s_x = tmp6_U8;
      /*$D642*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x02);
                s_y = tmp1_U8;
      /*$D643*/ tmp1_U8 = peek((ram_peek16(0x009b) + tmp1_U8));
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$D645*/ branchTarget = true; block_id = s_status_c ? 135 : 142;
      break;
    case 142:  // $D647
      /*$D647*/ CYCLES(0xd647, 2);
                s_status_c = 0x00;
                block_id = 143;
      break;
    case 143:  // $D648
      /*$D648*/ CYCLES(0xd648, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd648, pop16() + 1);;
      break;
    case 144:  // $D649
      /*$D649*/ CYCLES(0xd649, 4);
                branchTarget = true; block_id = s_status_not_z ? 143 : 145;
      break;
    case 145:  // $D64B
      /*$D64B*/ CYCLES(0xd64b, 45);
      /*$D64D*/ ram_poke(0x00d6, 0x00);
      /*$D650*/ poke(ram_peek16(0x0067), 0x00);
      /*$D652*/ s_y = 0x01;
      /*$D653*/ poke((ram_peek16(0x0067) + 0x0001), 0x00);
      /*$D657*/ tmp4_U16 = (ram_peek(0x0067) + 0x0002) + s_status_c;
                tmp1_U8 = (uint8_t)tmp4_U16;
      /*$D659*/ ram_poke(0x0069, tmp1_U8);
      /*$D65B*/ ram_poke(0x00af, tmp1_U8);
      /*$D65F*/ tmp1_U8 = (uint8_t)(ram_peek(0x0068) + (uint8_t)(tmp4_U16 >> 8));
      /*$D661*/ ram_poke(0x006a, tmp1_U8);
      /*$D663*/ ram_poke(0x00b0, tmp1_U8);
                block_id = 146;
      break;
    case 146:  // $D665
      /*$D665*/ CYCLES(0xd665, 6);
                FUNC_CLRTXTPTR(0xd667);
                branchTarget = true; block_id = 147;
      break;
    case 147:  // $D668
      /*$D668*/ CYCLES(0xd668, 4);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
                block_id = 148;
      break;
    case 148:  // $D66A
      /*$D66A*/ CYCLES(0xd66a, 4);
                branchTarget = true; block_id = s_status_not_z ? 151 : 149;
      break;
    case 149:  // $D66C
      /*$D66C*/ CYCLES(0xd66c, 40);
      /*$D670*/ ram_poke(0x006f, ram_peek(0x0073));
      /*$D672*/ ram_poke(0x0070, ram_peek(0x0074));
      /*$D674*/ tmp1_U8 = ram_peek(0x0069);
      /*$D676*/ tmp6_U8 = ram_peek(0x006a);
      /*$D678*/ ram_poke(0x006b, tmp1_U8);
      /*$D67A*/ ram_poke(0x006c, tmp6_U8);
      /*$D67C*/ ram_poke(0x006d, tmp1_U8);
      /*$D67E*/ ram_poke(0x006e, tmp6_U8);
      /*$D680*/ branchTarget = true; push16(0xd682); block_id = 239;
      break;
    case 150:  // $D683
      /*$D683*/ CYCLES(0xd683, 33);
      /*$D685*/ ram_poke(0x0052, 0x55);
      /*$D687*/ tmp1_U8 = pop8();
      /*$D688*/ s_y = tmp1_U8;
      /*$D689*/ tmp2_U8 = pop8();
      /*$D68A*/ s_x = 0xf8;
      /*$D68C*/ s_sp = 0xf8;
      /*$D68D*/ push8(tmp2_U8);
      /*$D68F*/ push8(tmp1_U8);
      /*$D690*/ s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$D692*/ ram_poke(0x007a, 0x00);
      /*$D694*/ ram_poke(0x0014, 0x00);
                block_id = 151;
      break;
    case 151:  // $D696
      /*$D696*/ CYCLES(0xd696, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd696, pop16() + 1);;
      break;
    case 152:  // $D697
      /*$D697*/ CYCLES(0xd697, 24);
      /*$D69A*/ tmp4_U16 = ram_peek(0x0067) + 0x00ff;
      /*$D69C*/ ram_poke(0x00b8, ((uint8_t)tmp4_U16));
      /*$D6A0*/ tmp5_U16 = ram_peek(0x0068);
                tmp4_U16 = (tmp5_U16 + 0x00ff) + (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x00ff);
                tmp1_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$D6A2*/ ram_poke(0x00b9, tmp1_U8);
      /*$D6A4*/ branchTarget = true; block_id = find_block_id_func_t001(0xd6a4, pop16() + 1);;
      break;
    case 153:  // $D6A5
      /*$D6A5*/ CYCLES(0xd6a5, 4);
                branchTarget = true; block_id = !s_status_c ? 157 : 154;
      break;
    case 154:  // $D6A7
      /*$D6A7*/ CYCLES(0xd6a7, 4);
                branchTarget = true; block_id = !s_status_not_z ? 157 : 155;
      break;
    case 155:  // $D6A9
      /*$D6A9*/ CYCLES(0xd6a9, 7);
                tmp6_U8 = s_a;
                s_status_c = (tmp6_U8 >= 0xc9);
      /*$D6AB*/ branchTarget = true; block_id = !(tmp6_U8 != 0xc9) ? 157 : 156;
      break;
    case 156:  // $D6AD
      /*$D6AD*/ CYCLES(0xd6ad, 7);
                tmp6_U8 = s_a;
                tmp1_U8 = tmp6_U8 != 0x2c;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp6_U8 >= 0x2c);
                s_status_n = ((uint8_t)(tmp6_U8 - 0x2c) & 0x80);
      /*$D6AF*/ branchTarget = true; block_id = tmp1_U8 ? 151 : 157;
      break;
    case 157:  // $D6B1
      /*$D6B1*/ CYCLES(0xd6b1, 6);
                branchTarget = true; push16(0xd6b3); block_id = 317;
      break;
    case 158:  // $D6B4
      /*$D6B4*/ CYCLES(0xd6b4, 6);
                FUNC_FNDLIN(0xd6b6);
                branchTarget = true; block_id = 159;
      break;
    case 159:  // $D6B7
      /*$D6B7*/ CYCLES(0xd6b7, 6);
                FUNC_CHRGOT(0xd6b9);
                branchTarget = true; block_id = 160;
      break;
    case 160:  // $D6BA
      /*$D6BA*/ CYCLES(0xd6ba, 4);
                branchTarget = true; block_id = !s_status_not_z ? 165 : 161;
      break;
    case 161:  // $D6BC
      /*$D6BC*/ CYCLES(0xd6bc, 7);
      /*$D6BE*/ branchTarget = true; block_id = !(s_a != 0xc9) ? 163 : 162;
      break;
    case 162:  // $D6C0
      /*$D6C0*/ CYCLES(0xd6c0, 7);
                tmp1_U8 = s_a;
                tmp6_U8 = tmp1_U8 != 0x2c;
                s_status_not_z = tmp6_U8;
                s_status_c = (tmp1_U8 >= 0x2c);
                s_status_n = ((uint8_t)(tmp1_U8 - 0x2c) & 0x80);
      /*$D6C2*/ branchTarget = true; block_id = tmp6_U8 ? 143 : 163;
      break;
    case 163:  // $D6C4
      /*$D6C4*/ CYCLES(0xd6c4, 6);
                FUNC_CHRGET(0xd6c6);
      /*$D6C7*/ CYCLES(0xd6c7, 6);
                branchTarget = true; push16(0xd6c9); block_id = 317;
      break;
    case 164:  // $D6CA
      /*$D6CA*/ CYCLES(0xd6ca, 4);
                branchTarget = true; block_id = s_status_not_z ? 151 : 165;
      break;
    case 165:  // $D6CC
      /*$D6CC*/ CYCLES(0xd6cc, 14);
                tmp1_U8 = pop8();
      /*$D6CD*/ tmp1_U8 = pop8();
      /*$D6D2*/ branchTarget = true; block_id = (ram_peek(0x0050) | ram_peek(0x0051)) ? 167 : 166;
      break;
    case 166:  // $D6D4
      /*$D6D4*/ CYCLES(0xd6d4, 11);
      /*$D6D6*/ ram_poke(0x0050, 0xff);
      /*$D6D8*/ ram_poke(0x0051, 0xff);
                block_id = 167;
      break;
    case 167:  // $D6DA
      /*$D6DA*/ CYCLES(0xd6da, 11);
                s_y = 0x01;
      /*$D6DC*/ tmp6_U8 = peek((ram_peek16(0x009b) + 0x0001));
      /*$D6DE*/ branchTarget = true; block_id = !tmp6_U8 ? 182 : 168;
      break;
    case 168:  // $D6E0
      /*$D6E0*/ CYCLES(0xd6e0, 6);
                branchTarget = true; push16(0xd6e2); block_id = 243;
      break;
    case 169:  // $D6E3
      /*$D6E3*/ CYCLES(0xd6e3, 6);
                branchTarget = true; push16(0xd6e5); block_id = 361;
      break;
    case 170:  // $D6E6
      /*$D6E6*/ CYCLES(0xd6e6, 19);
                tmp1_U8 = s_y;
      /*$D6E7*/ tmp6_U8 = peek((ram_peek16(0x009b) + (uint8_t)(tmp1_U8 + 0x01)));
      /*$D6E9*/ s_x = tmp6_U8;
      /*$D6EA*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x02);
                s_y = tmp1_U8;
      /*$D6EB*/ tmp1_U8 = peek((ram_peek16(0x009b) + tmp1_U8));
                s_a = tmp1_U8;
      /*$D6ED*/ tmp6_U8 = ram_peek(0x0051);
                s_status_c = (tmp1_U8 >= tmp6_U8);
      /*$D6EF*/ branchTarget = true; block_id = (tmp1_U8 != tmp6_U8) ? 172 : 171;
      break;
    case 171:  // $D6F1
      /*$D6F1*/ CYCLES(0xd6f1, 7);
                tmp6_U8 = s_x;
                tmp1_U8 = ram_peek(0x0050);
                s_status_c = (tmp6_U8 >= tmp1_U8);
      /*$D6F3*/ branchTarget = true; block_id = !(tmp6_U8 != tmp1_U8) ? 173 : 172;
      break;
    case 172:  // $D6F5
      /*$D6F5*/ CYCLES(0xd6f5, 4);
                branchTarget = true; block_id = s_status_c ? 182 : 173;
      break;
    case 173:  // $D6F7
      /*$D6F7*/ CYCLES(0xd6f7, 9);
                ram_poke(0x0085, s_y);
      /*$D6F9*/ branchTarget = true; push16(0xd6fb); block_id = 1070;
      break;
    case 174:  // $D6FC
      /*$D6FC*/ CYCLES(0xd6fc, 4);
                s_a = 0x20;
                block_id = 175;
      break;
    case 175:  // $D6FE
      /*$D6FE*/ CYCLES(0xd6fe, 7);
                s_y = ram_peek(0x0085);
      /*$D700*/ s_a = (s_a & 0x7f);
                block_id = 176;
      break;
    case 176:  // $D702
      /*$D702*/ CYCLES(0xd702, 6);
                branchTarget = true; push16(0xd704); block_id = 390;
      break;
    case 177:  // $D705
      /*$D705*/ CYCLES(0xd705, 11);
      /*$D707*/ tmp1_U8 = ram_peek(0x0024) >= 0x21;
                s_status_c = tmp1_U8;
      /*$D709*/ branchTarget = true; block_id = !tmp1_U8 ? 180 : 178;
      break;
    case 178:  // $D70B
      /*$D70B*/ CYCLES(0xd70b, 6);
                branchTarget = true; push16(0xd70d); block_id = 361;
      break;
    case 179:  // $D70E
      /*$D70E*/ CYCLES(0xd70e, 7);
      /*$D710*/ ram_poke(0x0024, 0x05);
                block_id = 180;
      break;
    case 180:  // $D712
      /*$D712*/ CYCLES(0xd712, 9);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$D713*/ tmp1_U8 = peek((ram_peek16(0x009b) + tmp1_U8));
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$D715*/ branchTarget = true; block_id = tmp1_U8 ? 187 : 181;
      break;
    case 181:  // $D717
      /*$D717*/ CYCLES(0xd717, 23);
                tmp1_U8 = s_a;
      /*$D718*/ tmp6_U8 = peek((ram_peek16(0x009b) + tmp1_U8));
      /*$D71A*/ s_x = tmp6_U8;
      /*$D71B*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
                s_y = tmp1_U8;
      /*$D71C*/ tmp1_U8 = peek((ram_peek16(0x009b) + tmp1_U8));
      /*$D71E*/ ram_poke(0x009b, tmp6_U8);
      /*$D720*/ ram_poke(0x009c, tmp1_U8);
      /*$D722*/ branchTarget = true; block_id = tmp1_U8 ? 167 : 182;
      break;
    case 182:  // $D724
      /*$D724*/ CYCLES(0xd724, 9);
                s_a = 0x0d;
      /*$D726*/ branchTarget = true; push16(0xd728); block_id = 390;
      break;
    case 183:  // $D729
      /*$D729*/ CYCLES(0xd729, 6);
                branchTarget = true; block_id = 216;
      break;
    case 184:  // $D72C
      /*$D72C*/ CYCLES(0xd72c, 6);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$D72D*/ branchTarget = true; block_id = tmp1_U8 ? 186 : 185;
      break;
    case 185:  // $D72F
      /*$D72F*/ CYCLES(0xd72f, 4);
                ram_poke(0x009e, (uint8_t)(ram_peek(0x009e) + 0x01));
                block_id = 186;
      break;
    case 186:  // $D731
      /*$D731*/ CYCLES(0xd731, 6);
                tmp1_U8 = peek((ram_peek16(0x009d) + s_y));
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$D733*/ branchTarget = true; block_id = find_block_id_func_t001(0xd733, pop16() + 1);;
      break;
    case 187:  // $D734
      /*$D734*/ CYCLES(0xd734, 4);
                branchTarget = true; block_id = !s_status_n ? 176 : 188;
      break;
    case 188:  // $D736
      /*$D736*/ CYCLES(0xd736, 28);
      /*$D737*/ tmp4_U16 = s_a;
                tmp5_U16 = tmp4_U16 - 0x007f;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0xff80);
      /*$D739*/ s_x = ((uint8_t)tmp5_U16);
      /*$D73A*/ ram_poke(0x0085, s_y);
      /*$D73E*/ ram_poke(0x009d, 0xd0);
      /*$D742*/ ram_poke(0x009e, 0xcf);
      /*$D744*/ s_y = 0xff;
                block_id = 189;
      break;
    case 189:  // $D746
      /*$D746*/ CYCLES(0xd746, 6);
                tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp1_U8;
      /*$D747*/ branchTarget = true; block_id = !tmp1_U8 ? 192 : 190;
      break;
    case 190:  // $D749
      /*$D749*/ CYCLES(0xd749, 6);
                FUNC_GETCHAR(0xd74b);
      /*$D74C*/ CYCLES(0xd74c, 4);
                branchTarget = true; block_id = !s_status_n ? 190 : 191;
      break;
    case 191:  // $D74E
      /*$D74E*/ CYCLES(0xd74e, 4);
                branchTarget = true; block_id = s_status_n ? 189 : 192;
      break;
    case 192:  // $D750
      /*$D750*/ CYCLES(0xd750, 9);
                s_a = 0x20;
      /*$D752*/ branchTarget = true; push16(0xd754); block_id = 390;
      break;
    case 193:  // $D755
      /*$D755*/ CYCLES(0xd755, 6);
                FUNC_GETCHAR(0xd757);
      /*$D758*/ CYCLES(0xd758, 4);
                branchTarget = true; block_id = s_status_n ? 196 : 194;
      break;
    case 194:  // $D75A
      /*$D75A*/ CYCLES(0xd75a, 6);
                branchTarget = true; push16(0xd75c); block_id = 390;
      break;
    case 195:  // $D75D
      /*$D75D*/ CYCLES(0xd75d, 4);
                branchTarget = true; block_id = s_status_not_z ? 193 : 196;
      break;
    case 196:  // $D75F
      /*$D75F*/ CYCLES(0xd75f, 6);
                branchTarget = true; push16(0xd761); block_id = 390;
      break;
    case 197:  // $D762
      /*$D762*/ CYCLES(0xd762, 7);
                s_a = 0x20;
      /*$D764*/ branchTarget = true; block_id = 175;
      break;
    case 198:  // $D766
      /*$D766*/ CYCLES(0xd766, 12);
                s_a = 0x80;
      /*$D768*/ ram_poke(0x0014, 0x80);
      /*$D76A*/ branchTarget = true; push16(0xd76c); block_id = 324;
      break;
    case 199:  // $D76D
      /*$D76D*/ CYCLES(0xd76d, 6);
                FUNC_GETFORPTR(0xd76f);
                branchTarget = true; block_id = 200;
      break;
    case 200:  // $D770
      /*$D770*/ CYCLES(0xd770, 4);
                branchTarget = true; block_id = s_status_not_z ? 202 : 201;
      break;
    case 201:  // $D772
      /*$D772*/ CYCLES(0xd772, 9);
      /*$D773*/ tmp5_U16 = (s_x + 0x000f) + s_status_c;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                tmp1_U8 = (uint8_t)tmp5_U16;
      /*$D775*/ s_x = tmp1_U8;
      /*$D776*/ s_sp = tmp1_U8;
                block_id = 202;
      break;
    case 202:  // $D777
      /*$D777*/ CYCLES(0xd777, 12);
                tmp1_U8 = pop8();
      /*$D778*/ tmp1_U8 = pop8();
      /*$D779*/ s_a = 0x09;
      /*$D77B*/ branchTarget = true; push16(0xd77d); block_id = 29;
      break;
    case 203:  // $D77E
      /*$D77E*/ CYCLES(0xd77e, 6);
                branchTarget = true; push16(0xd780); block_id = 289;
      break;
    case 204:  // $D781
      /*$D781*/ CYCLES(0xd781, 36);
      /*$D783*/ tmp5_U16 = s_y + ram_peek(0x00b8);
      /*$D785*/ push8(((uint8_t)tmp5_U16));
      /*$D788*/ tmp4_U16 = ram_peek(0x00b9);
                tmp5_U16 = tmp4_U16 + (uint8_t)(tmp5_U16 >> 8);
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x0000);
      /*$D78A*/ push8(((uint8_t)tmp5_U16));
      /*$D78D*/ push8(ram_peek(0x0076));
      /*$D790*/ push8(ram_peek(0x0075));
      /*$D791*/ s_a = 0xc1;
      /*$D793*/ branchTarget = true; push16(0xd795); block_id = 566;
      break;
    case 205:  // $D796
      /*$D796*/ CYCLES(0xd796, 6);
                branchTarget = true; push16(0xd798); block_id = 487;
      break;
    case 206:  // $D799
      /*$D799*/ CYCLES(0xd799, 6);
                branchTarget = true; push16(0xd79b); block_id = 486;
      break;
    case 207:  // $D79C
      /*$D79C*/ CYCLES(0xd79c, 33);
      /*$D7A2*/ ram_poke(0x009e, ((ram_peek(0x00a2) | 0x7f) & ram_peek(0x009e)));
      /*$D7A6*/ s_y = 0xd7;
      /*$D7A8*/ ram_poke(0x005e, 0xaf);
      /*$D7AA*/ ram_poke(0x005f, 0xd7);
      /*$D7AC*/ branchTarget = true; block_id = 529;
      break;
    case 208:  // $D7AF
      /*$D7AF*/ CYCLES(0xd7af, 12);
                s_a = 0x13;
      /*$D7B1*/ s_y = 0xe9;
      /*$D7B3*/ FUNC_UPAY2FAC(0xd7b5);
                branchTarget = true; block_id = 209;
      break;
    case 209:  // $D7B6
      /*$D7B6*/ CYCLES(0xd7b6, 6);
                FUNC_CHRGOT(0xd7b8);
                branchTarget = true; block_id = 210;
      break;
    case 210:  // $D7B9
      /*$D7B9*/ CYCLES(0xd7b9, 7);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0xc7);
      /*$D7BB*/ branchTarget = true; block_id = (tmp2_U8 != 0xc7) ? 213 : 211;
      break;
    case 211:  // $D7BD
      /*$D7BD*/ CYCLES(0xd7bd, 6);
                FUNC_CHRGET(0xd7bf);
                branchTarget = true; block_id = 212;
      break;
    case 212:  // $D7C0
      /*$D7C0*/ CYCLES(0xd7c0, 6);
                branchTarget = true; push16(0xd7c2); block_id = 486;
      break;
    case 213:  // $D7C3
      /*$D7C3*/ CYCLES(0xd7c3, 6);
                FUNC_SIGN(0xd7c5);
                branchTarget = true; block_id = 214;
      break;
    case 214:  // $D7C6
      /*$D7C6*/ CYCLES(0xd7c6, 6);
                branchTarget = true; push16(0xd7c8); block_id = 528;
      break;
    case 215:  // $D7C9
      /*$D7C9*/ CYCLES(0xd7c9, 16);
      /*$D7CB*/ push8(ram_peek(0x0086));
      /*$D7CE*/ push8(ram_peek(0x0085));
      /*$D7D1*/ push8(0x81);
                block_id = 216;
      break;
    case 216:  // $D7D2
      /*$D7D2*/ CYCLES(0xd7d2, 11);
                tmp1_U8 = s_sp;
                s_x = tmp1_U8;
      /*$D7D3*/ ram_poke(0x00f8, tmp1_U8);
      /*$D7D5*/ branchTarget = true; push16(0xd7d7); block_id = 243;
      break;
    case 217:  // $D7D8
      /*$D7D8*/ CYCLES(0xd7d8, 16);
                s_a = ram_peek(0x00b8);
      /*$D7DA*/ s_y = ram_peek(0x00b9);
      /*$D7DE*/ tmp6_U8 = (uint8_t)(ram_peek(0x0076) + 0x01);
                s_x = tmp6_U8;
      /*$D7DF*/ branchTarget = true; block_id = !tmp6_U8 ? 219 : 218;
      break;
    case 218:  // $D7E1
      /*$D7E1*/ CYCLES(0xd7e1, 7);
                ram_poke(0x0079, s_a);
      /*$D7E3*/ ram_poke(0x007a, s_y);
                block_id = 219;
      break;
    case 219:  // $D7E5
      /*$D7E5*/ CYCLES(0xd7e5, 11);
                s_y = 0x00;
      /*$D7E7*/ tmp6_U8 = peek(ram_peek16al(0x00b8));
                s_a = tmp6_U8;
      /*$D7E9*/ branchTarget = true; block_id = tmp6_U8 ? 237 : 220;
      break;
    case 220:  // $D7EB
      /*$D7EB*/ CYCLES(0xd7eb, 12);
                s_y = 0x02;
      /*$D7ED*/ tmp6_U8 = peek((ram_peek16al(0x00b8) + 0x0002));
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$D7EF*/ s_status_c = 0x00;
      /*$D7F0*/ branchTarget = true; block_id = !tmp6_U8 ? 231 : 221;
      break;
    case 221:  // $D7F2
      /*$D7F2*/ CYCLES(0xd7f2, 29);
                tmp6_U8 = s_y;
      /*$D7F3*/ tmp1_U8 = peek((ram_peek16al(0x00b8) + (uint8_t)(tmp6_U8 + 0x01)));
      /*$D7F5*/ ram_poke(0x0075, tmp1_U8);
      /*$D7F7*/ tmp6_U8 = (uint8_t)(tmp6_U8 + 0x02);
                s_y = tmp6_U8;
      /*$D7F8*/ tmp1_U8 = peek((ram_peek16al(0x00b8) + tmp6_U8));
      /*$D7FA*/ ram_poke(0x0076, tmp1_U8);
      /*$D7FD*/ tmp5_U16 = (tmp6_U8 + ram_peek(0x00b8)) + s_status_c;
                tmp6_U8 = (uint8_t)tmp5_U16;
                s_a = tmp6_U8;
      /*$D7FF*/ ram_poke(0x00b8, tmp6_U8);
      /*$D801*/ branchTarget = true; block_id = !(uint8_t)(tmp5_U16 >> 8) ? 223 : 222;
      break;
    case 222:  // $D803
      /*$D803*/ CYCLES(0xd803, 4);
                ram_poke(0x00b9, (uint8_t)(ram_peek(0x00b9) + 0x01));
                block_id = 223;
      break;
    case 223:  // $D805
      /*$D805*/ CYCLES(0xd805, 7);
                tmp1_U8 = ram_peek(0x00f2);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
      /*$D807*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 228 : 224;
      break;
    case 224:  // $D809
      /*$D809*/ CYCLES(0xd809, 9);
      /*$D80B*/ tmp6_U8 = (uint8_t)(ram_peek(0x0076) + 0x01);
                s_x = tmp6_U8;
      /*$D80C*/ branchTarget = true; block_id = !tmp6_U8 ? 228 : 225;
      break;
    case 225:  // $D80E
      /*$D80E*/ CYCLES(0xd80e, 9);
                s_a = 0x23;
      /*$D810*/ branchTarget = true; push16(0xd812); block_id = 390;
      break;
    case 226:  // $D813
      /*$D813*/ CYCLES(0xd813, 12);
                s_x = ram_peek(0x0075);
      /*$D815*/ s_a = ram_peek(0x0076);
      /*$D817*/ branchTarget = true; push16(0xd819); block_id = 1070;
      break;
    case 227:  // $D81A
      /*$D81A*/ CYCLES(0xd81a, 6);
                branchTarget = true; push16(0xd81c); block_id = 388;
      break;
    case 228:  // $D81D
      /*$D81D*/ CYCLES(0xd81d, 6);
                FUNC_CHRGET(0xd81f);
                branchTarget = true; block_id = 229;
      break;
    case 229:  // $D820
      /*$D820*/ CYCLES(0xd820, 6);
                branchTarget = true; push16(0xd822); block_id = 232;
      break;
    case 230:  // $D823
      /*$D823*/ CYCLES(0xd823, 6);
                branchTarget = true; block_id = 216;
      break;
    case 231:  // $D826
      /*$D826*/ CYCLES(0xd826, 4);
                branchTarget = true; block_id = !s_status_not_z ? 254 : 232;
      break;
    case 232:  // $D828
      /*$D828*/ CYCLES(0xd828, 4);
                branchTarget = true; block_id = !s_status_not_z ? 242 : 233;
      break;
    case 233:  // $D82A
      /*$D82A*/ CYCLES(0xd82a, 7);
                tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 - 0x0080) - (uint8_t)(0x01 - s_status_c);
                tmp6_U8 = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_c = tmp6_U8;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0xff7f);
                s_a = ((uint8_t)tmp4_U16);
      /*$D82C*/ branchTarget = true; block_id = !tmp6_U8 ? 236 : 234;
      break;
    case 234:  // $D82E
      /*$D82E*/ CYCLES(0xd82e, 7);
                tmp6_U8 = s_a >= 0x40;
                s_status_c = tmp6_U8;
      /*$D830*/ branchTarget = true; block_id = tmp6_U8 ? 238 : 235;
      break;
    case 235:  // $D832
      /*$D832*/ CYCLES(0xd832, 23);
                tmp4_U16 = s_a << 0x01;
      /*$D833*/ s_y = ((uint8_t)tmp4_U16);
      /*$D834*/ tmp6_U8 = ram_peek((0xd001 + (tmp4_U16 & 0x00ff)));
      /*$D837*/ push8(tmp6_U8);
      /*$D838*/ tmp1_U8 = ram_peek((0xd000 + (tmp4_U16 & 0x00ff)));
      /*$D83B*/ push8(tmp1_U8);
      /*$D83C*/ FUNC_CHRGET(0x0000);
                tmp2_U8 = pop8();
                tmp2_U8 = pop8();
                branchTarget = true; block_id = find_block_id_func_t001(0xd83b, ((tmp1_U8 + (tmp6_U8 << 8)) + 0x0001));
      break;
    case 236:  // $D83F
      /*$D83F*/ CYCLES(0xd83f, 6);
                branchTarget = true; block_id = 324;
      break;
    case 237:  // $D842
      /*$D842*/ CYCLES(0xd842, 7);
                tmp6_U8 = s_a;
                s_status_c = (tmp6_U8 >= 0x3a);
      /*$D844*/ branchTarget = true; block_id = !(tmp6_U8 != 0x3a) ? 223 : 238;
      break;
    case 238:  // $D846
      /*$D846*/ CYCLES(0xd846, 6);
                branchTarget = true; block_id = 568;
      break;
    case 239:  // $D849
      /*$D849*/ CYCLES(0xd849, 16);
      /*$D84C*/ tmp3_U16 = ram_peek(0x0067);
                tmp4_U16 = tmp3_U16 - 0x0001;
                tmp2_U8 = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_c = tmp2_U8;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0xfffe);
                s_a = ((uint8_t)tmp4_U16);
      /*$D84E*/ tmp1_U8 = ram_peek(0x0068);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
      /*$D850*/ branchTarget = true; block_id = tmp2_U8 ? 241 : 240;
      break;
    case 240:  // $D852
      /*$D852*/ CYCLES(0xd852, 2);
                tmp6_U8 = (uint8_t)(s_y - 0x01);
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_y = tmp6_U8;
                block_id = 241;
      break;
    case 241:  // $D853
      /*$D853*/ CYCLES(0xd853, 7);
                ram_poke(0x007d, s_a);
      /*$D855*/ ram_poke(0x007e, s_y);
                block_id = 242;
      break;
    case 242:  // $D857
      /*$D857*/ CYCLES(0xd857, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd857, pop16() + 1);;
      break;
    case 243:  // $D858
      /*$D858*/ CYCLES(0xd858, 12);
                tmp6_U8 = io_peek(0xc000);
                s_a = tmp6_U8;
      /*$D85B*/ tmp1_U8 = tmp6_U8 != 0x83;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp6_U8 >= 0x83);
                s_status_n = ((uint8_t)(tmp6_U8 - 0x83) & 0x80);
      /*$D85D*/ branchTarget = true; block_id = !tmp1_U8 ? 245 : 244;
      break;
    case 244:  // $D85F
      /*$D85F*/ CYCLES(0xd85f, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd85f, pop16() + 1);;
      break;
    case 245:  // $D860
      /*$D860*/ CYCLES(0xd860, 6);
                branchTarget = true; push16(0xd862); block_id = 92;
      break;
    case 246:  // $D863
      /*$D863*/ CYCLES(0xd863, 11);
                s_x = 0xff;
      /*$D865*/ tmp6_U8 = ram_peek(0x00d8);
                s_status_v = ((tmp6_U8 >> 0x06) & 0x01);
      /*$D867*/ branchTarget = true; block_id = !(tmp6_U8 & 0x80) ? 248 : 247;
      break;
    case 247:  // $D869
      /*$D869*/ CYCLES(0xd869, 6);
                branchTarget = true; block_id = 1192;
      break;
    case 248:  // $D86C
      /*$D86C*/ CYCLES(0xd86c, 7);
                tmp6_U8 = s_a;
                s_status_not_z = (tmp6_U8 != 0x03);
                tmp1_U8 = tmp6_U8 >= 0x03;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp6_U8 - 0x03) & 0x80);
      /*$D86E*/ branchTarget = true; block_id = tmp1_U8 ? 250 : 249;
      break;
    case 249:  // $D870
      /*$D870*/ CYCLES(0xd870, 2);
                s_status_c = 0x00;
                block_id = 250;
      break;
    case 250:  // $D871
      /*$D871*/ CYCLES(0xd871, 4);
                branchTarget = true; block_id = s_status_not_z ? 257 : 251;
      break;
    case 251:  // $D873
      /*$D873*/ CYCLES(0xd873, 16);
                s_a = ram_peek(0x00b8);
      /*$D875*/ s_y = ram_peek(0x00b9);
      /*$D879*/ tmp1_U8 = (uint8_t)(ram_peek(0x0076) + 0x01);
                s_x = tmp1_U8;
      /*$D87A*/ branchTarget = true; block_id = !tmp1_U8 ? 253 : 252;
      break;
    case 252:  // $D87C
      /*$D87C*/ CYCLES(0xd87c, 21);
                ram_poke(0x0079, s_a);
      /*$D87E*/ ram_poke(0x007a, s_y);
      /*$D884*/ ram_poke(0x0077, ram_peek(0x0075));
      /*$D886*/ ram_poke(0x0078, ram_peek(0x0076));
                block_id = 253;
      break;
    case 253:  // $D888
      /*$D888*/ CYCLES(0xd888, 4);
                tmp1_U8 = pop8();
      /*$D889*/ tmp1_U8 = pop8();
                block_id = 254;
      break;
    case 254:  // $D88A
      /*$D88A*/ CYCLES(0xd88a, 11);
                s_a = 0x5d;
      /*$D88C*/ s_y = 0xd3;
      /*$D88E*/ branchTarget = true; block_id = !s_status_c ? 256 : 255;
      break;
    case 255:  // $D890
      /*$D890*/ CYCLES(0xd890, 6);
                branchTarget = true; block_id = 52;
      break;
    case 256:  // $D893
      /*$D893*/ CYCLES(0xd893, 6);
                branchTarget = true; block_id = 55;
      break;
    case 257:  // $D8AF
      /*$D8AF*/ CYCLES(0xd8af, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd8af, pop16() + 1);;
      break;
    case 258:  // $D912
      /*$D912*/ CYCLES(0xd912, 11);
                push8((s_status_c | ((s_status_not_z == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | s_status_n));
      /*$D913*/ ram_poke(0x0076, (uint8_t)(ram_peek(0x0076) - 0x01));
      /*$D915*/ tmp2_U8 = pop8();
                s_status_c = (tmp2_U8 & 0x01);
                s_status_i = ((tmp2_U8 & 0x04) != 0);
                s_status_d = ((tmp2_U8 & 0x08) != 0);
                s_status_b = 0x00;
                s_status_v = ((tmp2_U8 & 0x40) != 0);
      /*$D916*/ branchTarget = true; block_id = (~tmp2_U8 & 2) ? 260 : 259;
      break;
    case 259:  // $D918
      /*$D918*/ CYCLES(0xd918, 6);
                branchTarget = true; block_id = 146;
      break;
    case 260:  // $D91B
      /*$D91B*/ CYCLES(0xd91b, 6);
                branchTarget = true; push16(0xd91d); block_id = 149;
      break;
    case 261:  // $D91E
      /*$D91E*/ CYCLES(0xd91e, 6);
                branchTarget = true; block_id = 264;
      break;
    case 262:  // $D921
      /*$D921*/ CYCLES(0xd921, 9);
                s_a = 0x03;
      /*$D923*/ branchTarget = true; push16(0xd925); block_id = 29;
      break;
    case 263:  // $D926
      /*$D926*/ CYCLES(0xd926, 26);
      /*$D928*/ push8(ram_peek(0x00b9));
      /*$D92B*/ push8(ram_peek(0x00b8));
      /*$D92E*/ push8(ram_peek(0x0076));
      /*$D931*/ push8(ram_peek(0x0075));
      /*$D934*/ push8(0xb0);
                block_id = 264;
      break;
    case 264:  // $D935
      /*$D935*/ CYCLES(0xd935, 6);
                FUNC_CHRGOT(0xd937);
                branchTarget = true; block_id = 265;
      break;
    case 265:  // $D938
      /*$D938*/ CYCLES(0xd938, 6);
                branchTarget = true; push16(0xd93a); block_id = 267;
      break;
    case 266:  // $D93B
      /*$D93B*/ CYCLES(0xd93b, 6);
                branchTarget = true; block_id = 216;
      break;
    case 267:  // $D93E
      /*$D93E*/ CYCLES(0xd93e, 6);
                branchTarget = true; push16(0xd940); block_id = 317;
      break;
    case 268:  // $D941
      /*$D941*/ CYCLES(0xd941, 6);
                branchTarget = true; push16(0xd943); block_id = 290;
      break;
    case 269:  // $D944
      /*$D944*/ CYCLES(0xd944, 11);
      /*$D948*/ branchTarget = true; block_id = (ram_peek(0x0076) >= ram_peek(0x0051)) ? 272 : 270;
      break;
    case 270:  // $D94A
      /*$D94A*/ CYCLES(0xd94a, 14);
      /*$D94C*/ tmp4_U16 = (s_y + ram_peek(0x00b8)) + 0x0001;
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = tmp1_U8;
                s_a = ((uint8_t)tmp4_U16);
      /*$D94E*/ s_x = ram_peek(0x00b9);
      /*$D950*/ branchTarget = true; block_id = !tmp1_U8 ? 273 : 271;
      break;
    case 271:  // $D952
      /*$D952*/ CYCLES(0xd952, 6);
                s_x = (uint8_t)(s_x + 0x01);
      /*$D953*/ branchTarget = true; block_id = s_status_c ? 273 : 272;
      break;
    case 272:  // $D955
      /*$D955*/ CYCLES(0xd955, 7);
                s_a = ram_peek(0x0067);
      /*$D957*/ s_x = ram_peek(0x0068);
                block_id = 273;
      break;
    case 273:  // $D959
      /*$D959*/ CYCLES(0xd959, 6);
                FUNC_FNDLIN2(0xd95b);
                branchTarget = true; block_id = 274;
      break;
    case 274:  // $D95C
      /*$D95C*/ CYCLES(0xd95c, 4);
                branchTarget = true; block_id = !s_status_c ? 281 : 275;
      break;
    case 275:  // $D95E
      /*$D95E*/ CYCLES(0xd95e, 21);
      /*$D960*/ tmp4_U16 = (ram_peek(0x009b) - 0x0001) - (uint8_t)(0x01 - s_status_c);
      /*$D962*/ ram_poke(0x00b8, ((uint8_t)tmp4_U16));
      /*$D966*/ tmp5_U16 = ram_peek(0x009c);
                tmp4_U16 = tmp5_U16 - ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0xffff);
                tmp1_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$D968*/ ram_poke(0x00b9, tmp1_U8);
                block_id = 276;
      break;
    case 276:  // $D96A
      /*$D96A*/ CYCLES(0xd96a, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd96a, pop16() + 1);;
      break;
    case 277:  // $D96B
      /*$D96B*/ CYCLES(0xd96b, 4);
                branchTarget = true; block_id = s_status_not_z ? 276 : 278;
      break;
    case 278:  // $D96D
      /*$D96D*/ CYCLES(0xd96d, 12);
      /*$D96F*/ ram_poke(0x0085, 0xff);
      /*$D971*/ FUNC_GETFORPTR(0xd973);
                branchTarget = true; block_id = 279;
      break;
    case 279:  // $D974
      /*$D974*/ CYCLES(0xd974, 9);
                s_sp = s_x;
      /*$D975*/ tmp6_U8 = s_a;
                s_status_c = (tmp6_U8 >= 0xb0);
      /*$D977*/ branchTarget = true; block_id = !(tmp6_U8 != 0xb0) ? 283 : 280;
      break;
    case 280:  // $D979
      /*$D979*/ CYCLES(0xd979, 14);
                s_x = 0x16;
      /*$D97E*/ branchTarget = true; block_id = 44;
      break;
    case 281:  // $D97C
      /*$D97C*/ CYCLES(0xd97c, 9);
                s_x = 0x5a;
      /*$D97E*/ branchTarget = true; block_id = 44;
      break;
    case 282:  // $D981
      /*$D981*/ CYCLES(0xd981, 6);
                branchTarget = true; block_id = 568;
      break;
    case 283:  // $D984
      /*$D984*/ CYCLES(0xd984, 11);
                tmp1_U8 = pop8();
      /*$D985*/ tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$D986*/ tmp1_U8 = s_y;
                s_status_c = (tmp1_U8 >= 0x42);
      /*$D988*/ branchTarget = true; block_id = !(tmp1_U8 != 0x42) ? 296 : 284;
      break;
    case 284:  // $D98A
      /*$D98A*/ CYCLES(0xd98a, 19);
                ram_poke(0x0075, s_a);
      /*$D98C*/ tmp1_U8 = pop8();
      /*$D98D*/ ram_poke(0x0076, tmp1_U8);
      /*$D98F*/ tmp1_U8 = pop8();
      /*$D990*/ ram_poke(0x00b8, tmp1_U8);
      /*$D992*/ tmp1_U8 = pop8();
      /*$D993*/ ram_poke(0x00b9, tmp1_U8);
                block_id = 285;
      break;
    case 285:  // $D995
      /*$D995*/ CYCLES(0xd995, 6);
                branchTarget = true; push16(0xd997); block_id = 289;
      break;
    case 286:  // $D998
      /*$D998*/ CYCLES(0xd998, 14);
      /*$D99A*/ tmp4_U16 = s_y;
                tmp5_U16 = ram_peek(0x00b8);
                tmp3_U16 = tmp4_U16 + tmp5_U16;
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = tmp1_U8;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp5_U16);
                tmp6_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$D99C*/ ram_poke(0x00b8, tmp6_U8);
      /*$D99E*/ branchTarget = true; block_id = !tmp1_U8 ? 288 : 287;
      break;
    case 287:  // $D9A0
      /*$D9A0*/ CYCLES(0xd9a0, 4);
                tmp1_U8 = (uint8_t)(ram_peek(0x00b9) + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x00b9, tmp1_U8);
                block_id = 288;
      break;
    case 288:  // $D9A2
      /*$D9A2*/ CYCLES(0xd9a2, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xd9a2, pop16() + 1);;
      break;
    case 289:  // $D9A3
      /*$D9A3*/ CYCLES(0xd9a3, 19);
      /*$D9A5*/ s_status_v = ((ram_peek(0x00a2) >> 0x06) & 0x01);
      /*$D9A8*/ ram_poke(0x000d, 0x3a);
      /*$D9AA*/ s_y = 0x00;
      /*$D9AC*/ ram_poke(0x000e, 0x00);
                block_id = 291;
      break;
    case 290:  // $D9A6
      /*$D9A6*/ CYCLES(0xd9a6, 14);
      /*$D9A8*/ ram_poke(0x000d, 0x00);
      /*$D9AA*/ s_y = 0x00;
      /*$D9AC*/ ram_poke(0x000e, 0x00);
                block_id = 291;
      break;
    case 291:  // $D9AE
      /*$D9AE*/ CYCLES(0xd9ae, 14);
      /*$D9B0*/ tmp1_U8 = ram_peek(0x000d);
                s_x = tmp1_U8;
      /*$D9B2*/ ram_poke(0x000d, ram_peek(0x000e));
      /*$D9B4*/ ram_poke(0x000e, tmp1_U8);
                block_id = 292;
      break;
    case 292:  // $D9B6
      /*$D9B6*/ CYCLES(0xd9b6, 7);
                tmp1_U8 = peek((ram_peek16al(0x00b8) + s_y));
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$D9B8*/ branchTarget = true; block_id = !tmp1_U8 ? 288 : 293;
      break;
    case 293:  // $D9BA
      /*$D9BA*/ CYCLES(0xd9ba, 7);
                tmp1_U8 = s_a;
                tmp6_U8 = ram_peek(0x000e);
                tmp2_U8 = tmp1_U8 != tmp6_U8;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp1_U8 >= tmp6_U8);
                s_status_n = ((uint8_t)(tmp1_U8 - tmp6_U8) & 0x80);
      /*$D9BC*/ branchTarget = true; block_id = !tmp2_U8 ? 288 : 294;
      break;
    case 294:  // $D9BE
      /*$D9BE*/ CYCLES(0xd9be, 9);
                s_y = (uint8_t)(s_y + 0x01);
      /*$D9BF*/ tmp2_U8 = s_a;
                tmp6_U8 = tmp2_U8 != 0x22;
                s_status_not_z = tmp6_U8;
                s_status_c = (tmp2_U8 >= 0x22);
      /*$D9C1*/ branchTarget = true; block_id = tmp6_U8 ? 292 : 295;
      break;
    case 295:  // $D9C3
      /*$D9C3*/ CYCLES(0xd9c3, 4);
                branchTarget = true; block_id = !s_status_not_z ? 291 : 296;
      break;
    case 296:  // $D9C5
      /*$D9C5*/ CYCLES(0xd9c5, 7);
                tmp1_U8 = pop8();
      /*$D9C6*/ tmp1_U8 = pop8();
      /*$D9C7*/ tmp1_U8 = pop8();
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$D9C8*/ branchTarget = true; block_id = find_block_id_func_t001(0xd9c8, pop16() + 1);;
      break;
    case 297:  // $D9C9
      /*$D9C9*/ CYCLES(0xd9c9, 6);
                branchTarget = true; push16(0xd9cb); block_id = 495;
      break;
    case 298:  // $D9CC
      /*$D9CC*/ CYCLES(0xd9cc, 6);
                FUNC_CHRGOT(0xd9ce);
                branchTarget = true; block_id = 299;
      break;
    case 299:  // $D9CF
      /*$D9CF*/ CYCLES(0xd9cf, 7);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0xab);
      /*$D9D1*/ branchTarget = true; block_id = !(tmp2_U8 != 0xab) ? 301 : 300;
      break;
    case 300:  // $D9D3
      /*$D9D3*/ CYCLES(0xd9d3, 9);
                s_a = 0xc4;
      /*$D9D5*/ branchTarget = true; push16(0xd9d7); block_id = 566;
      break;
    case 301:  // $D9D8
      /*$D9D8*/ CYCLES(0xd9d8, 7);
                tmp2_U8 = ram_peek(0x009d);
                s_status_not_z = tmp2_U8;
      /*$D9DA*/ branchTarget = true; block_id = tmp2_U8 ? 304 : 302;
      break;
    case 302:  // $D9DC
      /*$D9DC*/ CYCLES(0xd9dc, 6);
                branchTarget = true; push16(0xd9de); block_id = 290;
      break;
    case 303:  // $D9DF
      /*$D9DF*/ CYCLES(0xd9df, 4);
                branchTarget = true; block_id = !s_status_not_z ? 286 : 304;
      break;
    case 304:  // $D9E1
      /*$D9E1*/ CYCLES(0xd9e1, 6);
                FUNC_CHRGOT(0xd9e3);
                branchTarget = true; block_id = 305;
      break;
    case 305:  // $D9E4
      /*$D9E4*/ CYCLES(0xd9e4, 4);
                branchTarget = true; block_id = s_status_c ? 307 : 306;
      break;
    case 306:  // $D9E6
      /*$D9E6*/ CYCLES(0xd9e6, 6);
                branchTarget = true; block_id = 267;
      break;
    case 307:  // $D9E9
      /*$D9E9*/ CYCLES(0xd9e9, 6);
                branchTarget = true; block_id = 232;
      break;
    case 308:  // $D9EC
      /*$D9EC*/ CYCLES(0xd9ec, 6);
                branchTarget = true; push16(0xd9ee); block_id = 858;
      break;
    case 309:  // $D9EF
      /*$D9EF*/ CYCLES(0xd9ef, 9);
                tmp2_U8 = s_a;
                push8(tmp2_U8);
      /*$D9F0*/ s_status_c = (tmp2_U8 >= 0xb0);
      /*$D9F2*/ branchTarget = true; block_id = !(tmp2_U8 != 0xb0) ? 311 : 310;
      break;
    case 310:  // $D9F4
      /*$D9F4*/ CYCLES(0xd9f4, 7);
                tmp6_U8 = s_a;
                s_status_c = (tmp6_U8 >= 0xab);
      /*$D9F6*/ branchTarget = true; block_id = (tmp6_U8 != 0xab) ? 282 : 311;
      break;
    case 311:  // $D9F8
      /*$D9F8*/ CYCLES(0xd9f8, 7);
                tmp6_U8 = (uint8_t)(ram_peek(0x00a1) - 0x01);
                ram_poke(0x00a1, tmp6_U8);
      /*$D9FA*/ branchTarget = true; block_id = tmp6_U8 ? 313 : 312;
      break;
    case 312:  // $D9FC
      /*$D9FC*/ CYCLES(0xd9fc, 7);
                tmp6_U8 = pop8();
                s_a = tmp6_U8;
      /*$D9FD*/ branchTarget = true; block_id = 233;
      break;
    case 313:  // $DA00
      /*$DA00*/ CYCLES(0xda00, 6);
                FUNC_CHRGET(0xda02);
      /*$DA03*/ CYCLES(0xda03, 6);
                branchTarget = true; push16(0xda05); block_id = 317;
      break;
    case 314:  // $DA06
      /*$DA06*/ CYCLES(0xda06, 7);
                tmp6_U8 = s_a;
                s_status_c = (tmp6_U8 >= 0x2c);
      /*$DA08*/ branchTarget = true; block_id = !(tmp6_U8 != 0x2c) ? 311 : 315;
      break;
    case 315:  // $DA0A
      /*$DA0A*/ CYCLES(0xda0a, 2);
                tmp6_U8 = pop8();
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
                block_id = 316;
      break;
    case 316:  // $DA0B
      /*$DA0B*/ CYCLES(0xda0b, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xda0b, pop16() + 1);;
      break;
    case 317:  // $DA0C
      /*$DA0C*/ CYCLES(0xda0c, 11);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_x = 0x00;
      /*$DA0E*/ ram_poke(0x0050, 0x00);
      /*$DA10*/ ram_poke(0x0051, 0x00);
                block_id = 318;
      break;
    case 318:  // $DA12
      /*$DA12*/ CYCLES(0xda12, 4);
                branchTarget = true; block_id = s_status_c ? 316 : 319;
      break;
    case 319:  // $DA14
      /*$DA14*/ CYCLES(0xda14, 21);
                tmp3_U16 = s_a;
                tmp5_U16 = (tmp3_U16 - 0x002f) - (uint8_t)(0x01 - s_status_c);
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)0xffd0);
      /*$DA16*/ ram_poke(0x000d, ((uint8_t)tmp5_U16));
      /*$DA18*/ tmp6_U8 = ram_peek(0x0051);
                s_a = tmp6_U8;
      /*$DA1A*/ ram_poke(0x005e, tmp6_U8);
      /*$DA1E*/ branchTarget = true; block_id = (tmp6_U8 >= 0x19) ? 310 : 320;
      break;
    case 320:  // $DA20
      /*$DA20*/ CYCLES(0xda20, 52);
      /*$DA22*/ tmp5_U16 = ram_peek(0x0050);
      /*$DA23*/ ram_poke(0x005e, (uint8_t)((ram_peek(0x005e) << 0x01) | (uint8_t)((tmp5_U16 << 0x01) >> 8)));
      /*$DA25*/ tmp5_U16 = tmp5_U16 << 0x02;
      /*$DA26*/ tmp3_U16 = (ram_peek(0x005e) << 0x01) | (uint8_t)((tmp5_U16 & 0x01ff) >> 8);
                ram_poke(0x005e, ((uint8_t)tmp3_U16));
      /*$DA28*/ tmp5_U16 = ((tmp5_U16 & 0x00ff) + ram_peek(0x0050)) + (uint8_t)(tmp3_U16 >> 8);
      /*$DA2A*/ ram_poke(0x0050, ((uint8_t)tmp5_U16));
      /*$DA30*/ ram_poke(0x0051, (uint8_t)((ram_peek(0x005e) + ram_peek(0x0051)) + (uint8_t)(tmp5_U16 >> 8)));
      /*$DA32*/ tmp5_U16 = ram_peek(0x0050) << 0x01;
                ram_poke(0x0050, ((uint8_t)tmp5_U16));
      /*$DA34*/ tmp5_U16 = (ram_peek(0x0051) << 0x01) | (uint8_t)(tmp5_U16 >> 8);
                ram_poke(0x0051, ((uint8_t)tmp5_U16));
      /*$DA38*/ tmp3_U16 = ram_peek(0x0050);
                tmp4_U16 = ram_peek(0x000d);
                tmp5_U16 = (tmp3_U16 + tmp4_U16) + (uint8_t)(tmp5_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
      /*$DA3A*/ ram_poke(0x0050, ((uint8_t)tmp5_U16));
      /*$DA3C*/ branchTarget = true; block_id = !(uint8_t)(tmp5_U16 >> 8) ? 322 : 321;
      break;
    case 321:  // $DA3E
      /*$DA3E*/ CYCLES(0xda3e, 4);
                ram_poke(0x0051, (uint8_t)(ram_peek(0x0051) + 0x01));
                block_id = 322;
      break;
    case 322:  // $DA40
      /*$DA40*/ CYCLES(0xda40, 6);
                FUNC_CHRGET(0xda42);
                branchTarget = true; block_id = 323;
      break;
    case 323:  // $DA43
      /*$DA43*/ CYCLES(0xda43, 6);
                branchTarget = true; block_id = 318;
      break;
    case 324:  // $DA46
      /*$DA46*/ CYCLES(0xda46, 6);
                branchTarget = true; push16(0xda48); block_id = 617;
      break;
    case 325:  // $DA49
      /*$DA49*/ CYCLES(0xda49, 16);
                ram_poke(0x0085, s_a);
      /*$DA4B*/ ram_poke(0x0086, s_y);
      /*$DA4D*/ s_a = 0xd0;
      /*$DA4F*/ branchTarget = true; push16(0xda51); block_id = 566;
      break;
    case 326:  // $DA52
      /*$DA52*/ CYCLES(0xda52, 16);
      /*$DA54*/ push8(ram_peek(0x0012));
      /*$DA57*/ push8(ram_peek(0x0011));
      /*$DA58*/ branchTarget = true; push16(0xda5a); block_id = 495;
      break;
    case 327:  // $DA5B
      /*$DA5B*/ CYCLES(0xda5b, 9);
                tmp6_U8 = pop8();
      /*$DA5C*/ tmp5_U16 = (tmp6_U8 << 0x01) | s_status_c;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                tmp6_U8 = (uint8_t)tmp5_U16;
                s_status_not_z = tmp6_U8;
                s_a = tmp6_U8;
      /*$DA5D*/ branchTarget = true; push16(0xda5f); block_id = 489;
      break;
    case 328:  // $DA60
      /*$DA60*/ CYCLES(0xda60, 4);
                branchTarget = true; block_id = s_status_not_z ? 335 : 329;
      break;
    case 329:  // $DA62
      /*$DA62*/ CYCLES(0xda62, 2);
                tmp2_U8 = pop8();
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 330;
      break;
    case 330:  // $DA63
      /*$DA63*/ CYCLES(0xda63, 4);
                branchTarget = true; block_id = !s_status_n ? 334 : 331;
      break;
    case 331:  // $DA65
      /*$DA65*/ CYCLES(0xda65, 6);
                branchTarget = true; push16(0xda67); block_id = 987;
      break;
    case 332:  // $DA68
      /*$DA68*/ CYCLES(0xda68, 6);
                branchTarget = true; push16(0xda6a); block_id = 668;
      break;
    case 333:  // $DA6B
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
    case 334:  // $DA77
      /*$DA77*/ CYCLES(0xda77, 6);
                branchTarget = true; block_id = 977;
      break;
    case 335:  // $DA7A
      /*$DA7A*/ CYCLES(0xda7a, 2);
                tmp2_U8 = pop8();
                block_id = 336;
      break;
    case 336:  // $DA7B
      /*$DA7B*/ CYCLES(0xda7b, 14);
                s_y = 0x02;
      /*$DA7D*/ tmp2_U8 = peek((ram_peek16al(0x00a0) + 0x0002));
      /*$DA7F*/ tmp6_U8 = ram_peek(0x0070);
                s_status_not_z = (tmp2_U8 != tmp6_U8);
      /*$DA81*/ branchTarget = true; block_id = !(tmp2_U8 >= tmp6_U8) ? 342 : 337;
      break;
    case 337:  // $DA83
      /*$DA83*/ CYCLES(0xda83, 4);
                branchTarget = true; block_id = s_status_not_z ? 339 : 338;
      break;
    case 338:  // $DA85
      /*$DA85*/ CYCLES(0xda85, 12);
      /*$DA86*/ tmp2_U8 = peek((ram_peek16al(0x00a0) + (uint8_t)(s_y - 0x01)));
      /*$DA8A*/ branchTarget = true; block_id = !(tmp2_U8 >= ram_peek(0x006f)) ? 342 : 339;
      break;
    case 339:  // $DA8C
      /*$DA8C*/ CYCLES(0xda8c, 11);
                tmp6_U8 = ram_peek(0x00a1);
      /*$DA8E*/ tmp2_U8 = ram_peek(0x006a);
                s_status_not_z = (tmp6_U8 != tmp2_U8);
                tmp2_U8 = tmp6_U8 >= tmp2_U8;
                s_status_c = tmp2_U8;
      /*$DA90*/ branchTarget = true; block_id = !tmp2_U8 ? 342 : 340;
      break;
    case 340:  // $DA92
      /*$DA92*/ CYCLES(0xda92, 4);
                branchTarget = true; block_id = s_status_not_z ? 343 : 341;
      break;
    case 341:  // $DA94
      /*$DA94*/ CYCLES(0xda94, 11);
      /*$DA96*/ tmp2_U8 = ram_peek(0x00a0) >= ram_peek(0x0069);
                s_status_c = tmp2_U8;
      /*$DA98*/ branchTarget = true; block_id = tmp2_U8 ? 343 : 342;
      break;
    case 342:  // $DA9A
      /*$DA9A*/ CYCLES(0xda9a, 12);
                s_a = ram_peek(0x00a0);
      /*$DA9C*/ s_y = ram_peek(0x00a1);
      /*$DA9E*/ branchTarget = true; block_id = 346;
      break;
    case 343:  // $DAA1
      /*$DAA1*/ CYCLES(0xdaa1, 12);
      /*$DAA3*/ tmp2_U8 = peek(ram_peek16al(0x00a0));
                s_a = tmp2_U8;
      /*$DAA5*/ branchTarget = true; push16(0xdaa7); block_id = 759;
      break;
    case 344:  // $DAA8
      /*$DAA8*/ CYCLES(0xdaa8, 19);
      /*$DAAC*/ ram_poke(0x00ab, ram_peek(0x008c));
      /*$DAAE*/ ram_poke(0x00ac, ram_peek(0x008d));
      /*$DAB0*/ branchTarget = true; push16(0xdab2); block_id = 834;
      break;
    case 345:  // $DAB3
      /*$DAB3*/ CYCLES(0xdab3, 7);
                s_a = 0x9d;
      /*$DAB5*/ s_y = 0x00;
                block_id = 346;
      break;
    case 346:  // $DAB7
      /*$DAB7*/ CYCLES(0xdab7, 12);
                ram_poke(0x008c, s_a);
      /*$DAB9*/ ram_poke(0x008d, s_y);
      /*$DABB*/ FUNC_FRETMS(0xdabd);
                branchTarget = true; block_id = 347;
      break;
    case 347:  // $DABE
      /*$DABE*/ CYCLES(0xdabe, 29);
      /*$DAC0*/ tmp6_U8 = peek(ram_peek16al(0x008c));
      /*$DAC2*/ poke(ram_peek16(0x0085), tmp6_U8);
      /*$DAC5*/ tmp6_U8 = peek((ram_peek16al(0x008c) + 0x0001));
      /*$DAC7*/ poke((ram_peek16(0x0085) + 0x0001), tmp6_U8);
      /*$DAC9*/ s_y = 0x02;
      /*$DACA*/ tmp6_U8 = peek((ram_peek16al(0x008c) + 0x0002));
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$DACC*/ poke((ram_peek16(0x0085) + 0x0002), tmp6_U8);
      /*$DACE*/ branchTarget = true; block_id = find_block_id_func_t001(0xdace, pop16() + 1);;
      break;
    case 348:  // $DACF
      /*$DACF*/ CYCLES(0xdacf, 6);
                branchTarget = true; push16(0xdad1); block_id = 381;
      break;
    case 349:  // $DAD2
      /*$DAD2*/ CYCLES(0xdad2, 6);
                FUNC_CHRGOT(0xdad4);
                branchTarget = true; block_id = 350;
      break;
    case 350:  // $DAD5
      /*$DAD5*/ CYCLES(0xdad5, 4);
                branchTarget = true; block_id = !s_status_not_z ? 361 : 351;
      break;
    case 351:  // $DAD7
      /*$DAD7*/ CYCLES(0xdad7, 4);
                branchTarget = true; block_id = !s_status_not_z ? 363 : 352;
      break;
    case 352:  // $DAD9
      /*$DAD9*/ CYCLES(0xdad9, 7);
                tmp1_U8 = s_a;
                tmp2_U8 = tmp1_U8 != 0xc0;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp1_U8 >= 0xc0);
                s_status_n = ((uint8_t)(tmp1_U8 - 0xc0) & 0x80);
      /*$DADB*/ branchTarget = true; block_id = !tmp2_U8 ? 368 : 353;
      break;
    case 353:  // $DADD
      /*$DADD*/ CYCLES(0xdadd, 9);
                tmp2_U8 = s_a;
                tmp1_U8 = tmp2_U8 != 0xc3;
                s_status_not_z = tmp1_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - 0xc3) & 0x80);
      /*$DADF*/ s_status_c = 0x00;
      /*$DAE0*/ branchTarget = true; block_id = !tmp1_U8 ? 368 : 354;
      break;
    case 354:  // $DAE2
      /*$DAE2*/ CYCLES(0xdae2, 9);
      /*$DAE5*/ branchTarget = true; block_id = !(s_a != 0x2c) ? 364 : 355;
      break;
    case 355:  // $DAE7
      /*$DAE7*/ CYCLES(0xdae7, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x3b);
      /*$DAE9*/ branchTarget = true; block_id = !(tmp1_U8 != 0x3b) ? 376 : 356;
      break;
    case 356:  // $DAEB
      /*$DAEB*/ CYCLES(0xdaeb, 6);
                branchTarget = true; push16(0xdaed); block_id = 495;
      break;
    case 357:  // $DAEE
      /*$DAEE*/ CYCLES(0xdaee, 7);
                tmp1_U8 = ram_peek(0x0011);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
      /*$DAF0*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 348 : 358;
      break;
    case 358:  // $DAF2
      /*$DAF2*/ CYCLES(0xdaf2, 6);
                branchTarget = true; push16(0xdaf4); block_id = 1073;
      break;
    case 359:  // $DAF5
      /*$DAF5*/ CYCLES(0xdaf5, 6);
                branchTarget = true; push16(0xdaf7); block_id = 761;
      break;
    case 360:  // $DAF8
      /*$DAF8*/ CYCLES(0xdaf8, 6);
                branchTarget = true; block_id = 348;
      break;
    case 361:  // $DAFB
      /*$DAFB*/ CYCLES(0xdafb, 9);
                s_a = 0x0d;
      /*$DAFD*/ branchTarget = true; push16(0xdaff); block_id = 390;
      break;
    case 362:  // $DB00
      /*$DB00*/ CYCLES(0xdb00, 4);
                tmp1_U8 = s_a ^ 0xff;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 363;
      break;
    case 363:  // $DB02
      /*$DB02*/ CYCLES(0xdb02, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xdb02, pop16() + 1);;
      break;
    case 364:  // $DB03
      /*$DB03*/ CYCLES(0xdb03, 11);
                tmp1_U8 = ram_peek(0x0024);
                s_a = tmp1_U8;
      /*$DB05*/ s_status_not_z = (tmp1_U8 != 0x18);
                tmp1_U8 = tmp1_U8 >= 0x18;
                s_status_c = tmp1_U8;
      /*$DB07*/ branchTarget = true; block_id = !tmp1_U8 ? 367 : 365;
      break;
    case 365:  // $DB09
      /*$DB09*/ CYCLES(0xdb09, 6);
                branchTarget = true; push16(0xdb0b); block_id = 361;
      break;
    case 366:  // $DB0C
      /*$DB0C*/ CYCLES(0xdb0c, 4);
                branchTarget = true; block_id = s_status_not_z ? 376 : 367;
      break;
    case 367:  // $DB0E
      /*$DB0E*/ CYCLES(0xdb0e, 14);
                tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 + 0x0010) + s_status_c;
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = tmp1_U8;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x0010);
                tmp2_U8 = (uint8_t)tmp4_U16;
      /*$DB10*/ tmp6_U8 = tmp2_U8 & 0xf0;
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp6_U8;
      /*$DB12*/ ram_poke(0x0024, tmp6_U8);
      /*$DB14*/ branchTarget = true; block_id = !tmp1_U8 ? 376 : 368;
      break;
    case 368:  // $DB16
      /*$DB16*/ CYCLES(0xdb16, 7);
                push8((s_status_c | ((s_status_not_z == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | s_status_n));
      /*$DB17*/ branchTarget = true; push16(0xdb19); block_id = 857;
      break;
    case 369:  // $DB1A
      /*$DB1A*/ CYCLES(0xdb1a, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x29);
      /*$DB1C*/ branchTarget = true; block_id = !(tmp1_U8 != 0x29) ? 371 : 370;
      break;
    case 370:  // $DB1E
      /*$DB1E*/ CYCLES(0xdb1e, 6);
                branchTarget = true; block_id = 568;
      break;
    case 371:  // $DB21
      /*$DB21*/ CYCLES(0xdb21, 6);
                tmp1_U8 = pop8();
                tmp6_U8 = tmp1_U8 & 0x01;
                s_status_c = tmp6_U8;
                s_status_i = ((tmp1_U8 & 0x04) != 0);
                s_status_d = ((tmp1_U8 & 0x08) != 0);
                s_status_b = 0x00;
                s_status_v = ((tmp1_U8 & 0x40) != 0);
      /*$DB22*/ branchTarget = true; block_id = !tmp6_U8 ? 374 : 372;
      break;
    case 372:  // $DB24
      /*$DB24*/ CYCLES(0xdb24, 11);
                tmp6_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp6_U8;
      /*$DB26*/ tmp4_U16 = tmp6_U8;
                tmp5_U16 = ram_peek(0x0024);
                tmp3_U16 = (tmp4_U16 - tmp5_U16) - (uint8_t)(0x01 - s_status_c);
                tmp6_U8 = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_c = tmp6_U8;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)(~tmp5_U16));
                s_a = ((uint8_t)tmp3_U16);
      /*$DB28*/ branchTarget = true; block_id = !tmp6_U8 ? 376 : 373;
      break;
    case 373:  // $DB2A
      /*$DB2A*/ CYCLES(0xdb2a, 2);
                s_x = s_a;
                block_id = 374;
      break;
    case 374:  // $DB2B
      /*$DB2B*/ CYCLES(0xdb2b, 2);
                s_x = (uint8_t)(s_x + 0x01);
                block_id = 375;
      break;
    case 375:  // $DB2C
      /*$DB2C*/ CYCLES(0xdb2c, 6);
                tmp6_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp6_U8;
                s_x = tmp6_U8;
      /*$DB2D*/ branchTarget = true; block_id = tmp6_U8 ? 378 : 376;
      break;
    case 376:  // $DB2F
      /*$DB2F*/ CYCLES(0xdb2f, 6);
                FUNC_CHRGET(0xdb31);
                branchTarget = true; block_id = 377;
      break;
    case 377:  // $DB32
      /*$DB32*/ CYCLES(0xdb32, 6);
                branchTarget = true; block_id = 351;
      break;
    case 378:  // $DB35
      /*$DB35*/ CYCLES(0xdb35, 6);
                branchTarget = true; push16(0xdb37); block_id = 388;
      break;
    case 379:  // $DB38
      /*$DB38*/ CYCLES(0xdb38, 4);
                branchTarget = true; block_id = s_status_not_z ? 375 : 380;
      break;
    case 380:  // $DB3A
      /*$DB3A*/ CYCLES(0xdb3a, 6);
                branchTarget = true; push16(0xdb3c); block_id = 761;
      break;
    case 381:  // $DB3D
      /*$DB3D*/ CYCLES(0xdb3d, 6);
                branchTarget = true; push16(0xdb3f); block_id = 843;
      break;
    case 382:  // $DB40
      /*$DB40*/ CYCLES(0xdb40, 7);
      /*$DB41*/ s_y = 0x00;
      /*$DB43*/ s_x = (uint8_t)(s_a + 0x01);
                block_id = 383;
      break;
    case 383:  // $DB44
      /*$DB44*/ CYCLES(0xdb44, 6);
                tmp6_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_x = tmp6_U8;
      /*$DB45*/ branchTarget = true; block_id = !tmp6_U8 ? 363 : 384;
      break;
    case 384:  // $DB47
      /*$DB47*/ CYCLES(0xdb47, 9);
                tmp6_U8 = peek((ram_peek16al(0x005e) + s_y));
                s_a = tmp6_U8;
      /*$DB49*/ branchTarget = true; push16(0xdb4b); block_id = 390;
      break;
    case 385:  // $DB4C
      /*$DB4C*/ CYCLES(0xdb4c, 9);
                s_y = (uint8_t)(s_y + 0x01);
      /*$DB4D*/ tmp6_U8 = s_a;
                s_status_c = (tmp6_U8 >= 0x0d);
      /*$DB4F*/ branchTarget = true; block_id = (tmp6_U8 != 0x0d) ? 383 : 386;
      break;
    case 386:  // $DB51
      /*$DB51*/ CYCLES(0xdb51, 6);
                branchTarget = true; push16(0xdb53); block_id = 362;
      break;
    case 387:  // $DB54
      /*$DB54*/ CYCLES(0xdb54, 6);
                branchTarget = true; block_id = 383;
      break;
    case 388:  // $DB57
      /*$DB57*/ CYCLES(0xdb57, 9);
                s_a = 0x20;
      /*$DB59*/ s_status_v = ((ram_peek(0x3fa9) >> 0x06) & 0x01);
                block_id = 390;
      break;
    case 389:  // $DB5A
      /*$DB5A*/ CYCLES(0xdb5a, 4);
                s_a = 0x3f;
                block_id = 390;
      break;
    case 390:  // $DB5C
      /*$DB5C*/ CYCLES(0xdb5c, 11);
                tmp2_U8 = s_a | 0x80;
                s_a = tmp2_U8;
      /*$DB5E*/ s_status_not_z = (tmp2_U8 != 0xa0);
                tmp1_U8 = tmp2_U8 >= 0xa0;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - 0xa0) & 0x80);
      /*$DB60*/ branchTarget = true; block_id = !tmp1_U8 ? 392 : 391;
      break;
    case 391:  // $DB62
      /*$DB62*/ CYCLES(0xdb62, 4);
                tmp6_U8 = s_a | ram_peek(0x00f3);
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
                block_id = 392;
      break;
    case 392:  // $DB64
      /*$DB64*/ CYCLES(0xdb64, 6);
                branchTarget = true; push16(0xdb66); block_id = 1594;
      break;
    case 393:  // $DB67
      /*$DB67*/ CYCLES(0xdb67, 14);
      /*$DB69*/ push8((s_a & 0x7f));
      /*$DB6A*/ s_a = ram_peek(0x00f1);
      /*$DB6C*/ branchTarget = true; push16(0xdb6e); block_id = 1522;
      break;
    case 394:  // $DB6F
      /*$DB6F*/ CYCLES(0xdb6f, 4);
                tmp6_U8 = pop8();
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$DB70*/ branchTarget = true; block_id = find_block_id_func_t001(0xdb70, pop16() + 1);;
      break;
    case 395:  // $DB75
      /*$DB75*/ CYCLES(0xdb75, 4);
                branchTarget = true; block_id = s_status_n ? 397 : 396;
      break;
    case 396:  // $DB77
      /*$DB77*/ CYCLES(0xdb77, 7);
                s_y = 0xff;
      /*$DB79*/ branchTarget = true; block_id = 398;
      break;
    case 397:  // $DB7B
      /*$DB7B*/ CYCLES(0xdb7b, 7);
                s_a = ram_peek(0x007b);
      /*$DB7D*/ s_y = ram_peek(0x007c);
                block_id = 398;
      break;
    case 398:  // $DB7F
      /*$DB7F*/ CYCLES(0xdb7f, 12);
                ram_poke(0x0075, s_a);
      /*$DB81*/ ram_poke(0x0076, s_y);
      /*$DB83*/ branchTarget = true; block_id = 568;
      break;
    case 399:  // $DB87
      /*$DB87*/ CYCLES(0xdb87, 7);
                tmp6_U8 = ram_peek(0x00d8);
                s_status_v = ((tmp6_U8 >> 0x06) & 0x01);
      /*$DB89*/ branchTarget = true; block_id = !(tmp6_U8 & 0x80) ? 401 : 400;
      break;
    case 400:  // $DB8B
      /*$DB8B*/ CYCLES(0xdb8b, 9);
                s_x = 0xfe;
      /*$DB8D*/ branchTarget = true; block_id = 1192;
      break;
    case 401:  // $DB90
      /*$DB90*/ CYCLES(0xdb90, 12);
                s_a = 0xef;
      /*$DB92*/ s_y = 0xdc;
      /*$DB94*/ branchTarget = true; push16(0xdb96); block_id = 380;
      break;
    case 402:  // $DB97
      /*$DB97*/ CYCLES(0xdb97, 16);
                tmp6_U8 = ram_peek(0x0079);
                s_a = tmp6_U8;
      /*$DB99*/ tmp1_U8 = ram_peek(0x007a);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
      /*$DB9B*/ ram_poke(0x00b8, tmp6_U8);
      /*$DB9D*/ ram_poke(0x00b9, tmp1_U8);
      /*$DB9F*/ branchTarget = true; block_id = find_block_id_func_t001(0xdb9f, pop16() + 1);;
      break;
    case 403:  // $DBA0
      /*$DBA0*/ CYCLES(0xdba0, 6);
                branchTarget = true; push16(0xdba2); block_id = 743;
      break;
    case 404:  // $DBA3
      /*$DBA3*/ CYCLES(0xdba3, 24);
                s_x = 0x01;
      /*$DBA5*/ s_y = 0x02;
      /*$DBA9*/ ram_poke(0x0201, 0x00);
      /*$DBAC*/ s_status_not_z = 0x40;
                s_status_n = 0x00;
                s_a = 0x40;
      /*$DBAE*/ branchTarget = true; push16(0xdbb0); block_id = 420;
      break;
    case 405:  // $DBB1
      /*$DBB1*/ CYCLES(0xdbb1, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xdbb1, pop16() + 1);;
      break;
    case 406:  // $DBB2
      /*$DBB2*/ CYCLES(0xdbb2, 7);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x22);
      /*$DBB4*/ branchTarget = true; block_id = (tmp2_U8 != 0x22) ? 411 : 407;
      break;
    case 407:  // $DBB6
      /*$DBB6*/ CYCLES(0xdbb6, 6);
                branchTarget = true; push16(0xdbb8); block_id = 548;
      break;
    case 408:  // $DBB9
      /*$DBB9*/ CYCLES(0xdbb9, 9);
                s_a = 0x3b;
      /*$DBBB*/ branchTarget = true; push16(0xdbbd); block_id = 566;
      break;
    case 409:  // $DBBE
      /*$DBBE*/ CYCLES(0xdbbe, 6);
                branchTarget = true; push16(0xdbc0); block_id = 381;
      break;
    case 410:  // $DBC1
      /*$DBC1*/ CYCLES(0xdbc1, 6);
                branchTarget = true; block_id = 412;
      break;
    case 411:  // $DBC4
      /*$DBC4*/ CYCLES(0xdbc4, 6);
                branchTarget = true; push16(0xdbc6); block_id = 389;
      break;
    case 412:  // $DBC7
      /*$DBC7*/ CYCLES(0xdbc7, 6);
                branchTarget = true; push16(0xdbc9); block_id = 743;
      break;
    case 413:  // $DBCA
      /*$DBCA*/ CYCLES(0xdbca, 14);
      /*$DBCC*/ ram_poke(0x01ff, 0x2c);
      /*$DBCF*/ branchTarget = true; push16(0xdbd1); block_id = 85;
      break;
    case 414:  // $DBD2
      /*$DBD2*/ CYCLES(0xdbd2, 12);
                tmp1_U8 = ram_peek(0x0200);
                s_a = tmp1_U8;
      /*$DBD7*/ branchTarget = true; block_id = (tmp1_U8 != 0x03) ? 419 : 415;
      break;
    case 415:  // $DBD9
      /*$DBD9*/ CYCLES(0xdbd9, 6);
                branchTarget = true; block_id = 246;
      break;
    case 416:  // $DBDC
      /*$DBDC*/ CYCLES(0xdbdc, 6);
                branchTarget = true; push16(0xdbde); block_id = 389;
      break;
    case 417:  // $DBDF
      /*$DBDF*/ CYCLES(0xdbdf, 6);
                branchTarget = true; block_id = 85;
      break;
    case 418:  // $DBE2
      /*$DBE2*/ CYCLES(0xdbe2, 16);
                s_x = ram_peek(0x007d);
      /*$DBE4*/ s_y = ram_peek(0x007e);
      /*$DBE6*/ s_a = 0x98;
      /*$DBE8*/ block_id = 420;
      break;
    case 419:  // $DBE9
      /*$DBE9*/ CYCLES(0xdbe9, 4);
                s_a = 0x00;
                block_id = 420;
      break;
    case 420:  // $DBEB
      /*$DBEB*/ CYCLES(0xdbeb, 11);
                ram_poke(0x0015, s_a);
      /*$DBED*/ ram_poke(0x007f, s_x);
      /*$DBEF*/ ram_poke(0x0080, s_y);
                block_id = 421;
      break;
    case 421:  // $DBF1
      /*$DBF1*/ CYCLES(0xdbf1, 6);
                branchTarget = true; push16(0xdbf3); block_id = 617;
      break;
    case 422:  // $DBF4
      /*$DBF4*/ CYCLES(0xdbf4, 40);
                ram_poke(0x0085, s_a);
      /*$DBF6*/ ram_poke(0x0086, s_y);
      /*$DBFC*/ ram_poke(0x0087, ram_peek(0x00b8));
      /*$DBFE*/ ram_poke(0x0088, ram_peek(0x00b9));
      /*$DC00*/ tmp1_U8 = ram_peek(0x007f);
                s_x = tmp1_U8;
      /*$DC02*/ tmp6_U8 = ram_peek(0x0080);
                s_y = tmp6_U8;
      /*$DC04*/ ram_poke(0x00b8, tmp1_U8);
      /*$DC06*/ ram_poke(0x00b9, tmp6_U8);
      /*$DC08*/ FUNC_CHRGOT(0xdc0a);
                branchTarget = true; block_id = 423;
      break;
    case 423:  // $DC0B
      /*$DC0B*/ CYCLES(0xdc0b, 4);
                branchTarget = true; block_id = s_status_not_z ? 431 : 424;
      break;
    case 424:  // $DC0D
      /*$DC0D*/ CYCLES(0xdc0d, 7);
                tmp6_U8 = ram_peek(0x0015);
                s_status_n = (tmp6_U8 & 0x80);
                tmp6_U8 = (tmp6_U8 >> 0x06) & 0x01;
                s_status_v = tmp6_U8;
      /*$DC0F*/ branchTarget = true; block_id = !tmp6_U8 ? 427 : 425;
      break;
    case 425:  // $DC11
      /*$DC11*/ CYCLES(0xdc11, 6);
                branchTarget = true; push16(0xdc13); block_id = 1544;
      break;
    case 426:  // $DC14
      /*$DC14*/ CYCLES(0xdc14, 19);
      /*$DC16*/ ram_poke(0x0200, (s_a & 0x7f));
      /*$DC19*/ s_x = 0xff;
      /*$DC1B*/ s_y = 0x01;
      /*$DC1D*/ branchTarget = true; block_id = 430;
      break;
    case 427:  // $DC1F
      /*$DC1F*/ CYCLES(0xdc1f, 4);
                branchTarget = true; block_id = s_status_n ? 457 : 428;
      break;
    case 428:  // $DC21
      /*$DC21*/ CYCLES(0xdc21, 6);
                branchTarget = true; push16(0xdc23); block_id = 389;
      break;
    case 429:  // $DC24
      /*$DC24*/ CYCLES(0xdc24, 6);
                branchTarget = true; push16(0xdc26); block_id = 416;
      break;
    case 430:  // $DC27
      /*$DC27*/ CYCLES(0xdc27, 7);
                ram_poke(0x00b8, s_x);
      /*$DC29*/ ram_poke(0x00b9, s_y);
                block_id = 431;
      break;
    case 431:  // $DC2B
      /*$DC2B*/ CYCLES(0xdc2b, 6);
                FUNC_CHRGET(0xdc2d);
                branchTarget = true; block_id = 432;
      break;
    case 432:  // $DC2E
      /*$DC2E*/ CYCLES(0xdc2e, 7);
                tmp2_U8 = ram_peek(0x0011);
                s_status_v = ((tmp2_U8 >> 0x06) & 0x01);
      /*$DC30*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 444 : 433;
      break;
    case 433:  // $DC32
      /*$DC32*/ CYCLES(0xdc32, 7);
      /*$DC34*/ branchTarget = true; block_id = !((ram_peek(0x0015) >> 0x06) & 0x01) ? 435 : 434;
      break;
    case 434:  // $DC36
      /*$DC36*/ CYCLES(0xdc36, 16);
      /*$DC37*/ ram_poke(0x00b8, (uint8_t)(s_x + 0x01));
      /*$DC39*/ s_a = 0x00;
      /*$DC3B*/ ram_poke(0x000d, 0x00);
      /*$DC3D*/ branchTarget = true; block_id = 437;
      break;
    case 435:  // $DC3F
      /*$DC3F*/ CYCLES(0xdc3f, 11);
                tmp6_U8 = s_a;
                ram_poke(0x000d, tmp6_U8);
      /*$DC41*/ s_status_c = (tmp6_U8 >= 0x22);
      /*$DC43*/ branchTarget = true; block_id = !(tmp6_U8 != 0x22) ? 438 : 436;
      break;
    case 436:  // $DC45
      /*$DC45*/ CYCLES(0xdc45, 11);
      /*$DC47*/ ram_poke(0x000d, 0x3a);
      /*$DC49*/ s_a = 0x2c;
                block_id = 437;
      break;
    case 437:  // $DC4B
      /*$DC4B*/ CYCLES(0xdc4b, 2);
                s_status_c = 0x00;
                block_id = 438;
      break;
    case 438:  // $DC4C
      /*$DC4C*/ CYCLES(0xdc4c, 18);
                ram_poke(0x000e, s_a);
      /*$DC50*/ s_y = ram_peek(0x00b9);
      /*$DC52*/ tmp5_U16 = ram_peek(0x00b8);
                tmp3_U16 = tmp5_U16 + s_status_c;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp5_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp3_U16);
      /*$DC54*/ branchTarget = true; block_id = !(uint8_t)(tmp3_U16 >> 8) ? 440 : 439;
      break;
    case 439:  // $DC56
      /*$DC56*/ CYCLES(0xdc56, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 440;
      break;
    case 440:  // $DC57
      /*$DC57*/ CYCLES(0xdc57, 6);
                branchTarget = true; push16(0xdc59); block_id = 762;
      break;
    case 441:  // $DC5A
      /*$DC5A*/ CYCLES(0xdc5a, 6);
                FUNC_POINT(0xdc5c);
                branchTarget = true; block_id = 442;
      break;
    case 442:  // $DC5D
      /*$DC5D*/ CYCLES(0xdc5d, 6);
                branchTarget = true; push16(0xdc5f); block_id = 336;
      break;
    case 443:  // $DC60
      /*$DC60*/ CYCLES(0xdc60, 6);
                branchTarget = true; block_id = 447;
      break;
    case 444:  // $DC63
      /*$DC63*/ CYCLES(0xdc63, 11);
                push8(s_a);
      /*$DC67*/ branchTarget = true; block_id = !ram_peek(0x0200) ? 455 : 445;
      break;
    case 445:  // $DC69
      /*$DC69*/ CYCLES(0xdc69, 7);
                tmp6_U8 = pop8();
                s_a = tmp6_U8;
      /*$DC6A*/ branchTarget = true; push16(0xdc6c); block_id = 1022;
      break;
    case 446:  // $DC6D
      /*$DC6D*/ CYCLES(0xdc6d, 9);
                s_status_n = (ram_peek(0x0012) & 0x80);
      /*$DC6F*/ branchTarget = true; push16(0xdc71); block_id = 330;
      break;
    case 447:  // $DC72
      /*$DC72*/ CYCLES(0xdc72, 6);
                FUNC_CHRGOT(0xdc74);
                branchTarget = true; block_id = 448;
      break;
    case 448:  // $DC75
      /*$DC75*/ CYCLES(0xdc75, 4);
                branchTarget = true; block_id = !s_status_not_z ? 451 : 449;
      break;
    case 449:  // $DC77
      /*$DC77*/ CYCLES(0xdc77, 7);
                tmp6_U8 = s_a;
                s_status_c = (tmp6_U8 >= 0x2c);
      /*$DC79*/ branchTarget = true; block_id = !(tmp6_U8 != 0x2c) ? 451 : 450;
      break;
    case 450:  // $DC7B
      /*$DC7B*/ CYCLES(0xdc7b, 6);
      /*$DB71*/ CYCLES(0xdb71, 7);
                tmp6_U8 = ram_peek(0x0015);
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$DB73*/ branchTarget = true; block_id = !tmp6_U8 ? 399 : 395;
      break;
    case 451:  // $DC7E
      /*$DC7E*/ CYCLES(0xdc7e, 33);
      /*$DC82*/ ram_poke(0x007f, ram_peek(0x00b8));
      /*$DC84*/ ram_poke(0x0080, ram_peek(0x00b9));
      /*$DC88*/ tmp6_U8 = ram_peek(0x0088);
                s_y = tmp6_U8;
      /*$DC8A*/ ram_poke(0x00b8, ram_peek(0x0087));
      /*$DC8C*/ ram_poke(0x00b9, tmp6_U8);
      /*$DC8E*/ FUNC_CHRGOT(0xdc90);
                branchTarget = true; block_id = 452;
      break;
    case 452:  // $DC91
      /*$DC91*/ CYCLES(0xdc91, 4);
                branchTarget = true; block_id = !s_status_not_z ? 464 : 453;
      break;
    case 453:  // $DC93
      /*$DC93*/ CYCLES(0xdc93, 6);
                branchTarget = true; push16(0xdc95); block_id = 565;
      break;
    case 454:  // $DC96
      /*$DC96*/ CYCLES(0xdc96, 6);
                branchTarget = true; block_id = 421;
      break;
    case 455:  // $DC99
      /*$DC99*/ CYCLES(0xdc99, 7);
      /*$DC9B*/ branchTarget = true; block_id = ram_peek(0x0015) ? 445 : 456;
      break;
    case 456:  // $DC9D
      /*$DC9D*/ CYCLES(0xdc9d, 6);
      /*$DB86*/ CYCLES(0xdb86, 2);
                tmp1_U8 = pop8();
                block_id = 399;
      break;
    case 457:  // $DCA0
      /*$DCA0*/ CYCLES(0xdca0, 6);
                branchTarget = true; push16(0xdca2); block_id = 289;
      break;
    case 458:  // $DCA3
      /*$DCA3*/ CYCLES(0xdca3, 7);
                s_y = (uint8_t)(s_y + 0x01);
      /*$DCA5*/ branchTarget = true; block_id = s_a ? 461 : 459;
      break;
    case 459:  // $DCA7
      /*$DCA7*/ CYCLES(0xdca7, 12);
                s_x = 0x2a;
      /*$DCA9*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$DCAA*/ tmp1_U8 = peek((ram_peek16al(0x00b8) + tmp1_U8));
                s_status_not_z = tmp1_U8;
      /*$DCAC*/ branchTarget = true; block_id = !tmp1_U8 ? 475 : 460;
      break;
    case 460:  // $DCAE
      /*$DCAE*/ CYCLES(0xdcae, 19);
                tmp1_U8 = s_y;
      /*$DCAF*/ tmp6_U8 = peek((ram_peek16al(0x00b8) + (uint8_t)(tmp1_U8 + 0x01)));
      /*$DCB1*/ ram_poke(0x007b, tmp6_U8);
      /*$DCB4*/ tmp6_U8 = peek((ram_peek16al(0x00b8) + (uint8_t)(tmp1_U8 + 0x02)));
      /*$DCB6*/ s_y = (uint8_t)(tmp1_U8 + 0x03);
      /*$DCB7*/ ram_poke(0x007c, tmp6_U8);
                block_id = 461;
      break;
    case 461:  // $DCB9
      /*$DCB9*/ CYCLES(0xdcb9, 11);
                tmp1_U8 = peek((ram_peek16al(0x00b8) + s_y));
                s_a = tmp1_U8;
      /*$DCBB*/ s_x = tmp1_U8;
      /*$DCBC*/ branchTarget = true; push16(0xdcbe); block_id = 286;
      break;
    case 462:  // $DCBF
      /*$DCBF*/ CYCLES(0xdcbf, 7);
                tmp1_U8 = s_x;
                s_status_c = (tmp1_U8 >= 0x83);
      /*$DCC1*/ branchTarget = true; block_id = (tmp1_U8 != 0x83) ? 457 : 463;
      break;
    case 463:  // $DCC3
      /*$DCC3*/ CYCLES(0xdcc3, 6);
                branchTarget = true; block_id = 431;
      break;
    case 464:  // $DCC6
      /*$DCC6*/ CYCLES(0xdcc6, 14);
                s_a = ram_peek(0x007f);
      /*$DCC8*/ s_y = ram_peek(0x0080);
      /*$DCCA*/ tmp6_U8 = ram_peek(0x0015);
                s_status_not_z = tmp6_U8;
                tmp1_U8 = tmp6_U8 & 0x80;
                s_status_n = tmp1_U8;
                s_x = tmp6_U8;
      /*$DCCC*/ branchTarget = true; block_id = !tmp1_U8 ? 466 : 465;
      break;
    case 465:  // $DCCE
      /*$DCCE*/ CYCLES(0xdcce, 6);
                branchTarget = true; block_id = 241;
      break;
    case 466:  // $DCD1
      /*$DCD1*/ CYCLES(0xdcd1, 11);
                s_y = 0x00;
      /*$DCD3*/ tmp6_U8 = peek(ram_peek16(0x007f));
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$DCD5*/ branchTarget = true; block_id = !tmp6_U8 ? 468 : 467;
      break;
    case 467:  // $DCD7
      /*$DCD7*/ CYCLES(0xdcd7, 12);
                s_a = 0xdf;
      /*$DCD9*/ s_y = 0xdc;
      /*$DCDB*/ branchTarget = true; block_id = 380;
      break;
    case 468:  // $DCDE
      /*$DCDE*/ CYCLES(0xdcde, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xdcde, pop16() + 1);;
      break;
    case 469:  // $DCF9
      /*$DCF9*/ CYCLES(0xdcf9, 4);
                branchTarget = true; block_id = s_status_not_z ? 471 : 470;
      break;
    case 470:  // $DCFB
      /*$DCFB*/ CYCLES(0xdcfb, 7);
                s_y = 0x00;
      /*$DCFD*/ branchTarget = true; block_id = 472;
      break;
    case 471:  // $DCFF
      /*$DCFF*/ CYCLES(0xdcff, 6);
                branchTarget = true; push16(0xdd01); block_id = 617;
      break;
    case 472:  // $DD02
      /*$DD02*/ CYCLES(0xdd02, 12);
                ram_poke(0x0085, s_a);
      /*$DD04*/ ram_poke(0x0086, s_y);
      /*$DD06*/ FUNC_GETFORPTR(0xdd08);
                branchTarget = true; block_id = 473;
      break;
    case 473:  // $DD09
      /*$DD09*/ CYCLES(0xdd09, 4);
                branchTarget = true; block_id = !s_status_not_z ? 476 : 474;
      break;
    case 474:  // $DD0B
      /*$DD0B*/ CYCLES(0xdd0b, 4);
                s_status_not_z = 0x00;
                s_x = 0x00;
                block_id = 475;
      break;
    case 475:  // $DD0D
      /*$DD0D*/ CYCLES(0xdd0d, 4);
                branchTarget = true; block_id = !s_status_not_z ? 494 : 476;
      break;
    case 476:  // $DD0F
      /*$DD0F*/ CYCLES(0xdd0f, 33);
                tmp6_U8 = s_x;
                s_sp = tmp6_U8;
      /*$DD14*/ s_a = (uint8_t)(tmp6_U8 + 0x04);
      /*$DD1B*/ ram_poke(0x0060, (uint8_t)(tmp6_U8 + 0x0a));
      /*$DD1D*/ s_y = 0x01;
      /*$DD1F*/ FUNC_UPAY2FAC(0xdd21);
                branchTarget = true; block_id = 477;
      break;
    case 477:  // $DD22
      /*$DD22*/ CYCLES(0xdd22, 23);
      /*$DD26*/ ram_poke(0x00a2, ram_peek((0x0109 + s_sp)));
      /*$DD28*/ s_a = ram_peek(0x0085);
      /*$DD2A*/ s_y = ram_peek(0x0086);
      /*$DD2C*/ branchTarget = true; push16(0xdd2e); block_id = 878;
      break;
    case 478:  // $DD2F
      /*$DD2F*/ CYCLES(0xdd2f, 6);
                branchTarget = true; push16(0xdd31); block_id = 977;
      break;
    case 479:  // $DD32
      /*$DD32*/ CYCLES(0xdd32, 9);
                s_y = 0x01;
      /*$DD34*/ FUNC_FCOMP2(0xdd36);
                branchTarget = true; block_id = 480;
      break;
    case 480:  // $DD37
      /*$DD37*/ CYCLES(0xdd37, 12);
                tmp6_U8 = s_sp;
                s_x = tmp6_U8;
      /*$DD39*/ tmp3_U16 = s_a;
                tmp5_U16 = ram_peek((0x0109 + tmp6_U8));
                tmp4_U16 = tmp3_U16 - tmp5_U16;
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp5_U16));
      /*$DD3C*/ branchTarget = true; block_id = !((uint8_t)tmp4_U16) ? 483 : 481;
      break;
    case 481:  // $DD3E
      /*$DD3E*/ CYCLES(0xdd3e, 35);
                tmp6_U8 = s_x;
      /*$DD41*/ ram_poke(0x0075, ram_peek((0x010f + tmp6_U8)));
      /*$DD46*/ ram_poke(0x0076, ram_peek((0x0110 + tmp6_U8)));
      /*$DD4B*/ ram_poke(0x00b8, ram_peek((0x0112 + tmp6_U8)));
      /*$DD50*/ ram_poke(0x00b9, ram_peek((0x0111 + tmp6_U8)));
                block_id = 482;
      break;
    case 482:  // $DD52
      /*$DD52*/ CYCLES(0xdd52, 6);
                branchTarget = true; block_id = 216;
      break;
    case 483:  // $DD55
      /*$DD55*/ CYCLES(0xdd55, 14);
      /*$DD56*/ tmp4_U16 = s_x;
                tmp5_U16 = (tmp4_U16 + 0x0011) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x0011);
                tmp6_U8 = (uint8_t)tmp5_U16;
      /*$DD58*/ s_x = tmp6_U8;
      /*$DD59*/ s_sp = tmp6_U8;
      /*$DD5A*/ FUNC_CHRGOT(0xdd5c);
                branchTarget = true; block_id = 484;
      break;
    case 484:  // $DD5D
      /*$DD5D*/ CYCLES(0xdd5d, 7);
      /*$DD5F*/ branchTarget = true; block_id = (s_a != 0x2c) ? 482 : 485;
      break;
    case 485:  // $DD61
      /*$DD61*/ CYCLES(0xdd61, 6);
                FUNC_CHRGET(0xdd63);
      /*$DD64*/ CYCLES(0xdd64, 6);
                branchTarget = true; push16(0xdd66); block_id = 471;
      break;
    case 486:  // $DD67
      /*$DD67*/ CYCLES(0xdd67, 6);
                branchTarget = true; push16(0xdd69); block_id = 495;
      break;
    case 487:  // $DD6A
      /*$DD6A*/ CYCLES(0xdd6a, 6);
                s_status_c = 0x00;
      /*$DD6B*/ block_id = 489;
      break;
    case 488:  // $DD6C
      /*$DD6C*/ CYCLES(0xdd6c, 2);
                s_status_c = 0x01;
                block_id = 489;
      break;
    case 489:  // $DD6D
      /*$DD6D*/ CYCLES(0xdd6d, 7);
                tmp6_U8 = ram_peek(0x0011);
                tmp2_U8 = tmp6_U8 & 0x80;
                s_status_n = tmp2_U8;
                s_status_v = ((tmp6_U8 >> 0x06) & 0x01);
                s_status_not_z = (s_a & tmp6_U8);
      /*$DD6F*/ branchTarget = true; block_id = tmp2_U8 ? 492 : 490;
      break;
    case 490:  // $DD71
      /*$DD71*/ CYCLES(0xdd71, 4);
                branchTarget = true; block_id = s_status_c ? 493 : 491;
      break;
    case 491:  // $DD73
      /*$DD73*/ CYCLES(0xdd73, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xdd73, pop16() + 1);;
      break;
    case 492:  // $DD74
      /*$DD74*/ CYCLES(0xdd74, 4);
                branchTarget = true; block_id = s_status_c ? 491 : 493;
      break;
    case 493:  // $DD76
      /*$DD76*/ CYCLES(0xdd76, 4);
                s_x = 0xa3;
                block_id = 494;
      break;
    case 494:  // $DD78
      /*$DD78*/ CYCLES(0xdd78, 6);
                branchTarget = true; block_id = 44;
      break;
    case 495:  // $DD7B
      /*$DD7B*/ CYCLES(0xdd7b, 7);
      /*$DD7D*/ branchTarget = true; block_id = ram_peek(0x00b8) ? 497 : 496;
      break;
    case 496:  // $DD7F
      /*$DD7F*/ CYCLES(0xdd7f, 4);
                ram_poke(0x00b9, (uint8_t)(ram_peek(0x00b9) - 0x01));
                block_id = 497;
      break;
    case 497:  // $DD81
      /*$DD81*/ CYCLES(0xdd81, 23);
                ram_poke(0x00b8, (uint8_t)(ram_peek(0x00b8) - 0x01));
      /*$DD83*/ s_x = 0x00;
      /*$DD85*/ s_status_v = ((ram_peek(0x0048) >> 0x06) & 0x01);
      /*$DD88*/ push8(0x00);
      /*$DD89*/ s_a = 0x01;
      /*$DD8B*/ branchTarget = true; push16(0xdd8d); block_id = 29;
      break;
    case 498:  // $DD8E
      /*$DD8E*/ CYCLES(0xdd8e, 6);
                branchTarget = true; push16(0xdd90); block_id = 538;
      break;
    case 499:  // $DD91
      /*$DD91*/ CYCLES(0xdd91, 7);
      /*$DD93*/ ram_poke(0x0089, 0x00);
                block_id = 500;
      break;
    case 500:  // $DD95
      /*$DD95*/ CYCLES(0xdd95, 6);
                FUNC_CHRGOT(0xdd97);
                branchTarget = true; block_id = 501;
      break;
    case 501:  // $DD98
      /*$DD98*/ CYCLES(0xdd98, 9);
      /*$DD99*/ tmp4_U16 = s_a;
                tmp3_U16 = tmp4_U16 - 0x00cf;
                tmp2_U8 = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_c = tmp2_U8;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0xff30);
                s_a = ((uint8_t)tmp3_U16);
      /*$DD9B*/ branchTarget = true; block_id = !tmp2_U8 ? 506 : 502;
      break;
    case 502:  // $DD9D
      /*$DD9D*/ CYCLES(0xdd9d, 7);
                tmp6_U8 = s_a >= 0x03;
                s_status_c = tmp6_U8;
      /*$DD9F*/ branchTarget = true; block_id = tmp6_U8 ? 506 : 503;
      break;
    case 503:  // $DDA1
      /*$DDA1*/ CYCLES(0xdda1, 19);
                tmp6_U8 = s_a;
      /*$DDA6*/ tmp6_U8 = ((uint8_t)((tmp6_U8 << 0x01) | (tmp6_U8 >= 0x01)) ^ 0x01) ^ ram_peek(0x0089);
                s_a = tmp6_U8;
      /*$DDA8*/ tmp6_U8 = tmp6_U8 >= ram_peek(0x0089);
                s_status_c = tmp6_U8;
      /*$DDAA*/ branchTarget = true; block_id = !tmp6_U8 ? 526 : 504;
      break;
    case 504:  // $DDAC
      /*$DDAC*/ CYCLES(0xddac, 9);
                ram_poke(0x0089, s_a);
      /*$DDAE*/ FUNC_CHRGET(0xddb0);
                branchTarget = true; block_id = 505;
      break;
    case 505:  // $DDB1
      /*$DDB1*/ CYCLES(0xddb1, 6);
                branchTarget = true; block_id = 501;
      break;
    case 506:  // $DDB4
      /*$DDB4*/ CYCLES(0xddb4, 7);
                tmp6_U8 = ram_peek(0x0089);
                s_x = tmp6_U8;
      /*$DDB6*/ branchTarget = true; block_id = tmp6_U8 ? 519 : 507;
      break;
    case 507:  // $DDB8
      /*$DDB8*/ CYCLES(0xddb8, 4);
                branchTarget = true; block_id = s_status_c ? 531 : 508;
      break;
    case 508:  // $DDBA
      /*$DDBA*/ CYCLES(0xddba, 7);
                tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 + 0x0007) + s_status_c;
                tmp6_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = tmp6_U8;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x0007);
                s_a = ((uint8_t)tmp4_U16);
      /*$DDBC*/ branchTarget = true; block_id = !tmp6_U8 ? 531 : 509;
      break;
    case 509:  // $DDBE
      /*$DDBE*/ CYCLES(0xddbe, 7);
                tmp4_U16 = s_a;
                tmp5_U16 = ram_peek(0x0011);
                tmp3_U16 = (tmp4_U16 + tmp5_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp5_U16);
                tmp6_U8 = (uint8_t)tmp3_U16;
                s_a = tmp6_U8;
      /*$DDC0*/ branchTarget = true; block_id = tmp6_U8 ? 511 : 510;
      break;
    case 510:  // $DDC2
      /*$DDC2*/ CYCLES(0xddc2, 6);
      /*$E597*/ CYCLES(0xe597, 16);
      /*$E599*/ push8(ram_peek(0x00a1));
      /*$E59A*/ tmp6_U8 = ram_peek(0x00a0);
                s_a = tmp6_U8;
      /*$E59C*/ push8(tmp6_U8);
      /*$E59D*/ branchTarget = true; push16(0xe59f); block_id = 538;
      break;
    case 511:  // $DDC5
      /*$DDC5*/ CYCLES(0xddc5, 14);
                tmp3_U16 = (s_a + 0x00ff) + s_status_c;
      /*$DDC7*/ ram_poke(0x005e, ((uint8_t)tmp3_U16));
      /*$DDC9*/ tmp3_U16 = tmp3_U16 << 0x01;
      /*$DDCA*/ tmp5_U16 = tmp3_U16 & 0x00ff;
                tmp4_U16 = ram_peek(0x005e);
                tmp3_U16 = (tmp5_U16 + tmp4_U16) + (uint8_t)((tmp3_U16 & 0x01ff) >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp5_U16, (uint8_t)tmp4_U16);
      /*$DDCC*/ s_y = ((uint8_t)tmp3_U16);
                block_id = 512;
      break;
    case 512:  // $DDCD
      /*$DDCD*/ CYCLES(0xddcd, 11);
                tmp6_U8 = pop8();
                s_a = tmp6_U8;
      /*$DDCE*/ tmp6_U8 = tmp6_U8 >= ram_peek((0xd0b2 + s_y));
                s_status_c = tmp6_U8;
      /*$DDD1*/ branchTarget = true; block_id = tmp6_U8 ? 533 : 513;
      break;
    case 513:  // $DDD3
      /*$DDD3*/ CYCLES(0xddd3, 6);
                branchTarget = true; push16(0xddd5); block_id = 487;
      break;
    case 514:  // $DDD6
      /*$DDD6*/ CYCLES(0xddd6, 2);
                push8(s_a);
                block_id = 515;
      break;
    case 515:  // $DDD7
      /*$DDD7*/ CYCLES(0xddd7, 6);
                branchTarget = true; push16(0xddd9); block_id = 524;
      break;
    case 516:  // $DDDA
      /*$DDDA*/ CYCLES(0xddda, 9);
                tmp6_U8 = pop8();
                s_a = tmp6_U8;
      /*$DDDB*/ tmp6_U8 = ram_peek(0x0087);
                s_y = tmp6_U8;
      /*$DDDD*/ branchTarget = true; block_id = !(tmp6_U8 & 0x80) ? 522 : 517;
      break;
    case 517:  // $DDDF
      /*$DDDF*/ CYCLES(0xdddf, 6);
                tmp6_U8 = s_a;
                s_status_not_z = tmp6_U8;
                s_x = tmp6_U8;
      /*$DDE0*/ branchTarget = true; block_id = !tmp6_U8 ? 532 : 518;
      break;
    case 518:  // $DDE2
      /*$DDE2*/ CYCLES(0xdde2, 4);
                branchTarget = true; block_id = s_status_not_z ? 536 : 519;
      break;
    case 519:  // $DDE4
      /*$DDE4*/ CYCLES(0xdde4, 14);
                tmp6_U8 = ram_peek(0x0011);
                ram_poke(0x0011, (tmp6_U8 >> 0x01));
      /*$DDE7*/ s_a = (uint8_t)((s_x << 0x01) | (tmp6_U8 & 0x01));
      /*$DDE8*/ tmp6_U8 = ram_peek(0x00b8);
                s_x = tmp6_U8;
      /*$DDEA*/ branchTarget = true; block_id = tmp6_U8 ? 521 : 520;
      break;
    case 520:  // $DDEC
      /*$DDEC*/ CYCLES(0xddec, 4);
                ram_poke(0x00b9, (uint8_t)(ram_peek(0x00b9) - 0x01));
                block_id = 521;
      break;
    case 521:  // $DDEE
      /*$DDEE*/ CYCLES(0xddee, 14);
                ram_poke(0x00b8, (uint8_t)(ram_peek(0x00b8) - 0x01));
      /*$DDF0*/ s_y = 0x1b;
      /*$DDF2*/ ram_poke(0x0089, s_a);
      /*$DDF4*/ branchTarget = true; block_id = 512;
      break;
    case 522:  // $DDF6
      /*$DDF6*/ CYCLES(0xddf6, 9);
                tmp6_U8 = s_a >= ram_peek((0xd0b2 + s_y));
                s_status_c = tmp6_U8;
      /*$DDF9*/ branchTarget = true; block_id = tmp6_U8 ? 536 : 523;
      break;
    case 523:  // $DDFB
      /*$DDFB*/ CYCLES(0xddfb, 4);
                branchTarget = true; block_id = !s_status_c ? 514 : 524;
      break;
    case 524:  // $DDFD
      /*$DDFD*/ CYCLES(0xddfd, 19);
                tmp6_U8 = s_y;
      /*$DE00*/ push8(ram_peek((0xd0b4 + tmp6_U8)));
      /*$DE04*/ push8(ram_peek((0xd0b3 + tmp6_U8)));
      /*$DE05*/ branchTarget = true; push16(0xde07); block_id = 527;
      break;
    case 525:  // $DE08
      /*$DE08*/ CYCLES(0xde08, 9);
      /*$DD86*/ CYCLES(0xdd86, 14);
                push8(ram_peek(0x0089));
      /*$DD88*/ push8(s_x);
      /*$DD89*/ s_a = 0x01;
      /*$DD8B*/ branchTarget = true; push16(0xdd8d); block_id = 29;
      break;
    case 526:  // $DE0D
      /*$DE0D*/ CYCLES(0xde0d, 6);
                branchTarget = true; block_id = 568;
      break;
    case 527:  // $DE10
      /*$DE10*/ CYCLES(0xde10, 9);
                s_a = ram_peek(0x00a2);
      /*$DE12*/ s_x = ram_peek((0xd0b2 + s_y));
                block_id = 528;
      break;
    case 528:  // $DE15
      /*$DE15*/ CYCLES(0xde15, 19);
                tmp6_U8 = s_a;
                s_y = tmp6_U8;
      /*$DE16*/ tmp1_U8 = pop8();
      /*$DE17*/ ram_poke(0x005e, tmp1_U8);
      /*$DE19*/ ram_poke(0x005e, (uint8_t)(ram_peek(0x005e) + 0x01));
      /*$DE1B*/ tmp1_U8 = pop8();
      /*$DE1C*/ ram_poke(0x005f, tmp1_U8);
      /*$DE1F*/ push8(tmp6_U8);
                block_id = 529;
      break;
    case 529:  // $DE20
      /*$DE20*/ CYCLES(0xde20, 6);
                branchTarget = true; push16(0xde22); block_id = 987;
      break;
    case 530:  // $DE23
      /*$DE23*/ CYCLES(0xde23, 31);
      /*$DE25*/ push8(ram_peek(0x00a1));
      /*$DE28*/ push8(ram_peek(0x00a0));
      /*$DE2B*/ push8(ram_peek(0x009f));
      /*$DE2E*/ push8(ram_peek(0x009e));
      /*$DE2F*/ tmp6_U8 = ram_peek(0x009d);
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$DE31*/ push8(tmp6_U8);
      /*$DE32*/ branchTarget = true; block_id = find_block_id_func_t001(0xde32, ram_peek16al(0x005e));
      break;
    case 531:  // $DE35
      /*$DE35*/ CYCLES(0xde35, 6);
                s_y = 0xff;
      /*$DE37*/ tmp6_U8 = pop8();
                s_status_not_z = tmp6_U8;
                s_a = tmp6_U8;
                block_id = 532;
      break;
    case 532:  // $DE38
      /*$DE38*/ CYCLES(0xde38, 4);
                branchTarget = true; block_id = !s_status_not_z ? 537 : 533;
      break;
    case 533:  // $DE3A
      /*$DE3A*/ CYCLES(0xde3a, 7);
      /*$DE3C*/ branchTarget = true; block_id = !(s_a != 0x64) ? 535 : 534;
      break;
    case 534:  // $DE3E
      /*$DE3E*/ CYCLES(0xde3e, 6);
                branchTarget = true; push16(0xde40); block_id = 487;
      break;
    case 535:  // $DE41
      /*$DE41*/ CYCLES(0xde41, 4);
                ram_poke(0x0087, s_y);
                block_id = 536;
      break;
    case 536:  // $DE43
      /*$DE43*/ CYCLES(0xde43, 45);
                tmp6_U8 = pop8();
      /*$DE44*/ s_status_c = (tmp6_U8 & 0x01);
      /*$DE45*/ ram_poke(0x0016, (tmp6_U8 >> 0x01));
      /*$DE47*/ tmp6_U8 = pop8();
      /*$DE48*/ ram_poke(0x00a5, tmp6_U8);
      /*$DE4A*/ tmp6_U8 = pop8();
      /*$DE4B*/ ram_poke(0x00a6, tmp6_U8);
      /*$DE4D*/ tmp6_U8 = pop8();
      /*$DE4E*/ ram_poke(0x00a7, tmp6_U8);
      /*$DE50*/ tmp6_U8 = pop8();
      /*$DE51*/ ram_poke(0x00a8, tmp6_U8);
      /*$DE53*/ tmp6_U8 = pop8();
      /*$DE54*/ ram_poke(0x00a9, tmp6_U8);
      /*$DE56*/ tmp6_U8 = pop8();
      /*$DE57*/ ram_poke(0x00aa, tmp6_U8);
      /*$DE5B*/ ram_poke(0x00ab, (tmp6_U8 ^ ram_peek(0x00a2)));
                block_id = 537;
      break;
    case 537:  // $DE5D
      /*$DE5D*/ CYCLES(0xde5d, 6);
                tmp6_U8 = ram_peek(0x009d);
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$DE5F*/ branchTarget = true; block_id = find_block_id_func_t001(0xde5f, pop16() + 1);;
      break;
    case 538:  // $DE60
      /*$DE60*/ CYCLES(0xde60, 7);
      /*$DE62*/ ram_poke(0x0011, 0x00);
                block_id = 539;
      break;
    case 539:  // $DE64
      /*$DE64*/ CYCLES(0xde64, 6);
                FUNC_CHRGET(0xde66);
                branchTarget = true; block_id = 540;
      break;
    case 540:  // $DE67
      /*$DE67*/ CYCLES(0xde67, 4);
                branchTarget = true; block_id = s_status_c ? 542 : 541;
      break;
    case 541:  // $DE69
      /*$DE69*/ CYCLES(0xde69, 6);
                branchTarget = true; block_id = 1022;
      break;
    case 542:  // $DE6C
      /*$DE6C*/ CYCLES(0xde6c, 6);
                FUNC_ISLETC(0xde6e);
                branchTarget = true; block_id = 543;
      break;
    case 543:  // $DE6F
      /*$DE6F*/ CYCLES(0xde6f, 4);
                branchTarget = true; block_id = s_status_c ? 571 : 544;
      break;
    case 544:  // $DE71
      /*$DE71*/ CYCLES(0xde71, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x2e);
      /*$DE73*/ branchTarget = true; block_id = !(tmp1_U8 != 0x2e) ? 541 : 545;
      break;
    case 545:  // $DE75
      /*$DE75*/ CYCLES(0xde75, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0xc9);
      /*$DE77*/ branchTarget = true; block_id = !(tmp1_U8 != 0xc9) ? 569 : 546;
      break;
    case 546:  // $DE79
      /*$DE79*/ CYCLES(0xde79, 7);
      /*$DE7B*/ branchTarget = true; block_id = !(s_a != 0xc8) ? 539 : 547;
      break;
    case 547:  // $DE7D
      /*$DE7D*/ CYCLES(0xde7d, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x22);
      /*$DE7F*/ branchTarget = true; block_id = (tmp1_U8 != 0x22) ? 552 : 548;
      break;
    case 548:  // $DE81
      /*$DE81*/ CYCLES(0xde81, 14);
      /*$DE83*/ s_y = ram_peek(0x00b9);
      /*$DE85*/ tmp3_U16 = ram_peek(0x00b8);
                tmp5_U16 = tmp3_U16 + s_status_c;
                tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
                s_status_c = tmp1_U8;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp5_U16);
      /*$DE87*/ branchTarget = true; block_id = !tmp1_U8 ? 550 : 549;
      break;
    case 549:  // $DE89
      /*$DE89*/ CYCLES(0xde89, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 550;
      break;
    case 550:  // $DE8A
      /*$DE8A*/ CYCLES(0xde8a, 6);
                branchTarget = true; push16(0xde8c); block_id = 761;
      break;
    case 551:  // $DE8D
      /*$DE8D*/ CYCLES(0xde8d, 6);
                FUNC_POINT(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xde8d, pop16() + 1);;
      break;
    case 552:  // $DE90
      /*$DE90*/ CYCLES(0xde90, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0xc6);
      /*$DE92*/ branchTarget = true; block_id = (tmp1_U8 != 0xc6) ? 557 : 553;
      break;
    case 553:  // $DE94
      /*$DE94*/ CYCLES(0xde94, 7);
                s_y = 0x18;
      /*$DE96*/ branchTarget = true; block_id = 570;
      break;
    case 554:  // $DE98
      /*$DE98*/ CYCLES(0xde98, 7);
      /*$DE9A*/ branchTarget = true; block_id = ram_peek(0x009d) ? 556 : 555;
      break;
    case 555:  // $DE9C
      /*$DE9C*/ CYCLES(0xde9c, 14);
                s_y = 0x01;
      /*$DEA1*/ branchTarget = true; block_id = 742;
      break;
    case 556:  // $DE9F
      /*$DE9F*/ CYCLES(0xde9f, 9);
                s_y = 0x00;
      /*$DEA1*/ branchTarget = true; block_id = 742;
      break;
    case 557:  // $DEA4
      /*$DEA4*/ CYCLES(0xdea4, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0xc2);
      /*$DEA6*/ branchTarget = true; block_id = (tmp1_U8 != 0xc2) ? 559 : 558;
      break;
    case 558:  // $DEA8
      /*$DEA8*/ CYCLES(0xdea8, 6);
      /*$E354*/ CYCLES(0xe354, 6);
                branchTarget = true; push16(0xe356); block_id = 746;
      break;
    case 559:  // $DEAB
      /*$DEAB*/ CYCLES(0xdeab, 7);
      /*$DEAD*/ branchTarget = true; block_id = !(s_a >= 0xd2) ? 561 : 560;
      break;
    case 560:  // $DEAF
      /*$DEAF*/ CYCLES(0xdeaf, 6);
      /*$DF0C*/ CYCLES(0xdf0c, 7);
      /*$DF0E*/ branchTarget = true; block_id = !(s_a != 0xd7) ? 577 : 582;
      break;
    case 561:  // $DEB2
      /*$DEB2*/ CYCLES(0xdeb2, 6);
                branchTarget = true; push16(0xdeb4); block_id = 564;
      break;
    case 562:  // $DEB5
      /*$DEB5*/ CYCLES(0xdeb5, 6);
                branchTarget = true; push16(0xdeb7); block_id = 495;
      break;
    case 563:  // $DEB8
      /*$DEB8*/ CYCLES(0xdeb8, 14);
                s_a = 0x29;
      /*$DEBD*/ s_status_v = ((ram_peek(0x2ca9) >> 0x06) & 0x01);
                block_id = 566;
      break;
    case 564:  // $DEBB
      /*$DEBB*/ CYCLES(0xdebb, 9);
                s_a = 0x28;
      /*$DEBD*/ s_status_v = ((ram_peek(0x2ca9) >> 0x06) & 0x01);
                block_id = 566;
      break;
    case 565:  // $DEBE
      /*$DEBE*/ CYCLES(0xdebe, 4);
                s_a = 0x2c;
                block_id = 566;
      break;
    case 566:  // $DEC0
      /*$DEC0*/ CYCLES(0xdec0, 11);
                s_y = 0x00;
      /*$DEC2*/ tmp1_U8 = s_a;
                tmp6_U8 = peek(ram_peek16al(0x00b8));
                s_status_c = (tmp1_U8 >= tmp6_U8);
      /*$DEC4*/ branchTarget = true; block_id = (tmp1_U8 != tmp6_U8) ? 568 : 567;
      break;
    case 567:  // $DEC6
      /*$DEC6*/ CYCLES(0xdec6, 6);
                FUNC_CHRGET(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xdec6, pop16() + 1);;
      break;
    case 568:  // $DEC9
      /*$DEC9*/ CYCLES(0xdec9, 9);
                s_x = 0x10;
      /*$DECB*/ branchTarget = true; block_id = 44;
      break;
    case 569:  // $DECE
      /*$DECE*/ CYCLES(0xdece, 4);
                s_y = 0x15;
                block_id = 570;
      break;
    case 570:  // $DED0
      /*$DED0*/ CYCLES(0xded0, 9);
                tmp1_U8 = pop8();
      /*$DED1*/ tmp1_U8 = pop8();
      /*$DED2*/ branchTarget = true; block_id = 515;
      break;
    case 571:  // $DED5
      /*$DED5*/ CYCLES(0xded5, 6);
                branchTarget = true; push16(0xded7); block_id = 617;
      break;
    case 572:  // $DED8
      /*$DED8*/ CYCLES(0xded8, 14);
                ram_poke(0x00a0, s_a);
      /*$DEDA*/ ram_poke(0x00a1, s_y);
      /*$DEDE*/ branchTarget = true; block_id = !ram_peek(0x0011) ? 574 : 573;
      break;
    case 573:  // $DEE0
      /*$DEE0*/ CYCLES(0xdee0, 9);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_x = 0x00;
      /*$DEE2*/ ram_poke(0x00ac, 0x00);
      /*$DEE4*/ branchTarget = true; block_id = find_block_id_func_t001(0xdee4, pop16() + 1);;
      break;
    case 574:  // $DEE5
      /*$DEE5*/ CYCLES(0xdee5, 7);
                tmp1_U8 = ram_peek(0x0012);
                s_x = tmp1_U8;
      /*$DEE7*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 576 : 575;
      break;
    case 575:  // $DEE9
      /*$DEE9*/ CYCLES(0xdee9, 23);
      /*$DEEB*/ tmp1_U8 = peek(ram_peek16al(0x00a0));
      /*$DEEF*/ tmp6_U8 = peek((ram_peek16al(0x00a0) + 0x0001));
      /*$DEF1*/ s_y = tmp6_U8;
      /*$DEF2*/ s_a = tmp1_U8;
      /*$DEF3*/ branchTarget = true; block_id = 741;
      break;
    case 576:  // $DEF6
      /*$DEF6*/ CYCLES(0xdef6, 6);
                FUNC_UPAY2FAC(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xdef6, pop16() + 1);;
      break;
    case 577:  // $DEF9
      /*$DEF9*/ CYCLES(0xdef9, 6);
                FUNC_CHRGET(0xdefb);
                branchTarget = true; block_id = 578;
      break;
    case 578:  // $DEFC
      /*$DEFC*/ CYCLES(0xdefc, 6);
                branchTarget = true; push16(0xdefe); block_id = 1142;
      break;
    case 579:  // $DEFF
      /*$DEFF*/ CYCLES(0xdeff, 11);
                s_a = s_x;
      /*$DF00*/ s_y = ram_peek(0x00f0);
      /*$DF02*/ branchTarget = true; push16(0xdf04); block_id = 1366;
      break;
    case 580:  // $DF05
      /*$DF05*/ CYCLES(0xdf05, 7);
                s_y = s_a;
      /*$DF06*/ branchTarget = true; push16(0xdf08); block_id = 742;
      break;
    case 581:  // $DF09
      /*$DF09*/ CYCLES(0xdf09, 6);
                branchTarget = true; block_id = 563;
      break;
    case 582:  // $DF10
      /*$DF10*/ CYCLES(0xdf10, 11);
                tmp1_U8 = (uint8_t)(s_a << 0x01);
      /*$DF11*/ push8(tmp1_U8);
      /*$DF12*/ s_x = tmp1_U8;
      /*$DF13*/ FUNC_CHRGET(0xdf15);
                branchTarget = true; block_id = 583;
      break;
    case 583:  // $DF16
      /*$DF16*/ CYCLES(0xdf16, 7);
                tmp2_U8 = s_x >= 0xcf;
                s_status_c = tmp2_U8;
      /*$DF18*/ branchTarget = true; block_id = !tmp2_U8 ? 590 : 584;
      break;
    case 584:  // $DF1A
      /*$DF1A*/ CYCLES(0xdf1a, 6);
                branchTarget = true; push16(0xdf1c); block_id = 564;
      break;
    case 585:  // $DF1D
      /*$DF1D*/ CYCLES(0xdf1d, 6);
                branchTarget = true; push16(0xdf1f); block_id = 495;
      break;
    case 586:  // $DF20
      /*$DF20*/ CYCLES(0xdf20, 6);
                branchTarget = true; push16(0xdf22); block_id = 565;
      break;
    case 587:  // $DF23
      /*$DF23*/ CYCLES(0xdf23, 6);
                branchTarget = true; push16(0xdf25); block_id = 488;
      break;
    case 588:  // $DF26
      /*$DF26*/ CYCLES(0xdf26, 23);
                tmp1_U8 = pop8();
      /*$DF27*/ s_x = tmp1_U8;
      /*$DF2A*/ push8(ram_peek(0x00a1));
      /*$DF2D*/ push8(ram_peek(0x00a0));
      /*$DF2E*/ s_a = tmp1_U8;
      /*$DF2F*/ push8(tmp1_U8);
      /*$DF30*/ branchTarget = true; push16(0xdf32); block_id = 858;
      break;
    case 589:  // $DF33
      /*$DF33*/ CYCLES(0xdf33, 12);
                tmp1_U8 = pop8();
      /*$DF34*/ s_y = tmp1_U8;
      /*$DF36*/ push8(s_x);
      /*$DF37*/ branchTarget = true; block_id = 592;
      break;
    case 590:  // $DF3A
      /*$DF3A*/ CYCLES(0xdf3a, 6);
                branchTarget = true; push16(0xdf3c); block_id = 561;
      break;
    case 591:  // $DF3D
      /*$DF3D*/ CYCLES(0xdf3d, 4);
                tmp2_U8 = pop8();
      /*$DF3E*/ s_y = tmp2_U8;
                block_id = 592;
      break;
    case 592:  // $DF3F
      /*$DF3F*/ CYCLES(0xdf3f, 23);
                tmp1_U8 = s_y;
                tmp6_U8 = peek((0xcfdc + tmp1_U8));
      /*$DF42*/ ram_poke(0x0091, tmp6_U8);
      /*$DF44*/ tmp1_U8 = peek((0xcfdd + tmp1_U8));
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$DF47*/ ram_poke(0x0092, tmp1_U8);
      /*$DF49*/ branchTarget = true; push16(0xdf4b); block_id = 1;
      break;
    case 593:  // $DF4C
      /*$DF4C*/ CYCLES(0xdf4c, 6);
                branchTarget = true; block_id = 487;
      break;
    case 594:  // $DF65
      /*$DF65*/ CYCLES(0xdf65, 6);
                branchTarget = true; push16(0xdf67); block_id = 489;
      break;
    case 595:  // $DF68
      /*$DF68*/ CYCLES(0xdf68, 4);
                branchTarget = true; block_id = s_status_c ? 598 : 596;
      break;
    case 596:  // $DF6A
      /*$DF6A*/ CYCLES(0xdf6a, 26);
      /*$DF70*/ ram_poke(0x00a6, ((ram_peek(0x00aa) | 0x7f) & ram_peek(0x00a6)));
      /*$DF72*/ s_a = 0xa5;
      /*$DF74*/ s_y = 0x00;
      /*$DF76*/ FUNC_FCOMP(0xdf78);
                branchTarget = true; block_id = 597;
      break;
    case 597:  // $DF79
      /*$DF79*/ CYCLES(0xdf79, 7);
                tmp1_U8 = s_a;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$DF7A*/ branchTarget = true; block_id = 606;
      break;
    case 598:  // $DF7D
      /*$DF7D*/ CYCLES(0xdf7d, 16);
                s_a = 0x00;
      /*$DF7F*/ ram_poke(0x0011, 0x00);
      /*$DF81*/ ram_poke(0x0089, (uint8_t)(ram_peek(0x0089) - 0x01));
      /*$DF83*/ branchTarget = true; push16(0xdf85); block_id = 843;
      break;
    case 599:  // $DF86
      /*$DF86*/ CYCLES(0xdf86, 23);
                ram_poke(0x009d, s_a);
      /*$DF88*/ ram_poke(0x009e, s_x);
      /*$DF8A*/ ram_poke(0x009f, s_y);
      /*$DF8C*/ s_a = ram_peek(0x00a8);
      /*$DF8E*/ s_y = ram_peek(0x00a9);
      /*$DF90*/ branchTarget = true; push16(0xdf92); block_id = 844;
      break;
    case 600:  // $DF93
      /*$DF93*/ CYCLES(0xdf93, 18);
                ram_poke(0x00a8, s_x);
      /*$DF95*/ ram_poke(0x00a9, s_y);
      /*$DF97*/ tmp1_U8 = s_a;
                s_x = tmp1_U8;
      /*$DF99*/ tmp3_U16 = tmp1_U8 - ram_peek(0x009d);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                tmp1_U8 = (uint8_t)tmp3_U16;
                s_a = tmp1_U8;
      /*$DF9B*/ branchTarget = true; block_id = !tmp1_U8 ? 603 : 601;
      break;
    case 601:  // $DF9D
      /*$DF9D*/ CYCLES(0xdf9d, 7);
                s_a = 0x01;
      /*$DF9F*/ branchTarget = true; block_id = !s_status_c ? 603 : 602;
      break;
    case 602:  // $DFA1
      /*$DFA1*/ CYCLES(0xdfa1, 7);
                s_x = ram_peek(0x009d);
      /*$DFA3*/ s_a = 0xff;
                block_id = 603;
      break;
    case 603:  // $DFA5
      /*$DFA5*/ CYCLES(0xdfa5, 9);
                ram_poke(0x00a2, s_a);
      /*$DFA7*/ s_y = 0xff;
      /*$DFA9*/ s_x = (uint8_t)(s_x + 0x01);
                block_id = 604;
      break;
    case 604:  // $DFAA
      /*$DFAA*/ CYCLES(0xdfaa, 7);
                s_y = (uint8_t)(s_y + 0x01);
      /*$DFAB*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp1_U8;
      /*$DFAC*/ branchTarget = true; block_id = tmp1_U8 ? 608 : 605;
      break;
    case 605:  // $DFAE
      /*$DFAE*/ CYCLES(0xdfae, 4);
                tmp1_U8 = ram_peek(0x00a2);
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
                block_id = 606;
      break;
    case 606:  // $DFB0
      /*$DFB0*/ CYCLES(0xdfb0, 4);
                branchTarget = true; block_id = s_status_n ? 611 : 607;
      break;
    case 607:  // $DFB2
      /*$DFB2*/ CYCLES(0xdfb2, 6);
                s_status_c = 0x00;
      /*$DFB3*/ branchTarget = true; block_id = 611;
      break;
    case 608:  // $DFB5
      /*$DFB5*/ CYCLES(0xdfb5, 11);
                tmp1_U8 = s_y;
                tmp6_U8 = peek((ram_peek16al(0x00a8) + tmp1_U8));
      /*$DFB7*/ tmp1_U8 = peek((ram_peek16al(0x009e) + tmp1_U8));
                s_status_c = (tmp6_U8 >= tmp1_U8);
      /*$DFB9*/ branchTarget = true; block_id = !(tmp6_U8 != tmp1_U8) ? 604 : 609;
      break;
    case 609:  // $DFBB
      /*$DFBB*/ CYCLES(0xdfbb, 7);
                s_x = 0xff;
      /*$DFBD*/ branchTarget = true; block_id = s_status_c ? 611 : 610;
      break;
    case 610:  // $DFBF
      /*$DFBF*/ CYCLES(0xdfbf, 4);
                s_x = 0x01;
                block_id = 611;
      break;
    case 611:  // $DFC1
      /*$DFC1*/ CYCLES(0xdfc1, 12);
      /*$DFC3*/ tmp3_U16 = ((uint8_t)(s_x + 0x01) << 0x01) | s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
      /*$DFC4*/ tmp1_U8 = ((uint8_t)tmp3_U16) & ram_peek(0x0016);
                s_a = tmp1_U8;
      /*$DFC6*/ branchTarget = true; block_id = !tmp1_U8 ? 613 : 612;
      break;
    case 612:  // $DFC8
      /*$DFC8*/ CYCLES(0xdfc8, 4);
                s_a = 0x01;
                block_id = 613;
      break;
    case 613:  // $DFCA
      /*$DFCA*/ CYCLES(0xdfca, 6);
                branchTarget = true; block_id = 997;
      break;
    case 614:  // $DFCD
      /*$DFCD*/ CYCLES(0xdfcd, 6);
                branchTarget = true; push16(0xdfcf); block_id = 859;
      break;
    case 615:  // $DFD0
      /*$DFD0*/ CYCLES(0xdfd0, 6);
                FUNC_PREAD(0xdfd2);
                branchTarget = true; block_id = 616;
      break;
    case 616:  // $DFD3
      /*$DFD3*/ CYCLES(0xdfd3, 6);
                branchTarget = true; block_id = 742;
      break;
    case 617:  // $DFE3
      /*$DFE3*/ CYCLES(0xdfe3, 9);
                s_x = 0x00;
      /*$DFE5*/ FUNC_CHRGOT(0xdfe7);
                branchTarget = true; block_id = 618;
      break;
    case 618:  // $DFE8
      /*$DFE8*/ CYCLES(0xdfe8, 4);
                ram_poke(0x0010, s_x);
                block_id = 619;
      break;
    case 619:  // $DFEA
      /*$DFEA*/ CYCLES(0xdfea, 9);
                ram_poke(0x0081, s_a);
      /*$DFEC*/ FUNC_CHRGOT(0xdfee);
                branchTarget = true; block_id = 620;
      break;
    case 620:  // $DFEF
      /*$DFEF*/ CYCLES(0xdfef, 6);
                FUNC_ISLETC(0xdff1);
                branchTarget = true; block_id = 621;
      break;
    case 621:  // $DFF2
      /*$DFF2*/ CYCLES(0xdff2, 4);
                branchTarget = true; block_id = s_status_c ? 623 : 622;
      break;
    case 622:  // $DFF4
      /*$DFF4*/ CYCLES(0xdff4, 6);
                branchTarget = true; block_id = 568;
      break;
    case 623:  // $DFF7
      /*$DFF7*/ CYCLES(0xdff7, 16);
                s_x = 0x00;
      /*$DFF9*/ ram_poke(0x0011, 0x00);
      /*$DFFB*/ ram_poke(0x0012, 0x00);
      /*$E007*/ CYCLES(0xe007, 6);
                FUNC_CHRGET(0xe009);
                branchTarget = true; block_id = 625;
      break;
    case 624:  // $E003
      /*$E003*/ CYCLES(0xe003, 6);
                branchTarget = true; block_id = 55;
      break;
    case 625:  // $E00A
      /*$E00A*/ CYCLES(0xe00a, 4);
                branchTarget = true; block_id = !s_status_c ? 628 : 626;
      break;
    case 626:  // $E00C
      /*$E00C*/ CYCLES(0xe00c, 6);
                FUNC_ISLETC(0xe00e);
                branchTarget = true; block_id = 627;
      break;
    case 627:  // $E00F
      /*$E00F*/ CYCLES(0xe00f, 4);
                branchTarget = true; block_id = !s_status_c ? 631 : 628;
      break;
    case 628:  // $E011
      /*$E011*/ CYCLES(0xe011, 2);
                s_x = s_a;
                block_id = 629;
      break;
    case 629:  // $E012
      /*$E012*/ CYCLES(0xe012, 6);
                FUNC_CHRGET(0xe014);
      /*$E015*/ CYCLES(0xe015, 4);
                branchTarget = true; block_id = !s_status_c ? 629 : 630;
      break;
    case 630:  // $E017
      /*$E017*/ CYCLES(0xe017, 6);
                FUNC_ISLETC(0xe019);
      /*$E01A*/ CYCLES(0xe01a, 4);
                branchTarget = true; block_id = s_status_c ? 629 : 631;
      break;
    case 631:  // $E01C
      /*$E01C*/ CYCLES(0xe01c, 7);
      /*$E01E*/ branchTarget = true; block_id = (s_a != 0x24) ? 633 : 632;
      break;
    case 632:  // $E020
      /*$E020*/ CYCLES(0xe020, 11);
      /*$E022*/ ram_poke(0x0011, 0xff);
      /*$E024*/ branchTarget = true; block_id = 636;
      break;
    case 633:  // $E026
      /*$E026*/ CYCLES(0xe026, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x25);
      /*$E028*/ branchTarget = true; block_id = (tmp1_U8 != 0x25) ? 637 : 634;
      break;
    case 634:  // $E02A
      /*$E02A*/ CYCLES(0xe02a, 7);
      /*$E02C*/ branchTarget = true; block_id = (ram_peek(0x0014) & 0x80) ? 622 : 635;
      break;
    case 635:  // $E02E
      /*$E02E*/ CYCLES(0xe02e, 14);
      /*$E030*/ ram_poke(0x0012, 0x80);
      /*$E034*/ ram_poke(0x0081, (0x80 | ram_peek(0x0081)));
                block_id = 636;
      break;
    case 636:  // $E036
      /*$E036*/ CYCLES(0xe036, 12);
      /*$E039*/ s_x = (s_x | 0x80);
      /*$E03A*/ FUNC_CHRGET(0xe03c);
                branchTarget = true; block_id = 637;
      break;
    case 637:  // $E03D
      /*$E03D*/ CYCLES(0xe03d, 16);
                ram_poke(0x0082, s_x);
      /*$E042*/ tmp3_U16 = (s_a | ram_peek(0x0014));
                tmp4_U16 = tmp3_U16 - 0x0028;
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0xffd7);
      /*$E044*/ branchTarget = true; block_id = ((uint8_t)tmp4_U16) ? 639 : 638;
      break;
    case 638:  // $E046
      /*$E046*/ CYCLES(0xe046, 6);
      /*$E11E*/ CYCLES(0xe11e, 7);
      /*$E120*/ branchTarget = true; block_id = ram_peek(0x0014) ? 677 : 672;
      break;
    case 639:  // $E049
      /*$E049*/ CYCLES(0xe049, 7);
                tmp1_U8 = ram_peek(0x0014);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
      /*$E04B*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 641 : 640;
      break;
    case 640:  // $E04D
      /*$E04D*/ CYCLES(0xe04d, 4);
                branchTarget = true; block_id = s_status_v ? 638 : 641;
      break;
    case 641:  // $E04F
      /*$E04F*/ CYCLES(0xe04f, 18);
      /*$E051*/ ram_poke(0x0014, 0x00);
      /*$E053*/ s_a = ram_peek(0x0069);
      /*$E055*/ s_x = ram_peek(0x006a);
      /*$E057*/ s_y = 0x00;
                block_id = 642;
      break;
    case 642:  // $E059
      /*$E059*/ CYCLES(0xe059, 4);
                ram_poke(0x009c, s_x);
                block_id = 643;
      break;
    case 643:  // $E05B
      /*$E05B*/ CYCLES(0xe05b, 11);
                ram_poke(0x009b, s_a);
      /*$E05F*/ branchTarget = true; block_id = (s_x != ram_peek(0x006c)) ? 645 : 644;
      break;
    case 644:  // $E061
      /*$E061*/ CYCLES(0xe061, 7);
      /*$E063*/ branchTarget = true; block_id = !(s_a != ram_peek(0x006b)) ? 653 : 645;
      break;
    case 645:  // $E065
      /*$E065*/ CYCLES(0xe065, 11);
      /*$E067*/ tmp1_U8 = peek((ram_peek16(0x009b) + s_y));
      /*$E069*/ branchTarget = true; block_id = (ram_peek(0x0081) != tmp1_U8) ? 648 : 646;
      break;
    case 646:  // $E06B
      /*$E06B*/ CYCLES(0xe06b, 12);
      /*$E06D*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$E06E*/ tmp1_U8 = peek((ram_peek16(0x009b) + tmp1_U8));
      /*$E070*/ branchTarget = true; block_id = !(ram_peek(0x0082) != tmp1_U8) ? 660 : 647;
      break;
    case 647:  // $E072
      /*$E072*/ CYCLES(0xe072, 2);
                s_y = (uint8_t)(s_y - 0x01);
                block_id = 648;
      break;
    case 648:  // $E073
      /*$E073*/ CYCLES(0xe073, 12);
      /*$E076*/ tmp3_U16 = ram_peek(0x009b);
                tmp4_U16 = tmp3_U16 + 0x0007;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x0007);
                s_a = ((uint8_t)tmp4_U16);
      /*$E078*/ branchTarget = true; block_id = !(uint8_t)(tmp4_U16 >> 8) ? 643 : 649;
      break;
    case 649:  // $E07A
      /*$E07A*/ CYCLES(0xe07a, 6);
                tmp1_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp1_U8;
      /*$E07B*/ branchTarget = true; block_id = tmp1_U8 ? 642 : 1660;
      break;
    case 650:  // $E07D
      /*$E07D*/ CYCLES(0xe07d, 7);
                tmp6_U8 = s_a;
                s_status_not_z = (tmp6_U8 != 0x41);
                tmp1_U8 = tmp6_U8 >= 0x41;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp6_U8 - 0x41) & 0x80);
      /*$E07F*/ branchTarget = true; block_id = !tmp1_U8 ? 652 : 651;
      break;
    case 651:  // $E081
      /*$E081*/ CYCLES(0xe081, 9);
                tmp3_U16 = (s_a - 0x005b) - (uint8_t)(0x01 - s_status_c);
      /*$E084*/ tmp4_U16 = tmp3_U16 & 0x00ff;
                tmp5_U16 = tmp4_U16 - 0x00a5;
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0xff5a);
                tmp1_U8 = (uint8_t)(tmp3_U16 - 0x00a5);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 652;
      break;
    case 652:  // $E086
      /*$E086*/ CYCLES(0xe086, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xe086, pop16() + 1);;
      break;
    case 653:  // $E087
      /*$E087*/ CYCLES(0xe087, 11);
                tmp1_U8 = pop8();
      /*$E088*/ push8(tmp1_U8);
      /*$E08B*/ branchTarget = true; block_id = (tmp1_U8 != 0xd7) ? 656 : 654;
      break;
    case 654:  // $E08D
      /*$E08D*/ CYCLES(0xe08d, 14);
                tmp1_U8 = s_sp;
                s_x = tmp1_U8;
      /*$E08E*/ tmp1_U8 = ram_peek((0x0102 + tmp1_U8));
      /*$E091*/ s_status_c = (tmp1_U8 >= 0xde);
      /*$E093*/ branchTarget = true; block_id = (tmp1_U8 != 0xde) ? 656 : 655;
      break;
    case 655:  // $E095
      /*$E095*/ CYCLES(0xe095, 9);
                s_a = 0x9a;
      /*$E097*/ s_status_not_z = 0xe0;
                s_status_n = 0x80;
                s_y = 0xe0;
      /*$E099*/ branchTarget = true; block_id = find_block_id_func_t001(0xe099, pop16() + 1);;
      break;
    case 656:  // $E09C
      /*$E09C*/ CYCLES(0xe09c, 36);
      /*$E0A0*/ ram_poke(0x009b, ram_peek(0x006b));
      /*$E0A2*/ ram_poke(0x009c, ram_peek(0x006c));
      /*$E0A4*/ tmp1_U8 = ram_peek(0x006d);
      /*$E0A6*/ tmp6_U8 = ram_peek(0x006e);
                s_y = tmp6_U8;
      /*$E0A8*/ ram_poke(0x0096, tmp1_U8);
      /*$E0AA*/ ram_poke(0x0097, tmp6_U8);
      /*$E0AD*/ tmp3_U16 = tmp1_U8;
                tmp4_U16 = tmp3_U16 + 0x0007;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x0007);
                s_a = ((uint8_t)tmp4_U16);
      /*$E0AF*/ branchTarget = true; block_id = !(uint8_t)(tmp4_U16 >> 8) ? 658 : 657;
      break;
    case 657:  // $E0B1
      /*$E0B1*/ CYCLES(0xe0b1, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 658;
      break;
    case 658:  // $E0B2
      /*$E0B2*/ CYCLES(0xe0b2, 12);
                ram_poke(0x0094, s_a);
      /*$E0B4*/ ram_poke(0x0095, s_y);
      /*$E0B6*/ branchTarget = true; push16(0xe0b8); block_id = 17;
      break;
    case 659:  // $E0B9
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
                block_id = 660;
      break;
    case 660:  // $E0DE
      /*$E0DE*/ CYCLES(0xe0de, 16);
      /*$E0E1*/ tmp4_U16 = ram_peek(0x009b);
                tmp3_U16 = tmp4_U16 + 0x0002;
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = tmp1_U8;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0x0002);
                s_a = ((uint8_t)tmp3_U16);
      /*$E0E3*/ tmp6_U8 = ram_peek(0x009c);
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_y = tmp6_U8;
      /*$E0E5*/ branchTarget = true; block_id = !tmp1_U8 ? 662 : 661;
      break;
    case 661:  // $E0E7
      /*$E0E7*/ CYCLES(0xe0e7, 2);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
                block_id = 662;
      break;
    case 662:  // $E0E8
      /*$E0E8*/ CYCLES(0xe0e8, 9);
                ram_poke(0x0083, s_a);
      /*$E0EA*/ ram_poke(0x0084, s_y);
      /*$E0EC*/ branchTarget = true; block_id = find_block_id_func_t001(0xe0ec, pop16() + 1);;
      break;
    case 663:  // $E0ED
      /*$E0ED*/ CYCLES(0xe0ed, 19);
      /*$E0EF*/ tmp4_U16 = ram_peek(0x000f) << 0x01;
      /*$E0F0*/ tmp4_U16 = ((tmp4_U16 & 0x00ff) + 0x0005) + (uint8_t)(tmp4_U16 >> 8);
      /*$E0F2*/ tmp3_U16 = tmp4_U16 & 0x00ff;
                tmp5_U16 = ram_peek(0x009b);
                tmp4_U16 = (tmp3_U16 + tmp5_U16) + (uint8_t)(tmp4_U16 >> 8);
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = tmp1_U8;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)tmp5_U16);
                s_a = ((uint8_t)tmp4_U16);
      /*$E0F4*/ tmp6_U8 = ram_peek(0x009c);
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_y = tmp6_U8;
      /*$E0F6*/ branchTarget = true; block_id = !tmp1_U8 ? 665 : 664;
      break;
    case 664:  // $E0F8
      /*$E0F8*/ CYCLES(0xe0f8, 2);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
                block_id = 665;
      break;
    case 665:  // $E0F9
      /*$E0F9*/ CYCLES(0xe0f9, 9);
                ram_poke(0x0094, s_a);
      /*$E0FB*/ ram_poke(0x0095, s_y);
      /*$E0FD*/ branchTarget = true; block_id = find_block_id_func_t001(0xe0fd, pop16() + 1);;
      break;
    case 666:  // $E102
      /*$E102*/ CYCLES(0xe102, 6);
                FUNC_CHRGET(0xe104);
      /*$E105*/ CYCLES(0xe105, 6);
                branchTarget = true; push16(0xe107); block_id = 486;
      break;
    case 667:  // $E108
      /*$E108*/ CYCLES(0xe108, 7);
                tmp1_U8 = ram_peek(0x00a2);
                s_status_not_z = tmp1_U8;
      /*$E10A*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 670 : 668;
      break;
    case 668:  // $E10C
      /*$E10C*/ CYCLES(0xe10c, 11);
      /*$E10E*/ tmp2_U8 = ram_peek(0x009d) >= 0x90;
                s_status_c = tmp2_U8;
      /*$E110*/ branchTarget = true; block_id = !tmp2_U8 ? 671 : 669;
      break;
    case 669:  // $E112
      /*$E112*/ CYCLES(0xe112, 12);
                s_a = 0xfe;
      /*$E114*/ s_y = 0xe0;
      /*$E116*/ FUNC_FCOMP(0xe118);
                branchTarget = true; block_id = 670;
      break;
    case 670:  // $E119
      /*$E119*/ CYCLES(0xe119, 4);
                branchTarget = true; block_id = s_status_not_z ? 684 : 671;
      break;
    case 671:  // $E11B
      /*$E11B*/ CYCLES(0xe11b, 6);
                FUNC_QINT(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xe11b, pop16() + 1);;
      break;
    case 672:  // $E122
      /*$E122*/ CYCLES(0xe122, 18);
      /*$E126*/ push8((ram_peek(0x0010) | ram_peek(0x0012)));
      /*$E129*/ push8(ram_peek(0x0011));
      /*$E12A*/ s_y = 0x00;
                block_id = 673;
      break;
    case 673:  // $E12C
      /*$E12C*/ CYCLES(0xe12c, 19);
      /*$E12D*/ push8(s_y);
      /*$E130*/ push8(ram_peek(0x0082));
      /*$E133*/ push8(ram_peek(0x0081));
      /*$E134*/ branchTarget = true; push16(0xe136); block_id = 666;
      break;
    case 674:  // $E137
      /*$E137*/ CYCLES(0xe137, 53);
                tmp1_U8 = pop8();
      /*$E138*/ ram_poke(0x0081, tmp1_U8);
      /*$E13A*/ tmp1_U8 = pop8();
      /*$E13B*/ ram_poke(0x0082, tmp1_U8);
      /*$E13D*/ tmp1_U8 = pop8();
      /*$E13F*/ tmp6_U8 = s_sp;
                s_x = tmp6_U8;
      /*$E143*/ push8(ram_peek((0x0102 + tmp6_U8)));
      /*$E147*/ push8(ram_peek((0x0101 + tmp6_U8)));
      /*$E14A*/ ram_poke((0x0102 + tmp6_U8), ram_peek(0x00a0));
      /*$E14F*/ ram_poke((0x0101 + tmp6_U8), ram_peek(0x00a1));
      /*$E152*/ s_y = (uint8_t)(tmp1_U8 + 0x01);
      /*$E153*/ FUNC_CHRGOT(0xe155);
      /*$E156*/ CYCLES(0xe156, 7);
      /*$E158*/ branchTarget = true; block_id = !(s_a != 0x2c) ? 673 : 675;
      break;
    case 675:  // $E15A
      /*$E15A*/ CYCLES(0xe15a, 9);
                ram_poke(0x000f, s_y);
      /*$E15C*/ branchTarget = true; push16(0xe15e); block_id = 563;
      break;
    case 676:  // $E15F
      /*$E15F*/ CYCLES(0xe15f, 18);
                tmp1_U8 = pop8();
      /*$E160*/ ram_poke(0x0011, tmp1_U8);
      /*$E162*/ tmp1_U8 = pop8();
      /*$E163*/ ram_poke(0x0012, tmp1_U8);
      /*$E167*/ ram_poke(0x0010, (tmp1_U8 & 0x7f));
                block_id = 677;
      break;
    case 677:  // $E169
      /*$E169*/ CYCLES(0xe169, 7);
                s_x = ram_peek(0x006b);
      /*$E16B*/ s_a = ram_peek(0x006c);
                block_id = 678;
      break;
    case 678:  // $E16D
      /*$E16D*/ CYCLES(0xe16d, 14);
                ram_poke(0x009b, s_x);
      /*$E16F*/ tmp1_U8 = s_a;
                ram_poke(0x009c, tmp1_U8);
      /*$E173*/ branchTarget = true; block_id = (tmp1_U8 != ram_peek(0x006e)) ? 680 : 679;
      break;
    case 679:  // $E175
      /*$E175*/ CYCLES(0xe175, 7);
                tmp1_U8 = s_x;
                tmp6_U8 = ram_peek(0x006d);
                s_status_c = (tmp1_U8 >= tmp6_U8);
      /*$E177*/ branchTarget = true; block_id = !(tmp1_U8 != tmp6_U8) ? 691 : 680;
      break;
    case 680:  // $E179
      /*$E179*/ CYCLES(0xe179, 16);
      /*$E17B*/ tmp1_U8 = peek(ram_peek16(0x009b));
      /*$E17D*/ s_y = 0x01;
      /*$E180*/ branchTarget = true; block_id = (tmp1_U8 != ram_peek(0x0081)) ? 682 : 681;
      break;
    case 681:  // $E182
      /*$E182*/ CYCLES(0xe182, 11);
                tmp6_U8 = ram_peek(0x0082);
      /*$E184*/ tmp1_U8 = peek((ram_peek16(0x009b) + s_y));
                s_status_c = (tmp6_U8 >= tmp1_U8);
      /*$E186*/ branchTarget = true; block_id = !(tmp6_U8 != tmp1_U8) ? 686 : 682;
      break;
    case 682:  // $E188
      /*$E188*/ CYCLES(0xe188, 24);
                tmp6_U8 = s_y;
      /*$E189*/ tmp1_U8 = peek((ram_peek16(0x009b) + (uint8_t)(tmp6_U8 + 0x01)));
      /*$E18C*/ tmp4_U16 = tmp1_U8 + ram_peek(0x009b);
      /*$E18E*/ s_x = ((uint8_t)tmp4_U16);
      /*$E18F*/ tmp6_U8 = (uint8_t)(tmp6_U8 + 0x02);
                s_y = tmp6_U8;
      /*$E190*/ tmp6_U8 = peek((ram_peek16(0x009b) + tmp6_U8));
      /*$E192*/ tmp5_U16 = tmp6_U8;
                tmp3_U16 = ram_peek(0x009c);
                tmp4_U16 = (tmp5_U16 + tmp3_U16) + (uint8_t)(tmp4_U16 >> 8);
                tmp6_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = tmp6_U8;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)tmp3_U16);
                s_a = ((uint8_t)tmp4_U16);
      /*$E194*/ branchTarget = true; block_id = !tmp6_U8 ? 678 : 683;
      break;
    case 683:  // $E196
      /*$E196*/ CYCLES(0xe196, 9);
                s_x = 0x6b;
      /*$E198*/ block_id = 685;
      break;
    case 684:  // $E199
      /*$E199*/ CYCLES(0xe199, 4);
                s_x = 0x35;
                block_id = 685;
      break;
    case 685:  // $E19B
      /*$E19B*/ CYCLES(0xe19b, 6);
                branchTarget = true; block_id = 44;
      break;
    case 686:  // $E19E
      /*$E19E*/ CYCLES(0xe19e, 11);
                s_x = 0x78;
      /*$E1A2*/ branchTarget = true; block_id = ram_peek(0x0010) ? 685 : 687;
      break;
    case 687:  // $E1A4
      /*$E1A4*/ CYCLES(0xe1a4, 7);
                tmp1_U8 = ram_peek(0x0014);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$E1A6*/ branchTarget = true; block_id = !tmp1_U8 ? 689 : 688;
      break;
    case 688:  // $E1A8
      /*$E1A8*/ CYCLES(0xe1a8, 4);
                s_status_c = 0x01;
      /*$E1A9*/ branchTarget = true; block_id = find_block_id_func_t001(0xe1a9, pop16() + 1);;
      break;
    case 689:  // $E1AA
      /*$E1AA*/ CYCLES(0xe1aa, 6);
                FUNC_GETARY(0xe1ac);
      /*$E1AD*/ CYCLES(0xe1ad, 14);
                tmp1_U8 = ram_peek(0x000f);
      /*$E1AF*/ s_y = 0x04;
      /*$E1B1*/ tmp6_U8 = peek((ram_peek16(0x009b) + 0x0004));
                s_status_c = (tmp1_U8 >= tmp6_U8);
      /*$E1B3*/ branchTarget = true; block_id = (tmp1_U8 != tmp6_U8) ? 683 : 690;
      break;
    case 690:  // $E1B5
      /*$E1B5*/ CYCLES(0xe1b5, 6);
                branchTarget = true; block_id = 712;
      break;
    case 691:  // $E1B8
      /*$E1B8*/ CYCLES(0xe1b8, 7);
      /*$E1BA*/ branchTarget = true; block_id = !ram_peek(0x0014) ? 693 : 692;
      break;
    case 692:  // $E1BC
      /*$E1BC*/ CYCLES(0xe1bc, 9);
                s_x = 0x2a;
      /*$E1BE*/ branchTarget = true; block_id = 44;
      break;
    case 693:  // $E1C1
      /*$E1C1*/ CYCLES(0xe1c1, 6);
                FUNC_GETARY(0xe1c3);
      /*$E1C4*/ CYCLES(0xe1c4, 6);
                branchTarget = true; push16(0xe1c6); block_id = 32;
      break;
    case 694:  // $E1C7
      /*$E1C7*/ CYCLES(0xe1c7, 23);
      /*$E1C9*/ s_y = 0x00;
      /*$E1CA*/ ram_poke(0x00ae, 0x00);
      /*$E1CC*/ s_x = 0x05;
      /*$E1CE*/ tmp6_U8 = ram_peek(0x0081);
      /*$E1D0*/ poke(ram_peek16(0x009b), tmp6_U8);
      /*$E1D2*/ branchTarget = true; block_id = !(tmp6_U8 & 0x80) ? 696 : 695;
      break;
    case 695:  // $E1D4
      /*$E1D4*/ CYCLES(0xe1d4, 2);
                s_x = (uint8_t)(s_x - 0x01);
                block_id = 696;
      break;
    case 696:  // $E1D5
      /*$E1D5*/ CYCLES(0xe1d5, 12);
                tmp6_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp6_U8;
      /*$E1D6*/ tmp1_U8 = ram_peek(0x0082);
      /*$E1D8*/ poke((ram_peek16(0x009b) + tmp6_U8), tmp1_U8);
      /*$E1DA*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 698 : 697;
      break;
    case 697:  // $E1DC
      /*$E1DC*/ CYCLES(0xe1dc, 4);
      /*$E1DD*/ s_x = (uint8_t)(s_x - 0x02);
                block_id = 698;
      break;
    case 698:  // $E1DE
      /*$E1DE*/ CYCLES(0xe1de, 16);
                ram_poke(0x00ad, s_x);
      /*$E1E4*/ tmp1_U8 = (uint8_t)(s_y + 0x03);
                s_y = tmp1_U8;
      /*$E1E5*/ poke((ram_peek16(0x009b) + tmp1_U8), ram_peek(0x000f));
                block_id = 699;
      break;
    case 699:  // $E1E7
      /*$E1E7*/ CYCLES(0xe1e7, 14);
                s_x = 0x0b;
      /*$E1E9*/ s_a = 0x00;
      /*$E1EB*/ tmp1_U8 = (ram_peek(0x0010) >> 0x06) & 0x01;
                s_status_v = tmp1_U8;
      /*$E1ED*/ branchTarget = true; block_id = !tmp1_U8 ? 701 : 700;
      break;
    case 700:  // $E1EF
      /*$E1EF*/ CYCLES(0xe1ef, 14);
                tmp1_U8 = pop8();
      /*$E1F1*/ tmp4_U16 = tmp1_U8 + 0x0001;
      /*$E1F3*/ s_x = ((uint8_t)tmp4_U16);
      /*$E1F4*/ tmp1_U8 = pop8();
      /*$E1F5*/ tmp3_U16 = tmp1_U8;
                tmp4_U16 = tmp3_U16 + (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 701;
      break;
    case 701:  // $E1F7
      /*$E1F7*/ CYCLES(0xe1f7, 18);
                tmp1_U8 = s_y;
      /*$E1F8*/ poke((ram_peek16(0x009b) + (uint8_t)(tmp1_U8 + 0x01)), s_a);
      /*$E1FA*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x02);
                s_y = tmp1_U8;
      /*$E1FB*/ tmp6_U8 = s_x;
                s_a = tmp6_U8;
      /*$E1FC*/ poke((ram_peek16(0x009b) + tmp1_U8), tmp6_U8);
      /*$E1FE*/ branchTarget = true; push16(0xe200); block_id = 730;
      break;
    case 702:  // $E201
      /*$E201*/ CYCLES(0xe201, 18);
                ram_poke(0x00ad, s_x);
      /*$E203*/ ram_poke(0x00ae, s_a);
      /*$E205*/ s_y = ram_peek(0x005e);
      /*$E207*/ tmp1_U8 = (uint8_t)(ram_peek(0x000f) - 0x01);
                ram_poke(0x000f, tmp1_U8);
      /*$E209*/ branchTarget = true; block_id = tmp1_U8 ? 699 : 703;
      break;
    case 703:  // $E20B
      /*$E20B*/ CYCLES(0xe20b, 7);
                tmp4_U16 = (s_a + ram_peek(0x0095)) + s_status_c;
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = tmp1_U8;
                s_a = ((uint8_t)tmp4_U16);
      /*$E20D*/ branchTarget = true; block_id = tmp1_U8 ? 717 : 704;
      break;
    case 704:  // $E20F
      /*$E20F*/ CYCLES(0xe20f, 14);
                tmp1_U8 = s_a;
                ram_poke(0x0095, tmp1_U8);
      /*$E211*/ s_y = tmp1_U8;
      /*$E213*/ tmp4_U16 = s_x;
                tmp3_U16 = ram_peek(0x0094);
                tmp5_U16 = (tmp4_U16 + tmp3_U16) + s_status_c;
                tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
                s_status_c = tmp1_U8;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)tmp3_U16);
                s_a = ((uint8_t)tmp5_U16);
      /*$E215*/ branchTarget = true; block_id = !tmp1_U8 ? 706 : 705;
      break;
    case 705:  // $E217
      /*$E217*/ CYCLES(0xe217, 6);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$E218*/ branchTarget = true; block_id = !tmp1_U8 ? 717 : 706;
      break;
    case 706:  // $E21A
      /*$E21A*/ CYCLES(0xe21a, 6);
                branchTarget = true; push16(0xe21c); block_id = 32;
      break;
    case 707:  // $E21D
      /*$E21D*/ CYCLES(0xe21d, 21);
                ram_poke(0x006d, s_a);
      /*$E21F*/ ram_poke(0x006e, s_y);
      /*$E221*/ s_a = 0x00;
      /*$E223*/ ram_poke(0x00ae, (uint8_t)(ram_peek(0x00ae) + 0x01));
      /*$E225*/ tmp1_U8 = ram_peek(0x00ad);
                s_y = tmp1_U8;
      /*$E227*/ branchTarget = true; block_id = !tmp1_U8 ? 709 : 708;
      break;
    case 708:  // $E229
      /*$E229*/ CYCLES(0xe229, 9);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp1_U8;
      /*$E22A*/ poke((ram_peek16al(0x0094) + tmp1_U8), s_a);
      /*$E22C*/ branchTarget = true; block_id = tmp1_U8 ? 708 : 709;
      break;
    case 709:  // $E22E
      /*$E22E*/ CYCLES(0xe22e, 11);
                ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) - 0x01));
      /*$E230*/ tmp1_U8 = (uint8_t)(ram_peek(0x00ae) - 0x01);
                ram_poke(0x00ae, tmp1_U8);
      /*$E232*/ branchTarget = true; block_id = tmp1_U8 ? 708 : 710;
      break;
    case 710:  // $E234
      /*$E234*/ CYCLES(0xe234, 38);
                ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) + 0x01));
      /*$E239*/ tmp5_U16 = ram_peek(0x006d) - ram_peek(0x009b);
      /*$E23D*/ poke((ram_peek16(0x009b) + 0x0002), ((uint8_t)tmp5_U16));
      /*$E241*/ s_y = 0x03;
      /*$E242*/ tmp3_U16 = ram_peek(0x006e);
                tmp4_U16 = ram_peek(0x009c);
                tmp5_U16 = (tmp3_U16 - tmp4_U16) - ((uint8_t)(tmp5_U16 >> 8) & 0x01);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp4_U16));
      /*$E244*/ poke((ram_peek16(0x009b) + 0x0003), ((uint8_t)tmp5_U16));
      /*$E246*/ tmp1_U8 = ram_peek(0x0010);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$E248*/ branchTarget = true; block_id = tmp1_U8 ? 729 : 711;
      break;
    case 711:  // $E24A
      /*$E24A*/ CYCLES(0xe24a, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 712;
      break;
    case 712:  // $E24B
      /*$E24B*/ CYCLES(0xe24b, 14);
                tmp6_U8 = peek((ram_peek16(0x009b) + s_y));
      /*$E24D*/ ram_poke(0x000f, tmp6_U8);
      /*$E24F*/ s_a = 0x00;
      /*$E251*/ ram_poke(0x00ad, 0x00);
                block_id = 713;
      break;
    case 713:  // $E253
      /*$E253*/ CYCLES(0xe253, 24);
                ram_poke(0x00ae, s_a);
      /*$E255*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$E256*/ tmp6_U8 = pop8();
      /*$E257*/ s_x = tmp6_U8;
      /*$E258*/ ram_poke(0x00a0, tmp6_U8);
      /*$E25A*/ tmp6_U8 = pop8();
      /*$E25B*/ ram_poke(0x00a1, tmp6_U8);
      /*$E25D*/ tmp1_U8 = peek((ram_peek16(0x009b) + tmp1_U8));
                s_status_not_z = (tmp6_U8 != tmp1_U8);
                tmp1_U8 = tmp6_U8 >= tmp1_U8;
                s_status_c = tmp1_U8;
      /*$E25F*/ branchTarget = true; block_id = !tmp1_U8 ? 718 : 714;
      break;
    case 714:  // $E261
      /*$E261*/ CYCLES(0xe261, 4);
                branchTarget = true; block_id = s_status_not_z ? 716 : 715;
      break;
    case 715:  // $E263
      /*$E263*/ CYCLES(0xe263, 11);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$E265*/ tmp1_U8 = peek((ram_peek16(0x009b) + tmp1_U8));
                tmp1_U8 = s_x >= tmp1_U8;
                s_status_c = tmp1_U8;
      /*$E267*/ branchTarget = true; block_id = !tmp1_U8 ? 719 : 716;
      break;
    case 716:  // $E269
      /*$E269*/ CYCLES(0xe269, 6);
                branchTarget = true; block_id = 683;
      break;
    case 717:  // $E26C
      /*$E26C*/ CYCLES(0xe26c, 6);
                branchTarget = true; block_id = 43;
      break;
    case 718:  // $E26F
      /*$E26F*/ CYCLES(0xe26f, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 719;
      break;
    case 719:  // $E270
      /*$E270*/ CYCLES(0xe270, 12);
      /*$E272*/ tmp1_U8 = ram_peek(0x00ae) | ram_peek(0x00ad);
                s_a = tmp1_U8;
      /*$E274*/ s_status_c = 0x00;
      /*$E275*/ branchTarget = true; block_id = !tmp1_U8 ? 722 : 720;
      break;
    case 720:  // $E277
      /*$E277*/ CYCLES(0xe277, 6);
                branchTarget = true; push16(0xe279); block_id = 730;
      break;
    case 721:  // $E27A
      /*$E27A*/ CYCLES(0xe27a, 12);
      /*$E27B*/ tmp3_U16 = (s_x + ram_peek(0x00a0)) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
      /*$E27D*/ s_x = ((uint8_t)tmp3_U16);
      /*$E27E*/ s_a = s_y;
      /*$E27F*/ s_y = ram_peek(0x005e);
                block_id = 722;
      break;
    case 722:  // $E281
      /*$E281*/ CYCLES(0xe281, 14);
                tmp5_U16 = s_a;
                tmp4_U16 = ram_peek(0x00a1);
                tmp3_U16 = (tmp5_U16 + tmp4_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp5_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp3_U16);
      /*$E283*/ ram_poke(0x00ad, s_x);
      /*$E285*/ tmp1_U8 = (uint8_t)(ram_peek(0x000f) - 0x01);
                ram_poke(0x000f, tmp1_U8);
      /*$E287*/ branchTarget = true; block_id = tmp1_U8 ? 713 : 723;
      break;
    case 723:  // $E289
      /*$E289*/ CYCLES(0xe289, 14);
                ram_poke(0x00ae, s_a);
      /*$E28B*/ s_x = 0x05;
      /*$E28F*/ branchTarget = true; block_id = !(ram_peek(0x0081) & 0x80) ? 725 : 724;
      break;
    case 724:  // $E291
      /*$E291*/ CYCLES(0xe291, 2);
                s_x = (uint8_t)(s_x - 0x01);
                block_id = 725;
      break;
    case 725:  // $E292
      /*$E292*/ CYCLES(0xe292, 7);
      /*$E294*/ branchTarget = true; block_id = !(ram_peek(0x0082) & 0x80) ? 727 : 726;
      break;
    case 726:  // $E296
      /*$E296*/ CYCLES(0xe296, 4);
      /*$E297*/ s_x = (uint8_t)(s_x - 0x02);
                block_id = 727;
      break;
    case 727:  // $E298
      /*$E298*/ CYCLES(0xe298, 12);
                ram_poke(0x0064, s_x);
      /*$E29A*/ s_a = 0x00;
      /*$E29C*/ branchTarget = true; push16(0xe29e); block_id = 731;
      break;
    case 728:  // $E29F
      /*$E29F*/ CYCLES(0xe29f, 23);
      /*$E2A0*/ tmp3_U16 = (s_x + ram_peek(0x0094)) + s_status_c;
      /*$E2A2*/ ram_poke(0x0083, ((uint8_t)tmp3_U16));
      /*$E2A5*/ tmp4_U16 = s_y;
                tmp5_U16 = ram_peek(0x0095);
                tmp3_U16 = (tmp4_U16 + tmp5_U16) + (uint8_t)(tmp3_U16 >> 8);
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp5_U16);
                tmp1_U8 = (uint8_t)tmp3_U16;
      /*$E2A7*/ ram_poke(0x0084, tmp1_U8);
      /*$E2A9*/ s_y = tmp1_U8;
      /*$E2AA*/ tmp1_U8 = ram_peek(0x0083);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 729;
      break;
    case 729:  // $E2AC
      /*$E2AC*/ CYCLES(0xe2ac, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xe2ac, pop16() + 1);;
      break;
    case 730:  // $E2AD
      /*$E2AD*/ CYCLES(0xe2ad, 16);
                tmp1_U8 = s_y;
                ram_poke(0x005e, tmp1_U8);
      /*$E2AF*/ tmp6_U8 = peek((ram_peek16(0x009b) + tmp1_U8));
      /*$E2B1*/ ram_poke(0x0064, tmp6_U8);
      /*$E2B4*/ tmp1_U8 = peek((ram_peek16(0x009b) + (uint8_t)(tmp1_U8 - 0x01)));
                s_a = tmp1_U8;
                block_id = 731;
      break;
    case 731:  // $E2B6
      /*$E2B6*/ CYCLES(0xe2b6, 18);
                ram_poke(0x0065, s_a);
      /*$E2BA*/ ram_poke(0x0099, 0x10);
      /*$E2BC*/ s_x = 0x00;
      /*$E2BE*/ s_y = 0x00;
                block_id = 732;
      break;
    case 732:  // $E2C0
      /*$E2C0*/ CYCLES(0xe2c0, 14);
      /*$E2C1*/ tmp3_U16 = s_x << 0x01;
      /*$E2C2*/ s_x = ((uint8_t)tmp3_U16);
      /*$E2C4*/ tmp3_U16 = (s_y << 0x01) | (uint8_t)(tmp3_U16 >> 8);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = tmp1_U8;
                tmp6_U8 = (uint8_t)tmp3_U16;
                s_a = tmp6_U8;
      /*$E2C5*/ s_y = tmp6_U8;
      /*$E2C6*/ branchTarget = true; block_id = tmp1_U8 ? 717 : 733;
      break;
    case 733:  // $E2C8
      /*$E2C8*/ CYCLES(0xe2c8, 11);
                tmp3_U16 = ram_peek(0x00ad) << 0x01;
                ram_poke(0x00ad, ((uint8_t)tmp3_U16));
      /*$E2CA*/ tmp3_U16 = (ram_peek(0x00ae) << 0x01) | (uint8_t)(tmp3_U16 >> 8);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = tmp1_U8;
                ram_poke(0x00ae, ((uint8_t)tmp3_U16));
      /*$E2CC*/ branchTarget = true; block_id = !tmp1_U8 ? 735 : 734;
      break;
    case 734:  // $E2CE
      /*$E2CE*/ CYCLES(0xe2ce, 19);
      /*$E2D0*/ tmp3_U16 = s_x + ram_peek(0x0064);
      /*$E2D2*/ s_x = ((uint8_t)tmp3_U16);
      /*$E2D4*/ tmp5_U16 = s_y;
                tmp4_U16 = ram_peek(0x0065);
                tmp3_U16 = (tmp5_U16 + tmp4_U16) + (uint8_t)(tmp3_U16 >> 8);
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = tmp1_U8;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp5_U16, (uint8_t)tmp4_U16);
                tmp6_U8 = (uint8_t)tmp3_U16;
                s_a = tmp6_U8;
      /*$E2D6*/ s_y = tmp6_U8;
      /*$E2D7*/ branchTarget = true; block_id = tmp1_U8 ? 717 : 735;
      break;
    case 735:  // $E2D9
      /*$E2D9*/ CYCLES(0xe2d9, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x0099) - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x0099, tmp1_U8);
      /*$E2DB*/ branchTarget = true; block_id = tmp1_U8 ? 732 : 736;
      break;
    case 736:  // $E2DD
      /*$E2DD*/ CYCLES(0xe2dd, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xe2dd, pop16() + 1);;
      break;
    case 737:  // $E2DE
      /*$E2DE*/ CYCLES(0xe2de, 7);
      /*$E2E0*/ branchTarget = true; block_id = !ram_peek(0x0011) ? 739 : 738;
      break;
    case 738:  // $E2E2
      /*$E2E2*/ CYCLES(0xe2e2, 6);
                branchTarget = true; push16(0xe2e4); block_id = 843;
      break;
    case 739:  // $E2E5
      /*$E2E5*/ CYCLES(0xe2e5, 6);
                FUNC_GARBAGE(0xe2e7);
                branchTarget = true; block_id = 740;
      break;
    case 740:  // $E2E8
      /*$E2E8*/ CYCLES(0xe2e8, 18);
      /*$E2EB*/ tmp3_U16 = ram_peek(0x006f) - ram_peek(0x006d);
      /*$E2ED*/ s_y = ((uint8_t)tmp3_U16);
      /*$E2F0*/ tmp3_U16 = (ram_peek(0x0070) - ram_peek(0x006e)) - ((uint8_t)(tmp3_U16 >> 8) & 0x01);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp3_U16);
                block_id = 741;
      break;
    case 741:  // $E2F2
      /*$E2F2*/ CYCLES(0xe2f2, 23);
      /*$E2F4*/ ram_poke(0x0011, 0x00);
      /*$E2F6*/ ram_poke(0x009e, s_a);
      /*$E2F8*/ ram_poke(0x009f, s_y);
      /*$E2FA*/ s_x = 0x90;
      /*$E2FC*/ branchTarget = true; block_id = 998;
      break;
    case 742:  // $E301
      /*$E301*/ CYCLES(0xe301, 9);
                s_a = 0x00;
      /*$E303*/ s_status_c = 0x01;
      /*$E304*/ branchTarget = true; block_id = 741;
      break;
    case 743:  // $E306
      /*$E306*/ CYCLES(0xe306, 9);
      /*$E308*/ tmp1_U8 = (uint8_t)(ram_peek(0x0076) + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$E309*/ branchTarget = true; block_id = tmp1_U8 ? 729 : 744;
      break;
    case 744:  // $E30B
      /*$E30B*/ CYCLES(0xe30b, 14);
                s_x = 0x95;
      /*$E310*/ branchTarget = true; block_id = 44;
      break;
    case 745:  // $E30E
      /*$E30E*/ CYCLES(0xe30e, 9);
                s_x = 0xe0;
      /*$E310*/ branchTarget = true; block_id = 44;
      break;
    case 746:  // $E341
      /*$E341*/ CYCLES(0xe341, 9);
                s_a = 0xc2;
      /*$E343*/ branchTarget = true; push16(0xe345); block_id = 566;
      break;
    case 747:  // $E346
      /*$E346*/ CYCLES(0xe346, 12);
                tmp1_U8 = s_a | 0x80;
                s_a = tmp1_U8;
      /*$E348*/ ram_poke(0x0014, tmp1_U8);
      /*$E34A*/ branchTarget = true; push16(0xe34c); block_id = 619;
      break;
    case 748:  // $E34D
      /*$E34D*/ CYCLES(0xe34d, 12);
                ram_poke(0x008a, s_a);
      /*$E34F*/ ram_poke(0x008b, s_y);
      /*$E351*/ branchTarget = true; block_id = 487;
      break;
    case 749:  // $E357
      /*$E357*/ CYCLES(0xe357, 16);
      /*$E359*/ push8(ram_peek(0x008b));
      /*$E35A*/ tmp1_U8 = ram_peek(0x008a);
                s_a = tmp1_U8;
      /*$E35C*/ push8(tmp1_U8);
      /*$E35D*/ branchTarget = true; push16(0xe35f); block_id = 561;
      break;
    case 750:  // $E360
      /*$E360*/ CYCLES(0xe360, 6);
                branchTarget = true; push16(0xe362); block_id = 487;
      break;
    case 751:  // $E363
      /*$E363*/ CYCLES(0xe363, 31);
                tmp1_U8 = pop8();
      /*$E364*/ ram_poke(0x008a, tmp1_U8);
      /*$E366*/ tmp1_U8 = pop8();
      /*$E367*/ ram_poke(0x008b, tmp1_U8);
      /*$E36B*/ tmp1_U8 = peek((ram_peek16al(0x008a) + 0x0002));
      /*$E36D*/ ram_poke(0x0083, tmp1_U8);
      /*$E36F*/ s_x = tmp1_U8;
      /*$E370*/ s_y = 0x03;
      /*$E371*/ tmp1_U8 = peek((ram_peek16al(0x008a) + 0x0003));
                s_a = tmp1_U8;
      /*$E373*/ branchTarget = true; block_id = !tmp1_U8 ? 745 : 752;
      break;
    case 752:  // $E375
      /*$E375*/ CYCLES(0xe375, 6);
                ram_poke(0x0084, s_a);
      /*$E377*/ s_y = (uint8_t)(s_y + 0x01);
                block_id = 753;
      break;
    case 753:  // $E378
      /*$E378*/ CYCLES(0xe378, 11);
                tmp1_U8 = s_y;
                tmp6_U8 = peek((ram_peek16(0x0083) + tmp1_U8));
      /*$E37A*/ push8(tmp6_U8);
      /*$E37B*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_y = tmp1_U8;
      /*$E37C*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 753 : 754;
      break;
    case 754:  // $E37E
      /*$E37E*/ CYCLES(0xe37e, 9);
                s_y = ram_peek(0x0084);
      /*$E380*/ branchTarget = true; push16(0xe382); block_id = 978;
      break;
    case 755:  // $E383
      /*$E383*/ CYCLES(0xe383, 41);
      /*$E385*/ push8(ram_peek(0x00b9));
      /*$E388*/ push8(ram_peek(0x00b8));
      /*$E389*/ tmp1_U8 = s_y;
                tmp6_U8 = peek((ram_peek16al(0x008a) + tmp1_U8));
      /*$E38B*/ ram_poke(0x00b8, tmp6_U8);
      /*$E38D*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
                s_y = tmp1_U8;
      /*$E38E*/ tmp1_U8 = peek((ram_peek16al(0x008a) + tmp1_U8));
      /*$E390*/ ram_poke(0x00b9, tmp1_U8);
      /*$E394*/ push8(ram_peek(0x0084));
      /*$E395*/ tmp1_U8 = ram_peek(0x0083);
                s_a = tmp1_U8;
      /*$E397*/ push8(tmp1_U8);
      /*$E398*/ branchTarget = true; push16(0xe39a); block_id = 486;
      break;
    case 756:  // $E39B
      /*$E39B*/ CYCLES(0xe39b, 16);
                tmp1_U8 = pop8();
      /*$E39C*/ ram_poke(0x008a, tmp1_U8);
      /*$E39E*/ tmp1_U8 = pop8();
      /*$E39F*/ ram_poke(0x008b, tmp1_U8);
      /*$E3A1*/ FUNC_CHRGOT(0xe3a3);
      /*$E3A4*/ CYCLES(0xe3a4, 4);
                branchTarget = true; block_id = !s_status_not_z ? 758 : 757;
      break;
    case 757:  // $E3A6
      /*$E3A6*/ CYCLES(0xe3a6, 6);
                branchTarget = true; block_id = 568;
      break;
    case 758:  // $E3A9
      /*$E3A9*/ CYCLES(0xe3a9, 48);
                tmp1_U8 = pop8();
      /*$E3AA*/ ram_poke(0x00b8, tmp1_U8);
      /*$E3AC*/ tmp1_U8 = pop8();
      /*$E3AD*/ ram_poke(0x00b9, tmp1_U8);
      /*$E3B1*/ tmp1_U8 = pop8();
      /*$E3B2*/ poke(ram_peek16al(0x008a), tmp1_U8);
      /*$E3B4*/ tmp1_U8 = pop8();
      /*$E3B6*/ poke((ram_peek16al(0x008a) + 0x0001), tmp1_U8);
      /*$E3B8*/ tmp1_U8 = pop8();
      /*$E3BA*/ poke((ram_peek16al(0x008a) + 0x0002), tmp1_U8);
      /*$E3BC*/ tmp1_U8 = pop8();
      /*$E3BE*/ poke((ram_peek16al(0x008a) + 0x0003), tmp1_U8);
      /*$E3C0*/ tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$E3C1*/ s_status_not_z = 0x04;
                s_status_n = 0x00;
                s_y = 0x04;
      /*$E3C2*/ poke((ram_peek16al(0x008a) + 0x0004), tmp1_U8);
      /*$E3C4*/ branchTarget = true; block_id = find_block_id_func_t001(0xe3c4, pop16() + 1);;
      break;
    case 759:  // $E3D5
      /*$E3D5*/ CYCLES(0xe3d5, 19);
                tmp2_U8 = ram_peek(0x00a0);
                s_x = tmp2_U8;
      /*$E3D7*/ tmp6_U8 = ram_peek(0x00a1);
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_y = tmp6_U8;
      /*$E3D9*/ ram_poke(0x008c, tmp2_U8);
      /*$E3DB*/ ram_poke(0x008d, tmp6_U8);
      /*$E3DD*/ branchTarget = true; push16(0xe3df); block_id = 778;
      break;
    case 760:  // $E3E0
      /*$E3E0*/ CYCLES(0xe3e0, 12);
                ram_poke(0x009e, s_x);
      /*$E3E2*/ ram_poke(0x009f, s_y);
      /*$E3E4*/ ram_poke(0x009d, s_a);
      /*$E3E6*/ branchTarget = true; block_id = find_block_id_func_t001(0xe3e6, pop16() + 1);;
      break;
    case 761:  // $E3E7
      /*$E3E7*/ CYCLES(0xe3e7, 11);
      /*$E3E9*/ ram_poke(0x000d, 0x22);
      /*$E3EB*/ ram_poke(0x000e, 0x22);
                block_id = 762;
      break;
    case 762:  // $E3ED
      /*$E3ED*/ CYCLES(0xe3ed, 18);
                tmp6_U8 = s_a;
                ram_poke(0x00ab, tmp6_U8);
      /*$E3EF*/ tmp1_U8 = s_y;
                ram_poke(0x00ac, tmp1_U8);
      /*$E3F1*/ ram_poke(0x009e, tmp6_U8);
      /*$E3F3*/ ram_poke(0x009f, tmp1_U8);
      /*$E3F5*/ s_y = 0xff;
                block_id = 763;
      break;
    case 763:  // $E3F7
      /*$E3F7*/ CYCLES(0xe3f7, 9);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$E3F8*/ tmp1_U8 = peek((ram_peek16(0x00ab) + tmp1_U8));
                s_a = tmp1_U8;
      /*$E3FA*/ branchTarget = true; block_id = !tmp1_U8 ? 767 : 764;
      break;
    case 764:  // $E3FC
      /*$E3FC*/ CYCLES(0xe3fc, 7);
      /*$E3FE*/ branchTarget = true; block_id = !(s_a != ram_peek(0x000d)) ? 766 : 765;
      break;
    case 765:  // $E400
      /*$E400*/ CYCLES(0xe400, 7);
      /*$E402*/ branchTarget = true; block_id = (s_a != ram_peek(0x000e)) ? 763 : 766;
      break;
    case 766:  // $E404
      /*$E404*/ CYCLES(0xe404, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x22);
      /*$E406*/ branchTarget = true; block_id = !(tmp1_U8 != 0x22) ? 768 : 767;
      break;
    case 767:  // $E408
      /*$E408*/ CYCLES(0xe408, 2);
                s_status_c = 0x00;
                block_id = 768;
      break;
    case 768:  // $E409
      /*$E409*/ CYCLES(0xe409, 19);
                tmp1_U8 = s_y;
                ram_poke(0x009d, tmp1_U8);
      /*$E40C*/ tmp3_U16 = tmp1_U8;
                tmp4_U16 = ram_peek(0x00ab);
                tmp5_U16 = (tmp3_U16 + tmp4_U16) + s_status_c;
                tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
                s_status_c = tmp1_U8;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
      /*$E40E*/ ram_poke(0x00ad, ((uint8_t)tmp5_U16));
      /*$E410*/ s_x = ram_peek(0x00ac);
      /*$E412*/ branchTarget = true; block_id = !tmp1_U8 ? 770 : 769;
      break;
    case 769:  // $E414
      /*$E414*/ CYCLES(0xe414, 2);
                s_x = (uint8_t)(s_x + 0x01);
                block_id = 770;
      break;
    case 770:  // $E415
      /*$E415*/ CYCLES(0xe415, 11);
                ram_poke(0x00ae, s_x);
      /*$E417*/ tmp1_U8 = ram_peek(0x00ac);
                s_a = tmp1_U8;
      /*$E419*/ branchTarget = true; block_id = !tmp1_U8 ? 772 : 771;
      break;
    case 771:  // $E41B
      /*$E41B*/ CYCLES(0xe41b, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x02);
      /*$E41D*/ branchTarget = true; block_id = (tmp1_U8 != 0x02) ? 774 : 772;
      break;
    case 772:  // $E41F
      /*$E41F*/ CYCLES(0xe41f, 7);
                s_a = s_y;
      /*$E420*/ branchTarget = true; push16(0xe422); block_id = 759;
      break;
    case 773:  // $E423
      /*$E423*/ CYCLES(0xe423, 12);
                s_x = ram_peek(0x00ab);
      /*$E425*/ s_y = ram_peek(0x00ac);
      /*$E427*/ branchTarget = true; push16(0xe429); block_id = 835;
      break;
    case 774:  // $E42A
      /*$E42A*/ CYCLES(0xe42a, 11);
                tmp1_U8 = ram_peek(0x0052);
                s_x = tmp1_U8;
      /*$E42C*/ s_status_c = (tmp1_U8 >= 0x5e);
      /*$E42E*/ branchTarget = true; block_id = (tmp1_U8 != 0x5e) ? 777 : 775;
      break;
    case 775:  // $E430
      /*$E430*/ CYCLES(0xe430, 4);
                s_x = 0xbf;
                block_id = 776;
      break;
    case 776:  // $E432
      /*$E432*/ CYCLES(0xe432, 6);
                branchTarget = true; block_id = 44;
      break;
    case 777:  // $E435
      /*$E435*/ CYCLES(0xe435, 50);
      /*$E437*/ tmp1_U8 = s_x;
                ram_poke(tmp1_U8, ram_peek(0x009d));
      /*$E43B*/ ram_poke((uint8_t)(0x01 + tmp1_U8), ram_peek(0x009e));
      /*$E43D*/ tmp6_U8 = ram_peek(0x009f);
                s_a = tmp6_U8;
      /*$E43F*/ ram_poke((uint8_t)(0x02 + tmp1_U8), tmp6_U8);
      /*$E443*/ ram_poke(0x00a0, tmp1_U8);
      /*$E445*/ ram_poke(0x00a1, 0x00);
      /*$E447*/ s_y = 0xff;
      /*$E448*/ ram_poke(0x0011, 0xff);
      /*$E44A*/ ram_poke(0x0053, tmp1_U8);
      /*$E44E*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x03);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$E44F*/ ram_poke(0x0052, tmp1_U8);
      /*$E451*/ branchTarget = true; block_id = find_block_id_func_t001(0xe451, pop16() + 1);;
      break;
    case 778:  // $E452
      /*$E452*/ CYCLES(0xe452, 4);
                ram_poke(0x0013, (ram_peek(0x0013) >> 0x01));
                block_id = 779;
      break;
    case 779:  // $E454
      /*$E454*/ CYCLES(0xe454, 18);
                tmp1_U8 = s_a;
                push8(tmp1_U8);
      /*$E458*/ tmp5_U16 = (tmp1_U8 ^ 0xff);
                tmp4_U16 = ram_peek(0x006f);
                tmp3_U16 = (tmp5_U16 + tmp4_U16) + 0x0001;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp5_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp3_U16);
      /*$E45A*/ s_y = ram_peek(0x0070);
      /*$E45C*/ branchTarget = true; block_id = (uint8_t)(tmp3_U16 >> 8) ? 781 : 780;
      break;
    case 780:  // $E45E
      /*$E45E*/ CYCLES(0xe45e, 2);
                s_y = (uint8_t)(s_y - 0x01);
                block_id = 781;
      break;
    case 781:  // $E45F
      /*$E45F*/ CYCLES(0xe45f, 7);
                tmp1_U8 = s_y;
                tmp6_U8 = ram_peek(0x006e);
                s_status_not_z = (tmp1_U8 != tmp6_U8);
                tmp6_U8 = tmp1_U8 >= tmp6_U8;
                s_status_c = tmp6_U8;
      /*$E461*/ branchTarget = true; block_id = !tmp6_U8 ? 785 : 782;
      break;
    case 782:  // $E463
      /*$E463*/ CYCLES(0xe463, 4);
                branchTarget = true; block_id = s_status_not_z ? 784 : 783;
      break;
    case 783:  // $E465
      /*$E465*/ CYCLES(0xe465, 7);
                tmp6_U8 = s_a >= ram_peek(0x006d);
                s_status_c = tmp6_U8;
      /*$E467*/ branchTarget = true; block_id = !tmp6_U8 ? 785 : 784;
      break;
    case 784:  // $E469
      /*$E469*/ CYCLES(0xe469, 19);
                tmp6_U8 = s_a;
                ram_poke(0x006f, tmp6_U8);
      /*$E46B*/ tmp1_U8 = s_y;
                ram_poke(0x0070, tmp1_U8);
      /*$E46D*/ ram_poke(0x0071, tmp6_U8);
      /*$E46F*/ ram_poke(0x0072, tmp1_U8);
      /*$E471*/ s_x = tmp6_U8;
      /*$E472*/ tmp6_U8 = pop8();
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$E473*/ branchTarget = true; block_id = find_block_id_func_t001(0xe473, pop16() + 1);;
      break;
    case 785:  // $E474
      /*$E474*/ CYCLES(0xe474, 11);
                s_x = 0x4d;
      /*$E476*/ tmp6_U8 = ram_peek(0x0013);
                s_a = tmp6_U8;
      /*$E478*/ branchTarget = true; block_id = (tmp6_U8 & 0x80) ? 776 : 786;
      break;
    case 786:  // $E47A
      /*$E47A*/ CYCLES(0xe47a, 6);
                FUNC_GARBAGE(0xe47c);
      /*$E47D*/ CYCLES(0xe47d, 12);
      /*$E47F*/ ram_poke(0x0013, 0x80);
      /*$E481*/ tmp6_U8 = pop8();
                s_a = tmp6_U8;
      /*$E482*/ branchTarget = true; block_id = tmp6_U8 ? 779 : 1661;
      break;
    case 787:  // $E484
      /*$E484*/ CYCLES(0xe484, 7);
                s_x = ram_peek(0x0073);
      /*$E486*/ s_a = ram_peek(0x0074);
                block_id = 788;
      break;
    case 788:  // $E488
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
                block_id = 789;
      break;
    case 789:  // $E4A0
      /*$E4A0*/ CYCLES(0xe4a0, 7);
      /*$E4A2*/ branchTarget = true; block_id = !(s_a != ram_peek(0x0052)) ? 791 : 790;
      break;
    case 790:  // $E4A4
      /*$E4A4*/ CYCLES(0xe4a4, 6);
                FUNC_CHKVAR(0xe4a6);
      /*$E4A7*/ CYCLES(0xe4a7, 4);
                branchTarget = true; block_id = !s_status_not_z ? 789 : 791;
      break;
    case 791:  // $E4A9
      /*$E4A9*/ CYCLES(0xe4a9, 21);
      /*$E4AB*/ ram_poke(0x008f, 0x07);
      /*$E4AD*/ tmp6_U8 = ram_peek(0x0069);
                s_a = tmp6_U8;
      /*$E4AF*/ tmp1_U8 = ram_peek(0x006a);
                s_x = tmp1_U8;
      /*$E4B1*/ ram_poke(0x005e, tmp6_U8);
      /*$E4B3*/ ram_poke(0x005f, tmp1_U8);
                block_id = 792;
      break;
    case 792:  // $E4B5
      /*$E4B5*/ CYCLES(0xe4b5, 7);
      /*$E4B7*/ branchTarget = true; block_id = (s_x != ram_peek(0x006c)) ? 794 : 793;
      break;
    case 793:  // $E4B9
      /*$E4B9*/ CYCLES(0xe4b9, 7);
      /*$E4BB*/ branchTarget = true; block_id = !(s_a != ram_peek(0x006b)) ? 796 : 794;
      break;
    case 794:  // $E4BD
      /*$E4BD*/ CYCLES(0xe4bd, 6);
                FUNC_CHKSMPLVAR(0xe4bf);
                branchTarget = true; block_id = 795;
      break;
    case 795:  // $E4C0
      /*$E4C0*/ CYCLES(0xe4c0, 4);
                branchTarget = true; block_id = !s_status_not_z ? 792 : 796;
      break;
    case 796:  // $E4C2
      /*$E4C2*/ CYCLES(0xe4c2, 14);
                ram_poke(0x0094, s_a);
      /*$E4C4*/ ram_poke(0x0095, s_x);
      /*$E4C8*/ ram_poke(0x008f, 0x03);
                block_id = 797;
      break;
    case 797:  // $E4CA
      /*$E4CA*/ CYCLES(0xe4ca, 7);
                s_a = ram_peek(0x0094);
      /*$E4CC*/ s_x = ram_peek(0x0095);
                block_id = 798;
      break;
    case 798:  // $E4CE
      /*$E4CE*/ CYCLES(0xe4ce, 7);
                tmp1_U8 = s_x;
                tmp6_U8 = ram_peek(0x006e);
                s_status_c = (tmp1_U8 >= tmp6_U8);
      /*$E4D0*/ branchTarget = true; block_id = (tmp1_U8 != tmp6_U8) ? 801 : 799;
      break;
    case 799:  // $E4D2
      /*$E4D2*/ CYCLES(0xe4d2, 7);
                tmp6_U8 = s_a;
                tmp1_U8 = ram_peek(0x006d);
                s_status_c = (tmp6_U8 >= tmp1_U8);
      /*$E4D4*/ branchTarget = true; block_id = (tmp6_U8 != tmp1_U8) ? 801 : 800;
      break;
    case 800:  // $E4D6
      /*$E4D6*/ CYCLES(0xe4d6, 6);
      /*$E562*/ CYCLES(0xe562, 7);
      /*$E564*/ branchTarget = true; block_id = !ram_peek(0x008b) ? 822 : 823;
      break;
    case 801:  // $E4D9
      /*$E4D9*/ CYCLES(0xe4d9, 52);
                ram_poke(0x005e, s_a);
      /*$E4DB*/ ram_poke(0x005f, s_x);
      /*$E4DF*/ tmp6_U8 = peek(ram_peek16al(0x005e));
      /*$E4E1*/ s_x = tmp6_U8;
      /*$E4E3*/ tmp6_U8 = peek((ram_peek16al(0x005e) + 0x0001));
      /*$E4E5*/ tmp1_U8 = s_status_c;
                push8((tmp1_U8 | ((tmp6_U8 == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | (tmp6_U8 & 0x80)));
      /*$E4E7*/ tmp6_U8 = peek((ram_peek16al(0x005e) + 0x0002));
      /*$E4E9*/ tmp3_U16 = (tmp6_U8 + ram_peek(0x0094)) + tmp1_U8;
      /*$E4EB*/ ram_poke(0x0094, ((uint8_t)tmp3_U16));
      /*$E4ED*/ s_y = 0x03;
      /*$E4EE*/ tmp6_U8 = peek((ram_peek16al(0x005e) + 0x0003));
      /*$E4F2*/ ram_poke(0x0095, (uint8_t)((tmp6_U8 + ram_peek(0x0095)) + (uint8_t)(tmp3_U16 >> 8)));
      /*$E4F4*/ tmp6_U8 = pop8();
                s_status_i = ((tmp6_U8 & 0x04) != 0);
                s_status_d = ((tmp6_U8 & 0x08) != 0);
                s_status_b = 0x00;
                s_status_v = ((tmp6_U8 & 0x40) != 0);
      /*$E4F5*/ branchTarget = true; block_id = !(tmp6_U8 & 0x80) ? 797 : 802;
      break;
    case 802:  // $E4F7
      /*$E4F7*/ CYCLES(0xe4f7, 6);
      /*$E4F8*/ branchTarget = true; block_id = (s_x & 0x80) ? 797 : 803;
      break;
    case 803:  // $E4FA
      /*$E4FA*/ CYCLES(0xe4fa, 24);
      /*$E4FB*/ tmp1_U8 = peek((ram_peek16al(0x005e) + (uint8_t)(s_y + 0x01)));
      /*$E4FD*/ s_y = 0x00;
      /*$E4FF*/ tmp3_U16 = tmp1_U8 << 0x01;
      /*$E500*/ tmp3_U16 = ((tmp3_U16 & 0x00ff) + 0x0005) + (uint8_t)(tmp3_U16 >> 8);
      /*$E502*/ tmp4_U16 = tmp3_U16 & 0x00ff;
                tmp5_U16 = ram_peek(0x005e);
                tmp3_U16 = (tmp4_U16 + tmp5_U16) + (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp5_U16);
                tmp1_U8 = (uint8_t)tmp3_U16;
                s_a = tmp1_U8;
      /*$E504*/ ram_poke(0x005e, tmp1_U8);
      /*$E506*/ branchTarget = true; block_id = !(uint8_t)(tmp3_U16 >> 8) ? 805 : 804;
      break;
    case 804:  // $E508
      /*$E508*/ CYCLES(0xe508, 4);
                ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) + 0x01));
                block_id = 805;
      break;
    case 805:  // $E50A
      /*$E50A*/ CYCLES(0xe50a, 4);
                s_x = ram_peek(0x005f);
                block_id = 806;
      break;
    case 806:  // $E50C
      /*$E50C*/ CYCLES(0xe50c, 7);
      /*$E50E*/ branchTarget = true; block_id = (s_x != ram_peek(0x0095)) ? 808 : 807;
      break;
    case 807:  // $E510
      /*$E510*/ CYCLES(0xe510, 7);
      /*$E512*/ branchTarget = true; block_id = !(s_a != ram_peek(0x0094)) ? 798 : 808;
      break;
    case 808:  // $E514
      /*$E514*/ CYCLES(0xe514, 6);
                FUNC_CHKVAR(0xe516);
      /*$E517*/ CYCLES(0xe517, 4);
                branchTarget = true; block_id = !s_status_not_z ? 806 : 1662;
      break;
    case 809:  // $E519
      /*$E519*/ CYCLES(0xe519, 7);
                tmp1_U8 = peek((ram_peek16al(0x005e) + s_y));
      /*$E51B*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 820 : 810;
      break;
    case 810:  // $E51D
      /*$E51D*/ CYCLES(0xe51d, 9);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$E51E*/ tmp1_U8 = peek((ram_peek16al(0x005e) + tmp1_U8));
      /*$E520*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 820 : 811;
      break;
    case 811:  // $E522
      /*$E522*/ CYCLES(0xe522, 2);
                s_y = (uint8_t)(s_y + 0x01);
                FUNC_CHKVAR(0x0000);
                block_id = find_block_id_func_t001(0xe522, pop16() + 1);;
      break;
    case 812:  // $E523
      /*$E523*/ CYCLES(0xe523, 7);
                tmp1_U8 = peek((ram_peek16al(0x005e) + s_y));
      /*$E525*/ branchTarget = true; block_id = !tmp1_U8 ? 820 : 813;
      break;
    case 813:  // $E527
      /*$E527*/ CYCLES(0xe527, 19);
                tmp1_U8 = s_y;
      /*$E528*/ tmp6_U8 = peek((ram_peek16al(0x005e) + (uint8_t)(tmp1_U8 + 0x01)));
      /*$E52A*/ s_x = tmp6_U8;
      /*$E52C*/ tmp1_U8 = peek((ram_peek16al(0x005e) + (uint8_t)(tmp1_U8 + 0x02)));
                s_a = tmp1_U8;
      /*$E52E*/ tmp6_U8 = ram_peek(0x0070);
                s_status_not_z = (tmp1_U8 != tmp6_U8);
      /*$E530*/ branchTarget = true; block_id = !(tmp1_U8 >= tmp6_U8) ? 816 : 814;
      break;
    case 814:  // $E532
      /*$E532*/ CYCLES(0xe532, 4);
                branchTarget = true; block_id = s_status_not_z ? 820 : 815;
      break;
    case 815:  // $E534
      /*$E534*/ CYCLES(0xe534, 7);
      /*$E536*/ branchTarget = true; block_id = (s_x >= ram_peek(0x006f)) ? 820 : 816;
      break;
    case 816:  // $E538
      /*$E538*/ CYCLES(0xe538, 7);
                tmp6_U8 = s_a;
                tmp1_U8 = ram_peek(0x009c);
                s_status_not_z = (tmp6_U8 != tmp1_U8);
      /*$E53A*/ branchTarget = true; block_id = !(tmp6_U8 >= tmp1_U8) ? 820 : 817;
      break;
    case 817:  // $E53C
      /*$E53C*/ CYCLES(0xe53c, 4);
                branchTarget = true; block_id = s_status_not_z ? 819 : 818;
      break;
    case 818:  // $E53E
      /*$E53E*/ CYCLES(0xe53e, 7);
      /*$E540*/ branchTarget = true; block_id = !(s_x >= ram_peek(0x009b)) ? 820 : 819;
      break;
    case 819:  // $E542
      /*$E542*/ CYCLES(0xe542, 28);
                ram_poke(0x009b, s_x);
      /*$E544*/ ram_poke(0x009c, s_a);
      /*$E54A*/ ram_poke(0x008a, ram_peek(0x005e));
      /*$E54C*/ ram_poke(0x008b, ram_peek(0x005f));
      /*$E550*/ ram_poke(0x0091, ram_peek(0x008f));
                block_id = 820;
      break;
    case 820:  // $E552
      /*$E552*/ CYCLES(0xe552, 16);
      /*$E555*/ tmp3_U16 = ram_peek(0x008f);
                tmp5_U16 = ram_peek(0x005e);
                tmp4_U16 = tmp3_U16 + tmp5_U16;
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = tmp1_U8;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)tmp5_U16);
                tmp6_U8 = (uint8_t)tmp4_U16;
                s_a = tmp6_U8;
      /*$E557*/ ram_poke(0x005e, tmp6_U8);
      /*$E559*/ branchTarget = true; block_id = !tmp1_U8 ? 822 : 821;
      break;
    case 821:  // $E55B
      /*$E55B*/ CYCLES(0xe55b, 4);
                ram_poke(0x005f, (uint8_t)(ram_peek(0x005f) + 0x01));
                block_id = 822;
      break;
    case 822:  // $E55D
      /*$E55D*/ CYCLES(0xe55d, 9);
                s_x = ram_peek(0x005f);
      /*$E55F*/ s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_y = 0x00;
      /*$E561*/ branchTarget = true; block_id = find_block_id_func_t001(0xe561, pop16() + 1);;
      break;
    case 823:  // $E566
      /*$E566*/ CYCLES(0xe566, 53);
      /*$E56A*/ tmp1_U8 = (ram_peek(0x0091) & 0x04) >> 0x01;
      /*$E56C*/ ram_poke(0x0091, tmp1_U8);
      /*$E56E*/ tmp1_U8 = peek((ram_peek16al(0x008a) + tmp1_U8));
      /*$E570*/ tmp4_U16 = tmp1_U8 + ram_peek(0x009b);
      /*$E572*/ ram_poke(0x0096, ((uint8_t)tmp4_U16));
      /*$E576*/ tmp5_U16 = ram_peek(0x009c);
                tmp4_U16 = tmp5_U16 + (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x0000);
      /*$E578*/ ram_poke(0x0097, ((uint8_t)tmp4_U16));
      /*$E57E*/ ram_poke(0x0094, ram_peek(0x006f));
      /*$E580*/ ram_poke(0x0095, ram_peek(0x0070));
      /*$E582*/ FUNC_MVBLKUP2(0xe584);
      /*$E585*/ CYCLES(0xe585, 31);
                tmp1_U8 = ram_peek(0x0091);
      /*$E588*/ tmp6_U8 = ram_peek(0x0094);
      /*$E58A*/ poke((ram_peek16al(0x008a) + (uint8_t)(tmp1_U8 + 0x01)), tmp6_U8);
      /*$E58C*/ s_x = tmp6_U8;
      /*$E58D*/ ram_poke(0x0095, (uint8_t)(ram_peek(0x0095) + 0x01));
      /*$E58F*/ tmp6_U8 = ram_peek(0x0095);
                s_a = tmp6_U8;
      /*$E592*/ poke((ram_peek16al(0x008a) + (uint8_t)(tmp1_U8 + 0x02)), tmp6_U8);
      /*$E594*/ branchTarget = true; block_id = 788;
      break;
    case 824:  // $E5A0
      /*$E5A0*/ CYCLES(0xe5a0, 6);
                branchTarget = true; push16(0xe5a2); block_id = 488;
      break;
    case 825:  // $E5A3
      /*$E5A3*/ CYCLES(0xe5a3, 26);
                tmp1_U8 = pop8();
      /*$E5A4*/ ram_poke(0x00ab, tmp1_U8);
      /*$E5A6*/ tmp1_U8 = pop8();
      /*$E5A7*/ ram_poke(0x00ac, tmp1_U8);
      /*$E5A9*/ s_y = 0x00;
      /*$E5AB*/ tmp1_U8 = peek(ram_peek16(0x00ab));
      /*$E5AE*/ tmp4_U16 = tmp1_U8;
                tmp1_U8 = peek(ram_peek16al(0x00a0));
                tmp5_U16 = tmp1_U8;
                tmp3_U16 = tmp4_U16 + tmp5_U16;
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = tmp1_U8;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp5_U16);
                s_a = ((uint8_t)tmp3_U16);
      /*$E5B0*/ branchTarget = true; block_id = !tmp1_U8 ? 827 : 826;
      break;
    case 826:  // $E5B2
      /*$E5B2*/ CYCLES(0xe5b2, 9);
                s_x = 0xb0;
      /*$E5B4*/ branchTarget = true; block_id = 44;
      break;
    case 827:  // $E5B7
      /*$E5B7*/ CYCLES(0xe5b7, 6);
                branchTarget = true; push16(0xe5b9); block_id = 759;
      break;
    case 828:  // $E5BA
      /*$E5BA*/ CYCLES(0xe5ba, 6);
                branchTarget = true; push16(0xe5bc); block_id = 834;
      break;
    case 829:  // $E5BD
      /*$E5BD*/ CYCLES(0xe5bd, 12);
                s_a = ram_peek(0x008c);
      /*$E5BF*/ s_y = ram_peek(0x008d);
      /*$E5C1*/ branchTarget = true; push16(0xe5c3); block_id = 844;
      break;
    case 830:  // $E5C4
      /*$E5C4*/ CYCLES(0xe5c4, 6);
                branchTarget = true; push16(0xe5c6); block_id = 836;
      break;
    case 831:  // $E5C7
      /*$E5C7*/ CYCLES(0xe5c7, 12);
                s_a = ram_peek(0x00ab);
      /*$E5C9*/ s_y = ram_peek(0x00ac);
      /*$E5CB*/ branchTarget = true; push16(0xe5cd); block_id = 844;
      break;
    case 832:  // $E5CE
      /*$E5CE*/ CYCLES(0xe5ce, 6);
                branchTarget = true; push16(0xe5d0); block_id = 774;
      break;
    case 833:  // $E5D1
      /*$E5D1*/ CYCLES(0xe5d1, 6);
                branchTarget = true; block_id = 500;
      break;
    case 834:  // $E5D4
      /*$E5D4*/ CYCLES(0xe5d4, 24);
      /*$E5D6*/ tmp6_U8 = peek(ram_peek16(0x00ab));
      /*$E5D8*/ push8(tmp6_U8);
      /*$E5DA*/ tmp6_U8 = peek((ram_peek16(0x00ab) + 0x0001));
      /*$E5DC*/ s_x = tmp6_U8;
      /*$E5DE*/ tmp6_U8 = peek((ram_peek16(0x00ab) + 0x0002));
      /*$E5E0*/ s_y = tmp6_U8;
      /*$E5E1*/ tmp6_U8 = pop8();
                s_a = tmp6_U8;
                block_id = 835;
      break;
    case 835:  // $E5E2
      /*$E5E2*/ CYCLES(0xe5e2, 7);
                ram_poke(0x005e, s_x);
      /*$E5E4*/ ram_poke(0x005f, s_y);
                block_id = 836;
      break;
    case 836:  // $E5E6
      /*$E5E6*/ CYCLES(0xe5e6, 6);
                tmp1_U8 = s_a;
                s_y = tmp1_U8;
      /*$E5E7*/ branchTarget = true; block_id = !tmp1_U8 ? 840 : 837;
      break;
    case 837:  // $E5E9
      /*$E5E9*/ CYCLES(0xe5e9, 2);
                push8(s_a);
                block_id = 838;
      break;
    case 838:  // $E5EA
      /*$E5EA*/ CYCLES(0xe5ea, 14);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp1_U8;
      /*$E5EB*/ tmp6_U8 = peek((ram_peek16al(0x005e) + tmp1_U8));
      /*$E5ED*/ poke((ram_peek16(0x0071) + tmp1_U8), tmp6_U8);
      /*$E5F0*/ branchTarget = true; block_id = tmp1_U8 ? 838 : 839;
      break;
    case 839:  // $E5F2
      /*$E5F2*/ CYCLES(0xe5f2, 2);
                tmp1_U8 = pop8();
                s_a = tmp1_U8;
                block_id = 840;
      break;
    case 840:  // $E5F3
      /*$E5F3*/ CYCLES(0xe5f3, 12);
      /*$E5F4*/ tmp3_U16 = s_a;
                tmp5_U16 = ram_peek(0x0071);
                tmp4_U16 = tmp3_U16 + tmp5_U16;
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = tmp1_U8;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)tmp5_U16);
                tmp6_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$E5F6*/ ram_poke(0x0071, tmp6_U8);
      /*$E5F8*/ branchTarget = true; block_id = !tmp1_U8 ? 842 : 841;
      break;
    case 841:  // $E5FA
      /*$E5FA*/ CYCLES(0xe5fa, 4);
                tmp1_U8 = (uint8_t)(ram_peek(0x0072) + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x0072, tmp1_U8);
                block_id = 842;
      break;
    case 842:  // $E5FC
      /*$E5FC*/ CYCLES(0xe5fc, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xe5fc, pop16() + 1);;
      break;
    case 843:  // $E600
      /*$E600*/ CYCLES(0xe600, 7);
                s_a = ram_peek(0x00a0);
      /*$E602*/ s_y = ram_peek(0x00a1);
                block_id = 844;
      break;
    case 844:  // $E604
      /*$E604*/ CYCLES(0xe604, 12);
                ram_poke(0x005e, s_a);
      /*$E606*/ ram_poke(0x005f, s_y);
      /*$E608*/ FUNC_FRETMS(0xe60a);
                branchTarget = true; block_id = 845;
      break;
    case 845:  // $E60B
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
                tmp6_U8 = (~tmp1_U8 & 2);
                s_status_not_z = tmp6_U8;
                s_status_i = ((tmp1_U8 & 0x04) != 0);
                s_status_d = ((tmp1_U8 & 0x08) != 0);
                s_status_b = 0x00;
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
      /*$E61B*/ branchTarget = true; block_id = tmp6_U8 ? 851 : 846;
      break;
    case 846:  // $E61D
      /*$E61D*/ CYCLES(0xe61d, 7);
                tmp6_U8 = s_y;
                tmp1_U8 = ram_peek(0x0070);
                tmp2_U8 = tmp6_U8 != tmp1_U8;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp6_U8 >= tmp1_U8);
                s_status_n = ((uint8_t)(tmp6_U8 - tmp1_U8) & 0x80);
      /*$E61F*/ branchTarget = true; block_id = tmp2_U8 ? 851 : 847;
      break;
    case 847:  // $E621
      /*$E621*/ CYCLES(0xe621, 7);
                tmp2_U8 = s_x;
                tmp1_U8 = ram_peek(0x006f);
                tmp6_U8 = tmp2_U8 != tmp1_U8;
                s_status_not_z = tmp6_U8;
                s_status_c = (tmp2_U8 >= tmp1_U8);
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
      /*$E623*/ branchTarget = true; block_id = tmp6_U8 ? 851 : 848;
      break;
    case 848:  // $E625
      /*$E625*/ CYCLES(0xe625, 14);
                tmp6_U8 = s_a;
                push8(tmp6_U8);
      /*$E627*/ tmp4_U16 = tmp6_U8;
                tmp5_U16 = ram_peek(0x006f);
                tmp3_U16 = tmp4_U16 + tmp5_U16;
                tmp6_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = tmp6_U8;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp5_U16);
      /*$E629*/ ram_poke(0x006f, ((uint8_t)tmp3_U16));
      /*$E62B*/ branchTarget = true; block_id = !tmp6_U8 ? 850 : 849;
      break;
    case 849:  // $E62D
      /*$E62D*/ CYCLES(0xe62d, 4);
                ram_poke(0x0070, (uint8_t)(ram_peek(0x0070) + 0x01));
                block_id = 850;
      break;
    case 850:  // $E62F
      /*$E62F*/ CYCLES(0xe62f, 2);
                tmp6_U8 = pop8();
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
                block_id = 851;
      break;
    case 851:  // $E630
      /*$E630*/ CYCLES(0xe630, 9);
                ram_poke(0x005e, s_x);
      /*$E632*/ ram_poke(0x005f, s_y);
      /*$E634*/ branchTarget = true; block_id = find_block_id_func_t001(0xe634, pop16() + 1);;
      break;
    case 852:  // $E635
      /*$E635*/ CYCLES(0xe635, 7);
                tmp6_U8 = s_y;
                tmp2_U8 = ram_peek(0x0054);
                tmp1_U8 = tmp6_U8 != tmp2_U8;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp6_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp6_U8 - tmp2_U8) & 0x80);
      /*$E637*/ branchTarget = true; block_id = tmp1_U8 ? 855 : 853;
      break;
    case 853:  // $E639
      /*$E639*/ CYCLES(0xe639, 7);
                tmp6_U8 = s_a;
                tmp1_U8 = ram_peek(0x0053);
                tmp2_U8 = tmp6_U8 != tmp1_U8;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp6_U8 >= tmp1_U8);
                s_status_n = ((uint8_t)(tmp6_U8 - tmp1_U8) & 0x80);
      /*$E63B*/ branchTarget = true; block_id = tmp2_U8 ? 855 : 854;
      break;
    case 854:  // $E63D
      /*$E63D*/ CYCLES(0xe63d, 14);
                tmp2_U8 = s_a;
                ram_poke(0x0052, tmp2_U8);
      /*$E63F*/ tmp3_U16 = tmp2_U8;
                tmp5_U16 = (tmp3_U16 - 0x0003) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)0xfffc);
                tmp2_U8 = (uint8_t)tmp5_U16;
                s_a = tmp2_U8;
      /*$E641*/ ram_poke(0x0053, tmp2_U8);
      /*$E643*/ s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_y = 0x00;
                block_id = 855;
      break;
    case 855:  // $E645
      /*$E645*/ CYCLES(0xe645, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xe645, pop16() + 1);;
      break;
    case 856:  // $E6F2
      /*$E6F2*/ CYCLES(0xe6f2, 6);
                branchTarget = true; block_id = 684;
      break;
    case 857:  // $E6F5
      /*$E6F5*/ CYCLES(0xe6f5, 6);
                FUNC_CHRGET(0xe6f7);
                branchTarget = true; block_id = 858;
      break;
    case 858:  // $E6F8
      /*$E6F8*/ CYCLES(0xe6f8, 6);
                branchTarget = true; push16(0xe6fa); block_id = 486;
      break;
    case 859:  // $E6FB
      /*$E6FB*/ CYCLES(0xe6fb, 6);
                branchTarget = true; push16(0xe6fd); block_id = 667;
      break;
    case 860:  // $E6FE
      /*$E6FE*/ CYCLES(0xe6fe, 7);
      /*$E700*/ branchTarget = true; block_id = ram_peek(0x00a0) ? 856 : 861;
      break;
    case 861:  // $E702
      /*$E702*/ CYCLES(0xe702, 9);
                s_x = ram_peek(0x00a1);
      /*$E704*/ FUNC_CHRGOT(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xe704, pop16() + 1);;
      break;
    case 862:  // $E73D
      /*$E73D*/ CYCLES(0xe73d, 16);
                tmp1_U8 = ram_peek(0x00ad);
                s_x = tmp1_U8;
      /*$E73F*/ tmp6_U8 = ram_peek(0x00ae);
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_y = tmp6_U8;
      /*$E741*/ ram_poke(0x00b8, tmp1_U8);
      /*$E743*/ ram_poke(0x00b9, tmp6_U8);
      /*$E745*/ branchTarget = true; block_id = find_block_id_func_t001(0xe745, pop16() + 1);;
      break;
    case 863:  // $E746
      /*$E746*/ CYCLES(0xe746, 6);
                branchTarget = true; push16(0xe748); block_id = 486;
      break;
    case 864:  // $E749
      /*$E749*/ CYCLES(0xe749, 6);
                branchTarget = true; push16(0xe74b); block_id = 867;
      break;
    case 865:  // $E74C
      /*$E74C*/ CYCLES(0xe74c, 6);
                branchTarget = true; push16(0xe74e); block_id = 565;
      break;
    case 866:  // $E74F
      /*$E74F*/ CYCLES(0xe74f, 6);
                branchTarget = true; block_id = 858;
      break;
    case 867:  // $E752
      /*$E752*/ CYCLES(0xe752, 11);
      /*$E754*/ tmp2_U8 = ram_peek(0x009d) >= 0x91;
                s_status_c = tmp2_U8;
      /*$E756*/ branchTarget = true; block_id = tmp2_U8 ? 856 : 868;
      break;
    case 868:  // $E758
      /*$E758*/ CYCLES(0xe758, 6);
                FUNC_QINT(0xe75a);
                branchTarget = true; block_id = 869;
      break;
    case 869:  // $E75B
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
    case 870:  // $E764
      /*$E764*/ CYCLES(0xe764, 16);
      /*$E766*/ push8(ram_peek(0x0050));
      /*$E769*/ push8(ram_peek(0x0051));
      /*$E76A*/ branchTarget = true; push16(0xe76c); block_id = 867;
      break;
    case 871:  // $E76D
      /*$E76D*/ CYCLES(0xe76d, 24);
      /*$E76F*/ tmp2_U8 = peek(ram_peek16al(0x0050));
      /*$E771*/ s_y = tmp2_U8;
      /*$E772*/ tmp2_U8 = pop8();
      /*$E773*/ ram_poke(0x0051, tmp2_U8);
      /*$E775*/ tmp2_U8 = pop8();
      /*$E776*/ ram_poke(0x0050, tmp2_U8);
      /*$E778*/ branchTarget = true; block_id = 742;
      break;
    case 872:  // $E77B
      /*$E77B*/ CYCLES(0xe77b, 6);
                branchTarget = true; push16(0xe77d); block_id = 863;
      break;
    case 873:  // $E77E
      /*$E77E*/ CYCLES(0xe77e, 11);
                tmp2_U8 = s_x;
                s_a = tmp2_U8;
      /*$E77F*/ s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_y = 0x00;
      /*$E781*/ poke(ram_peek16al(0x0050), tmp2_U8);
      /*$E783*/ branchTarget = true; block_id = find_block_id_func_t001(0xe783, pop16() + 1);;
      break;
    case 874:  // $E79F
      /*$E79F*/ CYCLES(0xe79f, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xe79f, pop16() + 1);;
      break;
    case 875:  // $E7A0
      /*$E7A0*/ CYCLES(0xe7a0, 12);
                s_a = 0x64;
      /*$E7A2*/ s_y = 0xee;
      /*$E7A4*/ branchTarget = true; block_id = 878;
      break;
    case 876:  // $E7B9
      /*$E7B9*/ CYCLES(0xe7b9, 6);
                FUNC_SHFTRGHT(0xe7bb);
                branchTarget = true; block_id = 877;
      break;
    case 877:  // $E7BC
      /*$E7BC*/ CYCLES(0xe7bc, 4);
                branchTarget = true; block_id = !s_status_c ? 889 : 878;
      break;
    case 878:  // $E7BE
      /*$E7BE*/ CYCLES(0xe7be, 6);
                FUNC_UPAY2ARG(0xe7c0);
                branchTarget = true; block_id = 879;
      break;
    case 879:  // $E7C1
      /*$E7C1*/ CYCLES(0xe7c1, 4);
                branchTarget = true; block_id = s_status_not_z ? 881 : 880;
      break;
    case 880:  // $E7C3
      /*$E7C3*/ CYCLES(0xe7c3, 6);
      /*$EB53*/ CYCLES(0xeb53, 11);
      /*$EB55*/ ram_poke(0x00a2, ram_peek(0x00aa));
      /*$EB57*/ s_x = 0x05;
                block_id = 980;
      break;
    case 881:  // $E7C6
      /*$E7C6*/ CYCLES(0xe7c6, 14);
      /*$E7C8*/ ram_poke(0x0092, ram_peek(0x00ac));
      /*$E7CA*/ s_x = 0xa5;
      /*$E7CC*/ s_a = ram_peek(0x00a5);
                block_id = 882;
      break;
    case 882:  // $E7CE
      /*$E7CE*/ CYCLES(0xe7ce, 6);
                tmp2_U8 = s_a;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_y = tmp2_U8;
      /*$E7CF*/ branchTarget = true; block_id = !tmp2_U8 ? 874 : 883;
      break;
    case 883:  // $E7D1
      /*$E7D1*/ CYCLES(0xe7d1, 9);
      /*$E7D2*/ tmp3_U16 = s_a;
                tmp5_U16 = ram_peek(0x009d);
                tmp4_U16 = tmp3_U16 - tmp5_U16;
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp5_U16));
                tmp2_U8 = (uint8_t)tmp4_U16;
                s_a = tmp2_U8;
      /*$E7D4*/ branchTarget = true; block_id = !tmp2_U8 ? 889 : 884;
      break;
    case 884:  // $E7D6
      /*$E7D6*/ CYCLES(0xe7d6, 4);
                branchTarget = true; block_id = !s_status_c ? 886 : 885;
      break;
    case 885:  // $E7D8
      /*$E7D8*/ CYCLES(0xe7d8, 31);
                ram_poke(0x009d, s_y);
      /*$E7DC*/ ram_poke(0x00a2, ram_peek(0x00aa));
      /*$E7E0*/ tmp4_U16 = (s_a ^ 0xff);
                tmp5_U16 = tmp4_U16 + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp5_U16);
      /*$E7E2*/ s_y = 0x00;
      /*$E7E4*/ ram_poke(0x0092, 0x00);
      /*$E7E6*/ s_x = 0x9d;
      /*$E7E8*/ branchTarget = true; block_id = 887;
      break;
    case 886:  // $E7EA
      /*$E7EA*/ CYCLES(0xe7ea, 7);
                s_y = 0x00;
      /*$E7EC*/ ram_poke(0x00ac, 0x00);
                block_id = 887;
      break;
    case 887:  // $E7EE
      /*$E7EE*/ CYCLES(0xe7ee, 7);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0xf9);
      /*$E7F0*/ branchTarget = true; block_id = ((uint8_t)(tmp2_U8 - 0xf9) & 0x80) ? 876 : 888;
      break;
    case 888:  // $E7F2
      /*$E7F2*/ CYCLES(0xe7f2, 14);
                s_y = s_a;
      /*$E7F3*/ s_a = ram_peek(0x00ac);
      /*$E7F5*/ tmp5_U16 = (uint8_t)(0x01 + s_x);
                tmp2_U8 = ram_peek(tmp5_U16);
                s_status_c = (tmp2_U8 & 0x01);
                ram_poke(tmp5_U16, (tmp2_U8 >> 0x01));
      /*$E7F7*/ FUNC_SHFTRGHT4(0xe7f9);
                branchTarget = true; block_id = 889;
      break;
    case 889:  // $E7FA
      /*$E7FA*/ CYCLES(0xe7fa, 7);
      /*$E7FC*/ branchTarget = true; block_id = !(ram_peek(0x00ab) & 0x80) ? 900 : 890;
      break;
    case 890:  // $E7FE
      /*$E7FE*/ CYCLES(0xe7fe, 11);
                s_y = 0x9d;
      /*$E802*/ branchTarget = true; block_id = !(s_x != 0xa5) ? 892 : 891;
      break;
    case 891:  // $E804
      /*$E804*/ CYCLES(0xe804, 4);
                s_y = 0xa5;
                block_id = 892;
      break;
    case 892:  // $E806
      /*$E806*/ CYCLES(0xe806, 60);
      /*$E809*/ tmp5_U16 = ((s_a ^ 0xff) + ram_peek(0x0092)) + 0x0001;
      /*$E80B*/ ram_poke(0x00ac, ((uint8_t)tmp5_U16));
      /*$E80D*/ tmp2_U8 = s_y;
      /*$E810*/ tmp1_U8 = s_x;
                tmp5_U16 = (ram_peek((0x0004 + tmp2_U8)) - ram_peek((uint8_t)(0x04 + tmp1_U8))) - (uint8_t)(0x01 - (uint8_t)(tmp5_U16 >> 8));
      /*$E812*/ ram_poke(0x00a1, ((uint8_t)tmp5_U16));
      /*$E817*/ tmp5_U16 = (ram_peek((0x0003 + tmp2_U8)) - ram_peek((uint8_t)(0x03 + tmp1_U8))) - ((uint8_t)(tmp5_U16 >> 8) & 0x01);
      /*$E819*/ ram_poke(0x00a0, ((uint8_t)tmp5_U16));
      /*$E81E*/ tmp5_U16 = (ram_peek((0x0002 + tmp2_U8)) - ram_peek((uint8_t)(0x02 + tmp1_U8))) - ((uint8_t)(tmp5_U16 >> 8) & 0x01);
      /*$E820*/ ram_poke(0x009f, ((uint8_t)tmp5_U16));
      /*$E825*/ tmp5_U16 = (ram_peek((0x0001 + tmp2_U8)) - ram_peek((uint8_t)(0x01 + tmp1_U8))) - ((uint8_t)(tmp5_U16 >> 8) & 0x01);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
      /*$E827*/ ram_poke(0x009e, ((uint8_t)tmp5_U16));
                block_id = 893;
      break;
    case 893:  // $E829
      /*$E829*/ CYCLES(0xe829, 4);
                branchTarget = true; block_id = s_status_c ? 895 : 894;
      break;
    case 894:  // $E82B
      /*$E82B*/ CYCLES(0xe82b, 6);
                FUNC_TWSCMPFAC(0xe82d);
                branchTarget = true; block_id = 895;
      break;
    case 895:  // $E82E
      /*$E82E*/ CYCLES(0xe82e, 7);
                s_y = 0x00;
      /*$E830*/ s_a = 0x00;
      /*$E831*/ s_status_c = 0x00;
                block_id = 896;
      break;
    case 896:  // $E832
      /*$E832*/ CYCLES(0xe832, 7);
                tmp2_U8 = ram_peek(0x009e);
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$E834*/ branchTarget = true; block_id = tmp2_U8 ? 902 : 897;
      break;
    case 897:  // $E836
      /*$E836*/ CYCLES(0xe836, 41);
      /*$E838*/ ram_poke(0x009e, ram_peek(0x009f));
      /*$E83C*/ ram_poke(0x009f, ram_peek(0x00a0));
      /*$E840*/ ram_poke(0x00a0, ram_peek(0x00a1));
      /*$E842*/ tmp2_U8 = ram_peek(0x00ac);
                s_x = tmp2_U8;
      /*$E844*/ ram_poke(0x00a1, tmp2_U8);
      /*$E846*/ ram_poke(0x00ac, s_y);
      /*$E848*/ tmp5_U16 = s_a;
                tmp3_U16 = (tmp5_U16 + 0x0008) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp5_U16, (uint8_t)0x0008);
                tmp2_U8 = (uint8_t)tmp3_U16;
                s_a = tmp2_U8;
      /*$E84A*/ s_status_c = (tmp2_U8 >= 0x20);
      /*$E84C*/ branchTarget = true; block_id = (tmp2_U8 != 0x20) ? 896 : 898;
      break;
    case 898:  // $E84E
      /*$E84E*/ CYCLES(0xe84e, 7);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$E850*/ ram_poke(0x009d, 0x00);
                block_id = 899;
      break;
    case 899:  // $E852
      /*$E852*/ CYCLES(0xe852, 6);
                ram_poke(0x00a2, s_a);
      /*$E854*/ branchTarget = true; block_id = find_block_id_func_t001(0xe854, pop16() + 1);;
      break;
    case 900:  // $E855
      /*$E855*/ CYCLES(0xe855, 53);
                tmp5_U16 = (s_a + ram_peek(0x0092)) + s_status_c;
      /*$E857*/ ram_poke(0x00ac, ((uint8_t)tmp5_U16));
      /*$E85B*/ tmp5_U16 = (ram_peek(0x00a1) + ram_peek(0x00a9)) + (uint8_t)(tmp5_U16 >> 8);
      /*$E85D*/ ram_poke(0x00a1, ((uint8_t)tmp5_U16));
      /*$E861*/ tmp5_U16 = (ram_peek(0x00a0) + ram_peek(0x00a8)) + (uint8_t)(tmp5_U16 >> 8);
      /*$E863*/ ram_poke(0x00a0, ((uint8_t)tmp5_U16));
      /*$E867*/ tmp5_U16 = (ram_peek(0x009f) + ram_peek(0x00a7)) + (uint8_t)(tmp5_U16 >> 8);
      /*$E869*/ ram_poke(0x009f, ((uint8_t)tmp5_U16));
      /*$E86D*/ tmp4_U16 = ram_peek(0x009e);
                tmp3_U16 = ram_peek(0x00a6);
                tmp5_U16 = (tmp4_U16 + tmp3_U16) + (uint8_t)(tmp5_U16 >> 8);
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)tmp3_U16);
                tmp2_U8 = (uint8_t)tmp5_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$E86F*/ ram_poke(0x009e, tmp2_U8);
      /*$E871*/ branchTarget = true; block_id = 905;
      break;
    case 901:  // $E874
      /*$E874*/ CYCLES(0xe874, 21);
                s_a = (uint8_t)((s_a + 0x0001) + s_status_c);
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
                block_id = 902;
      break;
    case 902:  // $E880
      /*$E880*/ CYCLES(0xe880, 4);
                branchTarget = true; block_id = !s_status_n ? 901 : 903;
      break;
    case 903:  // $E882
      /*$E882*/ CYCLES(0xe882, 9);
      /*$E883*/ tmp3_U16 = s_a;
                tmp5_U16 = ram_peek(0x009d);
                tmp4_U16 = tmp3_U16 - tmp5_U16;
                tmp2_U8 = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_c = tmp2_U8;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp5_U16));
                s_a = ((uint8_t)tmp4_U16);
      /*$E885*/ branchTarget = true; block_id = tmp2_U8 ? 898 : 904;
      break;
    case 904:  // $E887
      /*$E887*/ CYCLES(0xe887, 11);
      /*$E889*/ tmp4_U16 = (s_a ^ 0xff);
                tmp5_U16 = (tmp4_U16 + 0x0001) + s_status_c;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x0001);
                tmp2_U8 = (uint8_t)tmp5_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$E88B*/ ram_poke(0x009d, tmp2_U8);
                block_id = 905;
      break;
    case 905:  // $E88D
      /*$E88D*/ CYCLES(0xe88d, 4);
                branchTarget = true; block_id = !s_status_c ? 908 : 906;
      break;
    case 906:  // $E88F
      /*$E88F*/ CYCLES(0xe88f, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x009d) + 0x01);
                ram_poke(0x009d, tmp2_U8);
      /*$E891*/ branchTarget = true; block_id = !tmp2_U8 ? 916 : 907;
      break;
    case 907:  // $E893
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
                block_id = 908;
      break;
    case 908:  // $E89D
      /*$E89D*/ CYCLES(0xe89d, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xe89d, pop16() + 1);;
      break;
    case 909:  // $E89E
      /*$E89E*/ CYCLES(0xe89e, 11);
      /*$E8A2*/ ram_poke(0x00a2, (ram_peek(0x00a2) ^ 0xff));
                FUNC_TCFACMANT(0x0000);
                block_id = find_block_id_func_t001(0xe8a2, pop16() + 1);;
      break;
    case 910:  // $E8A4
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
      /*$E8C4*/ branchTarget = true; block_id = tmp2_U8 ? 915 : 1663;
      break;
    case 911:  // $E8C6
      /*$E8C6*/ CYCLES(0xe8c6, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x00a1) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x00a1, tmp2_U8);
      /*$E8C8*/ branchTarget = true; block_id = tmp2_U8 ? 915 : 912;
      break;
    case 912:  // $E8CA
      /*$E8CA*/ CYCLES(0xe8ca, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x00a0) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x00a0, tmp2_U8);
      /*$E8CC*/ branchTarget = true; block_id = tmp2_U8 ? 915 : 913;
      break;
    case 913:  // $E8CE
      /*$E8CE*/ CYCLES(0xe8ce, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x009f) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x009f, tmp2_U8);
      /*$E8D0*/ branchTarget = true; block_id = tmp2_U8 ? 915 : 914;
      break;
    case 914:  // $E8D2
      /*$E8D2*/ CYCLES(0xe8d2, 4);
                tmp2_U8 = (uint8_t)(ram_peek(0x009e) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x009e, tmp2_U8);
                block_id = 915;
      break;
    case 915:  // $E8D4
      /*$E8D4*/ CYCLES(0xe8d4, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xe8d4, pop16() + 1);;
      break;
    case 916:  // $E8D5
      /*$E8D5*/ CYCLES(0xe8d5, 9);
                s_x = 0x45;
      /*$E8D7*/ branchTarget = true; block_id = 44;
      break;
    case 917:  // $E8DC
      /*$E8DC*/ CYCLES(0xe8dc, 35);
                tmp2_U8 = s_x;
      /*$E8DE*/ ram_poke(0x00ac, ram_peek((uint8_t)(0x04 + tmp2_U8)));
      /*$E8E2*/ ram_poke((uint8_t)(0x04 + tmp2_U8), ram_peek((uint8_t)(0x03 + tmp2_U8)));
      /*$E8E6*/ ram_poke((uint8_t)(0x03 + tmp2_U8), ram_peek((uint8_t)(0x02 + tmp2_U8)));
      /*$E8EA*/ ram_poke((uint8_t)(0x02 + tmp2_U8), ram_peek((uint8_t)(0x01 + tmp2_U8)));
      /*$E8EE*/ ram_poke((uint8_t)(0x01 + tmp2_U8), ram_peek(0x00a4));
      /*$E8F0*/ CYCLES(0xe8f0, 7);
                tmp5_U16 = (s_a + 0x0008) + s_status_c;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                tmp2_U8 = (uint8_t)tmp5_U16;
                s_status_not_z = tmp2_U8;
                s_a = tmp2_U8;
      /*$E8F2*/ branchTarget = true; block_id = (tmp2_U8 & 0x80) ? 917 : 918;
      break;
    case 918:  // $E8F4
      /*$E8F4*/ CYCLES(0xe8f4, 4);
                branchTarget = true; block_id = !s_status_not_z ? 917 : 919;
      break;
    case 919:  // $E8F6
      /*$E8F6*/ CYCLES(0xe8f6, 12);
                tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 - 0x0008) - (uint8_t)(0x01 - s_status_c);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0xfff7);
      /*$E8F8*/ s_y = ((uint8_t)tmp4_U16);
      /*$E8F9*/ tmp2_U8 = ram_peek(0x00ac);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$E8FB*/ branchTarget = true; block_id = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01)) ? 923 : 920;
      break;
    case 920:  // $E8FD
      /*$E8FD*/ CYCLES(0xe8fd, 7);
                tmp4_U16 = (uint8_t)(0x01 + s_x);
                tmp5_U16 = ram_peek(tmp4_U16) << 0x01;
                tmp2_U8 = (uint8_t)(tmp5_U16 >> 8);
                s_status_c = tmp2_U8;
                ram_poke(tmp4_U16, ((uint8_t)tmp5_U16));
      /*$E8FF*/ branchTarget = true; block_id = !tmp2_U8 ? 922 : 921;
      break;
    case 921:  // $E901
      /*$E901*/ CYCLES(0xe901, 4);
                tmp5_U16 = (uint8_t)(0x01 + s_x);
                ram_poke(tmp5_U16, (uint8_t)(ram_peek(tmp5_U16) + 0x01));
                block_id = 922;
      break;
    case 922:  // $E903
      /*$E903*/ CYCLES(0xe903, 7);
                tmp2_U8 = s_x;
                tmp5_U16 = (uint8_t)(0x01 + tmp2_U8);
                tmp1_U8 = ram_peek(tmp5_U16);
                ram_poke(tmp5_U16, (uint8_t)((tmp1_U8 | (0x00 + (s_status_c << 8))) >> 0x01));
      /*$E905*/ tmp5_U16 = (uint8_t)(0x01 + tmp2_U8);
                tmp6_U8 = ram_peek(tmp5_U16);
                ram_poke(tmp5_U16, (uint8_t)((tmp6_U8 | (0x00 + ((tmp1_U8 & 0x01) << 8))) >> 0x01));
      /*$E907*/ CYCLES(0xe907, 18);
                tmp5_U16 = (uint8_t)(0x02 + tmp2_U8);
                tmp1_U8 = ram_peek(tmp5_U16);
                ram_poke(tmp5_U16, (uint8_t)((tmp1_U8 | (0x00 + ((tmp6_U8 & 0x01) << 8))) >> 0x01));
      /*$E909*/ tmp5_U16 = (uint8_t)(0x03 + tmp2_U8);
                tmp6_U8 = ram_peek(tmp5_U16);
                ram_poke(tmp5_U16, (uint8_t)((tmp6_U8 | (0x00 + ((tmp1_U8 & 0x01) << 8))) >> 0x01));
      /*$E90B*/ tmp5_U16 = (uint8_t)(0x04 + tmp2_U8);
                tmp2_U8 = ram_peek(tmp5_U16);
                ram_poke(tmp5_U16, (uint8_t)((tmp2_U8 | (0x00 + ((tmp6_U8 & 0x01) << 8))) >> 0x01));
      /*$E90D*/ s_a = (uint8_t)((s_a | (0x00 + ((tmp2_U8 & 0x01) << 8))) >> 0x01);
      /*$E90E*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_y = tmp2_U8;
      /*$E90F*/ branchTarget = true; block_id = tmp2_U8 ? 920 : 923;
      break;
    case 923:  // $E911
      /*$E911*/ CYCLES(0xe911, 4);
                s_status_c = 0x00;
      /*$E912*/ branchTarget = true; block_id = find_block_id_func_t001(0xe912, pop16() + 1);;
      break;
    case 924:  // $E97F
      /*$E97F*/ CYCLES(0xe97f, 6);
                FUNC_UPAY2ARG(0xe981);
      /*$E982*/ CYCLES(0xe982, 4);
                branchTarget = true; block_id = s_status_not_z ? 926 : 925;
      break;
    case 925:  // $E984
      /*$E984*/ CYCLES(0xe984, 6);
                branchTarget = true; block_id = 934;
      break;
    case 926:  // $E987
      /*$E987*/ CYCLES(0xe987, 6);
                branchTarget = true; push16(0xe989); block_id = 936;
      break;
    case 927:  // $E98A
      /*$E98A*/ CYCLES(0xe98a, 26);
      /*$E98C*/ ram_poke(0x0062, 0x00);
      /*$E98E*/ ram_poke(0x0063, 0x00);
      /*$E990*/ ram_poke(0x0064, 0x00);
      /*$E992*/ ram_poke(0x0065, 0x00);
      /*$E994*/ tmp2_U8 = ram_peek(0x00ac);
                s_status_not_z = tmp2_U8;
                s_a = tmp2_U8;
      /*$E996*/ FUNC_MULT1(0xe998);
      /*$E999*/ CYCLES(0xe999, 9);
                tmp2_U8 = ram_peek(0x00a1);
                s_status_not_z = tmp2_U8;
                s_a = tmp2_U8;
      /*$E99B*/ FUNC_MULT1(0xe99d);
      /*$E99E*/ CYCLES(0xe99e, 9);
                tmp2_U8 = ram_peek(0x00a0);
                s_status_not_z = tmp2_U8;
                s_a = tmp2_U8;
      /*$E9A0*/ FUNC_MULT1(0xe9a2);
      /*$E9A3*/ CYCLES(0xe9a3, 9);
                tmp2_U8 = ram_peek(0x009f);
                s_status_not_z = tmp2_U8;
                s_a = tmp2_U8;
      /*$E9A5*/ FUNC_MULT1(0xe9a7);
      /*$E9A8*/ CYCLES(0xe9a8, 9);
                s_a = ram_peek(0x009e);
      /*$E9AA*/ FUNC_MULT2(0xe9ac);
      /*$E9AD*/ CYCLES(0xe9ad, 6);
                branchTarget = true; block_id = 975;
      break;
    case 928:  // $E9B0
      /*$E9B0*/ CYCLES(0xe9b0, 4);
                branchTarget = true; block_id = s_status_not_z ? 1664 : 929;
      break;
    case 929:  // $E9B2
      /*$E9B2*/ CYCLES(0xe9b2, 6);
      /*$E8DA*/ CYCLES(0xe8da, 4);
                s_x = 0x61;
                block_id = 917;
      break;
    case 930:  // $E9B5
      /*$E9B5*/ CYCLES(0xe9b5, 6);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 & 0x01);
      /*$E9B6*/ s_a = ((tmp2_U8 >> 0x01) | 0x80);
                block_id = 931;
      break;
    case 931:  // $E9B8
      /*$E9B8*/ CYCLES(0xe9b8, 6);
                s_y = s_a;
      /*$E9B9*/ branchTarget = true; block_id = !s_status_c ? 933 : 932;
      break;
    case 932:  // $E9BB
      /*$E9BB*/ CYCLES(0xe9bb, 43);
      /*$E9BE*/ tmp5_U16 = ram_peek(0x0065) + ram_peek(0x00a9);
      /*$E9C0*/ ram_poke(0x0065, ((uint8_t)tmp5_U16));
      /*$E9C4*/ tmp5_U16 = (ram_peek(0x0064) + ram_peek(0x00a8)) + (uint8_t)(tmp5_U16 >> 8);
      /*$E9C6*/ ram_poke(0x0064, ((uint8_t)tmp5_U16));
      /*$E9CA*/ tmp5_U16 = (ram_peek(0x0063) + ram_peek(0x00a7)) + (uint8_t)(tmp5_U16 >> 8);
      /*$E9CC*/ ram_poke(0x0063, ((uint8_t)tmp5_U16));
      /*$E9D0*/ tmp4_U16 = ram_peek(0x0062);
                tmp3_U16 = ram_peek(0x00a6);
                tmp5_U16 = (tmp4_U16 + tmp3_U16) + (uint8_t)(tmp5_U16 >> 8);
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)tmp3_U16);
      /*$E9D2*/ ram_poke(0x0062, ((uint8_t)tmp5_U16));
                block_id = 933;
      break;
    case 933:  // $E9D4
      /*$E9D4*/ CYCLES(0xe9d4, 24);
                tmp2_U8 = ram_peek(0x0062);
                ram_poke(0x0062, (uint8_t)((tmp2_U8 | (0x00 + (s_status_c << 8))) >> 0x01));
      /*$E9D6*/ tmp6_U8 = ram_peek(0x0063);
                ram_poke(0x0063, (uint8_t)((tmp6_U8 | (0x00 + ((tmp2_U8 & 0x01) << 8))) >> 0x01));
      /*$E9D8*/ tmp2_U8 = ram_peek(0x0064);
                ram_poke(0x0064, (uint8_t)((tmp2_U8 | (0x00 + ((tmp6_U8 & 0x01) << 8))) >> 0x01));
      /*$E9DA*/ tmp6_U8 = ram_peek(0x0065);
                ram_poke(0x0065, (uint8_t)((tmp6_U8 | (0x00 + ((tmp2_U8 & 0x01) << 8))) >> 0x01));
      /*$E9DC*/ ram_poke(0x00ac, (uint8_t)((ram_peek(0x00ac) | (0x00 + ((tmp6_U8 & 0x01) << 8))) >> 0x01));
      /*$E9DE*/ tmp6_U8 = s_y;
      /*$E9DF*/ s_status_c = (tmp6_U8 & 0x01);
                tmp6_U8 = tmp6_U8 >> 0x01;
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$E9E0*/ branchTarget = true; block_id = tmp6_U8 ? 931 : 934;
      break;
    case 934:  // $E9E2
      /*$E9E2*/ CYCLES(0xe9e2, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xe9e2, pop16() + 1);;
      break;
    case 935:  // $E9E3
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
    case 936:  // $EA0E
      /*$EA0E*/ CYCLES(0xea0e, 7);
                tmp2_U8 = ram_peek(0x00a5);
                s_a = tmp2_U8;
      /*$EA10*/ branchTarget = true; block_id = !tmp2_U8 ? 944 : 937;
      break;
    case 937:  // $EA12
      /*$EA12*/ CYCLES(0xea12, 9);
      /*$EA13*/ tmp5_U16 = s_a;
                tmp3_U16 = ram_peek(0x009d);
                tmp4_U16 = tmp5_U16 + tmp3_U16;
                tmp6_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = tmp6_U8;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)tmp3_U16);
                tmp2_U8 = (uint8_t)tmp4_U16;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$EA15*/ branchTarget = true; block_id = !tmp6_U8 ? 940 : 938;
      break;
    case 938:  // $EA17
      /*$EA17*/ CYCLES(0xea17, 4);
                branchTarget = true; block_id = s_status_n ? 945 : 939;
      break;
    case 939:  // $EA19
      /*$EA19*/ CYCLES(0xea19, 7);
                s_status_c = 0x00;
      /*$EA1A*/ block_id = 941;
      break;
    case 940:  // $EA1B
      /*$EA1B*/ CYCLES(0xea1b, 4);
                branchTarget = true; block_id = !s_status_n ? 944 : 941;
      break;
    case 941:  // $EA1D
      /*$EA1D*/ CYCLES(0xea1d, 11);
                tmp4_U16 = s_a;
                tmp3_U16 = (tmp4_U16 + 0x0080) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0x0080);
                tmp6_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$EA1F*/ ram_poke(0x009d, tmp6_U8);
      /*$EA21*/ branchTarget = true; block_id = tmp6_U8 ? 943 : 942;
      break;
    case 942:  // $EA23
      /*$EA23*/ CYCLES(0xea23, 6);
                branchTarget = true; block_id = 899;
      break;
    case 943:  // $EA26
      /*$EA26*/ CYCLES(0xea26, 9);
                tmp6_U8 = ram_peek(0x00ab);
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$EA28*/ ram_poke(0x00a2, tmp6_U8);
      /*$EA2A*/ branchTarget = true; block_id = find_block_id_func_t001(0xea2a, pop16() + 1);;
      break;
    case 944:  // $EA31
      /*$EA31*/ CYCLES(0xea31, 9);
                tmp6_U8 = pop8();
      /*$EA32*/ tmp6_U8 = pop8();
      /*$EA33*/ branchTarget = true; block_id = 898;
      break;
    case 945:  // $EA36
      /*$EA36*/ CYCLES(0xea36, 6);
                branchTarget = true; block_id = 916;
      break;
    case 946:  // $EA39
      /*$EA39*/ CYCLES(0xea39, 6);
                branchTarget = true; push16(0xea3b); block_id = 982;
      break;
    case 947:  // $EA3C
      /*$EA3C*/ CYCLES(0xea3c, 6);
                tmp6_U8 = s_a;
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_x = tmp6_U8;
      /*$EA3D*/ branchTarget = true; block_id = !tmp6_U8 ? 951 : 948;
      break;
    case 948:  // $EA3F
      /*$EA3F*/ CYCLES(0xea3f, 9);
      /*$EA40*/ tmp3_U16 = s_a;
                tmp4_U16 = tmp3_U16 + 0x0002;
                tmp6_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = tmp6_U8;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x0002);
                s_a = ((uint8_t)tmp4_U16);
      /*$EA42*/ branchTarget = true; block_id = tmp6_U8 ? 945 : 949;
      break;
    case 949:  // $EA44
      /*$EA44*/ CYCLES(0xea44, 12);
                s_x = 0x00;
      /*$EA46*/ ram_poke(0x00ab, 0x00);
      /*$EA48*/ branchTarget = true; push16(0xea4a); block_id = 882;
      break;
    case 950:  // $EA4B
      /*$EA4B*/ CYCLES(0xea4b, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x009d) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x009d, tmp2_U8);
      /*$EA4D*/ branchTarget = true; block_id = !tmp2_U8 ? 945 : 951;
      break;
    case 951:  // $EA4F
      /*$EA4F*/ CYCLES(0xea4f, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xea4f, pop16() + 1);;
      break;
    case 952:  // $EA55
      /*$EA55*/ CYCLES(0xea55, 6);
                branchTarget = true; push16(0xea57); block_id = 982;
      break;
    case 953:  // $EA58
      /*$EA58*/ CYCLES(0xea58, 19);
                s_a = 0x50;
      /*$EA5A*/ s_y = 0xea;
      /*$EA5C*/ s_x = 0x00;
      /*$EA5E*/ ram_poke(0x00ab, 0x00);
      /*$EA60*/ FUNC_UPAY2FAC(0xea62);
      /*$EA63*/ CYCLES(0xea63, 6);
                branchTarget = true; block_id = 954;
      break;
    case 954:  // $EA69
      /*$EA69*/ CYCLES(0xea69, 4);
                branchTarget = true; block_id = !s_status_not_z ? 974 : 955;
      break;
    case 955:  // $EA6B
      /*$EA6B*/ CYCLES(0xea6b, 6);
                branchTarget = true; push16(0xea6d); block_id = 987;
      break;
    case 956:  // $EA6E
      /*$EA6E*/ CYCLES(0xea6e, 18);
      /*$EA71*/ tmp3_U16 = ram_peek(0x009d);
                tmp4_U16 = 0x0000 - tmp3_U16;
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)0x0000, (uint8_t)(~tmp3_U16));
      /*$EA73*/ ram_poke(0x009d, ((uint8_t)tmp4_U16));
      /*$EA75*/ branchTarget = true; push16(0xea77); block_id = 936;
      break;
    case 957:  // $EA78
      /*$EA78*/ CYCLES(0xea78, 7);
                tmp6_U8 = (uint8_t)(ram_peek(0x009d) + 0x01);
                ram_poke(0x009d, tmp6_U8);
      /*$EA7A*/ branchTarget = true; block_id = !tmp6_U8 ? 945 : 958;
      break;
    case 958:  // $EA7C
      /*$EA7C*/ CYCLES(0xea7c, 7);
                s_x = 0xfc;
      /*$EA7E*/ s_a = 0x01;
                block_id = 959;
      break;
    case 959:  // $EA80
      /*$EA80*/ CYCLES(0xea80, 11);
                tmp6_U8 = ram_peek(0x00a6);
      /*$EA82*/ tmp2_U8 = ram_peek(0x009e);
                tmp1_U8 = tmp6_U8 != tmp2_U8;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp6_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp6_U8 - tmp2_U8) & 0x80);
      /*$EA84*/ branchTarget = true; block_id = tmp1_U8 ? 963 : 960;
      break;
    case 960:  // $EA86
      /*$EA86*/ CYCLES(0xea86, 11);
                tmp1_U8 = ram_peek(0x00a7);
      /*$EA88*/ tmp2_U8 = ram_peek(0x009f);
                tmp6_U8 = tmp1_U8 != tmp2_U8;
                s_status_not_z = tmp6_U8;
                s_status_c = (tmp1_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
      /*$EA8A*/ branchTarget = true; block_id = tmp6_U8 ? 963 : 961;
      break;
    case 961:  // $EA8C
      /*$EA8C*/ CYCLES(0xea8c, 11);
                tmp6_U8 = ram_peek(0x00a8);
      /*$EA8E*/ tmp2_U8 = ram_peek(0x00a0);
                tmp1_U8 = tmp6_U8 != tmp2_U8;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp6_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp6_U8 - tmp2_U8) & 0x80);
      /*$EA90*/ branchTarget = true; block_id = tmp1_U8 ? 963 : 962;
      break;
    case 962:  // $EA92
      /*$EA92*/ CYCLES(0xea92, 7);
                tmp1_U8 = ram_peek(0x00a9);
      /*$EA94*/ tmp2_U8 = ram_peek(0x00a1);
                s_status_not_z = (tmp1_U8 != tmp2_U8);
                s_status_c = (tmp1_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
                block_id = 963;
      break;
    case 963:  // $EA96
      /*$EA96*/ CYCLES(0xea96, 7);
                tmp1_U8 = s_status_c;
                push8((tmp1_U8 | ((s_status_not_z == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | s_status_n));
      /*$EA97*/ tmp4_U16 = (s_a << 0x01) | tmp1_U8;
                s_a = ((uint8_t)tmp4_U16);
      /*$EA98*/ branchTarget = true; block_id = !(uint8_t)(tmp4_U16 >> 8) ? 967 : 964;
      break;
    case 964:  // $EA9A
      /*$EA9A*/ CYCLES(0xea9a, 9);
                tmp1_U8 = s_x;
                tmp2_U8 = (uint8_t)(tmp1_U8 + 0x01);
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$EA9B*/ ram_poke((uint8_t)(tmp1_U8 + 0x66), s_a);
      /*$EA9D*/ branchTarget = true; block_id = !tmp2_U8 ? 972 : 965;
      break;
    case 965:  // $EA9F
      /*$EA9F*/ CYCLES(0xea9f, 4);
                branchTarget = true; block_id = !s_status_n ? 973 : 966;
      break;
    case 966:  // $EAA1
      /*$EAA1*/ CYCLES(0xeaa1, 4);
                s_a = 0x01;
                block_id = 967;
      break;
    case 967:  // $EAA3
      /*$EAA3*/ CYCLES(0xeaa3, 6);
                tmp2_U8 = pop8();
                tmp1_U8 = tmp2_U8 & 0x01;
                s_status_c = tmp1_U8;
                s_status_i = ((tmp2_U8 & 0x04) != 0);
                s_status_d = ((tmp2_U8 & 0x08) != 0);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
      /*$EAA4*/ branchTarget = true; block_id = tmp1_U8 ? 971 : 968;
      break;
    case 968:  // $EAA6
      /*$EAA6*/ CYCLES(0xeaa6, 18);
                tmp4_U16 = ram_peek(0x00a9) << 0x01;
                ram_poke(0x00a9, ((uint8_t)tmp4_U16));
      /*$EAA8*/ tmp4_U16 = (ram_peek(0x00a8) << 0x01) | (uint8_t)(tmp4_U16 >> 8);
                ram_poke(0x00a8, ((uint8_t)tmp4_U16));
      /*$EAAA*/ tmp4_U16 = (ram_peek(0x00a7) << 0x01) | (uint8_t)(tmp4_U16 >> 8);
                ram_poke(0x00a7, ((uint8_t)tmp4_U16));
      /*$EAAC*/ tmp4_U16 = (ram_peek(0x00a6) << 0x01) | (uint8_t)(tmp4_U16 >> 8);
                tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = tmp2_U8;
                tmp1_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x00a6, tmp1_U8);
      /*$EAAE*/ branchTarget = true; block_id = tmp2_U8 ? 963 : 969;
      break;
    case 969:  // $EAB0
      /*$EAB0*/ CYCLES(0xeab0, 4);
                branchTarget = true; block_id = s_status_n ? 959 : 970;
      break;
    case 970:  // $EAB2
      /*$EAB2*/ CYCLES(0xeab2, 4);
                branchTarget = true; block_id = !s_status_n ? 963 : 971;
      break;
    case 971:  // $EAB4
      /*$EAB4*/ CYCLES(0xeab4, 50);
      /*$EAB7*/ tmp4_U16 = (ram_peek(0x00a9) - ram_peek(0x00a1)) - (uint8_t)(0x01 - s_status_c);
      /*$EAB9*/ ram_poke(0x00a9, ((uint8_t)tmp4_U16));
      /*$EABD*/ tmp4_U16 = (ram_peek(0x00a8) - ram_peek(0x00a0)) - ((uint8_t)(tmp4_U16 >> 8) & 0x01);
      /*$EABF*/ ram_poke(0x00a8, ((uint8_t)tmp4_U16));
      /*$EAC3*/ tmp4_U16 = (ram_peek(0x00a7) - ram_peek(0x009f)) - ((uint8_t)(tmp4_U16 >> 8) & 0x01);
      /*$EAC5*/ ram_poke(0x00a7, ((uint8_t)tmp4_U16));
      /*$EAC9*/ tmp3_U16 = ram_peek(0x00a6);
                tmp5_U16 = ram_peek(0x009e);
                tmp4_U16 = (tmp3_U16 - tmp5_U16) - ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp5_U16));
      /*$EACB*/ ram_poke(0x00a6, ((uint8_t)tmp4_U16));
      /*$EACD*/ s_a = s_a;
      /*$EACE*/ branchTarget = true; block_id = 968;
      break;
    case 972:  // $EAD1
      /*$EAD1*/ CYCLES(0xead1, 7);
                s_a = 0x40;
      /*$EAD3*/ branchTarget = true; block_id = 967;
      break;
    case 973:  // $EAD5
      /*$EAD5*/ CYCLES(0xead5, 21);
      /*$EADB*/ ram_poke(0x00ac, (uint8_t)(s_a << 0x06));
      /*$EADD*/ tmp2_U8 = pop8();
                s_status_i = ((tmp2_U8 & 0x04) != 0);
                s_status_d = ((tmp2_U8 & 0x08) != 0);
                s_status_b = 0x00;
      /*$EADE*/ branchTarget = true; block_id = 975;
      break;
    case 974:  // $EAE1
      /*$EAE1*/ CYCLES(0xeae1, 9);
                s_x = 0x85;
      /*$EAE3*/ branchTarget = true; block_id = 44;
      break;
    case 975:  // $EAE6
      /*$EAE6*/ CYCLES(0xeae6, 33);
      /*$EAE8*/ ram_poke(0x009e, ram_peek(0x0062));
      /*$EAEC*/ ram_poke(0x009f, ram_peek(0x0063));
      /*$EAF0*/ ram_poke(0x00a0, ram_peek(0x0064));
      /*$EAF4*/ ram_poke(0x00a1, ram_peek(0x0065));
      /*$EAF6*/ branchTarget = true; block_id = 895;
      break;
    case 976:  // $EAF9
      /*$EAF9*/ CYCLES(0xeaf9, 63);
                ram_poke(0x005e, s_a);
      /*$EAFB*/ ram_poke(0x005f, s_y);
      /*$EAFF*/ tmp6_U8 = peek((ram_peek16al(0x005e) + 0x0004));
      /*$EB01*/ ram_poke(0x00a1, tmp6_U8);
      /*$EB04*/ tmp6_U8 = peek((ram_peek16al(0x005e) + 0x0003));
      /*$EB06*/ ram_poke(0x00a0, tmp6_U8);
      /*$EB09*/ tmp6_U8 = peek((ram_peek16al(0x005e) + 0x0002));
      /*$EB0B*/ ram_poke(0x009f, tmp6_U8);
      /*$EB0E*/ tmp6_U8 = peek((ram_peek16al(0x005e) + 0x0001));
      /*$EB10*/ ram_poke(0x00a2, tmp6_U8);
      /*$EB14*/ ram_poke(0x009e, (tmp6_U8 | 0x80));
      /*$EB16*/ s_y = 0x00;
      /*$EB17*/ tmp6_U8 = peek(ram_peek16al(0x005e));
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$EB19*/ ram_poke(0x009d, tmp6_U8);
      /*$EB1B*/ ram_poke(0x00ac, 0x00);
      /*$EB1D*/ branchTarget = true; block_id = find_block_id_func_t001(0xeb1d, pop16() + 1);;
      break;
    case 977:  // $EB27
      /*$EB27*/ CYCLES(0xeb27, 7);
                s_x = ram_peek(0x0085);
      /*$EB29*/ s_y = ram_peek(0x0086);
                block_id = 978;
      break;
    case 978:  // $EB2B
      /*$EB2B*/ CYCLES(0xeb2b, 6);
                branchTarget = true; push16(0xeb2d); block_id = 987;
      break;
    case 979:  // $EB2E
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
    case 980:  // $EB59
      /*$EB59*/ CYCLES(0xeb59, 12);
                tmp2_U8 = s_x;
                tmp1_U8 = ram_peek((uint8_t)(0xa4 + tmp2_U8));
                s_a = tmp1_U8;
      /*$EB5B*/ ram_poke((uint8_t)(0x9c + tmp2_U8), tmp1_U8);
      /*$EB5D*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$EB5E*/ branchTarget = true; block_id = tmp2_U8 ? 980 : 981;
      break;
    case 981:  // $EB60
      /*$EB60*/ CYCLES(0xeb60, 6);
                ram_poke(0x00ac, s_x);
      /*$EB62*/ branchTarget = true; block_id = find_block_id_func_t001(0xeb62, pop16() + 1);;
      break;
    case 982:  // $EB63
      /*$EB63*/ CYCLES(0xeb63, 6);
                branchTarget = true; push16(0xeb65); block_id = 987;
      break;
    case 983:  // $EB66
      /*$EB66*/ CYCLES(0xeb66, 4);
                s_x = 0x06;
                block_id = 984;
      break;
    case 984:  // $EB68
      /*$EB68*/ CYCLES(0xeb68, 12);
                tmp2_U8 = s_x;
                tmp1_U8 = ram_peek((uint8_t)(0x9c + tmp2_U8));
                s_a = tmp1_U8;
      /*$EB6A*/ ram_poke((uint8_t)(0xa4 + tmp2_U8), tmp1_U8);
      /*$EB6C*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$EB6D*/ branchTarget = true; block_id = tmp2_U8 ? 984 : 985;
      break;
    case 985:  // $EB6F
      /*$EB6F*/ CYCLES(0xeb6f, 4);
                ram_poke(0x00ac, s_x);
                block_id = 986;
      break;
    case 986:  // $EB71
      /*$EB71*/ CYCLES(0xeb71, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xeb71, pop16() + 1);;
      break;
    case 987:  // $EB72
      /*$EB72*/ CYCLES(0xeb72, 7);
                tmp2_U8 = ram_peek(0x009d);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$EB74*/ branchTarget = true; block_id = !tmp2_U8 ? 986 : 988;
      break;
    case 988:  // $EB76
      /*$EB76*/ CYCLES(0xeb76, 7);
                tmp4_U16 = ram_peek(0x00ac) << 0x01;
                tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = tmp2_U8;
                tmp1_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x00ac, tmp1_U8);
      /*$EB78*/ branchTarget = true; block_id = !tmp2_U8 ? 986 : 989;
      break;
    case 989:  // $EB7A
      /*$EB7A*/ CYCLES(0xeb7a, 6);
                FUNC_INCFACMAN(0xeb7c);
                branchTarget = true; block_id = 990;
      break;
    case 990:  // $EB7D
      /*$EB7D*/ CYCLES(0xeb7d, 4);
                branchTarget = true; block_id = s_status_not_z ? 986 : 991;
      break;
    case 991:  // $EB7F
      /*$EB7F*/ CYCLES(0xeb7f, 6);
                branchTarget = true; block_id = 906;
      break;
    case 992:  // $EB82
      /*$EB82*/ CYCLES(0xeb82, 7);
                tmp6_U8 = ram_peek(0x009d);
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$EB84*/ branchTarget = true; block_id = !tmp6_U8 ? 996 : 993;
      break;
    case 993:  // $EB86
      /*$EB86*/ CYCLES(0xeb86, 4);
                s_a = ram_peek(0x00a2);
                block_id = 994;
      break;
    case 994:  // $EB88
      /*$EB88*/ CYCLES(0xeb88, 9);
                tmp2_U8 = (uint8_t)(((s_a << 0x01) | s_status_c) >> 8);
                s_status_c = tmp2_U8;
      /*$EB89*/ s_status_not_z = 0xff;
                s_status_n = 0x80;
                s_a = 0xff;
      /*$EB8B*/ branchTarget = true; block_id = tmp2_U8 ? 996 : 995;
      break;
    case 995:  // $EB8D
      /*$EB8D*/ CYCLES(0xeb8d, 4);
                s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_a = 0x01;
                block_id = 996;
      break;
    case 996:  // $EB8F
      /*$EB8F*/ CYCLES(0xeb8f, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xeb8f, pop16() + 1);;
      break;
    case 997:  // $EB93
      /*$EB93*/ CYCLES(0xeb93, 14);
                ram_poke(0x009e, s_a);
      /*$EB97*/ ram_poke(0x009f, 0x00);
      /*$EB99*/ s_x = 0x88;
                block_id = 998;
      break;
    case 998:  // $EB9B
      /*$EB9B*/ CYCLES(0xeb9b, 9);
      /*$EB9F*/ s_status_c = (uint8_t)((((ram_peek(0x009e) ^ 0xff) << 0x01) | s_status_c) >> 8);
                block_id = 999;
      break;
    case 999:  // $EBA0
      /*$EBA0*/ CYCLES(0xeba0, 26);
      /*$EBA2*/ ram_poke(0x00a1, 0x00);
      /*$EBA4*/ ram_poke(0x00a0, 0x00);
      /*$EBA6*/ ram_poke(0x009d, s_x);
      /*$EBA8*/ ram_poke(0x00ac, 0x00);
      /*$EBAA*/ ram_poke(0x00a2, 0x00);
      /*$EBAC*/ branchTarget = true; block_id = 893;
      break;
    case 1000:  // $EBB2
      /*$EBB2*/ CYCLES(0xebb2, 4);
                ram_poke(0x0060, s_a);
                FUNC_FCOMP2(0x0000);
                block_id = find_block_id_func_t001(0xebb2, pop16() + 1);;
      break;
    case 1001:  // $EBB4
      /*$EBB4*/ CYCLES(0xebb4, 18);
                ram_poke(0x0061, s_y);
      /*$EBB8*/ tmp6_U8 = peek(ram_peek16al(0x0060));
      /*$EBBA*/ s_y = 0x01;
      /*$EBBB*/ s_x = tmp6_U8;
      /*$EBBC*/ branchTarget = true; block_id = !tmp6_U8 ? 1665 : 1002;
      break;
    case 1002:  // $EBBE
      /*$EBBE*/ CYCLES(0xebbe, 11);
                tmp2_U8 = peek((ram_peek16al(0x0060) + s_y));
      /*$EBC2*/ branchTarget = true; block_id = ((tmp2_U8 ^ ram_peek(0x00a2)) & 0x80) ? 993 : 1003;
      break;
    case 1003:  // $EBC4
      /*$EBC4*/ CYCLES(0xebc4, 7);
                tmp2_U8 = s_x;
                tmp1_U8 = ram_peek(0x009d);
                s_status_c = (tmp2_U8 >= tmp1_U8);
      /*$EBC6*/ branchTarget = true; block_id = (tmp2_U8 != tmp1_U8) ? 1008 : 1004;
      break;
    case 1004:  // $EBC8
      /*$EBC8*/ CYCLES(0xebc8, 14);
                tmp1_U8 = peek((ram_peek16al(0x0060) + s_y));
      /*$EBCA*/ tmp1_U8 = tmp1_U8 | 0x80;
      /*$EBCC*/ tmp2_U8 = ram_peek(0x009e);
                s_status_c = (tmp1_U8 >= tmp2_U8);
      /*$EBCE*/ branchTarget = true; block_id = (tmp1_U8 != tmp2_U8) ? 1008 : 1005;
      break;
    case 1005:  // $EBD0
      /*$EBD0*/ CYCLES(0xebd0, 12);
                tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$EBD1*/ tmp2_U8 = peek((ram_peek16al(0x0060) + tmp2_U8));
      /*$EBD3*/ tmp1_U8 = ram_peek(0x009f);
                s_status_c = (tmp2_U8 >= tmp1_U8);
      /*$EBD5*/ branchTarget = true; block_id = (tmp2_U8 != tmp1_U8) ? 1008 : 1006;
      break;
    case 1006:  // $EBD7
      /*$EBD7*/ CYCLES(0xebd7, 12);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$EBD8*/ tmp1_U8 = peek((ram_peek16al(0x0060) + tmp1_U8));
      /*$EBDA*/ tmp2_U8 = ram_peek(0x00a0);
                s_status_c = (tmp1_U8 >= tmp2_U8);
      /*$EBDC*/ branchTarget = true; block_id = (tmp1_U8 != tmp2_U8) ? 1008 : 1007;
      break;
    case 1007:  // $EBDE
      /*$EBDE*/ CYCLES(0xebde, 19);
                tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$EBE3*/ tmp2_U8 = peek((ram_peek16al(0x0060) + tmp2_U8));
      /*$EBE5*/ tmp4_U16 = tmp2_U8;
                tmp5_U16 = ram_peek(0x00a1);
                tmp3_U16 = (tmp4_U16 - tmp5_U16) - (0x7f < ram_peek(0x00ac));
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)(~tmp5_U16));
                tmp2_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$EBE7*/ branchTarget = true; block_id = !tmp2_U8 ? 1018 : 1008;
      break;
    case 1008:  // $EBE9
      /*$EBE9*/ CYCLES(0xebe9, 7);
                s_a = ram_peek(0x00a2);
      /*$EBEB*/ branchTarget = true; block_id = !s_status_c ? 1010 : 1009;
      break;
    case 1009:  // $EBED
      /*$EBED*/ CYCLES(0xebed, 4);
                s_a = (s_a ^ 0xff);
                block_id = 1010;
      break;
    case 1010:  // $EBEF
      /*$EBEF*/ CYCLES(0xebef, 6);
                branchTarget = true; block_id = 994;
      break;
    case 1011:  // $EBF2
      /*$EBF2*/ CYCLES(0xebf2, 7);
                tmp1_U8 = ram_peek(0x009d);
                s_a = tmp1_U8;
      /*$EBF4*/ branchTarget = true; block_id = !tmp1_U8 ? 1021 : 1012;
      break;
    case 1012:  // $EBF6
      /*$EBF6*/ CYCLES(0xebf6, 12);
      /*$EBF7*/ s_a = (uint8_t)(s_a - 0x00a0);
      /*$EBF9*/ tmp2_U8 = ram_peek(0x00a2);
                s_status_v = ((tmp2_U8 >> 0x06) & 0x01);
      /*$EBFB*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1015 : 1013;
      break;
    case 1013:  // $EBFD
      /*$EBFD*/ CYCLES(0xebfd, 14);
                s_x = s_a;
      /*$EC00*/ ram_poke(0x00a4, 0xff);
      /*$EC02*/ FUNC_TCFACMANT(0xec04);
                branchTarget = true; block_id = 1014;
      break;
    case 1014:  // $EC05
      /*$EC05*/ CYCLES(0xec05, 2);
                s_a = s_x;
                block_id = 1015;
      break;
    case 1015:  // $EC06
      /*$EC06*/ CYCLES(0xec06, 11);
                s_x = 0x9d;
      /*$EC08*/ tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0xf9);
      /*$EC0A*/ branchTarget = true; block_id = !((uint8_t)(tmp2_U8 - 0xf9) & 0x80) ? 1019 : 1016;
      break;
    case 1016:  // $EC0C
      /*$EC0C*/ CYCLES(0xec0c, 6);
                FUNC_SHFTRGHT(0xec0e);
                branchTarget = true; block_id = 1017;
      break;
    case 1017:  // $EC0F
      /*$EC0F*/ CYCLES(0xec0f, 4);
                ram_poke(0x00a4, s_y);
                block_id = 1018;
      break;
    case 1018:  // $EC11
      /*$EC11*/ CYCLES(0xec11, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xec11, pop16() + 1);;
      break;
    case 1019:  // $EC12
      /*$EC12*/ CYCLES(0xec12, 24);
                s_y = s_a;
      /*$EC17*/ tmp2_U8 = ram_peek(0x009e);
                s_status_c = (tmp2_U8 & 0x01);
                ram_poke(0x009e, (tmp2_U8 >> 0x01));
      /*$EC19*/ tmp2_U8 = (ram_peek(0x00a2) & 0x80) | ram_peek(0x009e);
                s_a = tmp2_U8;
      /*$EC1B*/ ram_poke(0x009e, tmp2_U8);
      /*$EC1D*/ FUNC_SHFTRGHT4(0xec1f);
                branchTarget = true; block_id = 1020;
      break;
    case 1020:  // $EC20
      /*$EC20*/ CYCLES(0xec20, 6);
                ram_poke(0x00a4, s_y);
      /*$EC22*/ branchTarget = true; block_id = find_block_id_func_t001(0xec22, pop16() + 1);;
      break;
    case 1021:  // $EC40
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
    case 1022:  // $EC4A
      /*$EC4A*/ CYCLES(0xec4a, 7);
                s_y = 0x00;
      /*$EC4C*/ s_x = 0x0a;
                block_id = 1023;
      break;
    case 1023:  // $EC4E
      /*$EC4E*/ CYCLES(0xec4e, 9);
                tmp2_U8 = s_x;
                ram_poke((uint8_t)(0x99 + tmp2_U8), s_y);
      /*$EC50*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_x = tmp2_U8;
      /*$EC51*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1023 : 1024;
      break;
    case 1024:  // $EC53
      /*$EC53*/ CYCLES(0xec53, 4);
                branchTarget = true; block_id = !s_status_c ? 1029 : 1025;
      break;
    case 1025:  // $EC55
      /*$EC55*/ CYCLES(0xec55, 7);
                tmp2_U8 = s_a != 0x2d;
                s_status_not_z = tmp2_U8;
      /*$EC57*/ branchTarget = true; block_id = tmp2_U8 ? 1027 : 1026;
      break;
    case 1026:  // $EC59
      /*$EC59*/ CYCLES(0xec59, 7);
                ram_poke(0x00a3, s_x);
      /*$EC5B*/ branchTarget = true; block_id = !s_status_not_z ? 1028 : 1027;
      break;
    case 1027:  // $EC5D
      /*$EC5D*/ CYCLES(0xec5d, 7);
      /*$EC5F*/ branchTarget = true; block_id = (s_a != 0x2b) ? 1030 : 1028;
      break;
    case 1028:  // $EC61
      /*$EC61*/ CYCLES(0xec61, 6);
                FUNC_CHRGET(0xec63);
                branchTarget = true; block_id = 1029;
      break;
    case 1029:  // $EC64
      /*$EC64*/ CYCLES(0xec64, 4);
                branchTarget = true; block_id = !s_status_c ? 1055 : 1030;
      break;
    case 1030:  // $EC66
      /*$EC66*/ CYCLES(0xec66, 7);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x2e);
      /*$EC68*/ branchTarget = true; block_id = !(tmp2_U8 != 0x2e) ? 1043 : 1031;
      break;
    case 1031:  // $EC6A
      /*$EC6A*/ CYCLES(0xec6a, 7);
      /*$EC6C*/ branchTarget = true; block_id = (s_a != 0x45) ? 1044 : 1032;
      break;
    case 1032:  // $EC6E
      /*$EC6E*/ CYCLES(0xec6e, 6);
                FUNC_CHRGET(0xec70);
      /*$EC71*/ CYCLES(0xec71, 4);
                branchTarget = true; block_id = !s_status_c ? 1040 : 1033;
      break;
    case 1033:  // $EC73
      /*$EC73*/ CYCLES(0xec73, 7);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0xc9);
      /*$EC75*/ branchTarget = true; block_id = !(tmp2_U8 != 0xc9) ? 1038 : 1034;
      break;
    case 1034:  // $EC77
      /*$EC77*/ CYCLES(0xec77, 7);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x2d);
      /*$EC79*/ branchTarget = true; block_id = !(tmp2_U8 != 0x2d) ? 1038 : 1035;
      break;
    case 1035:  // $EC7B
      /*$EC7B*/ CYCLES(0xec7b, 7);
      /*$EC7D*/ branchTarget = true; block_id = !(s_a != 0xc8) ? 1039 : 1036;
      break;
    case 1036:  // $EC7F
      /*$EC7F*/ CYCLES(0xec7f, 7);
                tmp2_U8 = s_a;
                tmp1_U8 = tmp2_U8 != 0x2b;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp2_U8 >= 0x2b);
      /*$EC81*/ branchTarget = true; block_id = !tmp1_U8 ? 1039 : 1037;
      break;
    case 1037:  // $EC83
      /*$EC83*/ CYCLES(0xec83, 4);
                branchTarget = true; block_id = s_status_not_z ? 1041 : 1038;
      break;
    case 1038:  // $EC85
      /*$EC85*/ CYCLES(0xec85, 4);
                ram_poke(0x009c, (uint8_t)((ram_peek(0x009c) | (0x00 + (s_status_c << 8))) >> 0x01));
                block_id = 1039;
      break;
    case 1039:  // $EC87
      /*$EC87*/ CYCLES(0xec87, 6);
                FUNC_CHRGET(0xec89);
                branchTarget = true; block_id = 1040;
      break;
    case 1040:  // $EC8A
      /*$EC8A*/ CYCLES(0xec8a, 4);
                branchTarget = true; block_id = !s_status_c ? 1063 : 1041;
      break;
    case 1041:  // $EC8C
      /*$EC8C*/ CYCLES(0xec8c, 7);
      /*$EC8E*/ branchTarget = true; block_id = !(ram_peek(0x009c) & 0x80) ? 1044 : 1042;
      break;
    case 1042:  // $EC90
      /*$EC90*/ CYCLES(0xec90, 14);
      /*$EC93*/ s_a = (uint8_t)(0x0000 - ram_peek(0x009a));
      /*$EC95*/ branchTarget = true; block_id = 1045;
      break;
    case 1043:  // $EC98
      /*$EC98*/ CYCLES(0xec98, 11);
                ram_poke(0x009b, (uint8_t)((ram_peek(0x009b) | (0x00 + (s_status_c << 8))) >> 0x01));
      /*$EC9C*/ branchTarget = true; block_id = !((ram_peek(0x009b) >> 0x06) & 0x01) ? 1028 : 1044;
      break;
    case 1044:  // $EC9E
      /*$EC9E*/ CYCLES(0xec9e, 4);
                s_a = ram_peek(0x009a);
                block_id = 1045;
      break;
    case 1045:  // $ECA0
      /*$ECA0*/ CYCLES(0xeca0, 12);
      /*$ECA1*/ tmp3_U16 = s_a;
                tmp5_U16 = ram_peek(0x0099);
                tmp4_U16 = tmp3_U16 - tmp5_U16;
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)(~tmp5_U16));
                tmp1_U8 = (uint8_t)tmp4_U16;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$ECA3*/ ram_poke(0x009a, tmp1_U8);
      /*$ECA5*/ branchTarget = true; block_id = !tmp1_U8 ? 1052 : 1046;
      break;
    case 1046:  // $ECA7
      /*$ECA7*/ CYCLES(0xeca7, 4);
                branchTarget = true; block_id = !s_status_n ? 1050 : 1047;
      break;
    case 1047:  // $ECA9
      /*$ECA9*/ CYCLES(0xeca9, 6);
                branchTarget = true; push16(0xecab); block_id = 952;
      break;
    case 1048:  // $ECAC
      /*$ECAC*/ CYCLES(0xecac, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x009a) + 0x01);
                s_status_not_z = tmp2_U8;
                ram_poke(0x009a, tmp2_U8);
      /*$ECAE*/ branchTarget = true; block_id = tmp2_U8 ? 1047 : 1049;
      break;
    case 1049:  // $ECB0
      /*$ECB0*/ CYCLES(0xecb0, 4);
                branchTarget = true; block_id = !s_status_not_z ? 1052 : 1050;
      break;
    case 1050:  // $ECB2
      /*$ECB2*/ CYCLES(0xecb2, 6);
                branchTarget = true; push16(0xecb4); block_id = 946;
      break;
    case 1051:  // $ECB5
      /*$ECB5*/ CYCLES(0xecb5, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x009a) - 0x01);
                ram_poke(0x009a, tmp2_U8);
      /*$ECB7*/ branchTarget = true; block_id = tmp2_U8 ? 1050 : 1052;
      break;
    case 1052:  // $ECB9
      /*$ECB9*/ CYCLES(0xecb9, 7);
                tmp1_U8 = ram_peek(0x00a3);
                s_status_not_z = tmp1_U8;
                tmp2_U8 = tmp1_U8 & 0x80;
                s_status_n = tmp2_U8;
                s_a = tmp1_U8;
      /*$ECBB*/ branchTarget = true; block_id = tmp2_U8 ? 1054 : 1053;
      break;
    case 1053:  // $ECBD
      /*$ECBD*/ CYCLES(0xecbd, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xecbd, pop16() + 1);;
      break;
    case 1054:  // $ECBE
      /*$ECBE*/ CYCLES(0xecbe, 6);
                branchTarget = true; block_id = 1125;
      break;
    case 1055:  // $ECC1
      /*$ECC1*/ CYCLES(0xecc1, 9);
                push8(s_a);
      /*$ECC2*/ tmp2_U8 = ram_peek(0x009b);
                s_status_v = ((tmp2_U8 >> 0x06) & 0x01);
      /*$ECC4*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1057 : 1056;
      break;
    case 1056:  // $ECC6
      /*$ECC6*/ CYCLES(0xecc6, 4);
                ram_poke(0x0099, (uint8_t)(ram_peek(0x0099) + 0x01));
                block_id = 1057;
      break;
    case 1057:  // $ECC8
      /*$ECC8*/ CYCLES(0xecc8, 6);
                branchTarget = true; push16(0xecca); block_id = 946;
      break;
    case 1058:  // $ECCB
      /*$ECCB*/ CYCLES(0xeccb, 12);
                tmp6_U8 = pop8();
      /*$ECCD*/ tmp4_U16 = tmp6_U8;
                tmp3_U16 = tmp4_U16 - 0x0030;
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0xffcf);
                s_a = ((uint8_t)tmp3_U16);
      /*$ECCF*/ branchTarget = true; push16(0xecd1); block_id = 1060;
      break;
    case 1059:  // $ECD2
      /*$ECD2*/ CYCLES(0xecd2, 6);
                branchTarget = true; block_id = 1028;
      break;
    case 1060:  // $ECD5
      /*$ECD5*/ CYCLES(0xecd5, 7);
                push8(s_a);
      /*$ECD6*/ branchTarget = true; push16(0xecd8); block_id = 982;
      break;
    case 1061:  // $ECD9
      /*$ECD9*/ CYCLES(0xecd9, 7);
                tmp2_U8 = pop8();
                s_a = tmp2_U8;
      /*$ECDA*/ branchTarget = true; push16(0xecdc); block_id = 997;
      break;
    case 1062:  // $ECDD
      /*$ECDD*/ CYCLES(0xecdd, 19);
      /*$ECE1*/ ram_poke(0x00ab, (ram_peek(0x00aa) ^ ram_peek(0x00a2)));
      /*$ECE3*/ s_status_not_z = ram_peek(0x009d);
      /*$ECE5*/ branchTarget = true; block_id = 879;
      break;
    case 1063:  // $ECE8
      /*$ECE8*/ CYCLES(0xece8, 11);
                tmp1_U8 = ram_peek(0x009a);
                s_a = tmp1_U8;
      /*$ECEA*/ tmp1_U8 = tmp1_U8 >= 0x0a;
                s_status_c = tmp1_U8;
      /*$ECEC*/ branchTarget = true; block_id = !tmp1_U8 ? 1066 : 1064;
      break;
    case 1064:  // $ECEE
      /*$ECEE*/ CYCLES(0xecee, 11);
                s_a = 0x64;
      /*$ECF2*/ branchTarget = true; block_id = (ram_peek(0x009c) & 0x80) ? 1067 : 1065;
      break;
    case 1065:  // $ECF4
      /*$ECF4*/ CYCLES(0xecf4, 6);
                branchTarget = true; block_id = 916;
      break;
    case 1066:  // $ECF7
      /*$ECF7*/ CYCLES(0xecf7, 24);
      /*$ECFE*/ s_y = 0x00;
      /*$ED00*/ tmp2_U8 = peek(ram_peek16al(0x00b8));
      /*$ED03*/ s_a = (uint8_t)(((((s_a << 0x02) + ram_peek(0x009a)) << 0x01) + tmp2_U8) - 0x0030);
                block_id = 1067;
      break;
    case 1067:  // $ED05
      /*$ED05*/ CYCLES(0xed05, 9);
                ram_poke(0x009a, s_a);
      /*$ED07*/ branchTarget = true; block_id = 1039;
      break;
    case 1068:  // $ED19
      /*$ED19*/ CYCLES(0xed19, 12);
                s_a = 0x58;
      /*$ED1B*/ s_y = 0xd3;
      /*$ED1D*/ branchTarget = true; push16(0xed1f); block_id = 1072;
      break;
    case 1069:  // $ED20
      /*$ED20*/ CYCLES(0xed20, 7);
                s_a = ram_peek(0x0076);
      /*$ED22*/ s_x = ram_peek(0x0075);
                block_id = 1070;
      break;
    case 1070:  // $ED24
      /*$ED24*/ CYCLES(0xed24, 18);
                ram_poke(0x009e, s_a);
      /*$ED26*/ ram_poke(0x009f, s_x);
      /*$ED28*/ s_x = 0x90;
      /*$ED2A*/ s_status_c = 0x01;
      /*$ED2B*/ branchTarget = true; push16(0xed2d); block_id = 999;
      break;
    case 1071:  // $ED2E
      /*$ED2E*/ CYCLES(0xed2e, 6);
                branchTarget = true; push16(0xed30); block_id = 1073;
      break;
    case 1072:  // $ED31
      /*$ED31*/ CYCLES(0xed31, 6);
                branchTarget = true; block_id = 380;
      break;
    case 1073:  // $ED34
      /*$ED34*/ CYCLES(0xed34, 16);
      /*$ED36*/ s_a = 0x2d;
      /*$ED38*/ s_y = 0x00;
      /*$ED39*/ tmp1_U8 = ram_peek(0x00a2);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
      /*$ED3B*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 1075 : 1074;
      break;
    case 1074:  // $ED3D
      /*$ED3D*/ CYCLES(0xed3d, 7);
                tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$ED3E*/ ram_poke((0x00ff + tmp2_U8), s_a);
                block_id = 1075;
      break;
    case 1075:  // $ED41
      /*$ED41*/ CYCLES(0xed41, 19);
                ram_poke(0x00a2, s_a);
      /*$ED43*/ tmp2_U8 = s_y;
                ram_poke(0x00ad, tmp2_U8);
      /*$ED45*/ s_y = (uint8_t)(tmp2_U8 + 0x01);
      /*$ED46*/ s_a = 0x30;
      /*$ED48*/ tmp2_U8 = ram_peek(0x009d);
                s_x = tmp2_U8;
      /*$ED4A*/ branchTarget = true; block_id = tmp2_U8 ? 1077 : 1076;
      break;
    case 1076:  // $ED4C
      /*$ED4C*/ CYCLES(0xed4c, 6);
      /*$EE57*/ CYCLES(0xee57, 6);
                ram_poke((0x00ff + s_y), s_a);
                block_id = 1123;
      break;
    case 1077:  // $ED4F
      /*$ED4F*/ CYCLES(0xed4f, 11);
                s_a = 0x00;
      /*$ED51*/ tmp2_U8 = s_x;
                s_status_c = (tmp2_U8 >= 0x80);
      /*$ED53*/ branchTarget = true; block_id = !(tmp2_U8 != 0x80) ? 1079 : 1078;
      break;
    case 1078:  // $ED55
      /*$ED55*/ CYCLES(0xed55, 4);
                branchTarget = true; block_id = s_status_c ? 1081 : 1079;
      break;
    case 1079:  // $ED57
      /*$ED57*/ CYCLES(0xed57, 12);
                s_a = 0x14;
      /*$ED59*/ s_y = 0xed;
      /*$ED5B*/ branchTarget = true; push16(0xed5d); block_id = 924;
      break;
    case 1080:  // $ED5E
      /*$ED5E*/ CYCLES(0xed5e, 4);
                s_a = 0xf7;
                block_id = 1081;
      break;
    case 1081:  // $ED60
      /*$ED60*/ CYCLES(0xed60, 4);
                ram_poke(0x0099, s_a);
                block_id = 1082;
      break;
    case 1082:  // $ED62
      /*$ED62*/ CYCLES(0xed62, 12);
                s_a = 0x0f;
      /*$ED64*/ s_y = 0xed;
      /*$ED66*/ FUNC_FCOMP(0xed68);
                branchTarget = true; block_id = 1083;
      break;
    case 1083:  // $ED69
      /*$ED69*/ CYCLES(0xed69, 4);
                branchTarget = true; block_id = !s_status_not_z ? 1093 : 1084;
      break;
    case 1084:  // $ED6B
      /*$ED6B*/ CYCLES(0xed6b, 4);
                branchTarget = true; block_id = !s_status_n ? 1090 : 1085;
      break;
    case 1085:  // $ED6D
      /*$ED6D*/ CYCLES(0xed6d, 12);
                s_a = 0x0a;
      /*$ED6F*/ s_y = 0xed;
      /*$ED71*/ FUNC_FCOMP(0xed73);
                branchTarget = true; block_id = 1086;
      break;
    case 1086:  // $ED74
      /*$ED74*/ CYCLES(0xed74, 4);
                branchTarget = true; block_id = !s_status_not_z ? 1088 : 1087;
      break;
    case 1087:  // $ED76
      /*$ED76*/ CYCLES(0xed76, 4);
                branchTarget = true; block_id = !s_status_n ? 1092 : 1088;
      break;
    case 1088:  // $ED78
      /*$ED78*/ CYCLES(0xed78, 6);
                branchTarget = true; push16(0xed7a); block_id = 946;
      break;
    case 1089:  // $ED7B
      /*$ED7B*/ CYCLES(0xed7b, 7);
                tmp6_U8 = (uint8_t)(ram_peek(0x0099) - 0x01);
                ram_poke(0x0099, tmp6_U8);
      /*$ED7D*/ branchTarget = true; block_id = tmp6_U8 ? 1085 : 1090;
      break;
    case 1090:  // $ED7F
      /*$ED7F*/ CYCLES(0xed7f, 6);
                branchTarget = true; push16(0xed81); block_id = 952;
      break;
    case 1091:  // $ED82
      /*$ED82*/ CYCLES(0xed82, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x0099) + 0x01);
                ram_poke(0x0099, tmp2_U8);
      /*$ED84*/ branchTarget = true; block_id = tmp2_U8 ? 1082 : 1092;
      break;
    case 1092:  // $ED86
      /*$ED86*/ CYCLES(0xed86, 6);
                branchTarget = true; push16(0xed88); block_id = 875;
      break;
    case 1093:  // $ED89
      /*$ED89*/ CYCLES(0xed89, 6);
                FUNC_QINT(0xed8b);
                branchTarget = true; block_id = 1094;
      break;
    case 1094:  // $ED8C
      /*$ED8C*/ CYCLES(0xed8c, 16);
                s_x = 0x01;
      /*$ED91*/ tmp2_U8 = (uint8_t)(ram_peek(0x0099) + 0x000a);
                s_a = tmp2_U8;
      /*$ED93*/ branchTarget = true; block_id = (tmp2_U8 & 0x80) ? 1097 : 1095;
      break;
    case 1095:  // $ED95
      /*$ED95*/ CYCLES(0xed95, 7);
                tmp2_U8 = s_a >= 0x0b;
                s_status_c = tmp2_U8;
      /*$ED97*/ branchTarget = true; block_id = tmp2_U8 ? 1098 : 1096;
      break;
    case 1096:  // $ED99
      /*$ED99*/ CYCLES(0xed99, 9);
      /*$ED9B*/ s_x = (uint8_t)((s_a + 0x00ff) + s_status_c);
      /*$ED9C*/ s_a = 0x02;
                block_id = 1097;
      break;
    case 1097:  // $ED9E
      /*$ED9E*/ CYCLES(0xed9e, 2);
                s_status_c = 0x01;
                block_id = 1098;
      break;
    case 1098:  // $ED9F
      /*$ED9F*/ CYCLES(0xed9f, 16);
      /*$EDA1*/ ram_poke(0x009a, (uint8_t)((s_a - 0x0002) - (uint8_t)(0x01 - s_status_c)));
      /*$EDA3*/ tmp2_U8 = s_x;
                ram_poke(0x0099, tmp2_U8);
      /*$EDA5*/ s_status_n = (tmp2_U8 & 0x80);
      /*$EDA6*/ branchTarget = true; block_id = !tmp2_U8 ? 1100 : 1099;
      break;
    case 1099:  // $EDA8
      /*$EDA8*/ CYCLES(0xeda8, 4);
                branchTarget = true; block_id = !s_status_n ? 1103 : 1100;
      break;
    case 1100:  // $EDAA
      /*$EDAA*/ CYCLES(0xedaa, 19);
      /*$EDAE*/ tmp2_U8 = (uint8_t)(ram_peek(0x00ad) + 0x01);
                s_y = tmp2_U8;
      /*$EDAF*/ ram_poke((0x00ff + tmp2_U8), 0x2e);
      /*$EDB3*/ branchTarget = true; block_id = !s_x ? 1102 : 1101;
      break;
    case 1101:  // $EDB5
      /*$EDB5*/ CYCLES(0xedb5, 11);
      /*$EDB7*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$EDB8*/ ram_poke((0x00ff + tmp2_U8), 0x30);
                block_id = 1102;
      break;
    case 1102:  // $EDBB
      /*$EDBB*/ CYCLES(0xedbb, 4);
                ram_poke(0x00ad, s_y);
                block_id = 1103;
      break;
    case 1103:  // $EDBD
      /*$EDBD*/ CYCLES(0xedbd, 7);
                s_y = 0x00;
      /*$EDBF*/ s_x = 0x80;
                block_id = 1104;
      break;
    case 1104:  // $EDC1
      /*$EDC1*/ CYCLES(0xedc1, 55);
      /*$EDC4*/ tmp2_U8 = s_y;
                tmp4_U16 = ram_peek(0x00a1) + ram_peek((0xee6c + tmp2_U8));
      /*$EDC7*/ ram_poke(0x00a1, ((uint8_t)tmp4_U16));
      /*$EDCB*/ tmp4_U16 = (ram_peek(0x00a0) + ram_peek((0xee6b + tmp2_U8))) + (uint8_t)(tmp4_U16 >> 8);
      /*$EDCE*/ ram_poke(0x00a0, ((uint8_t)tmp4_U16));
      /*$EDD2*/ tmp4_U16 = (ram_peek(0x009f) + ram_peek((0xee6a + tmp2_U8))) + (uint8_t)(tmp4_U16 >> 8);
      /*$EDD5*/ ram_poke(0x009f, ((uint8_t)tmp4_U16));
      /*$EDD9*/ tmp4_U16 = (ram_peek(0x009e) + ram_peek((0xee69 + tmp2_U8))) + (uint8_t)(tmp4_U16 >> 8);
                tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = tmp2_U8;
      /*$EDDC*/ ram_poke(0x009e, ((uint8_t)tmp4_U16));
      /*$EDDE*/ tmp1_U8 = (uint8_t)(s_x + 0x01);
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$EDDF*/ branchTarget = true; block_id = tmp2_U8 ? 1107 : 1105;
      break;
    case 1105:  // $EDE1
      /*$EDE1*/ CYCLES(0xede1, 4);
                branchTarget = true; block_id = !s_status_n ? 1104 : 1106;
      break;
    case 1106:  // $EDE3
      /*$EDE3*/ CYCLES(0xede3, 4);
                branchTarget = true; block_id = s_status_n ? 1108 : 1107;
      break;
    case 1107:  // $EDE5
      /*$EDE5*/ CYCLES(0xede5, 4);
                branchTarget = true; block_id = s_status_n ? 1104 : 1108;
      break;
    case 1108:  // $EDE7
      /*$EDE7*/ CYCLES(0xede7, 6);
                s_a = s_x;
      /*$EDE8*/ branchTarget = true; block_id = !s_status_c ? 1110 : 1109;
      break;
    case 1109:  // $EDEA
      /*$EDEA*/ CYCLES(0xedea, 7);
      /*$EDEC*/ tmp5_U16 = ((s_a ^ 0xff) + 0x000a) + s_status_c;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 1110;
      break;
    case 1110:  // $EDEE
      /*$EDEE*/ CYCLES(0xedee, 36);
                tmp4_U16 = s_a;
                tmp5_U16 = (tmp4_U16 + 0x002f) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x002f);
                tmp2_U8 = (uint8_t)tmp5_U16;
      /*$EDF4*/ ram_poke(0x0083, (uint8_t)(s_y + 0x04));
      /*$EDF8*/ tmp1_U8 = (uint8_t)(ram_peek(0x00ad) + 0x01);
                s_y = tmp1_U8;
      /*$EDF9*/ s_x = tmp2_U8;
      /*$EDFC*/ ram_poke((0x00ff + tmp1_U8), (tmp2_U8 & 0x7f));
      /*$EDFF*/ tmp1_U8 = (uint8_t)(ram_peek(0x0099) - 0x01);
                ram_poke(0x0099, tmp1_U8);
      /*$EE01*/ branchTarget = true; block_id = tmp1_U8 ? 1112 : 1111;
      break;
    case 1111:  // $EE03
      /*$EE03*/ CYCLES(0xee03, 11);
      /*$EE05*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$EE06*/ ram_poke((0x00ff + tmp1_U8), 0x2e);
                block_id = 1112;
      break;
    case 1112:  // $EE09
      /*$EE09*/ CYCLES(0xee09, 24);
                ram_poke(0x00ad, s_y);
      /*$EE0B*/ tmp1_U8 = ram_peek(0x0083);
                s_y = tmp1_U8;
      /*$EE12*/ s_x = ((s_x ^ 0xff) & 0x80);
      /*$EE15*/ branchTarget = true; block_id = (tmp1_U8 != 0x24) ? 1104 : 1113;
      break;
    case 1113:  // $EE17
      /*$EE17*/ CYCLES(0xee17, 4);
                s_y = ram_peek(0x00ad);
                block_id = 1114;
      break;
    case 1114:  // $EE19
      /*$EE19*/ CYCLES(0xee19, 14);
                tmp1_U8 = s_y;
                tmp2_U8 = ram_peek((0x00ff + tmp1_U8));
                s_a = tmp2_U8;
      /*$EE1C*/ s_y = (uint8_t)(tmp1_U8 - 0x01);
      /*$EE1F*/ branchTarget = true; block_id = !(tmp2_U8 != 0x30) ? 1114 : 1115;
      break;
    case 1115:  // $EE21
      /*$EE21*/ CYCLES(0xee21, 7);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x2e);
      /*$EE23*/ branchTarget = true; block_id = !(tmp2_U8 != 0x2e) ? 1117 : 1116;
      break;
    case 1116:  // $EE25
      /*$EE25*/ CYCLES(0xee25, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 1117;
      break;
    case 1117:  // $EE26
      /*$EE26*/ CYCLES(0xee26, 11);
                s_a = 0x2b;
      /*$EE28*/ tmp2_U8 = ram_peek(0x009a);
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$EE2A*/ branchTarget = true; block_id = !tmp2_U8 ? 1123 : 1118;
      break;
    case 1118:  // $EE2C
      /*$EE2C*/ CYCLES(0xee2c, 4);
                branchTarget = true; block_id = !s_status_n ? 1120 : 1119;
      break;
    case 1119:  // $EE2E
      /*$EE2E*/ CYCLES(0xee2e, 14);
      /*$EE33*/ s_x = (uint8_t)(0x0000 - ram_peek(0x009a));
      /*$EE34*/ s_a = 0x2d;
                block_id = 1120;
      break;
    case 1120:  // $EE36
      /*$EE36*/ CYCLES(0xee36, 21);
                tmp2_U8 = s_y;
                ram_poke((0x0101 + tmp2_U8), s_a);
      /*$EE3B*/ ram_poke((0x0100 + tmp2_U8), 0x45);
      /*$EE3E*/ s_a = s_x;
      /*$EE3F*/ s_x = 0x2f;
      /*$EE41*/ s_status_c = 0x01;
                block_id = 1121;
      break;
    case 1121:  // $EE42
      /*$EE42*/ CYCLES(0xee42, 9);
                s_x = (uint8_t)(s_x + 0x01);
      /*$EE43*/ tmp5_U16 = (s_a - 0x000a) - (uint8_t)(0x01 - s_status_c);
                tmp2_U8 = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
                s_status_c = tmp2_U8;
                s_a = ((uint8_t)tmp5_U16);
      /*$EE45*/ branchTarget = true; block_id = tmp2_U8 ? 1121 : 1122;
      break;
    case 1122:  // $EE47
      /*$EE47*/ CYCLES(0xee47, 28);
                tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 + 0x003a) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x003a);
      /*$EE49*/ tmp2_U8 = s_y;
                ram_poke((0x0103 + tmp2_U8), ((uint8_t)tmp4_U16));
      /*$EE4D*/ ram_poke((0x0102 + tmp2_U8), s_x);
      /*$EE52*/ ram_poke((0x0104 + tmp2_U8), 0x00);
      /*$EE55*/ branchTarget = true; block_id = 1124;
      break;
    case 1123:  // $EE5A
      /*$EE5A*/ CYCLES(0xee5a, 9);
      /*$EE5C*/ ram_poke((0x0100 + s_y), 0x00);
                block_id = 1124;
      break;
    case 1124:  // $EE5F
      /*$EE5F*/ CYCLES(0xee5f, 9);
                s_a = 0x00;
      /*$EE61*/ s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_y = 0x01;
      /*$EE63*/ branchTarget = true; block_id = find_block_id_func_t001(0xee63, pop16() + 1);;
      break;
    case 1125:  // $EED0
      /*$EED0*/ CYCLES(0xeed0, 7);
                tmp6_U8 = ram_peek(0x009d);
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$EED2*/ branchTarget = true; block_id = !tmp6_U8 ? 1127 : 1126;
      break;
    case 1126:  // $EED4
      /*$EED4*/ CYCLES(0xeed4, 11);
      /*$EED6*/ tmp2_U8 = ram_peek(0x00a2) ^ 0xff;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$EED8*/ ram_poke(0x00a2, tmp2_U8);
                block_id = 1127;
      break;
    case 1127:  // $EEDA
      /*$EEDA*/ CYCLES(0xeeda, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xeeda, pop16() + 1);;
      break;
    case 1128:  // $F13E
      /*$F13E*/ CYCLES(0xf13e, 35);
      /*$F140*/ ram_poke(0x0000, 0x4c);
      /*$F142*/ ram_poke(0x0003, 0x4c);
      /*$F144*/ ram_poke(0x0090, 0x4c);
      /*$F146*/ ram_poke(0x000a, 0x4c);
      /*$F14A*/ s_y = 0xe1;
      /*$F14C*/ ram_poke(0x000b, 0x99);
      /*$F14E*/ ram_poke(0x000c, 0xe1);
      /*$F150*/ s_x = 0x1c;
                block_id = 1129;
      break;
    case 1129:  // $F152
      /*$F152*/ CYCLES(0xf152, 18);
                tmp2_U8 = s_x;
      /*$F155*/ ram_poke((uint8_t)(0xb0 + tmp2_U8), ram_peek((0xf10a + tmp2_U8)));
      /*$F157*/ ram_poke(0x00f1, tmp2_U8);
      /*$F159*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_x = tmp2_U8;
      /*$F15A*/ branchTarget = true; block_id = tmp2_U8 ? 1129 : 1130;
      break;
    case 1130:  // $F15C
      /*$F15C*/ CYCLES(0xf15c, 26);
                tmp2_U8 = s_x;
                ram_poke(0x00f2, tmp2_U8);
      /*$F15F*/ ram_poke(0x00a4, tmp2_U8);
      /*$F161*/ ram_poke(0x0054, tmp2_U8);
      /*$F163*/ push8(tmp2_U8);
      /*$F166*/ ram_poke(0x008f, 0x03);
      /*$F168*/ branchTarget = true; push16(0xf16a); block_id = 361;
      break;
    case 1131:  // $F16B
      /*$F16B*/ CYCLES(0xf16b, 38);
      /*$F16D*/ ram_poke(0x01fd, 0x01);
      /*$F170*/ ram_poke(0x01fc, 0x01);
      /*$F175*/ ram_poke(0x0052, 0x55);
      /*$F17B*/ ram_poke(0x0050, 0x00);
      /*$F17D*/ ram_poke(0x0051, 0x08);
      /*$F17F*/ s_y = 0x00;
                block_id = 1132;
      break;
    case 1132:  // $F181
      /*$F181*/ CYCLES(0xf181, 21);
                ram_poke(0x0051, (uint8_t)(ram_peek(0x0051) + 0x01));
      /*$F183*/ tmp2_U8 = s_y;
                tmp1_U8 = peek((ram_peek16al(0x0050) + tmp2_U8));
      /*$F185*/ tmp1_U8 = tmp1_U8 ^ 0xff;
                s_a = tmp1_U8;
      /*$F187*/ poke((ram_peek16al(0x0050) + tmp2_U8), tmp1_U8);
      /*$F189*/ tmp2_U8 = peek((ram_peek16al(0x0050) + tmp2_U8));
                s_status_c = (tmp1_U8 >= tmp2_U8);
      /*$F18B*/ branchTarget = true; block_id = (tmp1_U8 != tmp2_U8) ? 1134 : 1133;
      break;
    case 1133:  // $F18D
      /*$F18D*/ CYCLES(0xf18d, 14);
                tmp1_U8 = s_a ^ 0xff;
      /*$F18F*/ tmp2_U8 = s_y;
                poke((ram_peek16al(0x0050) + tmp2_U8), tmp1_U8);
      /*$F191*/ tmp2_U8 = peek((ram_peek16al(0x0050) + tmp2_U8));
                s_status_c = (tmp1_U8 >= tmp2_U8);
      /*$F193*/ branchTarget = true; block_id = !(tmp1_U8 != tmp2_U8) ? 1132 : 1134;
      break;
    case 1134:  // $F195
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
      /*$F1B4*/ branchTarget = true; block_id = tmp1_U8 ? 1136 : 1135;
      break;
    case 1135:  // $F1B6
      /*$F1B6*/ CYCLES(0xf1b6, 4);
                ram_poke(0x0068, (uint8_t)(ram_peek(0x0068) + 0x01));
                block_id = 1136;
      break;
    case 1136:  // $F1B8
      /*$F1B8*/ CYCLES(0xf1b8, 12);
                s_a = ram_peek(0x0067);
      /*$F1BA*/ s_y = ram_peek(0x0068);
      /*$F1BC*/ branchTarget = true; push16(0xf1be); block_id = 32;
      break;
    case 1137:  // $F1BF
      /*$F1BF*/ CYCLES(0xf1bf, 6);
                branchTarget = true; push16(0xf1c1); block_id = 145;
      break;
    case 1138:  // $F1C2
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
    case 1139:  // $F1D5
      /*$F1D5*/ CYCLES(0xf1d5, 6);
                branchTarget = true; push16(0xf1d7); block_id = 486;
      break;
    case 1140:  // $F1D8
      /*$F1D8*/ CYCLES(0xf1d8, 6);
                branchTarget = true; push16(0xf1da); block_id = 867;
      break;
    case 1141:  // $F1DB
      /*$F1DB*/ CYCLES(0xf1db, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xf1db, ram_peek16al(0x0050));
      break;
    case 1142:  // $F1EC
      /*$F1EC*/ CYCLES(0xf1ec, 6);
                branchTarget = true; push16(0xf1ee); block_id = 858;
      break;
    case 1143:  // $F1EF
      /*$F1EF*/ CYCLES(0xf1ef, 7);
                tmp2_U8 = s_x >= 0x30;
                s_status_c = tmp2_U8;
      /*$F1F1*/ branchTarget = true; block_id = tmp2_U8 ? 1148 : 1144;
      break;
    case 1144:  // $F1F3
      /*$F1F3*/ CYCLES(0xf1f3, 12);
                ram_poke(0x00f0, s_x);
      /*$F1F5*/ s_a = 0x2c;
      /*$F1F7*/ branchTarget = true; push16(0xf1f9); block_id = 566;
      break;
    case 1145:  // $F1FA
      /*$F1FA*/ CYCLES(0xf1fa, 6);
                branchTarget = true; push16(0xf1fc); block_id = 858;
      break;
    case 1146:  // $F1FD
      /*$F1FD*/ CYCLES(0xf1fd, 7);
                tmp2_U8 = s_x;
                s_status_not_z = (tmp2_U8 != 0x30);
                tmp1_U8 = tmp2_U8 >= 0x30;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - 0x30) & 0x80);
      /*$F1FF*/ branchTarget = true; block_id = tmp1_U8 ? 1148 : 1147;
      break;
    case 1147:  // $F201
      /*$F201*/ CYCLES(0xf201, 9);
                tmp2_U8 = s_x;
                ram_poke(0x002c, tmp2_U8);
      /*$F203*/ ram_poke(0x002d, tmp2_U8);
      /*$F205*/ branchTarget = true; block_id = find_block_id_func_t001(0xf205, pop16() + 1);;
      break;
    case 1148:  // $F206
      /*$F206*/ CYCLES(0xf206, 6);
                branchTarget = true; block_id = 684;
      break;
    case 1149:  // $F209
      /*$F209*/ CYCLES(0xf209, 6);
                branchTarget = true; push16(0xf20b); block_id = 1142;
      break;
    case 1150:  // $F20C
      /*$F20C*/ CYCLES(0xf20c, 7);
                tmp2_U8 = s_x >= ram_peek(0x00f0);
                s_status_c = tmp2_U8;
      /*$F20E*/ branchTarget = true; block_id = tmp2_U8 ? 1152 : 1151;
      break;
    case 1151:  // $F210
      /*$F210*/ CYCLES(0xf210, 14);
                tmp2_U8 = ram_peek(0x00f0);
      /*$F212*/ ram_poke(0x002c, tmp2_U8);
      /*$F214*/ ram_poke(0x002d, tmp2_U8);
      /*$F216*/ ram_poke(0x00f0, s_x);
                block_id = 1152;
      break;
    case 1152:  // $F218
      /*$F218*/ CYCLES(0xf218, 9);
                s_a = 0xc5;
      /*$F21A*/ branchTarget = true; push16(0xf21c); block_id = 566;
      break;
    case 1153:  // $F21D
      /*$F21D*/ CYCLES(0xf21d, 6);
                branchTarget = true; push16(0xf21f); block_id = 858;
      break;
    case 1154:  // $F220
      /*$F220*/ CYCLES(0xf220, 7);
                tmp1_U8 = s_x;
                s_status_not_z = (tmp1_U8 != 0x30);
                tmp2_U8 = tmp1_U8 >= 0x30;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - 0x30) & 0x80);
      /*$F222*/ branchTarget = true; block_id = tmp2_U8 ? 1148 : 1155;
      break;
    case 1155:  // $F224
      /*$F224*/ CYCLES(0xf224, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xf224, pop16() + 1);;
      break;
    case 1156:  // $F225
      /*$F225*/ CYCLES(0xf225, 6);
                branchTarget = true; push16(0xf227); block_id = 1142;
      break;
    case 1157:  // $F228
      /*$F228*/ CYCLES(0xf228, 12);
                s_a = s_x;
      /*$F229*/ tmp2_U8 = ram_peek(0x00f0);
                s_y = tmp2_U8;
      /*$F22B*/ tmp2_U8 = tmp2_U8 >= 0x28;
                s_status_c = tmp2_U8;
      /*$F22D*/ branchTarget = true; block_id = tmp2_U8 ? 1148 : 1158;
      break;
    case 1158:  // $F22F
      /*$F22F*/ CYCLES(0xf22f, 6);
                branchTarget = true; block_id = 1347;
      break;
    case 1159:  // $F232
      /*$F232*/ CYCLES(0xf232, 6);
                branchTarget = true; push16(0xf234); block_id = 1149;
      break;
    case 1160:  // $F235
      /*$F235*/ CYCLES(0xf235, 12);
                s_a = s_x;
      /*$F236*/ tmp2_U8 = ram_peek(0x002c);
                s_y = tmp2_U8;
      /*$F238*/ tmp2_U8 = tmp2_U8 >= 0x28;
                s_status_c = tmp2_U8;
      /*$F23A*/ branchTarget = true; block_id = tmp2_U8 ? 1148 : 1161;
      break;
    case 1161:  // $F23C
      /*$F23C*/ CYCLES(0xf23c, 9);
                s_y = ram_peek(0x00f0);
      /*$F819*/ CYCLES(0xf819, 6);
                branchTarget = true; push16(0xf81b); block_id = 1347;
      break;
    case 1162:  // $F241
      /*$F241*/ CYCLES(0xf241, 6);
                branchTarget = true; push16(0xf243); block_id = 1149;
      break;
    case 1163:  // $F244
      /*$F244*/ CYCLES(0xf244, 11);
                tmp2_U8 = s_x;
      /*$F245*/ s_y = tmp2_U8;
      /*$F246*/ tmp2_U8 = tmp2_U8 >= 0x28;
                s_status_c = tmp2_U8;
      /*$F248*/ branchTarget = true; block_id = tmp2_U8 ? 1148 : 1164;
      break;
    case 1164:  // $F24A
      /*$F24A*/ CYCLES(0xf24a, 9);
                s_a = ram_peek(0x00f0);
      /*$F24C*/ branchTarget = true; block_id = 1356;
      break;
    case 1165:  // $F24F
      /*$F24F*/ CYCLES(0xf24f, 6);
                branchTarget = true; push16(0xf251); block_id = 858;
      break;
    case 1166:  // $F252
      /*$F252*/ CYCLES(0xf252, 7);
      /*$F864*/ CYCLES(0xf864, 23);
                tmp2_U8 = s_x & 0x0f;
      /*$F866*/ ram_poke(0x0030, tmp2_U8);
      /*$F86B*/ tmp4_U16 = tmp2_U8 << 0x04;
                s_status_c = (uint8_t)((tmp4_U16 & 0x01ff) >> 8);
      /*$F86C*/ tmp2_U8 = ((uint8_t)tmp4_U16) | ram_peek(0x0030);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$F86E*/ ram_poke(0x0030, tmp2_U8);
      /*$F870*/ branchTarget = true; block_id = find_block_id_func_t001(0xf870, pop16() + 1);;
      break;
    case 1167:  // $F256
      /*$F256*/ CYCLES(0xf256, 6);
                branchTarget = true; push16(0xf258); block_id = 858;
      break;
    case 1168:  // $F259
      /*$F259*/ CYCLES(0xf259, 11);
                tmp2_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp2_U8;
      /*$F25A*/ s_a = tmp2_U8;
      /*$F25B*/ tmp2_U8 = tmp2_U8 >= 0x18;
                s_status_c = tmp2_U8;
      /*$F25D*/ branchTarget = true; block_id = tmp2_U8 ? 1148 : 1169;
      break;
    case 1169:  // $F25F
      /*$F25F*/ CYCLES(0xf25f, 6);
                branchTarget = true; block_id = 1452;
      break;
    case 1170:  // $F26D
      /*$F26D*/ CYCLES(0xf26d, 6);
                s_status_c = 0x01;
      /*$F26E*/ branchTarget = true; block_id = 1172;
      break;
    case 1171:  // $F26F
      /*$F26F*/ CYCLES(0xf26f, 2);
                s_status_c = 0x00;
                block_id = 1172;
      break;
    case 1172:  // $F270
      /*$F270*/ CYCLES(0xf270, 6);
                tmp2_U8 = ram_peek(0x00f2);
                tmp1_U8 = (uint8_t)((tmp2_U8 | (0x00 + (s_status_c << 8))) >> 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x00f2, tmp1_U8);
                s_status_c = (tmp2_U8 & 0x01);
      /*$F272*/ branchTarget = true; block_id = find_block_id_func_t001(0xf272, pop16() + 1);;
      break;
    case 1173:  // $F273
      /*$F273*/ CYCLES(0xf273, 7);
                s_a = 0xff;
      /*$F275*/ branchTarget = true; block_id = 1175;
      break;
    case 1174:  // $F277
      /*$F277*/ CYCLES(0xf277, 4);
                s_a = 0x3f;
                block_id = 1175;
      break;
    case 1175:  // $F279
      /*$F279*/ CYCLES(0xf279, 4);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_x = 0x00;
                block_id = 1176;
      break;
    case 1176:  // $F27B
      /*$F27B*/ CYCLES(0xf27b, 9);
                ram_poke(0x0032, s_a);
      /*$F27D*/ ram_poke(0x00f3, s_x);
      /*$F27F*/ branchTarget = true; block_id = find_block_id_func_t001(0xf27f, pop16() + 1);;
      break;
    case 1177:  // $F280
      /*$F280*/ CYCLES(0xf280, 11);
                s_a = 0x7f;
      /*$F282*/ s_status_not_z = 0x40;
                s_status_n = 0x00;
                s_x = 0x40;
      /*$F284*/ branchTarget = true; block_id = 1176;
      break;
    case 1178:  // $F286
      /*$F286*/ CYCLES(0xf286, 6);
                branchTarget = true; push16(0xf288); block_id = 486;
      break;
    case 1179:  // $F288
      /*$F288*/ CYCLES(0xf288, 7);
      /*$F28B*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0xf28b, 0xf28b, 0x03); error_handler(0xf28b);
      break;
    case 1180:  // $F289
      /*$F289*/ CYCLES(0xf289, 6);
                branchTarget = true; push16(0xf28b); block_id = 867;
      break;
    case 1181:  // $F28C
      /*$F28C*/ CYCLES(0xf28c, 18);
      /*$F292*/ tmp5_U16 = ram_peek(0x0051);
                tmp3_U16 = ram_peek(0x006e);
                tmp4_U16 = (tmp5_U16 - tmp3_U16) - (ram_peek(0x0050) < ram_peek(0x006d));
                tmp2_U8 = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_c = tmp2_U8;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)(~tmp3_U16));
      /*$F294*/ branchTarget = true; block_id = tmp2_U8 ? 1183 : 1182;
      break;
    case 1182:  // $F296
      /*$F296*/ CYCLES(0xf296, 6);
                branchTarget = true; block_id = 43;
      break;
    case 1183:  // $F299
      /*$F299*/ CYCLES(0xf299, 23);
                tmp2_U8 = ram_peek(0x0050);
      /*$F29B*/ ram_poke(0x0073, tmp2_U8);
      /*$F29D*/ ram_poke(0x006f, tmp2_U8);
      /*$F29F*/ tmp2_U8 = ram_peek(0x0051);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$F2A1*/ ram_poke(0x0074, tmp2_U8);
      /*$F2A3*/ ram_poke(0x0070, tmp2_U8);
      /*$F2A5*/ branchTarget = true; block_id = find_block_id_func_t001(0xf2a5, pop16() + 1);;
      break;
    case 1184:  // $F2A6
      /*$F2A6*/ CYCLES(0xf2a6, 6);
                branchTarget = true; push16(0xf2a8); block_id = 486;
      break;
    case 1185:  // $F2A9
      /*$F2A9*/ CYCLES(0xf2a9, 6);
                branchTarget = true; push16(0xf2ab); block_id = 867;
      break;
    case 1186:  // $F2AC
      /*$F2AC*/ CYCLES(0xf2ac, 18);
      /*$F2B2*/ tmp2_U8 = (uint8_t)(0x01 - ((uint8_t)(((ram_peek(0x0051) - ram_peek(0x0074)) - (ram_peek(0x0050) < ram_peek(0x0073))) >> 8) & 0x01));
                s_status_c = tmp2_U8;
      /*$F2B4*/ branchTarget = true; block_id = tmp2_U8 ? 1182 : 1187;
      break;
    case 1187:  // $F2B6
      /*$F2B6*/ CYCLES(0xf2b6, 18);
      /*$F2BC*/ tmp4_U16 = ram_peek(0x0051);
                tmp5_U16 = ram_peek(0x006a);
                tmp3_U16 = (tmp4_U16 - tmp5_U16) - (ram_peek(0x0050) < ram_peek(0x0069));
                tmp2_U8 = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_c = tmp2_U8;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)(~tmp5_U16));
      /*$F2BE*/ branchTarget = true; block_id = !tmp2_U8 ? 1182 : 1188;
      break;
    case 1188:  // $F2C0
      /*$F2C0*/ CYCLES(0xf2c0, 19);
      /*$F2C2*/ ram_poke(0x0069, ram_peek(0x0050));
      /*$F2C6*/ ram_poke(0x006a, ram_peek(0x0051));
      /*$F2C8*/ branchTarget = true; block_id = 149;
      break;
    case 1189:  // $F2CB
      /*$F2CB*/ CYCLES(0xf2cb, 9);
                s_a = 0xab;
      /*$F2CD*/ branchTarget = true; push16(0xf2cf); block_id = 566;
      break;
    case 1190:  // $F2D0
      /*$F2D0*/ CYCLES(0xf2d0, 38);
      /*$F2D2*/ ram_poke(0x00f4, ram_peek(0x00b8));
      /*$F2D6*/ ram_poke(0x00f5, ram_peek(0x00b9));
      /*$F2D9*/ tmp2_U8 = ram_peek(0x00d8);
                ram_poke(0x00d8, (uint8_t)((tmp2_U8 | 0x0100) >> 0x01));
                s_status_c = (tmp2_U8 & 0x01);
      /*$F2DD*/ ram_poke(0x00f6, ram_peek(0x0075));
      /*$F2E1*/ ram_poke(0x00f7, ram_peek(0x0076));
      /*$F2E3*/ branchTarget = true; push16(0xf2e5); block_id = 290;
      break;
    case 1191:  // $F2E6
      /*$F2E6*/ CYCLES(0xf2e6, 6);
                branchTarget = true; block_id = 286;
      break;
    case 1192:  // $F2E9
      /*$F2E9*/ CYCLES(0xf2e9, 70);
                ram_poke(0x00de, s_x);
      /*$F2ED*/ ram_poke(0x00df, ram_peek(0x00f8));
      /*$F2F1*/ ram_poke(0x00da, ram_peek(0x0075));
      /*$F2F5*/ ram_poke(0x00db, ram_peek(0x0076));
      /*$F2F9*/ ram_poke(0x00dc, ram_peek(0x0079));
      /*$F2FD*/ ram_poke(0x00dd, ram_peek(0x007a));
      /*$F301*/ ram_poke(0x00b8, ram_peek(0x00f4));
      /*$F305*/ ram_poke(0x00b9, ram_peek(0x00f5));
      /*$F309*/ ram_poke(0x0075, ram_peek(0x00f6));
      /*$F30D*/ ram_poke(0x0076, ram_peek(0x00f7));
      /*$F30F*/ FUNC_CHRGOT(0xf311);
                branchTarget = true; block_id = 1193;
      break;
    case 1193:  // $F312
      /*$F312*/ CYCLES(0xf312, 6);
                branchTarget = true; push16(0xf314); block_id = 267;
      break;
    case 1194:  // $F315
      /*$F315*/ CYCLES(0xf315, 6);
                branchTarget = true; block_id = 216;
      break;
    case 1195:  // $F318
      /*$F318*/ CYCLES(0xf318, 38);
      /*$F31A*/ ram_poke(0x0075, ram_peek(0x00da));
      /*$F31E*/ ram_poke(0x0076, ram_peek(0x00db));
      /*$F322*/ ram_poke(0x00b8, ram_peek(0x00dc));
      /*$F326*/ ram_poke(0x00b9, ram_peek(0x00dd));
      /*$F32A*/ s_sp = ram_peek(0x00df);
      /*$F32B*/ branchTarget = true; block_id = 216;
      break;
    case 1196:  // $F390
      /*$F390*/ CYCLES(0xf390, 16);
                tmp2_U8 = io_peek(0xc056);
      /*$F393*/ tmp2_U8 = io_peek(0xc053);
      /*$FB40*/ CYCLES(0xfb40, 16);
                tmp2_U8 = io_peek(0xc050);
      /*$FB43*/ tmp2_U8 = io_peek(0xc053);
      /*$FB46*/ branchTarget = true; push16(0xfb48); block_id = 1359;
      break;
    case 1197:  // $F399
      /*$F399*/ CYCLES(0xf399, 11);
                tmp2_U8 = io_peek(0xc054);
      /*$F39C*/ branchTarget = true; block_id = 1449;
      break;
    case 1198:  // $F3D8
      /*$F3D8*/ CYCLES(0xf3d8, 18);
                tmp2_U8 = io_peek(0xc055);
      /*$F3DB*/ tmp2_U8 = io_peek(0xc052);
                s_status_v = ((tmp2_U8 >> 0x06) & 0x01);
      /*$F3DE*/ s_a = 0x40;
      /*$F3E0*/ branchTarget = true; block_id = 1200;
      break;
    case 1199:  // $F3E2
      /*$F3E2*/ CYCLES(0xf3e2, 14);
                s_a = 0x20;
      /*$F3E4*/ tmp2_U8 = io_peek(0xc054);
      /*$F3E7*/ tmp2_U8 = io_peek(0xc053);
                s_status_v = ((tmp2_U8 >> 0x06) & 0x01);
                block_id = 1200;
      break;
    case 1200:  // $F3EA
      /*$F3EA*/ CYCLES(0xf3ea, 35);
                ram_poke(0x00e6, s_a);
      /*$F3EC*/ tmp2_U8 = io_peek(0xc057);
      /*$F3EF*/ tmp2_U8 = io_peek(0xc050);
      /*$F3F4*/ ram_poke(0x001c, 0x00);
      /*$F3F8*/ ram_poke(0x001b, ram_peek(0x00e6));
      /*$F3FA*/ s_y = 0x00;
      /*$F3FC*/ ram_poke(0x001a, 0x00);
                block_id = 1201;
      break;
    case 1201:  // $F3FE
      /*$F3FE*/ CYCLES(0xf3fe, 12);
                tmp2_U8 = ram_peek(0x001c);
                s_a = tmp2_U8;
      /*$F400*/ poke((ram_peek16al(0x001a) + s_y), tmp2_U8);
      /*$F402*/ FUNC_COLORSHIFT(0xf404);
                branchTarget = true; block_id = 1202;
      break;
    case 1202:  // $F405
      /*$F405*/ CYCLES(0xf405, 6);
                tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$F406*/ branchTarget = true; block_id = tmp2_U8 ? 1201 : 1203;
      break;
    case 1203:  // $F408
      /*$F408*/ CYCLES(0xf408, 14);
                ram_poke(0x001b, (uint8_t)(ram_peek(0x001b) + 0x01));
      /*$F40C*/ tmp2_U8 = ram_peek(0x001b) & 0x1f;
                s_status_not_z = tmp2_U8;
                s_status_n = 0x00;
                s_a = tmp2_U8;
      /*$F40E*/ branchTarget = true; block_id = tmp2_U8 ? 1201 : 1204;
      break;
    case 1204:  // $F410
      /*$F410*/ CYCLES(0xf410, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xf410, pop16() + 1);;
      break;
    case 1205:  // $F411
      /*$F411*/ CYCLES(0xf411, 75);
                tmp2_U8 = s_a;
                ram_poke(0x00e2, tmp2_U8);
      /*$F413*/ tmp1_U8 = s_x;
                ram_poke(0x00e0, tmp1_U8);
      /*$F415*/ tmp6_U8 = s_y;
                ram_poke(0x00e1, tmp6_U8);
      /*$F417*/ push8(tmp2_U8);
      /*$F418*/ tmp2_U8 = tmp2_U8 & 0xc0;
      /*$F41A*/ ram_poke(0x0026, tmp2_U8);
      /*$F420*/ ram_poke(0x0026, ((tmp2_U8 >> 0x02) | ram_peek(0x0026)));
      /*$F422*/ tmp2_U8 = pop8();
      /*$F423*/ ram_poke(0x0027, tmp2_U8);
      /*$F425*/ tmp5_U16 = tmp2_U8;
      /*$F428*/ ram_poke(0x0027, (uint8_t)((ram_peek(0x0027) << 0x01) | (uint8_t)(((tmp5_U16 << 0x03) & 0x01ff) >> 8)));
      /*$F42B*/ ram_poke(0x0027, (uint8_t)((ram_peek(0x0027) << 0x01) | (uint8_t)(((tmp5_U16 << 0x04) & 0x01ff) >> 8)));
      /*$F42E*/ ram_poke(0x0026, (uint8_t)((ram_peek(0x0026) | (0x00 + ((uint8_t)(((tmp5_U16 << 0x05) & 0x01ff) >> 8) << 8))) >> 0x01));
      /*$F436*/ ram_poke(0x0027, ((ram_peek(0x0027) & 0x1f) | ram_peek(0x00e6)));
      /*$F438*/ s_a = tmp1_U8;
      /*$F439*/ s_status_c = (tmp6_U8 >= 0x00);
      /*$F43B*/ branchTarget = true; block_id = !(tmp6_U8 != 0x00) ? 1208 : 1206;
      break;
    case 1206:  // $F43D
      /*$F43D*/ CYCLES(0xf43d, 7);
                s_y = 0x23;
      /*$F43F*/ tmp4_U16 = (s_a + 0x0004) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1207;
      break;
    case 1207:  // $F441
      /*$F441*/ CYCLES(0xf441, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 1208;
      break;
    case 1208:  // $F442
      /*$F442*/ CYCLES(0xf442, 7);
                tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 - 0x0007) - (uint8_t)(0x01 - s_status_c);
                tmp6_U8 = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_c = tmp6_U8;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0xfff8);
                s_a = ((uint8_t)tmp4_U16);
      /*$F444*/ branchTarget = true; block_id = tmp6_U8 ? 1207 : 1209;
      break;
    case 1209:  // $F446
      /*$F446*/ CYCLES(0xf446, 28);
                tmp6_U8 = s_y;
                ram_poke(0x00e5, tmp6_U8);
      /*$F448*/ tmp1_U8 = s_a;
                s_x = tmp1_U8;
      /*$F44C*/ ram_poke(0x0030, ram_peek((0xf4b9 + tmp1_U8)));
      /*$F44F*/ tmp6_U8 = tmp6_U8 & 0x01;
                s_status_c = tmp6_U8;
      /*$F450*/ tmp1_U8 = ram_peek(0x00e4);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$F452*/ ram_poke(0x001c, tmp1_U8);
      /*$F454*/ branchTarget = true; block_id = tmp6_U8 ? 1666 : 1210;
      break;
    case 1210:  // $F456
      /*$F456*/ CYCLES(0xf456, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xf456, pop16() + 1);;
      break;
    case 1211:  // $F457
      /*$F457*/ CYCLES(0xf457, 6);
                FUNC_HPOSN(0xf459);
                branchTarget = true; block_id = 1212;
      break;
    case 1212:  // $F45A
      /*$F45A*/ CYCLES(0xf45a, 19);
      /*$F45C*/ tmp6_U8 = s_y;
                tmp1_U8 = peek((ram_peek16al(0x0026) + tmp6_U8));
      /*$F460*/ tmp2_U8 = peek((ram_peek16al(0x0026) + tmp6_U8));
                tmp1_U8 = ((ram_peek(0x001c) ^ tmp1_U8) & ram_peek(0x0030)) ^ tmp2_U8;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$F462*/ poke((ram_peek16al(0x0026) + tmp6_U8), tmp1_U8);
      /*$F464*/ branchTarget = true; block_id = find_block_id_func_t001(0xf464, pop16() + 1);;
      break;
    case 1213:  // $F465
      /*$F465*/ CYCLES(0xf465, 4);
                branchTarget = true; block_id = !s_status_n ? 1224 : 1214;
      break;
    case 1214:  // $F467
      /*$F467*/ CYCLES(0xf467, 9);
                tmp1_U8 = ram_peek(0x0030);
      /*$F469*/ tmp6_U8 = tmp1_U8 & 0x01;
                s_status_c = tmp6_U8;
                s_a = (tmp1_U8 >> 0x01);
      /*$F46A*/ branchTarget = true; block_id = tmp6_U8 ? 1217 : 1215;
      break;
    case 1215:  // $F46C
      /*$F46C*/ CYCLES(0xf46c, 4);
                tmp6_U8 = s_a ^ 0xc0;
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
                block_id = 1216;
      break;
    case 1216:  // $F46E
      /*$F46E*/ CYCLES(0xf46e, 6);
                ram_poke(0x0030, s_a);
      /*$F470*/ branchTarget = true; block_id = find_block_id_func_t001(0xf470, pop16() + 1);;
      break;
    case 1217:  // $F471
      /*$F471*/ CYCLES(0xf471, 6);
                tmp6_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp6_U8;
      /*$F472*/ branchTarget = true; block_id = !(tmp6_U8 & 0x80) ? 1219 : 1218;
      break;
    case 1218:  // $F474
      /*$F474*/ CYCLES(0xf474, 4);
                s_y = 0x27;
                block_id = 1219;
      break;
    case 1219:  // $F476
      /*$F476*/ CYCLES(0xf476, 4);
                s_a = 0xc0;
                block_id = 1220;
      break;
    case 1220:  // $F478
      /*$F478*/ CYCLES(0xf478, 11);
                ram_poke(0x0030, s_a);
      /*$F47A*/ ram_poke(0x00e5, s_y);
      /*$F47C*/ s_a = ram_peek(0x001c);
                FUNC_COLORSHIFT(0x0000);
                block_id = find_block_id_func_t001(0xf47c, pop16() + 1);;
      break;
    case 1221:  // $F47E
      /*$F47E*/ CYCLES(0xf47e, 9);
                tmp2_U8 = (uint8_t)(s_a << 0x01);
                s_a = tmp2_U8;
      /*$F47F*/ s_status_not_z = (tmp2_U8 != 0xc0);
                s_status_c = (tmp2_U8 >= 0xc0);
                tmp2_U8 = (uint8_t)(tmp2_U8 - 0xc0) & 0x80;
                s_status_n = tmp2_U8;
      /*$F481*/ branchTarget = true; block_id = !tmp2_U8 ? 1223 : 1222;
      break;
    case 1222:  // $F483
      /*$F483*/ CYCLES(0xf483, 11);
      /*$F485*/ tmp6_U8 = ram_peek(0x001c) ^ 0x7f;
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$F487*/ ram_poke(0x001c, tmp6_U8);
                block_id = 1223;
      break;
    case 1223:  // $F489
      /*$F489*/ CYCLES(0xf489, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xf489, pop16() + 1);;
      break;
    case 1224:  // $F48A
      /*$F48A*/ CYCLES(0xf48a, 12);
      /*$F48C*/ tmp4_U16 = ram_peek(0x0030) << 0x01;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
      /*$F48D*/ tmp6_U8 = ((uint8_t)tmp4_U16) ^ 0x80;
                s_status_not_z = tmp6_U8;
                tmp1_U8 = tmp6_U8 & 0x80;
                s_status_n = tmp1_U8;
                s_a = tmp6_U8;
      /*$F48F*/ branchTarget = true; block_id = tmp1_U8 ? 1216 : 1225;
      break;
    case 1225:  // $F491
      /*$F491*/ CYCLES(0xf491, 12);
                s_a = 0x81;
      /*$F493*/ tmp6_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp6_U8;
      /*$F494*/ tmp6_U8 = tmp6_U8 >= 0x28;
                s_status_c = tmp6_U8;
      /*$F496*/ branchTarget = true; block_id = !tmp6_U8 ? 1220 : 1226;
      break;
    case 1226:  // $F498
      /*$F498*/ CYCLES(0xf498, 7);
                s_y = 0x00;
      /*$F49A*/ branchTarget = true; block_id = s_status_c ? 1220 : 1227;
      break;
    case 1227:  // $F49C
      /*$F49C*/ CYCLES(0xf49c, 2);
                s_status_c = 0x00;
                block_id = 1228;
      break;
    case 1228:  // $F49D
      /*$F49D*/ CYCLES(0xf49d, 11);
      /*$F4A1*/ branchTarget = true; block_id = !(ram_peek(0x00d1) & 0x04) ? 1236 : 1229;
      break;
    case 1229:  // $F4A3
      /*$F4A3*/ CYCLES(0xf4a3, 14);
      /*$F4A7*/ tmp6_U8 = peek((ram_peek16al(0x0026) + s_y));
                tmp6_U8 = (0x7f & ram_peek(0x0030)) & tmp6_U8;
                s_a = tmp6_U8;
      /*$F4A9*/ branchTarget = true; block_id = tmp6_U8 ? 1235 : 1230;
      break;
    case 1230:  // $F4AB
      /*$F4AB*/ CYCLES(0xf4ab, 14);
                ram_poke(0x00ea, (uint8_t)(ram_peek(0x00ea) + 0x01));
      /*$F4AF*/ s_a = (0x7f & ram_peek(0x0030));
      /*$F4B1*/ branchTarget = true; block_id = 1235;
      break;
    case 1231:  // $F4B3
      /*$F4B3*/ CYCLES(0xf4b3, 2);
                s_status_c = 0x00;
                block_id = 1232;
      break;
    case 1232:  // $F4B4
      /*$F4B4*/ CYCLES(0xf4b4, 11);
      /*$F4B8*/ branchTarget = true; block_id = !(ram_peek(0x00d1) & 0x04) ? 1236 : 1233;
      break;
    case 1233:  // $F4BA
      /*$F4BA*/ CYCLES(0xf4ba, 14);
                tmp6_U8 = peek((ram_peek16al(0x0026) + s_y));
      /*$F4BE*/ tmp6_U8 = (tmp6_U8 ^ ram_peek(0x001c)) & ram_peek(0x0030);
                s_a = tmp6_U8;
      /*$F4C0*/ branchTarget = true; block_id = tmp6_U8 ? 1235 : 1234;
      break;
    case 1234:  // $F4C2
      /*$F4C2*/ CYCLES(0xf4c2, 4);
                ram_poke(0x00ea, (uint8_t)(ram_peek(0x00ea) + 0x01));
                block_id = 1235;
      break;
    case 1235:  // $F4C4
      /*$F4C4*/ CYCLES(0xf4c4, 7);
                tmp6_U8 = s_y;
                tmp1_U8 = peek((ram_peek16al(0x0026) + tmp6_U8));
      /*$F4C6*/ poke((ram_peek16al(0x0026) + tmp6_U8), (s_a ^ tmp1_U8));
                block_id = 1236;
      break;
    case 1236:  // $F4C8
      /*$F4C8*/ CYCLES(0xf4c8, 19);
      /*$F4CA*/ tmp4_U16 = ram_peek(0x00d1);
                tmp5_U16 = ram_peek(0x00d3);
                tmp3_U16 = (tmp4_U16 + tmp5_U16) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp5_U16);
                tmp6_U8 = (uint8_t)tmp3_U16;
      /*$F4CC*/ tmp1_U8 = tmp6_U8 & 0x03;
      /*$F4D0*/ s_status_n = ((uint8_t)((tmp1_U8 | (0x00 + ((tmp1_U8 >= 0x02) << 8))) >> 0x01) & 0x80);
                tmp6_U8 = tmp6_U8 & 0x01;
                s_status_c = tmp6_U8;
      /*$F4D1*/ branchTarget = true; block_id = tmp6_U8 ? 1213 : 1237;
      break;
    case 1237:  // $F4D3
      /*$F4D3*/ CYCLES(0xf4d3, 4);
                branchTarget = true; block_id = s_status_n ? 1249 : 1238;
      break;
    case 1238:  // $F4D5
      /*$F4D5*/ CYCLES(0xf4d5, 14);
                s_status_c = 0x00;
      /*$F4D6*/ tmp1_U8 = ram_peek(0x0027);
                s_a = tmp1_U8;
      /*$F4DB*/ branchTarget = true; block_id = (tmp1_U8 & ram_peek(0xf5b9)) ? 1247 : 1239;
      break;
    case 1239:  // $F4DD
      /*$F4DD*/ CYCLES(0xf4dd, 7);
                tmp5_U16 = ram_peek(0x0026) << 0x01;
                tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
                s_status_c = tmp1_U8;
                ram_poke(0x0026, ((uint8_t)tmp5_U16));
      /*$F4DF*/ branchTarget = true; block_id = tmp1_U8 ? 1245 : 1240;
      break;
    case 1240:  // $F4E1
      /*$F4E1*/ CYCLES(0xf4e1, 9);
      /*$F4E4*/ branchTarget = true; block_id = !(s_a & ram_peek(0xf4cd)) ? 1242 : 1241;
      break;
    case 1241:  // $F4E6
      /*$F4E6*/ CYCLES(0xf4e6, 9);
                s_a = (uint8_t)((s_a + 0x001f) + s_status_c);
      /*$F4E8*/ s_status_c = 0x01;
      /*$F4E9*/ branchTarget = true; block_id = 1246;
      break;
    case 1242:  // $F4EB
      /*$F4EB*/ CYCLES(0xf4eb, 16);
                tmp5_U16 = (s_a + 0x0023) + s_status_c;
      /*$F4ED*/ push8(((uint8_t)tmp5_U16));
      /*$F4F0*/ tmp5_U16 = (ram_peek(0x0026) + 0x00b0) + (uint8_t)(tmp5_U16 >> 8);
                tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
                s_status_c = tmp1_U8;
                s_a = ((uint8_t)tmp5_U16);
      /*$F4F2*/ branchTarget = true; block_id = tmp1_U8 ? 1244 : 1243;
      break;
    case 1243:  // $F4F4
      /*$F4F4*/ CYCLES(0xf4f4, 4);
                tmp5_U16 = (s_a + 0x00f0) + s_status_c;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 1244;
      break;
    case 1244:  // $F4F6
      /*$F4F6*/ CYCLES(0xf4f6, 9);
                ram_poke(0x0026, s_a);
      /*$F4F8*/ tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$F4F9*/ branchTarget = true; block_id = s_status_c ? 1246 : 1245;
      break;
    case 1245:  // $F4FB
      /*$F4FB*/ CYCLES(0xf4fb, 4);
                tmp5_U16 = (s_a + 0x001f) + s_status_c;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 1246;
      break;
    case 1246:  // $F4FD
      /*$F4FD*/ CYCLES(0xf4fd, 4);
                tmp1_U8 = ram_peek(0x0026);
                ram_poke(0x0026, (uint8_t)((tmp1_U8 | (0x00 + (s_status_c << 8))) >> 0x01));
                s_status_c = (tmp1_U8 & 0x01);
                block_id = 1247;
      break;
    case 1247:  // $F4FF
      /*$F4FF*/ CYCLES(0xf4ff, 4);
                tmp3_U16 = s_a;
                tmp5_U16 = (tmp3_U16 + 0x00fc) + s_status_c;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)0x00fc);
                tmp1_U8 = (uint8_t)tmp5_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 1248;
      break;
    case 1248:  // $F501
      /*$F501*/ CYCLES(0xf501, 6);
                ram_poke(0x0027, s_a);
      /*$F503*/ branchTarget = true; block_id = find_block_id_func_t001(0xf503, pop16() + 1);;
      break;
    case 1249:  // $F505
      /*$F505*/ CYCLES(0xf505, 16);
      /*$F507*/ tmp3_U16 = (ram_peek(0x0027) + 0x0004) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                tmp6_U8 = (uint8_t)tmp3_U16;
                s_a = tmp6_U8;
      /*$F509*/ tmp1_U8 = ram_peek(0xf5b9);
                s_status_n = (tmp1_U8 & 0x80);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
                tmp1_U8 = tmp6_U8 & tmp1_U8;
                s_status_not_z = tmp1_U8;
      /*$F50C*/ branchTarget = true; block_id = tmp1_U8 ? 1248 : 1250;
      break;
    case 1250:  // $F50E
      /*$F50E*/ CYCLES(0xf50e, 7);
                tmp5_U16 = ram_peek(0x0026) << 0x01;
                tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
                s_status_c = tmp1_U8;
                ram_poke(0x0026, ((uint8_t)tmp5_U16));
      /*$F510*/ branchTarget = true; block_id = !tmp1_U8 ? 1255 : 1251;
      break;
    case 1251:  // $F512
      /*$F512*/ CYCLES(0xf512, 14);
                tmp1_U8 = (uint8_t)((s_a + 0x00e0) + s_status_c);
                s_a = tmp1_U8;
      /*$F514*/ s_status_c = 0x00;
      /*$F515*/ tmp6_U8 = ram_peek(0xf508);
                s_status_v = ((tmp6_U8 >> 0x06) & 0x01);
      /*$F518*/ branchTarget = true; block_id = !(tmp1_U8 & tmp6_U8) ? 1256 : 1252;
      break;
    case 1252:  // $F51A
      /*$F51A*/ CYCLES(0xf51a, 14);
      /*$F51C*/ tmp4_U16 = ram_peek(0x0026);
                tmp5_U16 = (tmp4_U16 + 0x0050) + s_status_c;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x0050);
      /*$F51E*/ tmp6_U8 = ((uint8_t)tmp5_U16) ^ 0xf0;
                s_a = tmp6_U8;
      /*$F520*/ branchTarget = true; block_id = !tmp6_U8 ? 1254 : 1253;
      break;
    case 1253:  // $F522
      /*$F522*/ CYCLES(0xf522, 4);
                s_a = (s_a ^ 0xf0);
                block_id = 1254;
      break;
    case 1254:  // $F524
      /*$F524*/ CYCLES(0xf524, 11);
                ram_poke(0x0026, s_a);
      /*$F526*/ s_a = ram_peek(0x00e6);
      /*$F528*/ branchTarget = true; block_id = !s_status_c ? 1256 : 1255;
      break;
    case 1255:  // $F52A
      /*$F52A*/ CYCLES(0xf52a, 4);
                tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 + 0x00e0) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x00e0);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1256;
      break;
    case 1256:  // $F52C
      /*$F52C*/ CYCLES(0xf52c, 7);
                tmp6_U8 = ram_peek(0x0026);
                tmp1_U8 = (uint8_t)((tmp6_U8 | (0x00 + (s_status_c << 8))) >> 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x0026, tmp1_U8);
                tmp6_U8 = tmp6_U8 & 0x01;
                s_status_c = tmp6_U8;
      /*$F52E*/ branchTarget = true; block_id = !tmp6_U8 ? 1248 : 1257;
      break;
    case 1257:  // $F530
      /*$F530*/ CYCLES(0xf530, 18);
                push8(s_a);
      /*$F533*/ ram_poke(0x00e0, 0x00);
      /*$F535*/ ram_poke(0x00e1, 0x00);
      /*$F537*/ ram_poke(0x00e2, 0x00);
      /*$F539*/ tmp6_U8 = pop8();
                s_a = tmp6_U8;
                block_id = 1258;
      break;
    case 1258:  // $F53A
      /*$F53A*/ CYCLES(0xf53a, 21);
                tmp6_U8 = s_a;
                push8(tmp6_U8);
      /*$F53C*/ tmp5_U16 = tmp6_U8 - ram_peek(0x00e0);
      /*$F53E*/ push8(((uint8_t)tmp5_U16));
      /*$F540*/ tmp5_U16 = (s_x - ram_peek(0x00e1)) - ((uint8_t)(tmp5_U16 >> 8) & 0x01);
                tmp6_U8 = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
                s_status_c = tmp6_U8;
                tmp1_U8 = (uint8_t)tmp5_U16;
                s_a = tmp1_U8;
      /*$F542*/ ram_poke(0x00d3, tmp1_U8);
      /*$F544*/ branchTarget = true; block_id = tmp6_U8 ? 1260 : 1259;
      break;
    case 1259:  // $F546
      /*$F546*/ CYCLES(0xf546, 18);
                tmp6_U8 = pop8();
      /*$F549*/ tmp5_U16 = ((tmp6_U8 ^ 0xff) + 0x0001) + s_status_c;
      /*$F54B*/ push8(((uint8_t)tmp5_U16));
      /*$F54E*/ s_a = (uint8_t)((0x0000 - ram_peek(0x00d3)) - (uint8_t)(0x01 - (uint8_t)(tmp5_U16 >> 8)));
                block_id = 1260;
      break;
    case 1260:  // $F550
      /*$F550*/ CYCLES(0xf550, 35);
                tmp6_U8 = s_a;
                ram_poke(0x00d1, tmp6_U8);
      /*$F552*/ ram_poke(0x00d5, tmp6_U8);
      /*$F554*/ tmp6_U8 = pop8();
      /*$F555*/ ram_poke(0x00d0, tmp6_U8);
      /*$F557*/ ram_poke(0x00d4, tmp6_U8);
      /*$F559*/ tmp6_U8 = pop8();
      /*$F55A*/ ram_poke(0x00e0, tmp6_U8);
      /*$F55C*/ ram_poke(0x00e1, s_x);
      /*$F560*/ tmp5_U16 = (s_y - ram_peek(0x00e2)) - 0x0001;
                tmp6_U8 = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
                s_status_c = tmp6_U8;
                s_a = ((uint8_t)tmp5_U16);
      /*$F562*/ block_id = !tmp6_U8 ? 1263 : 1262;
      break;
    case 1261:  // $F558
      /*$F558*/ CYCLES(0xf558, 2);
                fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0xf558, 0xf558, 0x03); error_handler(0xf558);
      break;
    case 1262:  // $F564
      /*$F564*/ CYCLES(0xf564, 7);
      /*$F566*/ tmp5_U16 = ((s_a ^ 0xff) + 0x00fe) + s_status_c;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 1263;
      break;
    case 1263:  // $F568
      /*$F568*/ CYCLES(0xf568, 35);
                tmp6_U8 = s_a;
                ram_poke(0x00d2, tmp6_U8);
      /*$F56A*/ ram_poke(0x00e2, s_y);
      /*$F56C*/ ram_poke(0x00d3, (uint8_t)((ram_peek(0x00d3) | (0x00 + (s_status_c << 8))) >> 0x01));
      /*$F56F*/ tmp5_U16 = tmp6_U8 - ram_peek(0x00d0);
      /*$F571*/ s_x = ((uint8_t)tmp5_U16);
      /*$F574*/ tmp4_U16 = ram_peek(0x00d1);
                tmp5_U16 = (0x00ff - tmp4_U16) - ((uint8_t)(tmp5_U16 >> 8) & 0x01);
                tmp6_U8 = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
                s_status_c = tmp6_U8;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)0x00ff, (uint8_t)(~tmp4_U16));
                tmp1_U8 = (uint8_t)tmp5_U16;
                s_a = tmp1_U8;
      /*$F576*/ ram_poke(0x001d, tmp1_U8);
      /*$F578*/ s_y = ram_peek(0x00e5);
      /*$F57A*/ branchTarget = true; block_id = tmp6_U8 ? 1266 : 1264;
      break;
    case 1264:  // $F57C
      /*$F57C*/ CYCLES(0xf57c, 7);
                s_status_n = ((uint8_t)(s_a << 0x01) & 0x80);
      /*$F57D*/ branchTarget = true; push16(0xf57f); block_id = 1213;
      break;
    case 1265:  // $F580
      /*$F580*/ CYCLES(0xf580, 2);
                s_status_c = 0x01;
                block_id = 1266;
      break;
    case 1266:  // $F581
      /*$F581*/ CYCLES(0xf581, 18);
      /*$F583*/ tmp5_U16 = (ram_peek(0x00d4) + ram_peek(0x00d2)) + s_status_c;
      /*$F585*/ ram_poke(0x00d4, ((uint8_t)tmp5_U16));
      /*$F589*/ tmp4_U16 = ram_peek(0x00d5);
                tmp5_U16 = tmp4_U16 - (uint8_t)(0x01 - (uint8_t)(tmp5_U16 >> 8));
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0xffff);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 1267;
      break;
    case 1267:  // $F58B
      /*$F58B*/ CYCLES(0xf58b, 26);
                ram_poke(0x00d5, s_a);
      /*$F58D*/ tmp6_U8 = s_y;
                tmp1_U8 = peek((ram_peek16al(0x0026) + tmp6_U8));
      /*$F593*/ tmp2_U8 = peek((ram_peek16al(0x0026) + tmp6_U8));
                tmp1_U8 = ((tmp1_U8 ^ ram_peek(0x001c)) & ram_peek(0x0030)) ^ tmp2_U8;
                s_a = tmp1_U8;
      /*$F595*/ poke((ram_peek16al(0x0026) + tmp6_U8), tmp1_U8);
      /*$F597*/ tmp6_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp6_U8;
      /*$F598*/ branchTarget = true; block_id = tmp6_U8 ? 1269 : 1268;
      break;
    case 1268:  // $F59A
      /*$F59A*/ CYCLES(0xf59a, 7);
                tmp6_U8 = (uint8_t)(ram_peek(0x001d) + 0x01);
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                ram_poke(0x001d, tmp6_U8);
      /*$F59C*/ branchTarget = true; block_id = !tmp6_U8 ? 1274 : 1269;
      break;
    case 1269:  // $F59E
      /*$F59E*/ CYCLES(0xf59e, 7);
                tmp6_U8 = ram_peek(0x00d3);
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$F5A0*/ branchTarget = true; block_id = s_status_c ? 1264 : 1270;
      break;
    case 1270:  // $F5A2
      /*$F5A2*/ CYCLES(0xf5a2, 6);
                branchTarget = true; push16(0xf5a4); block_id = 1237;
      break;
    case 1271:  // $F5A5
      /*$F5A5*/ CYCLES(0xf5a5, 23);
      /*$F5A8*/ tmp5_U16 = ram_peek(0x00d4) + ram_peek(0x00d0);
      /*$F5AA*/ ram_poke(0x00d4, ((uint8_t)tmp5_U16));
      /*$F5AE*/ tmp3_U16 = ram_peek(0x00d5);
                tmp4_U16 = ram_peek(0x00d1);
                tmp5_U16 = (tmp3_U16 + tmp4_U16) + (uint8_t)(tmp5_U16 >> 8);
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                tmp1_U8 = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
                s_status_v = tmp1_U8;
                s_a = ((uint8_t)tmp5_U16);
      /*$F5B0*/ branchTarget = true; block_id = !tmp1_U8 ? 1267 : 1272;
      break;
    case 1272:  // $F5B2
      /*$F5B2*/ CYCLES(0xf5b2, 11);
                tmp6_U8 = s_x;
                poke((ram_peek((uint8_t)(0x82 + tmp6_U8)) + (ram_peek((uint8_t)(0x83 + tmp6_U8)) << 8)), s_a);
      /*$F5B4*/ ram_poke(0x0088, s_y);
      /*$F5B6*/ branchTarget = true; block_id = !s_status_c ? 1261 : 1273;
      break;
    case 1273:  // $F5B8
      /*$F5B8*/ CYCLES(0xf5b8, 6);
      /*$F5BA*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0xf5ba, 0xf5ba, 0x03); error_handler(0xf5ba);
      break;
    case 1274:  // $F600
      /*$F600*/ CYCLES(0xf600, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xf600, pop16() + 1);;
      break;
    case 1275:  // $F626
      /*$F626*/ CYCLES(0xf626, 18);
                ram_poke(0x00d1, s_a);
      /*$F62A*/ ram_poke(0x00d4, 0x80);
      /*$F62C*/ ram_poke(0x00d5, 0x80);
      /*$F62E*/ s_x = ram_peek(0x00e7);
                block_id = 1276;
      break;
    case 1276:  // $F630
      /*$F630*/ CYCLES(0xf630, 16);
      /*$F633*/ tmp5_U16 = (ram_peek(0x00d4) + ram_peek(0x00d0)) + 0x0001;
                tmp6_U8 = (uint8_t)(tmp5_U16 >> 8);
                s_status_c = tmp6_U8;
      /*$F635*/ ram_poke(0x00d4, ((uint8_t)tmp5_U16));
      /*$F637*/ branchTarget = true; block_id = !tmp6_U8 ? 1279 : 1277;
      break;
    case 1277:  // $F639
      /*$F639*/ CYCLES(0xf639, 6);
                branchTarget = true; push16(0xf63b); block_id = 1231;
      break;
    case 1278:  // $F63C
      /*$F63C*/ CYCLES(0xf63c, 2);
                s_status_c = 0x00;
                block_id = 1279;
      break;
    case 1279:  // $F63D
      /*$F63D*/ CYCLES(0xf63d, 14);
      /*$F63F*/ tmp5_U16 = ram_peek(0x00d5);
                tmp4_U16 = ram_peek(0x00d2);
                tmp3_U16 = (tmp5_U16 + tmp4_U16) + s_status_c;
                tmp6_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = tmp6_U8;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp5_U16, (uint8_t)tmp4_U16);
      /*$F641*/ ram_poke(0x00d5, ((uint8_t)tmp3_U16));
      /*$F643*/ branchTarget = true; block_id = !tmp6_U8 ? 1281 : 1280;
      break;
    case 1280:  // $F645
      /*$F645*/ CYCLES(0xf645, 6);
                branchTarget = true; push16(0xf647); block_id = 1232;
      break;
    case 1281:  // $F648
      /*$F648*/ CYCLES(0xf648, 6);
                tmp6_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp6_U8;
      /*$F649*/ branchTarget = true; block_id = tmp6_U8 ? 1276 : 1282;
      break;
    case 1282:  // $F64B
      /*$F64B*/ CYCLES(0xf64b, 12);
                tmp6_U8 = ram_peek(0x00d1);
      /*$F64F*/ s_status_c = ((tmp6_U8 >> 0x02) & 0x01);
                tmp6_U8 = tmp6_U8 >> 0x03;
                s_a = tmp6_U8;
      /*$F650*/ branchTarget = true; block_id = tmp6_U8 ? 1275 : 1283;
      break;
    case 1283:  // $F652
      /*$F652*/ CYCLES(0xf652, 7);
                tmp6_U8 = (uint8_t)(ram_peek(0x001a) + 0x01);
                ram_poke(0x001a, tmp6_U8);
      /*$F654*/ branchTarget = true; block_id = tmp6_U8 ? 1285 : 1284;
      break;
    case 1284:  // $F656
      /*$F656*/ CYCLES(0xf656, 4);
                ram_poke(0x001b, (uint8_t)(ram_peek(0x001b) + 0x01));
                block_id = 1285;
      break;
    case 1285:  // $F658
      /*$F658*/ CYCLES(0xf658, 7);
                tmp6_U8 = s_x;
                tmp6_U8 = peek((ram_peek((uint8_t)(0x1a + tmp6_U8)) + (ram_peek((uint8_t)(0x1b + tmp6_U8)) << 8)));
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$F65A*/ branchTarget = true; block_id = tmp6_U8 ? 1275 : 1286;
      break;
    case 1286:  // $F65C
      /*$F65C*/ CYCLES(0xf65c, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xf65c, pop16() + 1);;
      break;
    case 1287:  // $F682
      /*$F682*/ CYCLES(0xf682, 18);
                ram_poke(0x00d1, s_a);
      /*$F686*/ ram_poke(0x00d4, 0x80);
      /*$F688*/ ram_poke(0x00d5, 0x80);
      /*$F68A*/ s_x = ram_peek(0x00e7);
                block_id = 1288;
      break;
    case 1288:  // $F68C
      /*$F68C*/ CYCLES(0xf68c, 16);
      /*$F68F*/ tmp3_U16 = (ram_peek(0x00d4) + ram_peek(0x00d0)) + 0x0001;
                tmp6_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = tmp6_U8;
      /*$F691*/ ram_poke(0x00d4, ((uint8_t)tmp3_U16));
      /*$F693*/ branchTarget = true; block_id = !tmp6_U8 ? 1291 : 1289;
      break;
    case 1289:  // $F695
      /*$F695*/ CYCLES(0xf695, 6);
                branchTarget = true; push16(0xf697); block_id = 1227;
      break;
    case 1290:  // $F698
      /*$F698*/ CYCLES(0xf698, 2);
                s_status_c = 0x00;
                block_id = 1291;
      break;
    case 1291:  // $F699
      /*$F699*/ CYCLES(0xf699, 14);
      /*$F69B*/ tmp3_U16 = ram_peek(0x00d5);
                tmp4_U16 = ram_peek(0x00d2);
                tmp5_U16 = (tmp3_U16 + tmp4_U16) + s_status_c;
                tmp6_U8 = (uint8_t)(tmp5_U16 >> 8);
                s_status_c = tmp6_U8;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
      /*$F69D*/ ram_poke(0x00d5, ((uint8_t)tmp5_U16));
      /*$F69F*/ branchTarget = true; block_id = !tmp6_U8 ? 1293 : 1292;
      break;
    case 1292:  // $F6A1
      /*$F6A1*/ CYCLES(0xf6a1, 6);
                branchTarget = true; push16(0xf6a3); block_id = 1228;
      break;
    case 1293:  // $F6A4
      /*$F6A4*/ CYCLES(0xf6a4, 6);
                tmp6_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp6_U8;
      /*$F6A5*/ branchTarget = true; block_id = tmp6_U8 ? 1288 : 1294;
      break;
    case 1294:  // $F6A7
      /*$F6A7*/ CYCLES(0xf6a7, 12);
                tmp6_U8 = ram_peek(0x00d1);
      /*$F6AB*/ s_status_c = ((tmp6_U8 >> 0x02) & 0x01);
                tmp6_U8 = tmp6_U8 >> 0x03;
                s_a = tmp6_U8;
      /*$F6AC*/ branchTarget = true; block_id = tmp6_U8 ? 1287 : 1295;
      break;
    case 1295:  // $F6AE
      /*$F6AE*/ CYCLES(0xf6ae, 7);
                tmp6_U8 = (uint8_t)(ram_peek(0x001a) + 0x01);
                ram_poke(0x001a, tmp6_U8);
      /*$F6B0*/ branchTarget = true; block_id = tmp6_U8 ? 1297 : 1296;
      break;
    case 1296:  // $F6B2
      /*$F6B2*/ CYCLES(0xf6b2, 4);
                ram_poke(0x001b, (uint8_t)(ram_peek(0x001b) + 0x01));
                block_id = 1297;
      break;
    case 1297:  // $F6B4
      /*$F6B4*/ CYCLES(0xf6b4, 7);
                tmp6_U8 = s_x;
                tmp6_U8 = peek((ram_peek((uint8_t)(0x1a + tmp6_U8)) + (ram_peek((uint8_t)(0x1b + tmp6_U8)) << 8)));
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$F6B6*/ branchTarget = true; block_id = tmp6_U8 ? 1287 : 1298;
      break;
    case 1298:  // $F6B8
      /*$F6B8*/ CYCLES(0xf6b8, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xf6b8, pop16() + 1);;
      break;
    case 1299:  // $F6B9
      /*$F6B9*/ CYCLES(0xf6b9, 6);
                branchTarget = true; push16(0xf6bb); block_id = 486;
      break;
    case 1300:  // $F6BC
      /*$F6BC*/ CYCLES(0xf6bc, 6);
                branchTarget = true; push16(0xf6be); block_id = 867;
      break;
    case 1301:  // $F6BF
      /*$F6BF*/ CYCLES(0xf6bf, 14);
                tmp2_U8 = ram_peek(0x0051);
                s_y = tmp2_U8;
      /*$F6C1*/ s_x = ram_peek(0x0050);
      /*$F6C3*/ s_status_not_z = (tmp2_U8 != 0x01);
                tmp2_U8 = tmp2_U8 >= 0x01;
                s_status_c = tmp2_U8;
      /*$F6C5*/ branchTarget = true; block_id = !tmp2_U8 ? 1304 : 1302;
      break;
    case 1302:  // $F6C7
      /*$F6C7*/ CYCLES(0xf6c7, 4);
                branchTarget = true; block_id = s_status_not_z ? 1308 : 1303;
      break;
    case 1303:  // $F6C9
      /*$F6C9*/ CYCLES(0xf6c9, 7);
                tmp6_U8 = s_x >= 0x18;
                s_status_c = tmp6_U8;
      /*$F6CB*/ branchTarget = true; block_id = tmp6_U8 ? 1308 : 1304;
      break;
    case 1304:  // $F6CD
      /*$F6CD*/ CYCLES(0xf6cd, 16);
      /*$F6CE*/ push8(s_x);
      /*$F6D0*/ push8(s_y);
      /*$F6D1*/ s_a = 0x2c;
      /*$F6D3*/ branchTarget = true; push16(0xf6d5); block_id = 566;
      break;
    case 1305:  // $F6D6
      /*$F6D6*/ CYCLES(0xf6d6, 6);
                branchTarget = true; push16(0xf6d8); block_id = 858;
      break;
    case 1306:  // $F6D9
      /*$F6D9*/ CYCLES(0xf6d9, 7);
                tmp2_U8 = s_x >= 0xc0;
                s_status_c = tmp2_U8;
      /*$F6DB*/ branchTarget = true; block_id = tmp2_U8 ? 1308 : 1307;
      break;
    case 1307:  // $F6DD
      /*$F6DD*/ CYCLES(0xf6dd, 16);
                ram_poke(0x009d, s_x);
      /*$F6DF*/ tmp6_U8 = pop8();
      /*$F6E0*/ s_y = tmp6_U8;
      /*$F6E1*/ tmp6_U8 = pop8();
      /*$F6E2*/ s_x = tmp6_U8;
      /*$F6E3*/ tmp6_U8 = ram_peek(0x009d);
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$F6E5*/ branchTarget = true; block_id = find_block_id_func_t001(0xf6e5, pop16() + 1);;
      break;
    case 1308:  // $F6E6
      /*$F6E6*/ CYCLES(0xf6e6, 6);
                branchTarget = true; block_id = 1148;
      break;
    case 1309:  // $F6E9
      /*$F6E9*/ CYCLES(0xf6e9, 6);
                branchTarget = true; push16(0xf6eb); block_id = 858;
      break;
    case 1310:  // $F6EC
      /*$F6EC*/ CYCLES(0xf6ec, 7);
                tmp2_U8 = s_x >= 0x08;
                s_status_c = tmp2_U8;
      /*$F6EE*/ branchTarget = true; block_id = tmp2_U8 ? 1308 : 1311;
      break;
    case 1311:  // $F6F0
      /*$F6F0*/ CYCLES(0xf6f0, 9);
                tmp6_U8 = ram_peek((0xf6f6 + s_x));
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$F6F3*/ ram_poke(0x00e4, tmp6_U8);
                block_id = 1312;
      break;
    case 1312:  // $F6F5
      /*$F6F5*/ CYCLES(0xf6f5, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xf6f5, pop16() + 1);;
      break;
    case 1313:  // $F6FE
      /*$F6FE*/ CYCLES(0xf6fe, 7);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0xc1);
      /*$F700*/ branchTarget = true; block_id = !(tmp2_U8 != 0xc1) ? 1318 : 1314;
      break;
    case 1314:  // $F702
      /*$F702*/ CYCLES(0xf702, 6);
                branchTarget = true; push16(0xf704); block_id = 1299;
      break;
    case 1315:  // $F705
      /*$F705*/ CYCLES(0xf705, 6);
                FUNC_HPLOT0(0xf707);
                branchTarget = true; block_id = 1316;
      break;
    case 1316:  // $F708
      /*$F708*/ CYCLES(0xf708, 6);
                FUNC_CHRGOT(0xf70a);
                branchTarget = true; block_id = 1317;
      break;
    case 1317:  // $F70B
      /*$F70B*/ CYCLES(0xf70b, 7);
                tmp2_U8 = s_a;
                tmp1_U8 = tmp2_U8 != 0xc1;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp2_U8 >= 0xc1);
                s_status_n = ((uint8_t)(tmp2_U8 - 0xc1) & 0x80);
      /*$F70D*/ branchTarget = true; block_id = tmp1_U8 ? 1312 : 1318;
      break;
    case 1318:  // $F70F
      /*$F70F*/ CYCLES(0xf70f, 6);
                branchTarget = true; push16(0xf711); block_id = 566;
      break;
    case 1319:  // $F712
      /*$F712*/ CYCLES(0xf712, 6);
                branchTarget = true; push16(0xf714); block_id = 1299;
      break;
    case 1320:  // $F715
      /*$F715*/ CYCLES(0xf715, 16);
                ram_poke(0x009d, s_y);
      /*$F717*/ s_y = s_a;
      /*$F718*/ s_a = s_x;
      /*$F719*/ s_x = ram_peek(0x009d);
      /*$F71B*/ branchTarget = true; push16(0xf71d); block_id = 1258;
      break;
    case 1321:  // $F71E
      /*$F71E*/ CYCLES(0xf71e, 6);
                branchTarget = true; block_id = 1316;
      break;
    case 1322:  // $F721
      /*$F721*/ CYCLES(0xf721, 6);
                branchTarget = true; push16(0xf723); block_id = 858;
      break;
    case 1323:  // $F724
      /*$F724*/ CYCLES(0xf724, 6);
                ram_poke(0x00f9, s_x);
      /*$F726*/ branchTarget = true; block_id = find_block_id_func_t001(0xf726, pop16() + 1);;
      break;
    case 1324:  // $F727
      /*$F727*/ CYCLES(0xf727, 6);
                branchTarget = true; push16(0xf729); block_id = 858;
      break;
    case 1325:  // $F72A
      /*$F72A*/ CYCLES(0xf72a, 6);
                ram_poke(0x00e7, s_x);
      /*$F72C*/ branchTarget = true; block_id = find_block_id_func_t001(0xf72c, pop16() + 1);;
      break;
    case 1326:  // $F72D
      /*$F72D*/ CYCLES(0xf72d, 6);
                branchTarget = true; push16(0xf72f); block_id = 858;
      break;
    case 1327:  // $F730
      /*$F730*/ CYCLES(0xf730, 26);
      /*$F732*/ ram_poke(0x001a, ram_peek(0x00e8));
      /*$F736*/ ram_poke(0x001b, ram_peek(0x00e9));
      /*$F738*/ tmp1_U8 = s_x;
                s_a = tmp1_U8;
      /*$F73B*/ tmp2_U8 = peek((ram_peek(0x001a) + (ram_peek(0x001b) << 8)));
                s_status_c = (tmp1_U8 >= tmp2_U8);
      /*$F73D*/ branchTarget = true; block_id = !(tmp1_U8 != tmp2_U8) ? 1329 : 1328;
      break;
    case 1328:  // $F73F
      /*$F73F*/ CYCLES(0xf73f, 4);
                branchTarget = true; block_id = s_status_c ? 1308 : 1329;
      break;
    case 1329:  // $F741
      /*$F741*/ CYCLES(0xf741, 6);
                tmp5_U16 = s_a << 0x01;
                tmp6_U8 = (uint8_t)(tmp5_U16 >> 8);
                s_status_c = tmp6_U8;
                s_a = ((uint8_t)tmp5_U16);
      /*$F742*/ branchTarget = true; block_id = !tmp6_U8 ? 1331 : 1330;
      break;
    case 1330:  // $F744
      /*$F744*/ CYCLES(0xf744, 6);
                ram_poke(0x001b, (uint8_t)(ram_peek(0x001b) + 0x01));
      /*$F746*/ s_status_c = 0x00;
                block_id = 1331;
      break;
    case 1331:  // $F747
      /*$F747*/ CYCLES(0xf747, 31);
                tmp6_U8 = s_a;
      /*$F748*/ tmp1_U8 = peek((ram_peek16al(0x001a) + tmp6_U8));
      /*$F74A*/ tmp5_U16 = (tmp1_U8 + ram_peek(0x001a)) + s_status_c;
                tmp1_U8 = (uint8_t)tmp5_U16;
      /*$F74C*/ s_x = tmp1_U8;
      /*$F74D*/ tmp6_U8 = (uint8_t)(tmp6_U8 + 0x01);
                s_y = tmp6_U8;
      /*$F74E*/ tmp6_U8 = peek((ram_peek16al(0x001a) + tmp6_U8));
      /*$F750*/ tmp4_U16 = tmp6_U8;
                tmp3_U16 = ram_peek(0x00e9);
                tmp5_U16 = (tmp4_U16 + tmp3_U16) + (uint8_t)(tmp5_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)tmp3_U16);
      /*$F752*/ ram_poke(0x001b, ((uint8_t)tmp5_U16));
      /*$F754*/ ram_poke(0x001a, tmp1_U8);
      /*$F756*/ FUNC_CHRGOT(0xf758);
                branchTarget = true; block_id = 1332;
      break;
    case 1332:  // $F759
      /*$F759*/ CYCLES(0xf759, 7);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0xc5);
      /*$F75B*/ branchTarget = true; block_id = (tmp2_U8 != 0xc5) ? 1336 : 1333;
      break;
    case 1333:  // $F75D
      /*$F75D*/ CYCLES(0xf75d, 6);
                branchTarget = true; push16(0xf75f); block_id = 566;
      break;
    case 1334:  // $F760
      /*$F760*/ CYCLES(0xf760, 6);
                branchTarget = true; push16(0xf762); block_id = 1299;
      break;
    case 1335:  // $F763
      /*$F763*/ CYCLES(0xf763, 6);
                FUNC_HPOSN(0xf765);
                branchTarget = true; block_id = 1336;
      break;
    case 1336:  // $F766
      /*$F766*/ CYCLES(0xf766, 6);
                tmp6_U8 = ram_peek(0x00f9);
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$F768*/ branchTarget = true; block_id = find_block_id_func_t001(0xf768, pop16() + 1);;
      break;
    case 1337:  // $F769
      /*$F769*/ CYCLES(0xf769, 6);
                branchTarget = true; push16(0xf76b); block_id = 1326;
      break;
    case 1338:  // $F76C
      /*$F76C*/ CYCLES(0xf76c, 6);
      /*$F605*/ CYCLES(0xf605, 57);
                tmp1_U8 = s_a;
      /*$F60A*/ ram_poke(0x00d3, (tmp1_U8 >> 0x04));
      /*$F60D*/ tmp1_U8 = tmp1_U8 & 0x0f;
      /*$F613*/ ram_poke(0x00d0, ram_peek((0xf5ba + tmp1_U8)));
      /*$F61C*/ ram_poke(0x00d2, (uint8_t)(ram_peek((0xf5bb + (tmp1_U8 ^ 0x0f))) + 0x01));
      /*$F61E*/ s_y = ram_peek(0x00e5);
      /*$F622*/ ram_poke(0x00ea, 0x00);
      /*$F624*/ tmp1_U8 = peek((ram_peek(0x001a) + (ram_peek(0x001b) << 8)));
                s_a = tmp1_U8;
                block_id = 1275;
      break;
    case 1339:  // $F76F
      /*$F76F*/ CYCLES(0xf76f, 6);
                branchTarget = true; push16(0xf771); block_id = 1326;
      break;
    case 1340:  // $F772
      /*$F772*/ CYCLES(0xf772, 6);
      /*$F661*/ CYCLES(0xf661, 57);
                tmp1_U8 = s_a;
      /*$F666*/ ram_poke(0x00d3, (tmp1_U8 >> 0x04));
      /*$F669*/ tmp1_U8 = tmp1_U8 & 0x0f;
      /*$F66F*/ ram_poke(0x00d0, ram_peek((0xf5ba + tmp1_U8)));
      /*$F678*/ ram_poke(0x00d2, (uint8_t)(ram_peek((0xf5bb + (tmp1_U8 ^ 0x0f))) + 0x01));
      /*$F67A*/ s_y = ram_peek(0x00e5);
      /*$F67E*/ ram_poke(0x00ea, 0x00);
      /*$F680*/ tmp1_U8 = peek((ram_peek(0x001a) + (ram_peek(0x001b) << 8)));
                s_a = tmp1_U8;
                block_id = 1287;
      break;
    case 1341:  // $F7E7
      /*$F7E7*/ CYCLES(0xf7e7, 6);
                branchTarget = true; push16(0xf7e9); block_id = 858;
      break;
    case 1342:  // $F7EA
      /*$F7EA*/ CYCLES(0xf7ea, 4);
                tmp2_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp2_U8;
      /*$F7EB*/ s_a = tmp2_U8;
                block_id = 1343;
      break;
    case 1343:  // $F7EC
      /*$F7EC*/ CYCLES(0xf7ec, 7);
                tmp1_U8 = s_a;
                s_status_not_z = (tmp1_U8 != 0x28);
                tmp6_U8 = tmp1_U8 >= 0x28;
                s_status_c = tmp6_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - 0x28) & 0x80);
      /*$F7EE*/ branchTarget = true; block_id = !tmp6_U8 ? 1346 : 1344;
      break;
    case 1344:  // $F7F0
      /*$F7F0*/ CYCLES(0xf7f0, 11);
                tmp5_U16 = s_a;
                tmp3_U16 = (tmp5_U16 - 0x0028) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp5_U16, (uint8_t)0xffd7);
      /*$F7F2*/ push8(((uint8_t)tmp3_U16));
      /*$F7F3*/ branchTarget = true; push16(0xf7f5); block_id = 361;
      break;
    case 1345:  // $F7F6
      /*$F7F6*/ CYCLES(0xf7f6, 7);
                tmp6_U8 = pop8();
                s_a = tmp6_U8;
      /*$F7F7*/ branchTarget = true; block_id = 1343;
      break;
    case 1346:  // $F7FA
      /*$F7FA*/ CYCLES(0xf7fa, 6);
                ram_poke(0x0024, s_a);
      /*$F7FC*/ branchTarget = true; block_id = find_block_id_func_t001(0xf7fc, pop16() + 1);;
      break;
    case 1347:  // $F800
      /*$F800*/ CYCLES(0xf800, 9);
                tmp2_U8 = s_a;
                tmp1_U8 = tmp2_U8 >> 0x01;
                s_a = tmp1_U8;
      /*$F801*/ push8(((tmp2_U8 & 0x01) | ((tmp1_U8 == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | (tmp1_U8 & 0x80)));
      /*$F802*/ FUNC_GBASCALC(0xf804);
                branchTarget = true; block_id = 1348;
      break;
    case 1348:  // $F805
      /*$F805*/ CYCLES(0xf805, 9);
                tmp6_U8 = pop8();
                tmp1_U8 = tmp6_U8 & 0x01;
                s_status_c = tmp1_U8;
                s_status_i = ((tmp6_U8 & 0x04) != 0);
                s_status_d = ((tmp6_U8 & 0x08) != 0);
                s_status_b = 0x00;
                s_status_v = ((tmp6_U8 & 0x40) != 0);
      /*$F806*/ s_a = 0x0f;
      /*$F808*/ branchTarget = true; block_id = !tmp1_U8 ? 1350 : 1349;
      break;
    case 1349:  // $F80A
      /*$F80A*/ CYCLES(0xf80a, 4);
                tmp3_U16 = s_a;
                tmp5_U16 = (tmp3_U16 + 0x00e0) + s_status_c;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)0x00e0);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 1350;
      break;
    case 1350:  // $F80C
      /*$F80C*/ CYCLES(0xf80c, 4);
                ram_poke(0x002e, s_a);
                FUNC_PLOT1(0x0000);
                block_id = find_block_id_func_t001(0xf80c, pop16() + 1);;
      break;
    case 1351:  // $F80E
      /*$F80E*/ CYCLES(0xf80e, 19);
                tmp6_U8 = s_y;
                tmp1_U8 = peek((ram_peek16al(0x0026) + tmp6_U8));
      /*$F814*/ tmp2_U8 = peek((ram_peek16al(0x0026) + tmp6_U8));
                tmp1_U8 = ((tmp1_U8 ^ ram_peek(0x0030)) & ram_peek(0x002e)) ^ tmp2_U8;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$F816*/ poke((ram_peek16al(0x0026) + tmp6_U8), tmp1_U8);
      /*$F818*/ branchTarget = true; block_id = find_block_id_func_t001(0xf818, pop16() + 1);;
      break;
    case 1352:  // $F81C
      /*$F81C*/ CYCLES(0xf81c, 7);
                tmp6_U8 = s_y;
                tmp1_U8 = ram_peek(0x002c);
                s_status_not_z = (tmp6_U8 != tmp1_U8);
                tmp2_U8 = tmp6_U8 >= tmp1_U8;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp6_U8 - tmp1_U8) & 0x80);
      /*$F81E*/ branchTarget = true; block_id = tmp2_U8 ? 1358 : 1353;
      break;
    case 1353:  // $F820
      /*$F820*/ CYCLES(0xf820, 7);
                s_y = (uint8_t)(s_y + 0x01);
      /*$F821*/ FUNC_PLOT1(0xf823);
                branchTarget = true; block_id = 1354;
      break;
    case 1354:  // $F824
      /*$F824*/ CYCLES(0xf824, 4);
                branchTarget = true; block_id = !s_status_c ? 1352 : 1355;
      break;
    case 1355:  // $F826
      /*$F826*/ CYCLES(0xf826, 4);
                tmp5_U16 = s_a;
                tmp3_U16 = (tmp5_U16 + 0x0001) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp5_U16, (uint8_t)0x0001);
                s_a = ((uint8_t)tmp3_U16);
                block_id = 1356;
      break;
    case 1356:  // $F828
      /*$F828*/ CYCLES(0xf828, 7);
                push8(s_a);
      /*$F829*/ branchTarget = true; push16(0xf82b); block_id = 1347;
      break;
    case 1357:  // $F82C
      /*$F82C*/ CYCLES(0xf82c, 9);
                tmp2_U8 = pop8();
                s_a = tmp2_U8;
      /*$F82D*/ tmp1_U8 = ram_peek(0x002d);
                s_status_not_z = (tmp2_U8 != tmp1_U8);
                tmp6_U8 = tmp2_U8 >= tmp1_U8;
                s_status_c = tmp6_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
      /*$F82F*/ branchTarget = true; block_id = !tmp6_U8 ? 1355 : 1358;
      break;
    case 1358:  // $F831
      /*$F831*/ CYCLES(0xf831, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xf831, pop16() + 1);;
      break;
    case 1359:  // $F836
      /*$F836*/ CYCLES(0xf836, 11);
      /*$F838*/ ram_poke(0x002d, 0x27);
      /*$F83A*/ s_y = 0x27;
                block_id = 1360;
      break;
    case 1360:  // $F83C
      /*$F83C*/ CYCLES(0xf83c, 12);
                s_a = 0x00;
      /*$F83E*/ ram_poke(0x0030, 0x00);
      /*$F840*/ branchTarget = true; push16(0xf842); block_id = 1356;
      break;
    case 1361:  // $F843
      /*$F843*/ CYCLES(0xf843, 6);
                tmp6_U8 = (uint8_t)(s_y - 0x01);
                s_status_not_z = tmp6_U8;
                tmp1_U8 = tmp6_U8 & 0x80;
                s_status_n = tmp1_U8;
                s_y = tmp6_U8;
      /*$F844*/ branchTarget = true; block_id = !tmp1_U8 ? 1360 : 1362;
      break;
    case 1362:  // $F846
      /*$F846*/ CYCLES(0xf846, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xf846, pop16() + 1);;
      break;
    case 1363:  // $F847
      /*$F847*/ CYCLES(0xf847, 23);
                tmp1_U8 = s_a;
                push8(tmp1_U8);
      /*$F848*/ tmp6_U8 = tmp1_U8 & 0x01;
                s_status_c = tmp6_U8;
      /*$F84D*/ ram_poke(0x0027, (((tmp1_U8 >> 0x01) & 0x03) | 0x04));
      /*$F84F*/ tmp1_U8 = pop8();
      /*$F850*/ s_a = (tmp1_U8 & 0x18);
      /*$F852*/ branchTarget = true; block_id = !tmp6_U8 ? 1365 : 1364;
      break;
    case 1364:  // $F854
      /*$F854*/ CYCLES(0xf854, 4);
                tmp3_U16 = s_a;
                tmp5_U16 = (tmp3_U16 + 0x007f) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)0x007f);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 1365;
      break;
    case 1365:  // $F856
      /*$F856*/ CYCLES(0xf856, 16);
                tmp1_U8 = s_a;
                ram_poke(0x0026, tmp1_U8);
      /*$F859*/ tmp3_U16 = tmp1_U8 << 0x02;
                s_status_c = (uint8_t)((tmp3_U16 & 0x01ff) >> 8);
      /*$F85A*/ tmp1_U8 = ((uint8_t)tmp3_U16) | ram_peek(0x0026);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$F85C*/ ram_poke(0x0026, tmp1_U8);
      /*$F85E*/ branchTarget = true; block_id = find_block_id_func_t001(0xf85e, pop16() + 1);;
      break;
    case 1366:  // $F871
      /*$F871*/ CYCLES(0xf871, 9);
                tmp1_U8 = s_a;
                tmp6_U8 = tmp1_U8 >> 0x01;
                s_a = tmp6_U8;
      /*$F872*/ push8(((tmp1_U8 & 0x01) | ((tmp6_U8 == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | (tmp6_U8 & 0x80)));
      /*$F873*/ FUNC_GBASCALC(0xf875);
                branchTarget = true; block_id = 1367;
      break;
    case 1367:  // $F876
      /*$F876*/ CYCLES(0xf876, 6);
                tmp1_U8 = peek((ram_peek16al(0x0026) + s_y));
                s_a = tmp1_U8;
      /*$F878*/ tmp1_U8 = pop8();
                s_status_c = (tmp1_U8 & 0x01);
                s_status_i = ((tmp1_U8 & 0x04) != 0);
                s_status_d = ((tmp1_U8 & 0x08) != 0);
                s_status_b = 0x00;
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                FUNC_SCRN2(0x0000);
                block_id = find_block_id_func_t001(0xf878, pop16() + 1);;
      break;
    case 1368:  // $F879
      /*$F879*/ CYCLES(0xf879, 4);
                branchTarget = true; block_id = !s_status_c ? 1370 : 1369;
      break;
    case 1369:  // $F87B
      /*$F87B*/ CYCLES(0xf87b, 7);
                tmp1_U8 = s_a;
      /*$F87E*/ s_status_c = ((tmp1_U8 >> 0x03) & 0x01);
                s_a = (tmp1_U8 >> 0x04);
                block_id = 1370;
      break;
    case 1370:  // $F87F
      /*$F87F*/ CYCLES(0xf87f, 6);
                tmp1_U8 = s_a & 0x0f;
                s_status_not_z = tmp1_U8;
                s_status_n = 0x00;
                s_a = tmp1_U8;
      /*$F881*/ branchTarget = true; block_id = find_block_id_func_t001(0xf881, pop16() + 1);;
      break;
    case 1371:  // $F882
      /*$F882*/ CYCLES(0xf882, 12);
                s_x = ram_peek(0x003a);
      /*$F884*/ s_y = ram_peek(0x003b);
      /*$F886*/ branchTarget = true; push16(0xf888); block_id = 1575;
      break;
    case 1372:  // $F889
      /*$F889*/ CYCLES(0xf889, 6);
                branchTarget = true; push16(0xf88b); block_id = 1415;
      break;
    case 1373:  // $F88C
      /*$F88C*/ CYCLES(0xf88c, 11);
                tmp1_U8 = s_x;
                tmp1_U8 = peek((ram_peek((uint8_t)(0x3a + tmp1_U8)) + (ram_peek((uint8_t)(0x3b + tmp1_U8)) << 8)));
      /*$F88E*/ s_y = tmp1_U8;
      /*$F88F*/ tmp6_U8 = tmp1_U8 & 0x01;
                s_status_c = tmp6_U8;
                s_a = (tmp1_U8 >> 0x01);
      /*$F890*/ branchTarget = true; block_id = !tmp6_U8 ? 1377 : 1374;
      break;
    case 1374:  // $F892
      /*$F892*/ CYCLES(0xf892, 6);
                tmp6_U8 = s_a;
                s_a = (uint8_t)((tmp6_U8 | (0x00 + (s_status_c << 8))) >> 0x01);
      /*$F893*/ branchTarget = true; block_id = (tmp6_U8 & 0x01) ? 1379 : 1375;
      break;
    case 1375:  // $F895
      /*$F895*/ CYCLES(0xf895, 7);
      /*$F897*/ branchTarget = true; block_id = !(s_a != 0xa2) ? 1379 : 1376;
      break;
    case 1376:  // $F899
      /*$F899*/ CYCLES(0xf899, 4);
                s_a = (s_a & 0x87);
                block_id = 1377;
      break;
    case 1377:  // $F89B
      /*$F89B*/ CYCLES(0xf89b, 14);
                tmp6_U8 = s_a;
                s_status_c = (tmp6_U8 & 0x01);
      /*$F89D*/ s_a = ram_peek((0xf962 + (tmp6_U8 >> 0x01)));
      /*$F8A0*/ FUNC_SCRN2(0xf8a2);
                branchTarget = true; block_id = 1378;
      break;
    case 1378:  // $F8A3
      /*$F8A3*/ CYCLES(0xf8a3, 4);
                branchTarget = true; block_id = s_status_not_z ? 1380 : 1379;
      break;
    case 1379:  // $F8A5
      /*$F8A5*/ CYCLES(0xf8a5, 7);
                s_y = 0x80;
      /*$F8A7*/ s_a = 0x00;
                block_id = 1380;
      break;
    case 1380:  // $F8A9
      /*$F8A9*/ CYCLES(0xf8a9, 36);
      /*$F8AA*/ tmp6_U8 = ram_peek((0xf9a6 + s_a));
      /*$F8AD*/ ram_poke(0x002e, tmp6_U8);
      /*$F8B1*/ ram_poke(0x002f, (tmp6_U8 & 0x03));
      /*$F8B3*/ tmp6_U8 = s_y;
      /*$F8B4*/ tmp1_U8 = tmp6_U8 & 0x8f;
      /*$F8B6*/ s_x = tmp1_U8;
      /*$F8B7*/ s_a = tmp6_U8;
      /*$F8B8*/ s_y = 0x03;
      /*$F8BA*/ s_status_c = (tmp1_U8 >= 0x8a);
      /*$F8BC*/ branchTarget = true; block_id = !(tmp1_U8 != 0x8a) ? 1385 : 1381;
      break;
    case 1381:  // $F8BE
      /*$F8BE*/ CYCLES(0xf8be, 6);
                tmp1_U8 = s_a;
                tmp6_U8 = tmp1_U8 & 0x01;
                s_status_c = tmp6_U8;
                s_a = (tmp1_U8 >> 0x01);
      /*$F8BF*/ branchTarget = true; block_id = !tmp6_U8 ? 1385 : 1382;
      break;
    case 1382:  // $F8C1
      /*$F8C1*/ CYCLES(0xf8c1, 2);
                s_a = (s_a >> 0x01);
                block_id = 1383;
      break;
    case 1383:  // $F8C2
      /*$F8C2*/ CYCLES(0xf8c2, 11);
                tmp6_U8 = s_a;
                s_status_c = (tmp6_U8 & 0x01);
      /*$F8C3*/ s_a = ((tmp6_U8 >> 0x01) | 0x20);
      /*$F8C5*/ tmp6_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp6_U8;
      /*$F8C6*/ branchTarget = true; block_id = tmp6_U8 ? 1383 : 1384;
      break;
    case 1384:  // $F8C8
      /*$F8C8*/ CYCLES(0xf8c8, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 1385;
      break;
    case 1385:  // $F8C9
      /*$F8C9*/ CYCLES(0xf8c9, 6);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
      /*$F8CA*/ branchTarget = true; block_id = tmp1_U8 ? 1381 : 1386;
      break;
    case 1386:  // $F8CC
      /*$F8CC*/ CYCLES(0xf8cc, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xf8cc, pop16() + 1);;
      break;
    case 1387:  // $F8D0
      /*$F8D0*/ CYCLES(0xf8d0, 6);
                branchTarget = true; push16(0xf8d2); block_id = 1371;
      break;
    case 1388:  // $F8D3
      /*$F8D3*/ CYCLES(0xf8d3, 2);
                push8(s_a);
                block_id = 1389;
      break;
    case 1389:  // $F8D4
      /*$F8D4*/ CYCLES(0xf8d4, 9);
                tmp6_U8 = peek((ram_peek16al(0x003a) + s_y));
                s_a = tmp6_U8;
      /*$F8D6*/ branchTarget = true; push16(0xf8d8); block_id = 1590;
      break;
    case 1390:  // $F8D9
      /*$F8D9*/ CYCLES(0xf8d9, 4);
                s_x = 0x01;
                block_id = 1391;
      break;
    case 1391:  // $F8DB
      /*$F8DB*/ CYCLES(0xf8db, 6);
                branchTarget = true; push16(0xf8dd); block_id = 1416;
      break;
    case 1392:  // $F8DE
      /*$F8DE*/ CYCLES(0xf8de, 9);
                tmp6_U8 = s_y;
                tmp1_U8 = tmp6_U8 >= ram_peek(0x002f);
                s_status_c = tmp1_U8;
      /*$F8E0*/ s_y = (uint8_t)(tmp6_U8 + 0x01);
      /*$F8E1*/ branchTarget = true; block_id = !tmp1_U8 ? 1389 : 1393;
      break;
    case 1393:  // $F8E3
      /*$F8E3*/ CYCLES(0xf8e3, 11);
                s_x = 0x03;
      /*$F8E5*/ tmp1_U8 = s_y >= 0x04;
                s_status_c = tmp1_U8;
      /*$F8E7*/ branchTarget = true; block_id = !tmp1_U8 ? 1391 : 1394;
      break;
    case 1394:  // $F8E9
      /*$F8E9*/ CYCLES(0xf8e9, 21);
                tmp1_U8 = pop8();
      /*$F8EE*/ ram_poke(0x002c, ram_peek((0xf9c0 + tmp1_U8)));
      /*$F8F3*/ ram_poke(0x002d, ram_peek((0xfa00 + tmp1_U8)));
                block_id = 1395;
      break;
    case 1395:  // $F8F5
      /*$F8F5*/ CYCLES(0xf8f5, 7);
                s_a = 0x00;
      /*$F8F7*/ s_y = 0x05;
                block_id = 1396;
      break;
    case 1396:  // $F8F9
      /*$F8F9*/ CYCLES(0xf8f9, 14);
                tmp5_U16 = ram_peek(0x002d) << 0x01;
                ram_poke(0x002d, ((uint8_t)tmp5_U16));
      /*$F8FB*/ tmp5_U16 = (ram_peek(0x002c) << 0x01) | (uint8_t)(tmp5_U16 >> 8);
                ram_poke(0x002c, ((uint8_t)tmp5_U16));
      /*$F8FD*/ tmp5_U16 = (s_a << 0x01) | (uint8_t)(tmp5_U16 >> 8);
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                s_a = ((uint8_t)tmp5_U16);
      /*$F8FE*/ tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp1_U8;
      /*$F8FF*/ branchTarget = true; block_id = tmp1_U8 ? 1396 : 1397;
      break;
    case 1397:  // $F901
      /*$F901*/ CYCLES(0xf901, 9);
                tmp5_U16 = s_a;
                tmp3_U16 = (tmp5_U16 + 0x00bf) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp5_U16, (uint8_t)0x00bf);
                tmp1_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$F903*/ branchTarget = true; push16(0xf905); block_id = 1594;
      break;
    case 1398:  // $F906
      /*$F906*/ CYCLES(0xf906, 6);
                tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp1_U8;
      /*$F907*/ branchTarget = true; block_id = tmp1_U8 ? 1395 : 1399;
      break;
    case 1399:  // $F909
      /*$F909*/ CYCLES(0xf909, 6);
                branchTarget = true; push16(0xf90b); block_id = 1415;
      break;
    case 1400:  // $F90C
      /*$F90C*/ CYCLES(0xf90c, 7);
                s_y = ram_peek(0x002f);
      /*$F90E*/ s_x = 0x06;
                block_id = 1401;
      break;
    case 1401:  // $F910
      /*$F910*/ CYCLES(0xf910, 7);
      /*$F912*/ branchTarget = true; block_id = !(s_x != 0x03) ? 1410 : 1402;
      break;
    case 1402:  // $F914
      /*$F914*/ CYCLES(0xf914, 7);
                tmp3_U16 = ram_peek(0x002e) << 0x01;
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = tmp1_U8;
                ram_poke(0x002e, ((uint8_t)tmp3_U16));
      /*$F916*/ branchTarget = true; block_id = !tmp1_U8 ? 1406 : 1403;
      break;
    case 1403:  // $F918
      /*$F918*/ CYCLES(0xf918, 11);
                tmp1_U8 = ram_peek((0xf9b3 + s_x));
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$F91B*/ branchTarget = true; push16(0xf91d); block_id = 1594;
      break;
    case 1404:  // $F91E
      /*$F91E*/ CYCLES(0xf91e, 9);
                tmp1_U8 = ram_peek((0xf9b9 + s_x));
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$F921*/ branchTarget = true; block_id = !tmp1_U8 ? 1406 : 1405;
      break;
    case 1405:  // $F923
      /*$F923*/ CYCLES(0xf923, 6);
                branchTarget = true; push16(0xf925); block_id = 1594;
      break;
    case 1406:  // $F926
      /*$F926*/ CYCLES(0xf926, 6);
                tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$F927*/ branchTarget = true; block_id = tmp1_U8 ? 1401 : 1407;
      break;
    case 1407:  // $F929
      /*$F929*/ CYCLES(0xf929, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xf929, pop16() + 1);;
      break;
    case 1408:  // $F92A
      /*$F92A*/ CYCLES(0xf92a, 6);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp1_U8;
      /*$F92B*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 1402 : 1409;
      break;
    case 1409:  // $F92D
      /*$F92D*/ CYCLES(0xf92d, 6);
                branchTarget = true; push16(0xf92f); block_id = 1590;
      break;
    case 1410:  // $F930
      /*$F930*/ CYCLES(0xf930, 14);
      /*$F932*/ tmp1_U8 = ram_peek(0x002e) >= 0xe8;
                s_status_c = tmp1_U8;
      /*$F934*/ tmp6_U8 = peek((ram_peek16al(0x003a) + s_y));
                s_a = tmp6_U8;
      /*$F936*/ branchTarget = true; block_id = !tmp1_U8 ? 1408 : 1411;
      break;
    case 1411:  // $F938
      /*$F938*/ CYCLES(0xf938, 6);
                FUNC_PCADJ3(0xf93a);
      /*$F93B*/ CYCLES(0xf93b, 7);
      /*$F93C*/ tmp1_U8 = (uint8_t)(s_a + 0x01);
                s_x = tmp1_U8;
      /*$F93D*/ branchTarget = true; block_id = tmp1_U8 ? 1413 : 1412;
      break;
    case 1412:  // $F93F
      /*$F93F*/ CYCLES(0xf93f, 2);
                s_y = (uint8_t)(s_y + 0x01);
                block_id = 1413;
      break;
    case 1413:  // $F940
      /*$F940*/ CYCLES(0xf940, 7);
                s_a = s_y;
      /*$F941*/ branchTarget = true; push16(0xf943); block_id = 1590;
      break;
    case 1414:  // $F944
      /*$F944*/ CYCLES(0xf944, 7);
                s_a = s_x;
      /*$F945*/ branchTarget = true; block_id = 1590;
      break;
    case 1415:  // $F948
      /*$F948*/ CYCLES(0xf948, 4);
                s_x = 0x03;
                block_id = 1416;
      break;
    case 1416:  // $F94A
      /*$F94A*/ CYCLES(0xf94a, 9);
                s_status_not_z = 0xa0;
                s_status_n = 0x80;
                s_a = 0xa0;
      /*$F94C*/ branchTarget = true; push16(0xf94e); block_id = 1594;
      break;
    case 1417:  // $F94F
      /*$F94F*/ CYCLES(0xf94f, 6);
                tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$F950*/ branchTarget = true; block_id = tmp1_U8 ? 1416 : 1418;
      break;
    case 1418:  // $F952
      /*$F952*/ CYCLES(0xf952, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xf952, pop16() + 1);;
      break;
    case 1419:  // $F953
      /*$F953*/ CYCLES(0xf953, 6);
                s_status_c = 0x01;
      /*$F954*/ s_a = ram_peek(0x002f);
                FUNC_PCADJ3(0x0000);
                block_id = find_block_id_func_t001(0xf954, pop16() + 1);;
      break;
    case 1420:  // $F956
      /*$F956*/ CYCLES(0xf956, 9);
                s_y = ram_peek(0x003b);
      /*$F958*/ tmp1_U8 = s_a;
                s_x = tmp1_U8;
      /*$F959*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 1422 : 1421;
      break;
    case 1421:  // $F95B
      /*$F95B*/ CYCLES(0xf95b, 2);
                s_y = (uint8_t)(s_y - 0x01);
                block_id = 1422;
      break;
    case 1422:  // $F95C
      /*$F95C*/ CYCLES(0xf95c, 7);
                tmp3_U16 = s_a;
                tmp5_U16 = ram_peek(0x003a);
                tmp4_U16 = (tmp3_U16 + tmp5_U16) + s_status_c;
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = tmp1_U8;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)tmp5_U16);
                tmp6_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$F95E*/ branchTarget = true; block_id = !tmp1_U8 ? 1424 : 1423;
      break;
    case 1423:  // $F960
      /*$F960*/ CYCLES(0xf960, 2);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
                block_id = 1424;
      break;
    case 1424:  // $F961
      /*$F961*/ CYCLES(0xf961, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xf961, pop16() + 1);;
      break;
    case 1425:  // $FA66
      /*$FA66*/ CYCLES(0xfa66, 6);
                FUNC_INIT(0xfa68);
                branchTarget = true; block_id = 1426;
      break;
    case 1426:  // $FA69
      /*$FA69*/ CYCLES(0xfa69, 6);
                FUNC_SETVID(0xfa6b);
                branchTarget = true; block_id = 1427;
      break;
    case 1427:  // $FA6C
      /*$FA6C*/ CYCLES(0xfa6c, 6);
                FUNC_SETKBD(0xfa6e);
                branchTarget = true; block_id = 1428;
      break;
    case 1428:  // $FA6F
      /*$FA6F*/ CYCLES(0xfa6f, 38);
                tmp1_U8 = io_peek(0xc058);
      /*$FA72*/ tmp1_U8 = io_peek(0xc05a);
      /*$FA75*/ tmp1_U8 = io_peek(0xc05d);
      /*$FA78*/ tmp1_U8 = io_peek(0xc05f);
      /*$FA7B*/ tmp1_U8 = io_peek(0xcfff);
      /*$FA7E*/ tmp1_U8 = io_peek(0xc010);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
      /*$FA81*/ s_status_d = 0x00;
      /*$FA82*/ branchTarget = true; push16(0xfa84); block_id = 1637;
      break;
    case 1429:  // $FA85
      /*$FA85*/ CYCLES(0xfa85, 18);
      /*$FA88*/ tmp1_U8 = ram_peek(0x03f3) ^ 0xa5;
      /*$FA8A*/ tmp6_U8 = ram_peek(0x03f4);
                s_status_c = (tmp1_U8 >= tmp6_U8);
      /*$FA8D*/ branchTarget = true; block_id = (tmp1_U8 != tmp6_U8) ? 1434 : 1430;
      break;
    case 1430:  // $FA8F
      /*$FA8F*/ CYCLES(0xfa8f, 9);
                tmp6_U8 = ram_peek(0x03f2);
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$FA92*/ branchTarget = true; block_id = tmp6_U8 ? 1433 : 1431;
      break;
    case 1431:  // $FA94
      /*$FA94*/ CYCLES(0xfa94, 12);
                s_a = 0xe0;
      /*$FA96*/ tmp6_U8 = ram_peek(0x03f3);
                tmp1_U8 = 0xe0 != tmp6_U8;
                s_status_not_z = tmp1_U8;
                s_status_c = (0xe0 >= tmp6_U8);
                s_status_n = ((uint8_t)(0xe0 - tmp6_U8) & 0x80);
      /*$FA99*/ branchTarget = true; block_id = tmp1_U8 ? 1433 : 1432;
      break;
    case 1432:  // $FA9B
      /*$FA9B*/ CYCLES(0xfa9b, 14);
      /*$FA9D*/ ram_poke(0x03f2, 0x03);
      /*$E000*/ CYCLES(0xe000, 6);
      /*$F128*/ CYCLES(0xf128, 38);
      /*$F12A*/ ram_poke(0x0076, 0xff);
      /*$F12E*/ s_sp = 0xfb;
      /*$F131*/ s_y = 0xf1;
      /*$F133*/ ram_poke(0x0001, 0x28);
      /*$F135*/ ram_poke(0x0002, 0xf1);
      /*$F137*/ ram_poke(0x0004, 0x28);
      /*$F139*/ ram_poke(0x0005, 0xf1);
      /*$F13B*/ branchTarget = true; push16(0xf13d); block_id = 1173;
      break;
    case 1433:  // $FAA3
      /*$FAA3*/ CYCLES(0xfaa3, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xfaa3, ram_peek16al(0x03f2));
      break;
    case 1434:  // $FAA6
      /*$FAA6*/ CYCLES(0xfaa6, 6);
                branchTarget = true; push16(0xfaa8); block_id = 1453;
      break;
    case 1435:  // $FAA9
      /*$FAA9*/ CYCLES(0xfaa9, 4);
                s_x = 0x05;
                block_id = 1436;
      break;
    case 1436:  // $FAAB
      /*$FAAB*/ CYCLES(0xfaab, 16);
                tmp1_U8 = s_x;
      /*$FAAE*/ ram_poke((0x03ef + tmp1_U8), ram_peek((0xfafc + tmp1_U8)));
      /*$FAB1*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_x = tmp1_U8;
      /*$FAB2*/ branchTarget = true; block_id = tmp1_U8 ? 1436 : 1437;
      break;
    case 1437:  // $FAB4
      /*$FAB4*/ CYCLES(0xfab4, 11);
      /*$FAB6*/ ram_poke(0x0000, s_x);
      /*$FAB8*/ ram_poke(0x0001, 0xc8);
                block_id = 1438;
      break;
    case 1438:  // $FABA
      /*$FABA*/ CYCLES(0xfaba, 18);
                s_y = 0x07;
      /*$FABC*/ ram_poke(0x0001, (uint8_t)(ram_peek(0x0001) - 0x01));
      /*$FABE*/ tmp1_U8 = ram_peek(0x0001);
                s_a = tmp1_U8;
      /*$FAC0*/ s_status_c = (tmp1_U8 >= 0xc0);
      /*$FAC2*/ branchTarget = true; block_id = !(tmp1_U8 != 0xc0) ? 1432 : 1439;
      break;
    case 1439:  // $FAC4
      /*$FAC4*/ CYCLES(0xfac4, 6);
                ram_poke(0x07f8, s_a);
                block_id = 1440;
      break;
    case 1440:  // $FAC7
      /*$FAC7*/ CYCLES(0xfac7, 12);
                tmp1_U8 = s_y;
                tmp6_U8 = peek((ram_peek16al(0x0000) + tmp1_U8));
                s_a = tmp6_U8;
      /*$FAC9*/ tmp1_U8 = ram_peek((0xfb01 + tmp1_U8));
                s_status_c = (tmp6_U8 >= tmp1_U8);
      /*$FACC*/ branchTarget = true; block_id = (tmp6_U8 != tmp1_U8) ? 1438 : 1441;
      break;
    case 1441:  // $FACE
      /*$FACE*/ CYCLES(0xface, 7);
      /*$FACF*/ tmp1_U8 = (uint8_t)(s_y - 0x02);
                s_status_not_z = tmp1_U8;
                tmp6_U8 = tmp1_U8 & 0x80;
                s_status_n = tmp6_U8;
                s_y = tmp1_U8;
      /*$FAD0*/ branchTarget = true; block_id = !tmp6_U8 ? 1440 : 1442;
      break;
    case 1442:  // $FAD2
      /*$FAD2*/ CYCLES(0xfad2, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xfad2, ram_peek16al(0x0000));
      break;
    case 1443:  // $FB1E
      /*$FB1E*/ CYCLES(0xfb1e, 12);
                tmp1_U8 = io_peek(0xc070);
      /*$FB21*/ s_y = 0x00;
      /*$FB24*/ block_id = 1444;
      break;
    case 1444:  // $FB25
      /*$FB25*/ CYCLES(0xfb25, 9);
                tmp6_U8 = peek((0xc064 + s_x));
                s_status_not_z = tmp6_U8;
                tmp1_U8 = tmp6_U8 & 0x80;
                s_status_n = tmp1_U8;
                s_a = tmp6_U8;
      /*$FB28*/ branchTarget = true; block_id = !tmp1_U8 ? 1447 : 1445;
      break;
    case 1445:  // $FB2A
      /*$FB2A*/ CYCLES(0xfb2a, 6);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$FB2B*/ branchTarget = true; block_id = tmp1_U8 ? 1444 : 1446;
      break;
    case 1446:  // $FB2D
      /*$FB2D*/ CYCLES(0xfb2d, 2);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
                block_id = 1447;
      break;
    case 1447:  // $FB2E
      /*$FB2E*/ CYCLES(0xfb2e, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfb2e, pop16() + 1);;
      break;
    case 1448:  // $FB2F
      /*$FB2F*/ CYCLES(0xfb2f, 18);
      /*$FB31*/ ram_poke(0x0048, 0x00);
      /*$FB33*/ tmp1_U8 = io_peek(0xc056);
      /*$FB36*/ tmp1_U8 = io_peek(0xc054);
                block_id = 1449;
      break;
    case 1449:  // $FB39
      /*$FB39*/ CYCLES(0xfb39, 12);
                tmp2_U8 = io_peek(0xc051);
      /*$FB3C*/ s_a = 0x00;
      /*$FB3E*/ branchTarget = true; block_id = 1451;
      break;
    case 1450:  // $FB49
      /*$FB49*/ CYCLES(0xfb49, 4);
                s_a = 0x14;
                block_id = 1451;
      break;
    case 1451:  // $FB4B
      /*$FB4B*/ CYCLES(0xfb4b, 28);
                ram_poke(0x0022, s_a);
      /*$FB4F*/ ram_poke(0x0020, 0x00);
      /*$FB53*/ ram_poke(0x0021, 0x28);
      /*$FB57*/ ram_poke(0x0023, 0x18);
      /*$FB59*/ s_a = 0x17;
                block_id = 1452;
      break;
    case 1452:  // $FB5B
      /*$FB5B*/ CYCLES(0xfb5b, 9);
                ram_poke(0x0025, s_a);
      /*$FB5D*/ branchTarget = true; block_id = 1493;
      break;
    case 1453:  // $FB60
      /*$FB60*/ CYCLES(0xfb60, 6);
                branchTarget = true; push16(0xfb62); block_id = 1508;
      break;
    case 1454:  // $FB63
      /*$FB63*/ CYCLES(0xfb63, 4);
                s_y = 0x08;
                block_id = 1455;
      break;
    case 1455:  // $FB65
      /*$FB65*/ CYCLES(0xfb65, 16);
                tmp1_U8 = s_y;
                tmp6_U8 = ram_peek((0xfb08 + tmp1_U8));
                s_a = tmp6_U8;
      /*$FB68*/ ram_poke((0x040e + tmp1_U8), tmp6_U8);
      /*$FB6B*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_y = tmp1_U8;
      /*$FB6C*/ branchTarget = true; block_id = tmp1_U8 ? 1455 : 1456;
      break;
    case 1456:  // $FB6E
      /*$FB6E*/ CYCLES(0xfb6e, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfb6e, pop16() + 1);;
      break;
    case 1457:  // $FB78
      /*$FB78*/ CYCLES(0xfb78, 7);
      /*$FB7A*/ branchTarget = true; block_id = (s_a != 0x8d) ? 1464 : 1458;
      break;
    case 1458:  // $FB7C
      /*$FB7C*/ CYCLES(0xfb7c, 9);
                tmp1_U8 = io_peek(0xc000);
                s_y = tmp1_U8;
      /*$FB7F*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 1464 : 1459;
      break;
    case 1459:  // $FB81
      /*$FB81*/ CYCLES(0xfb81, 7);
      /*$FB83*/ branchTarget = true; block_id = (s_y != 0x93) ? 1464 : 1460;
      break;
    case 1460:  // $FB85
      /*$FB85*/ CYCLES(0xfb85, 6);
                tmp1_U8 = io_peek(0xc010);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
                block_id = 1461;
      break;
    case 1461:  // $FB88
      /*$FB88*/ CYCLES(0xfb88, 9);
                tmp1_U8 = io_peek(0xc000);
                s_y = tmp1_U8;
      /*$FB8B*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 1461 : 1462;
      break;
    case 1462:  // $FB8D
      /*$FB8D*/ CYCLES(0xfb8d, 7);
      /*$FB8F*/ branchTarget = true; block_id = !(s_y != 0x83) ? 1464 : 1463;
      break;
    case 1463:  // $FB91
      /*$FB91*/ CYCLES(0xfb91, 6);
                tmp1_U8 = io_peek(0xc010);
                s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
                block_id = 1464;
      break;
    case 1464:  // $FB94
      /*$FB94*/ CYCLES(0xfb94, 6);
      /*$FBFD*/ CYCLES(0xfbfd, 7);
      /*$FBFF*/ branchTarget = true; block_id = (s_a >= 0xa0) ? 1482 : 1485;
      break;
    case 1465:  // $FB97
      /*$FB97*/ CYCLES(0xfb97, 7);
                s_status_c = 0x01;
      /*$FC2C*/ CYCLES(0xfc2c, 7);
                tmp1_U8 = s_a ^ 0xc0;
                s_a = tmp1_U8;
      /*$FC2E*/ branchTarget = true; block_id = !tmp1_U8 ? 1508 : 1497;
      break;
    case 1466:  // $FB9B
      /*$FB9B*/ CYCLES(0xfb9b, 12);
                tmp1_U8 = s_a;
                s_y = tmp1_U8;
      /*$FB9C*/ s_a = ram_peek((0xfa48 + tmp1_U8));
      /*$FB9F*/ branchTarget = true; push16(0xfba1); block_id = 1465;
      break;
    case 1467:  // $FBA2
      /*$FBA2*/ CYCLES(0xfba2, 6);
                branchTarget = true; push16(0xfba4); block_id = 1544;
      break;
    case 1468:  // $FBA5
      /*$FBA5*/ CYCLES(0xfba5, 7);
      /*$FBA7*/ branchTarget = true; block_id = (s_a >= 0xce) ? 1465 : 1469;
      break;
    case 1469:  // $FBA9
      /*$FBA9*/ CYCLES(0xfba9, 7);
      /*$FBAB*/ branchTarget = true; block_id = !(s_a >= 0xc9) ? 1465 : 1470;
      break;
    case 1470:  // $FBAD
      /*$FBAD*/ CYCLES(0xfbad, 7);
                tmp1_U8 = s_a;
                tmp6_U8 = tmp1_U8 != 0xcc;
                s_status_not_z = tmp6_U8;
                s_status_c = (tmp1_U8 >= 0xcc);
      /*$FBAF*/ branchTarget = true; block_id = !tmp6_U8 ? 1465 : 1471;
      break;
    case 1471:  // $FBB1
      /*$FBB1*/ CYCLES(0xfbb1, 4);
                branchTarget = true; block_id = s_status_not_z ? 1466 : 1472;
      break;
    case 1472:  // $FBB3
      /*$FBB3*/ CYCLES(0xfbb3, 24);
      /*$FBC0*/ FUNC_BASCALC(0x0000);
                block_id = find_block_id_func_t001(0xfbc0, pop16() + 1);;
      break;
    case 1473:  // $FBC1
      /*$FBC1*/ CYCLES(0xfbc1, 23);
                tmp6_U8 = s_a;
                push8(tmp6_U8);
      /*$FBC2*/ tmp1_U8 = tmp6_U8 & 0x01;
                s_status_c = tmp1_U8;
      /*$FBC7*/ ram_poke(0x0029, (((tmp6_U8 >> 0x01) & 0x03) | 0x04));
      /*$FBC9*/ tmp6_U8 = pop8();
      /*$FBCA*/ s_a = (tmp6_U8 & 0x18);
      /*$FBCC*/ branchTarget = true; block_id = !tmp1_U8 ? 1475 : 1474;
      break;
    case 1474:  // $FBCE
      /*$FBCE*/ CYCLES(0xfbce, 4);
                tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 + 0x007f) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x007f);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1475;
      break;
    case 1475:  // $FBD0
      /*$FBD0*/ CYCLES(0xfbd0, 16);
                tmp1_U8 = s_a;
                ram_poke(0x0028, tmp1_U8);
      /*$FBD3*/ tmp5_U16 = tmp1_U8 << 0x02;
                s_status_c = (uint8_t)((tmp5_U16 & 0x01ff) >> 8);
      /*$FBD4*/ tmp1_U8 = ((uint8_t)tmp5_U16) | ram_peek(0x0028);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$FBD6*/ ram_poke(0x0028, tmp1_U8);
      /*$FBD8*/ branchTarget = true; block_id = find_block_id_func_t001(0xfbd8, pop16() + 1);;
      break;
    case 1476:  // $FBD9
      /*$FBD9*/ CYCLES(0xfbd9, 7);
                tmp1_U8 = s_a;
                tmp6_U8 = tmp1_U8 != 0x87;
                s_status_not_z = tmp6_U8;
                s_status_c = (tmp1_U8 >= 0x87);
                s_status_n = ((uint8_t)(tmp1_U8 - 0x87) & 0x80);
      /*$FBDB*/ branchTarget = true; block_id = tmp6_U8 ? 1481 : 1477;
      break;
    case 1477:  // $FBDD
      /*$FBDD*/ CYCLES(0xfbdd, 9);
                s_a = 0x40;
      /*$FBDF*/ branchTarget = true; push16(0xfbe1); block_id = 1522;
      break;
    case 1478:  // $FBE2
      /*$FBE2*/ CYCLES(0xfbe2, 4);
                s_y = 0xc0;
                block_id = 1479;
      break;
    case 1479:  // $FBE4
      /*$FBE4*/ CYCLES(0xfbe4, 9);
                s_a = 0x0c;
      /*$FBE6*/ branchTarget = true; push16(0xfbe8); block_id = 1522;
      break;
    case 1480:  // $FBE9
      /*$FBE9*/ CYCLES(0xfbe9, 11);
                tmp6_U8 = io_peek(0xc030);
                s_a = tmp6_U8;
      /*$FBEC*/ tmp6_U8 = (uint8_t)(s_y - 0x01);
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_y = tmp6_U8;
      /*$FBED*/ branchTarget = true; block_id = tmp6_U8 ? 1479 : 1481;
      break;
    case 1481:  // $FBEF
      /*$FBEF*/ CYCLES(0xfbef, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfbef, pop16() + 1);;
      break;
    case 1482:  // $FBF0
      /*$FBF0*/ CYCLES(0xfbf0, 7);
                tmp6_U8 = ram_peek(0x0024);
                s_y = tmp6_U8;
      /*$FBF2*/ poke((ram_peek16al(0x0028) + tmp6_U8), s_a);
                block_id = 1483;
      break;
    case 1483:  // $FBF4
      /*$FBF4*/ CYCLES(0xfbf4, 14);
                ram_poke(0x0024, (uint8_t)(ram_peek(0x0024) + 0x01));
      /*$FBF6*/ tmp6_U8 = ram_peek(0x0024);
                s_a = tmp6_U8;
      /*$FBF8*/ tmp1_U8 = ram_peek(0x0021);
                s_status_not_z = (tmp6_U8 != tmp1_U8);
                tmp2_U8 = tmp6_U8 >= tmp1_U8;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp6_U8 - tmp1_U8) & 0x80);
      /*$FBFA*/ branchTarget = true; block_id = tmp2_U8 ? 1509 : 1484;
      break;
    case 1484:  // $FBFC
      /*$FBFC*/ CYCLES(0xfbfc, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfbfc, pop16() + 1);;
      break;
    case 1485:  // $FC01
      /*$FC01*/ CYCLES(0xfc01, 6);
                tmp2_U8 = s_a;
                s_y = tmp2_U8;
      /*$FC02*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1482 : 1486;
      break;
    case 1486:  // $FC04
      /*$FC04*/ CYCLES(0xfc04, 7);
      /*$FC06*/ branchTarget = true; block_id = !(s_a != 0x8d) ? 1509 : 1487;
      break;
    case 1487:  // $FC08
      /*$FC08*/ CYCLES(0xfc08, 7);
      /*$FC0A*/ branchTarget = true; block_id = !(s_a != 0x8a) ? 1510 : 1488;
      break;
    case 1488:  // $FC0C
      /*$FC0C*/ CYCLES(0xfc0c, 7);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x88);
      /*$FC0E*/ branchTarget = true; block_id = (tmp2_U8 != 0x88) ? 1476 : 1489;
      break;
    case 1489:  // $FC10
      /*$FC10*/ CYCLES(0xfc10, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x0024) - 0x01);
                s_status_not_z = tmp2_U8;
                tmp1_U8 = tmp2_U8 & 0x80;
                s_status_n = tmp1_U8;
                ram_poke(0x0024, tmp2_U8);
      /*$FC12*/ branchTarget = true; block_id = !tmp1_U8 ? 1484 : 1490;
      break;
    case 1490:  // $FC14
      /*$FC14*/ CYCLES(0xfc14, 11);
      /*$FC16*/ ram_poke(0x0024, ram_peek(0x0021));
      /*$FC18*/ ram_poke(0x0024, (uint8_t)(ram_peek(0x0024) - 0x01));
                block_id = 1491;
      break;
    case 1491:  // $FC1A
      /*$FC1A*/ CYCLES(0xfc1a, 11);
                tmp1_U8 = ram_peek(0x0022);
                s_a = tmp1_U8;
      /*$FC1C*/ tmp2_U8 = ram_peek(0x0025);
                s_status_not_z = (tmp1_U8 != tmp2_U8);
                tmp6_U8 = tmp1_U8 >= tmp2_U8;
                s_status_c = tmp6_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
      /*$FC1E*/ branchTarget = true; block_id = tmp6_U8 ? 1496 : 1492;
      break;
    case 1492:  // $FC20
      /*$FC20*/ CYCLES(0xfc20, 4);
                ram_poke(0x0025, (uint8_t)(ram_peek(0x0025) - 0x01));
                block_id = 1493;
      break;
    case 1493:  // $FC22
      /*$FC22*/ CYCLES(0xfc22, 4);
                s_a = ram_peek(0x0025);
                FUNC_VTABZ(0x0000);
                block_id = find_block_id_func_t001(0xfc22, pop16() + 1);;
      break;
    case 1494:  // $FC24
      /*$FC24*/ CYCLES(0xfc24, 6);
                FUNC_BASCALC(0xfc26);
                branchTarget = true; block_id = 1495;
      break;
    case 1495:  // $FC27
      /*$FC27*/ CYCLES(0xfc27, 7);
                tmp4_U16 = s_a;
                tmp5_U16 = ram_peek(0x0020);
                tmp3_U16 = (tmp4_U16 + tmp5_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)tmp5_U16);
                tmp1_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$FC29*/ ram_poke(0x0028, tmp1_U8);
                block_id = 1496;
      break;
    case 1496:  // $FC2B
      /*$FC2B*/ CYCLES(0xfc2b, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfc2b, pop16() + 1);;
      break;
    case 1497:  // $FC30
      /*$FC30*/ CYCLES(0xfc30, 7);
                tmp5_U16 = s_a;
                tmp3_U16 = (tmp5_U16 + 0x00fd) + s_status_c;
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = tmp2_U8;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp5_U16, (uint8_t)0x00fd);
                tmp6_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp6_U8;
                s_a = tmp6_U8;
      /*$FC32*/ branchTarget = true; block_id = !tmp2_U8 ? 1483 : 1498;
      break;
    case 1498:  // $FC34
      /*$FC34*/ CYCLES(0xfc34, 4);
                branchTarget = true; block_id = !s_status_not_z ? 1489 : 1499;
      break;
    case 1499:  // $FC36
      /*$FC36*/ CYCLES(0xfc36, 7);
                tmp3_U16 = s_a;
                tmp5_U16 = (tmp3_U16 + 0x00fd) + s_status_c;
                tmp2_U8 = (uint8_t)(tmp5_U16 >> 8);
                s_status_c = tmp2_U8;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)0x00fd);
                tmp6_U8 = (uint8_t)tmp5_U16;
                s_status_not_z = tmp6_U8;
                s_a = tmp6_U8;
      /*$FC38*/ branchTarget = true; block_id = !tmp2_U8 ? 1510 : 1500;
      break;
    case 1500:  // $FC3A
      /*$FC3A*/ CYCLES(0xfc3a, 4);
                branchTarget = true; block_id = !s_status_not_z ? 1491 : 1501;
      break;
    case 1501:  // $FC3C
      /*$FC3C*/ CYCLES(0xfc3c, 7);
                tmp5_U16 = s_a;
                tmp3_U16 = (tmp5_U16 + 0x00fd) + s_status_c;
                tmp2_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = tmp2_U8;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp5_U16, (uint8_t)0x00fd);
                tmp6_U8 = (uint8_t)tmp3_U16;
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_a = tmp6_U8;
      /*$FC3E*/ branchTarget = true; block_id = !tmp2_U8 ? 1668 : 1502;
      break;
    case 1502:  // $FC40
      /*$FC40*/ CYCLES(0xfc40, 4);
                branchTarget = true; block_id = s_status_not_z ? 1496 : 1503;
      break;
    case 1503:  // $FC42
      /*$FC42*/ CYCLES(0xfc42, 7);
                s_y = ram_peek(0x0024);
      /*$FC44*/ s_a = ram_peek(0x0025);
                block_id = 1504;
      break;
    case 1504:  // $FC46
      /*$FC46*/ CYCLES(0xfc46, 7);
                push8(s_a);
      /*$FC47*/ FUNC_VTABZ(0xfc49);
                branchTarget = true; block_id = 1505;
      break;
    case 1505:  // $FC4A
      /*$FC4A*/ CYCLES(0xfc4a, 6);
                FUNC_CLREOLZ(0xfc4c);
                branchTarget = true; block_id = 1506;
      break;
    case 1506:  // $FC4D
      /*$FC4D*/ CYCLES(0xfc4d, 16);
                s_y = 0x00;
      /*$FC4F*/ tmp2_U8 = pop8();
      /*$FC50*/ tmp2_U8 = (uint8_t)(tmp2_U8 + s_status_c);
                s_a = tmp2_U8;
      /*$FC52*/ tmp2_U8 = tmp2_U8 >= ram_peek(0x0023);
                s_status_c = tmp2_U8;
      /*$FC54*/ branchTarget = true; block_id = !tmp2_U8 ? 1504 : 1507;
      break;
    case 1507:  // $FC56
      /*$FC56*/ CYCLES(0xfc56, 4);
                branchTarget = true; block_id = s_status_c ? 1493 : 1508;
      break;
    case 1508:  // $FC58
      /*$FC58*/ CYCLES(0xfc58, 18);
                tmp2_U8 = ram_peek(0x0022);
                s_a = tmp2_U8;
      /*$FC5A*/ ram_poke(0x0025, tmp2_U8);
      /*$FC5C*/ s_y = 0x00;
      /*$FC5E*/ ram_poke(0x0024, 0x00);
      /*$FC60*/ branchTarget = true; block_id = 1504;
      break;
    case 1509:  // $FC62
      /*$FC62*/ CYCLES(0xfc62, 7);
      /*$FC64*/ ram_poke(0x0024, 0x00);
                block_id = 1510;
      break;
    case 1510:  // $FC66
      /*$FC66*/ CYCLES(0xfc66, 14);
                ram_poke(0x0025, (uint8_t)(ram_peek(0x0025) + 0x01));
      /*$FC68*/ tmp2_U8 = ram_peek(0x0025);
                s_a = tmp2_U8;
      /*$FC6C*/ branchTarget = true; block_id = !(tmp2_U8 >= ram_peek(0x0023)) ? 1667 : 1511;
      break;
    case 1511:  // $FC6E
      /*$FC6E*/ CYCLES(0xfc6e, 14);
                ram_poke(0x0025, (uint8_t)(ram_peek(0x0025) - 0x01));
      /*$FC70*/ tmp2_U8 = ram_peek(0x0022);
                s_a = tmp2_U8;
      /*$FC72*/ push8(tmp2_U8);
      /*$FC73*/ FUNC_VTABZ(0xfc75);
                branchTarget = true; block_id = 1512;
      break;
    case 1512:  // $FC76
      /*$FC76*/ CYCLES(0xfc76, 31);
      /*$FC78*/ ram_poke(0x002a, ram_peek(0x0028));
      /*$FC7C*/ ram_poke(0x002b, ram_peek(0x0029));
      /*$FC80*/ s_y = (uint8_t)(ram_peek(0x0021) - 0x01);
      /*$FC81*/ tmp6_U8 = pop8();
      /*$FC82*/ tmp3_U16 = tmp6_U8;
                tmp5_U16 = (tmp3_U16 + 0x0001) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)0x0001);
                tmp6_U8 = (uint8_t)tmp5_U16;
                s_a = tmp6_U8;
      /*$FC86*/ branchTarget = true; block_id = (tmp6_U8 >= ram_peek(0x0023)) ? 1516 : 1513;
      break;
    case 1513:  // $FC88
      /*$FC88*/ CYCLES(0xfc88, 7);
                push8(s_a);
      /*$FC89*/ FUNC_VTABZ(0xfc8b);
                branchTarget = true; block_id = 1514;
      break;
    case 1514:  // $FC8C
      /*$FC8C*/ CYCLES(0xfc8c, 12);
                tmp6_U8 = s_y;
                tmp2_U8 = peek((ram_peek16al(0x0028) + tmp6_U8));
      /*$FC8E*/ poke((ram_peek16al(0x002a) + tmp6_U8), tmp2_U8);
      /*$FC90*/ tmp6_U8 = (uint8_t)(tmp6_U8 - 0x01);
                tmp2_U8 = tmp6_U8 & 0x80;
                s_status_n = tmp2_U8;
                s_y = tmp6_U8;
      /*$FC91*/ branchTarget = true; block_id = !tmp2_U8 ? 1514 : 1515;
      break;
    case 1515:  // $FC93
      /*$FC93*/ CYCLES(0xfc93, 4);
                branchTarget = true; block_id = s_status_n ? 1512 : 1516;
      break;
    case 1516:  // $FC95
      /*$FC95*/ CYCLES(0xfc95, 9);
                s_y = 0x00;
      /*$FC97*/ FUNC_CLREOLZ(0xfc99);
                branchTarget = true; block_id = 1517;
      break;
    case 1517:  // $FC9A
      /*$FC9A*/ CYCLES(0xfc9a, 4);
                branchTarget = true; block_id = s_status_c ? 1493 : 1669;
      break;
    case 1518:  // $FC9C
      /*$FC9C*/ CYCLES(0xfc9c, 4);
                s_y = ram_peek(0x0024);
                FUNC_CLREOLZ(0x0000);
                block_id = find_block_id_func_t001(0xfc9c, pop16() + 1);;
      break;
    case 1519:  // $FC9E
      /*$FC9E*/ CYCLES(0xfc9e, 4);
                s_a = 0xa0;
                block_id = 1520;
      break;
    case 1520:  // $FCA0
      /*$FCA0*/ CYCLES(0xfca0, 12);
                tmp2_U8 = s_y;
                poke((ram_peek16al(0x0028) + tmp2_U8), s_a);
      /*$FCA2*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_y = tmp2_U8;
      /*$FCA3*/ tmp6_U8 = ram_peek(0x0021);
                s_status_not_z = (tmp2_U8 != tmp6_U8);
                tmp1_U8 = tmp2_U8 >= tmp6_U8;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp6_U8) & 0x80);
      /*$FCA5*/ branchTarget = true; block_id = !tmp1_U8 ? 1520 : 1521;
      break;
    case 1521:  // $FCA7
      /*$FCA7*/ CYCLES(0xfca7, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfca7, pop16() + 1);;
      break;
    case 1522:  // $FCA8
      /*$FCA8*/ CYCLES(0xfca8, 2);
                s_status_c = 0x01;
                block_id = 1523;
      break;
    case 1523:  // $FCA9
      /*$FCA9*/ CYCLES(0xfca9, 2);
                push8(s_a);
                block_id = 1524;
      break;
    case 1524:  // $FCAA
      /*$FCAA*/ CYCLES(0xfcaa, 7);
                tmp3_U16 = (s_a - 0x0001) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp3_U16 >> 8) & 0x01));
                tmp1_U8 = (uint8_t)tmp3_U16;
                s_a = tmp1_U8;
      /*$FCAC*/ branchTarget = true; block_id = tmp1_U8 ? 1524 : 1525;
      break;
    case 1525:  // $FCAE
      /*$FCAE*/ CYCLES(0xfcae, 9);
                tmp1_U8 = pop8();
      /*$FCAF*/ tmp3_U16 = tmp1_U8;
                tmp5_U16 = (tmp3_U16 - 0x0001) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)0xfffe);
                tmp1_U8 = (uint8_t)tmp5_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$FCB1*/ branchTarget = true; block_id = tmp1_U8 ? 1523 : 1526;
      break;
    case 1526:  // $FCB3
      /*$FCB3*/ CYCLES(0xfcb3, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfcb3, pop16() + 1);;
      break;
    case 1527:  // $FCBA
      /*$FCBA*/ CYCLES(0xfcba, 21);
      /*$FCC0*/ tmp5_U16 = ram_peek(0x003d);
                tmp3_U16 = ram_peek(0x003f);
                tmp4_U16 = (tmp5_U16 - tmp3_U16) - (ram_peek(0x003c) < ram_peek(0x003e));
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)(~tmp3_U16));
                s_a = ((uint8_t)tmp4_U16);
      /*$FCC2*/ tmp1_U8 = (uint8_t)(ram_peek(0x003c) + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x003c, tmp1_U8);
      /*$FCC4*/ branchTarget = true; block_id = tmp1_U8 ? 1529 : 1528;
      break;
    case 1528:  // $FCC6
      /*$FCC6*/ CYCLES(0xfcc6, 4);
                tmp1_U8 = (uint8_t)(ram_peek(0x003d) + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x003d, tmp1_U8);
                block_id = 1529;
      break;
    case 1529:  // $FCC8
      /*$FCC8*/ CYCLES(0xfcc8, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfcc8, pop16() + 1);;
      break;
    case 1530:  // $FCC9
      /*$FCC9*/ CYCLES(0xfcc9, 9);
                s_y = 0x4b;
      /*$FCCB*/ FUNC_ZERODLY(0xfccd);
      /*$FCCE*/ CYCLES(0xfcce, 4);
                branchTarget = true; block_id = s_status_not_z ? 1530 : 1531;
      break;
    case 1531:  // $FCD0
      /*$FCD0*/ CYCLES(0xfcd0, 7);
                tmp4_U16 = s_a;
                tmp3_U16 = (tmp4_U16 + 0x00fe) + s_status_c;
                tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
                s_status_c = tmp1_U8;
                s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0x00fe);
                s_a = ((uint8_t)tmp3_U16);
      /*$FCD2*/ branchTarget = true; block_id = tmp1_U8 ? 1530 : 1532;
      break;
    case 1532:  // $FCD4
      /*$FCD4*/ CYCLES(0xfcd4, 9);
                s_y = 0x21;
      /*$FCD6*/ FUNC_ZERODLY(0xfcd8);
      /*$FCD9*/ CYCLES(0xfcd9, 4);
      /*$FCDA*/ s_y = (uint8_t)(s_y + 0x02);
                FUNC_ZERODLY(0x0000);
                block_id = find_block_id_func_t001(0xfcda, pop16() + 1);;
      break;
    case 1533:  // $FCDB
      /*$FCDB*/ CYCLES(0xfcdb, 6);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp1_U8;
      /*$FCDC*/ branchTarget = true; block_id = tmp1_U8 ? 1533 : 1534;
      break;
    case 1534:  // $FCDE
      /*$FCDE*/ CYCLES(0xfcde, 4);
                branchTarget = true; block_id = !s_status_c ? 1537 : 1535;
      break;
    case 1535:  // $FCE0
      /*$FCE0*/ CYCLES(0xfce0, 4);
                s_y = 0x32;
                block_id = 1536;
      break;
    case 1536:  // $FCE2
      /*$FCE2*/ CYCLES(0xfce2, 6);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp1_U8;
      /*$FCE3*/ branchTarget = true; block_id = tmp1_U8 ? 1536 : 1537;
      break;
    case 1537:  // $FCE5
      /*$FCE5*/ CYCLES(0xfce5, 12);
                tmp1_U8 = io_peek(0xc020);
      /*$FCE8*/ s_y = 0x2c;
      /*$FCEA*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$FCEB*/ branchTarget = true; block_id = find_block_id_func_t001(0xfceb, pop16() + 1);;
      break;
    case 1538:  // $FCEC
      /*$FCEC*/ CYCLES(0xfcec, 4);
                s_x = 0x08;
                block_id = 1539;
      break;
    case 1539:  // $FCEE
      /*$FCEE*/ CYCLES(0xfcee, 7);
                push8(s_a);
      /*$FCEF*/ FUNC_RD2BIT(0xfcf1);
      /*$FCF2*/ CYCLES(0xfcf2, 12);
                tmp1_U8 = pop8();
      /*$FCF3*/ tmp3_U16 = (tmp1_U8 << 0x01) | s_status_c;
                s_status_c = (uint8_t)(tmp3_U16 >> 8);
                s_a = ((uint8_t)tmp3_U16);
      /*$FCF4*/ s_y = 0x3a;
      /*$FCF6*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$FCF7*/ branchTarget = true; block_id = tmp1_U8 ? 1539 : 1540;
      break;
    case 1540:  // $FCF9
      /*$FCF9*/ CYCLES(0xfcf9, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfcf9, pop16() + 1);;
      break;
    case 1541:  // $FCFA
      /*$FCFA*/ CYCLES(0xfcfa, 6);
                FUNC_RDBIT(0xfcfc);
                FUNC_RDBIT(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xfcfa, pop16() + 1);;
      break;
    case 1542:  // $FCFD
      /*$FCFD*/ CYCLES(0xfcfd, 14);
                s_y = (uint8_t)(s_y - 0x01);
      /*$FCFE*/ tmp1_U8 = io_peek(0xc060);
      /*$FD01*/ tmp1_U8 = tmp1_U8 ^ ram_peek(0x002f);
                s_a = tmp1_U8;
      /*$FD03*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 1542 : 1543;
      break;
    case 1543:  // $FD05
      /*$FD05*/ CYCLES(0xfd05, 12);
                tmp1_U8 = s_a ^ ram_peek(0x002f);
                s_a = tmp1_U8;
      /*$FD07*/ ram_poke(0x002f, tmp1_U8);
      /*$FD09*/ tmp1_U8 = s_y;
                s_status_not_z = (tmp1_U8 != 0x80);
                s_status_c = (tmp1_U8 >= 0x80);
                s_status_n = ((uint8_t)(tmp1_U8 - 0x80) & 0x80);
      /*$FD0B*/ branchTarget = true; block_id = find_block_id_func_t001(0xfd0b, pop16() + 1);;
      break;
    case 1544:  // $FD0C
      /*$FD0C*/ CYCLES(0xfd0c, 26);
                tmp1_U8 = ram_peek(0x0024);
                s_y = tmp1_U8;
      /*$FD0E*/ tmp6_U8 = peek((ram_peek16al(0x0028) + tmp1_U8));
      /*$FD10*/ push8(tmp6_U8);
      /*$FD15*/ poke((ram_peek16al(0x0028) + tmp1_U8), ((tmp6_U8 & 0x3f) | 0x40));
      /*$FD17*/ tmp1_U8 = pop8();
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$FD18*/ branchTarget = true; block_id = find_block_id_func_t001(0xfd18, ram_peek16al(0x0038));
      break;
    case 1545:  // $FD1B
      /*$FD1B*/ CYCLES(0xfd1b, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x004e) + 0x01);
                ram_poke(0x004e, tmp1_U8);
      /*$FD1D*/ branchTarget = true; block_id = tmp1_U8 ? 1547 : 1546;
      break;
    case 1546:  // $FD1F
      /*$FD1F*/ CYCLES(0xfd1f, 4);
                ram_poke(0x004f, (uint8_t)(ram_peek(0x004f) + 0x01));
                block_id = 1547;
      break;
    case 1547:  // $FD21
      /*$FD21*/ CYCLES(0xfd21, 9);
                tmp1_U8 = io_peek(0xc000);
      /*$FD24*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 1545 : 1548;
      break;
    case 1548:  // $FD26
      /*$FD26*/ CYCLES(0xfd26, 16);
                poke((ram_peek16al(0x0028) + s_y), s_a);
      /*$FD28*/ tmp1_U8 = io_peek(0xc000);
                s_a = tmp1_U8;
      /*$FD2B*/ tmp6_U8 = io_peek(0xc010);
                s_status_n = (tmp6_U8 & 0x80);
                s_status_v = ((tmp6_U8 >> 0x06) & 0x01);
                s_status_not_z = (tmp1_U8 & tmp6_U8);
      /*$FD2E*/ branchTarget = true; block_id = find_block_id_func_t001(0xfd2e, pop16() + 1);;
      break;
    case 1549:  // $FD2F
      /*$FD2F*/ CYCLES(0xfd2f, 6);
                branchTarget = true; push16(0xfd31); block_id = 1544;
      break;
    case 1550:  // $FD32
      /*$FD32*/ CYCLES(0xfd32, 6);
                branchTarget = true; push16(0xfd34); block_id = 1468;
      break;
    case 1551:  // $FD35
      /*$FD35*/ CYCLES(0xfd35, 6);
                branchTarget = true; push16(0xfd37); block_id = 1544;
      break;
    case 1552:  // $FD38
      /*$FD38*/ CYCLES(0xfd38, 7);
                tmp6_U8 = s_a;
                tmp1_U8 = tmp6_U8 != 0x9b;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp6_U8 >= 0x9b);
                s_status_n = ((uint8_t)(tmp6_U8 - 0x9b) & 0x80);
      /*$FD3A*/ branchTarget = true; block_id = !tmp1_U8 ? 1549 : 1553;
      break;
    case 1553:  // $FD3C
      /*$FD3C*/ CYCLES(0xfd3c, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfd3c, pop16() + 1);;
      break;
    case 1554:  // $FD3D
      /*$FD3D*/ CYCLES(0xfd3d, 23);
      /*$FD3F*/ push8(ram_peek(0x0032));
      /*$FD42*/ ram_poke(0x0032, 0xff);
      /*$FD44*/ tmp1_U8 = ram_peek((0x0200 + s_x));
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$FD47*/ branchTarget = true; push16(0xfd49); block_id = 1594;
      break;
    case 1555:  // $FD4A
      /*$FD4A*/ CYCLES(0xfd4a, 18);
                tmp1_U8 = pop8();
      /*$FD4B*/ ram_poke(0x0032, tmp1_U8);
      /*$FD4D*/ tmp1_U8 = ram_peek((0x0200 + s_x));
                s_a = tmp1_U8;
      /*$FD50*/ s_status_c = (tmp1_U8 >= 0x88);
      /*$FD52*/ branchTarget = true; block_id = !(tmp1_U8 != 0x88) ? 1564 : 1556;
      break;
    case 1556:  // $FD54
      /*$FD54*/ CYCLES(0xfd54, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0x98);
      /*$FD56*/ branchTarget = true; block_id = !(tmp1_U8 != 0x98) ? 1560 : 1557;
      break;
    case 1557:  // $FD58
      /*$FD58*/ CYCLES(0xfd58, 7);
                tmp1_U8 = s_x >= 0xf8;
                s_status_c = tmp1_U8;
      /*$FD5A*/ branchTarget = true; block_id = !tmp1_U8 ? 1559 : 1558;
      break;
    case 1558:  // $FD5C
      /*$FD5C*/ CYCLES(0xfd5c, 6);
                branchTarget = true; push16(0xfd5e); block_id = 1637;
      break;
    case 1559:  // $FD5F
      /*$FD5F*/ CYCLES(0xfd5f, 6);
                tmp1_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp1_U8;
      /*$FD60*/ branchTarget = true; block_id = tmp1_U8 ? 1566 : 1560;
      break;
    case 1560:  // $FD62
      /*$FD62*/ CYCLES(0xfd62, 9);
                s_status_not_z = 0xdc;
                s_status_n = 0x80;
                s_a = 0xdc;
      /*$FD64*/ branchTarget = true; push16(0xfd66); block_id = 1594;
      break;
    case 1561:  // $FD67
      /*$FD67*/ CYCLES(0xfd67, 6);
                branchTarget = true; push16(0xfd69); block_id = 1573;
      break;
    case 1562:  // $FD6A
      /*$FD6A*/ CYCLES(0xfd6a, 9);
                tmp1_U8 = ram_peek(0x0033);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$FD6C*/ branchTarget = true; push16(0xfd6e); block_id = 1594;
      break;
    case 1563:  // $FD6F
      /*$FD6F*/ CYCLES(0xfd6f, 4);
                s_x = 0x01;
                block_id = 1564;
      break;
    case 1564:  // $FD71
      /*$FD71*/ CYCLES(0xfd71, 6);
                tmp1_U8 = s_x;
                s_a = tmp1_U8;
      /*$FD72*/ branchTarget = true; block_id = !tmp1_U8 ? 1561 : 1565;
      break;
    case 1565:  // $FD74
      /*$FD74*/ CYCLES(0xfd74, 2);
                s_x = (uint8_t)(s_x - 0x01);
                block_id = 1566;
      break;
    case 1566:  // $FD75
      /*$FD75*/ CYCLES(0xfd75, 6);
                branchTarget = true; push16(0xfd77); block_id = 1551;
      break;
    case 1567:  // $FD78
      /*$FD78*/ CYCLES(0xfd78, 7);
      /*$FD7A*/ branchTarget = true; block_id = (s_a != 0x95) ? 1569 : 1568;
      break;
    case 1568:  // $FD7C
      /*$FD7C*/ CYCLES(0xfd7c, 4);
                tmp1_U8 = peek((ram_peek16al(0x0028) + s_y));
                s_a = tmp1_U8;
                block_id = 1569;
      break;
    case 1569:  // $FD7E
      /*$FD7E*/ CYCLES(0xfd7e, 7);
      /*$FD80*/ branchTarget = true; block_id = !(s_a >= 0xe0) ? 1571 : 1570;
      break;
    case 1570:  // $FD82
      /*$FD82*/ CYCLES(0xfd82, 4);
                s_a = (s_a & 0xdf);
                block_id = 1571;
      break;
    case 1571:  // $FD84
      /*$FD84*/ CYCLES(0xfd84, 12);
                tmp1_U8 = s_a;
                ram_poke((0x0200 + s_x), tmp1_U8);
      /*$FD87*/ s_status_c = (tmp1_U8 >= 0x8d);
      /*$FD89*/ branchTarget = true; block_id = (tmp1_U8 != 0x8d) ? 1554 : 1572;
      break;
    case 1572:  // $FD8B
      /*$FD8B*/ CYCLES(0xfd8b, 6);
                FUNC_CLREOL(0xfd8d);
                branchTarget = true; block_id = 1573;
      break;
    case 1573:  // $FD8E
      /*$FD8E*/ CYCLES(0xfd8e, 7);
                s_status_not_z = 0x8d;
                s_status_n = 0x80;
                s_a = 0x8d;
      /*$FD90*/ branchTarget = true; block_id = 1594;
      break;
    case 1574:  // $FD92
      /*$FD92*/ CYCLES(0xfd92, 7);
                s_y = ram_peek(0x003d);
      /*$FD94*/ s_x = ram_peek(0x003c);
                block_id = 1575;
      break;
    case 1575:  // $FD96
      /*$FD96*/ CYCLES(0xfd96, 6);
                branchTarget = true; push16(0xfd98); block_id = 1573;
      break;
    case 1576:  // $FD99
      /*$FD99*/ CYCLES(0xfd99, 6);
                branchTarget = true; push16(0xfd9b); block_id = 1413;
      break;
    case 1577:  // $FD9C
      /*$FD9C*/ CYCLES(0xfd9c, 12);
                s_y = 0x00;
      /*$FD9E*/ s_status_not_z = 0xad;
                s_status_n = 0x80;
                s_a = 0xad;
      /*$FDA0*/ branchTarget = true; block_id = 1594;
      break;
    case 1578:  // $FDA3
      /*$FDA3*/ CYCLES(0xfda3, 18);
      /*$FDA7*/ ram_poke(0x003e, (ram_peek(0x003c) | 0x07));
      /*$FDAB*/ ram_poke(0x003f, ram_peek(0x003d));
                block_id = 1579;
      break;
    case 1579:  // $FDAD
      /*$FDAD*/ CYCLES(0xfdad, 11);
      /*$FDB1*/ branchTarget = true; block_id = (ram_peek(0x003c) & 0x07) ? 1581 : 1580;
      break;
    case 1580:  // $FDB3
      /*$FDB3*/ CYCLES(0xfdb3, 6);
                branchTarget = true; push16(0xfdb5); block_id = 1574;
      break;
    case 1581:  // $FDB6
      /*$FDB6*/ CYCLES(0xfdb6, 9);
                s_status_not_z = 0xa0;
                s_status_n = 0x80;
                s_a = 0xa0;
      /*$FDB8*/ branchTarget = true; push16(0xfdba); block_id = 1594;
      break;
    case 1582:  // $FDBB
      /*$FDBB*/ CYCLES(0xfdbb, 9);
                tmp1_U8 = peek((ram_peek16al(0x003c) + s_y));
                s_a = tmp1_U8;
      /*$FDBD*/ branchTarget = true; push16(0xfdbf); block_id = 1590;
      break;
    case 1583:  // $FDC0
      /*$FDC0*/ CYCLES(0xfdc0, 6);
                FUNC_NXTA1(0xfdc2);
      /*$FDC3*/ CYCLES(0xfdc3, 4);
                branchTarget = true; block_id = !s_status_c ? 1579 : 1584;
      break;
    case 1584:  // $FDC5
      /*$FDC5*/ CYCLES(0xfdc5, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfdc5, pop16() + 1);;
      break;
    case 1585:  // $FDC6
      /*$FDC6*/ CYCLES(0xfdc6, 6);
                tmp1_U8 = s_a;
                tmp6_U8 = tmp1_U8 & 0x01;
                s_status_c = tmp6_U8;
                s_a = (tmp1_U8 >> 0x01);
      /*$FDC7*/ branchTarget = true; block_id = !tmp6_U8 ? 1580 : 1586;
      break;
    case 1586:  // $FDC9
      /*$FDC9*/ CYCLES(0xfdc9, 11);
      /*$FDCA*/ tmp6_U8 = (s_a >> 0x01) & 0x01;
                s_status_c = tmp6_U8;
      /*$FDCB*/ s_a = ram_peek(0x003e);
      /*$FDCD*/ branchTarget = true; block_id = !tmp6_U8 ? 1588 : 1587;
      break;
    case 1587:  // $FDCF
      /*$FDCF*/ CYCLES(0xfdcf, 4);
                s_a = (s_a ^ 0xff);
                block_id = 1588;
      break;
    case 1588:  // $FDD1
      /*$FDD1*/ CYCLES(0xfdd1, 14);
                tmp3_U16 = s_a;
                tmp4_U16 = ram_peek(0x003c);
                tmp5_U16 = (tmp3_U16 + tmp4_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp3_U16, (uint8_t)tmp4_U16);
      /*$FDD3*/ push8(((uint8_t)tmp5_U16));
      /*$FDD4*/ s_status_not_z = 0xbd;
                s_status_n = 0x80;
                s_a = 0xbd;
      /*$FDD6*/ branchTarget = true; push16(0xfdd8); block_id = 1594;
      break;
    case 1589:  // $FDD9
      /*$FDD9*/ CYCLES(0xfdd9, 2);
                tmp6_U8 = pop8();
                s_a = tmp6_U8;
                block_id = 1590;
      break;
    case 1590:  // $FDDA
      /*$FDDA*/ CYCLES(0xfdda, 14);
                tmp6_U8 = s_a;
                push8(tmp6_U8);
      /*$FDDE*/ s_a = (tmp6_U8 >> 0x04);
      /*$FDDF*/ branchTarget = true; push16(0xfde1); block_id = 1592;
      break;
    case 1591:  // $FDE2
      /*$FDE2*/ CYCLES(0xfde2, 6);
                tmp6_U8 = pop8();
      /*$FDE3*/ s_a = (tmp6_U8 & 0x0f);
                block_id = 1592;
      break;
    case 1592:  // $FDE5
      /*$FDE5*/ CYCLES(0xfde5, 11);
                tmp6_U8 = s_a | 0xb0;
                s_a = tmp6_U8;
      /*$FDE7*/ s_status_not_z = (tmp6_U8 != 0xba);
                tmp1_U8 = tmp6_U8 >= 0xba;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp6_U8 - 0xba) & 0x80);
      /*$FDE9*/ branchTarget = true; block_id = !tmp1_U8 ? 1594 : 1593;
      break;
    case 1593:  // $FDEB
      /*$FDEB*/ CYCLES(0xfdeb, 4);
                tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 + 0x0006) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x0006);
                tmp1_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 1594;
      break;
    case 1594:  // $FDED
      /*$FDED*/ CYCLES(0xfded, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xfded, ram_peek16al(0x0036));
      break;
    case 1595:  // $FDF0
      /*$FDF0*/ CYCLES(0xfdf0, 7);
                tmp1_U8 = s_a >= 0xa0;
                s_status_c = tmp1_U8;
      /*$FDF2*/ branchTarget = true; block_id = !tmp1_U8 ? 1597 : 1596;
      break;
    case 1596:  // $FDF4
      /*$FDF4*/ CYCLES(0xfdf4, 4);
                s_a = (s_a & ram_peek(0x0032));
                block_id = 1597;
      break;
    case 1597:  // $FDF6
      /*$FDF6*/ CYCLES(0xfdf6, 11);
                ram_poke(0x0035, s_y);
      /*$FDF8*/ push8(s_a);
      /*$FDF9*/ branchTarget = true; push16(0xfdfb); block_id = 1457;
      break;
    case 1598:  // $FDFC
      /*$FDFC*/ CYCLES(0xfdfc, 7);
                tmp6_U8 = pop8();
                s_a = tmp6_U8;
      /*$FDFD*/ tmp6_U8 = ram_peek(0x0035);
                s_status_not_z = tmp6_U8;
                s_status_n = (tmp6_U8 & 0x80);
                s_y = tmp6_U8;
      /*$FDFF*/ branchTarget = true; block_id = find_block_id_func_t001(0xfdff, pop16() + 1);;
      break;
    case 1599:  // $FE00
      /*$FE00*/ CYCLES(0xfe00, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x0034) - 0x01);
                ram_poke(0x0034, tmp1_U8);
      /*$FE02*/ branchTarget = true; block_id = !tmp1_U8 ? 1578 : 1600;
      break;
    case 1600:  // $FE04
      /*$FE04*/ CYCLES(0xfe04, 6);
                tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$FE05*/ branchTarget = true; block_id = tmp1_U8 ? 1605 : 1601;
      break;
    case 1601:  // $FE07
      /*$FE07*/ CYCLES(0xfe07, 7);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 >= 0xba);
      /*$FE09*/ branchTarget = true; block_id = (tmp1_U8 != 0xba) ? 1585 : 1602;
      break;
    case 1602:  // $FE0B
      /*$FE0B*/ CYCLES(0xfe0b, 18);
                ram_poke(0x0031, s_a);
      /*$FE0D*/ tmp1_U8 = ram_peek(0x003e);
                s_a = tmp1_U8;
      /*$FE0F*/ poke((ram_peek16al(0x0040) + s_y), tmp1_U8);
      /*$FE11*/ tmp1_U8 = (uint8_t)(ram_peek(0x0040) + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x0040, tmp1_U8);
      /*$FE13*/ branchTarget = true; block_id = tmp1_U8 ? 1604 : 1603;
      break;
    case 1603:  // $FE15
      /*$FE15*/ CYCLES(0xfe15, 4);
                tmp1_U8 = (uint8_t)(ram_peek(0x0041) + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x0041, tmp1_U8);
                block_id = 1604;
      break;
    case 1604:  // $FE17
      /*$FE17*/ CYCLES(0xfe17, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfe17, pop16() + 1);;
      break;
    case 1605:  // $FE1D
      /*$FE1D*/ CYCLES(0xfe1d, 6);
                ram_poke(0x0031, s_a);
      /*$FE1F*/ branchTarget = true; block_id = find_block_id_func_t001(0xfe1f, pop16() + 1);;
      break;
    case 1606:  // $FE5E
      /*$FE5E*/ CYCLES(0xfe5e, 6);
                FUNC_A1PC(0xfe60);
                branchTarget = true; block_id = 1607;
      break;
    case 1607:  // $FE61
      /*$FE61*/ CYCLES(0xfe61, 4);
                s_a = 0x14;
                block_id = 1608;
      break;
    case 1608:  // $FE63
      /*$FE63*/ CYCLES(0xfe63, 7);
                push8(s_a);
      /*$FE64*/ branchTarget = true; push16(0xfe66); block_id = 1387;
      break;
    case 1609:  // $FE67
      /*$FE67*/ CYCLES(0xfe67, 6);
                FUNC_PCADJ(0xfe69);
                branchTarget = true; block_id = 1610;
      break;
    case 1610:  // $FE6A
      /*$FE6A*/ CYCLES(0xfe6a, 18);
                ram_poke(0x003a, s_a);
      /*$FE6C*/ ram_poke(0x003b, s_y);
      /*$FE6E*/ tmp1_U8 = pop8();
      /*$FE70*/ tmp4_U16 = tmp1_U8;
                tmp5_U16 = tmp4_U16 - 0x0001;
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0xfffe);
                tmp1_U8 = (uint8_t)tmp5_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$FE72*/ branchTarget = true; block_id = tmp1_U8 ? 1608 : 1611;
      break;
    case 1611:  // $FE74
      /*$FE74*/ CYCLES(0xfe74, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfe74, pop16() + 1);;
      break;
    case 1612:  // $FE75
      /*$FE75*/ CYCLES(0xfe75, 6);
                tmp1_U8 = s_x;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$FE76*/ branchTarget = true; block_id = !tmp1_U8 ? 1614 : 1613;
      break;
    case 1613:  // $FE78
      /*$FE78*/ CYCLES(0xfe78, 12);
                tmp1_U8 = s_x;
                tmp6_U8 = ram_peek((uint8_t)(0x3c + tmp1_U8));
                s_a = tmp6_U8;
      /*$FE7A*/ ram_poke((uint8_t)(0x3a + tmp1_U8), tmp6_U8);
      /*$FE7C*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_status_not_z = tmp1_U8;
                tmp6_U8 = tmp1_U8 & 0x80;
                s_status_n = tmp6_U8;
                s_x = tmp1_U8;
      /*$FE7D*/ branchTarget = true; block_id = !tmp6_U8 ? 1613 : 1614;
      break;
    case 1614:  // $FE7F
      /*$FE7F*/ CYCLES(0xfe7f, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xfe7f, pop16() + 1);;
      break;
    case 1615:  // $FE84
      /*$FE84*/ CYCLES(0xfe84, 9);
                s_status_not_z = 0xff;
                s_status_n = 0x80;
                s_y = 0xff;
      /*$FE86*/ ram_poke(0x0032, 0xff);
      /*$FE88*/ branchTarget = true; block_id = find_block_id_func_t001(0xfe88, pop16() + 1);;
      break;
    case 1616:  // $FE89
      /*$FE89*/ CYCLES(0xfe89, 18);
      /*$FE8B*/ ram_poke(0x003e, 0x00);
      /*$FE8D*/ s_x = 0x38;
      /*$FE8F*/ s_y = 0x1b;
      /*$FE91*/ branchTarget = true; block_id = 1618;
      break;
    case 1617:  // $FE93
      /*$FE93*/ CYCLES(0xfe93, 14);
      /*$FE95*/ ram_poke(0x003e, 0x00);
      /*$FE97*/ s_x = 0x36;
      /*$FE99*/ s_y = 0xf0;
                block_id = 1618;
      break;
    case 1618:  // $FE9B
      /*$FE9B*/ CYCLES(0xfe9b, 11);
      /*$FE9D*/ tmp6_U8 = ram_peek(0x003e) & 0x0f;
                s_a = tmp6_U8;
      /*$FE9F*/ branchTarget = true; block_id = !tmp6_U8 ? 1620 : 1619;
      break;
    case 1619:  // $FEA1
      /*$FEA1*/ CYCLES(0xfea1, 11);
                s_a = (s_a | 0xc0);
      /*$FEA3*/ s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_y = 0x00;
      /*$FEA5*/ branchTarget = true; block_id = 1621;
      break;
    case 1620:  // $FEA7
      /*$FEA7*/ CYCLES(0xfea7, 4);
                s_status_not_z = 0xfd;
                s_status_n = 0x80;
                s_a = 0xfd;
                block_id = 1621;
      break;
    case 1621:  // $FEA9
      /*$FEA9*/ CYCLES(0xfea9, 9);
                tmp6_U8 = s_x;
                ram_poke(tmp6_U8, s_y);
      /*$FEAB*/ ram_poke((uint8_t)(0x01 + tmp6_U8), s_a);
      /*$FEAD*/ branchTarget = true; block_id = find_block_id_func_t001(0xfead, pop16() + 1);;
      break;
    case 1622:  // $FEB6
      /*$FEB6*/ CYCLES(0xfeb6, 6);
                FUNC_A1PC(0xfeb8);
                branchTarget = true; block_id = 1623;
      break;
    case 1623:  // $FEB9
      /*$FEB9*/ CYCLES(0xfeb9, 6);
                FUNC_MON_RESTORE(0xfebb);
                branchTarget = true; block_id = 1624;
      break;
    case 1624:  // $FEBC
      /*$FEBC*/ CYCLES(0xfebc, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xfebc, ram_peek16al(0x003a));
      break;
    case 1625:  // $FEF6
      /*$FEF6*/ CYCLES(0xfef6, 6);
                branchTarget = true; push16(0xfef8); block_id = 1599;
      break;
    case 1626:  // $FEF9
      /*$FEF9*/ CYCLES(0xfef9, 7);
                tmp1_U8 = pop8();
      /*$FEFA*/ tmp1_U8 = pop8();
      /*$FEFB*/ branchTarget = true; block_id = tmp1_U8 ? 1640 : 1627;
      break;
    case 1627:  // $FEFD
      /*$FEFD*/ CYCLES(0xfefd, 6);
                FUNC_RD2BIT(0xfeff);
      /*$FF00*/ CYCLES(0xff00, 9);
                s_a = 0x16;
      /*$FF02*/ FUNC_HEADER(0xff04);
      /*$FF05*/ CYCLES(0xff05, 9);
                ram_poke(0x002e, s_a);
      /*$FF07*/ FUNC_RD2BIT(0xff09);
                branchTarget = true; block_id = 1628;
      break;
    case 1628:  // $FF0A
      /*$FF0A*/ CYCLES(0xff0a, 9);
                s_y = 0x24;
      /*$FF0C*/ FUNC_RDBIT(0xff0e);
      /*$FF0F*/ CYCLES(0xff0f, 4);
                branchTarget = true; block_id = s_status_c ? 1628 : 1629;
      break;
    case 1629:  // $FF11
      /*$FF11*/ CYCLES(0xff11, 6);
                FUNC_RDBIT(0xff13);
      /*$FF14*/ CYCLES(0xff14, 4);
                s_y = 0x3b;
                block_id = 1630;
      break;
    case 1630:  // $FF16
      /*$FF16*/ CYCLES(0xff16, 6);
                branchTarget = true; push16(0xff18); block_id = 1538;
      break;
    case 1631:  // $FF19
      /*$FF19*/ CYCLES(0xff19, 16);
                tmp6_U8 = s_x;
                tmp1_U8 = s_a;
                poke((ram_peek((uint8_t)(0x3c + tmp6_U8)) + (ram_peek((uint8_t)(0x3d + tmp6_U8)) << 8)), tmp1_U8);
      /*$FF1D*/ ram_poke(0x002e, (tmp1_U8 ^ ram_peek(0x002e)));
      /*$FF1F*/ FUNC_NXTA1(0xff21);
      /*$FF22*/ CYCLES(0xff22, 7);
                s_y = 0x35;
      /*$FF24*/ branchTarget = true; block_id = !s_status_c ? 1630 : 1632;
      break;
    case 1632:  // $FF26
      /*$FF26*/ CYCLES(0xff26, 6);
                branchTarget = true; push16(0xff28); block_id = 1538;
      break;
    case 1633:  // $FF29
      /*$FF29*/ CYCLES(0xff29, 7);
                tmp1_U8 = s_a;
                tmp6_U8 = ram_peek(0x002e);
                s_status_c = (tmp1_U8 >= tmp6_U8);
      /*$FF2B*/ branchTarget = true; block_id = !(tmp1_U8 != tmp6_U8) ? 1637 : 1634;
      break;
    case 1634:  // $FF2D
      /*$FF2D*/ CYCLES(0xff2d, 9);
                s_status_not_z = 0xc5;
                s_status_n = 0x80;
                s_a = 0xc5;
      /*$FF2F*/ branchTarget = true; push16(0xff31); block_id = 1594;
      break;
    case 1635:  // $FF32
      /*$FF32*/ CYCLES(0xff32, 9);
                s_status_not_z = 0xd2;
                s_status_n = 0x80;
                s_a = 0xd2;
      /*$FF34*/ branchTarget = true; push16(0xff36); block_id = 1594;
      break;
    case 1636:  // $FF37
      /*$FF37*/ CYCLES(0xff37, 6);
                branchTarget = true; push16(0xff39); block_id = 1594;
      break;
    case 1637:  // $FF3A
      /*$FF3A*/ CYCLES(0xff3a, 9);
                s_status_not_z = 0x87;
                s_status_n = 0x80;
                s_a = 0x87;
      /*$FF3C*/ branchTarget = true; block_id = 1594;
      break;
    case 1638:  // $FF3F
      /*$FF3F*/ CYCLES(0xff3f, 19);
      /*$FF41*/ push8(ram_peek(0x0048));
      /*$FF42*/ s_a = ram_peek(0x0045);
      /*$FF44*/ s_x = ram_peek(0x0046);
      /*$FF46*/ s_y = ram_peek(0x0047);
      /*$FF48*/ tmp6_U8 = pop8();
                s_status_c = (tmp6_U8 & 0x01);
                s_status_not_z = (~tmp6_U8 & 2);
                s_status_i = ((tmp6_U8 & 0x04) != 0);
                s_status_d = ((tmp6_U8 & 0x08) != 0);
                s_status_b = 0x00;
                s_status_v = ((tmp6_U8 & 0x40) != 0);
                s_status_n = (tmp6_U8 & 0x80);
      /*$FF49*/ branchTarget = true; block_id = find_block_id_func_t001(0xff49, pop16() + 1);;
      break;
    case 1639:  // $FF65
      /*$FF65*/ CYCLES(0xff65, 7);
                s_status_d = 0x00;
      /*$FF66*/ branchTarget = true; push16(0xff68); block_id = 1637;
      break;
    case 1640:  // $FF69
      /*$FF69*/ CYCLES(0xff69, 12);
                s_a = 0xaa;
      /*$FF6B*/ ram_poke(0x0033, 0xaa);
      /*$FF6D*/ branchTarget = true; push16(0xff6f); block_id = 1561;
      break;
    case 1641:  // $FF70
      /*$FF70*/ CYCLES(0xff70, 6);
                FUNC_ZMODE(0xff72);
                branchTarget = true; block_id = 1642;
      break;
    case 1642:  // $FF73
      /*$FF73*/ CYCLES(0xff73, 6);
                FUNC_GETNUM(0xff75);
                branchTarget = true; block_id = 1643;
      break;
    case 1643:  // $FF76
      /*$FF76*/ CYCLES(0xff76, 7);
                ram_poke(0x0034, s_y);
      /*$FF78*/ s_y = 0x17;
                block_id = 1644;
      break;
    case 1644:  // $FF7A
      /*$FF7A*/ CYCLES(0xff7a, 6);
                tmp6_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp6_U8;
      /*$FF7B*/ branchTarget = true; block_id = (tmp6_U8 & 0x80) ? 1639 : 1645;
      break;
    case 1645:  // $FF7D
      /*$FF7D*/ CYCLES(0xff7d, 9);
                tmp6_U8 = s_a;
                tmp1_U8 = ram_peek((0xffcc + s_y));
                s_status_c = (tmp6_U8 >= tmp1_U8);
      /*$FF80*/ branchTarget = true; block_id = (tmp6_U8 != tmp1_U8) ? 1644 : 1646;
      break;
    case 1646:  // $FF82
      /*$FF82*/ CYCLES(0xff82, 6);
                branchTarget = true; push16(0xff84); block_id = 1658;
      break;
    case 1647:  // $FF85
      /*$FF85*/ CYCLES(0xff85, 9);
                s_y = ram_peek(0x0034);
      /*$FF87*/ branchTarget = true; block_id = 1642;
      break;
    case 1648:  // $FF8A
      /*$FF8A*/ CYCLES(0xff8a, 11);
                s_x = 0x03;
      /*$FF8F*/ s_a = (uint8_t)(s_a << 0x04);
                block_id = 1649;
      break;
    case 1649:  // $FF90
      /*$FF90*/ CYCLES(0xff90, 14);
                tmp4_U16 = s_a << 0x01;
                s_a = ((uint8_t)tmp4_U16);
      /*$FF91*/ tmp4_U16 = (ram_peek(0x003e) << 0x01) | (uint8_t)(tmp4_U16 >> 8);
                ram_poke(0x003e, ((uint8_t)tmp4_U16));
      /*$FF93*/ ram_poke(0x003f, (uint8_t)((ram_peek(0x003f) << 0x01) | (uint8_t)(tmp4_U16 >> 8)));
      /*$FF95*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp1_U8;
      /*$FF96*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 1649 : 1650;
      break;
    case 1650:  // $FF98
      /*$FF98*/ CYCLES(0xff98, 7);
      /*$FF9A*/ branchTarget = true; block_id = ram_peek(0x0031) ? 1652 : 1651;
      break;
    case 1651:  // $FF9C
      /*$FF9C*/ CYCLES(0xff9c, 11);
                tmp1_U8 = s_x;
                tmp6_U8 = ram_peek((uint8_t)(0x3f + tmp1_U8));
      /*$FF9E*/ ram_poke((uint8_t)(0x3d + tmp1_U8), tmp6_U8);
      /*$FFA0*/ ram_poke((uint8_t)(0x41 + tmp1_U8), tmp6_U8);
                block_id = 1652;
      break;
    case 1652:  // $FFA2
      /*$FFA2*/ CYCLES(0xffa2, 6);
                tmp1_U8 = (uint8_t)(s_x + 0x01);
                s_status_not_z = tmp1_U8;
                s_x = tmp1_U8;
      /*$FFA3*/ branchTarget = true; block_id = !tmp1_U8 ? 1650 : 1653;
      break;
    case 1653:  // $FFA5
      /*$FFA5*/ CYCLES(0xffa5, 4);
                branchTarget = true; block_id = s_status_not_z ? 1655 : 1654;
      break;
    case 1654:  // $FFA7
      /*$FFA7*/ CYCLES(0xffa7, 11);
                s_x = 0x00;
      /*$FFA9*/ ram_poke(0x003e, 0x00);
      /*$FFAB*/ ram_poke(0x003f, 0x00);
                block_id = 1655;
      break;
    case 1655:  // $FFAD
      /*$FFAD*/ CYCLES(0xffad, 18);
                tmp1_U8 = s_y;
      /*$FFB0*/ s_y = (uint8_t)(tmp1_U8 + 0x01);
      /*$FFB1*/ tmp1_U8 = ram_peek((0x0200 + tmp1_U8)) ^ 0xb0;
                s_a = tmp1_U8;
      /*$FFB3*/ tmp1_U8 = tmp1_U8 >= 0x0a;
                s_status_c = tmp1_U8;
      /*$FFB5*/ branchTarget = true; block_id = !tmp1_U8 ? 1648 : 1656;
      break;
    case 1656:  // $FFB7
      /*$FFB7*/ CYCLES(0xffb7, 11);
                tmp4_U16 = s_a;
                tmp5_U16 = (tmp4_U16 + 0x0088) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x0088);
                tmp1_U8 = (uint8_t)tmp5_U16;
                s_a = tmp1_U8;
      /*$FFB9*/ s_status_not_z = (tmp1_U8 != 0xfa);
                tmp6_U8 = tmp1_U8 >= 0xfa;
                s_status_c = tmp6_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - 0xfa) & 0x80);
      /*$FFBB*/ branchTarget = true; block_id = tmp6_U8 ? 1648 : 1657;
      break;
    case 1657:  // $FFBD
      /*$FFBD*/ CYCLES(0xffbd, 2);
                branchTarget = true; block_id = find_block_id_func_t001(0xffbd, pop16() + 1);;
      break;
    case 1658:  // $FFBE
      /*$FFBE*/ CYCLES(0xffbe, 16);
      /*$FFC0*/ push8(0xfe);
      /*$FFC1*/ tmp1_U8 = ram_peek((0xffe3 + s_y));
      /*$FFC4*/ push8(tmp1_U8);
      /*$FFC5*/ s_a = ram_peek(0x0031);
                FUNC_ZMODE(0x0000);
                tmp6_U8 = pop8();
                tmp6_U8 = pop8();
                block_id = find_block_id_func_t001(0xffc4, ((tmp1_U8 + 0xfe00) + 0x0001));
      break;
    case 1659:  // $FFC7
      /*$FFC7*/ CYCLES(0xffc7, 9);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_y = 0x00;
      /*$FFC9*/ ram_poke(0x0031, 0x00);
      /*$FFCB*/ branchTarget = true; block_id = find_block_id_func_t001(0xffcb, pop16() + 1);;
      break;
    case 1660:
      /*$E07B*/ FUNC_ISLETC(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xe07b, pop16() + 1);;
      break;
    case 1661:
      /*$E482*/ FUNC_GARBAGE(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xe482, pop16() + 1);;
      break;
    case 1662:
      /*$E517*/ FUNC_CHKSMPLVAR(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xe517, pop16() + 1);;
      break;
    case 1663:
      /*$E8C4*/ FUNC_INCFACMAN(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xe8c4, pop16() + 1);;
      break;
    case 1664:
      /*$E9B0*/ FUNC_MULT2(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xe9b0, pop16() + 1);;
      break;
    case 1665:
      /*$EBBC*/ FUNC_SIGN(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xebbc, pop16() + 1);;
      break;
    case 1666:
      /*$F454*/ FUNC_COLORSHIFT(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xf454, pop16() + 1);;
      break;
    case 1667:
      /*$FC6C*/ FUNC_VTABZ(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xfc6c, pop16() + 1);;
      break;
    case 1668:
      /*$FC3E*/ FUNC_CLREOL(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xfc3e, pop16() + 1);;
      break;
    case 1669:
      /*$FC9A*/ FUNC_CLREOL(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xfc9a, pop16() + 1);;
      break;
    default:
      fprintf(stderr, "panic: unknown block_id: %u\n", block_id);
      abort();
    }
  }
}

static const unsigned s_block_map_func_t001[] = {
    0xd392,   16, 0xd396,   18, 0xd41c,   47, 0xd41f,   48, 0xd426,   49,
    0xd42d,   51, 0xd434,   53, 0xd43c,   55, 0xd43f,   56, 0xd444,   57,
    0xd44d,   58, 0xd459,   61, 0xd467,   63, 0xd46a,   64, 0xd46f,   65,
    0xd4d6,   76, 0xd4f5,   79, 0xd533,   87, 0xd556,   93, 0xd569,   96,
    0xd649,  144, 0xd668,  147, 0xd66a,  148, 0xd683,  150, 0xd6a5,  153,
    0xd6b4,  158, 0xd6b7,  159, 0xd6ba,  160, 0xd6ca,  164, 0xd6e3,  169,
    0xd6e6,  170, 0xd6fc,  174, 0xd705,  177, 0xd70e,  179, 0xd729,  183,
    0xd755,  193, 0xd75d,  195, 0xd762,  197, 0xd766,  198, 0xd76d,  199,
    0xd770,  200, 0xd77e,  203, 0xd781,  204, 0xd796,  205, 0xd799,  206,
    0xd79c,  207, 0xd7af,  208, 0xd7b6,  209, 0xd7b9,  210, 0xd7c0,  212,
    0xd7c3,  213, 0xd7c6,  214, 0xd7c9,  215, 0xd7d8,  217, 0xd813,  226,
    0xd81a,  227, 0xd81d,  228, 0xd820,  229, 0xd823,  230, 0xd849,  239,
    0xd863,  246, 0xd870,  249, 0xd912,  258, 0xd91e,  261, 0xd921,  262,
    0xd926,  263, 0xd938,  265, 0xd93b,  266, 0xd93e,  267, 0xd941,  268,
    0xd944,  269, 0xd95c,  274, 0xd96b,  277, 0xd974,  279, 0xd995,  285,
    0xd998,  286, 0xd9c9,  297, 0xd9cc,  298, 0xd9cf,  299, 0xd9d8,  301,
    0xd9dc,  302, 0xd9df,  303, 0xd9e4,  305, 0xd9ec,  308, 0xd9ef,  309,
    0xda06,  314, 0xda43,  323, 0xda49,  325, 0xda52,  326, 0xda5b,  327,
    0xda60,  328, 0xda68,  332, 0xda6b,  333, 0xdaa8,  344, 0xdab3,  345,
    0xdabe,  347, 0xdad2,  349, 0xdad5,  350, 0xdaee,  357, 0xdaf5,  359,
    0xdaf8,  360, 0xdb00,  362, 0xdb0c,  366, 0xdb1a,  369, 0xdb32,  377,
    0xdb38,  379, 0xdb3d,  381, 0xdb40,  382, 0xdb4c,  385, 0xdb54,  387,
    0xdb67,  393, 0xdb6f,  394, 0xdb97,  402, 0xdba0,  403, 0xdba3,  404,
    0xdbb1,  405, 0xdbb2,  406, 0xdbb9,  408, 0xdbbe,  409, 0xdbc1,  410,
    0xdbc7,  412, 0xdbca,  413, 0xdbd2,  414, 0xdbdf,  417, 0xdbe2,  418,
    0xdbf4,  422, 0xdc0b,  423, 0xdc14,  426, 0xdc24,  429, 0xdc27,  430,
    0xdc2e,  432, 0xdc5a,  441, 0xdc5d,  442, 0xdc60,  443, 0xdc6d,  446,
    0xdc72,  447, 0xdc75,  448, 0xdc91,  452, 0xdc96,  454, 0xdca3,  458,
    0xdcbf,  462, 0xdcf9,  469, 0xdd02,  472, 0xdd09,  473, 0xdd22,  477,
    0xdd2f,  478, 0xdd32,  479, 0xdd37,  480, 0xdd5d,  484, 0xdd67,  486,
    0xdd6a,  487, 0xdd8e,  498, 0xdd91,  499, 0xdd98,  501, 0xddb1,  505,
    0xddd6,  514, 0xddda,  516, 0xde08,  525, 0xde23,  530, 0xde41,  535,
    0xde67,  540, 0xde6f,  543, 0xde8d,  551, 0xdeb5,  562, 0xdeb8,  563,
    0xded8,  572, 0xdefc,  578, 0xdeff,  579, 0xdf05,  580, 0xdf09,  581,
    0xdf16,  583, 0xdf1d,  585, 0xdf20,  586, 0xdf23,  587, 0xdf26,  588,
    0xdf33,  589, 0xdf3d,  591, 0xdf4c,  593, 0xdf65,  594, 0xdf68,  595,
    0xdf79,  597, 0xdf86,  599, 0xdf93,  600, 0xdfcd,  614, 0xdfd0,  615,
    0xdfd3,  616, 0xdfe8,  618, 0xdfef,  620, 0xdff2,  621, 0xe003,  624,
    0xe00a,  625, 0xe00f,  627, 0xe03d,  637, 0xe0b9,  659, 0xe108,  667,
    0xe137,  674, 0xe15f,  676, 0xe1c7,  694, 0xe201,  702, 0xe21d,  707,
    0xe27a,  721, 0xe29f,  728, 0xe2de,  737, 0xe2e5,  739, 0xe2e8,  740,
    0xe346,  747, 0xe34d,  748, 0xe357,  749, 0xe360,  750, 0xe363,  751,
    0xe383,  755, 0xe39b,  756, 0xe3e0,  760, 0xe423,  773, 0xe42a,  774,
    0xe4c0,  795, 0xe5a0,  824, 0xe5a3,  825, 0xe5ba,  828, 0xe5bd,  829,
    0xe5c4,  830, 0xe5c7,  831, 0xe5ce,  832, 0xe5d1,  833, 0xe60b,  845,
    0xe6fb,  859, 0xe6fe,  860, 0xe749,  864, 0xe74c,  865, 0xe74f,  866,
    0xe75b,  869, 0xe764,  870, 0xe76d,  871, 0xe77b,  872, 0xe77e,  873,
    0xe7bc,  877, 0xe7c1,  879, 0xe7fa,  889, 0xe82e,  895, 0xe98a,  927,
    0xea3c,  947, 0xea4b,  950, 0xea58,  953, 0xea69,  954, 0xea6e,  956,
    0xea78,  957, 0xeb2e,  979, 0xeb66,  983, 0xeb7d,  990, 0xec05, 1014,
    0xec0f, 1017, 0xec20, 1020, 0xec64, 1029, 0xecac, 1048, 0xecb5, 1051,
    0xeccb, 1058, 0xecd2, 1059, 0xecd9, 1061, 0xecdd, 1062, 0xed20, 1069,
    0xed2e, 1071, 0xed31, 1072, 0xed5e, 1080, 0xed69, 1083, 0xed74, 1086,
    0xed7b, 1089, 0xed82, 1091, 0xed89, 1093, 0xed8c, 1094, 0xeed0, 1125,
    0xf13e, 1128, 0xf16b, 1131, 0xf1bf, 1137, 0xf1c2, 1138, 0xf1d5, 1139,
    0xf1d8, 1140, 0xf1db, 1141, 0xf1ef, 1143, 0xf1fa, 1145, 0xf1fd, 1146,
    0xf20c, 1150, 0xf21d, 1153, 0xf220, 1154, 0xf225, 1156, 0xf228, 1157,
    0xf232, 1159, 0xf235, 1160, 0xf241, 1162, 0xf244, 1163, 0xf24f, 1165,
    0xf252, 1166, 0xf256, 1167, 0xf259, 1168, 0xf26d, 1170, 0xf26f, 1171,
    0xf273, 1173, 0xf277, 1174, 0xf280, 1177, 0xf286, 1178, 0xf289, 1180,
    0xf28c, 1181, 0xf2a6, 1184, 0xf2a9, 1185, 0xf2ac, 1186, 0xf2cb, 1189,
    0xf2d0, 1190, 0xf2e6, 1191, 0xf312, 1193, 0xf315, 1194, 0xf318, 1195,
    0xf390, 1196, 0xf399, 1197, 0xf3d8, 1198, 0xf3e2, 1199, 0xf405, 1202,
    0xf45a, 1212, 0xf580, 1265, 0xf5a5, 1271, 0xf63c, 1278, 0xf648, 1281,
    0xf698, 1290, 0xf6a4, 1293, 0xf6bc, 1300, 0xf6bf, 1301, 0xf6d6, 1305,
    0xf6d9, 1306, 0xf6e9, 1309, 0xf6ec, 1310, 0xf6fe, 1313, 0xf705, 1315,
    0xf708, 1316, 0xf70b, 1317, 0xf712, 1319, 0xf715, 1320, 0xf71e, 1321,
    0xf721, 1322, 0xf724, 1323, 0xf727, 1324, 0xf72a, 1325, 0xf730, 1327,
    0xf759, 1332, 0xf760, 1334, 0xf763, 1335, 0xf766, 1336, 0xf769, 1337,
    0xf76c, 1338, 0xf76f, 1339, 0xf772, 1340, 0xf7e7, 1341, 0xf7ea, 1342,
    0xf7f6, 1345, 0xf805, 1348, 0xf81c, 1352, 0xf824, 1354, 0xf82c, 1357,
    0xf843, 1361, 0xf876, 1367, 0xf889, 1372, 0xf88c, 1373, 0xf8a3, 1378,
    0xf8d3, 1388, 0xf8d9, 1390, 0xf8de, 1392, 0xf906, 1398, 0xf90c, 1400,
    0xf91e, 1404, 0xf926, 1406, 0xf930, 1410, 0xf944, 1414, 0xf94f, 1417,
    0xfa66, 1425, 0xfa69, 1426, 0xfa6c, 1427, 0xfa6f, 1428, 0xfa85, 1429,
    0xfaa9, 1435, 0xfb49, 1450, 0xfb63, 1454, 0xfba2, 1467, 0xfba5, 1468,
    0xfbe2, 1478, 0xfbe9, 1480, 0xfc27, 1495, 0xfc4a, 1505, 0xfc4d, 1506,
    0xfc58, 1508, 0xfc76, 1512, 0xfc8c, 1514, 0xfc9a, 1517, 0xfd1b, 1545,
    0xfd32, 1550, 0xfd35, 1551, 0xfd38, 1552, 0xfd4a, 1555, 0xfd5f, 1559,
    0xfd67, 1561, 0xfd6a, 1562, 0xfd6f, 1563, 0xfd78, 1567, 0xfd8e, 1573,
    0xfd99, 1576, 0xfd9c, 1577, 0xfdb6, 1581, 0xfdbb, 1582, 0xfdc0, 1583,
    0xfdd9, 1589, 0xfde2, 1591, 0xfdf0, 1595, 0xfdfc, 1598, 0xfe5e, 1606,
    0xfe61, 1607, 0xfe67, 1609, 0xfe6a, 1610, 0xfeb6, 1622, 0xfeb9, 1623,
    0xfebc, 1624, 0xfef6, 1625, 0xfef9, 1626, 0xff19, 1631, 0xff29, 1633,
    0xff32, 1635, 0xff37, 1636, 0xff3a, 1637, 0xff69, 1640, 0xff70, 1641,
    0xff73, 1642, 0xff76, 1643, 0xff85, 1647
};

static unsigned find_block_id_func_t001(uint16_t from_pc, uint16_t addr) {
  return addr_to_block_id(from_pc, addr, s_block_map_func_t001, sizeof(s_block_map_func_t001) / (sizeof(unsigned) * 2));
};
static const uint8_t s_mem_0090[0x0003] = {
  0x4C, 0xCD, 0x00
};
static const uint8_t s_mem_00b1[0x0018] = {
  0xE6, 0xB8, 0xD0, 0x02, 0xE6, 0xB9, 0xAD, 0x00, 0x02, 0xC9, 0x3A, 0xB0, 0x0A, 0xC9, 0x20, 0xF0,
  0xEF, 0x38, 0xE9, 0x30, 0x38, 0xE9, 0xD0, 0x60
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
