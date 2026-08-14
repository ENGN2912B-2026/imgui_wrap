//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/BoxT.hpp>

namespace gui2
{
  class VBox : public BoxT<Orientation::Vertical>
  {
  public:
    using Base = BoxT<Orientation::Vertical>;
    using Base::Base;
  };

} // namespace gui
