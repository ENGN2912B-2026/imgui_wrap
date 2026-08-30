//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Application.hpp>
#include <gui2/Panel.hpp>
#include <gui2/VBox.hpp>
#include <gui2/HBox.hpp>
#include <gui2/VStack.hpp>
#include <gui2/HStack.hpp>
#include <gui2/Button.hpp>
#include <gui2/CheckBox.hpp>
#include <gui2/Empty.hpp>
#include <gui2/Separator.hpp>

#include <print>
#include <format>
#include <chrono>

// This example demonstrates creation of dynamic content where items are
// added, modified, and removed from the GUI at runtime.

int main(int argc, char** argv)
{
  // Title of the window
  std::string title{"Hello dynamic v2"};

  // Create the application
  gui2::Application app{ title };

  // Get the window
  gui2::Window& window = app.getWindow();

#define USE_EXAMPLE 3
#if USE_EXAMPLE == 1
  {
    using namespace gui2;

    bool checkBox = false;

    Panel mainPanel{
      VBox {
        "Main Panel",
        [&]{ return "Checkbox state: " + std::string(checkBox ? "Checked" : "Unchecked"); },
        Button{ "Click Me", []() { std::println("Button clicked!"); } },
        CheckBox{ "Check me", &checkBox },
      }
    };

    window.setContent(
      HBox{
        Fixed{200, Panel{ "Left Panel" }},
        VBox{
          Stretch{3, std::move(mainPanel)},
          Stretch{1, Panel{ "Bottom Panel" }},
        },
      }
    );

    // Run the application
    app.run();
  }
#elif USE_EXAMPLE == 2
  {
    using namespace gui2;

    bool leftPanelCheckBox = false;
    bool checkBox = false;

    Panel mainPanel{
      VBox {
        "Main Panel",
        [&]{ return "Checkbox state: " + std::string(checkBox ? "Checked" : "Unchecked"); },
        Button{ "Click Me", []() { std::println("Button clicked!"); } },
        CheckBox{ "Check me", &checkBox },
        [&]{ return Button{ "Update title", [&](){ window.setTitle("Hello dynamic v2 - Updated!"); } }; },
        [&]{ return Button{ "Update size", [&](){ window.setSize({800, 600}); } }; },
        [&]{ return Button{ "Toggle left panel checkbox", [&](){ leftPanelCheckBox = !leftPanelCheckBox; } }; },
        [&]{ return checkBox ? Widget{Button{"Click Me"}} : Widget{}; },
      }
    };

    window.setContent(
      HBox{
        Fixed{200, Panel{ VStack{
          "Left Panel",
          "Line 1",
          Empty{},
          "Line 2",
          "",
          "Line 3",
          Widget{},
          "Line 4",
          [&]{return CheckBox{
            std::string(leftPanelCheckBox ? "Checked" : "Unchecked"),
            &leftPanelCheckBox };}
        }}},
        VBox{
          Stretch{3, std::move(mainPanel)},
          Stretch{1, Panel{ "Bottom Panel" }},
        },
      }
    );

    // Run the application
    app.run();
  }
#elif USE_EXAMPLE == 3
  {
    using namespace gui2;

    bool showLeftSide = true;
    bool showSidePanel = true;
    bool showBottomPanel = true;

    bool sidePanelCheckBox = false;

    int counter = 0;
    auto counterButton = [&]{
      return Button{ "Counter: " + std::to_string(counter), [&]{ ++counter; } };
    };
    auto resetButton = Button{ "Reset counter", [&]{ counter = 0; } };

    using Clock = std::chrono::steady_clock;
    auto previous = Clock::now();
    bool showFPS = false;
    double fps = 0.0;

    auto updateFPS = [&]{
      auto now = Clock::now();
      if (showFPS)
      {
        double dt = std::chrono::duration<double>(now - previous).count();
        double currentFPS = 1.0 / dt;
        fps = fps > 0.0 ? fps * 0.99 + currentFPS * 0.01 : currentFPS;
        window.setTitle(title + std::format(" - FPS: {:.1f}", fps));
      }
      previous = now;
    };

    Panel mainPanel{
      VStack {
        "Main Panel",
        Separator{},
        [&]{ updateFPS(); return Empty{}; },
        CheckBox{ "Show side panel", &showSidePanel },
        CheckBox{ "Show bottom panel", &showBottomPanel },
        CheckBox{ "Show FPS", &showFPS },

        [&]{ return Button{
          "Reset title",
          [&](){ window.setTitle(title); } }; },
        [&]{ return Button{
          "Toggle side panel checkbox",
          [&](){ sidePanelCheckBox = !sidePanelCheckBox; } }; },
      }
    };

    window.setContent(
      HBox{
        [&] { return showLeftSide ? Widget{VBox{
          // Main panel is always displayed.
          Stretch{3, std::ref(mainPanel)},
          // Bottom panel is conditionally displayed.
          [&]{ return !showBottomPanel ? Stretch{0} : Stretch{1,
            Panel{ VStack {
              "Bottom Panel",
              Separator{},
              HStack { "Counter Controls", counterButton, resetButton,
              },
            }}};
          },
        }} : Fixed{0}; },
        // Side panel is conditionally displayed.
        [&]{ return !showSidePanel ? Fixed{0} : Fixed{250,
          Panel{ VStack {
            "Side Panel",
            Separator{},
            CheckBox{ "Side checkbox", &sidePanelCheckBox },
            [&]{ return std::string(sidePanelCheckBox ? "Checked" : "Unchecked"); },
            "",
            CheckBox{ "Show left side", &showLeftSide },
          }}};
        },
      }
    );

    // Run the application
    app.run();

    // IMPORTANT: we need to call `app.run()` inside the scope where all the
    //            state variables are defined, otherwise they will be destroyed
    //            before the display loop starts, and all the lambdas that
    //            capture them will have dangling references.
  }
#endif

  // Success
  std::println("Finished successfully!\n");

  return 0;
}
