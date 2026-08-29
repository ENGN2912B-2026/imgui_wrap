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
    uninitialize();
  }

  FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept
  {
    if (this != &other)
    {
      fbo_ = std::move(other.fbo_);
      texture_ = std::move(other.texture_);
      renderBuffer_ = std::move(other.renderBuffer_);

      // Reset the other frame buffer to a default state
      FrameBuffer empty;
      other.fbo_ = std::move(empty.fbo_);
      other.texture_ = std::move(empty.texture_);
      other.renderBuffer_ = std::move(empty.renderBuffer_);
    }

    return *this;
  }

  bool FrameBuffer::isInitialized() const
  {
    return fbo_ > 0 && texture_.isInitialized() && renderBuffer_.isInitialized();
  }

  void FrameBuffer::initialize()
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

    // Initialize the texture
    texture_.initialize();

    // Initialize the render buffer object
    renderBuffer_.initialize();

    assert(isInitialized()
      && "Frame buffer, texture, and render buffer objects must be valid");
    assert(fbo_ > 0 && texture_.isInitialized() && renderBuffer_.isInitialized()
      && "Frame buffer, texture, and render buffer objects must be valid");

    // Create the attachments for the frame buffer --------------------------

    // Bind the frame buffer for the duration of this scope
    const AutoUnbind autoUnbind{ *this };

    // Create a color attachment texture
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, texture_.getId(), 0);

    // Create a render buffer object for depth and stencil attachment (we
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, renderBuffer_.getId());
  }

  void FrameBuffer::initialize(const Vec2i& size, GLint interpolationMode)
  {
    // Ensure the frame buffer is initialized
    initialize();

    // Set the size
    setSize(size);

    // Set the interpolation mode for the texture
    texture_.setInterpolationMode(interpolationMode);

    // Ensure the frame buffer is complete
    ensureCompleteIfNonEmpty_(size);
  }

  void FrameBuffer::uninitialize()
  {
    renderBuffer_.uninitialize();
    texture_.uninitialize();
    if (fbo_ > 0)
    {
      glDeleteFramebuffers(1, &fbo_);
      fbo_ = 0U;
    }
  }

  bool FrameBuffer::isComplete() const
  {
    if (isInitialized())
    { // Check if the frame buffer is complete
      const AutoUnbind autoUnbind{ *this };
      const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      return status == GL_FRAMEBUFFER_COMPLETE;
    }
    return false;
  }

  void FrameBuffer::setSize(const Vec2i& size)
  {
    if (!isInitialized())
    {
      throw std::runtime_error{ "ERROR::FRAMEBUFFER:: "
        "Cannot set size of uninitialized frame buffer!"};
    }

    // Update the size of the texture and render buffer
    texture_.setSize(size);
    renderBuffer_.setSize(size);

    // Ensure the frame buffer is complete
    ensureCompleteIfNonEmpty_(size);
  }

  Vec2i FrameBuffer::getSize() const
  {
    if (!isInitialized())
    {
      throw std::runtime_error{ "ERROR::FRAMEBUFFER:: "
        "Cannot get size of uninitialized frame buffer!"};
    }
    return texture_.getSize();
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

  void FrameBuffer::ensureCompleteIfNonEmpty_(const Vec2i& size)
  {
    if (size.x > 0 && size.y > 0 && !isComplete())
    {
      throw std::runtime_error{
        "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"};
    }
  }

} // namespace gl
