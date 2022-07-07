#include "TaskScheduler.hpp"
#include <exception>


TaskScheduler::TaskScheduler(int threadsNumber)
  : _workerNumber(static_cast<size_t>(threadsNumber))
{
  if (threadsNumber <= 0) 
    throw std::runtime_error("Task scheduler threads number can't be smalller or equel to zero");
}

TaskScheduler::~TaskScheduler()
{
  if(_workers.size())
    stop();
}

void TaskScheduler::addTask(std::shared_ptr<Task> task, int delayMs, bool repeatable)
{
  //TODO: implement deleysMs handling 
  //TODO: implement repeatable handling 
  initWorkers();
  {
    std::unique_lock<std::mutex> lock(_locker);
    _tasksQueue.push_back(task);
  }
  _conditionVriable.notify_one();
}

void TaskScheduler::stop()
{
  for (size_t i = 0; i < _workers.size(); ++i)
    addTask(nullptr);

  for (std::thread& worker : _workers)
    worker.join();
  _workers.clear();
}

void TaskScheduler::initWorkers()
{
  if (_workers.empty())
  {
    auto consume = [this]() {
      while (true)
      {
        std::shared_ptr<Task> task;
        {
          std::unique_lock<std::mutex> lock(_locker);
          _conditionVriable.wait(lock, [this]() { return !_tasksQueue.empty(); });
          task = std::move(_tasksQueue.back());
          _tasksQueue.pop_back();
        }
        if (task == nullptr)
          return;
        task->execute();
      }
    };
    for (size_t i = 0; i < _workerNumber; ++i)
      _workers.emplace_back(consume);
  }
}
