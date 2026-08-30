//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#include <gui/Window.hpp>
#include <gui/Frame.hpp>
#include <gui/VerticalSizer.hpp>
#include <backend/Backend.hpp>

#include <imgui.h>

#ifdef USE_IMPLOT
# include <implot.h>
#endif

#include <algorithm> // For std::remove

namespace gui
{
  using DefaultSizer = VerticalSizer;

  Window::Window(const std::string& title, const Vec2i& size) :
    title_{title},
    size_{size},
    backend_{nullptr},
    sizer_{ std::make_unique<DefaultSizer>() },
    frames_{}
  {

  }

  Window::~Window()
  {

  }

  void Window::init()
  {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
#ifdef USE_IMPLOT
    ImPlot::CreateContext();
#endif

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
#ifdef IMGUI_HAS_DOCK
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif
#ifdef IMGUI_HAS_VIEWPORT
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
#endif
#if IMGUI_VERSION_NUM >= 19004
    //io.ConfigDebugIsDebuggerPresent = ImOsIsDebuggerPresent();
#endif

    // Setup Dear ImGui style
    ImGui::StyleColorsLight();

    // Setup backend
    backend_ = backend::Backend::create();
    backend_->DpiAware = true;
    backend_->SrgbFramebuffer = false;
    backend_->Vsync = true;
    backend_->ClearColor = ImVec4(0.120f, 0.120f, 0.120f, 1.000f);
    backend_->InitCreateWindow(title_.c_str(), size_.to<float>());
    backend_->InitBackends();
  }

  void Window::deinit()
  {
    // Shutdown backend
    backend_->ShutdownBackends();
    backend_->ShutdownCloseWindow();
    backend_.reset();
    // Delete context
#ifdef USE_IMPLOT
    ImPlot::DestroyContext();
#endif
    ImGui::DestroyContext();
  }

  bool Window::renderBegin()
  {
    if (!backend_->NewFrame())
    {
      return false;
    }

    //Update size in case window was resized
    size_ = math::make<Vec2i>(ImGui::GetIO().DisplaySize);

    ImGui::NewFrame();
    return true;
  }

  void Window::renderEnd()
  {
    ImGui::Render();
    backend_->Render();
  }

  void Window::render()
  {
    // Render frames
    if (sizer_)
    {
      sizer_->setSize(size_);
      sizer_->setPosition({0, 0});
      sizer_->draw();
    }
    else
    {
      for (auto frame : frames_)
      {
        frame->renderBegin();
        frame->render();
        frame->renderEnd();
      }
    }
  }

  void Window::addFrame(Frame* frame)
  {
    frames_.push_back(frame);
    if (sizer_)
    {
      sizer_->addChild(frame);
    }
  }

  void Window::removeFrame(Frame* frame)
  {
    frames_.erase(
      std::remove(frames_.begin(), frames_.end(), frame),
      frames_.end());
    if (sizer_)
    {
      sizer_->removeChild(frame);
    }
  }

  std::vector<Frame*>& Window::getFrames()
  {
    return frames_;
  }

} // namespace gui
