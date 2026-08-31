
# std::print compatibility

check_cxx_source_compiles("
#include <print>
int main() { std::print(\"Hello, World!\"); std::println(\"Hello, World!\"); return 0; }
" HAS_STD_PRINT)

if(NOT HAS_STD_PRINT)
  message(STATUS "Using std_compat library for `std::print` and `std::println`")
  target_include_directories(std_compat INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
  )
endif(NOT HAS_STD_PRINT)
