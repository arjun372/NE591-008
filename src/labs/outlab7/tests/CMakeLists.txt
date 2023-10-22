set(TEST_SOURCES
        main.cpp
)

# Create the test executable
add_executable(outlab7_tests ${TEST_SOURCES})

# Link the test executable with Google Test and your project's library
target_link_libraries(outlab7_tests outlab7_lib gtest gtest_main)

add_test(NAME outlab7_tests COMMAND outlab7_tests)
