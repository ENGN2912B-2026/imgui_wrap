//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Panel.hpp>

namespace gui2
{
  //! \brief A ScrollArea is container that can hold a Widget as its content and
  //!        provides scrollable functionality if the content exceeds the visible
  //!        area.
  //!
  //! It is implemented as a Panel without a border.
  //!
  class ScrollArea : public Panel
  {
  public:
    //! \brief Constructs a scroll area with the given identifier.
    //! \param[in] id     An item identifier. It must be unique within
    //!                   the application.
    //! \param[in] widget The content of the scroll area, which is a Widget.
    template<Identifier T>
    ScrollArea(T&& id, Widget widget = {})
      : Panel(std::forward<T>(id), std::move(widget))
    {
      setDrawBorder(false);
    }

    //! \brief Constructs a scroll area with a unique identifier generated from
    //!        the source location where the constructor is called.
    //! \param[in] widget The content of the scroll area, which is a Widget.
    //! \param[in] location The source location where the scroll area is
    //!                     constructed.
    ScrollArea(Widget widget = {},
               std::source_location location = std::source_location::current())
      : Panel(std::move(widget), std::move(location))
    {
      setDrawBorder(false);
    }
  };
}