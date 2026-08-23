//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>

#include <vector>
#include <cassert>

namespace gui2
{
  // Concept of things that can be items in a Layout:
  // - The type is constructible into a Widget.
  template<typename T>
  concept LayoutItem =
    std::constructible_from<Widget, T&&>;

  // The layout orientation determines the direction in which the items are
  // arranged.
  enum class Orientation
  {
    // The items are arranged vertically, one below the other.
    Vertical,
    // The items are arranged horizontally, side by side.
    Horizontal,
  };

  // The layout mode determines how the items are arranged and sized within the
  // available space of the layout.
  enum class LayoutMode
  {
    // The items are arranged one after another in a single row or column,
    // depending on the orientation, without trying to fill all available space.
    Stack,
    // The available space is divided among the items in the layout, and each
    // item is given a portion of the space based on its weight or size. Items
    // are arranged in a single row or column, depending on the orientation.
    Box,
  };

  // A fixed-size item that can be added to a Layout. The size is specified in
  // pixels, and the content is a Widget that will be displayed within the
  // fixed-size area.
  //
  // The size must be enforced by a parent element supporting this feature, such
  // as a Layout in Box mode. If the parent does not support fixed-size items,
  // the size will be ignored and the content will be displayed with its natural
  // size.
  struct Fixed
  {
    // The fixed size of the item in pixels.
    size_t size;
    // The content of the item, which is a Widget that will be displayed within
    // the fixed-size area.
    Widget content;

    // \brief Constructs a Fixed item with the given size and content.
    // \param[in] _size     The fixed size of the item in pixels.
    // \param[in] _content  The content of the item.
    Fixed(size_t _size, Widget _content = {})
      : size{std::move(_size)}
      , content{std::move(_content)}
    { }

    // Display its content.
    Rect display(const Runtime& rt, const Rect& rect)
    {
      return content.display(rt, rect);
    }
  };

  // A stretchable item that can be added to a Layout. The weight determines
  // how much of the available space the item will take relative to other
  // stretchable items in the same layout.
  //
  // The weight value is unitless and it can only be interpreted relative to
  // the weights of other stretchable items in the same layout. For example, if
  // there are two stretchable items with weights 1 and 2, the first item will
  // take 1/3 of the available space, and the second item will take 2/3 of the
  // available space.
  //
  // The weight must be enforced by a parent element supporting this feature,
  // such as a Layout in Box mode. If the parent does not support stretchable
  // items, the weight will be ignored and the content will be displayed with
  // its natural size.
  struct Stretch
  {
    // The weight of the item, which determines how much of the available space
    // the item will take relative to other stretchable items in the same layout.
    size_t weight;
    // The content of the item, which is a Widget that will be displayed within
    // the stretchable area.
    Widget content;

    // \brief Constructs a Stretch item with the given weight and content.
    // \param[in] _weight   The weight of the item.
    // \param[in] _content  The content of the item.
    Stretch(size_t _weight, Widget _content = {})
      : weight{std::move(_weight)}
      , content{std::move(_content)}
    { }

    // Display its content.
    Rect display(const Runtime& rt, const Rect& rect)
    {
      return content.display(rt, rect);
    }
  };

  // A container that arranges its child items either vertically or
  // horizontally, and either in a stack or box layout mode. The items are
  // displayed in the order they are added to the layout, and with a spacing
  // between them defined by the runtime.
  //
  // Stack mode
  // ----------
  // In stack mode, the items are arranged one after another in a single row or
  // column, depending on the orientation, without trying to fill all available
  // space. The size of each item is determined by its content, and the layout
  // will expand to fit them.
  //
  // Box mode
  // --------
  // In box mode, the available space is divided among the items in the layout,
  // and each item is given a portion of the space based on its weight or size.
  // Items are arranged in a single row or column, depending on the orientation.
  //
  template<Orientation O, LayoutMode M>
  class Layout
  {
  public:
    // The orientation of the stack (vertical or horizontal).
    constexpr static Orientation kOrientation = O;
    // The layout mode (stack or box).
    constexpr static LayoutMode kLayoutMode = M;

    // Default constructor
    Layout() = default;

    // Constructor that takes a variable number of items.
    // Each item must be constructible into a Widget. They are stored in the
    // order they are passed to the constructor, and will be displayed in that
    // order.
    template<LayoutItem... T>
    Layout(T&&... items)
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

    // Validate that the given orientation is a valid value of the Orientation
    // enum class.
    static_assert(O == Orientation::Vertical || O == Orientation::Horizontal,
      "Invalid orientation");
    // Validate that the given layout mode is a valid value of the LayoutMode
    // enum class.
    static_assert(M == LayoutMode::Stack || M == LayoutMode::Box,
      "Invalid layout mode");

    std::vector<Rect> computeItemLayouts_(
      const Rect& rect, const Vec2i& itemSpacing);
  };

  using HStack = Layout<Orientation::Horizontal, LayoutMode::Stack>;
  using VStack = Layout<Orientation::Vertical,   LayoutMode::Stack>;
  using HBox   = Layout<Orientation::Horizontal, LayoutMode::Box>;
  using VBox   = Layout<Orientation::Vertical,   LayoutMode::Box>;

} // namespace gui

// Iimplementation ------------------------------------------------------------
namespace gui2
{
  // Invokes a callable if two values are equal at compile time.
  template<auto V1, auto V2>
  constexpr void IfEqual(std::invocable auto&& f)
  {
    if constexpr (V1 == V2) { std::invoke(std::forward<decltype(f)>(f)); }
  }

  // Invokes a callable if the given orientation is Vertical.
  template <Orientation orientation>
  constexpr void IfVertical(std::invocable auto&& f)
  {
    IfEqual<orientation, Orientation::Vertical>(std::forward<decltype(f)>(f));
  }

  // Invokes a callable if the given orientation is Horizontal.
  template <Orientation orientation>
  constexpr void IfHorizontal(std::invocable auto&& f)
  {
    IfEqual<orientation, Orientation::Horizontal>(std::forward<decltype(f)>(f));
  }

  template<Orientation O, LayoutMode M>
  Rect Layout<O, M>::display(const Runtime& rt, const Rect& rect)
  {
    const auto itemSpacing = rt.getItemSpacing();
    const auto itemRects = computeItemLayouts_(rect, itemSpacing);
    assert(itemRects.size() == items_.size()
      && "Mismatch between computed rectangles and items");
    Rect actualRect{ Rect::empty() }, itemActualRect{ Rect::empty() };
    for (size_t i = 0; i < items_.size(); ++i)
    {
      Rect itemRect{ itemRects[i] };
      // We need to adjust the position based on the last item's size.
      IfVertical<O>([&]{
        if (actualRect.hasHeight())
        { // Only adjust the position if the actual rectangle has a valid height
          const int spacing = itemActualRect.size.y > 0 ? itemSpacing.y : 0;
          itemRect.origin.y = std::max(actualRect.endY() + spacing,
                                       itemRect.origin.y);
        }
      });
      IfHorizontal<O>([&]{
        if (actualRect.hasWidth())
        { // Only adjust the position if the actual rectangle has a valid width
          const int spacing = itemActualRect.size.x > 0 ? itemSpacing.x : 0;
          itemRect.origin.x = std::max(actualRect.endX() + spacing,
                                       itemRect.origin.x);
        }
      });
      if (itemRect.hasWidth() && itemRect.endX() > rect.endX())
      { // If the item rectangle exceeds the available rectangle, we unset its
        // width to allow it to shrink.
        itemRect.unsetWidth();
      }
      if (itemRect.hasHeight() && itemRect.endY() > rect.endY())
      { // If the item rectangle exceeds the available rectangle, we unset its
        // height to allow it to shrink.
        itemRect.unsetHeight();
      }
      if (!itemRect.hasWidth())
      { // If the item rectangle has no width, we assign the available width
        // as an optional to inform the item how much space it can use.
        const int x1 = actualRect.hasWidth()
          ? std::max(itemRect.origin.x, actualRect.endX()) : itemRect.origin.x;
        const int x2 = actualRect.hasWidth()
          ? std::max(rect.endX(), actualRect.endX()) : rect.endX();
        const int availableWidth = x2 - x1;
        if (availableWidth > 0) { itemRect.setOptionalWidth(availableWidth); }
        else { itemRect.unsetHeight(); }
      }
      if (!itemRect.hasHeight())
      { // If the item rectangle has no height, we assign the available height
        // as an optional to inform the item how much space it can use.
        const int y1 = actualRect.hasHeight()
          ? std::max(itemRect.origin.y, actualRect.endY()) : itemRect.origin.y;
        const int y2 = actualRect.hasHeight()
          ? std::max(rect.endY(), actualRect.endY()) : rect.endY();
        const int availableHeight = y2 - y1;
        if (availableHeight > 0) { itemRect.setOptionalHeight(availableHeight); }
        else { itemRect.unsetHeight(); }
      }
      // Display the item and get its actual rectangle
      itemActualRect = items_[i].display(rt, itemRect);
      // Update the layout's actual rectangle based on actual rectangles of
      // the items.
      actualRect.unite(itemActualRect);
    }

    // Return the actual rectangle that encompasses all the items in the stack
    return actualRect;
  }

  template<Orientation O, LayoutMode M>
  std::vector<Rect> Layout<O, M>::computeItemLayouts_(
    const Rect& rect, const Vec2i& itemSpacing)
  {
    std::vector<Rect> rects(items_.size(), rect);
    if constexpr (M == LayoutMode::Stack)
    { // For Stack mode, we just unset the size in the orientation direction
      // and return the rectangles as they are.
      for (auto& r : rects)
      {
        IfVertical<O>([&]{r.unsetHeight();});
        IfHorizontal<O>([&]{r.unsetWidth();});
      }
    }
    else
    { // For Box mode, we need to compute the sizes and positions of the items
      // based on their weights and the available space.
      // The algorithm is as follows:
      // 1. Compute the total weight of stretch items and the total size of
      //    fixed items.
      // 2. Compute the size assigned to each unit of weight based on the
      //    available size and the total weight.
      // 3. Compute the size and position of each item based on its weight or
      //    fixed size.

      // Size of weighted children
      // -------------------------
      // size = childSize + space + ... + childSize
      // size = space * (numChildren - 1) + childSize * totalWeight
      // childSize = (size - space * (numChildren - 1)) / totalWeight

      constexpr int kFixed{ 0x0 }, kWeight{ 0x1 };
      std::vector<int> itemValues(items_.size());

      int totalWeight{ 0 }, fixedSize{ 0 }, nonZeroItems{ 0 };
      size_t lastWeightedIndex{ 0 };
      for (size_t i = 0; i < items_.size(); ++i)
      {
        if (const auto* fixed = items_[i].resolveAs<Fixed>())
        { // Item has a fixed size, add it to the total fixed size.
          const int size{ static_cast<int>(fixed->size) };
          itemValues[i] = kFixed | (size << 1);
          fixedSize += size;
          nonZeroItems += (size > 0 ? 1 : 0);
        }
        else
        { // Otherwise is a stretch item, add its weight to the total weight.
          const auto* stretch = items_[i].resolveAs<Stretch>();
          const int weight{ stretch ? static_cast<int>(stretch->weight) : 1 };
          itemValues[i] = kWeight | (weight << 1);
          totalWeight += weight;
          lastWeightedIndex = i;
          nonZeroItems += (weight > 0 ? 1 : 0);
        }
      }

      if (totalWeight <= 0)
      { // If totalWeight is 0, all children have fixed size,
        // we set it to 1 to avoid division by zero.
        totalWeight = 1;
      }

      int availableSize{ 0 }, spacing{ 0 };
      IfVertical<O>([&]{
        availableSize = rect.size.y;
        spacing = itemSpacing.y;
      });
      IfHorizontal<O>([&]{
        availableSize = rect.size.x;
        spacing = itemSpacing.x;
      });

      const int adjustSize{ availableSize - fixedSize };
      const int itemSize{ std::max(0,
        (adjustSize - (nonZeroItems - 1) * spacing) / totalWeight) };
      const int leftOverSize{ adjustSize
        - (nonZeroItems - 1) * spacing - totalWeight * itemSize };

      // Compute the item rectangles based on their types
      int pos{ 0 };
      for (size_t i = 0; i < items_.size(); ++i)
      {
        int size = (itemValues[i] >> 1);
        if (itemValues[i] & kWeight)
        { // Item is a stretch item, set its rectangle based on its weight.
          size *= itemSize;
          if (i == lastWeightedIndex)
          { // Add any leftover size to the last weighted item.
            size += leftOverSize;
          }
        }
        IfVertical<O>([&]{
          rects[i].origin.y += pos;
          rects[i].size.y = size;
        });
        IfHorizontal<O>([&]{
          rects[i].origin.x += pos;
          rects[i].size.x = size;
        });
        if (size > 0)
        { // Only add spacing if the item has a non-zero size.
          pos += size + spacing;
        }
      }
    }

    // Return the computed rectangles for each item
    return rects;
  }
}
