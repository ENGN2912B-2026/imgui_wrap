//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>

namespace gui2
{
  //! \brief A wrapper that disables interaction with its content.
  class Disabled
  {
  public:
    //! \brief Constructs a Disabled wrapper with the given content.
    //! \param[in] disabled Whether the content should be disabled.
    //! \param[in] content  The widget to display.
    Disabled(bool disabled, Widget content)
      : disabled_{disabled}, content_{std::move(content)} {}

    //! \brief Checks whether the content is disabled.
    //! \return `true` if the content is disabled; otherwise, `false`.
    bool isDisabled() const { return disabled_; }

    //! \brief Gets the wrapped widget.
    //! \return A const reference to the wrapped widget.
    Widget& getContent() { return content_; }

  private:
    bool disabled_;
    Widget content_;
  };

} // namespace gui2