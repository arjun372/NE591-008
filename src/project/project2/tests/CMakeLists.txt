set(TEST_SOURCES
        main.cpp
)

# Create the test executable
add_executable(project2_tests ${TEST_SOURCES})

# Link the test executable with Google Test and your project's library
target_link_libraries(project2_tests project2_lib gtest gtest_main)

add_test(NAME project2_tests COMMAND project2_tests)
