//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <array>
#include <algorithm> // require for std::min, std::max (on MacOS)

namespace math
{
  template <typename T>
  struct Vec2
  {
    T x, y;

    using value_type = T;

    template <typename U>
    Vec2<U> cast() const
    {
      return {static_cast<U>(x), static_cast<U>(y)};
    }

    template <typename U>
    std::array<U, 2> to() const
    {
      return {static_cast<U>(x), static_cast<U>(y)};
    }

    Vec2 operator+(const Vec2& other) const
    {
      return {x + other.x, y + other.y};
    }

    Vec2 operator-(const Vec2& other) const
    {
      return {x - other.x, y - other.y};
    }

    Vec2& operator+=(const Vec2& other)
    {
      x += other.x;
      y += other.y;
      return *this;
    }

    Vec2& operator-=(const Vec2& other)
    {
      x -= other.x;
      y -= other.y;
      return *this;
    }

    Vec2 operator*(T scalar) const
    {
      return {x * scalar, y * scalar};
    }

    Vec2 operator/(T scalar) const
    {
      return {x / scalar, y / scalar};
    }

    Vec2& operator*=(T scalar)
    {
      x *= scalar;
      y *= scalar;
      return *this;
    }

    Vec2& operator/=(T scalar)
    {
      x /= scalar;
      y /= scalar;
      return *this;
    }

    constexpr Vec2 cwiseMin(const Vec2& other) const
    {
      return {std::min(x, other.x), std::min(y, other.y)};
    }

    constexpr Vec2 cwiseMax(const Vec2& other) const
    {
      return {std::max(x, other.x), std::max(y, other.y)};
    }

    bool operator==(const Vec2& other) const
    {
      return x == other.x && y == other.y;
    }

    bool operator!=(const Vec2& other) const
    {
      return !(*this == other);
    }
  };

  template <typename T>
  Vec2<T> operator*(T scalar, const Vec2<T>& vec)
  {
    return vec * scalar;
  }

  template <typename T>
  Vec2<T> operator/(T scalar, const Vec2<T>& vec)
  {
    return {scalar / vec.x, scalar / vec.y};
  }

  template <typename V, typename U>
  static V make(const U& x, const U& y)
  {
    using T = typename V::value_type;
    return V{static_cast<T>(x), static_cast<T>(y)};
  }

  template <typename V, typename U>
  static V make(const U& u)
  {
    using T = typename V::value_type;
    return V{static_cast<T>(u.x), static_cast<T>(u.y)};
  }

  using Vec2i = Vec2<int>;
  using Vec2f = Vec2<float>;
  using Vec2d = Vec2<double>;

} // namespace math
