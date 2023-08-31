/**
 * @file Compute.h
 * @author Arjun Earthperson
 * @date 09/01/2023
 * @brief Compute methods for outlab1 in NE591-008.
 */

#pragma once

#include <iostream>
#include <cmath>

#include "utils/CommandLine.h"
#include "ProcessInputs.h"

typedef struct MatrixBuildArgs {
    size_t idx_row;
    size_t idx_col;
    InputParams &inputParams;
} MatrixBuildArgs;

/**
 * @brief Computes the value of an element in matrix A.
 *
 * This function computes the value of an element in matrix A based on its row and column indices.
 * If the row index is equal to the column index, the value is 1.0. Otherwise, the value is 0.5.
 *
 * @param args A struct containing the row and column indices of the element.
 * @return The computed value of the element.
 */
static inline long double elementFromA(const MatrixBuildArgs &args) {
    return (args.idx_col == args.idx_row) ? 1.0f : 0.5f;
}

/**
 * @brief Computes the value of an element in matrix B.
 *
 * This function computes the value of an element in matrix B based on its row and column indices.
 * If the row index is greater than the column index, the value is 0.75. Otherwise, the value is 0.25.
 *
 * @param args A struct containing the row and column indices of the element.
 * @return The computed value of the element.
 */
static inline long double elementFromB(const MatrixBuildArgs &args) {
    return (args.idx_row > args.idx_col) ? 0.75f : 0.25f;
}

/**
 * @brief Computes the value of an element in matrix C.
 *
 * This function computes the value of an element in matrix C by adding the corresponding values from matrices A and B.
 *
 * @param args A struct containing the row and column indices of the element.
 * @return The computed value of the element.
 */
static inline long double elementFromC(const MatrixBuildArgs &args) {
    return elementFromA(args) + elementFromB(args);
}

/**
 * @brief Computes the value of an element in matrix D.
 *
 * This function computes the value of an element in matrix D by multiplying the corresponding value from matrix A by a
 * constant k.
 *
 * @param args A struct containing the row and column indices of the element and the constant k.
 * @return The computed value of the element.
 */
static inline long double elementFromD(const MatrixBuildArgs &args) {
    return args.inputParams.k * elementFromA(args);
}

/**
 * @brief Computes the value of an element in matrix F.
 *
 * This function calculates the value of an element in matrix F using the formula 1/(row index + column index).
 * The row and column indices are assumed to start from 1.
 *
 * @param args A MatrixBuildArgs struct containing the row and column indices for the element to be computed.
 * @return The computed value of the element in matrix F.
 */
static inline long double elementFromF(const MatrixBuildArgs &args) {
    return 1.0f / (static_cast<long double>(args.idx_row + args.idx_col));
}

/**
 * @brief Computes the value of an element in matrix E.
 *
 * This function calculates the value of an element in matrix E by summing up the product of corresponding elements
 * from matrix A and matrix F for all columns from 1 to N. The indices of the element in matrix E are provided
 * through the MatrixBuildArgs structure.
 *
 * @param args A MatrixBuildArgs structure containing the row and column indices of the element in matrix E,
 * and the input parameters required for the computation.
 * @return The computed value of the element in matrix E.
 *
 * @note The computation involves a loop that runs from 1 to N (inclusive), where N is a parameter provided in
 * the inputParams member of the MatrixBuildArgs structure. For each iteration, the function constructs new
 * MatrixBuildArgs structures for evaluating elements from matrix A and matrix F, multiplies these elements,
 * and adds the product to an accumulator. The final value of the accumulator is returned as the result.
 */
static inline long double elementFromE(const MatrixBuildArgs &args) {

    long double accumulator = 0.0f;

    // loop n -> N
    for(size_t n = 1; n <= args.inputParams.N; n++) {

        // build args for evaluating element A(m,n)
        const MatrixBuildArgs argA = {
                .idx_row = args.idx_row,
                .idx_col = n,
                .inputParams = args.inputParams,
        };

        const long double a_mxn = elementFromA(argA);

        // build args for evaluating element F(n,j)
        const MatrixBuildArgs argF = {
                .idx_row = n,
                .idx_col = args.idx_col,
                .inputParams = args.inputParams,
        };

        const long double f_nxj = elementFromF(argF);

        const long double product = a_mxn * f_nxj;

        accumulator += product;
    }

    return accumulator;
}

/**
 * @struct ArgsPrintMatrix
 * @brief A struct to hold the arguments for the printMatrix function.
 *
 * This struct is used to pass multiple arguments to the printMatrix function. It includes the starting row and column,
 * the number of rows and columns to print, a reference to the input parameters, and a function pointer to the function
 * that will evaluate the value of each element in the matrix.
 *
 * @var ArgsPrintMatrix::start_row
 * The starting row index for the matrix to be printed.
 *
 * @var ArgsPrintMatrix::start_col
 * The starting column index for the matrix to be printed.
 *
 * @var ArgsPrintMatrix::num_rows
 * The number of rows to be printed from the matrix.
 *
 * @var ArgsPrintMatrix::num_cols
 * The number of columns to be printed from the matrix.
 *
 * @var ArgsPrintMatrix::inputParams
 * A reference to the input parameters needed to evaluate the matrix elements.
 *
 * @var ArgsPrintMatrix::evaluate
 * A function pointer to the function that will evaluate the value of each element in the matrix.
 */
typedef struct ArgsPrintMatrix {
    size_t start_row;
    size_t start_col;
    size_t num_rows;
    size_t num_cols;
    InputParams &inputParams;
    long double (*evaluate)(const MatrixBuildArgs &);
} ArgsPrintMatrix;

/**
 * @brief This function prints a matrix with a given title.
 *
 * The function loops over the rows and columns of the matrix, evaluates the value of each element using the provided
 * evaluation function, and then prints the value. The matrix is printed row by row, with each element having two
 * decimal points. A newline is printed at the end of each row. The title of the matrix is printed before the matrix
 * itself.
 *
 * @param args A struct of type ArgsPrintMatrix, which contains the following fields:
 * - start_row: The index of the first row of the matrix to be printed.
 * - start_col: The index of the first column of the matrix to be printed.
 * - num_rows: The number of rows in the matrix.
 * - num_cols: The number of columns in the matrix.
 * - scalar: A scalar value to be used in the evaluation of the matrix elements.
 * - inputParams: A reference to an InputParams object, which contains parameters to be used in the evaluation of the
 *   matrix elements.
 * - evaluate: A pointer to a function that takes a MatrixBuildArgs object and returns a long double. This function is
 *   used to evaluate the value of each element in the matrix.
 *
 * @param title A string that contains the title of the matrix. This title is printed before the matrix itself.
 *
 * @return void
 */
static void printMatrix(const ArgsPrintMatrix &args, const std::string &title) {
    // print the title
    std::cout<<std::endl<<title<<std::endl<<std::endl;

    // loop over rows
    for(auto idx_row = args.start_row; idx_row <= args.num_rows; idx_row++) {

        // loop over columns
        for(auto idx_col = args.start_col; idx_col <= args.num_cols; idx_col++) {

            // construct a new variable in the hope that the compiler will unfold this loop and vectorize the
            // evaluate op.
            auto points = MatrixBuildArgs{
                .idx_row = idx_row,
                .idx_col = idx_col,
                .inputParams = args.inputParams,
            };

            // get the element value
            const auto value = args.evaluate(points);

            //print two decimal points
            std::printf("%.2Lf  ", value);
        }
        // newline at end of columns
        std::cout<<std::endl;
    }
    // padding
    std::cout<<std::endl;
}
