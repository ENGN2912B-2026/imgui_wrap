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

#include <print>

// This example demonstrates creation of dynamic content where items are
// added, modified, and removed from the GUI at runtime.

int main(int argc, char** argv)
{
  // Create the application
  gui2::Application app{ "Hello dynamic v2" };

  // Get the window
  gui2::Window& window = app.getWindow();

#define USE_EXAMPLE 2
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
  }
#endif

  // Run the application
  app.run();

  // Success
  std::println("Finished successfully!\n");

  return 0;
}
