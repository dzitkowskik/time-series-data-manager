/*
 * time_series_reader.hpp
 *
 *  Created on: Dec 12, 2015
 *      Author: Karol Dzitkowski
 */

#ifndef TIME_SERIES_READER_HPP_
#define TIME_SERIES_READER_HPP_

#include "time_series.hpp"
#include "file.hpp"
#include "ts_file_definition.hpp"

#include <boost/shared_ptr.hpp>

class TimeSeriesReader
{
public:
	TimeSeriesReader()
		: _lastFilePosition(0)
	{}
	virtual ~TimeSeriesReader() {}

public:
	SharedTimeSeriesPtr ReadFromCSV(
		File& file,
		CSVFileDefinition& definition,
		const int maxRows = INT32_MAX);

	SharedTimeSeriesPtr ReadFromBinary(
		File& file,
		BinaryFileDefinition& definition,
		const int maxRows = INT32_MAX);

private:
	size_t _lastFilePosition;
};

#endif /* TIME_SERIES_READER_HPP_ */
