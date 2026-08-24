//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gl/Texture.hpp>

#include <stdexcept>
#include <cassert>

namespace gl
{
  Texture::Texture(Texture&& other) noexcept
  {
    operator=(std::move(other));
  }

  Texture::~Texture()
  {
    unitialize();
  }

  Texture& Texture::operator=(Texture&& other) noexcept
  {
    if (this != &other)
    {
      id_ = other.id_;

      // Reset the other texture to a default state
      Texture empty;
      other.id_ = empty.id_;
    }
    return *this;
  }

  void Texture::initialize()
  {
    if (id_ == 0)
    { // Create a color attachment texture
      glGenTextures(1, &id_);
      if (id_ == 0)
      {
        throw std::runtime_error{"ERROR::TEXTURE:: Failed to create texture!"};
      }
    }

    assert(isInitialized() && "Texture object must be valid");
    assert(id_ > 0 && "Texture object must be valid");
  }

  void Texture::initialize(const Vec2i& size, GLint interpolationMode)
  {
    // Ensure the texture is initialized
    initialize();

    // Bind
    bind();

    // Upload the image data to the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y,
                 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    // Set interpolation mode
    setInterpolationMode(interpolationMode);

    // Unbind
    unbind();
  }

  void Texture::initialize(const image::ImageRgb8& image,
                           GLint interpolationMode)
  {
    const int expectedRowStride = image.getWidth() * sizeof(image::Rgb8);
    if (image.getRowStride() != expectedRowStride)
    {
      throw std::invalid_argument{"ERROR::TEXTURE:: "
        "Image is not tightly packed!"};
    }

    // Ensure the texture is initialized
    initialize();

    // Bind
    bind();

    // Set pixel storage modes for unpacking the image data
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0); // Ensure no row padding is used
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // Use single byte alignment

    // Upload the image data to the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getWidth(), image.getHeight(),
                 0, GL_RGB, GL_UNSIGNED_BYTE, image.getData());

    // Set interpolation mode
    setInterpolationMode(interpolationMode);

    // Unbind
    unbind();
  }

  void Texture::unitialize()
  {
    if (id_ > 0)
    {
      glDeleteTextures(1, &id_);
      id_ = 0;
    }
  }

  void Texture::bind() const
  {
    glBindTexture(GL_TEXTURE_2D, id_);
  }

  void Texture::unbind() const
  {
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void Texture::setInterpolationMode(GLint interpolationMode)
  {
    if (!isInitialized())
    {
      throw std::runtime_error{ "ERROR::TEXTURE:: "
        "Cannot set interpolation mode on uninitialized texture!"};
    }

    // Bind the texture to set its parameters
    bind();

    // Set texture parameters for filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolationMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolationMode);

    // Unbind the texture after setting its parameters
    unbind();
  }

} // namespace gl
