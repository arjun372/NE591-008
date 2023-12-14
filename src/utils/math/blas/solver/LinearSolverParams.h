#ifndef NE591_008_LINEARSOLVERPARAMS_H
#define NE591_008_LINEARSOLVERPARAMS_H

/**
* @file LinearSolverParams.h
* @brief This file contains the definition of the LinearSolverParams struct which encapsulates
* the parameters required for a solver with templated matrix and vector types.
*
* The LinearSolverParams struct is a template structure that holds parameters necessary for
* solving problems with templated matrix and vector types. It includes parameters such as the size
* of the matrix/vector, the convergence threshold, the relaxation factor, the maximum number of
* iterations, the coefficient matrix, and the vector of constants.
*
* The LinearSolverParams struct also provides methods to set and get these parameters, and
* includes default values for the parameters.
*
* This file is part of the MyBLAS::Solver namespace, which encapsulates functionality related to solving
* linear algebra problems.
*
* @author Arjun Earthperson
* @date 12/01/2023
 */
#include <cmath>
#include <cstddef>
#include <limits>

#include "matrix/LazyMatrix.h"
#include "matrix/Matrix.h"
#include "vector/LazyVector.h"
#include "vector/Vector.h"

/**
 * @brief Namespace for the Solver module
 * This namespace encapsulates functionality related to solving linear algebra problems.
 */
namespace MyBLAS::Solver {

/**
 * @brief Template struct to hold parameters for a solver with templated matrix and vector types.
 *
 * This struct contains parameters necessary for solving problems with templated matrix and vector types.
 *
 * @tparam MatrixType The matrix type used for storing coefficients.
 * @tparam VectorType The vector type used for storing constants.
 * @tparam T The data type used for storing parameters.
 */
template <template <typename> class MatrixType, template <typename> class VectorType, typename T>
struct TemplatedParameters {

  public:
    /**
     * @brief Size of the matrix/vector.
     *
     * This value represents the size of the matrix/vector.
     */
    size_t n = std::numeric_limits<size_t>::quiet_NaN();

    /**
     * @brief The convergence threshold.
     *
     * This value represents the convergence threshold for the solver.
     */
    T convergence_threshold = std::numeric_limits<T>::quiet_NaN();

    /**
     * @brief The relaxation factor.
     *
     * This value represents the relaxation factor for the solver.
     */
    T relaxation_factor = std::numeric_limits<T>::quiet_NaN();

    /**
     * @brief Maximum number of iterations to perform.
     *
     * This value represents the maximum number of iterations to perform.
     */
    size_t max_iterations = std::numeric_limits<size_t>::quiet_NaN();

    /**
     * @brief Coefficient matrix A.
     *
     * This matrix holds the coefficients for the problem.
     */
    MatrixType<T> coefficients;

    /**
     * @brief Vector of constants b.
     *
     * This vector holds the constants for the problem.
     */
    VectorType<T> constants;

    /**
     * @brief Vector of initial guess for x.
     *
     * This vector holds the initial guess for x for the solution.
     */
    VectorType<T> initial_guess;

    /**
     * @brief Eigenvalue guess
     *
     * Holds the initial guess for the eigenvalue.
     */
    T eigenvalue;

    /**
     * @brief Default constructor for TemplatedParameters.
     */
    TemplatedParameters() = default;

    /**
     * @brief Constructor with initialization of parameters.
     *
     * @param _n The size of the matrix/vector.
     * @param _threshold The convergence threshold.
     * @param _relaxation_factor The relaxation factor.
     * @param _max_iterations The maximum number of iterations to perform.
     * @param _coefficients The coefficient matrix.
     * @param _constants The vector of constants.
     * @param _initial_guess The initial guess for the solution vector x.
     */
    TemplatedParameters(size_t _n, T _threshold, T _relaxation_factor, size_t _max_iterations,
                        const MatrixType<T>& _coefficients, const VectorType<T>& _constants, VectorType<T>& _initial_guess)
        : n(_n), convergence_threshold(_threshold), relaxation_factor(_relaxation_factor),
          max_iterations(_max_iterations), coefficients(_coefficients), constants(_constants), initial_guess(_initial_guess) {}

    /**
     * @brief Set the size of the matrix/vector.
     *
     * @param n The size of the matrix/vector.
     * @return A reference to the TemplatedParameters struct.
     *
     * @throw std::invalid_argument if the provided value is zero.
     */
    TemplatedParameters& setSize(size_t _n) {
        if (_n == 0) {
            throw std::invalid_argument("Invalid value for n. It should be a positive number.");
        }
        n = _n;
        return *this;
    }

    /**
     * @brief Set the convergence threshold.
     *
     * @param threshold The convergence threshold.
     * @return A reference to the TemplatedParameters struct.
     *
     * @throw std::invalid_argument if the provided value is non-positive or not finite.
     */
    TemplatedParameters& setConvergenceThreshold(T _threshold) {
        if (_threshold <= 0 || !std::isfinite(_threshold)) {
            throw std::invalid_argument("Invalid value for convergence threshold. It should be a positive finite number.");
        }
        convergence_threshold = _threshold;
        return *this;
    }

    /**
     * @brief Set the eigenvalue
     *
     * @param threshold The eigenvalue.
     * @return A reference to the TemplatedParameters struct.
     *
     * @throw std::invalid_argument if the provided value is non-positive or not finite.
     */
    TemplatedParameters& setEigenvalue(T _eigenvalue) {
        eigenvalue = _eigenvalue;
        return *this;
    }

    /**
     * @brief Set the relaxation factor.
     *
     * @param relaxation_factor The relaxation factor.
     * @return A reference to the TemplatedParameters struct.
     *
     * @throw std::invalid_argument if the provided value is non-positive or not finite.
     */
    TemplatedParameters& setRelaxationFactor(T _relaxation_factor) {
        if (_relaxation_factor <= 0 || !std::isfinite(_relaxation_factor)) {
            throw std::invalid_argument("Invalid value for relaxation factor. It should be a positive finite number.");
        }
        relaxation_factor = _relaxation_factor;
        return *this;
    }

    /**
     * @brief Set the maximum number of iterations to perform.
     *
     * @param max_iterations The maximum number of iterations to perform.
     * @return A reference to the TemplatedParameters struct.
     *
     * @throw std::invalid_argument if the provided value is zero.
     */
    TemplatedParameters& setMaxIterations(size_t _max_iterations) {
        if (_max_iterations == 0) {
            throw std::invalid_argument("Invalid value for max_iterations. It should be a positive number.");
        }
        max_iterations = _max_iterations;
        return *this;
    }

    /**
     * @brief Set the coefficient matrix.
     *
     * @param coefficients The coefficient matrix.
     * @return A reference to the TemplatedParameters struct.
     *
     * @throw std::invalid_argument if the dimensions of the coefficients matrix do not match with the size.
     */
    TemplatedParameters& setCoefficients(const MatrixType<T>& _coefficients) {
        if (_coefficients.getRows() != n || _coefficients.getCols() != n) {
            throw std::invalid_argument("Invalid dimensions for the coefficients matrix. It should be square and match with the size.");
        }
        coefficients = _coefficients;
        return *this;
    }

    /**
     * @brief Set the vector of constants.
     *
     * @param constants The vector of constants.
     * @return A reference to the TemplatedParameters struct.
     *
     * @throw std::invalid_argument if the size of the constants vector does not match with the size.
     */
    TemplatedParameters& setConstants(const VectorType<T>& _constants) {
        if (_constants.getSize() != n) {
            throw std::invalid_argument("Invalid size for the constants vector. It should match with the size.");
        }
        constants = _constants;
        return *this;
    }

    /**
     * @brief Set the initial guess vector.
     *
     * @param _initial_guess The initial guess vector.
     * @return A reference to the TemplatedParameters struct.
     *
     * @throw std::invalid_argument if the size of the initial guess vector does not match with the size n.
     */
    TemplatedParameters& setInitialGuess(const VectorType<T>& _initial_guess) {
        if (_initial_guess.getSize() != n) {
            throw std::invalid_argument("Invalid size for the constants vector. It should match with the size n.");
        }
        initial_guess = _initial_guess;
        return *this;
    }

    /**
     * @brief Get the size of the matrix/vector.
     *
     * @return The size of the matrix/vector.
     */
    [[nodiscard]] size_t getSize() const { return n; }

    /**
     * @brief Get the convergence threshold.
     *
     * @return The convergence threshold.
     */
    [[nodiscard]] T getThreshold() const { return convergence_threshold; }

    /**
     * @brief Get the relaxation factor.
     *
     * @return The relaxation factor.
     */
    [[nodiscard]] T getRelaxationFactor() const { return relaxation_factor; }

    /**
     * @brief Get the maximum number of iterations to perform.
     *
     * @return The maximum number of iterations to perform.
     */
    [[nodiscard]] size_t getMaxIterations() const { return max_iterations; }

    /**
     * @brief Get the coefficient matrix.
     *
     * @return The coefficient matrix.
     */
    const MatrixType<T>& getCoefficients() const { return coefficients; }

    /**
     * @brief Get the vector of constants.
     *
     * @return The vector of constants.
     */
    const VectorType<T>& getConstants() const { return constants; }

    /**
     * @brief Get the initial guess vector.
     *
     * @return The initial guess vector.
     */
    const VectorType<T>& getInitialGuess() const { return initial_guess; }

    /**
     * @brief Get eigenvalue
     *
     * @return The initial guess vector.
     */
    const T& getEigenValue() const { return eigenvalue; }


    friend std::ostream &operator<<(std::ostream &os, const TemplatedParameters<MatrixType, VectorType, T> &params) {
        os << std::left; // Align text to the left
        os << "::::::::::::::::::::::::::::::: Solver Parameters ::::::::::::::::::::::::::::::" << std::endl;
        os << "--------------------------------------------------------------------------------"<<std::endl;
        try {
            if (params.initial_guess.size()) {
                os << "Initial Guess" << std::endl;
                os << "--------------------------------------------------------------------------------" << std::endl;
                os << params.initial_guess;
                os << "--------------------------------------------------------------------------------" << std::endl;
            }
        } catch (...) {}
        os << std::setw(25) << "Size (n): " << params.n << std::endl;
        os << std::setw(25) << "Convergence Threshold: " << params.convergence_threshold << std::endl;
        os << std::setw(25) << "Relaxation Factor: " << params.relaxation_factor << std::endl;
        os << std::setw(25) << "Max Iterations: " << params.max_iterations;
        return os;
    }
};

/**
 * @brief Structure that represents the input to the linear solver.
 * @tparam T The type of the elements in the matrix/vector.
 */
template <typename T>
struct Parameters : public TemplatedParameters<MyBLAS::Matrix, MyBLAS::Vector, T> {};

/**
 * @brief Structure that represents the input to the linear solver.
 * @tparam T The type of the elements in the matrix/vector.
 */
template <typename T>
struct LazyParameters : public TemplatedParameters<MyBLAS::LazyMatrix, MyBLAS::LazyVector, T> {};

}  // namespace MyBLAS

#endif // NE591_008_LINEARSOLVERPARAMS_H
