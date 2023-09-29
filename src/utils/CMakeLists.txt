set(UTILS_LIB_HEADERS
        CheckBounds.h
        CommandLine.h
        FileParser.h
        Helpers.h
        json.hpp
        Profiler.h
        Project.h
        Stopwatch.h
        math/RootFinder.h
        math/blas/MyBLAS.h
        math/blas/Vector.h
        math/blas/Matrix.h
        math/blas/LU.h
        math/blas/LUP.h
)

set(UTILS_LIB_SOURCES
        utils_stub.cpp
)

if(NOT TARGET utils)
    # Create utils
    add_library(utils STATIC ${UTILS_LIB_SOURCES} ${UTILS_LIB_HEADERS})
    target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif ()
