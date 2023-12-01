add_subdirectory(math)
add_subdirectory(mpi)
add_subdirectory(profiler)
add_subdirectory(physics)

set(UTILS_LIB_HEADERS
        CheckBounds.h
        FileParser.h
        Helpers.h
        json.hpp
        Stopwatch.h
        CommandLine.h
        Project.h
)

set(UTILS_LIB_SOURCES
        utils_stub.cpp
)

if (NOT TARGET utils)
    # Create utils
    add_library(utils STATIC ${UTILS_LIB_SOURCES} ${UTILS_LIB_HEADERS})
    target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif ()

# Link utils against Boost libraries
target_link_libraries(utils utils_math utils_mpi utils_profiler utils_physics Boost::program_options Boost::serialization)
target_link_libraries(utils OpenMP::OpenMP_CXX)
