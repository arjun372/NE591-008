set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SRC inlab1.cpp ../../utils/CommandLine.h)
add_executable(inlab1 ${SRC})
target_link_libraries(inlab1 utils ${Boost_PROGRAM_OPTIONS_LIBRARY})
