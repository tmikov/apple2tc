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
    /*$664F*/ CYCLES_EDGE(0x664f, 1);
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
        /*$669D*/ CYCLES_EDGE(0x669d, 1);
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

/* ========================================================================== */
/* $6127, $60E7, $60E4 -- the hi-res cell plotter.                            */
/*                                                                            */
/* The game treats hi-res page 1 as a grid of 48 cell rows, each four         */
/* scanlines tall and one byte (seven pixels) wide. $6000 and $6030 hold the  */
/* low and high bytes of each row's base address -- $2000, $3000, $2080,      */
/* $3080, ... -- and successive scanlines within a cell are $400 apart, which */
/* is why walking down a cell is just +4 on the high byte.                    */
/*                                                                            */
/* Arguments, in zero page:                                                   */
/*   $00  shape index; picks four AND masks from the table at $6174           */
/*   $01  ink: 0 erases, 1 draws                                              */
/*   $02  column -- the byte offset within the cell row                       */
/*   $03  cell row, 0-47                                                      */
/* Scratch, also in zero page and so still written faithfully:                */
/*   $04/$05  destination pointer, advanced one scanline per iteration        */
/*   $06      index into the dot-pattern table at $6064                       */
/*   $07      scanline counter, 0-3                                           */
/*                                                                            */
/* $6064 holds sixteen bytes: eight zeros, then four dot patterns repeated    */
/* with their phase swapped. The index is                                     */
/*                                                                            */
/*     (($01 << 1 | scanline & 1) << 2) | ($02 & 3)                           */
/*                                                                            */
/* so ink 0 always lands in the zero half and erases, while ink 1 picks a     */
/* pattern whose phase comes from the scanline parity and the low two bits of */
/* the column -- a dither that keeps the shape's texture aligned to the       */
/* hi-res byte grid. Whatever comes out is ANDed with that scanline's mask.   */
/*                                                                            */
/* Decoded by hand from snake-byte.lst and the tables read out of             */
/* snake-byte.b33. The CYCLES constants and the block boundaries they sit on  */
/* deliberately mirror what apple2tc generates for the same addresses, so the */
/* block-head trace stays identical and probe-acceptance.sh can compare this  */
/* against the interpreter instruction for instruction.                       */
/* ========================================================================== */

void game_load_shape(uint16_t ret_addr) {
  // Read by CYCLES() when tracing is on; every generated function
  // declares it too.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$6127*/ CYCLES(0x6127, 53);
  // Four masks per shape, so the index is shape * 4. The original built that
  // with two ASLs and then walked the table with INX, which is why X is left
  // pointing at the last entry rather than one past it.
  const uint8_t first = (uint8_t)(ram_peek(0x0000) << 2);
  uint8_t mask = 0;
  for (unsigned line = 0; line < 4; ++line) {
    mask = ram_peek(0x6174 + (uint8_t)(first + line));
    ram_poke(0x6060 + line, mask);
  }
  s_x = (uint8_t)(first + 3);
  s_a = mask;
  s_status_not_z = mask;
  s_status_n = (mask & 0x80);

  if (ret_addr)
    pop16();
}

void game_draw_cell(uint16_t ret_addr) {
  // Read by CYCLES() when tracing is on; every generated function
  // declares it too.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$60E7*/ CYCLES(0x60e7, 22);
  const uint8_t row = ram_peek(0x0003);
  s_x = 0x00;
  ram_poke(0x0007, 0x00);
  ram_poke(0x0004, ram_peek(0x6000 + row));
  ram_poke(0x0005, ram_peek(0x6030 + row));

  for (unsigned line = 0; line < 4; ++line) {
    /*$60F7*/ CYCLES(0x60f7, 16);
    // Built in place in $06, as the original did: ROL rotates the scanline
    // parity in at the bottom, then two ASLs make room for the column phase.
    // The intermediate value is written out because $06 is zero page and a
    // probe may sample memory between these two blocks.
    ram_poke(0x0006, (uint8_t)((ram_peek(0x0001) << 1) | (ram_peek(0x0007) & 0x01)));
    /*$6100*/ CYCLES(0x6100, 62);
    ram_poke(0x0006, (uint8_t)((ram_peek(0x0006) << 2) | (ram_peek(0x0002) & 0x03)));

    const uint8_t col = ram_peek(0x0002);
    s_y = col;
    poke(
        ram_peek16al(0x0004) + col,
        ram_peek(0x6064 + ram_peek(0x0006)) & ram_peek(0x6060 + line));

    s_x = (uint8_t)(line + 1);
    ram_poke(0x0007, (uint8_t)(ram_peek(0x0007) + 0x01));

    // $611D-$611F: CLC / ADC #4 -- down one scanline. Decimal mode would turn
    // that into nonsense; the game never plots with D set, so say so loudly
    // rather than carry a dead BCD path the way the generated code must.
    if (s_status_d) {
      fprintf(stderr, "game_draw_cell: entered with decimal mode set\n");
      error_handler(0x60e7);
      abort();
    }
    const uint8_t hi = ram_peek(0x0005);
    const uint8_t next_hi = (uint8_t)(hi + 0x04);
    s_status_c = 0x00;
    s_status_v = ovf8(next_hi, hi, 0x04);
    s_a = next_hi;
    ram_poke(0x0005, next_hi);

    // $6122 CPX #4. The carry and sign it leaves are what the caller sees.
    s_status_not_z = (s_x != 0x04);
    s_status_c = (s_x >= 0x04);
    s_status_n = ((uint8_t)(s_x - 0x04) & 0x80);
    if (line != 3) {
      /*$6124*/ CYCLES_EDGE(0x6124, 1);
    }
  }

  /*$6126*/ CYCLES(0x6126, 6);
  if (ret_addr)
    pop16();
}

void game_plot_shape(uint16_t ret_addr) {
  // Read by CYCLES() when tracing is on; every generated function
  // declares it too.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$60E4*/ CYCLES(0x60e4, 6);
  game_load_shape(0x60e6);
  game_draw_cell(0x0000);

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $6148, $615A -- runs of cells.                                             */
/*                                                                            */
/* Both load the shape once and then repeat game_draw_cell along one axis:    */
/* $6148 walks the column ($02), $615A walks the row ($03). The run ends when */
/* the moving coordinate reaches $08, tested after the cell is drawn, so the  */
/* endpoint is inclusive and a degenerate run still plots one cell. A start   */
/* past the end wraps through 255 rather than drawing nothing -- faithfully   */
/* reproduced below, since nothing in the original guards against it.         */
/*                                                                            */
/* Arguments are game_draw_cell's, plus:                                      */
/*   $08  last column ($6148) or last row ($615A), inclusive                  */
/*                                                                            */
/* $70D4 shows what they are for: with $08 = $27 it draws the four borders of */
/* the 40x40 cell playfield -- top and bottom with $6148, left and right with */
/* $615A -- which is where the grid's dimensions come from.                   */
/*                                                                            */
/* The register and flag assignments are not decoration. Callers are still    */
/* generated code that reads this machine state, so a hand-written routine    */
/* has to leave behind what the 6502 would. Where apple2tc's own output drops */
/* some of it, that is its whole-program DCE proving a particular flag dead;  */
/* setting it anyway is always safe, and cheaper than re-deriving the proof   */
/* every time a caller changes.                                               */
/* ========================================================================== */

void game_plot_hline(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$6148*/ CYCLES(0x6148, 6);
  game_load_shape(0x614a);

  for (;;) {
    /*$614B*/ CYCLES(0x614b, 6);
    game_draw_cell(0x614d);

    /*$614E*/ CYCLES(0x614e, 8);
    const uint8_t col = ram_peek(0x0002);
    const uint8_t last = ram_peek(0x0008);
    s_a = col;
    s_status_c = (col >= last);
    s_status_not_z = (col != last);
    s_status_n = ((uint8_t)(col - last) & 0x80);
    if (col == last)
      break;

    /*$6154*/ CYCLES(0x6154, 8);
    ram_poke(0x0002, (uint8_t)(col + 1));
  }

  /*$6152*/ CYCLES_EDGE(0x6152, 1);
  /*$6159*/ CYCLES(0x6159, 6);

  if (ret_addr)
    pop16();
}

void game_plot_vline(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$615A*/ CYCLES(0x615a, 6);
  game_load_shape(0x615c);

  for (;;) {
    /*$615D*/ CYCLES(0x615d, 6);
    game_draw_cell(0x615f);

    /*$6160*/ CYCLES(0x6160, 8);
    const uint8_t row = ram_peek(0x0003);
    const uint8_t last = ram_peek(0x0008);
    s_a = row;
    s_status_c = (row >= last);
    s_status_not_z = (row != last);
    s_status_n = ((uint8_t)(row - last) & 0x80);
    if (row == last)
      break;

    /*$6166*/ CYCLES(0x6166, 8);
    ram_poke(0x0003, (uint8_t)(row + 1));
  }

  /*$6164*/ CYCLES_EDGE(0x6164, 1);
  /*$6159*/ CYCLES(0x6159, 6);

  if (ret_addr)
    pop16();
}
