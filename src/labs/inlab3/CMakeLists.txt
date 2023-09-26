add_subdirectory(../../utils utils)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC inlab3.h
        Compute.h
        Parser.h
        InputsOutputs.h
        extern/function.h
)

set(LINK_LIBS utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})

add_executable(inlab3 main.cpp ${LIB_SRC})
target_include_directories(inlab3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(inlab3 ${LINK_LIBS})
