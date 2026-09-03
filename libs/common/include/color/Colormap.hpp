//  Copyright (c) 2025 Daniel Moreno. All rights reserved.
//

#pragma once

#include <cstdint>
#include <vector>

namespace color
{
  // RGB color in floating point representation (0.0 - 1.0)
  struct Rgb_f
  {
    float r, g, b;
  };

  // RGB color in unsigned 8-bit representation (0 - 255)
  struct Rgb_u8
  {
    uint8_t r, g, b;
  };

  struct Colormap
  {
    // Classes of colormaps
    // ====================
    // Colormaps are often split into several categories based on their
    // functionality:
    //
    // Sequential
    // ----------
    // Change in lightness and often saturation of color incrementally, often
    // using a single hue; should be used for representing information that has
    // ordering.
    //
    // Diverging
    // ----------
    // Change in lightness and possibly saturation of two different colors that
    // meet in the middle at an unsaturated color; should be used when the
    // information being plotted has a critical middle value, such as topography
    // or when the data deviates around zero.
    //
    // Cyclic
    // ------
    // Change in lightness of two different colors that meet in the middle and
    // beginning/end at an unsaturated color; should be used for values that
    // wrap around at the endpoints, such as phase angle, wind direction, or
    // time of day.
    //
    // Qualitative
    // -----------
    // Often are miscellaneous colors; should be used to represent information
    // which does not have ordering or relationships.
    //
    // See: https://matplotlib.org/stable/users/explain/colors/colormaps.html#classes-of-colormaps
    //


    // Turbo colormap
    // --------------
    // A perceptually uniform colormap developed by Google AI for use in
    // data visualization.
    //
    static const std::vector<Rgb_f>& turbo_f();
    static const std::vector<Rgb_u8>& turbo_u8();

    // Glasbey Category10 colormap
    // ---------------------------
    // A qualitative colormap with 256 distinct colors, useful for categorical
    // data visualization.
    //
    static const std::vector<Rgb_f>& glasbey_category10();
  };
}
