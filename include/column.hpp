//
// Created by Karol Dzitkowski on 20.12.15.
//

#ifndef TIME_SERIES_DATA_READER_COLUMN_H
#define TIME_SERIES_DATA_READER_COLUMN_H

#include "data_type.h"

#include <stddef.h>
#include <stdexcept>
#include <boost/lexical_cast.hpp>

struct RawData
{
    char* Data;
    size_t Size;
};

class Column
{
public:
    Column(DataType type) : _type(type), _name("")
    { init(0); }

    Column(DataType type, std::string name) : _type(type), _name(name)
    { init(0); }

    Column(DataType type, size_t initialSize) : _type(type), _name("")
    { init(initialSize); }

    Column(DataType type, size_t initialSize, std::string name)
            : _type(type), _name(name)
    { init(initialSize); }

    ~Column()
    { delete [] _data; }

    Column(const Column& other)
            : _type(other._type),
              _actualSize(other._actualSize),
              _allocatedSize(other._allocatedSize),
              _dataSize(other._dataSize)
    {
        _data = new char[_allocatedSize];
        memcpy(_data, other._data, _actualSize);
    }
public:
    std::string getName() { return _name; }
    size_t getSize() { return _actualSize; }
    size_t getDataSize() { return _dataSize; }
    DataType getType() { return _type; }
    char* getData() { return _data; }

    template<typename T> T getValue(size_t index)
    {
        checkIndex(index);

        // Check if type of data is correct
        if(GetDataType<T>() != _type)
            throw std::runtime_error(_wrongDataTypeErrorMsg);

        // Return data
        T* actualData = (T*)_data;
        return actualData[index];
    }

    RawData getRaw(size_t index)
    {
        checkIndex(index);
        size_t actualIndex = index * _dataSize;
        return RawData { _data+actualIndex, _dataSize };
    }

    std::string getStringValue(size_t index)
    {
        time_t time;
        std::string timeString;
        switch (_type)
        {
            case DataType::d_time:
                time = getValue<time_t>(index);
                timeString = ctime(const_cast<const time_t*>(&time));
                timeString.erase(timeString.length()-1);
                return timeString;
            case DataType::d_char:
                return boost::lexical_cast<std::string>(getValue<char>(index));
            case DataType::d_int:
                return boost::lexical_cast<std::string>(getValue<int>(index));
            case DataType::d_unsigned:
                return boost::lexical_cast<std::string>(getValue<unsigned >(index));
            case DataType::d_float:
                return boost::lexical_cast<std::string>(getValue<float>(index));
            case DataType::d_double:
                return boost::lexical_cast<std::string>(getValue<double>(index));
        }
    }

    template<typename T> T getCastedValue(size_t index)
    {
        checkIndex(index);
        size_t actualIndex = index * _dataSize;
        switch (_type)
        {
            case DataType::d_time:
                return static_cast<T>(*((time_t*)(_data+actualIndex)));
            case DataType::d_char:
                return static_cast<T>(*(_data+actualIndex));
            case DataType::d_int:
                return static_cast<T>(*((int*)(_data+actualIndex)));
            case DataType::d_unsigned:
                return static_cast<T>(*((unsigned*)(_data+actualIndex)));
            case DataType::d_float:
                return static_cast<T>(*((float*)(_data+actualIndex)));
            case DataType::d_double:
                return static_cast<T>(*((double*)(_data+actualIndex)));
        }
    }

    template<typename T> void addValue(T value)
    {
        // Check if type of data is correct
        if(GetDataType<T>() != _type)
            throw std::runtime_error(_wrongDataTypeErrorMsg);

        // if we don't have enough storage - expand the array
        if(_allocatedSize < _actualSize + _dataSize)
            expand();

        // set next value
        memcpy(_data+_actualSize, &value, _dataSize);
        _actualSize += _dataSize;
    }

    void addStringValue(std::string& value)
    {
        std::tm time;
        switch (_type)
        {
            case DataType::d_time:
                strptime(value.c_str(), "%c", &time);
                time.tm_isdst = -1;
                addValue<time_t>(mktime(&time)); break;
            case DataType::d_char:
                addValue<char>(boost::lexical_cast<char>(value)); break;
            case DataType::d_int:
                addValue<int>(boost::lexical_cast<int>(value)); break;
            case DataType::d_unsigned:
                addValue<unsigned>(boost::lexical_cast<unsigned>(value)); break;
            case DataType::d_float:
                addValue<float>(boost::lexical_cast<float>(value)); break;
            case DataType::d_double:
                addValue<double>(boost::lexical_cast<double>(value)); break;
        }
    }

    size_t addRawValue(char* data)
    {
        // if we don't have enough storage - expand the array
        if(_allocatedSize < _actualSize + _dataSize)
            expand();

        memcpy(_data+_actualSize, data, _dataSize);
        _actualSize += _dataSize;
        return _dataSize;
    }

    void setName(std::string name) { _name = name; }

    bool compare(Column other)
    {
        if(_type != other.getType() || _actualSize != other.getSize())
            return false;
        auto result = std::memcmp(_data, other.getData(), _actualSize);
        return result == 0;
    }

private:
    void init(size_t initSize)
    {
        _allocatedSize = initSize;
        _data = new char[initSize];
        _actualSize = 0;
        _dataSize = GetDataTypeSize(_type);
    }

    void expand()
    {
        auto oldSize = _allocatedSize;
        auto oldData = _data;
        _allocatedSize = 2 * _allocatedSize + 4 * _dataSize;
        _data = new char[_allocatedSize];
        memcpy(_data, oldData, oldSize);
        delete [] oldData;
    }

    void checkIndex(size_t index)
    {
        if((index * _dataSize) > _actualSize)
            throw std::runtime_error(_indexOutOfBoundsErrorMsg);
    }

private:
    size_t _allocatedSize;
    size_t _dataSize;
    size_t _actualSize;
    char* _data;
    DataType _type;
    std::string _name;

private:
    const std::string _wrongDataTypeErrorMsg =
            "Data type of column is not compatible with requested type";

    const std::string _indexOutOfBoundsErrorMsg =
            "There is no data on that position - index is too big";
};

#endif //TIME_SERIES_DATA_READER_COLUMN_H
