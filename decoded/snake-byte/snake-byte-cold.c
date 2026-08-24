/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// Snake Byte started at its own entry point, $3750, instead of by booting the
/// Apple II and typing `CALL 14160`.
///
/// Why this exists
/// ---------------
/// The decompilation is rooted where the recording started: the reset vector.
/// That makes every ROM block the boot executes reachable code, which is why
/// the extern build carries 1,434 blocks of Applesoft against 79 of game. None
/// of it can be removed while the program still has to boot to reach the game.
///
/// This build starts at $3750. Nothing is deleted yet -- the boot blocks are
/// still in the file, simply never entered -- so the only difference from
/// snake-bytec1-ext is the entry point and the state it starts from. Removing
/// the now-unreachable blocks is the next step, and it is separable precisely
/// because this one changes nothing else.
///
/// What has to be installed, and why
/// ---------------------------------
/// A program entered at $3750 has not run the boot, so nothing has set up the
/// machine. The game reads that state without ever writing it -- most sharply
/// through $36/$37, which COUT dispatches through: unset, they are $FF and the
/// first character printed jumps into nothing.
///
/// entry-state-inc.h is that state, captured with --snapshot-at from
/// snake-bytec1-ext-run's own first arrival at $3750. It is $0000-$0802 of RAM,
/// the registers, and the video soft switch.
///
/// Note it is *not* decoded/rom/boot-state.txt, which is the machine at the
/// BASIC prompt with nothing typed. That is the right general reference for
/// what a boot produces, and the wrong state for this build: reaching $3750
/// means typing `CALL 14160`, which leaves 74 bytes of difference in
/// $0000-$07FF -- Applesoft's parse state, its stack frames, the input buffer
/// and the echoed command on the screen. Starting from the pristine state
/// diverges on the first screen sample. Measured, after trying it.
///
/// The game image itself is already in place: the generated init_emulated()
/// memcpy's it in, exactly as --preload does.
///
/// Input
/// -----
/// play.pkeys opens with the eleven keystrokes that type `CALL 14160` at the
/// prompt, which this build must not receive -- it is already past that point.
/// play-cold.pkeys is the same recording with those dropped and every
/// remaining stamp shifted down by the coordinate value at the first $3750,
/// which is a constant. See make-cold-keys.sh.

#define init_emulated snake_byte_cold_generated_init_emulated
#include "snake-byte-cold-body.c"
#undef init_emulated
#include "a2rom.c"
#include "game_native.c"
#include "game.c"

#include "apple2tc/a2host_api.h"
#include "apple2tc/a2io.h"
#include "entry-state-inc.h"

void init_emulated(void);

void init_emulated(void) {
  /* Loads the game image at $3750-$854E, and the $00B1-$00C8 zero-page segment
     the run data carries. */
  snake_byte_cold_generated_init_emulated();

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
