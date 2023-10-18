/**
* @file DiffusionMatrix.h
* @brief TODO:: DOCUMENT
* @author Arjun Earthperson
* @date 10/13/2023
* TODO:: DETAILED EXPLANATION
 */

#ifndef NE591_008_DIFFUSIONMATRIX_H
#define NE591_008_DIFFUSIONMATRIX_H

#include "DiffusionConstants.h"
#include "DiffusionParams.h"
#include "math/blas/matrix/LazyMatrix.h"
#include <ostream>

namespace MyPhysics::Diffusion {

//TODO:: DOCUMENT
template <typename T>
class Matrix : public MyBLAS::LazyMatrix<T> {

  public:
    using Generator = std::function<T(size_t, size_t)>;

  private:
    Params<T> _params{};
    size_t _size = 0;
    Constants<T> _constants{};

  public:

    explicit Matrix() = default;

    explicit Matrix(const Params<T> params): _params(params), _constants(Constants<T>::compute(params)) {
        _size = _params.getM() * _params.getN();
        this->setRows(_size);
        this->setCols(_size);
        this->setGenerator([this](size_t i, size_t j) {
            return generate(i, j, _constants);
        });
    }

    Matrix(const Matrix<T> &matrix): MyBLAS::LazyMatrix<T>(matrix), _params(matrix._params), _constants(Constants<T>::compute(matrix._params)) {
        _size = matrix._size;
        this->setRows(_size);
        this->setCols(_size);
        this->setGenerator([this](size_t i, size_t j) {
            return generate(i, j, _constants);
        });
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            _params = other._params;
            _size = other._size;
            _constants = other._constants;
            this->setRows(_size);
            this->setCols(_size);
            this->setGenerator([this](size_t i, size_t j) {
                return generate(i, j, _constants);
            });
        }
        return *this;
    }

    static T generate(size_t idx, size_t idy, const Constants<T> &c) {
        if (idx == idy) {
            return c.diagonal;
        }
        const size_t i = (idx / c.n), p = (idy / c.n);
        if ((i == p) && (idx == idy + 1 || idx == idy - 1)) {
            return c.minus_D_over_gamma_squared;
        }
        const size_t j = (idx % c.n), q = (idy % c.n);
        if ((j == q) && (idx == idy + c.n || idx == idy - c.n)) {
            return c.minus_D_over_delta_squared;
        }
        return 0;
    }

    Matrix(const Params<T> &params, Generator generator): _params(params), _constants(Constants<T>::compute(params)) {
        _size = _params.getM() * _params.getN();
        this->setRows(_size);
        this->setCols(_size);
        this->setGenerator(generator);
    }

    [[nodiscard]] const Params<T> &getParams() const { return _params; }

    [[nodiscard]] const Constants<T> &getConstants() const { return _constants; }

    [[nodiscard]] size_t getRows() const {
        return _size;
    }

    [[nodiscard]] size_t getCols() const {
        return _size;
    }
};
}
#endif // NE591_008_DIFFUSIONMATRIX_H
