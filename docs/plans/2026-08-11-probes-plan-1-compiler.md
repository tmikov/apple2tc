# Probe Compiler Implementation Plan (phase 1, part 1 of 2)

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Compile probe scripts into bytecode, observable through a new
`--probe-dump` flag, with no VM and nothing executing yet.

**Architecture:** A probe script declares counters and probes and installs
probes at addresses. Each probe compiles to one flat `uint32_t` instruction
stream with two entry offsets — the parameter initializers, and the body. This
plan builds the lexer, a precedence-climbing expression parser, the code
generator, the site hash table and a bytecode disassembler. `--probe-dump`
prints the compiled form and exits, which is what every test in this plan
diffs against a baseline. Part 2 adds the VM and wires dispatch into the
engines.

**Tech Stack:** C11, built into the existing `a2host` library. Tests are shell
+ `diff`, following `tests/run-tests.sh`.

**Spec:** `docs/plans/2026-08-11-probes-design.md`

---

## File Structure

| file | responsibility |
|---|---|
| `include/apple2tc/probe.h` | public API — everything outside `lib/a2host` uses only this |
| `lib/a2host/probe_internal.h` | opcodes, compiled-form structs, shared by compiler and (later) VM |
| `lib/a2host/probe_lex.c` | tokenizer |
| `lib/a2host/probe_parse.c` | parser, expression precedence, code generation |
| `lib/a2host/probe.c` | script loading, counter/probe/site tables, disassembler, diagnostics |
| `lib/a2host/a2host.c` | modified: `--probe=`, `--probe-out=`, `--probe-dump` options |
| `lib/a2host/CMakeLists.txt` | modified: new sources |
| `tests/probe/*.probe`, `tests/probe/*.expected` | script/baseline pairs |
| `tests/run-tests.sh` | modified: probe compile + rejection tests |

Part 2 adds `lib/a2host/probe_vm.c` and modifies `system-inc.h`,
`system2-inc.h` and `lib/engine6502/engine6502.cpp`.

---

## Task 1: Skeleton, options, and a failing load

**Files:**
- Create: `include/apple2tc/probe.h`
- Create: `lib/a2host/probe.c`
- Modify: `lib/a2host/CMakeLists.txt:6-10`
- Modify: `lib/a2host/a2host.c` (option loop at line 451, help at line 424)
- Modify: `tests/run-tests.sh`
- Create: `tests/.gitignore` containing `probe-tmp/` — `set -e` aborts before the
  `rm -rf`, so a failing run leaves the directory behind and `tests/` has no
  ignore file today

- [ ] **Step 1: Write the failing test**

Append to `tests/run-tests.sh`, immediately before the final `echo "Success!"`:

```sh
# --- Probes -----------------------------------------------------------------
#
# The probe compiler is tested through --probe-dump, which compiles a script,
# prints the compiled form and exits without running anything. Every case below
# is a script/baseline pair; a change in generated code shows up as a diff.

mkdir -p probe-tmp

# Compile a script and diff the result against its baseline.
probe_dump_test() {
  # $1: base name under probe/
  if ! $a2run --probe="probe/$1.probe" --probe-dump > "probe-tmp/$1.txt"; then
    echo "FAIL: --probe-dump failed on probe/$1.probe" >&2
    exit 1
  fi
  diff -q "probe/$1.expected" "probe-tmp/$1.txt"
}

probe_dump_test empty

# Assert that a2run rejects something, with the specific diagnostic we expect.
# Matching the message rather than just "FATAL" is deliberate: during review,
# two of these tests passed because a different check fired first and satisfied
# a loose grep. Tasks 3-6 reuse this.
expect_probe_reject() {
  # $1: description, $2: expected substring, $3...: a2run arguments
  desc="$1"; want="$2"; shift 2
  if $a2run "$@" > /dev/null 2>probe-tmp/err.txt; then
    echo "FAIL: a2run accepted $desc" >&2
    exit 1
  fi
  if ! grep -q 'FATAL' probe-tmp/err.txt || ! grep -q -- "$want" probe-tmp/err.txt; then
    echo "FAIL: rejected $desc, but not with the expected diagnostic '$want':" >&2
    cat probe-tmp/err.txt >&2
    exit 1
  fi
}

expect_probe_reject "a missing probe script" "cannot open probe script" \
  --probe=probe/does-not-exist.probe --probe-dump
# Without this, the empty-script case above passes with the --probe= handler
# deleted, because a dump with no script prints the same three lines.
expect_probe_reject "--probe-dump with no script" "--probe-dump requires" \
  --probe-dump
# Note: no --probe-dump here. With it, the dump check fires first and this test
# would pass even with the --probe-out validation removed entirely.
expect_probe_reject "--probe-out with no script" "--probe-out requires" \
  --probe-out=probe-tmp/out.txt

rm -rf probe-tmp
```

Create `tests/probe/empty.probe` containing only a comment:

```
# An empty script compiles to nothing.
```

Create `tests/probe/empty.expected`:

```
counters: 0
probes: 0
sites: 0
```

- [ ] **Step 2: Run it to verify it fails**

```
cd tests && ./run-tests.sh ../cmake-build-debug
```

Expected: FAIL — `a2run: unknown option --probe=probe/empty.probe`, exit non-zero.

- [ ] **Step 3: Write the public header**

Create `include/apple2tc/probe.h`:

```c
/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// Probes: small programs that run at chosen points in the emulated program,
/// so two implementations can be compared without a shared clock. See
/// `docs/plans/2026-08-11-probes-design.md`.
///
/// This is the whole surface outside `lib/a2host`. The engines call
/// `probe_dispatch()`; the host calls everything else.

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Compile a script. Reports to stderr and exits non-zero on any error: a
/// probe that silently fails to load would leave a report that reads as
/// agreement.
void probe_load_script(const char *path);

/// Where `printf` output goes; stdout until set. probe.c owns the file and
/// closes it, so a report survives an abort mid-run -- which matters when the
/// thing being diagnosed is a divergence.
void probe_set_output_path(const char *path);
void probe_close_output(void);

/// Print the compiled form of the loaded script and return. Used by
/// `--probe-dump` and by every compiler test.
void probe_dump(FILE *f);

/// True once a script with at least one installed site is loaded. The engine
/// consults this to decide whether it needs per-instruction callbacks.
bool probe_installed(void);

/// Run whatever is installed at \p pc. Returns immediately when nothing is,
/// which is the overwhelmingly common case.
void probe_dispatch(uint16_t pc);

/// Report probes that never fired, to stderr. A probe bound to an address that
/// does not exist in one of the two programs would otherwise pass quietly.
void probe_report_unfired(void);

#ifdef __cplusplus
} // extern "C"
#endif
```

- [ ] **Step 4: Write the skeleton implementation**

Create `lib/a2host/probe.c`:

```c
/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/probe.h"
#include "probe_internal.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE *s_out = NULL;

/// Every probe failure goes through here, so the diagnostic has one spelling
/// -- which matters because the tests grep for it. Never returns: a probe that
/// fails to load would otherwise leave a report that reads as agreement.
void probe_fatal(const char *fmt, ...) {
  va_list ap;
  fputs("FATAL: ", stderr);
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  fputc('\n', stderr);
  exit(2);
}

static FILE *probe_out(void) {
  return s_out ? s_out : stdout;
}

void probe_set_output_path(const char *path) {
  FILE *f = fopen(path, "wt");
  if (!f)
    probe_fatal("cannot open probe output '%s': %s", path, strerror(errno));
  s_out = f;
}

void probe_close_output(void) {
  if (s_out) {
    fclose(s_out);
    s_out = NULL;
  }
}

void probe_load_script(const char *path) {
  FILE *f = fopen(path, "rt");
  if (!f)
    probe_fatal("cannot open probe script '%s': %s", path, strerror(errno));
  fclose(f);
}

void probe_dump(FILE *f) {
  fprintf(f, "counters: 0\n");
  fprintf(f, "probes: 0\n");
  fprintf(f, "sites: 0\n");
}

bool probe_installed(void) {
  return false;
}

void probe_dispatch(uint16_t pc) {
  (void)pc;
}

void probe_report_unfired(void) {}
```

`probe_out()` is unused until the VM in part 2 — but the header promises stdout
by default, and `s_out` starts NULL, so writing the mapping now is what stops
that promise from becoming a null dereference for whoever writes the VM.

Declare `probe_fatal` in `probe_internal.h` so later tasks share it:

```c
/// Report and exit(2). Declared here so the lexer and parser share one
/// spelling of the diagnostic.
void probe_fatal(const char *fmt, ...);
```

- [ ] **Step 5: Add the sources to the build**

In `lib/a2host/CMakeLists.txt`, replace the `add_library(a2host ...)` block:

```cmake
add_library(a2host
  a2host.c ${A2TC_INC}/a2host_api.h
  probe.c probe_internal.h ${A2TC_INC}/probe.h
  ${A2TC_INC}/a2engine.h ${A2TC_INC}/a2host.h ${A2TC_INC}/system.h
  )
```

Create an empty-for-now `lib/a2host/probe_internal.h`:

```c
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
```

- [ ] **Step 6: Wire the options**

In `lib/a2host/a2host.c`, add near the other file-scope statics (after
`static FILE *kbd_file_ = NULL;` at line 38):

```c
static bool probe_dump_ = false;
```

Add the include at the top with the others:

```c
#include "apple2tc/probe.h"
```

In `a2host_parse_args`, inside the `for` loop before the final rejection, add:

```c
    if (strncmp(arg, "--probe=", 8) == 0) {
      probe_load_script(arg + 8);
      continue;
    }
    if (strncmp(arg, "--probe-out=", 12) == 0) {
      probe_out_path_ = arg + 12;
      continue;
    }
    if (strcmp(arg, "--probe-dump") == 0) {
      probe_dump_ = true;
      continue;
    }
```

The `--probe-out=` handler only records the path, because otherwise the result
would depend on whether it appeared before or after `--probe=` on the command
line. Validation happens once, after the loop:

```c
  // Both of these would otherwise succeed while doing nothing: a dump with no
  // script prints a well-formed, entirely fictional empty report and exits 0,
  // which is the failure this whole facility exists to make impossible.
  if (probe_dump_ && !probe_script_loaded_)
    probe_fatal("--probe-dump requires --probe=<script>");
  if (probe_out_path_ && !probe_script_loaded_)
    probe_fatal("--probe-out requires --probe=<script>");
  if (probe_out_path_)
    probe_set_output_path(probe_out_path_);
  if (probe_dump_) {
    probe_dump(stdout);
    exit(0);
  }
```

This sits at the end of `a2host_parse_args`, before any other post-parse
validation. `--probe-dump` compiles a script and exits; it must not be made to
satisfy constraints belonging to a run that will never happen.

`probe_script_loaded_` is a static set by the `--probe=` handler, and
`probe_fatal` needs declaring in `probe.h` rather than `probe_internal.h` for
`a2host.c` to reach it — or keep it internal and have `a2host.c` call a small
`probe_require_script(const char *why)`. Either is fine; prefer whichever keeps
`probe.h` smaller.

Add `probe_close_output()` to `a2host_shutdown`, beside the existing
`fclose(hash_file_)`.

In `print_help`, before `engine_print_help();`:

```c
  printf(" --probe=path     Load a probe script\n");
  printf(" --probe-out=p    Write probe output to the given file\n");
  printf(" --probe-dump     Print the compiled probe script and exit\n");
```

- [ ] **Step 7: Run the tests**

```
ninja -C cmake-build-debug && cd tests && ./run-tests.sh ../cmake-build-debug
```

Expected: `Success!`

- [ ] **Step 8: Commit**

```bash
git add include/apple2tc/probe.h lib/a2host/probe.c lib/a2host/probe_internal.h \
        lib/a2host/CMakeLists.txt lib/a2host/a2host.c tests/.gitignore \
        tests/run-tests.sh tests/probe/empty.probe tests/probe/empty.expected
git commit -m "probe: script loading skeleton and --probe-dump"
```

---

## Task 2: Tokenizer

**Files:**
- Create: `lib/a2host/probe_lex.c`
- Modify: `lib/a2host/probe_internal.h`
- Modify: `lib/a2host/CMakeLists.txt`

No test of its own — a tokenizer has no observable behaviour until something
parses. Task 3 tests both together. This is the one task in the plan without a
test step, and it is deliberate: a test that pokes at the lexer's internals
would have to be deleted the moment the parser exists.

- [ ] **Step 1: Declare the token interface**

Append to `lib/a2host/probe_internal.h`:

```c
#include <stdbool.h>
#include <stdio.h>

/* --- Tokenizer ------------------------------------------------------------ */

typedef enum {
  TOK_EOF,
  TOK_IDENT,
  TOK_NUMBER,
  TOK_STRING,
  // Punctuation and operators, one per lexeme.
  TOK_LPAREN, TOK_RPAREN, TOK_LBRACE, TOK_RBRACE, TOK_COMMA, TOK_ASSIGN,
  TOK_PLUS, TOK_MINUS, TOK_STAR, TOK_SLASH, TOK_PERCENT,
  TOK_AMP, TOK_PIPE, TOK_CARET, TOK_TILDE, TOK_BANG,
  TOK_ANDAND, TOK_OROR,
  TOK_EQ, TOK_NE, TOK_LT, TOK_LE, TOK_GT, TOK_GE,
  TOK_SHL, TOK_SHR,
  TOK_DASH, // only produced inside install site lists: $LO-$HI
  TOK_AT,
} tok_kind_t;

enum { PROBE_MAX_IDENT = 64, PROBE_MAX_STRING = 256 };

typedef struct {
  tok_kind_t kind;
  uint32_t num;                      // TOK_NUMBER
  char text[PROBE_MAX_STRING];       // TOK_IDENT, TOK_STRING
  unsigned line;
} token_t;

typedef struct {
  const char *src;
  const char *p;
  const char *path;
  unsigned line;
  token_t tok;      // current
} lexer_t;

void probe_lex_init(lexer_t *lx, const char *src, const char *path);
/// Advance to the next token, leaving it in `lx->tok`.
void probe_lex_next(lexer_t *lx);
/// Report at the current line and exit(2). Never returns.
void probe_error(lexer_t *lx, const char *fmt, ...);
```

- [ ] **Step 2: Implement it**

Create `lib/a2host/probe_lex.c`:

```c
/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "probe_internal.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

void probe_error(lexer_t *lx, const char *fmt, ...) {
  va_list ap;
  fprintf(stderr, "FATAL: %s:%u: ", lx->path, lx->line);
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  fputc('\n', stderr);
  exit(2);
}

void probe_lex_init(lexer_t *lx, const char *src, const char *path) {
  lx->src = src;
  lx->p = src;
  lx->path = path;
  lx->line = 1;
  probe_lex_next(lx);
}

/// Two-character operators, longest match first.
static const struct { const char *s; tok_kind_t k; } s_ops2[] = {
    {"&&", TOK_ANDAND}, {"||", TOK_OROR}, {"==", TOK_EQ}, {"!=", TOK_NE},
    {"<=", TOK_LE},     {">=", TOK_GE},   {"<<", TOK_SHL}, {">>", TOK_SHR},
};

static const struct { char c; tok_kind_t k; } s_ops1[] = {
    {'(', TOK_LPAREN}, {')', TOK_RPAREN}, {'{', TOK_LBRACE}, {'}', TOK_RBRACE},
    {',', TOK_COMMA},  {'=', TOK_ASSIGN}, {'+', TOK_PLUS},   {'-', TOK_MINUS},
    {'*', TOK_STAR},   {'/', TOK_SLASH},  {'%', TOK_PERCENT}, {'&', TOK_AMP},
    {'|', TOK_PIPE},   {'^', TOK_CARET},  {'~', TOK_TILDE},  {'!', TOK_BANG},
    {'<', TOK_LT},     {'>', TOK_GT},     {'@', TOK_AT},
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

  lx->tok.line = lx->line;
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
    char *end;
    unsigned long v = strtoul(lx->p, &end, base);
    if (v > 0xFFFFFFFFul)
      probe_error(lx, "number out of range");
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
      // A raw newline means the string was never closed. Checked before escape
      // processing, or a "\n" escape would look like one -- which would reject
      // essentially every printf in this design.
      if (c == '\n')
        probe_error(lx, "newline in string literal");
      if (c == '\\') {
        switch (*lx->p++) {
        case 'n': c = '\n'; break;
        case 't': c = '\t'; break;
        case '\\': c = '\\'; break;
        case '"': c = '"'; break;
        default: probe_error(lx, "unknown escape in string");
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

  probe_error(lx, "unexpected character '%c'", *lx->p);
}
```

Note `-` lexes as `TOK_MINUS`; the install-site parser treats it as a range
separator in the one context where that is unambiguous. `TOK_DASH` is never
produced and exists only to make that intent readable at the enum; delete it if
part 2 finds no use.

- [ ] **Step 3: Add to the build**

In `lib/a2host/CMakeLists.txt`, add `probe_lex.c` to the `a2host` sources.

- [ ] **Step 4: Verify it compiles**

```
ninja -C cmake-build-debug
```

Expected: builds clean, no warnings.

- [ ] **Step 5: Commit**

```bash
git add lib/a2host/probe_lex.c lib/a2host/probe_internal.h lib/a2host/CMakeLists.txt
git commit -m "probe: tokenizer"
```

---

## Task 3: Bytecode format, counters, and expression compilation

This is the core of the plan. Expressions appear in counter initializers,
parameter initializers, conditions and `printf` arguments, so everything later
depends on it.

**Files:**
- Modify: `lib/a2host/probe_internal.h`
- Create: `lib/a2host/probe_parse.c`
- Modify: `lib/a2host/probe.c`
- Create: `tests/probe/expr.probe`, `tests/probe/expr.expected`
- Modify: `tests/run-tests.sh`

- [ ] **Step 1: Write the failing test**

Create `tests/probe/expr.probe`:

```
# Precedence, associativity and every atom that does not need a running
# machine. The compiled form below is the assertion.
counter frame
counter limit = $1F4

probe e(
  prec  = 1 + 2 * 3,
  paren = (1 + 2) * 3,
  cmp   = frame < limit,
  bool  = frame > 1 && frame < 9,
  bits  = (frame & $F0) >> 4,
  un    = -1 + ~0 + !0,
  mem   = peek8($1F00) + peek16($06),
  reg   = a + x + y + sp + sr + pc,
  hash  = hash($2000, $3FFF)
) {
}
```

Create `tests/probe/expr.expected`. Generate it in step 5 and read it before
committing; the point of the baseline is that a human has looked at the
emitted code once.

Add to the probe section of `tests/run-tests.sh`, after the empty-script case:

```sh
probe_dump_test expr
```

- [ ] **Step 2: Run it to verify it fails**

```
cd tests && ./run-tests.sh ../cmake-build-debug
```

Expected: FAIL — the dump prints `counters: 0` and does not match.

- [ ] **Step 3: Define the compiled form**

Append to `lib/a2host/probe_internal.h`:

```c
/* --- Bytecode ------------------------------------------------------------- */

/// One opcode per cell, operands in following cells. Variable length, so
/// control flow and new operations can be added without changing the shape.
typedef enum {
  OP_END,           //
  OP_PUSH_LIT,      // <value>
  OP_LOAD_PARAM,    // <index>
  OP_LOAD_COUNTER,  // <index>
  OP_STORE_PARAM,   // <index>
  OP_STORE_COUNTER, // <index>
  OP_LOAD_REG,      // <reg_t>
  OP_PEEK8,         // pops addr
  OP_PEEK16,        // pops addr
  OP_HASH,          // pops hi, lo
  OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
  OP_AND, OP_OR, OP_XOR, OP_SHL, OP_SHR,
  OP_EQ, OP_NE, OP_LT, OP_LE, OP_GT, OP_GE,
  OP_NOT, OP_BITNOT, OP_NEG,
  OP_JMP,           // <target>
  OP_JZ,            // <target>  pops
  OP_JNZ,           // <target>  pops
  OP_PRINTF,        // <fmt index> <argc>  pops argc
  OP_KEY,           // pops stamp
  OP_STOP,
} opcode_t;

typedef enum { REG_A, REG_X, REG_Y, REG_SP, REG_SR, REG_PC } reg_t;

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
  uint32_t hits;
} probe_t;

/// A hash slot: an installed address, and the head of its chain. Chains live
/// in a separate array rather than in spare slots, so a chain node can never
/// sit in the path of a linear probe.
typedef struct {
  uint16_t addr;
  uint16_t used;  ///< 0 free, 1 occupied; addr 0 is otherwise ambiguous
  uint32_t first; ///< index into `insts`
} slot_t;

/// One installed probe. Several at one address chain in script order.
typedef struct {
  uint32_t probe_id;
  uint32_t next; ///< index into `insts`, or PROBE_NO_SITE
} inst_t;

#define PROBE_NO_SITE 0xFFFFFFFFu

/// The whole compiled script. One instance, file-scope in probe.c.
typedef struct {
  counter_t counters[PROBE_MAX_COUNTERS];
  unsigned ncounters;
  probe_t probes[PROBE_MAX_PROBES];
  unsigned nprobes;
  char *formats[PROBE_MAX_FORMATS];
  unsigned nformats;
  uint32_t code[PROBE_MAX_CODE];
  uint32_t ncode;
  slot_t *slots;      ///< open-addressed, power-of-two, NULL until installed
  uint32_t slot_mask;
  inst_t *insts;      ///< one per install declaration
  uint32_t ninsts;
  uint32_t nsites;    ///< distinct addresses, i.e. occupied slots
} script_t;

/// The opcode name table in probe.c must stay in step with opcode_t.
#define PROBE_NUM_OPCODES ((int)OP_STOP + 1)

void probe_parse_script(script_t *sc, const char *src, const char *path);
```

- [ ] **Step 4: Implement the parser**

Create `lib/a2host/probe_parse.c`. The expression parser is precedence
climbing: one table, one loop, no per-level functions.

```c
/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "probe_internal.h"

#include <stdlib.h>
#include <string.h>

typedef struct {
  script_t *sc;
  lexer_t lx;
  probe_t *cur;   ///< probe being compiled, for parameter lookup; NULL outside
} parser_t;

/* --- Emitting ------------------------------------------------------------- */

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

/* --- Expressions ---------------------------------------------------------- */

/// Binding power per binary operator; higher binds tighter. C precedence.
static int binop_prec(tok_kind_t k) {
  switch (k) {
  case TOK_OROR: return 1;
  case TOK_ANDAND: return 2;
  case TOK_PIPE: return 3;
  case TOK_CARET: return 4;
  case TOK_AMP: return 5;
  case TOK_EQ: case TOK_NE: return 6;
  case TOK_LT: case TOK_LE: case TOK_GT: case TOK_GE: return 7;
  case TOK_SHL: case TOK_SHR: return 8;
  case TOK_PLUS: case TOK_MINUS: return 9;
  case TOK_STAR: case TOK_SLASH: case TOK_PERCENT: return 10;
  default: return 0;
  }
}

static opcode_t binop_opcode(tok_kind_t k) {
  switch (k) {
  case TOK_OROR: case TOK_PIPE: return OP_OR;
  case TOK_ANDAND: case TOK_AMP: return OP_AND;
  case TOK_CARET: return OP_XOR;
  case TOK_EQ: return OP_EQ;
  case TOK_NE: return OP_NE;
  case TOK_LT: return OP_LT;
  case TOK_LE: return OP_LE;
  case TOK_GT: return OP_GT;
  case TOK_GE: return OP_GE;
  case TOK_SHL: return OP_SHL;
  case TOK_SHR: return OP_SHR;
  case TOK_PLUS: return OP_ADD;
  case TOK_MINUS: return OP_SUB;
  case TOK_STAR: return OP_MUL;
  case TOK_SLASH: return OP_DIV;
  case TOK_PERCENT: return OP_MOD;
  default: abort();
  }
}

static void parse_expr(parser_t *P, int min_prec);

static const struct { const char *name; reg_t reg; } s_regs[] = {
    {"a", REG_A}, {"x", REG_X}, {"y", REG_Y},
    {"sp", REG_SP}, {"sr", REG_SR}, {"pc", REG_PC},
};

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

  // peek8(e), peek16(e), hash(lo, hi)
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

  for (size_t i = 0; i != sizeof(s_regs) / sizeof(s_regs[0]); ++i) {
    if (strcmp(t.text, s_regs[i].name) == 0) {
      emit_op1(P, OP_LOAD_REG, s_regs[i].reg);
      return;
    }
  }

  int idx = find_param(P->cur, t.text);
  if (idx >= 0) {
    emit_op1(P, OP_LOAD_PARAM, (uint32_t)idx);
    return;
  }
  idx = find_counter(P->sc, t.text);
  if (idx >= 0) {
    emit_op1(P, OP_LOAD_COUNTER, (uint32_t)idx);
    return;
  }
  probe_error(&P->lx, "unknown name '%s'", t.text);
}

static void parse_expr(parser_t *P, int min_prec) {
  parse_primary(P);
  for (;;) {
    tok_kind_t k = P->lx.tok.kind;
    int prec = binop_prec(k);
    if (prec < min_prec || prec == 0)
      return;
    probe_lex_next(&P->lx);
    parse_expr(P, prec + 1); // left-associative
    emit(P, binop_opcode(k));
  }
}
```

**`&&` and `||` generate real short-circuit branches**, and are the only
operators that branch — which is why `binop_opcode` does not handle them.
Reaching it with either is an internal error.

An earlier draft mapped them onto `OP_AND`/`OP_OR`, on the reasoning that
comparisons yield 0 or 1. That reasoning was wrong and shipped: only the six
comparisons and `!` yield 0/1, while literals, counters, parameters, registers,
`peek8`, `peek16`, `hash` and every arithmetic and bitwise operator yield
arbitrary values and are all legal operands. `2 && 1` compiled to `2 & 1` = 0.
`OP_AND` does not even preserve truthiness, so `if (frame && 2)` took the wrong
branch. Short-circuiting also makes `x != 0 && 100 / x > 5` safe, which no
non-branching form can be.

With the left operand on the stack, `a && b` emits:

```c
emit(P, OP_JZ);
uint32_t j_false = emit(P, 0);
parse_expr(P, prec + 1);
emit(P, OP_JZ);
uint32_t j_false2 = emit(P, 0);
emit_op1(P, OP_PUSH_LIT, 1);
emit(P, OP_JMP);
uint32_t j_end = emit(P, 0);
P->sc->code[j_false] = P->sc->ncode;
P->sc->code[j_false2] = P->sc->ncode;
emit_op1(P, OP_PUSH_LIT, 0);
P->sc->code[j_end] = P->sc->ncode;
```

`||` is the mirror image, with `JNZ` to a true arm. `JZ`/`JNZ` pop, so the stack
is balanced on every path.

- [ ] **Step 5: Add declaration parsing and generate the baseline**

Continue `lib/a2host/probe_parse.c`:

```c
/* --- Declarations --------------------------------------------------------- */

static void parse_counter(parser_t *P) {
  if (P->lx.tok.kind != TOK_IDENT)
    probe_error(&P->lx, "expected a counter name");
  if (P->sc->ncounters == PROBE_MAX_COUNTERS)
    probe_error(&P->lx, "too many counters");
  if (find_counter(P->sc, P->lx.tok.text) >= 0)
    probe_error(&P->lx, "counter '%s' already declared", P->lx.tok.text);

  counter_t *c = &P->sc->counters[P->sc->ncounters++];
  snprintf(c->name, sizeof(c->name), "%s", P->lx.tok.text);
  c->init = 0;
  probe_lex_next(&P->lx);

  if (accept(P, TOK_ASSIGN)) {
    // Only a literal: an initial value must not depend on machine state, which
    // does not exist yet when counters are initialised.
    bool neg = accept(P, TOK_MINUS);
    if (P->lx.tok.kind != TOK_NUMBER)
      probe_error(&P->lx, "a counter initialiser must be a literal");
    c->init = neg ? (uint32_t)(-(int32_t)P->lx.tok.num) : P->lx.tok.num;
    probe_lex_next(&P->lx);
  }
}

static void parse_probe(parser_t *P) {
  if (P->lx.tok.kind != TOK_IDENT)
    probe_error(&P->lx, "expected a probe name");
  if (P->sc->nprobes == PROBE_MAX_PROBES)
    probe_error(&P->lx, "too many probes");
  if (find_probe(P->sc, P->lx.tok.text) >= 0)
    probe_error(&P->lx, "probe '%s' already declared", P->lx.tok.text);

  probe_t *pr = &P->sc->probes[P->sc->nprobes++];
  memset(pr, 0, sizeof(*pr));
  snprintf(pr->name, sizeof(pr->name), "%s", P->lx.tok.text);
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
      if (find_param(pr, P->lx.tok.text) >= 0)
        probe_error(&P->lx, "duplicate parameter '%s'", P->lx.tok.text);

      // Recorded before the initialiser is parsed, so a parameter cannot refer
      // to itself, but after earlier ones, so it may refer to those.
      unsigned slot = pr->nparams;
      snprintf(pr->params[slot], sizeof(pr->params[slot]), "%s", P->lx.tok.text);
      probe_lex_next(&P->lx);
      expect(P, TOK_ASSIGN, "'=' after the parameter name");
      parse_expr(P, 1);
      pr->nparams = (uint8_t)(slot + 1);
    } while (accept(P, TOK_COMMA));
    expect(P, TOK_RPAREN, "')' after the parameter list");
  }

  pr->body_offset = P->sc->ncode;
  parse_block(P);          // defined in Task 4
  emit(P, OP_END);
  P->cur = NULL;
}

void probe_parse_script(script_t *sc, const char *src, const char *path) {
  parser_t P;
  P.sc = sc;
  P.cur = NULL;
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
      parse_install(&P);   // defined in Task 5
    } else {
      probe_error(&P.lx, "expected 'counter', 'probe' or 'install'");
    }
  }
}
```

Until Task 4 exists, stub `parse_block` as a forward declaration that accepts
`{` `}` only:

```c
static void parse_block(parser_t *P) {
  expect(P, TOK_LBRACE, "'{'");
  expect(P, TOK_RBRACE, "'}'");
}
```

and `parse_install` likewise as `probe_error(&P->lx, "install: not yet
implemented");`. Both are replaced in the next two tasks. Move `parse_block`
and `parse_install` above `parse_probe`/`probe_parse_script` so no forward
declarations are needed.

- [ ] **Step 6: Write the disassembler and wire loading**

Replace the stubs in `lib/a2host/probe.c` with the real script, loader and
dump. The dump format is what every test diffs, so it is deliberately plain.

```c
#include "apple2tc/probe.h"
#include "probe_internal.h"

#include <stdlib.h>
#include <string.h>

static FILE *s_out = NULL;
static script_t s_script;
static bool s_loaded = false;

void probe_set_output(FILE *f) {
  s_out = f;
}

static char *read_file(const char *path) {
  FILE *f = fopen(path, "rb");
  if (!f) {
    fprintf(stderr, "FATAL: cannot open probe script '%s'\n", path);
    exit(2);
  }
  fseek(f, 0, SEEK_END);
  long len = ftell(f);
  fseek(f, 0, SEEK_SET);
  if (len < 0) {
    fprintf(stderr, "FATAL: cannot read probe script '%s'\n", path);
    exit(2);
  }
  char *buf = (char *)malloc((size_t)len + 1);
  if (!buf)
    abort();
  size_t got = fread(buf, 1, (size_t)len, f);
  buf[got] = 0;
  fclose(f);
  return buf;
}

void probe_load_script(const char *path) {
  if (s_loaded) {
    fprintf(stderr, "FATAL: only one probe script may be loaded\n");
    exit(2);
  }
  char *src = read_file(path);
  memset(&s_script, 0, sizeof(s_script));
  probe_parse_script(&s_script, src, path);
  free(src);
  s_loaded = true;
}

/* --- Disassembly ---------------------------------------------------------- */

static const char *const s_opnames[] = {
    "END", "PUSH_LIT", "LOAD_PARAM", "LOAD_COUNTER", "STORE_PARAM",
    "STORE_COUNTER", "LOAD_REG", "PEEK8", "PEEK16", "HASH",
    "ADD", "SUB", "MUL", "DIV", "MOD",
    "AND", "OR", "XOR", "SHL", "SHR",
    "EQ", "NE", "LT", "LE", "GT", "GE",
    "NOT", "BITNOT", "NEG",
    "JMP", "JZ", "JNZ", "PRINTF", "KEY", "STOP",
};

_Static_assert(
    sizeof(s_opnames) / sizeof(s_opnames[0]) == PROBE_NUM_OPCODES,
    "the opcode name table has drifted from opcode_t");

/// Opcodes taking one operand cell. Everything else takes none, except PRINTF.
static bool has_operand(opcode_t op) {
  switch (op) {
  case OP_PUSH_LIT: case OP_LOAD_PARAM: case OP_LOAD_COUNTER:
  case OP_STORE_PARAM: case OP_STORE_COUNTER: case OP_LOAD_REG:
  case OP_JMP: case OP_JZ: case OP_JNZ:
    return true;
  default:
    return false;
  }
}

static uint32_t dump_insn(FILE *f, const script_t *sc, uint32_t ip) {
  opcode_t op = (opcode_t)sc->code[ip];
  fprintf(f, "    %5u: %s", ip, s_opnames[op]);
  ++ip;
  if (op == OP_PRINTF) {
    uint32_t fmt = sc->code[ip++];
    uint32_t argc = sc->code[ip++];
    fprintf(f, " fmt=%u argc=%u", fmt, argc);
  } else if (has_operand(op)) {
    fprintf(f, " %u", sc->code[ip++]);
  }
  fputc('\n', f);
  return ip;
}

void probe_dump(FILE *f) {
  const script_t *sc = &s_script;

  fprintf(f, "counters: %u\n", sc->ncounters);
  for (unsigned i = 0; i != sc->ncounters; ++i)
    fprintf(f, "  %u %s = %u\n", i, sc->counters[i].name, sc->counters[i].init);

  fprintf(f, "formats: %u\n", sc->nformats);
  for (unsigned i = 0; i != sc->nformats; ++i)
    fprintf(f, "  %u \"%s\"\n", i, sc->formats[i]);

  fprintf(f, "probes: %u\n", sc->nprobes);
  for (unsigned i = 0; i != sc->nprobes; ++i) {
    const probe_t *pr = &sc->probes[i];
    fprintf(f, "  %u %s(", i, pr->name);
    for (unsigned p = 0; p != pr->nparams; ++p)
      fprintf(f, "%s%s", p ? ", " : "", pr->params[p]);
    fprintf(f, ") init=%u body=%u\n", pr->init_offset, pr->body_offset);
    uint32_t end = (i + 1 < sc->nprobes) ? sc->probes[i + 1].init_offset : sc->ncode;
    for (uint32_t ip = pr->init_offset; ip < end;)
      ip = dump_insn(f, sc, ip);
  }

  fprintf(f, "sites: %u\n", sc->nsites);
}
```

The `formats:` and `sites:` sections print as empty until Tasks 4 and 5. Update
`tests/probe/empty.expected` to match:

```
counters: 0
formats: 0
probes: 0
sites: 0
```

- [ ] **Step 7: Generate and inspect the baseline**

```
ninja -C cmake-build-debug
cmake-build-debug/tools/a2run/a2run --probe=tests/probe/expr.probe --probe-dump \
  > tests/probe/expr.expected
```

**Read `tests/probe/expr.expected` before continuing.** Check by hand that
`prec` emits `PUSH_LIT 1, PUSH_LIT 2, PUSH_LIT 3, MUL, ADD` and `paren` emits
`PUSH_LIT 1, PUSH_LIT 2, ADD, PUSH_LIT 3, MUL`. If precedence is wrong the
baseline will happily enshrine it.

- [ ] **Step 8: Run the tests**

```
cd tests && ./run-tests.sh ../cmake-build-debug
```

Expected: `Success!`

- [ ] **Step 9: Commit**

```bash
git add lib/a2host/probe_parse.c lib/a2host/probe_internal.h lib/a2host/probe.c \
        lib/a2host/CMakeLists.txt tests/probe/expr.probe tests/probe/expr.expected \
        tests/probe/empty.expected tests/run-tests.sh
git commit -m "probe: expression compiler and bytecode disassembler"
```

---

## Task 4: Statements

**Files:**
- Modify: `lib/a2host/probe_parse.c`
- Create: `tests/probe/stmt.probe`, `tests/probe/stmt.expected`
- Modify: `tests/run-tests.sh`

- [ ] **Step 1: Write the failing test**

Create `tests/probe/stmt.probe`:

```
counter frame
counter n

probe s(v = peek8($1F00)) {
  inc frame
  n = n + v
  if (frame > 100)
    printf("over %u\n", frame)
  else {
    printf("under %u %02X\n", frame, v)
    n = 0
  }
  if (v == $FF)
    stop
}
```

Add to `tests/run-tests.sh`:

```sh
probe_dump_test stmt
```

- [ ] **Step 2: Run it to verify it fails**

Expected: FAIL — `FATAL: probe/stmt.probe:6: expected '}'`.

- [ ] **Step 3: Implement statements**

In `lib/a2host/probe_parse.c`, replace the `parse_block` stub:

```c
/* --- printf --------------------------------------------------------------- */

/// Count conversions, rejecting anything whose rendering is not a plain
/// integer. `%s` would need string values on the stack, `%n` writes through a
/// pointer, and floats bring locale into an equivalence oracle.
static unsigned count_conversions(parser_t *P, const char *fmt) {
  unsigned n = 0;
  for (const char *p = fmt; *p; ++p) {
    if (*p != '%')
      continue;
    ++p;
    if (*p == '%')
      continue;
    while (*p == '0' || *p == '-' || *p == '+' || *p == ' ')
      ++p;
    while (*p >= '0' && *p <= '9')
      ++p;
    switch (*p) {
    case 'd': case 'u': case 'x': case 'X':
      ++n;
      break;
    case 0:
      probe_error(&P->lx, "format string ends inside a conversion");
      break;
    default:
      probe_error(&P->lx, "unsupported conversion '%%%c'; use %%d %%u %%x %%X", *p);
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
    abort();
  strcpy(copy, fmt);
  sc->formats[sc->nformats] = copy;
  return sc->nformats++;
}

static void parse_printf(parser_t *P) {
  expect(P, TOK_LPAREN, "'(' after printf");
  if (P->lx.tok.kind != TOK_STRING)
    probe_error(&P->lx, "printf needs a literal format string");
  char fmt[PROBE_MAX_STRING];
  snprintf(fmt, sizeof(fmt), "%s", P->lx.tok.text);
  probe_lex_next(&P->lx);

  unsigned argc = 0;
  while (accept(P, TOK_COMMA)) {
    parse_expr(P, 1);
    ++argc;
  }
  expect(P, TOK_RPAREN, "')' after the printf arguments");

  unsigned want = count_conversions(P, fmt);
  if (want != argc)
    probe_error(&P->lx, "format needs %u argument(s), %u given", want, argc);

  emit(P, OP_PRINTF);
  emit(P, intern_format(P, fmt));
  emit(P, argc);
}

/* --- Statements ----------------------------------------------------------- */

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
      probe_error(&P->lx, "expected a counter name after inc");
    int idx = find_counter(P->sc, P->lx.tok.text);
    if (idx < 0)
      probe_error(&P->lx, "unknown counter '%s'", P->lx.tok.text);
    probe_lex_next(&P->lx);
    emit_op1(P, OP_LOAD_COUNTER, (uint32_t)idx);
    emit_op1(P, OP_PUSH_LIT, 1);
    emit(P, OP_ADD);
    emit_op1(P, OP_STORE_COUNTER, (uint32_t)idx);
    return;
  }

  // Assignment: <counter> = e, or <param> = e.
  if (P->lx.tok.kind == TOK_IDENT) {
    char name[PROBE_MAX_IDENT];
    snprintf(name, sizeof(name), "%s", P->lx.tok.text);
    int par = find_param(P->cur, name);
    int cnt = find_counter(P->sc, name);
    if (par >= 0 || cnt >= 0) {
      probe_lex_next(&P->lx);
      expect(P, TOK_ASSIGN, "'=' in an assignment");
      parse_expr(P, 1);
      emit_op1(P, par >= 0 ? OP_STORE_PARAM : OP_STORE_COUNTER,
               (uint32_t)(par >= 0 ? par : cnt));
      return;
    }
  }

  probe_error(&P->lx, "expected a statement");
}
```

- [ ] **Step 4: Generate and inspect the baseline**

```
ninja -C cmake-build-debug
cmake-build-debug/tools/a2run/a2run --probe=tests/probe/stmt.probe --probe-dump \
  > tests/probe/stmt.expected
```

**Read it.** Verify the `if`/`else` branch targets: the `JZ` operand must be
the offset of the `else` arm, and the `JMP` operand the offset just past it.

This is also the first test of the lexer's string escapes, which Task 2 could
not exercise because nothing parsed yet. `stmt.probe`'s format strings end in
`\n`, and an earlier draft of the lexer checked for a literal newline *after*
interpreting escapes, so a `\n` escape rejected itself.

Which means **the dump must re-escape format strings when printing them**, or
an interned `"over %u\n"` emits a real newline into the middle of the
`formats:` section and the baseline stops being line-oriented. Add to
`probe_dump` in `probe.c`:

```c
/// Print a format string with escapes restored, so one line of dump stays one
/// line however many newlines the string contains.
static void dump_escaped(FILE *f, const char *s) {
  for (; *s; ++s) {
    switch (*s) {
    case '\n': fputs("\\n", f); break;
    case '\t': fputs("\\t", f); break;
    case '"': fputs("\\\"", f); break;
    case '\\': fputs("\\\\", f); break;
    default: fputc(*s, f);
    }
  }
}
```

and use it for the `formats:` entries. Then confirm the baseline shows
`"over %u\n"` on one line, and that nothing was rejected.

- [ ] **Step 5: Run the tests**

```
cd tests && ./run-tests.sh ../cmake-build-debug
```

Expected: `Success!`

- [ ] **Step 6: Commit**

```bash
git add lib/a2host/probe_parse.c tests/probe/stmt.probe tests/probe/stmt.expected \
        tests/run-tests.sh
git commit -m "probe: statements, control flow and printf validation"
```

---

## Task 5: Install sites and the dispatch hash

**Files:**
- Modify: `lib/a2host/probe_parse.c`
- Modify: `lib/a2host/probe.c`
- Create: `tests/probe/install.probe`, `tests/probe/install.expected`, `tests/probe/sites.txt`
- Modify: `tests/run-tests.sh`

- [ ] **Step 1: Write the failing test**

Create `tests/probe/sites.txt`:

```
# Addresses one per line, hex, comments allowed. This is the form apple2tc
# will emit for block heads in phase 3.
0300
0310
0320
```

Create `tests/probe/install.probe`:

```
counter n

probe one()  { inc n }
probe two()  { inc n }

install one at $6100
install two at $6100, $6200-$6203
install one at @"sites.txt"
```

Add to `tests/run-tests.sh`:

```sh
probe_dump_test install
```

- [ ] **Step 2: Run it to verify it fails**

Expected: FAIL — `FATAL: probe/install.probe:6: install: not yet implemented`.

- [ ] **Step 3: Implement site collection**

Sites are collected into a flat list while parsing, then hashed once at the
end, because the table has to be sized to the final count.

Append to `lib/a2host/probe_internal.h`:

```c
enum { PROBE_MAX_SITE_DECLS = 8192 };

typedef struct {
  uint16_t addr;
  uint32_t probe_id;
} site_decl_t;

/// Filled by the parser, consumed by probe_build_sites().
extern site_decl_t g_site_decls[PROBE_MAX_SITE_DECLS];
extern unsigned g_nsite_decls;

void probe_build_sites(script_t *sc);
```

In `lib/a2host/probe_parse.c`, replace the `parse_install` stub:

```c
site_decl_t g_site_decls[PROBE_MAX_SITE_DECLS];
unsigned g_nsite_decls = 0;

static void add_site(parser_t *P, uint16_t addr, uint32_t probe_id) {
  if (g_nsite_decls == PROBE_MAX_SITE_DECLS)
    probe_error(&P->lx, "too many install sites");
  g_site_decls[g_nsite_decls].addr = addr;
  g_site_decls[g_nsite_decls].probe_id = probe_id;
  ++g_nsite_decls;
}

/// One hex address per line; '#' comments and blank lines ignored. This is the
/// form that makes cross-engine comparison work: both sides install at exactly
/// the same finite set, so the reports have the same length.
static void add_sites_from_file(parser_t *P, const char *name, uint32_t probe_id) {
  // Resolved relative to the script, so a script and its site list travel
  // together.
  char path[512];
  const char *slash = strrchr(P->lx.path, '/');
  if (slash)
    snprintf(path, sizeof(path), "%.*s%s", (int)(slash - P->lx.path + 1), P->lx.path, name);
  else
    snprintf(path, sizeof(path), "%s", name);

  FILE *f = fopen(path, "rt");
  if (!f)
    probe_error(&P->lx, "cannot open site list '%s'", path);

  char line[256];
  unsigned lineno = 0;
  while (fgets(line, sizeof(line), f)) {
    ++lineno;
    char *s = line;
    while (*s == ' ' || *s == '\t')
      ++s;
    if (*s == '#' || *s == '\n' || *s == '\r' || !*s)
      continue;
    char *end;
    unsigned long v = strtoul(s, &end, 16);
    if (end == s || v > 0xFFFF) {
      fclose(f);
      probe_error(&P->lx, "%s:%u: expected a 16-bit hex address", path, lineno);
    }
    add_site(P, (uint16_t)v, probe_id);
  }
  fclose(f);
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
      probe_error(&P->lx, "expected an address, a range or '@file'");
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
      add_site(P, (uint16_t)a, (uint32_t)probe_id);
  } while (accept(P, TOK_COMMA));
}
```

- [ ] **Step 4: Build the hash table**

Append to `lib/a2host/probe.c`:

```c
/// Mixing matters: 6502 block heads cluster within a page, so `addr & mask`
/// would pile them into adjacent slots.
static uint32_t hash_addr(uint16_t addr) {
  return (uint32_t)addr * 2654435761u >> 13;
}

/// Find the slot for \p addr: either its own, or the first free one. Every
/// occupied slot is a chain head, so linear probing never trips over anything
/// that is not a real key.
static uint32_t find_slot(const script_t *sc, uint16_t addr) {
  uint32_t slot = hash_addr(addr) & sc->slot_mask;
  while (sc->slots[slot].used && sc->slots[slot].addr != addr)
    slot = (slot + 1) & sc->slot_mask;
  return slot;
}

void probe_build_sites(script_t *sc) {
  if (!g_nsite_decls)
    return;

  // Load factor under a half.
  uint32_t cap = 16;
  while (cap < g_nsite_decls * 2)
    cap *= 2;

  sc->slots = (slot_t *)calloc(cap, sizeof(slot_t));
  sc->insts = (inst_t *)calloc(g_nsite_decls, sizeof(inst_t));
  if (!sc->slots || !sc->insts)
    abort();
  sc->slot_mask = cap - 1;
  sc->ninsts = g_nsite_decls;
  sc->nsites = 0;

  // Walked in reverse and prepended, so chains come out in script order --
  // which is what makes `install tick` before `install state` mean that state
  // observes the incremented counter.
  for (unsigned i = g_nsite_decls; i-- > 0;) {
    uint16_t addr = g_site_decls[i].addr;
    uint32_t slot = find_slot(sc, addr);

    sc->insts[i].probe_id = g_site_decls[i].probe_id;
    if (sc->slots[slot].used) {
      sc->insts[i].next = sc->slots[slot].first;
    } else {
      sc->slots[slot].used = 1;
      sc->slots[slot].addr = addr;
      sc->insts[i].next = PROBE_NO_SITE;
      ++sc->nsites;
    }
    sc->slots[slot].first = i;
  }
}
```

Call it from `probe_load_script`, after `probe_parse_script`:

```c
  probe_build_sites(&s_script);
```

Extend `probe_dump`'s `sites:` section to print the chains in address order so
the output is stable regardless of hash layout:

```c
  // Walked in address order rather than slot order, so the output does not
  // depend on the hash layout.
  fprintf(f, "sites: %u\n", sc->nsites);
  for (uint32_t a = 0; sc->slots && a <= 0xFFFF; ++a) {
    uint32_t slot = find_slot(sc, (uint16_t)a);
    if (!sc->slots[slot].used)
      continue;
    for (uint32_t i = sc->slots[slot].first; i != PROBE_NO_SITE; i = sc->insts[i].next)
      fprintf(f, "  $%04X %s\n", (unsigned)a, sc->probes[sc->insts[i].probe_id].name);
  }
```

`find_slot` is used by both the builder and the dump, so declare it in
`probe_internal.h` and define it once in `probe.c` above `probe_build_sites`.

- [ ] **Step 5: Generate and inspect the baseline**

```
ninja -C cmake-build-debug
cmake-build-debug/tools/a2run/a2run --probe=tests/probe/install.probe --probe-dump \
  > tests/probe/install.expected
```

**Read it.** `$6100` must list `one` before `two`, `$6200`-`$6203` must each
list `two`, and `$0300`/`$0310`/`$0320` must list `one`. Total 8 sites.

- [ ] **Step 6: Run the tests**

```
cd tests && ./run-tests.sh ../cmake-build-debug
```

Expected: `Success!`

- [ ] **Step 7: Commit**

```bash
git add lib/a2host/probe_parse.c lib/a2host/probe.c lib/a2host/probe_internal.h \
        tests/probe/install.probe tests/probe/install.expected tests/probe/sites.txt \
        tests/run-tests.sh
git commit -m "probe: install sites and the dispatch hash"
```

---

## Task 6: Rejection tests

Every diagnostic the compiler can produce, asserted to actually fire. A check
nobody has watched fail is not a check — the same reasoning behind the existing
`expect_reject` block at `tests/run-tests.sh:69-88`.

**Files:**
- Modify: `tests/run-tests.sh`

- [ ] **Step 1: Write the tests**

Add to the probe section of `tests/run-tests.sh`:

Task 1 already provides `expect_probe_reject <desc> <expected-substring>
<a2run args...>`. Every case names the diagnostic it expects rather than
grepping for `FATAL` alone — during Task 1's review, two separate tests passed
because a *different* check fired first and satisfied a loose grep. Add a thin
wrapper for the common case of rejecting a script's *contents*:

```sh
# Compile a script given inline and assert it is rejected with a specific
# diagnostic.
expect_bad_script() {
  # $1: description, $2: expected substring, $3: script contents
  printf '%s\n' "$3" > probe-tmp/bad.probe
  expect_probe_reject "$1" "$2" --probe=probe-tmp/bad.probe --probe-dump
}

expect_bad_script "an unknown top-level keyword" "expected 'counter'" \
  'frobnicate x'
expect_bad_script "an unknown name in an expression" "unknown name 'nosuch'" \
  'probe p(v = nosuch) { }'
expect_bad_script "a duplicate counter" "already declared" \
  'counter c
counter c'
expect_bad_script "a duplicate probe" "already declared" \
  'probe p() { }
probe p() { }'
expect_bad_script "a duplicate parameter" "duplicate parameter" \
  'probe p(v = 1, v = 2) { }'
# Self-reference is prevented by nparams not being incremented until after the
# initialiser parses -- a fragile-looking invariant, so assert it directly.
expect_bad_script "a self-referential parameter" "unknown name 'v'" \
  'probe p(v = v) { }'
expect_bad_script "a counter named after a builtin" "reserved" \
  'counter hash'
expect_bad_script "a parameter named after a keyword" "reserved" \
  'probe p(printf = 1) { }'
expect_bad_script "a probe named after a keyword" "reserved" \
  'probe install() { }'
# Registers are shadowable, but not retroactively: the same spelling must not
# mean LOAD_REG above a counter declaration and LOAD_COUNTER below it.
expect_bad_script "a counter declared after its name resolved to a register" \
  "as a register" \
  'probe before(v = x) { }
counter x'
expect_bad_script "a non-literal counter init" "must be a literal" \
  'counter c = peek8($10)'
expect_bad_script "install of an unknown probe" "unknown probe" \
  'install nosuch at $300'
expect_bad_script "an unopenable site list" "cannot open site list" \
  'probe p() { }
install p at @"no-such-file.txt"'
expect_bad_script "a backwards range" "ends before it starts" \
  'probe p() { }
install p at $400-$300'
expect_bad_script "an out-of-range address" "out of range" \
  'probe p() { }
install p at $10000'
expect_bad_script "too few printf arguments" "argument" \
  'probe p() { printf("%u %u\n", 1) }'
expect_bad_script "too many printf arguments" "argument" \
  'probe p() { printf("%u\n", 1, 2) }'
expect_bad_script "an unsupported conversion" "unsupported conversion" \
  'probe p() { printf("%s\n", 1) }'
expect_bad_script "a truncated conversion" "ends inside a conversion" \
  'probe p() { printf("%") }'
expect_bad_script "an unterminated block" "unterminated block" \
  'probe p() { inc'
expect_bad_script "a newline inside a string" "newline in string literal" \
  'probe p() { printf("x) }'
expect_bad_script "assignment to an unknown name" "expected a statement" \
  'probe p() { nosuch = 1 }'
```

Two more about option combinations rather than script contents:

```sh
expect_probe_reject "two probe scripts" "only one probe script" \
  --probe=probe/empty.probe --probe=probe/empty.probe --probe-dump
expect_probe_reject "an unwritable report file" "cannot open probe output" \
  --probe=probe/empty.probe --probe-out=probe-tmp/nodir/out.txt
```

If any expected substring does not match what the compiler actually emits,
fix whichever is wrong — but do not weaken the assertion back to a bare
`FATAL`.

- [ ] **Step 2: Run them**

```
cd tests && ./run-tests.sh ../cmake-build-debug
```

Expected: `Success!`. Any case that fails names the diagnostic that is missing
or wrong — fix the compiler, not the test.

- [ ] **Step 3: Commit**

```bash
git add tests/run-tests.sh
git commit -m "probe: assert every compiler rejection"
```

---

## Task 7: Document the language

**Files:**
- Create: `docs/probes.md`
- Modify: `docs/plans/2026-08-11-probes-design.md`

- [ ] **Step 1: Write the reference**

Create `docs/probes.md` covering, with a worked example for each: the three
declarations, every statement, every expression atom, the operator precedence
table, the opcode list with its operand counts, the `--probe`, `--probe-out`
and `--probe-dump` options, and the dump format. The design document explains
*why*; this explains *what*, and it is the file someone writing a script reads.

State plainly the constraint that will bite: **a probe installed at an address
that is not a basic-block head will fire under `a2emu` and `a2run` but not in a
generated program**, so cross-engine comparison must install from `@"file"`.

- [ ] **Step 2: Cross-reference from the design doc**

Add to the top of `docs/plans/2026-08-11-probes-design.md`, after the date
line:

```markdown
The language reference is `docs/probes.md`. This document is the rationale.
```

- [ ] **Step 3: Commit**

```bash
git add docs/probes.md docs/plans/2026-08-11-probes-design.md
git commit -m "docs: probe language reference"
```

---

## Self-review against the spec

Checked while writing; recorded so part 2 inherits the gaps rather than
rediscovering them.

**Covered by this plan:** the script language in full (declarations,
statements, expression atoms, C operator precedence), the bytecode format and
opcode set, initializers as ordinary expressions, the two entry offsets
(`init_offset`/`body_offset` are computed and dumped, though nothing enters at
them yet), install by address, range and `@file`, the site hash with mixing and
script-order chaining, `printf` conversion validation at compile time, and loud
parse errors.

**One public-API change part 2 must make, found in review of Task 1.** The
design says `CYCLES` "tests a core-owned pointer inline, null when no script is
loaded" (`system-inc.h:29` is a macro expanded into the hot path of every
generated program, where the precedent is an inline test of the global
`g_debug`). `probe_installed()` is a function call and cannot serve that
purpose. Part 2 needs an `extern` flag or table pointer in `probe.h`, with
`probe_installed()` either dropped or kept as the init-time query it actually
is. Not added now, because nothing would set it.

**Deferred to part 2, by design:** the VM, `probe_dispatch`, `probe_installed`,
the `printf` renderer, `ram_peek`-based `PEEK8`/`PEEK16`, `OP_KEY` (which needs
the key source), `OP_STOP`'s host flag, never-fired diagnostics, the `CYCLES`
wiring in `system-inc.h:29` and `system2-inc.h:242`, the `debugCB` wiring at
`lib/engine6502/engine6502.cpp:193`, enabling `Emu6502::DebugASM` at
`engine6502.cpp:263`, the a2emu/a2run equivalence test, and the ROM-boot
acceptance test against `romc1-run`.

**Two decisions this plan makes that the spec left open:**

1. **`&&` and `||` short-circuit**, via `JZ`/`JNZ` branches. An earlier draft
   of this plan claimed they could safely reuse `OP_AND`/`OP_OR` because
   comparisons yield 0/1; that was false for every other kind of operand, and
   it shipped before review caught it. See Task 3 for the correction.
2. **`@file` names are quoted strings**, not bare identifiers, so a path with a
   dot needs no lexer state.

**One hazard carried forward:** `hash(lo, hi)` is compiled here but its
semantics — which hash, over what — are part 2's to define, and both engines
must produce identical values from identical memory. Use a plain FNV-1a over
the byte range read through `ram_peek`, and write that down where the opcode is
implemented.
