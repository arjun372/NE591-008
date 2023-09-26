add_subdirectory(../../utils utils)
add_subdirectory(examples)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC outlab2.h
        Compute.h
        Parser.h
)

set(LINK_LIBS utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})

add_executable(outlab2 main.cpp ${LIB_SRC})
target_include_directories(outlab2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(outlab2 ${LINK_LIBS})
