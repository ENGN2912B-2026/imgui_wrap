//  Copyright (c) 2024-2025 Daniel Moreno. All rights reserved.
//

#include <gui/Application.hpp>
#include <gui/Frame.hpp>
#include <imgui.h>

#if __cpp_lib_print >= 202207L
# include <print> // Use C++23 std::println
# define HAS_PRINT
#else
# include <iostream>
#endif

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
#ifdef HAS_PRINT
  std::println("Finished successfully!\n");
#else
  std::cout << "Finished successfully!\n";
#endif

  return 0;
}
