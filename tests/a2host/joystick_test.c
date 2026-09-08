/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The virtual joystick's policy, which is the half of the feature that can
/// be tested. The binding layer above it is sokol events and has no harness.

#include "../../lib/a2host/joystick.h"

#include "apple2tc/a2io.h"

#include <stdio.h>
#include <string.h>

static int failures;

static void check(bool ok, const char *what) {
  printf("%s: %s\n", ok ? "ok  " : "FAIL", what);
  if (!ok)
    ++failures;
}

/// A held direction rails its axis; releasing centres it.
static void test_dir_rails_and_centres(void) {
  a2host_joy_t j;
  a2host_joy_init(&j);

  check(a2host_joy_paddle(&j, 0) == A2_PADDLE_CENTER, "an untouched axis is centred");

  a2host_joy_dir(&j, A2HOST_JOYDIR_RIGHT, true);
  check(a2host_joy_paddle(&j, 0) == 255, "holding Right rails the X axis high");
  a2host_joy_dir(&j, A2HOST_JOYDIR_RIGHT, false);
  check(a2host_joy_paddle(&j, 0) == A2_PADDLE_CENTER, "releasing it centres again");

  a2host_joy_dir(&j, A2HOST_JOYDIR_LEFT, true);
  check(a2host_joy_paddle(&j, 0) == 0, "holding Left rails it low");
  a2host_joy_dir(&j, A2HOST_JOYDIR_LEFT, false);

  a2host_joy_dir(&j, A2HOST_JOYDIR_DOWN, true);
  check(a2host_joy_paddle(&j, 1) == 255, "Down rails the Y axis high");
  check(a2host_joy_paddle(&j, 0) == A2_PADDLE_CENTER, "and leaves X alone");
  a2host_joy_dir(&j, A2HOST_JOYDIR_DOWN, false);

  a2host_joy_dir(&j, A2HOST_JOYDIR_UP, true);
  check(a2host_joy_paddle(&j, 1) == 0, "Up rails it low");
}

/// Both opposite keys held is centred, not last-press-wins. Rolling a thumb
/// from one to the other holds both briefly; last-press-wins survives that
/// but leaves the axis railed when they are released in the other order.
static void test_opposite_dirs_centre(void) {
  a2host_joy_t j;
  a2host_joy_init(&j);

  a2host_joy_dir(&j, A2HOST_JOYDIR_LEFT, true);
  a2host_joy_dir(&j, A2HOST_JOYDIR_RIGHT, true);
  check(a2host_joy_paddle(&j, 0) == A2_PADDLE_CENTER, "both horizontal keys held is centred");

  a2host_joy_dir(&j, A2HOST_JOYDIR_LEFT, false);
  check(a2host_joy_paddle(&j, 0) == 255, "releasing Left leaves Right railing it");
}

/// reset() must clear the held-key booleans, not merely the derived axis.
/// The sequence is the one a lost window focus produces: the KEY_UP for a key
/// released outside the window never arrives.
static void test_reset_clears_held_keys(void) {
  a2host_joy_t j;
  a2host_joy_init(&j);

  a2host_joy_dir(&j, A2HOST_JOYDIR_LEFT, true);
  a2host_joy_reset(&j); // focus lost; Left is released outside the window
  check(a2host_joy_paddle(&j, 0) == A2_PADDLE_CENTER, "reset centres the axis");

  a2host_joy_dir(&j, A2HOST_JOYDIR_RIGHT, true);
  check(a2host_joy_paddle(&j, 0) == 255, "and a later Right rails right, not centre");
  a2host_joy_dir(&j, A2HOST_JOYDIR_RIGHT, false);
  check(a2host_joy_paddle(&j, 0) == A2_PADDLE_CENTER, "releasing it centres, not rails left");

  a2host_joy_button(&j, A2HOST_JOYBTN_SPACE, true);
  a2host_joy_reset(&j);
  check(a2host_joy_pbmask(&j) == 0, "reset releases the buttons too");
}

/// Default mapping: space->PB0, alt->PB1, ctrl->PB2.
static void test_default_button_mapping(void) {
  a2host_joy_t j;
  a2host_joy_init(&j);

  a2host_joy_button(&j, A2HOST_JOYBTN_SPACE, true);
  check(a2host_joy_pbmask(&j) == 0x1, "Space drives PB0 by default");
  a2host_joy_button(&j, A2HOST_JOYBTN_SPACE, false);

  a2host_joy_button(&j, A2HOST_JOYBTN_ALT, true);
  check(a2host_joy_pbmask(&j) == 0x2, "Alt drives PB1");
  a2host_joy_button(&j, A2HOST_JOYBTN_ALT, false);

  a2host_joy_button(&j, A2HOST_JOYBTN_CTRL, true);
  check(a2host_joy_pbmask(&j) == 0x4, "Ctrl drives PB2");
}

/// Neptune's override, and the aggregation it exposes. --joy-buttons=space:1
/// leaves Alt on its default of PB1, so two host buttons drive one input:
/// releasing either must not clear it while the other is held.
static void test_button_remap_and_or(void) {
  a2host_joy_t j;
  a2host_joy_init(&j);
  check(a2host_joy_parse_buttons(&j, "space:1,alt:0"), "the Neptune mapping parses");

  a2host_joy_button(&j, A2HOST_JOYBTN_SPACE, true);
  check(a2host_joy_pbmask(&j) == 0x2, "Space now drives PB1, which is Neptune's fire");
  a2host_joy_button(&j, A2HOST_JOYBTN_SPACE, false);
  a2host_joy_button(&j, A2HOST_JOYBTN_ALT, true);
  check(a2host_joy_pbmask(&j) == 0x1, "and Alt drives PB0");
  a2host_joy_button(&j, A2HOST_JOYBTN_ALT, false);

  a2host_joy_init(&j);
  check(a2host_joy_parse_buttons(&j, "space:1"), "a partial mapping parses");
  a2host_joy_button(&j, A2HOST_JOYBTN_SPACE, true);
  a2host_joy_button(&j, A2HOST_JOYBTN_ALT, true);
  check(a2host_joy_pbmask(&j) == 0x2, "Space and Alt both on PB1 is one held button");
  a2host_joy_button(&j, A2HOST_JOYBTN_SPACE, false);
  check(a2host_joy_pbmask(&j) == 0x2, "releasing one leaves PB1 held by the other");
  a2host_joy_button(&j, A2HOST_JOYBTN_ALT, false);
  check(a2host_joy_pbmask(&j) == 0, "releasing both clears it");
}

/// Malformed mappings are rejected rather than half-applied.
static void test_button_remap_rejects_junk(void) {
  a2host_joy_t j;
  a2host_joy_init(&j);
  check(!a2host_joy_parse_buttons(&j, "space:3"), "a PB number above 2 is rejected");
  check(!a2host_joy_parse_buttons(&j, "mouse:1"), "an unknown button name is rejected");
  check(!a2host_joy_parse_buttons(&j, "space"), "a missing colon is rejected");
  check(!a2host_joy_parse_buttons(&j, ""), "an empty mapping is rejected");

  a2host_joy_button(&j, A2HOST_JOYBTN_SPACE, true);
  check(a2host_joy_pbmask(&j) == 0x1, "and a rejected mapping left the defaults alone");
}

int main(void) {
  test_dir_rails_and_centres();
  test_opposite_dirs_centre();
  test_reset_clears_held_keys();
  test_default_button_mapping();
  test_button_remap_and_or();
  test_button_remap_rejects_junk();

  if (failures) {
    printf("\n%d joystick check(s) failed\n", failures);
    return 1;
  }
  printf("\njoystick: all checks passed\n");
  return 0;
}
