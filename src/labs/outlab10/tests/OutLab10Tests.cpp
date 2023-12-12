/**
* @file BaseMatrixTests.cpp
* @author Arjun Earthperson
* @date 10/27/2023
*
* @brief This file contains unit tests for the Matrix-Vector operations needed in OutLab10
 */

#include "math/Random.h"
#include "math/blas/matrix/LazyMatrix.h"
#include "math/blas/matrix/Matrix.h"
#include "math/blas/vector/Vector.h"
#include "math/relaxation/ConjugateGradient.h"
#include <gtest/gtest.h>

#define TOLERANCE 1e-14
#define FLOAT_TYPE long double

// Define a list of numeric types to run the tests with
typedef ::testing::Types<float> NumericTypes;
TYPED_TEST_SUITE(OutLab10MatrixTests, NumericTypes);

template <typename T>
class OutLab10MatrixTests : public ::testing::Test {
  public:
    // Helper function to compare two floating point numbers with a tolerance.
    static bool IsClose(T a, T b, FLOAT_TYPE tolerance = TOLERANCE) {
        return std::abs(a - b) < tolerance;
    }
};

/**
 * @brief Test for the Conjugate Gradient method.
 *
 * This test checks the correctness of the Conjugate Gradient method by solving a system of linear equations with a
 * known positive-definite matrix and right-hand side vector. It verifies that the solution vector is close to the
 * expected solution within a given tolerance.
 */
TYPED_TEST(OutLab10MatrixTests, ConjugateGradientTest) {
    // Define a symmetric positive-definite matrix A
    MyBLAS::Matrix<TypeParam> A(3, 3, [](size_t i, size_t j) -> TypeParam {
        if (i == j) return 4.0; // Diagonal elements
        else return 1.0;       // Off-diagonal elements
    });

    // Define a right-hand side vector b
    MyBLAS::Vector<TypeParam> b({11.0, 10.0, 14.0});

    // Define the expected solution vector (known solution)
    MyBLAS::Vector<TypeParam> expected_solution({1.0, 2.0, 3.0});

    // Apply the Conjugate Gradient method to solve the system
    auto solution = MyRelaxationMethod::applyConjugateGradient(A, b, 1000, static_cast<TypeParam>(TOLERANCE));

    // Verify that the solution is close to the expected solution within the given tolerance
    for (size_t i = 0; i < expected_solution.size(); ++i) {
        std::cout<<solution.x[i]<<", "<<expected_solution[i]<<std::endl;
        EXPECT_TRUE(this->IsClose(solution.x[i], expected_solution[i]));
    }
}
