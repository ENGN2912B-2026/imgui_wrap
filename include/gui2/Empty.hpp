//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

namespace gui2
{
  // An empty widget that does not display anything and does not occupy any
  // space.
  //
  // The widget can be used in place where the code expects a widget to be
  // supplied, but nothing should be displayed. It is useful for creating
  // conditional layouts where a widget may or may not be displayed based
  // on certain conditions.
  //
  // For example:
  //
  //    Widget myWidget{ condition ? Button{"Click Me"} : Empty{} };
  //
  // In this example, if the condition is true, a button will be displayed.
  // If the condition is false, nothing will be displayed.
  //
  class Empty
  {
  };

} // namespace gui
