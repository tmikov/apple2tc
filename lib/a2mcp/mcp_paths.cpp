/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "mcp_paths.h"
#include "mcp_server.h"

#include <sys/stat.h>
#include <climits>
#include <cstdlib>
#include <string>

namespace a2mcp {
namespace {

std::string real_or_throw(const std::string &path, const std::string &what) {
  char buf[PATH_MAX];
  if (!realpath(path.c_str(), buf))
    throw ToolError(what + ": " + path);
  return std::string(buf);
}

} // namespace

std::string jail_path(const std::string &rel, bool for_writing) {
  const std::string root = real_or_throw(options().root, "server root is unreadable");

  std::string resolved;
  if (for_writing) {
    // The file need not exist yet, so resolve its directory and re-attach the
    // final component. A "/" in that component would escape the resolved
    // directory, so it is rejected outright.
    size_t slash = rel.find_last_of('/');
    const std::string dir = slash == std::string::npos ? std::string(".") : rel.substr(0, slash);
    const std::string base = slash == std::string::npos ? rel : rel.substr(slash + 1);
    if (base.empty() || base == "." || base == "..")
      throw ToolError("not a file name: " + rel);
    resolved = real_or_throw(root + "/" + dir, "no such directory") + "/" + base;
  } else {
    resolved = real_or_throw(root + "/" + rel, "no such file");
    struct stat st;
    if (stat(resolved.c_str(), &st) != 0 || !S_ISREG(st.st_mode))
      throw ToolError("not a readable file: " + rel);
  }

  // Prefix-compare on the resolved paths, with the separator, so that a
  // sibling directory sharing a name prefix with the root is not accepted.
  if (resolved.compare(0, root.size(), root) != 0 ||
      (resolved.size() > root.size() && resolved[root.size()] != '/'))
    throw ToolError("path escapes the server root: " + rel);
  return resolved;
}

} // namespace a2mcp
