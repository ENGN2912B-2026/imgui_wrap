//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>

#include <string>
#include <utility>

namespace gui2
{
  //! \brief A modal dialog displayed by a Window, outside its normal content layout.
  //! \note Dialog titles must be unique within the window.
  class Dialog
  {
  public:
    //! \brief Constructs a dialog with a title and optional content.
    //! \param[in] title The dialog title, unique within the window.
    //! \param[in] content The content displayed inside the dialog.
    explicit Dialog(std::string title, Widget content = {})
      : title_{std::move(title)}, content_{std::move(content)} {}

    //! \brief Requests opening the dialog during its next display.
    void open() { openRequested_ = true; closeRequested_ = false; }
    //! \brief Requests closing the dialog during its next display.
    void close() { closeRequested_ = true; openRequested_ = false; }

    //! \brief Sets the content displayed inside the dialog.
    //! \param[in] content The dialog content.
    void setContent(Widget content) { content_ = std::move(content); }

    //! \brief Returns the dialog title.
    const std::string& getTitle() const { return title_; }

    //! \brief Sets the dialog size when it appears.
    //! \param[in] size The size in pixels; zero dimensions are automatically sized.
    void setSize(Vec2i size) { size_ = size; }
    //! \brief Returns the dialog size used when it appears.
    const Vec2i& getSize() const { return size_; }

    //! \brief Sets whether the dialog is centered in its parent when it appears.
    //! \param[in] centered true to center the dialog.
    void setCentered(bool centered) { centered_ = centered; }
    //! \brief Returns whether the dialog is centered when it appears.
    bool getCentered() const { return centered_; }

    //! \brief Sets whether the dialog can be resized.
    //! \param[in] resizable true to allow resizing.
    void setResizable(bool resizable) { resizable_ = resizable; }
    //! \brief Returns whether the dialog can be resized.
    bool getResizable() const { return resizable_; }

    //! \brief Sets whether the dialog hides its scrollbars.
    //! \param[in] noScrollbar true to hide the scrollbars.
    void setNoScrollbar(bool noScrollbar) { noScrollbar_ = noScrollbar; }
    //! \brief Returns whether the dialog hides its scrollbars.
    bool getNoScrollbar() const { return noScrollbar_; }

    //! \brief Takes and clears the pending open request.
    //! \return true if opening was requested.
    bool takeOpenRequest() { return std::exchange(openRequested_, false); }
    //! \brief Takes and clears the pending close request.
    //! \return true if closing was requested.
    bool takeCloseRequest() { return std::exchange(closeRequested_, false); }
    //! \brief Displays the dialog content within the given rectangle.
    //! \param[in] runtime The runtime used to display the content.
    //! \param[in] rect The rectangle where the content is displayed.
    //! \return The actual rectangle used to display the content.
    Rect displayContent(const Runtime& runtime, const Rect& rect)
    {
      return content_.display(runtime, rect);
    }

  private:
    std::string title_;
    Widget content_;
    Vec2i size_{0, 0};
    bool centered_ = false;
    bool resizable_ = true;
    bool noScrollbar_ = false;
    bool openRequested_ = false;
    bool closeRequested_ = false;
  };
}
