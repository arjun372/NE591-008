# OutLab 05: System of Linear Equations Solver, with Pivoting

Solves a system of linear equations solver, with pivoting. File based I/O is supported using JSON files.

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
to perform the build and run the `outlab5` target executable:

```bash
# Assuming cwd is the repo root:
#!/bin/bash

## Specify the build target
export BUILD_TARGET=outlab5

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export INPUT_FILE=./src/labs/outlab5/examples/sample_input_1.json
export OUTPUT_FILE=./src/labs/outlab5/examples/sample_output_1.json

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
```

## Output Format

The output is written to a JSON file as well.

### Sample Outputs File
```json

```

## Example:

The following is an example of the program's output:

```shell
```
