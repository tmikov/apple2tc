/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <iterator>

template <class A, class B = A>
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
  bool empty() const {
    return begin_ == end_;
  }
};

template <class A, class B>
IteratorRange<A, B> makeIteratorRange(A a, B b) {
  return {std::move(a), std::move(b)};
}
template <class T>
auto makeIteratorRange(T &t) {
  return makeIteratorRange(std::begin(t), std::end(t));
}
template <class T>
auto makeReverseRange(T &t) {
  return makeIteratorRange(std::rbegin(t), std::rend(t));
}
