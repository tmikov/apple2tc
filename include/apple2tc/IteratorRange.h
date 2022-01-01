/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

template <class A, class B>
class IteratorRange {
  A begin_;
  B end_;

public:
  IteratorRange(A a, B b) : begin_(std::move(a)), end_(std::move(b)) {}
  const A &begin() const {
    return begin_;
  }
  const B &end() const {
    return end_;
  }
};

template <class A, class B>
IteratorRange<A, B> makeIteratorRange(A a, B b) {
  return {std::move(a), std::move(b)};
}
