set(TEST_SOURCES
        main.cpp
        LUMatrixTests.cpp
)

# Create the test executable
add_executable(outlab5_tests ${TEST_SOURCES})

# Link the test executable with Google Test and your project's library
target_link_libraries(outlab5_tests outlab5_lib gtest gtest_main)

add_test(NAME outlab5_tests COMMAND outlab5_tests)
