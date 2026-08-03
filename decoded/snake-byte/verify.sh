#!/bin/bash
# Replay play.keys against the decompiled builds and compare per-frame video
# state hashes. With one argument, records a new golden trace. With none,
# checks the existing builds against play.frames.
set -e

bin=${BIN:-../../cmake-build-debug}
frames=1300
here=$(cd "$(dirname "$0")" && pwd)
cd "$here"

run() {
  # $1: executable, $2: output path
  "$1" --headless --key-file=play.keys --frames=$frames --hash-frames="$2" >/dev/null
}

if [ "$1" = "--record" ]; then
  run "$bin/decoded/snake-byte/snake-bytec1" /tmp/sb-record-a.frames
  run "$bin/decoded/snake-byte/snake-bytec1" /tmp/sb-record-b.frames
  if ! diff -q /tmp/sb-record-a.frames /tmp/sb-record-b.frames; then
    echo "ERROR: oracle is not reproducible - two runs of the same binary differ" >&2
    diff /tmp/sb-record-a.frames /tmp/sb-record-b.frames | head -20 >&2
    exit 1
  fi
  cp /tmp/sb-record-a.frames play.frames
  echo "Recorded $(wc -l < play.frames) frames to play.frames"
  exit 0
fi

run "$bin/decoded/snake-byte/snake-bytec1" /tmp/sb-check.frames
if diff -u play.frames /tmp/sb-check.frames > /tmp/sb-diff.txt; then
  echo "PASS: $(wc -l < play.frames) frames match"
else
  echo "FAIL: first divergence:" >&2
  head -10 /tmp/sb-diff.txt >&2
  exit 1
fi
