# Project Milestone 5: Parallel Gauss Seidel Solver

## Table of Contents

1. [Building](#building)
   - [EOS`remote.eos.ncsu.edu`](#eos)
2. [Usage](#usage)
   - [Parameters](#parameters)
   - [Inputs/Outputs](#inputsoutputs)
   - [Solver Options](#solver-options)
   - [Performance Benchmarking](#performance-benchmarking)
   - [General options](#general-options)
3. [Parameters Format](#input-format)
    - [Sample Parameters Files](#sample-input-files)
        - [[REQUIRED] Source Term q(i,j) CSV](#source-term-qij-csv-required)
        - [Parameters JSON](#parameters-json)
4. [Output Format](#output-format)
    - [Sample Outputs Files](#sample-outputs-files)
        - [Computed Flux 𝜙(𝑖,𝑗) CSV](#computed-flux-𝜙𝑖𝑗-csv)
        - [Results JSON](#results-json)
5. [Example](#example)

## Building

The code has been built and tested on the NCSU EOS `remote.eos.ncsu.edu` servers. 
It requires no additional configuration except choosing the build target, and output file. Here is a repeatable script
to perform the build and run the `project5` target executable:

### EOS
```bash
#!/bin/bash

# Begin by copying the files over using rsync, scp, sftp, etc...
# Then, assuming the repo root is the current directory:

## Specify the build target
export BUILD_TARGET=project5

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export INPUT_PARAMETERS=./src/project/project5/examples/config_32x32.json
export INPUT_SOURCETERMS=./src/project/project5/examples/sources_32x32.csv

export OUTPUT_RESULTS=./src/project/project5/examples/results_32x32.json
export OUTPUT_COMPUTED_FLUX=./src/project/project5/examples/results_32x32

## Execute
./build/bin/$BUILD_TARGET -i $INPUT_PARAMETERS -s $INPUT_SOURCETERMS -o $OUTPUT_RESULTS -f $OUTPUT_COMPUTED_FLUX
```

---

## Usage
### Parameters
- `-a arg`: Length of 1st dimension (+ve real)
- `-b arg`: Length of 2nd dimension (+ve real)
- `-m arg`: Number of mesh-points in 1st dimension
- `-n arg`: Number of mesh-points in 2nd dimension
- `-D arg`: Diffusion coefficient D (+ve real)
- `--cross-section arg`: Removal cross-section Σₐ (+ve real)

### Inputs/Outputs
- `-i [ --input-parameter-json ] arg`: Path to input parameter JSON
- `-s [ --source-terms-csv ] arg`: Path to source-terms 𝑞(𝑖,𝑗) CSV
- `-o [ --output-results-json ] arg`: Path to output results JSON
- `-f [ --output-flux ] arg`: Path to computed flux 𝜙(𝑖,𝑗) directory

### Solver Options
- `--use-point-jacobi`: Use the Point-Jacobi method
- `--use-gauss-seidel`: Use the Gauss-Seidel method
- `-t [ --threshold ] arg     `: iterative convergence threshold [𝜀 > 0]
- `-k [ --max-iterations ] arg`: maximum number of iterations [n ∈ ℕ]
- `-w [ --relaxation-factor ] arg`: SOR weight, typical ω ∈ [0,2]

### Performance Benchmarking
- `-B [ --bench ]`: Run performance benchmarks
- `-R [ --bench-runs ] arg (=1)`: <R> runs to perform
- `-T [ --bench-timeout ] arg (=0)`: Timeout after <T> seconds [0=never]

### General options
- `-h [ --help ]`: Show this help message
- `-q [ --quiet ]`: Reduce verbosity
- `-p [ --precision ] arg (=15)`: Number of digits to represent long double

## Parameters Format

<p style="background: black"><span style="font-weight: 900; color: red">CAUTION:</span> There are <span style="font-weight: 900; color: red">TWO</span> input files!</p> 

### Sample Parameters Files

#### Source Term q(i,j) CSV [REQUIRED]:

One `.csv` file, containing a 2D array (m x n), with the source terms in the following format:

   ```text
   1.1,1.2,1.3
   2.1,2.2,2.3
   3.1,3.2,3.3
   ```

#### Parameters JSON:

One `.json` file, containing quantification parameters. If not supplied, parameters will be read interactively over the
terminal.

   ```json
{
   "D": 1.0,
   "cross-section": 0.1,
   "dimensions": {
      "a": 10,
      "b": 10
   },
   "mesh": {
      "m": 32,
      "n": 32
   },
   "threshold": 1e-5,
   "max-iterations": 5000,
   "relaxation-factor": 1.00,
   "methods": [
      "point-jacobi"
   ]
}
   ```

## Output Format

<p style="background: black"><span style="font-weight: 900; color: yellow">CAUTION:</span> There are <span style="font-weight: 900; color: yellow">TWO</span> output files!</p>

### Sample Outputs Files

Both output files are optional, but if not supplied, outputs will be written to terminal only.

#### Computed Flux 𝜙(𝑖,𝑗) CSV:

      ```text
      0.00000, 0.00000, 0.00000, 0.00000, 0.00000
      0.00000, 322.680, 360.942, 377.734, 0.00000
      0.00000, 615.877, 663.982, 672.529, 0.00000
      0.00000, 873.227, 927.464, 928.282, 0.00000
      0.00000, 0.00000, 0.00000, 0.00000, 0.00000
      ```

#### Results JSON:

   ```json
{
   "inputs": {
      "D": 1.0,
      "cross-section": 0.1,
      "dimensions": {
         "a": 10.0,
         "b": 10.0
      },
      "mesh": {
         "m": 32,
         "n": 32,
         "𝛾": 0.30303030303030304,
         "𝛿": 0.30303030303030304
      },
      "methods": [
         "point-jacobi"
      ]
   },
   "outputs": {
      "1": {
         "converged": true,
         "iterations": {
            "actual": 437,
            "maximum": 5000
         },
         "iterative-error": {
            "actual": 9.963886456284327e-05,
            "maximum": 0.0001
         },
         "max-bytes": 0,
         "max-residual": null,
         "wall-time-ns": {
            "mean": 10429298960.0,
            "p5th": 10429298960.0,
            "p95th": 10429298960.0,
            "samples": 1
         }
      },
      "12": {
         "converged": true,
         "iterations": {
            "actual": 437,
            "maximum": 5000
         },
         "iterative-error": {
            "actual": 9.963886456284327e-05,
            "maximum": 0.0001
         },
         "max-bytes": 0,
         "max-residual": null,
         "wall-time-ns": {
            "mean": 1092790231.0,
            "p5th": 1092790231.0,
            "p95th": 1092790231.0,
            "samples": 1
         }
      },
      "16": {
         "converged": true,
         "iterations": {
            "actual": 437,
            "maximum": 5000
         },
         "iterative-error": {
            "actual": 9.963886456284327e-05,
            "maximum": 0.0001
         },
         "max-bytes": 0,
         "max-residual": null,
         "wall-time-ns": {
            "mean": 1118173445.0,
            "p5th": 1118173445.0,
            "p95th": 1118173445.0,
            "samples": 1
         }
      },
      "4": {
         "converged": true,
         "iterations": {
            "actual": 437,
            "maximum": 5000
         },
         "iterative-error": {
            "actual": 9.963886456284327e-05,
            "maximum": 0.0001
         },
         "max-bytes": 0,
         "max-residual": null,
         "wall-time-ns": {
            "mean": 2744634574.0,
            "p5th": 2744634574.0,
            "p95th": 2744634574.0,
            "samples": 1
         }
      },
      "8": {
         "converged": true,
         "iterations": {
            "actual": 437,
            "maximum": 5000
         },
         "iterative-error": {
            "actual": 9.963886456284327e-05,
            "maximum": 0.0001
         },
         "max-bytes": 0,
         "max-residual": null,
         "wall-time-ns": {
            "mean": 1499789305.0,
            "p5th": 1499789305.0,
            "p95th": 1499789305.0,
            "samples": 1
         }
      }
   }
}

```

## Example:

The following is an example of the program's output:

```shell
NE591: Project Milestone 5: Parallel Gauss Seidel Solver
Arjun Earthperson
10/20/2023
--------------------------------------------------------------------------------
using 128-bit floats
compiler: GNU 8.5.0
boost: 106600 /usr/lib64/libboost_program_options.so;/usr/lib64/libboost_serialization.so
--------------------------------------------------------------------------------
Parameters:
  -a arg                             = Length of 1st dimension (+ve real)
  -b arg                             = Length of 2nd dimension (+ve real)
  -m arg                             = Number of mesh-points in 1st dimension
  -n arg                             = Number of mesh-points in 2nd dimension
  -D arg                             = Diffusion coefficient D (+ve real)
  --cross-section arg                = Removal cross-section Σₐ (+ve real)

Solver Options:
  --use-point-jacobi                 = Use the Point-Jacobi method
  --use-gauss-seidel                 = Use the Gauss-Seidel method
  -t [ --threshold ] arg             = convergence threshold [𝜀 > 0]
  -k [ --max-iterations ] arg        = maximum iterations [n ∈ ℕ]

Inputs/Outputs:
  -i [ --input-parameter-json ] arg  = Path to input parameter JSON
  -s [ --source-terms-csv ] arg      = Path to source-terms 𝑞(𝑖,𝑗) CSV
  -o [ --output-results-json ] arg   = Path to output results JSON
  -f [ --flux-output-dir ] arg       = Path to computed flux 𝜙(𝑖,𝑗)

Performance Benchmarking:
  -R [ --bench-runs ] arg (=1)       = <R> runs to perform
  -T [ --bench-timeout ] arg (=0)    = Timeout after <T> seconds [0=never]
  -B [ --bench ]                     = Run performance benchmarks

General options:
  -h [ --help ]                      = Show this help message
  -q [ --quiet ]                     = Reduce verbosity
  -p [ --precision ] arg (=15)       = Number of digits to represent long 
                                     double

--------------------------------------------------------------------------------
			Precision in digits:  default: 6, maximum: 19, current: 15
--------------------------------------------------------------------------------
Would you like to use the Gauss-Seidel method? [YES/no]: y
--------------------------------------------------------------------------------
                                     Inputs
--------------------------------------------------------------------------------
	Input JSON,     i: ../config_16x16.json
	Output JSON,    o: ../project5_16x16.json
	----
	Convergence Threshold,                  𝜀: 1e-05
	Max iterations,                         k: 5000
	SOR weight,                             ω: N/A
	Use Gauss-Seidel                         : Yes
	Use Point-Jacobi                         : Yes
	----
	1st dimension length,                   a: 10
	2nd dimension length,                   b: 10
	1st dimension mesh-points,              m: 16
	2nd dimension mesh-points,              n: 16
	Diffusion coefficient,                  D: 1
	Macroscopic removal cross-section,     Σₐ: 0.1
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
                              Intermediates
--------------------------------------------------------------------------------
	Mesh spacing in the 1st dimension,     𝛿: 0.588235294117647
	Mesh spacing in the 2nd dimension,     𝛾: 0.588235294117647
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------

:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[1/1] : Point Jacobi
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 4.77e+07 :::::::: VARIANCE: 0.00e+00 :::::::: MEDIAN: 4.77e+07 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {4.77e+07, 4.77e+07} : (4.77e+07 ± 0.00e+00) : [4.77e+07, 4.77e+07] :::::
:::::: Estimated maximum allocated memory [bytes]: 0 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
--------------------------------------------------------------------------------
[N = 16]: Point Jacobi Method Results
--------------------------------------------------------------------------------
	order                     : 256
	total iterations          : 283
	converged                 : Yes
	iterative error           : 9.829758437587247e-06
	absolute maximum residual : nan

:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[1/1] : Point Jacobi
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 5.87e+07 :::::::: VARIANCE: 0.00e+00 :::::::: MEDIAN: 5.87e+07 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {5.87e+07, 5.87e+07} : (5.87e+07 ± 0.00e+00) : [5.87e+07, 5.87e+07] :::::
:::::: Estimated maximum allocated memory [bytes]: 0 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
--------------------------------------------------------------------------------
[N = 8]: Point Jacobi Method Results
--------------------------------------------------------------------------------
	order                     : 256
	total iterations          : 283
	converged                 : Yes
	iterative error           : 9.829758437587247e-06
	absolute maximum residual : nan

:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[1/1] : Point Jacobi
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 9.35e+07 :::::::: VARIANCE: 0.00e+00 :::::::: MEDIAN: 9.35e+07 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {9.35e+07, 9.35e+07} : (9.35e+07 ± 0.00e+00) : [9.35e+07, 9.35e+07] :::::
:::::: Estimated maximum allocated memory [bytes]: 0 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
--------------------------------------------------------------------------------
[N = 5]: Point Jacobi Method Results
--------------------------------------------------------------------------------
	order                     : 256
	total iterations          : 283
	converged                 : Yes
	iterative error           : 9.829758437587247e-06
	absolute maximum residual : nan

:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[1/1] : Point Jacobi
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 4.30e+08 :::::::: VARIANCE: 0.00e+00 :::::::: MEDIAN: 4.30e+08 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {4.30e+08, 4.30e+08} : (4.30e+08 ± 0.00e+00) : [4.30e+08, 4.30e+08] :::::
:::::: Estimated maximum allocated memory [bytes]: 0 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
--------------------------------------------------------------------------------
[N = 1]: Point Jacobi Method Results
--------------------------------------------------------------------------------
	order                     : 256
	total iterations          : 283
	converged                 : Yes
	iterative error           : 9.829758437587247e-06
	absolute maximum residual : nan

:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[1/1] : Gauss Seidel
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 6.38e+09 :::::::: VARIANCE: 0.00e+00 :::::::: MEDIAN: 6.38e+09 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {6.38e+09, 6.38e+09} : (6.38e+09 ± 0.00e+00) : [6.38e+09, 6.38e+09] :::::
:::::: Estimated maximum allocated memory [bytes]: 0 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
--------------------------------------------------------------------------------
[N = 16]: Gauss Seidel Method Results
--------------------------------------------------------------------------------
	order                     : 256
	total iterations          : 5000
	converged                 : No
	iterative error           : nan
	absolute maximum residual : nan

:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[1/1] : Gauss Seidel
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 4.72e+07 :::::::: VARIANCE: 0.00e+00 :::::::: MEDIAN: 4.72e+07 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {4.72e+07, 4.72e+07} : (4.72e+07 ± 0.00e+00) : [4.72e+07, 4.72e+07] :::::
:::::: Estimated maximum allocated memory [bytes]: 0 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
--------------------------------------------------------------------------------
[N = 8]: Gauss Seidel Method Results
--------------------------------------------------------------------------------
	order                     : 256
	total iterations          : 149
	converged                 : Yes
	iterative error           : 9.751573362559852e-06
	absolute maximum residual : nan

:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[1/1] : Gauss Seidel
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 5.34e+07 :::::::: VARIANCE: 0.00e+00 :::::::: MEDIAN: 5.34e+07 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {5.34e+07, 5.34e+07} : (5.34e+07 ± 0.00e+00) : [5.34e+07, 5.34e+07] :::::
:::::: Estimated maximum allocated memory [bytes]: 0 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
--------------------------------------------------------------------------------
[N = 5]: Gauss Seidel Method Results
--------------------------------------------------------------------------------
	order                     : 256
	total iterations          : 149
	converged                 : Yes
	iterative error           : 9.498875228151052e-06
	absolute maximum residual : nan

:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[1/1] : Gauss Seidel
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 2.23e+08 :::::::: VARIANCE: 0.00e+00 :::::::: MEDIAN: 2.23e+08 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {2.23e+08, 2.23e+08} : (2.23e+08 ± 0.00e+00) : [2.23e+08, 2.23e+08] :::::
:::::: Estimated maximum allocated memory [bytes]: 0 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
--------------------------------------------------------------------------------
[N = 1]: Gauss Seidel Method Results
--------------------------------------------------------------------------------
	order                     : 256
	total iterations          : 149
	converged                 : Yes
	iterative error           : 9.561698963463480e-06
	absolute maximum residual : nan
--------------------------------------------------------------------------------
JSON data has been written to ../project5_16x16.json

Process finished with exit code 0
```
