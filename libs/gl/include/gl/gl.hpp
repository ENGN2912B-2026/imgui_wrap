//  Copyright (c) 2024 Daniel Moreno. All rights reserved.
//

#pragma once

#include <math/Vec2.hpp>

#ifdef USE_GLAD
# include <glad/gl.h>
#else
# include <GL/gl.h>
#endif

namespace gl
{
  using math::Vec2i;
  using math::Vec2f;

} // namespace gl

//! \brief A macro that deletes the copy constructor and copy assignment
//!        operator and provides default move constructor and move assignment
//!        operator for a class.
#define GL_NO_COPY_DEFAULT_MOVE(Type)             \
  Type(const Type&) = delete;                     \
  Type(Type&&) noexcept = default;                \
  Type& operator=(const Type&) = delete;          \
  Type& operator=(Type&&) noexcept = default;
