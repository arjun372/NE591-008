# InLab 07: MPI Parallel Hello World

Simple `hello world` functionality, implemented using MPI. For scripting, I am using `bash` instead of `csh`. `bash` is
easier to use, has more functionality, and brings us out of the dark ages.

## I/O
The files [stdout.txt](stdout.txt) and [stderr.txt](stderr.txt) are the output files from my batched job run. Ofcourse,
[input](input) is the input file.

## Loading Modules
Although you can get away with loading default versions of modules, it's not great practice. Loading specific versions
improves traceability over time. Also, because I care more about code portability and than performance, I am using 
openmpi and gcc instead of the intel oneAPI compilers
```bash
#!/bin/bash
module load openmpi-gcc/openmpi4.1.0-gcc10.2.0
```

## Compiling C++
The [compile_inlab7.sh](compile_inlab7.sh) file contains the batching commands. Compile the `c++17` code, either with 
`gcc`, which requires explicit `-lstdc++ -lmpi` flags, or with `mpicxx`. Either option works.
```bash
#!/bin/bash

gcc -std=c++17 hello_world.cpp -o hello_world -lstdc++ -lmpi

mpicxx -std=c++17 hello_world.cpp -o hello_world

## Intel alternatives
# module load intel/2020.2.254 intel_mpi/2019.8.254
# mpicxx -std=c++17 hello_world.cpp -o hello_world
```

## Batched Jobs
The [submit_inlab7.sh](submit_inlab7.sh) file contains the batching commands.
```bash
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
```

## Running in Interactive Mode
1. Login to `Hazel` at `login.hpc.ncsu.edu`
2. Start an interactive session for 10 minutes: `bsub -Is -n 4 -R "span[hosts=1]" -W 10 bash`
3. Navigate to the shared directory: `cd /share/$GROUP/$USER`
4. Create a working directory: `mkdir -p Lab7/inlab`
5. Go to the directory, copy over the compile and submit scripts
6. Run the compile script `./compile_inlab7.sh`
7. Run the build target directly `mpirun ./hello_world`

##### All together:
```bash
$ ssh login.hpc.ncsu.edu
$ bsub -Is -n 4 -R "span[hosts=1]" -W 20 bash
$ mkdir -p /share/$GROUP/$USER/Lab7/inlab && cd /share/$GROUP/$USER/Lab7/inlab
$ # copy files over, I used rsync
$ ./compile_inlab7.sh
$ mpirun ./hello_world
```

## Example Output
```shell
[aarjun@c056n02 inlab]$ mpirun ./hello_world 

                 ███╗   ██╗███████╗      ███████╗ █████╗  ██╗
                 ████╗  ██║██╔════╝      ██╔════╝██╔══██╗███║
                 ██╔██╗ ██║█████╗  █████╗███████╗╚██████║╚██║
                 ██║╚██╗██║██╔══╝  ╚════╝╚════██║ ╚═══██║ ██║
                 ██║ ╚████║███████╗      ███████║ █████╔╝ ██║
                 ╚═╝  ╚═══╝╚══════╝      ╚══════╝ ╚════╝  ╚═╝
================================================================================
InLab 07: MPI Parallel Hello World
Arjun Earthperson
10/06/2023
================================================================================
                             Hello parallel worlds!                             
--------------------------------------------------------------------------------
Hello arjun from MPI Task 0 on host c056n02
[c056n02][0]: Execution time (sec) = 9.1e-08
--------------------------------------------------------------------------------
Hello arjun from MPI Task 2 on host c056n02
[c056n02][2]: Execution time (sec) = 1.63e-07
--------------------------------------------------------------------------------
Hello arjun from MPI Task 3 on host c056n02
[c056n02][3]: Execution time (sec) = 1.19e-07
--------------------------------------------------------------------------------
Hello arjun from MPI Task 1 on host c056n02
[c056n02][1]: Execution time (sec) = 7.8e-08
--------------------------------------------------------------------------------
[c056n02][3]: I log my errors here
[c056n02][0]: I log my errors here
[c056n02][1]: I log my errors here
[c056n02][2]: I log my errors here
```
