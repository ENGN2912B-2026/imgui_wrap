//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gl/gl.h>
#include <math/Vec2.hpp>

namespace gl
{
  using math::Vec2i;

  //! \brief A simple frame buffer class that can be used to render OpenGL
  //         content.
  class FrameBuffer
  {
  public:
    //! \brief Default constructor.
    //!
    //! The frame buffer is not initialized, and the size is set to (0, 0).
    FrameBuffer() = default;

    //! \brief Constructs a frame buffer with the given size and interpolation
    //!        mode.
    //! \param[in] size               The size of the frame buffer in pixels.
    //! \param[in] interpolationMode  The interpolation mode of the texture.
    //!                               This is a valid OpenGL texture parameter,
    //!                               such as GL_LINEAR or GL_NEAREST.
    //!
    //! The frame buffer is initialized with the given size and interpolation
    //! mode.
    FrameBuffer(const Vec2i& size, GLint interpolationMode = GL_LINEAR);

    //! \brief Copy constructor.
    FrameBuffer(const FrameBuffer&) = delete;

    //! \brief Move constructor.
    FrameBuffer(FrameBuffer&&) noexcept;

    //! \brief Destructor.
    //!
    //! The frame buffer is deleted, and the OpenGL resources are released.
    ~FrameBuffer();

    //! \brief Copy assignment operator.
    FrameBuffer& operator=(const FrameBuffer&) = delete;

    //! \brief Move assignment operator.
    FrameBuffer& operator=(FrameBuffer&&) noexcept;

    //! \brief Checks if the frame buffer is initialized.
    //! \return true if the frame buffer is initialized, false otherwise.
    bool isInitialized() const { return fbo_ > 0 && texture_ > 0 && rbo_ > 0; }

    //! \brief Initializes the frame buffer with the given size and
    //!        interpolation mode.
    //! \param[in] size               The size of the frame buffer in pixels.
    //! \param[in] interpolationMode  The interpolation mode of the texture.
    //!                               This is a valid OpenGL texture parameter,
    //!                               such as GL_LINEAR or GL_NEAREST.
    //!
    //! The frame buffer is initialized with the given size and interpolation
    //! mode. If the frame buffer is already initialized, but with a different
    //! size or interpolation mode, it is updated accordingly.
    void initialize(const Vec2i& size, GLint interpolationMode);

    //! \brief Sets the size of the frame buffer.
    //! \param[in] size  The new size of the frame buffer in pixels.
    //!
    //! If the frame buffer is already initialized, but with a different size,
    //! it is updated accordingly. If the frame buffer is not initialized, it
    //! is initialized with the given size and the current interpolation mode.
    void setSize(const Vec2i& size);

    //! \brief Gets the size of the frame buffer.
    //! \return The size of the frame buffer in pixels.
    const Vec2i& getSize() const { return size_; }

    //! \brief Sets the interpolation mode of the frame buffer's texture.
    //! \param[in] mode  The new interpolation mode.
    //!
    //! If the frame buffer is already initialized, the texture's interpolation
    //! mode is updated accordingly. If the frame buffer is not initialized, it
    //! is initialized with the current size and the new interpolation mode.
    void setInterpolationMode(GLint mode);

    //! \brief Gets the interpolation mode of the frame buffer's texture.
    //! \return The interpolation mode of the frame buffer's texture.
    const GLint getInterpolationMode() const { return interpolationMode_; }

    //! \brief Binds the frame buffer for rendering.
    //!
    //! After calling this method, all subsequent OpenGL rendering commands will
    //! be directed to this frame buffer until `unbind()` is called.
    void bind() const;

    //! \brief Unbinds the frame buffer, restoring the default frame buffer.
    //!
    //! After calling this method, all subsequent OpenGL rendering commands will
    //! be directed to the default frame buffer until `bind()` is called again.
    void unbind() const;

    //! \brief Gets the OpenGL texture ID of the frame buffer's color attachment.
    //! \return The OpenGL texture ID of the frame buffer's color attachment.
    GLuint getTexture() const { return texture_; }

  private:
    Vec2i size_               = {0, 0};
    GLuint fbo_               = 0U;
    GLuint texture_           = 0U;
    GLuint rbo_               = 0U;
    GLint interpolationMode_  = GL_LINEAR;

    void generateFrameBuffer_();
  };

} // namespace gl
