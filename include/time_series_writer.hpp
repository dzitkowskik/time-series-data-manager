//
// Created by Karol Dzitkowski on 20.12.15.
//

#ifndef TIME_SERIES_DATA_READER_TIME_SERIES_WRITER_H
#define TIME_SERIES_DATA_READER_TIME_SERIES_WRITER_H

#include "time_series.hpp"
#include "file.hpp"
#include "ts_file_definition.hpp"

class TimeSeriesWriter
{
public:
    TimeSeriesWriter()
            : _lastFilePosition(0)
    {}
    virtual ~TimeSeriesWriter() {}

public:
    void WriteToCSV(
            File& file,
            TimeSeries& series,
            CSVFileDefinition& definition);

    void WriteToBinary(
            File& file,
            TimeSeries series);

private:
    size_t _lastFilePosition;
};

#endif //TIME_SERIES_DATA_READER_TIME_SERIES_WRITER_H
