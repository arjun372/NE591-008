add_subdirectory(examples)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SRC inlab4.h
            Compute.h
            Parser.h
            InputsOutputs.h
            ../../utils/CommandLine.h
            ../../utils/CheckBounds.h
            ../../utils/Helpers.h
            ../../utils/Project.h
)

add_library(lib_inlab4 ${LIB_SRC})
set_target_properties(lib_inlab4 PROPERTIES LINKER_LANGUAGE CXX)
set(LINK_LIBS utils stdc++fs ${Boost_PROGRAM_OPTIONS_LIBRARY})
target_link_libraries(lib_inlab4 ${LINK_LIBS})

add_executable(inlab4 main.cpp ${LIB_SRC})
target_link_libraries(inlab4 lib_inlab4)
