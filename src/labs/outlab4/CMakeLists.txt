add_subdirectory(../../utils utils)
add_subdirectory(examples)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif ()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC outlab4.h
        Compute.h
        Parser.h
        InputsOutputs.h
)

set(LINK_LIBS utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})

# Create the outlab4_lib library target
add_library(outlab4_lib STATIC ${LIB_SRC})
target_include_directories(outlab4_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(outlab4_lib ${LINK_LIBS})

# Modify the outlab4 executable target to link with outlab4_lib
add_executable(outlab4 main.cpp)
target_link_libraries(outlab4 outlab4_lib)
