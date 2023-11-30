set(TEST_SOURCES
        main.cpp
        ../../blas/tests/TestedTypes.h
        PointJacobiTests.cpp
)

if (NOT TARGET relaxation_methods_tests)
    # Create the test executable
    add_executable(relaxation_methods_tests ${TEST_SOURCES})
    # Link the test executable with Google Test and your project's library
    target_link_libraries(relaxation_methods_tests utils gtest gtest_main)
    add_test(NAME relaxation_methods_tests COMMAND relaxation_methods_tests)
endif ()
