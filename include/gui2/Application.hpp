//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Window.hpp>

#include <string>
#include <atomic>

namespace gui2
{
  class Application
  {
    Window window_;
    std::atomic<bool> running_;
  public:
    Application(
      const std::string& windowTitle = "Window",
      const Vec2i& windowSize = {640, 480});

    virtual ~Application();

    Window& getWindow() { return window_; }
    const Window& getWindow() const { return window_; }

    bool isRunning() const { return running_; }

    void run();
    void quit() { running_ = false; }
  };
}