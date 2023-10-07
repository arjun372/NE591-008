add_subdirectory(../../utils utils)
add_subdirectory(examples)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif ()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC
        project1.h
        Compute.h
        Parser.h
        InputsOutputs.h
)

set(LINK_LIBS utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})

# Create the project1_lib library target
add_library(project1_lib STATIC ${LIB_SRC})
target_include_directories(project1_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(project1_lib ${LINK_LIBS})

# Modify the project1 executable target to link with project1_lib
add_executable(project1 main.cpp)
target_link_libraries(project1 project1_lib)


file(GLOB FILES "analysis" "examples")
file(COPY ${FILES} DESTINATION ${CMAKE_BINARY_DIR}/)
