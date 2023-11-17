<div style="display: none">
\page outlab7 OutLab 07: Series Sum using MPI Spanning Tree Protocol
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

The code has been built and tested on the `Hazel` or `login.hpc.ncsu.edu` servers. It requires no additional
configuration except choosing the build target, and output file. Here is a repeatable script
to perform the build and run the `outlab7` target executable:

```bash
#!/bin/bash

# Start by logging in to the login node. We will use this node to compile our sources.
ssh login.hpc.ncsu.edu

## Specify environment parameters
export BUILD_TARGET=outlab7
export BUILD_DIR=/tmp/$GROUP/$USER/earthperson_$BUILD_TARGET
export REPO_DIR=NE591-008-Monorepo
export REPO_URL=https://github.com/arjun372/$REPO_DIR.git

# Create a working directory in the $TMP directory
# We need to do this since the persistent /share/ directory is extremely slow for our use.
# Once we are done compiling, we can move the built executables and input files to the persistent share directory.

## Create a tmp build directory and go into it
mkdir -p $BUILD_DIR && cd $BUILD_DIR

## copy the files over. The easiest way (by far) is to just clone the git repo and cd into it.
git clone --recursive $REPO_URL && cd $REPO_DIR

# Load the build modules
module load openmpi-gcc/openmpi4.1.0-gcc10.2.0 cmake/3.24.1

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

# in this case we call it earthperson_$BUILD_TARGET to avoid naming conflicts
mkdir -p /share/$GROUP/$USER/earthperson_$BUILD_TARGET

# Go into that directory.
cd /share/$GROUP/$USER/earthperson_$BUILD_TARGET



# Begin by copying the files over using rsync, scp, sftp, etc...
# The easiest way by far is to just clone this repo:

# Then, assuming the repo root is the current directory:



# Then, start an interactive session
bsub -Is -n 16 -R "span[hosts=1]" -W 30 bash

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export INPUT_FILE=./src/labs/outlab7/examples/outlab7_input_example.json
export OUTPUT_FILE=./src/labs/outlab7/examples/outlab7_output_example.json

## Execute
mpirun ./build/bin/$BUILD_TARGET -i $INPUT_FILE -o $OUTPUT_FILE
```

### Parameters

- `-n arg`: Sum upper limit [n ∈ ℕ].
- `-i [ --input-json ] arg`: Input JSON containing n.
- `-o [ --output-json ] arg`: Path for the output JSON.

### General options

- `-h [ --help ]`: Show this help message.
- `-q [ --quiet ]`: Reduce verbosity.
- `-p [ --precision ] arg (=15)`: Number of digits to represent long double (default: 6, maximum: 19, current: 15).

## Performance Benchmarking
- `-R [ --bench-runs ] arg (=1)`: <R> runs to perform
- `-T [ --bench-timeout ] arg (=0)`: Timeout after <T> seconds [0=never]
- `-B [ --bench ]`: Run performance benchmarks

## Parameters Format

The expected input json file requires the following fields:

### Sample Parameters File

```json
{
   "n": 1e8
}
```

## Output Format

The output is written to a JSON file as well.

### Sample Outputs File

```json
{
   "inputs": {
      "n": 100000000
   },
   "outputs": {
      "benchmark": {
         "max": 322451552.0,
         "mean": 260817058.0,
         "min": 241098530.0,
         "p5th": 241155891.95,
         "p95th": 320252851.1,
         "samples": 10,
         "stddev": 29848429.197442453,
         "sum": 2608170580.0,
         "variance": 890928725554735.2
      },
      "total_sum": 2.0131691999274186
   }
}
```

## Example:

The following is an example of the program's output:

```shell
NE591: OutLab 07: Series Sum using MPI Spanning Tree Protocol
Arjun Earthperson
10/06/2023
--------------------------------------------------------------------------------
using 128-bit floats
compiler: GNU 8.5.0
boost:  
--------------------------------------------------------------------------------
Parameters:
  -n arg                           = sum upper limit [n ∈ ℕ]
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
                                     Inputs
--------------------------------------------------------------------------------
	Input JSON,              i: ./outlab7_input_example.json
	Output JSON,             o: ./outlab7_output_example.json
	Sum Upper Limit,         n: 100000000
--------------------------------------------------------------------------------

:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[10/10] : series sum
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 3.24e+09 :::::::: VARIANCE: 5.58e+15 :::::::: MEDIAN: 3.06e+08 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {2.41e+08, 4.73e+08} : (3.24e+08 ± 7.47e+07) : [2.48e+08, 4.51e+08] :::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

[4bcb2c377817][0]: Final sum: 2.013169199927419e+00

JSON data has been written to ./outlab7_output_example.json

Process finished with exit code 0
```
