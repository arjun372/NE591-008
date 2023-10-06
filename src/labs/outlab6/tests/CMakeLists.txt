set(TEST_SOURCES
        main.cpp
        LUMatrixTests.cpp
)

# Create the test executable
add_executable(outlab6_tests ${TEST_SOURCES})

# Link the test executable with Google Test and your project's library
target_link_libraries(outlab6_tests outlab6_lib gtest gtest_main)

add_test(NAME outlab6_tests COMMAND outlab6_tests)
