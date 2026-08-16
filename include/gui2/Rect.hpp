//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gui2/Types.hpp>

namespace gui2
{
  // A rectangle defined by an origin point and a size.
  struct Rect
  {
    // A canonical constant representing that a non-negative value is not
    // specified. This is used to indicate that a dimension (width or height)
    // of the rectangle is unspecified. Any other negative value is considered
    // invalid in the same context.
    constexpr static int kNone{ -1 };

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

    // Returns a new rectangle with no specified size.
    // An unspecified dimension is represented by a negative value.
    static constexpr Rect empty()
    {
      return {{0, 0}, {kNone, kNone}};
    }

    // Checks if the rectangle has a specified width.
    constexpr bool hasWidth() const
    {
      return size.x >= 0;
    }

    // Checks if the rectangle has a specified height.
    constexpr bool hasHeight() const
    {
      return size.y >= 0;
    }

    // Checks if both dimensions are specified.
    constexpr bool hasSize() const
    {
      return hasWidth() && hasHeight();
    }

    // Checks if at least one dimension is unspecified.
    constexpr bool isEmpty() const
    {
      return !hasSize();
    }

    // Unsets the width of the rectangle, making it unspecified.
    Rect& unsetWidth()
    {
      size.x = kNone;
      return *this;
    }

    // Unsets the height of the rectangle, making it unspecified.
    Rect& unsetHeight()
    {
      size.y = kNone;
      return *this;
    }

    // Unsets both the width and height of the rectangle, making them
    // unspecified.
    Rect& unsetSize()
    {
      return unsetWidth().unsetHeight();
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
