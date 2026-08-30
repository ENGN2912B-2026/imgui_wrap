//  Copyright (c) 2024 Daniel Moreno. All rights reserved.
//

#include <gui/ChildFrame.hpp>

#include <imgui.h>

namespace gui
{
  static constexpr int kDefaultChildFlags{
    ImGuiChildFlags_Borders
  };

  ChildFrame::ChildFrame(
    const std::string& name,
    const Vec2i& pos,
    const Vec2i& size,
    int childFlags,
    int windowFlags) :
      Widget{name, pos, size},
      childFlags_{ childFlags < 0 ? kDefaultChildFlags : childFlags },
      windowFlags_{ windowFlags < 0 ? ImGuiWindowFlags_None : windowFlags }
  {

  }

  ChildFrame::~ChildFrame()
  {

  }

  bool ChildFrame::renderBegin()
  {
    ImGui::SetNextWindowPos(getPosition().to<float>());
    return ImGui::BeginChild(
      getName().c_str(), getSize().to<float>(), childFlags_, windowFlags_);
  }

  void ChildFrame::renderEnd()
  {
    ImGui::EndChild();
  }

  void ChildFrame::render()
  {

  }

} // namespace gui
