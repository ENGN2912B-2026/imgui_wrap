//  Copyright (c) 2025 Daniel Moreno. All rights reserved.
//

#include <gui/Image.hpp>

#include <stdexcept>
#include <limits>

namespace gui
{
  Image::Image() : gui::ChildFrame{}
  {
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

    // Respect the image aspect ratio
    ImVec2 widgetSize = ImGui::GetContentRegionAvail();
    ImVec2 displaySize = widgetSize;
    float aspectRatio = static_cast<float>(width) / height;
    if (displaySize.x / displaySize.y > aspectRatio)
    {
      displaySize.x = displaySize.y * aspectRatio;
    }
    else
    {
      displaySize.y = displaySize.x / aspectRatio;
    }

    // Center the image
    ImVec2 displayPos{
      (widgetSize.x - displaySize.x) * 0.5f + ImGui::GetCursorPosX(),
      (widgetSize.y - displaySize.y) * 0.5f + ImGui::GetCursorPosY(),
    };

    // Set the display position
    ImGui::SetCursorPos(displayPos);
    const ImVec2 screenDisplayPos{ ImGui::GetCursorScreenPos() };

    // Apply view setting
    const float s1{ scale_ };
    const Vec2f t1{ translation_ };
    const ImVec2 uv0{ (t1 + Vec2f{ 0.0f, 0.0f } * s1).to<float>() };
    const ImVec2 uv1{ (t1 + Vec2f{ 1.0f, 1.0f } * s1).to<float>() };

    // Display the texture
    ImGui::Image(textureId, displaySize, uv0, uv1);

    // Handle mouse events
    if(ImGui::IsItemHovered())
    {
      auto& io = ImGui::GetIO();

      // update mouse position
      const Vec2f screenPoint{
        (io.MousePos.x - screenDisplayPos.x) / displaySize.x,
        (io.MousePos.y - screenDisplayPos.y) / displaySize.y,
      };
      const Vec2f imagePoint{ t1 + screenPoint * s1 };
      mousePosition_.x = imagePoint.x * width;
      mousePosition_.y = imagePoint.y * height;

      // update image zoom when mouse wheel is scrolled
      if(io.MouseWheel != 0.0f)
      {
        // compute the new scale
        constexpr float maxScale{ 1.0f };
        const float minScale{ 1.0f / std::max(width, height) };
        const float scaleFactor{ io.MouseWheel < 0 ? 1.1f : 0.9f };
        const float s2{ std::min(maxScale, std::max(minScale, scaleFactor * s1)) };

        // make the image position below the mouse to stay at a fixed point
        // before and after zooming, compute the new translation to keep the
        // fixed point where it was:
        //
        //    screenPoint <-> imagePoint
        //    imagePoint.x = uv0'.x + screenPoint.x * (uv1'.x - uv0'.x)
        //    imagePoint.y = uv0'.y + screenPoint.y * (uv1'.y - uv0'.y)
        //    uv0' = (0,0)*s2 + t2 = t2
        //    uv1' = (1,1)*s2 + t2
        //    uv1'- uv0' = (1,1)*s2 + t2 -t2 = (s2,s2)
        //    -> imagePoint = t2 + screenPoint * s2
        //    -> t2 = imagePoint - screenPoint * s2
        //
        Vec2f t2{ imagePoint - screenPoint * s2 };
        if (t2.x < 0.0f) { t2.x = 0.0f; }
        if (t2.y < 0.0f) { t2.y = 0.0f; }
        if (t2.x > 1.0f - s2) { t2.x = 1.0f - s2; }
        if (t2.y > 1.0f - s2) { t2.y = 1.0f - s2; }

        // update scale and translation
        scale_ = s2;
        translation_ = t2;
      }
      else if (io.MouseDoubleClicked[0])
      { // reset view on double click
        resetView();
      }
      else if(io.MouseDown[0])
      { // pan the image if mouse is moved while pressing the left button

        const Vec2f screenDelta{
          io.MouseDelta.x / displaySize.x,
          io.MouseDelta.y / displaySize.y,
        };
        const Vec2f imageDelta{ screenDelta * s1 };

        Vec2f t2{ t1 - imageDelta };
        if (t2.x < 0.0f) { t2.x = 0.0f; }
        if (t2.y < 0.0f) { t2.y = 0.0f; }
        if (t2.x > 1.0f - s1) { t2.x = 1.0f - s1; }
        if (t2.y > 1.0f - s1) { t2.y = 1.0f - s1; }

        // update translation
        translation_ = t2;
      }
    }
    else
    { // make mouse position invalid if the image is not hovered
      mousePosition_.x = std::numeric_limits<float>::quiet_NaN();
      mousePosition_.y = std::numeric_limits<float>::quiet_NaN();
    }
  }

  void Image::resetView()
  {
    scale_ = 1.0f;
    translation_ = { 0.0f, 0.0f };
    mousePosition_.x = std::numeric_limits<float>::quiet_NaN();
    mousePosition_.y = std::numeric_limits<float>::quiet_NaN();
  }
}
