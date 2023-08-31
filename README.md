# NE591 Lab MonoRepo #

This is a monorepo for the in- and out- lab deliverables for the `NE591-008` course. I have chosen to organize it 
this way to build on the cumulative code changes from each lab, and to reuse as many helper functions as possible. 
A secondary objective is to benchmark my implementations against the `Boost` library functions. 


## Locating Source Files ##
Sources and file paths for each lab follow the following structure:
```
├── src
│   ├── labs
│   │   ├── inlab1
│   │   │   ├── inlab1.cpp <= Entrypoint for inlab1
│   │   │   ├── Compute.h  <= Core compute logic for inlab1
│   │   │   ├── README.md  <= Usage and examples for inlab1
│   │   │   ├── ...
│   │   ├── outlab1
│   │   │   ├── outlab1.cpp <= Entrypoint for outlab1
│   │   │   ├── Compute.h  <= Core compute logic for outlab1
│   │   │   ├── ...
│   ...
```
## Building ##

The entire codebase has been built and tested on the `remote.eos.ncsu.edu` servers. It requires no additional configuration
except choosing the build targets (all by default). Here is a two part script to perform the build and run a 
target executable:

```bash
$ # Assuming cwd is the repo root:
$ mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && make -j && cd bin
$ ./inlab1
```
### Optional CMake Arguments ###

Available options are:

| CMake Option                 | Description                                                    | Default |
|------------------------------|----------------------------------------------------------------|---------|
| BUILD_TESTS                  | Build a separate test target (for e.g. `inlab1_tests`)         | ON      |
| OPTIMIZE_FOR_NATIVE          | Build with flag `-march=native`, trading portability for speed | ON      |
| BUILD_PERFORMANCE_BENCHMARKS | Build a separate benchmarks target (for e.g. `inlab1_bench`)   | ON      |

### Troubleshooting Builds ###
The optionally provided [Dockerfile](Dockerfile) provides a sanitized environment with the minimum dependencies
and an automated build for this repo.
