/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// Snake Byte routines decompiled by hand.
///
/// Like a2rom.c, this file is textually included into the generated translation
/// unit -- it uses the machine state that `apple2tc/system2-inc.h` defines with
/// internal linkage, plus generated helpers. It must come *after* a2rom.c,
/// since it calls the static `rom_cout1`. See snake-byte-ext.c.
///
/// Why hand-decompiled
/// -------------------
/// apple2tc drives disassembly from a recorded run. A routine only reached
/// through an indirect jump that the recording never took is invisible to it:
/// the bytes get classified as data and never appear in the output. That is the
/// case here, so these routines are decoded from the binary directly.

#include "game.h"

/* ========================================================================== */
/* $664A -- the game's own COUT handler.                                      */
/*                                                                            */
/* $6641 installs it:                                                         */
/*     LDA #$4A / STA $36 / LDA #$66 / STA $37 / RTS                          */
/* pointing CSWL/CSWH at $664A, so every subsequent COUT ($FDED = JMP ($36))   */
/* lands here. $6641 is called from $7485 and $793F.                          */
/*                                                                            */
/* The recorded session never runs $6641 -- neither it nor its callers appear  */
/* in snake-byte.json's BranchTargets, and the run-data records $FDED -> $FDF0 */
/* only. So the tracer saw $664A as data and apple2tc emitted nothing for it.  */
/*                                                                            */
/* Decoded from snake-byte.b33 by hand. The decode was cross-checked against   */
/* apple2tc using a scratch run-data file with $664A added as a branch target; */
/* the committed snake-byte.json is deliberately unmodified, because it is a   */
/* recording of what actually happened and not a place to assert reachability. */
/* The CYCLES() constants below come from that cross-check.                    */
/*                                                                            */
/* Original:                                                                  */
/*                                                                            */
/*   664A: PHA / AND #$7F / CMP #$20 / BCS $6655                              */
/*   6651: PLA / JMP $FDF0                 ; control chars -> ROM COUT1        */
/*   6655: STA $08 / STX $02 / STY $03     ; save char, X, Y                   */
/*   665B: LDA #0 / STA $00 / STA $01                                          */
/*   6661: LDA $29 / SEC / SBC #4 / CLC / ADC #$20 / STA $05                   */
/*   666B: LDA $28 / CLC / ADC $24 / STA $04                                   */
/*   6672: LDA $08 / SEC / SBC #$20                                            */
/*   6677: ASL / ASL / ROL $01 / ASL / ROL $01     ; (c - $20) * 8, 16-bit      */
/*   667E: CLC / ADC #$A9 / STA $00 / LDA $01 / ADC #$66 / STA $01             */
/*   6689: LDX #0                                                              */
/*   668B: TXA / TAY / LDA ($00),Y / LDY #0 / STA ($04),Y                      */
/*   6693: LDA $05 / CLC / ADC #4 / STA $05        ; next hi-res scanline       */
/*   669A: INX / CPX #8 / BNE $668B                                            */
/*   669F: LDX $02 / LDY $03 / JMP $6651           ; restore, then COUT1        */
/*   66A6: JMP $6655                       ; dead: nothing references $66A6     */
/*   66A9: font, 8 bytes per glyph                                             */
/*                                                                            */
/* Two things worth noting. The destination high byte is BASH - 4 + $20, i.e.  */
/* BASH + $1C, which maps text page 1 ($04xx) onto hi-res page 1 ($20xx): the  */
/* text cursor drives where the glyph lands. And after blitting, control falls  */
/* into $6651, so ROM COUT1 still runs and does the cursor advance, wrap and   */
/* scroll -- the game draws the glyph and lets the ROM keep the bookkeeping.   */
/* ========================================================================== */

void game_cout_hook(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$664A*/ CYCLES(0x664a, 9);
  const uint8_t ch = s_a; // PHA: the original char, high bit intact.
  const uint8_t glyph = (uint8_t)(ch & 0x7f);

  if (glyph >= 0x20) {
    // $664F BCS -- the branch itself, taken here (this is the taken arm;
    // glyph < 0x20 skips the block below without paying the extra cycle).
    /*$664F*/ CYCLES(0x664f, 1);
    /*$6655*/ CYCLES(0x6655, 82);

    // The original's SBC/ADC pairs honour the D flag. COUT is never reached in
    // decimal mode -- the ROM clears D at reset and neither BASIC nor the game
    // sets it around output -- so rather than carry dead decimal paths, fail
    // loudly if that assumption ever breaks.
    if (s_status_d) {
      fprintf(stderr, "game_cout_hook: entered with decimal mode set\n");
      error_handler(0x664a);
      abort();
    }

    ram_poke(0x0008, glyph);
    ram_poke(0x0002, s_x);
    ram_poke(0x0003, s_y);
    ram_poke(0x0000, 0x00);
    ram_poke(0x0001, 0x00);

    // Destination: the hi-res row matching the current text cursor.
    // BASH - 4 + $20 == BASH + $1C maps $04xx (text page 1) to $20xx (hi-res 1).
    ram_poke(0x0005, (uint8_t)(ram_peek(0x0029) - 0x04 + 0x20));
    ram_poke(0x0004, (uint8_t)(ram_peek(0x0028) + ram_peek(0x0024)));

    // Source: font at $66A9, 8 bytes per glyph, first glyph is $20.
    const uint16_t src = (uint16_t)(0x66a9 + (uint16_t)(glyph - 0x20) * 8);
    ram_poke(0x0000, (uint8_t)src);
    ram_poke(0x0001, (uint8_t)(src >> 8));
    s_x = 0x00;

    for (unsigned row = 0; row < 8; ++row) {
      /*$668B*/ CYCLES(0x668b, 33);
      s_a = (uint8_t)row; // TXA
      s_y = (uint8_t)row; // TAY
      const uint8_t bits = peek((uint16_t)(ram_peek16al(0x0000) + row));
      s_y = 0x00;
      poke(ram_peek16al(0x0004), bits);
      // Advance one hi-res scanline within the character cell: +$400.
      s_a = (uint8_t)(ram_peek(0x0005) + 0x04);
      ram_poke(0x0005, s_a);
      s_x = (uint8_t)(row + 1);
      // $669D BNE -- the branch itself, taken on every iteration but the
      // last (the loop test is `INX; CPX #8; BNE $668B`; row==7 is the one
      // iteration where it falls through instead of looping).
      if (row != 7) {
        /*$669D*/ CYCLES(0x669d, 1);
      }
    }

    /*$669F*/ CYCLES(0x669f, 9);
    s_x = ram_peek(0x0002);
    s_y = ram_peek(0x0003);
  }

  /*$6651*/ CYCLES(0x6651, 7);
  s_a = ch; // PLA
  rom_cout1(0xfffe); // JMP $FDF0

  if (ret_addr)
    pop16();
}
