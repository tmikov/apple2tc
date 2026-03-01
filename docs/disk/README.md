# Apple II Floppy Disk Support -- Documentation

This directory contains comprehensive technical documentation for implementing Disk II
floppy disk controller emulation in Apple2TC.

## Document Index

| File | Contents |
|------|----------|
| [hardware.md](hardware.md) | Disk II controller hardware: soft switches, stepper motor, Q6/Q7 modes, Logic State Sequencer (P6 ROM), P5 boot ROM, timing |
| [encoding.md](encoding.md) | GCR encoding: 6-and-2 and 4-and-4 schemes, prenibblize/denibblize algorithms, self-sync bytes, sector format on disk |
| [disk-formats.md](disk-formats.md) | Disk image file formats: DSK/DO/PO, NIB, WOZ 1.0/2.0, 2IMG. Sector interleave tables |
| [dos33-prodos.md](dos33-prodos.md) | Filesystem structures: DOS 3.3 VTOC/catalog/TSL, ProDOS volume/directory/file storage, boot process |
| [implementation-guide.md](implementation-guide.md) | Practical implementation guide: architecture patterns from AppleWin/mii_emu/apple2js/MAME, simplified vs full sequencer, key data structures |
| [tables.h](tables.h) | Ready-to-use C header with all encoding/decoding tables, sector interleave maps |

## Key References

- Jim Sather, "Understanding the Apple IIe" (1985) -- Chapter 9 is the definitive hardware reference
- Don Worth & Pieter Lechner, "Beneath Apple DOS" (1981) -- Definitive DOS 3.3 format reference
- [WOZ 2.0 Specification](https://applesaucefdc.com/woz/reference2/)
- [CiderPress2 Nibble Notes](https://ciderpress2.com/formatdoc/Nibble-notes.html)
- [The Amazing Disk II Controller Card](https://www.bigmessowires.com/2021/11/12/the-amazing-disk-ii-controller-card/)

## Key Reference Implementations

- [AppleWin](https://github.com/AppleWin/AppleWin) -- `source/Disk.cpp` (C++, full WOZ + DSK support)
- [mii_emu](https://github.com/buserror/mii_emu) -- `src/drivers/mii_disk2.c` (C, faithful LSS ROM emulation)
- [apple2js](https://github.com/whscullin/apple2js) -- `js/cards/disk2.ts` (TypeScript, clean two-tier architecture)
- [MAME](https://github.com/mamedev/mame) -- `src/devices/machine/wozfdc.cpp` (C++, cycle-accurate)
- [izapple2](https://github.com/ivanizag/izapple2) -- `cardDisk2Sequencer.go` (Go, both simplified and full)
