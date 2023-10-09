#!/bin/bash

## Although you can get away with loading default versions of modules, it's not great practice
## Loading specific versions improves traceability over time.

## Because I care more about portability and than performance,
## I am using openmpi and gcc instead of the intel oneAPI compilers
module load openmpi-gcc/openmpi4.1.0-gcc10.2.0

# Compile the C++17 code, either with gcc, which requires explicit `-lstdc++ -lmpi` flags
gcc -std=c++17 hello_world.cpp -o hello_world -lstdc++ -lmpi

# Or, with mpicxx
#mpicxx -std=c++17 hello_world.cpp -o hello_world


# module load intel/2020.2.254 intel_mpi/2019.8.254
# mpicxx -std=c++17 hello_world.cpp -o hello_world
