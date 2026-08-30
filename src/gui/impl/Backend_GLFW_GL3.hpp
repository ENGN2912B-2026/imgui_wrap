//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include "Backend.hpp"

// forward declaration
struct GLFWwindow;

namespace gui
{
  class Backend_GLFW_GL3 : public Backend
  {
  public:
    ~Backend_GLFW_GL3() = default;

    static std::unique_ptr<Backend_GLFW_GL3> create();

    GLFWwindow* window;
    const char* glsl_version;

    bool InitCreateWindow(const char* window_title, ImVec2 window_size) override;
    void InitBackends() override;
    bool NewFrame() override;
    void Render() override;
    void ShutdownCloseWindow() override;
    void ShutdownBackends() override;
    bool CaptureFramebuffer(
      ImGuiViewport* viewport,
      int x, int y, int w, int h,
      unsigned int* pixels_rgba, void* user_data) override;

    void SetWindowTitle(const char* title) override;
    void SetWindowSize(const ImVec2& size) override;
  };

} // namespace gui
