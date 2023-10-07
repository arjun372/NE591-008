# OutLab 03: Numerical Integration using Composite Newton-Cotes & Gauss-Legendre Quadratures

Supports numerical integration using Gauss-Legendre Quadratures, and the composite trapezoidal and Simpson's rules.
Implements Netwon-Raphson's root finding method to compute nodes and weights for any degree.
File based I/O is supported using JSON files.

## Table of Contents

1. [InLab 03: Numerical Integration using Composite Newton-Cotes Formula](#inlab-03-numerical-integration-using-composite-newton-cotes-formula)
2. [Building & Usage](#building--usage)
    - [Parameters](#parameters)
    - [General options](#general-options)
3. [Implementing your own `f(x)`](#implementing-your-own-fx)
4. [Output Format](#output-format)
    - [Sample Outputs File](#sample-outputs-file)
5. [Example](#example)

## Building & Usage

The code has been built and tested on the `remote.eos.ncsu.edu` servers. It requires no additional
configuration except choosing the build target, and output file. Here is a repeatable script
to perform the build and run the `outlab3` target executable:

```bash
# Assuming cwd is the repo root:
#!/bin/bash

## Specify the build target
export BUILD_TARGET=outlab3

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export OUTPUT_FILE=./src/labs/outlab3/outputs/sample_output.json

## Execute
./build/bin/$BUILD_TARGET -o $OUTPUT_FILE
```

### Parameters

- `-a [ --start ] arg`: Starting value `a` for the integration interval `[a,b]`
- `-b [ --stop ] arg`: Stopping value `b` for the integration interval `[a,b]`
- `-m [ --num-intervals ] arg`: Number of equal intervals from `[a,b]`
- `-t [ --use-trapezoidal ] arg (=1)`: Set quadrature rule to the composite trapezoidal method
- `-s [ --use-simpsons ] arg (=1)`: Set quadrature rule to the composite Simpson's method
- `-g [ --use-gaussian-quadratures ] arg (=1)`: Use Gaussian quadratures for the quadrature rule
- `-n [ --num-quadrature-points ] arg (=1)`: Number of (n) Gauss-Legendre Quadrature points
- `-o [ --output-json ] arg`: Path for the output JSON file

### General options

- `-h [ --help ]`: Show this help message
- `-q [ --quiet ]`: Reduce verbosity
- `-p [ --precision ] arg (=15)`: Number of digits to represent long double
- `-P [ --profile ]`: Turn on profiling for performance comparison

## Implementing your own `f(x)`

You can modify the `user_defined_fx` method located in [extern/function.h](extern/function.h). This code is built as a
static library `outlab3_extern` and linked to the `outlab3` target. After modifying this code, no additional compilation
steps need to be taken. For example, the current implementation returns `sin(x)`.

Here is a brief description of the `user_defined_fx` method:

```c++
/**
 * @brief A user-defined function that calculates the value of the function f(x) = sin(x)
 * @tparam T The type of the input and output. This should be a numeric type (e.g., int, float, double).
 * @param x The input to the function.
 * @return The value of the function at the input x.
 */
template <typename T>
[[maybe_unused]] static inline T user_defined_fx(const T x) {
    const T fx = std::sin(x);
    return fx;
}
```

## Output Format

The output is written to a JSON file.

### Sample Outputs File

```json
{
   "inputs": {
      "a": 0.0,
      "b": 3.141569,
      "m": 1000,
      "n": 8,
      "rules": [
         "trapezoidal",
         "simpsons",
         "gaussian"
      ]
   },
   "outputs": {
      "gaussian": {
         "integral": 1.9999999997202491,
         "quadrature-points": {
            "n0": 0.9602898564975362,
            "n1": 0.7966664774136267,
            "n2": 0.525532409916329,
            "n3": 0.1834346424956498,
            "n4": -0.1834346424956498,
            "n5": -0.525532409916329,
            "n6": -0.7966664774136267,
            "n7": -0.9602898564975362
         },
         "weights": {
            "w0": 0.10122853629037626,
            "w1": 0.22238103445337448,
            "w2": 0.31370664587788727,
            "w3": 0.362683783378362,
            "w4": 0.362683783378362,
            "w5": 0.31370664587788727,
            "w6": 0.22238103445337448,
            "w7": 0.10122853629037626
         }
      },
      "simpsons": {
         "h": 0.003141569,
         "integral": 1.9999999997213362
      },
      "trapezoidal": {
         "h": 0.003141569,
         "integral": 1.9999983548106866
      }
   }
}
```

## Example:

The following is an example of the program's output:

```
./bin/outlab3 -o output_test.json -m 1000 -a 0 -b 3.141592653589793 --use-simpsons true

             .              +   .                .   . .     .  .       .
                               .                    .       .     *
     .        .       *                        . . . .  .   .  + .             .
                        "You Are Here"            .   .  +  . . .
            .                 |             .  .   .    .    . .     .
                              |           .     .     . +.    +  .
.                            \|/            .       .   . .             .
         .          . .       V          .    * . . .  .  +   .
     .                 +      .           .   .      +                      .
                                        .       . +  .+. .
    .         .                      .     . + .  . .     .      .
                       .      .    .     . .   . . .        ! /                   .
      .           *             .    . .  +    .  .       - O -      .
              .       .     .    .  +   . .  *  .       . / |
.                          . + .  .  .  .. +  .                 .               .
            .      .  .  .  *   .  *  . +..  .            *
             .      .   . .   .   .   . .  +   .    .            +        .

	OutLab 03: Numerical Integration using Newton-Cotes & Gauss-Legendre
	Arjun Earthperson
	09/15/2023
--------------------------------------------------------------------------------
	compiler: GNU 8.5.0, boost: 106600 /usr/lib64/libboost_program_options.so
--------------------------------------------------------------------------------
Parameters:
  -a [ --start ] arg                    = Starting value (a) for the 
                                        integration interval [a,b]
  -b [ --stop ] arg                     = Stopping value (b) for the 
                                        integration interval [a,b]
  -m [ --num-intervals ] arg            = Number of equal intervals (m) from 
                                        [a,b]
  -t [ --use-trapezoidal ] arg (=1)     = Set quadrature rule to the composite 
                                        trapezoidal method
  -s [ --use-simpsons ] arg (=1)        = Set quadrature rule to the composite 
                                        Simpson's method
  -g [ --use-gaussian-quadratures ] arg (=1)
                                        = Set quadrature rule to Gauss-Legendre
                                        Quadrature method
  -n [ --num-quadrature-points ] arg    = Number of (n) Gauss-Legendre 
                                        Quadrature points
  -o [ --output-json ] arg              = path for the output JSON file

General options:
  -h [ --help ]                         = Show this help message
  -q [ --quiet ]                        = Reduce verbosity
  -p [ --precision ] arg (=15)          = Number of digits to represent long 
                                        double
  -P [ --profile ]                      = Turn on profiling for performance 
                                        comparison

--------------------------------------------------------------------------------
            Precision in digits:  default: 6, maximum: 19, current: 15
--------------------------------------------------------------------------------
Warning: File already exists at path, will be overwritten 
Would you like to use the trapezoidal rule? [YES/no]: y
y
Would you like to use Simpson's rule? [YES/no]: y
y
Would you like to use Gauss-Legendre Quadrature? [YES/no]: y
y
Enter the number of Gauss-Legendre Quadrature points: 8
8
--------------------------------------------------------------------------------
                                     Inputs
--------------------------------------------------------------------------------
	Interval Count,            m: 1000
	Interval Start,            a: 0
	Interval Stop,             b: 3.141569
	Output JSON,               o: output.json
	Use Trapezoidal Rule,      t: yes
	Use Simpson's Rule,        s: yes
	Use Gaussian Quadratures,  g: yes
--------------------------------------------------------------------------------
                                Outputs
--------------------------------------------------------------------------------
Computing integral using trapezoidal rule for m=(1000) intervals took 3718774 ns
Integral over [a,b] using trapezoidal rule: 1.99999835481069
--------------------------------------------------------------------------------
Computing integral using Simpson's rule for m=(1000) intervals took 491268 ns
Integral over [a,b] using Simpson's rule:   1.99999999972134
--------------------------------------------------------------------------------
Computing integral using Gauss-Legendre Quadrature for n=(8), degree-7 took 205150 ns
Integral over [a,b] using Gauss-Legendre Quadrature:   1.99999999972025
--------------------------------------------------------------------------------
Warning: File already exists at path, will be overwritten 
JSON data has been written to output.json
```
