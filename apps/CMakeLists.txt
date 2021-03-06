# Copyright (c) 2013-2014 Flowgrammable.org
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at:
# 
# http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an "AS IS"
# BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
# or implied. See the License for the specific language governing
# permissions and limitations under the License.

# Make sure that the top-level headers are on path.
include_directories(${CMAKE_SOURCE_DIR})

# Build application libraries directly in this location, making
# them easier to find by the linker.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Add an application with the given target name. The source files are
# given in the following arguments. The target is linked against the
# freeflow and freeflow-sdn libraries.
#
# TODO: Migrate this to the top-level cmake directory.
macro(add_application name)
  add_library(${name} SHARED ${ARGN})
  set_target_properties(${name} PROPERTIES 
    PREFIX ""       # Don't prefix with "lib"
    SUFFIX ".app")  # Use .app as the extension
  target_link_libraries(${name} freeflow freeflow-sdn)
endmacro()

# ---------------------------------------------------------------------------- #
# Applications

add_subdirectory(template)
add_subdirectory(noflow)


