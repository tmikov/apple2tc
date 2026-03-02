# Disk II Support Implementation Plan

> **For Claude:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task.

**Goal:** Add Disk II floppy controller emulation supporting DSK/DO disk images (read-only, simplified nibble-based approach).

**Architecture:** New C file `lib/a2io/disk2.c` implements the disk controller. State struct `a2_disk2_t` is embedded in `a2_iostate_t`. The existing `a2_io_peek`/`a2_io_poke` dispatch calls into disk2 functions for addresses $C0E0-$C0EF (soft switches) and $C600-$C6FF (boot ROM). DSK files are nibblized into circular track buffers on mount.

**Tech Stack:** C11, CMake, existing a2io library

**Design doc:** `docs/plans/2026-03-01-disk-support-design.md`

**Key reference docs:**
- `doc/disk/hardware.md` — soft switches, stepper motor, P6 ROM
- `doc/disk/encoding.md` — 6-and-2 GCR, sector format, prenibblize algorithm
- `doc/disk/implementation-guide.md` — simplified approach pseudocode
- `doc/disk/tables.h` — all lookup tables

---

### Task 1: Copy tables header and update CMakeLists.txt

**Files:**
- Create: `lib/a2io/disk2_tables.h` (copy from `doc/disk/tables.h`)
- Create: `lib/a2io/disk2.c` (empty stub)
- Modify: `lib/a2io/CMakeLists.txt`

**Step 1: Copy tables header**

```bash
cp doc/disk/tables.h lib/a2io/disk2_tables.h
```

**Step 2: Create empty disk2.c stub**

```c
/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/a2io.h"
#include "disk2_tables.h"

#include <stdlib.h>
#include <string.h>
```

**Step 3: Add disk2.c to CMakeLists.txt**

In `lib/a2io/CMakeLists.txt`, add `disk2.c` to the source list:

```cmake
add_library(a2io
  a2io.c ${A2TC_INC}/a2io.h
  disk2.c disk2_tables.h
  font.cpp font.h
  soundqueue.c ${A2TC_INC}/soundqueue.h
  )
```

**Step 4: Build to verify**

Run: `cd build && cmake .. && make -j`
Expected: Clean build, no errors.

**Step 5: Commit**

```bash
git add lib/a2io/disk2_tables.h lib/a2io/disk2.c lib/a2io/CMakeLists.txt
git commit -m "Add disk2 tables header and empty implementation stub"
```

---

### Task 2: Add disk2 state struct and function declarations to a2io.h

**Files:**
- Modify: `include/apple2tc/a2io.h`

**Step 1: Add disk2 types and state struct**

Add this *before* the `a2_iostate_t` definition (after the `A2_VC_HIRES` enum block):

```c
/// Disk II drive state.
typedef struct {
  uint8_t *nib_data;      ///< Nibblized track data (all 35 tracks).
  uint32_t position;      ///< Byte offset within current track.
  bool write_protected;
  bool mounted;
  bool skip;              ///< Timing toggle for read.
  int current_track;      ///< 0-34.
} a2_disk2_drive_t;

/// Disk II controller state.
typedef struct {
  uint8_t data_register;      ///< 74LS323 shift register.
  bool q6, q7;                ///< Mode latches.
  bool motor_on;
  uint64_t motor_off_cycle;   ///< Cycle when motor was turned off.
  int selected_drive;         ///< 0 or 1.
  uint8_t phases;             ///< Bitmask of active phase magnets.
  int phase_position;         ///< Half-track 0-69.
  a2_disk2_drive_t drive[2];
} a2_disk2_t;
```

**Step 2: Add disk2 field to a2_iostate_t**

Add `a2_disk2_t disk2;` field to the `a2_iostate_t` struct (after the `debug` field):

```c
typedef struct {
  // ... existing fields ...
  uint8_t debug;
  /// Disk II controller state.
  a2_disk2_t disk2;
} a2_iostate_t;
```

**Step 3: Add function declarations**

Add these before the `#ifdef __cplusplus` closing brace:

```c
/// Initialize disk2 controller state.
void a2_disk2_init(a2_disk2_t *disk);
/// Free disk2 controller state.
void a2_disk2_done(a2_disk2_t *disk);
/// Mount a DSK/DO format disk image (143360 bytes) on drive 0 or 1.
/// The data is copied and nibblized internally. Returns false on error.
bool a2_disk2_mount(a2_disk2_t *disk, int drive_num, const uint8_t *dsk_data, size_t size);
/// Unmount a drive.
void a2_disk2_unmount(a2_disk2_t *disk, int drive_num);
/// Handle a read from disk II soft switch ($C0E0-$C0EF). offset is 0x0-0xF.
uint8_t a2_disk2_peek(a2_disk2_t *disk, unsigned offset, unsigned cycles);
/// Handle a write to disk II soft switch ($C0E0-$C0EF). offset is 0x0-0xF.
void a2_disk2_poke(a2_disk2_t *disk, unsigned offset, uint8_t value, unsigned cycles);
/// Read from Disk II boot ROM ($C600-$C6FF). addr is 0x00-0xFF.
uint8_t a2_disk2_rom_peek(unsigned addr);
```

**Step 4: Build to verify**

Run: `cd build && make -j`
Expected: Linker errors for unresolved disk2 symbols (that's OK — we'll implement them next).

**Step 5: Commit**

```bash
git add include/apple2tc/a2io.h
git commit -m "Add Disk II state struct and function declarations to a2io.h"
```

---

### Task 3: Implement init, done, mount, unmount, and nibblization

This is the most complex task. The nibblization algorithm converts 256-byte DSK
sectors into the nibble stream that the emulated shift register reads.

**Files:**
- Modify: `lib/a2io/disk2.c`

**Reference:** Read `doc/disk/encoding.md` for the 6-and-2 encoding algorithm and
`doc/disk/implementation-guide.md` for the nibblization pseudocode.

**Step 1: Implement init and done**

```c
void a2_disk2_init(a2_disk2_t *disk) {
  memset(disk, 0, sizeof(*disk));
}

void a2_disk2_done(a2_disk2_t *disk) {
  for (int i = 0; i < 2; i++) {
    free(disk->drive[i].nib_data);
    disk->drive[i].nib_data = NULL;
  }
  memset(disk, 0, sizeof(*disk));
}
```

**Step 2: Implement helper: 4-and-4 encode**

Used for address field encoding. Splits a byte into odd/even bits, each written
as a disk byte with bit 7 set.

```c
/// Write a byte in 4-and-4 encoding (2 disk bytes).
static void encode_44(uint8_t *buf, int *pos, uint8_t val) {
  buf[(*pos)++] = (val >> 1) | 0xAA;
  buf[(*pos)++] = val | 0xAA;
}
```

**Step 3: Implement helper: write address field**

```c
/// Write the address field for a sector.
static void write_address_field(
    uint8_t *buf, int *pos, uint8_t volume, uint8_t track, uint8_t sector) {
  // Prologue.
  buf[(*pos)++] = DISK2_ADDR_PROLOGUE_1;
  buf[(*pos)++] = DISK2_ADDR_PROLOGUE_2;
  buf[(*pos)++] = DISK2_ADDR_PROLOGUE_3;
  // Volume, track, sector in 4-and-4.
  encode_44(buf, pos, volume);
  encode_44(buf, pos, track);
  encode_44(buf, pos, sector);
  // Checksum (XOR of all three).
  encode_44(buf, pos, volume ^ track ^ sector);
  // Epilogue.
  buf[(*pos)++] = DISK2_EPILOGUE_1;
  buf[(*pos)++] = DISK2_EPILOGUE_2;
  buf[(*pos)++] = DISK2_EPILOGUE_3;
}
```

**Step 4: Implement helper: write data field**

The 6-and-2 encoding pipeline:
1. Prenibblize: split 256 source bytes into 342 bytes (86 secondary + 256 primary)
2. XOR-encode: chain-XOR each byte with the previous
3. GCR translate: map each 6-bit value to a valid disk byte

```c
/// Write the data field for a 256-byte sector.
static void write_data_field(uint8_t *buf, int *pos, const uint8_t *sector) {
  // Prenibblize: 256 source bytes -> 342 6-bit values.
  // Secondary buffer (86 bytes): holds the low 2 bits of groups of 3 source bytes.
  // Primary buffer (256 bytes): holds the upper 6 bits.
  uint8_t nib_buf[342];
  memset(nib_buf, 0, sizeof(nib_buf));

  // Build secondary buffer (low 2 bits, packed 3 per byte, bit-reversed).
  for (int i = 0; i < 256; i++) {
    // Which secondary byte this source byte contributes to.
    int sec_idx = i % 86;
    // Which bit position (0, 2, or 4) within the secondary byte.
    int shift = (i / 86) * 2;
    // Extract low 2 bits of source, reverse them, shift into position.
    uint8_t low2 = sector[i] & 0x03;
    uint8_t reversed = ((low2 & 1) << 1) | ((low2 >> 1) & 1);
    nib_buf[sec_idx] |= reversed << shift;
  }

  // Primary buffer: upper 6 bits of each source byte.
  for (int i = 0; i < 256; i++) {
    nib_buf[86 + i] = sector[i] >> 2;
  }

  // Prologue.
  buf[(*pos)++] = DISK2_DATA_PROLOGUE_1;
  buf[(*pos)++] = DISK2_DATA_PROLOGUE_2;
  buf[(*pos)++] = DISK2_DATA_PROLOGUE_3;

  // XOR-encode and GCR translate (342 bytes + 1 checksum).
  uint8_t prev = 0;
  for (int i = 0; i < 342; i++) {
    uint8_t val = nib_buf[i] ^ prev;
    prev = nib_buf[i];
    buf[(*pos)++] = gcr_write_translate[val & 0x3F];
  }
  // Checksum is the last prev value XOR'd with itself = prev.
  buf[(*pos)++] = gcr_write_translate[prev & 0x3F];

  // Epilogue.
  buf[(*pos)++] = DISK2_EPILOGUE_1;
  buf[(*pos)++] = DISK2_EPILOGUE_2;
  buf[(*pos)++] = DISK2_EPILOGUE_3;
}
```

**Step 5: Implement nibblize_track**

```c
/// Nibblize one track (16 sectors) from DSK data into nibble buffer.
static void nibblize_track(
    int track, const uint8_t *dsk_data, uint8_t *nib_out, int *nib_len) {
  int pos = 0;
  uint8_t volume = 254; // Standard DOS 3.3 volume number.

  // Gap 1: sync bytes before first sector.
  for (int i = 0; i < 48; i++)
    nib_out[pos++] = 0xFF;

  for (int phys_sector = 0; phys_sector < DISK2_SECTORS_PER_TRACK; phys_sector++) {
    int logical = dos33_phys_to_logical[phys_sector];
    const uint8_t *sector_data =
        &dsk_data[(track * DISK2_SECTORS_PER_TRACK + logical) * DISK2_BYTES_PER_SECTOR];

    // Address field.
    write_address_field(nib_out, &pos, volume, (uint8_t)track, (uint8_t)phys_sector);

    // Gap 2: between address and data fields.
    for (int i = 0; i < 6; i++)
      nib_out[pos++] = 0xFF;

    // Data field.
    write_data_field(nib_out, &pos, sector_data);

    // Gap 3: between sectors.
    for (int i = 0; i < 27; i++)
      nib_out[pos++] = 0xFF;
  }

  *nib_len = pos;
}
```

**Step 6: Implement mount and unmount**

```c
bool a2_disk2_mount(a2_disk2_t *disk, int drive_num, const uint8_t *dsk_data, size_t size) {
  if (drive_num < 0 || drive_num > 1)
    return false;
  if (size != DISK2_DSK_SIZE)
    return false;

  a2_disk2_drive_t *drv = &disk->drive[drive_num];

  // Free any existing data.
  free(drv->nib_data);

  // Allocate nibblized data for all tracks.
  drv->nib_data = (uint8_t *)malloc(DISK2_TRACKS_PER_DISK * DISK2_NIB_TRACK_SIZE);
  if (!drv->nib_data)
    return false;

  // Nibblize all tracks.
  for (int t = 0; t < DISK2_TRACKS_PER_DISK; t++) {
    int nib_len = 0;
    nibblize_track(t, dsk_data, drv->nib_data + t * DISK2_NIB_TRACK_SIZE, &nib_len);
    // Pad remainder of track with sync bytes.
    while (nib_len < DISK2_NIB_TRACK_SIZE)
      drv->nib_data[t * DISK2_NIB_TRACK_SIZE + nib_len++] = 0xFF;
  }

  drv->position = 0;
  drv->current_track = 0;
  drv->write_protected = true; // Read-only for now.
  drv->mounted = true;
  drv->skip = false;

  return true;
}

void a2_disk2_unmount(a2_disk2_t *disk, int drive_num) {
  if (drive_num < 0 || drive_num > 1)
    return;
  a2_disk2_drive_t *drv = &disk->drive[drive_num];
  free(drv->nib_data);
  drv->nib_data = NULL;
  drv->mounted = false;
}
```

**Step 7: Build to verify**

Run: `cd build && make -j`
Expected: Clean compile (linker errors remain for peek/poke/rom_peek — next task).

**Step 8: Commit**

```bash
git add lib/a2io/disk2.c
git commit -m "Implement Disk II init, mount, and 6-and-2 nibblization"
```

---

### Task 4: Implement soft switch handler (peek/poke)

**Files:**
- Modify: `lib/a2io/disk2.c`

**Reference:** `doc/disk/hardware.md` for soft switch behavior,
`doc/disk/implementation-guide.md` for stepper motor algorithm.

**Step 1: Implement stepper motor handler**

```c
/// Handle stepper motor phase changes (offsets 0x0-0x7).
static void handle_stepper(a2_disk2_t *disk, unsigned offset) {
  int phase_num = offset >> 1;  // 0-3
  bool phase_on = offset & 1;

  if (phase_on)
    disk->phases |= (1 << phase_num);
  else
    disk->phases &= ~(1 << phase_num);

  // Compute movement direction from active magnets.
  int current_phase = (disk->phase_position / 2) & 3;
  int direction = 0;
  // Phase ahead?
  if (disk->phases & (1 << ((current_phase + 1) & 3)))
    direction += 1;
  // Phase behind?
  if (disk->phases & (1 << ((current_phase + 3) & 3)))
    direction -= 1;

  if (direction != 0) {
    disk->phase_position += direction;
    if (disk->phase_position < 0)
      disk->phase_position = 0;
    if (disk->phase_position > 69)
      disk->phase_position = 69;

    int new_track = disk->phase_position / 2;
    a2_disk2_drive_t *drv = &disk->drive[disk->selected_drive];
    if (drv->mounted && new_track != drv->current_track) {
      drv->current_track = new_track;
      drv->position = 0;
    }
  }
}
```

**Step 2: Implement read_nibble**

```c
/// Check if the motor is spinning (on or coasting down).
static bool is_motor_spinning(const a2_disk2_t *disk, unsigned cycles) {
  if (disk->motor_on)
    return true;
  // Motor coasts for ~1 second after being turned off.
  return (unsigned)(cycles - (unsigned)disk->motor_off_cycle) < DISK2_MOTOR_TIMEOUT_CYCLES;
}

/// Read the next nibble from the current track.
static uint8_t read_nibble(a2_disk2_t *disk, unsigned cycles) {
  a2_disk2_drive_t *drv = &disk->drive[disk->selected_drive];
  if (!drv->mounted || !is_motor_spinning(disk, cycles))
    return 0;

  // Toggle skip for timing approximation. Every other read returns 0
  // so the CPU's BPL loop spins once per valid byte.
  drv->skip = !drv->skip;
  if (drv->skip)
    return 0;

  uint8_t *track_base = drv->nib_data + drv->current_track * DISK2_NIB_TRACK_SIZE;
  uint8_t val = track_base[drv->position];
  drv->position++;
  if (drv->position >= DISK2_NIB_TRACK_SIZE)
    drv->position = 0;

  disk->data_register = val;
  return val;
}
```

**Step 3: Implement peek and poke**

```c
uint8_t a2_disk2_peek(a2_disk2_t *disk, unsigned offset, unsigned cycles) {
  switch (offset) {
  // Stepper phases (0x0-0x7).
  case 0x0: case 0x1: case 0x2: case 0x3:
  case 0x4: case 0x5: case 0x6: case 0x7:
    handle_stepper(disk, offset);
    break;
  // Motor off.
  case 0x8:
    disk->motor_on = false;
    disk->motor_off_cycle = cycles;
    break;
  // Motor on.
  case 0x9:
    disk->motor_on = true;
    break;
  // Drive 1 select.
  case 0xA:
    disk->selected_drive = 0;
    break;
  // Drive 2 select.
  case 0xB:
    disk->selected_drive = 1;
    break;
  // Q6L: shift mode. In read mode (!q7), return next nibble.
  case 0xC:
    disk->q6 = false;
    if (!disk->q7)
      return read_nibble(disk, cycles);
    break;
  // Q6H: load mode. In read mode (!q7), return write-protect status.
  case 0xD:
    disk->q6 = true;
    if (!disk->q7) {
      a2_disk2_drive_t *drv = &disk->drive[disk->selected_drive];
      return drv->write_protected ? 0xFF : 0x00;
    }
    break;
  // Q7L: read mode.
  case 0xE:
    disk->q7 = false;
    break;
  // Q7H: write mode.
  case 0xF:
    disk->q7 = true;
    break;
  }
  return disk->data_register;
}

void a2_disk2_poke(a2_disk2_t *disk, unsigned offset, uint8_t value, unsigned cycles) {
  // For read-only mode, writes trigger the same side effects as reads.
  // The value is only needed for write mode (future work).
  (void)value;
  a2_disk2_peek(disk, offset, cycles);
}
```

**Step 4: Build to verify**

Run: `cd build && make -j`
Expected: Linker error only for `a2_disk2_rom_peek` (next task).

**Step 5: Commit**

```bash
git add lib/a2io/disk2.c
git commit -m "Implement Disk II soft switch handler: stepper, motor, Q6/Q7, read"
```

---

### Task 5: Add P5 boot ROM and IO dispatch

**Files:**
- Modify: `lib/a2io/disk2.c` (add ROM data and rom_peek)
- Modify: `lib/a2io/a2io.c` (add dispatch)

**Step 1: Add P5 boot ROM data to disk2.c**

The P5 boot ROM is the standard 16-sector Disk II controller ROM.
Add this static array to `disk2.c`:

```c
/// Disk II P5 boot ROM (16-sector), mapped at $C600-$C6FF for slot 6.
/// This is the standard boot ROM that ships on every Disk II controller card.
static const uint8_t disk2_boot_rom[256] = {
    0xA2,0x20,0xA0,0x00,0xA2,0x03,0x86,0x3C, 0x8A,0x0A,0x24,0x3C,0xF0,0x10,0x05,0x3C,
    0x49,0xFF,0x29,0x7E,0xB0,0x08,0x4A,0xD0, 0xFB,0x98,0x9D,0x56,0x03,0xC8,0xE8,0x10,
    0xE5,0x20,0x58,0xFF,0xBA,0xBD,0x00,0x01, 0x0A,0x0A,0x0A,0x0A,0x85,0x2B,0xAA,0xBD,
    0x8E,0xC0,0xBD,0x8C,0xC0,0xBD,0x8A,0xC0, 0xBD,0x89,0xC0,0xA0,0x50,0xBD,0x80,0xC0,
    0x98,0x29,0x03,0x0A,0x05,0x2B,0xAA,0xBD, 0x81,0xC0,0xA9,0x56,0x20,0xA8,0xFC,0x88,
    0x10,0xEB,0x85,0x26,0x85,0x3D,0x85,0x41, 0xA9,0x08,0x85,0x27,0x18,0x08,0xBD,0x8C,
    0xC0,0x10,0xFB,0x49,0xD5,0xD0,0xF7,0xBD, 0x8C,0xC0,0x10,0xFB,0xC9,0xAA,0xD0,0xF3,
    0xEA,0xBD,0x8C,0xC0,0x10,0xFB,0xC9,0x96, 0xF0,0x09,0x28,0x90,0xDF,0x49,0xAD,0xF0,
    0x25,0xD0,0xD9,0xA0,0x03,0x85,0x40,0xBD, 0x8C,0xC0,0x10,0xFB,0x2A,0x85,0x3C,0xBD,
    0x8C,0xC0,0x10,0xFB,0x25,0x3C,0x88,0xD0, 0xEC,0x28,0xC5,0x3D,0xD0,0xBE,0xA5,0x40,
    0xC5,0x41,0xD0,0xB8,0xB0,0xB7,0xA0,0x56, 0x84,0x3C,0xBC,0x8C,0xC0,0x10,0xFB,0x59,
    0xD6,0x02,0xA4,0x3C,0x88,0x99,0x00,0x03, 0xD0,0xEE,0x84,0x3C,0xBC,0x8C,0xC0,0x10,
    0xFB,0x59,0xD6,0x02,0xA4,0x3C,0x91,0x26, 0xC8,0xD0,0xEF,0xBC,0x8C,0xC0,0x10,0xFB,
    0x59,0xD6,0x02,0xD0,0x87,0xA0,0x00,0xA2, 0x56,0xCA,0x30,0xFB,0xB1,0x26,0x5E,0x00,
    0x03,0x2A,0x5E,0x00,0x03,0x2A,0x91,0x26, 0xC8,0xD0,0xEE,0xE6,0x27,0xE6,0x3D,0xA5,
    0x3D,0xCD,0x00,0x08,0xA6,0x2B,0x90,0xDB, 0x4C,0x01,0x08,0x00,0x00,0x00,0x00,0x00,
};
```

**Step 2: Implement rom_peek**

```c
uint8_t a2_disk2_rom_peek(unsigned addr) {
  return disk2_boot_rom[addr & 0xFF];
}
```

**Step 3: Add disk2 dispatch to a2_io_peek in a2io.c**

In `a2io.c`, add `#include "disk2_tables.h"` is NOT needed here. Instead, the
dispatch just calls the disk2 functions declared in `a2io.h`.

In `a2_io_peek()`, add handling *before* the existing switch statement:

```c
uint8_t a2_io_peek(a2_iostate_t *io, uint16_t addr, unsigned cycles) {
  // Disk II boot ROM: $C600-$C6FF.
  if (addr >= 0xC600 && addr <= 0xC6FF)
    return a2_disk2_rom_peek(addr - 0xC600);

  switch (addr & 0xCFF0) {
  // ... existing cases ...

  // Disk II soft switches: $C0E0-$C0EF.
  case 0xC0E0:
    return a2_disk2_peek(&io->disk2, addr & 0x0F, cycles);

  default:
    // ... existing default ...
  }
  return 0;
}
```

**Step 4: Add disk2 dispatch to a2_io_poke in a2io.c**

Currently `a2_io_poke` just calls `a2_io_peek` twice (ignoring the value).
Add disk2 handling before that fallback:

```c
void a2_io_poke(a2_iostate_t *io, uint16_t addr, uint8_t value, unsigned cycles) {
  // Disk II soft switches need the write value for future write support.
  if ((addr & 0xCFF0) == 0xC0E0) {
    a2_disk2_poke(&io->disk2, addr & 0x0F, value, cycles);
    return;
  }
  a2_io_peek(io, addr, cycles);
}
```

**Step 5: Add disk2 init/done calls to a2_io_init/a2_io_done**

In `a2_io_init()`, add: `a2_disk2_init(&io->disk2);`
In `a2_io_done()`, add: `a2_disk2_done(&io->disk2);` (before the memset).

**Step 6: Build to verify**

Run: `cd build && make -j`
Expected: Clean build, no errors.

**Step 7: Commit**

```bash
git add lib/a2io/disk2.c lib/a2io/a2io.c
git commit -m "Add P5 boot ROM and wire disk2 dispatch into a2io peek/poke"
```

---

### Task 6: Add --disk1/--disk2 CLI args to a2emu

**Files:**
- Modify: `tools/a2emu/a2emu.cpp`

**Step 1: Add disk path fields to CLIArgs**

```cpp
struct CLIArgs {
  // ... existing fields ...
  std::string kbdPath{};
  /// Disk image paths for drive 1 and 2.
  std::string disk1Path{};
  std::string disk2Path{};
};
```

**Step 2: Add CLI parsing for --disk1 and --disk2**

In `parseCLI()`, add these cases in the option parsing loop (before the
`if (arg[0] == '-')` error check):

```cpp
    if (strncmp(arg, "--disk1=", 8) == 0) {
      cliArgs.disk1Path = arg + 8;
      continue;
    }
    if (strncmp(arg, "--disk2=", 8) == 0) {
      cliArgs.disk2Path = arg + 8;
      continue;
    }
```

Update `printHelp()`:

```cpp
  printf(" --disk1=path     Mount disk image in drive 1\n");
  printf(" --disk2=path     Mount disk image in drive 2\n");
```

**Step 3: Add disk mounting in A2Emu constructor**

After the `loadROM` call in the constructor, add disk mounting:

```cpp
  emu_.loadROM(apple2plus_rom, apple2plus_rom_len);

  // Mount disk images if specified.
  if (!cliArgs_.disk1Path.empty()) {
    if (FILE *f = fopen(cliArgs_.disk1Path.c_str(), "rb")) {
      auto data = readAll<std::vector<uint8_t>>(f);
      fclose(f);
      if (!a2_disk2_mount(&emu_.io()->disk2, 0, data.data(), data.size())) {
        fprintf(stderr, "Failed to mount disk1: %s (bad format or size)\n",
                cliArgs_.disk1Path.c_str());
        exit(2);
      }
      fprintf(stderr, "Mounted disk1: %s\n", cliArgs_.disk1Path.c_str());
    } else {
      perror(cliArgs_.disk1Path.c_str());
      exit(2);
    }
  }
  if (!cliArgs_.disk2Path.empty()) {
    if (FILE *f = fopen(cliArgs_.disk2Path.c_str(), "rb")) {
      auto data = readAll<std::vector<uint8_t>>(f);
      fclose(f);
      if (!a2_disk2_mount(&emu_.io()->disk2, 1, data.data(), data.size())) {
        fprintf(stderr, "Failed to mount disk2: %s (bad format or size)\n",
                cliArgs_.disk2Path.c_str());
        exit(2);
      }
      fprintf(stderr, "Mounted disk2: %s\n", cliArgs_.disk2Path.c_str());
    } else {
      perror(cliArgs_.disk2Path.c_str());
      exit(2);
    }
  }
```

**Step 4: Build to verify**

Run: `cd build && cmake .. && make -j`
Expected: Clean build, no errors.

**Step 5: Commit**

```bash
git add tools/a2emu/a2emu.cpp
git commit -m "Add --disk1/--disk2 CLI args to a2emu for mounting DSK images"
```

---

### Task 7: Integration test — boot DOS 3.3

**Prerequisites:** A DOS 3.3 system disk image (`dos33.dsk`, 143360 bytes).
Standard DOS 3.3 master disk images are widely available.

**Step 1: Run the emulator with a disk image**

```bash
cd build
./tools/a2emu/a2emu --disk1=/path/to/dos33.dsk
```

Expected: The emulator window opens, shows the Apple II boot sequence, loads DOS
3.3 from disk, and reaches the `]` BASIC prompt.

**Step 2: If boot fails — debug**

Common issues:
- **Black screen / hangs at boot:** P5 boot ROM not being read. Check that
  $C600-$C6FF dispatch is working (add `fprintf(stderr, ...)` in `a2_disk2_rom_peek`).
- **Hangs reading disk:** Soft switch handler not returning data. Add debug
  prints in `a2_disk2_peek` for Q6L reads.
- **Garbled boot / checksum errors:** Nibblization bug. Compare nibblized output
  against a known-good .nib file.
- **Stepper not moving:** Phase handling bug. Add debug prints in `handle_stepper`.

**Step 3: Test CATALOG command**

At the `]` prompt, type: `CATALOG` and press Enter.
Expected: Lists files on the disk.

**Step 4: Commit any fixes**

If debugging required code changes, commit them:

```bash
git add -u
git commit -m "Fix Disk II boot issues found during integration testing"
```
