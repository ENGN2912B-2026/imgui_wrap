//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Application.hpp>

#include <print>

int main(int argc, char** argv)
{
  // Create the application
  gui2::Application app{ "Hello GUI v2" };

  // Get the window
  gui2::Window& window = app.getWindow();
  window.setContent("Hello World!");

  // Run the application
  app.run();

  // Success
  std::println("Finished successfully!\n");

  return 0;
}
