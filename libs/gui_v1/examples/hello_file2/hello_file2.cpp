//  Copyright (c) 2025 Daniel Moreno. All rights reserved.
//
// This example shows how to show a file open dialog using `Lime2 File Dialog`.
// See:
//   https://github.com/Limeoats/L2DFileDialog/
//
// Example works but has some limitations:
// - Window is not modal, so user can interact with other parts of the GUI while
//   it's open. In particular, if a click happens outside the dialog, it will
//   be send to the main window behind, and it cannot be seen anymore,
//   or closed.
// - Sizes are hardcoded. Defaults were not very good for this example. I have
//   changed a few of them, but most likely they will not fit other users.
//

#include <gui/gui.hpp>

#include <iostream>

#include "L2DFileDialog.h"

class TopFrame : public gui::ChildFrame
{
  std::string filename_;
public:
  void render() override
  {
    ImGui::Text("Top Frame");

    if (!filename_.empty())
    {
      ImGui::Text("Selected file: %s", filename_.c_str());
    }
  }

  void setFilename(const std::string& filename)
  {
    filename_ = filename;
  }
};

class BottomFrame : public gui::ChildFrame
{
public:
  void render() override
  {
    ImGui::Text("Bottom Frame");
  }
};

class LeftFrame : public gui::ChildFrame
{
public:
  void render() override
  {
    ImGui::Text("Left Frame");
  }
};

class MainWindow : public gui::Frame
{
  TopFrame topFrame_;
  BottomFrame bottomFrame_;
  LeftFrame leftFrame_;
public:
  MainWindow() :
    gui::Frame{
      "Hello File!",                  // name
      { 0, 0 },                       // pos
      { 0, 0 },                       // size
      ImGuiWindowFlags_NoDecoration | // windowFlags
      ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_MenuBar
    },
    topFrame_{},
    bottomFrame_{},
    leftFrame_{}
  {
    // empty
  }

  void render() override
  {
    static char* file_dialog_buffer = nullptr;
    static char path[500] = "";

    // Render the menu bar
    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("Open"))
        {
          file_dialog_buffer = path;
          FileDialog::file_dialog_open = true;
          FileDialog::file_dialog_open_type = FileDialog::FileDialogType::OpenFile;
          topFrame_.setFilename("");
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    if (FileDialog::file_dialog_open) {
      FileDialog::ShowFileDialog(
        &FileDialog::file_dialog_open,
        file_dialog_buffer,
        sizeof(file_dialog_buffer),
        FileDialog::file_dialog_open_type);
        topFrame_.setFilename(file_dialog_buffer);
    }

    gui::VerticalSizer verticalSizer;

    verticalSizer.addChild(&topFrame_, 3);
    verticalSizer.addChild(&bottomFrame_);

    gui::HorizontalSizer horizontalSizer;
    horizontalSizer.addWithFixedSize(&leftFrame_, 200);
    horizontalSizer.addChild(&verticalSizer, 3);

    horizontalSizer.setSize(getContentSize());
    horizontalSizer.setPosition(getContentMin());
    horizontalSizer.draw();
  }
};

int main(int argc, char** argv)
{
  // Create the application
  gui::Application app{ "GUI: Hello File!" };

  // Create the main window
  MainWindow mainWindow;

  // Run the application
  app.run();

  return 0;
}
