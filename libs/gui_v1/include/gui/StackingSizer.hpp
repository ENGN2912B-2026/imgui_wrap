//  Copyright (c) 2024 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gui/Sizer.hpp>

namespace gui
{
  class StackingSizer : public Sizer
  {
  public:
    enum class Direction { Vertical, Horizontal };

    StackingSizer(Direction direction);

    void addChild(Widget* child, int weight = 1);

    void addWithFixedSize(Widget* child, int size);

    void draw() override;

    void apply();

  private:
    Direction direction_;
    std::vector<int> weights_;
  };

} // namespace gui
