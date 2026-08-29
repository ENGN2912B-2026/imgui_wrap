//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gl/gl.hpp>
#include <math/Vec2.hpp>
#include <image/Image.hpp>

namespace gl
{
  using math::Vec2i;

  //! \brief A simple texture class that can be used to manage OpenGL textures.
  class Texture
  {
  public:
    //! \brief Forward declaration of the AutoUnbind class.
    //!
    //! A helper class that automatically unbinds the texture when it goes out
    //! of scope.
    class AutoUnbind;

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

    //! \brief Binds the texture as `GL_TEXTURE_2D` for subsequent OpenGL
    //!        texture operations.
    //!
    //! After calling this method, all subsequent OpenGL texture 2D commands
    //! will be directed to this texture until `unbind()` is called.
    void bind() const;

    //! \brief Binds the texture as `GL_TEXTURE_2D` and returns an AutoUnbind
    //!        object that will automatically unbind the frame buffer when it
    //!        goes out of scope.
    //! \return An AutoUnbind object that will automatically unbind the frame
    //!         buffer when it goes out of scope.
    AutoUnbind bindScoped() const;

    //! \brief Unbinds the texture, restoring the default texture binding.
    //!
    // After calling this method, all subsequent OpenGL texture 2D commands will
    // be directed to the default texture until `bind()` is called again.
    void unbind() const;

    //! \brief Gets the OpenGL texture Id of the frame buffer's color attachment.
    //! \return The OpenGL texture Id of the frame buffer's color attachment.
    GLuint getId() const { return id_; }

    //! \brief Gets the size of the texture in pixels.
    //! \return The size of the texture in pixels.
    //! \throw std::runtime_error if the texture is not initialized.
    Vec2i getSize() const;

    //! \brief Sets the size of the texture in pixels.
    //! \param[in] size  The new size of the texture in pixels.
    //! \throw std::runtime_error if the texture is not initialized.
    void setSize(const Vec2i& size);

    //! \brief Sets the interpolation mode for this texture.
    //! \param[in] interpolationMode  The interpolation mode of the texture.
    //!                               This is a valid OpenGL texture parameter,
    //!                               such as GL_LINEAR or GL_NEAREST.
    //! \throw std::runtime_error if the texture is not initialized.
    void setInterpolationMode(GLint interpolationMode);

    //! \brief Uploads an image to the texture.
    //! \param[in] image  The image to upload.
    //! \throw std::runtime_error if the texture is not initialized, or if the
    //!        image is not tightly packed.
    //!
    //! This method uploads the given image data to the texture. The texture
    //! must be initialized before calling this method. The image must be
    //! tightly packed, meaning that the row stride of the image must be equal
    //! to the width of the image multiplied by the size of the pixel type.
    //! If the size of the image does not match the size of the texture, the
    //! texture will be resized to match the image size.
    void uploadImage(const image::ImageRgb8& image);

  private:
    GLuint id_ = 0U;
  };

} // namespace gl

// Implementation -------------------------------------------------------------
namespace gl
{
  //! \brief A helper class that automatically unbinds the texture when it
  //!        goes out of scope.
  class Texture::AutoUnbind
  {
  public:
    // \brief Constructs an AutoUnbind object and binds the given texture.
    //! \param[in] texture  The texture to bind.
    AutoUnbind(const Texture& texture)
    {
      glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous_);
      texture.bind();
    }
    //! \brief Destructor that automatically unbinds the texture.
    ~AutoUnbind()
    {
      glBindTexture(GL_TEXTURE_2D, previous_);
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
