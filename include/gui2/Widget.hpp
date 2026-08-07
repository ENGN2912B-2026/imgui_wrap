//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Runtime.hpp>

#include <string>
#include <memory>
#include <functional>

namespace gui2
{
  // Concept to check if a type can be displayed by the Runtime
  template<class T>
  concept Primitive =
    requires(Runtime& rt, T& value)
    {
      rt.display(value);
    };

  // Widget class
  // A widget is any object that can be displayed in the user interface.
  class Widget
  {
    struct Value
    {
      virtual ~Value() = default;
      virtual void display(const Runtime&, const OptionalSize&) = 0;
    };

    template<class T>
    struct ValueInstance : Value
    {
      T value;
      ValueInstance(T value) : value(std::move(value)) {}
      void display(const Runtime& rt, const OptionalSize& displaySize) override
      {
        rt.display(value, displaySize);
      }
    };

    std::unique_ptr<Value> value_;

  public:
    Widget() = default;

    template<Primitive T>
    Widget(T value) : value_{std::make_unique<ValueInstance<T>>(std::move(value))} {}

    void display(const Runtime& runtime, const OptionalSize& displaySize = {})
    {
      if (value_)
      {
        value_->display(runtime, displaySize);
      }
    }
  };

} // namespace gui
