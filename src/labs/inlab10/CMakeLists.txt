add_subdirectory(../../utils utils)
add_subdirectory(examples)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif ()

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

# Create the inlab8_lib library target
add_library(inlab8_lib STATIC ${LIB_SRC})
target_include_directories(inlab8_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${MPI_INCLUDE_PATH})
target_link_libraries(inlab8_lib utils stdc++fs ${MPI_LIBRARIES} Boost::program_options Boost::serialization)

# If using MPI compiler wrappers, you should set proper compiler flags
if(MPI_COMPILE_FLAGS)
    set_target_properties(inlab8_lib PROPERTIES COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()
if(MPI_LINK_FLAGS)
    set_target_properties(inlab8_lib PROPERTIES LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()

# Modify the inlab8 executable target to link with inlab8_lib
add_executable(inlab8 main.cpp)
target_link_libraries(inlab8 inlab8_lib)
