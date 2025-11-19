//  Copyright (c) 2025 Daniel Moreno. All rights reserved.
//

// This example shows how to display an OpenGL rendered image using WidgetGL
// with zoom and pan capabilities.
//

#include <gl/gl.h>
#include <gui/gui.hpp>
#include <gui/Image.hpp>

#include <cmath>
#include <stdexcept>
#include <vector>
#include <cstdint>

class TopFrame : public gui::Image
{
  GLuint textureId_;
public:
  TopFrame() : textureId_{ 0U }
  {
  }

  ~TopFrame()
  {
    if (textureId_ != 0U) {
      glDeleteTextures(1, &textureId_);
    }
  }

protected:
  ImTextureID textureId() const override
  {
    return static_cast<ImTextureID>(textureId_);
  }

  Vec2i textureSize(ImTextureID textureId) const override
  {
    Vec2i size{ 0, 0 };
    if (textureId > 0U)
    {
      GLint width, height;
      glBindTexture(GL_TEXTURE_2D, textureId);
      glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
      glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
      glBindTexture(GL_TEXTURE_2D, 0);
      size.x = static_cast<int>(width);
      size.y = static_cast<int>(height);
    }
    return size;
  }

  void updateTexture() override
  {
    if (textureId_ == 0U)
    {
      createTexture_();

      // make checkerboard pattern
      const size_t width = 320, height = 240;
      const size_t checkerSize = 20;
      std::vector<uint8_t> data(width * height * 3);
      for (size_t y = 0; y < height; ++y)
      {
        uint8_t* row = &data[y * width * 3];
        for (size_t x = 0; x < width; ++x)
        {
          const bool isWhite = ((x / checkerSize) % 2) == ((y / checkerSize) % 2);
          row[x * 3 + 0] = isWhite ? 200 : 50;
          row[x * 3 + 1] = isWhite ? 200 : 50;
          row[x * 3 + 2] = isWhite ? 200 : 50;
        }
      }

      // Upload pixels into texture
      glBindTexture(GL_TEXTURE_2D, textureId_);

      // Allow 1 byte alignment, default is 4 bytes
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

      // Upload image data to texture
      glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        static_cast<GLsizei>(width),
        static_cast<GLsizei>(height),
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        data.data());

      glBindTexture(GL_TEXTURE_2D, 0);
    }
  }

private:
  void createTexture_()
  {
    if (textureId_)
    {
      throw std::runtime_error("Texture already exists.");
    }

    // Initialize OpenGL texture
    glGenTextures(1, &textureId_);
    glBindTexture(GL_TEXTURE_2D, textureId_);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
  }

};

class BottomFrame : public gui::ChildFrame
{
  TopFrame& topFrame_;
public:
  BottomFrame(TopFrame& topFrame) : topFrame_{ topFrame }
  {
  }
  void render() override
  {
    ImGui::Text("Bottom Frame");
    ImGui::Text("Zoom: %.2fX", topFrame_.zoomValue());

    const auto pos = topFrame_.mousePosition();
    if (std::isnan(pos.x) || std::isnan(pos.y))
    {
      ImGui::Text("Pos: (outside image)");
    }
    else
    {
      ImGui::Text("Pos: %.2f, %.2f", pos.x, pos.y);
    }
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
  BottomFrame bottomFrame_{ topFrame_ };
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
  gui::Application app{ "GUI: Hello Image Zoom!" };

  // Create the main window
  MainWindow mainWindow;

  // Run the application
  app.run();

  return 0;
}
