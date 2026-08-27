/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// Hand-written replacements for the Apple II ROM entry points that Snake Byte
/// calls. The addresses listed here are also listed in `rom.externs`, which is
/// passed to the decompiler via `--extern-routines=`. That makes the decompiler
/// emit a declaration but no body for each of them, so the definitions in
/// `a2rom.c` take over.
///
/// All routines follow the calling convention of the generated code:
///   - `ret_addr` is the "fake return address" pushed on entry, mimicking JSR.
///     Zero means a tail call: nothing is pushed and nothing is popped, and the
///     6502 return address that is already on the stack belongs to the caller.
///   - CPU state is not passed or returned; it lives in the globals of
///     `apple2tc/system2-inc.h` (`s_a`, `s_x`, `s_y`, `s_status_*`, `s_ram`).
///
/// IMPORTANT: `a2rom.c` is *not* a standalone translation unit. See the comment
/// at the top of that file.

#pragma once

#include <stdint.h>

/// $F800 PLOT. Plot a lo-res block at column Y, row A. Trashes A, preserves Y.
void rom_plot(uint16_t ret_addr);

/// $F819 HLINE. Draw a horizontal lo-res line at row A from column Y to the
/// column in $2C.
void rom_hline(uint16_t ret_addr);

/// $F864 SETCOL. Set the lo-res color to A (low nibble), replicated into both
/// nibbles of $30.
void rom_setcol(uint16_t ret_addr);

/// $F871 SCRN. Read the lo-res block at column Y, row A; returns the color in A.
void rom_scrn(uint16_t ret_addr);

/// $FC58 HOME. Clear the text window and move the cursor to its top left.
void rom_home(uint16_t ret_addr);

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
void rom_fc68(uint16_t ret_addr);

/// $FDED COUT. Output the character in A through the output vector CSWL/CSWH
/// at $36/$37. See the extensive comment in `a2rom.c`: only the ROM COUT1
/// ($FDF0) target is implemented; any other target aborts loudly.
void rom_cout(uint16_t ret_addr);

/// $FE89 SETKBD. Reset the input vector KSWL/KSWH ($38/$39) to the keyboard.
void rom_setkbd(uint16_t ret_addr);

/// $FE93 SETVID. Reset the output vector CSWL/CSWH ($36/$37) to the screen,
/// i.e. to COUT1 at $FDF0.
void rom_setvid(uint16_t ret_addr);
