//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <math/Vec2.hpp>

namespace gui2
{
  using math::Vec2i;

  // A rectangle defined by an origin point and a size.
  struct Rect
  {
    Vec2i origin{ 0, 0 };
    Vec2i size{ 0, 0 };
  };

  // Orientation of a layout or box.
  enum class Orientation
  {
    Vertical,
    Horizontal,
  };

} // namespace gui
