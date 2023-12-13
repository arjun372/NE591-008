/**
 * @file RelaxationMethods.h
 * @author Arjun Earthperson
 * @date 09/30/2023
 * @brief This file contains the declaration of the RelaxationMethods namespace, which includes the Type enumeration,
 * the TypeKey function, and the Solution struct template.
 */

#ifndef NE591_008_RELAXATIONMETHODS_H
#define NE591_008_RELAXATIONMETHODS_H

#include "BLAS.h"
namespace MyRelaxationMethod {

/**
 * @brief Enum for different types of relaxation methods.
 *
 * This enum represents the different types of relaxation methods that can be used in the program.
 * The methods include Point Jacobi, Gauss Seidel, SOR, and SSOR.
 */
enum Type {
    METHOD_POINT_JACOBI,
    METHOD_SORPJ,
    METHOD_GAUSS_SEIDEL,
    METHOD_SYMMETRIC_GAUSS_SEIDEL,
    METHOD_SOR,
    METHOD_SSOR,
    METHOD_CONJUGATE_GRADIENT,
};

/**
 * @brief Function to get the key of a relaxation method type.
 *
 * This function takes a value of the Type enum and returns the corresponding key as a string.
 *
 * @param value The value of the Type enum.
 * @return The key of the relaxation method type as a string.
 */
const char *TypeKey(MyRelaxationMethod::Type value) {
    static const char *relaxationMethodTypeKeys[] = {"point-jacobi", "SORPJ", "gauss-seidel", "symmetric-gauss-seidel", "SOR", "SSOR", "CG"};
    return relaxationMethodTypeKeys[static_cast<int>(value)];
}

/**
 * @brief Function to approximate the optimal relaxation factor for a given mesh size.
 *
 * This function calculates the optimal relaxation factor for a given mesh size in the x and y directions.
 * The relaxation factor is used in the Successive Over-Relaxation (SOR) method to speed up the convergence of the
 * solution.
 *
 * @tparam T The data type of the mesh size and the relaxation factor.
 * @param meshX The size of the mesh in the x direction.
 * @param meshY The size of the mesh in the y direction.
 * @return The approximate optimal relaxation factor.
 */
template <typename T> T inline approximateOptimalRelaxationFactor(const size_t meshX, const size_t meshY) {
    const T dx = 1 / (static_cast<T>(meshX) - 1);
    const T dy = 1 / (static_cast<T>(meshY) - 1);
    const T rho = std::cos(M_PIl * dx) + std::cos(M_PIl * dy);
    const T factor = 2 / (1 + std::sqrt(1 - std::pow(rho, 2)));
    return factor;
}

/**
 * @brief Function to approximate the optimal relaxation factor for a square mesh.
 *
 * This function calculates the optimal relaxation factor for a square mesh of a given size.
 * The relaxation factor is used in the Successive Over-Relaxation (SOR) method to speed up the convergence of the
 * solution.
 *
 * @tparam T The data type of the mesh size and the relaxation factor.
 * @param meshXY The size of the square mesh.
 * @return The approximate optimal relaxation factor.
 */
template <typename T> T inline approximateOptimalRelaxationFactor(const size_t meshXY) {
    return approximateOptimalRelaxationFactor<T>(meshXY, meshXY);
}

/**
 * @brief Function to perform pre-checks on the coefficient matrix and the right-hand side vector.
 *
 * This function performs several pre-checks on the coefficient matrix and the right-hand side vector before solving
 * the system of linear equations. The checks include verifying that the coefficient matrix is square, that its
 * diagonal does not contain zeros, and that it is diagonally dominant.
 *
 * @tparam T The data type of the elements in the coefficient matrix and the right-hand side vector.
 * @param A The coefficient matrix.
 * @param b The right-hand side vector.
 * @return True if all pre-checks pass, false otherwise.
 */
template <typename MatrixType, typename VectorType>
bool passesPreChecks(const MatrixType &A, const VectorType &b) {

    if (MyBLAS::isEmptyMatrix(A)) {
        std::cerr << "Error: Failed pre-check: coefficient matrix is empty.\n";
        return false;
    }

    if (!MyBLAS::isSquareMatrix(A)) {
        std::cerr << "Error: Failed pre-check: coefficient matrix is non-square.\n";
        return false;
    }

    if (!MyBLAS::noZerosInDiagonal(A)) {
        std::cerr << "Error: Failed pre-check: coefficient matrix diagonal contains 0.\n";
        return false;
    }

    if (!MyBLAS::isDiagonallyDominant(A)) {
        std::cout << "Warning: coefficient matrix is not diagonally dominant, may not converge\n";
    }

    return true;
}

} // namespace MyRelaxationMethod

#endif // NE591_008_RELAXATIONMETHODS_H
