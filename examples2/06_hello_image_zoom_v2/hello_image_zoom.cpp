//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#include <gui2/Application.hpp>
#include <gui2/Panel.hpp>
#include <gui2/Layout.hpp>
#include <gui2/Separator.hpp>
#include <gui2/Image.hpp>

#include <gl/Texture.hpp>

#include <image/Image.hpp>

#include <print>

// This example shows how to create an image, fill it with a gradient pattern,
// and display it in a GUI window using `gl::Texture` and `gui2::Image`.
//

using namespace gui2;

//! \brief Creates a test image with a gradient pattern.
//! \param[in] imageSize  The size of the image to create.
//! \return An ImageRgb8 object containing the generated image.
image::ImageRgb8 createTestImage()
{
  constexpr size_t checkerSize{ 20 };
  image::ImageRgb8 image{ 320, 240 };
  // make a checkerboard pattern (exclude the first row and column)
  for (size_t y = 1; y < image.getHeight(); ++y)
  {
    auto* row = image.getRow(y);
    for (size_t x = 1; x < image.getWidth(); ++x)
    {
      const bool isWhite = ((x / checkerSize) % 2) == ((y / checkerSize) % 2);
      const uint8_t colorValue = isWhite ? 200 : 50;
      row[x] = image::Rgb8{colorValue, colorValue, colorValue};
    }
  }
  // make a red column at the left side of the image
  for (size_t y = 0; y < image.getHeight(); ++y)
  {
    auto* row = image.getRow(y);
    row[0] = image::Rgb8{255, 0, 0};
  }
  // make a green row at the top of the image
  for (size_t x = 0; x < image.getWidth(); ++x)
  {
    auto* row = image.getRow(0);
    row[x] = image::Rgb8{0, 255, 0};
  }
  return image;
}

class ImageWidget
{
public:
  Rect display(const Runtime& rt, const Rect& rect)
  {
    if (!texture_.isInitialized())
    { // Create a test image and load it into an OpenGL texture
      texture_.initialize(createTestImage(), GL_NEAREST);
      imageZoom_ = ImageZoom{ texture_, true, true };
    }

    // Display the frame buffer's texture as an image in the GUI
    return rt.display(imageZoom_, rect);
  }
private:
  ImageZoom imageZoom_;
  gl::Texture texture_;
};

static_assert(Displayable<ImageWidget>, "ImageWidget should be a Displayable");


int main(int argc, char** argv)
{
  // Title of the window
  std::string title{"GUI: Hello Image Zoom v2!"};

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
