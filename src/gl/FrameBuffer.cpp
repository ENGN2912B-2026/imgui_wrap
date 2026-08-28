//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#include <gl/FrameBuffer.hpp>

#include <stdexcept>
#include <cassert>

namespace gl
{
  FrameBuffer::FrameBuffer(const Vec2i& size, GLint inperpolationMode)
    : FrameBuffer{}
  {
    initialize(size, inperpolationMode);
  }

  FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
  {
    operator=(std::move(other));
  }

  FrameBuffer::~FrameBuffer()
  {
    if (rbo_ > 0) { glDeleteRenderbuffers(1, &rbo_); }
    texture_.uninitialize();
    if (fbo_ > 0) { glDeleteFramebuffers(1, &fbo_); }
  }

  FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept
  {
    if (this != &other)
    {
      fbo_ = other.fbo_;
      texture_ = std::move(other.texture_);
      rbo_ = other.rbo_;
      size_ = other.size_;
      interpolationMode_ = other.interpolationMode_;

      // Reset the other frame buffer to a default state
      FrameBuffer empty;
      other.fbo_ = empty.fbo_;
      other.texture_ = std::move(empty.texture_);
      other.rbo_ = empty.rbo_;
      other.size_ = empty.size_;
      other.interpolationMode_ = empty.interpolationMode_;
    }

    return *this;
  }

  bool FrameBuffer::isInitialized() const
  {
    return fbo_ > 0 && texture_.isInitialized() && rbo_ > 0;
  }

  void FrameBuffer::initialize(const Vec2i& size, GLint interpolationMode)
  {
    if (fbo_ == 0 )
    { // Create a frame buffer object (fbo)
      glGenFramebuffers(1, &fbo_);
      if (fbo_ == 0)
      {
        throw std::runtime_error{
          "ERROR::FRAMEBUFFER:: Failed to create frame buffer object!"};
      }
    }

    if (!texture_.isInitialized())
    { // Create a color attachment texture
      texture_.initialize();
    }

    if (rbo_ == 0)
    { // Create a render buffer object for depth and stencil attachment (we
      // won't be sampling these)
      glGenRenderbuffers(1, &rbo_);
      if (rbo_ == 0)
      {
        throw std::runtime_error{
          "ERROR::FRAMEBUFFER:: Failed to create render buffer object!"};
      }
    }

    assert(isInitialized()
      && "Frame buffer, texture, and render buffer objects must be valid");
    assert(fbo_ > 0 && texture_.isInitialized() && rbo_ > 0
      && "Frame buffer, texture, and render buffer objects must be valid");

    setSize(size);
    setInterpolationMode(interpolationMode);
  }

  void FrameBuffer::setSize(const Vec2i& size)
  {
    if (isInitialized())
    {
      if (size_ != size)
      { // New size, regenerate the texture and render buffer object
        size_ = size;
        generateFrameBuffer_();
      }
    }
    else
    { // Not initialized, call `initialize()` instead, which it also sets
      // the size and interpolation mode.
      initialize(size, interpolationMode_);
    }
  }

  void FrameBuffer::setInterpolationMode(GLint mode)
  {
    if (isInitialized())
    {
      if (interpolationMode_ != mode)
      {
        interpolationMode_ = mode;
        if (texture_.isInitialized())
        { // Update the texture parameters
          texture_.setInterpolationMode(interpolationMode_);
        }
      }
    }
    else
    { // Not initialized, call `initialize()` instead, which it also sets
      // the size and interpolation mode.
      initialize(size_, mode);
    }
  }

  void FrameBuffer::bind() const
  {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  }

  FrameBuffer::AutoUnbind FrameBuffer::bindScoped() const
  {
    return AutoUnbind{ *this };
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
    texture_.initialize(size_, interpolationMode_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           texture_.getId(), 0);

    // Create a render buffer object for depth and stencil attachment (we won't be sampling these)
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size_.x, size_.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);

    const bool bufferComplete{
      glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE };

    // Unbind everything
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    unbind();

    // Check if the frame buffer is complete
    if (!bufferComplete)
    {
      throw std::runtime_error{
        "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"};
    }
  }

} // namespace gl
