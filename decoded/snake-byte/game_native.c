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
