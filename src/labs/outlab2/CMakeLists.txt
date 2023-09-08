set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(examples)

add_library(outlab2_extern INTERFACE extern/function.h)

set(LIB_SRC
        outlab2.h
        Compute.h
        Parser.h
        ../../utils/CommandLine.h
        ../../utils/Project.h
)
add_library(lib_outlab2 ${LIB_SRC})
set_target_properties(lib_outlab2 PROPERTIES LINKER_LANGUAGE CXX)
set(LINK_LIBS outlab2_extern utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})
target_link_libraries(lib_outlab2 ${LINK_LIBS})

add_executable(outlab2 main.cpp ${LIB_SRC})
target_link_libraries(outlab2 lib_outlab2)
