cmake_minimum_required(VERSION 2.8.12)
if (POLICY CMP0048)
    cmake_policy(SET CMP0048 NEW)
endif (POLICY CMP0048)
project(blurhash VERSION 0.1.0)

add_executable(blurhash_b blurhash.c)

add_library(blurhash   SHARED encode.c decode.c)
add_library(blurhash_s STATIC encode.c decode.c)

set_target_properties(blurhash_b PROPERTIES OUTPUT_NAME blurhash)
set_target_properties(blurhash_s PROPERTIES OUTPUT_NAME blurhash)
set_target_properties(blurhash   PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})

message(STATUS "Linking libraries...")
target_link_libraries(blurhash_b blurhash_s)

INSTALL(TARGETS blurhash_b RUNTIME DESTINATION bin)
INSTALL(TARGETS blurhash   LIBRARY DESTINATION lib)
INSTALL(TARGETS blurhash_s ARCHIVE DESTINATION lib)
INSTALL(FILES blurhash.h DESTINATION include)
# INSTALL(FILES blurhash.1 DESTINATION share/man/man1)
