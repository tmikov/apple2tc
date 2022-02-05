/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <unordered_map>
#include <vector>

/// Wrapper over unordered_set<> that provides reproducible iteration order.
/// It is insertion order, unless erases are present. Erases keep the order
/// reproducible, but it is no longer insertion.
template <class K>
class PrimitiveSetVector {
  std::vector<K> vec_{};
  std::unordered_map<K, size_t> map_{};

public:
  using iterator = typename std::vector<K>::iterator;
  using const_iterator = typename std::vector<K>::const_iterator;

  auto empty() const {
    return vec_.empty();
  }
  auto size() const {
    return vec_.size();
  }

  std::pair<iterator, bool> insert(K key) {
    auto tmpRes = map_.template try_emplace(key, vec_.size());
    if (tmpRes.second) {
      // Inserted a new element.
      vec_.push_back(key);
      return std::make_pair(vec_.end() - 1, true);
    } else {
      // Existing element.
      return std::make_pair(vec_.begin() + tmpRes.first->second, false);
    }
  }

  size_t erase(const K &key) {
    auto it = map_.find(key);
    if (it == map_.end())
      return 0;
    eraseHelper(it, it->second);
    return 1;
  }

  iterator erase(const_iterator pos) {
    size_t index = pos - vec_.begin();
    eraseHelper(map_.find(*pos), index);
    return vec_.begin() + index;
  }
  iterator erase(iterator pos) {
    return erase(const_iterator(pos));
  }

  auto begin() {
    return vec_.begin();
  }
  auto end() {
    return vec_.end();
  }
  auto begin() const {
    return vec_.begin();
  }
  auto end() const {
    return vec_.end();
  }

private:
  void eraseHelper(decltype(map_.begin()) mapIt, size_t index) {
    map_.erase(mapIt);
    if (index == vec_.size() - 1) {
      vec_.pop_back();
    } else {
      // Swap the last element with the erased element.
      std::swap(vec_.back(), vec_[index]);
      vec_.pop_back();
      map_[vec_[index]] = index;
    }
  }
};