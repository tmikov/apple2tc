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

/* ========================================================================== */
/* $728D -- the high score                                                    */
/*                                                                            */
/* Four BCD bytes at $7252 against four at $7256, most significant first.      */
/* Below at any byte and it stops; above and it copies; equal and it moves on. */
/*                                                                            */
/* The four compares are a loop here, over a table of addresses. In game.c     */
/* that would be a bug -- the site list is built by grepping for literal       */
/* CYCLES addresses, and a computed one silently leaves the trace. This file   */
/* is deliberately outside that grep, so the constraint does not apply and     */
/* the code can be shaped by what it means instead.                            */
/* ========================================================================== */

/// Most significant first.
static const uint16_t kScoreByte[4] = {0x7255, 0x7254, 0x7253, 0x7252};
static const uint16_t kBestByte[4] = {0x7259, 0x7258, 0x7257, 0x7256};
static const uint16_t kCmpBlock[4] = {0x728d, 0x7297, 0x72a1, 0x72ab};
static const uint16_t kBelowEdge[4] = {0x7293, 0x729d, 0x72a7, 0x72b1};
static const uint16_t kEqualBlock[4] = {0x7295, 0x729f, 0x72a9, 0x72b3};

void game_promote_high_score(void) {
  bool beats_it = true;

  for (unsigned i = 0; i < 4; ++i) {
    GAME_CYCLES(kCmpBlock[i], 10);
    const uint8_t mine = ram_peek(kScoreByte[i]);
    const uint8_t best = ram_peek(kBestByte[i]);
    if (mine < best) {
      GAME_CYCLES(kBelowEdge[i], 1);
      beats_it = false;
      break;
    }
    GAME_CYCLES(kEqualBlock[i], 2);
    if (mine != best) {
      GAME_CYCLES(kEqualBlock[i], 1);
      break;
    }
    // Equal: fall through to the next byte. All four equal reaches the copy
    // below, which is a harmless self-assignment.
  }

  if (beats_it) {
    GAME_CYCLES(0x72b5, 32);
    for (unsigned i = 0; i < 4; ++i)
      ram_poke(kBestByte[i], ram_peek(kScoreByte[i]));
  }

  GAME_CYCLES(0x72cd, 6);
}

/* ========================================================================== */
/* $69C3 -- find an apple                                                     */
/*                                                                            */
/* Sweep whole columns looking for $0F on the occupancy map: from the snake's  */
/* own column leftwards, then from it again rightwards. First hit wins, so the */
/* result leans left. Nothing found parks the answer at row 0, column $14.     */
/*                                                                            */
/* $6B39/$6B3A are the cursor and $6B3B/$6B3C the answer. The cursor is a pair */
/* of locals here, mirrored back at the end because ram.probe still hashes it. */
/* ========================================================================== */

/// The lo-res occupancy map's value at \p c. $0F is an apple.
static uint8_t cell_at(Cell c, uint16_t ret) {
  s_a = c.row;
  s_y = c.col;
  rom_scrn(ret);
  return s_a;
}

void game_find_nearest_apple(void) {
  static const uint8_t kApple = 0x0f;
  static const uint8_t kLastRow = 0x27;

  Cell c = {.col = ram_peek(0x624f), .row = 1};
  bool found = false;

  GAME_CYCLES(0x69c3, 14);
  for (;;) { // leftwards
    GAME_CYCLES(0x69ce, 14);
    const uint8_t v = cell_at(c, 0x69d6);
    GAME_CYCLES(0x69d7, 4);
    if (v == kApple) {
      GAME_CYCLES(0x69d9, 1);
      found = true;
      break;
    }
    GAME_CYCLES(0x69db, 14);
    if (++c.row != kLastRow) {
      GAME_CYCLES(0x69e3, 1);
      continue;
    }
    GAME_CYCLES(0x69e5, 14);
    c.row = 1;
    if (--c.col == 0)
      break;
    GAME_CYCLES(0x69ed, 1);
  }

  if (!found) {
    GAME_CYCLES(0x69ef, 8);
    c.col = ram_peek(0x624f);

    for (;;) { // rightwards
      GAME_CYCLES(0x69f5, 14);
      const uint8_t v = cell_at(c, 0x69fd);
      GAME_CYCLES(0x69fe, 2);
      GAME_CYCLES(0x6a00, 2);
      if (v == kApple) {
        GAME_CYCLES(0x6a00, 1);
        break;
      }
      GAME_CYCLES(0x6a02, 14);
      if (++c.row != kLastRow) {
        GAME_CYCLES(0x6a0a, 1);
        continue;
      }
      GAME_CYCLES(0x6a0c, 20);
      c.row = 1;
      if (++c.col == kLastRow) {
        GAME_CYCLES(0x6a1b, 12);
        c.row = 0;
        c.col = 0x14;
        break;
      }
      GAME_CYCLES(0x6a19, 1);
    }
  }

  GAME_CYCLES(0x6a25, 22);
  // The cursor is scratch, but ram.probe hashes $6000-$BFFF, so what the
  // original left there is still compared.
  ram_poke(0x6b39, c.col);
  ram_poke(0x6b3a, c.row);
  ram_poke(0x6b3b, c.col);
  ram_poke(0x6b3c, c.row);
}

/* ========================================================================== */
/* $6AB8 -- is a move safe?                                                   */
/*                                                                            */
/* The four neighbour probes are the same shape four times over, so they are  */
/* a table here. In the original they are 60 bytes of straight-line code with */
/* the offsets and the block addresses interleaved.                           */
/*                                                                            */
/* The verdict comes back as an enum and the adapter turns it into A and the  */
/* flags, which is the one thing that cannot move into this file while        */
/* generated callers still branch on them.                                    */
/* ========================================================================== */

/// Where each neighbour is, and the four block addresses the original spends
/// on it: the SCRN block, the CMP after it, the not-taken edge, and the
/// increment.
static const struct {
  int8_t dcol, drow;
  uint16_t scrn_block, scrn_cycles, scrn_ret;
  uint16_t cmp_block, edge, inc_block;
} kNeighbour[4] = {
    {+1, 0, 0x6ae8, 12, 0x6aee, 0x6aef, 0x6af1, 0x6af3},
    {-1, 0, 0x6af6, 16, 0x6aff, 0x6b00, 0x6b02, 0x6b04},
    {0, +1, 0x6b07, 18, 0x6b12, 0x6b13, 0x6b15, 0x6b17},
    {0, -1, 0x6b1a, 18, 0x6b25, 0x6b26, 0x6b28, 0x6b2a},
};

MoveVerdict snake_move_verdict(uint8_t dir, uint8_t *cell_out) {
  GAME_CYCLES(0x6ab8, 42);

  // The head plus this direction's deltas.
  const Cell target = {
      .col = (uint8_t)(ram_peek(0x6232 + dir) + ram_peek(0x624f)),
      .row = (uint8_t)(ram_peek(0x6237 + dir) + ram_peek(0x6250)),
  };
  s_status_v = ovf8(target.row, ram_peek(0x6237 + dir), ram_peek(0x6250));
  ram_poke(0x6637, target.col);
  ram_poke(0x6638, target.row);

  const uint8_t cell = cell_at(target, 0x6ad4);
  *cell_out = cell;

  // Empty or an apple, and nothing else, may be stepped into.
  GAME_CYCLES(0x6ad5, 4);
  if (cell != 0x00)
    GAME_CYCLES(0x6ad9, 2);
  else
    GAME_CYCLES(0x6ad7, 1);
  GAME_CYCLES(0x6adb, 2);
  if (cell != 0x00 && cell != 0x0f) {
    GAME_CYCLES(0x6add, 6);
    return MOVE_TARGET_TAKEN;
  }
  GAME_CYCLES(0x6adb, 1);

  GAME_CYCLES(0x6ade, 12);
  ram_poke(0x6c4a, 0x00);
  if (target.row == 0) {
    // Row 0 is the top border; there is nothing above it to look at.
    GAME_CYCLES(0x6ae6, 1);
    GAME_CYCLES(0x6add, 6);
    return MOVE_ROW_ZERO;
  }

  // A target whose four neighbours are all occupied is a dead end: legal to
  // enter, fatal on the move after, so it is refused a step early.
  unsigned free_neighbours = 0;
  for (unsigned i = 0; i < 4; ++i) {
    GAME_CYCLES(kNeighbour[i].scrn_block, kNeighbour[i].scrn_cycles);
    const Cell n = {
        .col = (uint8_t)(target.col + kNeighbour[i].dcol),
        .row = (uint8_t)(target.row + kNeighbour[i].drow),
    };
    if (kNeighbour[i].drow)
      s_status_v = ovf8(n.row, target.row, kNeighbour[i].drow > 0 ? 0x01 : 0xfe);

    const uint8_t v = cell_at(n, kNeighbour[i].scrn_ret);
    GAME_CYCLES(kNeighbour[i].cmp_block, 4);
    if (v == 0x00) {
      GAME_CYCLES(kNeighbour[i].inc_block, 6);
      ++free_neighbours;
      ram_poke(0x6c4a, (uint8_t)free_neighbours);
    } else {
      GAME_CYCLES(kNeighbour[i].edge, 1);
    }
  }

  GAME_CYCLES(0x6b2d, 6);
  if (free_neighbours) {
    GAME_CYCLES(0x6b32, 8);
    return MOVE_OK;
  }
  GAME_CYCLES(0x6b30, 1);
  GAME_CYCLES(0x6b35, 8);
  return MOVE_DEAD_END;
}

/* ========================================================================== */
/* $7045 -- draw the playfield                                                */
/*                                                                            */
/* 62 blocks in one routine, and six things: set the video mode, spin, wipe    */
/* the occupancy map, open the wall gaps the difficulty calls for, draw the    */
/* border in both representations, and run the level's display list.           */
/*                                                                            */
/* The display list is the reason this one was worth the trace it cost. $8000  */
/* holds 29 scripts, one per level, each ending in '*'; the interpreter skips  */
/* level-1 scripts to find the current one and then dispatches single-letter   */
/* opcodes. As a chain of CMP/BNE with the operand loads interleaved it is 180 */
/* bytes that read as noise.                                                  */
/* ========================================================================== */

/// The display list's opcodes. Single letters, in Apple II ASCII.
typedef enum {
  OP_RESTART = 0x45, ///< 'E' -- back to level 1 and start over. Level 30 only.
  OP_HLINE = 0x48,   ///< 'H' -- ink, column, last column, row
  OP_PLOT = 0x50,    ///< 'P' -- ink, column, row. No script uses it.
  OP_STORE = 0x54,   ///< 'T' -- one byte, into $0304
  OP_VLINE = 0x56,   ///< 'V' -- ink, row, last row, column
  OP_END = 0x2a,     ///< '*' -- end of this level's script
} ScriptOp;

/// $7024 through its adapter. The ink arrives in the Z flag, not in A.
static void set_ink(uint8_t ink, uint16_t ret) {
  s_a = ink;
  s_status_not_z = ink;
  s_status_n = (ink & 0x80);
  game_set_ink(ret);
}

/// The ROM's HLINE, which takes its right-hand end from $2C.
static void lores_hline(uint8_t row, uint8_t from_col, uint16_t ret) {
  s_a = row;
  s_y = from_col;
  rom_hline(ret);
}

/// The ROM's PLOT.
static void lores_plot(uint8_t row, uint8_t col, uint16_t ret) {
  s_a = row;
  s_y = col;
  rom_plot(ret);
}

/// $7019 through its adapter: the next display-list byte.
static uint8_t script_byte(uint16_t ret) {
  game_next_byte(ret);
  return s_a;
}

/// Graphics, hi-res, page 2, full screen. The reads are the writes.
static void select_hires_page2(void) {
  GAME_CYCLES(0x7048, 32);
  ram_poke(0x6c46, 0x00);
  io_peek(0xc050);
  io_peek(0xc057);
  io_peek(0xc055);
  io_peek(0xc052);
  ram_poke(0x0002, 0x04);
  ram_poke(0x0003, 0x00);
}

/// A plain three-deep delay. Y is whatever the caller left in it; the original
/// does not initialise it, and the counters live in zero page because there is
/// nowhere else to put them.
static void spin(void) {
  for (;;) {
    GAME_CYCLES(0x7061, 4);
    if (--s_y) {
      GAME_CYCLES(0x7062, 1);
      continue;
    }
    GAME_CYCLES(0x7064, 7);
    ram_poke(0x0003, (uint8_t)(ram_peek(0x0003) - 1));
    if (ram_peek(0x0003)) {
      GAME_CYCLES(0x7066, 1);
      continue;
    }
    GAME_CYCLES(0x7068, 7);
    ram_poke(0x0002, (uint8_t)(ram_peek(0x0002) - 1));
    if (!ram_peek(0x0002))
      break;
    GAME_CYCLES(0x706a, 1);
  }
}

/// Clear the lo-res occupancy map, one full-width row at a time from the
/// bottom up. Ink 0 is black, so this erases.
static void wipe_occupancy_map(void) {
  GAME_CYCLES(0x706c, 13);
  ram_poke(0x0003, 0x27);
  set_ink(0x00, 0x7074);

  for (;;) {
    GAME_CYCLES(0x7075, 16);
    ram_poke(0x002c, 0x27);
    lores_hline(ram_peek(0x0003), 0x00, 0x707f);

    GAME_CYCLES(0x7080, 7);
    const uint8_t row = (uint8_t)(ram_peek(0x0003) - 1);
    ram_poke(0x0003, row);
    // BPL: row 0 is drawn, and the loop ends one step later.
    if (row & 0x80)
      break;
    GAME_CYCLES(0x7082, 1);
  }
}

/// One gap per bouncer, which is what the difficulty counts.
static void open_wall_gaps(void) {
  GAME_CYCLES(0x7093, 6);
  const uint8_t difficulty = ram_peek(0x0301);
  if (!difficulty) {
    GAME_CYCLES(0x7096, 1);
    return;
  }

  GAME_CYCLES(0x7098, 10);
  lores_plot(0x01, 0x01, 0x709d);

  GAME_CYCLES(0x709e, 8);
  if (ram_peek(0x0301) == 0x01) {
    GAME_CYCLES(0x70a3, 1);
    return;
  }
  GAME_CYCLES(0x70a5, 10);
  lores_plot(0x01, 0x26, 0x70ab);
}

/// The border, in both representations, plus the gap the snake leaves through.
static void draw_border(void) {
  GAME_CYCLES(0x70ac, 10);
  lores_hline(0x00, 0x00, 0x70b2); // $2C is still $27 from the wipe

  GAME_CYCLES(0x70b3, 10);
  lores_hline(0x27, 0x00, 0x70b9);

  GAME_CYCLES(0x70ba, 21);
  ram_poke(0x0002, 0x00);
  ram_poke(0x0003, 0x00);
  ram_poke(0x0008, 0x27);
  game_lores_vline(0x70c8);

  GAME_CYCLES(0x70c9, 16);
  ram_poke(0x0003, 0x00);
  ram_poke(0x0002, 0x27);
  game_lores_vline(0x70d3);

  GAME_CYCLES(0x70d4, 19);
  ram_poke(0x0002, 0x00);
  ram_poke(0x0003, 0x00);
  ram_poke(0x0008, 0x27);
  game_plot_hline(0x70e0);

  GAME_CYCLES(0x70e1, 16);
  ram_poke(0x0002, 0x00);
  ram_poke(0x0003, 0x27);
  game_plot_hline(0x70eb);

  GAME_CYCLES(0x70ec, 14);
  ram_poke(0x0002, 0x00);
  ram_poke(0x0003, 0x00);
  game_plot_vline(0x70f4);

  GAME_CYCLES(0x70f5, 16);
  ram_poke(0x0003, 0x00);
  ram_poke(0x0002, 0x27);
  game_plot_vline(0x70ff);

  // Ink 3 over columns $12-$16 of the bottom row, on top of the border just
  // laid down: the gap the snake leaves through.
  GAME_CYCLES(0x7100, 26);
  ram_poke(0x0001, 0x03);
  ram_poke(0x0003, 0x27);
  ram_poke(0x0002, 0x12);
  ram_poke(0x0008, 0x16);
  game_plot_hline(0x7112);
}

/// Walk the pointer to the current level's script, skipping one whole script
/// per level below it. DEX first, so level 1 skips nothing.
static void seek_script(void) {
  GAME_CYCLES(0x7113, 14);
  s_x = ram_peek(0x0303);
  ram_poke(0x000a, 0x00);
  ram_poke(0x000b, 0x80);

  for (;;) {
    GAME_CYCLES(0x711e, 4);
    if (!--s_x) {
      GAME_CYCLES(0x711f, 1);
      return;
    }
    for (;;) {
      GAME_CYCLES(0x7121, 6);
      const uint8_t b = script_byte(0x7123);
      GAME_CYCLES(0x7124, 4);
      if (b == OP_END) {
        GAME_CYCLES(0x7126, 1);
        break;
      }
      GAME_CYCLES(0x7128, 3);
    }
  }
}

void game_draw_playfield_native(void) {
  GAME_CYCLES(0x7045, 6);
  game_clear_hgr(0x7047);
  select_hires_page2();
  spin();
  wipe_occupancy_map();

  GAME_CYCLES(0x7084, 21);
  ram_poke(0x0022, 0x14); // text window top
  ram_poke(0x0000, 0x15); // shape
  ram_poke(0x0001, 0x0d); // ink
  set_ink(0x0d, 0x7092);

  open_wall_gaps();
  draw_border();

restart:
  seek_script();

  for (;;) {
    GAME_CYCLES(0x712b, 6);
    const uint8_t op = script_byte(0x712d);

    GAME_CYCLES(0x712e, 4);
    if (op == OP_RESTART) {
      GAME_CYCLES(0x7132, 9);
      ram_poke(0x0303, 0x01);
      goto restart;
    }
    GAME_CYCLES(0x7130, 1);

    GAME_CYCLES(0x713a, 4);
    if (op == OP_HLINE) {
      GAME_CYCLES(0x713e, 6);
      const uint8_t ink = script_byte(0x7140);
      GAME_CYCLES(0x7141, 9);
      ram_poke(0x0001, ink);
      const uint8_t col = script_byte(0x7145);
      GAME_CYCLES(0x7146, 9);
      ram_poke(0x0002, col);
      const uint8_t last = script_byte(0x714a);
      GAME_CYCLES(0x714b, 9);
      ram_poke(0x0008, last);
      const uint8_t row = script_byte(0x714f);
      GAME_CYCLES(0x7150, 12);
      ram_poke(0x0003, row);
      set_ink(ram_peek(0x0001), 0x7156);

      GAME_CYCLES(0x7157, 18);
      ram_poke(0x002c, ram_peek(0x0008));
      lores_hline(ram_peek(0x0003), ram_peek(0x0002), 0x7161);

      GAME_CYCLES(0x7162, 6);
      game_plot_hline(0x7164);
      GAME_CYCLES(0x7165, 3);
      continue;
    }
    GAME_CYCLES(0x713c, 1);

    GAME_CYCLES(0x7168, 4);
    if (op == OP_VLINE) {
      GAME_CYCLES(0x716c, 6);
      const uint8_t ink = script_byte(0x716e);
      GAME_CYCLES(0x716f, 9);
      ram_poke(0x0001, ink);
      const uint8_t row = script_byte(0x7173);
      GAME_CYCLES(0x7174, 9);
      ram_poke(0x0003, row);
      const uint8_t last = script_byte(0x7178);
      GAME_CYCLES(0x7179, 9);
      ram_poke(0x0008, last);
      const uint8_t col = script_byte(0x717d);
      GAME_CYCLES(0x717e, 12);
      ram_poke(0x0002, col);
      set_ink(ram_peek(0x0001), 0x7184);

      GAME_CYCLES(0x7185, 6);
      game_lores_vline(0x7187);
      GAME_CYCLES(0x7188, 6);
      game_plot_vline(0x718a);
      GAME_CYCLES(0x718b, 3);
      continue;
    }
    GAME_CYCLES(0x716a, 1);

    GAME_CYCLES(0x718e, 4);
    if (op == OP_PLOT) {
      GAME_CYCLES(0x7192, 6);
      const uint8_t ink = script_byte(0x7194);
      GAME_CYCLES(0x7195, 9);
      ram_poke(0x0001, ink);
      const uint8_t col = script_byte(0x7199);
      GAME_CYCLES(0x719a, 9);
      ram_poke(0x0002, col);
      const uint8_t row = script_byte(0x719e);
      GAME_CYCLES(0x719f, 12);
      ram_poke(0x0003, row);
      set_ink(ram_peek(0x0001), 0x71a5);

      GAME_CYCLES(0x71a6, 12);
      lores_plot(ram_peek(0x0003), ram_peek(0x0002), 0x71ac);
      GAME_CYCLES(0x71ad, 6);
      game_plot_shape(0x71af);
      GAME_CYCLES(0x71b0, 3);
      continue;
    }
    GAME_CYCLES(0x7190, 1);

    GAME_CYCLES(0x71b3, 4);
    if (op == OP_STORE) {
      GAME_CYCLES(0x71b7, 6);
      const uint8_t v = script_byte(0x71b9);
      GAME_CYCLES(0x71ba, 7);
      ram_poke(0x0304, v);
      continue;
    }
    GAME_CYCLES(0x71b5, 1);

    GAME_CYCLES(0x71c0, 4);
    if (op == OP_END) {
      GAME_CYCLES(0x71c2, 1);
      GAME_CYCLES(0x71c7, 6);
      return;
    }
    // Anything unrecognised is skipped. No script contains one.
    GAME_CYCLES(0x71c4, 3);
  }
}

/* ========================================================================== */
/* $60E7, $6B93, $702B -- the hi-res plotter                                  */
/*                                                                            */
/* Hi-res page 1 is a grid of 48 rows, four scanlines tall and one byte wide.  */
/* $6000/$6030 hold each row's base address split into low and high halves,    */
/* and successive scanlines are $400 apart -- which is why walking down a cell */
/* is +4 on the high byte and nothing else.                                    */
/*                                                                            */
/* $04/$05 (the destination pointer), $06 (the pattern index) and $07 (the     */
/* scanline counter) are locals, mirrored back at the end for ram.probe.       */
/* ========================================================================== */

/// The address of a cell row's first scanline, from the split table.
static uint16_t cell_row_base(uint8_t row) {
  return (uint16_t)(ram_peek(0x6000 + row) | (ram_peek(0x6030 + row) << 8));
}

/// Index into the 128-byte dot table at $6064: 16 inks of 8, four column
/// phases in each of two scanline parities.
static uint8_t dot_index(uint8_t ink, uint8_t scanline, uint8_t col) {
  return (uint8_t)((uint8_t)(((ink << 1) | (scanline & 1)) << 2) | (col & 3));
}

/// $60E7 -- draw the loaded shape into one cell, replacing what was there.
void game_draw_cell_native(uint8_t ink, Cell c) {
  GAME_CYCLES(0x60e7, 22);
  uint16_t dest = cell_row_base(c.row);
  ram_poke(0x0007, 0x00);
  ram_poke(0x0004, (uint8_t)dest);
  ram_poke(0x0005, (uint8_t)(dest >> 8));

  for (unsigned line = 0; line < 4; ++line) {
    GAME_CYCLES(0x60f7, 16);
    // Built in $06 in two steps, and written out between them because it is
    // zero page and a probe may sample there.
    ram_poke(0x0006, (uint8_t)((ink << 1) | (line & 1)));
    GAME_CYCLES(0x6100, 62);
    const uint8_t idx = dot_index(ink, (uint8_t)line, c.col);
    ram_poke(0x0006, idx);

    poke(dest + c.col, (uint8_t)(ram_peek(0x6064 + idx) & ram_peek(0x6060 + line)));

    ram_poke(0x0007, (uint8_t)(line + 1));
    dest += 0x0400; // one scanline down, i.e. +4 on the high byte
    ram_poke(0x0005, (uint8_t)(dest >> 8));

    if (line != 3)
      GAME_CYCLES(0x6124, 1);
  }

  GAME_CYCLES(0x6126, 6);
}

/// $6B93 -- the same cell, merged instead of replaced: only bits are set, and
/// the pattern is inverted first. $7F and not $FF because bit 7 is the byte's
/// hi-res palette bit and flipping it would shift the whole byte's colour.
///
/// It also builds the index differently -- ROR where $60F7 has ROL, so the
/// scanline parity lands in bit 7 and the two following ASLs shift it out.
/// The index degenerates to (ink >> 1) * 4 + (col & 3). Changing it to match
/// $60F7 fails the screen check, so whatever the author meant, it is load
/// bearing.
void game_merge_cell_native(uint8_t ink, Cell c) {
  GAME_CYCLES(0x6b96, 22);
  uint16_t dest = cell_row_base(c.row);
  ram_poke(0x0007, 0x00);
  ram_poke(0x0004, (uint8_t)dest);
  ram_poke(0x0005, (uint8_t)(dest >> 8));

  for (unsigned line = 0; line < 4; ++line) {
    GAME_CYCLES(0x6ba6, 85);
    const uint8_t parity = (uint8_t)(line & 1);
    const uint8_t idx =
        (uint8_t)((uint8_t)(((uint8_t)((parity << 7) | (ink >> 1))) << 2) | (c.col & 3));
    ram_poke(0x0006, idx);

    const uint16_t at = dest + c.col;
    poke(at,
         (uint8_t)(((ram_peek(0x6064 + idx) ^ 0x7f) & ram_peek(0x6060 + line)) | peek(at)));

    ram_poke(0x0007, (uint8_t)(line + 1));
    dest += 0x0400;
    ram_poke(0x0005, (uint8_t)(dest >> 8));

    if (line != 3)
      GAME_CYCLES(0x6bd7, 1);
  }

  GAME_CYCLES(0x6bd9, 6);
}

/// $702B -- zero hi-res page 1, $2000 through $3FFF. The inner loop runs a
/// full 256 bytes because Y wraps, so the terminating test is on the page.
void game_clear_hgr_native(void) {
  GAME_CYCLES(0x702b, 12);
  ram_poke(0x0004, 0x00);
  ram_poke(0x0005, 0x20);
  s_y = 0x00;

  for (uint8_t page = 0x20;;) {
    uint8_t y = 0;
    do {
      GAME_CYCLES(0x7035, 12);
      poke((uint16_t)(page << 8) + y, 0x00);
      ++y;
      s_y = y;
      if (y)
        GAME_CYCLES(0x703a, 1);
    } while (y);

    GAME_CYCLES(0x703c, 12);
    ++page;
    ram_poke(0x0005, page);
    if (page == 0x40)
      break;
    GAME_CYCLES(0x7042, 1);
  }

  GAME_CYCLES(0x7044, 6);
}

/* ========================================================================== */
/* $6148, $615A, $7000 -- runs of cells                                       */
/*                                                                            */
/* Each loads the shape once and repeats a draw along one axis until the       */
/* moving coordinate reaches $08. The end is tested after drawing, so it is    */
/* inclusive and a degenerate run still plots one cell. A start past the end   */
/* wraps through 255; nothing guards against it and nothing needs to.          */
/* ========================================================================== */

/// $6148 -- a horizontal run of hi-res cells, columns $02 through $08 on row
/// $03. The coordinate stays in zero page because game_draw_cell's adapter
/// reads it from there.
void game_plot_hline_native(void) {
  for (;;) {
    GAME_CYCLES(0x614b, 6);
    game_draw_cell(0x614d);

    GAME_CYCLES(0x614e, 8);
    const uint8_t col = ram_peek(0x0002);
    if (col == ram_peek(0x0008))
      break;

    GAME_CYCLES(0x6154, 8);
    ram_poke(0x0002, (uint8_t)(col + 1));
  }
  GAME_CYCLES(0x6152, 1);
  GAME_CYCLES(0x6159, 6);
}

/// $615A -- the same down a column: rows $03 through $08 in column $02.
void game_plot_vline_native(void) {
  for (;;) {
    GAME_CYCLES(0x615d, 6);
    game_draw_cell(0x615f);

    GAME_CYCLES(0x6160, 8);
    const uint8_t row = ram_peek(0x0003);
    if (row == ram_peek(0x0008))
      break;

    GAME_CYCLES(0x6166, 8);
    ram_poke(0x0003, (uint8_t)(row + 1));
  }
  GAME_CYCLES(0x6164, 1);
  GAME_CYCLES(0x6159, 6);
}

/// $7000 -- the lo-res half of a vertical run. Unlike the hi-res one it puts
/// $03 back where it found it, because the caller draws the hi-res run over
/// the same coordinates next.
void game_lores_vline_native(void) {
  const uint8_t first_row = ram_peek(0x0003);
  push8(first_row);

  for (;;) {
    GAME_CYCLES(0x7003, 12);
    lores_plot(ram_peek(0x0003), ram_peek(0x0002), 0x7009);

    GAME_CYCLES(0x700a, 8);
    const uint8_t row = ram_peek(0x0003);
    if (row == ram_peek(0x0008))
      break;

    GAME_CYCLES(0x7010, 8);
    ram_poke(0x0003, (uint8_t)(row + 1));
  }
  GAME_CYCLES(0x700e, 1);
  GAME_CYCLES(0x7015, 13);
  ram_poke(0x0003, pop8());
}

/* ========================================================================== */
/* $6594 -- step the bouncers, then take a key                                */
/* ========================================================================== */

/// Move one bouncer through the $6633-$6636 parameter block that $64C8's
/// adapter still reads. The load and store either side are the original's own
/// eight ram_pokes, which is what a struct copy looks like without structs.
static void step_bouncer_slot(int slot, uint16_t block, uint16_t cycles, uint16_t ret,
                              uint16_t back_block, uint16_t back_cycles) {
  GAME_CYCLES(block, cycles);
  const Bouncer in = bouncer_load(slot);
  ram_poke(0x6633, in.col);
  ram_poke(0x6634, in.row);
  ram_poke(0x6635, (uint8_t)in.dx);
  ram_poke(0x6636, (uint8_t)in.dy);
  game_move_bouncer(ret);

  GAME_CYCLES(back_block, back_cycles);
  const Bouncer out = {
      .col = ram_peek(0x6633),
      .row = ram_peek(0x6634),
      .dx = (int8_t)ram_peek(0x6635),
      .dy = (int8_t)ram_peek(0x6636),
  };
  bouncer_store(slot, out);
}

/// $6200 -- take the next key out of the ring buffer game_read_key fills.
/// Only a byte with bit 7 set counts; the slot is cleared and the read index
/// advances. Returns what the original leaves in A.
///
/// $6216, the RTS all paths share, is charged by the adapter and not here.
/// game_read_key also ends there and is not converted, so $6216 is still a
/// probe site -- and converting one of a shared block's two paths, while the
/// other still reports, is exactly how the two engines stop agreeing. The
/// trace caught it; the pinned site count could not, because the count was
/// right.
static uint8_t dequeue_key(void) {
  GAME_CYCLES(0x6200, 10);
  const uint8_t at = ram_peek(0x624c);
  const uint8_t key = ram_peek(0x623c + at);
  if (!(key & 0x80)) {
    GAME_CYCLES(0x6206, 1);
    return key;
  }

  GAME_CYCLES(0x6208, 24);
  ram_poke(0x623c + at, 0x00);
  ram_poke(0x624c, (uint8_t)((at + 1) & 0x0f));
  // X *is* live out of $6594 -- `apple2tc --ir` says so -- unlike X out of
  // $6C72, where the same check let the write go. So it is maintained.
  s_x = (uint8_t)(at + 1);
  return key;
}

/// $6594 -- step as many bouncers as the difficulty calls for, then fall into
/// the key dequeue whose byte is the return value.
uint8_t game_step_bouncers_native(void) {
  GAME_CYCLES(0x6594, 6);
  const uint8_t difficulty = ram_peek(0x0301);

  if (!difficulty) {
    GAME_CYCLES(0x6599, 3);
    return dequeue_key();
  }
  GAME_CYCLES(0x6597, 1);

  step_bouncer_slot(0, 0x659c, 38, 0x65b6, 0x65b7, 40);

  if (ram_peek(0x0301) == 0x01) {
    GAME_CYCLES(0x65d6, 3);
    return dequeue_key();
  }
  GAME_CYCLES(0x65d4, 1);

  step_bouncer_slot(1, 0x65d9, 38, 0x65f3, 0x65f4, 35);
  return dequeue_key();
}

/* ========================================================================== */
/* $6C72 -- turn the next input into a direction                              */
/* ========================================================================== */

/// The six inputs the game understands. $6C63 holds the key that produces
/// each one and $6C6A the code the game acts on; the two start out identical,
/// which is why the substitution is invisible until the player rebinds a key
/// from the title screen. Both tables are part of the loaded image, and the
/// first is written at $757C, from the redefinition screen.
///
/// Nothing here is checked by any oracle, and coverage cannot say so. The two
/// blocks that do the substitution both run -- play.pkeys presses I, J, K and
/// M all through the round -- but it presses the *default* bindings, where the
/// two tables are byte-identical and reading the wrong one is invisible.
/// play-hires.pkeys does rebind, to W A D X Q E, and then the recording ends:
/// it never plays afterwards. Measured, not assumed -- swapping input_code for
/// input_key here passes verify.sh 4/4, all three traces, memory and screen.
/// A recording that rebinds and then plays would close it.
enum { kInputCount = 6 };

static uint8_t input_key(int i) {
  return ram_peek(0x6c63 + i);
}

static uint8_t input_code(int i) {
  return ram_peek(0x6c6a + i);
}

/// The two codes that are settings rather than directions.
enum { kCodeJoystickOn = 0x80, kCodeJoystickOff = 0x8b };

/// The code every input collapses to in attract mode. Its caller stores $FF
/// at $6253, which ends the game in progress.
enum { kCodeStop = 0x92 };

/// $6C71 -- set once the player has chosen the joystick.
static bool joystick_selected(void) {
  return ram_peek(0x6c71) != 0;
}

static void select_joystick(bool on) {
  ram_poke(0x6c71, on ? 0x01 : 0x00);
}

/// $0302 -- attract mode: nobody answered the difficulty prompt before it
/// timed out, so the game is playing itself. Any input at all ends it, which
/// is why the whole key table is skipped below.
static bool attract_mode(void) {
  return ram_peek(0x0302) != 0;
}

/// A switch input. Every one of the game's three read sites -- here, the
/// difficulty prompt at $7428, and the pause check at $788B -- takes bit 7
/// *clear* to mean active, which is the opposite of a stock Apple II paddle
/// button. Consistent enough to be the convention of whatever switch box the
/// game was written for, so it is transcribed and not corrected.
static bool switch_pressed(uint16_t sw) {
  return !(io_peek(sw) & 0x80);
}

uint8_t game_read_direction_native(uint8_t key) {
  GAME_CYCLES(0x6c75, 9);

  if (attract_mode()) {
    GAME_CYCLES(0x6c7b, 6);
    if (joystick_selected()) {
      GAME_CYCLES(0x6c80, 6);
      if (switch_pressed(0xc061)) {
        GAME_CYCLES(0x6c85, 12);
        return kCodeStop;
      }
      GAME_CYCLES(0x6c83, 1);
    } else {
      GAME_CYCLES(0x6c7e, 1);
    }

    GAME_CYCLES(0x6c89, 6);
    if (key & 0x80) {
      GAME_CYCLES(0x6c8c, 8);
      return kCodeStop;
    }
    GAME_CYCLES(0x6c8a, 1);
    GAME_CYCLES(0x6c8f, 6);
    // Not a keypress, so nothing happened -- $00 out of an empty ring.
    return key;
  }
  GAME_CYCLES(0x6c79, 1);

  // Search the bindings from the last slot down, so that if the player has
  // bound the same key twice the higher slot wins.
  GAME_CYCLES(0x6c90, 6);
  uint8_t code = key;
  int slot = kInputCount - 1;
  for (;;) {
    GAME_CYCLES(0x6c93, 6);
    if (key == input_key(slot)) {
      GAME_CYCLES(0x6c96, 1);
      GAME_CYCLES(0x6c9e, 4);
      code = input_code(slot);
      break;
    }

    GAME_CYCLES(0x6c98, 4);
    if (--slot < 0) {
      GAME_CYCLES(0x6c9b, 3);
      break;
    }
    GAME_CYCLES(0x6c99, 1);
  }
  // X is not written back. The original leaves it on the matching slot, or
  // $FF, but nothing reads it: `apple2tc --ir` prints per-function register
  // liveness, and func_6c72's LiveOut is A, Y and the flags. Y is in that set,
  // so the joystick block below does maintain it.

  GAME_CYCLES(0x6ca1, 4);
  if (code == kCodeJoystickOn) {
    GAME_CYCLES(0x6ca5, 12);
    select_joystick(true);
    return 0x01;
  }
  GAME_CYCLES(0x6ca3, 1);

  GAME_CYCLES(0x6cab, 4);
  if (code == kCodeJoystickOff) {
    GAME_CYCLES(0x6caf, 12);
    select_joystick(false);
    return 0x00;
  }
  GAME_CYCLES(0x6cad, 1);

  GAME_CYCLES(0x6cb5, 4);
  if (code & 0x80) {
    // A direction. Hand it straight back.
    GAME_CYCLES(0x6cb9, 6);
    return code;
  }
  GAME_CYCLES(0x6cb7, 1);

  GAME_CYCLES(0x6cba, 6);
  const uint8_t joystick = ram_peek(0x6c71);
  if (!joystick) {
    GAME_CYCLES(0x6cbf, 8);
    return code;
  }
  GAME_CYCLES(0x6cbd, 1);

  // The joystick is two switch inputs read twice, with annunciator 2
  // selecting the pair -- four directions on two pins. Exactly one has to be
  // active: none or several is ambiguous and rejected.
  GAME_CYCLES(0x6cc2, 12);
  int pressed = 0;
  uint8_t chosen = 0; // only read when exactly one input turned out active
  io_peek(0xc05b); // annunciator 2 on
  if (switch_pressed(0xc062)) {
    GAME_CYCLES(0x6ccc, 4);
    chosen = 0;
    ++pressed;
  } else {
    GAME_CYCLES(0x6cca, 1);
  }

  GAME_CYCLES(0x6ccf, 6);
  if (switch_pressed(0xc063)) {
    GAME_CYCLES(0x6cd4, 4);
    chosen = 3;
    ++pressed;
  } else {
    GAME_CYCLES(0x6cd2, 1);
  }

  GAME_CYCLES(0x6cd7, 10);
  io_peek(0xc05a); // annunciator 2 off
  if (switch_pressed(0xc062)) {
    GAME_CYCLES(0x6cdf, 4);
    chosen = 1;
    ++pressed;
  } else {
    GAME_CYCLES(0x6cdd, 1);
  }

  GAME_CYCLES(0x6ce2, 6);
  if (switch_pressed(0xc063)) {
    GAME_CYCLES(0x6ce7, 4);
    chosen = 2;
    ++pressed;
  } else {
    GAME_CYCLES(0x6ce5, 1);
  }
  // Y, unlike X, is live out of here -- see above.
  s_y = (uint8_t)pressed;

  GAME_CYCLES(0x6cea, 4);
  if (pressed != 1) {
    GAME_CYCLES(0x6cee, 8);
    return 0x00;
  }
  GAME_CYCLES(0x6cec, 1);
  GAME_CYCLES(0x6cf1, 10);
  return input_code(chosen);
}

/* ========================================================================== */
/* $75D1 -- read a replacement key for one slot                               */
/* ========================================================================== */

/// Where slot \p slot sits on the redefinition screen, and what blinks there.
///
/// $75CB is not the slot's current binding, which is the obvious reading. Its
/// six bytes are $E2 $E4 $E5 $E3 $E4 $E5, and the repeats settle it: six
/// distinct keys cannot have four distinct glyphs. It is the *direction* the
/// slot stands for -- up, left, right, down, left, right -- drawn through the
/// game's own hi-res COUT, which is why the player knows which key is being
/// asked for.
static uint8_t slot_col(int slot) {
  return ram_peek(0x75bf + slot);
}

static uint8_t slot_row(int slot) {
  return ram_peek(0x75c5 + slot);
}

static uint8_t slot_glyph(int slot) {
  return ram_peek(0x75cb + slot);
}

/// Both halves of the blink count X down to zero 256 times, and the X they
/// start from is whatever COUT left behind -- the original never initialises
/// it. So the first pass is a different length from the other 255, by an
/// amount that depends on the ROM. Transcribed rather than tidied: it is the
/// delay's actual duration.
static uint8_t cout_left_x(void) {
  return s_x;
}

/// $75D1 -- the dark half: erase the glyph and wait, polling nothing.
static void edit_key_blank(uint8_t slot) {
  // The slot is parked at $0002 for the whole routine because COUT clobbers X
  // and every step below needs it again.
  GAME_CYCLES(0x75d1, 23);
  ram_poke(0x0002, slot);
  ram_poke(0x0024, slot_col(slot));
  ram_poke(0x0025, slot_row(slot));
  rom_fc68(0x75df);

  GAME_CYCLES(0x75e0, 11);
  s_x = slot;
  s_a = 0xa0; // space
  s_status_not_z = 0xa0;
  s_status_n = 0x80;
  rom_cout(0x75e6);

  GAME_CYCLES(0x75e7, 2);
  uint8_t x = cout_left_x();
  uint8_t y = 0;
  for (;;) {
    GAME_CYCLES(0x75e9, 4);
    if (--x) {
      GAME_CYCLES(0x75ea, 1);
      continue;
    }
    // $75EC is `LDA #$41 / BEQ`, a branch that cannot be taken and a value
    // nothing reads. Four cycles of the delay and nothing else.
    GAME_CYCLES(0x75ec, 4);
    GAME_CYCLES(0x75f0, 4);
    if (!--y)
      break;
    GAME_CYCLES(0x75f1, 1);
  }
}

/// $75F3 -- the lit half: draw the glyph and wait, reading the keyboard each
/// time the inner counter wraps. Returns the accepted key, or 0 if the wait
/// ran out or the key was rejected -- either way the blink starts again, and
/// no acceptable key is 0.
static uint8_t edit_key_prompt(uint8_t slot) {
  GAME_CYCLES(0x75f3, 23);
  s_x = slot;
  ram_poke(0x0024, slot_col(slot));
  ram_poke(0x0025, slot_row(slot));
  rom_fc68(0x7601);

  GAME_CYCLES(0x7602, 13);
  s_x = slot;
  const uint8_t glyph = slot_glyph(slot);
  s_a = glyph;
  s_status_not_z = glyph;
  s_status_n = (glyph & 0x80);
  rom_cout(0x7609);

  GAME_CYCLES(0x760a, 2);
  uint8_t x = cout_left_x();
  uint8_t y = 0;
  for (;;) {
    GAME_CYCLES(0x760c, 4);
    if (--x) {
      GAME_CYCLES(0x760d, 1);
      continue;
    }

    GAME_CYCLES_COORD(0x760f, 6);
    const uint8_t key = io_peek(0xc000);
    if (key & 0x80) {
      GAME_CYCLES(0x7612, 1);
      GAME_CYCLES(0x761c, 8);
      io_poke(0xc010, key); // clear the strobe

      // Anything from $A1 up -- every printable key -- plus the two arrows.
      // The carry the three compares leave is not written back: the caller's
      // next act on it is $7582's `CPX #$06`, which sets it.
      if (key >= 0xa1) {
        GAME_CYCLES(0x7621, 1);
        return key;
      }
      GAME_CYCLES(0x7623, 4);
      if (key == 0x88) { // left arrow
        GAME_CYCLES(0x7625, 1);
        return key;
      }
      GAME_CYCLES(0x7627, 4);
      if (key == 0x95) { // right arrow
        GAME_CYCLES(0x7629, 1);
        return key;
      }
      GAME_CYCLES(0x762b, 6);
      return 0;
    }

    GAME_CYCLES(0x7614, 4);
    if (--y) {
      GAME_CYCLES(0x7615, 1);
      continue;
    }
    GAME_CYCLES(0x7617, 6);
    return 0;
  }
}

uint8_t game_edit_key_native(uint8_t slot) {
  uint8_t key;
  do {
    edit_key_blank(slot);
    key = edit_key_prompt(slot);
  } while (!key);

  GAME_CYCLES(0x7630, 9);
  return key;
}

/* ========================================================================== */
/* $6BFB -- the falling tone                                                  */
/* ========================================================================== */

/// The whole tone is four bytes. $6C46 is the period, and doubles as the
/// on/off switch: game_mark_head raises it to 1 when the head moves and
/// game_draw_playfield clears it, so the sound follows the snake and stops
/// with it.
static uint8_t tone_period(void) {
  return ram_peek(0x6c46);
}

static void set_tone_period(uint8_t v) {
  ram_poke(0x6c46, v);
}

/// Passes left before the next click.
static uint8_t tone_countdown(void) {
  return ram_peek(0x6c47);
}

static void set_tone_countdown(uint8_t v) {
  ram_poke(0x6c47, v);
}

/// Where the click goes, as the low byte of the soft switch: $C030 is the
/// speaker and $C020 the cassette output, which nobody can hear. Muting is
/// therefore a store rather than a branch, and the click itself is one indexed
/// read -- see the $7642 header for why that shape was chosen.
static void set_click_port(uint8_t lo) {
  ram_poke(0x6c49, lo);
}

/// $69C2 -- toggled by Ctrl-S at $69B9.
static bool sound_muted(void) {
  return ram_peek(0x69c2) != 0;
}

void game_tick_sound_native(void) {
  GAME_CYCLES(0x6bfb, 6);
  ram_poke(0x6c48, 0x14); // twenty passes

  for (;;) {
    GAME_CYCLES(0x6c00, 6);
    const uint8_t period = tone_period();
    if (period) {
      GAME_CYCLES(0x6c05, 4);
      if (period < 0x80) {
        GAME_CYCLES(0x6c09, 8);
        const uint8_t left = (uint8_t)(tone_countdown() - 1);
        set_tone_countdown(left);
        if (!left) {
          GAME_CYCLES(0x6c0e, 28);
          const uint8_t port = ram_peek(0x6c49);
          s_y = port; // live out of this routine; the click is `LDA $C000,Y`
          peek((uint16_t)(0xc000 + port));

          // Two INC $6C46: every click lengthens the period, so the pitch
          // falls for as long as the head keeps moving.
          set_tone_period((uint8_t)(tone_period() + 2));
          set_tone_countdown(tone_period());
        } else {
          GAME_CYCLES(0x6c0c, 1);
        }
      } else {
        GAME_CYCLES(0x6c07, 1);
      }
    } else {
      GAME_CYCLES(0x6c03, 1);
    }

    GAME_CYCLES(0x6c20, 8);
    if (tone_period() >= 0x80) {
      // Fallen off the bottom of the range: silence until something restarts
      // it. $80 is reached from below in steps of two, so this is the end of
      // one slide rather than a wrap.
      GAME_CYCLES(0x6c27, 6);
      set_tone_period(0x00);
    } else {
      GAME_CYCLES(0x6c25, 1);
    }

    // Chosen afresh every pass, and defaulting to inaudible.
    GAME_CYCLES(0x6c2c, 12);
    set_click_port(0x20);
    if (!attract_mode()) {
      GAME_CYCLES(0x6c36, 6);
      if (!sound_muted()) {
        GAME_CYCLES(0x6c3b, 6);
        set_click_port(0x30);
      } else {
        GAME_CYCLES(0x6c39, 1);
      }
    } else {
      GAME_CYCLES(0x6c34, 1);
    }

    GAME_CYCLES(0x6c40, 8);
    const uint8_t left = (uint8_t)(ram_peek(0x6c48) - 1);
    ram_poke(0x6c48, left);
    if (!left)
      break;
    GAME_CYCLES(0x6c43, 1);
  }
  GAME_CYCLES(0x6c45, 6);
}
