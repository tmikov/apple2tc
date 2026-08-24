/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// Snake Byte, decompiled: the whole program, in one translation unit,
/// sharing nothing.
///
/// This is the artifact the exercise is for. It enters at the game's own entry
/// point, $3750, with the machine state the Apple II boot would have produced;
/// it contains no Applesoft ROM code and no decompiler-generated code.
///
/// Why one file
/// ------------
/// Two reasons, and the second is the important one.
///
/// The mechanical reason is that `apple2tc/system2-inc.h` does not merely
/// declare the emulated machine, it *defines* it, and nearly all of it has
/// internal linkage -- the registers, the flags, `s_ram`, the cycle counters,
/// the access helpers. Anything implementing a 6502 routine has to be in the
/// same translation unit as that.
///
/// The real reason is ownership. The other four targets in this directory are
/// scaffolding: `snake-bytec1-ext` boots the machine and is the control this
/// one is checked against, `snake-bytec1` is the control for *that*,
/// `snake-byte-easyc1-ext` is a fixture, and `snake-byte` is history. While
/// this file shared game.c and game_native.c with them, every change here had
/// to be safe for builds that still run a generated dispatch over the same
/// addresses -- which is a real constraint and cost a file split already. It
/// does not share anything now. Change it freely; the gate says whether the
/// game still behaves.
///
/// What is still in here that is not the game
/// ------------------------------------------
///   - `s_mem_3750`, the game's own binary image. Its data -- level scripts,
///     the font, the tables -- and it stays.
///   - `s_mem_d000`, the Apple II ROM image. Not for its code: the death pause
///     reads delay lengths out of ROM *as data* at $E000.
///   - the Apple II ROM's own entry points, as C. They are the machine's, not
///     the game's, but they are decompiled on the same terms as everything
///     else here: there is no category of code in this file that belongs to
///     someone else. The last five still shaped like the decompiler left them
///     -- BASCALC, VTABZ, CLREOL, CLREOLZ and WAIT -- were converted on
///     2026-08-24; what is still `bb_N:` and `goto` is the remaining entry
///     points, which is the same job unfinished.
///   - the entry state, $0000-$0802 plus registers and one soft switch,
///     captured with --snapshot-at. See make-entry-state.sh.
///
/// The first two are 2,020 lines of hex between them, so they live in
/// game-image.inc and rom-image.inc and are `#include`d below. That is a
/// concession to reading the file, not a retreat from owning it: nothing else
/// includes them, they define statics and so can be included only once, and
/// the split was verified by compiling before and after -- every non-debug
/// section identical, and the emitted code differing in exactly the two
/// `__LINE__` immediates the shift moved.
///
/// How it is checked
/// -----------------
/// probe-acceptance.sh runs this against snake-bytec1-ext and requires the two
/// to agree from $3750 onward: every block head that is still probed, and the
/// screen at every in-game sample. The screen check is the one that survives
/// as more of this file turns into ordinary C.

#include <stdint.h>

#include "apple2tc/a2host_api.h"
#include "apple2tc/a2io.h"


/* ========================================================================== *
 * The machine, the images, and what is left of the ROM                     *
 * ========================================================================== */

// Loaded binary at [$3750..$854E]
// --code-at: 3 asserted edges applied
// Loaded segment [$00B1..$00C8]
// 293 new runtime blocks added
// code labels: 1673
// data labels: 284

#include "apple2tc/system2-inc.h"

/// The zero-page fragment $00B1-$00C8 the run data carried: the six-byte
/// CHRGET routine Applesoft assembles there at boot. Small enough to read,
/// so unlike the other two images it stays in the file.
static const uint8_t s_mem_00b1[0x0018] = {
  0xE6, 0xB8, 0xD0, 0x02, 0xE6, 0xB9, 0xAD, 0x06, 0x02, 0xC9, 0x3A, 0xB0, 0x0A, 0xC9, 0x20, 0xF0,
  0xEF, 0x38, 0xE9, 0x30, 0x38, 0xE9, 0xD0, 0x60
};

#include "game-image.inc"
#include "rom-image.inc"

/// Put the three images into RAM: the zero-page fragment the run data carried,
/// the game itself, and the Apple II ROM. Named for what it does -- it used to
/// be the whole of init_emulated(), which now has state to install as well.
static void load_images(void) {
  memcpy(s_ram + 0x00b1, s_mem_00b1, 0x0018);
  memcpy(s_ram + 0x3750, s_mem_3750, 0x4dff);
  memcpy(s_ram + 0xd000, s_mem_d000, 0x3000);
}

static inline uint8_t ovf8(uint8_t res, uint8_t a, uint8_t b) {
  return (~(a ^ b) & (a ^ res)) >> 7;
}
static uint16_t adc_dec16(uint8_t a, uint8_t b, uint8_t cf) {
  struct ResAndStatus res = adc_decimal(a, b, cf);
  return res.result | (res.status << 8);
}
static uint16_t sbc_dec16(uint8_t a, uint8_t b, uint8_t cf) {
  struct ResAndStatus res = sbc_decimal(a, b, cf);
  return res.result | (res.status << 8);
}

/* --- The monitor's zero page ---------------------------------------------- */
/*
 * $0020-$003E, with the Apple II's own names. These are not the game's
 * variables: they belong to the ROM routines it calls, and the game writes
 * them only to pass arguments -- a cursor position before COUT, a colour
 * before PLOT, a right-hand end before HLINE. Keeping the monitor's spelling
 * is deliberate; every Apple II reference uses it, and a2rom.h's prose
 * already does.
 */
/* The monitor's own state, out of emulated RAM on the same terms as the game's.
   Names are the Apple II's, because every reference uses them and a2rom.h's
   prose already did. */
static uint8_t s_wndlft;  ///< text window: left, width, top, bottom
static uint8_t s_wndwdth;
static uint8_t s_wndtop;
static uint8_t s_wndbtm;
static uint8_t s_gbasl, s_gbash; ///< lo-res line base, from GBASCALC and V2
static uint8_t s_basl, s_bash;   ///< text line base, from BASCALC and CV
static uint8_t s_bas2l, s_bas2h; ///< the scroll's destination line
static uint8_t s_v2;      ///< VLINE's bottom row
static uint8_t s_mask;    ///< which nibble of a lo-res byte a PLOT touches
static uint8_t s_color;   ///< the lo-res colour, both nibbles
static uint8_t s_invflg;  ///< COUT1 ANDs the character with this: $FF normal
static uint8_t s_ysav1;   ///< where COUT1 parks Y
static uint8_t s_cswl, s_cswh; ///< the character output vector the game repoints
static uint8_t s_kswl, s_kswh; ///< the character input vector; nothing reads it
static uint8_t s_a2l;     ///< SETKBD/SETVID scratch

/// $002C, and still one byte doing two jobs.
///
/// To the ROM it is H2, the right-hand end of a lo-res HLINE. To
/// game_print_bcd it is the flag saying a digit has been printed, so
/// game_print_zero_if_blank knows whether the field came out empty. The two
/// never overlap -- nothing draws while a number is being printed -- and
/// draw_border actually *relies* on the H2 half persisting from
/// wipe_occupancy_map ("$2C is still $27 from the wipe"), so the storage stays
/// shared. Splitting it would need that argument made in both directions; the
/// accessors below carry the meaning instead.
static uint8_t s_h2;

/// The three pointer pairs, low byte first, as ram_peek16al read them.
static inline uint16_t gbas16(void) { return (uint16_t)(s_gbasl | (s_gbash << 8)); }
static inline uint16_t bas16(void) { return (uint16_t)(s_basl | (s_bash << 8)); }
static inline uint16_t bas2_16(void) { return (uint16_t)(s_bas2l | (s_bas2h << 8)); }
static inline uint16_t csw16(void) { return (uint16_t)(s_cswl | (s_cswh << 8)); }

/// $0024/$0025 -- CH and CV, the text cursor, out of emulated RAM.
///
/// The last two addresses in this block that anything still *computes* with.
/// The ROM's BASCALC turns CV into BASL/BASH, CLREOL and COUT1 step CH along a
/// line, and the game sets both before every field it prints. All of that is
/// C now; the bytes at $24/$25 are no longer read by anybody.
///
/// Moved even though the ROM owns them, because the ROM is being decompiled
/// too -- a routine that is still emitted as a switch over block ids is not a
/// different kind of code, only a less finished one. There is no version of
/// this artifact in which the monitor keeps its state in a 64K array.
///
/// Initialised from the entry snapshot, like the plotter's block.
static uint8_t s_ch, s_cv;

/// A cell on the 40x48 playfield grid.
typedef struct {
  uint8_t col;
  uint8_t row;
} Cell;

void game_cold_start(void);
void rom_plot(uint16_t ret_addr);
void rom_hline(uint16_t ret_addr);
void rom_setcol(uint16_t ret_addr);
void rom_scrn(uint16_t ret_addr);
void rom_home(uint16_t ret_addr);
void rom_fc68(uint16_t ret_addr);
void rom_cout(uint16_t ret_addr);
void rom_setkbd(uint16_t ret_addr);
void rom_setvid(uint16_t ret_addr);
void game_load_shape(uint16_t ret_addr);
void game_draw_cell(uint16_t ret_addr, uint8_t ink, Cell c);
void game_plot_hline(uint16_t ret_addr, Cell c, uint8_t to_col);
void game_plot_vline(uint16_t ret_addr, Cell c, uint8_t to_row);
void game_start_round(uint16_t ret_addr);
void game_play_loop(uint16_t ret_addr);
void game_setup(uint16_t ret_addr);
void game_set_ink(uint16_t ret_addr);
void game_lores_vline(uint16_t ret_addr, Cell c, uint8_t to_row);
void game_print_bcd(uint16_t ret_addr);
void game_clear_hgr(uint16_t ret_addr);
void game_plot_shape_merge(uint16_t ret_addr, uint8_t ink, Cell c);
void game_draw_playfield(uint16_t ret_addr);
void game_install_cout_hook(uint16_t ret_addr);
void game_start_life_adapter(uint16_t ret_addr);
void game_show_key(uint16_t ret_addr);
void game_move_ok(uint16_t ret_addr);
void game_move_bouncer(uint16_t ret_addr);
void game_update_high_score(uint16_t ret_addr);
void game_step_bouncers(uint16_t ret_addr);
void game_find_apple(uint16_t ret_addr);
void game_pause_or_toggle_sound(uint16_t ret_addr);
void game_edit_key(uint16_t ret_addr);
void game_read_direction(uint16_t ret_addr);
void game_print_inline_str(uint16_t ret_addr);
void rom_bascalc(uint16_t ret_addr);
void rom_vtabz(uint16_t ret_addr);
void rom_clreol(uint16_t ret_addr);
void rom_clreolz(uint16_t ret_addr);
void rom_wait(uint16_t ret_addr);

/// The program starts here. There used to be a func_t001 in between -- the
/// generated dispatch -- and by the end it was a stub that called this.
static void emulated_entry_point(void) {
  game_cold_start();
}















































































/// $FBC1 BASCALC. Turn a text line number in A into that line's base address
/// in BASL/BASH.
///
/// The Apple II text screen is three interleaved bands of eight lines. The ROM
/// does not use a table: bit 0 of the line picks the band's second half, bits
/// 1-2 pick the band, and bits 3-4 pick the line within it, folded in by the
/// `ADC #$7F` and the two shifts below.
///
/// A and the flags are left as the original leaves them, because COUT1 and
/// CLREOLZ both read BASL straight afterwards and the monitor's callers are
/// not all in this file's control.
void rom_bascalc(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$FBC1*/ CYCLES(0xfbc1, 20);
  const uint8_t line = s_a;
  // LSR: the carry is the line's low bit, and it is what decides the ADC below.
  const uint8_t odd = line & 0x01;
  s_status_c = odd;
  s_bash = (uint8_t)(((line >> 1) & 0x03) | 0x04);
  s_a = line & 0x18;

  if (!odd) {
    /*$FBCC*/ CYCLES_EDGE(0xfbcc, 1);
  } else {
    /*$FBCE*/ CYCLES(0xfbce, 2);
    // ADC #$7F with carry set, i.e. +$80: the second half of the band.
    if (!s_status_d) {
      const uint16_t r = (uint16_t)(s_a + 0x007f) + s_status_c;
      s_status_v = ovf8((uint8_t)r, s_a, 0x7f);
      s_a = (uint8_t)r;
    } else {
      const uint16_t r = adc_dec16(s_a, 0x7f, s_status_c);
      s_a = (uint8_t)r;
      s_status_v = (((uint8_t)(r >> 8) & 0x40) != 0);
    }
  }
  branchTarget = true;

  /*$FBD0*/ CYCLES(0xfbd0, 19);
  s_basl = s_a;
  // ASL twice, then OR the original back in. The second shift's carry out is
  // the one the original leaves behind.
  const uint16_t shifted = (uint16_t)(s_a << 0x02);
  s_status_c = (uint8_t)((shifted & 0x01ff) >> 8);
  const uint8_t addr_lo = (uint8_t)shifted | s_basl;
  s_status_not_z = addr_lo;
  s_status_n = (addr_lo & 0x80);
  s_a = addr_lo;
  s_basl = addr_lo;

  (void)branchTarget;
  if (ret_addr) pop16();
}

/// $FC24 VTABZ. BASCALC for the line in A, then shift the base right by the
/// window's left edge, so BASL points at the first column of the window rather
/// than of the screen.
void rom_vtabz(uint16_t ret_addr) {
  bool branchTarget = true;
  (void)branchTarget;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$FC24*/ CYCLES(0xfc24, 6);
  rom_bascalc(0xfc26);

  /*$FC27*/ CYCLES(0xfc27, 6);
  if (!s_status_d) {
    const uint8_t left = s_wndlft;
    const uint16_t r = ((uint16_t)s_a + left) + s_status_c;
    s_status_c = (uint8_t)(r >> 8);
    s_status_v = ovf8((uint8_t)r, s_a, left);
    s_a = (uint8_t)r;
    s_status_not_z = s_a;
    s_status_n = (s_a & 0x80);
  } else {
    const uint16_t r = adc_dec16(s_a, s_wndlft, s_status_c);
    s_a = (uint8_t)r;
    const uint8_t flags = (uint8_t)(r >> 8);
    s_status_c = (flags & 0x01);
    s_status_not_z = (uint8_t)(~flags & 2);
    s_status_v = ((flags & 0x40) != 0);
    s_status_n = (flags & 0x80);
  }
  s_basl = s_a;

  /*$FC2B*/ CYCLES(0xfc2b, 6);
  if (ret_addr) pop16();
}

/// $FC9C CLREOL. Blank from the cursor to the right edge of the window.
///
/// **Decoded from the binary and not verified.** Neither cold scenario reaches
/// it: probed at $FC9C, it fires 0 times in play and 0 in hires, because its
/// only caller is $FC9A on the scroll path and nothing in the recordings
/// scrolls. The body is three instructions and its tail call is the routine
/// below, which *is* exercised, so what is unchecked is the LDY and the jump.
void rom_clreol(uint16_t ret_addr) {
  bool branchTarget = true;
  (void)branchTarget;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$FC9C*/ CYCLES(0xfc9c, 3);
  s_y = s_ch;
  rom_clreolz(0x0000); // JMP -- a tail call.

  if (ret_addr) pop16();
}

/// $FC9E CLREOLZ. The same, from column Y rather than from the cursor. Writes
/// spaces up to but not including the window's width.
///
/// Y is left at the width and the carry set, which is how the original exits
/// the loop; both are still written because the monitor's callers read them.
void rom_clreolz(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$FC9E*/ CYCLES(0xfc9e, 2);
  s_a = 0xa0; // a space, high bit set

  for (;;) {
    /*$FCA0*/ CYCLES(0xfca0, 13);
    const uint8_t col = s_y;
    poke((uint16_t)(bas16() + col), s_a);

    const uint8_t next = (uint8_t)(col + 1);
    s_y = next;

    const uint8_t width = s_wndwdth;
    s_status_not_z = (next != width);
    s_status_c = (next >= width);
    s_status_n = (uint8_t)((uint8_t)(next - width) & 0x80);
    branchTarget = true;
    if (next >= width)
      break;

    /*$FCA5*/ CYCLES_EDGE(0xfca5, 1);
    branchTarget = true;
  }

  /*$FCA7*/ CYCLES(0xfca7, 6);
  (void)branchTarget;
  if (ret_addr) pop16();
}

/// $FCA8 WAIT. The monitor's delay: two nested `SBC #$01 / BNE` loops around
/// the value in A, which BELL uses to time its tone.
///
/// **Decoded from the binary and not verified.** Probed at $FCA8, it fires 0
/// times in both cold scenarios. Its only callers are $FBDF and $FBE6 inside
/// BELL1, and nothing in the recordings outputs a Ctrl-G. That was already
/// true of the version the decompiler emitted, so no coverage was lost here --
/// but an error in the transcription would not be caught either.
///
/// A comes back as 0 and the carry set. The inner loop counts A down to zero
/// from a copy on the stack, and the outer one counts the original down, so
/// the total is quadratic in A rather than linear.
void rom_wait(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$FCA8*/ CYCLES(0xfca8, 2);
  s_status_c = 0x01;

  for (;;) {
    /*$FCA9*/ CYCLES(0xfca9, 3);
    push8(s_a);

    // The inner loop: A down to zero, one SBC per pass.
    for (;;) {
      /*$FCAA*/ CYCLES(0xfcaa, 4);
      if (!s_status_d) {
        const uint16_t r = (uint16_t)(s_a - 0x0001) - (uint8_t)(0x01 - s_status_c);
        s_status_c = (uint8_t)(0x01 - ((uint8_t)(r >> 8) & 0x01));
        s_a = (uint8_t)r;
        s_status_not_z = s_a;
      } else {
        const uint16_t r = sbc_dec16(s_a, 0x01, s_status_c);
        s_a = (uint8_t)r;
        const uint8_t flags = (uint8_t)(r >> 8);
        s_status_c = (flags & 0x01);
        s_status_not_z = (uint8_t)(~flags & 2);
      }
      branchTarget = true;
      if (!s_status_not_z)
        break;
      /*$FCAC*/ CYCLES_EDGE(0xfcac, 1);
      branchTarget = true;
    }

    // The outer one: the copy off the stack, down by one.
    /*$FCAE*/ CYCLES(0xfcae, 8);
    s_a = pop8();
    if (!s_status_d) {
      const uint8_t before = s_a;
      const uint16_t r = (uint16_t)(before - 0x0001) - (uint8_t)(0x01 - s_status_c);
      s_status_c = (uint8_t)(0x01 - ((uint8_t)(r >> 8) & 0x01));
      s_status_v = ovf8((uint8_t)r, before, 0xfe);
      s_a = (uint8_t)r;
      s_status_not_z = s_a;
    } else {
      const uint16_t r = sbc_dec16(s_a, 0x01, s_status_c);
      s_a = (uint8_t)r;
      const uint8_t flags = (uint8_t)(r >> 8);
      s_status_c = (flags & 0x01);
      s_status_not_z = (uint8_t)(~flags & 2);
      s_status_v = ((flags & 0x40) != 0);
    }
    branchTarget = true;
    if (!s_status_not_z)
      break;
    /*$FCB1*/ CYCLES_EDGE(0xfcb1, 1);
    branchTarget = true;
  }

  /*$FCB3*/ CYCLES(0xfcb3, 6);
  (void)branchTarget;
  if (ret_addr) pop16();
}

/* ========================================================================== *
 * Apple II ROM entry points -- declarations                                *
 * ========================================================================== */

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

/* ========================================================================== *
 * The game as ordinary C -- declarations                                   *
 * ========================================================================== */

/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */



/// \file
/// Snake Byte as ordinary C: parameters, return values, structs, locals.
///
/// Where this fits
/// ---------------
/// `game.c` holds the game's routines in the shape the 6502 left them --
/// arguments in fixed zero-page addresses, results in A and the status flags,
/// an emulated stack, `CYCLES` at every block head. That shape is not a
/// stylistic choice: those routines are called by *generated* code, which
/// reads that machine state, so each one has to present the same ABI the
/// original did.
///
/// This file is the other end. Functions here take arguments and return
/// values, and know nothing about `s_a`, the flags, or the return address.
/// Each one is reached through a small adapter left behind in `game.c`, which
/// marshals the machine state in and out. As callers convert, adapters
/// disappear; `game.c` shrinks and this file grows, and the split between the
/// two is exactly the frontier.
///
/// What it costs, and when
/// -----------------------
/// Three oracles check this code, and they do not survive the conversion
/// equally:
///
///   the frame hashes (verify.sh) survive to the end -- they only look at
///     video memory, which is the machine's no matter who writes it.
///   the memory hashes (ram.probe) survive until a variable's *storage*
///     leaves emulated RAM. Until then an adapter can write everything back
///     before returning and the samples are unchanged.
///   the block-head trace dies first, per routine, the moment a branch moves
///     in here -- `CYCLES` cannot follow into real C without dragging the
///     block structure with it.
///
/// So the conversion order is not arbitrary. A routine with a single block
/// converts for free: its adapter keeps the one `CYCLES` and calls straight
/// in. A routine with internal branches costs its share of the trace, and
/// that is a deliberate trade to be made with the count in front of you, not
/// discovered afterwards.
///
/// Storage
/// -------
/// Variables still live at their original addresses, because generated code
/// and the unconverted half of `game.c` read them there. They are named here
/// and reached through accessors, so that when the storage does move, one
/// file changes and not every use.


/* --- Cycle accounting ----------------------------------------------------- */

/// Charge the cycles the original spent in a block, without registering a
/// probe site.
///
/// Timing cannot be dropped along with the block structure. A frame is a cycle
/// budget, so a converted routine that runs at a different speed moves every
/// later frame boundary and therefore every later frame hash -- the one oracle
/// meant to survive to the end. What is given up is only the *observability*
/// of the address: `CYCLES_EDGE` does not dispatch a probe, so the site leaves
/// the block-head trace. That is the middle rung of the ladder in this file's
/// header, and it is deliberate.
///
/// Spelled differently from `CYCLES` so the two cannot be confused, and so
/// probe-acceptance.sh can reject a plain `CYCLES` in this file -- one written
/// here would charge cycles and never be probed, which is a silent hole rather
/// than a declared trade.
#define GAME_CYCLES(addr, n) CYCLES_EDGE((addr), (n))

/// Charge the cycles and *keep* the probe, for the handful of addresses that
/// carry the input coordinate.
///
/// Replay does not stamp keystrokes on cycles -- it stamps them on a counter
/// the probe language increments at seven named addresses (see rec.probe).
/// Two of those, $6217 and $760F, sit inside routines this file is converting.
/// A converted site charges its cycles and drops its probe, so spelling one of
/// those GAME_CYCLES would stop the counter advancing there, and every key
/// stamped after that point would arrive at a different instant.
///
/// That is caught, but badly. The interpreter side of the comparison always
/// runs the original binary, so its counter never drifts; only the generated
/// side does, and the block-head trace then diverges. Measured: converting
/// $760F fails trace-ext on the play-hires scenario at line 640,983 of the
/// diff, with four addresses in the redefinition screen appearing on one side
/// and not the other. Nothing about that output says "the input coordinate
/// moved". verify.sh does not notice at all -- it replays the cycle-stamped
/// .keys files, which do not use the counter.
///
/// So the site keeps its probe, and the spelling says why at the call site.
/// probe-acceptance.sh asserts that every address written this way is one of
/// the coordinate's, which turns a 640,983-line diff into one line naming the
/// address.
///
/// The local is the emulator's assembly-trace plumbing: `CYCLES` consults a
/// `branchTarget` flag the generated dispatch keeps, to print one line per
/// block rather than one per instruction. Converted code has no block
/// structure for that question to be about, and a site spelled this way is a
/// block head by construction, so it answers yes and moves on.
#define GAME_CYCLES_COORD(addr, n) \
  do {                             \
    bool branchTarget = true;      \
    CYCLES((addr), (n));           \
    (void)branchTarget;            \
  } while (0)

/// Charge the cycles and keep the probe, because some *other* source still
/// names this address.
///
/// A converted routine normally takes its block heads out of the comparison
/// altogether: nothing probes them, on either engine, and the two agree by
/// saying nothing. That argument fails the moment another file still emits a
/// `CYCLES` for the same address, because then the interpreter reports it and
/// the generated build does not.
///
/// It has happened twice, for two different reasons. $6216 is an RTS shared by
/// two routines, one converted and one not. $720E is stranger: the low half of
/// $71F3 survives in the generated C as an orphan -- its only predecessors were
/// in the extern-replaced region, so nothing can reach it -- but it is still
/// text in the file, so it is still on the site list. Neither is visible in the
/// pinned count, which was right both times.
///
/// probe-acceptance.sh checks all three spellings against the site list built
/// from the other sources: an address here must be in it, an address on plain
/// GAME_CYCLES must not be, and GAME_CYCLES_COORD must be on the coordinate.
#define GAME_CYCLES_SHARED(addr, n) GAME_CYCLES_COORD((addr), (n))

/// Charge the cycles and keep the probe, because the address is where a
/// cross-build comparison lines the two traces up.
///
/// Exactly one address uses this, and it is $3750. probe-acceptance.sh checks
/// the cold-start build against the booting one, and the booting one's trace
/// has to have its boot prefix removed before the two are comparable. The only
/// way to find where that prefix ends is to look for the entry address, so the
/// entry address has to still be in the trace. Converting it to a plain
/// GAME_CYCLES takes it out and the alignment silently finds nothing.
///
/// Spelled apart from GAME_CYCLES_COORD, which it expands to, because the
/// reason is different and the two lints check different things.
#define GAME_CYCLES_ANCHOR(addr, n) GAME_CYCLES_COORD((addr), (n))

/* --- The bouncers --------------------------------------------------------- */

/// One of the two objects that ricochet around the playfield. The original
/// keeps them as four parallel pairs of bytes and copies one set into
/// $6633-$6636 before stepping it -- a hand-rolled calling convention, which
/// is why a struct fits so exactly.
typedef struct {
  uint8_t col;
  uint8_t row;
  /// Always +1 or -1. The original stores them as bytes and reflects with
  /// `EOR #$FE`, which swaps $01 and $FF.
  int8_t dx;
  int8_t dy;
} Bouncer;

/// Read bouncer \p i (0 or 1) out of emulated RAM.
Bouncer bouncer_load(int i);

/// Write bouncer \p i back to emulated RAM.
void bouncer_store(int i, Bouncer b);

/* --- Converted routines --------------------------------------------------- */

/// $660F -- start a life: put the snake's head in \p head_col and set both
/// bouncers going from opposite corners. Returns the value the original left
/// in A, which its one caller stores as the tail column.
uint8_t game_start_life(uint8_t head_col);

/// $6127 -- copy shape \p shape's four scanline masks into $6060, and return
/// the last one.
uint8_t game_load_shape_masks(uint8_t shape);

/// $6641 -- point the ROM's character-output vector at the game's own hi-res
/// handler, so every later COUT reaches game_cout_hook.
void game_install_cout_vector(void);

/// $64C8 -- step \p b one cell along its deltas and redraw it, reflecting off
/// whatever it hits.
void bouncer_step(Bouncer *b);

/// What $6AB8 decided about a candidate move. The original says all of this
/// in A and the Z flag; the adapter puts it back.
typedef enum {
  /// The target cell holds something other than empty or an apple.
  MOVE_TARGET_TAKEN,
  /// The target row is 0, the top border. Accepted, oddly -- but the border
  /// is occupied, so the check above normally rejects it first.
  MOVE_ROW_ZERO,
  /// Safe: at least one of the target's four neighbours is free.
  MOVE_OK,
  /// All four neighbours are occupied -- legal to enter, fatal next move.
  MOVE_DEAD_END,
} MoveVerdict;

/// $6AB8 -- judge a step in direction \p dir. \p cell_out receives what the
/// occupancy map held at the target, which the original leaves in A.
MoveVerdict snake_move_verdict(uint8_t dir, uint8_t *cell_out);

/// $728D -- copy the score at $7252 over the high score at $7256 if it beats
/// it, comparing BCD bytes most significant first.
void game_promote_high_score(void);

/// $6148 / $615A -- runs of hi-res cells along a row or down a column.
uint8_t game_plot_hline_native(uint8_t ink, Cell c, uint8_t to_col);
uint8_t game_plot_vline_native(uint8_t ink, Cell c, uint8_t to_row);

/// $7000 -- the lo-res half of a vertical run, restoring $03.
uint8_t game_lores_vline_native(Cell c, uint8_t to_row);

/// $6594 -- step the bouncers the difficulty calls for, then return the next
/// queued key.
uint8_t game_step_bouncers_native(void);

/// $60E7 -- draw the loaded shape into cell \p c in ink \p ink, replacing.
uint8_t game_draw_cell_native(uint8_t ink, Cell c);

/// $6B93 -- the same, merged into what is already there.
uint8_t game_merge_cell_native(uint8_t ink, Cell c);

/// $702B -- zero hi-res page 1.
void game_clear_hgr_native(void);

/// $7045 -- clear the screen, draw the border, and run the current level's
/// display list from $8000.
void game_draw_playfield_native(void);

/// $69C3 -- sweep columns outward from the snake for an apple, leaving the
/// answer at $6B3B/$6B3C.
void game_find_nearest_apple(void);

/// $6C75 -- turn \p key, the byte just taken off the ring, into the code the
/// game acts on: a direction, a joystick setting applied on the spot, or $00
/// for nothing. Reads the joystick itself when one is selected and the key
/// was not a direction.
uint8_t game_read_direction_native(uint8_t key);

/// $75D1 -- blink slot \p slot on the key-redefinition screen until the player
/// presses something it will accept, and return that key.
uint8_t game_edit_key_native(uint8_t slot);

/// $3750 -- the program's entry, and the outermost loop: relocate the level
/// data, initialise $0300-$0304, then new game -> level -> round -> life
/// forever. Never returns; the game has no way out.
void game_cold_start(void);

/// $69A9 -- ESC pauses until any key is pressed; Ctrl-S toggles the sound.
/// Every key the dispatch chain did not recognise arrives here and is ignored.
///
/// Returns the key the Ctrl-S test actually saw, which is \p key unless ESC
/// paused: the keypress that ends the pause replaces it and is tested in turn.
uint8_t game_pause_or_toggle_sound_native(uint8_t key);

/// $6BFB -- twenty passes of the falling tone that plays while the head moves.
void game_tick_sound_native(void);

/// $71F3 -- print \p byte as two decimal digits, dropping leading zeros.
void game_print_bcd_native(uint8_t byte);

/// $7226 -- print a single "0" if the number just printed was all zeros.
void game_print_zero_if_blank_native(void);

/// $7267 -- add the two-byte BCD value at $71CB to the four-byte score.
void game_add_score_native(void);

/// $7024 -- set the lo-res plot colour from an ink byte: 0 erases, anything
/// else draws.
void game_set_ink_native(uint8_t ink);

/// $7019 -- read the byte at the $000A pointer into A and advance it.
void game_next_byte_native(void);

/// $6C4B -- the game's pseudo-random byte, always $00-$7F.
uint8_t game_rand_byte_native(void);

/// $7642 -- put an apple on a free cell, by rejection sampling.
Cell game_place_apple_native(void);

/// $71CD -- recompute what one apple is worth for the current level.
void game_set_apple_value_native(void);

/// $6BEF -- plot the head on the occupancy map and flag it as newly there.
void game_mark_head_native(void);

/// $6BDA -- draw a cell, merging the head shape over it if the head is on it.
void game_draw_head_native(uint8_t ink, Cell c);

/// $7633 -- count one apple eaten, and make the noise for it.
void game_award_extra_life_native(void);

/// $60E4 -- load a shape and draw it into the current cell.
void game_plot_shape_native(uint8_t ink, Cell c);

/// $64A9 -- the rising-then-falling sweep an eaten apple makes.
void game_sound_sweep_native(void);

/// $7590 -- show \p key as slot \p slot's binding on the redefinition screen.
void game_show_key_native(uint8_t slot, uint8_t key);

/// $6B3D -- draw both side walls, and leave SCRN of the bottom-centre cell.
void game_draw_side_walls_native(void);

/// $6217 -- poll the keyboard and push what it finds into the ring at $623C.
void game_read_key_native(void);

/// $664A -- draw \p ch through the game's own hi-res font, then hand it on to
/// the ROM's COUT1 so the cursor still moves.
void game_cout_hook_native(uint8_t ch);

/// Why a life ended. The original says all of this in one byte at $6253, which
/// its caller at $7739 reads the moment the routine returns; game_play_loop's
/// adapter is what puts the byte back.
typedef enum {
  /// Reached the gate: column $14 of row 0. $6253 stays 0.
  LIFE_GATE,
  /// Moved onto an apple. $6253 is $0F.
  LIFE_APPLE,
  /// The quit key. $6253 is $FF.
  LIFE_QUIT,
  /// The level timer at $6255 ran out. $6253 is $FE.
  LIFE_TIMEOUT,
  /// Moved onto something solid. $6253 is that cell, which is what
  /// \p cell_out receives -- the caller distinguishes the cases by it.
  LIFE_CRASH,
} LifeEnd;

/// $6288 -- play one life: steer, move, draw and pace the snake until
/// something ends it. \p cell_out receives the occupancy byte the head landed
/// on, which is only meaningful for LIFE_CRASH.
LifeEnd game_play_loop_native(uint8_t *cell_out);

/// What the auto-steer decided. $6A32 answers in A, and $6288 re-examines the
/// answer as though it had been typed, so the two spellings are a key and a
/// direction number -- telling them apart is the high bit.
typedef enum {
  /// Carry on in the direction the snake is already going.
  STEER_STRAIGHT,
  /// Turn. \p key_out holds the absolute-direction key that achieves it.
  STEER_TURN,
  /// Nothing is safe. The original says this by discarding its own return
  /// address and jumping to $6315 in its caller, which is "carry on" reached
  /// without the caller's consent; see the alternate-exit transform in
  /// tools/apple2tc/routines.cpp.
  STEER_BOXED_IN,
} SteerChoice;

/// $6A32 -- steer toward the apple at $6B3B/$6B3C, trying candidate directions
/// in order of usefulness and taking the first that snake_move_verdict()
/// allows. Leaves the direction it settled on in $6B38, as the original does.
SteerChoice game_auto_steer(uint8_t *key_out);

/// $72CE -- draw the status panel: six labelled BCD fields in a 2x3 grid
/// across the bottom three text rows, then home the cursor.
void game_status_panel(void);

/// $78B3 -- the bonus screen: award twice the apple's value, draw a box over
/// the playfield, print BONUS and the amount, and hold it there.
void game_bonus_screen(void);

/// $6256 -- set up a life and hand over to the main loop: the snake as a
/// single cell at the bottom centre facing up, ten segments of growth owed,
/// the timer full, and the key ring empty.
void game_begin_life(void);

/// $7980 -- the setup screen: seed the random pointer, then either ask for a
/// difficulty (falling back to the demo if nobody answers) or run the key
/// redefinition screen.
void game_setup_screen(void);

/* ========================================================================== *
 * The 6502-shaped adapters -- declarations                                 *
 * ========================================================================== */

/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */



/// \file
/// Snake Byte routines decompiled by hand, because apple2tc could not reach
/// them. See game.c for why each one is here.
///
/// Like a2rom.c, game.c is textually included into the generated translation
/// unit and is never compiled on its own.

/// $664A -- the game's own COUT handler, a hi-res text renderer.
/// $7230 -- print the NUL-terminated string that follows the call.
void game_print_inline_str(uint16_t ret_addr);

void game_cout_hook(uint16_t ret_addr);

/* --- $60E4/$60E7/$6127: the hi-res cell plotter -------------------------- */

/// $6127 -- load the four scanline masks for shape $00 into $6060.
void game_load_shape(uint16_t ret_addr);

/// $60E7 -- draw the currently-loaded shape into the cell at row $03,
/// column $02, in ink $01.
void game_draw_cell(uint16_t ret_addr, uint8_t ink, Cell c);

/// $60E4 -- load the shape for $00, then draw it. The form nearly every
/// caller uses.

/// $6148 -- plot a horizontal run of cells, from column $02 through column
/// $08 inclusive, along row $03.
void game_plot_hline(uint16_t ret_addr, Cell c, uint8_t to_col);

/// $615A -- plot a vertical run of cells, from row $03 through row $08
/// inclusive, down column $02.
void game_plot_vline(uint16_t ret_addr, Cell c, uint8_t to_row);

/* --- $7000/$7019/$7024: the screen-script primitives ---------------------- */

/// $7019 -- fetch the next byte of the display list into A and advance the
/// $0A/$0B pointer.

/// $7024 -- set the lo-res colour from the ink flag in Z: black if zero,
/// grey otherwise. Tail-calls the ROM's SETCOL.
void game_set_ink(uint16_t ret_addr);

/// $7000 -- plot a vertical run on the lo-res occupancy map, from row $03
/// through row $08 inclusive at column $02, leaving $03 unchanged.
void game_lores_vline(uint16_t ret_addr, Cell c, uint8_t to_row);

/* --- $702B/$71F3/$7226/$7267: the score ---------------------------------- */

/// $71F3 -- print one BCD byte as two digits, suppressing leading zeros via
/// the $002C flag.
void game_print_bcd(uint16_t ret_addr);

/// $7226 -- print '0' if $002C shows no digit was printed. Called once at the
/// end of a multi-byte number.

/// $7267 -- add $71CC:$71CB to the four-byte BCD score at $7252.

/// $702B -- zero hi-res page 1, $2000 through $3FFF.
void game_clear_hgr(uint16_t ret_addr);

/* --- $6B93 --------------------------------------------------------------- */

/// $6B93 -- load the shape for $00 and merge it into the cell at row $03,
/// column $02, setting bits rather than replacing the byte.
void game_plot_shape_merge(uint16_t ret_addr, uint8_t ink, Cell c);

/// $7045 -- clear the screen, draw the border, then interpret the current
/// level's display list at $8000. See game.c for the opcodes.
void game_draw_playfield(uint16_t ret_addr);

/* --- snake state and scoring setup --------------------------------------- */

/// $6641 -- point CSWL/CSWH at $664A so COUT reaches game_cout_hook.
void game_install_cout_hook(uint16_t ret_addr);

/// $660F -- adapter for game_start_life(): head column in A, both bouncers
/// placed at opposite corners.
void game_start_life_adapter(uint16_t ret_addr);

/// $6BEF -- PLOT the head onto the lo-res map and raise $0305 and $6C46.

/// $6BDA -- draw the caller's cell, merging shape 1 over it when $0305 is set.

/// $71CD -- set the per-apple score at $71CB/$71CC to $71C8[difficulty] times
/// the level, in BCD.

/* --- apples and sound ---------------------------------------------------- */

/// $7642 -- place a new apple on a free cell, found by rejection sampling.

/// $64A9 -- a rising then falling pitch sweep, clicked through $6C49.

/// $7633 -- count one apple eaten and play the sweep.

/// $6217 -- poll the keyboard into the 16-entry ring buffer at $623C.

/// $7590 -- show the character in A at slot X of the key-redefinition screen.
void game_show_key(uint16_t ret_addr);

/// $6B3D -- draw both side walls in two inks, with a randomly placed seam.

/// $6AB8 -- can the snake step in direction $6B38? Returns A = 0 / Z set for
/// yes, and refuses dead ends one move early.
void game_move_ok(uint16_t ret_addr);

/// $64C8 -- step the bouncer at $6633/$6634 by its deltas, reflecting off
/// whatever it hits.
void game_move_bouncer(uint16_t ret_addr);

/// $728D -- copy the score at $7252 over the high score at $7256 if it beats
/// it, comparing BCD bytes most significant first.
void game_update_high_score(uint16_t ret_addr);

/// $6BFB -- twenty passes of a falling tone, driven by the period at $6C46.

/// $6594 -- step the bouncers the difficulty calls for, then return the next
/// queued key in A.
void game_step_bouncers(uint16_t ret_addr);

/// $69C3 -- find an apple by sweeping columns outward from the snake, leaving
/// the result at $6B3B/$6B3C.
void game_find_apple(uint16_t ret_addr);

/// $69A9 -- adapter for game_pause_or_toggle_sound_native(): ESC pauses until
/// a key, Ctrl-S toggles the sound flag at $69C2.
void game_pause_or_toggle_sound(uint16_t ret_addr);

/// $75D1 -- blink slot X of the key-redefinition screen and wait for a
/// replacement key.
void game_edit_key(uint16_t ret_addr);

/// $6C72 -- turn the next key, or the joystick, into a direction.
void game_read_direction(uint16_t ret_addr);

/// $6288 -- one life. Adapter for game_play_loop_native(); leaves the reason
/// it ended in $6253, which is what the caller at $7739 reads.
void game_play_loop(uint16_t ret_addr);

/// $72CE -- draw the status panel. Adapter for game_status_panel().

/// $78B3 -- the bonus screen. Adapter for game_bonus_screen(). Entered with
/// decimal mode set.

/// $6256 -- start a life and run it. Adapter for game_begin_life().
void game_start_round(uint16_t ret_addr);

/// $7980 -- the setup screen. Adapter for game_setup_screen().
void game_setup(uint16_t ret_addr);

/* ========================================================================== *
 * Apple II ROM entry points, hand-written                                  *
 * ========================================================================== */

/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// Hand-written replacements for the Apple II ROM entry points listed in
/// `rom.externs`. See `a2rom.h` for the per-routine documentation.
///
/// ============================================================================
/// THIS FILE IS NOT A STANDALONE TRANSLATION UNIT. IT MUST BE #include-d.
/// ============================================================================
///
/// The emulated machine state lives in `apple2tc/system2-inc.h`, and almost all
/// of it has *internal linkage*:
///
///   - `s_a`, `s_x`, `s_y`, `s_sp`, `s_status_*`, `s_ram`, `s_pc`, `s_cycles`,
///     `s_remaining_cycles` are all `static`.
///   - `peek`, `poke`, `peek16`, `ram_peek16al`, `push8`, `pop8`, `push16`,
///     `pop16`, `adc_decimal`, `sbc_decimal` are all `static`.
///   - The `CYCLES()` macro expands to references to `s_pc`, `s_cycles`,
///     `s_remaining_cycles` and the *local* variable `branchTarget`.
///
/// Only `ram_peek`, `ram_poke`, `ram_peek16`, `io_peek`, `io_poke` and
/// `error_handler` have external linkage (declared in `apple2tc/system.h`).
///
/// A separate `.c` file therefore cannot see the CPU state at all. In addition,
/// this file calls the still-generated helpers `rom_vtabz`, `rom_clreolz`,
/// `rom_clreol` and `rom_wait` and uses `ovf8()` / `adc_dec16()`, which
/// the decompiler emits as `static` in the generated file.
///
/// Consequently this file must be textually included into the same translation
/// unit as the generated C, *after* it. That is what `snake-byte-ext.c` does,
/// and `snake-byte-ext.c` is the file CMake compiles for the `snake-bytec1-ext`
/// target:
///
///     #include "snake-bytec1-ext.c"
///     #include "a2rom.c"
///
/// DO NOT add `a2rom.c` to CMake as a source file of its own. Compiling it
/// alone yields a wall of undefined identifiers, because everything listed
/// above is invisible outside the generated file's translation unit. Worse, if
/// it ever did compile separately it would get its *own* copy of `s_ram` and
/// the registers -- `system2-inc.h` declares them `static` -- and link cleanly
/// while reading and writing a machine state nothing else can see.
///
/// This file is used only by the `snake-bytec1-ext` build. The plain
/// `snake-bytec1` target is the self-contained reference build, which
/// decompiles the ROM instead of calling into here.


/* Helpers that remain in the generated code. Redeclared here so that this file
   reads standalone; C permits identical redeclarations. */
void rom_vtabz(uint16_t ret_addr);
void rom_clreolz(uint16_t ret_addr);
void rom_clreol(uint16_t ret_addr);
void rom_wait(uint16_t ret_addr);

/* $FDF0 COUT1, defined below. `rom_cout` dispatches to it, and so does the
   game's own $664A handler in game.c once it has drawn its glyph. */
static void rom_cout1(uint16_t ret_addr);

/* ========================================================================== */
/* Private helpers.                                                           */
/*                                                                            */
/* $F847 GBASCALC and $F80E PLOT1 used to be emitted as `FUNC_GBASCALC` and    */
/* `FUNC_PLOT1`. Their only callers were $F800/$F819/$F871, so once those      */
/* became external the two helpers became unreachable and the decompiler       */
/* dropped them. They are reproduced here verbatim from the pre-externs        */
/* output.                                                                    */
/* ========================================================================== */

/// $F847 GBASCALC. Compute the lo-res base address for row A into GBASL/GBASH
/// ($26/$27).
/// $F847 GBASCALC. The lo-res twin of BASCALC: a row 0-47 in A becomes that
/// row's base address in GBASL/GBASH.
///
/// Same shape as BASCALC and the same trick -- LSR puts the row's low bit in
/// the carry, the next three bits pick the band, and the `ADC #$7F` folds in
/// the half. It differs in the tail: BASCALC ORs the shifted value back in to
/// build a text address, and this one shifts by two and ORs, which lands on
/// the lo-res page instead.
static void rom_gbascalc(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$F847*/ CYCLES(0xf847, 20);
  const uint8_t row = s_a;
  const uint8_t odd = (uint8_t)(row & 0x01);
  s_status_c = odd;
  /*$F84D*/ s_gbash = (uint8_t)(((row >> 0x01) & 0x03) | 0x04);
  /*$F850*/ s_a = (uint8_t)(row & 0x18);

  if (odd) {
    /*$F854*/ CYCLES(0xf854, 2);
    if (!s_status_d)
      s_a = (uint8_t)((s_a + 0x7f) + s_status_c);
    else
      s_a = (uint8_t)adc_dec16(s_a, 0x7f, s_status_c);
  } else {
    // $F852 BCC -- the branch itself, taken here (not modelled by the block
    // above's own cost, which is the not-taken total; see the design doc on
    // edge costs).
    /*$F852*/ CYCLES_EDGE(0xf852, 1);
  }
  branchTarget = true;
  (void)branchTarget;

  /*$F856*/ CYCLES(0xf856, 19);
  s_gbasl = s_a;
  /*$F85C*/ s_gbasl = (uint8_t)((uint8_t)(s_a << 0x02) | s_gbasl);

  /*$F85E*/ if (ret_addr) pop16();
}

/// $F80E PLOT1. Store the color mask ($30) into the lo-res half-byte selected
/// by MASK ($2E) at GBASL/GBASH ($26) + Y.
static void rom_plot1(uint16_t ret_addr) {
  bool branchTarget = true;
  (void)branchTarget;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // One lo-res cell: replace the half of the byte MASK selects with the
  // matching half of COLOR, leaving the other half alone. `(old ^ colour) &
  // mask ^ old` is the ROM's way of saying that in three instructions.
  /*$F80E*/ CYCLES(0xf80e, 28);
  const uint16_t at = (uint16_t)(gbas16() + s_y);
  const uint8_t old = peek(at);
  const uint8_t mixed = (uint8_t)(((old ^ s_color) & s_mask) ^ old);
  s_status_not_z = mixed;
  s_status_n = (mixed & 0x80);
  s_a = mixed;
  /*$F816*/ poke(at, mixed);

  /*$F818*/ if (ret_addr) pop16();
}

/* ========================================================================== */
/* $F800 PLOT                                                                 */
/* ========================================================================== */

/// $F800 PLOT. Light the lo-res cell at row A, column Y, in COLOR.
///
/// Two cells share a byte, so the row's low bit picks which nibble -- MASK
/// becomes $0F for the lower half and $F0 for the upper. The ROM gets there by
/// keeping the bit on the stack across GBASCALC and then adding $E0 to $0F,
/// which is $F0 with the carry set and $EF without; only the low bit of the
/// row can make the difference, so the sum is one of the two masks.
void rom_plot(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$F800*/ CYCLES(0xf800, 11);
  const uint8_t row = s_a;
  const uint8_t half = (uint8_t)(row >> 0x01);
  const bool upper = (row & 0x01) != 0;
  s_a = half;
  /*$F801*/ push8((uint8_t)((row & 0x01) | ((half == 0) << 1) | (s_status_i << 2) |
                            (s_status_d << 3) | STATUS_B | (s_status_v << 6) |
                            (half & 0x80)));
  /*$F802*/ rom_gbascalc(0xfffe);

  /*$F805*/ CYCLES(0xf805, 8);
  {
    const uint8_t saved = pop8();
    s_status_c = (uint8_t)(saved & 0x01);
    s_status_i = ((saved & 0x04) != 0);
    s_status_d = ((saved & 0x08) != 0);
    s_status_b = 0x00;
    s_status_v = ((saved & 0x40) != 0);
  }
  /*$F806*/ s_a = 0x0f;

  if (upper) {
    /*$F80A*/ CYCLES(0xf80a, 2);
    if (!s_status_d) {
      const uint16_t r = ((uint16_t)s_a + 0x00e0) + s_status_c;
      s_status_c = (uint8_t)(r >> 8);
      s_status_v = ovf8((uint8_t)r, s_a, 0xe0);
      s_a = (uint8_t)r;
    } else {
      const uint16_t r = adc_dec16(s_a, 0xe0, s_status_c);
      s_a = (uint8_t)r;
      const uint8_t flags = (uint8_t)(r >> 8);
      s_status_c = (flags & 0x01);
      s_status_v = ((flags & 0x40) != 0);
    }
  } else {
    // $F808 BCC -- the branch itself, taken here.
    /*$F808*/ CYCLES_EDGE(0xf808, 1);
  }
  branchTarget = true;
  (void)branchTarget;

  /*$F80C*/ CYCLES(0xf80c, 3);
  s_mask = s_a;
  rom_plot1(0x0000); // JMP -- a tail call.

  if (ret_addr) pop16();
}

/* ========================================================================== */
/* $F819 HLINE                                                                */
/* ========================================================================== */

/// $F819 HLINE, and $F826 VLINEZ, which share this body.
///
/// HLINE plots along row A from column Y to H2. Its loop ends by falling out
/// of the bottom into VLINEZ's, which walks rows from A down to V2 in column
/// Y -- so one entry point draws a horizontal run and the other a vertical
/// one, out of the same six instructions.
///
/// The labels keep their addresses rather than becoming nested loops, for the
/// same reason game_cold_start's do: the two loops are entered at different
/// depths and leave through each other, which C's loop forms cannot say
/// without a flag that the original does not have.
void rom_hline(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$F819*/ CYCLES(0xf819, 6);
  rom_plot(0xfffe);
  branchTarget = true;

across: /* $F81C -- one column at a time, up to H2 */
  CYCLES(0xf81c, 5);
  s_status_c = (uint8_t)(s_y >= s_h2);
  branchTarget = true;
  if (s_status_c) {
    // $F81E BCS -- the branch itself, taken here.
    /*$F81E*/ CYCLES_EDGE(0xf81e, 1);
    goto done;
  }

  /*$F820*/ CYCLES(0xf820, 8);
  s_y = (uint8_t)(s_y + 0x01);
  /*$F821*/ rom_plot1(0xfffe);
  /*$F824*/ CYCLES(0xf824, 2);
  branchTarget = true;
  if (!s_status_c) {
    // $F824 BCC -- the branch itself, taken here.
    /*$F824*/ CYCLES_EDGE(0xf824, 1);
    goto across;
  }

down: /* $F826 -- one row at a time, up to V2 */
  CYCLES(0xf826, 2);
  if (!s_status_d) {
    const uint16_t r = ((uint16_t)s_a + 0x0001) + s_status_c;
    s_status_v = ovf8((uint8_t)r, s_a, 0x01);
    s_a = (uint8_t)r;
  } else {
    const uint16_t r = adc_dec16(s_a, 0x01, s_status_c);
    s_a = (uint8_t)r;
    s_status_v = (((uint8_t)(r >> 8) & 0x40) != 0);
  }

  /*$F828*/ CYCLES(0xf828, 9);
  push8(s_a);
  /*$F829*/ rom_plot(0xfffe);
  /*$F82C*/ CYCLES(0xf82c, 9);
  s_a = pop8();
  /*$F82D*/ s_status_c = (uint8_t)(s_a >= s_v2);
  branchTarget = true;
  if (!s_status_c) {
    // $F82F BCC -- the branch itself, taken here.
    /*$F82F*/ CYCLES_EDGE(0xf82f, 1);
    goto down;
  }

done:
  /*$F831*/ CYCLES(0xf831, 6);
  (void)branchTarget;
  if (ret_addr) pop16();
}

/* ========================================================================== */
/* $F864 SETCOL                                                               */
/* ========================================================================== */

void rom_setcol(uint16_t ret_addr) {
  bool branchTarget = true;
  (void)branchTarget;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // The lo-res colour is stored in both nibbles, so a PLOT can take whichever
  // half MASK selects without shifting. Four ASLs and an ORA get there; the
  // carry the original leaves is the top bit shifted out of the low nibble.
  /*$F864*/ CYCLES(0xf864, 25);
  const uint8_t low = (uint8_t)(s_a & 0x0f);
  s_color = low;
  const uint16_t shifted = (uint16_t)(low << 0x04);
  s_status_c = (uint8_t)((shifted & 0x01ff) >> 8);
  const uint8_t both = (uint8_t)((uint8_t)shifted | s_color);
  s_status_not_z = both;
  s_status_n = (both & 0x80);
  s_a = both;
  s_color = both;

  /*$F870*/ if (ret_addr) pop16();
}

/* ========================================================================== */
/* $F871 SCRN                                                                 */
/* ========================================================================== */

void rom_scrn(uint16_t ret_addr) {
  bool branchTarget = true;
  (void)branchTarget;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  // The row's low bit says which half of the byte holds this cell, and the
  // ROM keeps it across GBASCALC on the stack -- as the whole status
  // register, because LSR put it in the carry and PHP is one byte.
  /*$F871*/ CYCLES(0xf871, 11);
  const uint8_t row = s_a;
  const uint8_t half = (uint8_t)(row >> 0x01);
  const bool upper = (row & 0x01) != 0;
  s_a = half;
  /*$F872*/ push8((uint8_t)((row & 0x01) | ((half == 0) << 1) | (s_status_i << 2) |
                            (s_status_d << 3) | STATUS_B | (s_status_v << 6) |
                            (half & 0x80)));
  /*$F873*/ rom_gbascalc(0xfffe);

  /*$F876*/ CYCLES(0xf876, 11);
  s_a = peek((uint16_t)(gbas16() + s_y));

  // PLP: only the carry matters to what follows, but the rest is restored
  // because the original restores it.
  /*$F878*/ {
    const uint8_t saved = pop8();
    s_status_c = (uint8_t)(saved & 0x01);
    s_status_i = ((saved & 0x04) != 0);
    s_status_d = ((saved & 0x08) != 0);
    s_status_b = 0x00;
    s_status_v = ((saved & 0x40) != 0);
  }

  if (upper) {
    /*$F87B*/ CYCLES(0xf87b, 8);
    s_status_c = (uint8_t)((s_a >> 0x03) & 0x01);
    s_a = (uint8_t)(s_a >> 0x04);
  } else {
    // $F879 BCC -- the branch itself, taken here.
    /*$F879*/ CYCLES_EDGE(0xf879, 1);
  }

  /*$F87F*/ CYCLES(0xf87f, 8);
  s_a &= 0x0f;
  s_status_not_z = s_a;
  s_status_n = 0x00;

  /*$F881*/ if (ret_addr) pop16();
}

/* ========================================================================== */
/* $FC58 HOME                                                                 */
/* ========================================================================== */

/// $FC58 HOME. Clear the text window and put the cursor at its top left.
///
/// One line at a time from WNDTOP: VTAB to it, CLREOLZ it, next. The ROM
/// carries the line number on the stack across both calls because VTABZ and
/// CLREOLZ each destroy A.
void rom_home(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

home: /* $FC58 */
  CYCLES(0xfc58, 13);
  s_a = s_wndtop;
  /*$FC5A*/ s_cv = s_wndtop;
  /*$FC5C*/ s_y = 0x00;
  /*$FC5E*/ s_ch = 0x00;
  branchTarget = true;
  // $FC60 BEQ -- provably always taken (Y was just loaded 0), but the branch
  // instruction still executes and still pays its own cost every time. The
  // decompiler doesn't do cross-instruction flag proofs either, so it keeps
  // charging this the same way.
  /*$FC60*/ CYCLES_EDGE(0xfc60, 1);

  for (;;) { /* $FC46 -- CLRSC2, one line per pass */
    CYCLES(0xfc46, 9);
    push8(s_a);
    /*$FC47*/ rom_vtabz(0xfffe);
    /*$FC4A*/ CYCLES(0xfc4a, 6);
    rom_clreolz(0xfffe);

    /*$FC4D*/ CYCLES(0xfc4d, 13);
    s_y = 0x00;
    s_a = pop8();
    if (!s_status_d)
      s_a = (uint8_t)(s_a + s_status_c);
    else
      s_a = (uint8_t)adc_dec16(s_a, 0x00, s_status_c);

    /*$FC52*/ s_status_c = (uint8_t)(s_a >= s_wndbtm);
    branchTarget = true;
    if (!s_status_c) {
      // $FC54 BCC -- the branch itself, taken here.
      /*$FC54*/ CYCLES_EDGE(0xfc54, 1);
      continue;
    }

    /*$FC56*/ CYCLES(0xfc56, 2);
    branchTarget = true;
    if (!s_status_c)
      goto home; // the BCS's not-taken arm, which cannot be reached
    // $FC56 BCS -- the branch itself, same address as the block above because
    // this is a singleton one-instruction block.
    /*$FC56*/ CYCLES_EDGE(0xfc56, 1);
    break;
  }

  /*$FC22*/ CYCLES(0xfc22, 3); // TABV
  s_a = s_cv;
  (void)branchTarget;
  rom_vtabz(0x0000); // JMP -- a tail call.
  if (ret_addr) pop16();
}

/* ========================================================================== */
/* $FC68 -- the tail of LF: VTAB, or scroll if CV has run off the window.      */
/*                                                                            */
/*   FC68: LDA CV / CMP WNDBTM / BCC VTABZ    ; usual case: just recompute BAS */
/*   FC6E: DEC CV / LDA WNDTOP / PHA / ...    ; otherwise scroll up one line   */
/*                                                                            */
/* Transcribed from the pre-externs mega-switch blocks $FC68..$FC95 plus the   */
/* three exit blocks ($FC6C -> VTABZ, $FC22, $FC9A -> CLREOL).                */
/*                                                                            */
/* Besides COUT1 falling through $FC66, the game calls this directly as a      */
/* VTAB: $7590 and $75D1 store CH/CV and JSR here. Their CV values are 5..13,  */
/* well under WNDBTM ($18), so they always take the BCC and never scroll.      */
/* ========================================================================== */

/// $FC68. The tail of the ROM's line feed: VTAB to CV, or scroll the window up
/// one line first if CV has run past the bottom.
///
/// The game calls it directly as a plain VTAB -- $7590 and $75D1 store CH/CV
/// and JSR here -- with a CV always well under WNDBTM, so the scroll below
/// never runs on their behalf. See a2rom.h.
///
/// The scroll copies each line over the one above it, back to front, keeping
/// the source line's base in BASL and the destination's in BAS2L.
void rom_fc68(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$FC68*/ CYCLES(0xfc68, 8);
  s_a = s_cv;
  branchTarget = true;
  if (!(s_cv >= s_wndbtm)) {
    // $FC6C BCC -- the branch itself, taken here. Nothing to scroll.
    /*$FC6C*/ CYCLES_EDGE(0xfc6c, 1);
    rom_vtabz(0x0000); // JMP -- a tail call.
    if (ret_addr) pop16();
    return;
  }

  /*$FC6E*/ CYCLES(0xfc6e, 17);
  s_cv = (uint8_t)(s_cv - 0x01);
  /*$FC70*/ s_a = s_wndtop;
  /*$FC72*/ push8(s_a);
  /*$FC73*/ rom_vtabz(0xfffe);
  branchTarget = true;

scroll: /* $FC76 -- one line up per pass */
  CYCLES(0xfc76, 28);
  /*$FC78*/ s_bas2l = s_basl;
  /*$FC7C*/ s_bas2h = s_bash;
  /*$FC80*/ s_y = (uint8_t)(s_wndwdth - 0x01);
  /*$FC81*/ s_a = pop8();
  if (!s_status_d) {
    const uint16_t r = ((uint16_t)s_a + 0x0001) + s_status_c;
    s_status_v = ovf8((uint8_t)r, s_a, 0x01);
    s_a = (uint8_t)r;
  } else {
    const uint16_t r = adc_dec16(s_a, 0x01, s_status_c);
    s_a = (uint8_t)r;
    s_status_v = (((uint8_t)(r >> 8) & 0x40) != 0);
  }

  /*$FC86*/ branchTarget = true;
  if (s_a >= s_wndbtm) {
    // $FC86 BCS -- the branch itself, taken here. That was the last line.
    /*$FC86*/ CYCLES_EDGE(0xfc86, 1);
    goto last_line;
  }

  /*$FC88*/ CYCLES(0xfc88, 9);
  push8(s_a);
  /*$FC89*/ rom_vtabz(0xfffe);
  branchTarget = true;

copy: /* $FC8C -- one character, right to left */
  CYCLES(0xfc8c, 15);
  {
    const uint8_t at = s_y;
    /*$FC8E*/ poke((uint16_t)(bas2_16() + at), peek((uint16_t)(bas16() + at)));
    /*$FC90*/ const uint8_t next = (uint8_t)(at - 0x01);
    s_status_n = (uint8_t)(next & 0x80);
    s_y = next;
    branchTarget = true;
    if (!s_status_n) {
      // $FC91 BPL -- the branch itself, taken here (loop back).
      /*$FC91*/ CYCLES_EDGE(0xfc91, 1);
      goto copy;
    }
  }

  /*$FC93*/ CYCLES(0xfc93, 2);
  branchTarget = true;
  if (s_status_n) {
    // $FC93 BMI -- the branch itself, taken here (outer loop back).
    /*$FC93*/ CYCLES_EDGE(0xfc93, 1);
    goto scroll;
  }

last_line: /* $FC95 -- blank what the scroll left at the bottom */
  CYCLES(0xfc95, 8);
  s_y = 0x00;
  /*$FC97*/ rom_clreolz(0xfffe);

  /*$FC9A*/ CYCLES(0xfc9a, 2);
  branchTarget = true;
  (void)branchTarget;
  if (!s_status_c) {
    rom_clreol(0x0000); // JMP -- a tail call.
    if (ret_addr) pop16();
    return;
  }
  // $FC9A BCS -- taken here, and it falls into the trampoline charge below
  // before continuing; the not-taken arm above jumps straight out without it.
  /*$FC9A*/ CYCLES_EDGE(0xfc9a, 1);

  /*$FC22*/ CYCLES(0xfc22, 3); // TABV
  s_a = s_cv;
  rom_vtabz(0x0000);
  if (ret_addr) pop16();
}

/* ========================================================================== */
/* $FB78 COUTZ - the body of COUT1, reached through $FB94 -> $FBFD.           */
/*                                                                            */
/* Transcribed from the pre-externs mega-switch blocks $FB78..$FB94,          */
/* $FBD9..$FBEF, $FBF0..$FBFC, $FC01..$FC2B and $FC62..$FC66. The whole        */
/* subtree disappeared from the generated C once $FDED became external,        */
/* because $FDED was its only entry.                                          */
/*                                                                            */
/* Path coverage, measured by instrumenting this file and replaying the        */
/* 1300-frame play.keys session:                                              */
/*                                                                            */
/*   $FBF0 store char      402 hits    $FC62 CR            3 hits             */
/*   $FC68 scroll           39 hits    $FC66 LF            3 hits             */
/*   $FBD9 CMP #$87          1 hit     $FBDD bell body     1 hit              */
/*   $FBE4 speaker click   192 hits    $FB7C CR kbd poll   3 hits             */
/*   $FB85/$FB88 Ctrl-S      0 hits    $FC10 backspace     0 hits             */
/*                                                                            */
/* So the bell is live: ROM RESET reaches it via $FF3A (LDA #$87; JMP $FDED),  */
/* and the 192 clicks are the $FBE2 `LDY #$C0` loop. It is oracle-verified.    */
/*                                                                            */
/* Backspace ($FC10) is unreachable from Snake Byte, not merely untraced: no   */
/* byte the game emits is $88. The $08 inside the "CRASH" string at $7868 is   */
/* the trap for the eye - it has bit 7 clear, so $FC01 (TAY; BPL) routes it to */
/* $FBF0 as an inverse glyph long before the CMP #$88 at $FC0C.                */
/*                                                                            */
/* The Ctrl-S handshake ($FB85/$FB88) is reachable but untraced: $FB7C does    */
/* run (A == $8D three times), it just never saw a pending $93. It fires only  */
/* if the user holds Ctrl-S while a CR is being output. Transcribed but not    */
/* covered by the oracle - the one part of this file in that category.         */
/* ========================================================================== */

/// $FB78 COUTZ. The monitor's character-output dispatcher: printable
/// characters go on the screen, the four control codes it knows do their
/// thing, and everything else is dropped.
///
/// Entered from COUT1, which has already masked the character with INVFLG.
///
/// **Three of its arms are decoded and never run.** Probed at their entry
/// addresses across both cold scenarios: the character store fires 204 and 651
/// times and the carriage return and line feed 0 and 17, but the Ctrl-S
/// handshake ($FB85), its spin ($FB88), the backspace ($FC10) and the bell
/// ($FBD9 and $FBDD) fire **zero** times in either. Everything this game
/// prints is printable, a return, or a line feed; it never emits a Ctrl-G or a
/// backspace, and no recording holds a key down across a return. Those arms
/// rest on the binary alone.
///
/// The labels keep their addresses. This is a dispatcher whose arms rejoin at
/// several depths -- backspace falls into the cursor-up path, a wrapped line
/// falls into the carriage return, and a carriage return falls into the line
/// feed -- and writing it as nested ifs would need each of those spelled out
/// twice.
static void rom_coutz(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$FB78*/ CYCLES(0xfb78, 4);
  branchTarget = true;
  if (s_a != 0x8d) {
    /*$FB7A*/ CYCLES_EDGE(0xfb7a, 1);
    goto emit;
  }

  /* $FB7C -- the Ctrl-S handshake, on a carriage return only. If a key is
     already waiting and it is Ctrl-S, stop here until another key arrives.
     Ctrl-C is left in the keyboard latch on the way out so that whatever is
     running next still sees it; anything else is consumed. */
  /*$FB7C*/ CYCLES(0xfb7c, 6);
  s_y = io_peek(0xc000);
  branchTarget = true;
  if (!(s_y & 0x80)) {
    /*$FB7F*/ CYCLES_EDGE(0xfb7f, 1);
    goto emit;
  }

  /*$FB81*/ CYCLES(0xfb81, 4);
  branchTarget = true;
  if (s_y != 0x93) {
    /*$FB83*/ CYCLES_EDGE(0xfb83, 1);
    goto emit;
  }

  /*$FB85*/ CYCLES(0xfb85, 4);
  s_status_v = (uint8_t)((io_peek(0xc010) >> 0x06) & 0x01);

  for (;;) { /* $FB88 -- spin until a key is pressed */
    CYCLES(0xfb88, 6);
    s_y = io_peek(0xc000);
    branchTarget = true;
    if (!(s_y & 0x80)) {
      /*$FB8B*/ CYCLES_EDGE(0xfb8b, 1);
      continue;
    }

    /*$FB8D*/ CYCLES(0xfb8d, 4);
    branchTarget = true;
    if (s_y == 0x83) {
      // Ctrl-C: leave it latched.
      /*$FB8F*/ CYCLES_EDGE(0xfb8f, 1);
      break;
    }
    /*$FB91*/ CYCLES(0xfb91, 4);
    s_status_v = (uint8_t)((io_peek(0xc010) >> 0x06) & 0x01);
    break;
  }

emit: /* $FB94 JMP $FBFD */
  CYCLES(0xfb94, 3);
  /*$FBFD*/ CYCLES(0xfbfd, 4);
  branchTarget = true;
  if (!(s_a >= 0xa0)) {
    // The not-taken arm jumps straight to the dispatch without the edge charge.
    goto dispatch;
  }
  // $FBFF BCS -- taken here, and it falls into the trampoline charge before
  // continuing.
  /*$FBFF*/ CYCLES_EDGE(0xfbff, 1);

store: /* $FBF0 -- put the character at the cursor */
  CYCLES(0xfbf0, 9);
  s_y = s_ch;
  /*$FBF2*/ poke((uint16_t)(bas16() + s_y), s_a);

  /*$FBF4*/ CYCLES(0xfbf4, 13);
  s_ch = (uint8_t)(s_ch + 0x01);
  s_a = s_ch;
  {
    const uint8_t width = s_wndwdth;
    s_status_not_z = (uint8_t)(s_a != width);
    s_status_c = (uint8_t)(s_a >= width);
    s_status_n = (uint8_t)((uint8_t)(s_a - width) & 0x80);
    branchTarget = true;
    if (s_status_c) {
      // Off the right edge, so wrap: the same thing a carriage return does.
      /*$FBFA*/ CYCLES_EDGE(0xfbfa, 1);
      goto carriage_return;
    }
  }
  /*$FBFC*/ CYCLES(0xfbfc, 6);
  branchTarget = true;
  goto out;

dispatch: /* $FC01 -- not printable; which control code is it? */
  CYCLES(0xfc01, 4);
  s_y = s_a;
  branchTarget = true;
  if (!(s_a & 0x80)) {
    // Below $80 the monitor stores it anyway, high bit and all.
    /*$FC02*/ CYCLES_EDGE(0xfc02, 1);
    goto store;
  }

  /*$FC04*/ CYCLES(0xfc04, 4);
  branchTarget = true;
  if (s_a == 0x8d) {
    /*$FC06*/ CYCLES_EDGE(0xfc06, 1);
    goto carriage_return;
  }

  /*$FC08*/ CYCLES(0xfc08, 4);
  branchTarget = true;
  if (s_a == 0x8a) {
    /*$FC0A*/ CYCLES_EDGE(0xfc0a, 1);
    goto line_feed;
  }

  /*$FC0C*/ CYCLES(0xfc0c, 4);
  s_status_c = (uint8_t)(s_a >= 0x88);
  branchTarget = true;
  if (s_a != 0x88) {
    /*$FC0E*/ CYCLES_EDGE(0xfc0e, 1);
    goto bell;
  }

  /* $FC10 -- backspace. Off the left edge wraps to the end of the line above,
     which is why it falls into the cursor-up path rather than returning. */
  /*$FC10*/ CYCLES(0xfc10, 7);
  {
    const uint8_t back = (uint8_t)(s_ch - 0x01);
    s_status_not_z = back;
    s_status_n = (uint8_t)(back & 0x80);
    s_ch = back;
    branchTarget = true;
    if (!(back & 0x80)) {
      /*$FC12*/ CYCLES_EDGE(0xfc12, 1);
      /*$FBFC*/ CYCLES(0xfbfc, 6);
      branchTarget = true;
      goto out;
    }
  }

  /*$FC14*/ CYCLES(0xfc14, 11);
  /*$FC16*/ s_ch = s_wndwdth;
  /*$FC18*/ s_ch = (uint8_t)(s_ch - 0x01);

  /*$FC1A*/ CYCLES(0xfc1a, 8);
  {
    const uint8_t top = s_wndtop;
    s_a = top;
    const uint8_t cv = s_cv;
    s_status_not_z = (uint8_t)(top != cv);
    s_status_c = (uint8_t)(top >= cv);
    s_status_n = (uint8_t)((uint8_t)(top - cv) & 0x80);
    branchTarget = true;
    if (s_status_c) {
      // Already on the window's top line; there is nowhere to go up to.
      /*$FC1E*/ CYCLES_EDGE(0xfc1e, 1);
      /*$FC2B*/ CYCLES(0xfc2b, 6);
      branchTarget = true;
      goto out;
    }
  }

  /*$FC20*/ CYCLES(0xfc20, 5);
  s_cv = (uint8_t)(s_cv - 0x01);
  /*$FC22*/ CYCLES(0xfc22, 3); // TABV
  s_a = s_cv;
  rom_vtabz(0x0000);
  goto out;

bell: /* $FBD9 -- Ctrl-G, or a control code the monitor does not know */
  CYCLES(0xfbd9, 4);
  {
    const uint8_t ch = s_a;
    const uint8_t differs = (uint8_t)(ch != 0x87);
    s_status_not_z = differs;
    s_status_c = (uint8_t)(ch >= 0x87);
    s_status_n = (uint8_t)((uint8_t)(ch - 0x87) & 0x80);
    branchTarget = true;
    if (differs) {
      // Not the bell either. Drop it.
      /*$FBDB*/ CYCLES_EDGE(0xfbdb, 1);
      /*$FBEF*/ CYCLES(0xfbef, 6);
      branchTarget = true;
      goto out;
    }
  }

  // A tenth of a second of silence, then 192 clicks of the speaker.
  /*$FBDD*/ CYCLES(0xfbdd, 8);
  s_a = 0x40;
  /*$FBDF*/ rom_wait(0xfffe);
  branchTarget = true;
  /*$FBE2*/ CYCLES(0xfbe2, 2);
  s_y = 0xc0;

  for (;;) { /* $FBE4 */
    CYCLES(0xfbe4, 8);
    s_a = 0x0c;
    /*$FBE6*/ rom_wait(0xfffe);
    branchTarget = true;
    /*$FBE9*/ CYCLES(0xfbe9, 8);
    s_a = io_peek(0xc030);
    /*$FBEC*/ s_y = (uint8_t)(s_y - 0x01);
    s_status_not_z = s_y;
    s_status_n = (uint8_t)(s_y & 0x80);
    branchTarget = true;
    if (!s_y)
      break;
    /*$FBED*/ CYCLES_EDGE(0xfbed, 1);
  }

  /*$FBEF*/ CYCLES(0xfbef, 6);
  branchTarget = true;
  goto out;

carriage_return: /* $FC62 -- to the left edge, then down */
  CYCLES(0xfc62, 5);
  /*$FC64*/ s_ch = 0x00;

line_feed: /* $FC66 */
  CYCLES(0xfc66, 5);
  s_cv = (uint8_t)(s_cv + 0x01);
  /*$FC68*/ rom_fc68(0x0000); // JMP -- a tail call, and where a scroll happens.

out:
  (void)branchTarget;
  if (ret_addr) pop16();
}

/* ========================================================================== */
/* $FDED COUT                                                                 */
/* ========================================================================== */

/// $FDED COUT is a single instruction: `JMP ($36)`. It dispatches through the
/// output vector CSWL/CSWH, so the *behaviour* of COUT is whatever the vector
/// points at, and it must not be short-circuited to ROM COUT1.
///
/// Snake Byte can hook the vector: $6641 does
///     LDA #$4A / STA $36 / LDA #$66 / STA $37 / RTS
/// pointing it at $664A, the game's own hi-res text renderer. ($6641 is called
/// from $7485 and $793F.) $664A maps the text cursor (BASL/BASH + CH) to a
/// hi-res address by adding $1C to the high byte ($04xx -> $20xx) and draws an
/// 8-byte glyph from the font at $66A9; control characters below $20 fall
/// through to ROM COUT1 at $FDF0.
///
/// In the entire recorded session, however, the vector always points at ROM:
/// the run data records $FDED -> $FDF0 only, and $6641 was never executed
/// (neither it nor its callers $7485/$793F appear in BranchTargets). $664A is
/// therefore classified as data and is not present in the generated C at all.
///
/// Because the recorded session never dispatches anywhere but $FDF0, the
/// verification oracle structurally *cannot* detect a wrong choice here. So a
/// non-ROM vector target is a hard, loud failure rather than a silent fallback
/// to COUT1: a wrong-but-quiet answer would diverge invisibly, whereas an abort
/// fires at exactly the moment the question first matters.
///
/// KNOWN GAP for a later phase: implement $664A (the hi-res text renderer) and
/// dispatch to it here.
/// $FDED COUT -- `JMP ($36)`. Dispatches through the output vector CSWL/CSWH
/// rather than reimplementing COUT1, because Snake Byte repoints it: $6641
/// installs the game's own hi-res text renderer at $664A.
///
/// Any target other than the two we implement is a hard failure rather than a
/// fallback. The recorded session never leaves $FDF0, so `verify.sh` cannot
/// catch a wrong guess here -- a silent fallback would render with the wrong
/// font onto the wrong page, undetectably.
void rom_cout(uint16_t ret_addr) {
  bool branchTarget = true;
  uint16_t vector;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$FDED*/ CYCLES(0xfded, 5);
            vector = csw16(); // JMP ($36)
            branchTarget = true;
            switch (vector) {
            case 0xfdf0:
              rom_cout1(0xfffe);
              break;
            case 0x664a:
              game_cout_hook(0xfffe);
              break;
            default:
              fprintf(
                  stderr,
                  "rom_cout: output vector CSWL/CSWH ($36/$37) points at $%04X, "
                  "which is not implemented.\n"
                  "  Known targets are $FDF0 (ROM COUT1) and $664A (the game's "
                  "hi-res text renderer).\n",
                  vector);
              error_handler(0xfded);
              abort();
            }

            if (ret_addr) pop16();
            return;
}

/// $FDF0 COUT1 -- the ROM's own character output: mask to the current text
/// mode, then COUTZ for the actual placement and cursor bookkeeping.
/// $FDF0 COUT1. Put a character on the text screen, then restore Y.
///
/// Printable characters ($A0 and up) are masked with INVFLG, which is how the
/// monitor does inverse and flashing -- $FF leaves them alone. Control codes
/// are let through unmasked, since mangling them would change what they mean.
static void rom_cout1(uint16_t ret_addr) {
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$FDF0*/ CYCLES(0xfdf0, 4);
  const bool printable = s_a >= 0xa0;
  s_status_c = (uint8_t)printable;

  if (printable) {
    /*$FDF4*/ CYCLES(0xfdf4, 3);
    s_a = (uint8_t)(s_a & s_invflg);
  } else {
    // $FDF2 BCC -- the branch itself, taken here.
    /*$FDF2*/ CYCLES_EDGE(0xfdf2, 1);
  }

  /*$FDF6*/ CYCLES(0xfdf6, 12);
  s_ysav1 = s_y;
  /*$FDF8*/ push8(s_a);
  branchTarget = true;
  rom_coutz(0xfdfb); // JSR $FB78

  /*$FDFC*/ CYCLES(0xfdfc, 13);
  s_a = pop8();
  /*$FDFD*/ s_y = s_ysav1;
  s_status_not_z = s_y;
  s_status_n = (s_y & 0x80);

  branchTarget = true;
  (void)branchTarget;
  /*$FDFF*/ if (ret_addr) pop16();
}

/* ========================================================================== */
/* $FE89 SETKBD / $FE93 SETVID                                                */
/*                                                                            */
/* Both fall into the shared tail at $FE9B (SETPWRC/"install IO hooks"), which */
/* is duplicated into each function exactly as the decompiler emitted it.      */
/* ========================================================================== */

/// $FE89 SETKBD. Point the character *input* vector at the built-in keyboard.
///
/// The vector it writes, KSWL/KSWH, is not read by anything in this build --
/// nothing dispatches input through it -- but the routine runs at startup and
/// its cycles count, so it is here in full.
void rom_setkbd(uint16_t ret_addr) {
  bool branchTarget = true;
  (void)branchTarget;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$FE89*/ CYCLES(0xfe89, 11);
  s_a2l = 0x00;
  s_x = 0x38;
  s_y = 0x1b;
  // $FE91 BNE -- provably always taken (Y was just loaded #$1B, nonzero),
  // same reasoning as $FC60 in rom_home: the decompiler doesn't do
  // cross-instruction flag proofs, so the branch still executes and still
  // pays its own cost every time.
  /*$FE91*/ CYCLES_EDGE(0xfe91, 1);

  // $FE9B SETIO. A2L is the slot; slot 0 means the built-in device and the ROM
  // answers page $FD, where its own KEYIN and COUT1 live. A real slot would
  // give $Cn00 instead -- decoded, never taken, because the game never sets
  // one.
  /*$FE9B*/ CYCLES(0xfe9b, 7);
  /*$FE9D*/ const uint8_t slot = (uint8_t)(s_a2l & 0x0f);
  s_a = slot;
  if (slot) {
    /*$FEA1*/ CYCLES(0xfea1, 6);
    s_a = (uint8_t)(s_a | 0xc0);
    s_y = 0x00;
    // $FEA5 BEQ -- provably always taken (Y was just loaded 0).
    /*$FEA5*/ CYCLES_EDGE(0xfea5, 1);
  } else {
    // $FE9F BEQ -- the branch itself, taken here.
    /*$FE9F*/ CYCLES_EDGE(0xfe9f, 1);
    /*$FEA7*/ CYCLES(0xfea7, 2);
    s_a = 0xfd;
  }

  /*$FEA9*/ CYCLES(0xfea9, 14);
  s_kswl = s_y;
  s_kswh = s_a;

  /*$FEAD*/ if (ret_addr) pop16();
}

/// $FE93 SETVID. Point COUT's vector back at the ROM's own COUT1.
///
/// This is how the game gets out of its hi-res text hook -- or would be. It
/// writes CSWL/CSWH, which up to 2026-08-24 meant zero page $36/$37, and now
/// means the two variables those became. Between the move and this commit the
/// store went to RAM and was read by nobody, which nothing caught: the entry
/// snapshot already holds $FDF0 there, and the game only calls SETVID once, at
/// startup, before it has repointed anything. Had it ever called it after
/// installing the hook, COUT would have stayed hooked.
void rom_setvid(uint16_t ret_addr) {
  bool branchTarget = true;
  (void)branchTarget;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$FE93*/ CYCLES(0xfe93, 9);
  s_a2l = 0x00;
  s_x = 0x36;
  s_y = 0xf0;

  // $FE9B SETIO. A2L is the slot; slot 0 means the built-in device and the ROM
  // answers page $FD, where its own KEYIN and COUT1 live. A real slot would
  // give $Cn00 instead -- decoded, never taken, because the game never sets
  // one.
  /*$FE9B*/ CYCLES(0xfe9b, 7);
  /*$FE9D*/ const uint8_t slot = (uint8_t)(s_a2l & 0x0f);
  s_a = slot;
  if (slot) {
    /*$FEA1*/ CYCLES(0xfea1, 6);
    s_a = (uint8_t)(s_a | 0xc0);
    s_y = 0x00;
    // $FEA5 BEQ -- provably always taken (Y was just loaded 0).
    /*$FEA5*/ CYCLES_EDGE(0xfea5, 1);
  } else {
    // $FE9F BEQ -- the branch itself, taken here.
    /*$FE9F*/ CYCLES_EDGE(0xfe9f, 1);
    /*$FEA7*/ CYCLES(0xfea7, 2);
    s_a = 0xfd;
  }

  /*$FEA9*/ CYCLES(0xfea9, 14);
  s_cswl = s_y;
  s_cswh = s_a;

  /*$FEAD*/ if (ret_addr) pop16();
}

/* ========================================================================== *
 * The game, as ordinary C                                                  *
 * ========================================================================== */

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

/* --- The game's own tables ------------------------------------------------ */
/*
 * All of these are data inside the loaded image, so they are read and never
 * written -- except kShapeMask, which is the working copy kShapeMaskTable is
 * copied into. labels.txt carries the same names for the disassembler.
 */
enum {
  kHgrLineLo = 0x6000,      ///< 48 hi-res line addresses, low bytes
  kHgrLineHi = 0x6030,      ///< and high bytes
  kShapeMask = 0x6060,      ///< the loaded shape's four scanline masks
  kHgrPattern = 0x6064,     ///< dot patterns, indexed by the dot index
  kShapeMaskTable = 0x6174, ///< four masks per shape; the source for kShapeMask
  kSteerKey = 0x6a55,       ///< direction -> the key that turns to it
  kAppleValueTable = 0x71c8, ///< per-apple value, indexed by kDifficulty
  kKeyTable = 0x6c63,       ///< the six bound keys, in slot order
  kKeyDefaults = 0x6c6a,    ///< the same six as shipped; never written
  kKeyCH = 0x75b3,          ///< redefinition screen: where each slot's key
  kKeyCV = 0x75b9,          ///< ... and its arrow are printed
  kArrowCH = 0x75bf,
  kArrowCV = 0x75c5,
  kArrowGlyph = 0x75cb,     ///< which arrow glyph each slot blinks
};

/* --- The bouncers' working copy ------------------------------------------- */
/*
 * $6633-$6638 and $6C4A. The original does not step a bouncer where it lives:
 * it copies one of the two into $6633 first, steps that, and writes it back --
 * a calling convention in fixed memory, which is why a Bouncer struct fits it.
 */
enum {
  kBouncer = 0x6633,        ///< col, row, dx, dy at +0..+3
  kWantCol = 0x6637,        ///< where the step would land, before the walls
  kWantRow = 0x6638,        ///< get a say
  kBounceBlocked = 0x6c4a,  ///< that cell was occupied, so reflect instead
};

/* --- The auto-steer's answers --------------------------------------------- */
enum {
  /// The direction $6A32 settled on. game_move_ok and key_for_direction both
  /// read it back rather than being passed it.
  kSteerDir = 0x6b38,
  /// The apple sweep's cursor, left wherever the search stopped...
  kSearchCol = 0x6b39,
  kSearchRow = 0x6b3a,
  /// ...and the answer, which $6A25 copies out of it. Two pairs holding the
  /// same cell on exit, but only this one is what the steer reads.
  kAppleCol = 0x6b3b,
  kAppleRow = 0x6b3c,
};

/* --- Sound ---------------------------------------------------------------- */
enum {
  kTonePeriod = 0x6c46,    ///< and the on/off switch: 0 is silent
  kToneCountdown = 0x6c47, ///< passes left before the next click
  kTonePasses = 0x6c48,    ///< how many passes one tick of the tone runs
  kClickPort = 0x6c49,     ///< $30 the speaker, $20 the cassette, i.e. muted
  kSoundMuted = 0x69c2,    ///< toggled by Ctrl-S
};

/// $6C71 -- the player chose the joystick at the setup prompt.
enum { kJoystick = 0x6c71 };

/// $3754 and $3757 -- the address operands of the LDA and STA in the
/// relocation loop, which the loop increments to walk eight pages instead of
/// keeping a page counter. Written back because ram.probe hashes the range.
enum { kRelocLoadOp = 0x3754, kRelocStoreOp = 0x3757 };

/* --- The plotter's arguments ---------------------------------------------- */
/*
 * Nine bytes that lived at $0000-$0008 because the 6502 had nowhere else to
 * pass arguments. Every plot goes through them: set the shape, the ink and the
 * cell, then call.
 *
 * They are C variables now, and no longer aliased with anything -- the glyph
 * blitter, which used to borrow all nine for a font pointer and the caller's
 * registers, has its own locals. Making them actual parameters is the step
 * after this one, and each needs its own argument first: s_shape turned out to
 * be a genuine global, because bouncer_step erases with whatever was last left
 * in it.
 */
/*
 * Nine bytes of zero page that the drawing routines pass arguments in, because
 * the 6502 has nowhere else to put them. Every plot goes through here: set the
 * shape, the ink and the cell, then call.
 *
 * game_cout_hook_native is the exception and reuses all nine for something
 * unrelated -- see the second enum. That is not a naming problem to be tidied
 * away; it is a union the original wrote by hand, and the names say so.
 */
/// $0000 -- which shape to draw. game_load_shape turns it into the four
/// scanline masks at $6060.
///
/// A variable and not a parameter, which is what the original makes it:
/// bouncer_step erases where the bouncer was using whatever was last left
/// here, so the value outlives any one call. Every path that could have made
/// that a hazard was checked -- see the note below.
static uint8_t s_shape;


/// The block itself, moved out of emulated RAM and split into variables now
/// that the glyph blitter no longer shares it.
///
/// They are still variables rather than parameters, which is the next step and
/// not this one. What changed is that nothing can reach them by address, and
/// each one has a type and a name instead of an index.
/// Lo-res colour, 0..15; zero erases.
///
/// A global for the same reason s_shape is one: it is inherited. draw_border
/// draws six runs in the ink game_draw_playfield set before calling it, and
/// the redefinition screen's stem takes the ink of the arrow plotted above it.
/// plot_at and plot_shape_at therefore set it as well as passing it -- a run
/// that follows them reads it back. Dropping that write drew the stem in a
/// stale colour, which the cold gate's hires screen caught and its play screen
/// did not.
static uint8_t s_ink;


/// Plot the loaded shape, or a named one, into a cell. Defined further down,
/// next to the run helpers; declared here because the bouncers and the snake
/// use them well before that.
static void plot_at(uint8_t ink, Cell c);
static void plot_shape_at(uint8_t shape, uint8_t ink, Cell c);

/// The glyph blitter at $664A used to park five values in this block -- the
/// glyph, the caller's X and Y, and a source and destination pointer. They are
/// locals in game_cout_hook_native now.
///
/// Splitting them out needed an argument, because bouncer_step erases with
/// whatever was last left in s_shape: a COUT through the hi-res hook between a
/// shape write and that erase used to change which cells got erased. The
/// argument is that it cannot happen. The hook is installed in exactly two
/// places, game_bonus_screen and game_setup_screen, and each restores CSWL
/// before returning. Inside both windows every plot writes the shape
/// immediately before its call. And every path out of either window reaches
/// game_draw_playfield, which writes $15 unconditionally before it draws
/// anything, before the play loop can run -- game_bonus_screen returns into
/// round_cleared and game_setup_screen into new_game, and both fall through
/// start_round.
///
/// The gate could not have caught this being wrong until 2026-08-23: the cold
/// build ran $664A zero times under it. It now runs it 205 times per run, and
/// samples state at $760F while it does.

/* --- The snake, the key ring, and the click counter ----------------------- */
/*
 * $6232-$6255, one contiguous run holding everything a life consists of. The
 * storage stays where it is; these are names for it.
 */
enum {
  /// Column and row deltas, indexed by direction 1..4. Entry 0 is $00 in both
  /// and is never reached, since a direction of 0 means "no key".
  /// Measured against DIR_RIGHT..DIR_DOWN: columns are +1/0/-1/0 and rows are
  /// 0/-1/0/+1, so right is +column and up is -row, screen order.
  kColDelta = 0x6232,
  kRowDelta = 0x6237,
  /// The sixteen-entry ring the keyboard scan fills and the play loop drains.
  kKeyRing = 0x623c,
  /// Where the play loop reads next, and where the scan writes next. The scan
  /// refuses to advance the write cursor onto the read one, which is the
  /// ring's only full test. Both wrap with `& $0F`.
  kRingRead = 0x624c,
  kRingWrite = 0x624d,
  /// The direction the snake is travelling, DIR_RIGHT..DIR_DOWN.
  kDirection = 0x624e,
  /// The head's cell, and the tail's. The snake itself is not stored -- the
  /// lo-res screen is the occupancy map, and the tail walks it by reading the
  /// colour it finds to work out which way the body went.
  kHeadCol = 0x624f,
  kHeadRow = 0x6250,
  kTailCol = 0x6251,
  kTailRow = 0x6252,
  /// How the life ended, which $7739 reads the moment the play loop returns:
  /// $00 the gate, $0F an apple, $FF the quit key, $FE the timer, anything
  /// else the occupancy byte the head ran into. See LifeEnd, which is this
  /// byte with names on.
  kLifeOutcome = 0x6253,
  /// Segments still owed. While it is nonzero the tail is not trimmed, so the
  /// snake grows; a life starts with ten and each apple adds ten more.
  kGrowth = 0x6254,
  /// The life timer -- see kLifeTime for what seeds it and why $FF stops it.
  kLifeTimer = 0x6255,
};

/// $6473 -- how many more clicks the current sound effect owes. The pace loop
/// spends one per pass while it is nonzero, so the value is a duration:
/// $20 for eating an apple, $10 for a steering input, $07 for growing.
enum { kClickCount = 0x6473 };

/* ========================================================================== */
/* Converted routines                                                         */
/* ========================================================================== */

/// The snake's head column. Still at $624F: the generated dispatch reads it.
static void set_snake_head_col(uint8_t col) {
  ram_poke(kHeadCol, col);
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
    last = ram_peek(kShapeMaskTable + (uint8_t)((uint8_t)(shape << 2) + line));
    ram_poke(kShapeMask + line, last);
  }
  return last;
}

void game_install_cout_vector(void) {
  // CSWL/CSWH at $36/$37, pointed at $664A.
  s_cswl = 0x4a;
  s_cswh = 0x66;
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
/* The plots go through plot_at and plot_shape_at. Which one a call uses is    */
/* the point: the erase at $654C deliberately does *not* name a shape. It      */
/* reuses whatever s_shape holds, and the mask that shape selects decides      */
/* which pixels get cleared. Tidying that away would change the screen.        */
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
    ram_poke(kBounceBlocked, 0x00);
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
  s_ink = 0x00;
  rom_setcol(0x6552);

  GAME_CYCLES(0x6553, 20);
  plot_at(0x00, (Cell){.col = b->col, .row = b->row});

  GAME_CYCLES(0x6560, 14);
  s_a = b->row;
  s_y = b->col;
  rom_plot(0x6568);

  GAME_CYCLES(0x6569, 11);
  s_shape = 0x1a;

  if (want_row == 0) {
    // Off the board: not redrawn, and the position is not committed.
    GAME_CYCLES(0x6572, 6);
    ram_poke(kWantCol, want_col);
    ram_poke(kWantRow, want_row);
    ram_poke(kBounceBlocked, (uint8_t)blocked);
    return;
  }
  GAME_CYCLES(0x6570, 1);

  GAME_CYCLES(0x6573, 29);
  b->row = want_row;
  b->col = want_col;
  s_a = 0x03;
  s_ink = 0x03;
  rom_setcol(0x6586);

  GAME_CYCLES(0x6587, 6);
  plot_shape_at(0x1a, 0x03, (Cell){.col = b->col, .row = b->row});

  GAME_CYCLES(0x658a, 14);
  s_a = b->row;
  s_y = b->col;
  rom_plot(0x6592);

  GAME_CYCLES(0x6593, 6);
  ram_poke(kWantCol, want_col);
  ram_poke(kWantRow, want_row);
  ram_poke(kBounceBlocked, (uint8_t)blocked);
}

/* --- The scoreboard: $7252-$7266 ------------------------------------------ */
/*
 * Eleven fields, every one of them BCD, and every multi-byte one least
 * significant first. Six are what game_status_panel prints, and the labels it
 * prints beside them are where those six names come from -- see the $72CE
 * header for the layout.
 *
 * Named as constants rather than accessor functions because most of them are
 * *used* as addresses: the BCD helpers below take a low/high pair, and the
 * original's own add-and-carry shape survives in the converted code. The three
 * single-byte fields get accessors as well, since those read as values.
 */
enum {
  /// SCORE, four bytes. game_add_score adds an apple's worth into it;
  /// game_promote_high_score copies it over the high score at the end of a game.
  kScore = 0x7252,
  /// HI SCORE, four bytes, the same shape.
  kHiScore = 0x7256,
  /// APPLES LEFT -- what remains of this round's quota. start_round loads it
  /// from kApplesQuota and each apple eaten takes one off; both bytes zero is
  /// what ends the round.
  kApplesLeft = 0x725a,
  /// SNAKES LEFT -- lives. $7691 sets it to 2 for a new game.
  kLives = 0x725e,
  /// Apples on the playfield *right now* -- not, as this pair was commented
  /// before it was named, a countdown to the next one. game_place_apple ends
  /// by BCD-incrementing it ($766C) and $7743 decrements it when one is
  /// eaten; $77D0 places a replacement exactly when it reaches zero, which is
  /// why the field normally holds one apple and the timeout path's three
  /// arrive together. Measured on play-hires: it is $01 at every apple eaten,
  /// and $77D0 fires and places one every time.
  kApplesAfield = 0x725f,
  /// Apples eaten this round. Only the first $11 of them score -- $777B tests
  /// the high byte and $7780 compares the low against $11.
  kApplesEaten = 0x7261,
  /// The round's quota, copied into kApplesLeft by start_round. $10 for a
  /// fresh level, and the timeout path at $7817 adds three to both.
  kApplesQuota = 0x7263,
  /// LEVEL, one BCD byte.
  kLevel = 0x7265,
  /// What $6255, the life timer, is loaded with when a life begins. Comes from
  /// $0304, the level's own allowance.
  ///
  /// $77BC sets it to $FF once the round's last apple is gone, which stops the
  /// timer rather than lengthening it: $6255 counts down one per pace, but
  /// game_draw_side_walls reads any value with bit 7 set as out of range,
  /// clamps the wall height and writes $FF back ($6B55), so the count never
  /// reaches zero and the run to the gate is untimed. Decoded from $641C and
  /// $6B55, not observed -- all three recordings seed it $64 and never reach
  /// the clamp.
  kLifeTime = 0x7266,
};

/* --- The settings block: $0300-$0305 -------------------------------------- */
/*
 * Six bytes that outlive a life, set once at $376E and then kept up to date as
 * the game goes on. Everything else about a life is torn down and rebuilt.
 */
enum {
  /// How long the pace loop dawdles between steps, i.e. the snake's speed.
  /// $52 at startup and at the top of every round; $772E takes two off at the
  /// start of each life and stops at 3, so the snake speeds up as lives are
  /// lost and never goes faster than that.
  kStepDelay = 0x0300,
  /// 0..2, chosen at the setup prompt. Indexes the per-apple value table at
  /// $71C8, and decides how many bouncers step per pass.
  kDifficulty = 0x0301,
  /// The game is playing itself, because nobody answered the difficulty
  /// prompt before it timed out. Any input at all clears it, and while it is
  /// set the death pause does not wait to be told to carry on.
  kDemoMode = 0x0302,
  /// Which of the 29 display lists at $8000 this level draws, 1-based.
  /// select_script skips that many '*'-terminated scripts to find it.
  kScriptIndex = 0x0303,
  /// $0304 -- the level's time allowance, which seeds kLifeTime at the start
  /// of every life. Set by the display list's 'T' command (see run_script),
  /// and $64 until one says otherwise. It is *not* the apple value, which is
  /// kAppleValue and is computed by game_set_apple_value from the difficulty
  /// and the level number; that routine never reads this byte.
  kLevelTime = 0x0304,
  /// The head moved this step, so the next draw merges the head shape over
  /// the cell. game_mark_head raises it, game_draw_head reads it and clears it.
  kHeadMoved = 0x0305,
};

/* --- The three zero-page pointers ----------------------------------------- */
enum {
  /// Into the current display list. game_next_byte reads through it and bumps
  /// it; select_script points it at the right script first.
  kScriptPtr = 0x000a,
  /// Into the string that follows a JSR to game_print_inline_str -- which is
  /// where the printer finds it, by reading the return address off the stack.
  kStrPtr = 0x000c,
  /// game_rand_byte's cursor. It is not a generator: it walks $1800-$1FFF and
  /// returns the first byte it finds with bit 7 clear, so the "random" numbers
  /// are the game's own level data, relocated there by the cold start.
  /// $7980 clamps the high byte into [$18,$1F) on the way into the setup
  /// screen, which is what keeps it inside that window.
  kRandPtr = 0x000e,
};

/// $73D7 -- the setup screen has run once. The first time through it asks
/// nothing, takes difficulty 1 and demo mode, and only sets this.
enum { kSetupSeen = 0x73d7 };

/// VALUE -- what one apple is worth on this level, two BCD bytes.
/// game_set_apple_value recomputes it per level.
enum { kAppleValue = 0x71cb };

/// The bonus screen's own two-byte BCD scratch, and the lives count it
/// compares against to decide whether the round earned a bonus at all.
enum { kBonusAmount = 0x78b0, kLivesAtLevelStart = 0x78b2 };

static uint8_t lives(void) {
  return ram_peek(kLives);
}

static void set_lives(uint8_t v) {
  ram_poke(kLives, v);
}

static uint8_t level(void) {
  return ram_peek(kLevel);
}

static void set_level(uint8_t v) {
  ram_poke(kLevel, v);
}

static uint8_t life_time(void) {
  return ram_peek(kLifeTime);
}

static void set_life_time(uint8_t v) {
  ram_poke(kLifeTime, v);
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
static const uint16_t kScoreByte[4] = {kScore + 3, kScore + 2, kScore + 1, kScore};
static const uint16_t kBestByte[4] = {kHiScore + 3, kHiScore + 2, kHiScore + 1, kHiScore};
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

  Cell c = {.col = ram_peek(kHeadCol), .row = 1};
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
    c.col = ram_peek(kHeadCol);

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
  ram_poke(kSearchCol, c.col);
  ram_poke(kSearchRow, c.row);
  ram_poke(kAppleCol, c.col);
  ram_poke(kAppleRow, c.row);
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
      .col = (uint8_t)(ram_peek(kColDelta + dir) + ram_peek(kHeadCol)),
      .row = (uint8_t)(ram_peek(kRowDelta + dir) + ram_peek(kHeadRow)),
  };
  s_status_v = ovf8(target.row, ram_peek(kRowDelta + dir), ram_peek(kHeadRow));
  ram_poke(kWantCol, target.col);
  ram_poke(kWantRow, target.row);

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
  ram_poke(kBounceBlocked, 0x00);
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
      ram_poke(kBounceBlocked, (uint8_t)free_neighbours);
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

/// The three runs, with their endpoints as arguments instead of as four
/// assignments before the call.
///
/// The globals are still where the steppers read them -- game_plot_hline_native
/// walks s_col up to s_run_end and leaves it there -- so these set them and
/// call, exactly as plot_shape_at does. What they buy is that a caller states
/// the whole run in one place, including the endpoint it used to inherit from
/// whatever ran before it.
/// Plot at a cell in a given ink, keeping whatever shape s_shape holds.
///
/// Separate from plot_shape_at because for these callers the shape genuinely
/// is inherited -- see s_shape -- and passing one would be inventing a value
/// the original does not have.
static void plot_at(uint8_t ink, Cell c) {
  s_ink = ink; // inherited by any run that follows -- see s_ink
  game_plot_shape_native(ink, c);
}

static void plot_hline_at(uint8_t col, uint8_t row, uint8_t to_col, uint16_t ret) {
  game_plot_hline(ret, (Cell){.col = col, .row = row}, to_col);
}

static void plot_vline_at(uint8_t col, uint8_t row, uint8_t to_row, uint16_t ret) {
  game_plot_vline(ret, (Cell){.col = col, .row = row}, to_row);
}

static void lores_vline_at(uint8_t col, uint8_t row, uint8_t to_row, uint16_t ret) {
  game_lores_vline(ret, (Cell){.col = col, .row = row}, to_row);
}

/// The ROM's PLOT.
static void lores_plot(uint8_t row, uint8_t col, uint16_t ret) {
  s_a = row;
  s_y = col;
  rom_plot(ret);
}

/// $7019 through its adapter: the next display-list byte.
static uint8_t script_byte(uint16_t ret) {
  game_next_byte_native();
  return s_a;
}

/// Graphics, hi-res, page 2, full screen. The reads are the writes.
static void select_hires_page2(void) {
  GAME_CYCLES(0x7048, 32);
  ram_poke(kTonePeriod, 0x00);
  io_peek(0xc050);
  io_peek(0xc057);
  io_peek(0xc055);
  io_peek(0xc052);
}

/// A plain three-deep delay. Y is the innermost counter and is whatever the
/// caller left in it -- the original does not initialise it.
///
/// The other two were the plotter's column and row, borrowed because zero page
/// was the only place to put them; $7056 loaded them and $7061 counted them
/// down. They are parameters. Nothing reads what the loop leaves behind:
/// wipe_occupancy_map overwrites the row on its first line, open_wall_gaps
/// touches neither, and the column is not read until draw_border writes it.
static void spin(uint8_t outer, uint8_t middle) {
  for (;;) {
    GAME_CYCLES(0x7061, 4);
    if (--s_y) {
      GAME_CYCLES(0x7062, 1);
      continue;
    }
    GAME_CYCLES(0x7064, 7);
    middle = (uint8_t)(middle - 1);
    if (middle) {
      GAME_CYCLES(0x7066, 1);
      continue;
    }
    GAME_CYCLES(0x7068, 7);
    outer = (uint8_t)(outer - 1);
    if (!outer)
      break;
    GAME_CYCLES(0x706a, 1);
  }
}

/// Clear the lo-res occupancy map, one full-width row at a time from the
/// bottom up. Ink 0 is black, so this erases.
static void wipe_occupancy_map(void) {
  GAME_CYCLES(0x706c, 13);
  uint8_t at = 0x27;
  set_ink(0x00, 0x7074);

  for (;;) {
    GAME_CYCLES(0x7075, 16);
    s_h2 = 0x27;
    lores_hline(at, 0x00, 0x707f);

    GAME_CYCLES(0x7080, 7);
    const uint8_t row = (uint8_t)(at - 1);
    at = row;
    // BPL: row 0 is drawn, and the loop ends one step later.
    if (row & 0x80)
      break;
    GAME_CYCLES(0x7082, 1);
  }
}

/// One gap per bouncer, which is what the difficulty counts.
static void open_wall_gaps(void) {
  GAME_CYCLES(0x7093, 6);
  const uint8_t difficulty = ram_peek(kDifficulty);
  if (!difficulty) {
    GAME_CYCLES(0x7096, 1);
    return;
  }

  GAME_CYCLES(0x7098, 10);
  lores_plot(0x01, 0x01, 0x709d);

  GAME_CYCLES(0x709e, 8);
  if (ram_peek(kDifficulty) == 0x01) {
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

  // The four sides, twice: once on the lo-res occupancy map and once in
  // hi-res. Four of these seven used to leave the endpoint out and inherit
  // $27 from the call above; it is written at each of them now.
  GAME_CYCLES(0x70ba, 21);
  lores_vline_at(0x00, 0x00, 0x27, 0x70c8);

  GAME_CYCLES(0x70c9, 16);
  lores_vline_at(0x27, 0x00, 0x27, 0x70d3);

  GAME_CYCLES(0x70d4, 19);
  plot_hline_at(0x00, 0x00, 0x27, 0x70e0);

  GAME_CYCLES(0x70e1, 16);
  plot_hline_at(0x00, 0x27, 0x27, 0x70eb);

  GAME_CYCLES(0x70ec, 14);
  plot_vline_at(0x00, 0x00, 0x27, 0x70f4);

  GAME_CYCLES(0x70f5, 16);
  plot_vline_at(0x27, 0x00, 0x27, 0x70ff);

  // Ink 3 over columns $12-$16 of the bottom row, on top of the border just
  // laid down: the gap the snake leaves through.
  GAME_CYCLES(0x7100, 26);
  s_ink = 0x03;
  plot_hline_at(0x12, 0x27, 0x16, 0x7112);
}

/// Walk the pointer to the current level's script, skipping one whole script
/// per level below it. DEX first, so level 1 skips nothing.
static void seek_script(void) {
  GAME_CYCLES(0x7113, 14);
  s_x = ram_peek(kScriptIndex);
  ram_poke(kScriptPtr, 0x00);
  ram_poke(kScriptPtr + 1, 0x80);

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
  spin(0x04, 0x00); // the counts $7056 used to store into $02/$03
  wipe_occupancy_map();

  GAME_CYCLES(0x7084, 21);
  s_wndtop = 0x14;
  s_shape = 0x15;
  s_ink = 0x0d;
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
      ram_poke(kScriptIndex, 0x01);
      goto restart;
    }
    GAME_CYCLES(0x7130, 1);

    GAME_CYCLES(0x713a, 4);
    if (op == OP_HLINE) {
      GAME_CYCLES(0x713e, 6);
      const uint8_t ink = script_byte(0x7140);
      GAME_CYCLES(0x7141, 9);
      s_ink = ink;
      const uint8_t col = script_byte(0x7145);
      GAME_CYCLES(0x7146, 9);
      const uint8_t last = script_byte(0x714a);
      GAME_CYCLES(0x714b, 9);
      const uint8_t row = script_byte(0x714f);
      GAME_CYCLES(0x7150, 12);
      set_ink(ink, 0x7156);

      GAME_CYCLES(0x7157, 18);
      s_h2 = last;
      lores_hline(row, col, 0x7161);

      GAME_CYCLES(0x7162, 6);
      plot_hline_at(col, row, last, 0x7164);
      GAME_CYCLES(0x7165, 3);
      continue;
    }
    GAME_CYCLES(0x713c, 1);

    GAME_CYCLES(0x7168, 4);
    if (op == OP_VLINE) {
      GAME_CYCLES(0x716c, 6);
      const uint8_t ink = script_byte(0x716e);
      GAME_CYCLES(0x716f, 9);
      s_ink = ink;
      const uint8_t row = script_byte(0x7173);
      GAME_CYCLES(0x7174, 9);
      const uint8_t last = script_byte(0x7178);
      GAME_CYCLES(0x7179, 9);
      const uint8_t col = script_byte(0x717d);
      GAME_CYCLES(0x717e, 12);
      set_ink(ink, 0x7184);

      // The lo-res half puts s_row back where it found it, which is what lets
      // the hi-res half run the same span without restating it.
      GAME_CYCLES(0x7185, 6);
      lores_vline_at(col, row, last, 0x7187);
      GAME_CYCLES(0x7188, 6);
      plot_vline_at(col, row, last, 0x718a);
      GAME_CYCLES(0x718b, 3);
      continue;
    }
    GAME_CYCLES(0x716a, 1);

    GAME_CYCLES(0x718e, 4);
    if (op == OP_PLOT) {
      GAME_CYCLES(0x7192, 6);
      const uint8_t ink = script_byte(0x7194);
      GAME_CYCLES(0x7195, 9);
      const uint8_t col = script_byte(0x7199);
      GAME_CYCLES(0x719a, 9);
      const uint8_t row = script_byte(0x719e);
      GAME_CYCLES(0x719f, 12);
      set_ink(ink, 0x71a5);

      GAME_CYCLES(0x71a6, 12);
      lores_plot(row, col, 0x71ac);
      GAME_CYCLES(0x71ad, 6);
      game_plot_shape_native(ink, (Cell){.col = col, .row = row});
      GAME_CYCLES(0x71b0, 3);
      continue;
    }
    GAME_CYCLES(0x7190, 1);

    GAME_CYCLES(0x71b3, 4);
    if (op == OP_STORE) {
      GAME_CYCLES(0x71b7, 6);
      const uint8_t v = script_byte(0x71b9);
      GAME_CYCLES(0x71ba, 7);
      ram_poke(kLevelTime, v);
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
  return (uint16_t)(ram_peek(kHgrLineLo + row) | (ram_peek(kHgrLineHi + row) << 8));
}

/// Index into the 128-byte dot table at $6064: 16 inks of 8, four column
/// phases in each of two scanline parities.
static uint8_t dot_index(uint8_t ink, uint8_t scanline, uint8_t col) {
  return (uint8_t)((uint8_t)(((ink << 1) | (scanline & 1)) << 2) | (col & 3));
}

/// $60E7 -- draw the loaded shape into one cell, replacing what was there.
uint8_t game_draw_cell_native(uint8_t ink, Cell c) {
  uint8_t scanline, dot_idx, hgr_lo, hgr_hi;
  GAME_CYCLES(0x60e7, 22);
  uint16_t dest = cell_row_base(c.row);
  scanline = 0x00;
  hgr_lo = (uint8_t)dest;
  hgr_hi = (uint8_t)(dest >> 8);

  for (unsigned line = 0; line < 4; ++line) {
    GAME_CYCLES(0x60f7, 16);
    // Built in $06 in two steps, and written out between them because it is
    // zero page and a probe may sample there.
    dot_idx = (uint8_t)((ink << 1) | (line & 1));
    GAME_CYCLES(0x6100, 62);
    const uint8_t idx = dot_index(ink, (uint8_t)line, c.col);
    dot_idx = idx;

    poke(dest + c.col, (uint8_t)(ram_peek(kHgrPattern + idx) & ram_peek(kShapeMask + line)));

    scanline = (uint8_t)(line + 1);
    dest += 0x0400; // one scanline down, i.e. +4 on the high byte
    hgr_hi = (uint8_t)(dest >> 8);

    if (line != 3)
      GAME_CYCLES(0x6124, 1);
  }

  GAME_CYCLES(0x6126, 6);
  // The high byte the loop ended on. Its caller's caller puts it in A.
  return hgr_hi;
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
uint8_t game_merge_cell_native(uint8_t ink, Cell c) {
  uint8_t scanline, dot_idx, hgr_lo, hgr_hi;
  GAME_CYCLES(0x6b96, 22);
  uint16_t dest = cell_row_base(c.row);
  scanline = 0x00;
  hgr_lo = (uint8_t)dest;
  hgr_hi = (uint8_t)(dest >> 8);

  for (unsigned line = 0; line < 4; ++line) {
    GAME_CYCLES(0x6ba6, 85);
    const uint8_t parity = (uint8_t)(line & 1);
    const uint8_t idx =
        (uint8_t)((uint8_t)(((uint8_t)((parity << 7) | (ink >> 1))) << 2) | (c.col & 3));
    dot_idx = idx;

    const uint16_t at = dest + c.col;
    poke(at,
         (uint8_t)(((ram_peek(kHgrPattern + idx) ^ 0x7f) & ram_peek(kShapeMask + line)) | peek(at)));

    scanline = (uint8_t)(line + 1);
    dest += 0x0400;
    hgr_hi = (uint8_t)(dest >> 8);

    if (line != 3)
      GAME_CYCLES(0x6bd7, 1);
  }

  GAME_CYCLES(0x6bd9, 6);
  return hgr_hi;
}

/// $702B -- zero hi-res page 1, $2000 through $3FFF. The inner loop runs a
/// full 256 bytes because Y wraps, so the terminating test is on the page.
void game_clear_hgr_native(void) {
  uint8_t hgr_lo, hgr_hi;
  GAME_CYCLES(0x702b, 12);
  hgr_lo = 0x00;
  hgr_hi = 0x20;
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
    hgr_hi = page;
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

/// $6148 -- a horizontal run of hi-res cells, from s_col to s_run_end along
/// row s_row.
///
/// The coordinate stays a global rather than becoming a parameter because it
/// is read back on the way out: this loop leaves s_col on its endpoint, which
/// the adapter returns in A, and game_draw_cell reads s_col/s_row as the cell
/// to draw. Threading it means restructuring the steppers and both cell
/// drawers together.
uint8_t game_plot_hline_native(uint8_t ink, Cell c, uint8_t to_col) {
  for (;;) {
    GAME_CYCLES(0x614b, 6);
    game_draw_cell(0x614d, ink, c);

    GAME_CYCLES(0x614e, 8);
    if (c.col == to_col)
      break;

    GAME_CYCLES(0x6154, 8);
    c.col = (uint8_t)(c.col + 1);
  }
  GAME_CYCLES(0x6152, 1);
  GAME_CYCLES(0x6159, 6);
  return c.col; // where the CMP that ended the loop found it
}

/// $615A -- the same down a column: rows $03 through $08 in column $02.
uint8_t game_plot_vline_native(uint8_t ink, Cell c, uint8_t to_row) {
  for (;;) {
    GAME_CYCLES(0x615d, 6);
    game_draw_cell(0x615f, ink, c);

    GAME_CYCLES(0x6160, 8);
    if (c.row == to_row)
      break;

    GAME_CYCLES(0x6166, 8);
    c.row = (uint8_t)(c.row + 1);
  }
  GAME_CYCLES(0x6164, 1);
  GAME_CYCLES(0x6159, 6);
  return c.row;
}

/// $7000 -- the lo-res half of a vertical run. Unlike the hi-res one it puts
/// $03 back where it found it, because the caller draws the hi-res run over
/// the same coordinates next.
uint8_t game_lores_vline_native(Cell c, uint8_t to_row) {
  // The original saves the starting row and puts it back, which is what lets
  // the hi-res half of a display list's 'V' run the same span after it.
  push8(c.row);

  for (;;) {
    GAME_CYCLES(0x7003, 12);
    lores_plot(c.row, c.col, 0x7009);

    GAME_CYCLES(0x700a, 8);
    if (c.row == to_row)
      break;

    GAME_CYCLES(0x7010, 8);
    c.row = (uint8_t)(c.row + 1);
  }
  GAME_CYCLES(0x700e, 1);
  GAME_CYCLES(0x7015, 13);
  return pop8();
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
  ram_poke(kBouncer, in.col);
  ram_poke(kBouncer + 1, in.row);
  ram_poke(kBouncer + 2, (uint8_t)in.dx);
  ram_poke(kBouncer + 3, (uint8_t)in.dy);
  game_move_bouncer(ret);

  GAME_CYCLES(back_block, back_cycles);
  const Bouncer out = {
      .col = ram_peek(kBouncer),
      .row = ram_peek(kBouncer + 1),
      .dx = (int8_t)ram_peek(kBouncer + 2),
      .dy = (int8_t)ram_peek(kBouncer + 3),
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
  const uint8_t at = ram_peek(kRingRead);
  const uint8_t key = ram_peek(kKeyRing + at);
  if (!(key & 0x80)) {
    GAME_CYCLES(0x6206, 1);
    return key;
  }

  GAME_CYCLES(0x6208, 24);
  ram_poke(kKeyRing + at, 0x00);
  ram_poke(kRingRead, (uint8_t)((at + 1) & 0x0f));
  // X *is* live out of $6594 -- `apple2tc --ir` says so -- unlike X out of
  // $6C72, where the same check let the write go. So it is maintained.
  s_x = (uint8_t)(at + 1);
  return key;
}

/// $6594 -- step as many bouncers as the difficulty calls for, then fall into
/// the key dequeue whose byte is the return value.
uint8_t game_step_bouncers_native(void) {
  GAME_CYCLES(0x6594, 6);
  const uint8_t difficulty = ram_peek(kDifficulty);

  if (!difficulty) {
    GAME_CYCLES(0x6599, 3);
    return dequeue_key();
  }
  GAME_CYCLES(0x6597, 1);

  step_bouncer_slot(0, 0x659c, 38, 0x65b6, 0x65b7, 40);

  if (ram_peek(kDifficulty) == 0x01) {
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
/// The substitution went unchecked for a while, and coverage could not say so.
/// Both blocks that perform it run constantly -- play.pkeys presses I, J, K
/// and M all through the round -- but those are the *default* bindings, where
/// the two tables hold identical bytes and reading the wrong one is invisible.
/// play-hires.pkeys does rebind, and then the recording ends without playing.
/// Swapping input_code for input_key passed verify.sh 4/4, all three traces,
/// memory and screen.
///
/// play-rebind.pkeys closes it: bound to W A S Z Q E and then played with
/// them, so the tables differ across all 8 substitutions it performs. The same
/// swap now fails trace-ext.
enum { kInputCount = 6 };

static uint8_t input_key(int i) {
  return ram_peek(kKeyTable + i);
}

static uint8_t input_code(int i) {
  return ram_peek(kKeyDefaults + i);
}

/// The two codes that are settings rather than directions.
enum { kCodeJoystickOn = 0x80, kCodeJoystickOff = 0x8b };

/// The code every input collapses to in attract mode. Its caller stores $FF
/// at $6253, which ends the game in progress.
enum { kCodeStop = 0x92 };

/// $6C71 -- set once the player has chosen the joystick.
static bool joystick_selected(void) {
  return ram_peek(kJoystick) != 0;
}

static void select_joystick(bool on) {
  ram_poke(kJoystick, on ? 0x01 : 0x00);
}

/// $0302 -- attract mode: nobody answered the difficulty prompt before it
/// timed out, so the game is playing itself. Any input at all ends it, which
/// is why the whole key table is skipped below.
static bool attract_mode(void) {
  return ram_peek(kDemoMode) != 0;
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
  const uint8_t joystick = ram_peek(kJoystick);
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
  return ram_peek(kArrowCH + slot);
}

static uint8_t slot_row(int slot) {
  return ram_peek(kArrowCV + slot);
}

static uint8_t slot_glyph(int slot) {
  return ram_peek(kArrowGlyph + slot);
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
  // The original parks the slot at $0002 for the whole routine, because COUT
  // clobbers X and every step below needs it again. Here it is the parameter,
  // and the glyph blitter no longer has anything to clobber it with.
  GAME_CYCLES(0x75d1, 23);
  s_ch = slot_col(slot);
  s_cv = slot_row(slot);
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
  s_ch = slot_col(slot);
  s_cv = slot_row(slot);
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
  return ram_peek(kTonePeriod);
}

static void set_tone_period(uint8_t v) {
  ram_poke(kTonePeriod, v);
}

/// Passes left before the next click.
static uint8_t tone_countdown(void) {
  return ram_peek(kToneCountdown);
}

static void set_tone_countdown(uint8_t v) {
  ram_poke(kToneCountdown, v);
}

/// Where the click goes, as the low byte of the soft switch: $C030 is the
/// speaker and $C020 the cassette output, which nobody can hear. Muting is
/// therefore a store rather than a branch, and the click itself is one indexed
/// read -- see the $7642 header for why that shape was chosen.
static void set_click_port(uint8_t lo) {
  ram_poke(kClickPort, lo);
}

/// $69C2 -- toggled by Ctrl-S at $69B9.
static bool sound_muted(void) {
  return ram_peek(kSoundMuted) != 0;
}

/// $69B9 -- flip it. The storage stays in emulated RAM rather than becoming a
/// C variable because ram.probe hashes $6000-$BFFF, so moving it would read as
/// a divergence rather than as a refactor.
static void toggle_sound(void) {
  ram_poke(kSoundMuted, (uint8_t)(ram_peek(kSoundMuted) ^ 0x01));
}

void game_tick_sound_native(void) {
  GAME_CYCLES(0x6bfb, 6);
  ram_poke(kTonePasses, 0x14); // twenty

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
          const uint8_t port = ram_peek(kClickPort);
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
    const uint8_t left = (uint8_t)(ram_peek(kTonePasses) - 1);
    ram_poke(kTonePasses, left);
    if (!left)
      break;
    GAME_CYCLES(0x6c43, 1);
  }
  GAME_CYCLES(0x6c45, 6);
}

/* ========================================================================== */
/* $71F3 -- a BCD byte, with leading zeros suppressed                         */
/* ========================================================================== */

/// $002C -- the significance flag, holding the last significant digit rather
/// than a plain 1. Its caller clears it before the first byte of a number, so
/// leading zeros print nothing and interior ones print; $7226 consults it
/// after the last byte, and prints a single "0" if the whole number was.
static bool digit_seen(void) {
  return s_h2 != 0;
}

static void note_digit(uint8_t digit) {
  s_h2 = digit;
}

enum { kCharZero = 0xb0 };

/// `CLC / ADC #$B0`, which is what turns 0-9 into the character for it.
///
/// The original's ADC honours the D flag; this addition does not. Digits are
/// printed with D clear -- $7267 is the only thing in the game that sets it,
/// and it clears it again before returning -- so say so loudly rather than
/// carry a decimal path that cannot be reached.
static void cout_digit(uint8_t digit, uint16_t ret) {
  if (s_status_d) {
    fprintf(stderr, "cout_digit: entered with decimal mode set\n");
    error_handler(0x71f3);
    abort();
  }
  s_a = (uint8_t)(kCharZero + digit);
  rom_cout(ret);
}

void game_print_bcd_native(uint8_t byte) {
  const uint8_t high = (uint8_t)(byte >> 4);

  GAME_CYCLES(0x71f3, 15);
  if (!high) {
    GAME_CYCLES(0x71fa, 1);
  } else {
    GAME_CYCLES(0x71fc, 3);
    note_digit(high);
  }

  GAME_CYCLES(0x71fe, 8);
  if (digit_seen()) {
    GAME_CYCLES(0x7201, 1);
    GAME_CYCLES(0x7207, 14);
    cout_digit(high, 0x720d);
  } else {
    // A leading zero: dropped, and nothing is printed.
    GAME_CYCLES(0x7203, 7);
  }

  const uint8_t low = (uint8_t)(byte & 0x0f);

  GAME_CYCLES(0x720e, 10);
  if (!low) {
    GAME_CYCLES(0x7213, 1);
  } else {
    GAME_CYCLES(0x7215, 3);
    note_digit(low);
  }

  GAME_CYCLES(0x7217, 8);
  if (digit_seen()) {
    GAME_CYCLES(0x721a, 1);
    GAME_CYCLES(0x721e, 14);
    cout_digit(low, 0x7224);
    GAME_CYCLES(0x7225, 6);
  } else {
    GAME_CYCLES(0x721c, 10);
  }
}

/// $7226 -- called after the last byte of a number: if nothing significant was
/// printed, the number was zero, and one "0" is printed for the whole of it.
void game_print_zero_if_blank_native(void) {
  GAME_CYCLES(0x7226, 5);
  if (digit_seen()) {
    GAME_CYCLES(0x7228, 1);
    GAME_CYCLES(0x722f, 6);
    return;
  }

  GAME_CYCLES(0x722a, 5);
  s_a = kCharZero;
  rom_cout(0x0000); // JMP $FDED -- a tail call, so no return address.
}

/* ========================================================================== */
/* $7267 -- add to the score                                                  */
/* ========================================================================== */

void game_add_score_native(void) {
  GAME_CYCLES(0x7267, 56);

  // Decimal mode for the whole run, and adc_dec16 rather than a second
  // hand-written BCD adder: it is the one the emulator and the generated code
  // both use, so it cannot disagree with them about the undefined corners of
  // BCD ADC. It returns the sum in the low byte and the flags in the high one.
  s_status_d = 0x01;

  // Four bytes at $7252, least significant first, plus a two-byte value at
  // $71CB. The original adds the value into the low half and then propagates
  // the carry through the top half with `ADC #$00`, which flips the operand
  // order halfway -- kept, because adc_dec16 need not be symmetric over BCD
  // that is not valid BCD.
  unsigned carry = 0;
  uint8_t flags = 0;
  for (int i = 0; i < 4; ++i) {
    const uint8_t a = i < 2 ? ram_peek(kAppleValue + i) : ram_peek(kScore + i);
    const uint8_t m = i < 2 ? ram_peek(kScore + i) : 0x00;
    const uint16_t r = adc_dec16(a, m, carry);
    ram_poke(kScore + i, (uint8_t)r);
    flags = (uint8_t)(r >> 8);
    carry = flags & 0x01;
  }

  // Only the `easy` fixture checks the carry. Breaking the propagation between
  // bytes -- so the score never carries past $99 -- passes verify.sh 4/4, both
  // 1300-frame traces, memory and screen, and fails only the 3000-frame run
  // against snake-byte-easy.b33. Neither committed recording ever scores
  // enough to cross a byte boundary.
  //
  // C and V are live out of $7267 and N and Z are not, so only these are put
  // back -- `apple2tc --ir`, which also explains the D: it is live out too,
  // and the original's CLD is what makes it false.
  s_status_c = carry;
  s_status_v = ((flags & 0x40) != 0);
  s_status_d = 0x00;
}

/* ========================================================================== */
/* $7024, $7019 -- two small ones                                             */
/* ========================================================================== */

/// $7024 -- tell the ROM's lo-res plotter which colour to draw the occupancy
/// map in. Zero erases, anything else draws, so the map only ever holds colour
/// 0 or colour 5: it is a two-state map, not a picture.
///
/// The original takes its argument in the Z flag rather than in A, because
/// every caller reaches it with `LDA $01 / JSR $7024` and $01 is the same ink
/// byte the hi-res plotter takes. Here it is the byte, and the adapter asserts
/// that the flag agreed with it.
void game_set_ink_native(uint8_t ink) {
  GAME_CYCLES(0x7024, 2);
  if (!ink) {
    GAME_CYCLES(0x7024, 1);
  } else {
    GAME_CYCLES(0x7026, 2);
  }

  GAME_CYCLES(0x7028, 3);
  s_a = ink ? 0x05 : 0x00;
  rom_setcol(0x0000); // JMP $F864 -- a tail call.
}

/// $7019 -- read the byte the $000A pointer addresses and advance it. The
/// display-list interpreter's only way of reading its script.
void game_next_byte_native(void) {
  GAME_CYCLES(0x7019, 14);
  s_y = 0x00;
  s_a = peek(ram_peek16al(kScriptPtr));

  const uint8_t lo = (uint8_t)(ram_peek(kScriptPtr) + 1);
  ram_poke(kScriptPtr, lo);
  if (lo) {
    GAME_CYCLES(0x701f, 1);
  } else {
    GAME_CYCLES(0x7021, 5);
    ram_poke(kScriptPtr + 1, (uint8_t)(ram_peek(kScriptPtr + 1) + 1));
  }
  GAME_CYCLES(0x7023, 6);
  // A and Y are live out; N and Z are not.
}

/* ========================================================================== */
/* $6C4B, $7642, $71CD -- apples                                              */
/* ========================================================================== */

/// $6C4B -- the game's random number. A pointer at $000E walks memory from
/// $1800 upward and the first byte with bit 7 clear is the answer; the pointer
/// is reset to $1800 whenever it finds one that is not. Not random, but
/// unpredictable enough to place an apple and it costs nothing to keep.
///
/// There is no generator at all: program text and data are the entropy. The
/// result is therefore always $00-$7F, which is what makes it usable directly
/// as a coordinate.
///
/// The restart does not advance the pointer -- it stores $1800 and jumps
/// straight back to the load -- so a byte at $1800 with bit 7 set would hang
/// the game. Nothing enforces that; the original simply relies on it.
uint8_t game_rand_byte_native(void) {
  GAME_CYCLES(0x6c4b, 7);
  const uint8_t lo = (uint8_t)(ram_peek(kRandPtr) + 1);
  ram_poke(kRandPtr, lo);
  if (lo) {
    GAME_CYCLES(0x6c4d, 1);
  } else {
    GAME_CYCLES(0x6c4f, 5);
    ram_poke(kRandPtr + 1, (uint8_t)(ram_peek(kRandPtr + 1) + 1));
  }

  for (;;) {
    GAME_CYCLES(0x6c51, 9);
    const uint8_t b = peek(ram_peek16al(kRandPtr));
    if (!(b & 0x80)) {
      GAME_CYCLES(0x6c55, 1);
      GAME_CYCLES(0x6c62, 6);
      return b;
    }

    GAME_CYCLES(0x6c57, 13);
    ram_poke(kRandPtr, 0x00);
    ram_poke(kRandPtr + 1, 0x18);
  }
}

/// Add one, in BCD, to the two-byte counter at \p at. Returns the flags
/// adc_dec16 left on the second byte, because two of them are live out of
/// $7642.
static uint8_t bcd_inc16(uint16_t at) {
  s_status_d = 0x01;
  uint16_t r = adc_dec16(ram_peek(at), 0x01, 0x00);
  ram_poke(at, (uint8_t)r);

  r = adc_dec16(ram_peek(at + 1), 0x00, (uint8_t)(r >> 8) & 0x01);
  s_a = (uint8_t)r;
  ram_poke(at + 1, s_a);
  s_status_d = 0x00;
  return (uint8_t)(r >> 8);
}

Cell game_place_apple_native(void) {
  // Rejection sampling: two pseudo-random bytes as column and row, ask the
  // lo-res map whether that cell is free, and start over if it is not.
  // game_rand_byte returns $00-$7F while the field is 40x40, so most draws
  // land outside it and hit the border or garbage -- the retry loop does far
  // more work than it looks like.
  Cell at = {0, 0};
  for (;;) {
    GAME_CYCLES(0x7642, 6);
    at.col = game_rand_byte_native();

    GAME_CYCLES(0x7645, 9);
    at.row = game_rand_byte_native();

    GAME_CYCLES(0x764a, 15);
    const bool taken = cell_taken(at.col, at.row, 0x7652);

    GAME_CYCLES(0x7653, 2);
    if (!taken)
      break;
    GAME_CYCLES(0x7653, 1);
  }

  // White on the occupancy map, so the snake's collision test sees it.
  GAME_CYCLES(0x7655, 8);
  s_a = 0x0f;
  rom_setcol(0x7659);

  GAME_CYCLES(0x765a, 12);
  s_a = at.row;
  s_y = at.col;
  rom_plot(0x7660);

  GAME_CYCLES(0x7661, 16);
  plot_shape_at(0x01, 0x09, at);

  // One more apple on screen. $77D0 watches this pair and calls back here when
  // it reaches zero.
  GAME_CYCLES(0x766c, 32);
  const uint8_t flags = bcd_inc16(kApplesAfield);
  s_status_c = (flags & 0x01);
  s_status_v = ((flags & 0x40) != 0);
  return at;
}

/// $71CD -- what one apple is worth: the difficulty's entry in the $71C8 table
/// added to itself once per level, in BCD, into $71CB. X is never touched in
/// the original's loop, which is what makes it the same entry every time.
void game_set_apple_value_native(void) {
  GAME_CYCLES(0x71cd, 20);
  ram_poke(kAppleValue, 0x00);
  ram_poke(kAppleValue + 1, 0x00);
  const uint8_t per_apple = ram_peek(kAppleValueTable + ram_peek(kDifficulty));
  uint8_t levels = ram_peek(kScriptIndex);
  s_status_d = 0x01;

  uint8_t flags = 0;
  for (;;) {
    GAME_CYCLES(0x71dc, 28);
    uint16_t r = adc_dec16(per_apple, ram_peek(kAppleValue), 0x00);
    ram_poke(kAppleValue, (uint8_t)r);

    r = adc_dec16(ram_peek(kAppleValue + 1), 0x00, (uint8_t)(r >> 8) & 0x01);
    ram_poke(kAppleValue + 1, (uint8_t)r);
    flags = (uint8_t)(r >> 8);

    if (!--levels)
      break;
    GAME_CYCLES(0x71ef, 1);
  }

  GAME_CYCLES(0x71f1, 8);
  // V and D are the whole of this routine's live-out set.
  s_status_v = ((flags & 0x40) != 0);
  s_status_d = 0x00;
}

/* ========================================================================== */
/* $6BEF, $6BDA, $7633, $60E4 -- the head and the apple it eats               */
/* ========================================================================== */

/// $6BEF -- mark the head on the lo-res occupancy map, at the row and column
/// the caller has already loaded, and raise the two flags that say it is
/// there: $0305 for the next draw and $6C46 to start the tone.
void game_mark_head_native(void) {
  GAME_CYCLES(0x6bef, 6);
  rom_plot(0x6bf1);

  GAME_CYCLES(0x6bf2, 16);
  ram_poke(kHeadMoved, 0x01);
  ram_poke(kTonePeriod, 0x01);

  // A and its flags are live out of $6BEF, unlike almost everything else here.
  s_a = 0x01;
  s_status_not_z = 0x01;
  s_status_n = 0x00;
}

/// $6BDA -- draw the cell the caller set up, and if $0305 says the head is on
/// it, merge shape 1 over the top so the head reads as a head rather than
/// replacing the body cell underneath. $0305 is consumed here.
void game_draw_head_native(uint8_t ink, Cell c) {
  GAME_CYCLES(0x6bda, 6);
  game_plot_shape_native(ink, c);

  GAME_CYCLES(0x6bdd, 6);
  if (ram_peek(kHeadMoved)) {
    GAME_CYCLES(0x6be2, 11);
    s_shape = 0x01;
    game_plot_shape_merge(0x6be8, ink, c);
  } else {
    GAME_CYCLES(0x6be0, 1);
  }

  GAME_CYCLES(0x6be9, 12);
  ram_poke(kHeadMoved, 0x00);
  // Only V and D are live out, and neither is touched here.
}

/// $7633 -- an extra snake for clearing the round, and the noise that says so.
///
/// Not "eat an apple", which is what this was called: it BCD-increments $725E,
/// which the status panel prints as SNAKES LEFT, and its one caller is $7803 --
/// reached from $77EA, the round-cleared path, straight after the bonus
/// screen. Nothing about it runs when an apple is eaten; that path is $7743,
/// and it touches four other counters and not this one.
void game_award_extra_life_native(void) {
  GAME_CYCLES(0x7633, 22);
  s_status_d = 0x01;
  const uint16_t r = adc_dec16(lives(), 0x01, 0x00);
  set_lives((uint8_t)r);
  const uint8_t flags = (uint8_t)(r >> 8);
  s_status_c = (flags & 0x01);
  s_status_v = ((flags & 0x40) != 0);
  s_status_d = 0x00;
  game_sound_sweep_native();

  GAME_CYCLES(0x7641, 6);
}

/// $60E4 -- load a shape and draw it, which is the pair every caller wants.
void game_plot_shape_native(uint8_t ink, Cell c) {
  GAME_CYCLES(0x60e4, 6);
  game_load_shape(0x60e6);
  game_draw_cell(0x0000, ink, c); // JMP -- a tail call.
}

/* ========================================================================== */
/* $64A9, $7590, $6B3D                                                        */
/* ========================================================================== */

/// $64A9 -- the noise an apple makes. Two sweeps: X starts at 0, so the first
/// DEX wraps to 255 and the delay between clicks runs 256, 255, ... 1 and the
/// pitch rises; the second counts X up from 0, so the delay runs 256, 1, 2,
/// ... 255 and it falls again.
void game_sound_sweep_native(void) {
  GAME_CYCLES(0x64a9, 2);
  uint8_t x = 0x00;

  do {
    GAME_CYCLES(0x64ab, 4);
    uint8_t y = x;
    do {
      GAME_CYCLES(0x64ad, 4);
      if (--y)
        GAME_CYCLES(0x64ae, 1);
    } while (y);

    // The click, at whichever port $6C2C last chose. Neither the Y it loads
    // nor the byte it reads outlives the next pass.
    //
    // Nothing checks the port. Reading $C001+port instead of $C000+port
    // passes every oracle -- correctly, as it happens, since $C020-$C02F all
    // mirror the cassette toggle and $C030-$C03F the speaker. But hardcoding
    // $30 here would pass too, and that would be a real bug: the mute would
    // stop working and no oracle in this repo looks at sound.
    GAME_CYCLES(0x64b0, 12);
    peek((uint16_t)(0xc000 + ram_peek(kClickPort)));
    if (--x)
      GAME_CYCLES(0x64b7, 1);
  } while (x);

  uint8_t port = 0;
  uint8_t last = 0;
  do {
    GAME_CYCLES(0x64b9, 4);
    uint8_t y = x;
    do {
      GAME_CYCLES(0x64bb, 4);
      if (--y)
        GAME_CYCLES(0x64bc, 1);
    } while (y);

    GAME_CYCLES(0x64be, 12);
    port = ram_peek(kClickPort);
    last = peek((uint16_t)(0xc000 + port));
    if (++x)
      GAME_CYCLES(0x64c5, 1);
  } while (x);

  GAME_CYCLES(0x64c7, 6);

  // A, X, Y, N and Z are all live out of $64A9 -- so the last click's port and
  // the byte it read are, oddly, part of this routine's result.
  s_a = last;
  s_x = x;
  s_y = port;
  s_status_not_z = x;
  s_status_n = (x & 0x80);
}

/// $7590 -- show \p key as the binding of slot \p slot on the redefinition
/// screen. The two arrow keys have no printable glyph, so they are shown as
/// 'f' and 'g', which is where the arrow shapes live in the game's own font at
/// $66A9.
void game_show_key_native(uint8_t slot, uint8_t key) {
  GAME_CYCLES(0x7590, 7);

  uint8_t glyph = key;
  if (key == 0x88) { // left arrow
    GAME_CYCLES(0x7596, 2);
    glyph = 0xe6;
  } else {
    GAME_CYCLES(0x7594, 1);
  }

  GAME_CYCLES(0x7598, 4);
  if (glyph == 0x95) { // right arrow
    GAME_CYCLES(0x759c, 2);
    glyph = 0xe7;
  } else {
    GAME_CYCLES(0x759a, 1);
  }

  GAME_CYCLES(0x759e, 23);
  s_x = slot; // read back by the COUT hook, which is why it is set here
  s_ch = ram_peek(kKeyCH + slot);
  s_cv = ram_peek(kKeyCV + slot);
  rom_fc68(0x75ab);

  GAME_CYCLES(0x75ac, 10);
  s_a = glyph;
  s_status_not_z = glyph;
  s_status_n = (glyph & 0x80);
  rom_cout(0x75af);

  GAME_CYCLES(0x75b0, 9);
  s_x = slot;
  s_status_not_z = slot;
  s_status_n = (slot & 0x80);
}

/// $6B3D -- both side walls, each in two segments of different ink, with the
/// seam at a row derived from $6255. The seam is what the player aims for.
void game_draw_side_walls_native(void) {
  // The random byte is thrown away. The call is not: $6C4B advances the
  // pointer at $000E, so this is what keeps apple placement from repeating
  // level to level.
  GAME_CYCLES(0x6b3d, 6);
  (void)game_rand_byte_native();

  GAME_CYCLES(0x6b40, 26);
  s_shape = 0x15;
  s_ink = 0x02; // the upper segment

  uint8_t seed = ram_peek(kLifeTimer);
  if (seed & 0x80) {
    // A negative seed is clamped, and $6255 reset so the next call starts from
    // a known place.
    GAME_CYCLES(0x6b55, 8);
    ram_poke(kLifeTimer, 0xff);
    seed = 0x70;
  } else {
    GAME_CYCLES(0x6b53, 1);
  }

  // How far down the upper segment reaches, from the timer: the walls close in
  // as a life runs out.
  GAME_CYCLES(0x6b5c, 18);
  const uint8_t wall_top = (uint8_t)((seed >> 2) + 1);
  plot_vline_at(0x00, 0x01, wall_top, 0x6b64);

  GAME_CYCLES(0x6b65, 16);
  plot_vline_at(0x27, 0x01, wall_top, 0x6b6f);

  GAME_CYCLES(0x6b70, 30);
  const uint8_t seam = (uint8_t)(wall_top + 1);
  s_ink = 0x0d; // the lower segment
  plot_vline_at(0x27, seam, 0x27, 0x6b81);

  GAME_CYCLES(0x6b82, 18);
  plot_vline_at(0x00, seam, 0x27, 0x6b8b);

  // Tail call: SCRN of the bottom-centre cell, whose result the caller reads.
  GAME_CYCLES(0x6b8c, 7);
  s_a = 0x27;
  s_y = 0x14;
  rom_scrn(0x0000);
}

/* ========================================================================== */
/* $6217 -- the keyboard, into the ring                                       */
/* ========================================================================== */

/// A 16-entry ring at $623C, with $624D the write index and $624C the read
/// index; dequeue_key above is the other end of it.
///
/// If advancing the write index would land on the read index the buffer is
/// full and the key is dropped -- but note it has already been *stored* by
/// then. The byte is written and then disowned by not committing the index,
/// which is a byte of work saved and a slot of the ring left holding a key
/// nobody will read until it is overwritten.
void game_read_key_native(void) {
  // $6217 is on the replay coordinate, and is also where ram.probe and
  // screen.probe take their samples. It keeps its probe for both reasons.
  GAME_CYCLES_COORD(0x6217, 10);
  const uint8_t at = ram_peek(kRingWrite);
  const uint8_t key = io_peek(0xc000);
  s_a = key;
  s_x = at;

  if (key & 0x80) {
    GAME_CYCLES(0x621f, 21);
    io_poke(0xc010, key); // clear the strobe
    ram_poke(kKeyRing + at, key);

    // The $0F is the ring's size and nothing checks it: widening it to $1F
    // passes every oracle, because no recording ever presses sixteen keys
    // faster than the game reads them. Do not tidy it.
    const uint8_t next = (uint8_t)((at + 1) & 0x0f);
    s_x = (uint8_t)(at + 1);
    s_a = next;
    if (next != ram_peek(kRingRead)) {
      GAME_CYCLES(0x622e, 10);
      ram_poke(kRingWrite, next);
      return;
    }
    GAME_CYCLES(0x622c, 1);
  } else {
    GAME_CYCLES(0x621d, 1);
  }

  // The RTS belongs to the routine before this one, and both early exits
  // share it -- as does the key dequeue, whose adapter still emits it.
  GAME_CYCLES_SHARED(0x6216, 6);
  // A and X are the live-out set, and both are set above on every path.
}

/* ========================================================================== */
/* $664A -- the hi-res COUT hook                                              */
/*                                                                            */
/* Installed by $6641 over the ROM's character-output vector, so that every    */
/* COUT in the game and in the ROM draws through the game's own 8x8 font at    */
/* $66A9 instead of the text screen. Control characters are passed straight    */
/* on to $FDF0, which is what keeps the cursor, scrolling and the rest of      */
/* COUT's behaviour working underneath.                                       */
/*                                                                            */
/* Everything the original parked below $0009 is a local here. It used to have */
/* to stay in emulated RAM, because ram.probe hashed $0000-$00FF and the       */
/* residue had to match the original's down to the loop counter's last value;  */
/* that range left the hash when the storage moved, on 2026-08-23.             */
/* ========================================================================== */

/// Where glyph \p glyph's eight rows live. The font starts at $66A9 and the
/// first glyph in it is the space, $20.
static uint16_t glyph_rows(uint8_t glyph) {
  return (uint16_t)(0x66a9 + (uint16_t)(glyph - 0x20) * 8);
}

/// The hi-res address matching the text cursor. BASL/BASH at $28 point at the
/// text line, CH at $24 is the column; `- 4 + $20` on the high byte is
/// `+ $1C`, which maps $04xx (text page 1) onto $20xx (hi-res page 1).
static uint16_t hires_cursor(void) {
  const uint8_t hi = (uint8_t)(s_bash - 0x04 + 0x20);
  const uint8_t lo = (uint8_t)(s_basl + s_ch);
  return (uint16_t)(lo | (hi << 8));
}

void game_cout_hook_native(uint8_t ch) {
  GAME_CYCLES(0x664a, 9);
  const uint8_t glyph = (uint8_t)(ch & 0x7f);

  if (glyph >= 0x20) {
    GAME_CYCLES(0x664f, 1);
    GAME_CYCLES(0x6655, 82);

    // The original's SBC/ADC pairs honour the D flag. COUT is never reached in
    // decimal mode -- the ROM clears D at reset and neither BASIC nor the game
    // sets it around output -- so rather than carry dead decimal paths, fail
    // loudly if that assumption ever breaks.
    if (s_status_d) {
      fprintf(stderr, "game_cout_hook: entered with decimal mode set\n");
      error_handler(0x664a);
      abort();
    }

    // The original parks all of this in the plotter's zero-page block, which
    // it is not otherwise using. They are locals: the glyph, the caller's X
    // and Y, the source and the destination all die at the closing brace, and
    // nothing outside reads them. See the block's header for why the aliasing
    // had to be shown to be unobservable before they could be split out.
    const uint8_t saved_x = s_x, saved_y = s_y;
    const uint16_t src = glyph_rows(glyph);
    uint16_t dest = hires_cursor();
    s_x = 0x00;

    for (unsigned row = 0; row < 8; ++row) {
      GAME_CYCLES(0x668b, 33);
      poke(dest, peek((uint16_t)(src + row)));

      // One hi-res scanline down within the character cell, which is +$400.
      dest = (uint16_t)(dest + 0x0400);
      s_x = (uint8_t)(row + 1);

      // `INX / CPX #8 / BNE`: the branch is taken on every pass but the last.
      if (row != 7)
        GAME_CYCLES(0x669d, 1);
    }

    GAME_CYCLES(0x669f, 9);
    s_x = saved_x;
    s_y = saved_y;
  }

  GAME_CYCLES(0x6651, 7);
  s_a = ch; // PLA -- the high bit is still on it
  rom_cout1(0xfffe); // JMP $FDF0
}

/* ========================================================================== */
/* $6288 -- one life                                                          */
/*                                                                            */
/* The game's main loop: steer the snake, move it one cell, redraw the head    */
/* and the tail, and pace the whole thing with a delay loop that also ticks    */
/* the sound and polls the keyboard. It returns when the life ends, and the    */
/* five ways that can happen are LifeEnd.                                      */
/*                                                                            */
/* The direction lives at $624E and is 1..4. It is a cycle, so a turn is       */
/* ±1 and a wrap: $6232 and $6237 are the column and row deltas indexed by     */
/* it, and they read right, up, left, down -- decrementing turns clockwise.    */
/*                                                                            */
/* The shape drawn for a step is the direction plus $10, $08 or $04 depending  */
/* on which way the snake turned to get there, which is how the corner pieces  */
/* pick themselves. The original computes it before normalising the direction  */
/* back into range, so it is the *old* direction that selects the shape.       */
/*                                                                            */
/* Steering has two spellings. The arrow keys are relative -- $95 turns        */
/* clockwise, $88 anticlockwise -- and I/J/K/M are absolute, translated into   */
/* whichever turn would achieve them from where the snake is already going by  */
/* the four tables at $6387. Asking for the direction it is already going, or  */
/* for a reversal, yields $00 from those tables and is ignored.                */
/*                                                                            */
/* What is still in emulated RAM, and why: $624E-$6255 are read by the         */
/* generated caller at $7739 and by the routines this calls, and $6473 is read */
/* by nothing here but is hashed by ram.probe. The zero-page block $00-$03 is  */
/* the plotter's argument list. All of it goes when the storage does.          */
/* ========================================================================== */

/// The four directions, as $624E numbers them.
enum { DIR_RIGHT = 1, DIR_UP = 2, DIR_LEFT = 3, DIR_DOWN = 4 };

/// What the player can press, after game_read_direction_native() has had its
/// say. The arrows turn; the letters are the I/J/K/M diamond; $92 quits. The
/// last two are read by $69A9 alone, at the far end of the dispatch chain.
enum {
  KEY_TURN_CW = 0x95,
  KEY_TURN_CCW = 0x88,
  KEY_QUIT = 0x92,
  KEY_UP = 0xc9,
  KEY_LEFT = 0xca,
  KEY_RIGHT = 0xcb,
  KEY_DOWN = 0xcd,
  KEY_CTRL_S = 0x93,
  KEY_ESC = 0x9b,
};

/// $6387/$638C/$6391/$6396 -- turn an absolute-direction key into the relative
/// turn that achieves it from \p dir, or $00 for "nothing to do", which covers
/// both "already going that way" and "that would be a reversal".
static uint8_t turn_for_key(uint8_t key, uint8_t dir) {
  uint16_t table;
  switch (key) {
  case KEY_UP:
    table = 0x6387;
    break;
  case KEY_LEFT:
    table = 0x638c;
    break;
  case KEY_RIGHT:
    table = 0x6391;
    break;
  case KEY_DOWN:
    table = 0x6396;
    break;
  default:
    return 0;
  }
  return ram_peek(table + dir);
}

/// Draw \p shape into \p c with ink \p ink, through the plotter's zero-page
/// argument block.
static void plot_shape_at(uint8_t shape, uint8_t ink, Cell c) {
  s_shape = shape;
  s_ink = ink; // inherited by any run that follows -- see s_ink
  game_plot_shape_native(ink, c);
}

/// SCRN one cell.
static uint8_t scrn_cell(Cell c, uint16_t ret) {
  s_a = c.row;
  s_y = c.col;
  rom_scrn(ret);
  return s_a;
}

/// $649F -- one click of the speaker. $6C49 holds the port offset, $30 for
/// the speaker and $20 for the cassette output that nobody can hear, which is
/// how muting works; game_sound_sweep does the same thing at $64B0.
///
/// Not a keyboard read, which is what this was called until the scoreboard
/// pass went looking: the address is $C000 + $6C49, and the built-in symbol
/// database resolves the $C000 to KBD, so the disassembly reads `LDA KBD,Y`
/// and the index is what makes it the speaker.
static void click_speaker(void) {
  peek(0xc000 + ram_peek(kClickPort));
}

/* ========================================================================== */
/* $69A9 -- pause and mute                                                    */
/*                                                                            */
/* The dispatch chain above ends in a JSR here, so this is not only the pause */
/* key: it is where every key the game does not recognise comes to be         */
/* ignored. Both tests simply fail and it returns.                            */
/*                                                                            */
/* ESC spins on the keyboard until something is pressed, and the key that     */
/* ends the wait is left in A and falls through into the Ctrl-S test -- so    */
/* ESC and then Ctrl-S resumes and toggles the sound in a single pass. The    */
/* strobe is cleared after the wait rather than before it, so a key already   */
/* pending when ESC arrives ends the pause immediately.                       */
/*                                                                            */
/* Three of the six blocks are decoded from the binary rather than checked by */
/* a recording: $69AD and $69B2, the spin and the strobe clear, and $69B9,    */
/* the toggle. No committed recording presses either key. Converting takes    */
/* those addresses off the site list, so this comment is now the only record  */
/* that they are unverified.                                                  */
/*                                                                            */
/* What the other three do get is thinner than it looks, and it was measured  */
/* rather than assumed. Only play-rebind reaches this routine at all, and it  */
/* reaches it once. Inverting the Ctrl-S test is caught, by trace-ext on that */
/* scenario. Mis-charging the entry block is not: 4 cycles written as 5       */
/* passes verify.sh 4/4 and every probe scenario, because the one oracle that */
/* compares cycles (verify.sh) never executes this code, and the oracle that  */
/* does (probe-acceptance.sh) stamps its input on a probe counter precisely   */
/* so that it does not depend on cycles. 4 written as 4000 is caught, by      */
/* frame-boundary drift. So the cycle charges here are checked only to a      */
/* resolution of roughly a frame, and the numbers below rest on the opcode    */
/* timings, not on a passing test.                                            */
/* ========================================================================== */

uint8_t game_pause_or_toggle_sound_native(uint8_t key) {
  GAME_CYCLES(0x69a9, 4);
  if (key == KEY_ESC) {
    for (;;) {
      GAME_CYCLES(0x69ad, 6);
      key = io_peek(0xc000);
      if (key & 0x80)
        break;
      GAME_CYCLES(0x69b0, 1);
    }
    GAME_CYCLES(0x69b2, 4);
    io_poke(0xc010, key);
  } else {
    GAME_CYCLES(0x69ab, 1);
  }

  GAME_CYCLES(0x69b5, 4);
  if (key == KEY_CTRL_S) {
    GAME_CYCLES(0x69b9, 10);
    toggle_sound();
  } else {
    GAME_CYCLES(0x69b7, 1);
  }

  GAME_CYCLES(0x69c1, 6);
  return key;
}

LifeEnd game_play_loop_native(uint8_t *cell_out) {
  GAME_CYCLES(0x6288, 6);
  game_find_apple(0x628a);

  for (;;) {
    /* --- $628B: a key, and what the game makes of it -------------------- */
    GAME_CYCLES(0x628b, 6);
    game_read_key_native();
    GAME_CYCLES(0x628e, 6);
    game_read_direction(0x6290);
    uint8_t code = s_a;

    uint8_t dir = ram_peek(kDirection);
    uint8_t shape;

  dispatch: /* $6291 */
    GAME_CYCLES(0x6291, 2);
    if (!(code & 0x80)) {
      GAME_CYCLES(0x6291, 1);
      goto autopilot;
    }

  steer: /* $6293 -- a key with the high bit on, so the player is steering */
    GAME_CYCLES(0x6293, 10);
    ram_poke(kClickCount, 0x10);
    if (code == KEY_TURN_CW) {
      GAME_CYCLES(0x629c, 14);
      shape = (uint8_t)(dir + 0x10);
      // $624E is left one below range here and normalised at $62B8, which is
      // the order the samples see; computing the wrap early would be tidier
      // and would not match.
      ram_poke(kDirection, (uint8_t)(dir - 1));
      goto draw;
    }

    GAME_CYCLES(0x629a, 1);
    GAME_CYCLES(0x6306, 2);
    GAME_CYCLES(0x6306, 1);
    GAME_CYCLES(0x631e, 4);
    if (code == KEY_TURN_CCW) {
      GAME_CYCLES(0x6322, 17);
      shape = (uint8_t)(dir + 0x04);
      ram_poke(kDirection, (uint8_t)(dir + 1));
      goto draw;
    }

    GAME_CYCLES(0x6320, 1);
    GAME_CYCLES(0x6349, 4);
    if (code == KEY_QUIT) {
      GAME_CYCLES(0x634d, 12);
      return LIFE_QUIT;
    }

    // The four absolute keys, each a compare and a table lookup. Whatever the
    // table gives is examined as if the player had pressed it, which is why
    // this goes back to the top rather than falling through.
    {
      static const struct {
        uint8_t key;
        uint16_t cmp_addr, cmp_cycles, hit_addr, hit_cycles, edge_addr;
      } kAbsolute[] = {
          {KEY_UP, 0x6353, 4, 0x6357, 11, 0x634b},
          {KEY_LEFT, 0x6360, 4, 0x6364, 11, 0x6355},
          {KEY_RIGHT, 0x636d, 4, 0x6371, 11, 0x6362},
          {KEY_DOWN, 0x637a, 4, 0x637e, 11, 0x636f},
      };
      for (unsigned i = 0; i < 4; ++i) {
        GAME_CYCLES(kAbsolute[i].edge_addr, 1);
        GAME_CYCLES(kAbsolute[i].cmp_addr, kAbsolute[i].cmp_cycles);
        if (code == kAbsolute[i].key) {
          GAME_CYCLES(kAbsolute[i].hit_addr, kAbsolute[i].hit_cycles);
          code = turn_for_key(kAbsolute[i].key, dir);
          goto dispatch;
        }
      }
    }

    // $639B -- anything else is the pause/mute key.
    GAME_CYCLES(0x637c, 1);
    GAME_CYCLES(0x639b, 6);
    game_pause_or_toggle_sound(0x639d);
    GAME_CYCLES(0x639e, 3);
    goto pace;

  autopilot: /* $6308 -- no steering this step */
    GAME_CYCLES(0x6308, 6);
    if (ram_peek(kDemoMode)) {
      uint8_t proposal = 0;
      GAME_CYCLES(0x630d, 6);
      const SteerChoice choice = game_auto_steer(&proposal);
      if (choice == STEER_BOXED_IN) {
        // $6AB3 -- the auto-steer found nothing safe and gave up by jumping
        // here over its own return address. From this side that is simply
        // "carry straight on and take what comes".
        goto straight;
      }
      GAME_CYCLES(0x6310, 2);
      if (choice == STEER_TURN) {
        // $6312 -- it proposed a turn. Act on it as though it had been typed,
        // re-entering below the high-bit test the way $6312 does.
        GAME_CYCLES(0x6312, 3);
        code = proposal;
        goto steer;
      }
      GAME_CYCLES(0x6310, 1);
    } else {
      GAME_CYCLES(0x630b, 1);
    }

  straight: /* $6315 */
    GAME_CYCLES(0x6315, 11);
    shape = (uint8_t)(dir + 0x08);

  draw: /* $62A5 -- draw the head, then step it one cell */
  {
    GAME_CYCLES(0x62a5, 28);
    const Cell head = {.col = ram_peek(kHeadCol), .row = ram_peek(kHeadRow)};
    s_shape = shape;
    game_draw_head_native(0x0c, head);

    // $62B8 -- the direction back into 1..4, and the ink is the direction.
    GAME_CYCLES(0x62b8, 26);
    dir = (uint8_t)((((uint8_t)(ram_peek(kDirection) - 1)) & 3) + 1);
    ram_poke(kDirection, dir);
    s_a = dir;
    rom_setcol(0x62c7);

    GAME_CYCLES(0x62c8, 14);
    s_a = head.row;
    s_y = head.col;
    rom_plot(0x62d0);

    // $62D1 -- advance the head, and see what is there.
    GAME_CYCLES(0x62d1, 42);
    const Cell next = {
        .col = (uint8_t)(head.col + ram_peek(kColDelta + dir)),
        .row = (uint8_t)(head.row + ram_peek(kRowDelta + dir)),
    };
    ram_poke(kHeadCol, next.col);
    ram_poke(kHeadRow, next.row);
    const uint8_t cell = scrn_cell(next, 0x62ed);

    GAME_CYCLES(0x62ee, 25);
    ram_poke(kLifeOutcome, cell);
    GAME_CYCLES(0x6300, 6);
    plot_shape_at(dir, 0x0c, next);
    GAME_CYCLES(0x6303, 3);

    /* --- $6474: what did it move onto? ------------------------------- */
    GAME_CYCLES(0x6474, 6);
    if (cell == 0) {
      GAME_CYCLES(0x6479, 3);
      GAME_CYCLES(0x632e, 8);
      s_a = 0x07;
      rom_setcol(0x6332);
      GAME_CYCLES(0x6333, 14);
      s_a = next.row;
      s_y = next.col;
      rom_plot(0x633b);

      // $633C -- the gate is column $14 of row 0.
      GAME_CYCLES(0x633c, 8);
      if (next.col == 0x14) {
        GAME_CYCLES(0x6343, 6);
        if (next.row == 0) {
          GAME_CYCLES(0x6348, 6);
          return LIFE_GATE;
        }
        GAME_CYCLES(0x6346, 1);
      } else {
        GAME_CYCLES(0x6341, 1);
      }
      goto tail;
    }

    GAME_CYCLES(0x6477, 1);
    GAME_CYCLES(0x647c, 4);
    if (cell == 0x0f) {
      // $6480 -- an apple. Marked here; the caller does the scoring.
      GAME_CYCLES(0x6480, 14);
      ram_poke(kClickCount, 0x20);
      s_a = 0x07;
      rom_setcol(0x6489);
      GAME_CYCLES(0x648a, 14);
      s_a = next.row;
      s_y = next.col;
      game_mark_head_native();
      GAME_CYCLES(0x6493, 6);
      *cell_out = cell;
      return LIFE_APPLE;
    }

    // $6494 -- solid. Pause, buzzing, for a length taken byte by byte out
    // of ROM at $E000: nobody chose those numbers, they were simply there.
    GAME_CYCLES(0x647e, 1);
    GAME_CYCLES(0x6494, 6);
    // Both loops are DEY/BNE and DEX/BNE, which test *after* decrementing,
    // so a count of zero means 256 and not none. Ten of the bytes this reads
    // out of $E000 are zero, so that is the common case here rather than a
    // corner: getting it wrong costs 12,790 cycles of the pause, which is
    // three quarters of a frame and shifts everything after it.
    uint8_t x = 0xff;
    do {
      GAME_CYCLES(0x6498, 6);
      uint8_t y = peek(0xe000 + x);
      do {
        GAME_CYCLES(0x649c, 4);
        --y;
        if (y != 0)
          GAME_CYCLES(0x649d, 1);
      } while (y != 0);
      GAME_CYCLES(0x649f, 12);
      click_speaker();
      --x;
      if (x != 0)
        GAME_CYCLES(0x64a6, 1);
    } while (x != 0);
    GAME_CYCLES(0x64a8, 6);
    *cell_out = cell;
    return LIFE_CRASH;
  }

  tail: /* $63A1 -- trim the tail, unless the snake is still growing */
    GAME_CYCLES(0x63a1, 6);
    if (ram_peek(kGrowth)) {
      GAME_CYCLES(0x63a6, 15);
      ram_poke(kGrowth, (uint8_t)(ram_peek(kGrowth) - 1));
      ram_poke(kClickCount, 0x07);
    } else {
      GAME_CYCLES(0x63a4, 1);
      GAME_CYCLES(0x63b1, 14);
      const Cell tail = {.col = ram_peek(kTailCol), .row = ram_peek(kTailRow)};
      const uint8_t under = scrn_cell(tail, 0x63b9);

      // The original keeps `under` on the stack across the erase. It stays on
      // the emulated stack here too: ram.probe hashes the live stack, and a
      // sample taken inside the plotter would otherwise see a byte on one
      // engine and not the other.
      GAME_CYCLES(0x63ba, 11);
      push8(under);
      s_a = 0x00;
      rom_setcol(0x63bf);
      GAME_CYCLES(0x63c0, 14);
      s_a = tail.row;
      s_y = tail.col;
      rom_plot(0x63c8);
      GAME_CYCLES(0x63c9, 25);
      plot_at(0x00, tail);

      // $63DA -- the byte that was under the tail is the direction the tail
      // must follow, so the same delta tables move it on.
      GAME_CYCLES(0x63da, 44);
      const uint8_t tail_dir = pop8();
      const Cell tail_next = {
          .col = (uint8_t)(tail.col + ram_peek(kColDelta + tail_dir)),
          .row = (uint8_t)(tail.row + ram_peek(kRowDelta + tail_dir)),
      };
      ram_poke(kTailCol, tail_next.col);
      ram_poke(kTailRow, tail_next.row);
      const uint8_t ahead = scrn_cell(tail_next, 0x63f5);

      GAME_CYCLES(0x63f6, 32);
      plot_shape_at((uint8_t)(ahead + 0x0c), 0x0c, tail_next);
      GAME_CYCLES(0x640c, 3);
    }

  pace: /* $640F -- the timer, the walls, and the delay that sets the speed */
    GAME_CYCLES(0x640f, 20);
    click_speaker();
    {
      const uint8_t left = (uint8_t)(ram_peek(kLifeTimer) - 1);
      ram_poke(kLifeTimer, left);
      if (left == 0) {
        GAME_CYCLES(0x641c, 12);
        return LIFE_TIMEOUT;
      }
    }

    GAME_CYCLES(0x641a, 1);
    GAME_CYCLES(0x6422, 10);
    s_a = 0x27;
    s_y = 0x14;
    game_draw_side_walls_native();
    // The walls routine ends on a SCRN of the bottom-centre cell, and leaves
    // it in A -- that is its second result, and the original reads it here.
    GAME_CYCLES(0x6429, 4);
    if (s_a == 0) {
      // $642D -- the gate at the bottom is clear, so draw it. No edge charge
      // here: $6429's branch falls through to this and is only *taken* when
      // the cell is occupied.
      GAME_CYCLES(0x642d, 31);
      s_shape = 0x15;
      s_ink = 0x0d;
      plot_hline_at(0x12, 0x27, 0x16, 0x6443);
      GAME_CYCLES(0x6444, 8);
      s_a = 0x0d;
      rom_setcol(0x6448);
      GAME_CYCLES(0x6449, 10);
      s_a = 0x27;
      s_y = 0x14;
      rom_plot(0x644f);
    } else {
      GAME_CYCLES(0x642b, 1);
    }

    // $6450 -- the delay that sets the speed. $0300 iterations, each one
    // ticking the falling tone and taking a key, and counting $6473 down for
    // as long as the last move gave it something to say.
    // DEX/BNE again: $0300 of zero would mean 256 passes, not none.
    GAME_CYCLES(0x6450, 4);
    uint8_t n = ram_peek(kStepDelay);
    do {
      GAME_CYCLES(0x6453, 6);
      game_tick_sound_native();
      GAME_CYCLES(0x6456, 11);
      push8(n);
      game_read_key_native();
      GAME_CYCLES(0x645b, 6);
      if (ram_peek(kClickCount)) {
        GAME_CYCLES(0x6460, 18);
        click_speaker();
        ram_poke(kClickCount, (uint8_t)(ram_peek(kClickCount) - 1));
      } else {
        GAME_CYCLES(0x645e, 1);
      }
      GAME_CYCLES(0x646b, 10);
      n = pop8();
      --n;
      if (n != 0)
        GAME_CYCLES(0x646e, 1);
    } while (n != 0);
    GAME_CYCLES(0x6470, 3);
  }
}

/* ========================================================================== */
/* $6A32 -- the auto-steer                                                    */
/*                                                                            */
/* Chase the apple that $69C3 left at $6B3B/$6B3C. Candidate directions are    */
/* tried in order of how much they help, and the first one game_move_ok        */
/* accepts wins: the row is closed before the column, and each axis is tried   */
/* toward the apple before away from it.                                       */
/*                                                                            */
/* Two of the eight attempts repeat earlier ones, and the whole second half    */
/* re-tries the row after the column has failed. It is a search someone        */
/* unrolled by hand, and the order *is* the algorithm, so this keeps the order */
/* rather than folding the repeats away.                                       */
/*                                                                            */
/* None of the three committed recordings reaches the last three attempts or   */
/* the give-up. The auto-steer only runs when $0302 is set, which is a mode    */
/* chosen at the setup screen ($73DD, $7457, $7478) that none of them picks.   */
/* The blocks nothing exercises are $6A83, $6A8B, $6A9F, $6AA7, $6AA9, $6AB1   */
/* and $6AB3, decoded from the binary the way $664A was, and this comment is   */
/* now the only record of it -- converting the routine takes those addresses   */
/* off the site list, so probe-acceptance.sh no longer counts them among the   */
/* unverified.                                                                 */
/* ========================================================================== */

/// $6A55 -- the absolute-direction key that turns the snake to face \p dir.
/// Index 0 is unused; the four that matter are the I/J/K/M diamond.
static uint8_t key_for_direction(uint8_t dir) {
  return ram_peek(kSteerKey + dir);
}

/// Propose \p dir and report whether the move is allowed.
///
/// $6B38 holds the proposal because snake_move_verdict() reads it there, and
/// because the accept path at $6A48 reads it back out. The cycle charges are
/// arguments so that they land either side of the call, as the original's
/// STA/JSR and the BEQ after it do.
///
/// "Allowed" is whatever leaves Z set in the original, which is the two safe
/// verdicts -- and also a taken cell that happens to be the apple, since
/// eating it is the point.
static bool steer_try(
    uint8_t dir,
    uint16_t before_addr,
    unsigned before_cycles,
    uint16_t move_ok_ret,
    uint16_t after_addr,
    unsigned after_cycles) {
  GAME_CYCLES(before_addr, before_cycles);
  ram_poke(kSteerDir, dir);
  game_move_ok(move_ok_ret);
  GAME_CYCLES(after_addr, after_cycles);
  // The original branches on Z, which game_move_ok leaves set for exactly the
  // verdicts that permit the move -- including a target holding the apple.
  return s_status_not_z == 0;
}

SteerChoice game_auto_steer(uint8_t *key_out) {
  GAME_CYCLES(0x6a32, 10);
  const uint8_t apple_row = ram_peek(kAppleRow);
  const uint8_t apple_col = ram_peek(kAppleCol);
  const uint8_t head_row = ram_peek(kHeadRow);
  const uint8_t head_col = ram_peek(kHeadCol);

  bool settled = false;

  // $6A32 -- if the apple is on another row, close that first. This one test
  // is a BNE, so its edge is charged when the move is *refused*; every later
  // test is a BEQ to the accept path and charges its edge the other way round.
  if (apple_row != head_row) {
    GAME_CYCLES(0x6a3a, 4);
    uint8_t dir;
    if (apple_row >= head_row) {
      GAME_CYCLES(0x6a3c, 1);
      dir = DIR_DOWN;
    } else {
      GAME_CYCLES(0x6a3e, 2);
      dir = DIR_UP;
    }
    settled = steer_try(dir, 0x6a40, 10, 0x6a45, 0x6a46, 2);
    if (!settled)
      GAME_CYCLES(0x6a46, 1);
  } else {
    GAME_CYCLES(0x6a38, 1);
  }

  // $6A5A -- the column, toward the apple and then away from it.
  if (!settled) {
    GAME_CYCLES(0x6a5a, 10);
    if (apple_col >= head_col) {
      settled = steer_try(DIR_RIGHT, 0x6a62, 12, 0x6a69, 0x6a6a, 2);
      if (settled) {
        GAME_CYCLES(0x6a6a, 1);
      } else {
        settled = steer_try(DIR_LEFT, 0x6a6c, 12, 0x6a73, 0x6a74, 2);
        if (settled)
          GAME_CYCLES(0x6a74, 1);
        else
          GAME_CYCLES(0x6a76, 3);
      }
    } else {
      GAME_CYCLES(0x6a60, 1);
      settled = steer_try(DIR_LEFT, 0x6a79, 12, 0x6a80, 0x6a81, 2);
      if (settled) {
        GAME_CYCLES(0x6a81, 1);
      } else {
        settled = steer_try(DIR_RIGHT, 0x6a83, 12, 0x6a8a, 0x6a8b, 2);
        if (settled)
          GAME_CYCLES(0x6a8b, 1);
        // Refused: falls straight into $6A8D, where the other branch had to
        // spend a JMP to get.
      }
    }
  }

  // $6A8D -- the row again, now as an escape rather than as progress.
  if (!settled) {
    GAME_CYCLES(0x6a8d, 10);
    if (apple_row >= head_row) {
      settled = steer_try(DIR_DOWN, 0x6a95, 12, 0x6a9c, 0x6a9d, 2);
      if (settled)
        GAME_CYCLES(0x6a9d, 1);
    } else {
      GAME_CYCLES(0x6a93, 1);
    }
    if (!settled) {
      settled = steer_try(DIR_UP, 0x6a9f, 12, 0x6aa6, 0x6aa7, 2);
      if (settled) {
        GAME_CYCLES(0x6aa7, 1);
      } else {
        settled = steer_try(DIR_DOWN, 0x6aa9, 12, 0x6ab0, 0x6ab1, 2);
        if (settled) {
          GAME_CYCLES(0x6ab1, 1);
        } else {
          // $6AB3 -- nothing is safe.
          GAME_CYCLES(0x6ab3, 11);
          return STEER_BOXED_IN;
        }
      }
    }
  }

  // $6A48 -- a direction was accepted. Already going that way means there is
  // nothing to say; otherwise name the key that turns to it.
  GAME_CYCLES(0x6a48, 10);
  const uint8_t dir = ram_peek(kSteerDir);
  if (dir == ram_peek(kDirection)) {
    GAME_CYCLES(0x6a4e, 1);
    GAME_CYCLES(0x6a54, 6);
    *key_out = dir;
    return STEER_STRAIGHT;
  }
  GAME_CYCLES(0x6a50, 6);
  GAME_CYCLES(0x6a54, 6);
  *key_out = key_for_direction(dir);
  return STEER_TURN;
}

/* ========================================================================== */
/* $72CE -- the status panel                                                  */
/*                                                                            */
/* Six labelled numbers in a 2x3 grid on text rows $14-$16:                   */
/*                                                                            */
/*     SCORE:  ....        HI SCORE:  ....                                    */
/*     APPLES LEFT:  ..    VALUE:  ..                                         */
/*     SNAKES LEFT:  ..    LEVEL:  .                                          */
/*                                                                            */
/* Every field is the same four moves: place the cursor, print a label, clear  */
/* the leading-zero flag, then print BCD bytes most significant first and a    */
/* '0' if suppression ate all of them. The labels are inline strings after the */
/* JSR -- see game_print_inline_str -- so the "argument" each call passes is   */
/* the address the original pushed, and the text follows it.                   */
/*                                                                            */
/* $24 and $25 are the ROM's CH and CV. Only the last write to CV is followed  */
/* by VTAB, because COUT recomputes the line base itself on the way past.      */
/* Twenty-nine of the thirty block heads here keep their probes, spelled            */
/* GAME_CYCLES_SHARED. game_print_inline_str returns to an address it computes */
/* at run time, so every one of its return points is a dynamic block -- and    */
/* the generated C therefore still carries this routine's tail as cases        */
/* nothing can reach but that are still text in the file, and still on the     */
/* site list. The interpreter reports those addresses; so must this. Only      */
/* $72CE itself leaves, because that is the one the call now replaces.         */
/*                                                                            */
/* Every charge below is written out with a literal address rather than passed */
/* to a helper. A helper reads better and is wrong: the site-list lint finds   */
/* these by grepping for the literal, so an address reaching GAME_CYCLES       */
/* through a parameter is invisible to it, and nine of these were.            */
/* ========================================================================== */

/// $002C -- the flag game_print_bcd raises when it prints a digit, so that
/// game_print_zero_if_blank knows whether the field came out empty.
static void clear_leading_zero_flag(void) {
  s_h2 = 0x00;
}

void game_status_panel(void) {
  // SCORE, row $14 column $00. Four BCD bytes at $7252, little-endian.
  GAME_CYCLES(0x72ce, 16);
  s_cv = 0x14;
  s_ch = 0x00;
  game_print_inline_str(0x72d8);
  GAME_CYCLES(0x72e2, 15);
  clear_leading_zero_flag();
  s_a = ram_peek(kScore + 3);
  game_print_bcd(0x72eb);
  GAME_CYCLES(0x72ec, 10);
  s_a = ram_peek(kScore + 2);
  game_print_bcd(0x72f1);
  GAME_CYCLES(0x72f2, 10);
  s_a = ram_peek(kScore + 1);
  game_print_bcd(0x72f7);
  GAME_CYCLES(0x72f8, 10);
  s_a = ram_peek(kScore);
  game_print_bcd(0x72fd);
  GAME_CYCLES(0x72fe, 6);
  game_print_zero_if_blank_native();

  // HI SCORE, same row, column $14. Four bytes at $7256.
  GAME_CYCLES(0x7301, 11);
  s_ch = 0x14;
  game_print_inline_str(0x7307);
  GAME_CYCLES(0x7314, 15);
  clear_leading_zero_flag();
  s_a = ram_peek(kHiScore + 3);
  game_print_bcd(0x731d);
  GAME_CYCLES(0x731e, 10);
  s_a = ram_peek(kHiScore + 2);
  game_print_bcd(0x7323);
  GAME_CYCLES(0x7324, 10);
  s_a = ram_peek(kHiScore + 1);
  game_print_bcd(0x7329);
  GAME_CYCLES(0x732a, 10);
  s_a = ram_peek(kHiScore);
  game_print_bcd(0x732f);
  GAME_CYCLES(0x7330, 6);
  game_print_zero_if_blank_native();

  // APPLES LEFT, row $15 column $00. Two bytes at $725A.
  GAME_CYCLES(0x7333, 16);
  s_ch = 0x00;
  s_cv = 0x15;
  game_print_inline_str(0x733d);
  GAME_CYCLES(0x734d, 15);
  clear_leading_zero_flag();
  s_a = ram_peek(kApplesLeft + 1);
  game_print_bcd(0x7356);
  GAME_CYCLES(0x7357, 10);
  s_a = ram_peek(kApplesLeft);
  game_print_bcd(0x735c);
  GAME_CYCLES(0x735d, 6);
  game_print_zero_if_blank_native();

  // A space, which the next field's cursor move immediately overrides. It is
  // there to wipe the character one place past this field, left over from a
  // longer count earlier in the game.
  GAME_CYCLES(0x7360, 8);
  s_a = 0xa0;
  rom_cout(0x7364);

  // VALUE, same row, column $14. Two bytes at $71CB -- the current worth of an
  // apple, which game_set_apple_value computes per level.
  GAME_CYCLES(0x7365, 11);
  s_ch = 0x14;
  game_print_inline_str(0x736b);
  GAME_CYCLES(0x7375, 15);
  clear_leading_zero_flag();
  s_a = ram_peek(kAppleValue + 1);
  game_print_bcd(0x737e);
  GAME_CYCLES(0x737f, 10);
  s_a = ram_peek(kAppleValue);
  game_print_bcd(0x7384);
  GAME_CYCLES(0x7385, 6);
  game_print_zero_if_blank_native();

  // SNAKES LEFT, row $16 column $00. One byte at $725E, printed as though it
  // were the low half of a two-byte field: the high half is the literal 0
  // below, which prints nothing at all once leading zeros are suppressed. It
  // costs a call to keep the shape of every other field.
  GAME_CYCLES(0x7388, 16);
  s_cv = 0x16;
  s_ch = 0x00;
  game_print_inline_str(0x7392);
  GAME_CYCLES(0x73a2, 11);
  s_a = 0x00;
  clear_leading_zero_flag();
  game_print_bcd(0x73a8);
  GAME_CYCLES(0x73a9, 10);
  s_a = lives();
  game_print_bcd(0x73ae);
  GAME_CYCLES(0x73af, 6);
  game_print_zero_if_blank_native();

  // LEVEL, same row, column $14. One byte at $7265.
  GAME_CYCLES(0x73b2, 11);
  s_ch = 0x14;
  game_print_inline_str(0x73b8);
  GAME_CYCLES(0x73c2, 15);
  clear_leading_zero_flag();
  s_a = level();
  game_print_bcd(0x73cb);
  GAME_CYCLES(0x73cc, 6);
  game_print_zero_if_blank_native();

  // Home the cursor. This CV write is the one that needs VTAB, because nothing
  // prints after it to recompute the line base.
  GAME_CYCLES(0x73cf, 11);
  s_a = 0x00;
  s_cv = 0x00;
  rom_fc68(0x73d5);
  GAME_CYCLES(0x73d6, 6);
}

/* ========================================================================== */
/* $78B3 -- the bonus screen                                                  */
/*                                                                            */
/* Awarded when a level is finished. The bonus is twice whatever an apple was  */
/* worth on that level, which the original says twice over: once as BCD        */
/* arithmetic into $78B0/$78B1 so the number can be printed, and once as two   */
/* consecutive calls to game_add_score, which adds $71CB/$71CC each time.      */
/* Neither reads the other's answer.                                          */
/*                                                                            */
/* This routine is *entered with decimal mode set* -- unusual here, and the    */
/* reason the adapter cannot assert against it the way the others do. The      */
/* generated C makes the same claim: its binary-mode path is dead, folded away */
/* because D is known set on entry. $78C7 clears it before anything else runs. */
/*                                                                            */
/* Then a box: a frame in ink 9 and its interior wiped in ink 0, four rows at  */
/* a time. The text goes through the game's own hi-res font, which is what     */
/* $6641 installs and what the two pokes at $795F take back out again.         */
/* ========================================================================== */

void game_bonus_screen(void) {
  // $78B3 -- double the apple's value into $78B0/$78B1, in BCD.
  GAME_CYCLES(0x78b3, 36);
  const uint16_t lo = adc_dec16(ram_peek(kAppleValue), ram_peek(kAppleValue), 0x00);
  ram_poke(kBonusAmount, (uint8_t)lo);
  const uint16_t hi = adc_dec16(ram_peek(kAppleValue + 1), ram_peek(kAppleValue + 1), (uint8_t)(lo >> 8) & 0x01);
  ram_poke(kBonusAmount + 1, (uint8_t)hi);
  s_a = (uint8_t)hi;
  s_status_c = (uint8_t)(hi >> 8) & 0x01;
  s_status_v = ((uint8_t)(hi >> 8) & 0x40) != 0;
  s_status_d = 0x00; // $78C7 CLD

  // Twice, because the bonus is twice the apple value and game_add_score adds
  // it once.
  game_add_score_native();
  GAME_CYCLES(0x78cb, 6);
  game_add_score_native();
  GAME_CYCLES(0x78ce, 6);
  game_status_panel();

  // $78D1 -- the frame, in ink 9: top and bottom edges, then both sides.
  GAME_CYCLES(0x78d1, 31);
  s_shape = 0x01;
  s_ink = 0x09;
  // Columns $0D-$1A, rows $10-$15. The two sides used to inherit their column
  // from the edge above: an hline leaves s_col at its own endpoint, so the
  // first vline ran down $1A, the right edge, and not the $0D it looks like.
  plot_hline_at(0x0d, 0x10, 0x1a, 0x78e7);
  GAME_CYCLES(0x78e8, 16);
  plot_hline_at(0x0d, 0x15, 0x1a, 0x78f2);
  GAME_CYCLES(0x78f3, 16);
  plot_vline_at(0x1a, 0x10, 0x15, 0x78fd);
  GAME_CYCLES(0x78fe, 16);
  plot_vline_at(0x0d, 0x10, 0x15, 0x7908);

  // $7909 -- the interior, in ink 0, one row at a time from $11 to $14. The
  // original re-loads $02 each time and increments $03 in place, which is why
  // the rows are not written out as constants.
  GAME_CYCLES(0x7909, 26);
  s_ink = 0x00;
  plot_hline_at(0x0e, 0x11, 0x19, 0x791b);
  GAME_CYCLES(0x791c, 16);
  plot_hline_at(0x0e, 0x12, 0x19, 0x7924);
  GAME_CYCLES(0x7925, 16);
  plot_hline_at(0x0e, 0x13, 0x19, 0x792d);
  GAME_CYCLES(0x792e, 16);
  plot_hline_at(0x0e, 0x14, 0x19, 0x7936);

  // $7937 -- "BONUS: " and the amount, through the hi-res font.
  GAME_CYCLES(0x7937, 16);
  s_ch = 0x0f;
  s_cv = 0x09;
  game_install_cout_hook(0x7941);
  GAME_CYCLES(0x7942, 6);
  game_print_inline_str(0x7944);
  GAME_CYCLES(0x794d, 15);
  s_h2 = 0x00;
  s_a = ram_peek(kBonusAmount + 1);
  game_print_bcd(0x7956);
  GAME_CYCLES(0x7957, 10);
  s_a = ram_peek(kBonusAmount);
  game_print_bcd(0x795c);

  // $795D -- COUT back to the ROM's, and $02 becomes the outermost counter of
  // the pause below.
  GAME_CYCLES(0x795d, 15);
  s_cswl = 0xf0;
  s_cswh = 0xfd;
  uint8_t passes = 0x20; // $02 was the outermost counter

  // $7969 -- hold the screen. Everything from $794D on keeps its probe: the
  // inline-string printer returns to an address it computes, so this whole tail
  // survives in the generated C as dynamic blocks nothing reaches, and stays on
  // the site list. The edges are exempt -- edges are never probed.
  // Three nested counters, the innermost taking its
  // length from the middle one, so the delay shortens as it goes; the keyboard
  // is read and discarded each time round to keep the strobe clear. All three
  // are DEX/DEY loops, which test after decrementing -- a count of zero would
  // mean 256, and none of these start at zero.
  do {
    GAME_CYCLES(0x7969, 2);
    uint8_t x = 0x80;
    do {
      GAME_CYCLES(0x796b, 4);
      uint8_t y = x;
      do {
        GAME_CYCLES(0x796d, 4);
        --y;
        if (y != 0)
          GAME_CYCLES(0x796e, 1);
      } while (y != 0);
      GAME_CYCLES(0x7970, 12);
      click_speaker();
      --x;
      if (x != 0)
        GAME_CYCLES(0x7977, 1);
    } while (x != 0);
    GAME_CYCLES(0x7979, 7);
    passes = (uint8_t)(passes - 1);
    if (passes != 0)
      GAME_CYCLES(0x797b, 1);
  } while (passes != 0);
  GAME_CYCLES(0x797d, 6);
}

/* ========================================================================== */
/* $6256 -- start a life                                                      */
/*                                                                            */
/* Head and tail both at row $27 column $14 -- the snake begins as one cell,   */
/* in the gate at the bottom centre, facing up. $6254 owes it ten segments of  */
/* growth, which is what makes it appear to emerge from the gate rather than   */
/* start full length.                                                          */
/*                                                                            */
/* $6258 supplies the tail column rather than it being written here: the       */
/* original loads $14 into A, and game_start_life hands back $14 from its own  */
/* constant at $6630. The two are unrelated and happen to agree.               */
/*                                                                            */
/* Ends by falling into the main loop, which the original does with a JSR and  */
/* an RTS it never reaches -- the generated call passes a return address of 0  */
/* for the same reason.                                                        */
/* ========================================================================== */

void game_begin_life(void) {
  GAME_CYCLES(0x6256, 8);
  s_a = 0x14;
  game_start_life_adapter(0x625a);

  GAME_CYCLES(0x625b, 36);
  ram_poke(kTailCol, s_a); // from $6630, by way of $660F
  ram_poke(kHeadRow, 0x27); // the bottom edge
  ram_poke(kTailRow, 0x27); // the same cell
  ram_poke(kDirection, DIR_UP);
  ram_poke(kGrowth, 0x0a); // ten
  ram_poke(kLifeTimer, 0x64);

  // $6279 -- empty the sixteen-entry key ring at $623C. DEX/BPL, so it runs
  // down through 0 and stops when X wraps negative, one more pass than a
  // count of $0F suggests.
  uint8_t x = 0x0f;
  do {
    GAME_CYCLES(0x6279, 9);
    ram_poke(kKeyRing + x, 0x00);
    --x;
    if (!(x & 0x80))
      GAME_CYCLES(0x627d, 1);
  } while (!(x & 0x80));

  GAME_CYCLES(0x627f, 11);
  ram_poke(kRingRead, 0x00);
  ram_poke(kRingWrite, 0x00);
  game_play_loop(0x0000);
}

/* ========================================================================== */
/* $7980 -- the setup screen                                                  */
/*                                                                            */
/* Three jobs, in the order the original does them: clamp the pointer          */
/* game_rand_byte reads its numbers through, offer the difficulty prompt, and  */
/* -- if the player presses C rather than a digit -- run the key redefinition  */
/* screen instead.                                                             */
/*                                                                            */
/* $0301 is the difficulty, 0 to 2. $0302 is the flag that decides whether the */
/* game plays itself: set here when nobody answers the prompt in time, and     */
/* cleared when they do. That is the only place it is ever set, which is why   */
/* the auto-steer at $6A32 runs in some recordings and not others.             */
/*                                                                            */
/* $73D7 makes the first pass different. It starts zero, so the very first     */
/* call sets the demo going and returns without asking anything; every call    */
/* after that reaches the prompt.                                              */
/*                                                                            */
/* Nine blocks here are decoded from the binary rather than checked by a       */
/* recording: $73DD (that first pass), $742D through $744C (the joystick, only */
/* read when $6C71 selects it), $7455 (the prompt timing out) and $798A (one   */
/* arm of the clamp). Converting them takes their addresses off the site list, */
/* so this comment is the only record that they are unverified.                */
/* ========================================================================== */

void game_setup_screen(void) {
  // $7980 -- keep $0E/$0F inside the window game_rand_byte expects.
  GAME_CYCLES(0x7980, 7);
  const uint8_t hi = ram_peek(kRandPtr + 1);
  bool clamp_lo = hi >= 0x1f;
  if (!clamp_lo) {
    GAME_CYCLES(0x7986, 4);
    clamp_lo = hi < 0x18;
    if (!clamp_lo)
      GAME_CYCLES(0x7988, 1);
  } else {
    GAME_CYCLES(0x7984, 1);
  }
  if (clamp_lo) {
    GAME_CYCLES(0x798a, 8);
    ram_poke(kRandPtr, (uint8_t)(ram_peek(kRandPtr) & 0xde));
  }
  GAME_CYCLES(0x7990, 13);
  ram_poke(kRandPtr + 1, (uint8_t)((ram_peek(kRandPtr + 1) & 0x1f) | 0x18));

  // $73D8 -- the first call through here never asks anything.
  GAME_CYCLES(0x73d8, 6);
  if (!ram_peek(kSetupSeen)) {
    GAME_CYCLES(0x73dd, 20);
    ram_poke(kDemoMode, 0x01);
    ram_poke(kDifficulty, 0x01);
    ram_poke(kSetupSeen, 0x01);
    return;
  }

  // $73E9 -- the prompt, and the two counters that time it out. $02 is the
  // outer one and $03 the inner; both count *up* to zero.
  GAME_CYCLES(0x73db, 1);
  GAME_CYCLES(0x73e9, 16);
  s_cv = 0x17;
  s_ch = 0x00;
  game_print_inline_str(0x73f3);
  GAME_CYCLES(0x7414, 10);
  // $02 and $03 time the prompt out: the inner one wraps 256 times per tick of
  // the outer, and when the outer wraps nobody has answered.
  uint8_t outer_count = 0xe8;
  uint8_t inner_count = 0x00;

  uint8_t key;
wait: /* $741C */
  for (;;) {
    // $741C -- spin Y round once, then look at the keyboard. This site keeps
    // its probe: it is one of the addresses the replay coordinate counts.
    do {
      GAME_CYCLES(0x741c, 4);
      s_y = (uint8_t)(s_y + 1);
      if (s_y != 0)
        GAME_CYCLES(0x741d, 1);
    } while (s_y != 0);

    GAME_CYCLES_COORD(0x741f, 6);
    key = io_peek(0xc000);
    if (key & 0x80) {
      GAME_CYCLES(0x7422, 1);
      break;
    }

    GAME_CYCLES(0x7424, 7);
    const uint8_t inner = (uint8_t)(inner_count + 1);
    inner_count = inner;
    if (inner != 0) {
      GAME_CYCLES(0x7426, 1);
      continue;
    }

    // $7428 -- once the inner counter wraps, try the joystick, if one is
    // selected. Each button stands in for a digit.
    GAME_CYCLES(0x7428, 6);
    if (ram_peek(kJoystick)) {
      GAME_CYCLES(0x742d, 10);
      io_peek(0xc05b);
      if (!(io_peek(0xc062) & 0x80)) {
        GAME_CYCLES(0x7435, 5);
        key = 0xb1;
        break;
      }
      GAME_CYCLES(0x7433, 1);
      GAME_CYCLES(0x743a, 10);
      io_peek(0xc05a);
      if (!(io_peek(0xc062) & 0x80)) {
        GAME_CYCLES(0x7442, 5);
        key = 0xb0;
        break;
      }
      GAME_CYCLES(0x7440, 1);
      GAME_CYCLES(0x7447, 6);
      if (!(io_peek(0xc063) & 0x80)) {
        GAME_CYCLES(0x744c, 5);
        key = 0xb2;
        break;
      }
      GAME_CYCLES(0x744a, 1);
    } else {
      GAME_CYCLES(0x742b, 1);
    }

    // $7451 -- the outer counter. When it wraps too, nobody is answering.
    GAME_CYCLES(0x7451, 7);
    const uint8_t outer = (uint8_t)(outer_count + 1);
    outer_count = outer;
    if (outer == 0) {
      GAME_CYCLES(0x7455, 20);
      ram_poke(kDemoMode, 0x01);
      ram_poke(kDifficulty, 0x01);
      io_poke(0xc010, 0x01);
      return;
    }
    GAME_CYCLES(0x7453, 1);
  }

  // $7461 -- something was pressed. Clear the strobe with it still in A, the
  // way the original does.
  GAME_CYCLES(0x7461, 8);
  io_poke(0xc010, key);
  if (key != 0xc3) {
    GAME_CYCLES(0x7468, 4);
    if (key < 0xb0) {
      GAME_CYCLES(0x746a, 1);
      goto wait;
    }
    GAME_CYCLES(0x746c, 4);
    if (key >= 0xb3) {
      GAME_CYCLES(0x746e, 1);
      goto wait;
    }
    // $7470 -- a digit. The subtract is a plain SBC with carry set.
    GAME_CYCLES(0x7470, 24);
    ram_poke(kDifficulty, (uint8_t)(key - 0xb0));
    ram_poke(kDemoMode, 0x00);
    io_poke(0xc010, 0x00);
    return;
  }

  // $747F -- C, so redefine the keys instead. Show the six current bindings,
  // draw the highlight, then walk them again asking for replacements.
  GAME_CYCLES(0x7466, 1);
  GAME_CYCLES(0x747f, 6);
  game_clear_hgr(0x7481);
  GAME_CYCLES(0x7482, 10);
  io_peek(0xc052);
  game_install_cout_hook(0x7487);
  GAME_CYCLES(0x7488, 11);
  s_cv = 0x01;
  game_print_inline_str(0x748e);

  GAME_CYCLES(0x7541, 2);
  for (uint8_t i = 0; i != 6; ++i) {
    GAME_CYCLES(0x7543, 10);
    s_a = ram_peek(kKeyTable + i);
    s_x = i;
    game_show_key(0x7548);
    GAME_CYCLES(0x7549, 6);
    if (i != 5)
      GAME_CYCLES(0x754c, 1);
  }

  GAME_CYCLES(0x754e, 26);
  plot_shape_at(0x02, 0x0c, (Cell){.col = 0x1e, .row = 0x12});
  // The stem below it, down the same column -- which the original inherited
  // from the plot above rather than restating.
  GAME_CYCLES(0x7561, 21);
  s_shape = 0x0a;
  plot_vline_at(0x1e, 0x13, 0x1d, 0x756f);
  // At the stem's far end -- the vline above left s_row on $1D.
  GAME_CYCLES(0x7570, 11);
  s_a = 0x0e;
  plot_shape_at(0x0e, 0x0c, (Cell){.col = 0x1e, .row = 0x1d});

  GAME_CYCLES(0x7577, 2);
  for (uint8_t i = 0; i != 6; ++i) {
    GAME_CYCLES(0x7579, 6);
    s_x = i;
    game_edit_key(0x757b);
    GAME_CYCLES(0x757c, 11);
    ram_poke(kKeyTable + i, s_a);
    s_x = i;
    game_show_key(0x7581);
    GAME_CYCLES(0x7582, 6);
    if (i != 5)
      GAME_CYCLES(0x7585, 1);
  }

  // $7587 -- COUT back to the ROM's.
  GAME_CYCLES(0x7587, 16);
  s_cswl = 0xf0;
  s_cswh = 0xfd;
}

/* ========================================================================== *
 * Adapters: 6502 machine state in, real C out                              *
 * ========================================================================== */

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
  ram_poke(kStrPtr, (uint8_t)ret_addr);
  ram_poke(kStrPtr + 1, (uint8_t)(ret_addr >> 8));
  rom_fc68(0x7239); // VTAB to the current CV

  for (;;) {
    // The pointer is stepped before the read, which is why the caller passes
    // the address of the JSR's last byte rather than of the string.
    /*$7239*/ CYCLES(0x7239, 7);
    const uint8_t lo = (uint8_t)(ram_peek(kStrPtr) + 1);
    ram_poke(kStrPtr, lo);
    if (!lo) {
      /*$723D*/ CYCLES(0x723d, 5);
      ram_poke(kStrPtr + 1, (uint8_t)(ram_peek(kStrPtr + 1) + 1));
    } else {
      /*$723B*/ CYCLES_EDGE(0x723b, 1);
    }

    /*$723F*/ CYCLES(0x723f, 9);
    const uint8_t ch = peek(ram_peek16al(kStrPtr));
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
/* Arguments, which the original passed in zero page and this file passes in   */
/* variables of the same name:                                                */
/*   s_shape  picks four AND masks from the table at $6174 ($00)              */
/*   s_ink    0 erases, 1 draws ($01)                                         */
/*   s_col    the byte offset within the cell row ($02)                       */
/*   s_row    cell row, 0-47 ($03)                                            */
/* Scratch, which was $04-$07 and is now four locals:                          */
/*   the destination pointer, advanced one scanline per iteration             */
/*   the index into the dot-pattern table at $6064                            */
/*   the scanline counter, 0-3                                                */
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
  const uint8_t shape = s_shape;
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

void game_draw_cell(uint16_t ret_addr, uint8_t ink, Cell c) {
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

  const uint8_t last_hi = game_draw_cell_native(ink, c);

  // What the loop leaves: X counted to 4, Y is the column, A the last
  // destination high byte, and the flags come from CPX #4.
  s_x = 0x04;
  s_y = c.col;
  s_a = last_hi;
  s_status_c = 0x01;
  s_status_not_z = 0x00;
  s_status_n = 0x00;
  s_status_v = ovf8(s_a, (uint8_t)(s_a - 0x04), 0x04);

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

void game_plot_hline(uint16_t ret_addr, Cell c, uint8_t to_col) {
  // Adapter for game_plot_hline_native(). The shape load stays here: it is the
  // routine's own first block and keeps its probe site.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$6148*/ CYCLES(0x6148, 6);
  game_load_shape(0x614a);
  const uint8_t at = game_plot_hline_native(s_ink, c, to_col);

  // The CMP that ended the loop, and the coordinate it compared.
  s_a = at;
  s_status_c = 0x01;
  s_status_not_z = 0x00;
  s_status_n = 0x00;

  if (ret_addr)
    pop16();
}

void game_plot_vline(uint16_t ret_addr, Cell c, uint8_t to_row) {
  // Adapter for game_plot_vline_native(). The shape load stays here: it is the
  // routine's own first block and keeps its probe site.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$615A*/ CYCLES(0x615a, 6);
  game_load_shape(0x615c);
  const uint8_t at = game_plot_vline_native(s_ink, c, to_row);

  // The CMP that ended the loop, and the coordinate it compared.
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

void game_lores_vline(uint16_t ret_addr, Cell c, uint8_t to_row) {
  // Adapter for game_lores_vline_native(). Costs 4 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$7000*/ CYCLES(0x7000, 6);
  const uint8_t restored = game_lores_vline_native(c, to_row);
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

void game_plot_shape_merge(uint16_t ret_addr, uint8_t ink, Cell c) {
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

  s_y = c.row; // LDY $03 at $6B98, before the loop overwrites it
  const uint8_t last_hi = game_merge_cell_native(ink, c);

  s_x = 0x04;
  s_y = c.col;
  s_a = last_hi;
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




/* ========================================================================== */
/* $6217, $7590, $6B3D                                                        */
/* ========================================================================== */


void game_show_key(uint16_t ret_addr) {
  // Adapter for game_show_key_native(). Costs 7 trace sites.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  game_show_key_native(s_x, s_a);

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
  const MoveVerdict v = snake_move_verdict(ram_peek(kSteerDir), &cell);

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
      .col = ram_peek(kBouncer),
      .row = ram_peek(kBouncer + 1),
      .dx = (int8_t)ram_peek(kBouncer + 2),
      .dy = (int8_t)ram_peek(kBouncer + 3),
  };

  // The state the original leaves behind: A holds the row it loaded first, and
  // the flags come from that load.
  s_a = b.row;
  s_status_not_z = b.row;
  s_status_n = (b.row & 0x80);

  bouncer_step(&b);

  ram_poke(kBouncer, b.col);
  ram_poke(kBouncer + 1, b.row);
  ram_poke(kBouncer + 2, (uint8_t)b.dx);
  ram_poke(kBouncer + 3, (uint8_t)b.dy);

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $728D, $6BFB                                                               */
/* ========================================================================== */

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
  const uint8_t top = ram_peek(kScore + 3);
  s_a = top;
  s_status_not_z = top;
  s_status_n = (top & 0x80);

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

  const uint8_t row = ram_peek(kSearchRow);
  s_a = row;
  s_status_not_z = row;
  s_status_n = (row & 0x80);

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $69A9, $75D1, $6C72 -- the rest of the input path.                         */
/*                                                                            */
/* All three are adapters now; the decodes live in game_native.c. They carry  */
/* most of what no recording exercises -- ESC and Ctrl-S, the arrow keys on   */
/* the redefinition screen, and the whole joystick branch -- so the comments  */
/* above the native routines, rather than probe-acceptance.sh's baseline      */
/* list, are what now record which blocks rest on the binary alone.           */
/* ========================================================================== */

void game_pause_or_toggle_sound(uint16_t ret_addr) {
  // Adapter for game_pause_or_toggle_sound_native(). Costs 5 trace sites;
  // $69A9 itself survives, charged zero here and for real inside -- see
  // game_find_apple below for why that works.
  bool branchTarget = true;

  if (ret_addr)
    push16(ret_addr); // Fake return address.

  /*$69A9*/ CYCLES(0x69a9, 0);
  const uint8_t k = game_pause_or_toggle_sound_native(s_a);

  // A is not written back. On the Ctrl-S path the original returns with the
  // toggled flag in A rather than the key, and nothing reads it either way:
  // `apple2tc --ir` gives func_69a9 `LiveOut: STATUS_C` and nothing else.
  // The flags are written anyway -- C because it is the one the caller does
  // read, N and Z because they follow from k and cost nothing.
  s_status_c = (k >= 0x93);
  if (k == 0x93) {
    // $69B9 ended on the EOR, so N and Z describe the new flag, not the key.
    const uint8_t f = ram_peek(kSoundMuted);
    s_status_not_z = f;
    s_status_n = (f & 0x80);
  } else {
    s_status_not_z = (k != 0x93);
    s_status_n = ((uint8_t)(k - 0x93) & 0x80);
  }

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
    ram_poke(kLifeOutcome, 0x00);
    break;
  case LIFE_APPLE:
    ram_poke(kLifeOutcome, 0x0f);
    break;
  case LIFE_QUIT:
    ram_poke(kLifeOutcome, 0xff);
    break;
  case LIFE_TIMEOUT:
    ram_poke(kLifeOutcome, 0xfe);
    break;
  case LIFE_CRASH:
    ram_poke(kLifeOutcome, cell);
    break;
  }

  // A is dead at both call sites -- $7716 and $7739 both load $6253 straight
  // away -- but the original leaves the reason there on most paths, so this
  // does too rather than leaving something arbitrary.
  const uint8_t reason = ram_peek(kLifeOutcome);
  s_a = reason;
  s_status_not_z = reason;
  s_status_n = (uint8_t)(reason & 0x80);

  if (ret_addr)
    pop16();
}

/* ========================================================================== */
/* $72CE -- the status panel. See game_native.c.                              */
/* ========================================================================== */


/* ========================================================================== */
/* $78B3 -- the bonus screen. See game_native.c.                              */
/* ========================================================================== */


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

/* ========================================================================== *
 * The top level                                                            *
 * ========================================================================== */

/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// Snake Byte's top level, as C. Included only by snake-byte-cold.c.
///
/// Why this is not in game_native.c, where the rest of the converted game
/// lives: game_native.c is shared with snake-bytec1-ext, and that build still
/// has the generated func_t001 emitting every address in here. Two sources
/// claiming the same block head is exactly what probe-acceptance.sh's lint
/// rejects, and rightly -- in the ext build these addresses must keep their
/// probes, and they do, in the generated dispatch. Only the cold build has
/// replaced that dispatch, so only the cold build compiles this.
///
/// It goes away when snake-bytec1-ext does.


/* ========================================================================== */
/* $3750 and $7691 -- the top level                                           */
/*                                                                            */
/* The last of the decompiler's output, and the outermost loop in the game:   */
/* new game -> new level -> new round -> one life -> what happened -> repeat.  */
/* Everything it calls was converted long before it was.                      */
/*                                                                            */
/* The labels keep their addresses because this is a state machine, not a     */
/* nest of loops: a life ends by clearing the round, by dying, or by the      */
/* player quitting, and each re-enters at a different depth. As while-loops   */
/* it would need flags to say which level to break out to.                    */
/*                                                                            */
/* Decimal mode is explicit. The generated code emitted both a binary and a   */
/* decimal arm for every add and subtract because it could not prove which    */
/* way the D flag went; the assembly settles it, since the original brackets  */
/* each BCD run with SED/CLD. $7743-$7777 and $7817-$783A are decimal and     */
/* everything else is binary, so each operation is written the one way it     */
/* runs. s_status_d is still maintained: game_bonus is entered with decimal   */
/* mode set, which is why its adapter omits the assertion the others carry.   */
/* ========================================================================== */

/// BCD add across a low/high pair in emulated RAM, as SED/CLC/ADC/ADC leaves it.
static void bcd_add16_at(uint16_t lo, uint16_t hi, uint8_t by) {
  uint16_t r = adc_dec16(ram_peek(lo), by, 0);
  ram_poke(lo, (uint8_t)r);
  r = adc_dec16(ram_peek(hi), 0x00, (uint8_t)(r >> 8) & 0x01);
  ram_poke(hi, (uint8_t)r);
}

/// BCD subtract across a low/high pair, as SED/SEC/SBC/SBC leaves it.
static void bcd_sub16_at(uint16_t lo, uint16_t hi, uint8_t by) {
  uint16_t r = sbc_dec16(ram_peek(lo), by, 1);
  ram_poke(lo, (uint8_t)r);
  r = sbc_dec16(ram_peek(hi), 0x00, (uint8_t)(r >> 8) & 0x01);
  ram_poke(hi, (uint8_t)r);
}

void game_cold_start(void) {
  /* $3750 -- copy eight pages of level data from $3800 down to $1800. The
     original walks them by incrementing the operands of its own LDA and STA
     ($3754 and $3757), which is why it re-enters $3750 eight times rather
     than looping inside. The patched operand bytes are written back because
     ram.probe hashes that range. */
  for (unsigned page = 0; page != 8; ++page) {
    // Probed, not because anything needs to observe it, but because it is what
    // probe-acceptance.sh aligns the two builds on -- see GAME_CYCLES_ANCHOR.
    GAME_CYCLES_ANCHOR(0x3750, 2);
    for (unsigned i = 0; i != 256; ++i) {
      GAME_CYCLES(0x3752, 13);
      poke((uint16_t)(0x1800 + page * 256 + i), peek((uint16_t)(0x3800 + page * 256 + i)));
      if (i != 255)
        GAME_CYCLES(0x3759, 1);
    }
    GAME_CYCLES(0x375b, 20);
    ram_poke(kRelocLoadOp, (uint8_t)(ram_peek(kRelocLoadOp) + 1));
    ram_poke(kRelocStoreOp, (uint8_t)(ram_peek(kRelocStoreOp) + 1));
    if (page != 7)
      GAME_CYCLES(0x3766, 1);
  }

  GAME_CYCLES(0x3768, 6);
  rom_setvid(0x376a);
  GAME_CYCLES(0x376b, 6);
  rom_setkbd(0x376d);
  GAME_CYCLES(0x376e, 29);
  ram_poke(kStepDelay, 0x52);
  ram_poke(kDifficulty, 0x01);
  ram_poke(kDemoMode, 0x01); // so the first pass plays itself
  ram_poke(kScriptIndex, 0x01);
  ram_poke(kLevelTime, 0x64);
  goto round;             // $3783: JMP $76C2

new_game: /* $7691 */
  GAME_CYCLES(0x7691, 6);
  game_setup(0x7693);
  GAME_CYCLES(0x7694, 6);
  game_update_high_score(0x7696);
  GAME_CYCLES(0x7697, 40);
  ram_poke(kScriptIndex, 0x01);
  set_level(0x01);
  ram_poke(kScore, 0x00);
  ram_poke(kScore + 1, 0x00);
  ram_poke(kScore + 2, 0x00);
  ram_poke(kScore + 3, 0x00);
  set_lives(0x02);
  ram_poke(kApplesAfield, 0x00);

new_level: /* $76B7 */
  GAME_CYCLES(0x76b7, 14);
  ram_poke(kLivesAtLevelStart, lives());
  ram_poke(kApplesQuota, 0x10);

round: /* $76C2 */
  GAME_CYCLES(0x76c2, 6);
  ram_poke(kApplesQuota + 1, 0x00);

start_round: /* $76C7 */
  GAME_CYCLES(0x76c7, 40);
  ram_poke(kApplesAfield, 0x00);
  ram_poke(kApplesAfield + 1, 0x00);
  ram_poke(kApplesEaten, 0x00);
  ram_poke(kApplesEaten + 1, 0x00);
  ram_poke(kApplesLeft, ram_peek(kApplesQuota));
  ram_poke(kApplesLeft + 1, ram_peek(kApplesQuota + 1));
  game_draw_playfield(0x76e3);
  GAME_CYCLES(0x76e4, 14);
  set_life_time(ram_peek(kLevelTime));
  game_set_apple_value_native();
  GAME_CYCLES(0x76ed, 14);
  io_peek(0xc054);       // page 1
  s_a = io_peek(0xc053); // mixed text/graphics
  const Cell apple = game_place_apple_native();
  GAME_CYCLES(0x76f6, 6);
  // $76F6 redraws it, at the cell game_place_apple just chose.
  game_plot_shape_native(0x09, apple);
  GAME_CYCLES(0x76f9, 8);
  ram_poke(kStepDelay, 0x52);
  s_a = 0x00;
  GAME_CYCLES(0x7700, 23);
  ram_poke(kHeadMoved, 0x00);
  ram_poke(kLifeTimer, life_time());
  s_wndtop = 0x14; // window top, so HOME clears only the status panel
  rom_home(0x770f);
  GAME_CYCLES(0x7710, 6);
  game_status_panel();
  GAME_CYCLES(0x7713, 6);
  game_start_round(0x7715);
  GAME_CYCLES(0x7716, 3);
  goto verdict; // $7716: JMP $7739 -- a fresh round asks the same question

life: /* $7719 */
  GAME_CYCLES(0x7719, 19);
  ram_poke(kLifeTimer, life_time());
  s_wndtop = 0x14;
  rom_home(0x7725);
  GAME_CYCLES(0x7726, 6);
  game_status_panel();
  GAME_CYCLES(0x7729, 8);
  if (ram_peek(kStepDelay) >= 0x03) {
    // $7730 -- two steps faster each life, but never past 3.
    GAME_CYCLES(0x7730, 8);
    ram_poke(kStepDelay, (uint8_t)(ram_peek(kStepDelay) - 2));
  } else {
    GAME_CYCLES(0x772e, 1);
  }
  GAME_CYCLES(0x7736, 6);
  game_play_loop(0x7738);

verdict: /* $7739 -- $6253 says how the life ended */
  GAME_CYCLES(0x7739, 8);
  if (ram_peek(kLifeOutcome) == 0x0f)
    goto ate_apple;
  GAME_CYCLES(0x7740, 3);
  GAME_CYCLES(0x77e6, 4);
  if (ram_peek(kLifeOutcome) != 0x00)
    goto not_apple;
  goto round_cleared;

ate_apple: /* $773E */
  GAME_CYCLES(0x773e, 1);
  GAME_CYCLES(0x7743, 76);
  s_status_d = 0x01;
  bcd_sub16_at(kApplesAfield, kApplesAfield + 1, 0x01);
  bcd_sub16_at(kApplesLeft, kApplesLeft + 1, 0x01);
  bcd_add16_at(kApplesEaten, kApplesEaten + 1, 0x01);
  s_status_d = 0x00;

  // $777B -- points only for the first $11 apples of the round. The high
  // byte must be zero and the low one below $11, both BCD.
  if (ram_peek(kApplesEaten + 1)) {
    GAME_CYCLES(0x777b, 1);
  } else {
    GAME_CYCLES(0x777d, 8);
    if (ram_peek(kApplesEaten) >= 0x11) {
      GAME_CYCLES(0x7782, 1);
    } else {
      GAME_CYCLES(0x7784, 6);
      game_add_score_native();
    }
  }

  GAME_CYCLES(0x7787, 18);
  ram_poke(kGrowth, (uint8_t)(ram_peek(kGrowth) + 0x0a)); // ten more cells of snake

  // $7793 -- anything left in the round?
  if (ram_peek(kApplesLeft)) {
    GAME_CYCLES(0x7793, 1);
    goto next_apple;
  }
  GAME_CYCLES(0x7795, 6);
  if (ram_peek(kApplesLeft + 1)) {
    GAME_CYCLES(0x7798, 1);
    goto next_apple;
  }

  /* $779A -- that was the last one. Draw the bar across the bottom, put the
     marker on it, and stop the clock for the run to the gate -- see
     kLifeTime for why $FF stops it rather than lengthening it. */
  GAME_CYCLES(0x779a, 31);
  s_ink = 0x06;
  s_shape = 0x15;
  plot_hline_at(0x12, 0x00, 0x16, 0x77b0);
  GAME_CYCLES(0x77b1, 16);
  plot_shape_at(0x15, 0x00, (Cell){.col = 0x14, .row = 0x00});
  GAME_CYCLES(0x77bc, 14);
  set_life_time(0xff);
  s_a = 0x00;
  rom_setcol(0x77c5);
  GAME_CYCLES(0x77c6, 10);
  s_a = 0x00;
  s_y = 0x14;
  rom_plot(0x77cc);
  GAME_CYCLES(0x77cd, 3);
  goto life;

next_apple: /* $77D0 -- place one only when both countdown bytes are zero */
  GAME_CYCLES(0x77d0, 6);
  if (ram_peek(kApplesAfield)) {
    GAME_CYCLES(0x77d5, 3);
    goto life;
  }
  GAME_CYCLES(0x77d3, 1);
  GAME_CYCLES(0x77d8, 6);
  if (ram_peek(kApplesAfield + 1)) {
    GAME_CYCLES(0x77dd, 3);
    goto life;
  }
  GAME_CYCLES(0x77db, 1);
  GAME_CYCLES(0x77e0, 6);
  game_place_apple_native();
  GAME_CYCLES(0x77e3, 3);
  goto life;

round_cleared: /* $77EA */
  GAME_CYCLES(0x77ea, 32);
  {
    const uint16_t r = adc_dec16(level(), 0x01, 0x00);
    set_level((uint8_t)r);
  }
  s_status_d = 0x00;
  ram_poke(kScriptIndex, (uint8_t)(ram_peek(kScriptIndex) + 1));
  // $77F8 -- no life was lost this round, so it earns a bonus.
  if (lives() == ram_peek(kLivesAtLevelStart)) {
    GAME_CYCLES(0x7800, 6);
    game_bonus_screen();
  } else {
    GAME_CYCLES(0x77fe, 1);
  }
  GAME_CYCLES(0x7803, 6);
  game_award_extra_life_native();
  GAME_CYCLES(0x7806, 3);
  goto new_level;

not_apple: /* $77E8 */
  GAME_CYCLES(0x77e8, 1);
  GAME_CYCLES(0x7809, 4);
  if (ram_peek(kLifeOutcome) != 0xfe) {
    GAME_CYCLES(0x780b, 1);
    goto ended;
  }
  GAME_CYCLES(0x780d, 6);
  if (ram_peek(kApplesLeft + 1)) {
    GAME_CYCLES(0x7810, 1);
    goto harder;
  }
  GAME_CYCLES(0x7812, 6);
  if (!ram_peek(kApplesLeft)) {
    GAME_CYCLES(0x7815, 1);
    goto ended;
  }

harder: /* $7817 -- three more apples in the round, and three more to come */
  GAME_CYCLES(0x7817, 54);
  s_status_d = 0x01;
  bcd_add16_at(kApplesQuota, kApplesQuota + 1, 0x03);
  bcd_add16_at(kApplesLeft, kApplesLeft + 1, 0x03);
  s_status_d = 0x00;
  game_place_apple_native();
  GAME_CYCLES(0x783e, 6);
  game_place_apple_native();
  GAME_CYCLES(0x7841, 6);
  game_place_apple_native();
  GAME_CYCLES(0x7844, 3);
  goto life;

ended: /* $7847 */
  GAME_CYCLES(0x7847, 8);
  if (ram_peek(kLifeOutcome) == 0xff) {
    GAME_CYCLES(0x784e, 3);
    goto new_game; // the player pressed the quit key
  }
  GAME_CYCLES(0x784c, 1);
  GAME_CYCLES(0x7851, 4);
  // $FE means the snake ran out of room rather than died, and that just starts
  // another life. Anything else falls through to the pause. Note the sense:
  // the original *branches away* when it is not $FE, so equality is the
  // fall-through, not the exception.
  if (ram_peek(kLifeOutcome) == 0xfe) {
    GAME_CYCLES(0x7855, 3);
    goto life;
  }
  GAME_CYCLES(0x7853, 1);
  GAME_CYCLES(0x7858, 6);
  if (ram_peek(kDemoMode)) {
    GAME_CYCLES(0x785b, 1);
    goto lose_life; // the demo does not wait to be told to carry on
  }

  /* $785D -- "PRESS SPACE BAR TO CONTINUE", then wait for space or the
     paddle button, whichever the setup screen selected. */
  GAME_CYCLES(0x785d, 16);
  s_cv = 0x17;
  s_ch = 0x00;
  game_print_inline_str(0x7867);
  for (;;) {
    GAME_CYCLES(0x7886, 6);
    if (ram_peek(kJoystick)) {
      GAME_CYCLES(0x788b, 6);
      // The button reads with bit 7 *clear* when pressed on this path.
      if (!(io_peek(0xc061) & 0x80)) {
        GAME_CYCLES(0x788e, 1);
        break;
      }
    } else {
      GAME_CYCLES(0x7889, 1);
    }
    // $7890 is on the replay coordinate -- see GAME_CYCLES_COORD.
    GAME_CYCLES_COORD(0x7890, 8);
    const uint8_t key = io_peek(0xc000);
    if (key == 0xa0) {
      GAME_CYCLES(0x7897, 4);
      io_poke(0xc010, key);
      break;
    }
    GAME_CYCLES(0x7895, 1);
  }

lose_life: /* $789A */
  GAME_CYCLES(0x789a, 6);
  if (!lives()) {
    GAME_CYCLES(0x789f, 3);
    goto new_game;
  }
  GAME_CYCLES(0x789d, 1);
  GAME_CYCLES(0x78a2, 19);
  {
    const uint16_t r = sbc_dec16(lives(), 0x01, 0x01);
    set_lives((uint8_t)r);
  }
  s_status_d = 0x00;
  goto start_round;
}

/* ========================================================================== *
 * The machine state at $3750                                               *
 * ========================================================================== */

/* Generated by make-entry-state.sh. Do not edit. */
/* Snake Byte's machine state at $3750, captured from snake-bytec1-ext-run. */

/* RAM $0000-$0802. Outside this: the game image, the ROM, and $FF. */
#define SB_ENTRY_RAM_LEN 0x803
static const uint8_t kSnakeByteEntryRam[SB_ENTRY_RAM_LEN] = {
  76, 60, 212, 76, 58, 219, 255, 255, 255, 255, 76, 153, 225, 255, 0, 107,
  255, 0, 255, 4, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  0, 40, 0, 24, 0, 3, 255, 255, 128, 5, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 221, 255, 40, 240, 253, 27, 253, 255, 255, 255, 255, 0, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 255, 255, 255, 255, 10, 0,
  80, 55, 85, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 56, 255,
  255, 255, 255, 255, 255, 255, 255, 1, 8, 3, 8, 3, 8, 3, 8, 0,
  192, 255, 255, 0, 192, 255, 255, 255, 255, 255, 0, 255, 255, 0, 8, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 255, 255, 255, 255, 255, 3,
  76, 255, 0, 255, 255, 255, 255, 255, 255, 0, 0, 0, 0, 142, 0, 0,
  55, 80, 0, 0, 0, 142, 221, 64, 0, 0, 0, 0, 0, 10, 255, 3,
  8, 230, 184, 208, 2, 230, 185, 173, 6, 2, 201, 58, 176, 10, 201, 32,
  240, 239, 56, 233, 48, 56, 233, 208, 96, 128, 79, 199, 82, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 0, 255, 127, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 1, 0, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 101, 235, 216, 220, 236, 99, 236,
  151, 14, 236, 90, 231, 218, 241, 34, 216, 193, 241, 0, 1, 1, 168, 250,
  140, 49, 52, 49, 54, 48, 0, 49, 0, 48, 0, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  89, 250, 3, 224, 69, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 193,
  208, 208, 204, 197, 160, 221, 219, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 255, 255, 255, 255, 255, 255, 255, 255,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 255, 255, 255, 255, 255, 255, 255, 255,
  221, 195, 193, 204, 204, 160, 177, 180, 177, 182, 176, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 255, 255, 255, 255, 255, 255, 255, 255,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 255, 255, 255, 255, 255, 255, 255, 255,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 255, 255, 255, 255, 255, 255, 255, 255,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 255, 255, 255, 255, 255, 255, 255, 255,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 255, 255, 255, 255, 255, 255, 255, 255,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
  160, 160, 160, 160, 160, 160, 160, 160, 193, 255, 255, 255, 255, 255, 255, 255,
  0, 0, 0,
};

#define SB_ENTRY_A 0x37
#define SB_ENTRY_X 0x9D
#define SB_ENTRY_Y 0x50
#define SB_ENTRY_SP 0xF6
#define SB_ENTRY_STATUS 0x00
#define SB_ENTRY_VID_CONTROL 0x01

/* ========================================================================== *
 * Startup                                                                  *
 * ========================================================================== */

void init_emulated(void);

void init_emulated(void) {
  /* Loads the game image at $3750-$854E, and the $00B1-$00C8 zero-page segment
     the run data carries. */
  load_images();

  /* Unwritten RAM reads as $FF on a real machine, and the game does read some
     of it: game_rand_byte_native walks a pointer up from $1800 looking for a
     byte with bit 7 clear, and against a zero-filled machine it would find one
     immediately and never move. The generated init zeroes, so restore $FF --
     but only outside the game image, which is already in place above.

     $1800-$1FFF is included: $3750's first act is to copy the level data over
     it, so what is there beforehand does not matter, and $FF is what the
     machine would have had. */
  for (unsigned a = 0x0803; a != 0x3750; ++a)
    ram_poke((uint16_t)a, 0xFF);
  for (unsigned a = 0x854F; a != 0xC000; ++a)
    ram_poke((uint16_t)a, 0xFF);

  /* Then the state the boot itself produced: zero page, the stack and its live
     contents, the $03xx vectors, and the text screen. This comes last because
     it must win over both of the above. */
  for (unsigned i = 0; i != SB_ENTRY_RAM_LEN; ++i)
    ram_poke((uint16_t)i, kSnakeByteEntryRam[i]);

  /* The plotter's block used to be $0000-$0008 and is a C object now, so the
     line above no longer initialises it. Take it from the same snapshot, or
     the first read before any write would see zero where the booting build
     sees what BASIC left there. */
  s_shape = kSnakeByteEntryRam[0x00];
  s_ink = kSnakeByteEntryRam[0x01];
  s_ch = kSnakeByteEntryRam[0x24];
  s_cv = kSnakeByteEntryRam[0x25];
  s_wndlft = kSnakeByteEntryRam[0x20];
  s_wndwdth = kSnakeByteEntryRam[0x21];
  s_wndtop = kSnakeByteEntryRam[0x22];
  s_wndbtm = kSnakeByteEntryRam[0x23];
  s_gbasl = kSnakeByteEntryRam[0x26];
  s_gbash = kSnakeByteEntryRam[0x27];
  s_basl = kSnakeByteEntryRam[0x28];
  s_bash = kSnakeByteEntryRam[0x29];
  s_bas2l = kSnakeByteEntryRam[0x2a];
  s_bas2h = kSnakeByteEntryRam[0x2b];
  s_h2 = kSnakeByteEntryRam[0x2c];
  s_v2 = kSnakeByteEntryRam[0x2d];
  s_mask = kSnakeByteEntryRam[0x2e];
  s_color = kSnakeByteEntryRam[0x30];
  s_invflg = kSnakeByteEntryRam[0x32];
  s_ysav1 = kSnakeByteEntryRam[0x35];
  s_cswl = kSnakeByteEntryRam[0x36];
  s_cswh = kSnakeByteEntryRam[0x37];
  s_kswl = kSnakeByteEntryRam[0x38];
  s_kswh = kSnakeByteEntryRam[0x39];
  s_a2l = kSnakeByteEntryRam[0x3e];

  /* Registers. SP matters most -- the live stack bytes above are meaningless
     without it. The flags are $A0: N set, everything else clear. */
  s_a = SB_ENTRY_A;
  s_x = SB_ENTRY_X;
  s_y = SB_ENTRY_Y;
  s_sp = SB_ENTRY_SP;
  s_status_n = SB_ENTRY_STATUS & 0x80;
  s_status_v = (SB_ENTRY_STATUS & 0x40) != 0;
  s_status_b = (SB_ENTRY_STATUS & 0x10) != 0;
  s_status_d = (SB_ENTRY_STATUS & 0x08) != 0;
  s_status_i = (SB_ENTRY_STATUS & 0x04) != 0;
  s_status_not_z = !(SB_ENTRY_STATUS & 0x02);
  s_status_c = (SB_ENTRY_STATUS & 0x01) != 0;

  /* The soft switches, which no RAM image carries. $01 is text, page 1, not
     mixed -- the power-on state, so these reads are asserting it rather than
     changing it. Reading a soft switch is what selects it. */
  (void)SB_ENTRY_VID_CONTROL;
  io_peek(0xc051); /* text */
  io_peek(0xc052); /* not mixed */
  io_peek(0xc054); /* page 1 */

  /* The keyboard, which is neither RAM nor a soft switch and is the last piece
     of state the boot leaves behind. Two things to get right.

     First, $C000 does not read zero on an Apple II that has had a key pressed:
     it reads the last key with bit 7 clear, and the boot's final act was the
     Return that submitted CALL 14160.

     Second, and only visible because the traces diverged: a2host pushes a dummy
     Return into the queue whenever a key source is present, because the first
     key pressed before initialization would otherwise be lost. A booting
     program never notices -- KEYIN swallows it at the prompt. This one would
     ingest it as the player's first keystroke. It is pushed before
     init_emulated() precisely so it can be consumed here.

     Draining the queue and then latching $0D does both at once. */
  while (a2_io_keys_count(a2host_io()) != 0)
    io_poke(0xc010, 0);
  a2_io_push_key(a2host_io(), 0x0d);
  io_peek(0xc000);
  io_poke(0xc010, 0);
}

