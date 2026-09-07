/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "mcp_server.h"
#include "mcp_tools.h"

#include <istream>
#include <ostream>
#include <string>

namespace a2mcp {
namespace {

Options s_options;

const char *const kProtocolVersion = "2025-06-18";

nlohmann::json error_reply(const nlohmann::json &id, int code, const std::string &msg) {
  return {{"jsonrpc", "2.0"}, {"id", id}, {"error", {{"code", code}, {"message", msg}}}};
}

nlohmann::json result_reply(const nlohmann::json &id, nlohmann::json result) {
  return {{"jsonrpc", "2.0"}, {"id", id}, {"result", std::move(result)}};
}

nlohmann::json handle_initialize(const nlohmann::json &params) {
  // Echo the client's version when we speak it, so a client pinned to an older
  // revision is not told to use a newer one it cannot parse.
  std::string version = kProtocolVersion;
  auto it = params.find("protocolVersion");
  if (it != params.end() && it->is_string() && *it == "2024-11-05")
    version = it->get<std::string>();
  return {
      {"protocolVersion", version},
      {"capabilities", {{"tools", nlohmann::json::object()}}},
      {"serverInfo", {{"name", "a2mcp"}, {"version", "1"}}}};
}

} // namespace

const Options &options(void) {
  return s_options;
}
void set_options(const Options &opts) {
  s_options = opts;
}

int serve(std::istream &in, std::ostream &out) {
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty())
      continue;

    nlohmann::json msg = nlohmann::json::parse(line, nullptr, false);
    if (msg.is_discarded()) {
      out << error_reply(nullptr, -32700, "parse error").dump() << "\n" << std::flush;
      continue;
    }

    // No id means a notification: act on it, answer nothing. Replying to one is
    // a protocol violation, not merely noise.
    const bool notification = msg.find("id") == msg.end();
    const nlohmann::json id = notification ? nlohmann::json(nullptr) : msg["id"];
    const std::string method = msg.value("method", std::string());
    const nlohmann::json params = msg.value("params", nlohmann::json::object());

    nlohmann::json reply;
    if (method == "initialize") {
      reply = result_reply(id, handle_initialize(params));
    } else if (method == "tools/list") {
      reply = result_reply(id, nlohmann::json{{"tools", tool_schemas()}});
    } else if (method == "tools/call") {
      const std::string name = params.value("name", std::string());
      const nlohmann::json args = params.value("arguments", nlohmann::json::object());
      try {
        reply = result_reply(id, call_tool(name, args));
      } catch (const ToolError &e) {
        reply = result_reply(
            id,
            nlohmann::json{
                {"content", {{{"type", "text"}, {"text", e.what()}}}}, {"isError", true}});
      }
    } else if (method == "ping") {
      reply = result_reply(id, nlohmann::json::object());
    } else {
      reply = error_reply(id, -32601, "method not found: " + method);
    }

    if (!notification)
      out << reply.dump() << "\n" << std::flush;
  }
  return 0;
}

} // namespace a2mcp
