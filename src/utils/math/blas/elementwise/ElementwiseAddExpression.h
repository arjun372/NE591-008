#ifndef NE591_008_ELEMENTWISEADDEXPRESSION_H
#define NE591_008_ELEMENTWISEADDEXPRESSION_H

#include <cstddef>

namespace MyBLAS {
template <template <typename> class MatrixType, typename T>
class ElementwiseAdditionExpression {
  public:
    ElementwiseAdditionExpression(const MatrixType<T> &matrix1, const MatrixType<T> &matrix2)
        : matrix1_(matrix1), matrix2_(matrix2) {}

    T operator()(size_t i, size_t j) const { return matrix1_(i, j) + matrix2_(i, j); }

    static MatrixType<T> addVectors(const MatrixType<T> &a, const MatrixType<T> &b) {
        ElementwiseAdditionExpression expr(a, b);
        MatrixType<T> result = MatrixType<T>(a.getRows(), b.getCols());
        for (size_t i = 0; i < a.getRows(); ++i) {
            for (size_t j = 0; j < b.getCols(); ++j) {
                result(i, j) = expr(i, j);
            }
        }
        return result;
    }

  private:
    const MatrixType<T> &matrix1_;
    const MatrixType<T> &matrix2_;
};
}

#endif // NE591_008_ELEMENTWISEADDEXPRESSION_H
