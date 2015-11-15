//
// Created by Karol Dzitkowski on 19.10.15.
//

#include "time_series.h"
#include "foreach_macro.h"
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
std::vector<TimeSeries<DataType>> TimeSeries<DataType>::ReadFromCSV(
        File& file, CSVFileDefinition definition, const int maxRows)
{
    std::vector<TimeSeries<DataType>> result;
    std::ifstream inputFile(file.GetPath(), std::ios::in);

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

    inputFile.close();
    return result;
}

void WriteLine(std::ofstream& ofstream, std::vector<std::string> lineItems, CSVFileDefinition definition)
{
    std::string line;
    for(int i = 0; i < lineItems.size(); i++)
    {
        line += lineItems[i];
        if(i < lineItems.size()-1) line += definition.Separator;
    }
    ofstream << line << std::endl;
}

template<typename DataType>
void TimeSeries<DataType>::WriteToCSV(
        File& file,
        std::vector<TimeSeries<DataType>> series,
        CSVFileDefinition definition)
{
    std::ofstream outFile(file.GetPath(), std::ios::out);

    //WRITE HEADER
    std::vector<std::string> header;
    if(definition.UseCustomHeader)
        header = definition.Header;
    else
    {
        for (auto &ts : series)
            header.push_back(ts.GetName());
        header.insert(header.begin()+definition.TimeValueIndex, "time");
    }
    WriteLine(outFile, header, definition);

    //WRITE DATA
    std::vector<std::string> lineItems;
    std::vector<typename std::vector<DataType>::iterator> dataIterators;
    for(auto& ts : series) dataIterators.push_back(ts.BeginData());

    for(auto timeIt = series[0].BeginTime(); timeIt != series[0].EndTime(); timeIt++)
    {
        lineItems.clear();
        for(int i = 0; i < dataIterators.size(); i++)
        {
            lineItems.push_back(boost::lexical_cast<std::string>(*dataIterators[i]++));
        }
        std::string timeString = ctime(&(*timeIt));
        timeString.erase(timeString.length()-1);
        lineItems.insert(lineItems.begin()+definition.TimeValueIndex, timeString);
        WriteLine(outFile, lineItems, definition);
    }

    outFile.close();
}

template<typename DataType>
std::vector<TimeSeries<DataType>> TimeSeries<DataType>::ReadFromBinary(
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

template<typename DataType>
void TimeSeries<DataType>::WriteToBinary(
        File& file,
        std::vector<TimeSeries<DataType>> series)
{
    size_t size = sizeof(time_t) + series.size()*sizeof(DataType);
    char* data = new char[size];
    time_t* timePtr = reinterpret_cast<time_t*>(data);
    DataType* dataPtr = reinterpret_cast<DataType*>(data+sizeof(time_t));

    std::vector<typename std::vector<DataType>::iterator> dataIterators;
    for(auto& ts : series) dataIterators.push_back(ts.BeginData());
    for(auto timeIt = series[0].BeginTime(); timeIt != series[0].EndTime(); timeIt++)
    {
        *timePtr = *timeIt;
        for(int i = 0; i < dataIterators.size(); i++)
            dataPtr[i] = *dataIterators[i]++;
        if(file.WriteRaw(data, size))
            throw std::runtime_error("Error while writting to a file");
    }
    delete [] data;
}


#define TS_DATA_TYPE_SPEC(X) \
	template std::vector<TimeSeries<X>> TimeSeries<X>::ReadFromCSV(File&, CSVFileDefinition, const int); \
	template void TimeSeries<X>::WriteToCSV(File&, std::vector<TimeSeries<X>>, CSVFileDefinition); \
	template std::vector<TimeSeries<X>> TimeSeries<X>::ReadFromBinary(File&, BinaryFileDefinition, const int); \
	template void TimeSeries<X>::WriteToBinary(File&, std::vector<TimeSeries<X>>);
FOR_EACH(TS_DATA_TYPE_SPEC, double, float, int, unsigned int, long, unsigned long, long long, unsigned long long)
