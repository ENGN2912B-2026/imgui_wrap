//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Application.hpp>
#include <gui2/Panel.hpp>
#include <gui2/Layout.hpp>
#include <gui2/TextBox.hpp>

#include <print>

// This example shows how to create a simple GUI application with a scrollable
//   text area and a text input box. The user can type text into the input box,
//   and when they press Enter, the text is added to the scrollable area above.

using namespace gui2;


int main(int argc, char** argv)
{
  // Title of the window
  std::string title{"GUI: Hello scroll v2"};

  // Create the application
  Application app{ title };

  // Get the window
  Window& window = app.getWindow();

  // Content widget
  std::string content;
  Panel contentRegion{ std::cref(content) };

  // TextBox to input new lines of text
  std::string newLine;
  TextBox textBox{ &newLine,
    [&](){
      content += newLine + "\n";
      newLine.clear();
      contentRegion.addAction(Actions::ScrollToEnd);
      textBox.addAction(Actions::Focus);
    }};

  // Set the content of the window
  window.setContent(
    VBox{
      // Main panel
      Stretch{3, Panel{ VBox{
        "Main Panel",
        std::ref(contentRegion),
      }}},
      // Bottom panel
      Stretch{1, Panel{ VStack{ "Bottom Panel", std::ref(textBox) }}}
    }
  );

  // Run the application
  app.run();

  // Success
  std::println("Finished successfully!\n");

  return 0;
}
