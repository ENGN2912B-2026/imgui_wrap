//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>

#include <functional>

namespace gui2
{
  class Button
  {
  public:
    Button(const std::string& label, std::function<void()> onClick = {})
      : label_(label), onClick_(std::move(onClick)) {}

    const std::string& getLabel() const { return label_; }
    void onClick() const { onClick_(); }

  private:
    std::string label_;
    std::function<void()> onClick_;
  };

} // namespace gui
