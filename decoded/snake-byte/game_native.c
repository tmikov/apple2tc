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
