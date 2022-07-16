#include "ExecutionRecorder.hpp"
#include "TimeHelper.hpp"

#include <algorithm>
#include <optional>
#include <format>

Record::Record(IdType id, const std::string &description, const std::thread::id threadId) :
  _id{ id },
  _description(description),
  _threadId(threadId)
{
}

std::optional<Record> ExecutionRecorder::tryGetRecordAt(std::size_t position) const
{
    std::unique_lock lock(_locker);
    return _records.size() > position ? std::optional(_records[position]) : std::nullopt;
}

std::optional<Record> ExecutionRecorder::tryGetRecordBy(Record::IdType id) const
{
  std::unique_lock lock(_locker);
  auto elementIterator = std::find_if(std::cbegin(_records), std::cend(_records), 
    [id](const Record &record) { return record.getId() == id; });
  return elementIterator == std::cend(_records) ? std::optional(*elementIterator) : std::nullopt;
  
}

void ExecutionRecorder::pushBackRecord(Record &&record)
{
  std::unique_lock lock(_locker);
  _records.push_back(record);
}

std::size_t ExecutionRecorder::getRecordsCount() const
{
  std::unique_lock lock(_locker);
  return _records.size();
}

bool ExecutionRecorder::waitUnit(size_t recordCount, std::optional<Time::UnitType> timeout) const
{
  using namespace std::literals;
  Time::Point start = Time::Now();
  while (true)
  {
    if (getRecordsCount() == recordCount)
      return true;
    if (timeout.has_value() && timeout.value() <= Time::Now() - start)
      return false;
    std::this_thread::sleep_for(100ms);
  }
}

std::ostream &operator<<(std::ostream &os, const Record &record)
{
  return os << record.getThreadId() << std::format("\t{}\t{}", record.getId(), record.getDescription());
}

std::ostream &operator<<(std::ostream &os, const ExecutionRecorder &recorder)
{
  for (std::size_t index = 0, count = recorder.getRecordsCount(); index < count; ++index)
  {
    auto record = recorder.tryGetRecordAt(index);
    if (record.has_value())
      os << record.value();
  }
  return os;
}
