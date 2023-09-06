set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC inlab1.h
        CommandLineParser.h
            Compute.h
            ../../utils/CommandLine.h
            ../../utils/CheckBounds.h
            ../../utils/Helpers.h
            ../../utils/Profiler.h
            ../../utils/Project.h
)
add_library(lib_inlab1 ${LIB_SRC})
set_target_properties(lib_inlab1 PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(lib_inlab1 utils ${Boost_PROGRAM_OPTIONS_LIBRARY})

add_executable(inlab1 main.cpp ${LIB_SRC})
target_link_libraries(inlab1 lib_inlab1)

if(BUILD_PERFORMANCE_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
