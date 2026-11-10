//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>

#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace gui2
{
  // Concept of things that can be items in a Menu:
  // - The type is a Widget, or
  // - The type is a WidgetContent type (either Content or ContentFactory).
  //   which can be stored in a Widget.
  template<typename T>
  concept MenuContentItem =
    std::same_as<std::remove_cvref_t<T>, Widget> || WidgetContent<T>;

  // Concept of things that can be items in a MenuBar:
  // - The type is a MenuContentItem (a Widget or WidgetContent), and
  // - The type is not MenuBar, so copying uses the copy constructor instead
  //   of the variadic constructor.
  template<typename T>
  concept MenuBarItem =
    MenuContentItem<T> && !std::same_as<std::remove_cvref_t<T>, MenuBar>;

  //! \brief An item in a menu that performs an action when selected.
  //!
  //! A menu item either invokes a callback or toggles a boolean value. A
  //! boolean item displays a check mark when its value is true. Menu items
  //! are intended to be used inside a Menu.
  class MenuItem
  {
  public:
    //! \brief Constructs a menu item with an optional callback.
    //! \param[in] label   The text to display for the item.
    //! \param[in] onClick The callback to invoke when the item is selected.
    MenuItem(const std::string& label, std::function<void()> onClick = {})
      : label_{label}, onClick_{std::move(onClick)} {}

    //! \brief Constructs a menu item that toggles a boolean value.
    //! \param[in] label   The text to display for the item.
    //! \param[in] checked Pointer to the value to toggle, or nullptr for an
    //!                    item without a checked state.
    //! \note The pointed-to value must outlive the menu item and its use.
    MenuItem(const std::string& label, bool* checked)
      : label_{label}, checked_{checked} {}

    //! \brief Gets the label of the menu item.
    const std::string& getLabel() const { return label_; }

    //! \brief Gets the pointer to the item's checked state.
    //! \return The boolean pointer, or nullptr if the item has no checked state.
    bool* getChecked() const { return checked_; }

    //! \brief Executes the callback if it is set.
    //!
    //! This method is called by the runtime when the item is selected. The
    //! runtime handles toggling the checked state separately.
    void onClick() const { if (onClick_) { onClick_(); } }

  private:
    std::string label_;
    std::function<void()> onClick_;
    bool* checked_ = nullptr;
  };

  //! \brief A labeled menu containing menu items, separators, or nested menus.
  //!
  //! A Menu is placed inside a MenuBar or another Menu. Its items are displayed
  //! in the order they were added, and only while the menu is open.
  class Menu
  {
  public:
    //! \brief Constructs a menu with a label and any number of items.
    //! \tparam T The types of the items, each constructible into a Widget.
    //! \param[in] label The text to display for the menu.
    //! \param[in] items The items to display in the menu.
    template<MenuContentItem... T>
    Menu(const std::string& label, T&&... items) : label_{label}
    {
      (items_.emplace_back(std::forward<T>(items)), ...);
    }

    //! \brief Gets the label of the menu.
    const std::string& getLabel() const { return label_; }

    //! \brief Gets the items in the menu.
    //! \return A reference to the vector of items.
    std::vector<Widget>& getItems() { return items_; }

    //! \brief Gets the items in the menu.
    //! \return A const reference to the vector of items.
    const std::vector<Widget>& getItems() const { return items_; }

  private:
    std::string label_;
    std::vector<Widget> items_;
  };

  //! \brief A window menu bar containing menus.
  //!
  //! Install a MenuBar using Window::setMenuBar(). Menus are displayed in the
  //! order they were added. The window's content is displayed below the bar.
  //!
  //! For example:
  //! \code
  //!   window.setMenuBar(MenuBar{
  //!     Menu{"File", MenuItem{"Quit", [&] { app.quit(); }}},
  //!     Menu{"Help", MenuItem{"About", &showAboutWindow}}
  //!   });
  //! \endcode
  //! \note References captured by menu callbacks and boolean pointers used by
  //!       menu items must remain valid while the menu bar is displayed.
  class MenuBar
  {
  public:
    //! \brief Constructs a menu bar with any number of items.
    //! \tparam T The types of the items, each constructible into a Widget.
    //! \param[in] items The menus to display in the menu bar.
    template<MenuBarItem... T>
    MenuBar(T&&... items)
    {
      (items_.emplace_back(std::forward<T>(items)), ...);
    }

    //! \brief Gets the items in the menu bar.
    //! \return A reference to the vector of items.
    std::vector<Widget>& getItems() { return items_; }

    //! \brief Gets the items in the menu bar.
    //! \return A const reference to the vector of items.
    const std::vector<Widget>& getItems() const { return items_; }

  private:
    std::vector<Widget> items_;
  };
}
