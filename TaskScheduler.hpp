#ifndef TASK_SCHEDULER_HPP
#define TASK_SCHEDULER_HPP
#include <memory>

class Task
{
public:
  virtual ~Task() = default;
  virtual void execute() = 0;
};

class TaskScheduler
{
public:
  TaskScheduler(int threadsNumber = 1);
  virtual ~TaskScheduler();

  void addTask(std::shared_ptr<Task> task, int delayMs = 0, bool repeatable = false);
  void stop();
};
#endif  // !TASK_SCHEDULER_HPP