# vim: set foldmarker={{{,}}} foldlevel=0 foldmethod=marker:
cmake_minimum_required(VERSION 3.14)

project(ne591_008)

# In-source build prevention.
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

####################### Options ################### {{{
option(BUILD_TESTS "Build the tests" ON)  # Influences CTest.
option(OPTIMIZE_FOR_NATIVE "Build with -march=native" ON)
option(BUILD_PERFORMANCE_BENCHMARKS "Build performance benchmarks" ON)
####################### Options ##################### }}}

# Add the resources/cmake folder to Module Search Path
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include_directories("${PROJECT_BINARY_DIR}/include") # add the binary dir include directory for project-config.h
include_directories("${PROJECT_SOURCE_DIR}/src")

####################### Begin compiler configurations ################### {{{

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # Needed for Clang Tooling.

# Default to C++20.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

function(CHECK_COMPILER_VERSION MIN_VERSION)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS MIN_VERSION)
        message(FATAL_ERROR "${CMAKE_CXX_COMPILER_ID} Compiler version too old. Required minimum version: ${MIN_VERSION}")
    endif()
endfunction()

add_definitions(-DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")  # Needed to print file paths.


####################### Project Level CXX flags ################### {{{
## Floating point options https://gcc.gnu.org/wiki/FloatingPointMath
#set(PROJECT_CXX_FLAGS -frounding-math -fsignaling-nans)
set(PROJECT_CXX_FLAGS_DEBUG -Wall -Wextra -Werror -Wnon-virtual-dtor -Wold-style-cast -Wsign-compare -Wmissing-field-initializers -Wno-unused-parameter -Wno-unused-function)
list(APPEND PROJECT_CXX_FLAGS ${PROJECT_CXX_FLAGS_DEBUG})
if(OPTIMIZE_FOR_NATIVE)
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
    if(COMPILER_SUPPORTS_MARCH_NATIVE)
        list(APPEND PROJECT_CXX_FLAGS -march=native)
    endif()
endif()

add_compile_options(${PROJECT_CXX_FLAGS})
####################### Project Level CXX flags ################### }}}

######################## End compiler configurations #################### }}}

include(CTest)

## BOOST,ss
set(BOOST_MIN_VERSION "1.66.0" COMPONENTS program_options) # COMPONENTS program_options
SET(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
message(STATUS "Boost Include directory: ${Boost_INCLUDE_DIR}")
message(STATUS "Boost Library directories: ${Boost_LIBRARY_DIRS}")
list(APPEND LIBS ${Boost_LIBRARIES})
list(APPEND LIBS ${CMAKE_DL_LIBS})
message(STATUS "Libraries: ${LIBS}")
include_directories(SYSTEM "${Boost_INCLUDE_DIR}")

include_directories("${CMAKE_SOURCE_DIR}")  # Include the core headers via "src".

add_subdirectory(src)

# Configure a header file to pass some of the CMake settings to the source code
configure_file (
        "${PROJECT_SOURCE_DIR}/project-config.h.in"
        "${PROJECT_BINARY_DIR}/include/project-config.h"
)
