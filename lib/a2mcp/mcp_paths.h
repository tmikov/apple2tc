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
/// confirm it names an existing regular file inside it. Throws ToolError on an
/// escape, a missing path, or a non-regular file. Never lets a bad path reach
/// code that might exit() -- see the header comment in mcp_machine.cpp.
///
/// Split from jail_new_file() rather than taking a flag, because the two are
/// not the same check wearing different hats: this one resolves the whole
/// path, that one can only resolve a prefix of it.
std::string jail_existing_file(const std::string &rel);

/// Resolve \p rel for *writing*: the file need not exist, so only its
/// directory has to. If the final component already exists it is resolved too,
/// so an existing symlink there is judged by where it points rather than by
/// where it sits -- writing through such a link writes wherever it aims.
/// Throws ToolError on an escape or a missing directory.
std::string jail_new_file(const std::string &rel);

/// Write \p bytes to \p rel, jailed by jail_new_file(). The one way this
/// server creates a file: every caller gets the same jail and the same error
/// wording, and both name \p rel -- the path the agent actually asked for, not
/// the absolute one it has no way to recognise.
void write_jailed_file(const std::string &rel, const std::string &bytes);

} // namespace a2mcp
