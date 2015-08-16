# Setup individual tests here
#set(TEST_NAME <NAME_OF_TEST>)
#add_executable(${TEST_NAME} "${TEST_NAME}.cpp")
#target_link_libraries(${TEST_NAME} )
#set( ALL_TESTS ${ALL_TESTS} ${TEST_NAME} )

set(TEST_NAME testCmdLineOptions)
add_executable(${TEST_NAME} "${TEST_NAME}.cpp")
#target_link_libraries(${TEST_NAME} )
set( ALL_TESTS ${ALL_TESTS} ${TEST_NAME} )

# Link every test to ${GTEST_LIBS}, and add it to the test runner
foreach(this_test ${ALL_TESTS})
target_link_libraries(${this_test} ${GTEST_LIBS})
add_test( "check_${this_test}" "${this_test}")
endforeach(this_test)
 
