//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Types.hpp>

#include <stdexcept>

namespace gui2
{
  void validate(Orientation orientation)
  {
    switch (orientation)
    {
      case Orientation::Vertical:
      case Orientation::Horizontal:
        break;
      default:
        throw std::invalid_argument("Invalid orientation");
    }
  }
}