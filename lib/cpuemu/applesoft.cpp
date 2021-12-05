#include "apple2tc/apple2.h"

void dumpApplesoftBasic(FILE *f, const EmuApple2 *emu) {
  uint16_t start = emu->ram_peek16(EmuApple2::TXTTAB);
  uint16_t end = emu->ram_peek16(EmuApple2::PRGEND);

  fprintf(f, "[%04X..%04X]", start, end);
  for (uint16_t addr = start; addr && addr < end; ++addr)
    fprintf(f, " $%02X", emu->ram_peek(addr));
  fprintf(f, "\n");
}
