#include "math/relaxation/SORPJ.h"
#include <gtest/gtest.h>

#define TOLERANCE 1e-14
#define FLOAT_TYPE long double

namespace MyRelaxationMethods {

// Define a list of types to run the tests with
typedef ::testing::Types<float, double, long double> NumericTypes;
TYPED_TEST_SUITE(PointJacobiTests, NumericTypes);

template <typename T> class PointJacobiTests : public ::testing::Test {
  protected:
};

}
