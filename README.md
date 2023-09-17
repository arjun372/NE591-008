# NE591 Lab MonoRepo #

This is a monorepo for my in- and out- lab deliverables for the `NE591-008` course. I have chosen to organize it 
this way to build on the cumulative code changes from each lab, and to reuse as many helper functions as possible. 
A secondary objective is to benchmark my implementations against the `Boost` library functions. 

## Table of Contents
1. [NE591 Lab MonoRepo](#NE591-Lab-MonoRepo)
2. [Locating Source Files](#Locating-Source-Files)
3. [Building](#Building)
    - [Build Targets](#Build-Targets)
    - [Optional CMake Arguments](#Optional-CMake-Arguments)
4. [Docker Usage Guide](#Docker-Usage-Guide)
    - [Why Docker?](#Why-Docker?)
    - [Building the Docker Image](#Building-the-Docker-Image)
    - [Running the Docker Container](#Running-the-Docker-Container)
    - [Debugging the Code](#Debugging-the-Code)
    - [Building and Running Tests](#Building-and-Running-Tests)

## Locating Source Files ##
Sources and file paths for each lab follow the following structure:
```
├── README.md   <= You are now here
├── src
│   ├── labs
│   │   ├── inlab1
│   │   │   ├── inlab1.cpp       <= Entrypoint main() and run() methods
│   │   │   ├── Compute.h        <= Core compute logic
│   │   │   ├── Parser.h         <= Input processing and error checking 
│   │   │   ├── README.md        <= Usage and examples
│   │   │   ├── ...
│   │   ├── outlab1
│   │   ├── inlab2
│   │   ├── ...
│   ├── ...
├── ...
├── utils                        <= Helper methods
│   ├── CommandLine.h
│   ├── FileParser.h
│   ├── ...
```

## Building ##

The entire codebase has been built and tested on the `remote.eos.ncsu.edu` servers. It requires no additional 
configuration except choosing the build target. Here is a repeatable script to perform the
build and run a target executable:

```bash
# Assuming cwd is the repo root:
#!/bin/bash

## Specify the build target
export BUILD_TARGET=inlab1

## Create the build directory, configure and compile the $BUILD_TARGET
mkdir -p build && cd build && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make -j$(nproc) $BUILD_TARGET && cd ../

## Execute
./build/bin/$BUILD_TARGET
```

### Build Targets ###
Currently supported build targets include:

| #  |   InLab    |   OutLab    |
|:--:|:----------:|:-----------:|
| 1  | ✅ `inlab1` | ✅ `outlab1` |
| 2  | ✅ `inlab2` | ✅ `outlab2` |
| 3  | ✅ `inlab3` | ✅ `outlab3` |
| 4  |            |             |
| 5  |            |             |
| 6  |            |             |
| 7  |            |             |
| 8  |            |             |
| 9  |            |             |
| 10 |            |             |
| 11 |            |             |
| 12 |            |             |
| 13 |            |             |

### Optional CMake Arguments ###

Available options are:

| CMake Option                 | Description                                                       | Default |
|------------------------------|-------------------------------------------------------------------|---------|
| BUILD_TESTS                  | Build a separate test target (for e.g. `inlab1_tests`)            | OFF     |
| PORTABLE                     | Build without flag `-march=native`, trading speed for portability | OFF     |
| BUILD_PERFORMANCE_BENCHMARKS | Build a separate benchmarks target (for e.g. `inlab1_bench`)      | OFF     |

## Docker Usage Guide

This repository contains a Dockerfile that is designed to mimic the OS and packages available on the 'remote.eos.ncsu.edu' servers. It uses CentOS 8 as the base image and installs various development tools and libraries. The Dockerfile also sets up SSH for root access in the ssh-debugger stage.

### Why Docker?

Docker is a platform that allows us to package our application and its dependencies into a "container" which can run on any Linux machine. This helps to eliminate the "it works on my machine" problem. In this case, the Dockerfile was needed to ensure that the code can be built, run, and debugged in an environment that closely matches the 'remote.eos.ncsu.edu' servers.

### Building the Docker Image

To build the Docker image, navigate to the directory containing the Dockerfile and run the following command:

```bash
docker build -t my-app .
```

This command tells Docker to build an image using the Dockerfile in the current directory and tag the image with the name "my-app".

### Running the Docker Container

To run the Docker container, use the following command:

```bash
docker run -it my-app
```

This command tells Docker to run the container interactively (i.e., it attaches a terminal session to the running container) and starts a bash shell.

### Debugging the Code

The Dockerfile sets up SSH for root access in the ssh-debugger stage. This allows you to connect to the running container using SSH and debug the code. To do this, you need to run the container with port 22 exposed:

```bash
docker run -p 22:22 -it my-app
```

Then, you can connect to the running container using SSH:

```bash
ssh root@localhost
```

The password for the root user is "debugger".

### Building and Running Tests

The Dockerfile includes arguments for building tests and performance benchmarks. To build and run the tests, you can modify the `docker build` command as follows:

```bash
docker build --build-arg CMAKE_BUILD_TESTS=ON -t my-app .
```

Then, you can run the tests in the Docker container as you would normally do.

## Development - JetBrains Clion Support
You can use [JetBrains Clion](https://www.jetbrains.com/clion/download/#section=mac) as the debug environment. 
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

### Instructions

1. #### [Create Docker Toolchain](https://www.jetbrains.com/help/clion/clion-toolchains-in-docker.html#create-docker-toolchain)
   1. Go to `Settings / Preferences | Build, Execution, Deployment | Toolchains`.
   2. Click `Add toolchain` and select `Docker`.
   3. Click the `screw nut icon` next to the `Docker` field to select a Docker image.
      1. You can also configure a `Docker server` in `Settings / Preferences | Build, Execution, Deployment | Docker` and then select it in the toolchain settings.
   4. Select the Docker Image `eos:debugger` and wait until the tool detection finishes.
   5. Set the `C Compiler` to `Let CMake Detect`. It should detect `gcc`.
   6. Set the `C++ Compiler` to `Let CMake Detect`. It should detect `g++`.
   7. Set the `Debugger` to `Docker GDB`.
   8. Then save the settings.


2. #### [Build, Run, Debug with a Docker Toolchain](https://www.jetbrains.com/help/clion/clion-toolchains-in-docker.html#build-run-debug-docker)
   1. After configuring a Docker toolchain, you can select it in `CMake profiles` or in `Makefile` settings. Alternatively, move the toolchain to the top of the list to make it default.
   2. The project folder will mount to the Docker container and building, running, and debugging will be
      performed in it. CLion will start the container and shut it down after the command is executed.


### Troubleshooting
* #### [Improve Docker Toolchain performance on Windows](https://www.jetbrains.com/help/clion/clion-toolchains-in-docker.html#windows-performance)
   * To get better performance on Windows, we recommend using Docker with the WSL 2 backend.
      1. Set up `Docker Desktop` with the [WSL 2 backend](https://docs.docker.com/desktop/windows/wsl/).
      2. In the Docker desktop application, navigate to `Settings | Resources | WSL Integration` and enable integration
         with your WSL distribution.
      3. Place the project sources into the WSL filesystem, then open it in CLion and configure a Docker toolchain.
