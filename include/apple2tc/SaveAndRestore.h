/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <algorithm>

template <typename T>
class SaveAndRestore {
  T &x_;
  T oldValue_;

public:
  explicit SaveAndRestore(T &x) : x_(x), oldValue_(x) {}
  explicit SaveAndRestore(T &x, T newValue) : x_(x), oldValue_(std::move(x)) {
    x = std::move(newValue);
  }
  SaveAndRestore(const SaveAndRestore &) = delete;
  void operator=(const SaveAndRestore &) = delete;
  ~SaveAndRestore() {
    x_ = std::move(oldValue_);
  }
};

template <typename T>
inline auto makeSaveAndRestore(T &x) {
  return SaveAndRestore<T>(x);
}
template <typename T>
inline auto makeSaveAndRestore(T &x, T newValue) {
  return SaveAndRestore<T>(x, newValue);
}
