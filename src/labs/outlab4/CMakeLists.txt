add_subdirectory(../../utils utils)
add_subdirectory(examples)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC outlab4.h
        Compute.h
        Parser.h
        InputsOutputs.h
)

set(LINK_LIBS utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})

add_executable(outlab4 main.cpp ${LIB_SRC})
target_include_directories(outlab4 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(outlab4 ${LINK_LIBS})
