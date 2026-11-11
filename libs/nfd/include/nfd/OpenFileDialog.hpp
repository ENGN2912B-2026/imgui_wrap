//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <string>
#include <vector>

namespace backend
{
  class Backend;
}

namespace nfd
{
  //! \brief A file filter with comma-separated extensions, without dots.
  struct FileFilter
  {
    std::string name;
    std::string extensions;
  };

  //! \brief A native dialog for selecting a single file.
  class OpenFileDialog
  {
  public:
    //! \brief Sets the available file filters.
    //! \return A reference to this dialog.
    OpenFileDialog& setFilters(std::vector<FileFilter> filters);
    //! \brief Gets the available file filters.
    const std::vector<FileFilter>& getFilters() const { return filters_; }

    //! \brief Sets the initial directory as a UTF-8 path.
    //! \return A reference to this dialog.
    OpenFileDialog& setDefaultPath(std::string path);
    //! \brief Gets the initial directory.
    const std::string& getDefaultPath() const { return defaultPath_; }

    //! \brief Opens the dialog with the backend window as its parent when
    //!        using GLFW, or without a parent when GLFW is disabled.
    //! \return The selected UTF-8 path, or an empty string if canceled.
    //! \throw std::runtime_error if the dialog cannot be opened.
    //! \note Call on the GUI thread while the backend is active and, when
    //!       using GLFW, its window exists. NFD must not already be initialized
    //!       by the application.
    //! \note Parent windows are supported on Windows, macOS and Linux X11.
    //!       The bundled NFD helper does not support Wayland parenting.
    std::string open(backend::Backend& parent) const;

  private:
    std::vector<FileFilter> filters_;
    std::string defaultPath_;
  };
} // namespace nfd
