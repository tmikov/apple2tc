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
void a2_sound_done(a2_sound_t *sound);

/// The speaker bit has been accessed at cycle \p cycle.
void a2_sound_spkr(a2_sound_t *sound, unsigned cpu_freq, unsigned audio_rate, unsigned cycle);
/// Submit the sound generated up to cycle \p cycle.
void a2_sound_submit(a2_sound_t *sound, unsigned cpu_freq, unsigned audio_rate, unsigned cycle);
/// The asynchronous sound callback. Needs to populate the specified buffer with samples.
void a2_sound_cb(a2_sound_t *sound, float *buffer, unsigned num_frames, unsigned num_channels);

enum {
  A2_KBD_QUEUE_SIZE = 32,
  A2_DEBUG_IO1 = 1,
  A2_DEBUG_IO2 = 2,

  /// When set, text, otherwise graphics (default set).
  A2_VC_TEXT = 1,
  /// When set, mixed graphics with 4-lines of text below (default clr).
  A2_VC_MIXED = 2,
  /// When set, display page 2 (default clr).
  A2_VC_PAGE2 = 4,
  /// When set, high resolution graphics (default clr),
  A2_VC_HIRES = 8,
};

typedef enum { A2_VIDMODE_TEXT, A2_VIDMODE_GR, A2_VIDMODE_HGR } a2_vidmode_t;

typedef struct {
  // Input keyboard queue.
  uint8_t keys[A2_KBD_QUEUE_SIZE];
  unsigned keys_head;
  unsigned keys_count;
  /// The last key that was returned.
  uint8_t last_key;
  /// Video control status.
  uint8_t vid_control;
  /// Callback when speaker is accessed.
  void *spkr_cb_ctx;
  void (*spkr_cb)(void *ctx, unsigned cycles);
  /// Debug flags.
  uint8_t debug;
} a2_iostate_t;

void a2_io_init(a2_iostate_t *io);
void a2_io_done(a2_iostate_t *io);
static inline void a2_io_set_spkr_cb(
    a2_iostate_t *io,
    void *spkr_cb_ctx,
    void (*spkr_cb)(void *ctx, unsigned cycles)) {
  io->spkr_cb_ctx = spkr_cb_ctx;
  io->spkr_cb = spkr_cb;
}
void a2_io_push_key(a2_iostate_t *io, uint8_t key);
uint8_t a2_io_peek(a2_iostate_t *io, uint16_t addr, unsigned cycles);
void a2_io_poke(a2_iostate_t *io, uint16_t addr, uint8_t value, unsigned cycles);

/// Decode and return the vid control bits as a video mode.
static inline a2_vidmode_t a2_io_get_vidmode(const a2_iostate_t *io) {
  return io->vid_control & A2_VC_TEXT ? A2_VIDMODE_TEXT
      : io->vid_control & A2_VC_HIRES ? A2_VIDMODE_HGR
                                      : A2_VIDMODE_GR;
}

/// Return true if this is a mixed graphics mode (graphics with 4 lines of
/// text in the bottom).
static inline bool a2_io_is_vidmode_mixed(const a2_iostate_t *io) {
  return (io->vid_control & (A2_VC_TEXT | A2_VC_MIXED)) == A2_VC_MIXED;
}

/// Decode the vid ctrl bits to return an actuve page number: 0 or 1.
static inline uint8_t a2_io_get_vidpage_index(const a2_iostate_t *io) {
  return (io->vid_control & A2_VC_PAGE2) ? 1 : 0;
}

/// Return the starting offset of the active Hires page.
uint16_t a2_io_get_hires_page_offset(const a2_iostate_t *io);
/// Return the starting address of the active text page.
uint16_t a2_io_get_text_page_offset(const a2_iostate_t *io);

#ifdef __cplusplus
}
#endif
