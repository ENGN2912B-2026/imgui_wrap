//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Types.hpp>

namespace gui2
{
  //! \brief An empty widget that displays no content.
  //!
  //! By default, an empty widget occupies no space. It can be used when a
  //! widget is required but nothing should be displayed, such as in a
  //! conditional layout.
  //!
  //! For example:
  //!
  //!   Widget myWidget{
  //!     condition ? Widget{Button{"Click Me"}} : Widget{Empty{}} };
  //!
  //!
  //! When `condition` is false, nothing is displayed.
  //!
  //! An empty widget can also create a gap between layout items:
  //!
  //!   VBox{ "Line 1", Empty{}, "Line 2" }
  //!
  //! The minimum size can be set to reserve space for the empty widget.
  class Empty
  {
    Vec2i minSize_;
  public:
    //! \brief Constructs an empty widget with the given minimum size.
    //! \param[in] minSize The minimum size to reserve, in pixels.
    Empty(Vec2i minSize = {0, 0}) : minSize_(minSize) {}

    //! \brief Gets the minimum size of the empty widget.
    //! \return A const reference to the minimum size, in pixels.
    const Vec2i& getMinSize() const { return minSize_; }
  };

} // namespace gui2
