# vim: set foldmarker={{{,}}} foldlevel=0 foldmethod=marker:
cmake_minimum_required(VERSION 3.14)

project(ne591_008)

# In-source build prevention.
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

####################### Options ################### {{{
option(BUILD_TESTS "Build the tests" ON)  # Influences CTest.
option(CODE_COVERAGE "Perform code coverage analysis" OFF)
option(PORTABLE "Build without CPU specific optimizations" OFF)
option(BUILD_PERFORMANCE_BENCHMARKS "Build performance benchmarks" OFF)
option(ENABLE_PGO "Enable Profile-Guided Optimization (PGO)" OFF)
option(ENABLE_LTO "Enable Link Time Optimization (LTO)" ON)
####################### Options ##################### }}}

# Add the resources/cmake folder to Module Search Path
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include_directories("${PROJECT_BINARY_DIR}/include") # add the binary dir include directory for project-config.h
include_directories("${PROJECT_SOURCE_DIR}/src")

####################### Begin compiler configurations ################### {{{

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # Needed for Clang Tooling.

# Default to C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# Enable Link-Time Optimization (LTO)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ${ENABLE_LTO})

function(CHECK_COMPILER_VERSION MIN_VERSION)
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS MIN_VERSION)
        message(FATAL_ERROR "${CMAKE_CXX_COMPILER_ID} Compiler version too old. Required minimum version: ${MIN_VERSION}")
    endif ()
endfunction()

add_definitions(-DPROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")  # Needed to print file paths.

####################### Project Level CXX flags ################### {{{
## Floating point options https://gcc.gnu.org/wiki/FloatingPointMath
#set(PROJECT_CXX_FLAGS -ffast-math)
set(PROJECT_CXX_FLAGS_DEBUG -Wall -Wextra -Werror -Wsign-compare -Wmissing-field-initializers -Wnon-virtual-dtor -Wno-old-style-cast -Wno-cast-function-type -Wno-unused-parameter -Wno-unused-function)
list(APPEND PROJECT_CXX_FLAGS ${PROJECT_CXX_FLAGS_DEBUG})
set(PROJECT_CXX_LINK_FLAGS)
mark_as_advanced(PROJECT_CXX_LINK_FLAGS)

####################### CXX Optimization Flags ################### {{{
# Define a global variable
set(PROJECT_CXX_NON_PORTABLE_FLAGS)
mark_as_advanced(PROJECT_CXX_NON_PORTABLE_FLAGS)
function(CHECK_COMPILER_FLAG_SUPPORT FLAG SUPPORT_VAR FLAGS_VAR)
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG(${FLAG} ${SUPPORT_VAR})
    if (${SUPPORT_VAR})
        set(${FLAGS_VAR} "${${FLAGS_VAR}};${FLAG}" PARENT_SCOPE)
    endif ()
endfunction()
CHECK_COMPILER_FLAG_SUPPORT("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE PROJECT_CXX_NON_PORTABLE_FLAGS)
if (NOT PORTABLE)
    list(APPEND PROJECT_CXX_FLAGS ${PROJECT_CXX_NON_PORTABLE_FLAGS})
endif ()
####################### CXX Optimization Flags ################### }}}
####################### Project Level CXX flags ################### }}}

if (BUILD_TESTS)
    include(CTest)
    # Enable testing for the project
    enable_testing()
    # Add Google Test as a subdirectory
    add_subdirectory(external/googletest)
endif ()

if (CODE_COVERAGE)
    if(CMAKE_COMPILER_IS_GNUCXX)
        message(STATUS "Enabling coverage for GCC")
        list(APPEND PROJECT_CXX_FLAGS --coverage)
        list(APPEND PROJECT_CXX_LINK_FLAGS --coverage)
        add_subdirectory(coverage)
    else ()
        message(WARNING "Coverage only available for GCC ")
    endif ()
endif ()

## Profiler Guided Optimization
if(ENABLE_PGO)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-generate")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-generate")
    elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-use -fprofile-correction")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-use -fprofile-correction")
    endif()
endif()

add_compile_options(${PROJECT_CXX_FLAGS})
add_link_options(${PROJECT_CXX_LINK_FLAGS})
message(STATUS "Extra compile flags: ${PROJECT_CXX_FLAGS}")
message(STATUS "Extra link flags: ${PROJECT_CXX_LINK_FLAGS}")

######################## End compiler configurations #################### }}}

### Boost
set(BOOST_URL https://boostorg.jfrog.io/artifactory/main/release/1.83.0/source/boost_1_83_0.tar.bz2)
set(BOOST_URL_SHA256 6478edfe2f3305127cffe8caf73ea0176c53769f4bf1585be237eb30798c3b8e)
add_subdirectory(external/boost-cmake)

list(APPEND LIBS ${CMAKE_DL_LIBS})
message(STATUS "Libraries: ${LIBS}")

include_directories("${CMAKE_SOURCE_DIR}")  # Include the core headers via "src".

add_subdirectory(src)

# Configure a header file to pass some of the CMake settings to the source code
configure_file(
        "${PROJECT_SOURCE_DIR}/project-config.h.in"
        "${PROJECT_BINARY_DIR}/include/project-config.h"
)
