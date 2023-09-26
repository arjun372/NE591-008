add_subdirectory(../../utils utils)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC outlab1.h
        Compute.h
        Parser.h
)

set(LINK_LIBS utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})

add_executable(outlab1 main.cpp ${LIB_SRC})
target_include_directories(outlab1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(outlab1 ${LINK_LIBS})
