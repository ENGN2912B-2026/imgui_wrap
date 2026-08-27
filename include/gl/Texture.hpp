//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gl/gl.h>
#include <math/Vec2.hpp>
#include <image/Image.hpp>

namespace gl
{
  using math::Vec2i;

  //! \brief A simple texture class that can be used to manage OpenGL textures.
  class Texture
  {
  public:
    //! \brief Default constructor.
    //!
    //! The texture is not initialized, and the size is set to (0, 0).
    Texture() = default;

    //! \brief Copy constructor.
    Texture(const Texture&) = delete;

    //! \brief Move constructor.
    Texture(Texture&&) noexcept;

    //! \brief Destructor.
    //!
    //! The texture is deleted, and the OpenGL resources are released.
    ~Texture();

    //! \brief Copy assignment operator.
    Texture& operator=(const Texture&) = delete;

    //! \brief Move assignment operator.
    Texture& operator=(Texture&&) noexcept;

    //! \brief Checks if the texture is initialized.
    //! \return true if the texture is initialized, false otherwise.
    bool isInitialized() const { return id_ > 0; }

    //! \brief Initializes the texture.
    //! \throw std::runtime_error if the texture fails to be initialized.
    //!
    //! Initializes the texture without allocating any storage. If the texture
    //! is already initialized, this method does nothing.
    void initialize();

    //! \brief Initializes the texture with the given size and interpolation
    //!        mode.
    //! \param[in] size               The size of the texture in pixels.
    //! \param[in] interpolationMode  The interpolation mode of the texture.
    //!                               This is a valid OpenGL texture parameter,
    //!                               such as GL_LINEAR or GL_NEAREST.
    //! \throw std::runtime_error if the texture fails to be initialized.
    //!
    //! This method initializes the texture and allocates storage for it. If the
    //! texture is already initialized, it will be re-initialized with the new
    //! size and interpolation mode.
    void initialize(const Vec2i& size, GLint interpolationMode = GL_LINEAR);

    //! \brief Initializes the texture from the given image.
    //! \param[in] image              The image to upload to the texture.
    //! \param[in] interpolationMode  The interpolation mode of the texture.
    //!                               This is a valid OpenGL texture parameter,
    //!                               such as GL_LINEAR or GL_NEAREST.
    //! \throw std::invalid_argument if the image is not tightly packed.
    //! \throw std::runtime_error if the texture fails to be initialized.
    //!
    //! This method initializes the texture and uploads the image data to it.
    //! If the texture is already initialized, it will be re-initialized with
    //! the new image data.
    void initialize(const image::ImageRgb8& image,
                    GLint interpolationMode = GL_LINEAR);

    //! \brief Deletes the texture and releases the OpenGL resources.
    //!
    //! If the texture is not initialized, this method does nothing.
    void uninitialize();

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

    //! \brief Gets the OpenGL texture Id of the frame buffer's color attachment.
    //! \return The OpenGL texture Id of the frame buffer's color attachment.
    GLuint getId() const { return id_; }

    //! \brief Gets the size of the texture in pixels.
    //! \return The size of the texture in pixels.
    //! \throw std::runtime_error if the texture is not initialized.
    Vec2i getSize() const;

    //! \brief Sets the interpolation mode for this texture.
    //! \param[in] interpolationMode  The interpolation mode of the texture.
    //!                               This is a valid OpenGL texture parameter,
    //!                               such as GL_LINEAR or GL_NEAREST.
    //! \throw std::runtime_error if the texture is not initialized.
    void setInterpolationMode(GLint interpolationMode);

  private:
    GLuint id_ = 0U;
  };

} // namespace gl
