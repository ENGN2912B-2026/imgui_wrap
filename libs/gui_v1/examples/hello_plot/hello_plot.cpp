//  Copyright (c) 2024 Daniel Moreno. All rights reserved.
//

#include <gui/gui.hpp>

#include <cmath>

// Data for the bar plot
constexpr int kNumBars{ 11 };
float bar_x[kNumBars]{ 0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f };
float bar_y[kNumBars]{ 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f, 0.8f };
constexpr double kBarSize{ 0.08 };

// Data for the line plot
constexpr int kNumPts{ 1000 };
float x_data[1000];
float y_data[1000];
constexpr float PI = 3.14159265359f;


class TopFrame : public gui::ChildFrame
{
public:
  void render() override
  {
    ImGui::Text("Top Frame");

    if (ImPlot::BeginPlot("My Plot", ImVec2(-1, -1)))
    {
      ImPlot::PlotBars("My Bar Plot", bar_x, bar_y, kNumBars, kBarSize);
      ImPlot::PlotLine("My Line Plot", x_data, y_data, kNumPts);
      ImPlot::EndPlot();
    }

#if 0
    if (ImPlot::BeginPlot("##Plot",0,0,ImVec2(-1,-1),0,ImPlotAxisFlags_NoInitialFit,ImPlotAxisFlags_NoInitialFit))
    {
      constexpr int kNumPts{ 1000 };
      ImPlotRect limits{ ImPlot::GetPlotLimits() };
      ImPlot::SetNextLineStyle(ImVec4(1,1,0,1));
      ImPlot::PlotLineG("##item",
        [](int idx, void* data)
        {
          const ImPlotRect& limits{ *reinterpret_cast<ImPlotRect*>(data) };
          const double x{ (idx / 1000.0 - limits.X.Min) / (limits.X.Max - limits.X.Min) };
          const double y{ 0.5 * std::sin(x * 2 * 3.14159) + 0.5};
          return ImPlotPoint(x,y);
        },
        &limits,
        kNumPts);
      ImPlot::EndPlot();
    }
#endif
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
  // Populate x_data and y_data
  for (int i = 0; i < kNumPts; ++i)
  {
    x_data[i] = i / static_cast<float>(kNumPts);
    y_data[i] = 0.5f * std::sin(2*PI * x_data[i]) + 0.5f;
  }

  // Create the application
  gui::Application app{ "GUI: Hello Plot!" };

  // Create the main window
  MainWindow mainWindow;

  // Run the application
  app.run();

  return 0;
}
