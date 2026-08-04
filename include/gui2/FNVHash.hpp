//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <cstdint>
#include <string_view>
#include <array>
#include <utility>

#if defined(__cpp_lib_source_location) && __cpp_lib_source_location >= 201907L
# include <source_location>
# define HAS_SOURCE_LOCATION
#endif

namespace gui2
{
  // FNV-1a hash implementation for generating unique identifiers.
  // See: en.wikipedia.org/wiki/FNV-1a
  class FNV1a
  {
  public:
    constexpr FNV1a() : value_{FNV_OffsetBasis} {}

    constexpr explicit FNV1a(uint64_t value) : value_{FNV_OffsetBasis}
    {
      combine(value);
    }

    constexpr explicit FNV1a(std::string_view s) : value_{FNV_OffsetBasis}
    {
      combine(s);
    }

#if defined(HAS_SOURCE_LOCATION)
    constexpr explicit FNV1a(std::source_location loc) : value_{FNV_OffsetBasis}
    {
      combine(loc);
    }
#endif

    constexpr void combine(uint64_t value)
    {
      for (int i = 0; i < 8; ++i)
      {
        value_ ^= static_cast<uint8_t>(value);
        value_ *= FNV_Prime;
        value >>= 8;
      }
    }

    constexpr void combine(std::string_view s)
    {
      for (unsigned char c : s)
      {
        value_ ^= c;
        value_ *= FNV_Prime;
      }
    }

#if defined(HAS_SOURCE_LOCATION)
    constexpr void combine(std::source_location loc)
    {
      combine(loc.file_name());
      combine(loc.line());
      combine(loc.column());
    }
#endif

    constexpr uint64_t getValue() const
    {
      return value_;
    }

    constexpr std::array<char, 16> toHex() const
    {
      return makeHex(value_);
    }

  private:
    static constexpr uint64_t FNV_OffsetBasis = 14695981039346656037ull;
    static constexpr uint64_t FNV_Prime = 1099511628211ull;
    uint64_t value_;

    static constexpr std::array<char, 16> makeHex(uint64_t value)
    {
      constexpr char digits[] = "0123456789abcdef";
      std::array<char, 16> out{};
      for (int i = 15; i >= 0; --i)
      {
        out[i] = digits[value & 0xF];
        value >>= 4;
      }
      return out;
    }
  };
}

#if defined(HAS_SOURCE_LOCATION)
# undef HAS_SOURCE_LOCATION
#endif
