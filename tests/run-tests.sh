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

# --dump-reg-liveness: what actually crosses a call boundary, per function.
# reglive.s has hand-known flag behaviour -- see the comment at the top of it --
# so these baselines pin what the option reports rather than that it runs. Two
# of them, at -O2 and -O3, because the -O3 dce() pass is driven by this same
# liveness and deletes the stores it proves dead; the pair is what separates
# "the flag is not live" from "the store is already gone".
$a6502 reglive.s reglive.b33 && $apple2tc reglive.b33 -O3 --dump-reg-liveness > reglive-test.txt
diff -q reglive.dump reglive-test.txt
$apple2tc reglive.b33 -O2 --dump-reg-liveness > reglive-O2-test.txt
diff -q reglive-O2.dump reglive-O2-test.txt
# Said outright, so a regression reads as more than an unexplained baseline
# diff. At -O2 setcarry writes A, N, Z and C, and only C survives to its
# caller; clobbernz writes N and Z that nothing ever reads; usesy needs Y.
if ! grep -q '^FUNC  \$0330 \.\{10\} \.\{9\}C A\.\.N\.\.\.\.ZC ' reglive-O2-test.txt; then
  echo "FAIL: a routine returning its answer in carry did not report C alone live-out" >&2
  grep '^FUNC  \$0330' reglive-O2-test.txt >&2
  exit 1
fi
if ! grep -q '^FUNC  \$0340 \.\{10\} \.\{10\} AX\.N\.\.\.\.Z\. ' reglive-O2-test.txt; then
  echo "FAIL: N and Z clobbered with no reader were not reported dead" >&2
  grep '^FUNC  \$0340' reglive-O2-test.txt >&2
  exit 1
fi
if ! grep -q '^FUNC  \$0350 \.\.Y\.\{7\} ' reglive-O2-test.txt; then
  echo "FAIL: a routine indexing with Y did not report Y live-in" >&2
  grep '^FUNC  \$0350' reglive-O2-test.txt >&2
  exit 1
fi
rm reglive-test.txt reglive-O2-test.txt reglive.b33

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
# ...and the *next* run must advance again. The probe's stop flag is sticky in
# the library (a2run exits on it, so it never needs clearing); if a2mcp did not
# clear it per run, request 4 would come back with frames_run 1 and
# stop_reason "probe" forever, and no tool -- not even boot, which preserves
# probe state on purpose -- could get the server moving again.
if ! grep '"id":4' mcp-tmp/stop.txt | grep -q 'frames_run\\": 10'; then
  echo "FAIL: a probe's stop wedged every later a2mcp run" >&2
  exit 1
fi

# --probe= is forwarded to the library, and probe_out() defaults to *stdout* --
# which here carries JSON-RPC. Without --probe-out=, the stop.probe script's
# printf must still not land in the protocol stream. Every stdout line has to
# be a JSON object; the report line goes to stderr instead.
$a2mcp --root=. --probe=mcp/stop.probe < mcp/stop.jsonl \
  > mcp-tmp/probe-stdout.txt 2> mcp-tmp/probe-stderr.txt
if grep -qvE '^\{' mcp-tmp/probe-stdout.txt; then
  echo "FAIL: a probe's printf wrote into a2mcp's JSON-RPC stream" >&2
  grep -nvE '^\{' mcp-tmp/probe-stdout.txt >&2
  exit 1
fi
if ! grep -q 'halt at FD0C' mcp-tmp/probe-stderr.txt; then
  echo "FAIL: the probe report did not reach stderr either -- it went nowhere" >&2
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

$a6502 mcp/gr.s mcp-tmp/gr.b33
mcp_transcript gr
# The reply is a JSON object dumped into a JSON string, so its own quotes
# reach the transcript backslash-escaped and on one line -- a line-anchored
# `^1111111111` never matches for that reason, not because the grid is wrong.
# A plain, unanchored substring search on the full 40-character run of `1`s
# that gr.s paints across row 0 is specific enough that nothing else in the
# transcript can satisfy it by accident.
if ! grep -qF '1111111111111111111111111111111111111111' mcp-tmp/gr.txt; then
  echo "FAIL: the lo-res grid does not show the colour-1 row the program painted" >&2
  exit 1
fi

# `run`'s "screen_update" `until` value: two detectors that both count as one
# "update" -- a page-2 flip, and a settle (a change that then holds for K=2
# consecutive frame boundaries).
#
# flip.s flips PAGE2 four times, each landing in its own host frame (see the
# fixture's own comment for why the delay between flips is tuned to a window
# that is long enough to guarantee that but short enough that the settle
# detector's own K=2 window never closes in between -- otherwise the idle gap
# during the delay would itself register as a spurious settle, double
# counting one flip as two updates). Requests 3 and 4 ask for two updates
# each and must land exactly on flip 2 and flip 4 -- not flip 1 (too early)
# and not flip 3 (too late).
$a6502 mcp/flip.s mcp-tmp/flip.b33
mcp_transcript flip
if ! grep '"id":3' mcp-tmp/flip.txt | grep -q 'updates\\": 2'; then
  echo "FAIL: screen_update did not count exactly two flips by request 3" >&2
  exit 1
fi
if ! grep '"id":4' mcp-tmp/flip.txt | grep -q 'updates\\": 2'; then
  echo "FAIL: screen_update did not count exactly two more flips by request 4" >&2
  exit 1
fi
# Request 5 asks for one more update, but the program has thrown all four
# flips it has and is now spinning forever: there is nothing left to detect,
# so the call must run to its frame cap rather than fire early on a stale
# flip count, and the reply must say so.
if ! grep '"id":5' mcp-tmp/flip.txt | grep -q 'stop_reason\\": \\"limit'; then
  echo "FAIL: screen_update fired again after the program's last flip" >&2
  exit 1
fi
if ! grep '"id":5' mcp-tmp/flip.txt | grep -q 'updates\\": 0'; then
  echo "FAIL: screen_update reported updates after the program's last flip" >&2
  exit 1
fi
if ! grep '"id":5' mcp-tmp/flip.txt | grep -q 'no complete-frame boundary was found'; then
  echo "FAIL: screen_update's frame-cap reply is missing the note" >&2
  exit 1
fi

# flip-over.jsonl asks for one more update (5) than the program will ever
# throw (4), in a single call whose window spans well past the last real
# flip -- this is what catches "count the flip only, reset the quiescence
# state" not holding: flip.s gives page 1 and page 2 different content up
# front specifically so every flip also changes the visible hash, and a
# version of machine_run that credits both the flip and that hash change
# would leave `dirty` set after the last flip instead of clearing it, so two
# frames later the settle detector would fire a bogus fifth update. The
# honest answer is to run out the full 60-frame cap having only ever seen
# four.
mcp_transcript flip-over
if ! grep '"id":3' mcp-tmp/flip-over.txt | grep -q 'stop_reason\\": \\"limit'; then
  echo "FAIL: screen_update counted a fifth update the program never produced" >&2
  exit 1
fi
if ! grep '"id":3' mcp-tmp/flip-over.txt | grep -q 'updates\\": 4'; then
  echo "FAIL: screen_update did not honestly report all four real flips" >&2
  exit 1
fi

# settle.s writes to the *displayed* page five times, then stops and spins.
# Request 3 must detect the settle well short of its 60-frame cap -- the
# writing itself is done by around frame 8 (five ~1.4-frame-spaced writes),
# so a settle past, say, frame 30 would mean the detector is not firing on
# quiescence at all.
$a6502 mcp/settle.s mcp-tmp/settle.b33
mcp_transcript settle
if ! grep '"id":3' mcp-tmp/settle.txt | grep -q 'stop_reason\\": \\"screen_update'; then
  echo "FAIL: no settle was detected after settle.s stopped writing" >&2
  exit 1
fi
if ! grep '"id":3' mcp-tmp/settle.txt | grep -qE 'frames_run\\": ([1-9]|[12][0-9])[^0-9]'; then
  echo "FAIL: the settle in request 3 did not fire well short of the frame cap" >&2
  exit 1
fi
# The test that actually proves "static counts as nothing": request 4 starts
# from the now-settled, unchanging screen and asks for one more update. A
# settle requires *prior activity* within the call, so this must run all the
# way to its own frame cap instead of firing on request 3's leftover state --
# an instant return here would mean an already-static screen was mistaken for
# a completed picture.
if ! grep '"id":4' mcp-tmp/settle.txt | grep -q 'stop_reason\\": \\"limit'; then
  echo "FAIL: screen_update returned immediately on an already-static screen" >&2
  exit 1
fi
if ! grep '"id":4' mcp-tmp/settle.txt | grep -q 'updates\\": 0'; then
  echo "FAIL: screen_update reported an update on an already-static screen" >&2
  exit 1
fi
if ! grep '"id":4' mcp-tmp/settle.txt | grep -q 'no complete-frame boundary was found'; then
  echo "FAIL: screen_update's frame-cap reply on a static screen is missing the note" >&2
  exit 1
fi

# `run`'s `settle` grace window: a stopping *policy* over the same detectors
# above, not a new one. Advance `frames` frames exactly as a plain run does,
# then spend up to `settle` more looking for a moment when the displayed
# picture is complete (a flip, or the visible hash unchanged across two
# consecutive frame boundaries -- the same K=2 as screen_update's settle, but
# without screen_update's "requires prior activity" gate).
#
# Reusing settle.b33: request 3's frame cap (10) lands mid-draw, and the
# settle window (20) must carry it all the way to the fixture's real
# quiescence at frame 17 -- the same frame screen_update itself lands on
# above, which is what proves this isn't firing on a coincidental early
# plateau.
$a6502 mcp/settle.s mcp-tmp/settle.b33
mcp_transcript grace
if ! grep '"id":3' mcp-tmp/grace.txt | grep -q 'stop_reason\\": \\"settled'; then
  echo "FAIL: run(frames, settle) did not land on the fixture's quiescence" >&2
  exit 1
fi
if ! grep '"id":3' mcp-tmp/grace.txt | grep -q 'frames_run\\": 17'; then
  echo "FAIL: run(frames, settle) landed on the wrong frame" >&2
  exit 1
fi
# Request 4 starts from that already-settled screen and asks again: unlike
# screen_update (request 5, same starting state, runs all the way to its
# frame cap with updates 0), a settle grace has no "prior activity" gate, so
# an already-static screen is exactly the good moment it is looking for --
# it must fire within the K=2 minimum, not wait out the window.
if ! grep '"id":4' mcp-tmp/grace.txt | grep -q 'stop_reason\\": \\"settled'; then
  echo "FAIL: settle did not recognize an already-static screen as a good moment" >&2
  exit 1
fi
if ! grep '"id":4' mcp-tmp/grace.txt | grep -q 'frames_run\\": 7'; then
  echo "FAIL: settle did not fire on the first possible frame of an already-static screen" >&2
  exit 1
fi
if ! grep '"id":5' mcp-tmp/grace.txt | grep -q 'stop_reason\\": \\"limit'; then
  echo "FAIL: screen_update fired on an already-static screen -- it should require prior activity" >&2
  exit 1
fi
if ! grep '"id":5' mcp-tmp/grace.txt | grep -q 'updates\\": 0'; then
  echo "FAIL: screen_update reported an update on an already-static screen" >&2
  exit 1
fi

# visible-page.b33 redraws every single frame, so it never has an idle
# frame-boundary pair: settle's grace window must run out honestly and say
# so, rather than firing on a spurious "unchanged" reading.
$a6502 mcp/visible-page.s mcp-tmp/visible-page.b33
mcp_transcript grace-busy
if ! grep '"id":3' mcp-tmp/grace-busy.txt | grep -q 'stop_reason\\": \\"unsettled'; then
  echo "FAIL: settle did not give up honestly on a screen with no idle moment" >&2
  exit 1
fi
if ! grep '"id":3' mcp-tmp/grace-busy.txt | grep -q 'frames_run\\": 25'; then
  echo "FAIL: an unsettled stop did not run the full frames+settle window" >&2
  exit 1
fi
# `settle` is a policy over `until: "frames"` only; combined with
# "screen_change" (or "screen_update") it must reject rather than guess.
if ! grep '"id":4' mcp-tmp/grace-busy.txt | grep -q '"isError":true'; then
  echo "FAIL: settle combined with until: \"screen_change\" did not raise an error" >&2
  exit 1
fi
# `settle` gets the same malformed-argument treatment as `updates` and
# `frames_between`: a non-integer (5) and an in-type but out-of-range value
# (6) must both raise isError without taking the process down, and request 7
# -- an ordinary `status` right after -- must still answer in the same
# session.
if ! grep '"id":5' mcp-tmp/grace-busy.txt | grep -q '"isError":true'; then
  echo "FAIL: a non-integer settle did not raise an error" >&2
  exit 1
fi
if ! grep '"id":6' mcp-tmp/grace-busy.txt | grep -q '"isError":true'; then
  echo "FAIL: an out-of-range settle did not raise an error" >&2
  exit 1
fi
if ! grep -q '"id":7' mcp-tmp/grace-busy.txt; then
  echo "FAIL: a2mcp died on a malformed settle instead of answering the next request" >&2
  exit 1
fi

# `keys` schedules rather than injects, and the .keys file it can write is the
# whole reason to prefer that: a session recorded through the tool must replay
# bit-for-bit under a2run's own --key-file=.
mcp_transcript keys --keys-out=mcp-tmp/session.keys --hash-frames=mcp-tmp/keys-mcp.txt
# The reply is a JSON object dumped into a JSON string, so its own quotes reach
# the transcript backslash-escaped and on one line -- grepping for `^]PRINT
# 2+2` (bare prompt, line-anchored) never matches for that reason, not because
# the assertion failed. An unanchored, unescaped substring search is what
# proves the keystrokes actually reached BASIC; the exact evaluated answer is
# already pinned by the transcript diff above, so it needs no grep of its own.
if ! grep -qF 'PRINT 2+2' mcp-tmp/keys.txt; then
  echo "FAIL: keys were scheduled but BASIC never ran them" >&2
  exit 1
fi

# The reproducibility claim, and the reason the .keys file is the artifact: the
# session must replay under a2run frame for frame. 300 = 120 + 180, the frames
# the transcript runs.
$a2run --frames=300 --key-file=mcp-tmp/session.keys \
  --hash-frames=mcp-tmp/keys-a2run.txt > /dev/null
if ! diff -u mcp-tmp/keys-mcp.txt mcp-tmp/keys-a2run.txt; then
  echo "FAIL: a2run --key-file= does not reproduce the a2mcp session" >&2
  exit 1
fi

# `sound`: beep.s toggles the speaker every ~1005 cycles (see beep.s for the
# NMOS 6502 timing this comes from), 200 times, then goes silent forever.
$a6502 mcp/beep.s mcp-tmp/beep.b33
mcp_transcript sound
# The exact frequency depends on the delay loop's real cycle count, so assert
# the band the program can produce rather than a single number. The reply is
# a JSON object dumped into a JSON string, so its own quotes reach the
# transcript backslash-escaped, matching what mcp_tools.cpp's text_result()
# actually writes -- see the "sound" case in mcp_tools.cpp -- not the
# unescaped quotes a naive grep would expect.
if ! grep -qE '\\"approx_frequency_hz\\": (4[5-9][0-9]|5[0-6][0-9])' mcp-tmp/sound.txt; then
  echo "FAIL: sound did not report the ~510 Hz tone the program plays" >&2
  exit 1
fi
if [ "$(head -c 4 mcp-tmp/beep.wav)" != "RIFF" ]; then
  echo "FAIL: save_wav did not write a WAV" >&2
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

# The four ways a tool call used to end badly, in one transcript. Each reply is
# pinned by mcp/guard.expected; what the greps below add is that the failure is
# the *intended* one and that the server is still there afterwards.
#
# The write side of the jail gets its own symlink, mirroring the read-side one
# above: resolving only the parent directory and re-attaching the final
# component writes straight through a link sitting at that component.
# The link's target has to exist, or the rejection would be "cannot resolve"
# rather than the escape this is about.
escape_dir=$(mktemp -d)
printf 'untouched' > "$escape_dir/target.png"
ln -sf "$escape_dir/target.png" mcp-tmp/wescape.png
mcp_transcript guard
if [ "$(cat "$escape_dir/target.png")" != "untouched" ]; then
  echo "FAIL: a2mcp wrote a screenshot through a symlink pointing out of --root" >&2
  rm -rf "$escape_dir"
  exit 1
fi
rm -rf "$escape_dir"
# A regular file that is in the jail but is not a disk image reached
# mountDisk(), which exit(2)s -- killing the server mid-call with the client
# still waiting. Request 8 answering at all is the half that matters.
if ! grep -q '"id":8' mcp-tmp/guard.txt; then
  echo "FAIL: a2mcp died mid-session instead of returning a tool error" >&2
  exit 1
fi
# A long text at a wide spacing overflows the 32-bit cycle stamp: the keys wrap
# below the current cycle and all fire at once, and the .keys file comes out
# non-monotonic. A refusal is the only honest answer.
if ! grep '"id":5' mcp-tmp/guard.txt | grep -q '32-bit cycle counter'; then
  echo "FAIL: a long keys call wrapped its cycle stamps instead of being refused" >&2
  exit 1
fi

# A wrong-typed argument used to be a fifth way to take the server down (or, for
# `scale`, to silently misbehave instead): nlohmann's typed accessors throw
# nlohmann::json::type_error on a type mismatch, and a non-integer `scale` is
# not a mismatch at all -- it is silently truncated by a plain static_cast,
# so the "must be 1 or 2" check never saw the real value. Request 3 is that
# truncation (scale: 1.5), 4 is a type mismatch serve() used to let escape
# uncaught (render: 42, the reproduction named in review), 5 is a second
# truncation shape (scale as a string). Requests 6-9 put `blend` through the
# identical four shapes: a float (6), an in-type-but-zero value (7, below the
# 1..8 range), an in-type-but-too-large value (8, above it), and a string (9)
# -- the same class of bug this project has shipped before, so `blend` gets
# the same coverage `scale` does rather than trusting that its is_number_
# unsigned() check alone is enough. Requests 11 and 12 are `run`'s `updates`
# hitting the identical shape of bug -- a float (11) and an in-type but
# out-of-range value (12), so both the type check and the range check are
# exercised. Request 13 -- an ordinary `status` in the same session -- is what
# actually matters: mcp/malformed.expected pins its reply, so if any of
# 3-9/11-12 crashed the process instead of answering with isError, request 13
# would be missing from the transcript and the diff below would fail on a
# truncated file, not just a wrong one.
mcp_transcript malformed
for id in 3 4 5 6 7 8 9 11 12; do
  if ! grep -q "\"id\":$id.*\"isError\":true" mcp-tmp/malformed.txt; then
    echo "FAIL: malformed request $id did not come back as isError" >&2
    exit 1
  fi
done
if ! grep -q '"id":13' mcp-tmp/malformed.txt; then
  echo "FAIL: a2mcp died on a malformed argument instead of answering the next request" >&2
  exit 1
fi

# `screen` as PNG. Base64 of a PNG is long, so the committed transcript covers
# only the save_to success case and the root-escape rejection -- the inline
# case below is checked directly instead of via a baseline.
mcp_transcript image

# A PNG starts with the eight-byte signature 89 50 4E 47 0D 0A 1A 0A.
if [ ! -s mcp-tmp/shot.png ] || [ "$(head -c 8 mcp-tmp/shot.png | od -An -tx1 | tr -d ' ')" \
     != "89504e470d0a1a0a" ]; then
  echo "FAIL: save_to did not write a PNG" >&2
  exit 1
fi

# The IHDR's colour-type byte is at offset 25 and must be 2, RGB.
#
# This is the check that names the failure. The transcript diff above would
# also go red on a wrong encoding, but only until someone regenerates the
# baseline -- which is exactly how this bug would come back, and how it got in.
# What follows is what a perfectly valid, completely blank screenshot looked
# like. a2_screen is "RGB encoding of the Apple2 screen"
# and its fourth byte is padding that no renderer in a2io.c ever fills, so
# encoding it as four-channel RGBA produced a 280x192 PNG in which every pixel
# had alpha 0 -- correct signature, correct dimensions, byte-identical between
# runs, and invisible in any viewer that honours alpha. Every structural
# assertion passed. Only looking at a pixel found it.
if [ "$(dd if=mcp-tmp/shot.png bs=1 skip=25 count=1 2>/dev/null | od -An -tu1 | tr -d ' ')" != "2" ]; then
  echo "FAIL: the saved PNG is not RGB; an alpha channel here renders blank" >&2
  exit 1
fi

# Base64 of that same signature starts iVBORw0KGgo -- confirms the inline
# content block in image.expected actually carries a PNG rather than
# something else that happened to pass the mimeType check.
if ! grep -q 'iVBORw0KGgo' mcp/image.expected; then
  echo "FAIL: screen format=image's save_to reply did not inline a PNG too" >&2
  exit 1
fi

# The inline-PNG case (format "image" with no save_to) is not in the
# committed transcript -- its base64 would dominate the baseline -- so it is
# checked with a one-off invocation instead.
printf '%s\n' \
  '{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"protocolVersion":"2025-06-18","capabilities":{},"clientInfo":{"name":"t","version":"0"}}}' \
  '{"jsonrpc":"2.0","id":2,"method":"tools/call","params":{"name":"boot","arguments":{}}}' \
  '{"jsonrpc":"2.0","id":3,"method":"tools/call","params":{"name":"run","arguments":{"frames":120}}}' \
  '{"jsonrpc":"2.0","id":4,"method":"tools/call","params":{"name":"screen","arguments":{"format":"image"}}}' \
  > mcp-tmp/inline-image.jsonl
$a2mcp --root=. < mcp-tmp/inline-image.jsonl > mcp-tmp/inline-image.txt
if ! grep -q '"mimeType":"image/png"' mcp-tmp/inline-image.txt || \
   ! grep -q 'iVBORw0KGgo' mcp-tmp/inline-image.txt; then
  echo "FAIL: screen format=image did not return an inline PNG content block" >&2
  exit 1
fi

# `screen`'s `render` parameter -- the colour-clock HGR decoders.
#
# Applesoft's HCOLOR values are documented (1=green, 2=violet, 3=white,
# 5=orange, 6=blue), so a BASIC program that draws one bar per colour is
# ground truth the emulator itself can generate, rather than a bit pattern
# hand-verified against prose. mcp/hgr-render.jsonl types that program in,
# lets it run, then reads it back: "color140" (decode at true colour-clock
# resolution so each bar comes out in one consistent colour), "mono140" (the
# same colour-clock cells collapsed to an ink/no-ink occupancy mask), and a
# bogus render value that must be rejected the same way an unknown `format`
# is.
mcp_transcript hgr-render

python3 mcp/check_hgr_png.py bars mcp-tmp/hgr-bars.png
if [ $? -ne 0 ]; then
  echo "FAIL: render=color140 did not decode the HCOLOR bars to the documented colours" >&2
  exit 1
fi

# The structural check that doubling actually happened -- that COLOR140 is
# genuinely decoding at 140-pixel resolution and not just relabelling the
# per-dot COLOR renderer's output: every colour cell must land on two equal
# adjacent pixels.
python3 mcp/check_hgr_png.py pairs mcp-tmp/hgr-bars.png
if [ $? -ne 0 ]; then
  echo "FAIL: render=color140's output pixels are not paired at 140 resolution" >&2
  exit 1
fi

python3 mcp/check_hgr_png.py mono mcp-tmp/hgr-mono140.png
if [ $? -ne 0 ]; then
  echo "FAIL: render=mono140 produced a pixel that is not pure black or white" >&2
  exit 1
fi

# The discriminator mono140 exists for: HCOLOR=1 (row 10) is drawn as
# alternating dots -- a naive per-dot rendering of that row would come out
# roughly half black, a one-pixel-pitch comb, because that IS what a colour
# fill physically is on this hardware. mono140 must instead read the row as
# solid ink: zero black pixels across the full 280-pixel width.
python3 mcp/check_hgr_png.py row mcp-tmp/hgr-mono140.png 10 white
if [ $? -ne 0 ]; then
  echo "FAIL: render=mono140 did not read the alternating-dot green bar as solid ink" >&2
  exit 1
fi

# Control 1: HCOLOR=3 (row 130) sets both dots of every cell, so it must also
# read solid in mono140 -- showing the previous assertion's solidity comes
# from mono140 correctly calling a colour fill "ink", not from mono140
# blanket-filling every row white regardless of content.
python3 mcp/check_hgr_png.py row mcp-tmp/hgr-mono140.png 130 white
if [ $? -ne 0 ]; then
  echo "FAIL: render=mono140 did not read the white bar as solid ink" >&2
  exit 1
fi

# Control 2: row 9, immediately above the green bar, has no HPLOT output at
# all and must read solid black -- showing mono140 is filling cells that have
# ink, not blanket-filling the image white.
python3 mcp/check_hgr_png.py row mcp-tmp/hgr-mono140.png 9 black
if [ $? -ne 0 ]; then
  echo "FAIL: render=mono140 painted a row with no HPLOT output" >&2
  exit 1
fi

if ! grep '"id":8' mcp-tmp/hgr-render.txt | grep -q '"isError":true'; then
  echo "FAIL: an unknown screen render value was not rejected" >&2
  exit 1
fi

# `scale`: output pixels per colour cell for the two colour-clock HGR modes.
# Request 9 asks for color140 at scale:1, request 10 for mono140 at scale:1,
# request 11 for color140 at scale:2 (the default, spelled out explicitly),
# and requests 12/13 for the out-of-range values 3 and 0.

# Dimensions: scale:1 is 140x192; the default (request 6, no `scale` at all)
# and an explicit scale:2 (request 11) are both 280x192.
python3 mcp/check_hgr_png.py dims mcp-tmp/hgr-bars-140.png 140 192
if [ $? -ne 0 ]; then
  echo "FAIL: render=color140 scale=1 was not 140x192" >&2
  exit 1
fi
python3 mcp/check_hgr_png.py dims mcp-tmp/hgr-bars.png 280 192
if [ $? -ne 0 ]; then
  echo "FAIL: render=color140 with no scale (default) was not 280x192" >&2
  exit 1
fi
python3 mcp/check_hgr_png.py dims mcp-tmp/hgr-bars-scale2.png 280 192
if [ $? -ne 0 ]; then
  echo "FAIL: render=color140 scale=2 was not 280x192" >&2
  exit 1
fi
python3 mcp/check_hgr_png.py dims mcp-tmp/hgr-mono140-140.png 140 192
if [ $? -ne 0 ]; then
  echo "FAIL: render=mono140 scale=1 was not 140x192" >&2
  exit 1
fi

# The check that matters: scale:1 is exactly the un-doubled form of scale:2,
# not a resample, a crop, or a different decode -- column k of the 140-wide
# image must equal columns 2k and 2k+1 of the 280-wide image, for every row.
python3 mcp/check_hgr_png.py downsample mcp-tmp/hgr-bars-140.png mcp-tmp/hgr-bars.png
if [ $? -ne 0 ]; then
  echo "FAIL: render=color140 scale=1 lost information relative to scale=2" >&2
  exit 1
fi

# mono140 at scale:1 is still pure black/white -- scale changes width, not
# the mono decode.
python3 mcp/check_hgr_png.py mono mcp-tmp/hgr-mono140-140.png
if [ $? -ne 0 ]; then
  echo "FAIL: render=mono140 scale=1 produced a pixel that is not pure black or white" >&2
  exit 1
fi

if ! grep '"id":12' mcp-tmp/hgr-render.txt | grep -q '"isError":true'; then
  echo "FAIL: scale=3 was not rejected" >&2
  exit 1
fi
if ! grep '"id":13' mcp-tmp/hgr-render.txt | grep -q '"isError":true'; then
  echo "FAIL: scale=0 was not rejected" >&2
  exit 1
fi

# Mixed-mode HGR: the text window (screen lines 20-23) is drawn by the same
# draw_glyph_cb() no matter which a2_hgr_mode_t rendered the rest of the
# screen, so rows 160-191 of a "color" render and of a "color140" render at
# scale:2 must be pixel-identical -- the exact-oracle regression test for the
# widening bug that used to stretch that already-full-width text to double
# width via `x / widen` applied to every row instead of just the graphics
# ones. hgr-mixed140.s also puts an alternating-dot comb in the graphics area
# (rows 0-159), where "color" (per-dot artifact) and "color140" (colour-clock
# average) are expected to disagree -- checked below too, so a screen that
# happened to be blank could not pass the first assertion by accident.
$a6502 mcp/hgr-mixed140.s mcp-tmp/hgr-mixed140.b33
mcp_transcript hgr-mixed140

python3 mcp/check_hgr_png.py rows-equal \
  mcp-tmp/hgr-mixed140-color.png mcp-tmp/hgr-mixed140-color140.png 160 191
if [ $? -ne 0 ]; then
  echo "FAIL: mixed-mode text window differs between render=color and render=color140 scale=2" >&2
  exit 1
fi
python3 mcp/check_hgr_png.py rows-differ \
  mcp-tmp/hgr-mixed140-color.png mcp-tmp/hgr-mixed140-color140.png 0 159
if [ $? -ne 0 ]; then
  echo "FAIL: render=color and render=color140 did not disagree anywhere in the graphics area" >&2
  exit 1
fi

# `blend`: composite the last N frames' HGR page with a bitwise OR. blend.s
# draws a constant solid line at row 8 and flickers a second one at row 32 on
# and off roughly every host frame -- a single-buffered game in miniature.
# Request 3 catches it missing at blend:1; requests 5, 7, 9, 11, 13 (each
# after one more frame) check that blend:3 shows both lines in every one of
# five consecutive captures, which is the actual point of the feature -- a
# single capture can miss the second line, a blended one cannot.
$a6502 mcp/blend.s mcp-tmp/blend.b33
mcp_transcript blend
python3 mcp/check_hgr_png.py row mcp-tmp/blend-miss.png 32 black
if [ $? -ne 0 ]; then
  echo "FAIL: blend:1 did not miss the flickering line it was supposed to" >&2
  exit 1
fi
python3 mcp/check_hgr_png.py row mcp-tmp/blend-miss.png 8 white
if [ $? -ne 0 ]; then
  echo "FAIL: blend:1's capture lost the constant line too" >&2
  exit 1
fi
for k in 0 1 2 3 4; do
  python3 mcp/check_hgr_png.py row mcp-tmp/blend-both-$k.png 8 white
  if [ $? -ne 0 ]; then
    echo "FAIL: blend:3 capture $k lost the constant line" >&2
    exit 1
  fi
  python3 mcp/check_hgr_png.py row mcp-tmp/blend-both-$k.png 32 white
  if [ $? -ne 0 ]; then
    echo "FAIL: blend:3 capture $k missed the flickering line" >&2
    exit 1
  fi
done
# blend:1 (no compositing) never gets a blend_frames field -- see the reply
# doc comment on why the field is conditional -- while every blend:3 capture
# above must report exactly 3.
if grep '"id":4' mcp-tmp/blend.txt | grep -q 'blend_frames'; then
  echo "FAIL: blend:1 reported a blend_frames field that must not exist" >&2
  exit 1
fi
for id in 6 8 10 12 14; do
  if ! grep "\"id\":$id" mcp-tmp/blend.txt | grep -q 'blend_frames":3'; then
    echo "FAIL: request $id did not report blend_frames:3" >&2
    exit 1
  fi
done
# `blend` is meaningless outside HGR and must be accepted-and-ignored there,
# the same precedent `render`/`scale` set: requests 16 and 17, both taken in
# TEXT mode right after a reboot, must be byte-for-byte identical PNGs even
# though one asks for blend:1 and the other blend:4.
if ! cmp -s mcp-tmp/text-blend1.png mcp-tmp/text-blend4.png; then
  echo "FAIL: blend was not ignored in TEXT mode" >&2
  exit 1
fi
if grep -E '"id":16|"id":17' mcp-tmp/blend.txt | grep -q 'blend_frames'; then
  echo "FAIL: blend_frames leaked into a TEXT-mode reply" >&2
  exit 1
fi

# The blend ring must not survive a boot: request 4, taken after 40 frames of
# blend.s, blends a full ring (blend_frames:8). Request 5 reboots the same
# program from scratch (frame/cycle counters back to 0) and request 7, taken
# after only 11 of the new frames -- not enough for the ring to fill again --
# must report fewer than 8, proving the ring holds only frames run since this
# boot and not the previous session's leftovers. (Verified by temporarily
# disabling the reset in blend_reset(): request 7 then wrongly reports
# blend_frames:8, the stale full count from before the reboot.)
mcp_transcript blend-reset
if ! grep '"id":4' mcp-tmp/blend-reset.txt | grep -q 'blend_frames":8'; then
  echo "FAIL: blend:8 after 40 frames did not fill the ring" >&2
  exit 1
fi
if grep '"id":7' mcp-tmp/blend-reset.txt | grep -q 'blend_frames":8'; then
  echo "FAIL: the blend ring survived a boot instead of resetting" >&2
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

# a2io's soft switches had no direct test at all, which is how a write hitting
# every switch twice survived from 2021 to 2026 -- silencing any program that
# drives the speaker with STA $C030. a2io-test links a2io alone, with no CPU
# and no window, and asserts the shape of the generated waveform.
# See tests/a2io/a2io_test.c.
if ! "$bin/tests/a2io/a2io-test" > a2io-out.txt; then
  echo "FAIL: a2io soft switches" >&2
  cat a2io-out.txt >&2
  exit 1
fi
rm a2io-out.txt

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

# --echo-keys is the human-readable counterpart of --trace-keys above: stderr,
# not stdout, one line per key actually delivered, with a running (n/total)
# count while a --key-file= replay is driving delivery. The `#` line checks
# that a comment survives --key-file='s own parser, since that is the format
# --record-keys= writes and this option is meant to watch a replay of it.
printf '0 72\n0 73\n# a comment line, tolerated like --key-file= tolerates it\n0 32\n0 65\n0 33\n' \
  > probe-tmp/echo.keys
$a2run --frames=3 --echo-keys --key-file=probe-tmp/echo.keys \
  > /dev/null 2> probe-tmp/echo.err
echocount=$(grep -cE "^\[frame [0-9]+\] '.'  \([0-9]+/5\)\$" probe-tmp/echo.err || true)
if [ "$echocount" -ne 5 ]; then
  echo "FAIL: --echo-keys printed $echocount lines for a 5-key --key-file=, expected 5" >&2
  cat probe-tmp/echo.err >&2
  exit 1
fi
# Order and the running count both matter: a shuffled or off-by-one counter
# would still pass the count-only check above.
if ! grep -qE "^\[frame [0-9]+\] 'H'  \(1/5\)\$" probe-tmp/echo.err ||
   ! grep -qE "^\[frame [0-9]+\] 'I'  \(2/5\)\$" probe-tmp/echo.err ||
   ! grep -qE "^\[frame [0-9]+\] ' '  \(3/5\)\$" probe-tmp/echo.err ||
   ! grep -qE "^\[frame [0-9]+\] 'A'  \(4/5\)\$" probe-tmp/echo.err ||
   ! grep -qE "^\[frame [0-9]+\] '!'  \(5/5\)\$" probe-tmp/echo.err; then
  echo "FAIL: --echo-keys did not report H,I,SPACE,A,! in order as 1/5..5/5" >&2
  cat probe-tmp/echo.err >&2
  exit 1
fi
if ! grep -E "^\[frame [0-9]+\] '.'  \([0-9]+/5\)\$" probe-tmp/echo.err | tail -1 |
    grep -qE '\(5/5\)$'; then
  echo "FAIL: --echo-keys' last echoed line for a 5-key replay did not read (5/5)" >&2
  cat probe-tmp/echo.err >&2
  exit 1
fi

# --echo-keys must be a pure diagnostic: it writes to stderr and must never
# perturb the machine or its pacing (unlike --trace-keys, which deliberately
# forces fixed-step timing -- see a2host_fixed_step()'s comment). Replaying
# the same key file with and without --echo-keys must hash identically.
$a2run --frames=40 --key-file=probe-tmp/echo.keys --hash-frames=probe-tmp/hash-noecho.txt \
  > /dev/null
$a2run --frames=40 --key-file=probe-tmp/echo.keys --echo-keys \
  --hash-frames=probe-tmp/hash-echo.txt > /dev/null 2> /dev/null
if ! diff -q probe-tmp/hash-noecho.txt probe-tmp/hash-echo.txt > /dev/null; then
  echo "FAIL: --echo-keys changed the emulated frame hashes" >&2
  diff probe-tmp/hash-noecho.txt probe-tmp/hash-echo.txt | head -5 >&2
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
# A scheduled key must arrive as a *keystroke*, not as type-ahead. kbd-latch.s
# polls $C000 and never acknowledges through KBDSTRB, which is what a game that
# reads the keyboard its own way does. Type-ahead only reaches the latch while
# the strobe is clear, so under it every key after the first stages behind the
# strobe and never arrives -- which is exactly how dsk/Neptune.dsk came to look
# unresponsive to the keyboard while playing fine in a2emu, whose live keys
# always overwrite the latch.
$a6502 mcp/kbd-latch.s mcp-tmp/kbd-latch.b33 > /dev/null
mcp_transcript kbd-latch
python3 - mcp-tmp/kbd-latch.txt <<'EOF'
import json, sys
seen = {}
for line in open(sys.argv[1]):
    m = json.loads(line)
    if m.get("id") in (6, 9):
        seen[m["id"]] = m["result"]["content"][0]["text"][0]
if seen.get(6) != "A" or seen.get(9) != "B":
    print(f"FAIL: scheduled keys did not arrive as keystrokes: {seen}", file=sys.stderr)
    sys.exit(1)
EOF
if [ $? -ne 0 ]; then exit 1; fi

# `a2mcp --key-file=` replaying a cycle-stamped .keys recording -- the read
# half of the asymmetry --keys-out= already covers. mcp/replay.keys is a
# synthetic 10-key recording (see its own header comment for the cycle math).
# mcp/replay.jsonl boots, checks `status` before anything is delivered, runs
# 20+20 frames (delivering 4 keys, then the rest), checks `status` after
# each, then calls `boot` again -- which a2host_load_key_file() reloads the
# recording into, cursor at zero, right after a2host_reboot() has restarted
# the cycle counter at zero and reset RAM to its power-on fill -- and repeats
# the same 20+20/status sequence to confirm the reload replays the recording
# identically.
#
# `boot` on an already-running machine is a power-on, not a warm restart
# (dz issue 01a07cd8: measuring the difference is what found this gap), so
# "identically" is checked two ways below: the keys land at the same *frame*
# offsets (`status`'s `replay` counter), and the machine's video output is
# byte-identical too (`--hash-frames=`), which is the stronger of the two and
# subsumes it.
mcp_transcript replay --key-file=mcp/replay.keys --hash-frames=mcp-tmp/replay-mcp-hash.txt

# The video oracle: a2mcp replaying the recording must be byte-identical to
# a2run replaying the same recording over the same 40 frames -- the same
# oracle the plain `run.jsonl` block above holds a2mcp's frame loop to --
# and the second 40-frame stretch (after the `boot` at request 8) must be
# byte-identical to the first, since a `boot` on a running machine is now a
# power-on and the recording replays from the top either way.
$a2run --frames=40 --key-file=mcp/replay.keys --hash-frames=mcp-tmp/replay-a2run-hash.txt \
  > /dev/null
head -n 40 mcp-tmp/replay-mcp-hash.txt > mcp-tmp/replay-mcp-hash-first.txt
tail -n 40 mcp-tmp/replay-mcp-hash.txt > mcp-tmp/replay-mcp-hash-second.txt
if ! diff -q mcp-tmp/replay-mcp-hash-first.txt mcp-tmp/replay-a2run-hash.txt; then
  echo "FAIL: a2mcp's --key-file= replay diverges from a2run's over the same recording" >&2
  diff mcp-tmp/replay-mcp-hash-first.txt mcp-tmp/replay-a2run-hash.txt | head -5 >&2
  exit 1
fi
if ! diff -q mcp-tmp/replay-mcp-hash-second.txt mcp-tmp/replay-a2run-hash.txt; then
  echo "FAIL: a boot on a running a2mcp does not reproduce a fresh process byte-for-byte" >&2
  diff mcp-tmp/replay-mcp-hash-second.txt mcp-tmp/replay-a2run-hash.txt | head -5 >&2
  exit 1
fi

# `status`'s `replay` field before the reboot: zero before any run, a real
# partial count after the first `run`, and delivered == total once the
# recording is exhausted.
if ! grep '"id":3' mcp-tmp/replay.txt | grep -q '\\"delivered\\": 0' ||
   ! grep '"id":3' mcp-tmp/replay.txt | grep -q '\\"total\\": 10'; then
  echo "FAIL: status before any run did not report replay: {delivered: 0, total: 10}" >&2
  exit 1
fi
if ! grep '"id":5' mcp-tmp/replay.txt | grep -q '\\"delivered\\": 4'; then
  echo "FAIL: status after the first run did not report 4 keys delivered" >&2
  exit 1
fi
if ! grep '"id":7' mcp-tmp/replay.txt | grep -q '\\"delivered\\": 10'; then
  echo "FAIL: status after the recording finished did not report all 10 keys delivered" >&2
  exit 1
fi

# The reboot itself (request 8) must reload the recording rather than refuse
# or resume a stale cursor: request 9's status must read back to 0/10 --
# exactly request 3's reply -- not a refusal and not a stale partial count.
if ! grep '"id":9' mcp-tmp/replay.txt | grep -q '\\"delivered\\": 0' ||
   ! grep '"id":9' mcp-tmp/replay.txt | grep -q '\\"total\\": 10'; then
  echo "FAIL: status right after a reboot did not report replay: {delivered: 0, total: 10}" >&2
  exit 1
fi

# Key-delivery identity across the reboot: requests 11 and 13 must land on
# the same delivered counts as requests 5 and 7 did at the same frame
# offsets (20 and 40 frames into each boot) -- the same keys at the same
# cycle stamps, replayed against a cycle counter that also restarted at
# zero. Subsumed by the video byte-identity check above, but kept as its own
# assertion: it fails on exactly the cursor-reset math in
# a2host_load_key_file()/drain_key_presses(), which is the one thing this
# whole feature is about.
if ! grep '"id":11' mcp-tmp/replay.txt | grep -q '\\"delivered\\": 4'; then
  echo "FAIL: status 20 frames after the reboot did not report 4 keys delivered, same as before it" >&2
  exit 1
fi
if ! grep '"id":13' mcp-tmp/replay.txt | grep -q '\\"delivered\\": 10'; then
  echo "FAIL: status 40 frames after the reboot did not report all 10 keys delivered, same as before it" >&2
  exit 1
fi

# The realistic case: a game disk, not a bare ROM. `--root=..` here (instead
# of the usual `.`) so `disk1: "dsk/Neptune.dsk"` resolves -- the fixture
# lives at the repo root, not under tests/. 1400 frames is the reproduction
# dz issue 01a07cd8 was measured with: enough for the disk to finish booting
# and for the game to be well into its attract loop. Compares a fresh
# process's first boot against the *second* boot of one process that
# rebooted once, matching the "fresh process; boot; run N" versus "one
# process; boot; run N; boot; run N" oracle the fix was built around, and
# stronger than the bare-ROM check above: this booted state is a whole
# DOS/RWTS load, not a handful of typed characters.
printf '%s\n' \
  '{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"protocolVersion":"2025-06-18","capabilities":{},"clientInfo":{"name":"t","version":"0"}}}' \
  '{"jsonrpc":"2.0","id":2,"method":"tools/call","params":{"name":"boot","arguments":{"disk1":"dsk/Neptune.dsk"}}}' \
  '{"jsonrpc":"2.0","id":3,"method":"tools/call","params":{"name":"run","arguments":{"frames":1400}}}' \
  > mcp-tmp/reboot-disk-fresh.jsonl
printf '%s\n' \
  '{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"protocolVersion":"2025-06-18","capabilities":{},"clientInfo":{"name":"t","version":"0"}}}' \
  '{"jsonrpc":"2.0","id":2,"method":"tools/call","params":{"name":"boot","arguments":{"disk1":"dsk/Neptune.dsk"}}}' \
  '{"jsonrpc":"2.0","id":3,"method":"tools/call","params":{"name":"run","arguments":{"frames":1400}}}' \
  '{"jsonrpc":"2.0","id":4,"method":"tools/call","params":{"name":"boot","arguments":{"disk1":"dsk/Neptune.dsk"}}}' \
  '{"jsonrpc":"2.0","id":5,"method":"tools/call","params":{"name":"run","arguments":{"frames":1400}}}' \
  > mcp-tmp/reboot-disk-twice.jsonl
$a2mcp --root=.. --hash-frames=mcp-tmp/reboot-disk-fresh-hash.txt \
  < mcp-tmp/reboot-disk-fresh.jsonl > /dev/null
$a2mcp --root=.. --hash-frames=mcp-tmp/reboot-disk-twice-hash.txt \
  < mcp-tmp/reboot-disk-twice.jsonl > /dev/null
tail -n 1400 mcp-tmp/reboot-disk-twice-hash.txt > mcp-tmp/reboot-disk-twice-second.txt
if ! diff -q mcp-tmp/reboot-disk-fresh-hash.txt mcp-tmp/reboot-disk-twice-second.txt; then
  echo "FAIL: boot(disk1) on a running a2mcp does not reproduce a fresh process's boot byte-for-byte" >&2
  diff mcp-tmp/reboot-disk-fresh-hash.txt mcp-tmp/reboot-disk-twice-second.txt | head -5 >&2
  exit 1
fi

rm -rf mcp-tmp

echo "Success!"
