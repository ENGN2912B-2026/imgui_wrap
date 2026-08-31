
# std::source_location compatibility

check_cxx_source_compiles("
#include <source_location>
int main() { auto loc = std::source_location::current(); return 0; }
" HAS_STD_SOURCE_LOCATION)

if(NOT HAS_STD_SOURCE_LOCATION)
  message(STATUS "Using std_compat library for `std::source_location`")
  target_include_directories(std_compat INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
  )
endif(NOT HAS_STD_SOURCE_LOCATION)
