#ifndef TASK_SCHEDULER_HPP
#define TASK_SCHEDULER_HPP
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

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

private:
  void initWorkers();

private:
  size_t _workerNumber;
  std::vector<std::thread> _workers;
  std::vector<std::shared_ptr<Task>> _tasksQueue;
  std::mutex _locker;
  std::condition_variable _conditionVriable;

};
#endif  // !TASK_SCHEDULER_HPP