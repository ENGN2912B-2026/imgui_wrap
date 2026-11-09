//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Widget.hpp>
#include <gui2/Identifier.hpp>
#include <gui2/Actions.hpp>

#include <concepts>
#include <functional>

namespace gui2
{
  template<typename T>
  concept InputBoxValue =
    std::same_as<T, std::string> ||
    std::same_as<T, int> ||
    std::same_as<T, float> ||
    std::same_as<T, double>;

  //! \brief An InputBox is a GUI widget that allows the user to input and edit
  //!        a value.
  template<typename T>
  class InputBox
  {
    static_assert(InputBoxValue<T>,
      "InputBox supports std::string, int, float, and double values");

  public:
    //! \brief Constructs an InputBox with the given identifier and value.
    //! \param[in] id The identifier of the InputBox. It must be unique within
    //!               the application.
    //! \param[in] value A pointer to the value edited by the InputBox. The
    //!                  InputBox does not take ownership of the value, and the
    //!                  caller must ensure it remains valid while the widget
    //!                  is used.
    //! \param[in] onEditFinished The callback to invoke when the user finishes
    //!                           editing the value. It is called only if the
    //!                           value has changed.
    template<Identifier I>
    InputBox(I&& id,
             T* value,
             std::function<void()> onEditFinished = {});

    //! \brief Constructs an InputBox with an identifier generated from its
    //!        source location and a pointer to the value it edits.
    //! \param[in] value A pointer to the value edited by the InputBox. The
    //!                  InputBox does not take ownership of the value, and the
    //!                  caller must ensure it remains valid while the widget
    //!                  is used.
    //! \param[in] onEditFinished The callback to invoke when the user finishes
    //!                           editing the value. It is called only if the
    //!                           value has changed.
    //! \param[in] location The source location where the InputBox is
    //!                     constructed.
    InputBox(T* value,
             std::function<void()> onEditFinished = {},
             std::source_location location = std::source_location::current());

    //! \brief Returns the identifier of the InputBox.
    const std::string& getId() const { return id_; }

    //! \brief Returns the name of the InputBox.
    //!
    //! If the identifier type used at construction time provides a name, this
    //! function returns that name. Otherwise, it returns an empty string.
    const std::string& getName() const { return name_; }

    //! \brief Returns a pointer to the value edited by the InputBox.
    T* getValuePtr() { return value_; }

    //! \brief Executes the edit-finished callback if it is set.
    void onEditFinished() const
    {
      if (onEditFinished_) { onEditFinished_(); }
    }

    //! \brief Adds an action to be executed during the next display cycle.
    //! \param[in] action The action to add.
    void addAction(Actions action) { actions_ |= action; }

    //! \brief Takes and clears all existing actions.
    //! \return The actions that were taken.
    Actions takeActions() { return std::exchange(actions_, Actions::None); }

  private:
    std::string id_;
    std::string name_;
    T* value_;
    std::function<void()> onEditFinished_;
    Actions actions_ = Actions::None;
  };

  using TextBox = InputBox<std::string>;
  using IntBox = InputBox<int>;
  using FloatBox = InputBox<float>;
  using DoubleBox = InputBox<double>;

} // namespace gui2

// Implementation ------------------------------------------------------------
namespace gui2
{
  template<typename T>
  template<Identifier I>
  InputBox<T>::InputBox(I&& id,
                        T* value,
                        std::function<void()> onEditFinished)
    : value_{value}
    , id_{id.getValueStr()}
    , onEditFinished_{std::move(onEditFinished)}
  {
    if constexpr (IdentifierWithName<std::remove_cvref_t<I>>)
    {
      name_ = id.getName();
    }
  }

  template<typename T>
  inline InputBox<T>::InputBox(
    T* value,
    std::function<void()> onEditFinished,
    std::source_location location)
    : value_{value}
    , onEditFinished_{std::move(onEditFinished)}
  {
    LocationId id{std::move(location)};
    id_ = id.getValueStr();
  }
}
