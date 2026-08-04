#!/bin/bash
# Replay play.keys against the decompiled builds and compare per-frame video
# state hashes.
#
#   ./verify.sh            check both builds against play.frames
#   ./verify.sh --record   re-record play.frames from the reference build
#
# Two binaries are checked:
#
#   snake-bytec1      the reference build -- self-contained generated C, with
#                     the Apple II ROM decompiled alongside the game. This is
#                     what play.frames was recorded from.
#   snake-bytec1-ext  the same game with the ROM entry points in rom.externs
#                     replaced by the hand-written a2rom.c.
#
# Both matching the same trace is what proves the hand-written ROM replacements
# behave identically to the decompiled ROM, cycle counts included.
set -e

bin=${BIN:-../../cmake-build-debug}
frames=1300
here=$(cd "$(dirname "$0")" && pwd)
cd "$here"

ref="$bin/decoded/snake-byte/snake-bytec1"
ext="$bin/decoded/snake-byte/snake-bytec1-ext"

run() {
  # $1: executable, $2: output path
  "$1" --headless --key-file=play.keys --frames=$frames --hash-frames="$2" >/dev/null
}

if [ "$1" = "--record" ]; then
  # Record from the reference build only, and refuse to write unless two runs
  # agree -- a trace nobody has shown to be deterministic is worse than none.
  run "$ref" /tmp/sb-record-a.frames
  run "$ref" /tmp/sb-record-b.frames
  if ! diff -q /tmp/sb-record-a.frames /tmp/sb-record-b.frames; then
    echo "ERROR: oracle is not reproducible - two runs of the same binary differ" >&2
    diff /tmp/sb-record-a.frames /tmp/sb-record-b.frames | head -20 >&2
    exit 1
  fi
  cp /tmp/sb-record-a.frames play.frames
  echo "Recorded $(wc -l < play.frames) frames to play.frames"
  exit 0
fi

status=0
check() {
  # $1: label, $2: executable
  if [ ! -x "$2" ]; then
    echo "FAIL [$1]: $2 not built" >&2
    status=1
    return
  fi
  run "$2" "/tmp/sb-check-$1.frames"
  if diff -u play.frames "/tmp/sb-check-$1.frames" > "/tmp/sb-diff-$1.txt"; then
    echo "PASS [$1]: $(wc -l < play.frames) frames match"
  else
    echo "FAIL [$1]: first divergence:" >&2
    head -10 "/tmp/sb-diff-$1.txt" >&2
    status=1
  fi
}

check ref "$ref"
check ext "$ext"

# The hi-res text scenario. play-hires.keys presses SPACE and then C at the
# attract screen, which makes the game install its own COUT handler at $664A
# (via $6641) and print through it -- 177 calls in 390 frames.
#
# Only the ext build can run this: $664A is hand-decompiled in game.c, and the
# reference build does not contain it at all (the recorded session never
# installs the hook, so the tracer classified those bytes as data). This is
# therefore a regression test, not an independent oracle -- its authority comes
# from a one-time cross-check against a decompiler-generated $664A, built from a
# scratch run-data file with $664A added as a branch target.
#
# Capped at 390 frames: at 393 the C screen reaches $7541, which the recording
# never covered either, and both builds stop with "Unknown address $7541".
if [ -x "$ext" ]; then
  "$ext" --headless --key-file=play-hires.keys --frames=390 \
    --hash-frames=/tmp/sb-check-hires.frames >/dev/null
  if diff -u play-hires.frames /tmp/sb-check-hires.frames > /tmp/sb-diff-hires.txt; then
    echo "PASS [hires]: $(wc -l < play-hires.frames) frames match"
  else
    echo "FAIL [hires]: first divergence:" >&2
    head -10 /tmp/sb-diff-hires.txt >&2
    status=1
  fi
fi

exit $status
