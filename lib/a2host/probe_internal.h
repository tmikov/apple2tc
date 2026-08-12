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

/* --- Bytecode ------------------------------------------------------------- */

/// One opcode per cell, operands in following cells. Variable length, so
/// control flow and new operations can be added without changing the shape.
typedef enum {
  OP_END,
  OP_PUSH_LIT, // <value>
  OP_LOAD_PARAM, // <index>
  OP_LOAD_COUNTER, // <index>
  OP_STORE_PARAM, // <index>
  OP_STORE_COUNTER, // <index>
  OP_LOAD_REG, // <reg_t>
  OP_PEEK8, // pops addr
  OP_PEEK16, // pops addr
  OP_HASH, // pops end, then start; source order is hash(start, end)
  OP_ADD,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_MOD,
  OP_AND,
  OP_OR,
  OP_XOR,
  OP_SHL,
  OP_SHR,
  OP_EQ,
  OP_NE,
  OP_LT,
  OP_LE,
  OP_GT,
  OP_GE,
  OP_NOT,
  OP_BITNOT,
  OP_NEG,
  OP_JMP, // <target>
  OP_JZ, // <target>  pops
  OP_JNZ, // <target>  pops
  OP_PRINTF, // <fmt index> <argc>  pops argc
  OP_KEY, // pops stamp
  OP_STOP,
} opcode_t;

typedef enum { REG_A, REG_X, REG_Y, REG_SP, REG_SR, REG_PC } reg_t;

/// The opcode name table in probe.c must stay in step with opcode_t.
#define PROBE_NUM_OPCODES ((int)OP_STOP + 1)

enum {
  PROBE_MAX_COUNTERS = 64,
  PROBE_MAX_PROBES = 256,
  PROBE_MAX_PARAMS = 16,
  PROBE_MAX_FORMATS = 256,
  PROBE_MAX_CODE = 65536,
};

typedef struct {
  char name[PROBE_MAX_IDENT];
  uint32_t init;
} counter_t;

typedef struct {
  char name[PROBE_MAX_IDENT];
  char params[PROBE_MAX_PARAMS][PROBE_MAX_IDENT];
  uint8_t nparams;
  uint32_t init_offset; ///< initializer expressions; fall through to the body
  uint32_t body_offset;
  uint32_t end_offset; ///< one past the trailing OP_END
  uint32_t hits;
} probe_t;

/// A hash slot: an installed address, and the head of its chain. Chains live
/// in a separate array rather than in spare slots, so a chain node can never
/// sit in the path of a linear probe.
typedef struct {
  uint16_t addr;
  uint16_t used; ///< 0 free, 1 occupied; addr 0 is otherwise ambiguous
  uint32_t first; ///< index into `insts`
} slot_t;

/// One installed probe. Several at one address chain in script order.
typedef struct {
  uint32_t probe_id;
  uint32_t next; ///< index into `insts`, or PROBE_NO_SITE
} inst_t;

#define PROBE_NO_SITE 0xFFFFFFFFu

enum { PROBE_MAX_SITE_DECLS = 8192 };

/// One `install` target: a probe at one address. The parser appends these to
/// script_t::pending_sites; probe_build_sites() hashes them into slots/insts
/// and then frees the array -- it is parse-time scratch, dead the moment the
/// table exists, so it does not belong in the persistent script_t shape (nor,
/// worse, in file-scope storage that would sit in every generated program's
/// BSS whether or not that program's build ever installs a probe).
typedef struct {
  uint16_t addr;
  uint32_t probe_id;
} site_decl_t;

/// The whole compiled script. One instance, heap-allocated by
/// probe_load_script() into a file-scope pointer in probe.c (`s_script`,
/// NULL until a script is loaded) rather than kept as a file-scope value --
/// see that pointer's comment for why.
typedef struct {
  counter_t counters[PROBE_MAX_COUNTERS];
  unsigned ncounters;
  probe_t probes[PROBE_MAX_PROBES];
  unsigned nprobes;
  char *formats[PROBE_MAX_FORMATS];
  unsigned nformats;
  uint32_t code[PROBE_MAX_CODE];
  uint32_t ncode;
  site_decl_t *pending_sites; ///< parse-time scratch; see site_decl_t above
  uint32_t npending_sites;
  uint32_t pending_sites_cap;
  slot_t *slots; ///< open-addressed, power-of-two, NULL until installed
  uint32_t slot_mask;
  inst_t *insts; ///< one per install declaration
  uint32_t ninsts;
  uint32_t nsites; ///< distinct addresses, i.e. occupied slots
} script_t;

void probe_parse_script(script_t *sc, const char *src, const char *path);

/* --- Install sites --------------------------------------------------------- */

/// Hashes sc->pending_sites into sc->slots/sc->insts and frees
/// sc->pending_sites (see site_decl_t's comment). A no-op, leaving sc->slots
/// NULL, if no `install` was ever parsed.
void probe_build_sites(script_t *sc);
/// Find the slot for \p addr: its own, or the first free one. Precondition:
/// `sc->slots != NULL`, i.e. at least one site has been installed
/// (probe_build_sites leaves it NULL otherwise, and this does not check --
/// callers on a script with no installs must guard with `sc->nsites != 0` /
/// `probe_installed()` first).
uint32_t probe_find_slot(const script_t *sc, uint16_t addr);
