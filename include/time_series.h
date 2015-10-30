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
    TimeSeries() : _name(""), _readMaxRows(INT32_MAX) {}
    TimeSeries(std::string name) : _name(name), _readMaxRows(INT32_MAX) {}
    ~TimeSeries() {}
    TimeSeries(const TimeSeries& other)
            : _name(other._name), _data(other._data), _time(other._time), _readMaxRows(other._readMaxRows)
    {}
    TimeSeries(TimeSeries&& other)
            : _name(std::move(other._name)),
              _data(std::move(other._data)),
              _time(std::move(other._time)),
              _readMaxRows(std::move(other._readMaxRows))
    {}

public:
    std::string GetName() const { return _name; }
    size_t GetSize() const { return _time.size(); }
    const DataType* GetData() const { return _data.data(); }
    const time_t* GetTime() const { return _time.data(); }

    void InsertData(DataType value) { _data.push_back(value); }
    void InsertTime(time_t value) { _time.push_back(value); }

    void SetReadMaxRows(int max) { _readMaxRows = max; }

    typename std::vector<DataType>::iterator BeginData() { return _data.begin(); }
    typename std::vector<DataType>::iterator EndData() { return _data.end(); }
    typename std::vector<time_t>::iterator BeginTime() { return _time.begin(); }
    typename std::vector<time_t>::iterator EndTime() { return _time.end(); }
    typename std::vector<DataType>::const_iterator BeginData() const { return _data.cbegin(); }
    typename std::vector<DataType>::const_iterator EndData() const { return _data.cend(); }
    typename std::vector<time_t>::const_iterator BeginTime() const { return _time.cbegin(); }
    typename std::vector<time_t>::const_iterator EndTime() const { return _time.cend(); }

public:
    std::vector<TimeSeries<DataType>> ReadManyFromFile(
            const File& file,
            TSFileDefinition definition);

    void WriteManyToFile(
            const File& file,
            std::vector<TimeSeries<DataType>> series,
            TSFileDefinition definition);

//    void InitFromFile(File file, std::string name);
//    void SaveToFile(File file, std::string name);

public:
    bool Equal(const TimeSeries& other) const
    {
//        if(this->GetSize() != other.GetSize()) return false;
//        auto otherTimeIt = other.BeginTime();
//        auto otherDataIt = other.BeginData();
//        for(auto& time : _time) if(time != *otherTimeIt++) return false;
//        for(auto& data : _data) if(data != *otherDataIt++) return false;
        return (*this) == other;
    }

    bool operator==(const TimeSeries& other) const
	{
        return !(other._data != this->_data || other._time != this->_time);
    }

private:
    std::string _name;
    std::vector<DataType> _data;
    std::vector<time_t> _time;
    int _readMaxRows;
};

#endif //TIME_SERIES_DATA_READER_TIME_SERIES_H
