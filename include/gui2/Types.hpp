//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <math/Vec2.hpp>

namespace gui2
{
  using math::Vec2i;

  // Orientation of a layout or box.
  enum class Orientation
  {
    Vertical,
    Horizontal,
  };

  // Validates that the given orientation is a valid value of the Orientation
  // enum class. Throws an exception if the orientation is invalid.
  void validate(Orientation orientation);

  // Invokes a callable if two values are equal at compile time.
  template<auto V1, auto V2>
  constexpr void IfEqual(std::invocable auto&& f)
  {
    if constexpr (V1 == V2) { std::invoke(std::forward<decltype(f)>(f)); }
  }

  // Invokes a callable if the given orientation is Vertical.
  template <Orientation orientation>
  constexpr void IfVertical(std::invocable auto&& f)
  {
    IfEqual<orientation, Orientation::Vertical>(std::forward<decltype(f)>(f));
  }

  // Invokes a callable if the given orientation is Horizontal.
  template <Orientation orientation>
  constexpr void IfHorizontal(std::invocable auto&& f)
  {
    IfEqual<orientation, Orientation::Horizontal>(std::forward<decltype(f)>(f));
  }

} // namespace gui
