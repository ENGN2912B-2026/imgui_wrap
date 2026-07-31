//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//
#include "Backend_Null.hpp"

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

namespace gui
{
  std::unique_ptr<Backend_Null> Backend_Null::create()
  {
    return std::make_unique<Backend_Null>();
  }

  bool Backend_Null::InitCreateWindow(
    const char* window_title, ImVec2 window_size)
  {
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = window_size;
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset | ImGuiBackendFlags_HasMouseCursors;
#if IMGUI_VERSION_NUM < 18603
    for (int n = 0; n < ImGuiKey_COUNT; n++)
        io.KeyMap[n] = n;
#endif

    return true;
  }

  void Backend_Null::InitBackends()
  {

  }

  bool Backend_Null::NewFrame()
  {
    ImGuiIO& io = ImGui::GetIO();

    //unsigned char* pixels = NULL;
    //int width = 0;
    //int height = 0;
    //io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);

    uint64_t time = GetTimeInMicroseconds_();
    if (LastTime == 0)
    {
        LastTime = time;
    }
    io.DeltaTime = static_cast<float>(time - LastTime) / 1000000.0f; // Convert to seconds
    if (io.DeltaTime <= 0.0f)
    {
        io.DeltaTime = 0.000001f;
    }
    LastTime = time;

    // Decrement timeout
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
    memset(pixels, 0, static_cast<size_t>(w * h) * sizeof(unsigned int));
    return false;
  }

  void Backend_Null::Render()
  {
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
    // nothing to do
  }

  void Backend_Null::SetWindowTitle(const char* title)
  {
    // nothing to do
  }

  void Backend_Null::SetWindowSize(const ImVec2& size)
  {
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = size;
  }

} // namespace gui
