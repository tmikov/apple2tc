# Disk II Controller Hardware Reference

## 1. Controller Card Overview

The Disk II controller card (designed by Steve Wozniak) uses only 8 ICs:

| IC | Type | Function |
|----|------|----------|
| P5 | 256-byte PROM | Bootstrap ROM (boot code at $Cs00) |
| P6 | 256-byte PROM (28L22) | Logic State Sequencer ROM (state machine) |
| -- | 74LS174 | Hex D flip-flop (4-bit state register for sequencer) |
| -- | 74LS323 | 8-bit universal shift register (data register) |
| -- | 74LS259 | Addressable latch (stepper phase, motor, drive select) |
| -- | 556 | Dual timer (motor timeout) |
| -- | 74LS05 | Hex inverter (open collector) |
| -- | 74LS132 | Quad NAND Schmitt trigger |

The P6 sequencer ROM is the key innovation -- it replaces dozens of discrete logic ICs for
data separation and encoding.

## 2. Soft Switch Address Map

For a card in slot N, 16 soft switches are at `$C080 + (N * $10)` through `$C08F + (N * $10)`.
For the standard **slot 6**: `$C0E0`-`$C0EF`.

| Offset | Addr (Slot 6) | Name | Function |
|--------|---------------|------|----------|
| `+0` | `$C0E0` | PHASE0OFF | Turn off stepper phase 0 |
| `+1` | `$C0E1` | PHASE0ON | Turn on stepper phase 0 |
| `+2` | `$C0E2` | PHASE1OFF | Turn off stepper phase 1 |
| `+3` | `$C0E3` | PHASE1ON | Turn on stepper phase 1 |
| `+4` | `$C0E4` | PHASE2OFF | Turn off stepper phase 2 |
| `+5` | `$C0E5` | PHASE2ON | Turn on stepper phase 2 |
| `+6` | `$C0E6` | PHASE3OFF | Turn off stepper phase 3 |
| `+7` | `$C0E7` | PHASE3ON | Turn on stepper phase 3 |
| `+8` | `$C0E8` | MOTOROFF | Turn off drive motor |
| `+9` | `$C0E9` | MOTORON | Turn on drive motor |
| `+A` | `$C0EA` | DRIVE1 | Select drive 1 |
| `+B` | `$C0EB` | DRIVE2 | Select drive 2 |
| `+C` | `$C0EC` | Q6L | Clear Q6 (shift mode) |
| `+D` | `$C0ED` | Q6H | Set Q6 (load mode) |
| `+E` | `$C0EE` | Q7L | Clear Q7 (read mode) |
| `+F` | `$C0EF` | Q7H | Set Q7 (write mode) |

**Important behaviors:**
- Merely **accessing** the address (read or write) triggers the switch. The data value is
  irrelevant for control.
- Reading from any **even** address returns the data latch (shift register output).
  Odd addresses return 0 (but still trigger the switch).
- Writing to any address loads the data bus value into the controller's bus register
  (used for write operations when Q7=1, Q6=1).

## 3. Q6/Q7 Mode Combinations

| Q7 | Q6 | Mode | CPU Read Returns |
|----|----|------|------------------|
| 0 | 0 | **Read** (shift) | Data register contents |
| 0 | 1 | **Sense write-protect** | Bit 7 = write protect status |
| 1 | 0 | **Write** (shift) | Data register contents |
| 1 | 1 | **Write load** | Data register (loads bus value) |

### Typical read sequence:
```asm
    LDA $C0EE,X    ; Q7L: set read mode (Q7=0)
nibloop:
    LDA $C0EC,X    ; Q6L: read data register, shift mode
    BPL nibloop     ; loop until bit 7 set (byte ready)
    ; A now contains the nibble byte
```
(X = slot * 16, e.g. $60 for slot 6)

### Typical write-protect check:
```asm
    LDA $C0EE,X    ; Q7L: read mode
    LDA $C0ED,X    ; Q6H: sense write-protect
    BMI protected   ; bit 7 set = write protected
```

### Typical write sequence:
```asm
    LDA $C0EF,X    ; Q7H: write mode
    LDA data        ; byte to write
    STA $C0ED,X    ; Q6H: load shift register
    LDA $C0EC,X    ; Q6L: begin shifting out
    ; wait 32 cycles, then load next byte...
```

## 4. Stepper Motor

### Physical Arrangement

4-phase stepper motor with electromagnetic coils. Adjacent phases are 90 degrees apart.

- Energizing phases in sequence 0→1→2→3 moves head **outward** (higher tracks)
- Reverse sequence 3→2→1→0 moves head **inward** (toward track 0)
- Each phase step = **one half-track**
- Two phase steps = one full track
- Quarter-track positioning possible by energizing two adjacent coils simultaneously

### Phase Delta Table (Quarter-Track Movement)

When current magnetic position aligns with phase X and phase Y is activated:

| | Phase 0 ON | Phase 1 ON | Phase 2 ON | Phase 3 ON |
|---|---|---|---|---|
| **Currently at Phase 0** | 0 | +1 | +2 | -1 |
| **Currently at Phase 1** | -1 | 0 | +1 | +2 |
| **Currently at Phase 2** | -2 | -1 | 0 | +1 |
| **Currently at Phase 3** | +1 | -2 | -1 | 0 |

(Positive = outward/higher tracks, negative = inward/track 0)

### Seek Timing

| Operation | Time |
|-----------|------|
| Single track move | ~11.5 ms step + ~36 ms settle |
| Multi-track (accelerated) | ~8.5 ms per step during acceleration |
| Full seek (track 0 to 34) | < 200 ms |
| Recalibration to track 0 | ~1.5 seconds (80 half-steps at ~19.3 ms) |

The boot ROM recalibrates by executing 80 half-track phase steps inward (worst case from
track 40), hitting the mechanical stop at track 0.

### Track Position Tracking

- 35 physical tracks numbered 0-34
- 70 half-tracks (0-69), used by most emulators
- 140 quarter-tracks (0-139), needed for some copy protection
- Track number = half_track / 2 = quarter_track / 4
- Clamped to valid range (cannot go below 0 or above ~139)

## 5. Motor Control

- `MOTORON` ($C0E9): Enables motor for the currently selected drive
- `MOTOROFF` ($C0E8): Begins motor shutdown

**Motor coast-down**: After MOTOROFF, the motor continues spinning for approximately
**1 second** (~1,000,000 CPU cycles at 1 MHz). If MOTORON is accessed before the timer
expires, the timeout is canceled and the motor stays on.

**Spin-up time**: ~1 second from stopped to full speed.

## 6. The Logic State Sequencer (P6 ROM)

### Overview

The P6 PROM implements a 16-state finite state machine running at **2 MHz** (twice the
CPU clock). On each clock cycle, it outputs the next state and a shift register operation.

### ROM Address Formation

The 8-bit address into the P6 ROM:

| Bit | Signal | Source |
|-----|--------|--------|
| 7 | STATE[3] | 74LS174 flip-flop |
| 6 | STATE[2] | 74LS174 flip-flop |
| 5 | STATE[1] | 74LS174 flip-flop |
| 4 | STATE[0] | 74LS174 flip-flop |
| 3 | Q7 | Q7 latch |
| 2 | Q6 | Q6 latch |
| 1 | QA (SR MSB) | Bit 7 of shift register |
| 0 | RP (Read Pulse) | MC3470 pulse (1=flux transition detected) |

**Note on physical ROM**: The actual P6 PROM has scrambled bit ordering compared to
documentation. Emulators must rearrange address/data bits when loading raw ROM dumps.
Physical layout: `{STATE[0], STATE[2], STATE[3], PULSE, Q7, Q6, SR_MSB, STATE[1]}`

### ROM Output

| Bits | Field |
|------|-------|
| 7-4 | Next state (loaded into 74LS174) |
| 3-0 | Command for shift register |

### Sequencer Commands

| Code | Name | Operation | Description |
|------|------|-----------|-------------|
| `$0`-`$7` | CLR | `reg = 0x00` | Clear shift register |
| `$8` | NOP | (no change) | No operation |
| `$9` | SL0 | `reg = (reg << 1) \| 0` | Shift left, insert 0 |
| `$A` | SR | `reg = (reg >> 1) \| (WP ? 0x80 : 0)` | Shift right, insert write-protect |
| `$B` | LD | `reg = bus_value` | Load from CPU data bus |
| `$D` | SL1 | `reg = (reg << 1) \| 1` | Shift left, insert 1 |

**Note on SR**: If disk is write-protected, fills with 1s (0xFF). If not protected, shifts
right with 0 in MSB. This is how write-protect status is sensed via Q6H reads.

### How Reading Works

1. The sequencer runs at 2 MHz (8 cycles per 4us bit cell)
2. At **clock cycle 4** of each bit cell, the read head samples the disk for a pulse
3. A pulse (flux transition) = 1 bit, no pulse = 0 bit
4. The sequencer uses SL0/SL1 to shift bits into the data register
5. When the MSB of the shift register becomes 1 (QA=1), a complete byte is ready
6. The CPU polls `LDA $C0EC` and checks bit 7 (BPL loop)

### How Writing Works

1. Q7=1 puts sequencer in write mode
2. The write output equals **state bit 3** (states 8-F write 1, states 0-7 write 0)
3. CPU loads data via write to Q6H (LD command)
4. The shift register's MSB is sent to the write head each bit cell
5. Software must deliver a new byte every 32 CPU cycles (8 bits x 4us = 32us)

### Complete P6 ROM Data (16-Sector)

Organized as `rom[state * 16 + inputs]` where inputs = `{Q7, Q6, QA, RP}`:

```
State 0:  18 18 18 18 0A 0A 0A 0A 18 18 18 18 18 18 18 18
State 1:  2D 2D 38 38 0A 0A 0A 0A 28 28 28 28 28 28 28 28
State 2:  D8 38 08 28 0A 0A 0A 0A 39 39 39 39 3B 3B 3B 3B
State 3:  D8 48 48 48 0A 0A 0A 0A 48 48 48 48 48 48 48 48
State 4:  D8 58 D8 58 0A 0A 0A 0A 58 58 58 58 58 58 58 58
State 5:  D8 68 D8 68 0A 0A 0A 0A 68 68 68 68 68 68 68 68
State 6:  D8 78 D8 78 0A 0A 0A 0A 78 78 78 78 78 78 78 78
State 7:  D8 88 D8 88 0A 0A 0A 0A 08 08 88 88 08 08 88 88
State 8:  D8 98 D8 98 0A 0A 0A 0A 98 98 98 98 98 98 98 98
State 9:  D8 29 D8 A8 0A 0A 0A 0A A8 A8 A8 A8 A8 A8 A8 A8
State A:  CD BD D8 B8 0A 0A 0A 0A B9 B9 B9 B9 BB BB BB BB
State B:  D9 59 D8 C8 0A 0A 0A 0A C8 C8 C8 C8 C8 C8 C8 C8
State C:  D9 D9 D8 A0 0A 0A 0A 0A D8 D8 D8 D8 D8 D8 D8 D8
State D:  D8 08 E8 E8 0A 0A 0A 0A E8 E8 E8 E8 E8 E8 E8 E8
State E:  FD FD F8 F8 0A 0A 0A 0A F8 F8 F8 F8 F8 F8 F8 F8
State F:  DD 4D E0 E0 0A 0A 0A 0A 88 88 08 08 88 88 08 08
```

Each byte: high nibble = next state, low nibble = command.

## 7. MC3470 Read Amplifier

The MC3470 chip reads magnetic flux patterns and outputs a 1us pulse for each transition.
It has an internal automatic gain control (AGC) amplifier.

**Weak/fake bits**: When more than 2 consecutive zero bits appear (no transitions for > 8us),
the AGC amplifies background electrical noise, generating random spurious pulses. This is
exploited by some copy protection schemes. Emulators should generate random bits (~30%
chance of 1) when 4+ consecutive zeros are seen in the bitstream.

## 8. P5 Boot ROM (BOOT0)

The P5 PROM (256 bytes) is mapped at `$Cs00`-`$CsFF` (e.g., `$C600` for slot 6).

### Boot Process

1. **Build GCR translation table** at `$0300`-`$033F`: Tests each byte $00-$FF for valid
   6-and-2 encoding properties (bit 7 set, valid adjacent-bit patterns). 64 valid values
   get sequential indices.

2. **Determine slot number** from program counter on stack.

3. **Recalibrate to track 0**: 80 half-track steps inward.

4. **Read sector 0 of track 0**: Find address prologue ($D5 $AA $96), verify address,
   find data prologue ($D5 $AA $AD), read 342 GCR bytes, verify checksum, decode to
   `$0800`-`$08FF`.

5. **Jump to `$0801`**: Transfer control to BOOT1 code.

BOOT1 reads sectors 1-9 from track 0 into `$B600`-`$BFFF` (loads RWTS).
BOOT2 reads the rest of DOS from tracks 0-2 into `$9B00`-`$B5FF`.

## 9. Timing Specifications

| Parameter | Value |
|-----------|-------|
| CPU clock | ~1.023 MHz (14.318 MHz / 14) |
| Sequencer clock | ~2.046 MHz (2x CPU clock) |
| Bit cell period | 4 microseconds |
| Data rate | ~250 kbps (255.75 kbps) |
| Disk rotation speed | ~300 RPM |
| Time per revolution | ~200 ms (200,000 us) |
| Bits per track | ~50,000 |
| Bytes per track | ~6,250 (nibble bytes) |
| Tracks per disk | 35 (0-34) |
| Sectors per track (DOS 3.3) | 16 |
| Formatted capacity (DOS 3.3) | 140 KB (143,360 bytes) |
| Motor spin-up time | ~1 second |
| Motor coast-down time | ~1 second after MOTOROFF |
| Track density | 48 TPI |

## 10. Drive Connector Pinout (20-pin)

| Pin | Signal | Pin | Signal |
|-----|--------|-----|--------|
| 1 | GND | 2 | SEEKPH0 (stepper phase 0) |
| 3 | GND | 4 | SEEKPH1 |
| 5 | GND | 6 | SEEKPH2 |
| 7 | GND | 8 | SEEKPH3 |
| 9 | -12V | 10 | WRREQ (write request) |
| 11 | +5V | 12 | +5V |
| 13 | +12V | 14 | ENBL (drive enable) |
| 15 | +12V | 16 | RDDATA (read data pulses) |
| 17 | +12V | 18 | WRDATA (write data) |
| 19 | +12V | 20 | WRPROT (write protect sense) |

## Sources

- Jim Sather, "Understanding the Apple IIe" (1985), Chapter 9
- [The Amazing Disk II Controller Card](https://www.bigmessowires.com/2021/11/12/the-amazing-disk-ii-controller-card/)
- [Apple Disk II Interface Timing](https://embeddedmicro.weebly.com/apple-2iie.html)
- [P6 PROM Dump](https://embeddedmicro.weebly.com/apple-ii-p6-prom-dump.html)
- [P6 State Machine Analysis](https://embeddedmicro.weebly.com/apple-ii-prom-p6-statemachine.html)
- [CiderPress2 Nibble Notes](https://ciderpress2.com/formatdoc/Nibble-notes.html)
- [WOZ 1.0 Reference](https://applesaucefdc.com/woz/reference1/)
- [C600 Boot ROM Disassembly](https://mirrors.apple2.org.za/ground.icaen.uiowa.edu/MiscInfo/Programming/c600.disasm)
- [Apple II ROM Disassembly](https://6502disassembly.com/a2-rom/)
