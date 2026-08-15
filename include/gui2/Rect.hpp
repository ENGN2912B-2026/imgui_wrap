//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gui2/Types.hpp>

namespace gui2
{
  // A rectangle defined by an origin point and a size.
  struct Rect
  {
    // The origin point of the rectangle, representing the top-left corner.
    Vec2i origin{ 0, 0 };
    // The size of the rectangle, representing its width and height.
    Vec2i size{ 0, 0 };

    // Returns a new rectangle defined by the top-left and bottom-right corners.
    static constexpr Rect fromCorners(const Vec2i& topLeft,
                                      const Vec2i& bottomRight)
    {
      return { topLeft, bottomRight - topLeft };
    }

    // Returns a new rectangle that is empty.
    // An empty rectangle has a size with negative dimensions.
    static constexpr Rect empty()
    {
      return { {0, 0}, {-1, -1} };
    }

    // Checks if the rectangle is empty (size has negative dimensions).
    constexpr bool isEmpty() const
    {
      return size.x < 0 || size.y < 0;
    }

    // Returns the bottom-right corner of the rectangle if it is not empty,
    // otherwise returns the origin.
    constexpr Vec2i end() const
    {
      return isEmpty() ? origin : origin + size;
    }

    // Returns a new rectangle that is the union of this rectangle and another
    // rectangle.
    Rect united(const Rect& other) const
    {
      if (this->isEmpty()) { return other; }
      if (other.isEmpty()) { return *this; }
      const Vec2i newOrigin{ origin.cwiseMin(other.origin) };
      const Vec2i newEnd{ end().cwiseMax(other.end()) };
      return { newOrigin, newEnd - newOrigin };
    }

    // Updates this rectangle to be the union of this rectangle and another
    // rectangle, and returns a reference to this rectangle.
    Rect& unite(const Rect& other)
    {
      *this = united(other);
      return *this;
    }
  };

} // namespace gui
