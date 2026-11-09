//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Types.hpp>

#include <functional>

namespace gui2
{
  //! \brief A widget that executes user provided Dear ImGui code.
  //!
  //! When displayed, the Runtime positions the ImGui cursor at the top-left
  //! of the assigned rectangle and calls the user function. The function is
  //! expected to emit ImGui items; the area they cover is used as the widget
  //! rectangle.
  //!
  //! For example:
  //!
  //!   UserCode{ []{ ImGui::TextColored({1,0,0,1}, "Hello"); } }
  class UserCode
  {
  public:
    //! \brief Constructs a widget that runs user code.
    //! \param[in] code The function that runs the ImGui code.
    UserCode(std::function<void()> code) : code_{std::move(code)} {}

    //! \brief Executes the user function if it is set.
    void run() const { if (code_) { code_(); } }

  private:
    std::function<void()> code_;
  };

} // namespace gui2
