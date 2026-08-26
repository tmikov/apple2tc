# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

cmake_minimum_required(VERSION 3.3)
project(apple2tc)

# Build a decompiled program twice: <name> is the GUI version, <name>-run is a
# console one for batch replay, frame hashing and verification.
#
# They cannot be one binary with a flag -- on Windows console versus GUI is a
# link-time subsystem property. The sources are compiled *once* into an OBJECT
# library and linked twice, so this costs link time but not compile time, which
# matters when a single generated file runs to 18k lines.
function(add_a2_program name)
  add_library(${name}-obj OBJECT ${ARGN})
  target_link_libraries(${name}-obj PRIVATE a2host)

  add_executable(${name} $<TARGET_OBJECTS:${name}-obj>)
  target_link_libraries(${name} PRIVATE a2host a2host_gui)

  add_executable(${name}-run
    $<TARGET_OBJECTS:${name}-obj> $<TARGET_OBJECTS:a2host_console>)
  target_link_libraries(${name}-run PRIVATE a2host)
endfunction()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

#add_compile_options(-fsanitize=address,undefined)
#add_link_options(-fsanitize=address,undefined)
#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)
#add_compile_options(-fsanitize=thread)
#add_link_options(-fsanitize=thread)


set(APPLE2TC_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(APPLE2TC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
# This is used for easy reference of dependency includes.
set(A2TC_INC ${APPLE2TC_INCLUDE_DIR}/apple2tc)

include_directories(${APPLE2TC_INCLUDE_DIR})

add_subdirectory(lib)
add_subdirectory(tools)
add_subdirectory(decoded)
add_subdirectory(tests/pacing)
