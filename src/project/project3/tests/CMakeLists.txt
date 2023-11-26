set(TEST_SOURCES
        main.cpp
)

# Create the test executable
add_executable(project3_tests ${TEST_SOURCES})

# Link the test executable with Google Test and your project's library
target_link_libraries(project3_tests project3_lib gtest gtest_main)

add_test(NAME project3_tests COMMAND project3_tests)
