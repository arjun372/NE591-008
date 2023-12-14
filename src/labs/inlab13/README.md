# InLab 13: Fixed Point Iterative Method
`inlab13` builds on `inlab13`, implementing a new memory-profiler. Benchmark results and analysis are listed in the 
`analysis` folder. Checkout the associated [python notebook for analysis results](analysis/Project_Milestone_3_Analysis.ipynb).

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
to perform the build and run the `inlab13` target executable:

### EOS
```bash
#!/bin/bash

# Begin by copying the files over using rsync, scp, sftp, etc...
# Then, assuming the repo root is the current directory:

## Specify the build target
export BUILD_TARGET=inlab13

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export INPUT_PARAMETERS=./src/labs/inlab13/examples/inlab13_input.json
export OUTPUT_RESULTS=./src/labs/inlab13/examples/inlab13_output.json

## Execute
./build/bin/$BUILD_TARGET -i $INPUT_PARAMETERS -o $OUTPUT_RESULTS 
```

---

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
   "rho": 0.07,
   "beta": 0.0033,
   "dimensions": {
      "L": 4
   },
   "mesh": {
      "n": 32
   },
   "threshold": 1e-4,
   "max-iterations": 5000,
   "methods": [
      "fixed-point",
      "newton"
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
   "rho": 0.07,
   "beta": 0.0033,
   "dimensions": {
      "L": 4
   },
   "mesh": {
      "n": 32
   },
   "threshold": 1e-4,
   "max-iterations": 5000,
   "methods": [
      "fixed-point",
      "newton"
   ]
}

```

## Example:

The following is an example of the program's output:

```shell
NE591: InLab 13: Fixed Point Diffusion Equation Solver
Arjun Earthperson
11/12/2023
--------------------------------------------------------------------------------
using 128-bit floats
compiler: GNU 8.5.0
boost: 106600 /usr/lib64/libboost_program_options.so;/usr/lib64/libboost_serialization.so
--------------------------------------------------------------------------------
Parameters:
  -L arg                                = Length of 1st dimension (+ve real)
  -r arg                                = removal term [linear]
  -b arg                                = removal term [non-linear]
  -n arg                                = Number of nodes per dimension 
                                        excluding boundary nodes

Solver Options:
  --use-fixed-point                     = Use fixed point iteration
  --use-newton                          = Use newton's method
  -t [ --threshold ] arg                = convergence threshold [𝜀 > 0]
  -k [ --max-iterations ] arg           = maximum iterations [n ∈ ℕ]

Inputs/Outputs:
  -i [ --input-parameter-json ] arg     = Path to input parameter JSON
  -o [ --output-results-json ] arg (=/dev/stdout)
                                        = Path to output results JSON

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
INFO: Input parameter JSON filepath not provided. Parameters will be specified interactively
Enter a value for L:32
32
Enter a value for n:4
4
Enter a value for rho:0.5
0.5
Enter a value for beta:2
2
Enter a value for threshold:1e-4
1e-4
Enter a value for max-iterations:5000
5000
Would you like to use the fixed point iteration? [YES/no]: y
y
Would you like to use the newton iteration? [YES/no]: n
n
--------------------------------------------------------------------------------
                                     Inputs
--------------------------------------------------------------------------------
	Input JSON,     i: None
	Output JSON,    o: ../project3_16x16.json
	----
	Convergence Threshold,                  𝜀: 0.0001
	Max iterations,                         k: 5000
	Use Fixed Point Iterations               : Yes
	Use Newton's method                      : No
	----
	Length,                                 L: 32
	rho,                                  rho: 0.5
	beta,                                beta: 2
	Mesh-Points,                            n: 4
--------------------------------------------------------------------------------
:::::::::::::::::::::::::::: PROFILE SUMMARY [ns] ::::::::::::::::::::::::::::::
[2/2] : Fixed Point Method
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
:::::: SUM: 1.83e+04 :::::::: VARIANCE: 5.34e+05 :::::::: MEDIAN: 9.14e+03 :::::
:::::: {     MIN,      MAX} : (AVERAGE  ± STD.DEV.) : [PCT_05th, PCT_95th] :::::
:::::: {8.41e+03, 9.87e+03} : (9.14e+03 ± 7.31e+02) : [8.48e+03, 9.80e+03] :::::
:::::: Estimated maximum allocated memory [bytes]: 192 ::::::::::::::::::::::::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:::::: Converged        :     Yes
:::::: Iterations       :     2
:::::: Iterative Error  :     5.26e-02
:::::: Iterate Vector x :     0.00e+00    0.00e+00    0.00e+00    0.00e+00    0.00e+00    0.00e+00

Final scalar flux distribution:
0.400000 0.400000 0.400000 0.400000 
0.400000 0.400000 0.400000 0.400000 
0.400000 0.400000 0.400000 0.400000 
0.400000 0.400000 0.400000 0.400000 
Average Flux: 0.400000
--------------------------------------------------------------------------------
JSON data has been written to ../project3_16x16.json

Process finished with exit code 0

```
