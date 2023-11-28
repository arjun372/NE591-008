# Project Milestone 3: Performance Evaluation of Direct vs Iterative, Lazy-Evaluated, Serial Diffusion Equation Solvers

`project3` builds on `project2`, implementing a new memory-profiler. Benchmark results and analysis are listed in the 
`analysis` folder. Checkout the associated [python notebook for analysis results](analysis/Project_Milestone_3_Analysis.ipynb).

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
to perform the build and run the `project3` target executable:

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

# Create a working directory, in this case we call it earthperson_project3 to avoid naming conflicts
mkdir -p /share/$GROUP/$USER/earthperson_project3

# Go into that directory.
cd /share/$GROUP/$USER/earthperson_project3

# Load the build modules
module load openmpi-gcc/openmpi4.1.0-gcc10.2.0 cmake/3.24.1

# Begin by copying the files over using rsync, scp, sftp, etc...
# Then, assuming the repo root is the current directory:

## Specify the build target
export BUILD_TARGET=project3

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export INPUT_PARAMETERS=./src/project/project3/examples/project3_example_input_parameters.json
export INPUT_SOURCETERMS=./src/project/project3/examples/project3_example_source_terms.csv
export OUTPUT_RESULTS=./src/project/project3/examples/project3_example_output_results.json
export OUTPUT_COMPUTED_FLUX=./src/project/project3/examples/project3_example_computed_flux.csv

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
export BUILD_TARGET=project3

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export INPUT_PARAMETERS=./src/project/project3/examples/project3_example_input_parameters.json
export INPUT_SOURCETERMS=./src/project/project3/examples/project3_example_source_terms.csv

export OUTPUT_RESULTS=./src/project/project3/examples/project3_example_output_results.json
export OUTPUT_COMPUTED_FLUX=./src/project/project3/examples/project3_example_computed_flux

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
      "threshold": 1e-4,
      "max-iterations": 5000,
      "relaxation-factor": 1.08,
      "methods": [
         "LUP",
         "SOR"
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
NE591: Project Milestone 3: Performance Evaluation of Direct vs Iterative, Lazy-Evaluated, Serial Diffusion Equation Solvers
Arjun Earthperson
10/20/2023
--------------------------------------------------------------------------------
using 128-bit floats
compiler: GNU 8.5.0
boost:  
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
  -t [ --threshold ] arg             = convergence threshold [𝜀 > 0]
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
Provided path is a directory: ../project3_4x4
Would you like to use the LUP factorization method? [YES/no]: y
--------------------------------------------------------------------------------
                                     Inputs
--------------------------------------------------------------------------------
	Input JSON,     i: ../project3_example_input_parameters.json
	Output JSON,    o: ../project3_4x4.json
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
WARNING: Source terms matrix rows != m, overriding m to 5
WARNING: Source terms matrix columns != n, overriding n to 5
--------------------------------------------------------------------------------
                              Intermediates
--------------------------------------------------------------------------------
	Mesh spacing in the 1st dimension,     𝛿: 16.6666666666667
	Mesh spacing in the 2nd dimension,     𝛾: 16.6666666666667
--------------------------------------------------------------------------------
Source terms 𝑞(𝑖,𝑗):
--------------------------------------------------------------------------------
    0.000000000000000e+00    0.000000000000000e+00    0.000000000000000e+00    0.000000000000000e+00    0.000000000000000e+00
    0.000000000000000e+00    1.000000000000000e+00    0.000000000000000e+00    1.000000000000000e+00    0.000000000000000e+00
    0.000000000000000e+00    0.000000000000000e+00    0.000000000000000e+00    0.000000000000000e+00    0.000000000000000e+00
    0.000000000000000e+00    1.000000000000000e+00    0.000000000000000e+00    1.000000000000000e+00    0.000000000000000e+00
    0.000000000000000e+00    0.000000000000000e+00    0.000000000000000e+00    0.000000000000000e+00    0.000000000000000e+00
--------------------------------------------------------------------------------

:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[100/100] : LUP
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 7.66e+06 :::::::: VARIANCE: 1.14e+07 :::::::: MEDIAN: 7.58e+04 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {7.47e+04, 9.91e+04} : (7.66e+04 ± 3.38e+03) : [7.51e+04, 7.89e+04] :::::
:::::: Estimated maximum allocated memory [bytes]: 86752 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
--------------------------------------------------------------------------------
LUP Factorization Results
--------------------------------------------------------------------------------
	order                     : 25
	total iterations          : 0
	converged                 : Yes
	iterative error           : nan
	absolute maximum residual : 1.626303258728257e-19

:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[100/100] : Point Jacobi
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 2.37e+07 :::::::: VARIANCE: 4.29e+10 :::::::: MEDIAN: 9.51e+04 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {9.49e+04, 5.58e+05} : (2.37e+05 ± 2.07e+05) : [9.49e+04, 5.45e+05] :::::
:::::: Estimated maximum allocated memory [bytes]: 464 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
--------------------------------------------------------------------------------
Point Jacobi Method Results
--------------------------------------------------------------------------------
	order                     : 25
	total iterations          : 10
	converged                 : Yes
	iterative error           : 3.831722021940073e-05
	absolute maximum residual : 7.781501987961140e-08

:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[100/100] : SORJ
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 1.13e+07 :::::::: VARIANCE: 3.88e+07 :::::::: MEDIAN: 1.12e+05 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {1.12e+05, 1.57e+05} : (1.13e+05 ± 6.23e+03) : [1.12e+05, 1.12e+05] :::::
:::::: Estimated maximum allocated memory [bytes]: 464 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
--------------------------------------------------------------------------------
SOR Point Jacobi Method Results
--------------------------------------------------------------------------------
	order                     : 25
	total iterations          : 12
	converged                 : Yes
	iterative error           : 2.731521636766889e-05
	absolute maximum residual : 4.263614777215710e-08

:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[100/100] : Gauss Seidel
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 7.01e+06 :::::::: VARIANCE: 2.76e+07 :::::::: MEDIAN: 6.96e+04 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {6.95e+04, 1.22e+05} : (7.01e+04 ± 5.25e+03) : [6.95e+04, 6.96e+04] :::::
:::::: Estimated maximum allocated memory [bytes]: 464 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
--------------------------------------------------------------------------------
Gauss-Seidel Method Results
--------------------------------------------------------------------------------
	order                     : 25
	total iterations          : 7
	converged                 : Yes
	iterative error           : 2.011894422645126e-05
	absolute maximum residual : 3.761859416455708e-09

:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[100/100] : SOR
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 7.99e+06 :::::::: VARIANCE: 8.10e+07 :::::::: MEDIAN: 7.83e+04 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {7.82e+04, 1.38e+05} : (7.99e+04 ± 9.00e+03) : [7.82e+04, 7.84e+04] :::::
:::::: Estimated maximum allocated memory [bytes]: 464 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
--------------------------------------------------------------------------------
SOR Method Results
--------------------------------------------------------------------------------
	order                     : 25
	total iterations          : 8
	converged                 : Yes
	iterative error           : 8.566771904100693e-05
	absolute maximum residual : 3.872281957573891e-08

:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[100/100] : SSOR
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 1.41e+07 :::::::: VARIANCE: 8.64e+07 :::::::: MEDIAN: 1.39e+05 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {1.39e+05, 2.02e+05} : (1.41e+05 ± 9.30e+03) : [1.39e+05, 1.53e+05] :::::
:::::: Estimated maximum allocated memory [bytes]: 464 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
--------------------------------------------------------------------------------
Symmetric SOR Method Results
--------------------------------------------------------------------------------
	order                     : 25
	total iterations          : 7
	converged                 : Yes
	iterative error           : 1.114527235192123e-05
	absolute maximum residual : 1.318599388423596e-09
--------------------------------------------------------------------------------
JSON data has been written to ../project3_4x4.json

Process finished with exit code 0
```
