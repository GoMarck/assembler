cmake_minimum_required (VERSION 3.1)

project (assembler
  VERSION 0.1.0
  HOMEPAGE_URL https://github.com/GoMarck/assembler
  DESCRIPTION "C implementation of the assembler emulator"
  LANGUAGES C
)

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

include_directories (${CMAKE_SOURCE_DIR}/include)

set (SRCS
  src/cpu/mmu.c
  src/disk/code.c
  src/memory/dram.c
  src/memory/inst.c
  src/util/log.c
  src/util/convert.c
  )

add_library(assembler_lib STATIC ${SRCS})

add_executable (assembler src/main.c)
target_link_libraries(assembler assembler_lib)

###################################################################
#                            TEST ZONE                            #                        
###################################################################

# Add a new test case with an executable that should be built.
# 
# TEST_NAME is the name of the test. It would be add to CTest
# as a tag than we can run the test via CTest regex. It must 
# be an global unique name.
#
# Additional optional arguments:
#
#   TIMEOUT <secs>
#       Sets a timeout for running this test.
#
#   SRC_FILES <file1> <file2> ...
#       Specify source files that would be compiled as executable.
#
#   DEP_LIBS <lib1> <lib2> ...
#       Specify libraries that executable depends.
#
#   INC_DIRS <dir1> <dir2> ...
#       Specify extra include directories this test need.
#
#   DEPENDS <target1> <target2> ...
#       Specify targets that this test depends.
#
#       If this test need to be built after targets and we need to
#       add dependiences as shown, this args would be helpful.
function (ADD_ASSEMBLER_TEST TEST_NAME)
  # Parse out properities for which we have special handling.
  set (options)
  set (one_value_args TIMEOUT)
  set (multi_value_args SRC_FILES DEP_LIBS INC_DIRS DEPENDS)
  cmake_parse_arguments (ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

  set (${TEST_NAME}_SRCS ${CMAKE_SOURCE_DIR}/test/src/test_internal.c ${ARG_SRC_FILES})
  set (${TEST_NAME}_INC_DIRS ${CMAKE_SOURCE_DIR}/test/include ${ARG_INC_DIRS})

  # Build test case excutable.
  add_executable (${TEST_NAME} ${${TEST_NAME}_SRCS})
  target_include_directories (${TEST_NAME} PUBLIC ${${TEST_NAME}_INC_DIRS})

  # Link extra libs if need.
  if (ARG_DEP_LIBS)
    target_link_libraries (${TEST_NAME} ${ARG_DEP_LIBS})
  endif ()
  
  # Add dependiences if need.
  if (ARG_DEPENDS)
    add_dependencies (${TEST_NAME} ${ARG_DEPENDS})
  endif()

  # Add it to CTest.
  add_test (NAME ${TEST_NAME}
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endfunction ()

if (WITH_TEST)
  include (CTest)
  enable_testing ()
  # add example test.
  ADD_ASSEMBLER_TEST (example-test SRC_FILES test/example/example-test.c)
  # add cpu test.
  ADD_ASSEMBLER_TEST (cpu-test SRC_FILES src/cpu/cpu-test.c)
  # add instruction test.
  ADD_ASSEMBLER_TEST (inst-test SRC_FILES src/memory/inst-test.c DEP_LIBS assembler_lib)
  # add convert test.
  ADD_ASSEMBLER_TEST (convert-test SRC_FILES src/util/convert-test.c DEP_LIBS assembler_lib)
  # add assembler test
  ADD_ASSEMBLER_TEST (add-test SRC_FILES test/st/add.c DEP_LIBS assembler_lib)
endif ()