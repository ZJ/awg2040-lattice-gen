# Setup individual tests here
#set(TEST_NAME <NAME_OF_TEST>)
#add_executable(${TEST_NAME}.test EXCLUDE_FROM_ALL "${TEST_NAME}.cpp")
#target_link_libraries(${TEST_NAME}.test <Libraries to link>)
#set( ALL_TESTS ${ALL_TESTS} ${TEST_NAME}.test )

add_custom_target(all-tests)

set(TEST_NAME testCmdLineOptions)
add_executable(${TEST_NAME}.test EXCLUDE_FROM_ALL "${TEST_NAME}.cpp")
target_link_libraries(${TEST_NAME}.test options)
set( ALL_TESTS ${ALL_TESTS} ${TEST_NAME}.test )

# Link every test to ${GTEST_LIBS}, and add it to the test runner
foreach(this_test ${ALL_TESTS})
target_link_libraries(${this_test} ${GTEST_LIBS})
add_test( "${this_test}" "${this_test}")
add_dependencies(all-tests ${this_test})
endforeach(this_test)

## Coverage generation
# Zero any prior gathered coverage
add_custom_target(coverage-zero
COMMAND lcov --directory ${PROJECT_SOURCE_DIR} --zerocounter
COMMENT "Test Coverage: Zeroing counters"
VERBATIM)

# Generate Initial Coverage Info 
add_custom_target(coverage-initial
COMMAND lcov -q --directory ${PROJECT_SOURCE_DIR} --initial --capture --output-file coverage.info.base
COMMENT "Test Coverage: Generating intitial info"
VERBATIM)

# Process Coverage from Tests
add_custom_target(coverage-results
COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --blue "Capturing Test Coverage"
COMMAND lcov -q --directory ${PROJECT_SOURCE_DIR} --capture --output-file coverage.info.test
COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --blue "Merging with Initial Coverage"
COMMAND lcov -q --add-tracefile coverage.info.base --add-tracefile coverage.info.test --output-file coverage.info
COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --blue "Removing Irrelevant Information"
COMMAND lcov -q --remove coverage.info 'tests/*' '/usr/*' 'optionparser/*' --output-file coverage.info
COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --green "Listing Coverage"
COMMAND lcov --list coverage.info
COMMENT "Test Coverage: Generating results"
)
