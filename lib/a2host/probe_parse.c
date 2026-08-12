/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "probe_internal.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  script_t *sc;
  lexer_t lx;
  probe_t *cur; ///< probe being compiled, for parameter lookup; NULL outside
  unsigned depth; ///< current parse_expr nesting; see PROBE_MAX_EXPR_DEPTH
  // Set by resolve_name() the first time some probe resolves a given
  // register-named identifier *as a register* (i.e. neither a parameter nor
  // a counter matched first). parse_counter consults this: without it,
  // `probe p(v = x) { }` followed later by `counter x` would let `x` mean
  // the X register in `p` and counter 0 in every probe after it, two
  // meanings sharing one spelling depending on where the counter happened to
  // be declared -- exactly the kind of silent divergence probes exist to
  // catch, just moved into the script that defines them.
  bool reg_resolved[REG_PC + 1];
} parser_t;

/// Recursion limit for parse_expr/parse_primary. Far beyond anything a
/// hand-written script needs, but phase 3 has apple2tc generating scripts,
/// and an unbounded parenthesis run recurses this parser straight into a
/// stack overflow instead of the diagnostic a malformed script deserves.
enum { PROBE_MAX_EXPR_DEPTH = 250 };

/* --- Emitting ------------------------------------------------------------- */

/// Returns the index of the emitted cell, so a branch operand can be patched.
static uint32_t emit(parser_t *P, uint32_t cell) {
  if (P->sc->ncode == PROBE_MAX_CODE)
    probe_error(&P->lx, "probe script too large");
  P->sc->code[P->sc->ncode] = cell;
  return P->sc->ncode++;
}

static void emit_op1(parser_t *P, opcode_t op, uint32_t operand) {
  emit(P, op);
  emit(P, operand);
}

/* --- Name lookup ---------------------------------------------------------- */

static int find_counter(script_t *sc, const char *name) {
  for (unsigned i = 0; i != sc->ncounters; ++i)
    if (strcmp(sc->counters[i].name, name) == 0)
      return (int)i;
  return -1;
}

static int find_probe(script_t *sc, const char *name) {
  for (unsigned i = 0; i != sc->nprobes; ++i)
    if (strcmp(sc->probes[i].name, name) == 0)
      return (int)i;
  return -1;
}

static int find_param(probe_t *pr, const char *name) {
  if (!pr)
    return -1;
  for (unsigned i = 0; i != pr->nparams; ++i)
    if (strcmp(pr->params[i], name) == 0)
      return (int)i;
  return -1;
}

static const struct {
  const char *name;
  reg_t reg;
} s_regs[] = {
    {"a", REG_A},
    {"x", REG_X},
    {"y", REG_Y},
    {"sp", REG_SP},
    {"sr", REG_SR},
    {"pc", REG_PC},
};

/// Returns an index into s_regs, not a reg_t -- callers that want the
/// reg_t use s_regs[i].reg, so a reordering of the table can never silently
/// desync the two.
static int find_reg(const char *name) {
  for (size_t i = 0; i != sizeof(s_regs) / sizeof(s_regs[0]); ++i)
    if (strcmp(name, s_regs[i].name) == 0)
      return (int)i;
  return -1;
}

typedef enum { NAME_NONE, NAME_PARAM, NAME_COUNTER, NAME_REG } name_kind_t;

typedef struct {
  name_kind_t kind;
  uint32_t index; ///< param slot, counter index, or reg_t, depending on kind
} resolved_name_t;

/// The one place an identifier is resolved to a parameter, a counter, or a
/// register -- params, then counters, then registers, so a declared name
/// shadows a register of the same name. parse_primary is the only caller
/// today, but Task 4's assignment statement needs the exact same order (an
/// earlier version of this parser looked up registers first for reads only,
/// which let `counter y` read fine while `y = y + 1` wrote the counter and
/// read the Y register -- two different things sharing one spelling). Task 4
/// must call this instead of re-deriving the order, or that bug is one
/// keystroke away from coming back.
static resolved_name_t resolve_name(parser_t *P, const char *name) {
  int idx = find_param(P->cur, name);
  if (idx >= 0)
    return (resolved_name_t){NAME_PARAM, (uint32_t)idx};
  idx = find_counter(P->sc, name);
  if (idx >= 0)
    return (resolved_name_t){NAME_COUNTER, (uint32_t)idx};
  int ri = find_reg(name);
  if (ri >= 0) {
    reg_t reg = s_regs[ri].reg;
    P->reg_resolved[reg] = true; // see the comment on parser_t::reg_resolved
    return (resolved_name_t){NAME_REG, (uint32_t)reg};
  }
  return (resolved_name_t){NAME_NONE, 0};
}

/* --- Token helpers -------------------------------------------------------- */

static void expect(parser_t *P, tok_kind_t k, const char *what) {
  if (P->lx.tok.kind != k)
    probe_error(&P->lx, "expected %s", what);
  probe_lex_next(&P->lx);
}

static bool accept(parser_t *P, tok_kind_t k) {
  if (P->lx.tok.kind != k)
    return false;
  probe_lex_next(&P->lx);
  return true;
}

static bool is_kw(parser_t *P, const char *kw) {
  return P->lx.tok.kind == TOK_IDENT && strcmp(P->lx.tok.text, kw) == 0;
}

/// Copies an identifier into a `PROBE_MAX_IDENT`-sized table slot. The
/// source is a `TOK_IDENT`, whose length `probe_lex_next` already bounds to
/// under `PROBE_MAX_IDENT`, so this should never actually truncate; the
/// explicit check (rather than trusting that and letting `snprintf` truncate
/// silently) is what makes that a guarantee instead of a hope -- an `assert`
/// would do the same in a debug build, but `NDEBUG` compiles it away in
/// exactly the release build that ships.
static void copy_ident(char dst[PROBE_MAX_IDENT], const char *src) {
  size_t n = strlen(src);
  if (n >= PROBE_MAX_IDENT)
    probe_fatal("identifier too long");
  memcpy(dst, src, n + 1);
}

/* --- Expressions ---------------------------------------------------------- */

/// Binding power per binary operator; higher binds tighter. C precedence.
static int binop_prec(tok_kind_t k) {
  switch (k) {
  case TOK_OROR:
    return 1;
  case TOK_ANDAND:
    return 2;
  case TOK_PIPE:
    return 3;
  case TOK_CARET:
    return 4;
  case TOK_AMP:
    return 5;
  case TOK_EQ:
  case TOK_NE:
    return 6;
  case TOK_LT:
  case TOK_LE:
  case TOK_GT:
  case TOK_GE:
    return 7;
  case TOK_SHL:
  case TOK_SHR:
    return 8;
  case TOK_PLUS:
  case TOK_MINUS:
    return 9;
  case TOK_STAR:
  case TOK_SLASH:
  case TOK_PERCENT:
    return 10;
  default:
    return 0;
  }
}

// `&&` and `||` are not handled here. Every other operator in this grammar
// -- literals, counters, parameters, registers, peek8/peek16/hash, and every
// arithmetic and bitwise operator -- can produce any value, not just 0/1;
// only the six comparisons and `!` are guaranteed to. So OP_AND/OP_OR cannot
// stand in for them without losing truthiness (`2 && 1` is not `2 & 1` --
// the former is 1, the latter is 0). `&&` and `||` are also the only
// operators that must skip evaluating their right operand (so that e.g.
// `x != 0 && 100 / x > 5` is safe), which a single opcode consuming two
// already-pushed values could never do. parse_expr compiles them to
// branches instead; reaching either case below means that dispatch broke.
static opcode_t binop_opcode(tok_kind_t k) {
  switch (k) {
  case TOK_OROR:
  case TOK_ANDAND:
    probe_fatal(
        "internal error: %s should have been compiled as a branch", k == TOK_OROR ? "||" : "&&");
  case TOK_PIPE:
    return OP_OR;
  case TOK_AMP:
    return OP_AND;
  case TOK_CARET:
    return OP_XOR;
  case TOK_EQ:
    return OP_EQ;
  case TOK_NE:
    return OP_NE;
  case TOK_LT:
    return OP_LT;
  case TOK_LE:
    return OP_LE;
  case TOK_GT:
    return OP_GT;
  case TOK_GE:
    return OP_GE;
  case TOK_SHL:
    return OP_SHL;
  case TOK_SHR:
    return OP_SHR;
  case TOK_PLUS:
    return OP_ADD;
  case TOK_MINUS:
    return OP_SUB;
  case TOK_STAR:
    return OP_MUL;
  case TOK_SLASH:
    return OP_DIV;
  case TOK_PERCENT:
    return OP_MOD;
  default:
    probe_fatal("internal error: not a binary operator");
  }
}

static void parse_expr(parser_t *P, int min_prec);

static void parse_primary(parser_t *P) {
  token_t t = P->lx.tok;

  if (accept(P, TOK_NUMBER)) {
    emit_op1(P, OP_PUSH_LIT, t.num);
    return;
  }
  if (accept(P, TOK_LPAREN)) {
    parse_expr(P, 1);
    expect(P, TOK_RPAREN, "')'");
    return;
  }
  if (accept(P, TOK_MINUS)) {
    parse_expr(P, 11); // unary binds tighter than any binary operator
    emit(P, OP_NEG);
    return;
  }
  if (accept(P, TOK_TILDE)) {
    parse_expr(P, 11);
    emit(P, OP_BITNOT);
    return;
  }
  if (accept(P, TOK_BANG)) {
    parse_expr(P, 11);
    emit(P, OP_NOT);
    return;
  }

  if (t.kind != TOK_IDENT)
    probe_error(&P->lx, "expected an expression");
  probe_lex_next(&P->lx);

  if (strcmp(t.text, "peek8") == 0 || strcmp(t.text, "peek16") == 0) {
    expect(P, TOK_LPAREN, "'(' after peek");
    parse_expr(P, 1);
    expect(P, TOK_RPAREN, "')'");
    emit(P, strcmp(t.text, "peek8") == 0 ? OP_PEEK8 : OP_PEEK16);
    return;
  }
  if (strcmp(t.text, "hash") == 0) {
    expect(P, TOK_LPAREN, "'(' after hash");
    parse_expr(P, 1);
    expect(P, TOK_COMMA, "',' between hash bounds");
    parse_expr(P, 1);
    expect(P, TOK_RPAREN, "')'");
    emit(P, OP_HASH);
    return;
  }

  resolved_name_t rn = resolve_name(P, t.text);
  switch (rn.kind) {
  case NAME_PARAM:
    emit_op1(P, OP_LOAD_PARAM, rn.index);
    return;
  case NAME_COUNTER:
    emit_op1(P, OP_LOAD_COUNTER, rn.index);
    return;
  case NAME_REG:
    emit_op1(P, OP_LOAD_REG, rn.index);
    return;
  case NAME_NONE:
    break;
  }
  probe_error(&P->lx, "unknown name '%s'", t.text);
}

static void parse_expr(parser_t *P, int min_prec) {
  if (++P->depth > PROBE_MAX_EXPR_DEPTH)
    probe_error(&P->lx, "expression nested too deeply");

  parse_primary(P);
  for (;;) {
    tok_kind_t k = P->lx.tok.kind;
    int prec = binop_prec(k);
    if (prec < min_prec || prec == 0)
      break;
    probe_lex_next(&P->lx);

    if (k == TOK_ANDAND || k == TOK_OROR) {
      // C semantics: the right operand is not evaluated unless it can still
      // change the result, so guarding a division or a peek behind a test
      // actually works (`x != 0 && 100 / x > 5`). JZ and JNZ each pop the
      // value they test, and every path below -- both short-circuit exits
      // and the fall-through -- pushes exactly one replacement value before
      // joining at `j_end`, so the stack has exactly one value again
      // regardless of which path was taken.
      //
      // `&&`: if the left operand is zero, jump straight to pushing 0.
      // Otherwise evaluate the right operand; if that is zero, same jump,
      // same result. If neither was zero, fall through to pushing 1.
      // `||` is the mirror image, testing for nonzero and pushing 1 first.
      opcode_t test = (k == TOK_ANDAND) ? OP_JZ : OP_JNZ;
      uint32_t short_value = (k == TOK_ANDAND) ? 0 : 1;
      uint32_t fall_value = (k == TOK_ANDAND) ? 1 : 0;

      emit(P, test);
      uint32_t j_short1 = emit(P, 0);
      parse_expr(P, prec + 1); // left-associative
      emit(P, test);
      uint32_t j_short2 = emit(P, 0);
      emit_op1(P, OP_PUSH_LIT, fall_value);
      emit(P, OP_JMP);
      uint32_t j_end = emit(P, 0);
      P->sc->code[j_short1] = P->sc->ncode;
      P->sc->code[j_short2] = P->sc->ncode;
      emit_op1(P, OP_PUSH_LIT, short_value);
      P->sc->code[j_end] = P->sc->ncode;
      continue;
    }

    parse_expr(P, prec + 1); // left-associative
    emit(P, binop_opcode(k));
  }

  --P->depth;
}

/* --- printf ----------------------------------------------------------- */

/// probe_error always reports the lexer's *current* line, but by the time
/// count_conversions or the printf argument-count check runs, the lexer has
/// already stepped past the closing ')' -- and past however many lines the
/// argument list itself spans -- so calling probe_error directly from either
/// would blame the wrong line. This formats the message the same way and
/// forwards it through probe_error with the wanted line swapped in first;
/// probe_error never returns, so there is nothing to restore afterwards.
static _Noreturn void probe_error_at(parser_t *P, unsigned line, const char *fmt, ...)
    __attribute__((__format__(__printf__, 3, 4)));
static _Noreturn void probe_error_at(parser_t *P, unsigned line, const char *fmt, ...) {
  char msg[512];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(msg, sizeof(msg), fmt, ap);
  va_end(ap);
  P->lx.line = line;
  probe_error(&P->lx, "%s", msg);
}

/// Count conversions, rejecting anything whose rendering is not a plain
/// integer. `%s` would need string values on the stack, `%n` writes through a
/// pointer, and floats bring locale into an equivalence oracle. \p line is
/// the format string's own source line (see probe_error_at above), not
/// necessarily the lexer's current one.
static unsigned count_conversions(parser_t *P, unsigned line, const char *fmt) {
  unsigned n = 0;
  for (const char *p = fmt; *p; ++p) {
    if (*p != '%')
      continue;
    ++p;
    if (*p == '%')
      continue;
    while (*p == '0' || *p == '-' || *p == '+' || *p == ' ')
      ++p;
    // Width, capped: an uncapped width compiles today but becomes a real
    // problem the moment the VM's printf actually runs it -- a script that
    // wrote "%2000000000d" would ask for a two-gigabyte write into the
    // report. 999 is far past anything a real report column needs.
    unsigned width = 0;
    while (*p >= '0' && *p <= '9') {
      width = width * 10 + (unsigned)(*p - '0');
      if (width > 999)
        probe_error_at(P, line, "conversion width too large (max 999)");
      ++p;
    }
    if (*p == '.')
      // A dedicated message: falling through to the default case below
      // would report "unsupported conversion '%.'", which does not tell the
      // reader that dropping the precision (not the whole conversion) fixes
      // it.
      probe_error_at(P, line, "precision is not supported in a printf conversion");
    switch (*p) {
    case 'd':
    case 'u':
    case 'x':
    case 'X':
      ++n;
      break;
    case 0:
      probe_error_at(P, line, "format string ends inside a conversion");
      break;
    default:
      probe_error_at(P, line, "unsupported conversion '%%%c'; use %%d %%u %%x %%X", *p);
    }
  }
  return n;
}

static uint32_t intern_format(parser_t *P, const char *fmt) {
  script_t *sc = P->sc;
  for (unsigned i = 0; i != sc->nformats; ++i)
    if (strcmp(sc->formats[i], fmt) == 0)
      return i;
  if (sc->nformats == PROBE_MAX_FORMATS)
    probe_error(&P->lx, "too many format strings");
  char *copy = (char *)malloc(strlen(fmt) + 1);
  if (!copy)
    probe_fatal("out of memory interning a format string");
  strcpy(copy, fmt);
  sc->formats[sc->nformats] = copy;
  return sc->nformats++;
}

static void parse_printf(parser_t *P) {
  expect(P, TOK_LPAREN, "'(' after printf");
  if (P->lx.tok.kind != TOK_STRING)
    probe_error(&P->lx, "printf needs a literal format string");
  // The line the format string itself is on, so diagnostics about its
  // content (below) blame it rather than wherever the argument list ends.
  unsigned fmt_line = P->lx.line;
  char fmt[PROBE_MAX_STRING];
  // fmt is sized to match token_t::text, so this copy cannot truncate --
  // but "cannot" only as long as the two stay in sync, which is exactly the
  // kind of guarantee copy_ident's comment above argues a runtime check
  // earns and an assumption does not. Here the coupling is compile-time (two
  // fixed-size arrays), so a _Static_assert is the cheapest honest version:
  // a silently truncated format would lose its trailing content -- e.g. a
  // dropped `\n` -- which in a comparison report reads as the two engines
  // merely disagreeing on whitespace.
  _Static_assert(
      sizeof(fmt) == sizeof(P->lx.tok.text),
      "printf's format buffer must be exactly as large as a string token");
  snprintf(fmt, sizeof(fmt), "%s", P->lx.tok.text);
  probe_lex_next(&P->lx);

  unsigned argc = 0;
  while (accept(P, TOK_COMMA)) {
    parse_expr(P, 1);
    ++argc;
  }
  expect(P, TOK_RPAREN, "')' after the printf arguments");

  unsigned want = count_conversions(P, fmt_line, fmt);
  if (want != argc)
    probe_error_at(P, fmt_line, "format needs %u argument(s), %u given", want, argc);

  emit(P, OP_PRINTF);
  emit(P, intern_format(P, fmt));
  emit(P, argc);
}

/* --- Statements ------------------------------------------------------- */

static void parse_stmt(parser_t *P);

static void parse_block(parser_t *P) {
  expect(P, TOK_LBRACE, "'{'");
  while (!accept(P, TOK_RBRACE)) {
    if (P->lx.tok.kind == TOK_EOF)
      probe_error(&P->lx, "unterminated block");
    parse_stmt(P);
  }
}

static void parse_if(parser_t *P) {
  expect(P, TOK_LPAREN, "'(' after if");
  parse_expr(P, 1);
  expect(P, TOK_RPAREN, "')' after the condition");

  emit(P, OP_JZ);
  uint32_t jz_operand = emit(P, 0); // patched below
  parse_stmt(P);

  if (is_kw(P, "else")) {
    probe_lex_next(&P->lx);
    emit(P, OP_JMP);
    uint32_t jmp_operand = emit(P, 0);
    P->sc->code[jz_operand] = P->sc->ncode;
    parse_stmt(P);
    P->sc->code[jmp_operand] = P->sc->ncode;
  } else {
    P->sc->code[jz_operand] = P->sc->ncode;
  }
}

static void parse_stmt(parser_t *P) {
  if (P->lx.tok.kind == TOK_LBRACE) {
    parse_block(P);
    return;
  }
  if (is_kw(P, "if")) {
    probe_lex_next(&P->lx);
    parse_if(P);
    return;
  }
  if (is_kw(P, "else"))
    // Reached only when no enclosing parse_if consumed this "else": it is a
    // reserved word (s_reserved above), so without this check it would fall
    // all the way to the assignment path and report "unknown name 'else'",
    // which is technically true but not what a reader needs to hear.
    probe_error(&P->lx, "'else' without a matching 'if'");
  if (is_kw(P, "printf")) {
    probe_lex_next(&P->lx);
    parse_printf(P);
    return;
  }
  if (is_kw(P, "stop")) {
    probe_lex_next(&P->lx);
    emit(P, OP_STOP);
    return;
  }
  if (is_kw(P, "key")) {
    probe_lex_next(&P->lx);
    parse_expr(P, 1);
    emit(P, OP_KEY);
    return;
  }
  if (is_kw(P, "inc")) {
    probe_lex_next(&P->lx);
    if (P->lx.tok.kind != TOK_IDENT)
      probe_error(&P->lx, "expected a counter name after 'inc'");
    // Through resolve_name, not a bare find_counter: re-deriving the lookup
    // order here is exactly the params/counters/registers desync Task 3's
    // extraction of resolve_name exists to prevent (see its comment).
    resolved_name_t rn = resolve_name(P, P->lx.tok.text);
    if (rn.kind == NAME_NONE)
      // Distinct from the "not one" message below: this name is not
      // anything, so "needs a counter, but 'zz' is not one" would wrongly
      // imply 'zz' exists as some other kind of name.
      probe_error(&P->lx, "unknown name '%s'", P->lx.tok.text);
    if (rn.kind != NAME_COUNTER)
      probe_error(&P->lx, "'inc' needs a counter, but '%s' is not one", P->lx.tok.text);
    probe_lex_next(&P->lx);
    emit_op1(P, OP_LOAD_COUNTER, rn.index);
    emit_op1(P, OP_PUSH_LIT, 1);
    emit(P, OP_ADD);
    emit_op1(P, OP_STORE_COUNTER, rn.index);
    return;
  }

  // Assignment: <param> = e or <counter> = e, both resolved through
  // resolve_name so they agree with every read (and with `inc` above) on
  // which name means what.
  if (P->lx.tok.kind == TOK_IDENT) {
    resolved_name_t rn = resolve_name(P, P->lx.tok.text);
    switch (rn.kind) {
    case NAME_PARAM:
    case NAME_COUNTER: {
      opcode_t store_op = rn.kind == NAME_PARAM ? OP_STORE_PARAM : OP_STORE_COUNTER;
      probe_lex_next(&P->lx);
      expect(P, TOK_ASSIGN, "'=' in an assignment");
      parse_expr(P, 1);
      emit_op1(P, store_op, rn.index);
      return;
    }
    case NAME_REG:
      // Writing a register would perturb the very machine the probe is
      // observing -- the same reason PEEK8 goes through ram_peek rather than
      // a live read. A probe able to alter what it watches is no longer a
      // neutral observer, which defeats the reason probes exist.
      probe_error(
          &P->lx,
          "cannot assign to register '%s': probes must not alter machine state",
          P->lx.tok.text);
    case NAME_NONE:
      // A bare "expected a statement" below would not say which name was
      // wrong; naming it here is what makes the diagnostic useful.
      probe_error(&P->lx, "unknown name '%s'", P->lx.tok.text);
    }
  }

  probe_error(&P->lx, "expected a statement");
}

/* --- Installation ----------------------------------------------------- */

typedef enum { ADD_SITE_OK, ADD_SITE_DUP, ADD_SITE_FULL } add_site_result_t;

/// Appends one (addr, probe_id) to sc->pending_sites, growing it as needed
/// and capping it at PROBE_MAX_SITE_DECLS. Never raises a diagnostic itself
/// -- unlike every other "too many X" check in this file, one of this
/// function's two callers (add_sites_from_file) holds an open FILE* that
/// must be closed before probe_error_at runs, so the decision of *how* to
/// report ADD_SITE_DUP/ADD_SITE_FULL is left to the caller.
static add_site_result_t try_add_site(script_t *sc, uint16_t addr, uint32_t probe_id) {
  // Same probe, same address, twice: not a hash-table concern (the chain
  // just gets two links) but a script bug -- the probe now fires twice per
  // hit, which for a report compared byte-for-byte is a real divergence, not
  // a harmless redundancy.
  for (uint32_t i = 0; i != sc->npending_sites; ++i)
    if (sc->pending_sites[i].addr == addr && sc->pending_sites[i].probe_id == probe_id)
      return ADD_SITE_DUP;

  if (sc->npending_sites == PROBE_MAX_SITE_DECLS)
    return ADD_SITE_FULL;
  if (sc->npending_sites == sc->pending_sites_cap) {
    uint32_t new_cap = sc->pending_sites_cap ? sc->pending_sites_cap * 2 : 64;
    if (new_cap > PROBE_MAX_SITE_DECLS)
      new_cap = PROBE_MAX_SITE_DECLS;
    site_decl_t *grown = (site_decl_t *)realloc(sc->pending_sites, new_cap * sizeof(site_decl_t));
    if (!grown)
      probe_fatal("out of memory recording an install site");
    sc->pending_sites = grown;
    sc->pending_sites_cap = new_cap;
  }
  sc->pending_sites[sc->npending_sites].addr = addr;
  sc->pending_sites[sc->npending_sites].probe_id = probe_id;
  ++sc->npending_sites;
  return ADD_SITE_OK;
}

/// Reports what try_add_site's non-OK results mean, in the one shared wording
/// used everywhere sites are declared directly in the script (the range/
/// single-address form in parse_install below). \p line is the source line
/// to blame -- not necessarily the lexer's *current* line: by the time a
/// range like `install p at $100-$3000` finishes filling and this can fail
/// with "too many install sites", the lexer has already stepped past the
/// range and onto whatever follows it (possibly the next statement, on the
/// next line), the same reason parse_printf's fmt_line exists.
static void add_site(parser_t *P, unsigned line, uint16_t addr, uint32_t probe_id) {
  switch (try_add_site(P->sc, addr, probe_id)) {
  case ADD_SITE_OK:
    return;
  case ADD_SITE_DUP:
    probe_error_at(
        P, line, "probe '%s' is already installed at $%04X", P->sc->probes[probe_id].name, addr);
  case ADD_SITE_FULL:
    probe_error_at(
        P,
        line,
        "too many install sites (max %u) while installing '%s'",
        (unsigned)PROBE_MAX_SITE_DECLS,
        P->sc->probes[probe_id].name);
  }
}

/// Reads one logical line, however long, growing `*buf`/`*cap` as needed so a
/// long line is never silently split into several -- `fgets` alone truncates
/// at the buffer size and hands back the remainder as if it started a new
/// line, which is exactly how a long comment (this project's own sites.txt
/// opens with one) can spill non-'#' text into what looks like a fresh
/// address line. Returns false only when nothing at all was read (genuine
/// EOF, or a read error -- the caller tells the two apart with ferror() once
/// this returns false for good).
static bool read_line(FILE *f, char **buf, size_t *cap) {
  size_t len = 0;
  for (;;) {
    if (len + 2 > *cap) { // room for at least one more byte plus the NUL
      size_t new_cap = *cap ? *cap * 2 : 256;
      char *grown = (char *)realloc(*buf, new_cap);
      if (!grown)
        probe_fatal("out of memory reading a site list");
      *buf = grown;
      *cap = new_cap;
    }
    if (!fgets(*buf + len, (int)(*cap - len), f))
      return len != 0; // EOF/error; whatever was collected is the last line
    len += strlen(*buf + len);
    if (len > 0 && (*buf)[len - 1] == '\n')
      return true;
    // fgets stopped only because the buffer filled, with more of the same
    // line still unread (no trailing '\n' yet, and not EOF, or the next
    // fgets would have returned NULL above) -- loop and grow.
  }
}

/// One hex address per line; '#' comments and blank lines ignored. This is the
/// form that makes cross-engine comparison work: both sides install at exactly
/// the same finite set, so the reports have the same length and diff compares
/// them directly, with no subsequence matching -- which only holds if a
/// malformed list fails loudly instead of silently installing a subset (or,
/// worse, extra addresses it never named), the reason every rejection below
/// aborts the whole load rather than skipping one line. See read_file()'s
/// comment in probe.c for the same reasoning applied to the script file
/// itself.
static void add_sites_from_file(parser_t *P, const char *name, uint32_t probe_id) {
  unsigned at_line = P->lx.line; // the `@"..."` token's line; stable for the
                                 // whole call, since the lexer does not
                                 // advance past it until after we return
  // Resolved relative to the script, so a script and its site list travel
  // together. Split on both '/' and '\\': this project ships a2run/a2emu as
  // separate Windows executables (see a2host_api.h), so a script's own path
  // may arrive with either separator.
  char path[512];
  const char *slash = strrchr(P->lx.path, '/');
  const char *bslash = strrchr(P->lx.path, '\\');
  if (bslash && (!slash || bslash > slash))
    slash = bslash;
  int n;
  if (slash)
    n = snprintf(path, sizeof(path), "%.*s%s", (int)(slash - P->lx.path + 1), P->lx.path, name);
  else
    n = snprintf(path, sizeof(path), "%s", name);
  if (n < 0 || (size_t)n >= sizeof(path))
    probe_error_at(P, at_line, "site list path too long: '%s'", name);

  FILE *f = fopen(path, "rt");
  if (!f)
    probe_error_at(P, at_line, "cannot open site list '%s'", path);

  char *line = NULL;
  size_t line_cap = 0;
  unsigned lineno = 0;
  unsigned naddrs = 0;
  while (read_line(f, &line, &line_cap)) {
    ++lineno;
    char *s = line;
    while (*s == ' ' || *s == '\t')
      ++s;
    if (*s == '#' || *s == '\n' || *s == '\r' || !*s)
      continue;

    // Plain hex digits only: no leading '+'/'-' and no "0x" prefix, both of
    // which strtoul's base-16 mode would otherwise accept silently, letting
    // a typo like "-0" parse as the legitimate-looking address $0000.
    if (!isxdigit((unsigned char)*s) || (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))) {
      fclose(f);
      free(line);
      probe_error_at(P, at_line, "%s:%u: expected a 16-bit hex address", path, lineno);
    }
    char *end;
    unsigned long v = strtoul(s, &end, 16);
    if (v > 0xFFFF) {
      fclose(f);
      free(line);
      probe_error_at(P, at_line, "%s:%u: expected a 16-bit hex address", path, lineno);
    }
    // Nothing but trailing whitespace and an optional comment may follow the
    // address -- "300 400" on one line must fail, not silently install $0300
    // and drop $0400.
    char *t = end;
    while (*t == ' ' || *t == '\t')
      ++t;
    if (*t && *t != '#' && *t != '\n' && *t != '\r') {
      fclose(f);
      free(line);
      probe_error_at(P, at_line, "%s:%u: unexpected text after the address", path, lineno);
    }

    // Not add_site: this loop holds `f` and `line` open, and both diagnostics
    // below need them closed/freed first, the same reason every other error
    // exit in this loop does its own cleanup instead of calling probe_error
    // straight away.
    add_site_result_t r = try_add_site(P->sc, (uint16_t)v, probe_id);
    if (r != ADD_SITE_OK) {
      fclose(f);
      free(line);
      if (r == ADD_SITE_DUP)
        probe_error_at(
            P,
            at_line,
            "%s:%u: probe '%s' is already installed at $%04X",
            path,
            lineno,
            P->sc->probes[probe_id].name,
            (unsigned)v);
      else
        probe_error_at(
            P,
            at_line,
            "%s:%u: too many install sites (max %u) while installing '%s'",
            path,
            lineno,
            (unsigned)PROBE_MAX_SITE_DECLS,
            P->sc->probes[probe_id].name);
    }
    ++naddrs;
  }
  // ferror(), not just "the loop ended": a read error (e.g. `@"a-directory"`,
  // which fopen("rt") happily opens on Linux) must not be mistaken for a
  // clean, empty file -- both would otherwise install nothing and exit 0. Not
  // strerror(errno): ferror() does not guarantee errno was left meaningful
  // (same caution as read_file's, in probe.c).
  bool had_error = ferror(f) != 0;
  fclose(f);
  free(line);
  if (had_error)
    probe_error_at(P, at_line, "error reading site list '%s'", path);
  if (naddrs == 0)
    probe_error_at(P, at_line, "site list '%s' names no addresses", path);
}

static void parse_install(parser_t *P) {
  if (P->lx.tok.kind != TOK_IDENT)
    probe_error(&P->lx, "expected a probe name after install");
  int probe_id = find_probe(P->sc, P->lx.tok.text);
  if (probe_id < 0)
    probe_error(&P->lx, "unknown probe '%s'", P->lx.tok.text);
  probe_lex_next(&P->lx);

  if (!is_kw(P, "at"))
    probe_error(&P->lx, "expected 'at' after the probe name");
  probe_lex_next(&P->lx);

  do {
    if (accept(P, TOK_AT)) {
      // A quoted string, so a path may contain dots and slashes without the
      // lexer needing a mode.
      if (P->lx.tok.kind != TOK_STRING)
        probe_error(&P->lx, "expected a quoted file name after '@'");
      add_sites_from_file(P, P->lx.tok.text, (uint32_t)probe_id);
      probe_lex_next(&P->lx);
      continue;
    }
    if (P->lx.tok.kind != TOK_NUMBER)
      probe_error(&P->lx, "expected an address, a range or '@\"file\"'");
    // The address/range's own line, so a diagnostic raised after
    // probe_lex_next has moved the lexer on (e.g. "too many install sites",
    // raised from inside the fill loop below) still blames the statement
    // instead of whatever follows it.
    unsigned site_line = P->lx.line;
    uint32_t lo = P->lx.tok.num;
    if (lo > 0xFFFF)
      probe_error(&P->lx, "address out of range");
    probe_lex_next(&P->lx);

    uint32_t hi = lo;
    if (accept(P, TOK_MINUS)) {
      if (P->lx.tok.kind != TOK_NUMBER)
        probe_error(&P->lx, "expected the end of the range");
      hi = P->lx.tok.num;
      if (hi > 0xFFFF)
        probe_error(&P->lx, "address out of range");
      if (hi < lo)
        probe_error(&P->lx, "range ends before it starts");
      probe_lex_next(&P->lx);
    }
    for (uint32_t a = lo; a <= hi; ++a)
      add_site(P, site_line, (uint16_t)a, (uint32_t)probe_id);
  } while (accept(P, TOK_COMMA));
}

/* --- Declarations ------------------------------------------------------- */

/// Names a counter or parameter declaration cannot use. Not the registers --
/// those are ordinary identifiers a declaration is allowed to shadow, per
/// parse_primary's resolution order above. These are different: each is
/// matched by strcmp before identifier lookup ever runs (peek8/peek16/hash
/// in parse_primary, the rest in probe_parse_script and, from Task 4, the
/// statement parser), so declaring one would create a name that silently
/// means something else depending on where it is used, which shadowing
/// cannot make coherent.
static const char *const s_reserved[] = {
    "peek8",
    "peek16",
    "hash",
    "counter",
    "probe",
    "install",
    "if",
    "else",
    "printf",
    "inc",
    "key",
    "stop",
};

static bool is_reserved(const char *name) {
  for (size_t i = 0; i != sizeof(s_reserved) / sizeof(s_reserved[0]); ++i)
    if (strcmp(name, s_reserved[i]) == 0)
      return true;
  return false;
}

static void parse_counter(parser_t *P) {
  if (P->lx.tok.kind != TOK_IDENT)
    probe_error(&P->lx, "expected a counter name");
  if (P->sc->ncounters == PROBE_MAX_COUNTERS)
    probe_error(&P->lx, "too many counters");
  if (is_reserved(P->lx.tok.text))
    probe_error(&P->lx, "'%s' is a reserved name", P->lx.tok.text);
  if (find_counter(P->sc, P->lx.tok.text) >= 0)
    probe_error(&P->lx, "counter '%s' already declared", P->lx.tok.text);
  int ri = find_reg(P->lx.tok.text);
  if (ri >= 0 && P->reg_resolved[s_regs[ri].reg])
    probe_error(
        &P->lx,
        "counter '%s' is declared after a probe resolved '%s' as a register",
        P->lx.tok.text,
        P->lx.tok.text);

  counter_t *c = &P->sc->counters[P->sc->ncounters++];
  copy_ident(c->name, P->lx.tok.text);
  c->init = 0;
  probe_lex_next(&P->lx);

  if (accept(P, TOK_ASSIGN)) {
    // Only a literal: an initial value must not depend on machine state,
    // which does not exist yet when counters are initialised.
    bool neg = accept(P, TOK_MINUS);
    if (P->lx.tok.kind != TOK_NUMBER)
      probe_error(&P->lx, "a counter initialiser must be a literal");
    // -(int32_t)num is UB at num == 0x80000000 (INT32_MIN's magnitude has no
    // positive int32_t representation); unsigned negation has no such edge.
    c->init = neg ? 0u - P->lx.tok.num : P->lx.tok.num;
    probe_lex_next(&P->lx);
  }
}

static void parse_probe(parser_t *P) {
  if (P->lx.tok.kind != TOK_IDENT)
    probe_error(&P->lx, "expected a probe name");
  if (P->sc->nprobes == PROBE_MAX_PROBES)
    probe_error(&P->lx, "too many probes");
  if (is_reserved(P->lx.tok.text))
    probe_error(&P->lx, "'%s' is a reserved name", P->lx.tok.text);
  if (find_probe(P->sc, P->lx.tok.text) >= 0)
    probe_error(&P->lx, "probe '%s' already declared", P->lx.tok.text);

  probe_t *pr = &P->sc->probes[P->sc->nprobes++];
  memset(pr, 0, sizeof(*pr));
  copy_ident(pr->name, P->lx.tok.text);
  probe_lex_next(&P->lx);

  P->cur = pr;
  pr->init_offset = P->sc->ncode;

  expect(P, TOK_LPAREN, "'(' after the probe name");
  if (!accept(P, TOK_RPAREN)) {
    do {
      if (P->lx.tok.kind != TOK_IDENT)
        probe_error(&P->lx, "expected a parameter name");
      if (pr->nparams == PROBE_MAX_PARAMS)
        probe_error(&P->lx, "too many parameters");
      if (is_reserved(P->lx.tok.text))
        probe_error(&P->lx, "'%s' is a reserved name", P->lx.tok.text);
      if (find_param(pr, P->lx.tok.text) >= 0)
        probe_error(&P->lx, "duplicate parameter '%s'", P->lx.tok.text);

      // find_param above scans only the first pr->nparams entries, and that
      // count is not bumped until after this initialiser is parsed below --
      // so the name is already stored (and visible to parameters that
      // follow it), but not yet visible to itself. Do not move the
      // `pr->nparams = ...` up next to copy_ident: that would let `v = v`
      // compile as a LOAD_PARAM of the slot it is itself initialising.
      unsigned slot = pr->nparams;
      copy_ident(pr->params[slot], P->lx.tok.text);
      probe_lex_next(&P->lx);
      expect(P, TOK_ASSIGN, "'=' after the parameter name");
      parse_expr(P, 1);
      pr->nparams = (uint8_t)(slot + 1);
    } while (accept(P, TOK_COMMA));
    expect(P, TOK_RPAREN, "')' after the parameter list");
  }

  pr->body_offset = P->sc->ncode;
  parse_block(P);
  emit(P, OP_END);
  pr->end_offset = P->sc->ncode;
  P->cur = NULL;
}

void probe_parse_script(script_t *sc, const char *src, const char *path) {
  parser_t P = {0}; // zeroes reg_resolved along with everything else
  P.sc = sc;
  P.cur = NULL;
  P.depth = 0;
  probe_lex_init(&P.lx, src, path);

  while (P.lx.tok.kind != TOK_EOF) {
    if (is_kw(&P, "counter")) {
      probe_lex_next(&P.lx);
      parse_counter(&P);
    } else if (is_kw(&P, "probe")) {
      probe_lex_next(&P.lx);
      parse_probe(&P);
    } else if (is_kw(&P, "install")) {
      probe_lex_next(&P.lx);
      parse_install(&P);
    } else {
      probe_error(&P.lx, "expected 'counter', 'probe' or 'install'");
    }
  }
}
