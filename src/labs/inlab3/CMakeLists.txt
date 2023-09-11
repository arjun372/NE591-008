add_library(inlab3_extern INTERFACE extern/function.h)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC inlab3.h
            Compute.h
            Parser.h
            InputsOutputs.h
            ../../utils/CommandLine.h
            ../../utils/CheckBounds.h
            ../../utils/Helpers.h
            ../../utils/Project.h
            extern/function.h
)

add_library(lib_inlab3 ${LIB_SRC})
set_target_properties(lib_inlab3 PROPERTIES LINKER_LANGUAGE CXX)
set(LINK_LIBS inlab3_extern utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})
target_link_libraries(lib_inlab3 ${LINK_LIBS})

add_executable(inlab3 main.cpp ${LIB_SRC})
target_link_libraries(inlab3 lib_inlab3)
