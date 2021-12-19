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

add_library(sokol STATIC
  ${SOKOL_SRC}
  ${A2TC_INC}/sokol/blit.h
  ${A2TC_INC}/sokol/sokol_app.h
  ${A2TC_INC}/sokol/sokol_audio.h
  ${A2TC_INC}/sokol/sokol_gfx.h
  ${A2TC_INC}/sokol/sokol_glue.h
  ${A2TC_INC}/sokol/sokol_time.h
  )

if (APPLE)
  target_link_libraries(sokol
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
  target_link_libraries(sokol
    X11 Xi Xcursor GL dl pthread m)
endif ()
