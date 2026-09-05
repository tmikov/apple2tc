/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <nlohmann/json.hpp>

namespace a2mcp {

/// Boot the emulated machine, or reboot it if it is already running. Every
/// path in \p args is jailed to the server root and confirmed to exist before
/// it reaches the engine -- see mcp_paths.h. Throws ToolError on a bad path or
/// argument without ever letting the engine exit() the process.
void machine_boot(const nlohmann::json &args);

/// True once machine_boot() has succeeded at least once.
bool machine_booted(void);

/// The machine's current state. Throws ToolError if it has not been booted.
nlohmann::json machine_status(void);

/// Advance emulated time by \p args["frames"] frames, or until a probe's
/// `stop` or the engine itself ends the run early. Throws ToolError if it has
/// not been booted, or on a bad argument.
nlohmann::json machine_run(const nlohmann::json &args);

} // namespace a2mcp
