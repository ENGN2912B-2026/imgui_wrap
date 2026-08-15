//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gui2/Types.hpp>
#include <gui2/Rect.hpp>

#include <string>
#include <memory>

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
  class Panel;
  class Empty;
  class Button;
  class CheckBox;

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
    Rect display(const std::string& text, const Rect& rect) const;
    Rect display(const Button& button, const Rect& rect) const;

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

  // Concept of primitive types:
  // - Runtime has a display method for the type
  template<class T>
  concept Primitive =
    requires(Runtime& rt, T& value, Rect& rect)
    {
      { rt.display(value, rect) } -> std::convertible_to<Rect>;
    };

  // Concept of displayable types:
  // - The type has a display method which takes a Runtime& as argument
  template<class T>
  concept Displayable =
    requires(Runtime& rt, T& value, Rect& rect)
    {
      { value.display(rt, rect) } -> std::convertible_to<Rect>;
    };

  // Concept of content types:
  // - The type is either primitive or displayable
  template<class T>
  concept Content =
    Primitive<T> || Displayable<T>;

  // Concept of content factory types:
  // - The type is a callable that returns a Content type
  template<class T>
  concept ContentFactory =
    requires(T& value)
    {
      value();
    } &&
    Content<std::remove_cvref_t<std::invoke_result_t<T&>>>;

} // namespace gui
