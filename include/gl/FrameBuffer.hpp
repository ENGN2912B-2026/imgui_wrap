//  Copyright (c) 2024-2025 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gl/gl.h>
#include <math/Vec2.hpp>

namespace gl
{
  using math::Vec2i;

  class FrameBuffer
  {
    Vec2i size_;
    GLuint fbo_;
    GLuint texture_;
    GLuint rbo_;
    GLint inperpolationMode_;
  public:
    FrameBuffer(
      const Vec2i& size = {0, 0},
      GLint inperpolationMode = GL_LINEAR);

    ~FrameBuffer();

    // Set or get the frame buffer size
    void setSize(const Vec2i& size);
    const Vec2i& getSize() const { return size_; }

    // Set or get the texture interpolation mode
    void setInterpolationMode(GLint mode);
    const GLint getInterpolationMode() const { return inperpolationMode_; }

    void bind() const;
    void unbind() const;

    unsigned getTexture() const { return texture_; }

  private:
    void generateFrameBuffer_();
  };

} // namespace gl
