/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/a2io.h"

#include "font.h"

#include <stdbool.h>

void apple2_decode_text_screen(
    const uint8_t *pageStart,
    void *ctx,
    void (*drawGlyph)(void *ctx, uint8_t ch, unsigned x, unsigned y)) {
  // The screen memory is interleaved. It is organized in eight 128-byte
  // regions, where every region contains three 40-byte lines (there are 8 extra
  // bytes remaining in the end of each region).
  // These three lines occupy every 8-th line vertically on the actual screen.

  // How to convert from a screen line to memory offset?
  // rgn = scr_line % 8;
  // rgn_line = scr_line / 8;
  // offset = rgn * 128 + rgn_line * 40;
  //
  // or simply:
  // offset = (scr_line % 8) * 128 + (scr_line / 8) * 40;

  for (unsigned scr_line = 0; scr_line != 24; ++scr_line) {
    const uint8_t *start = pageStart + (scr_line % 8) * 128 + (scr_line / 8) * 40;
    for (unsigned col = 0; col != 40; ++col, ++start) {
      drawGlyph(ctx, *start, col, scr_line);
    }
  }
}

struct RenderText {
  a2_screen *screen;
  /// 0 or 0x40.
  uint8_t blinkOn;
};

static void draw_glyph_cb(void *ctx, uint8_t ch, unsigned x, unsigned y) {
  struct RenderText *self = (struct RenderText *)ctx;
  bool inverse = !(ch & (0x80 | self->blinkOn));
  ch = (ch >= 0x40 && ch < 0x80) ? ch - 0x40 : ch;

  a2_rgba8 fg;
  a2_rgba8 bg;
  if (!inverse) {
    fg = (a2_rgba8){0xFF, 0xFF, 0xFF, 0};
    bg = (a2_rgba8){0, 0, 0, 0};
  } else {
    fg = (a2_rgba8){0, 0, 0, 0};
    bg = (a2_rgba8){0xFF, 0xFF, 0xFF, 0};
  }

  const uint8_t *glyph = font_rom + (ch & 0x3F) * 8;
  a2_rgba8 *d = self->screen->data + y * A2_SCREEN_W_POT * 8 + x * 7;
  for (unsigned row = 0; row != 8; ++glyph, ++row) {
    for (unsigned col = 0; col != 7; ++col, ++d) {
      *d = *glyph & (0x40 >> col) ? fg : bg;
    }
    d += A2_SCREEN_W_POT - 7;
  }
}

void apple2_render_text_screen(const uint8_t *pageStart, a2_screen *screen, uint64_t ms) {
  struct RenderText ctx = {.screen = screen, .blinkOn = (ms / 267) & 1 ? 0x40 : 0};
  apple2_decode_text_screen(pageStart, &ctx, draw_glyph_cb);
}
