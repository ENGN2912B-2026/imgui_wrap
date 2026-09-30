//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Runtime.hpp>

#include <backend/Backend.hpp>
#include <gui2/Empty.hpp>
#include <gui2/Separator.hpp>
#include <gui2/Image.hpp>
#include <gui2/Button.hpp>
#include <gui2/CheckBox.hpp>
#include <gui2/TextBox.hpp>
#include <gui2/Panel.hpp>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <imgui_zoomable_image.h>

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
      ImGuiChildFlags_None
    };

    inline Rect getItemRect_()
    {
      ImVec2 pos = ImGui::GetItemRectMin();
      ImVec2 size = ImGui::GetItemRectSize();
      return { math::make<Vec2i>(pos).cast<int>(),
               math::make<Vec2i>(size).cast<int>() };
    }

    inline void updateSize_(std::vector<std::pair<std::string, Rect>>& sizingStack,
                            const Rect& rect)
    {
      for (auto& [id, r] : sizingStack)
      {
        r.unite(rect);
      }
    }

  } // anonymous namespace

  Runtime& Runtime::getInstance()
  {
    static Runtime instance;
    return instance;
  }

  void Runtime::initialize(const std::string& windowTitle,
                           const Vec2i& windowSize)
  {
    if (isInitialized())
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
    backend_ = backend::Backend::create();
    backend_->DpiAware = true;
    backend_->SrgbFramebuffer = false;
    backend_->Vsync = true;
    backend_->ClearColor = ImVec4(0.120f, 0.120f, 0.120f, 1.000f);
    backend_->InitCreateWindow(windowTitle.c_str(), windowSize.to<float>());
    backend_->InitBackends();
  }

  void Runtime::uninitialize()
  {
    if (!isInitialized())
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

    // Update the sized rectangles for all active session and clear sizing stack.
    for (const auto& [id, rect] : sizingStack_)
    {
      sizedRects_[id] = rect;
    }
    sizingStack_.clear();
  }

  Rect Runtime::display(const Empty& empty, const Rect& rect) const
  { // Display an empty rectangle.
    ImGui::SetCursorScreenPos(rect.origin.to<float>());
    Vec2i size{ rect.size };
    if (!rect.hasWidth()) { size.x = 0; }
    if (!rect.hasHeight()) { size.y = 0; }
    ImGui::Dummy(size.to<float>());
    return getItemRect_();
  }

  Rect Runtime::display(const std::string& text, const Rect& rect) const
  {
    ImGui::SetCursorScreenPos(rect.origin.to<float>());
    if (rect.hasWidth())
    {
      ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + rect.size.x);
    }
    ImGui::Text("%s", text.c_str());
    if (rect.hasWidth())
    {
      ImGui::PopTextWrapPos();
    }
    return getItemRect_();
  }

  Rect Runtime::display(const Separator& separator, const Rect& rect) const
  {
    ImGui::SetCursorScreenPos(rect.origin.to<float>());
    ImGui::Separator();
    return getItemRect_();
  }

  Rect Runtime::display(const Button& button, const Rect& rect) const
  {
    ImGui::SetCursorScreenPos(rect.origin.to<float>());
    if (ImGui::Button(button.getLabel().c_str()))
    {
      button.onClick();
    }
    return getItemRect_();
  }

  Rect Runtime::display(const Image& image, const Rect& rect) const
  {
    ImGui::SetCursorScreenPos(rect.origin.to<float>());
    if (const auto textureId = image.getTextureId(); textureId > 0)
    {
      ImGui::Image(
        (ImTextureID)(intptr_t)textureId,
        rect.getAvailableSize().to<float>()); // we need an actual size for the image
      return getItemRect_();
    }
    // If the image is not valid (textureId <= 0), we display an empty rectangle.
    return display(Empty{}, rect);
  }

  Rect Runtime::display(CheckBox& checkBox, const Rect& rect) const
  {
    ImGui::SetCursorScreenPos(rect.origin.to<float>());
    bool checked = checkBox.isChecked();
    if (ImGui::Checkbox(checkBox.getLabel().c_str(), &checked))
    {
      checkBox.setChecked(checked);
    }
    return getItemRect_();
  }

  Rect Runtime::display(TextBox& textBox, const Rect& rect) const
  {
    ImGui::SetCursorScreenPos(rect.origin.to<float>());
    ImGui::SetNextItemWidth(rect.getAvailableSize().x);

    std::string placeholder;
    std::string id = "##" + textBox.getId();
    std::string* text = textBox.getTextPtr() ? textBox.getTextPtr() : &placeholder;

    Actions actions = textBox.takeActions();
    if (contains(actions, Actions::Focus))
    {
      ImGui::SetKeyboardFocusHere();
    }

    // Disable the "Live Edit on Input" flag to prevent the text box from being
    // updated on every keystroke.
    ImGui::PushItemFlag(ImGuiItemFlags_LiveEditOnInput, false);

    if (ImGui::InputText(id.c_str(), text))
    { // Execute the callback function when the user presses the Enter key or
      // when the text box loses focus.
      textBox.onEditFinished();
    }

    // Restore the previous item flags
    ImGui::PopItemFlag();

    return getItemRect_();
  }

  Rect Runtime::display(ImageZoom& imageZoom, const Rect& rect) const
  {
    ImGui::SetCursorScreenPos(rect.origin.to<float>());
    static_assert(sizeof(ImGuiImage::State) == sizeof(ImageZoom::ZoomState),
                  "Size of ImGuiImage::State and ImageZoom::ZoomState must be equal");
    ImGuiImage::Zoomable(
      imageZoom.getTextureId(),
      rect.getAvailableSize().to<float>(),
      reinterpret_cast<ImGuiImage::State*>(&imageZoom.getZoomState()));
    return getItemRect_();
  }

  Rect Runtime::display(Panel& panel, const Rect& rect) const
  {
    ImGui::SetNextWindowPos(rect.origin.to<float>());
    Actions actions = panel.takeActions();
    int flags = kDefaultImGuiChildWindowFlags;
    if (panel.getDrawBorder())
    {
      flags |= ImGuiChildFlags_Borders;
    }
    if (ImGui::BeginChild(panel.getId().c_str(),
                          rect.getAvailableSize().to<float>(),
                          flags))
    {
      // Must call `ImGui::GetCursorScreenPos()` to get an initial position for
      // the inner rectangle, which already takes into account the window padding
      // and the current scroll position.
      const auto origin{ math::make<math::Vec2d>(ImGui::GetCursorScreenPos()) };
      const auto size{ math::make<math::Vec2d>(ImGui::GetContentRegionAvail()) };

      const Rect innerRect{ origin.cast<int>(), size.cast<int>() };
      const Rect contentActualRect = panel.displayContent(*this, innerRect);

      if (!contentActualRect.isEmpty())
      {
        // Make ImGui aware of the complete content extent produced by our layout
        // system, so that the child window can be scrolled to show all content.
        ImGui::SetCursorScreenPos(contentActualRect.end().to<float>());
        ImGui::Dummy(ImVec2{0, 0});

        // Size the actual rectangle if requested
        const auto padding{ innerRect.origin - rect.origin };
        const Rect sizeRect{ rect.origin, contentActualRect.size + 2 * padding };
        updateSize_(sizingStack_, sizeRect);
      }

      if (contains(actions, Actions::ScrollToEnd))
      {
        ImGui::SetScrollHereY();
      }
    }

    // Read the actual rectangle of the child window, which may be different
    // from the requested rectangle.
    // Must do this before calling `ImGui::EndChild()`, because `GetWindowPos()`
    // and `GetWindowSize()` return the position and size of the current window,
    // which is the child window only between `BeginChild()` and `EndChild()`.
    const Rect actualRect{
        math::make<math::Vec2d>(ImGui::GetWindowPos()).cast<int>(),
        math::make<math::Vec2d>(ImGui::GetWindowSize()).cast<int>()
    };

    ImGui::EndChild(); // For child windows `EndChild()` must be called even
                       // if `BeginChild()` returns false.

    return actualRect;
  }

  void Runtime::sizeBegin(const std::string& id) const
  {
    sizingStack_.emplace_back(id, Rect::empty());
  }

  void Runtime::sizeEnd() const
  {
    if (sizingStack_.empty())
    {
      throw std::runtime_error("No sizing session is active");
    }
    const auto& [id, rect] = sizingStack_.back();
    if (!rect.isEmpty())
    {
      sizedRects_[id] = rect;
    }
    sizingStack_.pop_back();
  }

  Rect Runtime::getSizedRect(const std::string& id) const
  {
    auto it = sizedRects_.find(id);
    if (it != sizedRects_.end())
    {
      return it->second;
    }
    return Rect::empty();
  }

  Vec2i Runtime::getWindowPadding() const
  {
    return math::make<Vec2i>(ImGui::GetStyle().WindowPadding);
  }

  Vec2i Runtime::getFramePadding() const
  {
    return math::make<Vec2i>(ImGui::GetStyle().FramePadding);
  }

  Vec2i Runtime::getItemSpacing() const
  {
    return math::make<Vec2i>(ImGui::GetStyle().ItemSpacing);
  }

  size_t Runtime::getFontSize() const
  {
    return static_cast<size_t>(ImGui::GetFontSize());
  }

  //! \brief Gets a reference to the backend used by the runtime.
  //! \return A reference to the backend used by the runtime.
  backend::Backend& Runtime::getBackend() const
  {
    if (backend_ == nullptr)
    {
      throw std::runtime_error("Runtime is not initialized");
    }
    return *backend_;
  }

} // namespace gui
