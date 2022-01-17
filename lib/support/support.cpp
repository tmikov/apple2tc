/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/support.h"

#include <cstdarg>

std::string vformat(const char *msg, va_list ap) {
  va_list ap1;
  va_copy(ap1, ap);
  int len = vsnprintf(nullptr, 0, msg, ap1);
  if (len < 0)
    return "";
  std::string res(len, ' ');
  vsnprintf(res.data(), res.size() + 1, msg, ap);
  return res;
}

std::string format(const char *msg, ...) {
  va_list ap;
  va_start(ap, msg);
  auto res = vformat(msg, ap);
  va_end(ap);
  return res;
}

void panicAbort(const char *msg, ...) {
  fprintf(stderr, "panic: ");
  va_list ap;
  va_start(ap, msg);
  vfprintf(stderr, msg, ap);
  va_end(ap);
  fprintf(stderr, "\n");
  abort();
}

bool eqci(const char *a, const char *b) {
  while (*a) {
    if (toupper(*a) != toupper(*b))
      return false;
    ++a;
    ++b;
  }
  return *b == 0;
}

void upcaseStr(std::string &s) {
  for (char &ch : s)
    ch = (char)toupper(ch);
}
