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
  class Button;

  using OptionalSize = std::optional<Vec2i>;

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
    void display(const std::string& text,
                 const OptionalSize& displaySize = {}) const;
    void display(const Panel& panel,
                 const OptionalSize& displaySize = {}) const;
    void display(const VBox& vbox,
                 const OptionalSize& displaySize = {}) const;
    void display(const HBox& hbox,
                 const OptionalSize& displaySize = {}) const;
    void display(const Button& button,
                 const OptionalSize& displaySize = {}) const;
  };

} // namespace gui
