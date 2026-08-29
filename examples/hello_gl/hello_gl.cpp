//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#include <gl/FrameBuffer.hpp>
#include <gui/gui.hpp>
#include <timer/Timer.hpp>

#include <cassert>
#include <cmath>

class TopFrame : public gui::ChildFrame
{
  gl::FrameBuffer* frameBuffer_;
  timer::Timer timer_;
  float angle_;
public:
  TopFrame() :
    frameBuffer_{ nullptr },
    timer_{},
    angle_{ 0.0f }
  {
    timer_.start(100, [this]()
    {
      angle_ += 7.2f;
      if (angle_ >= 360.0f)
      {
        angle_ -= 360.0f;
      }
    });
  }

  void render() override
  {
    ImGui::Text("Top Frame");

    if (!frameBuffer_)
    {
      frameBuffer_ = new gl::FrameBuffer();
      frameBuffer_->initialize();
    }

    frameBuffer_->setSize(math::make<gui::Vec2i>(ImGui::GetContentRegionAvail()));

    ImGui::Image(
      (ImTextureID)(intptr_t)frameBuffer_->getTexture().getId(),
      frameBuffer_->getSize().to<float>(),
      ImVec2(0, 1),
      ImVec2(1, 0)
    );

    drawGL();
  }

  void drawGL()
  {
    if (frameBuffer_ != nullptr)
    {
      configureViewport(-1, 1, -1 , 1);

      frameBuffer_->bind();

      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

      drawTriangle();

      frameBuffer_->unbind();
    }
  }

  void configureViewport(int x1, int y1, int x2, int y2)
  {
    assert(frameBuffer_ != nullptr);

    //make the viewport occupy the whole canvas
    const gui::Vec2i& canvasSize{ frameBuffer_->getSize() };
    glViewport(0, 0, canvasSize.x, canvasSize.y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //set the projection transformation
    glOrtho(x1, x2, y1, y2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
  }

  void drawTriangle()
  {
    const float factor_{ std::cos(angle_ * 3.14159f / 180.0f) };
    glBegin(GL_TRIANGLES);
    glColor3f(1, 0, 0); glVertex3f(-0.6f * factor_, -0.75f, 0.0f);
    glColor3f(0, 1, 0); glVertex3f( 0.6f * factor_, -0.75f, 0.0f);
    glColor3f(0, 0, 1); glVertex3f( 0.0f * factor_,  0.75f, 0.0f);
    glEnd();
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
  void render() override
  {
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
  gui::Application app{ "GUI: Hello GL!" };

  // Create the main window
  MainWindow mainWindow;

  // Run the application
  app.run();

  return 0;
}
