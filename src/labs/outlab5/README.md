<div style="display: none">
\page outlab5 OutLab 05: LUP - Linear Equations Solver II
</div>

# OutLab 05: System of Linear Equations Solver, with Pivoting

Solves a system of linear equations solver, with pivoting. You can use the `--no-pivoting` flag to turn off pivoting.
You can use the `--alternate-method` flag to use a different way of performing LU factorization.
File based I/O is supported using JSON files.

<div style="display: none">[TOC]</div>

## Overview
1. [Building & Usage](#building--usage)
    - [Parameters](#parameters)
    - [General options](#general-options)
2. [Input Format](#input-format)
3. [Output Format](#output-format)
    - [Sample Outputs File](#sample-outputs-file)
4. [Example](#example)

## Building & Usage

The code has been built and tested on the `remote.eos.ncsu.edu` servers. It requires no additional
configuration except choosing the build target, and output file. Here is a repeatable script
to perform the build and run the `outlab5` target executable:

```bash
# Assuming cwd is the repo root:
#!/bin/bash

## Specify the build target
export BUILD_TARGET=outlab5

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export INPUT_FILE=./src/labs/outlab5/examples/outlab5_input_4.json
export OUTPUT_FILE=./src/labs/outlab5/examples/outlab5_output_4.json

## Execute
./build/bin/$BUILD_TARGET -i $INPUT_FILE -o $OUTPUT_FILE
```

### Parameters

- `--no-pivoting`: Do not perform partial pivoting
- `--alternate-method`: Use alternate factorization method
- `-n [ --order ] arg`: order of the square matrix (n is a natural number)
- `-i [ --input-json ] arg`: input JSON file containing the LU matrix and constants vector B
- `-o [ --output-json ] arg`: path for the output JSON file

### General options

- `-h [ --help ]`: Show this help message
- `-q [ --quiet ]`: Reduce verbosity
- `-p [ --precision ] arg (=15)`: Number of digits to represent long double
- `-P [ --profile ]`: Turn on profiling for performance comparison

## Input Format

The expected input json file requires the following fields:

### Sample Input File

```json
{
  "coefficients": [
    [1.0, 2.0, 3.0, 4.0, 5.0],
    [2.0, 3.0, 4.0, 5.0, 1.0],
    [3.0, 4.0, 5.0, 1.0, 2.0],
    [4.0, 5.0, 1.0, 2.0, 3.0],
    [5.0, 1.0, 2.0, 3.0, 4.0]
  ],
  "constants": [
    1.1,
    1.2,
    1.3,
    1.4,
    1.5
  ]
}
```

## Output Format

The output is written to a JSON file as well.

### Sample Outputs File

```json
{
    "inputs": {
       "coefficients": [
          [1.0, 2.0, 3.0, 4.0, 5.0],
          [2.0, 3.0, 4.0, 5.0, 1.0],
          [3.0, 4.0, 5.0, 1.0, 2.0],
          [4.0, 5.0, 1.0, 2.0, 3.0],
          [5.0, 1.0, 2.0, 3.0, 4.0]
       ],
        "constants": [
            1.1,
            1.2,
            1.3,
            1.4,
            1.5
        ],
        "n": 5
    },
    "intermediates": {
        "lower": [
            [1.0, 0.0, 0.0, 0.0, 0.0],
            [0.8, 1.0, 0.0, 0.0, 0.0],
            [0.6, 0.8095238095238095, 1.0, 0.0, 0.0],
            [0.4, 0.6190476190476191, 0.8333333333333334, 1.0, 0.0],
            [0.2, 0.42857142857142855, 0.6666666666666666, 0.875, 1.0]
        ],
        "permutation": [
            [0.0, 0.0, 0.0, 0.0, 1.0],
            [0.0, 0.0, 0.0, 1.0, 0.0],
            [0.0, 0.0, 1.0, 0.0, 0.0],
            [0.0, 1.0, 0.0, 0.0, 0.0],
            [1.0, 0.0, 0.0, 0.0, 0.0]
        ],
        "upper": [
            [5.0, 1.0, 2.0, 3.0, 4.0],
            [0.0, 4.2, -0.6, -0.4, -0.2],
            [0.0, 0.0, 4.285714285714286, -0.47619047619047616, -0.23809523809523808],
            [0.0, 0.0, 0.0, 4.444444444444445, -0.277777777777777],
            [0.0, 0.0, 0.0, 0.0, 4.6875]
        ]
    },
    "outputs": {
        "max_residual": 1.0842021724855044e-19,
        "residual": [
            -1.0842021724855044e-19,
            0.0,
            -1.0842021724855044e-19,
            1.0842021724855044e-19,
            0.0
        ],
        "solution": [
            0.16666666666666666,
            0.06666666666666665,
            0.0666666666666667,
            0.06666666666666665,
            0.0666666666666667
        ]
    }
}
```

## Example:

The following is an example of the program's output:

```shell
NE591: OutLab 05: Solving a system of linear equations using LUP factorization
Arjun Earthperson
09/29/2023
--------------------------------------------------------------------------------
compiler: GNU 8.5.0, boost: 106600 /usr/lib64/libboost_program_options.so
--------------------------------------------------------------------------------
Parameters:
  --no-pivoting                 Do not perform partial pivoting
  --alternate-method            Use alternate factorization method
  -n [ --order ] arg (=0)       = order of the square matrix (natural number)
  -i [ --input-json ] arg       = input JSON containing L, U, and b
  -o [ --output-json ] arg      = path for the output JSON

General options:
  -h [ --help ]                 = Show this help message
  -q [ --quiet ]                = Reduce verbosity
  -p [ --precision ] arg (=15)  = Number of digits to represent long double
  -P [ --profile ]              = Turn on profiling for performance comparison

--------------------------------------------------------------------------------
			Precision in digits:  default: 6, maximum: 19, current: 6
--------------------------------------------------------------------------------
Warning: File already exists at path, will be overwritten 
--------------------------------------------------------------------------------
                                     Inputs
--------------------------------------------------------------------------------
	Matrix order,        n: 0
	Input JSON,          i: ../sample_input_4.json
	Output JSON,         o: sample_output_4.json
	Use Pivoting          : Yes
	Use Alternate Method  : No
--------------------------------------------------------------------------------
Reading matrix order (n) from input matrix dimensions: 5
--------------------------------------------------------------------------------
Coefficient Matrix (A):
--------------------------------------------------------------------------------
    1.000000e+00    2.000000e+00    3.000000e+00    4.000000e+00    5.000000e+00
    2.000000e+00    3.000000e+00    4.000000e+00    5.000000e+00    1.000000e+00
    3.000000e+00    4.000000e+00    5.000000e+00    1.000000e+00    2.000000e+00
    4.000000e+00    5.000000e+00    1.000000e+00    2.000000e+00    3.000000e+00
    5.000000e+00    1.000000e+00    2.000000e+00    3.000000e+00    4.000000e+00
--------------------------------------------------------------------------------
Constants Vector (b):
--------------------------------------------------------------------------------
    1.100000e+00    1.200000e+00    1.300000e+00    1.400000e+00    1.500000e+00
--------------------------------------------------------------------------------
Lower Triangular Matrix (L):
--------------------------------------------------------------------------------
    1.000000e+00    0.000000e+00    0.000000e+00    0.000000e+00    0.000000e+00
    8.000000e-01    1.000000e+00    0.000000e+00    0.000000e+00    0.000000e+00
    6.000000e-01    8.095238e-01    1.000000e+00    0.000000e+00    0.000000e+00
    4.000000e-01    6.190476e-01    8.333333e-01    1.000000e+00    0.000000e+00
    2.000000e-01    4.285714e-01    6.666667e-01    8.750000e-01    1.000000e+00
--------------------------------------------------------------------------------
Upper Triangular Matrix (U):
--------------------------------------------------------------------------------
    5.000000e+00    1.000000e+00    2.000000e+00    3.000000e+00    4.000000e+00
    0.000000e+00    4.200000e+00   -6.000000e-01   -4.000000e-01   -2.000000e-01
    0.000000e+00    0.000000e+00    4.285714e+00   -4.761905e-01   -2.380952e-01
    0.000000e+00    0.000000e+00    0.000000e+00    4.444444e+00   -2.777778e-01
    0.000000e+00    0.000000e+00    0.000000e+00    0.000000e+00    4.687500e+00
--------------------------------------------------------------------------------
Permutation Matrix (P):
--------------------------------------------------------------------------------
    0.000000e+00    0.000000e+00    0.000000e+00    0.000000e+00    1.000000e+00
    0.000000e+00    0.000000e+00    0.000000e+00    1.000000e+00    0.000000e+00
    0.000000e+00    0.000000e+00    1.000000e+00    0.000000e+00    0.000000e+00
    0.000000e+00    1.000000e+00    0.000000e+00    0.000000e+00    0.000000e+00
    1.000000e+00    0.000000e+00    0.000000e+00    0.000000e+00    0.000000e+00
--------------------------------------------------------------------------------
Permuted constants (Pb = P * b):
--------------------------------------------------------------------------------
    1.500000e+00    1.400000e+00    1.300000e+00    1.200000e+00    1.100000e+00
--------------------------------------------------------------------------------
Intermediate vector (y), where (Ly = Pb):
--------------------------------------------------------------------------------
    1.500000e+00    2.000000e-01    2.380952e-01    2.777778e-01    3.125000e-01
--------------------------------------------------------------------------------
Solution vector (x), where (Ux = y):
--------------------------------------------------------------------------------
    1.666667e-01    6.666667e-02    6.666667e-02    6.666667e-02    6.666667e-02
--------------------------------------------------------------------------------
Residual vector (r = b - Ax) :
--------------------------------------------------------------------------------
   -1.084202e-19    0.000000e+00   -1.084202e-19    1.084202e-19    0.000000e+00
--------------------------------------------------------------------------------
Max Residual abs(r): 1.0842021724855044340e-19
--------------------------------------------------------------------------------

Process finished with exit code 0
```
