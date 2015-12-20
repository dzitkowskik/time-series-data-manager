//
// Created by Karol Dzitkowski on 19.10.15.
//

#include "time_series_writer.hpp"
#include <iomanip>
#include <boost/lexical_cast.hpp>

void WriteLine(
        std::ofstream& ofstream,
        std::vector<std::string> lineItems,
        CSVFileDefinition& definition)
{
    std::string line;
    for(int i = 0; i < lineItems.size(); i++)
    {
        line += lineItems[i];
        if(i < lineItems.size()-1) line += definition.Separator;
    }
    ofstream << line << std::endl;
}

void TimeSeriesWriter::WriteToCSV(
        File& file,
        TimeSeries& series,
        CSVFileDefinition& definition)
{
    std::ofstream outFile(file.GetPath(), std::ios::out);
    // Write header as column names
    if(definition.HasHeader)
        WriteLine(outFile, series.getColumnNames(), definition);

    for(size_t i = 0; i < series.getRecordsCnt(); i++)
        WriteLine(outFile, series.getRecordAsStrings(i), definition);

    outFile.close();
}

void TimeSeriesWriter::WriteToBinary(
        File& file,
        TimeSeries series)
{
    size_t size = series.getRecordSize();
    char* data = new char[size];

    for(size_t i = 0; i < series.getRecordsCnt(); i++)
    {
        size_t offset = 0;
        for(auto& rawData : series.getRawRecordData(i))
        {
            memcpy(data+offset, rawData.Data, rawData.Size);
            offset += rawData.Size;
        }
        if (file.WriteRaw(data, size))
            throw std::runtime_error("Error while writting to a file");
    }

    delete [] data;
}
