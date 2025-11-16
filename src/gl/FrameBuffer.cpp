//  Copyright (c) 2024-2025 Daniel Moreno. All rights reserved.
//

#include <gl/gl.h>
#include <gl/FrameBuffer.hpp>

#include <stdexcept>

namespace gl
{
  FrameBuffer::FrameBuffer(const Vec2i& size, GLint inperpolationMode)
    : size_{ size }
    , inperpolationMode_{ inperpolationMode }
  {
    // Create a frame buffer object (fbo)
    glGenFramebuffers(1, &fbo_);

    // Create a color attachment texture
    glGenTextures(1, &texture_);

    // Create a render buffer object for depth and stencil attachment (we won't be sampling these)
    glGenRenderbuffers(1, &rbo_);

    if (size_.x > 0 && size_.y > 0)
    { // Generate the frame buffer for the given size
      generateFrameBuffer_();
    }
  }

  FrameBuffer::~FrameBuffer()
  {
    glDeleteFramebuffers(1, &fbo_);
    glDeleteTextures(1, &texture_);
    glDeleteRenderbuffers(1, &rbo_);
  }

  void FrameBuffer::setSize(const Vec2i& size)
  {
    if (size_ != size)
    { // New size, regenerate the texture and render buffer object
      size_ = size;
      generateFrameBuffer_();
    }
  }

  void FrameBuffer::setInterpolationMode(GLint mode)
  {
    if (inperpolationMode_ != mode)
    {
      inperpolationMode_ = mode;

      if (texture_ > 0)
      { // Update the texture parameters
        glBindTexture(GL_TEXTURE_2D, texture_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
        glBindTexture(GL_TEXTURE_2D, 0);
      }
    }
  }

  void FrameBuffer::bind() const
  {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  }

  void FrameBuffer::unbind() const
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void FrameBuffer::generateFrameBuffer_()
  {
    // Bind the frame buffer object (fbo)
    bind();

    // Create a color attachment texture
    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size_.x, size_.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, inperpolationMode_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, inperpolationMode_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_, 0);

    // Create a render buffer object for depth and stencil attachment (we won't be sampling these)
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size_.x, size_.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);

    const bool bufferComplete{
      glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE };

    // Unbind everything
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    unbind();

    // Check if the frame buffer is complete
    if (!bufferComplete)
    {
      throw std::runtime_error("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    }
  }

} // namespace gl
