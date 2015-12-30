/*
 * time_series_reader.cpp
 *
 *  Created on: Dec 12, 2015
 *      Author: Karol Dzitkowski
 */

#include "time_series_reader.hpp"
#include <iomanip>
#include <boost/make_shared.hpp>

std::vector<std::string> ReadHeader(std::ifstream& inFile, CSVFileDefinition& definition)
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

    definition.Header = result;
    return result;
}

SharedTimeSeriesPtr TimeSeriesReaderCSV::Read(
        File& file, const int maxRows)
{
    // Initialize time series
    auto result = boost::make_shared<TimeSeries>(file.GetPath());
    result->init(_definition.Columns);

    // Open file and set last position
    std::ifstream inputFile(file.GetPath(), std::ios::in);
    inputFile.seekg(_lastFilePosition, inputFile.beg);

    // Read header
    std::vector<std::string> header;
    if(_definition.HasHeader && _lastFilePosition == 0)
        header = ReadHeader(inputFile, _definition);
    else
        header = _definition.Header;
    result->setColumnNames(header);

    std::string line, token;
    size_t position = 0;
    int count = 0;
    while((count++ < maxRows) && std::getline(inputFile, line))
    {
        std::vector<std::string> record;
        do
        {
            position = line.find(_definition.Separator);
            token = line.substr(0, position);
            line.erase(0, position + _definition.Separator.length());
            record.push_back(token);
        } while(position != std::string::npos);
        result->addRecord(record);
        record.clear();
    }

    _lastFilePosition = inputFile.tellg();
    inputFile.close();

    return result;
}

SharedTimeSeriesPtr TimeSeriesReaderBinary::Read(
        File& file,
        const int maxRows)
{
    // Initialize time series
    auto result = boost::make_shared<TimeSeries>(file.GetPath());
    result->init(_definition.Columns);
    size_t size = result->getRecordSize();
    result->setColumnNames(_definition.Header);

    char* data = new char[size];
    int count = 0;

    while((count++ < maxRows) && (-1 != file.ReadRaw(data, size)))
        result->addRecord(data);

    delete [] data;
    return result;
}
