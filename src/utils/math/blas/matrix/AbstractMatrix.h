#ifndef NE591_008_ABSTRACTMATRIX_H
#define NE591_008_ABSTRACTMATRIX_H

#include <cstddef>

namespace MyBLAS {

template <typename DataType>
class AbstractMatrix {
  public:
    virtual ~AbstractMatrix() = default;

    // Access elements
    virtual DataType& operator()(size_t row, size_t col) = 0;
    virtual const DataType& operator()(size_t row, size_t col) const = 0;

    // Get dimensions
    virtual size_t getRows() const = 0;
    virtual size_t getCols() const = 0;

    // Arithmetic operations
    virtual AbstractMatrix& operator+(const AbstractMatrix& rhs) const = 0;
    virtual AbstractMatrix& operator-(const AbstractMatrix& rhs) const = 0;
    virtual AbstractMatrix& operator*(const AbstractMatrix& rhs) const = 0;
    virtual AbstractMatrix& operator/(const AbstractMatrix& rhs) const = 0;

    // In-place arithmetic operations
    virtual AbstractMatrix& operator+=(const AbstractMatrix& rhs) = 0;
    virtual AbstractMatrix& operator-=(const AbstractMatrix& rhs) = 0;
    virtual AbstractMatrix& operator*=(const AbstractMatrix& rhs) = 0;
    virtual AbstractMatrix& operator/=(const AbstractMatrix& rhs) = 0;

    // Comparison operations
    virtual bool operator==(const AbstractMatrix& rhs) const = 0;
    virtual bool operator!=(const AbstractMatrix& rhs) const = 0;

    // Submatrix operations
    virtual void setSubMatrix(size_t rowStart, size_t colStart, const AbstractMatrix& subMatrix) = 0;
    virtual AbstractMatrix subMatrix(size_t rowStart, size_t colStart, size_t subRows, size_t subCols) const = 0;

    // Row operations
    virtual void swapRows(size_t row1, size_t row2) = 0;
};

} // namespace MyBLAS

#endif // NE591_008_ABSTRACTMATRIX_H


//You can specify a default policy by providing a default value for the template parameter:
//
//```cpp
//template <typename T>
//class DefaultPolicy {
//  public:
//    static void execute(MyMatrix<T, DefaultPolicy>& matrix) {
//        // Default implementation
//    }
//};

//template <typename T, template <typename> class Policy = DefaultPolicy>
//class MyMatrix {
//  public:
//    void someMethod() {
//        Policy<T>::execute(*this);
//    }
//};
//```
//
//    In this example, `DefaultPolicy` is the default policy that is used when no policy is specified. You can create a `MyMatrix` with the default policy like this:
//
//```cpp
//        MyMatrix<int> matrix;  // Uses DefaultPolicy
//```
//
//    And you can still specify a different policy if you want:
//
//```cpp
//        MyMatrix<int, Policy1> matrix;  // Uses Policy1
//```
//
//    This design allows you to provide a sensible default implementation, while still giving users the flexibility to specify a different implementation if they need to.
