//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gl/RenderBuffer.hpp>

#include <stdexcept>

namespace gl
{
  void RenderBuffer::initialize()
  {
    if (!isInitialized())
    { // Create a render buffer object for depth and stencil attachment (we
      // won't be sampling these)
      glGenRenderbuffers(1, &getIdRef());
      if (!isInitialized())
      {
        throw std::runtime_error{
          "ERROR::RENDERBUFFER:: Failed to create render buffer object!"};
      }
    }
  }

  void RenderBuffer::uninitialize()
  {
    if (isInitialized())
    {
      glDeleteRenderbuffers(1, &getIdRef());
      getIdRef() = 0U;
    }
  }

  void RenderBuffer::setSize(const Vec2i& size)
  {
    if (!isInitialized())
    {
      throw std::runtime_error{ "ERROR::RENDERBUFFER:: "
        "Cannot set size of uninitialized render buffer!"};
    }

    // Bind the render buffer to set its storage
    const auto autoUnbind = AutoUnbind{ *this };

    // Allocate storage for the render buffer without uploading any data
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
  }

  RenderBuffer::AutoUnbind RenderBuffer::bindScoped() const
  {
    return AutoUnbind{ *this };
  }

  GLuint RenderBuffer::getBoundId()
  {
    GLint id = 0;
    glGetIntegerv(GL_RENDERBUFFER_BINDING, &id);
    return static_cast<GLuint>(id);
  }
}
