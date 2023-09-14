# OutLab 03: Numerical Integration using Composite Newton-Cotes & Gauss-Legendre Quadrature

Supports numerical integration using the composite trapezoidal and simpson's rules. 
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
to perform the build and run the `inlab3` target executable:

```bash
# Assuming cwd is the repo root:
#!/bin/bash

## Specify the build target
export BUILD_TARGET=inlab3

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export OUTPUT_FILE=./src/labs/inlab3/outputs/sample_output.json

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
- `-o [ --output-json ] arg`: Path for the output JSON file

### General options

- `-h [ --help ]`: Show this help message
- `-q [ --quiet ]`: Reduce verbosity
- `-p [ --precision ] arg (=15)`: Number of digits to represent long double
- `-P [ --profile ]`: Turn on profiling for performance comparison

## Implementing your own `f(x)`
You can modify the `user_defined_fx` method located in [extern/function.h](extern/function.h). This code is built as a 
static library `inlab3_extern` and linked to the `inlab3` target. After modifying this code, no additional compilation 
steps need to be taken. For example, the current implementation returns `sin(x)`.

Here is a brief description of the `fx_fill` method:
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
    "b": 3.141592653589793,
    "m": 1000,
    "rules": [
      "trapezoidal",
      "simpsons"
    ]
  },
  "outputs": {
    "simpsons": {
      "h": 0.003141592653589793,
      "integral": 2.0000000000010822
    },
    "trapezoidal": {
      "h": 0.003141592653589793,
      "integral": 1.9999983550656626
    }
  }
}
```

## Example:

The following is an example of the program's output:

```
./bin/inlab3 -o output_test.json -m 1000 -a 0 -b 3.141592653589793 --use-simpsons true

                    ███╗   ██╗███████╗      ███████╗ █████╗  ██╗
                    ████╗  ██║██╔════╝      ██╔════╝██╔══██╗███║
                    ██╔██╗ ██║█████╗  █████╗███████╗╚██████║╚██║
                    ██║╚██╗██║██╔══╝  ╚════╝╚════██║ ╚═══██║ ██║
                    ██║ ╚████║███████╗      ███████║ █████╔╝ ██║
                    ╚═╝  ╚═══╝╚══════╝      ╚══════╝ ╚════╝  ╚═╝
        
--------------------------------------------------------------------------------
	InLab 03: Numerical Integration using Composite Newton-Cotes Formulas
	Arjun Earthperson
	09/08/2023
--------------------------------------------------------------------------------
Build Configuration
compiler: GNU 8.5.0
flags: -Wall;-Wextra;-Werror;-Wnon-virtual-dtor;-Wold-style-cast;-Wsign-compare;-Wmissing-field-initializers;-Wno-unused-parameter;-Wno-unused-function;-march=native;-g;-fPIE
Boost: 106600; /usr/lib64/libboost_program_options.so
--------------------------------------------------------------------------------
Parameters:
  -a [ --start ] arg                    = Starting value a for the integration 
                                        interval [a,b]
  -b [ --stop ] arg                     = Stopping value b for the integration 
                                        interval [a,b]
  -m [ --num-intervals ] arg            = Number of equal intervals from [a,b]
  -t [ --use-trapezoidal ] arg (=1)     = Set quadrature rule to the composite 
                                        trapezoidal method
  -s [ --use-simpsons ] arg (=1)        = Set quadrature rule to the composite 
                                        Simpson's method
  -g [ --use-gaussian-quadratures ] arg (=1)
                                        = Use Gaussian quadratures for as the 
                                        quadrature rule
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
Would you like to use the trapezoidal rule? [YES/no]: y
y
Would you like to use Gaussian quadratures? [YES/no]: y
y
--------------------------------------------------------------------------------
                                     Inputs
--------------------------------------------------------------------------------
	Interval Count,            m: 1000
	Interval Start,            a: 0
	Interval Stop,             b: 3.14159265358979
	Output JSON,               o: output_test.json
	Use Trapezoidal Rule,      t: yes
	Use Simpson's Rule,        s: yes
	Use Gaussian Quadratures,  g: yes
--------------------------------------------------------------------------------
                                Outputs
--------------------------------------------------------------------------------
Computing integral using trapezoidal rule for m=(1000) intervals took 1378674 ns
Integral over [a,b] using trapezoidal rule: 1.99999835506566
--------------------------------------------------------------------------------
Computing integral using Simpson's rule for m=(1000) intervals took 153407 ns
Integral over [a,b] using Simpson's rule:   2.00000000000108
--------------------------------------------------------------------------------
	Gauss-Legendre Quadrature not available yet.
--------------------------------------------------------------------------------
JSON data has been written to output_test.json

Process finished with exit code 0
```
