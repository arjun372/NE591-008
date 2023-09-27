set(TEST_SOURCES
        main.cpp
        LUMatrixTests.cpp
)

# Create the test executable
add_executable(outlab4_tests ${TEST_SOURCES})

# Link the test executable with Google Test and your project's library
target_link_libraries(outlab4_tests outlab4_lib gtest gtest_main)

add_test(NAME outlab4_tests COMMAND outlab4_tests)
