<div style="display: none">
\page OutLab 10: Conjugate Gradient Method
</div>

OutLab 10: Conjugate Gradient Method

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
to perform the build and run the `outlab10` target executable:

```bash
# Assuming cwd is the repo root:
#!/bin/bash

## Specify the build target
export BUILD_TARGET=outlab10

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export OUTPUT_FILE=./src/labs/outlab10/examples/outlab10_output.json

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
   "outputs": {
      "CG": {
         "benchmark": {
            "mean": 1482.32,
            "p5th": 1120.9,
            "p95th": 1308.8,
            "samples": 100
         },
         "solution": {
            "converged": true,
            "iterations": {
               "actual": 2
            },
            "iterative-error": {
               "actual": 2.4977230927471127e-05
            },
            "solution": [
               0.0,
               0.0,
               0.0,
               0.0
            ]
         }
      },
      "SOR": {
         "benchmark": {
            "mean": 2334.67,
            "p5th": 1895.95,
            "p95th": 1987.15,
            "samples": 100
         },
         "solution": {
            "converged": true,
            "iterations": {
               "actual": 4
            },
            "iterative-error": {
               "actual": 9.662710781447525e-05
            },
            "solution": [
               4.689765580182186,
               -4.505558137202693,
               1.0297059892341907,
               2.960941254070212
            ]
         }
      }
   }
}
```

## Example:

The following is an example of the program's output:

```shell
NE591: OutLab 10: Conjugate Gradient Method
Arjun Earthperson
10/27/2023
--------------------------------------------------------------------------------
using 128-bit floats
compiler: GNU 8.5.0
boost: 106600 /usr/lib64/libboost_program_options.so;/usr/lib64/libboost_serialization.so
--------------------------------------------------------------------------------
Parameters:

Solver Options:
  -t [ --threshold ] arg           = convergence threshold [𝜀 > 0]
  -k [ --max-iterations ] arg      = maximum iterations [n ∈ ℕ]
  -n [ --order ] arg               = order of the square matrix [n ∈ ℕ]

File I/O Options:
  -i [ --input-json ] arg          = input JSON containing A, and b
  -g [ --generate ]                = Generate A,b ignoring input-json
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
Error: No output JSON filepath provided.

Enter output file path (file extension is .json): ../results_n_4.json
../results_n_4.json
Enter a value for order:4
4
--------------------------------------------------------------------------------
                                     Inputs
--------------------------------------------------------------------------------
	Generate A,b,            g: Yes
	Input JSON (for A, b),   i: [IGNORED] None provided
	Output JSON (for x),     o: ../results_n_4.json
	Convergence Threshold,   𝜀: 0.0001
	Max iterations,          k: 5000
	Matrix order,            n: 4--------------------------------------------------------------------------------
Input coefficients matrix A is square.
Warning: Input coefficients matrix A is not symmetric.
Input coefficients matrix A is diagonally dominant.
Input coefficients matrix A is positive definite.
:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[100/100] : CG Method
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 1.48e+05 :::::::: VARIANCE: 9.30e+06 :::::::: MEDIAN: 1.14e+03 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {1.11e+03, 3.17e+04} : (1.48e+03 ± 3.05e+03) : [1.12e+03, 1.31e+03] :::::
:::::: Estimated maximum allocated memory [bytes]: 0 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:::::: Converged       : Yes
:::::: Iterations      : 2
:::::: Iterative Error : 2.50e-05

--------------------------------------------------------------------------------
:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[100/100] : SOR Method
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 2.33e+05 :::::::: VARIANCE: 1.58e+07 :::::::: MEDIAN: 1.92e+03 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {1.89e+03, 4.19e+04} : (2.33e+03 ± 3.98e+03) : [1.90e+03, 1.99e+03] :::::
:::::: Estimated maximum allocated memory [bytes]: 0 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:::::: Converged       : Yes
:::::: Iterations      : 4
:::::: Iterative Error : 9.66e-05

--------------------------------------------------------------------------------
JSON data has been written to ../results_n_4.json

Process finished with exit code 0
```
