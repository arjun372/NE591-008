<div style="display: none">
\page outlab4 OutLab 04: LU - Linear Equations Solver II
</div>

# OutLab 04: System of Linear Equations Solver

Solves a system of linear equations by performing LU factorization, and solving the LU matrix using forward and backward
substitution. File based I/O is supported using JSON files.

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
to perform the build and run the `outlab4` target executable:

```bash
# Assuming cwd is the repo root:
#!/bin/bash

## Specify the build target
export BUILD_TARGET=outlab4

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export INPUT_FILE=./src/labs/outlab4/examples/sample_input_1.json
export OUTPUT_FILE=./src/labs/outlab4/examples/sample_output_1.json

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

## Input Format

The expected input json file requires the following fields:

### Sample Input File

```json
{
   "coefficients": [
      [1.0, 1.0, 2.0],
      [2.0, 4.0, 7.0],
      [3.0, 7.0, 15.0]
   ],
   "constants": [7.0, 21.0, 38.0]
}
```

## Output Format

The output is written to a JSON file as well.

### Sample Outputs File

```json
{
   "outputs": {
      "residual": [
         0.0,
         0.0,
         0.0
      ],
      "solution": [
         3.0,
         2.0,
         1.0
      ]
   }
}
```

## Example:

The following is an example of the program's output:

```shell
/tmp/tmp.xEkz05Cjga/cmake-build-debug-hermes-ne591/bin/outlab4 -i ../sample_input_1.json -o sample_output_1.json

NE591: OutLab 04: Solving a system of linear equations using LU factorization
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
			Precision in digits:  default: 6, maximum: 19, current: 15
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
                                     Inputs
--------------------------------------------------------------------------------
	Matrix order,  n: 0
	Input JSON,    i: ../sample_input_1.json
	Output JSON,   o: sample_output_1.json
--------------------------------------------------------------------------------
Reading matrix order (n) from input matrix dimensions: 3
--------------------------------------------------------------------------------
Coefficient Matrix (A):
--------------------------------------------------------------------------------
1 1 2 
2 4 7 
3 7 15 
--------------------------------------------------------------------------------
Constants Vector (b):
--------------------------------------------------------------------------------
7 21 38 
--------------------------------------------------------------------------------
Lower Triangular Matrix (L):
--------------------------------------------------------------------------------
1 0 0 
2 1 0 
3 2 1 
--------------------------------------------------------------------------------
Upper Triangular Matrix (U):
--------------------------------------------------------------------------------
1 1 2 
0 2 3 
0 0 3 
--------------------------------------------------------------------------------
Factorized Matrix LU: 
--------------------------------------------------------------------------------
1 1 2 
2 2 3 
3 2 3 
--------------------------------------------------------------------------------
Intermediate vector y = inv(L) * b:
--------------------------------------------------------------------------------
7 7 3 
--------------------------------------------------------------------------------
Solution vector (x):
--------------------------------------------------------------------------------
3 2 1 
--------------------------------------------------------------------------------
Residual vector r=|b' - b| :
--------------------------------------------------------------------------------
0 0 0 
--------------------------------------------------------------------------------
JSON data has been written to sample_output_1.json

Process finished with exit code 0
```
