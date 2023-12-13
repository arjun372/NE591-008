<div style="display: none">
\page InLab 11: Jacobi Preconditioned Conjugate Gradient Method
</div>

InLab 11: Jacobi Preconditioned Conjugate Gradient Method

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
to perform the build and run the `inlab11` target executable:

```bash
# Assuming cwd is the repo root:
#!/bin/bash

## Specify the build target
export BUILD_TARGET=inlab11

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export OUTPUT_FILE=./src/labs/inlab11/examples/inlab11_output.json

## Execute
./build/bin/$BUILD_TARGET -g -o $OUTPUT_FILE
```

### Parameters

- `-t [ --threshold ] arg     `: iterative convergence threshold [𝜀 > 0]
- `-g [ --generate ]`: Generate A,b ignoring input-json
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

No input file is needed. You can generate a random matrix using the `-g` or `--generate` flag.

## Output Format

The output is written to a JSON file as well.

### Sample Outputs File

```json
{
   "inputs": {
      "coefficients": [
         [
            1.6533097453783512,
            0.12124819341567544,
            0.09817691918737048
         ],
         [
            0.12124819341567544,
            10.819973685070268,
            0.2423287722448861
         ],
         [
            0.09817691918737048,
            0.2423287722448861,
            2.704082657092818
         ]
      ],
      "constants": [
         7.177451870317513,
         -49.3191090773808,
         2.3416251567267343
      ],
      "known-solution": [
         4.615027360996984,
         -4.634814194647449,
         1.1137547197128228
      ],
      "max-iterations": 5000,
      "methods": [],
      "order": 3,
      "threshold": 0.0001
   },
   "outputs": {
      "CG": {
         "benchmark": {
            "mean": 1899.74,
            "p5th": 1896.0,
            "p95th": 1897.0,
            "samples": 100
         },
         "solution": {
            "converged": true,
            "iterations": {
               "actual": 2
            },
            "iterative-error": {
               "actual": 2.6748964287537258e-17
            },
            "solution": [
               4.615027360996984,
               -4.634814194647449,
               1.1137547197128228
            ]
         }
      },
      "JPCG": {
         "benchmark": {
            "mean": 2325.95,
            "p5th": 2292.0,
            "p95th": 2371.2,
            "samples": 100
         },
         "solution": {
            "converged": true,
            "iterations": {
               "actual": 2
            },
            "iterative-error": {
               "actual": 3.771964891759634e-21
            },
            "solution": [
               4.615027360996984,
               -4.634814194647449,
               1.1137547197128228
            ]
         }
      }
   }
}
```

## Example:

The following is an example of the program's output:

```shell
NE591: InLab 11: Jacobi Preconditioned Conjugate Gradient Method
Arjun Earthperson
11/03/2023
--------------------------------------------------------------------------------
using 128-bit floats
compiler: GNU 8.5.0
boost: 106600 /usr/lib64/libboost_program_options.so;/usr/lib64/libboost_serialization.so
--------------------------------------------------------------------------------
Parameters:

Solver Options:
  -t [ --threshold ] arg                = convergence threshold [𝜀 > 0]
  -k [ --max-iterations ] arg           = maximum iterations [n ∈ ℕ]
  -n [ --order ] arg                    = order of the square matrix [n ∈ 
                                        ℕ]

File I/O Options:
  -i [ --input-json ] arg               = input JSON containing A, and b
  -g [ --generate ]                     = Generate A,b ignoring input-json
  -o [ --output-json ] arg (=/dev/stdout)
                                        = path for the output JSON

Performance Benchmarking:
  -R [ --bench-runs ] arg (=1)          = <R> runs to perform
  -T [ --bench-timeout ] arg (=0)       = Timeout after <T> seconds [0=never]
  -B [ --bench ]                        = Run performance benchmarks

General options:
  -h [ --help ]                         = Show this help message
  -q [ --quiet ]                        = Reduce verbosity
  -p [ --precision ] arg (=15)          = Number of digits to represent long 
                                        double

--------------------------------------------------------------------------------
			Precision in digits:  default: 6, maximum: 19, current: 15
--------------------------------------------------------------------------------
Enter a value for order:2048
2048
--------------------------------------------------------------------------------
                                     Inputs
--------------------------------------------------------------------------------
	Generate A,b,            g: Yes
	Input JSON (for A, b),   i: [IGNORED] None provided
	Output JSON (for x),     o: ../results.json
	Convergence Threshold,   𝜀: 0.0001
	Max iterations,          k: 5000
	Matrix order,            n: 2048
--------------------------------------------------------------------------------
Input coefficients matrix A is square.
Input coefficients matrix A is symmetric.
Input coefficients matrix A is diagonally dominant.
Input coefficients matrix A is positive definite.
:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[100/100] : Jacobi Preconditioned Conjugate Gradient Method
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 8.19e+09 :::::::: VARIANCE: 4.58e+11 :::::::: MEDIAN: 8.17e+07 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {8.15e+07, 8.42e+07} : (8.19e+07 ± 6.76e+05) : [8.16e+07, 8.41e+07] :::::
:::::: Estimated maximum allocated memory [bytes]: 328032 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:::::: Converged       : Yes
:::::: Iterations      : 4
:::::: Iterative Error : 3.57e-06

--------------------------------------------------------------------------------
:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[100/100] : Conjugate Gradient Method
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 9.55e+09 :::::::: VARIANCE: 5.07e+11 :::::::: MEDIAN: 9.53e+07 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {9.52e+07, 9.78e+07} : (9.55e+07 ± 7.12e+05) : [9.52e+07, 9.78e+07] :::::
:::::: Estimated maximum allocated memory [bytes]: 262432 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:::::: Converged       : Yes
:::::: Iterations      : 5
:::::: Iterative Error : 2.54e-05

--------------------------------------------------------------------------------
JSON data has been written to ../results.json

Process finished with exit code 0
```
