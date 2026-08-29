//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Application.hpp>
#include <gui2/Panel.hpp>
#include <gui2/Layout.hpp>
#include <gui2/Separator.hpp>
#include <gui2/Button.hpp>

#include <print>

#include <nfd.h>

// This example shows how to create an image, fill it with a gradient pattern,
// and display it in a GUI window using `gl::Texture` and `gui2::Image`.
//

using namespace gui2;

bool open_file_dialog(std::string& outFilename)
{
  nfdu8char_t *outPath;
  nfdu8filteritem_t filters[2] = { { "Source code", "c,cpp,cc" }, { "Headers", "h,hpp" } };
  nfdopendialogu8args_t args = {0};
  args.filterList = filters;
  args.filterCount = 2;
  nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
  if (result == NFD_OKAY)
  {
    puts("Success!");
    puts(outPath);
    outFilename = outPath;
    NFD_FreePathU8(outPath);
    return true;
  }
  else if (result == NFD_CANCEL)
  {
    puts("User pressed cancel.");
  }
  else
  {
    printf("Error: %s\n", NFD_GetError());
  }
  return false;
}

int main(int argc, char** argv)
{
  // Initialize Native File Dialog
  NFD_Init();

  // Title of the window
  std::string title{"GUI: Hello file v2"};

  // Create the application
  Application app{ title };

  // Get the window
  Window& window = app.getWindow();

  std::string filename = "<No file selected>";

  // Set the content of the window
  window.setContent(
    HBox{
      VBox{
        // Main panel
        Stretch{3, Panel{ VStack{
          "Main Panel",
          Separator{},
          HStack{"File: ", std::ref(filename)},
          Button{ "Open File", [&]{
            std::string selectedFilename;
            if (open_file_dialog(selectedFilename))
            { // User selected a file, update the filename variable
              filename = selectedFilename;
            }
          }},
        }}},
        // Bottom panel
        Stretch{1, Panel{ "Bottom Panel" }},
      },
      // Side panel
      Fixed{250, Panel{ "Side Panel"}},
    }
  );

  // Run the application
  app.run();

  // Success
  std::println("Finished successfully!\n");

  // Uninitialize Native File Dialog
  NFD_Quit();

  return 0;
}
