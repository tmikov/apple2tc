/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void add_watch(uint16_t addr, uint8_t size);
void add_nondebug(uint16_t from, uint16_t to);
void add_default_nondebug(void);

#ifdef __cplusplus
}
#endif
