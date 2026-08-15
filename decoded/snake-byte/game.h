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

/* --- $7000/$7019/$7024: the screen-script primitives ---------------------- */

/// $7019 -- fetch the next byte of the display list into A and advance the
/// $0A/$0B pointer.
void game_next_byte(uint16_t ret_addr);

/// $7024 -- set the lo-res colour from the ink flag in Z: black if zero,
/// grey otherwise. Tail-calls the ROM's SETCOL.
void game_set_ink(uint16_t ret_addr);

/// $7000 -- plot a vertical run on the lo-res occupancy map, from row $03
/// through row $08 inclusive at column $02, leaving $03 unchanged.
void game_lores_vline(uint16_t ret_addr);

/* --- $702B/$71F3/$7226/$7267: the score ---------------------------------- */

/// $71F3 -- print one BCD byte as two digits, suppressing leading zeros via
/// the $002C flag.
void game_print_bcd(uint16_t ret_addr);

/// $7226 -- print '0' if $002C shows no digit was printed. Called once at the
/// end of a multi-byte number.
void game_print_zero_if_blank(uint16_t ret_addr);

/// $7267 -- add $71CC:$71CB to the four-byte BCD score at $7252.
void game_add_score(uint16_t ret_addr);

/// $702B -- zero hi-res page 1, $2000 through $3FFF.
void game_clear_hgr(uint16_t ret_addr);

/* --- $6B93/$6C4B --------------------------------------------------------- */

/// $6C4B -- the next pseudo-random byte, $00-$7F, read out of the game's own
/// memory through the $0E/$0F pointer.
void game_rand_byte(uint16_t ret_addr);

/// $6B93 -- load the shape for $00 and merge it into the cell at row $03,
/// column $02, setting bits rather than replacing the byte.
void game_plot_shape_merge(uint16_t ret_addr);

/// $7045 -- clear the screen, draw the border, then interpret the current
/// level's display list at $8000. See game.c for the opcodes.
void game_draw_playfield(uint16_t ret_addr);

/* --- snake state and scoring setup --------------------------------------- */

/// $6641 -- point CSWL/CSWH at $664A so COUT reaches game_cout_hook.
void game_install_cout_hook(uint16_t ret_addr);

/// $660F -- reset the snake: head column from A, direction state to defaults.
void game_reset_snake(uint16_t ret_addr);

/// $6BEF -- PLOT the head onto the lo-res map and raise $0305 and $6C46.
void game_mark_head(uint16_t ret_addr);

/// $6BDA -- draw the caller's cell, merging shape 1 over it when $0305 is set.
void game_draw_head(uint16_t ret_addr);

/// $71CD -- set the per-apple score at $71CB/$71CC to $71C8[difficulty] times
/// the level, in BCD.
void game_set_apple_value(uint16_t ret_addr);
