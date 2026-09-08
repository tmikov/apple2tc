/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "joystick.h"

#include "apple2tc/a2io.h"

#include <stdlib.h>
#include <string.h>

/// The --joy-buttons= vocabulary. Names, not sokol keycodes: a front end maps
/// its own keys onto these, which is what keeps this library sokol-free.
static const char *const kButtonNames[A2HOST_JOYBTN_COUNT] = {"space", "alt", "ctrl"};

void a2host_joy_init(a2host_joy_t *j) {
  memset(j, 0, sizeof(*j));
  for (unsigned i = 0; i < A2HOST_JOYBTN_COUNT; ++i)
    j->btnPB[i] = (uint8_t)i;
}

bool a2host_joy_parse_buttons(a2host_joy_t *j, const char *spec) {
  // Parsed into a copy so a malformed entry half way through leaves the
  // caller's mapping untouched rather than partly rewritten.
  uint8_t parsed[A2HOST_JOYBTN_COUNT];
  memcpy(parsed, j->btnPB, sizeof(parsed));

  if (!spec || !*spec)
    return false;

  for (const char *p = spec; *p;) {
    const char *colon = strchr(p, ':');
    if (!colon)
      return false;

    size_t nameLen = (size_t)(colon - p);
    unsigned btn = A2HOST_JOYBTN_COUNT;
    for (unsigned i = 0; i < A2HOST_JOYBTN_COUNT; ++i) {
      if (strlen(kButtonNames[i]) == nameLen && memcmp(p, kButtonNames[i], nameLen) == 0) {
        btn = i;
        break;
      }
    }
    if (btn == A2HOST_JOYBTN_COUNT)
      return false;

    char *end = NULL;
    unsigned long pb = strtoul(colon + 1, &end, 10);
    if (end == colon + 1 || pb > 2)
      return false;
    parsed[btn] = (uint8_t)pb;

    if (*end == 0)
      break;
    if (*end != ',')
      return false;
    p = end + 1;
    if (*p == 0)
      return false; // a trailing comma has no entry after it to be lenient about
  }

  memcpy(j->btnPB, parsed, sizeof(parsed));
  return true;
}

void a2host_joy_dir(a2host_joy_t *j, a2host_joydir_t dir, bool down) {
  // Cast before comparing: GCC and Clang give an all-non-negative enum an
  // unsigned underlying type, so a negative argument would already be
  // rejected there, but MSVC's enums are `int`, and an unchecked negative
  // would index dirHeld[-1].
  if ((unsigned)dir < A2HOST_JOYDIR_COUNT)
    j->dirHeld[dir] = down;
}

void a2host_joy_button(a2host_joy_t *j, a2host_joybtn_t btn, bool down) {
  if ((unsigned)btn < A2HOST_JOYBTN_COUNT)
    j->btnHeld[btn] = down;
}

void a2host_joy_reset(a2host_joy_t *j) {
  memset(j->dirHeld, 0, sizeof(j->dirHeld));
  memset(j->btnHeld, 0, sizeof(j->btnHeld));
}

uint8_t a2host_joy_paddle(const a2host_joy_t *j, unsigned axis) {
  bool low = axis == 0 ? j->dirHeld[A2HOST_JOYDIR_LEFT] : j->dirHeld[A2HOST_JOYDIR_UP];
  bool high = axis == 0 ? j->dirHeld[A2HOST_JOYDIR_RIGHT] : j->dirHeld[A2HOST_JOYDIR_DOWN];

  // Subtraction rather than last-press-wins: rolling a thumb from one key to
  // the other holds both for a moment, and last-press-wins then leaves the
  // axis railed in a direction nobody is pressing once they are released in
  // the other order.
  int dir = (high ? 1 : 0) - (low ? 1 : 0);
  return dir > 0 ? 255 : dir < 0 ? 0 : (uint8_t)A2_PADDLE_CENTER;
}

uint8_t a2host_joy_pbmask(const a2host_joy_t *j) {
  // OR, not assignment: --joy-buttons=space:1 leaves alt on its default of
  // PB1, so two host buttons drive one input and releasing either must not
  // clear it while the other is held.
  uint8_t mask = 0;
  for (unsigned i = 0; i < A2HOST_JOYBTN_COUNT; ++i)
    if (j->btnHeld[i])
      mask |= (uint8_t)(1u << j->btnPB[i]);
  return mask;
}
