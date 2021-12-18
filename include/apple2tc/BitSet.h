/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>

class BitSet {
public:
  explicit BitSet(unsigned size)
      : data_(new uint32_t[(size + 31) >> 5]), bitSize_(size), wordSize_((size + 31) >> 5) {
    clear();
  }

  ~BitSet() {
    delete[] data_;
  }

  [[nodiscard]] unsigned size() const {
    return bitSize_;
  }

  void set(unsigned i, bool v) {
    assert(i < bitSize_);
    if (v)
      data_[i >> 5] |= 1 << (i & 31);
    else
      data_[i >> 5] &= ~(1 << (i & 31));
  }

  void setMulti(unsigned begin, unsigned end, bool v) {
    while (begin != end)
      set(begin++, v);
  }

  [[nodiscard]] bool get(unsigned i) const {
    assert(i < bitSize_);
    return data_[i >> 5] & (1 << (i & 31));
  }

  void clear() {
    memset(data_, 0, wordSize_ * 4);
  }

  /// return the index of the next set bit starting from \p startingFrom, or
  /// `size()` if none was found.
  [[nodiscard]] unsigned findSetBit(unsigned startingFrom) const {
    const uint32_t *p = data_ + (startingFrom >> 5);
    if (unsigned bitIndex = startingFrom & 31) {
      uint32_t w = *p >> bitIndex;
      for (; w; ++bitIndex, w >>= 1) {
        if (w & 1) {
          bitIndex += (p - data_) * 32;
          return bitIndex < bitSize_ ? bitIndex : bitSize_;
        }
      }
      ++p;
    }

    const uint32_t *end = data_ + wordSize_;
    for (; p != end; ++p) {
      uint32_t w = *p;
      for (unsigned bitIndex = 0; w; ++bitIndex, w >>= 1) {
        if (w & 1) {
          bitIndex += (p - data_) * 32;
          return bitIndex < bitSize_ ? bitIndex : bitSize_;
        }
      }
    }

    return bitSize_;
  }

  /// return the index of the next set bit starting from \p startingFrom, or
  /// `size()` if none was found.
  [[nodiscard]] unsigned findClearBit(unsigned startingFrom) const {
    const uint32_t *p = data_ + (startingFrom >> 5);
    if (unsigned bitIndex = startingFrom & 31) {
      uint32_t w = ~*p >> bitIndex;
      for (; w; ++bitIndex, w >>= 1) {
        if (w & 1) {
          bitIndex += (p - data_) * 32;
          return bitIndex < bitSize_ ? bitIndex : bitSize_;
        }
      }
      ++p;
    }

    const uint32_t *end = data_ + wordSize_;
    for (; p != end; ++p) {
      uint32_t w = ~*p;
      for (unsigned bitIndex = 0; w; ++bitIndex, w >>= 1) {
        if (w & 1) {
          bitIndex += (p - data_) * 32;
          return bitIndex < bitSize_ ? bitIndex : bitSize_;
        }
      }
    }

    return bitSize_;
  }

  void swap(BitSet &o) noexcept {
    std::swap(data_, o.data_);
    std::swap(bitSize_, o.bitSize_);
    std::swap(wordSize_, o.wordSize_);
  }

private:
  uint32_t *data_;
  /// Size in bits.
  unsigned bitSize_;
  /// Size in 32-bit words.
  unsigned wordSize_;
};

inline void swap(BitSet &a, BitSet &b) noexcept {
  a.swap(b);
}