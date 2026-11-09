//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <array>
#include <algorithm> // require for std::min, std::max (on MacOS)

namespace math
{
  template <typename T>
  struct Vec4
  {
    T x, y, z, w;

    using value_type = T;

    template <typename U>
    Vec4<U> cast() const
    {
      return {
        static_cast<U>(x),
        static_cast<U>(y),
        static_cast<U>(z),
        static_cast<U>(w)
      };
    }

    template <typename U>
    std::array<U, 4> to() const
    {
      return {
        static_cast<U>(x),
        static_cast<U>(y),
        static_cast<U>(z),
        static_cast<U>(w)
      };
    }

    Vec4 operator+(const Vec4& other) const
    {
      return {x + other.x, y + other.y, z + other.z, w + other.w};
    }

    Vec4 operator-(const Vec4& other) const
    {
      return {x - other.x, y - other.y, z - other.z, w - other.w};
    }

    Vec4& operator+=(const Vec4& other)
    {
      x += other.x;
      y += other.y;
      z += other.z;
      w += other.w;
      return *this;
    }

    Vec4& operator-=(const Vec4& other)
    {
      x -= other.x;
      y -= other.y;
      z -= other.z;
      w -= other.w;
      return *this;
    }

    Vec4 operator*(T scalar) const
    {
      return {x * scalar, y * scalar, z * scalar, w * scalar};
    }

    Vec4 operator/(T scalar) const
    {
      return {x / scalar, y / scalar, z / scalar, w / scalar};
    }

    Vec4& operator*=(T scalar)
    {
      x *= scalar;
      y *= scalar;
      z *= scalar;
      w *= scalar;
      return *this;
    }

    Vec4& operator/=(T scalar)
    {
      x /= scalar;
      y /= scalar;
      z /= scalar;
      w /= scalar;
      return *this;
    }

    constexpr Vec4 cwiseMin(const Vec4& other) const
    {
      return {
        std::min(x, other.x),
        std::min(y, other.y),
        std::min(z, other.z),
        std::min(w, other.w)
      };
    }

    constexpr Vec4 cwiseMax(const Vec4& other) const
    {
      return {
        std::max(x, other.x),
        std::max(y, other.y),
        std::max(z, other.z),
        std::max(w, other.w)
      };
    }

    bool operator==(const Vec4& other) const
    {
      return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    bool operator!=(const Vec4& other) const
    {
      return !(*this == other);
    }
  };

  template <typename T>
  Vec4<T> operator*(T scalar, const Vec4<T>& vec)
  {
    return vec * scalar;
  }

  template <typename T>
  Vec4<T> operator/(T scalar, const Vec4<T>& vec)
  {
    return {
      scalar / vec.x,
      scalar / vec.y,
      scalar / vec.z,
      scalar / vec.w
    };
  }

  template <typename V, typename U>
  static V make4(const U& x, const U& y, const U& z, const U& w)
  {
    using T = typename V::value_type;
    return {
      static_cast<T>(x),
      static_cast<T>(y),
      static_cast<T>(z),
      static_cast<T>(w)
    };
  }

  template <typename V, typename U>
  static V make4(const U& u)
  {
    using T = typename V::value_type;
    return {static_cast<T>(u.x), static_cast<T>(u.y),
            static_cast<T>(u.z), static_cast<T>(u.w)};
  }

  using Vec4i = Vec4<int>;
  using Vec4f = Vec4<float>;
  using Vec4d = Vec4<double>;

} // namespace math
