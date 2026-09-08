/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The one test of a2io's soft switches.
///
/// It exists because of a bug that a call count would not have caught: for
/// nearly five years `a2_io_poke()` performed *two* accesses per write, both
/// stamped with the same cycle. Every soft switch a2io implements is
/// idempotent except two, and both of those broke.
///
/// The speaker is the loud one. `a2_sound_spkr()` emits samples up to `cycle`
/// at the current level and then inverts the level, so a second call at the
/// same cycle emits nothing and inverts straight back: the level never moves,
/// the output is a DC constant, and a program driving the speaker with
/// `STA $C030` -- dsk/Neptune.dsk, for one -- is silent.
///
/// So the assertion here is on the *waveform*, not on how many callbacks fired.
/// A read and a write to $C030 are the same soft-switch access, so they must
/// produce the same samples; and those samples must be a square wave of the
/// period the toggles asked for. Suppressing the second callback while leaving
/// the level inverted would satisfy a call count and still fail this.

#include "apple2tc/a2io.h"
#include "apple2tc/apple2iodefs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// Apple II cycles per second, and the audio rate we render at.
enum { CPU_FREQ = 1020484, AUDIO_RATE = 44100 };

/// A 1 kHz square wave: 200 toggles, one every 510 cycles, the way a game
/// spins a delay loop around `STA $C030`.
enum { TOGGLES = 200, TOGGLE_CYCLES = 510, FIRST_CYCLE = 1000 };

/// Samples we render and compare. 4096 samples is ~94.8k cycles, which is
/// inside the ~102k cycles the toggles span, so the buffer is full of wave.
enum { SAMPLES = 4096 };

/// Cycles per audio sample; a half period of the wave is this many samples.
#define SAMPLES_PER_TOGGLE ((double)TOGGLE_CYCLES * AUDIO_RATE / CPU_FREQ)

static int failures;

static void check(bool ok, const char *what) {
  printf("%s: %s\n", ok ? "ok  " : "FAIL", what);
  if (!ok)
    ++failures;
}

static unsigned g_spkr_calls;

static void spkr_cb(void *ctx, unsigned cycles) {
  ++g_spkr_calls;
  a2_sound_spkr((a2_sound_t *)ctx, CPU_FREQ, AUDIO_RATE, cycles);
}

/// The shape of a rendered waveform, which is what we actually assert on.
typedef struct {
  unsigned edges;       ///< Sign changes: one per speaker toggle that landed.
  unsigned longest_run; ///< Longest stretch between two edges, in samples.
  unsigned calls;       ///< Speaker callbacks, for the record.
} waveform_t;

/// Toggle the speaker \c TOGGLES times through \p addr, either by reading it
/// (LDA $C030) or writing it (STA $C030), and render the result into \p buf.
static waveform_t render(bool write, float *buf) {
  a2_sound_t snd;
  a2_sound_init(&snd);
  // The sound engine only generates samples while the audio device is pulling
  // them, so pretend one is live before we start.
  float ignored[1];
  a2_sound_cb(&snd, ignored, 0, 1);

  a2_iostate_t io;
  a2_io_init(&io);
  a2_io_set_spkr_cb(&io, &snd, spkr_cb);

  g_spkr_calls = 0;
  unsigned cycles = FIRST_CYCLE;
  for (unsigned i = 0; i != TOGGLES; ++i, cycles += TOGGLE_CYCLES) {
    if (write)
      a2_io_poke(&io, A2_SPKR, 0, cycles);
    else
      a2_io_peek(&io, A2_SPKR, cycles);
  }
  a2_sound_submit(&snd, CPU_FREQ, AUDIO_RATE, cycles);

  memset(buf, 0, SAMPLES * sizeof(float));
  a2_sound_cb(&snd, buf, SAMPLES, 1);

  waveform_t w = {0, 0, g_spkr_calls};
  unsigned run = 1;
  for (unsigned i = 1; i != SAMPLES; ++i) {
    if ((buf[i] > 0) != (buf[i - 1] > 0)) {
      // The samples before the first edge are the lead-in -- the silence
      // between when the audio device came up and the first toggle -- and are
      // not part of the wave.
      if (w.edges && run > w.longest_run)
        w.longest_run = run;
      ++w.edges;
      run = 1;
    } else {
      ++run;
    }
  }

  a2_io_done(&io);
  a2_sound_done(&snd);
  return w;
}

static void test_speaker(void) {
  static float read_buf[SAMPLES];
  static float write_buf[SAMPLES];

  waveform_t r = render(false, read_buf);
  waveform_t w = render(true, write_buf);
  printf("  LDA $C030 x%u: callbacks=%u edges=%u longest run=%u samples\n",
         TOGGLES, r.calls, r.edges, r.longest_run);
  printf("  STA $C030 x%u: callbacks=%u edges=%u longest run=%u samples\n",
         TOGGLES, w.calls, w.edges, w.longest_run);

  // First establish that reading the switch produces the wave we asked for,
  // so that the equality below is an equality of something real. Every sample
  // in the buffer is inside the toggling, so the edge count is the buffer
  // length divided by the half period, give or take one at each end.
  const double expect_edges = SAMPLES / SAMPLES_PER_TOGGLE;
  check(
      r.edges + 2 >= (unsigned)expect_edges && r.edges <= (unsigned)expect_edges + 2,
      "reading $C030 produces one waveform edge per toggle");
  check(
      r.longest_run <= (unsigned)(SAMPLES_PER_TOGGLE + 2),
      "and holds each level for one half period, no longer");

  // The assertion this file exists for. A write to a soft switch is one
  // access, the same access a read makes, so it must produce the same samples.
  check(w.edges == r.edges, "writing $C030 produces the same number of edges as reading it");
  check(
      memcmp(read_buf, write_buf, sizeof(read_buf)) == 0,
      "and the same waveform, sample for sample");
}

static void test_kbdstrb(void) {
  a2_iostate_t io;
  a2_io_init(&io);

  // Two keys of type-ahead, then one STA $C010. The doubled access popped
  // both, so a burst of typing lost every second key.
  a2_io_push_key(&io, 'A');
  a2_io_push_key(&io, 'B');
  a2_io_poke(&io, A2_KBDSTRB, 0, 100);
  printf("  STA $C010 with 2 keys queued: %u left\n", a2_io_keys_count(&io));
  check(a2_io_keys_count(&io) == 1, "writing $C010 consumes exactly one key");
  check(a2_io_peek(&io, A2_KBD, 200) == ('B' | 0x80), "and the next key read is the second one");

  a2_io_done(&io);
}

/// The Apple II keyboard is one byte plus one flag, not a queue. The encoder
/// writes the new keycode into the byte and re-asserts the strobe on every
/// keypress, whether or not the program acknowledged the previous one. That is
/// what makes "read $C000, act on the keys you recognise, and clear the strobe
/// only for those" a working idiom, and games use it: dsk/Neptune.dsk steers
/// with I/J/K/M and its handler at $1060 reaches `STA $C010` on those four
/// arms only, leaving anything else sitting in the latch.
static void test_keyboard_latch(void) {
  a2_iostate_t io;
  a2_io_init(&io);

  a2_io_press_key(&io, 'J');
  check(a2_io_peek(&io, A2_KBD, 0) == ('J' | 0x80), "a pressed key reads with the strobe set");
  a2_io_poke(&io, A2_KBDSTRB, 0, 10);
  check(a2_io_peek(&io, A2_KBD, 20) == 'J', "and with it clear once acknowledged");

  // Neptune's shape. The game recognises J and acknowledges it, does not
  // recognise Q and leaves the strobe alone, and must still see the next J.
  a2_io_press_key(&io, 'Q');
  check(a2_io_peek(&io, A2_KBD, 30) == ('Q' | 0x80), "an unrecognised key is visible too");
  a2_io_press_key(&io, 'J');
  check(
      a2_io_peek(&io, A2_KBD, 40) == ('J' | 0x80),
      "and a press overwrites a key that was never acknowledged");

  a2_io_done(&io);
}

/// The queue is not hardware. It is staging for text typed *into* the machine
/// -- --kbd-file, a program's own startup keys -- where every character has to
/// arrive however slowly the program reads, instead of collapsing the way a
/// burst of real keypresses does.
static void test_type_ahead(void) {
  a2_iostate_t io;
  a2_io_init(&io);

  check(a2_io_push_str(&io, "HI"), "a string is staged whole");
  check(a2_io_peek(&io, A2_KBD, 0) == ('H' | 0x80), "its first character is in the latch");
  a2_io_poke(&io, A2_KBDSTRB, 0, 10);
  check(
      a2_io_peek(&io, A2_KBD, 20) == ('I' | 0x80),
      "and the next arrives when the program takes the first");
  a2_io_poke(&io, A2_KBDSTRB, 0, 30);
  check(a2_io_peek(&io, A2_KBD, 40) == 'I', "with nothing behind it");

  // A keypress is a hardware event: it lands in the latch now, over whatever
  // staged text was there. The host keeps the two apart -- a2host_gui.c
  // ignores live keystrokes while a key file is driving the run -- so this is
  // the defined outcome of a case that should not arise, not a supported mix.
  check(a2_io_push_str(&io, "XY"), "more text staged");
  a2_io_press_key(&io, 'Z');
  check(a2_io_peek(&io, A2_KBD, 50) == ('Z' | 0x80), "a press displaces staged text in the latch");
  a2_io_poke(&io, A2_KBDSTRB, 0, 60);
  check(a2_io_peek(&io, A2_KBD, 70) == ('Y' | 0x80), "and the rest of it follows");

  a2_io_done(&io);
}

/// The game port is not emulated and says nothing about it. Reads there used to
/// fall through a2_io_peek's default arm, which prints one line per access --
/// and a program polls the buttons every frame, so a run of dsk/Neptune.dsk
/// buried everything else on stderr under thousands of them.
///
/// This runs last and does not put stderr back: there is no portable way to
/// undo freopen(), and nothing after it writes there.
static void test_game_port(void) {
  static const char kTmp[] = "a2io-stderr.tmp";
  a2_iostate_t io;
  a2_io_init(&io);

  if (!freopen(kTmp, "w", stderr)) {
    check(false, "stderr could be captured");
    return;
  }
  bool all_zero = true;
  for (unsigned addr = A2_TAPEIN; addr != A2_PTRIG + 0x10; ++addr)
    if (a2_io_peek(&io, (uint16_t)addr, 0) != 0)
      all_zero = false;
  fflush(stderr);

  // Read back whatever landed there, so a failure can show it.
  char noise[256] = {0};
  long len = -1;
  FILE *f = fopen(kTmp, "rb");
  if (f) {
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    rewind(f);
    fread(noise, 1, sizeof(noise) - 1, f);
    fclose(f);
  }
  remove(kTmp);

  printf("  $C060-$C07F: %ld byte(s) on stderr\n", len);
  if (len > 0)
    printf("  first of it: %.60s\n", noise);
  check(all_zero, "the game port reads as no button held and a paddle at rest");
  check(len == 0, "and puts nothing on stderr");

  a2_io_done(&io);
}

/// The 32-bit cycle counter a2_io_peek() receives wraps about every 71
/// minutes of emulated time. The clock behind the paddle one-shots must not,
/// or an expired timer comes back to life. Walk the counter all the way
/// around its wrap in frame-sized steps -- the way a host ticks it -- and
/// check every step was counted.
static void test_clock_extension(void) {
  a2_iostate_t io;
  a2_io_init(&io);

  // One 1/60 s frame at 1.02 MHz, and enough of them to pass 2^32 cycles.
  enum { FRAME_CYCLES = 17030, FRAMES = 300000 };
  uint64_t expected = 0;
  unsigned stamp = 0;
  for (unsigned i = 0; i < FRAMES; ++i) {
    stamp += FRAME_CYCLES;
    expected += FRAME_CYCLES;
    a2_io_tick(&io, stamp);
  }

  printf("  clock: %llu, expected %llu\n",
      (unsigned long long)io.clock, (unsigned long long)expected);
  check(io.clock == expected, "the extended clock counts across the 32-bit wrap");
  check(expected > 0x100000000ull, "and the walk really did pass a wrap");

  a2_io_done(&io);

  // a2_io_tick() is the fallback for a program that touches no soft switch;
  // the soft-switch handlers themselves are the primary feeder of the clock,
  // and nothing above exercises that path. A fresh state, so this does not
  // interact with the wrap walk above.
  a2_iostate_t io2;
  a2_io_init(&io2);

  a2_io_peek(&io2, A2_KBD, 1000);
  check(io2.clock == 1000, "a peek at a soft switch advances the clock to its stamp");

  a2_io_peek(&io2, A2_KBD, 1510);
  check(io2.clock == 1510, "and a later peek advances it again");

  // a2_io_poke() tail-calls a2_io_peek() with the same stamp, so the clock
  // must land on that stamp exactly once -- landing on it twice would mean
  // the advance ran twice, which is the double-count this task guards against.
  a2_io_poke(&io2, A2_KBD, 0, 1600);
  check(io2.clock == 1600, "and a poke advances it to its stamp exactly once");

  a2_io_done(&io2);
}

int main(void) {
  test_speaker();
  test_kbdstrb();
  test_keyboard_latch();
  test_type_ahead();
  test_clock_extension();
  test_game_port();

  if (failures) {
    printf("\n%d a2io check(s) failed\n", failures);
    return 1;
  }
  printf("\na2io: all checks passed\n");
  return 0;
}
