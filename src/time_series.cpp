//
// Created by Karol Dzitkowski on 19.10.15.
//

#include "time_series.hpp"
#include "foreach_macro.hpp"
#include <iomanip>
#include <boost/lexical_cast.hpp>

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
	template void TimeSeries<X>::WriteToCSV(File&, std::vector<TimeSeries<X>>, CSVFileDefinition); \
	template void TimeSeries<X>::WriteToBinary(File&, std::vector<TimeSeries<X>>);
FOR_EACH(TS_DATA_TYPE_SPEC, double, float, int, unsigned int, long, unsigned long, long long, unsigned long long)
