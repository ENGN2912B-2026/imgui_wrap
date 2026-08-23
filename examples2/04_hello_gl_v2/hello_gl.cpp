//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Application.hpp>
#include <gui2/Panel.hpp>
#include <gui2/VBox.hpp>
#include <gui2/HBox.hpp>
#include <gui2/VStack.hpp>
#include <gui2/HStack.hpp>
#include <gui2/Separator.hpp>
#include <gui2/Button.hpp>

#include <gl/gl.h>
#include <gl/FrameBuffer.hpp>

#include <timer/Timer.hpp>

#include <imgui.h> //tmp

#include <print>

using namespace gui2;

class WidgetGL
{
public:
  Rect display(const Runtime& rt, const Rect& rect)
  {
    const Vec2i availableSize{ rect.getAvailableSize() };
    if (!frameBuffer_.isInitialized())
    {
      frameBuffer_.initialize(availableSize, GL_LINEAR);
      startAnimation();
    }
    else if (frameBuffer_.getSize() != availableSize)
    {
      frameBuffer_.setSize(availableSize);
    }

    ImGui::Image(
      (ImTextureID)(intptr_t)frameBuffer_.getTexture(),
      frameBuffer_.getSize().to<float>(),
      ImVec2(0, 1),
      ImVec2(1, 0)
    );

    drawGL_();

    return rect;
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
  gl::FrameBuffer frameBuffer_;
  timer::Timer timer_;
  float angle_ = 0.0f;

  void drawGL_()
  {
    if (frameBuffer_.isInitialized())
    {
      configureViewport_(-1, 1, -1 , 1);

      frameBuffer_.bind();

      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

      drawTriangle_();

      frameBuffer_.unbind();
    }
  }

  void configureViewport_(int x1, int y1, int x2, int y2)
  {
    assert(frameBuffer_.isInitialized());

    //make the viewport occupy the whole canvas
    const Vec2i& canvasSize{ frameBuffer_.getSize() };
    glViewport(0, 0, canvasSize.x, canvasSize.y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //set the projection transformation
    glOrtho(x1, x2, y1, y2, -1, 1);
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

static_assert(Displayable<WidgetGL>, "WidgetGL should be a Displayable");

// This example shows how to integrate OpenGL rendering into a GUI application.
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
  WidgetGL widgetGL;

  // Set the content of the window to include the OpenGL widget and other
  // GUI elements
  window.setContent(
    HBox{
      VBox{
        // Main panel
        Stretch{3, Panel{ VStack{
          "Main Panel",
          Separator{},
          std::ref(widgetGL),
        }}},
        // Bottom panel
        Stretch{1, Panel{ VStack{
          "Bottom Panel",
          Separator{},
          [&]{ return widgetGL.isAnimationRunning()
            ? "Animation is running" : "Animation is stopped"; },
        }}},
      },
      // Side panel
      Fixed{250, Panel{ VStack{
        "Side Panel",
        Separator{},
        [&]{ return widgetGL.isAnimationRunning()
          ? Button{ "Stop Animation", [&]{ widgetGL.stopAnimation(); } }
          : Button{ "Start Animation", [&]{ widgetGL.startAnimation(); } };
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
