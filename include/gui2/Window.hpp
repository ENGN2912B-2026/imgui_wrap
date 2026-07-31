//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>

#include <string>
#include <memory>
#include <functional>
#include <mutex>

namespace gui2
{
  // A desktop window.
  class Window
  {
    std::string title_;
    Vec2i size_;
    Widget widget_;
    mutable std::mutex mutex_;
  public:
    Window(
      const std::string& title = "Window",
      const Vec2i& size = {640, 480});
    ~Window();

    const std::string& getTitle() const;
    void setTitle(const std::string& title);

    const Vec2i& getSize() const;
    void setSize(const Vec2i& size);

    void displayContent(const Runtime& runtime) const;
    void setContent(Widget widget);
  };

} // namespace gui
