# Driving a2mcp to play a game

`docs/a2mcp.md` says what the six tools do. This says how to use them to boot an
unknown disk, work out its controls, and play it — the parts that are not
obvious from the tool descriptions, and the two or three that are actively
counter-intuitive.

Everything here was learned driving the finished server against
`dsk/Neptune.dsk`. Where something was observed once rather than established,
it says so.

## The shape of a session

    boot → reach a stable state → identify the controls → play in fixed steps

Only the third step is unusual, and it is the one that saves the most time: for
a game you have never seen, the control scheme is usually printed *inside the
disk image*, and reading it there costs one shell command instead of a dozen
guesses at a hundred frames each.

## Stepping: `screen_change` to arrive, fixed frames to play

`until: "screen_change"` stops on the first frame whose displayed screen differs
from the frame the call started on. That makes it exactly right for *getting
somewhere*:

- waiting out a disk boot,
- waiting for a loading screen to finish,
- advancing to the next thing worth looking at when you have no idea how long it
  takes.

It is useless the moment anything on screen animates, which in a game is
essentially always. Neptune's title screen animates, so
`run(frames=900, until="screen_change")` returned `frames_run: 1` — every time.
That is the condition working correctly and telling you nothing.

**Once the game is live, step by a fixed number of frames.** A frame is 1/60 s
of emulated time, so pick the step from how fast the game moves and how much you
are willing to miss:

| step | emulated | good for |
|---|---|---|
| 60 | 1 s | reacting to something you expect imminently |
| 150-260 | 2.5-4.5 s | ordinary play; one burst of input per step |
| 600+ | 10 s+ | attract loops, waiting out an intro |

You are blind between screenshots. A 260-frame step means four seconds of game
happened without you, so send the input for that whole window *before* the step,
not after it.

## Booting a disk: it does not autoboot

`boot(disk1: "...")` mounts the image and brings the machine up, and you land at
a BASIC `]` prompt, not in the game. Type `PR#6` to boot the drive:

    keys("PR#6\n") ; run(1200)

Observed with `dsk/Neptune.dsk`: still at the prompt after 600 frames, booted
within 1200 of `PR#6`. **The cause is not established.** `reset_regs()` runs
before `init_emulated()` loads the ROM in `a2host_init_emulation()`, which is
worth looking at if this ever matters more than the one extra tool call — but
nothing here confirms that is the reason.

## `boot` does not clear RAM

Rebooting re-initialises IO and the engine; it does not zero memory. In a
long-lived server the first `screen` after a `boot` can show the *previous*
session's screen. Neptune's boot showed a BASIC listing left over from an
earlier session in the same process.

Take it as a rule: after `boot`, do not trust a screenshot until you have run
enough frames for the new program to have drawn something.

## Finding the controls without guessing

Games of this era print their key map on a help screen, so the text is sitting
in the disk image in high-bit ASCII. Search for it:

```bash
python3 - <<'EOF'
d = open("dsk/Neptune.dsk", "rb").read()
cur, out = "", []
for b in d:
    c = b & 0x7F                      # Apple II text has the high bit set
    if 32 <= c < 127:
        cur += chr(c)
    else:
        if len(cur) >= 8: out.append(" ".join(cur.split()))
        cur = ""
for s in out:
    u = s.upper()
    if any(k in u for k in ("KEY", "ARROW", "FIRE", "LEFT", "RIGHT", "PRESS")):
        print(repr(s))
EOF
```

For Neptune that prints the key diagram directly:

    'LEFT J---- ----K RIGHT . W #'
    'I A -- -- -FIRE M M'

— an I/J/K/M diamond with `A` to fire. Worth trying before anything else.

**There are no paddles or buttons.** `lib/a2io` implements no `$C061`-`$C06F`
handling at all, so a game that wants a joystick or a paddle button cannot be
played through this server, and a game that *starts* on a button press cannot be
started. The keyboard is the whole input surface.

## Confirming your input actually landed

The trap is attract mode: a demo that plays itself looks exactly like a game you
are controlling, and you can spend a long time "playing" a movie. Do not judge
from the picture — judge from the status line.

For Neptune, the game showed `BO:20` (bombs) where the attract loop showed
`BO:00`, and `SH` moved `00 → 04 → 03` across a firing burst and a hit. Counters
that only a real game maintains are the cheap proof.

More generally, when you are unsure whether a keypress reached the program:
press a key that should do something visible, step, and compare — with `screen`
in `text` or GR mode if the game is in one of those, since a 40x24 grid diffs by
eye far more reliably than a hi-res picture.

## Spacing keys is how you emulate holding one down

`keys` schedules; `frames_between` sets the gap. There is no key-repeat and no
key-down state — a real Apple II keyboard hands the program one character at a
time, and so does this.

- `frames_between: 4-8` reads like a held direction key: `keys("KKKKKK", 5)`
  moves steadily right for half a second.
- `frames_between: 0` stamps everything at the current cycle and overruns the
  32-entry hardware queue past 32 keys. Only for short bursts.
- Interleave to act within one step: `keys("KKKKAAAAKKKKAAAA", 5)` moves and
  fires across about 80 frames, which is one ordinary step's worth of play.

Schedule the whole burst, then `run` once. Two `keys` calls with a `run` between
them cost twice the turns for the same emulated time.

## What the session actually costs

Emulated time is free — the machine is frozen between calls, so thinking is free
too. What you spend is **turns**: one perceive-act cycle is a `keys`, a `run`
and a `screen`, and a few hundred of those is a comfortable budget.

That rules some things in and some out. A game that pauses for input, or one
where four seconds of drift is survivable, plays fine. A twitch game does not:
you will get through a few seconds of it and learn little. Prefer the cheapest
representation that answers your question — `screen(format:"text")` is about a
kilobyte and an HGR PNG is far more, so read the text screen when the game is in
one, and only pay for an image when the game is in hi-res.

## Worked example: Neptune (Gebelli Software)

Verified in one session:

| | |
|---|---|
| boot | `boot(disk1:"dsk/Neptune.dsk")`, then `keys("PR#6\n")`, `run(1200)` |
| title | full-screen HGR, page 2, animated — `screen_change` fires every frame |
| start | space, from the title |
| in game? | `BO:20` in a real game, `BO:00` in the attract loop |
| controls | `J` left, `K` right, `I` up, `M` down, `A` fire (from the disk's own legend) |

Not established, and worth knowing that it is not: whether `A` is really fire
(pressing it moved `SH` from `00` to `04`, which is not what a fire button
should do), what `SH` and `BO` count exactly, and whether space is the only way
to start. Someone who plays it properly should correct this table.

## If this needs to auto-load

It is a plain document because that is how this repo distributes knowledge —
`CLAUDE.md` names the tool and points at `docs/a2mcp.md`, which points here. If
it ever needs to load itself into an agent's context on a matching task, a
Claude Code skill is this file plus a small frontmatter block, moved to
`.claude/skills/a2mcp-playing/SKILL.md`.
