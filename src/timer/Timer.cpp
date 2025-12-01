//  Copyright (c) 2024-2025 Daniel Moreno. All rights reserved.
//

#include <timer/Timer.hpp>

#include "TimerInternal_Impl.hpp"

namespace timer
{
  Timer::Timer() : impl_{ std::make_unique<Impl>() }
  {
  }

  Timer::~Timer()
  {
  }

  void Timer::start(size_t milliseconds, Callback callback)
  {
    impl_->start(milliseconds, std::move(callback));
  }

  void Timer::stop()
  {
    impl_->stop();
  }

  void Timer::setPeriod(size_t milliseconds)
  {
    impl_->setPeriod(milliseconds);
  }

  size_t Timer::period() const
  {
    return impl_->period();
  }

  bool Timer::isRunning() const
  {
    return impl_->isRunning();
  }

} // namespace timer
