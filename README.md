# NE591 Lab MonoRepo #

This is a monorepo for my in- and out- lab deliverables for the `NE591-008` course. I have chosen to organize it 
this way to build on the cumulative code changes from each lab, and to reuse as many helper functions as possible. 
A secondary objective is to benchmark my implementations against the `Boost` library functions. 

## Locating Source Files ##
Sources and file paths for each lab follow the following structure:
```
├── README.md   <= You are now here
├── src
│   ├── labs
│   │   ├── inlab1
│   │   │   ├── inlab1.cpp       <= Entrypoint main() and run() methods
│   │   │   ├── Compute.h        <= Core compute logic
│   │   │   ├── ProcessInputs.h  <= Input processing and error checking 
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
| 3  |            |             |
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

| CMake Option                 | Description                                                    | Default |
|------------------------------|----------------------------------------------------------------|---------|
| BUILD_TESTS                  | Build a separate test target (for e.g. `inlab1_tests`)         | OFF     |
| OPTIMIZE_FOR_NATIVE          | Build with flag `-march=native`, trading portability for speed | ON      |
| BUILD_PERFORMANCE_BENCHMARKS | Build a separate benchmarks target (for e.g. `inlab1_bench`)   | OFF     |

### Troubleshooting Builds ###
The optionally provided [Dockerfile](Dockerfile) provides a sanitized environment with the minimum dependencies
and an automated build for this repo. The environment mimics `remote.eos.ncsu.edu` by using `gcc v8.5.0` and 
`boost 1.66.0`.
