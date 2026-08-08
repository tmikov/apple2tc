/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The console front end: batch replay with no window, no graphics stack and
/// no audio.
///
/// A separate executable from the GUI one rather than a flag on it. On Windows
/// console versus GUI is a link-time subsystem property -- a GUI binary has
/// nowhere to write frame hashes unless it allocates a console, and a console
/// binary pops one up when launched from Explorer. No runtime flag bridges
/// that, so the two are built separately and share everything through
/// `lib/a2host`.

#include "apple2tc/a2host_api.h"

int main(int argc, char *argv[]) {
  a2host_parse_args(argc, argv);
  // Requires --frames, and does not return.
  a2host_run_headless();
  return 0;
}
