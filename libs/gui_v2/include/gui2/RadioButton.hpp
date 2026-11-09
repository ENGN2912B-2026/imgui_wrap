//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>

#include <functional>

namespace gui2
{
  //! \brief A radio button widget that selects a value from a group of options.
  //!
  //! A radio button is selected when the value pointed to by `currentOption`
  //! equals its option value. Selecting the button assigns its option value to
  //! `currentOption`.
  class RadioButton
  {
  public:
    //! \brief Constructs a radio button with the given label and option value.
    //! \param[in] label         The text to display beside the radio button.
    //! \param[in] currentOption A pointer to the value shared by the option
    //!                          group. The radio button does not take
    //!                          ownership of the value. If null, the button
    //!                          is never selected and cannot be selected.
    //! \param[in] optionValue   The value represented by this radio button.
    //!
    //! The pointed-to value must remain valid while the radio button is used.
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

    //! \brief Gets the label of the radio button.
    //! \return The text displayed beside the radio button.
    const std::string& getLabel() const { return label_; }

    //! \brief Checks whether this radio button is selected.
    //! \return `true` if the current option equals this button's option value.
    bool isSelected() const { return isSelected_(); }

    //! \brief Selects this radio button.
    //!
    //! The current option is set to this button's option value.
    void setSelected() { setSelected_(); }

  private:
    std::string label_;
    std::function<bool()> isSelected_;
    std::function<void()> setSelected_;
  };

} // namespace gui2
