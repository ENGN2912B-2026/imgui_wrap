//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Runtime.hpp>
#include <gui2/Empty.hpp>

#include <string>
#include <memory>
#include <functional>
#include <cassert>

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
    // Abstract base class for the value held by the Widget.
    struct Value
    {
      virtual ~Value() = default;
      virtual Rect display(const Runtime& rt, const Rect& rect) = 0;
    };

    // The actual value held by the Widget.
    std::unique_ptr<Value> value_;

    // Forward declarations.
    template<class T> struct WidgetValue;

  public:
    // Default constructor creates an empty Widget.
    Widget() : Widget{Empty{}} {}

    // Constructor that takes a value of type T, and stores it within the
    // Widget, which takes ownership of the value.
    template<WidgetContent T>
    Widget(T value);

    // Returns a pointer to the value of type T held by the Widget, or nullptr
    // if the Widget does not hold a value of type T.
    template<WidgetContent T>
    T* get();

    // Returns a const pointer to the value of type T held by the Widget, or nullptr
    // if the Widget does not hold a value of type T.
    template<WidgetContent T>
    const T* get() const;

    // Display the widget using the provided Runtime and within the specified
    // rectangle. Returns the actual rectangle used for displaying the widget.
    Rect display(const Runtime& rt, const Rect& rect);
  };

} // namespace gui

// Implementation -------------------------------------------------------------
namespace gui2
{
  template<class T>
  struct Widget::WidgetValue : Value
  {
    T value;
    WidgetValue(T value) : value(std::move(value)) {}
    Rect display(const Runtime& rt, const Rect& rect) override
    {
      if constexpr (Content<T>)
      {
        return displayContent(rt, value, rect);
      }
      else if constexpr (ContentFactory<T>)
      {
        auto&& content = value();
        return displayContent(rt, content, rect);
      }
      else
      {
        static_assert(false, "Type is not displayable");
      }
    }
  private:
    template<Content U>
    Rect displayContent(const Runtime& rt, U& content, const Rect& rect)
    {
      if constexpr (Primitive<U>)
      {
        return rt.display(content, rect);
      }
      else if constexpr (Displayable<U>)
      {
        return content.display(rt, rect);
      }
      else
      {
        static_assert(false, "Type is not displayable");
      }
    }
  };

  template<WidgetContent T>
  Widget::Widget(T value)
    : value_{std::make_unique<WidgetValue<T>>(std::move(value))}
  {
    // empty body
  }

  template<WidgetContent T>
  T* Widget::get()
  {
    if (auto* widgetValue = dynamic_cast<WidgetValue<T>*>(value_.get()))
    {
      return &widgetValue->value;
    }
    return nullptr;
  }

  template<WidgetContent T>
  const T* Widget::get() const
  {
    if (auto* widgetValue = dynamic_cast<const WidgetValue<T>*>(value_.get()))
    {
      return &widgetValue->value;
    }
    return nullptr;
  }

  inline Rect Widget::display(const Runtime& rt, const Rect& rect)
  {
    assert(value_ != nullptr && "Widget has no value to display");
    return value_->display(rt, rect);
  }
}