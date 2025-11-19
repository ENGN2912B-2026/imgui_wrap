//  Copyright (c) 2025 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gl/gl.h>
#include <gui/gui.hpp>

namespace gui
{
  // GUI Image class for displaying an image with zoom and pan capabilities
  class Image : public gui::ChildFrame
  {
  public:
    using Vec2f = math::Vec2f;
    using Vec2i = math::Vec2i;

    Image();
    ~Image();

    void render() override;

    void resetView();

    bool hasTexture() const { return textureId() > 0U; }

    float zoomValue() const { return 1.0f / scale_; }

    float scale() const { return scale_; }

    Vec2f translation() const { return translation_; }

    Vec2f mousePosition() const { return mousePosition_; }

  protected:
    virtual ImTextureID textureId() const { return 0U; };
    virtual Vec2i textureSize(ImTextureID textureId) const { return {0, 0}; };
    virtual void updateTexture() {}

  private:
    // private data members
    float scale_;
    Vec2f translation_;
    Vec2f mousePosition_;
  };
}