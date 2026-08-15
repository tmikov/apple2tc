#!/bin/bash
# Replay recorded key presses against the decompiled builds and compare
# per-frame video state hashes.
#
#   ./verify.sh [build-dir]            check both builds against the traces
#   ./verify.sh [build-dir] --record   re-record both from the reference build
#
# The build directory may also come from $BIN. It used to come *only* from
# there, and this script ignored its arguments -- so `./verify.sh ../../cmake-
# build-release`, which is exactly how the sibling probe-acceptance.sh is
# invoked, silently tested whatever was in cmake-build-debug instead. That cost
# a run of four false PASSes against a stale binary, so unrecognised arguments
# are now an error rather than nothing.
#
# Two binaries are checked:
#
#   snake-bytec1      the reference build -- self-contained generated C, with
#                     the Apple II ROM decompiled alongside the game. Both
#                     traces are recorded from this.
#   snake-bytec1-ext  the same game with the ROM entry points in rom.externs
#                     replaced by the hand-written a2rom.c, and $664A by the
#                     hand-written game.c.
#
# Both matching the same trace is what proves the hand-written replacements
# behave identically to the decompiled originals, cycle counts included.
#
# Two scenarios are replayed, both against both builds:
#
#   play        the ordinary game: attract screen, then a round of play.
#   hires       press C at the attract screen to reach the key-redefinition
#               screen, then reassign all six direction keys to W A D X Q E.
#               This is the scenario that exercises the game's own COUT
#               handler at $664A (a hi-res glyph blitter, installed by $6641)
#               and the screen body at $7541.
#
# The hires scenario is a real cross-check, not a regression test: $664A and
# $7541 are reached only through control flow the recorded session never took,
# so both are supplied by --code-at (see code-at.txt). The reference build gets
# a decompiler-generated $664A; in the extern build $FDED is replaced by the
# hand-written rom_cout, which deletes the generated $664A as unreachable and
# dispatches to game_cout_hook in game.c instead. So the two builds run
# genuinely different code for it and are compared frame for frame.
set -e

record=
bin=$BIN
while [ $# -gt 0 ]; do
  case $1 in
    --record) record=1 ;;
    -*) echo "Usage: $0 [build-dir] [--record]" >&2; exit 1 ;;
    *) bin=$1 ;;
  esac
  shift
done
here=$(cd "$(dirname "$0")" && pwd)
# Resolved against the caller's directory while that is still where we are; the
# default is relative to the script instead.
if [ -n "$bin" ]; then
  bin=$(cd "$bin" 2>/dev/null && pwd) || { echo "Error: no such build dir" >&2; exit 1; }
else
  bin=$here/../../cmake-build-debug
fi
cd "$here"

# The console builds. Each decompiled program is built twice -- the plain name
# is the GUI version, -run is a console one -- because on Windows console versus
# GUI is a link-time subsystem property that no runtime flag can bridge. The
# console binaries link no graphics or audio at all, which is what verification
# wants: there is nothing here for a display to affect.
ref="$bin/decoded/snake-byte/snake-bytec1-run"
ext="$bin/decoded/snake-byte/snake-bytec1-ext-run"

# name:keys:frames:trace
scenarios=(
  "play:play.keys:1300:play.frames"
  "hires:play-hires.keys:1300:play-hires.frames"
)

run() {
  # $1: executable, $2: key file, $3: frames, $4: output path
  "$1" --key-file="$2" --frames="$3" --hash-frames="$4" >/dev/null
}

if [ -n "$record" ]; then
  # Record from the reference build only, and refuse to write unless two runs
  # agree -- a trace nobody has shown to be deterministic is worse than none.
  for s in "${scenarios[@]}"; do
    IFS=: read -r name keys frames trace <<<"$s"
    run "$ref" "$keys" "$frames" /tmp/sb-record-a.frames
    run "$ref" "$keys" "$frames" /tmp/sb-record-b.frames
    if ! diff -q /tmp/sb-record-a.frames /tmp/sb-record-b.frames >/dev/null; then
      echo "ERROR [$name]: oracle is not reproducible - two runs of the same binary differ" >&2
      diff /tmp/sb-record-a.frames /tmp/sb-record-b.frames | head -20 >&2
      exit 1
    fi
    cp /tmp/sb-record-a.frames "$trace"
    echo "Recorded $(wc -l < "$trace") frames to $trace"
  done
  exit 0
fi

status=0
check() {
  # $1: scenario name, $2: build label, $3: executable, $4: keys, $5: frames, $6: trace
  if [ ! -x "$3" ]; then
    echo "FAIL [$1/$2]: $3 not built" >&2
    status=1
    return
  fi
  run "$3" "$4" "$5" "/tmp/sb-check-$1-$2.frames"
  if diff -u "$6" "/tmp/sb-check-$1-$2.frames" > "/tmp/sb-diff-$1-$2.txt"; then
    echo "PASS [$1/$2]: $(wc -l < "$6") frames match"
  else
    echo "FAIL [$1/$2]: first divergence:" >&2
    head -10 "/tmp/sb-diff-$1-$2.txt" >&2
    status=1
  fi
}

for s in "${scenarios[@]}"; do
  IFS=: read -r name keys frames trace <<<"$s"
  check "$name" ref "$ref" "$keys" "$frames" "$trace"
  check "$name" ext "$ext" "$keys" "$frames" "$trace"
done

exit $status
