//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gui2/Types.hpp>

#include <string>
#include <memory>
#include <vector>
#include <optional>

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
  class VBox;
  class HBox;
  class Empty;
  class Button;
  class CheckBox;
  class Dynamic;

  using OptionalSize = std::optional<Vec2i>;

  class Runtime;

  // Concept to check if a type can be displayed by the Runtime
  template<class T>
  concept Displayable =
    requires(Runtime& rt, T& value)
    {
      value.display(rt);
    };

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

    // Display functions

    // Primitive items which do not have state
    void display(const std::string& text, const OptionalSize& displaySize = {}) const;
    void display(const Empty& empty, const OptionalSize& displaySize = {}) const;
    void display(const Button& button, const OptionalSize& displaySize = {}) const;

    // Primitive items which have state which might change during display
    void display(CheckBox& checkBox, const OptionalSize& displaySize = {}) const;

    // Container items, they maybe have children which could change during display
    void display(Panel& panel, const OptionalSize& displaySize = {}) const;

    // Dynamic items, they are evaluated at runtime and may change during display
    void display(Dynamic& dynamic, const OptionalSize& displaySize = {}) const;
    template<Displayable T>
    void display(T& value, const OptionalSize& displaySize = {}) const
    {
      value.display(*this, displaySize);
    }

    // Layout containers, they do not really belong in the runtime renderer,
    // their API doesn't include a "display" method. We keep them here for
    // now to make everything functional, but we should consider moving them
    // to a separate layout module.
    void display(VBox& vbox, const OptionalSize& displaySize = {}) const;
    void display(HBox& hbox, const OptionalSize& displaySize = {}) const;
  };

} // namespace gui
