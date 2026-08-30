//  Copyright (c) 2024 Daniel Moreno. All rights reserved.
//

#pragma once

namespace gl
{
  class Shape
  {
  public:
    virtual ~Shape() = 0;

    virtual void initGL() {};

    virtual void deinitGL() {};

    virtual void drawGL() const {};
  };
} // namespace gl
