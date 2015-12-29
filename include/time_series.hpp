//
// Created by Karol Dzitkowski on 11.10.15.
//

#ifndef TIME_SERIES_DATA_READER_TIME_SERIES_H
#define TIME_SERIES_DATA_READER_TIME_SERIES_H

#include "column.hpp"

#include <string>
#include <vector>
#include <limits>
#include <algorithm>

class TimeSeries
{
public:
    TimeSeries() : _name(""), _recordsCnt(0) {}
    TimeSeries(std::string name) : _name(name), _recordsCnt(0) {}
    ~TimeSeries() {}
    TimeSeries(const TimeSeries& other)
            : _name(other._name), _columns(other._columns), _recordsCnt(other._recordsCnt)
    {}
    TimeSeries(TimeSeries&& other)
            : _name(std::move(other._name)),
              _columns(std::move(other._columns)),
              _recordsCnt(std::move(other._recordsCnt))
    {}

public:
    void init(std::vector<DataType> columnTypes)
    {
        for(auto& type : columnTypes)
            _columns.push_back(Column(type));
    }

    const Column& getColumn(size_t colIdx) { _columns[colIdx]; }
    std::string getName() { return _name; }
    void setName(std::string name) { _name = name; }
    size_t getColumnsNumber() { return _columns.size(); }
    size_t getRecordsCnt() { return _recordsCnt; }

    void setColumnNames(std::vector<std::string> names)
    {
        for(int i = 0; i < names.size(); i++)
            _columns[i].setName(names[i]);
    }

    std::vector<std::string> getColumnNames()
    {
        std::vector<std::string> result;
        for(auto& column : _columns)
            result.push_back(column.getName());
        return result;
    }

    size_t getRecordSize()
    {
        size_t size = 0;
        for(auto& column : _columns)
            size += column.getDataSize();
        return size;
    }

    void addRecord(char* data)
    {
        size_t offset = 0;
        for(auto& column : _columns)
            offset += column.addRawValue(data+offset);
        _recordsCnt++;
    }

    void addRecord(std::vector<std::string>& record)
    {
        if(record.size() != _columns.size())
            throw std::runtime_error(_recordSizeErrorMsg);

        for(int i = 0; i < record.size(); i++)
            _columns[i].addStringValue(record[i]);
        _recordsCnt++;
    }

    std::vector<RawData> getRawRecordData(size_t rowIdx)
    {
        std::vector<RawData> result;
        for(auto& column : _columns)
            result.push_back(column.getRaw(rowIdx));
        return result;
    }

    std::vector<std::string> getRecordAsStrings(size_t rowIdx)
    {
        std::vector<std::string> result;
        for(auto& column : _columns)
            result.push_back(column.getStringValue(rowIdx));
        return result;
    }

    bool compare(TimeSeries& other)
    {
        if(_recordsCnt != other.getRecordsCnt()) return false;
        for(int i = 0; i < getColumnsNumber(); i++)
            if(!_columns[i].compare(other.getColumn(i)))
                return false;
        return true;
    }

private:
    std::string _name;
    std::vector<Column> _columns;
    size_t _recordsCnt;

private:
    const char* _recordSizeErrorMsg =
            "Record size does not equal number of columns";
};

#endif //TIME_SERIES_DATA_READER_TIME_SERIES_H
