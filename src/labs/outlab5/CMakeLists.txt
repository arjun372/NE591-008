add_subdirectory(../../utils utils)
add_subdirectory(examples)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC outlab5.h
        Compute.h
        Parser.h
        InputsOutputs.h
)

set(LINK_LIBS utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})

# Create the outlab5_lib library target
add_library(outlab5_lib STATIC ${LIB_SRC})
target_include_directories(outlab5_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(outlab5_lib ${LINK_LIBS})

# Modify the outlab5 executable target to link with outlab5_lib
add_executable(outlab5 main.cpp)
target_link_libraries(outlab5 outlab5_lib)
