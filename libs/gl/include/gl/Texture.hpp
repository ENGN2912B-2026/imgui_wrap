//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gl/Handle.hpp>
#include <gl/AutoUnbind.hpp>
#include <image/Image.hpp>

#include <concepts>
#include <typeindex>

namespace gl
{
  //! \brief Pixel types supported by Texture.
  template<typename T>
  concept TexturePixelType = std::same_as<T, image::ImageViewRgb8::PixelType> ||
                             std::same_as<T, image::ImageViewGray8::PixelType>;

  //! \brief A simple texture class that can be used to manage OpenGL textures.
  class Texture : public Handle
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

    //! \brief Pixel types supported by Texture.
    using Rgb8 = image::ImageViewRgb8::PixelType;
    using Gray8 = image::ImageViewGray8::PixelType;

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
    //! \tparam PixelType             The image pixel type (RGB by default).
    //! \throw std::runtime_error if the texture fails to be initialized.
    //!
    //! This method initializes the texture and allocates storage for it. If the
    //! texture is already initialized, it will be re-initialized with the new
    //! size, interpolation mode, and format.
    template<TexturePixelType PixelType = Rgb8>
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
    void initialize(const image::ImageViewRgb8& imageView,
                    GLint interpolationMode = GL_LINEAR);

    //! \brief Initializes the texture from the given grayscale image.
    //! \param[in] imageView          The grayscale image to upload.
    //! \param[in] interpolationMode  The interpolation mode of the texture.
    //!                               This is a valid OpenGL texture parameter,
    //!                               such as GL_LINEAR or GL_NEAREST.
    //! \throw std::invalid_argument if the image is not tightly packed.
    //! \throw std::runtime_error if the texture fails to be initialized.
    //!
    //! This method initializes the texture and uploads the image data to it.
    //! If the texture is already initialized, it will be re-initialized with
    //! the new image data.
    void initialize(const image::ImageViewGray8& imageView,
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
    //! \param[in] size   The new size of the texture in pixels.
    //! \tparam PixelType The image pixel type (RGB by default).
    //! \throw std::runtime_error if the texture is not initialized.
    //!
    //! Reallocates the texture and discards its previous contents.
    template<TexturePixelType PixelType = Rgb8>
    void setSize(const Vec2i& size);

    //! \brief Gets the texture format.
    //! \return The type index of the image pixel type, comparable with
    //!         typeid(image::ImageViewRgb8::PixelType) or
    //!         typeid(image::ImageViewGray8::PixelType).
    //! \throw std::runtime_error if the texture is not initialized, has no
    //!                           storage, or has an unsupported format.
    std::type_index getFormat() const;

    //! \brief Sets the texture format while preserving its size.
    //! \tparam PixelType  The new image pixel type.
    //! \throw std::runtime_error if the texture is not initialized, has no
    //!                           storage, or has an unsupported format.
    //!
    //! Changing the format reallocates the texture and discards its contents.
    //! Setting the existing format does nothing.
    template<TexturePixelType PixelType>
    void setFormat();

    //! \brief Sets the interpolation mode for this texture.
    //! \param[in] interpolationMode  The interpolation mode of the texture.
    //!                               This is a valid OpenGL texture parameter,
    //!                               such as GL_LINEAR or GL_NEAREST.
    //! \throw std::runtime_error if the texture is not initialized.
    void setInterpolationMode(GLint interpolationMode);

    //! \brief Uploads an image to the texture.
    //! \param[in] image  The image to upload.
    //! \throw std::runtime_error if the texture is not initialized.
    //! \throw std::invalid_argument if the image is not tightly packed.
    //!
    //! This method uploads the given image data to the texture. The texture
    //! must be initialized before calling this method. The image must be
    //! tightly packed, meaning that the row stride of the image must be equal
    //! to the width of the image multiplied by the size of the pixel type
    //! (three bytes per pixel). If the size of the image does not match the
    //! size of the texture, the texture will be resized to match the image
    //! size. The texture format is changed to RGB if necessary.
    void uploadImage(const image::ImageViewRgb8& imageView);

    //! \brief Uploads a grayscale image to the texture.
    //! \param[in] imageView  The grayscale image to upload.
    //! \throw std::runtime_error if the texture is not initialized.
    //! \throw std::invalid_argument if the image is not tightly packed.
    //!
    //! This method uploads the given image data to the texture. The texture
    //! must be initialized before calling this method. The image must be
    //! tightly packed, meaning that the row stride of the image must be equal
    //! to the width of the image (one byte per pixel). If the size of the
    //! image does not match the size of the texture, the texture will be
    //! resized to match the image size. The texture format is changed to
    //! grayscale if necessary.
    void uploadImage(const image::ImageViewGray8& imageView);

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

  private:
    void setSize_(const Vec2i& size, std::type_index format);
    void setFormat_(std::type_index format);
    void uploadImageData_(const void* data, const Vec2i& size,
                          std::type_index format);

  };

  //! \brief Static assertion to ensure that Texture is AutoUnbindable.
  static_assert(AutoUnbindable<Texture>, "Texture must be AutoUnbindable");

} // namespace gl

// Implementation -------------------------------------------------------------
namespace gl
{
  template<TexturePixelType PixelType>
  void Texture::initialize(const Vec2i& size, GLint interpolationMode)
  {
    initialize();
    setSize<PixelType>(size);
    setInterpolationMode(interpolationMode);
  }

  template<TexturePixelType PixelType>
  void Texture::setSize(const Vec2i& size)
  {
    setSize_(size, typeid(PixelType));
  }

  template<TexturePixelType PixelType>
  void Texture::setFormat()
  {
    setFormat_(typeid(PixelType));
  }
}
