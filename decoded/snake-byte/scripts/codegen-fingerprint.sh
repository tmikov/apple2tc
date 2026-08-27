#!/bin/bash
#
# Print a hash of the code the compiler emits, for proving that an edit changed
# nothing. Run it before and after; identical output means identical
# instructions.
#
# Three things are filtered out because they move for reasons that are not code
# changes, and each one looks exactly like a real difference:
#
#   -g0        debug info carries line numbers, so any edit moves it
#   -DNDEBUG   with NDEBUG, assert() calls are completely elided, not just line
#              numbers. This file has one assert() whose condition changes would
#              be invisible to this tool; such changes must be verified by code
#              review instead of fingerprinting
#   .L#        gcc renumbers its internal labels whenever a function is added
#              to or removed from the translation unit
#
set -e
cd "$(dirname "$0")/.."
cc -I ../../include -std=gnu11 -O2 -g0 -DNDEBUG -S -o - "${1:-snake-byte.c}" |
  grep -v '^[[:space:]]*\.file\|^[[:space:]]*\.ident\|^[[:space:]]*#' |
  sed 's/\.L[A-Z]*[0-9][0-9]*/.L#/g' | sha256sum | cut -c1-16
