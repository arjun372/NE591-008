<div style="display: none">
\page InLab 10: Preconditioned Conjugate Gradient Method
</div>

# InLab 08: Iterative Linear Equation Solvers

File based I/O is supported using JSON files.

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
to perform the build and run the `inlab10` target executable:

```bash
# Assuming cwd is the repo root:
#!/bin/bash

## Specify the build target
export BUILD_TARGET=inlab10

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export INPUT_FILE=./src/labs/inlab10/examples/inlab10_input.json
export OUTPUT_FILE=./src/labs/inlab10/examples/inlab10_output.json

## Execute
./build/bin/$BUILD_TARGET -i $INPUT_FILE -o $OUTPUT_FILE
```

### Parameters

- `-t [ --threshold ] arg     `: iterative convergence threshold [𝜀 > 0]
- `-k [ --max-iterations ] arg`: maximum number of iterations [n ∈ ℕ]
- `-i [ --input-json ] arg`: input JSON containing A, and b
- `-o [ --output-json ] arg`: path for the output JSON

### General options

- `-h [ --help ]`: Show this help message
- `-q [ --quiet ]`: Reduce verbosity
- `-p [ --precision ] arg (=15)`: Number of digits to represent long double
- `-P [ --profile ]`: Turn on performance profiling

## Parameters Format

The expected input json file requires the following fields:

### Sample Parameters File

```json
{
   "max-iterations": 500,
   "stopping-criterion": 1e-8,
   "coefficients": [
      [ 36.1, -9.00, -9.00,  0.00],
      [-9.00,  36.1,  0.00, -9.00],
      [-9.00,  0.00,  36.1, -9.00],
      [ 0.00, -9.00, -9.00,  36.1]
   ],
   "constants": [1.00, 1.00, 1.00, 1.00]
}
```

## Output Format

The output is written to a JSON file as well.

### Sample Outputs File

```json
{
   "inputs": {
      "coefficients": [
         [
            36.1,
            -9.0,
            -9.0,
            0.0
         ],
         [
            -9.0,
            36.1,
            0.0,
            -9.0
         ],
         [
            -9.0,
            0.0,
            36.1,
            -9.0
         ],
         [
            0.0,
            -9.0,
            -9.0,
            36.1
         ]
      ],
      "constants": [
         1.0,
         1.0,
         1.0,
         1.0
      ],
      "max-iterations": 500,
      "n": 4,
      "stopping-criterion": 1e-08
   },
   "outputs": {
      "Cy": [
         2.0,
         2.0,
         2.0,
         2.0
      ],
      "benchmark": {
         "mean": 2821.2421,
         "p5th": 2757.0,
         "p95th": 2793.0,
         "samples": 10000
      },
      "y + z": [
         3.0,
         3.0,
         3.0,
         3.0
      ],
      "y^T•(A•z)": 144.8,
      "y^T•z": 8.0
   }
}
```

## Example:

The following is an example of the program's output:

```shell
NE591: InLab 10: Matrix-Vector Operations Setup
Arjun Earthperson
10/27/2023
--------------------------------------------------------------------------------
using 128-bit floats
compiler: GNU 8.5.0
boost: 106600 /usr/lib64/libboost_program_options.so;/usr/lib64/libboost_serialization.so
--------------------------------------------------------------------------------
Parameters:

Solver Options:
  -e [ --stopping-criterion ]      = stopping criterion [+ve R]
  -k [ --max-iterations ]          = maximum iterations [k ∈ ℕ]

File I/O Options:
  -i [ --input-json ] arg          = input JSON containing n
  -o [ --output-json ] arg         = path for the output JSON

Performance Benchmarking:
  -R [ --bench-runs ] arg (=1)     = <R> runs to perform
  -T [ --bench-timeout ] arg (=0)  = Timeout after <T> seconds [0=never]
  -B [ --bench ]                   = Run performance benchmarks

General options:
  -h [ --help ]                    = Show this help message
  -q [ --quiet ]                   = Reduce verbosity
  -p [ --precision ] arg (=15)     = Number of digits to represent long double

--------------------------------------------------------------------------------
			Precision in digits:  default: 6, maximum: 19, current: 15
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
                                     Inputs
--------------------------------------------------------------------------------
	Input JSON,              i: ../outlab10_input.json
	Output JSON,             o: ../outlab10_output.json
	Stopping Criterion,      e: 1e-08
	Maximum Iterations,      k: 500
--------------------------------------------------------------------------------
Input coefficients matrix A is square.
Input coefficients matrix A is symmetric.
Input coefficients matrix A is positive definite.
--------------------------------------------------------------------------------
Coefficient Matrix (A):
--------------------------------------------------------------------------------
    3.610000000000000e+01   -9.000000000000000e+00   -9.000000000000000e+00    0.000000000000000e+00
   -9.000000000000000e+00    3.610000000000000e+01    0.000000000000000e+00   -9.000000000000000e+00
   -9.000000000000000e+00    0.000000000000000e+00    3.610000000000000e+01   -9.000000000000000e+00
    0.000000000000000e+00   -9.000000000000000e+00   -9.000000000000000e+00    3.610000000000000e+01
--------------------------------------------------------------------------------
Constants Vector (b):
--------------------------------------------------------------------------------
    1.000000000000000e+00    1.000000000000000e+00    1.000000000000000e+00    1.000000000000000e+00
:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[10000/10000] : Matrix-Vector Operations
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 2.82e+07 :::::::: VARIANCE: 1.04e+06 :::::::: MEDIAN: 2.77e+03 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {2.74e+03, 3.98e+04} : (2.82e+03 ± 1.02e+03) : [2.76e+03, 2.79e+03] :::::
:::::: Estimated maximum allocated memory [bytes]: 760 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

[    Cy   ]:     2.000000000000000e+00    2.000000000000000e+00    2.000000000000000e+00    2.000000000000000e+00
[  y + z  ]:     3.000000000000000e+00    3.000000000000000e+00    3.000000000000000e+00    3.000000000000000e+00
[  y^T•z  ]: 8.000000000000000e+00
[y^T•(A•z)]: 1.448000000000000e+02

JSON data has been written to ../outlab10_output.json

Process finished with exit code 0
```
