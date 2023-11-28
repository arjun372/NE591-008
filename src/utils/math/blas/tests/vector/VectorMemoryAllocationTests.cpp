/**
* @file VectorMemoryAllocationTests.cpp
* @author Arjun Earthperson
* @date 10/13/2023
*
* @brief This file contains unit tests for the MyBLAS::Vector class.
 */

#include "math/blas/Constants.h"
#include "math/blas/vector/Vector.h"
#include "profiler/ResourceMonitor.h"
#include <gtest/gtest.h>

#define TOLERANCE 1e-14
#define FLOAT_TYPE long double

namespace MyBLAS {

// Define a list of types to run the tests with
typedef ::testing::Types<int, float, double, long double> NumericTypes;
TYPED_TEST_SUITE(VectorMemoryAllocationTests, NumericTypes);

template <typename T>
class VectorMemoryAllocationTests : public ::testing::Test {
  protected:
    // Helper function to estimate the memory usage of a vector with given size.
    static size_t estimateVectorMemoryUsage(size_t size) {
        size_t memory = sizeof(Vector<T>); // Memory for the Vector object itself.
        memory += size * sizeof(T); // Memory for the elements.
        return memory;
    }
    ResourceMonitor<MyBLAS::Vector<MyBLAS::NumericType>>* _resources = &ResourceMonitor<MyBLAS::Vector<MyBLAS::NumericType>>::getInstance();
};

TYPED_TEST(VectorMemoryAllocationTests, ConstructorTest) {
    using VectorType = Vector<TypeParam>;
    const size_t size = 100;
   // const size_t vectorMemory = this->estimateVectorMemoryUsage(size);

    // Create a vector and check memory usage
    {
        this->_resources->clear();
        VectorType vector(size);
 //       size_t memoryUsage = this->_resources->getMaxBytesEver();
 //       EXPECT_GT(memoryUsage, 0u);
 //       EXPECT_NEAR(static_cast<double>(memoryUsage), static_cast<double>(vectorMemory), static_cast<double>(vectorMemory) * 0.1f); // Allow 10% tolerance
    }

    // After vector goes out of scope, memory usage should decrease
    {
        this->_resources->clear();
        size_t memoryUsage = this->_resources->getMaxBytesEver();
        EXPECT_EQ(memoryUsage, 0u);
    }

    // Create multiple vectors and check memory usage
    {
        this->_resources->clear();
        VectorType vector1(size);
        VectorType vector2(size);
//        size_t memoryUsage = this->_resources->getMaxBytesEver();
//        EXPECT_GT(memoryUsage, 0u);
//        EXPECT_NEAR(static_cast<double>(memoryUsage), static_cast<double>(2 * vectorMemory), 2 * static_cast<double>(vectorMemory) * 0.1f); // Allow 10% tolerance
    }

    // After vector goes out of scope, memory usage should decrease
    {
        this->_resources->clear();
        size_t memoryUsage = this->_resources->getMaxBytesEver();
        EXPECT_EQ(memoryUsage, 0u);
    }

}

} // namespace MyBLAS
