#include "time_series.h"
#include <gtest/gtest.h>

//#define DDJ_USE_BOOST 1

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

TEST(TimeSeries, ReadWrite_Data_ToFile)
{
    std::string testFileName = "/home/ghash/test.csv";
    const TimeSeries<> ts1 = GenerateFakeTimeSeries("fake1", 1000);
    const TimeSeries<> ts2 = GenerateFakeTimeSeries("fake2", 1000);
    TSFileDefinition fileDefinition;
    TimeSeries<> ts;
    File testFile(testFileName);
    ts.WriteManyToFile(testFile, std::vector<TimeSeries<>> {ts1, ts2}, fileDefinition);
    auto result = ts.ReadManyFromFile(testFile, fileDefinition);

    EXPECT_TRUE(result[0].Equal(ts1));
    EXPECT_EQ(result[1], ts2);
}
