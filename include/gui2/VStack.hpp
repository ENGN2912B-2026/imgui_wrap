//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/StackT.hpp>

namespace gui2
{
  // A vertical stack of widgets. The widgets are displayed in the order they
  // are added to the stack, and with a spacing between them defined by the
  // runtime. Their sizes are determined by their content, and the stack will
  // expand to fit them.
  using VStack = StackT<Orientation::Vertical>;
} // namespace gui