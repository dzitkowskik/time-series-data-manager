#include "file_comparer.h"
#include "time_series.h"
#include "time_series_file_reader.h"
#include "time_series_file_writer.h"
#include <iostream>

//#define DDJ_TIME_SERIES_READER_USE_BOOST 1

TimeSeries<> GenerateFakeTimeSeries(std::string name, int size)
{
    TimeSeries<> result(name);
    for(int i = 0; i < size; i++)
    {
        result.InsertTime(i);
        result.InsertData((float)i * (i % 3));
    }
    return result;
}

int main(int argc, char* argv[])
{
    std::string testFileName = "/home/ghash/test.csv";
    auto ts1 = GenerateFakeTimeSeries("fake1", 1000);
    auto ts2 = GenerateFakeTimeSeries("fake2", 1000);
    OutputDefinition outputDefinition;
    TimeSeriesFileWriter writer(outputDefinition);
    writer.Write(testFileName, std::vector<TimeSeries<>> {ts1, ts2});
    InputDefinition inputDefinition;
    TimeSeriesFileReader reader(inputDefinition);
    auto ts = reader.Read(testFileName);

    if(ts[0].Equal(ts1)) std::cout << "OK 1" << std::endl;
    else std::cout << "FAIL 1" << std::endl;
    if(ts[1].Equal(ts2)) std::cout << "OK 2" << std::endl;
    else std::cout << "FAIL 2" << std::endl;

    std::cout << "DONE" << std::endl;
    return 0;
}