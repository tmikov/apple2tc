/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cassert>
#include <iterator>

struct ListEntry {
  ListEntry *prev, *next;

  ListEntry() {
    debugClear();
  }
  ListEntry(const ListEntry &) = delete;
  ListEntry(ListEntry &&) = delete;
  void operator=(const ListEntry &) = delete;
  void operator=(ListEntry &&) = delete;

  void debugClear() {
#ifndef NDEBUG
    this->prev = this->next = 0;
#endif
  }

  static void insertBeforeEntry(ListEntry *before, ListEntry *entry) {
    assert(!entry->prev && !entry->next);
    assert(before->prev && before->next);
    entry->prev = before->prev;
    entry->next = before;
    before->prev->next = entry;
    before->prev = entry;
  }
  static void insertAfterEntry(ListEntry *after, ListEntry *entry) {
    assert(!entry->prev && !entry->next);
    assert(after->prev && after->next);
    entry->next = after->next;
    entry->prev = after;
    after->next->prev = entry;
    after->next = entry;
  }

  static void insertAfterEntry(ListEntry *after, ListEntry *entryFirst, ListEntry *entryLast) {
    entryLast->next = after->next;
    entryFirst->prev = after;
    after->next->prev = entryLast;
    after->next = entryFirst;
  }

  static void removeEntry(ListEntry *entry) {
    entry->prev->next = entry->next;
    entry->next->prev = entry->prev;
    entry->debugClear();
  }

  static void replaceEntry(ListEntry *old, ListEntry *newEntry) {
#if 0
    insertAfterEntry(old, newEntry);
    removeEntry(old);
#else
    assert(old->prev && old->next);
    assert(!newEntry->prev && !newEntry->next);
    old->prev->next = newEntry;
    old->next->prev = newEntry;
    newEntry->prev = old->prev;
    newEntry->next = old->next;

    old->debugClear();
#endif
  }
};

// C++ trickery. In C++ we can't usually use container_of/offsetof to convert from a member field
// to the enclosing object. So, instead we must use inheritance and static_cast<>.
//
// There is an additional complication though: we get suboptimal code because static_cast<> from
// NULL always returns NULL even when the base offset is non-0. So we need a way to tell the
// compiler that the pointer is non-NULL. We can do that by using references.
//
// So, if we have a struct S which we want to use in a LinkedList, first we have it inherit from
// LinkedList through an intermediate struct, which we will use as a tag for for static_cast<>
// (for the cases when the same struct may need to participate in different lists).
//
// struct _Base1 : public ListEntry {};
// struct _Base2 : public ListEntry {};
//
// struct S : public _Base1, _Base2
// {
//   ....
// }
//
// Now "struct S" contains links for two independent lists. This is how we convert from a
// (ListEntry *) to a (struct S *):
//
// struct Accessor
// {
//   S * fromListEntry ( ListEntry * entry )
//   {
//     ListEntry & _entry = *entry; // convert it to a reference
//     return &static_cast<S&>(static_cast<_Base1&>(_entry));
//   }
//
//   // Note that exactly the same applies for conversion in the opposite direction:
//   ListEntry * toListEntry ( S * p )
//   {
//     S & _p = *p;
//     return &static_cast<_Base1&>(_p);
//   }
// }

// The default accessor for the simple case when there is only one ListEntry base
template <class T>
struct DefaultCircularListAccessor {
  static T *fromListEntry(ListEntry *entry) {
    assert(entry);
    // NOTE: we need to go through these contortions to avoid checking the pointer against NULL
    ListEntry &_entry = *entry;
    return &static_cast<T &>(_entry);
  }

  static ListEntry *toListEntry(T *p) {
    assert(p);
    // NOTE: we need to go through these contortions to avoid checking the pointer against NULL
    T &_p = *p;
    return &static_cast<ListEntry &>(_p);
  }
};

template <class T, class Accessor = DefaultCircularListAccessor<T>>
class CircularList {
public:
  using AccessorType = Accessor;

  CircularList(const CircularList &l) = delete;
  CircularList &operator=(const CircularList &l) = delete;

  CircularList() {
    m_root.prev = m_root.next = &m_root;
  }

  void destroyAll() {
    while (!empty())
      delete pop_back_val();
  }

  T *fromListEntry(ListEntry *entry) {
    return Accessor::fromListEntry(entry);
  }

  ListEntry *toListEntry(T *p) {
    return Accessor::toListEntry(p);
  }

  const T *fromListEntry(const ListEntry *entry) const {
    return Accessor::fromListEntry(entry);
  }

  const ListEntry *toListEntry(const T *p) const {
    return Accessor::toListEntry(p);
  }

  [[nodiscard]] bool empty() const {
    return m_root.next == &m_root;
  }

  /// Count and return the number of elements. This method is O(n).
  [[nodiscard]] size_t countElements() const {
    size_t size = 0;
    for (const ListEntry *cur = m_root.next, *end = &m_root; cur != end; cur = cur->next)
      ++size;
    return size;
  }
  /// Count and return the number of elements up to \p maxCount. This method is O(n).
  [[nodiscard]] size_t countElementsUpTo(size_t maxCount) const {
    size_t size = 0;
    for (const ListEntry *cur = m_root.next, *end = &m_root; cur != end; cur = cur->next)
      if (++size >= maxCount)
        break;
    return size;
  }

  T &front() {
    assert(!empty());
    return *fromListEntry(m_root.next);
  }
  const T &front() const {
    assert(!empty());
    return *fromListEntry(m_root.next);
  }
  T &back() {
    assert(!empty());
    return *fromListEntry(m_root.prev);
  }
  const T &back() const {
    assert(!empty());
    return *fromListEntry(m_root.prev);
  }

  void push_front(T *item) {
    ListEntry::insertAfterEntry(&m_root, Accessor::toListEntry(item));
  }
  void push_back(T *item) {
    ListEntry::insertAfterEntry(m_root.prev, Accessor::toListEntry(item));
  }

  void replace(T *old, T *newEntry) {
    ListEntry::replaceEntry(Accessor::toListEntry(old), Accessor::toListEntry(newEntry));
  }

  void pop_back() {
    assert(!empty());
    ListEntry::removeEntry(m_root.prev);
  }
  void pop_front() {
    assert(!empty());
    ListEntry::removeEntry(m_root.next);
  }
  T *pop_back_val() {
    assert(!empty());
    auto *res = m_root.prev;
    ListEntry::removeEntry(res);
    return Accessor::fromListEntry(res);
  }
  T *pop_front_val() {
    assert(!empty());
    auto *res = m_root.next;
    ListEntry::removeEntry(res);
    return Accessor::fromListEntry(res);
  }

  void destructiveAppend(CircularList<T, Accessor> &&lst) {
    if (!lst.empty()) {
      // insertAfterEntry( last(), lst.first(), lst.last() );
      ListEntry::insertAfterEntry(m_root.prev, lst.m_root.next, lst.m_root.prev);
      lst.m_root.prev = lst.m_root.next = &lst.m_root; // mark it as empty
    }
  }

  template <bool FWD>
  class iterator_base {
  public:
    bool operator==(const iterator_base &it) const {
      return m_p == it.m_p;
    }
    bool operator!=(const iterator_base &it) const {
      return m_p != it.m_p;
    }
    explicit operator bool() const {
      return m_p;
    }

  protected:
    explicit iterator_base(std::nullptr_t) {
      m_p = nullptr;
    }
    explicit iterator_base(ListEntry *p) {
      assert(p);
      m_p = p;
    }

    ListEntry *getPrev() const {
      assert(m_p->prev && m_p->next);
      return FWD ? m_p->prev : m_p->next;
    }
    ListEntry *getNext() const {
      assert(m_p->prev && m_p->next);
      return FWD ? m_p->next : m_p->prev;
    }

    ListEntry *m_p;
  };

  template <bool FWD>
  class iterator_impl : public iterator_base<FWD> {
    friend class CircularList;
    using Super = iterator_base<FWD>;

  public:
    iterator_impl() : Super(nullptr) {}
    explicit iterator_impl(ListEntry *p) : Super(p) {}
    explicit iterator_impl(T *item) : Super(Accessor::toListEntry(item)) {}
    iterator_impl(const iterator_impl<FWD> &it) : Super(it.m_p) {}

    iterator_impl &operator=(const iterator_impl &it) {
      Super::m_p = it.m_p;
      return *this;
    }

    ListEntry *entry() const {
      return Super::m_p;
    }
    T *ptr() const {
      return Accessor::fromListEntry(Super::m_p);
    }
    T *operator->() const {
      return ptr();
    }
    T &operator*() const {
      return *ptr();
    }

    iterator_impl &operator++() {
      Super::m_p = Super::getNext();
      return *this;
    }
    iterator_impl &operator--() {
      Super::m_p = Super::getPrev();
      return *this;
    }
    iterator_impl operator++(int) {
      ListEntry *tmp = Super::m_p;
      Super::m_p = Super::getNext();
      return iterator_impl(tmp);
    }
    iterator_impl operator--(int) {
      ListEntry *tmp = Super::m_p;
      Super::m_p = Super::getPrev();
      return iterator_impl(tmp);
    }
  };

  template <bool FWD>
  class const_iterator_impl : public iterator_base<FWD> {
    friend class CircularList;
    using Super = iterator_base<FWD>;

  public:
    const_iterator_impl() : Super(nullptr) {}
    explicit const_iterator_impl(const ListEntry *p) : Super(const_cast<ListEntry *>(p)) {}
    explicit const_iterator_impl(const T *item) : Super(Accessor::toListEntry(item)) {}
    const_iterator_impl(const Super &it) : Super(it.m_p) {} // NOLINT(google-explicit-constructor)

    const_iterator_impl &operator=(const Super &it) {
      Super::m_p = it.m_p;
      return *this;
    }

    const ListEntry *entry() const {
      return Super::m_p;
    }
    const T *ptr() const {
      return Accessor::fromListEntry(Super::m_p);
    }
    const T *operator->() const {
      return ptr();
    }
    const T &operator*() const {
      return *ptr();
    }

    const_iterator_impl &operator++() {
      Super::m_p = Super::getNext();
      return *this;
    }
    const_iterator_impl &operator--() {
      Super::m_p = Super::getPrev();
      return *this;
    }
    const_iterator_impl operator++(int) {
      ListEntry *tmp = Super::m_p;
      Super::m_p = Super::getNext();
      return const_iterator_impl(tmp);
    }
    const_iterator_impl operator--(int) {
      ListEntry *tmp = Super::m_p;
      Super::m_p = Super::getPrev();
      return const_iterator_impl(tmp);
    }
  };

  using iterator = iterator_impl<true>;
  using const_iterator = const_iterator_impl<true>;

  using reverse_iterator = iterator_impl<false>;
  using const_reverse_iterator = const_iterator_impl<false>;

  iterator begin() {
    return iterator(m_root.next);
  }
  iterator end() {
    return iterator(&m_root);
  }
  const_iterator begin() const {
    return const_iterator(m_root.next);
  }
  const_iterator end() const {
    return const_iterator(const_cast<ListEntry *>(&m_root)); // TODO
  }
  reverse_iterator rbegin() {
    return reverse_iterator(m_root.prev);
  }
  reverse_iterator rend() {
    return reverse_iterator(&m_root);
  }
  const_reverse_iterator rbegin() const {
    return const_iterator(m_root.prev);
  }
  const_reverse_iterator rend() const {
    return const_iterator(const_cast<ListEntry *>(&m_root)); // TODO
  }

  /// Insert the specified item after the insertion point.
  void insertAfter(iterator after, T *item) {
    ListEntry::insertAfterEntry(after.entry(), Accessor::toListEntry(item));
  }
  void insertAfter(T *after, T *item) {
    ListEntry::insertAfterEntry(Accessor::toListEntry(after), Accessor::toListEntry(item));
  }
  void insertBefore(iterator before, T *item) {
    ListEntry::insertBeforeEntry(before.entry(), Accessor::toListEntry(item));
  }
  void insertBefore(T *before, T *item) {
    ListEntry::insertBeforeEntry(Accessor::toListEntry(before), Accessor::toListEntry(item));
  }

  void remove(iterator it) {
    ListEntry::removeEntry(it.entry());
  }
  void remove(T *item) {
    ListEntry::removeEntry(Accessor::toListEntry(item));
  }

private:
  ListEntry m_root;
};
