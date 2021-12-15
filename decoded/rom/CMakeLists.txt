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
  set(SOKOL_SRC sokol.c soundqueue.c soundqueue.h)
endif ()

link_libraries(a2io)

add_executable(rom
  main.c ${A2TC_INC}/system.h ${A2TC_INC}/system-inc.h
  rom.c
  ${SOKOL_SRC} sokol_app.h sokol_gfx.h sokol_glue.h
  )

if (EMSCRIPTEN)
  target_link_options(rom PRIVATE --shell-file ${CMAKE_CURRENT_SOURCE_DIR}/shell.html)
elseif (APPLE)
  target_link_libraries(rom
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
  target_link_libraries(rom
    X11 Xi Xcursor GL dl pthread m)
endif ()
