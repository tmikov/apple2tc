/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The Apple II ROM entry points this game calls, and the monitor's own zero
/// page that they work on.
///
/// The two layers call each other at exactly one point in each direction. The
/// game installs its own hi-res text renderer in the ROM's output vector, so
/// `rom_cout` dispatches *up* into `game_cout_hook`; that renderer then hands
/// the character *down* to `rom_cout1` once it has drawn its glyph. Both are
/// declared here, on each side of that boundary.

#pragma once

#include <stdbool.h>
#include <stdint.h>

/* --- The monitor's zero page ---------------------------------------------- */
/// The Apple II monitor's own zero page. Not the game's variables: they belong
/// to the ROM routines it calls, and the game writes them only to pass
/// arguments -- a cursor position before COUT, a colour before PLOT, a
/// right-hand end before HLINE. The names are the Apple II's own, because
/// every reference to this hardware uses them.
///
/// Addresses, not byte pairs. The 6502 splits a pointer into two bytes because
/// that is all it can address; nothing here has two halves meaning different
/// things. Where a routine really does write one half -- VTABZ rewrites only
/// the low byte -- the code says so with a mask or a shift.
extern struct Monitor {
  uint8_t wndlft, wndwdth, wndtop, wndbtm; ///< the text window
  uint16_t gbas;    ///< lo-res line base, from GBASCALC and V2
  uint16_t bas;     ///< text line base, from BASCALC and CV
  uint16_t bas2;    ///< the scroll's destination line
  uint8_t v2;       ///< VLINE's bottom row
  uint8_t mask;     ///< which nibble of a lo-res byte a PLOT touches
  uint8_t color;    ///< the lo-res colour, both nibbles
  uint8_t invflg;   ///< COUT1 ANDs the character with this: $FF normal
  uint16_t csw;     ///< the character output vector the game repoints
  uint16_t ksw;     ///< the character input vector; nothing reads it
  uint8_t a2l;      ///< SETKBD/SETVID scratch

  /// $002C, and still one byte doing two jobs.
  ///
  /// To the ROM it is H2, the right-hand end of a lo-res HLINE. To
  /// game_print_bcd it is the flag saying a digit has been printed, so
  /// game_print_zero_if_blank knows whether the field came out empty. The two
  /// never overlap -- nothing draws while a number is being printed -- and
  /// draw_border actually *relies* on the H2 half persisting from
  /// wipe_occupancy_map ("$2C is still $27 from the wipe"), so the storage
  /// stays shared. Splitting it would need that argument made in both
  /// directions; the accessors below carry the meaning instead.
  uint8_t h2;

  /// CH and CV, the text cursor, and the only two of the monitor's bytes that
  /// anything still computes with: BASCALC turns CV into a line base, CLREOL
  /// and COUT1 step CH along a line, and the game sets both before every
  /// field it prints. Initialised from the entry snapshot.
  uint8_t ch, cv;
} s_mon;

/* ========================================================================== *
 * Apple II ROM entry points -- declarations                                *
 * ========================================================================== */

/*
 * The Apple II ROM entry points the game calls, as ordinary C functions:
 * parameters and return values, not A/X/Y and the status flags. Each keeps its
 * canonical address and monitor name, because that is how every Apple II
 * reference identifies them.
 */

/// $F800 PLOT. Plot a lo-res block at column Y, row A. Trashes A, preserves Y.
void rom_plot(uint8_t row, uint8_t col);

/// $F819 HLINE. Draw a horizontal lo-res line at row A from column Y to the
/// column in $2C.
void rom_hline(uint8_t row, uint8_t from_col);

/// $F864 SETCOL. Set the lo-res color to A (low nibble), replicated into both
/// nibbles of $30.
void rom_setcol(uint8_t ink);

/// $F871 SCRN. Read the lo-res block at column Y, row A; returns the color in A.
uint8_t rom_scrn(uint8_t row, uint8_t col);

/// $FC58 HOME. Clear the text window and move the cursor to its top left.
void rom_home(void);

/// $FC68. The tail of the ROM's LF handling, not a documented entry point under
/// a familiar name:
///
///     FC68: LDA CV / CMP WNDBTM / BCC VTABZ
///     FC6E: DEC CV / ... scroll the window up one line, clear the bottom
///
/// So it recomputes BASL/BASH from CV, and only scrolls when CV has run past the
/// bottom of the window. COUT1 reaches it by falling through $FC66, but the game
/// also calls it directly as a VTAB -- $7590 and $75D1 store CH/CV and JSR here,
/// with CV always well under WNDBTM ($18), so the scroll half never runs.
void rom_fc68(void);

/// $FDED COUT. Output the character in A through the output vector CSWL/CSWH
/// at $36/$37. See the extensive comment in `a2rom.c`: only the ROM COUT1
/// ($FDF0) target is implemented; any other target aborts loudly.
void rom_cout(uint8_t ch);

/// $FE89 SETKBD. Reset the input vector KSWL/KSWH ($38/$39) to the keyboard.
void rom_setkbd(void);

/// $FE93 SETVID. Reset the output vector CSWL/CSWH ($36/$37) to the screen,
/// i.e. to COUT1 at $FDF0.
void rom_setvid(void);

uint8_t rom_bascalc(uint8_t line, bool *carry_out);
bool rom_vtabz(uint8_t line);
void rom_clreol(void);
bool rom_clreolz(uint8_t col);
void rom_wait(uint8_t n);

/// $FDF0 COUT1. The ROM's own character output, which the game's hi-res
/// renderer falls back to for control characters and calls when it is done.
void rom_cout1(uint8_t ch);

/// Supplied by the game: its hi-res text renderer, reached when the output
/// vector has been repointed at it. See rom_cout.
void game_cout_hook(uint8_t ch);
