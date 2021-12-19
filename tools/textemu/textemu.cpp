/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "apple2tc/DebugState6502.h"
#include "apple2tc/a2io.h"
#include "apple2tc/a2symbols.h"
#include "apple2tc/apple2.h"
#include "apple2tc/d6502.h"
#include "apple2tc/support.h"

#include <cstdio>
#include <cstdlib>
#include <memory>
#include <thread>

class Debug6502 : public EmuApple2 {
public:
  DebugState6502 dbg{};

  explicit Debug6502() {
    setDebugStateCB(&dbg, DebugState6502::debugStateCB);
  }
};

static std::vector<char> readAll(const char *path) {
  FILE *f = fopen(path, "rb");
  if (!f) {
    fprintf(stderr, "***ERROR: can't open %s", path);
    exit(1);
  }
  auto res = readAll<std::vector<char>>(f);
  fclose(f);
  return res;
}

static void clearScreen() {
  printf("\x1b[2J\x1b[0;0H");
  fflush(stdout);
}

static void drawTextScreen(const Emu6502 *emu) {
  unsigned pageStart = EmuApple2::TXT1SCRN;
  apple2_decode_text_screen(
      emu->getMainRAM() + EmuApple2::TXT1SCRN,
      nullptr,
      [](void *, uint8_t ch, unsigned x, unsigned y) {
        if (x == 0)
          printf("%02u: ", y);
        ch &= 0x7F;
        putchar(ch >= 32 ? ch : ' ');
        if (x == 39)
          putchar('\n');
      });
}

static void consumeLine() {
  int ch;
  while ((ch = getchar()) != EOF && ch != '\n') {
  }
}

static void runLoop(Debug6502 *emu) {
  // CPU Freq: 1023000 Hz
  // cycle: 1/1023000 seconds
  // 0.020 / (1/1023000) <=> 0.020 * 1023000 <=> 20 * 1023
  static constexpr unsigned CYCLES_20MS = (unsigned)(0.020 * Emu6502::CLOCK_FREQ);

  unsigned lastDisplay = emu->getCycles();

  for (;;) {
    // TODO: This is very quick and dirty. We should really measure how much actual
    //       time has passed, how many actual cycles, and adjust.
    emu->runFor(CYCLES_20MS);
    std::this_thread::sleep_for(std::chrono::duration<unsigned, std::milli>(20));

    if (emu->getCycles() - lastDisplay > Emu6502::CLOCK_FREQ) {
      lastDisplay = emu->getCycles();
      if (emu->getDebugFlags() & Emu6502::DebugStdout)
        drawTextScreen(emu);

      if (emu->getDebugFlags() & Emu6502::DebugKbdin) {
        printf("> ");
        int ch;
        switch (getchar()) {
        case EOF:
          return;
        case '\n':
          break;
        case 'a':
          dumpApplesoftBasic(stdout, emu);
          emu->setDebugFlags(Emu6502::DebugKbdin | Emu6502::DebugASM);
          while ((ch = getchar()) != EOF && ch != '\n')
            a2_io_push_key(emu->io(), ch);
          a2_io_push_key(emu->io(), '\r');
          break;
        case 'r':
          consumeLine();
          emu->reset();
          break;
        case ':':
          while ((ch = getchar()) != EOF && ch != '\n')
            a2_io_push_key(emu->io(), ch);
          a2_io_push_key(emu->io(), '\r');
          break;
        case '`':
          a2_io_push_key(emu->io(), 27);
          while ((ch = getchar()) != EOF && ch != '\n') {
            a2_io_push_key(emu->io(), ch);
          }
          break;

        default:
          consumeLine();
          printf("INVALID COMMAND\n");
          break;
        }
      }
    }
  }
}

int main(int argc, const char **argv) {
  auto emu = std::make_unique<Debug6502>();
  auto rom = readAll(argc < 2 ? "rom/apple2plus.rom" : argv[1]);
  emu->loadROM((const uint8_t *)rom.data(), rom.size());
  // emu->addDebugFlags(Emu6502::DebugASM);
  emu->addDebugFlags(Emu6502::DebugKbdin);
  emu->addDebugFlags(Emu6502::DebugStdout);
  emu->dbg.addDefaultNonDebug();
  // emu->addWatch("FRETOP", 0x6F, 2);

  runLoop(emu.get());

  return 0;
}