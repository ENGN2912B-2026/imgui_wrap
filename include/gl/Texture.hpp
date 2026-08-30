//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gl/Handler.hpp>
#include <gl/AutoUnbind.hpp>
#include <image/Image.hpp>

namespace gl
{
  //! \brief A simple texture class that can be used to manage OpenGL textures.
  class Texture : public Handler
  {
  public:
    //! \brief AutoUnbind is a helper class that automatically unbinds the
    //!        texture when it goes out of scope. This is useful for ensuring
    //!        that the texture is properly unbound, even if an exception is
    //!        thrown.
    using AutoUnbind = gl::AutoUnbind<Texture>;

    //! \brief Deleted copy constructor and copy assignment operator, and
    //!        default move constructor and move assignment operator.
    GL_NO_COPY_DEFAULT_MOVE(Texture)

    //! \brief Default constructor.
    //!
    //! The texture is not initialized, and the size is set to (0, 0).
    Texture() = default;

    //! \brief Destructor.
    //!
    //! The texture is deleted, and the OpenGL resources are released.
    ~Texture() { uninitialize(); }

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

    //! \brief Binds the texture as `GL_TEXTURE_2D` and returns an AutoUnbind
    //!        object that will automatically unbind the texture when it goes
    //!        out of scope.
    //! \return An AutoUnbind object that will automatically unbind the texture
    //!         when it goes out of scope.
    AutoUnbind bindScoped() const;

    //! \brief Binds a texture as `GL_TEXTURE_2D`.
    //! \param[in] id The OpenGL ID of the texture to bind.
    static void bind(GLuint id) { glBindTexture(GL_TEXTURE_2D, id); }

    //! \brief Unbinds the current texture, restoring the default texture
    //!        binding.
    static void unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

    //! \brief Gets the OpenGL ID of the currently bound texture.
    //! \return The OpenGL ID of the currently bound texture.
    static GLuint getBoundId();
  };

  //! \brief Static assertion to ensure that Texture is AutoUnbindable.
  static_assert(AutoUnbindable<Texture>, "Texture must be AutoUnbindable");

} // namespace gl
