<div style="display: none">
\page outlab7 OutLab 07: Iterative Linear Equation Solvers
</div>

# OutLab 07: Iterative Linear Equation Solvers

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
to perform the build and run the `outlab7` target executable:

```bash
# Assuming cwd is the repo root:
#!/bin/bash

## Specify the build target
export BUILD_TARGET=outlab7

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export INPUT_FILE=./src/labs/outlab7/examples/outlab10_input.json
export OUTPUT_FILE=./src/labs/outlab7/examples/outlab7_output_1.json

## Execute
./build/bin/$BUILD_TARGET -i $INPUT_FILE -o $OUTPUT_FILE
```

### Parameters

- `-t [ --threshold ] arg     `: iterative convergence threshold [𝜀 > 0]
- `-k [ --max-iterations ] arg`: maximum number of iterations [n ∈ ℕ]
- `-n [ --order ] arg`: order of the square matrix [n ∈ ℕ]
- `-i [ --input-json ] arg`: input JSON containing A, and b
- `-o [ --output-json ] arg`: path for the output JSON
- `-w [ --relaxation-factor ] arg`: SOR weight, typical ω ∈ [0,2]
- `-g [ --generate ]`: Generate A,b ignoring input-json

### Solver Methods

- `--use-LUP`: Use LUP factorization
- `--use-point-jacobi`: Use the Point-Jacobi method
- `--use-gauss-seidel`: Use the Gauss-Seidel method
- `--use-SOR`: Use the SOR method
- `--use-SORJ`: Use the SOR Jacobi method
- `--use-SSOR`: [DISABLED] Use the symmetric SOR method

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
      "known-solution": [],
      "max-iterations": 5000,
      "methods": [
         "gauss-seidel"
      ],
      "order": 4,
      "relaxation-factor": 1.08,
      "threshold": 0.0001
   },
   "outputs": {
      "gauss-seidel": {
         "converged": true,
         "iterations": {
            "actual": 6,
            "maximum": 5000
         },
         "iterative-error": {
            "actual": 3.6891402111559554e-05,
            "maximum": 0.0001
         },
         "l2-error": 0.0,
         "max-residual": 0.00026517572756905696,
         "solution": [
            0.05523884269588214,
            0.05524374428049525,
            0.05524374428049525,
            0.055246188283903444
         ],
         "wall-time-ns": {
            "mean": 488.4,
            "samples": 52.437009830843714,
            "std": 52.437009830843714
         }
      },
   }
}
}
```

## Example:

The following is an example of the program's output:

```shell
NE591: OutLab 07: Solving a system of linear equations using iterative methods
Arjun Earthperson
10/06/2023
--------------------------------------------------------------------------------
compiler: GNU 8.5.0, boost: 106600 /usr/lib64/libboost_program_options.so
--------------------------------------------------------------------------------
Parameters:
  -t [ --threshold ] arg         = convergence threshold [𝜀 > 0]
  -k [ --max-iterations ] arg    = maximum iterations [n ∈ ℕ]
  -w [ --relaxation-factor ] arg = SOR weight, typical ω ∈ [0,2]
  -n [ --order ] arg             = order of the square matrix [n ∈ ℕ]
  -i [ --input-json ] arg        = input JSON containing A, and b
  -g [ --generate ]              = Generate A,b ignoring input-json
  -o [ --output-json ] arg       = path for the output JSON

Solver Methods:
  --use-LUP                      = Use LUP factorization
  --use-point-jacobi             = Use the Point-Jacobi method
  --use-gauss-seidel             = Use the Gauss-Seidel method
  --use-SOR                      = Use the SOR method
  --use-SORJ                     = Use the SOR Jacobi method
  --use-SSOR                     = [DISABLED] Use the symmetric SOR method

General options:
  -h [ --help ]                  = Show this help message
  -q [ --quiet ]                 = Reduce verbosity
  -p [ --precision ] arg (=15)   = Number of digits to represent long double
  -P [ --profile ]               = Turn on performance profiling

--------------------------------------------------------------------------------
			Precision in digits:  default: 6, maximum: 19, current: 2
--------------------------------------------------------------------------------
Warning: File already exists at path, will be overwritten 
--------------------------------------------------------------------------------
                                     Inputs
--------------------------------------------------------------------------------
	Generate A,b,            g: No
	Parameters JSON (for A, b),   i:  ../outlab10_input.json
	Output JSON (for x),     o: ../outlab7_output_1.json
	Convergence Threshold,   𝜀: 0.0001
	Max iterations,          k: 5000
	Matrix order,            n: None provided, will be inferred from input JSON
	SOR weight,              ω: 1.080000
	Use LUP factorization     : Yes
	Use Gauss-Seidel          : Yes
	Use Point-Jacobi          : Yes
	Use SOR                   : Yes
	Use Point-Jacobi with SOR : Yes
	Use symmetric SOR         : Yes
--------------------------------------------------------------------------------
Reading matrix order (n) from input matrix dimensions: 4
--------------------------------------------------------------------------------
Coefficient Matrix (A):
--------------------------------------------------------------------------------
    3.61e+01   -9.00e+00   -9.00e+00    0.00e+00
   -9.00e+00    3.61e+01    0.00e+00   -9.00e+00
   -9.00e+00    0.00e+00    3.61e+01   -9.00e+00
    0.00e+00   -9.00e+00   -9.00e+00    3.61e+01
--------------------------------------------------------------------------------
Constants Vector (b):
--------------------------------------------------------------------------------
    1.00e+00    1.00e+00    1.00e+00    1.00e+00
--------------------------------------------------------------------------------
LUP Factorization Results
--------------------------------------------------------------------------------
	order                     : 4
	total iterations          : 0
	converged                 : Yes
	iterative error           : nan
	relative error            : 0.00e+00
	absolute maximum residual : 2.17e-19
	execution time [ns]       : 1185.600000 ± 716.407593
--------------------------------------------------------------------------------
Point Jacobi Method Results
--------------------------------------------------------------------------------
	order                     : 4
	total iterations          : 10
	converged                 : Yes
	iterative error           : 5.26e-05
	relative error            : 0.00e+00
	absolute maximum residual : 9.50e-04
	execution time [ns]       : 511.400000 ± 97.897089
--------------------------------------------------------------------------------
SOR Point Jacobi Method Results
--------------------------------------------------------------------------------
	order                     : 4
	total iterations          : 9
	converged                 : Yes
	iterative error           : 5.36e-05
	relative error            : 0.00e+00
	absolute maximum residual : 8.96e-04
	execution time [ns]       : 413.900000 ± 14.159449
--------------------------------------------------------------------------------
Gauss-Seidel Method Results
--------------------------------------------------------------------------------
	order                     : 4
	total iterations          : 6
	converged                 : Yes
	iterative error           : 3.69e-05
	relative error            : 0.00e+00
	absolute maximum residual : 2.65e-04
	execution time [ns]       : 481.000000 ± 56.723893
--------------------------------------------------------------------------------
SOR Method Results
--------------------------------------------------------------------------------
	order                     : 4
	total iterations          : 4
	converged                 : Yes
	iterative error           : 3.89e-05
	relative error            : 0.00e+00
	absolute maximum residual : 3.52e-06
	execution time [ns]       : 350.100000 ± 14.645477
--------------------------------------------------------------------------------
Symmetric SOR Method Results
--------------------------------------------------------------------------------
	order                     : 4
	total iterations          : 3
	converged                 : Yes
	iterative error           : 5.62e-05
	relative error            : 0.00e+00
	absolute maximum residual : 8.02e-05
	execution time [ns]       : 465.400000 ± 60.016998
--------------------------------------------------------------------------------
JSON data has been written to ../outlab7_output_1.json

Process finished with exit code 0
```
