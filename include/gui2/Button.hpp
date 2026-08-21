//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>

#include <functional>

namespace gui2
{
  //! \brief A button widget that can be clicked to trigger an action.
  class Button
  {
  public:
    //! \brief Constructs a button with the given label and an optional onClick
    //!        callback.
    //! \param[in] label   The text to display on the button.
    //! \param[in] onClick A callback function to invoke when the button is
    //!                    clicked.
    Button(const std::string& label, std::function<void()> onClick = {})
      : label_{label}, onClick_{std::move(onClick)} {}

    //! \brief Gets the label of the button.
    const std::string& getLabel() const { return label_; }
    //! \brief Gets the onClick callback of the button.
    const std::function<void()>& getOnClick() const { return onClick_; }

    //! \brief Execute the onClick callback if it is set.
    //!
    //! This method is called when the button is clicked.
    void onClick() const { if (onClick_) { onClick_(); } }

  private:
    std::string label_;
    std::function<void()> onClick_;
  };

} // namespace gui
