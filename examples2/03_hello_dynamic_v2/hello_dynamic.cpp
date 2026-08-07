//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Application.hpp>
#include <gui2/Panel.hpp>
#include <gui2/VBox.hpp>
#include <gui2/HBox.hpp>
#include <gui2/Button.hpp>
#include <gui2/CheckBox.hpp>

#include <print>

// This example demonstrates creation of dynamic content where items are
// added, modified, and removed from the GUI at runtime.

int main(int argc, char** argv)
{
  // Create the application
  gui2::Application app{ "Hello dynamic v2" };

  // Get the window
  gui2::Window& window = app.getWindow();

#define USE_EXAMPLE 1
#if USE_EXAMPLE == 1
  {
    using namespace gui2;

    bool checkBox = false;

    Panel mainPanel{
      VBox {
        "Main Panel",
        HBox{ "Second line left long text with something else", "Second line right long text also here for this test" },
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
#endif

  // Run the application
  app.run();

  // Success
  std::println("Finished successfully!\n");

  return 0;
}
