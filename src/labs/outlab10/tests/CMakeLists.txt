set(TEST_SOURCES
        main.cpp
        OutLab10Tests.cpp
)

# Create the test executable
add_executable(outlab10_tests ${TEST_SOURCES})

# Link the test executable with Google Test and your project's library
target_link_libraries(outlab10_tests outlab10_lib gtest gtest_main)

add_test(NAME outlab10_tests COMMAND outlab10_tests)
