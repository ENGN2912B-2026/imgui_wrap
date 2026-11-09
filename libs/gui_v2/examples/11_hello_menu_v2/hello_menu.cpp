//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Application.hpp>
#include <gui2/Menu.hpp>
#include <gui2/Separator.hpp>

// A menu item can call a function or toggle a boolean.
int main(int argc, char** argv)
{
  using namespace gui2;

  Application app{"Hello menu v2"};
  Window& window = app.getWindow();
  bool showMessage = true;

  window.setMenuBar(MenuBar{
    Menu{"File",
      MenuItem{"Reset", [&] { showMessage = true; }},
      Separator{},
      MenuItem{"Quit", [&] { app.quit(); }}
    },
    Menu{"View",
      MenuItem{"Show message", &showMessage}
    }
  });

  window.setContent([&] {
    return showMessage ? "Hello from the menu example!" : "";
  });

  app.run();
  return 0;
}
