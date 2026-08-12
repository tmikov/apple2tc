/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "probe_internal.h"

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

_Noreturn void probe_error(lexer_t *lx, const char *fmt, ...) {
  // Render the caller's message first, then hand the combined string to
  // probe_fatal so "FATAL: " keeps its one spelling; probe_fatal's own
  // format is a plain "%s", so the printf-format checking that matters is
  // the __attribute__ on this function's declaration, checked at its call
  // sites.
  char msg[512];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(msg, sizeof(msg), fmt, ap);
  va_end(ap);
  probe_fatal("%s:%u: %s", lx->path, lx->line, msg);
}

void probe_lex_init(lexer_t *lx, const char *src, const char *path) {
  lx->p = src;
  lx->path = path;
  lx->line = 1;
  probe_lex_next(lx);
}

/// Two-character operators.
static const struct {
  const char *s;
  tok_kind_t k;
} s_ops2[] = {
    {"&&", TOK_ANDAND},
    {"||", TOK_OROR},
    {"==", TOK_EQ},
    {"!=", TOK_NE},
    {"<=", TOK_LE},
    {">=", TOK_GE},
    {"<<", TOK_SHL},
    {">>", TOK_SHR},
};

/// Single-character operators.
static const struct {
  char c;
  tok_kind_t k;
} s_ops1[] = {
    {'(', TOK_LPAREN},  {')', TOK_RPAREN}, {'{', TOK_LBRACE}, {'}', TOK_RBRACE}, {',', TOK_COMMA},
    {'=', TOK_ASSIGN},  {'+', TOK_PLUS},   {'-', TOK_MINUS},  {'*', TOK_STAR},   {'/', TOK_SLASH},
    {'%', TOK_PERCENT}, {'&', TOK_AMP},    {'|', TOK_PIPE},   {'^', TOK_CARET},  {'~', TOK_TILDE},
    {'!', TOK_BANG},    {'<', TOK_LT},     {'>', TOK_GT},     {'@', TOK_AT},
};

void probe_lex_next(lexer_t *lx) {
  // Whitespace and # comments to end of line.
  for (;;) {
    while (*lx->p == ' ' || *lx->p == '\t' || *lx->p == '\r')
      ++lx->p;
    if (*lx->p == '\n') {
      ++lx->line;
      ++lx->p;
      continue;
    }
    if (*lx->p == '#') {
      while (*lx->p && *lx->p != '\n')
        ++lx->p;
      continue;
    }
    break;
  }

  lx->tok.num = 0;
  lx->tok.text[0] = 0;

  if (!*lx->p) {
    lx->tok.kind = TOK_EOF;
    return;
  }

  // $hex or decimal.
  if (*lx->p == '$' || isdigit((unsigned char)*lx->p)) {
    int base = 10;
    if (*lx->p == '$') {
      base = 16;
      ++lx->p;
      if (!isxdigit((unsigned char)*lx->p))
        probe_error(lx, "expected hex digits after '$'");
    }
    // strtoull, not strtoul: unsigned long is only 32 bits on LLP64 targets
    // (Windows), where `v > 0xFFFFFFFFul` can never be true and an
    // out-of-range literal would silently saturate instead of erroring --
    // exactly the kind of platform-dependent divergence this format exists
    // to avoid. unsigned long long is at least 64 bits everywhere, so both
    // the errno check and the magnitude check are meaningful on every
    // target.
    char *end;
    errno = 0;
    unsigned long long v = strtoull(lx->p, &end, base);
    if (errno == ERANGE || v > 0xFFFFFFFFull)
      probe_error(lx, "number out of range");
    // Reject "0x10" and "2frames": a number directly followed by another
    // identifier character is almost certainly a typo, and left alone would
    // lex as two tokens with an error surfacing later, further from the
    // cause.
    if (isalnum((unsigned char)*end) || *end == '_')
      probe_error(lx, "invalid number");
    lx->p = end;
    lx->tok.kind = TOK_NUMBER;
    lx->tok.num = (uint32_t)v;
    return;
  }

  if (isalpha((unsigned char)*lx->p) || *lx->p == '_') {
    const char *start = lx->p;
    while (isalnum((unsigned char)*lx->p) || *lx->p == '_')
      ++lx->p;
    size_t n = (size_t)(lx->p - start);
    if (n >= PROBE_MAX_IDENT)
      probe_error(lx, "identifier too long");
    memcpy(lx->tok.text, start, n);
    lx->tok.text[n] = 0;
    lx->tok.kind = TOK_IDENT;
    return;
  }

  if (*lx->p == '"') {
    ++lx->p;
    size_t n = 0;
    while (*lx->p && *lx->p != '"') {
      char c = *lx->p++;
      // A raw newline means the string was never closed; a raw CR would
      // lex silently and then vanish from a `diff` of the report. Checked
      // before escape processing, or a "\n" escape would look like one.
      if (c == '\n' || c == '\r')
        probe_error(lx, "raw newline or CR in string literal");
      if (c == '\\') {
        // `*lx->p++` may read the terminating NUL, if `\` is the last
        // character before it, and then compute a pointer one past that
        // NUL. Both are well-defined: reading the NUL is reading the last
        // in-bounds element of the (NUL-terminated, per probe_lex_init's
        // contract) source, and computing one-past-the-end of it is legal
        // even though dereferencing it would not be (C11 6.5.6p8). We never
        // dereference it: the NUL matches none of the cases below, so
        // `default:` always fires, and `probe_error` never returns.
        switch (*lx->p++) {
        case 'n':
          c = '\n';
          break;
        case 't':
          c = '\t';
          break;
        case '\\':
          c = '\\';
          break;
        case '"':
          c = '"';
          break;
        default:
          probe_error(lx, "unknown escape in string");
          break;
        }
      }
      if (n + 1 >= PROBE_MAX_STRING)
        probe_error(lx, "string literal too long");
      lx->tok.text[n++] = c;
    }
    if (*lx->p != '"')
      probe_error(lx, "unterminated string literal");
    ++lx->p;
    lx->tok.text[n] = 0;
    lx->tok.kind = TOK_STRING;
    return;
  }

  // Two-character operators are tried before one-character ones, so e.g.
  // "&&" wins over "&" -- that loop ordering is what gives longest match,
  // not any ordering within either table.
  for (size_t i = 0; i != sizeof(s_ops2) / sizeof(s_ops2[0]); ++i) {
    if (lx->p[0] == s_ops2[i].s[0] && lx->p[1] == s_ops2[i].s[1]) {
      lx->p += 2;
      lx->tok.kind = s_ops2[i].k;
      return;
    }
  }
  for (size_t i = 0; i != sizeof(s_ops1) / sizeof(s_ops1[0]); ++i) {
    if (lx->p[0] == s_ops1[i].c) {
      ++lx->p;
      lx->tok.kind = s_ops1[i].k;
      return;
    }
  }

  unsigned char bad = (unsigned char)*lx->p;
  if (isprint(bad))
    probe_error(lx, "unexpected character '%c' (0x%02X)", bad, bad);
  else
    probe_error(lx, "unexpected character 0x%02X", bad);
}
