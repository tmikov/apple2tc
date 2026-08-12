/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "probe_internal.h"

#include <stdbool.h>
#include <stdio.h>
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

/* --- Statements and installation (stubs; Tasks 4 and 5) ------------------- */

/// Replaced in Task 4.
static void parse_block(parser_t *P) {
  expect(P, TOK_LBRACE, "'{'");
  expect(P, TOK_RBRACE, "'}'");
}

/// Replaced in Task 5.
static void parse_install(parser_t *P) {
  probe_error(&P->lx, "install: not yet implemented");
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
