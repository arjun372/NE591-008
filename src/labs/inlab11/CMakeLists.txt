add_subdirectory(../../utils utils)
add_subdirectory(examples)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC inlab11.h
        Compute.h
        Parser.h
        InputsOutputs.h
)

# Create the inlab11_lib library target
add_library(inlab11_lib STATIC ${LIB_SRC})
target_include_directories(inlab11_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${MPI_INCLUDE_PATH})
target_link_libraries(inlab11_lib utils stdc++fs ${MPI_LIBRARIES} Boost::program_options Boost::serialization)

# If using MPI compiler wrappers, you should set proper compiler flags
if(MPI_COMPILE_FLAGS)
    set_target_properties(inlab11_lib PROPERTIES COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()
if(MPI_LINK_FLAGS)
    set_target_properties(inlab11_lib PROPERTIES LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()

# Modify the inlab11 executable target to link with inlab11_lib
add_executable(inlab11 main.cpp)
target_link_libraries(inlab11 inlab11_lib)
