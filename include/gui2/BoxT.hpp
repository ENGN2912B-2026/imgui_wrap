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

  enum class LayoutOrientation
  {
    Vertical,
    Horizontal
  };

  template<LayoutOrientation orientation>
  class BoxT
  {
  public:
    constexpr static LayoutOrientation kOrientation = orientation;

    BoxT() = default;

    template<typename... T>
    requires (std::constructible_from<BoxItem, T&&> && ...)
    BoxT(T&&... items)
    {
      (items_.emplace_back(std::forward<T>(items)), ...);
    }

    const std::vector<BoxItem>& getItems() const { return items_; }

  private:
    std::vector<BoxItem> items_;
  };

} // namespace gui
