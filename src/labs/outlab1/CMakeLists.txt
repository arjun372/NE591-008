set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC outlab1.h
            Compute.h
            Parser.h
            ../../utils/CommandLine.h
            ../../utils/CheckBounds.h
            ../../utils/Helpers.h
            ../../utils/Project.h
)
add_library(lib_outlab1 ${LIB_SRC})
set_target_properties(lib_outlab1 PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(lib_outlab1 utils ${Boost_PROGRAM_OPTIONS_LIBRARY})

add_executable(outlab1 main.cpp ${LIB_SRC})
target_link_libraries(outlab1 lib_outlab1)

if(BUILD_PERFORMANCE_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
