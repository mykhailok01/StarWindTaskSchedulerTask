#include "TaskScheduler.hpp"
#include "TimeHelper.hpp"
#include "ExecutionRecorder.hpp"

#include <iostream>
#include <functional>

void WorkFor(Time::UnitType duration)
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

void FillWithTasks(TaskScheduler &taskScheduler, std::function<void()> taskRepeat, std::size_t repeatsNumber)
{
  auto taskFunction = std::make_shared<FunctionTask>(taskRepeat);
  for (std::size_t i = 0; i < repeatsNumber; ++i)
  {
    taskScheduler.addTask(taskFunction);
  }
}

std::unique_ptr<ExecutionRecorder> DemonstrateConsumerProducerPart(std::size_t threadsNumber, std::size_t tasksNumber, Time::UnitType waitTimeout, Time::UnitType workDuration)
{
  using namespace std::literals;
  auto recorder = std::make_unique<ExecutionRecorder>();
  std::thread::id hostThreadId = std::this_thread::get_id();
  recorder->pushBackRecord({ 0,
    std::format("Demonstration started with: (threadsNumber = {}, tasksNumber = {}, waitTimeout = {}, workDuration= {})\n",
      threadsNumber, tasksNumber,  waitTimeout, workDuration), hostThreadId });

  TaskScheduler taskScheduler(threadsNumber);
  auto taskLamda = [&recorder, workDuration]()
  {
    std::thread::id thisId = std::this_thread::get_id();
    recorder->pushBackRecord({ 1, std::format("Start a task.\n"), thisId });
    WorkFor(workDuration);
    recorder->pushBackRecord({ 2, std::format("Task is finished after {}.\n", workDuration), thisId });
  };
  Time::Point beforeDemoTime = Time::Now();
  FillWithTasks(taskScheduler, taskLamda, tasksNumber);
  bool noTimeout = recorder->waitUnit(tasksNumber * 2 + 1, waitTimeout);
  Time::UnitType exectutionTime = Time::Now() - beforeDemoTime;
  taskScheduler.stop();
  if (noTimeout)
    recorder->pushBackRecord({ 3, std::format("Demonstration ended. Total time {}\n\n", exectutionTime) , hostThreadId });
  else
  {
    std::this_thread::sleep_for(workDuration);
    recorder->pushBackRecord({ 4, std::format("Demonstration ended. It failed with timeout. Total time {}\n\n", exectutionTime), hostThreadId});
  }
  return recorder;
}

void PrintDemoOutput(std::unique_ptr<ExecutionRecorder> record)
{
  std::cout << *record;
}

int main()
{
  using namespace std::literals;

  PrintDemoOutput(DemonstrateConsumerProducerPart(1, 8, 9s, 1s));
  PrintDemoOutput(DemonstrateConsumerProducerPart(2, 8, 5s, 1s));
  PrintDemoOutput(DemonstrateConsumerProducerPart(3, 8, 3s, 1s));
  PrintDemoOutput(DemonstrateConsumerProducerPart(4, 8, 2s + 500ms, 1s));
}
