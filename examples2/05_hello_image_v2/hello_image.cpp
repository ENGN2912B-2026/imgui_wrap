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

class ImageWidget
{
public:
  Rect display(const Runtime& rt, const Rect& rect)
  {
    if (!texture_.isInitialized())
    { // Create a test image and load it into an OpenGL texture
      texture_.initialize(createTestImage(rect.getAvailableSize()));
    }

    // Display the frame buffer's texture as an image in the GUI
    return rt.display(Image{ texture_ }, rect);
  }
private:
  gl::Texture texture_;
};

static_assert(Displayable<ImageWidget>, "ImageWidget should be a Displayable");


int main(int argc, char** argv)
{
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
