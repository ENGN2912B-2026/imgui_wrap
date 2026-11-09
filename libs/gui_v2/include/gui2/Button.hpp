//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>

#include <functional>
#include <optional>

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
    //! \param[in] color   An optional color (RGBA) for the button background
    //!                    (`ImGuiCol_Button`). If not set, the theme color is
    //!                    used.
    Button(const std::string& label, std::function<void()> onClick = {},
           std::optional<Vec4f> color = {})
      : label_{label}, onClick_{std::move(onClick)}, color_{color} {}

    //! \brief Gets the label of the button.
    const std::string& getLabel() const { return label_; }
    //! \brief Gets the onClick callback of the button.
    const std::function<void()>& getOnClick() const { return onClick_; }

    //! \brief Gets the optional background color of the button.
    const std::optional<Vec4f>& getColor() const { return color_; }
    //! \brief Sets the optional background color of the button.
    //! \param[in] color The color (RGBA), or std::nullopt to use the theme.
    void setColor(std::optional<Vec4f> color) { color_ = color; }

    //! \brief Execute the onClick callback if it is set.
    //!
    //! This method is called when the button is clicked.
    void onClick() const { if (onClick_) { onClick_(); } }

  private:
    std::string label_;
    std::function<void()> onClick_;
    std::optional<Vec4f> color_;
  };

} // namespace gui
