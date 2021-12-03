cmake_minimum_required(VERSION 3.3)
project(apple2c)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(APPLE2C_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(APPLE2C_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${APPLE2C_INCLUDE_DIR})

add_subdirectory(lib)
add_subdirectory(tools)
