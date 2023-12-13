add_subdirectory(../../utils utils)
add_subdirectory(examples)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif ()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC
        project5.h
        Compute.h
        Parser.h
        InputsOutputs.h
)

set(LINK_LIBS utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})

# Create the project5_lib library target
add_library(project5_lib STATIC ${LIB_SRC})
target_include_directories(project5_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(project5_lib ${LINK_LIBS})

# Modify the project5 executable target to link with project5_lib
add_executable(project5 main.cpp)
target_link_libraries(project5 project5_lib)


file(GLOB FILES "analysis" "examples")
file(COPY ${FILES} DESTINATION ${CMAKE_BINARY_DIR}/)
