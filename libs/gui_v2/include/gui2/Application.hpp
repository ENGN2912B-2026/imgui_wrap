//  Copyright (c) 2026 Daniel Moreno. All rights reserved.
//
#pragma once

#include <gui2/Window.hpp>

#include <string>
#include <atomic>

namespace gui2
{
  //! \brief The Application class represents a GUI application that manages the
  //!        main window and the event loop.
  //!
  //! The Application class is responsible for creating and managing the main
  //! window of the application, handling the event loop, and providing methods
  //! to run and quit the application. It encapsulates the main window and
  //! provides an interface for interacting with it.
  class Application
  {
  public:
    //! \brief Constructs a  GUI Application.
    //! \param[in] windowTitle The title of the application window.
    //! \param[in] windowSize The size of the application window in pixels.
    //!                       When the window is created by the system, its
    //!                       actual size is usually scaled according to the
    //!                       display DPI, so the actual size of the window
    //!                       when first shown may be different.
    //! \note The Application instance owns the main window and manages its
    //!       lifecycle.
    Application(const std::string& windowTitle = "Window",
                const Vec2i& windowSize = {640, 480});

    //! \brief Application cannot be copied.
    Application(const Application&) = delete;

    //! \brief Application cannot be moved.
    Application(Application&&) noexcept = delete;

    //! \brief Destructor for the Application class.
    virtual ~Application();

    //! \brief Application cannot be copy-assigned.
    Application& operator=(const Application&) = delete;

    //! \brief Application cannot be move-assigned.
    Application& operator=(Application&&) noexcept = delete;

    //! \brief Gets a reference to the main window of the application.
    //! \return A reference to the main Window object.
    Window& getWindow() { return window_; }

    //! \brief Gets a const reference to the main window of the application.
    //! \return A const reference to the main Window object.
    const Window& getWindow() const { return window_; }

    //! \brief Checks if the application is currently running.
    //! \return true if the application is running, false otherwise.
    bool isRunning() const { return running_; }

    //! \brief Runs the application, entering the main event loop.
    //! \note This function blocks until the application is quit.
    void run();

    //! \brief Quits the application, exiting the main event loop.
    //! \note This function can be called from any thread to request the
    //!       application to quit. The application will exit the main event loop
    //!       and terminate gracefully.
    void quit() { running_ = false; }

    //! \brief Gets the application instance.
    //! \return A reference to the Application instance.
    //! \throw std::logic_error if the application instance does not exist.
    static Application& getInstance();

  private:
    Window window_;
    std::atomic<bool> running_;
    static Application* instance_;
  };

  //! \brief Gets a reference to the application instance of type T.
  //! \tparam T The type of the application instance to retrieve.
  //! \return A reference to the application instance of type T.
  //! \throw std::bad_cast if the application instance is not of type T.
  //! \throw std::logic_error if the application instance does not exist.
  template <typename T>
  requires std::is_base_of<Application, T>::value
  inline T& getApp()
  {
    // throws std::bad_cast if the instance is not of type T
    return dynamic_cast<T&>(Application::getInstance());
  }

} // namespace gui

// Declares `getApp()` function that returns a reference to the global
//  application instance casted to the specified `AppType`.
//
// Usage:
// ```cpp
// class MyApplication : public Application
// {
//   ...
// };
//
// DECLARE_APPLICATION(MyApplication)
//
// int main()
// {
//   getApp().run();
//   return 0;
// }
// ```
#define DECLARE_APPLICATION(AppType) \
  inline AppType& getApp() { return gui2::getApp<AppType>(); }
