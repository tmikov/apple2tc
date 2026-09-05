# Copyright (c) Tzvetan Mikov.
#
# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree.

# The MCP front end. Like a2run it has no sources of its own; everything lives
# in lib/a2mcp.
add_executable(a2mcp
  $<TARGET_OBJECTS:a2mcp_console> $<TARGET_OBJECTS:engine6502>)
target_link_libraries(a2mcp a2mcp_lib engine6502)
