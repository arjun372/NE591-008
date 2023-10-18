set(TEST_SOURCES
        main.cpp
        DiffusionParamsTests.cpp
        DiffusionConstantsTests.cpp
        DiffusionMatrixTests.cpp
)

if (NOT TARGET physics_diffusion_tests)
    # Create the test executable
    add_executable(physics_diffusion_tests ${TEST_SOURCES})
    # Link the test executable with Google Test and your project's library
    target_link_libraries(physics_diffusion_tests utils gtest gtest_main)
    add_test(NAME physics_diffusion_tests COMMAND physics_diffusion_tests)
endif ()
