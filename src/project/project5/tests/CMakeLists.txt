set(TEST_SOURCES
        main.cpp
)

# Create the test executable
add_executable(project5_tests ${TEST_SOURCES})

# Link the test executable with Google Test and your project's library
target_link_libraries(project5_tests project5_lib gtest gtest_main)

add_test(NAME project5_tests COMMAND project5_tests)
