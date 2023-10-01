/**
 * @file MethodTypes.h
 * @author Arjun Earthperson
 * @date 09/30/2023
 * @brief TODO:: Document
*/

#ifndef NE591_008_RELAXATIONMETHODS_H
#define NE591_008_RELAXATIONMETHODS_H


namespace MyRelaxationMethod {

    // TODO:: Document
    enum Type {
        METHOD_POINT_JACOBI,
        METHOD_GAUSS_SEIDEL,
        METHOD_SOR,
        METHOD_SSOR
    };

    // TODO:: Document
    const char* TypeKey(MyRelaxationMethod::Type value) {
        static const char* relaxationMethodTypeKeys[] = {
                "LUP",
                "point-jacobi",
                "gauss-seidel",
                "SOR",
                "SSOR"
        };
        return relaxationMethodTypeKeys[static_cast<int>(value)];
    }

    // TODO:: Document
    template <typename T>
    struct Solution {
        explicit Solution<T>(MyBLAS::Vector<T> &initial) {
            x = initial;
        }
        explicit Solution<T>(size_t size) {
            x = MyBLAS::Vector<T>(size, 0);
        }
        Solution() = default;
        bool converged = false;
        size_t iterations = 0;
        T iterative_error = std::numeric_limits<T>::max();
        MyBLAS::Vector<T> x;
    };

}

#endif //NE591_008_RELAXATIONMETHODS_H
