<div style="display: none">
\page OutLab 12: Inverse Power Iteration Method
</div>

OutLab 12: Inverse Power Iteration Method

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
to perform the build and run the `outlab12` target executable:

```bash
# Assuming cwd is the repo root:
#!/bin/bash

## Specify the build target
export BUILD_TARGET=outlab12

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export OUTPUT_FILE=./src/labs/outlab12/examples/outlab12_output.json

## Execute
./build/bin/$BUILD_TARGET -g -o $OUTPUT_FILE
```

### Parameters

- `-t [ --convergence_threshold ] arg     `: iterative convergence convergence_threshold [𝜀 > 0]
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
  "convergence_threshold": 1e-4
}
```

## Output Format

The output is written to a JSON file as well.

### Sample Outputs File

```json
{
   "inverse-power-iteration": {
      "benchmark": {
         "mean": 71070.14,
         "p5th": 65543.25,
         "p95th": 83677.75,
         "samples": 100
      },
      "solution": {
         "converged": true,
         "eigenvalue": 5.617862032888341,
         "iterations": 12,
         "iterative-error": {
            "eigenvalue": 0.00026600375184247446,
            "x": 4.73496412487924e-05
         },
         "residual": [
            0.009352377667662094,
            0.001073025507593375,
            0.003015940623797602,
            -0.008716994010167288
         ],
         "residual-∞-norm": 0.009352377667662094,
         "x": [
            0.5970258552065718,
            -0.37204730392844737,
            -0.6032105375306189,
            0.37586963067858625
         ]
      }
   },
}
```

## Example:

The following is an example of the program's output:

```shell
NE591: OutLab 12: Inverse Power Iteration Method
Arjun Earthperson
11/10/2023
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
  --lambda arg                          = guess for eigenvalue
  --use-direct                          = use the direct PI method
  --use-rayleigh                        = use the Rayleigh Quotient PI method
  --use-inverse                         = use the inverse PI method

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
Would you like to use the Use the inverse PI method? [YES/no]: y
y
Enter a value for lambda:-10
-10
Would you like to use the the direct PI method? [YES/no]: n
n
Would you like to use the the Rayleigh Quotient PI method? [YES/no]: n
n
--------------------------------------------------------------------------------
                                     Inputs
--------------------------------------------------------------------------------
	Use inverse PI method     : Yes
	Use direct PI method      : Yes
	Use Rayleigh PI method    : Yes
	Generate A               g: No
	Input JSON (for A),      i:  ../inlab12_input.json
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
::::::::::::::::::::::::::::::: Solver Parameters ::::::::::::::::::::::::::::::
--------------------------------------------------------------------------------
Initial Guess
--------------------------------------------------------------------------------
-3.451901831525128e-01   -3.999578137192380e-01   -1.776804031623526e-01   4.288425371722089e-01    
--------------------------------------------------------------------------------
Size (n):                4
Convergence Threshold:   1.000000000000000e-04
Relaxation Factor:       nan
Max Iterations:          50
:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[100/100] : Rayleigh Quotient Power Iteration Method
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 1.11e+07 :::::::: VARIANCE: 2.50e+09 :::::::: MEDIAN: 1.02e+05 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {9.90e+04, 5.78e+05} : (1.11e+05 ± 5.00e+04) : [9.99e+04, 1.20e+05] :::::
:::::: Estimated maximum allocated memory [bytes]: 736 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:::::: Converged        :     Yes
:::::: Iterations       :     11
:::::: Iterative Error  :     5.83e-05
:::::: Iterate Vector x : 5.94e-01    -3.72e-01   -6.04e-01   3.78e-01    
:::::: Eigenvalue       :     5.62e+00
:::::: Eigenvalue Error :     3.27e-04
:::::: Residual ∞-Norm  :     1.48e-02
:::::: Residual         : 1.48e-02    2.22e-03    4.38e-03    -1.41e-02   


--------------------------------------------------------------------------------
JSON data has been written to /dev/stdout

Process finished with exit code 0
```
