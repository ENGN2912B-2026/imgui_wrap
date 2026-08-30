//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <version>

#if defined(__has_include)
  #if __has_include(<format>)
    // We need to include <format> in order for `__cpp_lib_format` to be defined.
    #include <format>
  #endif
  #if __has_include(<print>)
    // We need to include <print> in order for `__cpp_lib_print` to be defined.
    #include <print>
  #endif
#endif

#if defined(__cpp_lib_print) && __cpp_lib_print >= 202207L
  // Use C++23 std::print and std::println
  #pragma message("Using C++23 std::print and std::println")
#elif defined(__cpp_lib_format) && __cpp_lib_format >= 201907L
  // Use C++20 std::format and std::print
  #pragma message("Using C++20 std::format and custom print/println")
  #include <iostream>
  namespace std
  {
    template<class... Args>
    void print(std::format_string<Args...> fmt, Args&&... args)
    {
      std::cout << std::format(fmt, std::forward<Args>(args)...);
    }

    template<class... Args>
    void println(std::format_string<Args...> fmt, Args&&... args)
    {
      std::cout << std::format(fmt, std::forward<Args>(args)...)
                << '\n';
    }
  }
#else
  // Error
  #error "std::print and std::println are not available."

  /*

  // Use the fmt library instead.
  #include <fmt/core.h>
  namespace std
  {
    template<class... Args>
    void print(fmt::format_string<Args...> fmt, Args&&... args)
    {
        fmt::print(fmt, std::forward<Args>(args)...);
    }

    template<class... Args>
    void println(fmt::format_string<Args...> fmt, Args&&... args)
    {
        fmt::println(fmt, std::forward<Args>(args)...);
    }
  }

  */

#endif