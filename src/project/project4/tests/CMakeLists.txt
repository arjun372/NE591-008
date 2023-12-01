set(TEST_SOURCES
        main.cpp
)

# Create the test executable
add_executable(project4_tests ${TEST_SOURCES})

# Link the test executable with Google Test and your project's library
target_link_libraries(project4_tests project4_lib gtest gtest_main)

add_test(NAME project4_tests COMMAND project4_tests)
