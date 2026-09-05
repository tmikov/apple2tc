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
#include <cstdio>
#include <cstdlib>
#include <string>

namespace a2mcp {
namespace {

/// realpath(\p path), reported against \p rel: an error message names the path
/// the agent supplied, never the resolved absolute one. The absolute path
/// would say where this server's root happens to live on this machine, which
/// is both a leak and, in a committed test baseline, a hardcoded checkout.
std::string real_or_throw(const std::string &path, const char *what, const std::string &rel) {
  char buf[PATH_MAX];
  if (!realpath(path.c_str(), buf))
    throw ToolError(std::string(what) + ": " + rel);
  return std::string(buf);
}

std::string real_root(void) {
  return real_or_throw(options().root, "server root is unreadable", options().root);
}

/// Prefix-compare on resolved paths, with the separator, so that a sibling
/// directory sharing a name prefix with the root is not accepted.
void confirm_inside(const std::string &resolved, const std::string &root, const std::string &rel) {
  if (resolved.compare(0, root.size(), root) != 0 ||
      (resolved.size() > root.size() && resolved[root.size()] != '/'))
    throw ToolError("path escapes the server root: " + rel);
}

} // namespace

std::string jail_existing_file(const std::string &rel) {
  const std::string root = real_root();
  const std::string resolved = real_or_throw(root + "/" + rel, "no such file", rel);
  struct stat st;
  if (stat(resolved.c_str(), &st) != 0 || !S_ISREG(st.st_mode))
    throw ToolError("not a readable file: " + rel);
  confirm_inside(resolved, root, rel);
  return resolved;
}

std::string jail_new_file(const std::string &rel) {
  const std::string root = real_root();

  // The file need not exist yet, so resolve its directory and re-attach the
  // final component. A "/" in that component would escape the resolved
  // directory, so it is rejected outright.
  const size_t slash = rel.find_last_of('/');
  const std::string dir = slash == std::string::npos ? std::string(".") : rel.substr(0, slash);
  const std::string base = slash == std::string::npos ? rel : rel.substr(slash + 1);
  if (base.empty() || base == "." || base == "..")
    throw ToolError("not a file name: " + rel);
  std::string resolved = real_or_throw(root + "/" + dir, "no such directory", rel) + "/" + base;

  // A resolved directory plus a literal name is not yet a resolved path: if
  // that name is a symlink, opening it for writing writes to its target, which
  // may be anywhere. lstat rather than stat, so a *dangling* link is seen as a
  // link too -- realpath() then fails on it and real_or_throw rejects it,
  // which is the right answer: a link whose target cannot be resolved cannot
  // be shown to be inside the root.
  struct stat st;
  if (lstat(resolved.c_str(), &st) == 0 && S_ISLNK(st.st_mode))
    resolved = real_or_throw(resolved, "cannot resolve the symlink at", rel);

  confirm_inside(resolved, root, rel);
  return resolved;
}

void write_jailed_file(const std::string &rel, const std::string &bytes) {
  const std::string path = jail_new_file(rel);
  FILE *f = fopen(path.c_str(), "wb");
  if (!f)
    throw ToolError("cannot write " + rel);
  const size_t n = fwrite(bytes.data(), 1, bytes.size(), f);
  const bool ok = fclose(f) == 0 && n == bytes.size();
  if (!ok)
    throw ToolError("short write to " + rel);
}

} // namespace a2mcp
