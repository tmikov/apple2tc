/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstdio>
#include <optional>

/// Read a file into a container T. T is usually std::string for text files
/// and std::vector<uint8_t> for binary files.
template <class T>
T readAll(FILE *f) {
  T buf;
  static constexpr unsigned CHUNK = 8192;
  for (;;) {
    size_t size = buf.size();
    buf.resize(size + CHUNK);
    size_t nr = fread(&buf[size], 1, CHUNK, f);
    buf.resize(size + nr);
    if (nr != CHUNK)
      break;
  }
  return buf;
}
