//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>

namespace gui2
{
  class Panel
  {
    Widget widget_;
  public:
    Panel(Widget widget = {}) : widget_{std::move(widget)} {}
    void setContent(Widget widget) { widget_ = std::move(widget); }
    void displayContent(const Runtime& runtime) const { widget_.display(runtime); }
  };

} // namespace gui
