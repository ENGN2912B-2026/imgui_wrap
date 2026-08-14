//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/BoxT.hpp>

namespace gui2
{
  class HBox : public BoxT<Orientation::Horizontal>
  {
  public:
    using Base = BoxT<Orientation::Horizontal>;
    using Base::Base;
  };

} // namespace gui
