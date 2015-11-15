//
// Created by Karol Dzitkowski on 11.10.15.
//

#ifndef TIME_SERIES_DATA_READER_TIME_SERIES_H
#define TIME_SERIES_DATA_READER_TIME_SERIES_H

#include <string>
#include <vector>
#include "file.h"
#include "ts_file_definition.h"
#include <limits>

template<typename DataType = double>
class TimeSeries
{
public:
    TimeSeries() : _name("") {}
    TimeSeries(std::string name) : _name(name) {}
    ~TimeSeries() {}
    TimeSeries(const TimeSeries& other)
            : _name(other._name), _data(other._data), _time(other._time)
    {}
    TimeSeries(TimeSeries&& other)
            : _name(std::move(other._name)),
              _data(std::move(other._data)),
              _time(std::move(other._time))
    {}

//    GETTERS
public:
    std::string GetName() const { return _name; }
    std::vector<DataType> GetData() const { return _data; }
    std::vector<time_t> GetTime() const { return _time; }
    const DataType* GetDataRaw() const { return _data.data(); }
    const time_t* GetTimeRaw() const { return _time.data(); }
    size_t GetSize() const { return _time.size(); }

//    SETTERS
public:
    void SetName(std::string name) { _name = name; }
    void SetData(std::vector<DataType> data) { _data = data; }
    void SetTime(std::vector<time_t> time) { _time = time; }
    void InsertData(DataType value) { _data.push_back(value); }
    void InsertTime(time_t value) { _time.push_back(value); }
    void Insert(std::pair<time_t, DataType> item)
    {
        _time.push_back(item.first);
        _data.push_back(item.second);
    }

//    ACCESS METHODS
public:
    std::string& name() { return _name; }
    std::vector<DataType>& data() { return _data; }
    std::vector<time_t>& time() { return _time; }

//    ITERATORS
public:
    typename std::vector<DataType>::iterator BeginData() { return _data.begin(); }
    typename std::vector<DataType>::iterator EndData() { return _data.end(); }
    typename std::vector<time_t>::iterator BeginTime() { return _time.begin(); }
    typename std::vector<time_t>::iterator EndTime() { return _time.end(); }
    typename std::vector<DataType>::const_iterator BeginData() const { return _data.cbegin(); }
    typename std::vector<DataType>::const_iterator EndData() const { return _data.cend(); }
    typename std::vector<time_t>::const_iterator BeginTime() const { return _time.cbegin(); }
    typename std::vector<time_t>::const_iterator EndTime() const { return _time.cend(); }

//    READ WRITE FROM FILES
public:
    static
    std::vector<TimeSeries<DataType>> ReadFromCSV(
            File& file,
            CSVFileDefinition definition,
            const int maxRows = INT32_MAX);

    static
    void WriteToCSV(
            File& file,
            std::vector<TimeSeries<DataType>> series,
            CSVFileDefinition definition);

    static
    std::vector<TimeSeries<DataType>> ReadFromBinary(
            File& file,
            BinaryFileDefinition definition,
            const int maxRows = INT32_MAX);

    static
    void WriteToBinary(
            File& file,
            std::vector<TimeSeries<DataType>> series);

//    OPERATORS
public:
    bool Equal(const TimeSeries& other) const
    {
        return (*this) == other;
    }

    bool operator==(const TimeSeries& other) const
	{
        return !(other._data != this->_data || other._time != this->_time);
    }

    std::pair<time_t, DataType> operator[](const int) const;

//    FIELDS
private:
    std::string _name;
    std::vector<DataType> _data;
    std::vector<time_t> _time;
};

#endif //TIME_SERIES_DATA_READER_TIME_SERIES_H
