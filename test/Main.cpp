#include "TaskScheduler.hpp"
#include <chrono>
#include <iostream>
#include <functional>

using TimeDuration = std::chrono::microseconds;
using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock, TimeDuration>;

TimePoint GetNow()
{
  return std::chrono::time_point_cast<TimeDuration>(Clock::now());
}


void WorkFor(TimeDuration duration)
{
  TimePoint startTime = GetNow();
  do; while (GetNow() - startTime < duration);
}

class FunctionTask : public Task
{
  std::function<void()> _function;
public:
  FunctionTask(std::function<void()> function) 
  {
    _function = function;
  }

  void execute() override
  {
    _function();
  }
};

int main()
{
  using namespace std::literals;
  TaskScheduler taskScheduler(1);
  auto taskFuntion = []()
  {
    std::cout << "Start a task\n";
    WorkFor(5s);
    std::cout << "Out after 5s\n";
  };
  taskScheduler.addTask(std::make_shared<FunctionTask>(taskFuntion));
  std::this_thread::sleep_for(10s);
}
