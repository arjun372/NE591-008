# InLab 02: I/O Setup for Lagrange Interpolation Polynomials

This program is designed to setup the I/O requirements for performing Lagrange interpolation. It takes in a set of 
points and returns the interpolated values at specified evaluation points. 

File based I/O is supported using CSV (comma separated values) files.

## Usage
```shell
./inlab2 [options]
```

## Options
- `-n [ --num-points ] arg` (optional): Number of interpolation points n.
- `-m [ --num-samples ] arg`: Number of Lagrange interpolation evaluation points.
- `-x [ --x-points ] arg`: Distinct and sorted (x) interpolation points if --input-csv is unset.
- `-f [ --fx-points ] arg`: f(x=n) points if --use-fx-function and --input-csv are unset.
- `-i [ --input-csv ] arg`: Path for input CSV file with two columns [x, f(x)].
- `-o [ --output-csv ] arg`: Path for output CSV file with five columns [i, x, f(x), L(x), E(x)].
- `-F [ --use-fx-function ]`: Use bundled f(x=n) function.

## General options:

- `-h [ --help ]`: Show this help message.
- `-q [ --quiet ]`: Reduce verbosity.
- `-p [ --precision ] arg (=15)`: Number of digits to represent long double.
- `-P [ --profile ]`: Turn on profiling for performance comparison.

## Implementing your own `f(x)`
When using the `--use-fx-function` flag, the code uses outputs from the `fill_fx` method, located in 
[extern/function.cpp](extern/function.cpp). This code is built as a static library `inlab2_extern` and linked to the 
`inlab2` target. When modifying this code, no additional compilation steps need to be taken.

Here is a brief description of the `fx_fill` method, declared in the associated `function.h` header file:
```c++
/**
 * @brief Fill the `fx` vector, optionally using the values in the `x` vector.
 * @param x The input vector containing `x` values.
 * @param fx The output vector to be filled with `fx` values.
 * @details This function takes an input vector `x` and an output vector `fx`.
 *          It fills the `fx` vector with values.
 *          The `fx` vector is assumed to be initially zeroed out.
 *          The function does not return any value.
 * @note The `x` and `fx` vectors must have the same size.
 * @note The `fx` vector will be modified by this function.
 * @note The `x` vector will not be modified by this function.
 */
template <typename T> [[maybe_unused]] void fill_fx(const std::vector<T> &x, std::vector<T> &fx);
```

## File Read/Write
Although data for `x` and `f(x)` can be entered sequentially in interactive mode, it is recommended to use the
option to read an input CSV file. The code looks for the headers `x`and `f(x)`, so please use them precisely. There is a
sample input CSV file under [inputs/sample_input.csv](inputs/sample_input.csv).

## Input Format

The input points can be provided in three ways:

1. Using the `-x, --x-points` option: The distinct and sorted `(x)` interpolation points can be directly specified as 
   command-line arguments. For example:
    ```shell
    ./inlab2 -n 4 -x 0.5 1.0 1.5 2.0 -m 100 -o output.csv
    ```

2. Using the `-i, --input-csv` option: The input points can be read from a CSV file with two columns `[x, f(x)]`. 
   The path to the CSV file should be provided as a command-line argument. For example:
    
   ```shell
    ./inlab2 -i input.csv -m 100 -o output.csv
    ```
   
    ### Sample Inputs File
    ```shell
    $ cat ./sample_input.csv
    x,f(x)
    0.8364,1.0411
    -0.2296,0.7325
    -1.2275,2.3859
    -0.7006,0.3934
    -1.0390,1.1686
    -0.6570,0.3813
    -0.1797,0.7948
    0.0814,1.0743
    -0.1277,0.8583
    1.1261,1.0381
    ```
   
3. Using the interactive shell mode, where the will prompt the user to enter the values one by one. For example: 
    ```shell
    Enter the number of interpolation points: 2
    Enter points for the interval x, sorted, and one at a time: 
    3.2
    5.6
    Enter 2 points for f(x) for every x, one at a time: 
    -1.0
    0.67
    --------------------------------------------------------------------------------
                                         Inputs
    --------------------------------------------------------------------------------
                   x                             f(x)
        +3.200000000000000e+00			-1.000000000000000e+00
        +5.600000000000000e+00			+6.700000000000000e-01
    ....
    ```

## Output Format

The output is written to a CSV file with five columns `[i, x, f(x), L(x), E(x)]`, where:
- `i` is the index of the evaluation point
- `x` is the evaluation point
- `f(x)` is the actual value at the evaluation point
- `L(x)` is the interpolated value at the evaluation point
- `E(x)` is the error between the interpolated value and the actual value

### Sample Outputs File
* `--use-fx-function` set
    If the flag `--use-fx-function` is set, the output contains five columns `[i, x, f(x), L(x), E(x)]`.
    ```shell
    $ cat ./outputs-with-fx-function.csv 
    i,x(i),L(x),f(x),E(x),
    1,0.000000,0.000000,0.000000,
    2,0.000000,0.000000,0.000000,
    3,0.000000,0.000000,0.000000,
    ...
    ```
* `--use-fx-function` unset
  If the flag `--use-fx-function` is not set, the output contains three columns `[i, x, L(x)]`.
    ```shell
    $ cat ./outputs-without-fx-function.csv 
    i,x(i),L(x),
    1,0.000000,0.000000,
    2,0.000000,0.000000,
    3,0.000000,0.000000,
    ...
    ```

## Example:

The following is an example of the program's output:

```shell

                    ███╗   ██╗███████╗      ███████╗ █████╗  ██╗
                    ████╗  ██║██╔════╝      ██╔════╝██╔══██╗███║
                    ██╔██╗ ██║█████╗  █████╗███████╗╚██████║╚██║
                    ██║╚██╗██║██╔══╝  ╚════╝╚════██║ ╚═══██║ ██║
                    ██║ ╚████║███████╗      ███████║ █████╔╝ ██║
                    ╚═╝  ╚═══╝╚══════╝      ╚══════╝ ╚════╝  ╚═╝
        
--------------------------------------------------------------------------------
	InLab 02: I/O Setup for Lagrange Interpolation Polynomials
	Arjun Earthperson
	09/01/2023
--------------------------------------------------------------------------------
Build Configuration
compiler: GNU 11.4.0
flags: -Wall;-Wextra;-Werror;-Wnon-virtual-dtor;-Wold-style-cast;-Wsign-compare;-Wmissing-field-initializers;-Wno-unused-function;-march=native;-g;-fPIE
Boost: 1.74.0Boost::program_options
--------------------------------------------------------------------------------

Parameters:
  -n [ --num-points ] arg       = (optional) number of interpolation points n
  -m [ --num-samples ] arg      = number of Lagrange interpolation evaluation 
                                points
  -x [ --x-points ] arg         = distinct and sorted (x) interpolation points 
                                if --input-csv is unset
  -f [ --fx-points ] arg        = f(x=n) points if --use-fx-function and 
                                --input-csv are unset
  -i [ --input-csv ] arg        = path for input CSV file with two columns [x, 
                                f(x)]
  -o [ --output-csv ] arg       = path for output CSV file with five columns 
                                [i, x, f(x), L(x), E(x)]
  -F [ --use-fx-function ]      = use bundled f(x=n) function

General options:
  -h [ --help ]                 = Show this help message
  -q [ --quiet ]                = Reduce verbosity
  -p [ --precision ] arg (=15)  = Number of digits to represent long double
  -P [ --profile ]              = Turn on profiling for performance comparison

--------------------------------------------------------------------------------
Precision in digits:  default: 6, maximum: 19, current: 15
--------------------------------------------------------------------------------
Reading from file: /tmp/ne591-008/cmake-build-debug-docker/sample_input.csv
--------------------------------------------------------------------------------
                                     Inputs
--------------------------------------------------------------------------------
               x                             f(x)
	+1.041100000000000e+00			+8.364000000000000e-01
	+7.325000000000000e-01			-2.296000000000000e-01
	+2.385900000000000e+00			-1.227500000000000e+00
	+3.934000000000000e-01			-7.006000000000000e-01
	+1.168600000000000e+00			-1.039000000000000e+00
	+3.813000000000000e-01			-6.570000000000000e-01
	+7.948000000000000e-01			-1.797000000000000e-01
	+1.074300000000000e+00			+8.140000000000000e-02
	+8.583000000000000e-01			-1.277000000000000e-01
	+1.038100000000000e+00			+1.126100000000000e+00
--------------------------------------------------------------------------------
	num-points,     n: 10
	num-samples,    m: 1000
	input-csv,      i: /tmp/ne591-008/cmake-build-debug-docker/sample_input.csv
	output-csv,     o: /tmp/ne591-008/cmake-build-debug-docker/output2.csv
--------------------------------------------------------------------------------

Process finished with exit code 0
```
