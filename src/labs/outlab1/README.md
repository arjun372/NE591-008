# OutLab 01: Non-vectorized, elementwise (mul, add) operations on 2D matrices

This program performs non-vectorized, elementwise operations on 2D matrices. It takes in three matrices (A, B, F) and a 
scalar (k), and performs the following operations:

- Matrix C = A + B
- Matrix D = k * A
- Matrix E = A * F

## Usage

To run the program, use the following command:

```
./outlab1 -k <scalar> -M <m-rank> -N <n-rank> -J <j-rank>
```

### Parameters

- `-k [ --scalar ] arg`: Scalar multiplier for matrix A
- `-M [ --m-rank ] arg`: Row rank for matrix A and B
- `-N [ --n-rank ] arg`: Column rank for matrix B and row rank for matrix F
- `-J [ --j-rank ] arg`: Column rank for matrix F

### General options

- `-h [ --help ]`: Show help message
- `-q [ --quiet ]`: Reduce verbosity
- `-p [ --precision ] arg (=19)`: Number of digits to represent long double
- `-P [ --profile ]`: Turn on profiling for performance comparison

## Example

```
./outlab1 -k 1.356 -M 7 -N 8 -J 9
```

This command will perform the operations with a scalar of 1.356, and matrices A and B of size 7x8, and matrix F of size 
8x9.
