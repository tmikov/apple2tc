# Every loop that can only be left by pressing a key must be able to yield.
#
# The game runs as a coroutine on its own thread. A cycle charge is its only
# suspend point: cycles_expired() parks it there and hands the host the window
# in which the host draws a frame and polls the keyboard. A loop that spins on
# $C000 with no charge in it therefore waits for a key that can never arrive --
# the host is never given a turn in which to deliver one. The window freezes
# and the process has to be killed.
#
# Nothing else in this repository can catch that. The probe scenarios stamp
# their keys on a counter incremented at the *read*, so under replay a spinning
# loop keeps incrementing it and receives its keys without a frame ever
# happening: the whole gate stays green while the real game hangs. It happened
# on 2026-08-26 -- ESC pause and the ROM's Ctrl-S screen hold, both found by
# playing the game on a Mac and neither visible to 28 passing checks.
#
# So the check is static. Reads of the keyboard, its strobe and the buttons are
# what a loop can block on; a charge is `advance()` or any `GAME_CYCLES` form,
# since those charge too.

FNR == 1 { ntop = 0; depth = 0 }   # each file starts clean

function flag_all(what,   i) {
  for (i = 0; i < ntop; i++)
    if (what == "input") saw_input[i] = 1; else saw_charge[i] = 1
}

/io_peek\(0xc0(00|1[0-9]|6[0-7])\)/ { flag_all("input") }
/(advance|GAME_CYCLES[A-Z_]*)[ \t]*\(/ { flag_all("charge") }

{
  line = $0
  # A loop header opens a new scope to watch. Braceless bodies are covered by
  # the per-line flagging above, which has already run for this line.
  if (line ~ /^[ \t]*(for|while)[ \t]*\(/ || line ~ /^[ \t]*do[ \t]*\{/) {
    start[ntop] = FNR; startdepth[ntop] = depth
    saw_input[ntop] = 0; saw_charge[ntop] = 0
    text[ntop] = line
    ntop++
    # this line's own reads belong to the loop as well
    if (line ~ /io_peek\(0xc0(00|1[0-9]|6[0-7])\)/) saw_input[ntop-1] = 1
    if (line ~ /(advance|GAME_CYCLES[A-Z_]*)[ \t]*\(/) saw_charge[ntop-1] = 1
  }

  n = gsub(/\{/, "{", line); depth += n
  n = gsub(/\}/, "}", line); depth -= n

  # close every loop whose body has ended
  while (ntop > 0 && depth <= startdepth[ntop-1] && FNR > start[ntop-1]) {
    ntop--
    if (saw_input[ntop] && !saw_charge[ntop]) {
      bad++
      sub(/^[ \t]+/, "", text[ntop])
      printf "  %s:%d: %s\n", FILENAME, start[ntop], text[ntop]
    }
  }
}

END {
  if (bad) {
    printf "FAIL [yield]: %d loop(s) read input but can never yield\n", bad
    print  "  A loop with no cycle charge is never suspended, so the host never"
    print  "  runs, so the key it is waiting for never arrives. The game hangs"
    print  "  with a frozen window. Give each loop an advance() worth one pass."
    exit 1
  }
}
