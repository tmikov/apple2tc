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

/// $7230 -- print the NUL-terminated string that follows the call.
///
/// The original takes its argument by popping its own return address, walking
/// the bytes after the JSR, and then pushing the address of the terminator so
/// that its RTS lands past them. Declaring it in inline-str.txt is what lets
/// apple2tc point each call site's fall-through past the string instead; the
/// call still passes the *original* return address, because ExternRoutines
/// derives that from the call site and not from the fall-through. So \p
/// ret_addr is the same pointer the original popped, and this reads its
/// argument from the same place for the same reason.
///
/// Nothing is pushed or popped. The original's PLA/PLA and PHA/PHA cancel, so
/// the emulated stack pointer ends where the caller left it either way, and
/// ram.probe compares only the live stack.
///
/// $0C/$0D are still written at every step. They are scratch, but they are in
/// the range ram.probe hashes, so the residue has to be the residue the
/// original left.
void game_print_inline_str(uint16_t ret_addr) {
  bool branchTarget = true;

  /*$7230*/ CYCLES(0x7230, 20);
  ram_poke(0x000c, (uint8_t)ret_addr);
  ram_poke(0x000d, (uint8_t)(ret_addr >> 8));
  rom_fc68(0x7239); // VTAB to the current CV

  for (;;) {
    // The pointer is stepped before the read, which is why the caller passes
    // the address of the JSR's last byte rather than of the string.
    /*$7239*/ CYCLES(0x7239, 7);
    const uint8_t lo = (uint8_t)(ram_peek(0x000c) + 1);
    ram_poke(0x000c, lo);
    if (!lo) {
      /*$723D*/ CYCLES(0x723d, 5);
      ram_poke(0x000d, (uint8_t)(ram_peek(0x000d) + 1));
    } else {
      /*$723B*/ CYCLES_EDGE(0x723b, 1);
    }

    /*$723F*/ CYCLES(0x723f, 9);
    const uint8_t ch = peek(ram_peek16al(0x000c));
    s_y = 0x00;
    s_a = ch;
    s_status_not_z = ch;
    s_status_n = (ch & 0x80);
    if (!ch) {
      /*$7243*/ CYCLES_EDGE(0x7243, 1);
      break;
    }

    /*$7245*/ CYCLES(0x7245, 6);
    rom_cout(0x7247);
    /*$7248*/ CYCLES(0x7248, 3);
  }

  /*$724B*/ CYCLES(0x724b, 18);
}

void game_cout_hook(uint16_t ret_addr) {
  // Adapter for game_cout_hook_native(). Costs 4 trace sites; $664A survives,
  // probed here with no cycles and charged for real inside.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$664A*/ CYCLES(0x664a, 0);
  game_cout_hook_native(s_a);

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
  // Adapter for game_draw_cell_native(). Costs 3 trace sites; $60E7 survives.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$60E7*/ CYCLES(0x60e7, 0);
  if (s_status_d) {
    fprintf(stderr, "game_draw_cell: entered with decimal mode set\n");
    error_handler(0x60e7);
    abort();
  }

  const Cell c = {.col = ram_peek(0x0002), .row = ram_peek(0x0003)};
  game_draw_cell_native(ram_peek(0x0001), c);

  // What the loop leaves: X counted to 4, Y is the column, A the last
  // destination high byte, and the flags come from CPX #4.
  s_x = 0x04;
  s_y = c.col;
  s_a = ram_peek(0x0005);
  s_status_c = 0x01;
  s_status_not_z = 0x00;
  s_status_n = 0x00;
  s_status_v = ovf8(s_a, (uint8_t)(s_a - 0x04), 0x04);

  if (ret_addr)
    pop16();
}

void game_plot_shape(uint16_t ret_addr) {
  // Adapter for game_plot_shape_native(). Costs 1 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  game_plot_shape_native();

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
  // Adapter for game_plot_hline_native(). The shape load stays here: it is the
  // routine's own first block and keeps its probe site.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$6148*/ CYCLES(0x6148, 6);
  game_load_shape(0x614a);
  game_plot_hline_native();

  // The CMP that ended the loop, and the coordinate it compared.
  const uint8_t at = ram_peek(0x0002);
  s_a = at;
  s_status_c = 0x01;
  s_status_not_z = 0x00;
  s_status_n = 0x00;

  if (ret_addr)
    pop16();
}

void game_plot_vline(uint16_t ret_addr) {
  // Adapter for game_plot_vline_native(). The shape load stays here: it is the
  // routine's own first block and keeps its probe site.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$615A*/ CYCLES(0x615a, 6);
  game_load_shape(0x615c);
  game_plot_vline_native();

  // The CMP that ended the loop, and the coordinate it compared.
  const uint8_t at = ram_peek(0x0003);
  s_a = at;
  s_status_c = 0x01;
  s_status_not_z = 0x00;
  s_status_n = 0x00;

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
  // Adapter for game_next_byte_native(). Costs 3 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  game_next_byte_native();

  if (ret_addr)
    pop16();
}

void game_set_ink(uint16_t ret_addr) {
  // Adapter for game_set_ink_native(). Costs 3 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // The original branches on Z and the native takes the byte. That is the same
  // question only because every caller sets both with one `LDA`, which is an
  // assumption worth stating out loud rather than relying on quietly.
  assert(!s_a == !s_status_not_z);
  game_set_ink_native(s_a);

  if (ret_addr)
    pop16();
}

void game_lores_vline(uint16_t ret_addr) {
  // Adapter for game_lores_vline_native(). Costs 4 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$7000*/ CYCLES(0x7000, 6);
  game_lores_vline_native();

  const uint8_t restored = ram_peek(0x0003);
  s_a = restored;
  s_status_not_z = restored;
  s_status_n = (restored & 0x80);

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

void game_print_bcd(uint16_t ret_addr) {
  // Adapter for game_print_bcd_native(). Costs 11 trace sites. No flags are
  // put back: every caller's next act is an `LDA` or a store, so `apple2tc
  // --ir` has nothing live here.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  game_print_bcd_native(s_a);

  if (ret_addr)
    pop16();
}

void game_print_zero_if_blank(uint16_t ret_addr) {
  // Adapter for game_print_zero_if_blank_native(). Costs 3 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  game_print_zero_if_blank_native();

  if (ret_addr)
    pop16();
}

void game_add_score(uint16_t ret_addr) {
  // Adapter for game_add_score_native(). Costs 1 trace site.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  game_add_score_native();

  if (ret_addr)
    pop16();
}

void game_clear_hgr(uint16_t ret_addr) {
  // Adapter for game_clear_hgr_native(). Costs 3 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$702B*/ CYCLES(0x702b, 0);
  game_clear_hgr_native();

  s_status_c = 0x01;
  s_status_not_z = 0x00;
  s_status_n = 0x00;

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
  // Adapter for game_rand_byte_native(). Costs 5 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  s_a = game_rand_byte_native();

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
  // Adapter for game_merge_cell_native(), plus the shape load it opens with.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$6B93*/ CYCLES(0x6b93, 6);
  game_load_shape(0x6b95);

  if (s_status_d) {
    fprintf(stderr, "game_plot_shape_merge: entered with decimal mode set\n");
    error_handler(0x6b93);
    abort();
  }

  const Cell c = {.col = ram_peek(0x0002), .row = ram_peek(0x0003)};
  s_y = c.row; // LDY $03 at $6B98, before the loop overwrites it
  game_merge_cell_native(ram_peek(0x0001), c);

  s_x = 0x04;
  s_y = c.col;
  s_a = ram_peek(0x0005);
  s_status_c = 0x01;
  s_status_not_z = 0x00;
  s_status_n = 0x00;
  s_status_v = ovf8(s_a, (uint8_t)(s_a - 0x04), 0x04);

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
  // Adapter for game_mark_head_native(). Costs 2 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  game_mark_head_native();

  if (ret_addr)
    pop16();
}

void game_draw_head(uint16_t ret_addr) {
  // Adapter for game_draw_head_native(). Costs 4 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  game_draw_head_native();

  if (ret_addr)
    pop16();
}

void game_set_apple_value(uint16_t ret_addr) {
  // Adapter for game_set_apple_value_native(). Costs 3 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  game_set_apple_value_native();

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
  // Adapter for game_place_apple_native(). Costs 8 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  game_place_apple_native();

  if (ret_addr)
    pop16();
}

void game_sound_sweep(uint16_t ret_addr) {
  // Adapter for game_sound_sweep_native(). Costs 8 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  game_sound_sweep_native();

  if (ret_addr)
    pop16();
}

void game_eat_apple(uint16_t ret_addr) {
  // Adapter for game_eat_apple_native(). Costs 2 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  game_eat_apple_native();

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $6217, $7590, $6B3D                                                        */
/* ========================================================================== */

void game_read_key(uint16_t ret_addr) {
  // Adapter for game_read_key_native(). Costs 2 trace sites: $6217 keeps its
  // probe because the replay coordinate counts it, and $6216 because the key
  // dequeue's adapter still emits it.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  game_read_key_native();

  if (ret_addr)
    pop16();
}

void game_show_key(uint16_t ret_addr) {
  // Adapter for game_show_key_native(). Costs 7 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  game_show_key_native(s_x, s_a);

  if (ret_addr)
    pop16();
}

void game_draw_side_walls(uint16_t ret_addr) {
  // Adapter for game_draw_side_walls_native(). Costs 8 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  game_draw_side_walls_native();

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
  // Adapter for game_tick_sound_native(). Costs 12 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  game_tick_sound_native();

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $6594, $69C3                                                               */
/* ========================================================================== */

void game_step_bouncers(uint16_t ret_addr) {
  // Adapter for game_step_bouncers_native(). Costs 9 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$6594*/ CYCLES(0x6594, 0);
  const uint8_t key = game_step_bouncers_native();
  // $6216 is shared with game_read_key, which is not converted, so it is
  // still a probe site and has to stay one on this path too.
  /*$6216*/ CYCLES(0x6216, 6);
  s_a = key;
  s_status_not_z = key;
  s_status_n = (key & 0x80);

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
  // Adapter for game_edit_key_native(). Costs 17 trace sites -- one short of
  // the 18 blocks, because $760F stays a probe: it is one of the seven
  // addresses the replay coordinate counts (see GAME_CYCLES_COORD).
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  const uint8_t slot = s_x;
  s_a = game_edit_key_native(slot);
  s_x = slot;
  s_status_not_z = slot;
  s_status_n = (slot & 0x80);

  if (ret_addr)
    pop16();
}

void game_read_direction(uint16_t ret_addr) {
  // Adapter for game_read_direction_native(). Costs 31 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // The JSR stays here rather than moving into the native routine, because
  // game_step_bouncers's own adapter is what keeps $6216 -- the RTS it shares
  // with the unconverted game_read_key -- a probe site.
  /*$6C72*/ CYCLES(0x6c72, 6);
  game_step_bouncers(0x6c74);

  // The original saves the key on the stack across the $0302 test; here it is
  // an argument. The pushed byte is never observed: nothing between the PHA
  // and the PLA samples memory, and ram.probe compares only the live stack.
  const uint8_t code = game_read_direction_native(s_a);
  s_a = code;
  s_status_not_z = code;
  s_status_n = (code & 0x80);

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $6288 -- one life. See game_native.c for what it does.                     */
/* ========================================================================== */

void game_play_loop(uint16_t ret_addr) {
  // Adapter for game_play_loop_native(). Costs 72 trace sites -- the whole of
  // the main loop's block structure, which is the largest single trade made so
  // far and the reason this routine waited until every block head in it was
  // covered by a recording.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // The native side does binary arithmetic only. The original's callers clear
  // decimal mode before getting here and set it afterwards, around the score.
  if (s_status_d) {
    fprintf(stderr, "game_play_loop: entered with decimal mode set\n");
    error_handler(0x6288);
    abort();
  }

  uint8_t cell = 0;
  const LifeEnd end = game_play_loop_native(&cell);

  // $6253 is the interface, not residue: the caller at $7739 loads it the
  // instant this returns and compares it against $0F. Written explicitly for
  // all five endings, though three of them already hold the right byte from
  // $62EE, because a reader should not have to know that to check it.
  switch (end) {
  case LIFE_GATE:
    ram_poke(0x6253, 0x00);
    break;
  case LIFE_APPLE:
    ram_poke(0x6253, 0x0f);
    break;
  case LIFE_QUIT:
    ram_poke(0x6253, 0xff);
    break;
  case LIFE_TIMEOUT:
    ram_poke(0x6253, 0xfe);
    break;
  case LIFE_CRASH:
    ram_poke(0x6253, cell);
    break;
  }

  // A is dead at both call sites -- $7716 and $7739 both load $6253 straight
  // away -- but the original leaves the reason there on most paths, so this
  // does too rather than leaving something arbitrary.
  const uint8_t reason = ram_peek(0x6253);
  s_a = reason;
  s_status_not_z = reason;
  s_status_n = (uint8_t)(reason & 0x80);

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $72CE -- the status panel. See game_native.c.                              */
/* ========================================================================== */

void game_draw_status(uint16_t ret_addr) {
  // Adapter for game_status_panel(). Costs 30 trace sites; the routine is
  // straight-line, so all of them are its own and none is shared.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  game_status_panel();

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $78B3 -- the bonus screen. See game_native.c.                              */
/* ========================================================================== */

void game_bonus(uint16_t ret_addr) {
  // Adapter for game_bonus_screen(). Costs 22 trace sites.
  //
  // No decimal-mode assertion here, unlike the other adapters: this routine is
  // *supposed* to be entered with D set, does its BCD arithmetic, and clears it
  // at $78C7.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  game_bonus_screen();

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $6256 -- start a life. See game_native.c.                                  */
/* ========================================================================== */

void game_start_round(uint16_t ret_addr) {
  // Adapter for game_begin_life(). Costs 4 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  if (s_status_d) {
    fprintf(stderr, "game_start_round: entered with decimal mode set\n");
    error_handler(0x6256);
    abort();
  }

  game_begin_life();

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $7980 -- the setup screen. See game_native.c.                              */
/* ========================================================================== */

void game_setup(uint16_t ret_addr) {
  // Adapter for game_setup_screen(). Costs 20 trace sites; the rest of this
  // routine's addresses stay probed, because the inline-string printer leaves
  // everything after its call reachable only through the dynamic block map.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  if (s_status_d) {
    fprintf(stderr, "game_setup: entered with decimal mode set\n");
    error_handler(0x7980);
    abort();
  }

  game_setup_screen();

  if (ret_addr)
    pop16();
}
