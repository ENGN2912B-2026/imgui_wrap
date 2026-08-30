//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gl/gl.hpp>

#include <utility> // for std::move

namespace gl
{
  //! \brief A class that represents an OpenGL handler.
  class Handle
  {
  public:
    //! \brief Default constructor.
    //!
    //! The handler is not initialized.
    Handle() = default;

    //! \brief Constructs a handler with the given OpenGL ID.
    //! \param[in] id The OpenGL ID of the handler.
    Handle(GLuint id) : id_{id} {}

    //! \brief Copy constructor.
    Handle(const Handle&) = delete;

    //! \brief Move constructor.
    //! \param[in] other The handler to move from.
    Handle(Handle&& other) noexcept;

    //! \brief Destructor.
    ~Handle() = default;

    //! \brief Copy assignment operator.
    Handle& operator=(const Handle&) = delete;

    //! \brief Move assignment operator.
    //! \param[in] other The handler to move from.
    Handle& operator=(Handle&& other) noexcept;

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
  inline Handle::Handle(Handle&& other) noexcept
    : id_{ std::move(other.id_) }
  {
    other.id_ = 0U;
  }

  inline Handle& Handle::operator=(Handle&& other) noexcept
  {
    if (this != &other)
    {
      id_ = std::move(other.id_);
      other.id_ = 0U;
    }
    return *this;
  }
}