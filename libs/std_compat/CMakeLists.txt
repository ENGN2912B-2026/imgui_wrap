
# std_compat library for compatibility with older C++ compilers

add_library(std_compat INTERFACE)

include(CheckCXXSourceCompiles)

add_subdirectory(source_location)
add_subdirectory(constexpr_string)
add_subdirectory(format)
add_subdirectory(print)
