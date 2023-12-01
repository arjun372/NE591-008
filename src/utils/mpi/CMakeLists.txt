set(LIB_HEADERS
        MPIProject.h
)

set(LIB_SOURCES
        stub.cpp
)

if (NOT TARGET utils_mpi)
    # Create target
    add_library(utils_mpi STATIC ${LIB_HEADERS} ${LIB_SOURCES})
    target_include_directories(utils_mpi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif ()

# Link target against libraries
#target_link_libraries(profiler utils Boost::program_options)
target_link_libraries(utils_mpi Boost::program_options)
#target_link_libraries(profiler OpenMP::OpenMP_CXX)
