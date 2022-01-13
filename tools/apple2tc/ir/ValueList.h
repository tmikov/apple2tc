/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include "apple2tc/CircularList.h"

#include <climits>
#include <memory>

namespace ir {

template <class ValueT, class OwnerT>
class ValueListBase {
public:
  using ValueType = ValueT;
  using OwnerType = OwnerT;

  class UseNode : public ListEntry {
  public:
    /// Constructor that doesn't add to the value's list.
    explicit UseNode(ValueType *value, OwnerType *owner, bool) : value_(value), owner_(owner) {}
    explicit UseNode(ValueType *value, OwnerType *owner) : value_(value), owner_(owner) {
      if (value)
        value->userList_.push_back(this);
    }
    ~UseNode() {
      if (value_)
        value_->userList_.remove(this);
    }

    [[nodiscard]] OwnerType *owner() const {
      return owner_;
    }
    [[nodiscard]] ValueType *value() const {
      return value_;
    }
    void setValue(ValueType *value) {
      if (value_)
        value_->userList_.remove(this);
      value_ = value;
      if (value)
        value_->userList_.push_back(this);
    }

  private:
    friend class ValueListBase;
    ValueType *value_;
    // TODO: we should remove this and use bit tricks.
    OwnerType *owner_;
  };

  class UserListType : public CircularList<UseNode> {
  public:
    ~UserListType() {
      while (!CircularList<UseNode>::empty())
        CircularList<UseNode>::back().setValue(nullptr);
    }
  };

  using iterator = UseNode *;
  using const_iterator = const UseNode *;

  ValueListBase(const ValueListBase &) = delete;
  void operator=(const ValueListBase &) = delete;

protected:
  explicit ValueListBase(OwnerType *owner, unsigned capacity)
      : owner_(owner),
        data_(reinterpret_cast<UseNode *>(getSmall())),
        size_(0),
        capacity_(capacity) {}

public:
  ~ValueListBase() {
    clear();
    deallocate();
  }

  void clear() {
    // Destroy in reverse order.
    for (UseNode *e = data_ + size_, *s = data_; e != s; --e) {
      auto *cur = e - 1;
      cur->~UseNode();
    }
    size_ = 0;
  }

  /// Move all values from the specified list onto our list, resetting the owner.
  void moveFrom(ValueListBase &&other) {
    // If the other object is not in small mode and we are empty, we can just
    // "steal" its data.
    if (!other.isSmall() && empty()) {
      // Free our buffer.
      deallocate();
      // Steal the other buffer.
      data_ = other.data_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      // Reset the other to empty. Note that we don't know its small capacity,
      // so we pretend it is zero, which is sub-optimal.
      other.data_ = reinterpret_cast<UseNode *>(getSmall());
      other.size_ = 0;
      other.capacity_ = 0;

      // Reset the owner of the stolen items.
      for (UseNode *cur = data_, *e = data_ + size_; cur != e; ++cur)
        cur->owner_ = owner_;

      return;
    }

    // Slow path - just copy all nodes and reset the other.
    for (const auto &node : other)
      push_back(node.value());

    other.clear();
  }

  [[nodiscard]] bool empty() const {
    return size_ == 0;
  }
  [[nodiscard]] size_t size() const {
    return size_;
  }

  [[nodiscard]] UseNode &operator[](size_t index) {
    assert(index < size_);
    return data_[index];
  }
  [[nodiscard]] const UseNode &operator[](size_t index) const {
    assert(index < size_);
    return data_[index];
  }
  [[nodiscard]] iterator begin() {
    return data_;
  }
  [[nodiscard]] iterator end() {
    return data_ + size_;
  }
  [[nodiscard]] const_iterator begin() const {
    return data_;
  }
  [[nodiscard]] const_iterator end() const {
    return data_ + size_;
  }

  void push_back(ValueType *value) {
    if (size_ == capacity_)
      grow();
    new (data_ + size_++) UseNode(value, owner_);
  }

private:
  inline const void *getSmall() const;
  inline void *getSmall();

  bool isSmall() const {
    return (const void *)data_ == getSmall();
  }

  /// Deallocate the buffer, if not small.
  void deallocate() {
    if (!isSmall())
      std::allocator<UseNode>().deallocate(data_, capacity_);
  }

  void grow() {
    // Calculate new capacity, with overflow checking.
    size_t newCap;
    // Capacity can't be zero, since we always start with kSmallCap.
    assert(capacity_ != 0);
    if (capacity_ <= UINT_MAX / 2)
      newCap = capacity_ * 2;
    else if (capacity_ != UINT_MAX)
      newCap = UINT_MAX;
    else
      throw std::bad_array_new_length();

    // Allocate new buffer.
    UseNode *newData = std::allocator<UseNode>().allocate(newCap);

    // Move the nodes.
    moveNodes(data_, data_ + size_, newData, owner_);

    // Deallocate the old buffer. Note that we do not destruct the old nodes!
    deallocate();
    data_ = newData;
    capacity_ = newCap;
  }

  /// Copy the nodes, removing the old ones from the list and adding the new ones.
  static void moveNodes(UseNode *from, UseNode *end, UseNode *dest, OwnerType *newOwner) {
    for (; from != end; ++dest, ++from) {
      auto *value = from->value_;
      new (dest) UseNode(value, newOwner, false);
      if (value)
        value->userList_.replace(from, dest);
    }
  }

private:
  UseNode *data_;
  unsigned size_;
  unsigned capacity_;
  OwnerType *owner_;
}; // class ValueListBase

template <class ValueT, class OwnerT, unsigned SMALL_CAP = 2>
class ValueList : public ValueListBase<ValueT, OwnerT> {
public:
  using Super = ValueListBase<ValueT, OwnerT>;
  using typename Super::UseNode;

  explicit ValueList(OwnerT *owner) : ValueListBase<ValueT, OwnerT>(owner, SMALL_CAP) {}

private:
  friend class ValueListBase<ValueT, OwnerT>;
  alignas(UseNode) char small_[sizeof(UseNode) * SMALL_CAP];
};

template <class ValueT, class OwnerT>
inline const void *ValueListBase<ValueT, OwnerT>::getSmall() const {
  return static_cast<const ValueList<ValueT, OwnerT, 0> *>(this)->small_;
}
template <class ValueT, class OwnerT>
inline void *ValueListBase<ValueT, OwnerT>::getSmall() {
  return static_cast<ValueList<ValueT, OwnerT, 0> *>(this)->small_;
}
} // namespace ir