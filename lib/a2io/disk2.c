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

/// Encode a byte using 4-and-4 encoding (for address fields).
/// Splits into odd/even bits, each written as a disk byte with bit 7 set.
static void encode_44(uint8_t *buf, int *pos, uint8_t val) {
  buf[(*pos)++] = (val >> 1) | 0xAA;
  buf[(*pos)++] = val | 0xAA;
}

/// Write an address field: prologue, volume/track/sector in 4-and-4, checksum, epilogue.
static void write_address_field(
    uint8_t *buf, int *pos, uint8_t volume, uint8_t track, uint8_t sector) {
  buf[(*pos)++] = DISK2_ADDR_PROLOGUE_1; // D5
  buf[(*pos)++] = DISK2_ADDR_PROLOGUE_2; // AA
  buf[(*pos)++] = DISK2_ADDR_PROLOGUE_3; // 96
  encode_44(buf, pos, volume);
  encode_44(buf, pos, track);
  encode_44(buf, pos, sector);
  encode_44(buf, pos, volume ^ track ^ sector);
  buf[(*pos)++] = DISK2_EPILOGUE_1; // DE
  buf[(*pos)++] = DISK2_EPILOGUE_2; // AA
  buf[(*pos)++] = DISK2_EPILOGUE_3; // EB
}

/// Write a data field using 6-and-2 GCR encoding.
///
/// The 6-and-2 encoding pipeline:
/// 1. Prenibblize: split 256 source bytes into 342 bytes (86 secondary + 256 primary)
///    - Secondary buffer (indices 0-85): low 2 bits of groups of 3 source bytes, bit-reversed
///    - Primary buffer (indices 86-341): upper 6 bits of each source byte
/// 2. XOR-encode: chain-XOR each value with the previous
/// 3. GCR translate: map each 6-bit value through gcr_write_translate[]
static void write_data_field(uint8_t *buf, int *pos, const uint8_t *sector) {
  uint8_t nib_buf[342];
  memset(nib_buf, 0, sizeof(nib_buf));

  // Prenibblize: build secondary buffer (low 2 bits, packed, bit-reversed).
  for (int i = 0; i < 256; i++) {
    int sec_idx = i % 86;
    int shift = (i / 86) * 2;
    uint8_t low2 = sector[i] & 0x03;
    uint8_t reversed = ((low2 & 1) << 1) | ((low2 >> 1) & 1);
    nib_buf[sec_idx] |= reversed << shift;
  }

  // Prenibblize: primary buffer (upper 6 bits).
  for (int i = 0; i < 256; i++) {
    nib_buf[86 + i] = sector[i] >> 2;
  }

  // Prologue.
  buf[(*pos)++] = DISK2_DATA_PROLOGUE_1; // D5
  buf[(*pos)++] = DISK2_DATA_PROLOGUE_2; // AA
  buf[(*pos)++] = DISK2_DATA_PROLOGUE_3; // AD

  // XOR-encode and GCR translate.
  uint8_t prev = 0;
  for (int i = 0; i < 342; i++) {
    uint8_t val = nib_buf[i] ^ prev;
    prev = nib_buf[i];
    buf[(*pos)++] = gcr_write_translate[val & 0x3F];
  }
  // Checksum.
  buf[(*pos)++] = gcr_write_translate[prev & 0x3F];

  // Epilogue.
  buf[(*pos)++] = DISK2_EPILOGUE_1; // DE
  buf[(*pos)++] = DISK2_EPILOGUE_2; // AA
  buf[(*pos)++] = DISK2_EPILOGUE_3; // EB
}

/// Nibblize a single track from DSK format into the nibble stream.
/// Writes address fields, data fields, gaps, and sync bytes for all 16 sectors.
static void nibblize_track(
    int track, const uint8_t *dsk_data, uint8_t *nib_out, int *nib_len) {
  int pos = 0;
  uint8_t volume = 254;

  // Gap 1: initial sync bytes.
  for (int i = 0; i < 48; i++)
    nib_out[pos++] = 0xFF;

  for (int phys_sector = 0; phys_sector < DISK2_SECTORS_PER_TRACK; phys_sector++) {
    int logical = dos33_phys_to_logical[phys_sector];
    const uint8_t *sector_data =
        &dsk_data[(track * DISK2_SECTORS_PER_TRACK + logical) * DISK2_BYTES_PER_SECTOR];

    write_address_field(nib_out, &pos, volume, (uint8_t)track, (uint8_t)phys_sector);

    // Gap 2: sync between address and data fields.
    for (int i = 0; i < 6; i++)
      nib_out[pos++] = 0xFF;

    write_data_field(nib_out, &pos, sector_data);

    // Gap 3: sync between sectors.
    for (int i = 0; i < 27; i++)
      nib_out[pos++] = 0xFF;
  }

  *nib_len = pos;
}

bool a2_disk2_mount(a2_disk2_t *disk, int drive_num, const uint8_t *dsk_data, size_t size) {
  if (drive_num < 0 || drive_num > 1)
    return false;
  if (size != DISK2_DSK_SIZE)
    return false;

  a2_disk2_drive_t *drv = &disk->drive[drive_num];
  free(drv->nib_data);

  drv->nib_data = (uint8_t *)malloc(DISK2_TRACKS_PER_DISK * DISK2_NIB_TRACK_SIZE);
  if (!drv->nib_data)
    return false;

  for (int t = 0; t < DISK2_TRACKS_PER_DISK; t++) {
    int nib_len = 0;
    nibblize_track(t, dsk_data, drv->nib_data + t * DISK2_NIB_TRACK_SIZE, &nib_len);
    // Pad the rest of the track with sync bytes.
    while (nib_len < DISK2_NIB_TRACK_SIZE)
      drv->nib_data[t * DISK2_NIB_TRACK_SIZE + nib_len++] = 0xFF;
  }

  drv->position = 0;
  drv->current_track = 0;
  drv->write_protected = true;
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

/// Handle stepper motor phase changes (offsets 0x0-0x7).
static void handle_stepper(a2_disk2_t *disk, unsigned offset) {
  int phase_num = offset >> 1;
  bool phase_on = offset & 1;

  if (phase_on)
    disk->phases |= (1 << phase_num);
  else
    disk->phases &= ~(1 << phase_num);

  int current_phase = (disk->phase_position / 2) & 3;
  int direction = 0;
  if (disk->phases & (1 << ((current_phase + 1) & 3)))
    direction += 1;
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

/// Check if the motor is spinning (on or coasting down).
static bool is_motor_spinning(const a2_disk2_t *disk, unsigned cycles) {
  if (disk->motor_on)
    return true;
  return (unsigned)(cycles - (unsigned)disk->motor_off_cycle) < DISK2_MOTOR_TIMEOUT_CYCLES;
}

/// Read the next nibble from the current track.
static uint8_t read_nibble(a2_disk2_t *disk, unsigned cycles) {
  a2_disk2_drive_t *drv = &disk->drive[disk->selected_drive];
  if (!drv->mounted || !is_motor_spinning(disk, cycles))
    return 0;

  // Toggle skip for timing approximation.
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

uint8_t a2_disk2_peek(a2_disk2_t *disk, unsigned offset, unsigned cycles) {
  switch (offset) {
  case 0x0: case 0x1: case 0x2: case 0x3:
  case 0x4: case 0x5: case 0x6: case 0x7:
    handle_stepper(disk, offset);
    break;
  case 0x8:
    disk->motor_on = false;
    disk->motor_off_cycle = cycles;
    break;
  case 0x9:
    disk->motor_on = true;
    break;
  case 0xA:
    disk->selected_drive = 0;
    break;
  case 0xB:
    disk->selected_drive = 1;
    break;
  case 0xC:
    disk->q6 = false;
    if (!disk->q7)
      return read_nibble(disk, cycles);
    break;
  case 0xD:
    disk->q6 = true;
    if (!disk->q7) {
      a2_disk2_drive_t *drv = &disk->drive[disk->selected_drive];
      return drv->write_protected ? 0xFF : 0x00;
    }
    break;
  case 0xE:
    disk->q7 = false;
    break;
  case 0xF:
    disk->q7 = true;
    break;
  }
  return disk->data_register;
}

void a2_disk2_poke(a2_disk2_t *disk, unsigned offset, uint8_t value, unsigned cycles) {
  (void)value;
  a2_disk2_peek(disk, offset, cycles);
}
