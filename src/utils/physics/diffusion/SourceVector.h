/**
* @file SourceVector.h
* @brief TODO:: DOCUMENT
* @author Arjun Earthperson
* @date 10/13/2023
* TODO:: DETAILED EXPLANATION
 */

#ifndef NE591_008_SOURCEVECTOR_H
#define NE591_008_SOURCEVECTOR_H

#include "DiffusionConstants.h"
#include "DiffusionParams.h"

#include "math/blas/vector/LazyVector.h"

namespace MyPhysics::Diffusion {

template <typename T>
class Vector : public MyBLAS::LazyVector<T> {

  public:
    using Generator = std::function<T(size_t)>;

  private:
    Params<T> _params{};
    size_t _size = 0;
    Constants<T> _constants{};

  public:

    explicit Vector() = default;

    explicit Vector(const Params<T> params): _params(params), _constants(Constants<T>::compute(params)) {
        _size = _params.getM() * _params.getN();
        this->setSize(_size);
        this->setGenerator([this](size_t i) {
            return generate(i, _constants);
        });
    }

    Vector(const Vector<T> &vector): _params(vector.params), _constants(Constants<T>::compute(vector.params)) {
        _size = vector._size;
        this->setSize(_size);
        this->setGenerator([this](size_t i) {
            return generate(i, _constants);
        });
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            _params = other._params;
            _size = other._size;
            _constants = other._constants;
            this->setSize(_size);
            this->setGenerator([this](size_t i) {
                return generate(i, _constants);
            });
        }
        return *this;
    }

    static T generate(size_t idx, const Constants<T> &c) {
        // Define your generation logic here
        // This is just a placeholder
        return static_cast<T>(idx);
    }

    Vector(const Params<T> &params, Generator generator): _params(params), _constants(Constants<T>::compute(params)) {
        _size = _params.getM() * _params.getN();
        this->setSize(_size);
        this->setGenerator(generator);
    }

    [[nodiscard]] const Params<T> &getParams() const { return _params; }

    [[nodiscard]] const Constants<T> &getConstants() const { return _constants; }

    [[nodiscard]] size_t getSize() const {
        return _size;
    }

};
}

#endif // NE591_008_SOURCEVECTOR_H
