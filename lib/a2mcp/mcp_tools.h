/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace a2mcp {

/// The array returned by tools/list.
nlohmann::json tool_schemas(void);

/// Run one tool. Throws ToolError on any failure the agent should see.
nlohmann::json call_tool(const std::string &name, const nlohmann::json &args);

} // namespace a2mcp
