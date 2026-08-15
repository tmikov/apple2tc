/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdint.h>

/// \file
/// Snake Byte routines decompiled by hand, because apple2tc could not reach
/// them. See game.c for why each one is here.
///
/// Like a2rom.c, game.c is textually included into the generated translation
/// unit and is never compiled on its own.

/// $664A -- the game's own COUT handler, a hi-res text renderer.
void game_cout_hook(uint16_t ret_addr);

/* --- $60E4/$60E7/$6127: the hi-res cell plotter -------------------------- */

/// $6127 -- load the four scanline masks for shape $00 into $6060.
void game_load_shape(uint16_t ret_addr);

/// $60E7 -- draw the currently-loaded shape into the cell at row $03,
/// column $02, in ink $01.
void game_draw_cell(uint16_t ret_addr);

/// $60E4 -- load the shape for $00, then draw it. The form nearly every
/// caller uses.
void game_plot_shape(uint16_t ret_addr);

/// $6148 -- plot a horizontal run of cells, from column $02 through column
/// $08 inclusive, along row $03.
void game_plot_hline(uint16_t ret_addr);

/// $615A -- plot a vertical run of cells, from row $03 through row $08
/// inclusive, down column $02.
void game_plot_vline(uint16_t ret_addr);
