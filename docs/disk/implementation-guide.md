# Disk II Emulation Implementation Guide

Practical implementation patterns distilled from studying AppleWin, mii_emu, apple2js,
MAME, izapple2, and Epple-II.

## 1. Two Approaches to Disk II Emulation

### How the Real Hardware Works

The Disk II controller reads data one **bit** at a time from the spinning disk. A hardware
state machine (the Logic State Sequencer / LSS, implemented in the P6 ROM) runs at 2 MHz
and assembles individual bits into bytes in an 8-bit shift register. When the shift register
fills up (bit 7 becomes 1), the byte is ready. Software reads it by polling the data latch
in a tight loop:

```asm
nibloop:
    LDA $C0EC       ; read shift register
    BPL nibloop     ; loop until bit 7 set
```

This creates a natural boundary: software only ever sees **complete bytes** (nibbles). It
never interacts with individual bits -- that's handled entirely by the hardware state machine.

### Simplified Approach: Nibble-Level Emulation

Because software only polls for complete bytes, we can skip emulating the bit-level state
machine entirely. Instead, we pre-convert the disk image into a circular buffer of nibble
bytes (the same bytes the real hardware would produce), and hand them to the CPU one at a
time when it reads $C0EC.

This works for **DSK** and **NIB** disk images, which contain sector data in a format that
can be converted to/from the nibble stream. Standard DOS 3.3 and ProDOS disks work
perfectly with this approach.

**What it can't do**: The simplified approach has no notion of individual bit timing. Some
copy-protected disks rely on non-standard bit patterns, timing between sectors, "weak bits"
(regions that read differently each time), or half-track data. Since the nibble stream is
pre-computed, none of these effects can be reproduced.

### Full Sequencer Approach: Bit-Level Emulation

To handle copy-protected disks, we must emulate the actual P6 state machine. On every CPU
cycle, we step the sequencer twice (it runs at 2 MHz), feeding it individual bits from the
disk's raw bitstream. The sequencer assembles bytes exactly as the real hardware would,
including all timing-dependent effects.

This is required for **WOZ** disk images, which store the raw magnetic flux data as a
bitstream rather than pre-decoded sector data. WOZ images preserve copy protection, weak
bits, non-standard sector formats, and quarter-track data.

### Summary

| | Simplified (DSK/NIB) | Full Sequencer (WOZ) |
|---|---|---|
| Data unit | Whole nibbles (8 bits) | Individual bits |
| Timing | Skip/toggle approximation | Cycle-accurate 2 MHz LSS |
| State machine | Q6/Q7 mode switch only | Full 16-state P6 ROM |
| Copy protection | Not supported | Fully supported |
| Write support | Direct nibble write | Bit-level write |
| Complexity | Low | High |

**Recommendation**: Start with the simplified approach for DSK/NIB support. This covers
the vast majority of disk images. Add the full sequencer later only if WOZ/copy-protection
support is needed.

## 2. Key Data Structures

```cpp
struct DiskController {
    // Shift register (74LS323)
    uint8_t data_register;

    // Sequencer state (74LS174, for full LSS mode)
    uint8_t lss_state;          // 4-bit, 0x0-0xF

    // Mode latches
    bool q6;                    // false=shift, true=load
    bool q7;                    // false=read, true=write

    // Motor and drive selection
    bool motor_on;
    uint64_t motor_off_cycle;   // cycle count when motor was turned off
    int selected_drive;         // 0 or 1

    // Stepper motor
    uint8_t magnet_states;      // bitmask of which phases are on
    int phase;                  // current half-track position (0-69)

    // Bus value (last value written by CPU)
    uint8_t bus_value;

    // Drives
    FloppyDrive drives[2];
};

struct FloppyDrive {
    // Track image (nibblized or raw bits)
    uint8_t *track_data;
    uint32_t track_length;      // bytes (NIB) or bits (WOZ)

    // Read/write position within track
    uint32_t position;          // byte offset (NIB) or bit offset (WOZ)

    // State
    bool write_protected;
    bool track_dirty;           // needs write-back
    int current_track;          // 0-34

    // For simplified mode
    bool skip;                  // toggle for timing approximation

    // For WOZ mode
    uint8_t head_window;        // 4-bit sliding window for MC3470
    double extra_cycles;        // fractional cycle accumulator
};
```

## 3. Simplified Approach (DSK/NIB)

The disk is represented as a circular buffer of ~6656 nibble bytes per track (either loaded
directly from a NIB file, or nibblized from a DSK file on load). The emulator maintains a
read position that advances through the buffer. When the CPU reads $C0EC (Q6L in read mode),
we return the next byte.

One subtlety: on real hardware, the shift register takes ~32us (32 CPU cycles) to fill.
Software polls in a tight `LDA $C0EC / BPL` loop that takes ~4 cycles per iteration, so it
reads the latch ~8 times before a valid byte appears. The simplified approach handles this
with a `skip` toggle -- alternating between returning 0 (bit 7 clear, so BPL loops) and the
actual nibble. This is crude but sufficient; the exact ratio doesn't matter as long as the
CPU sees valid bytes at a reasonable rate.

### Soft Switch Handler

```cpp
uint8_t disk_io(uint16_t addr, uint8_t value, bool is_write) {
    int offset = addr & 0x0F;
    FloppyDrive &drive = drives[selected_drive];

    switch (offset) {
    case 0x0: case 0x1: case 0x2: case 0x3:
    case 0x4: case 0x5: case 0x6: case 0x7:
        handle_stepper(offset);
        break;

    case 0x8:  // MOTOROFF
        motor_off_cycle = current_cycle;
        break;

    case 0x9:  // MOTORON
        motor_on = true;
        break;

    case 0xA:  // DRIVE1
        selected_drive = 0;
        break;

    case 0xB:  // DRIVE2
        selected_drive = 1;
        break;

    case 0xC:  // Q6L
        q6 = false;
        if (!q7) {
            // Read mode, shift mode: return next nibble
            return read_nibble(drive);
        }
        break;

    case 0xD:  // Q6H
        q6 = true;
        if (!q7) {
            // Read mode, load mode: sense write-protect
            return drive.write_protected ? 0xFF : 0x00;
        } else {
            // Write mode, load: load shift register
            if (is_write) data_register = value;
        }
        break;

    case 0xE:  // Q7L
        q7 = false;
        break;

    case 0xF:  // Q7H
        q7 = true;
        if (is_write) data_register = value;
        break;
    }

    // Even addresses return data register, odd return 0
    return (offset & 1) ? 0 : data_register;
}
```

### Simplified Read

The `skip` toggle approximates timing (only return a byte every other access):

```cpp
uint8_t read_nibble(FloppyDrive &drive) {
    // Advance position
    drive.position++;
    if (drive.position >= drive.track_length)
        drive.position = 0;

    // Only return valid data every other read (timing approximation)
    drive.skip = !drive.skip;
    if (drive.skip)
        return 0;  // bit 7 clear -> CPU loops

    return drive.track_data[drive.position];
}
```

### Simplified Write

```cpp
void write_nibble(FloppyDrive &drive, uint8_t value) {
    drive.track_data[drive.position] = value;
    drive.track_dirty = true;
    drive.position++;
    if (drive.position >= drive.track_length)
        drive.position = 0;
}
```

## 4. Stepper Motor Implementation

### Magnet-Based (Recommended)

```cpp
void handle_stepper(int offset) {
    int phase_num = offset >> 1;    // 0-3
    bool phase_on = offset & 1;     // odd = ON, even = OFF

    if (phase_on)
        magnet_states |= (1 << phase_num);
    else
        magnet_states &= ~(1 << phase_num);

    // Compute movement based on active magnets relative to current position
    int current_phase = (phase / 2) & 3;  // phase is in half-tracks
    int direction = 0;

    if (magnet_states & (1 << ((current_phase + 1) & 3)))
        direction += 1;
    if (magnet_states & (1 << ((current_phase + 3) & 3)))
        direction -= 1;

    if (direction != 0) {
        phase += direction;
        if (phase < 0) phase = 0;
        if (phase > 69) phase = 69;  // 35 tracks * 2 half-tracks - 1

        int new_track = phase / 2;
        if (new_track != drive.current_track) {
            flush_track(drive);
            load_track(drive, new_track);
        }
    }
}
```

### Alternative: Lookup Table (from mii_emu)

```cpp
static const int8_t phase_delta[4][4] = {
    { 0,  1,  2, -1},
    {-1,  0,  1,  2},
    {-2, -1,  0,  1},
    { 1, -2, -1,  0}
};
// quarter_track += phase_delta[current_quarter_phase][activated_phase];
```

## 5. Motor Timing

```cpp
static const uint64_t MOTOR_TIMEOUT_CYCLES = 1000000;  // ~1 second at 1 MHz

bool is_motor_spinning() {
    if (motor_on) return true;
    return (current_cycle - motor_off_cycle) < MOTOR_TIMEOUT_CYCLES;
}
```

## 6. DSK → Nibble Conversion

When loading a DSK file, each track must be nibblized. See encoding.md for the full
algorithm. High-level:

```cpp
void nibblize_track(int track, const uint8_t *dsk_data, bool is_dos_order,
                    uint8_t *nib_out, int *nib_len) {
    int pos = 0;

    // Gap 1: initial sync
    for (int i = 0; i < 48; i++)
        nib_out[pos++] = 0xFF;

    for (int phys_sector = 0; phys_sector < 16; phys_sector++) {
        // Map physical sector to file offset
        int logical = is_dos_order
            ? dos33_phys_to_logical[phys_sector]
            : prodos_phys_to_logical[phys_sector];
        const uint8_t *sector_data =
            &dsk_data[(track * 16 + logical) * 256];

        // Write address field
        write_address_field(nib_out, &pos, /*volume=*/254, track, phys_sector);

        // Gap 2
        for (int i = 0; i < 6; i++)
            nib_out[pos++] = 0xFF;

        // Write data field
        write_data_field(nib_out, &pos, sector_data);

        // Gap 3
        for (int i = 0; i < 27; i++)
            nib_out[pos++] = 0xFF;
    }

    *nib_len = pos;
}
```

## 7. Full LSS Sequencer (WOZ Support)

The full sequencer emulates the P6 ROM state machine that runs in the real hardware. On
each CPU cycle, the sequencer is stepped twice (2 MHz clock). Each step:

1. Reads one bit from the WOZ bitstream (a circular buffer of flux transitions)
2. Feeds it through a 4-bit sliding window simulating the MC3470 read amplifier
3. Looks up the P6 ROM using 8 address bits: {state[3:0], Q7, Q6, shift_reg[7], pulse}
4. The ROM output gives the next state (upper nibble) and a shift register command (lower nibble)
5. In write mode, state bit 3 drives the write head (states 8-F write a 1, states 0-7 write a 0)

The CPU interacts with this exactly as before (reading $C0EC, etc.) -- it just sees the
bytes that the sequencer has assembled, which now include all timing-dependent effects.

```cpp
void lss_tick(uint64_t cpu_cycles) {
    FloppyDrive &drive = drives[selected_drive];
    if (!is_motor_spinning()) return;

    // LSS runs at 2 MHz = 2 ticks per CPU cycle
    for (int tick = 0; tick < 2; tick++) {
        // Sample disk bit (at appropriate timing)
        bool pulse = false;
        if (tick == 0 && !q7) {  // read mode, sample at first tick of pair
            // Read next bit from WOZ bitstream
            uint8_t bit = get_next_woz_bit(drive);

            // MC3470 head window simulation
            drive.head_window = ((drive.head_window << 1) | bit) & 0x0F;
            if (drive.head_window == 0) {
                // Fake bit generation
                pulse = (rand() % 10) < 3;
            } else {
                pulse = (drive.head_window & 0x02) != 0;
            }
        }

        // Form ROM address
        uint8_t rom_addr =
            (lss_state << 4) |
            (q7 ? 0x08 : 0) |
            (q6 ? 0x04 : 0) |
            ((data_register & 0x80) ? 0x02 : 0) |
            (pulse ? 0x01 : 0);

        uint8_t rom_out = p6_rom[rom_addr];
        lss_state = (rom_out >> 4) & 0x0F;

        // Execute command (lower nibble)
        uint8_t cmd = rom_out & 0x0F;
        if ((cmd & 0x08) == 0) {
            data_register = 0x00;           // CLR
        } else {
            switch (cmd & 0x07) {
            case 0x0: break;                // NOP
            case 0x1: data_register <<= 1;  // SL0
                break;
            case 0x2:                        // SR
                data_register = (data_register >> 1) |
                    (drive.write_protected ? 0x80 : 0x00);
                break;
            case 0x3: data_register = bus_value;  // LD
                break;
            case 0x5: data_register = (data_register << 1) | 1;  // SL1
                break;
            }
        }

        // Write output (state bit 3 = write head)
        if (q7 && (lss_state & 0x08)) {
            write_bit_to_track(drive, 1);
        } else if (q7) {
            write_bit_to_track(drive, 0);
        }
    }
}
```

### P6 ROM Loading

The physical P6 ROM has scrambled bit ordering. When loading a raw ROM dump, bits must
be rearranged. Alternatively, use the pre-decoded table from hardware.md Section 6.

## 8. Integration with Apple2TC

### Where to Hook In

The existing `EmuApple2` class handles IO at $C000-$CFFF. The Disk II controller should be
registered for slot 6 IO addresses $C0E0-$C0EF (and ROM space $C600-$C6FF).

Key integration points:
1. **IO read/write**: Hook `$C0E0`-`$C0EF` soft switches
2. **Slot ROM**: Map P5 boot ROM at `$C600`-`$C6FF`
3. **Cycle counting**: The disk controller needs to know elapsed CPU cycles for motor
   timing and (if doing full LSS) bit-level disk rotation

### Disk Image Loading

Support these formats in order of priority:
1. **DSK/DO** -- Most common, easiest. Nibblize on load.
2. **PO** -- Same as DSK but ProDOS sector order.
3. **NIB** -- Already nibblized, load directly.
4. **WOZ** -- For copy-protected disks. Requires full LSS.
5. **2IMG** -- Wrapper, detect inner format from header.

### P5 Boot ROM

The 256-byte boot ROM must be loaded at $Cs00 (slot ROM space). This is the code the
Apple II executes when it tries to boot from the disk. You can:
- Use the actual ROM dump (widely available)
- Extract from an existing Apple II ROM image

## 9. Testing Strategy

1. **Boot DOS 3.3**: Load a standard DOS 3.3 disk image, verify it boots to `]` prompt
2. **Boot ProDOS**: Load a ProDOS disk, verify BASIC.SYSTEM loads
3. **File operations**: CATALOG, LOAD, SAVE, BLOAD, BRUN
4. **Game loading**: Test with known game disk images
5. **Write support**: Format a disk, save files, verify data integrity
6. **Two drives**: Test with two disk images mounted simultaneously

## Sources

- [AppleWin](https://github.com/AppleWin/AppleWin) -- `source/Disk.cpp`
- [mii_emu](https://github.com/buserror/mii_emu) -- `src/drivers/mii_disk2.c`
- [apple2js](https://github.com/whscullin/apple2js) -- `js/cards/disk2.ts`
- [MAME](https://github.com/mamedev/mame) -- `src/devices/machine/wozfdc.cpp`
- [izapple2](https://github.com/ivanizag/izapple2) -- `cardDisk2Sequencer.go`
- [Epple-II](https://github.com/cmosher01/Epple-II) -- `src/diskcontroller.cpp`
