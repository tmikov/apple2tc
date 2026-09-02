/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// Snake Byte: the whole game in one translation unit, sharing nothing.
///
/// It starts at the game's own entry point with the machine state an Apple II
/// boot would have left, and runs the game as C. There is no Applesoft, and
/// nothing here is generated.
///
/// Why one file for the game
/// -------------------------
/// It no longer has to be. `apple2tc/system2-inc.h` used to *define* the
/// emulated machine with internal linkage, so anything built on it shared one
/// translation unit; the machine is now split into `system2.h` and
/// `system2-impl.inc`, and this program compiles the second half once in
/// `system2-impl.c`. This file includes only the declarations.
///
/// The better reason is ownership. The other targets in this directory are
/// scaffolding -- controls this one is checked against, and one fixture -- and
/// while this file shared sources with them, every change here had to stay
/// safe for builds running a dispatch over the same addresses. It shares
/// nothing now. Change it freely; the gate says whether the game still
/// behaves.
///
/// What is in here that is not the game
/// ------------------------------------
///   - `s_mem_3750`, the game's own binary image, for its data: the level
///     scripts, the font, the tables.
///   - `s_mem_d000`, the Apple II ROM image -- not for its code, but because
///     the death pause reads its delay lengths out of ROM *as data*.
///   - the ROM entry points the game calls, as C. They are the machine's code
///     rather than the game's, but there is no category of code here that
///     belongs to someone else.
///   - the entry state: zero page, the stack, the registers and one soft
///     switch, captured with --snapshot-at. See make-entry-state.sh.
///
/// The two images are 2,020 lines of hex, so they live in `game-image.inc` and
/// `rom-image.inc`. That is a concession to reading the file, not a retreat
/// from owning it.
///
/// How it is checked
/// -----------------
/// `probe-acceptance.sh` runs this against a build that boots the real machine
/// and requires the two to agree: the screen and memory at every in-game
/// sample, and the speaker's toggle timeline. The screen check is the one that
/// survives as more of this file turns into ordinary C.
///
/// One hazard when editing. `assert()` bakes `__LINE__` into the text section,
/// so deleting a comment changes the emitted code. To prove an edit is
/// comment-only, compile `-O2 -g0 -DNDEBUG -S` before and after and diff.

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "apple2tc/a2host_api.h"
#include "apple2tc/a2io.h"

/* ========================================================================== *
 * The machine, the images, and what is left of the ROM                     *
 * ========================================================================== */

#include "apple2tc/system2.h"

#include "clock.h"
#include "rom.h"

/// Defined next to click_speaker; declared here because the sound routines
/// above it also toggle the speaker.
static void speaker_access(uint8_t port);


/// The zero-page fragment $00B1-$00C8 the run data carried: the six-byte
/// CHRGET routine Applesoft assembles there at boot. Small enough to read,
/// so unlike the other two images it stays in the file.
static const uint8_t s_mem_00b1[0x0018] = {0xE6, 0xB8, 0xD0, 0x02, 0xE6, 0xB9, 0xAD, 0x06,
                                           0x02, 0xC9, 0x3A, 0xB0, 0x0A, 0xC9, 0x20, 0xF0,
                                           0xEF, 0x38, 0xE9, 0x30, 0x38, 0xE9, 0xD0, 0x60};

#include "game-image.inc"
#include "rom-image.inc"

/// Put the three images into RAM: the zero-page fragment the run data carried,
/// the game itself, and the Apple II ROM.
static void load_images(void) {
  memcpy(s_ram + 0x00b1, s_mem_00b1, 0x0018);
  memcpy(s_ram + 0x3750, s_mem_3750, 0x4dff);
  memcpy(s_ram + 0xd000, s_mem_d000, 0x3000);
}

static uint16_t adc_dec16(uint8_t a, uint8_t b, uint8_t cf) {
  struct ResAndStatus res = adc_decimal(a, b, cf);
  return res.result | (res.status << 8);
}
static uint16_t sbc_dec16(uint8_t a, uint8_t b, uint8_t cf) {
  struct ResAndStatus res = sbc_decimal(a, b, cf);
  return res.result | (res.status << 8);
}


/// A cell on the 40x48 playfield grid.
typedef struct {
  uint8_t col;
  uint8_t row;
} Cell;

/// Lo-res colour, and also the ink index the hi-res cell drawers take. The
/// game uses one numbering for both. Named for what each is used to draw,
/// because that is the question somebody changing them will have.
typedef enum {
  INK_ERASE = 0x00,      ///< black, i.e. rub out whatever is there
  INK_WALL_TOP = 0x02,   ///< the side walls above the seam
  INK_GATE = 0x03,       ///< the gap the snake leaves through, and the bouncers
  INK_OCCUPIED = 0x05,   ///< the occupancy map's "something is here"
  INK_HEAD_MARK = 0x07,  ///< the head's own mark on the occupancy map
  INK_APPLE = 0x09,
  INK_SNAKE = 0x0c,
  INK_WALL_BOTTOM = 0x0d, ///< the side walls below the seam
  INK_WHITE = 0x0f,
} Ink;

/// Which of the shape table's entries a cell drawer stamps. Shapes are stored
/// four scanline masks apiece; see game_load_shape_masks.
typedef enum {
  SHAPE_APPLE = 0x01,   ///< also merged over the head so it reads as a head
  SHAPE_VBAR = 0x0a,    ///< a full-height vertical bar, one cell wide
  SHAPE_SOLID = 0x15,   ///< the default: a full cell, used for walls and text
  SHAPE_BOUNCER = 0x1a,
} Shape;

/// What the lo-res occupancy map holds at a cell. Anything else is solid.
typedef enum {
  CELL_EMPTY = 0x00,
  CELL_APPLE = 0x0f,
} CellContent;

/// One of the two objects that ricochet around the playfield. The original
/// keeps them as four parallel pairs of bytes and copies one set into
/// $6633-$6636 before stepping it -- a hand-rolled calling convention, which
/// is why a struct fits so exactly, and which step_bouncer_slot now spells as
/// passing one.
typedef struct {
  uint8_t col;
  uint8_t row;
  /// Always +1 or -1. The original stores them as bytes and reflects with
  /// `EOR #$FE`, which swaps $01 and $FF.
  int8_t dx;
  int8_t dy;
} Bouncer;

static void game_cold_start(void);
static void game_print_inline_str(uint16_t ret_addr);

/// The program starts here. There used to be a func_t001 in between -- the
/// generated dispatch -- and by the end it was a stub that called this.
void emulated_entry_point(void) {
  game_cold_start();
}



/* ========================================================================== *
 * The game as ordinary C -- declarations                                   *
 * ========================================================================== */


/* --- Converted routines --------------------------------------------------- */

/// start a life: put the snake's head in \p head_col and set both
/// bouncers going from opposite corners. Returns the value the original left
/// in A, which its one caller stores as the tail column.
static uint8_t game_start_life(uint8_t head_col);

/// copy shape \p shape's four scanline masks into $6060, and return
/// the last one.
static void game_load_shape_masks(uint8_t shape);

/// point the ROM's character-output vector at the game's own hi-res
/// handler, so every later COUT reaches game_cout_hook.
static void game_install_cout_vector(void);

/// step \p b one cell along its deltas and redraw it, reflecting off
/// whatever it hits.
static void bouncer_step(Bouncer *b);

/// What $6AB8 decided about a candidate move. The original says all of this
/// in A and the Z flag; here it is the return type.
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

/// judge a step in direction \p dir. \p cell_out receives what the
/// occupancy map held at the target, which the original leaves in A.
static MoveVerdict snake_move_verdict(uint8_t dir, uint8_t *cell_out);

/// copy s_progress.score over s_progress.best if it beats
/// it, comparing BCD bytes most significant first.
static void game_promote_high_score(void);

/// $6148 / $615A -- runs of hi-res cells along a row or down a column.
static void game_plot_hline(uint8_t ink, Cell c, uint8_t to_col);
static void game_plot_vline(uint8_t ink, Cell c, uint8_t to_row);

/// the lo-res half of a vertical run, restoring $03.
static void game_lores_vline(Cell c, uint8_t to_row);

/// step the bouncers the difficulty calls for, then return the next
/// queued key.
static uint8_t game_step_bouncers(void);

/// draw the loaded shape into cell \p c in ink \p ink, replacing.
static void game_draw_cell(uint8_t ink, Cell c);

/// the same, merged into what is already there.
static void game_merge_cell(uint8_t ink, Cell c);

/// zero hi-res page 1.
static void game_clear_hgr(void);

/// clear the screen, draw the border, and run the current level's
/// display list from $8000.
static void game_draw_playfield(void);

/// Sweep columns outward from the snake looking for an apple, and leave the
/// nearest one where the auto-steer will find it.
static void game_find_nearest_apple(void);

/// turn \p key, the byte just taken off the ring, into the code the
/// game acts on: a direction, a joystick setting applied on the spot, or $00
/// for nothing. Reads the joystick itself when one is selected and the key
/// was not a direction.
static uint8_t game_read_direction(uint8_t key);

/// blink slot \p slot on the key-redefinition screen until the player
/// presses something it will accept, and return that key.
static uint8_t game_edit_key(uint8_t slot);

/// the program's entry, and the outermost loop: relocate the level
/// data, initialise the game's state, then new game -> level -> round -> life
/// forever. Never returns; the game has no way out.
static void game_cold_start(void);

/// ESC pauses until any key is pressed; Ctrl-S toggles the sound.
/// Every key the dispatch chain did not recognise arrives here and is ignored.
///
/// Returns the key the Ctrl-S test actually saw, which is \p key unless ESC
/// paused: the keypress that ends the pause replaces it and is tested in turn.
static void game_pause_or_toggle_sound(uint8_t key);

/// twenty passes of the falling tone that plays while the head moves.
static void game_tick_sound(void);

/// print \p byte as two decimal digits, dropping leading zeros.
static void game_print_bcd(uint8_t byte);

/// print a single "0" if the number just printed was all zeros.
static void game_print_zero_if_blank(void);

/// add the two-byte BCD value in s_progress.apple_value to the four-byte score.
static void game_add_score(void);

/// set the lo-res plot colour from an ink byte: 0 erases, anything
/// else draws.
static void game_set_ink(uint8_t ink);

/// read the byte at the $000A pointer into A and advance it.
static uint8_t game_next_byte(void);

/// the game's pseudo-random byte, always $00-$7F.
static uint8_t game_rand_byte(void);

/// put an apple on a free cell, by rejection sampling.
static Cell game_place_apple(void);

/// recompute what one apple is worth for the current level.
static void game_set_apple_value(void);

/// plot the head on the occupancy map and flag it as newly there.
static void game_mark_head(uint8_t row, uint8_t col);

/// draw a cell, merging the head shape over it if the head is on it.
static void game_draw_head(uint8_t ink, Cell c);

/// count one apple eaten, and make the noise for it.
static void game_award_extra_life(void);

/// load a shape and draw it into the current cell.
static void game_plot_shape(uint8_t ink, Cell c);

/// the rising-then-falling sweep an eaten apple makes.
static void game_sound_sweep(void);

/// show \p key as slot \p slot's binding on the redefinition screen.
static void game_show_key(uint8_t slot, uint8_t key);

/// draw both side walls, and leave SCRN of the bottom-centre cell.
static uint8_t game_draw_side_walls(void);

/// poll the keyboard and push what it finds into the ring at $623C.
static void game_read_key(void);

/// draw \p ch through the game's own hi-res font, then hand it on to
/// the ROM's COUT1 so the cursor still moves.
void game_cout_hook(uint8_t ch);

/// Why a life ended. The original said all of this in a single byte, which its
/// caller read the moment the routine returned; the value still goes into
/// s_life_outcome for the code that has not been converted yet.
typedef enum {
  /// Reached the gate at the bottom of the playfield.
  LIFE_GATE,
  /// Moved onto an apple.
  LIFE_APPLE,
  /// The player pressed the quit key.
  LIFE_QUIT,
  /// The level timer ran out.
  LIFE_TIMEOUT,
  /// Moved onto something solid. \p cell_out receives the occupancy byte,
  /// which is how the caller tells a wall from the snake's own tail.
  LIFE_CRASH,
  /// Not a life-ending outcome. snake_step()'s signal that the head landed
  /// on an empty, non-gate cell and the life goes on to `tail:`. Kept last
  /// and named apart from the four real endings above so it cannot be
  /// mistaken for one; game_play_loop() never returns it, so no caller
  /// outside snake_step() ever sees it.
  LIFE_CONTINUE,
} LifeEnd;

/// play one life: steer, move, draw and pace the snake until
/// something ends it. \p cell_out receives the occupancy byte the head landed
/// on, which is only meaningful for LIFE_CRASH.
static LifeEnd game_play_loop(uint8_t *cell_out);

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

/// Steer toward the nearest apple, trying candidate directions in order of
/// usefulness and taking the first that snake_move_verdict() allows.
static SteerChoice game_auto_steer(uint8_t *key_out);

/// draw the status panel: six labelled BCD fields in a 2x3 grid
/// across the bottom three text rows, then home the cursor.
static void game_status_panel(void);

/// the bonus screen: award twice the apple's value, draw a box over
/// the playfield, print BONUS and the amount, and hold it there.
static void game_bonus_screen(void);

/// set up a life and hand over to the main loop: the snake as a
/// single cell at the bottom centre facing up, ten segments of growth owed,
/// the timer full, and the key ring empty.
static void game_begin_life(void);

/// the setup screen: seed the random pointer, then either ask for a
/// difficulty (falling back to the demo if nobody answers) or run the key
/// redefinition screen.
static void game_setup_screen(void);

/* ========================================================================== *
 * Routines decompiled by hand -- declarations                              *
 * ========================================================================== */

/// the game's own COUT handler, a hi-res text renderer.
/// print the NUL-terminated string that follows the call.
static void game_print_inline_str(uint16_t ret_addr);

/// Step \p b one cell along its deltas and redraw it, reflecting off whatever
/// it hits.

/// Play one life, and leave the reason it ended in s_life_outcome.
static void game_play_one_life(void);


/* ========================================================================== *
 * The game, as ordinary C                                                  *
 * ========================================================================== */

/* ========================================================================== */
/* Storage                                                                    */
/* ========================================================================== */

/// The two bouncers. They lived at $6639-$6640, and not as two structs: the
/// fields were interleaved in pairs -- both columns, then both rows, then both
/// dx, both dy -- so nothing could be overlaid on them and every access was a
/// load or a store of four separate bytes.
///
/// Out of RAM the interleaving has nothing left to say, and this is an array
/// of two. The initialisers are the shipped image's bytes de-interleaved;
/// game_start_life overwrites both before the first life.
static Bouncer s_bouncers[2] = {
    {.col = 0x04, .row = 0x04, .dx = +1, .dy = +1},
    {.col = 0x26, .row = 0x01, .dx = -1, .dy = +1},
};

/* --- The game's own tables ------------------------------------------------ */
/*
 * Read-only data inside the loaded image, and const arrays here. They were
 * ram_peek(kTable + i) until 2026-08-25, which is what a 6502 has to write and
 * not what the data is: every one of these is a lookup indexed by a small
 * integer.
 *
 * Every extent below is *derived*, not assumed -- the playbook's own red flag,
 * because the listing does not delimit a table and a 16-byte one that is
 * really 128 is invisible until something indexes past 16. Each says where its
 * end comes from. The bytes are the shipped image's, transcribed from
 * snake-byte.b33; a wrong byte in any of them moves pixels, and the screen
 * oracle compares 6,808 and 9,524 samples.
 */

/// each hi-res cell row's base address. The original splits the
/// 48 addresses into parallel low and high tables because a 6502 indexes bytes;
/// one table of addresses is the same data said once. kHgrLineHi ended at
/// $605F, where the shape masks began.
static const uint16_t kHgrLineBase[48] = {
    0x2000, 0x3000, 0x2080, 0x3080, 0x2100, 0x3100, 0x2180, 0x3180, 0x2200, 0x3200, 0x2280, 0x3280,
    0x2300, 0x3300, 0x2380, 0x3380, 0x2028, 0x3028, 0x20a8, 0x30a8, 0x2128, 0x3128, 0x21a8, 0x31a8,
    0x2228, 0x3228, 0x22a8, 0x32a8, 0x2328, 0x3328, 0x23a8, 0x33a8, 0x2050, 0x3050, 0x20d0, 0x30d0,
    0x2150, 0x3150, 0x21d0, 0x31d0, 0x2250, 0x3250, 0x22d0, 0x32d0, 0x2350, 0x3350, 0x23d0, 0x33d0,
};

/// dot patterns, indexed by dot_index(): ink 0-15, scanline parity,
/// column mod 4, so ((15*2+1)<<2)|3 == 127 is the largest index and the table
/// is exactly 128. known-data.txt says the same: "ends exactly at hgr_draw".
static const uint8_t kHgrPattern[128] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x91, 0xa2, 0xc4, 0xa2, 0xc4, 0x88, 0x91,
    0xc4, 0x88, 0x91, 0xa2, 0x91, 0xa2, 0xc4, 0x88, 0x55, 0x2a, 0x55, 0x2a, 0x55, 0x2a, 0x55, 0x2a,
    0x08, 0x11, 0x22, 0x44, 0x22, 0x44, 0x08, 0x11, 0x2a, 0x55, 0x2a, 0x55, 0x55, 0x2a, 0x55, 0x2a,
    0xd5, 0xaa, 0xd5, 0xaa, 0xd5, 0xaa, 0xd5, 0xaa, 0xf7, 0xee, 0xdd, 0xbb, 0xdd, 0xbb, 0xf7, 0xee,
    0x11, 0x22, 0x44, 0x08, 0x44, 0x08, 0x11, 0x22, 0xaa, 0xd5, 0xaa, 0xd5, 0xaa, 0xd5, 0xaa, 0xd5,
    0x33, 0x66, 0x4c, 0x19, 0x4c, 0x19, 0x33, 0x66, 0xaa, 0xd5, 0xaa, 0xd5, 0x55, 0x2a, 0x55, 0x2a,
    0x2a, 0x55, 0x2a, 0x55, 0x2a, 0x55, 0x2a, 0x55, 0xaa, 0xd5, 0xaa, 0xd5, 0x2a, 0x55, 0x2a, 0x55,
    0x3b, 0x77, 0x6e, 0x5d, 0x6e, 0x5d, 0x3b, 0x77, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
};

/// four AND masks per shape, indexed (shape << 2) + line.
///
/// The extent is the one that had to be worked out rather than looked up. The
/// data ends at $61DF -- 27 shapes, the last of them $1A, which is the largest
/// the code names. But game_play_loop draws the tail with shape `ahead + $0C`,
/// where `ahead` is a lo-res cell and so 0-15, which reaches shape $1B and
/// index 111. $61E0-$61FF is zero, and $6200 is code (dequeue_key). So the
/// table runs to the code boundary, indices 108-139 are the padding the
/// original reads in that case, and nothing can index past it: the byte
/// arithmetic wraps at 256, but shape is bounded by $1B.
static const uint8_t kShapeMaskTable[140] = {
    0x00, 0x00, 0x00, 0x00, 0x8c, 0xbf, 0x8c, 0x00, 0x00, 0x8c, 0xbf, 0x8c, 0x98, 0xfe, 0x98, 0x00,
    0x8c, 0xbf, 0x8c, 0x00, 0xb3, 0xb0, 0x8f, 0x00, 0x8f, 0xb0, 0xb3, 0xb3, 0xfc, 0x83, 0xf3, 0xb3,
    0xf3, 0x83, 0xfc, 0x00, 0xff, 0x00, 0xff, 0x00, 0xb3, 0xb3, 0xb3, 0xb3, 0xff, 0x00, 0xff, 0x00,
    0xb3, 0xb3, 0xb3, 0xb3, 0xe0, 0xff, 0xe0, 0x00, 0xbf, 0x0c, 0x0c, 0x0c, 0x83, 0xff, 0x83, 0x00,
    0x0c, 0x0c, 0x0c, 0xbf, 0x8f, 0xb0, 0xb3, 0xb3, 0xfc, 0x83, 0xf3, 0xb3, 0xf3, 0x83, 0xfc, 0x00,
    0xb3, 0xb0, 0x8f, 0x00, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xf8, 0xfe, 0xff, 0xff, 0xbf, 0x0f, 0x03,
    0x03, 0x0f, 0xbf, 0xff, 0xff, 0xfe, 0xf8, 0xe0, 0x9c, 0xe3, 0xe3, 0x9c, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

/// one five-entry table per absolute-direction key,
/// giving the relative turn that achieves it from the current direction, or
/// $00 for "nothing to do". Five bytes each because they are indexed by
/// direction 1-4 with entry 0 unused; $639B is code, which is where the fourth
/// one ends.
static const uint8_t kTurnForKey[4][5] = {
    {
        0x00,
        0x88,
        0x00,
        0x95,
        0x00,
    },
    {
        0x00,
        0x00,
        0x88,
        0x00,
        0x95,
    },
    {
        0x00,
        0x00,
        0x95,
        0x00,
        0x88,
    },
    {
        0x00,
        0x95,
        0x00,
        0x88,
        0x00,
    },
};

/// direction to the key that turns to it. known-data.txt derives the
/// five bytes from the LDA $6A55,X that reads them.
static const uint8_t kSteerKey[5] = {
    0x00,
    0xcb,
    0xc9,
    0xca,
    0xcd,
};

/// the six keys as shipped. The live six at $6C63 are s_input.bindings,
/// which the redefinition screen writes; these are never written.
static const uint8_t kKeyDefaults[6] = {
    0xc9,
    0xca,
    0xcb,
    0xcd,
    0x88,
    0x95,
};

/// what one apple is worth, indexed by difficulty 0-2. Three bytes:
/// s_progress.apple_value is s_progress.apple_value, which is where the table stops.
static const uint8_t kAppleValueTable[3] = {
    0x10,
    0x15,
    0x20,
};

/// $75B3-$75D0 -- the redefinition screen's layout, five six-entry tables, one
/// per slot: where the key is printed, where its arrow is, and which glyph the
/// arrow uses. known-data.txt derives the span.
static const uint8_t kKeyCH[6] = {
    0x0a,
    0x06,
    0x0e,
    0x0a,
    0x1a,
    0x22,
};
static const uint8_t kKeyCV[6] = {
    0x05,
    0x09,
    0x09,
    0x0d,
    0x09,
    0x09,
};
static const uint8_t kArrowCH[6] = {
    0x0a,
    0x08,
    0x0c,
    0x0a,
    0x1c,
    0x20,
};
static const uint8_t kArrowCV[6] = {
    0x07,
    0x09,
    0x09,
    0x0b,
    0x09,
    0x09,
};
static const uint8_t kArrowGlyph[6] = {
    0xe2,
    0xe4,
    0xe5,
    0xe3,
    0xe4,
    0xe5,
};

/* --- What the bouncer step left behind ------------------------------------ */
/*
 * $6637, $6638 and $6C4A held where the step would have landed before the
 * walls got a say, and whether that cell was occupied. Nothing in this file
 * ever read any of the three -- bouncer_step works in locals and commits into
 * the Bouncer it was handed -- so the three stores existed only because the
 * memory oracle hashed the bytes. Out of RAM they are dead code, and they are
 * gone. The addresses are recorded here because that is the only place the
 * knowledge now lives.
 */

/* --- The auto-steer's answer ---------------------------------------------- */
/*
 * $6B38/$6B39 is s_apple. $6B39/$6B3A were the apple sweep's cursor, left
 * wherever the search stopped, and went the same way as the three above:
 * written at the end of the sweep, read by nothing. s_apple is the answer
 * and is real.
 */

/// The cell the sweep decided to steer towards.
static Cell s_apple = {.col = 0x13, .row = 0x1d};

/* --- Sound: s_sound and $69C2 ----------------------------------------------- */

/// The one-bit speaker. `port` is $30 for the speaker and $20 for the cassette
/// output, which is where a muted click goes.
static struct {
  uint8_t period;    ///< the tone's period, and its on/off switch: 0 is silent
  uint8_t countdown; ///< passes left before the next click
  uint8_t passes;    ///< how many passes one tick of the tone runs
  /// How many more clicks the current sound effect owes. The pace loop
  /// spends one per pass while it is nonzero, so the value is a duration:
  /// $20 for eating an apple, $10 for a steering input, $07 for growing.
  uint8_t click_count;
  uint8_t port;  ///< the low byte of the soft switch: $30 the speaker, $20 the cassette
  bool muted;    ///< toggled by Ctrl-S
} s_sound = {.port = 0x20, .passes = 0x0f};

/// SCRN one cell. Defined further down, next to the plot helpers.
static uint8_t scrn_cell(Cell c);

/// Plot the loaded shape, or a named one, into a cell. Defined further down,
/// next to the run helpers; declared here because the bouncers and the snake
/// use them well before that.
static void plot_at(uint8_t ink, Cell c);
static void plot_shape_at(uint8_t shape, uint8_t ink, Cell c);

/* --- The snake, the key ring, and the click counter ----------------------- */
/*
 * One contiguous run in the original, holding everything a life consists of. The two delta tables are read-only image data and stay where
 * they are, as const arrays; everything after them is state and is C
 * variables. The initialisers are the shipped image's bytes, which for this
 * block are the leftovers of whoever last ran the game before it was
 * assembled: game_begin_life overwrites all of them, but game_draw_status and
 * the first pace read some of them first.
 */

/// The four directions, as $624E numbered them.
enum { DIR_RIGHT = 1, DIR_UP = 2, DIR_LEFT = 3, DIR_DOWN = 4 };

/// Column and row deltas, indexed by direction 1..4. Entry 0 is $00 in both
/// and is never reached, since a direction of 0 means "no key".
/// Measured against DIR_RIGHT..DIR_DOWN: columns are +1/0/-1/0 and rows are
/// 0/-1/0/+1, so right is +column and up is -row, screen order.
static const uint8_t kColDelta[5] = {0x00, 0x01, 0x00, 0xff, 0x00};
static const uint8_t kRowDelta[5] = {0x00, 0x00, 0xff, 0x00, 0x01};

/// Which shape the head is drawn with, by how it is turning and which way it
/// ends up facing. The original computes these by adding $10, $04 or $08 to
/// the direction, which is the same table with the rows implied.
typedef enum { TURN_CW, TURN_CCW, TURN_STRAIGHT } Turn;

/// Entry 0 of each row is unused padding, same as kColDelta/kRowDelta above:
/// directions are 1-based, and a direction of 0 never reaches this table.
static const uint8_t kSnakeShape[3][5] = {
    [TURN_CW] = {0, 0x11, 0x12, 0x13, 0x14},
    [TURN_CCW] = {0, 0x05, 0x06, 0x07, 0x08},
    [TURN_STRAIGHT] = {0, 0x09, 0x0a, 0x0b, 0x0c},
};

/// The keyboard ring the poll fills and the play loop drains, and the six
/// bindings the redefinition screen edits.
static struct {
  /// The sixteen-entry ring the keyboard scan fills and the play loop
  /// drains. Only a byte with bit 7 set counts as present.
  uint8_t ring[16];
  /// Where the play loop reads next, and where the scan writes next. The
  /// scan refuses to advance the write cursor onto the read one, which is
  /// the ring's only full test. Both wrap with `& $0F`.
  uint8_t read, write;
  /// the six bound keys, in slot order. The key-redefinition screen
  /// writes them; kKeyDefaults is the same six as shipped and is never
  /// written.
  uint8_t bindings[6];
  bool joystick; ///< the player chose the joystick at the setup prompt
} s_input = {.bindings = {0xc9, 0xca, 0xcb, 0xcd, 0x88, 0x95}};

/// Everything belonging to the snake currently on the playfield.
static struct {
  /// The head's cell, and the tail's. The snake itself is not stored -- the
  /// lo-res screen is the occupancy map, and the tail walks it by reading the
  /// colour it finds to work out which way the body went.
  Cell head, tail;
  uint8_t direction; ///< the direction the snake is travelling, DIR_RIGHT..DIR_DOWN
  /// Segments still owed. While it is nonzero the tail is not trimmed, so the
  /// snake grows; a life starts with ten and each apple adds ten more.
  uint8_t growth;
  /// The head moved this step, so the next draw merges the head shape over
  /// the cell. game_mark_head raises it, game_draw_head reads it and clears
  /// it.
  bool head_moved;
  /// which shape to draw. game_load_shape turns it into the four
  /// scanline masks at $6060.
  ///
  /// A variable and not a parameter, which is what the original makes it:
  /// bouncer_step erases where the bouncer was using whatever was last left
  /// here, so the value outlives any one call. Every path that could have
  /// made that a hazard was checked -- see the note below.
  uint8_t shape;
  /// the loaded shape's four scanline masks, which game_load_shape
  /// copies out of kShapeMaskTable and the two hi-res cell drawers read.
  /// $FF FF FF FF in the shipped image, which is what a shape of all dots
  /// would leave there.
  uint8_t shape_mask[4];
  /// The direction $6A32 settled on. game_move_ok and key_for_direction both
  /// read it back rather than being passed it.
  uint8_t steer_dir;
} s_snake = {
    .head = {.col = 0x14, .row = 0x24},
    .tail = {.col = 0x14, .row = 0x27},
    .direction = DIR_UP,
    .growth = 0x07,
    .shape_mask = {0xff, 0xff, 0xff, 0xff},
    .steer_dir = DIR_UP,
};

/// How the life ended, which $7739 reads the moment the play loop returns:
/// $00 the gate, $0F an apple, $FF the quit key, $FE the timer, anything
/// else the occupancy byte the head ran into. See LifeEnd, which is this
/// byte with names on.
///
/// The gate and apple cases deliberately reuse CellContent's encoding
/// (CELL_EMPTY/CELL_APPLE) rather than inventing their own: the gate is
/// reached by moving onto an empty cell, and eating is moving onto the apple
/// cell, so the sentinel *is* what a scan would have returned. 0xFF and 0xFE
/// are picked for quit/timeout precisely because no real occupancy byte
/// (0x00-0x0F) can ever collide with them.
static uint8_t s_life_outcome;

/// The two life outcomes above that are not real occupancy bytes and so
/// cannot borrow CellContent's encoding the way the gate and apple cases do.
enum { OUTCOME_TIMEOUT = 0xfe, OUTCOME_QUIT = 0xff };

/// The life timer -- see s_life_time for what seeds it and why $FF stops it.
static uint8_t s_life_timer = 0x61;

/* ========================================================================== */
/* Converted routines                                                         */
/* ========================================================================== */

static uint8_t game_start_life(uint8_t head_col) {
  s_snake.head.col = head_col;

  // Opposite corners, converging. The original's nine stores are these two.
  const Bouncer a = {.col = 0x01, .row = 0x01, .dx = +1, .dy = +1};
  const Bouncer b = {.col = 0x26, .row = 0x01, .dx = -1, .dy = +1};
  s_bouncers[0] = a;
  s_bouncers[1] = b;

  // $6630 `LDA #$14`. Its one caller, $6256, stores this as the tail column;
  // it is not related to head_col, which happens to be $14 as well.
  return 0x14;
}

static void game_load_shape_masks(uint8_t shape) {
  // Four masks per shape at $6174, into the four the plotter reads.
  uint8_t last = 0;
  for (unsigned line = 0; line < 4; ++line) {
    // The cast is the index: `shape << 2` overflows a byte at shape $40 and the
    // original's ASL wraps, so the table is 256 long and this stays inside it.
    last = kShapeMaskTable[(uint8_t)((shape << 2) + line)];
    s_snake.shape_mask[line] = last;
  }
}

static void game_install_cout_vector(void) {
  // CSWL/CSWH at $36/$37, pointed at $664A.
  s_mon.csw = 0x664a;
}

/* ========================================================================== */
/* the bouncer                                                       */
/*                                                                            */
/* What the original spends bytes on and this does not: $6633-$6636 are a      */
/* parameter block the caller copies in and out, $6637/$6638 are the candidate */
/* cell, and $6C4A counts how many axes were blocked. All five are locals in   */
/* any language with a stack, and all five are locals here -- the parameter    */
/* block is the argument, and the other three were mirrored back only for as   */
/* long as the memory oracle hashed the bytes.                                 */
/*                                                                            */
/* The plots go through plot_at and plot_shape_at. Which one a call uses is    */
/* the point: the erase at $654C deliberately does *not* name a shape. It      */
/* reuses whatever s_snake.shape holds, and the mask that shape selects        */
/* decides which pixels get cleared. Tidying that away would change the        */
/* screen.                                                                     */
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
  return -d;
}

/// True if the cell at (\p col, \p row) is occupied, asked of the lo-res
/// occupancy map through the ROM's SCRN.
///
/// Charges nothing: the CMP that follows each SCRN is its own block in the
/// original and the cycles differ per call site, so each caller charges its
/// own. Folding them in here is what broke the first attempt -- 4 cycles
/// missing three times over, and every oracle diverged at once.
static bool cell_taken(uint8_t col, uint8_t row) {
  return rom_scrn(row, col) != CELL_EMPTY;
}

static void bouncer_step(Bouncer *b) {
  unsigned blocked = 0;

  if (b->row == 0) {
    return;
  }

  uint8_t want_col = b->col + b->dx;
  uint8_t want_row = b->row + b->dy;

  const bool diagonal_taken = cell_taken(want_col, want_row);
  if (diagonal_taken) {
    // Which axis actually stopped it? Ask the two cells either side.
    const bool across_taken = cell_taken(want_col, b->row);
    if (across_taken) {
      want_col = b->col;
      b->dx = reflect(b->dx);
      ++blocked;
    }

    const bool down_taken = cell_taken(b->col, want_row);
    if (down_taken) {
      want_row = b->row;
      b->dy = reflect(b->dy);
      ++blocked;
    }

    if (blocked == 0) {
      // An inside corner: only the diagonal is blocked, so go back the way
      // it came.
      want_col = b->col;
      want_row = b->row;
      b->dx = reflect(b->dx);
      b->dy = reflect(b->dy);
    }
  }

  // Erase where it was. Ink 0 is black, and the shape is whatever the caller
  // last left in $00 -- see the header.
  rom_setcol(INK_ERASE);

  plot_at(INK_ERASE, (Cell){.col = b->col, .row = b->row});

  rom_plot(b->row, b->col);

  s_snake.shape = SHAPE_BOUNCER;

  if (want_row == 0) {
    // Off the board: not redrawn, and the position is not committed.
    return;
  }

  b->row = want_row;
  b->col = want_col;
  rom_setcol(INK_GATE);

  plot_shape_at(SHAPE_BOUNCER, INK_GATE, (Cell){.col = b->col, .row = b->row});

  rom_plot(b->row, b->col);
}

/* --- The scoreboard ------------------------------------------------------- */
/*
 * Eleven fields, every one of them BCD, and every multi-byte one least
 * significant first. Six are what game_status_panel prints, and the labels it
 * prints beside them are where those six names come from -- see the $72CE
 * header for the layout.
 *
 * They are C variables now. The multi-byte ones stay arrays rather than
 * becoming a single integer, because the game's arithmetic on them is BCD and
 * byte-at-a-time: the carry from one half to the next is the original's own
 * `ADC #$00`, and folding a pair into one number would have to invent a BCD
 * meaning for the whole that the game never uses.
 *
 * Every initialiser is the byte the shipped image holds at that address, and
 * none of them is decoration: a new game seeds itself from s_progress.quota and
 * the status panel is drawn before anything writes s_progress.lives or s_progress.level, so
 * several of these are read before they are written.
 */

/// The score, and how far the player has got. The counters are BCD, because
/// the status panel prints them a nibble at a time.
static struct {
  /// SCORE, four bytes. game_add_score adds an apple's worth into it;
  /// game_promote_high_score copies it over the high score at the end of a
  /// game.
  uint8_t score[4];
  uint8_t best[4]; ///< HI SCORE, four bytes, the same shape
  uint8_t level;   ///< LEVEL, one BCD byte
  uint8_t lives;   ///< SNAKES LEFT. $7691 sets it to 2 for a new game
  uint8_t lives_at_level_start; ///< restored if the level is failed
  /// The bonus screen's own two-byte BCD scratch, and the lives count it
  /// compares against to decide whether the round earned a bonus at all.
  uint8_t bonus[2];
  /// VALUE -- what one apple is worth on this level, two BCD bytes.
  /// game_set_apple_value recomputes it per level.
  uint8_t apple_value[2];
  /// Apples on the playfield *right now* -- not, as this pair was commented
  /// before it was named, a countdown to the next one. game_place_apple ends
  /// by BCD-incrementing it ($766C) and $7743 decrements it when one is
  /// eaten; $77D0 places a replacement exactly when it reaches zero, which
  /// is why the field normally holds one apple and the timeout path's three
  /// arrive together. Measured on play-hires: it is $01 at every apple
  /// eaten, and $77D0 fires and places one every time.
  uint8_t afield[2];
  /// Apples eaten this round. Only the first $11 of them score -- $777B
  /// tests the high byte and $7780 compares the low against $11.
  uint8_t eaten[2];
  /// APPLES LEFT -- what remains of this round's quota. start_round loads
  /// it from quota and each apple eaten takes one off; both bytes zero is
  /// what ends the round.
  uint8_t left[2];
  /// The round's quota, copied into left by start_round. $10 for a fresh
  /// level, and the timeout path at $7817 adds three to both.
  uint8_t quota[2];
} s_progress = {
    .left = {0x10, 0x00},
    .lives = 0x02,
    .quota = {0x10, 0x00},
    .level = 0x01,
    .lives_at_level_start = 0x02,
};

/// What s_life_timer is loaded with when a life begins. Comes from s_level_time,
/// the level's own allowance.
///
/// $77BC sets it to $FF once the round's last apple is gone, which stops the
/// timer rather than lengthening it: s_life_timer counts down one per pace,
/// but game_draw_side_walls reads any value with bit 7 set as out of range,
/// clamps the wall height and writes $FF back ($6B55), so the count never
/// reaches zero and the run to the gate is untimed. Decoded from $641C and
/// $6B55, not observed -- all three recordings seed it $64 and never reach
/// the clamp.
static uint8_t s_life_time = 0x64;

/* --- The settings block: s_step_delay-s_level_time, plus s_snake.head_moved */
/*
 * Six bytes that outlive a life, set once at $376E and then kept up to date as
 * the game goes on. Everything else about a life is torn down and rebuilt.
 * game_cold_start writes all six before anything reads them, so unlike the
 * scoreboard these need no initialisers. Five of the six are the static
 * variables below; the sixth, head_moved, was grouped into s_snake instead.
 */

/// How long the pace loop dawdles between steps, i.e. the snake's speed.
/// $52 at startup and at the top of every round; $772E takes two off at the
/// start of each life and stops at 3, so the snake speeds up as lives are
/// lost and never goes faster than that.
static uint8_t s_step_delay;

/// 0..2, chosen at the setup prompt. Indexes the per-apple value table at
/// $71C8, and decides how many bouncers step per pass.
static uint8_t s_difficulty;

/// The game is playing itself, because nobody answered the difficulty
/// prompt before it timed out. Any input at all clears it, and while it is
/// set the death pause does not wait to be told to carry on.
static bool s_demo_mode;

/// Which of the 29 display lists at $8000 this level draws, 1-based.
/// select_script skips that many '*'-terminated scripts to find it.
static uint8_t s_script_index;

/// The level's time allowance, which seeds s_life_time at the start of every
/// life. Set by the display list's 'T' command (see run_script), and $64 until
/// one says otherwise. It is *not* the apple value, which is s_progress.apple_value and
/// is computed by game_set_apple_value from the difficulty and the level
/// number; that routine never reads this byte.
static uint8_t s_level_time;

/* --- The three zero-page pointers: $000A-$000F ---------------------------- */
/*
 * Pointer pairs in the original, low byte first, and 16-bit variables here.
 * Their byte-at-a-time increments -- bump the low byte, and if it wrapped bump
 * the high one -- are what a 6502 writes for ++p, so that is what they are;
 * the branch survives only because the two arms cost different cycles.
 *
 * The initialisers are what the machine holds at $3750, out of
 * entry-state-inc.h rather than out of the game image: this is zero page, and
 * the game never loaded over it.
 */

/// Into the current display list. game_next_byte reads through it and bumps
/// it; select_script points it at the right script first.
static uint16_t s_script_ptr = 0x994c;

/// Into the string that follows a JSR to game_print_inline_str -- which is
/// where the printer finds it, by reading the return address off the stack.
static uint16_t s_str_ptr = 0xffe1;

/// game_rand_byte's cursor. It is not a generator: it walks $1800-$1FFF and
/// returns the first byte it finds with bit 7 clear, so the "random" numbers
/// are the game's own level data, relocated there by the cold start.
/// game_setup_screen clamps the high byte into [$18,$1F) on the way in, which
/// is what keeps it inside that window.
static uint16_t s_rand_ptr = 0x6b00;

/// the setup screen has run once. The first time through it asks
/// nothing, takes difficulty 1 and demo mode, and only sets this. The shipped
/// image already has it set, so that first pass is skipped on a cold start and
/// the prompt appears immediately.
static bool s_setup_seen = true;

/* ========================================================================== */
/* the high score                                                    */
/*                                                                            */
/* Four BCD bytes in s_progress.score against four in s_progress.best, most    */
/* significant first. Below at any byte and it stops; above and it copies;     */
/* equal and it moves on.                                                      */
/*                                                                            */
/* The four compares are a loop here, over the byte indices. In game.c that    */
/* would be a bug -- the site list is built by grepping for literal CYCLES     */
/* addresses, and a computed one silently leaves the trace. This file is       */
/* deliberately outside that grep, so the constraint does not apply and the    */
/* code can be shaped by what it means instead.                                */
/* ========================================================================== */

/// Most significant first, which is the order the compare runs in.
static const uint8_t kMsbFirst[4] = {3, 2, 1, 0};

static void game_promote_high_score(void) {
  bool beats_it = true;

  for (unsigned i = 0; i < 4; ++i) {
    const uint8_t mine = s_progress.score[kMsbFirst[i]];
    const uint8_t best = s_progress.best[kMsbFirst[i]];
    if (mine < best) {
      beats_it = false;
      break;
    }
    if (mine != best) {
      break;
    }
    // Equal: fall through to the next byte. All four equal reaches the copy
    // below, which is a harmless self-assignment.
  }

  if (beats_it) {
    for (unsigned i = 0; i < 4; ++i)
      s_progress.best[i] = s_progress.score[i];
  }
}

/* ========================================================================== */
/* find an apple                                                     */
/*                                                                            */
/* Sweep whole columns looking for $0F on the occupancy map: from the snake's  */
/* own column leftwards, then from it again rightwards. First hit wins, so the */
/* result leans left. Nothing found parks the answer at row 0, column $14.     */
/*                                                                            */
/* $6B39/$6B3A were the cursor and s_apple the answer. The cursor is a         */
/* pair of locals here: nothing ever read the two bytes back, so once the      */
/* storage left RAM the stores that mirrored them were dead code.              */
/* ========================================================================== */

/// The lo-res occupancy map's value at \p c. $0F is an apple.
static uint8_t cell_at(Cell c) {
  return rom_scrn(c.row, c.col);
}

static void game_find_nearest_apple(void) {
  static const uint8_t kLastRow = 0x27;

  Cell c = {.col = s_snake.head.col, .row = 1};
  bool found = false;

  for (;;) { // leftwards
    const uint8_t v = cell_at(c);
    if (v == CELL_APPLE) {
      found = true;
      break;
    }
    if (++c.row != kLastRow) {
      continue;
    }
    c.row = 1;
    if (--c.col == 0)
      break;
  }

  if (!found) {
    c.col = s_snake.head.col;

    for (;;) { // rightwards
      const uint8_t v = cell_at(c);
      if (v == CELL_APPLE) {
        break;
      }
      if (++c.row != kLastRow) {
        continue;
      }
      c.row = 1;
      if (++c.col == kLastRow) {
        c.row = 0;
        c.col = 0x14;
        break;
      }
    }
  }

  s_apple = c;
}

/* ========================================================================== */
/* is a move safe?                                                   */
/*                                                                            */
/* The four neighbour probes are the same shape four times over, so they are  */
/* a table here. In the original they are 60 bytes of straight-line code with */
/* the offsets and the block addresses interleaved.                           */
/*                                                                            */
/* ========================================================================== */

/// Where each neighbour is, and the four block addresses the original spends
/// on it: the SCRN block, the CMP after it, the not-taken edge, and the
/// increment.
static const struct {
  int8_t dcol, drow;
} kNeighbour[4] = {
    {+1, 0},
    {-1, 0},
    {0, +1},
    {0, -1},
};

static MoveVerdict snake_move_verdict(uint8_t dir, uint8_t *cell_out) {
  // The head plus this direction's deltas.
  const Cell target = {
      .col = kColDelta[dir] + s_snake.head.col,
      .row = kRowDelta[dir] + s_snake.head.row,
  };

  const uint8_t cell = cell_at(target);
  *cell_out = cell;

  // Empty or an apple, and nothing else, may be stepped into.
  if (cell != CELL_EMPTY && cell != CELL_APPLE) {
    return MOVE_TARGET_TAKEN;
  }

  if (target.row == 0) {
    // Row 0 is the top border; there is nothing above it to look at.
    return MOVE_ROW_ZERO;
  }

  // A target whose four neighbours are all occupied is a dead end: legal to
  // enter, fatal on the move after, so it is refused a step early.
  unsigned free_neighbours = 0;
  for (unsigned i = 0; i < 4; ++i) {
    const Cell n = {
        .col = target.col + kNeighbour[i].dcol,
        .row = target.row + kNeighbour[i].drow,
    };

    if (cell_at(n) == CELL_EMPTY) {
      ++free_neighbours;
    }
  }

  if (free_neighbours) {
    return MOVE_OK;
  }
  return MOVE_DEAD_END;
}

/* ========================================================================== */
/* draw the playfield                                                */
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
  OP_HLINE = 0x48, ///< 'H' -- ink, column, last column, row
  OP_PLOT = 0x50, ///< 'P' -- ink, column, row. No script uses it.
  OP_STORE = 0x54, ///< 'T' -- one byte, into $0304
  OP_VLINE = 0x56, ///< 'V' -- ink, row, last row, column
  OP_END = 0x2a, ///< '*' -- end of this level's script
} ScriptOp;

/// the lo-res plot colour. Zero erases, anything else draws.
static void set_ink(uint8_t ink) {
  game_set_ink(ink);
}

/// The ROM's HLINE, which takes its right-hand end from $2C.
static void lores_hline(uint8_t row, uint8_t from_col) {
  rom_hline(row, from_col);
}

/// Plot at a cell in a given ink, keeping whatever shape s_snake.shape holds.
///
/// Separate from plot_shape_at because for these callers the shape genuinely
/// is inherited -- see s_snake.shape -- and passing one would be inventing a value
/// the original does not have.
static void plot_at(uint8_t ink, Cell c) {
  game_plot_shape(ink, c);
}

/// The three runs, with their ink and both endpoints as arguments instead of
/// as assignments before the call. What they buy is that a caller states the
/// whole run in one place, including the two things it used to inherit from
/// whatever ran before it.
static void plot_hline_at(uint8_t ink, uint8_t col, uint8_t row, uint8_t to_col) {
  game_plot_hline(ink, (Cell){.col = col, .row = row}, to_col);
}

static void plot_vline_at(uint8_t ink, uint8_t col, uint8_t row, uint8_t to_row) {
  game_plot_vline(ink, (Cell){.col = col, .row = row}, to_row);
}

static void lores_vline_at(uint8_t col, uint8_t row, uint8_t to_row) {
  // The row it restores was this routine's result in A. Nothing reads it; the
  // adapter that did has gone.
  game_lores_vline((Cell){.col = col, .row = row}, to_row);
}

/// The ROM's PLOT.
static void lores_plot(uint8_t row, uint8_t col) {
  rom_plot(row, col);
}

/// $7019's name for game_next_byte: the next display-list byte.
static uint8_t script_byte(void) {
  return game_next_byte();
}

/// Graphics, hi-res, page 2, full screen. The reads are the writes.
static void select_hires_page2(void) {
  s_sound.period = 0x00;
  io_peek(0xc050);
  io_peek(0xc057);
  io_peek(0xc055);
  io_peek(0xc052);
}

/// A plain three-deep delay, all three counters as arguments.
///
/// \p inner is the innermost, and the original does not initialise it -- it
/// counts down from whatever Y happened to hold. That is not a value the code
/// can state, so it was measured instead: game_clear_hgr's page loop is
/// what runs immediately before, and it exits with Y wrapped to zero. Asserted
/// over full runs of both scenarios, where it is zero every time. It is
/// therefore passed as zero, which counts a full 256.
///
/// The other two were the plotter's column and row, borrowed because zero page
/// was the only place to put them; $7056 loaded them and $7061 counted them
/// down. Nothing reads what the loop leaves behind: wipe_occupancy_map
/// overwrites the row on its first line, open_wall_gaps touches neither, and
/// the column is not read until draw_border writes it.
static void spin(uint8_t inner, uint8_t middle, uint8_t outer) {
  for (;;) {
    // The 1.29 s hold on the cleared screen, and nothing else happens in it.
    // Three nested counters, so this innermost charge is 99.6% of the pause.
    advance(5);
    if (--inner) {
      continue;
    }
    middle = middle - 1;
    if (middle) {
      continue;
    }
    outer = outer - 1;
    if (!outer)
      break;
  }
}

/// Clear the lo-res occupancy map, one full-width row at a time from the
/// bottom up. Ink 0 is black, so this erases.
static void wipe_occupancy_map(void) {
  uint8_t at = 0x27;
  set_ink(INK_ERASE);

  for (;;) {
    s_mon.h2 = 0x27;
    lores_hline(at, 0x00);

    const uint8_t row = at - 1;
    at = row;
    // BPL: row 0 is drawn, and the loop ends one step later.
    if (row & 0x80)
      break;
  }
}

/// One gap per bouncer, which is what the difficulty counts.
static void open_wall_gaps(void) {
  const uint8_t difficulty = s_difficulty;
  if (!difficulty) {
    return;
  }

  lores_plot(0x01, 0x01);

  if (s_difficulty == 0x01) {
    return;
  }
  lores_plot(0x01, 0x26);
}

/// The border, in both representations, plus the gap the snake leaves through.
///
/// \p ink is the six sides' colour, which the original inherits from
/// game_draw_playfield rather than setting.
static void draw_border(uint8_t ink) {
  lores_hline(0x00, 0x00); // $2C is still $27 from the wipe

  lores_hline(0x27, 0x00);

  // The four sides, twice: once on the lo-res occupancy map and once in
  // hi-res. Four of these seven used to leave the endpoint out and inherit
  // $27 from the call above; it is written at each of them now.
  lores_vline_at(0x00, 0x00, 0x27);
  lores_vline_at(0x27, 0x00, 0x27);
  plot_hline_at(ink, 0x00, 0x00, 0x27);
  plot_hline_at(ink, 0x00, 0x27, 0x27);
  plot_vline_at(ink, 0x00, 0x00, 0x27);
  plot_vline_at(ink, 0x27, 0x00, 0x27);

  // Ink 3 over columns $12-$16 of the bottom row, on top of the border just
  // laid down: the gap the snake leaves through.
  plot_hline_at(INK_GATE, 0x12, 0x27, 0x16);
}

/// Walk the pointer to the current level's script, skipping one whole script
/// per level below it. DEX first, so level 1 skips nothing.
static void seek_script(void) {
  uint8_t left = s_script_index;
  s_script_ptr = 0x8000;

  for (;;) {
    if (!--left) {
      return;
    }
    for (;;) {
      const uint8_t b = script_byte();
      if (b == OP_END) {
        break;
      }
    }
  }
}

static void game_draw_playfield(void) {
  game_clear_hgr();
  select_hires_page2();
  spin(0x00, 0x00, 0x04); // the counts $7056 used to store into $02/$03
  wipe_occupancy_map();

  s_mon.wndtop = 0x14;
  s_snake.shape = SHAPE_SOLID;
  set_ink(INK_WALL_BOTTOM);

  open_wall_gaps();
  draw_border(INK_WALL_BOTTOM);

restart:
  seek_script();

  for (;;) {
    const uint8_t op = script_byte();

    if (op == OP_RESTART) {
      s_script_index = 0x01;
      goto restart;
    }

    if (op == OP_HLINE) {
      const uint8_t ink = script_byte();
      const uint8_t col = script_byte();
      const uint8_t last = script_byte();
      const uint8_t row = script_byte();
      set_ink(ink);

      s_mon.h2 = last;
      lores_hline(row, col);

      plot_hline_at(ink, col, row, last);
      continue;
    }

    if (op == OP_VLINE) {
      const uint8_t ink = script_byte();
      const uint8_t row = script_byte();
      const uint8_t last = script_byte();
      const uint8_t col = script_byte();
      set_ink(ink);

      // The lo-res half puts $03 back where it found it, which is what let
      // the original's hi-res half run the same span without restating it.
      // Both spans are stated here.
      lores_vline_at(col, row, last);
      plot_vline_at(ink, col, row, last);
      continue;
    }

    if (op == OP_PLOT) {
      const uint8_t ink = script_byte();
      const uint8_t col = script_byte();
      const uint8_t row = script_byte();
      set_ink(ink);

      lores_plot(row, col);
      game_plot_shape(ink, (Cell){.col = col, .row = row});
      continue;
    }

    if (op == OP_STORE) {
      const uint8_t v = script_byte();
      s_level_time = v;
      continue;
    }

    if (op == OP_END) {
      return;
    }
    // Anything unrecognised is skipped. No script contains one.
  }
}

/* ========================================================================== */
/* the hi-res plotter                                  */
/*                                                                            */
/* Hi-res page 1 is a grid of 48 rows, four scanlines tall and one byte wide.  */
/* $6000/$6030 hold each row's base address split into low and high halves,    */
/* and successive scanlines are $400 apart -- which is why walking down a cell */
/* is +4 on the high byte and nothing else.                                    */
/*                                                                            */
/* $04/$05 (the destination pointer), $06 (the pattern index) and $07 (the     */
/* scanline counter) are locals.                                              */
/* ========================================================================== */

/// The address of a cell row's first scanline, from the split table.
static uint16_t cell_row_base(uint8_t row) {
  return kHgrLineBase[row];
}

/// Index into the 128-byte dot table at $6064: 16 inks of 8, four column
/// phases in each of two scanline parities.
static uint8_t dot_index(uint8_t ink, uint8_t scanline, uint8_t col) {
  return (((ink << 1) | (scanline & 1)) << 2) | (col & 3);
}

/// A byte of hi-res page 1. The page is $2000-$3FFF, and successive scanlines
/// of a row are $400 apart, which is why walking down a cell is +4 on the high
/// byte and nothing else.
static void hgr_poke(uint16_t addr, uint8_t v) {
  poke(addr, v);
}
static uint8_t hgr_peek(uint16_t addr) {
  return peek(addr);
}

/// draw the loaded shape into one cell, replacing what was there.
static void game_draw_cell(uint8_t ink, Cell c) {
  uint16_t dest = cell_row_base(c.row);

  for (unsigned line = 0; line < 4; ++line) {
    // Built in $06 in two steps, and written out between them because it is
    // zero page and a probe may sample there.
    const uint8_t idx = dot_index(ink, line, c.col);

    hgr_poke(dest + c.col, kHgrPattern[idx] & s_snake.shape_mask[line]);
    dest += 0x0400; // one scanline down, i.e. +4 on the high byte
  }
  // The carry is what the loop's CPX #4 leaves.
}

/// the same cell, merged instead of replaced: only bits are set, and
/// the pattern is inverted first. $7F and not $FF because bit 7 is the byte's
/// hi-res palette bit and flipping it would shift the whole byte's colour.
///
/// It also builds the index differently -- ROR where $60F7 has ROL, so the
/// scanline parity lands in bit 7 and the two following ASLs shift it out.
/// The index degenerates to (ink >> 1) * 4 + (col & 3). Changing it to match
/// $60F7 fails the screen check, so whatever the author meant, it is load
/// bearing.
static void game_merge_cell(uint8_t ink, Cell c) {
  uint16_t dest = cell_row_base(c.row);

  for (unsigned line = 0; line < 4; ++line) {
    const uint8_t parity = line & 1;
    const uint8_t idx = (((parity << 7) | (ink >> 1)) << 2) | (c.col & 3);

    const uint16_t at = dest + c.col;
    hgr_poke(at, ((kHgrPattern[idx] ^ 0x7f) & s_snake.shape_mask[line]) | hgr_peek(at));
    dest += 0x0400;
  }
}

/// zero hi-res page 1, $2000 through $3FFF. The inner loop runs a
/// full 256 bytes because Y wraps, so the terminating test is on the page.
///
/// It costs no time at all now. On a 6502 this was 106,897 cycles -- 105 ms of
/// hi-res page wiping downward -- and the design gave that up deliberately:
/// the fill is not worth preserving, and $7056 follows immediately with a
/// 1.29 s hold that still is, so the pair still reads as one pause.
static void game_clear_hgr(void) {
  for (uint8_t page = 0x20;;) {
    uint8_t y = 0;
    do {
      hgr_poke((page << 8) + y, 0x00);
      ++y;
    } while (y);

    ++page;
    if (page == 0x40)
      break;
  }
}

/* ========================================================================== */
/* runs of cells                                       */
/*                                                                            */
/* Each loads the shape once and repeats a draw along one axis until the       */
/* moving coordinate reaches $08. The end is tested after drawing, so it is    */
/* inclusive and a degenerate run still plots one cell. A start past the end   */
/* wraps through 255; nothing guards against it and nothing needs to.          */
/* ========================================================================== */

/// a horizontal run of hi-res cells, from \p c along row c.row to
/// \p to_col. The original walked $02/$03 and left $02 on the endpoint, which
/// its caller read out of A; nothing reads it here, so nothing returns it.
static void game_plot_hline(uint8_t ink, Cell c, uint8_t to_col) {
  // Loads the four scanline masks the cell drawers read. The mask it
  // returns was the original\'s result in A and nothing reads it.
  game_load_shape_masks(s_snake.shape);
  for (;;) {
    game_draw_cell(ink, c);

    if (c.col == to_col)
      break;

    c.col = c.col + 1;
  }
}

/// the same down a column: rows $03 through $08 in column $02.
static void game_plot_vline(uint8_t ink, Cell c, uint8_t to_row) {
  // Loads the four scanline masks the cell drawers read. The mask it
  // returns was the original\'s result in A and nothing reads it.
  game_load_shape_masks(s_snake.shape);
  for (;;) {
    game_draw_cell(ink, c);

    if (c.row == to_row)
      break;

    c.row = c.row + 1;
  }
}

/// the lo-res half of a vertical run. Unlike the hi-res one it puts
/// $03 back where it found it, because the caller draws the hi-res run over
/// the same coordinates next.
static void game_lores_vline(Cell c, uint8_t to_row) {
  // The original saves the starting row on the stack, because the hi-res half
  // of a display list's 'V' runs the same span next and the loop below walks
  // c.row to the end of it. Every caller states both ends now.

  for (;;) {
    lores_plot(c.row, c.col);

    if (c.row == to_row)
      break;

    c.row = c.row + 1;
  }
}

/* ========================================================================== */
/* step the bouncers, then take a key                                */
/* ========================================================================== */

/// Move one bouncer. The original copies it into the parameter block at
/// $6633-$6636, calls $64C8, and copies it back out; those eight ram_pokes
/// were what a struct copy looks like without structs, and the block is the
/// argument now.
static void step_bouncer_slot(int slot) {
  Bouncer b = s_bouncers[slot];
  bouncer_step(&b);
  s_bouncers[slot] = b;
}

/// take the next key out of the ring buffer game_read_key fills.
/// Only a byte with bit 7 set counts; the slot is cleared and the read index
/// advances. Returns what the original leaves in A.
///
/// $6216 is the RTS all paths share, this one's and game_read_key's. While
/// only one of the two was converted it had to stay a probe site, because
/// converting one of a shared block's two paths while the other still reports
/// is exactly how the two engines stop agreeing. Both are C now. The
/// trace caught it; the pinned site count could not, because the count was
/// right.
static uint8_t dequeue_key(void) {
  const uint8_t at = s_input.read;
  const uint8_t key = s_input.ring[at];
  if (!(key & 0x80)) {
    return key;
  }

  s_input.ring[at] = 0x00;
  s_input.read = (at + 1) & 0x0f;
  // X *is* live out of $6594 -- `apple2tc --ir` says so -- unlike X out of
  // $6C72, where the same check let the write go. So it is maintained.
  return key;
}

/// step as many bouncers as the difficulty calls for, then fall into
/// the key dequeue whose byte is the return value.
static uint8_t game_step_bouncers(void) {
  const uint8_t difficulty = s_difficulty;

  if (!difficulty) {
    return dequeue_key();
  }

  step_bouncer_slot(0);

  if (s_difficulty == 0x01) {
    return dequeue_key();
  }

  step_bouncer_slot(1);
  return dequeue_key();
}

/* ========================================================================== */
/* turn the next input into a direction                              */
/* ========================================================================== */

/// The six inputs the game understands. $6C63 holds the key that produces
/// each one and $6C6A the code the game acts on; the two start out identical,
/// which is why the substitution is invisible until the player rebinds a key
/// from the title screen. Both tables are part of the loaded image, and the
/// first is written at $757C, from the redefinition screen.
///
/// Confusing the two is invisible until the player rebinds a key: with the
/// default bindings both tables hold identical bytes, so reading the wrong one
/// looks exactly right. It stayed wrong for a while for that reason, and only
/// a test that rebinds and *then* plays can tell them apart.
enum { kInputCount = 6 };

static uint8_t input_key(int i) {
  return s_input.bindings[i];
}

static uint8_t input_code(int i) {
  return kKeyDefaults[i];
}

/// The two codes that are settings rather than directions.
enum { kCodeJoystickOn = 0x80, kCodeJoystickOff = 0x8b };

/// The code every input collapses to in attract mode. Its caller stores $FF
/// in s_life_outcome, which ends the game in progress.
enum { kCodeStop = 0x92 };

/// attract mode: nobody answered the difficulty prompt before it
/// timed out, so the game is playing itself. Any input at all ends it, which
/// is why the whole key table is skipped below.
static bool attract_mode(void) {
  return s_demo_mode;
}

/// A switch input. Every one of the game's three read sites -- here, the
/// difficulty prompt at $7428, and the pause check at $788B -- takes bit 7
/// *clear* to mean active, which is the opposite of a stock Apple II paddle
/// button. Consistent enough to be the convention of whatever switch box the
/// game was written for, so it is transcribed and not corrected.
static bool switch_pressed(uint16_t sw) {
  return !(io_peek(sw) & 0x80);
}

static uint8_t game_read_direction(uint8_t key) {
  if (attract_mode()) {
    if (s_input.joystick) {
      if (switch_pressed(0xc061)) {
        return kCodeStop;
      }
    }

    if (key & 0x80) {
      return kCodeStop;
    }
    // Not a keypress, so nothing happened -- $00 out of an empty ring.
    return key;
  }

  // Search the bindings from the last slot down, so that if the player has
  // bound the same key twice the higher slot wins.
  uint8_t code = key;
  int slot = kInputCount - 1;
  for (;;) {
    if (key == input_key(slot)) {
      code = input_code(slot);
      break;
    }

    if (--slot < 0) {
      break;
    }
  }
  // X is not written back. The original leaves it on the matching slot, or
  // $FF, but nothing reads it: `apple2tc --ir` prints per-function register
  // liveness, and func_6c72's LiveOut is A, Y and the flags. Y is in that set,
  // so the joystick block below does maintain it.

  if (code == kCodeJoystickOn) {
    s_input.joystick = true;
    return 0x01;
  }

  if (code == kCodeJoystickOff) {
    s_input.joystick = false;
    return 0x00;
  }

  if (code & 0x80) {
    // A direction. Hand it straight back.
    return code;
  }

  const bool joystick = s_input.joystick;
  if (!joystick) {
    return code;
  }

  // The joystick is two switch inputs read twice, with annunciator 2
  // selecting the pair -- four directions on two pins. Exactly one has to be
  // active: none or several is ambiguous and rejected.
  int pressed = 0;
  uint8_t chosen = 0; // only read when exactly one input turned out active
  io_peek(0xc05b); // annunciator 2 on
  if (switch_pressed(0xc062)) {
    chosen = 0;
    ++pressed;
  }

  if (switch_pressed(0xc063)) {
    chosen = 3;
    ++pressed;
  }

  io_peek(0xc05a); // annunciator 2 off
  if (switch_pressed(0xc062)) {
    chosen = 1;
    ++pressed;
  }

  if (switch_pressed(0xc063)) {
    chosen = 2;
    ++pressed;
  }
  // Y, unlike X, is live out of here -- see above.

  if (pressed != 1) {
    return 0x00;
  }
  return input_code(chosen);
}

/* ========================================================================== */
/* read a replacement key for one slot                               */
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
  return kArrowCH[slot];
}

static uint8_t slot_row(int slot) {
  return kArrowCV[slot];
}

static uint8_t slot_glyph(int slot) {
  return kArrowGlyph[slot];
}

/// the dark half: erase the glyph and wait, polling nothing.
static void edit_key_blank(uint8_t slot) {
  // The original parks the slot at $0002 for the whole routine, because COUT
  // clobbers X and every step below needs it again. Here it is the parameter,
  // and the glyph blitter no longer has anything to clobber it with.
  s_mon.ch = slot_col(slot);
  s_mon.cv = slot_row(slot);
  rom_fc68();

  rom_cout(0xa0);

  // Both halves of the blink count X down to zero 256 times, and the X they
  // start from is whatever COUT left behind -- the original never initialises
  // it. What COUT leaves is what its caller put there, which is the slot: the
  // ROM's COUT1 does not touch X at all, and the hi-res hook saves and
  // restores it. So the first pass of the delay is `slot` long and the other
  // 255 are a full 256. Transcribed rather than tidied: it is the delay's
  // actual duration.
  uint8_t x = slot;
  uint8_t y = 0;
  for (;;) {
    // The dark half of the redefinition cursor's blink. Unlike the lit half
    // this one polls nothing, so it cannot hang -- but delete it and the
    // cursor stops blinking, which is the visible half of the pair.
    advance(5);
    if (--x) {
      continue;
    }
    // $75EC is `LDA #$41 / BEQ`, a branch that cannot be taken and a value
    // nothing reads. Four cycles of the delay and nothing else.
    if (!--y)
      break;
  }
}

/// the lit half: draw the glyph and wait, reading the keyboard each
/// time the inner counter wraps. Returns the accepted key, or 0 if the wait
/// ran out or the key was rejected -- either way the blink starts again, and
/// no acceptable key is 0.
static uint8_t edit_key_prompt(uint8_t slot) {
  s_mon.ch = slot_col(slot);
  s_mon.cv = slot_row(slot);
  rom_fc68();

  const uint8_t glyph = slot_glyph(slot);
  rom_cout(glyph);

  // The slot again -- see edit_key_blank for why COUT leaves it in X.
  uint8_t x = slot;
  uint8_t y = 0;
  for (;;) {
    // The redefinition cursor's blink, and the loop that waits for the key to
    // bind. Both halves matter: the rate is visible and the poll is the yield.
    advance(5);
    if (--x) {
      continue;
    }

    GAME_CYCLES_COORD(0x760f, 6);
    const uint8_t key = io_peek(0xc000);
    if (key & 0x80) {
      io_poke(0xc010, key); // clear the strobe

      // Anything from $A1 up -- every printable key -- plus the two arrows.
      // The carry the three compares leave is not written back: the caller's
      // next act on it is $7582's `CPX #$06`, which sets it.
      if (key >= 0xa1) {
        return key;
      }
      if (key == 0x88) { // left arrow
        return key;
      }
      if (key == 0x95) { // right arrow
        return key;
      }
      return 0;
    }

    if (--y) {
      continue;
    }
    return 0;
  }
}

static uint8_t game_edit_key(uint8_t slot) {
  uint8_t key;
  do {
    edit_key_blank(slot);
    key = edit_key_prompt(slot);
  } while (!key);

  return key;
}

/* ========================================================================== */
/* the falling tone                                                  */
/* ========================================================================== */

/* The whole tone is four bytes, declared up with the rest of the state.       */
/* s_sound.period doubles as the on/off switch: game_mark_head raises it to 1   */
/* when the head moves and game_draw_playfield clears it, so the sound follows */
/* the snake and stops with it. s_sound.port is where the click goes, as the   */
/* low byte of the soft switch: $C030 is the speaker and $C020 the cassette    */
/* output, which nobody can hear -- so muting is a store rather than a branch, */
/* and the click itself is one indexed read. See the $7642 header for why that */
/* shape was chosen.                                                          */

static void game_tick_sound(void) {
  s_sound.passes = 0x14; // twenty

  for (;;) {
    // One pass of the tone loop. The pitch is the number of passes between
    // clicks -- s_sound.countdown of them -- so a pass's duration *is* the
    // note, and this is the charge that sets it.
    //
    // Three numbers rather than one, because the three kinds of pass really
    // do take different times on a 6502 and the difference is audible. This
    // is the bare pass, with nothing to count and nothing to say.
    advance(38);
    const uint8_t period = s_sound.period;
    if (period) {
      if (period < 0x80) {
        // Twelve more when there is a countdown to advance, which is every
        // pass of an actual note: 50 against 38 is most of a semitone.
        advance(12);
        const uint8_t left = s_sound.countdown - 1;
        s_sound.countdown = left;
        if (!left) {
          // The click, and the cycles between it and the previous one are
          // the half-period of the note.
          advance(59);
          const uint8_t port = s_sound.port;
          speaker_access(port);

          // Two INC s_sound.period: every click lengthens the period, so the pitch
          // falls for as long as the head keeps moving.
          s_sound.period = s_sound.period + 2;
          s_sound.countdown = s_sound.period;
        }
      }
    }

    if (s_sound.period >= 0x80) {
      // Fallen off the bottom of the range: silence until something restarts
      // it. $80 is reached from below in steps of two, so this is the end of
      // one slide rather than a wrap.
      s_sound.period = 0x00;
    }

    // Chosen afresh every pass, and defaulting to inaudible. Picking a real
    // port costs more than falling through to silence, and while the game is
    // audible that difference is in every pass -- so it is part of the pitch
    // too, and the note is flatter when you can hear it than when you cannot.
    s_sound.port = 0x20;
    if (!attract_mode()) {
      advance(6);
      if (!s_sound.muted) {
        advance(5);
        s_sound.port = 0x30;
      }
    }

    const uint8_t left = s_sound.passes - 1;
    s_sound.passes = left;
    if (!left)
      break;
  }
}

/* ========================================================================== */
/* a BCD byte, with leading zeros suppressed                         */
/* ========================================================================== */

/// the significance flag, holding the last significant digit rather
/// than a plain 1. Its caller clears it before the first byte of a number, so
/// leading zeros print nothing and interior ones print; $7226 consults it
/// after the last byte, and prints a single "0" if the whole number was.
static bool digit_seen(void) {
  return s_mon.h2 != 0;
}

static void note_digit(uint8_t digit) {
  s_mon.h2 = digit;
}

enum { kCharZero = 0xb0 };

/// `CLC / ADC #$B0`, which is what turns 0-9 into the character for it.
///
/// The original's ADC honours the D flag; this addition does not. Digits are
/// printed with D clear -- $7267 is the only thing in the game that sets it,
/// and it clears it again before returning -- so say so loudly rather than
/// carry a decimal path that cannot be reached.
static void cout_digit(uint8_t digit) {
  rom_cout(kCharZero + digit);
}

static void game_print_bcd(uint8_t byte) {
  const uint8_t high = byte >> 4;

  if (high) {
    note_digit(high);
  }

  if (digit_seen()) {
    cout_digit(high);
  } else {
    // A leading zero: dropped, and nothing is printed.
  }

  const uint8_t low = byte & 0x0f;

  if (low) {
    note_digit(low);
  }

  if (digit_seen()) {
    cout_digit(low);
  }
}

/// called after the last byte of a number: if nothing significant was
/// printed, the number was zero, and one "0" is printed for the whole of it.
static void game_print_zero_if_blank(void) {
  if (digit_seen()) {
    return;
  }

  rom_cout(kCharZero); // JMP $FDED -- a tail call, so no return address.
}

/* ========================================================================== */
/* add to the score                                                  */
/* ========================================================================== */

static void game_add_score(void) {
  // Decimal mode for the whole run, and adc_dec16 rather than a second
  // hand-written BCD adder: it is the one the emulator and the generated code
  // both use, so it cannot disagree with them about the undefined corners of
  // BCD ADC. It returns the sum in the low byte and the flags in the high one.

  // Four bytes in s_progress.score, least significant first, plus a two-byte value at
  // s_progress.apple_value. The original adds the value into the low half and then propagates
  // the carry through the top half with `ADC #$00`, which flips the operand
  // order halfway -- kept, because adc_dec16 need not be symmetric over BCD
  // that is not valid BCD.
  unsigned carry = 0;
  uint8_t flags = 0;
  for (int i = 0; i < 4; ++i) {
    const uint8_t a = i < 2 ? s_progress.apple_value[i] : s_progress.score[i];
    const uint8_t m = i < 2 ? s_progress.score[i] : 0x00;
    const uint16_t r = adc_dec16(a, m, carry);
    s_progress.score[i] = r;
    flags = r >> 8;
    carry = flags & 0x01;
  }

  // The carry between bytes is barely covered: only the long fixture run ever
  // scores enough to cross a byte boundary, so breaking the propagation here
  // looks correct to almost everything.
  //
  // D is the only flag live out of $7267 that survives here, and the
  // original's CLD is what makes it false. `apple2tc --ir` also called C and V
  // live out; that was true of the generated program, whose caller read them.
  // Nothing does now.
}

/* ========================================================================== */
/* two small ones                                             */
/* ========================================================================== */

/// tell the ROM's lo-res plotter which colour to draw the occupancy
/// map in. Zero erases, anything else draws, so the map only ever holds colour
/// 0 or colour 5: it is a two-state map, not a picture.
///
/// The original takes its argument in the Z flag rather than in A, because
/// every caller reaches it with `LDA $01 / JSR $7024` and $01 is the same ink
/// byte the hi-res plotter takes. Here it is simply the byte.
static void game_set_ink(uint8_t ink) {
  rom_setcol(ink ? INK_OCCUPIED : INK_ERASE); // JMP $F864 -- a tail call.
}

/// read the byte the $000A pointer addresses and advance it. The
/// display-list interpreter's only way of reading its script.
static uint8_t game_next_byte(void) {
  const uint8_t b = peek(s_script_ptr);

  ++s_script_ptr;
  if (!(s_script_ptr & 0xff)) {
    // The low byte wrapped, so the original had to bump the high one too.
  }
  return b;
}

/* ========================================================================== */
/* apples                                              */
/* ========================================================================== */

/// the game's random number. A pointer at $000E walks memory from
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
static uint8_t game_rand_byte(void) {
  ++s_rand_ptr;

  for (;;) {
    const uint8_t b = peek(s_rand_ptr);
    if (!(b & 0x80)) {
      return b;
    }

    s_rand_ptr = 0x1800;
  }
}

/// Add one, in BCD, to the two-byte counter at \p at.
static void bcd_inc16(uint8_t at[2]) {
  uint16_t r = adc_dec16(at[0], 0x01, 0x00);
  at[0] = r;

  r = adc_dec16(at[1], 0x00, (r >> 8) & 0x01);
  at[1] = r;
}

static Cell game_place_apple(void) {
  // Rejection sampling: two pseudo-random bytes as column and row, ask the
  // lo-res map whether that cell is free, and start over if it is not.
  // game_rand_byte returns $00-$7F while the field is 40x40, so most draws
  // land outside it and hit the border or garbage -- the retry loop does far
  // more work than it looks like.
  Cell at = {0, 0};
  for (;;) {
    at.col = game_rand_byte();

    at.row = game_rand_byte();

    const bool taken = cell_taken(at.col, at.row);

    if (!taken)
      break;
  }

  // White on the occupancy map, so the snake's collision test sees it.
  rom_setcol(INK_WHITE);

  rom_plot(at.row, at.col);

  plot_shape_at(SHAPE_APPLE, INK_APPLE, at);

  // One more apple on screen. $77D0 watches this pair and calls back here when
  // it reaches zero.
  bcd_inc16(s_progress.afield);
  return at;
}

/// what one apple is worth: the difficulty's entry in the $71C8 table
/// added to itself once per level, in BCD, into s_progress.apple_value. X is never touched in
/// the original's loop, which is what makes it the same entry every time.
static void game_set_apple_value(void) {
  s_progress.apple_value[0] = 0x00;
  s_progress.apple_value[1] = 0x00;
  const uint8_t per_apple = kAppleValueTable[s_difficulty];
  uint8_t levels = s_script_index;

  for (;;) {
    uint16_t r = adc_dec16(per_apple, s_progress.apple_value[0], 0x00);
    s_progress.apple_value[0] = r;

    r = adc_dec16(s_progress.apple_value[1], 0x00, (r >> 8) & 0x01);
    s_progress.apple_value[1] = r;

    if (!--levels)
      break;
  }

  // D is the whole of this routine's live-out set now that V has gone.
}

/* ========================================================================== */
/* the head and the apple it eats               */
/* ========================================================================== */

/// mark the head on the lo-res occupancy map, at the row and column
/// the caller has already loaded, and raise the two flags that say it is
/// there: s_snake.head_moved for the next draw and s_sound.period to start the tone.
static void game_mark_head(uint8_t row, uint8_t col) {
  rom_plot(row, col);

  s_snake.head_moved = true;
  s_sound.period = 0x01;

  // A and its flags are live out of $6BEF, unlike almost everything else here.
}

/// draw the cell the caller set up, and if s_snake.head_moved says the head is on
/// it, merge shape 1 over the top so the head reads as a head rather than
/// replacing the body cell underneath. s_snake.head_moved is consumed here.
static void game_draw_head(uint8_t ink, Cell c) {
  game_plot_shape(ink, c);

  if (s_snake.head_moved) {
    s_snake.shape = SHAPE_APPLE;
    { // was game_plot_shape_merge()

      game_load_shape_masks(s_snake.shape);

      // The high byte it returns was the original's result in A; nothing
      // reads it now.
      game_merge_cell(ink, c);
    }
  }

  s_snake.head_moved = false;
  // Only V and D are live out, and neither is touched here.
}

/// an extra snake for clearing the round, and the noise that says so.
///
/// Not "eat an apple", which is what this was called: it BCD-increments $725E,
/// which the status panel prints as SNAKES LEFT, and its one caller is $7803 --
/// reached from $77EA, the round-cleared path, straight after the bonus
/// screen. Nothing about it runs when an apple is eaten; that path is $7743,
/// and it touches four other counters and not this one.
static void game_award_extra_life(void) {
  s_progress.lives = adc_dec16(s_progress.lives, 0x01, 0x00);
  game_sound_sweep();
}

/// load a shape and draw it, which is the pair every caller wants.
static void game_plot_shape(uint8_t ink, Cell c) {
  game_load_shape_masks(s_snake.shape);
  game_draw_cell(ink, c); // JMP -- a tail call.
}

/* ========================================================================== */
/* $64A9, $7590, $6B3D                                                        */
/* ========================================================================== */

/// the noise an apple makes. Two sweeps: X starts at 0, so the first
/// DEX wraps to 255 and the delay between clicks runs 256, 255, ... 1 and the
/// pitch rises; the second counts X up from 0, so the delay runs 256, 1, 2,
/// ... 255 and it falls again.
static void game_sound_sweep(void) {
  uint8_t x = 0x00;

  do {
    uint8_t y = x;
    do {
      // The rising half of the sweep: x clicks apart, and x shrinks.
      advance(5);
    } while (--y);

    // The click, at whichever port $6C2C last chose. Neither the Y it loads
    // nor the byte it reads outlives the next pass.
    //
    // Nothing checks the port. Reading $C001+port instead of $C000+port
    // passes every oracle -- correctly, as it happens, since $C020-$C02F all
    // mirror the cassette toggle and $C030-$C03F the speaker. But hardcoding
    // $30 here would pass too, and that would be a real bug: the mute would
    // stop working and no oracle in this repo looks at sound.
    advance(13);
    speaker_access(s_sound.port);
  } while (--x);

  do {
    uint8_t y = x;
    do {
      // The falling half, x counting back up.
      advance(5);
    } while (--y);

    advance(13);
    // The click itself. The read *is* the write -- see the note above.
    speaker_access(s_sound.port);
  } while (++x);
}

/// show \p key as the binding of slot \p slot on the redefinition
/// screen. The two arrow keys have no printable glyph, so they are shown as
/// 'f' and 'g', which is where the arrow shapes live in the game's own font at
/// $66A9.
static void game_show_key(uint8_t slot, uint8_t key) {
  uint8_t glyph = key;
  if (key == 0x88) { // left arrow
    glyph = 0xe6;
  }

  if (glyph == 0x95) { // right arrow
    glyph = 0xe7;
  }

  // The original parks the slot in X here because COUT would otherwise be the
  // last thing to touch it. Nothing reads it: the hi-res hook saves and
  // restores X without looking at it, and every use of the slot below is the
  // parameter.
  s_mon.ch = kKeyCH[slot];
  s_mon.cv = kKeyCV[slot];
  rom_fc68();

  rom_cout(glyph);
}

/// both side walls, each in two segments of different ink, with the
/// seam at a row derived from s_life_timer. The seam is what the player aims for.
static uint8_t game_draw_side_walls(void) {
  // The snake's tempo, and the reason this is an advance rather than nothing.
  //
  // Both side walls are redrawn once per snake step, and on a 6502 that took
  // 28,848 cycles -- measured at all 82 calls of a play run, where it is that
  // number every time. It is 1.7 frames, 28 ms, a quarter of the 115 ms a
  // step takes. Drawing is instant here now, so without this charge the step
  // falls to 84 ms and the snake moves a quarter faster: not a rendering
  // difference but a gameplay one, and the most perceptible thing in the game
  // after the sound.
  //
  // This is the case the design's "drawing becomes instant, agreed" does not
  // cover. What was agreed was giving up the *sight* of a fill sweeping down
  // the screen. The time a redraw takes is a different thing when the game's
  // main loop is paced by it.
  advance(28848);
  // The random byte is thrown away. The call is not: $6C4B advances the
  // pointer at $000E, so this is what keeps apple placement from repeating
  // level to level.
  (void)game_rand_byte();

  s_snake.shape = SHAPE_SOLID;

  uint8_t seed = s_life_timer;
  if (seed & 0x80) {
    // A negative seed is clamped, and s_life_timer reset so the next call starts from
    // a known place.
    s_life_timer = 0xff;
    seed = 0x70;
  }

  // How far down the upper segment reaches, from the timer: the walls close in
  // as a life runs out.
  const uint8_t wall_top = (seed >> 2) + 1;
  plot_vline_at(INK_WALL_TOP, 0x00, 0x01, wall_top);

  plot_vline_at(INK_WALL_TOP, 0x27, 0x01, wall_top);

  const uint8_t seam = wall_top + 1;
  // The lower segment.
  plot_vline_at(INK_WALL_BOTTOM, 0x27, seam, 0x27);

  plot_vline_at(INK_WALL_BOTTOM, 0x00, seam, 0x27);

  // Tail call: SCRN of the bottom-centre cell, which is this routine's second
  // result -- the caller uses it to decide whether to draw the gate.
  return scrn_cell((Cell){.col = 0x14, .row = 0x27});
}

/* ========================================================================== */
/* the keyboard, into the ring                                       */
/* ========================================================================== */

/// A 16-entry ring at $623C, with $624D the write index and $624C the read
/// index; dequeue_key above is the other end of it.
///
/// If advancing the write index would land on the read index the buffer is
/// full and the key is dropped -- but note it has already been *stored* by
/// then. The byte is written and then disowned by not committing the index,
/// which is a byte of work saved and a slot of the ring left holding a key
/// nobody will read until it is overwritten.
static void game_read_key(void) {
  // $6217 is on the replay coordinate, and is also where ram.probe and
  // screen.probe take their samples. It keeps its probe for both reasons.
  GAME_CYCLES_COORD(0x6217, 10);
  const uint8_t at = s_input.write;
  const uint8_t key = io_peek(0xc000);

  if (key & 0x80) {
    io_poke(0xc010, key); // clear the strobe
    s_input.ring[at] = key;

    // The $0F is the ring's size and nothing checks it: widening it to $1F
    // passes every oracle, because no recording ever presses sixteen keys
    // faster than the game reads them. Do not tidy it.
    const uint8_t next = (at + 1) & 0x0f;
    if (next != s_input.read) {
      s_input.write = next;
      return;
    }
  }

  // The RTS belongs to the routine before this one, and both early exits
  // share it, as does dequeue_key.
}

/* ========================================================================== */
/* the hi-res COUT hook                                              */
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
  return 0x66a9 + (glyph - 0x20) * 8;
}

/// The hi-res address matching the text cursor. BASL/BASH at $28 point at the
/// text line, CH at $24 is the column; `- 4 + $20` on the high byte is
/// `+ $1C`, which maps $04xx (text page 1) onto $20xx (hi-res page 1).
static uint16_t hires_cursor(void) {
  // Each half separately, and that is not an accident of the split: the low
  // byte wraps at 8 bits without carrying into the high one, so a cursor near
  // the end of a line addresses the start of the same hi-res row.
  const uint8_t hi = (s_mon.bas >> 8) - 0x04 + 0x20;
  const uint8_t lo = (s_mon.bas & 0xff) + s_mon.ch;
  return lo | (hi << 8);
}

void game_cout_hook(uint8_t ch) {
  const uint8_t glyph = ch & 0x7f;

  if (glyph >= 0x20) {
    // The original's SBC/ADC pairs honour the D flag. COUT is never reached in
    // decimal mode -- the ROM clears D at reset and neither BASIC nor the game
    // sets it around output -- so rather than carry dead decimal paths, fail
    // loudly if that assumption ever breaks.

    // The original parks all of this in the plotter's zero-page block, which
    // it is not otherwise using. They are locals: the glyph, the caller's X
    // and Y, the source and the destination all die at the closing brace, and
    // nothing outside reads them. See the block's header for why the aliasing
    // had to be shown to be unobservable before they could be split out.
    // The original saves the caller's X and Y here and puts them back at the
    // end, because it uses X as the scanline counter. That counter is the loop
    // variable below, so there is nothing left to save from.
    const uint16_t src = glyph_rows(glyph);
    uint16_t dest = hires_cursor();

    for (unsigned row = 0; row < 8; ++row) {
      hgr_poke(dest, peek(src + row));

      // One hi-res scanline down within the character cell, which is +$400.
      dest = dest + 0x0400;

      // `INX / CPX #8 / BNE`: the branch is taken on every pass but the last.
    }
  }

  rom_cout1(ch); // JMP $FDF0 -- the PLA, high bit still on it
}

/* ========================================================================== */
/* one life                                                          */
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
/* Nothing here is in emulated RAM any more; the four tables at $6387 are the  */
/* loaded image, which is a different thing.                                  */
/* ========================================================================== */

/// What the player can press, after game_read_direction() has had its
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

/// turn an absolute-direction key into the relative
/// turn that achieves it from \p dir, or $00 for "nothing to do", which covers
/// both "already going that way" and "that would be a reversal".
static uint8_t turn_for_key(uint8_t key, uint8_t dir) {
  switch (key) {
  case KEY_UP:
    return kTurnForKey[0][dir];
  case KEY_LEFT:
    return kTurnForKey[1][dir];
  case KEY_RIGHT:
    return kTurnForKey[2][dir];
  case KEY_DOWN:
    return kTurnForKey[3][dir];
  default:
    return 0;
  }
}

/// Draw \p shape into \p c with ink \p ink, through the plotter's zero-page
/// argument block.
static void plot_shape_at(uint8_t shape, uint8_t ink, Cell c) {
  s_snake.shape = shape;
  game_plot_shape(ink, c);
}

/// SCRN one cell.
static uint8_t scrn_cell(Cell c) {
  return rom_scrn(c.row, c.col);
}

/// one click of the speaker. s_sound.port holds the port offset, $30 for
/// the speaker and $20 for the cassette output that nobody can hear, which is
/// how muting works; game_sound_sweep does the same thing at $64B0.
///
/// Not a keyboard read, which is what this was called until the scoreboard
/// pass went looking: the address is $C000 + s_sound.port, and the built-in symbol
/// database resolves the $C000 to KBD, so the disassembly reads `LDA KBD,Y`
/// and the index is what makes it the speaker.
/// Every access to the speaker soft switch goes through here.
///
/// The Apple II speaker is a one-bit cone and reading $C030 flips it, so the
/// waveform is entirely the sequence of these accesses and the cycles they
/// happen on. That makes the sequence an exact oracle for any change to this
/// program's timing: dump it before and after, and it must be identical. It is
/// also the only check the sound has ever had.
///
/// $C020 is the cassette output, which is where a muted click goes.
static void speaker_access(uint8_t port) {
  static FILE *dump;
  static int tried;
  if (!tried) {
    const char *path = getenv("A2_TOGGLE_DUMP");
    tried = 1;
    if (path)
      dump = fopen(path, "w");
  }
  if (dump)
    fprintf(dump, "%u %u\n", s_cycles, (unsigned)port);
  peek(0xc000 + port);
}

static void click_speaker(void) {
  speaker_access(s_sound.port);
}

/* ========================================================================== */
/* pause and mute                                                    */
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
/* Barely covered, and worth knowing before editing: no test presses either   */
/* key, so the pause spin, the strobe clear and the mute toggle rest on the   */
/* binary alone. This comment is the only record that they are unverified.    */
/* ========================================================================== */

static void game_pause_or_toggle_sound(uint8_t key) {
  if (key == KEY_ESC) {
    for (;;) {
      // ESC pauses the game here until any key is pressed, and this charge is
      // the pause: it is the only suspend point in the loop, so it is also
      // the only thing that lets the host draw a frame and deliver the key
      // that ends the wait. Seven cycles is one pass of the original.
      //
      // Deleting it is what shipped on 2026-08-26: the game hung on ESC and a
      // second ESC could not reach it. No recording presses ESC, so all 28
      // checks passed. yield-lint.awk exists because of this loop.
      advance(7);
      key = io_peek(0xc000);
      if (key & 0x80)
        break;
    }
    io_poke(0xc010, key);
  }

  if (key == KEY_CTRL_S) {
    s_sound.muted = !s_sound.muted;
  }
}

/// draw the head in \p shape, then step it one cell and see what it landed
/// on. \p cell_out receives the occupancy byte the head landed on, which is
/// only meaningful for LIFE_CRASH; the buzz in that branch runs to
/// completion before this returns.
///
/// No \p dir parameter: $62B8 reads s_snake.direction fresh rather than
/// trusting whatever direction its caller had before the normalisation
/// below, so a direction passed in would always be overwritten before use --
/// game_play_loop's own `dir` local plays no part here.
///
/// Returns LIFE_CONTINUE for the one case that is not an ending: an empty,
/// non-gate cell, where the original falls through to `tail:`.
static LifeEnd snake_step(uint8_t shape, uint8_t *cell_out) {
  const Cell head = s_snake.head;
  s_snake.shape = shape;
  game_draw_head(INK_SNAKE, head);

  // $62B8 -- the direction back into 1..4, and the ink is the direction.
  const uint8_t dir = ((s_snake.direction - 1) & 3) + 1;
  s_snake.direction = dir;
  rom_setcol(dir);

  rom_plot(head.row, head.col);

  // $62D1 -- advance the head, and see what is there.
  const Cell next = {
      .col = head.col + kColDelta[dir],
      .row = head.row + kRowDelta[dir],
  };
  s_snake.head = next;
  const uint8_t cell = scrn_cell(next);

  s_life_outcome = cell;
  plot_shape_at(dir, INK_SNAKE, next);

  /* --- $6474: what did it move onto? ------------------------------- */
  if (cell == CELL_EMPTY) {
    rom_setcol(INK_HEAD_MARK);
    rom_plot(next.row, next.col);

    // $633C -- the gate is column $14 of row 0.
    if (next.col == 0x14) {
      if (next.row == 0) {
        return LIFE_GATE;
      }
    }
    return LIFE_CONTINUE;
  }

  if (cell == CELL_APPLE) {
    // $6480 -- an apple. Marked here; the caller does the scoring.
    s_sound.click_count = 0x20;
    rom_setcol(INK_HEAD_MARK);
    game_mark_head(next.row, next.col);
    *cell_out = cell;
    return LIFE_APPLE;
  }

  // $6494 -- solid. Pause, buzzing, for a length taken byte by byte out
  // of ROM at $E000: nobody chose those numbers, they were simply there.
  // Both loops are DEY/BNE and DEX/BNE, which test *after* decrementing,
  // so a count of zero means 256 and not none. Ten of the bytes this reads
  // out of $E000 are zero, so that is the common case here rather than a
  // corner: getting it wrong costs 12,790 cycles of the pause, which is
  // three quarters of a frame and shifts everything after it.
  uint8_t x = 0xff;
  do {
    advance(6);
    uint8_t y = peek(0xe000 + x);
    do {
      // The buzz after dying: 255 clicks whose spacing is whatever byte of
      // the ROM $E000+x happens to hold, which is why it is noise rather
      // than a note.
      advance(5);
      --y;
    } while (y != 0);
    advance(13);
    click_speaker();
    --x;
  } while (x != 0);
  *cell_out = cell;
  return LIFE_CRASH;
}

static LifeEnd game_play_loop(uint8_t *cell_out) {
  game_find_nearest_apple();

  for (;;) {
    /* --- $628B: a key, and what the game makes of it -------------------- */
    game_read_key();
    uint8_t code;
    {
      const uint8_t key = game_step_bouncers();
      // The original saves the key on the stack across the direction test;
      // here it is an argument.
      code = game_read_direction(key);
    }

    uint8_t dir = s_snake.direction;
    uint8_t shape;

  dispatch: /* $6291 */
    if (!(code & 0x80)) {
      goto autopilot;
    }

  steer: /* a key with the high bit on, so the player is steering */
    s_sound.click_count = 0x10;
    if (code == KEY_TURN_CW) {
      shape = kSnakeShape[TURN_CW][dir];
      // Computing the wrap early would be tidier and would not match.
      s_snake.direction = dir - 1;
      goto draw;
    }

    if (code == KEY_TURN_CCW) {
      shape = kSnakeShape[TURN_CCW][dir];
      s_snake.direction = dir + 1;
      goto draw;
    }

    if (code == KEY_QUIT) {
      return LIFE_QUIT;
    }

    // The four absolute keys, each a compare and a table lookup. Whatever the
    // table gives is examined as if the player had pressed it, which is why
    // this goes back to the top rather than falling through.
    {
      static const uint8_t kAbsolute[] = {KEY_UP, KEY_LEFT, KEY_RIGHT, KEY_DOWN};
      for (unsigned i = 0; i < 4; ++i) {
        if (code == kAbsolute[i]) {
          code = turn_for_key(kAbsolute[i], dir);
          goto dispatch;
        }
      }
    }

    // $639B -- anything else is the pause/mute key.
    game_pause_or_toggle_sound(code);
    goto pace;

  autopilot: /* no steering this step */
    if (s_demo_mode) {
      uint8_t proposal = 0;
      const SteerChoice choice = game_auto_steer(&proposal);
      if (choice == STEER_BOXED_IN) {
        // $6AB3 -- the auto-steer found nothing safe and gave up by jumping
        // here over its own return address. From this side that is simply
        // "carry straight on and take what comes".
        goto straight;
      }
      if (choice == STEER_TURN) {
        // $6312 -- it proposed a turn. Act on it as though it had been typed,
        // re-entering below the high-bit test the way $6312 does.
        code = proposal;
        goto steer;
      }
    }

  straight: /* $6315 */
    shape = kSnakeShape[TURN_STRAIGHT][dir];

  draw: /* draw the head, then step it one cell */
  {
    uint8_t cell = 0;
    const LifeEnd outcome = snake_step(shape, &cell);
    if (outcome != LIFE_CONTINUE) {
      *cell_out = cell;
      return outcome;
    }
    goto tail;
  }

  tail: /* trim the tail, unless the snake is still growing */
    if (s_snake.growth) {
      s_snake.growth = s_snake.growth - 1;
      s_sound.click_count = 0x07;
    } else {
      const Cell tail = s_snake.tail;
      const uint8_t under = scrn_cell(tail);

      // The original keeps `under` on the stack across the erase. It stays on
      // the emulated stack here too: ram.probe hashes the live stack, and a
      // sample taken inside the plotter would otherwise see a byte on one
      // engine and not the other.
      rom_setcol(INK_ERASE);
      rom_plot(tail.row, tail.col);
      plot_at(INK_ERASE, tail);

      // $63DA -- the byte that was under the tail is the direction the tail
      // must follow, so the same delta tables move it on.
      const uint8_t tail_dir = under;
      const Cell tail_next = {
          .col = tail.col + kColDelta[tail_dir],
          .row = tail.row + kRowDelta[tail_dir],
      };
      s_snake.tail = tail_next;
      const uint8_t ahead = scrn_cell(tail_next);

      // ahead + 0x0c is a computed shape-table offset (ahead is the
      // direction 1-4 stored under the next body segment), not one of
      // Shape's four named entries -- a one-off outside the closed set,
      // left bare on purpose.
      plot_shape_at(ahead + 0x0c, INK_SNAKE, tail_next);
    }

  pace: /* the timer, the walls, and the delay that sets the speed */
    click_speaker();
    {
      const uint8_t left = s_life_timer - 1;
      s_life_timer = left;
      if (left == 0) {
        return LIFE_TIMEOUT;
      }
    }

    const uint8_t gate_cell = game_draw_side_walls();
    if (gate_cell == CELL_EMPTY) {
      // $642D -- the gate at the bottom is clear, so draw it. No edge charge
      // here: $6429's branch falls through to this and is only *taken* when
      // the cell is occupied.
      s_snake.shape = SHAPE_SOLID;
      plot_hline_at(INK_WALL_BOTTOM, 0x12, 0x27, 0x16);
      rom_setcol(INK_WALL_BOTTOM);
      rom_plot(0x27, 0x14);
    }

    // $6450 -- the delay that sets the speed. s_step_delay iterations, each one
    // ticking the falling tone and taking a key, and counting $6473 down for
    // as long as the last move gave it something to say.
    // DEX/BNE again: s_step_delay of zero would mean 256 passes, not none.
    uint8_t n = s_step_delay;
    do {
      game_tick_sound();
      // The main loop's keyboard poll, ~1.2 ms apart. This is the yield the
      // whole game depends on: 82 passes per snake step, and the only place
      // between one step and the next where the host gets to draw or the
      // player gets to steer.
      // 48 rather than the 58 the region measures, for the same reason as the
      // death pause: game_read_key charges $6217's own 10 and that
      // charge stays, because $6217 is where the replay coordinate and both
      // sampling probes live.
      advance(48);
      game_read_key();
      if (s_sound.click_count) {
        // The steering click. Its spacing is what makes the waveform
        // irregular the way the original is.
        advance(41);
        click_speaker();
        s_sound.click_count = s_sound.click_count - 1;
      }
      --n;
    } while (n != 0);
  }
}

/* ========================================================================== */
/* the auto-steer                                                    */
/*                                                                            */
/* Chase the nearest apple. Candidate directions are                          */
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

/// the absolute-direction key that turns the snake to face \p dir.
/// Index 0 is unused; the four that matter are the I/J/K/M diamond.
static uint8_t key_for_direction(uint8_t dir) {
  return kSteerKey[dir];
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
static bool steer_try(uint8_t dir) {
  /* What the original's BEQ tests. It was s_status_not_z; nothing outside this
     routine ever read that flag, so it is a local. This one the gate does
     run: inverting the sense of the return below fails the play screen. */
  uint8_t move_taken = 0;

  s_snake.steer_dir = dir;
  { // was game_move_ok()

    uint8_t cell = 0;
    const MoveVerdict v = snake_move_verdict(s_snake.steer_dir, &cell);

    // Turn the verdict back into what the callers at $6A40 branch on. The
    // switch covers every MoveVerdict, so move_taken is always assigned; the
    // initialiser above is there because the compiler cannot see that.
    switch (v) {
    case MOVE_TARGET_TAKEN:
      // $6AD9 CMP #$0F left these.
      move_taken = cell != CELL_APPLE;
      break;
    case MOVE_ROW_ZERO:
    case MOVE_OK:
      move_taken = 0x00;
      break;
    case MOVE_DEAD_END:
      move_taken = 0x01;
      break;
    }
  }
  // The original branches on Z, which game_move_ok leaves set for exactly the
  // verdicts that permit the move -- including a target holding the apple.
  return move_taken == 0;
}

static SteerChoice game_auto_steer(uint8_t *key_out) {
  const uint8_t apple_row = s_apple.row;
  const uint8_t apple_col = s_apple.col;
  const uint8_t head_row = s_snake.head.row;
  const uint8_t head_col = s_snake.head.col;

  bool settled = false;

  // $6A32 -- if the apple is on another row, close that first. This one test
  // is a BNE, so its edge is charged when the move is *refused*; every later
  // test is a BEQ to the accept path and charges its edge the other way round.
  if (apple_row != head_row) {
    uint8_t dir;
    if (apple_row >= head_row) {
      dir = DIR_DOWN;
    } else {
      dir = DIR_UP;
    }
    settled = steer_try(dir);
  }

  // $6A5A -- the column, toward the apple and then away from it.
  if (!settled) {
    if (apple_col >= head_col) {
      settled = steer_try(DIR_RIGHT);
      if (!(settled)) {
        settled = steer_try(DIR_LEFT);
      }
    } else {
      settled = steer_try(DIR_LEFT);
      if (!(settled)) {
        settled = steer_try(DIR_RIGHT);
        // Refused: falls straight into $6A8D, where the other branch had to
        // spend a JMP to get.
      }
    }
  }

  // $6A8D -- the row again, now as an escape rather than as progress.
  if (!settled) {
    if (apple_row >= head_row) {
      settled = steer_try(DIR_DOWN);
    }
    if (!settled) {
      settled = steer_try(DIR_UP);
      if (!(settled)) {
        settled = steer_try(DIR_DOWN);
        if (!(settled)) {
          // $6AB3 -- nothing is safe.
          return STEER_BOXED_IN;
        }
      }
    }
  }

  // $6A48 -- a direction was accepted. Already going that way means there is
  // nothing to say; otherwise name the key that turns to it.
  const uint8_t dir = s_snake.steer_dir;
  if (dir == s_snake.direction) {
    *key_out = dir;
    return STEER_STRAIGHT;
  }
  *key_out = key_for_direction(dir);
  return STEER_TURN;
}

/* ========================================================================== */
/* the status panel                                                  */
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
/*                                                                            */
/* Every charge below is written out with a literal address rather than passed */
/* to a helper. A helper reads better and is wrong: the site-list lint finds   */
/* these by grepping for the literal, so an address reaching GAME_CYCLES       */
/* through a parameter is invisible to it, and nine of these were.            */
/* ========================================================================== */

/// the flag game_print_bcd raises when it prints a digit, so that
/// game_print_zero_if_blank knows whether the field came out empty.
static void clear_leading_zero_flag(void) {
  s_mon.h2 = 0x00;
}

static void game_status_panel(void) {
  // SCORE, row $14 column $00. Four BCD bytes in s_progress.score, little-endian.
  s_mon.cv = 0x14;
  s_mon.ch = 0x00;
  game_print_inline_str(0x72d8);
  clear_leading_zero_flag();
  game_print_bcd(s_progress.score[3]);
  game_print_bcd(s_progress.score[2]);
  game_print_bcd(s_progress.score[1]);
  game_print_bcd(s_progress.score[0]);
  game_print_zero_if_blank();

  // HI SCORE, same row, column $14. Four bytes in s_progress.best.
  s_mon.ch = 0x14;
  game_print_inline_str(0x7307);
  clear_leading_zero_flag();
  game_print_bcd(s_progress.best[3]);
  game_print_bcd(s_progress.best[2]);
  game_print_bcd(s_progress.best[1]);
  game_print_bcd(s_progress.best[0]);
  game_print_zero_if_blank();

  // APPLES LEFT, row $15 column $00. Two bytes at $725A.
  s_mon.ch = 0x00;
  s_mon.cv = 0x15;
  game_print_inline_str(0x733d);
  clear_leading_zero_flag();
  game_print_bcd(s_progress.left[1]);
  game_print_bcd(s_progress.left[0]);
  game_print_zero_if_blank();

  // A space, which the next field's cursor move immediately overrides. It is
  // there to wipe the character one place past this field, left over from a
  // longer count earlier in the game.
  rom_cout(0xa0);

  // VALUE, same row, column $14. Two bytes in s_progress.apple_value -- the current worth of an
  // apple, which game_set_apple_value computes per level.
  s_mon.ch = 0x14;
  game_print_inline_str(0x736b);
  clear_leading_zero_flag();
  game_print_bcd(s_progress.apple_value[1]);
  game_print_bcd(s_progress.apple_value[0]);
  game_print_zero_if_blank();

  // SNAKES LEFT, row $16 column $00. One byte at $725E, printed as though it
  // were the low half of a two-byte field: the high half is the literal 0
  // below, which prints nothing at all once leading zeros are suppressed. It
  // costs a call to keep the shape of every other field.
  s_mon.cv = 0x16;
  s_mon.ch = 0x00;
  game_print_inline_str(0x7392);
  clear_leading_zero_flag();
  game_print_bcd(0x00);
  game_print_bcd(s_progress.lives);
  game_print_zero_if_blank();

  // LEVEL, same row, column $14. One byte at $7265.
  s_mon.ch = 0x14;
  game_print_inline_str(0x73b8);
  clear_leading_zero_flag();
  game_print_bcd(s_progress.level);
  game_print_zero_if_blank();

  // Home the cursor. This CV write is the one that needs VTAB, because nothing
  // prints after it to recompute the line base.
  s_mon.cv = 0x00;
  rom_fc68();
}

/* ========================================================================== */
/* the bonus screen                                                  */
/*                                                                            */
/* Awarded when a level is finished. The bonus is twice whatever an apple was  */
/* worth on that level, which the original says twice over: once as BCD        */
/* arithmetic into $78B0/$78B1 so the number can be printed, and once as two   */
/* consecutive calls to game_add_score, each adding the full                   */
/* s_progress.apple_value. Neither reads the other's answer.                   */
/*                                                                            */
/* This routine is *entered with decimal mode set* -- unusual here, and why it */
/* carries no assert_binary_mode. The generated C makes the same claim: its     */
/* binary-mode path is dead, folded away because D is known set on entry.      */
/* $78C7 clears it before anything else runs.                                  */
/*                                                                            */
/* Then a box: a frame in ink 9 and its interior wiped in ink 0, four rows at  */
/* a time. The text goes through the game's own hi-res font, which is what     */
/* $6641 installs and what the two pokes at $795F take back out again.         */
/* ========================================================================== */

static void game_bonus_screen(void) {
  // $78B3 -- double the apple's value into $78B0/$78B1, in BCD.
  const uint16_t lo = adc_dec16(s_progress.apple_value[0], s_progress.apple_value[0], 0x00);
  s_progress.bonus[0] = lo;
  const uint16_t hi =
      adc_dec16(s_progress.apple_value[1], s_progress.apple_value[1], (lo >> 8) & 0x01);
  s_progress.bonus[1] = hi;

  // Twice, because the bonus is twice the apple value and game_add_score adds
  // it once.
  game_add_score();
  game_add_score();
  game_status_panel();

  // $78D1 -- the frame, in ink 9: top and bottom edges, then both sides.
  s_snake.shape = SHAPE_APPLE;
  // Columns $0D-$1A, rows $10-$15. The two sides used to inherit their column
  // from the edge above: an hline left $02 at its own endpoint, so the
  // first vline ran down $1A, the right edge, and not the $0D it looks like.
  plot_hline_at(INK_APPLE, 0x0d, 0x10, 0x1a);
  plot_hline_at(INK_APPLE, 0x0d, 0x15, 0x1a);
  plot_vline_at(INK_APPLE, 0x1a, 0x10, 0x15);
  plot_vline_at(INK_APPLE, 0x0d, 0x10, 0x15);

  // $7909 -- the interior, in ink 0, one row at a time from $11 to $14. The
  // original re-loads $02 each time and increments $03 in place, which is why
  // the rows are not written out as constants.
  plot_hline_at(INK_ERASE, 0x0e, 0x11, 0x19);
  plot_hline_at(INK_ERASE, 0x0e, 0x12, 0x19);
  plot_hline_at(INK_ERASE, 0x0e, 0x13, 0x19);
  plot_hline_at(INK_ERASE, 0x0e, 0x14, 0x19);

  // $7937 -- "BONUS: " and the amount, through the hi-res font.
  s_mon.ch = 0x0f;
  s_mon.cv = 0x09;
  game_install_cout_vector();
  // The LDA #$4A flags are overwritten by the second load; only these outlive.
  game_print_inline_str(0x7944);
  s_mon.h2 = 0x00;
  game_print_bcd(s_progress.bonus[1]);
  game_print_bcd(s_progress.bonus[0]);

  // $795D -- COUT back to the ROM's, and $02 becomes the outermost counter of
  // the pause below.
  s_mon.csw = 0xfdf0;
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
    uint8_t x = 0x80;
    do {
      uint8_t y = x;
      do {
        --y;
      } while (y != 0);
      click_speaker();
      --x;
    } while (x != 0);
    passes = passes - 1;
  } while (passes != 0);
}

/* ========================================================================== */
/* start a life                                                      */
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

static void game_begin_life(void) {
  const uint8_t tail_col = game_start_life(0x14);

  s_snake.tail.col = tail_col; // from $6630, by way of $660F
  s_snake.head.row = 0x27; // the bottom edge
  s_snake.tail.row = 0x27; // the same cell
  s_snake.direction = DIR_UP;
  s_snake.growth = 0x0a; // ten
  s_life_timer = 0x64;

  // $6279 -- empty the sixteen-entry key ring at $623C. DEX/BPL, so it runs
  // down through 0 and stops when X wraps negative, one more pass than a
  // count of $0F suggests.
  uint8_t x = 0x0f;
  do {
    s_input.ring[x] = 0x00;
    --x;
  } while (!(x & 0x80));

  s_input.read = 0x00;
  s_input.write = 0x00;
  game_play_one_life();
}

/* ========================================================================== */
/* the setup screen                                                  */
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

static void game_setup_screen(void) {
  // $7980 -- keep $0E/$0F inside the window game_rand_byte expects.
  const uint8_t hi = s_rand_ptr >> 8;
  bool clamp_lo = hi >= 0x1f;
  if (!clamp_lo) {
    clamp_lo = hi < 0x18;
  }
  if (clamp_lo) {
    s_rand_ptr &= 0xffde;
  }
  s_rand_ptr = (s_rand_ptr & 0x1fff) | 0x1800;

  // $73D8 -- the first call through here never asks anything.
  if (!s_setup_seen) {
    s_demo_mode = true;
    s_difficulty = 0x01;
    s_setup_seen = true;
    return;
  }

  // $73E9 -- the prompt, and the two counters that time it out. $02 is the
  // outer one and $03 the inner; both count *up* to zero.
  s_mon.cv = 0x17;
  s_mon.ch = 0x00;
  game_print_inline_str(0x73f3);
  // $02 and $03 time the prompt out: the inner one wraps 256 times per tick of
  // the outer, and when the outer wraps nobody has answered.
  uint8_t outer_count = 0xe8;
  uint8_t inner_count = 0x00;

  uint8_t key;
wait: /* $741C */
  for (;;) {
    // $741C -- spin Y round once, then look at the keyboard. This site keeps
    // its probe: it is one of the addresses the replay coordinate counts.
    // Y again, and again not initialised by the original. Measured the same
    // way as spin's: zero on every pass of both scenarios, and the loop itself
    // leaves it zero, so only the first arrival could ever differ.
    uint8_t ticks = 0;
    do {
      // The 7.8 s timeout into demo mode -- 36% of a play run's cycles, and
      // the longest wait in the game. It is also a polling loop, so this is
      // the yield that lets the player answer the prompt at all.
      advance(5);
      ++ticks;
    } while (ticks != 0);

    // The keyboard read. Already a charge, and it must stay one; it keeps its
    // probe as well because it is on the replay coordinate.
    GAME_CYCLES_COORD(0x741f, 6);
    key = io_peek(0xc000);
    if (key & 0x80) {
      break;
    }

    const uint8_t inner = inner_count + 1;
    inner_count = inner;
    if (inner != 0) {
      continue;
    }

    // $7428 -- once the inner counter wraps, try the joystick, if one is
    // selected. Each button stands in for a digit.
    if (s_input.joystick) {
      io_peek(0xc05b);
      if (!(io_peek(0xc062) & 0x80)) {
        key = 0xb1;
        break;
      }
      io_peek(0xc05a);
      if (!(io_peek(0xc062) & 0x80)) {
        key = 0xb0;
        break;
      }
      if (!(io_peek(0xc063) & 0x80)) {
        key = 0xb2;
        break;
      }
    }

    // $7451 -- the outer counter. When it wraps too, nobody is answering.
    const uint8_t outer = outer_count + 1;
    outer_count = outer;
    if (outer == 0) {
      s_demo_mode = true;
      s_difficulty = 0x01;
      io_poke(0xc010, 0x01);
      return;
    }
  }

  // $7461 -- something was pressed. Clear the strobe with it still in A, the
  // way the original does.
  io_poke(0xc010, key);
  if (key != 0xc3) {
    if (key < 0xb0) {
      goto wait;
    }
    if (key >= 0xb3) {
      goto wait;
    }
    // $7470 -- a digit. The subtract is a plain SBC with carry set.
    s_difficulty = key - 0xb0;
    s_demo_mode = false;
    io_poke(0xc010, 0x00);
    return;
  }

  // $747F -- C, so redefine the keys instead. Show the six current bindings,
  // draw the highlight, then walk them again asking for replacements.
  game_clear_hgr();
  io_peek(0xc052);
  game_install_cout_vector();
  // The LDA #$4A flags are overwritten by the second load; only these outlive.
  s_mon.cv = 0x01;
  game_print_inline_str(0x748e);

  for (uint8_t i = 0; i != 6; ++i) {
    game_show_key(i, s_input.bindings[i]);
  }

  // 0x02: the arrowhead's own shape-table entry, a one-off outside Shape's
  // four named values -- left bare on purpose.
  plot_shape_at(0x02, INK_SNAKE, (Cell){.col = 0x1e, .row = 0x12});
  // The shaft below it, down the same column -- which the original inherited
  // from the plot above rather than restating. SHAPE_VBAR is the same mask
  // kSnakeShape[TURN_STRAIGHT] gives the snake's own straight vertical body
  // segment; drawn as a run of cells here, it reads as the arrow's shaft.
  s_snake.shape = SHAPE_VBAR;
  plot_vline_at(INK_SNAKE, 0x1e, 0x13, 0x1d);
  // At the stem's far end -- the vline above left $03 on $1D. 0x0e is
  // another one-off shape-table entry, same reasoning as the 0x02 above.
  plot_shape_at(0x0e, INK_SNAKE, (Cell){.col = 0x1e, .row = 0x1d});

  for (uint8_t i = 0; i != 6; ++i) {
    const uint8_t chosen = game_edit_key(i);
    s_input.bindings[i] = chosen;
    game_show_key(i, chosen);
  }

  // $7587 -- COUT back to the ROM's.
  s_mon.csw = 0xfdf0;
}

/* ========================================================================== *
 * Routines decompiled by hand                                              *
 * ========================================================================== *
 *
 * apple2tc drives disassembly from a recorded run, so a routine reached only
 * through an indirect jump the recording never took is invisible to it: the
 * bytes are classified as data and nothing is emitted. These were decoded from
 * the binary instead, which is why each carries more of the original than the
 * rest of the file does.
 */

/* ========================================================================== */
/* The game's own COUT handler, a hi-res text renderer.                       */
/*                                                                            */
/* game_install_cout_vector points the ROM's output vector at this, so every   */
/* later COUT lands here instead of the monitor's COUT1. Control characters    */
/* are handed straight on to COUT1; anything printable is drawn as an 8-byte   */
/* glyph from the game's own font.                                            */
/*                                                                            */
/* Decoded from the binary by hand, so the original is kept for reference:     */
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

/// print the NUL-terminated string that follows the call.
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
static void game_print_inline_str(uint16_t ret_addr) {
  s_str_ptr = ret_addr;
  rom_fc68(); // VTAB to the current CV

  for (;;) {
    // The pointer is stepped before the read, which is why the caller passes
    // the address of the JSR's last byte rather than of the string.
    ++s_str_ptr;

    const uint8_t ch = peek(s_str_ptr);
    if (!ch) {
      break;
    }

    rom_cout(ch);
  }
}

/* ========================================================================== */
/* the hi-res cell plotter.                            */
/*                                                                            */
/* The game treats hi-res page 1 as a grid of 48 cell rows, each four         */
/* scanlines tall and one byte (seven pixels) wide. $6000 and $6030 hold the  */
/* low and high bytes of each row's base address -- $2000, $3000, $2080,      */
/* $3080, ... -- and successive scanlines within a cell are $400 apart, which */
/* is why walking down a cell is just +4 on the high byte.                    */
/*                                                                            */
/* Arguments. The original passed all three in zero page; the cell is a       */
/* parameter here and only the shape is still a variable:                     */
/*    s_snake.shape  -- picks four AND masks from the table at $6174 ($00)    */
/*    c.col  -- the byte offset within the cell row ($02)                     */
/*    c.row  -- cell row, 0-47 ($03)                                          */
/* Scratch, which was $04-$07 and is now four locals:                         */
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
/* snake-byte.b33, and checked against them byte for byte -- the gate cannot  */
/* check a table entry nothing reads.                                         */
/* ========================================================================== */

/* ========================================================================== */
/* runs of cells.                                             */
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

/* ========================================================================== */
/* the screen-script primitives.                       */
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

/* ========================================================================== */
/* the score.                                   */
/*                                                                            */
/* The score is BCD: four little-endian bytes in s_progress.score, eight      */
/* digits. $7267 adds to it with the 6502's decimal mode, and $71F3 prints    */
/* one byte of it as two digits by nibble, since in BCD a nibble is already a */
/* digit.                                                                     */
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

/* ========================================================================== */
/* the merging cell plotter.                                         */
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

/* ========================================================================== */
/* draw the playfield, and run the level's display list.             */
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
/* ========================================================================== */

/* ========================================================================== */
/* snake state and scoring setup.        */
/*                                                                            */
/* $624F/$6250 are the snake's head column and row; $6251/$6252 are the tail. */
/* $62D7 walks the head forward by adding the current direction's deltas from */
/* $6232/$6237 and SCRNs the result off the lo-res map (see the $7019 header).*/
/*                                                                            */
/* $0301 is the difficulty, 0-2, and $71CD is what gives it away: it indexes  */
/* the three-byte table at $71C8 -- $10, $15, $20 -- with $0301 and adds that */
/* entry to s_progress.apple_value/s_progress.apple_value[1] once per level,  */
/* in BCD. So an apple is worth base[difficulty] * level, which is why        */
/* s_progress.apple_value read $15 throughout the recordings: difficulty 1,   */
/* level 1. It is also what decides the two                                   */
/* optional wall gaps in game_draw_playfield.                                 */
/* ========================================================================== */

/* ========================================================================== */
/* apples, and the sound trick.                        */
/*                                                                            */
/* s_sound.port is the game's mute switch, and it is a nice piece of work:    */
/* every sound routine reads `LDA $C000,Y` with Y = s_sound.port, and $6C2C   */
/* picks either $30 or $20 for it. $C030 is the speaker; $C020 is the         */
/* cassette output. So                                                        */
/* turning the sound off routes the identical click to a port nobody is       */
/* listening to, and the timing loops do not change at all -- no branch in    */
/* the hot path, and muting cannot alter the game's speed.                    */
/* ========================================================================== */

/* ========================================================================== */
/* $6217, $7590, $6B3D                                                        */
/* ========================================================================== */

/* ========================================================================== */
/* is the next cell in direction $6B38 worth moving into?            */
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

/* ========================================================================== */
/* move the bouncer one step.                                        */
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


/* ========================================================================== */
/* $728D, $6BFB                                                               */
/* ========================================================================== */

/* ========================================================================== */
/* $6594, $69C3                                                               */
/* ========================================================================== */

/* ========================================================================== */
/* the rest of the input path.                         */
/*                                                                            */
/* All three are decoded in full here now, not adapters over another file.    */
/* They carry most of what no recording exercises -- ESC and Ctrl-S, the      */
/* arrow keys on the redefinition screen, and the whole joystick branch --    */
/* so the comments above each routine, rather than probe-acceptance.sh's      */
/* baseline list, are what now record which blocks rest on the binary alone.  */
/* ========================================================================== */

/* ========================================================================== */
/* one life, via game_play_loop above.                                */
/* ========================================================================== */

/// play one life and record how it ended in s_life_outcome, which is where
/// $7739 reads it. What used to be an adapter is just this write-back now: no
/// return address, and A is not left holding the reason because nothing reads
/// it there any more.
static void game_play_one_life(void) {
  uint8_t cell = 0;
  // CELL_EMPTY/CELL_APPLE here are not the occupancy map -- they are
  // s_life_outcome borrowing that enum's encoding on purpose. See
  // s_life_outcome's own comment for why 0xff/0xfe are safe alongside it.
  switch (game_play_loop(&cell)) {
  case LIFE_GATE:
    s_life_outcome = CELL_EMPTY;
    break;
  case LIFE_APPLE:
    s_life_outcome = CELL_APPLE;
    break;
  case LIFE_QUIT:
    s_life_outcome = OUTCOME_QUIT;
    break;
  case LIFE_TIMEOUT:
    s_life_outcome = OUTCOME_TIMEOUT;
    break;
  case LIFE_CRASH:
    s_life_outcome = cell;
    break;
  case LIFE_CONTINUE:
    // Never returned here: game_play_loop() converts snake_step()'s
    // LIFE_CONTINUE straight into `goto tail` and keeps playing, so this
    // switch never sees it. The case exists so the compiler catches it if
    // that stops being true.
    abort();
  }
}

/* ========================================================================== *
 * The top level                                                            *
 * ========================================================================== */

/* ========================================================================== */
/* $3750 and $7691 -- the top level                                           */
/*                                                                            */
/* The last of the decompiler's output, and the outermost loop in the game:   */
/* new game -> new level -> new round -> one life -> what happened -> repeat.  */
/* Everything it calls was converted long before it was.                      */
/*                                                                            */
/* Four nested for(;;) loops now, zero labels: game, level, round, life. A    */
/* life's ending sets `ending` and breaks its own loop; the round and level   */
/* loops above re-test it and break again in turn, so a life ending unwinds   */
/* as many as three loops in a row of breaks -- exactly the flag the old      */
/* labels never needed. The two outer loops carry their setup at the bottom   */
/* rather than the top because the cold entry below jumps in three levels     */
/* down, past both: an ordinary iteration falls back to that same spot, so    */
/* putting the setup there serves both paths at once.                         */
/*                                                                            */
/* Decimal mode is explicit. The generated code emitted both a binary and a   */
/* decimal arm for every add and subtract because it could not prove which    */
/* way the D flag went; the assembly settles it, since the original brackets  */
/* each BCD run with SED/CLD. $7743-$7777 and $7817-$783A are decimal and     */
/* everything else is binary, so each operation is written the one way it     */
/* runs, and there is no D flag left to consult.                             */
/*                                                                            */
/* The ROM's own arms went with it, on an argument rather than a measurement:  */
/* every SED/CLD region in this file contains only adc_dec16, sbc_dec16 and    */
/* the bcd_* helpers -- which take their decimal-ness explicitly -- and no ROM */
/* call at all, and each clears D before it returns. So the monitor was only   */
/* ever entered in binary. game_bonus_screen looked like the counter-example   */
/* and is not: it is entered with D set, and its $78C7 CLD comes before it     */
/* prints anything. Measured as well: 3,942 D tests across both scenarios,     */
/* every one of them binary.                                                   */
/* ========================================================================== */

/// BCD add across a low/high pair, as SED/CLC/ADC/ADC leaves it.
static void bcd_add16(uint8_t at[2], uint8_t by) {
  uint16_t r = adc_dec16(at[0], by, 0);
  at[0] = r;
  r = adc_dec16(at[1], 0x00, (r >> 8) & 0x01);
  at[1] = r;
}

/// BCD subtract across a low/high pair, as SED/SEC/SBC/SBC leaves it.
static void bcd_sub16(uint8_t at[2], uint8_t by) {
  uint16_t r = sbc_dec16(at[0], by, 1);
  at[0] = r;
  r = sbc_dec16(at[1], 0x00, (r >> 8) & 0x01);
  at[1] = r;
}

/// How a round ended, and so how far game_cold_start's loop nest unwinds. The
/// life loop sets it on the way out; the round and level loops around it read
/// it. It replaces the three jumps the original made out of $7847/$789A.
typedef enum {
  /// $789A took a life off the count. Another attempt at the same round.
  ROUND_RETRY,
  /// $77EA -- every apple eaten. The level advances.
  ROUND_CLEARED,
  /// The quit key, or no lives left. Back to $7691.
  ROUND_GAME_OVER,
  /// Not an ending. handle_life_result()'s signal that the round goes on and
  /// the life loop falls into $7719, which is what the original does by not
  /// jumping. Kept last and named apart from the three real endings above so
  /// it cannot be mistaken for one; it never reaches the round or level loop.
  ROUND_CONTINUE,
} RoundEnd;

/// $76C7 -- everything a fresh attempt at a round resets before the first
/// life of it runs. Straight-line in the original too: the round loop's whole
/// prologue, down to the JMP at $7716.
///
/// **The statement order here is only weakly checked.** Two reorderings were
/// mutation-tested and both passed, and both are genuinely inert rather than
/// missed: game_draw_playfield touches neither `left` nor `quota`, and while
/// it *can* rewrite s_level_time through the script's OP_STORE, in both
/// scenarios that op fires 8 times between them and every one writes the 100
/// that is already there. A script that set a different level time would make
/// the s_life_time line's position observable. Nothing in the committed set
/// does.
static void start_round(void) {
  s_progress.afield[0] = 0x00;
  s_progress.afield[1] = 0x00;
  s_progress.eaten[0] = 0x00;
  s_progress.eaten[1] = 0x00;
  s_progress.left[0] = s_progress.quota[0];
  s_progress.left[1] = s_progress.quota[1];
  game_draw_playfield();
  s_life_time = s_level_time;
  game_set_apple_value();
  io_peek(0xc054); // page 1
  io_peek(0xc053); // mixed text/graphics
  const Cell apple = game_place_apple();
  // $76F6 redraws it, at the cell game_place_apple just chose.
  game_plot_shape(INK_APPLE, apple);
  s_step_delay = 0x52;
  s_snake.head_moved = false;
  s_life_timer = s_life_time;
  s_mon.wndtop = 0x14; // window top, so HOME clears only the status panel
  rom_home();
  game_status_panel();
  game_begin_life();
}

/// $7739 -- what the life that just ended means for the round.
///
/// The original reaches this both from $7716 (a fresh round) and from the
/// bottom of the life loop, dispatching on s_life_outcome and either falling
/// into $7719 for another life or jumping out of the nest three different
/// ways. Those three jumps are the three non-CONTINUE values here.
static RoundEnd handle_life_result(void) {
  /* $7739: s_life_outcome says how the life ended. */
  if (s_life_outcome == CELL_APPLE) {
    /* $773E */
    bcd_sub16(s_progress.afield, 0x01);
    bcd_sub16(s_progress.left, 0x01);
    bcd_add16(s_progress.eaten, 0x01);

    // $777B -- points only for the first $11 apples of the round. The
    // high byte must be zero and the low one below $11, both BCD.
    if (!(s_progress.eaten[1])) {
      if (!(s_progress.eaten[0] >= 0x11)) {
        game_add_score();
      }
    }

    s_snake.growth = s_snake.growth + 0x0a; // ten more cells

    // $7793 -- anything left in the round?
    if (s_progress.left[0] || s_progress.left[1]) {
      // place one only when both countdown bytes are zero
      if (!s_progress.afield[0] && !s_progress.afield[1]) {
        game_place_apple();
      }
    } else {
      /* that was the last one. Draw the bar across the bottom, put the
         marker on it, and stop the clock for the run to the gate -- see
         s_life_time for why $FF stops it rather than lengthening it. */
      s_snake.shape = SHAPE_SOLID;
      // 0x06 is not one of the nine named Ink values (0x00,0x02,0x03,
      // 0x05,0x07,0x09,0x0c,0x0d,0x0f) -- a one-off bar colour used
      // only here, left bare on purpose rather than inventing a tenth
      // Ink member for it.
      plot_hline_at(0x06, 0x12, 0x00, 0x16);
      plot_shape_at(SHAPE_SOLID, INK_ERASE, (Cell){.col = 0x14, .row = 0x00});
      s_life_time = 0xff;
      rom_setcol(INK_ERASE);
      rom_plot(0x00, 0x14);
    }
    // both arms fall into $7719
  } else if (s_life_outcome != CELL_EMPTY) {
    /* $77E8 -- $FE is the snake running out of room rather than dying.
       With apples still owed that only makes the round harder; with
       none left, and for every other outcome, the life is over. */
    if (s_life_outcome == OUTCOME_TIMEOUT && (s_progress.left[1] || s_progress.left[0])) {
      // three more apples in the round, and three more to come
      bcd_add16(s_progress.quota, 0x03);
      bcd_add16(s_progress.left, 0x03);
      game_place_apple();
      game_place_apple();
      game_place_apple();
      // falls into $7719
    } else if (s_life_outcome == OUTCOME_QUIT) {
      /* $7847 */
      return ROUND_GAME_OVER; // the player pressed the quit key
    } else if (s_life_outcome == OUTCOME_TIMEOUT) {
      // **Never executed by any test in either scenario.** Out of
      // room with the round already emptied: another life, no pause
      // and no life lost. Note the sense: the original *branches
      // away* when it is not $FE, so equality is the fall-through.
      // falls into $7719
    } else {
      if (!s_demo_mode) { // the demo does not wait to be told to carry on
        /* "PRESS SPACE BAR TO CONTINUE", then wait for space or the
           paddle button, whichever the setup screen selected. */
        s_mon.cv = 0x17;
        s_mon.ch = 0x00;
        game_print_inline_str(0x7867);
        for (;;) {
          // "PRESS SPACE BAR TO CONTINUE", and this is the only yield
          // in the loop that waits for it. Nothing bounds this one --
          // the game sits here until the player presses something -- so
          // losing it hangs the program outright rather than merely
          // making it stutter.
          // Eight, not sixteen: a pass of this loop costs 16 cycles,
          // but half of that is $7890's own charge below, which
          // survives because the address carries the replay coordinate.
          // Charging the measured region here as well would count it
          // twice -- and did, until the block-head trace ran 22% short
          // and said so.
          advance(8);
          if (s_input.joystick) {
            // The button reads with bit 7 *clear* when pressed here.
            if (!(io_peek(0xc061) & 0x80)) {
              break;
            }
          }
          // $7890 is on the replay coordinate -- see GAME_CYCLES_COORD.
          GAME_CYCLES_COORD(0x7890, 8);
          const uint8_t key = io_peek(0xc000);
          if (key == 0xa0) {
            io_poke(0xc010, key);
            break;
          }
        }
      }

      /* $789A */
      if (!s_progress.lives) {
        return ROUND_GAME_OVER;
      }
      {
        const uint16_t r = sbc_dec16(s_progress.lives, 0x01, 0x01);
        s_progress.lives = r;
      }
      return ROUND_RETRY; // $789A: JMP $76C7
    }
  } else {
    /* $77EA -- the round is empty, so the level is cleared. */
    {
      const uint16_t r = adc_dec16(s_progress.level, 0x01, 0x00);
      s_progress.level = r;
    }
    s_script_index = s_script_index + 1;
    // $77F8 -- no life was lost this round, so it earns a bonus.
    if (s_progress.lives == s_progress.lives_at_level_start) {
      game_bonus_screen();
    }
    game_award_extra_life();
    return ROUND_CLEARED;
  }

  /* falls into $7719 */
  return ROUND_CONTINUE;
}

static void game_cold_start(void) {
  /* copy eight pages of level data from $3800 down to $1800. The
     original walks them by incrementing the operands of its own LDA and STA
     ($3754 and $3757), which is why it re-enters $3750 eight times rather
     than looping inside. The page counter is this loop's own, so the two
     patched bytes are written by nothing and read by nothing; they were
     mirrored back only for as long as the memory oracle hashed them. */
  for (unsigned page = 0; page != 8; ++page) {
    // Probed, not because anything needs to observe it, but because it is what
    // probe-acceptance.sh aligns the two builds on -- see GAME_CYCLES_ANCHOR.
    GAME_CYCLES_ANCHOR(0x3750, 2);
    for (unsigned i = 0; i != 256; ++i) {
      poke(0x1800 + page * 256 + i, peek(0x3800 + page * 256 + i));
    }
  }

  rom_setvid();
  rom_setkbd();
  s_step_delay = 0x52;
  s_difficulty = 0x01;
  s_demo_mode = true; // so the first pass plays itself
  s_script_index = 0x01;
  s_level_time = 0x64;

  /* $3783: JMP $76C2, which is three levels into the nest below rather than at
     the top of it. That is the one edge here that is not a loop edge, and it
     is deliberate: the demo pass skips $7691's and $76B7's initialisation, so
     it plays whatever level, score and lives the loaded image carries.

     Both outer loops are therefore written with their initialisation at the
     *bottom*, which is where the jumps that used to reach $7691 and $76B7 fall
     out to. Entering from the top then skips it exactly once, on this pass,
     with no flag to say so. The price is that $76B7's two lines appear twice:
     $7691 falls into $76B7, so restarting the game must do both. */

  for (;;) { // a new game
    for (;;) { // a new level
      s_progress.quota[1] = 0x00; // $76C2

      // How the round below ended. Every way out of the life loop sets it.
      RoundEnd ending = ROUND_RETRY;

      for (;;) { // one attempt at the round -- $76C7
        start_round();

        /* $7716: JMP $7739 -- a fresh round asks the same question a finished
           life does, so the first life of the round enters this loop at the
           top and every later one arrives from the bottom. */
        for (;;) { // one life
          ending = handle_life_result();
          if (ending != ROUND_CONTINUE) {
            break;
          }

          /* $7719 -- another life on the same round */
          s_life_timer = s_life_time;
          s_mon.wndtop = 0x14;
          rom_home();
          game_status_panel();
          if (s_step_delay >= 0x03) {
            // $7730 -- two steps faster each life, but never past 3.
            s_step_delay = s_step_delay - 2;
          }
          game_play_one_life();
        }

        if (ending != ROUND_RETRY) {
          break;
        }
      }

      if (ending == ROUND_GAME_OVER) {
        break;
      }

      /* $76B7 -- $77EA fell into this */
      s_progress.lives_at_level_start = s_progress.lives;
      s_progress.quota[0] = 0x10;
    }

    /* $7691 */
    game_setup_screen();
    game_promote_high_score();
    s_script_index = 0x01;
    s_progress.level = 0x01;
    s_progress.score[0] = 0x00;
    s_progress.score[1] = 0x00;
    s_progress.score[2] = 0x00;
    s_progress.score[3] = 0x00;
    s_progress.lives = 0x02;
    s_progress.afield[0] = 0x00;

    /* $76B7 again -- $7691 falls into it, and the level loop's own copy above
       is out of reach from here. */
    s_progress.lives_at_level_start = s_progress.lives;
    s_progress.quota[0] = 0x10;
  }
}

/* ========================================================================== *
 * The machine state at $3750                                               *
 * ========================================================================== */

/* Generated by make-entry-state.sh, and included rather than pasted: until
   2026-08-30 this file carried a reformatted copy of the whole thing, so
   regenerating the snapshot changed a header the build did not read and the
   copy here went on being the real one. Nothing said so. */
#include "entry-state-inc.h"

/* ========================================================================== *
 * Startup                                                                  *
 * ========================================================================== */

void init_emulated(void);

void init_emulated(void) {
  /* Loads the game image at $3750-$854E, and the $00B1-$00C8 zero-page segment
     the run data carries. */
  load_images();

  /* Unwritten RAM reads as $FF on a real machine, and the game does read some
     of it: game_rand_byte walks a pointer up from $1800 looking for a
     byte with bit 7 clear, and against a zero-filled machine it would find one
     immediately and never move. The generated init zeroes, so restore $FF --
     but only outside the game image, which is already in place above.

     $1800-$1FFF is included: $3750's first act is to copy the level data over
     it, so what is there beforehand does not matter, and $FF is what the
     machine would have had. */
  for (unsigned a = 0x0803; a != 0x3750; ++a)
    ram_poke(a, 0xFF);
  for (unsigned a = 0x854F; a != 0xC000; ++a)
    ram_poke(a, 0xFF);

  /* Then the state the boot itself produced: zero page, the stack and its live
     contents, the $03xx vectors, and the text screen. This comes last because
     it must win over both of the above. */
  for (unsigned i = 0; i != SB_ENTRY_RAM_LEN; ++i)
    ram_poke(i, kSnakeByteEntryRam[i]);

  /* The plotter's block used to be $0000-$0008 and is a C object now, so the
     line above no longer initialises it. Take it from the same snapshot, or
     the first read before any write would see zero where the booting build
     sees what BASIC left there. */
  s_snake.shape = kSnakeByteEntryRam[0x00];
  s_mon.ch = kSnakeByteEntryRam[0x24];
  s_mon.cv = kSnakeByteEntryRam[0x25];
  s_mon.wndlft = kSnakeByteEntryRam[0x20];
  s_mon.wndwdth = kSnakeByteEntryRam[0x21];
  s_mon.wndtop = kSnakeByteEntryRam[0x22];
  s_mon.wndbtm = kSnakeByteEntryRam[0x23];
  s_mon.gbas = kSnakeByteEntryRam[0x26] | (kSnakeByteEntryRam[0x27] << 8);
  s_mon.bas = kSnakeByteEntryRam[0x28] | (kSnakeByteEntryRam[0x29] << 8);
  s_mon.bas2 = kSnakeByteEntryRam[0x2a] | (kSnakeByteEntryRam[0x2b] << 8);
  s_mon.h2 = kSnakeByteEntryRam[0x2c];
  s_mon.v2 = kSnakeByteEntryRam[0x2d];
  s_mon.mask = kSnakeByteEntryRam[0x2e];
  s_mon.color = kSnakeByteEntryRam[0x30];
  s_mon.invflg = kSnakeByteEntryRam[0x32];
  s_mon.csw = kSnakeByteEntryRam[0x36] | (kSnakeByteEntryRam[0x37] << 8);
  s_mon.ksw = kSnakeByteEntryRam[0x38] | (kSnakeByteEntryRam[0x39] << 8);
  s_mon.a2l = kSnakeByteEntryRam[0x3e];

  /* Registers. SP matters most -- the live stack bytes above are meaningless
     without it. The flags are $A0: N set, everything else clear.

     The carry is loaded with the rest even though nothing reads it before a
     ROM routine overwrites it: this block is the machine's state at $3750, and
     leaving one member out to save a line would make it something else. The
     ten *residue* carry writes -- game routines setting it because the
     original left it set -- are a different thing and are gone. */
  // No registers are loaded: there are none left, and the emulated stack went
  // with them -- every PHA/PLA in this file bracketed a call that could not
  // reach the C local it was protecting.

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
