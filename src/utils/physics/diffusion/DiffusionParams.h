/**
* @file DiffusionParams.h
* @brief This file contains the definition of the Params struct which encapsulates
* the parameters required for a diffusion solver.
*
* The Params struct is a template structure that holds parameters necessary for
* solving diffusion problems. It includes parameters such as the length of the
* rectangular region in both x and y directions, the number of mesh points in
* both x and y directions, the mesh spacing in both x and y directions, the
* diffusion coefficient for the diffusion equation, the macroscopic removal
* cross-section, and a matrix of sources for the diffusion problem.
*
* The Params struct also provides methods to set and get these parameters, and
* to compute the mesh spacing. It also includes an overloaded output stream
* operator for displaying the contents of Params.
*
* This file is part of the MyPhysics::Diffusion namespace, which encapsulates
* functionality related to solving diffusion problems.
*
* @author Arjun Earthperson
* @date 10/13/2023
 */

#ifndef NE591_008_DIFFUSIONPARAMS_H
#define NE591_008_DIFFUSIONPARAMS_H

#include <cmath>
#include <cstddef>
#include <memory>
#include <ostream>
#include <stdexcept>

#include "math/blas/matrix/Matrix.h"

/**
 * @brief Namespace for the diffusion physics module.
 * This namespace encapsulates functionality related to solving diffusion problems.
 */
namespace MyPhysics::Diffusion {

/**
 * @brief Template struct to hold parameters for the diffusion solver.
 *
 * This struct contains parameters necessary for solving diffusion problems.
 *
 * @tparam FloatType The data type used for storing parameters.
 */
template <typename FloatType>
struct Params {

  private:
    /**
     * @brief Length of the rectangular region in the x-direction.
     *
     * This value represents the extent of the rectangular region in the x-direction.
     */
    FloatType _a = 1;

    /**
     * @brief Length of the rectangular region in the y-direction.
     *
     * This value represents the extent of the rectangular region in the y-direction.
     */
    FloatType _b = 1;

    /**
     * @brief Number of mesh points in the x-direction, excluding boundary points.
     *
     * This value determines the number of mesh points in the x-direction, excluding the boundary points.
     */
    size_t _m = 1;

    /**
     * @brief Number of mesh points in the y-direction, excluding boundary points.
     *
     * This value determines the number of mesh points in the y-direction, excluding the boundary points.
     */
    size_t _n = 1;

    /**
     * @brief Mesh spacing in the x-direction.
     *
     * This value represents the spacing between mesh points in the x-direction.
     */
    FloatType _delta = 1;

    /**
     * @brief Mesh spacing in the y-direction.
     *
     * This value represents the spacing between mesh points in the y-direction.
     */
    FloatType _gamma = 1;

    /**
     * @brief Diffusion coefficient for the diffusion equation.
     *
     * This coefficient represents the diffusion property in the diffusion equation.
     */
    FloatType _diff_coeff = 0.0f;

    /**
     * @brief Macroscopic removal cross-section.
     *
     * This value represents the macroscopic removal cross-section, which is a parameter in the diffusion equation.
     */
    FloatType _cross_section = 0.0f;

    /**
     * @brief Matrix of sources for the diffusion problem.
     *
     * This matrix holds source terms for the diffusion problem.
     */
    MyBLAS::Matrix<FloatType> _sources = MyBLAS::Matrix<FloatType>();

    /**
     * @brief Calculates the mesh spacings in the given direction.
     *
     * Computes the mesh spacings using the equation: spacing = length / (points + 1).
     * These spacings are used to discretize the diffusion operator in the given equation.
     *
     * @param length The length of the rectangular region in the given direction.
     * @param points The number of mesh points in the given direction, excluding the boundary points.
     * @param[out] delta The mesh spacing in the given direction.
     */
    static inline FloatType compute_mesh_spacing(const FloatType length, const size_t points) {
        const FloatType spacing = length / (static_cast<FloatType>(points) + static_cast<FloatType>(1));
        return spacing;
    }

  public:
    /**
     * @brief Default constructor for Params.
     */
    Params() = default;

    /**
     * @brief Constructor with initialization of parameters.
     *
     * @param a The length of the rectangular region in the x-direction.
     * @param b The length of the rectangular region in the y-direction.
     * @param m The number of mesh points in the x-direction.
     * @param n The number of mesh points in the y-direction.
     * @param coefficient Diffusion coefficient for the diffusion equation.
     * @param crossSection Macroscopic removal cross-section.
     * @param sources Matrix of sources for the diffusion problem.
     */
    Params(FloatType a, FloatType b, size_t m, size_t n, FloatType coefficient, FloatType crossSection, MyBLAS::Matrix<FloatType> &sources) {
        setA(a);
        setB(b);
        setM(m);
        setN(n);
        setDiffusionCoefficient(coefficient);
        setMacroscopicRemovalCrossSection(crossSection);
        setSources(sources);
    }

    /**
     * @brief Set the length of the rectangular region in the x-direction.
     *
     * @param a The length of the rectangular region in the x-direction.
     * @return A reference to the Params struct.
     *
     * @throw std::invalid_argument if the provided value is non-positive or not finite.
     */
    Params &setA(FloatType a) {
        if (a <= 0 || !std::isfinite(a)) {
            throw std::invalid_argument("Invalid value for a. It should be a positive finite number.");
        }
        _a = a;
        _delta = compute_mesh_spacing(a, _m);
        return *this;
    }

    /**
     * @brief Set the length of the rectangular region in the y-direction.
     *
     * @param b The length of the rectangular region in the y-direction.
     * @return A reference to the Params struct.
     *
     * @throw std::invalid_argument if the provided value is non-positive or not finite.
     */
    Params &setB(FloatType b) {
        if (b <= 0 || !std::isfinite(b)) {
            throw std::invalid_argument("Invalid value for b. It should be a positive finite number.");
        }
        _b = b;
        _gamma = compute_mesh_spacing(b, _n);
        return *this;
    }

    /**
     * @brief Set the number of mesh points in the x-direction.
     *
     * @param m The number of mesh points in the x-direction.
     * @return A reference to the Params struct.
     *
     * @throw std::invalid_argument if the provided value is zero.
     */
    Params &setM(size_t m) {
        if (m == 0) {
            throw std::invalid_argument("Invalid value for m. It should be a positive number.");
        }
        _m = m;
        _delta = compute_mesh_spacing(_a, m);
        return *this;
    }

    /**
     * @brief Set the number of mesh points in the y-direction.
     *
     * @param n The number of mesh points in the y-direction.
     * @return A reference to the Params struct.
     *
     * @throw std::invalid_argument if the provided value is zero.
     */
    Params &setN(size_t n) {
        if (n == 0) {
            throw std::invalid_argument("Invalid value for n. It should be a positive number.");
        }
        _n = n;
        _gamma = compute_mesh_spacing(_b, n);
        return *this;
    }

    /**
     * @brief Set the diffusion coefficient for the diffusion equation.
     *
     * @param coefficient Diffusion coefficient for the diffusion equation.
     * @return A reference to the Params struct.
     *
     * @throw std::invalid_argument if the provided value is non-positive or not finite.
     */
    Params &setDiffusionCoefficient(FloatType coefficient) {
       if (coefficient <= 0 || !std::isfinite(coefficient)) {
           throw std::invalid_argument("Invalid value for diffusion coefficient. It should be a positive finite number.");
       }
       _diff_coeff = coefficient;
       return *this;
    }

    /**
     * @brief Set the macroscopic removal cross-section.
     *
     * @param crossSection Macroscopic removal cross-section.
     * @return A reference to the Params struct.
     *
     * @throw std::invalid_argument if the provided value is non-positive or not finite.
     */
    Params &setMacroscopicRemovalCrossSection(FloatType crossSection) {
       if (crossSection <= 0 || !std::isfinite(crossSection)) {
           throw std::invalid_argument("Invalid value for macroscopic removal cross-section. It should be a positive finite number.");
       }
       _cross_section = crossSection;
       return *this;
    }

    /**
     * @brief Set the matrix of sources for the diffusion problem.
     *
     * @param sources Matrix of sources for the diffusion problem.
     * @return A reference to the Params struct.
     *
     * @throw std::invalid_argument if the dimensions of the sources matrix do not match with the number of mesh points.
     */
    Params &setSources(const MyBLAS::Matrix<FloatType> &sources) {
       if (sources.getRows() != _m || sources.getCols() != _n) {
           throw std::invalid_argument("Invalid dimensions for the sources matrix. It should match with the number of mesh points.");
       }
       _sources = sources;
       return *this;
    }

    /**
     * @brief Get the length of the rectangular region in the x-direction.
     *
     * @return The length of the rectangular region in the x-direction.
     */
    [[nodiscard]] FloatType getA() const { return _a; }

    /**
     * @brief Get the length of the rectangular region in the y-direction.
     *
     * @return The length of the rectangular region in the y-direction.
     */
    [[nodiscard]] FloatType getB() const { return _b; }

    /**
     * @brief Get the number of mesh points in the x-direction.
     *
     * @return The number of mesh points in the x-direction.
     */
    [[nodiscard]] size_t getM() const { return _m; }

    /**
     * @brief Get the number of mesh points in the y-direction.
     *
     * @return The number of mesh points in the y-direction.
     */
    [[nodiscard]] size_t getN() const { return _n; }

    /**
     * @brief Get the mesh spacing in the x-direction.
     *
     * @return The mesh spacing in the x-direction.
     */
    [[nodiscard]] FloatType getDelta() const { return _delta; }

    /**
     * @brief Get the mesh spacing in the y-direction.
     *
     * @return The mesh spacing in the y-direction.
     */
    [[nodiscard]] FloatType getGamma() const { return _gamma; }

    /**
     * @brief Get the diffusion coefficient for the diffusion equation.
     *
     * @return The diffusion coefficient for the diffusion equation.
     */
    [[nodiscard]] FloatType getDiffusionCoefficient() const { return _diff_coeff; }

    /**
     * @brief Get the macroscopic removal cross-section.
     *
     * @return The macroscopic removal cross-section.
     */
    [[nodiscard]] FloatType getMacroscopicRemovalCrossSection() const { return _cross_section; }

    /**
     * @brief Get the matrix of sources for the diffusion problem.
     *
     * @return The matrix of sources for the diffusion problem.
     */
    const MyBLAS::Matrix<FloatType> &getSources() const { return _sources; }

    /**
     * @brief Overloaded output stream operator for displaying the contents of Params.
     *
     * This operator allows you to stream the contents of a Params object to an output stream, typically for debugging or logging purposes.
     *
     * @param os The output stream to which the Params content will be written.
     * @param params The Params object to display.
     * @return The updated output stream after writing the Params content.
     */
    friend std::ostream &operator<<(std::ostream &os, const Params &params) {
       os << "a: " << params._a << " b: " << params._b << " m: " << params._m << " n: " << params._n
          << " delta: " << params._delta << " gamma: " << params._gamma << " diff_coeff: " << params._diff_coeff
          << " cross_section: " << params._cross_section;
       return os;
    }
};

}  // namespace MyPhysics::Diffusion

#endif // NE591_008_DIFFUSIONPARAMS_H

// TODO:: REMOVE!
//#ifndef NE591_008_DIFFUSIONPARAMS_H
//#define NE591_008_DIFFUSIONPARAMS_H
//
//#include <cmath>
//#include <cstddef>
//#include <memory>
//#include <ostream>
//#include <stdexcept>
//
//#include "math/blas/matrix/Matrix.h"
//
//namespace MyPhysics::Diffusion {
//
//template <typename FloatType>
//struct Params {
//
//  private:
//    FloatType _a = 1;
//    FloatType _b = 1;
//    size_t _m = 1;
//    size_t _n = 1;
//    FloatType _delta = 1;
//    FloatType _gamma = 1;
//    FloatType _diff_coeff = 0.0f;
//    FloatType _cross_section = 0.0f;
//    MyBLAS::Matrix<FloatType> _sources = MyBLAS::Matrix<FloatType>();
//
//    static inline FloatType compute_mesh_spacing(const FloatType length, const size_t points) {
//       const FloatType spacing = length / (static_cast<FloatType>(points) + static_cast<FloatType>(1));
//       return spacing;
//    }
//
//  public:
//    Params() = default;
//
//    Params(FloatType a, FloatType b, size_t m, size_t n, FloatType coefficient, FloatType crossSection, MyBLAS::Matrix<FloatType> &sources) {
//       setA(a);
//       setB(b);
//       setM(m);
//       setN(n);
//       setDiffusionCoefficient(coefficient);
//       setMacroscopicRemovalCrossSection(crossSection);
//       setSources(sources);
//    }
//
//    Params &setA(FloatType a) {
//       if (a <= 0 || !std::isfinite(a)) {
//           throw std::invalid_argument("Invalid value for a. It should be a positive finite number.");
//       }
//       _a = a;
//       _delta = compute_mesh_spacing(a, _m);
//       return *this;
//    }
//
//    Params &setB(FloatType b) {
//       if (b <= 0 || !std::isfinite(b)) {
//           throw std::invalid_argument("Invalid value for b. It should be a positive finite number.");
//       }
//       _b = b;
//       _gamma = compute_mesh_spacing(b, _n);
//       return *this;
//    }
//
//    Params &setM(size_t m) {
//       if (m == 0) {
//           throw std::invalid_argument("Invalid value for m. It should be a positive number.");
//       }
//       _m = m;
//       _delta = compute_mesh_spacing(_a, m);
//       return *this;
//    }
//
//    Params &setN(size_t n) {
//       if (n == 0) {
//           throw std::invalid_argument("Invalid value for n. It should be a positive number.");
//       }
//       _n = n;
//       _gamma = compute_mesh_spacing(_b, n);
//       return *this;
//    }
//
//    Params &setDiffusionCoefficient(FloatType coefficient) {
//       if (coefficient <= 0 || !std::isfinite(coefficient)) {
//           throw std::invalid_argument("Invalid value for diffusion coefficient. It should be a positive finite number.");
//       }
//       _diff_coeff = coefficient;
//       return *this;
//    }
//
//    Params &setMacroscopicRemovalCrossSection(FloatType crossSection) {
//       if (crossSection <= 0 || !std::isfinite(crossSection)) {
//           throw std::invalid_argument("Invalid value for macroscopic removal cross-section. It should be a positive finite number.");
//       }
//       _cross_section = crossSection;
//       return *this;
//    }
//
//    Params &setSources(const MyBLAS::Matrix<FloatType> &sources) {
//       if (sources.getRows() != _m || sources.getCols() != _n) {
//           throw std::invalid_argument("Invalid dimensions for the sources matrix. It should match with the number of mesh points.");
//       }
//       _sources = sources;
//       return *this;
//    }
//
//    [[nodiscard]] FloatType getA() const { return _a; }
//
//    [[nodiscard]] FloatType getB() const { return _b; }
//
//    [[nodiscard]] size_t getM() const { return _m; }
//
//    [[nodiscard]] size_t getN() const { return _n; }
//
//    [[nodiscard]] FloatType getDelta() const { return _delta; }
//
//    [[nodiscard]] FloatType getGamma() const { return _gamma; }
//
//    [[nodiscard]] FloatType getDiffusionCoefficient() const { return _diff_coeff; }
//
//    [[nodiscard]] FloatType getMacroscopicRemovalCrossSection() const { return _cross_section; }
//
//    const MyBLAS::Matrix<FloatType> &getSources() const { return _sources; }
//
//    friend std::ostream &operator<<(std::ostream &os, const Params &params) {
//       os << "a: " << params._a << " b: " << params._b << " m: " << params._m << " n: " << params._n
//          << " delta: " << params._delta << " gamma: " << params._gamma << " diff_coeff: " << params._diff_coeff
//          << " cross_section: " << params._cross_section;
//       return os;
//    }
//};
//
//}  // namespace MyPhysics::Diffusion
//
//#endif // NE591_008_DIFFUSIONPARAMS_H
