# Disk II Support -- Design

## Goals

- Add Disk II floppy controller emulation to Apple2TC
- Support both the emulator (a2emu) and decompiled binaries
- Simplified (nibble-based) approach, DSK/DO format, read-only
- Full sequencer (WOZ) and write support are future work

## Architecture

Disk controller lives in `lib/a2io/` as a C module, embedded in the existing
`a2_iostate_t` struct. Decompiled binaries get disk support automatically since
they already use `a2_io_peek`/`a2_io_poke`.

### Files

| File | Change |
|------|--------|
| `lib/a2io/disk2.c` | New. Controller: init, mount, soft switch handler, nibblization, P5 boot ROM |
| `lib/a2io/disk2_tables.h` | New. Copy of `doc/disk/tables.h` -- GCR tables, interleave maps, constants |
| `include/apple2tc/a2io.h` | Edit. Add `a2_disk2_t` struct and function declarations |
| `lib/a2io/a2io.c` | Edit. Dispatch $C0E0-$C0EF and $C600-$C6FF to disk2 functions |
| `lib/a2io/CMakeLists.txt` | Edit. Add `disk2.c` to sources |
| `tools/a2emu/a2emu.cpp` | Edit. Add `--disk1`/`--disk2` CLI args, load DSK, mount, boot from ROM |

No changes to the CPU emulator, IR pipeline, decompiler, or other tools.

## State

```c
typedef struct {
    // Controller
    uint8_t data_register;      // 74LS323 shift register
    bool q6, q7;                // Mode latches
    bool motor_on;
    uint64_t motor_off_cycle;   // Cycle when motor was turned off
    int selected_drive;         // 0 or 1

    // Stepper
    uint8_t phases;             // Bitmask of active phase magnets
    int phase_position;         // Half-track 0-69

    // Drives
    struct {
        uint8_t *nib_data;      // Nibblized track data (all 35 tracks)
        uint32_t nib_track_len; // Bytes per track (6656)
        uint32_t position;      // Byte offset within current track
        bool write_protected;
        int current_track;      // 0-34
        bool skip;              // Timing toggle for read
        bool mounted;
    } drive[2];
} a2_disk2_t;
```

Embedded as a field in `a2_iostate_t`.

## Soft Switch Handler ($C0E0-$C0EF)

| Offset | Name | Action |
|--------|------|--------|
| 0x0-0x7 | PHASE0-3 OFF/ON | Update `phases` bitmask, compute stepper movement, clamp to 0-69 half-tracks, update current track |
| 0x8 | MOTOROFF | Record `motor_off_cycle`, clear `motor_on` |
| 0x9 | MOTORON | Set `motor_on = true` |
| 0xA | DRIVE1 | `selected_drive = 0` |
| 0xB | DRIVE2 | `selected_drive = 1` |
| 0xC | Q6L | `q6 = false`. If `!q7`: return `read_nibble()` |
| 0xD | Q6H | `q6 = true`. If `!q7`: return write-protect status |
| 0xE | Q7L | `q7 = false` |
| 0xF | Q7H | `q7 = true` |

Even offsets return `data_register`, odd offsets return 0.

### read_nibble()

Advance position in current track's nibble buffer. Toggle `skip` flag; when
skip is true, return 0x00 (simulates ~32-cycle byte-ready timing -- the CPU's
BPL loop spins until bit 7 is set). Return 0x00 if motor is not spinning.

### Motor timeout

`motor_on || (current_cycle - motor_off_cycle < 1,000,000)` (~1 second
coast-down at 1.023 MHz).

### Stepper

Magnet-based: compute direction from active phases vs. current phase position.
Move one half-track per step. Clamp to 0-69.

## DSK Loading & Nibblization

On `a2_disk2_mount()`:

1. Validate file size == 143,360 bytes
2. Allocate 35 * 6,656 = 232,960 bytes for nibblized data
3. Nibblize all 35 tracks:
   - For each of 16 physical sectors:
     - Map physical → DOS 3.3 logical via `dos33_phys_to_logical[]`
     - Write address field (D5 AA 96, 4-and-4 encoded vol/trk/sec/checksum, DE AA EB)
     - Write data field (D5 AA AD, prenibblize + XOR + GCR encode 256→342 bytes, checksum, DE AA EB)
     - Write sync gaps between fields and sectors

## Boot ROM

P5 boot ROM (256 bytes) embedded as a static const array in `disk2.c`.
Served via `a2_disk2_rom_peek()` for reads in $C600-$C6FF.

## Emulator Integration

- `--disk1 <path.dsk>` and `--disk2 <path.dsk>` CLI flags
- On startup: read DSK file, call `a2_disk2_mount()`
- With `--disk1`, emulator boots from ROM (Autostart ROM finds P5 at $C600)
- Without `--disk1`, existing .b33 behavior unchanged

## Decompiled Binary Integration

Decompiled code already calls `a2_io_peek`/`a2_io_poke`. Disk access works
once `a2_disk2_mount()` is called on the `a2_iostate_t` at startup.

## Testing

Boot a DOS 3.3 system disk with `a2emu --disk1 dos33.dsk` and verify it
reaches the `]` prompt.

## Future Work

- Write support (dirty track flush back to DSK)
- PO (ProDOS order) and NIB format support
- WOZ 1.0/2.0 with full P6 ROM sequencer for copy-protected disks
