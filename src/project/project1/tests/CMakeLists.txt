set(TEST_SOURCES
        main.cpp
        LUMatrixTests.cpp
)

# Create the test executable
add_executable(project1_tests ${TEST_SOURCES})

# Link the test executable with Google Test and your project's library
target_link_libraries(project1_tests project1_lib gtest gtest_main)

add_test(NAME project1_tests COMMAND project1_tests)
