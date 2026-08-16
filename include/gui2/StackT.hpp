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

  // A container that stacks its child items either vertically or horizontally.
  // The items are displayed in the order they are added to the stack, and with
  // a spacing between them defined by the runtime. Their sizes are determined
  // by their content, and the stack will expand to fit them.
  template <Orientation orientation>
  class StackT
  {
  public:
    // The orientation of the stack (vertical or horizontal).
    constexpr static Orientation kOrientation = orientation;

    // Default constructor
    StackT() = default;

    // Constructor that takes a variable number of items.
    // Each item must be constructible into a Widget. They are stored in the
    // order they are passed to the constructor, and will be displayed in that
    // order.
    template<StackItem... T>
    StackT(T&&... items)
    {
      (items_.emplace_back(std::forward<T>(items)), ...);
    }

    // Returns a const reference to the vector of items in the stack.
    const std::vector<Widget>& getItems() const { return items_; }
    // Returns a non-const reference to the vector of items in the stack.
    std::vector<Widget>& getItems() { return items_; }

    // Displays the stack and its items using the given runtime and rectangle.
    // The stack will display its items in the order they were added, and with
    // a spacing between them defined by the runtime. The sizes of the items
    // are determined by their content, and the stack will expand to fit them.
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
    validate(orientation);

    Rect actualRect{ Rect::empty() };
    Rect itemRect{ rect };
    IfVertical<orientation>([&]{itemRect.unsetHeight();});
    IfHorizontal<orientation>([&]{itemRect.unsetWidth();});

    const auto itemSpacing = rt.getItemSpacing();
    for (auto& item : items_)
    {
      // Display the item and get its actual rectangle
      const Rect itemActualRect = item.display(rt, itemRect);
      // Update our actual rectangle based on the actual rectangles of the items
      actualRect.unite(itemActualRect);
      // Update the position of the next item rectangle based on the orientation
      IfVertical<orientation>([&]{
        if (itemActualRect.size.y > 0)
        { // Only add the spacing if the item has a valid height
          itemRect.origin.y += itemActualRect.size.y + itemSpacing.y;
        }
      });
      IfHorizontal<orientation>([&]{
        if (itemActualRect.size.x > 0)
        { // Only add the spacing if the item has a valid width
          itemRect.origin.x += itemActualRect.size.x + itemSpacing.x;
        }
      });
    }
    // Return the actual rectangle that encompasses all the items in the stack
    return actualRect;
  }
}
