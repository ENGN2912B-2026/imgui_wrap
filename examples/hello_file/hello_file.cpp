//  Copyright (c) 2025 Daniel Moreno. All rights reserved.
//
// This example shows how to show a file open dialog using `imfilebrowser`.
// See:
//   https://github.com/AirGuanZ/imgui-filebrowser
//

#include <gui/gui.hpp>

#include <iostream>

#include "imfilebrowser.h"

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
  ImGui::FileBrowser fileDialog_;
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
    leftFrame_{},
    fileDialog_{}
  {
    fileDialog_.SetTitle("Select a File");
    fileDialog_.SetTypeFilters({ ".h", ".c", ".hpp", ".cpp" });
  }

  void render() override
  {
    // Render the menu bar
    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("Open"))
        {
          fileDialog_.Open();
          topFrame_.setFilename("");
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    fileDialog_.Display();
    if(fileDialog_.HasSelected())
    {
      topFrame_.setFilename(fileDialog_.GetSelected().string());
      fileDialog_.ClearSelected();
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
