/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "mcp_screen.h"
#include "mcp_server.h"

// Each header brackets its own declarations in `extern "C"` under
// `#ifdef __cplusplus`, so no wrapper is needed here -- and adding one would
// be actively wrong: a2io.h pulls in soundqueue.h, which uses <atomic> in its
// C++ branch, and C++ standard headers do not tolerate C linkage.
#include "apple2tc/a2engine.h"
#include "apple2tc/a2host_api.h"
#include "apple2tc/a2io.h"

#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_WRITE_NO_STDIO
#include "stb_image_write.h"

namespace a2mcp {
namespace {

struct TextCtx {
  std::string out;
};

/// Screen codes to printable ASCII. Inverse and flashing render as the plain
/// character: an agent reading the screen wants the text, and no game observed
/// so far conveys anything by inverse alone that the words do not also say.
///
/// Only the low SIX bits (0x3F) identify the glyph; bit 6 is a video
/// attribute (it distinguishes flashing from inverse) and carries no glyph
/// identity of its own, so it must be masked away rather than kept. This has
/// to match `draw_glyph_cb` in lib/a2io/a2io.c -- which indexes the font ROM
/// with `ch & 0x3F` -- or the text and image renderings of the same screen
/// would disagree with each other.
void draw_glyph(void *ctx, uint8_t ch, unsigned x, unsigned y) {
  (void)y;
  auto *tc = static_cast<TextCtx *>(ctx);
  uint8_t c = ch & 0x3F;
  if (c < 0x20)
    c += 0x40;
  tc->out.push_back((char)c);
  if (x == 39)
    tc->out.push_back('\n');
}

} // namespace

std::string screen_text(void) {
  TextCtx ctx;
  ctx.out.reserve(24 * 41);
  apple2_decode_text_screen(get_ram() + a2_io_get_text_page_offset(a2host_io()), &ctx, draw_glyph);
  return ctx.out;
}

namespace {

/// The standard Apple II lo-res palette, in nibble order.
const char *const kGrColours[16] = {
    "black",
    "magenta",
    "dark blue",
    "purple",
    "dark green",
    "grey 1",
    "medium blue",
    "light blue",
    "brown",
    "orange",
    "grey 2",
    "pink",
    "green",
    "yellow",
    "aqua",
    "white"};

} // namespace

std::string screen_gr(void) {
  const a2_iostate_t *io = a2host_io();
  const uint8_t *page = get_ram() + a2_io_get_text_page_offset(io);
  const bool mixed = a2_io_is_vidmode_mixed(io);
  // In mixed mode the bottom four text rows replace the last eight cell rows.
  const unsigned cell_rows = mixed ? 40 : 48;

  std::string out;
  out.reserve(cell_rows * 41 + 512);
  for (unsigned cell_y = 0; cell_y != cell_rows; ++cell_y) {
    const unsigned text_row = cell_y / 2;
    for (unsigned x = 0; x != 40; ++x) {
      // Same interleave as apple2_decode_text_screen() in lib/a2io/a2io.c
      // ((scr_line % 8) * 128 + (scr_line / 8) * 40): `& 7`/`>> 3` are just
      // `% 8`/`/ 8` for the unsigned, non-negative text_row here. Replicated
      // rather than routed through that function because it decodes whole
      // characters, which we would then have to un-pack back into nibbles.
      const unsigned offset = (text_row & 7) * 0x80 + (text_row >> 3) * 40 + x;
      const uint8_t b = page[offset];
      // draw_gr_cb() in lib/a2io/a2io.c paints `ch & 0x0F` (the low nibble)
      // into the upper of the pair's two cells first, then `ch >> 4` into the
      // lower one -- so the even cell_y (upper cell) takes the low nibble.
      const uint8_t nib = (cell_y & 1) ? (uint8_t)(b >> 4) : (uint8_t)(b & 0x0F);
      out.push_back("0123456789ABCDEF"[nib]);
    }
    out.push_back('\n');
  }

  out += "\nlo-res colours: ";
  for (unsigned i = 0; i != 16; ++i) {
    out.push_back("0123456789ABCDEF"[i]);
    out += "=";
    out += kGrColours[i];
    out += i == 15 ? "\n" : ", ";
  }

  if (mixed) {
    out += "\ntext (bottom four lines):\n";
    const std::string text = screen_text();
    // The last four of the 24 lines screen_text() produces.
    size_t pos = 0;
    for (unsigned i = 0; i != 20; ++i)
      pos = text.find('\n', pos) + 1;
    out += text.substr(pos);
  }
  return out;
}

namespace {

void png_sink(void *ctx, void *data, int size) {
  static_cast<std::string *>(ctx)->append(static_cast<const char *>(data), (size_t)size);
}

} // namespace

std::string screen_png(a2_hgr_mode_t hgr_mode) {
  const a2_iostate_t *io = a2host_io();
  const uint8_t *ram = get_ram();
  // Blink phase is a function of wall-clock time in the renderers, so a fixed
  // value is passed instead: a screenshot of a paused machine must not depend
  // on when it was taken.
  const uint64_t kBlinkMs = 0;

  static a2_screen screen;
  switch (a2_io_get_vidmode(io)) {
  case A2_VIDMODE_TEXT:
    apple2_render_text_screen(ram + a2_io_get_text_page_offset(io), &screen, kBlinkMs);
    break;
  case A2_VIDMODE_GR:
    apple2_render_gr_screen(
        ram + a2_io_get_text_page_offset(io), &screen, kBlinkMs, a2_io_is_vidmode_mixed(io));
    break;
  case A2_VIDMODE_HGR:
    apple2_render_hgr_screen_mode(
        ram + a2_io_get_hires_page_offset(io),
        ram + a2_io_get_text_page_offset(io),
        &screen,
        kBlinkMs,
        a2_io_is_vidmode_mixed(io),
        hgr_mode);
    break;
  }

  // Three channels, not four. a2_screen is "RGB encoding of the Apple2 screen"
  // (a2io.h) and its fourth byte is padding: every renderer in a2io.c builds
  // its pixels as {r, g, b, 0} and nothing ever fills that byte in. Handing
  // stb four channels therefore emits a PNG whose every pixel is alpha 0 --
  // structurally perfect, 280x192, deterministic, byte-identical between runs,
  // and completely invisible in any viewer that honours alpha. Repacking to
  // RGB is what makes the screenshot a picture rather than a transparent
  // rectangle.
  std::vector<uint8_t> rgb((size_t)A2_SCREEN_W * A2_SCREEN_H * 3);
  for (unsigned y = 0; y != A2_SCREEN_H; ++y) {
    const a2_rgba8 *src = screen.data + (size_t)y * A2_SCREEN_W_POT;
    uint8_t *dst = rgb.data() + (size_t)y * A2_SCREEN_W * 3;
    for (unsigned x = 0; x != A2_SCREEN_W; ++x) {
      *dst++ = src[x].r;
      *dst++ = src[x].g;
      *dst++ = src[x].b;
    }
  }

  std::string out;
  if (!stbi_write_png_to_func(
          png_sink, &out, A2_SCREEN_W, A2_SCREEN_H, 3, rgb.data(), A2_SCREEN_W * 3))
    throw ToolError("PNG encoding failed");
  return out;
}

std::string base64(const std::string &bytes) {
  static const char kAlpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  std::string out;
  out.reserve((bytes.size() + 2) / 3 * 4);
  size_t i = 0;
  for (; i + 2 < bytes.size(); i += 3) {
    const uint32_t v = (uint8_t)bytes[i] << 16 | (uint8_t)bytes[i + 1] << 8 | (uint8_t)bytes[i + 2];
    out.push_back(kAlpha[v >> 18 & 63]);
    out.push_back(kAlpha[v >> 12 & 63]);
    out.push_back(kAlpha[v >> 6 & 63]);
    out.push_back(kAlpha[v & 63]);
  }
  if (i != bytes.size()) {
    const bool two = bytes.size() - i == 2;
    const uint32_t v = (uint8_t)bytes[i] << 16 | (two ? (uint8_t)bytes[i + 1] << 8 : 0);
    out.push_back(kAlpha[v >> 18 & 63]);
    out.push_back(kAlpha[v >> 12 & 63]);
    out.push_back(two ? kAlpha[v >> 6 & 63] : '=');
    out.push_back('=');
  }
  return out;
}

} // namespace a2mcp
