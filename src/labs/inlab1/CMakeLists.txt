add_subdirectory(../../utils utils)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC inlab1.h
        Compute.h
        Parser.h
)

set(LINK_LIBS utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})

add_executable(inlab1 main.cpp ${LIB_SRC})
target_include_directories(inlab1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(inlab1 ${LINK_LIBS})
