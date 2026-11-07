#include "apple2tc/a2io.h"
#include <stdio.h>
#include <string.h>

// "Language Card" area Switches
// Bank 1 and Bank 2 here are the 4K banks at $D000-$DFFF. The
// remaining area from $E000-$FFFF is the same for both
// sets of switches.

//            $C080 ;LC RAM bank2, Read and WR-protect RAM
// ROMIN =    $C081 ;LC RAM bank2, Read ROM instead of RAM,
//                  ;two or more successive reads WR-enables RAM
//            $C082 ;LC RAM bank2, Read ROM instead of RAM,
//                  ;WR-protect RAM
// LCBANK2 =  $C083 ;LC RAM bank2, Read RAM
//                  ;two or more successive reads WR-enables RAM
//            $C084-$C087 are echoes of $C080-$C083
//            $C088 ;LC RAM bank1, Read and WR-protect RAM
//            $C089 ;LC RAM bank1, Read ROM instead of RAM,
//                  ;two or more successive reads WR-enables RAM
//            $C08A ;LC RAM bank1, Read ROM instead of RAM,
//                  ;WR-protect RAM
// LCBANK1 =  $C08B ;LC RAM bank1, Read RAM
//                  ;two or more successive reads WR-enables RAM
//            $C08C-$C08F are echoes of $C088-$C08B

// Encoding for the LSBs above
// v      v- Bank 1/2 v- Read ROM? v- Write Protect 
// 0x00   2           N            Y
// 0x01   2           Y            N  
// 0x10   2           Y            Y 
// 0x11   2           N            N
// 1x00   1           N            Y
// 1x01   1           Y            N
// 1x10   1           Y            Y
// 1x11   1           Y            N

void a2_language_card_init(a2_language_card_t *lc) {
  memset(lc->bank1_ram, 0xFF, sizeof(lc->bank1_ram));
  memset(lc->bank2_ram, 0xFF, sizeof(lc->bank2_ram));
  memset(lc->high_ram, 0xFF, sizeof(lc->high_ram));
  memset(lc->rom, 0xFF, sizeof(lc->rom));
  lc->banked_memory_read_rom = true;
  lc->banked_memory_ram_select = A2_LC_RAM2;
  lc->banked_memory_write_protect = true;
  lc->visible_memory_dirty = true;
}

void a2_language_card_load_rom(a2_language_card_t *lc, const uint8_t *rom, uint16_t rom_size) {
  if (rom_size > sizeof(lc->rom)) {
    fprintf(stderr, "a2_language_card_load_rom: ROM size %u exceeds maximum %zu\n", rom_size, sizeof(lc->rom));
    return;
  }
  // Load into highest addresses of the ROM area
  uint16_t rom_start = sizeof(lc->rom) - rom_size;
  memcpy(lc->rom + rom_start, rom, rom_size);
  lc->visible_memory_dirty = true;
}

uint8_t a2_language_card_peek(a2_language_card_t *lc, unsigned offset) {
  if (lc->banked_memory_read_rom) {
    return lc->rom[offset];
  }
  if (offset >= sizeof(lc->bank1_ram)) {
    return lc->high_ram[offset - sizeof(lc->bank1_ram)];
  }
  switch (lc->banked_memory_ram_select) {
  case A2_LC_RAM1:
    return lc->bank1_ram[offset];
  case A2_LC_RAM2:
    return lc->bank2_ram[offset];
  }
  fprintf(stderr, "a2_language_card_peek: invalid banked_memory_ram_select %d\n", lc->banked_memory_ram_select);
}

void a2_language_card_poke(a2_language_card_t *lc, unsigned offset, uint8_t value) {
  if (lc->banked_memory_write_protect) {
    fprintf(stderr, "a2_language_card_poke: write to protected banked memory at offset 0x%04X, value = 0x%02X\n", offset, value);
    return;
  }
  if (offset >= sizeof(lc->bank1_ram)) {
    lc->high_ram[offset - sizeof(lc->bank1_ram)] = value;
    lc->visible_memory_dirty = true;
    return;
  }
  switch (lc->banked_memory_ram_select) {
  case A2_LC_RAM1:
    lc->bank1_ram[offset] = value;
    break;
  case A2_LC_RAM2:
    lc->bank2_ram[offset] = value;
    break;
  default:
    fprintf(stderr, "a2_language_card_poke: write to invalid write source %d banked memory at offset 0x%04X, value = 0x%02X\n", lc->banked_memory_ram_select, offset, value);
    break;
  }
  lc->visible_memory_dirty = true;
}

void a2_language_card_sync_ram(a2_language_card_t *lc, uint8_t *ram) {
  if (!lc->visible_memory_dirty)
    return;
  for (unsigned offset = 0; offset < 0x3000; ++offset)
    ram[0xD000 + offset] = a2_language_card_peek(lc, offset);
  lc->visible_memory_dirty = false;
}

// See truth table at the top of this file for how the banked memory is selected and write protected.
void a2_language_card_decode_peek(a2_language_card_t *lc, uint16_t addr, uint16_t prev_addr) {
  const bool prev_read_rom = lc->banked_memory_read_rom;
  const a2_banked_memory_ram_select_t prev_bank = lc->banked_memory_ram_select;

  // Write source is in the MSB of the address
  lc->banked_memory_ram_select = (addr & 0x0008) ? A2_LC_RAM1 : A2_LC_RAM2;
  if (!(addr & 0x0001)) {
    lc->banked_memory_write_protect = true;
  } else if (addr == prev_addr) {
    lc->banked_memory_write_protect = false;
  }
  // Read source ROM is true if bit 1 ^ bit 0 == 1, else RAM
  lc->banked_memory_read_rom = ((addr & 0x0002) >> 1) ^ (addr & 0x0001);

  if (lc->banked_memory_read_rom != prev_read_rom || lc->banked_memory_ram_select != prev_bank)
    lc->visible_memory_dirty = true;
}