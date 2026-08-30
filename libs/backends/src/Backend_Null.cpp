//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//
#include <backend/Backend_Null.hpp>

#ifdef USE_ROBOTO_WEBFONT
# include <roboto_regular_webfont_ttf.hpp>
#endif

#include <chrono>

namespace
{
  static uint64_t GetTimeInMicroseconds_()
  {
    using namespace std;
    chrono::microseconds ms = chrono::duration_cast<chrono::microseconds>(
      chrono::high_resolution_clock::now().time_since_epoch());
    return static_cast<uint64_t>(ms.count());
  }

  void ImGui_ImplNullRender_UpdateTexture(ImTextureData* tex)
  {
    if (tex->Status == ImTextureStatus_WantCreate
      || tex->Status == ImTextureStatus_WantDestroy)
    {
      tex->SetStatus(ImTextureStatus_OK);
    }
    if (tex->Status == ImTextureStatus_WantDestroy)
    {
      tex->SetTexID(ImTextureID_Invalid);
      tex->SetStatus(ImTextureStatus_Destroyed);
    }
  }
}

namespace backend
{
  std::unique_ptr<Backend_Null> Backend_Null::create()
  {
    return std::make_unique<Backend_Null>();
  }

  bool Backend_Null::InitCreateWindow(
    const char* window_title, ImVec2 window_size)
  {
    DpiScale = 1.0f;

    // Adjust scale
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig font_config;
    font_config.OversampleH = 1;
    font_config.OversampleV = 1;
    font_config.PixelSnapH = true;
    font_config.SizePixels = 17.0f * DpiScale;
    font_config.GlyphOffset.y = 1.0f * DpiScale;
    io.Fonts->Clear();

#ifdef USE_ROBOTO_WEBFONT
    // Load Roboto webfont
    font_config.FontDataOwnedByAtlas = false;
    io.Fonts->AddFontFromMemoryTTF(
      roboto_regular_webfont_ttf_data,
      roboto_regular_webfont_ttf_size,
      font_config.SizePixels,
      &font_config);
#else
    // Load default font
    io.Fonts->AddFontDefault(&font_config);
#endif

    ImGui::GetStyle().ScaleAllSizes(DpiScale);

    // Update the display size (This is required!)
    SetWindowSize(window_size);

    return true;
  }

  void Backend_Null::InitBackends()
  {
    // Original implementation of `InitBackends()` was just this:
    //
    //     IMGUI_IMPL_API bool ImGui_ImplNull_Init()
    //     {
    //       ImGui_ImplNullPlatform_Init();
    //       ImGui_ImplNullRender_Init();
    //       return true;
    //     }
    //
    // We just copy the body of those functions here.

    //IMGUI_IMPL_API bool ImGui_ImplNullPlatform_Init()
    {
      ImGuiIO& io = ImGui::GetIO();
      io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
      //return true;
    }

    //IMGUI_IMPL_API bool ImGui_ImplNullRender_Init()
    {
      ImGuiIO& io = ImGui::GetIO();
      io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
      io.BackendFlags |= ImGuiBackendFlags_RendererHasTextures;
      //return true;
    }
  }

  bool Backend_Null::NewFrame()
  {
    // Original implementation of `NewFrame()` was just this:
    //
    //    IMGUI_IMPL_API void ImGui_ImplNull_NewFrame()
    //    {
    //      ImGui_ImplNullPlatform_NewFrame();
    //      ImGui_ImplNullRender_NewFrame();
    //    }
    //
    // These are their implementations:
    //
    //     IMGUI_IMPL_API void ImGui_ImplNullPlatform_NewFrame()
    //     {
    //       ImGuiIO& io = ImGui::GetIO();
    //       io.DisplaySize = ImVec2(1920, 1080);
    //       io.DeltaTime = 1.0f / 60.0f;
    //     }
    //     IMGUI_IMPL_API void ImGui_ImplNullRender_NewFrame()
    //     {
    //     }
    //
    // We see that first one is only updating io.DeltaTime by a fix amount of
    // 1/60th of a second, and the second is empty. Additionally, the first one
    // is also updating io.DisplaySize to a fixed value of 1920x1080, which does
    // not seem necessary for our code since we are already setting the display
    // size in `SetWindowSize()`.
    //
    // Here we do something equivalent but instead of using a fixed value for
    // io.DeltaTime, we use the actual time elapsed since the last frame. This
    // is more accurate and allows us to timeout if the time elapsed is too
    // long.
    //

    // A helper function to convert microseconds to seconds
    const auto toSeconds = [](uint64_t microseconds) -> float
    {
      return static_cast<float>(microseconds) / 1000000.0f;
    };

    // A small value to use when we do not have a valid delta time.
    constexpr uint64_t kMinDeltaTime = 1; // 1 microsecond

    // Get the current time in microseconds
    const uint64_t time = GetTimeInMicroseconds_();

    // Compute the elapsed time since the last frame
    const uint64_t delta_time = LastTime == 0 ? kMinDeltaTime : time - LastTime;

    // Update ImGui's delta time
    ImGuiIO& io = ImGui::GetIO();
    io.DeltaTime = toSeconds(delta_time);

    // Update our last time to the current time
    LastTime = time;

    // Update the timeout and check if we should exit
    Timeout -= io.DeltaTime;
    if (Timeout < 0)
    {
      printf("Backend_Null::Timeout\n");
      return false;
    }

    return true;
  }

  bool Backend_Null::CaptureFramebuffer(
    ImGuiViewport* viewport,
    int x, int y, int w, int h,
    unsigned int* pixels, void* user_data)
  {
    // We cannot capture the framebuffer since we are not actually rendering
    // anything, so we just fill the pixels with black and return false to
    // indicate that we did not capture anything.
    memset(pixels, 0, static_cast<size_t>(w * h) * sizeof(unsigned int));
    return false;
  }

  void Backend_Null::Render()
  {
    // This function is a trimmed down version of what GLFW_GL3 backend does
    // in its Render() function. The original null-backend Render() function
    // has only the first part where textures are updated. We also include the
    // second part where the command lists are processed.

    ImDrawData* draw_data = ImGui::GetDrawData();

    // Update textures
    if (draw_data->Textures != nullptr)
    {
      for (ImTextureData* tex : *draw_data->Textures)
      {
        if (tex->Status != ImTextureStatus_OK)
        {
          ImGui_ImplNullRender_UpdateTexture(tex);
        }
      }
    }

    // Render command lists
    for (const ImDrawList* draw_list : draw_data->CmdLists)
    {
      for (int cmd_i = 0; cmd_i < draw_list->CmdBuffer.Size; cmd_i++)
      {
        const ImDrawCmd* pcmd = &draw_list->CmdBuffer[cmd_i];
        if (pcmd->UserCallback != nullptr)
        { // User callback, registered via ImDrawList::AddCallback()
          pcmd->UserCallback(draw_list, pcmd);
        }
        else
        { // Render triangles
          // (In this backend, we don't actually render anything)
        }
      }
    }
  }

  void Backend_Null::ShutdownCloseWindow()
  {
    // nothing to do
  }

  void Backend_Null::ShutdownBackends()
  {
    // Original implementation of `ShutdownBackends()` was just this:
    //
    //     IMGUI_IMPL_API void ImGui_ImplNull_Shutdown()
    //     {
    //       ImGui_ImplNullRender_Shutdown();
    //       ImGui_ImplNullPlatform_Shutdown();
    //     }
    //
    // Note that order is reversed from the initialization order.
    // We just copy the body of those functions here.

    //IMGUI_IMPL_API void ImGui_ImplNullRender_Shutdown()
    {
      ImGuiIO& io = ImGui::GetIO();
      io.BackendFlags &= ~ImGuiBackendFlags_RendererHasVtxOffset;
      io.BackendFlags &= ~ImGuiBackendFlags_RendererHasTextures;
    }

    //IMGUI_IMPL_API void ImGui_ImplNullPlatform_Shutdown()
    {
      ImGuiIO& io = ImGui::GetIO();
      io.BackendFlags &= ~ImGuiBackendFlags_HasMouseCursors;
    }
  }

  void Backend_Null::SetWindowTitle(const char* title)
  {
    // nothing to do
  }

  void Backend_Null::SetWindowSize(const ImVec2& size)
  {
    // We do not have a real window, but we can still update ImGui's display
    // size to match the requested size.
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = size;
  }

} // namespace backend
