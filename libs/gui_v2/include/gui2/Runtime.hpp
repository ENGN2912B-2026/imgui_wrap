//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <gui2/Types.hpp>
#include <gui2/Rect.hpp>
#include <backend/Backend.hpp>

#include <string>
#include <memory>
#include <concepts>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace gui2
{
  // forward declaration
  class Empty;
  class Separator;
  class Image;
  class Button;
  class CheckBox;
  class TextBox;
  class ImageZoom;
  class Panel;

  //! \brief The Runtime class manages the GUI runtime environment, including
  //!        the backend and main window.
  //!
  //! The Runtime class is a singleton that provides methods to initialize and
  //! deinitialize the GUI runtime, manage the backend, and display various GUI
  //! items. It also provides methods to get and set the window title and size,
  //! and to begin and end rendering frames.
  class Runtime
  {
  public:
    //! \brief Destructor for the Runtime class.
    ~Runtime() { uninitialize(); }
    static Runtime& getInstance();

    //! \brief Checks if the runtime is initialized.
    //! \return true if the runtime is initialized, false otherwise.
    bool isInitialized() const { return backend_ != nullptr; }

    //! \brief Initializes the runtime and creates a desktop window.
    //! \param[in] windowTitle The title of the desktop window.
    //! \param[in] windowSize The size of the desktop window in pixels.
    //!                       When the window is created by the system, its
    //!                       actual size is usually scaled according to the
    //!                       display DPI, so the actual size of the window
    //!                       when first shown may be different.
    //! \note This method must be called before any other methods of the
    //!       Runtime class are used. If the runtime is already initialized,
    //!       this method does nothing.
    void initialize(const std::string& windowTitle = "Window",
                    const Vec2i& windowSize = {640, 480});

    //! \brief Uninitializes the runtime and destroys the desktop window.
    //! \note This method should be called when the application is about to
    //!       exit to clean up resources. If the runtime is not initialized,
    //!       this method does nothing.
    void uninitialize();

    //! \brief Sets the window title of the desktop window.
    //! \param[in] title The title of the desktop window.
    void setWindowTitle(const std::string& title);

    //! \brief Gets the current window size of the desktop window.
    //! \return The size of the desktop window in pixels.
    Vec2i getWindowSize() const;

    //! \brief Sets the window size of the desktop window.
    //! \param[in] size The size of the desktop window in pixels.
    void setWindowSize(const Vec2i& size);

    //! \brief Begins a new rendering frame.
    //! \return true if the frame was successfully begun, false otherwise.
    bool frameBegin();

    //! \brief Ends the current rendering frame.
    void frameEnd();

    // Display functions ------------------------------------------------------

    // Primitive items which do not have state
    Rect display(const Empty& empty, const Rect& rect) const;
    Rect display(const Separator& separator, const Rect& rect) const;
    Rect display(const std::string& text, const Rect& rect) const;
    Rect display(const Button& button, const Rect& rect) const;
    Rect display(const Image& image, const Rect& rect) const;

    // Primitive items which have state which might change during display
    Rect display(CheckBox& checkBox, const Rect& rect) const;
    Rect display(TextBox& textBox, const Rect& rect) const;
    Rect display(ImageZoom& imageZoom, const Rect& rect) const;

    // Container items, they maybe have as children other items
    Rect display(Panel& panel, const Rect& rect) const;

    // Content sizing ---------------------------------------------------------

    //! \brief Begins a sizing operation for a content item with the given ID.
    //! \param[in] id The unique identifier of the content item to size.
    //! \note This function should be called before displaying the content item
    //!       to be sized.
    void sizeBegin(const std::string& id) const;

    //! \brief Ends the current sizing operation.
    //! \note This function should be called after displaying the content item
    //!       to be sized.
    void sizeEnd() const;

    //! \brief Gets the sized rectangle for a content item with the given ID.
    //! \param[in] id The unique identifier of the content item.
    //! \return The sized rectangle for the content item, or an empty rectangle
    //!         if the content item has not been sized yet.
    Rect getSizedRect(const std::string& id) const;

    // Other functions --------------------------------------------------------

    //! \brief Gets the padding of the desktop window and child windows.
    //! \return The padding of the desktop window and child windows.
    //! \note The padding is the space between the window border and the content
    //!       area of the window. It is used to compute the inner rectangle of
    //!       the window where the content is displayed.
    Vec2i getWindowPadding() const;

    //! \brief Gets the padding of the frame around items in a layout.
    //! \return The padding of the frame around items in a layout.
    //! \note The frame padding is the space between the frame border and the
    //!       content area of the frame. It is used by most widgets.
    Vec2i getFramePadding() const;

    //! \brief Gets the spacing between items in a layout.
    //! \return The spacing between items in a layout.
    //! \note The spacing is the space between the items in the layout.
    Vec2i getItemSpacing() const;

    //! \brief Gets the height of a single line of text.
    //! \return The height of a single line of text in pixels.
    size_t getFontSize() const;

    //! \brief Gets a reference to the backend used by the runtime.
    //! \return A reference to the backend used by the runtime.
    //! \throw std::runtime_error if the runtime is not initialized.
    backend::Backend& getBackend() const;

  private:
    std::unique_ptr<backend::Backend> backend_;
    mutable std::unordered_map<std::string, Rect> sizedRects_;
    mutable std::vector<std::pair<std::string, Rect>> sizingStack_;
    Runtime() = default;
  };

  //! \namespace runtime
  //! \brief Contains helper templates and functions used by the Runtime class.
  namespace runtime
  {
    //! \brief Resolves a value by removing any reference wrappers if present.
    //! \tparam T The type of the value to resolve.
    //! \param value The value to resolve.
    //! \return The resolved value.
    template<typename T> inline decltype(auto) resolve_reference(T& value);

    // Helper struct to determine if a type is a factory (a callable that
    // returns a Content type, or a factory that returns another factory,
    // recursively).
    template <typename T> struct is_factory : std::false_type{};
  }

  // Concept of primitive types:
  // - Runtime has a display method for the type.
  template<typename T>
  concept Primitive =
    requires(T& value, Runtime& rt, Rect& rect)
    {
      { rt.display(runtime::resolve_reference(value), rect) }
        -> std::convertible_to<Rect>;
    };

  // Concept of displayable types:
  // - The type has a display method that takes a Runtime and a Rect and returns
  //   a Rect.
  template<typename T>
  concept Displayable =
    requires(T& value, Runtime& rt, Rect& rect)
    {
      { runtime::resolve_reference(value).display(rt, rect) }
        -> std::convertible_to<Rect>;
    };

  // Concept of content types:
  // - The type is either primitive or displayable.
  template<typename T>
  concept Content = Primitive<T> || Displayable<T>;

  // Concept of content factory types:
  // - The type is a callable that returns a Content type.
  // - The type is a callable that returns another factory type.
  template<typename F>
  concept ContentFactory = runtime::is_factory<std::remove_cvref_t<F>>::value;

} // namespace gui

// Implementation -------------------------------------------------------------
namespace gui2
{
  namespace runtime
  {
    // Resolves a value by removing any reference wrappers if present.
    template<typename T>
    inline T& resolve_reference_impl(T& value)
    {
      return value;
    }

    // Specialization for std::reference_wrapper<T>: returns the underlying
    // reference.
    template<typename T>
    inline T& resolve_reference_impl(std::reference_wrapper<T> value)
    {
      return value.get();
    }

    // Resolves a value by removing any reference wrappers if present.
    template<typename T>
    inline decltype(auto) resolve_reference(T& value)
    {
      return resolve_reference_impl(value);
    }

    // Specialization for callable types: the type is a factory if it returns a
    // Content type or another factory type.
    template<typename F>
      requires std::invocable<F&>
    struct is_factory<F>
    {
      using Result = std::remove_cvref_t<std::invoke_result_t<F&>>;
      static constexpr bool value =
        Content<Result> || is_factory<Result>::value;
    };
  }
}
