//  Copyright (c) 2024-2025 Daniel Moreno. All rights reserved.
//

#include <gui/Application.hpp>
#include <gui/Frame.hpp>
#include <imgui.h>

#include <print> // Check if C++23 is working

class MainWindow : public gui::Frame
{
public:
  void render() override
  {
    ImGui::Text("Hello World!");
  }
};

int main(int argc, char** argv)
{
  // Create the application
  gui::Application app{ "GUI: Hello World!" };

  // Create the main window
  MainWindow mainWindow;

  // Run the application
  app.run();

  // Success
  std::println("Finished successfully!\n");

  return 0;
}
