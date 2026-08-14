//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>

#include <vector>
#include <variant>

namespace gui2
{
  struct Pixels
  {
    int value;
    Pixels(int value = -1) : value(value) {}
  };

  struct Weight
  {
    int value;
    Weight(int value = 1) : value(value) {}
  };

  struct BoxItem
  {
    Widget widget;
    std::variant<Pixels, Weight> sizeOrWeight = Weight{1};

    BoxItem(Widget widget)
        : widget(std::move(widget))
    {
    }

    BoxItem(Widget widget, Pixels size)
        : widget(std::move(widget)), sizeOrWeight(size)
    {
    }

    BoxItem(Widget widget, Weight weight)
        : widget(std::move(widget)), sizeOrWeight(weight)
    {
    }
  };

  struct Fixed
  {
    Pixels size;
    Widget widget;
    Fixed(Pixels size, Widget widget)
      : size{std::move(size)}
      , widget{std::move(widget)}
    { }
    operator BoxItem() { return {std::move(widget), std::move(size)}; }
  };

  struct Stretch
  {
    Weight weight;
    Widget widget;
    Stretch(Weight weight, Widget widget)
      : weight{std::move(weight)}
      , widget{std::move(widget)}
    { }
    operator BoxItem() { return {std::move(widget), std::move(weight)}; }
  };

  template<Orientation orientation>
  class BoxT
  {
  public:
    constexpr static Orientation kOrientation = orientation;

    BoxT() = default;

    template<typename... T>
    requires (std::constructible_from<BoxItem, T&&> && ...)
    BoxT(T&&... items)
    {
      (items_.emplace_back(std::forward<T>(items)), ...);
    }

    const std::vector<BoxItem>& getItems() const { return items_; }
    std::vector<BoxItem>& getItems() { return items_; }

    void display(const Runtime& rt, const Rect& rect);

  private:
    std::vector<BoxItem> items_;

    struct Rect1d
    {
      int origin{ 0 };
      int size{ 0 };
    };

    std::vector<Rect1d> computeItemLayouts_(
      const Rect1d& availableRect, int itemSpacing) const;
  };

} // namespace gui


// Template implementations ---------------------------------------------------
namespace gui2
{
  template<Orientation orientation>
  std::vector<typename BoxT<orientation>::Rect1d> BoxT<orientation>::computeItemLayouts_(
    const Rect1d& availableRect, int itemSpacing) const
  {
    // Size of weighted children
    // -------------------------
    // size = childSize + space + ... + childSize
    // size = space * (numChildren - 1) + childSize * totalWeight
    // childSize = (size - space * (numChildren - 1)) / totalWeight

    const auto hasWeight = [](const BoxItem& item) {
      return std::holds_alternative<Weight>(item.sizeOrWeight);
    };
    const auto hasSize = [](const BoxItem& item) {
      return std::holds_alternative<Pixels>(item.sizeOrWeight);
    };
    const auto getWeight = [](const BoxItem& item) {
      return std::get<Weight>(item.sizeOrWeight).value;
    };
    const auto getSize = [](const BoxItem& item) {
      return std::get<Pixels>(item.sizeOrWeight).value;
    };

    int totalWeight{ 0 }, fixedSize{ 0 }, lastWeightedIndex{ 0 };
    const int numItems{ static_cast<int>(items_.size()) };
    for (int i = 0; i < numItems; ++i)
    {
      if (hasWeight(items_[i]))
      {
        totalWeight += getWeight(items_[i]);
        lastWeightedIndex = i;
      }
      else if (hasSize(items_[i]))
      {
        fixedSize += getSize(items_[i]);
      }
      else
      {
        throw std::logic_error(
          "VBox item must have either a weight or a fixed size");
      }
    }

    if (totalWeight <= 0)
    { // if totalWeight is 0, all children have fixed size,
      //  we set to 1 to avoid division by zero
      totalWeight = 1;
    }

    const int adjustSize{ availableRect.size - fixedSize };
    const int itemSize{ std::max(0,
      (adjustSize - (numItems - 1) * itemSpacing) / totalWeight) };
    const int leftOverSize{ adjustSize
      - (numItems - 1) * itemSpacing - totalWeight * itemSize };

    // Apply to children
    std::vector<Rect1d> rects;
    rects.reserve(items_.size());
    int pos{ availableRect.origin };
    for (int i = 0; i < numItems; ++i)
    {
      const auto& item{ items_[i] };
      int size{ hasSize(item) ? getSize(item) : getWeight(item) * itemSize };
      if (i == lastWeightedIndex)
      {
        size += leftOverSize;
      }
      rects.push_back({ pos, size });
      pos += size + itemSpacing;
    }

    // Return the computed rectangles for each item
    return rects;
  }

  template<Orientation orientation>
  void BoxT<orientation>::display(const Runtime& rt, const Rect& rect)
  {
    // Compute the sizes and positions of the VBox items
    Rect1d availableRect;
    int itemSpacing = 0;
    if constexpr (orientation == Orientation::Vertical)
    {
      availableRect = { rect.origin.y, rect.size.y };
      itemSpacing = rt.getItemSpacing().y;
    }
    else if constexpr (orientation == Orientation::Horizontal)
    {
      availableRect = { rect.origin.x, rect.size.x };
      itemSpacing = rt.getItemSpacing().x;
    }
    else
    {
      throw std::logic_error{"Invalid layout orientation"};
    }
    const auto rects1d = computeItemLayouts_(availableRect, itemSpacing);

    // Display the items
    const int numItems{ static_cast<int>(items_.size()) };
    Rect itemRect{ rect };
    for (int i = 0; i < numItems; ++i)
    {
      if constexpr (orientation == Orientation::Vertical)
      {
        itemRect.origin.y = rects1d[i].origin;
        itemRect.size.y = rects1d[i].size;
      }
      else if constexpr (orientation == Orientation::Horizontal)
      {
        itemRect.origin.x = rects1d[i].origin;
        itemRect.size.x = rects1d[i].size;
      }
      else
      {
        throw std::logic_error{"Invalid layout orientation"};
      }
      items_[i].widget.display(rt, itemRect);
    }
  }

} // namespace gui