//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>

#include <string>
#include <stdexcept>

namespace gui2
{
  class Panel
  {
    std::string name_;
    Widget widget_;
  public:
    Panel(std::string name = "panel", Widget widget = {})
    {
      setName(std::move(name));
      setContent(std::move(widget));
    }

    const std::string& getName() const { return name_; }
    void setName(std::string name)
    {
      if (name.empty())
      {
        throw std::invalid_argument{"Panel name cannot be empty"};
      }
      name_ = std::move(name);
    }

    const Widget& getContent() const { return widget_; }
    void setContent(Widget widget) { widget_ = std::move(widget); }

    void displayContent(const Runtime& runtime) const { widget_.display(runtime); }
  };

} // namespace gui
