//  Copyright (c) 2024-2025 Daniel Moreno. All rights reserved.
//

#pragma once

#include <memory>
#include <functional>

namespace timer
{
  class Timer
  {
    class Impl;
    std::unique_ptr<Impl> impl_;
  public:
    // Callback type
    using Callback = std::function<void()>;

    // Default constructor
    Timer();

    // Destructor
    ~Timer();

    // Start the timer
    void start(size_t milliseconds, Callback callback);

    // Stop the timer
    void stop();

    // Set the timer period
    void setPeriod(size_t milliseconds);

    // Get the timer period
    size_t period() const;

    // Check if the timer is running
    bool isRunning() const;
  };
} // namespace timer
