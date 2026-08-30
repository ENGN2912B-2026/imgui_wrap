//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gui2/Types.hpp>

#include <limits>
#include <cassert>

namespace gui2
{
  // A rectangle defined by an origin point and a size.
  struct Rect
  {
    // A canonical constant representing that a non-negative value is not
    // specified. This is used to indicate that a dimension (width or height)
    // of the rectangle is unspecified.
    constexpr static int kNone{ std::numeric_limits<int>::min() };

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

    //! \brief Returns the width of the rectangle.
    //! \return The width of the rectangle if it is specified, otherwise returns
    //!         a negative value indicating that the width is unspecified.
    constexpr int getWidth() const { return size.x; }

    //! \brief Returns the height of the rectangle.
    //! \return The height of the rectangle if it is specified, otherwise returns
    //!         a negative value indicating that the height is unspecified.
    constexpr int getHeight() const { return size.y; }

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

    //! \brief Sets the specified value as an optional width.
    //! \param[in] width  The width value to set as optional.
    //! \return A reference to the current rectangle with the updated width.
    Rect& setOptionalWidth(int width)
    {
      assert(width >= 0 && "Width must be non-negative");
      size.x = -width;
      return *this;
    }

    //! \brief Sets the specified value as an optional height.
    //! \param[in] height  The height value to set as optional.
    //! \return A reference to the current rectangle with the updated height.
    Rect& setOptionalHeight(int height)
    {
      assert(height >= 0 && "Height must be non-negative");
      size.y = -height;
      return *this;
    }

    //! \brief Sets both width and height as optional values.
    //! \param[in] width   The width value to set as optional.
    //! \param[in] height  The height value to set as optional.
    //! \return A reference to the current rectangle with both dimensions set
    Rect& setOptionalSize(int width, int height)
    {
      setOptionalWidth(width);
      setOptionalHeight(height);
      return *this;
    }

    //! \brief Unsets the width of the rectangle, making it optional.
    //! \return A reference to the current rectangle with the width unset.
    Rect& unsetWidth()
    {
      size.x = kNone;
      return *this;
    }

    //! \brief Unsets the height of the rectangle, making it optional.
    //! \return A reference to the current rectangle with the height unset.
    Rect& unsetHeight()
    {
      size.y = kNone;
      return *this;
    }

    //! \brief Checks if the rectangle has an optional width.
    //! \return true if the rectangle has an optional width, false otherwise.
    bool hasOptionalWidth() const
    {
      return !hasWidth() && size.x != kNone;
    }

    //! \brief Checks if the rectangle has an optional height.
    //! \return true if the rectangle has an optional height, false otherwise.
    bool hasOptionalHeight() const
    {
      return !hasHeight() && size.y != kNone;
    }

    //! \brief Returns the available width of the rectangle.
    //! \return The available width of the rectangle.
    //!
    //! If the rectangle has width, it is returned. If it does not, but i has
    //! an optional width, the optional width is returned. Otherwise, 0 is
    //! returned.
    int getAvailableWidth() const
    {
      return hasWidth() ? size.x : hasOptionalWidth() ? -size.x : 0;
    }

    //! \brief Returns the available height of the rectangle.
    //! \return The available height of the rectangle.
    //!
    //! If the rectangle has height, it is returned. If it does not, but i has
    //! an optional height, the optional height is returned. Otherwise, 0 is
    //! returned.
    int getAvailableHeight() const
    {
      return hasHeight() ? size.y : hasOptionalHeight() ? -size.y : 0;
    }

    //! \brief Returns the available size of the rectangle as a Vec2i.
    //! \return The available size of the rectangle as a Vec2i.
    //!
    //! It returns the available width and height. For further details,
    //! see `getAvailableWidth()` and `getAvailableHeight()`.
    Vec2i getAvailableSize() const
    {
      return { getAvailableWidth(), getAvailableHeight() };
    }

    // Returns the x-coordinate of the right edge of the rectangle if it has
    // a specified width, otherwise returns the x-coordinate of the origin.
    constexpr int endX() const
    {
      return hasWidth() ? origin.x + size.x : origin.x;
    }

    // Returns the y-coordinate of the bottom edge of the rectangle if it has
    // a specified height, otherwise returns the y-coordinate of the origin.
    constexpr int endY() const
    {
      return hasHeight() ? origin.y + size.y : origin.y;
    }

    // Returns a new point representing the bottom-right corner of the rectangle
    // if both dimensions are specified. For unspecified dimensions, it returns
    // the value of the origin for that dimension.
    constexpr Vec2i end() const
    {
      return { endX(), endY() };
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
