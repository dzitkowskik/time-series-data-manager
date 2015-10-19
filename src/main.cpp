#include "time_series.h"

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
    const TimeSeries<> ts1 = GenerateFakeTimeSeries("fake1", 1000);
    const TimeSeries<> ts2 = GenerateFakeTimeSeries("fake2", 1000);
    TSFileDefinition fileDefinition;
    TimeSeries<> ts;
    File testFile(testFileName);
    ts.WriteManyToFile(testFile, std::vector<TimeSeries<>> {ts1, ts2}, fileDefinition);
    auto result = ts.ReadManyFromFile(testFile, fileDefinition);
    if(result[0].Equal(ts1)) std::cout << "OK 1" << std::endl;
    else std::cout << "FAIL 1" << std::endl;
    if(result[1].Equal(ts2)) std::cout << "OK 2" << std::endl;
    else std::cout << "FAIL 2" << std::endl;
    std::cout << "DONE" << std::endl;
    return 0;
}