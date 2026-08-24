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

#include "game_native.h"

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
    ram_poke(0x3754, (uint8_t)(ram_peek(0x3754) + 1));
    ram_poke(0x3757, (uint8_t)(ram_peek(0x3757) + 1));
    if (page != 7)
      GAME_CYCLES(0x3766, 1);
  }

  GAME_CYCLES(0x3768, 6);
  rom_setvid(0x376a);
  GAME_CYCLES(0x376b, 6);
  rom_setkbd(0x376d);
  GAME_CYCLES(0x376e, 29);
  ram_poke(0x0300, 0x52); // step delay
  ram_poke(0x0301, 0x01); // difficulty
  ram_poke(0x0302, 0x01); // demo mode, so the first pass plays itself
  ram_poke(0x0303, 0x01); // level script index
  ram_poke(0x0304, 0x64); // apple value
  goto round;             // $3783: JMP $76C2

new_game: /* $7691 */
  GAME_CYCLES(0x7691, 6);
  game_setup(0x7693);
  GAME_CYCLES(0x7694, 6);
  game_update_high_score(0x7696);
  GAME_CYCLES(0x7697, 40);
  ram_poke(0x0303, 0x01);
  ram_poke(0x7265, 0x01); // level number, BCD
  ram_poke(0x7252, 0x00); // score, four BCD bytes
  ram_poke(0x7253, 0x00);
  ram_poke(0x7254, 0x00);
  ram_poke(0x7255, 0x00);
  ram_poke(0x725e, 0x02); // lives
  ram_poke(0x725f, 0x00);

new_level: /* $76B7 */
  GAME_CYCLES(0x76b7, 14);
  ram_poke(0x78b2, ram_peek(0x725e)); // game_bonus compares against this
  ram_poke(0x7263, 0x10);             // apples this round, BCD

round: /* $76C2 */
  GAME_CYCLES(0x76c2, 6);
  ram_poke(0x7264, 0x00);

start_round: /* $76C7 */
  GAME_CYCLES(0x76c7, 40);
  ram_poke(0x725f, 0x00);
  ram_poke(0x7260, 0x00);
  ram_poke(0x7261, 0x00);
  ram_poke(0x7262, 0x00);
  ram_poke(0x725a, ram_peek(0x7263));
  ram_poke(0x725b, ram_peek(0x7264));
  game_draw_playfield(0x76e3);
  GAME_CYCLES(0x76e4, 14);
  ram_poke(0x7266, ram_peek(0x0304));
  game_set_apple_value(0x76ec);
  GAME_CYCLES(0x76ed, 14);
  io_peek(0xc054);       // page 1
  s_a = io_peek(0xc053); // mixed text/graphics
  game_place_apple(0x76f5);
  GAME_CYCLES(0x76f6, 6);
  game_plot_shape(0x76f8);
  GAME_CYCLES(0x76f9, 8);
  ram_poke(0x0300, 0x52);
  s_a = 0x00;
  GAME_CYCLES(0x7700, 23);
  ram_poke(0x0305, 0x00);
  ram_poke(0x6255, ram_peek(0x7266));
  ram_poke(0x0022, 0x14); // window top, so HOME clears only the status panel
  rom_home(0x770f);
  GAME_CYCLES(0x7710, 6);
  game_draw_status(0x7712);
  GAME_CYCLES(0x7713, 6);
  game_start_round(0x7715);
  GAME_CYCLES(0x7716, 3);
  goto verdict; // $7716: JMP $7739 -- a fresh round asks the same question

life: /* $7719 */
  GAME_CYCLES(0x7719, 19);
  ram_poke(0x6255, ram_peek(0x7266));
  ram_poke(0x0022, 0x14);
  rom_home(0x7725);
  GAME_CYCLES(0x7726, 6);
  game_draw_status(0x7728);
  GAME_CYCLES(0x7729, 8);
  if (ram_peek(0x0300) >= 0x03) {
    // $7730 -- two steps faster each life, but never past 3.
    GAME_CYCLES(0x7730, 8);
    ram_poke(0x0300, (uint8_t)(ram_peek(0x0300) - 2));
  } else {
    GAME_CYCLES(0x772e, 1);
  }
  GAME_CYCLES(0x7736, 6);
  game_play_loop(0x7738);

verdict: /* $7739 -- $6253 says how the life ended */
  GAME_CYCLES(0x7739, 8);
  if (ram_peek(0x6253) == 0x0f)
    goto ate_apple;
  GAME_CYCLES(0x7740, 3);
  GAME_CYCLES(0x77e6, 4);
  if (ram_peek(0x6253) != 0x00)
    goto not_apple;
  goto round_cleared;

ate_apple: /* $773E */
  GAME_CYCLES(0x773e, 1);
  GAME_CYCLES(0x7743, 76);
  s_status_d = 0x01;
  bcd_sub16_at(0x725f, 0x7260, 0x01); // apples until the next one appears
  bcd_sub16_at(0x725a, 0x725b, 0x01); // apples left in the round
  bcd_add16_at(0x7261, 0x7262, 0x01); // apples eaten
  s_status_d = 0x00;

  // $777B -- points only for the first $110 apples.
  if (ram_peek(0x7262)) {
    GAME_CYCLES(0x777b, 1);
  } else {
    GAME_CYCLES(0x777d, 8);
    if (ram_peek(0x7261) >= 0x11) {
      GAME_CYCLES(0x7782, 1);
    } else {
      GAME_CYCLES(0x7784, 6);
      game_add_score(0x7786);
    }
  }

  GAME_CYCLES(0x7787, 18);
  ram_poke(0x6254, (uint8_t)(ram_peek(0x6254) + 0x0a)); // ten more cells of snake

  // $7793 -- anything left in the round?
  if (ram_peek(0x725a)) {
    GAME_CYCLES(0x7793, 1);
    goto next_apple;
  }
  GAME_CYCLES(0x7795, 6);
  if (ram_peek(0x725b)) {
    GAME_CYCLES(0x7798, 1);
    goto next_apple;
  }

  /* $779A -- that was the last one. Draw the bar across the bottom and put
     the marker on it, then blank the apple value so the next round re-reads
     it from $0304. */
  GAME_CYCLES(0x779a, 31);
  ram_poke(0x0001, 0x06);
  ram_poke(0x0003, 0x00);
  ram_poke(0x0002, 0x12);
  ram_poke(0x0008, 0x16);
  ram_poke(0x0000, 0x15);
  game_plot_hline(0x77b0);
  GAME_CYCLES(0x77b1, 16);
  ram_poke(0x0001, 0x00);
  ram_poke(0x0002, 0x14);
  game_plot_shape(0x77bb);
  GAME_CYCLES(0x77bc, 14);
  ram_poke(0x7266, 0xff);
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
  if (ram_peek(0x725f)) {
    GAME_CYCLES(0x77d5, 3);
    goto life;
  }
  GAME_CYCLES(0x77d3, 1);
  GAME_CYCLES(0x77d8, 6);
  if (ram_peek(0x7260)) {
    GAME_CYCLES(0x77dd, 3);
    goto life;
  }
  GAME_CYCLES(0x77db, 1);
  GAME_CYCLES(0x77e0, 6);
  game_place_apple(0x77e2);
  GAME_CYCLES(0x77e3, 3);
  goto life;

round_cleared: /* $77EA */
  GAME_CYCLES(0x77ea, 32);
  {
    const uint16_t r = adc_dec16(ram_peek(0x7265), 0x01, 0x00); // level, BCD
    ram_poke(0x7265, (uint8_t)r);
  }
  s_status_d = 0x00;
  ram_poke(0x0303, (uint8_t)(ram_peek(0x0303) + 1)); // next level script
  // $77F8 -- no life was lost this round, so it earns a bonus.
  if (ram_peek(0x725e) == ram_peek(0x78b2)) {
    GAME_CYCLES(0x7800, 6);
    game_bonus(0x7802);
  } else {
    GAME_CYCLES(0x77fe, 1);
  }
  GAME_CYCLES(0x7803, 6);
  game_eat_apple(0x7805);
  GAME_CYCLES(0x7806, 3);
  goto new_level;

not_apple: /* $77E8 */
  GAME_CYCLES(0x77e8, 1);
  GAME_CYCLES(0x7809, 4);
  if (ram_peek(0x6253) != 0xfe) {
    GAME_CYCLES(0x780b, 1);
    goto ended;
  }
  GAME_CYCLES(0x780d, 6);
  if (ram_peek(0x725b)) {
    GAME_CYCLES(0x7810, 1);
    goto harder;
  }
  GAME_CYCLES(0x7812, 6);
  if (!ram_peek(0x725a)) {
    GAME_CYCLES(0x7815, 1);
    goto ended;
  }

harder: /* $7817 -- three more apples in the round, and three more to come */
  GAME_CYCLES(0x7817, 54);
  s_status_d = 0x01;
  bcd_add16_at(0x7263, 0x7264, 0x03);
  bcd_add16_at(0x725a, 0x725b, 0x03);
  s_status_d = 0x00;
  game_place_apple(0x783d);
  GAME_CYCLES(0x783e, 6);
  game_place_apple(0x7840);
  GAME_CYCLES(0x7841, 6);
  game_place_apple(0x7843);
  GAME_CYCLES(0x7844, 3);
  goto life;

ended: /* $7847 */
  GAME_CYCLES(0x7847, 8);
  if (ram_peek(0x6253) == 0xff) {
    GAME_CYCLES(0x784e, 3);
    goto new_game; // the player pressed the quit key
  }
  GAME_CYCLES(0x784c, 1);
  GAME_CYCLES(0x7851, 4);
  // $FE means the snake ran out of room rather than died, and that just starts
  // another life. Anything else falls through to the pause. Note the sense:
  // the original *branches away* when it is not $FE, so equality is the
  // fall-through, not the exception.
  if (ram_peek(0x6253) == 0xfe) {
    GAME_CYCLES(0x7855, 3);
    goto life;
  }
  GAME_CYCLES(0x7853, 1);
  GAME_CYCLES(0x7858, 6);
  if (ram_peek(0x0302)) {
    GAME_CYCLES(0x785b, 1);
    goto lose_life; // the demo does not wait to be told to carry on
  }

  /* $785D -- "PRESS SPACE BAR TO CONTINUE", then wait for space or the
     paddle button, whichever the setup screen selected. */
  GAME_CYCLES(0x785d, 16);
  ram_poke(0x0025, 0x17);
  ram_poke(0x0024, 0x00);
  game_print_inline_str(0x7867);
  for (;;) {
    GAME_CYCLES(0x7886, 6);
    if (ram_peek(0x6c71)) {
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
  if (!ram_peek(0x725e)) {
    GAME_CYCLES(0x789f, 3);
    goto new_game;
  }
  GAME_CYCLES(0x789d, 1);
  GAME_CYCLES(0x78a2, 19);
  {
    const uint16_t r = sbc_dec16(ram_peek(0x725e), 0x01, 0x01);
    ram_poke(0x725e, (uint8_t)r);
  }
  s_status_d = 0x00;
  goto start_round;
}
