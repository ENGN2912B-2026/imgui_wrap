//  Copyright (c) 2024 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gui/Widget.hpp>

#include <string>

namespace gui
{
  class Frame : public Widget
  {
    int flags_;
  public:
    Frame(
      const std::string& name = {},
      const Vec2i& pos = {0, 0},
      const Vec2i& size = {0, 0},
      int flags = -1);
    virtual ~Frame();

    bool renderBegin() override;
    void renderEnd() override;
    virtual void render() override;
  };

} // namespace gui
