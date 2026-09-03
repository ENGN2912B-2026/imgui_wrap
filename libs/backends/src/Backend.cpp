//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//
#include <backend/Backend.hpp>

#ifdef USE_GLFW_GL3
# include <backend/Backend_GLFW_GL3.hpp>
#endif
#include <backend/Backend_Null.hpp>

namespace backend
{
  std::unique_ptr<Backend> Backend::create()
  {
#ifdef USE_GLFW_GL3
    return Backend_GLFW_GL3::create();
#endif

    // Dummy/Null Backend (last one in list so its only the default when there
    //  are no other backends compiled in)
    return Backend_Null::create();
  }
}
