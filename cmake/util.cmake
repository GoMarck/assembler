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