set(UTILS_LIB_SOURCES
        CheckBounds.h
        Stopwatch.h
        Helpers.h
        CommandLine.h
        Profiler.h
        Project.h
        FileParser.h
        json.hpp
        math/RootFinder.h
        math/blas/Matrix.h
        math/blas/Vector.h
        math/blas/MyBLAS.h
)

# Create utils
add_library(utils STATIC ${UTILS_LIB_SOURCES})
set_target_properties(utils PROPERTIES LINKER_LANGUAGE CXX)
