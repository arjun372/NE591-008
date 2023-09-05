set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SRC outlab2.cpp ../../utils/CommandLine.h ../../utils/Project.h)

add_executable(outlab2 ${SRC})
set_target_properties(outlab2 PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(outlab2 utils ${Boost_PROGRAM_OPTIONS_LIBRARY})
