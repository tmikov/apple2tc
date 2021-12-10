/*
 * Copyright (c) Tzvetan Mikov.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "apple2tc/d6502.h"

#include <cstdlib>

void genDisasmTable() {
  printf("static const CPUOpcode s_opcodes[256] = {\n");
  for (unsigned i = 0; i != 256; ++i) {
    CPUOpcode opc = decodeOpcode((uint8_t)i);
    printf(
        "  /* $%02X */ { CPUInstKind::%s, CPUAddrMode::%s },\n",
        i,
        opc.kind != CPUInstKind::INVALID ? cpuInstName(opc.kind) : "INVALID",
        opc.kind != CPUInstKind::INVALID ? cpuAddrModeName(opc.addrMode) : "_invalid");
  }
  printf("};\n");
}

void genAsmTable() {
  AsmEncoding opcodes[256];
  for (unsigned i = 0; i != 256; ++i) {
    opcodes[i].opcode = decodeOpcode((uint8_t)i);
    opcodes[i].encoding = i;
  }

  qsort(opcodes, 256, sizeof(opcodes[0]), [](const void *_a, const void *_b) -> int {
    const auto *a = (AsmEncoding *)_a;
    const auto *b = (AsmEncoding *)_b;
    return (int)a->opcode.kind * ((int)CPUAddrMode::_invalid + 2) + (int)a->opcode.addrMode -
        ((int)b->opcode.kind * ((int)CPUAddrMode::_invalid + 2) + (int)b->opcode.addrMode);
  });

  printf("static const AsmEncoding s_encodings[] = {\n");
  unsigned i;
  for (i = 0; i != 256 && opcodes[i].opcode.kind != CPUInstKind::INVALID; ++i) {
    auto opc = opcodes[i].opcode;
    printf(
        "  { { CPUInstKind::%s, CPUAddrMode::%s }, 0x%02X },\n",
        opc.kind != CPUInstKind::INVALID ? cpuInstName(opc.kind) : "INVALID",
        opc.kind != CPUInstKind::INVALID ? cpuAddrModeName(opc.addrMode) : "_invalid",
        opcodes[i].encoding);
  }
  printf("};\n");
  printf("static const unsigned s_encodings_len = %u;\n", i);
}

static void genSortedNames() {
  struct NameKind {
    const char *name;
    CPUInstKind kind;
  };
  static constexpr unsigned COUNT = (unsigned)CPUInstKind::_last;
  NameKind names[(unsigned)CPUInstKind::_last];
  for (unsigned i = 0; i != COUNT; ++i) {
    names[i].name = cpuInstName((CPUInstKind)i);
    names[i].kind = (CPUInstKind)i;
  }

  qsort(
      names,
      COUNT,
      sizeof(names[0]),
      [](const void *a, const void *b) -> int {
        return strcmp(
            static_cast<const NameKind *>(a)->name, static_cast<const NameKind *>(b)->name);
      });

  printf("static const char s_sortedNames[][4] = {\n");
  for (unsigned i = 0; i != COUNT; ++i)
    printf("  \"%s\",\n", names[i].name);
  printf("};\n");
  printf("static const CPUInstKind s_sortedNameKinds[] = {\n");
  for (unsigned i = 0; i != COUNT; ++i)
    printf("  CPUInstKind::%s,\n", cpuInstName(names[i].kind));
  printf("};\n");
}

int main() {
  genDisasmTable();
  printf("\n");
  genAsmTable();
  printf("\n");
  genSortedNames();
  return 0;
}