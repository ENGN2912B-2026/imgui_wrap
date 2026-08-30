//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>

#include <string>
#include <memory>
#include <functional>
//#include <mutex>

namespace gui2
{
  // A desktop window.
  class Window
  {
    std::string title_;
    Vec2i size_;
    Widget widget_;
  public:
    // Constructs a new Window with the given title and size.
    // \param[in] title The title of the window.
    // \param[in] size The size of the window in pixels. When the window is
    //                 created by the system, its actual size is usually scaled
    //                 according to the display DPI, so the actual size of the
    //                 window when first shown may be different.
    Window(const std::string& title = "Window",
           const Vec2i& size = {640, 480});

    // Gets the title of the window.
    const std::string& getTitle() const;
    // Sets the title of the window.
    void setTitle(const std::string& title);

    // Gets the current size of the window in pixels.
    const Vec2i& getSize() const;
    // Sets the size of the window in pixels.
    void setSize(const Vec2i& size);

    // Displays the content of the window using the provided Runtime.
    // This function is called within the rendering loop in the application
    // for each rendered frame to display the content of the window. It is not
    // intended to be called directly by the user.
    void displayContent(const Runtime& rt);

    // Sets the content of the window to be displayed. The content is provided
    // as a Widget, which can be any displayable GUI element or container.
    void setContent(Widget widget);
  };

} // namespace gui
