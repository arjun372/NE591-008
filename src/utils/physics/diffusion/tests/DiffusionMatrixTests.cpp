/**
* @file DiffusionMatrixTestss.cpp
* @author Arjun Earthperson
* @date 10/13/2023
*
* @brief TODO::DOCUMENT
*/

#include "physics/diffusion/DiffusionMatrix.h"
#include "physics/diffusion/DiffusionParams.h"

#include <gtest/gtest.h>

#define TOLERANCE 1e-14
#define FLOAT_TYPE long double

using namespace MyPhysics::Diffusion;

// Define a list of types to run the tests with
typedef ::testing::Types<float, double, long double> FloatTypes;
TYPED_TEST_SUITE(DiffusionMatrixTests, FloatTypes);

template <typename T>
class DiffusionMatrixTests : public ::testing::Test {};

TYPED_TEST(DiffusionMatrixTests, ConstructorTest) {
    Params<TypeParam> params;
    params.setA(1.0).setB(1.0).setM(10).setN(10).setDiffusionCoefficient(0.1).setMacroscopicRemovalCrossSection(0.2);
    Matrix<TypeParam> matrix(params);
    EXPECT_EQ(matrix.getParams().getA(), 1.0);
    EXPECT_EQ(matrix.getParams().getB(), 1.0);
    EXPECT_EQ(matrix.getParams().getM(), 10);
    EXPECT_EQ(matrix.getParams().getN(), 10);
}

TYPED_TEST(DiffusionMatrixTests, CopyConstructorTest) {
    Params<TypeParam> params;
    params.setA(1.0).setB(1.0).setM(10).setN(10).setDiffusionCoefficient(0.1).setMacroscopicRemovalCrossSection(0.2);
    Matrix<TypeParam> matrix1(params);
    Matrix<TypeParam> matrix2(matrix1);
    EXPECT_EQ(matrix2.getParams().getA(), 1.0);
    EXPECT_EQ(matrix2.getParams().getB(), 1.0);
    EXPECT_EQ(matrix2.getParams().getM(), 10);
    EXPECT_EQ(matrix2.getParams().getN(), 10);
}

TYPED_TEST(DiffusionMatrixTests, AssignmentOperatorTest) {
    Params<TypeParam> params;
    params.setA(1.0).setB(1.0).setM(10).setN(10).setDiffusionCoefficient(0.1).setMacroscopicRemovalCrossSection(0.2);
    Matrix<TypeParam> matrix1(params);
    Matrix<TypeParam> matrix2 = matrix1;
    EXPECT_EQ(matrix2.getParams().getA(), 1.0);
    EXPECT_EQ(matrix2.getParams().getB(), 1.0);
    EXPECT_EQ(matrix2.getParams().getM(), 10);
    EXPECT_EQ(matrix2.getParams().getN(), 10);
}

TYPED_TEST(DiffusionMatrixTests, MatrixDimensionsMatchMeshDimensionsTest) {
    Params<TypeParam> params;
    const size_t m = 3810;
    const size_t n = 1029;
    params.setA(1.0).setB(1.0).setM(m).setN(n).setDiffusionCoefficient(0.1).setMacroscopicRemovalCrossSection(0.2);
    Matrix<TypeParam> matrix(params);
    EXPECT_EQ(matrix.getRows(), params.getM() * params.getN());
    EXPECT_EQ(matrix.getCols(), params.getM() * params.getN());
}

template <typename TypeParam>
static MyBLAS::Matrix<TypeParam> naive_fill_diffusion_matrix_and_vector(const MyPhysics::Diffusion::Constants<TypeParam> &c) {
    const size_t m = c.m;
    const size_t n = c.n;

    auto diffusion_matrix_A = MyBLAS::Matrix<TypeParam>(std::vector<std::vector<TypeParam>>(m * n, std::vector<TypeParam>(m * n, 0)));
    // Loop through all the nodes i = 1, ..., m and j = 1, ..., n
    for (size_t i = 1; i <= m; ++i) {
        for (size_t j = 1; j <= n; ++j) {
            // Calculate the index of the current node in the matrix A and vector B
            int idx = (i - 1) * n + (j - 1);

            // Fill the diagonal element of the matrix A using the given equation
            diffusion_matrix_A[idx][idx] = c.diagonal;

            // Fill the off-diagonal elements of the matrix A using the given equation
            if (i > 1) {
                diffusion_matrix_A[idx][idx - n] = c.minus_D_over_delta_squared;
            }
            if (i < m) {
                diffusion_matrix_A[idx][idx + n] = c.minus_D_over_delta_squared;
            }
            if (j > 1) {
                diffusion_matrix_A[idx][idx - 1] = c.minus_D_over_gamma_squared;
            }
            if (j < n) {
                diffusion_matrix_A[idx][idx + 1] = c.minus_D_over_gamma_squared;
            }

            // Fill the right-hand-side vector B using the fixed source q(i, j)
           // intermediates.right_hand_side_vector_B[idx] = c.sources[i - 1][j - 1];
        }
    }
    return diffusion_matrix_A;
}

TYPED_TEST(DiffusionMatrixTests, DiffusionEquationSquareFillTest) {
    auto p = MyPhysics::Diffusion::Params<TypeParam>()
                      .setA(100)
                      .setB(100)
                      .setDiffusionCoefficient(0.033)
                      .setMacroscopicRemovalCrossSection(0.77)
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

TYPED_TEST(DiffusionMatrixTests, DiffusionEquationWideFillTest) {
    auto p = MyPhysics::Diffusion::Params<TypeParam>()
                 .setA(100)
                 .setB(100)
                 .setDiffusionCoefficient(0.033)
                 .setMacroscopicRemovalCrossSection(0.77)
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

TYPED_TEST(DiffusionMatrixTests, DiffusionEquationTallFillTest) {
    auto p = MyPhysics::Diffusion::Params<TypeParam>()
                 .setA(100)
                 .setB(100)
                 .setDiffusionCoefficient(0.033)
                 .setMacroscopicRemovalCrossSection(0.77)
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

TYPED_TEST(DiffusionMatrixTests, DiffusionMatrix32Test) {
    auto p = MyPhysics::Diffusion::Params<TypeParam>()
                 .setA(100)
                 .setB(100)
                 .setDiffusionCoefficient(0.033)
                 .setMacroscopicRemovalCrossSection(0.77)
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

TYPED_TEST(DiffusionMatrixTests, MatrixEqualityTest) {
    Params<TypeParam> params;
    params.setA(1.0)
        .setB(1.0)
        .setM(9)
        .setN(11)
        .setDiffusionCoefficient(0.1)
        .setMacroscopicRemovalCrossSection(0.2);
    Matrix<TypeParam> matrix1(params);
    Matrix<TypeParam> matrix2(params);
    // Check that the two matrices are equal
    for (size_t i = 0; i < matrix1.getRows(); ++i) {
        for (size_t j = 0; j < matrix1.getCols(); ++j) {
            EXPECT_EQ(matrix1[i][j], matrix2[i][j]);
        }
    }
}

TYPED_TEST(DiffusionMatrixTests, MatrixInequalityTest) {
    Params<TypeParam> p1;
    p1.setA(1.0)
    .setB(1.0)
    .setM(9)
    .setN(11)
    .setDiffusionCoefficient(0.1)
    .setMacroscopicRemovalCrossSection(0.2);
    Matrix<TypeParam> matrix1(p1);
    Params<TypeParam> p2;
    p2.setA(1.0)
    .setB(1.0)
    .setM(11)
    .setN(9)
    .setDiffusionCoefficient(0.002381)
    .setMacroscopicRemovalCrossSection(1e-2);
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

TYPED_TEST(DiffusionMatrixTests, MatrixSizeTest) {
    Params<TypeParam> params;
    Params<TypeParam> p;
    p.setA(1.0)
    .setB(1.0)
    .setM(11)
    .setN(11)
    .setDiffusionCoefficient(0.1)
    .setMacroscopicRemovalCrossSection(0.2);
    Matrix<TypeParam> matrix(params);
    // Check that the size of the matrix is correct
    EXPECT_EQ(matrix.getRows(), params.getM() * params.getN());
    EXPECT_EQ(matrix.getCols(), params.getM() * params.getN());
}

TYPED_TEST(DiffusionMatrixTests, DiffusionEquationBoundaryCheck) {
    Params<TypeParam> p;
    p.setA(1.0)
    .setB(1.0)
    .setM(5)
    .setN(7)
    .setDiffusionCoefficient(0.1)
    .setMacroscopicRemovalCrossSection(0.2);
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

TYPED_TEST(DiffusionMatrixTests, DiffusionEquationGeneratorTest) {
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

TYPED_TEST(DiffusionMatrixTests, DiagonalElementTest) {
    Params<TypeParam> p;
    p.setA(1.0)
    .setB(1.0)
    .setM(20)
    .setN(20)
    .setDiffusionCoefficient(0.1)
    .setMacroscopicRemovalCrossSection(0.2);
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

TYPED_TEST(DiffusionMatrixTests, OffDiagonalElementTest) {
    Params<TypeParam> params;
    params.setA(1.0)
    .setB(1.0)
    .setM(10)
    .setN(10)
    .setDiffusionCoefficient(0.1)
    .setMacroscopicRemovalCrossSection(0.2);
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
 * In the diffusion matrix, the non-zero elements are on the diagonal (i == j), and on the positions where `i` and `j`
 * differ by `1` or `n` (the number of columns). This is because in the diffusion matrix, each node is connected to its
 * immediate neighbors (left, right, up, down) in the grid.
 */
TYPED_TEST(DiffusionMatrixTests, ZeroElementTest) {
    Params<TypeParam> params;
    params.setA(1.0)
    .setB(1.0)
    .setM(7)
    .setN(11)
    .setDiffusionCoefficient(0.1)
    .setMacroscopicRemovalCrossSection(0.2);
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

TYPED_TEST(DiffusionMatrixTests, MatrixSymmetryTest) {
    Params<TypeParam> p;
    p.setA(1.0)
    .setB(1.0)
    .setM(8)
    .setN(3)
    .setDiffusionCoefficient(0.1)
    .setMacroscopicRemovalCrossSection(0.2);
    Matrix<TypeParam> matrix(p);
    auto memoryBacked = naive_fill_diffusion_matrix_and_vector(MyPhysics::Diffusion::Constants<TypeParam>::compute(p));
    for (size_t i = 0; i < matrix.getRows(); ++i) {
        for (size_t j = 0; j < matrix.getCols(); ++j) {
            EXPECT_EQ(matrix[i][j], memoryBacked[i][j]);
            EXPECT_EQ(matrix[i][j], matrix[j][i]);
        }
    }
}

TYPED_TEST(DiffusionMatrixTests, SquareMatrixSymmetryTimingTest) {
    Params<TypeParam> p;
    p.setA(1.0)
    .setB(1.0)
    .setM(91)
    .setN(91)
    .setDiffusionCoefficient(0.1)
    .setMacroscopicRemovalCrossSection(0.2);
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

TYPED_TEST(DiffusionMatrixTests, WideMatrixSymmetryTimingTest) {
    Params<TypeParam> p;
    p.setA(1.0)
    .setB(1.0)
    .setM(8)
    .setN(1024)
    .setDiffusionCoefficient(0.1)
    .setMacroscopicRemovalCrossSection(0.2);
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

TYPED_TEST(DiffusionMatrixTests, TallMatrixSymmetryTimingTest) {
    Params<TypeParam> p;
    p.setA(1.0)
    .setB(1.0)
    .setM(1024)
    .setN(8)
    .setDiffusionCoefficient(0.1)
    .setMacroscopicRemovalCrossSection(0.2);
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

// Define a list of types to run the tests with
typedef ::testing::Types<short, int, float, double, long double> NumericTypes;
TYPED_TEST_SUITE(MatrixVectorTests, NumericTypes);

template <typename T>
class MatrixVectorTests : public ::testing::Test {};

TYPED_TEST(MatrixVectorTests, MatrixVectorMultiplicationTest1) {
    Params<TypeParam> p;
    p.setA(2)
    .setB(2)
    .setM(3)
    .setN(3)
    .setDiffusionCoefficient(1)
    .setMacroscopicRemovalCrossSection(2);
    Matrix<TypeParam> m1(p);
    MyBLAS::Vector<TypeParam> v1(9, 1);
    auto result = m1 * v1;
    EXPECT_EQ(result.size(), 9);
    // Add more assertions based on the expected result of the multiplication
}

TYPED_TEST(MatrixVectorTests, MatrixVectorMultiplicationTest2) {
    Params<TypeParam> p;
    p.setA(3)
        .setB(3)
        .setM(2)
        .setN(2)
        .setDiffusionCoefficient(1)
        .setMacroscopicRemovalCrossSection(2);
    Matrix<TypeParam> m1(p);
    MyBLAS::Vector<TypeParam> v1(4, 2);
    auto result = m1 * v1;
    EXPECT_EQ(result.size(), 4);
    // Add more assertions based on the expected result of the multiplication
}

TYPED_TEST(MatrixVectorTests, MatrixVectorMultiplicationTest3) {
    Params<TypeParam> p;
    p.setA(3)
        .setB(3)
        .setM(4)
        .setN(4)
        .setDiffusionCoefficient(1)
        .setMacroscopicRemovalCrossSection(2);
    Matrix<TypeParam> m1(p);
    MyBLAS::Vector<TypeParam> v1(16, 3);
    auto result = m1 * v1;
    EXPECT_EQ(result.size(), 16);
    // Add more assertions based on the expected result of the multiplication
}
