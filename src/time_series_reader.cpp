/*
 * time_series_reader.cpp
 *
 *  Created on: Dec 12, 2015
 *      Author: Karol Dzitkowski
 */

#include "time_series_reader.hpp"
#include "foreach_macro.hpp"
#include <iomanip>
#include <boost/lexical_cast.hpp>

std::vector<std::string> ReadHeader(std::ifstream& inFile, const CSVFileDefinition definition)
{
    std::vector<std::string> result;
    std::string headerLine;
    std::getline(inFile, headerLine);
    size_t position = 0;
    do
    {
        position = headerLine.find(definition.Separator);
        auto colName = headerLine.substr(0, position);
        headerLine.erase(0, position + definition.Separator.length());
        result.push_back(colName);
    } while(std::string::npos != position);

    return result;
}


template<typename DataType>
std::vector<TimeSeries<DataType>> TimeSeriesReader::ReadFromCSV(
        File& file, CSVFileDefinition definition, const int maxRows)
{
    std::vector<TimeSeries<DataType>> result;
    std::ifstream inputFile(file.GetPath(), std::ios::in);
    inputFile.seekg(_lastFilePosition, inputFile.beg);

    // GET HEADER
    std::vector<std::string> header;
    if(definition.UseCustomHeader)
        header = definition.Header;
    else
        header = ReadHeader(inputFile, definition);

    // Create time series with names from header
    for(int index = 0; index < header.size(); index++)
        if(index != definition.TimeValueIndex)
            result.push_back(TimeSeries<DataType>(header[index]));

    std::string line, token;
    size_t position = 0, i, j;
    int count = 0;
    while(std::getline(inputFile, line) && (count < maxRows))
    {
        i = 0, j = 0;  // column number
        do
        {
            position = line.find(definition.Separator);
            token = line.substr(0, position);
            line.erase(0, position + definition.Separator.length());

            if(j++ == definition.TimeValueIndex)
            {
                std::tm time;
                strptime(token.c_str(), "%c", &time);
                time.tm_isdst = -1;
                auto tt = mktime(&time);
                for (auto &ts : result)
                    ts.InsertTime(tt);
            }
            else result[i++].InsertData(boost::lexical_cast<DataType>(token));

        } while(position != std::string::npos);
        count++;
    }

    _lastFilePosition = inputFile.tellg();
    inputFile.close();
    return result;
}

template<typename DataType>
std::vector<TimeSeries<DataType>> TimeSeriesReader::ReadFromBinary(
        File& file,
        BinaryFileDefinition definition,
        const int maxRows)
{
    std::vector<TimeSeries<DataType>> result(definition.NoTimeSeries);
    size_t size = sizeof(time_t) + definition.NoTimeSeries * sizeof(DataType);
    char* data = new char[size];
    time_t* timePtr = reinterpret_cast<time_t*>(data);
    DataType* dataPtr = reinterpret_cast<DataType*>(data+sizeof(time_t));
    int rows = 0;
    while(-1 != file.ReadRaw(data, size) && rows++ < maxRows)
    {
        for(int i = 0; i < definition.NoTimeSeries; i++)
            result[i].Insert(std::make_pair(*timePtr, dataPtr[i]));
    }
    delete [] data;
    return result;
}

#define TS_READER_SPEC(X) \
	template std::vector<TimeSeries<X>> TimeSeriesReader::ReadFromCSV<X>(File&, CSVFileDefinition, const int); \
	template std::vector<TimeSeries<X>> TimeSeriesReader::ReadFromBinary<X>(File&, BinaryFileDefinition, const int);
FOR_EACH(TS_READER_SPEC, double, float, int, unsigned int, long, unsigned long, long long, unsigned long long)
