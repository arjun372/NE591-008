/**
* @file DiffusionMatrix.h
* @brief This file defines the Matrix class within the MyPhysics::Diffusion namespace.
*        The Matrix class is a template class that extends the MyBLAS::LazyMatrix class.
*        It is used to represent a diffusion matrix in a physics simulation.
* @author Arjun Earthperson
* @date 10/13/2023
 */

#ifndef NE591_008_DIFFUSIONMATRIX_H
#define NE591_008_DIFFUSIONMATRIX_H

#include "DiffusionConstants.h"
#include "DiffusionParams.h"
#include "math/blas/matrix/LazyMatrix.h"
#include <ostream>

/**
 * @namespace MyPhysics::Diffusion
 * @brief This namespace contains classes and functions related to diffusion physics.
 */
namespace MyPhysics::Diffusion {

/**
 * @class Matrix
 * @brief This class represents a diffusion matrix. It extends the MyBLAS::LazyMatrix class.
 * @tparam T The type of the elements in the matrix.
 */
template <typename T>
class Matrix : public MyBLAS::LazyMatrix<T> {

  public:
    /**
     * @typedef Generator
     * @brief A function that generates a matrix element given its row and column indices.
     */
    using Generator = std::function<T(size_t, size_t)>;

  private:
    /**
     * @brief The parameters of the diffusion process.
     */
    Params<T> _params{};
    /**
     * @brief The size of the matrix.
     */
    size_t _size = 0;
    /**
     * @brief The constants used in the diffusion process.
     */
    Constants<T> _constants{};

  public:
    /**
     * @brief Default constructor.
     */
    explicit Matrix() = default;
    /**
     * @brief Constructor that takes diffusion parameters.
     * @param params The diffusion parameters.
     */
    explicit Matrix(const Params<T> params): _params(params), _constants(Constants<T>::compute(params)) {
        _size = _params.getM() * _params.getN();
        this->setRows(_size);
        this->setCols(_size);
        this->setGenerator([this](size_t i, size_t j) {
            return generate(i, j, _constants);
        });
    }
    /**
     * @brief Copy constructor.
     * @param matrix The matrix to copy.
     */
    Matrix(const Matrix<T> &matrix): MyBLAS::LazyMatrix<T>(matrix), _params(matrix._params), _constants(Constants<T>::compute(matrix._params)) {
        _size = matrix._size;
        this->setRows(_size);
        this->setCols(_size);
        this->setGenerator([this](size_t i, size_t j) {
            return generate(i, j, _constants);
        });
    }
    /**
     * @brief Copy assignment operator.
     * @param other The matrix to copy.
     * @return A reference to this matrix.
     */
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
    /**
     * @brief Generates a matrix element.
     * @param idx The row index.
     * @param idy The column index.
     * @param c The diffusion constants.
     * @return The generated matrix element.
     */
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
    /**
     * @brief Constructor that takes diffusion parameters and a generator function.
     * @param params The diffusion parameters.
     * @param generator The generator function.
     */
    Matrix(const Params<T> &params, Generator generator): _params(params), _constants(Constants<T>::compute(params)) {
        _size = _params.getM() * _params.getN();
        this->setRows(_size);
        this->setCols(_size);
        this->setGenerator(generator);
    }
    /**
     * @brief Returns the diffusion parameters.
     * @return The diffusion parameters.
     */
    [[maybe_unused]] [[nodiscard]] const Params<T> &getParams() const { return _params; }
    /**
     * @brief Returns the diffusion constants.
     * @return The diffusion constants.
     */
    [[maybe_unused]] [[nodiscard]] const Constants<T> &getConstants() const { return _constants; }
    /**
     * @brief Returns the number of rows in the matrix.
     * @return The number of rows in the matrix.
     */
    [[nodiscard]] size_t getRows() const {
        return _size;
    }
    /**
     * @brief Returns the number of columns in the matrix.
     * @return The number of columns in the matrix.
     */
    [[nodiscard]] size_t getCols() const {
        return _size;
    }
};
}
#endif // NE591_008_DIFFUSIONMATRIX_H
