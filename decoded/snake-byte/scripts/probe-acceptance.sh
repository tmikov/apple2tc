#!/bin/bash
#
# The cross-engine gate for probe-stamped input: the interpreter and each
# generated build must produce byte-identical block-head traces, and identical
# memory, when replayed from the .pkeys files -- keystrokes stamped on the probe
# counter that play.probe/rec.probe define, rather than on raw cycles. Both of
# verify.sh's scenarios are run: play.pkeys (23 keys) and play-hires.pkeys (19).
# Modeled directly on decoded/rom/probe-acceptance.sh; read that header first.
#
# Why this is stricter than verify.sh. verify.sh (play.keys, cycle-stamped)
# diffs one video hash per frame -- 1,300 numbers. This diffs the address of
# every basic block dispatched over the same run, which is on the order of
# 2.7 million lines. Two programs can hash the same screen at frame boundaries
# while taking different paths to get there; they cannot produce the same
# 2.7-million-line PC trace by accident. And ram.probe then compares memory at
# every in-game sample, because a wrong byte written need not change control
# flow within one run. trace.probe carries play.probe's own `kb` delivery (same
# sites, same counter, `key` instead of `record`) so the replay lands on the
# coordinate the .pkeys file was stamped on, not on wall cycles -- see
# rec.probe for why the coordinate needs exactly those sites, and for the
# measurements behind $6217 and $760F.
#
# Why two site lists. snake-bytec1.c decompiles the ROM alongside the game and
# is self-contained. snake-bytec1-ext.c has the ROM entry points listed in
# rom.externs, and the game's own $664A COUT hook, replaced by hand-written
# a2rom.c/game.c (stitched into one translation unit by snake-byte-ext.c --
# see that file's header for why they must share a TU). The two therefore
# expose different sets of block heads: every CYCLES(0x... site the compiled
# binary can actually be probed at, which for the -ext build spans all three
# source files, not just the generated one. Installing a probe at an address
# that isn't a block head in the binary under test would fire in the
# interpreter, not exist on the generated side, and still diff clean -- the
# one failure mode this design cannot otherwise see (see AddEdgeCycles in
# tools/apple2tc/ir/Values.def for why CYCLES_EDGE(0x is deliberately excluded
# from the grep).
#
# Each backend gets its own site list and its own interpreter run, rather than
# asserting the two lists are equal and sharing one run -- the ext list is in
# fact a strict subset of the ref list today (see the note this script prints
# at the end), but nothing requires that to stay true.

set -e
# The site-list pipelines below (`grep | sed | sort -u`) run under `set -e`
# alone, `$?` is `sort`'s, not `grep`'s: a `grep` that hard-fails on a
# missing/renamed source file (exit 2, plus a warning on stderr) would still
# leave `sort` exiting 0, so the script would sail on with a silently
# shrunken site list. pipefail makes the pipeline's status the worst of its
# parts, so that failure now aborts here instead.
set -o pipefail

if [ -z "$1" ]; then
  echo "Usage: $0 <build-dir>" >&2
  exit 1
fi

bin=$1
a2run="$bin/tools/a2run/a2run"
here=$(dirname "$0")/..

[ -x "$a2run" ] || { echo "Error: not found: $a2run" >&2; exit 1; }

# ---------------------------------------------------------------------------
# Before that: are the binaries under test actually built from the sources in
# the tree?
#
# Everything below runs programs out of the build directory. If one of them is
# older than the source it came from, every check still runs and every check
# still passes -- against the previous build. That is not hypothetical either:
# it happened twice on 2026-08-24, once because ninja decided it had no work to
# do for a file it had just been handed, and once because the build had failed
# and the old executable was still sitting there. Both times this script
# reported six green cold checks about code that was not being run.
#
# Cheap to rule out: every program is newer than every source that can change
# what it does.
stale=0
for prog_src in     "snake-byte-run:snake-byte.c game-image.inc rom-image.inc"     "snake-bytec1-ext-run:reference/snake-bytec1-ext.c reference/snake-byte-ext.c reference/a2rom.c reference/game.c reference/game_native.c"     "snake-byte-easyc1-ext-run:reference/snake-byte-easyc1-ext.c reference/snake-byte-easy-ext.c reference/a2rom.c reference/game.c reference/game_native.c"     "snake-bytec1-run:reference/snake-bytec1.c"; do
  prog="$bin/decoded/snake-byte/${prog_src%%:*}"
  [ -x "$prog" ] || continue
  for src in ${prog_src#*:}; do
    [ -f "$here/$src" ] || continue
    if [ "$here/$src" -nt "$prog" ]; then
      echo "FAIL: $(basename "$prog") is older than $src -- rebuild before trusting this" >&2
      stale=1
    fi
  done
done
[ "$stale" -eq 0 ] || exit 1

# ---------------------------------------------------------------------------
# Before anything else: is the committed generated C what decompile.sh
# produces?
#
# Everything below this point runs the *committed* .c files. If they have
# drifted from the decompiler that supposedly generated them, every check in
# this script still passes -- it is comparing an interpreter against a build of
# a stale artifact, and agreeing about it. The site counts do not notice
# either, because they are grepped from the same stale text.
#
# That is not hypothetical. Until 2026-08-23 the two ext files had been written
# by an emitter whose line breaking had since moved, so `./decompile.sh`
# rewrote them by ~12,000 lines apiece; the drift was cosmetic that time, and
# nothing here would have said so had it not been.
#
# Regenerating into a temporary directory costs a couple of seconds and is the
# only check in this file that looks at the artifacts rather than through them.
# It is deliberately first: a stale artifact makes every later PASS a statement
# about the wrong file.
regen=$(mktemp -d)
trap 'rm -rf "$regen"' EXIT
"$here/scripts/decompile.sh" "$bin" "$regen" > /dev/null 2>&1 || {
  echo "FAIL [regen]: decompile.sh failed against $bin" >&2
  exit 1
}
# decompile.sh writes flat into its output directory; the committed copies live
# under reference/ and testdata/, so the pairs are spelled out rather than
# derived from one name.
for pair in reference/snake-bytec1.c reference/snake-bytec1-ext.c \
            reference/snake-byte-easyc1-ext.c testdata/coverage.txt; do
  if ! cmp -s "$regen/$pair" "$here/$pair"; then
    echo "FAIL [regen]: $pair is not what decompile.sh produces" >&2
    echo "  Regenerate and commit it with whatever input changed." >&2
    # Layout drift and a real change in the decompiler's output look the same
    # in `git diff` and are not the same thing, so say which this is.
    if [ "$(tr -d " \t\n" < "$regen/$pair" | cksum)" = "$(tr -d " \t\n" < "$here/$pair" | cksum)" ]; then
      echo "  Whitespace-stripped the two are identical: layout only, no change in output." >&2
    else
      echo "  The tokens differ: the decompiler's output really has changed. Read the diff." >&2
    fi
    exit 1
  fi
done
echo "[regen] the committed generated C is what decompile.sh produces"

frames=${FRAMES:-1300}
b33="$here/testdata/snake-byte.b33"
keys="${KEYS:-$here/testdata/play.pkeys}"

# $b33 and $frames are per-scenario: the `easy` scenario at the bottom replays
# a patched image for longer. check_backend and check_memory read both.

# Every script that delivers keys carries its own copy of the coordinate --
# the probe language has no include, so the `install kb at ...` line is
# duplicated five ways. They must all name the same sites in the same order or
# the stamps in a .pkeys file mean different things to different scripts, and
# each script stays individually valid while the set quietly disagrees. That is
# the same drift hazard the trace.probe/trace-ext.probe check covers, so it is
# checked the same way: asserted, not trusted.
coord=$(grep -h '^install kb at ' "$here/testdata/rec.probe")
for p in testdata/play.probe testdata/trace.probe testdata/trace-ext.probe \
         testdata/trace-easy.probe testdata/trace-cold.probe testdata/ram.probe \
         testdata/ram-cold.probe testdata/screen.probe; do
  if [ "$(grep -h '^install kb at ' "$here/$p")" != "$coord" ]; then
    echo "FAIL: $p installs the coordinate differently from rec.probe" >&2
    echo "  rec.probe: $coord" >&2
    echo "  $p: $(grep -h '^install kb at ' "$here/$p")" >&2
    exit 1
  fi
done
echo "coordinate: ${coord#install kb at }"

# The number of keys a correct run must load and deliver, derived from the
# .pkeys file itself rather than hardcoded: an empty or truncated key file (a
# regression in the file, or a mis-parse) would otherwise still make the rest
# of this script pass -- see the "Loaded N keys" check below for why an empty
# key file is exactly the failure mode this exists to catch. `|| true`: grep
# exits 1 on zero matches (an empty/all-comment file), which would otherwise
# trip `set -e` and abort with no explanation before the check below runs.
set_scenario() {
  # $1: the .pkeys file for this scenario. Sets $keys and $expected_keys, which
  # check_backend and check_memory both read.
  keys=$1
  if [ ! -f "$keys" ]; then
    echo "FAIL: $keys does not exist" >&2
    exit 1
  fi
  expected_keys=$(grep -vc '^#' "$keys" || true)
  if [ "$expected_keys" -eq 0 ]; then
    echo "FAIL: $keys has no keys to replay -- empty or all-comment" >&2
    exit 1
  fi
  echo "--- scenario $(basename "$keys"): $expected_keys keys ---"
}

# Every probe site a set of sources defines, one hex address per line, sorted.
#
# Two spellings count. A bare `CYCLES(0x...` is a block head in generated or
# hand-written emulator-shaped C. `GAME_CYCLES_COORD(0x...` is the exception
# converted real C is allowed: a site that keeps its probe because the replay
# coordinate counts it. The plain `GAME_CYCLES` of converted code is neither --
# it charges cycles and is deliberately unprobed -- and the leading
# `[^_A-Za-z]` is what keeps it out, since `CYCLES(0x` is a substring of
# `GAME_CYCLES(0x`.
site_addrs() {
  grep -ohE '(^|[^_A-Za-z])CYCLES\(0x[0-9a-f]+|GAME_CYCLES_COORD\(0x[0-9a-f]+|GAME_CYCLES_ANCHOR\(0x[0-9a-f]+' "$@" \
    | sed 's/.*0x//' | sort -u
}

# Compare one generated build against the interpreter on that build's own
# block heads. $1: label, $2: generated binary, $3: probe script, $4: where to
# keep the site list, $5: the exact expected site count, $6..: the C
# source(s) that make up the binary.
check_backend() {
  local label=$1 prog=$2 probe=$3 sites=$4 expect=$5
  shift 5
  local srcs=("$@")
  local tag="$label-$(basename "$keys" .pkeys)"
  local interp="/tmp/pkeys-probe-interp-$tag.txt" gen="/tmp/pkeys-probe-$tag.txt"

  [ -x "$prog" ] || { echo "Error: not found: $prog" >&2; exit 1; }

  # Block heads, straight from the artifact(s) that define them. The list is
  # committed so a failure is bisectable without regenerating the C first, but
  # it is still rebuilt here so it never goes stale relative to the C actually
  # being tested.
  site_addrs "${srcs[@]}" > "$sites"
  local nsites
  nsites=$(wc -l < "$sites")
  echo "[$label] site list: $nsites block heads"
  # Exact, not a floor. It began as a floor -- a generous margin, enough to
  # notice a source file going missing out of the grep. That was the only way
  # the number could move. It is not any more: every routine converted to real
  # C in game_native.c takes its block heads out of this list on purpose, so
  # the count is now the running cost of the conversion. Pinning it exactly is
  # what puts that cost in the diff instead of in a number nobody reads.
  if [ "$nsites" -ne "$expect" ]; then
    echo "FAIL [$label]: site list has $nsites block heads, expected exactly $expect" >&2
    echo "  A conversion lowers this deliberately; anything else is a regression." >&2
    exit 1
  fi

  "$a2run" --preload "$b33" --key-file="$keys" --probe="$probe" \
    --probe-out="$interp" --frames="$frames" \
    > /dev/null 2>"/tmp/pkeys-probe-interp-$tag.err"
  "$prog" --key-file="$keys" --probe="$probe" \
    --probe-out="$gen" --frames="$frames" \
    > /dev/null 2>"/tmp/pkeys-probe-$tag.err"

  # A probe that never fired means the two sides did not cover the same set,
  # even if what they did emit matches -- e.g. a site both installed but
  # neither ever executed would leave two empty reports that diff clean.
  #
  # Neither check below is enough on its own to prove keys were actually
  # delivered: an empty --key-file= makes both engines produce identical,
  # "never fired"-free traces too -- the reports agree vacuously because
  # nothing input-dependent ran. "Loaded $expected_keys keys" is a2host's own
  # confirmation (lib/a2host/a2host.c's load_key_file()) that the file was
  # read and parsed, not skipped, empty, or truncated; asserting it in BOTH
  # runs is what makes this gate mean "the two engines agree with real input
  # in play," not just "the two engines agree."
  local side
  for side in "interp-$tag" "$tag"; do
    if grep -q "never fired" "/tmp/pkeys-probe-$side.err"; then
      echo "FAIL [$side]: a probe never fired" >&2
      grep "never fired" "/tmp/pkeys-probe-$side.err" >&2
      exit 1
    fi
    if ! grep -qx "Loaded $expected_keys keys" "/tmp/pkeys-probe-$side.err"; then
      echo "FAIL [$side]: expected to see 'Loaded $expected_keys keys' on stderr" \
           "(from $keys) -- keys were not actually delivered" >&2
      cat "/tmp/pkeys-probe-$side.err" >&2
      exit 1
    fi
  done

  if ! diff -q "$interp" "$gen" > /dev/null; then
    echo "FAIL [$label]: the interpreter and the generated build disagree" >&2
    diff "$interp" "$gen" | head -20 >&2
    echo "interp hits: $(wc -l < "$interp")" >&2
    echo "gen hits:    $(wc -l < "$gen")" >&2
    exit 1
  fi

  echo "[$label] PASS: $(wc -l < "$interp") probe hits match over $frames frames"

  # Accumulate which addresses actually fired, for coverage_report below. The
  # "never fired" check above is per *probe*, and trace is a single probe
  # installed at every block head, so one address that never executes is
  # invisible to it -- the report still diffs clean because both engines agree
  # about running nothing. The trace prints upper-case hex; the site list is
  # lower-case.
  tr 'A-F' 'a-f' < "$interp" >> "/tmp/pkeys-cover-$label.txt"
}

# What fraction of the site list the two scenarios actually reach, and -- the
# part that matters -- whether any hand-written routine contains a block that
# never runs. A hand-decoded block nothing executes is not compared against
# anything: the whole cross-engine argument for it is vacuous, and a mutation
# test there passes every gate. $baseline is the number of such blocks known
# and accepted at the time of writing; growing it is a regression.
#
# $1 label, $2 site list, $3 baseline, $4.. the hand-written sources.
coverage_report() {
  local label=$1 sites=$2 baseline=$3
  shift 3
  local hand=("$@")
  local hit="/tmp/pkeys-cover-hit-$label.txt"
  sort -u "/tmp/pkeys-cover-$label.txt" > "$hit"
  echo "[$label] coverage: $(comm -12 "$sites" "$hit" | wc -l)/$(wc -l < "$sites")" \
       "block heads run in at least one scenario"

  [ ${#hand[@]} -gt 0 ] || return 0
  local handsites="/tmp/pkeys-cover-hand-$label.txt"
  site_addrs "${hand[@]}" > "$handsites"
  local dead ndead
  dead=$(comm -23 "$handsites" "$hit")
  ndead=$(printf '%s\n' "$dead" | grep -c . || true)
  echo "[$label] hand-written: $(( $(wc -l < "$handsites") - ndead ))/$(wc -l < "$handsites") run"
  if [ "$ndead" -gt "$baseline" ]; then
    echo "FAIL [$label]: $ndead hand-written block heads never run, baseline is $baseline" >&2
    echo "$dead" >&2
    exit 1
  fi
  [ "$ndead" -eq 0 ] || echo "[$label] unverified: $(echo $dead)"
}

# Every CYCLES in a hand-written source must carry a literal hex address.
#
# The site lists are built by grepping the C for `CYCLES(0x...`, so an address
# that is any other expression -- a table lookup, a variable, a macro -- still
# compiles, still counts cycles, and still runs, but never reaches the list.
# The probe is then not installed there, neither engine reports it, and the
# diff is clean because both sides say nothing. It is the block-head hazard
# again, arriving through the hand-written side instead of the generated one.
#
# Measured, not hypothetical: a draft of game_update_high_score looped over a
# table of addresses and quietly took eight sites out of the gate, and nothing
# else here noticed -- the floor below is far too coarse to see 1669 become
# 1661.
# Strip comments before looking for code. Prose about CYCLES is not a CYCLES
# site, and two false positives from this file's own explanatory comments is
# enough to make the check see what the compiler sees.
strip_comments() {
  sed -e 's|//.*||' -e 's|/\*[^*]*\*/||g' -e 's|^[[:space:]]*\*.*||' \
      -e 's|^[[:space:]]*/\*.*||' "$1"
}

check_literal_sites() {
  local bad f
  bad=""
  for f in "$@"; do
    bad="$bad$(strip_comments "$f" | grep -nE 'CYCLES(_EDGE)?\([^)]' \
      | grep -vE 'CYCLES(_EDGE)?\(0x[0-9a-f]+' | sed "s|^|$f:|" || true)"
  done
  if [ -n "$bad" ]; then
    echo "FAIL: a CYCLES site has a non-literal address, so it cannot be probed" >&2
    echo "$bad" >&2
    exit 1
  fi
}
check_literal_sites "$here/reference/a2rom.c" "$here/reference/game.c"

# game_native.c must contain no CYCLES at all.
#
# It is not in the site-list grep above, and must not be: it holds real C, with
# no block structure to account for. But that also means a CYCLES written there
# would charge cycles and never be probed -- the same silent hole as a
# non-literal address, arriving from the other direction. Block accounting
# belongs in the adapter in game.c, which is what keeps a converted routine's
# trace intact.
# GAME_CYCLES (i.e. CYCLES_EDGE) is how converted code keeps its timing exact
# without claiming a probe site, and is expected here. A plain CYCLES is not:
# it would charge cycles and never be probed, since this file is deliberately
# absent from the grep above -- the same silent hole as a non-literal address,
# arriving from the other side.
if strip_comments "$here/reference/game_native.c" | grep -qE '(^|[^_A-Za-z])CYCLES\([^)]'; then
  echo "FAIL: game_native.c uses CYCLES; converted code must use GAME_CYCLES" >&2
  strip_comments "$here/reference/game_native.c" | grep -nE '(^|[^_A-Za-z])CYCLES\([^)]' >&2
  exit 1
fi

# ...with two spelled exceptions, and both are bounded here.
#
# Converted code normally takes its block heads out of the comparison
# altogether: nothing probes them on either engine, and the two agree by saying
# nothing. Two situations break that, and each gets its own spelling so the
# reason is at the call site rather than in a list somewhere else.
#
# GAME_CYCLES_COORD: replay stamps keys on a counter incremented at the seven
# addresses $coord names, and two of them sit inside routines being converted.
# Dropping the probe there stops the counter and every later keystroke lands at
# a different instant. It is caught -- only the generated side drifts -- but as
# a 640,000-line trace diff naming an unrelated screen.
#
# GAME_CYCLES_SHARED: some other source still emits a CYCLES for the same
# address, so the interpreter reports it and the converted build would not.
# Twice so far: $6216, an RTS two routines share, one converted and one not;
# and $720E, the low half of $71F3, which survives in the generated C as an
# orphan nothing can reach but which is still text in the file and so still on
# the site list.
#
# The pinned site count sees neither: it was right both times. So all three
# spellings are checked against the site list built from every source but this
# one. `|| true` on each grep: finding none is legitimate, and pipefail would
# otherwise hand grep's exit 1 to set -e and abort with no message.
native_addrs() {
  # $1: the macro name, exactly. Its own name is anchored so that GAME_CYCLES
  # does not also match GAME_CYCLES_SHARED.
  strip_comments "$here/reference/game_native.c" \
    | grep -ohE "$1\\(0x[0-9a-f]+" | sed 's/.*0x//' | sort -u || true
}

# An adapter's entry probe is the one legitimate way an address can be in both
# files. `CYCLES(addr, 0)` in game.c fires the probe and charges nothing; the
# native routine then charges the cycles with GAME_CYCLES and does not probe.
# Between them the address is probed once and charged once, which is how a
# converted routine keeps its own entry in the trace for free. Recognised by
# the zero, and only in game.c -- the generated C has zero-cycle sites of its
# own, at addresses of its own.
adapter_entry_sites() {
  grep -ohE 'CYCLES\(0x[0-9a-f]+, 0\)' "$here/reference/game.c" | sed 's/.*0x//;s/,.*//' | sort -u || true
}

check_native_spellings() {
  # $1 label, $2.. every source of this build except game_native.c.
  local label=$1
  shift
  local others entries plain shared coordu bad
  others=$(site_addrs "$@")
  entries=$(adapter_entry_sites)
  plain=$(native_addrs GAME_CYCLES)
  shared=$(native_addrs GAME_CYCLES_SHARED)
  coordu=$(native_addrs GAME_CYCLES_COORD)

  # An entry site must be the adapter's alone. If the generated C emits the
  # same address the zero-cycle probe is not the only one, and the exemption
  # below would be hiding a real divergence.
  bad=$(comm -12 <(echo "$entries") <(site_addrs "$1"))
  if [ -n "$bad" ]; then
    echo "FAIL [$label]: adapter entry probe at an address the generated C also emits:" >&2
    echo "$bad" >&2
    exit 1
  fi

  bad=$(comm -12 <(echo "$plain") <(comm -23 <(echo "$others") <(echo "$entries")))
  if [ -n "$bad" ]; then
    echo "FAIL [$label]: game_native.c drops the probe at addresses another source still emits:" >&2
    echo "$bad" >&2
    echo "  Use GAME_CYCLES_SHARED, or the interpreter will report them and the build will not." >&2
    exit 1
  fi

  bad=$(comm -23 <(echo "$shared") <(echo "$others"))
  if [ -n "$bad" ]; then
    echo "FAIL [$label]: GAME_CYCLES_SHARED at addresses no other source emits:" >&2
    echo "$bad" >&2
    echo "  Nothing is sharing them; plain GAME_CYCLES is what they want." >&2
    exit 1
  fi

  bad=$(comm -23 <(echo "$coordu") <(echo "$coord_addrs"))
  if [ -n "$bad" ]; then
    echo "FAIL [$label]: GAME_CYCLES_COORD at addresses that are not on the coordinate:" >&2
    echo "$bad" >&2
    exit 1
  fi

  echo "[$label] probes kept in converted code: $(echo ${shared:-none}) (shared)," \
       "$(echo ${coordu:-none}) (coordinate)"
}

coord_addrs=$(echo "${coord#install kb at }" | tr -d ' ' | tr ',' '\n' | tr -d '$' \
  | tr 'A-F' 'a-f' | sort -u)
check_native_spellings ext "$here/reference/snake-bytec1-ext.c" "$here/reference/a2rom.c" "$here/reference/game.c"
check_native_spellings easy "$here/reference/snake-byte-easyc1-ext.c" "$here/reference/a2rom.c" "$here/reference/game.c"

# The built program must start on its own, with no key file at all.
#
# The decompilation boots to the Applesoft prompt, exactly as the machine did,
# and snake-byte-ext.c wraps init_emulated() to queue `call 14160` through
# a2host_buffer_keys(). If that wiring breaks the binary still runs, still
# passes every check below -- they all supply --key-file -- and simply sits at
# a prompt forever. So it is asserted here rather than noticed by a person.
#
# $6217 is the in-game keyboard ingest: it fires only once the game is running,
# which is the thing being claimed.
check_autostarts() {
  local prog=$1 out="/tmp/pkeys-autostart-$(basename "$prog").txt"
  local probe="/tmp/pkeys-autostart.probe"
  cat > "$probe" <<'PROBE'
counter n
probe hit() { inc n
  printf("%d\n", n) }
install hit at $6217
PROBE
  "$prog" --probe="$probe" --probe-out="$out" --frames=1300 > /dev/null 2>&1
  local hits
  hits=$(tail -1 "$out" 2>/dev/null || true)
  if [ -z "$hits" ] || [ "$hits" -lt 1000 ]; then
    echo "FAIL [autostart]: $(basename "$prog") did not reach the game unaided" >&2
    echo "  in-game keyboard polls over 1300 frames: ${hits:-0}, expected >= 1000" >&2
    exit 1
  fi
  echo "[autostart] $(basename "$prog"): ${hits} in-game polls, no key file"
}
check_autostarts "$bin/decoded/snake-byte/snake-bytec1-ext-run"
check_autostarts "$bin/decoded/snake-byte/snake-byte-easyc1-ext-run"

# trace.probe and trace-ext.probe name themselves after the backend they
# install over, so the two runs cannot silently share one. They must otherwise
# be the same program -- checked here rather than trusted, since an edit to
# one and not the other would leave both halves individually valid while
# quietly testing two different things.
strip_probe() {
  # Drop comments, blank lines and the install lines, leaving the program.
  grep -vE '^\s*(#|install\b|$)' "$1"
}
for other in trace-ext trace-easy trace-cold; do
  if ! diff -q <(strip_probe "$here/testdata/trace.probe") <(strip_probe "$here/testdata/$other.probe") \
       > /dev/null; then
    echo "FAIL: trace.probe and $other.probe are not the same program" >&2
    diff <(strip_probe "$here/testdata/trace.probe") <(strip_probe "$here/testdata/$other.probe") >&2
    exit 1
  fi
done

# The two expected counts. 1,694 for the reference build, which is pure
# decompiler output and moves only if the decompiler does. 1,171 for the two
# extern builds, which is the same figure less the ROM bodies a2rom.c supplies,
# the return edges --prune-returns removes, and every block head given up so
# far to game_native.c -- see that file's header for what each conversion cost
# and why.
#
# Only the last of those three moves in ordinary work, and it only ever falls.
# Write the new number here in the same commit as the conversion that lowers
# it; a count that drifts on its own is the one thing this check exists to
# catch.
# The scenarios, as `<keys>:<frames>`. The frame count is per scenario because
# they are not the same length: a recording is only worth replaying for as long
# as it has keys left to deliver.
#
# play and play-hires are the two verify.sh replays, now run against the
# interpreter as well. play-hires is not a duplicate of play: it reaches $664A
# (the game's own hi-res COUT hook) and $7541, code the recording never took
# and that exists only via --code-at -- and in the -ext build $664A is
# hand-written C, so the hires/ext pair is the only cross-engine check that
# replacement gets.
#
# play-rebind is what the other two could not do. Both of them press the
# *default* direction keys, where $6C63 and $6C6A hold identical bytes, so the
# substitution $6C93/$6C9E performs is invisible -- reading the wrong table
# passed every check in this file. This one rebinds to W A S Z Q E and then
# plays with them: 8 substitutions with the two tables differing. It also
# enters $69A9 for the first time.
#
# KEYS= overrides the list for a one-off run; entries still need their `:frames`.
rm -f /tmp/pkeys-cover-*.txt
for scenario in ${KEYS:-"$here/testdata/play.pkeys:1300" "$here/testdata/play-hires.pkeys:1300" \
                        "$here/testdata/play-rebind.pkeys:3300"}; do
  IFS=: read -r keyfile frames <<<"$scenario"
  set_scenario "$keyfile"

check_backend trace "$bin/decoded/snake-byte/snake-bytec1-run" "$here/testdata/trace.probe" \
  "$here/testdata/blocks.txt" 1694 "$here/reference/snake-bytec1.c"
check_backend trace-ext "$bin/decoded/snake-byte/snake-bytec1-ext-run" "$here/testdata/trace-ext.probe" \
  "$here/testdata/blocks-ext.txt" 1171 "$here/reference/snake-bytec1-ext.c" "$here/reference/a2rom.c" "$here/reference/game.c" \
  "$here/reference/game_native.c"

if diff -q "$here/testdata/blocks.txt" "$here/testdata/blocks-ext.txt" > /dev/null; then
  echo "the two back ends agree on the block-head set"
else
  echo "note: the back ends emit different block-head sets ($(wc -l < "$here/testdata/blocks.txt")" \
       "vs $(wc -l < "$here/testdata/blocks-ext.txt")); each was checked against its own"
fi

# Memory equivalence. The checks above prove the engines take the same path;
# this proves they compute the same values along it. A wrong byte written to
# memory need not change control flow within 1300 frames, so neither check
# subsumes the other. ram.probe documents why the stack page is excluded and
# why that exclusion costs nothing (return-address slots are synthetic in a
# generated build by construction; bytes below SP are dead).
#
# No site list: ram.probe installs at fixed addresses, so there is nothing to
# derive or to go stale. The "Loaded N keys" assertion still applies -- without
# input, memory would agree vacuously the same way the traces would.
check_memory() {
  local label=$1 prog=$2
  local tag="$label-$(basename "$keys" .pkeys)"
  local interp="/tmp/pkeys-ram-interp-$tag.txt" gen="/tmp/pkeys-ram-$tag.txt"

  [ -x "$prog" ] || { echo "Error: not found: $prog" >&2; exit 1; }

  "$a2run" --preload "$b33" --key-file="$keys" --probe="$here/testdata/ram.probe" \
    --probe-out="$interp" --frames="$frames" \
    > /dev/null 2>"/tmp/pkeys-ram-interp-$tag.err"
  "$prog" --key-file="$keys" --probe="$here/testdata/ram.probe" \
    --probe-out="$gen" --frames="$frames" \
    > /dev/null 2>"/tmp/pkeys-ram-$tag.err"

  local side
  for side in "interp-$tag" "$tag"; do
    if grep -q "never fired" "/tmp/pkeys-ram-$side.err"; then
      echo "FAIL [ram/$side]: a probe never fired" >&2
      exit 1
    fi
    if ! grep -qx "Loaded $expected_keys keys" "/tmp/pkeys-ram-$side.err"; then
      echo "FAIL [ram/$side]: expected 'Loaded $expected_keys keys' on stderr" >&2
      exit 1
    fi
  done

  if ! diff -q "$interp" "$gen" > /dev/null; then
    echo "FAIL [ram/$label]: the engines disagree on memory contents" >&2
    diff "$interp" "$gen" | head -10 >&2
    exit 1
  fi

  echo "[ram/$label] PASS: memory identical at $(wc -l < "$interp") in-game samples"
}

# Screen equivalence at program-defined instants. See screen.probe: this is the
# check designed to survive the conversion of game.c to real C, so it is run
# now, while the stronger checks can still confirm it is not lying.
check_screen() {
  local label=$1 prog=$2
  local tag="$label-$(basename "$keys" .pkeys)"
  local interp="/tmp/pkeys-screen-interp-$tag.txt" gen="/tmp/pkeys-screen-$tag.txt"

  "$a2run" --preload "$b33" --key-file="$keys" --probe="$here/testdata/screen.probe" \
    --probe-out="$interp" --frames="$frames" > /dev/null 2>&1
  "$prog" --key-file="$keys" --probe="$here/testdata/screen.probe" \
    --probe-out="$gen" --frames="$frames" > /dev/null 2>&1

  if [ ! -s "$interp" ]; then
    echo "FAIL [screen/$label]: no samples" >&2
    exit 1
  fi
  if ! diff -q "$interp" "$gen" > /dev/null; then
    echo "FAIL [screen/$label]: the engines disagree on screen contents" >&2
    diff "$interp" "$gen" | head -6 >&2
    exit 1
  fi
  echo "[screen/$label] PASS: screen identical at $(wc -l < "$interp") in-game samples"
}

check_memory ref "$bin/decoded/snake-byte/snake-bytec1-run"
check_memory ext "$bin/decoded/snake-byte/snake-bytec1-ext-run"
check_screen ref "$bin/decoded/snake-byte/snake-bytec1-run"
check_screen ext "$bin/decoded/snake-byte/snake-bytec1-ext-run"
done

# ---------------------------------------------------------------------------
# The `easy` scenario.
#
# Both recordings above stop on level 1, whose display list is just `T $64`, so
# $7113's interpreter and the 'H' and 'V' cases that draw every later level's
# interior walls never run -- 26 of the 29 level scripts use each. This replays
# the *existing* play-hires keys against snake-byte-easy.b33, the same game with
# its per-level apple quota lowered from 16 to 2 (make-easy.sh). No new play was
# recorded: the snake keeps moving after the input runs out, which is enough to
# clear two levels.
#
# Only the extern build is compared, and against the interpreter rather than a
# reference build -- the interpreter is ground truth, which is the stronger
# control, and a second generated pair would cost ~900KB of committed C to prove
# something weaker. There is no memory check: ram.probe's hashes cover $6000-
# $BFFF, which includes the two patched bytes, so it would compare the fixture
# against itself and add nothing the trace does not already give.
b33="$here/testdata/snake-byte-easy.b33"
frames=${EASY_FRAMES:-3000}
set_scenario "$here/testdata/play-hires.pkeys"
echo "    (against snake-byte-easy.b33, $frames frames)"
check_backend trace-easy "$bin/decoded/snake-byte/snake-byte-easyc1-ext-run" \
  "$here/testdata/trace-easy.probe" "$here/testdata/blocks-easy.txt" 1171 \
  "$here/reference/snake-byte-easyc1-ext.c" "$here/reference/a2rom.c" "$here/reference/game.c" "$here/reference/game_native.c"

# The fixture's block heads are the same set as the stock extern build's, and
# the hand-written sources are literally the same files, so what this scenario
# reaches counts toward that build's coverage. Folding it in here is what lets
# the display-list blocks come off the unverified list.
check_screen easy "$bin/decoded/snake-byte/snake-byte-easyc1-ext-run"

cat "/tmp/pkeys-cover-trace-easy.txt" >> "/tmp/pkeys-cover-trace-ext.txt"

# ---------------------------------------------------------------------------
# The cold-start build, against the booting one.
#
# Every other check in this file compares a generated build against the
# interpreter, because the interpreter runs the original binary and is ground
# truth. That is not available here: a2run boots an Apple II, and the whole
# point of snake-byte is that it does not. There is no interpreter that
# starts at $3750.
#
# So the control is snake-bytec1-ext -- itself checked against the interpreter
# a few dozen lines above, which is what makes it usable as one. The two run
# different code (one boots and types CALL 14160, the other installs the state
# that would have produced and starts at $3750) and must agree block for block
# from $3750 onward.
#
# Aligning is the whole trick: drop everything in the booting build's trace
# before its *first* $3750. First, not any: $3750 is re-entered eight times by
# the relocation loop that copies the level data down to $1800.
#
# What this does NOT cover, because it is easy to overestimate: both builds
# include the same game.c, game_native.c and a2rom.c, so a bug in the game's own
# C changes both identically and passes here. That is checked above, against the
# interpreter. This pair checks only what is cold-specific -- the entry state,
# the pruning, and the key offset -- which is exactly what nothing else can see.
#
# Mutation-tested, 2026-08-23:
#   entry state not installed        -> FAIL (trace)
#   key offset 181207 -> 181200      -> FAIL (trace)
#   a reachable case deleted         -> FAIL (site count)
# Two byte-level mutations of the entry state passed and are worth knowing
# about: $0000 is scratch the game overwrites, and $0036 is reinstalled by
# game_install_cout_hook before anything prints. Most of the 2,051 bytes do not
# matter; the state is captured whole because that is cheaper and safer than
# working out which ones do.
echo "--- cold-start build vs the booting build ---"

cold_prog="$bin/decoded/snake-byte/snake-byte-run"
ext_prog="$bin/decoded/snake-byte/snake-bytec1-ext-run"
[ -x "$cold_prog" ] || { echo "Error: not found: $cold_prog" >&2; exit 1; }

# The cold build's own block-head set. It is much smaller than the ext build's
# -- the ROM is gone -- so it gets its own list rather than reusing one.
# The spelling lint above reads game.c and game_native.c -- the sources the
# booting builds share -- and so does not cover the cold file at all. Its one
# load-bearing invariant is checked here instead.
#
# GAME_CYCLES_SHARED means "some other source still emits a CYCLES for this
# address, so keep probing". site_addrs() does not grep for that spelling, so
# if no other source does emit it, the site keeps probing and silently drops
# off the block-head list. Checked on 2026-08-24 after exactly that happened:
# an adapter went away, its plain CYCLES went with it, and the native's SHARED
# spelling was suddenly the only mention of $6216.
site_addrs "$here/snake-byte.c" > "$here/testdata/blocks-cold.txt"

for a in $(grep -ohE 'GAME_CYCLES_SHARED\(0x[0-9a-f]+' "$here/snake-byte.c" \
             | sed 's/.*0x//' | sort -u); do
  if ! grep -qx "$a" "$here/testdata/blocks-cold.txt"; then
    echo "FAIL [cold]: GAME_CYCLES_SHARED(\$$a) but no other source emits it" >&2
    echo "  site_addrs() does not see that spelling, so the site keeps probing" >&2
    echo "  and leaves the block-head list. Use plain CYCLES." >&2
    exit 1
  fi
done
cold_sites=$(wc -l < "$here/testdata/blocks-cold.txt")
if [ "$cold_sites" -ne 6 ]; then
  echo "FAIL [cold]: site list has $cold_sites block heads, expected exactly 6" >&2
  echo "  Converting a routine lowers this deliberately; anything else is a regression." >&2
  exit 1
fi
echo "[cold] site list: $cold_sites block heads"

# Both builds run the *same* probe, installed at the cold build's block heads.
# The alternative -- give each its own list and filter afterwards -- was tried
# and is a trap: the lists are lowercase and the trace prints uppercase, so the
# filter silently dropped every address containing a letter and the comparison
# still looked almost right.
#
# Installing the cold list into the ext build is only sound because the cold
# heads are a subset of the ext ones; a probe at an address that is not a block
# head in the binary under test does not fire and does not say so.
if ! comm -23 <(sort "$here/testdata/blocks-cold.txt") <(sort "$here/testdata/blocks-ext.txt") \
     | grep -q .; then :; else
  echo "FAIL [cold]: cold block heads are not a subset of the ext build's" >&2
  comm -23 <(sort "$here/testdata/blocks-cold.txt") <(sort "$here/testdata/blocks-ext.txt") >&2
  exit 1
fi

# Both scenarios, because they exercise different halves of the build. `play`
# is the ordinary game. `hires` presses C at the attract screen, which is what
# reaches the hi-res text screen, the key-redefinition screen and -- the reason
# it was added -- game_cout_hook_native at $664A. Measured before adding it:
# the play scenario alone runs that routine exactly 0 times in the cold build,
# so every byte of the glyph blitter was ungated here. It runs it 205 times.
#
# play-hires-cold.pkeys is derived, not recorded: make-cold-keys.sh shifts
# play-hires.pkeys down by the coordinate of the first $3750 arrival. That
# offset is 181207, re-measured rather than trusted.
#
# Both runs get the same --frames, on purpose, rather than a smaller number
# for the cold side. The check below is cold_n >= ext_n, then truncates cold
# to ext_n before comparing -- so the cold side only ever needs to be *at
# least* as long, and over-provisioning it is free. A smaller cold budget
# would have to encode how many frames the booting build spends reaching
# $3750 before it can even start comparing, and that boot length is a fact
# about the ROM and the scenario, not a constant -- it would go stale the
# next time either changed. Equal frames sidesteps that: at equal frame
# counts the cold build necessarily produces *more* block heads than the
# booting one, since none of its frames are spent booting, so cold_n >= ext_n
# holds without anyone having to compute the offset.
#
# The budget itself is 40000, not something smaller: two of four structural
# mutations of game_cold_start's loop nest survived a 20,000-frame run of
# this comparison undetected and were only caught by the screen/RAM check
# further down, at 40,000. Measured cost at 40,000, both builds, both
# scenarios: the whole gate takes about 2m30s.
cold_compare() {
  local label=$1 ext_keys=$2 cold_keys=$3

  "$ext_prog"  --key-file="$here/$ext_keys"  --probe="$here/testdata/trace-cold.probe" \
    --probe-out=/tmp/pkeys-cold-ext.txt  --frames=40000 > /dev/null 2>&1
  "$cold_prog" --key-file="$here/$cold_keys" --probe="$here/testdata/trace-cold.probe" \
    --probe-out=/tmp/pkeys-cold-cold.txt --frames=40000 > /dev/null 2>&1

  # Align at the *first* $3750: it is re-entered eight times by the relocation
  # loop that copies the level data down to $1800.
  awk 'f { print } /^3750$/ && !f { f = 1; print }' /tmp/pkeys-cold-ext.txt \
    > /tmp/pkeys-cold-ext-al.txt

  local ext_n cold_n
  ext_n=$(wc -l < /tmp/pkeys-cold-ext-al.txt)
  cold_n=$(wc -l < /tmp/pkeys-cold-cold.txt)
  # Test the alignment directly rather than by magnitude. The old check was
  # "at least 100,000 blocks", which is a proxy that happens to hold for the
  # play scenario and does not for hires -- that one spends most of its run in
  # the redefinition screen, which is converted C and therefore barely probed,
  # and produces 95,468. What alignment actually means is that the first line
  # is the entry address.
  if [ "$(head -n 1 /tmp/pkeys-cold-ext-al.txt)" != "3750" ]; then
    echo "FAIL [cold/$label]: aligned trace does not start at \$3750 -- alignment is wrong" >&2
    exit 1
  fi
  if [ "$ext_n" -lt 10000 ]; then
    echo "FAIL [cold/$label]: only $ext_n blocks after aligning -- the run did not get far" >&2
    exit 1
  fi
  if [ "$cold_n" -lt "$ext_n" ]; then
    echo "FAIL [cold/$label]: cold produced $cold_n blocks, fewer than the $ext_n to compare against" >&2
    echo "  Raise its --frames, or it stopped early." >&2
    exit 1
  fi
  head -n "$ext_n" /tmp/pkeys-cold-cold.txt > /tmp/pkeys-cold-cold-p.txt
  if ! cmp -s /tmp/pkeys-cold-ext-al.txt /tmp/pkeys-cold-cold-p.txt; then
    echo "FAIL [cold/$label]: the cold and booting builds disagree from \$3750 onward" >&2
    diff /tmp/pkeys-cold-ext-al.txt /tmp/pkeys-cold-cold-p.txt | head -20 >&2
    exit 1
  fi
  echo "[cold/$label] PASS: $ext_n block heads match the booting build from \$3750"

  # The screen, which is the oracle meant to outlive the trace, and memory,
  # minus the stack -- see ram-cold.probe for why the stack is out. The screen
  # alone would miss a wrong byte that has not been drawn yet, and most of the
  # cleanup ahead moves values around without moving pixels.
  local what probe n
  for what in screen ram-cold; do
    probe="$here/testdata/$what.probe"
    "$ext_prog"  --key-file="$here/$ext_keys"  --probe="$probe" \
      --probe-out=/tmp/pkeys-cold-x-ext.txt  --frames=40000 > /dev/null 2>&1
    "$cold_prog" --key-file="$here/$cold_keys" --probe="$probe" \
      --probe-out=/tmp/pkeys-cold-x-cold.txt --frames=40000 > /dev/null 2>&1
    # Compare the common prefix. Neither side is reliably the shorter one:
    # cold skips the boot, so the same frame budget carries it further, and in
    # the hires scenario it produces 8,731 samples against the booting build's
    # 8,465. Taking cold's count and truncating ext to it silently compared a
    # long file against a short one and failed on length -- which read as a
    # screen difference and was not one.
    local ext_s cold_s
    ext_s=$(wc -l < /tmp/pkeys-cold-x-ext.txt)
    cold_s=$(wc -l < /tmp/pkeys-cold-x-cold.txt)
    n=$ext_s
    [ "$cold_s" -lt "$n" ] && n=$cold_s
    if [ "$n" -lt 6000 ]; then
      echo "FAIL [cold/$label]: only $n $what samples in common; a run did not get far" >&2
      exit 1
    fi
    head -n "$n" /tmp/pkeys-cold-x-ext.txt > /tmp/pkeys-cold-x-p.txt
    head -n "$n" /tmp/pkeys-cold-x-cold.txt > /tmp/pkeys-cold-x-c.txt
    if ! cmp -s /tmp/pkeys-cold-x-p.txt /tmp/pkeys-cold-x-c.txt; then
      echo "FAIL [cold/$label]: $what differs from the booting build" >&2
      diff /tmp/pkeys-cold-x-p.txt /tmp/pkeys-cold-x-c.txt | head -4 >&2
      exit 1
    fi
    echo "[cold/$label] PASS: $what identical at $n in-game samples"
  done
}

cold_compare play  testdata/play.pkeys       testdata/play-cold.pkeys
cold_compare hires testdata/play-hires.pkeys testdata/play-hires-cold.pkeys

# The speaker's toggle timeline: every $C0xx access and the cycle it happened
# on. The Apple II speaker is one bit and reading $C030 flips it, so this
# sequence *is* the waveform -- which makes it an exact check on any change to
# the game's timing, and the only oracle the sound has ever had.
#
# --probe is not optional here, and leaving it off is a trap that cost most of
# an afternoon. The replay coordinate is a counter the *probe script* defines,
# so --key-file without --probe cannot honour a .pkeys stamp: the keys arrive
# at the wrong moments, the game never leaves its self-playing demo, and every
# measurement taken that way is wrong while looking entirely plausible.
#
# play sends clicks to both $30 (the speaker) and $20 (the cassette output,
# which is where they go while the game demos itself). hires is all $20,
# because it presses C at the attract screen and edits keys rather than
# playing.
for sc in play:testdata/toggle-play.txt hires:testdata/toggle-hires.txt; do
  name=${sc%%:*}; want=${sc#*:}
  case $name in
    play) keys=testdata/play-cold.pkeys ;;
    hires) keys=testdata/play-hires-cold.pkeys ;;
  esac
  A2_TOGGLE_DUMP="$regen/toggle-$name.txt" \
    "$bin/decoded/snake-byte/snake-byte-run" --frames=1300 --no-sound \
    --key-file="$here/$keys" --probe="$here/testdata/trace-cold.probe" \
    --probe-out=/dev/null > /dev/null 2>&1
  if ! diff -q "$here/$want" "$regen/toggle-$name.txt" > /dev/null; then
    echo "FAIL [toggle/$name]: the speaker timeline changed" >&2
    diff "$here/$want" "$regen/toggle-$name.txt" | head -5 >&2
    exit 1
  fi
  echo "[toggle/$name] PASS: $(wc -l < "$here/$want" | tr -d " ") speaker accesses match"
done

# --- does the hand-owned file still compile without complaint? -------------
#
# It is the only source here nothing regenerates, so nothing else notices when
# an edit leaves a wart. Three C23-extension warnings shipped on 2026-08-26
# from the virtual-clock collapse: deleting the charge that followed a label
# left `done:` and `last_line:` with no statement after them, which C11 does
# not allow.
#
# They were invisible to the author and obvious to the user, and *that* is the
# thing this check has to survive: gcc does not diagnose either of them and
# clang does. Checking only the configured compiler would have reproduced the
# original failure exactly -- green on the Linux box, three warnings on the
# Mac. So it checks every C compiler it can find, and says which.
#
# Default warnings rather than -Wall, matching the real build. -Wall adds one
# finding here, a dead `ovf8` that predates this file, and gating on it would
# bundle an unrelated cleanup into a bug fix.
warn_cc=""
for c in "$(sed -n 's/^CMAKE_C_COMPILER:[^=]*=//p' "$bin/CMakeCache.txt" | head -n 1)" \
         "$(command -v clang || true)" "$(command -v gcc || true)"; do
  [ -n "$c" ] && [ -x "$c" ] || continue
  # the same compiler can arrive twice (cc -> gcc); compare resolved targets
  r=$(cd "$(dirname "$c")" 2> /dev/null && pwd -P)/$(basename "$c")
  case " $warn_cc " in *" $r "*) continue ;; esac
  warn_cc="$warn_cc $r"
done
if [ -z "$warn_cc" ]; then
  echo "FAIL [warn]: found no C compiler to check snake-byte.c with" >&2
  exit 1
fi
warn_names=""
for c in $warn_cc; do
  out=$("$c" -I "$here/../../include" -std=gnu11 -fsyntax-only \
          "$here/snake-byte.c" 2>&1 | grep 'snake-byte\.c.*warning' || true)
  if [ -n "$out" ]; then
    echo "FAIL [warn]: $(basename "$c") warns about snake-byte.c" >&2
    echo "$out" >&2
    exit 1
  fi
  warn_names="$warn_names $(basename "$c")"
done
echo "[warn] PASS: clean under$warn_names"

# --- can every waiting loop still be interrupted? --------------------------
#
# The one class of defect this gate is structurally blind to, and it shipped:
# on 2026-08-26 the virtual-clock collapse deleted the cycle charge inside the
# ESC pause loop and the ROM's Ctrl-S screen hold. Both then spun on $C000
# with nothing to suspend them, so the host never got a turn, so the key that
# would end the wait could never be delivered. The game hung. All 28 checks
# below stayed green, because the probe scenarios stamp their keys on a counter
# that a spinning loop keeps incrementing -- the replay is happy to deliver
# keys to a program that has stopped drawing frames.
#
# Two checks, and the static one is the real guarantee.
awk -f "$here/scripts/yield-lint.awk" "$here/snake-byte.c" || exit 1
echo "[yield] PASS: every loop that reads input can suspend"

# And a live one, because a lint can only see what it knows to look for.
# esc-pause.keys presses ESC 78 times across a run, so one of them lands while
# the game is playing whatever the timing happens to be. An unyielding pause
# makes --frames never terminate, hence the watchdog -- written with sleep and
# kill rather than timeout(1), which macOS does not ship.
#
# Honest about its limit: if a future timing change stops any ESC from landing
# in the play loop, this check quietly stops testing anything. The lint above
# is what does not degrade. Mutation-tested when written -- with the two
# advance(7) calls removed it exits 124, and with them it exits 0.
# Watched by polling rather than by a second background job: a `sleep N; kill`
# subshell outlives a check that finishes in a second, and then fires into a
# recycled pid. That killed this script during its own first run.
"$bin/decoded/snake-byte/snake-byte-run" --frames=2400 --no-sound \
  --key-file="$here/testdata/esc-pause.keys" > /dev/null 2>&1 &
hang_pid=$!
hang_waited=0
while kill -0 "$hang_pid" 2> /dev/null && [ "$hang_waited" -lt 90 ]; do
  sleep 1
  hang_waited=$((hang_waited + 1))
done
if kill -0 "$hang_pid" 2> /dev/null; then
  kill -9 "$hang_pid" 2> /dev/null
  wait "$hang_pid" 2> /dev/null
  echo "FAIL [yield/esc]: still running after ${hang_waited}s -- the game hung on ESC" >&2
  echo "  A pause loop lost its charge: it spins, the host never runs, and the" >&2
  echo "  keypress that would resume can never arrive. See yield-lint.awk." >&2
  exit 1
fi
wait "$hang_pid"; hang_rc=$?
if [ "$hang_rc" -ne 0 ]; then
  echo "FAIL [yield/esc]: the run exited $hang_rc after ESC" >&2
  echo "  A pause loop lost its charge: it spins, the host never runs, and the" >&2
  echo "  keypress that would resume can never arrive. See yield-lint.awk." >&2
  exit 1
fi
echo "[yield/esc] PASS: the game resumes from ESC"

echo "--- coverage over all scenarios ---"
coverage_report trace "$here/testdata/blocks.txt" 0
# Baseline 20, and the number is a statement about the recordings rather than
# about the decode. Every entry is a hand-written block that no scenario
# executes, so its decode rests on the binary alone with no cross-engine check
# behind it. They cluster into whole features, not scattered branches:
#
#   (was: the joystick (11), $6CC2-$6CF1, and attract mode (6), $6C7B-$6C8F --
#   neither recording plugs a joystick in, and both answer the difficulty
#   prompt, so $0302 is never set. All 17 left the site list when
#   game_read_direction converted, the same way $64D2/$6572 and $6B35 did.
#   Seventeen at once is the largest such drop so far and the clearest case
#   for reading this number carefully: nothing about that code became better
#   understood, it merely stopped being asked about.)
#   (was: the 'P' opcode (7), $7192-$71B0 -- none of the 29 level scripts
#   uses it. Off the list with the display-list interpreter's conversion.)
#   the ROM (20)        in a2rom.c, mostly paths for arguments the game never
#       passes to PLOT, HLINE and SCRN.
#   (was: pause and mute (3), $69AD $69B2 -- the ESC spin -- and $69B9, the
#   Ctrl-S toggle. Off the list with game_pause_or_toggle_sound_native's
#   conversion, and unverified still; the comment above that routine is now
#   the only record of them. This entry used to say the routine was
#   deliberately *not* converted while no recording pressed those keys. That
#   was set aside as a project decision: waiting on a recording that nobody
#   was going to make left the routine in the emulator-shaped file
#   indefinitely, and the decode is checkable against the binary either way.)
#   (was: arrow keys (3), $7623 $7627 $762B -- the redefinition screen's arrow
#   cases, which play-hires never reaches because it assigns W A D X Q E, all
#   above $A1. Off the list with game_edit_key's conversion, and unverified
#   still.)
#   (was: difficulty 2 (3), $65D9 $65F4 for the second bouncer and $70A5 for
#   its wall gap -- both recordings play at difficulty 1. Off the list with
#   game_step_bouncers_native and the display-list interpreter.)
#   (was: off the board (2), $64D2 $6572 -- those two left the site list
#   entirely when game_move_bouncer converted to real C, so they are no longer
#   counted here. Unexercised code that stops being probed stops being
#   measured, which is worth remembering: this number can fall for a good
#   reason or a bad one.)
#   (was: dead end (1), $6B35 -- gone from the list with game_move_ok's
#   conversion, like $64D2/$6572 before it. Converting unexercised code stops
#   it being counted rather than makes it verified.)
#   (was: level 30 (1), $7132 -- the 'E' opcode that wraps back to level 1.)
#   (was: unrecognised (1), $71C4 -- every byte in every script is a valid
#   opcode.)
#   (was: page crossing (1), $7021 -- game_next_byte's carry into the high
#   byte, never taken because no script straddles a page. Off the list with
#   that routine's conversion, and unverified still.)
#
# What remains is the ROM group alone, and every other group above is now a
# "(was:". That is worth reading carefully rather than as progress: with the
# game fully converted, the only hand-written blocks still *probed* are the
# ones in a2rom.c, so this number has stopped being able to say anything about
# the game at all. The comments above each converted routine took over that
# job, and they are prose -- nothing checks them.
#
# The number still exists to stop the ROM set growing quietly, and to be
# ratcheted down whenever a scenario reaches one of them -- as happened at
# 22 -> 21 when the `easy` fixture covered $6C4F. It is not a target to be
# satisfied.
coverage_report trace-ext "$here/testdata/blocks-ext.txt" 20 "$here/reference/a2rom.c" "$here/reference/game.c" \
  "$here/reference/game_native.c"
