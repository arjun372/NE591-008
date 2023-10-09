set(CMAKE_INCLUDE_CURRENT_DIR ON)

#export PATH=$PATH:/usr/lib64/openmpi/bin
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib64/openmpi/lib/

file(COPY "input" DESTINATION ${CMAKE_BINARY_DIR}/)

set(MPI_C_COMPILER /usr/lib64/openmpi/bin/mpicc)
set(MPI_CXX_COMPILER /usr/lib64/openmpi/bin/mpicxx)

# Find MPI
find_package(MPI REQUIRED)

# Include MPI includes
include_directories(${MPI_INCLUDE_PATH})

# Add executable
add_executable(inlab7 hello_world.cpp)

# Link against MPI libraries
target_link_libraries(inlab7 ${MPI_LIBRARIES})

# If using MPI compiler wrappers, you should set proper compiler flags
if(MPI_COMPILE_FLAGS)
    set_target_properties(inlab7 PROPERTIES
            COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(inlab7 PROPERTIES
            LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()

#set(LINK_LIBS utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})
#target_link_libraries(inlab7 outlab6_lib)
