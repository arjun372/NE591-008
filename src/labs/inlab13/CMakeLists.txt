add_subdirectory(../../utils utils)
add_subdirectory(examples)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC
        inlab13.h
        Compute.h
        Parser.h
        InputsOutputs.h
)

set(LINK_LIBS utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})

# Create the inlab13_lib library target
add_library(inlab13_lib STATIC ${LIB_SRC})
target_include_directories(inlab13_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(inlab13_lib ${LINK_LIBS})

# Modify the inlab13 executable target to link with inlab13_lib
add_executable(inlab13 main.cpp)
target_link_libraries(inlab13 inlab13_lib)


file(GLOB FILES "analysis" "examples")
file(COPY ${FILES} DESTINATION ${CMAKE_BINARY_DIR}/)
