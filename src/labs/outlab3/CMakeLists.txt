add_library(outlab3_extern INTERFACE extern/function.h)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC outlab3.h
            Compute.h
            Parser.h
            InputsOutputs.h
            ../../utils/CommandLine.h
            ../../utils/CheckBounds.h
            ../../utils/Helpers.h
            ../../utils/Project.h
            extern/function.h
)

add_library(lib_outlab3 ${LIB_SRC})
set_target_properties(lib_outlab3 PROPERTIES LINKER_LANGUAGE CXX)
set(LINK_LIBS outlab3_extern utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})
target_link_libraries(lib_outlab3 ${LINK_LIBS})

add_executable(outlab3 main.cpp ${LIB_SRC})
target_link_libraries(outlab3 lib_outlab3)
