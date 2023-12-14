add_subdirectory(../../utils utils)
add_subdirectory(examples)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC
        outlab13.h
        Compute.h
        Parser.h
        InputsOutputs.h
)

set(LINK_LIBS utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})

# Create the outlab13_lib library target
add_library(outlab13_lib STATIC ${LIB_SRC})
target_include_directories(outlab13_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(outlab13_lib ${LINK_LIBS})

# Modify the outlab13 executable target to link with outlab13_lib
add_executable(outlab13 main.cpp)
target_link_libraries(outlab13 outlab13_lib)


file(GLOB FILES "analysis" "examples")
file(COPY ${FILES} DESTINATION ${CMAKE_BINARY_DIR}/)
