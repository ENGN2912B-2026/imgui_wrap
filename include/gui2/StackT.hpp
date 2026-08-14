//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>

#include <vector>

namespace gui2
{
  template <Orientation orientation>
  class StackT
  {
  public:
    constexpr static Orientation kOrientation = orientation;

    StackT() = default;

    template<typename... T>
    requires (std::constructible_from<Widget, T&&> && ...)
    StackT(T&&... items)
    {
      (items_.emplace_back(std::forward<T>(items)), ...);
    }

    const std::vector<Widget>& getItems() const { return items_; }
    std::vector<Widget>& getItems() { return items_; }

  private:
    std::vector<Widget> items_;
  };

} // namespace gui
