cmake_minimum_required (VERSION 3.1)

project (assembler
  VERSION 0.0.0
  HOMEPAGE_URL https://github.com/GoMarck/assembler
  DESCRIPTION "C implementation of the assembler emulator"
  LANGUAGES C
)

set (CMAKE_C_COMPILER "/usr/bin/gcc")

# Add common flags
# -std=gnu99: use gnu99 standard
set (CMAKE_C_FLAGS "-std=gnu99")
# -Wall: allow all warnings
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
# -Wno-unused-function: ignore unused function temporarily
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-function")
# -Werror: treat warning as error
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")

file (GLOB_RECURSE SRCS RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c")

add_executable (assembler ${SRCS})

target_include_directories (assembler PUBLIC ${CMAKE_SOURCE_DIR}/src)
