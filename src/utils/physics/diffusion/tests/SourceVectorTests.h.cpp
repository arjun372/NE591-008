/**
* @file SourceVectorTests.cpp
* @author Arjun Earthperson
* @date 10/13/2023
*
* //TODO:: DOCUMENT
*/

#include "physics/diffusion/SourceVector.h"
#include <gtest/gtest.h>

#define TOLERANCE 1e-14
#define FLOAT_TYPE long double

/**
* @class SourceVectorValidTest
* @brief A test fixture class for valid SourceVector.
*/
class SourceVectorValidTest : public ::testing::TestWithParam<MyPhysics::Diffusion::Constants<FLOAT_TYPE>> {
 protected:
   MyPhysics::Diffusion::Params<FLOAT_TYPE> params{};
};
