# InLab 01: Iterative Taylor series approximation of sin(x)

It uses an iterative Taylor series approximation to calculate the sine of an angle.

## Usage
The program runs in interactive mode. However, input arguments can be provided for convenience. 
The program accepts the following input variables:

- `-x [ --angle ] arg`: The angle in radians. The absolute value of x should be less than 1.0.
- `-t [ --convergence-threshold ] arg`: The iterative convergence threshold. e should be greater than 0.
- `-n [ --iterations ] arg`: The total number of iterations.

The program also has the following general options:

- `-h [ --help ]`: Show the help message.
- `-q [ --quiet ]`: Reduce verbosity.
- `-p [ --precision ] arg (=19)`: The number of digits to represent double_t. The default value is 19.

## Example

To run the program with an angle of 0.5 radians, a convergence threshold of 1e-8, and 100 iterations, use the following command:

```
./inlab1 -x 0.5 -t 1e-9 -n 100
```

To reduce verbosity and set the precision to 10 digits, use the following command:

```
./inlab1 -x 0.5 -t 1e-9 -n 100 -q
```
