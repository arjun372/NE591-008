add_subdirectory(../../utils utils)
add_subdirectory(inputs)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC inlab2.h
        Compute.h
        Parser.h
        extern/function.h
)

set(LINK_LIBS utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})

add_executable(inlab2 main.cpp ${LIB_SRC})
target_include_directories(inlab2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(inlab2 ${LINK_LIBS})
