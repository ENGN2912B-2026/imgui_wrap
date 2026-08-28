//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Application.hpp>
#include <gui2/Panel.hpp>
#include <gui2/Layout.hpp>
#include <gui2/Separator.hpp>
#include <gui2/Button.hpp>

#include <gui2/WidgetGL.hpp>

#include <timer/Timer.hpp>

#include <print>

// This example shows how to integrate OpenGL rendering into a GUI application.
//
// It demonstrates how to create a custom widget that uses OpenGL to render
// content and how to display that content within a GUI panel. The example also
// includes a timer to animate the OpenGL content, and buttons to start and stop
// the animation.
//

using namespace gui2;

class ViewerGL : public WidgetGL
{
public:
  virtual void initialize(const Vec2i& availableSize) override
  {
    WidgetGL::initialize(availableSize);
    startAnimation();
  }

  virtual void drawGL() override
  {
    configureViewport_(-1, -1, 1, 1);
    WidgetGL::drawGL();
    drawTriangle_();
  }

  bool isAnimationRunning() const
  {
    return timer_.isRunning();
  }

  void startAnimation()
  {
    if (!timer_.isRunning())
    {
      timer_.start(100,
        [this]
        { // timer callback
          angle_ += 7.2f;
          if (angle_ >= 360.0f)
          {
            angle_ -= 360.0f;
          }
        }
      );
    }
  }

  void stopAnimation()
  {
    if (timer_.isRunning())
    {
      timer_.stop();
    }
  }

private:
  timer::Timer timer_;
  float angle_ = 0.0f;

  void configureViewport_(int x1, int y1, int x2, int y2)
  {
    assert(frameBuffer_.isInitialized());

    //make the viewport occupy the whole canvas
    const Vec2i& canvasSize{ frameBuffer_.getSize() };
    glViewport(0, 0, canvasSize.x, canvasSize.y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //set the projection transformation
    glOrtho(x1, x2, y2, y1, -1, 1); // flip y-axis to match GUI coordinates
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }

  void drawTriangle_()
  {
    const float factor_{ std::cos(angle_ * 3.14159f / 180.0f) };
    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0); glVertex3f(-0.6f * factor_, -0.75f, 0.0f);
    glColor3f(0, 1, 0); glVertex3f( 0.6f * factor_, -0.75f, 0.0f);
    glColor3f(0, 0, 1); glVertex3f( 0.0f * factor_,  0.75f, 0.0f);
    glEnd();
  }
};

int main(int argc, char** argv)
{
  using namespace gui2;

  // Title of the window
  std::string title{"GUI: Hello GL v2"};

  // Create the application
  Application app{ title };

  // Get the window
  Window& window = app.getWindow();

  // Create the OpenGL widget
  ViewerGL viewerGL;

  // Set the content of the window to include the OpenGL widget and other
  // GUI elements
  window.setContent(
    HBox{
      VBox{
        // Main panel
        Stretch{3, Panel{ VStack{
          "Main Panel",
          Separator{},
          std::ref(viewerGL),
        }}},
        // Bottom panel
        Stretch{1, Panel{ VStack{
          "Bottom Panel",
          Separator{},
          [&]{ return viewerGL.isAnimationRunning()
            ? "Animation is running" : "Animation is stopped"; },
        }}},
      },
      // Side panel
      Fixed{250, Panel{ VStack{
        "Side Panel",
        Separator{},
        [&]{ return viewerGL.isAnimationRunning()
          ? Button{ "Stop Animation", [&]{ viewerGL.stopAnimation(); } }
          : Button{ "Start Animation", [&]{ viewerGL.startAnimation(); } };
        },
      }}},
    }
  );

  // Run the application
  app.run();

  // Success
  std::println("Finished successfully!\n");

  return 0;
}
