//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gl/gl.hpp>

#include <utility> // for std::move

namespace gl
{
  //! \brief A class that represents an OpenGL handler.
  class Handler
  {
  public:
    //! \brief Default constructor.
    //!
    //! The handler is not initialized.
    Handler() = default;

    //! \brief Constructs a handler with the given OpenGL ID.
    //! \param[in] id The OpenGL ID of the handler.
    Handler(GLuint id) : id_{id} {}

    //! \brief Copy constructor.
    Handler(const Handler&) = delete;

    //! \brief Move constructor.
    //! \param[in] other The handler to move from.
    Handler(Handler&& other) noexcept;

    //! \brief Destructor.
    ~Handler() = default;

    //! \brief Copy assignment operator.
    Handler& operator=(const Handler&) = delete;

    //! \brief Move assignment operator.
    //! \param[in] other The handler to move from.
    Handler& operator=(Handler&& other) noexcept;

    //! \brief Checks if the handler is initialized.
    //! \return true if the handler is initialized, false otherwise.
    bool isInitialized() const { return id_ > 0; }

    //! \brief Gets the OpenGL ID of the handler.
    //! \return The OpenGL ID of the handler.
    GLuint getId() const { return id_; }

    //! \brief Gets a reference to the OpenGL ID of the handler.
    //! \return A reference to the OpenGL ID of the handler.
    GLuint& getIdRef() { return id_; }

  private:
    GLuint id_ = 0U;
  };

} // namespace gl

// Implementation -------------------------------------------------------------
namespace gl
{
  inline Handler::Handler(Handler&& other) noexcept
    : id_{ std::move(other.id_) }
  {
    other.id_ = 0U;
  }

  inline Handler& Handler::operator=(Handler&& other) noexcept
  {
    if (this != &other)
    {
      id_ = std::move(other.id_);
      other.id_ = 0U;
    }
    return *this;
  }
}