# CMakeLists.txt
# This file is part of the blurhash distribution (https://github.com/woltapp/blurhash).
# Copyright (c) 2025 Fumiama Minamoto.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

cmake_minimum_required(VERSION 2.8.12)
if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)
project(blurhash VERSION 0.1.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Ofast")

add_executable(blurhash_b blurhash.c)

add_library(blurhash   SHARED encode.c decode.c)
add_library(blurhash_s STATIC encode.c decode.c)

set_target_properties(blurhash_b PROPERTIES OUTPUT_NAME blurhash)
set_target_properties(blurhash_s PROPERTIES OUTPUT_NAME blurhash)
set_target_properties(blurhash   PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})

message(STATUS "Linking libraries...")
target_link_libraries(blurhash_s stb m)
target_link_libraries(blurhash   stb m)
target_link_libraries(blurhash_b blurhash_s)

INSTALL(TARGETS blurhash_b RUNTIME DESTINATION bin)
INSTALL(TARGETS blurhash   LIBRARY DESTINATION lib)
INSTALL(TARGETS blurhash_s ARCHIVE DESTINATION lib)
INSTALL(FILES blurhash.h DESTINATION include)
# INSTALL(FILES blurhash.1 DESTINATION share/man/man1)
