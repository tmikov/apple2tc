# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

# The MCP front end: a2host driven by JSON-RPC instead of by a frame limit.
# No sokol -- like a2run, it links no graphics stack at all.
include_directories(${APPLE2TC_ROOT_DIR}/external/json/include)
include_directories(${APPLE2TC_ROOT_DIR}/external/stb)

add_library(a2mcp_lib
  mcp_machine.cpp mcp_machine.h
  mcp_paths.cpp mcp_paths.h
  mcp_screen.cpp mcp_screen.h
  mcp_server.cpp mcp_server.h
  mcp_tools.cpp mcp_tools.h
  )
target_link_libraries(a2mcp_lib a2host a2io support)

# An OBJECT library for the same reason as a2host_console: nothing in the
# executable's own objects references main(), so a static archive would never
# be pulled in.
add_library(a2mcp_console OBJECT mcp_main.cpp)
target_link_libraries(a2mcp_console PUBLIC a2mcp_lib)
