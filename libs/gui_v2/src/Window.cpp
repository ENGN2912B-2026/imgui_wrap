//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Window.hpp>
#include <gui2/Dialog.hpp>

#include <algorithm>

namespace gui2
{
  Window::Window(const std::string& title, const Vec2i& size)
    : title_{title}
    , size_{size}
    , widget_{}
  {

  }

  const std::string& Window::getTitle() const
  {
    return title_;
  }

  void Window::setTitle(const std::string& title)
  {
    title_ = title;
  }

  const Vec2i& Window::getSize() const
  {
    return size_;
  }

  void Window::setSize(const Vec2i& size)
  {
    size_ = size;
  }

  void Window::displayContent(const Runtime& rt)
  {
    Rect rect{{0, 0}, size_};
    Rect actualRect{rect.origin, {0, 0}};

    // Display the menu bar if it exists, and update the actualRect to include
    // its size.
    if (menuBar_)
    {
      // Copy the menu bar so it remains alive if a callback clears or replaces
      // the original. Its Widgets are shallow copies that share their content.
      auto menuBar = *menuBar_;
      actualRect.unite(rt.display(menuBar, rect));
    }

    // Display the window content, adjusting the rectangle to account for the
    // menu bar and window padding.
    rect.origin.y += actualRect.size.y;
    rect.size.y -= actualRect.size.y;
    rect.origin += rt.getWindowPadding();
    rect.size -= rt.getWindowPadding() * 2;
    widget_.display(rt, rect);

    // Display any registered dialogs, which are modal and appear above the
    // window content. The dialogs are displayed in the order they were added.
    for (Dialog* dialog : dialogs_)
    {
      rt.display(*dialog, rect);
    }
  }

  void Window::setContent(Widget widget)
  {
    widget_ = std::move(widget);
  }

  void Window::setMenuBar(MenuBar menuBar)
  {
    menuBar_ = std::move(menuBar);
  }

  void Window::clearMenuBar()
  {
    menuBar_.reset();
  }

  void Window::addDialog(Dialog& dialog)
  {
    dialogs_.push_back(&dialog);
  }
}