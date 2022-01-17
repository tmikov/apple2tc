/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <cstdarg>
#include <cstdio>
#include <optional>
#include <string>

/// Read a file into a container T. T is usually std::string for text files
/// and std::vector<uint8_t> for binary files.
template <class T>
T readAll(FILE *f) {
  T buf;
  static constexpr unsigned CHUNK = 8192;
  for (;;) {
    size_t size = buf.size();
    buf.resize(size + CHUNK);
    size_t nr = fread(&buf[size], 1, CHUNK, f);
    buf.resize(size + nr);
    if (nr != CHUNK)
      break;
  }
  return buf;
}

std::string vformat(const char *msg, va_list ap);
std::string format(const char *msg, ...) __attribute__((__format__(__printf__, 1, 2)));

[[noreturn]] void panicAbort(const char *msg, ...) __attribute__((__format__(__printf__, 1, 2)));

#define APPLE2TC_SUPPORT_STRINGIFY_HELPER(x) #x
#define APPLE2TC_SUPPORT_STRINGIFY(x) APPLE2TC_SUPPORT_STRINGIFY_HELPER(x)

#ifdef NDEBUG
#define PANIC_ABORT(...) (panicAbort(__VA_ARGS__))
#define PANIC_ASSERT(cond) ((cond) ? (void)0 : panicAbort("runtime assertion '%s' failed", #cond))
#define PANIC_ASSERT_MSG(cond, msg) ((cond) ? (void)0 : panicAbort("%s", msg))
#else
#define PANIC_ABORT(...) \
  (panicAbort(__FILE__ "(" APPLE2TC_SUPPORT_STRINGIFY(__LINE__) "): " __VA_ARGS__))
#define PANIC_ASSERT(cond) \
  ((cond) ? (void)0 : panicAbort("%s(%u): assertion '%s' failed", __FILE__, __LINE__, #cond))
#define PANIC_ASSERT_MSG(cond, msg) \
  ((cond) ? (void)0 : panicAbort("%s(%u): %s (assertion '%s')", __FILE__, __LINE__, msg, #cond))
#endif

/// Case insensitive ASCII string equality.
bool eqci(const char *a, const char *b);

void upcaseStr(std::string &s);
