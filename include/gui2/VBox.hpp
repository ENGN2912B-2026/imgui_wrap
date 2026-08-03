//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/BoxT.hpp>

namespace gui2
{
  class VBox : public BoxT<LayoutOrientation::Vertical>
  {
  public:
    using Base = BoxT<LayoutOrientation::Vertical>;
    using Base::Base;
  };

} // namespace gui
