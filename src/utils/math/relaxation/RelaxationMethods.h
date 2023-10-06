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
        METHOD_SSOR,
        METHOD_SORJ,
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
                "SSOR",
                "SORJ"
        };
        return relaxationMethodTypeKeys[static_cast<int>(value)];
    }

    //TODO:: DOCUMENT
    template<typename T>
    T inline approximateOptimalRelaxationFactor(const size_t meshX, const size_t meshY) {
        const T dx = 1 / (static_cast<T>(meshX) - 1);
        const T dy = 1 / (static_cast<T>(meshY) - 1);
        const T rho = std::cos(M_PIl * dx) + std::cos(M_PIl * dy);
        const T factor = 2 / (1 + std::sqrt(1 - std::pow(rho, 2)));
        return factor;
    }

    //TODO:: DOCUMENT
    template<typename T>
    T inline approximateOptimalRelaxationFactor(const size_t meshXY) {
        return approximateOptimalRelaxationFactor<T>(meshXY, meshXY);
    }

    // TODO:: DOCUMENT
    template <typename T>
    bool passesPreChecks(const MyBLAS::Matrix<T> &A, const MyBLAS::Vector<T> &b) {

        if(!MyBLAS::isSquareMatrix(A)) {
            std::cerr<<"Error: Failed pre-check: coefficient matrix is non-square.\n";
            return false;
        }

        if(!MyBLAS::noZerosInDiagonal(A)) {
            std::cerr<<"Error: Failed pre-check: coefficient matrix diagonal contains 0.\n";
            return false;
        }

        if(!MyBLAS::isDiagonallyDominant(A)) {
            std::cout<<"Warning: coefficient matrix is not diagonally dominant, may not converge\n";
        }

        return true;
    }

}

#endif //NE591_008_RELAXATIONMETHODS_H
