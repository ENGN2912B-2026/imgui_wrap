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
    Image(TextureId textureId_) : textureId_{textureId_} {}

    //! \brief Gets the texture Id of the image.
    //! \return The texture Id of the image.
    TextureId getTextureId() const { return textureId_; }

  private:
    TextureId textureId_ = 0;
  };

} // namespace gui
