/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// Snake Byte as ordinary C. See game_native.h for what this file is for and
/// what it costs to move a routine into it.
///
/// Like a2rom.c and game.c this is textually included into the generated
/// translation unit, and it must come *before* game.c, whose adapters call
/// into it.

#include "game_native.h"

/* ========================================================================== */
/* Storage                                                                    */
/*                                                                            */
/* The two bouncers occupy eight bytes at $6639-$6640, but not as two structs: */
/* the fields are interleaved in pairs -- both columns, both rows, both dx,    */
/* both dy. So a struct cannot simply be overlaid, and load/store it is.       */
/*                                                                            */
/* That is temporary. The addresses are here because generated code still      */
/* reads them; when it stops, these two functions become the only thing that   */
/* has to change.                                                             */
/* ========================================================================== */

static const struct {
  uint16_t col, row, dx, dy;
} kBouncerAddr[2] = {
    {0x6639, 0x663a, 0x663d, 0x663e},
    {0x663b, 0x663c, 0x663f, 0x6640},
};

Bouncer bouncer_load(int i) {
  Bouncer b;
  b.col = ram_peek(kBouncerAddr[i].col);
  b.row = ram_peek(kBouncerAddr[i].row);
  b.dx = (int8_t)ram_peek(kBouncerAddr[i].dx);
  b.dy = (int8_t)ram_peek(kBouncerAddr[i].dy);
  return b;
}

void bouncer_store(int i, Bouncer b) {
  ram_poke(kBouncerAddr[i].col, b.col);
  ram_poke(kBouncerAddr[i].row, b.row);
  ram_poke(kBouncerAddr[i].dx, (uint8_t)b.dx);
  ram_poke(kBouncerAddr[i].dy, (uint8_t)b.dy);
}

/* ========================================================================== */
/* Converted routines                                                         */
/* ========================================================================== */

/// The snake's head column. Still at $624F: the generated dispatch reads it.
static void set_snake_head_col(uint8_t col) {
  ram_poke(0x624f, col);
}

uint8_t game_start_life(uint8_t head_col) {
  set_snake_head_col(head_col);

  // Opposite corners, converging. The original's nine stores are these two.
  const Bouncer a = {.col = 0x01, .row = 0x01, .dx = +1, .dy = +1};
  const Bouncer b = {.col = 0x26, .row = 0x01, .dx = -1, .dy = +1};
  bouncer_store(0, a);
  bouncer_store(1, b);

  // $6630 `LDA #$14`. Its one caller, $6256, stores this as the tail column;
  // it is not related to head_col, which happens to be $14 as well.
  return 0x14;
}

uint8_t game_load_shape_masks(uint8_t shape) {
  // Four masks per shape at $6174, and $6060 is where the plotter reads them.
  // Both stay in emulated RAM: $6060 is read by game_draw_cell, which is not
  // converted, and $6174 is part of the loaded binary image.
  uint8_t last = 0;
  for (unsigned line = 0; line < 4; ++line) {
    last = ram_peek(0x6174 + (uint8_t)((uint8_t)(shape << 2) + line));
    ram_poke(0x6060 + line, last);
  }
  return last;
}

void game_install_cout_vector(void) {
  // CSWL/CSWH at $36/$37, pointed at $664A.
  ram_poke(0x0036, 0x4a);
  ram_poke(0x0037, 0x66);
}

/* ========================================================================== */
/* $64C8 -- the bouncer                                                       */
/*                                                                            */
/* What the original spends bytes on and this does not: $6633-$6636 are a      */
/* parameter block the caller copies in and out, $6637/$6638 are the candidate */
/* cell, and $6C4A counts how many axes were blocked. All five are locals in   */
/* any language with a stack.                                                 */
/*                                                                            */
/* Two of them still have to be written back. ram.probe hashes $6000-$BFFF,    */
/* so the residue the original leaves in $6637/$6638 and $6C4A is compared;    */
/* the values are computed as locals here and mirrored at the end. Those       */
/* writes go away when the memory oracle does, and not before.                */
/*                                                                            */
/* The plotter still takes its arguments in zero page, so the draw calls set   */
/* $00-$03 by hand. Note the erase at $654C deliberately does *not* set $00:   */
/* it reuses whatever shape is already there, and the mask that shape selects  */
/* decides which pixels get cleared. Tidying that away would change the        */
/* screen.                                                                    */
/* ========================================================================== */

/// Reflect a delta.
///
/// The original is `EOR #$FE`, which swaps $01 and $FF -- a bounce with no
/// compare. Written here as negation, which is the same thing over the only
/// values a delta ever holds and says what it means.
///
/// The two differ for anything but +/-1 (EOR would turn 2 into $FC, negation
/// into $FE), so the equivalence rests on the invariant, and the invariant is
/// checkable: $6635/$6636 are written only by this reflection and by the
/// copy-in from $663D-$6640, which in turn are written only by game_start_life
/// -- with +/-1 -- and the matching copy-back. A closed set. Swapping the two
/// spellings also passes every oracle, which is corroboration rather than
/// proof.
static int8_t reflect(int8_t d) {
  assert(d == 1 || d == -1);
  return (int8_t)-d;
}

/// True if the cell at (\p col, \p row) is occupied, asked of the lo-res
/// occupancy map through the ROM's SCRN.
///
/// Charges nothing: the CMP that follows each SCRN is its own block in the
/// original and the cycles differ per call site, so each caller charges its
/// own. Folding them in here is what broke the first attempt -- 4 cycles
/// missing three times over, and every oracle diverged at once.
static bool cell_taken(uint8_t col, uint8_t row, uint16_t ret) {
  s_a = row;
  s_y = col;
  rom_scrn(ret);
  return s_a != 0x00;
}

void bouncer_step(Bouncer *b) {
  unsigned blocked = 0;

  if (b->row == 0) {
    GAME_CYCLES(0x64d2, 6);
    ram_poke(0x6c4a, 0x00);
    return;
  }
  GAME_CYCLES(0x64d0, 1);

  GAME_CYCLES(0x64d3, 36);
  uint8_t want_col = (uint8_t)(b->col + b->dx);
  uint8_t want_row = (uint8_t)(b->row + b->dy);

  const bool diagonal_taken = cell_taken(want_col, want_row, 0x64d4);
  GAME_CYCLES(0x64eb, 4);
  if (diagonal_taken) {
    // Which axis actually stopped it? Ask the two cells either side.
    GAME_CYCLES(0x64ef, 14);
    const bool across_taken = cell_taken(want_col, b->row, 0x64f7);
    GAME_CYCLES(0x64f8, 4);
    if (across_taken) {
      GAME_CYCLES(0x64fc, 24);
      want_col = b->col;
      b->dx = reflect(b->dx);
      ++blocked;
    } else {
      GAME_CYCLES(0x64fa, 1);
    }

    GAME_CYCLES(0x650d, 14);
    const bool down_taken = cell_taken(b->col, want_row, 0x6515);
    GAME_CYCLES(0x6516, 4);
    if (down_taken) {
      GAME_CYCLES(0x651a, 24);
      want_row = b->row;
      b->dy = reflect(b->dy);
      ++blocked;
    } else {
      GAME_CYCLES(0x6518, 1);
    }

    GAME_CYCLES(0x652b, 6);
    if (blocked == 0) {
      // An inside corner: only the diagonal is blocked, so go back the way
      // it came.
      GAME_CYCLES(0x6530, 36);
      want_col = b->col;
      want_row = b->row;
      b->dx = reflect(b->dx);
      b->dy = reflect(b->dy);
    } else {
      GAME_CYCLES(0x652e, 1);
    }
  } else {
    GAME_CYCLES(0x64ed, 1);
  }

  // Erase where it was. Ink 0 is black, and the shape is whatever the caller
  // last left in $00 -- see the header.
  GAME_CYCLES(0x654c, 11);
  s_a = 0x00;
  ram_poke(0x0001, 0x00);
  rom_setcol(0x6552);

  GAME_CYCLES(0x6553, 20);
  ram_poke(0x0002, b->col);
  ram_poke(0x0003, b->row);
  game_plot_shape(0x655f);

  GAME_CYCLES(0x6560, 14);
  s_a = b->row;
  s_y = b->col;
  rom_plot(0x6568);

  GAME_CYCLES(0x6569, 11);
  ram_poke(0x0000, 0x1a);

  if (want_row == 0) {
    // Off the board: not redrawn, and the position is not committed.
    GAME_CYCLES(0x6572, 6);
    ram_poke(0x6637, want_col);
    ram_poke(0x6638, want_row);
    ram_poke(0x6c4a, (uint8_t)blocked);
    return;
  }
  GAME_CYCLES(0x6570, 1);

  GAME_CYCLES(0x6573, 29);
  b->row = want_row;
  b->col = want_col;
  ram_poke(0x0003, b->row);
  ram_poke(0x0002, b->col);
  s_a = 0x03;
  ram_poke(0x0001, 0x03);
  rom_setcol(0x6586);

  GAME_CYCLES(0x6587, 6);
  game_plot_shape(0x6589);

  GAME_CYCLES(0x658a, 14);
  s_a = b->row;
  s_y = b->col;
  rom_plot(0x6592);

  GAME_CYCLES(0x6593, 6);
  ram_poke(0x6637, want_col);
  ram_poke(0x6638, want_row);
  ram_poke(0x6c4a, (uint8_t)blocked);
}
