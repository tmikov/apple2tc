/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "mcp_screen.h"

// Each header brackets its own declarations in `extern "C"` under
// `#ifdef __cplusplus`, so no wrapper is needed here -- and adding one would
// be actively wrong: a2io.h pulls in soundqueue.h, which uses <atomic> in its
// C++ branch, and C++ standard headers do not tolerate C linkage.
#include "apple2tc/a2engine.h"
#include "apple2tc/a2host_api.h"
#include "apple2tc/a2io.h"

namespace a2mcp {
namespace {

struct TextCtx {
  std::string out;
};

/// Screen codes to printable ASCII. Inverse and flashing render as the plain
/// character: an agent reading the screen wants the text, and no game observed
/// so far conveys anything by inverse alone that the words do not also say.
void draw_glyph(void *ctx, uint8_t ch, unsigned x, unsigned y) {
  (void)y;
  auto *tc = static_cast<TextCtx *>(ctx);
  uint8_t c = ch & 0x7F;
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

} // namespace a2mcp
