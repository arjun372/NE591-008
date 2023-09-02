add_library(inlab2_extern INTERFACE extern/function.h)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SRC inlab2.cpp ProcessInputs.h Compute.h ../../utils/CommandLine.h extern/function.h)

add_executable(inlab2 ${SRC})
target_link_libraries(inlab2 inlab2_extern utils ${Boost_PROGRAM_OPTIONS_LIBRARY})

if(BUILD_PERFORMANCE_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

add_subdirectory(inputs)
