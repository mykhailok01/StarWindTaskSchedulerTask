#include "TaskScheduler.hpp"
#include "TimeHelper.hpp"

#include <iostream>
#include <functional>

void WorkFor(Time::Duration duration)
{
  Time::Point startTime = Time::Now();
  do; while (Time::Now() - startTime < duration);
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
