//  Copyright (c) 2024 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gui/Types.hpp>

namespace gui
{
  class Rect
  {
    Vec2i pos_;
    Vec2i size_;
  public:
    Rect(const Vec2i& pos = {0, 0}, const Vec2i& size = {0, 0})
      : pos_{pos}, size_{size}
    {
    }

    const Vec2i& getPosition() const { return pos_; }
    void setPosition(const Vec2i& pos) { pos_ = pos; }

    const Vec2i& getSize() const { return size_; }
    void setSize(const Vec2i& size) { size_ = size; }
  };

} // namespace gui
