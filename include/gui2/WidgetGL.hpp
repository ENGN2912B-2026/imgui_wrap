//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Image.hpp>

#include <gl/FrameBuffer.hpp>

namespace gui2
{
  //! \brief A widget that allows OpenGL rendering within the GUI.
  //!
  //! The WidgetGL class provides a convenient way to integrate OpenGL rendering
  //! into a GUI application. It manages an OpenGL frame buffer and allows
  //! derived classes to implement their own OpenGL rendering logic by
  //! overriding the `drawGL()` method. The frame buffer is automatically
  //! initialized and resized based on the available size of the widget.
  class WidgetGL
  {
  public:
    //! \brief Display the OpenGL content within the GUI.
    //! \param[in] rt   The runtime object used for rendering.
    //! \param[in] rect The rectangle defining the area where the OpenGL content
    //!                 should be displayed.
    //! \return The rectangle representing the area occupied by the OpenGL
    //!         content within the GUI.
    Rect display(const Runtime& rt, const Rect& rect)
    {
      // Get the available size for the widget
      const Vec2i availableSize{ rect.getAvailableSize() };

      // Initialize or resize the frame buffer if necessary
      if (!frameBuffer_.isInitialized())
      { // Initialize the frame buffer
        initialize(availableSize);
      }
      else if (frameBuffer_.getSize() != availableSize)
      { // Resize the frame buffer
        frameBuffer_.setSize(availableSize);
      }

      if (frameBuffer_.isInitialized())
      { // We can draw the OpenGL content into the frame buffer
        auto autoUnbind{ frameBuffer_.bindScoped() };

        // Set the OpenGL viewport to match the frame buffer size
        const Vec2i& frameBufferSize{ frameBuffer_.getSize() };
        glViewport(0, 0, frameBufferSize.x, frameBufferSize.y);

        // Draw the OpenGL content into the frame buffer
        drawGL();
      }

      // Display the frame buffer's texture as an image in the GUI
      return rt.display(Image{ frameBuffer_.getTexture() }, rect);
    }

  protected:
    //! \brief The OpenGL frame buffer used for rendering.
    gl::FrameBuffer frameBuffer_;

    //! \brief Initializes the frame buffer with the given available size.
    //!
    //! This method is called when the frame buffer is not initialized or when
    //! the available size changes. Derived classes can override this method to
    //! perform any necessary initialization or configuration of the frame buffer.
    virtual void initialize(const Vec2i& availableSize)
    {
      frameBuffer_.initialize(availableSize);
    }

    //! \brief Draws the OpenGL content into the frame buffer.
    //!
    //! This method is called after the frame buffer has been bound and the
    //! OpenGL viewport has been set to match the frame buffer size. Derived
    //! classes should override this method to implement their own OpenGL
    //! rendering logic. The default implementation clears the frame buffer
    //! with a solid color.
    virtual void drawGL()
    {
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

  };

  // Static assertion to ensure that WidgetGL is a Displayable type
  static_assert(Displayable<WidgetGL>, "WidgetGL should be a Displayable");

} // namespace gui
