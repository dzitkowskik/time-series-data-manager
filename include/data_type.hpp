/*
 *  data_type.hpp
 *
 *  Created on: 17-09-2015
 *      Author: Karol Dzitkowski
 */

#ifndef DDJ_DATA_TYPE_HPP_
#define DDJ_DATA_TYPE_HPP_

#include <cstring>
#include <time.h>

enum class DataType
{
    d_time,
    d_char,
    d_int,
    d_unsigned,
    d_float,
    d_double
};

inline size_t GetDataTypeSize(DataType type)
{
    switch(type)
    {
        case DataType::d_time: return sizeof(time_t);
        case DataType::d_char: return sizeof(char);
        case DataType::d_int: return sizeof(int);
        case DataType::d_unsigned: return sizeof(unsigned);
        case DataType::d_float: return sizeof(float);
        case DataType::d_double: return sizeof(double);
    }
    return 0;
}

template<typename T> inline DataType GetDataType();
template<> inline DataType GetDataType<time_t>() { return DataType::d_time; }
template<> inline DataType GetDataType<char>() { return DataType::d_char; }
template<> inline DataType GetDataType<int>() { return DataType::d_int; }
template<> inline DataType GetDataType<unsigned>() { return DataType::d_unsigned; }
template<> inline DataType GetDataType<float>() { return DataType::d_float; }
template<> inline DataType GetDataType<double>() { return DataType::d_double; }

#endif /* DDJ_DATA_TYPE_HPP_ */
