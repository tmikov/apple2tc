/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdint.h>

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

/// A cell on the 40x48 playfield grid.
typedef struct {
  uint8_t col;
  uint8_t row;
} Cell;

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
void game_plot_hline_native(void);
void game_plot_vline_native(void);

/// $7000 -- the lo-res half of a vertical run, restoring $03.
void game_lores_vline_native(void);

/// $6594 -- step the bouncers the difficulty calls for, then return the next
/// queued key.
uint8_t game_step_bouncers_native(void);

/// $60E7 -- draw the loaded shape into cell \p c in ink \p ink, replacing.
void game_draw_cell_native(uint8_t ink, Cell c);

/// $6B93 -- the same, merged into what is already there.
void game_merge_cell_native(uint8_t ink, Cell c);

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
void game_place_apple_native(void);

/// $71CD -- recompute what one apple is worth for the current level.
void game_set_apple_value_native(void);

/// $6BEF -- plot the head on the occupancy map and flag it as newly there.
void game_mark_head_native(void);

/// $6BDA -- draw a cell, merging the head shape over it if the head is on it.
void game_draw_head_native(void);

/// $7633 -- count one apple eaten, and make the noise for it.
void game_eat_apple_native(void);

/// $60E4 -- load a shape and draw it into the current cell.
void game_plot_shape_native(void);

/// $64A9 -- the rising-then-falling sweep an eaten apple makes.
void game_sound_sweep_native(void);

/// $7590 -- show \p key as slot \p slot's binding on the redefinition screen.
void game_show_key_native(uint8_t slot, uint8_t key);

/// $6B3D -- draw both side walls, and leave SCRN of the bottom-centre cell.
void game_draw_side_walls_native(void);
