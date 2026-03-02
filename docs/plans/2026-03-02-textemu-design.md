# textemu Terminal Emulator Improvements

## Goal

Make textemu usable for interactive Apple II emulation in a terminal, with two modes:
- **ncurses mode**: full 40x24 screen redraw using ncurses
- **stream mode**: cursor-following output to stdout, suitable for piping

## Approach

Single-thread, polling-based architecture with ~50Hz frame rate. No threads, no synchronization.

## Architecture

### Main Loop

```
parse CLI args (--ncurses, --stream, ROM path)
load ROM
detect mode: explicit flag > auto (TTY -> ncurses, pipe -> stream)
init display backend
loop:
    frameStart = steady_clock::now()
    emu->runFor(20460 cycles)          // 20ms of 6502 time
    while (key = backend->readKey()) >= 0:
        push key to Apple II keyboard queue
    backend->updateScreen(emu)
    elapsed = steady_clock::now() - frameStart
    if elapsed < 20ms: sleep_for(20ms - elapsed)
```

No catch-up on missed frames — just proceed with the next frame.

### Display Backend Interface

Abstract base class in textemu.cpp:

- `init()` / `shutdown()` — setup and teardown
- `readKey()` — non-blocking, returns -1 if no key
- `updateScreen(emu)` — refresh display from emulator state

Two implementations: `NcursesDisplay` and `StreamDisplay`.

### Ncurses Mode

- 40x24 ncurses window, no decorations
- Shadow buffer (40x24 uint8_t) for diffing — only update changed cells via `mvaddch()`
- Character attributes: normal = `A_NORMAL`, inverse = `A_REVERSE`, flashing = treated as inverse
- Keyboard: `nodelay(stdscr, TRUE)` for non-blocking `getch()`
- Key mapping: printable -> uppercase, Backspace/Left -> ASCII 8, Right -> ASCII 21, Enter -> 13, Escape -> 27
- Cleanup via `endwin()` on exit and signal handlers

### Stream Mode

- Terminal set to raw mode via `tcsetattr()`: `~ICANON`, `~ECHO`, `VMIN=0`, `VTIME=0`
- Keyboard: `read(0, &ch, 1)` non-blocking, same key translations
- Output via cursor-following:
  - Track Apple II cursor from zero-page `$24` (CH, column) and `$25` (CV, row)
  - Maintain lastCH/lastCV from previous frame
  - When cursor advances: read characters between old and new position from text screen memory, emit to stdout
  - When cursor moves backward (CLS, etc.): emit `\n` separator
  - Scroll handled naturally: cursor stays on line 23, previously emitted lines already output
- Restore original termios on exit and signal handlers

### Signal Handling

Global `DisplayBackend *` pointer. SIGINT/SIGTERM handlers call `shutdown()` then `_exit()`.

### CLI

- `--ncurses` — force ncurses mode
- `--stream` — force stream mode
- No flag — auto-detect via `isatty(STDOUT_FILENO)`
- Positional arg — ROM path (falls back to built-in rom or `"rom/apple2plus.rom"`)

### Build

Add ncurses dependency to `tools/textemu/CMakeLists.txt`.

## Key Details

- Apple II text screen at 0x0400, interleaved layout: `offset = (line % 8) * 128 + (line / 8) * 40`
- Cursor zero-page: CH = $24 (column 0-39), CV = $25 (row 0-23)
- Apple II keyboard queue via `a2_io_push_key()`
- Character decoding: `ch & 0x7F`, control chars replaced with space
- Inverse detection: bits 7-6 == 00

## Dropped Features

The existing textemu has a debug command interface (once-per-second prompt with commands
like `a` for Applesoft BASIC dump, `r` for reset, `:` and `` ` `` for line injection).
This is replaced by direct keyboard passthrough — every keystroke goes to the Apple II.
Debug features are available via the `id` tool or `a2emu`.

The `Debug6502` wrapper class (with `DebugState6502` callback) is retained for future use
(e.g., a `--trace` flag could be added later).

## Scope

- Text mode only (no graphics modes)
- No sound
- No disk support (separate branch)
- All changes in textemu.cpp + CMakeLists.txt
