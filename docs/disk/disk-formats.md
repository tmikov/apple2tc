# Apple II Disk Image Formats

## 1. DSK / DO Format (DOS 3.3 Order)

The simplest and most common Apple II disk image format.

- **File size**: 143,360 bytes (exactly)
- **Structure**: 35 tracks x 16 sectors x 256 bytes, stored sequentially
- **No header or metadata**
- **Extensions**: `.dsk` (ambiguous), `.do` (explicitly DOS-order)

### File Offset Calculation

```c
offset = (track * 16 + dos_logical_sector) * 256;
```

Track 0 sector 0 starts at byte 0, track 0 sector 1 at byte 256, etc.

### Ambiguity

`.dsk` files may be either DOS-order or ProDOS-order. Detection heuristic: inspect
track 17 sector 0 for VTOC markers, or check for ProDOS volume header at block 2.

## 2. PO Format (ProDOS Order)

- **File size**: 143,360 bytes (same as DSK)
- **Extension**: `.po`
- **Same raw data**, but sectors stored in ProDOS logical order instead of DOS 3.3 order

### ProDOS Block Mapping

ProDOS uses 512-byte blocks (2 sectors each). 280 blocks per 5.25" disk.

```c
offset = block * 512;
```

Block-to-physical-sector mapping (per track, 8 blocks per track):

| Block | Phys Sector A (1st 256B) | Phys Sector B (2nd 256B) |
|-------|--------------------------|--------------------------|
| 0 | 0 | 2 |
| 1 | 4 | 6 |
| 2 | 1 | 3 |
| 3 | 5 | 7 |
| 4 | 8 | 10 |
| 5 | 12 | 14 |
| 6 | 9 | 11 |
| 7 | 13 | 15 |

## 3. Sector Interleave Tables

The "physical sector" is the number encoded in the address field on disk. The "logical
sector" is the index within the image file for that track.

### DOS 3.3: Physical → Logical

```c
const uint8_t dos33_phys_to_logical[16] = {
    0x00, 0x07, 0x0E, 0x06, 0x0D, 0x05, 0x0C, 0x04,
    0x0B, 0x03, 0x0A, 0x02, 0x09, 0x01, 0x08, 0x0F
};
```

### DOS 3.3: Logical → Physical

```c
const uint8_t dos33_logical_to_phys[16] = {
    0x00, 0x0D, 0x0B, 0x09, 0x07, 0x05, 0x03, 0x01,
    0x0E, 0x0C, 0x0A, 0x08, 0x06, 0x04, 0x02, 0x0F
};
```

### ProDOS: Physical → Logical

```c
const uint8_t prodos_phys_to_logical[16] = {
    0x00, 0x08, 0x01, 0x09, 0x02, 0x0A, 0x03, 0x0B,
    0x04, 0x0C, 0x05, 0x0D, 0x06, 0x0E, 0x07, 0x0F
};
```

### ProDOS: Logical → Physical

```c
const uint8_t prodos_logical_to_phys[16] = {
    0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E,
    0x01, 0x03, 0x05, 0x07, 0x09, 0x0B, 0x0D, 0x0F
};
```

### Converting Between DO and PO

```c
// DO sector → PO sector (for a given track)
uint8_t do_to_po(uint8_t dos_logical) {
    uint8_t physical = dos33_logical_to_phys[dos_logical];
    return prodos_phys_to_logical[physical];
}
```

## 4. NIB Format (Nibblized)

- **File size**: 232,960 bytes (exactly)
- **Structure**: 35 tracks x 6,656 bytes/track, no header
- **Contains raw nibblized data** as read by the Disk II controller

### File Offset

```c
offset = track * 6656;
```

Each track contains the full nibble stream: sync bytes, address fields, data fields, gaps.
Every byte has bit 7 set (values $80-$FF). Can be fed directly to the emulated shift register.

**Limitations**: Cannot represent self-sync byte timing (extra zero bits are lost) or
copy-protection schemes that rely on timing. Works for all standard DOS 3.3 and ProDOS disks.

## 5. WOZ Format Version 1.0

Modern preservation format with bit-level accuracy. Supports copy protection, timing tricks,
quarter-tracks, and weak bits.

### File Header (12 bytes)

| Offset | Size | Value |
|--------|------|-------|
| 0 | 4 | `"WOZ1"` (57 4F 5A 31) |
| 4 | 1 | `$FF` (high-bit check) |
| 5 | 3 | `$0A $0D $0A` (line-ending corruption check) |
| 8 | 4 | CRC32 of all data from byte 12 to EOF (0 if uncalculated) |

### Chunk Format

All chunks after header: 4-byte ASCII ID + 4-byte LE size + data.

### INFO Chunk (60 bytes data, file offset 20)

| Offset | Size | Field |
|--------|------|-------|
| 0 | 1 | Info version (1) |
| 1 | 1 | Disk type (1=5.25", 2=3.5") |
| 2 | 1 | Write protected (0/1) |
| 3 | 1 | Synchronized (0/1) |
| 4 | 1 | Cleaned (0/1, MC3470 fake bits removed) |
| 5 | 32 | Creator (UTF-8, space-padded) |

### TMAP Chunk (160 bytes, file offset 88)

Maps quarter-track positions to track indices:
- Byte 0 = track 0.00, byte 1 = track 0.25, byte 2 = track 0.50, etc.
- Value 0x00-0x9F = index into TRKS chunk
- Value 0xFF = empty (no track)

For standard 35-track disks, only bytes at positions 0, 4, 8, ... 136 are non-0xFF.

### TRKS Chunk (file offset 256)

Fixed 6,656-byte blocks per track:

| Offset | Size | Field |
|--------|------|-------|
| 0 | 6,646 | Bitstream data (zero-padded) |
| 6,646 | 2 | Bytes Used (uint16 LE) |
| 6,648 | 2 | Bit Count (uint16 LE) |
| 6,650 | 2 | Splice Point (0xFFFF if none) |
| 6,652 | 1 | Splice Nibble |
| 6,653 | 1 | Splice Bit Count |

**Track data offset**: `tmap_value * 6656 + 256`

**Bitstream**: Bits packed MSB-first. Normalized to 4us intervals. `1` = flux transition,
`0` = no transition.

## 6. WOZ Format Version 2.0

### Header: `"WOZ2"` (57 4F 5A 32), otherwise same structure.

### INFO Chunk (60 bytes) -- Additional Fields

| Offset | Size | Field |
|--------|------|-------|
| 37 | 1 | Disk sides (1 or 2) |
| 38 | 1 | Boot sector format (0=unknown, 1=16-sec, 2=13-sec, 3=both) |
| 39 | 1 | Optimal bit timing (125ns units; 32 = 4us for 5.25") |
| 40 | 2 | Compatible hardware (bitmask) |
| 42 | 2 | Required RAM (KB, 0=unknown) |
| 44 | 2 | Largest track (512-byte blocks) |

### TMAP Chunk -- Same as WOZ 1.0

### TRKS Chunk -- Variable-Length Tracks

**Major change from WOZ 1.0**: Tracks are variable-length, stored in 512-byte blocks.

**Track entry directory** (bytes 256-1535): 160 entries, 8 bytes each:

| Offset | Size | Field |
|--------|------|-------|
| 0 | 2 | Starting block (uint16 LE, min 3) |
| 2 | 2 | Block count (uint16 LE) |
| 4 | 4 | Bit count (uint32 LE) |

Track data blocks begin at byte 1536 (block 3), aligned to 512-byte boundaries.

**Track data offset**: `starting_block * 512`

### META Chunk (optional)

Tab-delimited UTF-8 key-value pairs separated by newline (`$0A`).
Keys: `title`, `subtitle`, `publisher`, `developer`, `copyright`, `version`, etc.

### WRIT Chunk (optional)

Write-back instructions for physical disk output. Not needed for emulation.

## 7. 2IMG Format

A wrapper that can contain DSK, PO, or NIB data with a 64-byte header.

### Header (64 bytes)

| Offset | Size | Field |
|--------|------|-------|
| 0x00 | 4 | Magic: `"2IMG"` |
| 0x04 | 4 | Creator ID (4-char) |
| 0x08 | 2 | Header size (always 0x0040) |
| 0x0A | 2 | Version (0x0001) |
| 0x0C | 4 | Image format: 0=DOS order, 1=ProDOS order, 2=NIB |
| 0x10 | 4 | Flags (bit 31=write-protect, bits 0-7=volume if bit 8 set) |
| 0x14 | 4 | ProDOS blocks (280 for standard 5.25") |
| 0x18 | 4 | Data offset (from start of file) |
| 0x1C | 4 | Data length |
| 0x20 | 4 | Comment offset (0 if none) |
| 0x24 | 4 | Comment length (0 if none) |
| 0x28 | 4 | Creator data offset (0 if none) |
| 0x2C | 4 | Creator data length (0 if none) |
| 0x30 | 16 | Reserved (zero) |

## 8. WOZ Emulation Implementation

### Bit Streaming

```c
uint8_t get_next_bit(uint8_t *track_data, uint32_t bit_count, uint32_t *bit_pos) {
    uint32_t byte_idx = *bit_pos >> 3;
    uint8_t  bit_idx  = 7 - (*bit_pos & 7);  // MSB first
    uint8_t  bit      = (track_data[byte_idx] >> bit_idx) & 1;
    *bit_pos = (*bit_pos + 1) % bit_count;
    return bit;
}
```

### MC3470 Fake Bit Simulation

```c
uint8_t head_window = 0;  // 4-bit sliding window

uint8_t next_disk_bit(void) {
    head_window = ((head_window << 1) | get_next_woz_bit()) & 0x0F;
    if (head_window != 0x00) {
        return (head_window & 0x02) >> 1;
    } else {
        return rand() % 10 < 3;  // ~30% chance of 1
    }
}
```

### Track Switching Position Scaling

```c
new_position = current_position * new_track_bit_count / old_track_bit_count;
```

For empty tracks (TMAP = 0xFF), use 51,200 bits (6,400 bytes) as virtual length.

## 9. Format Comparison

| Property | DSK/DO | PO | NIB | WOZ 1.0 | WOZ 2.0 |
|----------|--------|----|-----|---------|---------|
| File size | 143,360 | 143,360 | 232,960 | ~250KB | Variable |
| Data type | Decoded sectors | Decoded sectors | Raw nibbles | Bitstream | Bitstream |
| Sector order | DOS 3.3 logical | ProDOS logical | Physical | Physical | Physical |
| Copy protection | No | No | Partial | Full | Full |
| Timing bits | No | No | No | Yes | Yes |
| Header | None | None | None | 12B + chunks | 12B + chunks |

## 10. Nibblizing DSK Images

When loading a DSK/PO file, the emulator must nibblize each sector to produce the raw
nibble stream. See [encoding.md](encoding.md) Section 8 for the complete sector write
algorithm, and [tables.h](tables.h) for ready-to-use C tables.

The process per track:
1. Write gap 1 (~48 sync bytes)
2. For each physical sector 0-15:
   a. Look up which logical sector this physical sector corresponds to (using interleave table)
   b. Read 256 bytes from the DSK file at `(track * 16 + logical_sector) * 256`
   c. Write the sector (address field + gap + data field + gap) as described in encoding.md

## Sources

- [WOZ 2.0 Specification](https://applesaucefdc.com/woz/reference2/)
- [WOZ 1.0 Specification](https://applesaucefdc.com/woz/reference1/)
- [CiderPress2 Nibble Notes](https://ciderpress2.com/formatdoc/Nibble-notes.html)
- [CiderPress2 WOZ Notes](https://ciderpress2.com/formatdoc/Woz-notes.html)
- [2IMG Format Specification](https://gswv.apple2.org.za/a2zine/Docs/DiskImage_2MG_Info.txt)
- [The WOZ Format (Nerdly Pleasures)](http://nerdlypleasures.blogspot.com/2021/02/the-woz-format-accurate-preservation-of.html)
