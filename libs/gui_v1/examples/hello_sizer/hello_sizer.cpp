//  Copyright (c) 2024 Daniel Moreno. All rights reserved.
//

#include <gui/gui.hpp>

class TopFrame : public gui::ChildFrame
{
public:
  void render() override
  {
    ImGui::Text("Top Frame");
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
public:
  void render() override
  {
    gui::VerticalSizer verticalSizer;
    TopFrame topFrame;
    BottomFrame bottomFrame;
    verticalSizer.addChild(&topFrame, 3);
    verticalSizer.addChild(&bottomFrame);

    gui::HorizontalSizer horizontalSizer;
    LeftFrame leftFrame;
    horizontalSizer.addWithFixedSize(&leftFrame, 200);
    horizontalSizer.addChild(&verticalSizer, 3);

    horizontalSizer.setSize(getContentSize());
    horizontalSizer.setPosition(getContentMin());
    horizontalSizer.draw();
  }
};

int main(int argc, char** argv)
{
  // Create the application
  gui::Application app{ "GUI: Hello Sizer!" };

  // Create the main window
  MainWindow mainWindow;

  // Run the application
  app.run();

  return 0;
}
