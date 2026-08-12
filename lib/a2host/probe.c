/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/probe.h"

#include "probe_internal.h"

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// Where `probe_dispatch`'s eventual `printf` output goes. NULL means
/// "unset", not "closed" -- `probe_out()` is what maps that to stdout.
static FILE *s_out = NULL;

static FILE *probe_out(void) {
  return s_out ? s_out : stdout;
}

_Noreturn void probe_fatal(const char *fmt, ...) {
  fputs("FATAL: ", stderr);
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  fputc('\n', stderr);
  exit(2);
}

/// Every fatal diagnostic that involves opening a file goes through here, so
/// each one remembers *why* the open failed -- "no such file" and "permission
/// denied" are different bugs.
_Noreturn static void probe_fatal_open(const char *what, const char *path) {
  probe_fatal("%s '%s': %s", what, path, strerror(errno));
}

static script_t s_script;
static bool s_loaded = false;

static char *read_file(const char *path) {
  FILE *f = fopen(path, "rb");
  if (!f)
    probe_fatal_open("cannot open probe script", path);

  // Read to EOF in a loop rather than sizing one fread() from ftell(): ftell
  // lies for plenty of files that fopen("rb") happily opens -- a directory
  // (glibc hands back LONG_MAX), and specials like /proc/self/limits (which
  // report a size of 0 and then deliver real content to an actual read).
  // Trusting either would compile the script as empty and exit 0, which is
  // the "loads quietly, reports agreement" failure probes exist to catch,
  // just relocated into their own loader.
  size_t cap = 4096, len = 0;
  char *buf = (char *)malloc(cap);
  if (!buf)
    probe_fatal("cannot allocate %zu bytes for probe script '%s'", cap, path);
  for (;;) {
    if (len == cap) {
      cap *= 2;
      char *grown = (char *)realloc(buf, cap);
      if (!grown)
        probe_fatal("cannot allocate %zu bytes for probe script '%s'", cap, path);
      buf = grown;
    }
    size_t got = fread(buf + len, 1, cap - len, f);
    len += got;
    if (got == 0)
      break;
  }
  // A short read that isn't EOF (i.e. an I/O error) would otherwise parse
  // silently as a truncated-but-well-formed prefix of the script. Not
  // probe_fatal_open: ferror() doesn't guarantee errno was left meaningful,
  // so appending strerror(errno) here can print "...: Success".
  if (ferror(f))
    probe_fatal("cannot read probe script '%s'", path);
  buf[len] = 0; // always in bounds: the loop above never breaks with len == cap
  fclose(f);
  return buf;
}

/// Frees the interned format strings on every exit path -- registered with
/// atexit() below, not called from probe_close_output(). Two reasons that
/// has to be atexit() rather than a hook off an existing teardown call:
/// probe_close_output() also fires the moment probe_set_output_path() opens
/// a *second* output file, which can happen before the probe has actually
/// run, so freeing the script's formats there would pull them out from under
/// a live run; and `--probe-dump` -- which is how every test in this module
/// exercises the compiler -- calls exit(0) directly and never reaches
/// a2host_shutdown() (hence never probe_close_output()) at all. atexit()
/// fires on every exit() call, that one included, so it is the only hook
/// that actually covers the tests LeakSanitizer would otherwise flag.
static void free_formats(void) {
  for (unsigned i = 0; i != s_script.nformats; ++i)
    free(s_script.formats[i]);
}

void probe_load_script(const char *path) {
  if (s_loaded)
    probe_fatal("only one probe script may be loaded");
  char *src = read_file(path);
  memset(&s_script, 0, sizeof(s_script));
  probe_parse_script(&s_script, src, path);
  free(src);
  s_loaded = true;
  atexit(free_formats); // see free_formats' comment; registered exactly once
                        // since s_loaded above allows only one script
}

void probe_set_output_path(const char *path) {
  FILE *f = fopen(path, "wt");
  if (!f)
    probe_fatal_open("cannot open probe output", path);
  probe_close_output();
  s_out = f;
}

void probe_close_output(void) {
  if (s_out) {
    fclose(s_out);
    s_out = NULL;
  }
}

// A switch with no `default`, not a table: a positional array tolerates a
// transposed pair (LE/GE is the classic) as long as the count stays right,
// and a designated-initializer array loses even that -- an opcode added
// without a table entry just reads NULL at runtime. A switch that omits an
// enumerator and has no default makes GCC's -Wswitch (on under -Wall) fail
// the build instead, so the guarantee is back at compile time, which is
// where the last two versions of this both should have lived.
static const char *opname(opcode_t op) {
  switch (op) {
  case OP_END:
    return "END";
  case OP_PUSH_LIT:
    return "PUSH_LIT";
  case OP_LOAD_PARAM:
    return "LOAD_PARAM";
  case OP_LOAD_COUNTER:
    return "LOAD_COUNTER";
  case OP_STORE_PARAM:
    return "STORE_PARAM";
  case OP_STORE_COUNTER:
    return "STORE_COUNTER";
  case OP_LOAD_REG:
    return "LOAD_REG";
  case OP_PEEK8:
    return "PEEK8";
  case OP_PEEK16:
    return "PEEK16";
  case OP_HASH:
    return "HASH";
  case OP_ADD:
    return "ADD";
  case OP_SUB:
    return "SUB";
  case OP_MUL:
    return "MUL";
  case OP_DIV:
    return "DIV";
  case OP_MOD:
    return "MOD";
  case OP_AND:
    return "AND";
  case OP_OR:
    return "OR";
  case OP_XOR:
    return "XOR";
  case OP_SHL:
    return "SHL";
  case OP_SHR:
    return "SHR";
  case OP_EQ:
    return "EQ";
  case OP_NE:
    return "NE";
  case OP_LT:
    return "LT";
  case OP_LE:
    return "LE";
  case OP_GT:
    return "GT";
  case OP_GE:
    return "GE";
  case OP_NOT:
    return "NOT";
  case OP_BITNOT:
    return "BITNOT";
  case OP_NEG:
    return "NEG";
  case OP_JMP:
    return "JMP";
  case OP_JZ:
    return "JZ";
  case OP_JNZ:
    return "JNZ";
  case OP_PRINTF:
    return "PRINTF";
  case OP_KEY:
    return "KEY";
  case OP_STOP:
    return "STOP";
  }
  // Reachable only for a code[] cell that isn't one of the enumerators above
  // -- corrupt bytecode, not a missing table entry (-Wswitch already catches
  // that at compile time). dump_insn treats NULL as "corrupt".
  return NULL;
}

/// Opcodes taking one operand cell. PRINTF takes two; everything else none.
/// Exhaustive and default-less like opname() above, so adding an opcode
/// forces a decision here instead of silently inheriting whatever a
/// `default:` used to return.
static bool has_operand(opcode_t op) {
  switch (op) {
  case OP_PUSH_LIT:
  case OP_LOAD_PARAM:
  case OP_LOAD_COUNTER:
  case OP_STORE_PARAM:
  case OP_STORE_COUNTER:
  case OP_LOAD_REG:
  case OP_JMP:
  case OP_JZ:
  case OP_JNZ:
    return true;
  case OP_END:
  case OP_PEEK8:
  case OP_PEEK16:
  case OP_HASH:
  case OP_ADD:
  case OP_SUB:
  case OP_MUL:
  case OP_DIV:
  case OP_MOD:
  case OP_AND:
  case OP_OR:
  case OP_XOR:
  case OP_SHL:
  case OP_SHR:
  case OP_EQ:
  case OP_NE:
  case OP_LT:
  case OP_LE:
  case OP_GT:
  case OP_GE:
  case OP_NOT:
  case OP_BITNOT:
  case OP_NEG:
  case OP_PRINTF:
  case OP_KEY:
  case OP_STOP:
    return false;
  }
  return false; // unreachable if -Wswitch is honored; see opname() above
}

/// Print a format string with escapes restored, so one line of dump stays one
/// line however many newlines the string contains, and every other
/// non-printable byte (raw BEL, ESC, VT, FF, ...) renders visibly instead of
/// vanishing into the terminal or a `diff` of the report -- the same reason
/// the lexer rejects a raw CR in a string literal, generalized to the rest of
/// the non-printable range instead of just the one byte that bit first.
static void dump_escaped(FILE *f, const char *s) {
  for (const unsigned char *u = (const unsigned char *)s; *u; ++u) {
    switch (*u) {
    case '\n':
      fputs("\\n", f);
      break;
    case '\t':
      fputs("\\t", f);
      break;
    case '"':
      fputs("\\\"", f);
      break;
    case '\\':
      fputs("\\\\", f);
      break;
    default:
      if (isprint(*u))
        fputc(*u, f);
      else
        fprintf(f, "\\x%02X", *u);
    }
  }
}

static uint32_t dump_insn(FILE *f, const script_t *sc, uint32_t ip) {
  opcode_t op = (opcode_t)sc->code[ip];
  const char *name = opname(op);
  if (!name)
    probe_fatal("corrupt probe bytecode: opcode %u at offset %u", sc->code[ip], ip);
  fprintf(f, "    %5u: %s", ip, name);
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
  for (unsigned i = 0; i != sc->nformats; ++i) {
    fprintf(f, "  %u \"", i);
    dump_escaped(f, sc->formats[i]);
    fputs("\"\n", f);
  }

  fprintf(f, "probes: %u\n", sc->nprobes);
  for (unsigned i = 0; i != sc->nprobes; ++i) {
    const probe_t *pr = &sc->probes[i];
    fprintf(f, "  %u %s(", i, pr->name);
    for (unsigned p = 0; p != pr->nparams; ++p)
      fprintf(f, "%s%s", p ? ", " : "", pr->params[p]);
    fprintf(f, ") init=%u body=%u\n", pr->init_offset, pr->body_offset);
    for (uint32_t ip = pr->init_offset; ip < pr->end_offset;)
      ip = dump_insn(f, sc, ip);
  }

  fprintf(f, "sites: %u\n", sc->nsites);
}

bool probe_installed(void) {
  return false;
}

void probe_dispatch(uint16_t pc) {
  (void)pc;
}

void probe_report_unfired(void) {}
