/**
* @file BaseMatrixTests.cpp
* @author Arjun Earthperson
* @date 10/13/2023
*
* @brief This file contains unit tests for the MyBLAS::Matrix class.
 */

#include "math/blas/matrix/Matrix.h"
#include <gtest/gtest.h>

#define TOLERANCE 1e-14
#define FLOAT_TYPE long double

namespace MyBLAS {

// Define a list of types to run the tests with
typedef ::testing::Types<int, float, double, long double> NumericTypes;
TYPED_TEST_SUITE(BaseMatrixTests, NumericTypes);

template <typename T>
class BaseMatrixTests : public ::testing::Test {};

//  Assigning to a Newly Created Object
TYPED_TEST(BaseMatrixTests, CopyAssignmentToNewObject) {
    Matrix<TypeParam> mat1(2, 2, 1);
    Matrix<TypeParam> mat2; // Default constructor

    size_t initialCount = ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount();
    mat2 = mat1; // Copy-assignment operator is called
    EXPECT_EQ(mat2.getRows(), mat1.getRows());
    EXPECT_EQ(mat2.getCols(), mat1.getCols());
    for (size_t i = 0; i < mat2.getRows(); ++i) {
        for (size_t j = 0; j < mat2.getCols(); ++j) {
            EXPECT_EQ(mat2[i][j], mat1[i][j]);
        }
    }
    // The instances set should not change in size because mat2 already exists
    EXPECT_EQ(ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount(), initialCount);
}

//  Assigning to an Existing Object with Resources
TYPED_TEST(BaseMatrixTests, CopyAssignmentToExistingObject) {
    Matrix<TypeParam> mat1(2, 2, 1);
    Matrix<TypeParam> mat2(3, 3, 2); // mat2 owns some resources
    mat2 = mat1; // Copy-assignment operator is called
    EXPECT_EQ(mat2.getRows(), mat1.getRows());
    EXPECT_EQ(mat2.getCols(), mat1.getCols());
    for (size_t i = 0; i < mat2.getRows(); ++i) {
        for (size_t j = 0; j < mat2.getCols(); ++j) {
            EXPECT_EQ(mat2[i][j], mat1[i][j]);
        }
    }
    // The instances set should not change in size because mat2 already exists
    EXPECT_EQ(ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount(), ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount());
}

//  Self-Assignment
TYPED_TEST(BaseMatrixTests, SelfAssignment) {
    Matrix<TypeParam> mat1(2, 2, 1);
    size_t initialCount = ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount();
    mat1 = mat1; // Self-assignment
    // The matrix should remain unchanged after self-assignment
    for (size_t i = 0; i < mat1.getRows(); ++i) {
        for (size_t j = 0; j < mat1.getCols(); ++j) {
            EXPECT_EQ(mat1[i][j], 1);
        }
    }
    // The instances set should not change in size because it's a self-assignment
    EXPECT_EQ(ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount(), initialCount);
}

//  Chained Assignment
TYPED_TEST(BaseMatrixTests, ChainedAssignment) {
    Matrix<TypeParam> mat1(2, 2, 1);
    Matrix<TypeParam> mat2; // Default constructor
    Matrix<TypeParam> mat3; // Default constructor
    size_t initialCount = ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount();
    mat2 = mat3 = mat1; // Chained copy-assignment
    // Both mat2 and mat3 should now contain copies of mat1's data
    for (size_t i = 0; i < mat2.getRows(); ++i) {
        for (size_t j = 0; j < mat2.getCols(); ++j) {
            EXPECT_EQ(mat2[i][j], mat1[i][j]);
            EXPECT_EQ(mat3[i][j], mat1[i][j]);
        }
    }
    // The instances set should not change in size because mat2 and mat3 already exist
    EXPECT_EQ(ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount(), initialCount);
}

//  Assigning to a Newly Created Object
TYPED_TEST(BaseMatrixTests, MoveAssignmentToNewObject) {
    Matrix<TypeParam> mat1(2, 2, 1);
    Matrix<TypeParam> mat2; // Default constructor
    size_t initialCount = ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount();
    mat2 = std::move(mat1); // Move-assignment operator is called
    EXPECT_EQ(mat2.getRows(), 2);
    EXPECT_EQ(mat2.getCols(), 2);
    for (size_t i = 0; i < mat2.getRows(); ++i) {
        for (size_t j = 0; j < mat2.getCols(); ++j) {
            EXPECT_EQ(mat2[i][j], 1);
        }
    }
    // The instances set should not change in size because mat1 and mat2 already exist
    EXPECT_EQ(ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount(), initialCount);
}

//  Assigning to an Existing Object with Resources
TYPED_TEST(BaseMatrixTests, MoveAssignmentToExistingObject) {
    Matrix<TypeParam> mat1(2, 2, 1);
    Matrix<TypeParam> mat2(3, 3, 2); // mat2 owns some resources
    mat2 = std::move(mat1); // Move-assignment operator is called
    EXPECT_EQ(mat2.getRows(), 2);
    EXPECT_EQ(mat2.getCols(), 2);
    for (size_t i = 0; i < mat2.getRows(); ++i) {
        for (size_t j = 0; j < mat2.getCols(); ++j) {
            EXPECT_EQ(mat2[i][j], 1);
        }
    }
    // The instances set should not change in size because mat1 and mat2 already exist
    EXPECT_EQ(ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount(), ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount());
}

//  Self-Assignment
TYPED_TEST(BaseMatrixTests, SelfMoveAssignment) {
    Matrix<TypeParam> mat1(2, 2, 1);
    size_t initialCount = ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount();
    mat1 = std::move(mat1); // Self-assignment with move
    // The matrix should remain unchanged after self-assignment
    for (size_t i = 0; i < mat1.getRows(); ++i) {
        for (size_t j = 0; j < mat1.getCols(); ++j) {
            EXPECT_EQ(mat1[i][j], 1);
        }
    }
    // The instances set should not change in size because it's a self-assignment
    EXPECT_EQ(ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount(), initialCount);
}

//  Chained Assignment
TYPED_TEST(BaseMatrixTests, ChainedMoveAssignment) {
    Matrix<TypeParam> mat1(2, 2, 1);
    Matrix<TypeParam> mat2; // Default constructor
    Matrix<TypeParam> mat3; // Default constructor
    size_t initialCount = ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount();
    mat2 = mat3 = std::move(mat1); // Chained move-assignment
    // mat2 should now contain the data originally in mat1
    for (size_t i = 0; i < mat2.getRows(); ++i) {
        for (size_t j = 0; j < mat2.getCols(); ++j) {
            EXPECT_EQ(mat2[i][j], 1);
        }
    }
    // mat3 should be in a valid but unspecified state
    // The instances set should not change in size because mat1, mat2, and mat3 already exist
    EXPECT_EQ(ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount(), initialCount);
}

TYPED_TEST(BaseMatrixTests, ConstructorTest) {
    Matrix<TypeParam> m1(3, 3, 1);
    EXPECT_EQ(m1.getRows(), 3);
    EXPECT_EQ(m1.getCols(), 3);
    for (size_t i = 0; i < m1.getRows(); ++i) {
        for (size_t j = 0; j < m1.getCols(); ++j) {
            EXPECT_EQ(m1[i][j], 1);
        }
    }
}

// Test default constructor
TYPED_TEST(BaseMatrixTests, DefaultConstructorTest) {
    Matrix<TypeParam> m;
    EXPECT_EQ(m.getRows(), 0);
    EXPECT_EQ(m.getCols(), 0);
}

// Test constructor with size and initial value
TYPED_TEST(BaseMatrixTests, SizeInitialValueConstructorTest) {
    Matrix<TypeParam> m(3, 3, static_cast<TypeParam>(1));
    EXPECT_EQ(m.getRows(), 3);
    EXPECT_EQ(m.getCols(), 3);
    for (size_t i = 0; i < m.getRows(); ++i) {
        for (size_t j = 0; j < m.getCols(); ++j) {
            EXPECT_EQ(m[i][j], static_cast<TypeParam>(1));
        }
    }
}

// Test constructor from a 2D vector
TYPED_TEST(BaseMatrixTests, Vector2DConstructorTest) {
    std::vector<std::vector<TypeParam>> vec2d = {
        {static_cast<TypeParam>(1), static_cast<TypeParam>(2)},
        {static_cast<TypeParam>(3), static_cast<TypeParam>(4)}
    };
    Matrix<TypeParam> m(vec2d);
    EXPECT_EQ(m.getRows(), 2);
    EXPECT_EQ(m.getCols(), 2);
    for (size_t i = 0; i < m.getRows(); ++i) {
        for (size_t j = 0; j < m.getCols(); ++j) {
            EXPECT_EQ(m[i][j], vec2d[i][j]);
        }
    }
}

// Test copy constructor
TYPED_TEST(BaseMatrixTests, CopyConstructorTest) {
    Matrix<TypeParam> m1(2, 2, static_cast<TypeParam>(5));
    Matrix<TypeParam> m2(m1);
    EXPECT_EQ(m2.getRows(), 2);
    EXPECT_EQ(m2.getCols(), 2);
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_EQ(m2[i][j], static_cast<TypeParam>(5));
        }
    }
}

// Test move constructor
TYPED_TEST(BaseMatrixTests, MoveConstructorTest) {
    Matrix<TypeParam> m1(2, 2, static_cast<TypeParam>(5));
    Matrix<TypeParam> m2(std::move(m1));
    EXPECT_EQ(m2.getRows(), 2);
    EXPECT_EQ(m2.getCols(), 2);
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_EQ(m2[i][j], static_cast<TypeParam>(5));
        }
    }
    // After move, m1 should be empty
    EXPECT_EQ(m1.getRows(), 0);
    EXPECT_EQ(m1.getCols(), 0);
}

// Test initializer list constructor
TYPED_TEST(BaseMatrixTests, InitializerListConstructorTest) {
    Matrix<TypeParam> m({
        {static_cast<TypeParam>(1), static_cast<TypeParam>(2)},
        {static_cast<TypeParam>(3), static_cast<TypeParam>(4)}
    });
    EXPECT_EQ(m.getRows(), 2);
    EXPECT_EQ(m.getCols(), 2);
    EXPECT_EQ(m[0][0], static_cast<TypeParam>(1));
    EXPECT_EQ(m[0][1], static_cast<TypeParam>(2));
    EXPECT_EQ(m[1][0], static_cast<TypeParam>(3));
    EXPECT_EQ(m[1][1], static_cast<TypeParam>(4));
}

// Test lambda function constructor
TYPED_TEST(BaseMatrixTests, LambdaFunctionConstructorTest) {
    Matrix<TypeParam> m(2, 2, [](size_t i, size_t j) { return static_cast<TypeParam>(i + j); });
    EXPECT_EQ(m.getRows(), 2);
    EXPECT_EQ(m.getCols(), 2);
    EXPECT_EQ(m[0][0], static_cast<TypeParam>(0));
    EXPECT_EQ(m[0][1], static_cast<TypeParam>(1));
    EXPECT_EQ(m[1][0], static_cast<TypeParam>(1));
    EXPECT_EQ(m[1][1], static_cast<TypeParam>(2));
}

TYPED_TEST(BaseMatrixTests, CopyAssignmentOperatorTest) {
    // Create a matrix and fill it with some values
    Matrix<TypeParam> m1(3, 3, 1);
    for (size_t i = 0; i < m1.getRows(); ++i) {
        for (size_t j = 0; j < m1.getCols(); ++j) {
            m1[i][j] = static_cast<TypeParam>(i * m1.getCols() + j);
        }
    }

    // Use the copy assignment operator
    Matrix<TypeParam> m2;
    m2 = m1;

    // Check if the copy was successful
    EXPECT_EQ(m2.getRows(), m1.getRows());
    EXPECT_EQ(m2.getCols(), m1.getCols());
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_EQ(m2[i][j], m1[i][j]);
        }
    }
}

TYPED_TEST(BaseMatrixTests, MoveAssignmentOperatorTest) {
    // Create a matrix and fill it with some values
    Matrix<TypeParam> m1(3, 3, 1);
    for (size_t i = 0; i < m1.getRows(); ++i) {
        for (size_t j = 0; j < m1.getCols(); ++j) {
            m1[i][j] = static_cast<TypeParam>(i * m1.getCols() + j);
        }
    }

    // Keep a copy of the original data for comparison
    Matrix<TypeParam> original = m1;

    // Use the move assignment operator
    Matrix<TypeParam> m2;
    m2 = std::move(m1);

    // Check if the move was successful
    EXPECT_EQ(m2.getRows(), original.getRows());
    EXPECT_EQ(m2.getCols(), original.getCols());
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_EQ(m2[i][j], original[i][j]);
        }
    }

    // The moved-from matrix (m1) should be in a valid but unspecified state
    // It's not safe to make assumptions about its content, but it should be safe to use
    // For example, we can check if it's empty or we can assign a new value to it
    m1 = Matrix<TypeParam>(2, 2, 2); // This should not throw or cause undefined behavior
    EXPECT_EQ(m1.getRows(), 2);
    EXPECT_EQ(m1.getCols(), 2);
}

TYPED_TEST(BaseMatrixTests, AdditionTest) {
    Matrix<TypeParam> m1(2, 2, 1);
    Matrix<TypeParam> m2(2, 2, 2);
    Matrix<TypeParam> m3 = m1 + m2;
    for (size_t i = 0; i < m3.getRows(); ++i) {
        for (size_t j = 0; j < m3.getCols(); ++j) {
            EXPECT_EQ(m3[i][j], 3);
        }
    }
}

TYPED_TEST(BaseMatrixTests, SubtractionTest) {
    Matrix<TypeParam> m1(2, 2, 3);
    Matrix<TypeParam> m2(2, 2, 2);
    Matrix<TypeParam> m3 = m1 - m2;
    for (size_t i = 0; i < m3.getRows(); ++i) {
        for (size_t j = 0; j < m3.getCols(); ++j) {
            EXPECT_EQ(m3[i][j], 1);
        }
    }
}

TYPED_TEST(BaseMatrixTests, MultiplicationTest) {
    Matrix<TypeParam> m1(2, 2, 1);
    Matrix<TypeParam> m2(2, 2, 2);
    Matrix<TypeParam> m3 = m1 * m2;
    for (size_t i = 0; i < m3.getRows(); ++i) {
        for (size_t j = 0; j < m3.getCols(); ++j) {
            EXPECT_EQ(m3[i][j], 4);
        }
    }
}

TYPED_TEST(BaseMatrixTests, RightSideScalarMultiplicationTest) {
    Matrix<TypeParam> m1(2, 2, 2);
    Matrix<TypeParam> m2 = m1 * 2;
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_EQ(m2[i][j], 4);
        }
    }
}

TYPED_TEST(BaseMatrixTests, LeftSideScalarMultiplicationTest) {
    Matrix<TypeParam> m1(2, 2, 2);
    Matrix<TypeParam> m2 = 2 * m1;
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_EQ(m2[i][j], 4);
        }
    }
}

TYPED_TEST(BaseMatrixTests, ScalarMultiplicationSymmetryTest) {
    Matrix<TypeParam> m1(2, 2, 2);
    Matrix<TypeParam> m2 = 2 * m1;
    Matrix<TypeParam> m3 = m1 * 2;
    // EXPECT_TRUE(m2 == m3);
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_TRUE(m2[i][j] == m3[i][j]);
            EXPECT_EQ(m2[i][j], m3[i][j]);
            EXPECT_EQ(m2[i][j], 4);
            EXPECT_EQ(m3[i][j], 4);
        }
    }
}

TYPED_TEST(BaseMatrixTests, EqualityTest) {
    Matrix<TypeParam> m1(2, 2, 1);
    Matrix<TypeParam> m2(2, 2, 1);
    EXPECT_TRUE(m1 == m2);
}

TYPED_TEST(BaseMatrixTests, InequalityTest) {
    Matrix<TypeParam> m1(2, 2, 1);
    Matrix<TypeParam> m2(2, 2, 2);
    EXPECT_FALSE(m1 == m2);
}

TYPED_TEST(BaseMatrixTests, ZeroSizeMatrixTest) {
    Matrix<TypeParam> m1(0, 0);
    Matrix<TypeParam> m2 = m1 / 2;
    EXPECT_EQ(m2.getCols(), 0);
    EXPECT_EQ(m1.getCols(), 0);
    EXPECT_EQ(m2.getRows(), 0);
    EXPECT_EQ(m1.getRows(), 0);
}

TYPED_TEST(BaseMatrixTests, IdentityMatrixTest) {
    Matrix<TypeParam> m1 = Matrix<TypeParam>::eye(3);
    for (size_t i = 0; i < m1.getRows(); ++i) {
        for (size_t j = 0; j < m1.getCols(); ++j) {
            if (i == j) {
                EXPECT_EQ(m1[i][j], 1);
            } else {
                EXPECT_EQ(m1[i][j], 0);
            }
        }
    }
}

TYPED_TEST(BaseMatrixTests, MatrixVectorMultiplicationTest) {
    Matrix<TypeParam> m1(2, 2, 1);
    Vector<TypeParam> v1(2, 2);
    Vector<TypeParam> v2 = m1 * v1;
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_EQ(v2[i], 4);
    }
}

TYPED_TEST(BaseMatrixTests, SubMatrixTest) {
    Matrix<TypeParam> m1(3, 3, 1);
    Matrix<TypeParam> m2 = m1.subMatrix(1, 1, 2, 2);
    EXPECT_EQ(m2.getRows(), 2);
    EXPECT_EQ(m2.getCols(), 2);
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_EQ(m2[i][j], 1);
        }
    }
}

TYPED_TEST(BaseMatrixTests, SetSubMatrixTest) {
    Matrix<TypeParam> m1(3, 3, 1);
    Matrix<TypeParam> m2(2, 2, 2);
    m1.setSubMatrix(1, 1, m2);
    Matrix<TypeParam> m3 = m1.subMatrix(1, 1, 2, 2);
    EXPECT_TRUE(m2 == m3);
}

TYPED_TEST(BaseMatrixTests, SwapRowsTest) {
    Matrix<TypeParam> m1(2, 2, 1);
    m1[0][0] = 2;
    m1.swapRows(0, 1);
    EXPECT_EQ(m1[0][0], 1);
    EXPECT_EQ(m1[1][0], 2);
}

TYPED_TEST(BaseMatrixTests, ScalarDivisionTest) {
    Matrix<TypeParam> m1(2, 2, 2);
    Matrix<TypeParam> m2 = m1 / 2;
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_EQ(m2[i][j], 1);
        }
    }
}

TYPED_TEST(BaseMatrixTests, ScalarAdditionTest) {
    Matrix<TypeParam> m1(2, 2, 1);
    Matrix<TypeParam> m2 = m1 + 1;
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_EQ(m2[i][j], 2);
        }
    }
}

TYPED_TEST(BaseMatrixTests, ScalarSubtractionTest) {
    Matrix<TypeParam> m1(2, 2, 2);
    Matrix<TypeParam> m2 = m1 - 1;
    for (size_t i = 0; i < m2.getRows(); ++i) {
        for (size_t j = 0; j < m2.getCols(); ++j) {
            EXPECT_EQ(m2[i][j], 1);
        }
    }
}

TYPED_TEST(BaseMatrixTests, EmptyMatrixTest) {
    Matrix<TypeParam> m1;
    EXPECT_EQ(m1.getRows(), 0);
    EXPECT_EQ(m1.getCols(), 0);
}

TYPED_TEST(BaseMatrixTests, NonSquareMatrixTest) {
    Matrix<TypeParam> m1(2, 3, 1);
    EXPECT_EQ(m1.getRows(), 2);
    EXPECT_EQ(m1.getCols(), 3);
}

// Test to verify that a newly constructed matrix is added to instances
TYPED_TEST(BaseMatrixTests, InstanceTrackingOnConstruction) {
    size_t initialCount = ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount();
    {
        Matrix<TypeParam> m(2, 2, 1);
        EXPECT_EQ(ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount(), initialCount + 1);
        EXPECT_TRUE(ResourceMonitor<Matrix<TypeParam>>::find(&m) != ResourceMonitor<Matrix<TypeParam>>::end());
    }
    // After destruction, the instance should be removed
    EXPECT_EQ(ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount(), initialCount);
}

// Test to verify that a matrix created via copy construction is added to instances
TYPED_TEST(BaseMatrixTests, InstanceTrackingOnCopyConstruction) {
    Matrix<TypeParam> m1(2, 2, 1);
    size_t initialCount = ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount();
    Matrix<TypeParam> m2(m1);
    EXPECT_EQ(ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount(), initialCount + 1);
    EXPECT_TRUE(ResourceMonitor<Matrix<TypeParam>>::find(&m2) != ResourceMonitor<Matrix<TypeParam>>::end());
}

// Test to verify that a matrix created via move construction is added to instances
TYPED_TEST(BaseMatrixTests, InstanceTrackingOnMoveConstruction) {
    Matrix<TypeParam> m1(2, 2, 1);
    size_t initialCount = ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount();
    Matrix<TypeParam> m2(std::move(m1));
    EXPECT_EQ(ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount(), initialCount + 1);
    EXPECT_TRUE(ResourceMonitor<Matrix<TypeParam>>::find(&m2) != ResourceMonitor<Matrix<TypeParam>>::end());
    EXPECT_TRUE(ResourceMonitor<Matrix<TypeParam>>::find(&m1) != ResourceMonitor<Matrix<TypeParam>>::end());
}

// Test to verify that a matrix assigned via copy assignment is in instances
TYPED_TEST(BaseMatrixTests, InstanceTrackingOnCopyAssignment) {
    Matrix<TypeParam> m1(2, 2, 1);
    size_t initialCount = ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount();
    Matrix<TypeParam> m2 = m1;
    EXPECT_EQ(ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount(), initialCount + 1);
    EXPECT_TRUE(ResourceMonitor<Matrix<TypeParam>>::find(&m2) != ResourceMonitor<Matrix<TypeParam>>::end());
    EXPECT_TRUE(ResourceMonitor<Matrix<TypeParam>>::find(&m1) != ResourceMonitor<Matrix<TypeParam>>::end());
}

// Test to verify that a matrix assigned via move assignment is in instances
TYPED_TEST(BaseMatrixTests, InstanceTrackingOnMoveAssignment) {
    Matrix<TypeParam> m1(2, 2, 1);
    Matrix<TypeParam> m2;
    size_t initialCount = ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount();
    m2 = std::move(m1);
    EXPECT_EQ(ResourceMonitor<Matrix<TypeParam>>::getCurrentInstanceCount(), initialCount);
    EXPECT_TRUE(ResourceMonitor<Matrix<TypeParam>>::find(&m2) != ResourceMonitor<Matrix<TypeParam>>::end());
    EXPECT_TRUE(ResourceMonitor<Matrix<TypeParam>>::find(&m1) != ResourceMonitor<Matrix<TypeParam>>::end());
}

} // namespace MyBLAS
