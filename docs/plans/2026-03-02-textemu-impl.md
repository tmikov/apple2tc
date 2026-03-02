# textemu Terminal Improvements — Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Replace the unusable textemu with a proper terminal-based Apple II emulator supporting ncurses (full screen) and stream (cursor-following stdout) modes.

**Architecture:** Single-threaded polling loop at ~50Hz. Abstract `DisplayBackend` base class with `NcursesDisplay` and `StreamDisplay` implementations. All code in `textemu.cpp`.

**Tech Stack:** C++17, ncurses, POSIX termios.

**Testing flags:** `--frames=N` exits after N frames. `--dump-screen` prints 40x24 ASCII text screen to stderr on exit. These enable fully automated testing of both backends.

---

### Task 0: Pre-flight — verify clean build and existing tests pass

**Purpose:** Establish a known-good baseline before making any changes.

**Step 1: Full build**

```bash
cmake -G Ninja -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug && ninja -C cmake-build-debug
```

Expected: All targets build with zero errors.

**Step 2: Run existing tests**

```bash
cd tests && bash run-tests.sh ../cmake-build-debug
```

Expected: `Success!` output.

**Step 3: Verify current textemu binary exists**

```bash
ls -la cmake-build-debug/tools/textemu/textemu
```

Expected: Binary exists.

**Completion criteria:** Full build succeeds. Existing tests pass. textemu binary exists.

---

### Task 1: Add ncurses dependency to build

**Files:**
- Modify: `tools/textemu/CMakeLists.txt`

**Step 1: Update CMakeLists.txt**

Replace the contents of `tools/textemu/CMakeLists.txt` with:

```cmake
# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

link_libraries(d6502 cpuemu a2io support)
add_executable(textemu textemu.cpp)
target_link_libraries(textemu ${CURSES_LIBRARIES})
```

**Step 2: Verify build and linkage**

```bash
cmake -G Ninja -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug && ninja -C cmake-build-debug textemu
ldd cmake-build-debug/tools/textemu/textemu | grep -i curses
```

Expected: Builds successfully. `ldd` shows ncurses/curses in linked libraries.

**Step 3: Verify no regressions**

```bash
ninja -C cmake-build-debug && cd tests && bash run-tests.sh ../cmake-build-debug ../cmake-build-debug
```

Expected: Full build succeeds, tests print `Success!`.

**Step 4: Commit**

```bash
git add tools/textemu/CMakeLists.txt
git commit -m "textemu: add ncurses build dependency"
```

**Completion criteria:** textemu links against ncurses. Full project builds. Existing tests pass.

---

### Task 2: Scaffold DisplayBackend interface, CLI parsing, and test flags

This task replaces the entire body of `textemu.cpp` with the new structure. Display backends are stubs. Includes `--frames=N` and `--dump-screen` flags for automated testing.

**Files:**
- Modify: `tools/textemu/textemu.cpp`

**Step 1: Write the new textemu.cpp skeleton**

Replace the file contents. Key elements:

- `Debug6502` class retained unchanged (for future trace support)
- `DisplayBackend` abstract base: `init()`, `shutdown()`, `readKey()`, `updateScreen()`
- `NcursesDisplay` and `StreamDisplay` as stubs (empty implementations)
- Signal handling: global `g_backend` pointer, `signalHandler` calls `shutdown()` then `_exit(1)`
- CLI parsing: `--ncurses`, `--stream`, `--frames=N`, `--dump-screen`, positional ROM path
- Auto-detect mode via `isatty(STDOUT_FILENO)`
- Main loop with wall-clock timing (20ms frames), frame counter with `--frames` limit
- `dumpTextScreen()` helper: on exit, if `--dump-screen` set, decode Apple II text page and print 24 lines of 40 chars to stderr

```cpp
/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "apple2tc/DebugState6502.h"
#include "apple2tc/a2io.h"
#include "apple2tc/apple2.h"
#include "apple2tc/apple2plus_rom.h"
#include "apple2tc/d6502.h"
#include "apple2tc/support.h"

#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <thread>

#include <ncurses.h>
#include <termios.h>
#include <unistd.h>

// ---------------------------------------------------------------------------
// Emulator wrapper (retained for future debug/trace support)
// ---------------------------------------------------------------------------

class Debug6502 : public EmuApple2 {
public:
  DebugState6502 dbg{};
  explicit Debug6502() {
    setDebugStateCB(&dbg, DebugState6502::debugStateCB);
  }
};

// ---------------------------------------------------------------------------
// Display backend interface
// ---------------------------------------------------------------------------

class DisplayBackend {
public:
  virtual ~DisplayBackend() = default;
  virtual void init() = 0;
  virtual void shutdown() = 0;
  /// Non-blocking key read. Returns Apple II key code (0-127), or -1 if none.
  virtual int readKey() = 0;
  virtual void updateScreen(const Emu6502 *emu) = 0;
};

// ---------------------------------------------------------------------------
// Ncurses display — stub (Task 3)
// ---------------------------------------------------------------------------

class NcursesDisplay : public DisplayBackend {
public:
  void init() override {}
  void shutdown() override {}
  int readKey() override { return -1; }
  void updateScreen(const Emu6502 *) override {}
};

// ---------------------------------------------------------------------------
// Stream display — stub (Task 4)
// ---------------------------------------------------------------------------

class StreamDisplay : public DisplayBackend {
public:
  void init() override {}
  void shutdown() override {}
  int readKey() override { return -1; }
  void updateScreen(const Emu6502 *) override {}
};

// ---------------------------------------------------------------------------
// Signal handling
// ---------------------------------------------------------------------------

static DisplayBackend *g_backend = nullptr;

static void signalHandler(int) {
  if (g_backend)
    g_backend->shutdown();
  _exit(1);
}

static void installSignalHandlers() {
  struct sigaction sa{};
  sa.sa_handler = signalHandler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGINT, &sa, nullptr);
  sigaction(SIGTERM, &sa, nullptr);
}

// ---------------------------------------------------------------------------
// Screen dump (for --dump-screen)
// ---------------------------------------------------------------------------

static void dumpTextScreen(const Emu6502 *emu) {
  const uint8_t *page = emu->getMainRAM() + EmuApple2::TXT1SCRN;
  for (unsigned y = 0; y < 24; ++y) {
    unsigned offset = (y % 8) * 128 + (y / 8) * 40;
    for (unsigned x = 0; x < 40; ++x) {
      uint8_t raw = page[offset + x];
      uint8_t ascii = raw & 0x7F;
      fputc((ascii >= 32 && ascii < 127) ? ascii : ' ', stderr);
    }
    fputc('\n', stderr);
  }
}

// ---------------------------------------------------------------------------
// Main loop
// ---------------------------------------------------------------------------

static void runLoop(Debug6502 *emu, DisplayBackend *backend, unsigned maxFrames) {
  static constexpr unsigned CYCLES_20MS = (unsigned)(0.020 * Emu6502::CLOCK_FREQ);

  for (unsigned frame = 0; maxFrames == 0 || frame < maxFrames; ++frame) {
    auto frameStart = std::chrono::steady_clock::now();

    emu->runFor(CYCLES_20MS);

    int key;
    while ((key = backend->readKey()) >= 0)
      a2_io_push_key(emu->io(), key);

    backend->updateScreen(emu);

    auto elapsed = std::chrono::steady_clock::now() - frameStart;
    auto remaining =
        std::chrono::milliseconds(20) -
        std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
    if (remaining.count() > 0)
      std::this_thread::sleep_for(remaining);
  }
}

// ---------------------------------------------------------------------------
// CLI and main
// ---------------------------------------------------------------------------

enum class DisplayMode { Auto, Ncurses, Stream };

int main(int argc, const char **argv) {
  DisplayMode mode = DisplayMode::Auto;
  const char *romPath = nullptr;
  unsigned maxFrames = 0;
  bool dumpScreen = false;

  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "--ncurses") == 0)
      mode = DisplayMode::Ncurses;
    else if (strcmp(argv[i], "--stream") == 0)
      mode = DisplayMode::Stream;
    else if (strncmp(argv[i], "--frames=", 9) == 0)
      maxFrames = atoi(argv[i] + 9);
    else if (strcmp(argv[i], "--dump-screen") == 0)
      dumpScreen = true;
    else if (argv[i][0] == '-') {
      fprintf(stderr, "Unknown option: %s\n", argv[i]);
      fprintf(stderr,
          "Usage: textemu [--ncurses|--stream] [--frames=N] [--dump-screen] [rom-path]\n");
      return 1;
    } else {
      romPath = argv[i];
    }
  }

  if (mode == DisplayMode::Auto)
    mode = isatty(STDOUT_FILENO) ? DisplayMode::Ncurses : DisplayMode::Stream;

  auto emu = std::make_unique<Debug6502>();
  if (romPath) {
    FILE *f = fopen(romPath, "rb");
    if (!f) {
      fprintf(stderr, "Error: can't open %s\n", romPath);
      return 1;
    }
    auto rom = readAll<std::vector<char>>(f);
    fclose(f);
    emu->loadROM((const uint8_t *)rom.data(), rom.size());
  } else {
    emu->loadROM(apple2plus_rom, apple2plus_rom_len);
  }

  std::unique_ptr<DisplayBackend> backend;
  if (mode == DisplayMode::Ncurses)
    backend = std::make_unique<NcursesDisplay>();
  else
    backend = std::make_unique<StreamDisplay>();

  g_backend = backend.get();
  installSignalHandlers();
  backend->init();

  runLoop(emu.get(), backend.get(), maxFrames);

  backend->shutdown();

  if (dumpScreen)
    dumpTextScreen(emu.get());

  return 0;
}
```

**Step 2: Build**

```bash
cmake -G Ninja -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug && ninja -C cmake-build-debug textemu
```

Expected: Builds with zero errors.

**Step 3: Verify CLI parsing**

```bash
./cmake-build-debug/tools/textemu/textemu --help 2>&1; echo "exit: $?"
```

Expected: `Unknown option: --help`, usage line, exit code 1.

**Step 4: Verify --frames causes clean exit**

```bash
./cmake-build-debug/tools/textemu/textemu --stream --frames=10 2>/dev/null; echo "exit: $?"
```

Expected: Exit code 0. Completes in under 1 second (10 frames * 20ms = 200ms).

**Step 5: Verify --dump-screen produces 24 lines on stderr**

```bash
./cmake-build-debug/tools/textemu/textemu --stream --frames=100 --dump-screen 2>screen.txt >/dev/null
wc -l screen.txt
cat screen.txt
```

Expected: `24 screen.txt`. Content is 24 lines of 40 characters. Since backends are stubs the emulator still runs, so screen content depends on the boot ROM — look for non-empty output.

**Step 6: Verify no regressions**

```bash
ninja -C cmake-build-debug && cd tests && bash run-tests.sh ../cmake-build-debug ../cmake-build-debug
```

Expected: `Success!`

**Step 7: Commit**

```bash
git add tools/textemu/textemu.cpp
git commit -m "textemu: scaffold DisplayBackend interface, CLI parsing, test flags"
```

**Completion criteria:** Builds cleanly. `--frames=10` exits in under 1 second with code 0. `--dump-screen` produces exactly 24 lines on stderr. CLI rejects unknown flags. Existing tests pass.

---

### Task 3: Implement NcursesDisplay

**Files:**
- Modify: `tools/textemu/textemu.cpp` (replace `NcursesDisplay` stub)

**Step 1: Implement NcursesDisplay**

Replace the `NcursesDisplay` stub class with:

```cpp
class NcursesDisplay : public DisplayBackend {
  uint8_t shadow_[24][40]{};
  bool firstFrame_ = true;

public:
  void init() override {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);
    memset(shadow_, 0, sizeof(shadow_));
  }

  void shutdown() override {
    endwin();
  }

  int readKey() override {
    int ch = getch();
    if (ch == ERR)
      return -1;
    return translateKey(ch);
  }

  void updateScreen(const Emu6502 *emu) override {
    uint8_t current[24][40];
    struct Ctx {
      uint8_t (*buf)[40];
    } ctx{current};

    apple2_decode_text_screen(
        emu->getMainRAM() + EmuApple2::TXT1SCRN, &ctx,
        [](void *vctx, uint8_t ch, unsigned x, unsigned y) {
          static_cast<Ctx *>(vctx)->buf[y][x] = ch;
        });

    for (unsigned y = 0; y < 24; ++y) {
      for (unsigned x = 0; x < 40; ++x) {
        uint8_t raw = current[y][x];
        if (raw != shadow_[y][x] || firstFrame_) {
          shadow_[y][x] = raw;
          bool inverse = (raw & 0xC0) == 0x00;
          uint8_t ascii = raw & 0x7F;
          char displayCh = (ascii >= 32 && ascii < 127) ? (char)ascii : ' ';
          if (inverse)
            attron(A_REVERSE);
          mvaddch(y, x, displayCh);
          if (inverse)
            attroff(A_REVERSE);
        }
      }
    }

    firstFrame_ = false;
    refresh();
  }

private:
  static int translateKey(int ch) {
    switch (ch) {
    case KEY_BACKSPACE:
    case 127:
    case KEY_LEFT:
      return 8;
    case KEY_RIGHT:
      return 21;
    case KEY_UP:
      return 11;
    case KEY_DOWN:
      return 10;
    case '\n':
    case '\r':
    case KEY_ENTER:
      return '\r';
    case 27:
      return 27;
    default:
      if (ch >= 0 && ch < 128) {
        if (ch >= 'a' && ch <= 'z')
          ch = ch - 'a' + 'A';
        return ch;
      }
      return -1;
    }
  }
};
```

**Step 2: Build**

```bash
ninja -C cmake-build-debug textemu
```

Expected: Zero errors.

**Step 3: Automated test — ncurses boots and dump-screen shows Apple II prompt**

```bash
./cmake-build-debug/tools/textemu/textemu --ncurses --frames=200 --dump-screen </dev/null 2>screen.txt >/dev/null
echo "exit: $?"
cat screen.txt
grep -c ']' screen.txt
```

Expected: Exit code 0. `screen.txt` has 24 lines. `grep` finds at least 1 match for `]` (the Apple II BASIC prompt). This proves: ncurses init/shutdown works, emulator ran 200 frames (~4 seconds of Apple II time), screen content was rendered.

**Step 4: Verify no regressions**

```bash
ninja -C cmake-build-debug && cd tests && bash run-tests.sh ../cmake-build-debug ../cmake-build-debug
```

Expected: `Success!`

**Step 5: Commit**

```bash
git add tools/textemu/textemu.cpp
git commit -m "textemu: implement NcursesDisplay with shadow-buffer diffing"
```

**Completion criteria:** Builds cleanly. `--ncurses --frames=200 --dump-screen` exits cleanly and screen dump contains `]` prompt. Existing tests pass.

---

### Task 4: Implement StreamDisplay

**Files:**
- Modify: `tools/textemu/textemu.cpp` (replace `StreamDisplay` stub)

**Step 1: Implement StreamDisplay**

Replace the `StreamDisplay` stub class with:

```cpp
class StreamDisplay : public DisplayBackend {
  struct termios origTermios_{};
  bool termiosChanged_ = false;

  static constexpr uint16_t ZP_CH = 0x24;
  static constexpr uint16_t ZP_CV = 0x25;

  uint8_t lastCH_ = 0;
  uint8_t lastCV_ = 0;
  bool started_ = false;

public:
  void init() override {
    if (isatty(STDIN_FILENO)) {
      tcgetattr(STDIN_FILENO, &origTermios_);
      struct termios raw = origTermios_;
      raw.c_lflag &= ~(ICANON | ECHO | ISIG);
      raw.c_cc[VMIN] = 0;
      raw.c_cc[VTIME] = 0;
      tcsetattr(STDIN_FILENO, TCSANOW, &raw);
      termiosChanged_ = true;
    }
  }

  void shutdown() override {
    if (termiosChanged_) {
      tcsetattr(STDIN_FILENO, TCSANOW, &origTermios_);
      termiosChanged_ = false;
    }
  }

  int readKey() override {
    unsigned char ch;
    ssize_t n = read(STDIN_FILENO, &ch, 1);
    if (n <= 0)
      return -1;
    if (ch >= 'a' && ch <= 'z')
      ch = ch - 'a' + 'A';
    if (ch == '\n')
      ch = '\r';
    if (ch == 127)
      ch = 8;
    return ch;
  }

  void updateScreen(const Emu6502 *emu) override {
    uint8_t ch = emu->ram_peek(ZP_CH);
    uint8_t cv = emu->ram_peek(ZP_CV);

    if (!started_) {
      lastCH_ = ch;
      lastCV_ = cv;
      started_ = true;
      return;
    }

    if (cv == lastCV_ && ch == lastCH_)
      return;

    if (cv < lastCV_ || (cv == lastCV_ && ch < lastCH_)) {
      putchar('\n');
      fflush(stdout);
      lastCH_ = ch;
      lastCV_ = cv;
      return;
    }

    const uint8_t *textPage = emu->getMainRAM() + EmuApple2::TXT1SCRN;
    uint8_t curCV = lastCV_;
    uint8_t curCH = lastCH_;

    while (curCV < cv || (curCV == cv && curCH < ch)) {
      unsigned offset = (curCV % 8) * 128 + (curCV / 8) * 40 + curCH;
      uint8_t raw = textPage[offset];
      uint8_t ascii = raw & 0x7F;
      putchar((ascii >= 32 && ascii < 127) ? ascii : ' ');

      curCH++;
      if (curCH >= 40) {
        curCH = 0;
        curCV++;
        putchar('\n');
      }
    }

    fflush(stdout);
    lastCH_ = ch;
    lastCV_ = cv;
  }
};
```

**Step 2: Build**

```bash
ninja -C cmake-build-debug textemu
```

Expected: Zero errors.

**Step 3: Automated test — stream mode produces boot output**

```bash
OUTPUT=$(./cmake-build-debug/tools/textemu/textemu --stream --frames=200 </dev/null 2>/dev/null)
echo "Got ${#OUTPUT} bytes"
echo "$OUTPUT" | head -5
```

Expected: Non-zero byte count. Output contains recognizable Apple II boot text.

**Step 4: Automated test — stream mode dump-screen shows prompt**

```bash
./cmake-build-debug/tools/textemu/textemu --stream --frames=200 --dump-screen </dev/null 2>screen.txt >/dev/null
grep -c ']' screen.txt
```

Expected: At least 1 match (the `]` BASIC prompt).

**Step 5: Automated test — piped keyboard input produces expected output**

```bash
printf 'PRINT "HELLO"\r' | ./cmake-build-debug/tools/textemu/textemu --stream --frames=500 --dump-screen 2>screen.txt
cat screen.txt
grep -c 'HELLO' screen.txt
```

Expected: `grep` finds at least 1 match — the `PRINT` command's output visible in the screen dump.

**Step 6: Automated test — auto-detection selects stream when stdout is piped**

```bash
BYTES=$(./cmake-build-debug/tools/textemu/textemu --frames=200 </dev/null 2>/dev/null | wc -c)
test "$BYTES" -gt 0 && echo "PASS: auto-detect stream ($BYTES bytes)" || echo "FAIL"
```

Expected: `PASS`.

**Step 7: Verify no regressions**

```bash
ninja -C cmake-build-debug && cd tests && bash run-tests.sh ../cmake-build-debug ../cmake-build-debug
```

Expected: `Success!`

**Step 8: Commit**

```bash
git add tools/textemu/textemu.cpp
git commit -m "textemu: implement StreamDisplay with cursor-following output"
```

**Completion criteria:** Builds cleanly. Stream mode produces output with `--frames`. Piped input with `PRINT "HELLO"` shows `HELLO` in dump-screen. Auto-detection works. Existing tests pass. All verification is automated.

---

### Task 5: Final verification

All tests are automated. Run the full suite.

**Step 1: Full build**

```bash
cmake -G Ninja -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug && ninja -C cmake-build-debug
```

Expected: Zero errors.

**Step 2: Existing tests**

```bash
cd tests && bash run-tests.sh ../cmake-build-debug
```

Expected: `Success!`

**Step 3: All textemu automated tests**

```bash
BIN=./cmake-build-debug/tools/textemu/textemu
PASS=0; FAIL=0

# Test 1: --frames exits cleanly
$BIN --stream --frames=10 </dev/null >/dev/null 2>/dev/null; CODE=$?
test "$CODE" -eq 0 && { echo "PASS: --frames clean exit"; PASS=$((PASS+1)); } || { echo "FAIL: exit code $CODE"; FAIL=$((FAIL+1)); }

# Test 2: --dump-screen produces 24 lines
$BIN --stream --frames=100 --dump-screen </dev/null >/dev/null 2>screen.txt; LINES=$(wc -l < screen.txt)
test "$LINES" -eq 24 && { echo "PASS: dump-screen 24 lines"; PASS=$((PASS+1)); } || { echo "FAIL: got $LINES lines"; FAIL=$((FAIL+1)); }

# Test 3: ncurses boots without crash
$BIN --ncurses --frames=200 --dump-screen </dev/null >/dev/null 2>screen.txt; CODE=$?
test "$CODE" -eq 0 && { echo "PASS: ncurses no crash"; PASS=$((PASS+1)); } || { echo "FAIL: exit code $CODE"; FAIL=$((FAIL+1)); }

# Test 4: ncurses dump-screen has prompt
grep -q ']' screen.txt && { echo "PASS: ncurses has ] prompt"; PASS=$((PASS+1)); } || { echo "FAIL: no ] prompt"; FAIL=$((FAIL+1)); }

# Test 5: stream mode produces stdout output
BYTES=$($BIN --stream --frames=200 </dev/null 2>/dev/null | wc -c)
test "$BYTES" -gt 0 && { echo "PASS: stream output ($BYTES bytes)"; PASS=$((PASS+1)); } || { echo "FAIL: no output"; FAIL=$((FAIL+1)); }

# Test 6: stream dump-screen has prompt
$BIN --stream --frames=200 --dump-screen </dev/null 2>screen.txt >/dev/null
grep -q ']' screen.txt && { echo "PASS: stream has ] prompt"; PASS=$((PASS+1)); } || { echo "FAIL: no ] prompt"; FAIL=$((FAIL+1)); }

# Test 7: piped input works
printf 'PRINT "HELLO"\r' | $BIN --stream --frames=500 --dump-screen 2>screen.txt >/dev/null
grep -q 'HELLO' screen.txt && { echo "PASS: PRINT HELLO works"; PASS=$((PASS+1)); } || { echo "FAIL: no HELLO"; FAIL=$((FAIL+1)); }

# Test 8: auto-detection (piped stdout = stream mode)
BYTES=$($BIN --frames=200 </dev/null 2>/dev/null | wc -c)
test "$BYTES" -gt 0 && { echo "PASS: auto-detect stream ($BYTES bytes)"; PASS=$((PASS+1)); } || { echo "FAIL: no output"; FAIL=$((FAIL+1)); }

# Test 9: unknown flag rejected
$BIN --bogus 2>/dev/null; CODE=$?
test "$CODE" -eq 1 && { echo "PASS: unknown flag rejected"; PASS=$((PASS+1)); } || { echo "FAIL: exit code $CODE"; FAIL=$((FAIL+1)); }

echo ""
echo "Results: $PASS passed, $FAIL failed"
rm -f screen.txt
test "$FAIL" -eq 0
```

Expected: `9 passed, 0 failed`. Exit code 0.

**Step 4: Commit only if there were changes**

```bash
git diff --quiet tools/textemu/ || (git add tools/textemu/textemu.cpp && git commit -m "textemu: final cleanup")
```

**Completion criteria:** Full build succeeds. Existing tests pass. All 9 automated textemu tests pass.

---

### Task 6: Update design doc with final state

**Files:**
- Modify: `docs/plans/2026-03-02-textemu-design.md`

**Step 1: Add "Implemented" status to design doc header**

Add a line after the title: `**Status:** Implemented`

**Step 2: Commit**

```bash
git add docs/plans/2026-03-02-textemu-design.md
git commit -m "docs: mark textemu design as implemented"
```

**Completion criteria:** Design doc updated. Commit succeeds.
