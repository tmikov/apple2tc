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
#include "game_native.h"

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
  // Adapter. The body is game_load_shape_masks() in game_native.c; what is
  // left here is the machine state its generated callers observe. One block,
  // so the CYCLES site and the block-head trace are unaffected.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$6127*/ CYCLES(0x6127, 53);
  const uint8_t shape = ram_peek(0x0000);
  const uint8_t mask = game_load_shape_masks(shape);
  // The original walked the table with INX, so X is left pointing at the last
  // entry rather than one past it.
  s_x = (uint8_t)((uint8_t)(shape << 2) + 3);
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

/* ========================================================================== */
/* $7045 -- draw the playfield, and run the level's display list.             */
/*                                                                            */
/* One routine, but two halves. The first draws what every level has: the     */
/* border, in both representations (see the $7019 header for why everything   */
/* is drawn twice). The second, from $7113, interprets the level's own script */
/* for whatever else it needs.                                                */
/*                                                                            */
/* The script                                                                 */
/* ----------                                                                 */
/* $8000 holds 29 of them, one per level, each ending in '*'. $7113 sets the  */
/* pointer to $8000 and skips $0303 - 1 whole scripts to find the current     */
/* level's. The opcodes, all operands fetched with game_next_byte:            */
/*                                                                            */
/*   'H' $48  ink, column, last column, row  -- horizontal run                */
/*   'V' $56  ink, row, last row, column     -- vertical run                  */
/*   'P' $50  ink, column, row               -- one cell                      */
/*   'T' $54  value                          -- store to $0304                */
/*   'E' $45  (none)  -- reset $0303 to 1 and start over, i.e. wrap to level 1 */
/*   '*' $2A  end of this level's script                                      */
/*                                                                            */
/* Anything else falls through $71C4 back to the dispatch and is ignored.     */
/* Decoded from the data: 'H' appears in 26 of the 29 scripts and 'V' in 26,  */
/* 'T' only in level 1's (which is otherwise empty -- the border is all there */
/* is), 'E' only in level 30's, and 'P' in none of them at all.               */
/*                                                                            */
/* Verification                                                               */
/* ------------                                                               */
/* Neither committed recording finishes level 1, so none of this ran in the   */
/* gate until snake-byte-easy.b33 (make-easy.sh) lowered the apple quota and  */
/* let play-hires reach level 3. 'H', 'V' and the skip loop are covered by    */
/* that scenario. 'E', 'P' and the $71C4 fallthrough are not, and cannot be   */
/* by replay: 'E' needs level 30, and no script in the game uses 'P'.         */
/*                                                                            */
/* On flags: as elsewhere in this file the routine leaves what the 6502 would */
/* leave, since generated callers can observe it. The exception is a load     */
/* immediately before a call that overwrites the flags anyway -- LDA/LDY into */
/* the ROM's HLINE and PLOT -- where only the register is set, matching what  */
/* game_cout_hook already does at $669F.                                      */
/* ========================================================================== */

void game_draw_playfield(uint16_t ret_addr) {
  // Adapter for game_draw_playfield_native(). The largest single conversion:
  // 61 trace sites, a fifth of what game.c had. $7045 itself survives, charged
  // zero here and for real inside.
  //
  // Nothing of the machine state outlives it that a caller reads -- the
  // routine ends in an RTS after a comparison whose result nothing consults --
  // so the adapter marshals nothing back.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$7045*/ CYCLES(0x7045, 0);
  game_draw_playfield_native();

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $6641, $660F, $6BEF, $6BDA, $71CD -- snake state and scoring setup.        */
/*                                                                            */
/* $624F/$6250 are the snake's head column and row; $6251/$6252 are the tail. */
/* $62D7 walks the head forward by adding the current direction's deltas from */
/* $6232/$6237 and SCRNs the result off the lo-res map (see the $7019 header).*/
/*                                                                            */
/* $0301 is the difficulty, 0-2, and $71CD is what gives it away: it indexes  */
/* the three-byte table at $71C8 -- $10, $15, $20 -- with $0301 and adds that */
/* entry to $71CB/$71CC once per level, in BCD. So an apple is worth          */
/* base[difficulty] * level, which is why $71CB read $15 throughout the       */
/* recordings: difficulty 1, level 1. It is also what decides the two         */
/* optional wall gaps in game_draw_playfield.                                 */
/* ========================================================================== */

void game_install_cout_hook(uint16_t ret_addr) {
  // Adapter for game_install_cout_vector().
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$6641*/ CYCLES(0x6641, 16);
  game_install_cout_vector();
  // The LDA #$4A flags are overwritten by the second load; only these outlive.
  s_a = 0x66;
  s_status_not_z = 0x66;
  s_status_n = 0x00;

  if (ret_addr)
    pop16();
}

void game_start_life_adapter(uint16_t ret_addr) {
  // Adapter for game_start_life(). Named for what it is; rom.externs maps
  // $660F to this.
  //
  // Renamed from game_reset_snake on the way through: it sets the snake's head
  // column, but the other eight stores are the two bouncers, placed at
  // opposite corners and converging. Writing it as a struct is what made that
  // obvious -- as parallel ram_pokes it read as nine unrelated bytes.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$660F*/ CYCLES(0x660f, 50);
  s_a = game_start_life(s_a);
  s_status_not_z = s_a;
  s_status_n = 0x00;

  if (ret_addr)
    pop16();
}

void game_mark_head(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // PLOT with the row and column the caller loaded, marking the head on the
  // lo-res occupancy map, then raise the two flags that say it is there.
  /*$6BEF*/ CYCLES(0x6bef, 6);
  rom_plot(0x6bf1);

  /*$6BF2*/ CYCLES(0x6bf2, 16);
  s_a = 0x01;
  s_status_not_z = 0x01;
  s_status_n = 0x00;
  ram_poke(0x0305, 0x01);
  ram_poke(0x6c46, 0x01);

  if (ret_addr)
    pop16();
}

void game_draw_head(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // Draw the cell the caller set up, then -- if $0305 says the head is on it
  // -- merge shape 1 over the top, so the head reads as a head rather than
  // replacing the body cell underneath. $0305 is consumed here.
  /*$6BDA*/ CYCLES(0x6bda, 6);
  game_plot_shape(0x6bdc);

  /*$6BDD*/ CYCLES(0x6bdd, 6);
  if (ram_peek(0x0305)) {
    /*$6BE2*/ CYCLES(0x6be2, 11);
    ram_poke(0x0000, 0x01);
    game_plot_shape_merge(0x6be8);
  } else {
    /*$6BE0*/ CYCLES_EDGE(0x6be0, 1);
  }

  /*$6BE9*/ CYCLES(0x6be9, 12);
  s_status_not_z = 0x00;
  s_status_n = 0x00;
  ram_poke(0x0305, 0x00);

  if (ret_addr)
    pop16();
}

void game_set_apple_value(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // $71CB/$71CC = $71C8[difficulty] * level, by repeated BCD addition. X is
  // never touched in the loop, so it is the same table entry added $0303
  // times.
  /*$71CD*/ CYCLES(0x71cd, 20);
  ram_poke(0x71cb, 0x00);
  ram_poke(0x71cc, 0x00);
  s_x = ram_peek(0x0301);
  s_y = ram_peek(0x0303);
  s_status_d = 0x01;

  for (;;) {
    /*$71DC*/ CYCLES(0x71dc, 28);
    s_a = ram_peek(0x71c8 + s_x);
    s_status_c = 0x00;

    uint16_t r = adc_dec16(s_a, ram_peek(0x71cb), s_status_c);
    s_a = (uint8_t)r;
    s_status_c = ((uint8_t)(r >> 8) & 0x01);
    ram_poke(0x71cb, s_a);

    r = adc_dec16(ram_peek(0x71cc), 0x00, s_status_c);
    s_a = (uint8_t)r;
    const uint8_t flags = (uint8_t)(r >> 8);
    s_status_c = (flags & 0x01);
    s_status_v = ((flags & 0x40) != 0);
    ram_poke(0x71cc, s_a);

    // DEY overwrites the N and Z the ADC just left.
    const uint8_t n = (uint8_t)(s_y - 0x01);
    s_y = n;
    s_status_not_z = n;
    s_status_n = (n & 0x80);
    if (!n)
      break;
    /*$71EF*/ CYCLES_EDGE(0x71ef, 1);
  }

  /*$71F1*/ CYCLES(0x71f1, 8);
  s_status_d = 0x00;

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $7642, $64A9, $7633 -- apples, and the sound trick.                        */
/*                                                                            */
/* $6C49 is the game's mute switch, and it is a nice piece of work: every     */
/* sound routine reads `LDA $C000,Y` with Y = $6C49, and $6C2C picks either   */
/* $30 or $20 for it. $C030 is the speaker; $C020 is the cassette output. So  */
/* turning the sound off routes the identical click to a port nobody is       */
/* listening to, and the timing loops do not change at all -- no branch in    */
/* the hot path, and muting cannot alter the game's speed.                    */
/* ========================================================================== */

void game_place_apple(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // Rejection sampling: take two pseudo-random bytes as column and row, ask
  // the lo-res map whether that cell is free, and start over if it is not.
  // game_rand_byte returns $00-$7F while the field is 40x40, so most draws
  // land outside it and hit the non-zero border or garbage -- the retry loop
  // is doing more work than it looks like.
  for (;;) {
    /*$7642*/ CYCLES(0x7642, 6);
    game_rand_byte(0x7644);
    /*$7645*/ CYCLES(0x7645, 9);
    ram_poke(0x0002, s_a);
    game_rand_byte(0x7649);
    /*$764A*/ CYCLES(0x764a, 15);
    ram_poke(0x0003, s_a);
    s_a = ram_peek(0x0003);
    const uint8_t col = ram_peek(0x0002);
    s_status_not_z = col;
    s_y = col;
    rom_scrn(0x7652);

    /*$7653*/ CYCLES(0x7653, 2);
    if (!s_status_not_z)
      break;
    /*$7653*/ CYCLES_EDGE(0x7653, 1);
  }

  // White on the occupancy map, so the snake's collision test sees it.
  /*$7655*/ CYCLES(0x7655, 8);
  s_a = 0x0f;
  rom_setcol(0x7659);

  /*$765A*/ CYCLES(0x765a, 12);
  s_a = ram_peek(0x0003);
  s_y = ram_peek(0x0002);
  rom_plot(0x7660);

  /*$7661*/ CYCLES(0x7661, 16);
  ram_poke(0x0000, 0x01); // shape 1
  ram_poke(0x0001, 0x09); // ink 9
  game_plot_shape(0x766b);

  // One more apple on screen, BCD at $725F/$7260. $77D0 checks this pair and
  // calls back here when it reaches zero.
  /*$766C*/ CYCLES(0x766c, 32);
  s_status_d = 0x01;
  uint16_t r = adc_dec16(ram_peek(0x725f), 0x01, 0x00);
  s_status_c = ((uint8_t)(r >> 8) & 0x01);
  ram_poke(0x725f, (uint8_t)r);

  r = adc_dec16(ram_peek(0x7260), 0x00, s_status_c);
  s_a = (uint8_t)r;
  const uint8_t flags = (uint8_t)(r >> 8);
  s_status_c = (flags & 0x01);
  s_status_not_z = (~flags & 2);
  s_status_v = ((flags & 0x40) != 0);
  s_status_n = (flags & 0x80);
  ram_poke(0x7260, s_a);
  s_status_d = 0x00;

  if (ret_addr)
    pop16();
}

void game_sound_sweep(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // Two sweeps. X starts at 0, so the first DEX wraps to 255 and the delay
  // between clicks runs 256, 255, ... 1 -- the pitch rises. The second half
  // counts X up from 0, so the delay runs 256, 1, 2, ... 255 and the pitch
  // falls. Together: the sound an apple makes.
  /*$64A9*/ CYCLES(0x64a9, 2);
  s_x = 0x00;

  do {
    /*$64AB*/ CYCLES(0x64ab, 4);
    s_y = s_x; // TXA / TAY

    uint8_t y;
    do {
      /*$64AD*/ CYCLES(0x64ad, 4);
      y = (uint8_t)(s_y - 0x01);
      s_y = y;
      if (y) {
        /*$64AE*/ CYCLES_EDGE(0x64ae, 1);
      }
    } while (y);

    /*$64B0*/ CYCLES(0x64b0, 12);
    // The click. LDY $6C49 / LDA $C000,Y -- speaker or cassette, see above.
    // Neither the Y it loads nor the byte it reads outlives the next TAY.
    peek((uint16_t)(0xc000 + ram_peek(0x6c49)));
    s_x = (uint8_t)(s_x - 0x01);
    if (s_x) {
      /*$64B7*/ CYCLES_EDGE(0x64b7, 1);
    }
  } while (s_x);

  do {
    /*$64B9*/ CYCLES(0x64b9, 4);
    s_y = s_x;

    uint8_t y;
    do {
      /*$64BB*/ CYCLES(0x64bb, 4);
      y = (uint8_t)(s_y - 0x01);
      s_y = y;
      if (y) {
        /*$64BC*/ CYCLES_EDGE(0x64bc, 1);
      }
    } while (y);

    /*$64BE*/ CYCLES(0x64be, 12);
    // Here the Y and A do outlive the loop -- this is the last click before
    // the RTS, so the caller sees them.
    const uint8_t port = ram_peek(0x6c49);
    s_y = port;
    s_a = peek((uint16_t)(0xc000 + port));
    s_x = (uint8_t)(s_x + 0x01);
    s_status_not_z = s_x;
    s_status_n = (s_x & 0x80);
    if (s_x) {
      /*$64C5*/ CYCLES_EDGE(0x64c5, 1);
    }
  } while (s_x);

  /*$64C7*/ CYCLES(0x64c7, 6);

  if (ret_addr)
    pop16();
}

void game_eat_apple(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // One more apple eaten this level, BCD at $725E -- $77F8 compares it with
  // $78B2 -- and then the sound.
  /*$7633*/ CYCLES(0x7633, 22);
  s_status_d = 0x01;
  const uint16_t r = adc_dec16(ram_peek(0x725e), 0x01, 0x00);
  const uint8_t flags = (uint8_t)(r >> 8);
  s_status_c = (flags & 0x01);
  s_status_v = ((flags & 0x40) != 0);
  ram_poke(0x725e, (uint8_t)r);
  s_status_d = 0x00;
  game_sound_sweep(0x7640);

  /*$7641*/ CYCLES(0x7641, 6);

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $6217, $7590, $6B3D                                                        */
/* ========================================================================== */

void game_read_key(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // A 16-entry ring buffer at $623C: $624D is the write index, $624C the
  // read index. If advancing the write index would land on the read index the
  // buffer is full, and the key is dropped -- note it has already been stored
  // by then, so the byte is written and then disowned by not committing the
  // index. The RTS at $6216 belongs to the routine before this one; both of
  // the early exits here share it.
  /*$6217*/ CYCLES(0x6217, 10);
  s_x = ram_peek(0x624d);
  const uint8_t key = io_peek(0xc000);
  s_a = key;

  if (key & 0x80) {
    /*$621F*/ CYCLES(0x621f, 21);
    io_poke(0xc010, key); // clear the strobe
    const uint8_t w = s_x;
    ram_poke(0x623c + w, key);
    const uint8_t next = (uint8_t)((uint8_t)(w + 0x01) & 0x0f);
    s_x = (uint8_t)(w + 0x01);
    s_a = next;

    if (next != ram_peek(0x624c)) {
      /*$622E*/ CYCLES(0x622e, 10);
      ram_poke(0x624d, next);
      if (ret_addr)
        pop16();
      return;
    }
    /*$622C*/ CYCLES_EDGE(0x622c, 1);
  } else {
    /*$621D*/ CYCLES_EDGE(0x621d, 1);
  }

  /*$6216*/ CYCLES(0x6216, 6);

  if (ret_addr)
    pop16();
}

void game_show_key(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // Print the character in A at slot X of the key-redefinition screen. The
  // two arrow keys have no printable glyph, so they are shown as 'f' and 'g'
  // -- which in the game's own font at $66A9 is where the arrow shapes live.
  /*$7590*/ CYCLES(0x7590, 7);
  ram_poke(0x0002, s_x);
  if (s_a == 0x88) { // left arrow
    /*$7596*/ CYCLES(0x7596, 2);
    s_a = 0xe6;
  } else {
    /*$7594*/ CYCLES_EDGE(0x7594, 1);
  }

  /*$7598*/ CYCLES(0x7598, 4);
  s_status_c = (s_a >= 0x95);
  if (s_a == 0x95) { // right arrow
    /*$759C*/ CYCLES(0x759c, 2);
    s_a = 0xe7;
  } else {
    /*$759A*/ CYCLES_EDGE(0x759a, 1);
  }

  // Position the cursor from the per-slot tables, then print.
  /*$759E*/ CYCLES(0x759e, 23);
  push8(s_a);
  const uint8_t slot = s_x;
  ram_poke(0x0024, ram_peek(0x75b3 + slot));
  ram_poke(0x0025, ram_peek(0x75b9 + slot));
  rom_fc68(0x75ab);

  /*$75AC*/ CYCLES(0x75ac, 10);
  s_a = pop8();
  s_status_not_z = s_a;
  s_status_n = (s_a & 0x80);
  rom_cout(0x75af);

  /*$75B0*/ CYCLES(0x75b0, 9);
  s_x = ram_peek(0x0002);
  s_status_not_z = s_x;
  s_status_n = (s_x & 0x80);

  if (ret_addr)
    pop16();
}

void game_draw_side_walls(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // Both side walls, each in two segments of different ink, with the seam at
  // a row derived from $6255. The seam is what the player aims for.
  /*$6B3D*/ CYCLES(0x6b3d, 6);
  game_rand_byte(0x6b3f);

  /*$6B40*/ CYCLES(0x6b40, 26);
  ram_poke(0x0000, 0x15); // shape
  ram_poke(0x0001, 0x02); // ink of the upper segment
  ram_poke(0x0002, 0x00); // left wall
  ram_poke(0x0003, 0x01);
  const uint8_t seed = ram_peek(0x6255);
  s_a = seed;
  if (seed & 0x80) {
    // A negative seed is clamped, and $6255 is reset so the next call starts
    // from a known place.
    /*$6B55*/ CYCLES(0x6b55, 8);
    ram_poke(0x6255, 0xff);
    s_a = 0x70;
  } else {
    /*$6B53*/ CYCLES_EDGE(0x6b53, 1);
  }

  /*$6B5C*/ CYCLES(0x6b5c, 18);
  const uint8_t v = s_a;
  s_status_c = ((v >> 0x01) & 0x01); // LSR, then LSR again into $08
  ram_poke(0x0008, (uint8_t)(v >> 0x02));
  ram_poke(0x0008, (uint8_t)(ram_peek(0x0008) + 0x01));
  game_plot_vline(0x6b64);

  /*$6B65*/ CYCLES(0x6b65, 16);
  ram_poke(0x0002, 0x27); // right wall, same rows
  ram_poke(0x0003, 0x01);
  game_plot_vline(0x6b6f);

  /*$6B70*/ CYCLES(0x6b70, 30);
  ram_poke(0x0008, (uint8_t)(ram_peek(0x0008) + 0x01));
  const uint8_t seam = ram_peek(0x0008);
  push8(seam);
  ram_poke(0x0003, seam);
  ram_poke(0x0001, 0x0d); // ink of the lower segment
  ram_poke(0x0008, 0x27);
  game_plot_vline(0x6b81);

  /*$6B82*/ CYCLES(0x6b82, 18);
  ram_poke(0x0003, pop8());
  ram_poke(0x0002, 0x00);
  game_plot_vline(0x6b8b);

  // Tail call: SCRN of the bottom-centre cell, whose result the caller reads.
  /*$6B8C*/ CYCLES(0x6b8c, 7);
  s_a = 0x27;
  s_y = 0x14;
  rom_scrn(0x0000);

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $6AB8 -- is the next cell in direction $6B38 worth moving into?            */
/*                                                                            */
/* The move validator behind the direction-choosing chain at $6A40, which     */
/* calls it once per candidate direction and branches on the answer. Two      */
/* separate rejections:                                                       */
/*                                                                            */
/*   the target cell itself must be empty or an apple ($0F). Anything else is */
/*   wall or snake, and it returns immediately.                               */
/*                                                                            */
/*   at least one of the target's four orthogonal neighbours must be empty.   */
/*   $6C4A counts them, and a target with none is a dead end -- legal to      */
/*   enter and fatal on the next move, so it is refused a step early.         */
/*                                                                            */
/* Returns A = 0 with Z set when the move is good, A = 1 when it is a dead    */
/* end, and simply Z clear on the early rejection. apple2tc drops the A       */
/* entirely -- its whole-program DCE can see that every caller branches on Z  */
/* alone -- but the 6502 sets it, so it is set here.                          */
/*                                                                            */
/* $6637/$6638 hold the target column and row: $6232[dir] + $624F and         */
/* $6237[dir] + $6250, the direction deltas added to the snake's head.        */
/* ========================================================================== */

void game_move_ok(uint16_t ret_addr) {
  // Adapter for snake_move_verdict(). Costs 21 trace sites; $6AB8 itself
  // survives, charged zero here and for real inside.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$6AB8*/ CYCLES(0x6ab8, 0);
  if (s_status_d) {
    fprintf(stderr, "game_move_ok: entered with decimal mode set\n");
    error_handler(0x6ab8);
    abort();
  }

  uint8_t cell = 0;
  const MoveVerdict v = snake_move_verdict(ram_peek(0x6b38), &cell);

  // Turn the verdict back into what the callers at $6A40 branch on.
  switch (v) {
  case MOVE_TARGET_TAKEN:
    // $6AD9 CMP #$0F left these.
    s_a = cell;
    s_status_not_z = (cell != 0x0f);
    s_status_c = (cell >= 0x0f);
    s_status_n = ((uint8_t)(cell - 0x0f) & 0x80);
    break;
  case MOVE_ROW_ZERO:
  case MOVE_OK:
    s_a = 0x00;
    s_status_not_z = 0x00;
    s_status_n = 0x00;
    break;
  case MOVE_DEAD_END:
    s_a = 0x01;
    s_status_not_z = 0x01;
    s_status_n = 0x00;
    break;
  }

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $64C8 -- move the bouncer one step.                                        */
/*                                                                            */
/* $6633/$6634 are its column and row, $6635/$6636 its per-axis deltas, and   */
/* $6637/$6638 the candidate cell it is trying to move to. A row of 0 means   */
/* it is not on the board, and both the entry test and the exit test use      */
/* that.                                                                      */
/*                                                                            */
/* The bounce is the neat part. A delta is +1 or -1, and `EOR #$FE` swaps     */
/* them: $01 ^ $FE = $FF, $FF ^ $FE = $01. So reflecting off a wall is one    */
/* instruction with no compare.                                               */
/*                                                                            */
/* Three cases when the diagonal target is occupied, distinguished by SCRNing */
/* the two cells either side of it:                                           */
/*                                                                            */
/*   the horizontal neighbour is blocked -- keep the column, flip $6635       */
/*   the vertical neighbour is blocked   -- keep the row, flip $6636          */
/*   neither is blocked, yet the diagonal is -- an inside corner. $6C4A is    */
/*   still zero, so $6530 undoes both axes and flips both deltas, sending it  */
/*   back the way it came.                                                    */
/* ========================================================================== */

void game_move_bouncer(uint16_t ret_addr) {
  // Adapter. The body is bouncer_step() in game_native.c.
  //
  // Cost: the trace gives up every block head in here except $64C8's, which
  // stays below. The cycles do not move -- bouncer_step charges each block
  // with GAME_CYCLES -- so the frame hashes and the memory samples are
  // unaffected.
  //
  // $6633-$6636 is the original's parameter block, so the marshalling here is
  // literally a load and a store of the struct its caller already copies in
  // and out by hand.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$64C8*/ CYCLES(0x64c8, 12);
  if (s_status_d) {
    fprintf(stderr, "game_move_bouncer: entered with decimal mode set\n");
    error_handler(0x64c8);
    abort();
  }

  Bouncer b = {
      .col = ram_peek(0x6633),
      .row = ram_peek(0x6634),
      .dx = (int8_t)ram_peek(0x6635),
      .dy = (int8_t)ram_peek(0x6636),
  };

  // The state the original leaves behind: A holds the row it loaded first, and
  // the flags come from that load.
  s_a = b.row;
  s_status_not_z = b.row;
  s_status_n = (b.row & 0x80);

  bouncer_step(&b);

  ram_poke(0x6633, b.col);
  ram_poke(0x6634, b.row);
  ram_poke(0x6635, (uint8_t)b.dx);
  ram_poke(0x6636, (uint8_t)b.dy);

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $728D, $6BFB                                                               */
/* ========================================================================== */

/// One `LDA score / CMP best` pair of $728D. Returns false when the score
/// byte is below the high-score byte. No CYCLES: all four call sites sit
/// inside a counted block already.
static bool game_hi_cmp(uint16_t score, uint16_t best) {
  const uint8_t mine = ram_peek(score);
  const uint8_t high = ram_peek(best);
  s_a = mine;
  s_status_not_z = (mine != high);
  s_status_c = (mine >= high);
  s_status_n = ((uint8_t)(mine - high) & 0x80);
  return mine >= high;
}

void game_update_high_score(uint16_t ret_addr) {
  // Adapter for game_promote_high_score(). Costs 9 trace sites.
  //
  // $728D itself survives as a probe site: CYCLES(addr, 0) below still sets
  // s_pc and dispatches, while the block's real 10 cycles are charged inside
  // game_promote_high_score by GAME_CYCLES. So a converted routine can keep
  // its entry site for nothing, which is worth doing every time.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$728D*/ CYCLES(0x728d, 0);
  game_promote_high_score();

  // What the original leaves in A and the flags depends on where it stopped:
  // the last byte it compared, or the top score byte if it copied. Both are
  // reproduced from memory rather than threaded out of the C.
  const uint8_t top = ram_peek(0x7255);
  s_a = top;
  s_status_not_z = top;
  s_status_n = (top & 0x80);

  if (ret_addr)
    pop16();
}

void game_tick_sound(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // Twenty passes of a falling tone. $6C46 is the period and $6C47 the
  // countdown to the next click; each click lengthens the period by two, so
  // the pitch drops, and the period wraps back to zero once it passes $80.
  // $6C46 is raised to 1 by game_mark_head and cleared by game_draw_playfield,
  // so the tone plays while the head is moving and stops when it is not.
  /*$6BFB*/ CYCLES(0x6bfb, 6);
  ram_poke(0x6c48, 0x14);

  for (;;) {
    /*$6C00*/ CYCLES(0x6c00, 6);
    const uint8_t period = ram_peek(0x6c46);
    s_a = period;
    if (period) {
      /*$6C05*/ CYCLES(0x6c05, 4);
      if (period < 0x80) {
        /*$6C09*/ CYCLES(0x6c09, 8);
        const uint8_t left = (uint8_t)(ram_peek(0x6c47) - 0x01);
        ram_poke(0x6c47, left);
        if (!left) {
          /*$6C0E*/ CYCLES(0x6c0e, 28);
          const uint8_t port = ram_peek(0x6c49);
          s_y = port;
          peek((uint16_t)(0xc000 + port)); // the click
          ram_poke(0x6c46, (uint8_t)(ram_peek(0x6c46) + 0x01));
          ram_poke(0x6c46, (uint8_t)(ram_peek(0x6c46) + 0x01));
          ram_poke(0x6c47, ram_peek(0x6c46));
        } else {
          /*$6C0C*/ CYCLES_EDGE(0x6c0c, 1);
        }
      } else {
        /*$6C07*/ CYCLES_EDGE(0x6c07, 1);
      }
    } else {
      /*$6C03*/ CYCLES_EDGE(0x6c03, 1);
    }

    /*$6C20*/ CYCLES(0x6c20, 8);
    s_status_c = (ram_peek(0x6c46) >= 0x80);
    if (s_status_c) {
      /*$6C27*/ CYCLES(0x6c27, 6);
      ram_poke(0x6c46, 0x00);
    } else {
      /*$6C25*/ CYCLES_EDGE(0x6c25, 1);
    }

    // Route the click: cassette by default, speaker only when both $0302 and
    // $69C2 are clear. See the $7642 header for why this is done with an
    // address rather than a branch.
    /*$6C2C*/ CYCLES(0x6c2c, 12);
    ram_poke(0x6c49, 0x20);
    const uint8_t muted = ram_peek(0x0302);
    s_a = muted;
    if (!muted) {
      /*$6C36*/ CYCLES(0x6c36, 6);
      const uint8_t busy = ram_peek(0x69c2);
      s_a = busy;
      if (!busy) {
        /*$6C3B*/ CYCLES(0x6c3b, 6);
        s_a = 0x30;
        ram_poke(0x6c49, 0x30);
      } else {
        /*$6C39*/ CYCLES_EDGE(0x6c39, 1);
      }
    } else {
      /*$6C34*/ CYCLES_EDGE(0x6c34, 1);
    }

    /*$6C40*/ CYCLES(0x6c40, 8);
    const uint8_t n = (uint8_t)(ram_peek(0x6c48) - 0x01);
    s_status_not_z = n;
    s_status_n = (n & 0x80);
    ram_poke(0x6c48, n);
    if (!n)
      break;
    /*$6C43*/ CYCLES_EDGE(0x6c43, 1);
  }

  /*$6C45*/ CYCLES(0x6c45, 6);

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $6594, $69C3                                                               */
/* ========================================================================== */

void game_step_bouncers(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // Difficulty ($0301) decides how many bouncers exist: none at 0, one at 1,
  // two above. That is the same count as the wall gaps game_draw_playfield
  // opens, one per bouncer.
  //
  // Each bouncer's state is swapped into the working locations $6633-$6636
  // that game_move_bouncer operates on, and swapped back out afterwards.
  // Bouncer 1 lives at $6639/$663A/$663D/$663E, bouncer 2 at
  // $663B/$663C/$663F/$6640.
  //
  // Every path then falls into the key dequeue at $6200, whose byte is this
  // routine's return value in A.
  /*$6594*/ CYCLES(0x6594, 6);
  if (ram_peek(0x0301)) {
    /*$6597*/ CYCLES_EDGE(0x6597, 1);
    /*$659C*/ CYCLES(0x659c, 38);
    ram_poke(0x6633, ram_peek(0x6639));
    ram_poke(0x6634, ram_peek(0x663a));
    ram_poke(0x6635, ram_peek(0x663d));
    ram_poke(0x6636, ram_peek(0x663e));
    game_move_bouncer(0x65b6);

    /*$65B7*/ CYCLES(0x65b7, 40);
    ram_poke(0x6639, ram_peek(0x6633));
    ram_poke(0x663a, ram_peek(0x6634));
    ram_poke(0x663d, ram_peek(0x6635));
    ram_poke(0x663e, ram_peek(0x6636));
    const uint8_t diff = ram_peek(0x0301);
    s_status_c = (diff >= 0x01);
    if (diff != 0x01) {
      /*$65D4*/ CYCLES_EDGE(0x65d4, 1);
      /*$65D9*/ CYCLES(0x65d9, 38);
      ram_poke(0x6633, ram_peek(0x663b));
      ram_poke(0x6634, ram_peek(0x663c));
      ram_poke(0x6635, ram_peek(0x663f));
      ram_poke(0x6636, ram_peek(0x6640));
      game_move_bouncer(0x65f3);

      /*$65F4*/ CYCLES(0x65f4, 35);
      ram_poke(0x663b, ram_peek(0x6633));
      ram_poke(0x663c, ram_peek(0x6634));
      ram_poke(0x663f, ram_peek(0x6635));
      ram_poke(0x6640, ram_peek(0x6636));
    } else {
      /*$65D6*/ CYCLES(0x65d6, 3);
    }
  } else {
    /*$6599*/ CYCLES(0x6599, 3);
  }

  // $6200: take the next key out of the ring buffer game_read_key fills.
  // Only a byte with bit 7 set counts as a key; the slot is cleared and the
  // read index advances. Shares the RTS at $6216 with game_read_key.
  /*$6200*/ CYCLES(0x6200, 10);
  const uint8_t r = ram_peek(0x624c);
  s_x = r;
  const uint8_t key = ram_peek(0x623c + r);
  s_a = key;
  if (key & 0x80) {
    /*$6208*/ CYCLES(0x6208, 24);
    push8(key);
    ram_poke(0x623c + r, 0x00);
    s_x = (uint8_t)(r + 0x01);
    ram_poke(0x624c, (uint8_t)(s_x & 0x0f));
    s_a = pop8();
  } else {
    /*$6206*/ CYCLES_EDGE(0x6206, 1);
  }

  /*$6216*/ CYCLES(0x6216, 6);

  if (ret_addr)
    pop16();
}

void game_find_apple(uint16_t ret_addr) {
  // Adapter for game_find_nearest_apple(). Costs 12 trace sites; $69C3 itself
  // survives, charged zero here and for real inside -- see
  // game_update_high_score above for why that works.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$69C3*/ CYCLES(0x69c3, 0);
  game_find_nearest_apple();

  const uint8_t row = ram_peek(0x6b3a);
  s_a = row;
  s_status_not_z = row;
  s_status_n = (row & 0x80);

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $69A9, $75D1, $6C72 -- the rest of the input path.                         */
/*                                                                            */
/* These three carry most of what neither recording exercises: ESC and        */
/* Ctrl-S, the arrow keys on the redefinition screen, and the whole joystick  */
/* branch. See probe-acceptance.sh's baseline list -- the decodes here are    */
/* from the binary alone, with no cross-engine check behind them.             */
/* ========================================================================== */

void game_pause_or_toggle_sound(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // ESC pauses by spinning on the keyboard until anything is pressed; that
  // key then falls through to the Ctrl-S test, so ESC followed by Ctrl-S both
  // unpauses and toggles the sound. $69C2 is the flag $6C2C reads when
  // deciding whether to point the click at the speaker.
  /*$69A9*/ CYCLES(0x69a9, 4);
  if (s_a == 0x9b) { // ESC
    for (;;) {
      /*$69AD*/ CYCLES(0x69ad, 6);
      s_a = io_peek(0xc000);
      if (s_a & 0x80)
        break;
      /*$69B0*/ CYCLES_EDGE(0x69b0, 1);
    }
    /*$69B2*/ CYCLES(0x69b2, 4);
    io_poke(0xc010, s_a);
  } else {
    /*$69AB*/ CYCLES_EDGE(0x69ab, 1);
  }

  /*$69B5*/ CYCLES(0x69b5, 4);
  const uint8_t k = s_a;
  s_status_not_z = (k != 0x93);
  s_status_c = (k >= 0x93);
  s_status_n = ((uint8_t)(k - 0x93) & 0x80);
  if (k == 0x93) { // Ctrl-S
    /*$69B9*/ CYCLES(0x69b9, 10);
    const uint8_t f = (uint8_t)(ram_peek(0x69c2) ^ 0x01);
    s_status_not_z = f;
    s_status_n = (f & 0x80);
    ram_poke(0x69c2, f);
  } else {
    /*$69B7*/ CYCLES_EDGE(0x69b7, 1);
  }

  /*$69C1*/ CYCLES(0x69c1, 6);

  if (ret_addr)
    pop16();
}

void game_edit_key(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // Blink slot X of the key-redefinition screen and wait for a replacement.
  // The blink is two halves: print a space, spin, print the current key from
  // $75CB, spin again while polling the keyboard. Only the second half reads
  // input, so a key pressed during the first half waits for the second.
  //
  // Accepted: anything from $A1 up, plus the two arrow keys. $75BF/$75C5 are
  // the cursor position tables, indexed by slot.
  for (;;) {
    /*$75D1*/ CYCLES(0x75d1, 23);
    const uint8_t slot = s_x;
    ram_poke(0x0002, slot);
    ram_poke(0x0024, ram_peek(0x75bf + slot));
    ram_poke(0x0025, ram_peek(0x75c5 + slot));
    rom_fc68(0x75df);

    /*$75E0*/ CYCLES(0x75e0, 11);
    s_x = ram_peek(0x0002);
    s_a = 0xa0; // space
    s_status_not_z = 0xa0;
    s_status_n = 0x80;
    rom_cout(0x75e6);

    /*$75E7*/ CYCLES(0x75e7, 2);
    s_y = 0x00;

    for (;;) { // blank half: X inner, Y outer, no polling
      /*$75E9*/ CYCLES(0x75e9, 4);
      s_x = (uint8_t)(s_x - 0x01);
      if (s_x) {
        /*$75EA*/ CYCLES_EDGE(0x75ea, 1);
        continue;
      }
      /*$75EC*/ CYCLES(0x75ec, 4);
      /*$75F0*/ CYCLES(0x75f0, 4);
      s_y = (uint8_t)(s_y - 0x01);
      if (!s_y)
        break;
      /*$75F1*/ CYCLES_EDGE(0x75f1, 1);
    }

    /*$75F3*/ CYCLES(0x75f3, 23);
    const uint8_t s2 = ram_peek(0x0002);
    s_x = s2;
    ram_poke(0x0024, ram_peek(0x75bf + s2));
    ram_poke(0x0025, ram_peek(0x75c5 + s2));
    rom_fc68(0x7601);

    /*$7602*/ CYCLES(0x7602, 13);
    const uint8_t s3 = ram_peek(0x0002);
    s_x = s3;
    s_a = ram_peek(0x75cb + s3);
    s_status_not_z = s_a;
    s_status_n = (s_a & 0x80);
    rom_cout(0x7609);

    /*$760A*/ CYCLES(0x760a, 2);
    s_y = 0x00;

    bool accepted = false, restart = false;
    for (;;) { // visible half, polling as it goes
      /*$760C*/ CYCLES(0x760c, 4);
      s_x = (uint8_t)(s_x - 0x01);
      if (s_x) {
        /*$760D*/ CYCLES_EDGE(0x760d, 1);
        continue;
      }

      /*$760F*/ CYCLES(0x760f, 6);
      s_a = io_peek(0xc000);
      if (s_a & 0x80) {
        /*$7612*/ CYCLES_EDGE(0x7612, 1);
        /*$761C*/ CYCLES(0x761c, 8);
        const uint8_t key = s_a;
        io_poke(0xc010, key);
        s_status_c = (key >= 0xa1);
        if (key >= 0xa1) {
          /*$7621*/ CYCLES_EDGE(0x7621, 1);
          accepted = true;
          break;
        }
        /*$7623*/ CYCLES(0x7623, 4);
        s_status_c = (key >= 0x88);
        if (key == 0x88) { // left arrow
          /*$7625*/ CYCLES_EDGE(0x7625, 1);
          accepted = true;
          break;
        }
        /*$7627*/ CYCLES(0x7627, 4);
        s_status_c = (key >= 0x95);
        if (key == 0x95) { // right arrow
          /*$7629*/ CYCLES_EDGE(0x7629, 1);
          accepted = true;
          break;
        }
        /*$762B*/ CYCLES(0x762b, 6);
        s_x = ram_peek(0x0002);
        restart = true;
        break;
      }

      /*$7614*/ CYCLES(0x7614, 4);
      s_y = (uint8_t)(s_y - 0x01);
      if (s_y) {
        /*$7615*/ CYCLES_EDGE(0x7615, 1);
        continue;
      }
      /*$7617*/ CYCLES(0x7617, 6);
      s_x = ram_peek(0x0002);
      restart = true;
      break;
    }

    if (accepted)
      break;
    (void)restart;
  }

  /*$7630*/ CYCLES(0x7630, 9);
  const uint8_t slot = ram_peek(0x0002);
  s_status_not_z = slot;
  s_status_n = (slot & 0x80);
  s_x = slot;

  if (ret_addr)
    pop16();
}

void game_read_direction(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // Turn the next input into a direction. $6C63 holds the six redefinable key
  // codes and $6C6A the direction each maps to. $6C71 selects the joystick,
  // and $0302 the alternate mode in which only the button matters.
  /*$6C72*/ CYCLES(0x6c72, 6);
  game_step_bouncers(0x6c74);

  /*$6C75*/ CYCLES(0x6c75, 9);
  push8(s_a);
  if (ram_peek(0x0302)) {
    /*$6C7B*/ CYCLES(0x6c7b, 6);
    if (ram_peek(0x6c71)) {
      /*$6C80*/ CYCLES(0x6c80, 6);
      if (!(io_peek(0xc061) & 0x80)) {
        /*$6C85*/ CYCLES(0x6c85, 12);
        pop8();
        s_a = 0x92;
        s_status_not_z = 0x92;
        s_status_n = 0x80;
        if (ret_addr)
          pop16();
        return;
      }
      /*$6C83*/ CYCLES_EDGE(0x6c83, 1);
    } else {
      /*$6C7E*/ CYCLES_EDGE(0x6c7e, 1);
    }

    /*$6C89*/ CYCLES(0x6c89, 6);
    const uint8_t k = pop8();
    s_a = k;
    s_status_not_z = k;
    s_status_n = (k & 0x80);
    if (k & 0x80) {
      /*$6C8C*/ CYCLES(0x6c8c, 8);
      s_a = 0x92;
      s_status_not_z = 0x92;
      s_status_n = 0x80;
    } else {
      /*$6C8A*/ CYCLES_EDGE(0x6c8a, 1);
      /*$6C8F*/ CYCLES(0x6c8f, 6);
    }
    if (ret_addr)
      pop16();
    return;
  }
  /*$6C79*/ CYCLES_EDGE(0x6c79, 1);

  /*$6C90*/ CYCLES(0x6c90, 6);
  s_a = pop8();
  s_x = 0x05;

  // Walk the six key codes downward; a match substitutes its direction.
  for (;;) {
    /*$6C93*/ CYCLES(0x6c93, 6);
    if (s_a == ram_peek(0x6c63 + s_x)) {
      /*$6C96*/ CYCLES_EDGE(0x6c96, 1);
      /*$6C9E*/ CYCLES(0x6c9e, 4);
      s_a = ram_peek(0x6c6a + s_x);
      break;
    }
    /*$6C98*/ CYCLES(0x6c98, 4);
    s_x = (uint8_t)(s_x - 0x01);
    if (s_x & 0x80) {
      /*$6C9B*/ CYCLES(0x6c9b, 3);
      break;
    }
    /*$6C99*/ CYCLES_EDGE(0x6c99, 1);
  }

  /*$6CA1*/ CYCLES(0x6ca1, 4);
  if (s_a == 0x80) {
    /*$6CA5*/ CYCLES(0x6ca5, 12);
    s_a = 0x01;
    s_status_not_z = 0x01;
    s_status_n = 0x00;
    ram_poke(0x6c71, 0x01); // joystick on
    if (ret_addr)
      pop16();
    return;
  }
  /*$6CA3*/ CYCLES_EDGE(0x6ca3, 1);

  /*$6CAB*/ CYCLES(0x6cab, 4);
  if (s_a == 0x8b) {
    /*$6CAF*/ CYCLES(0x6caf, 12);
    s_a = 0x00;
    s_status_not_z = 0x00;
    s_status_n = 0x00;
    ram_poke(0x6c71, 0x00); // joystick off
    if (ret_addr)
      pop16();
    return;
  }
  /*$6CAD*/ CYCLES_EDGE(0x6cad, 1);

  /*$6CB5*/ CYCLES(0x6cb5, 4);
  {
    const uint8_t a = s_a;
    s_status_not_z = (a != 0x00);
    s_status_n = (a & 0x80);
    if (a & 0x80) {
      // A real key: hand it straight back.
      /*$6CB9*/ CYCLES(0x6cb9, 6);
      if (ret_addr)
        pop16();
      return;
    }
  }
  /*$6CB7*/ CYCLES_EDGE(0x6cb7, 1);

  /*$6CBA*/ CYCLES(0x6cba, 6);
  s_x = ram_peek(0x6c71);
  if (!s_x) {
    /*$6CBF*/ CYCLES(0x6cbf, 8);
    s_status_not_z = (s_a != 0x00);
    s_status_n = (s_a & 0x80);
    if (ret_addr)
      pop16();
    return;
  }
  /*$6CBD*/ CYCLES_EDGE(0x6cbd, 1);

  // The joystick. Two annunciator settings times two button inputs give four
  // directions; Y counts how many are active, and anything but exactly one is
  // rejected as ambiguous.
  /*$6CC2*/ CYCLES(0x6cc2, 12);
  s_y = 0x00;
  io_peek(0xc05b);
  if (!(io_peek(0xc062) & 0x80)) {
    /*$6CCC*/ CYCLES(0x6ccc, 4);
    s_x = 0x00;
    s_y = (uint8_t)(s_y + 0x01);
  } else {
    /*$6CCA*/ CYCLES_EDGE(0x6cca, 1);
  }

  /*$6CCF*/ CYCLES(0x6ccf, 6);
  if (!(io_peek(0xc063) & 0x80)) {
    /*$6CD4*/ CYCLES(0x6cd4, 4);
    s_x = 0x03;
    s_y = (uint8_t)(s_y + 0x01);
  } else {
    /*$6CD2*/ CYCLES_EDGE(0x6cd2, 1);
  }

  /*$6CD7*/ CYCLES(0x6cd7, 10);
  io_peek(0xc05a);
  if (!(io_peek(0xc062) & 0x80)) {
    /*$6CDF*/ CYCLES(0x6cdf, 4);
    s_x = 0x01;
    s_y = (uint8_t)(s_y + 0x01);
  } else {
    /*$6CDD*/ CYCLES_EDGE(0x6cdd, 1);
  }

  /*$6CE2*/ CYCLES(0x6ce2, 6);
  if (!(io_peek(0xc063) & 0x80)) {
    /*$6CE7*/ CYCLES(0x6ce7, 4);
    s_x = 0x02;
    s_y = (uint8_t)(s_y + 0x01);
  } else {
    /*$6CE5*/ CYCLES_EDGE(0x6ce5, 1);
  }

  /*$6CEA*/ CYCLES(0x6cea, 4);
  if (s_y == 0x01) {
    /*$6CEC*/ CYCLES_EDGE(0x6cec, 1);
    /*$6CF1*/ CYCLES(0x6cf1, 10);
    s_a = ram_peek(0x6c6a + s_x);
    s_status_not_z = s_a;
    s_status_n = (s_a & 0x80);
  } else {
    /*$6CEE*/ CYCLES(0x6cee, 8);
    s_a = 0x00;
    s_status_not_z = 0x00;
    s_status_n = 0x00;
  }

  if (ret_addr)
    pop16();
}
