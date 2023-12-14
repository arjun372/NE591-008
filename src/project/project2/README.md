# Project Milestone 2: Iterative, Lazy-Evaluated, Serial Diffusion Equation Solver

Serial 2D, one speed, time-independent, neutron diffusion equation solver. Matrix equation solved using 5 iterative 
solvers and 1 direct solver (LU factorization) with pivoting. File I/O is performed using a combination of `CSV` and 
`JSON` files.

## Table of Contents

1. [Building](#building)
   - [EOS`remote.eos.ncsu.edu`](#eos)
   - [Hazel`login.hpc.ncsu.edu`](#on-hazel) 
     - [Interactive Mode](#interactive-mode)
     - [Batch Jobs](#batch-jobs)
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

The code has been built and tested on the NCSU Hazel`login.hpc.ncsu.edu` and NCSU EOS `remote.eos.ncsu.edu` servers. 
It requires no additional configuration except choosing the build target, and output file. Here is a repeatable script
to perform the build and run the `project2` target executable:

### Hazel
There are two ways to do run jobs on Hazel, currently, we support the interactive mode option.
#### Interactive Mode
This mode requires a few additional steps, but for the most part, it follows the same process as the EOS server.
```bash
#!/bin/bash

# Start by logging in
ssh login.hpc.ncsu.edu

# Then, start an interactive session
bsub -Is -n 4 -R "span[hosts=1]" -W 20 bash

# Create a working directory, in this case we call it earthperson_project2 to avoid naming conflicts
mkdir -p /share/$GROUP/$USER/earthperson_project2

# Go into that directory.
cd /share/$GROUP/$USER/earthperson_project2

# Load the build modules
module load openmpi-gcc/openmpi4.1.0-gcc10.2.0 cmake/3.24.1

# Begin by copying the files over using rsync, scp, sftp, etc...
# Then, assuming the repo root is the current directory:

## Specify the build target
export BUILD_TARGET=project2

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export INPUT_PARAMETERS=./src/project/project2/examples/project2_example_input_parameters.json
export INPUT_SOURCETERMS=./src/project/project2/examples/project2_example_source_terms.csv
export OUTPUT_RESULTS=./src/project/project2/examples/project2_example_output_results.json
export OUTPUT_COMPUTED_FLUX=./src/project/project2/examples/project2_example_computed_flux

## Execute
./build/bin/$BUILD_TARGET -i $INPUT_PARAMETERS -s $INPUT_SOURCETERMS -o $OUTPUT_RESULTS -f $OUTPUT_COMPUTED_FLUX
```
#### Batch Jobs

Coming Soon

### EOS
```bash
#!/bin/bash

# Begin by copying the files over using rsync, scp, sftp, etc...
# Then, assuming the repo root is the current directory:

## Specify the build target
export BUILD_TARGET=project2

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export INPUT_PARAMETERS=./src/project/project2/examples/project2_example_input_parameters.json
export INPUT_SOURCETERMS=./src/project/project2/examples/project2_example_source_terms.csv

export OUTPUT_RESULTS=./src/project/project2/examples/project2_example_output_results.json
export OUTPUT_COMPUTED_FLUX=./src/project/project2/examples/project2_example_computed_flux

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
- `--use-LUP`: Use LUP factorization
- `--use-point-jacobi`: Use the Point-Jacobi method
- `--use-gauss-seidel`: Use the Gauss-Seidel method
- `--use-SOR`: Use the SOR method
- `--use-SORJ`: Use the SOR Jacobi method
- `--use-SSOR`: Use the symmetric SOR method
- `-t [ --convergence_threshold ] arg     `: iterative convergence convergence_threshold [𝜀 > 0]
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
      "D": 0.07,
      "cross-section": 0.0033,
      "dimensions": {
         "a": 100,
         "b": 100
      },
      "mesh": {
         "m": 3,
         "n": 3
      },
      "convergence_threshold": 1e-4,
      "max-iterations": 5000,
      "relaxation-factor": 1.08,
      "methods": [
         "LUP",
         "SOR",
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
      "D": 0.07,
      "cross-section": 0.0033,
      "dimensions": {
         "a": 100.0,
         "b": 100.0
      },
      "mesh": {
         "m": 5,
         "n": 5,
         "𝛾": 16.666666666666668,
         "𝛿": 16.666666666666668
      },
      "methods": [
         "LUP",
         "SOR"
      ]
   },
   "outputs": {
      "LUP": {
         "converged": true,
         "iterations": {
            "actual": 0,
            "maximum": 5000
         },
         "iterative-error": {
            "actual": null,
            "maximum": 0.0001
         },
         "max-residual": 1.6263032587282567e-19,
         "residual": [],
         "solution": [],
         "wall-time-ns": {
            "max": 186824.0,
            "mean": 121115.6,
            "min": 98279.0,
            "p5th": 98457.6,
            "p95th": 173750.0,
            "samples": 5,
            "stddev": 33979.430546140706,
            "sum": 605578.0,
            "variance": 1154601700.24
         }
      },
      "SOR": {
         "converged": true,
         "iterations": {
            "actual": 8,
            "maximum": 5000
         },
         "iterative-error": {
            "actual": 8.566771904100693e-05,
            "maximum": 0.0001
         },
         "max-residual": 3.872281957573891e-08,
         "residual": [],
         "solution": [],
         "wall-time-ns": {
            "max": 132794.0,
            "mean": 87897.0,
            "min": 76575.0,
            "p5th": 76586.6,
            "p95th": 121584.0,
            "samples": 5,
            "stddev": 22448.59176875022,
            "sum": 439485.0,
            "variance": 503939272.4
         }
      }
   }
}
```

## Example:

The following is an example of the program's output:

```shell
NE591: Project Milestone 2: Iterative, Lazy-Evaluated, Serial Diffusion Equation Solver
Arjun Earthperson
10/13/2023
--------------------------------------------------------------------------------
using 128-bit floats
compiler: GNU 8.5.0, boost: 106600 /usr/lib64/libboost_program_options.so
--------------------------------------------------------------------------------
Parameters:
  -a arg                             = Length of 1st dimension (+ve real)
  -b arg                             = Length of 2nd dimension (+ve real)
  -m arg                             = Number of mesh-points in 1st dimension
  -n arg                             = Number of mesh-points in 2nd dimension
  -D arg                             = Diffusion coefficient D (+ve real)
  --cross-section arg                = Removal cross-section Σₐ (+ve real)

Solver Options:
  --use-LUP                          = Use LUP factorization
  --use-point-jacobi                 = Use the Point-Jacobi method
  --use-SORJ                         = Use the SOR Jacobi method
  --use-gauss-seidel                 = Use the Gauss-Seidel method
  --use-SOR                          = Use the SOR method
  --use-SSOR                         = Use the symmetric SOR method
  -t [ --convergence_threshold ] arg             = convergence convergence_threshold [𝜀 > 0]
  -k [ --max-iterations ] arg        = maximum iterations [n ∈ ℕ]
  -w [ --relaxation-factor ] arg     = SOR weight, typical ω ∈ [0,2]

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
Provided path is a directory: ../project2_example_fluxes
Would you like to use the LUP factorization method? [YES/no]: y
--------------------------------------------------------------------------------
                                     Inputs
--------------------------------------------------------------------------------
	Input JSON,     i: ../project2_example_input_parameters.json
	Output JSON,    o: ../project2_example_output.json
	----
	Convergence Threshold,                  𝜀: 0.0001
	Max iterations,                         k: 5000
	SOR weight,                             ω: 1.080000
	Use LUP factorization                    : Yes
	Use Gauss-Seidel                         : Yes
	Use Point-Jacobi                         : Yes
	Use SOR                                  : Yes
	Use Point-Jacobi with SOR                : Yes
	Use symmetric SOR                        : Yes
	----
	1st dimension length,                   a: 100
	2nd dimension length,                   b: 100
	1st dimension mesh-points,              m: 3
	2nd dimension mesh-points,              n: 3
	Diffusion coefficient,                  D: 0.07
	Macroscopic removal cross-section,     Σₐ: 0.0033
--------------------------------------------------------------------------------
                              Intermediates
--------------------------------------------------------------------------------
	Mesh spacing in the 1st dimension,     𝛿: 25
	Mesh spacing in the 2nd dimension,     𝛾: 25
--------------------------------------------------------------------------------
Source terms 𝑞(𝑖,𝑗):
--------------------------------------------------------------------------------
    1.100000000000000e+00    1.200000000000000e+00    1.300000000000000e+00
    2.100000000000000e+00    2.200000000000000e+00    2.300000000000000e+00
    3.100000000000000e+00    3.200000000000000e+00    3.300000000000000e+00
--------------------------------------------------------------------------------
::::::::::::::::::::::::::::::: PROFILE SUMMARY ::::::::::::::::::::::::::::::::
[5/5] : LUP
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 1.30e+05 :::::::: VARIANCE: 6.94e+07 :::::::: MEDIAN: 2.25e+04 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {1.96e+04, 4.23e+04} : (2.60e+04 ± 8.33e+03) : [1.99e+04, 3.88e+04] :::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
--------------------------------------------------------------------------------
LUP Factorization Results
--------------------------------------------------------------------------------
	order                     : 9
	total iterations          : 0
	converged                 : Yes
	iterative error           : nan
	absolute maximum residual : 2.168404344971009e-19
--------------------------------------------------------------------------------
::::::::::::::::::::::::::::::: PROFILE SUMMARY ::::::::::::::::::::::::::::::::
[5/5] : SOR
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 2.53e+05 :::::::: VARIANCE: 8.48e+04 :::::::: MEDIAN: 5.06e+04 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {5.03e+04, 5.12e+04} : (5.07e+04 ± 2.91e+02) : [5.03e+04, 5.11e+04] :::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
--------------------------------------------------------------------------------
SOR Method Results
--------------------------------------------------------------------------------
	order                     : 9
	total iterations          : 7
	converged                 : Yes
	iterative error           : 9.925586527559588e-05
	absolute maximum residual : 2.241588572204044e-08
--------------------------------------------------------------------------------
JSON data has been written to ../project2_example_output.json

Process finished with exit code 0
```
