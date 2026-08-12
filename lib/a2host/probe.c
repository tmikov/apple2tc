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

/// Frees the interned format strings, the install-site hash table (`slots`
/// and `insts`, allocated by probe_build_sites()) and, if parsing failed
/// before probe_build_sites() ran, the still-pending site declarations -- on
/// every exit path, registered with atexit() below, not called from
/// probe_close_output(). Two reasons that has to be atexit() rather than a
/// hook off an existing teardown call: probe_close_output() also fires the
/// moment probe_set_output_path() opens a *second* output file, which can
/// happen before the probe has actually run, so freeing the script's
/// resources there would pull them out from under a live run; and
/// `--probe-dump` -- which is how every test in this module exercises the
/// compiler -- calls exit(0) directly and never reaches a2host_shutdown()
/// (hence never probe_close_output()) at all. atexit() fires on every exit()
/// call, that one included, so it is the only hook that actually covers the
/// tests LeakSanitizer would otherwise flag.
static void free_script_resources(void) {
  for (unsigned i = 0; i != s_script.nformats; ++i)
    free(s_script.formats[i]);
  free(s_script.slots);
  free(s_script.insts);
  // Ordinarily already NULL: probe_build_sites() frees pending_sites itself
  // once it has consumed it. It is still non-NULL here only when parsing
  // failed before probe_build_sites() ever ran (e.g. a syntax error after a
  // few `install` lines already parsed) -- free(NULL) is a no-op, so this
  // covers that path without needing to know which case applies.
  free(s_script.pending_sites);
}

void probe_load_script(const char *path) {
  if (s_loaded)
    probe_fatal("only one probe script may be loaded");
  char *src = read_file(path);
  memset(&s_script, 0, sizeof(s_script));
  // Registered before parsing, not after: a script that fails partway
  // through -- e.g. after interning a printf format string, then hitting a
  // syntax error -- exits via probe_error before probe_parse_script ever
  // returns. Registering here means free_script_resources still runs on that
  // path and reclaims what had already been allocated; registering after
  // parse_script (as an earlier version of this did) would have skipped it.
  // s_loaded above guarantees probe_load_script itself never runs twice, so
  // this still registers exactly once.
  atexit(free_script_resources);
  probe_parse_script(&s_script, src, path);
  free(src);
  probe_build_sites(&s_script);
  s_loaded = true;
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

/* --- Install sites -----------------------------------------------------
 *
 * An open-addressed, power-of-two-sized hash table over 16-bit addresses.
 * Sized once (by probe_build_sites, from the final pending-site count) to
 * keep the load factor at most a half, which is what bounds
 * probe_find_slot's scan: with at most half the slots ever occupied, at
 * least one empty slot always exists on the probe sequence for both a
 * present and an absent address, so the `while (slots[slot].used ...)` loop
 * below is guaranteed to terminate -- there is no separate "table full" case
 * to handle. Placed ahead of probe_dump, its first caller, so
 * probe_find_slot's precondition (declared in probe_internal.h) and the
 * table's invariants are in scope before anything relies on them.
 */

/// Mixing matters: 6502 block heads cluster within a page, so `addr & mask`
/// would pile them into adjacent slots.
static uint32_t hash_addr(uint16_t addr) {
  return (uint32_t)addr * 2654435761u >> 13;
}

// hash_addr discards the low 13 bits of its 32-bit product, leaving 19
// significant bits (bits 13..31) for `& slot_mask` to draw from below.
// probe_build_sites' `cap` never needs more mask bits than that: its ceiling
// is the smallest power of two >= 2*PROBE_MAX_SITE_DECLS, so bounding
// PROBE_MAX_SITE_DECLS keeps every mask bit backed by a real bit of
// hash_addr's output. Past this point some mask bits would always see zero
// from hash_addr, silently shrinking the table's reachable half -- a trap
// that is invisible at the current cap and stays that way only because the
// assert below fails the build first if PROBE_MAX_SITE_DECLS ever grows
// past it.
_Static_assert(
    PROBE_MAX_SITE_DECLS <= (1u << 18),
    "hash_addr's fixed >>13 shift cannot address a slot_mask this wide -- widen the shift too");

uint32_t probe_find_slot(const script_t *sc, uint16_t addr) {
  uint32_t slot = hash_addr(addr) & sc->slot_mask;
  while (sc->slots[slot].used && sc->slots[slot].addr != addr)
    slot = (slot + 1) & sc->slot_mask;
  return slot;
}

void probe_build_sites(script_t *sc) {
  if (!sc->npending_sites)
    return;

  // Load factor at most a half.
  uint32_t cap = 16;
  while (cap < sc->npending_sites * 2)
    cap *= 2;

  sc->slots = (slot_t *)calloc(cap, sizeof(slot_t));
  sc->insts = (inst_t *)calloc(sc->npending_sites, sizeof(inst_t));
  if (!sc->slots || !sc->insts)
    probe_fatal("out of memory building the probe site table");
  sc->slot_mask = cap - 1;
  sc->ninsts = sc->npending_sites;
  sc->nsites = 0;

  // Walked in reverse and prepended, so chains come out in script order --
  // which is what makes `install tick` before `install state` mean that
  // state observes the incremented counter.
  for (uint32_t i = sc->npending_sites; i-- > 0;) {
    uint16_t addr = sc->pending_sites[i].addr;
    uint32_t slot = probe_find_slot(sc, addr);

    sc->insts[i].probe_id = sc->pending_sites[i].probe_id;
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

  // pending_sites is parse-time scratch (see its comment in
  // probe_internal.h): dead the moment the table above exists, so it is
  // freed here rather than carried to atexit teardown.
  free(sc->pending_sites);
  sc->pending_sites = NULL;
  sc->npending_sites = 0;
  sc->pending_sites_cap = 0;
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
  // Walked in address order rather than slot order, so the output does not
  // depend on hash layout -- a change to hash_addr or the load factor must
  // not perturb the expected dump. `sc->slots` is hoisted out of the loop
  // condition (rather than tested 65,536 times): it is loop-invariant, only
  // ever NULL when there is nothing to walk at all.
  if (sc->slots) {
    for (uint32_t a = 0; a <= 0xFFFF; ++a) {
      uint32_t slot = probe_find_slot(sc, (uint16_t)a);
      if (!sc->slots[slot].used)
        continue;
      for (uint32_t i = sc->slots[slot].first; i != PROBE_NO_SITE; i = sc->insts[i].next)
        fprintf(f, "  $%04X %s\n", (unsigned)a, sc->probes[sc->insts[i].probe_id].name);
    }
  }
}

bool probe_installed(void) {
  return s_script.nsites != 0;
}

void probe_dispatch(uint16_t pc) {
  (void)pc;
}

void probe_report_unfired(void) {}
