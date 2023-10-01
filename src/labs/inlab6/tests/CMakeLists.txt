set(TEST_SOURCES
        main.cpp
        LUMatrixTests.cpp
)

# Create the test executable
add_executable(inlab6_tests ${TEST_SOURCES})

# Link the test executable with Google Test and your project's library
target_link_libraries(inlab6_tests inlab6_lib gtest gtest_main)

add_test(NAME inlab6_tests COMMAND inlab6_tests)
