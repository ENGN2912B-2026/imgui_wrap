//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Application.hpp>
#include <gui2/Panel.hpp>

#include <print>

int main(int argc, char** argv)
{
  // Create the application
  gui2::Application app{ "Hello Sizer v2" };

  // Get the window
  gui2::Window& window = app.getWindow();

  gui2::Panel topPanel{ "Top Panel" };
  gui2::Panel bottomPanel{ "Bottom Panel" };
  gui2::Panel rightPanel{ "Right Panel" };

  window.setContent(std::move(topPanel));

  // window.setContent(
  //   gui2::VBox{
  //     std::string{"Item 1"},
  //     std::string{"Item 2"}
  //   }
  // );

  // Run the application
  app.run();

  // Success
  std::println("Finished successfully!\n");

  return 0;
}
