/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <string>

namespace a2mcp {

/// Resolve \p rel, an agent-supplied path, against `options().root` and
/// confirm it stays inside it. Throws ToolError on any failure: an escape, a
/// missing file, or (when \p for_writing is false) a file that is not a
/// regular file. Never lets a bad path reach code that might exit() -- see the
/// header comment in mcp_machine.cpp.
std::string jail_path(const std::string &rel, bool for_writing);

} // namespace a2mcp
