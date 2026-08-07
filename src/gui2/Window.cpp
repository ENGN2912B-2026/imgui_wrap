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

  Window::~Window() = default;

  const std::string& Window::getTitle() const
  {
    std::lock_guard<std::mutex> lock(mutex_);
    return title_;
  }

  void Window::setTitle(const std::string& title)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    title_ = title;
  }

  const Vec2i& Window::getSize() const
  {
    std::lock_guard<std::mutex> lock(mutex_);
    return size_;
  }

  void Window::setSize(const Vec2i& size)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    size_ = size;
  }

  void Window::displayContent(const Runtime& runtime)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    widget_.display(runtime);
  }

  void Window::setContent(Widget widget)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    widget_ = std::move(widget);
  }
}