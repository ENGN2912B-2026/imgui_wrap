//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gl/gl.h>
#include <gl/Texture.hpp>
#include <math/Vec2.hpp>

namespace gl
{
  using math::Vec2i;

  //! \brief A simple frame buffer class that can be used to render OpenGL
  //         content.
  class FrameBuffer
  {
  public:
    //! \brief Forward declaration of the AutoUnbind class.
    //!
    //! A helper class that automatically unbinds the frame buffer when it goes
    //! out of scope. This is useful for ensuring that the frame buffer is
    //! properly unbound after rendering, even if an exception is thrown.
    class AutoUnbind;

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
    bool isInitialized() const;

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
    void initialize(const Vec2i& size, GLint interpolationMode = GL_LINEAR);

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

    //! \brief Binds the frame buffer for rendering and returns an AutoUnbind
    //!        object that will automatically unbind the frame buffer when it
    //!        goes out of scope.
    //! \return An AutoUnbind object that will automatically unbind the frame
    //!         buffer when it goes out of scope.
    AutoUnbind bindScoped() const;

    //! \brief Unbinds the frame buffer, restoring the default frame buffer.
    //!
    //! After calling this method, all subsequent OpenGL rendering commands will
    //! be directed to the default frame buffer until `bind()` is called again.
    void unbind() const;

    //! \brief Gets the OpenGL texture of the frame buffer's color attachment.
    //! \return The OpenGL texture of the frame buffer's color attachment.
    const Texture& getTexture() const { return texture_; }

  private:
    Vec2i size_               = {0, 0};
    GLuint fbo_               = 0U;
    Texture texture_          = {};
    GLuint rbo_               = 0U;
    GLint interpolationMode_  = GL_LINEAR;

    void generateFrameBuffer_();
  };

} // namespace gl


// Implementation -------------------------------------------------------------
namespace gl
{
  //! \brief A helper class that automatically unbinds the frame buffer when it
  //!        goes out of scope.
  class FrameBuffer::AutoUnbind
  {
  public:
    // \brief Constructs an AutoUnbind object and binds the given frame buffer.
    //! \param[in] frameBuffer  The frame buffer to bind.
    AutoUnbind(const FrameBuffer& frameBuffer)
    {
      glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &previous_);
      frameBuffer.bind();
    }
    //! \brief Destructor that automatically unbinds the frame buffer.
    ~AutoUnbind()
    {
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, previous_);
    }
    //! \brief Deleted copy constructor to prevent copying.
    AutoUnbind(const AutoUnbind&) = delete;
    //! \brief Deleted move constructor to prevent moving.
    AutoUnbind(AutoUnbind&&) = delete;
    //! \brief Deleted copy assignment operator to prevent copying.
    AutoUnbind& operator=(const AutoUnbind&) = delete;
    //! \brief Deleted move assignment operator to prevent moving.
    AutoUnbind& operator=(AutoUnbind&&) = delete;
  private:
    GLint previous_;
  };
}
