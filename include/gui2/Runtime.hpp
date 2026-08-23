//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gui2/Types.hpp>
#include <gui2/Rect.hpp>

#include <string>
#include <memory>
#include <concepts>
#include <type_traits>

namespace gui
{
  // forward declaration
  class Backend;
}

namespace gui2
{
  // forward declaration
  //class Backend;
  using Backend = gui::Backend;
  class Empty;
  class Separator;
  class Image;
  class Button;
  class CheckBox;
  class Panel;

  class Runtime
  {
    std::unique_ptr<Backend> backend_;
    Runtime() = default;
  public:
    ~Runtime();
    static Runtime& getInstance();

    // Initializes the runtime and creates a desktop window.
    void init(
      const std::string& windowTitle = "Window",
      const Vec2i& windowSize = {640, 480});

    // Deinitializes the runtime and destroys the desktop window.
    void deinit();

    // Sets the window title of the desktop window.
    void setWindowTitle(const std::string& title);

    // Gets the current window size of the desktop window.
    Vec2i getWindowSize() const;

      // Sets the window size of the desktop window.
    void setWindowSize(const Vec2i& size);

    // Begins a new rendering frame.
    // Returns true if the frame was successfully begun, false otherwise.
    bool frameBegin();

    // Ends the current rendering frame.
    void frameEnd();

    // Display functions ------------------------------------------------------

    // Primitive items which do not have state
    Rect display(const Empty& empty, const Rect& rect) const;
    Rect display(const Separator& separator, const Rect& rect) const;
    Rect display(const std::string& text, const Rect& rect) const;
    Rect display(const Button& button, const Rect& rect) const;
    Rect display(const Image& image, const Rect& rect) const;

    // Primitive items which have state which might change during display
    Rect display(CheckBox& checkBox, const Rect& rect) const;

    // Container items, they maybe have as children other items
    Rect display(Panel& panel, const Rect& rect) const;

    // Other functions --------------------------------------------------------

    // Gets the padding of the desktop window and child windows.
    // This padding is used to compute the inner rectangle of the window
    // where the content is displayed.
    Vec2i getWindowPadding() const;

    // Gets the spacing between items in a layout.
    Vec2i getItemSpacing() const;
  };

  //! \namespace runtime
  //! \brief Contains helper templates and functions used by the Runtime class.
  namespace runtime
  {
    //! \brief Resolves a value by removing any reference wrappers if present.
    //! \tparam T The type of the value to resolve.
    //! \param value The value to resolve.
    //! \return The resolved value.
    template<typename T> inline decltype(auto) resolve_reference(T& value);

    // Helper struct to determine if a type is a factory (a callable that
    // returns a Content type, or a factory that returns another factory,
    // recursively).
    template <typename T> struct is_factory : std::false_type{};
  }

  // Concept of primitive types:
  // - Runtime has a display method for the type.
  template<typename T>
  concept Primitive =
    requires(T& value, Runtime& rt, Rect& rect)
    {
      { rt.display(runtime::resolve_reference(value), rect) }
        -> std::convertible_to<Rect>;
    };

  // Concept of displayable types:
  // - The type has a display method that takes a Runtime and a Rect and returns
  //   a Rect.
  template<typename T>
  concept Displayable =
    requires(T& value, Runtime& rt, Rect& rect)
    {
      { runtime::resolve_reference(value).display(rt, rect) }
        -> std::convertible_to<Rect>;
    };

  // Concept of content types:
  // - The type is either primitive or displayable.
  template<typename T>
  concept Content = Primitive<T> || Displayable<T>;

  // Concept of content factory types:
  // - The type is a callable that returns a Content type.
  // - The type is a callable that returns another factory type.
  template<typename F>
  concept ContentFactory = runtime::is_factory<std::remove_cvref_t<F>>::value;

} // namespace gui

// Implementation -------------------------------------------------------------
namespace gui2
{
  namespace runtime
  {
    // Resolves a value by removing any reference wrappers if present.
    template<typename T>
    inline T& resolve_reference_impl(T& value)
    {
      return value;
    }

    // Specialization for std::reference_wrapper<T>: returns the underlying
    // reference.
    template<typename T>
    inline T& resolve_reference_impl(std::reference_wrapper<T> value)
    {
      return value.get();
    }

    // Resolves a value by removing any reference wrappers if present.
    template<typename T>
    inline decltype(auto) resolve_reference(T& value)
    {
      return resolve_reference_impl(value);
    }

    // Specialization for callable types: the type is a factory if it returns a
    // Content type or another factory type.
    template<typename F>
      requires std::invocable<F&>
    struct is_factory<F>
    {
      using Result = std::remove_cvref_t<std::invoke_result_t<F&>>;
      static constexpr bool value =
        Content<Result> || is_factory<Result>::value;
    };
  }
}
