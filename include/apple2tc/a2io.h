/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

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

#ifdef __cplusplus
}
#endif
