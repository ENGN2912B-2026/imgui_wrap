//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>

namespace gui2
{
  class CheckBox
  {
  public:
    CheckBox(const std::string& label, bool* checked)
      : label_{label}, checked_{checked} {}

    const std::string& getLabel() const { return label_; }

    bool isChecked() const { return checked_ && *checked_; }
    void setChecked(bool checked) { if (checked_) *checked_ = checked; }

  private:
    std::string label_;
    bool* checked_;
  };

} // namespace gui
