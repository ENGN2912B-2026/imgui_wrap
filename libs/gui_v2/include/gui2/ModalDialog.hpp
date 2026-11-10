//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>
#include <gui2/Identifier.hpp>

namespace gui2
{
  //! \brief A modal dialog with a unique identifier and Widget content.
  //!
  //! At most one modal is displayed at a time. The first open modal encountered
  //! becomes active; other open modals wait until it closes or disappears.
  //! Identifiers must be unique and stable across frames. Modals cannot be
  //! displayed inside menus or other modals.
  class ModalDialog
  {
  public:
    //! \brief Constructs a modal with an identifier and optional content.
    //! \param[in] id The unique identifier; its name, if any, is the title.
    //! \param[in] widget The content of the modal.
    template<Identifier T>
    ModalDialog(T&& id, Widget widget = {});

    //! \brief Constructs a modal identified by its construction location.
    //! \param[in] widget The content of the modal.
    //! \param[in] location The source location where the modal is constructed.
    ModalDialog(Widget widget = {},
                std::source_location location = std::source_location::current());

    //! \brief Returns the unique identifier.
    const std::string& getId() const { return id_; }
    //! \brief Returns the name supplied by the identifier, if any.
    const std::string& getName() const { return name_; }

    //! \brief Sets the title displayed in the modal.
    //! \param[in] title The visible title, independent of the identifier.
    void setTitle(std::string title) { title_ = std::move(title); }
    //! \brief Returns the modal title.
    const std::string& getTitle() const { return title_; }

    //! \brief Sets whether the modal is open.
    //! \param[in] open true to open the modal, false to close it.
    void setOpen(bool open) { open_ = open; }
    //! \brief Returns whether the modal is open, including while waiting.
    bool isOpen() const { return open_; }

    //! \brief Sets the size when the modal appears; zero dimensions auto-size.
    //! \param[in] size The size in pixels.
    void setSize(Vec2i size) { size_ = size; }
    //! \brief Returns the size used when the modal appears.
    const Vec2i& getSize() const { return size_; }

    //! \brief Sets whether the modal is centered in its parent when it appears.
    //! \param[in] centered true to center the modal.
    void setCentered(bool centered) { centered_ = centered; }
    //! \brief Returns whether the modal is centered when it appears.
    bool getCentered() const { return centered_; }

    //! \brief Sets whether the modal can be resized.
    //! \param[in] resizable true to allow resizing.
    void setResizable(bool resizable) { resizable_ = resizable; }
    //! \brief Returns whether the modal can be resized.
    bool getResizable() const { return resizable_; }

    //! \brief Sets whether the modal hides its scrollbars.
    //! \param[in] noScrollbar true to hide the scrollbars.
    void setNoScrollbar(bool noScrollbar) { noScrollbar_ = noScrollbar; }
    //! \brief Returns whether the modal hides its scrollbars.
    bool getNoScrollbar() const { return noScrollbar_; }

    //! \brief Displays the modal content within the given rectangle.
    //! \param[in] runtime The runtime used to display the content.
    //! \param[in] rect The rectangle where the content is displayed.
    //! \return The actual rectangle used to display the content.
    Rect displayContent(const Runtime& runtime, const Rect& rect)
    {
      return widget_.display(runtime, rect);
    }

  private:
    std::string id_;
    std::string name_;
    std::string title_;
    Widget widget_;
    Vec2i size_{0, 0};
    bool open_ = false;
    bool centered_ = false;
    bool resizable_ = true;
    bool noScrollbar_ = false;
  };

  template<Identifier T>
  ModalDialog::ModalDialog(T&& id, Widget widget)
    : id_{id.getValueStr()}, widget_{std::move(widget)}
  {
    if constexpr (IdentifierWithName<std::remove_cvref_t<T>>)
    {
      name_ = id.getName();
      title_ = name_;
    }
  }

  inline ModalDialog::ModalDialog(Widget widget, std::source_location location)
    : id_{LocationId{std::move(location)}.getValueStr()}
    , widget_{std::move(widget)}
  {
  }
}
