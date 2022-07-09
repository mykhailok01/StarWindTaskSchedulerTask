#ifndef TIME_HELPER_HPP
#define TIME_HELPER_HPP
#include <chrono>

class Time
{
public:
  using Duration = std::chrono::microseconds;
  using Clock = std::chrono::high_resolution_clock;
  using Point = std::chrono::time_point<Clock, Duration>;


  static inline Point Now()
  {
    return std::chrono::time_point_cast<Duration>(Clock::now());
  }
};
#endif