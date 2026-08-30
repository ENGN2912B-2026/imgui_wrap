//  Copyright (c) 2024 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gui/Types.hpp>

#include <string>
#include <vector>
#include <memory>

// Forward declaration of backend::Backend class
namespace backend { class Backend; }

namespace gui
{
  // forward declaration
  class Frame;
  class Sizer;

  // A top-level window
  // ------------------
  // The window instance will register itself with the global application
  // instance if one exists. This causes that the window is displayed when
  // the application is `run` function is called. For this reason, it is
  // recommended to create the application before creating any window.
  class Window
  {
    std::string title_;
    Vec2i size_;
    std::vector<Frame*> frames_;
    std::unique_ptr<Sizer> sizer_;
    std::unique_ptr<backend::Backend> backend_;
  public:
    Window(
      const std::string& title = "Window",
      const Vec2i& size = {640, 480});
    virtual ~Window();

    void init();
    void deinit();

    const std::string& getTitle() const { return title_; }
    void setTitle(const std::string& title) { title_ = title; }

    bool renderBegin();
    void renderEnd();
    virtual void render();

    void addFrame(Frame* frame);
    void removeFrame(Frame* frame);
    std::vector<Frame*>& getFrames();

    backend::Backend* getBackendPtr() { return backend_.get(); }
  };

} // namespace gui
