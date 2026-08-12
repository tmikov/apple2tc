/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// The compiled form of a probe script, shared by the compiler and the VM.
/// Not a public interface -- see `include/apple2tc/probe.h`.

#pragma once

#include <stdint.h>

/* --- Fatal diagnostics ------------------------------------------------------ */

/// Every fatal diagnostic that does not come from the tokenizer/parser (which
/// go through `probe_error` instead, to add `path:line`) goes through here,
/// so "FATAL: " has exactly one spelling for the tests that grep for it.
/// Reports `fmt` after the prefix, adds a trailing newline, and exit(2)s.
/// Never returns.
_Noreturn void probe_fatal(const char *fmt, ...) __attribute__((__format__(__printf__, 1, 2)));

/* --- Tokenizer ------------------------------------------------------------ */

typedef enum {
  TOK_EOF,
  TOK_IDENT,
  TOK_NUMBER,
  TOK_STRING,
  // Punctuation and operators, one per lexeme.
  TOK_LPAREN,
  TOK_RPAREN,
  TOK_LBRACE,
  TOK_RBRACE,
  TOK_COMMA,
  TOK_ASSIGN,
  TOK_PLUS,
  TOK_MINUS,
  TOK_STAR,
  TOK_SLASH,
  TOK_PERCENT,
  TOK_AMP,
  TOK_PIPE,
  TOK_CARET,
  TOK_TILDE,
  TOK_BANG,
  TOK_ANDAND,
  TOK_OROR,
  TOK_EQ,
  TOK_NE,
  TOK_LT,
  TOK_LE,
  TOK_GT,
  TOK_GE,
  TOK_SHL,
  TOK_SHR,
  TOK_AT,
} tok_kind_t;

enum { PROBE_MAX_IDENT = 64, PROBE_MAX_STRING = 256 };

typedef struct {
  tok_kind_t kind;
  uint32_t num; // TOK_NUMBER
  char text[PROBE_MAX_STRING]; // TOK_IDENT, TOK_STRING
} token_t;

typedef struct {
  const char *p;
  const char *path;
  unsigned line;
  token_t tok; // current
} lexer_t;

/// `src` must be NUL-terminated and must outlive the lexer -- `lx->p` walks
/// it directly rather than copying, and reads `p[1]` for two-character
/// operators, which stays in bounds only because the NUL is always there to
/// stop it.
void probe_lex_init(lexer_t *lx, const char *src, const char *path);
/// Advance to the next token, leaving it in `lx->tok`.
void probe_lex_next(lexer_t *lx);
/// Report at the current line and exit(2). Never returns.
_Noreturn void probe_error(lexer_t *lx, const char *fmt, ...)
    __attribute__((__format__(__printf__, 2, 3)));
