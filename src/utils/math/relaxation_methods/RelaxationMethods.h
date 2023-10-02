/**
 * @file RelaxationMethods.h
 * @author Arjun Earthperson
 * @date 09/30/2023
 * @brief This file contains the declaration of the RelaxationMethods namespace, which includes the Type enumeration,
 * the TypeKey function, and the Solution struct template.
*/

#ifndef NE591_008_RELAXATIONMETHODS_H
#define NE591_008_RELAXATIONMETHODS_H

namespace MyRelaxationMethod {

    /**
     * @brief Enum for different types of relaxation methods.
     *
     * This enum represents the different types of relaxation methods that can be used in the program.
     * The methods include Point Jacobi, Gauss Seidel, SOR, and SSOR.
     */
    enum Type {
        METHOD_POINT_JACOBI,
        METHOD_GAUSS_SEIDEL,
        METHOD_SOR,
        METHOD_SSOR
    };

    /**
     * @brief Function to get the key of a relaxation method type.
     *
     * This function takes a value of the Type enum and returns the corresponding key as a string.
     *
     * @param value The value of the Type enum.
     * @return The key of the relaxation method type as a string.
     */
    const char* TypeKey(MyRelaxationMethod::Type value) {
        static const char* relaxationMethodTypeKeys[] = {
                "point-jacobi",
                "gauss-seidel",
                "SOR",
                "SSOR"
        };
        return relaxationMethodTypeKeys[static_cast<int>(value)];
    }

    /**
     * @brief Struct template to represent a solution.
     *
     * This struct template represents a solution. It contains a boolean to indicate if the solution has converged,
     * a size_t to represent the number of iterations, a T to represent the iterative error, and a Vector of T to represent the solution.
     *
     * @tparam T The type of the elements in the solution vector.
     */
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
