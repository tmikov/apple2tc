/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/probe.h"

#include "probe_internal.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// Where `probe_dispatch`'s eventual `printf` output goes. NULL means
/// "unset", not "closed" -- `probe_out()` is what maps that to stdout.
static FILE *s_out = NULL;

static FILE *probe_out(void) {
  return s_out ? s_out : stdout;
}

/// Every fatal diagnostic that involves opening a file goes through here, so
/// each one remembers *why* the open failed -- "no such file" and "permission
/// denied" are different bugs. (a2host.c has its own FATAL messages for
/// option validation, which have no errno to report and so have no use for
/// this helper.)
static void probe_fatal(const char *what, const char *path) {
  fprintf(stderr, "FATAL: %s '%s': %s\n", what, path, strerror(errno));
  exit(2);
}

void probe_load_script(const char *path) {
  FILE *f = fopen(path, "rt");
  if (!f)
    probe_fatal("cannot open probe script", path);
  fclose(f);
}

void probe_set_output_path(const char *path) {
  FILE *f = fopen(path, "wt");
  if (!f)
    probe_fatal("cannot open probe output", path);
  probe_close_output();
  s_out = f;
}

void probe_close_output(void) {
  if (s_out) {
    fclose(s_out);
    s_out = NULL;
  }
}

void probe_dump(FILE *f) {
  fprintf(f, "counters: 0\n");
  fprintf(f, "probes: 0\n");
  fprintf(f, "sites: 0\n");
}

bool probe_installed(void) {
  return false;
}

void probe_dispatch(uint16_t pc) {
  (void)pc;
}

void probe_report_unfired(void) {}
