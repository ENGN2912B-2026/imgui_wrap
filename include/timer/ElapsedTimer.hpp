//  Copyright (c) 2025 Daniel Moreno. All rights reserved.
//

#pragma once

#include <chrono>

namespace timer
{
  // ElapsedTimer measures elapsed time between its construction
  // (or last reset) and the call to elapsed().
  class ElapsedTimer
  {
    std::chrono::high_resolution_clock::time_point startTimePoint_;
  public:
    // Constructor initializes the start time point to the current time.
    ElapsedTimer()
      : startTimePoint_{ std::chrono::high_resolution_clock::now() }
    {
    }

    // Resets the start time point to the current time.
    void reset()
    {
      startTimePoint_ =  std::chrono::high_resolution_clock::now();
    }

    // Returns the elapsed time in seconds since construction or last reset.
    double elapsed() const
    {
      const auto endTimePoint{ std::chrono::high_resolution_clock::now() };
      const std::chrono::duration<double> elapsedDuration{
        endTimePoint - startTimePoint_ };
      return elapsedDuration.count();
    }
  };
} // namespace timer
