//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>
#include <gui2/Identifier.hpp>

#include <string>

namespace gui2
{
  class Panel
  {
    std::string id_;
    std::string name_;
    Widget widget_;
  public:
    template<Identifier T>
    Panel(T&& id, Widget widget = {})
      : widget_{std::move(widget)}
      , id_{id.getValueStr()}
    {
      if constexpr (IdentifierWithName<std::remove_cvref_t<T>>)
      {
        name_ = id.getName();
      }
    }

    Panel(Widget widget = {},
          std::source_location location = std::source_location::current())
      : widget_{std::move(widget)}
    {
      LocationId id{std::move(location)};
      id_ = id.getValueStr();
    }

    const std::string& getId() const { return id_; }
    const std::string& getName() const { return name_; }

    void displayContent(const Runtime& runtime) const { widget_.display(runtime); }
  };

} // namespace gui
