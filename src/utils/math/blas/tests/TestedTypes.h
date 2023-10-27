/**
* @file TestedTypes.h
* @author Arjun Earthperson
* @date 10/27/2023
* @brief Type definitions for testing various MyBLAS data structures
*/

#ifndef NE591_008_TESTEDTYPES_H
#define NE591_008_TESTEDTYPES_H

#include "math/blas/matrix/LazyMatrix.h"
#include "math/blas/matrix/Matrix.h"
#include <gtest/gtest.h>
#include <tuple>

#define TOLERANCE 1e-14
#define FLOAT_TYPE long double

// Define a list of numeric types to run the tests with
typedef ::testing::Types<float, double, long double> NumericTypes;

// Type trait to generate matrix types
template <template <typename> typename MatrixType, typename TList>
struct MatrixTypesImpl;

template <template <typename> typename MatrixType, typename... Ts>
struct MatrixTypesImpl<MatrixType, ::testing::Types<Ts...>> {
    using type = ::testing::Types<MatrixType<Ts>...>;
};

template <typename T>
using LazyMatrixType = MyBLAS::LazyMatrix<T>;

template <typename T>
using MatrixType = MyBLAS::Matrix<T>;

using LazyMatrixTypes = typename MatrixTypesImpl<LazyMatrixType, NumericTypes>::type;
using EachMatrixTypes = typename MatrixTypesImpl<MatrixType, NumericTypes>::type;

// Helper type list concatenation
template <typename... Ts>
struct TypeList {};

template <typename TList1, typename TList2>
struct ConcatTypeLists;

template <typename... Ts1, typename... Ts2>
struct ConcatTypeLists<::testing::Types<Ts1...>, ::testing::Types<Ts2...>> {
    using type = ::testing::Types<Ts1..., Ts2...>;
};

using MatrixTypes = typename ConcatTypeLists<LazyMatrixTypes, EachMatrixTypes>::type;

#endif // NE591_008_TESTEDTYPES_H
