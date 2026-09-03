//  Copyright (c) 2025-2026 Daniel Moreno. All rights reserved.
//

#include <gui/Image.hpp>

#include <stdexcept>
#include <limits>

namespace gui
{
  Image::Image(bool enablePanZoom)
  : gui::ChildFrame{}
  , zoomState_{}
  {
    zoomState_.zoomPanEnabled = enablePanZoom;
    resetView();
  }

  Image::~Image()
  {

  }

  void Image::render()
  {
    updateTexture();
    const ImTextureID textureId{ this->textureId() };
    if (!textureId)
    { // No image has been set
      return;
    }

    // Get texture size
    const Vec2i textureSize{ this->textureSize(textureId) };
    const int width{ textureSize.x };
    const int height{ textureSize.y };
    if (width <= 0 || height <= 0)
    {
      throw std::runtime_error{ "Invalid texture size" };
    }
    zoomState_.textureSize = ImVec2(width, height);

    // Display the image with zoom and pan capabilities
    ImVec2 displaySize = ImGui::GetContentRegionAvail();
    ImGuiImage::Zoomable(textureId, displaySize, &zoomState_);
  }

  void Image::resetView()
  {
    zoomState_.zoomLevel = 1.0f;
    zoomState_.panOffset = { 0.0f, 0.0f };
    zoomState_.mousePosition.x = std::numeric_limits<float>::quiet_NaN();
    zoomState_.mousePosition.y = std::numeric_limits<float>::quiet_NaN();
  }
}
