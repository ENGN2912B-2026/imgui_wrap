//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <string>
#include <source_location>
#include <stdexcept>

#include <gui2/FNVHash.hpp>

// Define a helper macro to use in place of `constexpr` for `std::string` if the
// compiler does not support it. This allows
#ifdef HAS_STD_CONSTEXPR_STRING
# define CONSTEXPR_STR constexpr
#else
# define CONSTEXPR_STR
#endif

namespace gui2
{
  // Concept for classes that provided a unique identifier value
  template<typename T>
  concept Identifier =
    requires(T const& id)
    {
      { id.getValue() } -> std::convertible_to<uint64_t>;
    };

  template<typename T>
  concept IdentifierWithName =
    Identifier<T> &&
    requires(T const& id)
    {
        { id.getName() } -> std::convertible_to<std::string>;
    };

  // Manual identifier from a string value or a numeric value
  class StringId
  {
    FNV1a hash_;
    std::string name_;
  public:
    CONSTEXPR_STR StringId(std::string name)
      : hash_{name}, name_{std::move(name)}
    {
      if (name.empty())
      {
        throw std::invalid_argument{"Id name cannot be empty"};
      }
    }

    constexpr StringId(uint64_t value)
      : hash_{value}, name_{std::to_string(value)}
    {
    }

    constexpr uint64_t getValue() const { return hash_.getValue(); }
    constexpr std::array<char, 16> getValueHex() const { return hash_.toHex(); }
    const std::string getValueStr() const
    {
      const auto hex = getValueHex();
      return std::string{hex.data(), hex.size()};
    }

    const std::string& getName() const { return name_; }
  };

  // Automatic identifier based on the source location
  class LocationId
  {
    FNV1a hash_;
    std::source_location location_;
  public:
    constexpr LocationId(std::source_location location = std::source_location::current())
      : hash_{location}, location_{std::move(location)}
    {
    }

    constexpr uint64_t getValue() const { return hash_.getValue(); }
    constexpr std::array<char, 16> getValueHex() const { return hash_.toHex(); }
    const std::string getValueStr() const
    {
      const auto hex = getValueHex();
      return std::string{hex.data(), hex.size()};
    }

    const std::source_location& getLocation() const { return location_; }
  };

  // Type aliases for convenience
  using Id = StringId;
  using AutoId = LocationId;

} // namespace gui

#ifdef CONSTEXPR_STR
# undef CONSTEXPR_STR
#endif