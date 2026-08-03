//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Runtime.hpp>

#include "../gui/impl/Backend.hpp"
#include <gui2/Panel.hpp>
#include <gui2/VBox.hpp>
#include <gui2/HBox.hpp>
#include <gui2/Button.hpp>

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

    //------------------------------------------------------------------------
    // Helper functions ------------------------------------------------------
    //------------------------------------------------------------------------
    void computeItemLayouts_(
      const std::vector<BoxItem>& items,
      int currentPosition,
      int availableSize,
      int itemSpacing,
      std::vector<int>& sizes,
      std::vector<int>& positions)
    {
      // Size of weighted children
      // -------------------------
      // size = childSize + space + ... + childSize
      // size = space * (numChildren - 1) + childSize * totalWeight
      // childSize = (size - space * (numChildren - 1)) / totalWeight

      const auto hasWeight = [](const BoxItem& item) {
        return std::holds_alternative<Weight>(item.sizeOrWeight);
      };
      const auto hasSize = [](const BoxItem& item) {
        return std::holds_alternative<Pixels>(item.sizeOrWeight);
      };
      const auto getWeight = [](const BoxItem& item) {
        return std::get<Weight>(item.sizeOrWeight).value;
      };
      const auto getSize = [](const BoxItem& item) {
        return std::get<Pixels>(item.sizeOrWeight).value;
      };

      sizes.clear();
      sizes.reserve(items.size());

      positions.clear();
      positions.reserve(items.size());

      int totalWeight{ 0 }, fixedSize{ 0 }, lastWeightedIndex{ 0 };
      const int numItems{ static_cast<int>(items.size()) };
      for (int i = 0; i < numItems; ++i)
      {
        if (hasWeight(items[i]))
        {
          totalWeight += getWeight(items[i]);
          lastWeightedIndex = i;
        }
        else if (hasSize(items[i]))
        {
          fixedSize += getSize(items[i]);
        }
        else
        {
          throw std::logic_error(
            "VBox item must have either a weight or a fixed size");
        }
      }

      if (totalWeight <= 0)
      { // if totalWeight is 0, all children have fixed size,
        //  we set to 1 to avoid division by zero
        totalWeight = 1;
      }

      const int adjustSize{ availableSize - fixedSize };
      const int itemSize{ std::max(0,
        (adjustSize - (numItems - 1) * itemSpacing) / totalWeight) };
      const int leftOverSize{ adjustSize
        - (numItems - 1) * itemSpacing - totalWeight * itemSize };

      // Apply to children
      int pos{ currentPosition };
      for (int i = 0; i < numItems; ++i)
      {
        const auto& item{ items[i] };
        int size{ hasSize(item) ? getSize(item) : getWeight(item) * itemSize };
        if (i == lastWeightedIndex)
        {
          size += leftOverSize;
        }
        int newSize = size;
        int newPos = pos + size + itemSpacing;
        sizes.push_back(newSize);
        positions.push_back(pos);
        pos = newPos;
      }
    }

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

  void Runtime::display(const std::string& text, const OptionalSize& displaySize) const
  {
    ImGui::Text("%s", text.c_str());
  }

  void Runtime::display(const Panel& panel, const OptionalSize& displaySize) const
  {
    ImVec2 size{0, 0};
    if (displaySize.has_value())
    {
      size = displaySize.value().to<float>();
    }
    int flags = kDefaultImGuiChildWindowFlags;
    ImVec2 currentPosition{ ImGui::GetCursorScreenPos() };
    ImGui::SetNextWindowPos(currentPosition);
    if (ImGui::BeginChild(panel.getName().c_str(), size, flags))
    {
      panel.displayContent(*this);
    }
    ImGui::EndChild(); // For child windows `EndChild()` must be called even
                       // if `BeginChild()` returns false.
  }

  void Runtime::display(const VBox& vbox, const OptionalSize& displaySize) const
  {
    // Get the current cursor position and the available size
    ImVec2 currentPosition{ ImGui::GetCursorScreenPos() };
    ImVec2 availableSize{ ImGui::GetContentRegionAvail() };
    ImVec2 itemSpacing{ ImGui::GetStyle().ItemSpacing };

    if (displaySize.has_value())
    {
      availableSize = displaySize.value().to<float>();
    }

    // Compute the sizes and positions of the VBox items
    const auto& items{ vbox.getItems() };
    std::vector<int> sizes, positions;
    computeItemLayouts_(
      items, currentPosition.y, availableSize.y, itemSpacing.y,
      sizes, positions);

    // Display the items
    const int numItems{ static_cast<int>(vbox.getItems().size()) };
    for (int i = 0; i < numItems; ++i)
    {
      currentPosition.y = static_cast<float>(positions[i]);
      ImGui::SetCursorScreenPos(currentPosition);

      Vec2i itemSize{ static_cast<int>(availableSize.x), sizes[i] };
      items[i].widget.display(*this, itemSize);
    }
  }

  void Runtime::display(const HBox& hbox, const OptionalSize& displaySize) const
  {
    // Get the current cursor position and the available size
    ImVec2 currentPosition{ ImGui::GetCursorScreenPos() };
    ImVec2 availableSize{ ImGui::GetContentRegionAvail() };
    ImVec2 itemSpacing{ ImGui::GetStyle().ItemSpacing };

    if (displaySize.has_value())
    {
      availableSize = displaySize.value().to<float>();
    }

    // Compute the sizes and positions of the HBox items
    const auto& items{ hbox.getItems() };
    std::vector<int> sizes, positions;
    computeItemLayouts_(
      items, currentPosition.x, availableSize.x, itemSpacing.x,
      sizes, positions);

    // Display the items
    const int numItems{ static_cast<int>(hbox.getItems().size()) };
    for (int i = 0; i < numItems; ++i)
    {
      currentPosition.x = static_cast<float>(positions[i]);
      ImGui::SetCursorScreenPos(currentPosition);

      Vec2i itemSize{ sizes[i], static_cast<int>(availableSize.y) };
      items[i].widget.display(*this, itemSize);
    }
  }

  void Runtime::display(const Button& button, const OptionalSize& displaySize) const
  {
    if (ImGui::Button(button.getLabel().c_str()))
    {
      button.onClick();
    }
  }
}