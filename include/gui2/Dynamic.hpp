//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Runtime.hpp>

#include <functional>

namespace gui2
{
  class Dynamic
  {
  public:
    Dynamic(std::function<Widget()> updateFunc = {}) : updateFunc_(updateFunc) {}

    void display(const Runtime& runtime, const OptionalSize& displaySize = {})
    {
      if (updateFunc_)
      {
        updateFunc_().display(runtime, displaySize);
      }
    }

  private:
    std::function<Widget()> updateFunc_;
  };

} // namespace gui
