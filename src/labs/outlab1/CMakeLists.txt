set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SRC outlab1.cpp Compute.h ../../utils/CommandLine.h)
add_executable(outlab1 ${SRC})
target_link_libraries(outlab1 utils ${Boost_PROGRAM_OPTIONS_LIBRARY})

if(BUILD_PERFORMANCE_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
