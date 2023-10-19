
#ifndef NE591_008_CONTAINEREXPRESSION_H
#define NE591_008_CONTAINEREXPRESSION_H

#include <cstddef>

namespace MyBLAS {
template <template <typename> class ContainerType1, template <typename> class ContainerType2, typename T>
class ContainerExpression {
  public:
    ContainerExpression(const ContainerType1<T> &matrix1, const ContainerType2<T> &matrix2)
        : container1_(matrix1), container2_(matrix2) {}

    T operator()(size_t i, size_t k) const {
        T result = 0;
        for (size_t j = 0; j < container1_.getCols(); ++j) {
            result += container1_(i, j) * container2_(j, k);
        }
        return result;
    }

    T operator()(size_t i) const {
        T result = 0;
        for (size_t j = 0; j < container1_.getCols(); ++j) {
            result += container1_(i, j) * container2_[j];
        }
        return result;
    }

    static ContainerType2<T> multiplyMatrixMatrixType2(const ContainerType1<T> &a, const ContainerType2<T> &b) {
        ContainerExpression expr(a, b);
        return ContainerType2<T>(a.getRows(), b.getCols(), [expr](size_t i, size_t k) { return expr(i, k); });
    }

    static ContainerType1<T> multiplyMatrixMatrixType1(const ContainerType1<T> &a, const ContainerType2<T> &b) {
        ContainerExpression expr(a, b);
        return ContainerType1<T>(a.getRows(), b.getCols(), [expr](size_t i, size_t k) { return expr(i, k); });
    }

    static ContainerType2<T> multiplyMatrixVector(const ContainerType1<T> &a, const ContainerType2<T> &b) {
        ContainerExpression expr(a, b);
        return ContainerType2<T>(a.size(), [expr](size_t i) { return expr(i); });
    }


  private:
    const ContainerType1<T> &container1_;
    const ContainerType2<T> &container2_;
};
}
#endif // NE591_008_CONTAINEREXPRESSION_H
