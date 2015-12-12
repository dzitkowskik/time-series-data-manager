/*
 * time_series_reader.hpp
 *
 *  Created on: Dec 12, 2015
 *      Author: Karol Dzitkowski
 */

#ifndef TIME_SERIES_READER_HPP_
#define TIME_SERIES_READER_HPP_

#include "time_series.hpp"

class TimeSeriesReader
{
public:
	TimeSeriesReader()
		: _bytesRead(0),
		  _rowsRead(0),
		  _lastFilePosition(0)
	{}
	virtual ~TimeSeriesReader() {}

public:
	template<typename DataType = double>
    std::vector<TimeSeries<DataType>> ReadFromCSV(
            File& file,
            CSVFileDefinition definition,
            const int maxRows = INT32_MAX);

	template<typename DataType = double>
    std::vector<TimeSeries<DataType>> ReadFromBinary(
            File& file,
            BinaryFileDefinition definition,
            const int maxRows = INT32_MAX);

private:
	size_t _bytesRead;
	size_t _rowsRead;
	size_t _lastFilePosition;
};

#endif /* TIME_SERIES_READER_HPP_ */
