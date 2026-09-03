//  Copyright (c) 2024 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gui/Rect.hpp>

#include <string>
#include <vector>

namespace gui
{
  class Widget : public Rect
  {
    std::string name_;
    std::vector<Widget*> children_;
  public:
    Widget(
      const std::string& name = {},
      const Vec2i& pos = {0, 0},
      const Vec2i& size = {0, 0});

    virtual ~Widget() = 0;

    virtual bool renderBegin();
    virtual void renderEnd();
    virtual void render();

    const std::string& getName() const;

    void setName(const std::string& name);

    const std::vector<Widget*>& getChildren() const;

    virtual void addChild(Widget* child);

    virtual void removeChild(Widget* child);

    virtual void draw();

    Vec2i getItemSpacing() const;

    Vec2i getContentMin() const;

    Vec2i getContentMax() const;

    Vec2i getContentSize() const;

  };

} // namespace gui
