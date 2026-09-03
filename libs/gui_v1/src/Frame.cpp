//  Copyright (c) 2024 Daniel Moreno. All rights reserved.
//

#include <gui/Frame.hpp>
#include <gui/Window.hpp>
#include <gui/Application.hpp>

#include <imgui.h>

namespace
{
  std::string generateIdentifier_(uint64_t seed, size_t length = 4)
  {
    const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::string result;
    std::srand(seed);

    for (size_t i = 0; i < length; ++i) {
        result += alphanum[std::rand() % (sizeof(alphanum) - 1)];
    }

    return result;
  }
}

namespace gui
{
  static constexpr int kDefaultFlags{
    ImGuiWindowFlags_NoDecoration |
    ImGuiWindowFlags_NoResize
  };

  Frame::Frame(
    const std::string& name,
    const Vec2i& pos,
    const Vec2i& size,
    int flags) :
      Widget{name, pos, size},
      flags_{ flags < 0 ? kDefaultFlags : flags}
  {
    if (auto app = Application::getInstancePtr())
    {
      app->getWindow().addFrame(this);
    }
  }

  Frame::~Frame()
  {
    if (auto app = Application::getInstancePtr())
    {
      app->getWindow().removeFrame(this);
    }
  }

  bool Frame::renderBegin()
  {
    ImGui::SetNextWindowPos(getPosition().to<float>());
    ImGui::SetNextWindowSize(getSize().to<float>());
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    return ImGui::Begin(getName().c_str(), nullptr, flags_);
  }

  void Frame::renderEnd()
  {
    ImGui::End();
    ImGui::PopStyleVar();
  }

  void Frame::render()
  {

  }

} // namespace gui
