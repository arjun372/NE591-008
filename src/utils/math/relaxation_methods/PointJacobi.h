/**
 * @file PointJacobi.h
 * @author Arjun Earthperson
 * @date 09/30/2023
 * @brief TODO:: Document
*/

#ifndef NE591_008_POINTJACOBI_H
#define NE591_008_POINTJACOBI_H

#include <cstddef>
#include "math/blas/Matrix.h"
#include "math/blas/Vector.h"

#include "math/relaxation_methods/RelaxationMethods.h"

// TODO:: Document
namespace MyRelaxationMethod {

    // TODO:: Document
    template <typename T>
    Solution<T> applyPointJacobi(const MyBLAS::Matrix<T>& A, const MyBLAS::Vector<T>& b, const size_t max_iterations, const T tolerance) {

        const size_t n = A.getRows();
        Solution<T> results(n);

        if(!MyBLAS::isSquareMatrix(A)) {
            return results;
        }

        const T tolerance_squared = std::pow(tolerance, 2);
        T iterative_error_squared = std::numeric_limits<T>::max();

        MyBLAS::Vector<T> new_x(n, 0);

        for (results.iterations = 0; results.iterations < max_iterations; (results.iterations)++) {
            for (size_t i = 0; i < n; ++i) {
                T sum = 0;
                for (size_t j = 0; j < n; ++j) {
                    if (i != j) {
                        sum += A[i][j] * results.x[j];
                    }
                }
                new_x[i] = (b[i] - sum) / A[i][i];
            }

            // Check for convergence

            iterative_error_squared = MyBLAS::L2Norm<MyBLAS::Vector<T>, T>(new_x, results.x, n);


            if (iterative_error_squared < tolerance_squared) {
                results.converged = true;
                break;
            }

            results.x = new_x;
        }

        results.iterative_error = std::sqrt(iterative_error_squared);
        return results;
    }
}
#endif //NE591_008_POINTJACOBI_H
