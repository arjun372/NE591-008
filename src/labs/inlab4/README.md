<div style="display: none">
\page inlab4 InLab 04: LU - Linear Equations Solver I
</div>

# InLab 04: System of Linear Equations Solver

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
to perform the build and run the `inlab4` target executable:

```bash
# Assuming cwd is the repo root:
#!/bin/bash

## Specify the build target
export BUILD_TARGET=inlab4

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export INPUT_FILE=./src/labs/inlab4/examples/sample_input.json
export OUTPUT_FILE=./src/labs/inlab4/examples/sample_output.json

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
  "lower": [
    [1.0, 0.0, 0.0],
    [2.0, 1.0, 0.0],
    [3.0, 2.0, 1.0]
  ],
  "upper": [
    [1.0, 1.0, 2.0],
    [0.0, 2.0, 3.0],
    [0.0, 0.0, 3.0]
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
    "x": [
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
--------------------------------------------------------------------------------
InLab 04: Solving a system of linear equations using forward, back substitution
Arjun Earthperson
09/15/2023
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
Warning: File already exists at path, will be overwritten 
--------------------------------------------------------------------------------
                                     Inputs
--------------------------------------------------------------------------------
	Matrix order,  n: 0
	Input JSON,    i: ../sample_input.json
	Output JSON,   o: output.json
--------------------------------------------------------------------------------
Reading matrix order (n) from input matrix dimensions: 3
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
Constants Vector (b):
--------------------------------------------------------------------------------
7 21 38 
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
LU Matrix:
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
Warning: File already exists at path, will be overwritten 
JSON data has been written to output.json

Process finished with exit code 0
```
