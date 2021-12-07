/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/a2io.h"

#include "font.h"

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
  /// Set if the display mode is mixed.
  bool mixed;
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

/// GR colors.
static const a2_rgba8 s_gr_colors[] = {
    {0, 0, 0}, // 0x0 black
    {227, 30, 96}, // 0x1 deep red
    {96, 78, 189}, // 0x2 dark blue
    {255, 68, 253}, // 0x3 purple
    {0, 163, 96}, // 0x4 dark green
    {156, 156, 156}, // 0x5 dark gray
    {20, 207, 253}, // 0x6 medium blue
    {208, 195, 255}, // 0x7 light blue
    {96, 114, 3}, // 0x8 brown
    {255, 106, 60}, // 0x9 orange
    {156, 156, 156}, // 0xa light gray
    {255, 160, 208}, // 0xb pink
    {20, 245, 60}, // 0xc green
    {208, 221, 141}, // 0xd yellow
    {114, 255, 208}, // 0xe aquamarine
    {255, 255, 255}, // 0xf white
};

static void draw_gr_cb(void *ctx, uint8_t ch, unsigned x, unsigned y) {
  struct RenderText *self = (struct RenderText *)ctx;

  if (y >= 20 && self->mixed) {
    draw_glyph_cb(ctx, ch, x, y);
    return;
  }

  a2_rgba8 *d = self->screen->data + y * A2_SCREEN_W_POT * 8 + x * 7;
  for (unsigned row = 0; row != 4; ++row) {
    for (unsigned col = 0; col != 7; ++col, ++d)
      *d = s_gr_colors[ch & 0x0F];
    d += A2_SCREEN_W_POT - 7;
  }
  ch >>= 4;
  for (unsigned row = 0; row != 4; ++row) {
    for (unsigned col = 0; col != 7; ++col, ++d)
      *d = s_gr_colors[ch];
    d += A2_SCREEN_W_POT - 7;
  }
}

void apple2_render_text_screen(const uint8_t *pageStart, a2_screen *screen, uint64_t ms) {
  struct RenderText ctx = {.screen = screen, .blinkOn = (ms / 267) & 1 ? 0x40 : 0};
  apple2_decode_text_screen(pageStart, &ctx, draw_glyph_cb);
}

void apple2_render_gr_screen(const uint8_t *pageStart, a2_screen *screen, uint64_t ms, bool mixed) {
  struct RenderText ctx = {.screen = screen, .blinkOn = (ms / 267) & 1 ? 0x40 : 0, .mixed = mixed};
  apple2_decode_text_screen(pageStart, &ctx, draw_gr_cb);
}

void apple2_render_hgr_screen(
    const uint8_t *grPageStart,
    const uint8_t *textPageStart,
    a2_screen *screen,
    uint64_t ms,
    bool mixed,
    bool mono) {
  // There are 8 1024B blocks. Block 0 starts at line 0, block 1 starts at line 1, etc.
  //
  // Each 1024KB block consists of 8 128B regions. Each region consists of 3 40B lines,
  // with 8 bytes extra at the end. Each region starts 8 lines after the previous one.
  //
  // The 3 lines in a region are spaced vertically across 64 screen lines.
  //
  // Example:
  // Block 0, region 7, line 2 would be at: 0 + 7 * 8 + 2 * 64 = screen line 184
  //
  // How to calculate offset of screen line scr_line?
  // block = scr_line % 8
  // rgn = (scr_line / 8) % 8
  // rgn_line = (scr_line / 8) / 8
  // offset = block * 1024 + rgn * 128 + rgn_line * 40;
  //    or
  // offset = (scr_line % 8) * 1024 + (scr_line / 8) % 8 * 128 + (scr_line / 64) * 40

  a2_rgba8 *srow = screen->data;
  unsigned end_line = mixed ? 160 : 192;
  for (unsigned scr_line = 0; scr_line != end_line; srow += A2_SCREEN_W_POT, ++scr_line) {
    const uint8_t *start =
        grPageStart + (scr_line % 8) * 1024 + ((scr_line / 8) % 8) * 128 + (scr_line / 64) * 40;

    a2_rgba8 *d = srow;
    if (mono) {
      const a2_rgba8 fg = {0xFF, 0xFF, 0xFF, 0};
      const a2_rgba8 bg = {0, 0, 0, 0};
      for (unsigned bcol = 0; bcol != 40; ++start, ++bcol) {
        uint8_t memb = *start;
        for (unsigned i = 0; i != 7; memb >>= 1, ++d, ++i) {
          *d = memb & 1 ? fg : bg;
        }
      }
    } else {
      // const orangeCol: Color = [255, 106, 60];
      // const greenCol: Color = [20, 245, 60];
      // const blueCol: Color = [20, 207, 253];
      // const violetCol: Color = [255, 68, 253];
      // const whiteCol: Color = [255, 255, 255];
      // const blackCol: Color = [0, 0, 0];
      const a2_rgba8 black = {0, 0, 0};
      const a2_rgba8 white = {0xFF, 0xFF, 0xFF};
      static a2_rgba8 colors[4] = {
          // Violet.
          {255, 68, 253},
          // Green.
          {20, 245, 60},
          // Blue.
          {20, 207, 253},
          // Red.
          {255, 106, 60},
      };
      uint8_t odd = 0;
      uint8_t last = 0;

      for (unsigned bcol = 0; bcol != 40; ++start, ++bcol) {
        uint8_t memb = *start;
        uint8_t highBit = (memb >> 6) & 2;
        for (unsigned i = 0; i != 7; memb >>= 1, ++d, ++i) {
          if ((memb & 1) == 0) {
            *d = black;
          } else {
            if (last)
              *d = white;
            else {
              *d = colors[highBit | odd];
            }
          }
          last = memb & 1;
          odd ^= 1;
        }
      }
    }
  }

  if (mixed) {
    struct RenderText ctx = {.screen = screen, .blinkOn = (ms / 267) & 1 ? 0x40 : 0};
    for (unsigned scr_line = 20; scr_line != 24; ++scr_line) {
      const uint8_t *start = textPageStart + (scr_line % 8) * 128 + (scr_line / 8) * 40;
      for (unsigned col = 0; col != 40; ++col, ++start) {
        draw_glyph_cb(&ctx, *start, col, scr_line);
      }
    }
  }
}
