#!/bin/bash
# Does an edit emit the same instructions?
#
# The oracle for a change that is supposed to be cosmetic -- a rename, a
# comment, a cast removed, a literal respelled. If the disassembly is
# unchanged, the edit cannot have changed behaviour, and no gate run is needed
# to say so.
#
#   same-code.sh <file.c> [git-rev]      default rev: HEAD
#
# It compiles <file.c> as it is now against the same file at <git-rev>, at -O2,
# and diffs the disassembly. Exit 0 means identical.
#
# Two things it does that the obvious version gets wrong:
#
#   - The old copy is compiled *beside the new one*, in a directory holding
#     symlinks to the file's siblings. Write it to /tmp instead and its
#     relative #includes do not resolve, the compiler stops before emitting
#     anything, and the empty output compares equal to nothing at all. This
#     happened here on 2026-08-27: three warnings were reported as a
#     regression on the strength of a comparison that had never compiled.
#   - Addresses and label numbers are stripped, because they shift when
#     anything above them changes size and say nothing about behaviour.
#
# What it cannot see: __LINE__ inside assert() bakes the line number into the
# text, so an edit that moves lines shows up even when it changes nothing. If
# that is what you are doing, add -DNDEBUG to CFLAGS below -- and note that it
# then also elides every assert argument, so a changed assert condition
# becomes invisible too. Deleting only casts or renaming only symbols does not
# move any line, which is why neither needs it.
#
# For a rename, the symbol names are in the text: expect a diff, and check it
# is only the names by re-running with the old name substituted in.
set -u

die() { echo "same-code.sh: $*" >&2; exit 2; }

[ $# -ge 1 ] || die "usage: same-code.sh <file.c> [git-rev]"
new=$1
rev=${2:-HEAD}
[ -f "$new" ] || die "no such file: $new"

CC=${CC:-cc}
CFLAGS=${CFLAGS:--O2 -std=gnu11}
root=$(git rev-parse --show-toplevel) || die "not in a git repository"
dir=$(cd "$(dirname "$new")" && pwd -P)
base=$(basename "$new")
rel=${dir#"$root"/}

work=$(mktemp -d) || die "mktemp failed"
trap 'rm -rf "$work"' EXIT
old_dir=$work/old
mkdir -p "$old_dir"

# every sibling, so relative includes resolve exactly as they do in the tree
for f in "$dir"/*; do
  [ -e "$f" ] || continue
  ln -s "$f" "$old_dir/$(basename "$f")"
done
rm -f "$old_dir/$base"
git -C "$root" show "$rev:$rel/$base" > "$old_dir/$base" ||
  die "$rel/$base is not in $rev"

disas() { # <source> <tag>
  local src=$1 tag=$2
  if ! $CC $CFLAGS -I "$root/include" -I "$(dirname "$src")" \
       -c "$src" -o "$work/$tag.o" 2> "$work/$tag.err"; then
    echo "same-code.sh: $tag did not compile -- the comparison is void" >&2
    if [ "$tag" = old ]; then
      echo "  The old copy is built against the *current* siblings, so this is" >&2
      echo "  expected if a header changed too. This script compares one file" >&2
      echo "  against its own past, not the tree against its past." >&2
    fi
    head -20 "$work/$tag.err" >&2
    exit 2
  fi
  # tail -n +3 drops objdump's banner, which names the object file and would
  # otherwise differ on every run for a reason that is not the code.
  objdump -d --no-show-raw-insn "$work/$tag.o" | tail -n +3 |
    sed -e 's/^ *[0-9a-f]*:\t//' -e 's/0x[0-9a-f]*//g' -e 's/\.L[0-9]*/.L/g' \
    > "$work/$tag.txt"
}

disas "$old_dir/$base" old
disas "$dir/$base" new

if diff -q "$work/old.txt" "$work/new.txt" > /dev/null; then
  echo "same-code: $rel/$base emits identical instructions to $rev"
  exit 0
fi
echo "same-code: $rel/$base DIFFERS from $rev" >&2
diff "$work/old.txt" "$work/new.txt" | head -40 >&2
exit 1
