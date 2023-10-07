add_subdirectory(../../utils utils)
add_subdirectory(examples)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif ()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC outlab6.h
        Compute.h
        Parser.h
        InputsOutputs.h
)

set(LINK_LIBS utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})

# Create the outlab6_lib library target
add_library(outlab6_lib STATIC ${LIB_SRC})
target_include_directories(outlab6_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(outlab6_lib ${LINK_LIBS})

# Modify the outlab6 executable target to link with outlab6_lib
add_executable(outlab6 main.cpp)
target_link_libraries(outlab6 outlab6_lib)
