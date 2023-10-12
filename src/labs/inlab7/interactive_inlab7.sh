#!/bin/bash

WD="/share/$GROUP/$USER/Lab7/inlab/"

#BSUB -J  hello_world
#BSUB -oo $WD/hello_world.out.%J
#BSUB -e  $WD/hello_world.err.%J
#BSUB -W  00:1
#BSUB -n  4

# Load the necessary modules
module load openmpi-gcc/openmpi4.1.0-gcc10.2.0

# Compile the C++ code
gcc -std=c++17 $WD/hello_world.cpp -o $WD/hello_world -lstdc++ -lmpi

# Run the program
mpirun $WD/hello_world
