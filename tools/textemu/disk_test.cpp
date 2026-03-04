/*
 * Minimal disk boot test — no display, just traces disk2 activity and dumps state.
 */
#include "apple2tc/a2io.h"
#include "apple2tc/apple2.h"
#include "apple2tc/apple2plus_rom.h"
#include "apple2tc/support.h"

// Include private disk tables for round-trip testing.
#include "../../lib/a2io/disk2_tables.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>

int main(int argc, const char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: disk_test <disk.dsk> [cycles]\n");
    return 1;
  }

  unsigned runCycles = 500000; // ~0.5 sec of emulated time
  if (argc >= 3)
    runCycles = atoi(argv[2]);

  // Quick GCR round-trip test.
  {
    fprintf(stderr, "\n--- GCR round-trip test ---\n");
    // Verify write→read round-trip for all 64 values.
    int mismatches = 0;
    for (int v = 0; v < 64; v++) {
      uint8_t disk_byte = gcr_write_translate[v];
      uint8_t decoded = gcr_read_translate[disk_byte];
      if (decoded != v) {
        fprintf(stderr, "  TABLE MISMATCH: v=%02X disk_byte=%02X decoded=%02X\n", v, disk_byte, decoded);
        mismatches++;
      }
    }
    fprintf(stderr, "  Table round-trip: %d mismatches\n", mismatches);

    // Full nibblize→denibblize round-trip, replicating the boot ROM's decode algorithm.
    uint8_t test_sector[256];
    for (int i = 0; i < 256; i++)
      test_sector[i] = i; // 0x00..0xFF

    // --- ENCODE (same as write_data_field in disk2.c) ---
    uint8_t nib_buf[342];
    memset(nib_buf, 0, sizeof(nib_buf));
    for (int i = 0; i < 256; i++) {
      int sec_idx = i % 86;
      int shift = (i / 86) * 2;
      uint8_t low2 = test_sector[i] & 0x03;
      uint8_t reversed = ((low2 & 1) << 1) | ((low2 >> 1) & 1);
      nib_buf[sec_idx] |= reversed << shift;
    }
    for (int i = 0; i < 256; i++)
      nib_buf[86 + i] = test_sector[i] >> 2;

    // XOR-encode and GCR translate.
    uint8_t encoded[343]; // 342 data + 1 checksum
    uint8_t prev = 0;
    for (int i = 0; i < 342; i++) {
      uint8_t val = nib_buf[i] ^ prev;
      prev = nib_buf[i];
      encoded[i] = gcr_write_translate[val & 0x3F];
    }
    encoded[342] = gcr_write_translate[prev & 0x3F]; // checksum

    // --- DECODE (replicate boot ROM algorithm exactly) ---
    // Step 1: XOR-decode using our translate table (same as boot ROM's EOR $02D6,Y).
    // Secondary bytes stored in reverse order: sec_mem[Y] gets the Y'th decoded byte,
    // but stored at decreasing addresses (Y=$55 down to $00).
    uint8_t sec_mem[86]; // simulates $0300-$0355
    uint8_t acc = 0;
    for (int i = 0; i < 86; i++) {
      acc ^= gcr_read_translate[encoded[i]];
      sec_mem[85 - i] = acc; // boot ROM stores at $0300+(85-i) effectively
    }

    // Primary bytes stored in forward order.
    uint8_t pri_mem[256];
    for (int i = 0; i < 256; i++) {
      acc ^= gcr_read_translate[encoded[86 + i]];
      pri_mem[i] = acc;
    }

    // Checksum.
    acc ^= gcr_read_translate[encoded[342]];
    if (acc != 0)
      fprintf(stderr, "  CHECKSUM ERROR: %02X\n", acc);
    else
      fprintf(stderr, "  Checksum OK\n");

    // Step 2: Combine primary + secondary (replicate boot ROM's LSR/ROL loop).
    // X cycles 85,84,...,1,0,reset,85,84,... with cycle length 86.
    uint8_t result[256];
    for (int y = 0; y < 256; y++) {
      int x = 85 - (y % 86);
      if (x < 0)
        x += 86;
      uint8_t a = pri_mem[y];
      // First pair: LSR sec_mem[x] → carry; ROL A ← carry
      uint8_t carry = sec_mem[x] & 1;
      sec_mem[x] >>= 1;
      a = (a << 1) | carry;
      // Second pair:
      carry = sec_mem[x] & 1;
      sec_mem[x] >>= 1;
      a = (a << 1) | carry;
      result[y] = a;
    }

    // Compare result with test_sector.
    int diffs = 0;
    for (int i = 0; i < 256; i++) {
      if (result[i] != test_sector[i]) {
        if (diffs < 10)
          fprintf(stderr, "  DIFF at %d: expected %02X got %02X (xor %02X)\n",
              i, test_sector[i], result[i], test_sector[i] ^ result[i]);
        diffs++;
      }
    }
    fprintf(stderr, "  Full round-trip: %d diffs (of 256)\n", diffs);
  }

  // Load disk image.
  FILE *f = fopen(argv[1], "rb");
  if (!f) {
    perror(argv[1]);
    return 2;
  }
  auto dsk = readAll<std::vector<uint8_t>>(f);
  fclose(f);
  fprintf(stderr, "Disk image: %s (%zu bytes)\n", argv[1], dsk.size());

  // Create emulator with Apple II+ ROM.
  auto emu = std::make_unique<EmuApple2>();
  emu->loadROM(apple2plus_rom, apple2plus_rom_len);

  // Mount disk.
  if (!a2_disk2_mount(&emu->io()->disk2, 0, dsk.data(), dsk.size())) {
    fprintf(stderr, "Failed to mount disk\n");
    return 2;
  }

  // Install boot ROM into RAM for instruction fetch.
  a2_disk2_install_rom(emu->getMainRAMWritable());

  // Dump first 16 nibblized bytes from track 0 to verify nibblization.
  {
    a2_disk2_drive_t *drv = &emu->io()->disk2.drive[0];
    fprintf(stderr, "Track 0 nibble data (first 64 bytes):\n");
    for (int i = 0; i < 64; i++) {
      fprintf(stderr, "%02X ", drv->nib_data[i]);
      if ((i & 15) == 15)
        fprintf(stderr, "\n");
    }
  }

  // Debug callback to verify combining loop correctness.
  struct TraceCtx {
    int combine_count = 0;
    Emu6502 *emu = nullptr;
    const uint8_t *dsk = nullptr;
    size_t dsk_size = 0;
    // Ring buffer of last N PCs before BRK.
    uint16_t pc_trace[32] = {};
    uint8_t a_trace[32] = {};
    uint8_t x_trace[32] = {};
    uint8_t y_trace[32] = {};
    int trace_idx = 0;
    bool brk_captured = false;
  } traceCtx;
  traceCtx.emu = emu.get();
  traceCtx.dsk = dsk.data();
  traceCtx.dsk_size = dsk.size();

  emu->addDebugFlags(Emu6502::DebugASM);
  emu->setDebugStateCB(&traceCtx, [](void *ctx, Emu6502 *e, uint16_t pc) -> Emu6502::StopReason {
    auto *tc = static_cast<TraceCtx *>(ctx);
    // $C6D5 = start of combining loop (LDY #$00), fires once per sector read.
    if (pc == 0xC6D5 && tc->combine_count < 12) {
      uint16_t target_page = e->ram_peek(0x27);
      uint16_t target_addr = e->ram_peek(0x26) + (target_page << 8);
      uint8_t phys_sector = e->ram_peek(0x3D);
      int logical = dos33_phys_to_logical[phys_sector & 0x0F];

      fprintf(stderr, "\n*** COMBINE #%d: page $%02X, phys=%d, logical=%d ***\n",
          tc->combine_count, target_page, phys_sector, logical);

      // Capture primary and secondary before combining modifies them.
      uint8_t primary[256], secondary[86];
      for (int i = 0; i < 256; i++)
        primary[i] = e->ram_peek(target_addr + i);
      for (int i = 0; i < 86; i++)
        secondary[i] = e->ram_peek(0x0300 + i);

      // Manually combine using the boot ROM's algorithm.
      uint8_t manual_result[256];
      uint8_t sec_copy[86];
      memcpy(sec_copy, secondary, 86);
      for (int y = 0; y < 256; y++) {
        // X cycling: 85, 84, ..., 0, reset to 85, 84, ...
        int x;
        if (y < 86) x = 85 - y;
        else if (y < 172) x = 85 - (y - 86);
        else x = 85 - (y - 172);

        uint8_t a = primary[y];
        uint8_t carry = sec_copy[x] & 1;
        sec_copy[x] >>= 1;
        a = (a << 1) | carry;
        carry = sec_copy[x] & 1;
        sec_copy[x] >>= 1;
        a = (a << 1) | carry;
        manual_result[y] = a;
      }

      // Compare manual result with DSK.
      const uint8_t *dsk_sector = tc->dsk + logical * 256;
      int manual_diffs = 0, manual_bit7 = 0;
      for (int i = 0; i < 256; i++) {
        if (manual_result[i] != dsk_sector[i]) {
          manual_diffs++;
          if ((manual_result[i] ^ dsk_sector[i]) == 0x80) manual_bit7++;
        }
      }

      // Compare primary bytes with expected (DSK >> 2).
      int pri_diffs = 0;
      for (int i = 0; i < 256; i++) {
        if (primary[i] != (dsk_sector[i] >> 2)) {
          pri_diffs++;
          if (pri_diffs <= 3)
            fprintf(stderr, "    PRI DIFF [%d]: got %02X expected %02X (src=%02X)\n",
                i, primary[i], dsk_sector[i] >> 2, dsk_sector[i]);
        }
      }

      fprintf(stderr, "    Primary vs DSK>>2: %d diffs\n", pri_diffs);
      fprintf(stderr, "    Manual combine vs DSK: %d diffs (%d bit7-only)\n",
          manual_diffs, manual_bit7);

      tc->combine_count++;
    }

    // Record trace for all PCs.
    tc->pc_trace[tc->trace_idx % 32] = pc;
    auto trace_regs = e->getRegs();
    tc->a_trace[tc->trace_idx % 32] = trace_regs.a;
    tc->x_trace[tc->trace_idx % 32] = trace_regs.x;
    tc->y_trace[tc->trace_idx % 32] = trace_regs.y;
    tc->trace_idx++;

    // Track RWTS address field scanning.
    static int addr_scan_count = 0;
    static int addr_timeout_count = 0;
    static int addr_success_count = 0;
    static int data_scan_count = 0;
    // Watch $0478 for changes.
    static uint8_t last_0478 = 0xFF;
    static int watch_0478_count = 0;
    {
      uint8_t cur = e->ram_peek(0x0478);
      if (cur != last_0478 && watch_0478_count < 30) {
        fprintf(stderr, "*** $0478 CHANGED: %d → %d at PC=$%04X ***\n",
            last_0478, cur, pc);
        last_0478 = cur;
        watch_0478_count++;
      }
    }
    // $3E60: MYSEEK entry - shows how it's called (carry flag, stack).
    static int myseek_count = 0;
    if (pc == 0x3E60 && myseek_count < 5) {
      auto regs = e->getRegs();
      // PLA hasn't happened yet, so A still has old value.
      // Stack top has the value that PLA will pop.
      uint8_t stack_top = e->ram_peek(0x100 + regs.sp + 1);
      uint8_t stack_2nd = e->ram_peek(0x100 + regs.sp + 2);
      uint8_t stack_3rd = e->ram_peek(0x100 + regs.sp + 3);
      // IOB pointer is at $3C-$3D.
      uint16_t iob_addr = e->ram_peek(0x3C) | (e->ram_peek(0x3D) << 8);
      uint8_t iob_byte1 = e->ram_peek(iob_addr + 1);
      fprintf(stderr, "\n*** MYSEEK #%d: A=$%02X C=%d stack=[%02X %02X %02X] SP=$%02X IOB=$%04X ***\n",
          myseek_count, regs.a, (regs.status & 1), stack_top, stack_2nd, stack_3rd, regs.sp, iob_addr);
      fprintf(stderr, "    IOB bytes: ");
      for (int i = 0; i < 16; i++)
        fprintf(stderr, "%02X ", e->ram_peek(iob_addr + i));
      fprintf(stderr, "\n");
      fprintf(stderr, "    IOB[1]=$%02X bit0=%d (half-track flag)\n", iob_byte1, iob_byte1 & 1);
      myseek_count++;
    }
    // $39A0: seek routine entry.
    static int seek_count = 0;
    if (pc == 0x39A0 && seek_count < 10) {
      auto regs = e->getRegs();
      a2_disk2_t *d = &static_cast<EmuApple2 *>(e)->io()->disk2;
      a2_disk2_drive_t *drv = &d->drive[d->selected_drive];
      fprintf(stderr, "\n*** SEEK #%d: target=%d X=$%02X $0478=%d pp=%d (phys trk %d) ***\n",
          seek_count, regs.a, regs.x, e->ram_peek(0x0478),
          d->phase_position, drv->current_track);
      seek_count++;
    }
    // $39FC: seek routine RTS.
    static int seek_exit_count = 0;
    if (pc == 0x39FC && seek_exit_count < 10) {
      a2_disk2_t *d = &static_cast<EmuApple2 *>(e)->io()->disk2;
      a2_disk2_drive_t *drv = &d->drive[d->selected_drive];
      fprintf(stderr, "*** SEEK EXIT #%d: $0478=%d pp=%d (phys trk %d) ***\n",
          seek_exit_count, e->ram_peek(0x0478),
          d->phase_position, drv->current_track);
      seek_exit_count++;
    }
    // $3944: address field scanner entry.
    if (pc == 0x3944 && addr_scan_count < 10) {
      a2_disk2_t *d = &static_cast<EmuApple2 *>(e)->io()->disk2;
      a2_disk2_drive_t *drv = &d->drive[d->selected_drive];
      fprintf(stderr, "\n*** ADDR SCAN #%d: trk=%d pos=%d $0478=%d ***\n",
          addr_scan_count, drv->current_track, drv->position, e->ram_peek(0x0478));
      addr_scan_count++;
    }
    // $3942: SEC; RTS = timeout/error return from address/data scanner.
    if (pc == 0x3942 && addr_timeout_count < 10) {
      a2_disk2_t *d = &static_cast<EmuApple2 *>(e)->io()->disk2;
      a2_disk2_drive_t *drv = &d->drive[d->selected_drive];
      fprintf(stderr, "*** ADDR/DATA TIMEOUT #%d: trk=%d pos=%d ***\n",
          addr_timeout_count, drv->current_track, drv->position);
      addr_timeout_count++;
    }
    // $399E: CLC before RTS = address scanner SUCCESS.
    if (pc == 0x399E && addr_success_count < 20) {
      // $002E = track, $002D = sector from address field.
      uint8_t addr_trk = e->ram_peek(0x2E);
      uint8_t addr_sec = e->ram_peek(0x2D);
      uint8_t addr_vol = e->ram_peek(0x2F);
      // Show return address from stack.
      auto regs = e->getRegs();
      uint16_t ret_addr = e->ram_peek(0x100 + regs.sp + 1) |
                          (e->ram_peek(0x100 + regs.sp + 2) << 8);
      // Show IOB and expected values.
      uint8_t iob_trk = e->ram_peek(0x0478); // Current track for slot 6
      fprintf(stderr, "*** ADDR SUCCESS #%d: vol=%d trk=%d sec=%d | $0478=%d ret=$%04X ***\n",
          addr_success_count, addr_vol, addr_trk, addr_sec, iob_trk, ret_addr + 1);
      addr_success_count++;
    }
    // $3DC8: instruction after JSR to address scanner returns.
    // Dump state to understand why data scanner is not called.
    static int post_addr_count = 0;
    if (pc == 0x3DC7 && post_addr_count < 5) {
      auto regs = e->getRegs();
      uint8_t addr_trk = e->ram_peek(0x2E);
      uint8_t addr_sec = e->ram_peek(0x2D);
      // What does the code compare against?
      // Dump bytes at $3DC8-$3DE0 to see opcodes.
      fprintf(stderr, "*** POST-ADDR #%d: PC=$%04X A=$%02X X=$%02X Y=$%02X C=%d ***\n",
          post_addr_count, pc, regs.a, regs.x, regs.y, (regs.status & 1));
      fprintf(stderr, "    $002C-$002F (cksum/sec/trk/vol): %02X %02X %02X %02X\n",
          e->ram_peek(0x2C), e->ram_peek(0x2D), e->ram_peek(0x2E), e->ram_peek(0x2F));
      // Dump code at $3D80-$3E00 (full read routine context).
      fprintf(stderr, "    Code $3D80: ");
      for (int i = 0; i < 128; i++) {
        fprintf(stderr, "%02X ", e->ram_peek(0x3D80 + i));
        if ((i & 31) == 31) fprintf(stderr, "\n               ");
      }
      fprintf(stderr, "\n");
      // Dump seek routine opcode at $39A4.
      fprintf(stderr, "    Seek $39A0: ");
      for (int i = 0; i < 16; i++)
        fprintf(stderr, "%02X ", e->ram_peek(0x39A0 + i));
      fprintf(stderr, "\n");
      // Dump code around the $0478 corruption sites.
      fprintf(stderr, "    Code $3E60: ");
      for (int i = 0; i < 48; i++)
        fprintf(stderr, "%02X ", e->ram_peek(0x3E60 + i));
      fprintf(stderr, "\n");
      // Check various track locations.
      fprintf(stderr, "    $0478=%d $047E=%d $04D8=%d $002A=%d $002B=%02X\n",
          e->ram_peek(0x0478), e->ram_peek(0x047E), e->ram_peek(0x04D8),
          e->ram_peek(0x002A), e->ram_peek(0x002B));
      post_addr_count++;
    }
    // $38DC: data field scanner entry.
    if (pc == 0x38DC && data_scan_count < 10) {
      a2_disk2_t *d = &static_cast<EmuApple2 *>(e)->io()->disk2;
      a2_disk2_drive_t *drv = &d->drive[d->selected_drive];
      fprintf(stderr, "*** DATA SCAN #%d: trk=%d pos=%d ***\n",
          data_scan_count, drv->current_track, drv->position);
      data_scan_count++;
    }

    // Capture instruction trace before BRK at $3900.
    if (pc == 0x3900 && !tc->brk_captured) {
      tc->brk_captured = true;
      fprintf(stderr, "\n*** BRK at $3900 detected! ***\n");
      fprintf(stderr, "    Regs: A=$%02X X=$%02X Y=$%02X SP=$%02X\n",
          trace_regs.a, trace_regs.x, trace_regs.y, trace_regs.sp);
      fprintf(stderr, "    Last %d instructions:\n", 32);
      for (int i = 32; i > 0; i--) {
        int idx = (tc->trace_idx - i) % 32;
        if (idx < 0) idx += 32;
        fprintf(stderr, "      PC=$%04X A=$%02X X=$%02X Y=$%02X\n",
            tc->pc_trace[idx], tc->a_trace[idx], tc->x_trace[idx], tc->y_trace[idx]);
      }
      // Dump stack.
      fprintf(stderr, "    Stack ($01%02X-$01FF): ", trace_regs.sp + 1);
      for (int i = trace_regs.sp + 1; i <= 0xFF; i++)
        fprintf(stderr, "%02X ", e->ram_peek(0x100 + i));
      fprintf(stderr, "\n");
    }

    // $C6EB = INC $27 (right after combining loop exits).
    // At this point, the combined result is in the target page.
    if (pc == 0xC6EB) {
      uint16_t target_page = e->ram_peek(0x27);
      uint16_t target_addr = e->ram_peek(0x26) + (target_page << 8);
      uint8_t phys_sector = e->ram_peek(0x3D);
      int logical = dos33_phys_to_logical[phys_sector & 0x0F];
      const uint8_t *dsk_sector = tc->dsk + logical * 256;

      int post_diffs = 0, post_bit7 = 0;
      for (int i = 0; i < 256; i++) {
        uint8_t actual = e->ram_peek(target_addr + i);
        if (actual != dsk_sector[i]) {
          post_diffs++;
          if ((actual ^ dsk_sector[i]) == 0x80) post_bit7++;
          if (post_diffs <= 3)
            fprintf(stderr, "    POST-COMBINE DIFF [%d]: got $%02X expected $%02X (xor $%02X)\n",
                i, actual, dsk_sector[i], actual ^ dsk_sector[i]);
        }
      }
      if (post_diffs > 0)
        fprintf(stderr, "    POST-COMBINE page $%02X: %d diffs (%d bit7-only)\n",
            target_page, post_diffs, post_bit7);
    }

    return Emu6502::StopReason::None;
  });

  // Run in chunks, printing disk state periodically.
  unsigned totalCycles = 0;
  unsigned chunk = 20460; // ~20ms
  bool brk_detected = false;
  // Track first time PC enters key regions.
  bool seen_3700 = false, seen_B700 = false, seen_monitor = false;
  while (totalCycles < runCycles) {
    unsigned before = emu->getCycles();
    emu->runFor(chunk);
    unsigned after = emu->getCycles();
    totalCycles += (after - before);

    auto regs = emu->getRegs();
    a2_disk2_t *d = &emu->io()->disk2;
    a2_disk2_drive_t *drv = &d->drive[d->selected_drive];

    // Detect key PC regions.
    if (!seen_3700 && regs.pc >= 0x3600 && regs.pc < 0x4000) {
      seen_3700 = true;
      fprintf(stderr, "*** FIRST entry to $3600-$3FFF at cycle %u: PC=$%04X\n", totalCycles, regs.pc);
    }
    if (!seen_B700 && regs.pc >= 0xB600 && regs.pc < 0xC000) {
      seen_B700 = true;
      fprintf(stderr, "*** FIRST entry to $B600-$BFFF at cycle %u: PC=$%04X\n", totalCycles, regs.pc);
    }

    // Detect when PC enters GETLN/RDKEY area (keyboard wait) after boot.
    // This indicates BRK happened and Monitor is waiting for input.
    if (!seen_monitor && totalCycles > 1000000 && regs.pc >= 0xFD00 && regs.pc < 0xFD30) {
      seen_monitor = true;
      fprintf(stderr, "*** KEYBOARD WAIT at cycle %u: PC=$%04X A=$%02X X=$%02X Y=$%02X SP=$%02X\n",
          totalCycles, regs.pc, regs.a, regs.x, regs.y, regs.sp);
      // Dump the full stack to see return chain.
      fprintf(stderr, "    Stack ($01%02X-$01FF): ", regs.sp + 1);
      for (int i = regs.sp + 1; i <= 0xFF; i++)
        fprintf(stderr, "%02X ", emu->ram_peek(0x100 + i));
      fprintf(stderr, "\n");
      // Decode return addresses from stack.
      fprintf(stderr, "    Return chain: ");
      for (int i = regs.sp + 1; i < 0xFF; i += 2) {
        uint16_t ret = emu->ram_peek(0x100 + i) | (emu->ram_peek(0x100 + i + 1) << 8);
        fprintf(stderr, "$%04X ", ret + 1);
      }
      fprintf(stderr, "\n");
      // Check Monitor's saved BRK address at $3A-$3B.
      fprintf(stderr, "    BRK addr ($3A-$3B): $%02X%02X\n",
          emu->ram_peek(0x3B), emu->ram_peek(0x3A));
      brk_detected = true;
    }

    // Only print periodic status every 500 chunks to reduce noise.
    if (totalCycles % (chunk * 500) < chunk) {
      fprintf(stderr, "[%u] PC=$%04X A=$%02X X=$%02X Y=$%02X SP=$%02X | "
                      "motor=%d drv=%d trk=%d pos=%d q6=%d q7=%d phases=0x%X\n",
          totalCycles, regs.pc, regs.a, regs.x, regs.y, regs.sp,
          d->motor_on, d->selected_drive, drv->current_track, drv->position,
          d->q6, d->q7, d->phases);
    }
  }

  // Dump text screen.
  fprintf(stderr, "\n--- Text screen ---\n");
  const uint8_t *page = emu->getMainRAM() + EmuApple2::TXT1SCRN;
  for (unsigned y = 0; y < 24; y++) {
    unsigned offset = (y % 8) * 128 + (y / 8) * 40;
    for (unsigned x = 0; x < 40; x++) {
      uint8_t raw = page[offset + x];
      uint8_t ascii = raw & 0x7F;
      fputc((ascii >= 32 && ascii < 127) ? ascii : '.', stderr);
    }
    fputc('\n', stderr);
  }

  // Dump zero-page locations used by boot ROM.
  fprintf(stderr, "\n--- Zero page (boot ROM state) ---\n");
  fprintf(stderr, "$26-$27 (data ptr): $%02X%02X\n", emu->ram_peek(0x27), emu->ram_peek(0x26));
  fprintf(stderr, "$2B (slot*16):      $%02X\n", emu->ram_peek(0x2B));
  fprintf(stderr, "$3C (temp):         $%02X\n", emu->ram_peek(0x3C));
  fprintf(stderr, "$3D (sector count): $%02X\n", emu->ram_peek(0x3D));
  fprintf(stderr, "$40 (track):        $%02X\n", emu->ram_peek(0x40));
  fprintf(stderr, "$41 (target trk):   $%02X\n", emu->ram_peek(0x41));

  // Dump full RWTS code ($3800-$3A10).
  fprintf(stderr, "\n--- $3800-$3A10 (full RWTS) ---\n");
  for (int i = 0; i < 0x210; i++) {
    if (i % 16 == 0) fprintf(stderr, "$%04X: ", 0x3800 + i);
    fprintf(stderr, "%02X ", emu->ram_peek(0x3800 + i));
    if ((i & 15) == 15) fprintf(stderr, "\n");
  }

  // Dump RWTS code around the BRK jump.
  fprintf(stderr, "\n--- $3640-$3660 (RWTS around BRK jump) ---\n");
  for (int i = 0; i < 0x20; i++) {
    if (i % 16 == 0) fprintf(stderr, "$%04X: ", 0x3640 + i);
    fprintf(stderr, "%02X ", emu->ram_peek(0x3640 + i));
    if ((i & 15) == 15) fprintf(stderr, "\n");
  }
  // Dump RWTS entry area.
  fprintf(stderr, "\n--- $3800-$3830 (RWTS read code) ---\n");
  for (int i = 0; i < 0x30; i++) {
    if (i % 16 == 0) fprintf(stderr, "$%04X: ", 0x3800 + i);
    fprintf(stderr, "%02X ", emu->ram_peek(0x3800 + i));
    if ((i & 15) == 15) fprintf(stderr, "\n");
  }
  // Dump $38D0-$3910.
  fprintf(stderr, "\n--- $38D0-$3910 (RWTS prologue scan) ---\n");
  for (int i = 0; i < 0x40; i++) {
    if (i % 16 == 0) fprintf(stderr, "$%04X: ", 0x38D0 + i);
    fprintf(stderr, "%02X ", emu->ram_peek(0x38D0 + i));
    if ((i & 15) == 15) fprintf(stderr, "\n");
  }

  // Check if $0800 was loaded (boot sector destination).
  fprintf(stderr, "\n--- $0800-$080F (boot sector) ---\n");
  for (int i = 0; i < 16; i++)
    fprintf(stderr, "%02X ", emu->ram_peek(0x0800 + i));
  fprintf(stderr, "\n");

  // Dump memory around $3900 (where BRK occurred).
  fprintf(stderr, "\n--- $38F0-$3910 (around BRK at $3902) ---\n");
  for (int i = 0; i < 0x30; i++) {
    if (i % 16 == 0) fprintf(stderr, "$%04X: ", 0x38F0 + i);
    fprintf(stderr, "%02X ", emu->ram_peek(0x38F0 + i));
    if ((i & 15) == 15) fprintf(stderr, "\n");
  }

  // Check if DOS loaded to $9600-$9610.
  fprintf(stderr, "\n--- $9600-$960F (DOS start) ---\n");
  for (int i = 0; i < 16; i++)
    fprintf(stderr, "%02X ", emu->ram_peek(0x9600 + i));
  fprintf(stderr, "\n");

  // Check $B600-$B60F (RWTS area).
  fprintf(stderr, "\n--- $B600-$B60F (RWTS) ---\n");
  for (int i = 0; i < 16; i++)
    fprintf(stderr, "%02X ", emu->ram_peek(0xB600 + i));
  fprintf(stderr, "\n");

  // Count non-zero bytes in DOS area.
  int nonzero = 0;
  for (int i = 0x9600; i < 0xC000; i++)
    if (emu->ram_peek(i)) nonzero++;
  fprintf(stderr, "\nNon-zero bytes in $9600-$BFFF: %d / %d\n", nonzero, 0xC000 - 0x9600);

  // Count non-zero bytes around $3900.
  nonzero = 0;
  for (int i = 0x3800; i < 0x4000; i++)
    if (emu->ram_peek(i)) nonzero++;
  fprintf(stderr, "Non-zero bytes in $3800-$3FFF: %d / %d\n", nonzero, 0x4000 - 0x3800);

  // Compare boot ROM's GCR translate table (at $02D6+byte) with our gcr_read_translate.
  // The boot ROM builds this table at $0356+X during the first part of the boot ROM.
  // For valid disk bytes ($96-$FF), the table is at $02D6+$96=$036C through $02D6+$FF=$03D5.
  // This region is NOT overwritten by the secondary buffer ($0300-$0355).
  {
    fprintf(stderr, "\n--- Boot ROM translate table vs ours ---\n");
    int tbl_diffs = 0;
    for (int db = 0x96; db <= 0xFF; db++) {
      uint8_t rom_val = emu->ram_peek(0x02D6 + db);
      uint8_t our_val = gcr_read_translate[db];
      if (rom_val != our_val) {
        fprintf(stderr, "  TRANSLATE DIFF: disk_byte=$%02X rom=%02X ours=%02X\n", db, rom_val, our_val);
        tbl_diffs++;
      }
    }
    fprintf(stderr, "  Translate table: %d diffs (of %d valid disk bytes)\n", tbl_diffs, 0xFF - 0x96 + 1);
  }

  // Verify nibblization: for each loaded page, find best matching DSK sector.
  fprintf(stderr, "\n--- Nibblization verification (pages $36-$3F vs ALL DSK sectors) ---\n");
  // Also dump the boot sector's interleave table.
  fprintf(stderr, "  Boot sector interleave table ($084D-$085C): ");
  for (int i = 0; i < 16; i++)
    fprintf(stderr, "%02X ", emu->ram_peek(0x084D + i));
  fprintf(stderr, "\n");
  // Dump the 2nd-stage boot code entry.
  fprintf(stderr, "  $0800-$0810: ");
  for (int i = 0; i < 17; i++)
    fprintf(stderr, "%02X ", emu->ram_peek(0x0800 + i));
  fprintf(stderr, "\n");

  for (int page = 0; page < 10; page++) {
    uint16_t mem_base = 0x3600 + page * 256;
    // Compare against all 16 sectors on track 0 AND direct DSK offset.
    int best_sector = -1, best_diffs = 257;
    for (int s = 0; s < 16; s++) {
      int diffs = 0;
      for (int i = 0; i < 256; i++) {
        if (dsk[s * 256 + i] != emu->ram_peek(mem_base + i))
          diffs++;
      }
      if (diffs < best_diffs) {
        best_diffs = diffs;
        best_sector = s;
      }
    }
    // Check if diffs are all just bit 7.
    int direct_diffs = 0, bit7_only_diffs = 0, other_diffs = 0;
    for (int i = 0; i < 256; i++) {
      uint8_t expected = dsk[page * 256 + i];
      uint8_t actual = emu->ram_peek(mem_base + i);
      if (expected != actual) {
        direct_diffs++;
        if ((expected ^ actual) == 0x80)
          bit7_only_diffs++;
        else
          other_diffs++;
      }
    }
    fprintf(stderr, "  Page $%02X: best=DSK %d (%d diffs), vs DSK %d: %d diffs (%d bit7-only, %d other)\n",
        0x36 + page, best_sector, best_diffs, page, direct_diffs, bit7_only_diffs, other_diffs);
    if (other_diffs > 0 && other_diffs <= 5) {
      for (int i = 0; i < 256; i++) {
        uint8_t expected = dsk[page * 256 + i];
        uint8_t actual = emu->ram_peek(mem_base + i);
        if (expected != actual && (expected ^ actual) != 0x80)
          fprintf(stderr, "    non-bit7 diff at +$%02X: expected $%02X got $%02X (xor $%02X)\n",
              i, expected, actual, expected ^ actual);
      }
    }
  }

  // Decode nibble data directly from buffer and compare with DSK.
  {
    fprintf(stderr, "\n--- Direct nibble decode test (track 0, all sectors) ---\n");
    a2_disk2_drive_t *drv = &emu->io()->disk2.drive[0];
    uint8_t *trk = drv->nib_data; // track 0 nibble data
    int pos = 0;
    int total_decode_diffs = 0;

    // Scan through the track looking for data fields.
    while (pos < DISK2_NIB_TRACK_SIZE - 400) {
      // Look for address prologue D5 AA 96.
      if (trk[pos] != 0xD5 || trk[pos + 1] != 0xAA || trk[pos + 2] != 0x96) {
        pos++;
        continue;
      }
      // Found address prologue at pos.
      int addr_start = pos;
      pos += 3;
      // Decode 4-and-4: volume, track, sector, checksum.
      uint8_t vol = ((trk[pos] << 1) | 1) & trk[pos + 1];
      pos += 2;
      uint8_t trk_num = ((trk[pos] << 1) | 1) & trk[pos + 1];
      pos += 2;
      uint8_t sec = ((trk[pos] << 1) | 1) & trk[pos + 1];
      pos += 2;
      pos += 2; // skip checksum
      pos += 3; // skip epilogue DE AA EB

      // Now look for data prologue D5 AA AD.
      int max_search = pos + 20;
      while (pos < max_search && (trk[pos] != 0xD5 || trk[pos + 1] != 0xAA || trk[pos + 2] != 0xAD))
        pos++;
      if (pos >= max_search) {
        fprintf(stderr, "  Sector %d: data prologue not found\n", sec);
        continue;
      }
      pos += 3; // skip D5 AA AD

      // Decode 343 GCR bytes (342 data + 1 checksum).
      uint8_t decoded[342];
      uint8_t acc = 0;
      for (int i = 0; i < 342; i++) {
        acc ^= gcr_read_translate[trk[pos++]];
        decoded[i] = acc;
      }
      uint8_t cksum_byte = gcr_read_translate[trk[pos++]];
      acc ^= cksum_byte;
      pos += 3; // skip epilogue

      // decoded[0..85] = secondary (stored reversed as boot ROM does).
      // decoded[86..341] = primary.
      // Simulate boot ROM's storage: sec_mem[85-i] = decoded[i] for secondary.
      uint8_t sec_mem[86];
      for (int i = 0; i < 86; i++)
        sec_mem[85 - i] = decoded[i];

      // Combine primary + secondary (boot ROM LSR/ROL).
      uint8_t result[256];
      for (int y = 0; y < 256; y++) {
        int x = 85 - (y % 86);
        uint8_t a = decoded[86 + y]; // primary
        uint8_t carry = sec_mem[x] & 1;
        sec_mem[x] >>= 1;
        a = (a << 1) | carry;
        carry = sec_mem[x] & 1;
        sec_mem[x] >>= 1;
        a = (a << 1) | carry;
        result[y] = a;
      }

      // For sector 0, compare secondary buffer with emulator's $0300-$0355.
      if (sec == 0) {
        fprintf(stderr, "  Phys sector 0: direct decode secondary ($0300=nib[85]...$0355=nib[0]):\n    ");
        for (int i = 0; i < 16; i++)
          fprintf(stderr, "%02X ", sec_mem[i]); // sec_mem[0] = nib_buf[85]... wait
        fprintf(stderr, "\n");
        // sec_mem was stored as sec_mem[85-i] = decoded[i] = nib_buf[i]
        // So sec_mem[0] = decoded[85] = nib_buf[85], sec_mem[85] = decoded[0] = nib_buf[0]
        // This matches the boot ROM: $0300 = nib_buf[85], $0355 = nib_buf[0]
        fprintf(stderr, "    nib_buf[0..15]: ");
        for (int i = 0; i < 16; i++)
          fprintf(stderr, "%02X ", decoded[i]); // decoded[i] = nib_buf[i]
        fprintf(stderr, "\n");
        fprintf(stderr, "    nib_buf[80..85]: ");
        for (int i = 80; i < 86; i++)
          fprintf(stderr, "%02X ", decoded[i]);
        fprintf(stderr, "\n");

        // Compare sec_mem with emulator's $0300-$0355
        int sec_diffs = 0;
        for (int i = 0; i < 86; i++) {
          uint8_t emu_val = emu->ram_peek(0x0300 + i);
          // After combining, the emulator's $0300+i has been shifted.
          // We can't compare directly since combining modifies sec_mem.
          // But we captured the COMBINE #0 secondary before combining.
          // Let me just print our computed secondary.
        }
      }

      // Compare with DSK data.
      int logical = dos33_phys_to_logical[sec];
      int diffs = 0, bit7_diffs = 0;
      for (int i = 0; i < 256; i++) {
        uint8_t expected = dsk[logical * 256 + i];
        if (result[i] != expected) {
          diffs++;
          if ((result[i] ^ expected) == 0x80)
            bit7_diffs++;
        }
      }
      if (diffs > 0) {
        fprintf(stderr, "  Phys sector %d (DSK logical %d): %d diffs (%d bit7-only), cksum=%s\n",
            sec, logical, diffs, bit7_diffs, acc == 0 ? "OK" : "FAIL");
      }
      total_decode_diffs += diffs;
    }
    if (total_decode_diffs == 0)
      fprintf(stderr, "  All sectors decode correctly from nibble buffer!\n");
    else
      fprintf(stderr, "  Total decode diffs: %d\n", total_decode_diffs);
  }

  // Search for JMP $3900 (4C 00 39) and JSR $3900 (20 00 39) in memory.
  fprintf(stderr, "\n--- Searching for JMP/JSR $3900 ---\n");
  for (int addr = 0; addr < 0xC000; addr++) {
    uint8_t b0 = emu->ram_peek(addr);
    uint8_t b1 = emu->ram_peek(addr + 1);
    uint8_t b2 = emu->ram_peek(addr + 2);
    if ((b0 == 0x4C || b0 == 0x20) && b1 == 0x00 && b2 == 0x39)
      fprintf(stderr, "  $%04X: %02X 00 39 (%s $3900)\n", addr, b0, b0 == 0x4C ? "JMP" : "JSR");
    // Also check for indirect JMP vectors: any word containing $3900.
    if (b0 == 0x00 && b1 == 0x39)
      fprintf(stderr, "  $%04X: 00 39 (vector to $3900)\n", addr);
  }
  // Also search ROM area ($D000-$FFFF).
  fprintf(stderr, "--- Searching ROM for JMP/JSR $3900 ---\n");
  for (int addr = 0xD000; addr < 0xFFFC; addr++) {
    uint8_t b0 = emu->ram_peek(addr);
    uint8_t b1 = emu->ram_peek(addr + 1);
    uint8_t b2 = emu->ram_peek(addr + 2);
    if ((b0 == 0x4C || b0 == 0x20) && b1 == 0x00 && b2 == 0x39)
      fprintf(stderr, "  $%04X: %02X 00 39 (%s $3900)\n", addr, b0, b0 == 0x4C ? "JMP" : "JSR");
  }

  // Compare $38xx with $B8xx to check if RWTS was loaded to wrong location.
  fprintf(stderr, "\n--- $B8F0-$B920 (RWTS for comparison) ---\n");
  for (int i = 0; i < 0x30; i++) {
    if (i % 16 == 0) fprintf(stderr, "$%04X: ", 0xB8F0 + i);
    fprintf(stderr, "%02X ", emu->ram_peek(0xB8F0 + i));
    if ((i & 15) == 15) fprintf(stderr, "\n");
  }

  // Check if $38xx == $B8xx (RWTS loaded to both?)
  int match = 0, differ = 0;
  for (int i = 0; i < 0x800; i++) {
    if (emu->ram_peek(0x3800 + i) == emu->ram_peek(0xB800 + i))
      match++;
    else
      differ++;
  }
  fprintf(stderr, "\n$3800-$3FFF vs $B800-$BFFF: %d match, %d differ\n", match, differ);

  // Dump stack area to see return addresses.
  fprintf(stderr, "\n--- Stack ($01F0-$01FF) ---\n");
  for (int i = 0; i < 16; i++)
    fprintf(stderr, "%02X ", emu->ram_peek(0x01F0 + i));
  fprintf(stderr, "\n");

  return 0;
}
