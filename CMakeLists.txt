cmake_minimum_required (VERSION 3.1)

project (assembler
  VERSION 0.1.0
  HOMEPAGE_URL https://github.com/GoMarck/assembler
  DESCRIPTION "C implementation of the assembler emulator"
  LANGUAGES C
)

include(cmake/util.cmake)

set (CMAKE_C_COMPILER "/usr/bin/gcc")

option (FAIL_ON_WARNING "Build fail if meets compile warning" ON)
option (WITH_TEST "Build unit test codes" ON)

# Add common flags
# -std=gnu99: use gnu99 standard
set (CMAKE_C_FLAGS "-std=gnu99")
# -Wall: allow all warnings
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
if (FAIL_ON_WARNING)
  # -Werror: treat warning as error
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
endif ()

include_directories (${CMAKE_SOURCE_DIR}/src)

set (SRCS
  src/cpu/mmu.c
  src/disk/code.c
  src/memory/dram.c
  src/memory/inst.c
  src/main.c)

add_executable (assembler ${SRCS})

if (WITH_TEST)
  include (CTest)
  enable_testing ()
  # add cpu test.
  ADD_ASSEMBLER_TEST (cpu-test SRC_FILES src/cpu/cpu-test.c)
endif ()