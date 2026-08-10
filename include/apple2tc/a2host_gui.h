/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The windowed front end's API, for a program that wants a window *and*
/// something of its own on top of it.
///
/// Separate from `a2host_api.h` because this one is unavoidably sokol-flavoured
/// and that one must stay sokol-free -- a console front end links the host
/// without a graphics stack at all.

#pragma once

#include "apple2tc/sokol/sokol_app.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Offered every event before the host handles it. Return true to consume it.
///
/// This is how a front end adds keys of its own without forking the host's
/// event handling -- a2emu uses it for F1/F2.
typedef bool (*a2host_gui_event_hook)(const sapp_event *ev);
void a2host_gui_set_event_hook(a2host_gui_event_hook hook);

#ifdef __cplusplus
} // extern "C"
#endif
