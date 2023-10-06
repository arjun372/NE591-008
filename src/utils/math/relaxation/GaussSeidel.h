/**
 * @file GaussSeidel.h
 * @author Arjun Earthperson
 * @date 09/30/2023
 * @brief TODO:: Document
*/

#ifndef NE591_008_GAUSSSEIDEL_H
#define NE591_008_GAUSSSEIDEL_H

#include "math/LinearSolver.h"
#include "math/blas/Ops.h"
#include "math/blas/Matrix.h"
#include "math/blas/Vector.h"
#include "math/blas/MyBLAS.h"
#include "SOR.h"

/**
 * @namespace MyRelaxationMethod
 * @brief This namespace contains implementations of various relaxation methods for solving systems of linear equations.
 */
namespace MyRelaxationMethod {

    // TODO:: DOCUMENT
    template<typename T>
    MyLinearSolvingMethod::Solution<T> applyGaussSeidel(const MyBLAS::Matrix<T> &A, const MyBLAS::Vector<T> &b,
                                                        const size_t max_iterations, const T tolerance) {
        const T relaxation_factor = 1;
        return applySOR(A, b, max_iterations, tolerance, relaxation_factor);
    }
}

#endif //NE591_008_GAUSSSEIDEL_H
