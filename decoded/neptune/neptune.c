// Loaded binary at [$0000..$BFFF]
// --code-at: 1 asserted edges applied
// 29 new runtime blocks added
// code labels: 1085
// data labels: 271

#include "apple2tc/system2-inc.h"

static const uint8_t s_mem_0000[0xc000];
static const uint8_t s_mem_d000[0x3000];

void init_emulated(void) {
  memcpy(s_ram + 0x0000, s_mem_0000, 0xc000);
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
void rom_mon_wait(uint16_t ret_addr);
void func_1000(uint16_t ret_addr);
void func_1030(uint16_t ret_addr);
void func_1240(uint16_t ret_addr);
void func_1490(uint16_t ret_addr);
void func_1500(uint16_t ret_addr);
void func_1530(uint16_t ret_addr);
void func_1560(uint16_t ret_addr);
void func_1640(uint16_t ret_addr);
void func_1738(uint16_t ret_addr);
void func_18e4(uint16_t ret_addr);
void func_1af0(uint16_t ret_addr);
void func_1d80(uint16_t ret_addr);
void func_1da0(uint16_t ret_addr);
void func_1db0(uint16_t ret_addr);
void func_1dc0(uint16_t ret_addr);
void func_1dd0(uint16_t ret_addr);
void func_1e00(uint16_t ret_addr);
void func_1e40(uint16_t ret_addr);
void func_1e80(uint16_t ret_addr);
void func_1ef3(uint16_t ret_addr);
void func_1fd0(uint16_t ret_addr);
void func_2200(uint16_t ret_addr);
void func_2240(uint16_t ret_addr);
void func_2380(uint16_t ret_addr);
void func_2400(uint16_t ret_addr);
void func_24a0(uint16_t ret_addr);
void func_2500(uint16_t ret_addr);
void func_2520(uint16_t ret_addr);
void func_2550(uint16_t ret_addr);
void func_2590(uint16_t ret_addr);
void func_2600(uint16_t ret_addr);
void func_2680(uint16_t ret_addr);
void func_26c0(uint16_t ret_addr);
void func_2745(uint16_t ret_addr);
void func_2750(uint16_t ret_addr);
void func_2770(uint16_t ret_addr);
void func_2790(uint16_t ret_addr);
void func_2800(uint16_t ret_addr);
void func_2880(uint16_t ret_addr);
void func_2900(uint16_t ret_addr);
void func_2a00(uint16_t ret_addr);
void func_2a20(uint16_t ret_addr);
void func_2aa0(uint16_t ret_addr);
void func_2b30(uint16_t ret_addr);
void func_2b60(uint16_t ret_addr);
void func_2b70(uint16_t ret_addr);
void func_2c40(uint16_t ret_addr);
void func_2c80(uint16_t ret_addr);
void func_2cb0(uint16_t ret_addr);
void func_2ce0(uint16_t ret_addr);
void func_2ce8(uint16_t ret_addr);
void func_2d00(uint16_t ret_addr);
void func_2d38(uint16_t ret_addr);
void func_2d40(uint16_t ret_addr);
void func_2d80(uint16_t ret_addr);
void func_2d90(uint16_t ret_addr);
void func_2e00(uint16_t ret_addr);
void func_2f00(uint16_t ret_addr);
void func_2f80(uint16_t ret_addr);
void func_2fa0(uint16_t ret_addr);
void func_2fd3(uint16_t ret_addr);
void func_3000(uint16_t ret_addr);
void func_3040(uint16_t ret_addr);
void func_3070(uint16_t ret_addr);
void func_3440(uint16_t ret_addr);
void func_3480(uint16_t ret_addr);
void delay_one_tick(uint16_t ret_addr);
void poll_input_select_mode(uint16_t ret_addr);
void func_3600(uint16_t ret_addr);
void func_3630(uint16_t ret_addr);
void func_3650(uint16_t ret_addr);
void func_37a0(uint16_t ret_addr);
void func_3800(uint16_t ret_addr);
void func_3860(uint16_t ret_addr);
void func_ac00(uint16_t ret_addr);
void func_ae00(uint16_t ret_addr);
void func_ae58(uint16_t ret_addr);
void func_ae80(uint16_t ret_addr);
void func_aec0(uint16_t ret_addr);
void func_b9a0(uint16_t ret_addr);
void func_ba00(uint16_t ret_addr);
void func_bad8(uint16_t ret_addr);
void func_baf0(uint16_t ret_addr);
void FUNC_SAV1(uint16_t ret_addr);

void emulated_entry_point(void) {
  func_t001(false);
}

void func_1000(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1000*/ CYCLES(0x1000, 3);
            s_x = ram_peek(0x0004);
bb_1:
  /*$1002*/ CYCLES(0x1002, 17);
            tmp1_U8 = s_x;
  /*$1005*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$100A*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$100C*/ s_y = ram_peek(0x0006);
bb_2:
  /*$100E*/ CYCLES(0x100e, 6);
            tmp1_U8 = peek(ram_peek16(0x100f));
            s_a = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_4;
  /*$1011*/ CYCLES_EDGE(0x1011, 1);
            branchTarget = true;
            goto bb_9;
bb_4:
  /*$1013*/ CYCLES(0x1013, 14);
            tmp1_U8 = s_a;
            push8(tmp1_U8);
  /*$1016*/ tmp2_U8 = peek((ram_peek16al(0x0000) + s_y));
  /*$1018*/ tmp3_U16 = ((tmp1_U8 ^ 0xff) & tmp2_U8) << 0x01;
            s_status_c = (uint8_t)(tmp3_U16 >> 8);
            branchTarget = true;
            if (((uint8_t)tmp3_U16)) {
    /*$1019*/ CYCLES_EDGE(0x1019, 1);
    /*$105C*/ CYCLES(0x105c, 15);
              tmp1_U8 = pop8();
    /*$105D*/ ram_poke(0x0008, (uint8_t)(ram_peek(0x0008) + 0x01));
    /*$105F*/ if (ret_addr) pop16(); return;
            }
  /*$101B*/ CYCLES(0x101b, 8);
            tmp1_U8 = pop8();
            s_a = tmp1_U8;
            branchTarget = true;
            if (!(s_y >= 0x28))
              goto bb_8;
  /*$101E*/ CYCLES_EDGE(0x101e, 1);
            branchTarget = true;
            goto bb_9;
bb_8:
  /*$1020*/ CYCLES(0x1020, 6);
            poke((ram_peek16al(0x0000) + s_y), s_a);
bb_9:
  /*$1022*/ CYCLES(0x1022, 13);
            ram_poke(0x100f, (uint8_t)(ram_peek(0x100f) + 0x01));
  /*$1025*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= ram_peek(0x0007)))
              goto bb_11;
  /*$1028*/ CYCLES_EDGE(0x1028, 1);
            branchTarget = true;
            goto bb_2;
bb_11:
  /*$102A*/ CYCLES(0x102a, 7);
            tmp1_U8 = (uint8_t)(s_x + 0x01);
            s_x = tmp1_U8;
  /*$102B*/ tmp1_U8 = tmp1_U8 >= ram_peek(0x0005);
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_13;
  /*$102D*/ CYCLES_EDGE(0x102d, 1);
            branchTarget = true;
            goto bb_1;
bb_13:
  /*$102F*/ CYCLES(0x102f, 6);
            if (ret_addr) pop16(); return;
}


void func_1030(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1030*/ CYCLES(0x1030, 3);
            s_x = ram_peek(0x0004);
bb_1:
  /*$1032*/ CYCLES(0x1032, 17);
            tmp1_U8 = s_x;
  /*$1035*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$103A*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$103C*/ s_y = ram_peek(0x0006);
bb_2:
  /*$103E*/ CYCLES(0x103e, 6);
            tmp1_U8 = peek(ram_peek16(0x103f));
            s_a = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_4;
  /*$1041*/ CYCLES_EDGE(0x1041, 1);
            branchTarget = true;
            goto bb_7;
bb_4:
  /*$1043*/ CYCLES(0x1043, 6);
            s_a = (s_a ^ 0x7f);
            branchTarget = true;
            if (!(s_y >= 0x28))
              goto bb_6;
  /*$1047*/ CYCLES_EDGE(0x1047, 1);
            branchTarget = true;
            goto bb_7;
bb_6:
  /*$1049*/ CYCLES(0x1049, 11);
            tmp1_U8 = s_y;
            tmp2_U8 = peek((ram_peek16al(0x0000) + tmp1_U8));
            tmp2_U8 = s_a & tmp2_U8;
            s_a = tmp2_U8;
  /*$104B*/ poke((ram_peek16al(0x0000) + tmp1_U8), tmp2_U8);
bb_7:
  /*$104D*/ CYCLES(0x104d, 13);
            ram_poke(0x103f, (uint8_t)(ram_peek(0x103f) + 0x01));
  /*$1050*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= ram_peek(0x0007)))
              goto bb_9;
  /*$1053*/ CYCLES_EDGE(0x1053, 1);
            branchTarget = true;
            goto bb_2;
bb_9:
  /*$1055*/ CYCLES(0x1055, 7);
            tmp1_U8 = (uint8_t)(s_x + 0x01);
            s_x = tmp1_U8;
  /*$1056*/ tmp1_U8 = tmp1_U8 >= ram_peek(0x0005);
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_11;
  /*$1058*/ CYCLES_EDGE(0x1058, 1);
            branchTarget = true;
            goto bb_1;
bb_11:
  /*$105A*/ CYCLES(0x105a, 6);
            if (ret_addr) pop16(); return;
}


void func_1240(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1240*/ CYCLES(0x1240, 56);
  /*$1246*/ ram_poke(0x103f, ram_peek((0x1230 + (uint8_t)(ram_peek(0x00d0) - 0x01))));
  /*$1249*/ s_a = ram_peek(0x00d3);
  /*$124B*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_2;
  /*$124C*/ s_a = (uint8_t)((s_a + 0x0070) + s_status_c);
            goto bb_3;
bb_2:
  /*$124C*/ s_a = ((uint8_t)adc_dec16(s_a, 0x70, s_status_c));
bb_3:
  /*$124E*/ ram_poke(0x1040, s_a);
  /*$1251*/ tmp1_U8 = ram_peek(0x00d1);
            s_a = tmp1_U8;
  /*$1253*/ ram_poke(0x0004, tmp1_U8);
  /*$1255*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_5;
  /*$1256*/ s_a = (uint8_t)((s_a + 0x0009) + s_status_c);
            goto bb_6;
bb_5:
  /*$1256*/ s_a = ((uint8_t)adc_dec16(s_a, 0x09, s_status_c));
bb_6:
  /*$1258*/ ram_poke(0x0005, s_a);
  /*$125A*/ tmp1_U8 = ram_peek(0x00d2);
            s_a = tmp1_U8;
  /*$125C*/ ram_poke(0x0006, tmp1_U8);
  /*$125E*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_8;
  /*$125F*/ s_a = (uint8_t)((s_a + 0x0003) + s_status_c);
            goto bb_9;
bb_8:
  /*$125F*/ s_a = ((uint8_t)adc_dec16(s_a, 0x03, s_status_c));
bb_9:
  /*$1261*/ ram_poke(0x0007, s_a);
  /*$1263*/ func_1030(0x1265);
  /*$1266*/ CYCLES(0x1266, 12);
            ram_poke(0x00d0, (uint8_t)(ram_peek(0x00d0) + 0x01));
  /*$1268*/ tmp1_U8 = ram_peek(0x00d0);
            s_a = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0x10))
              goto bb_11;
  /*$126C*/ CYCLES_EDGE(0x126c, 1);
            branchTarget = true;
            goto bb_12;
bb_11:
  /*$126E*/ CYCLES(0x126e, 5);
            s_a = 0x10;
  /*$1270*/ ram_poke(0x00d0, 0x10);
bb_12:
  /*$1272*/ CYCLES(0x1272, 15);
  /*$1273*/ s_x = (uint8_t)(s_a - 0x01);
  /*$1274*/ s_a = ram_peek(0x00d3);
  /*$1276*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_14;
  /*$1277*/ tmp1_U8 = (uint8_t)((s_a - ram_peek((0x1200 + s_x))) - (uint8_t)(0x01 - s_status_c));
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            goto bb_15;
bb_14:
  /*$1277*/ tmp3_U16 = sbc_dec16(s_a, ram_peek((0x1200 + s_x)), s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_n = ((uint8_t)(tmp3_U16 >> 8) & 0x80);
bb_15:
            branchTarget = true;
            if (s_status_n)
              goto bb_17;
  /*$127A*/ CYCLES_EDGE(0x127a, 1);
            branchTarget = true;
            goto bb_21;
bb_17:
  /*$127C*/ CYCLES(0x127c, 13);
            ram_poke(0x00d2, (uint8_t)(ram_peek(0x00d2) - 0x01));
  /*$1280*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_19;
  /*$1281*/ s_a = (uint8_t)((s_a + 0x0007) + s_status_c);
            goto bb_20;
bb_19:
  /*$1281*/ s_a = ((uint8_t)adc_dec16(s_a, 0x07, s_status_c));
bb_20:
bb_21:
  /*$1283*/ CYCLES(0x1283, 16);
            ram_poke(0x00d3, s_a);
  /*$1285*/ s_a = ram_peek(0x00d1);
  /*$1287*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_23;
  /*$1288*/ tmp2_U16 = s_a;
            tmp3_U16 = ram_peek((0x1210 + s_x));
            tmp4_U16 = (tmp2_U16 + tmp3_U16) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp2_U16, (uint8_t)tmp3_U16);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_24;
bb_23:
  /*$1288*/ tmp3_U16 = adc_dec16(s_a, ram_peek((0x1210 + s_x)), s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_24:
  /*$128B*/ tmp1_U8 = s_a >= 0xc0;
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_26;
  /*$128D*/ CYCLES_EDGE(0x128d, 1);
  /*$1294*/ CYCLES(0x1294, 59);
            ram_poke(0x00d1, s_a);
  /*$1299*/ ram_poke(0x100f, ram_peek((0x1230 + s_x)));
  /*$129C*/ s_a = ram_peek(0x00d3);
  /*$129E*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_28;
            else
              goto bb_27;
bb_26:
  /*$128F*/ CYCLES(0x128f, 11);
            s_a = 0x00;
  /*$1291*/ ram_poke(0x00d0, 0x00);
  /*$1293*/ if (ret_addr) pop16(); return;
bb_27:
  /*$129F*/ s_a = (uint8_t)((s_a + 0x0070) + s_status_c);
            goto bb_29;
bb_28:
  /*$129F*/ s_a = ((uint8_t)adc_dec16(s_a, 0x70, s_status_c));
bb_29:
  /*$12A1*/ ram_poke(0x1010, s_a);
  /*$12A4*/ tmp1_U8 = ram_peek(0x00d1);
            s_a = tmp1_U8;
  /*$12A6*/ ram_poke(0x0004, tmp1_U8);
  /*$12A8*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_31;
  /*$12A9*/ s_a = (uint8_t)((s_a + 0x0009) + s_status_c);
            goto bb_32;
bb_31:
  /*$12A9*/ s_a = ((uint8_t)adc_dec16(s_a, 0x09, s_status_c));
bb_32:
  /*$12AB*/ ram_poke(0x0005, s_a);
  /*$12AD*/ tmp1_U8 = ram_peek(0x00d2);
            s_a = tmp1_U8;
  /*$12AF*/ ram_poke(0x0006, tmp1_U8);
  /*$12B1*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_34;
  /*$12B2*/ tmp4_U16 = s_a;
            tmp3_U16 = (tmp4_U16 + 0x0003) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0x0003);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_35;
bb_34:
  /*$12B2*/ tmp3_U16 = adc_dec16(s_a, 0x03, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_35:
  /*$12B4*/ ram_poke(0x0007, s_a);
  /*$12B8*/ ram_poke(0x0008, 0x00);
  /*$12BA*/ func_1000(0x12bc);
  /*$12BD*/ CYCLES(0x12bd, 5);
            branchTarget = true;
            if (ram_peek(0x0008))
              goto bb_37;
  /*$12BF*/ CYCLES_EDGE(0x12bf, 1);
  /*$1330*/ CYCLES(0x1330, 20);
  /*$1332*/ tmp1_U8 = (uint8_t)(ram_peek(0x00d0) - 0x01);
  /*$1336*/ ram_poke(0x00d4, ram_peek((0x1220 + tmp1_U8)));
  /*$1338*/ tmp1_U8 = ram_peek((0x1200 + tmp1_U8));
            s_a = tmp1_U8;
  /*$133C*/ s_x = (uint8_t)(tmp1_U8 + 0x01);
            goto bb_41;
bb_37:
  /*$12C1*/ CYCLES(0x12c1, 38);
  /*$12C2*/ ram_poke(0x0005, s_x);
  /*$12C4*/ ram_poke(0x0008, s_y);
  /*$12C8*/ tmp1_U8 = (uint8_t)(ram_peek(0x00d0) - 0x01);
            s_x = tmp1_U8;
  /*$12CC*/ ram_poke(0x103f, ram_peek((0x1230 + tmp1_U8)));
  /*$12CF*/ s_a = ram_peek(0x00d3);
  /*$12D1*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_39;
  /*$12D2*/ tmp3_U16 = s_a;
            tmp4_U16 = (tmp3_U16 + 0x0070) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x0070);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_40;
bb_39:
  /*$12D2*/ tmp3_U16 = adc_dec16(s_a, 0x70, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_40:
  /*$12D4*/ ram_poke(0x1040, s_a);
  /*$12D7*/ func_1030(0x12d9);
  /*$12DA*/ CYCLES(0x12da, 23);
  /*$12DC*/ ram_poke(0x0004, ram_peek(0x0005));
  /*$12DE*/ ram_poke(0x0005, (uint8_t)(ram_peek(0x0005) + 0x01));
  /*$12E2*/ ram_poke(0x0007, ram_peek(0x0008));
  /*$12E4*/ func_1030(0x12e6);
  /*$12E7*/ CYCLES(0x12e7, 13);
  /*$12E9*/ ram_poke(0x00d0, 0x00);
  /*$12EB*/ ram_poke(0x0007, (uint8_t)(ram_peek(0x0007) + 0x01));
  /*$1860*/ CYCLES(0x1860, 5);
            tmp1_U8 = ram_peek(0x00f5);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_50;
            else
              goto bb_51;
bb_41:
  /*$133D*/ CYCLES(0x133d, 2);
            s_y = 0x30;
bb_42:
  /*$133F*/ CYCLES(0x133f, 4);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_44;
  /*$1340*/ CYCLES_EDGE(0x1340, 1);
            branchTarget = true;
            goto bb_42;
bb_44:
  /*$1342*/ CYCLES(0x1342, 6);
            poke(ram_peek16(0x1343), s_a);
  /*$1345*/ s_y = 0x10;
bb_45:
  /*$1347*/ CYCLES(0x1347, 4);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_47;
  /*$1348*/ CYCLES_EDGE(0x1348, 1);
            branchTarget = true;
            goto bb_45;
bb_47:
  /*$134A*/ CYCLES(0x134a, 8);
            poke(ram_peek16(0x134b), s_a);
  /*$134D*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_49;
  /*$134E*/ CYCLES_EDGE(0x134e, 1);
            branchTarget = true;
            goto bb_41;
bb_49:
  /*$1350*/ CYCLES(0x1350, 6);
            if (ret_addr) pop16(); return;
bb_50:
  /*$1862*/ CYCLES_EDGE(0x1862, 1);
  /*$1880*/ CYCLES(0x1880, 10);
  /*$1882*/ ram_poke(0x0008, 0x00);
  /*$1886*/ ram_poke(0x0009, 0x03);
            goto bb_55;
bb_51:
  /*$1864*/ CYCLES(0x1864, 4);
            tmp1_U8 = s_a;
            s_status_c = (tmp1_U8 >= 0x01);
            branchTarget = true;
            if (!(tmp1_U8 != 0x01))
              goto bb_53;
  /*$1866*/ CYCLES_EDGE(0x1866, 1);
            branchTarget = true;
            goto bb_54;
bb_53:
  /*$1868*/ CYCLES(0x1868, 6);
            func_18e4(0x186a);
            branchTarget = true;
bb_54:
  /*$186B*/ CYCLES(0x186b, 6);
            if (ret_addr) pop16(); return;
bb_55:
  /*$1888*/ CYCLES(0x1888, 11);
            s_y = 0x00;
  /*$188A*/ tmp1_U8 = peek(ram_peek16al(0x0008));
            branchTarget = true;
            if ((tmp1_U8 & 0x01))
              goto bb_57;
  /*$188D*/ CYCLES_EDGE(0x188d, 1);
            branchTarget = true;
            goto bb_81;
bb_57:
  /*$188F*/ CYCLES(0x188f, 12);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$1890*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 >= ram_peek(0x0005)))
              goto bb_59;
  /*$1894*/ CYCLES_EDGE(0x1894, 1);
            branchTarget = true;
            goto bb_81;
bb_59:
  /*$1896*/ CYCLES(0x1896, 9);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_61;
  /*$1897*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
            goto bb_62;
bb_61:
  /*$1897*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
bb_62:
            branchTarget = true;
            if ((s_a >= ram_peek(0x0005)))
              goto bb_64;
  /*$189B*/ CYCLES_EDGE(0x189b, 1);
            branchTarget = true;
            goto bb_81;
bb_64:
  /*$189D*/ CYCLES(0x189d, 12);
  /*$189E*/ tmp1_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(s_y + 0x01)));
            s_a = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 >= ram_peek(0x0007)))
              goto bb_66;
  /*$18A2*/ CYCLES_EDGE(0x18a2, 1);
            branchTarget = true;
            goto bb_81;
bb_66:
  /*$18A4*/ CYCLES(0x18a4, 9);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_68;
  /*$18A5*/ s_a = (uint8_t)((s_a + 0x0003) + s_status_c);
            goto bb_69;
bb_68:
  /*$18A5*/ s_a = ((uint8_t)adc_dec16(s_a, 0x03, s_status_c));
bb_69:
            branchTarget = true;
            if ((s_a >= ram_peek(0x0007)))
              goto bb_71;
  /*$18A9*/ CYCLES_EDGE(0x18a9, 1);
            branchTarget = true;
            goto bb_81;
bb_71:
  /*$18AB*/ CYCLES(0x18ab, 68);
  /*$18AF*/ poke(ram_peek16al(0x0008), 0x02);
  /*$18B1*/ s_y = 0x01;
  /*$18B2*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0001));
            s_a = tmp1_U8;
  /*$18B4*/ ram_poke(0x0004, tmp1_U8);
  /*$18B6*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_73;
  /*$18B7*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
            goto bb_74;
bb_73:
  /*$18B7*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
bb_74:
  /*$18B9*/ ram_poke(0x0005, s_a);
  /*$18BB*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$18BC*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
  /*$18BE*/ ram_poke(0x0006, tmp1_U8);
  /*$18C0*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_76;
  /*$18C1*/ s_a = (uint8_t)((s_a + 0x0002) + s_status_c);
            goto bb_77;
bb_76:
  /*$18C1*/ s_a = ((uint8_t)adc_dec16(s_a, 0x02, s_status_c));
bb_77:
  /*$18C3*/ ram_poke(0x0007, s_a);
  /*$18C6*/ tmp1_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(s_y + 0x01)));
            s_a = tmp1_U8;
  /*$18C8*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_79;
  /*$18C9*/ s_a = (uint8_t)((s_a + 0x0067) + s_status_c);
            goto bb_80;
bb_79:
  /*$18C9*/ s_a = ((uint8_t)adc_dec16(s_a, 0x67, s_status_c));
bb_80:
  /*$18CB*/ ram_poke(0x1544, s_a);
  /*$18D0*/ ram_poke(0x1543, 0xc0);
  /*$1A90*/ CYCLES(0x1a90, 16);
  /*$1A94*/ poke((ram_peek16al(0x0008) + 0x0003), 0x00);
  /*$1A96*/ func_1530(0x1a98);
  /*$1A99*/ CYCLES(0x1a99, 7);
            s_x = 0x20;
  /*$1A9B*/ s_y = 0x00;
  /*$1A9D*/ func_2e00(0x0000);
            if (ret_addr) pop16(); return;
bb_81:
  /*$18D9*/ CYCLES(0x18d9, 14);
            s_a = ram_peek(0x0008);
  /*$18DB*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_83;
  /*$18DC*/ tmp4_U16 = s_a;
            tmp3_U16 = (tmp4_U16 + 0x0004) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0x0004);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_84;
bb_83:
  /*$18DC*/ tmp3_U16 = adc_dec16(s_a, 0x04, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_84:
  /*$18DE*/ tmp1_U8 = s_a;
            ram_poke(0x0008, tmp1_U8);
            branchTarget = true;
            if ((tmp1_U8 >= 0x10))
              goto bb_86;
  /*$18E2*/ CYCLES_EDGE(0x18e2, 1);
            branchTarget = true;
            goto bb_55;
bb_86:
  /*$18E2*/ func_18e4(0x0000);
            if (ret_addr) pop16(); return;
}


void func_1490(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1490*/ CYCLES(0x1490, 10);
            tmp1_U8 = s_y;
            ram_poke(0x0007, tmp1_U8);
  /*$1492*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 != ram_peek(0x00c3)))
              goto bb_2;
  /*$1495*/ CYCLES_EDGE(0x1495, 1);
  /*$149C*/ CYCLES(0x149c, 5);
            branchTarget = true;
            if ((s_y != ram_peek(0x00c7)))
              goto bb_3;
            else
              goto bb_4;
bb_2:
  /*$1497*/ CYCLES(0x1497, 6);
            s_x = ram_peek(0x00bd);
            branchTarget = true;
            goto bb_5;
bb_3:
  /*$149E*/ CYCLES_EDGE(0x149e, 1);
  /*$14A5*/ CYCLES(0x14a5, 3);
            s_x = ram_peek(0x00bf);
            goto bb_5;
bb_4:
  /*$14A0*/ CYCLES(0x14a0, 6);
            s_x = ram_peek(0x00be);
            branchTarget = true;
bb_5:
  /*$14A7*/ CYCLES(0x14a7, 18);
            ram_poke(0x0005, s_x);
  /*$14A9*/ ram_poke(0x0005, (uint8_t)(ram_peek(0x0005) + 0x01));
  /*$14AB*/ ram_poke(0x0007, (uint8_t)(ram_peek(0x0007) + 0x01));
  /*$14AD*/ tmp1_U8 = ram_peek(0x00f5);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_7;
  /*$14AF*/ CYCLES_EDGE(0x14af, 1);
  /*$14B4*/ CYCLES(0x14b4, 4);
            tmp1_U8 = s_a;
            s_status_c = (tmp1_U8 >= 0x01);
            branchTarget = true;
            if ((tmp1_U8 != 0x01))
              goto bb_8;
            else
              goto bb_9;
bb_7:
  /*$14B1*/ CYCLES(0x14b1, 3);
  /*$17E0*/ CYCLES(0x17e0, 7);
            branchTarget = true;
            if ((ram_peek(0x0070) != 0x02))
              goto bb_10;
            else
              goto bb_11;
bb_8:
  /*$14B6*/ CYCLES_EDGE(0x14b6, 1);
  /*$14BB*/ CYCLES(0x14bb, 6);
            if (ret_addr) pop16(); return;
bb_9:
  /*$14B8*/ CYCLES(0x14b8, 3);
  /*$1C50*/ CYCLES(0x1c50, 10);
  /*$1C52*/ ram_poke(0x0008, 0x00);
  /*$1C56*/ ram_poke(0x0009, 0x03);
            goto bb_91;
bb_10:
  /*$17E4*/ CYCLES_EDGE(0x17e4, 1);
            branchTarget = true;
            goto bb_24;
bb_11:
  /*$17E6*/ CYCLES(0x17e6, 12);
            s_a = ram_peek(0x0071);
  /*$17E8*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_13;
  /*$17E9*/ s_a = (uint8_t)((s_a + 0x0008) + s_status_c);
            goto bb_14;
bb_13:
  /*$17E9*/ s_a = ((uint8_t)adc_dec16(s_a, 0x08, s_status_c));
bb_14:
            branchTarget = true;
            if (!(s_a != ram_peek(0x0005)))
              goto bb_16;
  /*$17ED*/ CYCLES_EDGE(0x17ed, 1);
            branchTarget = true;
            goto bb_24;
bb_16:
  /*$17EF*/ CYCLES(0x17ef, 8);
            tmp1_U8 = ram_peek(0x0072);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 >= ram_peek(0x0007)))
              goto bb_18;
  /*$17F3*/ CYCLES_EDGE(0x17f3, 1);
            branchTarget = true;
            goto bb_24;
bb_18:
  /*$17F5*/ CYCLES(0x17f5, 9);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_20;
  /*$17F6*/ s_a = (uint8_t)((s_a + 0x0008) + s_status_c);
            goto bb_21;
bb_20:
  /*$17F6*/ s_a = ((uint8_t)adc_dec16(s_a, 0x08, s_status_c));
bb_21:
            branchTarget = true;
            if ((s_a >= ram_peek(0x0007)))
              goto bb_23;
  /*$17FA*/ CYCLES_EDGE(0x17fa, 1);
            branchTarget = true;
            goto bb_24;
bb_23:
  /*$17FC*/ CYCLES(0x17fc, 3);
  /*$1803*/ CYCLES(0x1803, 17);
            ram_poke(0x0070, (uint8_t)(ram_peek(0x0070) + 0x01));
  /*$1805*/ ram_poke(0x0077, (uint8_t)(ram_peek(0x0077) + 0x01));
  /*$1807*/ s_x = 0x00;
  /*$1809*/ s_y = 0x05;
  /*$180B*/ func_2e00(0x0000);
            if (ret_addr) pop16(); return;
bb_24:
  /*$1800*/ CYCLES(0x1800, 3);
  /*$1950*/ CYCLES(0x1950, 10);
  /*$1952*/ ram_poke(0x0008, 0x00);
  /*$1956*/ ram_poke(0x0009, 0x03);
            goto bb_31;
bb_25:
  /*$1943*/ tmp3_U16 = s_a;
            tmp2_U16 = (tmp3_U16 + 0x0050) + s_status_c;
            s_status_c = (uint8_t)(tmp2_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp2_U16, (uint8_t)tmp3_U16, (uint8_t)0x0050);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_27;
bb_26:
  /*$1943*/ tmp2_U16 = adc_dec16(s_a, 0x50, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            tmp1_U8 = (uint8_t)(tmp2_U16 >> 8);
            s_status_c = (tmp1_U8 & 0x01);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
bb_27:
            branchTarget = true;
            if (s_status_c)
              goto bb_29;
  /*$1945*/ CYCLES_EDGE(0x1945, 1);
            branchTarget = true;
            goto bb_30;
bb_29:
  /*$1947*/ CYCLES(0x1947, 2);
            s_a = 0xff;
bb_30:
  /*$1949*/ CYCLES(0x1949, 9);
            ram_poke(0x0089, s_a);
  /*$194B*/ if (ret_addr) pop16(); return;
bb_31:
  /*$1958*/ CYCLES(0x1958, 11);
            s_y = 0x00;
  /*$195A*/ tmp1_U8 = peek(ram_peek16al(0x0008));
            branchTarget = true;
            if ((tmp1_U8 & 0x01))
              goto bb_33;
  /*$195D*/ CYCLES_EDGE(0x195d, 1);
            branchTarget = true;
            goto bb_57;
bb_33:
  /*$195F*/ CYCLES(0x195f, 12);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$1960*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 >= ram_peek(0x0005)))
              goto bb_35;
  /*$1964*/ CYCLES_EDGE(0x1964, 1);
            branchTarget = true;
            goto bb_57;
bb_35:
  /*$1966*/ CYCLES(0x1966, 9);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_37;
  /*$1967*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
            goto bb_38;
bb_37:
  /*$1967*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
bb_38:
            branchTarget = true;
            if ((s_a >= ram_peek(0x0005)))
              goto bb_40;
  /*$196B*/ CYCLES_EDGE(0x196b, 1);
            branchTarget = true;
            goto bb_57;
bb_40:
  /*$196D*/ CYCLES(0x196d, 12);
  /*$196E*/ tmp1_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(s_y + 0x01)));
            s_a = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 >= ram_peek(0x0007)))
              goto bb_42;
  /*$1972*/ CYCLES_EDGE(0x1972, 1);
            branchTarget = true;
            goto bb_57;
bb_42:
  /*$1974*/ CYCLES(0x1974, 9);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_44;
  /*$1975*/ s_a = (uint8_t)((s_a + 0x0003) + s_status_c);
            goto bb_45;
bb_44:
  /*$1975*/ s_a = ((uint8_t)adc_dec16(s_a, 0x03, s_status_c));
bb_45:
            branchTarget = true;
            if ((s_a >= ram_peek(0x0007)))
              goto bb_47;
  /*$1979*/ CYCLES_EDGE(0x1979, 1);
            branchTarget = true;
            goto bb_57;
bb_47:
  /*$197B*/ CYCLES(0x197b, 68);
  /*$197F*/ poke(ram_peek16al(0x0008), 0x02);
  /*$1981*/ s_y = 0x01;
  /*$1982*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0001));
            s_a = tmp1_U8;
  /*$1984*/ ram_poke(0x0004, tmp1_U8);
  /*$1986*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_49;
  /*$1987*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
            goto bb_50;
bb_49:
  /*$1987*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
bb_50:
  /*$1989*/ ram_poke(0x0005, s_a);
  /*$198B*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$198C*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
  /*$198E*/ ram_poke(0x0006, tmp1_U8);
  /*$1990*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_52;
  /*$1991*/ s_a = (uint8_t)((s_a + 0x0002) + s_status_c);
            goto bb_53;
bb_52:
  /*$1991*/ s_a = ((uint8_t)adc_dec16(s_a, 0x02, s_status_c));
bb_53:
  /*$1993*/ ram_poke(0x0007, s_a);
  /*$1996*/ tmp1_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(s_y + 0x01)));
            s_a = tmp1_U8;
  /*$1998*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_55;
  /*$1999*/ s_a = (uint8_t)((s_a + 0x0067) + s_status_c);
            goto bb_56;
bb_55:
  /*$1999*/ s_a = ((uint8_t)adc_dec16(s_a, 0x67, s_status_c));
bb_56:
  /*$199B*/ ram_poke(0x1544, s_a);
  /*$19A0*/ ram_poke(0x1543, 0xc0);
  /*$1A90*/ CYCLES(0x1a90, 16);
  /*$1A94*/ poke((ram_peek16al(0x0008) + 0x0003), 0x00);
  /*$1A96*/ func_1530(0x1a98);
  /*$1A99*/ CYCLES(0x1a99, 7);
            s_x = 0x20;
  /*$1A9B*/ s_y = 0x00;
  /*$1A9D*/ func_2e00(0x0000);
            if (ret_addr) pop16(); return;
bb_57:
  /*$19A9*/ CYCLES(0x19a9, 14);
            s_a = ram_peek(0x0008);
  /*$19AB*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_59;
  /*$19AC*/ tmp2_U16 = s_a;
            tmp3_U16 = (tmp2_U16 + 0x0004) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0x0004);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_60;
bb_59:
  /*$19AC*/ tmp2_U16 = adc_dec16(s_a, 0x04, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            s_status_v = (((uint8_t)(tmp2_U16 >> 8) & 0x40) != 0);
bb_60:
  /*$19AE*/ tmp1_U8 = s_a;
            ram_poke(0x0008, tmp1_U8);
            branchTarget = true;
            if ((tmp1_U8 >= 0x10))
              goto bb_62;
  /*$19B2*/ CYCLES_EDGE(0x19b2, 1);
            branchTarget = true;
            goto bb_31;
bb_62:
  /*$19B4*/ CYCLES(0x19b4, 21);
  /*$19B6*/ ram_poke(0x0008, 0x20);
  /*$19BA*/ ram_poke(0x0009, 0x03);
  /*$19BC*/ s_y = 0x00;
  /*$19BE*/ tmp1_U8 = peek(ram_peek16al(0x0008));
  /*$19C0*/ tmp1_U8 = tmp1_U8 & 0x01;
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_64;
  /*$19C1*/ CYCLES_EDGE(0x19c1, 1);
            branchTarget = true;
            goto bb_88;
bb_64:
  /*$19C3*/ CYCLES(0x19c3, 12);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$19C4*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
  /*$19C6*/ tmp1_U8 = tmp1_U8 >= ram_peek(0x0005);
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_66;
  /*$19C8*/ CYCLES_EDGE(0x19c8, 1);
            branchTarget = true;
            goto bb_88;
bb_66:
  /*$19CA*/ CYCLES(0x19ca, 9);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_68;
  /*$19CB*/ tmp3_U16 = s_a;
            tmp2_U16 = (tmp3_U16 + 0x000f) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp2_U16, (uint8_t)tmp3_U16, (uint8_t)0x000f);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_69;
bb_68:
  /*$19CB*/ tmp2_U16 = adc_dec16(s_a, 0x0f, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            s_status_v = (((uint8_t)(tmp2_U16 >> 8) & 0x40) != 0);
bb_69:
  /*$19CD*/ tmp1_U8 = s_a >= ram_peek(0x0005);
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_71;
  /*$19CF*/ CYCLES_EDGE(0x19cf, 1);
            branchTarget = true;
            goto bb_88;
bb_71:
  /*$19D1*/ CYCLES(0x19d1, 12);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$19D2*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
  /*$19D4*/ tmp1_U8 = tmp1_U8 >= ram_peek(0x0007);
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_73;
  /*$19D6*/ CYCLES_EDGE(0x19d6, 1);
            branchTarget = true;
            goto bb_88;
bb_73:
  /*$19D8*/ CYCLES(0x19d8, 9);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_75;
  /*$19D9*/ tmp2_U16 = s_a;
            tmp3_U16 = (tmp2_U16 + 0x0005) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0x0005);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_76;
bb_75:
  /*$19D9*/ tmp2_U16 = adc_dec16(s_a, 0x05, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            s_status_v = (((uint8_t)(tmp2_U16 >> 8) & 0x40) != 0);
bb_76:
  /*$19DB*/ tmp1_U8 = s_a >= ram_peek(0x0007);
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_78;
  /*$19DD*/ CYCLES_EDGE(0x19dd, 1);
            branchTarget = true;
            goto bb_88;
bb_78:
  /*$19DF*/ CYCLES(0x19df, 71);
  /*$19E3*/ poke(ram_peek16al(0x0008), 0x02);
  /*$19E5*/ s_y = 0x01;
  /*$19E6*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0001));
            s_a = tmp1_U8;
  /*$19E8*/ ram_poke(0x0004, tmp1_U8);
  /*$19EA*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_80;
  /*$19EB*/ s_a = (uint8_t)((s_a + 0x000f) + s_status_c);
            goto bb_81;
bb_80:
  /*$19EB*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0f, s_status_c));
bb_81:
  /*$19ED*/ ram_poke(0x0005, s_a);
  /*$19EF*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$19F0*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
  /*$19F2*/ ram_poke(0x0006, tmp1_U8);
  /*$19F4*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_83;
  /*$19F5*/ s_a = (uint8_t)((s_a + 0x0004) + s_status_c);
            goto bb_84;
bb_83:
  /*$19F5*/ s_a = ((uint8_t)adc_dec16(s_a, 0x04, s_status_c));
bb_84:
  /*$19F7*/ ram_poke(0x0007, s_a);
  /*$19F9*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$19FA*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
  /*$19FC*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_86;
  /*$19FD*/ s_a = (uint8_t)((s_a + 0x0070) + s_status_c);
            goto bb_87;
bb_86:
  /*$19FD*/ s_a = ((uint8_t)adc_dec16(s_a, 0x70, s_status_c));
bb_87:
  /*$19FF*/ ram_poke(0x1544, s_a);
  /*$1A04*/ ram_poke(0x1543, 0x00);
  /*$1A07*/ func_1af0(0x1a09);
  /*$1A0A*/ CYCLES(0x1a0a, 3);
  /*$1940*/ CYCLES(0x1940, 9);
            s_a = ram_peek(0x0089);
  /*$1942*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_26;
            else
              goto bb_25;
bb_88:
  /*$1A0D*/ CYCLES(0x1a0d, 5);
            branchTarget = true;
            if (ram_peek(0x007d))
              goto bb_90;
  /*$1A0F*/ CYCLES_EDGE(0x1a0f, 1);
  /*$1A18*/ CYCLES(0x1a18, 6);
            if (ret_addr) pop16(); return;
bb_90:
  /*$1A11*/ CYCLES(0x1a11, 16);
            ram_poke(0x007e, (uint8_t)(ram_peek(0x007e) + 0x01));
  /*$1A15*/ ram_poke(0x007d, 0x00);
  /*$1A17*/ if (ret_addr) pop16(); return;
bb_91:
  /*$1C58*/ CYCLES(0x1c58, 11);
            s_y = 0x00;
  /*$1C5A*/ tmp1_U8 = peek(ram_peek16al(0x0008));
            branchTarget = true;
            if ((tmp1_U8 & 0x01))
              goto bb_93;
  /*$1C5D*/ CYCLES_EDGE(0x1c5d, 1);
            branchTarget = true;
            goto bb_117;
bb_93:
  /*$1C5F*/ CYCLES(0x1c5f, 12);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$1C60*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 >= ram_peek(0x0005)))
              goto bb_95;
  /*$1C64*/ CYCLES_EDGE(0x1c64, 1);
            branchTarget = true;
            goto bb_117;
bb_95:
  /*$1C66*/ CYCLES(0x1c66, 9);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_97;
  /*$1C67*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
            goto bb_98;
bb_97:
  /*$1C67*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
bb_98:
            branchTarget = true;
            if ((s_a >= ram_peek(0x0005)))
              goto bb_100;
  /*$1C6B*/ CYCLES_EDGE(0x1c6b, 1);
            branchTarget = true;
            goto bb_117;
bb_100:
  /*$1C6D*/ CYCLES(0x1c6d, 12);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$1C6E*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 >= ram_peek(0x0007)))
              goto bb_102;
  /*$1C72*/ CYCLES_EDGE(0x1c72, 1);
            branchTarget = true;
            goto bb_117;
bb_102:
  /*$1C74*/ CYCLES(0x1c74, 9);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_104;
  /*$1C75*/ s_a = (uint8_t)((s_a + 0x0003) + s_status_c);
            goto bb_105;
bb_104:
  /*$1C75*/ s_a = ((uint8_t)adc_dec16(s_a, 0x03, s_status_c));
bb_105:
            branchTarget = true;
            if ((s_a >= ram_peek(0x0007)))
              goto bb_107;
  /*$1C79*/ CYCLES_EDGE(0x1c79, 1);
            branchTarget = true;
            goto bb_117;
bb_107:
  /*$1C7B*/ CYCLES(0x1c7b, 68);
  /*$1C7F*/ poke(ram_peek16al(0x0008), 0x02);
  /*$1C81*/ s_y = 0x01;
  /*$1C82*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0001));
            s_a = tmp1_U8;
  /*$1C84*/ ram_poke(0x0004, tmp1_U8);
  /*$1C86*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_109;
  /*$1C87*/ s_a = (uint8_t)((s_a + 0x000a) + s_status_c);
            goto bb_110;
bb_109:
  /*$1C87*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0a, s_status_c));
bb_110:
  /*$1C89*/ ram_poke(0x0005, s_a);
  /*$1C8B*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$1C8C*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
  /*$1C8E*/ ram_poke(0x0006, tmp1_U8);
  /*$1C90*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_112;
  /*$1C91*/ s_a = (uint8_t)((s_a + 0x0004) + s_status_c);
            goto bb_113;
bb_112:
  /*$1C91*/ s_a = ((uint8_t)adc_dec16(s_a, 0x04, s_status_c));
bb_113:
  /*$1C93*/ ram_poke(0x0007, s_a);
  /*$1C96*/ tmp1_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(s_y + 0x01)));
            s_a = tmp1_U8;
  /*$1C98*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_115;
  /*$1C99*/ s_a = (uint8_t)((s_a + 0x0067) + s_status_c);
            goto bb_116;
bb_115:
  /*$1C99*/ s_a = ((uint8_t)adc_dec16(s_a, 0x67, s_status_c));
bb_116:
  /*$1C9B*/ ram_poke(0x1544, s_a);
  /*$1CA0*/ ram_poke(0x1543, 0xd8);
  /*$1AE0*/ CYCLES(0x1ae0, 16);
  /*$1AE4*/ poke((ram_peek16al(0x0008) + 0x0003), 0x00);
  /*$1AE6*/ func_1530(0x1ae8);
  /*$1AE9*/ CYCLES(0x1ae9, 7);
            s_x = 0x80;
  /*$1AEB*/ s_y = 0x00;
  /*$1AED*/ func_2e00(0x0000);
            if (ret_addr) pop16(); return;
bb_117:
  /*$1CA9*/ CYCLES(0x1ca9, 14);
            s_a = ram_peek(0x0008);
  /*$1CAB*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_119;
  /*$1CAC*/ tmp2_U16 = s_a;
            tmp3_U16 = (tmp2_U16 + 0x0008) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0x0008);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_120;
bb_119:
  /*$1CAC*/ tmp3_U16 = adc_dec16(s_a, 0x08, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_120:
  /*$1CAE*/ tmp1_U8 = s_a;
            ram_poke(0x0008, tmp1_U8);
  /*$1CB0*/ tmp1_U8 = tmp1_U8 >= 0x20;
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_122;
  /*$1CB2*/ CYCLES_EDGE(0x1cb2, 1);
            branchTarget = true;
            goto bb_91;
bb_122:
  /*$1CB4*/ CYCLES(0x1cb4, 6);
            if (ret_addr) pop16(); return;
}


void func_1500(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1500*/ CYCLES(0x1500, 3);
            s_x = ram_peek(0x0004);
bb_1:
  /*$1502*/ CYCLES(0x1502, 17);
            tmp1_U8 = s_x;
  /*$1505*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$1507*/ tmp1_U8 = ram_peek((0x0c00 + tmp1_U8));
            s_a = tmp1_U8;
  /*$150A*/ ram_poke(0x0001, tmp1_U8);
  /*$150C*/ s_y = ram_peek(0x0006);
bb_2:
  /*$150E*/ CYCLES(0x150e, 4);
            branchTarget = true;
            if (!(s_y >= 0x28))
              goto bb_4;
  /*$1510*/ CYCLES_EDGE(0x1510, 1);
            branchTarget = true;
            goto bb_7;
bb_4:
  /*$1512*/ CYCLES(0x1512, 6);
            tmp1_U8 = peek(ram_peek16(0x1513));
            s_a = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_6;
  /*$1515*/ CYCLES_EDGE(0x1515, 1);
            branchTarget = true;
            goto bb_7;
bb_6:
  /*$1517*/ CYCLES(0x1517, 6);
            poke((ram_peek16al(0x0000) + s_y), s_a);
bb_7:
  /*$1519*/ CYCLES(0x1519, 13);
            ram_poke(0x1513, (uint8_t)(ram_peek(0x1513) + 0x01));
  /*$151C*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 != ram_peek(0x0007)))
              goto bb_9;
  /*$151F*/ CYCLES_EDGE(0x151f, 1);
            branchTarget = true;
            goto bb_2;
bb_9:
  /*$1521*/ CYCLES(0x1521, 7);
            tmp1_U8 = (uint8_t)(s_x + 0x01);
            s_x = tmp1_U8;
  /*$1522*/ tmp1_U8 = tmp1_U8 >= ram_peek(0x0005);
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_11;
  /*$1524*/ CYCLES_EDGE(0x1524, 1);
            branchTarget = true;
            goto bb_1;
bb_11:
  /*$1526*/ CYCLES(0x1526, 6);
            if (ret_addr) pop16(); return;
}


void func_1530(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1530*/ CYCLES(0x1530, 3);
            s_x = ram_peek(0x0004);
bb_1:
  /*$1532*/ CYCLES(0x1532, 17);
            tmp1_U8 = s_x;
  /*$1535*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$153A*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$153C*/ s_y = ram_peek(0x0006);
bb_2:
  /*$153E*/ CYCLES(0x153e, 4);
            branchTarget = true;
            if (!(s_y >= 0x28))
              goto bb_4;
  /*$1540*/ CYCLES_EDGE(0x1540, 1);
            branchTarget = true;
            goto bb_7;
bb_4:
  /*$1542*/ CYCLES(0x1542, 6);
            tmp1_U8 = peek(ram_peek16(0x1543));
            branchTarget = true;
            if (tmp1_U8)
              goto bb_6;
  /*$1545*/ CYCLES_EDGE(0x1545, 1);
            branchTarget = true;
            goto bb_7;
bb_6:
  /*$1547*/ CYCLES(0x1547, 8);
  /*$1549*/ poke((ram_peek16al(0x0000) + s_y), 0x00);
bb_7:
  /*$154B*/ CYCLES(0x154b, 13);
            ram_poke(0x1543, (uint8_t)(ram_peek(0x1543) + 0x01));
  /*$154E*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 != ram_peek(0x0007)))
              goto bb_9;
  /*$1551*/ CYCLES_EDGE(0x1551, 1);
            branchTarget = true;
            goto bb_2;
bb_9:
  /*$1553*/ CYCLES(0x1553, 7);
            tmp1_U8 = (uint8_t)(s_x + 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= ram_peek(0x0005)))
              goto bb_11;
  /*$1556*/ CYCLES_EDGE(0x1556, 1);
            branchTarget = true;
            goto bb_1;
bb_11:
  /*$1558*/ CYCLES(0x1558, 6);
            if (ret_addr) pop16(); return;
}


void func_1560(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1560*/ CYCLES(0x1560, 13);
  /*$1562*/ ram_poke(0x0008, 0x00);
  /*$1564*/ ram_poke(0x000a, 0x00);
  /*$1568*/ ram_poke(0x0009, 0x03);
bb_1:
  /*$156A*/ CYCLES(0x156a, 9);
            s_y = 0x00;
  /*$156C*/ tmp1_U8 = peek(ram_peek16al(0x0008));
            s_status_not_z = tmp1_U8;
            s_a = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_3;
  /*$156E*/ CYCLES_EDGE(0x156e, 1);
            branchTarget = true;
            goto bb_39;
bb_3:
  /*$1570*/ CYCLES(0x1570, 4);
            tmp1_U8 = s_a;
            s_a = (tmp1_U8 >> 0x01);
            branchTarget = true;
            if (!(tmp1_U8 & 0x01))
              goto bb_5;
  /*$1571*/ CYCLES_EDGE(0x1571, 1);
  /*$1582*/ CYCLES(0x1582, 64);
            push8(s_a);
  /*$1583*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$1584*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
  /*$1586*/ ram_poke(0x0004, tmp1_U8);
  /*$1588*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_7;
            else
              goto bb_6;
bb_5:
  /*$1573*/ CYCLES(0x1573, 6);
            func_1640(0x1575);
  /*$1576*/ CYCLES(0x1576, 3);
            branchTarget = true;
            goto bb_47;
bb_6:
  /*$1589*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
            goto bb_8;
bb_7:
  /*$1589*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
bb_8:
  /*$158B*/ ram_poke(0x0005, s_a);
  /*$158D*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$158E*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
  /*$1590*/ ram_poke(0x0006, tmp1_U8);
  /*$1592*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_10;
  /*$1593*/ s_a = (uint8_t)((s_a + 0x0002) + s_status_c);
            goto bb_11;
bb_10:
  /*$1593*/ s_a = ((uint8_t)adc_dec16(s_a, 0x02, s_status_c));
bb_11:
  /*$1595*/ ram_poke(0x0007, s_a);
  /*$1598*/ tmp1_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(s_y + 0x01)));
            s_a = tmp1_U8;
  /*$159A*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_13;
  /*$159B*/ s_a = (uint8_t)((s_a + 0x0067) + s_status_c);
            goto bb_14;
bb_13:
  /*$159B*/ s_a = ((uint8_t)adc_dec16(s_a, 0x67, s_status_c));
bb_14:
  /*$159D*/ ram_poke(0x1544, s_a);
  /*$15A2*/ ram_poke(0x1543, 0xc0);
  /*$15A5*/ func_1530(0x15a7);
  /*$15A8*/ CYCLES(0x15a8, 6);
            tmp1_U8 = pop8();
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_16;
  /*$15A9*/ CYCLES_EDGE(0x15a9, 1);
  /*$15AE*/ CYCLES(0x15ae, 14);
            s_a = ram_peek(0x0004);
  /*$15B0*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_18;
            else
              goto bb_17;
bb_16:
  /*$15AB*/ CYCLES(0x15ab, 3);
  /*$16C0*/ CYCLES(0x16c0, 16);
            s_a = ram_peek(0x0004);
  /*$16C2*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_63;
            else
              goto bb_62;
bb_17:
  /*$15B1*/ s_a = (uint8_t)((s_a - ram_peek(0x15b2)) - (uint8_t)(0x01 - s_status_c));
            goto bb_19;
bb_18:
  /*$15B1*/ s_a = ((uint8_t)sbc_dec16(s_a, ram_peek(0x15b2), s_status_c));
bb_19:
  /*$15B3*/ tmp1_U8 = s_a;
            ram_poke(0x0004, tmp1_U8);
            branchTarget = true;
            if (!(tmp1_U8 >= 0xf0))
              goto bb_21;
  /*$15B7*/ CYCLES_EDGE(0x15b7, 1);
            branchTarget = true;
            goto bb_38;
bb_21:
  /*$15B9*/ CYCLES(0x15b9, 17);
  /*$15BB*/ poke((ram_peek16al(0x0008) + 0x0001), s_a);
  /*$15BE*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_23;
  /*$15BF*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
            goto bb_24;
bb_23:
  /*$15BF*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
bb_24:
  /*$15C1*/ ram_poke(0x0005, s_a);
bb_25:
  /*$15C3*/ CYCLES(0x15c3, 13);
            s_y = 0x03;
  /*$15C5*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0003));
            s_a = tmp1_U8;
  /*$15C7*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_27;
  /*$15C8*/ tmp1_U8 = (uint8_t)((s_a - 0x0004) - (uint8_t)(0x01 - s_status_c));
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            goto bb_28;
bb_27:
  /*$15C8*/ tmp2_U16 = sbc_dec16(s_a, 0x04, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            s_status_n = ((uint8_t)(tmp2_U16 >> 8) & 0x80);
bb_28:
            branchTarget = true;
            if (s_status_n)
              goto bb_30;
  /*$15CA*/ CYCLES_EDGE(0x15ca, 1);
            branchTarget = true;
            goto bb_43;
bb_30:
  /*$15CC*/ CYCLES(0x15cc, 25);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_32;
  /*$15CD*/ s_a = (uint8_t)((s_a + 0x0007) + s_status_c);
            goto bb_33;
bb_32:
  /*$15CD*/ s_a = ((uint8_t)adc_dec16(s_a, 0x07, s_status_c));
bb_33:
  /*$15CF*/ tmp1_U8 = s_y;
            poke((ram_peek16al(0x0008) + tmp1_U8), s_a);
  /*$15D1*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_y = tmp1_U8;
  /*$15D2*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
  /*$15D4*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_35;
  /*$15D5*/ s_a = (uint8_t)((s_a - 0x0001) - (uint8_t)(0x01 - s_status_c));
            goto bb_36;
bb_35:
  /*$15D5*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x01, s_status_c));
bb_36:
            branchTarget = true;
            if ((s_a >= 0xfc))
              goto bb_38;
  /*$15D9*/ CYCLES_EDGE(0x15d9, 1);
            branchTarget = true;
            goto bb_42;
bb_38:
  /*$15DB*/ CYCLES(0x15db, 10);
            s_y = 0x00;
  /*$15DD*/ s_status_not_z = 0x00;
            s_a = 0x00;
  /*$15DF*/ poke(ram_peek16al(0x0008), 0x00);
bb_39:
  /*$15E1*/ CYCLES(0x15e1, 2);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_41;
  /*$15E1*/ CYCLES_EDGE(0x15e1, 1);
            branchTarget = true;
            goto bb_47;
bb_41:
  /*$15E3*/ CYCLES(0x15e3, 5);
            ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) - 0x01));
bb_42:
  /*$15E5*/ CYCLES(0x15e5, 10);
            ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) - 0x01));
  /*$15E7*/ ram_poke(0x0007, (uint8_t)(ram_peek(0x0007) - 0x01));
bb_43:
  /*$15E9*/ CYCLES(0x15e9, 39);
            tmp1_U8 = s_a;
            poke((ram_peek16al(0x0008) + s_y), tmp1_U8);
  /*$15EB*/ s_y = 0x03;
  /*$15ED*/ poke(ram_peek16al(0x15ee), tmp1_U8);
  /*$15F1*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0003));
            s_a = tmp1_U8;
  /*$15F3*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_45;
  /*$15F4*/ s_a = (uint8_t)((s_a + 0x0067) + s_status_c);
            goto bb_46;
bb_45:
  /*$15F4*/ s_a = ((uint8_t)adc_dec16(s_a, 0x67, s_status_c));
bb_46:
  /*$15F6*/ ram_poke(0x1514, s_a);
  /*$15FB*/ ram_poke(0x1513, 0xc0);
  /*$15FE*/ func_1500(0x1600);
            branchTarget = true;
bb_47:
  /*$1601*/ CYCLES(0x1601, 14);
            s_a = ram_peek(0x0008);
  /*$1603*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_49;
  /*$1604*/ s_a = (uint8_t)((s_a + 0x0008) + s_status_c);
            goto bb_50;
bb_49:
  /*$1604*/ s_a = ((uint8_t)adc_dec16(s_a, 0x08, s_status_c));
bb_50:
  /*$1606*/ tmp1_U8 = s_a;
            ram_poke(0x0008, tmp1_U8);
            branchTarget = true;
            if (!(tmp1_U8 >= 0x10))
              goto bb_52;
  /*$160A*/ CYCLES_EDGE(0x160a, 1);
  /*$160F*/ CYCLES(0x160f, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x00fe) - 0x01);
            ram_poke(0x00fe, tmp1_U8);
            branchTarget = true;
            if (tmp1_U8)
              goto bb_53;
            else
              goto bb_54;
bb_52:
  /*$160C*/ CYCLES(0x160c, 3);
            branchTarget = true;
            goto bb_1;
bb_53:
  /*$1611*/ CYCLES_EDGE(0x1611, 1);
            branchTarget = true;
            goto bb_61;
bb_54:
  /*$1613*/ CYCLES(0x1613, 49);
  /*$1615*/ ram_poke(0x00fd, 0x00);
  /*$1619*/ s_a = (uint8_t)(ram_peek(0x00ff) << 0x01);
  /*$161A*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_56;
  /*$161B*/ s_a = (uint8_t)((s_a + ram_peek(0x00ff)) + s_status_c);
            goto bb_57;
bb_56:
  /*$161B*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x00ff), s_status_c));
bb_57:
  /*$161D*/ tmp1_U8 = s_a;
            ram_poke(0x00ff, tmp1_U8);
  /*$161F*/ tmp2_U16 = tmp1_U8;
  /*$1620*/ ram_poke(0x00fd, (uint8_t)((ram_peek(0x00fd) << 0x01) | (uint8_t)((tmp2_U16 << 0x01) >> 8)));
  /*$1623*/ ram_poke(0x00fd, (uint8_t)((ram_peek(0x00fd) << 0x01) | (uint8_t)(((tmp2_U16 << 0x02) & 0x01ff) >> 8)));
  /*$1628*/ tmp2_U16 = ram_peek(0x00fd) << 0x02;
  /*$1629*/ s_y = ((uint8_t)tmp2_U16);
            branchTarget = true;
            if (!(ram_peek((0x0300 + (tmp2_U16 & 0x00ff))) != 0x01))
              goto bb_59;
  /*$162F*/ CYCLES_EDGE(0x162f, 1);
            branchTarget = true;
            goto bb_60;
bb_59:
  /*$1631*/ CYCLES(0x1631, 7);
  /*$1633*/ ram_poke((0x0300 + s_y), 0x03);
bb_60:
  /*$1636*/ CYCLES(0x1636, 5);
            s_a = 0x0a;
  /*$1638*/ ram_poke(0x00fe, 0x0a);
bb_61:
  /*$163A*/ CYCLES(0x163a, 6);
            if (ret_addr) pop16(); return;
bb_62:
  /*$16C3*/ s_a = (uint8_t)((s_a + ram_peek(0x16c4)) + s_status_c);
            goto bb_64;
bb_63:
  /*$16C3*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x16c4), s_status_c));
bb_64:
  /*$16C5*/ s_y = 0x04;
  /*$16C7*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0004));
            branchTarget = true;
            if ((s_a >= tmp1_U8))
              goto bb_66;
  /*$16C9*/ CYCLES_EDGE(0x16c9, 1);
            branchTarget = true;
            goto bb_67;
bb_66:
  /*$16CB*/ CYCLES(0x16cb, 5);
            tmp1_U8 = peek((ram_peek16al(0x0008) + s_y));
            s_a = tmp1_U8;
bb_67:
  /*$16CD*/ CYCLES(0x16cd, 21);
            tmp1_U8 = s_a;
            ram_poke(0x0004, tmp1_U8);
  /*$16D1*/ poke((ram_peek16al(0x0008) + 0x0001), tmp1_U8);
  /*$16D3*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_69;
  /*$16D4*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
            goto bb_70;
bb_69:
  /*$16D4*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
bb_70:
  /*$16D6*/ ram_poke(0x0005, s_a);
            branchTarget = true;
            goto bb_25;
}


void func_1640(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1640*/ CYCLES(0x1640, 48);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$1641*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
  /*$1643*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_2;
  /*$1644*/ s_a = (uint8_t)((s_a - 0x0004) - (uint8_t)(0x01 - s_status_c));
            goto bb_3;
bb_2:
  /*$1644*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x04, s_status_c));
bb_3:
  /*$1646*/ ram_poke(0x000b, s_a);
  /*$1648*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$1649*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
  /*$164B*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_5;
  /*$164C*/ s_a = (uint8_t)((s_a - 0x0002) - (uint8_t)(0x01 - s_status_c));
            goto bb_6;
bb_5:
  /*$164C*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x02, s_status_c));
bb_6:
  /*$1650*/ ram_poke(0x000c, s_a);
  /*$1653*/ tmp1_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(s_y + 0x01)));
            s_a = tmp1_U8;
  /*$1655*/ ram_poke(0x000a, tmp1_U8);
  /*$1657*/ func_2b60(0x1659);
  /*$165A*/ CYCLES(0x165a, 12);
            ram_poke(0x000a, (uint8_t)(ram_peek(0x000a) + 0x01));
  /*$165C*/ tmp1_U8 = ram_peek(0x000a);
            s_a = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0x04)) {
    /*$1660*/ CYCLES_EDGE(0x1660, 1);
    /*$1667*/ CYCLES(0x1667, 16);
              s_y = 0x00;
    /*$166B*/ poke(ram_peek16al(0x0008), 0x00);
    /*$166D*/ if (ret_addr) pop16(); return;
            }
  /*$1662*/ CYCLES(0x1662, 14);
            s_y = 0x03;
  /*$1664*/ poke((ram_peek16al(0x0008) + 0x0003), s_a);
  /*$1666*/ if (ret_addr) pop16(); return;
}


void func_1738(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1738*/ CYCLES(0x1738, 6);
            tmp1_U8 = ram_peek(0x0320);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_7;
            else
              goto bb_8;
bb_1:
  /*$1723*/ CYCLES(0x1723, 6);
            poke(ram_peek16al(0x1724), s_a);
  /*$1726*/ s_y = 0x18;
bb_2:
  /*$1728*/ CYCLES(0x1728, 4);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_4;
  /*$1729*/ CYCLES_EDGE(0x1729, 1);
            branchTarget = true;
            goto bb_2;
bb_4:
  /*$172B*/ CYCLES(0x172b, 4);
            tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_6;
  /*$172C*/ CYCLES_EDGE(0x172c, 1);
            branchTarget = true;
            goto bb_1;
bb_6:
  /*$172E*/ CYCLES(0x172e, 6);
            if (ret_addr) pop16(); return;
bb_7:
  /*$173B*/ CYCLES_EDGE(0x173b, 1);
  /*$17AE*/ CYCLES(0x17ae, 6);
            if (ret_addr) pop16(); return;
bb_8:
  /*$173D*/ CYCLES(0x173d, 4);
            tmp1_U8 = s_a & 0x01;
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_10;
  /*$173E*/ CYCLES_EDGE(0x173e, 1);
  /*$1745*/ CYCLES(0x1745, 52);
            tmp1_U8 = ram_peek(0x0321);
            s_a = tmp1_U8;
  /*$1748*/ ram_poke(0x0004, tmp1_U8);
  /*$174A*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_14;
            else
              goto bb_13;
bb_10:
  /*$1740*/ CYCLES(0x1740, 2);
            branchTarget = true;
            if (s_status_c)
              goto bb_12;
  /*$1740*/ CYCLES_EDGE(0x1740, 1);
  /*$17B0*/ CYCLES(0x17b0, 35);
            s_a = ram_peek(0x0321);
  /*$17B3*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_43;
            else
              goto bb_42;
bb_12:
  /*$1742*/ CYCLES(0x1742, 0);
            fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x1742, 0x1742, 0x03); error_handler(0x1742);
bb_13:
  /*$174B*/ s_a = (uint8_t)((s_a + 0x000f) + s_status_c);
            goto bb_15;
bb_14:
  /*$174B*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0f, s_status_c));
bb_15:
  /*$174D*/ ram_poke(0x0005, s_a);
  /*$174F*/ tmp1_U8 = ram_peek(0x0322);
            s_a = tmp1_U8;
  /*$1752*/ ram_poke(0x0006, tmp1_U8);
  /*$1754*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_17;
  /*$1755*/ s_a = (uint8_t)((s_a + 0x0004) + s_status_c);
            goto bb_18;
bb_17:
  /*$1755*/ s_a = ((uint8_t)adc_dec16(s_a, 0x04, s_status_c));
bb_18:
  /*$1757*/ ram_poke(0x0007, s_a);
  /*$1759*/ s_a = ram_peek(0x0323);
  /*$175C*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_20;
  /*$175D*/ s_a = (uint8_t)((s_a + 0x0070) + s_status_c);
            goto bb_21;
bb_20:
  /*$175D*/ s_a = ((uint8_t)adc_dec16(s_a, 0x70, s_status_c));
bb_21:
  /*$175F*/ ram_poke(0x1544, s_a);
  /*$1764*/ ram_poke(0x1543, 0x00);
  /*$1767*/ func_1530(0x1769);
  /*$176A*/ CYCLES(0x176a, 10);
            s_a = ram_peek(0x0323);
  /*$176D*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_23;
  /*$176E*/ tmp1_U8 = (uint8_t)((s_a - 0x0004) - (uint8_t)(0x01 - s_status_c));
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            goto bb_24;
bb_23:
  /*$176E*/ tmp3_U16 = sbc_dec16(s_a, 0x04, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_n = ((uint8_t)(tmp3_U16 >> 8) & 0x80);
bb_24:
            branchTarget = true;
            if (s_status_n)
              goto bb_26;
  /*$1770*/ CYCLES_EDGE(0x1770, 1);
  /*$178D*/ CYCLES(0x178d, 4);
            ram_poke(0x0323, s_a);
            goto bb_34;
bb_26:
  /*$1772*/ CYCLES(0x1772, 22);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_28;
  /*$1773*/ tmp3_U16 = s_a;
            tmp2_U16 = (tmp3_U16 + 0x0007) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp2_U16, (uint8_t)tmp3_U16, (uint8_t)0x0007);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_29;
bb_28:
  /*$1773*/ tmp3_U16 = adc_dec16(s_a, 0x07, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_29:
  /*$1775*/ ram_poke(0x0323, s_a);
  /*$1778*/ ram_poke(0x0322, (uint8_t)(ram_peek(0x0322) - 0x01));
  /*$177B*/ tmp1_U8 = ram_peek(0x0322);
            s_a = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0xf0))
              goto bb_31;
  /*$1780*/ CYCLES_EDGE(0x1780, 1);
            branchTarget = true;
            goto bb_34;
bb_31:
  /*$1782*/ CYCLES(0x1782, 4);
            tmp1_U8 = s_a >= 0xfc;
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_33;
  /*$1784*/ CYCLES_EDGE(0x1784, 1);
            branchTarget = true;
            goto bb_34;
bb_33:
  /*$1786*/ CYCLES(0x1786, 8);
            s_a = 0x00;
  /*$1788*/ ram_poke(0x0320, 0x00);
  /*$178B*/ CYCLES_EDGE(0x178b, 1);
            branchTarget = true;
            goto bb_41;
bb_34:
  /*$1790*/ CYCLES(0x1790, 38);
            s_a = ram_peek(0x0323);
  /*$1793*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_36;
  /*$1794*/ s_a = (uint8_t)((s_a + 0x0070) + s_status_c);
            goto bb_37;
bb_36:
  /*$1794*/ s_a = ((uint8_t)adc_dec16(s_a, 0x70, s_status_c));
bb_37:
  /*$1796*/ ram_poke(0x1514, s_a);
  /*$179B*/ ram_poke(0x1513, 0x00);
  /*$179E*/ tmp1_U8 = ram_peek(0x0322);
            s_a = tmp1_U8;
  /*$17A1*/ ram_poke(0x0006, tmp1_U8);
  /*$17A3*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_39;
  /*$17A4*/ tmp2_U16 = s_a;
            tmp3_U16 = (tmp2_U16 + 0x0004) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0x0004);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_40;
bb_39:
  /*$17A4*/ tmp3_U16 = adc_dec16(s_a, 0x04, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_40:
  /*$17A6*/ ram_poke(0x0007, s_a);
  /*$17A8*/ func_1500(0x17aa);
            branchTarget = true;
bb_41:
  /*$17AB*/ CYCLES(0x17ab, 3);
  /*$1720*/ CYCLES(0x1720, 4);
            s_x = ram_peek(0x0323);
            goto bb_1;
bb_42:
  /*$17B4*/ s_a = (uint8_t)((s_a - 0x0004) - (uint8_t)(0x01 - s_status_c));
            goto bb_44;
bb_43:
  /*$17B4*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x04, s_status_c));
bb_44:
  /*$17B6*/ ram_poke(0x000b, s_a);
  /*$17B8*/ s_a = ram_peek(0x0322);
  /*$17BB*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_46;
  /*$17BC*/ tmp2_U16 = s_a;
            tmp3_U16 = (tmp2_U16 - 0x0001) - (uint8_t)(0x01 - s_status_c);
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0xfffe);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_47;
bb_46:
  /*$17BC*/ tmp3_U16 = sbc_dec16(s_a, 0x01, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_47:
  /*$17BE*/ ram_poke(0x000c, s_a);
  /*$17C0*/ tmp1_U8 = ram_peek(0x0323);
            s_a = tmp1_U8;
  /*$17C3*/ ram_poke(0x000a, tmp1_U8);
  /*$17C5*/ func_2b60(0x17c7);
  /*$17C8*/ CYCLES(0x17c8, 12);
            ram_poke(0x000a, (uint8_t)(ram_peek(0x000a) + 0x01));
  /*$17CA*/ tmp1_U8 = ram_peek(0x000a);
            s_a = tmp1_U8;
  /*$17CC*/ tmp1_U8 = tmp1_U8 >= 0x04;
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8) {
    /*$17CE*/ CYCLES_EDGE(0x17ce, 1);
    /*$17D4*/ CYCLES(0x17d4, 12);
    /*$17D6*/ ram_poke(0x0320, 0x00);
    /*$17D9*/ if (ret_addr) pop16(); return;
            }
  /*$17D0*/ CYCLES(0x17d0, 10);
            ram_poke(0x0323, s_a);
  /*$17D3*/ if (ret_addr) pop16(); return;
}


void func_18e4(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;
  uint16_t tmp4_U16;
  uint16_t tmp5_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$18E4*/ CYCLES(0x18e4, 21);
  /*$18E6*/ ram_poke(0x0008, 0x20);
  /*$18EA*/ ram_poke(0x0009, 0x03);
  /*$18EC*/ s_y = 0x00;
  /*$18EE*/ tmp1_U8 = peek(ram_peek16al(0x0008));
  /*$18F0*/ tmp2_U8 = tmp1_U8 & 0x01;
            s_status_c = tmp2_U8;
            tmp1_U8 = tmp1_U8 >> 0x01;
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            branchTarget = true;
            if (tmp2_U8)
              goto bb_2;
  /*$18F1*/ CYCLES_EDGE(0x18f1, 1);
            branchTarget = true;
            goto bb_26;
bb_2:
  /*$18F3*/ CYCLES(0x18f3, 12);
            tmp2_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp2_U8;
  /*$18F4*/ tmp2_U8 = peek((ram_peek16al(0x0008) + tmp2_U8));
            s_a = tmp2_U8;
  /*$18F6*/ tmp1_U8 = ram_peek(0x0005);
            s_status_not_z = (tmp2_U8 != tmp1_U8);
            tmp3_U8 = tmp2_U8 >= tmp1_U8;
            s_status_c = tmp3_U8;
            s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
            branchTarget = true;
            if (!tmp3_U8)
              goto bb_4;
  /*$18F8*/ CYCLES_EDGE(0x18f8, 1);
            branchTarget = true;
            goto bb_26;
bb_4:
  /*$18FA*/ CYCLES(0x18fa, 9);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_6;
  /*$18FB*/ tmp4_U16 = s_a;
            tmp5_U16 = (tmp4_U16 + 0x000f) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x000f);
            s_a = ((uint8_t)tmp5_U16);
            goto bb_7;
bb_6:
  /*$18FB*/ tmp5_U16 = adc_dec16(s_a, 0x0f, s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
bb_7:
  /*$18FD*/ tmp3_U8 = s_a;
            tmp1_U8 = ram_peek(0x0005);
            s_status_not_z = (tmp3_U8 != tmp1_U8);
            tmp2_U8 = tmp3_U8 >= tmp1_U8;
            s_status_c = tmp2_U8;
            s_status_n = ((uint8_t)(tmp3_U8 - tmp1_U8) & 0x80);
            branchTarget = true;
            if (tmp2_U8)
              goto bb_9;
  /*$18FF*/ CYCLES_EDGE(0x18ff, 1);
            branchTarget = true;
            goto bb_26;
bb_9:
  /*$1901*/ CYCLES(0x1901, 12);
            tmp2_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp2_U8;
  /*$1902*/ tmp2_U8 = peek((ram_peek16al(0x0008) + tmp2_U8));
            s_a = tmp2_U8;
  /*$1904*/ tmp1_U8 = ram_peek(0x0007);
            s_status_not_z = (tmp2_U8 != tmp1_U8);
            tmp3_U8 = tmp2_U8 >= tmp1_U8;
            s_status_c = tmp3_U8;
            s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
            branchTarget = true;
            if (!tmp3_U8)
              goto bb_11;
  /*$1906*/ CYCLES_EDGE(0x1906, 1);
            branchTarget = true;
            goto bb_26;
bb_11:
  /*$1908*/ CYCLES(0x1908, 9);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_13;
  /*$1909*/ tmp5_U16 = s_a;
            tmp4_U16 = (tmp5_U16 + 0x0005) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x0005);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_14;
bb_13:
  /*$1909*/ tmp5_U16 = adc_dec16(s_a, 0x05, s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
bb_14:
  /*$190B*/ tmp3_U8 = s_a;
            tmp1_U8 = ram_peek(0x0007);
            s_status_not_z = (tmp3_U8 != tmp1_U8);
            tmp2_U8 = tmp3_U8 >= tmp1_U8;
            s_status_c = tmp2_U8;
            s_status_n = ((uint8_t)(tmp3_U8 - tmp1_U8) & 0x80);
            branchTarget = true;
            if (tmp2_U8)
              goto bb_16;
  /*$190D*/ CYCLES_EDGE(0x190d, 1);
            branchTarget = true;
            goto bb_26;
bb_16:
  /*$190F*/ CYCLES(0x190f, 71);
  /*$1913*/ poke(ram_peek16al(0x0008), 0x02);
  /*$1915*/ s_y = 0x01;
  /*$1916*/ tmp2_U8 = peek((ram_peek16al(0x0008) + 0x0001));
            s_a = tmp2_U8;
  /*$1918*/ ram_poke(0x0004, tmp2_U8);
  /*$191A*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_18;
  /*$191B*/ s_a = (uint8_t)((s_a + 0x000f) + s_status_c);
            goto bb_19;
bb_18:
  /*$191B*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0f, s_status_c));
bb_19:
  /*$191D*/ ram_poke(0x0005, s_a);
  /*$191F*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp2_U8;
  /*$1920*/ tmp2_U8 = peek((ram_peek16al(0x0008) + tmp2_U8));
            s_a = tmp2_U8;
  /*$1922*/ ram_poke(0x0006, tmp2_U8);
  /*$1924*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_21;
  /*$1925*/ s_a = (uint8_t)((s_a + 0x0004) + s_status_c);
            goto bb_22;
bb_21:
  /*$1925*/ s_a = ((uint8_t)adc_dec16(s_a, 0x04, s_status_c));
bb_22:
  /*$1927*/ ram_poke(0x0007, s_a);
  /*$1929*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp2_U8;
  /*$192A*/ tmp2_U8 = peek((ram_peek16al(0x0008) + tmp2_U8));
            s_a = tmp2_U8;
  /*$192C*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_24;
  /*$192D*/ s_a = (uint8_t)((s_a + 0x0070) + s_status_c);
            goto bb_25;
bb_24:
  /*$192D*/ s_a = ((uint8_t)adc_dec16(s_a, 0x70, s_status_c));
bb_25:
  /*$192F*/ ram_poke(0x1544, s_a);
  /*$1934*/ ram_poke(0x1543, 0x00);
  /*$1937*/ func_1af0(0x1939);
  /*$193A*/ CYCLES(0x193a, 3);
  /*$1940*/ CYCLES(0x1940, 9);
            s_a = ram_peek(0x0089);
  /*$1942*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_28;
            else
              goto bb_27;
bb_26:
  /*$193D*/ CYCLES(0x193d, 6);
            if (ret_addr) pop16(); return;
bb_27:
  /*$1943*/ tmp4_U16 = s_a;
            tmp5_U16 = (tmp4_U16 + 0x0050) + s_status_c;
            s_status_c = (uint8_t)(tmp5_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x0050);
            tmp2_U8 = (uint8_t)tmp5_U16;
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_a = tmp2_U8;
            goto bb_29;
bb_28:
  /*$1943*/ tmp5_U16 = adc_dec16(s_a, 0x50, s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            tmp2_U8 = (uint8_t)(tmp5_U16 >> 8);
            s_status_c = (tmp2_U8 & 0x01);
            s_status_not_z = (~tmp2_U8 & 2);
            s_status_v = ((tmp2_U8 & 0x40) != 0);
            s_status_n = (tmp2_U8 & 0x80);
bb_29:
            branchTarget = true;
            if (s_status_c)
              goto bb_31;
  /*$1945*/ CYCLES_EDGE(0x1945, 1);
            branchTarget = true;
            goto bb_32;
bb_31:
  /*$1947*/ CYCLES(0x1947, 2);
            s_status_not_z = 0xff;
            s_status_n = 0x80;
            s_a = 0xff;
bb_32:
  /*$1949*/ CYCLES(0x1949, 9);
            ram_poke(0x0089, s_a);
  /*$194B*/ if (ret_addr) pop16(); return;
}


void func_1af0(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1AF0*/ CYCLES(0x1af0, 16);
  /*$1AF4*/ poke((ram_peek16al(0x0008) + 0x0003), 0x00);
  /*$1AF6*/ func_1530(0x1af8);
  /*$1AF9*/ CYCLES(0x1af9, 7);
            s_x = 0x50;
  /*$1AFB*/ s_y = 0x00;
  /*$1AFD*/ func_2e00(0x0000);
            if (ret_addr) pop16(); return;
}


void func_1d80(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1D80*/ CYCLES(0x1d80, 39);
            s_a = ram_peek(0x00e0);
  /*$1D82*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_2;
  /*$1D83*/ s_a = (uint8_t)((s_a - 0x000e) - (uint8_t)(0x01 - s_status_c));
            goto bb_3;
bb_2:
  /*$1D83*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x0e, s_status_c));
bb_3:
  /*$1D85*/ ram_poke(0x0004, s_a);
  /*$1D87*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_5;
  /*$1D88*/ s_a = (uint8_t)((s_a + 0x002a) + s_status_c);
            goto bb_6;
bb_5:
  /*$1D88*/ s_a = ((uint8_t)adc_dec16(s_a, 0x2a, s_status_c));
bb_6:
  /*$1D8A*/ ram_poke(0x0005, s_a);
  /*$1D8C*/ s_a = ram_peek(0x00e1);
  /*$1D8E*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_8;
  /*$1D8F*/ s_a = (uint8_t)((s_a - 0x0003) - (uint8_t)(0x01 - s_status_c));
            goto bb_9;
bb_8:
  /*$1D8F*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x03, s_status_c));
bb_9:
  /*$1D91*/ tmp1_U8 = s_a & 0xfe;
            s_a = tmp1_U8;
  /*$1D93*/ ram_poke(0x0006, tmp1_U8);
  /*$1D95*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_11;
  /*$1D96*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
            goto bb_12;
bb_11:
  /*$1D96*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
bb_12:
  /*$1D98*/ ram_poke(0x0007, s_a);
  /*$1EB0*/ CYCLES(0x1eb0, 10);
  /*$1EB2*/ ram_poke(0x0009, 0x00);
  /*$1EB6*/ ram_poke(0x000a, 0x01);
bb_13:
  /*$1EB8*/ CYCLES(0x1eb8, 12);
  /*$1EBA*/ ram_poke(0x0008, ram_peek(0x0009));
  /*$1EBC*/ func_1e80(0x1ebe);
  /*$1EBF*/ CYCLES(0x1ebf, 6);
            func_1db0(0x1ec1);
  /*$1EC2*/ CYCLES(0x1ec2, 17);
            ram_poke(0x0009, (uint8_t)(ram_peek(0x0009) + 0x01));
  /*$1EC6*/ ram_poke(0x0008, ram_peek(0x000a));
  /*$1EC8*/ func_1e80(0x1eca);
  /*$1ECB*/ CYCLES(0x1ecb, 6);
            func_1dc0(0x1ecd);
  /*$1ECE*/ CYCLES(0x1ece, 12);
            ram_poke(0x000a, (uint8_t)(ram_peek(0x000a) + 0x01));
  /*$1ED2*/ tmp1_U8 = ram_peek(0x000a) >= 0x06;
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_15;
  /*$1ED4*/ CYCLES_EDGE(0x1ed4, 1);
            branchTarget = true;
            goto bb_13;
bb_15:
  /*$1ED6*/ CYCLES(0x1ed6, 6);
            func_1ef3(0x1ed8);
  /*$1ED9*/ CYCLES(0x1ed9, 7);
  /*$1EDA*/ s_a = 0x07;
  /*$1EDC*/ ram_poke(0x0001, 0x07);
bb_16:
  /*$1EDE*/ CYCLES(0x1ede, 6);
            func_1dd0(0x1ee0);
  /*$1EE1*/ CYCLES(0x1ee1, 24);
            s_a = ram_peek(0x0001);
  /*$1EE3*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_18;
  /*$1EE4*/ s_a = (uint8_t)((s_a + 0x0008) + s_status_c);
            goto bb_19;
bb_18:
  /*$1EE4*/ s_a = ((uint8_t)adc_dec16(s_a, 0x08, s_status_c));
bb_19:
  /*$1EE7*/ ram_poke(0x0001, s_a);
  /*$1EE9*/ s_a = ram_peek(0x0000);
  /*$1EEB*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_21;
  /*$1EEC*/ tmp2_U16 = s_a;
            tmp3_U16 = (tmp2_U16 + 0x0004) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0x0004);
            tmp1_U8 = (uint8_t)tmp3_U16;
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            goto bb_22;
bb_21:
  /*$1EEC*/ tmp3_U16 = adc_dec16(s_a, 0x04, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            tmp1_U8 = (uint8_t)(tmp3_U16 >> 8);
            s_status_not_z = (~tmp1_U8 & 2);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
            s_status_n = (tmp1_U8 & 0x80);
bb_22:
  /*$1EEE*/ ram_poke(0x0000, s_a);
            branchTarget = true;
            if (s_status_n)
              goto bb_24;
  /*$1EF0*/ CYCLES_EDGE(0x1ef0, 1);
            branchTarget = true;
            goto bb_16;
bb_24:
  /*$1EF2*/ CYCLES(0x1ef2, 6);
            if (ret_addr) pop16(); return;
}


void func_1da0(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1DA0*/ CYCLES(0x1da0, 13);
  /*$1DA2*/ ram_poke(0x0000, 0x10);
  /*$1DA4*/ s_a = 0x00;
  /*$1DA6*/ ram_poke(0x0001, 0x00);
  /*$1DA8*/ func_1dd0(0x0000);
            if (ret_addr) pop16(); return;
}


void func_1db0(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1DB0*/ CYCLES(0x1db0, 13);
  /*$1DB2*/ ram_poke(0x0000, 0x20);
  /*$1DB4*/ s_a = 0x60;
  /*$1DB6*/ ram_poke(0x0001, 0x60);
  /*$1DB8*/ func_1dd0(0x0000);
            if (ret_addr) pop16(); return;
}


void func_1dc0(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1DC0*/ CYCLES(0x1dc0, 13);
  /*$1DC2*/ ram_poke(0x0000, 0x10);
  /*$1DC4*/ s_a = 0x50;
  /*$1DC6*/ ram_poke(0x0001, 0x50);
  /*$1DC8*/ func_1dd0(0x0000);
            if (ret_addr) pop16(); return;
}


void func_1dd0(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1DD0*/ CYCLES(0x1dd0, 3);
            s_x = ram_peek(0x0000);
bb_1:
  /*$1DD2*/ CYCLES(0x1dd2, 3);
            s_y = ram_peek(0x0001);
bb_2:
  /*$1DD4*/ CYCLES(0x1dd4, 10);
  /*$1DD7*/ tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_4;
  /*$1DD8*/ CYCLES_EDGE(0x1dd8, 1);
            branchTarget = true;
            goto bb_2;
bb_4:
  /*$1DDA*/ CYCLES(0x1dda, 8);
            poke(ram_peek16(0x1ddb), s_a);
  /*$1DDD*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_x = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_6;
  /*$1DDE*/ CYCLES_EDGE(0x1dde, 1);
            branchTarget = true;
            goto bb_1;
bb_6:
  /*$1DE0*/ CYCLES(0x1de0, 6);
            if (ret_addr) pop16(); return;
}


void func_1e00(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1E00*/ CYCLES(0x1e00, 14);
  /*$1E02*/ ram_poke(0x1e19, 0x00);
  /*$1E07*/ ram_poke(0x1e1a, 0x9b);
  /*$1E0A*/ s_x = 0xa8;
bb_1:
  /*$1E0C*/ CYCLES(0x1e0c, 16);
            tmp1_U8 = s_x;
  /*$1E0F*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$1E14*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$1E16*/ s_y = 0x00;
bb_2:
  /*$1E18*/ CYCLES(0x1e18, 18);
            tmp1_U8 = peek(ram_peek16(0x1e19));
  /*$1E1B*/ poke((ram_peek16al(0x0000) + s_y), tmp1_U8);
  /*$1E1D*/ tmp1_U8 = (uint8_t)(ram_peek(0x1e19) + 0x01);
            ram_poke(0x1e19, tmp1_U8);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_4;
  /*$1E20*/ CYCLES_EDGE(0x1e20, 1);
            branchTarget = true;
            goto bb_5;
bb_4:
  /*$1E22*/ CYCLES(0x1e22, 6);
            ram_poke(0x1e1a, (uint8_t)(ram_peek(0x1e1a) + 0x01));
bb_5:
  /*$1E25*/ CYCLES(0x1e25, 6);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0x28))
              goto bb_7;
  /*$1E28*/ CYCLES_EDGE(0x1e28, 1);
            branchTarget = true;
            goto bb_2;
bb_7:
  /*$1E2A*/ CYCLES(0x1e2a, 6);
            tmp1_U8 = (uint8_t)(s_x + 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0xc0))
              goto bb_9;
  /*$1E2D*/ CYCLES_EDGE(0x1e2d, 1);
            branchTarget = true;
            goto bb_1;
bb_9:
  /*$1E2F*/ CYCLES(0x1e2f, 6);
            func_2c40(0x1e31);
  /*$1E32*/ CYCLES(0x1e32, 3);
  /*$2DC0*/ CYCLES(0x2dc0, 6);
            func_2d00(0x2dc2);
  /*$2DC3*/ CYCLES(0x2dc3, 6);
            func_2d40(0x2dc5);
  /*$2DC6*/ CYCLES(0x2dc6, 6);
            func_2d80(0x2dc8);
  /*$2DC9*/ CYCLES(0x2dc9, 6);
            func_2d90(0x2dcb);
  /*$2DCC*/ CYCLES(0x2dcc, 3);
  /*$2DA0*/ CYCLES(0x2da0, 19);
  /*$2DA6*/ s_x = (uint8_t)((uint8_t)((ram_peek(0x0088) & 0x0f) << 0x01) + 0x01);
  /*$2DA7*/ s_y = 0x1d;
  /*$2DA9*/ func_2ce8(0x2dab);
  /*$2DAC*/ CYCLES(0x2dac, 20);
  /*$2DB2*/ tmp2_U16 = (ram_peek(0x0088) >> 0x04) << 0x01;
            s_status_c = (uint8_t)(tmp2_U16 >> 8);
  /*$2DB4*/ s_x = (uint8_t)(((uint8_t)tmp2_U16) + 0x01);
  /*$2DB5*/ func_2ce8(0x0000);
            if (ret_addr) pop16(); return;
}


void func_1e40(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1E40*/ CYCLES(0x1e40, 3);
            s_x = ram_peek(0x0004);
bb_1:
  /*$1E42*/ CYCLES(0x1e42, 17);
            tmp1_U8 = s_x;
  /*$1E45*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$1E47*/ tmp1_U8 = ram_peek((0x0c00 + tmp1_U8));
            s_a = tmp1_U8;
  /*$1E4A*/ ram_poke(0x0001, tmp1_U8);
  /*$1E4C*/ s_y = ram_peek(0x0006);
bb_2:
  /*$1E4E*/ CYCLES(0x1e4e, 4);
            branchTarget = true;
            if (!(s_y >= 0x28))
              goto bb_4;
  /*$1E50*/ CYCLES_EDGE(0x1e50, 1);
            branchTarget = true;
            goto bb_5;
bb_4:
  /*$1E52*/ CYCLES(0x1e52, 4);
            tmp1_U8 = peek(ram_peek16(0x1e53));
  /*$1E55*/ CYCLES(0x1e55, 11);
            tmp2_U8 = s_y;
            tmp3_U8 = peek((ram_peek16al(0x0000) + tmp2_U8));
            tmp3_U8 = tmp1_U8 ^ tmp3_U8;
            s_a = tmp3_U8;
  /*$1E57*/ poke((ram_peek16al(0x0000) + tmp2_U8), tmp3_U8);
bb_5:
  /*$1E59*/ CYCLES(0x1e59, 8);
            tmp2_U8 = (uint8_t)(ram_peek(0x1e53) + 0x01);
            ram_poke(0x1e53, tmp2_U8);
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_7;
  /*$1E5C*/ CYCLES_EDGE(0x1e5c, 1);
            branchTarget = true;
            goto bb_8;
bb_7:
  /*$1E5E*/ CYCLES(0x1e5e, 6);
            ram_poke(0x1e54, (uint8_t)(ram_peek(0x1e54) + 0x01));
bb_8:
  /*$1E61*/ CYCLES(0x1e61, 7);
            tmp2_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp2_U8;
            branchTarget = true;
            if (!(tmp2_U8 != ram_peek(0x0007)))
              goto bb_10;
  /*$1E64*/ CYCLES_EDGE(0x1e64, 1);
            branchTarget = true;
            goto bb_2;
bb_10:
  /*$1E66*/ CYCLES(0x1e66, 7);
            tmp2_U8 = (uint8_t)(s_x + 0x01);
            s_x = tmp2_U8;
  /*$1E67*/ tmp3_U8 = ram_peek(0x0005);
            tmp1_U8 = tmp2_U8 != tmp3_U8;
            s_status_not_z = tmp1_U8;
            s_status_c = (tmp2_U8 >= tmp3_U8);
            s_status_n = ((uint8_t)(tmp2_U8 - tmp3_U8) & 0x80);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_12;
  /*$1E69*/ CYCLES_EDGE(0x1e69, 1);
            branchTarget = true;
            goto bb_1;
bb_12:
  /*$1E6B*/ CYCLES(0x1e6b, 6);
            if (ret_addr) pop16(); return;
}


void func_1e80(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint16_t tmp2_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1E80*/ CYCLES(0x1e80, 31);
  /*$1E82*/ s_a = (uint8_t)(ram_peek(0x0008) << 0x01);
  /*$1E83*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_2;
  /*$1E84*/ s_a = (uint8_t)((s_a + 0x00a0) + s_status_c);
            goto bb_3;
bb_2:
  /*$1E84*/ s_a = ((uint8_t)adc_dec16(s_a, 0xa0, s_status_c));
bb_3:
  /*$1E86*/ ram_poke(0x1e54, s_a);
  /*$1E8B*/ ram_poke(0x1e53, 0x00);
  /*$1E90*/ ram_poke(0x1e55, 0x11);
  /*$1E93*/ func_1e40(0x1e95);
  /*$1E96*/ CYCLES(0x1e96, 6);
            func_1da0(0x1e98);
  /*$1E99*/ CYCLES(0x1e99, 31);
  /*$1E9B*/ s_a = (uint8_t)(ram_peek(0x0008) << 0x01);
  /*$1E9C*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_5;
  /*$1E9D*/ tmp1_U16 = s_a;
            tmp2_U16 = (tmp1_U16 + 0x00a0) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp2_U16, (uint8_t)tmp1_U16, (uint8_t)0x00a0);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_6;
bb_5:
  /*$1E9D*/ tmp2_U16 = adc_dec16(s_a, 0xa0, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            s_status_v = (((uint8_t)(tmp2_U16 >> 8) & 0x40) != 0);
bb_6:
  /*$1E9F*/ ram_poke(0x1e54, s_a);
  /*$1EA4*/ ram_poke(0x1e53, 0x00);
  /*$1EA7*/ s_a = 0x51;
  /*$1EA9*/ ram_poke(0x1e55, 0x51);
  /*$1EAC*/ func_1e40(0x1eae);
  /*$1EAF*/ CYCLES(0x1eaf, 6);
            if (ret_addr) pop16(); return;
}


void func_1ef3(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1EF3*/ CYCLES(0x1ef3, 8);
            s_a = 0x00;
  /*$1EF5*/ rom_mon_wait(0x1ef7);
  /*$1EF8*/ CYCLES(0x1ef8, 11);
  /*$1EFA*/ ram_poke(0x0000, 0x40);
  /*$1EFC*/ if (ret_addr) pop16(); return;
}


void func_1fd0(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$1FD0*/ CYCLES(0x1fd0, 5);
            branchTarget = true;
            if (!ram_peek(0x00a0))
              goto bb_2;
  /*$1FD2*/ CYCLES_EDGE(0x1fd2, 1);
  /*$1FD9*/ CYCLES(0x1fd9, 2);
            s_a = 0x50;
            goto bb_3;
bb_2:
  /*$1FD4*/ CYCLES(0x1fd4, 4);
            s_a = 0x20;
  /*$1FD6*/ CYCLES_EDGE(0x1fd6, 1);
            branchTarget = true;
bb_3:
  /*$1FDB*/ CYCLES(0x1fdb, 20);
            ram_poke(0x00e0, s_a);
  /*$1FDD*/ tmp1_U8 = ram_peek(0x0086);
            s_x = tmp1_U8;
  /*$1FE2*/ ram_poke(0x3821, ram_peek((0x37d8 + tmp1_U8)));
  /*$1FE5*/ if (ret_addr) pop16(); return;
}


void func_2200(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2200*/ CYCLES(0x2200, 3);
            s_x = ram_peek(0x0004);
bb_1:
  /*$2202*/ CYCLES(0x2202, 17);
            tmp1_U8 = s_x;
  /*$2205*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$220A*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$220C*/ s_y = ram_peek(0x0006);
bb_2:
  /*$220E*/ CYCLES(0x220e, 4);
            branchTarget = true;
            if (!(s_y >= 0x28))
              goto bb_4;
  /*$2210*/ CYCLES_EDGE(0x2210, 1);
            branchTarget = true;
            goto bb_5;
bb_4:
  /*$2212*/ CYCLES(0x2212, 10);
            tmp1_U8 = peek(ram_peek16(0x2213));
  /*$2215*/ poke((ram_peek16al(0x0000) + s_y), tmp1_U8);
bb_5:
  /*$2217*/ CYCLES(0x2217, 13);
            ram_poke(0x2213, (uint8_t)(ram_peek(0x2213) + 0x01));
  /*$221A*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 != ram_peek(0x0007)))
              goto bb_7;
  /*$221D*/ CYCLES_EDGE(0x221d, 1);
            branchTarget = true;
            goto bb_2;
bb_7:
  /*$221F*/ CYCLES(0x221f, 6);
            s_a = 0x00;
            branchTarget = true;
            if (!(s_y >= 0x28))
              goto bb_9;
  /*$2223*/ CYCLES_EDGE(0x2223, 1);
            branchTarget = true;
            goto bb_10;
bb_9:
  /*$2225*/ CYCLES(0x2225, 6);
            poke((ram_peek16al(0x0000) + s_y), s_a);
bb_10:
  /*$2227*/ CYCLES(0x2227, 7);
            tmp1_U8 = (uint8_t)(s_x + 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= ram_peek(0x0005)))
              goto bb_12;
  /*$222A*/ CYCLES_EDGE(0x222a, 1);
            branchTarget = true;
            goto bb_1;
bb_12:
  /*$222C*/ CYCLES(0x222c, 15);
            poke(ram_peek16(0x222d), s_a);
  /*$2231*/ ram_poke(0x000a, 0x01);
  /*$2233*/ if (ret_addr) pop16(); return;
}


void func_2240(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2240*/ CYCLES(0x2240, 10);
  /*$2242*/ ram_poke(0x0008, 0x00);
  /*$2246*/ ram_poke(0x0009, 0x03);
bb_1:
  /*$2248*/ CYCLES(0x2248, 9);
            s_y = 0x00;
  /*$224A*/ tmp1_U8 = peek(ram_peek16al(0x0008));
            branchTarget = true;
            if (tmp1_U8)
              goto bb_3;
  /*$224C*/ CYCLES_EDGE(0x224c, 1);
            branchTarget = true;
            goto bb_33;
bb_3:
  /*$224E*/ CYCLES(0x224e, 80);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$224F*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
  /*$2251*/ ram_poke(0x0004, tmp1_U8);
  /*$2253*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_5;
  /*$2254*/ s_a = (uint8_t)((s_a + 0x0008) + s_status_c);
            goto bb_6;
bb_5:
  /*$2254*/ s_a = ((uint8_t)adc_dec16(s_a, 0x08, s_status_c));
bb_6:
  /*$2256*/ ram_poke(0x0005, s_a);
  /*$2258*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$2259*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
  /*$225B*/ ram_poke(0x0006, tmp1_U8);
  /*$225D*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_8;
  /*$225E*/ s_a = (uint8_t)((s_a + 0x0004) + s_status_c);
            goto bb_9;
bb_8:
  /*$225E*/ s_a = ((uint8_t)adc_dec16(s_a, 0x04, s_status_c));
bb_9:
  /*$2260*/ ram_poke(0x0007, s_a);
  /*$2262*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
  /*$2263*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
  /*$2265*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_11;
  /*$2266*/ s_a = (uint8_t)((s_a + 0x0070) + s_status_c);
            goto bb_12;
bb_11:
  /*$2266*/ s_a = ((uint8_t)adc_dec16(s_a, 0x70, s_status_c));
bb_12:
  /*$2268*/ ram_poke(0x2214, s_a);
  /*$226C*/ tmp1_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(s_y + 0x01)));
  /*$2276*/ ram_poke(0x2213, 0x40);
  /*$2279*/ func_2200(0x227b);
  /*$227C*/ CYCLES(0x227c, 30);
  /*$227E*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0005));
  /*$2280*/ ram_poke(0x0000, tmp1_U8);
  /*$2282*/ s_y = 0x03;
  /*$2284*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0003));
            s_a = tmp1_U8;
  /*$2286*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_14;
  /*$2287*/ tmp1_U8 = (uint8_t)((s_a - ram_peek(0x0000)) - (uint8_t)(0x01 - s_status_c));
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            goto bb_15;
bb_14:
  /*$2287*/ tmp3_U16 = sbc_dec16(s_a, ram_peek(0x0000), s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_n = ((uint8_t)(tmp3_U16 >> 8) & 0x80);
bb_15:
  /*$2289*/ poke((ram_peek16al(0x0008) + s_y), s_a);
            branchTarget = true;
            if (s_status_n)
              goto bb_17;
  /*$228B*/ CYCLES_EDGE(0x228b, 1);
            branchTarget = true;
            goto bb_39;
bb_17:
  /*$228D*/ CYCLES(0x228d, 31);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_19;
  /*$228E*/ s_a = (uint8_t)((s_a + 0x0007) + s_status_c);
            goto bb_20;
bb_19:
  /*$228E*/ s_a = ((uint8_t)adc_dec16(s_a, 0x07, s_status_c));
bb_20:
  /*$2290*/ tmp1_U8 = s_y;
            poke((ram_peek16al(0x0008) + tmp1_U8), s_a);
  /*$2292*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_y = tmp1_U8;
  /*$2293*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
            s_a = tmp1_U8;
  /*$2295*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_22;
  /*$2296*/ s_a = (uint8_t)((s_a - 0x0001) - (uint8_t)(0x01 - s_status_c));
            goto bb_23;
bb_22:
  /*$2296*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x01, s_status_c));
bb_23:
  /*$2298*/ tmp1_U8 = s_a;
            poke((ram_peek16al(0x0008) + s_y), tmp1_U8);
            branchTarget = true;
            if ((tmp1_U8 >= 0xf0))
              goto bb_25;
  /*$229C*/ CYCLES_EDGE(0x229c, 1);
            branchTarget = true;
            goto bb_39;
bb_25:
  /*$229E*/ CYCLES(0x229e, 4);
            branchTarget = true;
            if (!(s_a >= 0xf9))
              goto bb_27;
  /*$22A0*/ CYCLES_EDGE(0x22a0, 1);
            branchTarget = true;
            goto bb_39;
bb_27:
  /*$22A2*/ CYCLES(0x22a2, 21);
            s_y = 0x00;
  /*$22A4*/ tmp1_U8 = peek(ram_peek16al(0x0008));
            s_a = tmp1_U8;
  /*$22A6*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_29;
  /*$22A7*/ s_a = (uint8_t)((s_a + 0x0001) + s_status_c);
            goto bb_30;
bb_29:
  /*$22A7*/ s_a = ((uint8_t)adc_dec16(s_a, 0x01, s_status_c));
bb_30:
  /*$22A9*/ tmp1_U8 = s_a;
            poke((ram_peek16al(0x0008) + s_y), tmp1_U8);
            branchTarget = true;
            if ((tmp1_U8 >= 0x08))
              goto bb_32;
  /*$22AD*/ CYCLES_EDGE(0x22ad, 1);
  /*$22B8*/ CYCLES(0x22b8, 10);
            s_x = 0x10;
  /*$22BA*/ s_y = 0x00;
  /*$22BC*/ func_2e00(0x22be);
  /*$22BF*/ CYCLES(0x22bf, 2);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_34;
            else
              goto bb_35;
bb_32:
  /*$22AF*/ CYCLES(0x22af, 10);
            s_y = 0x00;
  /*$22B3*/ poke(ram_peek16al(0x0008), 0x00);
bb_33:
  /*$22B5*/ CYCLES(0x22b5, 3);
            branchTarget = true;
            goto bb_51;
bb_34:
  /*$22BF*/ CYCLES_EDGE(0x22bf, 1);
            branchTarget = true;
            goto bb_38;
bb_35:
  /*$22C1*/ CYCLES(0x22c1, 2);
            branchTarget = true;
            if (s_status_not_z)
              goto bb_37;
  /*$22C1*/ CYCLES_EDGE(0x22c1, 1);
            branchTarget = true;
            goto bb_38;
bb_37:
  /*$22C3*/ CYCLES(0x22c3, 18);
  /*$22CB*/ poke((ram_peek16al(0x00a0) + 0x0001), ram_peek((0x21f8 + (s_a >> 0x02))));
  /*$22CD*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x22cd, 0x22cd, 0x03); error_handler(0x22cd);
bb_38:
  /*$22CC*/ CYCLES(0x22cc, 13);
            s_y = 0x02;
  /*$22D0*/ poke((ram_peek16al(0x0008) + 0x0002), 0x28);
            branchTarget = true;
            goto bb_51;
bb_39:
  /*$22D5*/ CYCLES(0x22d5, 21);
            s_y = 0x06;
  /*$22D7*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0006));
            s_a = tmp1_U8;
  /*$22D9*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_41;
  /*$22DA*/ s_a = (uint8_t)((s_a + 0x0001) + s_status_c);
            goto bb_42;
bb_41:
  /*$22DA*/ s_a = ((uint8_t)adc_dec16(s_a, 0x01, s_status_c));
bb_42:
  /*$22DC*/ tmp1_U8 = s_a;
            poke((ram_peek16al(0x0008) + s_y), tmp1_U8);
            branchTarget = true;
            if ((tmp1_U8 >= 0x03))
              goto bb_44;
  /*$22E0*/ CYCLES_EDGE(0x22e0, 1);
            branchTarget = true;
            goto bb_51;
bb_44:
  /*$22E2*/ CYCLES(0x22e2, 23);
  /*$22E4*/ poke((ram_peek16al(0x0008) + s_y), 0x00);
  /*$22E6*/ s_y = 0x04;
  /*$22E8*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0004));
            s_a = tmp1_U8;
  /*$22EA*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_46;
  /*$22EB*/ s_a = (uint8_t)((s_a + 0x0001) + s_status_c);
            goto bb_47;
bb_46:
  /*$22EB*/ s_a = ((uint8_t)adc_dec16(s_a, 0x01, s_status_c));
bb_47:
            branchTarget = true;
            if ((s_a >= 0x03))
              goto bb_49;
  /*$22EF*/ CYCLES_EDGE(0x22ef, 1);
            branchTarget = true;
            goto bb_50;
bb_49:
  /*$22F1*/ CYCLES(0x22f1, 2);
            s_a = 0x00;
bb_50:
  /*$22F3*/ CYCLES(0x22f3, 9);
            poke((ram_peek16al(0x0008) + s_y), s_a);
            branchTarget = true;
bb_51:
  /*$2310*/ CYCLES(0x2310, 11);
            s_a = ram_peek(0x0008);
  /*$2312*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_53;
  /*$2313*/ tmp2_U16 = s_a;
            tmp3_U16 = (tmp2_U16 + 0x0008) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0x0008);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_54;
bb_53:
  /*$2313*/ tmp3_U16 = adc_dec16(s_a, 0x08, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_54:
  /*$2315*/ tmp1_U8 = s_a >= 0x40;
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8) {
    /*$2317*/ CYCLES_EDGE(0x2317, 1);
    /*$231E*/ CYCLES(0x231e, 6);
              if (ret_addr) pop16(); return;
            }
  /*$2319*/ CYCLES(0x2319, 6);
            ram_poke(0x0008, s_a);
            branchTarget = true;
            goto bb_1;
}


void func_2380(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2380*/ CYCLES(0x2380, 2);
            s_y = 0x60;
bb_1:
  /*$2382*/ CYCLES(0x2382, 11);
  /*$2384*/ tmp1_U8 = s_y;
            ram_poke((0x0300 + tmp1_U8), 0x00);
  /*$2387*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_3;
  /*$2388*/ CYCLES_EDGE(0x2388, 1);
            branchTarget = true;
            goto bb_1;
bb_3:
  /*$238A*/ CYCLES(0x238a, 2);
            s_x = 0x00;
bb_4:
  /*$238C*/ CYCLES(0x238c, 60);
            tmp1_U8 = s_x;
  /*$238F*/ tmp2_U16 = tmp1_U8 << 0x03;
            tmp3_U8 = (uint8_t)tmp2_U16;
  /*$2393*/ ram_poke((0x0300 + (tmp2_U16 & 0x00ff)), 0x01);
  /*$239A*/ ram_poke((0x0300 + (uint8_t)(tmp3_U8 + 0x01)), ram_peek((0x2340 + tmp1_U8)));
  /*$23A1*/ ram_poke((0x0300 + (uint8_t)(tmp3_U8 + 0x02)), ram_peek((0x2350 + tmp1_U8)));
  /*$23AA*/ ram_poke((0x0300 + (uint8_t)(tmp3_U8 + 0x05)), ram_peek((0x2360 + tmp1_U8)));
  /*$23AD*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0x08))
              goto bb_6;
  /*$23B0*/ CYCLES_EDGE(0x23b0, 1);
            branchTarget = true;
            goto bb_4;
bb_6:
  /*$23B2*/ CYCLES(0x23b2, 2);
            s_y = 0x00;
bb_7:
  /*$23B4*/ CYCLES(0x23b4, 15);
            tmp1_U8 = s_y;
  /*$23B7*/ ram_poke((0x0340 + tmp1_U8), ram_peek((0x2320 + tmp1_U8)));
  /*$23BA*/ tmp3_U8 = (uint8_t)(tmp1_U8 + 0x01);
            s_y = tmp3_U8;
  /*$23BB*/ s_status_not_z = (tmp3_U8 != 0x1f);
            tmp3_U8 = tmp3_U8 >= 0x1f;
            s_status_c = tmp3_U8;
            s_status_n = ((uint8_t)(tmp1_U8 - 0x1e) & 0x80);
            branchTarget = true;
            if (tmp3_U8)
              goto bb_9;
  /*$23BD*/ CYCLES_EDGE(0x23bd, 1);
            branchTarget = true;
            goto bb_7;
bb_9:
  /*$23BF*/ CYCLES(0x23bf, 6);
            if (ret_addr) pop16(); return;
}


void func_2400(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;
  uint8_t tmp4_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2400*/ CYCLES(0x2400, 10);
  /*$2402*/ ram_poke(0x0008, 0x40);
  /*$2406*/ ram_poke(0x0009, 0x03);
bb_1:
  /*$2408*/ CYCLES(0x2408, 67);
  /*$240A*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0002));
  /*$2410*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$2415*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$2417*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
            s_x = tmp1_U8;
  /*$241B*/ ram_poke(0x0002, ram_peek((0x0d00 + tmp1_U8)));
  /*$2420*/ ram_poke(0x0003, ram_peek((0x0c00 + tmp1_U8)));
  /*$2423*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0003));
  /*$2425*/ s_y = tmp1_U8;
  /*$2428*/ poke((ram_peek16al(0x0000) + tmp1_U8), 0x00);
  /*$242A*/ poke((ram_peek16al(0x0002) + tmp1_U8), 0x00);
            branchTarget = true;
            if (ram_peek(0x000a))
              goto bb_3;
  /*$242E*/ CYCLES_EDGE(0x242e, 1);
            branchTarget = true;
            goto bb_24;
bb_3:
  /*$2430*/ CYCLES(0x2430, 9);
  /*$2432*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0004));
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_5;
  /*$2434*/ CYCLES_EDGE(0x2434, 1);
  /*$2452*/ CYCLES(0x2452, 20);
  /*$2454*/ tmp4_U8 = peek((ram_peek16al(0x0008) + 0x0002));
            s_a = tmp4_U8;
  /*$2456*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_15;
            else
              goto bb_14;
bb_5:
  /*$2436*/ CYCLES(0x2436, 20);
  /*$2438*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0002));
            s_a = tmp1_U8;
  /*$243A*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_7;
  /*$243B*/ s_a = (uint8_t)((s_a - 0x0001) - (uint8_t)(0x01 - s_status_c));
            goto bb_8;
bb_7:
  /*$243B*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x01, s_status_c));
bb_8:
  /*$243F*/ tmp1_U8 = peek(ram_peek16al(0x0008));
            branchTarget = true;
            if (!(s_a >= tmp1_U8))
              goto bb_10;
  /*$2441*/ CYCLES_EDGE(0x2441, 1);
            branchTarget = true;
            goto bb_13;
bb_10:
  /*$2443*/ CYCLES(0x2443, 4);
            s_y = 0x04;
  /*$2445*/ s_status_not_z = 0x01;
            s_a = 0x01;
bb_11:
  /*$2447*/ CYCLES(0x2447, 8);
            poke((ram_peek16al(0x0008) + s_y), s_a);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_13;
  /*$2449*/ CYCLES_EDGE(0x2449, 1);
            branchTarget = true;
            goto bb_19;
bb_13:
  /*$244B*/ CYCLES(0x244b, 11);
  /*$244D*/ poke((ram_peek16al(0x0008) + 0x0002), s_a);
            branchTarget = true;
            goto bb_19;
bb_14:
  /*$2457*/ s_a = (uint8_t)((s_a + 0x0001) + s_status_c);
            goto bb_16;
bb_15:
  /*$2457*/ s_a = ((uint8_t)adc_dec16(s_a, 0x01, s_status_c));
bb_16:
  /*$245B*/ tmp4_U8 = peek((ram_peek16al(0x0008) + 0x0001));
            branchTarget = true;
            if ((s_a >= tmp4_U8))
              goto bb_18;
  /*$245D*/ CYCLES_EDGE(0x245d, 1);
            branchTarget = true;
            goto bb_13;
bb_18:
  /*$245F*/ CYCLES(0x245f, 6);
            s_a = 0x00;
  /*$2461*/ s_status_not_z = 0x04;
            s_y = 0x04;
  /*$2463*/ CYCLES_EDGE(0x2463, 1);
            branchTarget = true;
            goto bb_11;
bb_19:
  /*$2465*/ CYCLES(0x2465, 11);
            s_y = 0x02;
  /*$2467*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0002));
            s_a = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0x20))
              goto bb_21;
  /*$246B*/ CYCLES_EDGE(0x246b, 1);
            branchTarget = true;
            goto bb_23;
bb_21:
  /*$246D*/ CYCLES(0x246d, 4);
            branchTarget = true;
            if ((s_a >= 0x88))
              goto bb_23;
  /*$246F*/ CYCLES_EDGE(0x246f, 1);
            branchTarget = true;
            goto bb_24;
bb_23:
  /*$2471*/ CYCLES(0x2471, 55);
            tmp1_U8 = s_a;
  /*$2475*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$247A*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$247C*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
            s_x = tmp1_U8;
  /*$2480*/ ram_poke(0x0002, ram_peek((0x0d00 + tmp1_U8)));
  /*$2485*/ ram_poke(0x0003, ram_peek((0x0c00 + tmp1_U8)));
  /*$2488*/ tmp1_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(s_y + 0x01)));
  /*$248A*/ s_y = tmp1_U8;
  /*$248D*/ poke((ram_peek16al(0x0000) + tmp1_U8), 0x28);
  /*$248F*/ poke((ram_peek16al(0x0002) + tmp1_U8), 0x28);
bb_24:
  /*$2491*/ CYCLES(0x2491, 14);
            s_a = ram_peek(0x0008);
  /*$2493*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_26;
  /*$2494*/ tmp2_U16 = s_a;
            tmp3_U16 = (tmp2_U16 + 0x0005) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0x0005);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_27;
bb_26:
  /*$2494*/ tmp3_U16 = adc_dec16(s_a, 0x05, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_27:
  /*$2496*/ tmp1_U8 = s_a;
            ram_poke(0x0008, tmp1_U8);
  /*$2498*/ s_status_not_z = (tmp1_U8 != 0x54);
            tmp4_U8 = tmp1_U8 >= 0x54;
            s_status_c = tmp4_U8;
            s_status_n = ((uint8_t)(tmp1_U8 - 0x54) & 0x80);
            branchTarget = true;
            if (tmp4_U8) {
    /*$249A*/ CYCLES_EDGE(0x249a, 1);
    /*$249F*/ CYCLES(0x249f, 6);
              if (ret_addr) pop16(); return;
            }
  /*$249C*/ CYCLES(0x249c, 3);
            branchTarget = true;
            goto bb_1;
}


void func_24a0(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$24A0*/ CYCLES(0x24a0, 6);
            func_2240(0x24a2);
  /*$24A3*/ CYCLES(0x24a3, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x0020) - 0x01);
            ram_poke(0x0020, tmp1_U8);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_2;
  /*$24A5*/ CYCLES_EDGE(0x24a5, 1);
            branchTarget = true;
            goto bb_3;
bb_2:
  /*$24A7*/ CYCLES(0x24a7, 11);
  /*$24A9*/ ram_poke(0x0020, 0x02);
  /*$24AB*/ func_2400(0x24ad);
            branchTarget = true;
bb_3:
  /*$24AE*/ CYCLES(0x24ae, 6);
            if (ret_addr) pop16(); return;
}


void func_2500(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2500*/ CYCLES(0x2500, 3);
            s_x = ram_peek(0x0004);
bb_1:
  /*$2502*/ CYCLES(0x2502, 18);
            tmp1_U8 = s_x;
  /*$2505*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$250A*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$250C*/ s_y = 0x27;
  /*$250E*/ s_a = 0x00;
bb_2:
  /*$2510*/ CYCLES(0x2510, 10);
            tmp1_U8 = s_y;
            poke((ram_peek16al(0x0000) + tmp1_U8), s_a);
  /*$2512*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_4;
  /*$2513*/ CYCLES_EDGE(0x2513, 1);
            branchTarget = true;
            goto bb_2;
bb_4:
  /*$2515*/ CYCLES(0x2515, 7);
            tmp1_U8 = (uint8_t)(s_x + 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= ram_peek(0x0005)))
              goto bb_6;
  /*$2518*/ CYCLES_EDGE(0x2518, 1);
            branchTarget = true;
            goto bb_1;
bb_6:
  /*$251A*/ CYCLES(0x251a, 3);
  /*$3770*/ CYCLES(0x3770, 51);
  /*$3772*/ ram_poke(0x00b0, 0x9c);
  /*$3776*/ ram_poke(0x00b3, 0x90);
  /*$377A*/ ram_poke(0x00b6, 0x98);
  /*$377E*/ ram_poke(0x00b1, 0x10);
  /*$3782*/ ram_poke(0x00b4, 0x1c);
  /*$3786*/ ram_poke(0x00b7, 0x24);
  /*$378A*/ ram_poke(0x00b2, 0x00);
  /*$378E*/ ram_poke(0x00b5, 0x03);
  /*$3790*/ s_a = 0x05;
  /*$3792*/ ram_poke(0x00b8, 0x05);
  /*$3794*/ if (ret_addr) pop16(); return;
}


void func_2520(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2520*/ CYCLES(0x2520, 14);
  /*$2522*/ ram_poke(0x2539, 0x00);
  /*$2527*/ ram_poke(0x253a, 0x77);
  /*$252A*/ s_x = 0x78;
bb_1:
  /*$252C*/ CYCLES(0x252c, 16);
            tmp1_U8 = s_x;
  /*$252F*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$2534*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$2536*/ s_y = 0x00;
bb_2:
  /*$2538*/ CYCLES(0x2538, 18);
            tmp1_U8 = peek(ram_peek16(0x2539));
  /*$253B*/ poke((ram_peek16al(0x0000) + s_y), tmp1_U8);
  /*$253D*/ tmp1_U8 = (uint8_t)(ram_peek(0x2539) + 0x01);
            ram_poke(0x2539, tmp1_U8);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_4;
  /*$2540*/ CYCLES_EDGE(0x2540, 1);
            branchTarget = true;
            goto bb_5;
bb_4:
  /*$2542*/ CYCLES(0x2542, 6);
            ram_poke(0x253a, (uint8_t)(ram_peek(0x253a) + 0x01));
bb_5:
  /*$2545*/ CYCLES(0x2545, 6);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0x28))
              goto bb_7;
  /*$2548*/ CYCLES_EDGE(0x2548, 1);
            branchTarget = true;
            goto bb_2;
bb_7:
  /*$254A*/ CYCLES(0x254a, 6);
            tmp1_U8 = (uint8_t)(s_x + 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0xa0))
              goto bb_9;
  /*$254D*/ CYCLES_EDGE(0x254d, 1);
            branchTarget = true;
            goto bb_1;
bb_9:
  /*$254F*/ CYCLES(0x254f, 6);
            if (ret_addr) pop16(); return;
}


void func_2550(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2550*/ CYCLES(0x2550, 10);
  /*$2552*/ ram_poke(0x0006, 0x77);
  /*$2556*/ ram_poke(0x0007, 0x9e);
bb_1:
  /*$2558*/ CYCLES(0x2558, 3);
            s_x = ram_peek(0x0007);
bb_2:
  /*$255A*/ CYCLES(0x255a, 32);
            tmp1_U8 = s_x;
  /*$255D*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$2562*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$2564*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_x = tmp1_U8;
  /*$2568*/ ram_poke(0x0002, ram_peek((0x0d00 + tmp1_U8)));
  /*$256D*/ ram_poke(0x0003, ram_peek((0x0c00 + tmp1_U8)));
  /*$256F*/ s_y = 0x13;
bb_3:
  /*$2571*/ CYCLES(0x2571, 17);
            tmp1_U8 = s_y;
            tmp2_U8 = peek((ram_peek16al(0x0002) + tmp1_U8));
  /*$2573*/ poke((ram_peek16al(0x0000) + tmp1_U8), tmp2_U8);
  /*$2575*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0x1e))
              goto bb_5;
  /*$2578*/ CYCLES_EDGE(0x2578, 1);
            branchTarget = true;
            goto bb_3;
bb_5:
  /*$257A*/ CYCLES(0x257a, 5);
            branchTarget = true;
            if (!(s_x != ram_peek(0x0006)))
              goto bb_7;
  /*$257C*/ CYCLES_EDGE(0x257c, 1);
            branchTarget = true;
            goto bb_2;
bb_7:
  /*$257E*/ CYCLES(0x257e, 13);
            ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) + 0x01));
  /*$2582*/ func_2770(0x2584);
  /*$2585*/ CYCLES(0x2585, 7);
            tmp1_U8 = ram_peek(0x0006);
            s_a = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0x89))
              goto bb_9;
  /*$2589*/ CYCLES_EDGE(0x2589, 1);
            branchTarget = true;
            goto bb_1;
bb_9:
  /*$258B*/ CYCLES(0x258b, 6);
            if (ret_addr) pop16(); return;
}


void func_2590(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2590*/ CYCLES(0x2590, 16);
  /*$2592*/ ram_poke(0x0004, 0x00);
  /*$2596*/ ram_poke(0x0005, 0xa8);
  /*$2598*/ func_2500(0x259a);
  /*$259B*/ CYCLES(0x259b, 6);
            func_2520(0x259d);
  /*$259E*/ CYCLES(0x259e, 18);
  /*$25A3*/ ram_poke(0x0002, ram_peek(0x0d9f));
  /*$25A8*/ ram_poke(0x0003, ram_peek(0x0c9f));
  /*$25AA*/ s_x = 0xa0;
bb_1:
  /*$25AC*/ CYCLES(0x25ac, 16);
            tmp1_U8 = s_x;
  /*$25AF*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$25B4*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$25B6*/ s_y = 0x27;
bb_2:
  /*$25B8*/ CYCLES(0x25b8, 15);
            tmp1_U8 = s_y;
            tmp2_U8 = peek((ram_peek16al(0x0002) + tmp1_U8));
            s_a = tmp2_U8;
  /*$25BA*/ poke((ram_peek16al(0x0000) + tmp1_U8), tmp2_U8);
  /*$25BC*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_4;
  /*$25BD*/ CYCLES_EDGE(0x25bd, 1);
            branchTarget = true;
            goto bb_2;
bb_4:
  /*$25BF*/ CYCLES(0x25bf, 6);
            tmp1_U8 = (uint8_t)(s_x + 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0xa7))
              goto bb_6;
  /*$25C2*/ CYCLES_EDGE(0x25c2, 1);
            branchTarget = true;
            goto bb_1;
bb_6:
  /*$25C4*/ CYCLES(0x25c4, 6);
            func_2550(0x25c6);
  /*$25C7*/ CYCLES(0x25c7, 6);
            func_2680(0x25c9);
  /*$25CA*/ CYCLES(0x25ca, 6);
            func_2750(0x25cc);
  /*$25CD*/ CYCLES(0x25cd, 6);
            func_2750(0x25cf);
  /*$25D0*/ CYCLES(0x25d0, 6);
            func_2750(0x25d2);
  /*$25D3*/ CYCLES(0x25d3, 6);
            func_2750(0x25d5);
  /*$25D6*/ CYCLES(0x25d6, 6);
            func_26c0(0x25d8);
  /*$25D9*/ CYCLES(0x25d9, 6);
            func_2800(0x25db);
  /*$25DC*/ CYCLES(0x25dc, 6);
            if (ret_addr) pop16(); return;
}


void func_2600(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2600*/ CYCLES(0x2600, 2);
            s_x = 0x8b;
bb_1:
  /*$2602*/ CYCLES(0x2602, 17);
            tmp1_U8 = s_x;
  /*$2605*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$260A*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$260C*/ s_y = ram_peek(0x0006);
bb_2:
  /*$260E*/ CYCLES(0x260e, 23);
            tmp1_U8 = peek(ram_peek16(0x260f));
            s_a = tmp1_U8;
  /*$2611*/ tmp2_U8 = s_y;
            poke((ram_peek16al(0x0000) + tmp2_U8), tmp1_U8);
  /*$2613*/ ram_poke(0x260f, (uint8_t)(ram_peek(0x260f) + 0x01));
  /*$2616*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
            s_y = tmp2_U8;
            branchTarget = true;
            if ((tmp2_U8 >= ram_peek(0x0007)))
              goto bb_4;
  /*$2619*/ CYCLES_EDGE(0x2619, 1);
            branchTarget = true;
            goto bb_2;
bb_4:
  /*$261B*/ CYCLES(0x261b, 6);
            tmp2_U8 = (uint8_t)(s_x + 0x01);
            s_x = tmp2_U8;
            branchTarget = true;
            if ((tmp2_U8 >= 0x9f))
              goto bb_6;
  /*$261E*/ CYCLES_EDGE(0x261e, 1);
            branchTarget = true;
            goto bb_1;
bb_6:
  /*$2620*/ CYCLES(0x2620, 6);
            if (ret_addr) pop16(); return;
}


void func_2680(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2680*/ CYCLES(0x2680, 5);
  /*$2682*/ ram_poke(0x0008, 0x00);
bb_1:
  /*$2684*/ CYCLES(0x2684, 39);
            tmp1_U8 = ram_peek(0x0008);
            s_x = tmp1_U8;
  /*$2689*/ ram_poke(0x260f, ram_peek((0x2630 + tmp1_U8)));
  /*$268F*/ ram_poke(0x2610, ram_peek((0x2638 + tmp1_U8)));
  /*$2692*/ tmp1_U8 = ram_peek((0x2640 + tmp1_U8));
            s_a = tmp1_U8;
  /*$2695*/ ram_poke(0x0006, tmp1_U8);
  /*$2697*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_3;
  /*$2698*/ s_a = (uint8_t)((s_a + 0x0003) + s_status_c);
            goto bb_4;
bb_3:
  /*$2698*/ s_a = ((uint8_t)adc_dec16(s_a, 0x03, s_status_c));
bb_4:
  /*$269A*/ ram_poke(0x0007, s_a);
  /*$269C*/ func_2600(0x269e);
  /*$269F*/ CYCLES(0x269f, 8);
  /*$26A1*/ func_2790(0x26a3);
  /*$26A4*/ CYCLES(0x26a4, 12);
            ram_poke(0x0008, (uint8_t)(ram_peek(0x0008) + 0x01));
  /*$26A6*/ tmp1_U8 = ram_peek(0x0008);
            s_a = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0x05))
              goto bb_6;
  /*$26AA*/ CYCLES_EDGE(0x26aa, 1);
            branchTarget = true;
            goto bb_1;
bb_6:
  /*$26AC*/ CYCLES(0x26ac, 6);
            if (ret_addr) pop16(); return;
}


void func_26c0(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$26C0*/ CYCLES(0x26c0, 5);
  /*$26C2*/ ram_poke(0x0008, 0x03);
bb_1:
  /*$26C4*/ CYCLES(0x26c4, 39);
            tmp1_U8 = ram_peek(0x0008);
  /*$26C9*/ ram_poke(0x260f, ram_peek((0x2630 + tmp1_U8)));
  /*$26CF*/ ram_poke(0x2610, ram_peek((0x2638 + tmp1_U8)));
  /*$26D2*/ tmp1_U8 = ram_peek((0x2640 + tmp1_U8));
            s_a = tmp1_U8;
  /*$26D5*/ ram_poke(0x0006, tmp1_U8);
  /*$26D7*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_3;
  /*$26D8*/ s_a = (uint8_t)((s_a + 0x0003) + s_status_c);
            goto bb_4;
bb_3:
  /*$26D8*/ s_a = ((uint8_t)adc_dec16(s_a, 0x03, s_status_c));
bb_4:
  /*$26DA*/ ram_poke(0x0007, s_a);
  /*$26DC*/ func_2600(0x26de);
  /*$26DF*/ CYCLES(0x26df, 2);
            s_x = 0x8b;
bb_5:
  /*$26E1*/ CYCLES(0x26e1, 31);
            tmp1_U8 = s_x;
  /*$26E4*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$26E9*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$26EB*/ tmp2_U8 = ram_peek(0x0007);
            s_y = tmp2_U8;
  /*$26EF*/ poke((ram_peek16al(0x0000) + tmp2_U8), 0x00);
  /*$26F1*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0x9f))
              goto bb_7;
  /*$26F4*/ CYCLES_EDGE(0x26f4, 1);
            branchTarget = true;
            goto bb_5;
bb_7:
  /*$26F6*/ CYCLES(0x26f6, 8);
            s_a = 0x00;
  /*$26F8*/ func_2790(0x26fa);
  /*$26FB*/ CYCLES(0x26fb, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x0008) - 0x01);
            ram_poke(0x0008, tmp1_U8);
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_9;
  /*$26FD*/ CYCLES_EDGE(0x26fd, 1);
            branchTarget = true;
            goto bb_1;
bb_9:
  /*$26FF*/ CYCLES(0x26ff, 6);
            if (ret_addr) pop16(); return;
}


void func_2745(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2745*/ CYCLES(0x2745, 6);
            if (ret_addr) pop16(); return;
}


void func_2750(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2750*/ CYCLES(0x2750, 23);
  /*$2752*/ ram_poke(0x000a, 0x00);
  /*$2756*/ ram_poke(0x000b, 0x96);
  /*$275A*/ ram_poke(0x0004, 0xa0);
  /*$275E*/ ram_poke(0x0005, 0x98);
            branchTarget = true;
bb_1:
  /*$2700*/ CYCLES(0x2700, 2);
            s_y = 0x03;
bb_2:
  /*$2702*/ CYCLES(0x2702, 14);
            tmp1_U8 = s_y;
            tmp2_U8 = peek((ram_peek16al(0x000a) + tmp1_U8));
            s_a = tmp2_U8;
  /*$2704*/ ram_poke(tmp1_U8, tmp2_U8);
  /*$2707*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_4;
  /*$2708*/ CYCLES_EDGE(0x2708, 1);
            branchTarget = true;
            goto bb_2;
bb_4:
  /*$270A*/ CYCLES(0x270a, 6);
            s_y = ram_peek(0x0000);
  /*$270C*/ s_x = ram_peek(0x0001);
bb_5:
  /*$270E*/ CYCLES(0x270e, 6);
            func_2745(0x2710);
  /*$2711*/ CYCLES(0x2711, 4);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_7;
  /*$2712*/ CYCLES_EDGE(0x2712, 1);
            branchTarget = true;
            goto bb_5;
bb_7:
  /*$2714*/ CYCLES(0x2714, 4);
            tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_9;
  /*$2715*/ CYCLES_EDGE(0x2715, 1);
            branchTarget = true;
            goto bb_5;
bb_9:
  /*$2717*/ CYCLES(0x2717, 10);
            poke(ram_peek16al(0x2718), s_a);
  /*$271A*/ s_y = ram_peek(0x0002);
  /*$271C*/ s_x = ram_peek(0x0003);
bb_10:
  /*$271E*/ CYCLES(0x271e, 6);
            func_2745(0x2720);
  /*$2721*/ CYCLES(0x2721, 4);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_12;
  /*$2722*/ CYCLES_EDGE(0x2722, 1);
            branchTarget = true;
            goto bb_10;
bb_12:
  /*$2724*/ CYCLES(0x2724, 4);
            tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_14;
  /*$2725*/ CYCLES_EDGE(0x2725, 1);
            branchTarget = true;
            goto bb_10;
bb_14:
  /*$2727*/ CYCLES(0x2727, 16);
            poke(ram_peek16al(0x2728), s_a);
  /*$272A*/ s_a = ram_peek(0x000a);
  /*$272C*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_16;
  /*$272D*/ tmp1_U8 = (uint8_t)((s_a + 0x0004) + s_status_c);
            s_status_not_z = tmp1_U8;
            s_a = tmp1_U8;
            goto bb_17;
bb_16:
  /*$272D*/ tmp3_U16 = adc_dec16(s_a, 0x04, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_not_z = (~(uint8_t)(tmp3_U16 >> 8) & 2);
bb_17:
  /*$272F*/ ram_poke(0x000a, s_a);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_19;
  /*$2731*/ CYCLES_EDGE(0x2731, 1);
            branchTarget = true;
            goto bb_20;
bb_19:
  /*$2733*/ CYCLES(0x2733, 5);
            ram_poke(0x000b, (uint8_t)(ram_peek(0x000b) + 0x01));
bb_20:
  /*$2735*/ CYCLES(0x2735, 8);
            branchTarget = true;
            if (!(ram_peek(0x000a) != ram_peek(0x0004)))
              goto bb_22;
  /*$2739*/ CYCLES_EDGE(0x2739, 1);
            branchTarget = true;
            goto bb_1;
bb_22:
  /*$273B*/ CYCLES(0x273b, 8);
            tmp1_U8 = ram_peek(0x000b);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 != ram_peek(0x0005)))
              goto bb_24;
  /*$273F*/ CYCLES_EDGE(0x273f, 1);
            branchTarget = true;
            goto bb_1;
bb_24:
  /*$2741*/ CYCLES(0x2741, 6);
            if (ret_addr) pop16(); return;
}


void func_2770(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2770*/ CYCLES(0x2770, 23);
  /*$2772*/ ram_poke(0x000a, 0xa0);
  /*$2776*/ ram_poke(0x000b, 0x98);
  /*$277A*/ ram_poke(0x0004, 0xb0);
  /*$277E*/ ram_poke(0x0005, 0x98);
            branchTarget = true;
bb_1:
  /*$2700*/ CYCLES(0x2700, 2);
            s_y = 0x03;
bb_2:
  /*$2702*/ CYCLES(0x2702, 14);
            tmp1_U8 = s_y;
            tmp2_U8 = peek((ram_peek16al(0x000a) + tmp1_U8));
            s_a = tmp2_U8;
  /*$2704*/ ram_poke(tmp1_U8, tmp2_U8);
  /*$2707*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_4;
  /*$2708*/ CYCLES_EDGE(0x2708, 1);
            branchTarget = true;
            goto bb_2;
bb_4:
  /*$270A*/ CYCLES(0x270a, 6);
            s_y = ram_peek(0x0000);
  /*$270C*/ s_x = ram_peek(0x0001);
bb_5:
  /*$270E*/ CYCLES(0x270e, 6);
            func_2745(0x2710);
  /*$2711*/ CYCLES(0x2711, 4);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_7;
  /*$2712*/ CYCLES_EDGE(0x2712, 1);
            branchTarget = true;
            goto bb_5;
bb_7:
  /*$2714*/ CYCLES(0x2714, 4);
            tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_9;
  /*$2715*/ CYCLES_EDGE(0x2715, 1);
            branchTarget = true;
            goto bb_5;
bb_9:
  /*$2717*/ CYCLES(0x2717, 10);
            poke(ram_peek16al(0x2718), s_a);
  /*$271A*/ s_y = ram_peek(0x0002);
  /*$271C*/ s_x = ram_peek(0x0003);
bb_10:
  /*$271E*/ CYCLES(0x271e, 6);
            func_2745(0x2720);
  /*$2721*/ CYCLES(0x2721, 4);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_12;
  /*$2722*/ CYCLES_EDGE(0x2722, 1);
            branchTarget = true;
            goto bb_10;
bb_12:
  /*$2724*/ CYCLES(0x2724, 4);
            tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_14;
  /*$2725*/ CYCLES_EDGE(0x2725, 1);
            branchTarget = true;
            goto bb_10;
bb_14:
  /*$2727*/ CYCLES(0x2727, 16);
            poke(ram_peek16al(0x2728), s_a);
  /*$272A*/ s_a = ram_peek(0x000a);
  /*$272C*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_16;
  /*$272D*/ tmp3_U16 = s_a;
            tmp4_U16 = (tmp3_U16 + 0x0004) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x0004);
            tmp1_U8 = (uint8_t)tmp4_U16;
            s_status_not_z = tmp1_U8;
            s_a = tmp1_U8;
            goto bb_17;
bb_16:
  /*$272D*/ tmp4_U16 = adc_dec16(s_a, 0x04, s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
            s_status_not_z = (~tmp1_U8 & 2);
            s_status_v = ((tmp1_U8 & 0x40) != 0);
bb_17:
  /*$272F*/ ram_poke(0x000a, s_a);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_19;
  /*$2731*/ CYCLES_EDGE(0x2731, 1);
            branchTarget = true;
            goto bb_20;
bb_19:
  /*$2733*/ CYCLES(0x2733, 5);
            ram_poke(0x000b, (uint8_t)(ram_peek(0x000b) + 0x01));
bb_20:
  /*$2735*/ CYCLES(0x2735, 8);
            branchTarget = true;
            if (!(ram_peek(0x000a) != ram_peek(0x0004)))
              goto bb_22;
  /*$2739*/ CYCLES_EDGE(0x2739, 1);
            branchTarget = true;
            goto bb_1;
bb_22:
  /*$273B*/ CYCLES(0x273b, 8);
            branchTarget = true;
            if (!(ram_peek(0x000b) != ram_peek(0x0005)))
              goto bb_24;
  /*$273F*/ CYCLES_EDGE(0x273f, 1);
            branchTarget = true;
            goto bb_1;
bb_24:
  /*$2741*/ CYCLES(0x2741, 6);
            if (ret_addr) pop16(); return;
}


void func_2790(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2790*/ CYCLES(0x2790, 23);
  /*$2792*/ ram_poke(0x000a, 0xa0);
  /*$2796*/ ram_poke(0x000b, 0x98);
  /*$279A*/ ram_poke(0x0004, 0x00);
  /*$279E*/ ram_poke(0x0005, 0x99);
            branchTarget = true;
bb_1:
  /*$2700*/ CYCLES(0x2700, 2);
            s_y = 0x03;
bb_2:
  /*$2702*/ CYCLES(0x2702, 14);
            tmp1_U8 = s_y;
            tmp2_U8 = peek((ram_peek16al(0x000a) + tmp1_U8));
            s_a = tmp2_U8;
  /*$2704*/ ram_poke(tmp1_U8, tmp2_U8);
  /*$2707*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_4;
  /*$2708*/ CYCLES_EDGE(0x2708, 1);
            branchTarget = true;
            goto bb_2;
bb_4:
  /*$270A*/ CYCLES(0x270a, 6);
            s_y = ram_peek(0x0000);
  /*$270C*/ s_x = ram_peek(0x0001);
bb_5:
  /*$270E*/ CYCLES(0x270e, 6);
            func_2745(0x2710);
  /*$2711*/ CYCLES(0x2711, 4);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_7;
  /*$2712*/ CYCLES_EDGE(0x2712, 1);
            branchTarget = true;
            goto bb_5;
bb_7:
  /*$2714*/ CYCLES(0x2714, 4);
            tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_9;
  /*$2715*/ CYCLES_EDGE(0x2715, 1);
            branchTarget = true;
            goto bb_5;
bb_9:
  /*$2717*/ CYCLES(0x2717, 10);
            poke(ram_peek16al(0x2718), s_a);
  /*$271A*/ s_y = ram_peek(0x0002);
  /*$271C*/ s_x = ram_peek(0x0003);
bb_10:
  /*$271E*/ CYCLES(0x271e, 6);
            func_2745(0x2720);
  /*$2721*/ CYCLES(0x2721, 4);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_12;
  /*$2722*/ CYCLES_EDGE(0x2722, 1);
            branchTarget = true;
            goto bb_10;
bb_12:
  /*$2724*/ CYCLES(0x2724, 4);
            tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_14;
  /*$2725*/ CYCLES_EDGE(0x2725, 1);
            branchTarget = true;
            goto bb_10;
bb_14:
  /*$2727*/ CYCLES(0x2727, 16);
            poke(ram_peek16al(0x2728), s_a);
  /*$272A*/ s_a = ram_peek(0x000a);
  /*$272C*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_16;
  /*$272D*/ tmp3_U16 = s_a;
            tmp4_U16 = (tmp3_U16 + 0x0004) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x0004);
            tmp1_U8 = (uint8_t)tmp4_U16;
            s_status_not_z = tmp1_U8;
            s_a = tmp1_U8;
            goto bb_17;
bb_16:
  /*$272D*/ tmp4_U16 = adc_dec16(s_a, 0x04, s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
            s_status_not_z = (~tmp2_U8 & 2);
            s_status_v = ((tmp2_U8 & 0x40) != 0);
bb_17:
  /*$272F*/ ram_poke(0x000a, s_a);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_19;
  /*$2731*/ CYCLES_EDGE(0x2731, 1);
            branchTarget = true;
            goto bb_20;
bb_19:
  /*$2733*/ CYCLES(0x2733, 5);
            ram_poke(0x000b, (uint8_t)(ram_peek(0x000b) + 0x01));
bb_20:
  /*$2735*/ CYCLES(0x2735, 8);
            branchTarget = true;
            if (!(ram_peek(0x000a) != ram_peek(0x0004)))
              goto bb_22;
  /*$2739*/ CYCLES_EDGE(0x2739, 1);
            branchTarget = true;
            goto bb_1;
bb_22:
  /*$273B*/ CYCLES(0x273b, 8);
            tmp1_U8 = ram_peek(0x000b);
            s_a = tmp1_U8;
  /*$273D*/ tmp2_U8 = ram_peek(0x0005);
            s_status_c = (tmp1_U8 >= tmp2_U8);
            branchTarget = true;
            if (!(tmp1_U8 != tmp2_U8))
              goto bb_24;
  /*$273F*/ CYCLES_EDGE(0x273f, 1);
            branchTarget = true;
            goto bb_1;
bb_24:
  /*$2741*/ CYCLES(0x2741, 6);
            if (ret_addr) pop16(); return;
}


void func_2800(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2800*/ CYCLES(0x2800, 10);
  /*$2802*/ ram_poke(0x0006, 0x77);
  /*$2806*/ ram_poke(0x0007, 0x9f);
bb_1:
  /*$2808*/ CYCLES(0x2808, 3);
            s_x = ram_peek(0x0006);
bb_2:
  /*$280A*/ CYCLES(0x280a, 32);
            tmp1_U8 = s_x;
  /*$280D*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$2812*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$2814*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_x = tmp1_U8;
  /*$2818*/ ram_poke(0x0002, ram_peek((0x0d00 + tmp1_U8)));
  /*$281D*/ ram_poke(0x0003, ram_peek((0x0c00 + tmp1_U8)));
  /*$281F*/ s_y = 0x13;
bb_3:
  /*$2821*/ CYCLES(0x2821, 17);
            tmp1_U8 = s_y;
            tmp2_U8 = peek((ram_peek16al(0x0000) + tmp1_U8));
  /*$2823*/ poke((ram_peek16al(0x0002) + tmp1_U8), tmp2_U8);
  /*$2825*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0x1e))
              goto bb_5;
  /*$2828*/ CYCLES_EDGE(0x2828, 1);
            branchTarget = true;
            goto bb_3;
bb_5:
  /*$282A*/ CYCLES(0x282a, 9);
  /*$282B*/ tmp1_U8 = (uint8_t)(s_x + 0x02);
            s_x = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 != ram_peek(0x0007)))
              goto bb_7;
  /*$282E*/ CYCLES_EDGE(0x282e, 1);
            branchTarget = true;
            goto bb_2;
bb_7:
  /*$2830*/ CYCLES(0x2830, 11);
            ram_poke(0x0007, (uint8_t)(ram_peek(0x0007) - 0x01));
  /*$2832*/ func_2770(0x2834);
  /*$2835*/ CYCLES(0x2835, 7);
            tmp1_U8 = ram_peek(0x0007);
            s_a = tmp1_U8;
  /*$2837*/ tmp2_U8 = tmp1_U8 != 0x8c;
            s_status_not_z = tmp2_U8;
            s_status_c = (tmp1_U8 >= 0x8c);
            s_status_n = ((uint8_t)(tmp1_U8 - 0x8c) & 0x80);
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_9;
  /*$2839*/ CYCLES_EDGE(0x2839, 1);
            branchTarget = true;
            goto bb_1;
bb_9:
  /*$283B*/ CYCLES(0x283b, 6);
            if (ret_addr) pop16(); return;
}


void func_2880(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2880*/ CYCLES(0x2880, 15);
  /*$2882*/ ram_poke(0x2899, 0x00);
  /*$2887*/ ram_poke(0x289a, 0x6e);
  /*$288A*/ s_x = ram_peek(0x0004);
bb_1:
  /*$288C*/ CYCLES(0x288c, 17);
            tmp1_U8 = s_x;
  /*$288F*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$2894*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$2896*/ s_y = ram_peek(0x0006);
bb_2:
  /*$2898*/ CYCLES(0x2898, 18);
            tmp1_U8 = peek(ram_peek16(0x2899));
            s_a = tmp1_U8;
  /*$289B*/ poke((ram_peek16al(0x0000) + s_y), tmp1_U8);
  /*$289D*/ tmp1_U8 = (uint8_t)(ram_peek(0x2899) + 0x01);
            ram_poke(0x2899, tmp1_U8);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_4;
  /*$28A0*/ CYCLES_EDGE(0x28a0, 1);
            branchTarget = true;
            goto bb_5;
bb_4:
  /*$28A2*/ CYCLES(0x28a2, 6);
            ram_poke(0x289a, (uint8_t)(ram_peek(0x289a) + 0x01));
bb_5:
  /*$28A5*/ CYCLES(0x28a5, 7);
            tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= ram_peek(0x0007)))
              goto bb_7;
  /*$28A8*/ CYCLES_EDGE(0x28a8, 1);
            branchTarget = true;
            goto bb_2;
bb_7:
  /*$28AA*/ CYCLES(0x28aa, 11);
            poke(ram_peek16(0x28ab), s_a);
  /*$28AD*/ tmp1_U8 = (uint8_t)(s_x + 0x01);
            s_x = tmp1_U8;
  /*$28AE*/ tmp2_U8 = ram_peek(0x0005);
            s_status_not_z = (tmp1_U8 != tmp2_U8);
            tmp3_U8 = tmp1_U8 >= tmp2_U8;
            s_status_c = tmp3_U8;
            s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
            branchTarget = true;
            if (tmp3_U8)
              goto bb_9;
  /*$28B0*/ CYCLES_EDGE(0x28b0, 1);
            branchTarget = true;
            goto bb_1;
bb_9:
  /*$28B2*/ CYCLES(0x28b2, 10);
            poke(ram_peek16(0x28b3), s_a);
  /*$28B5*/ if (ret_addr) pop16(); return;
}


void func_2900(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2900*/ CYCLES(0x2900, 23);
  /*$2902*/ ram_poke(0x0090, 0x99);
  /*$2906*/ ram_poke(0x0091, 0x9a);
  /*$290A*/ ram_poke(0x0092, 0x13);
  /*$290E*/ ram_poke(0x0093, 0x04);
            branchTarget = true;
bb_1:
  /*$28D0*/ CYCLES(0x28d0, 31);
  /*$28D2*/ ram_poke(0x0004, ram_peek(0x0090));
  /*$28D6*/ ram_poke(0x0005, ram_peek(0x0091));
  /*$28D8*/ tmp1_U8 = ram_peek(0x0093);
            s_a = tmp1_U8;
  /*$28DA*/ ram_poke(0x0006, tmp1_U8);
  /*$28DC*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_3;
  /*$28DD*/ tmp2_U16 = s_a;
            tmp3_U16 = (tmp2_U16 + 0x0006) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0x0006);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_4;
bb_3:
  /*$28DD*/ tmp3_U16 = adc_dec16(s_a, 0x06, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_4:
  /*$28DF*/ ram_poke(0x0007, s_a);
  /*$28E1*/ func_2880(0x28e3);
  /*$28E4*/ CYCLES(0x28e4, 13);
            ram_poke(0x0090, (uint8_t)(ram_peek(0x0090) - 0x01));
  /*$28E6*/ s_a = 0x40;
  /*$28E8*/ rom_mon_wait(0x28ea);
  /*$28EB*/ CYCLES(0x28eb, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x0092) - 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            ram_poke(0x0092, tmp1_U8);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_6;
  /*$28ED*/ CYCLES_EDGE(0x28ed, 1);
            branchTarget = true;
            goto bb_1;
bb_6:
  /*$28EF*/ CYCLES(0x28ef, 6);
            if (ret_addr) pop16(); return;
}


void func_2a00(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$2A00*/ CYCLES(0x2a00, 4);
            io_poke(0xc010, s_a);
bb_1:
  /*$2A03*/ CYCLES(0x2a03, 6);
            tmp1_U8 = io_peek(0xc000);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 & 0x80))
              goto bb_3;
  /*$2A06*/ CYCLES_EDGE(0x2a06, 1);
  /*$2A0E*/ CYCLES(0x2a0e, 4);
            tmp1_U8 = s_a;
            s_status_c = (tmp1_U8 >= 0x8d);
            branchTarget = true;
            if ((tmp1_U8 != 0x8d))
              goto bb_4;
            else
              goto bb_5;
bb_3:
  /*$2A08*/ CYCLES(0x2a08, 6);
            func_2aa0(0x2a0a);
  /*$2A0B*/ CYCLES(0x2a0b, 3);
            branchTarget = true;
            goto bb_1;
bb_4:
  /*$2A10*/ CYCLES_EDGE(0x2a10, 1);
  /*$2A15*/ CYCLES(0x2a15, 6);
            ram_poke(0x0002, s_a);
            branchTarget = true;
            goto bb_0;
bb_5:
  /*$2A12*/ CYCLES(0x2a12, 9);
            tmp1_U8 = ram_peek(0x0002);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
  /*$2A14*/ if (ret_addr) pop16(); return;
}


void func_2a20(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2A20*/ CYCLES(0x2a20, 23);
  /*$2A22*/ ram_poke(0x0000, 0x17);
  /*$2A26*/ ram_poke(0x0001, 0x0b);
  /*$2A28*/ tmp1_U8 = ram_peek(0x1078);
            s_a = tmp1_U8;
  /*$2A2B*/ ram_poke(0x0002, tmp1_U8);
  /*$2A2D*/ func_2a00(0x2a2f);
  /*$2A30*/ CYCLES(0x2a30, 27);
            ram_poke(0x1078, s_a);
  /*$2A35*/ ram_poke(0x0000, 0x44);
  /*$2A39*/ ram_poke(0x0001, 0x09);
  /*$2A3B*/ tmp1_U8 = ram_peek(0x108a);
            s_a = tmp1_U8;
  /*$2A3E*/ ram_poke(0x0002, tmp1_U8);
  /*$2A40*/ func_2a00(0x2a42);
  /*$2A43*/ CYCLES(0x2a43, 27);
            ram_poke(0x108a, s_a);
  /*$2A48*/ ram_poke(0x0000, 0x3f);
  /*$2A4C*/ ram_poke(0x0001, 0x0b);
  /*$2A4E*/ tmp1_U8 = ram_peek(0x1066);
            s_a = tmp1_U8;
  /*$2A51*/ ram_poke(0x0002, tmp1_U8);
  /*$2A53*/ func_2a00(0x2a55);
  /*$2A56*/ CYCLES(0x2a56, 27);
            ram_poke(0x1066, s_a);
  /*$2A5B*/ ram_poke(0x0000, 0x3a);
  /*$2A5F*/ ram_poke(0x0001, 0x09);
  /*$2A61*/ tmp1_U8 = ram_peek(0x109c);
            s_a = tmp1_U8;
  /*$2A64*/ ram_poke(0x0002, tmp1_U8);
  /*$2A66*/ func_2a00(0x2a68);
  /*$2A69*/ CYCLES(0x2a69, 27);
            ram_poke(0x109c, s_a);
  /*$2A6E*/ ram_poke(0x0000, 0x2c);
  /*$2A72*/ ram_poke(0x0001, 0x0b);
  /*$2A74*/ tmp1_U8 = ram_peek(0x1390);
            s_a = tmp1_U8;
  /*$2A77*/ ram_poke(0x0002, tmp1_U8);
  /*$2A79*/ func_2a00(0x2a7b);
  /*$2A7C*/ CYCLES(0x2a7c, 27);
            ram_poke(0x1390, s_a);
  /*$2A81*/ ram_poke(0x0000, 0xd4);
  /*$2A85*/ ram_poke(0x0001, 0x08);
  /*$2A87*/ tmp1_U8 = ram_peek(0x1306);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
  /*$2A8A*/ ram_poke(0x0002, tmp1_U8);
  /*$2A8C*/ func_2a00(0x2a8e);
  /*$2A8F*/ CYCLES(0x2a8f, 10);
            ram_poke(0x1306, s_a);
  /*$2A92*/ if (ret_addr) pop16(); return;
}


void func_2aa0(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2AA0*/ CYCLES(0x2aa0, 18);
            s_y = 0x00;
  /*$2AA4*/ poke(ram_peek16al(0x0000), 0xa0);
  /*$2AA6*/ s_a = 0x70;
  /*$2AA8*/ rom_mon_wait(0x2aaa);
  /*$2AAB*/ CYCLES(0x2aab, 17);
  /*$2AAD*/ poke((ram_peek16al(0x0000) + s_y), ram_peek(0x0002));
  /*$2AAF*/ s_status_not_z = 0x70;
            s_status_n = 0x00;
            s_a = 0x70;
  /*$2AB1*/ rom_mon_wait(0x2ab3);
  /*$2AB4*/ CYCLES(0x2ab4, 6);
            if (ret_addr) pop16(); return;
}


void func_2b30(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;
  uint8_t tmp4_U8;
  uint8_t tmp5_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2B30*/ CYCLES(0x2b30, 48);
            tmp1_U8 = ram_peek(0x000a);
  /*$2B35*/ ram_poke(0x2b13, ram_peek((0x2b58 + tmp1_U8)));
  /*$2B3B*/ ram_poke(0x2b14, ram_peek((0x2b5c + tmp1_U8)));
  /*$2B3E*/ tmp1_U8 = ram_peek(0x000b);
            s_a = tmp1_U8;
  /*$2B40*/ ram_poke(0x0004, tmp1_U8);
  /*$2B42*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_11;
            else
              goto bb_10;
bb_1:
  /*$2B02*/ CYCLES(0x2b02, 17);
            tmp1_U8 = s_x;
  /*$2B05*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$2B07*/ tmp1_U8 = ram_peek((0x0c00 + tmp1_U8));
            s_a = tmp1_U8;
  /*$2B0A*/ ram_poke(0x0001, tmp1_U8);
  /*$2B0C*/ s_y = ram_peek(0x0006);
bb_2:
  /*$2B0E*/ CYCLES(0x2b0e, 4);
            branchTarget = true;
            if (!(s_y >= 0x28))
              goto bb_4;
  /*$2B10*/ CYCLES_EDGE(0x2b10, 1);
            branchTarget = true;
            goto bb_5;
bb_4:
  /*$2B12*/ CYCLES(0x2b12, 15);
            tmp1_U8 = peek(ram_peek16(0x2b13));
  /*$2B15*/ tmp4_U8 = s_y;
            tmp5_U8 = peek((ram_peek16al(0x0000) + tmp4_U8));
            tmp5_U8 = tmp1_U8 ^ tmp5_U8;
            s_a = tmp5_U8;
  /*$2B17*/ poke((ram_peek16al(0x0000) + tmp4_U8), tmp5_U8);
bb_5:
  /*$2B19*/ CYCLES(0x2b19, 13);
            ram_poke(0x2b13, (uint8_t)(ram_peek(0x2b13) + 0x01));
  /*$2B1C*/ tmp4_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp4_U8;
            branchTarget = true;
            if (!(tmp4_U8 != ram_peek(0x0007)))
              goto bb_7;
  /*$2B1F*/ CYCLES_EDGE(0x2b1f, 1);
            branchTarget = true;
            goto bb_2;
bb_7:
  /*$2B21*/ CYCLES(0x2b21, 11);
            tmp4_U8 = (uint8_t)(s_x + 0x01);
            s_x = tmp4_U8;
  /*$2B22*/ poke(ram_peek16(0x2b23), s_a);
  /*$2B25*/ tmp5_U8 = ram_peek(0x0005);
            tmp1_U8 = tmp4_U8 != tmp5_U8;
            s_status_not_z = tmp1_U8;
            s_status_c = (tmp4_U8 >= tmp5_U8);
            s_status_n = ((uint8_t)(tmp4_U8 - tmp5_U8) & 0x80);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_9;
  /*$2B27*/ CYCLES_EDGE(0x2b27, 1);
            branchTarget = true;
            goto bb_1;
bb_9:
  /*$2B29*/ CYCLES(0x2b29, 6);
            if (ret_addr) pop16(); return;
bb_10:
  /*$2B43*/ s_a = (uint8_t)((s_a + 0x0018) + s_status_c);
            goto bb_12;
bb_11:
  /*$2B43*/ s_a = ((uint8_t)adc_dec16(s_a, 0x18, s_status_c));
bb_12:
  /*$2B45*/ ram_poke(0x0005, s_a);
  /*$2B47*/ tmp1_U8 = ram_peek(0x000c);
            s_a = tmp1_U8;
  /*$2B49*/ ram_poke(0x0006, tmp1_U8);
  /*$2B4B*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_14;
  /*$2B4C*/ tmp2_U16 = s_a;
            tmp3_U16 = (tmp2_U16 + 0x0005) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0x0005);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_15;
bb_14:
  /*$2B4C*/ tmp3_U16 = adc_dec16(s_a, 0x05, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_15:
  /*$2B4E*/ ram_poke(0x0007, s_a);
  /*$2B00*/ CYCLES(0x2b00, 3);
            s_x = ram_peek(0x0004);
            goto bb_1;
}


void func_2b60(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2B60*/ CYCLES(0x2b60, 6);
            func_2b30(0x2b62);
  /*$2B63*/ CYCLES(0x2b63, 6);
            func_2b70(0x2b65);
  /*$2B66*/ CYCLES(0x2b66, 3);
            func_2b30(0x0000);
            if (ret_addr) pop16(); return;
}


void func_2b70(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2B70*/ CYCLES(0x2b70, 2);
            s_x = 0x10;
bb_1:
  /*$2B72*/ CYCLES(0x2b72, 2);
            s_y = 0xc0;
bb_2:
  /*$2B74*/ CYCLES(0x2b74, 4);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_4;
  /*$2B75*/ CYCLES_EDGE(0x2b75, 1);
            branchTarget = true;
            goto bb_2;
bb_4:
  /*$2B77*/ CYCLES(0x2b77, 6);
            poke(ram_peek16al(0x2b78), s_a);
  /*$2B7A*/ s_y = 0x80;
bb_5:
  /*$2B7C*/ CYCLES(0x2b7c, 4);
            tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_7;
  /*$2B7D*/ CYCLES_EDGE(0x2b7d, 1);
            branchTarget = true;
            goto bb_5;
bb_7:
  /*$2B7F*/ CYCLES(0x2b7f, 8);
            poke(ram_peek16al(0x2b80), s_a);
  /*$2B82*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_9;
  /*$2B83*/ CYCLES_EDGE(0x2b83, 1);
            branchTarget = true;
            goto bb_1;
bb_9:
  /*$2B85*/ CYCLES(0x2b85, 6);
            if (ret_addr) pop16(); return;
}


void func_2c40(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2C40*/ CYCLES(0x2c40, 2);
            s_x = 0xa8;
bb_1:
  /*$2C42*/ CYCLES(0x2c42, 23);
            tmp1_U8 = s_x;
  /*$2C45*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$2C4A*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$2C4C*/ s_y = ram_peek(0x008a);
  /*$2C50*/ ram_poke(0x0002, ram_peek(0x008b));
bb_2:
  /*$2C52*/ CYCLES(0x2c52, 4);
            branchTarget = true;
            if (!(s_y >= 0x28))
              goto bb_4;
  /*$2C54*/ CYCLES_EDGE(0x2c54, 1);
            branchTarget = true;
            goto bb_8;
bb_4:
  /*$2C56*/ CYCLES(0x2c56, 23);
  /*$2C5A*/ tmp1_U8 = s_y;
            tmp2_U8 = peek((ram_peek16al(0x0000) + tmp1_U8));
  /*$2C5C*/ poke((ram_peek16al(0x0000) + tmp1_U8), ((ram_peek(0x0002) ^ 0xff) & tmp2_U8));
  /*$2C5E*/ tmp1_U8 = (uint8_t)(ram_peek(0x0002) << 0x01);
            ram_poke(0x0002, tmp1_U8);
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_6;
  /*$2C60*/ CYCLES_EDGE(0x2c60, 1);
            branchTarget = true;
            goto bb_4;
bb_6:
  /*$2C62*/ CYCLES(0x2c62, 9);
  /*$2C64*/ ram_poke(0x0002, 0x01);
  /*$2C66*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_8;
  /*$2C67*/ CYCLES_EDGE(0x2c67, 1);
            branchTarget = true;
            goto bb_2;
bb_8:
  /*$2C69*/ CYCLES(0x2c69, 6);
            tmp1_U8 = (uint8_t)(s_x + 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0xb0))
              goto bb_10;
  /*$2C6C*/ CYCLES_EDGE(0x2c6c, 1);
            branchTarget = true;
            goto bb_1;
bb_10:
  /*$2C6E*/ CYCLES(0x2c6e, 6);
            if (ret_addr) pop16(); return;
}


void func_2c80(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2C80*/ CYCLES(0x2c80, 73);
            tmp1_U8 = s_x;
  /*$2C83*/ tmp2_U8 = s_y;
            ram_poke((0x4350 + tmp2_U8), ram_peek((0x9f00 + tmp1_U8)));
  /*$2C89*/ ram_poke((0x4750 + tmp2_U8), ram_peek((0x9f18 + tmp1_U8)));
  /*$2C8F*/ ram_poke((0x4b50 + tmp2_U8), ram_peek((0x9f30 + tmp1_U8)));
  /*$2C95*/ ram_poke((0x4f50 + tmp2_U8), ram_peek((0x9f48 + tmp1_U8)));
  /*$2C9B*/ ram_poke((0x5350 + tmp2_U8), ram_peek((0x9f60 + tmp1_U8)));
  /*$2CA1*/ ram_poke((0x5750 + tmp2_U8), ram_peek((0x9f78 + tmp1_U8)));
  /*$2CA4*/ tmp3_U8 = ram_peek((0x9f90 + tmp1_U8));
            s_a = tmp3_U8;
  /*$2CA7*/ ram_poke((0x5b50 + tmp2_U8), tmp3_U8);
  /*$2CAA*/ s_x = (uint8_t)(tmp1_U8 - 0x01);
  /*$2CAB*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
            s_status_not_z = tmp2_U8;
            s_status_n = (tmp2_U8 & 0x80);
            s_y = tmp2_U8;
  /*$2CAC*/ if (ret_addr) pop16(); return;
}


void func_2cb0(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2CB0*/ CYCLES(0x2cb0, 73);
            tmp1_U8 = s_x;
  /*$2CB3*/ tmp2_U8 = s_y;
            ram_poke((0x43d0 + tmp2_U8), ram_peek((0x9f00 + tmp1_U8)));
  /*$2CB9*/ ram_poke((0x47d0 + tmp2_U8), ram_peek((0x9f18 + tmp1_U8)));
  /*$2CBF*/ ram_poke((0x4bd0 + tmp2_U8), ram_peek((0x9f30 + tmp1_U8)));
  /*$2CC5*/ ram_poke((0x4fd0 + tmp2_U8), ram_peek((0x9f48 + tmp1_U8)));
  /*$2CCB*/ ram_poke((0x53d0 + tmp2_U8), ram_peek((0x9f60 + tmp1_U8)));
  /*$2CD1*/ ram_poke((0x57d0 + tmp2_U8), ram_peek((0x9f78 + tmp1_U8)));
  /*$2CD4*/ tmp3_U8 = ram_peek((0x9f90 + tmp1_U8));
            s_a = tmp3_U8;
  /*$2CD7*/ ram_poke((0x5bd0 + tmp2_U8), tmp3_U8);
  /*$2CDA*/ s_y = (uint8_t)(tmp2_U8 - 0x01);
  /*$2CDB*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_x = tmp1_U8;
  /*$2CDC*/ if (ret_addr) pop16(); return;
}


void func_2ce0(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2CE0*/ CYCLES(0x2ce0, 6);
            func_2c80(0x2ce2);
  /*$2CE3*/ CYCLES(0x2ce3, 3);
            func_2c80(0x0000);
            if (ret_addr) pop16(); return;
}


void func_2ce8(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2CE8*/ CYCLES(0x2ce8, 6);
            func_2cb0(0x2cea);
  /*$2CEB*/ CYCLES(0x2ceb, 3);
            func_2cb0(0x0000);
            if (ret_addr) pop16(); return;
}


void func_2d00(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2D00*/ CYCLES(0x2d00, 19);
  /*$2D06*/ s_x = (uint8_t)((uint8_t)((ram_peek(0x0080) & 0x0f) << 0x01) + 0x01);
  /*$2D07*/ s_y = 0x0f;
  /*$2D09*/ func_2ce0(0x2d0b);
  /*$2D0C*/ CYCLES(0x2d0c, 9);
            s_a = ram_peek(0x0080);
  /*$2D0E*/ func_2d38(0x2d10);
  /*$2D11*/ CYCLES(0x2d11, 10);
  /*$2D12*/ s_x = (uint8_t)(s_a + 0x01);
  /*$2D13*/ func_2ce0(0x2d15);
  /*$2D16*/ CYCLES(0x2d16, 17);
  /*$2D1C*/ s_x = (uint8_t)((uint8_t)((ram_peek(0x0081) & 0x0f) << 0x01) + 0x01);
  /*$2D1D*/ func_2ce0(0x2d1f);
  /*$2D20*/ CYCLES(0x2d20, 9);
            s_a = ram_peek(0x0081);
  /*$2D22*/ func_2d38(0x2d24);
  /*$2D25*/ CYCLES(0x2d25, 10);
  /*$2D26*/ s_x = (uint8_t)(s_a + 0x01);
  /*$2D27*/ func_2ce0(0x2d29);
  /*$2D2A*/ CYCLES(0x2d2a, 14);
  /*$2D2E*/ tmp1_U16 = (ram_peek(0x0082) & 0x0f) << 0x01;
            s_status_c = (uint8_t)(tmp1_U16 >> 8);
  /*$2D30*/ s_x = (uint8_t)(((uint8_t)tmp1_U16) + 0x01);
  /*$2D31*/ func_2ce0(0x0000);
            if (ret_addr) pop16(); return;
}


void func_2d38(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2D38*/ CYCLES(0x2d38, 16);
  /*$2D3C*/ s_a = (uint8_t)((s_a >> 0x04) << 0x01);
  /*$2D3D*/ if (ret_addr) pop16(); return;
}


void func_2d40(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2D40*/ CYCLES(0x2d40, 19);
  /*$2D46*/ s_x = (uint8_t)((uint8_t)((ram_peek(0x0083) & 0x0f) << 0x01) + 0x01);
  /*$2D47*/ s_y = 0x0f;
  /*$2D49*/ func_2ce8(0x2d4b);
  /*$2D4C*/ CYCLES(0x2d4c, 9);
            s_a = ram_peek(0x0083);
  /*$2D4E*/ func_2d38(0x2d50);
  /*$2D51*/ CYCLES(0x2d51, 10);
  /*$2D52*/ s_x = (uint8_t)(s_a + 0x01);
  /*$2D53*/ func_2ce8(0x2d55);
  /*$2D56*/ CYCLES(0x2d56, 17);
  /*$2D5C*/ s_x = (uint8_t)((uint8_t)((ram_peek(0x0084) & 0x0f) << 0x01) + 0x01);
  /*$2D5D*/ func_2ce8(0x2d5f);
  /*$2D60*/ CYCLES(0x2d60, 9);
            s_a = ram_peek(0x0084);
  /*$2D62*/ func_2d38(0x2d64);
  /*$2D65*/ CYCLES(0x2d65, 10);
  /*$2D66*/ s_x = (uint8_t)(s_a + 0x01);
  /*$2D67*/ func_2ce8(0x2d69);
  /*$2D6A*/ CYCLES(0x2d6a, 14);
  /*$2D70*/ s_x = (uint8_t)((uint8_t)((ram_peek(0x0085) & 0x0f) << 0x01) + 0x01);
  /*$2D71*/ func_2ce8(0x0000);
            if (ret_addr) pop16(); return;
}


void func_2d80(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint16_t tmp2_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2D80*/ CYCLES(0x2d80, 18);
            s_a = ram_peek(0x0086);
  /*$2D82*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_2;
  /*$2D83*/ tmp1_U16 = s_a;
            tmp2_U16 = (tmp1_U16 + 0x0001) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp2_U16, (uint8_t)tmp1_U16, (uint8_t)0x0001);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_3;
bb_2:
  /*$2D83*/ tmp2_U16 = adc_dec16(s_a, 0x01, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            s_status_v = (((uint8_t)(tmp2_U16 >> 8) & 0x40) != 0);
bb_3:
  /*$2D87*/ s_x = (uint8_t)((uint8_t)(s_a << 0x01) + 0x01);
  /*$2D88*/ s_y = 0x1d;
  /*$2D8A*/ func_2ce0(0x0000);
            if (ret_addr) pop16(); return;
}


void func_2d90(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2D90*/ CYCLES(0x2d90, 14);
  /*$2D94*/ s_x = (uint8_t)((uint8_t)(ram_peek(0x0087) << 0x01) + 0x01);
  /*$2D95*/ s_y = 0x27;
  /*$2D97*/ func_2ce0(0x0000);
            if (ret_addr) pop16(); return;
}


void func_2e00(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2E00*/ CYCLES(0x2e00, 38);
            s_status_d = 0x01;
  /*$2E03*/ tmp1_U16 = adc_dec16(s_x, ram_peek(0x0080), 0x00);
            s_status_c = ((uint8_t)(tmp1_U16 >> 8) & 0x01);
  /*$2E05*/ ram_poke(0x0080, ((uint8_t)tmp1_U16));
  /*$2E07*/ s_a = s_y;
            if (0x01)
              goto bb_2;
  /*$2E08*/ tmp1_U16 = (s_a + ram_peek(0x0081)) + s_status_c;
            s_status_c = (uint8_t)(tmp1_U16 >> 8);
            s_a = ((uint8_t)tmp1_U16);
            goto bb_3;
bb_2:
  /*$2E08*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x0081), s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
bb_3:
  /*$2E0A*/ ram_poke(0x0081, s_a);
  /*$2E0C*/ s_a = 0x00;
            if (s_status_d)
              goto bb_5;
  /*$2E0E*/ tmp1_U16 = s_a;
            tmp2_U16 = ram_peek(0x0082);
            tmp3_U16 = (tmp1_U16 + tmp2_U16) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp1_U16, (uint8_t)tmp2_U16);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_6;
bb_5:
  /*$2E0E*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x0082), s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_6:
  /*$2E10*/ ram_poke(0x0082, s_a);
  /*$2E12*/ s_status_d = 0x00;
  /*$2E15*/ ram_poke(0x007d, 0x00);
  /*$2E17*/ func_2d00(0x0000);
            if (ret_addr) pop16(); return;
}


void func_2f00(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2F00*/ CYCLES(0x2f00, 4);
            s_x = 0x00;
  /*$2F02*/ s_y = 0xa7;
bb_1:
  /*$2F04*/ CYCLES(0x2f04, 58);
            tmp1_U8 = s_y;
  /*$2F07*/ push8(ram_peek((0x0c00 + tmp1_U8)));
  /*$2F08*/ tmp2_U8 = s_x;
  /*$2F0B*/ ram_poke((0x0c00 + tmp1_U8), ram_peek((0x0c00 + tmp2_U8)));
  /*$2F0E*/ tmp3_U8 = pop8();
  /*$2F0F*/ ram_poke((0x0c00 + tmp2_U8), tmp3_U8);
  /*$2F15*/ push8(ram_peek((0x0d00 + tmp1_U8)));
  /*$2F19*/ ram_poke((0x0d00 + tmp1_U8), ram_peek((0x0d00 + tmp2_U8)));
  /*$2F1C*/ tmp3_U8 = pop8();
            s_a = tmp3_U8;
  /*$2F1D*/ ram_poke((0x0d00 + tmp2_U8), tmp3_U8);
  /*$2F20*/ tmp3_U8 = (uint8_t)(tmp2_U8 + 0x01);
            s_x = tmp3_U8;
  /*$2F21*/ s_y = (uint8_t)(tmp1_U8 - 0x01);
  /*$2F22*/ s_status_not_z = (tmp3_U8 != 0x54);
            tmp3_U8 = tmp3_U8 >= 0x54;
            s_status_c = tmp3_U8;
            s_status_n = ((uint8_t)(tmp2_U8 - 0x53) & 0x80);
            branchTarget = true;
            if (tmp3_U8)
              goto bb_3;
  /*$2F24*/ CYCLES_EDGE(0x2f24, 1);
            branchTarget = true;
            goto bb_1;
bb_3:
  /*$2F26*/ CYCLES(0x2f26, 6);
            if (ret_addr) pop16(); return;
}


void func_2f80(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint16_t tmp2_U16;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2F80*/ CYCLES(0x2f80, 6);
            func_3000(0x2f82);
  /*$2F83*/ CYCLES(0x2f83, 41);
            if (s_status_d)
              goto bb_2;
  /*$2F83*/ s_a = (uint8_t)((s_a + 0x0006) + s_status_c);
            goto bb_3;
bb_2:
  /*$2F83*/ s_a = ((uint8_t)adc_dec16(s_a, 0x06, s_status_c));
bb_3:
  /*$2F85*/ ram_poke(0x0000, s_a);
  /*$2F87*/ s_a = 0xa8;
  /*$2F89*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_5;
  /*$2F8A*/ s_a = (uint8_t)((s_a - ram_peek(0x0000)) - (uint8_t)(0x01 - s_status_c));
            goto bb_6;
bb_5:
  /*$2F8A*/ s_a = ((uint8_t)sbc_dec16(s_a, ram_peek(0x0000), s_status_c));
bb_6:
  /*$2F8C*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_8;
  /*$2F8D*/ tmp1_U16 = s_a;
            tmp2_U16 = (tmp1_U16 - 0x0005) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp2_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp2_U16, (uint8_t)tmp1_U16, (uint8_t)0xfffa);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_9;
bb_8:
  /*$2F8D*/ tmp2_U16 = sbc_dec16(s_a, 0x05, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            tmp3_U8 = (uint8_t)(tmp2_U16 >> 8);
            s_status_c = (tmp3_U8 & 0x01);
            s_status_v = ((tmp3_U8 & 0x40) != 0);
bb_9:
  /*$2F8F*/ ram_poke(0x00e0, s_a);
  /*$2F94*/ push8(ram_peek(0x1066));
  /*$2F98*/ ram_poke(0x1066, ram_peek(0x1078));
  /*$2F9B*/ tmp3_U8 = pop8();
            s_a = tmp3_U8;
  /*$2FE0*/ CYCLES(0x2fe0, 6);
            func_3040(0x2fe2);
  /*$2FE3*/ CYCLES(0x2fe3, 5);
            tmp3_U8 = ram_peek(0x00e3);
            s_status_not_z = tmp3_U8;
            s_status_n = (tmp3_U8 & 0x80);
            branchTarget = true;
            if (!tmp3_U8)
              goto bb_11;
  /*$2FE5*/ CYCLES_EDGE(0x2fe5, 1);
  /*$2FE8*/ CYCLES(0x2fe8, 2);
            branchTarget = true;
            if (!s_status_n)
              goto bb_12;
            else
              goto bb_13;
bb_11:
  /*$2FE7*/ CYCLES(0x2fe7, 6);
            if (ret_addr) pop16(); return;
bb_12:
  /*$2FE8*/ CYCLES_EDGE(0x2fe8, 1);
  /*$2FEE*/ CYCLES(0x2fee, 2);
            s_status_not_z = 0xff;
            s_status_n = 0x80;
            s_a = 0xff;
            goto bb_14;
bb_13:
  /*$2FEA*/ CYCLES(0x2fea, 4);
            s_status_not_z = 0x01;
            s_status_n = 0x00;
            s_a = 0x01;
  /*$2FEC*/ CYCLES_EDGE(0x2fec, 1);
            branchTarget = true;
bb_14:
  /*$2FF0*/ CYCLES(0x2ff0, 9);
            ram_poke(0x00e3, s_a);
  /*$2FF2*/ if (ret_addr) pop16(); return;
}


void func_2fa0(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;
  uint8_t tmp4_U8;
  uint8_t tmp5_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2FA0*/ CYCLES(0x2fa0, 9);
            s_x = 0x00;
            branchTarget = true;
            if ((uint8_t)((ram_peek(0x00ff) << 0x01) >> 8))
              goto bb_2;
  /*$2FA5*/ CYCLES_EDGE(0x2fa5, 1);
            branchTarget = true;
            goto bb_3;
bb_2:
  /*$2FA7*/ CYCLES(0x2fa7, 2);
            s_x = 0xc0;
bb_3:
  /*$2FA9*/ CYCLES(0x2fa9, 15);
            ram_poke(0x00af, s_x);
  /*$2FAB*/ s_a = ram_peek(0x007e);
  /*$2FAD*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_5;
  /*$2FAE*/ tmp1_U16 = s_a;
            tmp2_U16 = ram_peek(0x0086);
            tmp3_U16 = (tmp1_U16 + tmp2_U16) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp1_U16, (uint8_t)tmp2_U16);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_6;
bb_5:
  /*$2FAE*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x0086), s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_6:
  /*$2FB0*/ tmp4_U8 = s_a >= 0x0a;
            s_status_c = tmp4_U8;
            branchTarget = true;
            if (tmp4_U8)
              goto bb_8;
  /*$2FB2*/ CYCLES_EDGE(0x2fb2, 1);
  /*$2FB5*/ CYCLES(0x2fb5, 4);
            branchTarget = true;
            if ((s_x != 0x00))
              goto bb_9;
            else
              goto bb_10;
bb_8:
  /*$2FB4*/ CYCLES(0x2fb4, 6);
            if (ret_addr) pop16(); return;
bb_9:
  /*$2FB7*/ CYCLES_EDGE(0x2fb7, 1);
  /*$2FC6*/ CYCLES(0x2fc6, 8);
            branchTarget = true;
            if (!(ram_peek(0x0c00) != 0x40))
              goto bb_13;
            else
              goto bb_14;
bb_10:
  /*$2FB9*/ CYCLES(0x2fb9, 8);
            branchTarget = true;
            if (!(ram_peek(0x0c00) != 0x40))
              goto bb_12;
  /*$2FBE*/ CYCLES_EDGE(0x2fbe, 1);
  /*$2FC3*/ CYCLES(0x2fc3, 3);
            branchTarget = true;
            goto bb_22;
bb_12:
  /*$2FC0*/ CYCLES(0x2fc0, 3);
            branchTarget = true;
            goto bb_23;
bb_13:
  /*$2FCB*/ CYCLES_EDGE(0x2fcb, 1);
  /*$2FD0*/ CYCLES(0x2fd0, 3);
            branchTarget = true;
            goto bb_22;
bb_14:
  /*$2FCD*/ CYCLES(0x2fcd, 3);
            branchTarget = true;
            goto bb_23;
bb_15:
  /*$AD00*/ CYCLES(0xad00, 2);
            s_x = 0xb8;
bb_16:
  /*$AD02*/ CYCLES(0xad02, 16);
            tmp4_U8 = s_x;
  /*$AD05*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp4_U8)));
  /*$AD0A*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp4_U8)));
  /*$AD0C*/ s_y = 0x20;
bb_17:
  /*$AD0E*/ CYCLES(0xad0e, 22);
            tmp4_U8 = peek(ram_peek16(0xad0f));
  /*$AD11*/ tmp5_U8 = s_y;
            poke((ram_peek16al(0x0000) + tmp5_U8), tmp4_U8);
  /*$AD13*/ ram_poke(0xad0f, (uint8_t)(ram_peek(0xad0f) + 0x01));
  /*$AD16*/ tmp5_U8 = (uint8_t)(tmp5_U8 + 0x01);
            s_y = tmp5_U8;
            branchTarget = true;
            if ((tmp5_U8 >= 0x28))
              goto bb_19;
  /*$AD19*/ CYCLES_EDGE(0xad19, 1);
            branchTarget = true;
            goto bb_17;
bb_19:
  /*$AD1B*/ CYCLES(0xad1b, 6);
            tmp5_U8 = (uint8_t)(s_x + 0x01);
            s_x = tmp5_U8;
  /*$AD1C*/ tmp5_U8 = tmp5_U8 >= 0xc0;
            s_status_c = tmp5_U8;
            branchTarget = true;
            if (tmp5_U8)
              goto bb_21;
  /*$AD1E*/ CYCLES_EDGE(0xad1e, 1);
            branchTarget = true;
            goto bb_16;
bb_21:
  /*$AD20*/ CYCLES(0xad20, 6);
            if (ret_addr) pop16(); return;
bb_22:
  /*$AD40*/ CYCLES(0xad40, 15);
  /*$AD42*/ ram_poke(0xad0f, 0x80);
  /*$AD47*/ ram_poke(0xad10, 0x6e);
            branchTarget = true;
            goto bb_15;
bb_23:
  /*$AD50*/ CYCLES(0xad50, 15);
  /*$AD52*/ ram_poke(0xad0f, 0xc0);
  /*$AD57*/ ram_poke(0xad10, 0x6e);
            branchTarget = true;
            goto bb_15;
}


void func_2fd3(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$2FD3*/ CYCLES(0x2fd3, 4);
            tmp1_U8 = s_a;
            s_status_c = (tmp1_U8 >= 0x40);
            branchTarget = true;
            if ((tmp1_U8 != 0x40)) {
    /*$2FD5*/ CYCLES_EDGE(0x2fd5, 1);
    /*$2FD8*/ CYCLES(0x2fd8, 6);
              func_2f80(0x2fda);
    /*$2FDB*/ CYCLES(0x2fdb, 6);
              func_2f00(0x2fdd);
    /*$2FDE*/ CYCLES(0x2fde, 6);
              if (ret_addr) pop16(); return;
            }
  /*$2FD7*/ CYCLES(0x2fd7, 6);
            if (ret_addr) pop16(); return;
}


void func_3000(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$3000*/ CYCLES(0x3000, 18);
            s_a = ram_peek(0x00e2);
  /*$3002*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_2;
  /*$3003*/ s_a = (uint8_t)((s_a + 0x0060) + s_status_c);
            goto bb_3;
bb_2:
  /*$3003*/ s_a = ((uint8_t)adc_dec16(s_a, 0x60, s_status_c));
bb_3:
  /*$3005*/ ram_poke(0x1040, s_a);
  /*$3008*/ s_a = 0x00;
  /*$300A*/ tmp1_U8 = ram_peek(0x00e5);
            s_x = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_5;
  /*$300C*/ CYCLES_EDGE(0x300c, 1);
            branchTarget = true;
            goto bb_14;
bb_5:
  /*$300E*/ CYCLES(0x300e, 8);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_7;
  /*$300F*/ s_a = (uint8_t)((s_a + 0x0050) + s_status_c);
            goto bb_8;
bb_7:
  /*$300F*/ s_a = ((uint8_t)adc_dec16(s_a, 0x50, s_status_c));
bb_8:
  /*$3011*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_10;
  /*$3012*/ CYCLES_EDGE(0x3012, 1);
            branchTarget = true;
            goto bb_14;
bb_10:
  /*$3014*/ CYCLES(0x3014, 4);
            s_status_c = 0x00;
            if (s_status_d)
              goto bb_12;
  /*$3015*/ s_a = (uint8_t)((s_a + 0x0050) + s_status_c);
            goto bb_13;
bb_12:
  /*$3015*/ s_a = ((uint8_t)adc_dec16(s_a, 0x50, s_status_c));
bb_13:
bb_14:
  /*$3017*/ CYCLES(0x3017, 36);
            ram_poke(0x103f, s_a);
  /*$301A*/ tmp1_U8 = ram_peek(0x00e0);
            s_a = tmp1_U8;
  /*$301C*/ ram_poke(0x0004, tmp1_U8);
  /*$301E*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_16;
  /*$301F*/ s_a = (uint8_t)((s_a + 0x000b) + s_status_c);
            goto bb_17;
bb_16:
  /*$301F*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0b, s_status_c));
bb_17:
  /*$3021*/ ram_poke(0x0005, s_a);
  /*$3023*/ tmp1_U8 = ram_peek(0x00e1);
            s_a = tmp1_U8;
  /*$3025*/ ram_poke(0x0006, tmp1_U8);
  /*$3027*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_19;
  /*$3028*/ s_a = (uint8_t)((s_a + 0x0007) + s_status_c);
            goto bb_20;
bb_19:
  /*$3028*/ s_a = ((uint8_t)adc_dec16(s_a, 0x07, s_status_c));
bb_20:
  /*$302A*/ ram_poke(0x0007, s_a);
  /*$302C*/ func_1030(0x302e);
  /*$302F*/ CYCLES(0x302f, 11);
            s_a = ram_peek(0x00e0);
  /*$3031*/ s_status_c = 0x00;
  /*$3032*/ if (ret_addr) pop16(); return;
}


void func_3040(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$3040*/ CYCLES(0x3040, 33);
            ram_poke(0x1078, s_a);
  /*$3046*/ push8(ram_peek(0x36cd));
  /*$304A*/ ram_poke(0x36cd, ram_peek(0x36db));
  /*$304D*/ tmp1_U8 = pop8();
  /*$304E*/ ram_poke(0x36db, tmp1_U8);
  /*$3051*/ if (ret_addr) pop16(); return;
}


void func_3070(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;
  uint16_t tmp4_U16;
  uint16_t tmp5_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$3070*/ CYCLES(0x3070, 25);
  /*$3072*/ ram_poke(0x000a, 0x00);
  /*$3076*/ ram_poke(0x000b, 0x86);
  /*$307A*/ ram_poke(0x000c, 0x5e);
  /*$307E*/ ram_poke(0x0005, 0x02);
  /*$3082*/ ram_poke(0x0004, 0x00);
bb_1:
  /*$3084*/ CYCLES(0x3084, 3);
            s_x = ram_peek(0x000c);
bb_2:
  /*$3086*/ CYCLES(0x3086, 37);
            tmp1_U8 = s_x;
  /*$3089*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$308E*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$3090*/ tmp2_U8 = ram_peek(0x0004);
            s_y = tmp2_U8;
  /*$3094*/ tmp3_U8 = peek((ram_peek16al(0x0000) + tmp2_U8));
  /*$3096*/ poke((ram_peek16al(0x0000) + tmp2_U8), (ram_peek(0x0005) | tmp3_U8));
  /*$3098*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0xa7))
              goto bb_4;
  /*$309B*/ CYCLES_EDGE(0x309b, 1);
            branchTarget = true;
            goto bb_2;
bb_4:
  /*$309D*/ CYCLES(0x309d, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x0005) << 0x01);
            ram_poke(0x0005, tmp1_U8);
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_6;
  /*$309F*/ CYCLES_EDGE(0x309f, 1);
            branchTarget = true;
            goto bb_9;
bb_6:
  /*$30A1*/ CYCLES(0x30a1, 17);
  /*$30A3*/ ram_poke(0x0005, 0x01);
  /*$30A5*/ ram_poke(0x0004, (uint8_t)(ram_peek(0x0004) + 0x01));
            branchTarget = true;
            if ((ram_peek(0x0004) >= 0x28))
              goto bb_8;
  /*$30AB*/ CYCLES_EDGE(0x30ab, 1);
            branchTarget = true;
            goto bb_9;
bb_8:
  /*$30AD*/ CYCLES(0x30ad, 6);
            if (ret_addr) pop16(); return;
bb_9:
  /*$30AE*/ CYCLES(0x30ae, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x0005) << 0x01);
            ram_poke(0x0005, tmp1_U8);
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_11;
  /*$30B0*/ CYCLES_EDGE(0x30b0, 1);
            branchTarget = true;
            goto bb_12;
bb_11:
  /*$30B2*/ CYCLES(0x30b2, 10);
  /*$30B4*/ ram_poke(0x0005, 0x01);
  /*$30B6*/ ram_poke(0x0004, (uint8_t)(ram_peek(0x0004) + 0x01));
bb_12:
  /*$30B8*/ CYCLES(0x30b8, 11);
  /*$30BA*/ tmp1_U8 = peek(ram_peek16al(0x000a));
            s_a = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 != 0x01))
              goto bb_14;
  /*$30BE*/ CYCLES_EDGE(0x30be, 1);
  /*$30C5*/ CYCLES(0x30c5, 4);
            branchTarget = true;
            if ((s_a != 0x03))
              goto bb_15;
            else
              goto bb_16;
bb_14:
  /*$30C0*/ CYCLES(0x30c0, 8);
            ram_poke(0x000c, (uint8_t)(ram_peek(0x000c) - 0x01));
            branchTarget = true;
            goto bb_17;
bb_15:
  /*$30C7*/ CYCLES_EDGE(0x30c7, 1);
            branchTarget = true;
            goto bb_17;
bb_16:
  /*$30C9*/ CYCLES(0x30c9, 5);
            ram_poke(0x000c, (uint8_t)(ram_peek(0x000c) + 0x01));
bb_17:
  /*$30CB*/ CYCLES(0x30cb, 21);
            s_a = ram_peek(0x000a);
  /*$30CD*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_19;
  /*$30CE*/ tmp4_U16 = (s_a + 0x0001) + s_status_c;
            s_status_c = (uint8_t)(tmp4_U16 >> 8);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_20;
bb_19:
  /*$30CE*/ tmp5_U16 = adc_dec16(s_a, 0x01, s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            s_status_c = ((uint8_t)(tmp5_U16 >> 8) & 0x01);
bb_20:
  /*$30D0*/ ram_poke(0x000a, s_a);
  /*$30D2*/ s_a = ram_peek(0x000b);
            if (s_status_d)
              goto bb_22;
  /*$30D4*/ tmp4_U16 = s_a;
            tmp5_U16 = tmp4_U16 + s_status_c;
            s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x0000);
            s_a = ((uint8_t)tmp5_U16);
            goto bb_23;
bb_22:
  /*$30D4*/ tmp5_U16 = adc_dec16(s_a, 0x00, s_status_c);
            s_a = ((uint8_t)tmp5_U16);
            s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
bb_23:
  /*$30D6*/ ram_poke(0x000b, s_a);
            branchTarget = true;
            goto bb_1;
}


void func_3440(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$3440*/ CYCLES(0x3440, 3);
            s_x = ram_peek(0x0004);
bb_1:
  /*$3442*/ CYCLES(0x3442, 17);
            tmp1_U8 = s_x;
  /*$3445*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$344A*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$344C*/ s_y = ram_peek(0x0006);
bb_2:
  /*$344E*/ CYCLES(0x344e, 4);
            branchTarget = true;
            if (!(s_y >= 0x28))
              goto bb_4;
  /*$3450*/ CYCLES_EDGE(0x3450, 1);
            branchTarget = true;
            goto bb_5;
bb_4:
  /*$3452*/ CYCLES(0x3452, 17);
            tmp1_U8 = peek(ram_peek16(0x3453));
  /*$3457*/ tmp2_U8 = s_y;
            tmp3_U8 = peek((ram_peek16al(0x0000) + tmp2_U8));
  /*$3459*/ poke((ram_peek16al(0x0000) + tmp2_U8), ((tmp1_U8 ^ 0xff) & tmp3_U8));
bb_5:
  /*$345B*/ CYCLES(0x345b, 13);
            ram_poke(0x3453, (uint8_t)(ram_peek(0x3453) + 0x01));
  /*$345E*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp2_U8;
            branchTarget = true;
            if (!(tmp2_U8 != ram_peek(0x0007)))
              goto bb_7;
  /*$3461*/ CYCLES_EDGE(0x3461, 1);
            branchTarget = true;
            goto bb_2;
bb_7:
  /*$3463*/ CYCLES(0x3463, 9);
            s_x = (uint8_t)(s_x + 0x01);
  /*$3464*/ tmp2_U8 = (uint8_t)(ram_peek(0x0008) - 0x01);
            ram_poke(0x0008, tmp2_U8);
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_9;
  /*$3466*/ CYCLES_EDGE(0x3466, 1);
            branchTarget = true;
            goto bb_13;
bb_9:
  /*$3468*/ CYCLES(0x3468, 23);
  /*$346A*/ ram_poke(0x0008, ram_peek(0x0009));
  /*$346C*/ ram_poke(0x0005, (uint8_t)(ram_peek(0x0005) - 0x01));
  /*$346E*/ s_a = ram_peek(0x3453);
  /*$3471*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_11;
  /*$3472*/ s_a = (uint8_t)((s_a + 0x0009) + s_status_c);
            goto bb_12;
bb_11:
  /*$3472*/ s_a = ((uint8_t)adc_dec16(s_a, 0x09, s_status_c));
bb_12:
  /*$3474*/ ram_poke(0x3453, s_a);
bb_13:
  /*$3477*/ CYCLES(0x3477, 5);
            branchTarget = true;
            if ((s_x >= ram_peek(0x0005)))
              goto bb_15;
  /*$3479*/ CYCLES_EDGE(0x3479, 1);
            branchTarget = true;
            goto bb_1;
bb_15:
  /*$347B*/ CYCLES(0x347b, 6);
            if (ret_addr) pop16(); return;
}


void func_3480(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;
  uint16_t tmp4_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$3480*/ CYCLES(0x3480, 58);
            tmp1_U8 = ram_peek(0x0071);
            s_a = tmp1_U8;
  /*$3482*/ ram_poke(0x0004, tmp1_U8);
  /*$3484*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_17;
            else
              goto bb_16;
bb_1:
  /*$3402*/ CYCLES(0x3402, 17);
            tmp1_U8 = s_x;
  /*$3405*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$340A*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$340C*/ s_y = ram_peek(0x0006);
bb_2:
  /*$340E*/ CYCLES(0x340e, 4);
            branchTarget = true;
            if (!(s_y >= 0x28))
              goto bb_4;
  /*$3410*/ CYCLES_EDGE(0x3410, 1);
            branchTarget = true;
            goto bb_5;
bb_4:
  /*$3412*/ CYCLES(0x3412, 17);
            tmp1_U8 = peek(ram_peek16(0x3413));
  /*$3415*/ tmp2_U8 = s_y;
            tmp3_U8 = peek((ram_peek16al(0x0000) + tmp2_U8));
  /*$3419*/ poke((ram_peek16al(0x0000) + tmp2_U8), ((tmp1_U8 | tmp3_U8) & 0x7f));
bb_5:
  /*$341B*/ CYCLES(0x341b, 13);
            ram_poke(0x3413, (uint8_t)(ram_peek(0x3413) + 0x01));
  /*$341E*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
            s_y = tmp2_U8;
            branchTarget = true;
            if (!(tmp2_U8 != ram_peek(0x0007)))
              goto bb_7;
  /*$3421*/ CYCLES_EDGE(0x3421, 1);
            branchTarget = true;
            goto bb_2;
bb_7:
  /*$3423*/ CYCLES(0x3423, 9);
            s_x = (uint8_t)(s_x + 0x01);
  /*$3424*/ tmp2_U8 = (uint8_t)(ram_peek(0x0008) - 0x01);
            ram_poke(0x0008, tmp2_U8);
            branchTarget = true;
            if (!tmp2_U8)
              goto bb_9;
  /*$3426*/ CYCLES_EDGE(0x3426, 1);
            branchTarget = true;
            goto bb_13;
bb_9:
  /*$3428*/ CYCLES(0x3428, 23);
  /*$342A*/ ram_poke(0x0008, ram_peek(0x0009));
  /*$342C*/ ram_poke(0x0005, (uint8_t)(ram_peek(0x0005) - 0x01));
  /*$342E*/ s_a = ram_peek(0x3413);
  /*$3431*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_11;
  /*$3432*/ s_a = (uint8_t)((s_a + 0x0009) + s_status_c);
            goto bb_12;
bb_11:
  /*$3432*/ s_a = ((uint8_t)adc_dec16(s_a, 0x09, s_status_c));
bb_12:
  /*$3434*/ ram_poke(0x3413, s_a);
bb_13:
  /*$3437*/ CYCLES(0x3437, 5);
            branchTarget = true;
            if ((s_x >= ram_peek(0x0005)))
              goto bb_15;
  /*$3439*/ CYCLES_EDGE(0x3439, 1);
            branchTarget = true;
            goto bb_1;
bb_15:
  /*$343B*/ CYCLES(0x343b, 6);
            if (ret_addr) pop16(); return;
bb_16:
  /*$3485*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
            goto bb_18;
bb_17:
  /*$3485*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
bb_18:
  /*$3487*/ ram_poke(0x0005, s_a);
  /*$3489*/ tmp1_U8 = ram_peek(0x0072);
            s_a = tmp1_U8;
  /*$348B*/ ram_poke(0x0006, tmp1_U8);
  /*$348D*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_20;
  /*$348E*/ s_a = (uint8_t)((s_a + 0x0009) + s_status_c);
            goto bb_21;
bb_20:
  /*$348E*/ s_a = ((uint8_t)adc_dec16(s_a, 0x09, s_status_c));
bb_21:
  /*$3490*/ ram_poke(0x0007, s_a);
  /*$3494*/ ram_poke(0x3453, 0x00);
  /*$3497*/ s_a = ram_peek(0x0073);
  /*$3499*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_23;
  /*$349A*/ s_a = (uint8_t)((s_a + 0x0039) + s_status_c);
            goto bb_24;
bb_23:
  /*$349A*/ s_a = ((uint8_t)adc_dec16(s_a, 0x39, s_status_c));
bb_24:
  /*$349C*/ ram_poke(0x3454, s_a);
  /*$349F*/ tmp1_U8 = ram_peek(0x0074);
  /*$34A1*/ ram_poke(0x0008, tmp1_U8);
  /*$34A3*/ ram_poke(0x0009, tmp1_U8);
  /*$34A5*/ func_3440(0x34a7);
  /*$34A8*/ CYCLES(0x34a8, 9);
            s_a = ram_peek(0x0073);
  /*$34AA*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_26;
  /*$34AB*/ tmp1_U8 = (uint8_t)((s_a - 0x0002) - (uint8_t)(0x01 - s_status_c));
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            goto bb_27;
bb_26:
  /*$34AB*/ tmp4_U16 = sbc_dec16(s_a, 0x02, s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_n = ((uint8_t)(tmp4_U16 >> 8) & 0x80);
bb_27:
            branchTarget = true;
            if (s_status_n)
              goto bb_29;
  /*$34AD*/ CYCLES_EDGE(0x34ad, 1);
            branchTarget = true;
            goto bb_33;
bb_29:
  /*$34AF*/ CYCLES(0x34af, 9);
            ram_poke(0x0072, (uint8_t)(ram_peek(0x0072) - 0x01));
  /*$34B1*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_31;
  /*$34B2*/ s_a = (uint8_t)((s_a + 0x0007) + s_status_c);
            goto bb_32;
bb_31:
  /*$34B2*/ s_a = ((uint8_t)adc_dec16(s_a, 0x07, s_status_c));
bb_32:
bb_33:
  /*$34B4*/ CYCLES(0x34b4, 18);
            ram_poke(0x0073, s_a);
  /*$34B6*/ s_a = ram_peek(0x0071);
  /*$34B8*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_35;
  /*$34B9*/ s_a = (uint8_t)((s_a + ram_peek(0x0077)) + s_status_c);
            goto bb_36;
bb_35:
  /*$34B9*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x0077), s_status_c));
bb_36:
  /*$34BB*/ tmp1_U8 = s_a;
            ram_poke(0x0071, tmp1_U8);
            branchTarget = true;
            if ((tmp1_U8 >= 0xa0))
              goto bb_38;
  /*$34BF*/ CYCLES_EDGE(0x34bf, 1);
  /*$34C8*/ CYCLES(0x34c8, 7);
            tmp1_U8 = ram_peek(0x0072);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!(tmp1_U8 >= 0xf0))
              goto bb_39;
            else
              goto bb_40;
bb_38:
  /*$34C1*/ CYCLES(0x34c1, 11);
  /*$34C3*/ ram_poke(0x0070, 0x00);
  /*$34C5*/ if (ret_addr) pop16(); return;
bb_39:
  /*$34CC*/ CYCLES_EDGE(0x34cc, 1);
            branchTarget = true;
            goto bb_42;
bb_40:
  /*$34CE*/ CYCLES(0x34ce, 4);
            branchTarget = true;
            if ((s_a >= 0xf8))
              goto bb_42;
  /*$34D0*/ CYCLES_EDGE(0x34d0, 1);
  /*$3510*/ CYCLES(0x3510, 11);
  /*$3512*/ ram_poke(0x0070, 0x00);
  /*$3514*/ if (ret_addr) pop16(); return;
bb_42:
  /*$34D2*/ CYCLES(0x34d2, 7);
            branchTarget = true;
            if (!(ram_peek(0x0070) != 0x03))
              goto bb_44;
  /*$34D6*/ CYCLES_EDGE(0x34d6, 1);
            branchTarget = true;
            goto bb_49;
bb_44:
  /*$34D8*/ CYCLES(0x34d8, 7);
            tmp1_U8 = (uint8_t)(ram_peek(0x0075) - 0x01);
            ram_poke(0x0075, tmp1_U8);
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_46;
  /*$34DA*/ CYCLES_EDGE(0x34da, 1);
            branchTarget = true;
            goto bb_49;
bb_46:
  /*$34DC*/ CYCLES(0x34dc, 12);
  /*$34DE*/ ram_poke(0x0075, 0x04);
  /*$34E0*/ tmp1_U8 = (uint8_t)(ram_peek(0x0074) - 0x01);
            ram_poke(0x0074, tmp1_U8);
            branchTarget = true;
            if (tmp1_U8) {
    /*$34E2*/ CYCLES_EDGE(0x34e2, 1);
              branchTarget = true;
            } else {
    /*$34E4*/ CYCLES(0x34e4, 5);
    /*$34E6*/ ram_poke(0x0074, 0x01);
            }
bb_49:
  /*$34E8*/ CYCLES(0x34e8, 55);
            tmp1_U8 = ram_peek(0x0071);
            s_a = tmp1_U8;
  /*$34EA*/ ram_poke(0x0004, tmp1_U8);
  /*$34EC*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_51;
  /*$34ED*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
            goto bb_52;
bb_51:
  /*$34ED*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
bb_52:
  /*$34EF*/ ram_poke(0x0005, s_a);
  /*$34F1*/ tmp1_U8 = ram_peek(0x0072);
            s_a = tmp1_U8;
  /*$34F3*/ ram_poke(0x0006, tmp1_U8);
  /*$34F5*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_54;
  /*$34F6*/ s_a = (uint8_t)((s_a + 0x0009) + s_status_c);
            goto bb_55;
bb_54:
  /*$34F6*/ s_a = ((uint8_t)adc_dec16(s_a, 0x09, s_status_c));
bb_55:
  /*$34F8*/ ram_poke(0x0007, s_a);
  /*$34FC*/ ram_poke(0x3413, 0x00);
  /*$34FF*/ s_a = ram_peek(0x0073);
  /*$3501*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_57;
  /*$3502*/ s_a = (uint8_t)((s_a + 0x0039) + s_status_c);
            goto bb_58;
bb_57:
  /*$3502*/ s_a = ((uint8_t)adc_dec16(s_a, 0x39, s_status_c));
bb_58:
  /*$3504*/ ram_poke(0x3414, s_a);
  /*$3507*/ tmp1_U8 = ram_peek(0x0074);
  /*$3509*/ ram_poke(0x0008, tmp1_U8);
  /*$350B*/ ram_poke(0x0009, tmp1_U8);
  /*$3400*/ CYCLES(0x3400, 3);
            s_x = ram_peek(0x0004);
            goto bb_1;
}


void delay_one_tick(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$3570*/ CYCLES(0x3570, 6);
            rom_mon_wait(0x3572);
  /*$3573*/ CYCLES(0x3573, 11);
  /*$3574*/ push8(s_x);
  /*$3575*/ func_aec0(0x3577);
  /*$3578*/ CYCLES(0x3578, 12);
            tmp1_U8 = pop8();
  /*$3579*/ s_x = tmp1_U8;
  /*$357A*/ if (ret_addr) pop16(); return;
}


void poll_input_select_mode(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$3584*/ CYCLES(0x3584, 10);
            tmp1_U8 = io_peek(0xc061);
  /*$3587*/ tmp2_U8 = io_peek(0xc062);
            branchTarget = true;
            if (!((tmp1_U8 & tmp2_U8) & 0x80))
              goto bb_2;
  /*$358A*/ CYCLES_EDGE(0x358a, 1);
            branchTarget = true;
            goto bb_4;
bb_2:
  /*$358C*/ CYCLES(0x358c, 10);
            tmp2_U8 = io_peek(0xc061);
  /*$358F*/ tmp1_U8 = io_peek(0xc062);
            branchTarget = true;
            if (((tmp2_U8 | tmp1_U8) & 0x80)) {
    /*$3592*/ CYCLES_EDGE(0x3592, 1);
    /*$35F0*/ CYCLES(0x35f0, 3);
    /*$3700*/ CYCLES(0x3700, 108);
    /*$3702*/ ram_poke(0x138c, 0xad);
    /*$3705*/ ram_poke(0x1300, 0xad);
    /*$370A*/ ram_poke(0x138d, 0x62);
    /*$370F*/ ram_poke(0x1301, 0x61);
    /*$3714*/ ram_poke(0x138e, 0xc0);
    /*$3717*/ ram_poke(0x1302, 0xc0);
    /*$371C*/ ram_poke(0x138f, 0x30);
    /*$371F*/ ram_poke(0x1303, 0x30);
    /*$3724*/ ram_poke(0x1390, 0x05);
    /*$3729*/ ram_poke(0x1304, 0x07);
    /*$372E*/ ram_poke(0x1305, 0x60);
    /*$3731*/ ram_poke(0x1391, 0x60);
    /*$3736*/ ram_poke(0x1060, 0x20);
    /*$373B*/ ram_poke(0x1061, 0xb0);
    /*$3740*/ ram_poke(0x1062, 0x36);
    /*$3745*/ ram_poke(0x1063, 0x4c);
    /*$374A*/ ram_poke(0x1064, 0x80);
    /*$374F*/ ram_poke(0x1065, 0x11);
    /*$3752*/ s_status_not_z = 0x80;
              s_status_n = 0x80;
    /*$3754*/ if (ret_addr) pop16(); return;
            }
bb_4:
  /*$3594*/ CYCLES(0x3594, 6);
            tmp1_U8 = io_peek(0xc000);
            s_status_not_z = tmp1_U8;
            tmp1_U8 = tmp1_U8 & 0x80;
            s_status_n = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8) {
    /*$3597*/ CYCLES_EDGE(0x3597, 1);
    /*$359A*/ CYCLES(0x359a, 110);
    /*$359C*/ ram_poke(0x138c, 0xad);
    /*$359F*/ ram_poke(0x1300, 0xad);
    /*$35A2*/ ram_poke(0x1060, 0xad);
    /*$35A7*/ ram_poke(0x138d, 0x00);
    /*$35AA*/ ram_poke(0x1301, 0x00);
    /*$35AD*/ ram_poke(0x1061, 0x00);
    /*$35B2*/ ram_poke(0x138e, 0xc0);
    /*$35B5*/ ram_poke(0x1302, 0xc0);
    /*$35B8*/ ram_poke(0x1062, 0xc0);
    /*$35BD*/ ram_poke(0x138f, 0xc9);
    /*$35C0*/ ram_poke(0x1065, 0xc9);
    /*$35C3*/ ram_poke(0x1305, 0xc9);
    /*$35C8*/ ram_poke(0x1390, 0xc1);
    /*$35CD*/ ram_poke(0x1391, 0xd0);
    /*$35D2*/ ram_poke(0x1392, 0x24);
    /*$35D7*/ ram_poke(0x1303, 0x10);
    /*$35DA*/ ram_poke(0x1063, 0x10);
    /*$35DF*/ ram_poke(0x1304, 0x2a);
    /*$35E4*/ ram_poke(0x1306, 0xda);
    /*$35E9*/ ram_poke(0x1064, 0x49);
    /*$35EC*/ s_status_not_z = 0x80;
              s_status_n = 0x80;
    /*$35EE*/ if (ret_addr) pop16(); return;
            }
  /*$3599*/ CYCLES(0x3599, 6);
            if (ret_addr) pop16(); return;
}


void func_3600(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint16_t tmp2_U16;
  uint8_t tmp3_U8;
  uint8_t tmp4_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$3600*/ CYCLES(0x3600, 19);
  /*$3602*/ ram_poke(0x361c, 0x90);
  /*$3605*/ s_a = ram_peek(0x000a);
  /*$3607*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_2;
  /*$3608*/ tmp1_U16 = s_a;
            tmp2_U16 = (tmp1_U16 + 0x0039) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp2_U16, (uint8_t)tmp1_U16, (uint8_t)0x0039);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_3;
bb_2:
  /*$3608*/ tmp2_U16 = adc_dec16(s_a, 0x39, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            s_status_v = (((uint8_t)(tmp2_U16 >> 8) & 0x40) != 0);
bb_3:
  /*$360A*/ ram_poke(0x361d, s_a);
  /*$360D*/ s_x = 0x60;
bb_4:
  /*$360F*/ CYCLES(0x360f, 16);
            tmp3_U8 = s_x;
  /*$3612*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp3_U8)));
  /*$3617*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp3_U8)));
  /*$3619*/ s_y = 0x0c;
bb_5:
  /*$361B*/ CYCLES(0x361b, 22);
            tmp3_U8 = peek(ram_peek16al(0x361c));
  /*$361E*/ tmp4_U8 = s_y;
            poke((ram_peek16al(0x0000) + tmp4_U8), tmp3_U8);
  /*$3620*/ ram_poke(0x361c, (uint8_t)(ram_peek(0x361c) + 0x01));
  /*$3623*/ tmp4_U8 = (uint8_t)(tmp4_U8 + 0x01);
            s_y = tmp4_U8;
            branchTarget = true;
            if ((tmp4_U8 >= 0x1c))
              goto bb_7;
  /*$3626*/ CYCLES_EDGE(0x3626, 1);
            branchTarget = true;
            goto bb_5;
bb_7:
  /*$3628*/ CYCLES(0x3628, 6);
            tmp4_U8 = s_x;
            tmp3_U8 = (uint8_t)(tmp4_U8 + 0x01);
            s_x = tmp3_U8;
  /*$3629*/ s_status_not_z = (tmp3_U8 != 0x67);
            tmp3_U8 = tmp3_U8 >= 0x67;
            s_status_c = tmp3_U8;
            s_status_n = ((uint8_t)(tmp4_U8 - 0x66) & 0x80);
            branchTarget = true;
            if (tmp3_U8)
              goto bb_9;
  /*$362B*/ CYCLES_EDGE(0x362b, 1);
            branchTarget = true;
            goto bb_4;
bb_9:
  /*$362D*/ CYCLES(0x362d, 6);
            if (ret_addr) pop16(); return;
}


void func_3630(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$3630*/ CYCLES(0x3630, 5);
  /*$3632*/ ram_poke(0x000a, 0x00);
bb_1:
  /*$3634*/ CYCLES(0x3634, 6);
            func_3600(0x3636);
  /*$3637*/ CYCLES(0x3637, 6);
            func_3650(0x3639);
  /*$363A*/ CYCLES(0x363a, 12);
            ram_poke(0x000a, (uint8_t)(ram_peek(0x000a) + 0x01));
  /*$363E*/ tmp1_U8 = ram_peek(0x000a) >= 0x07;
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_3;
  /*$3640*/ CYCLES_EDGE(0x3640, 1);
            branchTarget = true;
            goto bb_1;
bb_3:
  /*$3642*/ CYCLES(0x3642, 2);
            s_x = 0x04;
bb_4:
  /*$3644*/ CYCLES(0x3644, 8);
            s_a = 0x00;
  /*$3646*/ rom_mon_wait(0x3648);
  /*$3649*/ CYCLES(0x3649, 4);
            tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_x = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_6;
  /*$364A*/ CYCLES_EDGE(0x364a, 1);
            branchTarget = true;
            goto bb_4;
bb_6:
  /*$364C*/ CYCLES(0x364c, 6);
            if (ret_addr) pop16(); return;
}


void func_3650(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$3650*/ CYCLES(0x3650, 2);
            s_x = 0x01;
bb_1:
  /*$3652*/ CYCLES(0x3652, 8);
            s_a = 0x80;
  /*$3654*/ rom_mon_wait(0x3656);
  /*$3657*/ CYCLES(0x3657, 12);
            poke(ram_peek16al(0x3658), s_a);
  /*$365A*/ s_a = 0x50;
  /*$365C*/ rom_mon_wait(0x365e);
  /*$365F*/ CYCLES(0x365f, 8);
            poke(ram_peek16al(0x3660), s_a);
  /*$3662*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_3;
  /*$3663*/ CYCLES_EDGE(0x3663, 1);
            branchTarget = true;
            goto bb_1;
bb_3:
  /*$3665*/ CYCLES(0x3665, 5);
            s_a = 0x00;
  /*$3667*/ rom_mon_wait(0x0000);
            if (ret_addr) pop16(); return;
}


void func_37a0(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;
  uint8_t tmp4_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$37A0*/ CYCLES(0x37a0, 29);
  /*$37A2*/ ram_poke(0x37c5, 0xf0);
  /*$37A5*/ s_a = ram_peek(0x0006);
  /*$37A7*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_2;
  /*$37A8*/ s_a = (uint8_t)((s_a + 0x0060) + s_status_c);
            goto bb_3;
bb_2:
  /*$37A8*/ s_a = ((uint8_t)adc_dec16(s_a, 0x60, s_status_c));
bb_3:
  /*$37AA*/ ram_poke(0x37c6, s_a);
  /*$37AD*/ s_a = ram_peek(0x0004);
  /*$37AF*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_5;
  /*$37B0*/ s_a = (uint8_t)((s_a + 0x0005) + s_status_c);
            goto bb_6;
bb_5:
  /*$37B0*/ s_a = ((uint8_t)adc_dec16(s_a, 0x05, s_status_c));
bb_6:
  /*$37B2*/ ram_poke(0x0007, s_a);
  /*$37B4*/ s_x = 0x00;
bb_7:
  /*$37B6*/ CYCLES(0x37b6, 50);
            tmp1_U8 = ram_peek(0x0004);
  /*$37BB*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$37C0*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$37C2*/ tmp1_U8 = ram_peek(0x0005);
            s_y = tmp1_U8;
  /*$37C4*/ tmp2_U8 = s_x;
            tmp3_U8 = peek((ram_peek16(0x37c5) + tmp2_U8));
  /*$37C7*/ tmp4_U8 = peek((ram_peek16al(0x0000) + tmp1_U8));
  /*$37C9*/ poke((ram_peek16al(0x0000) + tmp1_U8), (tmp3_U8 | tmp4_U8));
  /*$37CB*/ s_x = (uint8_t)(tmp2_U8 + 0x01);
  /*$37CC*/ ram_poke(0x0004, (uint8_t)(ram_peek(0x0004) + 0x01));
            branchTarget = true;
            if ((ram_peek(0x0004) >= ram_peek(0x0007)))
              goto bb_9;
  /*$37D2*/ CYCLES_EDGE(0x37d2, 1);
            branchTarget = true;
            goto bb_7;
bb_9:
  /*$37D4*/ CYCLES(0x37d4, 6);
            if (ret_addr) pop16(); return;
}


void func_3800(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;
  uint8_t tmp4_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$3800*/ CYCLES(0x3800, 2);
            s_x = 0x02;
bb_1:
  /*$3802*/ CYCLES(0x3802, 12);
            tmp1_U8 = s_x;
  /*$3804*/ ram_poke((uint8_t)(0x04 + tmp1_U8), ram_peek((uint8_t)(ram_peek(0x3803) + tmp1_U8)));
  /*$3806*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_3;
  /*$3807*/ CYCLES_EDGE(0x3807, 1);
            branchTarget = true;
            goto bb_1;
bb_3:
  /*$3809*/ CYCLES(0x3809, 6);
            func_3860(0x380b);
  /*$380C*/ CYCLES(0x380c, 5);
            branchTarget = true;
            if (!ram_peek(0x00a0))
              goto bb_5;
  /*$380E*/ CYCLES_EDGE(0x380e, 1);
            branchTarget = true;
            goto bb_24;
bb_5:
  /*$3810*/ CYCLES(0x3810, 12);
            ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) + 0x01));
  /*$3812*/ tmp1_U8 = ram_peek(0x0006);
            s_a = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0x07))
              goto bb_7;
  /*$3816*/ CYCLES_EDGE(0x3816, 1);
            branchTarget = true;
            goto bb_8;
bb_7:
  /*$3818*/ CYCLES(0x3818, 2);
            s_a = 0x00;
bb_8:
  /*$381A*/ CYCLES(0x381a, 15);
            ram_poke(0x0006, s_a);
  /*$381C*/ ram_poke(0x0004, (uint8_t)(ram_peek(0x0004) - 0x01));
            branchTarget = true;
            if (!(ram_peek(0x0004) >= ram_peek(0x3821)))
              goto bb_10;
  /*$3822*/ CYCLES_EDGE(0x3822, 1);
            branchTarget = true;
            goto bb_20;
bb_10:
  /*$3824*/ CYCLES(0x3824, 18);
  /*$3826*/ ram_poke(0x0004, 0xa0);
  /*$3828*/ s_a = ram_peek(0x0005);
  /*$382A*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_12;
  /*$382B*/ s_a = (uint8_t)((s_a + 0x0008) + s_status_c);
            goto bb_13;
bb_12:
  /*$382B*/ s_a = ((uint8_t)adc_dec16(s_a, 0x08, s_status_c));
bb_13:
            branchTarget = true;
            if ((s_a >= 0x28))
              goto bb_15;
  /*$3830*/ CYCLES_EDGE(0x3830, 1);
            branchTarget = true;
            goto bb_19;
bb_15:
  /*$3832*/ CYCLES(0x3832, 4);
            s_status_c = 0x01;
            if (s_status_d)
              goto bb_17;
  /*$3833*/ s_a = (uint8_t)((s_a - 0x0028) - (uint8_t)(0x01 - s_status_c));
            goto bb_18;
bb_17:
  /*$3833*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x28, s_status_c));
bb_18:
bb_19:
  /*$3835*/ CYCLES(0x3835, 3);
            ram_poke(0x0005, s_a);
bb_20:
  /*$3837*/ CYCLES(0x3837, 2);
            s_x = 0x02;
bb_21:
  /*$3839*/ CYCLES(0x3839, 12);
            tmp1_U8 = s_x;
  /*$383B*/ ram_poke((uint8_t)(ram_peek(0x383c) + tmp1_U8), ram_peek((uint8_t)(0x04 + tmp1_U8)));
  /*$383D*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_23;
  /*$383E*/ CYCLES_EDGE(0x383e, 1);
            branchTarget = true;
            goto bb_21;
bb_23:
  /*$3840*/ CYCLES(0x3840, 6);
            func_37a0(0x3842);
            branchTarget = true;
bb_24:
  /*$3843*/ CYCLES(0x3843, 12);
            s_a = ram_peek(0x3803);
  /*$3846*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_26;
  /*$3847*/ tmp2_U16 = s_a;
            tmp3_U16 = (tmp2_U16 + 0x0003) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0x0003);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_27;
bb_26:
  /*$3847*/ tmp3_U16 = adc_dec16(s_a, 0x03, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_27:
            branchTarget = true;
            if ((s_a >= 0xb9))
              goto bb_29;
  /*$384B*/ CYCLES_EDGE(0x384b, 1);
            branchTarget = true;
            goto bb_30;
bb_29:
  /*$384D*/ CYCLES(0x384d, 2);
            s_a = 0xb0;
bb_30:
  /*$384F*/ CYCLES(0x384f, 12);
            tmp1_U8 = s_a;
            ram_poke(0x3803, tmp1_U8);
  /*$3852*/ ram_poke(0x383c, tmp1_U8);
  /*$3855*/ tmp4_U8 = tmp1_U8 != 0xb0;
            s_status_not_z = tmp4_U8;
            s_status_c = (tmp1_U8 >= 0xb0);
            s_status_n = ((uint8_t)(tmp1_U8 - 0xb0) & 0x80);
            branchTarget = true;
            if (!tmp4_U8)
              goto bb_32;
  /*$3857*/ CYCLES_EDGE(0x3857, 1);
            branchTarget = true;
            goto bb_0;
bb_32:
  /*$3859*/ CYCLES(0x3859, 6);
            if (ret_addr) pop16(); return;
}


void func_3860(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;
  uint8_t tmp4_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$3860*/ CYCLES(0x3860, 29);
  /*$3862*/ ram_poke(0x3885, 0xf0);
  /*$3865*/ s_a = ram_peek(0x0006);
  /*$3867*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_2;
  /*$3868*/ s_a = (uint8_t)((s_a + 0x0060) + s_status_c);
            goto bb_3;
bb_2:
  /*$3868*/ s_a = ((uint8_t)adc_dec16(s_a, 0x60, s_status_c));
bb_3:
  /*$386A*/ ram_poke(0x3886, s_a);
  /*$386D*/ s_a = ram_peek(0x0004);
  /*$386F*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_5;
  /*$3870*/ s_a = (uint8_t)((s_a + 0x0005) + s_status_c);
            goto bb_6;
bb_5:
  /*$3870*/ s_a = ((uint8_t)adc_dec16(s_a, 0x05, s_status_c));
bb_6:
  /*$3872*/ ram_poke(0x0007, s_a);
  /*$3874*/ s_x = 0x00;
bb_7:
  /*$3876*/ CYCLES(0x3876, 52);
            tmp1_U8 = ram_peek(0x0004);
  /*$387B*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$3880*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$3882*/ tmp1_U8 = ram_peek(0x0005);
            s_y = tmp1_U8;
  /*$3884*/ tmp2_U8 = s_x;
            tmp3_U8 = peek((ram_peek16(0x3885) + tmp2_U8));
  /*$3889*/ tmp4_U8 = peek((ram_peek16al(0x0000) + tmp1_U8));
  /*$388B*/ poke((ram_peek16al(0x0000) + tmp1_U8), ((tmp3_U8 ^ 0xff) & tmp4_U8));
  /*$388D*/ s_x = (uint8_t)(tmp2_U8 + 0x01);
  /*$388E*/ ram_poke(0x0004, (uint8_t)(ram_peek(0x0004) + 0x01));
  /*$3890*/ tmp2_U8 = ram_peek(0x0004);
            s_a = tmp2_U8;
            branchTarget = true;
            if ((tmp2_U8 >= ram_peek(0x0007)))
              goto bb_9;
  /*$3894*/ CYCLES_EDGE(0x3894, 1);
            branchTarget = true;
            goto bb_7;
bb_9:
  /*$3896*/ CYCLES(0x3896, 13);
            s_status_c = 0x01;
            if (s_status_d)
              goto bb_11;
  /*$3897*/ s_a = (uint8_t)((s_a - 0x0005) - (uint8_t)(0x01 - s_status_c));
            goto bb_12;
bb_11:
  /*$3897*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x05, s_status_c));
bb_12:
  /*$3899*/ ram_poke(0x0004, s_a);
  /*$389B*/ if (ret_addr) pop16(); return;
}


void func_ac00(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$AC00*/ CYCLES(0xac00, 8);
            tmp1_U8 = ram_peek(0x0085);
            s_a = tmp1_U8;
  /*$AC02*/ tmp2_U8 = ram_peek(0x0082);
            s_status_c = (tmp1_U8 >= tmp2_U8);
            branchTarget = true;
            if ((tmp1_U8 != tmp2_U8))
              goto bb_2;
  /*$AC04*/ CYCLES_EDGE(0xac04, 1);
            branchTarget = true;
            goto bb_6;
bb_2:
  /*$AC06*/ CYCLES(0xac06, 2);
            branchTarget = true;
            if (!s_status_c)
              goto bb_4;
  /*$AC06*/ CYCLES_EDGE(0xac06, 1);
            branchTarget = true;
            goto bb_19;
bb_4:
  /*$AC08*/ CYCLES(0xac08, 2);
            branchTarget = true;
            if (s_status_c)
              goto bb_6;
  /*$AC08*/ CYCLES_EDGE(0xac08, 1);
            branchTarget = true;
            goto bb_18;
bb_6:
  /*$AC0A*/ CYCLES(0xac0a, 8);
            tmp2_U8 = ram_peek(0x0084);
            s_a = tmp2_U8;
  /*$AC0C*/ tmp1_U8 = ram_peek(0x0081);
            s_status_c = (tmp2_U8 >= tmp1_U8);
            branchTarget = true;
            if ((tmp2_U8 != tmp1_U8))
              goto bb_8;
  /*$AC0E*/ CYCLES_EDGE(0xac0e, 1);
            branchTarget = true;
            goto bb_12;
bb_8:
  /*$AC10*/ CYCLES(0xac10, 2);
            branchTarget = true;
            if (!s_status_c)
              goto bb_10;
  /*$AC10*/ CYCLES_EDGE(0xac10, 1);
            branchTarget = true;
            goto bb_19;
bb_10:
  /*$AC12*/ CYCLES(0xac12, 2);
            branchTarget = true;
            if (s_status_c)
              goto bb_12;
  /*$AC12*/ CYCLES_EDGE(0xac12, 1);
            branchTarget = true;
            goto bb_18;
bb_12:
  /*$AC14*/ CYCLES(0xac14, 8);
            tmp1_U8 = ram_peek(0x0083);
            s_a = tmp1_U8;
  /*$AC16*/ tmp1_U8 = tmp1_U8 >= ram_peek(0x0080);
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_14;
  /*$AC18*/ CYCLES_EDGE(0xac18, 1);
            branchTarget = true;
            goto bb_19;
bb_14:
  /*$AC1A*/ CYCLES(0xac1a, 2);
            branchTarget = true;
            if (s_status_c)
              goto bb_16;
  /*$AC1A*/ CYCLES_EDGE(0xac1a, 1);
            branchTarget = true;
            goto bb_18;
bb_16:
  /*$AC1C*/ CYCLES(0xac1c, 2);
            branchTarget = true;
            if (s_status_c)
              goto bb_18;
  /*$AC1C*/ CYCLES_EDGE(0xac1c, 1);
            branchTarget = true;
            goto bb_0;
bb_18:
  /*$AC1E*/ CYCLES(0xac1e, 18);
  /*$AC20*/ ram_poke(0x0083, ram_peek(0x0080));
  /*$AC24*/ ram_poke(0x0084, ram_peek(0x0081));
  /*$AC26*/ tmp1_U8 = ram_peek(0x0082);
            s_a = tmp1_U8;
  /*$AC28*/ ram_poke(0x0085, tmp1_U8);
bb_19:
  /*$AC2A*/ CYCLES(0xac2a, 6);
            if (ret_addr) pop16(); return;
}


void func_ae00(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;
  uint16_t tmp4_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$AE00*/ CYCLES(0xae00, 3);
            s_x = ram_peek(0x0004);
bb_1:
  /*$AE02*/ CYCLES(0xae02, 41);
            tmp1_U8 = s_x;
  /*$AE05*/ ram_poke(0x0001, ram_peek((0xaf40 + tmp1_U8)));
  /*$AE0A*/ ram_poke(0x0000, ram_peek((0xaf00 + tmp1_U8)));
  /*$AE0C*/ tmp1_U8 = ram_peek(0x0006);
  /*$AE14*/ ram_poke(0x0003, ram_peek((0x0c00 + tmp1_U8)));
  /*$AE19*/ ram_poke(0x0002, ram_peek((0x0d00 + tmp1_U8)));
            branchTarget = true;
            if (!ram_peek(0xae1c))
              goto bb_3;
  /*$AE1D*/ CYCLES_EDGE(0xae1d, 1);
            branchTarget = true;
            goto bb_8;
bb_3:
  /*$AE1F*/ CYCLES(0xae1f, 3);
            s_y = ram_peek(0x0008);
bb_4:
  /*$AE21*/ CYCLES(0xae21, 18);
            tmp1_U8 = s_y;
            tmp2_U8 = peek((ram_peek16al(0x0000) + tmp1_U8));
  /*$AE23*/ poke((ram_peek16al(0x0002) + tmp1_U8), tmp2_U8);
  /*$AE25*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
            s_y = tmp1_U8;
  /*$AE26*/ tmp1_U8 = tmp1_U8 >= ram_peek(0x0009);
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_6;
  /*$AE28*/ CYCLES_EDGE(0xae28, 1);
            branchTarget = true;
            goto bb_4;
bb_6:
  /*$AE2A*/ CYCLES(0xae2a, 2);
            branchTarget = true;
            if (!s_status_c)
              goto bb_8;
  /*$AE2A*/ CYCLES_EDGE(0xae2a, 1);
            branchTarget = true;
            goto bb_11;
bb_8:
  /*$AE2C*/ CYCLES(0xae2c, 3);
            s_y = ram_peek(0x0008);
bb_9:
  /*$AE2E*/ CYCLES(0xae2e, 23);
            tmp1_U8 = s_y;
            tmp2_U8 = peek((ram_peek16al(0x0000) + tmp1_U8));
  /*$AE30*/ tmp3_U8 = peek((ram_peek16al(0x0002) + tmp1_U8));
  /*$AE32*/ poke((ram_peek16al(0x0002) + tmp1_U8), (tmp2_U8 | tmp3_U8));
  /*$AE34*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= ram_peek(0x0009)))
              goto bb_11;
  /*$AE37*/ CYCLES_EDGE(0xae37, 1);
            branchTarget = true;
            goto bb_9;
bb_11:
  /*$AE39*/ CYCLES(0xae39, 15);
            s_x = (uint8_t)(s_x + 0x01);
  /*$AE3A*/ s_a = ram_peek(0x000b);
  /*$AE3C*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_13;
  /*$AE3D*/ tmp4_U16 = (s_a + ram_peek(0x000a)) + s_status_c;
            s_status_c = (uint8_t)(tmp4_U16 >> 8);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_14;
bb_13:
  /*$AE3D*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x000a), s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
bb_14:
  /*$AE3F*/ ram_poke(0x000b, s_a);
            branchTarget = true;
            if (s_status_c)
              goto bb_16;
  /*$AE41*/ CYCLES_EDGE(0xae41, 1);
            branchTarget = true;
            goto bb_18;
bb_16:
  /*$AE43*/ CYCLES(0xae43, 11);
            ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) - 0x01));
  /*$AE45*/ func_ae58(0xae47);
  /*$AE48*/ CYCLES(0xae48, 2);
            branchTarget = true;
            if (!s_status_not_z)
              goto bb_18;
  /*$AE48*/ CYCLES_EDGE(0xae48, 1);
            branchTarget = true;
            goto bb_19;
bb_18:
  /*$AE4A*/ CYCLES(0xae4a, 6);
  /*$AE4C*/ ram_poke(0xae1c, 0x00);
bb_19:
  /*$AE4F*/ CYCLES(0xae4f, 10);
            ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) + 0x01));
            branchTarget = true;
            if ((s_x >= ram_peek(0x0005)))
              goto bb_21;
  /*$AE53*/ CYCLES_EDGE(0xae53, 1);
            branchTarget = true;
            goto bb_1;
bb_21:
  /*$AE55*/ CYCLES(0xae55, 6);
            if (ret_addr) pop16(); return;
}


void func_ae58(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$AE58*/ CYCLES(0xae58, 16);
            tmp1_U8 = (uint8_t)(ram_peek(0xae1c) + 0x01);
            s_status_not_z = tmp1_U8;
            ram_poke(0xae1c, tmp1_U8);
  /*$AE5B*/ poke(ram_peek16al(0xae5c), s_a);
  /*$AE5E*/ if (ret_addr) pop16(); return;
}


void func_ae80(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t tmp1_U16;
  uint16_t tmp2_U16;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$AE80*/ CYCLES(0xae80, 42);
  /*$AE82*/ ram_poke(0x0004, 0x00);
  /*$AE86*/ ram_poke(0x0005, 0x30);
  /*$AE8A*/ ram_poke(0x0006, 0x07);
  /*$AE8E*/ ram_poke(0x0008, 0x05);
  /*$AE92*/ ram_poke(0x0009, 0x22);
  /*$AE96*/ ram_poke(0x000b, 0x00);
  /*$AE9A*/ ram_poke(0xae1c, 0x00);
  /*$AE9D*/ func_ae00(0xae9f);
  /*$AEA0*/ CYCLES(0xaea0, 7);
            branchTarget = true;
            if (!(ram_peek(0x000a) & 0x1f))
              goto bb_2;
  /*$AEA4*/ CYCLES_EDGE(0xaea4, 1);
            branchTarget = true;
            goto bb_3;
bb_2:
  /*$AEA6*/ CYCLES(0xaea6, 6);
            func_aec0(0xaea8);
            branchTarget = true;
bb_3:
  /*$AEA9*/ CYCLES(0xaea9, 12);
            s_a = ram_peek(0x000a);
  /*$AEAB*/ s_status_c = 0x01;
            if (s_status_d)
              goto bb_5;
  /*$AEAC*/ tmp1_U16 = s_a;
            tmp2_U16 = (tmp1_U16 - 0x0008) - (uint8_t)(0x01 - s_status_c);
            s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp2_U16 >> 8) & 0x01));
            s_status_v = ovf8((uint8_t)tmp2_U16, (uint8_t)tmp1_U16, (uint8_t)0xfff7);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_6;
bb_5:
  /*$AEAC*/ tmp2_U16 = sbc_dec16(s_a, 0x08, s_status_c);
            s_a = ((uint8_t)tmp2_U16);
            tmp3_U8 = (uint8_t)(tmp2_U16 >> 8);
            s_status_c = (tmp3_U8 & 0x01);
            s_status_v = ((tmp3_U8 & 0x40) != 0);
bb_6:
  /*$AEAE*/ ram_poke(0x000a, s_a);
            branchTarget = true;
            if (!s_status_c)
              goto bb_8;
  /*$AEB0*/ CYCLES_EDGE(0xaeb0, 1);
            branchTarget = true;
            goto bb_0;
bb_8:
  /*$AEB2*/ CYCLES(0xaeb2, 11);
            s_status_not_z = 0xf8;
            s_status_n = 0x80;
            s_a = 0xf8;
  /*$AEB4*/ ram_poke(0x000a, 0xf8);
  /*$AEB6*/ if (ret_addr) pop16(); return;
}


void func_aec0(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$AEC0*/ CYCLES(0xaec0, 2);
            s_x = 0x54;
bb_1:
  /*$AEC2*/ CYCLES(0xaec2, 16);
            tmp1_U8 = s_x;
  /*$AEC5*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$AECA*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$AECC*/ s_y = 0x02;
bb_2:
  /*$AECE*/ CYCLES(0xaece, 19);
            tmp1_U8 = s_y;
            tmp2_U8 = peek((ram_peek16al(0x0000) + tmp1_U8));
  /*$AED2*/ poke((ram_peek16al(0x0000) + tmp1_U8), (tmp2_U8 ^ 0x80));
  /*$AED4*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0x15))
              goto bb_4;
  /*$AED7*/ CYCLES_EDGE(0xaed7, 1);
            branchTarget = true;
            goto bb_2;
bb_4:
  /*$AED9*/ CYCLES(0xaed9, 6);
            tmp1_U8 = (uint8_t)(s_x + 0x01);
            s_x = tmp1_U8;
  /*$AEDA*/ tmp1_U8 = tmp1_U8 >= 0x70;
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_6;
  /*$AEDC*/ CYCLES_EDGE(0xaedc, 1);
            branchTarget = true;
            goto bb_1;
bb_6:
  /*$AEDE*/ CYCLES(0xaede, 6);
            if (ret_addr) pop16(); return;
}


void func_b9a0(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$B9A0*/ CYCLES(0xb9a0, 20);
  /*$B9A2*/ ram_poke(0x0000, 0x00);
  /*$B9A6*/ ram_poke(0x0001, 0x40);
  /*$B9AA*/ ram_poke(0x0004, 0x00);
  /*$B9AE*/ ram_poke(0x0005, 0xb5);
bb_1:
  /*$B9B0*/ CYCLES(0xb9b0, 9);
            s_y = 0x00;
  /*$B9B2*/ tmp1_U8 = peek(ram_peek16al(0x0004));
            s_status_n = (tmp1_U8 & 0x80);
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_3;
  /*$B9B4*/ CYCLES_EDGE(0xb9b4, 1);
  /*$B9B7*/ CYCLES(0xb9b7, 2);
            branchTarget = true;
            if (s_status_n)
              goto bb_4;
            else
              goto bb_5;
bb_3:
  /*$B9B6*/ CYCLES(0xb9b6, 6);
            if (ret_addr) pop16(); return;
bb_4:
  /*$B9B7*/ CYCLES_EDGE(0xb9b7, 1);
  /*$B9D8*/ CYCLES(0xb9d8, 8);
  /*$B9DA*/ s_x = (s_a & 0x7f);
  /*$B9DB*/ s_y = 0x00;
  /*$B9DD*/ s_a = 0x00;
            goto bb_19;
bb_5:
  /*$B9B9*/ CYCLES(0xb9b9, 24);
            poke((ram_peek16al(0x0000) + s_y), s_a);
  /*$B9BB*/ s_a = ram_peek(0x0000);
  /*$B9BD*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_7;
  /*$B9BE*/ tmp2_U16 = (s_a + 0x0001) + s_status_c;
            s_status_c = (uint8_t)(tmp2_U16 >> 8);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_8;
bb_7:
  /*$B9BE*/ tmp3_U16 = adc_dec16(s_a, 0x01, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
bb_8:
  /*$B9C0*/ ram_poke(0x0000, s_a);
  /*$B9C2*/ s_a = ram_peek(0x0001);
            if (s_status_d)
              goto bb_10;
  /*$B9C4*/ s_a = (uint8_t)(s_a + s_status_c);
            goto bb_11;
bb_10:
  /*$B9C4*/ s_a = ((uint8_t)adc_dec16(s_a, 0x00, s_status_c));
bb_11:
  /*$B9C6*/ ram_poke(0x0001, s_a);
bb_12:
  /*$B9C8*/ CYCLES(0xb9c8, 21);
            s_a = ram_peek(0x0004);
  /*$B9CA*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_14;
  /*$B9CB*/ tmp2_U16 = (s_a + 0x0001) + s_status_c;
            s_status_c = (uint8_t)(tmp2_U16 >> 8);
            s_a = ((uint8_t)tmp2_U16);
            goto bb_15;
bb_14:
  /*$B9CB*/ tmp3_U16 = adc_dec16(s_a, 0x01, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
bb_15:
  /*$B9CD*/ ram_poke(0x0004, s_a);
  /*$B9CF*/ s_a = ram_peek(0x0005);
            if (s_status_d)
              goto bb_17;
  /*$B9D1*/ tmp2_U16 = s_a;
            tmp3_U16 = tmp2_U16 + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0x0000);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_18;
bb_17:
  /*$B9D1*/ tmp3_U16 = adc_dec16(s_a, 0x00, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_18:
  /*$B9D3*/ ram_poke(0x0005, s_a);
            branchTarget = true;
            goto bb_1;
bb_19:
  /*$B9DF*/ CYCLES(0xb9df, 12);
            tmp1_U8 = s_y;
            poke((ram_peek16al(0x0000) + tmp1_U8), s_a);
  /*$B9E1*/ s_y = (uint8_t)(tmp1_U8 + 0x01);
  /*$B9E2*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_21;
  /*$B9E3*/ CYCLES_EDGE(0xb9e3, 1);
            branchTarget = true;
            goto bb_19;
bb_21:
  /*$B9E5*/ CYCLES(0xb9e5, 21);
            s_a = s_y;
  /*$B9E6*/ s_status_c = 0x00;
            if (s_status_d)
              goto bb_23;
  /*$B9E7*/ tmp3_U16 = (s_a + ram_peek(0x0000)) + s_status_c;
            s_status_c = (uint8_t)(tmp3_U16 >> 8);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_24;
bb_23:
  /*$B9E7*/ tmp3_U16 = adc_dec16(s_a, ram_peek(0x0000), s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_c = ((uint8_t)(tmp3_U16 >> 8) & 0x01);
bb_24:
  /*$B9E9*/ ram_poke(0x0000, s_a);
  /*$B9EB*/ s_a = ram_peek(0x0001);
            if (s_status_d)
              goto bb_26;
  /*$B9ED*/ s_a = (uint8_t)(s_a + s_status_c);
            goto bb_27;
bb_26:
  /*$B9ED*/ s_a = ((uint8_t)adc_dec16(s_a, 0x00, s_status_c));
bb_27:
  /*$B9EF*/ ram_poke(0x0001, s_a);
            branchTarget = true;
            goto bb_12;
}


void func_ba00(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$BA00*/ CYCLES(0xba00, 6);
            func_b9a0(0xba02);
  /*$BA03*/ CYCLES(0xba03, 2);
            s_x = 0x00;
bb_1:
  /*$BA05*/ CYCLES(0xba05, 40);
            tmp1_U8 = s_x;
  /*$BA08*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
  /*$BA0D*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
  /*$BA13*/ poke(ram_peek16al(0x0000), 0x02);
  /*$BA19*/ poke((ram_peek16al(0x0000) + 0x0027), 0x40);
  /*$BA1B*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 >= 0xa5))
              goto bb_3;
  /*$BA1E*/ CYCLES_EDGE(0xba1e, 1);
            branchTarget = true;
            goto bb_1;
bb_3:
  /*$BA20*/ CYCLES(0xba20, 7);
  /*$BA22*/ ram_poke(0x0002, 0x55);
  /*$BA24*/ s_y = 0x27;
bb_4:
  /*$BA26*/ CYCLES(0xba26, 21);
  /*$BA28*/ tmp1_U8 = s_y;
            poke((ram_peek16al(0x0000) + tmp1_U8), ram_peek(0x0002));
  /*$BA2E*/ ram_poke(0x0002, (ram_peek(0x0002) ^ 0x7f));
  /*$BA30*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_6;
  /*$BA31*/ CYCLES_EDGE(0xba31, 1);
            branchTarget = true;
            goto bb_4;
bb_6:
  /*$BA33*/ CYCLES(0xba33, 18);
            s_x = 0x00;
  /*$BA38*/ ram_poke(0x0001, ram_peek(0x0c00));
  /*$BA3D*/ ram_poke(0x0000, ram_peek(0x0d00));
  /*$BA3F*/ s_y = 0x27;
bb_7:
  /*$BA41*/ CYCLES(0xba41, 21);
  /*$BA43*/ tmp1_U8 = s_y;
            poke((ram_peek16al(0x0000) + tmp1_U8), ram_peek(0x0002));
  /*$BA49*/ ram_poke(0x0002, (ram_peek(0x0002) ^ 0x7f));
  /*$BA4B*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_y = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_9;
  /*$BA4C*/ CYCLES_EDGE(0xba4c, 1);
            branchTarget = true;
            goto bb_7;
bb_9:
  /*$BA4E*/ CYCLES(0xba4e, 6);
            if (ret_addr) pop16(); return;
}


void func_bad8(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$BAD8*/ CYCLES(0xbad8, 2);
            s_x = 0x43;
bb_1:
  /*$BADA*/ CYCLES(0xbada, 33);
            tmp1_U8 = s_x;
  /*$BADD*/ ram_poke(0x0000, ram_peek((0xba50 + tmp1_U8)));
  /*$BAE2*/ ram_poke(0x0001, ram_peek((0xba94 + tmp1_U8)));
  /*$BAE4*/ s_y = 0x00;
  /*$BAE6*/ tmp2_U8 = peek(ram_peek16al(0x0000));
  /*$BAEA*/ poke(ram_peek16al(0x0000), (tmp2_U8 ^ 0x80));
  /*$BAEC*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            s_x = tmp1_U8;
            branchTarget = true;
            if ((tmp1_U8 & 0x80))
              goto bb_3;
  /*$BAED*/ CYCLES_EDGE(0xbaed, 1);
            branchTarget = true;
            goto bb_1;
bb_3:
  /*$BAEF*/ CYCLES(0xbaef, 6);
            if (ret_addr) pop16(); return;
}


void func_baf0(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$BAF0*/ CYCLES(0xbaf0, 6);
            func_ba00(0xbaf2);
  /*$BAF3*/ CYCLES(0xbaf3, 6);
            func_bad8(0xbaf5);
  /*$BAF6*/ CYCLES(0xbaf6, 11);
  /*$BAF8*/ ram_poke(0x000a, 0xf8);
  /*$BAFA*/ func_ae80(0xbafc);
  /*$BAFD*/ CYCLES(0xbafd, 3);
            func_ae80(0x0000);
            if (ret_addr) pop16(); return;
}


void FUNC_SAV1(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

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
  uint8_t tmp3_U8;
  uint16_t tmp4_U16;
  uint16_t tmp5_U16;
  uint16_t tmp6_U16;
  uint8_t tmp7_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  for(;;) {
    switch (block_id) {
    case 0:
                s_a = 0x3c;
                s_x = 0xf8;
                s_y = 0xd4;
                s_sp = 0xf8;
                s_status_n = 0x00;
                s_status_v = 0x00;
                s_status_b = 0x00;
                s_status_d = 0x00;
                s_status_i = 0x00;
                s_status_not_z = 0x01;
                s_status_c = 0x00;
      /*$0000*/ CYCLES(0x0000, 3);
                branchTarget = true; block_id = find_block_id_func_t001(0x0000, ram_peek16(0x0001));
      break;
    case 1:  // $0100
      /*$0100*/ CYCLES(0x0100, 53);
      /*$0102*/ ram_poke(0x0000, 0xd0);
      /*$0106*/ ram_poke(0x0001, 0x5e);
      /*$010A*/ ram_poke(0x0002, 0x01);
      /*$010C*/ tmp1_U8 = io_peek(0xc050);
      /*$010F*/ tmp1_U8 = io_peek(0xc052);
      /*$0112*/ tmp1_U8 = io_peek(0xc055);
      /*$0115*/ tmp1_U8 = io_peek(0xc057);
      /*$011A*/ s_sp = 0xff;
      /*$011B*/ s_y = 0x19;
      /*$0121*/ push8(0x20);
      /*$0122*/ s_a = 0xa8;
      /*$0124*/ tmp1_U8 = pop8();
                s_status_c = (tmp1_U8 & 0x01);
                s_status_i = ((tmp1_U8 & 0x04) != 0);
                s_status_d = ((tmp1_U8 & 0x08) != 0);
                s_status_b = 0x00;
                s_status_v = ((tmp1_U8 & 0x40) != 0);
      /*$0125*/ branchTarget = true; block_id = 1636;
      break;
    case 2:  // $0E00
      /*$0E00*/ CYCLES(0x0e00, 25);
      /*$0E02*/ ram_poke(0x0004, 0x02);
      /*$0E06*/ ram_poke(0x0002, ram_peek(0x00f0));
      /*$0E0A*/ ram_poke(0x0003, ram_peek(0x00f1));
      /*$0E0E*/ ram_poke(0x0005, 0x00);
      /*$0E10*/ s_x = ram_peek(0x00f4);
                block_id = 3;
      break;
    case 3:  // $0E12
      /*$0E12*/ CYCLES(0x0e12, 14);
                tmp1_U8 = s_x;
      /*$0E15*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
      /*$0E1A*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
                block_id = 4;
      break;
    case 4:  // $0E1C
      /*$0E1C*/ CYCLES(0x0e1c, 12);
                s_y = 0x00;
      /*$0E1E*/ tmp1_U8 = peek(ram_peek16al(0x0002));
                s_a = tmp1_U8;
      /*$0E20*/ push8(tmp1_U8);
      /*$0E21*/ branchTarget = true; block_id = tmp1_U8 ? 5 : 6;
      break;
    case 5:  // $0E21
      /*$0E21*/ CYCLES_EDGE(0x0e21, 1);
                branchTarget = true; block_id = 8;
      break;
    case 6:  // $0E23
      /*$0E23*/ CYCLES(0x0e23, 5);
                tmp1_U8 = ram_peek(0x0004);
                s_a = tmp1_U8;
      /*$0E25*/ branchTarget = true; block_id = tmp1_U8 ? 7 : 8;
      break;
    case 7:  // $0E25
      /*$0E25*/ CYCLES_EDGE(0x0e25, 1);
                branchTarget = true; block_id = 12;
      break;
    case 8:  // $0E27
      /*$0E27*/ CYCLES(0x0e27, 4);
      /*$0E29*/ branchTarget = true; block_id = !(s_a != 0x03) ? 9 : 10;
      break;
    case 9:  // $0E29
      /*$0E29*/ CYCLES_EDGE(0x0e29, 1);
      /*$0E38*/ CYCLES(0x0e38, 22);
                tmp2_U8 = ram_peek(0x0004);
      /*$0E3A*/ push8(tmp2_U8);
      /*$0E3B*/ tmp3_U8 = ram_peek(0x0005);
                s_y = tmp3_U8;
      /*$0E3F*/ tmp1_U8 = peek((ram_peek16al(0x0000) + tmp3_U8));
      /*$0E41*/ poke((ram_peek16al(0x0000) + tmp3_U8), ((tmp2_U8 ^ 0x7f) & tmp1_U8));
                block_id = 11;
      break;
    case 10:  // $0E2B
      /*$0E2B*/ CYCLES(0x0e2b, 24);
                tmp1_U8 = ram_peek(0x0004);
      /*$0E2D*/ push8(tmp1_U8);
      /*$0E2E*/ tmp2_U8 = ram_peek(0x0005);
                s_y = tmp2_U8;
      /*$0E30*/ tmp3_U8 = peek((ram_peek16al(0x0000) + tmp2_U8));
      /*$0E34*/ poke((ram_peek16al(0x0000) + tmp2_U8), ((tmp1_U8 | tmp3_U8) & 0x7f));
      /*$0E36*/ CYCLES_EDGE(0x0e36, 1);
                branchTarget = true; block_id = 11;
      break;
    case 11:  // $0E43
      /*$0E43*/ CYCLES(0x0e43, 4);
                tmp3_U8 = pop8();
                s_a = tmp3_U8;
                block_id = 12;
      break;
    case 12:  // $0E44
      /*$0E44*/ CYCLES(0x0e44, 4);
                tmp1_U8 = (uint8_t)(s_a << 0x01);
                s_a = tmp1_U8;
      /*$0E45*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 13 : 14;
      break;
    case 13:  // $0E45
      /*$0E45*/ CYCLES_EDGE(0x0e45, 1);
                branchTarget = true; block_id = 17;
      break;
    case 14:  // $0E47
      /*$0E47*/ CYCLES(0x0e47, 14);
                ram_poke(0x0005, (uint8_t)(ram_peek(0x0005) + 0x01));
      /*$0E49*/ s_a = 0x01;
      /*$0E4B*/ tmp3_U8 = ram_peek(0x0005);
                s_y = tmp3_U8;
      /*$0E4F*/ branchTarget = true; block_id = !(tmp3_U8 >= 0x28) ? 15 : 16;
      break;
    case 15:  // $0E4F
      /*$0E4F*/ CYCLES_EDGE(0x0e4f, 1);
                branchTarget = true; block_id = 17;
      break;
    case 16:  // $0E51
      /*$0E51*/ CYCLES(0x0e51, 7);
                tmp3_U8 = pop8();
      /*$0E80*/ CYCLES(0x0e80, 9);
                s_y = 0x00;
      /*$0E82*/ tmp3_U8 = peek(ram_peek16al(0x00f0));
                s_a = tmp3_U8;
      /*$0E84*/ branchTarget = true; block_id = (tmp3_U8 & 0x80) ? 41 : 42;
      break;
    case 17:  // $0E55
      /*$0E55*/ CYCLES(0x0e55, 4);
                tmp3_U8 = (uint8_t)(s_a << 0x01);
                s_a = tmp3_U8;
      /*$0E56*/ branchTarget = true; block_id = !(tmp3_U8 & 0x80) ? 18 : 19;
      break;
    case 18:  // $0E56
      /*$0E56*/ CYCLES_EDGE(0x0e56, 1);
                branchTarget = true; block_id = 20;
      break;
    case 19:  // $0E58
      /*$0E58*/ CYCLES(0x0e58, 9);
      /*$0E5A*/ ram_poke(0x0005, (uint8_t)(ram_peek(0x0005) + 0x01));
      /*$0E5C*/ s_a = 0x01;
                block_id = 20;
      break;
    case 20:  // $0E5E
      /*$0E5E*/ CYCLES(0x0e5e, 10);
                ram_poke(0x0004, s_a);
      /*$0E60*/ tmp3_U8 = (uint8_t)(ram_peek(0x0002) + 0x01);
                ram_poke(0x0002, tmp3_U8);
      /*$0E62*/ branchTarget = true; block_id = tmp3_U8 ? 21 : 22;
      break;
    case 21:  // $0E62
      /*$0E62*/ CYCLES_EDGE(0x0e62, 1);
                branchTarget = true; block_id = 23;
      break;
    case 22:  // $0E64
      /*$0E64*/ CYCLES(0x0e64, 5);
                ram_poke(0x0003, (uint8_t)(ram_peek(0x0003) + 0x01));
                block_id = 23;
      break;
    case 23:  // $0E66
      /*$0E66*/ CYCLES(0x0e66, 6);
                tmp3_U8 = pop8();
                s_status_n = (tmp3_U8 & 0x80);
                s_a = tmp3_U8;
      /*$0E67*/ branchTarget = true; block_id = !tmp3_U8 ? 24 : 25;
      break;
    case 24:  // $0E67
      /*$0E67*/ CYCLES_EDGE(0x0e67, 1);
                branchTarget = true; block_id = 4;
      break;
    case 25:  // $0E69
      /*$0E69*/ CYCLES(0x0e69, 2);
                branchTarget = true; block_id = s_status_n ? 26 : 27;
      break;
    case 26:  // $0E69
      /*$0E69*/ CYCLES_EDGE(0x0e69, 1);
                branchTarget = true; block_id = 4;
      break;
    case 27:  // $0E6B
      /*$0E6B*/ CYCLES(0x0e6b, 4);
                tmp3_U8 = s_a;
                s_a = (tmp3_U8 >> 0x01);
      /*$0E6C*/ branchTarget = true; block_id = !(tmp3_U8 & 0x01) ? 28 : 29;
      break;
    case 28:  // $0E6C
      /*$0E6C*/ CYCLES_EDGE(0x0e6c, 1);
                branchTarget = true; block_id = 4;
      break;
    case 29:  // $0E6E
      /*$0E6E*/ CYCLES(0x0e6e, 4);
                tmp3_U8 = s_a;
                tmp2_U8 = tmp3_U8 & 0x01;
                s_status_c = tmp2_U8;
                s_a = (tmp3_U8 >> 0x01);
      /*$0E6F*/ branchTarget = true; block_id = !tmp2_U8 ? 30 : 31;
      break;
    case 30:  // $0E6F
      /*$0E6F*/ CYCLES_EDGE(0x0e6f, 1);
                branchTarget = true; block_id = 35;
      break;
    case 31:  // $0E71
      /*$0E71*/ CYCLES(0x0e71, 4);
                tmp2_U8 = (uint8_t)(s_x + 0x01);
                s_status_not_z = tmp2_U8;
                s_x = tmp2_U8;
      /*$0E72*/ branchTarget = true; block_id = tmp2_U8 ? 32 : 33;
      break;
    case 32:  // $0E72
      /*$0E72*/ CYCLES_EDGE(0x0e72, 1);
                branchTarget = true; block_id = 3;
      break;
    case 33:  // $0E74
      /*$0E74*/ CYCLES(0x0e74, 2);
                branchTarget = true; block_id = !s_status_not_z ? 34 : 35;
      break;
    case 34:  // $0E74
      /*$0E74*/ CYCLES_EDGE(0x0e74, 1);
                branchTarget = true; block_id = 3;
      break;
    case 35:  // $0E76
      /*$0E76*/ CYCLES(0x0e76, 4);
                tmp2_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$0E77*/ branchTarget = true; block_id = tmp2_U8 ? 36 : 37;
      break;
    case 36:  // $0E77
      /*$0E77*/ CYCLES_EDGE(0x0e77, 1);
                branchTarget = true; block_id = 3;
      break;
    case 37:  // $0E79
      /*$0E79*/ CYCLES(0x0e79, 2);
                branchTarget = true; block_id = !s_status_not_z ? 38 : 39;
      break;
    case 38:  // $0E79
      /*$0E79*/ CYCLES_EDGE(0x0e79, 1);
                branchTarget = true; block_id = 3;
      break;
    case 39:  // $0E7B
      /*$0E7B*/ CYCLES(0x0e7b, 7);
                branchTarget = true; push16(0x0e7c); block_id = 1852;
      break;
    case 40:  // $0E7D
      /*$0E7D*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x0e7b, 0x0e7d, 0x02); error_handler(0x0e7b);
      break;
    case 41:  // $0E84
      /*$0E84*/ CYCLES_EDGE(0x0e84, 1);
                branchTarget = true; block_id = 51;
      break;
    case 42:  // $0E86
      /*$0E86*/ CYCLES(0x0e86, 4);
                tmp3_U8 = s_a;
                s_a = (tmp3_U8 >> 0x01);
      /*$0E87*/ branchTarget = true; block_id = !(tmp3_U8 & 0x01) ? 43 : 44;
      break;
    case 43:  // $0E87
      /*$0E87*/ CYCLES_EDGE(0x0e87, 1);
                branchTarget = true; block_id = 51;
      break;
    case 44:  // $0E89
      /*$0E89*/ CYCLES(0x0e89, 4);
      /*$0E8A*/ branchTarget = true; block_id = !(s_a & 0x01) ? 45 : 46;
      break;
    case 45:  // $0E8A
      /*$0E8A*/ CYCLES_EDGE(0x0e8a, 1);
                branchTarget = true; block_id = 50;
      break;
    case 46:  // $0E8C
      /*$0E8C*/ CYCLES(0x0e8c, 7);
                tmp3_U8 = (uint8_t)(ram_peek(0x00f4) + 0x01);
                s_status_not_z = tmp3_U8;
                ram_poke(0x00f4, tmp3_U8);
      /*$0E8E*/ branchTarget = true; block_id = tmp3_U8 ? 47 : 48;
      break;
    case 47:  // $0E8E
      /*$0E8E*/ CYCLES_EDGE(0x0e8e, 1);
                branchTarget = true; block_id = 51;
      break;
    case 48:  // $0E90
      /*$0E90*/ CYCLES(0x0e90, 2);
                branchTarget = true; block_id = !s_status_not_z ? 49 : 50;
      break;
    case 49:  // $0E90
      /*$0E90*/ CYCLES_EDGE(0x0e90, 1);
                branchTarget = true; block_id = 51;
      break;
    case 50:  // $0E92
      /*$0E92*/ CYCLES(0x0e92, 5);
                ram_poke(0x00f4, (uint8_t)(ram_peek(0x00f4) - 0x01));
                block_id = 51;
      break;
    case 51:  // $0E94
      /*$0E94*/ CYCLES(0x0e94, 7);
                tmp3_U8 = (uint8_t)(ram_peek(0x00f0) + 0x01);
                ram_poke(0x00f0, tmp3_U8);
      /*$0E96*/ branchTarget = true; block_id = tmp3_U8 ? 52 : 53;
      break;
    case 52:  // $0E96
      /*$0E96*/ CYCLES_EDGE(0x0e96, 1);
                branchTarget = true; block_id = 54;
      break;
    case 53:  // $0E98
      /*$0E98*/ CYCLES(0x0e98, 5);
                ram_poke(0x00f1, (uint8_t)(ram_peek(0x00f1) + 0x01));
                block_id = 54;
      break;
    case 54:  // $0E9A
      /*$0E9A*/ CYCLES(0x0e9a, 8);
                tmp3_U8 = ram_peek(0x00f0);
      /*$0E9C*/ tmp2_U8 = ram_peek(0x00f2);
                s_status_c = (tmp3_U8 >= tmp2_U8);
      /*$0E9E*/ branchTarget = true; block_id = (tmp3_U8 != tmp2_U8) ? 55 : 56;
      break;
    case 55:  // $0E9E
      /*$0E9E*/ CYCLES_EDGE(0x0e9e, 1);
                branchTarget = true; block_id = 59;
      break;
    case 56:  // $0EA0
      /*$0EA0*/ CYCLES(0x0ea0, 8);
                tmp3_U8 = ram_peek(0x00f1);
      /*$0EA2*/ tmp2_U8 = ram_peek(0x00f3);
                s_status_c = (tmp3_U8 >= tmp2_U8);
      /*$0EA4*/ branchTarget = true; block_id = (tmp3_U8 != tmp2_U8) ? 57 : 58;
      break;
    case 57:  // $0EA4
      /*$0EA4*/ CYCLES_EDGE(0x0ea4, 1);
                branchTarget = true; block_id = 59;
      break;
    case 58:  // $0EA6
      /*$0EA6*/ CYCLES(0x0ea6, 3);
                branchTarget = true; block_id = 767;
      break;
    case 59:  // $0EA9
      /*$0EA9*/ CYCLES(0x0ea9, 9);
                s_y = 0x00;
      /*$0EAB*/ tmp2_U8 = peek(ram_peek16al(0x0002));
                s_status_not_z = tmp2_U8;
                tmp3_U8 = tmp2_U8 & 0x80;
                s_status_n = tmp3_U8;
                s_a = tmp2_U8;
      /*$0EAD*/ branchTarget = true; block_id = tmp3_U8 ? 60 : 61;
      break;
    case 60:  // $0EAD
      /*$0EAD*/ CYCLES_EDGE(0x0ead, 1);
      /*$0EB0*/ CYCLES(0x0eb0, 8);
                push8(s_a);
      /*$0EB1*/ tmp2_U8 = ram_peek(0x00f5);
                s_a = tmp2_U8;
      /*$0EB3*/ branchTarget = true; block_id = tmp2_U8 ? 62 : 63;
      break;
    case 61:  // $0EAF
      /*$0EAF*/ CYCLES(0x0eaf, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x0eaf, pop16() + 1);;
      break;
    case 62:  // $0EB3
      /*$0EB3*/ CYCLES_EDGE(0x0eb3, 1);
      /*$0EC5*/ CYCLES(0x0ec5, 4);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x01);
      /*$0EC7*/ branchTarget = true; block_id = (tmp2_U8 != 0x01) ? 68 : 69;
      break;
    case 63:  // $0EB5
      /*$0EB5*/ CYCLES(0x0eb5, 8);
                tmp3_U8 = pop8();
                s_a = tmp3_U8;
      /*$0EB8*/ branchTarget = true; block_id = (tmp3_U8 != 0x81) ? 64 : 65;
      break;
    case 64:  // $0EB8
      /*$0EB8*/ CYCLES_EDGE(0x0eb8, 1);
      /*$0EBD*/ CYCLES(0x0ebd, 4);
                tmp2_U8 = s_a;
                tmp3_U8 = tmp2_U8 != 0x82;
                s_status_not_z = tmp3_U8;
                s_status_c = (tmp2_U8 >= 0x82);
                s_status_n = ((uint8_t)(tmp2_U8 - 0x82) & 0x80);
      /*$0EBF*/ branchTarget = true; block_id = tmp3_U8 ? 66 : 67;
      break;
    case 65:  // $0EBA
      /*$0EBA*/ CYCLES(0x0eba, 3);
                branchTarget = true; block_id = 545;
      break;
    case 66:  // $0EBF
      /*$0EBF*/ CYCLES_EDGE(0x0ebf, 1);
      /*$0EC4*/ CYCLES(0x0ec4, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x0ec4, pop16() + 1);;
      break;
    case 67:  // $0EC1
      /*$0EC1*/ CYCLES(0x0ec1, 3);
                branchTarget = true; block_id = 565;
      break;
    case 68:  // $0EC7
      /*$0EC7*/ CYCLES_EDGE(0x0ec7, 1);
      /*$0EE0*/ CYCLES(0x0ee0, 10);
                tmp3_U8 = pop8();
                s_status_not_z = tmp3_U8;
                s_status_n = (tmp3_U8 & 0x80);
                s_a = tmp3_U8;
      /*$0EE1*/ branchTarget = true; block_id = find_block_id_func_t001(0x0ee1, pop16() + 1);;
      break;
    case 69:  // $0EC9
      /*$0EC9*/ CYCLES(0x0ec9, 8);
                tmp3_U8 = pop8();
      /*$0ECA*/ s_status_c = (tmp3_U8 >= 0x82);
      /*$0ECC*/ branchTarget = true; block_id = (tmp3_U8 != 0x82) ? 70 : 71;
      break;
    case 70:  // $0ECC
      /*$0ECC*/ CYCLES_EDGE(0x0ecc, 1);
      /*$0ED1*/ CYCLES(0x0ed1, 3);
      /*$1B00*/ CYCLES(0x1b00, 10);
      /*$1B02*/ ram_poke(0x0008, 0x00);
      /*$1B06*/ ram_poke(0x0009, 0x03);
                block_id = 786;
      break;
    case 71:  // $0ECE
      /*$0ECE*/ CYCLES(0x0ece, 3);
                branchTarget = true; block_id = 565;
      break;
    case 72:  // $0F00
      /*$0F00*/ CYCLES(0x0f00, 25);
      /*$0F02*/ ram_poke(0x0004, 0x02);
      /*$0F06*/ ram_poke(0x0002, ram_peek(0x00f6));
      /*$0F0A*/ ram_poke(0x0003, ram_peek(0x00f7));
      /*$0F0E*/ ram_poke(0x0005, 0x00);
      /*$0F10*/ s_x = ram_peek(0x00fa);
                block_id = 73;
      break;
    case 73:  // $0F12
      /*$0F12*/ CYCLES(0x0f12, 14);
                tmp3_U8 = s_x;
      /*$0F15*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp3_U8)));
      /*$0F1A*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp3_U8)));
                block_id = 74;
      break;
    case 74:  // $0F1C
      /*$0F1C*/ CYCLES(0x0f1c, 12);
                s_y = 0x00;
      /*$0F1E*/ tmp3_U8 = peek(ram_peek16al(0x0002));
                s_a = tmp3_U8;
      /*$0F20*/ push8(tmp3_U8);
      /*$0F21*/ branchTarget = true; block_id = tmp3_U8 ? 75 : 76;
      break;
    case 75:  // $0F21
      /*$0F21*/ CYCLES_EDGE(0x0f21, 1);
                branchTarget = true; block_id = 78;
      break;
    case 76:  // $0F23
      /*$0F23*/ CYCLES(0x0f23, 5);
                tmp3_U8 = ram_peek(0x0004);
                s_a = tmp3_U8;
      /*$0F25*/ branchTarget = true; block_id = tmp3_U8 ? 77 : 78;
      break;
    case 77:  // $0F25
      /*$0F25*/ CYCLES_EDGE(0x0f25, 1);
                branchTarget = true; block_id = 82;
      break;
    case 78:  // $0F27
      /*$0F27*/ CYCLES(0x0f27, 4);
      /*$0F29*/ branchTarget = true; block_id = !(s_a != 0x03) ? 79 : 80;
      break;
    case 79:  // $0F29
      /*$0F29*/ CYCLES_EDGE(0x0f29, 1);
      /*$0F38*/ CYCLES(0x0f38, 22);
                tmp2_U8 = ram_peek(0x0004);
      /*$0F3A*/ push8(tmp2_U8);
      /*$0F3B*/ tmp1_U8 = ram_peek(0x0005);
                s_y = tmp1_U8;
      /*$0F3F*/ tmp3_U8 = peek((ram_peek16al(0x0000) + tmp1_U8));
      /*$0F41*/ poke((ram_peek16al(0x0000) + tmp1_U8), ((tmp2_U8 ^ 0x7f) & tmp3_U8));
                block_id = 81;
      break;
    case 80:  // $0F2B
      /*$0F2B*/ CYCLES(0x0f2b, 24);
                tmp3_U8 = ram_peek(0x0004);
      /*$0F2D*/ push8(tmp3_U8);
      /*$0F2E*/ tmp2_U8 = ram_peek(0x0005);
                s_y = tmp2_U8;
      /*$0F30*/ tmp1_U8 = peek((ram_peek16al(0x0000) + tmp2_U8));
      /*$0F34*/ poke((ram_peek16al(0x0000) + tmp2_U8), ((tmp3_U8 | tmp1_U8) & 0x7f));
      /*$0F36*/ CYCLES_EDGE(0x0f36, 1);
                branchTarget = true; block_id = 81;
      break;
    case 81:  // $0F43
      /*$0F43*/ CYCLES(0x0f43, 4);
                tmp1_U8 = pop8();
                s_a = tmp1_U8;
                block_id = 82;
      break;
    case 82:  // $0F44
      /*$0F44*/ CYCLES(0x0f44, 4);
                tmp3_U8 = (uint8_t)(s_a << 0x01);
                s_a = tmp3_U8;
      /*$0F45*/ branchTarget = true; block_id = !(tmp3_U8 & 0x80) ? 83 : 84;
      break;
    case 83:  // $0F45
      /*$0F45*/ CYCLES_EDGE(0x0f45, 1);
                branchTarget = true; block_id = 87;
      break;
    case 84:  // $0F47
      /*$0F47*/ CYCLES(0x0f47, 14);
                ram_poke(0x0005, (uint8_t)(ram_peek(0x0005) + 0x01));
      /*$0F49*/ s_a = 0x01;
      /*$0F4B*/ tmp1_U8 = ram_peek(0x0005);
                s_y = tmp1_U8;
      /*$0F4F*/ branchTarget = true; block_id = !(tmp1_U8 >= 0x28) ? 85 : 86;
      break;
    case 85:  // $0F4F
      /*$0F4F*/ CYCLES_EDGE(0x0f4f, 1);
                branchTarget = true; block_id = 87;
      break;
    case 86:  // $0F51
      /*$0F51*/ CYCLES(0x0f51, 7);
                tmp1_U8 = pop8();
      /*$0F80*/ CYCLES(0x0f80, 9);
                s_y = 0x00;
      /*$0F82*/ tmp1_U8 = peek(ram_peek16al(0x00f6));
                s_a = tmp1_U8;
      /*$0F84*/ branchTarget = true; block_id = (tmp1_U8 & 0x80) ? 111 : 112;
      break;
    case 87:  // $0F55
      /*$0F55*/ CYCLES(0x0f55, 4);
                tmp1_U8 = (uint8_t)(s_a << 0x01);
                s_a = tmp1_U8;
      /*$0F56*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 88 : 89;
      break;
    case 88:  // $0F56
      /*$0F56*/ CYCLES_EDGE(0x0f56, 1);
                branchTarget = true; block_id = 90;
      break;
    case 89:  // $0F58
      /*$0F58*/ CYCLES(0x0f58, 9);
      /*$0F5A*/ ram_poke(0x0005, (uint8_t)(ram_peek(0x0005) + 0x01));
      /*$0F5C*/ s_a = 0x01;
                block_id = 90;
      break;
    case 90:  // $0F5E
      /*$0F5E*/ CYCLES(0x0f5e, 10);
                ram_poke(0x0004, s_a);
      /*$0F60*/ tmp1_U8 = (uint8_t)(ram_peek(0x0002) + 0x01);
                ram_poke(0x0002, tmp1_U8);
      /*$0F62*/ branchTarget = true; block_id = tmp1_U8 ? 91 : 92;
      break;
    case 91:  // $0F62
      /*$0F62*/ CYCLES_EDGE(0x0f62, 1);
                branchTarget = true; block_id = 93;
      break;
    case 92:  // $0F64
      /*$0F64*/ CYCLES(0x0f64, 5);
                ram_poke(0x0003, (uint8_t)(ram_peek(0x0003) + 0x01));
                block_id = 93;
      break;
    case 93:  // $0F66
      /*$0F66*/ CYCLES(0x0f66, 6);
                tmp1_U8 = pop8();
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$0F67*/ branchTarget = true; block_id = !tmp1_U8 ? 94 : 95;
      break;
    case 94:  // $0F67
      /*$0F67*/ CYCLES_EDGE(0x0f67, 1);
                branchTarget = true; block_id = 74;
      break;
    case 95:  // $0F69
      /*$0F69*/ CYCLES(0x0f69, 2);
                branchTarget = true; block_id = s_status_n ? 96 : 97;
      break;
    case 96:  // $0F69
      /*$0F69*/ CYCLES_EDGE(0x0f69, 1);
                branchTarget = true; block_id = 74;
      break;
    case 97:  // $0F6B
      /*$0F6B*/ CYCLES(0x0f6b, 4);
                tmp1_U8 = s_a;
                s_a = (tmp1_U8 >> 0x01);
      /*$0F6C*/ branchTarget = true; block_id = !(tmp1_U8 & 0x01) ? 98 : 99;
      break;
    case 98:  // $0F6C
      /*$0F6C*/ CYCLES_EDGE(0x0f6c, 1);
                branchTarget = true; block_id = 74;
      break;
    case 99:  // $0F6E
      /*$0F6E*/ CYCLES(0x0f6e, 4);
                tmp1_U8 = s_a;
                tmp2_U8 = tmp1_U8 & 0x01;
                s_status_c = tmp2_U8;
                s_a = (tmp1_U8 >> 0x01);
      /*$0F6F*/ branchTarget = true; block_id = tmp2_U8 ? 100 : 101;
      break;
    case 100:  // $0F6F
      /*$0F6F*/ CYCLES_EDGE(0x0f6f, 1);
                branchTarget = true; block_id = 105;
      break;
    case 101:  // $0F71
      /*$0F71*/ CYCLES(0x0f71, 4);
                tmp2_U8 = (uint8_t)(s_x + 0x01);
                s_status_not_z = tmp2_U8;
                s_x = tmp2_U8;
      /*$0F72*/ branchTarget = true; block_id = tmp2_U8 ? 102 : 103;
      break;
    case 102:  // $0F72
      /*$0F72*/ CYCLES_EDGE(0x0f72, 1);
                branchTarget = true; block_id = 73;
      break;
    case 103:  // $0F74
      /*$0F74*/ CYCLES(0x0f74, 2);
                branchTarget = true; block_id = !s_status_not_z ? 104 : 105;
      break;
    case 104:  // $0F74
      /*$0F74*/ CYCLES_EDGE(0x0f74, 1);
                branchTarget = true; block_id = 73;
      break;
    case 105:  // $0F76
      /*$0F76*/ CYCLES(0x0f76, 4);
                tmp2_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$0F77*/ branchTarget = true; block_id = tmp2_U8 ? 106 : 107;
      break;
    case 106:  // $0F77
      /*$0F77*/ CYCLES_EDGE(0x0f77, 1);
                branchTarget = true; block_id = 73;
      break;
    case 107:  // $0F79
      /*$0F79*/ CYCLES(0x0f79, 2);
                branchTarget = true; block_id = !s_status_not_z ? 108 : 109;
      break;
    case 108:  // $0F79
      /*$0F79*/ CYCLES_EDGE(0x0f79, 1);
                branchTarget = true; block_id = 73;
      break;
    case 109:  // $0F7B
      /*$0F7B*/ CYCLES(0x0f7b, 7);
                branchTarget = true; push16(0x0f7c); block_id = 1852;
      break;
    case 110:  // $0F7D
      /*$0F7D*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x0f7b, 0x0f7d, 0x02); error_handler(0x0f7b);
      break;
    case 111:  // $0F84
      /*$0F84*/ CYCLES_EDGE(0x0f84, 1);
                branchTarget = true; block_id = 121;
      break;
    case 112:  // $0F86
      /*$0F86*/ CYCLES(0x0f86, 4);
                tmp2_U8 = s_a;
                s_a = (tmp2_U8 >> 0x01);
      /*$0F87*/ branchTarget = true; block_id = !(tmp2_U8 & 0x01) ? 113 : 114;
      break;
    case 113:  // $0F87
      /*$0F87*/ CYCLES_EDGE(0x0f87, 1);
                branchTarget = true; block_id = 121;
      break;
    case 114:  // $0F89
      /*$0F89*/ CYCLES(0x0f89, 4);
      /*$0F8A*/ branchTarget = true; block_id = (s_a & 0x01) ? 115 : 116;
      break;
    case 115:  // $0F8A
      /*$0F8A*/ CYCLES_EDGE(0x0f8a, 1);
                branchTarget = true; block_id = 120;
      break;
    case 116:  // $0F8C
      /*$0F8C*/ CYCLES(0x0f8c, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x00fa) + 0x01);
                s_status_not_z = tmp2_U8;
                ram_poke(0x00fa, tmp2_U8);
      /*$0F8E*/ branchTarget = true; block_id = tmp2_U8 ? 117 : 118;
      break;
    case 117:  // $0F8E
      /*$0F8E*/ CYCLES_EDGE(0x0f8e, 1);
                branchTarget = true; block_id = 121;
      break;
    case 118:  // $0F90
      /*$0F90*/ CYCLES(0x0f90, 2);
                branchTarget = true; block_id = !s_status_not_z ? 119 : 120;
      break;
    case 119:  // $0F90
      /*$0F90*/ CYCLES_EDGE(0x0f90, 1);
                branchTarget = true; block_id = 121;
      break;
    case 120:  // $0F92
      /*$0F92*/ CYCLES(0x0f92, 5);
                ram_poke(0x00fa, (uint8_t)(ram_peek(0x00fa) - 0x01));
                block_id = 121;
      break;
    case 121:  // $0F94
      /*$0F94*/ CYCLES(0x0f94, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x00f6) + 0x01);
                ram_poke(0x00f6, tmp2_U8);
      /*$0F96*/ branchTarget = true; block_id = tmp2_U8 ? 122 : 123;
      break;
    case 122:  // $0F96
      /*$0F96*/ CYCLES_EDGE(0x0f96, 1);
                branchTarget = true; block_id = 124;
      break;
    case 123:  // $0F98
      /*$0F98*/ CYCLES(0x0f98, 5);
                ram_poke(0x00f7, (uint8_t)(ram_peek(0x00f7) + 0x01));
                block_id = 124;
      break;
    case 124:  // $0F9A
      /*$0F9A*/ CYCLES(0x0f9a, 8);
                tmp2_U8 = ram_peek(0x00f6);
      /*$0F9C*/ tmp1_U8 = ram_peek(0x00f8);
                s_status_c = (tmp2_U8 >= tmp1_U8);
      /*$0F9E*/ branchTarget = true; block_id = (tmp2_U8 != tmp1_U8) ? 125 : 126;
      break;
    case 125:  // $0F9E
      /*$0F9E*/ CYCLES_EDGE(0x0f9e, 1);
                branchTarget = true; block_id = 129;
      break;
    case 126:  // $0FA0
      /*$0FA0*/ CYCLES(0x0fa0, 8);
                tmp2_U8 = ram_peek(0x00f7);
      /*$0FA2*/ tmp1_U8 = ram_peek(0x00f9);
                s_status_c = (tmp2_U8 >= tmp1_U8);
      /*$0FA4*/ branchTarget = true; block_id = (tmp2_U8 != tmp1_U8) ? 127 : 128;
      break;
    case 127:  // $0FA4
      /*$0FA4*/ CYCLES_EDGE(0x0fa4, 1);
                branchTarget = true; block_id = 129;
      break;
    case 128:  // $0FA6
      /*$0FA6*/ CYCLES(0x0fa6, 3);
                branchTarget = true; block_id = 767;
      break;
    case 129:  // $0FA9
      /*$0FA9*/ CYCLES(0x0fa9, 9);
                s_y = 0x00;
      /*$0FAB*/ tmp1_U8 = peek(ram_peek16al(0x0002));
                s_status_not_z = tmp1_U8;
                tmp2_U8 = tmp1_U8 & 0x80;
                s_status_n = tmp2_U8;
                s_a = tmp1_U8;
      /*$0FAD*/ branchTarget = true; block_id = tmp2_U8 ? 130 : 131;
      break;
    case 130:  // $0FAD
      /*$0FAD*/ CYCLES_EDGE(0x0fad, 1);
      /*$0FB0*/ CYCLES(0x0fb0, 8);
                push8(s_a);
      /*$0FB3*/ branchTarget = true; block_id = ram_peek(0x00f5) ? 132 : 133;
      break;
    case 131:  // $0FAF
      /*$0FAF*/ CYCLES(0x0faf, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x0faf, pop16() + 1);;
      break;
    case 132:  // $0FB3
      /*$0FB3*/ CYCLES_EDGE(0x0fb3, 1);
      /*$0FC5*/ CYCLES(0x0fc5, 10);
                tmp1_U8 = pop8();
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$0FC6*/ branchTarget = true; block_id = find_block_id_func_t001(0x0fc6, pop16() + 1);;
      break;
    case 133:  // $0FB5
      /*$0FB5*/ CYCLES(0x0fb5, 8);
                tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$0FB8*/ branchTarget = true; block_id = (tmp1_U8 != 0x81) ? 134 : 135;
      break;
    case 134:  // $0FB8
      /*$0FB8*/ CYCLES_EDGE(0x0fb8, 1);
      /*$0FBD*/ CYCLES(0x0fbd, 4);
                tmp1_U8 = s_a;
                tmp2_U8 = tmp1_U8 != 0x82;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp1_U8 >= 0x82);
                s_status_n = ((uint8_t)(tmp1_U8 - 0x82) & 0x80);
      /*$0FBF*/ branchTarget = true; block_id = tmp2_U8 ? 136 : 137;
      break;
    case 135:  // $0FBA
      /*$0FBA*/ CYCLES(0x0fba, 3);
                branchTarget = true; block_id = 545;
      break;
    case 136:  // $0FBF
      /*$0FBF*/ CYCLES_EDGE(0x0fbf, 1);
      /*$0FC4*/ CYCLES(0x0fc4, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x0fc4, pop16() + 1);;
      break;
    case 137:  // $0FC1
      /*$0FC1*/ CYCLES(0x0fc1, 3);
                branchTarget = true; block_id = 565;
      break;
    case 138:  // $1000
      /*$1000*/ CYCLES(0x1000, 3);
                s_x = ram_peek(0x0004);
                block_id = 139;
      break;
    case 139:  // $1002
      /*$1002*/ CYCLES(0x1002, 17);
                tmp2_U8 = s_x;
      /*$1005*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp2_U8)));
      /*$100A*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp2_U8)));
      /*$100C*/ s_y = ram_peek(0x0006);
                block_id = 140;
      break;
    case 140:  // $100E
      /*$100E*/ CYCLES(0x100e, 6);
                tmp2_U8 = peek(ram_peek16(0x100f));
                s_a = tmp2_U8;
      /*$1011*/ branchTarget = true; block_id = !tmp2_U8 ? 141 : 142;
      break;
    case 141:  // $1011
      /*$1011*/ CYCLES_EDGE(0x1011, 1);
                branchTarget = true; block_id = 147;
      break;
    case 142:  // $1013
      /*$1013*/ CYCLES(0x1013, 14);
                tmp2_U8 = s_a;
                push8(tmp2_U8);
      /*$1016*/ tmp1_U8 = peek((ram_peek16al(0x0000) + s_y));
      /*$1018*/ tmp4_U16 = ((tmp2_U8 ^ 0xff) & tmp1_U8) << 0x01;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
      /*$1019*/ branchTarget = true; block_id = ((uint8_t)tmp4_U16) ? 143 : 144;
      break;
    case 143:  // $1019
      /*$1019*/ CYCLES_EDGE(0x1019, 1);
      /*$105C*/ CYCLES(0x105c, 15);
                tmp2_U8 = pop8();
                s_a = tmp2_U8;
      /*$105D*/ tmp2_U8 = (uint8_t)(ram_peek(0x0008) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x0008, tmp2_U8);
      /*$105F*/ branchTarget = true; block_id = find_block_id_func_t001(0x105f, pop16() + 1);;
      break;
    case 144:  // $101B
      /*$101B*/ CYCLES(0x101b, 8);
                tmp2_U8 = pop8();
                s_a = tmp2_U8;
      /*$101E*/ branchTarget = true; block_id = (s_y >= 0x28) ? 145 : 146;
      break;
    case 145:  // $101E
      /*$101E*/ CYCLES_EDGE(0x101e, 1);
                branchTarget = true; block_id = 147;
      break;
    case 146:  // $1020
      /*$1020*/ CYCLES(0x1020, 6);
                poke((ram_peek16al(0x0000) + s_y), s_a);
                block_id = 147;
      break;
    case 147:  // $1022
      /*$1022*/ CYCLES(0x1022, 13);
                ram_poke(0x100f, (uint8_t)(ram_peek(0x100f) + 0x01));
      /*$1025*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$1028*/ branchTarget = true; block_id = !(tmp2_U8 >= ram_peek(0x0007)) ? 148 : 149;
      break;
    case 148:  // $1028
      /*$1028*/ CYCLES_EDGE(0x1028, 1);
                branchTarget = true; block_id = 140;
      break;
    case 149:  // $102A
      /*$102A*/ CYCLES(0x102a, 7);
                tmp2_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp2_U8;
      /*$102B*/ tmp1_U8 = ram_peek(0x0005);
                s_status_not_z = (tmp2_U8 != tmp1_U8);
                tmp3_U8 = tmp2_U8 >= tmp1_U8;
                s_status_c = tmp3_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
      /*$102D*/ branchTarget = true; block_id = !tmp3_U8 ? 150 : 151;
      break;
    case 150:  // $102D
      /*$102D*/ CYCLES_EDGE(0x102d, 1);
                branchTarget = true; block_id = 139;
      break;
    case 151:  // $102F
      /*$102F*/ CYCLES(0x102f, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x102f, pop16() + 1);;
      break;
    case 152:  // $1030
      /*$1030*/ CYCLES(0x1030, 3);
                s_x = ram_peek(0x0004);
                block_id = 153;
      break;
    case 153:  // $1032
      /*$1032*/ CYCLES(0x1032, 17);
                tmp3_U8 = s_x;
      /*$1035*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp3_U8)));
      /*$103A*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp3_U8)));
      /*$103C*/ s_y = ram_peek(0x0006);
                block_id = 154;
      break;
    case 154:  // $103E
      /*$103E*/ CYCLES(0x103e, 6);
                tmp3_U8 = peek(ram_peek16(0x103f));
                s_a = tmp3_U8;
      /*$1041*/ branchTarget = true; block_id = !tmp3_U8 ? 155 : 156;
      break;
    case 155:  // $1041
      /*$1041*/ CYCLES_EDGE(0x1041, 1);
                branchTarget = true; block_id = 159;
      break;
    case 156:  // $1043
      /*$1043*/ CYCLES(0x1043, 6);
                s_a = (s_a ^ 0x7f);
      /*$1047*/ branchTarget = true; block_id = (s_y >= 0x28) ? 157 : 158;
      break;
    case 157:  // $1047
      /*$1047*/ CYCLES_EDGE(0x1047, 1);
                branchTarget = true; block_id = 159;
      break;
    case 158:  // $1049
      /*$1049*/ CYCLES(0x1049, 11);
                tmp3_U8 = s_y;
                tmp1_U8 = peek((ram_peek16al(0x0000) + tmp3_U8));
                tmp1_U8 = s_a & tmp1_U8;
                s_a = tmp1_U8;
      /*$104B*/ poke((ram_peek16al(0x0000) + tmp3_U8), tmp1_U8);
                block_id = 159;
      break;
    case 159:  // $104D
      /*$104D*/ CYCLES(0x104d, 13);
                ram_poke(0x103f, (uint8_t)(ram_peek(0x103f) + 0x01));
      /*$1050*/ tmp3_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp3_U8;
      /*$1053*/ branchTarget = true; block_id = !(tmp3_U8 >= ram_peek(0x0007)) ? 160 : 161;
      break;
    case 160:  // $1053
      /*$1053*/ CYCLES_EDGE(0x1053, 1);
                branchTarget = true; block_id = 154;
      break;
    case 161:  // $1055
      /*$1055*/ CYCLES(0x1055, 7);
                tmp3_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp3_U8;
      /*$1056*/ tmp1_U8 = ram_peek(0x0005);
                s_status_not_z = (tmp3_U8 != tmp1_U8);
                tmp2_U8 = tmp3_U8 >= tmp1_U8;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp3_U8 - tmp1_U8) & 0x80);
      /*$1058*/ branchTarget = true; block_id = !tmp2_U8 ? 162 : 163;
      break;
    case 162:  // $1058
      /*$1058*/ CYCLES_EDGE(0x1058, 1);
                branchTarget = true; block_id = 153;
      break;
    case 163:  // $105A
      /*$105A*/ CYCLES(0x105a, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x105a, pop16() + 1);;
      break;
    case 164:  // $1060
      /*$1060*/ CYCLES(0x1060, 6);
                tmp2_U8 = peek(ram_peek16(0x1061));
                s_status_not_z = tmp2_U8;
                tmp1_U8 = tmp2_U8 & 0x80;
                s_status_n = tmp1_U8;
                s_a = tmp2_U8;
      /*$1063*/ branchTarget = true; block_id = tmp1_U8 ? 166 : 165;
      break;
    case 165:  // $1063
      /*$1063*/ CYCLES_EDGE(0x1063, 1);
                branchTarget = true; block_id = find_block_id_func_t001(0x1063, (0x1065 + ((int8_t)ram_peek(0x1064))));
      break;
    case 166:  // $1065
      /*$1065*/ CYCLES(0x1065, 4);
                tmp1_U8 = s_a;
                tmp2_U8 = ram_peek(0x1066);
                s_status_c = (tmp1_U8 >= tmp2_U8);
      /*$1067*/ branchTarget = true; block_id = (tmp1_U8 != tmp2_U8) ? 167 : 168;
      break;
    case 167:  // $1067
      /*$1067*/ CYCLES_EDGE(0x1067, 1);
                branchTarget = true; block_id = 176;
      break;
    case 168:  // $1069
      /*$1069*/ CYCLES(0x1069, 5);
                tmp1_U8 = ram_peek(0x00e3);
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$106B*/ branchTarget = true; block_id = tmp1_U8 ? 169 : 170;
      break;
    case 169:  // $106B
      /*$106B*/ CYCLES_EDGE(0x106b, 1);
                branchTarget = true; block_id = 172;
      break;
    case 170:  // $106D
      /*$106D*/ CYCLES(0x106d, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x00e3) + 0x01);
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x00e3, tmp1_U8);
      /*$106F*/ block_id = tmp1_U8 ? 171 : 172;
      break;
    case 171:  // $106F
      /*$106F*/ CYCLES_EDGE(0x106f, 1);
                block_id = 205;
      break;
    case 172:  // $1071
      /*$1071*/ CYCLES(0x1071, 2);
                branchTarget = true; block_id = !s_status_n ? 173 : 174;
      break;
    case 173:  // $1071
      /*$1071*/ CYCLES_EDGE(0x1071, 1);
                branchTarget = true; block_id = 205;
      break;
    case 174:  // $1073
      /*$1073*/ CYCLES(0x1073, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x00e3) + 0x01);
                ram_poke(0x00e3, tmp1_U8);
      /*$1075*/ branchTarget = true; block_id = !tmp1_U8 ? 175 : 176;
      break;
    case 175:  // $1075
      /*$1075*/ CYCLES_EDGE(0x1075, 1);
                branchTarget = true; block_id = 205;
      break;
    case 176:  // $1077
      /*$1077*/ CYCLES(0x1077, 4);
                tmp2_U8 = s_a;
                tmp1_U8 = ram_peek(0x1078);
                s_status_c = (tmp2_U8 >= tmp1_U8);
      /*$1079*/ branchTarget = true; block_id = (tmp2_U8 != tmp1_U8) ? 177 : 178;
      break;
    case 177:  // $1079
      /*$1079*/ CYCLES_EDGE(0x1079, 1);
                branchTarget = true; block_id = 186;
      break;
    case 178:  // $107B
      /*$107B*/ CYCLES(0x107b, 5);
                tmp2_U8 = ram_peek(0x00e3);
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$107D*/ branchTarget = true; block_id = tmp2_U8 ? 179 : 180;
      break;
    case 179:  // $107D
      /*$107D*/ CYCLES_EDGE(0x107d, 1);
                branchTarget = true; block_id = 182;
      break;
    case 180:  // $107F
      /*$107F*/ CYCLES(0x107f, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x00e3) - 0x01);
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x00e3, tmp2_U8);
      /*$1081*/ branchTarget = true; block_id = tmp2_U8 ? 181 : 182;
      break;
    case 181:  // $1081
      /*$1081*/ CYCLES_EDGE(0x1081, 1);
      /*$10AC*/ CYCLES(0x10ac, 2);
                branchTarget = true; block_id = !s_status_n ? 206 : 207;
      break;
    case 182:  // $1083
      /*$1083*/ CYCLES(0x1083, 2);
                branchTarget = true; block_id = s_status_n ? 183 : 184;
      break;
    case 183:  // $1083
      /*$1083*/ CYCLES_EDGE(0x1083, 1);
                branchTarget = true; block_id = 205;
      break;
    case 184:  // $1085
      /*$1085*/ CYCLES(0x1085, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x00e3) - 0x01);
                ram_poke(0x00e3, tmp2_U8);
      /*$1087*/ branchTarget = true; block_id = !tmp2_U8 ? 185 : 186;
      break;
    case 185:  // $1087
      /*$1087*/ CYCLES_EDGE(0x1087, 1);
                branchTarget = true; block_id = 205;
      break;
    case 186:  // $1089
      /*$1089*/ CYCLES(0x1089, 4);
                tmp1_U8 = s_a;
                tmp2_U8 = ram_peek(0x108a);
                s_status_c = (tmp1_U8 >= tmp2_U8);
      /*$108B*/ branchTarget = true; block_id = (tmp1_U8 != tmp2_U8) ? 187 : 188;
      break;
    case 187:  // $108B
      /*$108B*/ CYCLES_EDGE(0x108b, 1);
                branchTarget = true; block_id = 196;
      break;
    case 188:  // $108D
      /*$108D*/ CYCLES(0x108d, 5);
                tmp1_U8 = ram_peek(0x00e4);
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$108F*/ branchTarget = true; block_id = tmp1_U8 ? 189 : 190;
      break;
    case 189:  // $108F
      /*$108F*/ CYCLES_EDGE(0x108f, 1);
                branchTarget = true; block_id = 192;
      break;
    case 190:  // $1091
      /*$1091*/ CYCLES(0x1091, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x00e4) + 0x01);
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x00e4, tmp1_U8);
      /*$1093*/ branchTarget = true; block_id = tmp1_U8 ? 191 : 192;
      break;
    case 191:  // $1093
      /*$1093*/ CYCLES_EDGE(0x1093, 1);
                branchTarget = true; block_id = 205;
      break;
    case 192:  // $1095
      /*$1095*/ CYCLES(0x1095, 2);
                branchTarget = true; block_id = !s_status_n ? 193 : 194;
      break;
    case 193:  // $1095
      /*$1095*/ CYCLES_EDGE(0x1095, 1);
                branchTarget = true; block_id = 205;
      break;
    case 194:  // $1097
      /*$1097*/ CYCLES(0x1097, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x00e4) + 0x01);
                ram_poke(0x00e4, tmp1_U8);
      /*$1099*/ branchTarget = true; block_id = !tmp1_U8 ? 195 : 196;
      break;
    case 195:  // $1099
      /*$1099*/ CYCLES_EDGE(0x1099, 1);
                branchTarget = true; block_id = 205;
      break;
    case 196:  // $109B
      /*$109B*/ CYCLES(0x109b, 4);
                tmp2_U8 = s_a;
                tmp1_U8 = ram_peek(0x109c);
                s_status_c = (tmp2_U8 >= tmp1_U8);
      /*$109D*/ branchTarget = true; block_id = (tmp2_U8 != tmp1_U8) ? 197 : 198;
      break;
    case 197:  // $109D
      /*$109D*/ CYCLES_EDGE(0x109d, 1);
                branchTarget = true; block_id = 207;
      break;
    case 198:  // $109F
      /*$109F*/ CYCLES(0x109f, 5);
                tmp1_U8 = ram_peek(0x00e4);
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$10A1*/ branchTarget = true; block_id = tmp1_U8 ? 199 : 200;
      break;
    case 199:  // $10A1
      /*$10A1*/ CYCLES_EDGE(0x10a1, 1);
                branchTarget = true; block_id = 202;
      break;
    case 200:  // $10A3
      /*$10A3*/ CYCLES(0x10a3, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x00e4) - 0x01);
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x00e4, tmp1_U8);
      /*$10A5*/ branchTarget = true; block_id = tmp1_U8 ? 201 : 202;
      break;
    case 201:  // $10A5
      /*$10A5*/ CYCLES_EDGE(0x10a5, 1);
                branchTarget = true; block_id = 205;
      break;
    case 202:  // $10A7
      /*$10A7*/ CYCLES(0x10a7, 2);
                branchTarget = true; block_id = s_status_n ? 203 : 204;
      break;
    case 203:  // $10A7
      /*$10A7*/ CYCLES_EDGE(0x10a7, 1);
                branchTarget = true; block_id = 205;
      break;
    case 204:  // $10A9
      /*$10A9*/ CYCLES(0x10a9, 5);
                ram_poke(0x00e4, (uint8_t)(ram_peek(0x00e4) - 0x01));
                block_id = 205;
      break;
    case 205:  // $10AB
      /*$10AB*/ CYCLES(0x10ab, 4);
                io_poke(0xc010, s_a);
                block_id = 207;
      break;
    case 206:  // $10AC
      /*$10AC*/ CYCLES_EDGE(0x10ac, 1);
      /*$106E*/ CYCLES(0x106e, 0);
                fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x106e, 0x106e, 0x03); error_handler(0x106e);
      break;
    case 207:  // $10AE
      /*$10AE*/ CYCLES(0x10ae, 3);
      /*$1180*/ CYCLES(0x1180, 5);
                tmp2_U8 = ram_peek(0x00e4);
                s_status_n = (tmp2_U8 & 0x80);
      /*$1182*/ branchTarget = true; block_id = !tmp2_U8 ? 262 : 263;
      break;
    case 208:  // $1100
      /*$1100*/ CYCLES(0x1100, 18);
                s_a = ram_peek(0x00e2);
      /*$1102*/ s_status_c = 0x00;
      /*$1103*/ block_id = s_status_d ? 210 : 209;
      break;
    case 209:  // $1103
      /*$1103*/ s_a = (uint8_t)((s_a + 0x0060) + s_status_c);
                block_id = 211;
      break;
    case 210:  // $1103
      /*$1103*/ s_a = ((uint8_t)adc_dec16(s_a, 0x60, s_status_c));
                block_id = 211;
      break;
    case 211:  // $1105
      /*$1105*/ ram_poke(0x1040, s_a);
      /*$1108*/ s_a = 0x00;
      /*$110A*/ tmp1_U8 = ram_peek(0x00e5);
                s_x = tmp1_U8;
      /*$110C*/ branchTarget = true; block_id = !tmp1_U8 ? 212 : 213;
      break;
    case 212:  // $110C
      /*$110C*/ CYCLES_EDGE(0x110c, 1);
                branchTarget = true; block_id = 221;
      break;
    case 213:  // $110E
      /*$110E*/ CYCLES(0x110e, 8);
                s_status_c = 0x00;
      /*$110F*/ block_id = s_status_d ? 215 : 214;
      break;
    case 214:  // $110F
      /*$110F*/ s_a = (uint8_t)((s_a + 0x0050) + s_status_c);
                block_id = 216;
      break;
    case 215:  // $110F
      /*$110F*/ s_a = ((uint8_t)adc_dec16(s_a, 0x50, s_status_c));
                block_id = 216;
      break;
    case 216:  // $1111
      /*$1111*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp1_U8;
      /*$1112*/ branchTarget = true; block_id = !tmp1_U8 ? 217 : 218;
      break;
    case 217:  // $1112
      /*$1112*/ CYCLES_EDGE(0x1112, 1);
                branchTarget = true; block_id = 221;
      break;
    case 218:  // $1114
      /*$1114*/ CYCLES(0x1114, 4);
                s_status_c = 0x00;
      /*$1115*/ block_id = s_status_d ? 220 : 219;
      break;
    case 219:  // $1115
      /*$1115*/ s_a = (uint8_t)((s_a + 0x0050) + s_status_c);
                block_id = 222;
      break;
    case 220:  // $1115
      /*$1115*/ s_a = ((uint8_t)adc_dec16(s_a, 0x50, s_status_c));
                block_id = 222;
      break;
    case 221:  // $1117
      /*$1117*/ CYCLES(0x1117, 36);
                ram_poke(0x103f, s_a);
      /*$111A*/ tmp1_U8 = ram_peek(0x00e0);
                s_a = tmp1_U8;
      /*$111C*/ ram_poke(0x0004, tmp1_U8);
      /*$111E*/ s_status_c = 0x00;
      /*$111F*/ block_id = s_status_d ? 224 : 223;
      break;
    case 222:  // $1117
      /*$1115*/ block_id = 221;
      break;
    case 223:  // $111F
      /*$111F*/ s_a = (uint8_t)((s_a + 0x0009) + s_status_c);
                block_id = 225;
      break;
    case 224:  // $111F
      /*$111F*/ s_a = ((uint8_t)adc_dec16(s_a, 0x09, s_status_c));
                block_id = 225;
      break;
    case 225:  // $1121
      /*$1121*/ ram_poke(0x0005, s_a);
      /*$1123*/ tmp1_U8 = ram_peek(0x00e1);
                s_a = tmp1_U8;
      /*$1125*/ ram_poke(0x0006, tmp1_U8);
      /*$1127*/ s_status_c = 0x00;
      /*$1128*/ block_id = s_status_d ? 227 : 226;
      break;
    case 226:  // $1128
      /*$1128*/ tmp4_U16 = s_a;
                tmp5_U16 = (tmp4_U16 + 0x0007) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x0007);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 228;
      break;
    case 227:  // $1128
      /*$1128*/ tmp5_U16 = adc_dec16(s_a, 0x07, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
                block_id = 228;
      break;
    case 228:  // $112A
      /*$112A*/ ram_poke(0x0007, s_a);
      /*$112C*/ func_1030(0x112e);
                branchTarget = true; block_id = 229;
      break;
    case 229:  // $112F
      /*$112F*/ CYCLES(0x112f, 6);
                branchTarget = true; push16(0x1131); block_id = 164;
      break;
    case 230:  // $1132
      /*$1132*/ CYCLES(0x1132, 18);
                s_a = ram_peek(0x00e2);
      /*$1134*/ s_status_c = 0x00;
      /*$1135*/ block_id = s_status_d ? 232 : 231;
      break;
    case 231:  // $1135
      /*$1135*/ s_a = (uint8_t)((s_a + 0x0060) + s_status_c);
                block_id = 233;
      break;
    case 232:  // $1135
      /*$1135*/ s_a = ((uint8_t)adc_dec16(s_a, 0x60, s_status_c));
                block_id = 233;
      break;
    case 233:  // $1137
      /*$1137*/ ram_poke(0x1010, s_a);
      /*$113A*/ tmp1_U8 = (uint8_t)(ram_peek(0x00e7) - 0x01);
                ram_poke(0x00e7, tmp1_U8);
      /*$113C*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 234 : 235;
      break;
    case 234:  // $113C
      /*$113C*/ CYCLES_EDGE(0x113c, 1);
                branchTarget = true; block_id = 238;
      break;
    case 235:  // $113E
      /*$113E*/ CYCLES(0x113e, 17);
      /*$1140*/ ram_poke(0x00e7, 0x08);
      /*$1142*/ ram_poke(0x00e5, (uint8_t)(ram_peek(0x00e5) + 0x01));
      /*$1148*/ branchTarget = true; block_id = !(ram_peek(0x00e5) >= 0x03) ? 236 : 237;
      break;
    case 236:  // $1148
      /*$1148*/ CYCLES_EDGE(0x1148, 1);
                branchTarget = true; block_id = 238;
      break;
    case 237:  // $114A
      /*$114A*/ CYCLES(0x114a, 5);
      /*$114C*/ ram_poke(0x00e5, 0x00);
                block_id = 238;
      break;
    case 238:  // $114E
      /*$114E*/ CYCLES(0x114e, 7);
                s_a = 0x00;
      /*$1150*/ tmp1_U8 = ram_peek(0x00e5);
                s_x = tmp1_U8;
      /*$1152*/ branchTarget = true; block_id = !tmp1_U8 ? 239 : 240;
      break;
    case 239:  // $1152
      /*$1152*/ CYCLES_EDGE(0x1152, 1);
                branchTarget = true; block_id = 248;
      break;
    case 240:  // $1154
      /*$1154*/ CYCLES(0x1154, 8);
                s_status_c = 0x00;
      /*$1155*/ block_id = s_status_d ? 242 : 241;
      break;
    case 241:  // $1155
      /*$1155*/ s_a = (uint8_t)((s_a + 0x0050) + s_status_c);
                block_id = 243;
      break;
    case 242:  // $1155
      /*$1155*/ s_a = ((uint8_t)adc_dec16(s_a, 0x50, s_status_c));
                block_id = 243;
      break;
    case 243:  // $1157
      /*$1157*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp1_U8;
      /*$1158*/ branchTarget = true; block_id = !tmp1_U8 ? 244 : 245;
      break;
    case 244:  // $1158
      /*$1158*/ CYCLES_EDGE(0x1158, 1);
                branchTarget = true; block_id = 248;
      break;
    case 245:  // $115A
      /*$115A*/ CYCLES(0x115a, 4);
                s_status_c = 0x00;
      /*$115B*/ block_id = s_status_d ? 247 : 246;
      break;
    case 246:  // $115B
      /*$115B*/ s_a = (uint8_t)((s_a + 0x0050) + s_status_c);
                block_id = 249;
      break;
    case 247:  // $115B
      /*$115B*/ s_a = ((uint8_t)adc_dec16(s_a, 0x50, s_status_c));
                block_id = 249;
      break;
    case 248:  // $115D
      /*$115D*/ CYCLES(0x115d, 41);
                ram_poke(0x100f, s_a);
      /*$1160*/ tmp1_U8 = ram_peek(0x00e0);
                s_a = tmp1_U8;
      /*$1162*/ ram_poke(0x0004, tmp1_U8);
      /*$1164*/ s_status_c = 0x00;
      /*$1165*/ block_id = s_status_d ? 251 : 250;
      break;
    case 249:  // $115D
      /*$115B*/ block_id = 248;
      break;
    case 250:  // $1165
      /*$1165*/ s_a = (uint8_t)((s_a + 0x0009) + s_status_c);
                block_id = 252;
      break;
    case 251:  // $1165
      /*$1165*/ s_a = ((uint8_t)adc_dec16(s_a, 0x09, s_status_c));
                block_id = 252;
      break;
    case 252:  // $1167
      /*$1167*/ ram_poke(0x0005, s_a);
      /*$1169*/ tmp1_U8 = ram_peek(0x00e1);
                s_a = tmp1_U8;
      /*$116B*/ ram_poke(0x0006, tmp1_U8);
      /*$116D*/ s_status_c = 0x00;
      /*$116E*/ block_id = s_status_d ? 254 : 253;
      break;
    case 253:  // $116E
      /*$116E*/ tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 + 0x0007) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x0007);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 255;
      break;
    case 254:  // $116E
      /*$116E*/ tmp4_U16 = adc_dec16(s_a, 0x07, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 255;
      break;
    case 255:  // $1170
      /*$1170*/ ram_poke(0x0007, s_a);
      /*$1174*/ ram_poke(0x0008, 0x00);
      /*$1176*/ func_1000(0x1178);
                branchTarget = true; block_id = 256;
      break;
    case 256:  // $1179
      /*$1179*/ CYCLES(0x1179, 5);
                tmp3_U8 = ram_peek(0x0008);
                s_status_not_z = tmp3_U8;
                s_status_n = (tmp3_U8 & 0x80);
                s_a = tmp3_U8;
      /*$117B*/ branchTarget = true; block_id = tmp3_U8 ? 257 : 258;
      break;
    case 257:  // $117B
      /*$117B*/ CYCLES_EDGE(0x117b, 1);
      /*$11F0*/ CYCLES(0x11f0, 5);
                branchTarget = true; block_id = find_block_id_func_t001(0x11f0, ram_peek16al(0x0200));
      break;
    case 258:  // $117D
      /*$117D*/ CYCLES(0x117d, 2);
                branchTarget = true; block_id = !s_status_not_z ? 259 : 260;
      break;
    case 259:  // $117D
      /*$117D*/ CYCLES_EDGE(0x117d, 1);
      /*$11F3*/ CYCLES(0x11f3, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x0089) - 0x01);
                ram_poke(0x0089, tmp1_U8);
      /*$11F5*/ branchTarget = true; block_id = tmp1_U8 ? 298 : 299;
      break;
    case 260:  // $117F
      /*$117F*/ CYCLES(0x117f, 7);
                branchTarget = true; push16(0x1180); block_id = 1852;
      break;
    case 261:  // $1181
      /*$1181*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x117f, 0x1181, 0x02); error_handler(0x117f);
      break;
    case 262:  // $1182
      /*$1182*/ CYCLES_EDGE(0x1182, 1);
                branchTarget = true; block_id = 288;
      break;
    case 263:  // $1184
      /*$1184*/ CYCLES(0x1184, 2);
                branchTarget = true; block_id = s_status_n ? 264 : 265;
      break;
    case 264:  // $1184
      /*$1184*/ CYCLES_EDGE(0x1184, 1);
                branchTarget = true; block_id = 276;
      break;
    case 265:  // $1186
      /*$1186*/ CYCLES(0x1186, 14);
                s_a = ram_peek(0x00e2);
      /*$1188*/ s_status_c = 0x00;
      /*$1189*/ block_id = s_status_d ? 267 : 266;
      break;
    case 266:  // $1189
      /*$1189*/ tmp4_U16 = s_a;
                tmp5_U16 = (tmp4_U16 + 0x0002) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x0002);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 268;
      break;
    case 267:  // $1189
      /*$1189*/ tmp5_U16 = adc_dec16(s_a, 0x02, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
                block_id = 268;
      break;
    case 268:  // $118B
      /*$118B*/ tmp1_U8 = s_a;
                ram_poke(0x00e2, tmp1_U8);
      /*$118D*/ tmp1_U8 = tmp1_U8 >= 0x07;
                s_status_c = tmp1_U8;
      /*$118F*/ branchTarget = true; block_id = !tmp1_U8 ? 269 : 270;
      break;
    case 269:  // $118F
      /*$118F*/ CYCLES_EDGE(0x118f, 1);
                branchTarget = true; block_id = 288;
      break;
    case 270:  // $1191
      /*$1191*/ CYCLES(0x1191, 19);
                s_status_c = 0x01;
      /*$1192*/ block_id = s_status_d ? 272 : 271;
      break;
    case 271:  // $1192
      /*$1192*/ tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 - 0x0007) - (uint8_t)(0x01 - s_status_c);
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0xfff8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 273;
      break;
    case 272:  // $1192
      /*$1192*/ tmp4_U16 = sbc_dec16(s_a, 0x07, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 273;
      break;
    case 273:  // $1194
      /*$1194*/ ram_poke(0x00e2, s_a);
      /*$1196*/ ram_poke(0x00e1, (uint8_t)(ram_peek(0x00e1) + 0x01));
      /*$119A*/ tmp1_U8 = ram_peek(0x00e1) >= 0x1c;
                s_status_c = tmp1_U8;
      /*$119C*/ branchTarget = true; block_id = !tmp1_U8 ? 274 : 275;
      break;
    case 274:  // $119C
      /*$119C*/ CYCLES_EDGE(0x119c, 1);
                branchTarget = true; block_id = 288;
      break;
    case 275:  // $119E
      /*$119E*/ CYCLES(0x119e, 12);
      /*$11A0*/ ram_poke(0x00e1, 0x1c);
      /*$11A4*/ ram_poke(0x00e2, 0x00);
      /*$11A6*/ branchTarget = true; block_id = 276;
      break;
    case 276:  // $11A8
      /*$11A8*/ CYCLES(0x11a8, 9);
                s_a = ram_peek(0x00e2);
      /*$11AA*/ s_status_c = 0x01;
      /*$11AB*/ block_id = s_status_d ? 278 : 277;
      break;
    case 277:  // $11AB
      /*$11AB*/ tmp4_U16 = s_a;
                tmp5_U16 = (tmp4_U16 - 0x0002) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0xfffd);
                tmp1_U8 = (uint8_t)tmp5_U16;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 279;
      break;
    case 278:  // $11AB
      /*$11AB*/ tmp5_U16 = sbc_dec16(s_a, 0x02, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
                block_id = 279;
      break;
    case 279:  // $11AD
      /*$11AD*/ branchTarget = true; block_id = !s_status_n ? 280 : 281;
      break;
    case 280:  // $11AD
      /*$11AD*/ CYCLES_EDGE(0x11ad, 1);
                branchTarget = true; block_id = 287;
      break;
    case 281:  // $11AF
      /*$11AF*/ CYCLES(0x11af, 19);
                s_status_c = 0x00;
      /*$11B0*/ block_id = s_status_d ? 283 : 282;
      break;
    case 282:  // $11B0
      /*$11B0*/ tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 + 0x0007) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x0007);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 284;
      break;
    case 283:  // $11B0
      /*$11B0*/ tmp4_U16 = adc_dec16(s_a, 0x07, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 284;
      break;
    case 284:  // $11B2
      /*$11B2*/ ram_poke(0x00e2, s_a);
      /*$11B4*/ ram_poke(0x00e1, (uint8_t)(ram_peek(0x00e1) - 0x01));
      /*$11B8*/ tmp1_U8 = ram_peek(0x00e1) >= 0x01;
                s_status_c = tmp1_U8;
      /*$11BA*/ branchTarget = true; block_id = tmp1_U8 ? 285 : 286;
      break;
    case 285:  // $11BA
      /*$11BA*/ CYCLES_EDGE(0x11ba, 1);
                branchTarget = true; block_id = 288;
      break;
    case 286:  // $11BC
      /*$11BC*/ CYCLES(0x11bc, 7);
      /*$11BE*/ ram_poke(0x00e1, 0x01);
      /*$11C0*/ s_a = 0x01;
                block_id = 287;
      break;
    case 287:  // $11C2
      /*$11C2*/ CYCLES(0x11c2, 5);
                ram_poke(0x00e2, s_a);
      /*$11C4*/ block_id = 288;
      break;
    case 288:  // $11C5
      /*$11C5*/ CYCLES(0x11c5, 5);
                tmp1_U8 = ram_peek(0x00e3);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$11C7*/ branchTarget = true; block_id = !tmp1_U8 ? 289 : 290;
      break;
    case 289:  // $11C7
      /*$11C7*/ CYCLES_EDGE(0x11c7, 1);
                branchTarget = true; block_id = 297;
      break;
    case 290:  // $11C9
      /*$11C9*/ CYCLES(0x11c9, 2);
                branchTarget = true; block_id = s_status_n ? 291 : 292;
      break;
    case 291:  // $11C9
      /*$11C9*/ CYCLES_EDGE(0x11c9, 1);
      /*$11D9*/ CYCLES(0x11d9, 12);
                ram_poke(0x00e0, (uint8_t)(ram_peek(0x00e0) - 0x01));
      /*$11DB*/ tmp2_U8 = ram_peek(0x00e0);
                s_a = tmp2_U8;
      /*$11DD*/ tmp1_U8 = tmp2_U8 != 0x08;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp2_U8 >= 0x08);
                s_status_n = ((uint8_t)(tmp2_U8 - 0x08) & 0x80);
      /*$11DF*/ branchTarget = true; block_id = tmp1_U8 ? 295 : 296;
      break;
    case 292:  // $11CB
      /*$11CB*/ CYCLES(0x11cb, 12);
                ram_poke(0x00e0, (uint8_t)(ram_peek(0x00e0) + 0x01));
      /*$11CD*/ tmp1_U8 = ram_peek(0x00e0);
                s_a = tmp1_U8;
      /*$11CF*/ s_status_not_z = (tmp1_U8 != 0x9d);
                tmp2_U8 = tmp1_U8 >= 0x9d;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - 0x9d) & 0x80);
      /*$11D1*/ branchTarget = true; block_id = !tmp2_U8 ? 293 : 294;
      break;
    case 293:  // $11D1
      /*$11D1*/ CYCLES_EDGE(0x11d1, 1);
                branchTarget = true; block_id = 297;
      break;
    case 294:  // $11D3
      /*$11D3*/ CYCLES(0x11d3, 7);
                s_status_not_z = 0x9d;
                s_status_n = 0x80;
                s_a = 0x9d;
      /*$11D5*/ ram_poke(0x00e0, 0x9d);
      /*$11D7*/ CYCLES_EDGE(0x11d7, 1);
                branchTarget = true; block_id = 297;
      break;
    case 295:  // $11DF
      /*$11DF*/ CYCLES_EDGE(0x11df, 1);
                branchTarget = true; block_id = 297;
      break;
    case 296:  // $11E1
      /*$11E1*/ CYCLES(0x11e1, 5);
                s_status_not_z = 0x09;
                s_status_n = 0x00;
                s_a = 0x09;
      /*$11E3*/ ram_poke(0x00e0, 0x09);
                block_id = 297;
      break;
    case 297:  // $11E5
      /*$11E5*/ CYCLES(0x11e5, 12);
      /*$11E8*/ branchTarget = true; block_id = find_block_id_func_t001(0x11e8, pop16() + 1);;
      break;
    case 298:  // $11F5
      /*$11F5*/ CYCLES_EDGE(0x11f5, 1);
      /*$11FA*/ CYCLES(0x11fa, 3);
      /*$3100*/ CYCLES(0x3100, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x0028) - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x0028, tmp1_U8);
      /*$3102*/ branchTarget = true; block_id = tmp1_U8 ? 1526 : 1527;
      break;
    case 299:  // $11F7
      /*$11F7*/ CYCLES(0x11f7, 3);
      /*$2C00*/ CYCLES(0x2c00, 2);
                s_x = 0xa8;
                block_id = 1385;
      break;
    case 300:  // $1240
      /*$1240*/ CYCLES(0x1240, 56);
      /*$1246*/ ram_poke(0x103f, ram_peek((0x1230 + (uint8_t)(ram_peek(0x00d0) - 0x01))));
      /*$1249*/ s_a = ram_peek(0x00d3);
      /*$124B*/ s_status_c = 0x00;
      /*$124C*/ block_id = s_status_d ? 302 : 301;
      break;
    case 301:  // $124C
      /*$124C*/ s_a = (uint8_t)((s_a + 0x0070) + s_status_c);
                block_id = 303;
      break;
    case 302:  // $124C
      /*$124C*/ s_a = ((uint8_t)adc_dec16(s_a, 0x70, s_status_c));
                block_id = 303;
      break;
    case 303:  // $124E
      /*$124E*/ ram_poke(0x1040, s_a);
      /*$1251*/ tmp1_U8 = ram_peek(0x00d1);
                s_a = tmp1_U8;
      /*$1253*/ ram_poke(0x0004, tmp1_U8);
      /*$1255*/ s_status_c = 0x00;
      /*$1256*/ block_id = s_status_d ? 305 : 304;
      break;
    case 304:  // $1256
      /*$1256*/ s_a = (uint8_t)((s_a + 0x0009) + s_status_c);
                block_id = 306;
      break;
    case 305:  // $1256
      /*$1256*/ s_a = ((uint8_t)adc_dec16(s_a, 0x09, s_status_c));
                block_id = 306;
      break;
    case 306:  // $1258
      /*$1258*/ ram_poke(0x0005, s_a);
      /*$125A*/ tmp1_U8 = ram_peek(0x00d2);
                s_a = tmp1_U8;
      /*$125C*/ ram_poke(0x0006, tmp1_U8);
      /*$125E*/ s_status_c = 0x00;
      /*$125F*/ block_id = s_status_d ? 308 : 307;
      break;
    case 307:  // $125F
      /*$125F*/ s_a = (uint8_t)((s_a + 0x0003) + s_status_c);
                block_id = 309;
      break;
    case 308:  // $125F
      /*$125F*/ s_a = ((uint8_t)adc_dec16(s_a, 0x03, s_status_c));
                block_id = 309;
      break;
    case 309:  // $1261
      /*$1261*/ ram_poke(0x0007, s_a);
      /*$1263*/ func_1030(0x1265);
                branchTarget = true; block_id = 310;
      break;
    case 310:  // $1266
      /*$1266*/ CYCLES(0x1266, 12);
                ram_poke(0x00d0, (uint8_t)(ram_peek(0x00d0) + 0x01));
      /*$1268*/ tmp2_U8 = ram_peek(0x00d0);
                s_a = tmp2_U8;
      /*$126C*/ branchTarget = true; block_id = !(tmp2_U8 >= 0x10) ? 311 : 312;
      break;
    case 311:  // $126C
      /*$126C*/ CYCLES_EDGE(0x126c, 1);
                branchTarget = true; block_id = 313;
      break;
    case 312:  // $126E
      /*$126E*/ CYCLES(0x126e, 5);
                s_a = 0x10;
      /*$1270*/ ram_poke(0x00d0, 0x10);
                block_id = 313;
      break;
    case 313:  // $1272
      /*$1272*/ CYCLES(0x1272, 15);
      /*$1273*/ s_x = (uint8_t)(s_a - 0x01);
      /*$1274*/ s_a = ram_peek(0x00d3);
      /*$1276*/ s_status_c = 0x01;
      /*$1277*/ block_id = s_status_d ? 315 : 314;
      break;
    case 314:  // $1277
      /*$1277*/ tmp1_U8 = (uint8_t)((s_a - ram_peek((0x1200 + s_x))) - (uint8_t)(0x01 - s_status_c));
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 316;
      break;
    case 315:  // $1277
      /*$1277*/ tmp4_U16 = sbc_dec16(s_a, ram_peek((0x1200 + s_x)), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_n = ((uint8_t)(tmp4_U16 >> 8) & 0x80);
                block_id = 316;
      break;
    case 316:  // $127A
      /*$127A*/ branchTarget = true; block_id = !s_status_n ? 317 : 318;
      break;
    case 317:  // $127A
      /*$127A*/ CYCLES_EDGE(0x127a, 1);
                branchTarget = true; block_id = 321;
      break;
    case 318:  // $127C
      /*$127C*/ CYCLES(0x127c, 13);
                ram_poke(0x00d2, (uint8_t)(ram_peek(0x00d2) - 0x01));
      /*$1280*/ s_status_c = 0x00;
      /*$1281*/ block_id = s_status_d ? 320 : 319;
      break;
    case 319:  // $1281
      /*$1281*/ s_a = (uint8_t)((s_a + 0x0007) + s_status_c);
                block_id = 322;
      break;
    case 320:  // $1281
      /*$1281*/ s_a = ((uint8_t)adc_dec16(s_a, 0x07, s_status_c));
                block_id = 322;
      break;
    case 321:  // $1283
      /*$1283*/ CYCLES(0x1283, 16);
                ram_poke(0x00d3, s_a);
      /*$1285*/ s_a = ram_peek(0x00d1);
      /*$1287*/ s_status_c = 0x00;
      /*$1288*/ block_id = s_status_d ? 324 : 323;
      break;
    case 322:  // $1283
      /*$1281*/ block_id = 321;
      break;
    case 323:  // $1288
      /*$1288*/ tmp4_U16 = s_a;
                tmp5_U16 = ram_peek((0x1210 + s_x));
                tmp6_U16 = (tmp4_U16 + tmp5_U16) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)tmp5_U16);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 325;
      break;
    case 324:  // $1288
      /*$1288*/ tmp6_U16 = adc_dec16(s_a, ram_peek((0x1210 + s_x)), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 325;
      break;
    case 325:  // $128B
      /*$128B*/ tmp1_U8 = s_a >= 0xc0;
                s_status_c = tmp1_U8;
      /*$128D*/ branchTarget = true; block_id = !tmp1_U8 ? 326 : 327;
      break;
    case 326:  // $128D
      /*$128D*/ CYCLES_EDGE(0x128d, 1);
      /*$1294*/ CYCLES(0x1294, 59);
                ram_poke(0x00d1, s_a);
      /*$1299*/ ram_poke(0x100f, ram_peek((0x1230 + s_x)));
      /*$129C*/ s_a = ram_peek(0x00d3);
      /*$129E*/ s_status_c = 0x00;
      /*$129F*/ block_id = s_status_d ? 329 : 328;
      break;
    case 327:  // $128F
      /*$128F*/ CYCLES(0x128f, 11);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$1291*/ ram_poke(0x00d0, 0x00);
      /*$1293*/ branchTarget = true; block_id = find_block_id_func_t001(0x1293, pop16() + 1);;
      break;
    case 328:  // $129F
      /*$129F*/ s_a = (uint8_t)((s_a + 0x0070) + s_status_c);
                block_id = 330;
      break;
    case 329:  // $129F
      /*$129F*/ s_a = ((uint8_t)adc_dec16(s_a, 0x70, s_status_c));
                block_id = 330;
      break;
    case 330:  // $12A1
      /*$12A1*/ ram_poke(0x1010, s_a);
      /*$12A4*/ tmp1_U8 = ram_peek(0x00d1);
                s_a = tmp1_U8;
      /*$12A6*/ ram_poke(0x0004, tmp1_U8);
      /*$12A8*/ s_status_c = 0x00;
      /*$12A9*/ block_id = s_status_d ? 332 : 331;
      break;
    case 331:  // $12A9
      /*$12A9*/ s_a = (uint8_t)((s_a + 0x0009) + s_status_c);
                block_id = 333;
      break;
    case 332:  // $12A9
      /*$12A9*/ s_a = ((uint8_t)adc_dec16(s_a, 0x09, s_status_c));
                block_id = 333;
      break;
    case 333:  // $12AB
      /*$12AB*/ ram_poke(0x0005, s_a);
      /*$12AD*/ tmp1_U8 = ram_peek(0x00d2);
                s_a = tmp1_U8;
      /*$12AF*/ ram_poke(0x0006, tmp1_U8);
      /*$12B1*/ s_status_c = 0x00;
      /*$12B2*/ block_id = s_status_d ? 335 : 334;
      break;
    case 334:  // $12B2
      /*$12B2*/ tmp6_U16 = s_a;
                tmp5_U16 = (tmp6_U16 + 0x0003) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp6_U16, (uint8_t)0x0003);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 336;
      break;
    case 335:  // $12B2
      /*$12B2*/ tmp5_U16 = adc_dec16(s_a, 0x03, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
                block_id = 336;
      break;
    case 336:  // $12B4
      /*$12B4*/ ram_poke(0x0007, s_a);
      /*$12B8*/ ram_poke(0x0008, 0x00);
      /*$12BA*/ func_1000(0x12bc);
                branchTarget = true; block_id = 337;
      break;
    case 337:  // $12BD
      /*$12BD*/ CYCLES(0x12bd, 5);
      /*$12BF*/ branchTarget = true; block_id = !ram_peek(0x0008) ? 338 : 339;
      break;
    case 338:  // $12BF
      /*$12BF*/ CYCLES_EDGE(0x12bf, 1);
      /*$1330*/ CYCLES(0x1330, 20);
      /*$1332*/ tmp1_U8 = (uint8_t)(ram_peek(0x00d0) - 0x01);
      /*$1336*/ ram_poke(0x00d4, ram_peek((0x1220 + tmp1_U8)));
      /*$1338*/ tmp1_U8 = ram_peek((0x1200 + tmp1_U8));
                s_a = tmp1_U8;
      /*$133C*/ s_x = (uint8_t)(tmp1_U8 + 0x01);
                block_id = 359;
      break;
    case 339:  // $12C1
      /*$12C1*/ CYCLES(0x12c1, 38);
      /*$12C2*/ ram_poke(0x0005, s_x);
      /*$12C4*/ ram_poke(0x0008, s_y);
      /*$12C8*/ tmp1_U8 = (uint8_t)(ram_peek(0x00d0) - 0x01);
                s_x = tmp1_U8;
      /*$12CC*/ ram_poke(0x103f, ram_peek((0x1230 + tmp1_U8)));
      /*$12CF*/ s_a = ram_peek(0x00d3);
      /*$12D1*/ s_status_c = 0x00;
      /*$12D2*/ block_id = s_status_d ? 341 : 340;
      break;
    case 340:  // $12D2
      /*$12D2*/ tmp5_U16 = s_a;
                tmp6_U16 = (tmp5_U16 + 0x0070) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp5_U16, (uint8_t)0x0070);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 342;
      break;
    case 341:  // $12D2
      /*$12D2*/ tmp6_U16 = adc_dec16(s_a, 0x70, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 342;
      break;
    case 342:  // $12D4
      /*$12D4*/ ram_poke(0x1040, s_a);
      /*$12D7*/ func_1030(0x12d9);
                branchTarget = true; block_id = 343;
      break;
    case 343:  // $12DA
      /*$12DA*/ CYCLES(0x12da, 23);
      /*$12DC*/ ram_poke(0x0004, ram_peek(0x0005));
      /*$12DE*/ ram_poke(0x0005, (uint8_t)(ram_peek(0x0005) + 0x01));
      /*$12E2*/ ram_poke(0x0007, ram_peek(0x0008));
      /*$12E4*/ func_1030(0x12e6);
                branchTarget = true; block_id = 344;
      break;
    case 344:  // $12E7
      /*$12E7*/ CYCLES(0x12e7, 13);
      /*$12E9*/ ram_poke(0x00d0, 0x00);
      /*$12EB*/ ram_poke(0x0007, (uint8_t)(ram_peek(0x0007) + 0x01));
      /*$1860*/ CYCLES(0x1860, 5);
                tmp2_U8 = ram_peek(0x00f5);
                s_a = tmp2_U8;
      /*$1862*/ branchTarget = true; block_id = !tmp2_U8 ? 637 : 638;
      break;
    case 345:  // $1300
      /*$1300*/ CYCLES(0x1300, 6);
                tmp1_U8 = peek(ram_peek16(0x1301));
                s_status_not_z = tmp1_U8;
                tmp2_U8 = tmp1_U8 & 0x80;
                s_status_n = tmp2_U8;
                s_a = tmp1_U8;
      /*$1303*/ branchTarget = true; block_id = tmp2_U8 ? 347 : 346;
      break;
    case 346:  // $1303
      /*$1303*/ CYCLES_EDGE(0x1303, 1);
                branchTarget = true; block_id = find_block_id_func_t001(0x1303, (0x1305 + ((int8_t)ram_peek(0x1304))));
      break;
    case 347:  // $1305
      /*$1305*/ CYCLES(0x1305, 4);
                tmp2_U8 = s_a;
                tmp1_U8 = ram_peek(0x1306);
                tmp3_U8 = tmp2_U8 != tmp1_U8;
                s_status_not_z = tmp3_U8;
                s_status_c = (tmp2_U8 >= tmp1_U8);
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
      /*$1307*/ branchTarget = true; block_id = tmp3_U8 ? 348 : 349;
      break;
    case 348:  // $1307
      /*$1307*/ CYCLES_EDGE(0x1307, 1);
                branchTarget = true; block_id = 358;
      break;
    case 349:  // $1309
      /*$1309*/ CYCLES(0x1309, 9);
                io_poke(0xc010, s_a);
      /*$130C*/ tmp3_U8 = ram_peek(0x0088);
                s_status_not_z = tmp3_U8;
                s_status_n = (tmp3_U8 & 0x80);
                s_a = tmp3_U8;
      /*$130E*/ branchTarget = true; block_id = !tmp3_U8 ? 350 : 351;
      break;
    case 350:  // $130E
      /*$130E*/ CYCLES_EDGE(0x130e, 1);
                branchTarget = true; block_id = 358;
      break;
    case 351:  // $1310
      /*$1310*/ CYCLES(0x1310, 39);
                ram_poke(0x00d0, (uint8_t)(ram_peek(0x00d0) + 0x01));
      /*$1312*/ s_a = ram_peek(0x00e0);
      /*$1314*/ s_status_c = 0x00;
      /*$1315*/ block_id = s_status_d ? 353 : 352;
      break;
    case 352:  // $1315
      /*$1315*/ s_a = (uint8_t)((s_a + 0x0009) + s_status_c);
                block_id = 354;
      break;
    case 353:  // $1315
      /*$1315*/ s_a = ((uint8_t)adc_dec16(s_a, 0x09, s_status_c));
                block_id = 354;
      break;
    case 354:  // $1317
      /*$1317*/ ram_poke(0x00d1, s_a);
      /*$1319*/ s_a = ram_peek(0x00e1);
      /*$131B*/ s_status_c = 0x00;
      /*$131C*/ block_id = s_status_d ? 356 : 355;
      break;
    case 355:  // $131C
      /*$131C*/ s_a = (uint8_t)((s_a + 0x0001) + s_status_c);
                block_id = 357;
      break;
    case 356:  // $131C
      /*$131C*/ s_a = ((uint8_t)adc_dec16(s_a, 0x01, s_status_c));
                block_id = 357;
      break;
    case 357:  // $131E
      /*$131E*/ ram_poke(0x00d2, s_a);
      /*$1322*/ ram_poke(0x00d3, ram_peek(0x00e2));
      /*$1326*/ ram_poke(0x00d4, 0x00);
      /*$2E20*/ CYCLES(0x2e20, 17);
      /*$2E24*/ tmp6_U16 = sbc_dec16(ram_peek(0x0088), 0x01, 0x01);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
      /*$2E26*/ ram_poke(0x0088, ((uint8_t)tmp6_U16));
      /*$2E28*/ s_status_d = 0x00;
      /*$2E29*/ branchTarget = true; block_id = 1430;
      break;
    case 358:  // $132F
      /*$132F*/ CYCLES(0x132f, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x132f, pop16() + 1);;
      break;
    case 359:  // $133D
      /*$133D*/ CYCLES(0x133d, 2);
                s_y = 0x30;
                block_id = 360;
      break;
    case 360:  // $133F
      /*$133F*/ CYCLES(0x133f, 4);
                tmp3_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp3_U8;
      /*$1340*/ branchTarget = true; block_id = tmp3_U8 ? 361 : 362;
      break;
    case 361:  // $1340
      /*$1340*/ CYCLES_EDGE(0x1340, 1);
                branchTarget = true; block_id = 360;
      break;
    case 362:  // $1342
      /*$1342*/ CYCLES(0x1342, 6);
                poke(ram_peek16(0x1343), s_a);
      /*$1345*/ s_y = 0x10;
                block_id = 363;
      break;
    case 363:  // $1347
      /*$1347*/ CYCLES(0x1347, 4);
                tmp3_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp3_U8;
      /*$1348*/ branchTarget = true; block_id = tmp3_U8 ? 364 : 365;
      break;
    case 364:  // $1348
      /*$1348*/ CYCLES_EDGE(0x1348, 1);
                branchTarget = true; block_id = 363;
      break;
    case 365:  // $134A
      /*$134A*/ CYCLES(0x134a, 8);
                poke(ram_peek16(0x134b), s_a);
      /*$134D*/ tmp3_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp3_U8;
                s_status_n = (tmp3_U8 & 0x80);
                s_x = tmp3_U8;
      /*$134E*/ branchTarget = true; block_id = tmp3_U8 ? 366 : 367;
      break;
    case 366:  // $134E
      /*$134E*/ CYCLES_EDGE(0x134e, 1);
                branchTarget = true; block_id = 359;
      break;
    case 367:  // $1350
      /*$1350*/ CYCLES(0x1350, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x1350, pop16() + 1);;
      break;
    case 368:  // $1380
      /*$1380*/ CYCLES(0x1380, 5);
      /*$1382*/ branchTarget = true; block_id = ram_peek(0x00c0) ? 369 : 370;
      break;
    case 369:  // $1382
      /*$1382*/ CYCLES_EDGE(0x1382, 1);
                branchTarget = true; block_id = 384;
      break;
    case 370:  // $1384
      /*$1384*/ CYCLES(0x1384, 5);
      /*$1386*/ branchTarget = true; block_id = ram_peek(0x00c4) ? 371 : 372;
      break;
    case 371:  // $1386
      /*$1386*/ CYCLES_EDGE(0x1386, 1);
                branchTarget = true; block_id = 384;
      break;
    case 372:  // $1388
      /*$1388*/ CYCLES(0x1388, 5);
      /*$138A*/ branchTarget = true; block_id = ram_peek(0x00c8) ? 373 : 374;
      break;
    case 373:  // $138A
      /*$138A*/ CYCLES_EDGE(0x138a, 1);
                branchTarget = true; block_id = 384;
      break;
    case 374:  // $138C
      /*$138C*/ CYCLES(0x138c, 8);
                tmp3_U8 = peek(ram_peek16(0x138d));
                s_a = tmp3_U8;
      /*$138F*/ tmp1_U8 = ram_peek(0x1390);
                tmp2_U8 = tmp3_U8 != tmp1_U8;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp3_U8 >= tmp1_U8);
                s_status_n = ((uint8_t)(tmp3_U8 - tmp1_U8) & 0x80);
      /*$1391*/ branchTarget = true; block_id = !tmp2_U8 ? 376 : 375;
      break;
    case 375:  // $1391
      /*$1391*/ CYCLES_EDGE(0x1391, 1);
                branchTarget = true; block_id = find_block_id_func_t001(0x1391, (0x1393 + ((int8_t)ram_peek(0x1392))));
      break;
    case 376:  // $1393
      /*$1393*/ CYCLES(0x1393, 55);
                io_poke(0xc010, s_a);
      /*$1396*/ ram_poke(0x00c0, (uint8_t)(ram_peek(0x00c0) + 0x01));
      /*$1398*/ s_a = ram_peek(0x00e0);
      /*$139A*/ s_status_c = 0x00;
      /*$139B*/ block_id = s_status_d ? 378 : 377;
      break;
    case 377:  // $139B
      /*$139B*/ s_a = (uint8_t)((s_a + 0x0006) + s_status_c);
                block_id = 379;
      break;
    case 378:  // $139B
      /*$139B*/ s_a = ((uint8_t)adc_dec16(s_a, 0x06, s_status_c));
                block_id = 379;
      break;
    case 379:  // $139D
      /*$139D*/ tmp2_U8 = s_a;
                s_x = tmp2_U8;
      /*$13A1*/ ram_poke(0x00c2, ram_peek((0x0c00 + tmp2_U8)));
      /*$13A6*/ ram_poke(0x00c1, ram_peek((0x0d00 + tmp2_U8)));
      /*$13A8*/ s_a = ram_peek(0x00e1);
      /*$13AA*/ s_status_c = 0x00;
      /*$13AB*/ block_id = s_status_d ? 381 : 380;
      break;
    case 380:  // $13AB
      /*$13AB*/ tmp6_U16 = s_a;
                tmp5_U16 = (tmp6_U16 + 0x0007) + s_status_c;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp6_U16, (uint8_t)0x0007);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 382;
      break;
    case 381:  // $13AB
      /*$13AB*/ tmp5_U16 = adc_dec16(s_a, 0x07, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                tmp2_U8 = (uint8_t)(tmp5_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 382;
      break;
    case 382:  // $13AD
      /*$13AD*/ ram_poke(0x00c3, s_a);
      /*$13AF*/ s_a = 0x00;
      /*$13B1*/ ram_poke(0x00cc, 0x00);
      /*$13B3*/ ram_poke(0x00bd, s_x);
      /*$13B5*/ tmp2_U8 = (uint8_t)(ram_peek(0x007d) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x007d, tmp2_U8);
                block_id = 383;
      break;
    case 383:  // $13B7
      /*$13B7*/ CYCLES(0x13b7, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x13b7, pop16() + 1);;
      break;
    case 384:  // $13B8
      /*$13B8*/ CYCLES(0x13b8, 12);
                ram_poke(0x00cc, (uint8_t)(ram_peek(0x00cc) + 0x01));
      /*$13BA*/ tmp3_U8 = ram_peek(0x00cc);
                s_a = tmp3_U8;
      /*$13BE*/ branchTarget = true; block_id = (tmp3_U8 != 0x08) ? 385 : 386;
      break;
    case 385:  // $13BE
      /*$13BE*/ CYCLES_EDGE(0x13be, 1);
                branchTarget = true; block_id = 394;
      break;
    case 386:  // $13C0
      /*$13C0*/ CYCLES(0x13c0, 43);
      /*$13C2*/ ram_poke(0x00c4, 0x01);
      /*$13C4*/ s_a = ram_peek(0x00e0);
      /*$13C6*/ s_status_c = 0x00;
      /*$13C7*/ block_id = s_status_d ? 388 : 387;
      break;
    case 387:  // $13C7
      /*$13C7*/ s_a = (uint8_t)((s_a + 0x0006) + s_status_c);
                block_id = 389;
      break;
    case 388:  // $13C7
      /*$13C7*/ s_a = ((uint8_t)adc_dec16(s_a, 0x06, s_status_c));
                block_id = 389;
      break;
    case 389:  // $13C9
      /*$13C9*/ tmp2_U8 = s_a;
                s_x = tmp2_U8;
      /*$13CD*/ ram_poke(0x00c6, ram_peek((0x0c00 + tmp2_U8)));
      /*$13D2*/ ram_poke(0x00c5, ram_peek((0x0d00 + tmp2_U8)));
      /*$13D4*/ s_a = ram_peek(0x00e1);
      /*$13D6*/ s_status_c = 0x00;
      /*$13D7*/ block_id = s_status_d ? 391 : 390;
      break;
    case 390:  // $13D7
      /*$13D7*/ tmp5_U16 = s_a;
                tmp6_U16 = (tmp5_U16 + 0x0007) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp5_U16, (uint8_t)0x0007);
                tmp2_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp2_U8;
                s_a = tmp2_U8;
                block_id = 392;
      break;
    case 391:  // $13D7
      /*$13D7*/ tmp6_U16 = adc_dec16(s_a, 0x07, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp2_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 392;
      break;
    case 392:  // $13D9
      /*$13D9*/ ram_poke(0x00c7, s_a);
      /*$13DB*/ ram_poke(0x00be, s_x);
      /*$13DD*/ branchTarget = true; block_id = s_status_not_z ? 393 : 394;
      break;
    case 393:  // $13DD
      /*$13DD*/ CYCLES_EDGE(0x13dd, 1);
                branchTarget = true; block_id = 403;
      break;
    case 394:  // $13DF
      /*$13DF*/ CYCLES(0x13df, 4);
                tmp2_U8 = s_a;
                s_status_c = (tmp2_U8 >= 0x10);
      /*$13E1*/ branchTarget = true; block_id = (tmp2_U8 != 0x10) ? 395 : 396;
      break;
    case 395:  // $13E1
      /*$13E1*/ CYCLES_EDGE(0x13e1, 1);
                branchTarget = true; block_id = 403;
      break;
    case 396:  // $13E3
      /*$13E3*/ CYCLES(0x13e3, 41);
      /*$13E5*/ ram_poke(0x00c8, 0x01);
      /*$13E7*/ s_a = ram_peek(0x00e0);
      /*$13E9*/ s_status_c = 0x00;
      /*$13EA*/ block_id = s_status_d ? 398 : 397;
      break;
    case 397:  // $13EA
      /*$13EA*/ s_a = (uint8_t)((s_a + 0x0006) + s_status_c);
                block_id = 399;
      break;
    case 398:  // $13EA
      /*$13EA*/ s_a = ((uint8_t)adc_dec16(s_a, 0x06, s_status_c));
                block_id = 399;
      break;
    case 399:  // $13EC
      /*$13EC*/ tmp2_U8 = s_a;
                s_x = tmp2_U8;
      /*$13F0*/ ram_poke(0x00c9, ram_peek((0x0d00 + tmp2_U8)));
      /*$13F5*/ ram_poke(0x00ca, ram_peek((0x0c00 + tmp2_U8)));
      /*$13F7*/ s_a = ram_peek(0x00e1);
      /*$13F9*/ s_status_c = 0x00;
      /*$13FA*/ block_id = s_status_d ? 401 : 400;
      break;
    case 400:  // $13FA
      /*$13FA*/ tmp6_U16 = s_a;
                tmp5_U16 = (tmp6_U16 + 0x0007) + s_status_c;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp6_U16, (uint8_t)0x0007);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 402;
      break;
    case 401:  // $13FA
      /*$13FA*/ tmp5_U16 = adc_dec16(s_a, 0x07, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                tmp2_U8 = (uint8_t)(tmp5_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                block_id = 402;
      break;
    case 402:  // $13FC
      /*$13FC*/ ram_poke(0x00cb, s_a);
      /*$13FE*/ ram_poke(0x00bf, s_x);
                block_id = 403;
      break;
    case 403:  // $1400
      /*$1400*/ CYCLES(0x1400, 5);
      /*$1402*/ branchTarget = true; block_id = !ram_peek(0x00c0) ? 404 : 405;
      break;
    case 404:  // $1402
      /*$1402*/ CYCLES_EDGE(0x1402, 1);
                branchTarget = true; block_id = 411;
      break;
    case 405:  // $1404
      /*$1404*/ CYCLES(0x1404, 28);
                tmp2_U8 = ram_peek(0x00c3);
      /*$140B*/ tmp1_U8 = peek((ram_peek16(0x00c1) + tmp2_U8));
      /*$140F*/ poke((ram_peek16(0x00c1) + tmp2_U8), (((ram_peek((0x1350 + tmp2_U8)) ^ 0xff) & tmp1_U8) & 0x7f));
      /*$1411*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_y = tmp2_U8;
      /*$1412*/ tmp2_U8 = tmp2_U8 >= 0x28;
                s_status_c = tmp2_U8;
      /*$1414*/ branchTarget = true; block_id = tmp2_U8 ? 406 : 407;
      break;
    case 406:  // $1414
      /*$1414*/ CYCLES_EDGE(0x1414, 1);
                branchTarget = true; block_id = 410;
      break;
    case 407:  // $1416
      /*$1416*/ CYCLES(0x1416, 9);
                tmp2_U8 = peek((ram_peek16(0x00c1) + s_y));
      /*$1418*/ tmp5_U16 = tmp2_U8 << 0x01;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
      /*$1419*/ branchTarget = true; block_id = !((uint8_t)tmp5_U16) ? 408 : 409;
      break;
    case 408:  // $1419
      /*$1419*/ CYCLES_EDGE(0x1419, 1);
      /*$1424*/ CYCLES(0x1424, 20);
                tmp2_U8 = s_y;
      /*$1427*/ tmp1_U8 = peek((ram_peek16(0x00c1) + tmp2_U8));
      /*$1429*/ poke((ram_peek16(0x00c1) + tmp2_U8), (ram_peek((0x1350 + tmp2_U8)) | tmp1_U8));
      /*$142B*/ ram_poke(0x00c3, (uint8_t)(ram_peek(0x00c3) + 0x01));
                block_id = 411;
      break;
    case 409:  // $141B
      /*$141B*/ CYCLES(0x141b, 6);
                func_1490(0x141d);
                branchTarget = true; block_id = 410;
      break;
    case 410:  // $141E
      /*$141E*/ CYCLES(0x141e, 7);
      /*$1420*/ ram_poke(0x00c0, 0x00);
      /*$1422*/ CYCLES_EDGE(0x1422, 1);
                branchTarget = true; block_id = 411;
      break;
    case 411:  // $142D
      /*$142D*/ CYCLES(0x142d, 5);
      /*$142F*/ branchTarget = true; block_id = !ram_peek(0x00c4) ? 412 : 413;
      break;
    case 412:  // $142F
      /*$142F*/ CYCLES_EDGE(0x142f, 1);
                branchTarget = true; block_id = 419;
      break;
    case 413:  // $1431
      /*$1431*/ CYCLES(0x1431, 28);
                tmp2_U8 = ram_peek(0x00c7);
      /*$1438*/ tmp1_U8 = peek((ram_peek16(0x00c5) + tmp2_U8));
      /*$143C*/ poke((ram_peek16(0x00c5) + tmp2_U8), (((ram_peek((0x1350 + tmp2_U8)) ^ 0xff) & tmp1_U8) & 0x7f));
      /*$143E*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_y = tmp2_U8;
      /*$143F*/ tmp2_U8 = tmp2_U8 >= 0x28;
                s_status_c = tmp2_U8;
      /*$1441*/ branchTarget = true; block_id = tmp2_U8 ? 414 : 415;
      break;
    case 414:  // $1441
      /*$1441*/ CYCLES_EDGE(0x1441, 1);
                branchTarget = true; block_id = 418;
      break;
    case 415:  // $1443
      /*$1443*/ CYCLES(0x1443, 9);
                tmp2_U8 = peek((ram_peek16(0x00c5) + s_y));
      /*$1445*/ tmp5_U16 = tmp2_U8 << 0x01;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
      /*$1446*/ branchTarget = true; block_id = !((uint8_t)tmp5_U16) ? 416 : 417;
      break;
    case 416:  // $1446
      /*$1446*/ CYCLES_EDGE(0x1446, 1);
      /*$1451*/ CYCLES(0x1451, 20);
                tmp2_U8 = s_y;
      /*$1454*/ tmp1_U8 = peek((ram_peek16(0x00c5) + tmp2_U8));
      /*$1456*/ poke((ram_peek16(0x00c5) + tmp2_U8), (ram_peek((0x1350 + tmp2_U8)) | tmp1_U8));
      /*$1458*/ ram_poke(0x00c7, (uint8_t)(ram_peek(0x00c7) + 0x01));
                block_id = 419;
      break;
    case 417:  // $1448
      /*$1448*/ CYCLES(0x1448, 6);
                func_1490(0x144a);
                branchTarget = true; block_id = 418;
      break;
    case 418:  // $144B
      /*$144B*/ CYCLES(0x144b, 7);
      /*$144D*/ ram_poke(0x00c4, 0x00);
      /*$144F*/ CYCLES_EDGE(0x144f, 1);
                branchTarget = true; block_id = 419;
      break;
    case 419:  // $145A
      /*$145A*/ CYCLES(0x145a, 5);
                tmp2_U8 = ram_peek(0x00c8);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$145C*/ branchTarget = true; block_id = !tmp2_U8 ? 420 : 421;
      break;
    case 420:  // $145C
      /*$145C*/ CYCLES_EDGE(0x145c, 1);
                branchTarget = true; block_id = 427;
      break;
    case 421:  // $145E
      /*$145E*/ CYCLES(0x145e, 28);
                tmp2_U8 = ram_peek(0x00cb);
      /*$1465*/ tmp1_U8 = peek((ram_peek16(0x00c9) + tmp2_U8));
      /*$1469*/ poke((ram_peek16(0x00c9) + tmp2_U8), (((ram_peek((0x1350 + tmp2_U8)) ^ 0xff) & tmp1_U8) & 0x7f));
      /*$146B*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_y = tmp2_U8;
      /*$146C*/ tmp2_U8 = tmp2_U8 >= 0x28;
                s_status_c = tmp2_U8;
      /*$146E*/ branchTarget = true; block_id = tmp2_U8 ? 422 : 423;
      break;
    case 422:  // $146E
      /*$146E*/ CYCLES_EDGE(0x146e, 1);
                branchTarget = true; block_id = 426;
      break;
    case 423:  // $1470
      /*$1470*/ CYCLES(0x1470, 9);
                tmp2_U8 = peek((ram_peek16(0x00c1) + s_y));
      /*$1472*/ tmp5_U16 = tmp2_U8 << 0x01;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
      /*$1473*/ branchTarget = true; block_id = !((uint8_t)tmp5_U16) ? 424 : 425;
      break;
    case 424:  // $1473
      /*$1473*/ CYCLES_EDGE(0x1473, 1);
      /*$147E*/ CYCLES(0x147e, 20);
                tmp2_U8 = s_y;
      /*$1481*/ tmp1_U8 = peek((ram_peek16(0x00c9) + tmp2_U8));
                tmp1_U8 = ram_peek((0x1350 + tmp2_U8)) | tmp1_U8;
                s_a = tmp1_U8;
      /*$1483*/ poke((ram_peek16(0x00c9) + tmp2_U8), tmp1_U8);
      /*$1485*/ tmp2_U8 = (uint8_t)(ram_peek(0x00cb) + 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                ram_poke(0x00cb, tmp2_U8);
                block_id = 427;
      break;
    case 425:  // $1475
      /*$1475*/ CYCLES(0x1475, 6);
                func_1490(0x1477);
                branchTarget = true; block_id = 426;
      break;
    case 426:  // $1478
      /*$1478*/ CYCLES(0x1478, 7);
      /*$147A*/ ram_poke(0x00c8, 0x00);
      /*$147C*/ CYCLES_EDGE(0x147c, 1);
      /*$14C0*/ CYCLES(0x14c0, 5);
                tmp2_U8 = ram_peek(0x007d);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$14C2*/ branchTarget = true; block_id = !tmp2_U8 ? 438 : 439;
      break;
    case 427:  // $1487
      /*$1487*/ CYCLES(0x1487, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x1487, pop16() + 1);;
      break;
    case 428:  // $1490
      /*$1490*/ CYCLES(0x1490, 10);
                tmp2_U8 = s_y;
                ram_poke(0x0007, tmp2_U8);
      /*$1492*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_y = tmp2_U8;
      /*$1495*/ branchTarget = true; block_id = (tmp2_U8 != ram_peek(0x00c3)) ? 429 : 430;
      break;
    case 429:  // $1495
      /*$1495*/ CYCLES_EDGE(0x1495, 1);
      /*$149C*/ CYCLES(0x149c, 5);
      /*$149E*/ branchTarget = true; block_id = (s_y != ram_peek(0x00c7)) ? 431 : 432;
      break;
    case 430:  // $1497
      /*$1497*/ CYCLES(0x1497, 6);
                s_x = ram_peek(0x00bd);
      /*$1499*/ branchTarget = true; block_id = 433;
      break;
    case 431:  // $149E
      /*$149E*/ CYCLES_EDGE(0x149e, 1);
      /*$14A5*/ CYCLES(0x14a5, 3);
                s_x = ram_peek(0x00bf);
                block_id = 433;
      break;
    case 432:  // $14A0
      /*$14A0*/ CYCLES(0x14a0, 6);
                s_x = ram_peek(0x00be);
      /*$14A2*/ branchTarget = true; block_id = 433;
      break;
    case 433:  // $14A7
      /*$14A7*/ CYCLES(0x14a7, 18);
                ram_poke(0x0005, s_x);
      /*$14A9*/ ram_poke(0x0005, (uint8_t)(ram_peek(0x0005) + 0x01));
      /*$14AB*/ ram_poke(0x0007, (uint8_t)(ram_peek(0x0007) + 0x01));
      /*$14AD*/ tmp2_U8 = ram_peek(0x00f5);
                s_a = tmp2_U8;
      /*$14AF*/ branchTarget = true; block_id = tmp2_U8 ? 434 : 435;
      break;
    case 434:  // $14AF
      /*$14AF*/ CYCLES_EDGE(0x14af, 1);
      /*$14B4*/ CYCLES(0x14b4, 4);
                tmp2_U8 = s_a;
                tmp1_U8 = tmp2_U8 != 0x01;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp2_U8 >= 0x01);
                s_status_n = ((uint8_t)(tmp2_U8 - 0x01) & 0x80);
      /*$14B6*/ branchTarget = true; block_id = tmp1_U8 ? 436 : 437;
      break;
    case 435:  // $14B1
      /*$14B1*/ CYCLES(0x14b1, 3);
      /*$17E0*/ CYCLES(0x17e0, 7);
      /*$17E4*/ branchTarget = true; block_id = (ram_peek(0x0070) != 0x02) ? 622 : 623;
      break;
    case 436:  // $14B6
      /*$14B6*/ CYCLES_EDGE(0x14b6, 1);
      /*$14BB*/ CYCLES(0x14bb, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x14bb, pop16() + 1);;
      break;
    case 437:  // $14B8
      /*$14B8*/ CYCLES(0x14b8, 3);
      /*$1C50*/ CYCLES(0x1c50, 10);
      /*$1C52*/ ram_poke(0x0008, 0x00);
      /*$1C56*/ ram_poke(0x0009, 0x03);
                block_id = 871;
      break;
    case 438:  // $14C2
      /*$14C2*/ CYCLES_EDGE(0x14c2, 1);
                branchTarget = true; block_id = 446;
      break;
    case 439:  // $14C4
      /*$14C4*/ CYCLES(0x14c4, 5);
                tmp1_U8 = ram_peek(0x00c0);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$14C6*/ branchTarget = true; block_id = tmp1_U8 ? 440 : 441;
      break;
    case 440:  // $14C6
      /*$14C6*/ CYCLES_EDGE(0x14c6, 1);
                branchTarget = true; block_id = 446;
      break;
    case 441:  // $14C8
      /*$14C8*/ CYCLES(0x14c8, 5);
                tmp1_U8 = ram_peek(0x00c4);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$14CA*/ branchTarget = true; block_id = tmp1_U8 ? 442 : 443;
      break;
    case 442:  // $14CA
      /*$14CA*/ CYCLES_EDGE(0x14ca, 1);
                branchTarget = true; block_id = 446;
      break;
    case 443:  // $14CC
      /*$14CC*/ CYCLES(0x14cc, 5);
                tmp1_U8 = ram_peek(0x00c8);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$14CE*/ branchTarget = true; block_id = tmp1_U8 ? 444 : 445;
      break;
    case 444:  // $14CE
      /*$14CE*/ CYCLES_EDGE(0x14ce, 1);
                branchTarget = true; block_id = 446;
      break;
    case 445:  // $14D0
      /*$14D0*/ CYCLES(0x14d0, 10);
                s_a = 0x00;
      /*$14D2*/ ram_poke(0x007d, 0x00);
      /*$14D4*/ tmp1_U8 = (uint8_t)(ram_peek(0x007e) + 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x007e, tmp1_U8);
                block_id = 446;
      break;
    case 446:  // $14D6
      /*$14D6*/ CYCLES(0x14d6, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x14d6, pop16() + 1);;
      break;
    case 447:  // $1500
      /*$1500*/ CYCLES(0x1500, 3);
                s_x = ram_peek(0x0004);
                block_id = 448;
      break;
    case 448:  // $1502
      /*$1502*/ CYCLES(0x1502, 17);
                tmp1_U8 = s_x;
      /*$1505*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
      /*$1507*/ tmp1_U8 = ram_peek((0x0c00 + tmp1_U8));
                s_a = tmp1_U8;
      /*$150A*/ ram_poke(0x0001, tmp1_U8);
      /*$150C*/ s_y = ram_peek(0x0006);
                block_id = 449;
      break;
    case 449:  // $150E
      /*$150E*/ CYCLES(0x150e, 4);
      /*$1510*/ branchTarget = true; block_id = (s_y >= 0x28) ? 450 : 451;
      break;
    case 450:  // $1510
      /*$1510*/ CYCLES_EDGE(0x1510, 1);
                branchTarget = true; block_id = 454;
      break;
    case 451:  // $1512
      /*$1512*/ CYCLES(0x1512, 6);
                tmp1_U8 = peek(ram_peek16(0x1513));
                s_a = tmp1_U8;
      /*$1515*/ branchTarget = true; block_id = !tmp1_U8 ? 452 : 453;
      break;
    case 452:  // $1515
      /*$1515*/ CYCLES_EDGE(0x1515, 1);
                branchTarget = true; block_id = 454;
      break;
    case 453:  // $1517
      /*$1517*/ CYCLES(0x1517, 6);
                poke((ram_peek16al(0x0000) + s_y), s_a);
                block_id = 454;
      break;
    case 454:  // $1519
      /*$1519*/ CYCLES(0x1519, 13);
                ram_poke(0x1513, (uint8_t)(ram_peek(0x1513) + 0x01));
      /*$151C*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$151F*/ branchTarget = true; block_id = (tmp1_U8 != ram_peek(0x0007)) ? 455 : 456;
      break;
    case 455:  // $151F
      /*$151F*/ CYCLES_EDGE(0x151f, 1);
                branchTarget = true; block_id = 449;
      break;
    case 456:  // $1521
      /*$1521*/ CYCLES(0x1521, 7);
                tmp1_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp1_U8;
      /*$1522*/ tmp2_U8 = ram_peek(0x0005);
                s_status_not_z = (tmp1_U8 != tmp2_U8);
                tmp3_U8 = tmp1_U8 >= tmp2_U8;
                s_status_c = tmp3_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
      /*$1524*/ branchTarget = true; block_id = !tmp3_U8 ? 457 : 458;
      break;
    case 457:  // $1524
      /*$1524*/ CYCLES_EDGE(0x1524, 1);
                branchTarget = true; block_id = 448;
      break;
    case 458:  // $1526
      /*$1526*/ CYCLES(0x1526, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x1526, pop16() + 1);;
      break;
    case 459:  // $1530
      /*$1530*/ CYCLES(0x1530, 3);
                s_x = ram_peek(0x0004);
                block_id = 460;
      break;
    case 460:  // $1532
      /*$1532*/ CYCLES(0x1532, 17);
                tmp3_U8 = s_x;
      /*$1535*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp3_U8)));
      /*$1537*/ tmp3_U8 = ram_peek((0x0c00 + tmp3_U8));
                s_a = tmp3_U8;
      /*$153A*/ ram_poke(0x0001, tmp3_U8);
      /*$153C*/ s_y = ram_peek(0x0006);
                block_id = 461;
      break;
    case 461:  // $153E
      /*$153E*/ CYCLES(0x153e, 4);
      /*$1540*/ branchTarget = true; block_id = (s_y >= 0x28) ? 462 : 463;
      break;
    case 462:  // $1540
      /*$1540*/ CYCLES_EDGE(0x1540, 1);
                branchTarget = true; block_id = 466;
      break;
    case 463:  // $1542
      /*$1542*/ CYCLES(0x1542, 6);
                tmp3_U8 = peek(ram_peek16(0x1543));
                s_a = tmp3_U8;
      /*$1545*/ branchTarget = true; block_id = !tmp3_U8 ? 464 : 465;
      break;
    case 464:  // $1545
      /*$1545*/ CYCLES_EDGE(0x1545, 1);
                branchTarget = true; block_id = 466;
      break;
    case 465:  // $1547
      /*$1547*/ CYCLES(0x1547, 8);
                s_a = 0x00;
      /*$1549*/ poke((ram_peek16al(0x0000) + s_y), 0x00);
                block_id = 466;
      break;
    case 466:  // $154B
      /*$154B*/ CYCLES(0x154b, 13);
                ram_poke(0x1543, (uint8_t)(ram_peek(0x1543) + 0x01));
      /*$154E*/ tmp3_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp3_U8;
      /*$1551*/ branchTarget = true; block_id = (tmp3_U8 != ram_peek(0x0007)) ? 467 : 468;
      break;
    case 467:  // $1551
      /*$1551*/ CYCLES_EDGE(0x1551, 1);
                branchTarget = true; block_id = 461;
      break;
    case 468:  // $1553
      /*$1553*/ CYCLES(0x1553, 7);
                tmp3_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp3_U8;
      /*$1554*/ tmp2_U8 = ram_peek(0x0005);
                s_status_not_z = (tmp3_U8 != tmp2_U8);
                tmp1_U8 = tmp3_U8 >= tmp2_U8;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp3_U8 - tmp2_U8) & 0x80);
      /*$1556*/ branchTarget = true; block_id = !tmp1_U8 ? 469 : 470;
      break;
    case 469:  // $1556
      /*$1556*/ CYCLES_EDGE(0x1556, 1);
                branchTarget = true; block_id = 460;
      break;
    case 470:  // $1558
      /*$1558*/ CYCLES(0x1558, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x1558, pop16() + 1);;
      break;
    case 471:  // $1560
      /*$1560*/ CYCLES(0x1560, 13);
      /*$1562*/ ram_poke(0x0008, 0x00);
      /*$1564*/ ram_poke(0x000a, 0x00);
      /*$1568*/ ram_poke(0x0009, 0x03);
                block_id = 472;
      break;
    case 472:  // $156A
      /*$156A*/ CYCLES(0x156a, 9);
                s_y = 0x00;
      /*$156C*/ tmp1_U8 = peek(ram_peek16al(0x0008));
                s_status_not_z = tmp1_U8;
                s_a = tmp1_U8;
      /*$156E*/ branchTarget = true; block_id = !tmp1_U8 ? 473 : 474;
      break;
    case 473:  // $156E
      /*$156E*/ CYCLES_EDGE(0x156e, 1);
                branchTarget = true; block_id = 512;
      break;
    case 474:  // $1570
      /*$1570*/ CYCLES(0x1570, 4);
                tmp1_U8 = s_a;
                s_a = (tmp1_U8 >> 0x01);
      /*$1571*/ branchTarget = true; block_id = (tmp1_U8 & 0x01) ? 475 : 476;
      break;
    case 475:  // $1571
      /*$1571*/ CYCLES_EDGE(0x1571, 1);
      /*$1582*/ CYCLES(0x1582, 64);
                push8(s_a);
      /*$1583*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$1584*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$1586*/ ram_poke(0x0004, tmp1_U8);
      /*$1588*/ s_status_c = 0x00;
      /*$1589*/ block_id = s_status_d ? 479 : 478;
      break;
    case 476:  // $1573
      /*$1573*/ CYCLES(0x1573, 6);
                func_1640(0x1575);
                branchTarget = true; block_id = 477;
      break;
    case 477:  // $1576
      /*$1576*/ CYCLES(0x1576, 3);
                branchTarget = true; block_id = 520;
      break;
    case 478:  // $1589
      /*$1589*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
                block_id = 480;
      break;
    case 479:  // $1589
      /*$1589*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
                block_id = 480;
      break;
    case 480:  // $158B
      /*$158B*/ ram_poke(0x0005, s_a);
      /*$158D*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$158E*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$1590*/ ram_poke(0x0006, tmp1_U8);
      /*$1592*/ s_status_c = 0x00;
      /*$1593*/ block_id = s_status_d ? 482 : 481;
      break;
    case 481:  // $1593
      /*$1593*/ s_a = (uint8_t)((s_a + 0x0002) + s_status_c);
                block_id = 483;
      break;
    case 482:  // $1593
      /*$1593*/ s_a = ((uint8_t)adc_dec16(s_a, 0x02, s_status_c));
                block_id = 483;
      break;
    case 483:  // $1595
      /*$1595*/ ram_poke(0x0007, s_a);
      /*$1598*/ tmp1_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(s_y + 0x01)));
                s_a = tmp1_U8;
      /*$159A*/ s_status_c = 0x00;
      /*$159B*/ block_id = s_status_d ? 485 : 484;
      break;
    case 484:  // $159B
      /*$159B*/ s_a = (uint8_t)((s_a + 0x0067) + s_status_c);
                block_id = 486;
      break;
    case 485:  // $159B
      /*$159B*/ s_a = ((uint8_t)adc_dec16(s_a, 0x67, s_status_c));
                block_id = 486;
      break;
    case 486:  // $159D
      /*$159D*/ ram_poke(0x1544, s_a);
      /*$15A2*/ ram_poke(0x1543, 0xc0);
      /*$15A5*/ func_1530(0x15a7);
                branchTarget = true; block_id = 487;
      break;
    case 487:  // $15A8
      /*$15A8*/ CYCLES(0x15a8, 6);
                tmp1_U8 = pop8();
      /*$15A9*/ branchTarget = true; block_id = tmp1_U8 ? 488 : 489;
      break;
    case 488:  // $15A9
      /*$15A9*/ CYCLES_EDGE(0x15a9, 1);
      /*$15AE*/ CYCLES(0x15ae, 14);
                s_a = ram_peek(0x0004);
      /*$15B0*/ s_status_c = 0x01;
      /*$15B1*/ block_id = s_status_d ? 491 : 490;
      break;
    case 489:  // $15AB
      /*$15AB*/ CYCLES(0x15ab, 3);
      /*$16C0*/ CYCLES(0x16c0, 16);
                s_a = ram_peek(0x0004);
      /*$16C2*/ s_status_c = 0x00;
      /*$16C3*/ block_id = s_status_d ? 557 : 556;
      break;
    case 490:  // $15B1
      /*$15B1*/ s_a = (uint8_t)((s_a - ram_peek(0x15b2)) - (uint8_t)(0x01 - s_status_c));
                block_id = 492;
      break;
    case 491:  // $15B1
      /*$15B1*/ s_a = ((uint8_t)sbc_dec16(s_a, ram_peek(0x15b2), s_status_c));
                block_id = 492;
      break;
    case 492:  // $15B3
      /*$15B3*/ tmp1_U8 = s_a;
                ram_poke(0x0004, tmp1_U8);
      /*$15B7*/ branchTarget = true; block_id = (tmp1_U8 >= 0xf0) ? 493 : 494;
      break;
    case 493:  // $15B7
      /*$15B7*/ CYCLES_EDGE(0x15b7, 1);
                branchTarget = true; block_id = 511;
      break;
    case 494:  // $15B9
      /*$15B9*/ CYCLES(0x15b9, 17);
      /*$15BB*/ poke((ram_peek16al(0x0008) + 0x0001), s_a);
      /*$15BE*/ s_status_c = 0x00;
      /*$15BF*/ block_id = s_status_d ? 496 : 495;
      break;
    case 495:  // $15BF
      /*$15BF*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
                block_id = 497;
      break;
    case 496:  // $15BF
      /*$15BF*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
                block_id = 497;
      break;
    case 497:  // $15C1
      /*$15C1*/ ram_poke(0x0005, s_a);
                block_id = 498;
      break;
    case 498:  // $15C3
      /*$15C3*/ CYCLES(0x15c3, 13);
                s_y = 0x03;
      /*$15C5*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0003));
                s_a = tmp1_U8;
      /*$15C7*/ s_status_c = 0x01;
      /*$15C8*/ block_id = s_status_d ? 500 : 499;
      break;
    case 499:  // $15C8
      /*$15C8*/ tmp1_U8 = (uint8_t)((s_a - 0x0004) - (uint8_t)(0x01 - s_status_c));
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 501;
      break;
    case 500:  // $15C8
      /*$15C8*/ tmp5_U16 = sbc_dec16(s_a, 0x04, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_n = ((uint8_t)(tmp5_U16 >> 8) & 0x80);
                block_id = 501;
      break;
    case 501:  // $15CA
      /*$15CA*/ branchTarget = true; block_id = !s_status_n ? 502 : 503;
      break;
    case 502:  // $15CA
      /*$15CA*/ CYCLES_EDGE(0x15ca, 1);
                branchTarget = true; block_id = 516;
      break;
    case 503:  // $15CC
      /*$15CC*/ CYCLES(0x15cc, 25);
                s_status_c = 0x00;
      /*$15CD*/ block_id = s_status_d ? 505 : 504;
      break;
    case 504:  // $15CD
      /*$15CD*/ s_a = (uint8_t)((s_a + 0x0007) + s_status_c);
                block_id = 506;
      break;
    case 505:  // $15CD
      /*$15CD*/ s_a = ((uint8_t)adc_dec16(s_a, 0x07, s_status_c));
                block_id = 506;
      break;
    case 506:  // $15CF
      /*$15CF*/ tmp1_U8 = s_y;
                poke((ram_peek16al(0x0008) + tmp1_U8), s_a);
      /*$15D1*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_y = tmp1_U8;
      /*$15D2*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$15D4*/ s_status_c = 0x01;
      /*$15D5*/ block_id = s_status_d ? 508 : 507;
      break;
    case 507:  // $15D5
      /*$15D5*/ s_a = (uint8_t)((s_a - 0x0001) - (uint8_t)(0x01 - s_status_c));
                block_id = 509;
      break;
    case 508:  // $15D5
      /*$15D5*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x01, s_status_c));
                block_id = 509;
      break;
    case 509:  // $15D7
      /*$15D9*/ branchTarget = true; block_id = !(s_a >= 0xfc) ? 510 : 511;
      break;
    case 510:  // $15D9
      /*$15D9*/ CYCLES_EDGE(0x15d9, 1);
                branchTarget = true; block_id = 515;
      break;
    case 511:  // $15DB
      /*$15DB*/ CYCLES(0x15db, 10);
                s_y = 0x00;
      /*$15DD*/ s_status_not_z = 0x00;
                s_a = 0x00;
      /*$15DF*/ poke(ram_peek16al(0x0008), 0x00);
                block_id = 512;
      break;
    case 512:  // $15E1
      /*$15E1*/ CYCLES(0x15e1, 2);
                branchTarget = true; block_id = !s_status_not_z ? 513 : 514;
      break;
    case 513:  // $15E1
      /*$15E1*/ CYCLES_EDGE(0x15e1, 1);
                branchTarget = true; block_id = 520;
      break;
    case 514:  // $15E3
      /*$15E3*/ CYCLES(0x15e3, 5);
                ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) - 0x01));
                block_id = 515;
      break;
    case 515:  // $15E5
      /*$15E5*/ CYCLES(0x15e5, 10);
                ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) - 0x01));
      /*$15E7*/ ram_poke(0x0007, (uint8_t)(ram_peek(0x0007) - 0x01));
                block_id = 516;
      break;
    case 516:  // $15E9
      /*$15E9*/ CYCLES(0x15e9, 39);
                tmp1_U8 = s_a;
                poke((ram_peek16al(0x0008) + s_y), tmp1_U8);
      /*$15EB*/ s_y = 0x03;
      /*$15ED*/ poke(ram_peek16al(0x15ee), tmp1_U8);
      /*$15F1*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0003));
                s_a = tmp1_U8;
      /*$15F3*/ s_status_c = 0x00;
      /*$15F4*/ block_id = s_status_d ? 518 : 517;
      break;
    case 517:  // $15F4
      /*$15F4*/ s_a = (uint8_t)((s_a + 0x0067) + s_status_c);
                block_id = 519;
      break;
    case 518:  // $15F4
      /*$15F4*/ s_a = ((uint8_t)adc_dec16(s_a, 0x67, s_status_c));
                block_id = 519;
      break;
    case 519:  // $15F6
      /*$15F6*/ ram_poke(0x1514, s_a);
      /*$15FB*/ ram_poke(0x1513, 0xc0);
      /*$15FE*/ func_1500(0x1600);
                branchTarget = true; block_id = 520;
      break;
    case 520:  // $1601
      /*$1601*/ CYCLES(0x1601, 14);
                s_a = ram_peek(0x0008);
      /*$1603*/ s_status_c = 0x00;
      /*$1604*/ block_id = s_status_d ? 522 : 521;
      break;
    case 521:  // $1604
      /*$1604*/ tmp5_U16 = s_a;
                tmp6_U16 = (tmp5_U16 + 0x0008) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp5_U16, (uint8_t)0x0008);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 523;
      break;
    case 522:  // $1604
      /*$1604*/ tmp6_U16 = adc_dec16(s_a, 0x08, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 523;
      break;
    case 523:  // $1606
      /*$1606*/ tmp1_U8 = s_a;
                ram_poke(0x0008, tmp1_U8);
      /*$1608*/ tmp1_U8 = tmp1_U8 >= 0x10;
                s_status_c = tmp1_U8;
      /*$160A*/ branchTarget = true; block_id = tmp1_U8 ? 524 : 525;
      break;
    case 524:  // $160A
      /*$160A*/ CYCLES_EDGE(0x160a, 1);
      /*$160F*/ CYCLES(0x160f, 7);
                tmp1_U8 = (uint8_t)(ram_peek(0x00fe) - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                ram_poke(0x00fe, tmp1_U8);
      /*$1611*/ branchTarget = true; block_id = tmp1_U8 ? 526 : 527;
      break;
    case 525:  // $160C
      /*$160C*/ CYCLES(0x160c, 3);
                branchTarget = true; block_id = 472;
      break;
    case 526:  // $1611
      /*$1611*/ CYCLES_EDGE(0x1611, 1);
                branchTarget = true; block_id = 534;
      break;
    case 527:  // $1613
      /*$1613*/ CYCLES(0x1613, 49);
      /*$1615*/ ram_poke(0x00fd, 0x00);
      /*$1619*/ s_a = (uint8_t)(ram_peek(0x00ff) << 0x01);
      /*$161A*/ s_status_c = 0x00;
      /*$161B*/ block_id = s_status_d ? 529 : 528;
      break;
    case 528:  // $161B
      /*$161B*/ tmp6_U16 = s_a;
                tmp5_U16 = ram_peek(0x00ff);
                tmp4_U16 = (tmp6_U16 + tmp5_U16) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)tmp5_U16);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 530;
      break;
    case 529:  // $161B
      /*$161B*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x00ff), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 530;
      break;
    case 530:  // $161D
      /*$161D*/ tmp1_U8 = s_a;
                ram_poke(0x00ff, tmp1_U8);
      /*$161F*/ tmp4_U16 = tmp1_U8;
      /*$1620*/ ram_poke(0x00fd, (uint8_t)((ram_peek(0x00fd) << 0x01) | (uint8_t)((tmp4_U16 << 0x01) >> 8)));
      /*$1623*/ ram_poke(0x00fd, (uint8_t)((ram_peek(0x00fd) << 0x01) | (uint8_t)(((tmp4_U16 << 0x02) & 0x01ff) >> 8)));
      /*$1628*/ tmp4_U16 = ram_peek(0x00fd) << 0x02;
      /*$1629*/ s_y = ((uint8_t)tmp4_U16);
      /*$162A*/ tmp1_U8 = ram_peek((0x0300 + (tmp4_U16 & 0x00ff)));
      /*$162D*/ s_status_c = (tmp1_U8 >= 0x01);
      /*$162F*/ branchTarget = true; block_id = (tmp1_U8 != 0x01) ? 531 : 532;
      break;
    case 531:  // $162F
      /*$162F*/ CYCLES_EDGE(0x162f, 1);
                branchTarget = true; block_id = 533;
      break;
    case 532:  // $1631
      /*$1631*/ CYCLES(0x1631, 7);
      /*$1633*/ ram_poke((0x0300 + s_y), 0x03);
                block_id = 533;
      break;
    case 533:  // $1636
      /*$1636*/ CYCLES(0x1636, 5);
                s_status_not_z = 0x0a;
                s_status_n = 0x00;
                s_a = 0x0a;
      /*$1638*/ ram_poke(0x00fe, 0x0a);
                block_id = 534;
      break;
    case 534:  // $163A
      /*$163A*/ CYCLES(0x163a, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x163a, pop16() + 1);;
      break;
    case 535:  // $1640
      /*$1640*/ CYCLES(0x1640, 48);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$1641*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$1643*/ s_status_c = 0x01;
      /*$1644*/ block_id = s_status_d ? 537 : 536;
      break;
    case 536:  // $1644
      /*$1644*/ s_a = (uint8_t)((s_a - 0x0004) - (uint8_t)(0x01 - s_status_c));
                block_id = 538;
      break;
    case 537:  // $1644
      /*$1644*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x04, s_status_c));
                block_id = 538;
      break;
    case 538:  // $1646
      /*$1646*/ ram_poke(0x000b, s_a);
      /*$1648*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$1649*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$164B*/ s_status_c = 0x01;
      /*$164C*/ block_id = s_status_d ? 540 : 539;
      break;
    case 539:  // $164C
      /*$164C*/ tmp4_U16 = s_a;
                tmp5_U16 = (tmp4_U16 - 0x0002) - (uint8_t)(0x01 - s_status_c);
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0xfffd);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 541;
      break;
    case 540:  // $164C
      /*$164C*/ tmp5_U16 = sbc_dec16(s_a, 0x02, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
                block_id = 541;
      break;
    case 541:  // $164E
      /*$1650*/ ram_poke(0x000c, s_a);
      /*$1653*/ tmp1_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(s_y + 0x01)));
                s_a = tmp1_U8;
      /*$1655*/ ram_poke(0x000a, tmp1_U8);
      /*$1657*/ func_2b60(0x1659);
                branchTarget = true; block_id = 542;
      break;
    case 542:  // $165A
      /*$165A*/ CYCLES(0x165a, 12);
                ram_poke(0x000a, (uint8_t)(ram_peek(0x000a) + 0x01));
      /*$165C*/ tmp1_U8 = ram_peek(0x000a);
                s_a = tmp1_U8;
      /*$165E*/ tmp1_U8 = tmp1_U8 >= 0x04;
                s_status_c = tmp1_U8;
      /*$1660*/ branchTarget = true; block_id = tmp1_U8 ? 543 : 544;
      break;
    case 543:  // $1660
      /*$1660*/ CYCLES_EDGE(0x1660, 1);
      /*$1667*/ CYCLES(0x1667, 16);
                s_y = 0x00;
      /*$1669*/ s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$166B*/ poke(ram_peek16al(0x0008), 0x00);
      /*$166D*/ branchTarget = true; block_id = find_block_id_func_t001(0x166d, pop16() + 1);;
      break;
    case 544:  // $1662
      /*$1662*/ CYCLES(0x1662, 14);
                s_status_not_z = 0x03;
                s_status_n = 0x00;
                s_y = 0x03;
      /*$1664*/ poke((ram_peek16al(0x0008) + 0x0003), s_a);
      /*$1666*/ branchTarget = true; block_id = find_block_id_func_t001(0x1666, pop16() + 1);;
      break;
    case 545:  // $1680
      /*$1680*/ CYCLES(0x1680, 9);
                s_y = 0x00;
      /*$1682*/ tmp3_U8 = peek(ram_peek16al(0x00ce));
      /*$1684*/ branchTarget = true; block_id = tmp3_U8 ? 546 : 547;
      break;
    case 546:  // $1684
      /*$1684*/ CYCLES_EDGE(0x1684, 1);
      /*$16A3*/ CYCLES(0x16a3, 14);
                s_a = ram_peek(0x00ce);
      /*$16A5*/ s_status_c = 0x00;
      /*$16A6*/ block_id = s_status_d ? 552 : 551;
      break;
    case 547:  // $1686
      /*$1686*/ CYCLES(0x1686, 62);
      /*$1688*/ tmp1_U8 = s_y;
                poke((ram_peek16al(0x00ce) + tmp1_U8), 0x01);
      /*$168F*/ poke((ram_peek16al(0x00ce) + (uint8_t)(tmp1_U8 + 0x01)), 0x10);
      /*$1694*/ poke((ram_peek16al(0x00ce) + (uint8_t)(tmp1_U8 + 0x02)), 0x28);
      /*$1699*/ poke((ram_peek16al(0x00ce) + (uint8_t)(tmp1_U8 + 0x03)), 0x00);
      /*$169B*/ s_y = (uint8_t)(tmp1_U8 + 0x04);
      /*$169C*/ s_a = s_x;
      /*$169D*/ s_status_c = 0x01;
      /*$169E*/ block_id = s_status_d ? 549 : 548;
      break;
    case 548:  // $169E
      /*$169E*/ tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 - 0x000c) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0xfff3);
                tmp1_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 550;
      break;
    case 549:  // $169E
      /*$169E*/ tmp4_U16 = sbc_dec16(s_a, 0x0c, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
                block_id = 550;
      break;
    case 550:  // $16A0
      /*$16A0*/ poke((ram_peek16al(0x00ce) + s_y), s_a);
      /*$16A2*/ branchTarget = true; block_id = find_block_id_func_t001(0x16a2, pop16() + 1);;
      break;
    case 551:  // $16A6
      /*$16A6*/ tmp4_U16 = s_a;
                tmp5_U16 = (tmp4_U16 + 0x0008) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x0008);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 553;
      break;
    case 552:  // $16A6
      /*$16A6*/ tmp5_U16 = adc_dec16(s_a, 0x08, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
                block_id = 553;
      break;
    case 553:  // $16A8
      /*$16A8*/ tmp1_U8 = s_a;
                ram_poke(0x00ce, tmp1_U8);
      /*$16AA*/ tmp1_U8 = tmp1_U8 >= 0x10;
                s_status_c = tmp1_U8;
      /*$16AC*/ branchTarget = true; block_id = !tmp1_U8 ? 554 : 555;
      break;
    case 554:  // $16AC
      /*$16AC*/ CYCLES_EDGE(0x16ac, 1);
                branchTarget = true; block_id = 545;
      break;
    case 555:  // $16AE
      /*$16AE*/ CYCLES(0x16ae, 11);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$16B0*/ ram_poke(0x00ce, 0x00);
      /*$16B2*/ branchTarget = true; block_id = find_block_id_func_t001(0x16b2, pop16() + 1);;
      break;
    case 556:  // $16C3
      /*$16C3*/ s_a = (uint8_t)((s_a + ram_peek(0x16c4)) + s_status_c);
                block_id = 558;
      break;
    case 557:  // $16C3
      /*$16C3*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x16c4), s_status_c));
                block_id = 558;
      break;
    case 558:  // $16C5
      /*$16C5*/ s_y = 0x04;
      /*$16C7*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0004));
      /*$16C9*/ branchTarget = true; block_id = !(s_a >= tmp1_U8) ? 559 : 560;
      break;
    case 559:  // $16C9
      /*$16C9*/ CYCLES_EDGE(0x16c9, 1);
                branchTarget = true; block_id = 561;
      break;
    case 560:  // $16CB
      /*$16CB*/ CYCLES(0x16cb, 5);
                tmp1_U8 = peek((ram_peek16al(0x0008) + s_y));
                s_a = tmp1_U8;
                block_id = 561;
      break;
    case 561:  // $16CD
      /*$16CD*/ CYCLES(0x16cd, 21);
                tmp1_U8 = s_a;
                ram_poke(0x0004, tmp1_U8);
      /*$16D1*/ poke((ram_peek16al(0x0008) + 0x0001), tmp1_U8);
      /*$16D3*/ s_status_c = 0x00;
      /*$16D4*/ block_id = s_status_d ? 563 : 562;
      break;
    case 562:  // $16D4
      /*$16D4*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
                block_id = 564;
      break;
    case 563:  // $16D4
      /*$16D4*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
                block_id = 564;
      break;
    case 564:  // $16D6
      /*$16D6*/ ram_poke(0x0005, s_a);
      /*$16D8*/ branchTarget = true; block_id = 498;
      break;
    case 565:  // $1700
      /*$1700*/ CYCLES(0x1700, 6);
                tmp3_U8 = ram_peek(0x0320);
                s_status_not_z = tmp3_U8;
                s_status_n = (tmp3_U8 & 0x80);
                s_a = tmp3_U8;
      /*$1703*/ branchTarget = true; block_id = tmp3_U8 ? 566 : 567;
      break;
    case 566:  // $1703
      /*$1703*/ CYCLES_EDGE(0x1703, 1);
      /*$171F*/ CYCLES(0x171f, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x171f, pop16() + 1);;
      break;
    case 567:  // $1705
      /*$1705*/ CYCLES(0x1705, 34);
      /*$1707*/ ram_poke(0x0320, 0x01);
      /*$170A*/ s_a = s_x;
      /*$170B*/ s_status_c = 0x01;
      /*$170C*/ block_id = s_status_d ? 569 : 568;
      break;
    case 568:  // $170C
      /*$170C*/ tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 - 0x000f) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0xfff0);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 570;
      break;
    case 569:  // $170C
      /*$170C*/ tmp4_U16 = sbc_dec16(s_a, 0x0f, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp1_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 570;
      break;
    case 570:  // $170E
      /*$170E*/ ram_poke(0x0321, s_a);
      /*$1713*/ ram_poke(0x0322, 0x28);
      /*$1716*/ s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$1718*/ ram_poke(0x0323, 0x00);
      /*$171B*/ branchTarget = true; block_id = find_block_id_func_t001(0x171b, pop16() + 1);;
      break;
    case 571:  // $1723
      /*$1723*/ CYCLES(0x1723, 6);
                poke(ram_peek16al(0x1724), s_a);
      /*$1726*/ s_y = 0x18;
                block_id = 572;
      break;
    case 572:  // $1728
      /*$1728*/ CYCLES(0x1728, 4);
                tmp1_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp1_U8;
      /*$1729*/ branchTarget = true; block_id = tmp1_U8 ? 573 : 574;
      break;
    case 573:  // $1729
      /*$1729*/ CYCLES_EDGE(0x1729, 1);
                branchTarget = true; block_id = 572;
      break;
    case 574:  // $172B
      /*$172B*/ CYCLES(0x172b, 4);
                tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp1_U8;
                tmp2_U8 = tmp1_U8 & 0x80;
                s_status_n = tmp2_U8;
                s_x = tmp1_U8;
      /*$172C*/ branchTarget = true; block_id = !tmp2_U8 ? 575 : 576;
      break;
    case 575:  // $172C
      /*$172C*/ CYCLES_EDGE(0x172c, 1);
                branchTarget = true; block_id = 571;
      break;
    case 576:  // $172E
      /*$172E*/ CYCLES(0x172e, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x172e, pop16() + 1);;
      break;
    case 577:  // $1738
      /*$1738*/ CYCLES(0x1738, 6);
                tmp2_U8 = ram_peek(0x0320);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$173B*/ branchTarget = true; block_id = !tmp2_U8 ? 578 : 579;
      break;
    case 578:  // $173B
      /*$173B*/ CYCLES_EDGE(0x173b, 1);
      /*$17AE*/ CYCLES(0x17ae, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x17ae, pop16() + 1);;
      break;
    case 579:  // $173D
      /*$173D*/ CYCLES(0x173d, 4);
                tmp2_U8 = s_a & 0x01;
                s_status_c = tmp2_U8;
      /*$173E*/ branchTarget = true; block_id = tmp2_U8 ? 580 : 581;
      break;
    case 580:  // $173E
      /*$173E*/ CYCLES_EDGE(0x173e, 1);
      /*$1745*/ CYCLES(0x1745, 52);
                tmp2_U8 = ram_peek(0x0321);
                s_a = tmp2_U8;
      /*$1748*/ ram_poke(0x0004, tmp2_U8);
      /*$174A*/ s_status_c = 0x00;
      /*$174B*/ block_id = s_status_d ? 585 : 584;
      break;
    case 581:  // $1740
      /*$1740*/ CYCLES(0x1740, 2);
                branchTarget = true; block_id = !s_status_c ? 582 : 583;
      break;
    case 582:  // $1740
      /*$1740*/ CYCLES_EDGE(0x1740, 1);
      /*$17B0*/ CYCLES(0x17b0, 35);
                s_a = ram_peek(0x0321);
      /*$17B3*/ s_status_c = 0x01;
      /*$17B4*/ block_id = s_status_d ? 615 : 614;
      break;
    case 583:  // $1742
      /*$1742*/ CYCLES(0x1742, 0);
                fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x1742, 0x1742, 0x03); error_handler(0x1742);
      break;
    case 584:  // $174B
      /*$174B*/ s_a = (uint8_t)((s_a + 0x000f) + s_status_c);
                block_id = 586;
      break;
    case 585:  // $174B
      /*$174B*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0f, s_status_c));
                block_id = 586;
      break;
    case 586:  // $174D
      /*$174D*/ ram_poke(0x0005, s_a);
      /*$174F*/ tmp2_U8 = ram_peek(0x0322);
                s_a = tmp2_U8;
      /*$1752*/ ram_poke(0x0006, tmp2_U8);
      /*$1754*/ s_status_c = 0x00;
      /*$1755*/ block_id = s_status_d ? 588 : 587;
      break;
    case 587:  // $1755
      /*$1755*/ s_a = (uint8_t)((s_a + 0x0004) + s_status_c);
                block_id = 589;
      break;
    case 588:  // $1755
      /*$1755*/ s_a = ((uint8_t)adc_dec16(s_a, 0x04, s_status_c));
                block_id = 589;
      break;
    case 589:  // $1757
      /*$1757*/ ram_poke(0x0007, s_a);
      /*$1759*/ s_a = ram_peek(0x0323);
      /*$175C*/ s_status_c = 0x00;
      /*$175D*/ block_id = s_status_d ? 591 : 590;
      break;
    case 590:  // $175D
      /*$175D*/ s_a = (uint8_t)((s_a + 0x0070) + s_status_c);
                block_id = 592;
      break;
    case 591:  // $175D
      /*$175D*/ s_a = ((uint8_t)adc_dec16(s_a, 0x70, s_status_c));
                block_id = 592;
      break;
    case 592:  // $175F
      /*$175F*/ ram_poke(0x1544, s_a);
      /*$1764*/ ram_poke(0x1543, 0x00);
      /*$1767*/ func_1530(0x1769);
                branchTarget = true; block_id = 593;
      break;
    case 593:  // $176A
      /*$176A*/ CYCLES(0x176a, 10);
                s_a = ram_peek(0x0323);
      /*$176D*/ s_status_c = 0x01;
      /*$176E*/ block_id = s_status_d ? 595 : 594;
      break;
    case 594:  // $176E
      /*$176E*/ tmp2_U8 = (uint8_t)((s_a - 0x0004) - (uint8_t)(0x01 - s_status_c));
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 596;
      break;
    case 595:  // $176E
      /*$176E*/ tmp4_U16 = sbc_dec16(s_a, 0x04, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_n = ((uint8_t)(tmp4_U16 >> 8) & 0x80);
                block_id = 596;
      break;
    case 596:  // $1770
      /*$1770*/ branchTarget = true; block_id = !s_status_n ? 597 : 598;
      break;
    case 597:  // $1770
      /*$1770*/ CYCLES_EDGE(0x1770, 1);
      /*$178D*/ CYCLES(0x178d, 4);
                ram_poke(0x0323, s_a);
                block_id = 606;
      break;
    case 598:  // $1772
      /*$1772*/ CYCLES(0x1772, 22);
                s_status_c = 0x00;
      /*$1773*/ block_id = s_status_d ? 600 : 599;
      break;
    case 599:  // $1773
      /*$1773*/ tmp4_U16 = s_a;
                tmp5_U16 = (tmp4_U16 + 0x0007) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x0007);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 601;
      break;
    case 600:  // $1773
      /*$1773*/ tmp5_U16 = adc_dec16(s_a, 0x07, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
                block_id = 601;
      break;
    case 601:  // $1775
      /*$1775*/ ram_poke(0x0323, s_a);
      /*$1778*/ ram_poke(0x0322, (uint8_t)(ram_peek(0x0322) - 0x01));
      /*$177B*/ tmp2_U8 = ram_peek(0x0322);
                s_a = tmp2_U8;
      /*$1780*/ branchTarget = true; block_id = !(tmp2_U8 >= 0xf0) ? 602 : 603;
      break;
    case 602:  // $1780
      /*$1780*/ CYCLES_EDGE(0x1780, 1);
                branchTarget = true; block_id = 606;
      break;
    case 603:  // $1782
      /*$1782*/ CYCLES(0x1782, 4);
                tmp2_U8 = s_a >= 0xfc;
                s_status_c = tmp2_U8;
      /*$1784*/ branchTarget = true; block_id = tmp2_U8 ? 604 : 605;
      break;
    case 604:  // $1784
      /*$1784*/ CYCLES_EDGE(0x1784, 1);
                branchTarget = true; block_id = 606;
      break;
    case 605:  // $1786
      /*$1786*/ CYCLES(0x1786, 8);
                s_a = 0x00;
      /*$1788*/ ram_poke(0x0320, 0x00);
      /*$178B*/ CYCLES_EDGE(0x178b, 1);
                branchTarget = true; block_id = 613;
      break;
    case 606:  // $1790
      /*$1790*/ CYCLES(0x1790, 38);
                s_a = ram_peek(0x0323);
      /*$1793*/ s_status_c = 0x00;
      /*$1794*/ block_id = s_status_d ? 608 : 607;
      break;
    case 607:  // $1794
      /*$1794*/ s_a = (uint8_t)((s_a + 0x0070) + s_status_c);
                block_id = 609;
      break;
    case 608:  // $1794
      /*$1794*/ s_a = ((uint8_t)adc_dec16(s_a, 0x70, s_status_c));
                block_id = 609;
      break;
    case 609:  // $1796
      /*$1796*/ ram_poke(0x1514, s_a);
      /*$179B*/ ram_poke(0x1513, 0x00);
      /*$179E*/ tmp2_U8 = ram_peek(0x0322);
                s_a = tmp2_U8;
      /*$17A1*/ ram_poke(0x0006, tmp2_U8);
      /*$17A3*/ s_status_c = 0x00;
      /*$17A4*/ block_id = s_status_d ? 611 : 610;
      break;
    case 610:  // $17A4
      /*$17A4*/ tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 + 0x0004) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x0004);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 612;
      break;
    case 611:  // $17A4
      /*$17A4*/ tmp4_U16 = adc_dec16(s_a, 0x04, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 612;
      break;
    case 612:  // $17A6
      /*$17A6*/ ram_poke(0x0007, s_a);
      /*$17A8*/ func_1500(0x17aa);
                branchTarget = true; block_id = 613;
      break;
    case 613:  // $17AB
      /*$17AB*/ CYCLES(0x17ab, 3);
      /*$1720*/ CYCLES(0x1720, 4);
                s_x = ram_peek(0x0323);
                block_id = 571;
      break;
    case 614:  // $17B4
      /*$17B4*/ s_a = (uint8_t)((s_a - 0x0004) - (uint8_t)(0x01 - s_status_c));
                block_id = 616;
      break;
    case 615:  // $17B4
      /*$17B4*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x04, s_status_c));
                block_id = 616;
      break;
    case 616:  // $17B6
      /*$17B6*/ ram_poke(0x000b, s_a);
      /*$17B8*/ s_a = ram_peek(0x0322);
      /*$17BB*/ s_status_c = 0x01;
      /*$17BC*/ block_id = s_status_d ? 618 : 617;
      break;
    case 617:  // $17BC
      /*$17BC*/ tmp4_U16 = s_a;
                tmp5_U16 = (tmp4_U16 - 0x0001) - (uint8_t)(0x01 - s_status_c);
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0xfffe);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 619;
      break;
    case 618:  // $17BC
      /*$17BC*/ tmp5_U16 = sbc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
                block_id = 619;
      break;
    case 619:  // $17BE
      /*$17BE*/ ram_poke(0x000c, s_a);
      /*$17C0*/ tmp2_U8 = ram_peek(0x0323);
                s_a = tmp2_U8;
      /*$17C3*/ ram_poke(0x000a, tmp2_U8);
      /*$17C5*/ func_2b60(0x17c7);
      /*$17C8*/ CYCLES(0x17c8, 12);
                ram_poke(0x000a, (uint8_t)(ram_peek(0x000a) + 0x01));
      /*$17CA*/ tmp2_U8 = ram_peek(0x000a);
                s_a = tmp2_U8;
      /*$17CC*/ s_status_not_z = (tmp2_U8 != 0x04);
                tmp1_U8 = tmp2_U8 >= 0x04;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - 0x04) & 0x80);
      /*$17CE*/ branchTarget = true; block_id = tmp1_U8 ? 620 : 621;
      break;
    case 620:  // $17CE
      /*$17CE*/ CYCLES_EDGE(0x17ce, 1);
      /*$17D4*/ CYCLES(0x17d4, 12);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$17D6*/ ram_poke(0x0320, 0x00);
      /*$17D9*/ branchTarget = true; block_id = find_block_id_func_t001(0x17d9, pop16() + 1);;
      break;
    case 621:  // $17D0
      /*$17D0*/ CYCLES(0x17d0, 10);
                ram_poke(0x0323, s_a);
      /*$17D3*/ branchTarget = true; block_id = find_block_id_func_t001(0x17d3, pop16() + 1);;
      break;
    case 622:  // $17E4
      /*$17E4*/ CYCLES_EDGE(0x17e4, 1);
                branchTarget = true; block_id = 636;
      break;
    case 623:  // $17E6
      /*$17E6*/ CYCLES(0x17e6, 12);
                s_a = ram_peek(0x0071);
      /*$17E8*/ s_status_c = 0x00;
      /*$17E9*/ block_id = s_status_d ? 625 : 624;
      break;
    case 624:  // $17E9
      /*$17E9*/ s_a = (uint8_t)((s_a + 0x0008) + s_status_c);
                block_id = 626;
      break;
    case 625:  // $17E9
      /*$17E9*/ s_a = ((uint8_t)adc_dec16(s_a, 0x08, s_status_c));
                block_id = 626;
      break;
    case 626:  // $17EB
      /*$17ED*/ branchTarget = true; block_id = (s_a != ram_peek(0x0005)) ? 627 : 628;
      break;
    case 627:  // $17ED
      /*$17ED*/ CYCLES_EDGE(0x17ed, 1);
                branchTarget = true; block_id = 636;
      break;
    case 628:  // $17EF
      /*$17EF*/ CYCLES(0x17ef, 8);
                tmp1_U8 = ram_peek(0x0072);
                s_a = tmp1_U8;
      /*$17F3*/ branchTarget = true; block_id = (tmp1_U8 >= ram_peek(0x0007)) ? 629 : 630;
      break;
    case 629:  // $17F3
      /*$17F3*/ CYCLES_EDGE(0x17f3, 1);
                branchTarget = true; block_id = 636;
      break;
    case 630:  // $17F5
      /*$17F5*/ CYCLES(0x17f5, 9);
                s_status_c = 0x00;
      /*$17F6*/ block_id = s_status_d ? 632 : 631;
      break;
    case 631:  // $17F6
      /*$17F6*/ s_a = (uint8_t)((s_a + 0x0008) + s_status_c);
                block_id = 633;
      break;
    case 632:  // $17F6
      /*$17F6*/ s_a = ((uint8_t)adc_dec16(s_a, 0x08, s_status_c));
                block_id = 633;
      break;
    case 633:  // $17F8
      /*$17FA*/ branchTarget = true; block_id = !(s_a >= ram_peek(0x0007)) ? 634 : 635;
      break;
    case 634:  // $17FA
      /*$17FA*/ CYCLES_EDGE(0x17fa, 1);
                branchTarget = true; block_id = 636;
      break;
    case 635:  // $17FC
      /*$17FC*/ CYCLES(0x17fc, 3);
      /*$1803*/ CYCLES(0x1803, 17);
                ram_poke(0x0070, (uint8_t)(ram_peek(0x0070) + 0x01));
      /*$1805*/ ram_poke(0x0077, (uint8_t)(ram_peek(0x0077) + 0x01));
      /*$1807*/ s_x = 0x00;
      /*$1809*/ s_y = 0x05;
      /*$180B*/ func_2e00(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x180b, pop16() + 1);;
      break;
    case 636:  // $1800
      /*$1800*/ CYCLES(0x1800, 3);
      /*$1950*/ CYCLES(0x1950, 10);
      /*$1952*/ ram_poke(0x0008, 0x00);
      /*$1956*/ ram_poke(0x0009, 0x03);
                block_id = 707;
      break;
    case 637:  // $1862
      /*$1862*/ CYCLES_EDGE(0x1862, 1);
      /*$1880*/ CYCLES(0x1880, 10);
      /*$1882*/ ram_poke(0x0008, 0x00);
      /*$1886*/ ram_poke(0x0009, 0x03);
                block_id = 642;
      break;
    case 638:  // $1864
      /*$1864*/ CYCLES(0x1864, 4);
                tmp1_U8 = s_a;
                tmp2_U8 = tmp1_U8 != 0x01;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp1_U8 >= 0x01);
                s_status_n = ((uint8_t)(tmp1_U8 - 0x01) & 0x80);
      /*$1866*/ branchTarget = true; block_id = tmp2_U8 ? 639 : 640;
      break;
    case 639:  // $1866
      /*$1866*/ CYCLES_EDGE(0x1866, 1);
                branchTarget = true; block_id = 641;
      break;
    case 640:  // $1868
      /*$1868*/ CYCLES(0x1868, 6);
                func_18e4(0x186a);
                branchTarget = true; block_id = 641;
      break;
    case 641:  // $186B
      /*$186B*/ CYCLES(0x186b, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x186b, pop16() + 1);;
      break;
    case 642:  // $1888
      /*$1888*/ CYCLES(0x1888, 11);
                s_y = 0x00;
      /*$188A*/ tmp1_U8 = peek(ram_peek16al(0x0008));
      /*$188D*/ branchTarget = true; block_id = !(tmp1_U8 & 0x01) ? 643 : 644;
      break;
    case 643:  // $188D
      /*$188D*/ CYCLES_EDGE(0x188d, 1);
                branchTarget = true; block_id = 668;
      break;
    case 644:  // $188F
      /*$188F*/ CYCLES(0x188f, 12);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$1890*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$1894*/ branchTarget = true; block_id = (tmp1_U8 >= ram_peek(0x0005)) ? 645 : 646;
      break;
    case 645:  // $1894
      /*$1894*/ CYCLES_EDGE(0x1894, 1);
                branchTarget = true; block_id = 668;
      break;
    case 646:  // $1896
      /*$1896*/ CYCLES(0x1896, 9);
                s_status_c = 0x00;
      /*$1897*/ block_id = s_status_d ? 648 : 647;
      break;
    case 647:  // $1897
      /*$1897*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
                block_id = 649;
      break;
    case 648:  // $1897
      /*$1897*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
                block_id = 649;
      break;
    case 649:  // $1899
      /*$189B*/ branchTarget = true; block_id = !(s_a >= ram_peek(0x0005)) ? 650 : 651;
      break;
    case 650:  // $189B
      /*$189B*/ CYCLES_EDGE(0x189b, 1);
                branchTarget = true; block_id = 668;
      break;
    case 651:  // $189D
      /*$189D*/ CYCLES(0x189d, 12);
      /*$189E*/ tmp1_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(s_y + 0x01)));
                s_a = tmp1_U8;
      /*$18A2*/ branchTarget = true; block_id = (tmp1_U8 >= ram_peek(0x0007)) ? 652 : 653;
      break;
    case 652:  // $18A2
      /*$18A2*/ CYCLES_EDGE(0x18a2, 1);
                branchTarget = true; block_id = 668;
      break;
    case 653:  // $18A4
      /*$18A4*/ CYCLES(0x18a4, 9);
                s_status_c = 0x00;
      /*$18A5*/ block_id = s_status_d ? 655 : 654;
      break;
    case 654:  // $18A5
      /*$18A5*/ s_a = (uint8_t)((s_a + 0x0003) + s_status_c);
                block_id = 656;
      break;
    case 655:  // $18A5
      /*$18A5*/ s_a = ((uint8_t)adc_dec16(s_a, 0x03, s_status_c));
                block_id = 656;
      break;
    case 656:  // $18A7
      /*$18A9*/ branchTarget = true; block_id = !(s_a >= ram_peek(0x0007)) ? 657 : 658;
      break;
    case 657:  // $18A9
      /*$18A9*/ CYCLES_EDGE(0x18a9, 1);
                branchTarget = true; block_id = 668;
      break;
    case 658:  // $18AB
      /*$18AB*/ CYCLES(0x18ab, 68);
      /*$18AF*/ poke(ram_peek16al(0x0008), 0x02);
      /*$18B1*/ s_y = 0x01;
      /*$18B2*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0001));
                s_a = tmp1_U8;
      /*$18B4*/ ram_poke(0x0004, tmp1_U8);
      /*$18B6*/ s_status_c = 0x00;
      /*$18B7*/ block_id = s_status_d ? 660 : 659;
      break;
    case 659:  // $18B7
      /*$18B7*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
                block_id = 661;
      break;
    case 660:  // $18B7
      /*$18B7*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
                block_id = 661;
      break;
    case 661:  // $18B9
      /*$18B9*/ ram_poke(0x0005, s_a);
      /*$18BB*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$18BC*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$18BE*/ ram_poke(0x0006, tmp1_U8);
      /*$18C0*/ s_status_c = 0x00;
      /*$18C1*/ block_id = s_status_d ? 663 : 662;
      break;
    case 662:  // $18C1
      /*$18C1*/ s_a = (uint8_t)((s_a + 0x0002) + s_status_c);
                block_id = 664;
      break;
    case 663:  // $18C1
      /*$18C1*/ s_a = ((uint8_t)adc_dec16(s_a, 0x02, s_status_c));
                block_id = 664;
      break;
    case 664:  // $18C3
      /*$18C3*/ ram_poke(0x0007, s_a);
      /*$18C6*/ tmp1_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(s_y + 0x01)));
                s_a = tmp1_U8;
      /*$18C8*/ s_status_c = 0x00;
      /*$18C9*/ block_id = s_status_d ? 666 : 665;
      break;
    case 665:  // $18C9
      /*$18C9*/ s_a = (uint8_t)((s_a + 0x0067) + s_status_c);
                block_id = 667;
      break;
    case 666:  // $18C9
      /*$18C9*/ s_a = ((uint8_t)adc_dec16(s_a, 0x67, s_status_c));
                block_id = 667;
      break;
    case 667:  // $18CB
      /*$18CB*/ ram_poke(0x1544, s_a);
      /*$18D0*/ ram_poke(0x1543, 0xc0);
      /*$18D3*/ branchTarget = true; block_id = 778;
      break;
    case 668:  // $18D9
      /*$18D9*/ CYCLES(0x18d9, 14);
                s_a = ram_peek(0x0008);
      /*$18DB*/ s_status_c = 0x00;
      /*$18DC*/ block_id = s_status_d ? 670 : 669;
      break;
    case 669:  // $18DC
      /*$18DC*/ tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 + 0x0004) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x0004);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 671;
      break;
    case 670:  // $18DC
      /*$18DC*/ tmp4_U16 = adc_dec16(s_a, 0x04, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 671;
      break;
    case 671:  // $18DE
      /*$18DE*/ tmp1_U8 = s_a;
                ram_poke(0x0008, tmp1_U8);
      /*$18E2*/ branchTarget = true; block_id = !(tmp1_U8 >= 0x10) ? 672 : 1851;
      break;
    case 672:  // $18E2
      /*$18E2*/ CYCLES_EDGE(0x18e2, 1);
                branchTarget = true; block_id = 642;
      break;
    case 673:  // $18E4
      /*$18E4*/ CYCLES(0x18e4, 21);
      /*$18E6*/ ram_poke(0x0008, 0x20);
      /*$18EA*/ ram_poke(0x0009, 0x03);
      /*$18EC*/ s_y = 0x00;
      /*$18EE*/ tmp2_U8 = peek(ram_peek16al(0x0008));
      /*$18F0*/ tmp1_U8 = tmp2_U8 & 0x01;
                s_status_c = tmp1_U8;
                tmp2_U8 = tmp2_U8 >> 0x01;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$18F1*/ branchTarget = true; block_id = !tmp1_U8 ? 674 : 675;
      break;
    case 674:  // $18F1
      /*$18F1*/ CYCLES_EDGE(0x18f1, 1);
                branchTarget = true; block_id = 699;
      break;
    case 675:  // $18F3
      /*$18F3*/ CYCLES(0x18f3, 12);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$18F4*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$18F6*/ tmp2_U8 = ram_peek(0x0005);
                s_status_not_z = (tmp1_U8 != tmp2_U8);
                tmp3_U8 = tmp1_U8 >= tmp2_U8;
                s_status_c = tmp3_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
      /*$18F8*/ branchTarget = true; block_id = tmp3_U8 ? 676 : 677;
      break;
    case 676:  // $18F8
      /*$18F8*/ CYCLES_EDGE(0x18f8, 1);
                branchTarget = true; block_id = 699;
      break;
    case 677:  // $18FA
      /*$18FA*/ CYCLES(0x18fa, 9);
                s_status_c = 0x00;
      /*$18FB*/ block_id = s_status_d ? 679 : 678;
      break;
    case 678:  // $18FB
      /*$18FB*/ tmp4_U16 = s_a;
                tmp5_U16 = (tmp4_U16 + 0x000f) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x000f);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 680;
      break;
    case 679:  // $18FB
      /*$18FB*/ tmp5_U16 = adc_dec16(s_a, 0x0f, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
                block_id = 680;
      break;
    case 680:  // $18FD
      /*$18FD*/ tmp3_U8 = s_a;
                tmp2_U8 = ram_peek(0x0005);
                s_status_not_z = (tmp3_U8 != tmp2_U8);
                tmp1_U8 = tmp3_U8 >= tmp2_U8;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp3_U8 - tmp2_U8) & 0x80);
      /*$18FF*/ branchTarget = true; block_id = !tmp1_U8 ? 681 : 682;
      break;
    case 681:  // $18FF
      /*$18FF*/ CYCLES_EDGE(0x18ff, 1);
                branchTarget = true; block_id = 699;
      break;
    case 682:  // $1901
      /*$1901*/ CYCLES(0x1901, 12);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$1902*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$1904*/ tmp2_U8 = ram_peek(0x0007);
                s_status_not_z = (tmp1_U8 != tmp2_U8);
                tmp3_U8 = tmp1_U8 >= tmp2_U8;
                s_status_c = tmp3_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
      /*$1906*/ branchTarget = true; block_id = tmp3_U8 ? 683 : 684;
      break;
    case 683:  // $1906
      /*$1906*/ CYCLES_EDGE(0x1906, 1);
                branchTarget = true; block_id = 699;
      break;
    case 684:  // $1908
      /*$1908*/ CYCLES(0x1908, 9);
                s_status_c = 0x00;
      /*$1909*/ block_id = s_status_d ? 686 : 685;
      break;
    case 685:  // $1909
      /*$1909*/ tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 + 0x0005) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x0005);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 687;
      break;
    case 686:  // $1909
      /*$1909*/ tmp4_U16 = adc_dec16(s_a, 0x05, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 687;
      break;
    case 687:  // $190B
      /*$190B*/ tmp3_U8 = s_a;
                tmp2_U8 = ram_peek(0x0007);
                s_status_not_z = (tmp3_U8 != tmp2_U8);
                tmp1_U8 = tmp3_U8 >= tmp2_U8;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp3_U8 - tmp2_U8) & 0x80);
      /*$190D*/ branchTarget = true; block_id = !tmp1_U8 ? 688 : 689;
      break;
    case 688:  // $190D
      /*$190D*/ CYCLES_EDGE(0x190d, 1);
                branchTarget = true; block_id = 699;
      break;
    case 689:  // $190F
      /*$190F*/ CYCLES(0x190f, 71);
      /*$1913*/ poke(ram_peek16al(0x0008), 0x02);
      /*$1915*/ s_y = 0x01;
      /*$1916*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0001));
                s_a = tmp1_U8;
      /*$1918*/ ram_poke(0x0004, tmp1_U8);
      /*$191A*/ s_status_c = 0x00;
      /*$191B*/ block_id = s_status_d ? 691 : 690;
      break;
    case 690:  // $191B
      /*$191B*/ s_a = (uint8_t)((s_a + 0x000f) + s_status_c);
                block_id = 692;
      break;
    case 691:  // $191B
      /*$191B*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0f, s_status_c));
                block_id = 692;
      break;
    case 692:  // $191D
      /*$191D*/ ram_poke(0x0005, s_a);
      /*$191F*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$1920*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$1922*/ ram_poke(0x0006, tmp1_U8);
      /*$1924*/ s_status_c = 0x00;
      /*$1925*/ block_id = s_status_d ? 694 : 693;
      break;
    case 693:  // $1925
      /*$1925*/ s_a = (uint8_t)((s_a + 0x0004) + s_status_c);
                block_id = 695;
      break;
    case 694:  // $1925
      /*$1925*/ s_a = ((uint8_t)adc_dec16(s_a, 0x04, s_status_c));
                block_id = 695;
      break;
    case 695:  // $1927
      /*$1927*/ ram_poke(0x0007, s_a);
      /*$1929*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$192A*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$192C*/ s_status_c = 0x00;
      /*$192D*/ block_id = s_status_d ? 697 : 696;
      break;
    case 696:  // $192D
      /*$192D*/ s_a = (uint8_t)((s_a + 0x0070) + s_status_c);
                block_id = 698;
      break;
    case 697:  // $192D
      /*$192D*/ s_a = ((uint8_t)adc_dec16(s_a, 0x70, s_status_c));
                block_id = 698;
      break;
    case 698:  // $192F
      /*$192F*/ ram_poke(0x1544, s_a);
      /*$1934*/ ram_poke(0x1543, 0x00);
      /*$1937*/ func_1af0(0x1939);
      /*$193A*/ CYCLES(0x193a, 3);
                branchTarget = true; block_id = 700;
      break;
    case 699:  // $193D
      /*$193D*/ CYCLES(0x193d, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x193d, pop16() + 1);;
      break;
    case 700:  // $1940
      /*$1940*/ CYCLES(0x1940, 9);
                s_a = ram_peek(0x0089);
      /*$1942*/ s_status_c = 0x00;
      /*$1943*/ block_id = s_status_d ? 702 : 701;
      break;
    case 701:  // $1943
      /*$1943*/ tmp4_U16 = s_a;
                tmp5_U16 = (tmp4_U16 + 0x0050) + s_status_c;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x0050);
                tmp1_U8 = (uint8_t)tmp5_U16;
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
                block_id = 703;
      break;
    case 702:  // $1943
      /*$1943*/ tmp5_U16 = adc_dec16(s_a, 0x50, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_not_z = (~tmp1_U8 & 2);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                s_status_n = (tmp1_U8 & 0x80);
                block_id = 703;
      break;
    case 703:  // $1945
      /*$1945*/ branchTarget = true; block_id = !s_status_c ? 704 : 705;
      break;
    case 704:  // $1945
      /*$1945*/ CYCLES_EDGE(0x1945, 1);
                branchTarget = true; block_id = 706;
      break;
    case 705:  // $1947
      /*$1947*/ CYCLES(0x1947, 2);
                s_status_not_z = 0xff;
                s_status_n = 0x80;
                s_a = 0xff;
                block_id = 706;
      break;
    case 706:  // $1949
      /*$1949*/ CYCLES(0x1949, 9);
                ram_poke(0x0089, s_a);
      /*$194B*/ branchTarget = true; block_id = find_block_id_func_t001(0x194b, pop16() + 1);;
      break;
    case 707:  // $1958
      /*$1958*/ CYCLES(0x1958, 11);
                s_y = 0x00;
      /*$195A*/ tmp1_U8 = peek(ram_peek16al(0x0008));
      /*$195D*/ branchTarget = true; block_id = !(tmp1_U8 & 0x01) ? 708 : 709;
      break;
    case 708:  // $195D
      /*$195D*/ CYCLES_EDGE(0x195d, 1);
                branchTarget = true; block_id = 733;
      break;
    case 709:  // $195F
      /*$195F*/ CYCLES(0x195f, 12);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$1960*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$1964*/ branchTarget = true; block_id = (tmp1_U8 >= ram_peek(0x0005)) ? 710 : 711;
      break;
    case 710:  // $1964
      /*$1964*/ CYCLES_EDGE(0x1964, 1);
                branchTarget = true; block_id = 733;
      break;
    case 711:  // $1966
      /*$1966*/ CYCLES(0x1966, 9);
                s_status_c = 0x00;
      /*$1967*/ block_id = s_status_d ? 713 : 712;
      break;
    case 712:  // $1967
      /*$1967*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
                block_id = 714;
      break;
    case 713:  // $1967
      /*$1967*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
                block_id = 714;
      break;
    case 714:  // $1969
      /*$196B*/ branchTarget = true; block_id = !(s_a >= ram_peek(0x0005)) ? 715 : 716;
      break;
    case 715:  // $196B
      /*$196B*/ CYCLES_EDGE(0x196b, 1);
                branchTarget = true; block_id = 733;
      break;
    case 716:  // $196D
      /*$196D*/ CYCLES(0x196d, 12);
      /*$196E*/ tmp1_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(s_y + 0x01)));
                s_a = tmp1_U8;
      /*$1972*/ branchTarget = true; block_id = (tmp1_U8 >= ram_peek(0x0007)) ? 717 : 718;
      break;
    case 717:  // $1972
      /*$1972*/ CYCLES_EDGE(0x1972, 1);
                branchTarget = true; block_id = 733;
      break;
    case 718:  // $1974
      /*$1974*/ CYCLES(0x1974, 9);
                s_status_c = 0x00;
      /*$1975*/ block_id = s_status_d ? 720 : 719;
      break;
    case 719:  // $1975
      /*$1975*/ s_a = (uint8_t)((s_a + 0x0003) + s_status_c);
                block_id = 721;
      break;
    case 720:  // $1975
      /*$1975*/ s_a = ((uint8_t)adc_dec16(s_a, 0x03, s_status_c));
                block_id = 721;
      break;
    case 721:  // $1977
      /*$1979*/ branchTarget = true; block_id = !(s_a >= ram_peek(0x0007)) ? 722 : 723;
      break;
    case 722:  // $1979
      /*$1979*/ CYCLES_EDGE(0x1979, 1);
                branchTarget = true; block_id = 733;
      break;
    case 723:  // $197B
      /*$197B*/ CYCLES(0x197b, 68);
      /*$197F*/ poke(ram_peek16al(0x0008), 0x02);
      /*$1981*/ s_y = 0x01;
      /*$1982*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0001));
                s_a = tmp1_U8;
      /*$1984*/ ram_poke(0x0004, tmp1_U8);
      /*$1986*/ s_status_c = 0x00;
      /*$1987*/ block_id = s_status_d ? 725 : 724;
      break;
    case 724:  // $1987
      /*$1987*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
                block_id = 726;
      break;
    case 725:  // $1987
      /*$1987*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
                block_id = 726;
      break;
    case 726:  // $1989
      /*$1989*/ ram_poke(0x0005, s_a);
      /*$198B*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$198C*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$198E*/ ram_poke(0x0006, tmp1_U8);
      /*$1990*/ s_status_c = 0x00;
      /*$1991*/ block_id = s_status_d ? 728 : 727;
      break;
    case 727:  // $1991
      /*$1991*/ s_a = (uint8_t)((s_a + 0x0002) + s_status_c);
                block_id = 729;
      break;
    case 728:  // $1991
      /*$1991*/ s_a = ((uint8_t)adc_dec16(s_a, 0x02, s_status_c));
                block_id = 729;
      break;
    case 729:  // $1993
      /*$1993*/ ram_poke(0x0007, s_a);
      /*$1996*/ tmp1_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(s_y + 0x01)));
                s_a = tmp1_U8;
      /*$1998*/ s_status_c = 0x00;
      /*$1999*/ block_id = s_status_d ? 731 : 730;
      break;
    case 730:  // $1999
      /*$1999*/ s_a = (uint8_t)((s_a + 0x0067) + s_status_c);
                block_id = 732;
      break;
    case 731:  // $1999
      /*$1999*/ s_a = ((uint8_t)adc_dec16(s_a, 0x67, s_status_c));
                block_id = 732;
      break;
    case 732:  // $199B
      /*$199B*/ ram_poke(0x1544, s_a);
      /*$19A0*/ ram_poke(0x1543, 0xc0);
      /*$19A3*/ branchTarget = true; block_id = 778;
      break;
    case 733:  // $19A9
      /*$19A9*/ CYCLES(0x19a9, 14);
                s_a = ram_peek(0x0008);
      /*$19AB*/ s_status_c = 0x00;
      /*$19AC*/ block_id = s_status_d ? 735 : 734;
      break;
    case 734:  // $19AC
      /*$19AC*/ tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 + 0x0004) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x0004);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 736;
      break;
    case 735:  // $19AC
      /*$19AC*/ tmp4_U16 = adc_dec16(s_a, 0x04, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 736;
      break;
    case 736:  // $19AE
      /*$19AE*/ tmp1_U8 = s_a;
                ram_poke(0x0008, tmp1_U8);
      /*$19B2*/ branchTarget = true; block_id = !(tmp1_U8 >= 0x10) ? 737 : 738;
      break;
    case 737:  // $19B2
      /*$19B2*/ CYCLES_EDGE(0x19b2, 1);
                branchTarget = true; block_id = 707;
      break;
    case 738:  // $19B4
      /*$19B4*/ CYCLES(0x19b4, 21);
      /*$19B6*/ ram_poke(0x0008, 0x20);
      /*$19BA*/ ram_poke(0x0009, 0x03);
      /*$19BC*/ s_y = 0x00;
      /*$19BE*/ tmp1_U8 = peek(ram_peek16al(0x0008));
      /*$19C0*/ tmp1_U8 = tmp1_U8 & 0x01;
                s_status_c = tmp1_U8;
      /*$19C1*/ branchTarget = true; block_id = !tmp1_U8 ? 739 : 740;
      break;
    case 739:  // $19C1
      /*$19C1*/ CYCLES_EDGE(0x19c1, 1);
                branchTarget = true; block_id = 764;
      break;
    case 740:  // $19C3
      /*$19C3*/ CYCLES(0x19c3, 12);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$19C4*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$19C6*/ tmp1_U8 = tmp1_U8 >= ram_peek(0x0005);
                s_status_c = tmp1_U8;
      /*$19C8*/ branchTarget = true; block_id = tmp1_U8 ? 741 : 742;
      break;
    case 741:  // $19C8
      /*$19C8*/ CYCLES_EDGE(0x19c8, 1);
                branchTarget = true; block_id = 764;
      break;
    case 742:  // $19CA
      /*$19CA*/ CYCLES(0x19ca, 9);
                s_status_c = 0x00;
      /*$19CB*/ block_id = s_status_d ? 744 : 743;
      break;
    case 743:  // $19CB
      /*$19CB*/ tmp4_U16 = s_a;
                tmp5_U16 = (tmp4_U16 + 0x000f) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x000f);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 745;
      break;
    case 744:  // $19CB
      /*$19CB*/ tmp5_U16 = adc_dec16(s_a, 0x0f, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
                block_id = 745;
      break;
    case 745:  // $19CD
      /*$19CD*/ tmp1_U8 = s_a >= ram_peek(0x0005);
                s_status_c = tmp1_U8;
      /*$19CF*/ branchTarget = true; block_id = !tmp1_U8 ? 746 : 747;
      break;
    case 746:  // $19CF
      /*$19CF*/ CYCLES_EDGE(0x19cf, 1);
                branchTarget = true; block_id = 764;
      break;
    case 747:  // $19D1
      /*$19D1*/ CYCLES(0x19d1, 12);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$19D2*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$19D4*/ tmp1_U8 = tmp1_U8 >= ram_peek(0x0007);
                s_status_c = tmp1_U8;
      /*$19D6*/ branchTarget = true; block_id = tmp1_U8 ? 748 : 749;
      break;
    case 748:  // $19D6
      /*$19D6*/ CYCLES_EDGE(0x19d6, 1);
                branchTarget = true; block_id = 764;
      break;
    case 749:  // $19D8
      /*$19D8*/ CYCLES(0x19d8, 9);
                s_status_c = 0x00;
      /*$19D9*/ block_id = s_status_d ? 751 : 750;
      break;
    case 750:  // $19D9
      /*$19D9*/ tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 + 0x0005) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x0005);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 752;
      break;
    case 751:  // $19D9
      /*$19D9*/ tmp4_U16 = adc_dec16(s_a, 0x05, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 752;
      break;
    case 752:  // $19DB
      /*$19DB*/ tmp1_U8 = s_a >= ram_peek(0x0007);
                s_status_c = tmp1_U8;
      /*$19DD*/ branchTarget = true; block_id = !tmp1_U8 ? 753 : 754;
      break;
    case 753:  // $19DD
      /*$19DD*/ CYCLES_EDGE(0x19dd, 1);
                branchTarget = true; block_id = 764;
      break;
    case 754:  // $19DF
      /*$19DF*/ CYCLES(0x19df, 71);
      /*$19E3*/ poke(ram_peek16al(0x0008), 0x02);
      /*$19E5*/ s_y = 0x01;
      /*$19E6*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0001));
                s_a = tmp1_U8;
      /*$19E8*/ ram_poke(0x0004, tmp1_U8);
      /*$19EA*/ s_status_c = 0x00;
      /*$19EB*/ block_id = s_status_d ? 756 : 755;
      break;
    case 755:  // $19EB
      /*$19EB*/ s_a = (uint8_t)((s_a + 0x000f) + s_status_c);
                block_id = 757;
      break;
    case 756:  // $19EB
      /*$19EB*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0f, s_status_c));
                block_id = 757;
      break;
    case 757:  // $19ED
      /*$19ED*/ ram_poke(0x0005, s_a);
      /*$19EF*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$19F0*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$19F2*/ ram_poke(0x0006, tmp1_U8);
      /*$19F4*/ s_status_c = 0x00;
      /*$19F5*/ block_id = s_status_d ? 759 : 758;
      break;
    case 758:  // $19F5
      /*$19F5*/ s_a = (uint8_t)((s_a + 0x0004) + s_status_c);
                block_id = 760;
      break;
    case 759:  // $19F5
      /*$19F5*/ s_a = ((uint8_t)adc_dec16(s_a, 0x04, s_status_c));
                block_id = 760;
      break;
    case 760:  // $19F7
      /*$19F7*/ ram_poke(0x0007, s_a);
      /*$19F9*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$19FA*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$19FC*/ s_status_c = 0x00;
      /*$19FD*/ block_id = s_status_d ? 762 : 761;
      break;
    case 761:  // $19FD
      /*$19FD*/ s_a = (uint8_t)((s_a + 0x0070) + s_status_c);
                block_id = 763;
      break;
    case 762:  // $19FD
      /*$19FD*/ s_a = ((uint8_t)adc_dec16(s_a, 0x70, s_status_c));
                block_id = 763;
      break;
    case 763:  // $19FF
      /*$19FF*/ ram_poke(0x1544, s_a);
      /*$1A04*/ ram_poke(0x1543, 0x00);
      /*$1A07*/ func_1af0(0x1a09);
      /*$1A0A*/ CYCLES(0x1a0a, 3);
                branchTarget = true; block_id = 700;
      break;
    case 764:  // $1A0D
      /*$1A0D*/ CYCLES(0x1a0d, 5);
                tmp1_U8 = ram_peek(0x007d);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$1A0F*/ branchTarget = true; block_id = !tmp1_U8 ? 765 : 766;
      break;
    case 765:  // $1A0F
      /*$1A0F*/ CYCLES_EDGE(0x1a0f, 1);
      /*$1A18*/ CYCLES(0x1a18, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x1a18, pop16() + 1);;
      break;
    case 766:  // $1A11
      /*$1A11*/ CYCLES(0x1a11, 16);
                ram_poke(0x007e, (uint8_t)(ram_peek(0x007e) + 0x01));
      /*$1A13*/ s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$1A15*/ ram_poke(0x007d, 0x00);
      /*$1A17*/ branchTarget = true; block_id = find_block_id_func_t001(0x1a17, pop16() + 1);;
      break;
    case 767:  // $1A20
      /*$1A20*/ CYCLES(0x1a20, 5);
                tmp2_U8 = ram_peek(0x00f5);
                s_a = tmp2_U8;
      /*$1A22*/ branchTarget = true; block_id = tmp2_U8 ? 768 : 769;
      break;
    case 768:  // $1A22
      /*$1A22*/ CYCLES_EDGE(0x1a22, 1);
      /*$1A50*/ CYCLES(0x1a50, 4);
                tmp1_U8 = s_a;
                s_status_c = (tmp1_U8 & 0x01);
                tmp1_U8 = tmp1_U8 >> 0x01;
                s_a = tmp1_U8;
      /*$1A51*/ branchTarget = true; block_id = tmp1_U8 ? 773 : 774;
      break;
    case 769:  // $1A24
      /*$1A24*/ CYCLES(0x1a24, 44);
                ram_poke(0x00f5, (uint8_t)(ram_peek(0x00f5) + 0x01));
      /*$1A28*/ ram_poke(0x00f6, 0x00);
      /*$1A2C*/ ram_poke(0x00f7, 0x8f);
      /*$1A30*/ ram_poke(0x00f8, 0x00);
      /*$1A34*/ ram_poke(0x00f9, 0x93);
      /*$1A38*/ ram_poke(0x00fa, 0x01);
      /*$1A3C*/ ram_poke(0x00f2, 0x48);
      /*$1A40*/ ram_poke(0x00f3, 0x89);
      /*$1A42*/ s_y = 0x1f;
      /*$1A44*/ s_a = 0x00;
                block_id = 770;
      break;
    case 770:  // $1A46
      /*$1A46*/ CYCLES(0x1a46, 9);
                tmp1_U8 = s_y;
                ram_poke((0x0300 + tmp1_U8), s_a);
      /*$1A49*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_y = tmp1_U8;
      /*$1A4A*/ branchTarget = true; block_id = !(tmp1_U8 & 0x80) ? 771 : 772;
      break;
    case 771:  // $1A4A
      /*$1A4A*/ CYCLES_EDGE(0x1a4a, 1);
                branchTarget = true; block_id = 770;
      break;
    case 772:  // $1A4C
      /*$1A4C*/ CYCLES(0x1a4c, 3);
      /*$2079*/ CYCLES(0x2079, 11);
      /*$207B*/ ram_poke(0x00a0, 0x01);
      /*$207D*/ func_3800(0x207f);
      /*$2080*/ CYCLES(0x2080, 6);
                branchTarget = true; push16(0x2082); block_id = 2;
      break;
    case 773:  // $1A51
      /*$1A51*/ CYCLES_EDGE(0x1a51, 1);
      /*$1AA0*/ CYCLES(0x1aa0, 6);
                tmp1_U8 = (uint8_t)((s_a << 0x01) | s_status_c);
                s_a = tmp1_U8;
      /*$1AA3*/ branchTarget = true; block_id = (tmp1_U8 != 0x02) ? 780 : 781;
      break;
    case 774:  // $1A53
      /*$1A53*/ CYCLES(0x1a53, 45);
                ram_poke(0x00f5, (uint8_t)(ram_peek(0x00f5) + 0x01));
      /*$1A57*/ ram_poke(0x00f0, 0x50);
      /*$1A59*/ ram_poke(0x00f6, 0x50);
      /*$1A5D*/ ram_poke(0x00f1, 0x89);
      /*$1A61*/ ram_poke(0x00f7, 0x92);
      /*$1A65*/ ram_poke(0x00f2, 0x00);
      /*$1A69*/ ram_poke(0x00f3, 0x8c);
      /*$1A6D*/ ram_poke(0x00f4, 0x53);
      /*$1A71*/ ram_poke(0x00fa, 0x54);
      /*$1A73*/ s_y = 0x30;
                block_id = 775;
      break;
    case 775:  // $1A75
      /*$1A75*/ CYCLES(0x1a75, 11);
      /*$1A77*/ tmp1_U8 = s_y;
                ram_poke((0x0300 + tmp1_U8), 0x00);
      /*$1A7A*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_y = tmp1_U8;
      /*$1A7B*/ branchTarget = true; block_id = tmp1_U8 ? 776 : 777;
      break;
    case 776:  // $1A7B
      /*$1A7B*/ CYCLES_EDGE(0x1a7b, 1);
                branchTarget = true; block_id = 775;
      break;
    case 777:  // $1A7D
      /*$1A7D*/ CYCLES(0x1a7d, 6);
                func_2fa0(0x1a7f);
      /*$1A80*/ CYCLES(0x1a80, 11);
                s_a = 0x00;
      /*$1A82*/ ram_poke(0x00f8, 0x00);
      /*$1A84*/ ram_poke(0x00f9, 0x00);
      /*$1A86*/ branchTarget = true; block_id = 1069;
      break;
    case 778:  // $1A90
      /*$1A90*/ CYCLES(0x1a90, 16);
      /*$1A94*/ poke((ram_peek16al(0x0008) + 0x0003), 0x00);
      /*$1A96*/ func_1530(0x1a98);
                branchTarget = true; block_id = 779;
      break;
    case 779:  // $1A99
      /*$1A99*/ CYCLES(0x1a99, 7);
                s_x = 0x20;
      /*$1A9B*/ s_y = 0x00;
      /*$1A9D*/ func_2e00(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x1a9d, pop16() + 1);;
      break;
    case 780:  // $1AA3
      /*$1AA3*/ CYCLES_EDGE(0x1aa3, 1);
      /*$1AD0*/ CYCLES(0x1ad0, 4);
      /*$1AD2*/ branchTarget = true; block_id = (s_a != 0x03) ? 782 : 783;
      break;
    case 781:  // $1AA5
      /*$1AA5*/ CYCLES(0x1aa5, 53);
                ram_poke(0x00f5, (uint8_t)(ram_peek(0x00f5) + 0x01));
      /*$1AA9*/ ram_poke(0x00f0, 0x60);
      /*$1AAB*/ ram_poke(0x00f6, 0x60);
      /*$1AAF*/ ram_poke(0x00f1, 0x8c);
      /*$1AB3*/ ram_poke(0x00f7, 0x8c);
      /*$1AB7*/ ram_poke(0x00f2, 0x28);
      /*$1ABB*/ ram_poke(0x00f3, 0x8d);
      /*$1ABF*/ ram_poke(0x00f4, 0xa6);
      /*$1AC3*/ ram_poke(0x00fa, 0x00);
      /*$1AC5*/ s_a = ram_peek(0x0c00);
      /*$1AC8*/ func_2fd3(0x1aca);
      /*$1ACB*/ CYCLES(0x1acb, 3);
                branchTarget = true; block_id = 1069;
      break;
    case 782:  // $1AD2
      /*$1AD2*/ CYCLES_EDGE(0x1ad2, 1);
                branchTarget = true; block_id = 784;
      break;
    case 783:  // $1AD4
      /*$1AD4*/ CYCLES(0x1ad4, 11);
      /*$1AD6*/ ram_poke(0x0020, 0x01);
      /*$1AD8*/ func_2380(0x1ada);
      /*$1ADB*/ CYCLES(0x1adb, 3);
      /*$2180*/ CYCLES(0x2180, 6);
                branchTarget = true; push16(0x2182); block_id = 1209;
      break;
    case 784:  // $1AE0
      /*$1AE0*/ CYCLES(0x1ae0, 16);
      /*$1AE4*/ poke((ram_peek16al(0x0008) + 0x0003), 0x00);
      /*$1AE6*/ func_1530(0x1ae8);
      /*$1AE9*/ CYCLES(0x1ae9, 7);
                s_x = 0x80;
      /*$1AEB*/ s_y = 0x00;
      /*$1AED*/ func_2e00(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x1aed, pop16() + 1);;
      break;
    case 785:  // $1AF0
      /*$1AF0*/ CYCLES(0x1af0, 16);
      /*$1AF4*/ poke((ram_peek16al(0x0008) + 0x0003), 0x00);
      /*$1AF6*/ func_1530(0x1af8);
      /*$1AF9*/ CYCLES(0x1af9, 7);
                s_x = 0x50;
      /*$1AFB*/ s_y = 0x00;
      /*$1AFD*/ func_2e00(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x1afd, pop16() + 1);;
      break;
    case 786:  // $1B08
      /*$1B08*/ CYCLES(0x1b08, 9);
                s_y = 0x00;
      /*$1B0A*/ tmp2_U8 = peek(ram_peek16al(0x0008));
      /*$1B0C*/ branchTarget = true; block_id = tmp2_U8 ? 787 : 788;
      break;
    case 787:  // $1B0C
      /*$1B0C*/ CYCLES_EDGE(0x1b0c, 1);
      /*$1B60*/ CYCLES(0x1b60, 14);
                s_a = ram_peek(0x0008);
      /*$1B62*/ s_status_c = 0x00;
      /*$1B63*/ block_id = s_status_d ? 805 : 804;
      break;
    case 788:  // $1B0E
      /*$1B0E*/ CYCLES(0x1b0e, 63);
      /*$1B10*/ tmp2_U8 = s_y;
                poke((ram_peek16al(0x0008) + tmp2_U8), 0x01);
      /*$1B12*/ s_y = (uint8_t)(tmp2_U8 + 0x01);
      /*$1B13*/ s_a = s_x;
      /*$1B14*/ s_status_c = 0x01;
      /*$1B15*/ block_id = s_status_d ? 790 : 789;
      break;
    case 789:  // $1B15
      /*$1B15*/ s_a = (uint8_t)((s_a - 0x0018) - (uint8_t)(0x01 - s_status_c));
                block_id = 791;
      break;
    case 790:  // $1B15
      /*$1B15*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x18, s_status_c));
                block_id = 791;
      break;
    case 791:  // $1B17
      /*$1B17*/ tmp2_U8 = s_y;
                poke((ram_peek16al(0x0008) + tmp2_U8), s_a);
      /*$1B1C*/ poke((ram_peek16al(0x0008) + (uint8_t)(tmp2_U8 + 0x01)), ram_peek(0x008f));
      /*$1B21*/ poke((ram_peek16al(0x0008) + (uint8_t)(tmp2_U8 + 0x02)), 0x00);
      /*$1B26*/ poke((ram_peek16al(0x0008) + (uint8_t)(tmp2_U8 + 0x03)), 0x00);
      /*$1B29*/ poke((ram_peek16al(0x0008) + (uint8_t)(tmp2_U8 + 0x04)), 0x00);
      /*$1B2B*/ s_x = 0x01;
                block_id = 792;
      break;
    case 792:  // $1B2D
      /*$1B2D*/ CYCLES(0x1b2d, 25);
                tmp2_U8 = s_x;
      /*$1B30*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp2_U8)));
      /*$1B35*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp2_U8)));
      /*$1B39*/ tmp2_U8 = peek((ram_peek16al(0x0000) + 0x0027));
      /*$1B3C*/ branchTarget = true; block_id = !(uint8_t)(tmp2_U8 << 0x01) ? 793 : 794;
      break;
    case 793:  // $1B3C
      /*$1B3C*/ CYCLES_EDGE(0x1b3c, 1);
      /*$1B46*/ CYCLES(0x1b46, 43);
      /*$1B47*/ ram_poke(0x0000, s_x);
      /*$1B4B*/ tmp2_U8 = peek((ram_peek16al(0x0008) + 0x0001));
                s_a = tmp2_U8;
      /*$1B4D*/ s_status_c = 0x01;
      /*$1B4E*/ block_id = s_status_d ? 799 : 798;
      break;
    case 794:  // $1B3E
      /*$1B3E*/ CYCLES(0x1b3e, 11);
                s_a = s_x;
      /*$1B3F*/ s_status_c = 0x00;
      /*$1B40*/ block_id = s_status_d ? 796 : 795;
      break;
    case 795:  // $1B40
      /*$1B40*/ s_a = (uint8_t)((s_a + 0x0004) + s_status_c);
                block_id = 797;
      break;
    case 796:  // $1B40
      /*$1B40*/ s_a = ((uint8_t)adc_dec16(s_a, 0x04, s_status_c));
                block_id = 797;
      break;
    case 797:  // $1B42
      /*$1B42*/ s_x = s_a;
      /*$1B43*/ branchTarget = true; block_id = 792;
      break;
    case 798:  // $1B4E
      /*$1B4E*/ tmp4_U16 = (s_a - ram_peek(0x0000)) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_a = ((uint8_t)tmp4_U16);
                block_id = 800;
      break;
    case 799:  // $1B4E
      /*$1B4E*/ tmp4_U16 = sbc_dec16(s_a, ram_peek(0x0000), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 800;
      break;
    case 800:  // $1B50
      /*$1B50*/ block_id = s_status_d ? 802 : 801;
      break;
    case 801:  // $1B50
      /*$1B50*/ tmp4_U16 = s_a;
                tmp5_U16 = (tmp4_U16 - 0x0004) - (uint8_t)(0x01 - s_status_c);
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0xfffb);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 803;
      break;
    case 802:  // $1B50
      /*$1B50*/ tmp5_U16 = sbc_dec16(s_a, 0x04, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
                block_id = 803;
      break;
    case 803:  // $1B52
      /*$1B54*/ tmp2_U8 = s_a >> 0x02;
      /*$1B57*/ poke((ram_peek16al(0x0008) + 0x0005), tmp2_U8);
      /*$1B59*/ s_y = 0x06;
      /*$1B5A*/ poke((ram_peek16al(0x0008) + 0x0006), tmp2_U8);
      /*$1B5C*/ CYCLES_EDGE(0x1b5c, 1);
                branchTarget = true; block_id = 808;
      break;
    case 804:  // $1B63
      /*$1B63*/ tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 + 0x0008) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x0008);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 806;
      break;
    case 805:  // $1B63
      /*$1B63*/ tmp4_U16 = adc_dec16(s_a, 0x08, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 806;
      break;
    case 806:  // $1B65
      /*$1B65*/ tmp2_U8 = s_a;
                ram_poke(0x0008, tmp2_U8);
      /*$1B69*/ branchTarget = true; block_id = !(tmp2_U8 >= 0x20) ? 807 : 808;
      break;
    case 807:  // $1B69
      /*$1B69*/ CYCLES_EDGE(0x1b69, 1);
                branchTarget = true; block_id = 786;
      break;
    case 808:  // $1B6B
      /*$1B6B*/ CYCLES(0x1b6b, 7);
                tmp2_U8 = ram_peek(0x008f);
      /*$1B6D*/ s_status_c = (tmp2_U8 >= 0x28);
      /*$1B6F*/ branchTarget = true; block_id = (tmp2_U8 != 0x28) ? 809 : 810;
      break;
    case 809:  // $1B6F
      /*$1B6F*/ CYCLES_EDGE(0x1b6f, 1);
      /*$1B75*/ CYCLES(0x1b75, 2);
                s_status_not_z = 0x28;
                s_status_n = 0x00;
                s_a = 0x28;
                block_id = 811;
      break;
    case 810:  // $1B71
      /*$1B71*/ CYCLES(0x1b71, 4);
                s_status_not_z = 0x27;
                s_status_n = 0x00;
                s_a = 0x27;
      /*$1B73*/ CYCLES_EDGE(0x1b73, 1);
                branchTarget = true; block_id = 811;
      break;
    case 811:  // $1B77
      /*$1B77*/ CYCLES(0x1b77, 9);
                ram_poke(0x008f, s_a);
      /*$1B79*/ branchTarget = true; block_id = find_block_id_func_t001(0x1b79, pop16() + 1);;
      break;
    case 812:  // $1B80
      /*$1B80*/ CYCLES(0x1b80, 10);
      /*$1B82*/ ram_poke(0x0008, 0x00);
      /*$1B86*/ ram_poke(0x0009, 0x03);
                block_id = 813;
      break;
    case 813:  // $1B88
      /*$1B88*/ CYCLES(0x1b88, 9);
                s_y = 0x00;
      /*$1B8A*/ tmp2_U8 = peek(ram_peek16al(0x0008));
                s_a = tmp2_U8;
      /*$1B8C*/ branchTarget = true; block_id = !tmp2_U8 ? 814 : 815;
      break;
    case 814:  // $1B8C
      /*$1B8C*/ CYCLES_EDGE(0x1b8c, 1);
                branchTarget = true; block_id = 818;
      break;
    case 815:  // $1B8E
      /*$1B8E*/ CYCLES(0x1b8e, 4);
      /*$1B8F*/ branchTarget = true; block_id = !(s_a >> 0x01) ? 816 : 817;
      break;
    case 816:  // $1B8F
      /*$1B8F*/ CYCLES_EDGE(0x1b8f, 1);
      /*$1B97*/ CYCLES(0x1b97, 61);
                tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$1B98*/ tmp2_U8 = peek((ram_peek16al(0x0008) + tmp2_U8));
                s_a = tmp2_U8;
      /*$1B9A*/ ram_poke(0x0004, tmp2_U8);
      /*$1B9C*/ s_status_c = 0x00;
      /*$1B9D*/ block_id = s_status_d ? 820 : 819;
      break;
    case 817:  // $1B91
      /*$1B91*/ CYCLES(0x1b91, 6);
                func_1640(0x1b93);
                branchTarget = true; block_id = 818;
      break;
    case 818:  // $1B94
      /*$1B94*/ CYCLES(0x1b94, 3);
                branchTarget = true; block_id = 865;
      break;
    case 819:  // $1B9D
      /*$1B9D*/ s_a = (uint8_t)((s_a + 0x000a) + s_status_c);
                block_id = 821;
      break;
    case 820:  // $1B9D
      /*$1B9D*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0a, s_status_c));
                block_id = 821;
      break;
    case 821:  // $1B9F
      /*$1B9F*/ ram_poke(0x0005, s_a);
      /*$1BA1*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$1BA2*/ tmp2_U8 = peek((ram_peek16al(0x0008) + tmp2_U8));
                s_a = tmp2_U8;
      /*$1BA4*/ ram_poke(0x0006, tmp2_U8);
      /*$1BA6*/ s_status_c = 0x00;
      /*$1BA7*/ block_id = s_status_d ? 823 : 822;
      break;
    case 822:  // $1BA7
      /*$1BA7*/ s_a = (uint8_t)((s_a + 0x0004) + s_status_c);
                block_id = 824;
      break;
    case 823:  // $1BA7
      /*$1BA7*/ s_a = ((uint8_t)adc_dec16(s_a, 0x04, s_status_c));
                block_id = 824;
      break;
    case 824:  // $1BA9
      /*$1BA9*/ ram_poke(0x0007, s_a);
      /*$1BAC*/ tmp2_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(s_y + 0x01)));
                s_a = tmp2_U8;
      /*$1BAE*/ s_status_c = 0x00;
      /*$1BAF*/ block_id = s_status_d ? 826 : 825;
      break;
    case 825:  // $1BAF
      /*$1BAF*/ s_a = (uint8_t)((s_a + 0x0067) + s_status_c);
                block_id = 827;
      break;
    case 826:  // $1BAF
      /*$1BAF*/ s_a = ((uint8_t)adc_dec16(s_a, 0x67, s_status_c));
                block_id = 827;
      break;
    case 827:  // $1BB1
      /*$1BB1*/ ram_poke(0x1544, s_a);
      /*$1BB6*/ ram_poke(0x1543, 0xd8);
      /*$1BB9*/ func_1530(0x1bbb);
      /*$1BBC*/ CYCLES(0x1bbc, 13);
                s_y = 0x05;
      /*$1BBE*/ tmp2_U8 = peek((ram_peek16al(0x0008) + 0x0005));
                s_a = tmp2_U8;
      /*$1BC0*/ s_status_c = 0x01;
      /*$1BC1*/ block_id = s_status_d ? 829 : 828;
      break;
    case 828:  // $1BC1
      /*$1BC1*/ tmp2_U8 = (uint8_t)((s_a - 0x0001) - (uint8_t)(0x01 - s_status_c));
                s_status_not_z = tmp2_U8;
                s_a = tmp2_U8;
                block_id = 830;
      break;
    case 829:  // $1BC1
      /*$1BC1*/ tmp4_U16 = sbc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_not_z = (~(uint8_t)(tmp4_U16 >> 8) & 2);
                block_id = 830;
      break;
    case 830:  // $1BC3
      /*$1BC3*/ branchTarget = true; block_id = s_status_not_z ? 831 : 832;
      break;
    case 831:  // $1BC3
      /*$1BC3*/ CYCLES_EDGE(0x1bc3, 1);
                branchTarget = true; block_id = 833;
      break;
    case 832:  // $1BC5
      /*$1BC5*/ CYCLES(0x1bc5, 24);
                tmp2_U8 = s_y;
      /*$1BC6*/ tmp1_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(tmp2_U8 + 0x01)));
      /*$1BC9*/ poke((ram_peek16al(0x0008) + tmp2_U8), tmp1_U8);
      /*$1BCB*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_y = tmp2_U8;
      /*$1BCC*/ tmp2_U8 = peek((ram_peek16al(0x0008) + tmp2_U8));
      /*$1BCE*/ s_a = (tmp2_U8 ^ 0x01);
                block_id = 833;
      break;
    case 833:  // $1BD0
      /*$1BD0*/ CYCLES(0x1bd0, 25);
                poke((ram_peek16al(0x0008) + s_y), s_a);
      /*$1BD2*/ s_y = 0x03;
      /*$1BD4*/ tmp2_U8 = peek((ram_peek16al(0x0008) + 0x0003));
                s_a = tmp2_U8;
      /*$1BD6*/ s_status_c = 0x01;
      /*$1BD7*/ block_id = s_status_d ? 835 : 834;
      break;
    case 834:  // $1BD7
      /*$1BD7*/ tmp2_U8 = (uint8_t)((s_a - ram_peek(0x1bd8)) - (uint8_t)(0x01 - s_status_c));
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 836;
      break;
    case 835:  // $1BD7
      /*$1BD7*/ tmp4_U16 = sbc_dec16(s_a, ram_peek(0x1bd8), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_n = ((uint8_t)(tmp4_U16 >> 8) & 0x80);
                block_id = 836;
      break;
    case 836:  // $1BD9
      /*$1BD9*/ poke((ram_peek16al(0x0008) + s_y), s_a);
      /*$1BDB*/ branchTarget = true; block_id = !s_status_n ? 837 : 838;
      break;
    case 837:  // $1BDB
      /*$1BDB*/ CYCLES_EDGE(0x1bdb, 1);
                branchTarget = true; block_id = 847;
      break;
    case 838:  // $1BDD
      /*$1BDD*/ CYCLES(0x1bdd, 23);
                s_status_c = 0x00;
      /*$1BDE*/ block_id = s_status_d ? 840 : 839;
      break;
    case 839:  // $1BDE
      /*$1BDE*/ s_a = (uint8_t)((s_a + 0x0007) + s_status_c);
                block_id = 841;
      break;
    case 840:  // $1BDE
      /*$1BDE*/ s_a = ((uint8_t)adc_dec16(s_a, 0x07, s_status_c));
                block_id = 841;
      break;
    case 841:  // $1BE0
      /*$1BE0*/ tmp2_U8 = s_y;
                poke((ram_peek16al(0x0008) + tmp2_U8), s_a);
      /*$1BE2*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_y = tmp2_U8;
      /*$1BE3*/ tmp2_U8 = peek((ram_peek16al(0x0008) + tmp2_U8));
                s_a = tmp2_U8;
      /*$1BE5*/ s_status_c = 0x01;
      /*$1BE6*/ block_id = s_status_d ? 843 : 842;
      break;
    case 842:  // $1BE6
      /*$1BE6*/ tmp2_U8 = (uint8_t)((s_a - 0x0001) - (uint8_t)(0x01 - s_status_c));
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 844;
      break;
    case 843:  // $1BE6
      /*$1BE6*/ tmp4_U16 = sbc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_n = ((uint8_t)(tmp4_U16 >> 8) & 0x80);
                block_id = 844;
      break;
    case 844:  // $1BE8
      /*$1BE8*/ branchTarget = true; block_id = !s_status_n ? 845 : 846;
      break;
    case 845:  // $1BE8
      /*$1BE8*/ CYCLES_EDGE(0x1be8, 1);
      /*$1BF4*/ CYCLES(0x1bf4, 6);
                poke((ram_peek16al(0x0008) + s_y), s_a);
                block_id = 847;
      break;
    case 846:  // $1BEA
      /*$1BEA*/ CYCLES(0x1bea, 13);
                s_y = 0x00;
      /*$1BEE*/ poke(ram_peek16al(0x0008), 0x00);
      /*$1BF0*/ branchTarget = true; block_id = 865;
      break;
    case 847:  // $1BF6
      /*$1BF6*/ CYCLES(0x1bf6, 9);
      /*$1BF8*/ tmp2_U8 = peek((ram_peek16al(0x0008) + 0x0004));
      /*$1BFA*/ branchTarget = true; block_id = tmp2_U8 ? 848 : 849;
      break;
    case 848:  // $1BFA
      /*$1BFA*/ CYCLES_EDGE(0x1bfa, 1);
      /*$1C08*/ CYCLES(0x1c08, 17);
                s_y = 0x01;
      /*$1C0A*/ tmp2_U8 = peek((ram_peek16al(0x0008) + 0x0001));
                s_a = tmp2_U8;
      /*$1C0C*/ s_status_c = 0x00;
      /*$1C0D*/ block_id = s_status_d ? 854 : 853;
      break;
    case 849:  // $1BFC
      /*$1BFC*/ CYCLES(0x1bfc, 20);
                s_y = 0x01;
      /*$1BFE*/ tmp2_U8 = peek((ram_peek16al(0x0008) + 0x0001));
                s_a = tmp2_U8;
      /*$1C00*/ s_status_c = 0x01;
      /*$1C01*/ block_id = s_status_d ? 851 : 850;
      break;
    case 850:  // $1C01
      /*$1C01*/ tmp4_U16 = s_a;
                tmp5_U16 = ram_peek(0x1c02);
                tmp6_U16 = (tmp4_U16 - tmp5_U16) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp6_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)(~tmp5_U16));
                tmp2_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 852;
      break;
    case 851:  // $1C01
      /*$1C01*/ tmp6_U16 = sbc_dec16(s_a, ram_peek(0x1c02), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp2_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 852;
      break;
    case 852:  // $1C03
      /*$1C03*/ poke((ram_peek16al(0x0008) + s_y), s_a);
      /*$1C05*/ branchTarget = true; block_id = find_block_id_func_t001(0x1c05, ram_peek16al(0x1c06));
      break;
    case 853:  // $1C0D
      /*$1C0D*/ s_a = (uint8_t)((s_a + ram_peek(0x1c0e)) + s_status_c);
                block_id = 855;
      break;
    case 854:  // $1C0D
      /*$1C0D*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x1c0e), s_status_c));
                block_id = 855;
      break;
    case 855:  // $1C0F
      /*$1C0F*/ poke((ram_peek16al(0x0008) + s_y), s_a);
      /*$1C11*/ CYCLES(0x1c11, 61);
                s_y = 0x01;
      /*$1C13*/ tmp2_U8 = peek((ram_peek16al(0x0008) + 0x0001));
                s_a = tmp2_U8;
      /*$1C15*/ ram_poke(0x0004, tmp2_U8);
      /*$1C17*/ s_status_c = 0x00;
      /*$1C18*/ block_id = s_status_d ? 857 : 856;
      break;
    case 856:  // $1C18
      /*$1C18*/ s_a = (uint8_t)((s_a + 0x000a) + s_status_c);
                block_id = 858;
      break;
    case 857:  // $1C18
      /*$1C18*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0a, s_status_c));
                block_id = 858;
      break;
    case 858:  // $1C1A
      /*$1C1A*/ ram_poke(0x0005, s_a);
      /*$1C1C*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$1C1D*/ tmp2_U8 = peek((ram_peek16al(0x0008) + tmp2_U8));
                s_a = tmp2_U8;
      /*$1C1F*/ ram_poke(0x0006, tmp2_U8);
      /*$1C21*/ s_status_c = 0x00;
      /*$1C22*/ block_id = s_status_d ? 860 : 859;
      break;
    case 859:  // $1C22
      /*$1C22*/ s_a = (uint8_t)((s_a + 0x0004) + s_status_c);
                block_id = 861;
      break;
    case 860:  // $1C22
      /*$1C22*/ s_a = ((uint8_t)adc_dec16(s_a, 0x04, s_status_c));
                block_id = 861;
      break;
    case 861:  // $1C24
      /*$1C24*/ ram_poke(0x0007, s_a);
      /*$1C26*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$1C27*/ tmp2_U8 = peek((ram_peek16al(0x0008) + tmp2_U8));
                s_a = tmp2_U8;
      /*$1C29*/ s_status_c = 0x00;
      /*$1C2A*/ block_id = s_status_d ? 863 : 862;
      break;
    case 862:  // $1C2A
      /*$1C2A*/ s_a = (uint8_t)((s_a + 0x0067) + s_status_c);
                block_id = 864;
      break;
    case 863:  // $1C2A
      /*$1C2A*/ s_a = ((uint8_t)adc_dec16(s_a, 0x67, s_status_c));
                block_id = 864;
      break;
    case 864:  // $1C2C
      /*$1C2C*/ ram_poke(0x1514, s_a);
      /*$1C31*/ ram_poke(0x1513, 0xd8);
      /*$1C34*/ func_1500(0x1c36);
      /*$1C37*/ CYCLES(0x1c37, 2);
                block_id = 865;
      break;
    case 865:  // $1C38
      /*$1C38*/ CYCLES(0x1c38, 14);
                s_a = ram_peek(0x0008);
      /*$1C3A*/ s_status_c = 0x00;
      /*$1C3B*/ block_id = s_status_d ? 867 : 866;
      break;
    case 866:  // $1C3B
      /*$1C3B*/ tmp6_U16 = s_a;
                tmp5_U16 = (tmp6_U16 + 0x0008) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp6_U16, (uint8_t)0x0008);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 868;
      break;
    case 867:  // $1C3B
      /*$1C3B*/ tmp5_U16 = adc_dec16(s_a, 0x08, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
                block_id = 868;
      break;
    case 868:  // $1C3D
      /*$1C3D*/ tmp2_U8 = s_a;
                ram_poke(0x0008, tmp2_U8);
      /*$1C3F*/ s_status_not_z = (tmp2_U8 != 0x20);
                tmp1_U8 = tmp2_U8 >= 0x20;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - 0x20) & 0x80);
      /*$1C41*/ branchTarget = true; block_id = tmp1_U8 ? 869 : 870;
      break;
    case 869:  // $1C41
      /*$1C41*/ CYCLES_EDGE(0x1c41, 1);
      /*$1C46*/ CYCLES(0x1c46, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x1c46, pop16() + 1);;
      break;
    case 870:  // $1C43
      /*$1C43*/ CYCLES(0x1c43, 3);
                branchTarget = true; block_id = 813;
      break;
    case 871:  // $1C58
      /*$1C58*/ CYCLES(0x1c58, 11);
                s_y = 0x00;
      /*$1C5A*/ tmp1_U8 = peek(ram_peek16al(0x0008));
      /*$1C5D*/ branchTarget = true; block_id = !(tmp1_U8 & 0x01) ? 872 : 873;
      break;
    case 872:  // $1C5D
      /*$1C5D*/ CYCLES_EDGE(0x1c5d, 1);
                branchTarget = true; block_id = 897;
      break;
    case 873:  // $1C5F
      /*$1C5F*/ CYCLES(0x1c5f, 12);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$1C60*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$1C64*/ branchTarget = true; block_id = (tmp1_U8 >= ram_peek(0x0005)) ? 874 : 875;
      break;
    case 874:  // $1C64
      /*$1C64*/ CYCLES_EDGE(0x1c64, 1);
                branchTarget = true; block_id = 897;
      break;
    case 875:  // $1C66
      /*$1C66*/ CYCLES(0x1c66, 9);
                s_status_c = 0x00;
      /*$1C67*/ block_id = s_status_d ? 877 : 876;
      break;
    case 876:  // $1C67
      /*$1C67*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
                block_id = 878;
      break;
    case 877:  // $1C67
      /*$1C67*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
                block_id = 878;
      break;
    case 878:  // $1C69
      /*$1C6B*/ branchTarget = true; block_id = !(s_a >= ram_peek(0x0005)) ? 879 : 880;
      break;
    case 879:  // $1C6B
      /*$1C6B*/ CYCLES_EDGE(0x1c6b, 1);
                branchTarget = true; block_id = 897;
      break;
    case 880:  // $1C6D
      /*$1C6D*/ CYCLES(0x1c6d, 12);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$1C6E*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$1C72*/ branchTarget = true; block_id = (tmp1_U8 >= ram_peek(0x0007)) ? 881 : 882;
      break;
    case 881:  // $1C72
      /*$1C72*/ CYCLES_EDGE(0x1c72, 1);
                branchTarget = true; block_id = 897;
      break;
    case 882:  // $1C74
      /*$1C74*/ CYCLES(0x1c74, 9);
                s_status_c = 0x00;
      /*$1C75*/ block_id = s_status_d ? 884 : 883;
      break;
    case 883:  // $1C75
      /*$1C75*/ s_a = (uint8_t)((s_a + 0x0003) + s_status_c);
                block_id = 885;
      break;
    case 884:  // $1C75
      /*$1C75*/ s_a = ((uint8_t)adc_dec16(s_a, 0x03, s_status_c));
                block_id = 885;
      break;
    case 885:  // $1C77
      /*$1C79*/ branchTarget = true; block_id = !(s_a >= ram_peek(0x0007)) ? 886 : 887;
      break;
    case 886:  // $1C79
      /*$1C79*/ CYCLES_EDGE(0x1c79, 1);
                branchTarget = true; block_id = 897;
      break;
    case 887:  // $1C7B
      /*$1C7B*/ CYCLES(0x1c7b, 68);
      /*$1C7F*/ poke(ram_peek16al(0x0008), 0x02);
      /*$1C81*/ s_y = 0x01;
      /*$1C82*/ tmp1_U8 = peek((ram_peek16al(0x0008) + 0x0001));
                s_a = tmp1_U8;
      /*$1C84*/ ram_poke(0x0004, tmp1_U8);
      /*$1C86*/ s_status_c = 0x00;
      /*$1C87*/ block_id = s_status_d ? 889 : 888;
      break;
    case 888:  // $1C87
      /*$1C87*/ s_a = (uint8_t)((s_a + 0x000a) + s_status_c);
                block_id = 890;
      break;
    case 889:  // $1C87
      /*$1C87*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0a, s_status_c));
                block_id = 890;
      break;
    case 890:  // $1C89
      /*$1C89*/ ram_poke(0x0005, s_a);
      /*$1C8B*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$1C8C*/ tmp1_U8 = peek((ram_peek16al(0x0008) + tmp1_U8));
                s_a = tmp1_U8;
      /*$1C8E*/ ram_poke(0x0006, tmp1_U8);
      /*$1C90*/ s_status_c = 0x00;
      /*$1C91*/ block_id = s_status_d ? 892 : 891;
      break;
    case 891:  // $1C91
      /*$1C91*/ s_a = (uint8_t)((s_a + 0x0004) + s_status_c);
                block_id = 893;
      break;
    case 892:  // $1C91
      /*$1C91*/ s_a = ((uint8_t)adc_dec16(s_a, 0x04, s_status_c));
                block_id = 893;
      break;
    case 893:  // $1C93
      /*$1C93*/ ram_poke(0x0007, s_a);
      /*$1C96*/ tmp1_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(s_y + 0x01)));
                s_a = tmp1_U8;
      /*$1C98*/ s_status_c = 0x00;
      /*$1C99*/ block_id = s_status_d ? 895 : 894;
      break;
    case 894:  // $1C99
      /*$1C99*/ s_a = (uint8_t)((s_a + 0x0067) + s_status_c);
                block_id = 896;
      break;
    case 895:  // $1C99
      /*$1C99*/ s_a = ((uint8_t)adc_dec16(s_a, 0x67, s_status_c));
                block_id = 896;
      break;
    case 896:  // $1C9B
      /*$1C9B*/ ram_poke(0x1544, s_a);
      /*$1CA0*/ ram_poke(0x1543, 0xd8);
      /*$1CA3*/ branchTarget = true; block_id = 784;
      break;
    case 897:  // $1CA9
      /*$1CA9*/ CYCLES(0x1ca9, 14);
                s_a = ram_peek(0x0008);
      /*$1CAB*/ s_status_c = 0x00;
      /*$1CAC*/ block_id = s_status_d ? 899 : 898;
      break;
    case 898:  // $1CAC
      /*$1CAC*/ tmp5_U16 = s_a;
                tmp6_U16 = (tmp5_U16 + 0x0008) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp5_U16, (uint8_t)0x0008);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 900;
      break;
    case 899:  // $1CAC
      /*$1CAC*/ tmp6_U16 = adc_dec16(s_a, 0x08, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 900;
      break;
    case 900:  // $1CAE
      /*$1CAE*/ tmp1_U8 = s_a;
                ram_poke(0x0008, tmp1_U8);
      /*$1CB0*/ s_status_not_z = (tmp1_U8 != 0x20);
                tmp2_U8 = tmp1_U8 >= 0x20;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - 0x20) & 0x80);
      /*$1CB2*/ branchTarget = true; block_id = !tmp2_U8 ? 901 : 902;
      break;
    case 901:  // $1CB2
      /*$1CB2*/ CYCLES_EDGE(0x1cb2, 1);
                branchTarget = true; block_id = 871;
      break;
    case 902:  // $1CB4
      /*$1CB4*/ CYCLES(0x1cb4, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x1cb4, pop16() + 1);;
      break;
    case 903:  // $1D00
      /*$1D00*/ CYCLES(0x1d00, 18);
                s_a = ram_peek(0x00e2);
      /*$1D02*/ s_status_c = 0x00;
      /*$1D03*/ block_id = s_status_d ? 905 : 904;
      break;
    case 904:  // $1D03
      /*$1D03*/ s_a = (uint8_t)((s_a + 0x0060) + s_status_c);
                block_id = 906;
      break;
    case 905:  // $1D03
      /*$1D03*/ s_a = ((uint8_t)adc_dec16(s_a, 0x60, s_status_c));
                block_id = 906;
      break;
    case 906:  // $1D05
      /*$1D05*/ ram_poke(0x1040, s_a);
      /*$1D08*/ s_a = 0x00;
      /*$1D0A*/ tmp2_U8 = ram_peek(0x00e5);
                s_x = tmp2_U8;
      /*$1D0C*/ branchTarget = true; block_id = !tmp2_U8 ? 907 : 908;
      break;
    case 907:  // $1D0C
      /*$1D0C*/ CYCLES_EDGE(0x1d0c, 1);
                branchTarget = true; block_id = 916;
      break;
    case 908:  // $1D0E
      /*$1D0E*/ CYCLES(0x1d0e, 8);
                s_status_c = 0x00;
      /*$1D0F*/ block_id = s_status_d ? 910 : 909;
      break;
    case 909:  // $1D0F
      /*$1D0F*/ s_a = (uint8_t)((s_a + 0x0050) + s_status_c);
                block_id = 911;
      break;
    case 910:  // $1D0F
      /*$1D0F*/ s_a = ((uint8_t)adc_dec16(s_a, 0x50, s_status_c));
                block_id = 911;
      break;
    case 911:  // $1D11
      /*$1D11*/ tmp2_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp2_U8;
      /*$1D12*/ branchTarget = true; block_id = !tmp2_U8 ? 912 : 913;
      break;
    case 912:  // $1D12
      /*$1D12*/ CYCLES_EDGE(0x1d12, 1);
                branchTarget = true; block_id = 916;
      break;
    case 913:  // $1D14
      /*$1D14*/ CYCLES(0x1d14, 4);
                s_status_c = 0x00;
      /*$1D15*/ block_id = s_status_d ? 915 : 914;
      break;
    case 914:  // $1D15
      /*$1D15*/ s_a = (uint8_t)((s_a + 0x0050) + s_status_c);
                block_id = 917;
      break;
    case 915:  // $1D15
      /*$1D15*/ s_a = ((uint8_t)adc_dec16(s_a, 0x50, s_status_c));
                block_id = 917;
      break;
    case 916:  // $1D17
      /*$1D17*/ CYCLES(0x1d17, 36);
                ram_poke(0x103f, s_a);
      /*$1D1A*/ tmp2_U8 = ram_peek(0x00e0);
                s_a = tmp2_U8;
      /*$1D1C*/ ram_poke(0x0004, tmp2_U8);
      /*$1D1E*/ s_status_c = 0x00;
      /*$1D1F*/ block_id = s_status_d ? 919 : 918;
      break;
    case 917:  // $1D17
      /*$1D15*/ block_id = 916;
      break;
    case 918:  // $1D1F
      /*$1D1F*/ s_a = (uint8_t)((s_a + 0x000b) + s_status_c);
                block_id = 920;
      break;
    case 919:  // $1D1F
      /*$1D1F*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0b, s_status_c));
                block_id = 920;
      break;
    case 920:  // $1D21
      /*$1D21*/ ram_poke(0x0005, s_a);
      /*$1D23*/ tmp2_U8 = ram_peek(0x00e1);
                s_a = tmp2_U8;
      /*$1D25*/ ram_poke(0x0006, tmp2_U8);
      /*$1D27*/ s_status_c = 0x00;
      /*$1D28*/ block_id = s_status_d ? 922 : 921;
      break;
    case 921:  // $1D28
      /*$1D28*/ s_a = (uint8_t)((s_a + 0x0007) + s_status_c);
                block_id = 923;
      break;
    case 922:  // $1D28
      /*$1D28*/ s_a = ((uint8_t)adc_dec16(s_a, 0x07, s_status_c));
                block_id = 923;
      break;
    case 923:  // $1D2A
      /*$1D2A*/ ram_poke(0x0007, s_a);
      /*$1D2C*/ func_1030(0x1d2e);
                branchTarget = true; block_id = 924;
      break;
    case 924:  // $1D2F
      /*$1D2F*/ CYCLES(0x1d2f, 6);
                func_1d80(0x1d31);
                branchTarget = true; block_id = 925;
      break;
    case 925:  // $1D32
      /*$1D32*/ CYCLES(0x1d32, 8);
                tmp2_U8 = ram_peek(0x0c00);
      /*$1D35*/ s_status_c = (tmp2_U8 >= 0x40);
      /*$1D37*/ branchTarget = true; block_id = !(tmp2_U8 != 0x40) ? 926 : 927;
      break;
    case 926:  // $1D37
      /*$1D37*/ CYCLES_EDGE(0x1d37, 1);
                branchTarget = true; block_id = 928;
      break;
    case 927:  // $1D39
      /*$1D39*/ CYCLES(0x1d39, 6);
                func_2f00(0x1d3b);
      /*$1D3C*/ CYCLES(0x1d3c, 6);
                func_2f80(0x1d3e);
                branchTarget = true; block_id = 928;
      break;
    case 928:  // $1D3F
      /*$1D3F*/ CYCLES(0x1d3f, 3);
      /*$AC30*/ CYCLES(0xac30, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x0087) - 0x01);
                ram_poke(0x0087, tmp2_U8);
      /*$AC32*/ branchTarget = true; block_id = (tmp2_U8 & 0x80) ? 1749 : 1750;
      break;
    case 929:  // $1D80
      /*$1D80*/ CYCLES(0x1d80, 39);
                s_a = ram_peek(0x00e0);
      /*$1D82*/ s_status_c = 0x01;
      /*$1D83*/ block_id = s_status_d ? 931 : 930;
      break;
    case 930:  // $1D83
      /*$1D83*/ s_a = (uint8_t)((s_a - 0x000e) - (uint8_t)(0x01 - s_status_c));
                block_id = 932;
      break;
    case 931:  // $1D83
      /*$1D83*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x0e, s_status_c));
                block_id = 932;
      break;
    case 932:  // $1D85
      /*$1D85*/ ram_poke(0x0004, s_a);
      /*$1D87*/ s_status_c = 0x00;
      /*$1D88*/ block_id = s_status_d ? 934 : 933;
      break;
    case 933:  // $1D88
      /*$1D88*/ s_a = (uint8_t)((s_a + 0x002a) + s_status_c);
                block_id = 935;
      break;
    case 934:  // $1D88
      /*$1D88*/ s_a = ((uint8_t)adc_dec16(s_a, 0x2a, s_status_c));
                block_id = 935;
      break;
    case 935:  // $1D8A
      /*$1D8A*/ ram_poke(0x0005, s_a);
      /*$1D8C*/ s_a = ram_peek(0x00e1);
      /*$1D8E*/ s_status_c = 0x01;
      /*$1D8F*/ block_id = s_status_d ? 937 : 936;
      break;
    case 936:  // $1D8F
      /*$1D8F*/ s_a = (uint8_t)((s_a - 0x0003) - (uint8_t)(0x01 - s_status_c));
                block_id = 938;
      break;
    case 937:  // $1D8F
      /*$1D8F*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x03, s_status_c));
                block_id = 938;
      break;
    case 938:  // $1D91
      /*$1D91*/ tmp2_U8 = s_a & 0xfe;
                s_a = tmp2_U8;
      /*$1D93*/ ram_poke(0x0006, tmp2_U8);
      /*$1D95*/ s_status_c = 0x00;
      /*$1D96*/ block_id = s_status_d ? 940 : 939;
      break;
    case 939:  // $1D96
      /*$1D96*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
                block_id = 941;
      break;
    case 940:  // $1D96
      /*$1D96*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
                block_id = 941;
      break;
    case 941:  // $1D98
      /*$1D98*/ ram_poke(0x0007, s_a);
      /*$1EB0*/ CYCLES(0x1eb0, 10);
      /*$1EB2*/ ram_poke(0x0009, 0x00);
      /*$1EB6*/ ram_poke(0x000a, 0x01);
                block_id = 986;
      break;
    case 942:  // $1DA0
      /*$1DA0*/ CYCLES(0x1da0, 13);
      /*$1DA2*/ ram_poke(0x0000, 0x10);
      /*$1DA4*/ s_a = 0x00;
      /*$1DA6*/ ram_poke(0x0001, 0x00);
      /*$1DA8*/ func_1dd0(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x1da8, pop16() + 1);;
      break;
    case 943:  // $1DB0
      /*$1DB0*/ CYCLES(0x1db0, 13);
      /*$1DB2*/ ram_poke(0x0000, 0x20);
      /*$1DB4*/ s_a = 0x60;
      /*$1DB6*/ ram_poke(0x0001, 0x60);
      /*$1DB8*/ func_1dd0(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x1db8, pop16() + 1);;
      break;
    case 944:  // $1DC0
      /*$1DC0*/ CYCLES(0x1dc0, 13);
      /*$1DC2*/ ram_poke(0x0000, 0x10);
      /*$1DC4*/ s_a = 0x50;
      /*$1DC6*/ ram_poke(0x0001, 0x50);
      /*$1DC8*/ func_1dd0(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x1dc8, pop16() + 1);;
      break;
    case 945:  // $1DD0
      /*$1DD0*/ CYCLES(0x1dd0, 3);
                s_x = ram_peek(0x0000);
                block_id = 946;
      break;
    case 946:  // $1DD2
      /*$1DD2*/ CYCLES(0x1dd2, 3);
                s_y = ram_peek(0x0001);
                block_id = 947;
      break;
    case 947:  // $1DD4
      /*$1DD4*/ CYCLES(0x1dd4, 10);
      /*$1DD7*/ tmp2_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp2_U8;
      /*$1DD8*/ branchTarget = true; block_id = tmp2_U8 ? 948 : 949;
      break;
    case 948:  // $1DD8
      /*$1DD8*/ CYCLES_EDGE(0x1dd8, 1);
                branchTarget = true; block_id = 947;
      break;
    case 949:  // $1DDA
      /*$1DDA*/ CYCLES(0x1dda, 8);
                poke(ram_peek16(0x1ddb), s_a);
      /*$1DDD*/ tmp2_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$1DDE*/ branchTarget = true; block_id = tmp2_U8 ? 950 : 951;
      break;
    case 950:  // $1DDE
      /*$1DDE*/ CYCLES_EDGE(0x1dde, 1);
                branchTarget = true; block_id = 946;
      break;
    case 951:  // $1DE0
      /*$1DE0*/ CYCLES(0x1de0, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x1de0, pop16() + 1);;
      break;
    case 952:  // $1E00
      /*$1E00*/ CYCLES(0x1e00, 14);
      /*$1E02*/ ram_poke(0x1e19, 0x00);
      /*$1E07*/ ram_poke(0x1e1a, 0x9b);
      /*$1E0A*/ s_x = 0xa8;
                block_id = 953;
      break;
    case 953:  // $1E0C
      /*$1E0C*/ CYCLES(0x1e0c, 16);
                tmp2_U8 = s_x;
      /*$1E0F*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp2_U8)));
      /*$1E14*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp2_U8)));
      /*$1E16*/ s_y = 0x00;
                block_id = 954;
      break;
    case 954:  // $1E18
      /*$1E18*/ CYCLES(0x1e18, 18);
                tmp2_U8 = peek(ram_peek16(0x1e19));
      /*$1E1B*/ poke((ram_peek16al(0x0000) + s_y), tmp2_U8);
      /*$1E1D*/ tmp2_U8 = (uint8_t)(ram_peek(0x1e19) + 0x01);
                ram_poke(0x1e19, tmp2_U8);
      /*$1E20*/ branchTarget = true; block_id = tmp2_U8 ? 955 : 956;
      break;
    case 955:  // $1E20
      /*$1E20*/ CYCLES_EDGE(0x1e20, 1);
                branchTarget = true; block_id = 957;
      break;
    case 956:  // $1E22
      /*$1E22*/ CYCLES(0x1e22, 6);
                ram_poke(0x1e1a, (uint8_t)(ram_peek(0x1e1a) + 0x01));
                block_id = 957;
      break;
    case 957:  // $1E25
      /*$1E25*/ CYCLES(0x1e25, 6);
                tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$1E28*/ branchTarget = true; block_id = !(tmp2_U8 >= 0x28) ? 958 : 959;
      break;
    case 958:  // $1E28
      /*$1E28*/ CYCLES_EDGE(0x1e28, 1);
                branchTarget = true; block_id = 954;
      break;
    case 959:  // $1E2A
      /*$1E2A*/ CYCLES(0x1e2a, 6);
                tmp2_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp2_U8;
      /*$1E2D*/ branchTarget = true; block_id = !(tmp2_U8 >= 0xc0) ? 960 : 961;
      break;
    case 960:  // $1E2D
      /*$1E2D*/ CYCLES_EDGE(0x1e2d, 1);
                branchTarget = true; block_id = 953;
      break;
    case 961:  // $1E2F
      /*$1E2F*/ CYCLES(0x1e2f, 6);
                func_2c40(0x1e31);
                branchTarget = true; block_id = 962;
      break;
    case 962:  // $1E32
      /*$1E32*/ CYCLES(0x1e32, 3);
      /*$2DC0*/ CYCLES(0x2dc0, 6);
                func_2d00(0x2dc2);
                branchTarget = true; block_id = 1432;
      break;
    case 963:  // $1E40
      /*$1E40*/ CYCLES(0x1e40, 3);
                s_x = ram_peek(0x0004);
                block_id = 964;
      break;
    case 964:  // $1E42
      /*$1E42*/ CYCLES(0x1e42, 17);
                tmp2_U8 = s_x;
      /*$1E45*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp2_U8)));
      /*$1E47*/ tmp2_U8 = ram_peek((0x0c00 + tmp2_U8));
                s_a = tmp2_U8;
      /*$1E4A*/ ram_poke(0x0001, tmp2_U8);
      /*$1E4C*/ s_y = ram_peek(0x0006);
                block_id = 965;
      break;
    case 965:  // $1E4E
      /*$1E4E*/ CYCLES(0x1e4e, 4);
      /*$1E50*/ branchTarget = true; block_id = (s_y >= 0x28) ? 966 : 967;
      break;
    case 966:  // $1E50
      /*$1E50*/ CYCLES_EDGE(0x1e50, 1);
                branchTarget = true; block_id = 968;
      break;
    case 967:  // $1E52
      /*$1E52*/ CYCLES(0x1e52, 4);
                tmp2_U8 = peek(ram_peek16(0x1e53));
      /*$1E55*/ CYCLES(0x1e55, 11);
                tmp1_U8 = s_y;
                tmp3_U8 = peek((ram_peek16al(0x0000) + tmp1_U8));
                tmp3_U8 = tmp2_U8 ^ tmp3_U8;
                s_a = tmp3_U8;
      /*$1E57*/ poke((ram_peek16al(0x0000) + tmp1_U8), tmp3_U8);
                block_id = 968;
      break;
    case 968:  // $1E59
      /*$1E59*/ CYCLES(0x1e59, 8);
                tmp2_U8 = (uint8_t)(ram_peek(0x1e53) + 0x01);
                ram_poke(0x1e53, tmp2_U8);
      /*$1E5C*/ branchTarget = true; block_id = tmp2_U8 ? 969 : 970;
      break;
    case 969:  // $1E5C
      /*$1E5C*/ CYCLES_EDGE(0x1e5c, 1);
                branchTarget = true; block_id = 971;
      break;
    case 970:  // $1E5E
      /*$1E5E*/ CYCLES(0x1e5e, 6);
                ram_poke(0x1e54, (uint8_t)(ram_peek(0x1e54) + 0x01));
                block_id = 971;
      break;
    case 971:  // $1E61
      /*$1E61*/ CYCLES(0x1e61, 7);
                tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$1E64*/ branchTarget = true; block_id = (tmp1_U8 != ram_peek(0x0007)) ? 972 : 973;
      break;
    case 972:  // $1E64
      /*$1E64*/ CYCLES_EDGE(0x1e64, 1);
                branchTarget = true; block_id = 965;
      break;
    case 973:  // $1E66
      /*$1E66*/ CYCLES(0x1e66, 7);
                tmp1_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp1_U8;
      /*$1E67*/ tmp3_U8 = ram_peek(0x0005);
                tmp2_U8 = tmp1_U8 != tmp3_U8;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp1_U8 >= tmp3_U8);
                s_status_n = ((uint8_t)(tmp1_U8 - tmp3_U8) & 0x80);
      /*$1E69*/ branchTarget = true; block_id = tmp2_U8 ? 974 : 975;
      break;
    case 974:  // $1E69
      /*$1E69*/ CYCLES_EDGE(0x1e69, 1);
                branchTarget = true; block_id = 964;
      break;
    case 975:  // $1E6B
      /*$1E6B*/ CYCLES(0x1e6b, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x1e6b, pop16() + 1);;
      break;
    case 976:  // $1E80
      /*$1E80*/ CYCLES(0x1e80, 31);
      /*$1E82*/ s_a = (uint8_t)(ram_peek(0x0008) << 0x01);
      /*$1E83*/ s_status_c = 0x00;
      /*$1E84*/ block_id = s_status_d ? 978 : 977;
      break;
    case 977:  // $1E84
      /*$1E84*/ s_a = (uint8_t)((s_a + 0x00a0) + s_status_c);
                block_id = 979;
      break;
    case 978:  // $1E84
      /*$1E84*/ s_a = ((uint8_t)adc_dec16(s_a, 0xa0, s_status_c));
                block_id = 979;
      break;
    case 979:  // $1E86
      /*$1E86*/ ram_poke(0x1e54, s_a);
      /*$1E8B*/ ram_poke(0x1e53, 0x00);
      /*$1E90*/ ram_poke(0x1e55, 0x11);
      /*$1E93*/ func_1e40(0x1e95);
                branchTarget = true; block_id = 980;
      break;
    case 980:  // $1E96
      /*$1E96*/ CYCLES(0x1e96, 6);
                func_1da0(0x1e98);
                branchTarget = true; block_id = 981;
      break;
    case 981:  // $1E99
      /*$1E99*/ CYCLES(0x1e99, 31);
      /*$1E9B*/ s_a = (uint8_t)(ram_peek(0x0008) << 0x01);
      /*$1E9C*/ s_status_c = 0x00;
      /*$1E9D*/ block_id = s_status_d ? 983 : 982;
      break;
    case 982:  // $1E9D
      /*$1E9D*/ tmp6_U16 = s_a;
                tmp5_U16 = (tmp6_U16 + 0x00a0) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp6_U16, (uint8_t)0x00a0);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 984;
      break;
    case 983:  // $1E9D
      /*$1E9D*/ tmp5_U16 = adc_dec16(s_a, 0xa0, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
                block_id = 984;
      break;
    case 984:  // $1E9F
      /*$1E9F*/ ram_poke(0x1e54, s_a);
      /*$1EA4*/ ram_poke(0x1e53, 0x00);
      /*$1EA7*/ s_a = 0x51;
      /*$1EA9*/ ram_poke(0x1e55, 0x51);
      /*$1EAC*/ func_1e40(0x1eae);
                branchTarget = true; block_id = 985;
      break;
    case 985:  // $1EAF
      /*$1EAF*/ CYCLES(0x1eaf, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x1eaf, pop16() + 1);;
      break;
    case 986:  // $1EB8
      /*$1EB8*/ CYCLES(0x1eb8, 12);
      /*$1EBA*/ ram_poke(0x0008, ram_peek(0x0009));
      /*$1EBC*/ func_1e80(0x1ebe);
                branchTarget = true; block_id = 987;
      break;
    case 987:  // $1EBF
      /*$1EBF*/ CYCLES(0x1ebf, 6);
                func_1db0(0x1ec1);
                branchTarget = true; block_id = 988;
      break;
    case 988:  // $1EC2
      /*$1EC2*/ CYCLES(0x1ec2, 17);
                ram_poke(0x0009, (uint8_t)(ram_peek(0x0009) + 0x01));
      /*$1EC6*/ ram_poke(0x0008, ram_peek(0x000a));
      /*$1EC8*/ func_1e80(0x1eca);
                branchTarget = true; block_id = 989;
      break;
    case 989:  // $1ECB
      /*$1ECB*/ CYCLES(0x1ecb, 6);
                func_1dc0(0x1ecd);
                branchTarget = true; block_id = 990;
      break;
    case 990:  // $1ECE
      /*$1ECE*/ CYCLES(0x1ece, 12);
                ram_poke(0x000a, (uint8_t)(ram_peek(0x000a) + 0x01));
      /*$1ED2*/ tmp2_U8 = ram_peek(0x000a) >= 0x06;
                s_status_c = tmp2_U8;
      /*$1ED4*/ branchTarget = true; block_id = !tmp2_U8 ? 991 : 992;
      break;
    case 991:  // $1ED4
      /*$1ED4*/ CYCLES_EDGE(0x1ed4, 1);
                branchTarget = true; block_id = 986;
      break;
    case 992:  // $1ED6
      /*$1ED6*/ CYCLES(0x1ed6, 6);
                func_1ef3(0x1ed8);
                branchTarget = true; block_id = 993;
      break;
    case 993:  // $1ED9
      /*$1ED9*/ CYCLES(0x1ed9, 7);
      /*$1EDA*/ s_a = 0x07;
      /*$1EDC*/ ram_poke(0x0001, 0x07);
                block_id = 994;
      break;
    case 994:  // $1EDE
      /*$1EDE*/ CYCLES(0x1ede, 6);
                func_1dd0(0x1ee0);
                branchTarget = true; block_id = 995;
      break;
    case 995:  // $1EE1
      /*$1EE1*/ CYCLES(0x1ee1, 24);
                s_a = ram_peek(0x0001);
      /*$1EE3*/ s_status_c = 0x00;
      /*$1EE4*/ block_id = s_status_d ? 997 : 996;
      break;
    case 996:  // $1EE4
      /*$1EE4*/ s_a = (uint8_t)((s_a + 0x0008) + s_status_c);
                block_id = 998;
      break;
    case 997:  // $1EE4
      /*$1EE4*/ s_a = ((uint8_t)adc_dec16(s_a, 0x08, s_status_c));
                block_id = 998;
      break;
    case 998:  // $1EE6
      /*$1EE7*/ ram_poke(0x0001, s_a);
      /*$1EE9*/ s_a = ram_peek(0x0000);
      /*$1EEB*/ s_status_c = 0x00;
      /*$1EEC*/ block_id = s_status_d ? 1000 : 999;
      break;
    case 999:  // $1EEC
      /*$1EEC*/ tmp5_U16 = s_a;
                tmp6_U16 = (tmp5_U16 + 0x0004) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp5_U16, (uint8_t)0x0004);
                tmp2_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 1001;
      break;
    case 1000:  // $1EEC
      /*$1EEC*/ tmp6_U16 = adc_dec16(s_a, 0x04, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp2_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 1001;
      break;
    case 1001:  // $1EEE
      /*$1EEE*/ ram_poke(0x0000, s_a);
      /*$1EF0*/ branchTarget = true; block_id = !s_status_n ? 1002 : 1003;
      break;
    case 1002:  // $1EF0
      /*$1EF0*/ CYCLES_EDGE(0x1ef0, 1);
                branchTarget = true; block_id = 994;
      break;
    case 1003:  // $1EF2
      /*$1EF2*/ CYCLES(0x1ef2, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x1ef2, pop16() + 1);;
      break;
    case 1004:  // $1EF3
      /*$1EF3*/ CYCLES(0x1ef3, 8);
                s_a = 0x00;
      /*$1EF5*/ rom_mon_wait(0x1ef7);
      /*$1EF8*/ CYCLES(0x1ef8, 11);
                s_status_not_z = 0x40;
                s_status_n = 0x00;
                s_a = 0x40;
      /*$1EFA*/ ram_poke(0x0000, 0x40);
      /*$1EFC*/ branchTarget = true; block_id = find_block_id_func_t001(0x1efc, pop16() + 1);;
      break;
    case 1005:  // $1F00
      /*$1F00*/ CYCLES(0x1f00, 20);
      /*$1F02*/ ram_poke(0x00d0, 0x00);
      /*$1F04*/ ram_poke(0x00c0, 0x00);
      /*$1F06*/ ram_poke(0x00c4, 0x00);
      /*$1F08*/ ram_poke(0x00c8, 0x00);
      /*$1F0A*/ func_1fd0(0x1f0c);
                branchTarget = true; block_id = 1006;
      break;
    case 1006:  // $1F0D
      /*$1F0D*/ CYCLES(0x1f0d, 67);
      /*$1F10*/ ram_poke(0x00e1, 0x00);
      /*$1F12*/ ram_poke(0x00e2, 0x00);
      /*$1F14*/ ram_poke(0x00e3, 0x00);
      /*$1F16*/ ram_poke(0x00e4, 0x00);
      /*$1F18*/ ram_poke(0x00e5, 0x00);
      /*$1F1A*/ ram_poke(0x00e6, 0x00);
      /*$1F1C*/ ram_poke(0x00e7, 0x00);
      /*$1F1E*/ ram_poke(0x00f0, 0x00);
      /*$1F20*/ ram_poke(0x00f2, 0x00);
      /*$1F24*/ ram_poke(0x00f1, 0x80);
      /*$1F28*/ ram_poke(0x00f3, 0x86);
      /*$1F2C*/ ram_poke(0x00f4, 0xa6);
      /*$1F30*/ ram_poke(0x00f5, 0x00);
      /*$1F34*/ ram_poke(0x0004, 0x00);
      /*$1F36*/ s_a = 0xc0;
      /*$1F38*/ ram_poke(0x0005, 0xc0);
      /*$1F3A*/ func_2500(0x1f3c);
                branchTarget = true; block_id = 1007;
      break;
    case 1007:  // $1F3D
      /*$1F3D*/ CYCLES(0x1f3d, 24);
      /*$1F3E*/ tmp2_U8 = s_a;
                io_poke(0xc055, tmp2_U8);
      /*$1F41*/ io_poke(0xc050, tmp2_U8);
      /*$1F44*/ io_poke(0xc052, tmp2_U8);
      /*$1F47*/ io_poke(0xc057, tmp2_U8);
      /*$1F4A*/ func_1e00(0x1f4c);
                branchTarget = true; block_id = 1008;
      break;
    case 1008:  // $1F4D
      /*$1F4D*/ CYCLES(0x1f4d, 4);
                s_y = 0x30;
      /*$1F4F*/ s_a = 0x00;
                block_id = 1009;
      break;
    case 1009:  // $1F51
      /*$1F51*/ CYCLES(0x1f51, 9);
                tmp2_U8 = s_y;
                ram_poke((0x0300 + tmp2_U8), s_a);
      /*$1F54*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_y = tmp2_U8;
      /*$1F55*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1010 : 1011;
      break;
    case 1010:  // $1F55
      /*$1F55*/ CYCLES_EDGE(0x1f55, 1);
                branchTarget = true; block_id = 1009;
      break;
    case 1011:  // $1F57
      /*$1F57*/ CYCLES(0x1f57, 32);
      /*$1F59*/ ram_poke(0x00ce, 0x00);
      /*$1F5D*/ ram_poke(0x00cf, 0x03);
      /*$1F65*/ ram_poke(0x00fe, 0x10);
      /*$1F69*/ ram_poke(0x0028, 0x08);
      /*$1F6B*/ branchTarget = true; push16(0x1f6d); block_id = 1547;
      break;
    case 1012:  // $1F6E
      /*$1F6E*/ CYCLES(0x1f6e, 61);
      /*$1F70*/ ram_poke(0x0071, 0x10);
      /*$1F74*/ ram_poke(0x0072, 0x28);
      /*$1F78*/ ram_poke(0x0073, 0x00);
      /*$1F7C*/ ram_poke(0x0074, 0x10);
      /*$1F80*/ ram_poke(0x0076, 0x00);
      /*$1F84*/ ram_poke(0x0070, 0x01);
      /*$1F88*/ ram_poke(0x00e3, 0x00);
      /*$1F8A*/ ram_poke(0x00e4, 0x00);
      /*$1F8E*/ ram_poke(0x007e, 0x00);
      /*$1F90*/ ram_poke(0x007d, 0x00);
      /*$1F92*/ ram_poke(0x00e1, (uint8_t)(ram_peek(0x00e1) + 0x01));
      /*$1F94*/ ram_poke(0x00e1, (uint8_t)(ram_peek(0x00e1) + 0x01));
      /*$1F96*/ tmp2_U8 = ram_peek(0x00a0);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$1F98*/ branchTarget = true; block_id = tmp2_U8 ? 1013 : 1014;
      break;
    case 1013:  // $1F98
      /*$1F98*/ CYCLES_EDGE(0x1f98, 1);
      /*$1F9D*/ CYCLES(0x1f9d, 4);
      /*$1F9F*/ branchTarget = true; block_id = (s_a != 0x01) ? 1015 : 1016;
      break;
    case 1014:  // $1F9A
      /*$1F9A*/ CYCLES(0x1f9a, 5);
                branchTarget = true; block_id = find_block_id_func_t001(0x1f9a, ram_peek16al(0x0036));
      break;
    case 1015:  // $1F9F
      /*$1F9F*/ CYCLES_EDGE(0x1f9f, 1);
      /*$1FC2*/ CYCLES(0x1fc2, 8);
      /*$1FC4*/ ram_poke(0x00f5, 0x02);
      /*$1FC6*/ branchTarget = true; block_id = 767;
      break;
    case 1016:  // $1FA1
      /*$1FA1*/ CYCLES(0x1fa1, 21);
      /*$1FA3*/ ram_poke(0x00f4, 0x5e);
      /*$1FA7*/ ram_poke(0x00f0, 0x00);
      /*$1FAB*/ ram_poke(0x00f1, 0x86);
      /*$1FAD*/ func_3070(0x1faf);
      /*$1FB0*/ CYCLES(0x1fb0, 24);
      /*$1FB2*/ ram_poke(0x00f4, 0x5e);
      /*$1FB6*/ ram_poke(0x00f0, 0x00);
      /*$1FBA*/ ram_poke(0x00f1, 0x86);
      /*$1FBF*/ branchTarget = true; block_id = 767;
      break;
    case 1017:  // $1FD0
      /*$1FD0*/ CYCLES(0x1fd0, 5);
      /*$1FD2*/ branchTarget = true; block_id = ram_peek(0x00a0) ? 1018 : 1019;
      break;
    case 1018:  // $1FD2
      /*$1FD2*/ CYCLES_EDGE(0x1fd2, 1);
      /*$1FD9*/ CYCLES(0x1fd9, 2);
                s_a = 0x50;
                block_id = 1020;
      break;
    case 1019:  // $1FD4
      /*$1FD4*/ CYCLES(0x1fd4, 4);
                s_a = 0x20;
      /*$1FD6*/ CYCLES_EDGE(0x1fd6, 1);
                branchTarget = true; block_id = 1020;
      break;
    case 1020:  // $1FDB
      /*$1FDB*/ CYCLES(0x1fdb, 20);
                ram_poke(0x00e0, s_a);
      /*$1FDD*/ tmp2_U8 = ram_peek(0x0086);
                s_x = tmp2_U8;
      /*$1FDF*/ tmp2_U8 = ram_peek((0x37d8 + tmp2_U8));
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$1FE2*/ ram_poke(0x3821, tmp2_U8);
      /*$1FE5*/ branchTarget = true; block_id = find_block_id_func_t001(0x1fe5, pop16() + 1);;
      break;
    case 1021:  // $2000
      /*$2000*/ CYCLES(0x2000, 6);
                branchTarget = true; push16(0x2002); block_id = 2;
      break;
    case 1022:  // $2003
      /*$2003*/ CYCLES(0x2003, 6);
                branchTarget = true; push16(0x2005); block_id = 208;
      break;
    case 1023:  // $2006
      /*$2006*/ CYCLES(0x2006, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x00d4) - 0x01);
                ram_poke(0x00d4, tmp2_U8);
      /*$2008*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1024 : 1025;
      break;
    case 1024:  // $2008
      /*$2008*/ CYCLES_EDGE(0x2008, 1);
                branchTarget = true; block_id = 1029;
      break;
    case 1025:  // $200A
      /*$200A*/ CYCLES(0x200a, 10);
      /*$200C*/ ram_poke(0x00d4, 0x01);
      /*$200E*/ tmp2_U8 = ram_peek(0x00d0);
                s_a = tmp2_U8;
      /*$2010*/ branchTarget = true; block_id = !tmp2_U8 ? 1026 : 1027;
      break;
    case 1026:  // $2010
      /*$2010*/ CYCLES_EDGE(0x2010, 1);
      /*$2018*/ CYCLES(0x2018, 6);
                branchTarget = true; push16(0x201a); block_id = 345;
      break;
    case 1027:  // $2012
      /*$2012*/ CYCLES(0x2012, 6);
                func_1240(0x2014);
                branchTarget = true; block_id = 1028;
      break;
    case 1028:  // $2015
      /*$2015*/ CYCLES(0x2015, 3);
                branchTarget = true; block_id = 1029;
      break;
    case 1029:  // $201B
      /*$201B*/ CYCLES(0x201b, 10);
                poke(ram_peek16al(0x201c), s_a);
      /*$201E*/ branchTarget = true; push16(0x2020); block_id = 368;
      break;
    case 1030:  // $2021
      /*$2021*/ CYCLES(0x2021, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x0020) - 0x01);
                ram_poke(0x0020, tmp2_U8);
      /*$2023*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1031 : 1032;
      break;
    case 1031:  // $2023
      /*$2023*/ CYCLES_EDGE(0x2023, 1);
                branchTarget = true; block_id = 1034;
      break;
    case 1032:  // $2025
      /*$2025*/ CYCLES(0x2025, 11);
      /*$2027*/ ram_poke(0x0020, 0x01);
      /*$2029*/ func_1560(0x202b);
                branchTarget = true; block_id = 1033;
      break;
    case 1033:  // $202C
      /*$202C*/ CYCLES(0x202c, 6);
                func_1738(0x202e);
                branchTarget = true; block_id = 1034;
      break;
    case 1034:  // $202F
      /*$202F*/ CYCLES(0x202f, 7);
      /*$2033*/ branchTarget = true; block_id = !(ram_peek(0x0086) >= 0x02) ? 1035 : 1036;
      break;
    case 1035:  // $2033
      /*$2033*/ CYCLES_EDGE(0x2033, 1);
                branchTarget = true; block_id = 1048;
      break;
    case 1036:  // $2035
      /*$2035*/ CYCLES(0x2035, 5);
                tmp2_U8 = ram_peek(0x0070);
                s_a = tmp2_U8;
      /*$2037*/ branchTarget = true; block_id = !tmp2_U8 ? 1037 : 1038;
      break;
    case 1037:  // $2037
      /*$2037*/ CYCLES_EDGE(0x2037, 1);
                branchTarget = true; block_id = 1048;
      break;
    case 1038:  // $2039
      /*$2039*/ CYCLES(0x2039, 4);
      /*$203A*/ branchTarget = true; block_id = (s_a >> 0x01) ? 1039 : 1040;
      break;
    case 1039:  // $203A
      /*$203A*/ CYCLES_EDGE(0x203a, 1);
                branchTarget = true; block_id = 1045;
      break;
    case 1040:  // $203C
      /*$203C*/ CYCLES(0x203c, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x0076) - 0x01);
                ram_poke(0x0076, tmp2_U8);
      /*$203E*/ branchTarget = true; block_id = tmp2_U8 ? 1041 : 1042;
      break;
    case 1041:  // $203E
      /*$203E*/ CYCLES_EDGE(0x203e, 1);
                branchTarget = true; block_id = 1048;
      break;
    case 1042:  // $2040
      /*$2040*/ CYCLES(0x2040, 17);
      /*$2042*/ ram_poke(0x0075, 0x04);
      /*$2046*/ ram_poke(0x0077, 0x00);
      /*$2048*/ tmp2_U8 = (uint8_t)(ram_peek(0x0070) + 0x01);
                ram_poke(0x0070, tmp2_U8);
      /*$204A*/ branchTarget = true; block_id = tmp2_U8 ? 1043 : 1044;
      break;
    case 1043:  // $204A
      /*$204A*/ CYCLES_EDGE(0x204a, 1);
                branchTarget = true; block_id = 1048;
      break;
    case 1044:  // $204C
      /*$204C*/ CYCLES(0x204c, 8);
      /*$204F*/ block_id = 1045;
      break;
    case 1045:  // $2050
      /*$2050*/ CYCLES(0x2050, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x0076) - 0x01);
                ram_poke(0x0076, tmp2_U8);
      /*$2052*/ branchTarget = true; block_id = tmp2_U8 ? 1046 : 1047;
      break;
    case 1046:  // $2052
      /*$2052*/ CYCLES_EDGE(0x2052, 1);
                branchTarget = true; block_id = 1048;
      break;
    case 1047:  // $2054
      /*$2054*/ CYCLES(0x2054, 11);
      /*$2056*/ ram_poke(0x0076, 0x03);
      /*$2058*/ func_3480(0x205a);
                branchTarget = true; block_id = 1048;
      break;
    case 1048:  // $205B
      /*$205B*/ CYCLES(0x205b, 11);
      /*$205D*/ ram_poke(0x00a0, 0x00);
      /*$205F*/ func_3800(0x2061);
                branchTarget = true; block_id = 1049;
      break;
    case 1049:  // $2062
      /*$2062*/ CYCLES(0x2062, 5);
                branchTarget = true; block_id = find_block_id_func_t001(0x2062, ram_peek16al(0x0036));
      break;
    case 1050:  // $2083
      /*$2083*/ CYCLES(0x2083, 6);
                branchTarget = true; push16(0x2085); block_id = 208;
      break;
    case 1051:  // $2086
      /*$2086*/ CYCLES(0x2086, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x00d4) - 0x01);
                ram_poke(0x00d4, tmp2_U8);
      /*$2088*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1052 : 1053;
      break;
    case 1052:  // $2088
      /*$2088*/ CYCLES_EDGE(0x2088, 1);
                branchTarget = true; block_id = 1056;
      break;
    case 1053:  // $208A
      /*$208A*/ CYCLES(0x208a, 10);
      /*$208C*/ ram_poke(0x00d4, 0x01);
      /*$208E*/ tmp2_U8 = ram_peek(0x00d0);
                s_a = tmp2_U8;
      /*$2090*/ branchTarget = true; block_id = !tmp2_U8 ? 1054 : 1055;
      break;
    case 1054:  // $2090
      /*$2090*/ CYCLES_EDGE(0x2090, 1);
      /*$2098*/ CYCLES(0x2098, 6);
                branchTarget = true; push16(0x209a); block_id = 345;
      break;
    case 1055:  // $2092
      /*$2092*/ CYCLES(0x2092, 6);
                func_1240(0x2094);
      /*$2095*/ CYCLES(0x2095, 3);
                branchTarget = true; block_id = 1056;
      break;
    case 1056:  // $209B
      /*$209B*/ CYCLES(0x209b, 10);
                poke(ram_peek16al(0x209c), s_a);
      /*$209E*/ branchTarget = true; push16(0x20a0); block_id = 368;
      break;
    case 1057:  // $20A1
      /*$20A1*/ CYCLES(0x20a1, 12);
      /*$20A4*/ branchTarget = true; push16(0x20a6); block_id = 208;
      break;
    case 1058:  // $20A7
      /*$20A7*/ CYCLES(0x20a7, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x00d4) - 0x01);
                ram_poke(0x00d4, tmp2_U8);
      /*$20A9*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1059 : 1060;
      break;
    case 1059:  // $20A9
      /*$20A9*/ CYCLES_EDGE(0x20a9, 1);
                branchTarget = true; block_id = 1063;
      break;
    case 1060:  // $20AB
      /*$20AB*/ CYCLES(0x20ab, 10);
      /*$20AD*/ ram_poke(0x00d4, 0x01);
      /*$20AF*/ tmp2_U8 = ram_peek(0x00d0);
                s_a = tmp2_U8;
      /*$20B1*/ branchTarget = true; block_id = !tmp2_U8 ? 1061 : 1062;
      break;
    case 1061:  // $20B1
      /*$20B1*/ CYCLES_EDGE(0x20b1, 1);
      /*$20B9*/ CYCLES(0x20b9, 6);
                branchTarget = true; push16(0x20bb); block_id = 345;
      break;
    case 1062:  // $20B3
      /*$20B3*/ CYCLES(0x20b3, 6);
                func_1240(0x20b5);
      /*$20B6*/ CYCLES(0x20b6, 3);
                branchTarget = true; block_id = 1063;
      break;
    case 1063:  // $20BC
      /*$20BC*/ CYCLES(0x20bc, 10);
                poke(ram_peek16(0x20bd), s_a);
      /*$20BF*/ branchTarget = true; push16(0x20c1); block_id = 368;
      break;
    case 1064:  // $20C2
      /*$20C2*/ CYCLES(0x20c2, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x0020) - 0x01);
                ram_poke(0x0020, tmp2_U8);
      /*$20C4*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1065 : 1066;
      break;
    case 1065:  // $20C4
      /*$20C4*/ CYCLES_EDGE(0x20c4, 1);
                branchTarget = true; block_id = 1068;
      break;
    case 1066:  // $20C6
      /*$20C6*/ CYCLES(0x20c6, 11);
      /*$20C8*/ ram_poke(0x0020, 0x01);
      /*$20CA*/ branchTarget = true; push16(0x20cc); block_id = 812;
      break;
    case 1067:  // $20CD
      /*$20CD*/ CYCLES(0x20cd, 6);
                func_1738(0x20cf);
                branchTarget = true; block_id = 1068;
      break;
    case 1068:  // $20D0
      /*$20D0*/ CYCLES(0x20d0, 10);
                s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_a = 0x01;
      /*$20D2*/ ram_poke(0x00a0, 0x01);
      /*$20D4*/ branchTarget = true; block_id = find_block_id_func_t001(0x20d4, ram_peek16al(0x0038));
      break;
    case 1069:  // $2100
      /*$2100*/ CYCLES(0x2100, 6);
                branchTarget = true; push16(0x2102); block_id = 2;
      break;
    case 1070:  // $2103
      /*$2103*/ CYCLES(0x2103, 6);
                branchTarget = true; push16(0x2105); block_id = 208;
      break;
    case 1071:  // $2106
      /*$2106*/ CYCLES(0x2106, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x00d4) - 0x01);
                ram_poke(0x00d4, tmp2_U8);
      /*$2108*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1072 : 1073;
      break;
    case 1072:  // $2108
      /*$2108*/ CYCLES_EDGE(0x2108, 1);
                branchTarget = true; block_id = 1076;
      break;
    case 1073:  // $210A
      /*$210A*/ CYCLES(0x210a, 10);
      /*$210C*/ ram_poke(0x00d4, 0x01);
      /*$210E*/ tmp2_U8 = ram_peek(0x00d0);
                s_a = tmp2_U8;
      /*$2110*/ branchTarget = true; block_id = !tmp2_U8 ? 1074 : 1075;
      break;
    case 1074:  // $2110
      /*$2110*/ CYCLES_EDGE(0x2110, 1);
      /*$2118*/ CYCLES(0x2118, 6);
      /*$211A*/ block_id = 1076;
      break;
    case 1075:  // $2112
      /*$2112*/ CYCLES(0x2112, 9);
      /*$2115*/ branchTarget = true; block_id = 1076;
      break;
    case 1076:  // $211B
      /*$211B*/ CYCLES(0x211b, 16);
                poke(ram_peek16al(0x211c), s_a);
      /*$2121*/ branchTarget = true; push16(0x2123); block_id = 1084;
      break;
    case 1077:  // $2124
      /*$2124*/ CYCLES(0x2124, 6);
                branchTarget = true; push16(0x2126); block_id = 208;
      break;
    case 1078:  // $2127
      /*$2127*/ CYCLES(0x2127, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x00d4) - 0x01);
                ram_poke(0x00d4, tmp2_U8);
      /*$2129*/ branchTarget = true; block_id = !(tmp2_U8 & 0x80) ? 1079 : 1080;
      break;
    case 1079:  // $2129
      /*$2129*/ CYCLES_EDGE(0x2129, 1);
                branchTarget = true; block_id = 1083;
      break;
    case 1080:  // $212B
      /*$212B*/ CYCLES(0x212b, 10);
      /*$212D*/ ram_poke(0x00d4, 0x00);
      /*$212F*/ tmp2_U8 = ram_peek(0x00d0);
                s_a = tmp2_U8;
      /*$2131*/ branchTarget = true; block_id = !tmp2_U8 ? 1081 : 1082;
      break;
    case 1081:  // $2131
      /*$2131*/ CYCLES_EDGE(0x2131, 1);
      /*$2139*/ CYCLES(0x2139, 6);
      /*$213B*/ block_id = 1083;
      break;
    case 1082:  // $2133
      /*$2133*/ CYCLES(0x2133, 9);
      /*$2136*/ branchTarget = true; block_id = 1083;
      break;
    case 1083:  // $213C
      /*$213C*/ CYCLES(0x213c, 20);
                poke(ram_peek16(0x213d), s_a);
      /*$2142*/ s_status_not_z = 0x02;
                s_status_n = 0x00;
                s_a = 0x02;
      /*$2144*/ ram_poke(0x00a0, 0x02);
      /*$2146*/ branchTarget = true; block_id = find_block_id_func_t001(0x2146, ram_peek16al(0x004e));
      break;
    case 1084:  // $2150
      /*$2150*/ CYCLES(0x2150, 6);
                branchTarget = true; push16(0x2152); block_id = 72;
      break;
    case 1085:  // $2153
      /*$2153*/ CYCLES(0x2153, 5);
                tmp2_U8 = ram_peek(0x00af);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$2155*/ branchTarget = true; block_id = tmp2_U8 ? 1086 : 1087;
      break;
    case 1086:  // $2155
      /*$2155*/ CYCLES_EDGE(0x2155, 1);
      /*$2158*/ CYCLES(0x2158, 9);
                s_status_c = 0x01;
      /*$2159*/ block_id = s_status_d ? 1089 : 1088;
      break;
    case 1087:  // $2157
      /*$2157*/ CYCLES(0x2157, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x2157, pop16() + 1);;
      break;
    case 1088:  // $2159
      /*$2159*/ tmp6_U16 = s_a;
                tmp5_U16 = (tmp6_U16 - 0x0001) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp6_U16, (uint8_t)0xfffe);
                tmp2_U8 = (uint8_t)tmp5_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 1090;
      break;
    case 1089:  // $2159
      /*$2159*/ tmp5_U16 = sbc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                tmp2_U8 = (uint8_t)(tmp5_U16 >> 8);
                s_status_c = (tmp2_U8 & 0x01);
                s_status_not_z = (~tmp2_U8 & 2);
                s_status_v = ((tmp2_U8 & 0x40) != 0);
                s_status_n = (tmp2_U8 & 0x80);
                block_id = 1090;
      break;
    case 1090:  // $215B
      /*$215B*/ ram_poke(0x00af, s_a);
      /*$215D*/ branchTarget = true; block_id = s_status_not_z ? 1091 : 1092;
      break;
    case 1091:  // $215D
      /*$215D*/ CYCLES_EDGE(0x215d, 1);
                branchTarget = true; block_id = 1087;
      break;
    case 1092:  // $215F
      /*$215F*/ CYCLES(0x215f, 6);
                func_2f80(0x2161);
      /*$2162*/ CYCLES(0x2162, 6);
                func_2f00(0x2164);
      /*$2165*/ CYCLES(0x2165, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x2165, pop16() + 1);;
      break;
    case 1093:  // $2183
      /*$2183*/ CYCLES(0x2183, 6);
                branchTarget = true; push16(0x2185); block_id = 208;
      break;
    case 1094:  // $2186
      /*$2186*/ CYCLES(0x2186, 8);
                s_a = ram_peek(0x2187);
      /*$2188*/ rom_mon_wait(0x218a);
      /*$218B*/ CYCLES(0x218b, 10);
                s_status_not_z = 0x02;
                s_status_n = 0x00;
                s_a = 0x02;
      /*$218D*/ ram_poke(0x00a0, 0x02);
      /*$218F*/ branchTarget = true; block_id = find_block_id_func_t001(0x218f, ram_peek16al(0x003a));
      break;
    case 1095:  // $2200
      /*$2200*/ CYCLES(0x2200, 3);
                s_x = ram_peek(0x0004);
                block_id = 1096;
      break;
    case 1096:  // $2202
      /*$2202*/ CYCLES(0x2202, 17);
                tmp2_U8 = s_x;
      /*$2205*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp2_U8)));
      /*$220A*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp2_U8)));
      /*$220C*/ s_y = ram_peek(0x0006);
                block_id = 1097;
      break;
    case 1097:  // $220E
      /*$220E*/ CYCLES(0x220e, 4);
      /*$2210*/ branchTarget = true; block_id = (s_y >= 0x28) ? 1098 : 1099;
      break;
    case 1098:  // $2210
      /*$2210*/ CYCLES_EDGE(0x2210, 1);
                branchTarget = true; block_id = 1100;
      break;
    case 1099:  // $2212
      /*$2212*/ CYCLES(0x2212, 10);
                tmp2_U8 = peek(ram_peek16(0x2213));
      /*$2215*/ poke((ram_peek16al(0x0000) + s_y), tmp2_U8);
                block_id = 1100;
      break;
    case 1100:  // $2217
      /*$2217*/ CYCLES(0x2217, 13);
                ram_poke(0x2213, (uint8_t)(ram_peek(0x2213) + 0x01));
      /*$221A*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$221D*/ branchTarget = true; block_id = (tmp2_U8 != ram_peek(0x0007)) ? 1101 : 1102;
      break;
    case 1101:  // $221D
      /*$221D*/ CYCLES_EDGE(0x221d, 1);
                branchTarget = true; block_id = 1097;
      break;
    case 1102:  // $221F
      /*$221F*/ CYCLES(0x221f, 6);
                s_a = 0x00;
      /*$2223*/ branchTarget = true; block_id = (s_y >= 0x28) ? 1103 : 1104;
      break;
    case 1103:  // $2223
      /*$2223*/ CYCLES_EDGE(0x2223, 1);
                branchTarget = true; block_id = 1105;
      break;
    case 1104:  // $2225
      /*$2225*/ CYCLES(0x2225, 6);
                poke((ram_peek16al(0x0000) + s_y), s_a);
                block_id = 1105;
      break;
    case 1105:  // $2227
      /*$2227*/ CYCLES(0x2227, 7);
                tmp2_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp2_U8;
      /*$2228*/ tmp2_U8 = tmp2_U8 >= ram_peek(0x0005);
                s_status_c = tmp2_U8;
      /*$222A*/ branchTarget = true; block_id = !tmp2_U8 ? 1106 : 1107;
      break;
    case 1106:  // $222A
      /*$222A*/ CYCLES_EDGE(0x222a, 1);
                branchTarget = true; block_id = 1096;
      break;
    case 1107:  // $222C
      /*$222C*/ CYCLES(0x222c, 15);
                poke(ram_peek16(0x222d), s_a);
      /*$222F*/ s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_a = 0x01;
      /*$2231*/ ram_poke(0x000a, 0x01);
      /*$2233*/ branchTarget = true; block_id = find_block_id_func_t001(0x2233, pop16() + 1);;
      break;
    case 1108:  // $2240
      /*$2240*/ CYCLES(0x2240, 10);
      /*$2242*/ ram_poke(0x0008, 0x00);
      /*$2246*/ ram_poke(0x0009, 0x03);
                block_id = 1109;
      break;
    case 1109:  // $2248
      /*$2248*/ CYCLES(0x2248, 9);
                s_y = 0x00;
      /*$224A*/ tmp2_U8 = peek(ram_peek16al(0x0008));
      /*$224C*/ branchTarget = true; block_id = !tmp2_U8 ? 1110 : 1111;
      break;
    case 1110:  // $224C
      /*$224C*/ CYCLES_EDGE(0x224c, 1);
                branchTarget = true; block_id = 1141;
      break;
    case 1111:  // $224E
      /*$224E*/ CYCLES(0x224e, 80);
                tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$224F*/ tmp2_U8 = peek((ram_peek16al(0x0008) + tmp2_U8));
                s_a = tmp2_U8;
      /*$2251*/ ram_poke(0x0004, tmp2_U8);
      /*$2253*/ s_status_c = 0x00;
      /*$2254*/ block_id = s_status_d ? 1113 : 1112;
      break;
    case 1112:  // $2254
      /*$2254*/ s_a = (uint8_t)((s_a + 0x0008) + s_status_c);
                block_id = 1114;
      break;
    case 1113:  // $2254
      /*$2254*/ s_a = ((uint8_t)adc_dec16(s_a, 0x08, s_status_c));
                block_id = 1114;
      break;
    case 1114:  // $2256
      /*$2256*/ ram_poke(0x0005, s_a);
      /*$2258*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$2259*/ tmp2_U8 = peek((ram_peek16al(0x0008) + tmp2_U8));
                s_a = tmp2_U8;
      /*$225B*/ ram_poke(0x0006, tmp2_U8);
      /*$225D*/ s_status_c = 0x00;
      /*$225E*/ block_id = s_status_d ? 1116 : 1115;
      break;
    case 1115:  // $225E
      /*$225E*/ s_a = (uint8_t)((s_a + 0x0004) + s_status_c);
                block_id = 1117;
      break;
    case 1116:  // $225E
      /*$225E*/ s_a = ((uint8_t)adc_dec16(s_a, 0x04, s_status_c));
                block_id = 1117;
      break;
    case 1117:  // $2260
      /*$2260*/ ram_poke(0x0007, s_a);
      /*$2262*/ tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$2263*/ tmp2_U8 = peek((ram_peek16al(0x0008) + tmp2_U8));
                s_a = tmp2_U8;
      /*$2265*/ s_status_c = 0x00;
      /*$2266*/ block_id = s_status_d ? 1119 : 1118;
      break;
    case 1118:  // $2266
      /*$2266*/ s_a = (uint8_t)((s_a + 0x0070) + s_status_c);
                block_id = 1120;
      break;
    case 1119:  // $2266
      /*$2266*/ s_a = ((uint8_t)adc_dec16(s_a, 0x70, s_status_c));
                block_id = 1120;
      break;
    case 1120:  // $2268
      /*$2268*/ ram_poke(0x2214, s_a);
      /*$226C*/ tmp2_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(s_y + 0x01)));
      /*$2276*/ ram_poke(0x2213, 0x40);
      /*$2279*/ func_2200(0x227b);
      /*$227C*/ CYCLES(0x227c, 30);
      /*$227E*/ tmp2_U8 = peek((ram_peek16al(0x0008) + 0x0005));
      /*$2280*/ ram_poke(0x0000, tmp2_U8);
      /*$2282*/ s_y = 0x03;
      /*$2284*/ tmp2_U8 = peek((ram_peek16al(0x0008) + 0x0003));
                s_a = tmp2_U8;
      /*$2286*/ s_status_c = 0x01;
      /*$2287*/ block_id = s_status_d ? 1122 : 1121;
      break;
    case 1121:  // $2287
      /*$2287*/ tmp2_U8 = (uint8_t)((s_a - ram_peek(0x0000)) - (uint8_t)(0x01 - s_status_c));
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
                block_id = 1123;
      break;
    case 1122:  // $2287
      /*$2287*/ tmp5_U16 = sbc_dec16(s_a, ram_peek(0x0000), s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_n = ((uint8_t)(tmp5_U16 >> 8) & 0x80);
                block_id = 1123;
      break;
    case 1123:  // $2289
      /*$2289*/ poke((ram_peek16al(0x0008) + s_y), s_a);
      /*$228B*/ branchTarget = true; block_id = !s_status_n ? 1124 : 1125;
      break;
    case 1124:  // $228B
      /*$228B*/ CYCLES_EDGE(0x228b, 1);
                branchTarget = true; block_id = 1147;
      break;
    case 1125:  // $228D
      /*$228D*/ CYCLES(0x228d, 31);
                s_status_c = 0x00;
      /*$228E*/ block_id = s_status_d ? 1127 : 1126;
      break;
    case 1126:  // $228E
      /*$228E*/ s_a = (uint8_t)((s_a + 0x0007) + s_status_c);
                block_id = 1128;
      break;
    case 1127:  // $228E
      /*$228E*/ s_a = ((uint8_t)adc_dec16(s_a, 0x07, s_status_c));
                block_id = 1128;
      break;
    case 1128:  // $2290
      /*$2290*/ tmp2_U8 = s_y;
                poke((ram_peek16al(0x0008) + tmp2_U8), s_a);
      /*$2292*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_y = tmp2_U8;
      /*$2293*/ tmp2_U8 = peek((ram_peek16al(0x0008) + tmp2_U8));
                s_a = tmp2_U8;
      /*$2295*/ s_status_c = 0x01;
      /*$2296*/ block_id = s_status_d ? 1130 : 1129;
      break;
    case 1129:  // $2296
      /*$2296*/ s_a = (uint8_t)((s_a - 0x0001) - (uint8_t)(0x01 - s_status_c));
                block_id = 1131;
      break;
    case 1130:  // $2296
      /*$2296*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x01, s_status_c));
                block_id = 1131;
      break;
    case 1131:  // $2298
      /*$2298*/ tmp2_U8 = s_a;
                poke((ram_peek16al(0x0008) + s_y), tmp2_U8);
      /*$229C*/ branchTarget = true; block_id = !(tmp2_U8 >= 0xf0) ? 1132 : 1133;
      break;
    case 1132:  // $229C
      /*$229C*/ CYCLES_EDGE(0x229c, 1);
                branchTarget = true; block_id = 1147;
      break;
    case 1133:  // $229E
      /*$229E*/ CYCLES(0x229e, 4);
      /*$22A0*/ branchTarget = true; block_id = (s_a >= 0xf9) ? 1134 : 1135;
      break;
    case 1134:  // $22A0
      /*$22A0*/ CYCLES_EDGE(0x22a0, 1);
                branchTarget = true; block_id = 1147;
      break;
    case 1135:  // $22A2
      /*$22A2*/ CYCLES(0x22a2, 21);
                s_y = 0x00;
      /*$22A4*/ tmp2_U8 = peek(ram_peek16al(0x0008));
                s_a = tmp2_U8;
      /*$22A6*/ s_status_c = 0x00;
      /*$22A7*/ block_id = s_status_d ? 1137 : 1136;
      break;
    case 1136:  // $22A7
      /*$22A7*/ s_a = (uint8_t)((s_a + 0x0001) + s_status_c);
                block_id = 1138;
      break;
    case 1137:  // $22A7
      /*$22A7*/ s_a = ((uint8_t)adc_dec16(s_a, 0x01, s_status_c));
                block_id = 1138;
      break;
    case 1138:  // $22A9
      /*$22A9*/ tmp2_U8 = s_a;
                poke((ram_peek16al(0x0008) + s_y), tmp2_U8);
      /*$22AD*/ branchTarget = true; block_id = !(tmp2_U8 >= 0x08) ? 1139 : 1140;
      break;
    case 1139:  // $22AD
      /*$22AD*/ CYCLES_EDGE(0x22ad, 1);
      /*$22B8*/ CYCLES(0x22b8, 10);
                s_x = 0x10;
      /*$22BA*/ s_y = 0x00;
      /*$22BC*/ func_2e00(0x22be);
      /*$22BF*/ CYCLES(0x22bf, 2);
                branchTarget = true; block_id = s_status_not_z ? 1142 : 1143;
      break;
    case 1140:  // $22AF
      /*$22AF*/ CYCLES(0x22af, 10);
                s_y = 0x00;
      /*$22B3*/ poke(ram_peek16al(0x0008), 0x00);
                block_id = 1141;
      break;
    case 1141:  // $22B5
      /*$22B5*/ CYCLES(0x22b5, 3);
                branchTarget = true; block_id = 1159;
      break;
    case 1142:  // $22BF
      /*$22BF*/ CYCLES_EDGE(0x22bf, 1);
                branchTarget = true; block_id = 1146;
      break;
    case 1143:  // $22C1
      /*$22C1*/ CYCLES(0x22c1, 2);
                branchTarget = true; block_id = !s_status_not_z ? 1144 : 1145;
      break;
    case 1144:  // $22C1
      /*$22C1*/ CYCLES_EDGE(0x22c1, 1);
                branchTarget = true; block_id = 1146;
      break;
    case 1145:  // $22C3
      /*$22C3*/ CYCLES(0x22c3, 18);
      /*$22CB*/ poke((ram_peek16al(0x00a0) + 0x0001), ram_peek((0x21f8 + (s_a >> 0x02))));
      /*$22CD*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x22cd, 0x22cd, 0x03); error_handler(0x22cd);
      break;
    case 1146:  // $22CC
      /*$22CC*/ CYCLES(0x22cc, 13);
                s_y = 0x02;
      /*$22D0*/ poke((ram_peek16al(0x0008) + 0x0002), 0x28);
      /*$22D2*/ branchTarget = true; block_id = 1159;
      break;
    case 1147:  // $22D5
      /*$22D5*/ CYCLES(0x22d5, 21);
                s_y = 0x06;
      /*$22D7*/ tmp2_U8 = peek((ram_peek16al(0x0008) + 0x0006));
                s_a = tmp2_U8;
      /*$22D9*/ s_status_c = 0x00;
      /*$22DA*/ block_id = s_status_d ? 1149 : 1148;
      break;
    case 1148:  // $22DA
      /*$22DA*/ s_a = (uint8_t)((s_a + 0x0001) + s_status_c);
                block_id = 1150;
      break;
    case 1149:  // $22DA
      /*$22DA*/ s_a = ((uint8_t)adc_dec16(s_a, 0x01, s_status_c));
                block_id = 1150;
      break;
    case 1150:  // $22DC
      /*$22DC*/ tmp2_U8 = s_a;
                poke((ram_peek16al(0x0008) + s_y), tmp2_U8);
      /*$22E0*/ branchTarget = true; block_id = !(tmp2_U8 >= 0x03) ? 1151 : 1152;
      break;
    case 1151:  // $22E0
      /*$22E0*/ CYCLES_EDGE(0x22e0, 1);
                branchTarget = true; block_id = 1159;
      break;
    case 1152:  // $22E2
      /*$22E2*/ CYCLES(0x22e2, 23);
      /*$22E4*/ poke((ram_peek16al(0x0008) + s_y), 0x00);
      /*$22E6*/ s_y = 0x04;
      /*$22E8*/ tmp2_U8 = peek((ram_peek16al(0x0008) + 0x0004));
                s_a = tmp2_U8;
      /*$22EA*/ s_status_c = 0x00;
      /*$22EB*/ block_id = s_status_d ? 1154 : 1153;
      break;
    case 1153:  // $22EB
      /*$22EB*/ s_a = (uint8_t)((s_a + 0x0001) + s_status_c);
                block_id = 1155;
      break;
    case 1154:  // $22EB
      /*$22EB*/ s_a = ((uint8_t)adc_dec16(s_a, 0x01, s_status_c));
                block_id = 1155;
      break;
    case 1155:  // $22ED
      /*$22EF*/ branchTarget = true; block_id = !(s_a >= 0x03) ? 1156 : 1157;
      break;
    case 1156:  // $22EF
      /*$22EF*/ CYCLES_EDGE(0x22ef, 1);
                branchTarget = true; block_id = 1158;
      break;
    case 1157:  // $22F1
      /*$22F1*/ CYCLES(0x22f1, 2);
                s_a = 0x00;
                block_id = 1158;
      break;
    case 1158:  // $22F3
      /*$22F3*/ CYCLES(0x22f3, 9);
                poke((ram_peek16al(0x0008) + s_y), s_a);
      /*$22F5*/ branchTarget = true; block_id = 1159;
      break;
    case 1159:  // $2310
      /*$2310*/ CYCLES(0x2310, 11);
                s_a = ram_peek(0x0008);
      /*$2312*/ s_status_c = 0x00;
      /*$2313*/ block_id = s_status_d ? 1161 : 1160;
      break;
    case 1160:  // $2313
      /*$2313*/ tmp5_U16 = s_a;
                tmp6_U16 = (tmp5_U16 + 0x0008) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp5_U16, (uint8_t)0x0008);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1162;
      break;
    case 1161:  // $2313
      /*$2313*/ tmp6_U16 = adc_dec16(s_a, 0x08, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 1162;
      break;
    case 1162:  // $2315
      /*$2315*/ tmp2_U8 = s_a;
                s_status_not_z = (tmp2_U8 != 0x40);
                tmp3_U8 = tmp2_U8 >= 0x40;
                s_status_c = tmp3_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - 0x40) & 0x80);
      /*$2317*/ branchTarget = true; block_id = tmp3_U8 ? 1163 : 1164;
      break;
    case 1163:  // $2317
      /*$2317*/ CYCLES_EDGE(0x2317, 1);
      /*$231E*/ CYCLES(0x231e, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x231e, pop16() + 1);;
      break;
    case 1164:  // $2319
      /*$2319*/ CYCLES(0x2319, 6);
                ram_poke(0x0008, s_a);
      /*$231B*/ branchTarget = true; block_id = 1109;
      break;
    case 1165:  // $2380
      /*$2380*/ CYCLES(0x2380, 2);
                s_y = 0x60;
                block_id = 1166;
      break;
    case 1166:  // $2382
      /*$2382*/ CYCLES(0x2382, 11);
      /*$2384*/ tmp3_U8 = s_y;
                ram_poke((0x0300 + tmp3_U8), 0x00);
      /*$2387*/ tmp3_U8 = (uint8_t)(tmp3_U8 - 0x01);
                s_y = tmp3_U8;
      /*$2388*/ branchTarget = true; block_id = !(tmp3_U8 & 0x80) ? 1167 : 1168;
      break;
    case 1167:  // $2388
      /*$2388*/ CYCLES_EDGE(0x2388, 1);
                branchTarget = true; block_id = 1166;
      break;
    case 1168:  // $238A
      /*$238A*/ CYCLES(0x238a, 2);
                s_x = 0x00;
                block_id = 1169;
      break;
    case 1169:  // $238C
      /*$238C*/ CYCLES(0x238c, 60);
                tmp3_U8 = s_x;
      /*$238F*/ tmp6_U16 = tmp3_U8 << 0x03;
                tmp2_U8 = (uint8_t)tmp6_U16;
      /*$2393*/ ram_poke((0x0300 + (tmp6_U16 & 0x00ff)), 0x01);
      /*$239A*/ ram_poke((0x0300 + (uint8_t)(tmp2_U8 + 0x01)), ram_peek((0x2340 + tmp3_U8)));
      /*$23A1*/ ram_poke((0x0300 + (uint8_t)(tmp2_U8 + 0x02)), ram_peek((0x2350 + tmp3_U8)));
      /*$23AA*/ ram_poke((0x0300 + (uint8_t)(tmp2_U8 + 0x05)), ram_peek((0x2360 + tmp3_U8)));
      /*$23AD*/ tmp3_U8 = (uint8_t)(tmp3_U8 + 0x01);
                s_x = tmp3_U8;
      /*$23B0*/ branchTarget = true; block_id = !(tmp3_U8 >= 0x08) ? 1170 : 1171;
      break;
    case 1170:  // $23B0
      /*$23B0*/ CYCLES_EDGE(0x23b0, 1);
                branchTarget = true; block_id = 1169;
      break;
    case 1171:  // $23B2
      /*$23B2*/ CYCLES(0x23b2, 2);
                s_y = 0x00;
                block_id = 1172;
      break;
    case 1172:  // $23B4
      /*$23B4*/ CYCLES(0x23b4, 15);
                tmp3_U8 = s_y;
                tmp2_U8 = ram_peek((0x2320 + tmp3_U8));
                s_a = tmp2_U8;
      /*$23B7*/ ram_poke((0x0340 + tmp3_U8), tmp2_U8);
      /*$23BA*/ tmp2_U8 = (uint8_t)(tmp3_U8 + 0x01);
                s_y = tmp2_U8;
      /*$23BB*/ s_status_not_z = (tmp2_U8 != 0x1f);
                tmp2_U8 = tmp2_U8 >= 0x1f;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp3_U8 - 0x1e) & 0x80);
      /*$23BD*/ branchTarget = true; block_id = !tmp2_U8 ? 1173 : 1174;
      break;
    case 1173:  // $23BD
      /*$23BD*/ CYCLES_EDGE(0x23bd, 1);
                branchTarget = true; block_id = 1172;
      break;
    case 1174:  // $23BF
      /*$23BF*/ CYCLES(0x23bf, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x23bf, pop16() + 1);;
      break;
    case 1175:  // $2400
      /*$2400*/ CYCLES(0x2400, 10);
      /*$2402*/ ram_poke(0x0008, 0x40);
      /*$2406*/ ram_poke(0x0009, 0x03);
                block_id = 1176;
      break;
    case 1176:  // $2408
      /*$2408*/ CYCLES(0x2408, 67);
      /*$240A*/ tmp2_U8 = peek((ram_peek16al(0x0008) + 0x0002));
      /*$2410*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp2_U8)));
      /*$2415*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp2_U8)));
      /*$2417*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_x = tmp2_U8;
      /*$241B*/ ram_poke(0x0002, ram_peek((0x0d00 + tmp2_U8)));
      /*$2420*/ ram_poke(0x0003, ram_peek((0x0c00 + tmp2_U8)));
      /*$2423*/ tmp2_U8 = peek((ram_peek16al(0x0008) + 0x0003));
      /*$2425*/ s_y = tmp2_U8;
      /*$2428*/ poke((ram_peek16al(0x0000) + tmp2_U8), 0x00);
      /*$242A*/ poke((ram_peek16al(0x0002) + tmp2_U8), 0x00);
      /*$242E*/ branchTarget = true; block_id = !ram_peek(0x000a) ? 1177 : 1178;
      break;
    case 1177:  // $242E
      /*$242E*/ CYCLES_EDGE(0x242e, 1);
                branchTarget = true; block_id = 1199;
      break;
    case 1178:  // $2430
      /*$2430*/ CYCLES(0x2430, 9);
      /*$2432*/ tmp2_U8 = peek((ram_peek16al(0x0008) + 0x0004));
      /*$2434*/ branchTarget = true; block_id = tmp2_U8 ? 1179 : 1180;
      break;
    case 1179:  // $2434
      /*$2434*/ CYCLES_EDGE(0x2434, 1);
      /*$2452*/ CYCLES(0x2452, 20);
      /*$2454*/ tmp2_U8 = peek((ram_peek16al(0x0008) + 0x0002));
                s_a = tmp2_U8;
      /*$2456*/ s_status_c = 0x00;
      /*$2457*/ block_id = s_status_d ? 1190 : 1189;
      break;
    case 1180:  // $2436
      /*$2436*/ CYCLES(0x2436, 20);
      /*$2438*/ tmp2_U8 = peek((ram_peek16al(0x0008) + 0x0002));
                s_a = tmp2_U8;
      /*$243A*/ s_status_c = 0x01;
      /*$243B*/ block_id = s_status_d ? 1182 : 1181;
      break;
    case 1181:  // $243B
      /*$243B*/ s_a = (uint8_t)((s_a - 0x0001) - (uint8_t)(0x01 - s_status_c));
                block_id = 1183;
      break;
    case 1182:  // $243B
      /*$243B*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x01, s_status_c));
                block_id = 1183;
      break;
    case 1183:  // $243D
      /*$243F*/ tmp2_U8 = peek(ram_peek16al(0x0008));
      /*$2441*/ branchTarget = true; block_id = (s_a >= tmp2_U8) ? 1184 : 1185;
      break;
    case 1184:  // $2441
      /*$2441*/ CYCLES_EDGE(0x2441, 1);
                branchTarget = true; block_id = 1188;
      break;
    case 1185:  // $2443
      /*$2443*/ CYCLES(0x2443, 4);
                s_y = 0x04;
      /*$2445*/ s_status_not_z = 0x01;
                s_a = 0x01;
                block_id = 1186;
      break;
    case 1186:  // $2447
      /*$2447*/ CYCLES(0x2447, 8);
                poke((ram_peek16al(0x0008) + s_y), s_a);
      /*$2449*/ branchTarget = true; block_id = s_status_not_z ? 1187 : 1188;
      break;
    case 1187:  // $2449
      /*$2449*/ CYCLES_EDGE(0x2449, 1);
                branchTarget = true; block_id = 1194;
      break;
    case 1188:  // $244B
      /*$244B*/ CYCLES(0x244b, 11);
      /*$244D*/ poke((ram_peek16al(0x0008) + 0x0002), s_a);
      /*$244F*/ branchTarget = true; block_id = 1194;
      break;
    case 1189:  // $2457
      /*$2457*/ s_a = (uint8_t)((s_a + 0x0001) + s_status_c);
                block_id = 1191;
      break;
    case 1190:  // $2457
      /*$2457*/ s_a = ((uint8_t)adc_dec16(s_a, 0x01, s_status_c));
                block_id = 1191;
      break;
    case 1191:  // $2459
      /*$245B*/ tmp2_U8 = peek((ram_peek16al(0x0008) + 0x0001));
      /*$245D*/ branchTarget = true; block_id = !(s_a >= tmp2_U8) ? 1192 : 1193;
      break;
    case 1192:  // $245D
      /*$245D*/ CYCLES_EDGE(0x245d, 1);
                branchTarget = true; block_id = 1188;
      break;
    case 1193:  // $245F
      /*$245F*/ CYCLES(0x245f, 6);
                s_a = 0x00;
      /*$2461*/ s_status_not_z = 0x04;
                s_y = 0x04;
      /*$2463*/ CYCLES_EDGE(0x2463, 1);
                branchTarget = true; block_id = 1186;
      break;
    case 1194:  // $2465
      /*$2465*/ CYCLES(0x2465, 11);
                s_y = 0x02;
      /*$2467*/ tmp2_U8 = peek((ram_peek16al(0x0008) + 0x0002));
                s_a = tmp2_U8;
      /*$246B*/ branchTarget = true; block_id = !(tmp2_U8 >= 0x20) ? 1195 : 1196;
      break;
    case 1195:  // $246B
      /*$246B*/ CYCLES_EDGE(0x246b, 1);
                branchTarget = true; block_id = 1198;
      break;
    case 1196:  // $246D
      /*$246D*/ CYCLES(0x246d, 4);
      /*$246F*/ branchTarget = true; block_id = !(s_a >= 0x88) ? 1197 : 1198;
      break;
    case 1197:  // $246F
      /*$246F*/ CYCLES_EDGE(0x246f, 1);
                branchTarget = true; block_id = 1199;
      break;
    case 1198:  // $2471
      /*$2471*/ CYCLES(0x2471, 55);
                tmp2_U8 = s_a;
      /*$2475*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp2_U8)));
      /*$247A*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp2_U8)));
      /*$247C*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_x = tmp2_U8;
      /*$2480*/ ram_poke(0x0002, ram_peek((0x0d00 + tmp2_U8)));
      /*$2485*/ ram_poke(0x0003, ram_peek((0x0c00 + tmp2_U8)));
      /*$2488*/ tmp2_U8 = peek((ram_peek16al(0x0008) + (uint8_t)(s_y + 0x01)));
      /*$248A*/ s_y = tmp2_U8;
      /*$248D*/ poke((ram_peek16al(0x0000) + tmp2_U8), 0x28);
      /*$248F*/ poke((ram_peek16al(0x0002) + tmp2_U8), 0x28);
                block_id = 1199;
      break;
    case 1199:  // $2491
      /*$2491*/ CYCLES(0x2491, 14);
                s_a = ram_peek(0x0008);
      /*$2493*/ s_status_c = 0x00;
      /*$2494*/ block_id = s_status_d ? 1201 : 1200;
      break;
    case 1200:  // $2494
      /*$2494*/ tmp6_U16 = s_a;
                tmp5_U16 = (tmp6_U16 + 0x0005) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp6_U16, (uint8_t)0x0005);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 1202;
      break;
    case 1201:  // $2494
      /*$2494*/ tmp5_U16 = adc_dec16(s_a, 0x05, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
                block_id = 1202;
      break;
    case 1202:  // $2496
      /*$2496*/ tmp2_U8 = s_a;
                ram_poke(0x0008, tmp2_U8);
      /*$2498*/ s_status_not_z = (tmp2_U8 != 0x54);
                tmp3_U8 = tmp2_U8 >= 0x54;
                s_status_c = tmp3_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - 0x54) & 0x80);
      /*$249A*/ branchTarget = true; block_id = tmp3_U8 ? 1203 : 1204;
      break;
    case 1203:  // $249A
      /*$249A*/ CYCLES_EDGE(0x249a, 1);
      /*$249F*/ CYCLES(0x249f, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x249f, pop16() + 1);;
      break;
    case 1204:  // $249C
      /*$249C*/ CYCLES(0x249c, 3);
                branchTarget = true; block_id = 1176;
      break;
    case 1205:  // $24A0
      /*$24A0*/ CYCLES(0x24a0, 6);
                func_2240(0x24a2);
      /*$24A3*/ CYCLES(0x24a3, 7);
                tmp3_U8 = (uint8_t)(ram_peek(0x0020) - 0x01);
                s_status_not_z = tmp3_U8;
                s_status_n = (tmp3_U8 & 0x80);
                ram_poke(0x0020, tmp3_U8);
      /*$24A5*/ branchTarget = true; block_id = tmp3_U8 ? 1206 : 1207;
      break;
    case 1206:  // $24A5
      /*$24A5*/ CYCLES_EDGE(0x24a5, 1);
                branchTarget = true; block_id = 1208;
      break;
    case 1207:  // $24A7
      /*$24A7*/ CYCLES(0x24a7, 11);
      /*$24A9*/ ram_poke(0x0020, 0x02);
      /*$24AB*/ func_2400(0x24ad);
                branchTarget = true; block_id = 1208;
      break;
    case 1208:  // $24AE
      /*$24AE*/ CYCLES(0x24ae, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x24ae, pop16() + 1);;
      break;
    case 1209:  // $24D0
      /*$24D0*/ CYCLES(0x24d0, 11);
      /*$24D2*/ ram_poke(0x000a, 0x00);
      /*$24D4*/ func_24a0(0x24d6);
      /*$24D7*/ CYCLES(0x24d7, 5);
                tmp2_U8 = ram_peek(0x000a);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$24D9*/ branchTarget = true; block_id = !tmp2_U8 ? 1210 : 1211;
      break;
    case 1210:  // $24D9
      /*$24D9*/ CYCLES_EDGE(0x24d9, 1);
      /*$24DC*/ CYCLES(0x24dc, 3);
                branchTarget = true; block_id = 1331;
      break;
    case 1211:  // $24DB
      /*$24DB*/ CYCLES(0x24db, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x24db, pop16() + 1);;
      break;
    case 1212:  // $2500
      /*$2500*/ CYCLES(0x2500, 3);
                s_x = ram_peek(0x0004);
                block_id = 1213;
      break;
    case 1213:  // $2502
      /*$2502*/ CYCLES(0x2502, 18);
                tmp3_U8 = s_x;
      /*$2505*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp3_U8)));
      /*$250A*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp3_U8)));
      /*$250C*/ s_y = 0x27;
      /*$250E*/ s_a = 0x00;
                block_id = 1214;
      break;
    case 1214:  // $2510
      /*$2510*/ CYCLES(0x2510, 10);
                tmp3_U8 = s_y;
                poke((ram_peek16al(0x0000) + tmp3_U8), s_a);
      /*$2512*/ tmp3_U8 = (uint8_t)(tmp3_U8 - 0x01);
                s_y = tmp3_U8;
      /*$2513*/ branchTarget = true; block_id = !(tmp3_U8 & 0x80) ? 1215 : 1216;
      break;
    case 1215:  // $2513
      /*$2513*/ CYCLES_EDGE(0x2513, 1);
                branchTarget = true; block_id = 1214;
      break;
    case 1216:  // $2515
      /*$2515*/ CYCLES(0x2515, 7);
                tmp3_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp3_U8;
      /*$2516*/ tmp3_U8 = tmp3_U8 >= ram_peek(0x0005);
                s_status_c = tmp3_U8;
      /*$2518*/ branchTarget = true; block_id = !tmp3_U8 ? 1217 : 1218;
      break;
    case 1217:  // $2518
      /*$2518*/ CYCLES_EDGE(0x2518, 1);
                branchTarget = true; block_id = 1213;
      break;
    case 1218:  // $251A
      /*$251A*/ CYCLES(0x251a, 3);
      /*$3770*/ CYCLES(0x3770, 51);
      /*$3772*/ ram_poke(0x00b0, 0x9c);
      /*$3776*/ ram_poke(0x00b3, 0x90);
      /*$377A*/ ram_poke(0x00b6, 0x98);
      /*$377E*/ ram_poke(0x00b1, 0x10);
      /*$3782*/ ram_poke(0x00b4, 0x1c);
      /*$3786*/ ram_poke(0x00b7, 0x24);
      /*$378A*/ ram_poke(0x00b2, 0x00);
      /*$378E*/ ram_poke(0x00b5, 0x03);
      /*$3790*/ s_status_not_z = 0x05;
                s_status_n = 0x00;
                s_a = 0x05;
      /*$3792*/ ram_poke(0x00b8, 0x05);
      /*$3794*/ branchTarget = true; block_id = find_block_id_func_t001(0x3794, pop16() + 1);;
      break;
    case 1219:  // $2520
      /*$2520*/ CYCLES(0x2520, 14);
      /*$2522*/ ram_poke(0x2539, 0x00);
      /*$2527*/ ram_poke(0x253a, 0x77);
      /*$252A*/ s_x = 0x78;
                block_id = 1220;
      break;
    case 1220:  // $252C
      /*$252C*/ CYCLES(0x252c, 16);
                tmp3_U8 = s_x;
      /*$252F*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp3_U8)));
      /*$2534*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp3_U8)));
      /*$2536*/ s_y = 0x00;
                block_id = 1221;
      break;
    case 1221:  // $2538
      /*$2538*/ CYCLES(0x2538, 18);
                tmp3_U8 = peek(ram_peek16(0x2539));
                s_a = tmp3_U8;
      /*$253B*/ poke((ram_peek16al(0x0000) + s_y), tmp3_U8);
      /*$253D*/ tmp3_U8 = (uint8_t)(ram_peek(0x2539) + 0x01);
                ram_poke(0x2539, tmp3_U8);
      /*$2540*/ branchTarget = true; block_id = tmp3_U8 ? 1222 : 1223;
      break;
    case 1222:  // $2540
      /*$2540*/ CYCLES_EDGE(0x2540, 1);
                branchTarget = true; block_id = 1224;
      break;
    case 1223:  // $2542
      /*$2542*/ CYCLES(0x2542, 6);
                ram_poke(0x253a, (uint8_t)(ram_peek(0x253a) + 0x01));
                block_id = 1224;
      break;
    case 1224:  // $2545
      /*$2545*/ CYCLES(0x2545, 6);
                tmp3_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp3_U8;
      /*$2548*/ branchTarget = true; block_id = !(tmp3_U8 >= 0x28) ? 1225 : 1226;
      break;
    case 1225:  // $2548
      /*$2548*/ CYCLES_EDGE(0x2548, 1);
                branchTarget = true; block_id = 1221;
      break;
    case 1226:  // $254A
      /*$254A*/ CYCLES(0x254a, 6);
                tmp3_U8 = s_x;
                tmp2_U8 = (uint8_t)(tmp3_U8 + 0x01);
                s_x = tmp2_U8;
      /*$254B*/ s_status_not_z = (tmp2_U8 != 0xa0);
                tmp2_U8 = tmp2_U8 >= 0xa0;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp3_U8 - 0x9f) & 0x80);
      /*$254D*/ branchTarget = true; block_id = !tmp2_U8 ? 1227 : 1228;
      break;
    case 1227:  // $254D
      /*$254D*/ CYCLES_EDGE(0x254d, 1);
                branchTarget = true; block_id = 1220;
      break;
    case 1228:  // $254F
      /*$254F*/ CYCLES(0x254f, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x254f, pop16() + 1);;
      break;
    case 1229:  // $2550
      /*$2550*/ CYCLES(0x2550, 10);
      /*$2552*/ ram_poke(0x0006, 0x77);
      /*$2556*/ ram_poke(0x0007, 0x9e);
                block_id = 1230;
      break;
    case 1230:  // $2558
      /*$2558*/ CYCLES(0x2558, 3);
                s_x = ram_peek(0x0007);
                block_id = 1231;
      break;
    case 1231:  // $255A
      /*$255A*/ CYCLES(0x255a, 32);
                tmp2_U8 = s_x;
      /*$255D*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp2_U8)));
      /*$2562*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp2_U8)));
      /*$2564*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_x = tmp2_U8;
      /*$2568*/ ram_poke(0x0002, ram_peek((0x0d00 + tmp2_U8)));
      /*$256D*/ ram_poke(0x0003, ram_peek((0x0c00 + tmp2_U8)));
      /*$256F*/ s_y = 0x13;
                block_id = 1232;
      break;
    case 1232:  // $2571
      /*$2571*/ CYCLES(0x2571, 17);
                tmp2_U8 = s_y;
                tmp3_U8 = peek((ram_peek16al(0x0002) + tmp2_U8));
      /*$2573*/ poke((ram_peek16al(0x0000) + tmp2_U8), tmp3_U8);
      /*$2575*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_y = tmp2_U8;
      /*$2578*/ branchTarget = true; block_id = !(tmp2_U8 >= 0x1e) ? 1233 : 1234;
      break;
    case 1233:  // $2578
      /*$2578*/ CYCLES_EDGE(0x2578, 1);
                branchTarget = true; block_id = 1232;
      break;
    case 1234:  // $257A
      /*$257A*/ CYCLES(0x257a, 5);
      /*$257C*/ branchTarget = true; block_id = (s_x != ram_peek(0x0006)) ? 1235 : 1236;
      break;
    case 1235:  // $257C
      /*$257C*/ CYCLES_EDGE(0x257c, 1);
                branchTarget = true; block_id = 1231;
      break;
    case 1236:  // $257E
      /*$257E*/ CYCLES(0x257e, 13);
                ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) + 0x01));
      /*$2582*/ func_2770(0x2584);
      /*$2585*/ CYCLES(0x2585, 7);
                tmp2_U8 = ram_peek(0x0006);
                s_a = tmp2_U8;
      /*$2587*/ s_status_not_z = (tmp2_U8 != 0x89);
                tmp3_U8 = tmp2_U8 >= 0x89;
                s_status_c = tmp3_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - 0x89) & 0x80);
      /*$2589*/ branchTarget = true; block_id = !tmp3_U8 ? 1237 : 1238;
      break;
    case 1237:  // $2589
      /*$2589*/ CYCLES_EDGE(0x2589, 1);
                branchTarget = true; block_id = 1230;
      break;
    case 1238:  // $258B
      /*$258B*/ CYCLES(0x258b, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x258b, pop16() + 1);;
      break;
    case 1239:  // $2590
      /*$2590*/ CYCLES(0x2590, 16);
      /*$2592*/ ram_poke(0x0004, 0x00);
      /*$2596*/ ram_poke(0x0005, 0xa8);
      /*$2598*/ func_2500(0x259a);
      /*$259B*/ CYCLES(0x259b, 6);
                func_2520(0x259d);
      /*$259E*/ CYCLES(0x259e, 18);
      /*$25A3*/ ram_poke(0x0002, ram_peek(0x0d9f));
      /*$25A8*/ ram_poke(0x0003, ram_peek(0x0c9f));
      /*$25AA*/ s_x = 0xa0;
                block_id = 1240;
      break;
    case 1240:  // $25AC
      /*$25AC*/ CYCLES(0x25ac, 16);
                tmp3_U8 = s_x;
      /*$25AF*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp3_U8)));
      /*$25B4*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp3_U8)));
      /*$25B6*/ s_y = 0x27;
                block_id = 1241;
      break;
    case 1241:  // $25B8
      /*$25B8*/ CYCLES(0x25b8, 15);
                tmp3_U8 = s_y;
                tmp2_U8 = peek((ram_peek16al(0x0002) + tmp3_U8));
                s_a = tmp2_U8;
      /*$25BA*/ poke((ram_peek16al(0x0000) + tmp3_U8), tmp2_U8);
      /*$25BC*/ tmp3_U8 = (uint8_t)(tmp3_U8 - 0x01);
                s_y = tmp3_U8;
      /*$25BD*/ branchTarget = true; block_id = !(tmp3_U8 & 0x80) ? 1242 : 1243;
      break;
    case 1242:  // $25BD
      /*$25BD*/ CYCLES_EDGE(0x25bd, 1);
                branchTarget = true; block_id = 1241;
      break;
    case 1243:  // $25BF
      /*$25BF*/ CYCLES(0x25bf, 6);
                tmp3_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp3_U8;
      /*$25C2*/ branchTarget = true; block_id = !(tmp3_U8 >= 0xa7) ? 1244 : 1245;
      break;
    case 1244:  // $25C2
      /*$25C2*/ CYCLES_EDGE(0x25c2, 1);
                branchTarget = true; block_id = 1240;
      break;
    case 1245:  // $25C4
      /*$25C4*/ CYCLES(0x25c4, 6);
                func_2550(0x25c6);
      /*$25C7*/ CYCLES(0x25c7, 6);
                func_2680(0x25c9);
      /*$25CA*/ CYCLES(0x25ca, 6);
                func_2750(0x25cc);
      /*$25CD*/ CYCLES(0x25cd, 6);
                func_2750(0x25cf);
      /*$25D0*/ CYCLES(0x25d0, 6);
                func_2750(0x25d2);
      /*$25D3*/ CYCLES(0x25d3, 6);
                func_2750(0x25d5);
      /*$25D6*/ CYCLES(0x25d6, 6);
                func_26c0(0x25d8);
      /*$25D9*/ CYCLES(0x25d9, 6);
                func_2800(0x25db);
      /*$25DC*/ CYCLES(0x25dc, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x25dc, pop16() + 1);;
      break;
    case 1246:  // $2600
      /*$2600*/ CYCLES(0x2600, 2);
                s_x = 0x8b;
                block_id = 1247;
      break;
    case 1247:  // $2602
      /*$2602*/ CYCLES(0x2602, 17);
                tmp3_U8 = s_x;
      /*$2605*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp3_U8)));
      /*$260A*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp3_U8)));
      /*$260C*/ s_y = ram_peek(0x0006);
                block_id = 1248;
      break;
    case 1248:  // $260E
      /*$260E*/ CYCLES(0x260e, 23);
                tmp3_U8 = peek(ram_peek16(0x260f));
                s_a = tmp3_U8;
      /*$2611*/ tmp2_U8 = s_y;
                poke((ram_peek16al(0x0000) + tmp2_U8), tmp3_U8);
      /*$2613*/ ram_poke(0x260f, (uint8_t)(ram_peek(0x260f) + 0x01));
      /*$2616*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_y = tmp2_U8;
      /*$2619*/ branchTarget = true; block_id = !(tmp2_U8 >= ram_peek(0x0007)) ? 1249 : 1250;
      break;
    case 1249:  // $2619
      /*$2619*/ CYCLES_EDGE(0x2619, 1);
                branchTarget = true; block_id = 1248;
      break;
    case 1250:  // $261B
      /*$261B*/ CYCLES(0x261b, 6);
                tmp2_U8 = s_x;
                tmp3_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_x = tmp3_U8;
      /*$261C*/ s_status_not_z = (tmp3_U8 != 0x9f);
                tmp3_U8 = tmp3_U8 >= 0x9f;
                s_status_c = tmp3_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - 0x9e) & 0x80);
      /*$261E*/ branchTarget = true; block_id = !tmp3_U8 ? 1251 : 1252;
      break;
    case 1251:  // $261E
      /*$261E*/ CYCLES_EDGE(0x261e, 1);
                branchTarget = true; block_id = 1247;
      break;
    case 1252:  // $2620
      /*$2620*/ CYCLES(0x2620, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x2620, pop16() + 1);;
      break;
    case 1253:  // $2680
      /*$2680*/ CYCLES(0x2680, 5);
      /*$2682*/ ram_poke(0x0008, 0x00);
                block_id = 1254;
      break;
    case 1254:  // $2684
      /*$2684*/ CYCLES(0x2684, 39);
                tmp3_U8 = ram_peek(0x0008);
                s_x = tmp3_U8;
      /*$2689*/ ram_poke(0x260f, ram_peek((0x2630 + tmp3_U8)));
      /*$268F*/ ram_poke(0x2610, ram_peek((0x2638 + tmp3_U8)));
      /*$2692*/ tmp3_U8 = ram_peek((0x2640 + tmp3_U8));
                s_a = tmp3_U8;
      /*$2695*/ ram_poke(0x0006, tmp3_U8);
      /*$2697*/ s_status_c = 0x00;
      /*$2698*/ block_id = s_status_d ? 1256 : 1255;
      break;
    case 1255:  // $2698
      /*$2698*/ s_a = (uint8_t)((s_a + 0x0003) + s_status_c);
                block_id = 1257;
      break;
    case 1256:  // $2698
      /*$2698*/ s_a = ((uint8_t)adc_dec16(s_a, 0x03, s_status_c));
                block_id = 1257;
      break;
    case 1257:  // $269A
      /*$269A*/ ram_poke(0x0007, s_a);
      /*$269C*/ func_2600(0x269e);
      /*$269F*/ CYCLES(0x269f, 8);
      /*$26A1*/ func_2790(0x26a3);
      /*$26A4*/ CYCLES(0x26a4, 12);
                ram_poke(0x0008, (uint8_t)(ram_peek(0x0008) + 0x01));
      /*$26A6*/ tmp3_U8 = ram_peek(0x0008);
                s_a = tmp3_U8;
      /*$26A8*/ s_status_not_z = (tmp3_U8 != 0x05);
                tmp2_U8 = tmp3_U8 >= 0x05;
                s_status_c = tmp2_U8;
                s_status_n = ((uint8_t)(tmp3_U8 - 0x05) & 0x80);
      /*$26AA*/ branchTarget = true; block_id = !tmp2_U8 ? 1258 : 1259;
      break;
    case 1258:  // $26AA
      /*$26AA*/ CYCLES_EDGE(0x26aa, 1);
                branchTarget = true; block_id = 1254;
      break;
    case 1259:  // $26AC
      /*$26AC*/ CYCLES(0x26ac, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x26ac, pop16() + 1);;
      break;
    case 1260:  // $26C0
      /*$26C0*/ CYCLES(0x26c0, 5);
      /*$26C2*/ ram_poke(0x0008, 0x03);
                block_id = 1261;
      break;
    case 1261:  // $26C4
      /*$26C4*/ CYCLES(0x26c4, 39);
                tmp2_U8 = ram_peek(0x0008);
      /*$26C9*/ ram_poke(0x260f, ram_peek((0x2630 + tmp2_U8)));
      /*$26CF*/ ram_poke(0x2610, ram_peek((0x2638 + tmp2_U8)));
      /*$26D2*/ tmp2_U8 = ram_peek((0x2640 + tmp2_U8));
                s_a = tmp2_U8;
      /*$26D5*/ ram_poke(0x0006, tmp2_U8);
      /*$26D7*/ s_status_c = 0x00;
      /*$26D8*/ block_id = s_status_d ? 1263 : 1262;
      break;
    case 1262:  // $26D8
      /*$26D8*/ s_a = (uint8_t)((s_a + 0x0003) + s_status_c);
                block_id = 1264;
      break;
    case 1263:  // $26D8
      /*$26D8*/ s_a = ((uint8_t)adc_dec16(s_a, 0x03, s_status_c));
                block_id = 1264;
      break;
    case 1264:  // $26DA
      /*$26DA*/ ram_poke(0x0007, s_a);
      /*$26DC*/ func_2600(0x26de);
      /*$26DF*/ CYCLES(0x26df, 2);
                s_x = 0x8b;
                block_id = 1265;
      break;
    case 1265:  // $26E1
      /*$26E1*/ CYCLES(0x26e1, 31);
                tmp2_U8 = s_x;
      /*$26E4*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp2_U8)));
      /*$26E9*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp2_U8)));
      /*$26EB*/ tmp3_U8 = ram_peek(0x0007);
                s_y = tmp3_U8;
      /*$26EF*/ poke((ram_peek16al(0x0000) + tmp3_U8), 0x00);
      /*$26F1*/ tmp2_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_x = tmp2_U8;
      /*$26F4*/ branchTarget = true; block_id = !(tmp2_U8 >= 0x9f) ? 1266 : 1267;
      break;
    case 1266:  // $26F4
      /*$26F4*/ CYCLES_EDGE(0x26f4, 1);
                branchTarget = true; block_id = 1265;
      break;
    case 1267:  // $26F6
      /*$26F6*/ CYCLES(0x26f6, 8);
                s_a = 0x00;
      /*$26F8*/ func_2790(0x26fa);
      /*$26FB*/ CYCLES(0x26fb, 7);
                tmp2_U8 = (uint8_t)(ram_peek(0x0008) - 0x01);
                s_status_not_z = tmp2_U8;
                tmp3_U8 = tmp2_U8 & 0x80;
                s_status_n = tmp3_U8;
                ram_poke(0x0008, tmp2_U8);
      /*$26FD*/ branchTarget = true; block_id = !tmp3_U8 ? 1268 : 1269;
      break;
    case 1268:  // $26FD
      /*$26FD*/ CYCLES_EDGE(0x26fd, 1);
                branchTarget = true; block_id = 1261;
      break;
    case 1269:  // $26FF
      /*$26FF*/ CYCLES(0x26ff, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x26ff, pop16() + 1);;
      break;
    case 1270:  // $2700
      /*$2700*/ CYCLES(0x2700, 2);
                s_y = 0x03;
                block_id = 1271;
      break;
    case 1271:  // $2702
      /*$2702*/ CYCLES(0x2702, 14);
                tmp3_U8 = s_y;
                tmp2_U8 = peek((ram_peek16al(0x000a) + tmp3_U8));
                s_a = tmp2_U8;
      /*$2704*/ ram_poke(tmp3_U8, tmp2_U8);
      /*$2707*/ tmp3_U8 = (uint8_t)(tmp3_U8 - 0x01);
                s_y = tmp3_U8;
      /*$2708*/ branchTarget = true; block_id = !(tmp3_U8 & 0x80) ? 1272 : 1273;
      break;
    case 1272:  // $2708
      /*$2708*/ CYCLES_EDGE(0x2708, 1);
                branchTarget = true; block_id = 1271;
      break;
    case 1273:  // $270A
      /*$270A*/ CYCLES(0x270a, 6);
                s_y = ram_peek(0x0000);
      /*$270C*/ s_x = ram_peek(0x0001);
                block_id = 1274;
      break;
    case 1274:  // $270E
      /*$270E*/ CYCLES(0x270e, 6);
                func_2745(0x2710);
      /*$2711*/ CYCLES(0x2711, 4);
                tmp3_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp3_U8;
      /*$2712*/ branchTarget = true; block_id = tmp3_U8 ? 1275 : 1276;
      break;
    case 1275:  // $2712
      /*$2712*/ CYCLES_EDGE(0x2712, 1);
                branchTarget = true; block_id = 1274;
      break;
    case 1276:  // $2714
      /*$2714*/ CYCLES(0x2714, 4);
                tmp3_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp3_U8;
      /*$2715*/ branchTarget = true; block_id = tmp3_U8 ? 1277 : 1278;
      break;
    case 1277:  // $2715
      /*$2715*/ CYCLES_EDGE(0x2715, 1);
                branchTarget = true; block_id = 1274;
      break;
    case 1278:  // $2717
      /*$2717*/ CYCLES(0x2717, 10);
                poke(ram_peek16al(0x2718), s_a);
      /*$271A*/ s_y = ram_peek(0x0002);
      /*$271C*/ s_x = ram_peek(0x0003);
                block_id = 1279;
      break;
    case 1279:  // $271E
      /*$271E*/ CYCLES(0x271e, 6);
                func_2745(0x2720);
      /*$2721*/ CYCLES(0x2721, 4);
                tmp3_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp3_U8;
      /*$2722*/ branchTarget = true; block_id = tmp3_U8 ? 1280 : 1281;
      break;
    case 1280:  // $2722
      /*$2722*/ CYCLES_EDGE(0x2722, 1);
                branchTarget = true; block_id = 1279;
      break;
    case 1281:  // $2724
      /*$2724*/ CYCLES(0x2724, 4);
                tmp3_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp3_U8;
      /*$2725*/ branchTarget = true; block_id = tmp3_U8 ? 1282 : 1283;
      break;
    case 1282:  // $2725
      /*$2725*/ CYCLES_EDGE(0x2725, 1);
                branchTarget = true; block_id = 1279;
      break;
    case 1283:  // $2727
      /*$2727*/ CYCLES(0x2727, 16);
                poke(ram_peek16al(0x2728), s_a);
      /*$272A*/ s_a = ram_peek(0x000a);
      /*$272C*/ s_status_c = 0x00;
      /*$272D*/ block_id = s_status_d ? 1285 : 1284;
      break;
    case 1284:  // $272D
      /*$272D*/ tmp5_U16 = s_a;
                tmp6_U16 = (tmp5_U16 + 0x0004) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp5_U16, (uint8_t)0x0004);
                tmp3_U8 = (uint8_t)tmp6_U16;
                s_status_not_z = tmp3_U8;
                s_a = tmp3_U8;
                block_id = 1286;
      break;
    case 1285:  // $272D
      /*$272D*/ tmp6_U16 = adc_dec16(s_a, 0x04, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp3_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_not_z = (~tmp3_U8 & 2);
                s_status_v = ((tmp3_U8 & 0x40) != 0);
                block_id = 1286;
      break;
    case 1286:  // $272F
      /*$272F*/ ram_poke(0x000a, s_a);
      /*$2731*/ branchTarget = true; block_id = s_status_not_z ? 1287 : 1288;
      break;
    case 1287:  // $2731
      /*$2731*/ CYCLES_EDGE(0x2731, 1);
                branchTarget = true; block_id = 1289;
      break;
    case 1288:  // $2733
      /*$2733*/ CYCLES(0x2733, 5);
                ram_poke(0x000b, (uint8_t)(ram_peek(0x000b) + 0x01));
                block_id = 1289;
      break;
    case 1289:  // $2735
      /*$2735*/ CYCLES(0x2735, 8);
      /*$2739*/ branchTarget = true; block_id = (ram_peek(0x000a) != ram_peek(0x0004)) ? 1290 : 1291;
      break;
    case 1290:  // $2739
      /*$2739*/ CYCLES_EDGE(0x2739, 1);
                branchTarget = true; block_id = 1270;
      break;
    case 1291:  // $273B
      /*$273B*/ CYCLES(0x273b, 8);
                tmp3_U8 = ram_peek(0x000b);
                s_a = tmp3_U8;
      /*$273D*/ tmp2_U8 = ram_peek(0x0005);
                tmp1_U8 = tmp3_U8 != tmp2_U8;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp3_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp3_U8 - tmp2_U8) & 0x80);
      /*$273F*/ branchTarget = true; block_id = tmp1_U8 ? 1292 : 1293;
      break;
    case 1292:  // $273F
      /*$273F*/ CYCLES_EDGE(0x273f, 1);
                branchTarget = true; block_id = 1270;
      break;
    case 1293:  // $2741
      /*$2741*/ CYCLES(0x2741, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x2741, pop16() + 1);;
      break;
    case 1294:  // $2745
      /*$2745*/ CYCLES(0x2745, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x2745, pop16() + 1);;
      break;
    case 1295:  // $2750
      /*$2750*/ CYCLES(0x2750, 23);
      /*$2752*/ ram_poke(0x000a, 0x00);
      /*$2756*/ ram_poke(0x000b, 0x96);
      /*$275A*/ ram_poke(0x0004, 0xa0);
      /*$275E*/ ram_poke(0x0005, 0x98);
      /*$2760*/ branchTarget = true; block_id = 1270;
      break;
    case 1296:  // $2770
      /*$2770*/ CYCLES(0x2770, 23);
      /*$2772*/ ram_poke(0x000a, 0xa0);
      /*$2776*/ ram_poke(0x000b, 0x98);
      /*$277A*/ ram_poke(0x0004, 0xb0);
      /*$277E*/ ram_poke(0x0005, 0x98);
      /*$2780*/ branchTarget = true; block_id = 1270;
      break;
    case 1297:  // $2790
      /*$2790*/ CYCLES(0x2790, 23);
      /*$2792*/ ram_poke(0x000a, 0xa0);
      /*$2796*/ ram_poke(0x000b, 0x98);
      /*$279A*/ ram_poke(0x0004, 0x00);
      /*$279E*/ ram_poke(0x0005, 0x99);
      /*$27A0*/ branchTarget = true; block_id = 1270;
      break;
    case 1298:  // $2800
      /*$2800*/ CYCLES(0x2800, 10);
      /*$2802*/ ram_poke(0x0006, 0x77);
      /*$2806*/ ram_poke(0x0007, 0x9f);
                block_id = 1299;
      break;
    case 1299:  // $2808
      /*$2808*/ CYCLES(0x2808, 3);
                s_x = ram_peek(0x0006);
                block_id = 1300;
      break;
    case 1300:  // $280A
      /*$280A*/ CYCLES(0x280a, 32);
                tmp1_U8 = s_x;
      /*$280D*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
      /*$2812*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
      /*$2814*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_x = tmp1_U8;
      /*$2818*/ ram_poke(0x0002, ram_peek((0x0d00 + tmp1_U8)));
      /*$281D*/ ram_poke(0x0003, ram_peek((0x0c00 + tmp1_U8)));
      /*$281F*/ s_y = 0x13;
                block_id = 1301;
      break;
    case 1301:  // $2821
      /*$2821*/ CYCLES(0x2821, 17);
                tmp1_U8 = s_y;
                tmp2_U8 = peek((ram_peek16al(0x0000) + tmp1_U8));
      /*$2823*/ poke((ram_peek16al(0x0002) + tmp1_U8), tmp2_U8);
      /*$2825*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
                s_y = tmp1_U8;
      /*$2828*/ branchTarget = true; block_id = !(tmp1_U8 >= 0x1e) ? 1302 : 1303;
      break;
    case 1302:  // $2828
      /*$2828*/ CYCLES_EDGE(0x2828, 1);
                branchTarget = true; block_id = 1301;
      break;
    case 1303:  // $282A
      /*$282A*/ CYCLES(0x282a, 9);
      /*$282B*/ tmp1_U8 = (uint8_t)(s_x + 0x02);
                s_x = tmp1_U8;
      /*$282E*/ branchTarget = true; block_id = (tmp1_U8 != ram_peek(0x0007)) ? 1304 : 1305;
      break;
    case 1304:  // $282E
      /*$282E*/ CYCLES_EDGE(0x282e, 1);
                branchTarget = true; block_id = 1300;
      break;
    case 1305:  // $2830
      /*$2830*/ CYCLES(0x2830, 11);
                ram_poke(0x0007, (uint8_t)(ram_peek(0x0007) - 0x01));
      /*$2832*/ func_2770(0x2834);
      /*$2835*/ CYCLES(0x2835, 7);
                tmp1_U8 = ram_peek(0x0007);
                s_a = tmp1_U8;
      /*$2837*/ tmp2_U8 = tmp1_U8 != 0x8c;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp1_U8 >= 0x8c);
                s_status_n = ((uint8_t)(tmp1_U8 - 0x8c) & 0x80);
      /*$2839*/ branchTarget = true; block_id = tmp2_U8 ? 1306 : 1307;
      break;
    case 1306:  // $2839
      /*$2839*/ CYCLES_EDGE(0x2839, 1);
                branchTarget = true; block_id = 1299;
      break;
    case 1307:  // $283B
      /*$283B*/ CYCLES(0x283b, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x283b, pop16() + 1);;
      break;
    case 1308:  // $2851
      /*$2851*/ s_a = (uint8_t)((s_a + 0x0001) + s_status_c);
                block_id = 1310;
      break;
    case 1309:  // $2851
      /*$2851*/ s_a = ((uint8_t)adc_dec16(s_a, 0x01, s_status_c));
                block_id = 1310;
      break;
    case 1310:  // $2853
      /*$2855*/ ram_poke(0x0087, (s_a & 0x07));
      /*$2857*/ ram_poke(0x0086, (uint8_t)(ram_peek(0x0086) + 0x01));
      /*$285D*/ branchTarget = true; block_id = !(ram_peek(0x0086) >= 0x04) ? 1311 : 1312;
      break;
    case 1311:  // $285D
      /*$285D*/ CYCLES_EDGE(0x285d, 1);
                branchTarget = true; block_id = 1313;
      break;
    case 1312:  // $285F
      /*$285F*/ CYCLES(0x285f, 5);
      /*$2861*/ ram_poke(0x0086, 0x03);
                block_id = 1313;
      break;
    case 1313:  // $2863
      /*$2863*/ CYCLES(0x2863, 13);
      /*$2865*/ ram_poke(0x0088, 0x20);
      /*$2869*/ ram_poke(0x00a0, 0x00);
      /*$286B*/ branchTarget = true; block_id = 1005;
      break;
    case 1314:  // $2880
      /*$2880*/ CYCLES(0x2880, 15);
      /*$2882*/ ram_poke(0x2899, 0x00);
      /*$2887*/ ram_poke(0x289a, 0x6e);
      /*$288A*/ s_x = ram_peek(0x0004);
                block_id = 1315;
      break;
    case 1315:  // $288C
      /*$288C*/ CYCLES(0x288c, 17);
                tmp2_U8 = s_x;
      /*$288F*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp2_U8)));
      /*$2894*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp2_U8)));
      /*$2896*/ s_y = ram_peek(0x0006);
                block_id = 1316;
      break;
    case 1316:  // $2898
      /*$2898*/ CYCLES(0x2898, 18);
                tmp2_U8 = peek(ram_peek16(0x2899));
                s_a = tmp2_U8;
      /*$289B*/ poke((ram_peek16al(0x0000) + s_y), tmp2_U8);
      /*$289D*/ tmp2_U8 = (uint8_t)(ram_peek(0x2899) + 0x01);
                ram_poke(0x2899, tmp2_U8);
      /*$28A0*/ branchTarget = true; block_id = tmp2_U8 ? 1317 : 1318;
      break;
    case 1317:  // $28A0
      /*$28A0*/ CYCLES_EDGE(0x28a0, 1);
                branchTarget = true; block_id = 1319;
      break;
    case 1318:  // $28A2
      /*$28A2*/ CYCLES(0x28a2, 6);
                ram_poke(0x289a, (uint8_t)(ram_peek(0x289a) + 0x01));
                block_id = 1319;
      break;
    case 1319:  // $28A5
      /*$28A5*/ CYCLES(0x28a5, 7);
                tmp2_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp2_U8;
      /*$28A8*/ branchTarget = true; block_id = !(tmp2_U8 >= ram_peek(0x0007)) ? 1320 : 1321;
      break;
    case 1320:  // $28A8
      /*$28A8*/ CYCLES_EDGE(0x28a8, 1);
                branchTarget = true; block_id = 1316;
      break;
    case 1321:  // $28AA
      /*$28AA*/ CYCLES(0x28aa, 11);
                poke(ram_peek16(0x28ab), s_a);
      /*$28AD*/ tmp2_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp2_U8;
      /*$28AE*/ tmp1_U8 = ram_peek(0x0005);
                s_status_not_z = (tmp2_U8 != tmp1_U8);
                tmp3_U8 = tmp2_U8 >= tmp1_U8;
                s_status_c = tmp3_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
      /*$28B0*/ branchTarget = true; block_id = !tmp3_U8 ? 1322 : 1323;
      break;
    case 1322:  // $28B0
      /*$28B0*/ CYCLES_EDGE(0x28b0, 1);
                branchTarget = true; block_id = 1315;
      break;
    case 1323:  // $28B2
      /*$28B2*/ CYCLES(0x28b2, 10);
                poke(ram_peek16(0x28b3), s_a);
      /*$28B5*/ branchTarget = true; block_id = find_block_id_func_t001(0x28b5, pop16() + 1);;
      break;
    case 1324:  // $28D0
      /*$28D0*/ CYCLES(0x28d0, 31);
      /*$28D2*/ ram_poke(0x0004, ram_peek(0x0090));
      /*$28D6*/ ram_poke(0x0005, ram_peek(0x0091));
      /*$28D8*/ tmp3_U8 = ram_peek(0x0093);
                s_a = tmp3_U8;
      /*$28DA*/ ram_poke(0x0006, tmp3_U8);
      /*$28DC*/ s_status_c = 0x00;
      /*$28DD*/ block_id = s_status_d ? 1326 : 1325;
      break;
    case 1325:  // $28DD
      /*$28DD*/ tmp6_U16 = s_a;
                tmp5_U16 = (tmp6_U16 + 0x0006) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp6_U16, (uint8_t)0x0006);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 1327;
      break;
    case 1326:  // $28DD
      /*$28DD*/ tmp5_U16 = adc_dec16(s_a, 0x06, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
                block_id = 1327;
      break;
    case 1327:  // $28DF
      /*$28DF*/ ram_poke(0x0007, s_a);
      /*$28E1*/ func_2880(0x28e3);
      /*$28E4*/ CYCLES(0x28e4, 13);
                ram_poke(0x0090, (uint8_t)(ram_peek(0x0090) - 0x01));
      /*$28E6*/ s_a = 0x40;
      /*$28E8*/ rom_mon_wait(0x28ea);
      /*$28EB*/ CYCLES(0x28eb, 7);
                tmp3_U8 = (uint8_t)(ram_peek(0x0092) - 0x01);
                s_status_not_z = tmp3_U8;
                s_status_n = (tmp3_U8 & 0x80);
                ram_poke(0x0092, tmp3_U8);
      /*$28ED*/ branchTarget = true; block_id = tmp3_U8 ? 1328 : 1329;
      break;
    case 1328:  // $28ED
      /*$28ED*/ CYCLES_EDGE(0x28ed, 1);
                branchTarget = true; block_id = 1324;
      break;
    case 1329:  // $28EF
      /*$28EF*/ CYCLES(0x28ef, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x28ef, pop16() + 1);;
      break;
    case 1330:  // $2900
      /*$2900*/ CYCLES(0x2900, 23);
      /*$2902*/ ram_poke(0x0090, 0x99);
      /*$2906*/ ram_poke(0x0091, 0x9a);
      /*$290A*/ ram_poke(0x0092, 0x13);
      /*$290E*/ ram_poke(0x0093, 0x04);
      /*$2910*/ branchTarget = true; block_id = 1324;
      break;
    case 1331:  // $2940
      /*$2940*/ CYCLES(0x2940, 6);
                branchTarget = true; push16(0x2942); block_id = 208;
      break;
    case 1332:  // $2943
      /*$2943*/ CYCLES(0x2943, 8);
                s_a = 0x70;
      /*$2945*/ rom_mon_wait(0x2947);
      /*$2948*/ CYCLES(0x2948, 7);
      /*$294C*/ branchTarget = true; block_id = (ram_peek(0x00e0) >= 0x70) ? 1333 : 1334;
      break;
    case 1333:  // $294C
      /*$294C*/ CYCLES_EDGE(0x294c, 1);
                branchTarget = true; block_id = 1331;
      break;
    case 1334:  // $294E
      /*$294E*/ CYCLES(0x294e, 6);
                func_2900(0x2950);
                branchTarget = true; block_id = 1335;
      break;
    case 1335:  // $2951
      /*$2951*/ CYCLES(0x2951, 6);
                branchTarget = true; push16(0x2953); block_id = 208;
      break;
    case 1336:  // $2954
      /*$2954*/ CYCLES(0x2954, 8);
                s_a = 0x70;
      /*$2956*/ rom_mon_wait(0x2958);
      /*$2959*/ CYCLES(0x2959, 7);
      /*$295D*/ branchTarget = true; block_id = !(ram_peek(0x00e0) >= 0x78) ? 1337 : 1338;
      break;
    case 1337:  // $295D
      /*$295D*/ CYCLES_EDGE(0x295d, 1);
                branchTarget = true; block_id = 1335;
      break;
    case 1338:  // $295F
      /*$295F*/ CYCLES(0x295f, 25);
                s_a = ram_peek(0x0093);
      /*$2961*/ s_status_c = 0x00;
      /*$2962*/ block_id = s_status_d ? 1340 : 1339;
      break;
    case 1339:  // $2962
      /*$2962*/ s_a = (uint8_t)((s_a + 0x0003) + s_status_c);
                block_id = 1341;
      break;
    case 1340:  // $2962
      /*$2962*/ s_a = ((uint8_t)adc_dec16(s_a, 0x03, s_status_c));
                block_id = 1341;
      break;
    case 1341:  // $2964
      /*$2964*/ ram_poke(0x0000, s_a);
      /*$2966*/ s_status_c = 0x00;
      /*$2967*/ block_id = s_status_d ? 1343 : 1342;
      break;
    case 1342:  // $2967
      /*$2967*/ tmp5_U16 = s_a;
                tmp6_U16 = (tmp5_U16 + 0x000a) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp5_U16, (uint8_t)0x000a);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1344;
      break;
    case 1343:  // $2967
      /*$2967*/ tmp6_U16 = adc_dec16(s_a, 0x0a, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 1344;
      break;
    case 1344:  // $2969
      /*$2969*/ ram_poke(0x0001, s_a);
      /*$296B*/ tmp3_U8 = ram_peek(0x00e1);
                s_a = tmp3_U8;
      /*$296F*/ branchTarget = true; block_id = !(tmp3_U8 >= ram_peek(0x0000)) ? 1345 : 1346;
      break;
    case 1345:  // $296F
      /*$296F*/ CYCLES_EDGE(0x296f, 1);
                branchTarget = true; block_id = 1335;
      break;
    case 1346:  // $2971
      /*$2971*/ CYCLES(0x2971, 5);
      /*$2973*/ branchTarget = true; block_id = (s_a >= ram_peek(0x0001)) ? 1347 : 1348;
      break;
    case 1347:  // $2973
      /*$2973*/ CYCLES_EDGE(0x2973, 1);
                branchTarget = true; block_id = 1335;
      break;
    case 1348:  // $2975
      /*$2975*/ CYCLES(0x2975, 3);
      /*$2840*/ CYCLES(0x2840, 6);
                func_2590(0x2842);
      /*$2843*/ CYCLES(0x2843, 16);
      /*$2845*/ ram_poke(0x008a, 0x27);
      /*$2849*/ ram_poke(0x008b, 0x40);
      /*$284B*/ func_1e00(0x284d);
      /*$284E*/ CYCLES(0x284e, 24);
                s_a = ram_peek(0x0087);
      /*$2850*/ s_status_c = 0x00;
      /*$2851*/ block_id = s_status_d ? 1309 : 1308;
      break;
    case 1349:  // $2A03
      /*$2A03*/ CYCLES(0x2a03, 6);
                tmp3_U8 = io_peek(0xc000);
                s_a = tmp3_U8;
      /*$2A06*/ branchTarget = true; block_id = (tmp3_U8 & 0x80) ? 1350 : 1351;
      break;
    case 1350:  // $2A06
      /*$2A06*/ CYCLES_EDGE(0x2a06, 1);
      /*$2A0E*/ CYCLES(0x2a0e, 4);
                tmp3_U8 = s_a;
                s_status_c = (tmp3_U8 >= 0x8d);
      /*$2A10*/ branchTarget = true; block_id = (tmp3_U8 != 0x8d) ? 1352 : 1353;
      break;
    case 1351:  // $2A08
      /*$2A08*/ CYCLES(0x2a08, 6);
                func_2aa0(0x2a0a);
      /*$2A0B*/ CYCLES(0x2a0b, 3);
                branchTarget = true; block_id = 1349;
      break;
    case 1352:  // $2A10
      /*$2A10*/ CYCLES_EDGE(0x2a10, 1);
      /*$2A15*/ CYCLES(0x2a15, 6);
                tmp3_U8 = s_a;
                ram_poke(0x0002, tmp3_U8);
      /*$2A00*/ CYCLES(0x2a00, 4);
                io_poke(0xc010, tmp3_U8);
                block_id = 1349;
      break;
    case 1353:  // $2A12
      /*$2A12*/ CYCLES(0x2a12, 9);
                tmp3_U8 = ram_peek(0x0002);
                s_status_not_z = tmp3_U8;
                s_status_n = (tmp3_U8 & 0x80);
                s_a = tmp3_U8;
      /*$2A14*/ branchTarget = true; block_id = find_block_id_func_t001(0x2a14, pop16() + 1);;
      break;
    case 1354:  // $2A20
      /*$2A20*/ CYCLES(0x2a20, 23);
      /*$2A22*/ ram_poke(0x0000, 0x17);
      /*$2A26*/ ram_poke(0x0001, 0x0b);
      /*$2A28*/ tmp3_U8 = ram_peek(0x1078);
                s_a = tmp3_U8;
      /*$2A2B*/ ram_poke(0x0002, tmp3_U8);
      /*$2A2D*/ func_2a00(0x2a2f);
      /*$2A30*/ CYCLES(0x2a30, 27);
                ram_poke(0x1078, s_a);
      /*$2A35*/ ram_poke(0x0000, 0x44);
      /*$2A39*/ ram_poke(0x0001, 0x09);
      /*$2A3B*/ tmp3_U8 = ram_peek(0x108a);
                s_a = tmp3_U8;
      /*$2A3E*/ ram_poke(0x0002, tmp3_U8);
      /*$2A40*/ func_2a00(0x2a42);
      /*$2A43*/ CYCLES(0x2a43, 27);
                ram_poke(0x108a, s_a);
      /*$2A48*/ ram_poke(0x0000, 0x3f);
      /*$2A4C*/ ram_poke(0x0001, 0x0b);
      /*$2A4E*/ tmp3_U8 = ram_peek(0x1066);
                s_a = tmp3_U8;
      /*$2A51*/ ram_poke(0x0002, tmp3_U8);
      /*$2A53*/ func_2a00(0x2a55);
      /*$2A56*/ CYCLES(0x2a56, 27);
                ram_poke(0x1066, s_a);
      /*$2A5B*/ ram_poke(0x0000, 0x3a);
      /*$2A5F*/ ram_poke(0x0001, 0x09);
      /*$2A61*/ tmp3_U8 = ram_peek(0x109c);
                s_a = tmp3_U8;
      /*$2A64*/ ram_poke(0x0002, tmp3_U8);
      /*$2A66*/ func_2a00(0x2a68);
      /*$2A69*/ CYCLES(0x2a69, 27);
                ram_poke(0x109c, s_a);
      /*$2A6E*/ ram_poke(0x0000, 0x2c);
      /*$2A72*/ ram_poke(0x0001, 0x0b);
      /*$2A74*/ tmp3_U8 = ram_peek(0x1390);
                s_a = tmp3_U8;
      /*$2A77*/ ram_poke(0x0002, tmp3_U8);
      /*$2A79*/ func_2a00(0x2a7b);
      /*$2A7C*/ CYCLES(0x2a7c, 27);
                ram_poke(0x1390, s_a);
      /*$2A81*/ ram_poke(0x0000, 0xd4);
      /*$2A85*/ ram_poke(0x0001, 0x08);
      /*$2A87*/ tmp3_U8 = ram_peek(0x1306);
                s_status_not_z = tmp3_U8;
                s_status_n = (tmp3_U8 & 0x80);
                s_a = tmp3_U8;
      /*$2A8A*/ ram_poke(0x0002, tmp3_U8);
      /*$2A8C*/ func_2a00(0x2a8e);
      /*$2A8F*/ CYCLES(0x2a8f, 10);
                ram_poke(0x1306, s_a);
      /*$2A92*/ branchTarget = true; block_id = find_block_id_func_t001(0x2a92, pop16() + 1);;
      break;
    case 1355:  // $2AA0
      /*$2AA0*/ CYCLES(0x2aa0, 18);
                s_y = 0x00;
      /*$2AA4*/ poke(ram_peek16al(0x0000), 0xa0);
      /*$2AA6*/ s_a = 0x70;
      /*$2AA8*/ rom_mon_wait(0x2aaa);
      /*$2AAB*/ CYCLES(0x2aab, 17);
      /*$2AAD*/ poke((ram_peek16al(0x0000) + s_y), ram_peek(0x0002));
      /*$2AAF*/ s_status_not_z = 0x70;
                s_status_n = 0x00;
                s_a = 0x70;
      /*$2AB1*/ rom_mon_wait(0x2ab3);
      /*$2AB4*/ CYCLES(0x2ab4, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x2ab4, pop16() + 1);;
      break;
    case 1356:  // $2B02
      /*$2B02*/ CYCLES(0x2b02, 17);
                tmp3_U8 = s_x;
      /*$2B05*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp3_U8)));
      /*$2B07*/ tmp3_U8 = ram_peek((0x0c00 + tmp3_U8));
                s_a = tmp3_U8;
      /*$2B0A*/ ram_poke(0x0001, tmp3_U8);
      /*$2B0C*/ s_y = ram_peek(0x0006);
                block_id = 1357;
      break;
    case 1357:  // $2B0E
      /*$2B0E*/ CYCLES(0x2b0e, 4);
      /*$2B10*/ branchTarget = true; block_id = (s_y >= 0x28) ? 1358 : 1359;
      break;
    case 1358:  // $2B10
      /*$2B10*/ CYCLES_EDGE(0x2b10, 1);
                branchTarget = true; block_id = 1360;
      break;
    case 1359:  // $2B12
      /*$2B12*/ CYCLES(0x2b12, 15);
                tmp3_U8 = peek(ram_peek16(0x2b13));
      /*$2B15*/ tmp1_U8 = s_y;
                tmp2_U8 = peek((ram_peek16al(0x0000) + tmp1_U8));
                tmp2_U8 = tmp3_U8 ^ tmp2_U8;
                s_a = tmp2_U8;
      /*$2B17*/ poke((ram_peek16al(0x0000) + tmp1_U8), tmp2_U8);
                block_id = 1360;
      break;
    case 1360:  // $2B19
      /*$2B19*/ CYCLES(0x2b19, 13);
                ram_poke(0x2b13, (uint8_t)(ram_peek(0x2b13) + 0x01));
      /*$2B1C*/ tmp3_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp3_U8;
      /*$2B1F*/ branchTarget = true; block_id = (tmp3_U8 != ram_peek(0x0007)) ? 1361 : 1362;
      break;
    case 1361:  // $2B1F
      /*$2B1F*/ CYCLES_EDGE(0x2b1f, 1);
                branchTarget = true; block_id = 1357;
      break;
    case 1362:  // $2B21
      /*$2B21*/ CYCLES(0x2b21, 11);
                tmp1_U8 = (uint8_t)(s_x + 0x01);
                s_x = tmp1_U8;
      /*$2B22*/ poke(ram_peek16(0x2b23), s_a);
      /*$2B25*/ tmp2_U8 = ram_peek(0x0005);
                tmp3_U8 = tmp1_U8 != tmp2_U8;
                s_status_not_z = tmp3_U8;
                s_status_c = (tmp1_U8 >= tmp2_U8);
                s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
      /*$2B27*/ branchTarget = true; block_id = tmp3_U8 ? 1363 : 1364;
      break;
    case 1363:  // $2B27
      /*$2B27*/ CYCLES_EDGE(0x2b27, 1);
                branchTarget = true; block_id = 1356;
      break;
    case 1364:  // $2B29
      /*$2B29*/ CYCLES(0x2b29, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x2b29, pop16() + 1);;
      break;
    case 1365:  // $2B30
      /*$2B30*/ CYCLES(0x2b30, 48);
                tmp3_U8 = ram_peek(0x000a);
      /*$2B35*/ ram_poke(0x2b13, ram_peek((0x2b58 + tmp3_U8)));
      /*$2B3B*/ ram_poke(0x2b14, ram_peek((0x2b5c + tmp3_U8)));
      /*$2B3E*/ tmp3_U8 = ram_peek(0x000b);
                s_a = tmp3_U8;
      /*$2B40*/ ram_poke(0x0004, tmp3_U8);
      /*$2B42*/ s_status_c = 0x00;
      /*$2B43*/ block_id = s_status_d ? 1367 : 1366;
      break;
    case 1366:  // $2B43
      /*$2B43*/ s_a = (uint8_t)((s_a + 0x0018) + s_status_c);
                block_id = 1368;
      break;
    case 1367:  // $2B43
      /*$2B43*/ s_a = ((uint8_t)adc_dec16(s_a, 0x18, s_status_c));
                block_id = 1368;
      break;
    case 1368:  // $2B45
      /*$2B45*/ ram_poke(0x0005, s_a);
      /*$2B47*/ tmp3_U8 = ram_peek(0x000c);
                s_a = tmp3_U8;
      /*$2B49*/ ram_poke(0x0006, tmp3_U8);
      /*$2B4B*/ s_status_c = 0x00;
      /*$2B4C*/ block_id = s_status_d ? 1370 : 1369;
      break;
    case 1369:  // $2B4C
      /*$2B4C*/ tmp6_U16 = s_a;
                tmp5_U16 = (tmp6_U16 + 0x0005) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp6_U16, (uint8_t)0x0005);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 1371;
      break;
    case 1370:  // $2B4C
      /*$2B4C*/ tmp5_U16 = adc_dec16(s_a, 0x05, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
                block_id = 1371;
      break;
    case 1371:  // $2B4E
      /*$2B4E*/ ram_poke(0x0007, s_a);
      /*$2B00*/ CYCLES(0x2b00, 3);
                s_x = ram_peek(0x0004);
                block_id = 1356;
      break;
    case 1372:  // $2B60
      /*$2B60*/ CYCLES(0x2b60, 6);
                func_2b30(0x2b62);
                branchTarget = true; block_id = 1373;
      break;
    case 1373:  // $2B63
      /*$2B63*/ CYCLES(0x2b63, 6);
                func_2b70(0x2b65);
                branchTarget = true; block_id = 1374;
      break;
    case 1374:  // $2B66
      /*$2B66*/ CYCLES(0x2b66, 3);
                func_2b30(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x2b66, pop16() + 1);;
      break;
    case 1375:  // $2B70
      /*$2B70*/ CYCLES(0x2b70, 2);
                s_x = 0x10;
                block_id = 1376;
      break;
    case 1376:  // $2B72
      /*$2B72*/ CYCLES(0x2b72, 2);
                s_y = 0xc0;
                block_id = 1377;
      break;
    case 1377:  // $2B74
      /*$2B74*/ CYCLES(0x2b74, 4);
                tmp3_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp3_U8;
      /*$2B75*/ branchTarget = true; block_id = tmp3_U8 ? 1378 : 1379;
      break;
    case 1378:  // $2B75
      /*$2B75*/ CYCLES_EDGE(0x2b75, 1);
                branchTarget = true; block_id = 1377;
      break;
    case 1379:  // $2B77
      /*$2B77*/ CYCLES(0x2b77, 6);
                poke(ram_peek16al(0x2b78), s_a);
      /*$2B7A*/ s_y = 0x80;
                block_id = 1380;
      break;
    case 1380:  // $2B7C
      /*$2B7C*/ CYCLES(0x2b7c, 4);
                tmp3_U8 = (uint8_t)(s_y - 0x01);
                s_y = tmp3_U8;
      /*$2B7D*/ branchTarget = true; block_id = tmp3_U8 ? 1381 : 1382;
      break;
    case 1381:  // $2B7D
      /*$2B7D*/ CYCLES_EDGE(0x2b7d, 1);
                branchTarget = true; block_id = 1380;
      break;
    case 1382:  // $2B7F
      /*$2B7F*/ CYCLES(0x2b7f, 8);
                poke(ram_peek16al(0x2b80), s_a);
      /*$2B82*/ tmp3_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp3_U8;
                s_status_n = (tmp3_U8 & 0x80);
                s_x = tmp3_U8;
      /*$2B83*/ branchTarget = true; block_id = tmp3_U8 ? 1383 : 1384;
      break;
    case 1383:  // $2B83
      /*$2B83*/ CYCLES_EDGE(0x2b83, 1);
                branchTarget = true; block_id = 1376;
      break;
    case 1384:  // $2B85
      /*$2B85*/ CYCLES(0x2b85, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x2b85, pop16() + 1);;
      break;
    case 1385:  // $2C02
      /*$2C02*/ CYCLES(0x2c02, 39);
                tmp3_U8 = s_x;
      /*$2C05*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp3_U8)));
      /*$2C0A*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp3_U8)));
      /*$2C0C*/ tmp2_U8 = ram_peek(0x008a);
                s_y = tmp2_U8;
      /*$2C12*/ tmp1_U8 = peek((ram_peek16al(0x0000) + tmp2_U8));
      /*$2C14*/ poke((ram_peek16al(0x0000) + tmp2_U8), ((ram_peek(0x008b) ^ 0xff) & tmp1_U8));
      /*$2C16*/ tmp3_U8 = (uint8_t)(tmp3_U8 + 0x01);
                s_x = tmp3_U8;
      /*$2C19*/ branchTarget = true; block_id = !(tmp3_U8 >= 0xb0) ? 1386 : 1387;
      break;
    case 1386:  // $2C19
      /*$2C19*/ CYCLES_EDGE(0x2c19, 1);
                branchTarget = true; block_id = 1385;
      break;
    case 1387:  // $2C1B
      /*$2C1B*/ CYCLES(0x2c1b, 7);
                tmp3_U8 = ram_peek(0x008b);
                tmp2_U8 = tmp3_U8 & 0x01;
                s_status_c = tmp2_U8;
                ram_poke(0x008b, (tmp3_U8 >> 0x01));
      /*$2C1D*/ branchTarget = true; block_id = !tmp2_U8 ? 1388 : 1389;
      break;
    case 1388:  // $2C1D
      /*$2C1D*/ CYCLES_EDGE(0x2c1d, 1);
                branchTarget = true; block_id = 1392;
      break;
    case 1389:  // $2C1F
      /*$2C1F*/ CYCLES(0x2c1f, 20);
      /*$2C21*/ ram_poke(0x008b, 0x40);
      /*$2C23*/ ram_poke(0x008a, (uint8_t)(ram_peek(0x008a) - 0x01));
      /*$2C29*/ tmp2_U8 = ram_peek(0x008a) >= 0x0a;
                s_status_c = tmp2_U8;
      /*$2C2B*/ branchTarget = true; block_id = tmp2_U8 ? 1390 : 1391;
      break;
    case 1390:  // $2C2B
      /*$2C2B*/ CYCLES_EDGE(0x2c2b, 1);
                branchTarget = true; block_id = 1392;
      break;
    case 1391:  // $2C2D
      /*$2C2D*/ CYCLES(0x2c2d, 3);
                branchTarget = true; block_id = 903;
      break;
    case 1392:  // $2C30
      /*$2C30*/ CYCLES(0x2c30, 11);
                s_status_not_z = 0x1e;
                s_status_n = 0x00;
                s_a = 0x1e;
      /*$2C32*/ ram_poke(0x0089, 0x1e);
      /*$2C34*/ branchTarget = true; block_id = find_block_id_func_t001(0x2c34, pop16() + 1);;
      break;
    case 1393:  // $2C40
      /*$2C40*/ CYCLES(0x2c40, 2);
                s_x = 0xa8;
                block_id = 1394;
      break;
    case 1394:  // $2C42
      /*$2C42*/ CYCLES(0x2c42, 23);
                tmp2_U8 = s_x;
      /*$2C45*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp2_U8)));
      /*$2C4A*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp2_U8)));
      /*$2C4C*/ s_y = ram_peek(0x008a);
      /*$2C4E*/ tmp2_U8 = ram_peek(0x008b);
                s_a = tmp2_U8;
      /*$2C50*/ ram_poke(0x0002, tmp2_U8);
                block_id = 1395;
      break;
    case 1395:  // $2C52
      /*$2C52*/ CYCLES(0x2c52, 4);
      /*$2C54*/ branchTarget = true; block_id = (s_y >= 0x28) ? 1396 : 1397;
      break;
    case 1396:  // $2C54
      /*$2C54*/ CYCLES_EDGE(0x2c54, 1);
                branchTarget = true; block_id = 1401;
      break;
    case 1397:  // $2C56
      /*$2C56*/ CYCLES(0x2c56, 23);
      /*$2C5A*/ tmp3_U8 = s_y;
                tmp2_U8 = peek((ram_peek16al(0x0000) + tmp3_U8));
      /*$2C5C*/ poke((ram_peek16al(0x0000) + tmp3_U8), ((ram_peek(0x0002) ^ 0xff) & tmp2_U8));
      /*$2C5E*/ tmp3_U8 = (uint8_t)(ram_peek(0x0002) << 0x01);
                ram_poke(0x0002, tmp3_U8);
      /*$2C60*/ branchTarget = true; block_id = !(tmp3_U8 & 0x80) ? 1398 : 1399;
      break;
    case 1398:  // $2C60
      /*$2C60*/ CYCLES_EDGE(0x2c60, 1);
                branchTarget = true; block_id = 1397;
      break;
    case 1399:  // $2C62
      /*$2C62*/ CYCLES(0x2c62, 9);
                s_a = 0x01;
      /*$2C64*/ ram_poke(0x0002, 0x01);
      /*$2C66*/ tmp3_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp3_U8;
      /*$2C67*/ branchTarget = true; block_id = tmp3_U8 ? 1400 : 1401;
      break;
    case 1400:  // $2C67
      /*$2C67*/ CYCLES_EDGE(0x2c67, 1);
                branchTarget = true; block_id = 1395;
      break;
    case 1401:  // $2C69
      /*$2C69*/ CYCLES(0x2c69, 6);
                tmp2_U8 = s_x;
                tmp3_U8 = (uint8_t)(tmp2_U8 + 0x01);
                s_x = tmp3_U8;
      /*$2C6A*/ s_status_not_z = (tmp3_U8 != 0xb0);
                tmp3_U8 = tmp3_U8 >= 0xb0;
                s_status_c = tmp3_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - 0xaf) & 0x80);
      /*$2C6C*/ branchTarget = true; block_id = !tmp3_U8 ? 1402 : 1403;
      break;
    case 1402:  // $2C6C
      /*$2C6C*/ CYCLES_EDGE(0x2c6c, 1);
                branchTarget = true; block_id = 1394;
      break;
    case 1403:  // $2C6E
      /*$2C6E*/ CYCLES(0x2c6e, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x2c6e, pop16() + 1);;
      break;
    case 1404:  // $2C80
      /*$2C80*/ CYCLES(0x2c80, 73);
                tmp3_U8 = s_x;
      /*$2C83*/ tmp2_U8 = s_y;
                ram_poke((0x4350 + tmp2_U8), ram_peek((0x9f00 + tmp3_U8)));
      /*$2C89*/ ram_poke((0x4750 + tmp2_U8), ram_peek((0x9f18 + tmp3_U8)));
      /*$2C8F*/ ram_poke((0x4b50 + tmp2_U8), ram_peek((0x9f30 + tmp3_U8)));
      /*$2C95*/ ram_poke((0x4f50 + tmp2_U8), ram_peek((0x9f48 + tmp3_U8)));
      /*$2C9B*/ ram_poke((0x5350 + tmp2_U8), ram_peek((0x9f60 + tmp3_U8)));
      /*$2CA1*/ ram_poke((0x5750 + tmp2_U8), ram_peek((0x9f78 + tmp3_U8)));
      /*$2CA4*/ tmp1_U8 = ram_peek((0x9f90 + tmp3_U8));
                s_a = tmp1_U8;
      /*$2CA7*/ ram_poke((0x5b50 + tmp2_U8), tmp1_U8);
      /*$2CAA*/ s_x = (uint8_t)(tmp3_U8 - 0x01);
      /*$2CAB*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_y = tmp2_U8;
      /*$2CAC*/ branchTarget = true; block_id = find_block_id_func_t001(0x2cac, pop16() + 1);;
      break;
    case 1405:  // $2CB0
      /*$2CB0*/ CYCLES(0x2cb0, 73);
                tmp2_U8 = s_x;
      /*$2CB3*/ tmp3_U8 = s_y;
                ram_poke((0x43d0 + tmp3_U8), ram_peek((0x9f00 + tmp2_U8)));
      /*$2CB9*/ ram_poke((0x47d0 + tmp3_U8), ram_peek((0x9f18 + tmp2_U8)));
      /*$2CBF*/ ram_poke((0x4bd0 + tmp3_U8), ram_peek((0x9f30 + tmp2_U8)));
      /*$2CC5*/ ram_poke((0x4fd0 + tmp3_U8), ram_peek((0x9f48 + tmp2_U8)));
      /*$2CCB*/ ram_poke((0x53d0 + tmp3_U8), ram_peek((0x9f60 + tmp2_U8)));
      /*$2CD1*/ ram_poke((0x57d0 + tmp3_U8), ram_peek((0x9f78 + tmp2_U8)));
      /*$2CD4*/ tmp1_U8 = ram_peek((0x9f90 + tmp2_U8));
                s_a = tmp1_U8;
      /*$2CD7*/ ram_poke((0x5bd0 + tmp3_U8), tmp1_U8);
      /*$2CDA*/ s_y = (uint8_t)(tmp3_U8 - 0x01);
      /*$2CDB*/ tmp2_U8 = (uint8_t)(tmp2_U8 - 0x01);
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_x = tmp2_U8;
      /*$2CDC*/ branchTarget = true; block_id = find_block_id_func_t001(0x2cdc, pop16() + 1);;
      break;
    case 1406:  // $2CE0
      /*$2CE0*/ CYCLES(0x2ce0, 6);
                func_2c80(0x2ce2);
                branchTarget = true; block_id = 1407;
      break;
    case 1407:  // $2CE3
      /*$2CE3*/ CYCLES(0x2ce3, 3);
                func_2c80(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x2ce3, pop16() + 1);;
      break;
    case 1408:  // $2CE8
      /*$2CE8*/ CYCLES(0x2ce8, 6);
                func_2cb0(0x2cea);
                branchTarget = true; block_id = 1409;
      break;
    case 1409:  // $2CEB
      /*$2CEB*/ CYCLES(0x2ceb, 3);
                func_2cb0(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x2ceb, pop16() + 1);;
      break;
    case 1410:  // $2D00
      /*$2D00*/ CYCLES(0x2d00, 19);
      /*$2D06*/ s_x = (uint8_t)((uint8_t)((ram_peek(0x0080) & 0x0f) << 0x01) + 0x01);
      /*$2D07*/ s_y = 0x0f;
      /*$2D09*/ func_2ce0(0x2d0b);
                branchTarget = true; block_id = 1411;
      break;
    case 1411:  // $2D0C
      /*$2D0C*/ CYCLES(0x2d0c, 9);
                s_a = ram_peek(0x0080);
      /*$2D0E*/ func_2d38(0x2d10);
                branchTarget = true; block_id = 1412;
      break;
    case 1412:  // $2D11
      /*$2D11*/ CYCLES(0x2d11, 10);
      /*$2D12*/ s_x = (uint8_t)(s_a + 0x01);
      /*$2D13*/ func_2ce0(0x2d15);
                branchTarget = true; block_id = 1413;
      break;
    case 1413:  // $2D16
      /*$2D16*/ CYCLES(0x2d16, 17);
      /*$2D1C*/ s_x = (uint8_t)((uint8_t)((ram_peek(0x0081) & 0x0f) << 0x01) + 0x01);
      /*$2D1D*/ func_2ce0(0x2d1f);
                branchTarget = true; block_id = 1414;
      break;
    case 1414:  // $2D20
      /*$2D20*/ CYCLES(0x2d20, 9);
                s_a = ram_peek(0x0081);
      /*$2D22*/ func_2d38(0x2d24);
                branchTarget = true; block_id = 1415;
      break;
    case 1415:  // $2D25
      /*$2D25*/ CYCLES(0x2d25, 10);
      /*$2D26*/ s_x = (uint8_t)(s_a + 0x01);
      /*$2D27*/ func_2ce0(0x2d29);
                branchTarget = true; block_id = 1416;
      break;
    case 1416:  // $2D2A
      /*$2D2A*/ CYCLES(0x2d2a, 14);
      /*$2D2E*/ tmp5_U16 = (ram_peek(0x0082) & 0x0f) << 0x01;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
      /*$2D30*/ s_x = (uint8_t)(((uint8_t)tmp5_U16) + 0x01);
      /*$2D31*/ func_2ce0(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x2d31, pop16() + 1);;
      break;
    case 1417:  // $2D38
      /*$2D38*/ CYCLES(0x2d38, 16);
      /*$2D3C*/ tmp5_U16 = (s_a >> 0x04) << 0x01;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                tmp2_U8 = (uint8_t)tmp5_U16;
                s_status_not_z = tmp2_U8;
                s_status_n = (tmp2_U8 & 0x80);
                s_a = tmp2_U8;
      /*$2D3D*/ branchTarget = true; block_id = find_block_id_func_t001(0x2d3d, pop16() + 1);;
      break;
    case 1418:  // $2D40
      /*$2D40*/ CYCLES(0x2d40, 19);
      /*$2D46*/ s_x = (uint8_t)((uint8_t)((ram_peek(0x0083) & 0x0f) << 0x01) + 0x01);
      /*$2D47*/ s_y = 0x0f;
      /*$2D49*/ func_2ce8(0x2d4b);
                branchTarget = true; block_id = 1419;
      break;
    case 1419:  // $2D4C
      /*$2D4C*/ CYCLES(0x2d4c, 9);
                s_a = ram_peek(0x0083);
      /*$2D4E*/ func_2d38(0x2d50);
                branchTarget = true; block_id = 1420;
      break;
    case 1420:  // $2D51
      /*$2D51*/ CYCLES(0x2d51, 10);
      /*$2D52*/ s_x = (uint8_t)(s_a + 0x01);
      /*$2D53*/ func_2ce8(0x2d55);
                branchTarget = true; block_id = 1421;
      break;
    case 1421:  // $2D56
      /*$2D56*/ CYCLES(0x2d56, 17);
      /*$2D5C*/ s_x = (uint8_t)((uint8_t)((ram_peek(0x0084) & 0x0f) << 0x01) + 0x01);
      /*$2D5D*/ func_2ce8(0x2d5f);
                branchTarget = true; block_id = 1422;
      break;
    case 1422:  // $2D60
      /*$2D60*/ CYCLES(0x2d60, 9);
                s_a = ram_peek(0x0084);
      /*$2D62*/ func_2d38(0x2d64);
                branchTarget = true; block_id = 1423;
      break;
    case 1423:  // $2D65
      /*$2D65*/ CYCLES(0x2d65, 10);
      /*$2D66*/ s_x = (uint8_t)(s_a + 0x01);
      /*$2D67*/ func_2ce8(0x2d69);
                branchTarget = true; block_id = 1424;
      break;
    case 1424:  // $2D6A
      /*$2D6A*/ CYCLES(0x2d6a, 14);
      /*$2D6E*/ tmp5_U16 = (ram_peek(0x0085) & 0x0f) << 0x01;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
      /*$2D70*/ s_x = (uint8_t)(((uint8_t)tmp5_U16) + 0x01);
      /*$2D71*/ func_2ce8(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x2d71, pop16() + 1);;
      break;
    case 1425:  // $2D80
      /*$2D80*/ CYCLES(0x2d80, 18);
                s_a = ram_peek(0x0086);
      /*$2D82*/ s_status_c = 0x00;
      /*$2D83*/ block_id = s_status_d ? 1427 : 1426;
      break;
    case 1426:  // $2D83
      /*$2D83*/ tmp5_U16 = s_a;
                tmp6_U16 = (tmp5_U16 + 0x0001) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp5_U16, (uint8_t)0x0001);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1428;
      break;
    case 1427:  // $2D83
      /*$2D83*/ tmp6_U16 = adc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 1428;
      break;
    case 1428:  // $2D85
      /*$2D85*/ tmp6_U16 = s_a << 0x01;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
      /*$2D87*/ s_x = (uint8_t)(((uint8_t)tmp6_U16) + 0x01);
      /*$2D88*/ s_y = 0x1d;
      /*$2D8A*/ func_2ce0(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x2d8a, pop16() + 1);;
      break;
    case 1429:  // $2D90
      /*$2D90*/ CYCLES(0x2d90, 14);
      /*$2D92*/ tmp6_U16 = ram_peek(0x0087) << 0x01;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
      /*$2D94*/ s_x = (uint8_t)(((uint8_t)tmp6_U16) + 0x01);
      /*$2D95*/ s_y = 0x27;
      /*$2D97*/ func_2ce0(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x2d97, pop16() + 1);;
      break;
    case 1430:  // $2DA0
      /*$2DA0*/ CYCLES(0x2da0, 19);
      /*$2DA6*/ s_x = (uint8_t)((uint8_t)((ram_peek(0x0088) & 0x0f) << 0x01) + 0x01);
      /*$2DA7*/ s_y = 0x1d;
      /*$2DA9*/ func_2ce8(0x2dab);
                branchTarget = true; block_id = 1431;
      break;
    case 1431:  // $2DAC
      /*$2DAC*/ CYCLES(0x2dac, 20);
      /*$2DB2*/ tmp5_U16 = (ram_peek(0x0088) >> 0x04) << 0x01;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
      /*$2DB4*/ s_x = (uint8_t)(((uint8_t)tmp5_U16) + 0x01);
      /*$2DB5*/ func_2ce8(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x2db5, pop16() + 1);;
      break;
    case 1432:  // $2DC3
      /*$2DC3*/ CYCLES(0x2dc3, 6);
                func_2d40(0x2dc5);
                branchTarget = true; block_id = 1433;
      break;
    case 1433:  // $2DC6
      /*$2DC6*/ CYCLES(0x2dc6, 6);
                func_2d80(0x2dc8);
                branchTarget = true; block_id = 1434;
      break;
    case 1434:  // $2DC9
      /*$2DC9*/ CYCLES(0x2dc9, 6);
                func_2d90(0x2dcb);
                branchTarget = true; block_id = 1435;
      break;
    case 1435:  // $2DCC
      /*$2DCC*/ CYCLES(0x2dcc, 3);
                branchTarget = true; block_id = 1430;
      break;
    case 1436:  // $2E00
      /*$2E00*/ CYCLES(0x2e00, 38);
                s_status_d = 0x01;
      /*$2E03*/ tmp5_U16 = adc_dec16(s_x, ram_peek(0x0080), 0x00);
                s_status_c = ((uint8_t)(tmp5_U16 >> 8) & 0x01);
      /*$2E05*/ ram_poke(0x0080, ((uint8_t)tmp5_U16));
      /*$2E07*/ s_a = s_y;
      /*$2E08*/ block_id = 0x01 ? 1438 : 1437;
      break;
    case 1437:  // $2E08
      /*$2E08*/ tmp6_U16 = (s_a + ram_peek(0x0081)) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1439;
      break;
    case 1438:  // $2E08
      /*$2E08*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x0081), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1439;
      break;
    case 1439:  // $2E0A
      /*$2E0A*/ ram_poke(0x0081, s_a);
      /*$2E0C*/ s_a = 0x00;
      /*$2E0E*/ block_id = s_status_d ? 1441 : 1440;
      break;
    case 1440:  // $2E0E
      /*$2E0E*/ tmp6_U16 = s_a;
                tmp5_U16 = ram_peek(0x0082);
                tmp4_U16 = (tmp6_U16 + tmp5_U16) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)tmp5_U16);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1442;
      break;
    case 1441:  // $2E0E
      /*$2E0E*/ tmp4_U16 = adc_dec16(s_a, ram_peek(0x0082), s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 1442;
      break;
    case 1442:  // $2E10
      /*$2E10*/ ram_poke(0x0082, s_a);
      /*$2E12*/ s_status_d = 0x00;
      /*$2E15*/ ram_poke(0x007d, 0x00);
      /*$2E17*/ func_2d00(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x2e17, pop16() + 1);;
      break;
    case 1443:  // $2F00
      /*$2F00*/ CYCLES(0x2f00, 4);
                s_x = 0x00;
      /*$2F02*/ s_y = 0xa7;
                block_id = 1444;
      break;
    case 1444:  // $2F04
      /*$2F04*/ CYCLES(0x2f04, 58);
                tmp2_U8 = s_y;
      /*$2F07*/ push8(ram_peek((0x0c00 + tmp2_U8)));
      /*$2F08*/ tmp3_U8 = s_x;
      /*$2F0B*/ ram_poke((0x0c00 + tmp2_U8), ram_peek((0x0c00 + tmp3_U8)));
      /*$2F0E*/ tmp1_U8 = pop8();
      /*$2F0F*/ ram_poke((0x0c00 + tmp3_U8), tmp1_U8);
      /*$2F15*/ push8(ram_peek((0x0d00 + tmp2_U8)));
      /*$2F19*/ ram_poke((0x0d00 + tmp2_U8), ram_peek((0x0d00 + tmp3_U8)));
      /*$2F1C*/ tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$2F1D*/ ram_poke((0x0d00 + tmp3_U8), tmp1_U8);
      /*$2F20*/ tmp1_U8 = (uint8_t)(tmp3_U8 + 0x01);
                s_x = tmp1_U8;
      /*$2F21*/ s_y = (uint8_t)(tmp2_U8 - 0x01);
      /*$2F22*/ s_status_not_z = (tmp1_U8 != 0x54);
                tmp1_U8 = tmp1_U8 >= 0x54;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp3_U8 - 0x53) & 0x80);
      /*$2F24*/ branchTarget = true; block_id = !tmp1_U8 ? 1445 : 1446;
      break;
    case 1445:  // $2F24
      /*$2F24*/ CYCLES_EDGE(0x2f24, 1);
                branchTarget = true; block_id = 1444;
      break;
    case 1446:  // $2F26
      /*$2F26*/ CYCLES(0x2f26, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x2f26, pop16() + 1);;
      break;
    case 1447:  // $2F80
      /*$2F80*/ CYCLES(0x2f80, 6);
                func_3000(0x2f82);
      /*$2F83*/ CYCLES(0x2f83, 41);
                block_id = s_status_d ? 1449 : 1448;
      break;
    case 1448:  // $2F83
      /*$2F83*/ s_a = (uint8_t)((s_a + 0x0006) + s_status_c);
                block_id = 1450;
      break;
    case 1449:  // $2F83
      /*$2F83*/ s_a = ((uint8_t)adc_dec16(s_a, 0x06, s_status_c));
                block_id = 1450;
      break;
    case 1450:  // $2F85
      /*$2F85*/ ram_poke(0x0000, s_a);
      /*$2F87*/ s_a = 0xa8;
      /*$2F89*/ s_status_c = 0x01;
      /*$2F8A*/ block_id = s_status_d ? 1452 : 1451;
      break;
    case 1451:  // $2F8A
      /*$2F8A*/ s_a = (uint8_t)((s_a - ram_peek(0x0000)) - (uint8_t)(0x01 - s_status_c));
                block_id = 1453;
      break;
    case 1452:  // $2F8A
      /*$2F8A*/ s_a = ((uint8_t)sbc_dec16(s_a, ram_peek(0x0000), s_status_c));
                block_id = 1453;
      break;
    case 1453:  // $2F8C
      /*$2F8C*/ s_status_c = 0x01;
      /*$2F8D*/ block_id = s_status_d ? 1455 : 1454;
      break;
    case 1454:  // $2F8D
      /*$2F8D*/ tmp4_U16 = s_a;
                tmp5_U16 = (tmp4_U16 - 0x0005) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0xfffa);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 1456;
      break;
    case 1455:  // $2F8D
      /*$2F8D*/ tmp5_U16 = sbc_dec16(s_a, 0x05, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 1456;
      break;
    case 1456:  // $2F8F
      /*$2F8F*/ ram_poke(0x00e0, s_a);
      /*$2F94*/ push8(ram_peek(0x1066));
      /*$2F98*/ ram_poke(0x1066, ram_peek(0x1078));
      /*$2F9B*/ tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$2FE0*/ CYCLES(0x2fe0, 6);
                func_3040(0x2fe2);
      /*$2FE3*/ CYCLES(0x2fe3, 5);
                tmp1_U8 = ram_peek(0x00e3);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_a = tmp1_U8;
      /*$2FE5*/ branchTarget = true; block_id = tmp1_U8 ? 1475 : 1476;
      break;
    case 1457:  // $2FA0
      /*$2FA0*/ CYCLES(0x2fa0, 9);
                s_x = 0x00;
      /*$2FA5*/ branchTarget = true; block_id = !(uint8_t)((ram_peek(0x00ff) << 0x01) >> 8) ? 1458 : 1459;
      break;
    case 1458:  // $2FA5
      /*$2FA5*/ CYCLES_EDGE(0x2fa5, 1);
                branchTarget = true; block_id = 1460;
      break;
    case 1459:  // $2FA7
      /*$2FA7*/ CYCLES(0x2fa7, 2);
                s_x = 0xc0;
                block_id = 1460;
      break;
    case 1460:  // $2FA9
      /*$2FA9*/ CYCLES(0x2fa9, 15);
                ram_poke(0x00af, s_x);
      /*$2FAB*/ s_a = ram_peek(0x007e);
      /*$2FAD*/ s_status_c = 0x00;
      /*$2FAE*/ block_id = s_status_d ? 1462 : 1461;
      break;
    case 1461:  // $2FAE
      /*$2FAE*/ tmp5_U16 = s_a;
                tmp4_U16 = ram_peek(0x0086);
                tmp6_U16 = (tmp5_U16 + tmp4_U16) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp5_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1463;
      break;
    case 1462:  // $2FAE
      /*$2FAE*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x0086), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 1463;
      break;
    case 1463:  // $2FB0
      /*$2FB0*/ tmp1_U8 = s_a;
                s_status_not_z = (tmp1_U8 != 0x0a);
                tmp3_U8 = tmp1_U8 >= 0x0a;
                s_status_c = tmp3_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - 0x0a) & 0x80);
      /*$2FB2*/ branchTarget = true; block_id = !tmp3_U8 ? 1464 : 1465;
      break;
    case 1464:  // $2FB2
      /*$2FB2*/ CYCLES_EDGE(0x2fb2, 1);
      /*$2FB5*/ CYCLES(0x2fb5, 4);
      /*$2FB7*/ branchTarget = true; block_id = (s_x != 0x00) ? 1466 : 1467;
      break;
    case 1465:  // $2FB4
      /*$2FB4*/ CYCLES(0x2fb4, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x2fb4, pop16() + 1);;
      break;
    case 1466:  // $2FB7
      /*$2FB7*/ CYCLES_EDGE(0x2fb7, 1);
      /*$2FC6*/ CYCLES(0x2fc6, 8);
      /*$2FCB*/ branchTarget = true; block_id = !(ram_peek(0x0c00) != 0x40) ? 1470 : 1471;
      break;
    case 1467:  // $2FB9
      /*$2FB9*/ CYCLES(0x2fb9, 8);
      /*$2FBE*/ branchTarget = true; block_id = (ram_peek(0x0c00) != 0x40) ? 1468 : 1469;
      break;
    case 1468:  // $2FBE
      /*$2FBE*/ CYCLES_EDGE(0x2fbe, 1);
      /*$2FC3*/ CYCLES(0x2fc3, 3);
                branchTarget = true; block_id = 1765;
      break;
    case 1469:  // $2FC0
      /*$2FC0*/ CYCLES(0x2fc0, 3);
                branchTarget = true; block_id = 1766;
      break;
    case 1470:  // $2FCB
      /*$2FCB*/ CYCLES_EDGE(0x2fcb, 1);
      /*$2FD0*/ CYCLES(0x2fd0, 3);
                branchTarget = true; block_id = 1765;
      break;
    case 1471:  // $2FCD
      /*$2FCD*/ CYCLES(0x2fcd, 3);
                branchTarget = true; block_id = 1766;
      break;
    case 1472:  // $2FD3
      /*$2FD3*/ CYCLES(0x2fd3, 4);
                tmp1_U8 = s_a;
                tmp2_U8 = tmp1_U8 != 0x40;
                s_status_not_z = tmp2_U8;
                s_status_c = (tmp1_U8 >= 0x40);
                s_status_n = ((uint8_t)(tmp1_U8 - 0x40) & 0x80);
      /*$2FD5*/ branchTarget = true; block_id = tmp2_U8 ? 1473 : 1474;
      break;
    case 1473:  // $2FD5
      /*$2FD5*/ CYCLES_EDGE(0x2fd5, 1);
      /*$2FD8*/ CYCLES(0x2fd8, 6);
                func_2f80(0x2fda);
      /*$2FDB*/ CYCLES(0x2fdb, 6);
                func_2f00(0x2fdd);
      /*$2FDE*/ CYCLES(0x2fde, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x2fde, pop16() + 1);;
      break;
    case 1474:  // $2FD7
      /*$2FD7*/ CYCLES(0x2fd7, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x2fd7, pop16() + 1);;
      break;
    case 1475:  // $2FE5
      /*$2FE5*/ CYCLES_EDGE(0x2fe5, 1);
      /*$2FE8*/ CYCLES(0x2fe8, 2);
                branchTarget = true; block_id = !s_status_n ? 1477 : 1478;
      break;
    case 1476:  // $2FE7
      /*$2FE7*/ CYCLES(0x2fe7, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x2fe7, pop16() + 1);;
      break;
    case 1477:  // $2FE8
      /*$2FE8*/ CYCLES_EDGE(0x2fe8, 1);
      /*$2FEE*/ CYCLES(0x2fee, 2);
                s_status_not_z = 0xff;
                s_status_n = 0x80;
                s_a = 0xff;
                block_id = 1479;
      break;
    case 1478:  // $2FEA
      /*$2FEA*/ CYCLES(0x2fea, 4);
                s_status_not_z = 0x01;
                s_status_n = 0x00;
                s_a = 0x01;
      /*$2FEC*/ CYCLES_EDGE(0x2fec, 1);
                branchTarget = true; block_id = 1479;
      break;
    case 1479:  // $2FF0
      /*$2FF0*/ CYCLES(0x2ff0, 9);
                ram_poke(0x00e3, s_a);
      /*$2FF2*/ branchTarget = true; block_id = find_block_id_func_t001(0x2ff2, pop16() + 1);;
      break;
    case 1480:  // $3000
      /*$3000*/ CYCLES(0x3000, 18);
                s_a = ram_peek(0x00e2);
      /*$3002*/ s_status_c = 0x00;
      /*$3003*/ block_id = s_status_d ? 1482 : 1481;
      break;
    case 1481:  // $3003
      /*$3003*/ s_a = (uint8_t)((s_a + 0x0060) + s_status_c);
                block_id = 1483;
      break;
    case 1482:  // $3003
      /*$3003*/ s_a = ((uint8_t)adc_dec16(s_a, 0x60, s_status_c));
                block_id = 1483;
      break;
    case 1483:  // $3005
      /*$3005*/ ram_poke(0x1040, s_a);
      /*$3008*/ s_a = 0x00;
      /*$300A*/ tmp3_U8 = ram_peek(0x00e5);
                s_x = tmp3_U8;
      /*$300C*/ branchTarget = true; block_id = !tmp3_U8 ? 1484 : 1485;
      break;
    case 1484:  // $300C
      /*$300C*/ CYCLES_EDGE(0x300c, 1);
                branchTarget = true; block_id = 1493;
      break;
    case 1485:  // $300E
      /*$300E*/ CYCLES(0x300e, 8);
                s_status_c = 0x00;
      /*$300F*/ block_id = s_status_d ? 1487 : 1486;
      break;
    case 1486:  // $300F
      /*$300F*/ s_a = (uint8_t)((s_a + 0x0050) + s_status_c);
                block_id = 1488;
      break;
    case 1487:  // $300F
      /*$300F*/ s_a = ((uint8_t)adc_dec16(s_a, 0x50, s_status_c));
                block_id = 1488;
      break;
    case 1488:  // $3011
      /*$3011*/ tmp3_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp3_U8;
      /*$3012*/ branchTarget = true; block_id = !tmp3_U8 ? 1489 : 1490;
      break;
    case 1489:  // $3012
      /*$3012*/ CYCLES_EDGE(0x3012, 1);
                branchTarget = true; block_id = 1493;
      break;
    case 1490:  // $3014
      /*$3014*/ CYCLES(0x3014, 4);
                s_status_c = 0x00;
      /*$3015*/ block_id = s_status_d ? 1492 : 1491;
      break;
    case 1491:  // $3015
      /*$3015*/ s_a = (uint8_t)((s_a + 0x0050) + s_status_c);
                block_id = 1494;
      break;
    case 1492:  // $3015
      /*$3015*/ s_a = ((uint8_t)adc_dec16(s_a, 0x50, s_status_c));
                block_id = 1494;
      break;
    case 1493:  // $3017
      /*$3017*/ CYCLES(0x3017, 36);
                ram_poke(0x103f, s_a);
      /*$301A*/ tmp3_U8 = ram_peek(0x00e0);
                s_a = tmp3_U8;
      /*$301C*/ ram_poke(0x0004, tmp3_U8);
      /*$301E*/ s_status_c = 0x00;
      /*$301F*/ block_id = s_status_d ? 1496 : 1495;
      break;
    case 1494:  // $3017
      /*$3015*/ block_id = 1493;
      break;
    case 1495:  // $301F
      /*$301F*/ s_a = (uint8_t)((s_a + 0x000b) + s_status_c);
                block_id = 1497;
      break;
    case 1496:  // $301F
      /*$301F*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0b, s_status_c));
                block_id = 1497;
      break;
    case 1497:  // $3021
      /*$3021*/ ram_poke(0x0005, s_a);
      /*$3023*/ tmp3_U8 = ram_peek(0x00e1);
                s_a = tmp3_U8;
      /*$3025*/ ram_poke(0x0006, tmp3_U8);
      /*$3027*/ s_status_c = 0x00;
      /*$3028*/ block_id = s_status_d ? 1499 : 1498;
      break;
    case 1498:  // $3028
      /*$3028*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 + 0x0007) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x0007);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1500;
      break;
    case 1499:  // $3028
      /*$3028*/ tmp4_U16 = adc_dec16(s_a, 0x07, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 1500;
      break;
    case 1500:  // $302A
      /*$302A*/ ram_poke(0x0007, s_a);
      /*$302C*/ func_1030(0x302e);
      /*$302F*/ CYCLES(0x302f, 11);
                tmp3_U8 = ram_peek(0x00e0);
                s_status_not_z = tmp3_U8;
                s_status_n = (tmp3_U8 & 0x80);
                s_a = tmp3_U8;
      /*$3031*/ s_status_c = 0x00;
      /*$3032*/ branchTarget = true; block_id = find_block_id_func_t001(0x3032, pop16() + 1);;
      break;
    case 1501:  // $3040
      /*$3040*/ CYCLES(0x3040, 33);
                ram_poke(0x1078, s_a);
      /*$3046*/ push8(ram_peek(0x36cd));
      /*$304A*/ ram_poke(0x36cd, ram_peek(0x36db));
      /*$304D*/ tmp3_U8 = pop8();
                s_status_not_z = tmp3_U8;
                s_status_n = (tmp3_U8 & 0x80);
                s_a = tmp3_U8;
      /*$304E*/ ram_poke(0x36db, tmp3_U8);
      /*$3051*/ branchTarget = true; block_id = find_block_id_func_t001(0x3051, pop16() + 1);;
      break;
    case 1502:  // $3070
      /*$3070*/ CYCLES(0x3070, 25);
      /*$3072*/ ram_poke(0x000a, 0x00);
      /*$3076*/ ram_poke(0x000b, 0x86);
      /*$307A*/ ram_poke(0x000c, 0x5e);
      /*$307E*/ ram_poke(0x0005, 0x02);
      /*$3082*/ ram_poke(0x0004, 0x00);
                block_id = 1503;
      break;
    case 1503:  // $3084
      /*$3084*/ CYCLES(0x3084, 3);
                s_x = ram_peek(0x000c);
                block_id = 1504;
      break;
    case 1504:  // $3086
      /*$3086*/ CYCLES(0x3086, 37);
                tmp3_U8 = s_x;
      /*$3089*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp3_U8)));
      /*$308E*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp3_U8)));
      /*$3090*/ tmp1_U8 = ram_peek(0x0004);
                s_y = tmp1_U8;
      /*$3094*/ tmp2_U8 = peek((ram_peek16al(0x0000) + tmp1_U8));
      /*$3096*/ poke((ram_peek16al(0x0000) + tmp1_U8), (ram_peek(0x0005) | tmp2_U8));
      /*$3098*/ tmp3_U8 = (uint8_t)(tmp3_U8 + 0x01);
                s_x = tmp3_U8;
      /*$309B*/ branchTarget = true; block_id = !(tmp3_U8 >= 0xa7) ? 1505 : 1506;
      break;
    case 1505:  // $309B
      /*$309B*/ CYCLES_EDGE(0x309b, 1);
                branchTarget = true; block_id = 1504;
      break;
    case 1506:  // $309D
      /*$309D*/ CYCLES(0x309d, 7);
                tmp3_U8 = (uint8_t)(ram_peek(0x0005) << 0x01);
                ram_poke(0x0005, tmp3_U8);
      /*$309F*/ branchTarget = true; block_id = !(tmp3_U8 & 0x80) ? 1507 : 1508;
      break;
    case 1507:  // $309F
      /*$309F*/ CYCLES_EDGE(0x309f, 1);
                branchTarget = true; block_id = 1511;
      break;
    case 1508:  // $30A1
      /*$30A1*/ CYCLES(0x30a1, 17);
      /*$30A3*/ ram_poke(0x0005, 0x01);
      /*$30A5*/ ram_poke(0x0004, (uint8_t)(ram_peek(0x0004) + 0x01));
      /*$30A7*/ tmp3_U8 = ram_peek(0x0004);
                s_a = tmp3_U8;
      /*$30A9*/ s_status_not_z = (tmp3_U8 != 0x28);
                tmp1_U8 = tmp3_U8 >= 0x28;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp3_U8 - 0x28) & 0x80);
      /*$30AB*/ branchTarget = true; block_id = !tmp1_U8 ? 1509 : 1510;
      break;
    case 1509:  // $30AB
      /*$30AB*/ CYCLES_EDGE(0x30ab, 1);
                branchTarget = true; block_id = 1511;
      break;
    case 1510:  // $30AD
      /*$30AD*/ CYCLES(0x30ad, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x30ad, pop16() + 1);;
      break;
    case 1511:  // $30AE
      /*$30AE*/ CYCLES(0x30ae, 7);
                tmp3_U8 = (uint8_t)(ram_peek(0x0005) << 0x01);
                ram_poke(0x0005, tmp3_U8);
      /*$30B0*/ branchTarget = true; block_id = !(tmp3_U8 & 0x80) ? 1512 : 1513;
      break;
    case 1512:  // $30B0
      /*$30B0*/ CYCLES_EDGE(0x30b0, 1);
                branchTarget = true; block_id = 1514;
      break;
    case 1513:  // $30B2
      /*$30B2*/ CYCLES(0x30b2, 10);
      /*$30B4*/ ram_poke(0x0005, 0x01);
      /*$30B6*/ ram_poke(0x0004, (uint8_t)(ram_peek(0x0004) + 0x01));
                block_id = 1514;
      break;
    case 1514:  // $30B8
      /*$30B8*/ CYCLES(0x30b8, 11);
      /*$30BA*/ tmp1_U8 = peek(ram_peek16al(0x000a));
                s_a = tmp1_U8;
      /*$30BE*/ branchTarget = true; block_id = (tmp1_U8 != 0x01) ? 1515 : 1516;
      break;
    case 1515:  // $30BE
      /*$30BE*/ CYCLES_EDGE(0x30be, 1);
      /*$30C5*/ CYCLES(0x30c5, 4);
      /*$30C7*/ branchTarget = true; block_id = (s_a != 0x03) ? 1517 : 1518;
      break;
    case 1516:  // $30C0
      /*$30C0*/ CYCLES(0x30c0, 8);
                ram_poke(0x000c, (uint8_t)(ram_peek(0x000c) - 0x01));
      /*$30C2*/ branchTarget = true; block_id = 1519;
      break;
    case 1517:  // $30C7
      /*$30C7*/ CYCLES_EDGE(0x30c7, 1);
                branchTarget = true; block_id = 1519;
      break;
    case 1518:  // $30C9
      /*$30C9*/ CYCLES(0x30c9, 5);
                ram_poke(0x000c, (uint8_t)(ram_peek(0x000c) + 0x01));
                block_id = 1519;
      break;
    case 1519:  // $30CB
      /*$30CB*/ CYCLES(0x30cb, 21);
                s_a = ram_peek(0x000a);
      /*$30CD*/ s_status_c = 0x00;
      /*$30CE*/ block_id = s_status_d ? 1521 : 1520;
      break;
    case 1520:  // $30CE
      /*$30CE*/ tmp4_U16 = (s_a + 0x0001) + s_status_c;
                s_status_c = (uint8_t)(tmp4_U16 >> 8);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1522;
      break;
    case 1521:  // $30CE
      /*$30CE*/ tmp4_U16 = adc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_c = ((uint8_t)(tmp4_U16 >> 8) & 0x01);
                block_id = 1522;
      break;
    case 1522:  // $30D0
      /*$30D0*/ ram_poke(0x000a, s_a);
      /*$30D2*/ s_a = ram_peek(0x000b);
      /*$30D4*/ block_id = s_status_d ? 1524 : 1523;
      break;
    case 1523:  // $30D4
      /*$30D4*/ tmp4_U16 = s_a;
                tmp6_U16 = tmp4_U16 + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1525;
      break;
    case 1524:  // $30D4
      /*$30D4*/ tmp6_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 1525;
      break;
    case 1525:  // $30D6
      /*$30D6*/ ram_poke(0x000b, s_a);
      /*$30D8*/ branchTarget = true; block_id = 1503;
      break;
    case 1526:  // $3102
      /*$3102*/ CYCLES_EDGE(0x3102, 1);
                branchTarget = true; block_id = 1538;
      break;
    case 1527:  // $3104
      /*$3104*/ CYCLES(0x3104, 11);
      /*$3106*/ ram_poke(0x0028, 0x08);
      /*$3108*/ tmp1_U8 = io_peek(0xc000);
                s_status_not_z = tmp1_U8;
                tmp3_U8 = tmp1_U8 & 0x80;
                s_status_n = tmp3_U8;
                s_a = tmp1_U8;
      /*$310B*/ branchTarget = true; block_id = !tmp3_U8 ? 1528 : 1529;
      break;
    case 1528:  // $310B
      /*$310B*/ CYCLES_EDGE(0x310b, 1);
                branchTarget = true; block_id = 1538;
      break;
    case 1529:  // $310D
      /*$310D*/ CYCLES(0x310d, 10);
      /*$3112*/ branchTarget = true; block_id = (s_a != 0x83) ? 1530 : 1531;
      break;
    case 1530:  // $3112
      /*$3112*/ CYCLES_EDGE(0x3112, 1);
      /*$3117*/ CYCLES(0x3117, 4);
      /*$3119*/ branchTarget = true; block_id = (s_a != 0x9b) ? 1532 : 1533;
      break;
    case 1531:  // $3114
      /*$3114*/ CYCLES(0x3114, 3);
      /*$3213*/ CYCLES(0x3213, 8);
                tmp3_U8 = ram_peek(0x1060);
                s_a = tmp3_U8;
      /*$3216*/ tmp1_U8 = tmp3_U8 != 0xad;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp3_U8 >= 0xad);
                s_status_n = ((uint8_t)(tmp3_U8 - 0xad) & 0x80);
      /*$3218*/ branchTarget = true; block_id = !tmp1_U8 ? 1539 : 1540;
      break;
    case 1532:  // $3119
      /*$3119*/ CYCLES_EDGE(0x3119, 1);
      /*$311E*/ CYCLES(0x311e, 4);
      /*$3120*/ branchTarget = true; block_id = (s_a != 0x93) ? 1534 : 1535;
      break;
    case 1533:  // $311B
      /*$311B*/ CYCLES(0x311b, 3);
      /*$3220*/ CYCLES(0x3220, 4);
                io_poke(0xc010, s_a);
                block_id = 1541;
      break;
    case 1534:  // $3120
      /*$3120*/ CYCLES_EDGE(0x3120, 1);
      /*$3125*/ CYCLES(0x3125, 4);
                tmp3_U8 = s_a;
                tmp1_U8 = tmp3_U8 != 0x92;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp3_U8 >= 0x92);
                s_status_n = ((uint8_t)(tmp3_U8 - 0x92) & 0x80);
      /*$3127*/ branchTarget = true; block_id = tmp1_U8 ? 1536 : 1537;
      break;
    case 1535:  // $3122
      /*$3122*/ CYCLES(0x3122, 3);
      /*$3240*/ CYCLES(0x3240, 13);
                io_poke(0xc010, s_a);
      /*$3243*/ s_x = 0x30;
      /*$3245*/ tmp1_U8 = ram_peek(0x007f);
                s_a = tmp1_U8;
      /*$3247*/ tmp3_U8 = tmp1_U8 != 0x20;
                s_status_not_z = tmp3_U8;
                s_status_c = (tmp1_U8 >= 0x20);
                s_status_n = ((uint8_t)(tmp1_U8 - 0x20) & 0x80);
      /*$3249*/ branchTarget = true; block_id = !tmp3_U8 ? 1544 : 1545;
      break;
    case 1536:  // $3127
      /*$3127*/ CYCLES_EDGE(0x3127, 1);
      /*$312C*/ CYCLES(0x312c, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x312c, pop16() + 1);;
      break;
    case 1537:  // $3129
      /*$3129*/ CYCLES(0x3129, 3);
      /*$32C0*/ CYCLES(0x32c0, 7);
                io_poke(0xc010, s_a);
      /*$32C3*/ branchTarget = true; block_id = 1751;
      break;
    case 1538:  // $3180
      /*$3180*/ CYCLES(0x3180, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x3180, pop16() + 1);;
      break;
    case 1539:  // $3218
      /*$3218*/ CYCLES_EDGE(0x3218, 1);
      /*$3200*/ CYCLES(0x3200, 14);
                tmp1_U8 = s_a;
                io_poke(0xc051, tmp1_U8);
      /*$3203*/ io_poke(0xc055, tmp1_U8);
      /*$3206*/ func_2a20(0x3208);
      /*$3209*/ CYCLES(0x3209, 18);
                tmp1_U8 = s_a;
                io_poke(0xc050, tmp1_U8);
      /*$320C*/ io_poke(0xc057, tmp1_U8);
      /*$320F*/ io_poke(0xc010, tmp1_U8);
      /*$3212*/ branchTarget = true; block_id = find_block_id_func_t001(0x3212, pop16() + 1);;
      break;
    case 1540:  // $321A
      /*$321A*/ CYCLES(0x321a, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x321a, pop16() + 1);;
      break;
    case 1541:  // $3223
      /*$3223*/ CYCLES(0x3223, 8);
                tmp1_U8 = io_peek(0xc000);
                s_a = tmp1_U8;
      /*$3226*/ tmp3_U8 = tmp1_U8 != 0x9b;
                s_status_not_z = tmp3_U8;
                s_status_c = (tmp1_U8 >= 0x9b);
                s_status_n = ((uint8_t)(tmp1_U8 - 0x9b) & 0x80);
      /*$3228*/ branchTarget = true; block_id = tmp3_U8 ? 1542 : 1543;
      break;
    case 1542:  // $3228
      /*$3228*/ CYCLES_EDGE(0x3228, 1);
                branchTarget = true; block_id = 1541;
      break;
    case 1543:  // $322A
      /*$322A*/ CYCLES(0x322a, 10);
                io_poke(0xc010, s_a);
      /*$322D*/ branchTarget = true; block_id = find_block_id_func_t001(0x322d, pop16() + 1);;
      break;
    case 1544:  // $3249
      /*$3249*/ CYCLES_EDGE(0x3249, 1);
                branchTarget = true; block_id = 1546;
      break;
    case 1545:  // $324B
      /*$324B*/ CYCLES(0x324b, 2);
                s_status_not_z = 0x20;
                s_status_n = 0x00;
                s_x = 0x20;
                block_id = 1546;
      break;
    case 1546:  // $324D
      /*$324D*/ CYCLES(0x324d, 93);
                tmp3_U8 = s_x;
                ram_poke(0x007f, tmp3_U8);
      /*$324F*/ ram_poke(0x1343, tmp3_U8);
      /*$3252*/ ram_poke(0x134b, tmp3_U8);
      /*$3255*/ ram_poke(0x1ddb, tmp3_U8);
      /*$3258*/ ram_poke(0x201c, tmp3_U8);
      /*$325B*/ ram_poke(0x209c, tmp3_U8);
      /*$325E*/ ram_poke(0x20bd, tmp3_U8);
      /*$3261*/ ram_poke(0x211c, tmp3_U8);
      /*$3264*/ ram_poke(0x213d, tmp3_U8);
      /*$3267*/ ram_poke(0x222d, tmp3_U8);
      /*$326A*/ ram_poke(0x2718, tmp3_U8);
      /*$326D*/ ram_poke(0x2728, tmp3_U8);
      /*$3270*/ ram_poke(0x28ab, tmp3_U8);
      /*$3273*/ ram_poke(0x28b3, tmp3_U8);
      /*$3276*/ ram_poke(0x2b23, tmp3_U8);
      /*$3279*/ ram_poke(0x2b78, tmp3_U8);
      /*$327C*/ ram_poke(0x2b80, tmp3_U8);
      /*$327F*/ ram_poke(0x3658, tmp3_U8);
      /*$3282*/ ram_poke(0x3660, tmp3_U8);
      /*$3285*/ ram_poke(0xae5c, tmp3_U8);
      /*$3288*/ ram_poke(0x1724, tmp3_U8);
      /*$328B*/ ram_poke(0x15ee, tmp3_U8);
      /*$328E*/ branchTarget = true; block_id = find_block_id_func_t001(0x328e, pop16() + 1);;
      break;
    case 1547:  // $3340
      /*$3340*/ CYCLES(0x3340, 38);
                tmp2_U8 = ram_peek(0x0086);
                s_x = tmp2_U8;
      /*$3342*/ tmp3_U8 = ram_peek((0x3300 + tmp2_U8));
      /*$3345*/ ram_poke(0x16c4, tmp3_U8);
      /*$3348*/ ram_poke(0x15b2, tmp3_U8);
      /*$334E*/ ram_poke(0x1bd8, ram_peek((0x3308 + tmp2_U8)));
      /*$3354*/ ram_poke(0x2187, ram_peek((0x3310 + tmp2_U8)));
      /*$335A*/ branchTarget = true; block_id = !(ram_peek(0x0086) & 0x01) ? 1548 : 1549;
      break;
    case 1548:  // $335A
      /*$335A*/ CYCLES_EDGE(0x335a, 1);
                branchTarget = true; block_id = 1550;
      break;
    case 1549:  // $335C
      /*$335C*/ CYCLES(0x335c, 6);
                func_2f80(0x335e);
      /*$335F*/ CYCLES(0x335f, 6);
                func_2f00(0x3361);
                branchTarget = true; block_id = 1550;
      break;
    case 1550:  // $3362
      /*$3362*/ CYCLES(0x3362, 7);
                tmp3_U8 = ram_peek(0x0086);
                s_a = tmp3_U8;
      /*$3364*/ s_status_not_z = (tmp3_U8 != 0x02);
                tmp1_U8 = tmp3_U8 >= 0x02;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp3_U8 - 0x02) & 0x80);
      /*$3366*/ branchTarget = true; block_id = tmp1_U8 ? 1551 : 1552;
      break;
    case 1551:  // $3366
      /*$3366*/ CYCLES_EDGE(0x3366, 1);
      /*$3386*/ CYCLES(0x3386, 2);
                branchTarget = true; block_id = !s_status_not_z ? 1553 : 1554;
      break;
    case 1552:  // $3368
      /*$3368*/ CYCLES(0x3368, 42);
      /*$336A*/ ram_poke(0x1c02, 0x04);
      /*$336D*/ ram_poke(0x1c0e, 0x04);
      /*$3372*/ ram_poke(0x1c06, 0x11);
      /*$3377*/ ram_poke(0x1b52, 0xea);
      /*$337A*/ s_status_not_z = 0xea;
                s_status_n = 0x80;
                s_a = 0xea;
      /*$337C*/ ram_poke(0x20a1, 0xea);
      /*$337F*/ ram_poke(0x20a2, 0xea);
      /*$3382*/ ram_poke(0x20a3, 0xea);
      /*$3385*/ branchTarget = true; block_id = find_block_id_func_t001(0x3385, pop16() + 1);;
      break;
    case 1553:  // $3386
      /*$3386*/ CYCLES_EDGE(0x3386, 1);
                branchTarget = true; block_id = 1560;
      break;
    case 1554:  // $3388
      /*$3388*/ CYCLES(0x3388, 2);
                branchTarget = true; block_id = s_status_not_z ? 1555 : 1556;
      break;
    case 1555:  // $3388
      /*$3388*/ CYCLES_EDGE(0x3388, 1);
      /*$3390*/ CYCLES(0x3390, 46);
      /*$3392*/ ram_poke(0x1c02, 0x08);
      /*$3395*/ ram_poke(0x1c0e, 0x08);
      /*$339A*/ ram_poke(0x1c06, 0x38);
      /*$339F*/ ram_poke(0x1b52, 0x4a);
      /*$33A4*/ ram_poke(0x20a1, 0x20);
      /*$33A9*/ ram_poke(0x20a2, 0x00);
      /*$33AC*/ s_status_not_z = 0x0f;
                s_status_n = 0x00;
                s_a = 0x0f;
      /*$33AE*/ ram_poke(0x20a3, 0x0f);
      /*$33B1*/ branchTarget = true; block_id = find_block_id_func_t001(0x33b1, pop16() + 1);;
      break;
    case 1556:  // $338A
      /*$338A*/ CYCLES(0x338a, 2);
                branchTarget = true; block_id = !s_status_not_z ? 1557 : 1558;
      break;
    case 1557:  // $338A
      /*$338A*/ CYCLES_EDGE(0x338a, 1);
                branchTarget = true; block_id = 1560;
      break;
    case 1558:  // $338C
      /*$338C*/ CYCLES(0x338c, 7);
                branchTarget = true; push16(0x338d); block_id = 1852;
      break;
    case 1559:  // $338E
      /*$338E*/ fprintf(stderr, "abort: pc=$%04X, target=$%04X, reason=%u", 0x338c, 0x338e, 0x02); error_handler(0x338c);
      break;
    case 1560:  // $33C0
      /*$33C0*/ CYCLES(0x33c0, 46);
      /*$33C2*/ ram_poke(0x1c02, 0x04);
      /*$33C5*/ ram_poke(0x1c0e, 0x04);
      /*$33CA*/ ram_poke(0x1c06, 0x38);
      /*$33CF*/ ram_poke(0x1b52, 0xea);
      /*$33D4*/ ram_poke(0x20a1, 0x20);
      /*$33D9*/ ram_poke(0x20a2, 0x00);
      /*$33DC*/ s_status_not_z = 0x0f;
                s_status_n = 0x00;
                s_a = 0x0f;
      /*$33DE*/ ram_poke(0x20a3, 0x0f);
      /*$33E1*/ branchTarget = true; block_id = find_block_id_func_t001(0x33e1, pop16() + 1);;
      break;
    case 1561:  // $3402
      /*$3402*/ CYCLES(0x3402, 17);
                tmp1_U8 = s_x;
      /*$3405*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
      /*$3407*/ tmp1_U8 = ram_peek((0x0d00 + tmp1_U8));
                s_a = tmp1_U8;
      /*$340A*/ ram_poke(0x0000, tmp1_U8);
      /*$340C*/ s_y = ram_peek(0x0006);
                block_id = 1562;
      break;
    case 1562:  // $340E
      /*$340E*/ CYCLES(0x340e, 4);
      /*$3410*/ branchTarget = true; block_id = (s_y >= 0x28) ? 1563 : 1564;
      break;
    case 1563:  // $3410
      /*$3410*/ CYCLES_EDGE(0x3410, 1);
                branchTarget = true; block_id = 1565;
      break;
    case 1564:  // $3412
      /*$3412*/ CYCLES(0x3412, 17);
                tmp1_U8 = peek(ram_peek16(0x3413));
      /*$3415*/ tmp3_U8 = s_y;
                tmp2_U8 = peek((ram_peek16al(0x0000) + tmp3_U8));
      /*$3417*/ tmp2_U8 = (tmp1_U8 | tmp2_U8) & 0x7f;
                s_a = tmp2_U8;
      /*$3419*/ poke((ram_peek16al(0x0000) + tmp3_U8), tmp2_U8);
                block_id = 1565;
      break;
    case 1565:  // $341B
      /*$341B*/ CYCLES(0x341b, 13);
                ram_poke(0x3413, (uint8_t)(ram_peek(0x3413) + 0x01));
      /*$341E*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$3421*/ branchTarget = true; block_id = (tmp1_U8 != ram_peek(0x0007)) ? 1566 : 1567;
      break;
    case 1566:  // $3421
      /*$3421*/ CYCLES_EDGE(0x3421, 1);
                branchTarget = true; block_id = 1562;
      break;
    case 1567:  // $3423
      /*$3423*/ CYCLES(0x3423, 9);
                s_x = (uint8_t)(s_x + 0x01);
      /*$3424*/ tmp3_U8 = (uint8_t)(ram_peek(0x0008) - 0x01);
                ram_poke(0x0008, tmp3_U8);
      /*$3426*/ branchTarget = true; block_id = tmp3_U8 ? 1568 : 1569;
      break;
    case 1568:  // $3426
      /*$3426*/ CYCLES_EDGE(0x3426, 1);
                branchTarget = true; block_id = 1573;
      break;
    case 1569:  // $3428
      /*$3428*/ CYCLES(0x3428, 23);
      /*$342A*/ ram_poke(0x0008, ram_peek(0x0009));
      /*$342C*/ ram_poke(0x0005, (uint8_t)(ram_peek(0x0005) - 0x01));
      /*$342E*/ s_a = ram_peek(0x3413);
      /*$3431*/ s_status_c = 0x00;
      /*$3432*/ block_id = s_status_d ? 1571 : 1570;
      break;
    case 1570:  // $3432
      /*$3432*/ tmp6_U16 = s_a;
                tmp4_U16 = (tmp6_U16 + 0x0009) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp6_U16, (uint8_t)0x0009);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1572;
      break;
    case 1571:  // $3432
      /*$3432*/ tmp4_U16 = adc_dec16(s_a, 0x09, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 1572;
      break;
    case 1572:  // $3434
      /*$3434*/ ram_poke(0x3413, s_a);
                block_id = 1573;
      break;
    case 1573:  // $3437
      /*$3437*/ CYCLES(0x3437, 5);
                tmp3_U8 = s_x;
                tmp2_U8 = ram_peek(0x0005);
                s_status_not_z = (tmp3_U8 != tmp2_U8);
                tmp1_U8 = tmp3_U8 >= tmp2_U8;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp3_U8 - tmp2_U8) & 0x80);
      /*$3439*/ branchTarget = true; block_id = !tmp1_U8 ? 1574 : 1575;
      break;
    case 1574:  // $3439
      /*$3439*/ CYCLES_EDGE(0x3439, 1);
                branchTarget = true; block_id = 1561;
      break;
    case 1575:  // $343B
      /*$343B*/ CYCLES(0x343b, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x343b, pop16() + 1);;
      break;
    case 1576:  // $3440
      /*$3440*/ CYCLES(0x3440, 3);
                s_x = ram_peek(0x0004);
                block_id = 1577;
      break;
    case 1577:  // $3442
      /*$3442*/ CYCLES(0x3442, 17);
                tmp1_U8 = s_x;
      /*$3445*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
      /*$3447*/ tmp1_U8 = ram_peek((0x0d00 + tmp1_U8));
                s_a = tmp1_U8;
      /*$344A*/ ram_poke(0x0000, tmp1_U8);
      /*$344C*/ s_y = ram_peek(0x0006);
                block_id = 1578;
      break;
    case 1578:  // $344E
      /*$344E*/ CYCLES(0x344e, 4);
      /*$3450*/ branchTarget = true; block_id = (s_y >= 0x28) ? 1579 : 1580;
      break;
    case 1579:  // $3450
      /*$3450*/ CYCLES_EDGE(0x3450, 1);
                branchTarget = true; block_id = 1581;
      break;
    case 1580:  // $3452
      /*$3452*/ CYCLES(0x3452, 17);
                tmp1_U8 = peek(ram_peek16(0x3453));
      /*$3457*/ tmp2_U8 = s_y;
                tmp3_U8 = peek((ram_peek16al(0x0000) + tmp2_U8));
                tmp1_U8 = (tmp1_U8 ^ 0xff) & tmp3_U8;
                s_a = tmp1_U8;
      /*$3459*/ poke((ram_peek16al(0x0000) + tmp2_U8), tmp1_U8);
                block_id = 1581;
      break;
    case 1581:  // $345B
      /*$345B*/ CYCLES(0x345b, 13);
                ram_poke(0x3453, (uint8_t)(ram_peek(0x3453) + 0x01));
      /*$345E*/ tmp1_U8 = (uint8_t)(s_y + 0x01);
                s_y = tmp1_U8;
      /*$3461*/ branchTarget = true; block_id = (tmp1_U8 != ram_peek(0x0007)) ? 1582 : 1583;
      break;
    case 1582:  // $3461
      /*$3461*/ CYCLES_EDGE(0x3461, 1);
                branchTarget = true; block_id = 1578;
      break;
    case 1583:  // $3463
      /*$3463*/ CYCLES(0x3463, 9);
                s_x = (uint8_t)(s_x + 0x01);
      /*$3464*/ tmp2_U8 = (uint8_t)(ram_peek(0x0008) - 0x01);
                ram_poke(0x0008, tmp2_U8);
      /*$3466*/ branchTarget = true; block_id = tmp2_U8 ? 1584 : 1585;
      break;
    case 1584:  // $3466
      /*$3466*/ CYCLES_EDGE(0x3466, 1);
                branchTarget = true; block_id = 1589;
      break;
    case 1585:  // $3468
      /*$3468*/ CYCLES(0x3468, 23);
      /*$346A*/ ram_poke(0x0008, ram_peek(0x0009));
      /*$346C*/ ram_poke(0x0005, (uint8_t)(ram_peek(0x0005) - 0x01));
      /*$346E*/ s_a = ram_peek(0x3453);
      /*$3471*/ s_status_c = 0x00;
      /*$3472*/ block_id = s_status_d ? 1587 : 1586;
      break;
    case 1586:  // $3472
      /*$3472*/ tmp4_U16 = s_a;
                tmp6_U16 = (tmp4_U16 + 0x0009) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)0x0009);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1588;
      break;
    case 1587:  // $3472
      /*$3472*/ tmp6_U16 = adc_dec16(s_a, 0x09, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_v = (((uint8_t)(tmp6_U16 >> 8) & 0x40) != 0);
                block_id = 1588;
      break;
    case 1588:  // $3474
      /*$3474*/ ram_poke(0x3453, s_a);
                block_id = 1589;
      break;
    case 1589:  // $3477
      /*$3477*/ CYCLES(0x3477, 5);
                tmp2_U8 = s_x;
                tmp1_U8 = ram_peek(0x0005);
                s_status_not_z = (tmp2_U8 != tmp1_U8);
                tmp3_U8 = tmp2_U8 >= tmp1_U8;
                s_status_c = tmp3_U8;
                s_status_n = ((uint8_t)(tmp2_U8 - tmp1_U8) & 0x80);
      /*$3479*/ branchTarget = true; block_id = !tmp3_U8 ? 1590 : 1591;
      break;
    case 1590:  // $3479
      /*$3479*/ CYCLES_EDGE(0x3479, 1);
                branchTarget = true; block_id = 1577;
      break;
    case 1591:  // $347B
      /*$347B*/ CYCLES(0x347b, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x347b, pop16() + 1);;
      break;
    case 1592:  // $3480
      /*$3480*/ CYCLES(0x3480, 58);
                tmp2_U8 = ram_peek(0x0071);
                s_a = tmp2_U8;
      /*$3482*/ ram_poke(0x0004, tmp2_U8);
      /*$3484*/ s_status_c = 0x00;
      /*$3485*/ block_id = s_status_d ? 1594 : 1593;
      break;
    case 1593:  // $3485
      /*$3485*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
                block_id = 1595;
      break;
    case 1594:  // $3485
      /*$3485*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
                block_id = 1595;
      break;
    case 1595:  // $3487
      /*$3487*/ ram_poke(0x0005, s_a);
      /*$3489*/ tmp3_U8 = ram_peek(0x0072);
                s_a = tmp3_U8;
      /*$348B*/ ram_poke(0x0006, tmp3_U8);
      /*$348D*/ s_status_c = 0x00;
      /*$348E*/ block_id = s_status_d ? 1597 : 1596;
      break;
    case 1596:  // $348E
      /*$348E*/ s_a = (uint8_t)((s_a + 0x0009) + s_status_c);
                block_id = 1598;
      break;
    case 1597:  // $348E
      /*$348E*/ s_a = ((uint8_t)adc_dec16(s_a, 0x09, s_status_c));
                block_id = 1598;
      break;
    case 1598:  // $3490
      /*$3490*/ ram_poke(0x0007, s_a);
      /*$3494*/ ram_poke(0x3453, 0x00);
      /*$3497*/ s_a = ram_peek(0x0073);
      /*$3499*/ s_status_c = 0x00;
      /*$349A*/ block_id = s_status_d ? 1600 : 1599;
      break;
    case 1599:  // $349A
      /*$349A*/ s_a = (uint8_t)((s_a + 0x0039) + s_status_c);
                block_id = 1601;
      break;
    case 1600:  // $349A
      /*$349A*/ s_a = ((uint8_t)adc_dec16(s_a, 0x39, s_status_c));
                block_id = 1601;
      break;
    case 1601:  // $349C
      /*$349C*/ ram_poke(0x3454, s_a);
      /*$349F*/ tmp3_U8 = ram_peek(0x0074);
      /*$34A1*/ ram_poke(0x0008, tmp3_U8);
      /*$34A3*/ ram_poke(0x0009, tmp3_U8);
      /*$34A5*/ func_3440(0x34a7);
      /*$34A8*/ CYCLES(0x34a8, 9);
                s_a = ram_peek(0x0073);
      /*$34AA*/ s_status_c = 0x01;
      /*$34AB*/ block_id = s_status_d ? 1603 : 1602;
      break;
    case 1602:  // $34AB
      /*$34AB*/ tmp3_U8 = (uint8_t)((s_a - 0x0002) - (uint8_t)(0x01 - s_status_c));
                s_status_n = (tmp3_U8 & 0x80);
                s_a = tmp3_U8;
                block_id = 1604;
      break;
    case 1603:  // $34AB
      /*$34AB*/ tmp6_U16 = sbc_dec16(s_a, 0x02, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_n = ((uint8_t)(tmp6_U16 >> 8) & 0x80);
                block_id = 1604;
      break;
    case 1604:  // $34AD
      /*$34AD*/ branchTarget = true; block_id = !s_status_n ? 1605 : 1606;
      break;
    case 1605:  // $34AD
      /*$34AD*/ CYCLES_EDGE(0x34ad, 1);
                branchTarget = true; block_id = 1609;
      break;
    case 1606:  // $34AF
      /*$34AF*/ CYCLES(0x34af, 9);
                ram_poke(0x0072, (uint8_t)(ram_peek(0x0072) - 0x01));
      /*$34B1*/ s_status_c = 0x00;
      /*$34B2*/ block_id = s_status_d ? 1608 : 1607;
      break;
    case 1607:  // $34B2
      /*$34B2*/ s_a = (uint8_t)((s_a + 0x0007) + s_status_c);
                block_id = 1610;
      break;
    case 1608:  // $34B2
      /*$34B2*/ s_a = ((uint8_t)adc_dec16(s_a, 0x07, s_status_c));
                block_id = 1610;
      break;
    case 1609:  // $34B4
      /*$34B4*/ CYCLES(0x34b4, 18);
                ram_poke(0x0073, s_a);
      /*$34B6*/ s_a = ram_peek(0x0071);
      /*$34B8*/ s_status_c = 0x00;
      /*$34B9*/ block_id = s_status_d ? 1612 : 1611;
      break;
    case 1610:  // $34B4
      /*$34B2*/ block_id = 1609;
      break;
    case 1611:  // $34B9
      /*$34B9*/ tmp6_U16 = s_a;
                tmp4_U16 = ram_peek(0x0077);
                tmp5_U16 = (tmp6_U16 + tmp4_U16) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp6_U16, (uint8_t)tmp4_U16);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 1613;
      break;
    case 1612:  // $34B9
      /*$34B9*/ tmp5_U16 = adc_dec16(s_a, ram_peek(0x0077), s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
                block_id = 1613;
      break;
    case 1613:  // $34BB
      /*$34BB*/ tmp3_U8 = s_a;
                ram_poke(0x0071, tmp3_U8);
      /*$34BD*/ tmp3_U8 = tmp3_U8 >= 0xa0;
                s_status_c = tmp3_U8;
      /*$34BF*/ branchTarget = true; block_id = !tmp3_U8 ? 1614 : 1615;
      break;
    case 1614:  // $34BF
      /*$34BF*/ CYCLES_EDGE(0x34bf, 1);
      /*$34C8*/ CYCLES(0x34c8, 7);
                tmp3_U8 = ram_peek(0x0072);
                s_a = tmp3_U8;
      /*$34CC*/ branchTarget = true; block_id = !(tmp3_U8 >= 0xf0) ? 1616 : 1617;
      break;
    case 1615:  // $34C1
      /*$34C1*/ CYCLES(0x34c1, 11);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$34C3*/ ram_poke(0x0070, 0x00);
      /*$34C5*/ branchTarget = true; block_id = find_block_id_func_t001(0x34c5, pop16() + 1);;
      break;
    case 1616:  // $34CC
      /*$34CC*/ CYCLES_EDGE(0x34cc, 1);
                branchTarget = true; block_id = 1619;
      break;
    case 1617:  // $34CE
      /*$34CE*/ CYCLES(0x34ce, 4);
                tmp3_U8 = s_a >= 0xf8;
                s_status_c = tmp3_U8;
      /*$34D0*/ branchTarget = true; block_id = !tmp3_U8 ? 1618 : 1619;
      break;
    case 1618:  // $34D0
      /*$34D0*/ CYCLES_EDGE(0x34d0, 1);
      /*$3510*/ CYCLES(0x3510, 11);
                s_status_not_z = 0x00;
                s_status_n = 0x00;
                s_a = 0x00;
      /*$3512*/ ram_poke(0x0070, 0x00);
      /*$3514*/ branchTarget = true; block_id = find_block_id_func_t001(0x3514, pop16() + 1);;
      break;
    case 1619:  // $34D2
      /*$34D2*/ CYCLES(0x34d2, 7);
      /*$34D6*/ branchTarget = true; block_id = (ram_peek(0x0070) != 0x03) ? 1620 : 1621;
      break;
    case 1620:  // $34D6
      /*$34D6*/ CYCLES_EDGE(0x34d6, 1);
                branchTarget = true; block_id = 1626;
      break;
    case 1621:  // $34D8
      /*$34D8*/ CYCLES(0x34d8, 7);
                tmp3_U8 = (uint8_t)(ram_peek(0x0075) - 0x01);
                ram_poke(0x0075, tmp3_U8);
      /*$34DA*/ branchTarget = true; block_id = tmp3_U8 ? 1622 : 1623;
      break;
    case 1622:  // $34DA
      /*$34DA*/ CYCLES_EDGE(0x34da, 1);
                branchTarget = true; block_id = 1626;
      break;
    case 1623:  // $34DC
      /*$34DC*/ CYCLES(0x34dc, 12);
      /*$34DE*/ ram_poke(0x0075, 0x04);
      /*$34E0*/ tmp3_U8 = (uint8_t)(ram_peek(0x0074) - 0x01);
                ram_poke(0x0074, tmp3_U8);
      /*$34E2*/ branchTarget = true; block_id = tmp3_U8 ? 1624 : 1625;
      break;
    case 1624:  // $34E2
      /*$34E2*/ CYCLES_EDGE(0x34e2, 1);
                branchTarget = true; block_id = 1626;
      break;
    case 1625:  // $34E4
      /*$34E4*/ CYCLES(0x34e4, 5);
      /*$34E6*/ ram_poke(0x0074, 0x01);
                block_id = 1626;
      break;
    case 1626:  // $34E8
      /*$34E8*/ CYCLES(0x34e8, 55);
                tmp3_U8 = ram_peek(0x0071);
                s_a = tmp3_U8;
      /*$34EA*/ ram_poke(0x0004, tmp3_U8);
      /*$34EC*/ s_status_c = 0x00;
      /*$34ED*/ block_id = s_status_d ? 1628 : 1627;
      break;
    case 1627:  // $34ED
      /*$34ED*/ s_a = (uint8_t)((s_a + 0x000c) + s_status_c);
                block_id = 1629;
      break;
    case 1628:  // $34ED
      /*$34ED*/ s_a = ((uint8_t)adc_dec16(s_a, 0x0c, s_status_c));
                block_id = 1629;
      break;
    case 1629:  // $34EF
      /*$34EF*/ ram_poke(0x0005, s_a);
      /*$34F1*/ tmp3_U8 = ram_peek(0x0072);
                s_a = tmp3_U8;
      /*$34F3*/ ram_poke(0x0006, tmp3_U8);
      /*$34F5*/ s_status_c = 0x00;
      /*$34F6*/ block_id = s_status_d ? 1631 : 1630;
      break;
    case 1630:  // $34F6
      /*$34F6*/ s_a = (uint8_t)((s_a + 0x0009) + s_status_c);
                block_id = 1632;
      break;
    case 1631:  // $34F6
      /*$34F6*/ s_a = ((uint8_t)adc_dec16(s_a, 0x09, s_status_c));
                block_id = 1632;
      break;
    case 1632:  // $34F8
      /*$34F8*/ ram_poke(0x0007, s_a);
      /*$34FC*/ ram_poke(0x3413, 0x00);
      /*$34FF*/ s_a = ram_peek(0x0073);
      /*$3501*/ s_status_c = 0x00;
      /*$3502*/ block_id = s_status_d ? 1634 : 1633;
      break;
    case 1633:  // $3502
      /*$3502*/ tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 + 0x0039) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x0039);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1635;
      break;
    case 1634:  // $3502
      /*$3502*/ tmp4_U16 = adc_dec16(s_a, 0x39, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 1635;
      break;
    case 1635:  // $3504
      /*$3504*/ ram_poke(0x3414, s_a);
      /*$3507*/ tmp3_U8 = ram_peek(0x0074);
      /*$3509*/ ram_poke(0x0008, tmp3_U8);
      /*$350B*/ ram_poke(0x0009, tmp3_U8);
      /*$3400*/ CYCLES(0x3400, 3);
                s_x = ram_peek(0x0004);
                block_id = 1561;
      break;
    case 1636:  // $3523
      /*$3523*/ CYCLES(0x3523, 6);
                io_poke(0xc010, s_a);
      /*$3526*/ s_x = 0x00;
                block_id = 1637;
      break;
    case 1637:  // $3528
      /*$3528*/ CYCLES(0x3528, 6);
                poll_input_select_mode(0x352a);
                branchTarget = true; block_id = 1638;
      break;
    case 1638:  // $352B
      /*$352B*/ CYCLES(0x352b, 2);
                branchTarget = true; block_id = s_status_n ? 1639 : 1640;
      break;
    case 1639:  // $352B
      /*$352B*/ CYCLES_EDGE(0x352b, 1);
      /*$3580*/ CYCLES(0x3580, 3);
      /*$AC83*/ CYCLES(0xac83, 54);
      /*$AC85*/ ram_poke(0x0080, 0x00);
      /*$AC87*/ ram_poke(0x0081, 0x00);
      /*$AC89*/ ram_poke(0x0082, 0x00);
      /*$AC8D*/ ram_poke(0x0086, 0x00);
      /*$AC91*/ ram_poke(0x0087, 0x04);
      /*$AC95*/ ram_poke(0x0088, 0x20);
      /*$AC99*/ ram_poke(0x0089, 0x01);
      /*$AC9D*/ ram_poke(0x008a, 0x27);
      /*$ACA1*/ ram_poke(0x008b, 0x40);
      /*$ACA5*/ ram_poke(0x008f, 0x28);
      /*$ACAC*/ branchTarget = true; block_id = !(ram_peek(0x0c00) != 0x40) ? 1752 : 1753;
      break;
    case 1640:  // $352D
      /*$352D*/ CYCLES(0x352d, 8);
                s_a = 0x40;
      /*$352F*/ delay_one_tick(0x3531);
                branchTarget = true; block_id = 1641;
      break;
    case 1641:  // $3532
      /*$3532*/ CYCLES(0x3532, 4);
                tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp1_U8;
      /*$3533*/ branchTarget = true; block_id = tmp1_U8 ? 1642 : 1643;
      break;
    case 1642:  // $3533
      /*$3533*/ CYCLES_EDGE(0x3533, 1);
                branchTarget = true; block_id = 1637;
      break;
    case 1643:  // $3535
      /*$3535*/ CYCLES(0x3535, 59);
      /*$3537*/ ram_poke(0x0086, 0x00);
      /*$353B*/ ram_poke(0x0087, 0x00);
      /*$353F*/ ram_poke(0x0088, 0x00);
      /*$3543*/ ram_poke(0x0089, 0x01);
      /*$3547*/ ram_poke(0x008a, 0x10);
      /*$354B*/ ram_poke(0x008b, 0x40);
      /*$354F*/ ram_poke(0x0080, 0x00);
      /*$3551*/ ram_poke(0x0081, 0x00);
      /*$3553*/ ram_poke(0x0082, 0x00);
      /*$3557*/ ram_poke(0x008f, 0x28);
      /*$355B*/ ram_poke(0x00ff, 0x5f);
      /*$355F*/ ram_poke(0x00a0, 0x00);
      /*$3561*/ branchTarget = true; block_id = 1005;
      break;
    case 1644:  // $3570
      /*$3570*/ CYCLES(0x3570, 6);
                rom_mon_wait(0x3572);
      /*$3573*/ CYCLES(0x3573, 11);
      /*$3574*/ push8(s_x);
      /*$3575*/ func_aec0(0x3577);
                branchTarget = true; block_id = 1645;
      break;
    case 1645:  // $3578
      /*$3578*/ CYCLES(0x3578, 12);
                tmp1_U8 = pop8();
                s_a = tmp1_U8;
      /*$3579*/ s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$357A*/ branchTarget = true; block_id = find_block_id_func_t001(0x357a, pop16() + 1);;
      break;
    case 1646:  // $3584
      /*$3584*/ CYCLES(0x3584, 10);
                tmp3_U8 = io_peek(0xc061);
      /*$3587*/ tmp1_U8 = io_peek(0xc062);
      /*$358A*/ branchTarget = true; block_id = ((tmp3_U8 & tmp1_U8) & 0x80) ? 1647 : 1648;
      break;
    case 1647:  // $358A
      /*$358A*/ CYCLES_EDGE(0x358a, 1);
                branchTarget = true; block_id = 1650;
      break;
    case 1648:  // $358C
      /*$358C*/ CYCLES(0x358c, 10);
                tmp3_U8 = io_peek(0xc061);
      /*$358F*/ tmp1_U8 = io_peek(0xc062);
      /*$3592*/ branchTarget = true; block_id = ((tmp3_U8 | tmp1_U8) & 0x80) ? 1649 : 1650;
      break;
    case 1649:  // $3592
      /*$3592*/ CYCLES_EDGE(0x3592, 1);
      /*$35F0*/ CYCLES(0x35f0, 3);
      /*$3700*/ CYCLES(0x3700, 108);
      /*$3702*/ ram_poke(0x138c, 0xad);
      /*$3705*/ ram_poke(0x1300, 0xad);
      /*$370A*/ ram_poke(0x138d, 0x62);
      /*$370F*/ ram_poke(0x1301, 0x61);
      /*$3714*/ ram_poke(0x138e, 0xc0);
      /*$3717*/ ram_poke(0x1302, 0xc0);
      /*$371C*/ ram_poke(0x138f, 0x30);
      /*$371F*/ ram_poke(0x1303, 0x30);
      /*$3724*/ ram_poke(0x1390, 0x05);
      /*$3729*/ ram_poke(0x1304, 0x07);
      /*$372E*/ ram_poke(0x1305, 0x60);
      /*$3731*/ ram_poke(0x1391, 0x60);
      /*$3736*/ ram_poke(0x1060, 0x20);
      /*$373B*/ ram_poke(0x1061, 0xb0);
      /*$3740*/ ram_poke(0x1062, 0x36);
      /*$3745*/ ram_poke(0x1063, 0x4c);
      /*$374A*/ ram_poke(0x1064, 0x80);
      /*$374F*/ ram_poke(0x1065, 0x11);
      /*$3752*/ s_status_not_z = 0x80;
                s_status_n = 0x80;
                s_a = 0x80;
      /*$3754*/ branchTarget = true; block_id = find_block_id_func_t001(0x3754, pop16() + 1);;
      break;
    case 1650:  // $3594
      /*$3594*/ CYCLES(0x3594, 6);
                tmp1_U8 = io_peek(0xc000);
                s_status_not_z = tmp1_U8;
                tmp3_U8 = tmp1_U8 & 0x80;
                s_status_n = tmp3_U8;
                s_a = tmp1_U8;
      /*$3597*/ branchTarget = true; block_id = tmp3_U8 ? 1651 : 1652;
      break;
    case 1651:  // $3597
      /*$3597*/ CYCLES_EDGE(0x3597, 1);
      /*$359A*/ CYCLES(0x359a, 110);
      /*$359C*/ ram_poke(0x138c, 0xad);
      /*$359F*/ ram_poke(0x1300, 0xad);
      /*$35A2*/ ram_poke(0x1060, 0xad);
      /*$35A7*/ ram_poke(0x138d, 0x00);
      /*$35AA*/ ram_poke(0x1301, 0x00);
      /*$35AD*/ ram_poke(0x1061, 0x00);
      /*$35B2*/ ram_poke(0x138e, 0xc0);
      /*$35B5*/ ram_poke(0x1302, 0xc0);
      /*$35B8*/ ram_poke(0x1062, 0xc0);
      /*$35BD*/ ram_poke(0x138f, 0xc9);
      /*$35C0*/ ram_poke(0x1065, 0xc9);
      /*$35C3*/ ram_poke(0x1305, 0xc9);
      /*$35C8*/ ram_poke(0x1390, 0xc1);
      /*$35CD*/ ram_poke(0x1391, 0xd0);
      /*$35D2*/ ram_poke(0x1392, 0x24);
      /*$35D7*/ ram_poke(0x1303, 0x10);
      /*$35DA*/ ram_poke(0x1063, 0x10);
      /*$35DF*/ ram_poke(0x1304, 0x2a);
      /*$35E4*/ ram_poke(0x1306, 0xda);
      /*$35E9*/ ram_poke(0x1064, 0x49);
      /*$35EC*/ s_status_not_z = 0x80;
                s_status_n = 0x80;
                s_a = 0x80;
      /*$35EE*/ branchTarget = true; block_id = find_block_id_func_t001(0x35ee, pop16() + 1);;
      break;
    case 1652:  // $3599
      /*$3599*/ CYCLES(0x3599, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x3599, pop16() + 1);;
      break;
    case 1653:  // $3600
      /*$3600*/ CYCLES(0x3600, 19);
      /*$3602*/ ram_poke(0x361c, 0x90);
      /*$3605*/ s_a = ram_peek(0x000a);
      /*$3607*/ s_status_c = 0x00;
      /*$3608*/ block_id = s_status_d ? 1655 : 1654;
      break;
    case 1654:  // $3608
      /*$3608*/ tmp4_U16 = s_a;
                tmp5_U16 = (tmp4_U16 + 0x0039) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x0039);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 1656;
      break;
    case 1655:  // $3608
      /*$3608*/ tmp5_U16 = adc_dec16(s_a, 0x39, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
                block_id = 1656;
      break;
    case 1656:  // $360A
      /*$360A*/ ram_poke(0x361d, s_a);
      /*$360D*/ s_x = 0x60;
                block_id = 1657;
      break;
    case 1657:  // $360F
      /*$360F*/ CYCLES(0x360f, 16);
                tmp1_U8 = s_x;
      /*$3612*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
      /*$3617*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
      /*$3619*/ s_y = 0x0c;
                block_id = 1658;
      break;
    case 1658:  // $361B
      /*$361B*/ CYCLES(0x361b, 22);
                tmp1_U8 = peek(ram_peek16al(0x361c));
                s_a = tmp1_U8;
      /*$361E*/ tmp3_U8 = s_y;
                poke((ram_peek16al(0x0000) + tmp3_U8), tmp1_U8);
      /*$3620*/ ram_poke(0x361c, (uint8_t)(ram_peek(0x361c) + 0x01));
      /*$3623*/ tmp3_U8 = (uint8_t)(tmp3_U8 + 0x01);
                s_y = tmp3_U8;
      /*$3626*/ branchTarget = true; block_id = !(tmp3_U8 >= 0x1c) ? 1659 : 1660;
      break;
    case 1659:  // $3626
      /*$3626*/ CYCLES_EDGE(0x3626, 1);
                branchTarget = true; block_id = 1658;
      break;
    case 1660:  // $3628
      /*$3628*/ CYCLES(0x3628, 6);
                tmp3_U8 = s_x;
                tmp1_U8 = (uint8_t)(tmp3_U8 + 0x01);
                s_x = tmp1_U8;
      /*$3629*/ s_status_not_z = (tmp1_U8 != 0x67);
                tmp1_U8 = tmp1_U8 >= 0x67;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp3_U8 - 0x66) & 0x80);
      /*$362B*/ branchTarget = true; block_id = !tmp1_U8 ? 1661 : 1662;
      break;
    case 1661:  // $362B
      /*$362B*/ CYCLES_EDGE(0x362b, 1);
                branchTarget = true; block_id = 1657;
      break;
    case 1662:  // $362D
      /*$362D*/ CYCLES(0x362d, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x362d, pop16() + 1);;
      break;
    case 1663:  // $3630
      /*$3630*/ CYCLES(0x3630, 5);
      /*$3632*/ ram_poke(0x000a, 0x00);
                block_id = 1664;
      break;
    case 1664:  // $3634
      /*$3634*/ CYCLES(0x3634, 6);
                func_3600(0x3636);
      /*$3637*/ CYCLES(0x3637, 6);
                func_3650(0x3639);
      /*$363A*/ CYCLES(0x363a, 12);
                ram_poke(0x000a, (uint8_t)(ram_peek(0x000a) + 0x01));
      /*$363E*/ tmp1_U8 = ram_peek(0x000a) >= 0x07;
                s_status_c = tmp1_U8;
      /*$3640*/ branchTarget = true; block_id = !tmp1_U8 ? 1665 : 1666;
      break;
    case 1665:  // $3640
      /*$3640*/ CYCLES_EDGE(0x3640, 1);
                branchTarget = true; block_id = 1664;
      break;
    case 1666:  // $3642
      /*$3642*/ CYCLES(0x3642, 2);
                s_x = 0x04;
                block_id = 1667;
      break;
    case 1667:  // $3644
      /*$3644*/ CYCLES(0x3644, 8);
                s_a = 0x00;
      /*$3646*/ rom_mon_wait(0x3648);
      /*$3649*/ CYCLES(0x3649, 4);
                tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_status_not_z = tmp1_U8;
                s_status_n = (tmp1_U8 & 0x80);
                s_x = tmp1_U8;
      /*$364A*/ branchTarget = true; block_id = tmp1_U8 ? 1668 : 1669;
      break;
    case 1668:  // $364A
      /*$364A*/ CYCLES_EDGE(0x364a, 1);
                branchTarget = true; block_id = 1667;
      break;
    case 1669:  // $364C
      /*$364C*/ CYCLES(0x364c, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x364c, pop16() + 1);;
      break;
    case 1670:  // $3650
      /*$3650*/ CYCLES(0x3650, 2);
                s_x = 0x01;
                block_id = 1671;
      break;
    case 1671:  // $3652
      /*$3652*/ CYCLES(0x3652, 8);
                s_a = 0x80;
      /*$3654*/ rom_mon_wait(0x3656);
      /*$3657*/ CYCLES(0x3657, 12);
                poke(ram_peek16al(0x3658), s_a);
      /*$365A*/ s_a = 0x50;
      /*$365C*/ rom_mon_wait(0x365e);
      /*$365F*/ CYCLES(0x365f, 8);
                poke(ram_peek16al(0x3660), s_a);
      /*$3662*/ tmp1_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp1_U8;
      /*$3663*/ branchTarget = true; block_id = tmp1_U8 ? 1672 : 1673;
      break;
    case 1672:  // $3663
      /*$3663*/ CYCLES_EDGE(0x3663, 1);
                branchTarget = true; block_id = 1671;
      break;
    case 1673:  // $3665
      /*$3665*/ CYCLES(0x3665, 5);
                s_a = 0x00;
      /*$3667*/ rom_mon_wait(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x3667, pop16() + 1);;
      break;
    case 1674:  // $37A0
      /*$37A0*/ CYCLES(0x37a0, 29);
      /*$37A2*/ ram_poke(0x37c5, 0xf0);
      /*$37A5*/ s_a = ram_peek(0x0006);
      /*$37A7*/ s_status_c = 0x00;
      /*$37A8*/ block_id = s_status_d ? 1676 : 1675;
      break;
    case 1675:  // $37A8
      /*$37A8*/ s_a = (uint8_t)((s_a + 0x0060) + s_status_c);
                block_id = 1677;
      break;
    case 1676:  // $37A8
      /*$37A8*/ s_a = ((uint8_t)adc_dec16(s_a, 0x60, s_status_c));
                block_id = 1677;
      break;
    case 1677:  // $37AA
      /*$37AA*/ ram_poke(0x37c6, s_a);
      /*$37AD*/ s_a = ram_peek(0x0004);
      /*$37AF*/ s_status_c = 0x00;
      /*$37B0*/ block_id = s_status_d ? 1679 : 1678;
      break;
    case 1678:  // $37B0
      /*$37B0*/ tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 + 0x0005) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0x0005);
                s_a = ((uint8_t)tmp4_U16);
                block_id = 1680;
      break;
    case 1679:  // $37B0
      /*$37B0*/ tmp4_U16 = adc_dec16(s_a, 0x05, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                s_status_v = (((uint8_t)(tmp4_U16 >> 8) & 0x40) != 0);
                block_id = 1680;
      break;
    case 1680:  // $37B2
      /*$37B2*/ ram_poke(0x0007, s_a);
      /*$37B4*/ s_x = 0x00;
                block_id = 1681;
      break;
    case 1681:  // $37B6
      /*$37B6*/ CYCLES(0x37b6, 50);
                tmp1_U8 = ram_peek(0x0004);
      /*$37BB*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
      /*$37C0*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
      /*$37C2*/ tmp1_U8 = ram_peek(0x0005);
                s_y = tmp1_U8;
      /*$37C4*/ tmp3_U8 = s_x;
                tmp2_U8 = peek((ram_peek16(0x37c5) + tmp3_U8));
      /*$37C7*/ tmp7_U8 = peek((ram_peek16al(0x0000) + tmp1_U8));
      /*$37C9*/ poke((ram_peek16al(0x0000) + tmp1_U8), (tmp2_U8 | tmp7_U8));
      /*$37CB*/ s_x = (uint8_t)(tmp3_U8 + 0x01);
      /*$37CC*/ ram_poke(0x0004, (uint8_t)(ram_peek(0x0004) + 0x01));
      /*$37CE*/ tmp3_U8 = ram_peek(0x0004);
                s_a = tmp3_U8;
      /*$37D0*/ tmp1_U8 = ram_peek(0x0007);
                s_status_not_z = (tmp3_U8 != tmp1_U8);
                tmp7_U8 = tmp3_U8 >= tmp1_U8;
                s_status_c = tmp7_U8;
                s_status_n = ((uint8_t)(tmp3_U8 - tmp1_U8) & 0x80);
      /*$37D2*/ branchTarget = true; block_id = !tmp7_U8 ? 1682 : 1683;
      break;
    case 1682:  // $37D2
      /*$37D2*/ CYCLES_EDGE(0x37d2, 1);
                branchTarget = true; block_id = 1681;
      break;
    case 1683:  // $37D4
      /*$37D4*/ CYCLES(0x37d4, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x37d4, pop16() + 1);;
      break;
    case 1684:  // $3802
      /*$3802*/ CYCLES(0x3802, 12);
                tmp7_U8 = s_x;
      /*$3804*/ ram_poke((uint8_t)(0x04 + tmp7_U8), ram_peek((uint8_t)(ram_peek(0x3803) + tmp7_U8)));
      /*$3806*/ tmp7_U8 = (uint8_t)(tmp7_U8 - 0x01);
                s_x = tmp7_U8;
      /*$3807*/ branchTarget = true; block_id = !(tmp7_U8 & 0x80) ? 1685 : 1686;
      break;
    case 1685:  // $3807
      /*$3807*/ CYCLES_EDGE(0x3807, 1);
                branchTarget = true; block_id = 1684;
      break;
    case 1686:  // $3809
      /*$3809*/ CYCLES(0x3809, 6);
                func_3860(0x380b);
                branchTarget = true; block_id = 1687;
      break;
    case 1687:  // $380C
      /*$380C*/ CYCLES(0x380c, 5);
      /*$380E*/ branchTarget = true; block_id = ram_peek(0x00a0) ? 1688 : 1689;
      break;
    case 1688:  // $380E
      /*$380E*/ CYCLES_EDGE(0x380e, 1);
                branchTarget = true; block_id = 1708;
      break;
    case 1689:  // $3810
      /*$3810*/ CYCLES(0x3810, 12);
                ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) + 0x01));
      /*$3812*/ tmp7_U8 = ram_peek(0x0006);
                s_a = tmp7_U8;
      /*$3816*/ branchTarget = true; block_id = !(tmp7_U8 >= 0x07) ? 1690 : 1691;
      break;
    case 1690:  // $3816
      /*$3816*/ CYCLES_EDGE(0x3816, 1);
                branchTarget = true; block_id = 1692;
      break;
    case 1691:  // $3818
      /*$3818*/ CYCLES(0x3818, 2);
                s_a = 0x00;
                block_id = 1692;
      break;
    case 1692:  // $381A
      /*$381A*/ CYCLES(0x381a, 15);
                ram_poke(0x0006, s_a);
      /*$381C*/ ram_poke(0x0004, (uint8_t)(ram_peek(0x0004) - 0x01));
      /*$3822*/ branchTarget = true; block_id = (ram_peek(0x0004) >= ram_peek(0x3821)) ? 1693 : 1694;
      break;
    case 1693:  // $3822
      /*$3822*/ CYCLES_EDGE(0x3822, 1);
                branchTarget = true; block_id = 1704;
      break;
    case 1694:  // $3824
      /*$3824*/ CYCLES(0x3824, 18);
      /*$3826*/ ram_poke(0x0004, 0xa0);
      /*$3828*/ s_a = ram_peek(0x0005);
      /*$382A*/ s_status_c = 0x00;
      /*$382B*/ block_id = s_status_d ? 1696 : 1695;
      break;
    case 1695:  // $382B
      /*$382B*/ s_a = (uint8_t)((s_a + 0x0008) + s_status_c);
                block_id = 1697;
      break;
    case 1696:  // $382B
      /*$382B*/ s_a = ((uint8_t)adc_dec16(s_a, 0x08, s_status_c));
                block_id = 1697;
      break;
    case 1697:  // $382D
      /*$3830*/ branchTarget = true; block_id = !(s_a >= 0x28) ? 1698 : 1699;
      break;
    case 1698:  // $3830
      /*$3830*/ CYCLES_EDGE(0x3830, 1);
                branchTarget = true; block_id = 1702;
      break;
    case 1699:  // $3832
      /*$3832*/ CYCLES(0x3832, 4);
                s_status_c = 0x01;
      /*$3833*/ block_id = s_status_d ? 1701 : 1700;
      break;
    case 1700:  // $3833
      /*$3833*/ s_a = (uint8_t)((s_a - 0x0028) - (uint8_t)(0x01 - s_status_c));
                block_id = 1703;
      break;
    case 1701:  // $3833
      /*$3833*/ s_a = ((uint8_t)sbc_dec16(s_a, 0x28, s_status_c));
                block_id = 1703;
      break;
    case 1702:  // $3835
      /*$3835*/ CYCLES(0x3835, 3);
                ram_poke(0x0005, s_a);
                block_id = 1704;
      break;
    case 1703:  // $3835
      /*$3833*/ block_id = 1702;
      break;
    case 1704:  // $3837
      /*$3837*/ CYCLES(0x3837, 2);
                s_x = 0x02;
                block_id = 1705;
      break;
    case 1705:  // $3839
      /*$3839*/ CYCLES(0x3839, 12);
                tmp7_U8 = s_x;
      /*$383B*/ ram_poke((uint8_t)(ram_peek(0x383c) + tmp7_U8), ram_peek((uint8_t)(0x04 + tmp7_U8)));
      /*$383D*/ tmp7_U8 = (uint8_t)(tmp7_U8 - 0x01);
                s_x = tmp7_U8;
      /*$383E*/ branchTarget = true; block_id = !(tmp7_U8 & 0x80) ? 1706 : 1707;
      break;
    case 1706:  // $383E
      /*$383E*/ CYCLES_EDGE(0x383e, 1);
                branchTarget = true; block_id = 1705;
      break;
    case 1707:  // $3840
      /*$3840*/ CYCLES(0x3840, 6);
                func_37a0(0x3842);
                branchTarget = true; block_id = 1708;
      break;
    case 1708:  // $3843
      /*$3843*/ CYCLES(0x3843, 12);
                s_a = ram_peek(0x3803);
      /*$3846*/ s_status_c = 0x00;
      /*$3847*/ block_id = s_status_d ? 1710 : 1709;
      break;
    case 1709:  // $3847
      /*$3847*/ tmp4_U16 = s_a;
                tmp5_U16 = (tmp4_U16 + 0x0003) + s_status_c;
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp4_U16, (uint8_t)0x0003);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 1711;
      break;
    case 1710:  // $3847
      /*$3847*/ tmp5_U16 = adc_dec16(s_a, 0x03, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_v = (((uint8_t)(tmp5_U16 >> 8) & 0x40) != 0);
                block_id = 1711;
      break;
    case 1711:  // $3849
      /*$384B*/ branchTarget = true; block_id = !(s_a >= 0xb9) ? 1712 : 1713;
      break;
    case 1712:  // $384B
      /*$384B*/ CYCLES_EDGE(0x384b, 1);
                branchTarget = true; block_id = 1714;
      break;
    case 1713:  // $384D
      /*$384D*/ CYCLES(0x384d, 2);
                s_a = 0xb0;
                block_id = 1714;
      break;
    case 1714:  // $384F
      /*$384F*/ CYCLES(0x384f, 12);
                tmp7_U8 = s_a;
                ram_poke(0x3803, tmp7_U8);
      /*$3852*/ ram_poke(0x383c, tmp7_U8);
      /*$3855*/ tmp1_U8 = tmp7_U8 != 0xb0;
                s_status_not_z = tmp1_U8;
                s_status_c = (tmp7_U8 >= 0xb0);
                s_status_n = ((uint8_t)(tmp7_U8 - 0xb0) & 0x80);
      /*$3857*/ branchTarget = true; block_id = tmp1_U8 ? 1715 : 1716;
      break;
    case 1715:  // $3857
      /*$3857*/ CYCLES_EDGE(0x3857, 1);
      /*$3800*/ CYCLES(0x3800, 2);
                s_x = 0x02;
                block_id = 1684;
      break;
    case 1716:  // $3859
      /*$3859*/ CYCLES(0x3859, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0x3859, pop16() + 1);;
      break;
    case 1717:  // $3860
      /*$3860*/ CYCLES(0x3860, 29);
      /*$3862*/ ram_poke(0x3885, 0xf0);
      /*$3865*/ s_a = ram_peek(0x0006);
      /*$3867*/ s_status_c = 0x00;
      /*$3868*/ block_id = s_status_d ? 1719 : 1718;
      break;
    case 1718:  // $3868
      /*$3868*/ s_a = (uint8_t)((s_a + 0x0060) + s_status_c);
                block_id = 1720;
      break;
    case 1719:  // $3868
      /*$3868*/ s_a = ((uint8_t)adc_dec16(s_a, 0x60, s_status_c));
                block_id = 1720;
      break;
    case 1720:  // $386A
      /*$386A*/ ram_poke(0x3886, s_a);
      /*$386D*/ s_a = ram_peek(0x0004);
      /*$386F*/ s_status_c = 0x00;
      /*$3870*/ block_id = s_status_d ? 1722 : 1721;
      break;
    case 1721:  // $3870
      /*$3870*/ s_a = (uint8_t)((s_a + 0x0005) + s_status_c);
                block_id = 1723;
      break;
    case 1722:  // $3870
      /*$3870*/ s_a = ((uint8_t)adc_dec16(s_a, 0x05, s_status_c));
                block_id = 1723;
      break;
    case 1723:  // $3872
      /*$3872*/ ram_poke(0x0007, s_a);
      /*$3874*/ s_x = 0x00;
                block_id = 1724;
      break;
    case 1724:  // $3876
      /*$3876*/ CYCLES(0x3876, 52);
                tmp1_U8 = ram_peek(0x0004);
      /*$387B*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
      /*$3880*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
      /*$3882*/ tmp1_U8 = ram_peek(0x0005);
                s_y = tmp1_U8;
      /*$3884*/ tmp7_U8 = s_x;
                tmp3_U8 = peek((ram_peek16(0x3885) + tmp7_U8));
      /*$3889*/ tmp2_U8 = peek((ram_peek16al(0x0000) + tmp1_U8));
      /*$388B*/ poke((ram_peek16al(0x0000) + tmp1_U8), ((tmp3_U8 ^ 0xff) & tmp2_U8));
      /*$388D*/ s_x = (uint8_t)(tmp7_U8 + 0x01);
      /*$388E*/ ram_poke(0x0004, (uint8_t)(ram_peek(0x0004) + 0x01));
      /*$3890*/ tmp7_U8 = ram_peek(0x0004);
                s_a = tmp7_U8;
      /*$3894*/ branchTarget = true; block_id = !(tmp7_U8 >= ram_peek(0x0007)) ? 1725 : 1726;
      break;
    case 1725:  // $3894
      /*$3894*/ CYCLES_EDGE(0x3894, 1);
                branchTarget = true; block_id = 1724;
      break;
    case 1726:  // $3896
      /*$3896*/ CYCLES(0x3896, 13);
                s_status_c = 0x01;
      /*$3897*/ block_id = s_status_d ? 1728 : 1727;
      break;
    case 1727:  // $3897
      /*$3897*/ tmp5_U16 = s_a;
                tmp4_U16 = (tmp5_U16 - 0x0005) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp4_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp5_U16, (uint8_t)0xfffa);
                tmp7_U8 = (uint8_t)tmp4_U16;
                s_status_not_z = tmp7_U8;
                s_status_n = (tmp7_U8 & 0x80);
                s_a = tmp7_U8;
                block_id = 1729;
      break;
    case 1728:  // $3897
      /*$3897*/ tmp4_U16 = sbc_dec16(s_a, 0x05, s_status_c);
                s_a = ((uint8_t)tmp4_U16);
                tmp7_U8 = (uint8_t)(tmp4_U16 >> 8);
                s_status_c = (tmp7_U8 & 0x01);
                s_status_not_z = (~tmp7_U8 & 2);
                s_status_v = ((tmp7_U8 & 0x40) != 0);
                s_status_n = (tmp7_U8 & 0x80);
                block_id = 1729;
      break;
    case 1729:  // $3899
      /*$3899*/ ram_poke(0x0004, s_a);
      /*$389B*/ branchTarget = true; block_id = find_block_id_func_t001(0x389b, pop16() + 1);;
      break;
    case 1730:  // $AC04
      /*$AC04*/ CYCLES_EDGE(0xac04, 1);
                branchTarget = true; block_id = 1735;
      break;
    case 1731:  // $AC06
      /*$AC06*/ CYCLES(0xac06, 2);
                branchTarget = true; block_id = s_status_c ? 1732 : 1733;
      break;
    case 1732:  // $AC06
      /*$AC06*/ CYCLES_EDGE(0xac06, 1);
                branchTarget = true; block_id = 1748;
      break;
    case 1733:  // $AC08
      /*$AC08*/ CYCLES(0xac08, 2);
                branchTarget = true; block_id = !s_status_c ? 1734 : 1735;
      break;
    case 1734:  // $AC08
      /*$AC08*/ CYCLES_EDGE(0xac08, 1);
                branchTarget = true; block_id = 1747;
      break;
    case 1735:  // $AC0A
      /*$AC0A*/ CYCLES(0xac0a, 8);
                tmp7_U8 = ram_peek(0x0084);
                s_a = tmp7_U8;
      /*$AC0C*/ tmp1_U8 = ram_peek(0x0081);
                tmp3_U8 = tmp7_U8 != tmp1_U8;
                s_status_not_z = tmp3_U8;
                s_status_c = (tmp7_U8 >= tmp1_U8);
                s_status_n = ((uint8_t)(tmp7_U8 - tmp1_U8) & 0x80);
      /*$AC0E*/ branchTarget = true; block_id = !tmp3_U8 ? 1736 : 1737;
      break;
    case 1736:  // $AC0E
      /*$AC0E*/ CYCLES_EDGE(0xac0e, 1);
                branchTarget = true; block_id = 1741;
      break;
    case 1737:  // $AC10
      /*$AC10*/ CYCLES(0xac10, 2);
                branchTarget = true; block_id = s_status_c ? 1738 : 1739;
      break;
    case 1738:  // $AC10
      /*$AC10*/ CYCLES_EDGE(0xac10, 1);
                branchTarget = true; block_id = 1748;
      break;
    case 1739:  // $AC12
      /*$AC12*/ CYCLES(0xac12, 2);
                branchTarget = true; block_id = !s_status_c ? 1740 : 1741;
      break;
    case 1740:  // $AC12
      /*$AC12*/ CYCLES_EDGE(0xac12, 1);
                branchTarget = true; block_id = 1747;
      break;
    case 1741:  // $AC14
      /*$AC14*/ CYCLES(0xac14, 8);
                tmp3_U8 = ram_peek(0x0083);
                s_a = tmp3_U8;
      /*$AC16*/ tmp1_U8 = ram_peek(0x0080);
                s_status_not_z = (tmp3_U8 != tmp1_U8);
                tmp7_U8 = tmp3_U8 >= tmp1_U8;
                s_status_c = tmp7_U8;
                s_status_n = ((uint8_t)(tmp3_U8 - tmp1_U8) & 0x80);
      /*$AC18*/ branchTarget = true; block_id = tmp7_U8 ? 1742 : 1743;
      break;
    case 1742:  // $AC18
      /*$AC18*/ CYCLES_EDGE(0xac18, 1);
                branchTarget = true; block_id = 1748;
      break;
    case 1743:  // $AC1A
      /*$AC1A*/ CYCLES(0xac1a, 2);
                branchTarget = true; block_id = !s_status_c ? 1744 : 1745;
      break;
    case 1744:  // $AC1A
      /*$AC1A*/ CYCLES_EDGE(0xac1a, 1);
                branchTarget = true; block_id = 1747;
      break;
    case 1745:  // $AC1C
      /*$AC1C*/ CYCLES(0xac1c, 2);
                branchTarget = true; block_id = !s_status_c ? 1746 : 1747;
      break;
    case 1746:  // $AC1C
      /*$AC1C*/ CYCLES_EDGE(0xac1c, 1);
      /*$AC00*/ CYCLES(0xac00, 8);
                tmp7_U8 = ram_peek(0x0085);
                s_a = tmp7_U8;
      /*$AC02*/ tmp1_U8 = ram_peek(0x0082);
                tmp3_U8 = tmp7_U8 != tmp1_U8;
                s_status_not_z = tmp3_U8;
                s_status_c = (tmp7_U8 >= tmp1_U8);
                s_status_n = ((uint8_t)(tmp7_U8 - tmp1_U8) & 0x80);
      /*$AC04*/ branchTarget = true; block_id = !tmp3_U8 ? 1730 : 1731;
      break;
    case 1747:  // $AC1E
      /*$AC1E*/ CYCLES(0xac1e, 18);
      /*$AC20*/ ram_poke(0x0083, ram_peek(0x0080));
      /*$AC24*/ ram_poke(0x0084, ram_peek(0x0081));
      /*$AC26*/ tmp3_U8 = ram_peek(0x0082);
                s_status_not_z = tmp3_U8;
                s_status_n = (tmp3_U8 & 0x80);
                s_a = tmp3_U8;
      /*$AC28*/ ram_poke(0x0085, tmp3_U8);
                block_id = 1748;
      break;
    case 1748:  // $AC2A
      /*$AC2A*/ CYCLES(0xac2a, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xac2a, pop16() + 1);;
      break;
    case 1749:  // $AC32
      /*$AC32*/ CYCLES_EDGE(0xac32, 1);
      /*$AC4B*/ CYCLES(0xac4b, 6);
                func_3630(0xac4d);
      /*$AC4E*/ CYCLES(0xac4e, 11);
                ram_poke(0x0087, (uint8_t)(ram_peek(0x0087) + 0x01));
      /*$AC50*/ func_ac00(0xac52);
                branchTarget = true; block_id = 1751;
      break;
    case 1750:  // $AC34
      /*$AC34*/ CYCLES(0xac34, 23);
      /*$AC36*/ ram_poke(0x0089, 0x01);
      /*$AC3A*/ ram_poke(0x008a, 0x27);
      /*$AC3E*/ ram_poke(0x008b, 0x40);
      /*$AC41*/ ram_poke(0x0088, 0x20);
      /*$AC43*/ branchTarget = true; block_id = 1005;
      break;
    case 1751:  // $AC53
      /*$AC53*/ CYCLES(0xac53, 6);
                func_baf0(0xac55);
      /*$AC56*/ CYCLES(0xac56, 3);
      /*$3520*/ CYCLES(0x3520, 6);
                func_1e00(0x3522);
                branchTarget = true; block_id = 1636;
      break;
    case 1752:  // $ACAC
      /*$ACAC*/ CYCLES_EDGE(0xacac, 1);
                branchTarget = true; block_id = 1754;
      break;
    case 1753:  // $ACAE
      /*$ACAE*/ CYCLES(0xacae, 6);
                func_2f80(0xacb0);
      /*$ACB1*/ CYCLES(0xacb1, 6);
                func_2f00(0xacb3);
                branchTarget = true; block_id = 1754;
      break;
    case 1754:  // $ACB4
      /*$ACB4*/ CYCLES(0xacb4, 24);
                tmp3_U8 = io_peek(0xc010);
                s_a = tmp3_U8;
      /*$ACB7*/ s_status_c = 0x00;
      /*$ACB8*/ block_id = s_status_d ? 1756 : 1755;
      break;
    case 1755:  // $ACB8
      /*$ACB8*/ s_a = (uint8_t)((s_a + ram_peek(0x00ff)) + s_status_c);
                block_id = 1757;
      break;
    case 1756:  // $ACB8
      /*$ACB8*/ s_a = ((uint8_t)adc_dec16(s_a, ram_peek(0x00ff), s_status_c));
                block_id = 1757;
      break;
    case 1757:  // $ACBA
      /*$ACBC*/ ram_poke(0x00ff, (uint8_t)((s_a << 0x01) | 0x0001));
      /*$ACC0*/ ram_poke(0x00a0, 0x00);
      /*$ACC2*/ branchTarget = true; block_id = 1005;
      break;
    case 1758:  // $AD00
      /*$AD00*/ CYCLES(0xad00, 2);
                s_x = 0xb8;
                block_id = 1759;
      break;
    case 1759:  // $AD02
      /*$AD02*/ CYCLES(0xad02, 16);
                tmp3_U8 = s_x;
      /*$AD05*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp3_U8)));
      /*$AD0A*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp3_U8)));
      /*$AD0C*/ s_y = 0x20;
                block_id = 1760;
      break;
    case 1760:  // $AD0E
      /*$AD0E*/ CYCLES(0xad0e, 22);
                tmp3_U8 = peek(ram_peek16(0xad0f));
                s_a = tmp3_U8;
      /*$AD11*/ tmp1_U8 = s_y;
                poke((ram_peek16al(0x0000) + tmp1_U8), tmp3_U8);
      /*$AD13*/ ram_poke(0xad0f, (uint8_t)(ram_peek(0xad0f) + 0x01));
      /*$AD16*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
                s_y = tmp1_U8;
      /*$AD19*/ branchTarget = true; block_id = !(tmp1_U8 >= 0x28) ? 1761 : 1762;
      break;
    case 1761:  // $AD19
      /*$AD19*/ CYCLES_EDGE(0xad19, 1);
                branchTarget = true; block_id = 1760;
      break;
    case 1762:  // $AD1B
      /*$AD1B*/ CYCLES(0xad1b, 6);
                tmp1_U8 = s_x;
                tmp3_U8 = (uint8_t)(tmp1_U8 + 0x01);
                s_x = tmp3_U8;
      /*$AD1C*/ s_status_not_z = (tmp3_U8 != 0xc0);
                tmp3_U8 = tmp3_U8 >= 0xc0;
                s_status_c = tmp3_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - 0xbf) & 0x80);
      /*$AD1E*/ branchTarget = true; block_id = !tmp3_U8 ? 1763 : 1764;
      break;
    case 1763:  // $AD1E
      /*$AD1E*/ CYCLES_EDGE(0xad1e, 1);
                branchTarget = true; block_id = 1759;
      break;
    case 1764:  // $AD20
      /*$AD20*/ CYCLES(0xad20, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xad20, pop16() + 1);;
      break;
    case 1765:  // $AD40
      /*$AD40*/ CYCLES(0xad40, 15);
      /*$AD42*/ ram_poke(0xad0f, 0x80);
      /*$AD47*/ ram_poke(0xad10, 0x6e);
      /*$AD4A*/ branchTarget = true; block_id = 1758;
      break;
    case 1766:  // $AD50
      /*$AD50*/ CYCLES(0xad50, 15);
      /*$AD52*/ ram_poke(0xad0f, 0xc0);
      /*$AD57*/ ram_poke(0xad10, 0x6e);
      /*$AD5A*/ branchTarget = true; block_id = 1758;
      break;
    case 1767:  // $AE00
      /*$AE00*/ CYCLES(0xae00, 3);
                s_x = ram_peek(0x0004);
                block_id = 1768;
      break;
    case 1768:  // $AE02
      /*$AE02*/ CYCLES(0xae02, 41);
                tmp3_U8 = s_x;
      /*$AE05*/ ram_poke(0x0001, ram_peek((0xaf40 + tmp3_U8)));
      /*$AE0A*/ ram_poke(0x0000, ram_peek((0xaf00 + tmp3_U8)));
      /*$AE0C*/ tmp3_U8 = ram_peek(0x0006);
      /*$AE14*/ ram_poke(0x0003, ram_peek((0x0c00 + tmp3_U8)));
      /*$AE19*/ ram_poke(0x0002, ram_peek((0x0d00 + tmp3_U8)));
      /*$AE1D*/ branchTarget = true; block_id = ram_peek(0xae1c) ? 1769 : 1770;
      break;
    case 1769:  // $AE1D
      /*$AE1D*/ CYCLES_EDGE(0xae1d, 1);
                branchTarget = true; block_id = 1775;
      break;
    case 1770:  // $AE1F
      /*$AE1F*/ CYCLES(0xae1f, 3);
                s_y = ram_peek(0x0008);
                block_id = 1771;
      break;
    case 1771:  // $AE21
      /*$AE21*/ CYCLES(0xae21, 18);
                tmp3_U8 = s_y;
                tmp1_U8 = peek((ram_peek16al(0x0000) + tmp3_U8));
      /*$AE23*/ poke((ram_peek16al(0x0002) + tmp3_U8), tmp1_U8);
      /*$AE25*/ tmp3_U8 = (uint8_t)(tmp3_U8 + 0x01);
                s_y = tmp3_U8;
      /*$AE26*/ tmp3_U8 = tmp3_U8 >= ram_peek(0x0009);
                s_status_c = tmp3_U8;
      /*$AE28*/ branchTarget = true; block_id = !tmp3_U8 ? 1772 : 1773;
      break;
    case 1772:  // $AE28
      /*$AE28*/ CYCLES_EDGE(0xae28, 1);
                branchTarget = true; block_id = 1771;
      break;
    case 1773:  // $AE2A
      /*$AE2A*/ CYCLES(0xae2a, 2);
                branchTarget = true; block_id = s_status_c ? 1774 : 1775;
      break;
    case 1774:  // $AE2A
      /*$AE2A*/ CYCLES_EDGE(0xae2a, 1);
                branchTarget = true; block_id = 1778;
      break;
    case 1775:  // $AE2C
      /*$AE2C*/ CYCLES(0xae2c, 3);
                s_y = ram_peek(0x0008);
                block_id = 1776;
      break;
    case 1776:  // $AE2E
      /*$AE2E*/ CYCLES(0xae2e, 23);
                tmp3_U8 = s_y;
                tmp1_U8 = peek((ram_peek16al(0x0000) + tmp3_U8));
      /*$AE30*/ tmp7_U8 = peek((ram_peek16al(0x0002) + tmp3_U8));
      /*$AE32*/ poke((ram_peek16al(0x0002) + tmp3_U8), (tmp1_U8 | tmp7_U8));
      /*$AE34*/ tmp3_U8 = (uint8_t)(tmp3_U8 + 0x01);
                s_y = tmp3_U8;
      /*$AE37*/ branchTarget = true; block_id = !(tmp3_U8 >= ram_peek(0x0009)) ? 1777 : 1778;
      break;
    case 1777:  // $AE37
      /*$AE37*/ CYCLES_EDGE(0xae37, 1);
                branchTarget = true; block_id = 1776;
      break;
    case 1778:  // $AE39
      /*$AE39*/ CYCLES(0xae39, 15);
                s_x = (uint8_t)(s_x + 0x01);
      /*$AE3A*/ s_a = ram_peek(0x000b);
      /*$AE3C*/ s_status_c = 0x00;
      /*$AE3D*/ block_id = s_status_d ? 1780 : 1779;
      break;
    case 1779:  // $AE3D
      /*$AE3D*/ tmp4_U16 = s_a;
                tmp5_U16 = ram_peek(0x000a);
                tmp6_U16 = (tmp4_U16 + tmp5_U16) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp4_U16, (uint8_t)tmp5_U16);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1781;
      break;
    case 1780:  // $AE3D
      /*$AE3D*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x000a), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp3_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp3_U8 & 0x01);
                s_status_v = ((tmp3_U8 & 0x40) != 0);
                block_id = 1781;
      break;
    case 1781:  // $AE3F
      /*$AE3F*/ ram_poke(0x000b, s_a);
      /*$AE41*/ branchTarget = true; block_id = !s_status_c ? 1782 : 1783;
      break;
    case 1782:  // $AE41
      /*$AE41*/ CYCLES_EDGE(0xae41, 1);
                branchTarget = true; block_id = 1785;
      break;
    case 1783:  // $AE43
      /*$AE43*/ CYCLES(0xae43, 11);
                ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) - 0x01));
      /*$AE45*/ func_ae58(0xae47);
      /*$AE48*/ CYCLES(0xae48, 2);
                branchTarget = true; block_id = s_status_not_z ? 1784 : 1785;
      break;
    case 1784:  // $AE48
      /*$AE48*/ CYCLES_EDGE(0xae48, 1);
                branchTarget = true; block_id = 1786;
      break;
    case 1785:  // $AE4A
      /*$AE4A*/ CYCLES(0xae4a, 6);
                s_a = 0x00;
      /*$AE4C*/ ram_poke(0xae1c, 0x00);
                block_id = 1786;
      break;
    case 1786:  // $AE4F
      /*$AE4F*/ CYCLES(0xae4f, 10);
                ram_poke(0x0006, (uint8_t)(ram_peek(0x0006) + 0x01));
      /*$AE51*/ tmp3_U8 = s_x;
                tmp7_U8 = ram_peek(0x0005);
                s_status_not_z = (tmp3_U8 != tmp7_U8);
                tmp1_U8 = tmp3_U8 >= tmp7_U8;
                s_status_c = tmp1_U8;
                s_status_n = ((uint8_t)(tmp3_U8 - tmp7_U8) & 0x80);
      /*$AE53*/ branchTarget = true; block_id = !tmp1_U8 ? 1787 : 1788;
      break;
    case 1787:  // $AE53
      /*$AE53*/ CYCLES_EDGE(0xae53, 1);
                branchTarget = true; block_id = 1768;
      break;
    case 1788:  // $AE55
      /*$AE55*/ CYCLES(0xae55, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xae55, pop16() + 1);;
      break;
    case 1789:  // $AE58
      /*$AE58*/ CYCLES(0xae58, 16);
                tmp3_U8 = (uint8_t)(ram_peek(0xae1c) + 0x01);
                s_status_not_z = tmp3_U8;
                s_status_n = (tmp3_U8 & 0x80);
                ram_poke(0xae1c, tmp3_U8);
      /*$AE5B*/ poke(ram_peek16al(0xae5c), s_a);
      /*$AE5E*/ branchTarget = true; block_id = find_block_id_func_t001(0xae5e, pop16() + 1);;
      break;
    case 1790:  // $AEA4
      /*$AEA4*/ CYCLES_EDGE(0xaea4, 1);
                branchTarget = true; block_id = 1792;
      break;
    case 1791:  // $AEA6
      /*$AEA6*/ CYCLES(0xaea6, 6);
                func_aec0(0xaea8);
                branchTarget = true; block_id = 1792;
      break;
    case 1792:  // $AEA9
      /*$AEA9*/ CYCLES(0xaea9, 12);
                s_a = ram_peek(0x000a);
      /*$AEAB*/ s_status_c = 0x01;
      /*$AEAC*/ block_id = s_status_d ? 1794 : 1793;
      break;
    case 1793:  // $AEAC
      /*$AEAC*/ tmp6_U16 = s_a;
                tmp5_U16 = (tmp6_U16 - 0x0008) - (uint8_t)(0x01 - s_status_c);
                s_status_c = (uint8_t)(0x01 - ((uint8_t)(tmp5_U16 >> 8) & 0x01));
                s_status_v = ovf8((uint8_t)tmp5_U16, (uint8_t)tmp6_U16, (uint8_t)0xfff7);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 1795;
      break;
    case 1794:  // $AEAC
      /*$AEAC*/ tmp5_U16 = sbc_dec16(s_a, 0x08, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                tmp1_U8 = (uint8_t)(tmp5_U16 >> 8);
                s_status_c = (tmp1_U8 & 0x01);
                s_status_v = ((tmp1_U8 & 0x40) != 0);
                block_id = 1795;
      break;
    case 1795:  // $AEAE
      /*$AEAE*/ ram_poke(0x000a, s_a);
      /*$AEB0*/ branchTarget = true; block_id = s_status_c ? 1796 : 1797;
      break;
    case 1796:  // $AEB0
      /*$AEB0*/ CYCLES_EDGE(0xaeb0, 1);
      /*$AE80*/ CYCLES(0xae80, 42);
      /*$AE82*/ ram_poke(0x0004, 0x00);
      /*$AE86*/ ram_poke(0x0005, 0x30);
      /*$AE8A*/ ram_poke(0x0006, 0x07);
      /*$AE8E*/ ram_poke(0x0008, 0x05);
      /*$AE92*/ ram_poke(0x0009, 0x22);
      /*$AE96*/ ram_poke(0x000b, 0x00);
      /*$AE9A*/ ram_poke(0xae1c, 0x00);
      /*$AE9D*/ func_ae00(0xae9f);
      /*$AEA0*/ CYCLES(0xaea0, 7);
      /*$AEA4*/ branchTarget = true; block_id = (ram_peek(0x000a) & 0x1f) ? 1790 : 1791;
      break;
    case 1797:  // $AEB2
      /*$AEB2*/ CYCLES(0xaeb2, 11);
                s_status_not_z = 0xf8;
                s_status_n = 0x80;
                s_a = 0xf8;
      /*$AEB4*/ ram_poke(0x000a, 0xf8);
      /*$AEB6*/ branchTarget = true; block_id = find_block_id_func_t001(0xaeb6, pop16() + 1);;
      break;
    case 1798:  // $AEC0
      /*$AEC0*/ CYCLES(0xaec0, 2);
                s_x = 0x54;
                block_id = 1799;
      break;
    case 1799:  // $AEC2
      /*$AEC2*/ CYCLES(0xaec2, 16);
                tmp1_U8 = s_x;
      /*$AEC5*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp1_U8)));
      /*$AECA*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp1_U8)));
      /*$AECC*/ s_y = 0x02;
                block_id = 1800;
      break;
    case 1800:  // $AECE
      /*$AECE*/ CYCLES(0xaece, 19);
                tmp1_U8 = s_y;
                tmp7_U8 = peek((ram_peek16al(0x0000) + tmp1_U8));
      /*$AED0*/ tmp7_U8 = tmp7_U8 ^ 0x80;
                s_a = tmp7_U8;
      /*$AED2*/ poke((ram_peek16al(0x0000) + tmp1_U8), tmp7_U8);
      /*$AED4*/ tmp1_U8 = (uint8_t)(tmp1_U8 + 0x01);
                s_y = tmp1_U8;
      /*$AED7*/ branchTarget = true; block_id = !(tmp1_U8 >= 0x15) ? 1801 : 1802;
      break;
    case 1801:  // $AED7
      /*$AED7*/ CYCLES_EDGE(0xaed7, 1);
                branchTarget = true; block_id = 1800;
      break;
    case 1802:  // $AED9
      /*$AED9*/ CYCLES(0xaed9, 6);
                tmp1_U8 = s_x;
                tmp7_U8 = (uint8_t)(tmp1_U8 + 0x01);
                s_x = tmp7_U8;
      /*$AEDA*/ s_status_not_z = (tmp7_U8 != 0x70);
                tmp7_U8 = tmp7_U8 >= 0x70;
                s_status_c = tmp7_U8;
                s_status_n = ((uint8_t)(tmp1_U8 - 0x6f) & 0x80);
      /*$AEDC*/ branchTarget = true; block_id = !tmp7_U8 ? 1803 : 1804;
      break;
    case 1803:  // $AEDC
      /*$AEDC*/ CYCLES_EDGE(0xaedc, 1);
                branchTarget = true; block_id = 1799;
      break;
    case 1804:  // $AEDE
      /*$AEDE*/ CYCLES(0xaede, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xaede, pop16() + 1);;
      break;
    case 1805:  // $B9A0
      /*$B9A0*/ CYCLES(0xb9a0, 20);
      /*$B9A2*/ ram_poke(0x0000, 0x00);
      /*$B9A6*/ ram_poke(0x0001, 0x40);
      /*$B9AA*/ ram_poke(0x0004, 0x00);
      /*$B9AE*/ ram_poke(0x0005, 0xb5);
                block_id = 1806;
      break;
    case 1806:  // $B9B0
      /*$B9B0*/ CYCLES(0xb9b0, 9);
                s_y = 0x00;
      /*$B9B2*/ tmp7_U8 = peek(ram_peek16al(0x0004));
                s_status_not_z = tmp7_U8;
                s_status_n = (tmp7_U8 & 0x80);
                s_a = tmp7_U8;
      /*$B9B4*/ branchTarget = true; block_id = tmp7_U8 ? 1807 : 1808;
      break;
    case 1807:  // $B9B4
      /*$B9B4*/ CYCLES_EDGE(0xb9b4, 1);
      /*$B9B7*/ CYCLES(0xb9b7, 2);
                branchTarget = true; block_id = s_status_n ? 1809 : 1810;
      break;
    case 1808:  // $B9B6
      /*$B9B6*/ CYCLES(0xb9b6, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xb9b6, pop16() + 1);;
      break;
    case 1809:  // $B9B7
      /*$B9B7*/ CYCLES_EDGE(0xb9b7, 1);
      /*$B9D8*/ CYCLES(0xb9d8, 8);
      /*$B9DA*/ s_x = (s_a & 0x7f);
      /*$B9DB*/ s_y = 0x00;
      /*$B9DD*/ s_a = 0x00;
                block_id = 1824;
      break;
    case 1810:  // $B9B9
      /*$B9B9*/ CYCLES(0xb9b9, 24);
                poke((ram_peek16al(0x0000) + s_y), s_a);
      /*$B9BB*/ s_a = ram_peek(0x0000);
      /*$B9BD*/ s_status_c = 0x00;
      /*$B9BE*/ block_id = s_status_d ? 1812 : 1811;
      break;
    case 1811:  // $B9BE
      /*$B9BE*/ tmp5_U16 = (s_a + 0x0001) + s_status_c;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 1813;
      break;
    case 1812:  // $B9BE
      /*$B9BE*/ tmp5_U16 = adc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_c = ((uint8_t)(tmp5_U16 >> 8) & 0x01);
                block_id = 1813;
      break;
    case 1813:  // $B9C0
      /*$B9C0*/ ram_poke(0x0000, s_a);
      /*$B9C2*/ s_a = ram_peek(0x0001);
      /*$B9C4*/ block_id = s_status_d ? 1815 : 1814;
      break;
    case 1814:  // $B9C4
      /*$B9C4*/ s_a = (uint8_t)(s_a + s_status_c);
                block_id = 1816;
      break;
    case 1815:  // $B9C4
      /*$B9C4*/ s_a = ((uint8_t)adc_dec16(s_a, 0x00, s_status_c));
                block_id = 1816;
      break;
    case 1816:  // $B9C6
      /*$B9C6*/ ram_poke(0x0001, s_a);
                block_id = 1817;
      break;
    case 1817:  // $B9C8
      /*$B9C8*/ CYCLES(0xb9c8, 21);
                s_a = ram_peek(0x0004);
      /*$B9CA*/ s_status_c = 0x00;
      /*$B9CB*/ block_id = s_status_d ? 1819 : 1818;
      break;
    case 1818:  // $B9CB
      /*$B9CB*/ tmp5_U16 = (s_a + 0x0001) + s_status_c;
                s_status_c = (uint8_t)(tmp5_U16 >> 8);
                s_a = ((uint8_t)tmp5_U16);
                block_id = 1820;
      break;
    case 1819:  // $B9CB
      /*$B9CB*/ tmp5_U16 = adc_dec16(s_a, 0x01, s_status_c);
                s_a = ((uint8_t)tmp5_U16);
                s_status_c = ((uint8_t)(tmp5_U16 >> 8) & 0x01);
                block_id = 1820;
      break;
    case 1820:  // $B9CD
      /*$B9CD*/ ram_poke(0x0004, s_a);
      /*$B9CF*/ s_a = ram_peek(0x0005);
      /*$B9D1*/ block_id = s_status_d ? 1822 : 1821;
      break;
    case 1821:  // $B9D1
      /*$B9D1*/ tmp5_U16 = s_a;
                tmp6_U16 = tmp5_U16 + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_status_v = ovf8((uint8_t)tmp6_U16, (uint8_t)tmp5_U16, (uint8_t)0x0000);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1823;
      break;
    case 1822:  // $B9D1
      /*$B9D1*/ tmp6_U16 = adc_dec16(s_a, 0x00, s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                tmp7_U8 = (uint8_t)(tmp6_U16 >> 8);
                s_status_c = (tmp7_U8 & 0x01);
                s_status_v = ((tmp7_U8 & 0x40) != 0);
                block_id = 1823;
      break;
    case 1823:  // $B9D3
      /*$B9D3*/ ram_poke(0x0005, s_a);
      /*$B9D5*/ branchTarget = true; block_id = 1806;
      break;
    case 1824:  // $B9DF
      /*$B9DF*/ CYCLES(0xb9df, 12);
                tmp7_U8 = s_y;
                poke((ram_peek16al(0x0000) + tmp7_U8), s_a);
      /*$B9E1*/ s_y = (uint8_t)(tmp7_U8 + 0x01);
      /*$B9E2*/ tmp7_U8 = (uint8_t)(s_x - 0x01);
                s_x = tmp7_U8;
      /*$B9E3*/ branchTarget = true; block_id = tmp7_U8 ? 1825 : 1826;
      break;
    case 1825:  // $B9E3
      /*$B9E3*/ CYCLES_EDGE(0xb9e3, 1);
                branchTarget = true; block_id = 1824;
      break;
    case 1826:  // $B9E5
      /*$B9E5*/ CYCLES(0xb9e5, 21);
                s_a = s_y;
      /*$B9E6*/ s_status_c = 0x00;
      /*$B9E7*/ block_id = s_status_d ? 1828 : 1827;
      break;
    case 1827:  // $B9E7
      /*$B9E7*/ tmp6_U16 = (s_a + ram_peek(0x0000)) + s_status_c;
                s_status_c = (uint8_t)(tmp6_U16 >> 8);
                s_a = ((uint8_t)tmp6_U16);
                block_id = 1829;
      break;
    case 1828:  // $B9E7
      /*$B9E7*/ tmp6_U16 = adc_dec16(s_a, ram_peek(0x0000), s_status_c);
                s_a = ((uint8_t)tmp6_U16);
                s_status_c = ((uint8_t)(tmp6_U16 >> 8) & 0x01);
                block_id = 1829;
      break;
    case 1829:  // $B9E9
      /*$B9E9*/ ram_poke(0x0000, s_a);
      /*$B9EB*/ s_a = ram_peek(0x0001);
      /*$B9ED*/ block_id = s_status_d ? 1831 : 1830;
      break;
    case 1830:  // $B9ED
      /*$B9ED*/ s_a = (uint8_t)(s_a + s_status_c);
                block_id = 1832;
      break;
    case 1831:  // $B9ED
      /*$B9ED*/ s_a = ((uint8_t)adc_dec16(s_a, 0x00, s_status_c));
                block_id = 1832;
      break;
    case 1832:  // $B9EF
      /*$B9EF*/ ram_poke(0x0001, s_a);
      /*$B9F1*/ branchTarget = true; block_id = 1817;
      break;
    case 1833:  // $BA00
      /*$BA00*/ CYCLES(0xba00, 6);
                func_b9a0(0xba02);
      /*$BA03*/ CYCLES(0xba03, 2);
                s_x = 0x00;
                block_id = 1834;
      break;
    case 1834:  // $BA05
      /*$BA05*/ CYCLES(0xba05, 40);
                tmp7_U8 = s_x;
      /*$BA08*/ ram_poke(0x0001, ram_peek((0x0c00 + tmp7_U8)));
      /*$BA0D*/ ram_poke(0x0000, ram_peek((0x0d00 + tmp7_U8)));
      /*$BA13*/ poke(ram_peek16al(0x0000), 0x02);
      /*$BA19*/ poke((ram_peek16al(0x0000) + 0x0027), 0x40);
      /*$BA1B*/ tmp7_U8 = (uint8_t)(tmp7_U8 + 0x01);
                s_x = tmp7_U8;
      /*$BA1C*/ tmp7_U8 = tmp7_U8 >= 0xa5;
                s_status_c = tmp7_U8;
      /*$BA1E*/ branchTarget = true; block_id = !tmp7_U8 ? 1835 : 1836;
      break;
    case 1835:  // $BA1E
      /*$BA1E*/ CYCLES_EDGE(0xba1e, 1);
                branchTarget = true; block_id = 1834;
      break;
    case 1836:  // $BA20
      /*$BA20*/ CYCLES(0xba20, 7);
      /*$BA22*/ ram_poke(0x0002, 0x55);
      /*$BA24*/ s_y = 0x27;
                block_id = 1837;
      break;
    case 1837:  // $BA26
      /*$BA26*/ CYCLES(0xba26, 21);
      /*$BA28*/ tmp7_U8 = s_y;
                poke((ram_peek16al(0x0000) + tmp7_U8), ram_peek(0x0002));
      /*$BA2E*/ ram_poke(0x0002, (ram_peek(0x0002) ^ 0x7f));
      /*$BA30*/ tmp7_U8 = (uint8_t)(tmp7_U8 - 0x01);
                s_y = tmp7_U8;
      /*$BA31*/ branchTarget = true; block_id = !(tmp7_U8 & 0x80) ? 1838 : 1839;
      break;
    case 1838:  // $BA31
      /*$BA31*/ CYCLES_EDGE(0xba31, 1);
                branchTarget = true; block_id = 1837;
      break;
    case 1839:  // $BA33
      /*$BA33*/ CYCLES(0xba33, 18);
                s_x = 0x00;
      /*$BA38*/ ram_poke(0x0001, ram_peek(0x0c00));
      /*$BA3D*/ ram_poke(0x0000, ram_peek(0x0d00));
      /*$BA3F*/ s_y = 0x27;
                block_id = 1840;
      break;
    case 1840:  // $BA41
      /*$BA41*/ CYCLES(0xba41, 21);
      /*$BA43*/ tmp7_U8 = s_y;
                poke((ram_peek16al(0x0000) + tmp7_U8), ram_peek(0x0002));
      /*$BA47*/ tmp1_U8 = ram_peek(0x0002) ^ 0x7f;
                s_a = tmp1_U8;
      /*$BA49*/ ram_poke(0x0002, tmp1_U8);
      /*$BA4B*/ tmp7_U8 = (uint8_t)(tmp7_U8 - 0x01);
                s_status_not_z = tmp7_U8;
                tmp1_U8 = tmp7_U8 & 0x80;
                s_status_n = tmp1_U8;
                s_y = tmp7_U8;
      /*$BA4C*/ branchTarget = true; block_id = !tmp1_U8 ? 1841 : 1842;
      break;
    case 1841:  // $BA4C
      /*$BA4C*/ CYCLES_EDGE(0xba4c, 1);
                branchTarget = true; block_id = 1840;
      break;
    case 1842:  // $BA4E
      /*$BA4E*/ CYCLES(0xba4e, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xba4e, pop16() + 1);;
      break;
    case 1843:  // $BAD8
      /*$BAD8*/ CYCLES(0xbad8, 2);
                s_x = 0x43;
                block_id = 1844;
      break;
    case 1844:  // $BADA
      /*$BADA*/ CYCLES(0xbada, 33);
                tmp1_U8 = s_x;
      /*$BADD*/ ram_poke(0x0000, ram_peek((0xba50 + tmp1_U8)));
      /*$BAE2*/ ram_poke(0x0001, ram_peek((0xba94 + tmp1_U8)));
      /*$BAE4*/ s_y = 0x00;
      /*$BAE6*/ tmp7_U8 = peek(ram_peek16al(0x0000));
      /*$BAE8*/ tmp7_U8 = tmp7_U8 ^ 0x80;
                s_a = tmp7_U8;
      /*$BAEA*/ poke(ram_peek16al(0x0000), tmp7_U8);
      /*$BAEC*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
                s_status_not_z = tmp1_U8;
                tmp7_U8 = tmp1_U8 & 0x80;
                s_status_n = tmp7_U8;
                s_x = tmp1_U8;
      /*$BAED*/ branchTarget = true; block_id = !tmp7_U8 ? 1845 : 1846;
      break;
    case 1845:  // $BAED
      /*$BAED*/ CYCLES_EDGE(0xbaed, 1);
                branchTarget = true; block_id = 1844;
      break;
    case 1846:  // $BAEF
      /*$BAEF*/ CYCLES(0xbaef, 6);
                branchTarget = true; block_id = find_block_id_func_t001(0xbaef, pop16() + 1);;
      break;
    case 1847:  // $BAF0
      /*$BAF0*/ CYCLES(0xbaf0, 6);
                func_ba00(0xbaf2);
      /*$BAF3*/ CYCLES(0xbaf3, 6);
                func_bad8(0xbaf5);
      /*$BAF6*/ CYCLES(0xbaf6, 11);
      /*$BAF8*/ ram_poke(0x000a, 0xf8);
      /*$BAFA*/ func_ae80(0xbafc);
      /*$BAFD*/ CYCLES(0xbafd, 3);
                func_ae80(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0xbafd, pop16() + 1);;
      break;
    case 1848:  // $FA47
      /*$FA47*/ CYCLES_EDGE(0xfa47, 1);
      /*$FA4C*/ CYCLES(0xfa4c, 10);
                tmp7_U8 = pop8();
                s_status_c = (tmp7_U8 & 0x01);
                s_status_not_z = (~tmp7_U8 & 2);
                s_status_i = ((tmp7_U8 & 0x04) != 0);
                s_status_d = ((tmp7_U8 & 0x08) != 0);
                s_status_b = 0x00;
                s_status_v = ((tmp7_U8 & 0x40) != 0);
                s_status_n = (tmp7_U8 & 0x80);
      /*$FA4D*/ FUNC_SAV1(0xfa4f);
      /*$FA50*/ CYCLES(0xfa50, 19);
                tmp7_U8 = pop8();
      /*$FA51*/ ram_poke(0x003a, tmp7_U8);
      /*$FA53*/ tmp7_U8 = pop8();
                s_status_not_z = tmp7_U8;
                s_status_n = (tmp7_U8 & 0x80);
                s_a = tmp7_U8;
      /*$FA54*/ ram_poke(0x003b, tmp7_U8);
      /*$FA56*/ branchTarget = true; block_id = find_block_id_func_t001(0xfa56, ram_peek16al(0x03f0));
      break;
    case 1849:  // $FA49
      /*$FA49*/ CYCLES(0xfa49, 5);
                branchTarget = true; block_id = find_block_id_func_t001(0xfa49, ram_peek16al(0x03fe));
      break;
    case 1850:  // $FF4C
      /*$FF4C*/ CYCLES(0xff4c, 29);
                ram_poke(0x0046, s_x);
      /*$FF4E*/ ram_poke(0x0047, s_y);
      /*$FF50*/ push8((s_status_c | ((s_status_not_z == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | s_status_n));
      /*$FF51*/ tmp7_U8 = pop8();
                s_a = tmp7_U8;
      /*$FF52*/ ram_poke(0x0048, tmp7_U8);
      /*$FF54*/ tmp7_U8 = s_sp;
                s_status_not_z = tmp7_U8;
                s_status_n = (tmp7_U8 & 0x80);
                s_x = tmp7_U8;
      /*$FF55*/ ram_poke(0x0049, tmp7_U8);
      /*$FF57*/ s_status_d = 0x00;
      /*$FF58*/ branchTarget = true; block_id = find_block_id_func_t001(0xff58, pop16() + 1);;
      break;
    case 1851:
      /*$18E2*/ func_18e4(0x0000);
                branchTarget = true; block_id = find_block_id_func_t001(0x18e2, pop16() + 1);;
      break;
    case 1852:  // $10100
      /*$0E7B*/ push8((s_status_c | ((s_status_not_z == 0) << 1) | (s_status_i << 2) | (s_status_d << 3) | STATUS_B | (s_status_v << 6) | s_status_n));
      /*$FA40*/ CYCLES(0xfa40, 18);
                ram_poke(0x0045, s_a);
      /*$FA42*/ tmp2_U8 = pop8();
      /*$FA43*/ push8(tmp2_U8);
      /*$FA46*/ tmp4_U16 = tmp2_U8 << 0x03;
                tmp2_U8 = (uint8_t)tmp4_U16;
                s_status_c = (uint8_t)((tmp4_U16 & 0x01ff) >> 8);
                s_status_not_z = tmp2_U8;
                tmp3_U8 = tmp2_U8 & 0x80;
                s_status_n = tmp3_U8;
                s_a = tmp2_U8;
      /*$FA47*/ branchTarget = true; block_id = tmp3_U8 ? 1848 : 1849;
      break;
    default:
      fprintf(stderr, "panic: unknown block_id: %u\n", block_id);
      abort();
    }
  }
}

static const unsigned s_block_map_func_t001[] = {
    0x0100,    1, 0x10ae,  207, 0x112f,  229, 0x1132,  230, 0x1179,  256,
    0x1266,  310, 0x12bd,  337, 0x12da,  343, 0x12e7,  344, 0x132f,  358,
    0x13b7,  383, 0x141e,  410, 0x144b,  418, 0x1478,  426, 0x1576,  477,
    0x15a8,  487, 0x1601,  520, 0x165a,  542, 0x176a,  593, 0x17ab,  613,
    0x1a99,  779, 0x1d00,  903, 0x1d2f,  924, 0x1d32,  925, 0x1e32,  962,
    0x1e96,  980, 0x1e99,  981, 0x1eaf,  985, 0x1ebf,  987, 0x1ec2,  988,
    0x1ecb,  989, 0x1ece,  990, 0x1ed9,  993, 0x1ee1,  995, 0x1f0d, 1006,
    0x1f3d, 1007, 0x1f4d, 1008, 0x1f6e, 1012, 0x2000, 1021, 0x2003, 1022,
    0x2006, 1023, 0x2015, 1028, 0x201b, 1029, 0x2021, 1030, 0x202c, 1033,
    0x202f, 1034, 0x2062, 1049, 0x2083, 1050, 0x2086, 1051, 0x209b, 1056,
    0x20a1, 1057, 0x20a7, 1058, 0x20bc, 1063, 0x20c2, 1064, 0x20cd, 1067,
    0x2103, 1070, 0x2106, 1071, 0x2124, 1077, 0x2127, 1078, 0x2153, 1085,
    0x2183, 1093, 0x2186, 1094, 0x2943, 1332, 0x2954, 1336, 0x2b63, 1373,
    0x2b66, 1374, 0x2ce3, 1407, 0x2ceb, 1409, 0x2d0c, 1411, 0x2d11, 1412,
    0x2d16, 1413, 0x2d20, 1414, 0x2d25, 1415, 0x2d2a, 1416, 0x2d4c, 1419,
    0x2d51, 1420, 0x2d56, 1421, 0x2d60, 1422, 0x2d65, 1423, 0x2d6a, 1424,
    0x2dac, 1431, 0x2dc3, 1432, 0x2dc6, 1433, 0x2dc9, 1434, 0x2dcc, 1435,
    0x352b, 1638, 0x3532, 1641, 0x3578, 1645, 0x380c, 1687, 0x3843, 1708
};

static unsigned find_block_id_func_t001(uint16_t from_pc, uint16_t addr) {
  return addr_to_block_id(from_pc, addr, s_block_map_func_t001, sizeof(s_block_map_func_t001) / (sizeof(unsigned) * 2));
};
#include "game-image.inc"
#include "rom-image.inc"

/// The Apple II ROM entry point Neptune calls, written by hand. It has to
/// share this translation unit: system2-inc.h defines the machine with
/// internal linkage, and rom.c uses the generated statics ovf8() and
/// sbc_dec16(). See rom.c for why it keeps the 6502's loop shape.
#include "rom.c"
