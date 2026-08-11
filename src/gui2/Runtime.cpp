//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Runtime.hpp>

#include "../gui/impl/Backend.hpp"
#include <gui2/Panel.hpp>
#include <gui2/VBox.hpp>
#include <gui2/HBox.hpp>
#include <gui2/Button.hpp>
#include <gui2/CheckBox.hpp>
#include <gui2/Empty.hpp>

#include <imgui.h>

#ifdef USE_IMPLOT
# include <implot.h>
#endif

#include <stdexcept>

namespace gui2
{
  namespace
  {
    constexpr int kDefaultImGuiWindowFlags{
      ImGuiWindowFlags_NoDecoration |
      ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove
    };

    constexpr int kDefaultImGuiChildWindowFlags{
      ImGuiChildFlags_Borders
    };

  } // anonymous namespace

  Runtime::~Runtime()
  {
    deinit();
  }

  Runtime& Runtime::getInstance()
  {
    static Runtime instance;
    return instance;
  }

  void Runtime::init(
    const std::string& windowTitle,
    const Vec2i& windowSize)
  {
    if (backend_ != nullptr)
    { // Already initialized
      return;
    }

    // Initialize the runtime
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
#ifdef USE_IMPLOT
    ImPlot::CreateContext();
#endif

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
#ifdef IMGUI_HAS_DOCK
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif
#ifdef IMGUI_HAS_VIEWPORT
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
#endif
#if IMGUI_VERSION_NUM >= 19004
    //io.ConfigDebugIsDebuggerPresent = ImOsIsDebuggerPresent();
#endif

    // Setup Dear ImGui style
    ImGui::StyleColorsLight();

    // Setup backend
    backend_ = Backend::create();
    backend_->DpiAware = true;
    backend_->SrgbFramebuffer = false;
    backend_->Vsync = true;
    backend_->ClearColor = ImVec4(0.120f, 0.120f, 0.120f, 1.000f);
    backend_->InitCreateWindow(windowTitle.c_str(), windowSize.to<float>());
    backend_->InitBackends();
  }

  void Runtime::deinit()
  {
    if (backend_ == nullptr)
    { // Not initialized
      return;
    }

    // Shutdown backend
    backend_->ShutdownBackends();
    backend_->ShutdownCloseWindow();
    backend_.reset();
    // Delete context
#ifdef USE_IMPLOT
    ImPlot::DestroyContext();
#endif
    ImGui::DestroyContext();
  }

  void Runtime::setWindowTitle(const std::string& title)
  {
    if (backend_ == nullptr)
    { // Not initialized
      return;
    }

    backend_->SetWindowTitle(title.c_str());
  }

  Vec2i Runtime::getWindowSize() const
  {
    if (backend_ == nullptr)
    { // Not initialized
      return {0, 0};
    }

    ImVec2 size = ImGui::GetIO().DisplaySize;
    return {static_cast<int>(size.x), static_cast<int>(size.y)};
  }

  void Runtime::setWindowSize(const Vec2i& size)
  {
    if (backend_ == nullptr)
    { // Not initialized
      return;
    }

    backend_->SetWindowSize(size.to<float>());
  }

  bool Runtime::frameBegin()
  {
    if (!backend_->NewFrame())
    {
      return false;
    }

    // Start a new Dear ImGui frame
    ImGui::NewFrame();

    // We create a single Dear ImGui window that covers the entire viewport.
    // In this way, the desktop window and the Dear ImGui window looks like a
    // single window.
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    return ImGui::Begin("mainFrame", nullptr, kDefaultImGuiWindowFlags);
  }

  void Runtime::frameEnd()
  {
    // End the current Dear ImGui window and pop the style variable.
    ImGui::End();
    ImGui::PopStyleVar();

    // Render the Dear ImGui frame and the desktop window.
    ImGui::Render();
    backend_->Render();
  }

  void Runtime::display(const Empty& empty, const Rect& rect) const
  {
    // Do nothing, just reserve space for the empty item
    ImGui::SetCursorScreenPos(rect.origin.to<float>());
  }

  void Runtime::display(const std::string& text, const Rect& rect) const
  {
    ImGui::SetCursorScreenPos(rect.origin.to<float>());
    float localPosX = ImGui::GetCursorPosX();
    ImGui::PushTextWrapPos(localPosX + rect.size.x);
    ImGui::Text("%s", text.c_str());
    ImGui::PopTextWrapPos();
  }

   void Runtime::display(const Button& button, const Rect& rect) const
  {
    ImGui::SetCursorScreenPos(rect.origin.to<float>());
    if (ImGui::Button(button.getLabel().c_str()))
    {
      button.onClick();
    }
  }

  void Runtime::display(CheckBox& checkBox, const Rect& rect) const
  {
    bool checked = checkBox.isChecked();
    ImGui::SetCursorScreenPos(rect.origin.to<float>());
    if (ImGui::Checkbox(checkBox.getLabel().c_str(), &checked))
    {
      checkBox.setChecked(checked);
    }
  }

  void Runtime::display(Panel& panel, const Rect& rect) const
  {
    int flags = kDefaultImGuiChildWindowFlags;
    ImGui::SetNextWindowPos(rect.origin.to<float>());
    if (ImGui::BeginChild(panel.getId().c_str(), rect.size.to<float>(), flags))
    {
      Vec2i pad = getWindowPadding();
      Rect innerRect{ rect.origin + pad, rect.size - pad * 2 };
      panel.displayContent(*this, innerRect);
    }
    ImGui::EndChild(); // For child windows `EndChild()` must be called even
                       // if `BeginChild()` returns false.
  }

  Vec2i Runtime::getWindowPadding() const
  {
    return math::make<Vec2i>(ImGui::GetStyle().WindowPadding);
  }

  Vec2i Runtime::getItemSpacing() const
  {
    return math::make<Vec2i>(ImGui::GetStyle().ItemSpacing);
  }

} // namespace gui