/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <nlohmann/json.hpp>
#include <iosfwd>
#include <stdexcept>
#include <string>

namespace a2mcp {

/// Thrown by a tool to report a failure to the agent. The server turns it into
/// an MCP result with isError set -- never a JSON-RPC error, which is reserved
/// for protocol-level faults the agent cannot act on.
class ToolError : public std::runtime_error {
public:
  explicit ToolError(const std::string &what) : std::runtime_error(what) {}
};

struct Options {
  std::string root;
  std::string probe;
  std::string probe_out;
  std::string keys_out;
  std::string hash_frames;
};

const Options &options(void);
void set_options(const Options &opts);

/// Read newline-delimited JSON-RPC from \p in, write replies to \p out, until
/// EOF. Returns a process exit code.
int serve(std::istream &in, std::ostream &out);

} // namespace a2mcp
