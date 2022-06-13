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
#   INCLUDE_DIRS <dir1> <dir2> ...
#       Specify extra include directories this test need.
#
#   DEPENDS <target1> <target2> ...
#       Specify targets that this test depends.
#
#       If this test need to be built after targets and we need to
#       add dependiences as shown, this args would be helpful.

function(ADD_ASSEMBLER_TEST TEST_NAME)
  # Parse out properities for which we have special handling.
  set(options)
  set(one_value_args TIMEOUT)
  set(multi_value_args SRC_FILES DEP_LIBS INCLUDE_DIRS DEPENDS)
  cmake_parse_arguments(ARG "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

  # Build test case excutable first.
  add_executable(${TEST_NAME} ${SRC_FILES})
  target_link_libraries(${TEST_NAME} ${DEP_LIBS})
  target_include_directories(${TEST_NAME} ${INCLUDE_DIRS})
  add_dependencies(${TEST_NAME} ${DEPENDS})

  # Add it to CTest.
  add_test(NAME ${TEST_NAME}
    COMMAND ${TEST_OUTPUT_DIR}/${TEST_NAME}
    WORKING_DIRECTORY ${TEST_OUTPUT_DIR})
endfunction()