//  Copyright (c) 2024 Daniel Moreno. All rights reserved.
//

#include <string>
#include <cstdint> // For uint64_t

namespace gui::util
{
  //! Generates a random identifier string.
  //! \param seed The seed for the random number generator. If 0, the seed is
  //!             not set. It defaults to 0.
  //! \param length The length of the identifier string. It defaults to 4.
  //! \return A random identifier string.
  //! \note The identifier string is generated using alphanumeric characters.
  //!
  //! If a `seed == 0` is desired, then it must be set prior to calling this
  //! function, like this:
  //! \code{.cpp}
  //!   std::srand(0);
  //! \endcode
  //!
  std::string generateIdentifier(uint64_t seed = 0U, size_t length = 4U);
}
