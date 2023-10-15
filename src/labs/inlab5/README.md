<div style="display: none">
\page inlab5 InLab 05: LUP - Linear Equations Solver I
</div>

# InLab 05: LU Factorization with Pivoting

LU factorization with pivoting. Factorization using forward and backward substitution. File based I/O is supported using
JSON files.

<div style="display: none">[TOC]</div>

## Overview

1. [Building & Usage](#building--usage)
    - [Parameters](#parameters)
    - [General options](#general-options)
2. [Parameters Format](#input-format)
3. [Output Format](#output-format)
    - [Sample Outputs File](#sample-outputs-file)
4. [Example](#example)

## Building & Usage

The code has been built and tested on the `remote.eos.ncsu.edu` servers. It requires no additional
configuration except choosing the build target, and output file. Here is a repeatable script
to perform the build and run the `inlab5` target executable:

```bash
# Assuming cwd is the repo root:
#!/bin/bash

## Specify the build target
export BUILD_TARGET=inlab5

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export INPUT_FILE=./src/labs/inlab5/examples/sample_input.json
export OUTPUT_FILE=./src/labs/inlab5/examples/sample_output.json

## Execute
./build/bin/$BUILD_TARGET -i $INPUT_FILE -o $OUTPUT_FILE
```

### Parameters

- `-n [ --order ] arg`: order of the square matrix (n is a natural number)
- `-i [ --input-json ] arg`: input JSON file containing the LU matrix and constants vector B
- `-o [ --output-json ] arg`: path for the output JSON file

### General options

- `-h [ --help ]`: Show this help message
- `-q [ --quiet ]`: Reduce verbosity
- `-p [ --precision ] arg (=15)`: Number of digits to represent long double
- `-P [ --profile ]`: Turn on profiling for performance comparison

## Parameters Format

The expected input json file requires the following fields:

### Sample Parameters File

```json
{
   "lower": [
      [1.00,  0.00,  0.00,  0.00,  0.00],
      [0.80,  1.00,  0.00,  0.00,  0.00],
      [0.60,  0.81,  1.00,  0.00,  0.00],
      [0.40,  0.62,  0.83,  1.00,  0.00],
      [0.20,  0.43,  0.67,  0.88,  1.00]
   ],
   "upper": [
      [5.00,  1.00,  2.00,  3.00,  4.00],
      [0.00,  4.20, -0.60, -0.40, -0.20],
      [0.00,  0.00,  4.29, -0.48, -0.24],
      [0.00,  0.00,  0.00,  4.44, -0.28],
      [0.00,  0.00,  0.00,  0.00,  4.69]
   ],
   "permutation": [
      [0.00,  0.00,  0.00,  0.00,  1.00],
      [0.00,  0.00,  0.00,  1.00,  0.00],
      [0.00,  0.00,  1.00,  0.00,  0.00],
      [0.00,  1.00,  0.00,  0.00,  0.00],
      [1.00,  0.00,  0.00,  0.00,  0.00]
   ],
   "constants": [
      1.10,
      1.20,
      1.30,
      1.40,
      1.50
   ]
}
```

## Output Format

The output is written to a JSON file as well.

### Sample Outputs File

```json
{
   "outputs": {
      "solution": [
         0.16708948516679858,
         0.06665254144358618,
         0.06665245686141212,
         0.06687777598494013,
         0.06599044776119405
      ]
   }
}
```

## Example:

The following is an example of the program's output:

```shell
InLab 05: Linear equations with permutation matrix, forward, back substitution
Arjun Earthperson
09/22/2023
--------------------------------------------------------------------------------
compiler: GNU 8.5.0, boost: 106600 /usr/lib64/libboost_program_options.so
--------------------------------------------------------------------------------
Parameters:
  -n [ --order ] arg (=0)       = order of the square matrix (n is a natural 
                                number)
  -i [ --input-json ] arg       = input JSON file containing the LU matrix and 
                                constants vector B
  -o [ --output-json ] arg      = path for the output JSON file

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
	Matrix order,  n: 0
	Parameters JSON,    i: ../sample_input.json
	Output JSON,   o: output.json
--------------------------------------------------------------------------------
Reading matrix order (n) from input matrix dimensions: 5
--------------------------------------------------------------------------------
Lower Triangular Matrix (L):
--------------------------------------------------------------------------------
    1.000000e+00    0.000000e+00    0.000000e+00    0.000000e+00    0.000000e+00
    8.000000e-01    1.000000e+00    0.000000e+00    0.000000e+00    0.000000e+00
    6.000000e-01    8.100000e-01    1.000000e+00    0.000000e+00    0.000000e+00
    4.000000e-01    6.200000e-01    8.300000e-01    1.000000e+00    0.000000e+00
    2.000000e-01    4.300000e-01    6.700000e-01    8.800000e-01    1.000000e+00
--------------------------------------------------------------------------------
Upper Triangular Matrix (U):
--------------------------------------------------------------------------------
    5.000000e+00    1.000000e+00    2.000000e+00    3.000000e+00    4.000000e+00
    0.000000e+00    4.200000e+00   -6.000000e-01   -4.000000e-01   -2.000000e-01
    0.000000e+00    0.000000e+00    4.290000e+00   -4.800000e-01   -2.400000e-01
    0.000000e+00    0.000000e+00    0.000000e+00    4.440000e+00   -2.800000e-01
    0.000000e+00    0.000000e+00    0.000000e+00    0.000000e+00    4.690000e+00
--------------------------------------------------------------------------------
Permutation Matrix (P):
--------------------------------------------------------------------------------
    0.000000e+00    0.000000e+00    0.000000e+00    0.000000e+00    1.000000e+00
    0.000000e+00    0.000000e+00    0.000000e+00    1.000000e+00    0.000000e+00
    0.000000e+00    0.000000e+00    1.000000e+00    0.000000e+00    0.000000e+00
    0.000000e+00    1.000000e+00    0.000000e+00    0.000000e+00    0.000000e+00
    1.000000e+00    0.000000e+00    0.000000e+00    0.000000e+00    0.000000e+00
--------------------------------------------------------------------------------
Constants Vector (b):
--------------------------------------------------------------------------------
    1.100000e+00    1.200000e+00    1.300000e+00    1.400000e+00    1.500000e+00
--------------------------------------------------------------------------------
LU Matrix:
--------------------------------------------------------------------------------
    5.000000e+00    1.000000e+00    2.000000e+00    3.000000e+00    4.000000e+00
    8.000000e-01    4.200000e+00   -6.000000e-01   -4.000000e-01   -2.000000e-01
    6.000000e-01    8.100000e-01    4.290000e+00   -4.800000e-01   -2.400000e-01
    4.000000e-01    6.200000e-01    8.300000e-01    4.440000e+00   -2.800000e-01
    2.000000e-01    4.300000e-01    6.700000e-01    8.800000e-01    4.690000e+00
--------------------------------------------------------------------------------
Permuted constants Pb = P * b:
--------------------------------------------------------------------------------
    1.500000e+00    1.400000e+00    1.300000e+00    1.200000e+00    1.100000e+00
--------------------------------------------------------------------------------
Intermediate vector y = inv(L) * b:
--------------------------------------------------------------------------------
    1.500000e+00    2.000000e-01    2.380000e-01    2.784600e-01    3.094952e-01
--------------------------------------------------------------------------------
Solution vector (x):
--------------------------------------------------------------------------------
    1.670895e-01    6.665254e-02    6.665246e-02    6.687778e-02    6.599045e-02
--------------------------------------------------------------------------------
JSON data has been written to output.json

Process finished with exit code 0
```
