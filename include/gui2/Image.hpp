//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Types.hpp>

#include <stdint.h>

namespace gui2
{
  //! \brief A type representing a generic texture ID.
  //!
  //! This type is used to represent a texture ID in a platform-independent
  //! manner. It is typically used to represent a texture ID obtained from a
  //! graphics API such as OpenGL or DirectX.
  using TextureID = uintptr_t;

  //! \brief A class representing an image that can be displayed in the GUI.
  //!
  //! The Image class encapsulates a texture ID that can be used to display an
  //! image in the GUI.
  class Image
  {
  public:
    //! \brief Default constructor for the Image class.
    Image() = default;

    //! \brief Constructs an Image object with the given texture ID and size.
    //! \param[in] textureID The texture ID of the image.
    Image(TextureID textureID_) : textureID_{textureID_} {}

    //! \brief Gets the texture ID of the image.
    //! \return The texture ID of the image.
    TextureID getTextureID() const { return textureID_; }

  private:
    TextureID textureID_ = 0;
  };

} // namespace gui
