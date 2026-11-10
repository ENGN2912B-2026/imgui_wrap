//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Application.hpp>
#include <gui2/Menu.hpp>
#include <gui2/ModalDialog.hpp>
#include <gui2/Overlay.hpp>
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

  ModalDialog about{"About", VStack{
    "Hello menu v2\nAn example of menus and a modal dialog.",
    Separator{},
    HBox{
      Stretch{1, Empty{}},
      Fixed{120, Button{"OK", [&] { about.setOpen(false); }}.setDefaultFocus(true)},
    },
  }};
  about.setSize({600, 180});

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
      MenuItem{"About", [&] { about.setOpen(true); }}
    }
  });

  window.setContent(Overlay{
    [&] { return showMessage ? "Hello from the menu example!" : ""; },
    std::ref(about)
  });

  app.run();
  return 0;
}
