/**
 * @file DiffusionParamsTests.cpp
 * @author Arjun Earthperson
 * @date 10/13/2023
 *
 * @brief The purpose of these tests is to ensure the correct functionality of the
 * DiffusionParams class, which is part of a physics simulation library. The DiffusionParams
 * class is responsible for storing and manipulating the parameters related to the diffusion
 * process in a physical system. The tests are designed to verify the correctness of the
 * class's methods, including constructors, setters, getters, and the output operator.
 * They also check the class's ability to handle invalid and edge-case inputs, such as
 * zero, negative, and non-finite values.
 *
 * The tests cover a wide range of scenarios to ensure comprehensive code coverage.
 * The tests for the default and parameterized constructors verify that the class correctly
 * initializes its member variables. The tests for the setters and getters ensure that
 * these methods correctly modify and retrieve the class's member variables. The tests for
 * invalid, negative, and non-finite inputs check that the class correctly throws exceptions
 * when given these types of inputs. The test for the output operator verifies that this
 * operator correctly formats a string representation of a DiffusionParams object. The
 * parameterized tests, instantiated with a variety of valid inputs, ensure that the class
 * behaves correctly under different conditions. Overall, these tests provide a thorough
 * examination of the DiffusionParams class's functionality.
 */

#include "physics/diffusion/DiffusionParams.h"
#include <gtest/gtest.h>

#define TOLERANCE 1e-14
#define FLOAT_TYPE long double

/**
 * @class DiffusionParamsValidTest
 * @brief A test fixture class for valid DiffusionParams.
 */
class DiffusionParamsValidTest : public ::testing::TestWithParam<MyPhysics::Diffusion::Params<FLOAT_TYPE>> {
  protected:
    MyPhysics::Diffusion::Params<FLOAT_TYPE> params{};
};

MyBLAS::Matrix<FLOAT_TYPE> _sources = {{1.0, 2.0, 3.0, 4.0, 5.0},
                                       {2.0, 3.0, 4.0, 5.0, 1.0},
                                       {3.0, 4.0, 5.0, 1.0, 2.0},
                                       {4.0, 5.0, 1.0, 2.0, 3.0},
                                       {5.0, 1.0, 2.0, 3.0, 4.0}};

/**
 * @brief Test for the parameterized constructor of DiffusionParams.
 */
TEST_P(DiffusionParamsValidTest, ParameterizedConstructor) {
    params = GetParam();
    EXPECT_GT(params.getA(), 0);
    EXPECT_GT(params.getB(), 0);
    EXPECT_GT(params.getM(), 0);
    EXPECT_GT(params.getN(), 0);
    EXPECT_GT(params.getDelta(), 0);
    EXPECT_GT(params.getGamma(), 0);
    EXPECT_GT(params.getDiffusionCoefficient(), 0);
    EXPECT_GT(params.getMacroscopicRemovalCrossSection(), 0);
    EXPECT_EQ(params.getSources().getRows(), params.getM());
    EXPECT_EQ(params.getSources().getCols(), params.getN());
}

/**
 * @brief Test for the setters and getters of DiffusionParams.
 */
TEST_P(DiffusionParamsValidTest, SettersAndGetters) {
    params = GetParam();
    params.setA(2.0).setB(3.0).setM(2).setN(2).setDiffusionCoefficient(0.5).setMacroscopicRemovalCrossSection(0.6);
    EXPECT_EQ(params.getA(), 2.0);
    EXPECT_EQ(params.getB(), 3.0);
    EXPECT_EQ(params.getM(), 2);
    EXPECT_EQ(params.getN(), 2);
    EXPECT_NEAR(params.getDelta(), 2.0 / 3.0, TOLERANCE);
    EXPECT_NEAR(params.getGamma(), 3.0 / 3.0, TOLERANCE);
    EXPECT_EQ(params.getDiffusionCoefficient(), 0.5);
    EXPECT_EQ(params.getMacroscopicRemovalCrossSection(), 0.6);
}

/**
 * @brief Test for the output operator of DiffusionParams.
 */
TEST_P(DiffusionParamsValidTest, OutputOperator) {
    params = GetParam();
    std::ostringstream os;
    os << params;
    EXPECT_FALSE(os.str().empty());
}

/**
 * @brief Instantiate the test suite for valid DiffusionParams.
 */
INSTANTIATE_TEST_SUITE_P(
    InputValues, DiffusionParamsValidTest,
    ::testing::Values(MyPhysics::Diffusion::Params<FLOAT_TYPE>(100.4, 78.5, 5, 5, 1.3029, 1e-4, _sources),
                      MyPhysics::Diffusion::Params<FLOAT_TYPE>(100.4, 78.5, 5, 5, 1.3029, 1e-4, _sources)
                          )
);

/**
 * @class DiffusionParamsTest
 * @brief A test fixture class for DiffusionParams.
 */
class DiffusionParamsTest : public ::testing::Test {
  protected:
    MyPhysics::Diffusion::Params<FLOAT_TYPE> params{};
};

/**
 * @brief Test for the default constructor of DiffusionParams.
 */
TEST_F(DiffusionParamsTest, DefaultConstructor) {
    EXPECT_EQ(params.getA(), 1);
    EXPECT_EQ(params.getB(), 1);
    EXPECT_EQ(params.getM(), 1);
    EXPECT_EQ(params.getN(), 1);
    EXPECT_EQ(params.getDelta(), 1);
    EXPECT_EQ(params.getGamma(), 1);
    EXPECT_EQ(params.getDiffusionCoefficient(), 0.0);
    EXPECT_EQ(params.getMacroscopicRemovalCrossSection(), 0.0);
}

/**
 * @brief Test for the parameterized constructor of DiffusionParams.
 */
TEST_F(DiffusionParamsTest, ParameterizedConstructor) {
    MyBLAS::Matrix<FLOAT_TYPE> sources = {{1.0, 2.0}, {3.0, 4.0}};
    MyPhysics::Diffusion::Params<FLOAT_TYPE> params(2.0, 3.0, 2, 2, 0.5, 0.6, sources);
    EXPECT_EQ(params.getA(), 2.0);
    EXPECT_EQ(params.getB(), 3.0);
    EXPECT_EQ(params.getM(), 2);
    EXPECT_EQ(params.getN(), 2);
    EXPECT_NEAR(params.getDelta(), 2.0 / 3.0, TOLERANCE);
    EXPECT_NEAR(params.getGamma(), 3.0 / 3.0, TOLERANCE);
    EXPECT_EQ(params.getDiffusionCoefficient(), 0.5);
    EXPECT_EQ(params.getMacroscopicRemovalCrossSection(), 0.6);
    EXPECT_EQ(params.getSources(), sources);
}

/**
 * @brief Test for the setters and getters of DiffusionParams.
 */
TEST_F(DiffusionParamsTest, SettersAndGetters) {
    MyBLAS::Matrix<FLOAT_TYPE> sources = {{1.0, 2.0}, {3.0, 4.0}};
    params.setA(2.0).setB(3.0).setM(2).setN(2).setDiffusionCoefficient(0.5).setMacroscopicRemovalCrossSection(0.6).setSources(sources);
    EXPECT_EQ(params.getA(), 2.0);
    EXPECT_EQ(params.getB(), 3.0);
    EXPECT_EQ(params.getM(), 2);
    EXPECT_EQ(params.getN(), 2);
    EXPECT_NEAR(params.getDelta(), 2.0 / 3.0, TOLERANCE);
    EXPECT_NEAR(params.getGamma(), 3.0 / 3.0, TOLERANCE);
    EXPECT_EQ(params.getDiffusionCoefficient(), 0.5);
    EXPECT_EQ(params.getMacroscopicRemovalCrossSection(), 0.6);
    EXPECT_EQ(params.getSources(), sources);
}

/**
 * @brief Test for invalid inputs to DiffusionParams.
 */
TEST_F(DiffusionParamsTest, InvalidInputs) {
    EXPECT_THROW(params.setA(0), std::invalid_argument);
    EXPECT_THROW(params.setB(0), std::invalid_argument);
    EXPECT_THROW(params.setM(0), std::invalid_argument);
    EXPECT_THROW(params.setN(0), std::invalid_argument);
    EXPECT_THROW(params.setDiffusionCoefficient(0), std::invalid_argument);
    EXPECT_THROW(params.setMacroscopicRemovalCrossSection(0), std::invalid_argument);
    MyBLAS::Matrix<FLOAT_TYPE> sources = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
    EXPECT_THROW(params.setSources(sources), std::invalid_argument);
}

/**
 * @brief Test for negative inputs to DiffusionParams.
 */
TEST_F(DiffusionParamsTest, NegativeInputs) {
    EXPECT_THROW(params.setA(-1), std::invalid_argument);
    EXPECT_THROW(params.setB(-1), std::invalid_argument);
    EXPECT_THROW(params.setDiffusionCoefficient(-1), std::invalid_argument);
    EXPECT_THROW(params.setMacroscopicRemovalCrossSection(-1), std::invalid_argument);
}

/**
 * @brief Test for non-finite inputs to DiffusionParams.
 */
TEST_F(DiffusionParamsTest, NonFiniteInputs) {
    EXPECT_THROW(params.setA(std::numeric_limits<FLOAT_TYPE>::infinity()), std::invalid_argument);
    EXPECT_THROW(params.setB(std::numeric_limits<FLOAT_TYPE>::infinity()), std::invalid_argument);
    EXPECT_THROW(params.setDiffusionCoefficient(std::numeric_limits<FLOAT_TYPE>::infinity()), std::invalid_argument);
    EXPECT_THROW(params.setMacroscopicRemovalCrossSection(std::numeric_limits<FLOAT_TYPE>::infinity()), std::invalid_argument);
    EXPECT_THROW(params.setA(std::numeric_limits<FLOAT_TYPE>::quiet_NaN()), std::invalid_argument);
    EXPECT_THROW(params.setB(std::numeric_limits<FLOAT_TYPE>::quiet_NaN()), std::invalid_argument);
    EXPECT_THROW(params.setDiffusionCoefficient(std::numeric_limits<FLOAT_TYPE>::quiet_NaN()), std::invalid_argument);
    EXPECT_THROW(params.setMacroscopicRemovalCrossSection(std::numeric_limits<FLOAT_TYPE>::quiet_NaN()), std::invalid_argument);
}

/**
 * @brief Test for the output operator of DiffusionParams.
 */
TEST_F(DiffusionParamsTest, OutputOperator) {
    MyBLAS::Matrix<FLOAT_TYPE> sources = {{1.0, 2.0}, {3.0, 4.0}};
    params.setA(2.0).setB(3.0).setM(2).setN(2).setDiffusionCoefficient(0.5).setMacroscopicRemovalCrossSection(0.6).setSources(sources);
    std::ostringstream os;
    os << params;
    EXPECT_EQ(os.str(), "a: 2 b: 3 m: 2 n: 2 delta: 0.666667 gamma: 1 diff_coeff: 0.5 cross_section: 0.6");
}
