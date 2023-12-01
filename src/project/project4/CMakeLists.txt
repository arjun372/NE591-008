add_subdirectory(../../utils utils)
add_subdirectory(examples)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif ()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC
        project4.h
        Compute.h
        Parser.h
        InputsOutputs.h
)

set(LINK_LIBS utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})

# Create the project4_lib library target
add_library(project4_lib STATIC ${LIB_SRC})
target_include_directories(project4_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(project4_lib ${LINK_LIBS})

# Modify the project4 executable target to link with project4_lib
add_executable(project4 main.cpp)
target_link_libraries(project4 project4_lib)


file(GLOB FILES "analysis" "examples")
file(COPY ${FILES} DESTINATION ${CMAKE_BINARY_DIR}/)
