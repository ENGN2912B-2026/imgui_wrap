//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gl/Texture.hpp>

#include <stdexcept>
#include <cassert>

namespace gl
{
  namespace
  {
    GLenum getPixelFormat_(std::type_index format)
    {
      if (format == typeid(image::ImageViewRgb8::PixelType)) return GL_RGB;
      if (format == typeid(image::ImageViewGray8::PixelType)) return GL_RED;
      throw std::invalid_argument{"ERROR::TEXTURE:: Invalid texture format!"};
    }

    void setSwizzle_(std::type_index format)
    {
      const GLint rgbSwizzle[] = {GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA};
      const GLint graySwizzle[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
      glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA,
                       format == typeid(image::ImageViewGray8::PixelType)
                         ? graySwizzle : rgbSwizzle);
    }
  }

  void Texture::initialize()
  {
    if (!isInitialized())
    { // Create a color attachment texture
      glGenTextures(1, &getIdRef());
      if (!isInitialized())
      {
        throw std::runtime_error{"ERROR::TEXTURE:: Failed to create texture!"};
      }
    }

    // Set default texture parameters for wrapping and filtering
    const auto autoUnbind = AutoUnbind{ *this };
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  void Texture::initialize(const image::ImageViewRgb8& imageView,
                           GLint interpolationMode)
  {
    // Ensure the texture is initialized
    initialize();

    // Upload the image data to the texture
    uploadImage(imageView);

    // Set interpolation mode
    setInterpolationMode(interpolationMode);
  }

  void Texture::initialize(const image::ImageViewGray8& imageView,
                           GLint interpolationMode)
  {
    // Ensure the texture is initialized
    initialize();

    // Upload the image data to the texture
    uploadImage(imageView);

    // Set interpolation mode
    setInterpolationMode(interpolationMode);
  }

  void Texture::uninitialize()
  {
    if (isInitialized())
    {
      glDeleteTextures(1, &getIdRef());
      getIdRef() = 0;
    }
  }

  Vec2i Texture::getSize() const
  {
    if (!isInitialized())
    {
      throw std::runtime_error{ "ERROR::TEXTURE:: "
        "Cannot get size of uninitialized texture!"};
    }

    // Bind the texture to get its parameters
    const auto autoUnbind = AutoUnbind{ *this };

    // Get the texture size
    GLint width, height;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

    return Vec2i{ width, height };
  }

  void Texture::setSize_(const Vec2i& size, std::type_index format)
  {
    if (!isInitialized())
    {
      throw std::runtime_error{ "ERROR::TEXTURE:: "
        "Cannot set size of uninitialized texture!"};
    }

    const GLenum glFormat = getPixelFormat_(format);

    // Bind the texture to set its parameters
    const auto autoUnbind = AutoUnbind{ *this };

    // Allocate storage for the texture without uploading any data
    glTexImage2D(GL_TEXTURE_2D, 0, glFormat, size.x, size.y, 0, glFormat,
                 GL_UNSIGNED_BYTE, nullptr);
    setSwizzle_(format);
  }

  std::type_index Texture::getFormat() const
  {
    if (!isInitialized())
    {
      throw std::runtime_error{ "ERROR::TEXTURE:: "
        "Cannot get format of uninitialized texture!"};
    }
    const auto autoUnbind = AutoUnbind{ *this };
    GLint internalFormat = 0;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT,
                             &internalFormat);
    if (internalFormat == GL_RGB8 || internalFormat == GL_RGB)
      return typeid(image::ImageViewRgb8::PixelType);
    if (internalFormat == GL_R8 || internalFormat == GL_RED)
      return typeid(image::ImageViewGray8::PixelType);

    throw std::runtime_error{ "ERROR::TEXTURE:: "
      "Texture format is unsupported or unallocated!"};
  }

  void Texture::setFormat_(std::type_index format)
  {
    if (!isInitialized())
    {
      throw std::runtime_error{ "ERROR::TEXTURE:: "
        "Cannot set format of uninitialized texture!"};
    }
    if (format != getFormat())
    {
      setSize_(getSize(), format);
    }
  }

  void Texture::setInterpolationMode(GLint interpolationMode)
  {
    if (!isInitialized())
    {
      throw std::runtime_error{ "ERROR::TEXTURE:: "
        "Cannot set interpolation mode on uninitialized texture!"};
    }

    // Bind the texture to set its parameters
    const auto autoUnbind = AutoUnbind{ *this };

    // Set texture parameters for filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolationMode);
  }

  void Texture::uploadImage(const image::ImageViewRgb8& imageView)
  {
    if (!isInitialized())
    {
      throw std::runtime_error{ "ERROR::TEXTURE:: "
        "Cannot upload image to uninitialized texture!"};
    }

    const int expectedRowStride = imageView.getWidth() * sizeof(image::Rgb8);
    if (imageView.getRowStride() != expectedRowStride)
    {
      throw std::invalid_argument{"ERROR::TEXTURE:: "
        "Image is not tightly packed!"};
    }

    uploadImageData_(imageView.getData(), imageView.getSize(),
                     typeid(image::ImageViewRgb8::PixelType));
  }

  void Texture::uploadImage(const image::ImageViewGray8& imageView)
  {
    if (!isInitialized())
    {
      throw std::runtime_error{ "ERROR::TEXTURE:: "
        "Cannot upload image to uninitialized texture!"};
    }

    const int expectedRowStride = imageView.getWidth() * sizeof(uint8_t);
    if (imageView.getRowStride() != expectedRowStride)
    {
      throw std::invalid_argument{"ERROR::TEXTURE:: "
        "Image is not tightly packed!"};
    }

    uploadImageData_(imageView.getData(), imageView.getSize(),
                     typeid(image::ImageViewGray8::PixelType));
  }

  void Texture::uploadImageData_(const void* data, const Vec2i& size,
                                 std::type_index format)
  {
    if (getSize() != size || getFormat() != format)
    {
      setSize_(size, format);
    }

    const auto autoUnbind = AutoUnbind{ *this };
    // Set pixel storage modes for unpacking the image data
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0); // Ensure no row padding is used
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // Use single byte alignment

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.x, size.y,
                    getPixelFormat_(format), GL_UNSIGNED_BYTE, data);
  }

  Texture::AutoUnbind Texture::bindScoped() const
  {
    return AutoUnbind{ *this };
  }

  GLuint Texture::getBoundId()
  {
    GLint id = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &id);
    return static_cast<GLuint>(id);
  }

} // namespace gl
