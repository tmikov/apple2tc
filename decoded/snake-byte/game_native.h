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
