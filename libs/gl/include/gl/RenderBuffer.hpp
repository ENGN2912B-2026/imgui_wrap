//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gl/Handle.hpp>
#include <gl/AutoUnbind.hpp>

namespace gl
{
  //! \brief A simple render buffer class that can be used to manage OpenGL
  //!        render buffers.
  class RenderBuffer : public Handle
  {
  public:
    //! \brief AutoUnbind is a helper class that automatically unbinds the
    //!        render buffer when it goes out of scope. This is useful for
    //!        ensuring that the render buffer is properly unbound, even if
    //!        an exception is thrown.
    using AutoUnbind = gl::AutoUnbind<RenderBuffer>;

    //! \brief Deleted copy constructor and copy assignment operator, and
    //!        default move constructor and move assignment operator.
    GL_NO_COPY_DEFAULT_MOVE(RenderBuffer)

    //! \brief Default constructor.
    //!
    //! The render buffer is not initialized.
    RenderBuffer() = default;

    //! \brief Destructor.
    ~RenderBuffer() { uninitialize(); }

    //! \brief Initializes the render buffer.
    //! \throw std::runtime_error if the render buffer fails to be initialized.
    //!
    //! Initializes the render buffer without allocating any storage. If the
    //! render buffer is already initialized, this method does nothing.
    void initialize();

    //! \brief Deletes the render buffer and releases the OpenGL resources.
    //!
    //! If the render buffer is not initialized, this method does nothing.
    void uninitialize();

    //! \brief Sets the size of the render buffer.
    //! \param[in] size  The new size of the render buffer in pixels.
    //! \throw std::runtime_error if the render buffer is not initialized.
    //!
    //! This method allocates storage for the render buffer with the given size.
    void setSize(const Vec2i& size);

    //! \brief Binds the frame buffer as `GL_DRAW_FRAMEBUFFER` for subsequent
    //!        OpenGL rendering operations and returns an AutoUnbind object that
    //!        will automatically unbind the frame buffer when it goes out of
    //!        scope.
    //! \return An AutoUnbind object that will automatically unbind the frame
    //!         buffer when it goes out of scope.
    AutoUnbind bindScoped() const;

    //! \brief Binds a render buffer as `GL_RENDERBUFFER`.
    //! \param[in] id The OpenGL ID of the render buffer to bind.
    static void bind(GLuint id) { glBindRenderbuffer(GL_RENDERBUFFER, id); }

    //! \brief Unbinds the current render buffer, restoring the default render
    //!        buffer.
    static void unbind() { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

    //! \brief Gets the OpenGL ID of the currently bound render buffer.
    //! \return The OpenGL ID of the currently bound render buffer.
    static GLuint getBoundId();
  };

  //! \brief Static assertion to ensure that RenderBuffer is AutoUnbindable.
  static_assert(AutoUnbindable<RenderBuffer>, "RenderBuffer must be AutoUnbindable");

} // namespace gl
