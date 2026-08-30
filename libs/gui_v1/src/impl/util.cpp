//  Copyright (c) 2024-2025 Daniel Moreno. All rights reserved.
//

#include "util.hpp"

#include <cstdlib>

namespace gui::util
{
  std::string generateIdentifier(uint64_t seed, size_t length)
  {
    const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::string result;
    if (seed != 0U)
    {
      std::srand(seed);
    }

    for (size_t i = 0; i < length; ++i) {
        result += alphanum[std::rand() % (sizeof(alphanum) - 1)];
    }

    return result;
  }
}
