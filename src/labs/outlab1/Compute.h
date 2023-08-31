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

static inline long double elementFromA(const MatrixBuildArgs &args) {
    return (args.idx_col == args.idx_row) ? 1.0f : 0.5f;
}

static inline long double elementFromB(const MatrixBuildArgs &args) {
    return (args.idx_row > args.idx_col) ? 0.75 : 0.25;
}

static inline long double elementFromC(const MatrixBuildArgs &args) {
    return elementFromA(args) + elementFromB(args);
}

static inline long double elementFromD(const MatrixBuildArgs &args) {
    return args.inputParams.k * elementFromA(args);
}

static inline long double elementFromF(const MatrixBuildArgs &args) {
    return 1.0f / (static_cast<long double>(args.idx_row + args.idx_col));
}


static inline long double elementFromE(const MatrixBuildArgs &args) {

    long double accumulator = 0.0f;

    // loop n -> N
    for(size_t n = 1; n < args.inputParams.N; n++) {

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


typedef struct  {
    size_t start_row;
    size_t start_col;
    size_t num_rows;
    size_t num_cols;
    long double scalar;
    InputParams &inputParams;
    long double (*evaluate)(const MatrixBuildArgs &);
} ArgsPrintMatrix;

static void printMatrix(const ArgsPrintMatrix &args, const std::string &title) {
    // print the title
    std::cout<<std::endl<<title<<std::endl<<std::endl;

    // loop over rows
    for(auto idx_row = args.start_row; idx_row < args.num_rows; idx_row++) {

        // loop over columns
        for(auto idx_col = args.start_col; idx_col < args.num_cols; idx_col++) {

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
