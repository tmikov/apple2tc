/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
#include "apple2tc/apple2.h"

#include "apple2tc/a2symbols.h"
#include "apple2tc/d6502.h"

#include <cstdio>
#include <cstdlib>
#include <memory>
#include <thread>

class Debug6502 : public EmuApple2 {
public:
  /// Set maximum number of instructions to execute. 0 means unlimited.
  void setLimit(unsigned limit) {
    limit_ = limit;
  }

  /// Add a watch to be printed during debugging.
  void addWatch(std::string name, uint16_t addr, uint8_t size);
  /// Remove a watch.
  void removeWatch(const char *name);
  /// Execution in the following area will not be debugged. The range is
  /// inclusive.
  void addNonDebug(uint16_t from, uint16_t to);

protected:
  StopReason debugState() override;

private:
  /// Number of instruction to execute.
  unsigned limit_ = 0;
  /// Current instruction number executing.
  unsigned icount_ = 0;

  /// A memory location to be printed during debugging.
  struct Watch {
    std::string name;
    uint16_t addr;
    uint8_t size;

    Watch(std::string &&name, uint16_t addr, uint8_t size)
        : name(std::move(name)), addr(addr), size(size) {}
  };

  /// All memory watches.
  std::vector<Watch> watches_;
  /// Memory areas where we don't print debugging info. The ranges are
  /// inclusive.
  std::vector<std::pair<uint16_t, uint16_t>> nonDebug_;
};

void Debug6502::addWatch(std::string name, uint16_t addr, uint8_t size) {
  auto it = std::find_if(
      watches_.begin(), watches_.end(), [&name](const Watch &w) { return w.name == name; });

  if (it != watches_.end()) {
    it->addr = addr;
    it->size = size;
  } else {
    watches_.emplace_back(std::move(name), addr, size);
  }
}

void Debug6502::removeWatch(const char *name) {
  auto it = std::find_if(
      watches_.begin(), watches_.end(), [&name](const Watch &w) { return w.name == name; });
  if (it != watches_.end())
    watches_.erase(it);
}

void Debug6502::addNonDebug(uint16_t from, uint16_t to) {
  nonDebug_.emplace_back(from, to);
}

Emu6502::StopReason Debug6502::debugState() {
  Regs r = getRegs();

  // Don't debug in areas that have been excluded.
  for (auto p : nonDebug_) {
    if (r.pc >= p.first && r.pc <= p.second)
      return StopReason::None;
  }

  if (limit_ && icount_ >= limit_)
    return StopReason::StopRequesed;
  ++icount_;

  // Address.
  {
    const char *name = findApple2Symbol(r.pc);
    printf("%04X: %-8s  ", r.pc, name ? name : "");
  }

  // Dump the registers.
  printf("A=%02X X=%02X Y=%02X SP=%02X SR=", r.a, r.x, r.y, r.sp);
  // Dump the flags.
  static const char names[9] = "NV.BDIZC";
  for (unsigned i = 0; i != 8; ++i)
    putchar((r.status & (0x80 >> i)) ? names[i] : '.');

  // Dump the next instruction.
  ThreeBytes bytes;
  for (unsigned i = 0; i != 3; ++i)
    bytes.d[i] = peek(r.pc + i);
  auto inst = decodeInst(r.pc, bytes);
  auto fmt = formatInst(inst, bytes, apple2SymbolResolver);
  printf("  %-8s    %s", fmt.bytes, fmt.inst);
  if (fmt.operand[0]) {
    printf("  %s", fmt.operand.c_str());
    if (inst.addrMode == CPUAddrMode::Rel)
      printf(" (%d)", (int8_t)bytes.d[1]);
  }
  printf("\n");

  // Dump watches
  for (const auto &watch : watches_) {
    printf("  %-8s", watch.name.c_str());
    if (watch.addr < 256)
      printf("($%02X)  = ", watch.addr);
    else
      printf("($%04X)= ", watch.addr);
    if (watch.size == 1)
      printf("$%02X (%u)\n", peek(watch.addr), peek(watch.addr));
    else
      printf("$%04X (%u)\n", peek16(watch.addr), peek16(watch.addr));
  }

  return StopReason::None;
}

static std::vector<char> readAll(FILE *f) {
  std::vector<char> buf;
  static constexpr unsigned CHUNK = 8192;
  for (;;) {
    size_t size = buf.size();
    buf.resize(size + CHUNK);
    size_t nr = fread(&buf[size], 1, CHUNK, f);
    buf.resize(size + nr);
    if (nr != CHUNK)
      break;
  }
  return buf;
}

static std::vector<char> readAll(const char *path) {
  FILE *f = fopen(path, "rb");
  if (!f) {
    fprintf(stderr, "***ERROR: can't open %s", path);
    exit(1);
  }
  auto res = readAll(f);
  fclose(f);
  return res;
}

static void clearScreen() {
  printf("\x1b[2J\x1b[0;0H");
  fflush(stdout);
}

static void drawTextScreen(const Emu6502 *emu) {
  unsigned pageStart = EmuApple2::TXT1SCRN;
  apple2DecodeTextScreen(
      emu, EmuApple2::TXT1SCRN, nullptr, [](void *, uint8_t ch, unsigned x, unsigned y) {
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
            emu->pushKey(ch);
          emu->pushKey('\r');
          break;
        case 'r':
          consumeLine();
          emu->reset();
          break;
        case ':':
          while ((ch = getchar()) != EOF && ch != '\n')
            emu->pushKey(ch);
          emu->pushKey('\r');
          break;
        case '`':
          emu->pushKey(27);
          while ((ch = getchar()) != EOF && ch != '\n') {
            emu->pushKey(ch);
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
  emu->addNonDebug(0xFCA8, 0xFCB3); // MONWAIT
  emu->addNonDebug(0xFD0C, 0xFD3C), // Keyboard
                                    // emu->addWatch("FRETOP", 0x6F, 2);

      runLoop(emu.get());

  return 0;
}