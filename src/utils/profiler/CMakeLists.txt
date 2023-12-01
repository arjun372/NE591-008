set(LIB_HEADERS
        Profiler.h
        ProfilerHelper.h
        ResourceMonitor.h
        ../Stopwatch.h
)

set(LIB_SOURCES
        stub.cpp
)

if (NOT TARGET utils_profiler)
    # Create target
    add_library(utils_profiler STATIC ${LIB_HEADERS} ${LIB_SOURCES})
    target_include_directories(utils_profiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif ()

# Link target against libraries
#target_link_libraries(profiler utils Boost::program_options)
target_link_libraries(utils_profiler Boost::program_options)
#target_link_libraries(profiler OpenMP::OpenMP_CXX)
