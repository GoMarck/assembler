cmake_minimum_required (VERSION 3.1)

project (assembler
  VERSION 0.0.0
  DESCRIPTION "C implementation of the assembler emulator"
  LANGUAGES C
)

file (GLOB_RECURSE SRCS RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c")

add_executable (assembler ${SRCS})

target_include_directories (assembler ${CMAKE_SOURCE_DIR}/src)