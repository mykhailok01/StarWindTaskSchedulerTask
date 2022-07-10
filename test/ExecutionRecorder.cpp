#include "ExecutionRecorder.hpp"
#include "TimeHelper.hpp"

#include <algorithm>
#include <optional>

Record::Record(IdType id, const std::string &description) :
  _id{ id },
  _description(description)
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

bool ExecutionRecorder::waitUnit(size_t recordCount, std::optional<Time::Duration> timeout) const
{
  Time::Point start = Time::Now();
  while (true)
  {
    if (getRecordsCount() == recordCount)
      return true;
    if (timeout.has_value() && timeout.value() <= Time::Now() - start)
      return false;
  }
}

