# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

cmake_minimum_required(VERSION 3.3)
project(apple2tc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(APPLE2TC_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(APPLE2TC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
# This is used for easy reference of dependency includes.
set(A2TC_INC ${APPLE2TC_INCLUDE_DIR}/apple2tc)

include_directories(${APPLE2TC_INCLUDE_DIR})

add_subdirectory(lib)
add_subdirectory(tools)
