//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gl/gl.hpp>

namespace gl
{
  //! \brief A concept that checks if a type is auto-unbindable.
  template<typename Object>
  concept AutoUnbindable =
    requires(const Object& obj, GLuint id)
    {
      { Object::getBoundId() } -> std::same_as<GLuint>;
      { Object::bind(id) } -> std::same_as<void>;
      { obj.getId() } -> std::same_as<GLuint>;
    };

  //! \brief A helper class that automatically unbinds the frame buffer when it
  //!        goes out of scope.
  template <typename Object>
  class AutoUnbind
  {
  public:
    //! \brief Constructs an AutoUnbind object and binds the given instance of
    //!        the Object type.
    //! \param[in] object  The instance of the Object type to bind.
    AutoUnbind(const Object& object) requires AutoUnbindable<Object>
     : previous_{ Object::getBoundId() }
    {
      Object::bind(object.getId());
    }
    //! \brief Destructor that automatically unbinds the frame buffer.
    ~AutoUnbind()
    {
      Object::bind(previous_);
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
    GLuint previous_;
  };

} // namespace gl
