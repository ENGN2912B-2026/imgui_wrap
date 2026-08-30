//  Copyright (c) 2025-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gui/gui.hpp>
#include <imgui_zoomable_image.h>

namespace gui
{
  // GUI Image class for displaying an image with zoom and pan capabilities
  class Image : public gui::ChildFrame
  {
  public:
    using Vec2f = math::Vec2f;
    using Vec2i = math::Vec2i;

    Image(bool enablePanZoom = false);
    virtual ~Image();

    void render() override;

    void resetView();

    bool hasTexture() const { return textureId() > 0U; }

    float zoomValue() const { return zoomState_.zoomLevel; }

    float scale() const { return 1.0f / zoomState_.zoomLevel; }

    Vec2f translation() const
    {
      return { zoomState_.panOffset.x, zoomState_.panOffset.y };
    }

    Vec2f mousePosition() const
    {
      return { zoomState_.mousePosition.x, zoomState_.mousePosition.y };
    }

    bool isPanZoomEnabled() const { return zoomState_.zoomPanEnabled; }

    void setPanZoomEnabled(bool enabled) { zoomState_.zoomPanEnabled = enabled; }

  protected:
    virtual ImTextureID textureId() const { return 0U; };
    virtual Vec2i textureSize(ImTextureID textureId) const { return {0, 0}; };
    virtual void updateTexture() {}

  private:
    // private data members
    ImGuiImage::State zoomState_;
  };
}