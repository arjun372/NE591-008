<div style="display: none">
\page OutLab 11: Power Iterations Method
</div>

OutLab 11: Power Iterations Method

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
to perform the build and run the `outlab11` target executable:

```bash
# Assuming cwd is the repo root:
#!/bin/bash

## Specify the build target
export BUILD_TARGET=outlab11

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export OUTPUT_FILE=./src/labs/outlab11/examples/outlab11_output.json

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

But if you like, you can also use:

```json
{
  "coefficients": [
    [4, -1, -1, 0],
    [-1, 3, 0, -1],
    [-1, 0, 4, -1],
    [0, -1, -1, 3]
  ],
  "max-iterations": 50,
  "threshold": 1e-4
}
```

## Output Format

The output is written to a JSON file as well.

### Sample Outputs File

```json
{
   "power_iteration": {
      "benchmark": {
         "mean": 44127.14,
         "p5th": 38376.6,
         "p95th": 70051.6,
         "samples": 100
      },
      "solution": {
         "converged": true,
         "eigenvalue": 5.618001047771479,
         "iterations": {
            "actual": 14
         },
         "iterative-error": {
            "actual": 5.032033910499058e-05
         },
         "solution": [
            0.5997197745151289,
            -0.37175794139874857,
            -0.6022848539235147,
            0.37334324662007495
         ]
      }
   }
}
```

## Example:

The following is an example of the program's output:

```shell
NE591: OutLab 11: Power Iterations Method
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
  -i [ --input-json ] arg               = input JSON containing A
  -g [ --generate ]                     = Generate A ignoring input-json
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
--------------------------------------------------------------------------------
                                     Inputs
--------------------------------------------------------------------------------
	Generate A               g: No
	Input JSON (for A),      i:  ../outlab11_input.json
	Output JSON (for x),     o: /dev/stdout
	Convergence Threshold,   𝜀: 0.0001
	Max iterations,          k: 50
	Matrix order,            n: None provided, will be inferred from input JSON
--------------------------------------------------------------------------------
Reading matrix order (n) from input matrix dimensions: 4
Input coefficients matrix A is square.
Input coefficients matrix A is symmetric.
Input coefficients matrix A is diagonally dominant.
Input coefficients matrix A is positive definite.
--------------------------------------------------------------------------------
Coefficient Matrix (A):
--------------------------------------------------------------------------------
    4.000000000000000e+00   -1.000000000000000e+00   -1.000000000000000e+00    0.000000000000000e+00
   -1.000000000000000e+00    3.000000000000000e+00    0.000000000000000e+00   -1.000000000000000e+00
   -1.000000000000000e+00    0.000000000000000e+00    4.000000000000000e+00   -1.000000000000000e+00
    0.000000000000000e+00   -1.000000000000000e+00   -1.000000000000000e+00    3.000000000000000e+00
--------------------------------------------------------------------------------
:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[100/100] : Power Iterations Method
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 4.41e+06 :::::::: VARIANCE: 8.62e+07 :::::::: MEDIAN: 4.03e+04 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {3.84e+04, 7.28e+04} : (4.41e+04 ± 9.28e+03) : [3.84e+04, 7.01e+04] :::::
:::::: Estimated maximum allocated memory [bytes]: 512 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:::::: Converged       : Yes
:::::: Eigenvalue      : 5.62e+00
:::::: Iterations      : 14
:::::: Iterative Error : 5.03e-05
:::::: Iterate Vector x:     6.00e-01   -3.72e-01   -6.02e-01    3.73e-01


--------------------------------------------------------------------------------
```
