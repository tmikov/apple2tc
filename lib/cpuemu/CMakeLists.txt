add_library(cpuemu
  emu6502.cpp ${A2TC_INC}/emu6502.h
  apple2.cpp applesoft.cpp ${A2TC_INC}/apple2.h
  )

