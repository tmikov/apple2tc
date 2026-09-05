/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "mcp_server.h"

#include <cstdio>
#include <cstring>
#include <iostream>

namespace {

void usage(void) {
  fprintf(
      stderr,
      "a2mcp: an MCP server driving the Apple II headless.\n"
      "\n"
      "  --root=<dir>        directory every agent-supplied path is confined to\n"
      "  --probe=<path>      probe script, compiled once at startup\n"
      "  --probe-out=<path>  probe report\n"
      "  --keys-out=<path>   write a cycle-stamped .keys file of the session\n"
      "  --hash-frames=<p>   per-frame video hashes, as a2run writes them\n");
}

bool prefix(const char *arg, const char *p, std::string *out) {
  size_t n = strlen(p);
  if (strncmp(arg, p, n) != 0)
    return false;
  *out = arg + n;
  return true;
}

} // namespace

int main(int argc, char *argv[]) {
  a2mcp::Options opts;
  for (int i = 1; i != argc; ++i) {
    const char *arg = argv[i];
    if (strcmp(arg, "--help") == 0) {
      usage();
      return 0;
    }
    if (prefix(arg, "--root=", &opts.root) || prefix(arg, "--probe=", &opts.probe) ||
        prefix(arg, "--probe-out=", &opts.probe_out) ||
        prefix(arg, "--keys-out=", &opts.keys_out) ||
        prefix(arg, "--hash-frames=", &opts.hash_frames))
      continue;
    fprintf(stderr, "a2mcp: unknown option \"%s\"\n", arg);
    usage();
    return 2;
  }
  if (opts.root.empty()) {
    fprintf(stderr, "a2mcp: --root=<dir> is required\n");
    return 2;
  }
  a2mcp::set_options(opts);
  // Unbuffered stdin would defeat getline; stdout is flushed per reply in
  // serve(). Nothing else may write to stdout -- see the header comment.
  return a2mcp::serve(std::cin, std::cout);
}
