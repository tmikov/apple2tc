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

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

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
///
/// **Operand pop order for every binary opcode below**: both operands are
/// pushed in source (left-to-right) order -- the left/first operand is
/// pushed first and so sits deeper on the stack, the right/second operand is
/// pushed second and so is on top. Each binary opcode pops the top (right)
/// operand first, then the one beneath it (left), and computes
/// `left <op> right`. ADD/MUL/AND/OR/XOR/EQ/NE are commutative, so the order
/// is unobservable for them; SUB/DIV/MOD/SHL/SHR/LT/LE/GT/GE are not, and a
/// VM that pops any of those backwards would still pass a test suite that
/// only checks the compiler, since nothing here executes bytecode -- see
/// each opcode below for its specific order. OP_HASH follows the same
/// convention (`hash(start, end)`: start is pushed first, so end pops
/// first), documented separately below because its two operands are not
/// named "left"/"right" in the language.
typedef enum {
  OP_END,
  OP_PUSH_LIT, // <value>
  OP_LOAD_PARAM, // <index>
  OP_LOAD_COUNTER, // <index>
  OP_STORE_PARAM, // <index>: pops the value to store
  OP_STORE_COUNTER, // <index>: pops the value to store
  OP_LOAD_REG, // <reg_t>
  OP_PEEK8, // pops addr
  OP_PEEK16, // pops addr
  OP_HASH, // pops end, then start; source order is hash(start, end)
  OP_ADD,
  OP_SUB, // pops right, then left; result is left - right
  OP_MUL,
  OP_DIV, // pops right (divisor), then left (dividend); result is left / right
  OP_MOD, // pops right, then left; result is left % right
  OP_AND,
  OP_OR,
  OP_XOR,
  OP_SHL, // pops right (shift count), then left (value); result is left << right
  OP_SHR, // pops right (shift count), then left (value); result is left >> right
  OP_EQ,
  OP_NE,
  OP_LT, // pops right, then left; result is left < right
  OP_LE, // pops right, then left; result is left <= right
  OP_GT, // pops right, then left; result is left > right
  OP_GE, // pops right, then left; result is left >= right
  OP_NOT,
  OP_BITNOT,
  OP_NEG,
  OP_JMP, // <target>: absolute cell index into script_t::code, not an offset
  OP_JZ, // <target> (absolute, as above); pops the tested value
  OP_JNZ, // <target> (absolute, as above); pops the tested value
  // <fmt index> <argc>; pops argc values. Arguments are pushed in source
  // order, so the first argument (matching the format's first conversion) is
  // deepest on the stack and the last argument is on top -- popping them
  // top-first therefore yields the *last* argument first. To fill the
  // format's conversions in source order, a VM must either collect the argc
  // popped values and consume them back-to-front, or pop into slots indexed
  // from argc-1 down to 0.
  OP_PRINTF,
  OP_KEY, // pops stamp
  OP_STOP,
} opcode_t;

typedef enum { REG_A, REG_X, REG_Y, REG_SP, REG_SR, REG_PC } reg_t;

/// No maximum simultaneous stack depth is computed or recorded anywhere in
/// this compiled form -- there is no PROBE_MAX_STACK_DEPTH and nothing walks
/// a probe's bytecode to size one. In practice it is bounded by a
/// combination of limits that each exist for an unrelated reason:
/// PROBE_MAX_EXPR_DEPTH (a naive push-then-combine codegen needs stack depth
/// proportional to expression nesting, not width, since a binary opcode
/// consumes both its operands the moment they are both pushed), the up-to
/// PROBE_MAX_PARAMS values live in a probe's parameter frame for the
/// duration of its initializers and body (see probe_t::init_offset below),
/// and however many arguments accumulate on the stack before one OP_PRINTF
/// (bounded only loosely, by PROBE_MAX_STRING's cap on how many `%`
/// conversions a format string can contain). None of that is tracked as a
/// single number anywhere. A VM should either compute a per-probe maximum by
/// walking the bytecode before running it, or provision a stack generous
/// enough for the worst case implied by those limits (or grow it
/// dynamically) -- it must not assume the compiler already did this sizing.

enum {
  PROBE_MAX_COUNTERS = 64,
  PROBE_MAX_PROBES = 256,
  PROBE_MAX_PARAMS = 16,
  PROBE_MAX_FORMATS = 256,
  PROBE_MAX_CODE = 65536,
  /// Recursion limit for parse_expr/parse_primary in probe_parse.c. Far
  /// beyond anything a hand-written script needs, but phase 3 has apple2tc
  /// generating scripts, and an unbounded parenthesis run recurses that
  /// parser straight into a stack overflow instead of the diagnostic a
  /// malformed script deserves.
  PROBE_MAX_EXPR_DEPTH = 250,
  /// Recursion limit for parse_stmt in probe_parse.c: block nesting
  /// (`{ { ... } }`) and `if`/`else` nesting both recurse through it once per
  /// level, with nothing else to stop them. Measured under `ulimit -s 1024`
  /// (Windows's default 1 MB main-thread stack, a supported target): 16,000
  /// nested blocks or 16,000 nested `if`s both segfault -- the `if` case is
  /// bounded only by accident, since 16,000 `if`s emit 32,000 code cells,
  /// comfortably under PROBE_MAX_CODE. 250 matches PROBE_MAX_EXPR_DEPTH
  /// above: far beyond anything a hand-written script needs, and nowhere
  /// near the ~16,000-level floor measured above.
  PROBE_MAX_STMT_DEPTH = 250,
};

typedef struct {
  char name[PROBE_MAX_IDENT];
  uint32_t init;
} counter_t;

typedef struct {
  char name[PROBE_MAX_IDENT];
  char params[PROBE_MAX_PARAMS][PROBE_MAX_IDENT];
  uint8_t nparams;
  /// Where this probe's code starts: `nparams` parameter-initializer
  /// expressions, one per declared parameter in declaration order, each
  /// leaving exactly one value on the stack and none of them popped before
  /// falling through to body_offset (see parse_probe in probe_parse.c). By
  /// the time execution reaches body_offset, all `nparams` values pushed
  /// since init_offset *are* the parameter frame -- there is no separate
  /// copy into named storage. OP_LOAD_PARAM i / OP_STORE_PARAM i address slot
  /// `i` of that frame, meaning the stack position `frame_base + i`, where
  /// `frame_base` is the stack depth on entry to init_offset. That is a
  /// fixed offset from the bottom of the frame, not from wherever the stack
  /// top happens to be while a later initializer or a body statement is
  /// mid-expression (execution always returns to depth `frame_base +
  /// nparams` between statements, but can be deeper in between) -- a VM
  /// needs a frame-pointer-style base for `frame_base`, not a
  /// top-of-stack-relative index.
  uint32_t init_offset;
  uint32_t body_offset; ///< where init_offset falls through once all nparams
                        ///< initializers have run and their values are on
                        ///< the stack; equals init_offset for a
                        ///< parameterless probe (nothing to fall through)
  uint32_t end_offset; ///< one past the trailing OP_END
  uint32_t hits; ///< reserved for the VM: intended as a per-probe fire
                 ///< count, incremented once each time this probe's body
                 ///< runs. Never written by the compiler -- every probe_t
                 ///< here has hits == 0 -- and not read by anything today.
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

/* --- Output ----------------------------------------------------------------- */

/// Where a probe's `printf` output goes: whatever `--probe-out=` set, or
/// stdout if nothing did. `static` in probe.c until here; the VM
/// (probe_vm_printf) is the only caller -- probe_dump takes an explicit
/// `FILE *` and never reaches this.
FILE *probe_out(void);

/// The mnemonic for \p op, or NULL if \p op is not one of the opcode_t
/// enumerators (corrupt bytecode, not a missing table entry -- see the
/// comment on this function's definition in probe.c). `static` there until
/// here; probe_vm_run's default case is the second caller, so a diagnostic
/// can name an opcode instead of just numbering it.
const char *opname(opcode_t op);

/* --- VM ----------------------------------------------------------------- */

enum {
  /// Ceiling on OP_PRINTF's argc, both here (the VM's fixed-size argument
  /// buffer) and in parse_printf (a compile-time rejection, so a script that
  /// exceeds it fails to load rather than overflowing a stack array at
  /// runtime). Far beyond anything a real report line needs -- PROBE_MAX_STRING
  /// (256 bytes) loosely bounds how many `%` conversions a format string can
  /// even contain, and 32 sits comfortably under that ceiling with room to
  /// spare.
  PROBE_MAX_PRINTF_ARGS = 32,
};

/// Execute bytecode from \p ip. Used for the address path with
/// ip = init_offset; part 3's macro path will enter at body_offset with the
/// parameter values already pushed. Takes no probe_t: the caller owns hit
/// counting, and passing one the VM does not read would only warn under
/// -Wunused-parameter.
void probe_vm_run(const script_t *sc, uint32_t ip);
/// Record the address dispatch reached, for LOAD_REG REG_PC.
void probe_vm_set_pc(uint16_t pc);
/// Reset every counter to its `counter x = <init>` value (0 if the script
/// left it implicit). Call once, after probe_build_sites() has run, when a
/// script loads -- counter storage is process-lifetime in the VM (see
/// s_counters in probe_vm.c), so this is what gives a freshly loaded script
/// a clean start rather than whatever a previous load left behind.
void probe_vm_init_counters(const script_t *sc);
//
// probe_stop_requested and probe_deliver_keys are declared by Task 5, which
// is also the task that defines them -- not here, ahead of time: a
// declaration with no definition anywhere yet documents behaviour (Task 5's
// own draft of this file said "True once a probe executed `stop`" while
// OP_STOP did not exist) that this task does not implement.
