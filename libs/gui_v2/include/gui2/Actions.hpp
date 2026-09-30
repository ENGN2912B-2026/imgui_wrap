//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

namespace gui2
{
  //! \brief Actions that can be performed on a GUI item
  //!
  //! The Actions enum defines a set of actions that can be performed on a GUI
  //! item, such as focusing the item or scrolling to the end of a scrollable.
  //! These are typically one-time actions that are requested by the application
  //! and performed by the GUI runtime during the next frame. After the action
  //! is performed, it is cleared, so that it is not performed again in
  //! subsequent frames unless requested again.
  //!
  //! Not all actions are applicable to all GUI items. For example, the
  //! `ScrollToEnd` action is only applicable to scrollable items.
  //!
  enum class Actions
  {
    //! \brief No action.
    None        = 0x00,
    //! \brief Focus the item.
    Focus       = 0x01,
    //! \brief Scroll to the end of a scrollable item.
    ScrollToEnd = 0x02,
  };

  //! \brief Bitwise OR operator for Actions enum.
  constexpr Actions operator|(Actions a, Actions b)
  {
    return static_cast<Actions>(static_cast<int>(a) | static_cast<int>(b));
  }

  //! \brief Bitwise OR assignment operator for Actions enum.
  constexpr Actions& operator|=(Actions& a, Actions b)
  {
    a = a | b;
    return a;
  }

  //! \brief Checks if an Actions value contains a specific action.
  constexpr bool contains(Actions a, Actions b)
  {
    return (static_cast<int>(a) & static_cast<int>(b)) != 0;
  }
}
