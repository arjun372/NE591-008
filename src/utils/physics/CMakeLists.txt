add_subdirectory(diffusion/tests)

set(LIB_HEADERS
        diffusion/DiffusionParams.h
        diffusion/DiffusionMatrix.h
        diffusion/DiffusionConstants.h
)

set(LIB_SOURCES
        stub.cpp
)

if (NOT TARGET utils_physics)
    # Create target
    add_library(utils_physics STATIC ${LIB_HEADERS} ${LIB_SOURCES})
    target_include_directories(utils_physics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif ()

# Link target against libraries
#target_link_libraries(physics utils Boost::program_options)
#target_link_libraries(utils_physics Boost::program_options)
#target_link_libraries(physics OpenMP::OpenMP_CXX)
