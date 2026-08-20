//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Runtime.hpp>
#include <gui2/Empty.hpp>

#include <memory>
#include <optional>
#include <typeinfo>

namespace gui2
{
  // Forward declarations
  class Widget;

  // Concept of things that Widget can hold as content:
  // - The type is a Content type (either Primitive or Displayable), or
  //   a ContentFactory type (a callable that returns a Content type).
  // - The type is not Widget itself (to avoid infinite recursion and wrapping
  //   a Widget into another Widget).
  template<class T>
  concept WidgetContent =
    (Content<T> || ContentFactory<T>) &&
    (!std::same_as<std::remove_cvref_t<T>, Widget>);

  // Widget class
  // A widget is any object that can be displayed in the user interface.
  class Widget
  {
  public:
    // Default constructor creates an empty Widget.
    Widget() = default;

    // Constructor that takes a value of type T, and stores it within the
    // Widget, which takes ownership of the value.
    template<WidgetContent T>
    Widget(T value);

    //! \brief Resolves the Widget's content and, if the resulting object is of
    //!        type `T`, returns a pointer to it; otherwise, returns `nullptr`.
    //! \tparam T The type to check the resolved object against.
    //! \return A pointer to the resolved object if it is of type `T`;
    //!         otherwise, `nullptr`.
    //! \note Calling `resolveAs<T>()` always resolves the Widget's content,
    //!       even if it has already been resolved and regardless of whether
    //!       the resulting object's type is `T`.
    template<Content T>
    T* resolveAs();

    //! \brief Resolves the Widget's content and, if the resulting object is of
    //!        the specified type `type`, returns a pointer to it; otherwise,
    //!        returns `nullptr`.
    //! \param type The type to check the resolved object against.
    //! \return A pointer to the resolved object if it is of the specified type;
    //!         otherwise, `nullptr`.
    //! \note Calling `resolveAs(const std::type_info& type)` always resolves
    //!       the Widget's content, even if it has already been resolved and
    //!       regardless of whether the resulting object's type is `type`.
    void* resolveAs(const std::type_info& type);

    // Returns a pointer to the value of type T held by the Widget, or nullptr
    // if the Widget does not hold a value of type T.
    template<WidgetContent T>
    T* get();

    // Returns a const pointer to the value of type T held by the Widget, or nullptr
    // if the Widget does not hold a value of type T.
    template<WidgetContent T>
    const T* get() const;

    // Display the widget using the provided Runtime and within the specified
    // rectangle. Returns the actual rectangle used for displaying the widget.
    Rect display(const Runtime& rt, const Rect& rect);

  private:
    class Value;
    std::unique_ptr<Value> value_;
    template<Content T> class ContentValue;
    template<ContentFactory T> class FactoryValue;
  };

} // namespace gui

// Implementation -------------------------------------------------------------
namespace gui2
{
  namespace widget
  {
    // Helper template to resolve a WidgetContent type to its underlying type.
    template<class T> struct resolved_type{};
  }

  //! \brief Resolves a WidgetContent type to its underlying type.
  template<class T>
  using resolved_type_t =
    typename widget::resolved_type<std::remove_cvref_t<T>>::type;

  namespace widget
  {
    // Specialization for Content types: the resolved type is the type itself.
    template<Content T>
    struct resolved_type<T>
    {
      using type = T;
    };

    // Specialization for ContentFactory types: the resolved type is the type
    // of the value it produces.
    template<ContentFactory F>
    struct resolved_type<F>
    {
      using type = resolved_type_t<std::invoke_result_t<F&>>;
    };

    // Resolves a ContentFactory type recursively to its resulting Content type.
    template<ContentFactory F>
    auto resolve(F&& factory) -> resolved_type_t<F>
    {
      // Invoke the factory to get the result.
      auto result = std::invoke(std::forward<F>(factory));

      // Check if the result needs to be further resolved.
      if constexpr (Content<std::remove_cvref_t<decltype(result)>>)
      { // If the result is a Content type, return it directly.
        return result;
      }
      else
      { // If the result is another ContentFactory, resolve it recursively.
        return resolve(std::move(result));
      }
    }
  }

  //! \brief Abstract base class for the value held by the Widget.
  class Widget::Value
  {
  public:
    //! \brief Virtual destructor to allow proper cleanup of derived classes.
    virtual ~Value() = default;

    //! \brief Virtual function to check if the value is a ContentFactory.
    //! \return `true` if the value is a ContentFactory, `false` otherwise.
    virtual bool isFactory() const { return false; }

    //! \brief Pure virtual function to resolve the value as a specific type.
    //! \param type The type to check the resolved object against.
    //! \return A pointer to the resolved object if it is of the specified type;
    //!         otherwise, `nullptr`.
    virtual void* resolveAs(const std::type_info& type) = 0;

    //! \brief Pure virtual function to display the value.
    //! \param rt    The runtime environment used for rendering.
    //! \param rect  The rectangle within which the value should be displayed.
    //! \return The actual rectangle used for displaying the value.
    //!
    //! This function resolves the value, then displays it, and finally
    //! unresolves it in preparation for the next display call.
    virtual Rect display(const Runtime& rt, const Rect& rect) = 0;

    //! \brief Factory method to create a Value object from a given
    //!        WidgetContent based on its type.
    //! \tparam T The type of the WidgetContent.
    //! \param value The value to create the Value object from.
    //! \return A unique pointer to the created Value object.
    //!
    //! If T is a Content type, it creates a ContentValue<T>.
    //! If T is a ContentFactory type, it creates a FactoryValue<T>.
    template<WidgetContent T>
    static std::unique_ptr<Value> create(T value);

  protected:
    //! \brief Helper function to display the content of type T.
    //! \tparam T The type of the content to display.
    //! \param rt The runtime environment used for rendering.
    //! \param content The content to display.
    //! \param rect The rectangle within which the content should be displayed.
    //! \return The actual rectangle used for displaying the content.
    template<Content T>
    static Rect displayContent(const Runtime& rt, T& content, const Rect& rect);
  };

  //! \brief Template class for holding a Content type within a Widget.
  //! \tparam T The type of the Content.
  template<Content T>
  class Widget::ContentValue : public Value
  {
  public:
    ContentValue(T content) : content_{std::move(content)} {}
    void* resolveAs(const std::type_info& type) override;
    Rect display(const Runtime& rt, const Rect& rect) override;
  private:
    T content_;
  };

  //! \brief Template class for holding a ContentFactory type within a Widget.
  //! \tparam F The type of the ContentFactory.
  template<ContentFactory F>
  class Widget::FactoryValue : public Value
  {
  public:
    FactoryValue(F factory) : factory_{std::move(factory)} {}
    void* resolveAs(const std::type_info& type) override;
    Rect display(const Runtime& rt, const Rect& rect) override;
  private:
    using Resolved = resolved_type_t<F>;
    F factory_;
    std::optional<Resolved> resolved_;
    void resolve_();
    void unresolve_();
  };

  //---------------------------------------------------------------------------
  // Widget::Value class implementation ---------------------------------------
  //---------------------------------------------------------------------------
  template<WidgetContent T>
  std::unique_ptr<Widget::Value> Widget::Value::create(T value)
  {
    if constexpr (Content<T>)
    {
      return std::make_unique<ContentValue<T>>(std::move(value));
    }
    else if constexpr (ContentFactory<T>)
    {
      return std::make_unique<FactoryValue<T>>(std::move(value));
    }
    else
    {
      static_assert(false,
        "Type is not `WidgetContent` (neither `Content` nor `ContentFactory`)");
    }
  }

  template<Content T>
  Rect Widget::Value::displayContent(const Runtime& rt, T& content, const Rect& rect)
  {
    if constexpr (Primitive<T>)
    {
      return rt.display(content, rect);
    }
    else if constexpr (Displayable<T>)
    {
      return content.display(rt, rect);
    }
    else
    {
      static_assert(false,
        "Type is not `Content` (neither `Primitive` nor `Displayable`)");
    }
  }

  //---------------------------------------------------------------------------
  // Widget::ContentValue<T> class implementation -----------------------------
  //---------------------------------------------------------------------------
  template<Content T>
  Rect Widget::ContentValue<T>::display(const Runtime& rt, const Rect& rect)
  {
    return displayContent(rt, content_, rect);
  }

  template<Content T>
  void* Widget::ContentValue<T>::resolveAs(const std::type_info& type)
  {
    if (typeid(content_) == type)
    {
      return &content_;
    }
    return nullptr;
  }

  //---------------------------------------------------------------------------
  // Widget::FactoryValue<F> class implementation -----------------------------
  //---------------------------------------------------------------------------
  template<ContentFactory F>
  void* Widget::FactoryValue<F>::resolveAs(const std::type_info& type)
  {
    resolve_();
    if (typeid(Resolved) == type)
    {
      return &resolved_.value();
    }
    if constexpr (std::same_as<Resolved, Widget>)
    {
      return resolved_->resolveAs(type);
    }
    return nullptr;
  }

  template<ContentFactory F>
  Rect Widget::FactoryValue<F>::display(const Runtime& rt, const Rect& rect)
  {
    resolve_();
    const Rect actualRect = displayContent(rt, *resolved_, rect);
    unresolve_();
    return actualRect;
  }

  template<ContentFactory F>
  void Widget::FactoryValue<F>::resolve_()
  {
    if (!resolved_.has_value())
    {
      resolved_.emplace(widget::resolve(factory_));
    }
  }

  template<ContentFactory F>
  void Widget::FactoryValue<F>::unresolve_()
  {
    if (resolved_.has_value())
    {
      resolved_.reset();
    }
  }

  //---------------------------------------------------------------------------
  // Widget class implementation ----------------------------------------------
  //---------------------------------------------------------------------------
  template<WidgetContent T>
  Widget::Widget(T value) : value_{ Value::create(std::move(value)) }
  {
    // empty body
  }

  template<Content T>
  T* Widget::resolveAs()
  {
    if (value_)
    {
      return static_cast<T*>(value_->resolveAs(typeid(T)));
    }
    return nullptr;
  }

  inline void* Widget::resolveAs(const std::type_info& type)
  {
    if (value_)
    {
      return value_->resolveAs(type);
    }
    return nullptr;
  }

  template<WidgetContent T>
  T* Widget::get()
  {
    if constexpr (Content<T>)
    { // Value is a Content type.
      if (auto* widgetValue = dynamic_cast<ContentValue<T>*>(value_.get()))
      {
        return &widgetValue->value;
      }
    }
    else if constexpr (ContentFactory<T>)
    { // Value is a ContentFactory type.
      if (auto* widgetValue = dynamic_cast<FactoryValue<T>*>(value_.get()))
      {
        return &widgetValue->value;
      }
    }
    else
    { // This static_assert will always fail if this branch is taken,
      // indicating that the type T is not a valid WidgetContent type.
      static_assert(std::same_as<T, void>,
          "Type is not `WidgetContent` (neither `Content` nor `ContentFactory`)");
    }
    // If the dynamic_cast fails, it means the stored value is not of type T,
    // so we return nullptr to indicate that the requested type is not present.
    return nullptr;
  }

  template<WidgetContent T>
  const T* Widget::get() const
  {
    return const_cast<Widget*>(this)->get<T>();
  }

  inline Rect Widget::display(const Runtime& rt, const Rect& rect)
  {
    return (value_ ? value_->display(rt, rect) : rt.display(Empty{}, rect));
  }
}
