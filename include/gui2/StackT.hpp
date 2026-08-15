//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>

#include <vector>

namespace gui2
{
  // Concept of things that can be items in a StackT:
  // - The type is constructible into a Widget.
  template<typename T>
  concept StackItem =
    std::constructible_from<Widget, T&&>;

  template <Orientation orientation>
  class StackT
  {
  public:
    constexpr static Orientation kOrientation = orientation;

    StackT() = default;
    StackT(StackT const&) = default;
    StackT(StackT&&) = default;
    StackT& operator=(StackT const&) = default;
    StackT& operator=(StackT&&) = default;

    template<StackItem... T>
    StackT(T&&... items)
    {
      (items_.emplace_back(std::forward<T>(items)), ...);
    }

    const std::vector<Widget>& getItems() const { return items_; }
    std::vector<Widget>& getItems() { return items_; }

    Rect display(const Runtime& rt, const Rect& rect);

  private:
    std::vector<Widget> items_;
  };

} // namespace gui

// Template implementations ---------------------------------------------------
namespace gui2
{
  template<Orientation orientation>
  Rect StackT<orientation>::display(const Runtime& rt, const Rect& rect)
  {
    Rect actualRect{ Rect::empty() };
    Rect itemRect{ rect };
    const auto itemSpacing = rt.getItemSpacing();
    for (auto& item : items_)
    {
      // Display the item and get its actual rectangle
      const Rect itemActualRect = item.display(rt, itemRect);
      // Update our actual rectangle based on the actual rectangles of the items
      actualRect.unite(itemActualRect);
      // Update the position of the next item rectangle based on the orientation
      if constexpr (orientation == Orientation::Vertical)
      { // VStack
        itemRect.origin.y += itemActualRect.size.y + itemSpacing.y;
      }
      else if constexpr (orientation == Orientation::Horizontal)
      { // HStack
        itemRect.origin.x += itemActualRect.size.x + itemSpacing.x;
      }
      else
      { // Invalid orientation
        static_assert(false, "Invalid layout orientation");
      }
    }
    return actualRect;
  }
}
