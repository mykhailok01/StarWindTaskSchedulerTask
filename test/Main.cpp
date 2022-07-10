#include "TaskScheduler.hpp"
#include "TimeHelper.hpp"
#include "ExecutionRecorder.hpp"

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
  ExecutionRecorder executionRecorder;
  auto taskFuntion = [&executionRecorder]()
  {
    executionRecorder.pushBackRecord({ 0, "Start a task\n" });
    WorkFor(5s);
    executionRecorder.pushBackRecord({ 1, "Task is finished after 5s\n" });
  };
  taskScheduler.addTask(std::make_shared<FunctionTask>(taskFuntion));
  executionRecorder.waitUnit(2);
  std::cout << executionRecorder;
}
