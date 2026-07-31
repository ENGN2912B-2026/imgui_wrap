//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gui2/Types.hpp>
#include <gui2/Runtime.hpp>

#include <string>
#include <memory>
#include <functional>
#include <mutex>

namespace gui2
{
  class Window
  {
    std::string title_;
    Vec2i size_;
    std::function<void(class Runtime&)> contentCallback_;
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

    template<typename T>
    void setContent(T content) //TODO: Use concepts to constrain T to be primitive types that runtime can display
    {
      std::lock_guard<std::mutex> lock(mutex_);
      contentCallback_ = [val = std::move(content)](Runtime& rt) { rt.display(val); };
    }

    void displayContent(Runtime& runtime);
  };

} // namespace gui
