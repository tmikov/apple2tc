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
/* $6064 is a 128-byte dot-pattern table, $6064-$60E3, immediately before the */
/* plotter code. It is 16 inks of 8 bytes, and the index the original builds  */
/*                                                                            */
/*     (($01 << 1 | scanline & 1) << 2) | ($02 & 3)                           */
/*                                                                            */
/* is just ink * 8 + parity * 4 + (column & 3): each ink carries four dot     */
/* phases for the column's position within the hi-res byte, in two sets so    */
/* that odd and even scanlines can differ. Ink 0 is all zeros and erases; ink */
/* 15 is $7F throughout and fills solid; the rest are dithers, several of     */
/* them the same pattern at opposite phase, which on a hi-res display is how  */
/* you get the complementary colour. Whatever comes out is ANDed with that    */
/* scanline's mask.                                                           */
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

/* ========================================================================== */
/* $7019, $7024, $7000 -- the screen-script primitives.                       */
/*                                                                            */
/* $7113 runs a little byte-coded display list: it fetches an opcode, then    */
/* that opcode's operands, then draws. 'H' ($48) is a horizontal run, 'V'     */
/* ($56) a vertical one, 'P' ($50) a single cell, 'T' ($54) sets $0304. The   */
/* three routines here are what every one of those cases is built from, which */
/* is why $7019 alone has fourteen call sites.                                */
/*                                                                            */
/* Why every draw happens twice                                               */
/* ---------------------------                                                */
/* The 'V' case is the clearest: $7185 calls $7000, then $7188 calls $615A.   */
/* The first draws the run with the ROM's lo-res PLOT, the second draws the   */
/* same run as hi-res cells. The hi-res page is what the player sees; the     */
/* lo-res page is the game's own 40x48 occupancy map, and it reads it back    */
/* with the ROM's SCRN. $62D7 is the payoff -- it adds the current direction  */
/* deltas from $6232/$6237 to the snake's head, SCRNs the result, and finds   */
/* out what it is about to run into. Collision detection for free, at the     */
/* cost of drawing everything twice.                                          */
/* ========================================================================== */

void game_next_byte(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$7019*/ CYCLES(0x7019, 14);
  s_y = 0x00;
  s_a = peek(ram_peek16al(0x000a));

  const uint8_t lo = (uint8_t)(ram_peek(0x000a) + 0x01);
  ram_poke(0x000a, lo);
  s_status_not_z = lo;
  s_status_n = (lo & 0x80);

  if (lo) {
    /*$701F*/ CYCLES_EDGE(0x701f, 1);
  } else {
    /*$7021*/ CYCLES(0x7021, 5);
    const uint8_t hi = (uint8_t)(ram_peek(0x000b) + 0x01);
    ram_poke(0x000b, hi);
    s_status_not_z = hi;
    s_status_n = (hi & 0x80);
  }

  /*$7023*/ CYCLES(0x7023, 6);

  if (ret_addr)
    pop16();
}

void game_set_ink(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // The argument arrives in the Z flag, not in A: every caller does
  // `LDA $01 / JSR $7024`, and $01 is the same ink byte the hi-res plotter
  // takes. Zero erases, anything else draws -- so the lo-res map gets colour
  // 0 or colour 5 and nothing in between. It is a two-state map, not a
  // picture.
  /*$7024*/ CYCLES(0x7024, 2);
  if (!s_status_not_z) {
    /*$7024*/ CYCLES_EDGE(0x7024, 1);
  } else {
    /*$7026*/ CYCLES(0x7026, 2);
    s_a = 0x05;
  }

  /*$7028*/ CYCLES(0x7028, 3);
  rom_setcol(0x0000); // JMP $F864 -- a tail call, so no return address.

  if (ret_addr)
    pop16();
}

void game_lores_vline(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // Same shape as game_plot_vline: rows $03 through $08 inclusive, down
  // column $02. Unlike that one it puts $03 back where it found it, because
  // the caller draws the hi-res run over the same coordinates next.
  /*$7000*/ CYCLES(0x7000, 6);
  push8(ram_peek(0x0003));

  for (;;) {
    /*$7003*/ CYCLES(0x7003, 12);
    s_a = ram_peek(0x0003);
    s_y = ram_peek(0x0002);
    rom_plot(0x7009);

    /*$700A*/ CYCLES(0x700a, 8);
    const uint8_t row = ram_peek(0x0003);
    const uint8_t last = ram_peek(0x0008);
    s_status_c = (row >= last);
    s_status_not_z = (row != last);
    s_status_n = ((uint8_t)(row - last) & 0x80);
    if (row == last)
      break;

    /*$7010*/ CYCLES(0x7010, 8);
    ram_poke(0x0003, (uint8_t)(row + 1));
  }

  /*$700E*/ CYCLES_EDGE(0x700e, 1);
  /*$7015*/ CYCLES(0x7015, 13);
  const uint8_t saved = pop8();
  s_a = saved;
  s_status_not_z = saved;
  s_status_n = (saved & 0x80);
  ram_poke(0x0003, saved);

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $71F3, $7226, $7267, $702B -- the score.                                   */
/*                                                                            */
/* The score is BCD: four bytes at $7252-$7255, little-endian, eight digits.  */
/* $7267 adds to it with the 6502's decimal mode, and $71F3 prints one byte   */
/* of it as two digits by nibble, since in BCD a nibble is already a digit.   */
/*                                                                            */
/* $002C is the leading-zero flag, and it is the reason these two are a pair. */
/* $71F3 prints a digit only once $2C is non-zero, and sets $2C from the      */
/* first non-zero digit it sees, so leading zeros are suppressed across a     */
/* whole multi-byte number rather than per byte. A caller clears $2C, prints  */
/* the bytes most-significant first, then calls $7226 -- which prints a       */
/* single '0' if $2C is still clear, so that a score of zero is not printed   */
/* as nothing at all. $72E4 is the canonical example.                         */
/*                                                                            */
/* $2C is the ROM's H2, the right-hand endpoint of HLINE. The game uses the   */
/* same byte for both ($7159 stores a column into it), which is safe only     */
/* because drawing and score printing never interleave.                       */
/*                                                                            */
/* $B0 is '0' in Apple II ASCII (high bit set). Digits above 9 would print as */
/* punctuation, which is another way of saying the arithmetic must be BCD.    */
/* ========================================================================== */

/// $7209 and $7220: CLC / ADC #$B0 / JSR COUT. The two are byte-identical
/// apart from the return address the JSR pushes, so they share one body here.
/// No CYCLES of its own -- both call sites are already inside a counted block.
static void game_emit_digit(uint16_t cout_ret) {
  // ADC honours the D flag. Digits are printed with D clear: $7267 is the
  // only thing that sets it and it clears it again before returning. Say so
  // loudly rather than carry a decimal path that cannot be reached.
  if (s_status_d) {
    fprintf(stderr, "game_emit_digit: entered with decimal mode set\n");
    error_handler(0x71f3);
    abort();
  }

  const uint8_t digit = s_a;
  const uint16_t sum = (uint16_t)digit + 0x00b0 + s_status_c;
  s_status_c = (uint8_t)(sum >> 8);
  s_status_v = ovf8((uint8_t)sum, digit, 0xb0);
  s_a = (uint8_t)sum;
  s_status_not_z = s_a;
  s_status_n = (s_a & 0x80);
  rom_cout(cout_ret);
}

void game_print_bcd(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$71F3*/ CYCLES(0x71f3, 15);
  const uint8_t byte = s_a;
  push8(byte);
  uint8_t digit = (uint8_t)(byte >> 4);
  s_a = digit;
  // $71F8 CMP #$00. N is always clear here -- a nibble cannot reach $80 --
  // but the compare still happens, so record what it leaves.
  s_status_c = 0x01;
  s_status_not_z = digit;
  s_status_n = 0x00;

  if (!digit) {
    /*$71FA*/ CYCLES_EDGE(0x71fa, 1);
  } else {
    /*$71FC*/ CYCLES(0x71fc, 3);
    ram_poke(0x002c, digit);
  }

  /*$71FE*/ CYCLES(0x71fe, 8);
  push8(digit);
  if (ram_peek(0x002c)) {
    /*$7201*/ CYCLES_EDGE(0x7201, 1);
    /*$7207*/ CYCLES(0x7207, 14);
    s_a = pop8();
    s_status_c = 0x00;
    game_emit_digit(0x720d);
  } else {
    /*$7203*/ CYCLES(0x7203, 7);
    pop8(); // The digit is dropped -- nothing significant has been printed.
  }

  /*$720E*/ CYCLES(0x720e, 10);
  digit = (uint8_t)(pop8() & 0x0f);
  s_a = digit;
  // $7211 CMP #$00, as above.
  s_status_c = 0x01;
  s_status_not_z = digit;
  s_status_n = 0x00;

  if (!digit) {
    /*$7213*/ CYCLES_EDGE(0x7213, 1);
  } else {
    /*$7215*/ CYCLES(0x7215, 3);
    ram_poke(0x002c, digit);
  }

  /*$7217*/ CYCLES(0x7217, 8);
  push8(digit);
  if (ram_peek(0x002c)) {
    /*$721A*/ CYCLES_EDGE(0x721a, 1);
    /*$721E*/ CYCLES(0x721e, 14);
    s_a = pop8();
    s_status_c = 0x00;
    game_emit_digit(0x7224);
    /*$7225*/ CYCLES(0x7225, 6);
  } else {
    /*$721C*/ CYCLES(0x721c, 10);
    pop8();
  }

  if (ret_addr)
    pop16();
}

void game_print_zero_if_blank(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$7226*/ CYCLES(0x7226, 5);
  const uint8_t seen = ram_peek(0x002c);
  s_a = seen;
  s_status_not_z = seen;
  s_status_n = (seen & 0x80);

  if (seen) {
    /*$7228*/ CYCLES_EDGE(0x7228, 1);
    /*$722F*/ CYCLES(0x722f, 6);
  } else {
    /*$722A*/ CYCLES(0x722a, 5);
    s_a = 0xb0;
    s_status_not_z = 0xb0;
    s_status_n = 0x80;
    rom_cout(0x0000); // JMP $FDED -- a tail call.
  }

  if (ret_addr)
    pop16();
}

void game_add_score(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // score += $71CC:$71CB, in BCD. adc_dec16 is the same decimal-mode adder
  // the emulator and the generated code use, rather than a second
  // hand-written one that could disagree with them about the undefined
  // corners of BCD ADC. Its result is the sum in the low byte and the flags
  // packed into the high byte.
  /*$7267*/ CYCLES(0x7267, 56);
  s_status_d = 0x01;

  uint16_t r = adc_dec16(ram_peek(0x71cb), ram_peek(0x7252), 0x00);
  s_status_c = ((uint8_t)(r >> 8) & 0x01);
  ram_poke(0x7252, (uint8_t)r);

  r = adc_dec16(ram_peek(0x71cc), ram_peek(0x7253), s_status_c);
  s_status_c = ((uint8_t)(r >> 8) & 0x01);
  ram_poke(0x7253, (uint8_t)r);

  r = adc_dec16(ram_peek(0x7254), 0x00, s_status_c);
  s_status_c = ((uint8_t)(r >> 8) & 0x01);
  ram_poke(0x7254, (uint8_t)r);

  r = adc_dec16(ram_peek(0x7255), 0x00, s_status_c);
  s_a = (uint8_t)r;
  const uint8_t flags = (uint8_t)(r >> 8);
  s_status_c = (flags & 0x01);
  s_status_not_z = (~flags & 2);
  s_status_v = ((flags & 0x40) != 0);
  s_status_n = (flags & 0x80);
  ram_poke(0x7255, s_a);

  s_status_d = 0x00;

  if (ret_addr)
    pop16();
}

void game_clear_hgr(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // Zero hi-res page 1, $2000 through $3FFF, a page at a time through the
  // $04/$05 pointer. The inner loop runs a full 256 bytes because Y wraps to
  // zero, so the terminating test is on the page number, not on Y.
  /*$702B*/ CYCLES(0x702b, 12);
  ram_poke(0x0004, 0x00);
  ram_poke(0x0005, 0x20);
  s_y = 0x00;

  for (;;) {
    uint8_t y;
    do {
      /*$7035*/ CYCLES(0x7035, 12);
      y = s_y;
      poke(ram_peek16al(0x0004) + y, 0x00);
      y = (uint8_t)(y + 1);
      s_y = y;
      if (y) {
        /*$703A*/ CYCLES_EDGE(0x703a, 1);
      }
    } while (y);

    /*$703C*/ CYCLES(0x703c, 12);
    const uint8_t page = (uint8_t)(ram_peek(0x0005) + 1);
    ram_poke(0x0005, page);
    s_status_c = (page >= 0x40);
    s_status_n = ((uint8_t)(page - 0x40) & 0x80);
    if (page == 0x40)
      break;
    /*$7042*/ CYCLES_EDGE(0x7042, 1);
  }

  /*$7044*/ CYCLES(0x7044, 6);

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $6C4B -- the next pseudo-random byte.                                      */
/*                                                                            */
/* There is no generator here at all: the game walks a pointer ($0E/$0F)      */
/* through its own memory and hands back whatever byte it lands on, skipping  */
/* any with the high bit set and restarting the walk at $1800 when it finds   */
/* one. Program text and data are the entropy. The result is therefore always */
/* $00-$7F, which is what makes it usable directly as a coordinate.           */
/*                                                                            */
/* The restart does not advance the pointer -- it stores $1800 and jumps      */
/* straight back to the load -- so a byte at $1800 with bit 7 set would hang  */
/* the game. Nothing enforces that; the original simply relies on it.         */
/* ========================================================================== */

void game_rand_byte(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$6C4B*/ CYCLES(0x6c4b, 7);
  const uint8_t lo = (uint8_t)(ram_peek(0x000e) + 0x01);
  ram_poke(0x000e, lo);
  if (lo) {
    /*$6C4D*/ CYCLES_EDGE(0x6c4d, 1);
  } else {
    /*$6C4F*/ CYCLES(0x6c4f, 5);
    ram_poke(0x000f, (uint8_t)(ram_peek(0x000f) + 0x01));
  }

  for (;;) {
    /*$6C51*/ CYCLES(0x6c51, 9);
    s_y = 0x00;
    const uint8_t b = peek(ram_peek16al(0x000e));
    s_a = b;
    s_status_not_z = b;
    s_status_n = (b & 0x80);
    if (!(b & 0x80)) {
      /*$6C55*/ CYCLES_EDGE(0x6c55, 1);
      /*$6C62*/ CYCLES(0x6c62, 6);
      break;
    }
    /*$6C57*/ CYCLES(0x6c57, 13);
    ram_poke(0x000e, 0x00);
    ram_poke(0x000f, 0x18);
  }

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $6B93 -- the merging cell plotter.                                         */
/*                                                                            */
/* Same shape as $60E4: load the shape's masks, then walk the cell's four     */
/* scanlines. Two things differ.                                              */
/*                                                                            */
/* It merges instead of replacing. $60E7 stores `pattern & mask`, wiping      */
/* whatever else shared the byte; this one stores `(pattern ^ $7F) & mask |   */
/* existing`, so it only ever sets bits. $7F and not $FF: bit 7 is the hi-res */
/* palette bit for the byte, and inverting that would shift the whole byte's  */
/* colour.                                                                    */
/*                                                                            */
/* And it builds the table index differently. $60F7 does ROL $06, giving      */
/*                                                                            */
/*     ink * 8 + parity * 4 + (column & 3)                                    */
/*                                                                            */
/* while $6BAD does ROR $06 on the same operands, giving                      */
/*                                                                            */
/*     (ink >> 1) * 4 + (column & 3)                                          */
/*                                                                            */
/* -- the parity bit lands in bit 7 and is shifted straight back out by the   */
/* two ASLs that follow. Whether that was intended is not something the       */
/* binary can tell us. What it means in practice is that the caller's ink 1   */
/* selects row 0, all zeros, which the EOR then turns into a solid $7F: the   */
/* routine fills the shape rather than dithering it. Reproduced exactly,      */
/* including the lost bit, because the screen the game draws depends on it.   */
/* ========================================================================== */

void game_plot_shape_merge(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$6B93*/ CYCLES(0x6b93, 6);
  game_load_shape(0x6b95);

  /*$6B96*/ CYCLES(0x6b96, 22);
  const uint8_t row = ram_peek(0x0003);
  s_x = 0x00;
  s_y = row;
  ram_poke(0x0007, 0x00);
  ram_poke(0x0004, ram_peek(0x6000 + row));
  ram_poke(0x0005, ram_peek(0x6030 + row));

  for (unsigned line = 0; line < 4; ++line) {
    /*$6BA6*/ CYCLES(0x6ba6, 85);

    // $6BAA-$6BB1: LDA $07 / ROR A / ROR $06 / ASL $06 / ASL $06. See the
    // header -- the parity bit rotated into bit 7 is shifted back out.
    const uint8_t parity = (uint8_t)(ram_peek(0x0007) & 0x01);
    uint8_t idx = ram_peek(0x0001);
    idx = (uint8_t)((parity << 7) | (idx >> 1));
    idx = (uint8_t)(idx << 2);
    idx = (uint8_t)(idx | (ram_peek(0x0002) & 0x03));
    ram_poke(0x0006, idx);

    const uint8_t col = ram_peek(0x0002);
    s_y = col;
    const uint16_t dst = (uint16_t)(ram_peek16al(0x0004) + col);
    poke(
        dst,
        (uint8_t)(((ram_peek(0x6064 + idx) ^ 0x7f) & ram_peek(0x6060 + line)) | peek(dst)));

    s_x = (uint8_t)(line + 1);
    ram_poke(0x0007, (uint8_t)(ram_peek(0x0007) + 0x01));

    if (s_status_d) {
      fprintf(stderr, "game_plot_shape_merge: entered with decimal mode set\n");
      error_handler(0x6b93);
      abort();
    }
    // $6BCE-$6BD3: CLC / ADC #4 -- down one scanline. Only V outlives this;
    // the CPX two instructions later rewrites C, Z and N.
    const uint8_t hi = ram_peek(0x0005);
    const uint8_t next_hi = (uint8_t)(hi + 0x04);
    s_status_v = ovf8(next_hi, hi, 0x04);
    s_a = next_hi;
    ram_poke(0x0005, next_hi);

    // $6BD5 CPX #4.
    s_status_c = (s_x >= 0x04);
    s_status_not_z = (s_x != 0x04);
    s_status_n = ((uint8_t)(s_x - 0x04) & 0x80);
    if (line != 3) {
      /*$6BD7*/ CYCLES_EDGE(0x6bd7, 1);
    }
  }

  /*$6BD9*/ CYCLES(0x6bd9, 6);

  if (ret_addr)
    pop16();
}
