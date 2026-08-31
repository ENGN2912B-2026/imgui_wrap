
# std::format compatibility

check_cxx_source_compiles("
#include <format>
int main() { std::format(\"Hello, {}!\", \"World\"); return 0; }
" HAS_STD_FORMAT)

if(NOT HAS_STD_FORMAT)
  message(STATUS "Using std_compat library for `std::format` (fmtlib)")

  include(FetchContent)
  FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG        10.2.0
  )
  FetchContent_MakeAvailable(fmt)

  target_link_libraries(std_compat INTERFACE fmt::fmt)
  target_include_directories(std_compat INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
  )
endif(NOT HAS_STD_FORMAT)
