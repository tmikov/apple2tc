/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

// Same reasoning as mcp_screen.cpp: a2io.h pulls in soundqueue.h, which uses
// <atomic> in its C++ branch, so it must not be wrapped in an extern "C"
// block here -- it already brackets its own declarations correctly.
#include "apple2tc/a2io.h"

#include <string>

namespace a2mcp {

/// The text-mode screen as 24 newline-separated lines of 40 characters each.
/// Trailing spaces are kept so column positions survive.
std::string screen_text(void);

/// The lo-res (GR) screen as a 40x48 grid of hex colour digits (40x40 in
/// mixed mode), followed by a blank line, a colour legend, and -- in mixed
/// mode -- the bottom four text lines.
std::string screen_gr(void);

/// The current screen (whatever mode it is in) rendered to a PNG, at a fixed
/// blink phase so that a paused machine's screenshot is reproducible.
/// \param hgr_mode - which of the a2_hgr_mode_t renderers to use for a HGR
///     page. Ignored (but harmless to pass) in TEXT and GR mode.
/// \param scale - output pixels per colour cell for A2_HGR_COLOR140 and
///     A2_HGR_MONO140: 1 for the native 140x192 image, 2 for the 280x192
///     image doubled to match A2_HGR_COLOR (and everything else). Ignored
///     (but harmless to pass) for every other mode, and for TEXT and GR --
///     none of those have cells, so there is nothing for `scale` to mean.
std::string screen_png(a2_hgr_mode_t hgr_mode, unsigned scale);

/// Standard base64 encoding (RFC 4648), with '=' padding.
std::string base64(const std::string &bytes);

} // namespace a2mcp
