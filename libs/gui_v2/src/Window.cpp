//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Window.hpp>

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
    if (menuBar_)
    {
      auto menuBar = *menuBar_;
      rt.display(menuBar);
      widget_.display(rt, rt.getContentRect());
      return;
    }
    Vec2i pad = rt.getWindowPadding();
    Rect rect{ pad, size_ - pad * 2 };
    widget_.display(rt, rect);
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
}