//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Application.hpp>
#include <gui2/Panel.hpp>
#include <gui2/Layout.hpp>
#include <gui2/Separator.hpp>
#include <gui2/Image.hpp>

#include <gl/gl.h>

#include <image/Image.hpp>

#include <print>

// This example shows how to create an image, fill it with a gradient pattern,
// and display it in a GUI window using OpenGL.
//

using namespace gui2;

//! \brief Creates a test image with a gradient pattern.
//! \param[in] imageSize  The size of the image to create.
//! \return An ImageRgb8 object containing the generated image.
image::ImageRgb8 createTestImage(const Vec2i& imageSize)
{
  image::ImageRgb8 image{ imageSize };

  for (size_t y = 0; y < imageSize.y; ++y)
  {
    auto* row = image.getRow(y);
    for (size_t x = 0; x < imageSize.x; ++x)
    {
      row[x] = image::Rgb8{
        static_cast<uint8_t>(((x + 1.0f) / imageSize.x) * 255), // R
        static_cast<uint8_t>(((y + 1.0f) / imageSize.y) * 255), // G
        128 // B
      };
    }
  }

  return image;
}

//! \brief Loads an image into an OpenGL texture with common settings.
//! \param[in] image  The image to load into the texture.
//! \return The OpenGL texture ID of the loaded image.
GLuint LoadTexture(const image::ImageRgb8& image)
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
  const unsigned char* image_data = image.getData()->data();
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0,
                GL_RGB, GL_UNSIGNED_BYTE, image_data);

  return image_texture;
}

class ImageWidget
{
public:
  Rect display(const Runtime& rt, const Rect& rect)
  {
    if (!textureID_)
    { // Create a test image and load it into an OpenGL texture
      const auto image = createTestImage(rect.getAvailableSize());
      textureID_ = LoadTexture(image);
    }

    // Display the frame buffer's texture as an image in the GUI
    const Rect actualRect{ rt.display(Image{ textureID_ }, rect) };

    // Return the actual rectangle used for displaying the image
    return actualRect;
  }

  ~ImageWidget()
  {
    if (textureID_ > 0)
    {
      glDeleteTextures(1, &textureID_);
    }
  }

private:
  GLuint textureID_ = 0;
};

static_assert(Displayable<ImageWidget>, "ImageWidget should be a Displayable");


int main(int argc, char** argv)
{
  using namespace gui2;

  // Title of the window
  std::string title{"GUI: Hello Image v2"};

  // Create the application
  Application app{ title };

  // Get the window
  Window& window = app.getWindow();

  // Create the OpenGL widget
  ImageWidget imageWidget;

  // Set the content of the window to include the OpenGL widget and other
  // GUI elements
  window.setContent(
    HBox{
      VBox{
        // Main panel
        Stretch{3, Panel{ VStack{
          "Main Panel",
          Separator{},
          std::ref(imageWidget),
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
