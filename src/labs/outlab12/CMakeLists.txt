add_subdirectory(../../utils utils)
add_subdirectory(examples)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC outlab12.h
        Compute.h
        Parser.h
        InputsOutputs.h
)

# Create the outlab12_lib library target
add_library(outlab12_lib STATIC ${LIB_SRC})
target_include_directories(outlab12_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${MPI_INCLUDE_PATH})
target_link_libraries(outlab12_lib utils stdc++fs ${MPI_LIBRARIES} Boost::program_options Boost::serialization)

# If using MPI compiler wrappers, you should set proper compiler flags
if(MPI_COMPILE_FLAGS)
    set_target_properties(outlab12_lib PROPERTIES COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()
if(MPI_LINK_FLAGS)
    set_target_properties(outlab12_lib PROPERTIES LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()

# Modify the outlab12 executable target to link with outlab12_lib
add_executable(outlab12 main.cpp)
target_link_libraries(outlab12 outlab12_lib)
