/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/soundqueue.h"

#include <stdlib.h>

bool sound_queue_init(sound_queue_t *q, unsigned capacity, unsigned elem_size) {
  assert(capacity && (capacity & (capacity - 1)) == 0 && "capacity must be a power of 2");
  assert(capacity % elem_size == 0 && "capacity not aligned");
  q->head = 0;
  q->tail = 0;
  atomic_init(&q->count, 0);
  q->capacity = capacity;
  q->elem_size = elem_size;
  q->data = (char *)malloc(capacity);
  return q->data != NULL;
}

void sound_queue_free(sound_queue_t *q) {
  free(q->data);
  q->data = NULL;
}

queue_parts_t sound_queue_writeparts(sound_queue_t *q, unsigned len) {
  assert(len % q->elem_size == 0 && "len isn't aligned to element size");
  unsigned expect = sound_queue_expect(q);
  if (len > expect)
    len = expect;

  if (len <= q->capacity - q->tail) {
    return (queue_parts_t){.part1 = q->data + q->tail, .size1 = len};
  } else {
    unsigned toCopy = q->capacity - q->tail;
    return (queue_parts_t){
        .part1 = q->data + q->tail, .size1 = toCopy, .part2 = q->data, .size2 = len - toCopy};
  }
}

queue_parts_t sound_queue_readparts(sound_queue_t *q, unsigned len) {
  assert(len % q->elem_size == 0 && "len isn't aligned to element size");
  unsigned count = sound_queue_count(q);
  if (len > count)
    len = count;

  if (len <= q->capacity - q->head) {
    return (queue_parts_t){.part1 = q->data + q->head, .size1 = len};
  } else {
    unsigned toCopy = q->capacity - q->head;
    return (queue_parts_t){
        .part1 = q->data + q->head, .size1 = toCopy, .part2 = q->data, .size2 = len - toCopy};
  }
}

void sound_queue_adv_tail(sound_queue_t *q, unsigned len) {
  assert(len % q->elem_size == 0 && "len isn't aligned to element size");
  q->tail = (q->tail + len) & (q->capacity - 1);
  atomic_fetch_add_explicit(&q->count, len, memory_order_release);
}

void sound_queue_adv_head(sound_queue_t *q, unsigned len) {
  assert(len % q->elem_size == 0 && "len isn't aligned to element size");
  q->head = (q->head + len) & (q->capacity - 1);
  atomic_fetch_add_explicit(&q->count, -len, memory_order_release);
}

unsigned sound_queue_push(sound_queue_t *q, const void *data, unsigned len) {
  assert(len % q->elem_size == 0 && "len isn't aligned to element size");
  queue_parts_t parts = sound_queue_writeparts(q, len);
  memcpy(parts.part1, data, parts.size1);
  if (parts.size2)
    memcpy(parts.part2, (const char *)data + parts.size1, parts.size2);
  sound_queue_adv_tail(q, parts.size1 + parts.size2);
  return parts.size1 + parts.size2;
}

unsigned sound_queue_peek(sound_queue_t *q, void *data, unsigned len) {
  assert(len % q->elem_size == 0 && "len isn't aligned to element size");
  queue_parts_t parts = sound_queue_readparts(q, len);
  memcpy(data, parts.part1, parts.size1);
  if (parts.size2)
    memcpy((char *)data + parts.size1, parts.part2, parts.size2);
  return parts.size1 + parts.size2;
}

unsigned sound_queue_pop(sound_queue_t *q, void *data, unsigned len) {
  unsigned peek_len = sound_queue_peek(q, data, len);
  sound_queue_adv_head(q, peek_len);
  return peek_len;
}
