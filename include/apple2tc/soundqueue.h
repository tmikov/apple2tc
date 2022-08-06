/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#ifdef __cplusplus
#include <atomic>
#include <memory>

using std::atomic_bool;
using std::atomic_uint;
using std::memory_order_acquire;
#else
#include <stdatomic.h>
#endif

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/// An asynchronous non-blocking queue than can be accessed simultaneously by a
/// writer and a reader thread,
typedef struct {
  /// Index of next element to read. Accessed only by the reader thread.
  unsigned head;
  /// Index of next element to write. Accessed only by the writer thread.
  unsigned tail;
  /// Number of elements in queue. Shared by the reader and writer thread.
  atomic_uint count;
  /// The capacity of the queue (the size of the block pointed by `data`) in
  /// bytes. Always a power of 2.
  unsigned capacity;
  /// The size of one logical element. This is used only in debug mode, to verify
  /// that all sizes that are pushed and popped are aligned to it.
  unsigned elem_size;
  /// The actual data.
  char *data;
} sound_queue_t;

/// Describes a logically consecutive region in the queue. Due to the wrap-around
/// nature of he queue, it can be split in two data regions.
typedef struct {
  char *part1;
  /// Size of part1 in bytes.
  unsigned size1;
  char *part2;
  /// Size of part2 in bytes.
  unsigned size2;
} queue_parts_t;

#ifdef __cplusplus
extern "C" {
#endif

/// Initialize the queue to empty before use. Return false if malloc() fails.
/// \param capacity - the capacity of the queue in bytes. Must be a power of 2.
/// \param elem_size - size of one logical element in bytes. This is used only
///     in debug mode to verify proper alignment of pushes and pops.
bool sound_queue_init(sound_queue_t *q, unsigned capacity, unsigned elem_size);

/// Deallocate the queue.
void sound_queue_free(sound_queue_t *q);

/// Return the size of the free space in the queue. This amount of data is
/// guaranteed to be writable.
static inline unsigned sound_queue_expect(sound_queue_t *q) {
  return q->capacity - atomic_load_explicit(&q->count, memory_order_acquire);
}

/// Return the size of the available data in the queue. This amount of data is
/// guaranteed to be readable.
static inline unsigned sound_queue_count(sound_queue_t *q) {
  return atomic_load_explicit(&q->count, memory_order_acquire);
}

/// Return a descriptor for writing into the queue of size len of less.
queue_parts_t sound_queue_writeparts(sound_queue_t *q, unsigned len);
/// Return a descriptor for reading from the queue of size len of less.
queue_parts_t sound_queue_readparts(sound_queue_t *q, unsigned len);
/// Advance the tail after writing to the queue to append to it.
void sound_queue_adv_tail(sound_queue_t *q, unsigned len);
/// Advance the head after reading from the queue to consume from it.
void sound_queue_adv_head(sound_queue_t *q, unsigned len);
/// Attempt to push the specified data into the queue and return the size of
/// the data that was actually pushed (in case there wasn't enough space in the
/// queue). This is a combination of `sound_queue_write_parts()`, `memcpy()`,
/// and `sound_queue_adv_tail()`.
unsigned sound_queue_push(sound_queue_t *q, const void *data, unsigned len);
/// Attempt to pop the specified length from the queue and return the size of
/// the data that was actually popped (in case there wasn't enough data in the
/// queue). This is a combination of `sound_queue_read_parts()`, `memcpy()`,
/// and `sound_queue_adv_head()`.
unsigned sound_queue_pop(sound_queue_t *q, void *data, unsigned len);
/// Read from the queue without removing the data.
unsigned sound_queue_peek(sound_queue_t *q, void *data, unsigned len);

/// Push exactly one logical element to the queue. Return 0 if the queue was full,
/// `len` otherwise.
static inline unsigned sound_queue_push1(sound_queue_t *q, const void *data, unsigned len) {
  assert(len == q->elem_size && "len must be one element");
  if (sound_queue_expect(q) < len)
    return 0;
  memcpy(q->data + q->tail, data, len);
  sound_queue_adv_tail(q, len);
  return len;
}

/// Peek exactly one logical element from the queue. Return 0 if the queue was
/// empty, `len` otherwise.
static inline unsigned sound_queue_peek1(sound_queue_t *q, void *data, unsigned len) {
  assert(len == q->elem_size && "len must be one element");
  if (sound_queue_count(q) < len)
    return 0;
  memcpy(data, q->data + q->head, len);
  return len;
}

/// Pop exactly one logical element from the queue. Return 0 if the queue was
/// empty, `len` otherwise.
static inline unsigned sound_queue_pop1(sound_queue_t *q, void *data, unsigned len) {
  unsigned res = sound_queue_peek1(q, data, len);
  sound_queue_adv_head(q, res);
  return res;
}

#ifdef __cplusplus
}
#endif
