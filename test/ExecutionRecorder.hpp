#ifndef EXECUTION_RECORDER_HPP
#define EXECUTION_RECORDER_HPP

#include "TimeHelper.hpp"
#include <string>
#include <mutex>
#include <vector>
#include <optional>

class Record final
{
public:
  using IdType = std::size_t;

  Record(IdType id, const std::string &description);
  Record(const Record &record) = default;
  Record(Record &&record) = default;
  Record &operator=(const Record &record) = default;
  Record &operator=(Record &&record) = default;

  IdType getId() const { return _id; }
  const std::string &getDescription() const { return _description; }
private:
  IdType _id;
  std::string _description;
};

class ExecutionRecorder final
{
public:
  std::optional<Record> tryGetRecordAt(std::size_t position) const;
  std::optional<Record> tryGetRecordBy(Record::IdType id) const;
  void pushBackRecord(Record &&record);
  std::size_t getRecordsCount() const;
  bool waitUnit(size_t recordCount, std::optional<Time::Duration> timeout = std::nullopt) const;
private:
  mutable std::mutex _locker;
  std::vector<Record> _records;
};

#endif