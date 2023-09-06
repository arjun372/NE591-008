add_library(inlab2_extern INTERFACE extern/function.h)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC inlab2.h
            Compute.h
            Parser.h
            ../../utils/CommandLine.h
            ../../utils/CheckBounds.h
            ../../utils/Helpers.h
            ../../utils/Project.h
            extern/function.h
)

add_library(lib_inlab2 ${LIB_SRC})
set_target_properties(lib_inlab2 PROPERTIES LINKER_LANGUAGE CXX)
set(LINK_LIBS inlab2_extern utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})
target_link_libraries(lib_inlab2 ${LINK_LIBS})

add_executable(inlab2 main.cpp ${LIB_SRC})
target_link_libraries(inlab2 lib_inlab2)

if(BUILD_PERFORMANCE_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

add_subdirectory(inputs)
