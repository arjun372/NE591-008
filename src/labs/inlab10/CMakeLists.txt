add_subdirectory(../../utils utils)
add_subdirectory(examples)

add_subdirectory(tests)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC inlab10.h
        Compute.h
        Parser.h
        InputsOutputs.h
)

# Find MPI
set(MPI_C_COMPILER /usr/lib64/openmpi/bin/mpicc)
set(MPI_CXX_COMPILER /usr/lib64/openmpi/bin/mpicxx)
find_package(MPI REQUIRED)

# Create the inlab10_lib library target
add_library(inlab10_lib STATIC ${LIB_SRC})
target_include_directories(inlab10_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${MPI_INCLUDE_PATH})
target_link_libraries(inlab10_lib utils stdc++fs ${MPI_LIBRARIES} Boost::program_options Boost::serialization)

# If using MPI compiler wrappers, you should set proper compiler flags
if(MPI_COMPILE_FLAGS)
    set_target_properties(inlab10_lib PROPERTIES COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()
if(MPI_LINK_FLAGS)
    set_target_properties(inlab10_lib PROPERTIES LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()

# Modify the inlab10 executable target to link with inlab10_lib
add_executable(inlab10 main.cpp)
target_link_libraries(inlab10 inlab10_lib)
