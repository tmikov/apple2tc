# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

if (EMSCRIPTEN)
  add_definitions(-DSOKOL_GLES2)
  set(SOKOL_SRC sokol.c)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
elseif (APPLE)
  add_definitions(-DSOKOL_GLCORE33)
  #  add_definitions(-DSOKOL_METAL)
  set(SOKOL_SRC sokol.m)
else ()
  add_definitions(-DSOKOL_GLCORE33)
  set(SOKOL_SRC sokol.c)
endif ()

link_libraries(d6502 cpuemu)
add_executable(a2emu a2emu.cpp
  blit.h
  font.cpp font.h
  apple2plus_rom.cpp apple2plus_rom.h
  ${SOKOL_SRC} sokol_app.h sokol_gfx.h sokol_glue.h
  )

if (EMSCRIPTEN)
  target_link_options(a2emu PRIVATE --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/shell.html)
elseif (APPLE)
  target_link_libraries(a2emu
    "-framework Cocoa"
    "-framework QuartzCore"
    "-framework OpenGL"
    "-framework AudioToolbox")
  #  link_libraries(
  #    "-framework Cocoa"
  #    "-framework QuartzCore"
  #    "-framework Metal"
  #    "-framework MetalKit")
else ()
  #NOTE: not tested yet.
  target_link_libraries(a2emu
    X11 Xi Xcursor GL dl pthread m)
endif ()
