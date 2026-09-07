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

    // `msg` need not be an object even when it parsed: a bare `42` or `[1]` is
    // valid JSON. find()/value() on such a value either quietly returns "not
    // found" (find) or throws type_error.306 (value) depending on which one
    // is used below, so the method is pulled out with find()+is_string()
    // rather than msg.value("method", ...): the latter would throw on this
    // same non-object input, outside any try block, and take the process
    // down before dispatch even begins.
    const bool notification = msg.find("id") == msg.end();
    const nlohmann::json id = notification ? nlohmann::json(nullptr) : msg["id"];
    const auto method_it = msg.find("method");
    const std::string method = (method_it != msg.end() && method_it->is_string())
        ? method_it->get<std::string>()
        : std::string();
    // Safe for the same reason: msg.value("params", ...) would throw if msg
    // is not an object, regardless of the requested type, because nlohmann's
    // value() checks that before it ever looks at the key.
    const nlohmann::json params =
        msg.is_object() ? msg.value("params", nlohmann::json::object()) : nlohmann::json::object();

    nlohmann::json reply;
    if (method == "initialize") {
      reply = result_reply(id, handle_initialize(params));
    } else if (method == "tools/list") {
      reply = result_reply(id, nlohmann::json{{"tools", tool_schemas()}});
    } else if (method == "tools/call") {
      // `name` is pulled out the same defensive way as `method` above, so it
      // is available for the catch below even when `params` itself is
      // malformed (not an object, or "name" not a string) -- the case that
      // throws before call_tool() is ever reached.
      std::string name;
      if (const auto name_it = params.find("name"); name_it != params.end() && name_it->is_string())
        name = name_it->get<std::string>();
      try {
        const nlohmann::json args = params.is_object()
            ? params.value("arguments", nlohmann::json::object())
            : nlohmann::json::object();
        reply = result_reply(id, call_tool(name, args));
      } catch (const ToolError &e) {
        reply = result_reply(
            id,
            nlohmann::json{
                {"content", {{{"type", "text"}, {"text", e.what()}}}}, {"isError", true}});
      } catch (const nlohmann::json::exception &e) {
        // Every typed accessor in every tool (args.value(...), it->get<T>(),
        // etc.) throws nlohmann::json::type_error/out_of_range on a
        // malformed argument -- e.g. `render: 42`, a number where a string
        // is expected. Those are base-classed under nlohmann::json::exception
        // and, unlike ToolError, are not raised on purpose, so there is no
        // hand-written message to preserve: report the tool and the fact
        // that its arguments were malformed, same isError shape as above,
        // and keep serving.
        reply = result_reply(
            id,
            nlohmann::json{
                {"content",
                 {{{"type", "text"},
                   {"text", "tool \"" + name + "\": malformed arguments: " + e.what()}}}},
                {"isError", true}});
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
