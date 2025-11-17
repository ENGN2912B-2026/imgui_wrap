//  Copyright (c) 2025 Daniel Moreno. All rights reserved.
//

// This example shows how to display an OpenGL rendered image.
// See:
//   https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
//

#include <gl/gl.h>
#include <gui/gui.hpp>

namespace
{
  class ImageRgb
  {
    size_t width_;
    size_t height_;
    std::vector<uint8_t> data_;
  public:
    ImageRgb(size_t width, size_t height) :
      width_{ width },
      height_{ height },
      data_(width * height * 3)
    {
    }

    size_t getWidth() const { return width_; }
    size_t getHeight() const { return height_; }
    const std::vector<uint8_t>& getData() const { return data_; }
    std::vector<uint8_t>& getData() { return data_; }
  };

  ImageRgb createTestImage(size_t width, size_t height)
  {
      ImageRgb image(width, height);
      std::vector<uint8_t>& data = image.getData();

      for (size_t y = 0; y < height; ++y)
      {
          for (size_t x = 0; x < width; ++x)
          {
              size_t index = (y * width + x) * 3;
              data[index + 0] = static_cast<uint8_t>(((x + 1.0f) / width) * 255); // R
              data[index + 1] = static_cast<uint8_t>(((y + 1.0f) / height) * 255); // G
              data[index + 2] = 128; // B
          }
      }

      return image;
  }

  // Simple helper function to load an image into a OpenGL texture with common settings
  GLuint LoadTexture(const ImageRgb& image)
  {
      // Create a OpenGL texture identifier
      GLuint image_texture;
      glGenTextures(1, &image_texture);
      glBindTexture(GL_TEXTURE_2D, image_texture);

      // Setup filtering parameters for display
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      // Upload pixels into texture
      const GLuint image_width = static_cast<GLuint>(image.getWidth());
      const GLuint image_height = static_cast<GLuint>(image.getHeight());
      const unsigned char* image_data = image.getData().data();
      glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0,
                   GL_RGB, GL_UNSIGNED_BYTE, image_data);

      return image_texture;
  }
}

class TopFrame : public gui::ChildFrame
{
  ImageRgb image_;
  GLuint texture_;
public:
  TopFrame() :
    image_{ createTestImage(320, 240) },
    texture_{ 0U }
  {
  }

  ~TopFrame()
  {
    if (texture_ != 0) {
      glDeleteTextures(1, &texture_);
      texture_ = 0;
    }
  }

  void render() override
  {
    ImGui::Text("Top Frame");

    if (!texture_)
    {
      texture_ = LoadTexture(image_);
    }

    // Respect the image aspect ratio
    auto contentRegion = ImGui::GetContentRegionAvail();
    auto displaySize = contentRegion;
    float aspectRatio = static_cast<float>(image_.getWidth()) / image_.getHeight();
    if (displaySize.x / displaySize.y > aspectRatio)
    {
      displaySize.x = displaySize.y * aspectRatio;
    }
    else
    {
      displaySize.y = displaySize.x / aspectRatio;
    }

    // Center the image
    ImGui::SetCursorPosX((contentRegion.x - displaySize.x) * 0.5f + ImGui::GetCursorPosX());
    ImGui::SetCursorPosY((contentRegion.y - displaySize.y) * 0.5f + ImGui::GetCursorPosY());

    // Display the image
    ImGui::Image((ImTextureID)(intptr_t)texture_, displaySize);
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
  gui::Application app{ "GUI: Hello Image!" };

  // Create the main window
  MainWindow mainWindow;

  // Run the application
  app.run();

  return 0;
}
