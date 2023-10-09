#!/bin/bash

WD="/share/$GROUP/$USER/Lab7/inlab/"

#BSUB -J hello_world
#BSUB -o stdout.%J
#BSUB -e stderr.%J
#BSUB -W 00:1
#BSUB -n 4

# Load the necessary modules
module load openmpi-gcc/openmpi4.1.0-gcc10.2.0

# Run the program
mpirun $WD/hello_world
