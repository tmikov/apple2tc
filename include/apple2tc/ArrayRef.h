/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cassert>
#include <initializer_list>
#include <vector>

template <class T>
class ArrayRef {
  const T *data_;
  size_t size_;

public:
  using iterator = const T *;

  /// Initialize with a single value.
  ArrayRef(const T &v) : data_(&v), size_(1) {}
  ArrayRef(const std::vector<T> &vec) : data_(vec.data()), size_(vec.size()) {}
  ArrayRef(const T *data, size_t size) : data_(data), size_(size) {}
  ArrayRef(std::initializer_list<T> init) : data_(init.begin()), size_(init.size()) {}

  size_t size() const {
    return size_;
  }
  bool empty() const {
    return size_ == 0;
  }
  iterator begin() const {
    return data_;
  }
  iterator end() const {
    return data_ + size_;
  }
  const T *data() const {
    return data_;
  }

  const T &operator[](size_t index) {
    assert(index < size_);
    return data_[index];
  }
  const T &at(size_t index) {
    assert(index < size_);
    return data_[index];
  }
};