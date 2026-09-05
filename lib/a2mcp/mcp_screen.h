/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <string>

namespace a2mcp {

/// The text-mode screen as 24 newline-separated lines of 40 characters each.
/// Trailing spaces are kept so column positions survive.
std::string screen_text(void);

/// The lo-res (GR) screen as a 40x48 grid of hex colour digits (40x40 in
/// mixed mode), followed by a blank line, a colour legend, and -- in mixed
/// mode -- the bottom four text lines.
std::string screen_gr(void);

} // namespace a2mcp
