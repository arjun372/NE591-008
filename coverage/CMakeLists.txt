# Code coverage - Note: GCC only
if(CMAKE_COMPILER_IS_GNUCXX)
    include(ProcessorCount)
    ProcessorCount(N)
    if(NOT N EQUAL 0)
        set(CTEST_BUILD_FLAGS -j${N})
    endif()

    include(CTest)

    find_program( GCOV_PATH gcov )

    add_custom_target(coverage
            COMMAND ${GCOV_PATH} -r -b ${CMAKE_CURRENT_BINARY_DIR}/*.gcda > Coverage.txt
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()
