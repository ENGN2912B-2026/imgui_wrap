//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Runtime.hpp>

#include <string>
#include <memory>
#include <functional>

namespace gui2
{
  // Forward declarations
  class Widget;

  // Concept of things that Widget can hold as content:
  // - The type is a Content type (either Primitive or Displayable), or
  //   a ContentFactory type (a callable that returns a Content type).
  // - The type is not Widget itself (to avoid infinite recursion and wrapping
  //   a Widget into another Widget).
  template<class T>
  concept WidgetContent =
    (Content<T> || ContentFactory<T>) &&
    (!std::same_as<std::remove_cvref_t<T>, Widget>);

  // Widget class
  // A widget is any object that can be displayed in the user interface.
  class Widget
  {
    struct Value
    {
      virtual ~Value() = default;
      virtual void display(const Runtime& rt, const Rect& rect) = 0;
    };

    template<class T>
    struct WidgetValue : Value
    {
      T value;
      WidgetValue(T value) : value(std::move(value)) {}
      void display(const Runtime& rt, const Rect& rect) override
      {
        if constexpr (Content<T>)
        {
          displayContent(rt, value, rect);
        }
        else if constexpr (ContentFactory<T>)
        {
          auto&& content = value();
          displayContent(rt, content, rect);
        }
      }
    private:
      template<Content U>
      void displayContent(const Runtime& rt, U& content, const Rect& rect)
      {
        if constexpr (Primitive<U>)
        {
          rt.display(content, rect);
        }
        else if constexpr (Displayable<U>)
        {
          content.display(rt, rect);
        }
      }
    };

    std::unique_ptr<Value> value_;

  public:
    Widget() = default;

    template<WidgetContent T>
    Widget(T value) : value_{std::make_unique<WidgetValue<T>>(std::move(value))} {}

    void display(const Runtime& rt, const Rect& rect)
    {
      if (value_)
      {
        value_->display(rt, rect);
      }
    }
  };

} // namespace gui
