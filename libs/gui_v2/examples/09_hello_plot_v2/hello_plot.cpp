//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Application.hpp>
#include <gui2/Panel.hpp>
#include <gui2/Layout.hpp>
#include <gui2/Separator.hpp>
#include <gui2/Button.hpp>

#include <print>
#include <cmath>

#include <implot.h>

// This example shows how to create a simple plot with both bar and line plots
//  using ImPlot, and display it in a GUI window.

using namespace gui2;

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

class PlotWidget
{
public:
  Rect display(const Runtime& rt, const Rect& rect)
  {
    using namespace ImGui;
    if (ImPlot::BeginPlot("My Plot", ImVec2(-1, -1)))
    {
      ImPlot::PlotBars("My Bar Plot", bar_x, bar_y, kNumBars, kBarSize);
      ImPlot::PlotLine("My Line Plot", x_data, y_data, kNumPts);
      ImPlot::EndPlot();
    }
    return rect;
  }
};

static_assert(Displayable<PlotWidget>, "PlotWidget must be Displayable");

int main(int argc, char** argv)
{
  // Populate x_data and y_data
  for (int i = 0; i < kNumPts; ++i)
  {
    x_data[i] = i / static_cast<float>(kNumPts);
    y_data[i] = 0.5f * std::sin(2*PI * x_data[i]) + 0.5f;
  }

  // Title of the window
  std::string title{"GUI: Hello plot v2"};

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
          PlotWidget{},
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

  return 0;
}
