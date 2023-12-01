/**
* @file FluxCalculationTests.cpp
* @author Arjun Earthperson
* @date 12/01/2023
*
* @brief This file contains test cases for evaluating the accuracy and performance the calculated flux.
 */

#include "relaxation/SORPJ.h"
#include "physics/diffusion/DiffusionMatrix.h"
#include "physics/diffusion/DiffusionParams.h"
#include "solver/LinearSolverParams.h"

#include <gtest/gtest.h>

#define TOLERANCE 1e-14
#define FLOAT_TYPE long double

using namespace MyPhysics::Diffusion;

// Define a list of types to run the tests with
typedef ::testing::Types<float, double, long double> FloatTypes;
TYPED_TEST_SUITE(FluxCalculationTests, FloatTypes);

/**
* @class FluxCalculationTests
* @brief Test fixture for testing the flux calculations
* @tparam T The floating-point type to be used for testing.
 */

template <typename T>
class FluxCalculationTests : public ::testing::Test {
  protected:
    MyBLAS::LazyVector<T> getSources(MyPhysics::Diffusion::Params<T> &p) {
        return MyBLAS::LazyMatrix<T>(p.getM(), p.getN(), [](){
            return 1;
        }).asRowVector();
    }
};

TYPED_TEST(FluxCalculationTests, ConstructorTest) {
    auto diffusionParams = MyPhysics::Diffusion::Params<TypeParam>()
                 .setA(100)
                 .setB(100)
                 .setDiffusionCoefficient(0.033f)
                 .setMacroscopicRemovalCrossSection(0.77f)
                 .setM(9)
                 .setN(9);

    auto solverParams = MyBLAS::Solver::LazyParameters<TypeParam>()
        .setMaxIterations(5000)
        .setThreshold(1.0e-6f);

    const auto A = Matrix<TypeParam>(params);
    const auto b = this->getSources(params);

    MyBLAS::Solver::Solution<TypeParam> solution = MyRelaxationMethod::applyPointJacobi(A, b, solverParams.getMaxIterations(), solverParams.getThreshold());

    EXPECT_EQ(matrix.getParams().getA(), 1.0);
    EXPECT_EQ(matrix.getParams().getB(), 1.0);
    EXPECT_EQ(matrix.getParams().getM(), 10);
    EXPECT_EQ(matrix.getParams().getN(), 10);
}

/**
* @brief Test case for testing the copy constructor of the DiffusionMatrix class.
* It checks if the copy constructor correctly duplicates the matrix with the same parameters.
 */
TYPED_TEST(FluxCalculationTests, CopyConstructorTest) {
    Params<TypeParam> params;
    params.setA(1.0f).setB(1.0f).setM(10).setN(10).setDiffusionCoefficient(0.1f).setMacroscopicRemovalCrossSection(0.2f);
    Matrix<TypeParam> matrix1(params);
    Matrix<TypeParam> matrix2(matrix1);
    EXPECT_EQ(matrix2.getParams().getA(), 1.0);
    EXPECT_EQ(matrix2.getParams().getB(), 1.0);
    EXPECT_EQ(matrix2.getParams().getM(), 10);
    EXPECT_EQ(matrix2.getParams().getN(), 10);
}

/**
* @brief Test case for testing the assignment operator of the DiffusionMatrix class.
* It ensures that the assignment operator creates a copy of the matrix with the same parameters.
 */
TYPED_TEST(FluxCalculationTests, AssignmentOperatorTest) {
    Params<TypeParam> params;
    params.setA(1.0f).setB(1.0f).setM(10).setN(10).setDiffusionCoefficient(0.1f).setMacroscopicRemovalCrossSection(0.2f);
    Matrix<TypeParam> matrix1(params);
    Matrix<TypeParam> matrix2 = matrix1;
    EXPECT_EQ(matrix2.getParams().getA(), 1.0);
    EXPECT_EQ(matrix2.getParams().getB(), 1.0);
    EXPECT_EQ(matrix2.getParams().getM(), 10);
    EXPECT_EQ(matrix2.getParams().getN(), 10);
}

/**
* @brief Test case for ensuring that matrix dimensions match the mesh dimensions.
* It checks if the matrix dimensions are calculated correctly based on the provided parameters.
 */
TYPED_TEST(FluxCalculationTests, MatrixDimensionsMatchMeshDimensionsTest) {
    Params<TypeParam> params;
    const size_t m = 3810;
    const size_t n = 1029;
    params.setA(1.0f).setB(1.0f).setM(m).setN(n).setDiffusionCoefficient(0.1f).setMacroscopicRemovalCrossSection(0.2f);
    Matrix<TypeParam> matrix(params);
    EXPECT_EQ(matrix.getRows(), params.getM() * params.getN());
    EXPECT_EQ(matrix.getCols(), params.getM() * params.getN());
}

/**
* @brief Fill a diffusion matrix and vector using a naive approach.
* In the diffusion matrix, the non-zero elements are on the diagonal (i == j), and on the positions where `i` and `j`
* differ by `1` or `n` (the number of columns). This is because in the diffusion matrix, each node is connected to its
* immediate neighbors (left, right, up, down) in the grid.
* @param c Constants for the diffusion equation.
* @return The filled diffusion matrix.
 */
template <typename TypeParam>
static MyBLAS::Matrix<TypeParam> naive_fill_diffusion_matrix_and_vector(const MyPhysics::Diffusion::Constants<TypeParam> &c) {
    const size_t m = c.m;
    const size_t n = c.n;

    auto A = MyBLAS::Matrix<TypeParam>(std::vector<std::vector<TypeParam>>(m*n, std::vector<TypeParam>(m*n, 0)));
    // Loop through all the nodes i = 1, ..., m and j = 1, ..., n
    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            // Calculate the index of the current node in the matrix A and vector B
            size_t idx = (i - 1) * n + (j - 1);

            // Fill the diagonal element of the matrix A using the given equation
            A[idx][idx] = c.diagonal;

            // Fill the off-diagonal elements of the matrix A using the given equation
            if (i > 1) {
                A[idx][idx - n] = c.minus_D_over_delta_squared;
            }
            if (i < m) {
                A[idx][idx + n] = c.minus_D_over_delta_squared;
            }
            if (j > 1) {
                A[idx][idx - 1] = c.minus_D_over_gamma_squared;
            }
            if (j < n) {
                A[idx][idx + 1] = c.minus_D_over_gamma_squared;
            }
        }
    }
    return A;
}

/**
* @brief Test case for verifying that the DiffusionMatrix matches the results of naive matrix filling for a square grid.
* It compares the generated matrix against a matrix filled using the naive approach for a square grid.
 */
TYPED_TEST(FluxCalculationTests, DiffusionEquationSquareFillTest) {
    auto p = MyPhysics::Diffusion::Params<TypeParam>()
                 .setA(100)
                 .setB(100)
                 .setDiffusionCoefficient(0.033f)
                 .setMacroscopicRemovalCrossSection(0.77f)
                 .setM(9)
                 .setN(9);

    auto memoryBacked = naive_fill_diffusion_matrix_and_vector(MyPhysics::Diffusion::Constants<TypeParam>::compute(p));
    Matrix<TypeParam> matrix(p);
    for(size_t row = 0; row < matrix.getRows(); row++) {
        for(size_t col = 0; col < matrix.getCols(); col++) {
            EXPECT_EQ(matrix[row][col], memoryBacked[row][col]);
        }
    }
}

/**
 * @brief This test case checks if the diffusion matrix and vector are correctly filled for a wide configuration.
 * The test case sets up a diffusion parameter object with specific values and uses it to compute a diffusion constant.
 * The constant is then used to fill a matrix and a vector using a naive method. The test case then checks if the
 * elements of the matrix match with the elements of the matrix filled using the naive method.
 *
 * @tparam TypeParam The type of the elements in the matrix and vector.
 */
TYPED_TEST(FluxCalculationTests, DiffusionEquationWideFillTest) {
    auto p = MyPhysics::Diffusion::Params<TypeParam>()
                 .setA(100)
                 .setB(100)
                 .setDiffusionCoefficient(0.033f)
                 .setMacroscopicRemovalCrossSection(0.77f)
                 .setM(6)
                 .setN(10);

    auto memoryBacked = naive_fill_diffusion_matrix_and_vector(MyPhysics::Diffusion::Constants<TypeParam>::compute(p));
    Matrix<TypeParam> matrix(p);
    for(size_t row = 0; row < matrix.getRows(); row++) {
        for(size_t col = 0; col < matrix.getCols(); col++) {
            EXPECT_EQ(matrix[row][col], memoryBacked[row][col]);
        }
    }
}

/**
 * @brief This test case checks if the diffusion matrix and vector are correctly filled for a tall configuration.
 * The test case sets up a diffusion parameter object with specific values and uses it to compute a diffusion constant.
 * The constant is then used to fill a matrix and a vector using a naive method. The test case then checks if the
 * elements of the matrix match with the elements of the matrix filled using the naive method.
 *
 * @tparam TypeParam The type of the elements in the matrix and vector.
 */
TYPED_TEST(FluxCalculationTests, DiffusionEquationTallFillTest) {
    auto p = MyPhysics::Diffusion::Params<TypeParam>()
                 .setA(100)
                 .setB(100)
                 .setDiffusionCoefficient(0.033f)
                 .setMacroscopicRemovalCrossSection(0.77f)
                 .setM(13)
                 .setN(7);

    auto memoryBacked = naive_fill_diffusion_matrix_and_vector(MyPhysics::Diffusion::Constants<TypeParam>::compute(p));
    Matrix<TypeParam> matrix(p);
    for(size_t row = 0; row < matrix.getRows(); row++) {
        for(size_t col = 0; col < matrix.getCols(); col++) {
            EXPECT_EQ(matrix[row][col], memoryBacked[row][col]);
        }
    }
}

/**
 * @brief This test case checks if the diffusion matrix and vector are correctly filled for a square configuration.
 * The test case sets up a diffusion parameter object with specific values and uses it to compute a diffusion constant.
 * The constant is then used to fill a matrix and a vector using a naive method. The test case then checks if the
 * elements of the matrix match with the elements of the matrix filled using the naive method.
 *
 * @tparam TypeParam The type of the elements in the matrix and vector.
 */
TYPED_TEST(FluxCalculationTests, DiffusionMatrix32Test) {
    auto p = MyPhysics::Diffusion::Params<TypeParam>()
                 .setA(100)
                 .setB(100)
                 .setDiffusionCoefficient(0.033f)
                 .setMacroscopicRemovalCrossSection(0.77f)
                 .setM(32)
                 .setN(32);

    auto memoryBacked = naive_fill_diffusion_matrix_and_vector(MyPhysics::Diffusion::Constants<TypeParam>::compute(p));
    Matrix<TypeParam> matrix(p);
    for(size_t row = 0; row < matrix.getRows(); row++) {
        for(size_t col = 0; col < matrix.getCols(); col++) {
            EXPECT_EQ(matrix[row][col], memoryBacked[row][col]);
        }
    }
}

/**
 * @brief This test case checks if two matrices with the same parameters are equal.
 * The test case sets up two diffusion parameter objects with the same values and uses them to create two matrices.
 * The test case then checks if the elements of the two matrices are equal.
 *
 * @tparam TypeParam The type of the elements in the matrix.
 */
TYPED_TEST(FluxCalculationTests, MatrixEqualityTest) {
    Params<TypeParam> params;
    params.setA(1.0f)
        .setB(1.0f)
        .setM(9)
        .setN(11)
        .setDiffusionCoefficient(0.1f)
        .setMacroscopicRemovalCrossSection(0.2f);
    Matrix<TypeParam> matrix1(params);
    Matrix<TypeParam> matrix2(params);
    // Check that the two matrices are equal
    for (size_t i = 0; i < matrix1.getRows(); ++i) {
        for (size_t j = 0; j < matrix1.getCols(); ++j) {
            EXPECT_EQ(matrix1[i][j], matrix2[i][j]);
        }
    }
}

/**
 * @brief This test case checks if two matrices with different parameters are not equal.
 * The test case sets up two diffusion parameter objects with different values and uses them to create two matrices.
 * The test case then checks if the elements of the two matrices are not equal.
 *
 * @tparam TypeParam The type of the elements in the matrix.
 */
TYPED_TEST(FluxCalculationTests, MatrixInequalityTest) {
    Params<TypeParam> p1;
    p1.setA(1.0f)
        .setB(1.0f)
        .setM(9)
        .setN(11)
        .setDiffusionCoefficient(0.1f)
        .setMacroscopicRemovalCrossSection(0.2f);
    Matrix<TypeParam> matrix1(p1);
    Params<TypeParam> p2;
    p2.setA(1.0f)
        .setB(1.0f)
        .setM(11)
        .setN(9)
        .setDiffusionCoefficient(0.002381f)
        .setMacroscopicRemovalCrossSection(1e-2f);
    Matrix<TypeParam> matrix2(p2);
    // Check that the two matrices are not equal
    for (size_t i = 0; i < matrix1.getRows(); ++i) {
        for (size_t j = 0; j < matrix1.getCols(); ++j) {
            if (matrix1[i][j] != 0 && matrix2[i][j] != 0) {
                EXPECT_NE(matrix1[i][j], matrix2[i][j]);
            }
        }
    }
}

/**
 * @brief This test case checks if the size of the matrix is correct.
 * The test case sets up a diffusion parameter object with specific values and uses it to create a matrix.
 * The test case then checks if the number of rows and columns of the matrix match with the expected values.
 *
 * @tparam TypeParam The type of the elements in the matrix.
 */
TYPED_TEST(FluxCalculationTests, MatrixSizeTest) {
    Params<TypeParam> params;
    Params<TypeParam> p;
    p.setA(1.0f)
        .setB(1.0f)
        .setM(11)
        .setN(11)
        .setDiffusionCoefficient(0.1f)
        .setMacroscopicRemovalCrossSection(0.2f);
    Matrix<TypeParam> matrix(params);
    // Check that the size of the matrix is correct
    EXPECT_EQ(matrix.getRows(), params.getM() * params.getN());
    EXPECT_EQ(matrix.getCols(), params.getM() * params.getN());
}

/**
 * @brief This test case checks if the boundary elements of the diffusion matrix are correctly filled.
 * The test case sets up a diffusion parameter object with specific values and uses it to compute a diffusion constant.
 * The constant is then used to fill a matrix using a naive method. The test case then checks if the boundary elements
 * of the matrix match with the boundary elements of the matrix filled using the naive method.
 *
 * @tparam TypeParam The type of the elements in the matrix.
 */
TYPED_TEST(FluxCalculationTests, DiffusionEquationBoundaryCheck) {
    Params<TypeParam> p;
    p.setA(1.0f)
        .setB(1.0f)
        .setM(5)
        .setN(7)
        .setDiffusionCoefficient(0.1f)
        .setMacroscopicRemovalCrossSection(0.2f);
    Matrix<TypeParam> matrix(p);
    auto memoryBacked = naive_fill_diffusion_matrix_and_vector(MyPhysics::Diffusion::Constants<TypeParam>::compute(p));

    // Check the boundary elements of the matrix
    for (size_t i = 0; i < matrix.getRows(); ++i) {
        EXPECT_EQ(matrix[i][0], memoryBacked[i][0]);
        EXPECT_EQ(matrix[i][matrix.getCols() - 1], memoryBacked[i][matrix.getCols() - 1]);
    }
    for (size_t j = 0; j < matrix.getCols(); ++j) {
        EXPECT_EQ(matrix[0][j], memoryBacked[0][j]);
        EXPECT_EQ(matrix[matrix.getRows() - 1][j], memoryBacked[matrix.getRows() - 1][j]);
    }

    // Check the boundary elements of the matrix for the second row and second column
    EXPECT_EQ(matrix[1][0], memoryBacked[1][0]);
    EXPECT_EQ(matrix[1][matrix.getCols() - 1], memoryBacked[1][matrix.getCols() - 1]);
    EXPECT_EQ(matrix[0][1], memoryBacked[0][1]);
    EXPECT_EQ(matrix[matrix.getRows() - 1][1], memoryBacked[matrix.getRows() - 1][1]);
}

/**
 * @brief Test case for checking the generation of diffusion equation matrix.
 *
 * Creates a 3x3 matrix with a diffusion coefficient of 0.1 and a macroscopic removal cross section of 0.2.
 * Matrix is filled using a generator function that sets the diagonal elements to 1.0 and the off-diagonal elements to 0
 * The test case then checks if the elements of the matrix are as expected.
 *
 * @tparam TypeParam The type of the elements in the matrix.
 */
TYPED_TEST(FluxCalculationTests, DiffusionEquationGeneratorTest) {
    Params<TypeParam> params;
    params
        .setA(static_cast<TypeParam>(1.0))
        .setB(static_cast<TypeParam>(1.0)).setM(3).setN(3)
        .setDiffusionCoefficient(static_cast<TypeParam>(0.1))
        .setMacroscopicRemovalCrossSection(static_cast<TypeParam>(0.2));
    auto generator = [](size_t i, size_t j) {
        if (i == j) {
            return static_cast<TypeParam>(1.0);
        } else {
            return static_cast<TypeParam>(0.0);
        }
    };
    Matrix<TypeParam> matrix(params, generator);
    // Check the elements of the matrix
    EXPECT_EQ(matrix[0][0], static_cast<TypeParam>(1.0));
    EXPECT_EQ(matrix[0][1], static_cast<TypeParam>(0.0));
    EXPECT_EQ(matrix[0][2], static_cast<TypeParam>(0.0));
    EXPECT_EQ(matrix[1][0], static_cast<TypeParam>(0.0));
    EXPECT_EQ(matrix[1][1], static_cast<TypeParam>(1.0));
    EXPECT_EQ(matrix[1][2], static_cast<TypeParam>(0.0));
    EXPECT_EQ(matrix[2][0], static_cast<TypeParam>(0.0));
    EXPECT_EQ(matrix[2][1], static_cast<TypeParam>(0.0));
    EXPECT_EQ(matrix[2][2], static_cast<TypeParam>(1.0));
}

/**
 * @brief Test case for checking the diagonal elements of the diffusion matrix.
 *
 * Creates a 20x20 matrix with a diffusion coefficient of 0.1 and a macroscopic removal cross section of 0.2.
 * The matrix is filled using the naive_fill_diffusion_matrix_and_vector function.
 * The test case then checks if the diagonal elements of the matrix are as expected.
 *
 * @tparam TypeParam The type of the elements in the matrix.
 */
TYPED_TEST(FluxCalculationTests, DiagonalElementTest) {
    Params<TypeParam> p;
    p.setA(1.0f)
        .setB(1.0f)
        .setM(20)
        .setN(20)
        .setDiffusionCoefficient(0.1f)
        .setMacroscopicRemovalCrossSection(0.2f);
    Matrix<TypeParam> matrix(p);
    auto memoryBacked = naive_fill_diffusion_matrix_and_vector(MyPhysics::Diffusion::Constants<TypeParam>::compute(p));
    Constants<TypeParam> constants = Constants<TypeParam>::compute(p);
    for (size_t i = 0; i < matrix.getRows(); ++i) {
        // Check that the filled matrix and the generated matrix are in agreement with each other
        EXPECT_EQ(matrix[i][i], memoryBacked[i][i]);
        // Check that they are in agreement with the expected value (from constants)
        EXPECT_EQ(matrix[i][i], constants.diagonal);
    }
}

/**
 * @brief Test case for checking the off-diagonal elements of the diffusion matrix.
 *
 * Creates a 10x10 matrix with a diffusion coefficient of 0.1 and a macroscopic removal cross section of 0.2.
 * The matrix is filled using the naive_fill_diffusion_matrix_and_vector function.
 * The test case then checks if the off-diagonal elements of the matrix are as expected.
 *
 * @tparam TypeParam The type of the elements in the matrix.
 */
TYPED_TEST(FluxCalculationTests, OffDiagonalElementTest) {
    Params<TypeParam> params;
    params.setA(1.0f)
        .setB(1.0f)
        .setM(10)
        .setN(10)
        .setDiffusionCoefficient(0.1f)
        .setMacroscopicRemovalCrossSection(0.2f);
    Matrix<TypeParam> matrix(params);
    auto memoryBacked = naive_fill_diffusion_matrix_and_vector(MyPhysics::Diffusion::Constants<TypeParam>::compute(params));
    Constants<TypeParam> constants = Constants<TypeParam>::compute(params);
    for (size_t i = 0; i < matrix.getRows(); ++i) {
        for (size_t j = 0; j < matrix.getCols(); ++j) {
            // Check if the elements of the two matrices are equal
            ASSERT_EQ(matrix[i][j], memoryBacked[i][j]);
            // Check if the off-diagonal elements are the expected value
            if (i != j) {
                if ((i / constants.n) == (j / constants.n) && (i == j + 1 || i == j - 1)) {
                    ASSERT_EQ(matrix[i][j], constants.minus_D_over_gamma_squared);
                } else if ((i % constants.n) == (j % constants.n) && (i == j + constants.n || i == j - constants.n)) {
                    ASSERT_EQ(matrix[i][j], constants.minus_D_over_delta_squared);
                } else {
                    ASSERT_EQ(matrix[i][j], 0);
                }
            }
        }
    }
}

/**
 * @brief Test case for checking the zero elements of the diffusion matrix.
 *
 * Creates a 7x11 matrix with a diffusion coefficient of 0.1 and a macroscopic removal cross section of 0.2.
 * The matrix is filled using the naive_fill_diffusion_matrix_and_vector function.
 * The test case then checks if the elements of the matrix that are supposed to be zero are indeed zero.
 *
 * @tparam TypeParam The type of the elements in the matrix.
 */
TYPED_TEST(FluxCalculationTests, ZeroElementTest) {
    Params<TypeParam> params;
    params.setA(1.0f)
        .setB(1.0f)
        .setM(7)
        .setN(11)
        .setDiffusionCoefficient(0.1f)
        .setMacroscopicRemovalCrossSection(0.2f);
    Matrix<TypeParam> matrix(params);
    auto memoryBacked = naive_fill_diffusion_matrix_and_vector(MyPhysics::Diffusion::Constants<TypeParam>::compute(params));
    for (size_t i = 0; i < matrix.getRows(); ++i) {
        for (size_t j = 0; j < matrix.getCols(); ++j) {
            if (i != j) {
                if (std::abs(static_cast<int>(i) - static_cast<int>(j)) != 1) {
                    if (std::abs(static_cast<int>(i) - static_cast<int>(j)) != static_cast<int>(params.getN())) {
                        EXPECT_EQ(matrix[i][j], memoryBacked[i][j]);
                        EXPECT_EQ(matrix[i][j], static_cast<TypeParam>(0.0));
                    }
                }
            }
        }
    }
}

/**
 * @brief Test case for checking the symmetry of the diffusion matrix.
 *
 * This test case creates an 8x3 matrix with a diffusion coefficient of 0.1 and a macroscopic removal cross section of 0.2.
 * The matrix is filled using the naive_fill_diffusion_matrix_and_vector function.
 * The test case then checks if the matrix is symmetric, i.e., if matrix[i][j] is equal to matrix[j][i] for all i and j.
 *
 * @tparam TypeParam The type of the elements in the matrix.
 */
TYPED_TEST(FluxCalculationTests, MatrixSymmetryTest) {
    Params<TypeParam> p;
    p.setA(1.0f)
        .setB(1.0f)
        .setM(8)
        .setN(3)
        .setDiffusionCoefficient(0.1f)
        .setMacroscopicRemovalCrossSection(0.2f);
    Matrix<TypeParam> matrix(p);
    auto memoryBacked = naive_fill_diffusion_matrix_and_vector(MyPhysics::Diffusion::Constants<TypeParam>::compute(p));
    for (size_t i = 0; i < matrix.getRows(); ++i) {
        for (size_t j = 0; j < matrix.getCols(); ++j) {
            EXPECT_EQ(matrix[i][j], memoryBacked[i][j]);
            EXPECT_EQ(matrix[i][j], matrix[j][i]);
        }
    }
}

/**
 * @brief Test case for checking the performance of the symmetry test for a square matrix.
 *
 * This test case creates a 91x91 matrix with a diffusion coefficient of 0.1 and a macroscopic removal cross section of 0.2.
 * The matrix is filled using the naive_fill_diffusion_matrix_and_vector function.
 * The test case then checks the symmetry of the matrix and measures the time taken for the operation.
 *
 * @tparam TypeParam The type of the elements in the matrix.
 */
TYPED_TEST(FluxCalculationTests, SquareMatrixSymmetryTimingTest) {
    Params<TypeParam> p;
    p.setA(1.0f)
        .setB(1.0f)
        .setM(91)
        .setN(91)
        .setDiffusionCoefficient(0.1f)
        .setMacroscopicRemovalCrossSection(0.2f);
    auto start = std::chrono::high_resolution_clock::now();
    Matrix<TypeParam> matrix(p);
    for (size_t i = 0; i < matrix.getRows(); ++i) {
        for (size_t j = 0; j < matrix.getCols(); ++j) {
            EXPECT_EQ(matrix[i][j], matrix[j][i]);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "LazyMatrix: symmetry test for square (91, 91) matrix with ~2^13 elements took " << duration << " milliseconds." << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto memoryBacked = naive_fill_diffusion_matrix_and_vector(MyPhysics::Diffusion::Constants<TypeParam>::compute(p));
    for (size_t i = 0; i < matrix.getRows(); ++i) {
        for (size_t j = 0; j < matrix.getCols(); ++j) {
            EXPECT_EQ(matrix[i][j], matrix[j][i]);
        }
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Matrix: symmetry test for square (91, 91) matrix with ~2^13 elements took " << duration << " milliseconds." << std::endl;
    EXPECT_TRUE(duration < 8000);
}

/**
 * @brief Test case for checking the performance of the symmetry test for a wide matrix.
 *
 * This test case creates an 8x1024 matrix with a diffusion coefficient of 0.1 and a macroscopic removal cross section of 0.2.
 * The matrix is filled using the naive_fill_diffusion_matrix_and_vector function.
 * The test case then checks the symmetry of the matrix and measures the time taken for the operation.
 *
 * @tparam TypeParam The type of the elements in the matrix.
 */
TYPED_TEST(FluxCalculationTests, WideMatrixSymmetryTimingTest) {
    Params<TypeParam> p;
    p.setA(1.0f)
        .setB(1.0f)
        .setM(8)
        .setN(1024)
        .setDiffusionCoefficient(0.1f)
        .setMacroscopicRemovalCrossSection(0.2f);
    auto start = std::chrono::high_resolution_clock::now();
    Matrix<TypeParam> matrix(p);
    for (size_t i = 0; i < matrix.getRows(); ++i) {
        for (size_t j = 0; j < matrix.getCols(); ++j) {
            EXPECT_EQ(matrix[i][j], matrix[j][i]);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "LazyMatrix: symmetry test for wide matrix (8 rows, 1024 columns) with 2^13 elements took " << duration << " milliseconds." << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto memoryBacked = naive_fill_diffusion_matrix_and_vector(MyPhysics::Diffusion::Constants<TypeParam>::compute(p));
    for (size_t i = 0; i < matrix.getRows(); ++i) {
        for (size_t j = 0; j < matrix.getCols(); ++j) {
            EXPECT_EQ(matrix[i][j], matrix[j][i]);
        }
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Matrix: symmetry test for wide matrix (8 rows, 1024 columns) with 2^13 elements took " << duration << " milliseconds." << std::endl;
    EXPECT_TRUE(duration < 8000);
}

/**
 * @brief Test case for checking the performance of the symmetry test for a tall matrix.
 *
 * This test case creates a 1024x8 matrix with a diffusion coefficient of 0.1 and a macroscopic removal cross section of 0.2.
 * The matrix is filled using the naive_fill_diffusion_matrix_and_vector function.
 * The test case then checks the symmetry of the matrix and measures the time taken for the operation.
 *
 * @tparam TypeParam The type of the elements in the matrix.
 */
TYPED_TEST(FluxCalculationTests, TallMatrixSymmetryTimingTest) {
    Params<TypeParam> p;
    p.setA(1.0f)
        .setB(1.0f)
        .setM(1024)
        .setN(8)
        .setDiffusionCoefficient(0.1f)
        .setMacroscopicRemovalCrossSection(0.2f);
    auto start = std::chrono::high_resolution_clock::now();
    Matrix<TypeParam> matrix(p);
    for (size_t i = 0; i < matrix.getRows(); ++i) {
        for (size_t j = 0; j < matrix.getCols(); ++j) {
            EXPECT_EQ(matrix[i][j], matrix[j][i]);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "LazyMatrix: symmetry test for tall matrix (1024 rows, 8 columns) with 2^13 elements took " << duration << " milliseconds." << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto memoryBacked = naive_fill_diffusion_matrix_and_vector(MyPhysics::Diffusion::Constants<TypeParam>::compute(p));
    for (size_t i = 0; i < matrix.getRows(); ++i) {
        for (size_t j = 0; j < matrix.getCols(); ++j) {
            EXPECT_EQ(matrix[i][j], matrix[j][i]);
        }
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Matrix: symmetry test for tall matrix (1024 rows, 8 columns) with 2^13 elements took " << duration << " milliseconds." << std::endl;
    EXPECT_TRUE(duration < 8000);
}
