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
/// \param blend - 1 (default) renders the live page exactly as before. In
///     HGR only, 2 to kBlendMaxFrames instead composites the last `blend`
///     frames machine_run() has captured into mcp_machine.h's blend ring
///     with a bitwise OR and renders that -- a dot lit in any of them is lit
///     in the result, which is what persistence of vision does for a human
///     watching the real screen. OR is meaningful only for a bitmap, so
///     outside HGR `blend` is accepted and ignored, the same precedent
///     `render` and `scale` already set.
/// \param out_frames_used - if non-null, set to how many frames the
///     returned image actually represents: 1 whenever `blend` does not
///     apply (outside HGR, or `blend <= 1`), otherwise
///     screen_blend_composite()'s return -- `blend` itself, or fewer right
///     after the ring was reset.
std::string
screen_png(a2_hgr_mode_t hgr_mode, unsigned scale, unsigned blend, unsigned *out_frames_used);

/// Standard base64 encoding (RFC 4648), with '=' padding.
std::string base64(const std::string &bytes);

} // namespace a2mcp
