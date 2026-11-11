//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#include <nfd/OpenFileDialog.hpp>

#include <stdexcept>
#include <utility>

#ifdef USE_NATIVEFILEDIALOG_EXTENDED
# include <nfd.hpp>
# include <limits>
#endif

#if defined(USE_NATIVEFILEDIALOG_EXTENDED) && defined(USE_GLFW_GL3)
# include <backend/Backend_GLFW_GL3.hpp>
# if defined(_WIN32)
#  define GLFW_EXPOSE_NATIVE_WIN32
# elif defined(__APPLE__)
#  define GLFW_EXPOSE_NATIVE_COCOA
# elif defined(__linux__)
#  define GLFW_EXPOSE_NATIVE_X11
# else
#  error Unsupported platform for native file dialog parenting
# endif
# include <nfd_glfw3.h>
#endif

namespace nfd
{
  OpenFileDialog& OpenFileDialog::setFilters(std::vector<FileFilter> filters)
  {
    filters_ = std::move(filters);
    return *this;
  }

  OpenFileDialog& OpenFileDialog::setDefaultPath(std::string path)
  {
    defaultPath_ = std::move(path);
    return *this;
  }

#ifdef USE_NATIVEFILEDIALOG_EXTENDED
  // NFD is enabled, so we can use the open() method normally.
  std::string OpenFileDialog::open(backend::Backend& parent) const
  {
    nfdopendialogu8args_t args{};
#ifdef USE_GLFW_GL3
    auto* glfwBackend = dynamic_cast<backend::Backend_GLFW_GL3*>(&parent);
    if (!glfwBackend || !glfwBackend->window ||
        !NFD_GetNativeWindowFromGLFWWindow(glfwBackend->window, &args.parentWindow))
    {
      throw std::runtime_error{"Could not obtain the file dialog parent window"};
    }
#else
    (void)parent;
#endif

    if (filters_.size() > (std::numeric_limits<nfdfiltersize_t>::max)())
    {
      throw std::runtime_error{"Too many file dialog filters"};
    }

    std::vector<nfdu8filteritem_t> filters;
    filters.reserve(filters_.size());
    for (const auto& filter : filters_)
    {
      filters.push_back({filter.name.c_str(), filter.extensions.c_str()});
    }
    args.filterList = filters.empty() ? nullptr : filters.data();
    args.filterCount = static_cast<nfdfiltersize_t>(filters.size());
    args.defaultPath = defaultPath_.empty() ? nullptr : defaultPath_.c_str();

    if (NFD_Init() != NFD_OKAY)
    {
      throw std::runtime_error{std::string{"NFD initialization failed: "} + NFD_GetError()};
    }
    struct NfdGuard
    {
      ~NfdGuard() { NFD_Quit(); }
    } guard;

    nfdu8char_t* rawPath = nullptr;
    const nfdresult_t result = NFD_OpenDialogU8_With(&rawPath, &args);
    NFD::UniquePathU8 path{rawPath};
    if (result == NFD_OKAY)
    {
      return path.get();
    }
    if (result == NFD_CANCEL)
    {
      return {};
    }
    throw std::runtime_error{std::string{"NFD open dialog failed: "} + NFD_GetError()};
  }
#else
  // NFD is disabled, so we throw an exception when open() is called.
  std::string OpenFileDialog::open(backend::Backend& /*parent*/) const
  {
    throw std::runtime_error{"NFD is disabled, cannot open file dialog"};
  }
#endif
} // namespace nfd
