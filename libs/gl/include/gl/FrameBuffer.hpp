//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gl/RenderBuffer.hpp>
#include <gl/Texture.hpp>

namespace gl
{
  //! \brief A simple frame buffer class that can be used to render OpenGL
  //         content.
  class FrameBuffer : public Handle
  {
  public:
    //! \brief AutoUnbind is a helper class that automatically unbinds the
    //!        frame buffer when it goes out of scope. This is useful for
    //!        ensuring that the frame buffer is properly unbound, even if
    //!        an exception is thrown.
    using AutoUnbind = gl::AutoUnbind<FrameBuffer>;

    //! \brief Default constructor.
    //!
    //! The frame buffer is not initialized, and the size is set to (0, 0).
    FrameBuffer() = default;

    //! \brief Constructs a frame buffer with the given size and texture
    //!        interpolation mode.
    //! \param[in] size               The size of the frame buffer in pixels.
    //! \param[in] interpolationMode  The interpolation mode of the frame
    //!                               buffer's texture. This is a valid OpenGL
    //!                               texture parameter, such as GL_LINEAR
    //!                               or GL_NEAREST.
    //!
    //! The frame buffer is initialized with the given size and texture
    //! interpolation mode.
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

    //! \brief Initializes the frame buffer.
    //! \throw std::runtime_error if the texture fails to be initialized.
    //!
    //! Initializes the texture without allocating any storage. If the texture
    //! is already initialized, this method does nothing.
    void initialize();

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

    //! \brief Deletes the frame buffer and releases the OpenGL resources.
    //!
    //! If the frame buffer is not initialized, this method does nothing.
    void uninitialize();

    //! \brief Checks if the frame buffer is complete.
    //! \return true if the frame buffer is complete, false otherwise.
    bool isComplete() const;

    //! \brief Sets the size of the frame buffer.
    //! \param[in] size  The new size of the frame buffer in pixels.
    //!
    //! If the frame buffer is already initialized, but with a different size,
    //! it is updated accordingly. If the frame buffer is not initialized, it
    //! is initialized with the given size and the current interpolation mode.
    void setSize(const Vec2i& size);

    //! \brief Gets the size of the frame buffer.
    //! \return The size of the frame buffer in pixels.
    //! \throw std::runtime_error if the frame buffer is not initialized.
    Vec2i getSize() const;

    //! \brief Gets the OpenGL texture of the frame buffer's color attachment.
    //! \return The OpenGL texture of the frame buffer's color attachment.
    const Texture& getTexture() const { return texture_; }

    //! \brief Binds the frame buffer as `GL_FRAMEBUFFER` and returns an
    //!        AutoUnbind object that will automatically unbind the frame buffer
    //!        when it goes out of scope.
    //! \return An AutoUnbind object that will automatically unbind the frame
    //!         buffer when it goes out of scope.
    AutoUnbind bindScoped() const;

    //! \brief Binds a frame buffer as `GL_FRAMEBUFFER`.
    //! \param[in] id The OpenGL ID of the frame buffer to bind.
    static void bind(GLuint id) {  glBindFramebuffer(GL_FRAMEBUFFER, id); }

    //! \brief Unbinds the current frame buffer, restoring the default
    //!        frame buffer.
    static void unbind() { bind(0); }

    //! \brief Gets the OpenGL ID of the currently bound frame buffer.
    //! \return The OpenGL ID of the currently bound frame buffer.
    static GLuint getBoundId();

  private:
    Texture texture_           = {};
    RenderBuffer renderBuffer_ = {};

    void ensureCompleteIfNonEmpty_(const Vec2i& size);
  };

  //! \brief Static assertion to ensure that FrameBuffer is AutoUnbindable.
  static_assert(AutoUnbindable<FrameBuffer>, "FrameBuffer must be AutoUnbindable");


} // namespace gl
