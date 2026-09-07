/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "mcp_server.h"

#include "apple2tc/a2host_api.h"

#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

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

  // Opened before serve() so the first scheduled key of the session lands in
  // the file, and unconditionally on --keys-out= rather than folded into the
  // engine whitelist below: it is this front end's own option, not one
  // a2host_parse_args knows about.
  if (!opts.keys_out.empty())
    a2host_open_scheduled_keys_file(opts.keys_out.c_str());

  // A whitelist, not a pass-through: a2host_parse_args owns --trace,
  // --trace-keys, --probe-dump and --help, all of which write to stdout, which
  // carries JSON-RPC and nothing else. Building the vector here means an
  // option can only reach the library if this file names it.
  // Before a2host_parse_args, which is what acts on --probe-out= and must be
  // able to override this. Not part of the whitelist below because it is not
  // an option: --probe= is forwarded, and probe_out() defaults to *stdout*, so
  // without this a script's printf would land in the middle of the JSON-RPC
  // stream -- the one hole the whitelist cannot close, since it is the one
  // a2mcp deliberately opens.
  a2host_probe_output_to_stderr();

  std::vector<std::string> forwarded;
  forwarded.push_back("a2mcp");
  if (!opts.probe.empty())
    forwarded.push_back("--probe=" + opts.probe);
  if (!opts.probe_out.empty())
    forwarded.push_back("--probe-out=" + opts.probe_out);
  if (!opts.hash_frames.empty())
    forwarded.push_back("--hash-frames=" + opts.hash_frames);
  std::vector<char *> argp;
  for (auto &s : forwarded)
    argp.push_back(const_cast<char *>(s.c_str()));
  // Exits on a bad script, which is correct: it happens before the first
  // request is served, and the client sees the diagnostic on stderr.
  a2host_parse_args((int)argp.size(), argp.data());

  // Unbuffered stdin would defeat getline; stdout is flushed per reply in
  // serve(). Nothing else may write to stdout -- see the header comment.
  const int rc = a2mcp::serve(std::cin, std::cout);
  // hash_file_ is closed only here; Task 6's diff against a2run's own
  // --hash-frames output needs the tail flushed, not just whatever
  // a2host_record_frame() wrote mid-run.
  a2host_shutdown();
  return rc;
}
