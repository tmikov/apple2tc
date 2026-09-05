#!/bin/bash

set -e

if [ -z "$1" ]; then
  echo "Usage: $0 <build-dir>" >&2
  exit 1
fi

bin=$1

if [ ! -d "$bin" ]; then
  echo "Error: build directory '$bin' not found" >&2
  exit 1
fi

a6502=$bin/tools/a6502/a6502
apple2tc=$bin/tools/apple2tc/apple2tc
id=$bin/tools/id/id
a2run=$bin/tools/a2run/a2run
a2emu=$bin/tools/a2emu/a2emu
a2mcp=$bin/tools/a2mcp/a2mcp

for tool in "$a6502" "$apple2tc" "$id" "$a2run" "$a2emu" "$a2mcp"; do
  if [ ! -x "$tool" ]; then
    echo "Error: required binary not found: $tool" >&2
    exit 1
  fi
done

$a6502 trees.s trees.b33 && $apple2tc trees.b33 -O3 --ir > trees-test.ir
diff -q trees.ir trees-test.ir
rm trees-test.ir trees.b33

$a6502 trees1.s trees1.b33 && $apple2tc trees1.b33 -O3 --ir > trees1-test.ir
diff -q trees1.ir trees1-test.ir
rm trees1-test.ir trees1.b33

$a6502 subs.s subs.b33 && $apple2tc --run-data=dclear.json -O3 --ir subs.b33 > subs-test.ir
diff -q subs.ir subs-test.ir
rm subs-test.ir subs.b33

$a6502 ands.s ands.b33 && $apple2tc -O3 --ir ands.b33 > ands-test.ir
diff -q ands.ir ands-test.ir
rm ands-test.ir ands.b33

$a6502 func.s func.b33 && $apple2tc -O3 --ir func.b33 > func-test.ir
diff -q func.ir func-test.ir
rm func-test.ir func.b33

$a6502 phapla.s phapla.b33 && $apple2tc -O3 --ir phapla.b33 > phapla-test.ir
diff -q phapla.ir phapla-test.ir
rm phapla-test.ir phapla.b33

$a6502 stackmerge.s stackmerge.b33 && $apple2tc -O3 --ir stackmerge.b33 > stackmerge-test.ir
diff -q stackmerge.ir stackmerge-test.ir
rm stackmerge-test.ir stackmerge.b33

# Mutually recursive routines: the call graph has a cycle, so no callee-first
# order exists for CPURegLiveness to rely on.
$a6502 callcycle.s callcycle.b33 && $apple2tc -O3 --ir callcycle.b33 > callcycle-test.ir
diff -q callcycle.ir callcycle-test.ir
rm callcycle-test.ir callcycle.b33

# --code-at: hand-asserted dynamic branch edges. Two baselines, so this pins
# down what the option changes rather than merely that it runs -- without the
# edges neither continuation in codeat.s is reachable at all.
$a6502 codeat.s codeat.b33 && $apple2tc codeat.b33 --code-at=codeat.txt -O3 --ir > codeat-test.ir
diff -q codeat.ir codeat-test.ir
$apple2tc codeat.b33 -O3 --ir > codeat-noedges-test.ir
diff -q codeat-noedges.ir codeat-noedges-test.ir
rm codeat-test.ir codeat-noedges-test.ir

# --inline-str: routines taking a NUL-terminated string after the JSR. Two
# baselines again, and for the same reason -- without the option the tracer
# follows the fall-through into the text, and $C8 $C9 $00 decodes as
# INY / CMP #$00. That is a wrong decompilation the disassembler cannot notice,
# so the baseline pair is what pins the behaviour rather than an absence of
# warnings.
$a6502 inlinestr.s inlinestr.b33 && $apple2tc inlinestr.b33 -O3 --ir --ret-addr \
  --extern-routines=inlinestr.externs --inline-str=inlinestr.txt > inlinestr-test.ir
diff -q inlinestr.ir inlinestr-test.ir
$apple2tc inlinestr.b33 -O3 --ir --ret-addr --extern-routines=inlinestr.externs \
  > inlinestr-noinline-test.ir
diff -q inlinestr-noinline.ir inlinestr-noinline-test.ir
rm inlinestr-test.ir inlinestr-noinline-test.ir

# A declared routine that is still *generated* would pop a return address the
# option has just moved past the string, and print from whatever follows --
# with nothing in the output looking wrong. Both ways of arriving there are
# refused, and both refusals are watched failing here.
expect_inline_reject() {
  # $1: what is wrong with it, $2: expected substring, $3...: extra arguments
  local desc=$1 want=$2
  shift 2
  if $apple2tc inlinestr.b33 -O3 --ir --inline-str=inlinestr.txt "$@" \
      >/dev/null 2>inlinestr-bad.err; then
    echo "FAIL: --inline-str accepted $desc" >&2
    exit 1
  fi
  if ! grep -q "$want" inlinestr-bad.err; then
    echo "FAIL: --inline-str rejected $desc, but not with '$want':" >&2
    cat inlinestr-bad.err >&2
    exit 1
  fi
  rm -f inlinestr-bad.err
}

expect_inline_reject "no --extern-routines at all" "requires --extern-routines"
printf '# empty\n' > inlinestr-bad.externs
expect_inline_reject "a routine missing from the extern list" "is not in" \
  --extern-routines=inlinestr-bad.externs
rm -f inlinestr-bad.externs inlinestr.b33

# --alt-exit: routines that return by discarding their return address and
# jumping into the caller. Two baselines, since what the option does is turn a
# JSR into a CallAlt and pull the routine out of the caller's switch -- without
# it the routine is rejected for stack underflow and stays inlined.
$a6502 altexit.s altexit.b33 && $apple2tc altexit.b33 -O3 --ir --ret-addr --alt-exit \
  > altexit-test.ir
diff -q altexit.ir altexit-test.ir
$apple2tc altexit.b33 -O3 --ir --ret-addr > altexit-noalt-test.ir
diff -q altexit-noalt.ir altexit-noalt-test.ir
rm altexit-test.ir altexit-noalt-test.ir altexit.b33

# --prune-returns: an RTS carries every address the recording saw it return to,
# and externalizing a routine erases the JSR that pushed one of them. The edge
# survives regardless and keeps its target -- and everything reachable from it --
# alive. Two baselines: without the flag `sub` survives externalization entirely,
# because `other`'s recorded return to `inner` still reaches into it.
$a6502 pruneret.s pruneret.b33 >/dev/null
$apple2tc pruneret.b33 --run-data=pruneret.json -O3 --ir \
  --extern-routines=pruneret.externs --prune-returns > pruneret-test.ir
diff -q pruneret.ir pruneret-test.ir
$apple2tc pruneret.b33 --run-data=pruneret.json -O3 --ir \
  --extern-routines=pruneret.externs > pruneret-keep-test.ir
diff -q pruneret-keep.ir pruneret-keep-test.ir
# The point of the flag, asserted rather than left to the diff: the block only
# that erased JSR could return to is gone, and without the flag it is not.
if grep -q '0325' pruneret-test.ir; then
  echo "FAIL: --prune-returns kept a return edge nothing can produce" >&2
  exit 1
fi
if ! grep -q '0325' pruneret-keep-test.ir; then
  echo "FAIL: the pruneret fixture proves nothing -- \$0325 is gone either way" >&2
  exit 1
fi
rm -f pruneret-test.ir pruneret-keep-test.ir pruneret.b33

# An alternate-exit routine cannot also be hand-written. The refusal has to come
# from the externalizing pass, which runs first and deletes the body: after that
# the call site looks like an ordinary call that simply returns, and the exit is
# gone with nothing to notice it.
$a6502 altexit.s altexit.b33 >/dev/null
if $apple2tc altexit.b33 -O3 --ir --ret-addr --alt-exit \
    --extern-routines=altexit.externs >/dev/null 2>altexit-ext.err; then
  echo "FAIL: an alternate-exit routine was accepted as an extern" >&2
  exit 1
fi
if ! grep -q 'returns into its caller, at \$031C \$0322' altexit-ext.err; then
  echo "FAIL: refused the extern, but not for returning into its caller:" >&2
  cat altexit-ext.err >&2
  exit 1
fi
if ! grep -q 'called by generated code at \$0313' altexit-ext.err; then
  echo "FAIL: refused the extern without naming the call site that loses the exit:" >&2
  cat altexit-ext.err >&2
  exit 1
fi

# The other half of that rule, which is what makes it a rule about call sites
# and not about the routine's shape: declared together with the caller it
# returns into, there is no generated call left to lose anything, and it is
# accepted. Without this the end state -- every routine hand-written -- would be
# unreachable, since the last conversion would always be refused.
if ! $apple2tc altexit.b33 -O3 --ir --ret-addr --alt-exit \
    --extern-routines=altexit-both.externs >/dev/null 2>altexit-both.err; then
  echo "FAIL: refused an alternate-exit routine whose only caller is also extern:" >&2
  cat altexit-both.err >&2
  exit 1
fi
rm -f altexit-ext.err altexit-both.err altexit.b33

# The rejection that matters: an "alternate exit" landing back inside its own
# routine. Accepting it would make the routine swallow the target and become
# self-recursive, and nothing about the generated code would look wrong -- so
# this is asserted on the reason, not just on the absence of a CallAlt.
$a6502 altexit-bad.s altexit-bad.b33 >/dev/null
$apple2tc altexit-bad.b33 -O3 --ir --ret-addr --alt-exit -v2 \
  >altexit-bad.ir 2>altexit-bad.err
if ! grep -q 'alternate exit to \$0335 is inside the routine' altexit-bad.err; then
  echo "FAIL: --alt-exit did not reject an exit into its own routine:" >&2
  cat altexit-bad.err >&2
  exit 1
fi
# The second routine in the fixture unwinds and then does more work before
# jumping. There is nowhere to put that work once the routine is cut in two, so
# it must be refused as well.
if ! grep -q 'block \$0356 stack level underflow' altexit-bad.err; then
  echo "FAIL: --alt-exit did not reject work between the pops and the jump:" >&2
  cat altexit-bad.err >&2
  exit 1
fi
# The third is JSR'd from one place and jumped into from another. Every entry
# path other than a plain JSR is converted into a block that calls the routine
# and dispatches on the return address, and such a block cannot take an
# alternate exit -- it would be dropped without a trace.
if ! grep -q 'alternate exits, but not entered by a JSR alone' altexit-bad.err; then
  echo "FAIL: --alt-exit accepted a routine that is also jumped into:" >&2
  cat altexit-bad.err >&2
  exit 1
fi
# The fourth is the caller's side: outer4 reaches unwound4 only through sub4's
# alternate exit, and unwound4 reloads the stack pointer. Only the rescan in
# run() ever walks it -- on the first pass sub4's exits are not known yet, so
# outer4 looks clean and gets extracted with a block nothing checked.
if ! grep -q 'StoreSP block \$0397' altexit-bad.err; then
  echo "FAIL: --alt-exit did not check a block reached through an alternate exit:" >&2
  cat altexit-bad.err >&2
  exit 1
fi
# sub4 itself is fine and must still be extracted, otherwise the case above is
# passing for the wrong reason.
if ! grep -q '^function func_03a0' altexit-bad.ir; then
  echo "FAIL: sub4 was not extracted, so the rescan check proves nothing" >&2
  exit 1
fi
for rejected in func_0330 func_0350 func_0370 func_0390; do
  if grep -q "^function $rejected" altexit-bad.ir; then
    echo "FAIL: --alt-exit rejected $rejected but extracted it anyway" >&2
    exit 1
  fi
done
rm -f altexit-bad.ir altexit-bad.err altexit-bad.b33

# Each --code-at rejection, asserted to actually reject. A check nobody has
# watched fail is not a check.
expect_reject() {
  # $1: what is wrong with it, $2: contents of the --code-at file
  printf '%s\n' "$2" > codeat-bad.txt
  if $apple2tc codeat.b33 --code-at=codeat-bad.txt -O3 --ir >/dev/null 2>codeat-bad.err; then
    echo "FAIL: --code-at accepted $1" >&2
    exit 1
  fi
  if ! grep -q 'FATAL' codeat-bad.err; then
    echo "FAIL: --code-at rejected $1, but without a FATAL diagnostic:" >&2
    cat codeat-bad.err >&2
    exit 1
  fi
  rm -f codeat-bad.txt codeat-bad.err
}

expect_reject "an origin that is not a branch"  "0300 0330"
expect_reject "an origin inside an instruction" "0301 0330"
expect_reject "a line with only one address"    "0306"
expect_reject "a malformed hex address"         "030g 0330"
expect_reject "an address wider than 16 bits"   "0306 10330"

# A target pointing at data cannot be rejected -- that untraced bytes are code
# is precisely what the option asserts -- but one that does not decode warns.
printf '0306 0328\n' > codeat-bad.txt
$apple2tc codeat.b33 --code-at=codeat-bad.txt -O3 --ir >/dev/null 2>codeat-bad.err
if ! grep -q "does not decode to a valid instruction" codeat-bad.err; then
  echo "FAIL: --code-at did not warn about a target pointing into filler bytes" >&2
  exit 1
fi
# ...and the good file must stay silent, or the warning is noise.
$apple2tc codeat.b33 --code-at=codeat.txt -O3 --ir >/dev/null 2>codeat-bad.err
if grep -q "code-at" codeat-bad.err; then
  echo "FAIL: --code-at warned about codeat.txt, which is correct:" >&2
  cat codeat-bad.err >&2
  exit 1
fi
rm -f codeat-bad.txt codeat-bad.err codeat.b33

# A subroutine entry that is also the return point of a call. The RTS returning
# into it used to be rejected as an invalid predecessor, which also took the
# routine calling it down through the invalid-JSR cascade.
$a6502 retpoint.s retpoint.b33 && $apple2tc retpoint.b33 --code-at=retpoint.txt -O3 --ir \
  --routines-report=retpoint-report.txt > retpoint-test.ir
diff -q retpoint.ir retpoint-test.ir
# Said directly, so a regression reads as more than an unexplained baseline diff.
if grep -q "^REJECT" retpoint-report.txt; then
  echo "FAIL: every routine in retpoint.s should be recovered, but:" >&2
  grep "^REJECT" retpoint-report.txt >&2
  exit 1
fi
rm retpoint-test.ir retpoint-report.txt retpoint.b33

# `id`'s cross-reference search, over a target reached four different ways.
# The indirect jump is deliberately *not* expected as a code hit -- it names the
# vector, not the target -- while the vector itself is what the data scan is for.
$a6502 xref.s xref.b33 && printf 'loadb33 xref.b33\nxref $310 $31F $300 $321\n' \
  | $id > xref-test.txt 2>&1
diff -q xref.expected xref-test.txt
rm xref-test.txt xref.b33

# The per-opcode cycle table, dumped in full. Both engines derive from this one
# table -- if they ever diverge, today's flat-3-versus-bytes bug returns
# silently -- so it is worth pinning all 256 entries, not a sample.
printf 'cycles\n' | $id > cycles-test.txt 2>&1
diff -q cycles.expected cycles-test.txt
rm cycles-test.txt

# The coverage report, with one region of each kind it classifies. The
# overlap warning is checked separately below, since a declared range that the
# disassembler also reached as code means one of the two claims is wrong.
$a6502 coverage.s coverage.b33 && $apple2tc coverage.b33 --known-data=coverage-data.txt \
  -O3 --ir --coverage=coverage-test.txt > /dev/null
diff -q coverage.expected coverage-test.txt
rm coverage-test.txt

# Declaring code as data has to be noticed, not silently believed.
printf '0300 0304 wrong\n' > coverage-bad.txt
$apple2tc coverage.b33 --known-data=coverage-bad.txt -O3 --ir --coverage=coverage-test.txt \
  > /dev/null
if ! grep -q "also decompiled as code" coverage-test.txt; then
  echo "FAIL: --coverage did not flag a data range that overlaps code" >&2
  exit 1
fi
rm coverage-test.txt coverage-bad.txt coverage.b33

# a2emu and a2run are the same host and the same engine, differing only in that
# one has a window. Headless they must agree exactly -- which is the regression
# test for a2emu, a GUI program that otherwise has none. Booting the ROM is
# enough; no game data is needed, which keeps this out of decoded/.
$a2run --frames=40 --hash-frames=frontend-run.txt > /dev/null
$a2emu --headless --frames=40 --hash-frames=frontend-emu.txt > /dev/null
if ! diff -q frontend-run.txt frontend-emu.txt > /dev/null; then
  echo "FAIL: a2emu and a2run disagree headless; they share everything but the window" >&2
  diff frontend-run.txt frontend-emu.txt | head -5 >&2
  exit 1
fi
rm frontend-run.txt frontend-emu.txt

# --- a2mcp -------------------------------------------------------------------
# The MCP server is driven entirely by stdin, so a transcript of requests and a
# baseline of replies is the whole test. Deterministic because headless is
# fixed-step: the same requests run the same cycles every time.
mkdir -p mcp-tmp

mcp_transcript() {
  # $1: base name under mcp/; $2...: extra a2mcp arguments
  local base=$1; shift
  if ! $a2mcp --root=. "$@" < "mcp/$base.jsonl" > "mcp-tmp/$base.txt" 2>"mcp-tmp/$base.err"; then
    echo "FAIL: a2mcp exited non-zero on mcp/$base.jsonl" >&2
    cat "mcp-tmp/$base.err" >&2
    exit 1
  fi
  if ! diff -u "mcp/$base.expected" "mcp-tmp/$base.txt"; then
    echo "FAIL: a2mcp transcript $base differs from its baseline" >&2
    exit 1
  fi
}

mcp_transcript handshake
mcp_transcript boot
mcp_transcript reboot
mcp_transcript run

# a2mcp's frame loop must be a2run's. Same 40 frames, same video state.
$a2run --frames=40 --hash-frames=mcp-tmp/run-a2run.txt > /dev/null
$a2mcp --root=. --hash-frames=mcp-tmp/run-a2mcp.txt < mcp/run.jsonl > /dev/null
if ! diff -q mcp-tmp/run-a2run.txt mcp-tmp/run-a2mcp.txt; then
  echo "FAIL: a2mcp's frame loop diverges from a2run's over the same 40 frames" >&2
  exit 1
fi

mcp_transcript stop --probe=mcp/stop.probe --probe-out=mcp-tmp/stop-report.txt
# The reply's content is a JSON object serialised into a JSON string, so its
# quotes reach the transcript backslash-escaped.
if ! grep -q 'stop_reason\\": \\"probe' mcp-tmp/stop.txt; then
  echo "FAIL: a probe's stop did not end an a2mcp run" >&2
  exit 1
fi

mcp_transcript screen-text
# The banner is plain text inside the reply, not JSON-escaped, so an
# unadorned grep finds it -- and finding it independently of the diff means a
# wrong-but-stable baseline (960 spaces, mojibake) cannot pass silently.
if ! grep -q 'APPLE \]\[' mcp-tmp/screen-text.txt; then
  echo "FAIL: a2mcp read the text screen but did not find the ROM banner on it" >&2
  exit 1
fi

$a6502 mcp/visible-page.s mcp-tmp/visible-page.b33
mcp_transcript change
# visible-page.s stays in text mode while continuously scribbling on the text
# page, which text mode does display, so a screen_change run must stop on it
# almost immediately -- well short of the 600-frame cap.
#
# Scoped to request 4's own reply line: request 3 is a plain frame-limited
# run and always reports stop_reason "limit" too, so a check against the
# whole transcript would pass no matter what request 4 said.
if ! grep '"id":4' mcp-tmp/change.txt | grep -q 'stop_reason\\": \\"screen_change'; then
  echo "FAIL: screen_change did not fire on a write to the displayed page" >&2
  exit 1
fi

$a6502 mcp/hidden-page.s mcp-tmp/hidden-page.b33
mcp_transcript hidden
# hidden-page.s is visible-page.s's sibling: identical but for which page it
# scribbles on. It stays in text mode while continuously writing to hi-res
# page 1, which text mode does not display. a2host_visible_hash() must not
# see it: the mode-aware hash is the whole point, so a program scribbling on
# a page the display is not showing must not look like a screen change, and
# the run must go the full 300 frames. Unlike `change` above, this is the
# test that actually discriminates a mode-aware hash from hash_video_state()
# -- making a2host_visible_hash() an alias for hash_video_state() turns this
# one red while leaving `change` green, since a mode-blind hash still notices
# a write to the displayed page too.
#
# Scoped to request 4's own reply line for the same reason as `change` above:
# request 3 always reports "limit" too.
if ! grep '"id":4' mcp-tmp/hidden.txt | grep -q 'stop_reason\\": \\"limit'; then
  echo "FAIL: screen_change fired on a page that is not displayed" >&2
  exit 1
fi

# realpath resolves symlinks, so a link pointing outside the root is the same
# rejection as a "../" path -- but only if the jail resolves before it opens.
ln -sf /etc/passwd mcp-tmp/escape.dsk
printf '%s\n' \
  '{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"protocolVersion":"2025-06-18","capabilities":{},"clientInfo":{"name":"t","version":"0"}}}' \
  '{"jsonrpc":"2.0","id":2,"method":"tools/call","params":{"name":"boot","arguments":{"disk1":"mcp-tmp/escape.dsk"}}}' \
  > mcp-tmp/symlink.jsonl
if ! $a2mcp --root=. < mcp-tmp/symlink.jsonl | grep -q '"isError":true'; then
  echo "FAIL: a2mcp followed a symlink out of --root" >&2
  exit 1
fi

# ...but that check, and every other check in this repo, runs the host in
# *fixed-step* mode -- because reproducibility is what they all exist for. The
# wall-clock mode a person plays on had no coverage at all, and was totally
# broken for nine days before a bug report found it: a2host_gui advanced its
# "last repaint" marker before running the frame, so every repaint measured
# zero elapsed time and ran zero cycles.
#
# pacing-test drives a2host directly with a stub engine, no window, and asserts
# the thing that was false: a repaint reporting elapsed time runs a non-zero
# cycle budget. See tests/pacing/pacing_test.c.
if ! "$bin/tests/pacing/pacing-test" > pacing-out.txt; then
  echo "FAIL: a2host pacing" >&2
  cat pacing-out.txt >&2
  exit 1
fi
rm pacing-out.txt

# --- Probes -----------------------------------------------------------------
#
# The probe compiler is tested through --probe-dump, which compiles a script,
# prints the compiled form and exits without running anything. Every case below
# is a script/baseline pair; a change in generated code shows up as a diff.

mkdir -p probe-tmp

# Compile a script and diff the result against its baseline.
probe_dump_test() {
  # $1: base name under probe/
  if ! $a2run --probe="probe/$1.probe" --probe-dump > "probe-tmp/$1.txt"; then
    echo "FAIL: --probe-dump failed on probe/$1.probe" >&2
    exit 1
  fi
  diff -q "probe/$1.expected" "probe-tmp/$1.txt"
}

probe_dump_test empty
probe_dump_test expr
probe_dump_test stmt
probe_dump_test install
probe_dump_test record

# Assert that a2run rejects something, with the specific diagnostic we expect.
# Matching the exact message rather than just "FATAL" is deliberate: an earlier
# version of these tests passed because a different check fired first.
expect_probe_reject() {
  # $1: description, $2: expected substring, $3...: a2run arguments
  local desc="$1" want="$2"
  shift 2
  if $a2run "$@" > /dev/null 2>probe-tmp/err.txt; then
    echo "FAIL: a2run accepted $desc" >&2
    exit 1
  fi
  if ! grep -q "FATAL" probe-tmp/err.txt || ! grep -q -- "$want" probe-tmp/err.txt; then
    echo "FAIL: rejected $desc, but not with the expected diagnostic '$want':" >&2
    cat probe-tmp/err.txt >&2
    exit 1
  fi
}

expect_probe_reject "a missing probe script" "cannot open probe script" \
  --probe=probe/does-not-exist.probe --probe-dump
expect_probe_reject "--probe-dump with no script" "--probe-dump requires" \
  --probe-dump
expect_probe_reject "--probe-out with no script" "--probe-out requires" \
  --probe-out=probe-tmp/out.txt

# Compile a script given inline and assert it is rejected with a specific
# diagnostic. Matching the message rather than just "FATAL" is deliberate:
# during review, two tests passed while covering nothing because a different
# check fired first.
expect_bad_script() {
  # $1: description, $2: expected substring, $3: script contents
  printf '%s\n' "$3" > probe-tmp/bad.probe
  expect_probe_reject "$1" "$2" --probe=probe-tmp/bad.probe --probe-dump
}

# Same idea, but for a site-list file rather than the script itself: two
# files are needed, so a wrapper of its own.
expect_site_reject() {
  # $1: description, $2: expected substring, $3: site-list contents
  printf '%s\n' "$3" > probe-tmp/sites.txt
  printf 'probe p() { }\ninstall p at @"sites.txt"\n' > probe-tmp/bad.probe
  expect_probe_reject "$1" "$2" --probe=probe-tmp/bad.probe --probe-dump
}

# Long strings assembled once, used by several cases below.
long_ident=$(printf 'a%.0s' $(seq 1 70))
long_str=$(printf 'a%.0s' $(seq 1 300))
parens_open=$(printf '(%.0s' $(seq 1 260))
parens_close=$(printf ')%.0s' $(seq 1 260))
blocks_open=$(printf '{%.0s' $(seq 1 260))
blocks_close=$(printf '}%.0s' $(seq 1 260))
long_comment=$(printf 'zzzzzzzzzz %.0s' $(seq 1 40))
many_counters_script=""
for i in $(seq 0 64); do many_counters_script+="counter c$i
"; done
many_params_list=""
for i in $(seq 0 16); do
  if [ -n "$many_params_list" ]; then many_params_list+=", "; fi
  many_params_list+="p$i = 0"
done
# 33 conversions and 33 matching arguments -- one past PROBE_MAX_PRINTF_ARGS
# (32) -- for the printf-argument-limit rejection below. want == argc, so the
# ordinary "format needs N argument(s)" check does not fire first; only the
# VM-limit check does.
many_printf_fmt=""
many_printf_args=""
for i in $(seq 1 33); do
  many_printf_fmt+="%u "
  if [ -n "$many_printf_args" ]; then many_printf_args+=", "; fi
  many_printf_args+="$i"
done
# One more address than PROBE_MAX_SITE_DECLS (8192), for the site-list
# overflow case below -- the file-reading counterpart of the in-script
# "too many install sites" case above.
many_site_addrs=$(for i in $(seq 0 8192); do printf '%04x\n' "$i"; done)

# --- Lexer -------------------------------------------------------------

expect_bad_script "'\$' not followed by a hex digit" "expected hex digits after '\$'" \
  'probe p(v = $g) { }'
expect_bad_script "a number out of range" "number out of range" \
  'probe p(v = $100000000) { }'
expect_bad_script "an invalid number '0x10'" "invalid number" \
  'probe p(v = 0x10) { }'
expect_bad_script "an invalid number '2frames'" "invalid number" \
  'probe p(v = 2frames) { }'
expect_bad_script "an identifier too long" "identifier too long" \
  "counter $long_ident"
expect_bad_script "a string literal too long" "string literal too long" \
  "probe p() { printf(\"$long_str\") }"
expect_bad_script "an unknown escape in a string" "unknown escape in string" \
  'probe p() { printf("bad\qend") }'
expect_bad_script "a raw newline in a string literal" "raw newline or CR in string literal" \
  'probe p() {
  printf("abc
def")
}'
expect_bad_script "an unexpected character" "unexpected character ';'" \
  'probe p() { ; }'

# "unterminated string literal" needs EOF to land inside the open string with
# no raw newline first -- but expect_bad_script's printf '%s\n' always adds a
# trailing newline, and that newline itself would be read as the "raw
# newline" case before EOF is ever reached. So this one is written directly,
# with no trailing newline, instead of through the wrapper.
printf 'probe p() { printf("abc' > probe-tmp/bad.probe
expect_probe_reject "an unterminated string literal" "unterminated string literal" \
  --probe=probe-tmp/bad.probe --probe-dump

# --- Declarations --------------------------------------------------------

expect_bad_script "an unknown top-level keyword" "expected 'counter', 'probe' or 'install'" \
  'foo'
expect_bad_script "a duplicate counter" "counter 'n' already declared" \
  'counter n
counter n'
expect_bad_script "a duplicate probe" "probe 'p' already declared" \
  'probe p() { }
probe p() { }'
expect_bad_script "a duplicate parameter" "duplicate parameter 'v'" \
  'probe p(v = 1, v = 2) { }'
expect_bad_script "a self-referential parameter" "unknown name 'v'" \
  'probe p(v = v) { }'
expect_bad_script "a non-literal counter initialiser" "a counter initialiser must be a literal" \
  'counter n = frame'
expect_bad_script "a reserved name as a counter" "'install' is a reserved name" \
  'counter install'
expect_bad_script "a reserved name as a parameter" "'if' is a reserved name" \
  'probe p(if = 1) { }'
expect_bad_script "a reserved name as a probe" "'stop' is a reserved name" \
  'probe stop() { }'
expect_bad_script "a reserved name as a counter, 'record'" "'record' is a reserved name" \
  'counter record'
expect_bad_script "a counter declared after a probe resolved it as a register" \
  "counter 'x' is declared after a probe resolved 'x' as a register" \
  'probe p() { printf("%u\n", x) }
counter x'
expect_bad_script "too many counters" "too many counters" \
  "$many_counters_script"
expect_bad_script "too many parameters" "too many parameters" \
  "probe p($many_params_list) { }"

# --- Expressions -----------------------------------------------------------

expect_bad_script "an unknown name in an expression" "unknown name 'qq'" \
  'probe p(v = qq) { }'
expect_bad_script "an expression nested too deeply" "expression nested too deeply" \
  "probe p(v = ${parens_open}1${parens_close}) { }"
expect_bad_script "an empty parenthesized expression" "expected an expression" \
  'probe p() { if () { } }'

# --- Statements --------------------------------------------------------

expect_bad_script "a token that cannot start a statement" "expected a statement" \
  'probe p() { 123 }'
expect_bad_script "an unterminated block" "unterminated block" \
  'probe p() { if (1) { '
expect_bad_script "an assignment to a register" "cannot assign to register 'a'" \
  'probe p() { a = 1 }'
expect_bad_script "an assignment to an unknown name" "unknown name 'bar'" \
  'probe p() { bar = 1 }'
expect_bad_script "'inc' on a non-counter" "'inc' needs a counter, but 'a' is not one" \
  'probe p() { inc a }'
expect_bad_script "'inc' on an unknown name" "unknown name 'baz'" \
  'probe p() { inc baz }'
expect_bad_script "a stray 'else'" "'else' without a matching 'if'" \
  'probe p() { else { } }'
# PROBE_MAX_STMT_DEPTH is 250; 260 nested blocks (like the 260-deep
# parenthesis run above for expressions) trips it well short of any real
# stack limit -- see probe_parse.c's comment on the constant for the
# measurement (16,000 nested blocks or `if`s segfault under `ulimit -s
# 1024`, the unguarded parser's actual failure mode before this test existed).
expect_bad_script "a block nested too deeply" "statement nested too deeply" \
  "probe p() { ${blocks_open}${blocks_close} }"
expect_bad_script "an 'if' nested too deeply" "statement nested too deeply" \
  "probe p() { $(printf 'if(1)%.0s' $(seq 1 260))stop }"

expect_bad_script "printf with too few arguments" 'format needs 1 argument(s), 0 given' \
  'probe p() { printf("%d") }'
expect_bad_script "printf with too many arguments" 'format needs 0 argument(s), 1 given' \
  'probe p() { printf("ok", 1) }'
expect_bad_script "printf with an unsupported conversion" "unsupported conversion '%s'" \
  'probe p() { printf("%s") }'
expect_bad_script "printf with a precision" "precision is not supported in a printf conversion" \
  'probe p() { printf("%.2d", 1) }'
expect_bad_script "a format string ending inside a conversion" "format string ends inside a conversion" \
  'probe p() { printf("val%") }'
expect_bad_script "an over-wide conversion" "conversion width too large (max 999)" \
  'probe p() { printf("%1000d") }'
expect_bad_script "printf over the VM's argument limit" "printf takes at most 32 arguments" \
  "probe p() { printf(\"$many_printf_fmt\", $many_printf_args) }"

# --- Install -------------------------------------------------------------

expect_bad_script "install of an unknown probe" "unknown probe 'nope'" \
  'install nope at $100'
expect_bad_script "install missing 'at'" "expected 'at' after the probe name" \
  'probe p() { }
install p $100'
expect_bad_script "a malformed install site" "expected an address, a range or" \
  'probe p() { }
install p at foo'
expect_bad_script "install with '@' not followed by a string" "expected a quoted file name after" \
  'probe p() { }
install p at @123'
expect_bad_script "an out-of-range install address" "address out of range" \
  'probe p() { }
install p at $10000'
expect_bad_script "a backwards install range" "range ends before it starts" \
  'probe p() { }
install p at $200-$100'
expect_bad_script "too many install sites" "too many install sites" \
  'probe p() { }
install p at $0000-$2001'
expect_bad_script "a duplicate install" 'already installed at $0100' \
  'probe p() { }
install p at $100, $100'

# --- Site lists ----------------------------------------------------------
#
# probe/dirlist.probe installs from @"." -- a directory, not a file. fopen(dir,
# "rt") succeeds on Linux, so the rejection has to come from the reader (an
# I/O error on the first read), not from opening the path.
expect_probe_reject "a directory used as a site list" "error reading site list" \
  --probe=probe/dirlist.probe --probe-dump

printf 'probe p() { }\ninstall p at @"does-not-exist.txt"\n' > probe-tmp/bad.probe
expect_probe_reject "a site list naming a file that does not exist" "cannot open site list" \
  --probe=probe-tmp/bad.probe --probe-dump

expect_site_reject "trailing text after an address" "unexpected text after the address" \
  '0300 extra'
expect_site_reject "an empty site list" "names no addresses" \
  ''
expect_site_reject "a comments-only site list" "names no addresses" \
  '# just a comment'
expect_site_reject "a comment line longer than the read buffer" "names no addresses" \
  "#$long_comment"
expect_site_reject "a malformed hex address" "expected a 16-bit hex address" \
  'zzzz'
expect_site_reject "a 0x-prefixed address" "expected a 16-bit hex address" \
  '0x300'
expect_site_reject "a leading sign on an address" "expected a 16-bit hex address" \
  '-300'

# add_sites_from_file builds its own path:lineno-prefixed duplicate-install
# and site-overflow diagnostics, distinct from add_site's (asserted above, in
# the "a duplicate install" and "too many install sites" cases): both wrap
# the same underlying message with "<site-list path>:<line>: " first. Neither
# form was exercised until now -- only the in-script ones were.
expect_site_reject "a duplicate install from a site list" "sites.txt:2: probe 'p' is already installed at \$0300" \
  '0300
0300'
expect_site_reject "too many install sites from a site list" "sites.txt:8193: too many install sites" \
  "$many_site_addrs"

# --- Options ---------------------------------------------------------------

expect_probe_reject "two --probe= scripts" "only one probe script may be loaded" \
  --probe=probe/empty.probe --probe=probe/empty.probe --probe-dump
expect_probe_reject "an unwritable --probe-out=" "cannot open probe output" \
  --probe=probe/empty.probe --probe-out=probe-tmp/nodir/out.txt
expect_probe_reject "--record-keys with no probe script" "--record-keys requires --probe=" \
  --record-keys=probe-tmp/rec.txt
# hello.probe has neither `key` nor `record` -- indistinguishable from
# rec.probe/play.probe with the wrong one of the pair loaded, which is the
# realistic way to hit this: push_key() would divert every key into
# pending_keys_ with nothing to ever call `record` and release it, holding
# every key forever with no diagnostic.
expect_probe_reject "--record-keys with a script that never calls record" \
  "--record-keys requires the probe script to call \`record\`" \
  --probe=probe/hello.probe --record-keys=probe-tmp/rec.txt

# --- Probes: execution ------------------------------------------------------
#
# Unlike the compile tests above, these run the machine. a2run boots the ROM
# deterministically with no input, so a probe's report is stable.

probe_run_test() {
  # $1: base name under probe/, $2: frame count
  if ! $a2run --frames="$2" --probe="probe/$1.probe" \
              --probe-out="probe-tmp/$1.txt" > /dev/null; then
    echo "FAIL: running probe/$1.probe" >&2
    exit 1
  fi
  diff -q "probe/$1.expected" "probe-tmp/$1.txt"
}

probe_run_test hello 2
probe_run_test format 2
probe_run_test chain 2
probe_run_test arith 2
probe_run_test flow 2
probe_run_test mem 2

# `stop` and the never-fired report. --frames=20 is deliberately far more than
# the run needs (see stopat.probe's comment: the whole 129-line report is
# already produced within the first simulated frame) -- the point is that
# `stop` is what ends the run before frame 20, not the frame limit, which a
# --frames=2 run could not distinguish from a no-op `stop`.
probe_run_test stopat 20

# A probe that never fired must say so. This is the guard against the design's
# main hazard: a probe on an address one engine does not dispatch on looks
# exactly like agreement -- no output, no error -- unless something checks for
# the silence itself. probe_report_unfired() writes to stderr, never the
# report file (probe-tmp/unfired.txt below is /dev/null), so this check can
# run independently of whatever the report itself contains.
$a2run --frames=20 --probe=probe/stopat.probe --probe-out=/dev/null \
  2>probe-tmp/unfired.txt >/dev/null
if ! grep -q "never fired" probe-tmp/unfired.txt; then
  echo "FAIL: no never-fired report for a probe that never ran" >&2
  cat probe-tmp/unfired.txt >&2
  exit 1
fi
# probe 's' fired (129 times); it must not be listed as never-fired, and
# probe_report_unfired() must not print anything at all for probes that did
# fire (only "never" -- installed at $0001 -- belongs in this file).
if grep -q "^  s " probe-tmp/unfired.txt; then
  echo "FAIL: a probe that did fire was reported as never firing" >&2
  cat probe-tmp/unfired.txt >&2
  exit 1
fi

# The same probe script through both front ends. a2emu and a2run share a host
# and an interpreter engine, differing only in the window (see the frontend
# equivalence check above), so any difference here is a bug in a front end,
# not in the probe VM. Note what this does and does not prove: both tools are
# still the interpreter, so this pins the front ends against each other, not
# the interpreter against a generated program -- that cross-engine assertion
# is the ROM-boot acceptance test.
$a2run --frames=40 --probe=probe/hello.probe --probe-out=probe-tmp/fe-run.txt \
  > /dev/null
$a2emu --headless --frames=40 --probe=probe/hello.probe \
  --probe-out=probe-tmp/fe-emu.txt > /dev/null
if ! diff -q probe-tmp/fe-run.txt probe-tmp/fe-emu.txt > /dev/null; then
  echo "FAIL: a2emu and a2run disagree on a probe report" >&2
  diff probe-tmp/fe-run.txt probe-tmp/fe-emu.txt | head -5 >&2
  exit 1
fi

# Recording: the same ROM boot, with keys arriving from --kbd-file and being
# stamped on a probe counter rather than on cycles. Two runs must produce
# byte-identical files -- a recording nobody has shown to be deterministic is
# worse than none -- and every stamp must be a counter value, so far below any
# plausible cycle count that a cycle-stamped file could not be mistaken for it.
printf 'A' > probe-tmp/one.kbd
$a2run --frames=10 --probe=probe/reckeys.probe --kbd-file=probe-tmp/one.kbd \
  --record-keys=probe-tmp/rec-a.txt --probe-out=/dev/null > /dev/null
$a2run --frames=10 --probe=probe/reckeys.probe --kbd-file=probe-tmp/one.kbd \
  --record-keys=probe-tmp/rec-b.txt --probe-out=/dev/null > /dev/null
if ! diff -q probe-tmp/rec-a.txt probe-tmp/rec-b.txt > /dev/null; then
  echo "FAIL: --record-keys is not deterministic" >&2
  diff probe-tmp/rec-a.txt probe-tmp/rec-b.txt >&2
  exit 1
fi
if ! grep -qE '^[0-9]+ 65$' probe-tmp/rec-a.txt; then
  echo "FAIL: --record-keys did not record the 'A' that was typed" >&2
  cat probe-tmp/rec-a.txt >&2
  exit 1
fi

# A --kbd-file longer than the pending queue. push_key() diverts every key
# into pending_keys_ while recording, so io_'s own hardware queue never
# fills -- a reader that only asked *that* queue how much room was left would
# keep reading past pending_keys_'s capacity and hit its overflow FATAL,
# however far off the next `record` that drains it is. 40 keys, comfortably
# past PENDING_KEYS_MAX (== A2_KBD_QUEUE_SIZE == 32), is the regression check.
printf 'A%.0s' {1..40} > probe-tmp/forty.kbd
if ! $a2run --frames=20 --probe=probe/reckeys.probe --kbd-file=probe-tmp/forty.kbd \
     --record-keys=probe-tmp/rec-40.txt --probe-out=/dev/null > /dev/null; then
  echo "FAIL: --record-keys aborted on a --kbd-file longer than the pending queue" >&2
  exit 1
fi
count40=$(grep -cE '^[0-9]+ 65$' probe-tmp/rec-40.txt || true)
if [ "$count40" -ne 40 ]; then
  echo "FAIL: --record-keys recorded $count40 of 40 keys from a long --kbd-file" >&2
  cat probe-tmp/rec-40.txt >&2
  exit 1
fi

# --key-file= (cycle-stamped keys) combined with --record-keys=: this is the
# conversion the next task's play.keys -> play.pkeys step depends on, and it
# only works if drain_key_presses()/probe_deliver_keys() divert through
# push_key() the same way live typing does -- calling a2_io_push_key()
# directly, as they once did, bypasses the pending queue entirely and
# --record-keys= silently writes nothing but its header line.
printf '236 65\n236 66\n236 67\n' > probe-tmp/three.keys
if ! $a2run --frames=10 --probe=probe/reckeys.probe --key-file=probe-tmp/three.keys \
     --record-keys=probe-tmp/rec-kf.txt --probe-out=/dev/null > /dev/null; then
  echo "FAIL: --record-keys with --key-file= failed to run" >&2
  exit 1
fi
countkf=$(grep -cE '^[0-9]+ [0-9]+$' probe-tmp/rec-kf.txt || true)
if [ "$countkf" -ne 3 ]; then
  echo "FAIL: --record-keys with --key-file= recorded $countkf keys, expected 3" >&2
  cat probe-tmp/rec-kf.txt >&2
  exit 1
fi
if ! grep -qE '^[0-9]+ 65$' probe-tmp/rec-kf.txt || ! grep -qE '^[0-9]+ 66$' probe-tmp/rec-kf.txt ||
   ! grep -qE '^[0-9]+ 67$' probe-tmp/rec-kf.txt; then
  echo "FAIL: --record-keys with --key-file= did not record the keys it was given" >&2
  cat probe-tmp/rec-kf.txt >&2
  exit 1
fi

# --trace-keys must fire for every key source, --key-file= included, now that
# push_key() is the single point all of them funnel through (see its
# comment). No --probe= or --record-keys= here -- this pins --trace-keys on
# its own. Pinning the exact count matters: --trace-keys printing nothing for
# --key-file=-sourced keys is the pre-fix behaviour, and a test that only
# checked "at least one line" would pass on either.
printf '10 65\n20 66\n30 67\n' > probe-tmp/trace.keys
$a2run --frames=3 --trace-keys --key-file=probe-tmp/trace.keys > probe-tmp/trace.out
tracecount=$(grep -cE '^[0-9]+ (65|66|67)$' probe-tmp/trace.out || true)
if [ "$tracecount" -ne 3 ]; then
  echo "FAIL: --trace-keys printed $tracecount lines for a 3-key --key-file=, expected 3" >&2
  cat probe-tmp/trace.out >&2
  exit 1
fi

# Replay of a counter-stamped file must be driven by `key`, not by the frame
# drain. Counter stamps are small integers, so against get_cycles() they are
# all already in the past and an unguarded drain delivers the whole file
# during the first simulated frame -- leaving `key` nothing to do and making
# the two scripts below behave identically.
#
# replay-key.probe releases only the key stamped 1; replay-nokey.probe
# releases nothing at all. With the drain guarded, the ROM gets one key in the
# first case and none in the second, so the two reports differ. With the drain
# unguarded, the drain delivers both keys in both cases and the reports are
# the same -- which is the failure this asserts against.
#
# --frames=100 is load-bearing, not a round number: with no --bin=/--rom=,
# a2run boots the bundled ROM's self-test and slot scan before anything else
# runs, so the installed $FD1B KEYIN loop is not reached until roughly cycle
# 141,500 (about frame 8) -- measured directly by tracing $C000/$C010
# accesses. Before that, neither script has anything to distinguish: the key
# stamped 1 is already queued (via drain or via `key`, whichever the script
# uses) long before the loop's first check either way. The two scripts only
# diverge once the second key, stamped 999999, becomes cycle-due for the
# unguarded drain -- measured at roughly frame 59 (999999 cycles /
# ~17,051 cycles per frame). A run shorter than that -- --frames=10, as this
# test originally read -- passes or fails for the wrong reason: it reports
# the same "FAIL" whether or not the guard exists, because the divergence it
# depends on hasn't happened yet. 100 clears frame 59 with headroom rather
# than sitting on that threshold, so the test does not go brittle against a
# future change to ROM boot timing or cycle accounting.
printf '1 65\n999999 66\n' > probe-tmp/two.pkeys
$a2run --frames=100 --probe=probe/replay-key.probe --key-file=probe-tmp/two.pkeys \
  --probe-out=probe-tmp/replay-key.txt > /dev/null
$a2run --frames=100 --probe=probe/replay-nokey.probe --key-file=probe-tmp/two.pkeys \
  --probe-out=probe-tmp/replay-nokey.txt > /dev/null
if diff -q probe-tmp/replay-key.txt probe-tmp/replay-nokey.txt > /dev/null; then
  echo "FAIL: delivering a key changed nothing -- the frame drain delivered it anyway" >&2
  exit 1
fi

rm -rf probe-tmp
rm -rf mcp-tmp

echo "Success!"
