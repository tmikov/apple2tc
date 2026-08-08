/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

/// \file
/// An `a2engine.h` engine backed by the `Emu6502` interpreter.
///
/// The counterpart to the C that apple2tc generates: same contract, same host,
/// a completely different way of executing 6502. Which one a program gets is
/// decided at link time.
///
/// It carries a little more than the generated engine does, because the
/// generated engine has its program baked in at decompile time and this one
/// does not: loading a ROM and a `.b33`, mounting disks, and collecting the
/// run data that apple2tc later consumes. Those are the engine's own command
/// line options, reached through `engine_parse_arg`.
///
/// For the same reason it uses `a2host_api.h` as well as the narrow
/// `a2host.h`: mounting a disk and pushing keystrokes need the IO state
/// itself, not just `io_peek`/`io_poke`. Generated code never needs that, which
/// is why the narrow contract stays narrow.

#include "apple2tc/a2engine.h"
#include "apple2tc/a2host.h"
#include "apple2tc/a2host_api.h"

#include "apple2tc/DebugState6502.h"
#include "apple2tc/a2io.h"
#include "apple2tc/apple2plus_rom.h"
#include "apple2tc/emu6502.h"
#include "apple2tc/support.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace {

/// An Emu6502 whose IO goes to the host rather than to an `a2_iostate_t` of its
/// own. There is exactly one IO state in the process and the host owns it,
/// because the host is what renders it, replays keys into it and hashes it.
class HostedEmu : public Emu6502 {
public:
  HostedEmu() : Emu6502(0xC000, 0xCFFF) {}

protected:
  uint8_t ioPeek(uint16_t addr) override {
    return io_peek(addr);
  }
  void ioPoke(uint16_t addr, uint8_t value) override {
    io_poke(addr, value);
  }
};

HostedEmu s_emu;
DebugState6502 s_dbg;
a2_stop_reason_t s_stopReason = A2_STOP_CYCLES;

std::string s_romPath;
std::string s_runPath;
std::string s_disk1Path;
std::string s_disk2Path;
std::string s_outPath;
unsigned s_limit = 0;
bool s_collect = false;
/// Collect from the reset vector rather than from the loaded program, so the
/// trace covers the ROM boot the program is entered from.
bool s_fromRom = false;

std::vector<uint8_t> readFileOrDie(const std::string &path) {
  FILE *f = fopen(path.c_str(), "rb");
  if (!f) {
    perror(path.c_str());
    exit(2);
  }
  auto data = readAll<std::vector<uint8_t>>(f);
  fclose(f);
  return data;
}

/// Load a DOS 3.3 binary: two bytes of load address, two of length, then the
/// image. Returns the load address.
std::optional<uint16_t> loadB33(const std::vector<uint8_t> &data) {
  if (data.size() > 4) {
    uint16_t start = data[0] + data[1] * 256;
    if (data.size() - 4 <= 0x10000u - start) {
      memcpy(s_emu.getMainRAMWritable() + start, data.data() + 4, data.size() - 4);
      fprintf(stderr, "Loaded %zu bytes at $%04X (%u)\n", data.size() - 4, start, start);
      return start;
    }
  }
  fprintf(stderr, "%s: invalid b33 format\n", s_runPath.c_str());
  return std::nullopt;
}

void mountDisk(const std::string &path, int drive) {
  auto data = readFileOrDie(path);
  if (!a2_disk2_mount(&a2host_io()->disk2, drive, data.data(), data.size())) {
    fprintf(stderr, "Failed to mount disk%d: %s (bad format or size)\n", drive + 1, path.c_str());
    exit(2);
  }
  fprintf(stderr, "Mounted disk%d: %s\n", drive + 1, path.c_str());
}

void startCollecting() {
  s_emu.addDebugFlags(Emu6502::DebugASM);
  s_dbg.setModeCollect(&s_emu, s_limit);
}

/// Write out what was collected. Called when the engine stops itself, which is
/// how the collection limit reports completion.
void finishCollecting() {
  if (s_dbg.getMode() != DebugState6502::Mode::Collect)
    return;

  fflush(stdout);
  std::ofstream of;
  std::ostream *os = &std::cout;
  if (!s_outPath.empty()) {
    of.open(s_outPath, std::ios_base::out);
    if (!of) {
      perror(s_outPath.c_str());
      exit(2);
    }
    os = &of;
  }
  s_dbg.finishCollection(&s_emu, *os);
  s_dbg.clearCollectedData();
  os->flush();
  fprintf(stderr, "Collection written to %s\n", s_outPath.empty() ? "stdout" : s_outPath.c_str());

  s_emu.setDebugFlags(s_emu.getDebugFlags() & ~Emu6502::DebugASM);
  s_dbg.setModeNone();
}

/// The program is loaded once the ROM has finished booting, at the warm-restart
/// entry. Loading it any earlier would have BASIC's initialisation overwrite
/// it.
void onWarmRestart() {
  auto addr = loadB33(readFileOrDie(s_runPath));
  if (!addr)
    exit(2);

  if (s_fromRom) {
    // Enter it the way a person would, so the trace includes the boot.
    char buf[32];
    snprintf(buf, sizeof(buf), "CALL %u\r", *addr);
    a2_io_push_str(a2host_io(), buf);
  } else {
    auto r = s_emu.getRegs();
    r.pc = *addr;
    r.status = Emu6502::STATUS_IGNORED;
    s_emu.setRegs(r);
    fprintf(stderr, "Executing at $%04X\n", *addr);
  }

  if (s_collect && !s_fromRom)
    startCollecting();
}

/// Called at every instruction once Emu6502::DebugASM is on. Fans out to the
/// host's tracer and to the collector, either of which may be inactive.
Emu6502::StopReason debugCB(void *ctx, Emu6502 *emu, uint16_t pc) {
  if (g_debug & DebugASM)
    debug_asm(pc);
  return DebugState6502::debugStateCB(ctx, emu, pc);
}

} // namespace

/* --- a2engine.h ----------------------------------------------------------- */

uint8_t g_debug = 0;

void reset_regs(void) {
  s_emu.reset();
}

void set_regs(regs_t r) {
  s_emu.setRegs(Emu6502::Regs{
      .pc = r.pc, .a = r.a, .x = r.x, .y = r.y, .status = r.status, .sp = r.sp});
}

regs_t get_regs(void) {
  auto r = s_emu.getRegs();
  return regs_t{.pc = r.pc, .a = r.a, .x = r.x, .y = r.y, .status = r.status, .sp = r.sp};
}

unsigned get_cycles(void) {
  return s_emu.getCycles();
}

const uint8_t *get_ram(void) {
  return s_emu.getMainRAM();
}

void ram_poke(uint16_t addr, uint8_t value) {
  s_emu.ram_poke(addr, value);
}

uint8_t ram_peek(uint16_t addr) {
  return s_emu.ram_peek(addr);
}

uint16_t ram_peek16(uint16_t addr) {
  return (uint16_t)(s_emu.ram_peek(addr) + s_emu.ram_peek((uint16_t)(addr + 1)) * 256);
}

void init_emulated(void) {
  if (s_romPath.empty()) {
    s_emu.loadROM(apple2plus_rom, apple2plus_rom_len);
  } else {
    auto rom = readFileOrDie(s_romPath);
    s_emu.loadROM(rom.data(), rom.size());
  }

  if (!s_disk1Path.empty())
    mountDisk(s_disk1Path, 0);
  if (!s_disk2Path.empty())
    mountDisk(s_disk2Path, 1);
  // The CPU fetches instructions straight out of RAM, bypassing ioPeek(), so
  // the boot ROM has to actually be there.
  if (!s_disk1Path.empty() || !s_disk2Path.empty())
    a2_disk2_install_rom(s_emu.getMainRAMWritable());

  s_emu.setDebugStateCB(&s_dbg, debugCB);
  // Collecting wants a complete picture; tracing does not want the keyboard and
  // delay loops.
  if (!s_collect)
    s_dbg.addDefaultNonDebug();
  s_dbg.setResolveApple2Symbols(false);

  if (g_debug & DebugASM)
    s_emu.addDebugFlags(Emu6502::DebugASM);

  if (s_collect && s_fromRom)
    startCollecting();

  if (!s_runPath.empty()) {
    s_emu.addDebugFlags(Emu6502::DebugASM);
    s_dbg.setBreakpoint(0xD43C); // Warm restart, i.e. the BASIC prompt.
    s_dbg.setBreakpointCB([](uint16_t addr) {
      s_dbg.clearBreakpoint(addr);
      s_dbg.setBreakpointCB({});
      onWarmRestart();
      return Emu6502::StopReason::None;
    });
  }
}

void run_emulated(unsigned run_cycles) {
  auto reason = s_emu.runFor(run_cycles);
  if (reason == Emu6502::StopReason::StopRequesed) {
    s_stopReason = A2_STOP_REQUESTED;
    finishCollecting();
  } else {
    s_stopReason = A2_STOP_CYCLES;
  }
}

void shutdown_emulated(void) {
  // A run that ends on the frame limit rather than the collection limit still
  // has data worth keeping.
  finishCollecting();
}

a2_stop_reason_t engine_stop_reason(void) {
  return s_stopReason;
}

bool engine_parse_arg(const char *arg) {
  auto prefix = [arg](const char *p, std::string *out) {
    size_t n = strlen(p);
    if (strncmp(arg, p, n) != 0)
      return false;
    *out = arg + n;
    return true;
  };

  if (prefix("--rom=", &s_romPath))
    return true;
  if (prefix("--disk1=", &s_disk1Path))
    return true;
  if (prefix("--disk2=", &s_disk2Path))
    return true;
  if (prefix("--out=", &s_outPath))
    return true;
  if (strcmp(arg, "--collect") == 0) {
    s_collect = true;
    return true;
  }
  if (strcmp(arg, "--from-rom") == 0) {
    s_fromRom = true;
    return true;
  }
  if (strncmp(arg, "--limit=", 8) == 0) {
    s_limit = (unsigned)strtoul(arg + 8, nullptr, 10);
    return true;
  }
  // A bare argument is the program to run.
  if (arg[0] != '-' && s_runPath.empty()) {
    s_runPath = arg;
    return true;
  }
  return false;
}

void engine_print_help(void) {
  printf("\n 6502 interpreter engine:\n");
  printf(" <file>           DOS3.3 binary to load and run\n");
  printf(" --rom=path       Use this ROM instead of the built-in Apple II+\n");
  printf(" --disk1=path     Mount a disk image in drive 1\n");
  printf(" --disk2=path     Mount a disk image in drive 2\n");
  printf(" --from-rom       Enter the program via CALL from BASIC, so a\n");
  printf("                  collection covers the ROM boot as well\n");
  printf(" --collect        Collect run data for apple2tc\n");
  printf(" --limit=n        Stop collecting after n basic blocks\n");
  printf(" --out=path       Write collected data here instead of stdout\n");
}
