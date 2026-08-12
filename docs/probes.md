# Probe language reference

A probe script is a small program, compiled by `a2host` (so every front end —
`a2run`, `a2emu`, and any generated decompiled program, once phase 3 exists —
gets the same feature) into bytecode that runs at chosen addresses in the
emulated 6502 program. The rationale — why probes exist, why they are programs
rather than records, why initializers work the way they do — is in
`docs/plans/2026-08-11-probes-design.md`. This document is the *what*: every
construct in the language, checked against the compiler in `lib/a2host/`
(`probe_lex.c`, `probe_parse.c`, `probe.c`, `probe_internal.h`).

**Status.** Only the compiler exists. `--probe=path --probe-dump` compiles a
script and prints its bytecode; that is the entire observable behavior today.
`probe_dispatch()` is a stub, nothing calls it, and `key`/`stop`/`printf`
execute nothing yet — see [Execution status](#execution-status) at the end.
Everything else in this document — parsing, name resolution, diagnostics, the
dump format — is real and tested (`tests/run-tests.sh`, `tests/probe/`).

## The constraint that will bite

**`CYCLES` is emitted once per basic block, not once per instruction.** A
probe installed at an address that is not a block head fires under `a2emu`
and `a2run`, which single-step every instruction, but the same address
[does not exist as a callable site](plans/2026-08-11-probes-design.md#install-sites-and-why-file-is-the-one-that-matters)
in a generated program — there is no C statement there to attach it to.

That failure is silent. The probe still compiles, still installs, still
fires on one side, and the report from that side still looks complete — it
just isn't comparable to the other side's report, and nothing says so. A
report that reads as agreement when the two sides installed at different
sets is the exact failure this whole facility exists to prevent.

**So: for any cross-engine comparison, install from `@"file"`**, using the
block-head list apple2tc will emit (phase 3, not built yet). That makes both
sides install at an identical finite set, both reports have the same length,
and `diff` compares them directly — no subsequence matcher, nothing to
degrade gracefully on a mismatch. A bare address or a `$LO-$HI` range is fine
for probing a routine whose shape you already know, but carries the same
asymmetry and should not be used to compare engines.

## Quick example

```
probe trace(site = pc) {
  printf("%04X\n", site)
}

install trace at @"blocks.txt"
```

`trace` takes a snapshot of the program counter at entry and prints it. Every
address in `blocks.txt` gets one instance of `trace` installed. Compile it
with:

```
a2run --probe=trace.probe --probe-dump
```

## Declarations

A script is a sequence of top-level declarations, in any mix, read top to
bottom. Order matters — see [Name resolution](#name-resolution).

| form | notes |
|---|---|
| `counter <name> [= <literal>]` | a persistent 32-bit value; init defaults to 0 |
| `probe <name>([<param> = <expr>, ...]) { <stmt>... }` | params are the signature; body is arbitrary statements |
| `install <probe> at <site>[, <site>]...` | binds a declared probe to one or more addresses |

### `counter`

```
counter frame
counter limit = 500
counter neg = -1
counter hex = $1F4
```

A counter is script-global state, persisting across every probe invocation
and every install site. Its initializer, if given, must be a plain literal —
decimal or `$hex` — optionally negated; **not a general expression**. `counter
n = frame` is rejected with "a counter initialiser must be a literal," even
though `frame` and any other counter or parameter would be a well-defined
value at load time. `counter n = 1 + 1`, despite being a compile-time
constant, is *not* rejected with that message: the parser accepts the `= 1`
and returns, leaving the `+` to be tripped over at top level, so the actual
diagnostic is "expected 'counter', 'probe' or 'install'" — a wart, tracked in
`docs/plans/2026-08-11-probes-plan-1-compiler.md`'s self-review as left for
part 2 to fix. Only the *parameter* initializers on `probe` are full
expressions.

Dumped form (`--probe-dump`) shows the resolved value, not the source text —
`neg = -1` above dumps as `neg = 4294967295` (unsigned 32-bit wraparound).

### `probe`

```
probe pos(x = peek8($1F00), y = peek8($1F01), site = pc, sum = x + y) { }
```

The parameter list is the probe's signature: name, arity, order. Each
parameter has a mandatory initializer expression, evaluated at entry, in
declaration order — a later parameter's initializer may reference an earlier
one (`sum = x + y` above), but a parameter cannot reference itself
(`probe p(v = v) { }` is "unknown name 'v'": at the point `v`'s own
initializer is being parsed, `v` is not yet in scope).

The body is a block of statements ([below](#statements)), executed after all
initializers have run. Inside the body, every parameter of the probe is in
scope regardless of source order.

A parameter may shadow a register name — `x`, `y`, `sp`, `sr`, `pc`, `a` are
ordinary identifiers here, not reserved, so `pos` above is unremarkable: `x`
and `y` are the obvious names for a screen coordinate, and forbidding them
would be a bad language for that. `site = pc` in the same probe shows the
unshadowed register still resolving normally.

### `install`

A site is one of:

| form | meaning |
|---|---|
| `$ADDR` or decimal | one address, `$0000`-`$FFFF` |
| `$LO-$HI` | every address in the inclusive range, `LO <= HI` |
| `@"file"` | every address named in a site-list file, resolved relative to the script's own directory |

Several sites, mixing forms, separate with commas:

```
probe trace(site = pc) { }

install trace at $6100, $6200-$6203, @"blocks.txt"
```

with `blocks.txt` (next to the script):

```
7000
7010
```

installs `trace` at seven addresses: `$6100`, `$6200`-`$6203`, `$7000`, `$7010`.

Several `install`
statements — even for different probes — may target the same address;
probes installed at one address run in the script's install order (this is
how the design doc's frame/state example makes `state` observe the
post-increment counter: `tick` is installed first). Installing the same
`(probe, address)` pair twice is an error ("already installed at $ADDR").

There is no wildcard form ("every address") — deliberately, since "every
address" and "every generated-program block head" are different sets, and a
wildcard would make the report length mismatch described
[above](#the-constraint-that-will-bite) unconditional.

**Site-list file format** (the `@"file"` form): one address per line, plain
hex digits, no `$` or `0x` prefix (`0300`, not `$0300` or `0x300`); `#`
starts a comment to end of line; blank lines are ignored; nothing but
trailing whitespace or a comment may follow the address on its line. A file
naming no addresses at all — empty, comments-only — is rejected, not
silently treated as "install nothing." `tests/probe/sites.txt`:

```
# Addresses one per line, hex, comments allowed. This is the form apple2tc
# will emit for block heads in phase 3.
0300
0310
0320
```

A script may declare up to 8192 site bindings in total across every `install`
statement combined (`PROBE_MAX_SITE_DECLS`) — a large range or a large site
list counts against the same budget as everything else in the script.

## Statements

Valid inside a probe body only.

| statement | form | notes |
|---|---|---|
| block | `{ <stmt>... }` | braces group statements; also the body of `if`/`else` |
| conditional | `if (<expr>) <stmt> [else <stmt>]` | any nonzero value is true |
| assignment | `<counter> = <expr>` or `<param> = <expr>` | not to registers — see below |
| increment | `inc <counter>` | sugar for `<counter> = <counter> + 1`; counters only |
| formatted output | `printf("<fmt>", <expr>, ...)` | see [printf](#printf) |
| key delivery | `key <expr>` | deliver every pending key stamped ≤ `<expr>`; not executable yet |
| stop | `stop` | end the run cleanly; not executable yet |

```
counter frame
counter n

probe s(v = peek8($1F00)) {
  inc frame
  if (v == $FF)
    stop
  if (frame > 100) {
    n = n + 1
    printf("over %u (%d hits)\n", frame, n)
  } else
    printf("under %u %02X\n", frame, v)
  key frame
}
install s at $6100
```

**Assigning to a register is a compile error**: `probe p() { a = 1 }` fails
with "cannot assign to register 'a': probes must not alter machine state." A
probe that could perturb the machine it is observing would no longer be a
neutral observer — the same reasoning that routes `peek8`/`peek16` through
`ram_peek` rather than a live `peek` (see the design doc). This applies only
to unshadowed registers; if a counter or parameter is named `a`, assigning to
*that* `a` is ordinary counter/parameter assignment, not a register write.

`inc` only accepts a counter, not a parameter or a bare expression:
`inc a` (an unshadowed register) is "'inc' needs a counter, but 'a' is not
one"; `inc nosuch` is "unknown name 'nosuch'".

## Expressions

### Atoms

| atom | meaning |
|---|---|
| `123`, `$7B` | a literal, decimal or hex; up to 32 bits (`$100000000` is out of range) |
| `<param>` | a probe parameter, resolved by name |
| `<counter>` | a script-level counter, resolved by name |
| `a` `x` `y` `sp` `sr` `pc` | a CPU register, unless shadowed (see [name resolution](#name-resolution)) |
| `peek8(<expr>)` | one byte read from RAM at the given address, through `ram_peek` |
| `peek16(<expr>)` | two bytes, little-endian, through `ram_peek` |
| `hash(<lo>, <hi>)` | a hash of RAM `[lo, hi]`; the specific algorithm is part of the not-yet-built VM |
| `(<expr>)` | grouping |
| `-<expr>` `~<expr>` `!<expr>` | unary negate, bitwise NOT, logical NOT; bind tighter than any binary operator |

`peek8`, `peek16`, `hash`, and every statement keyword are matched before
identifier lookup, so none of them can be shadowed — declaring a counter,
parameter, or probe named `peek8` is rejected as a reserved name (see below).

### Operator precedence

C precedence and associativity (all binary operators are left-associative),
from loosest to tightest, matching `binop_prec` in `probe_parse.c`:

| level | operators |
|---|---|
| 1 (loosest) | `\|\|` |
| 2 | `&&` |
| 3 | `\|` |
| 4 | `^` |
| 5 | `&` |
| 6 | `==` `!=` |
| 7 | `<` `<=` `>` `>=` |
| 8 | `<<` `>>` |
| 9 | `+` `-` |
| 10 | `*` `/` `%` |
| 11 (tightest) | unary `!` `~` `-` |

There is no bitwise-vs-logical ambiguity to worry about the way C's `&`/`&&`
sometimes invites: every operator except the six comparisons and `!` can
produce any 32-bit value, not just 0/1, so `&`/`|` and `&&`/`||` are not
interchangeable even when both operands happen to look boolean.

### Short-circuit evaluation

`&&` and `||` short-circuit exactly as in C: the right operand is not
evaluated if the left already decides the result, so guarding a `peek` or a
divide behind a test works —

```
frame != 0 && 100 / frame > 5
```

— never evaluates `100 / frame` when `frame` is 0. This is compiled to
branches (`JZ`/`JNZ`), not to `AND`/`OR` opcodes, specifically because a
single opcode consuming two already-computed values could never skip
evaluating one of them.

`tests/probe/expr.probe` (`sc` probe) exercises chained, mixed, nested, and
embedded short-circuit expressions — e.g. `frame > 0 && frame < 100 && limit
> 0`, and a short-circuit expression embedded inside a `peek8` address — and
`tests/probe/expr.expected` is the compiled-branch assertion for all of them.

### Worked example: precedence and every atom

```
counter frame
counter limit = $1F4

probe e(
  prec  = 1 + 2 * 3,
  paren = (1 + 2) * 3,
  cmp   = (frame < limit) + (frame >= limit),
  bool  = frame > 1 && frame < 9,
  bits  = ((frame & $F0) | $0F) ^ $FF,
  un    = -1 + ~0 + !0,
  mem   = peek8($1F00) + peek16($06),
  reg   = a + x + y + sp + sr + pc,
  hashv = hash($2000, $3FFF)
) { }
```

## Name resolution

An identifier resolves in this order: **parameter → counter → register**. A
counter or parameter declared with a register's name shadows that register,
for both reads and assignments — `resolve_name()` in `probe_parse.c` is the
single place this order is applied, used by expression parsing, `inc`, and
assignment alike, so a name can never mean a register on one path and a
counter on another.

Resolution happens as the script is read, not after the whole file is
parsed, so **a counter must be declared before any probe that would
otherwise resolve its name to a register**:

```
probe p() { printf("%u\n", x) }   # x resolves to REG_X here
counter x                          # error
```

```
FATAL: ...: counter 'x' is declared after a probe resolved 'x' as a register
```

Reversing the two declarations makes `x` a counter everywhere after the
`counter x` line, including inside `p`. The restriction exists so the same
spelling can never compile to `LOAD_REG` in one probe and `LOAD_COUNTER` in
another depending on where a declaration happened to land — the kind of
silent divergence this whole tool exists to catch, here relocated into the
script that defines the probes.

**Reserved names**, matched before identifier lookup and never shadowable —
declaring a counter, parameter, or probe with one of these names is an error
("'if' is a reserved name"):

```
peek8  peek16  hash
counter  probe  install
if  else  printf  inc  key  stop
```

Registers (`a x y sp sr pc`) are *not* on this list — they are ordinary
identifiers a declaration is free to shadow, per the resolution order above.

## `printf`

```
printf("<format>", <expr>, ...)
```

The format string must be a literal string token directly after `printf(` —
not a variable, not a concatenation. Accepted conversions: `%d` `%u` `%x`
`%X`, each optionally preceded by any combination of the flags `0` `-` `+`
and space, and a width up to 999 digits — `%05d`, `%-8u`, `%+d`, `% d` all
compile. `%%` is a literal percent sign. That is a wider flag set than the
design doc's "width and zero-pad" mentions, but since the VM does not exist
yet, none of it — including whether width/zero-pad is honored the way C's
`printf` would — has defined runtime behavior; only that it compiles.

**Not supported, matching what the design explicitly rules out**: `%s`,
`%p`, `%n`, any floating-point conversion, a precision (`%.2d` is rejected
with a dedicated message, not a generic "unsupported conversion"), and
length modifiers (`%ld`, `%hd`). The format string is interpreted by the
probe's own formatter, not handed to libc, specifically to avoid `%s`/`%n`
and locale sensitivity — see the design doc's "The report" section.

**Argument count is checked when the probe compiles**, against the number of
`%`-conversions actually present (`%%` does not count): `printf("%d")` is
"format needs 1 argument(s), 0 given"; `printf("ok", 1)` is "format needs 0
argument(s), 1 given." A mismatch is a parse error, not a runtime surprise.

Identical format strings — even across different probes — are interned once
and share one entry in the dump's `formats:` table; see [dump
format](#dump-format) below.

## Command-line options

All three are `a2host` options, so every front end that links it (`a2run`,
`a2emu`) has them.

| option | effect |
|---|---|
| `--probe=<path>` | Load and compile a probe script. At most one per run — a second `--probe=` is "only one probe script may be loaded." |
| `--probe-out=<path>` | Where a probe's `printf` output will go, once the VM exists. Requires `--probe=`. |
| `--probe-dump` | Compile the loaded script, print its bytecode to stdout, and exit — no simulation runs. Requires `--probe=`. |

`--probe-dump` and `--probe-out=` without a preceding `--probe=` are both
rejected regardless of argument order (checked once, after the whole command
line is parsed).

## Dump format

`--probe-dump` prints four sections, in this order, always present even when
empty:

```
counters: <n>
  <index> <name> = <value>
  ...
formats: <n>
  <index> "<escaped string>"
  ...
probes: <n>
  <index> <name>(<param>, ...) init=<offset> body=<offset>
        <ip>: <OPNAME> [<operand>]
        ...
sites: <n>
  $<ADDR> <probe-name>
  ...
```

- **`counters`**: declaration order, resolved initializer value (unsigned
  32-bit — a negative initializer is shown wrapped, as in the `counter`
  example above).
- **`formats`**: one entry per *distinct* format string, in first-use order,
  with escapes rendered visibly (`\n`, `\t`, `\"`, `\\`, and any other
  non-printable byte as `\xHH`) so a dump stays one line per string
  regardless of what it contains.
- **`probes`**: declaration order. `init=` and `body=` are cell offsets into
  the shared instruction stream — `init` is where the parameter
  initializers start, `body` is where they fall through to after pushing
  all `nparams` values; a probe with no parameters has `init == body`.
  Instructions run from `init` through one past the trailing `END`.
- **`sites`**: `<n>` is the count of *distinct installed addresses*, not the
  number of lines that follow — several probes sharing one address (script
  order) each get their own line. Listed in ascending address order,
  regardless of the order `install` declared them, so the output is
  independent of the hash table's internal layout.

`tests/probe/install.expected` shows both of the last point's cases —
`$6100` carries two probes (`one` then `two`, matching install order), while
`sites: 8` counts the eight distinct addresses:

```
counters: 1
  0 n = 0
formats: 0
probes: 2
  0 one() init=0 body=0
        0: LOAD_COUNTER 0
        2: PUSH_LIT 1
        4: ADD
        5: STORE_COUNTER 0
        7: END
  1 two() init=8 body=8
        8: LOAD_COUNTER 0
       10: PUSH_LIT 1
       12: ADD
       13: STORE_COUNTER 0
       15: END
sites: 8
  $0300 one
  $0310 one
  $0320 one
  $6100 one
  $6100 two
  $6200 two
  $6201 two
  $6202 two
  $6203 two
```

## Opcodes

One cell per opcode, operands (if any) in the following cells. `opcode_t` in
`probe_internal.h` is the source of truth; the operand counts below are from
`has_operand()`/`OP_PRINTF`'s special case in `probe.c`.

| group | opcode | operands | notes |
|---|---|---|---|
| load | `PUSH_LIT` | 1 | literal value |
| | `LOAD_PARAM` | 1 | parameter index |
| | `LOAD_COUNTER` | 1 | counter index |
| | `LOAD_REG` | 1 | `reg_t`: `0`=A `1`=X `2`=Y `3`=SP `4`=SR `5`=PC |
| | `PEEK8` | 0 | pops address |
| | `PEEK16` | 0 | pops address |
| | `HASH` | 0 | pops `hi` then `lo`; source order is `hash(lo, hi)` |
| store | `STORE_PARAM` | 1 | parameter index; pops the value to store |
| | `STORE_COUNTER` | 1 | counter index; pops the value to store |
| arith | `ADD` `MUL` | 0 | commutative -- pop order is unobservable |
| | `SUB` `DIV` `MOD` | 0 | pop the right (2nd-pushed) operand, then the left (1st); result is `left OP right` (`DIV`/`MOD`: left is the dividend) |
| bitwise | `AND` `OR` `XOR` | 0 | commutative -- pop order is unobservable |
| | `SHL` `SHR` | 0 | pop the right (2nd-pushed, the shift count), then the left (1st, the value); result is `left OP right` |
| compare | `EQ` `NE` | 0 | commutative -- pop order is unobservable |
| | `LT` `LE` `GT` `GE` | 0 | pop the right (2nd-pushed) operand, then the left (1st); result is `left OP right` |
| unary | `NOT` `BITNOT` `NEG` | 0 | |
| control | `JMP` | 1 | target: an **absolute** cell index into the instruction stream, not an offset from the current position |
| | `JZ` | 1 | target (absolute, as above); pops the tested value |
| | `JNZ` | 1 | target (absolute, as above); pops the tested value |
| effect | `PRINTF` | 2 | format-table index, argument count; pops that many values. Arguments are pushed in source order, so the *last* argument is on top and pops first -- a VM must reverse them to fill the format's conversions in source order |
| | `KEY` | 0 | pops a stamp |
| | `STOP` | 0 | |
| | `END` | 0 | marks the end of a probe's instructions |

For every non-commutative binary opcode above, both operands are pushed in
source (left-to-right) order — the left/first operand ends up deepest on the
stack, the right/second operand on top — and the opcode pops the top (right)
operand first. `opcode_t` in `probe_internal.h` is the definitive listing,
with the same pop order spelled out per opcode as a comment; if this table and
that header ever disagree, the header is the source of truth. A VM that
popped one of these backwards would still pass every test in
`tests/run-tests.sh` today, since nothing there executes bytecode — see
[Execution status](#execution-status).

**Parameter frame.** A probe's `nparams` parameter initializers (running from
`init` to `body` in the dump format above) each leave exactly one value on the
stack, in declaration order, none of them popped again before falling through
to the body. Those `nparams` values *are* the parameter frame — there is no
separate copy into named storage. `OP_LOAD_PARAM i` / `OP_STORE_PARAM i`
address stack position `frame_base + i`, where `frame_base` is the stack depth
on entry to `init` — a fixed offset from the bottom of the frame, not from
wherever the stack top happens to be mid-expression. See `probe_t::init_offset`
in `probe_internal.h` for the full contract.

**Stack depth.** No maximum simultaneous stack depth is computed or recorded
anywhere in the compiled form — there is no constant for it, and nothing walks
a probe's bytecode to size one. It is bounded in practice by a combination of
`PROBE_MAX_EXPR_DEPTH`, the parameter frame (up to `PROBE_MAX_PARAMS` values
live for a probe's whole run), and however many arguments accumulate before
one `PRINTF`, but none of that is tracked as a single number. See the comment
above `enum { PROBE_MAX_COUNTERS = ... }` in `probe_internal.h` for the
detailed reasoning; a VM needs to either compute a per-probe maximum itself or
provision generously (or grow dynamically) rather than assume the compiler
already sized this.

## Limits

Every "too many" / "too long" diagnostic is a fixed compile-time constant in
`probe_internal.h`, checked with a rejection test in `tests/run-tests.sh`:

| limit | value | constant |
|---|---|---|
| identifier length | 64 (including the terminator) | `PROBE_MAX_IDENT` |
| string literal length | 256 (including the terminator) | `PROBE_MAX_STRING` |
| counters per script | 64 | `PROBE_MAX_COUNTERS` |
| probes per script | 256 | `PROBE_MAX_PROBES` |
| parameters per probe | 16 | `PROBE_MAX_PARAMS` |
| distinct format strings | 256 | `PROBE_MAX_FORMATS` |
| bytecode cells (whole script) | 65536 | `PROBE_MAX_CODE` |
| install-site bindings (whole script) | 8192 | `PROBE_MAX_SITE_DECLS` |
| expression nesting depth | 250 | `PROBE_MAX_EXPR_DEPTH` |
| statement nesting depth (blocks, `if`/`else`) | 250 | `PROBE_MAX_STMT_DEPTH` |

## Diagnostics

Every error is fatal: `FATAL: <message>` (or `FATAL: <path>:<line>:
<message>` for anything the lexer/parser catches, which is almost
everything) on stderr, `exit(2)`. There is no recovery and no partial
compile — a probe script that fails to load must not run with half its
probes silently missing, which would be exactly the kind of quiet gap this
tool exists to prevent.

A handful of diagnostics have no `path:line`, because they are not about a
specific place in the script: `--probe-dump requires --probe=<script>`,
`--probe-out requires --probe=<script>`, `only one probe script may be
loaded`, and file-open failures (`cannot open probe script '<path>':
<reason>`).

## Execution status

`key` and `stop` only make sense once something is running the bytecode.
Today: `key <expr>` will, once implemented, deliver every keystroke recorded
with a stamp ≤ `<expr>` into the keyboard queue; `stop` will end the run
cleanly, the same way on both the interpreter and a generated program.
Neither executes anything today — `probe_dispatch()` is an empty stub, and
nothing calls it. `--probe-out=` compiles and is accepted, but nothing is
written to the path yet, because nothing runs. This document covers
everything that exists to check against; `docs/plans/2026-08-11-probes-design.md`
covers the execution model these opcodes are designed for.
