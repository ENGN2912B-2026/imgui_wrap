//  Copyright (c) 2024 Daniel Moreno. All rights reserved.
//

#include <gui/imgui_stdlib.hpp>

#include <imgui.h>

namespace ImGui
{
  void Text(std::string_view text)
  {
    ImGui::TextUnformatted(text.data(), text.data() + text.size());
  }
}
