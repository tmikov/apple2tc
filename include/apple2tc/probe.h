/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// Probes: small programs that run at chosen points in the emulated program,
/// so two implementations can be compared without a shared clock. See
/// `docs/plans/2026-08-11-probes-design.md`.
///
/// This is the whole surface outside `lib/a2host`. The engines call
/// `probe_dispatch()`; the host calls everything else.

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Compile a script. Reports to stderr and exits non-zero on any error: a
/// probe that silently fails to load would leave a report that reads as
/// agreement.
void probe_load_script(const char *path);

/// Where `printf` output goes; stdout until set. probe.c owns the file and
/// closes it on an orderly exit -- see `probe_close_output`.
void probe_set_output_path(const char *path);

/// Close whatever `probe_set_output_path` opened. Safe to call even if it
/// never was; a no-op against stdout.
void probe_close_output(void);

/// Print the compiled form of the loaded script and return. Used by
/// `--probe-dump` and by every compiler test.
void probe_dump(FILE *f);

/// True once a script with at least one installed site is loaded. The engine
/// consults this to decide whether it needs per-instruction callbacks.
bool probe_installed(void);

/// Run whatever is installed at \p pc, returning immediately if nothing is.
/// The overwhelmingly common case is no site at this address.
void probe_dispatch(uint16_t pc);

/// Report probes that never fired, to stderr. A probe bound to an address that
/// does not exist in one of the two programs would otherwise pass quietly.
void probe_report_unfired(void);

#ifdef __cplusplus
} // extern "C"
#endif
