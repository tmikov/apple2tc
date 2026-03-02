# DOS 3.3 and ProDOS Disk Organization

## Part 1: DOS 3.3

### 1.1 Disk Geometry

| Parameter | Value |
|-----------|-------|
| Tracks | 35 (0-34, $00-$22) |
| Sectors per track | 16 (0-15, $00-$0F) |
| Bytes per sector | 256 |
| Total capacity | 143,360 bytes (140 KB) |

Track usage:
- **Tracks 0-2**: DOS image (boot code, RWTS, File Manager)
- **Track 17 ($11)**: VTOC and catalog (filesystem metadata)
- **Remaining tracks**: File data, allocated outward from track 17

### 1.2 VTOC (Volume Table of Contents) -- Track 17, Sector 0

| Offset | Size | Field |
|--------|------|-------|
| `$00` | 1 | Reserved ($04 for DOS 3.3, $02 for DOS 3.2) |
| `$01` | 1 | First catalog track (typically $11) |
| `$02` | 1 | First catalog sector (typically $0F) |
| `$03` | 1 | DOS version ($03 for DOS 3.3) |
| `$06` | 1 | Disk volume number (1-254) |
| `$27` | 1 | Max T/S pairs per TSL ($7A = 122) |
| `$30` | 1 | Last allocated track |
| `$31` | 1 | Allocation direction ($01=higher, $FF=lower) |
| `$34` | 1 | Tracks per disk ($23 = 35) |
| `$35` | 1 | Sectors per track ($10 = 16) |
| `$36-$37` | 2 | Bytes per sector ($00 $01 = 256, LE) |
| `$38-$C3` | 140 | Free sector bitmaps (4 bytes per track, 35 tracks) |

**Bitmap format** (4 bytes per track, only first 2 used for 16-sector):
```
Byte 0: [S15][S14][S13][S12][S11][S10][S9][S8]
Byte 1: [S7] [S6] [S5] [S4] [S3] [S2] [S1][S0]
```
`1` = free, `0` = used. Freshly formatted: `$FF $FF $00 $00`.

**Allocation strategy**: DOS allocates starting from track 17, alternating direction.
Tracks 18, 19, ... 34, then 16, 15, ... 0.

### 1.3 Catalog Structure -- Track 17, Sectors 15 down to 1

Linked list of sectors, each containing 7 file entries:

| Offset | Size | Field |
|--------|------|-------|
| `$00` | 1 | Unused |
| `$01` | 1 | Next catalog track ($00 = end) |
| `$02` | 1 | Next catalog sector |
| `$0B-$2D` | 35 | File entry 1 |
| `$2E-$50` | 35 | File entry 2 |
| ... | | ... |
| `$DD-$FF` | 35 | File entry 7 |

Max 15 catalog sectors x 7 entries = **105 files** maximum.

### 1.4 File Entry Format (35 bytes)

| Offset | Size | Field |
|--------|------|-------|
| `$00` | 1 | First TSL track ($FF = deleted, $00 = never used) |
| `$01` | 1 | First TSL sector |
| `$02` | 1 | File type + lock flag |
| `$03-$20` | 30 | Filename (high-ASCII, padded with $A0) |
| `$21-$22` | 2 | Sector count (LE, for display only) |

**File type byte** (`$02`):
- Bit 7: Lock flag (1=locked)
- Bits 0-6: Type code

| Value | Type | Description |
|-------|------|-------------|
| $00 | T | Text |
| $01 | I | Integer BASIC |
| $02 | A | Applesoft BASIC |
| $04 | B | Binary file |
| $08 | S | Special/typeless |
| $10 | R | Relocatable object |

**Deletion**: Offset `$00` set to `$FF`, original track saved at `$20` (allows undelete).

**File content prefixes**:
- Binary (B): 2 bytes load address + 2 bytes length
- Applesoft (A): 2 bytes program length
- Integer BASIC (I): 2 bytes program length

### 1.5 Track/Sector List (TSL)

Maps file's logical sectors to physical locations. Linked list:

| Offset | Size | Field |
|--------|------|-------|
| `$01` | 1 | Next TSL track ($00 = end) |
| `$02` | 1 | Next TSL sector |
| `$05-$06` | 2 | Sector offset in file (LE) |
| `$0C-$FF` | 244 | Up to 122 track/sector pairs (2 bytes each) |

A `$00 $00` entry = sparse sector (reads as 256 zero bytes).

### 1.6 Boot Process

**Stage 0 -- Autostart ROM**: Scans slots 7 down to 1. Transfers to `$Cs00`.

**Stage 1 -- BOOT0** (`$C600`): P5 boot ROM. Builds GCR decode table. Recalibrates to
track 0. Reads T0/S0 into `$0800`. Jumps to `$0801`.

**Stage 2 -- BOOT1** (`$0800`): Reads sectors 1-9 from track 0 into `$B600`-`$BFFF`
(loads RWTS).

**Stage 3 -- BOOT2** (`$B700`): Uses RWTS to read rest of DOS from tracks 0-2 into
`$9B00`-`$B5FF`.

**Stage 4 -- DOS Cold Start** (`$9D84`): Initializes data structures, runs HELLO program.

### 1.7 DOS 3.3 Memory Map

| Address | Component |
|---------|-----------|
| `$9600-$9CFF` | Disk I/O buffers |
| `$9D00-$BFFF` | DOS routines (8,960 bytes) |
| `$9D84` | Cold start entry |
| `$9DBF` | Warm start entry |
| `$AAC9-$B5FF` | File Manager (entry at $AAFD) |
| `$B600-$B6FF` | BOOT1 image |
| `$B700-$B7FF` | BOOT2 and IOB |
| `$B800-$BFFF` | RWTS (entry at $BD00) |

**Page 3 jump vectors**:

| Address | Target | Function |
|---------|--------|----------|
| `$03D0` | `$9DBF` | DOS warm start |
| `$03D3` | `$9D84` | DOS cold start |
| `$03D6` | `$AAFD` | File Manager |
| `$03D9` | `$B7B5` | RWTS entry |
| `$03E3` | -- | Get IOB address (A=hi, Y=lo) |
| `$03EA` | -- | Reconnect DOS |

### 1.8 RWTS Interface

Programs call RWTS via `JSR $03D9` after setting up an IOB (14 bytes, default at `$B7E8`):

| Offset | Field | Description |
|--------|-------|-------------|
| `$00` | Table type | Always $01 |
| `$01` | Slot x 16 | $60 for slot 6 |
| `$02` | Drive | $01 or $02 |
| `$03` | Volume | Expected ($00 = any) |
| `$04` | Track | Target ($00-$22) |
| `$05` | Sector | Target ($00-$0F) |
| `$06-$07` | DCT pointer | Device Characteristics Table address |
| `$08-$09` | Buffer pointer | 256-byte data buffer address |
| `$0C` | Command | $00=Seek, $01=Read, $02=Write, $04=Format |
| `$0D` | Return code | Error code on return |

**Calling sequence**:
```asm
    JSR $03E3      ; Get IOB address -> A(hi), Y(lo)
    STA ptr+1
    STY ptr
    ; ... set fields ...
    LDA ptr+1
    LDY ptr
    JSR $03D9      ; Call RWTS
    BCS error      ; Carry set = error
```

---

## Part 2: ProDOS

### 2.1 Block-Based Addressing

ProDOS uses 512-byte blocks instead of track/sector pairs:
- 5.25" floppy: 280 blocks (35 tracks x 8 blocks/track)
- Maximum volume: 65,535 blocks (~32 MB)

### 2.2 Volume Layout

| Block(s) | Contents |
|----------|----------|
| 0 | Boot loader |
| 1 | SOS boot (Apple ///) |
| 2-5 | Volume directory (4 blocks typical) |
| 6+ | Volume bitmap, then file data |

### 2.3 Volume Directory Header (Block 2, starting at offset $04)

Each directory block starts with prev/next block pointers (offsets $00-$03).

| Offset | Size | Field |
|--------|------|-------|
| `$04` | 1 | Storage type/name length (high nibble = $F for vol dir) |
| `$05-$13` | 15 | Volume name (ASCII) |
| `$1C-$1F` | 4 | Creation date/time |
| `$23` | 1 | Entry length ($27 = 39) |
| `$24` | 1 | Entries per block ($0D = 13) |
| `$25-$26` | 2 | File count (LE) |
| `$27-$28` | 2 | Bitmap pointer (block#) |
| `$29-$2A` | 2 | Total blocks (LE) |

### 2.4 File Entry Format (39 bytes)

| Offset | Size | Field |
|--------|------|-------|
| `$00` | 1 | Storage type / name length |
| `$01-$0F` | 15 | Filename (ASCII) |
| `$10` | 1 | File type |
| `$11-$12` | 2 | Key block (LE) |
| `$13-$14` | 2 | Blocks used (LE) |
| `$15-$17` | 3 | EOF (24-bit LE) |
| `$18-$1B` | 4 | Creation date/time |
| `$1E` | 1 | Access flags |
| `$1F-$20` | 2 | Aux type (LE) |
| `$21-$24` | 4 | Last modification date/time |
| `$25-$26` | 2 | Header pointer (directory's key block) |

**Storage types** (high nibble of `$00`):

| Value | Meaning |
|-------|---------|
| $0 | Deleted/inactive |
| $1 | Seedling (1 data block, EOF <= 512) |
| $2 | Sapling (1 index + data blocks, EOF <= 131,072) |
| $3 | Tree (1 master index + index + data, EOF <= 16,777,215) |
| $D | Subdirectory entry |
| $E | Subdirectory header |
| $F | Volume directory header |

**Common file types**:

| Code | Name | Description |
|------|------|-------------|
| $04 | TXT | Text |
| $06 | BIN | Binary (aux=load address) |
| $0F | DIR | Directory |
| $FC | BAS | Applesoft BASIC |
| $FF | SYS | System file |

### 2.5 File Storage

**Seedling** (type $1): Key pointer → single data block.

**Sapling** (type $2): Key pointer → index block containing up to 256 data block pointers.
Pointer storage: **low bytes at [0-127], high bytes at [128-255]** of the index block.

**Tree** (type $3): Key pointer → master index block → up to 128 index blocks → data blocks.
Same split storage in both master index and index blocks.

**Sparse files**: Block pointer of $0000 = unallocated (reads as zeros).

### 2.6 Volume Bitmap

One bit per block: `1` = free, `0` = in use. Packed left-to-right within bytes (high bit =
lowest block number). One 512-byte bitmap block covers 4,096 blocks.

### 2.7 Date/Time Format

```
Word 1 (date):  YYYYYYY MMMM DDDDD   (7/4/5 bits)
Word 2 (time):  000HHHHH 00MMMMMM     (5/6 bits)
```

### 2.8 ProDOS Boot Process

1. Disk II ROM loads block 0 to `$0800`, jumps to `$0801`
2. Boot loader reads volume directory, finds `PRODOS` (type $FF), loads to `$2000`
3. ProDOS kernel initializes, relocates to upper memory
4. Finds first `*.SYSTEM` file (type $FF), loads at `$2000`, executes

### 2.9 MLI (Machine Language Interface)

```asm
    JSR $BF00      ; MLI entry point
    .BYTE cmd      ; Command number
    .WORD plist    ; Pointer to parameter list
```

Key commands: $C8 (OPEN), $CA (READ), $CB (WRITE), $CC (CLOSE), $80 (READ_BLOCK),
$81 (WRITE_BLOCK).

---

## Part 3: How Games Access the Disk

### Standard DOS 3.3 Access

BASIC programs use `PRINT CHR$(4)"command"`. Machine language programs call RWTS via
`JSR $03D9`.

### Direct RWTS Access

Many games bypass the File Manager for speed and call RWTS directly with track/sector/buffer.

### Custom Boot Loaders

Copy-protected games often replace DOS entirely:
- Custom BOOT1 code at T0/S0
- Minimal RWTS with non-standard markers
- Direct track/sector loading without filesystem
- Modified address/data prologues, nibble counting, half-tracks

## Sources

- Don Worth & Pieter Lechner, "Beneath Apple DOS" (1981)
- [CiderPress2 DOS Notes](https://ciderpress2.com/formatdoc/DOS-notes.html)
- [CiderPress2 ProDOS Notes](https://ciderpress2.com/formatdoc/ProDOS-notes.html)
- [ProDOS Technical Reference](https://prodos8.com/docs/techref/)
- [Apple II Boot ROM Disassembly](https://mirrors.apple2.org.za/ground.icaen.uiowa.edu/MiscInfo/Programming/c600.disasm)
- [FPGA Disk Controller Boot Process](https://github.com/steve-chamberlin/fpga-disk-controller/blob/master/disk-II-boot-process.txt)
