# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

Apple2TC is an automated Apple II binary-to-C decompiler. It converts original Apple II game binaries into natively-running C code by analyzing runtime behavior through custom 6502 emulation. Successfully decompiled games include Robotron 2084 and Snake Byte.

## Build

CMake-based build requiring C++17 and C11. Use Ninja as the generator and build directories named `cmake-build-<config>`:

```shell
cmake -G Ninja -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
ninja -C cmake-build-debug
```

For release builds:
```shell
cmake -G Ninja -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
ninja -C cmake-build-release
```

Linux dependencies: `libx11-dev libxi-dev libxcursor-dev libasound2-dev mesa-common-dev libglvnd-dev`

Sanitizers can be enabled by uncommenting lines in the root `CMakeLists.txt`.

## Tests

Tests are in `tests/` and run via `tests/run-tests.sh <build-dir>`. The build directory is a required parameter. The workflow is: assemble `.s` files with `a6502`, decompile with `apple2tc -O3 --ir`, then diff against baseline `.ir` files.

```shell
cd tests
./run-tests.sh ../cmake-build-debug
```

## Code Style

- `.clang-format` configured: 2-space indent, 100 column limit, attached braces, pointer right-aligned (`char *p`)
- Assertions via `PANIC_ASSERT` macros from `support/` library

## Architecture

### Decompiler Pipeline (tools/apple2tc/)

The core decompiler uses a multi-stage IR pipeline:

1. **Disassembly** (`Disas.h/cpp`) — Jump-tracing 6502 disassembler producing `AsmBlock` basic blocks, augmented with runtime code coverage data from the emulator
2. **IR Generation** (`GenIR.h/cpp`) — Translates `AsmBlock`s into an intermediate representation, mapping 6502 registers/memory to IR operations
3. **IR** (`ir/IR.h`) — Module/Function/BasicBlock/Instruction hierarchy with type system (U8, U16, U32, CPUReg, Void)
4. **Optimization** — SSA conversion (`SSA.cpp`), dead code elimination (`dce.cpp`), simplification (`simplify.cpp`), register liveness (`CPURegLiveness.cpp`)
5. **Code Generation** — `PrintSimpleC.cpp` emits switch-based C code; assembly listing also available

Output format flags: `--simple-c`, `--ir`, `--irc1`, `--asm` (default)

### Libraries (lib/)

- **`d6502`** — 6502 instruction definitions, opcode tables, Apple II symbol database
- **`cpuemu`** — 6502 CPU emulator (`Emu6502` base, `EmuApple2` with Apple II IO handling for 0xC000-0xCFFF range)
- **`a2io`** — Apple II I/O: sound queue/speaker emulation, graphics rendering (Text/GR/HGR modes), screen-to-RGB conversion. Written in C, used by both the emulator and generated decompiled code
- **`sokol`** — Bundled cross-platform graphics/audio framework (no external deps)
- **`support`** — File I/O, string formatting, debug assertions
- **`decapplib`** — Runtime support for decompiled binaries (watch addresses, debug regions)

### Tools (tools/)

- **`a2emu`** — Apple II emulator with runtime data collection for the decompiler. Bundles Bolo and Robotron games (F1/F2 to load)
- **`apple2tc`** — The decompiler itself
- **`a6502`** — 6502 two-pass symbolic assembler (produces bit-accurate ROM images)
- **`id`** — Interactive disassembler for binary exploration

### Decoded Examples (decoded/)

Working decompilations in `decoded/rom/` (BASIC ROM), `decoded/robotron/`, `decoded/snake-byte/`, and `decoded/bolo/` (WIP). These build as part of the CMake project and link against `a2io` and `sokol`.

## Key Conventions

- Apple II memory: 0x0000-0xFFFF, IO at 0xC000-0xCFFF, screen pages at 0x0400/0x0800 (text) and 0x2000/0x4000 (HGR)
- CPU state in `Regs` struct: PC, A, X, Y, SP, status flags (N, V, B, D, I, Z, C)
- Input binaries use `.b33` format: 4-byte header (2-byte LE load address + 2-byte LE length) followed by raw binary data, loaded directly into emulator memory at the specified address
- Runtime data from emulator passed to decompiler via `--run-data=<file>.json`
- Public headers are in `include/apple2tc/`
