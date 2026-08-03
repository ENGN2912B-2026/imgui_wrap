//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/BoxT.hpp>

namespace gui2
{
  class HBox : public BoxT<LayoutOrientation::Horizontal>
  {
  public:
    using Base = BoxT<LayoutOrientation::Horizontal>;
    using Base::Base;
  };

} // namespace gui
