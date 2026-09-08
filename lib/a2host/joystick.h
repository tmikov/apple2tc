/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The virtual joystick's policy: what a held arrow key means, and which
/// pushbutton a host button drives.
///
/// A plain struct with pure functions over it, deliberately: no statics, no
/// a2io, no sokol. The device is analog and lives in a2io; the decision that
/// an arrow key means "rail" is here; the decision that the Left arrow key is
/// the one that does it belongs to a front end. Keeping this layer free of
/// the other two is what lets it be tested at all.

#pragma once

#include "apple2tc/a2host_api.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  /// Which direction keys are down. Held state lives here rather than in the
  /// front end so a2host_joy_reset() can reach it -- see the enum's comment.
  bool dirHeld[A2HOST_JOYDIR_COUNT];
  /// Which host buttons are down.
  bool btnHeld[A2HOST_JOYBTN_COUNT];
  /// Which pushbutton (0-2) each host button drives. Two host buttons may
  /// legitimately name the same one.
  uint8_t btnPB[A2HOST_JOYBTN_COUNT];
} a2host_joy_t;

/// Nothing held, and the conventional mapping: space->PB0, alt->PB1,
/// ctrl->PB2.
void a2host_joy_init(a2host_joy_t *j);

/// Apply a `--joy-buttons=` mapping such as "space:1,alt:0". Unlisted buttons
/// keep whatever they had. Returns false and changes nothing on malformed
/// input.
bool a2host_joy_parse_buttons(a2host_joy_t *j, const char *spec);

/// Press or release a direction key.
void a2host_joy_dir(a2host_joy_t *j, a2host_joydir_t dir, bool down);

/// Press or release a host button.
void a2host_joy_button(a2host_joy_t *j, a2host_joybtn_t btn, bool down);

/// Release every direction key and every button.
void a2host_joy_reset(a2host_joy_t *j);

/// The position for paddle \p axis: 0 for axis 0 (X, from left/right) and 1
/// for axis 1 (Y, from up/down). Digital and self-centring: a rail or the
/// centre, never anything between.
uint8_t a2host_joy_paddle(const a2host_joy_t *j, unsigned axis);

/// The pushbutton mask, bit n set while PBn is held. Each bit is the OR of
/// every host button mapped to it.
uint8_t a2host_joy_pbmask(const a2host_joy_t *j);
