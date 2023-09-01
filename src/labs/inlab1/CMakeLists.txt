set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SRC inlab1.cpp ProcessInputs.h Compute.h ../../utils/CommandLine.h)
add_executable(inlab1 ${SRC})
target_link_libraries(inlab1 utils ${Boost_PROGRAM_OPTIONS_LIBRARY})

if(BUILD_PERFORMANCE_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
