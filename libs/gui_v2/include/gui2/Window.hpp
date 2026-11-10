//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>
#include <gui2/Menu.hpp>

#include <string>
#include <memory>
#include <functional>
#include <vector>
//#include <mutex>

namespace gui2
{
  class Dialog;

  // A desktop window.
  class Window
  {
    std::string title_;
    Vec2i size_;
    Widget widget_;
    std::optional<MenuBar> menuBar_;
    std::vector<Dialog*> dialogs_;
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

    // Sets the window's menu bar. Captured references must outlive its use.
    void setMenuBar(MenuBar menuBar);
    // Removes the window's menu bar.
    void clearMenuBar();
    // Returns `true` if this window has a menu bar.
    bool hasMenuBar() const { return menuBar_.has_value(); }

    // Registers a modal dialog displayed after the window content. The dialog
    // must remain alive while the window displays it.
    void addDialog(Dialog& dialog);
  };

} // namespace gui
