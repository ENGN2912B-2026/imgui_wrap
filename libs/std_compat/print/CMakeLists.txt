
# std::print compatibility

check_cxx_source_compiles("
#include <version>
#ifdef __cpp_lib_print
# include <print>
#endif
int main() { std::print(\"Hello, World!\"); std::println(\"Hello, World!\"); return 0; }
" HAS_STD_PRINT)

check_cxx_source_compiles("
#include <version>
#ifdef __cpp_lib_format
# include <format>
#endif
int main() { std::format(\"Hello, {}!\", \"World\"); return 0; }
" HAS_STD_FORMAT)

if(NOT HAS_STD_PRINT)
  message(STATUS "Using std_compat library for `std::print` and `std::println`")
  target_include_directories(std_compat INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
  )
endif(NOT HAS_STD_PRINT)
