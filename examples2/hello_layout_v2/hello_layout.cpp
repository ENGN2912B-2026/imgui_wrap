//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Application.hpp>
#include <gui2/Panel.hpp>
#include <gui2/VBox.hpp>
#include <gui2/HBox.hpp>
#include <gui2/Button.hpp>

#include <print>

// This example demonstrates the use of VBox and HBox layout containers to
// create a simple GUI layout with panels.

int main(int argc, char** argv)
{
  // Create the application
  gui2::Application app{ "Hello Layout v2" };

  // Get the window
  gui2::Window& window = app.getWindow();

#define USE_EXAMPLE_LAYOUT 3
#if USE_EXAMPLE_LAYOUT == 1
  gui2::Panel leftPanel{ "leftPanel", "Left Panel" };
  gui2::Panel topPanel{ "topPanel", "Top Panel" };
  gui2::Panel bottomPanel{ "bottomPanel", "Bottom Panel" };

  gui2::HBox hbox{
    gui2::Fixed{200, std::move(leftPanel)},
    gui2::VBox{
      gui2::Stretch{3, std::move(topPanel)},
      gui2::Stretch{1, std::move(bottomPanel)},
    },
  };

  window.setContent(std::move(hbox));
#elif USE_EXAMPLE_LAYOUT == 2
  {
    using namespace gui2;
    window.setContent(
      HBox{
        Fixed{200, Panel{ "leftPanel", "Left Panel" }},
        VBox{
          Stretch{3, Panel{ "topPanel", "Top Panel" }},
          Stretch{1, Panel{ "bottomPanel", "Bottom Panel" }},
        },
      }
    );
  }
#elif USE_EXAMPLE_LAYOUT == 3
  {
    using namespace gui2;
    Panel mainPanel{ "mainPanel",
      VBox {
        "Main Panel",
        HBox{ "Second line left", "Second line right" },
        Button{ "Click Me", []() { std::println("Button clicked!"); } },
      }
    };

    window.setContent(
      HBox{
        Fixed{200, Panel{ "leftPanel", "Left Panel" }},
        VBox{
          Stretch{3, std::move(mainPanel)},
          Stretch{1, Panel{ "bottomPanel", "Bottom Panel" }},
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
