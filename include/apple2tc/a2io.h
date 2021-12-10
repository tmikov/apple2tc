/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "apple2tc/soundqueue.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// A helper function to decode the text mode screen. The callback is invoked
/// consecutively with every character starting from top left.
void apple2_decode_text_screen(
    const uint8_t *pageStart,
    void *ctx,
    void (*drawGlyph)(void *ctx, uint8_t ch, unsigned x, unsigned y));

#define A2_SCREEN_W 280
#define A2_SCREEN_H 192
#define A2_SCREEN_W_POT 512
#define A2_SCREEN_H_POT 256

typedef struct a2_rgba8 {
  uint8_t r, g, b, a;
} a2_rgba8;

/// RGB encoding of the Apple2 screen.
typedef struct a2_screen {
  a2_rgba8 data[A2_SCREEN_W_POT * A2_SCREEN_H_POT];
} a2_screen;

/// Render the text page pointed by pageStart into a RGB screen.
/// \param ms - millisecond since hardware reset. This is used to determine the
///     blink phase.
void apple2_render_text_screen(const uint8_t *pageStart, a2_screen *screen, uint64_t ms);

/// Render the lowres graphics (GR) page pointed by pageStart into a RGB screen.
/// \param ms - millisecond since hardware reset. This is used to determine the
///     blink phase. Only needed if mixed == true.
void apple2_render_gr_screen(const uint8_t *pageStart, a2_screen *screen, uint64_t ms, bool mixed);

/// Render the hires graphics (GR) page pointed by pageStart into a RGB screen.
/// \param textPageStart - the start of the text page. Only used if mixed == true.
/// \param ms - millisecond since hardware reset. This is used to determine the
///     blink phase. Only needed if mixed == true.
void apple2_render_hgr_screen(
    const uint8_t *grPageStart,
    const uint8_t *textPageStart,
    a2_screen *screen,
    uint64_t ms,
    bool mixed,
    bool mono);

typedef struct {
  /// Queue of float sound samples. Filled by the main thread, read by the
  /// sound callback.
  sound_queue_t sq;

  /// On web the sound callback may not activate until the user interacts
  /// with the page. We keep track of that to avoid filling the sound queue.
  atomic_bool cb_running;

  /// The cycle count for the previous speaker access. This allows us
  /// to detect cycle overflow/wraparound.
  unsigned last_cycle;
  /// After cycle wraparound, this keeps the high part that has been "lost".
  uint64_t cycle_base;
  /// The last speaker cycle we generated sound for.
  double last_generated_cycle;
  /// The last speaker state.
  float last_state;
} a2_sound_t;

void a2_sound_init(a2_sound_t *sound);
void a2_sound_free(a2_sound_t *sound);

/// The speaker bit has been accessed at cycle \p cycle.
void a2_sound_spkr(a2_sound_t *sound, unsigned cpu_freq, unsigned audio_rate, unsigned cycle);
/// Submit the sound generated up to cycle \p cycle.
void a2_sound_submit(a2_sound_t *sound, unsigned cpu_freq, unsigned audio_rate, unsigned cycle);
/// The asynchronous sound callback. Needs to populate the specified buffer with samples.
void a2_sound_cb(a2_sound_t *sound, float *buffer, unsigned num_frames, unsigned num_channels);

#ifdef __cplusplus
}
#endif
