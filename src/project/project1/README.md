# Project Milestone 1: LUP Factorization based Serial Diffusion Equation Solver

Serial 2D, one speed, time-independent, neutron diffusion equation solver. Matrix equation solved using LU factorization
with pivotin. File I/O is performed using a combination of `CSV` and `JSON` files.

## Table of Contents

1. [Project Milestone 1: LUP Factorization based Serial Diffusion Equation Solver](#project-milestone-1-lup-factorization-based-serial-diffusion-equation-solver)
2. [Building & Usage](#building--usage)
   - [Parameters](#parameters)
   - [Solver Methods](#solver-methods)
   - [General options](#general-options)
3. [Input Format](#input-format)
   - [Sample Input Files](#sample-input-files)
     - [[REQUIRED] Source Term q(i,j) CSV](#source-term-qij-csv-required)
     - [Parameters JSON](#parameters-json)
4. [Output Format](#output-format)
   - [Sample Outputs Files](#sample-outputs-files)
     - [Computed Flux 𝜙(𝑖,𝑗) CSV](#computed-flux-𝜙𝑖𝑗-csv)
     - [Results JSON](#results-json)
5. [Example](#example)

## Building & Usage

The code has been built and tested on the `remote.eos.ncsu.edu` servers. It requires no additional
configuration except choosing the build target, and output file. Here is a repeatable script
to perform the build and run the `project1` target executable:

```bash
# Assuming cwd is the repo root:
#!/bin/bash

## Specify the build target
export BUILD_TARGET=project1

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export INPUT_PARAMETERS=./src/labs/project1/examples/example_input_parameters.json
export INPUT_SOURCETERMS=./src/labs/project1/examples/example_source_terms.csv

export OUTPUT_RESULTS=./src/labs/project1/examples/example_output_results.json
export OUTPUT_COMPUTED_FLUX=./src/labs/project1/examples/example_computed_flux.csv

## Execute
./build/bin/$BUILD_TARGET -i $INPUT_PARAMETERS -s $INPUT_SOURCETERMS -o $OUTPUT_RESULTS -f $OUTPUT_COMPUTED_FLUX
```

### Parameters

- `-a arg`: Length of 1st dimension (+ve real)
- `-b arg`: Length of 2nd dimension (+ve real)
- `-m arg`: Number of mesh-points in 1st dimension
- `-n arg`: Number of mesh-points in 2nd dimension
- `-D arg`: Diffusion coefficient D (+ve real)
- `--cross-section arg`: Removal cross-section Σₐ (+ve real)
- `-i [ --input-parameter-json ] arg`: Path to input parameter JSON
- `-s [ --source-terms-csv ] arg`: Path to source-terms 𝑞(𝑖,𝑗) CSV
- `-o [ --output-results-json ] arg`: Path to output results JSON
- `-f [ --output-flux-csv ] arg`: Path to computed flux 𝜙(𝑖,𝑗) CSV

### Solver Methods

- `--use-LUP`: Use the LUP method
- `--use-point-jacobi`: [DISABLED] Use the Point-Jacobi method
- `--use-gauss-seidel`: [DISABLED] Use the Gauss-Seidel method
- `--use-SOR`: [DISABLED] Use the SOR method

### General options

- `-h [ --help ]`: Show this help message
- `-q [ --quiet ]`: Reduce verbosity
- `-p [ --precision ] arg (=15)`: Number of digits to represent long double
- `-P [ --profile ]`: Turn on performance profiling
- 
## Input Format

<p style="background: black"><span style="font-weight: 900; color: red">CAUTION:</span> There are <span style="font-weight: 900; color: red">TWO</span> input files!</p> 

### Sample Input Files

#### Source Term q(i,j) CSV [REQUIRED]:
   One `.csv` file, containing a 2D array (m x n), with the source terms in the following format:
   ```text
   1.1,1.2,1.3
   2.1,2.2,2.3
   3.1,3.2,3.3
   ```

#### Parameters JSON:
   One `.json` file, containing quantifcation parameters. If not supplied, parameters will be read interactively over the
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
      }
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
       "outputs": {
           "max_residual": 2.168404344971009e-19,
           "residual": [
               0,
               1.0842021724855044e-19,
               0,
               -2.168404344971009e-19,
               2.168404344971009e-19,
               0,
               2.168404344971009e-19,
               0,
               2.168404344971009e-19
           ],
           "solution": [
              322.6797742410937,
              360.9424588400793,
              377.7344809335781,
              615.8771291565212,
              663.981599580839,
              672.5292781157306,
              873.2268411659375,
              927.4639484321724,
              928.2815478584218
           ]
       },
      "inputs": {
         "D": 0.07,
         "cross-section": 0.0033,
         "dimensions": {
            "a": 100.0,
            "b": 100.0
         },
         "mesh": {
            "m": 3,
            "n": 3,
            "𝛾": 25.0,
            "𝛿": 25.0
         },
         "methods": [
            "LUP"
         ]
      }

}
```

## Example:

The following is an example of the program's output:

```shell
NE591: Project Milestone 1: Serial Neutron Diffusion Code
Arjun Earthperson
09/29/2023
--------------------------------------------------------------------------------
compiler: GNU 8.5.0, boost: 106600 /usr/lib64/libboost_program_options.so
--------------------------------------------------------------------------------
Parameters:
  -a arg                            = Length of 1st dimension (+ve real)
  -b arg                            = Length of 2nd dimension (+ve real)
  -m arg                            = Number of mesh-points in 1st dimension
  -n arg                            = Number of mesh-points in 2nd dimension
  -D arg                            = Diffusion coefficient D (+ve real)
  --cross-section arg               = Removal cross-section Σₐ (+ve real)
  -i [ --input-parameter-json ] arg = Path to input parameter JSON
  -s [ --source-terms-csv ] arg     = Path to source-terms 𝑞(𝑖,𝑗) CSV
  -o [ --output-results-json ] arg  = Path to output results JSON
  -f [ --output-flux-csv ] arg      = Path to computed flux 𝜙(𝑖,𝑗) CSV

Solver Methods:
  --use-LUP                         = Use the LUP method
  --use-point-jacobi                = [DISABLED] Use the Point-Jacobi method
  --use-gauss-seidel                = [DISABLED] Use the Gauss-Seidel method
  --use-SOR                         = [DISABLED] Use the SOR method

General options:
  -h [ --help ]                     = Show this help message
  -q [ --quiet ]                    = Reduce verbosity
  -p [ --precision ] arg (=15)      = Number of digits to represent long double
  -P [ --profile ]                  = Turn on performance profiling

--------------------------------------------------------------------------------
			Precision in digits:  default: 6, maximum: 19, current: 1
--------------------------------------------------------------------------------
Warning: File already exists at path, will be overwritten 
Warning: File already exists at path, will be overwritten 
--------------------------------------------------------------------------------
                                     Inputs
--------------------------------------------------------------------------------
	Input JSON                              i: ../example_input_parameters.json
	Output JSON                             o: ../example_output_results.json
	----
	Use LUP method                           : Yes
	Use Point-Jacobi method                  : No
	Use Gauss-Seidel method                  : No
	Use SOR method                           : No
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
	Mesh spacing in the 1st dimension,     𝛿: 2e+01
	Mesh spacing in the 2nd dimension,     𝛾: 2e+01
--------------------------------------------------------------------------------
Source terms 𝑞(𝑖,𝑗):
--------------------------------------------------------------------------------
    1.1e+00    1.2e+00    1.3e+00
    2.1e+00    2.2e+00    2.3e+00
    3.1e+00    3.2e+00    3.3e+00
--------------------------------------------------------------------------------
Diffusion Matrix A: 
--------------------------------------------------------------------------------
    3.7e-03   -1.1e-04    0.0e+00   -1.1e-04    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00
   -1.1e-04    3.7e-03   -1.1e-04    0.0e+00   -1.1e-04    0.0e+00    0.0e+00    0.0e+00    0.0e+00
    0.0e+00   -1.1e-04    3.7e-03    0.0e+00    0.0e+00   -1.1e-04    0.0e+00    0.0e+00    0.0e+00
   -1.1e-04    0.0e+00    0.0e+00    3.7e-03   -1.1e-04    0.0e+00   -1.1e-04    0.0e+00    0.0e+00
    0.0e+00   -1.1e-04    0.0e+00   -1.1e-04    3.7e-03   -1.1e-04    0.0e+00   -1.1e-04    0.0e+00
    0.0e+00    0.0e+00   -1.1e-04    0.0e+00   -1.1e-04    3.7e-03    0.0e+00    0.0e+00   -1.1e-04
    0.0e+00    0.0e+00    0.0e+00   -1.1e-04    0.0e+00    0.0e+00    3.7e-03   -1.1e-04    0.0e+00
    0.0e+00    0.0e+00    0.0e+00    0.0e+00   -1.1e-04    0.0e+00   -1.1e-04    3.7e-03   -1.1e-04
    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00   -1.1e-04    0.0e+00   -1.1e-04    3.7e-03
--------------------------------------------------------------------------------
Right Hand Side Vector B: 
--------------------------------------------------------------------------------
    1.1e+00    1.2e+00    1.3e+00    2.1e+00    2.2e+00    2.3e+00    3.1e+00    3.2e+00    3.3e+00
--------------------------------------------------------------------------------
Lower Triangular Matrix (L):
--------------------------------------------------------------------------------
    1.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00
   -3.0e-02    1.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00
    0.0e+00   -3.0e-02    1.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00
   -3.0e-02   -8.9e-04   -2.7e-05    1.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00
    0.0e+00   -3.0e-02   -8.9e-04   -3.0e-02    1.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00
    0.0e+00    0.0e+00   -3.0e-02   -8.0e-07   -3.0e-02    1.0e+00    0.0e+00    0.0e+00    0.0e+00
    0.0e+00    0.0e+00    0.0e+00   -3.0e-02   -9.0e-04   -2.7e-05    1.0e+00    0.0e+00    0.0e+00
    0.0e+00    0.0e+00    0.0e+00    0.0e+00   -3.0e-02   -9.0e-04   -3.0e-02    1.0e+00    0.0e+00
    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00   -3.0e-02   -8.0e-07   -3.0e-02    1.0e+00
--------------------------------------------------------------------------------
Upper Triangular Matrix (U):
--------------------------------------------------------------------------------
    3.7e-03   -1.1e-04    0.0e+00   -1.1e-04    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00
    0.0e+00    3.7e-03   -1.1e-04   -3.3e-06   -1.1e-04    0.0e+00    0.0e+00    0.0e+00    0.0e+00
    0.0e+00    0.0e+00    3.7e-03   -1.0e-07   -3.3e-06   -1.1e-04    0.0e+00    0.0e+00    0.0e+00
    0.0e+00    0.0e+00    0.0e+00    3.7e-03   -1.1e-04   -3.0e-09   -1.1e-04    0.0e+00    0.0e+00
    0.0e+00    0.0e+00    0.0e+00    0.0e+00    3.7e-03   -1.1e-04   -3.4e-06   -1.1e-04    0.0e+00
    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    3.7e-03   -1.0e-07   -3.4e-06   -1.1e-04
    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    3.7e-03   -1.1e-04   -3.0e-09
    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    3.7e-03   -1.1e-04
    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    3.7e-03
--------------------------------------------------------------------------------
Permutation Matrix (P):
--------------------------------------------------------------------------------
    1.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00
    0.0e+00    1.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00
    0.0e+00    0.0e+00    1.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00
    0.0e+00    0.0e+00    0.0e+00    1.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00
    0.0e+00    0.0e+00    0.0e+00    0.0e+00    1.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00
    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    1.0e+00    0.0e+00    0.0e+00    0.0e+00
    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    1.0e+00    0.0e+00    0.0e+00
    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    1.0e+00    0.0e+00
    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00    1.0e+00
--------------------------------------------------------------------------------
Permuted constants (Pb = P * b):
--------------------------------------------------------------------------------
    1.1e+00    1.2e+00    1.3e+00    2.1e+00    2.2e+00    2.3e+00    3.1e+00    3.2e+00    3.3e+00
--------------------------------------------------------------------------------
Intermediate vector (y), where (Ly = Pb):
--------------------------------------------------------------------------------
    1.1e+00    1.2e+00    1.3e+00    2.1e+00    2.3e+00    2.4e+00    3.2e+00    3.4e+00    3.5e+00
--------------------------------------------------------------------------------
Solution vector (x, phi), where (Ux = y):
--------------------------------------------------------------------------------
    3.2e+02    3.6e+02    3.8e+02    6.2e+02    6.6e+02    6.7e+02    8.7e+02    9.3e+02    9.3e+02
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
Computed flux 𝜙(𝑖,𝑗): 
--------------------------------------------------------------------------------
    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00
    0.0e+00    3.2e+02    3.6e+02    3.8e+02    0.0e+00
    0.0e+00    6.2e+02    6.6e+02    6.7e+02    0.0e+00
    0.0e+00    8.7e+02    9.3e+02    9.3e+02    0.0e+00
    0.0e+00    0.0e+00    0.0e+00    0.0e+00    0.0e+00
--------------------------------------------------------------------------------
Residual vector (r = b - Ax) :
--------------------------------------------------------------------------------
    0.0e+00    1.1e-19    0.0e+00   -2.2e-19    2.2e-19    0.0e+00    2.2e-19    0.0e+00    2.2e-19
--------------------------------------------------------------------------------
Max Residual abs(r): 2.1684043449710088680e-19
--------------------------------------------------------------------------------

JSON data has been written to profile_3x3.json

Process finished with exit code 0

```
