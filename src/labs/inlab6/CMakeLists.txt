add_subdirectory(../../utils utils)
add_subdirectory(examples)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC inlab6.h
        Compute.h
        Parser.h
        InputsOutputs.h
)

set(LINK_LIBS utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})

# Create the inlab6_lib library target
add_library(inlab6_lib STATIC ${LIB_SRC})
target_include_directories(inlab6_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(inlab6_lib ${LINK_LIBS})

# Modify the inlab6 executable target to link with inlab6_lib
add_executable(inlab6 main.cpp)
target_link_libraries(inlab6 inlab6_lib)
