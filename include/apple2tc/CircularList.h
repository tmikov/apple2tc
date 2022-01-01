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

  void push_back(T *item) {
    ListEntry::insertAfterEntry(m_root.prev, Accessor::toListEntry(item));
  }

  void insertAfter(T *after, T *toInsert) {
    ListEntry::insertAfterEntry(Accessor::toListEntry(after), Accessor::toListEntry(toInsert));
  }

  void remove(T *item) {
    ListEntry::removeEntry(Accessor::toListEntry(item));
  }

  void replace(T *old, T *newEntry) {
    ListEntry::replaceEntry(Accessor::toListEntry(old), Accessor::toListEntry(newEntry));
  }

  T *first() {
    return Accessor::fromListEntry(m_root.next);
  }

  T *last() {
    return Accessor::fromListEntry(m_root.prev);
  }

  T *next(T *item) {
    return Accessor::fromListEntry(Accessor::toListEntry(item)->next);
  }

  T *prev(T *item) {
    return Accessor::fromListEntry(Accessor::toListEntry(item)->prev);
  }

  const T *first() const {
    return Accessor::fromListEntry(m_root.next);
  }

  const T *last() const {
    return Accessor::fromListEntry(m_root.prev);
  }

  const T *next(const T *item) const {
    return Accessor::fromListEntry(Accessor::toListEntry(item)->next);
  }

  const T *prev(const T *item) const {
    return Accessor::fromListEntry(Accessor::toListEntry(item)->prev);
  }

  void destructiveAppend(CircularList<T, Accessor> &lst) {
    if (!lst.empty()) {
      // insertAfterEntry( last(), lst.first(), lst.last() );
      ListEntry::insertAfterEntry(m_root.prev, lst.m_root.next, lst.m_root.prev);
      lst.m_root.prev = lst.m_root.next = &lst.m_root; // mark it as empty
    }
  }

  class iterator_base : public std::iterator<std::bidirectional_iterator_tag, T> {
  public:
    bool operator==(const iterator_base &it) const {
      return m_p == it.m_p;
    }
    bool operator!=(const iterator_base &it) const {
      return m_p != it.m_p;
    }

    const T *operator->() const {
      return Accessor::fromListEntry(m_p);
    }
    const T &operator*() const {
      return *Accessor::fromListEntry(m_p);
    }

  protected:
    ListEntry *m_p;

    explicit iterator_base(ListEntry *p) {
      m_p = p;
    }
  };

  class iterator : public iterator_base {
  public:
    iterator(const iterator &it) : iterator_base(it.m_p) {}

    iterator &operator=(const iterator &it) {
      iterator_base::m_p = it.m_p;
      return *this;
    }

    T *operator->() const {
      return Accessor::fromListEntry(iterator_base::m_p);
    }
    T &operator*() const {
      return *Accessor::fromListEntry(iterator_base::m_p);
    }

    iterator &operator++() {
      iterator_base::m_p = iterator_base::m_p->next;
      return *this;
    }
    iterator &operator--() {
      iterator_base::m_p = iterator_base::m_p->prev;
      return *this;
    }
    iterator operator++(int) {
      ListEntry *tmp = iterator_base::m_p;
      iterator_base::m_p = iterator_base::m_p->next;
      return iterator(tmp);
    }
    iterator operator--(int) {
      ListEntry *tmp = iterator_base::m_p;
      iterator_base::m_p = iterator_base::m_p->prev;
      return iterator(tmp);
    }

  protected:
    explicit iterator(ListEntry *p) : iterator_base(p) {}

    friend class CircularList;
  };

  class const_iterator : public iterator_base {
  public:
    const_iterator(const iterator_base &it) : iterator_base(it.m_p) {}

    const_iterator &operator=(const iterator_base &it) {
      iterator_base::m_p = it.m_p;
      return *this;
    }

    const_iterator &operator++() {
      iterator_base::m_p = iterator_base::m_p->next;
      return *this;
    }
    const_iterator &operator--() {
      iterator_base::m_p = iterator_base::m_p->prev;
      return *this;
    }
    const_iterator operator++(int) {
      ListEntry *tmp = iterator_base::m_p;
      iterator_base::m_p = iterator_base::m_p->next;
      return const_iterator(tmp);
    }
    const_iterator operator--(int) {
      ListEntry *tmp = iterator_base::m_p;
      iterator_base::m_p = iterator_base::m_p->prev;
      return const_iterator(tmp);
    }

  protected:
    explicit const_iterator(ListEntry *p) : iterator_base(p) {}

    friend class CircularList;
  };

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

private:
  ListEntry m_root;
};