# NE591 Monorepo - Overview

This is a monorepo for my class project and inlab, outlab deliverables for the `NE591-008` course, titled `Mathematical
and Computational Methods in Nuclear Engineering`. The course aims to provide a theoretical foundation of mathematical
methods applied broadly in nuclear engineering and to construct algorithms to implement the resulting formalisms on
digital computers. The labs involve designing computer programs in low-level languages (exclusively Fortran or C++) and
their implementation, verification, and testing. I have chosen to organize this repo in a manner that builds on the
cumulative code changes from each deliverable, and reuses as many helper functions as possible. A secondary objective is
to benchmark my implementations against the `C++17`, `CUDA`, and `Boost` library functions.

<div style="display: none">[TOC]</div>

1. [Locating Source Files](#locating-source-files)
2. [Building](#building)
    - [Build Environments](#build-environments)
        - [EOS](#eos)
        - [Hazel](#hazel)
    - [Class Project](#class-project)
    - [InLabs and OutLabs](#inlabs-and-outlabs)
    - [Optional CMake Arguments](#optional-cmake-arguments)
3. [Docker Usage Guide](#docker-usage-guide)
    - [Why Docker?](#why-docker)
    - [Building the Docker Image](#building-the-docker-image)
    - [Running the Docker Container](#running-the-docker-container)
    - [Building and Running Tests](#building-and-running-tests)
4. [Development - JetBrains Clion Support](#development---jetbrains-clion-support)
    - [Prerequisites](#prerequisites)
    - [Clion Docker Toolchain](#clion-docker-toolchain)
        - [Instructions](#instructions)
        - [Troubleshooting Windows Issues](#troubleshooting)
    - [Clion Remote Debugging via SSH](#clion-remote-debugging-via-ssh)
5. [License](#license)
6. [FAQs](#faqs)

## Locating Source Files

Sources and file paths for each deliverable follow the following structure:

```
├── README.md   <= You are now here
├── src
│   ├── project     <= Class project deliverables
│   │   ├── project1
│   │   │   ├── main.cpp         <= Stub entrypoint main() method
│   │   │   ├── project1.cpp     <= run() method
│   │   │   ├── Compute.h        <= Core compute logic
│   │   │   ├── Parser.h         <= Argument parsing and error checking 
│   │   │   ├── InputOutputs.h   <= Struct definitions for file I/O
│   │   │   ├── README.md        <= Usage and examples
│   │   │   ├── examples         <= Directory with example inputs and outputs
│   │   │   ├── tests            <= CTests implemented using Google Test Suite
│   │   │   ├── ...
│   │   ├── project2
│   │   ├── ...
│   ├── labs        <= InLab, OutLab deliverables
│   │   ├── inlab1
│   │   │   ├── main.cpp         <= Stub entrypoint main() method
│   │   │   ├── inlab1.cpp       <= run() method
│   │   │   ├── Compute.h        <= Core compute logic
│   │   │   ├── Parser.h         <= Argument parsing and error checking 
│   │   │   ├── InputOutputs.h   <= Struct definitions for file I/O
│   │   │   ├── README.md        <= Usage and examples
│   │   │   ├── examples         <= Directory with example inputs and outputs
│   │   │   ├── tests            <= CTests implemented using Google Test Suite
│   │   │   ├── ...
│   │   ├── outlab1
│   │   ├── inlab2
│   │   ├── outlab2
│   │   ├── ...
│   ├── utils                        <= Helper methods
│   │   ├── math                     
│   │   │   ├── blas                 <= Self implemented Math, BLAS libraries
│   │   ├── CheckBounds.h
│   │   ├── CommandLine.h            <= Other helper functions
│   │   ├── ...
├── ...
├── external                     <= External dependencies
```

The `utils` directory contains helper methods used across the project. These include a helper math library, methods for
checking bounds, interacting with the command line, I/O operations on CSV and JSON files, general helper functions, and
a high-precision profiler for performance analysis.

## Building

The code has been built and tested on the NCSU Hazel`login.hpc.ncsu.edu` and NCSU EOS `remote.eos.ncsu.edu` servers.
It requires no additional configuration except choosing the build target, and output file. Here is a repeatable script
to perform the build and run the `project2` target executable:

### Hazel
There are two ways to do run jobs on Hazel, currently, we support the interactive mode option.
#### Interactive Mode
This mode requires a few additional steps, but for the most part, it follows the same process as the EOS server.
```bash
#!/bin/bash

# Start by logging in
ssh login.hpc.ncsu.edu

# Then, start an interactive session
bsub -Is -n 4 -R "span[hosts=1]" -W 20 bash

# Create a working directory, in this case we call it earthperson_project2 to avoid naming conflicts
mkdir -p /share/$GROUP/$USER/earthperson_project2

# Go into that directory.
cd /share/$GROUP/$USER/earthperson_project2

# Load the build modules
module load openmpi-gcc/openmpi4.1.0-gcc10.2.0 cmake/3.24.1

# Begin by copying the files over using rsync, scp, sftp, etc...
# Then, assuming the repo root is the current directory:

## Specify the build target
export BUILD_TARGET=project2

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export INPUT_PARAMETERS=./src/project/project2/examples/project2_example_input_parameters.json
export INPUT_SOURCETERMS=./src/project/project2/examples/project2_example_source_terms.csv
export OUTPUT_RESULTS=./src/project/project2/examples/project2_example_output_results.json
export OUTPUT_COMPUTED_FLUX=./src/project/project2/examples/project2_example_computed_flux

## Execute
./build/bin/$BUILD_TARGET -i $INPUT_PARAMETERS -s $INPUT_SOURCETERMS -o $OUTPUT_RESULTS -f $OUTPUT_COMPUTED_FLUX
```
#### Batch Jobs

Coming Soon

### EOS
```bash
#!/bin/bash

# Begin by copying the files over using rsync, scp, sftp, etc...
# Then, assuming the repo root is the current directory:

## Specify the build target
export BUILD_TARGET=project2

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Specify the input and output files.
## NOTE: This path is relative to the repo root directory
export INPUT_PARAMETERS=./src/project/project2/examples/project2_example_input_parameters.json
export INPUT_SOURCETERMS=./src/project/project2/examples/project2_example_source_terms.csv

export OUTPUT_RESULTS=./src/project/project2/examples/project2_example_output_results.json
export OUTPUT_COMPUTED_FLUX=./src/project/project2/examples/project2_example_computed_flux

## Execute
./build/bin/$BUILD_TARGET -i $INPUT_PARAMETERS -s $INPUT_SOURCETERMS -o $OUTPUT_RESULTS -f $OUTPUT_COMPUTED_FLUX
```


### Build Environments

The entire codebase has been built and tested on the NCSU Hazel `login.hpc.ncsu.edu` and EOS `remote.eos.ncsu.edu`
servers, which, as of Fall 2023, provide support for the following dependencies:

#### EOS

* `Boost : 1.83.0`
* `C/C++ compiler : GNU 8.5.0`

#### Hazel

* `Boost : 1.83.0`
* `C/C++ compiler : GNU 10.2.0`
* `CUDA: 12.0.0`
* `MPI: OpenMPI 4.1.0`

To facilitate local development, equivalent Dockerfiles have been created. Checkout [eos.Dockerfile](eos.Dockerfile)
and [hazel.Dockerfile](hazel.Dockerfile) and the [Docker instructions](#docker-usage-guide) below.

### Class Project

Currently supported project build targets include:
<table>
  <tr>
    <th colspan="1" style="text-align:center;"><strong>#</strong></th>
    <th colspan="2" style="text-align:center;"><strong>Project Milestone</strong></th>
  </tr>
  <tr>
    <td>1</td>
    <td><code>✅ project1</code>  <code>✅ project1_tests</code></td>
    </tr>
  <tr>
    <td>2</td>
    <td><code>✅ project2</code>  <code>✅ project2_tests</code> <code>✅ physics_diffusion_tests</code><code>✅ blas_tests</code></td>
  </tr>
  <tr>
    <td>3</td>
    <td><code>✅ project3</code>  <code>✅ project3_tests</code><code>✅ blas_tests</code></td>
  </tr>
  <tr>
    <td>4</td>
    <td><code>✅ project4</code>  <code>✅ project4_tests</code></td>
  </tr>
  <tr>
    <td>5</td>
    <td><code>✅ project5</code>  <code>✅ project5_tests</code></td>
  </tr>
</table>

### InLabs and OutLabs

Currently supported inlab, outlab build targets include:

<table>
  <tr>
    <th colspan="1" style="text-align:center;"><strong>#</strong></th>
    <th colspan="1" style="text-align:center;"><strong>InLab</strong></th>
    <th colspan="1" style="text-align:center;"><strong>OutLab</strong></th>
  </tr>
  <tr>
    <td>1</td>
    <td><code>✅ inlab1</code>  <code>❌ inlab1_tests</code></td>
    <td><code>✅ outlab1</code>  <code>❌ outlab1_tests</code></td>
  </tr>
  <tr>
    <td>2</td>
    <td><code>✅ inlab2</code>  <code>❌ inlab2_tests</code></td>
    <td><code>✅ outlab2</code>  <code>❌ outlab2_tests</code></td>
  </tr>
  <tr>
    <td>3</td>
    <td><code>✅ inlab3</code>  <code>❌ inlab3_tests</code></td>
    <td><code>✅ outlab3</code>  <code>❌ outlab3_tests</code></td>
  </tr>
  <tr>
    <td>4</td>
    <td><code>✅ inlab4</code>  <code>❌ inlab4_tests</code></td>
    <td><code>✅ outlab4</code>  <code>✅ outlab4_tests</code></td>
  </tr>
  <tr>
    <td>5</td>
    <td><code>✅ inlab5</code>  <code>❌ inlab5_tests</code></td>
    <td><code>✅ outlab5</code>  <code>✅ outlab5_tests</code></td>
  </tr>
  <tr>
    <td>6</td>
    <td><code>✅ inlab6</code>  <code>✅ inlab6_tests</code></td>
    <td><code>✅ outlab6</code>  <code>✅ outlab6_tests</code></td>
  </tr>
  <tr>
    <td>7</td>
    <td><code>✅ inlab7</code></td>
    <td><code>✅ outlab7</code>  <code>✅ outlab7_tests</code></td>
  </tr>
  <tr>
    <td>8</td>
    <td></td>
    <td></td>
  </tr>
  <tr>
    <td>9</td>
    <td></td>
    <td></td>
  </tr>
  <tr>
    <td>10</td>
    <td><code>✅ inlab10</code>  <code>✅ inlab10_tests</code></td>
    <td></td>
  </tr>
  <tr>
    <td>11</td>
    <td></td>
    <td></td>
  </tr>
  <tr>
    <td>12</td>
    <td></td>
    <td></td>
  </tr>
  <tr>
    <td>13</td>
    <td></td>
    <td></td>
  </tr>
</table>

### Optional CMake Arguments

Available options are:

| CMake Option               | Description                                                                                                                   | Default |
|----------------------------|-------------------------------------------------------------------------------------------------------------------------------|---------|
| BUILD_TESTS                | Build a separate test target (for e.g. `inlab1_tests`)                                                                        | ON      |
| PORTABLE                   | Build without explicitly setting flags `-march=native`, `-mfma`, `-mavx`, `-mavx2`, `-msse4.2`, trading speed for portability | OFF     |
| CODE_COVERAGE              | Perform code coverage analysis                                                                                                | OFF     |
| ENABLE_LTO                 | Enable Link-Time Optimization                                                                                                 | ON      |
| ENABLE_PGO                 | Enable Profile-Guided Optimization                                                                                            | OFF     |
| ENABLE_CODE_QUALITY_CHECKS | Enable extra compile-time warnings                                                                                            | ON      |

To use these options, you can add them to the `cmake` command in the build script. For example, to build with tests, you
would modify the `cmake` command as follows:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=ON
```

## Docker Usage Guide

This repository contains Dockerfiles that are designed to mimic the OS and packages available on the
`login.hpc.ncsu.edu` and `remote.eos.ncsu.edu` servers. These Dockerfiles also set up SSH for remote debugging access
with tools such as [JetBrains Clion](#development---jetbrains-clion-support).

### Why Docker?

Docker is a platform that allows us to package our application and its dependencies into a container which can run on
any Linux/macOS/Windows machine. This helps to eliminate the "it works on my machine" problem. In this case, the
Dockerfiles were needed to ensure that the code can be built, run, and debugged in an environment that closely matches
the `login.hpc.ncsu.edu` and `remote.eos.ncsu.edu` servers.

### Building the Docker Image

The current default [Dockerfile](Dockerfile) is a symlink to [eos.Dockerfile](eos.Dockerfile). You can change it to
build
for [hazel.Dockerfile](hazel.Dockerfile) instead. Alternatively, you can always specify the Dockerfile to use using the
`-f <your.Dockerfile>` option.

To simply run the executables, build an image from the [Dockerfile](Dockerfile) using the following command:

```bash
docker build -t ne591:binary .
```

### Running the Docker Container

To run the Docker container, use the following command:

```bash
docker run --rm -it ne591:binary
```

This command tells Docker to run the container interactively
(i.e., it attaches a terminal session to the running container) and starts a bash shell.

### Building and Running Tests

The Dockerfile includes arguments for building tests and performance benchmarks. To build and run the tests,
you can modify the `docker build` command as follows:

```bash
docker build --build-arg CMAKE_BUILD_TESTS=ON -t ne591:binary .
```

Then, you can run the tests in the Docker container as you would normally do.

## Development - JetBrains Clion Support

You can use [JetBrains Clion](https://www.jetbrains.com/clion/) as the debug environment.
Use Docker as the debug toolchain.

### Prerequisites

* Docker
* CLion

1. In Clion, first set your terminal to Bash.
    1. Go to `Settings / Preferences | Tools | Terminal`
    2. Under `Application Settings`, look for `Shell Path` and set it to `Bash`.

2. Then, build the docker `debugger` target, and save the built image with the tag `eos:debugger`.
      ```shell
          docker build --target=debugger -t eos:debugger .
      ```

### Clion Docker Toolchain

Developing in Clion using the Docker Toolchain provides a consistent experience. The debugging environment is an
ephemeral Docker container based on the `eos:debugger` image you just built.

#### Instructions

1. ##### [Create Docker Toolchain](https://www.jetbrains.com/help/clion/clion-toolchains-in-docker.html#create-docker-toolchain)
    1. Go to `Settings / Preferences | Build, Execution, Deployment | Toolchains`.
    2. Click `Add toolchain` and select `Docker`.
    3. Click the `screw nut icon` next to the `Docker` field to select a Docker image.
        1. You can also configure a `Docker server` in `Settings / Preferences | Build, Execution, Deployment | Docker`
           and then select it in the toolchain settings.
    4. Select the Docker Image `eos:debugger` and wait until the tool detection finishes.
    5. Set the `C Compiler` to `Let CMake Detect`. It should detect `gcc`.
    6. Set the `C++ Compiler` to `Let CMake Detect`. It should detect `g++`.
    7. Set the `Debugger` to `Docker GDB`.
    8. Then save the settings.


2. ##### [Build, Run, Debug with a Docker Toolchain](https://www.jetbrains.com/help/clion/clion-toolchains-in-docker.html#build-run-debug-docker)
    1. After configuring a Docker toolchain, you can select it in `CMake profiles` or in `Makefile` settings.
       Alternatively, move the toolchain to the top of the list to make it default.
    2. The project folder will mount to the Docker container and building, running, and debugging will be
       performed in it. CLion will start the container and shut it down after the command is executed.

### Troubleshooting

* #### [Improve Docker Toolchain performance on Windows](https://www.jetbrains.com/help/clion/clion-toolchains-in-docker.html#windows-performance)
    * To get better performance on Windows, we recommend using Docker with the WSL 2 backend.
        1. Set up `Docker Desktop` with the [WSL 2 backend](https://docs.docker.com/desktop/windows/wsl/).
        2. In the Docker desktop application, navigate to `Settings | Resources | WSL Integration` and enable
           integration
           with your WSL distribution.
        3. Place the project sources into the WSL filesystem, then open it in CLion and configure a Docker toolchain.

### Clion Remote Debugging via SSH

The Dockerfile sets up SSH for root access in the `ssh-debugger` stage. This allows you to connect to the running
container using SSH and debug the code. To do this, you need to build the SSH target and run the container in the
background with port 22 exposed:

```bash
docker build --target=ssh-debugger -t ne591:ssh-debugger .
docker run --privileged --name=eos-ssh-debugger -d -p 2222:22 -it ne591:ssh-debugger
```

Then, you can connect to the running container using SSH. Clion provides native support for debugging remotely. Follow
[these instructions](https://www.jetbrains.com/help/clion/remote-projects-support.html) to set it up.

```bash
ssh root@localhost
```

The password for the root user is "debugger".

## License

This project is licensed under the [Unlicense](http://unlicense.org/) - a license with no conditions whatsoever which
dedicates works to the public domain. Unlicensed works, modifications, and larger works may be distributed under
different terms and without source code.

However, note that this code uses libraries that are bound by their own licenses.

## FAQs

* <b>This is a course project. Why all the extra effort?</b>
    * My hope is that a stripped version of this code can serve as boilerplate so future students can limit all the
      hair-pulling to their algorithms, instead of worrying about I/O and input validation.
