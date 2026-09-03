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
  //    Widget myWidget{
  //      condition ? Widget{Button{"Click Me"}} : Widget{Empty{}} };
  //
  // In this example, if the condition is true, a button will be displayed.
  // If the condition is false, nothing will be displayed.
  //
  // Another example:
  //
  //    VBox { "Line 1", Empty{}, "Line 2" }
  //
  // In this example, the VBox will display "Line 1" and "Line 2" with an
  // empty space in between them.
  //
  class Empty {};

} // namespace gui
