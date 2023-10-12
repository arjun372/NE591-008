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
        math/blas/BLAS.h
        math/blas/Vector.h
        math/blas/Matrix.h
        math/blas/Ops.h
        math/factorization/LU.h
        math/factorization/LUP.h
        math/factorization/Factorize.h
        math/relaxation/PointJacobi.h
        math/relaxation/GaussSeidel.h
        math/relaxation/SOR.h
        math/relaxation/SSOR.h
        math/relaxation/RelaxationMethods.h
        math/LinearSolver.h
        math/blas/system/Circuit.h
        math/Random.h
        CommandlineHelpers.h
        math/blas/Stats.h
)

set(UTILS_LIB_SOURCES
        utils_stub.cpp
)

if (NOT TARGET utils)
    # Create utils
    add_library(utils STATIC ${UTILS_LIB_SOURCES} ${UTILS_LIB_HEADERS})
    target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif ()
