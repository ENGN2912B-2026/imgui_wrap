//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <math/Vec2.hpp>

#include <stdint.h>
#include <vector>
#include <cassert>

namespace image
{
  class ImageView
  {
  public:
    //! \brief Default constructor for the ImageView class.
    ImageView() = default;

    //! \brief Constructs an ImageView object.
    //! \param[in] data The pointer to the image data.
    //! \param[in] size The size of the image.
    //! \param[in] rowStride The row stride of the image in bytes.
    ImageView(std::byte* data, math::Vec2i size, size_t rowStride)
    {
      initialize(data, size, rowStride);
    }

    //! \brief Initializes the ImageView object.
    //! \param[in] data The pointer to the image data.
    //! \param[in] size The size of the image.
    //! \param[in] rowStride The row stride of the image in bytes.
    void initialize(std::byte* data, math::Vec2i size, size_t rowStride)
    {
      assert(data != nullptr);
      assert(size.x > 0 && size.y > 0);
      assert(rowStride > 0);
      data_ = data;
      size_ = size;
      rowStride_ = rowStride;
    }

    //! \brief Gets the pointer to the image data.
    //! \return The pointer to the image data.
    std::byte* getData() const { return data_; }

    //! \brief Gets the size of the image.
    //! \return The size of the image.
    math::Vec2i getSize() const { return size_; }

    //! \brief Gets the width of the image.
    //! \return The width of the image.
    int getWidth() const { return size_.x; }

    //! \brief Gets the height of the image.
    //! \return The height of the image.
    int getHeight() const { return size_.y; }

    //! \brief Gets the row stride of the image in bytes.
    //! \return The row stride of the image in bytes.
    size_t getRowStride() const { return rowStride_; }

    //! \brief Gets a pointer to the specified row of the image.
    //! \param[in] row The index of the row to get.
    //! \return A pointer to the specified row of the image.
    std::byte* getRow(size_t row) const
    {
      return data_ + row * rowStride_;
    }

    //! \brief Gets a pointer to the image data, casted to the specified type.
    //! \tparam T The type to cast the data pointer to.
    //! \return A pointer to the image data, casted to the specified type.
    template <typename T>
    T* getDataAs() const
    {
      return reinterpret_cast<T*>(data_);
    }

    //! \brief Gets a pointer to the specified row of the image, casted to the
    //!        specified type.
    //! \tparam T The type to cast the row pointer to.
    //! \param[in] row The index of the row to get.
    //! \return A pointer to the specified row of the image, casted to the
    //!          specified type
    template <typename T>
    T* getRowAs(size_t row) const
    {
      return reinterpret_cast<T*>(getRow(row));
    }

  private:
    std::byte* data_ = nullptr;
    math::Vec2i size_ = {0, 0};
    size_t rowStride_ = 0; // in bytes
  };


  template <typename T>
  class Image : public ImageView
  {
  public:
    using PixelType = T;

    //! \brief Default constructor for the Image class.
    Image() = default;

    //! \brief Constructs an Image object with the given size.
    //! \param[in] size The size of the image.
    Image(math::Vec2i size) : ImageView{}, data_{}
    {
      initialize(size);
    }

    //! \brief Constructs an Image object with the given width and height.
    //! \param[in] width The width of the image.
    //! \param[in] height The height of the image.
    Image(int width, int height) : Image{math::Vec2i{width, height}} {}

    //! \brief Initializes the Image object with the given size.
    //! \param[in] size The size of the image.
    void initialize(math::Vec2i size)
    {
      data_.resize(size.x * size.y);
      ImageView::initialize(reinterpret_cast<std::byte*>(data_.data()),
                            size, size.x * sizeof(PixelType));
    }

    //! \brief Gets a pointer to the image data.
    //! \return A pointer to the image data.
    PixelType* getData() const
    {
      return ImageView::getDataAs<PixelType>();
    }

    //! \brief Gets a pointer to the specified row of the image.
    //! \param[in] row The index of the row to get.
    //! \return A pointer to the specified row of the image.
    PixelType* getRow(size_t row) const
    {
      return ImageView::getRowAs<PixelType>(row);
    }

  private:
    std::vector<PixelType> data_;
  };

  //! \brief A struct representing an RGB pixel.
  template <typename T = uint8_t>
  struct Rgb
  {
    //! \brief The type of the color channels.
    using ChannelType = T;

    //! \brief The red component of the pixel.
    T r;
    //! \brief The green component of the pixel.
    T g;
    //! \brief The blue component of the pixel.
    T b;

    //! \brief Gets a pointer to the pixel data.
    //! \return A pointer to the pixel data.
    T* data() { return &r; }

    //! \brief Gets a reference to the specified color channel.
    //! \tparam N The index of the color channel (0 for red, 1 for green,
    //!         2 for blue).
    //! \return A reference to the specified color channel.
    template <size_t N>
    constexpr const T& get() requires (N < 3)
    {
           if constexpr (N == 0) return r;
      else if constexpr (N == 1) return g;
      else if constexpr (N == 2) return b;
      else static_assert(N < 3, "Index out of bounds for Rgb");
    }

    constexpr const T& operator[](size_t index) const
    {
      assert(index < 3);
      return reinterpret_cast<const T*>(this)[index];
    }
  };

  using Rgb8 = Rgb<uint8_t>;
  using Rgb16 = Rgb<uint16_t>;
  using Rgb32 = Rgb<uint32_t>;

  static_assert(sizeof(Rgb8) == 3 * sizeof(uint8_t), "Rgb struct must be packed");
  static_assert(sizeof(Rgb16) == 3 * sizeof(uint16_t), "Rgb struct must be packed");
  static_assert(sizeof(Rgb32) == 3 * sizeof(uint32_t), "Rgb struct must be packed");

  using ImageRgb8 = Image<Rgb8>;

} // namespace image
