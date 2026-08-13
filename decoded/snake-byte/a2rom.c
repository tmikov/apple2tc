/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// Hand-written replacements for the Apple II ROM entry points listed in
/// `rom.externs`. See `a2rom.h` for the per-routine documentation.
///
/// ============================================================================
/// THIS FILE IS NOT A STANDALONE TRANSLATION UNIT. IT MUST BE #include-d.
/// ============================================================================
///
/// The emulated machine state lives in `apple2tc/system2-inc.h`, and almost all
/// of it has *internal linkage*:
///
///   - `s_a`, `s_x`, `s_y`, `s_sp`, `s_status_*`, `s_ram`, `s_pc`, `s_cycles`,
///     `s_remaining_cycles` are all `static`.
///   - `peek`, `poke`, `peek16`, `ram_peek16al`, `push8`, `pop8`, `push16`,
///     `pop16`, `adc_decimal`, `sbc_decimal` are all `static`.
///   - The `CYCLES()` macro expands to references to `s_pc`, `s_cycles`,
///     `s_remaining_cycles` and the *local* variable `branchTarget`.
///
/// Only `ram_peek`, `ram_poke`, `ram_peek16`, `io_peek`, `io_poke` and
/// `error_handler` have external linkage (declared in `apple2tc/system.h`).
///
/// A separate `.c` file therefore cannot see the CPU state at all. In addition,
/// this file calls the still-generated helpers `FUNC_VTABZ`, `FUNC_CLREOLZ`,
/// `FUNC_CLREOL` and `FUNC_MON_WAIT` and uses `ovf8()` / `adc_dec16()`, which
/// the decompiler emits as `static` in the generated file.
///
/// Consequently this file must be textually included into the same translation
/// unit as the generated C, *after* it. That is what `snake-byte-ext.c` does,
/// and `snake-byte-ext.c` is the file CMake compiles for the `snake-bytec1-ext`
/// target:
///
///     #include "snake-bytec1-ext.c"
///     #include "a2rom.c"
///
/// DO NOT add `a2rom.c` to CMake as a source file of its own. Compiling it
/// alone yields a wall of undefined identifiers, because everything listed
/// above is invisible outside the generated file's translation unit. Worse, if
/// it ever did compile separately it would get its *own* copy of `s_ram` and
/// the registers -- `system2-inc.h` declares them `static` -- and link cleanly
/// while reading and writing a machine state nothing else can see.
///
/// This file is used only by the `snake-bytec1-ext` build. The plain
/// `snake-bytec1` target is the self-contained reference build, which
/// decompiles the ROM instead of calling into here.

#include "a2rom.h"
#include "game.h"

/* Helpers that remain in the generated code. Redeclared here so that this file
   reads standalone; C permits identical redeclarations. */
void FUNC_VTABZ(uint16_t ret_addr);
void FUNC_CLREOLZ(uint16_t ret_addr);
void FUNC_CLREOL(uint16_t ret_addr);
void FUNC_MON_WAIT(uint16_t ret_addr);

/* $FDF0 COUT1, defined below. `rom_cout` dispatches to it, and so does the
   game's own $664A handler in game.c once it has drawn its glyph. */
static void rom_cout1(uint16_t ret_addr);

/* ========================================================================== */
/* Private helpers.                                                           */
/*                                                                            */
/* $F847 GBASCALC and $F80E PLOT1 used to be emitted as `FUNC_GBASCALC` and    */
/* `FUNC_PLOT1`. Their only callers were $F800/$F819/$F871, so once those      */
/* became external the two helpers became unreachable and the decompiler       */
/* dropped them. They are reproduced here verbatim from the pre-externs        */
/* output.                                                                    */
/* ========================================================================== */

/// $F847 GBASCALC. Compute the lo-res base address for row A into GBASL/GBASH
/// ($26/$27).
static void rom_gbascalc(uint16_t ret_addr) {
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
            if (!tmp2_U8)
              goto bb_6;
bb_1:
  /*$F854*/ CYCLES(0xf854, 2);
            if (s_status_d)
              goto bb_3;
bb_2:
  /*$F854*/ s_a = (uint8_t)((s_a + 0x007f) + s_status_c);
            goto bb_4;
bb_3:
  /*$F854*/ s_a = ((uint8_t)adc_dec16(s_a, 0x7f, s_status_c));
bb_4:
bb_5:
  /*$F856*/ CYCLES(0xf856, 19);
            tmp2_U8 = s_a;
            ram_poke(0x0026, tmp2_U8);
  /*$F85C*/ ram_poke(0x0026, ((uint8_t)(tmp2_U8 << 0x02) | ram_peek(0x0026)));
  /*$F85E*/ if (ret_addr) pop16(); return;
bb_6:
  // $F852 BCC -- the branch itself, taken here (not modelled by bb_1's own
  // cost, which is the not-taken total; see the design doc on edge costs).
  /*$F852*/ CYCLES(0xf852, 1);
            goto bb_5;
}

/// $F80E PLOT1. Store the color mask ($30) into the lo-res half-byte selected
/// by MASK ($2E) at GBASL/GBASH ($26) + Y.
static void rom_plot1(uint16_t ret_addr) {
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

/* ========================================================================== */
/* $F800 PLOT                                                                 */
/* ========================================================================== */

void rom_plot(uint16_t ret_addr) {
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
  /*$F802*/ rom_gbascalc(0xfffe);
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
            if (!tmp2_U8)
              goto bb_6;
bb_1:
  /*$F80A*/ CYCLES(0xf80a, 2);
            if (s_status_d)
              goto bb_3;
bb_2:
  /*$F80A*/ tmp3_U16 = s_a;
            tmp4_U16 = (tmp3_U16 + 0x00e0) + s_status_c;
            s_status_c = (uint8_t)(tmp4_U16 >> 8);
            s_status_v = ovf8((uint8_t)tmp4_U16, (uint8_t)tmp3_U16, (uint8_t)0x00e0);
            s_a = ((uint8_t)tmp4_U16);
            goto bb_4;
bb_3:
  /*$F80A*/ tmp4_U16 = adc_dec16(s_a, 0xe0, s_status_c);
            s_a = ((uint8_t)tmp4_U16);
            tmp2_U8 = (uint8_t)(tmp4_U16 >> 8);
            s_status_c = (tmp2_U8 & 0x01);
            s_status_v = ((tmp2_U8 & 0x40) != 0);
bb_4:
bb_5:
  /*$F80C*/ CYCLES(0xf80c, 3);
            ram_poke(0x002e, s_a);
            rom_plot1(0x0000);
            if (ret_addr) pop16(); return;
bb_6:
  // $F808 BCC -- the branch itself, taken here.
  /*$F808*/ CYCLES(0xf808, 1);
            goto bb_5;
}

/* ========================================================================== */
/* $F819 HLINE                                                                */
/* ========================================================================== */

void rom_hline(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint16_t tmp2_U16;
  uint16_t tmp3_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$F819*/ CYCLES(0xf819, 6);
            rom_plot(0xfffe);
            branchTarget = true;
bb_1:
  /*$F81C*/ CYCLES(0xf81c, 5);
            tmp1_U8 = s_y >= ram_peek(0x002c);
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (tmp1_U8)
              goto bb_11;
bb_2:
  /*$F820*/ CYCLES(0xf820, 8);
            s_y = (uint8_t)(s_y + 0x01);
  /*$F821*/ rom_plot1(0xfffe);
  /*$F824*/ CYCLES(0xf824, 2);
            branchTarget = true;
            if (!s_status_c)
              goto bb_12;
bb_3:
  /*$F826*/ CYCLES(0xf826, 2);
            if (s_status_d)
              goto bb_5;
bb_4:
  /*$F826*/ tmp2_U16 = s_a;
            tmp3_U16 = (tmp2_U16 + 0x0001) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp2_U16, (uint8_t)0x0001);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_6;
bb_5:
  /*$F826*/ tmp3_U16 = adc_dec16(s_a, 0x01, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_6:
  /*$F828*/ CYCLES(0xf828, 9);
            push8(s_a);
  /*$F829*/ rom_plot(0xfffe);
  /*$F82C*/ CYCLES(0xf82c, 9);
            tmp1_U8 = pop8();
            s_a = tmp1_U8;
  /*$F82D*/ tmp1_U8 = tmp1_U8 >= ram_peek(0x002d);
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_13;
bb_7:
  /*$F831*/ CYCLES(0xf831, 6);
            if (ret_addr) pop16(); return;
bb_11:
  // $F81E BCS -- the branch itself, taken here.
  /*$F81E*/ CYCLES(0xf81e, 1);
            goto bb_7;
bb_12:
  // $F824 BCC -- the branch itself, taken here.
  /*$F824*/ CYCLES(0xf824, 1);
            goto bb_1;
bb_13:
  // $F82F BCC -- the branch itself, taken here.
  /*$F82F*/ CYCLES(0xf82f, 1);
            goto bb_3;
}

/* ========================================================================== */
/* $F864 SETCOL                                                               */
/* ========================================================================== */

void rom_setcol(uint16_t ret_addr) {
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

/* ========================================================================== */
/* $F871 SCRN                                                                 */
/* ========================================================================== */

void rom_scrn(uint16_t ret_addr) {
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
  /*$F873*/ rom_gbascalc(0xfffe);
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
            if (!tmp2_U8)
              goto bb_3;
bb_1:
  /*$F87B*/ CYCLES(0xf87b, 8);
            tmp2_U8 = s_a;
  /*$F87E*/ s_status_c = ((tmp2_U8 >> 0x03) & 0x01);
            s_a = (tmp2_U8 >> 0x04);
bb_2:
  /*$F87F*/ CYCLES(0xf87f, 8);
            tmp2_U8 = s_a & 0x0f;
            s_status_not_z = tmp2_U8;
            s_status_n = 0x00;
            s_a = tmp2_U8;
  /*$F881*/ if (ret_addr) pop16(); return;
bb_3:
  // $F879 BCC -- the branch itself, taken here.
  /*$F879*/ CYCLES(0xf879, 1);
            goto bb_2;
}

/* ========================================================================== */
/* $FC58 HOME                                                                 */
/* ========================================================================== */

void rom_home(uint16_t ret_addr) {
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
            branchTarget = true;
            // $FC60 BEQ -- provably always taken (Y was just loaded 0), but
            // the branch instruction still executes and still pays its own
            // cost every time. The decompiler doesn't do cross-instruction
            // flag proofs either, so it keeps charging this the same way.
  /*$FC60*/ CYCLES(0xfc60, 1);
            goto bb_2;
bb_1:
  /*$FC22*/ CYCLES(0xfc22, 3);
            s_a = ram_peek(0x0025);
            FUNC_VTABZ(0x0000);
            if (ret_addr) pop16(); return;
bb_2:
  /*$FC46*/ CYCLES(0xfc46, 9);
            push8(s_a);
  /*$FC47*/ FUNC_VTABZ(0xfffe);
  /*$FC4A*/ CYCLES(0xfc4a, 6);
            FUNC_CLREOLZ(0xfffe);
  /*$FC4D*/ CYCLES(0xfc4d, 13);
            s_y = 0x00;
  /*$FC4F*/ tmp1_U8 = pop8();
            s_a = tmp1_U8;
            if (s_status_d)
              goto bb_4;
bb_3:
  /*$FC50*/ s_a = (uint8_t)(s_a + s_status_c);
            goto bb_5;
bb_4:
  /*$FC50*/ s_a = ((uint8_t)adc_dec16(s_a, 0x00, s_status_c));
bb_5:
  /*$FC52*/ tmp1_U8 = s_a >= ram_peek(0x0023);
            s_status_c = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_7;
bb_6:
  /*$FC56*/ CYCLES(0xfc56, 2);
            branchTarget = true;
            if (s_status_c)
              goto bb_8;
            else
              goto bb_0;
bb_7:
  // $FC54 BCC -- the branch itself, taken here.
  /*$FC54*/ CYCLES(0xfc54, 1);
            goto bb_2;
bb_8:
  // $FC56 BCS -- the branch itself, same address as bb_6's own anchor
  // because this is a singleton one-instruction block.
  /*$FC56*/ CYCLES(0xfc56, 1);
            goto bb_1;
}

/* ========================================================================== */
/* $FC68 -- the tail of LF: VTAB, or scroll if CV has run off the window.      */
/*                                                                            */
/*   FC68: LDA CV / CMP WNDBTM / BCC VTABZ    ; usual case: just recompute BAS */
/*   FC6E: DEC CV / LDA WNDTOP / PHA / ...    ; otherwise scroll up one line   */
/*                                                                            */
/* Transcribed from the pre-externs mega-switch blocks $FC68..$FC95 plus the   */
/* three exit blocks ($FC6C -> VTABZ, $FC22, $FC9A -> CLREOL).                */
/*                                                                            */
/* Besides COUT1 falling through $FC66, the game calls this directly as a      */
/* VTAB: $7590 and $75D1 store CH/CV and JSR here. Their CV values are 5..13,  */
/* well under WNDBTM ($18), so they always take the BCC and never scroll.      */
/* ========================================================================== */

void rom_fc68(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint16_t tmp3_U16;
  uint16_t tmp4_U16;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FC68*/ CYCLES(0xfc68, 8);
            tmp1_U8 = ram_peek(0x0025);
            s_a = tmp1_U8;
  /*$FC6C*/ branchTarget = true;
            if (!(tmp1_U8 >= ram_peek(0x0023)))
              goto bb_10;
bb_1:
  /*$FC6E*/ CYCLES(0xfc6e, 17);
            ram_poke(0x0025, (uint8_t)(ram_peek(0x0025) - 0x01));
  /*$FC70*/ tmp1_U8 = ram_peek(0x0022);
            s_a = tmp1_U8;
  /*$FC72*/ push8(tmp1_U8);
  /*$FC73*/ FUNC_VTABZ(0xfffe);
            branchTarget = true;
bb_2:
  /*$FC76*/ CYCLES(0xfc76, 28);
  /*$FC78*/ ram_poke(0x002a, ram_peek(0x0028));
  /*$FC7C*/ ram_poke(0x002b, ram_peek(0x0029));
  /*$FC80*/ s_y = (uint8_t)(ram_peek(0x0021) - 0x01);
  /*$FC81*/ tmp1_U8 = pop8();
            s_a = tmp1_U8;
  /*$FC82*/ if (s_status_d)
              goto bb_4;
bb_3:
  /*$FC82*/ tmp4_U16 = s_a;
            tmp3_U16 = (tmp4_U16 + 0x0001) + s_status_c;
            s_status_v = ovf8((uint8_t)tmp3_U16, (uint8_t)tmp4_U16, (uint8_t)0x0001);
            s_a = ((uint8_t)tmp3_U16);
            goto bb_5;
bb_4:
  /*$FC82*/ tmp3_U16 = adc_dec16(s_a, 0x01, s_status_c);
            s_a = ((uint8_t)tmp3_U16);
            s_status_v = (((uint8_t)(tmp3_U16 >> 8) & 0x40) != 0);
bb_5:
  /*$FC86*/ branchTarget = true;
            if (s_a >= ram_peek(0x0023))
              goto bb_13;
bb_6:
  /*$FC88*/ CYCLES(0xfc88, 9);
            push8(s_a);
  /*$FC89*/ FUNC_VTABZ(0xfffe);
            branchTarget = true;
bb_7:
  /*$FC8C*/ CYCLES(0xfc8c, 15);
            tmp1_U8 = s_y;
            tmp2_U8 = peek((ram_peek16al(0x0028) + tmp1_U8));
  /*$FC8E*/ poke((ram_peek16al(0x002a) + tmp1_U8), tmp2_U8);
  /*$FC90*/ tmp1_U8 = (uint8_t)(tmp1_U8 - 0x01);
            tmp2_U8 = tmp1_U8 & 0x80;
            s_status_n = tmp2_U8;
            s_y = tmp1_U8;
  /*$FC91*/ branchTarget = true;
            if (!tmp2_U8)
              goto bb_14;
bb_8:
  /*$FC93*/ CYCLES(0xfc93, 2);
            branchTarget = true;
            if (s_status_n)
              goto bb_15;
bb_9:
  /*$FC95*/ CYCLES(0xfc95, 8);
            s_y = 0x00;
  /*$FC97*/ FUNC_CLREOLZ(0xfffe);
  /*$FC9A*/ CYCLES(0xfc9a, 2);
            branchTarget = true;
            if (!s_status_c)
              goto bb_11;
            // $FC9A BCS -- taken here (falls into the trampoline charge
            // below before continuing into bb_12; the not-taken arm above
            // jumps straight to bb_11 without it).
  /*$FC9A*/ CYCLES(0xfc9a, 1);
bb_12:
  /*$FC22*/ CYCLES(0xfc22, 3);
            s_a = ram_peek(0x0025);
            FUNC_VTABZ(0x0000);
            if (ret_addr) pop16(); return;
bb_10:
  // $FC6C BCC -- the branch itself, taken here.
  /*$FC6C*/ CYCLES(0xfc6c, 1);
            FUNC_VTABZ(0x0000);
            branchTarget = true;
            if (ret_addr) pop16(); return;
bb_11:
  /*$FC9A*/ FUNC_CLREOL(0x0000);
            branchTarget = true;
            if (ret_addr) pop16(); return;
bb_13:
  // $FC86 BCS -- the branch itself, taken here.
  /*$FC86*/ CYCLES(0xfc86, 1);
            goto bb_9;
bb_14:
  // $FC91 BPL -- the branch itself, taken here (loop back).
  /*$FC91*/ CYCLES(0xfc91, 1);
            goto bb_7;
bb_15:
  // $FC93 BNE -- the branch itself, taken here (outer loop back).
  /*$FC93*/ CYCLES(0xfc93, 1);
            goto bb_2;
}

/* ========================================================================== */
/* $FB78 COUTZ - the body of COUT1, reached through $FB94 -> $FBFD.           */
/*                                                                            */
/* Transcribed from the pre-externs mega-switch blocks $FB78..$FB94,          */
/* $FBD9..$FBEF, $FBF0..$FBFC, $FC01..$FC2B and $FC62..$FC66. The whole        */
/* subtree disappeared from the generated C once $FDED became external,        */
/* because $FDED was its only entry.                                          */
/*                                                                            */
/* Path coverage, measured by instrumenting this file and replaying the        */
/* 1300-frame play.keys session:                                              */
/*                                                                            */
/*   $FBF0 store char      402 hits    $FC62 CR            3 hits             */
/*   $FC68 scroll           39 hits    $FC66 LF            3 hits             */
/*   $FBD9 CMP #$87          1 hit     $FBDD bell body     1 hit              */
/*   $FBE4 speaker click   192 hits    $FB7C CR kbd poll   3 hits             */
/*   $FB85/$FB88 Ctrl-S      0 hits    $FC10 backspace     0 hits             */
/*                                                                            */
/* So the bell is live: ROM RESET reaches it via $FF3A (LDA #$87; JMP $FDED),  */
/* and the 192 clicks are the $FBE2 `LDY #$C0` loop. It is oracle-verified.    */
/*                                                                            */
/* Backspace ($FC10) is unreachable from Snake Byte, not merely untraced: no   */
/* byte the game emits is $88. The $08 inside the "CRASH" string at $7868 is   */
/* the trap for the eye - it has bit 7 clear, so $FC01 (TAY; BPL) routes it to */
/* $FBF0 as an inverse glyph long before the CMP #$88 at $FC0C.                */
/*                                                                            */
/* The Ctrl-S handshake ($FB85/$FB88) is reachable but untraced: $FB7C does    */
/* run (A == $8D three times), it just never saw a pending $93. It fires only  */
/* if the user holds Ctrl-S while a CR is being output. Transcribed but not    */
/* covered by the oracle - the one part of this file in that category.         */
/* ========================================================================== */

static void rom_coutz(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;
  uint8_t tmp2_U8;
  uint8_t tmp3_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FB78*/ CYCLES(0xfb78, 4);
  /*$FB7A*/ branchTarget = true;
            if (s_a != 0x8d)
              goto bb_27;
bb_1:
  /*$FB7C*/ CYCLES(0xfb7c, 6);
            tmp1_U8 = io_peek(0xc000);
            s_y = tmp1_U8;
  /*$FB7F*/ branchTarget = true;
            if (!(tmp1_U8 & 0x80))
              goto bb_28;
bb_2:
  /*$FB81*/ CYCLES(0xfb81, 4);
  /*$FB83*/ branchTarget = true;
            if (s_y != 0x93)
              goto bb_29;
bb_3:
  /*$FB85*/ CYCLES(0xfb85, 4);
            tmp1_U8 = io_peek(0xc010);
            s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
bb_4:
  /*$FB88*/ CYCLES(0xfb88, 6);
            tmp1_U8 = io_peek(0xc000);
            s_y = tmp1_U8;
  /*$FB8B*/ branchTarget = true;
            if (!(tmp1_U8 & 0x80))
              goto bb_30;
bb_5:
  /*$FB8D*/ CYCLES(0xfb8d, 4);
  /*$FB8F*/ branchTarget = true;
            if (!(s_y != 0x83))
              goto bb_31;
bb_6:
  /*$FB91*/ CYCLES(0xfb91, 4);
            tmp1_U8 = io_peek(0xc010);
            s_status_v = ((tmp1_U8 >> 0x06) & 0x01);
bb_7:
  /*$FB94*/ CYCLES(0xfb94, 3);
  /*$FBFD*/ CYCLES(0xfbfd, 4);
  /*$FBFF*/ branchTarget = true;
            if (!(s_a >= 0xa0))
              goto bb_10;
            // $FBFF BCS -- taken here (falls into the trampoline charge
            // below before continuing into bb_8; the not-taken arm above
            // jumps straight to bb_10 without it).
  /*$FBFF*/ CYCLES(0xfbff, 1);
bb_8:
  /*$FBF0*/ CYCLES(0xfbf0, 9);
            tmp1_U8 = ram_peek(0x0024);
            s_y = tmp1_U8;
  /*$FBF2*/ poke((ram_peek16al(0x0028) + tmp1_U8), s_a);
bb_9:
  /*$FBF4*/ CYCLES(0xfbf4, 13);
            ram_poke(0x0024, (uint8_t)(ram_peek(0x0024) + 0x01));
  /*$FBF6*/ tmp1_U8 = ram_peek(0x0024);
            s_a = tmp1_U8;
  /*$FBF8*/ tmp2_U8 = ram_peek(0x0021);
            s_status_not_z = (tmp1_U8 != tmp2_U8);
            tmp3_U8 = tmp1_U8 >= tmp2_U8;
            s_status_c = tmp3_U8;
            s_status_n = ((uint8_t)(tmp1_U8 - tmp2_U8) & 0x80);
  /*$FBFA*/ branchTarget = true;
            if (tmp3_U8)
              goto bb_33;
bb_19:
  /*$FBFC*/ CYCLES(0xfbfc, 6);
            branchTarget = true;
            if (ret_addr) pop16(); return;
bb_10:
  /*$FC01*/ CYCLES(0xfc01, 4);
            tmp3_U8 = s_a;
            s_y = tmp3_U8;
  /*$FC02*/ branchTarget = true;
            if (!(tmp3_U8 & 0x80))
              goto bb_34;
bb_11:
  /*$FC04*/ CYCLES(0xfc04, 4);
  /*$FC06*/ branchTarget = true;
            if (!(s_a != 0x8d))
              goto bb_35;
bb_12:
  /*$FC08*/ CYCLES(0xfc08, 4);
  /*$FC0A*/ branchTarget = true;
            if (!(s_a != 0x8a))
              goto bb_36;
bb_13:
  /*$FC0C*/ CYCLES(0xfc0c, 4);
            tmp3_U8 = s_a;
            s_status_c = (tmp3_U8 >= 0x88);
  /*$FC0E*/ branchTarget = true;
            if (tmp3_U8 != 0x88)
              goto bb_37;
bb_14:
  /*$FC10*/ CYCLES(0xfc10, 7);
            tmp3_U8 = (uint8_t)(ram_peek(0x0024) - 0x01);
            s_status_not_z = tmp3_U8;
            tmp2_U8 = tmp3_U8 & 0x80;
            s_status_n = tmp2_U8;
            ram_poke(0x0024, tmp3_U8);
  /*$FC12*/ branchTarget = true;
            if (!tmp2_U8)
              goto bb_38;
bb_15:
  /*$FC14*/ CYCLES(0xfc14, 11);
  /*$FC16*/ ram_poke(0x0024, ram_peek(0x0021));
  /*$FC18*/ ram_poke(0x0024, (uint8_t)(ram_peek(0x0024) - 0x01));
bb_16:
  /*$FC1A*/ CYCLES(0xfc1a, 8);
            tmp2_U8 = ram_peek(0x0022);
            s_a = tmp2_U8;
  /*$FC1C*/ tmp3_U8 = ram_peek(0x0025);
            s_status_not_z = (tmp2_U8 != tmp3_U8);
            tmp1_U8 = tmp2_U8 >= tmp3_U8;
            s_status_c = tmp1_U8;
            s_status_n = ((uint8_t)(tmp2_U8 - tmp3_U8) & 0x80);
  /*$FC1E*/ branchTarget = true;
            if (tmp1_U8)
              goto bb_39;
bb_17:
  /*$FC20*/ CYCLES(0xfc20, 5);
            ram_poke(0x0025, (uint8_t)(ram_peek(0x0025) - 0x01));
  /*$FC22*/ CYCLES(0xfc22, 3);
            s_a = ram_peek(0x0025);
            FUNC_VTABZ(0x0000);
            if (ret_addr) pop16(); return;
bb_18:
  /*$FC2B*/ CYCLES(0xfc2b, 6);
            branchTarget = true;
            if (ret_addr) pop16(); return;
bb_22:
  /*$FBD9*/ CYCLES(0xfbd9, 4);
            tmp2_U8 = s_a;
            tmp1_U8 = tmp2_U8 != 0x87;
            s_status_not_z = tmp1_U8;
            s_status_c = (tmp2_U8 >= 0x87);
            s_status_n = ((uint8_t)(tmp2_U8 - 0x87) & 0x80);
  /*$FBDB*/ branchTarget = true;
            if (tmp1_U8)
              goto bb_40;
bb_23:
  /*$FBDD*/ CYCLES(0xfbdd, 8);
            s_a = 0x40;
  /*$FBDF*/ FUNC_MON_WAIT(0xfffe);
            branchTarget = true;
bb_24:
  /*$FBE2*/ CYCLES(0xfbe2, 2);
            s_y = 0xc0;
bb_25:
  /*$FBE4*/ CYCLES(0xfbe4, 8);
            s_a = 0x0c;
  /*$FBE6*/ FUNC_MON_WAIT(0xfffe);
            branchTarget = true;
  /*$FBE9*/ CYCLES(0xfbe9, 8);
            tmp1_U8 = io_peek(0xc030);
            s_a = tmp1_U8;
  /*$FBEC*/ tmp1_U8 = (uint8_t)(s_y - 0x01);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_y = tmp1_U8;
  /*$FBED*/ branchTarget = true;
            if (tmp1_U8)
              goto bb_41;
bb_26:
  /*$FBEF*/ CYCLES(0xfbef, 6);
            branchTarget = true;
            if (ret_addr) pop16(); return;
bb_20:
  /*$FC62*/ CYCLES(0xfc62, 5);
  /*$FC64*/ ram_poke(0x0024, 0x00);
bb_21:
  /*$FC66*/ CYCLES(0xfc66, 5);
            ram_poke(0x0025, (uint8_t)(ram_peek(0x0025) + 0x01));
  /*$FC68*/ rom_fc68(0x0000);
            if (ret_addr) pop16(); return;

  /* Taken-branch trampolines: each pays the +1 a taken branch costs beyond
     the not-taken base already charged above, then jumps on. See the design
     doc; this mirrors what the decompiler does at every JCond. */
bb_27:
  /*$FB7A*/ CYCLES(0xfb7a, 1);
            goto bb_7;
bb_28:
  /*$FB7F*/ CYCLES(0xfb7f, 1);
            goto bb_7;
bb_29:
  /*$FB83*/ CYCLES(0xfb83, 1);
            goto bb_7;
bb_30:
  /*$FB8B*/ CYCLES(0xfb8b, 1);
            goto bb_4;
bb_31:
  /*$FB8F*/ CYCLES(0xfb8f, 1);
            goto bb_7;
bb_33:
  /*$FBFA*/ CYCLES(0xfbfa, 1);
            goto bb_20;
bb_34:
  /*$FC02*/ CYCLES(0xfc02, 1);
            goto bb_8;
bb_35:
  /*$FC06*/ CYCLES(0xfc06, 1);
            goto bb_20;
bb_36:
  /*$FC0A*/ CYCLES(0xfc0a, 1);
            goto bb_21;
bb_37:
  /*$FC0E*/ CYCLES(0xfc0e, 1);
            goto bb_22;
bb_38:
  /*$FC12*/ CYCLES(0xfc12, 1);
            goto bb_19;
bb_39:
  /*$FC1E*/ CYCLES(0xfc1e, 1);
            goto bb_18;
bb_40:
  /*$FBDB*/ CYCLES(0xfbdb, 1);
            goto bb_26;
bb_41:
  /*$FBED*/ CYCLES(0xfbed, 1);
            goto bb_25;
}

/* ========================================================================== */
/* $FDED COUT                                                                 */
/* ========================================================================== */

/// $FDED COUT is a single instruction: `JMP ($36)`. It dispatches through the
/// output vector CSWL/CSWH, so the *behaviour* of COUT is whatever the vector
/// points at, and it must not be short-circuited to ROM COUT1.
///
/// Snake Byte can hook the vector: $6641 does
///     LDA #$4A / STA $36 / LDA #$66 / STA $37 / RTS
/// pointing it at $664A, the game's own hi-res text renderer. ($6641 is called
/// from $7485 and $793F.) $664A maps the text cursor (BASL/BASH + CH) to a
/// hi-res address by adding $1C to the high byte ($04xx -> $20xx) and draws an
/// 8-byte glyph from the font at $66A9; control characters below $20 fall
/// through to ROM COUT1 at $FDF0.
///
/// In the entire recorded session, however, the vector always points at ROM:
/// the run data records $FDED -> $FDF0 only, and $6641 was never executed
/// (neither it nor its callers $7485/$793F appear in BranchTargets). $664A is
/// therefore classified as data and is not present in the generated C at all.
///
/// Because the recorded session never dispatches anywhere but $FDF0, the
/// verification oracle structurally *cannot* detect a wrong choice here. So a
/// non-ROM vector target is a hard, loud failure rather than a silent fallback
/// to COUT1: a wrong-but-quiet answer would diverge invisibly, whereas an abort
/// fires at exactly the moment the question first matters.
///
/// KNOWN GAP for a later phase: implement $664A (the hi-res text renderer) and
/// dispatch to it here.
/// $FDED COUT -- `JMP ($36)`. Dispatches through the output vector CSWL/CSWH
/// rather than reimplementing COUT1, because Snake Byte repoints it: $6641
/// installs the game's own hi-res text renderer at $664A.
///
/// Any target other than the two we implement is a hard failure rather than a
/// fallback. The recorded session never leaves $FDF0, so `verify.sh` cannot
/// catch a wrong guess here -- a silent fallback would render with the wrong
/// font onto the wrong page, undetectably.
void rom_cout(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t vector;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$FDED*/ CYCLES(0xfded, 5);
            vector = ram_peek16al(0x0036); // JMP ($36)
            branchTarget = true;
            switch (vector) {
            case 0xfdf0:
              rom_cout1(0xfffe);
              break;
            case 0x664a:
              game_cout_hook(0xfffe);
              break;
            default:
              fprintf(
                  stderr,
                  "rom_cout: output vector CSWL/CSWH ($36/$37) points at $%04X, "
                  "which is not implemented.\n"
                  "  Known targets are $FDF0 (ROM COUT1) and $664A (the game's "
                  "hi-res text renderer).\n",
                  vector);
              error_handler(0xfded);
              abort();
            }

            if (ret_addr) pop16();
            return;
}

/// $FDF0 COUT1 -- the ROM's own character output: mask to the current text
/// mode, then COUTZ for the actual placement and cursor bookkeeping.
static void rom_cout1(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_1:
  /*$FDF0*/ CYCLES(0xfdf0, 4);
            tmp1_U8 = s_a >= 0xa0;
            s_status_c = tmp1_U8;
  /*$FDF2*/ branchTarget = true;
            if (!tmp1_U8)
              goto bb_5;
bb_2:
  /*$FDF4*/ CYCLES(0xfdf4, 3);
            s_a = (s_a & ram_peek(0x0032));
bb_3:
  /*$FDF6*/ CYCLES(0xfdf6, 12);
            ram_poke(0x0035, s_y);
  /*$FDF8*/ push8(s_a);
  /*$FDF9*/ branchTarget = true;
            rom_coutz(0xfdfb); // JSR $FB78
bb_4:
  /*$FDFC*/ CYCLES(0xfdfc, 13);
            tmp1_U8 = pop8();
            s_a = tmp1_U8;
  /*$FDFD*/ tmp1_U8 = ram_peek(0x0035);
            s_status_not_z = tmp1_U8;
            s_status_n = (tmp1_U8 & 0x80);
            s_y = tmp1_U8;
  /*$FDFF*/ branchTarget = true;
            if (ret_addr) pop16(); return;
bb_5:
  // $FDF2 BCS -- the branch itself, taken here.
  /*$FDF2*/ CYCLES(0xfdf2, 1);
            goto bb_3;
}

/* ========================================================================== */
/* $FE89 SETKBD / $FE93 SETVID                                                */
/*                                                                            */
/* Both fall into the shared tail at $FE9B (SETPWRC/"install IO hooks"), which */
/* is duplicated into each function exactly as the decompiler emitted it.      */
/* ========================================================================== */

void rom_setkbd(uint16_t ret_addr) {
  bool branchTarget = true;
  uint8_t tmp1_U8;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

bb_0:
  /*$FE89*/ CYCLES(0xfe89, 11);
  /*$FE8B*/ ram_poke(0x003e, 0x00);
  /*$FE8D*/ s_x = 0x38;
  /*$FE8F*/ s_y = 0x1b;
            // $FE91 BNE -- provably always taken (Y was just loaded #$1B,
            // nonzero), same reasoning as $FC60 in rom_home: the decompiler
            // doesn't do cross-instruction flag proofs, so the branch still
            // executes and still pays its own cost every time.
  /*$FE91*/ CYCLES(0xfe91, 1);
  /*$FE9B*/ CYCLES(0xfe9b, 7);
  /*$FE9D*/ tmp1_U8 = ram_peek(0x003e) & 0x0f;
            s_a = tmp1_U8;
            branchTarget = true;
            if (!tmp1_U8)
              goto bb_4;
bb_1:
  /*$FEA1*/ CYCLES(0xfea1, 6);
            s_a = (s_a | 0xc0);
  /*$FEA3*/ s_y = 0x00;
            branchTarget = true;
            // $FEA5 BEQ -- provably always taken (Y was just loaded 0).
  /*$FEA5*/ CYCLES(0xfea5, 1);
            goto bb_3;
bb_2:
  /*$FEA7*/ CYCLES(0xfea7, 2);
            s_a = 0xfd;
bb_3:
  /*$FEA9*/ CYCLES(0xfea9, 14);
            tmp1_U8 = s_x;
            ram_poke(tmp1_U8, s_y);
  /*$FEAB*/ ram_poke((uint8_t)(0x01 + tmp1_U8), s_a);
  /*$FEAD*/ if (ret_addr) pop16(); return;
bb_4:
  // $FE9F BEQ -- the branch itself, taken here.
  /*$FE9F*/ CYCLES(0xfe9f, 1);
            goto bb_2;
}

void rom_setvid(uint16_t ret_addr) {
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
            if (!tmp1_U8)
              goto bb_4;
bb_1:
  /*$FEA1*/ CYCLES(0xfea1, 6);
            s_a = (s_a | 0xc0);
  /*$FEA3*/ s_y = 0x00;
            branchTarget = true;
            // $FEA5 BEQ -- provably always taken (Y was just loaded 0).
  /*$FEA5*/ CYCLES(0xfea5, 1);
            goto bb_3;
bb_2:
  /*$FEA7*/ CYCLES(0xfea7, 2);
            s_a = 0xfd;
bb_3:
  /*$FEA9*/ CYCLES(0xfea9, 14);
            tmp1_U8 = s_x;
            ram_poke(tmp1_U8, s_y);
  /*$FEAB*/ ram_poke((uint8_t)(0x01 + tmp1_U8), s_a);
  /*$FEAD*/ if (ret_addr) pop16(); return;
bb_4:
  // $FE9F BEQ -- the branch itself, taken here.
  /*$FE9F*/ CYCLES(0xfe9f, 1);
            goto bb_2;
}
