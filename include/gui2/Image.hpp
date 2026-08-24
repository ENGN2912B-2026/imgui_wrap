//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Types.hpp>

#include <stdint.h>

namespace gui2
{
  //! \brief A type representing a generic texture Id.
  //!
  //! This type is used to represent a texture Id in a platform-independent
  //! manner. It is typically used to represent a texture Id obtained from a
  //! graphics API such as OpenGL or DirectX.
  using TextureId = uintptr_t;

  //! \brief A class representing an image that can be displayed in the GUI.
  //!
  //! The Image class encapsulates a texture Id that can be used to display an
  //! image in the GUI.
  class Image
  {
  public:
      //! \brief Default constructor for the Image class.
    Image() = default;

    //! \brief Constructs an Image object with the given texture Id and size.
    //! \param[in] textureId The texture Id of the image.
    Image(TextureId textureId) : textureId_{textureId} {}

    //! \brief Gets the texture Id of the image.
    //! \return The texture Id of the image.
    TextureId getTextureId() const { return textureId_; }

  private:
    TextureId textureId_ = 0;
  };

  //! \brief A class representing an image with zoom and pan capabilities.
  //!
  //! The ImageZoom class extends the Image class to provide zoom and pan
  //! capabilities. As the zoom and pan state is kept internally, users must
  //! ensure the `ImageZoom` object persists across frames, or the zoom and pan
  //! state will be lost.
  class ImageZoom : public Image
  {
  public:
    //! \brief Default constructor for the ImageZoom class.
    ImageZoom() = default;

    //! \brief Constructs an ImageZoom object with the given texture Id and size.
    //! \param[in] textureId The texture Id of the image.
    //! \param[in] textureSize The size of the texture in pixels.
    //! \param[in] enableZoomPan Whether to enable zoom and pan functionality.
    //! \param[in] maintainAspectRatio Whether to maintain the aspect ratio of
    //!                                the image.
    ImageZoom(TextureId textureId,
              const Vec2i& textureSize,
              bool enableZoomPan = true,
              bool maintainAspectRatio = false) : Image{textureId}
    {
      zoomState_.textureSize = textureSize.cast<float>();
      zoomState_.zoomPanEnabled = enableZoomPan;
      zoomState_.maintainAspectRatio = maintainAspectRatio;
    }

    //! \brief Zoom and pan state of an image.
    //!
    //! Members:
    //! - Inputs (not modified by the widget):
    //!   - zoomPanEnabled: Enable or disable zooming and panning functionality.
    //!   - maintainAspectRatio: Maintain the aspect ratio of the image when
    //!                          resizing. Requires `textureSize` to be set.
    //!   - maxZoomLevel: Maximum allowed zoom level (0.0 = automatically set).
    //!   - textureSize: Size of the texture/image being displayed. This is
    //!                  the original size of the image in pixels. If not set,
    //!                  the widget will attempt to infer the size from the
    //!                  displayed image size and UV coordinates.
    //! - Outputs (set by the widget):
    //!   - zoomLevel: Current zoom level (1.0 = 100%).
    //!   - panOffset: Current pan offset in normalized coordinates (-1.0 to 1.0).
    //!   - mousePosition: Current mouse position within the image area, or NaN
    //!                    if the mouse is outside the image area.
    struct ZoomState
    {
      // User Inputs
      bool zoomPanEnabled       = true;
      bool maintainAspectRatio  = false;
      float maxZoomLevel        = 0.0f;
      Vec2f textureSize         = {0.0f, 0.0f};

      // Outputs
      float zoomLevel     = 1.0f;
      Vec2f panOffset     = {0.0f, 0.0f};
      Vec2f mousePosition = {0.0f, 0.0f};
    };

    //! \brief Gets the zoom and pan state of the image.
    //! \return The zoom and pan state of the image.
    const ZoomState& getZoomState() const { return zoomState_; }

    //! \brief Gets the zoom and pan state of the image.
    //! \return The zoom and pan state of the image.
    ZoomState& getZoomState() { return zoomState_; }

  private:
    ZoomState zoomState_ = {};
  };



} // namespace gui
