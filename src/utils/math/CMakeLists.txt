add_subdirectory(blas)
add_subdirectory(relaxation/tests)

set(LIB_HEADERS
        Math.h
        Stats.h
        Random.h
        RootFinder.h

        factorization/LU.h
        factorization/LUP.h
        factorization/Factorize.h

        relaxation/SORPJ.h
        relaxation/SOR.h
        relaxation/SSOR.h
        relaxation/RelaxationMethods.h
)

set(LIB_SOURCES
        stub.cpp
)

if (NOT TARGET utils_math)
    # Create target
    add_library(utils_math STATIC ${LIB_HEADERS} ${LIB_SOURCES})
    target_include_directories(utils_math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif ()

# Link target against libraries
#target_link_libraries(profiler utils Boost::program_options)
target_link_libraries(utils_math utils_math_blas Boost::program_options OpenMP::OpenMP_CXX)
#target_link_libraries(profiler OpenMP::OpenMP_CXX)
