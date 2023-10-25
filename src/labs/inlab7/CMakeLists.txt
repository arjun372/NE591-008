add_subdirectory(../../utils utils)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(COPY "input" DESTINATION ${CMAKE_BINARY_DIR}/)

set(LIB_SRC hello_world.cpp)

# Find MPI
set(MPI_C_COMPILER /usr/lib64/openmpi/bin/mpicc)
set(MPI_CXX_COMPILER /usr/lib64/openmpi/bin/mpicxx)
find_package(MPI REQUIRED)

# Include MPI includes
#include_directories(${MPI_INCLUDE_PATH})

# Create the executable target
add_executable(inlab7 ${LIB_SRC})
target_include_directories(inlab7 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${MPI_INCLUDE_PATH})
target_link_libraries(inlab7 utils stdc++fs ${MPI_LIBRARIES} Boost::program_options Boost::serialization)

# If using MPI compiler wrappers, you should set proper compiler flags
if(MPI_COMPILE_FLAGS)
    set_target_properties(inlab7 PROPERTIES COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()
if(MPI_LINK_FLAGS)
    set_target_properties(inlab7 PROPERTIES LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()
