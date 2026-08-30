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
    : Handle{ std::move(other) }
  {
    operator=(std::move(other));
  }

  FrameBuffer::~FrameBuffer()
  {
    uninitialize();
  }

  FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept
  {
    // Move the base Handle part of the FrameBuffer
    Handle::operator=(std::move(other));

    if (this != &other)
    {
      texture_ = std::move(other.texture_);
      renderBuffer_ = std::move(other.renderBuffer_);

      // Reset the other frame buffer to a default state
      FrameBuffer empty;
      other.texture_ = std::move(empty.texture_);
      other.renderBuffer_ = std::move(empty.renderBuffer_);
    }

    return *this;
  }

  bool FrameBuffer::isInitialized() const
  {
    return Handle::isInitialized()
      && texture_.isInitialized() && renderBuffer_.isInitialized();
  }

  void FrameBuffer::initialize()
  {
    if (!isInitialized())
    { // Create a frame buffer object
      glGenFramebuffers(1, &getIdRef());

      // Initialize the texture
      texture_.initialize();

      // Initialize the render buffer object
      renderBuffer_.initialize();

      if (!isInitialized())
      {
        throw std::runtime_error{
          "ERROR::FRAMEBUFFER:: Failed to create frame buffer object!"};
      }
    }

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
    if (isInitialized())
    { // Delete the frame buffer object
      glDeleteFramebuffers(1, &getIdRef());
      getIdRef() = 0U;
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

  FrameBuffer::AutoUnbind FrameBuffer::bindScoped() const
  {
    return AutoUnbind{ *this };
  }

  GLuint FrameBuffer::getBoundId()
  {
    GLint id = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &id);
    return static_cast<GLuint>(id);
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
