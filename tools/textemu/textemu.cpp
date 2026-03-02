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
      putchar((ascii >= 32 && ascii < 127) ? ascii : ' ');
    }
    putchar('\n');
  }
}

// ---------------------------------------------------------------------------
// Main loop
// ---------------------------------------------------------------------------

static void
runLoop(Debug6502 *emu, DisplayBackend *backend, unsigned maxFrames, bool suppressDisplay) {
  static constexpr unsigned CYCLES_20MS = (unsigned)(0.020 * Emu6502::CLOCK_FREQ);

  for (unsigned frame = 0; maxFrames == 0 || frame < maxFrames; ++frame) {
    auto frameStart = std::chrono::steady_clock::now();

    emu->runFor(CYCLES_20MS);

    int key;
    while ((key = backend->readKey()) >= 0)
      a2_io_push_key(emu->io(), key);

    if (!suppressDisplay)
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

  if (!dumpScreen)
    backend->init();

  runLoop(emu.get(), backend.get(), maxFrames, dumpScreen);

  if (!dumpScreen)
    backend->shutdown();

  if (dumpScreen)
    dumpTextScreen(emu.get());

  return 0;
}
