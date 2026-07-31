//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Application.hpp>
#include <gui2/Runtime.hpp>

namespace gui2
{
  Application::Application(
    const std::string& windowTitle,
    const Vec2i& windowSize)
    : window_{windowTitle, windowSize}
    , running_{false}
  {
  }

  Application::~Application()
  {
  }

  void Application::run()
  {
    auto& rt = Runtime::getInstance();

    // Initialize the runtime and create a desktop window
    rt.init(window_.getTitle(), window_.getSize());

    // Event loop
    running_ = true;
    std::string lastTitle = window_.getTitle();
    Vec2i lastSize = window_.getSize();
    while (running_ && rt.frameBegin())
    {
      //TODO: I need to sync the window size with the runtime window size, in case the user resized the window
      // in a thread-safe way.

      // Synchronize the window size three-way between the application window,
      // the runtime window, and the backend window.
      if (lastSize != window_.getSize())
      { // User has set a new size for the application window, so we need to
        // update the runtime window size.
        lastSize = window_.getSize();
        rt.setWindowSize(lastSize);
      }
      else
      { // Check if the runtime window size has changed (e.g., due to user
        // resizing the backend window)
        const Vec2i runtimeSize = rt.getWindowSize();
        if (lastSize != runtimeSize)
        {
          lastSize = runtimeSize;
          window_.setSize(lastSize);
        }
      }

      if (lastTitle != window_.getTitle())
      {
        lastTitle = window_.getTitle();
        rt.setWindowTitle(lastTitle);
      }

      window_.displayContent(rt);

      rt.frameEnd();
    }

    // Deinitialize the runtime
    rt.deinit();
  }
}