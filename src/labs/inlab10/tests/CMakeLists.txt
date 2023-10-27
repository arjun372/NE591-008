set(TEST_SOURCES
        main.cpp
        InLab10Tests.cpp
)

# Create the test executable
add_executable(inlab10_tests ${TEST_SOURCES})

# Link the test executable with Google Test and your project's library
target_link_libraries(inlab10_tests inlab10_lib gtest gtest_main)

add_test(NAME inlab10_tests COMMAND inlab10_tests)
