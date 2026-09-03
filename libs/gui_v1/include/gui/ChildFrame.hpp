//  Copyright (c) 2024 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gui/Widget.hpp>

#include <string>

namespace gui
{
  class ChildFrame : public Widget
  {
    int childFlags_;
    int windowFlags_;
  public:
    ChildFrame(
      const std::string& name = {},
      const Vec2i& pos = {0, 0},
      const Vec2i& size = {0, 0},
      int childFlags = -1,
      int windowFlags = -1);
    virtual ~ChildFrame();

    bool renderBegin() override;
    void renderEnd() override;
    virtual void render() override;
  };

} // namespace gui
