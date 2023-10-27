set(TEST_SOURCES
        main.cpp
)

# Create the test executable
add_executable(inlab8_tests ${TEST_SOURCES})

# Link the test executable with Google Test and your project's library
target_link_libraries(inlab8_tests inlab8_lib gtest gtest_main)

add_test(NAME inlab8_tests COMMAND inlab8_tests)
