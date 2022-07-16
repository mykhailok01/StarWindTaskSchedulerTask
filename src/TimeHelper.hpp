#ifndef TIME_HELPER_HPP
#define TIME_HELPER_HPP
#include <chrono>

class Time
{
public:
  using UnitType = std::chrono::milliseconds;
  using Clock = std::chrono::high_resolution_clock;
  using Point = std::chrono::time_point<Clock, UnitType>;


  static inline Point Now()
  {
    return std::chrono::time_point_cast<UnitType>(Clock::now());
  }
};
#endif