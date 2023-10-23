set(UTILS_LIB_HEADERS
        CheckBounds.h
        FileParser.h
        Helpers.h
        json.hpp
        Profiler.h
        Stopwatch.h
        CommandlineHelpers.h

        MPICommandLine.h
        CommandLine.h
        Project.h
        mpi/MPIProject.h

        math/blas/BLAS.h
        math/blas/Ops.h
        math/blas/Stats.h

        math/blas/system/Circuit.h

        math/blas/matrix/Matrix.h
        math/blas/matrix/LazyMatrix.h
        math/blas/matrix/ElementwiseExpression.h

        math/blas/vector/LazyVector.h
        math/blas/vector/Vector.h
        math/blas/vector/VectorExpression.h

        math/factorization/LU.h
        math/factorization/LUP.h
        math/factorization/Factorize.h

        math/LinearSolver.h
        math/Random.h
        math/RootFinder.h

        math/relaxation/PointJacobi.h
        math/relaxation/GaussSeidel.h
        math/relaxation/SOR.h
        math/relaxation/SSOR.h
        math/relaxation/RelaxationMethods.h

        physics/diffusion/DiffusionParams.h
        physics/diffusion/DiffusionMatrix.h
        physics/diffusion/DiffusionConstants.h
        math/Math.h
        math/blas/Constants.h
        math/blas/vector/MatrixVectorExpression.h
        math/blas/matrix/ContainerExpression.h
        math/blas/elementwise/ElementwiseAddExpression.h
        mpi/MPIContainer.h
)

set(UTILS_LIB_SOURCES
        utils_stub.cpp
)

if (NOT TARGET utils)
    # Create utils
    add_library(utils STATIC ${UTILS_LIB_SOURCES} ${UTILS_LIB_HEADERS})
    target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif ()

add_subdirectory(physics)
add_subdirectory(math)
