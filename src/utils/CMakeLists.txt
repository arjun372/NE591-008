set(UTILS_LIB_SOURCES BoundsCheck.h CommandLine.h)

# Create utils
add_library(utils STATIC ${UTILS_LIB_SOURCES})
set_target_properties(utils PROPERTIES LINKER_LANGUAGE CXX)
