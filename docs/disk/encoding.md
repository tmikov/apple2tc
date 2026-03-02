# Apple II Disk Encoding Reference

## 1. GCR Encoding Overview

The Disk II uses Group Code Recording (GCR), not FM or MFM like IBM PC drives.
- A `1` bit = magnetic flux transition
- A `0` bit = no transition within a 4us window
- The MC3470 read amplifier outputs a 1us pulse for each flux transition

### Encoding Constraints (16-sector / DOS 3.3)

Every disk byte must satisfy:
1. **Bit 7 must be set** (value >= $80) -- the shift register uses bit 7 to signal byte-ready
2. **No more than two consecutive zero bits** -- longer runs cause AGC noise
3. **At least one pair of adjacent 1-bits** in bits 0-6

These constraints yield exactly **64 valid byte values**, sufficient for 6 bits of data per
disk byte. Additionally, `$D5` and `$AA` are **reserved as markers** and never appear in
encoded data.

## 2. Three Encoding Schemes

### 2.1 4-and-4 Encoding (Address Fields Only)

Each source byte is split into two disk bytes by separating odd and even bits:

```
Source byte:        b7 b6 b5 b4 b3 b2 b1 b0
First disk byte:    1  b7  1  b5  1  b3  1  b1   (odd bits)
Second disk byte:   1  b6  1  b4  1  b2  1  b0   (even bits)
```

Every bit position is interleaved with a forced `1`, guaranteeing no consecutive zeros.

```c
// Encode
void encode_4and4(uint8_t value, uint8_t *out) {
    out[0] = (value >> 1) | 0xAA;  // odd bits
    out[1] = value | 0xAA;         // even bits
}

// Decode
uint8_t decode_4and4(uint8_t odd, uint8_t even) {
    return ((odd << 1) | 0x01) & even;
}
```

50% efficiency (2 disk bytes per data byte). Used only for the 4-byte address field
metadata (volume, track, sector, checksum).

### 2.2 6-and-2 Encoding (Data Fields, DOS 3.3+)

Encodes 6 bits per disk byte using a 64-entry lookup table. 256 source bytes expand to
342 disk bytes + 1 checksum = 343 bytes.

### 2.3 5-and-3 Encoding (Data Fields, DOS 3.2)

For the older 13-sector format. Only 32 valid byte values (stricter constraint: no more
than **one** consecutive zero bit). 256 source bytes expand to ~410 bytes. Not commonly
needed for emulation unless supporting DOS 3.2 disks.

## 3. The 6-and-2 Write Translate Table

Maps 6-bit values (0x00-0x3F) to valid disk bytes:

```c
const uint8_t write_translate_62[64] = {
    0x96, 0x97, 0x9A, 0x9B, 0x9D, 0x9E, 0x9F, 0xA6,  // 0x00-0x07
    0xA7, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB2, 0xB3,  // 0x08-0x0F
    0xB4, 0xB5, 0xB6, 0xB7, 0xB9, 0xBA, 0xBB, 0xBC,  // 0x10-0x17
    0xBD, 0xBE, 0xBF, 0xCB, 0xCD, 0xCE, 0xCF, 0xD3,  // 0x18-0x1F
    0xD6, 0xD7, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE,  // 0x20-0x27
    0xDF, 0xE5, 0xE6, 0xE7, 0xE9, 0xEA, 0xEB, 0xEC,  // 0x28-0x2F
    0xED, 0xEE, 0xEF, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6,  // 0x30-0x37
    0xF7, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF   // 0x38-0x3F
};
```

The **read (inverse) translate table** (256 entries, 0xFF = invalid):
```c
uint8_t read_translate_62[256];
memset(read_translate_62, 0xFF, 256);
for (int i = 0; i < 64; i++)
    read_translate_62[write_translate_62[i]] = i;
```

## 4. Sector Format on Disk (16-Sector)

Each of the 16 sectors per track consists of:

```
+--- Address Field ---+
| Prologue:  D5 AA 96 |  (3 bytes)
| Volume:    xx xx     |  (2 bytes, 4-and-4 encoded)
| Track:     xx xx     |  (2 bytes, 4-and-4 encoded)
| Sector:    xx xx     |  (2 bytes, 4-and-4 encoded)
| Checksum:  xx xx     |  (2 bytes, 4-and-4 of vol XOR trk XOR sec)
| Epilogue:  DE AA EB  |  (3 bytes)
+----------------------+
| Gap 2: ~6 self-sync bytes ($FF, 10-bit)
+--- Data Field ------+
| Prologue:  D5 AA AD |  (3 bytes)
| Data:      xxx...   |  (342 bytes, 6-and-2 encoded)
| Checksum:  xx       |  (1 byte)
| Epilogue:  DE AA EB |  (3 bytes)
+----------------------+
| Gap 3: ~20 self-sync bytes ($FF, 10-bit)
```

**Byte counts per sector:**
- Address field: 3 + 8 + 3 = 14 bytes
- Gap 2: ~6 bytes
- Data field: 3 + 342 + 1 + 3 = 349 bytes
- Gap 3: ~20 bytes
- Total: ~389-416 bytes per sector
- 16 sectors x ~416 = ~6,656 bytes per track

### Marker Sequences

| Sequence | Meaning |
|----------|---------|
| `D5 AA 96` | Address field prologue (16-sector) |
| `D5 AA B5` | Address field prologue (13-sector, DOS 3.2) |
| `D5 AA AD` | Data field prologue |
| `DE AA EB` | Epilogue (all fields) |

`$D5` and `$AA` never appear in any encoded data, making them unambiguous markers.

## 5. The 6-and-2 Prenibblize Algorithm (Encoding)

Converts 256 source bytes into 342 six-bit values, ready for XOR encoding and GCR lookup.

### Concept

Each 8-bit source byte is split:
- **Upper 6 bits** (bits 7-2) → stored in "primary buffer" (256 entries)
- **Lower 2 bits** (bits 1-0) → packed into "secondary buffer" (86 entries)

Three source bytes contribute their 2-bit remainders to each secondary buffer entry
(3 x 2 = 6 bits). Since 256/3 = 85.33, we need 86 entries (last 2 entries are partial).

### Step-by-Step

```c
void prenibblize(const uint8_t data[256], uint8_t buf[342]) {
    // buf[0..85]   = secondary buffer (2-bit groups)
    // buf[86..341] = primary buffer (6-bit high parts)

    memset(buf, 0, 86);

    for (int i = 255; i >= 0; i--) {
        uint8_t val = data[i];

        // Upper 6 bits -> primary buffer
        buf[86 + i] = val >> 2;

        // Lower 2 bits, bit-reversed, -> secondary buffer
        uint8_t low2 = ((val & 0x01) << 1) | ((val & 0x02) >> 1);
        int sec_idx = i % 86;
        buf[sec_idx] = (buf[sec_idx] << 2) | low2;
    }
}
```

Secondary buffer entry `buf[n]` contains 2-bit contributions from `data[n]`, `data[n+86]`,
and `data[n+172]` (where valid). The 2-bit values are bit-reversed before packing.

### XOR Encoding

```c
void xor_encode(uint8_t buf[342], uint8_t *checksum) {
    uint8_t prev = 0;
    for (int i = 0; i < 342; i++) {
        uint8_t temp = buf[i];
        buf[i] = temp ^ prev;
        prev = temp;
    }
    *checksum = prev;  // becomes the 343rd byte
}
```

### GCR Table Lookup

```c
void gcr_encode(uint8_t buf[343], uint8_t disk_out[343]) {
    for (int i = 0; i < 343; i++)
        disk_out[i] = write_translate_62[buf[i] & 0x3F];
}
```

### Complete Encoding Pipeline

```
256 source bytes
  → [prenibblize] → 342 six-bit values
  → [XOR encode]  → 342 values + 1 checksum = 343 six-bit values
  → [GCR lookup]  → 343 valid disk bytes
  → [wrap]        → D5 AA AD [343 bytes] DE AA EB = 349 disk bytes
```

## 6. The 6-and-2 Denibblize Algorithm (Decoding)

### Pipeline

```
349 disk bytes
  → [find D5 AA AD, strip prologue/epilogue]
  → [reverse GCR lookup] → 343 six-bit values
  → [XOR decode]         → 342 values + verify checksum = 0
  → [denibblize]         → 256 source bytes
```

### XOR Decode

```c
uint8_t xor_decode(uint8_t buf[343]) {
    uint8_t prev = 0;
    for (int i = 0; i < 342; i++) {
        buf[i] ^= prev;
        prev = buf[i];
    }
    return buf[342] ^ prev;  // 0 = success
}
```

### Denibblize

```c
void denibblize(const uint8_t buf[342], uint8_t data[256]) {
    for (int i = 0; i < 256; i++) {
        uint8_t hi6 = buf[86 + i];
        int sec_idx = i % 86;
        int round = i / 86;  // 0, 1, or 2
        uint8_t low2_reversed = (buf[sec_idx] >> (round * 2)) & 0x03;
        uint8_t low2 = ((low2_reversed & 0x01) << 1) | ((low2_reversed & 0x02) >> 1);
        data[i] = (hi6 << 2) | low2;
    }
}
```

## 7. Self-Sync Bytes

Self-sync bytes provide byte alignment when the disk starts being read.

### 10-bit Self-Sync (6-and-2 / DOS 3.3)

The value $FF written as a 10-bit sequence (8 data bits + 2 extra zero bits):

```
Normal 8-bit $FF:  1 1 1 1 1 1 1 1
10-bit sync $FF:   1 1 1 1 1 1 1 1 0 0
```

The extra zeros are generated by delaying the next byte delivery by 2 bit periods (8us).

**How alignment works**: At misaligned positions, the shift register reads various values.
At offsets 8 and 9, bit 7 is clear, so the register does NOT latch -- it keeps shifting,
which forces convergence toward correct alignment. After ~4-5 sync bytes, alignment locks.

### Gap Sizes

| Gap | Location | Typical Size |
|-----|----------|-------------|
| Gap 1 | Start of track | 40-128 sync bytes |
| Gap 2 | Address → Data field | 5-10 sync bytes |
| Gap 3 | Data → next Address field | 14-24 sync bytes |

In NIB files, self-sync bytes appear as regular $FF values (the extra timing bits are lost).
In WOZ files, the extra zero bits are preserved in the bitstream.

## 8. Complete Sector Write Example

```c
void write_sector(uint8_t *track, int *pos,
                  uint8_t volume, uint8_t track_num, uint8_t sector,
                  const uint8_t data[256])
{
    // Gap (self-sync bytes)
    for (int i = 0; i < 20; i++)
        track[(*pos)++] = 0xFF;  // 10-bit in hardware, 8-bit in NIB

    // Address field
    track[(*pos)++] = 0xD5;
    track[(*pos)++] = 0xAA;
    track[(*pos)++] = 0x96;
    encode_4and4(volume, &track[*pos]); *pos += 2;
    encode_4and4(track_num, &track[*pos]); *pos += 2;
    encode_4and4(sector, &track[*pos]); *pos += 2;
    encode_4and4(volume ^ track_num ^ sector, &track[*pos]); *pos += 2;
    track[(*pos)++] = 0xDE;
    track[(*pos)++] = 0xAA;
    track[(*pos)++] = 0xEB;

    // Gap 2
    for (int i = 0; i < 6; i++)
        track[(*pos)++] = 0xFF;

    // Data field
    track[(*pos)++] = 0xD5;
    track[(*pos)++] = 0xAA;
    track[(*pos)++] = 0xAD;

    uint8_t nbuf[343];
    prenibblize(data, nbuf);
    uint8_t checksum;
    xor_encode(nbuf, &checksum);
    nbuf[342] = checksum;

    for (int i = 0; i < 343; i++)
        track[(*pos)++] = write_translate_62[nbuf[i] & 0x3F];

    track[(*pos)++] = 0xDE;
    track[(*pos)++] = 0xAA;
    track[(*pos)++] = 0xEB;
}
```

## Sources

- Don Worth & Pieter Lechner, "Beneath Apple DOS" (1981)
- [CiderPress2 Nibble Notes](https://ciderpress2.com/formatdoc/Nibble-notes.html)
- [Apple GCR Disk Encoding (CLK Wiki)](https://github.com/TomHarte/CLK/wiki/Apple-GCR-disk-encoding)
- [Crazy Disk Encoding Schemes](https://www.bigmessowires.com/2011/10/02/crazy-disk-encoding-schemes/)
- [CiderPress Nibble.cpp](https://github.com/fadden/ciderpress/blob/master/diskimg/Nibble.cpp)
- [Apple II Disk I/O Programming](https://mirrors.apple2.org.za/ground.icaen.uiowa.edu/MiscInfo/Programming/diskio)
