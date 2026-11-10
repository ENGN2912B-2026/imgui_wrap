//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>

#include <vector>

namespace gui2
{
  //! \brief Displays main content followed by additional content.
  //!
  //! All content receives the same rectangle. Only the main content's bounds
  //! contribute to the returned rectangle; additional content is responsible
  //! for its own rendering and input behavior.
  class Overlay
  {
  public:
    //! \brief Constructs an overlay with main and additional content.
    //! \param[in] content The main content, which determines the layout bounds.
    //! \param[in] overlays The additional content, displayed in order.
    template<typename... T>
      requires (std::constructible_from<Widget, T&&> && ...)
    Overlay(Widget content, T&&... overlays)
      : content_{std::move(content)}
    {
      (overlays_.emplace_back(std::forward<T>(overlays)), ...);
    }

    //! \brief Displays all content and returns the main content's bounds.
    Rect display(const Runtime& rt, const Rect& rect)
    {
      const Rect actual = content_.display(rt, rect);
      for (auto& overlay : overlays_) { overlay.display(rt, rect); }
      return actual;
    }

  private:
    Widget content_;
    std::vector<Widget> overlays_;
  };
}
