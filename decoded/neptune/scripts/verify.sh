#!/bin/bash
# Replay recorded input against the decompiled build and compare per-frame
# video state hashes against a golden trace.
#
#   ./verify.sh [build-dir]              check the build against the traces
#   ./verify.sh [build-dir] --record     re-record the traces and the key file
#   ./verify.sh [build-dir] --coverage   report what the scenarios actually reach
#   ./verify.sh [build-dir] --hits A...  hit counts for specific addresses
#
# This is step 3 of docs/decompiling/playbook.md, the step the rest of the
# method is denominated in: every later change is trusted because this still
# passes.
#
# WHAT IS COMPARED
#
# --hash-frames writes one line per frame: frame number, cycle count, and a
# hash of the video *state* -- the mode bits and the text and hi-res pages,
# from memory rather than from a rendered framebuffer. Memory hashing is
# strictly more sensitive than pixels, needs no graphics context, and is
# reproducible; rendering derives its blink phase from wall-clock time and is
# not. The cycle count on each line means the trace is a timing check too, so
# any restructuring that changes a CYCLES() total is caught here rather than
# discovered later.
#
# TWO SCENARIOS
#
#   attract   no input at all. The title screen, its 256-round key wait, the
#             timeout into the attract demo, and the demo playing itself. This
#             is the only scenario that needs no key file, so it is the one
#             that still works if the key coordinate ever breaks.
#   play      the recorded session from testdata/play2.keys: space to start a
#             game, then movement, fire and bombs. Reaches SC:00180.
#
# WHY THE KEY FILE IS COMMITTED RATHER THAN DERIVED
#
# testdata/play2.keys is stamped against a disk boot, so it has to be rebased
# for a build that starts at the game's entry -- see scripts/collect.sh for why
# a constant suffices. The generated build reaches $3523 at cycle 62, having no
# ROM boot to perform, where the interpreter takes 141,185.
#
# The rebased file is committed rather than recomputed on every run, and that
# is deliberate. If verify.sh measured the offset each time, a build whose
# entry timing had drifted would get keys shifted by exactly the same amount,
# the replay would line up again, and the oracle would pass while hiding the
# very change it exists to catch. Fixed input is the point.
#
# WHAT THIS ORACLE CANNOT SEE -- see also 3d in the playbook
#
#   - Anything the recording does not reach. It is one game on level 1: no
#     death-and-restart, no second level, no high-score entry. Coverage is
#     measured in README.md rather than assumed.
#   - Anything outside video memory and the cycle count. RAM the game uses for
#     state is only visible where it eventually reaches the screen.
#   - Cross-engine differences. Both scenarios compare the generated build
#     against itself; cycle-stamped keys do not survive a change of engine
#     (measured: the interpreter and the generated build land one title-loop
#     iteration apart), so this cannot be used to compare against a2run.

set -eu

record=
coverage=
hits=
want=
bin=${BIN:-}
while [ $# -gt 0 ]; do
  case $1 in
    --record) record=1 ;;
    --coverage) coverage=1 ;;
    --hits) coverage=1; hits=1 ;;
    # An unrecognised argument is an error rather than nothing: the sibling
    # script in decoded/snake-byte once ignored its arguments and silently
    # tested a stale build directory, producing four false PASSes.
    -*) echo "Usage: $0 [build-dir] [--record|--coverage|--hits ADDR...]" >&2; exit 1 ;;
    *) if [ -n "$hits" ]; then want="$want $1"; else bin=$1; fi ;;
  esac
  shift
done

here=$(cd "$(dirname "$0")/.." && pwd)
if [ -n "$bin" ]; then
  bin=$(cd "$bin" 2>/dev/null && pwd) || { echo "Error: no such build dir" >&2; exit 1; }
else
  bin=$here/../../cmake-build-debug
fi
cd "$here"

# The console builds. They link no graphics or audio, which is what
# verification wants: there is nothing here for a display to affect.
#
#   neptunec1      the reference build -- self-contained generated C, with the
#                  Apple II ROM decompiled alongside the game. Both traces are
#                  recorded from this one.
#   neptunec1-ext  the same game with $FCA8 (MON_WAIT) replaced by the
#                  hand-written rom.c.
#   neptune        the artifact: hand-maintained, seeded from neptunec1-ext and
#                  converted from there. This is the build the directory exists
#                  to produce, and gating it against traces recorded from the
#                  generated one is what makes editing it safe.
#
# Both matching the same trace is what proves the hand-written replacement
# behaves identically to the decompiled original, cycle counts included -- and
# cycle counts are on every line of the trace precisely so that a delay loop
# rewritten by hand cannot pass while being subtly wrong.
ref="$bin/decoded/neptune/neptunec1-run"
ext="$bin/decoded/neptune/neptunec1-ext-run"
art="$bin/decoded/neptune/neptune-run"
a2run="$bin/tools/a2run/a2run"
entry=3523

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

# name:keys:frames:trace     ("-" for no key file)
scenarios=(
  "attract:-:1200:testdata/attract.frames"
  "play:testdata/play2-gen.keys:1929:testdata/play.frames"
)

run() {
  # $1: executable, $2: key file or "-", $3: frames, $4: output path
  if [ "$2" = "-" ]; then
    "$1" --frames="$3" --hash-frames="$4" >/dev/null 2>&1
  else
    "$1" --key-file="$2" --frames="$3" --hash-frames="$4" >/dev/null 2>&1
  fi
}

if [ ! -x "$ref" ]; then
  echo "FAIL: $ref not built" >&2
  exit 1
fi

if [ -n "$record" ]; then
  # The rebased key file first, because the traces are recorded with it.
  "$ref" --frames=200 --snapshot-at=$entry --snapshot-out="$tmp/gen.bin" >/dev/null 2>&1
  gen_entry=$(sed -n 's/^cycles //p' "$tmp/gen.bin.regs")
  [ -n "$gen_entry" ] || { echo "ERROR: the build never reached \$$entry" >&2; exit 1; }

  "$a2run" --frames=3000 --disk1="$here/../../dsk/Neptune.dsk" \
    --snapshot-at=$entry --snapshot-out="$tmp/disk.bin" >/dev/null 2>&1
  disk_entry=$(sed -n 's/^cycles //p' "$tmp/disk.bin.regs")
  [ -n "$disk_entry" ] || { echo "ERROR: the disk boot never reached \$$entry" >&2; exit 1; }

  offset=$((disk_entry - gen_entry))
  {
    echo "# Generated by decoded/neptune/scripts/verify.sh --record. Do not edit."
    echo "# play2.keys rebased for the generated build, which reaches \$$entry at"
    echo "# cycle $gen_entry where the disk boot takes $disk_entry: every stamp"
    echo "# shifted down by $offset."
    awk -v base="$disk_entry" -v off="$offset" '
      /^#/ { next }
      $1 < base { next }
      { printf "%d %s\n", $1 - off, $2 }
    ' testdata/play2.keys
  } > testdata/play2-gen.keys
  echo "Recorded testdata/play2-gen.keys (offset $offset)"

  # Two runs must agree before a trace is written. An oracle nobody has shown
  # to be deterministic is worse than none: it produces failures that look like
  # real bugs.
  for s in "${scenarios[@]}"; do
    IFS=: read -r name keys frames trace <<<"$s"
    run "$ref" "$keys" "$frames" "$tmp/a.frames"
    run "$ref" "$keys" "$frames" "$tmp/b.frames"
    if ! diff -q "$tmp/a.frames" "$tmp/b.frames" >/dev/null; then
      echo "ERROR [$name]: oracle is not reproducible -- two runs of the same binary differ" >&2
      diff "$tmp/a.frames" "$tmp/b.frames" | head -20 >&2
      exit 1
    fi
    cp "$tmp/a.frames" "$trace"
    echo "Recorded $(wc -l < "$trace") frames to $trace"
  done
  exit 0
fi

if [ -n "$coverage" ]; then
  # Per-site coverage, because a frame-hash trace flatters you otherwise: it
  # says the frames matched, not that the code ran. Every block head in the
  # generated C gets a probe and we count which ones fire.
  #
  # The sites are taken from the CYCLES() calls, because CYCLES is what
  # dispatches a probe -- a block that does not call it cannot be observed no
  # matter what is installed. This was got wrong once, by listing the dispatch
  # switch's `case N: // $XXXX` labels instead: that set excludes every block
  # inside the 84 functions, so addresses in them reported zero hits while
  # being executed, and "never runs" was concluded about code that runs.
  # Addresses above $FFFF are dropped -- the synthesised interrupt entry is
  # reported as $10100 and the probe loader rejects it.
  grep -oE 'CYCLES\(0x[0-9a-f]{1,5},' as-generated/neptunec1.c |
    grep -oE '0x[0-9a-f]+' |
    sed 's/^0x//' |
    awk 'function hex(s,   i, d, v) {
           v = 0; s = toupper(s)
           for (i = 1; i <= length(s); i++) {
             d = index("0123456789ABCDEF", substr(s, i, 1)) - 1
             if (d >= 0) v = v * 16 + d
           }
           return v
         }
         { v = hex($0); if (v <= 65535) printf "%04X\n", v }' |
    sort -u > "$tmp/blocks.txt"
  total=$(wc -l < "$tmp/blocks.txt")
  printf 'probe hit(site = pc) { printf("%%04X\\n", site) }\ninstall hit at @"blocks.txt"\n' \
    > "$tmp/cov.probe"

  : > "$tmp/all.txt"
  for s in "${scenarios[@]}"; do
    IFS=: read -r name keys frames trace <<<"$s"
    if [ "$keys" = "-" ]; then
      "$ref" --frames="$frames" --probe="$tmp/cov.probe" \
        --probe-out="$tmp/cov-$name.txt" >/dev/null 2>&1
    else
      "$ref" --key-file="$keys" --frames="$frames" --probe="$tmp/cov.probe" \
        --probe-out="$tmp/cov-$name.txt" >/dev/null 2>&1
    fi
    n=$(sort -u "$tmp/cov-$name.txt" | wc -l)
    printf "%-10s %4d of %d blocks (%d%%)\n" "$name" "$n" "$total" $((100 * n / total))
    cat "$tmp/cov-$name.txt" >> "$tmp/all.txt"
  done
  u=$(sort -u "$tmp/all.txt" | wc -l)
  if [ -z "$hits" ]; then
    printf "%-10s %4d of %d blocks (%d%%)\n" "union" "$u" "$total" $((100 * u / total))
    exit 0
  fi

  # Per-address hit counts, with the instrumentability of each stated rather
  # than implied. This exists because a probe that was never installed reports
  # zero hits and reads exactly like code that never ran -- which produced
  # three wrong conclusions in this directory before the guard was written.
  # A CYCLES_EDGE site is the usual reason: it charges a taken-branch penalty
  # and deliberately does not dispatch a probe, so it can never be observed.
  status=0
  for a in $want; do
    a=$(echo "$a" | tr 'a-f' 'A-F' | sed 's/^\$//')
    if ! grep -qix "$a" "$tmp/blocks.txt"; then
      why="not a CYCLES site"
      if grep -qi "CYCLES_EDGE(0x$(echo "$a" | tr 'A-F' 'a-f')," as-generated/neptunec1.c; then
        why="CYCLES_EDGE only -- never dispatches a probe"
      fi
      printf "  \$%-6s NOT INSTRUMENTED (%s) -- no conclusion available\n" "$a" "$why"
      status=1
      continue
    fi
    pa=$(grep -cix "$a" "$tmp/cov-attract.txt" || true)
    pp=$(grep -cix "$a" "$tmp/cov-play.txt" || true)
    printf "  \$%-6s attract=%-6s play=%s\n" "$a" "$pa" "$pp"
  done
  exit $status
fi

status=0
check() {
  # $1 scenario, $2 build label, $3 executable, $4 keys, $5 frames, $6 trace
  if [ ! -x "$3" ]; then
    echo "FAIL [$1/$2]: $3 not built" >&2
    status=1
    return
  fi
  run "$3" "$4" "$5" "$tmp/check-$1-$2.frames"
  if diff -u "$6" "$tmp/check-$1-$2.frames" > "$tmp/diff-$1-$2.txt"; then
    echo "PASS [$1/$2]: $(wc -l < "$6") frames match"
  else
    echo "FAIL [$1/$2]: first divergence:" >&2
    head -10 "$tmp/diff-$1-$2.txt" >&2
    status=1
  fi
}

for s in "${scenarios[@]}"; do
  IFS=: read -r name keys frames trace <<<"$s"
  if [ ! -r "$trace" ]; then
    echo "FAIL [$name]: no golden trace at $trace -- run --record" >&2
    status=1
    continue
  fi
  check "$name" ref "$ref" "$keys" "$frames" "$trace"
  check "$name" ext "$ext" "$keys" "$frames" "$trace"
  check "$name" art "$art" "$keys" "$frames" "$trace"
done

exit $status
