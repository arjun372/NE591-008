add_subdirectory(tests)

set(LIB_HEADERS

        BLAS.h
        Constants.h
        Ops.h

        matrix/Matrix.h
        matrix/LazyMatrix.h
        matrix/ElementwiseExpression.h
        matrix/ContainerExpression.h

        vector/LazyVector.h
        vector/Vector.h
        vector/VectorExpression.h

        vector/MatrixVectorExpression.h

        system/Circuit.h
        solver/LinearSolver.h
        solver/LinearSolverParams.h
)

set(LIB_SOURCES
        stub.cpp
)

if (NOT TARGET utils_math_blas)
    # Create target
    add_library(utils_math_blas STATIC ${LIB_HEADERS} ${LIB_SOURCES})
    target_include_directories(utils_math_blas PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif ()

# Link target against libraries
#target_link_libraries(profiler utils Boost::program_options)
target_link_libraries(utils_math_blas Boost::program_options)
#target_link_libraries(profiler OpenMP::OpenMP_CXX)
