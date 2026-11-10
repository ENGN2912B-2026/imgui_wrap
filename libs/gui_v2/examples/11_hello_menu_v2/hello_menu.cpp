//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Application.hpp>
#include <gui2/Menu.hpp>
#include <gui2/Dialog.hpp>
#include <gui2/Layout.hpp>
#include <gui2/Button.hpp>
#include <gui2/Separator.hpp>

// A menu item can call a function or toggle a boolean.
int main(int argc, char** argv)
{
  using namespace gui2;

  Application app{"Hello menu v2"};
  Window& window = app.getWindow();
  bool showMessage = true;

  Dialog about{"About"};
  about.setSize({600, 170});
  about.setCentered(true);
  about.setResizable(false);
  about.setNoScrollbar(true);
  about.setContent(VStack{
    "Hello menu v2\nAn example of menus and a modal dialog.",
    Separator{},
    HBox{
      Stretch{1, Empty{}},
      Fixed{120, Button{"OK", [&] { about.close(); }}.setDefaultFocus(true)},
    },
  });
  window.addDialog(about);

  window.setMenuBar(MenuBar{
    Menu{"File",
      MenuItem{"Reset", [&] { showMessage = true; }},
      Separator{},
      MenuItem{"Quit", [&] { app.quit(); }}
    },
    Menu{"View",
      MenuItem{"Show message", &showMessage}
    },
    Menu{"Help",
      MenuItem{"About", [&] { about.open(); }}
    }
  });

  window.setContent([&] {
    return showMessage ? "Hello from the menu example!" : "";
  });

  app.run();
  return 0;
}
