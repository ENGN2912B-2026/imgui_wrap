//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gl/Texture.hpp>

#include <stdexcept>
#include <cassert>

namespace gl
{
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

  void Texture::initialize(const Vec2i& size, GLint interpolationMode)
  {
    // Ensure the texture is initialized
    initialize();

    // Set the size of the texture
    setSize(size);

    // Set interpolation mode
    setInterpolationMode(interpolationMode);
  }

  void Texture::initialize(const image::ImageRgb8& image,
                           GLint interpolationMode)
  {
    // Ensure the texture is initialized
    initialize();

    // Upload the image data to the texture
    uploadImage(image);

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

  void Texture::setSize(const Vec2i& size)
  {
    if (!isInitialized())
    {
      throw std::runtime_error{ "ERROR::TEXTURE:: "
        "Cannot set size of uninitialized texture!"};
    }

    // Bind the texture to set its parameters
    const auto autoUnbind = AutoUnbind{ *this };

    // Allocate storage for the texture without uploading any data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y,
                 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

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

  void Texture::uploadImage(const image::ImageRgb8& image)
  {
    if (!isInitialized())
    {
      throw std::runtime_error{ "ERROR::TEXTURE:: "
        "Cannot upload image to uninitialized texture!"};
    }

    const int expectedRowStride = image.getWidth() * sizeof(image::Rgb8);
    if (image.getRowStride() != expectedRowStride)
    {
      throw std::invalid_argument{"ERROR::TEXTURE:: "
        "Image is not tightly packed!"};
    }

    // Bind the texture to upload the image data
    const auto autoUnbind = AutoUnbind{ *this };

    // Set pixel storage modes for unpacking the image data
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0); // Ensure no row padding is used
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // Use single byte alignment

    if (getSize() != image.getSize())
    { // Allocate a new texture size and upload the image data
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(),
                   0, GL_RGB, GL_UNSIGNED_BYTE, image.getData());
    }
    else
    { // Upload the image data to the existing texture size
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.getWidth(), image.getHeight(),
                      GL_RGB, GL_UNSIGNED_BYTE, image.getData());
    }
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
