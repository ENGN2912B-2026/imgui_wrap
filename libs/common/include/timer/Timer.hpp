//  Copyright (c) 2024-2026 Daniel Moreno. All rights reserved.
//

#pragma once

#include <memory>
#include <functional>

namespace timer
{
  //! \brief A simple timer class that can be used to schedule periodic tasks.
  //!
  //! The timer runs in a separate thread and calls the provided callback
  //! function at the specified interval, repeatedly until the timer is stopped
  //! or destroyed. This is not a one-shot timer.
  class Timer
  {
  public:
    //! \brief A callback function type that will be called when the timer
    //!        expires.
    using Callback = std::function<void()>;

    //! \brief Default constructor.
    Timer();

    //! \brief Copy constructor.
    Timer(const Timer&) = delete;

    //! \brief Move constructor.
    Timer(Timer&&) noexcept;

    //! \brief Destructor.
    ~Timer();

    //! \brief Assignment operator.
    Timer& operator=(const Timer&) = delete;

    //! \brief Move assignment operator.
    Timer& operator=(Timer&&) noexcept;

    //! \brief Start the timer.
    //! \param[in] milliseconds  The period of the timer in milliseconds.
    //! \param[in] callback      The callback function to be called at the
    //!                          specified interval.
    //! \note The callback function will be called in a separate thread.
    void start(size_t milliseconds, Callback callback);

    //! \brief Stop the timer.
    void stop();

    //! \brief Set the timer period.
    //! \param[in] milliseconds  The period of the timer in milliseconds.
    void setPeriod(size_t milliseconds);

    //! \brief Get the timer period.
    //! \return The period of the timer in milliseconds.
    size_t period() const;

    //! \brief Check if the timer is running.
    //! \return true if the timer is running, false otherwise.
    bool isRunning() const;

  private:
    class Impl;
    std::unique_ptr<Impl> impl_;
  };
} // namespace timer
