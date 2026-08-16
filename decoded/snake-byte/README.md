
To run the decompiled binary, after starting type `CALL 14160` in BASIC.

## Regenerating the run data

`snake-byte.json` is the recorded trace apple2tc decompiles against. It was
produced by a windowed `a2emu` in January 2022 and, until `a2run` existed,
nothing in the tree could reproduce it. The command is now:

```shell
a2run snake-byte.b33 --from-rom --collect --limit=3600348 \
      --key-file=play.keys --frames=1300 --out=new.json
```

**This does not reproduce the committed file byte for byte, and cannot.** The
2022 session's keystrokes and duration were never recorded, so its coverage
cannot be replayed. What the above produces is a *different session of the same
game*: 478 of the 500 branch targets match, the self-modification generations
are byte-identical, and the differences are all "this session went somewhere
else" — `func_64c8`'s region reached by one and not the other, a decimal `SBC`
executed by one and not the other.

So do not overwrite `snake-byte.json` casually. Every committed artefact
downstream — the generated C, `play.frames`, the coverage report — is keyed to
it, and replacing it invalidates all of them at once. Regenerate deliberately,
into a scratch file, and re-verify everything before adopting it.

## Recording a session

The `.pkeys` files are keystrokes stamped on the probe counter `rec.probe`
defines, and they drive every check in `probe-acceptance.sh`. To capture a new
one, play the *original* binary in the windowed emulator:

```shell
../../cmake-build-release/tools/a2emu/a2emu --preload snake-byte.b33 \
    --probe=rec.probe --record-keys=play-new.pkeys
```

`--preload` types nothing for you, so `CALL 14160` ends up in the recording the
way it does in the committed ones. Close the window when done — that runs
sokol's cleanup hook, which is what closes the file. Ctrl-C in the terminal
does not, and truncates it.

`--trace-keys > play-new.keys` instead captures *cycle*-stamped keys, the older
format, which `a2run --key-file=... --probe=rec.probe --record-keys=...`
converts. That is how `play.keys` became `play.pkeys`, and how
`play-rebind.pkeys` was made.

Then replay it for long enough to deliver every key — `play-rebind` needs 3300
frames for its 45 — and add it to the scenario list at the top of
`probe-acceptance.sh` as `<keys>:<frames>`.

**What a recording is worth is what it varies.** Both original recordings press
the default direction keys, where the two key tables at `$6C63` and `$6C6A` hold
identical bytes, so nothing checked that the game reads the right one of the
two — with full block coverage of the code that does. `play-rebind` exists for
exactly that: it rebinds to W A S Z Q E and then plays. Coverage will not tell
you which recordings you are missing; a mutation test will.
