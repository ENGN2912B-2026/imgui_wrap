//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>

#include <functional>

namespace gui2
{
  class RadioButton
  {
  public:
    template <typename T>
    RadioButton(const std::string& label, T* currentOption, T optionValue)
      : label_{label}
      , isSelected_{
          [currentOption, optionValue]
          {
            return currentOption && *currentOption == optionValue;
          }
        }
      , setSelected_{
          [currentOption, optionValue]
          {
            if (currentOption) { *currentOption = optionValue; }
          }
        }
    {
      // empty body
    }

    const std::string& getLabel() const { return label_; }

    bool isSelected() const { return isSelected_(); }
    void setSelected() { setSelected_(); }

  private:
    std::string label_;
    std::function<bool()> isSelected_;
    std::function<void()> setSelected_;
  };

} // namespace gui2
