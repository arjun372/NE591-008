# OutLab 06: Iterative Linear Equations Solver: Point-Jacobi

Solves a system of linear equations using the point-Jacobi method.
File based I/O is supported using JSON files.

## Table of Contents

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
to perform the build and run the `outlab6` target executable:

```bash
# Assuming cwd is the repo root:
#!/bin/bash

## Specify the build target
export BUILD_TARGET=outlab6

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export INPUT_FILE=./src/labs/outlab6/examples/outlab6_input_1.json
export OUTPUT_FILE=./src/labs/outlab6/examples/outlab6_output_1.json

## Execute
./build/bin/$BUILD_TARGET -i $INPUT_FILE -o $OUTPUT_FILE
```

### Parameters

- `-t [ --threshold ] arg     `: iterative convergence threshold [𝜀 > 0]
- `-k [ --max-iterations ] arg`: maximum number of iterations [n ∈ ℕ]
- `-n [ --order ] arg`: order of the square matrix [n ∈ ℕ]
- `-i [ --input-json ] arg`: input JSON containing A, and b
- `-o [ --output-json ] arg`: path for the output JSON

### Solver Methods

- `--use-LUP`: Use the LUP method
- `--use-point-jacobi`: [DISABLED] Use the Point-Jacobi method
- `--use-gauss-seidel`: [DISABLED] Use the Gauss-Seidel method
- `--use-SOR`: [DISABLED] Use the SOR method

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
         [ 36.1, -9.00, -9.00,  0.00],
         [-9.00,  36.1,  0.00, -9.00],
         [-9.00,  0.00,  36.1, -9.00],
         [ 0.00, -9.00, -9.00,  36.1]
      ],
      "constants": [1.00, 1.00, 1.00, 1.00],
      "max-iterations": 5000,
      "methods": [
         "point-jacobi"
      ],
      "order": 4,
      "threshold": 0.0001
   },
   "outputs": {
      "point-jacobi": {
         "converged": true,
         "execution-time-ns": 2352.0,
         "iterations": {
            "actual": 10,
            "maximum": 5000
         },
         "iterative-error": {
            "actual": 5.2623026955029647e-05,
            "maximum": 0.0001
         },
         "max-residual": 0.0009498456365382851,
         "solution": [
            0.05519614112505313,
            0.05519614112505313,
            0.05519614112505313,
            0.05519614112505313
         ]
      }
   }
}
```

## Example:

The following is an example of the program's output:

```shell
NE591: OutLab 06: Solving a system of linear equations using the Point Jacobi method
Arjun Earthperson
09/29/2023
--------------------------------------------------------------------------------
compiler: GNU 8.5.0, boost: 106600 /usr/lib64/libboost_program_options.so
--------------------------------------------------------------------------------
Parameters:
  -t [ --threshold ] arg        = iterative convergence threshold [𝜀 > 0]
  -k [ --max-iterations ] arg   = maximum number of iterations [n ∈ ℕ]
  -n [ --order ] arg            = order of the square matrix [n ∈ ℕ]
  -i [ --input-json ] arg       = input JSON containing A, and b
  -o [ --output-json ] arg      = path for the output JSON

Solver Methods:
  --use-point-jacobi            = Use the Point-Jacobi method
  --use-gauss-seidel            = [DISABLED] Use the Gauss-Seidel method
  --use-SOR                     = [DISABLED] Use the SOR method
  --use-SSOR                    = [DISABLED] Use the symmetric SOR method

General options:
  -h [ --help ]                 = Show this help message
  -q [ --quiet ]                = Reduce verbosity
  -p [ --precision ] arg (=15)  = Number of digits to represent long double
  -P [ --profile ]              = Turn on performance profiling

--------------------------------------------------------------------------------
			Precision in digits:  default: 6, maximum: 19, current: 6
--------------------------------------------------------------------------------
Warning: File already exists at path, will be overwritten 
Would you like to use the Gauss-Seidel method? [YES/no]: n
Would you like to use the SOR method? [YES/no]: n
Would you like to use the symmetric SOR method? [YES/no]: y
--------------------------------------------------------------------------------
                                     Inputs
--------------------------------------------------------------------------------
	Input JSON (for A, b),  i: ../outlab6_input_1.json
	Output JSON (for x),    o: ../outlab6_output_1.json
	Convergence Threshold,  𝜀: 0.0001
	Max iterations,         k: 5000
	Matrix order,           n: None provided, will be inferred from input JSON
	Use Gauss-Siedel         : No
	Use Point-Jacobi         : Yes
	Use SOR                  : No
	Use SSOR                 : Yes
--------------------------------------------------------------------------------
Reading matrix order (n) from input matrix dimensions: 4
--------------------------------------------------------------------------------
Coefficient Matrix (A):
--------------------------------------------------------------------------------
    3.610000e+01   -9.000000e+00   -9.000000e+00    0.000000e+00
   -9.000000e+00    3.610000e+01    0.000000e+00   -9.000000e+00
   -9.000000e+00    0.000000e+00    3.610000e+01   -9.000000e+00
    0.000000e+00   -9.000000e+00   -9.000000e+00    3.610000e+01
--------------------------------------------------------------------------------
Constants Vector (b):
--------------------------------------------------------------------------------
    1.000000e+00    1.000000e+00    1.000000e+00    1.000000e+00
--------------------------------------------------------------------------------
Point Seidel Method Results
--------------------------------------------------------------------------------
	total iterations          : 10
	converged                 : Yes
	iterative error           : 5.262303e-05
	absolute maximum residual : 9.498456e-04
	execution time (ns)       : 7.620000e+03
	execution time (ms)       : 7.620000e-03
	execution time (s)        : 7.620000e-06
--------------------------------------------------------------------------------
Symmetric Successive over-relaxation (SOR) not implemented yet.
--------------------------------------------------------------------------------
JSON data has been written to ../outlab6_output_1.json

Process finished with exit code 0
```
