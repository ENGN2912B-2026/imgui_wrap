//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <backend/Backend.hpp>

namespace backend
{
  class Backend_Null : public Backend
  {
  public:
    ~Backend_Null() = default;

    static std::unique_ptr<Backend_Null> create();

    ImU64 LastTime = 0;
    float Timeout = 1.0f; // 1 second

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

} // namespace backend
