//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <imgui.h>

#include <memory>

namespace gui
{
  class Backend
  {
  public:
    virtual ~Backend() = default;

    static std::unique_ptr<Backend> create();

    bool    DpiAware = true;                            // [In]  InitCreateWindow()
    bool    SrgbFramebuffer = false;                    // [In]  InitCreateWindow()
    ImVec4  ClearColor = { 0.f, 0.f, 0.f, 1.f };        // [In]  Render()
    float   DpiScale = 1.0f;                            // [Out] InitCreateWindow() / NewFrame()
    bool    Vsync = true;                               // [Out] Render()

    virtual bool InitCreateWindow(const char* window_title, ImVec2 window_size) = 0;
    virtual void InitBackends() = 0;
    virtual bool NewFrame() = 0;
    virtual void Render() = 0;
    virtual void ShutdownCloseWindow() = 0;
    virtual void ShutdownBackends() = 0;
    virtual bool CaptureFramebuffer(
      ImGuiViewport* viewport,
      int x, int y, int w, int h,
      unsigned int* pixels_rgba, void* user_data) = 0;

    virtual void SetWindowTitle(const char* title) = 0;
    virtual void SetWindowSize(const ImVec2& size) = 0;
  };

} // namespace gui
