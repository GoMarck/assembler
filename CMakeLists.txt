cmake_minimum_required (VERSION 3.1)

project (assembler
  VERSION 0.1.0
  HOMEPAGE_URL https://github.com/GoMarck/assembler
  DESCRIPTION "C implementation of the assembler emulator"
  LANGUAGES C
)

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

file (GLOB_RECURSE SRCS RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c")

add_executable (assembler ${SRCS})

target_include_directories (assembler PUBLIC ${CMAKE_SOURCE_DIR}/src)

if (WITH_TEST)
  include(CTest)
  enable_testing()
  # we need to have an function to generate the ctest testcases.
  add_executable(assembler_test test/example/example_test.c test/src/test_internal.c)
  target_include_directories(assembler_test PRIVATE ${CMAKE_SOURCE_DIR}/test/include)
endif ()